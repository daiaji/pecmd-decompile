/* Helper stubs for DONE-classified functions still missing definitions. */
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include "pecmd_defs.h"

extern int FUN_140067B78(WCHAR **pp, uint64_t *out);   /* @0x140067b78 */
extern WCHAR **FUN_14005B154(WCHAR **pp);              /* @0x14005b154 */

/* --- helper symbols referenced by restored bodies below --- */
extern void *PECMD_GrowByteBuffer(void **ps, int64_t len);          /* @0x140063424 分配清零槽数组 */
extern bool  PECMD_ParseHexOrDecBool(WCHAR **pp, int *out);                   /* @0x1400c11c0 */
extern uint64_t PECMD_EvalParenStripped(WCHAR **pp, uint64_t *out);          /* @0x1400745c8 解析数值/括号表达式 */
extern void *PECMD_VarLookup(void *script, LPCWSTR name, void *scope,
                             int namelen, void **found);            /* @0x140018978 */
extern int64_t *PECMD_AddVarDefault(int64_t *script, LPCWSTR name, LPCWSTR text,
                              int mode, int64_t flag);              /* @0x14001e5b0 */
extern void PECMD_VarTruncateUpdate(void *node, const void *src,
                                    uint64_t len);                   /* @0x14005b708 */
extern void PECMD_VarWriteValueCap(WCHAR **pval, uint64_t *pcap,
                                   const void *src, int64_t len);    /* @0x140066224 */

extern WCHAR *PECMD_AllocString(WCHAR **ps, int64_t count);        /* @0x140063720 串扩容 */
extern WCHAR *PECMD_StrCopyW(WCHAR **ps, LPCWSTR src, int64_t len); /* @0x140063888 定长拷贝 */
extern WCHAR *PECMD_AllocStrSlot(WCHAR **out);                           /* @0x140063620 分配引用串容器 */
extern void  FUN_14005b104(void *ps);                               /* @0x14005b104 释放引用串容器 */
extern void  PECMD_ExpandVarDispatch(int64_t *script, LPCWSTR src, int64_t *out,
                           int mode, int flag);                      /* @0x14007bf44 */

/* ---- PECMD_DecodeBig5NameTable (BIG5 字符名表解码/重排) 依赖 ---- */
extern uint32_t FUN_14001b4f8(int16_t *s, int16_t ch);              /* @0x14001b4f8 定位字符下标 */
extern void PECMD_InitTableSlots(void **pdata, void **pend, int *pcount, int size); /* @0x140063a6c 表初始化 */
extern void PECMD_VectorAppendGen(void **pdata, void **pend, int64_t *pcount,
                          void *tmp, int size, int grow);           /* @0x1400639f0 表追加行 */
extern WCHAR *FUN_140063694(WCHAR **ps, int64_t count);             /* @0x140063694 串扩容 */
extern uint64_t FUN_14005dff4(void);                                /* @0x14005dff4 PRNG */
extern void *FUN_140063224(void *ps, int64_t len);                  /* @0x140063224 输出串扩容 */
extern void FUN_14005b0b8(void *p);                                 /* @0x14005b0b8 临时缓冲复位 */

/* @0x14005b374 size=44 — 从 *pp 起跳过字符, 直到撞上 '\0'/ch1/ch2 (行切分定界) */
void PECMD_SkipUntilDelim(WCHAR **pp, WCHAR ch1, WCHAR ch2)
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
void PECMD_SplitTokenTrimWs(WCHAR **src, WCHAR **dst, int16_t delim)
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
void PECMD_ExtractTokenByDelim(WCHAR **src, WCHAR **dst, int16_t delim)
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
int PECMD_BytesToHexStr(WCHAR **out, LPCSTR src, int len)
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

/* @0x1400692d8 size=58 — WCHAR 串 → 十六进制串包装: len<1 先取串长,
 * 再以 len*2 字节长度调 PECMD_BytesToHexStr(@0x14006923c) 转 "0x%02X " 宽串 */
