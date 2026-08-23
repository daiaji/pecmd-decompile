/* ====================================================================
 * core_resdecode.c — 资源数据解码器 ResDecode（B8a）
 *
 *   FUN_1400E7D58  @0x1400e7d58   资源解码（XOR 密钥 + 前缀剥离 + #code=）
 *
 * 语义：对 ScriptRunA 读入的资源数据解码：
 *   ps[0]=数据指针 ps[1]=长度(字节)；flags:
 *     bit0   : 置位时先做 "#code=" 前缀预检，否则直接尝试解码
 *     bit1   : 置位时收尾擦除三个临时缓冲（ZeroMem）
 *     bit2   : 解码尝试参数（0xfde9 回退时的模式位来源）
 *     0xff01 : 模式字（循环内追加 0x200 预检标记 / 0x1000 重编标记）
 *     >>16   : XOR 密钥 key16（低字节 keyLo 用于字节级操作）
 *   流程：前 6 字节按密钥试探解码 → 检测 UTF-16BE/LE 与脚本前缀
 *   （常量 @0x14012412c/@0x140124128/@0x140124130 内容 TODO(verify)）→
 *   剥离前缀/字节序转换 → 整块解码（FUN_1400E7994 @0x1400e7994）→
 *   尾部补 8 个密钥填充 + 终止符 → 扫描 "#code=N"(N=代码页)/"#!tN" →
 *   需要时按代码页二次转换 → 返回 spec。
 *   返回值：spec（0、0xfde9 或 "#code=" 后的数字；未解析保持原值）。
 * ==================================================================== */
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

#include "pecmd_defs.h"
#include "win32_stub.h"

/* 已有实现（core_exec2.c / core_string.c / core_thread.c / core_var3.c） */
extern void *PECMD_ReallocBuffer(void *old, int64_t size);                    /* @0x140063224 */
extern void FUN_14005B0B8(void *p);                                     /* @0x14005b0b8 */
extern bool FUN_1400C11C0(LPCWSTR *ps, int *out);                      /* @0x1400c11c0 */
extern int64_t *FUN_1400702F0(int64_t *out, const char *src, uint64_t len); /* @0x1400702f0 */
extern uint64_t FUN_14001B4F8(const WCHAR *buf, WCHAR ch);             /* @0x14001b4f8 */

/* 同批（core_scriptdep.c）：core_var3.c 已实现，此处仅声明供衔接核对 */
/* 未实现辅助（TODO(verify) 挂起，语义待核对） */
extern int32_t FUN_14005B184(const char *a, const char *b, int n);
extern void FUN_140060A74(uint8_t *buf, int len);
extern int32_t FUN_1400E7994(uint32_t spec, const uint8_t *src, int srclen,
                               uint16_t *dst, int dstcap, uint32_t key);
extern void PECMD_SkipEncByteToEol(uint8_t **pp, uint8_t key);
extern void FUN_1400E7098(uint8_t **pp, uint8_t key);
extern void PECMD_SkipEncWCharToEol(WCHAR **pp, uint16_t key);
extern void FUN_1400E70F4(WCHAR **pp, uint16_t key);
extern void PECMD_ResReenc(WCHAR *src, int64_t *ps, uint32_t flag,
                           uint16_t key); /* @0x140075c7c TODO(verify): 按代码页二次转换 */

/* ========== FUN_1400E7D58 @0x1400e7d58 ==========
 * 资源数据解码。返回 spec：0 / 0xfde9 / "#code=" 解析出的数字。
 */
