/* Helper stubs for DONE-classified functions still missing definitions. */
#include <stdint.h>
#include <stdbool.h>

#include "pecmd_defs.h"

extern int FUN_140067B78(WCHAR **pp, uint64_t *out);   /* @0x140067b78 */
extern WCHAR **FUN_14005B154(WCHAR **pp);              /* @0x14005b154 */

/* --- helper symbols referenced by restored bodies below --- */
extern void *PECMD_GrowByteBuffer(void **ps, int64_t len);          /* @0x140063424 分配清零槽数组 */
extern bool  FUN_1400c11c0(WCHAR **pp, int *out);                   /* @0x1400c11c0 */
extern void *PECMD_VarLookup(void *script, LPCWSTR name, void *scope,
                             int namelen, void **found);            /* @0x140018978 */
extern int64_t *FUN_14001e5b0(int64_t *script, LPCWSTR name, LPCWSTR text,
                              int mode, int64_t flag);              /* @0x14001e5b0 */
extern void PECMD_VarTruncateUpdate(void *node, const void *src,
                                    uint64_t len);                   /* @0x14005b708 */
extern void PECMD_VarWriteValueCap(WCHAR **pval, uint64_t *pcap,
                                   const void *src, int64_t len);    /* @0x140066224 */

extern WCHAR *PECMD_AllocString(WCHAR **ps, int64_t count);        /* @0x140063720 串扩容 */
extern WCHAR *PECMD_StrCopyW(WCHAR **ps, LPCWSTR src, int64_t len); /* @0x140063888 定长拷贝 */
extern WCHAR *FUN_140063620(WCHAR **out);                           /* @0x140063620 分配引用串容器 */
extern void  FUN_14005b104(void *ps);                               /* @0x14005b104 释放引用串容器 */
extern void  FUN_14007bf44(int64_t *script, LPCWSTR src, int64_t *out,
                           int mode, int flag);                      /* @0x14007bf44 */

/* @0x14005b374 size=44 — 从 *pp 起跳过字符, 直到撞上 '\0'/ch1/ch2 (行切分定界) */
void FUN_14005b374(WCHAR **pp, WCHAR ch1, WCHAR ch2)
{
    WCHAR *p;

    p = *pp;
    if (p != NULL) {
        while (*p != L'\0' && (uint16_t)*p != (uint16_t)ch1 &&
               (uint16_t)*p != (uint16_t)ch2) {
            *pp = ++p;
        }
    }
}

LARGE_INTEGER PECMD_SetFilePointer(HANDLE h, LARGE_INTEGER dist, DWORD method)
{
    /* @0x14005c674 移动文件指针并返回新位置 */
    int64_t newPos = -1;
    SetFilePointerEx(h, dist.QuadPart, &newPos, method);
    return (LARGE_INTEGER)newPos;
}

/* @0x1400675b8 size=145 — 按定界符切分 token (跳过前导/尾随空白), 写入 dst 容器 */
void FUN_1400675b8(WCHAR **src, WCHAR **dst, int16_t delim)
{
    WCHAR *pStart;
    WCHAR *p;
    WCHAR *pEnd;
    int iLen;
    WCHAR wc;

    FUN_14005B154(src);          /* 跳过前导空白 */
    pStart = *src;
    p = pStart;
    wc = *p;
    while (wc != L'\0' && (uint16_t)wc != (uint16_t)delim) {
        p++;
        *src = p;
        wc = *p;
    }
    /* 归去尾随空白 (pEnd 停在最后一个非空白字符) */
    pEnd = *src;
    do {
        pEnd--;
        if (pEnd < pStart)
            break;
    } while (((uint16_t)*pEnd > 8 && (uint16_t)*pEnd < 0xe) || *pEnd == L' ');
    iLen = (int)((pEnd - pStart) + 1);
    /* 拷贝 iLen+1 宽字符(可能多读1), 再在 [iLen] 落 0 */
    PECMD_StrCopyW(dst, pStart, (int64_t)(iLen + 1));
    (*dst)[iLen] = L'\0';
    return;
}