void FUN_1400692d8(WCHAR **ps, LPCWSTR src, int len)
{
    if (len < 1) {
        len = lstrlenW(src);
    }
    PECMD_BytesToHexStr(ps, (LPCSTR)src, len * 2);
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
                if (!PECMD_ParseHexOrDecBool(&p, &n))
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
uint64_t PECMD_MapStringByLocale(LPCSTR src, void **out, uint32_t isBIG5)
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
            plVar4 = PECMD_AddVarDefault(script, pWVar5, pWVar7, -1, (int64_t)iVar8);
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

/* @0x140075c7c size=797 — BIG5 字符名表解码/重排: 以 uVar19(参4) 定位节首(puVar10),
 * 节内行以 (uVar19^10) 为分隔符经 PECMD_VectorAppendGen 收集入 0x28 字节行表(行串指针+行长),
 * PRNG(FUN_14005dff4) 随机抽未处理行, 行串 XOR uVar19 解密, ASCII 前缀直拷 + 余段
 * LCMapStringW(zh-CN) 映射, 再 XOR 回密存入行表输出槽; 循环至全表处理完, 按表序级联
 * 写入 *param_2 并在尾部附 uVar19 封口返回。SUB168/SUB164 为 64 位取模伪影, 已按
 * uint64 取模书写。 */
uint64_t PECMD_DecodeBig5NameTable(uint16_t *param_1, WCHAR **param_2, int param_3, int param_4)
{
    uint16_t uVar19;
    uint64_t uVar4;
    uint64_t uVar7;
    uint64_t uVar8;
    uint64_t uVar14;
    uint64_t uVar17;
    uint8_t *puVar10;
    uint8_t *puVar12;
    uint8_t *puVar13;
    uint32_t local_res18;
    int iVar3;
    int iVar16;
    int iVar20;
    int iVar21;
    int64_t lVar11;
    WCHAR WVar2;
    WCHAR *pWVar1;
    WCHAR *pWVar9;
    WCHAR *pWVar15;
    WCHAR *lpDestStr;
    WCHAR *lpSrcStr;
    WCHAR *local_98;
    uint8_t *local_90;
    uint8_t *local_88;
    int64_t *plVar18;
    int local_80[2] = {0, 0};
    uint64_t local_78;
    uint8_t *local_70;
    int64_t local_68;

    uVar19 = (uint16_t)param_4;
    uVar7 = FUN_14001b4f8((int16_t *)param_1, (int16_t)uVar19);
    local_78 = 0;
    puVar10 = (uint8_t *)&param_1[(size_t)uVar7];
    uVar17 = uVar7 & 0xffffffff;
    memset(&local_70, 0, 0x20);
    PECMD_InitTableSlots((void **)&local_90, (void **)&local_88, local_80, 0x28);
    uVar14 = 0x400;
    puVar13 = puVar10;
    if (param_4 != 0) goto LAB_140075d12;
    while (1) {
        local_68 = (int64_t)(int)uVar17;
        local_70 = (uint8_t *)param_1;
        PECMD_VectorAppendGen((void **)&local_90, (void **)&local_88, (int64_t *)local_80,
                      (void *)&local_78, 0x28, 0x400);
        (void)local_68;
        param_1 = (uint16_t *)puVar13;
        if ((int)uVar14 < (int)uVar17) {
            uVar14 = uVar17 & 0xffffffff;
        }
    LAB_140075d12:
        puVar12 = (uint8_t *)param_1;
        if (puVar10 <= puVar12) break;
        do {
            puVar13 = puVar12 + 1;
            if ((uVar19 ^ 10) == *(uint16_t *)puVar12) break;
            puVar12 = puVar13;
        } while (puVar13 < puVar10);
        uVar17 = ((uint64_t)(puVar13 - (uint8_t *)param_1)) >> 1;
    }
    FUN_140063694(&local_98, (int64_t)((int)uVar14 + 0x1a + (int)uVar7));
    uVar17 = 0;
    iVar21 = 0;
    uVar7 = (uint64_t)local_80[0];
    local_res18 = 0;
    if (local_80[0] < 1) goto LAB_140075f08;
LAB_140075dbb:
    uVar8 = FUN_14005dff4();                 /* PRNG 选行起点 (SUB168/SUB164=64位取模伪影) */
    uVar8 = uVar8 % uVar7;
    lVar11 = (int64_t)uVar8;
    if (lVar11 < (int64_t)uVar7) {
        plVar18 = (int64_t *)(local_90 + (uint64_t)lVar11 * 0x28);
        do {
            if (plVar18[0] == 0) {
                local_res18++;
                lVar11 = (int64_t)(int)uVar8;
                iVar3 = *(int *)(local_90 + (uint64_t)lVar11 * 0x28 + 0x10);
                *(uint64_t *)(local_90 + (uint64_t)lVar11 * 0x28) = 1;
                memcpy(local_98, *(void **)(local_90 + (uint64_t)lVar11 * 0x28 + 8),
                       (size_t)iVar3 * 2);
                pWVar1 = local_98 + iVar3;
                pWVar15 = local_98 + (int)uVar14 + iVar21 + 10;
                pWVar9 = local_98;
                lpDestStr = pWVar15;
                lpSrcStr = local_98;
                if (local_98 < pWVar1) goto LAB_140075e4c;
                goto LAB_140075e73;
            }
            lVar11++;
            uVar8 = (uint64_t)((int)uVar8 + 1);
            plVar18 += 5;
        } while (lVar11 < (int64_t)uVar7);
    }
    goto LAB_140075efd;
LAB_140075e4c:
    do {
        *pWVar9 = (WCHAR)(*pWVar9 ^ uVar19);
        pWVar9++;
    } while (pWVar9 < pWVar1);
    do {
        if (0x7f < (uint8_t)*lpSrcStr) break;
        WVar2 = *lpSrcStr;
        lpSrcStr++;
        *lpDestStr = WVar2;
        lpDestStr++;
    } while (lpSrcStr < pWVar1);
LAB_140075e73:
    iVar16 = (int)(lpDestStr - pWVar15);
    iVar20 = iVar3 - iVar16;
    if (0 < iVar20 &&
        (iVar20 = (int)LCMapStringW(0x20804,
                                    (uint32_t)(-((uint32_t)(param_3 != 0)) & 0x2000000)
                                        + 0x2000000,
                                    (LPCWSTR)lpSrcStr, iVar20, (LPWSTR)lpDestStr,
                                    iVar3 + iVar20),
         0 < iVar20)) {
        iVar16 += iVar20;
    }
    memset(local_98, 0, (size_t)iVar3 * 2);
    iVar21 += iVar16;
    *(WCHAR **)(local_90 + (uint64_t)lVar11 * 0x28 + 0x18) = pWVar15;
    *(int64_t *)(local_90 + (uint64_t)lVar11 * 0x28 + 0x20) = (int64_t)iVar16;
    if (0 < iVar16) {
        while (iVar16 = iVar16 - 1, -1 < iVar16) {
            *pWVar15 = (WCHAR)(*pWVar15 ^ uVar19);
            pWVar15++;
        }
    }
    uVar17 = (uint64_t)local_res18;
LAB_140075efd:
    if (local_80[0] <= (int)uVar17) goto LAB_140075f08;
    goto LAB_140075dbb;
LAB_140075f08:
    puVar10 = (uint8_t *)FUN_140063224((void *)*param_2,
                                       (int64_t)(iVar21 + 0x11) * 2 + 2);
    *param_2 = (WCHAR *)puVar10;
    if (0 < (int64_t)uVar7) {
        plVar18 = (int64_t *)(local_90 + 0x20);
        do {
            memcpy(puVar10, (void *)plVar18[-1], (size_t)((int)plVar18[0]) * 2);
            lVar11 = plVar18[0];
            plVar18 += 5;
            uVar7--;
            puVar10 += (size_t)lVar11 * 2;
        } while (uVar7 != 0);
    }
    *(uint16_t *)puVar10 = uVar19;
    FUN_14005b0b8((void *)local_98);
    uVar4 = (uint64_t)(uintptr_t)*param_2;
    FUN_14005b104((void *)&local_98);
    FUN_14005b104((void *)&local_90);
    return uVar4;
}

/* @0x14007f6e4 size=127 — 按分隔符取 token 赋值变量, 推进游标并返回其+2 */
int64_t *PECMD_SplitTokenAssignVar(int64_t *cursor, WCHAR **pp, uint16_t sep, int flag)
{
    WCHAR *tok = NULL;

    PECMD_AllocStrSlot(&tok);
    PECMD_ExtractTokenByDelim(pp, &tok, (int16_t)sep);
    PECMD_ExpandVarDispatch((int64_t *)(cursor[1]), tok, (int64_t *)cursor, 0, flag);
    cursor[2] = cursor[0];
    FUN_14005b104(&tok);
    return cursor + 2;
}

/* @0x1400a9a84 size=36 — 解析一个值(成功时推进 *pp)再越过 1 个字符;
 * 返回值沿用 PECMD_EvalParenStripped 的解析结果(ABI: ret 时 RAX 未改) */
uint64_t FUN_1400a9a84(int64_t *pp, uint64_t *out)
{
    uint64_t v;

    v = PECMD_EvalParenStripped((WCHAR **)pp, out);
    if (*(short *)(uintptr_t)*pp != 0) {
        *pp = (int64_t)((short *)(uintptr_t)*pp + 1);
    }
    return v;
}