uint32_t FUN_1400E7D58(int64_t *ps, uint32_t flags)
{
    uint32_t key = (uint32_t)(int32_t)flags >> 16;  /* 带符号右移取密钥 */
    uint16_t key16 = (uint16_t)(flags >> 16);
    uint8_t keyLo = (uint8_t)(flags >> 16);
    uint32_t fKeep = flags & 2;                     /* 收尾擦除标记 */
    uint32_t f4 = flags & 4;                        /* 解码回退模式位来源 */
    uint32_t mode;                                  /* 模式字 (local_res10[0]) */
    uint32_t code;                                  /* "#code=" 解析值槽 (local_res10[0]) */
    uint32_t spec = 0;                              /* 返回 spec (uVar13) */
    uint32_t specOut = 0xffffffff;                  /* 循环退出判断 (local_res20) */
    uint32_t cpArg = 0xff000000;                    /* "#!tN" 的 N, 代码页候选 (local_78) */
    int8_t mark = -1;                               /* "#code=" 处临时 '-' 标记 (local_res8) */
    int64_t work;                                   /* 初值 len/2; "#code=" 路径暂存 ANSI 副本 */
    uint8_t *data;                                  /* 数据起点 (puVar5/puVar18) */
    uint8_t *pCur;                                  /* 当前字节扫描指针 (local_98) */
    uint8_t *pNew = NULL;                           /* 解码缓冲 (local_80) */
    uint8_t *pOld = NULL;                           /* 交换缓冲 (local_60) */
    WCHAR *pHead = NULL;                            /* 前 6 字节探测缓冲 (local_88) */
    LPCWSTR pNum;                                   /* "#code=" 数字串副本 */
    int len, iVar3, skipN;
    bool bSkip = false;

    len = (int)ps[1];
    if (len < 1) return 0;
    data = (uint8_t *)(intptr_t)ps[0];
    pCur = data;
    PECMD_AllocWStringBuffer(&pHead, 0x28);
    work = len / 2;
    /* 前 3 个 WCHAR 按密钥试探解码 */
    pHead[0] = ((uint16_t *)data)[0] ^ key16;
    pHead[1] = ((uint16_t *)data)[1] ^ key16;
    pHead[2] = ((uint16_t *)data)[2] ^ key16;
    /* UTF-16BE（低字节 0 高字节非 0）或 2 字节前缀常量 @0x14012412c */
    if ((len > 1 && (uint8_t)pHead[0] == 0 && (uint8_t)((char *)pHead)[1] != 0) ||
        FUN_14005B184((const char *)pHead, "\x3a\x3a", 2) == 0) {
        /* TODO(verify): 常量 @0x14012412c 内容待核对（疑似 "::"） */
        FUN_140060A74(data, len);
        bSkip = true;
    }
    /* UTF-16LE（低字节非 0 高字节 0） */
    if ((uint8_t)pHead[0] != 0 && (uint8_t)((char *)pHead)[1] == 0) {
        bSkip = true;
    }
    /* 2 字节前缀常量 @0x140124128：剥离 2 字节 */
    if (FUN_14005B184((const char *)pHead, "\x3a\x3a", 2) == 0) {
        /* TODO(verify): 常量 @0x140124128 内容待核对（疑似 "::"） */
        bSkip = true;
        skipN = 2;
    }
    if (bSkip) {
        goto skip_prefix;
    }
    skipN = 3;
    /* 3 字节前缀常量 @0x140124130：强制 spec=0xfde9 并剥离 3 字节 */
    if (FUN_14005B184((const char *)pHead, "\x40\x3a\x3a", 3) == 0) {
        /* TODO(verify): 常量 @0x140124130 内容待核对（疑似 "@::"） */
        spec = 0xfde9;
        goto skip_prefix;
    }
    if ((flags & 1) == 0) goto no_precheck;
    /* bit0 置位：按字节检查数据开头的 "@::"/"#!" 前缀（16 位值 0x3a3a 等） */
    {
        uint8_t *pb = (uint8_t *)pHead;
        if (pb[0] == 0x40) pb++;
        if (pb[0] == 0x3a && pb[1] == 0x3a) {
            PECMD_SkipEncByteToEol(&pCur, keyLo);
            FUN_1400E7098(&pCur, keyLo);
        }
    }
    if (((uint8_t)*pCur ^ keyLo) == 0x40) pCur++;
    if (((int8_t)*pCur ^ (int32_t)(key & 0xffff)) == 0x23 &&
        ((int8_t)pCur[1] ^ (int32_t)(key & 0xffff)) == 0x21) {
        PECMD_SkipEncByteToEol(&pCur, keyLo);
        FUN_1400E7098(&pCur, keyLo);
    }
    {
        uint8_t *pb = pCur;
        if (((uint8_t)*pCur ^ keyLo) == 0x40) pb++;
        if (((uint8_t)*pb ^ keyLo) != 0x23) goto no_precheck;
        /* 拷贝 '#' 处起的 <=0x20 字节到 pHead（字节级 XOR），比较 "#code=" */
        skipN = (int)(intptr_t)data + (int)ps[1] - (int)(intptr_t)pb;
        if (skipN > 0x20) skipN = 0x20;
        for (int i = 0; i < skipN; i++) ((uint8_t *)pHead)[i] = pb[i] ^ keyLo;
        if (StrCmpNIA((LPCSTR)pHead, "#code=", 6) != 0) goto no_precheck;
        /* 命中 "#code=..."：原数据 '#' 后一字节临时改写为 '-'（收尾恢复 'c'） */
        f4 = 0xffffffff;
        mode = (flags & 0xff01) | 0x200;
        pb[1] = keyLo ^ 0x2d;
        mark = 1;
        /* 复制数字串（pHead+6 字节）为 WCHAR，解析 N 与可选 "#!tN" */
        FUN_1400702F0((int64_t *)&work, (const char *)pHead + 6, (uint64_t)-1);
        pNum = (LPCWSTR)(intptr_t)work;
        FUN_1400C11C0(&pNum, (int *)&f4);
        if ((int32_t)f4 >= 0) spec = f4;
        specOut = f4;
        if (((*pNum | 0x20) == 0x74)) {
            pNum++;
            FUN_1400C11C0(&pNum, (int *)&cpArg);
        }
        PECMD_FreeStrBuf((WCHAR **)&work);
    }
    goto main_decode;

skip_prefix:
    /* 剥离前缀/字节序数据：长度缩短、头部前移、尾部补密钥低字节 */
    len -= skipN;
    ps[2] = len;
    ps[1] = len;
    iVar3 = len / 2;
    if (skipN != 0) {
        /* 原 PECMD_MemMoveSafe（memmove 等价） */
        memmove((uint8_t *)(intptr_t)ps[0], (uint8_t *)(intptr_t)ps[0] + skipN,
                (size_t)(len - skipN) + 6);
    }
    ((uint8_t *)(intptr_t)ps[0])[len] = keyLo;
    data = (uint8_t *)(intptr_t)ps[0];
    mode = flags & 0xff01;
    goto postproc;

no_precheck:
    /* bit0 清 0：不做 "#code=" 预检，直接处理 */
    len = len + (int)((intptr_t)ps[0] - (intptr_t)data);
    if (3 < len) {
        uint8_t b0l = (uint8_t)pHead[0], b0h = (uint8_t)((char *)pHead)[1];
        if (b0l == 0) {
            if (b0h == 0) goto raw_decode;
            FUN_140060A74((uint8_t *)(intptr_t)ps[0], len);
        } else if (b0h != 0 || (uint8_t)pHead[1] == 0) {
            goto raw_decode;
        }
        /* 小端文本：跳过解码，直接后处理（长度取初值 len/2） */
        iVar3 = (int)work;
        mode = flags & 0xff01;
        spec = 0;
        goto postproc;
    }
raw_decode:
    /* 两次解码尝试：先自动模式，失败再 0xfde9 强制模式 */
    skipN = len * 3;
    pNew = (uint8_t *)PECMD_ReallocBuffer(pNew, (int64_t)(skipN + 3) * 2 + 2);
    iVar3 = FUN_1400E7994(0, (const uint8_t *)(intptr_t)ps[0], len, (uint16_t *)pNew, skipN,
                            (key & 0xffff) | 0x1000000);
    if (iVar3 < 1) {
        bool b = (f4 != 0);
        iVar3 = FUN_1400E7994(0xfde9, (const uint8_t *)(intptr_t)ps[0], len,
                                (uint16_t *)pNew, skipN,
                                ((uint32_t)(-(int32_t)b) & 0x40000) | (key & 0xffff));
        if (iVar3 < 1) {
            spec = 0;
            goto main_decode;
        }
        spec = 0xfde9;
    }
    ((uint16_t *)pNew)[iVar3] = 0;
    pOld = (uint8_t *)(intptr_t)ps[0];
    ps[0] = (int64_t)(intptr_t)pNew;
    pNew = pOld;
    mode = flags & 0xff01;
    goto postproc;

main_decode:
    /* 主解码：交换缓冲后整块解码到新缓冲 */
    {
        uint8_t *t = pOld;
        pOld = (uint8_t *)(intptr_t)ps[0];
        ps[0] = (int64_t)(intptr_t)t;
    }
    ps[0] = (int64_t)(intptr_t)PECMD_ReallocBuffer((void *)(intptr_t)ps[0],
                                             (int64_t)(len * 3 + 3) * 2 + 2);
    iVar3 = FUN_1400E7994(spec, data, len, (uint16_t *)(intptr_t)ps[0], len * 2,
                            key & 0xffff);

postproc:
    /* 后处理：尾部补密钥填充 + 终止符，扫描 "#code=N" / "#!tN" */
    for (;;) {
        ps[0] = (int64_t)(intptr_t)PECMD_ReallocBuffer((void *)(intptr_t)ps[0],
                                                 (int64_t)(iVar3 + 9) * 2 + 2);
        for (int j = 0; j < 8; j++) ((uint16_t *)(intptr_t)ps[0])[iVar3 + j] = key16;
        ((uint16_t *)(intptr_t)ps[0])[iVar3 + 8] = 0;
        if ((int32_t)mode < 0x10) {
            WCHAR *w = (WCHAR *)(intptr_t)ps[0];
            if ((w[0] ^ key16) == 0x40) w++;
            if (((w[0] ^ key16) == 0x3a) && ((w[1] ^ key16) == 0x3a)) {
                PECMD_SkipEncWCharToEol(&w, key16);
                FUN_1400E70F4(&w, key16);
            }
            w = (WCHAR *)(intptr_t)ps[0];
            if ((w[0] ^ key16) == 0x40) w++;
            if (((w[0] ^ key16) == 0x23) && ((w[1] ^ key16) == 0x21)) {
                PECMD_SkipEncWCharToEol(&w, key16);
                FUN_1400E70F4(&w, key16);
            }
            /* 拷贝 '#' 处起的 <=0x20 个 WCHAR 到 pHead（WCHAR 级 XOR），比较 "#code=" */
            {
                WCHAR *w2 = (WCHAR *)(intptr_t)ps[0];
                if ((w2[0] ^ key16) == 0x40) w2++;
                if ((w2[0] ^ key16) == 0x23) {
                    int n = iVar3 - (int)(w2 - (WCHAR *)(intptr_t)ps[0]);
                    if (n > 0x20) n = 0x20;
                    int i;
                    for (i = 0; i < n; i++) pHead[i] = w2[i] ^ key16;
                    pHead[i] = 0;
                    if (StrCmpNIW(pHead, WSTR("#code="), 6) == 0) {
                        LPCWSTR p = pHead + 6;
                        code = 0xffffffff;
                        FUN_1400C11C0(&p, (int *)&code);
                        if ((int32_t)code >= 0) spec = code;
                        specOut = code;
                        if (((p[0] | 0x20) == 0x74) || (p[0] == L'*')) {
                            p++;
                            FUN_1400C11C0(&p, (int *)&cpArg);
                        }
                    }
                }
            }
        }
        /* 退出：已重编、无 "#!tN" 代码页、代码页与系统不同、或已按系统代码页处理过 */
        if (((int32_t)mode > 0xfff) || ((int32_t)cpArg < 1) || (cpArg != g_SysCodePage) ||
            (specOut == g_SysCodePage)) {
            break;
        }
        /* 按代码页二次转换后重算长度（填充的密钥供 FUN_14001B4F8 定位终止） */
        {
            WCHAR *w = (WCHAR *)(intptr_t)ps[0];
            ps[0] = 0;
            PECMD_ResReenc(w, ps, (uint32_t)(cpArg == 0x3b6), key16 & 0xffff);
            iVar3 = (int)FUN_14001B4F8((WCHAR *)(intptr_t)ps[0], key16);
            mode |= 0x1000;
            PECMD_FreeStrBuf((WCHAR **)&w);
        }
    }
    /* 收尾：恢复 "#code=" 的 'c'，回填长度，可选擦除临时缓冲 */
    if (mark >= 0) {
        ((uint16_t *)(intptr_t)ps[0])[mark] = key16 ^ 99;
    }
    ps[1] = (int64_t)iVar3 * 2;
    ps[2] = (int64_t)iVar3 * 2;
    if (fKeep != 0) {
        FUN_14005B0B8(pHead);
        FUN_14005B0B8(pNew);
        FUN_14005B0B8(pOld);
    }
    PECMD_FreeStrBuf((WCHAR **)&pOld);
    PECMD_FreeStrBuf((WCHAR **)&pNew);
    PECMD_FreeStrBuf((WCHAR **)&pHead);
    return spec;
}
