/* ====================================================================
 * core_execline.c - 脚本行展开器（ExecLine2 族）
 *
 *   FUN_14007BF44   @0x14007bf44   分发器：按 script 标志选展开路径
 *   FUN_14007BDA8  @0x14007bda8   递归变量展开（循环直至稳定）
 *   FUN_14007A224  @0x14007a224   完整展开（变量/参数/环境变量）
 *   PECMD_ExpandEnvVars    @0x14007af60   变量展开核心（带回调标志）
 *
 * 语义：%name% 展开为变量/环境变量值；%数字、%*、%@、%# 命令行参数；
 *   %name:~s,l% 截取；%&name% 根表；%^name% 强制；%% 转义；%name?spec% 格式化。
 * 构建器（6 槽）与格式化函数由 core_strbld.c 提供。
 * ==================================================================== */
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <string.h>

#include "pecmd_defs.h"
#include "win32_stub.h"

/* core_strbld.c 构建器/格式化 */
extern void PECMD_StrBldInitWide(void *s[6], WCHAR **out, int *count, WCHAR **cur, WCHAR **end,
                                 WCHAR **base, WCHAR **limit); /* @0x14006d7e8 */
extern void PECMD_TextBufReserve(void *s[6], int need);        /* @0x14006d880 */
extern void PECMD_StrBldGrowWide(void *s[6]);                  /* @0x14006d92c */
extern WCHAR *PECMD_FormatTypedMemValue(int64_t node, uint64_t *lenOut, WCHAR *spec, WCHAR *dst,
                                        WCHAR *width);  /* @0x14006d9d0 */
extern void PECMD_FormatU64Dec(WCHAR *dst, uint64_t v); /* @0x1400e6d74 */

/* 核心分配/变量辅助 */
extern uint8_t *PECMD_VarLookup(void *script, LPCWSTR name, void *scope, int64_t len,
                                void **out);           /* @0x140018978 */
extern bool FUN_1400C1194(LPCWSTR *ps, uint64_t *out); /* @0x1400c1194 */
extern int64_t FUN_14005E04C(void);                    /* @0x14005e04c */


/* 前向声明（本文件内互相调用） */
int64_t FUN_14007BDA8(void *script, WCHAR *line, WCHAR **out, int mode, uint8_t opt);
int64_t FUN_14007A224(void *script, WCHAR *line, WCHAR **out, int mode, uint8_t opt);
extern int64_t PECMD_ExpandEnvVars(void *script, WCHAR *line, WCHAR **out, int mode, uint8_t opt,
                                   char *flagout);

/* 数字判断：c 是 '0'-'9'（Ghidra 写法 (ushort)(c+0xFFD0)<10） */
static inline int XDigit(WCHAR c)
{
    return (uint16_t)(c + 0xFFD0) < 10;
}

/* ========== FUN_14007BF44 @0x14007bf44 ==========
 * 分发器：script+0xda==0 且 script+0xd 低 4 位==0 时走递归变量展开，
 * 否则走完整展开。
 */
void FUN_14007BF44(void *script, WCHAR *line, WCHAR **out, int mode, uint8_t opt)
{
    if (*(char *)((char *)script + 0xda) == '\0' &&
        (*(uint8_t *)((char *)script + 0xd) & 0xf) == 0) {
        FUN_14007BDA8(script, line, out, mode, opt);
    }
    else {
        FUN_14007A224(script, line, out, mode, opt);
    }
}

/* ========== FUN_14007BDA8 @0x14007bda8 ==========
 * 递归展开：反复调用 XpandVar（最多 5 轮）直到结果稳定或含 % 标志置位；
 * opt!=0 时再用全量 opt 补一轮（最多 2 轮）。全程持锁。
 */
int64_t FUN_14007BDA8(void *script, WCHAR *line, WCHAR **out, int mode, uint8_t opt)
{
    uint8_t b5;
    char flag;
    int cnt, i;
    WCHAR *in, *chain;
    int64_t r;

    EnterCriticalSection(&g_csInit);
    b5 = opt & 0xfe;
    flag = '\0';
    chain = NULL;
    r = PECMD_ExpandEnvVars(script, line, out, mode, b5, &flag);
    cnt = 4;
    in = NULL;
    do {
        if (flag != '\0' || lstrcmpW(*out, line) == 0)
            break;
        in = *out;
        *out = chain;
        r = PECMD_ExpandEnvVars(script, in, out, mode, b5, &flag);
        cnt--;
        chain = in;
    } while (cnt >= 0);
    if (opt != 0) {
        WCHAR *s = *out;
        *out = chain;
        chain = s;
        r = PECMD_ExpandEnvVars(script, s, out, mode, b5 | opt, &flag);
        i = 1;
        do {
            in = s;
            if (flag != '\0' || lstrcmpW(*out, s) == 0)
                break;
            in = *out;
            *out = s;
            r = PECMD_ExpandEnvVars(script, in, out, mode, b5, &flag);
            i--;
            s = in;
        } while (i >= 0);
    }
    chain = in;
    PECMD_FreeStrBuf(&chain);
    LeaveCriticalSection(&g_csInit);
    return r;
}

