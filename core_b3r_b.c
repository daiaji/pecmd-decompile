/* ==========================================================================
 * partB.c — restored from decompiled Ghidra export (/tmp/restore_groups/partB.c)
 * See CONVENTIONS.md for restore rules.
 *
 * Contains:
 *   PECMD_ParseWindowOptions @0x1400734e4  window option flag parser
 *   PECMD_CacheLookupInsert @0x140074c60  list/cache entry lookup + insert
 *   PECMD_SetConfigString @0x140074e58  config string store
 *   PECMD_EnumDeviceList @0x140075848  enum / dev-list walk
 * ========================================================================== */

#include "win32_stub.h"
#include "pecmd_defs.h"
#include <string.h>

/* --------------------------------------------------------------------------
 * extern helper declarations (not defined in win32_stub.h / pecmd_defs.h)
 * ------------------------------------------------------------------------ */

/* token prefix matcher: returns non-zero if the wide string at *pp matches the
 * ASCII token (first n chars) and consumes it (advances *pp); 0 otherwise. */
extern char    PECMD_MatchTokenAdvance(char *tok, int64_t *pp, int n);

/* ASCII prefix compare (case-insensitive against wide string): 1 = match, 0 = no. */
extern int     PECMD_AsciiPrefixICmp(const char *s, const WCHAR *w, int n);

/* skip leading whitespace in the wide string at *ps; returns pointer past it. */
extern uint16_t *PECMD_SkipLeadingControlChars(uint16_t **ps);

/* parse integer at *pp with rounding, write into *out; returns bool flag. */
extern uint64_t PECMD_ParseIntRound(int64_t *pp, int *out);

/* read LogPixels / AppliedDPI; return the cached DPI value, or -1 if absent.
 * NOTE: project restores these two as void — used as int here (see TODO). */
extern int     PECMD_LoadLogPixelsDpi(void);
extern int     PECMD_ReadAppliedDpi(void);

/* grow string-ish buffer: ensure *ps capacity >= count units; returns *ps. */
extern WCHAR  *PECMD_AllocString(WCHAR **ps, int64_t count);

/* overallocate / allocate a buffer through *ps (allocation helper). */
extern void    PECMD_AllocStringSlot2(void **ps, int64_t len);

/* free a string buffer through *ps. */
extern void PECMD_FreeStrBuf(void *ps);

/* string append: *ps keeps old content and appends src; returns *ps. */
extern WCHAR  *PECMD_AppendWideStr(WCHAR **ps, LPCWSTR src);

/* ASCII / wide case-insensitive equality: non-zero = equal. */
extern int32_t PECMD_AsciiWideICmp(const char *a, const WCHAR *w);

/* packed system version: (major<<32 | minor<<16 | build). */
extern uint64_t PECMD_GetPackedSystemVersion(void);

/* MININT boot-option detection; returns non-zero flag. */
extern uint32_t PECMD_DetectMinintBoot(void);

/* string assign (StrCpyW2) — treated as void here per task. */
extern void    FUN_1400703e4(int64_t *param_1, LPCWSTR param_2);

/* ---- global data ---- */
extern void *g_pConfigStr;          /* config string pointer */
extern int32_t  g_i32D6F4;          /* config dirty flag */

/* dev-list enumeration callbacks (function pointers). */
extern int (*g_pWIMGetMountedImages)(void *buf, uint32_t *flags);
extern int (*g_pWIMGetMountedImageCount)(void *buf, uint32_t *flags);