/* @0x1400676e4 size=99 — 按定界符切分 token (不跳空白), 写入 dst 容器 */
void FUN_1400676e4(WCHAR **src, WCHAR **dst, int16_t delim)
{
    WCHAR *pStart;
    WCHAR *p;
    int iVar3;
    WCHAR wc;

    pStart = *src;
    p = pStart;
    wc = *p;
    while (wc != L'\0' && (uint16_t)wc != (uint16_t)delim) {
        p++;
        *src = p;
        wc = *p;
    }
    iVar3 = (int)(p - pStart);
    /* 拷贝 iVar3+1 宽字符(可能多读1), 再在 [iVar3] 落 0 */
    PECMD_StrCopyW(dst, pStart, (int64_t)(iVar3 + 1));
    (*dst)[iVar3] = L'\0';
    return;
}

bool PECMD_ParseNumSkipWs(WCHAR **pp, uint64_t *out)
{
    /* @0x140067cf4 size=41 解析数字（成功则跳过空白） */
    bool ok = FUN_140067B78(pp, out) > 0;

    if (ok) {
        FUN_14005B154(pp);
    }
    return ok;
}

/* @0x14006923c size=153 — LPCSTR 字节串 → 宽十六进制串 "0x%02X " 每字节 */
int FUN_14006923c(WCHAR **out, LPCSTR src, int len)
{
    LPWSTR pW;
    int i;

    if (len < 1) {
        len = lstrlenA(src);
    }
    PECMD_AllocString(out, (int64_t)(len * 5 + 1));
    pW = *out;
    if (0 < len) {
        for (i = 0; i < len; i++) {
            wsprintfW(pW, WSTR("0x%02X "), (unsigned char)src[i]);
            pW += 5;
        }
        pW[-1] = L'\0';
    }
    return 1;
}

uint64_t FUN_1400692d8(void)
{
    /* UNIMPLEMENTED @0xFUN_1400692d8 — decompile-failed, body 未还原 */
/* @0x1400692d8 size=58 */
    return 0;
}

/* @0x140069314 WCHAR串→字节缓冲(仅保留十进制数字, 每数字1字节), 返回字节数 */
int PECMD_WcharToByteDigits(void *out, LPCWSTR src)
{
    uint8_t *dst;
    WCHAR *p;
    WCHAR *pEnd;
    int len;
    int n;

    len = lstrlenW(src);
    PECMD_GrowByteBuffer((void **)out, (int64_t)(len + 4));
    dst = *(uint8_t **)out;
    p = (WCHAR *)src;
    pEnd = p + len;
    if (len >= 1) {
        while (p < pEnd) {
            if ((uint16_t)((uint16_t)*p - 0x30) < 10) {
                if (pEnd <= p)
                    break;
                if (!FUN_1400c11c0(&p, &n))
                    break;
                *dst = (uint8_t)n;
                dst++;
            } else {
                p++;
            }
        }
        dst[2] = 0;
        dst[1] = 0;
        *dst = 0;
        len = (int)(dst - *(uint8_t **)out);
    }
    return len;
}

/* @0x1400693c0 size=161 — GBK↔BIG5 区码表重映射 (LCMapStringA, locale 0x20804):
 *   len=-1 试探求目标长 → 扩容 *out 字节缓冲 → 正式映射回填并补 '\0', 返回缓冲指针。
 *   isBIG5=0 → LCMAP_UPPERCASE(0x2000000); isBIG5≠0 → 叠加 LCMAP_LOWERCASE(0x4000000)。 */
uint64_t FUN_1400693c0(LPCSTR src, void **out, uint32_t isBIG5)
{
    LPSTR lpDestStr;
    DWORD cchDest;
    DWORD dwMapFlags;

    dwMapFlags = (DWORD)(-(uint32_t)(isBIG5 != 0) & 0x2000000u) + 0x2000000u;
    cchDest = LCMapStringA(0x20804, dwMapFlags, src, -1, NULL, 0);
    PECMD_GrowByteBuffer(out, (int64_t)(cchDest + 1));
    lpDestStr = (LPSTR)*out;
    LCMapStringA(0x20804, dwMapFlags, src, -1, lpDestStr, cchDest);
    lpDestStr[cchDest] = '\0';
    return (uint64_t)(uintptr_t)*out;
}

