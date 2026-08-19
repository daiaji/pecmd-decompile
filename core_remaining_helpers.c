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

uint64_t FUN_14005b374(void)
{
    /* UNIMPLEMENTED @0xFUN_14005b374 — decompile-failed, body 未还原 */
/* @0x14005b374 size=44 */
    return 0;
}

LARGE_INTEGER PECMD_SetFilePointer(HANDLE h, LARGE_INTEGER dist, DWORD method)
{
    /* @0x14005c674 移动文件指针并返回新位置 */
    int64_t newPos = -1;
    SetFilePointerEx(h, dist.QuadPart, &newPos, method);
    return (LARGE_INTEGER)newPos;
}

uint64_t FUN_1400675b8(void)
{
    /* UNIMPLEMENTED @0xFUN_1400675b8 — decompile-failed, body 未还原 */
/* @0x1400675b8 size=145 */
    return 0;
}

uint64_t FUN_1400676e4(void)
{
    /* UNIMPLEMENTED @0xFUN_1400676e4 — decompile-failed, body 未还原 */
/* @0x1400676e4 size=99 */
    return 0;
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

uint64_t FUN_14006923c(void)
{
    /* UNIMPLEMENTED @0xFUN_14006923c — decompile-failed, body 未还原 */
/* @0x14006923c size=153 */
    return 0;
}

uint64_t FUN_1400692d8(void)
{
    /* UNIMPLEMENTED @0xFUN_1400692d8 — decompile-failed, body 未还原 */
/* @0x1400692d8 size=58 */
    return 0;
}

/* @0x140069314 WCHAR串→字节缓冲(仅保留十进制数字, 每数字1字节), 返回字节数 */
int FUN_140069314(void *out, LPCWSTR src)
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

uint64_t FUN_1400693c0(void)
{
    /* UNIMPLEMENTED @0xFUN_1400693c0 — decompile-failed, body 未还原 */
/* @0x1400693c0 size=161 */
    return 0;
}

/* @0x140075148 按行写变量 (查表/建新项 + 写值/截断 + 关键段保护) */
void FUN_140075148(void *script, LPCWSTR key, LPCWSTR data, int64_t len,
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

uint64_t FUN_140075c7c(void)
{
    /* SKIP @0x140075c7c — body 需 FUN_14001b4f8 / FUN_14005dff4, 项目未定义, 保持桩 */
/* @0x140075c7c size=797 */
    return 0;
}

uint64_t FUN_14007f6e4(void)
{
    /* UNIMPLEMENTED @0xFUN_14007f6e4 — decompile-failed, body 未还原 */
/* @0x14007f6e4 size=127 */
    return 0;
}

uint64_t FUN_1400a9a84(void)
{
    /* UNIMPLEMENTED @0xFUN_1400a9a84 — decompile-failed, body 未还原 */
/* @0x1400a9a84 size=36 */
    return 0;
}