/* ========================================================================== */
/* @0x1400734e4                                                               */
/* ========================================================================== */
/* signature: uint __fastcall PECMD_ParseWindowOptions(ushort * param_1, undefined1 * param_2, int * param_3) */
uint32_t PECMD_ParseWindowOptions(uint16_t *param_1, uint8_t *param_2, int *param_3)
{
    uint16_t uVar1;
    char cVar2;
    int iVar3;
    uint64_t uVar4;
    uint32_t uVar5;
    uint16_t *puVar6;
    uint16_t *local_res8;
    int local_res20[2];

    uVar5 = 0;
    uVar1 = *param_1;
    local_res8 = param_1;
    while (uVar1 != 0) {
        cVar2 = PECMD_MatchTokenAdvance("-top", (int64_t *)&local_res8, 4);
        if (cVar2 == '\0') {
            cVar2 = PECMD_MatchTokenAdvance("-bottom", (int64_t *)&local_res8, -1);
            if (cVar2 == '\0') {
                cVar2 = PECMD_MatchTokenAdvance("-forcenomin", (int64_t *)&local_res8, -1);
                if (cVar2 == '\0') {
                    cVar2 = PECMD_MatchTokenAdvance("-disminb", (int64_t *)&local_res8, -1);
                    if (cVar2 == '\0') {
                        cVar2 = PECMD_MatchTokenAdvance("-maxb", (int64_t *)&local_res8, -1);
                        if (cVar2 == '\0') {
                            cVar2 = PECMD_MatchTokenAdvance("-discloseb", (int64_t *)&local_res8, -1);
                            if (cVar2 == '\0') {
                                cVar2 = PECMD_MatchTokenAdvance("-nosysmenu", (int64_t *)&local_res8, -1);
                                if (cVar2 == '\0') {
                                    cVar2 = PECMD_MatchTokenAdvance("-nocap", (int64_t *)&local_res8, -1);
                                    if (cVar2 == '\0') {
                                        cVar2 = PECMD_MatchTokenAdvance("-disaltmv", (int64_t *)&local_res8, 9);
                                        if (cVar2 == '\0') {
                                            cVar2 = PECMD_MatchTokenAdvance("-altmv", (int64_t *)&local_res8, 6);
                                            if (cVar2 == '\0') {
                                                cVar2 = PECMD_MatchTokenAdvance("-mv", (int64_t *)&local_res8, 3);
                                                if (cVar2 == '\0') {
                                                    cVar2 = PECMD_MatchTokenAdvance("-layer", (int64_t *)&local_res8, 6);
                                                    if (cVar2 == '\0') {
                                                        cVar2 = PECMD_MatchTokenAdvance("-size", (int64_t *)&local_res8, 5);
                                                        if (cVar2 == '\0') {
                                                            cVar2 = PECMD_MatchTokenAdvance("-nfocus", (int64_t *)&local_res8, 7);
                                                            if (cVar2 == '\0') {
                                                                cVar2 = PECMD_MatchTokenAdvance("-ntab", (int64_t *)&local_res8, 5);
                                                                if (cVar2 == '\0') {
                                                                    cVar2 = PECMD_MatchTokenAdvance("-trap", (int64_t *)&local_res8, 5);
                                                                    if (cVar2 == '\0') {
                                                                        cVar2 = PECMD_MatchTokenAdvance("-csize", (int64_t *)&local_res8, 6);
                                                                        if (cVar2 == '\0') {
                                                                            cVar2 = PECMD_MatchTokenAdvance("-na", (int64_t *)&local_res8, 3);
                                                                            if (cVar2 == '\0') {
                                                                                cVar2 = PECMD_MatchTokenAdvance("-nxp", (int64_t *)&local_res8, 4);
                                                                                if (cVar2 == '\0') {
                                                                                    cVar2 = PECMD_MatchTokenAdvance("-scalef", (int64_t *)&local_res8, 7);
                                                                                    if (cVar2 == '\0') {
                                                                                        cVar2 = PECMD_MatchTokenAdvance("-scale", (int64_t *)&local_res8, 6);
                                                                                        if (cVar2 == '\0') {
                                                                                            /* "-scale:" prefix handling */
                                                                                            puVar6 = local_res8;
                                                                                            uVar4 = PECMD_AsciiPrefixICmp("-scale:", (const WCHAR *)local_res8, 7);
                                                                                            if ((char)uVar4 == '\0') {
                                                                                                uVar1 = *puVar6;
                                                                                                while ((uVar1 != 0 &&
                                                                                                       (((local_res8 = puVar6, uVar1 < 9 ||
                                                                                                         (0xd < uVar1)) && (uVar1 != 0x20))))) {
                                                                                                    puVar6 = puVar6 + 1;
                                                                                                    local_res8 = puVar6;
                                                                                                    uVar1 = *puVar6;
                                                                                                }
                                                                                                PECMD_SkipLeadingControlChars((uint16_t **)&local_res8);
                                                                                            } else {
                                                                                                local_res8 = puVar6 + 7;
                                                                                                local_res20[0] = 0;
                                                                                                PECMD_ParseIntRound((int64_t *)&local_res8, local_res20);
                                                                                                if (0 < local_res20[0]) {
                                                                                                    *param_3 = local_res20[0];
                                                                                                }
                                                                                            }
                                                                                        } else {
                                                                                            /* matched "-scale" */
                                                                                            iVar3 = PECMD_LoadLogPixelsDpi();
                                                                                            if (iVar3 != -1) {
                                                                                                *param_3 = -0x60;
                                                                                            }
                                                                                        }
                                                                                    } else {
                                                                                        /* matched "-scalef" */
                                                                                        iVar3 = PECMD_ReadAppliedDpi();
                                                                                        if (iVar3 != -1) {
                                                                                            *param_3 = iVar3;
                                                                                        }
                                                                                    }
                                                                                } else {
                                                                                    uVar5 = uVar5 | 0x40000;
                                                                                }
                                                                            } else {
                                                                                uVar5 = uVar5 | 0x20000;
                                                                            }
                                                                        } else {
                                                                            uVar5 = uVar5 | 0x10000;
                                                                        }
                                                                    } else {
                                                                        uVar5 = uVar5 | 0x800;
                                                                    }
                                                                } else {
                                                                    uVar5 = uVar5 | 0x4000;
                                                                }
                                                            } else {
                                                                uVar5 = uVar5 | 0x1000;
                                                            }
                                                        } else {
                                                            uVar5 = uVar5 | 0x400;
                                                        }
                                                    } else {
                                                        uVar5 = uVar5 | 0x200000;
                                                    }
                                                } else {
                                                    *param_2 = 2;
                                                }
                                            } else {
                                                *param_2 = 1;
                                            }
                                        } else {
                                            *param_2 = 0;
                                        }
                                    } else {
                                        uVar5 = uVar5 | 0x200;
                                    }
                                } else {
                                    uVar5 = uVar5 | 0x100;
                                }
                            } else {
                                uVar5 = uVar5 | 0x80;
                            }
                        } else {
                            uVar5 = uVar5 | 0x40;
                        }
                    } else {
                        uVar5 = uVar5 | 0x20;
                    }
                } else {
                    uVar5 = uVar5 | 0x10;
                }
            } else {
                uVar5 = uVar5 | 0x100000;
            }
        } else {
            uVar5 = uVar5 | 8;
        }
        uVar1 = *local_res8;
    }
    if (((uVar5 >> 0x11) & 1) != 0) {
        uVar5 = uVar5 & 0xffffefff;
    }
    return uVar5;
}