/* ========== FUN_14007A224 @0x14007a224 ==========
 * 完整展开：单遍扫描 %变量/%参数/%环境变量 就地展开。
 * opt&1 时参数展开可用；opt&0x40 或 mode<0 关闭引号标志。
 */
int64_t FUN_14007A224(void *script, WCHAR *line, WCHAR **out, int mode, uint8_t opt)
{
    bool neg, bVar2;
    uint8_t xflag;
    int count;
    WCHAR *cur, *end, *base, *limit;
    WCHAR *envBuf, *lpBuffer, *inP;
    size_t envCap;
    void *xb[6];
    int64_t *sp;
    int64_t lastArg, iVar10, iVar3, lVar11, lVar12, uVar5;
    char cVar17;
    WCHAR *p15, *p16, *nameAll, *nameEnv, *p6;
    WCHAR *lp, *val, *vp;
    int64_t start, len;
    uint8_t *node;
    WCHAR ch_joined;

    neg = (mode < 0);
    xflag = *(uint8_t *)((char *)script + 0xd) & 0x11;
    if (neg || (opt & 0x40))
        xflag = 0;

    sp = (int64_t *)script;
    count = mode;
    cur = NULL;
    end = NULL;
    base = NULL;
    limit = NULL;
    envBuf = NULL;
    envCap = 0x50;
    lastArg = -1;
    iVar10 = -1;
    iVar3 = -1;
    lVar11 = 0;
    lVar12 = 0;
    uVar5 = 0;
    bVar2 = false;
    cVar17 = 0;
    node = NULL;
    ch_joined = 0;
    p6 = NULL;

    EnterCriticalSection(&g_csInit);
    PECMD_AllocWStringBuffer((WCHAR **)&envBuf, 0x50);
    lpBuffer = envBuf;
    PECMD_StrBldInitWide(xb, out, &count, &cur, &end, &base, &limit);
    inP = line;

    for (;;) {
        iVar10 = iVar3;
        /* R24(D 活体取证 r24_hang_triage_002_004_038): dc:77750 环头单游标复载 ——
         * % 族分支只推进 line(如 line=pw16+1 / line=inP+2), 环头检查仍读 inP ⇒
         * 非空 %var% 展开后 inP 永停 '%', StrBldGrowWide 每迭代 +0x400 自旋膨胀
         * (002/004/038 实测 33-34MB); 空值 %var% 因 goto var_lookup 内 inP 直接推进
         * 而不挂。模板照抄 PECMD_ExpandEnvVars 同款 H1(dc:78742), 每个分支均已
         * 先行推进 line 或 inP, 此处复载语义安全。 */
        inP = line;
        if (end <= cur || *inP == L'\0') {
            *cur = L'\0';
            PECMD_AllocString(out, (int64_t)(cur - base) + 2);
            lVar11 = (int64_t)(cur - base);
            PECMD_FreeStrBuf(&envBuf);
            LeaveCriticalSection(&g_csInit);
            return lVar11 >> 1;
        }
        PECMD_StrBldGrowWide(xb);
        uVar5 = 0;
        iVar3 = iVar10;
        if (*inP != L'%') {
            iVar10 = 0x3dd;
            for (;;) {
                WCHAR ch = *inP;
                line = inP;
                if (ch == L'\0' || ch == L'%')
                    break;
                *cur = ch;
                inP++;
                cur++;
                iVar10--;
                line = inP;
                if (iVar10 < 0)
                    break;
            }
            continue;
        }
        /* % 处理 */
        if (opt & 1) {
            LPCWSTR arg;
            if (inP[1] == L'%') {
                *cur = L'%';
                cur++;
                line = inP + 2;
                continue;
            }
            iVar3 = lastArg;
            if (inP[1] == L'#') {
                line = inP + 2;
                if (sp[0xd] != 0) {
                    WCHAR *q = *(WCHAR **)(sp[0xd] + 0x10 + (int)sp[0xc] * 8);
                    WCHAR ch = *q;
                    while (ch != L'\0') {
                        *cur = ch;
                        q++;
                        cur++;
                        iVar3 = iVar10;
                        ch = *q;
                    }
                }
                continue;
            }
            if (inP[1] == L'*') {
                lVar11 = sp[0xd];
                line = inP + 2;
                if (lVar11 == 0)
                    continue;
                iVar10 = (int)sp[0xc];
            }
            else if (inP[1] == L'@') {
                lVar11 = sp[0xd];
                line = inP + 2;
                if (lVar11 == 0)
                    continue;
                iVar10 = (int)sp[0xc] + 1;
            }
            else {
                int iVar14 = 0;
                if ((uint16_t)(inP[1] + 0xFFD0) > 9 &&
                    ((uint16_t)inP[1] < 0x7e || (uint16_t)(inP[2] + 0xFFD0) > 9)) {
                    goto var_expand;
                }
                if ((uint16_t)inP[1] > 0x7d) {
                    iVar14 = 0x25;
                    inP++;
                }
                while (inP++, XDigit(*inP))
                    uVar5 = (uint64_t)(*inP - L'0') + uVar5 * 10;
                line = inP;
                if ((uint64_t)uVar5 < (uint64_t)(int64_t)(int)sp[0xc] && sp[0xd] != 0) {
                    arg = *(LPCWSTR *)(sp[0xd] + uVar5 * 8);
                    iVar3 = lstrlenW(arg);
                    lVar12 = (int64_t)iVar3;
                    if (iVar14 != 0 && *arg == L'"') {
                        if (arg[lVar12 - 1] == L'"')
                            lVar12--;
                        arg++;
                        lVar12--;
                    }
                    goto copy_common;
                }
                continue;
            }
            inP += 2;
            arg = *(LPCWSTR *)(lVar11 + iVar10 * 8);
            lastArg = iVar10;
            iVar3 = lstrlenW(arg);
            lVar12 = (int64_t)iVar3;
            goto copy_common;
        }

    var_expand: /* 变量/环境变量展开 */
    {
        WCHAR *p6v;
        WCHAR ch;
        p15 = inP + 1;
        ch = *p15;
        p6v = p15;
        while (ch == L'&') {
            p6v++;
            ch = *p6v;
        }
        iVar3 = StrCmpNIW(p6v, WSTR("RANDOM%"), 7);
        lVar11 = 0;
        if (iVar3 == 0) {
            uVar5 = FUN_14005E04C() & 0x7fffffffffffffffLL;
        }
        else {
            iVar3 = StrCmpNIW(p6v, WSTR("__THIS%"), 7);
            if (iVar3 != 0) {
                WCHAR *pw16 = p15;
                ch = *pw16;
                while ((ch != L'\0' && (lpBuffer = envBuf, ch != L'%')) && (ch != L'=') &&
                       ((uint16_t)ch < 9 || (uint16_t)ch > 0xd) && ch != L' ') {
                    pw16++;
                    ch = *pw16;
                }
                p16 = pw16;
                uVar5 = (int64_t)(pw16 - inP) >> 1;
                if (*pw16 == L'%') {
                    if (neg || (*p15 != L'&' && (xflag == 0 || *p15 == L'^'))) {
                        line = pw16 + 1;
                        if (*p15 == L'^') {
                            p15 = inP + 2;
                            lVar11 = 8;
                        }
                        cVar17 = (char)lVar11;
                        nameAll = p15;
                        nameEnv = p15;
                        goto env_expand;
                    }
                    line = pw16 + 1;
                    nameAll = p15;
                    nameEnv = p15;
                    if (*p15 == L'&') {
                        p15 = inP + 2;
                        bVar2 = true;
                        nameEnv = p15;
                    }
                    goto var_lookup;
                }
                /* 未闭合 %：原样复制 %... 段 */
                PECMD_TextBufReserve(xb, (int)uVar5 + 2);
                while (uVar5 > 0) {
                    WCHAR w = *inP;
                    inP++;
                    *cur = w;
                    cur++;
                    uVar5--;
                }
                line = pw16;
                continue;
            }
            uVar5 = (int64_t)script + 0x75bd036; /* __THIS 值 TODO(verify) */
        }
        /* RANDOM%/__THIS%：格式化输出 */
        {
            WCHAR a8[81];
            inP = p6v + 7;
            a8[0] = L'\0';
            PECMD_FormatU64Dec(a8, (uint64_t)uVar5);
            iVar3 = lstrlenW(a8);
            lVar12 = (int64_t)iVar3;
            if (iVar3 < 1)
                lVar12 = lVar11;
            p6 = a8;
            goto copy_common;
        }
    }

    copy_common: /* 复制 lVar12 个字符（p6 指向源） */
        PECMD_TextBufReserve(xb, (int)lVar12 + 2);
        iVar3 = lastArg;
        if (iVar10 >= 0) {
            lVar11 = iVar10;
            iVar10 = -1;
            p6 = (WCHAR *)*(LPCWSTR *)(sp[0xd] + lVar11 * 8);
            lastArg = -1;
            iVar3 = lastArg;
        }
        while (lVar12-- > 0) {
            WCHAR w = *p6;
            p6++;
            *cur = w;
            cur++;
            iVar3 = iVar10;
        }
        continue;

    var_lookup: /* %name% 变量查找（:~ 截取 / ?spec 格式化） */
    {
        WCHAR *q = nameAll;
        WCHAR *p7 = NULL, *p13 = NULL;
        start = 0;
        len = 0x7fffffffffffffffLL;
        for (;;) {
            p7 = q;
            q = NULL;
            lp = p7;
            if (p16 <= p7)
                break;
            {
                WCHAR ch = *p7;
                q = NULL;
                if (ch == L'\0')
                    break;
                if (ch == L':' && p7[1] == L'~') {
                    *p7 = L'\0';
                    node = PECMD_VarLookup(script, nameAll, NULL, -1, NULL);
                    *lp = L':';
                    lp += 2;
                    FUN_1400C1194((LPCWSTR *)&lp, (uint64_t *)&start);
                    if (*lp == L',' || *lp == L';' || *lp == L':') {
                        lp++;
                        FUN_1400C1194((LPCWSTR *)&lp, (uint64_t *)&len);
                    }
                    ch_joined = *lp;
                    goto joined;
                }
                lp = p7 + 1;
                q = lp;
                if (ch == L'?')
                    break;
            }
        }
        *p7 = L'\0';
        for (;;) {
            WCHAR ch = *lp;
            q = p7;
            if (ch == L'\0')
                break;
            lp++;
            q = lp;
            if (ch == L':')
                break;
        }
        p13 = p7;
        if (*p16 != L'\0' && p16[1] == L'%' && (p7 = p16 + 2, *p7 == L'd')) {
            for (lp = p7; *p7 != L'\0'; p7++) {
                if (*p7 == L'%') {
                    line = p7 + 1;
                    p16 = p7;
                    break;
                }
            }
        }
        *p16 = L'\0';
        node = PECMD_VarLookup(script, nameAll, NULL, -1, NULL);
        *p16 = L'%';
        if (p13 != NULL) {
            *p13 = L'?';
            p13++;
        }
        if (node == 0)
            goto not_found;
        {
            WCHAR f8[96];
            p6 = PECMD_FormatTypedMemValue((int64_t)node, (uint64_t *)&uVar5, p13, f8, q);
            goto fmt_copy;
        }
    }

    joined: /* :~ 截取命中后的值处理 */
        if (ch_joined == L'\0')
            goto joined_end;
        if (ch_joined == L'%') {
            line = lp + 1;
            goto joined_end;
        }
        lp++;
        ch_joined = *lp;
        goto joined;

    joined_end:
        if (node == 0)
            goto not_found;
        {
            int64_t vlen = *(int64_t *)((char *)node + 0x18) & 0x3fffffffffffffffLL;
            val = *(WCHAR **)((char *)node + 8);
            vp = val;
            if (vlen < start)
                start = vlen;
            if (start >= 0 && len >= 0) {
                if (start < 1) {
                    uVar5 = len;
                    goto fmt_copy;
                }
                goto joined_r;
            }
            {
                WCHAR w = *val;
                while (w != L'\0' && vp < val + vlen) {
                    vp++;
                    w = *vp;
                }
            }
            lVar11 = (int64_t)(vp - val) >> 1;
            if (start < 0) {
                start += lVar11;
                if (start < 0)
                    start = 0;
            }
            if (len < 0) {
                len += (lVar11 - start);
                if (len < 0)
                    len = 0;
            }
            p6 = val + start;
            uVar5 = len;
            goto fmt_copy;
        }

    joined_r: /* 截取边界钳制 */
        if (val + start <= vp) {
            p6 = val + start;
            uVar5 = len;
            goto fmt_copy;
        }
        if (*vp == L'\0') {
            len = 0;
            start = (int64_t)(vp - val) >> 1;
            p6 = val + start;
            uVar5 = len;
            goto fmt_copy;
        }
        vp++;
        goto joined_r;

    fmt_copy: /* 复制 uVar5 个字符（p6 指向源） */
        PECMD_TextBufReserve(xb, (int)uVar5 + 2);
        while (uVar5-- > 0) {
            WCHAR w = *p6;
            p6++;
            *cur = w;
            cur++;
            if (w == L'\0')
                break;
        }
        continue;

    not_found: /* 变量未命中：&/^ 前缀跳过，否则转环境变量 */
        iVar3 = lastArg;
        if (bVar2 || (char)lVar11 != 0)
            continue;
        cVar17 = 1;

    env_expand: /* 环境变量展开 */
    {
        WCHAR *e5, *e6, *mark = NULL;
        int64_t estart = 0;
        int64_t elen = 0x7fffffffffffffffLL;
        DWORD rlen;
        uint64_t vlen;
        e5 = nameEnv;
        e6 = e5;
        *lpBuffer = L'\0';
        if (nameEnv < p16) {
            for (;;) {
                e6 = e5;
                if (*e5 == L'\0')
                    break;
                if (*e5 == L':' && e5[1] == L'~') {
                    *e5 = L'\0';
                    e6 = e5 + 2;
                    FUN_1400C1194((LPCWSTR *)&e6, (uint64_t *)&estart);
                    mark = e5;
                    if (*e6 == L',' || *e6 == L';' || *e6 == L':') {
                        e6++;
                        FUN_1400C1194((LPCWSTR *)&e6, (uint64_t *)&elen);
                    }
                    break;
                }
                e5++;
                e6 = e5;
                if (p16 <= e5)
                    break;
            }
        }
        *p16 = L'\0';
        rlen = GetEnvironmentVariableW(nameEnv, lpBuffer, (DWORD)envCap);
        vlen = (uint64_t)rlen;
        if (vlen != 0 && *lpBuffer == L'\0') {
            envCap = (size_t)rlen + 100;
            PECMD_AllocWStringBuffer((WCHAR **)&envBuf, (int64_t)envCap);
            lpBuffer = envBuf;
            rlen = GetEnvironmentVariableW(nameEnv, lpBuffer, (DWORD)envCap);
            vlen = (uint64_t)rlen;
        }
        *p16 = L'%';
        uVar5 = vlen;
        if (mark == NULL) {
            p6 = lpBuffer;
            if (*lpBuffer == L'\0') {
                iVar3 = lastArg;
                if (cVar17 != 0 || (lVar11 = 1, neg))
                    continue;
                goto var_lookup;
            }
        }
        else {
            *mark = L':';
            if (vlen < (uint64_t)estart)
                estart = (int64_t)vlen;
            if (vlen < (uint64_t)elen)
                elen = (int64_t)vlen;
            if (estart < 0) {
                if (estart < -(int64_t)vlen)
                    estart = -(int64_t)vlen;
                estart += (int64_t)vlen;
            }
            {
                int64_t remain = (int64_t)vlen - estart;
                if (elen < 0) {
                    uVar5 = remain + elen;
                    if (uVar5 < 0)
                        uVar5 = 0;
                }
                else {
                    if (remain < (int64_t)vlen)
                        uVar5 = (uint64_t)remain;
                    if (elen < (int64_t)uVar5)
                        uVar5 = (uint64_t)elen;
                }
            }
            p6 = lpBuffer + estart;
        }
        PECMD_TextBufReserve(xb, (int)uVar5 + 2);
        while (uVar5-- > 0) {
            WCHAR w = *p6;
            p6++;
            *cur = w;
            cur++;
        }
        continue;
    }
    }
}