/* @0x140075148 按行写变量 (查表/建新项 + 写值/截断 + 关键段保护) */
void PECMD_VarWriteLine(void *script, LPCWSTR key, LPCWSTR data, int64_t len,
                   int64_t *pkey, char mode)
{
    int64_t *plVar3;
    int64_t *plVar4;
    WCHAR *pWVar5;
    LPCWSTR pWVar7;
    int64_t lVar6;
    int iVar8;

    iVar8 = (int)len;
    lVar6 = (int64_t)iVar8;
    EnterCriticalSection(&g_csInit);
    for (pWVar5 = (WCHAR *)key; (*pWVar5 == L'&' || *pWVar5 == L':'); pWVar5++)
        ;
    if (*pWVar5 != L'\0') {
        plVar3 = (int64_t *)PECMD_VarLookup(script, key, (void *)0, -1, (void **)0);
        if (plVar3 == (int64_t *)0) {
            if (mode != '\0')
                iVar8 = 0;
            pWVar7 = data;
            if (mode != '\0')
                pWVar7 = g_szEmpty;
            plVar4 = FUN_14001e5b0(script, pWVar5, pWVar7, -1, (int64_t)iVar8);
            if (plVar4 == (int64_t *)0)
                goto LAB_done;
        } else {
            uint8_t bVar2 = *(uint8_t *)((int64_t)plVar3 + 0x1f) & 0xc0;
            if (bVar2 == 0xc0) {
                if ((int64_t)(plVar3[3] & 0x3fffffffffffffff) < lVar6) {
                    len = plVar3[3] & 0x3fffffffffffffff;
                }
                PECMD_VarTruncateUpdate((void *)plVar3, (const void *)data,
                                        (uint64_t)(int)len);
                goto LAB_done;
            }
            plVar4 = plVar3;
            if (bVar2 != 0) {
                plVar3[1] = 0;
            }
        }
        if (mode == '\0') {
            if (plVar3 != (int64_t *)0) {
                PECMD_VarWriteValueCap((WCHAR **)(plVar4 + 1), (uint64_t *)(plVar4 + 3),
                                       (const void *)data, lVar6);
            }
        } else {
            int64_t lVar1 = *pkey;
            *pkey = plVar4[1];
            plVar4[1] = lVar1;
            plVar4[3] = lVar6;
        }
    }
LAB_done:
    LeaveCriticalSection(&g_csInit);
    return;
}

/* @0x140075c7c size=797 — BIG5 字符名表解码/重排: 按 uVar19 切行, PRNG(FUN_14005dff4) 选行,
 * 行内 XOR uVar19 解密 + LCMapStringW 大小写映射, 写回 *param_2 输出串。
 * SKIP: 依赖 FUN_140063224(输出串扩容)、LCMapStringW 全项目无定义 (link_stubs.c 仅备
 * LCMapStringA), FUN_14001d78c 亦仅注释注明为 memcpy 库替换; SUB168/SUB164 实为
 * 64 位取模伪影可普通书写, 但缺符号硬阻断链接, 保持桩不误报. */
uint64_t FUN_140075c7c(void)
{
    return 0;
}

/* @0x14007f6e4 size=127 — 按分隔符取 token 赋值变量, 推进游标并返回其+2 */
int64_t *FUN_14007f6e4(int64_t *cursor, WCHAR **pp, uint16_t sep, int flag)
{
    WCHAR *tok = NULL;

    FUN_140063620(&tok);
    FUN_1400676e4(pp, &tok, (int16_t)sep);
    FUN_14007bf44((int64_t *)(cursor[1]), tok, (int64_t *)cursor, 0, flag);
    cursor[2] = cursor[0];
    FUN_14005b104(&tok);
    return cursor + 2;
}

uint64_t FUN_1400a9a84(void)
{
    /* UNIMPLEMENTED @0xFUN_1400a9a84 — decompile-failed, body 未还原 */
/* @0x1400a9a84 size=36 */
    return 0;
}