/* ========================================================================== */
/* @0x140074c60                                                               */
/* ========================================================================== */
/* signature: longlong * __fastcall PECMD_CacheLookupInsert(LPCWSTR param_1, longlong param_2,
 *                                                undefined8 param_3, undefined8 ** param_4) */
int64_t *PECMD_CacheLookupInsert(LPCWSTR param_1, int64_t param_2, uint64_t param_3, uint64_t **param_4)
{
    int64_t lVar1;
    int64_t lVar2;
    uint64_t *puVar3;
    uint32_t uVar4;
    int iVar5;
    int64_t *plVar6;
    int64_t lVar7;
    int64_t *plVar8;
    int64_t lVar9;
    uint64_t **ppuVar10;
    int64_t lVar11;
    int64_t lVar12;
    uint64_t uVar13;
    int64_t lVar14;
    int64_t lVar15;
    uint64_t local_48;
    int64_t local_40;

    (void)param_3;

    EnterCriticalSection((LPCRITICAL_SECTION)&g_csInit);
    plVar8 = g_cacheTableA;
    ppuVar10 = (uint64_t **)&local_48;
    if ((uint64_t)0xfff < (uint64_t)(uintptr_t)param_4) {
        ppuVar10 = param_4;
    }
    local_48 = (uint64_t)(-(uint32_t)((uintptr_t)param_4 < 0x1000) &
                          ((uint32_t)(uintptr_t)param_4 & 0xff));
    lVar7 = 0;
    uVar4 = *(uint32_t *)ppuVar10 & 0x200;
    if (uVar4 != 0) {
        plVar8 = g_cacheTableB;
    }
    lVar12 = -1;
    lVar11 = *plVar8;
    lVar1 = plVar8[2];
    local_40 = plVar8[1];
    lVar14 = -1;
    if ((0 < plVar8[1]) && (lVar9 = lVar12, lVar15 = lVar14, 0 < lVar1)) {
        do {
            lVar2 = *(int64_t *)(lVar11 + lVar7 * 8);
            lVar12 = lVar7;
            lVar14 = lVar7;
            if ((lVar2 != 0) &&
               ((lVar12 = lVar9, lVar14 = lVar15,
                 *(int64_t *)(lVar2 + 0x10) != 0 &&
                 (iVar5 = lstrcmpiW(*(LPCWSTR *)(lVar2 + 0x10), param_1), iVar5 == 0)))) {
                plVar6 = (int64_t *)(lVar11 + lVar7 * 8);
                *(int64_t *)(*plVar6 + 8) = *(int64_t *)(*plVar6 + 8) + 1;
                goto LAB_140074e2e;
            }
            lVar7 = lVar7 + 1;
            lVar9 = lVar12;
            lVar15 = lVar14;
        } while (lVar7 < lVar1);
    }
    lVar7 = local_40;
    plVar6 = (int64_t *)0x0;
    if ((*(uint32_t *)ppuVar10 >> 8 & 1) == 0) {
        *(uint32_t *)ppuVar10 = *(uint32_t *)ppuVar10 | 1;
        lVar9 = lVar1;
        if (lVar12 < 0) {
            lVar9 = lVar1 + 1;
            PECMD_AllocString((WCHAR **)plVar8, lVar9 * 8);
            lVar11 = *plVar8;
            lVar12 = lVar1;
        }
        local_40 = lVar7 + 1;
        iVar5 = lstrlenW(param_1);
        uVar13 = ((int64_t)iVar5 * 2 + 0x11) & 0xfffffffffffffff0ULL;
        lVar7 = param_2;
        if (uVar4 != 0) {
            lVar7 = 0;
        }
        PECMD_AllocStringSlot2((void **)&local_48, lVar7 + 0x38 + (int64_t)uVar13);
        puVar3 = (uint64_t *)local_48;
        plVar6 = (int64_t *)(lVar11 + lVar12 * 8);
        lVar7 = 0;
        *plVar6 = (int64_t)local_48;
        puVar3[2] = (uint64_t)(puVar3 + 8);
        puVar3[0] = 0;
        puVar3[1] = 1;
        memcpy((uint8_t *)(puVar3 + 8), (const uint8_t *)param_1,
               (int)((int64_t)iVar5 * 2) + 2);
        if (uVar4 == 0) {
            lVar7 = (int64_t)(puVar3[2] + uVar13);
        }
        puVar3[3] = (uint64_t)lVar7;
        puVar3[4] = (uint64_t)param_2;
        local_48 = 0;
        plVar8[1] = local_40;
        if (lVar14 < 1) {
            plVar8[2] = lVar9;
        }
        PECMD_FreeStrBuf((WCHAR **)&local_48);
    }
LAB_140074e2e:
    LeaveCriticalSection((LPCRITICAL_SECTION)&g_csInit);
    return plVar6;
}