/* ========== PECMD_ExpandEnvVars @0x14007af60 ==========
 * 变量展开核心：与 ExecLine2B 结构相同，差异：
 *   - flagout 回调：环境变量值含 % 或结果含 % 时置位
 *   - 环境变量用 SetLastError(0)+GetLastError()==0xcb 判断未找到，
 *     未找到时原样复制原始段
 *   - (char)opt<0 清引号标志（而非 opt&0x40）
 *   - 无 lastArg 状态（%@ 直接取下一参数）
 */
int64_t PECMD_ExpandEnvVars(void *script, WCHAR *line, WCHAR **out, int mode, uint8_t opt,
                            char *flagout)
{
    bool neg, bVar2;
    uint8_t xflag;
    char fl0, local_268;
    int count;
    WCHAR *cur, *end, *base, *limit;
    WCHAR *envBuf, *lpBuffer, *inP, *p6;
    size_t envCap;
    void *xb[6];
    int64_t *sp;
    int64_t iVar3, iVar10, lVar7, uVar14, local_230;
    uint8_t envHit;
    WCHAR *p13, *p11, *nameStart;
    WCHAR *raw_src, *raw_dst; /* dc:78569-78570 的 pWVar13/pWVar8，函数域声明：三个入口共用 */
    WCHAR *lp, *val, *vp;
    int64_t start, len;
    uint8_t *node;
    WCHAR ch_joined;

    neg = (mode < 0);
    xflag = *(uint8_t *)((char *)script + 0xd) & 0x11;
    if (neg || (char)opt < 0)
        xflag = 0;
    fl0 = '\0';
    if (flagout != NULL)
        fl0 = *flagout;

    sp = (int64_t *)script;
    count = mode;
    cur = NULL;
    end = NULL;
    base = NULL;
    limit = NULL;
    envBuf = NULL;
    envCap = 0x50;
    iVar3 = 0;
    iVar10 = 0;
    lVar7 = 0;
    uVar14 = 0;
    local_230 = 0;
    envHit = 0; /* TODO(verify): 反编译初值来自未定义寄存器 */
    bVar2 = false;
    local_268 = 0;
    node = NULL;
    ch_joined = 0;
    p6 = NULL;
    nameStart = NULL;

    PECMD_AllocWStringBuffer((WCHAR **)&envBuf, 0x50);
    lpBuffer = envBuf;
    PECMD_StrBldInitWide(xb, out, &count, &cur, &end, &base, &limit);
    inP = line;

    for (;;) {
        WCHAR *p = inP;
        WCHAR *pctPos;
        if (end <= cur || *p == L'\0') {
            *cur = L'\0';
            if (envHit == 0 && flagout != NULL && *(char *)((char *)script + 0xda) == '\0') {
                *flagout = '\x01';
            }
            PECMD_AllocString(out, (int64_t)(cur - base) + 2);
            lVar7 = (int64_t)(cur - base);
            PECMD_FreeStrBuf(&envBuf);
            return lVar7 >> 1;
        }
        PECMD_StrBldGrowWide(xb);
        uVar14 = 0;
        if (*p != L'%') {
            /* 普通字符复制段 */
            iVar10 = 0x3dd;
            for (;;) {
                WCHAR ch = *p;
                if (ch == L'\0' || ch == L'%')
                    break;
                *cur = ch;
                p++;
                cur++;
                iVar10--;
                if (iVar10 < 0)
                    break;
            }
            inP = p;
            continue;
        }
        pctPos = p;
        if ((opt & 1) == 0)
            goto var_b38c;
        /* 参数展开（%#、%*、%@、%数字） */
        if (p[1] == L'%') {
            *cur = L'%';
            cur++;
            inP = p + 2;
            continue;
        }
        if (p[1] == L'#') {
            inP = p + 2;
            if (sp[0xd] != 0) {
                WCHAR *q = *(WCHAR **)(sp[0xd] + 0x10 + (int)sp[0xc] * 8);
                WCHAR ch = *q;
                while (ch != L'\0') {
                    *cur = ch;
                    q++;
                    cur++;
                    ch = *q;
                }
            }
            continue;
        }
        if (p[1] == L'*') {
            inP = p + 2;
            if (sp[0xd] != 0) {
                LPCWSTR arg = *(LPCWSTR *)(sp[0xd] + (int)sp[0xc] * 8);
                iVar10 = (int64_t)lstrlenW(arg);
                PECMD_TextBufReserve(xb, (int)iVar10 + 2);
                while (iVar10-- > 0) {
                    *cur = *arg;
                    arg++;
                    cur++;
                }
            }
            continue;
        }
        if (p[1] == L'@') {
            inP = p + 2;
            if (sp[0xd] != 0) {
                LPCWSTR arg = *(LPCWSTR *)(sp[0xd] + 8 + (int)sp[0xc] * 8);
                iVar10 = (int64_t)lstrlenW(arg);
                PECMD_TextBufReserve(xb, (int)iVar10 + 2);
                while (iVar10-- > 0) {
                    *cur = *arg;
                    arg++;
                    cur++;
                }
            }
            continue;
        }
        /* %数字 */
        if ((uint16_t)(p[1] + 0xFFD0) > 9 &&
            ((uint16_t)p[1] < 0x7e || (uint16_t)(p[2] + 0xFFD0) > 9)) {
            goto var_b38c;
        }
        {
            int i10 = 0;
            inP = p;
            if ((uint16_t)p[1] > 0x7d) {
                i10 = 0x25;
                inP = p + 1;
            }
            while (inP++, XDigit(*inP))
                uVar14 = (uint64_t)(*inP - L'0') + uVar14 * 10;
            if ((uint64_t)uVar14 < (uint64_t)(int64_t)(int)sp[0xc] && sp[0xd] != 0) {
                LPCWSTR arg = *(LPCWSTR *)(sp[0xd] + uVar14 * 8);
                iVar3 = (int64_t)lstrlenW(arg);
                uVar14 = (uint64_t)iVar3;
                if (i10 != 0 && *arg == L'"') {
                    if (arg[uVar14 - 1] == L'"')
                        uVar14--;
                    uVar14--;
                    arg++;
                }
                PECMD_TextBufReserve(xb, (int)uVar14 + 2);
                while (uVar14-- > 0) {
                    *cur = *arg;
                    arg++;
                    cur++;
                }
            }
            continue;
        }

    var_b38c: /* 变量/环境变量展开 */
    {
        WCHAR *p5;
        WCHAR ch;
        p11 = p + 1;
        ch = *p11;
        p5 = p11;
        while (ch == L'&') {
            p5++;
            ch = *p5;
        }
        iVar10 = StrCmpNIW(p5, WSTR("RANDOM%"), 7);
        if (iVar10 != 0) {
            iVar10 = StrCmpNIW(p5, WSTR("__THIS%"), 7);
            if (iVar10 != 0) {
                p13 = p11;
                ch = *p13;
                while (ch != L'\0' && ch != L'%' && ch != L'=')
                {
                    p13++;
                    ch = *p13; /* 原文每次重读(曾漏→%VAR% 行死循环, windbg 实锤) */
                }
                local_230 = (int64_t)(int)(char)xflag;
                local_268 = '\0';
                iVar10 = (int64_t)(p13 - p) >> 1;
                if (*p13 == L'%') {
                    nameStart = p11;
                    if (neg || (*p11 != L'&' && (xflag == 0 || *p11 == L'^'))) {
                        inP = p13 + 1;
                        if (*p11 != L'^')
                            goto env_b55d;
                        p11 = p + 2;
                        local_268 = '\b';
                        nameStart = p11;
                        goto env_b55d;
                    }
                    inP = p13 + 1;
                    bVar2 = false;
                    if (*p11 == L'&') {
                        p11 = p + 2;
                        local_230 = 1;
                    }
                    goto var_b6ee;
                }
                /* H3 修复(v2): dc:78427-78428 未闭合名走 else 直跳拷贝序言 bc3c，
                 * 载荷已就位——pWVar13=扫描终点(p13)、pWVar8='%'-起点(pctPos)、
                 * iVar10=:823 的全跨度；拷至 NUL 自然终止。
                 * 原移植裸跳缺 src/dst 装载(未初始化指针 AV 根因)；
                 * v1 误经 b961 守卫链(dc:78423 属 ? 特殊名子路径)致未闭合段原地自旋。 */
                raw_src = p13;
                raw_dst = pctPos;
                goto copy_raw;
            }
            uVar14 = (uint64_t)((int64_t)script + 0x75bd036); /* __THIS TODO(verify) */
        }
        else {
            uVar14 = (uint64_t)(FUN_14005E04C() & 0x7fffffffffffffffLL);
        }
        /* RANDOM%/__THIS%：格式化输出 */
        {
            WCHAR a8[81];
            a8[0] = L'\0';
            PECMD_FormatU64Dec(a8, uVar14);
            iVar10 = (int64_t)lstrlenW(a8);
            uVar14 = (uint64_t)iVar10;
            if (iVar10 < 1)
                uVar14 = 0;
            PECMD_TextBufReserve(xb, (int)uVar14 + 2);
            {
                WCHAR *src = a8;
                while (uVar14-- > 0) {
                    *cur = *src;
                    src++;
                    cur++;
                }
            }
            inP = p5 + 7;
            continue;
        }
    }

    var_b6ee: /* %name% 变量查找 */
    {
        WCHAR *q = nameStart;
        WCHAR *p7 = NULL, *p15 = NULL;
        int via_b91f = 0; /* R25-e(031/039 %A% 深水定案): dc:78377-78426 两种退出 —
                             pWVar13<=pWVar6 / nameStart 空 → goto LAB_14007b91f
                             (跳过 pWVar15=pWVar6 → spec=NULL → FormatTypedMemValue
                             直接取字符串); 仅 '?' 终止走 pWVar15=pWVar6(spec 格式化)。
                             msvc 原无条件 p15=p7 → %B% 场景 spec="?" → 展开 "0x70"。 */
        start = 0;
        len = 0x7fffffffffffffffLL;
        for (;;) {
            p7 = q;
            q = NULL;
            p15 = NULL;
            lp = p7;
            if (p13 <= p7) {
                via_b91f = 1;
                break;
            }
            {
                WCHAR ch = *p7;
                q = NULL;
                p15 = NULL;
                if (ch == L'\0') {
                    via_b91f = 1;
                    break;
                }
                if (ch == L':' && p7[1] == L'~') {
                    *p7 = L'\0';
                    node = PECMD_VarLookup(script, nameStart, NULL, -1, NULL);
                    *lp = L':';
                    lp += 2;
                    FUN_1400C1194((LPCWSTR *)&lp, (uint64_t *)&start);
                    if (*lp == L',' || *lp == L';' || *lp == L':') {
                        lp++;
                        FUN_1400C1194((LPCWSTR *)&lp, (uint64_t *)&len);
                    }
                    ch_joined = *lp;
                    goto joined7;
                }
                lp = p7 + 1;
                q = lp;
                if (ch == L'?')
                    break;
            }
        }
        *p7 = L'\0';
        if (via_b91f == 0) {
            for (;;) {
                WCHAR ch = *lp;
                q = p7;
                if (ch == L'\0')
                    break;
                lp++;
                q = lp;
                if (ch == L':')
                    break;
            }
            p15 = p7;
            if (*p13 != L'\0' && p13[1] == L'%' && (p7 = p13 + 2, *p7 == L'd')) {
                for (lp = p7; *p7 != L'\0'; p7++) {
                    if (*p7 == L'%') {
                        inP = p7 + 1;
                        p13 = p7;
                        break;
                    }
                }
            }
        }
        *p13 = L'\0';
        node = PECMD_VarLookup(script, nameStart, NULL, -1, NULL);
        *p13 = L'%';
        if (p15 != NULL) {
            *p15 = L'?';
            p15++;
        }
        if (node == 0)
            goto b961;
        {
            WCHAR f8[96];
            p6 = PECMD_FormatTypedMemValue((int64_t)node, (uint64_t *)&uVar14, p15, f8, q);
            goto ba34;
        }
    }

    ba34: /* 复制 uVar14 个字符（p6 指向源） */
        PECMD_TextBufReserve(xb, (int)uVar14 + 2);
        while (uVar14-- > 0) {
            WCHAR w = *p6;
            p6++;
            *cur = w;
            cur++;
            if (w == L'\0')
                break;
        }
        continue;

    joined7: /* :~ 截取命中后的值处理 */
        if (ch_joined == L'\0')
            goto b81b;
        if (ch_joined == L'%') {
            inP = lp + 1;
            goto b81b;
        }
        lp++;
        ch_joined = *lp;
        goto joined7;

    b81b:
        if (node == 0)
            goto b961;
        {
            int64_t vlen = *(int64_t *)((char *)node + 0x18) & 0x3fffffffffffffffLL;
            int64_t len2;
            val = *(WCHAR **)((char *)node + 8);
            vp = val;
            if (vlen < start)
                start = vlen;
            len2 = len;
            if (vlen < len)
                len2 = vlen;
            if (start >= 0 && len2 >= 0) {
                if (start < 1) {
                    uVar14 = (uint64_t)len2;
                    goto ba34;
                }
                goto joined_r2;
            }
            {
                WCHAR w = *val;
                while (w != L'\0' && vp < val + vlen) {
                    vp++;
                    w = *vp;
                }
            }
            lVar7 = (int64_t)(vp - val) >> 1;
            if (start < 0) {
                start += lVar7;
                if (start < 0)
                    start = 0;
            }
            if (len2 < 0) {
                len2 += (lVar7 - start);
                if (len2 < 0)
                    len2 = 0;
            }
            p6 = val + start;
            uVar14 = (uint64_t)len2;
            goto ba34;
        }

    joined_r2: /* 截取边界钳制 */
        if (val + start <= vp) {
            p6 = val + start;
            uVar14 = (uint64_t)len;
            goto ba34;
        }
        if (*vp == L'\0') {
            len = 0;
            start = (int64_t)(vp - val) >> 1;
            p6 = val + start;
            uVar14 = 0;
            goto ba34;
        }
        vp++;
        goto joined_r2;

    b961: /* 变量未命中 */
        if (local_230 != 0)
            continue;
        if (bVar2) {
            if (xflag == 1)
                continue;
            /* H2 修复: dc:78569-78571 原型由 b961 自带 src/dst 装载后直跳拷贝序言；
               原移植既无装载(未初始化指针)又曾跳前多减一次 inP(双重递减)，双错叠加 */
            raw_src = inP - 1;
            raw_dst = pctPos;
            goto copy_raw;
        }
        local_268 = '\x01';
        goto env_b55d;

    env_b55d: /* 环境变量展开 */
    {
        WCHAR *e5, *e6, *mark = NULL;
        int64_t estart = 0;
        int64_t elen = 0x7fffffffffffffffLL;
        DWORD err = 0xcb, rlen = 0;
        e5 = p11;
        for (;;) {
            if (p13 <= e5 || *e5 == L'\0')
                break;
            if (*e5 == L':' && e5[1] == L'~')
                break;
            e5++;
        }
        if (e5 < p13 && *e5 == L':') {
            *e5 = L'\0';
            e6 = e5 + 2;
            FUN_1400C1194((LPCWSTR *)&e6, (uint64_t *)&estart);
            mark = e5;
            if (*e6 == L',' || *e6 == L';' || *e6 == L':') {
                e6++;
                FUN_1400C1194((LPCWSTR *)&e6, (uint64_t *)&elen);
            }
        }
        if (fl0 == '\0') {
            SetLastError(0);
            *p13 = L'\0';
            rlen = GetEnvironmentVariableW(p11, lpBuffer, (DWORD)envCap);
            if ((int)rlen > 0 && *lpBuffer == L'\0') {
                envCap = (size_t)rlen + 100;
                PECMD_AllocWStringBuffer((WCHAR **)&envBuf, (int64_t)envCap);
                SetLastError(0);
                lpBuffer = envBuf;
                rlen = GetEnvironmentVariableW(p11, lpBuffer, (DWORD)envCap);
            }
            *p13 = L'%';
            err = GetLastError();
        }
        iVar3 = (int64_t)(int32_t)rlen;
        if (mark == NULL) {
            if (iVar3 < 1 && err == 0xcb) {
                if (local_268 != '\0' || (bVar2 = true, neg))
                    goto bab5;
                goto var_b6ee;
            }
            if (flagout != NULL && *(char *)((char *)script + 0xda) == '\0' &&
                StrChrW(lpBuffer, L'%') != NULL) {
                *flagout = '\x01';
            }
            p6 = lpBuffer;
            envHit = (uint8_t)(envHit & 0xff);
            if (iVar3 > 0)
                envHit = 1;
        }
        else {
            uVar14 = (uint64_t)iVar3;
            *mark = L':';
            if ((uint64_t)uVar14 < (uint64_t)estart)
                estart = (int64_t)uVar14;
            if ((uint64_t)uVar14 < (uint64_t)elen)
                elen = (int64_t)uVar14;
            if (estart < 0) {
                if (estart < -(int64_t)iVar3)
                    estart = -(int64_t)iVar3;
                estart += (int64_t)uVar14;
            }
            iVar10 = (int64_t)uVar14 - estart;
            if (elen < 0) {
                iVar3 = elen + iVar10;
                if (iVar3 < 0)
                    iVar3 = 0;
            }
            else {
                if (iVar10 < (int64_t)uVar14)
                    iVar3 = iVar10;
                if (elen < iVar3)
                    iVar3 = elen;
            }
            p6 = lpBuffer + estart;
        }
        PECMD_TextBufReserve(xb, (int)iVar3 + 2);
        while (iVar3-- > 0) {
            WCHAR w = *p6;
            p6++;
            *cur = w;
            cur++;
        }
        continue;
    }

    bab5: /* 未找到：原样复制原始段 */
        if (xflag != 1) {
            raw_src = inP - 1;
            raw_dst = pctPos;
        copy_raw:
            PECMD_TextBufReserve(xb, (int)iVar10 + 3);
            if (*raw_src == L'%') {
                WCHAR t = raw_src[1];
                if (t == L'%' || t == L'*' || t == L'@' || t == L'#' || t == L'~' || XDigit(t)) {
                    raw_src++;
                    iVar10++;
                }
            }
            while (iVar10-- > 0) {
                WCHAR w = *raw_dst;
                raw_dst++;
                *cur = w;
                cur++;
            }
            inP = raw_src; /* H1: dc:78742 循环头 param_2=pWVar13 强制同步主游标(零长拷贝也同步) */
        }
        continue;
    }
}