/* ========================================================================== */
/* @0x140074e58                                                               */
/* ========================================================================== */
/* signature: undefined __fastcall PECMD_SetConfigString(wchar_t * param_1, int param_2) */
void PECMD_SetConfigString(wchar_t *param_1, int param_2)
{
    uint32_t uVar1;
    uint64_t uVar2;

    if (param_2 == 0) {
LAB_140074e7e:
        uVar2 = PECMD_GetPackedSystemVersion();
        param_2 = 0;
        if ((0x9ffffffff < uVar2) &&
            (uVar1 = PECMD_DetectMinintBoot(), param_2 = 1, ((*param_1 == L'\0') & uVar1) != 0)) {
            param_1 = (wchar_t *)WSTR("R*-20B*-20");
            goto LAB_140074ec7;
        }
    } else if (*param_1 == L'?') {
        param_1 = param_1 + 1;
        goto LAB_140074e7e;
    }
    if ((*param_1 == L'\0') || (param_1[1] == L'\0')) {
        param_2 = 0;
    }
    if (param_2 == 0) {
        if (g_pConfigStr != 0) {
            HeapFree(g_hHeap, 0, (LPVOID)((char *)g_pConfigStr - 8));
        }
        g_i32D6F4 = 1;
        g_pConfigStr = 0;
        return;
    }
LAB_140074ec7:
    FUN_1400703e4((int64_t *)&g_pConfigStr, (LPCWSTR)param_1);
    g_i32D6F4 = 1;
    return;
}

/* ========================================================================== */
/* @0x140075848                                                               */
/* ========================================================================== */
/* signature: void * __fastcall PECMD_EnumDeviceList(undefined8 param_1, LPCWSTR param_2,
 *               LPCWSTR param_3, longlong * param_4, ushort * param_5,
 *               void * param_6, uint * param_7) */
void *PECMD_EnumDeviceList(uint64_t param_1, LPCWSTR param_2, LPCWSTR param_3, int64_t *param_4,
                    uint16_t *param_5, void *param_6, uint32_t *param_7)
{
    int bVar1;
    int iVar2;
    int iVar3;
    uint64_t uVar4;
    LPCWSTR pWVar5;
    void *pvVar6;
    void *pvVar7;
    void *pvVar8;
    uint32_t uVar9;
    uint32_t *puVar10;
    uint32_t local_b8[2];
    void *local_b0;
    WCHAR local_a8[56];

    (void)param_1;

    pvVar7 = (void *)0x0;
    puVar10 = local_b8 + 1;
    if (param_7 != (uint32_t *)0x0) {
        puVar10 = param_7;
    }
    local_b8[1] = 0;
    *puVar10 = 0;
    PECMD_AllocStringSlot2(&local_b0, 0x100020);
    local_b8[0] = 0x100000;
    iVar2 = (*g_pWIMGetMountedImages)(local_b0, local_b8);
    while (bVar1 = 0, iVar2 != 0) {
        uVar9 = local_b8[0] / 0x418;
        pvVar6 = pvVar7;
        pvVar8 = pvVar7;
        if (uVar9 != 0) {
            do {
                iVar2 = (int)(uintptr_t)pvVar6;
                if (((*param_2 == L'\0') ||
                     (iVar3 = lstrcmpiW(param_2, (LPCWSTR)((char *)local_b0 + (int64_t)iVar2 * 0x418 + 0x208)),
                      iVar3 != 0)) &&
                    ((*param_3 == L'\0') ||
                     (iVar3 = lstrcmpiW(param_3, (LPCWSTR)((char *)local_b0 + (int64_t)iVar2 * 0x418)),
                      iVar3 != 0))) {
                    if ((*param_2 == L'\0') && (*param_3 == L'\0')) goto LAB_1400759c5;
                } else {
                    bVar1 = 1;
                    if (param_6 != (void *)0x0) {
                        memcpy(param_6, (void *)((char *)local_b0 + (int64_t)iVar2 * 0x418), 0x418);
                        pvVar8 = param_6;
                        goto LAB_140075ae2;
                    }
                    if ((param_5 != (uint16_t *)0x0) &&
                        (uVar4 = (uint64_t)PECMD_AsciiWideICmp("rw", (const WCHAR *)param_5),
                         (char)uVar4 != '\0')) {
                        pWVar5 = WSTR("0");
                        if (*(int *)((char *)local_b0 + (int64_t)iVar2 * 0x418 + 0x414) != 0) {
                            pWVar5 = WSTR("1");
                        }
                        PECMD_AppendWideStr((WCHAR **)param_4, pWVar5);
                        goto LAB_140075ae2;
                    }
LAB_1400759c5:
                    wsprintfW(local_a8, WSTR("\r\n%lu 0x%lX \""));
                    PECMD_AppendWideStr((WCHAR **)param_4, local_a8);
                    pWVar5 = (LPCWSTR)((char *)local_b0 + (int64_t)iVar2 * 0x418);
                    PECMD_AppendWideStr((WCHAR **)param_4, pWVar5);
                    PECMD_AppendWideStr((WCHAR **)param_4, WSTR("\" \""));
                    PECMD_AppendWideStr((WCHAR **)param_4, pWVar5 + 0x104);
                    PECMD_AppendWideStr((WCHAR **)param_4, WSTR("\""));
                    if (bVar1) goto LAB_140075ae2;
                }
                pvVar6 = (void *)(uintptr_t)(iVar2 + 1U);
            } while ((int)(iVar2 + 1U) < (int)uVar9);
        }
        if ((*puVar10 != 0) || (pvVar8 = (void *)0x0, g_pWIMGetMountedImageCount == (int (*)(void *, uint32_t *))0x0))
            goto LAB_140075ae2;
        *puVar10 = 1;
        local_b8[0] = 0x100000;
        iVar2 = (*g_pWIMGetMountedImageCount)(local_b0, local_b8);
    }
    pvVar8 = (void *)0x1;
LAB_140075ae2:
    PECMD_FreeStrBuf((WCHAR **)&local_b0);
    return pvVar8;
}
