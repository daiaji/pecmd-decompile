/*
 * core_b3r_g.c — batch28 business functions (part G)
 *
 * Restored from decompiled Ghidra pseudocode in batch28.txt.
 *   PECMD_SaveImageToFile @0x14007d8d4  save image/icon to file
 *   PECMD_ResolveMacAddress @0x14007f078  large MAC/formatter function
 *   PECMD_QueryWindowState @0x140080a14  ?-list / *-list UI helper
 *   PECMD_CreateTextControl @0x140082a54  large OBAR/control-positioning function
 *
 * Conventions: FUN_140102a90 -> memset; WSTR("..") for wide literals;
 * CONCAT44/CONCAT22/CONCAT71 splice assignments are kept faithful with
 * plain bit ops (marked TODO(verify) where the source is incomplete).
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>

#include "win32_stub.h"
#include "pecmd_defs.h"

/* ---- Ghidra primitive aliases ---- */
typedef unsigned int        uint;
typedef unsigned short      ushort;
typedef uint8_t             byte;
typedef uint8_t             undefined1;
typedef uint16_t            undefined2;
typedef uint32_t            undefined4;
typedef uint64_t            undefined8;
typedef void                undefined;
typedef int64_t             longlong;
typedef uint64_t            ulonglong;

/* ---- CONCAT splice macros (Ghidra register glue) ---- */
#ifndef CONCAT44
#define CONCAT44(hi, lo) ((((uint64_t)(uint32_t)(hi)) << 32) | (uint32_t)(lo))
#endif
#ifndef CONCAT22
#define CONCAT22(hi2, lo2) ((((uint32_t)(uint16_t)(hi2)) << 16) | (uint16_t)(lo2))
#endif
#ifndef CONCAT71
#define CONCAT71(hi7, lo1) ((((uint64_t)(hi7)) << 8) | (uint8_t)(lo1))
#endif

/* ================================================================
 * Global data references used by this group
 * ================================================================ */
extern DWORD g_imgBufLen;        /* pending image-buffer length */
extern WCHAR g_szEmpty[];      /* empty string (.rdata)       */
extern int64_t g_pCacheBlock;        /* cached COM enumeration      */
extern GUID g_clsidCoCreate;        /* CLSID slot (CoCreateInstance) */
extern GUID g_iidCoCreate;        /* IID   slot (CoCreateInstance) */

/* GDI+ lazily-loaded function-pointer slots (typed) */
extern int (*g_pGdipGetImageWidth)();   /* GdipGetImageWidth    */
extern int (*g_pGdipGetImageHeight)();   /* GdipGetImageHeight   */
extern int (*g_pGdipCreateFromHDC)();         /* GdipCreateFromHDC    */
extern int (*g_pGdipSetInterpolationMode)(void *, int);          /* GdipSetInterpolationMode */
extern int (*g_pGdipDrawImageRectI)(void *, void *, int, int, int64_t, int); /* GdipDrawImageRectI */
extern int (*g_pGdipDeleteGraphics)();                 /* GdipDeleteGraphics   */
extern int (*g_pGdipDisposeImage)();               /* GdipDisposeImage     */

/* COM/OLE lazily-loaded slots */
extern int  (*g_pCoCreateInstance)(void *, void *, uint32_t, void *, void **); /* CoCreateInstance */

/* ================================================================
 * Helper-function externs (bodies provided elsewhere; NOT defined here)
 * ================================================================ */
extern void      PECMD_WideToAnsiStr(int64_t *ps, LPCWSTR src, int64_t len, uint64_t cap);
extern void     *PECMD_GrowByteBuffer(void **ps, int64_t len);
extern void PECMD_FreeStrBuf(void *ps);
extern int       PECMD_AdapterMacIpLookup(void *a, int64_t *b, int c);
extern int64_t   PECMD_ParseVolumeGuid(int64_t *param_1, uint32_t *param_2, int param_3);
extern int32_t PECMD_LoadOle32Apis(void);
extern void      PECMD_AllocMagicBlock(void **p, int64_t len);
extern int       PECMD_AnsiStrNCompare(char *buf, int64_t a, int64_t b);
extern WCHAR    *PECMD_AllocString(WCHAR **ps, int64_t count);
extern LPWSTR    PECMD_GuidToString(LPWSTR dst, uint32_t *guid, int mode);
extern void *PECMD_StrBldCopyWide(void *a, const WCHAR *b);
extern int64_t   PECMD_AsciiPrefixICmp(const char *s, const WCHAR *w, int n);
extern int64_t   PECMD_TokPrefixICmp(const char *a, const WCHAR *w, int n);
extern WCHAR    *PECMD_SkipLeadingControlChars(WCHAR **pp);
extern void      PECMD_StrDupAssign(WCHAR **ps, const WCHAR *src);
extern WCHAR    *PECMD_AssignString(WCHAR **ps, const WCHAR *src);
extern void      PECMD_AllocStrSlot(void *out);
extern int       PECMD_ParseUIntValue(LPCWSTR *, int *);
extern void      PECMD_ShowWindowMode(HWND, uint32_t *, int);
extern void      PECMD_CopyTokenTrimmed(int64_t *, int64_t *, short, short);
extern void      PECMD_VarSetUInt(void *script, uint64_t value, LPCWSTR key);
extern HBITMAP   PECMD_CaptureScreenRegion(RECT *, HDC *, LPCWSTR);
extern DWORD     PECMD_SaveBitmap(HBITMAP, LPCWSTR, int, LPCWSTR);
extern HICON     PECMD_LoadIcon(LPCWSTR, uint64_t *);
extern WCHAR    *PECMD_SkipWCharUntil(WCHAR **pp, uint16_t ch);
extern void      PECMD_ResetScriptChain(int64_t *, int64_t *);
extern void      PECMD_ExpandVarDispatch(int64_t *, WCHAR *, WCHAR **, int, int);
extern void      PECMD_ParseNumSkipWs(int64_t *, uint64_t *);
extern void      PECMD_SplitNextToken(int64_t *, WCHAR **, WCHAR **, uint32_t, int);
extern int64_t  *PECMD_SplitTokenAssignVar(WCHAR **out, WCHAR **pp, uint32_t sep, int flag);
extern void      PECMD_ParseLtwhParams(int64_t *, uint32_t *, int *, int *, uint32_t *);
extern void      PECMD_ExtractTokenByDelim(int64_t *, int64_t *, int);
extern void      PECMD_CopyUpToChar(int64_t *, int64_t *, uint32_t);
extern uint64_t  PECMD_ParseSignedNumber(short *);
extern void      PECMD_ParseHashNumbers(int64_t *, int64_t);
extern void      PECMD_SplitTokenTrimWs(int64_t *, int64_t *, short);
extern void      PECMD_ExpandBackslashNewline(const WCHAR *, char);
extern void      PECMD_ReadFileToWide(WCHAR *, int64_t *);
extern void      PECMD_DispatchCreateControl(uint64_t, int64_t, WCHAR **, int, int, int, int,
                               WCHAR **, WCHAR **, uint32_t, int *, LPCWSTR, uint64_t);

/* ================================================================
 * @0x14007d8d4  save image/icon to file
 * ================================================================ */
DWORD PECMD_SaveImageToFile(RECT *param_1, LPCWSTR param_2, LPCWSTR param_3, LPCWSTR param_4)
{
    HDC hdc;
    DWORD DVar1;
    int iVar2;
    int iVar3;
    HANDLE hFile;
    HICON pHVar4;
    HBITMAP ho;
    DWORD local_res8[2];
    /* 5th hidden stack parameter (Ghidra: in_stack_ffffffffffffff98) */
    uint64_t in_stack = 0;
    uint32_t uVar5;
    uint32_t local_48[2];
    int64_t local_40;
    HDC local_38;
    HGDIOBJ local_30;

    uVar5 = (uint32_t)(in_stack >> 32);
    pHVar4 = (HICON)0;
    if (param_1 == (RECT *)0) {
        if (param_2 == (LPCWSTR)0) {
            return 0x80070057;
        }
        if (g_imgBufLen < 1) {
            return 0;
        }
        local_res8[0] = 0;
        hFile = CreateFileW(param_2, 0x40000000, 0, (LPSECURITY_ATTRIBUTES)0, 2,
                            0x8000080, (HANDLE)0);
        if (hFile == (HANDLE)0xffffffffffffffff) {
            DVar1 = GetLastError();
            if (DVar1 == 0) {
                return 1;
            }
            return DVar1;
        }
        WriteFile(hFile, g_pImageBuf, (DWORD)g_imgBufLen, local_res8,
                  (void *)0);
        CloseHandle(hFile);
        g_imgBufLen = 0;
        if (g_pImageBuf != (LPCVOID)0) {
            HeapFree(g_hHeap, 0,
                     (LPVOID)((int64_t)(intptr_t)g_pImageBuf - 8));
            g_pImageBuf = (void *)(LPCVOID)0;
            return 0;
        }
        g_imgBufLen = 0;
        return 0;
    }
    if (param_3 == (LPCWSTR)0) {
        ho = PECMD_CaptureScreenRegion(param_1, (HDC *)0, WSTR("DISPLAY"));
        if (ho == (HBITMAP)0) {
            return 1;
        }
        goto LAB_14007db22;
    }
    pHVar4 = PECMD_LoadIcon(param_3, (uint64_t *)0);
    if (pHVar4 == (HICON)0) {
        return 1;
    }
    iVar2 = param_1->right - param_1->left;
    iVar3 = param_1->bottom - param_1->top;
    if ((0 < iVar2) && (0 < iVar3)) {
        local_res8[0] = 0;
        local_48[0] = 0;
        (*g_pGdipGetImageWidth)((void *)pHVar4, local_res8);
        (*g_pGdipGetImageHeight)((void *)pHVar4, local_48);
        if (((int64_t)iVar2 != (uint64_t)local_res8[0]) ||
            ((int64_t)iVar3 != (uint64_t)local_48[0]))
        {
            local_38 = (HDC)0;
            local_30 = (HGDIOBJ)0;
            ho = PECMD_CaptureScreenRegion(param_1, &local_38, param_4);
            hdc = local_38;
            if (ho == (HBITMAP)0) {
                return 1;
            }
            local_40 = 0;
            (*g_pGdipCreateFromHDC)(local_38, (void **)&local_40);
            (*g_pGdipSetInterpolationMode)((void *)(intptr_t)local_40, 2);
            (*g_pGdipDrawImageRectI)((void *)(intptr_t)local_40, (void *)pHVar4, 0, 0,
                             (int64_t)CONCAT44(uVar5, (uint32_t)iVar2),
                             (int)iVar3);
            SelectObject(hdc, local_30);
            DeleteDC(hdc);
            if (local_40 != 0) {
                (*g_pGdipDeleteGraphics)();
            }
            goto LAB_14007db22;
        }
    }
    DVar1 = PECMD_SaveBitmap((HBITMAP)pHVar4, param_2, 1, param_4);
    if (pHVar4 != (HICON)0) {
        (*g_pGdipDisposeImage)((void *)pHVar4);
        return DVar1;
    }
    return DVar1;

LAB_14007db22:
    DVar1 = PECMD_SaveBitmap(ho, param_2, 0, param_4);
    if (ho != (HBITMAP)0) {
        DeleteObject(ho);
    }
    if (pHVar4 != (HICON)0) {
        (*g_pGdipDisposeImage)((void *)pHVar4);
        return DVar1;
    }
    return DVar1;
}

/* ================================================================
 * @0x14007f078  large MAC-address / formatter function
 * ================================================================ */
uint32_t *PECMD_ResolveMacAddress(int param_1, uint64_t *param_2, uint64_t *param_3,
                        int64_t *param_4, uint64_t param_5, LPWSTR param_6,
                        LPWSTR param_7, int64_t *param_8, LPWSTR param_9)
{
    (void)param_5;
    ushort uVar1;
    short *psVar2;
    LPCSTR pCVar3;
    int iVar4;
    int iVar5;
    uint64_t uVar6;
    uint32_t *puVar7;
    LPSTR pCVar8;
    char cVar9;
    LPCWSTR pWVar10;
    uint32_t *puVar11;
    uint32_t *puVar12;
    int64_t *plVar13;
    uint32_t uVar14;
    int64_t lVar15;
    uint32_t *puVar16;
    bool bVar17;
    char *local_res10;
    uint64_t *local_res18;
    int64_t *local_res20;
    uint32_t *local_a8;
    uint8_t local_a0[6];
    uint32_t *local_98;
    LPCSTR local_90;
    int64_t *local_88;
    int64_t *local_80;
    int64_t *local_78;
    uint8_t local_6c[4];
    LPCWSTR local_68;
    char local_60[32];

    puVar11 = (uint32_t *)0;
    local_90 = (LPCSTR)0;
    local_res18 = param_3;
    local_res20 = param_4;
    PECMD_WideToAnsiStr((int64_t *)&local_90, (LPCWSTR)*param_2, -1, 0xffffffffffffffff);
    PECMD_GrowByteBuffer((void **)param_3, 0x32);
    *(uint8_t *)*param_3 = 0;
    *(uint16_t *)*param_4 = 0;
    pWVar10 = (LPCWSTR)*param_2;
    iVar4 = lstrlenW(pWVar10);
    pCVar3 = local_90;
    psVar2 = (short *)(intptr_t)*param_2;
    if (((*psVar2 == 0x7b) && (psVar2[(int64_t)iVar4 - 1] == 0x7d)) &&
        (((uVar1 = (ushort)psVar2[1], 0x2f < uVar1 && (uVar1 < 0x3a)) ||
          ((0x60 < (uVar1 | 0x20)) && ((uVar1 | 0x20) < 0x67)))) &&
        (((uVar1 = (ushort)psVar2[2], 0x2f < uVar1 && (uVar1 < 0x3a)) ||
          ((0x60 < (uVar1 | 0x20)) && ((uVar1 | 0x20) < 0x67)))))
    {
        bVar17 = true;
    }
    else {
        bVar17 = false;
    }
    local_a0[0] = 0;
    cVar9 = '\0';
    local_a0[1] = 0; local_a0[2] = 0; local_a0[3] = 0; local_a0[4] = 0;
    local_a0[5] = 0;
    if (bVar17) {
        cVar9 = '\x04';
        /* (bVar17 true: the MAC parse at LAB_14007f17e is skipped) */
    }
    else {
        if (*pWVar10 != L'\0') {
            cVar9 = '\x01';
        }
        if (iVar4 == 0x11) {
            uint32_t macval;
            local_98 = (uint32_t *)local_a0;
            local_res10 = (char *)local_90 + -1;
            puVar16 = puVar11;
            puVar12 = puVar11;
            do {
                iVar4 = sscanf(pCVar3 + (int)(intptr_t)puVar16, "%2x", &macval);
                if ((iVar4 < 1) ||
                    (((0 < (int)(intptr_t)puVar12) && (*local_res10 != ':')) &&
                     (*local_res10 != '-')))
                {
                    goto LAB_14007f219;
                }
                local_res10 = local_res10 + 3;
                /* TODO(verify): Ghidra lost the parsed value (wrote 0) */
                *(uint8_t *)local_98 = (uint8_t)macval;
                local_98 = (uint32_t *)((uintptr_t)local_98 + 1);
                uVar14 = (uint32_t)(intptr_t)puVar16 + 3;
                puVar16 = (uint32_t *)(uintptr_t)uVar14;
                puVar12 = (uint32_t *)(uintptr_t)((int)(intptr_t)puVar12 + 1);
            } while ((int)uVar14 < 0x12);
            cVar9 = '\x03';
        }
    }
LAB_14007f219:
    if (cVar9 == '\x03') {
        iVar4 = PECMD_AdapterMacIpLookup(local_a0, (int64_t *)param_2, 0);
        if (iVar4 == 0) {
            PECMD_FreeStrBuf((WCHAR **)&local_90);
            return (uint32_t *)0;
        }
        cVar9 = '\x04';
        bVar17 = true;
    }
    local_res10 = (char *)(intptr_t)*param_2;
    if (bVar17) {
        PECMD_ParseVolumeGuid((int64_t *)&local_res10, (uint32_t *)local_60, 0);
    }
    iVar4 = lstrlenW(pWVar10);
    if (*pWVar10 == L'=') {
        cVar9 = '\x02';
        pWVar10 = pWVar10 + 1;
    }
    if (*pWVar10 == L'\"') {
        if (pWVar10[(int64_t)iVar4 - 1] == L'\"') {
            ((LPWSTR)pWVar10)[(int64_t)iVar4 - 1] = L'\0';
        }
        pWVar10 = pWVar10 + 1;
    }
    if (cVar9 != '\0') {
        param_1 = -1;
    }
    PECMD_AssignString((WCHAR **)param_2, pWVar10);
    pWVar10 = (LPCWSTR)*param_2;
    local_68 = pWVar10;
    PECMD_WideToAnsiStr((int64_t *)&local_90, pWVar10, -1, 0xffffffffffffffff);
    iVar4 = lstrlenA(local_90);
    plVar13 = (int64_t *)g_pCacheBlock;
    local_a8 = (uint32_t *)0;
    /* local_res10 doubles as an int container (Ghidra CONCAT44 of its
     * top 32 bits with iVar4).  Preserve faithfully. */
    local_res10 = (char *)((((uint64_t)(uint32_t)((uint64_t)local_res10 >> 32)) << 32)
                           | (uint32_t)iVar4);
    puVar16 = puVar11;
    if (g_pCacheBlock == (int64_t)(intptr_t)(int64_t *)0) {
LAB_14007f319:
        local_80 = (int64_t *)0;
        local_88 = (int64_t *)0;
        local_78 = (int64_t *)0;
        EnterCriticalSection(&g_csCom);
        PECMD_LoadOle32Apis();
        iVar4 = -3;
        if ((g_pOleInit != (int (*)(int))0) &&
            (iVar4 = (*g_pOleInit)(0), iVar4 != 0) && (iVar4 == 1))
        {
            (*g_pOleUninit)();
        }
        (*g_pCoCreateInstance)(&g_clsidCoCreate, (void *)0, 0x15, &g_iidCoCreate,
                         (void **)&local_80);
        if (local_80 == (int64_t *)0) {
            if ((g_pOleUninit != (void (*)(void))0) && (iVar4 == 0)) {
                (*g_pOleUninit)();
            }
            LeaveCriticalSection(&g_csCom);
            goto LAB_14007f6c3;
        }
        if (local_88 == (int64_t *)0) {
            ((int (*)(void *, int, void **))*(uint64_t *)(*local_80 + 0x18))
                (local_80, 0, (void **)&local_88);
        }
        local_98 = (uint32_t *)0;
        puVar12 = puVar11;
        if (local_88 != (int64_t *)0) {
            iVar5 = ((int (*)(void *, int, void **, void *))*(uint64_t *)(*local_88 + 0x18))
                        (local_88, 1, (void **)&local_78, local_6c);
            if (iVar5 == 0) {
                lVar15 = (int64_t)puVar16 * 0x50;
                do {
                    ((void (*)(void *, void **))*(uint64_t *)(*local_78 + 0x38))
                        (local_78, (void **)&local_a8);
                    PECMD_GrowByteBuffer((void **)&local_98, lVar15 + 0x50);
                    puVar12 = local_98;
                    memcpy((void *)(lVar15 + (intptr_t)local_98), local_a8, 0x50);
                    puVar16 = (uint32_t *)((intptr_t)puVar16 + 1);
                    lVar15 = lVar15 + 0x50;
                    ((void (*)(void *))*(uint64_t *)(*local_78 + 0x10))(local_78);
                    local_78 = (int64_t *)0;
                    iVar5 = ((int (*)(void *, int, void **, void *))*(uint64_t *)(*local_88 + 0x18))
                                (local_88, 1, (void **)&local_78, local_6c);
                    pWVar10 = local_68;
                } while (iVar5 == 0);
            }
            if (local_88 != (int64_t *)0) {
                ((void (*)(void *))*(uint64_t *)(*local_88 + 0x10))(local_88);
                local_88 = (int64_t *)0;
            }
        }
        ((void (*)(void *))*(uint64_t *)(*local_80 + 0x10))(local_80);
        local_80 = (int64_t *)0;
        if (plVar13 == (int64_t *)0) {
            PECMD_AllocMagicBlock((void **)&g_pCacheBlock, 0x28);
            plVar13 = (int64_t *)g_pCacheBlock;
        }
        *plVar13 = 1;
        plVar13[1] = (int64_t)puVar16;
        plVar13[2] = (int64_t)puVar12;
        local_98 = (uint32_t *)0;
        PECMD_FreeStrBuf((WCHAR **)&local_98);
        if ((g_pOleUninit != (void (*)(void))0) && (iVar4 == 0)) {
            (*g_pOleUninit)();
        }
        LeaveCriticalSection(&g_csCom);
        iVar4 = (int)(intptr_t)local_res10;
    }
    else {
        puVar16 = (uint32_t *)(intptr_t)((int64_t *)g_pCacheBlock)[0];
        puVar12 = (uint32_t *)(intptr_t)((int64_t *)g_pCacheBlock)[1];
        if (*(int64_t *)g_pCacheBlock == 0) {
            goto LAB_14007f319;
        }
    }
    puVar7 = puVar11;
    local_a8 = puVar12;
    if (0 < (int64_t)puVar16) {
        do {
            local_a8 = puVar12;
            if (cVar9 == '\x04') {
                iVar4 = PECMD_AnsiStrNCompare(local_60, (int64_t)puVar12, 0x10);
                bVar17 = iVar4 == 0;
                if (bVar17) {
                    goto LAB_14007f55f;
                }
            }
            else if (cVar9 == '\x01') {
                uVar6 = (uint64_t)PECMD_AsciiPrefixICmp(local_90,
                                                (const WCHAR *)(uintptr_t)*(uint64_t *)((char *)puVar12 + 24),
                                                iVar4);
                if ((char)uVar6 != '\0') {
                    goto LAB_14007f55f;
                }
            }
            else {
                if (cVar9 == '\0') {
                    bVar17 = (int)(intptr_t)puVar7 == param_1;
                    if (bVar17) {
                        goto LAB_14007f55f;
                    }
                }
                if ((cVar9 == '\x02') &&
                    (iVar4 = lstrcmpiW(pWVar10,
                                       (LPCWSTR)(uintptr_t)*(uint64_t *)((char *)puVar12 + 16)),
                     puVar12 = local_a8, iVar4 == 0))
                {
                    goto LAB_14007f55f;
                }
            }
            uVar14 = (uint32_t)(intptr_t)puVar7 + 1;
            puVar12 = (uint32_t *)((intptr_t)puVar12 + 0x50);
            local_a8 = puVar12;
            if ((int64_t)puVar16 <= (int64_t)(int)uVar14) {
                break;
            }
            puVar7 = (uint32_t *)(uintptr_t)uVar14;
            iVar4 = (int)(intptr_t)local_res10;
        } while (true);
    }
    goto LAB_14007f6c3_zeroterm;

LAB_14007f55f:
    pCVar8 = (LPSTR)(uintptr_t)*local_res18;
    *pCVar8 = '\0';
    PECMD_AllocString((WCHAR **)param_2, 0x32);
    PECMD_GuidToString((LPWSTR)(uintptr_t)*param_2, local_a8, 1);
    iVar4 = PECMD_AdapterMacIpLookup(local_a0, (int64_t *)param_2, 1);
    if (-1 < iVar4) {
        int bi = 0;
        do {
            if (puVar11 != (uint32_t *)0) {
                *pCVar8 = '-';
                pCVar8 = pCVar8 + 1;
            }
            /* TODO(verify): Ghidra dropped the format argument */
            wsprintfA(pCVar8, "%02X", (unsigned int)local_a0[bi]);
            puVar11 = (uint32_t *)((intptr_t)puVar11 + 1);
            pCVar8 = pCVar8 + 2;
            bi++;
        } while ((intptr_t)puVar11 < 6);
    }
    PECMD_StrBldCopyWide(local_res20,
                  (LPCWSTR)(uintptr_t)*(uint64_t *)((char *)local_a8 + 16));
    if (param_8 != (int64_t *)0) {
        PECMD_StrBldCopyWide(param_8,
                      (LPCWSTR)(uintptr_t)*(uint64_t *)((char *)local_a8 + 24));
    }
    if (param_6 != (LPWSTR)0) {
        /* TODO(verify): missing format argument */
        wsprintfW(param_6, WSTR("%d"));
    }
    if (param_7 != (LPWSTR)0) {
        wsprintfW(param_7, WSTR("%d"));
    }
    if (param_9 != (LPWSTR)0) {
        wsprintfW(param_9, WSTR("%d"));
    }
    puVar11 = (uint32_t *)(intptr_t)*param_2;
    goto LAB_14007f6c3;

LAB_14007f6c3_zeroterm:
    *(uint16_t *)*param_2 = 0;
LAB_14007f6c3:
    PECMD_FreeStrBuf((WCHAR **)&local_90);
    return puVar11;
}

/* ================================================================
 * @0x140080a14  ?-list / *-list UI helper
 * ================================================================ */
uint32_t PECMD_QueryWindowState(int64_t *param_1, HWND param_2, LPCWSTR param_3,
                       short param_4)
{
    LPCWSTR pWVar1;
    int iVar2;
    uint32_t uVar3;
    int iVar4;
    LPCWSTR local_res18;
    uint32_t local_res20[2];
    LPCWSTR local_28[2];

    local_res20[0] = 0xf8000009;
    iVar2 = 0;
    if (*param_3 == L'?') {
        param_3 = param_3 + 1;
        iVar2 = 0x3f;
    }
    if (param_4 == 0x3f) {
        iVar2 = 1;
    }
    iVar4 = 0;
    local_res18 = param_3;
    if (*param_3 == L'*') {
        local_res18 = param_3 + 1;
        iVar4 = 0x2a;
    }
    pWVar1 = local_res18;
    if (iVar2 == 0) {
        PECMD_ParseUIntValue((LPCWSTR *)&local_res18, (int *)local_res20);
        if (local_res20[0] != 0xf8000009) {
            PECMD_ShowWindowMode(param_2, local_res20, iVar4);
            if (local_res20[0] == 0) {
                return 1;
            }
            return 3;
        }
    }
    else {
        PECMD_AllocStrSlot(&local_res18);
        local_28[0] = pWVar1;
        PECMD_CopyTokenTrimmed((int64_t *)local_28, (int64_t *)&local_res18, 0x3b, 0x3a);
        uVar3 = IsWindowVisible(param_2);
        PECMD_VarSetUInt(param_1, (uint64_t)uVar3, local_res18);
        PECMD_FreeStrBuf((WCHAR **)&local_res18);
    }
    return 0;
}

/* ================================================================
 * @0x140082a54  large OBAR/control-positioning function
 * ================================================================ */
uint64_t PECMD_CreateTextControl(int64_t *param_1, WCHAR *param_2, WPARAM param_3,
                       uint64_t param_4)
{
    WCHAR WVar1;
    bool bVar2;
    bool bVar3;
    int64_t lVar4;
    uint64_t uVar5;
    int64_t *plVar6;
    uint64_t *puVar7;
    uint64_t uVar8;
    int iVar9;
    uint32_t uVar10;
    uint32_t uVar11;
    uint32_t uVar12;
    uint64_t uVar13;
    uint64_t uVar14;
    WCHAR WVar15;
    WCHAR *pWVar16;
    WCHAR *local_res8;
    WCHAR *local_res10;
    WPARAM local_res18;
    uint64_t local_res20;
    int local_c4;
    LPCWSTR local_c0;
    LPCWSTR local_b8;
    LPCWSTR local_a0;
    int local_b0[2];
    LPCWSTR local_a8;
    uint64_t local_98;
    WCHAR *local_90;
    WCHAR *local_88;
    uint64_t local_80;
    int local_78[4];
    WCHAR *local_68;
    int64_t local_60;
    int64_t *local_58;
    uint64_t local_50;

    local_res10 = param_2;
    local_res18 = param_3;
    local_res20 = param_4;
    PECMD_AllocStrSlot(&local_60);
    local_50 = 0;
    local_58 = param_1;
    (void)local_50; (void)local_58;
    if (param_3 == 0) {
        PECMD_ResetScriptChain(param_1, (int64_t *)0);
        param_3 = (WPARAM)param_1[8];
        local_res18 = param_3;
        if (param_3 == 0) {
            local_res18 = 0;
            uVar5 = 0xffffffff80070057;
            goto LAB_140083250;
        }
    }
    uVar14 = 0;
    bVar2 = false;
    local_res8 = (WCHAR *)((uint64_t)local_res8 & 0xffffffffffffff00);
    bVar3 = false;
    WVar15 = L'-';
    uVar8 = uVar14;
    while (true) {
        uVar10 = (uint32_t)uVar8;
        WVar1 = *local_res10;
        if ((WVar1 == L'\0') ||
            (((8 < (uint16_t)WVar1) && ((uint16_t)WVar1 < 0xe)) ||
             (WVar1 == L' ')))
        {
            break;
        }
        if (WVar1 == L'-') {
            uVar8 = (uint64_t)(uVar10 | 0x8000000);
        }
        else if (WVar1 == L'|') {
            uVar8 = (uint64_t)(uVar10 | 0x10000000);
        }
        else if (WVar1 == L'+') {
            uVar8 = (uint64_t)(uVar10 | 0x4000000);
        }
        else if (WVar1 == L'.') {
            bVar2 = true;
        }
        else if (WVar1 == L'*') {
            /* CONCAT71(local_res8.high7, 1) */
            local_res8 = (WCHAR *)(((uint64_t)local_res8 & 0xffffffffffffff00) | 1);
        }
        else if (WVar1 == L'=') {
            bVar3 = true;
        }
        local_res10 = local_res10 + 1;
    }
    PECMD_SkipLeadingControlChars((WCHAR **)&local_res10);
    iVar9 = 0x2c;
    local_68 = local_res10;
    uVar13 = uVar14 & 0xffffffff;
    uVar11 = (uint32_t)uVar14;
    PECMD_SkipWCharUntil(&local_68, 0x2c);
    WVar1 = *local_68;
    *local_68 = (WCHAR)uVar14;
    local_90 = local_68;
    /* CONCAT22(local_c4.high16, WVar1) */
    local_c4 = (int)CONCAT22((uint16_t)((uint32_t)local_c4 >> 16), WVar1);
    local_98 = uVar14;
    if (WVar15 == *local_res10) {
        do {
            pWVar16 = local_res10;
            lVar4 = PECMD_TokPrefixICmp("-center", (const WCHAR *)local_res10, 7);
            if ((char)lVar4 == '\0') {
                lVar4 = PECMD_TokPrefixICmp("-right", (const WCHAR *)pWVar16, 6);
                if ((char)lVar4 == '\0') {
                    lVar4 = PECMD_TokPrefixICmp("-vcenter:", (const WCHAR *)pWVar16, 8);
                    uVar10 = (uint32_t)uVar8;
                    if ((char)lVar4 == '\0') {
                        uVar5 = (uint64_t)PECMD_AsciiPrefixICmp("-vcenter:", (const WCHAR *)pWVar16,
                                                        iVar9 + -0x23);
                        if ((char)uVar5 == '\0') {
                            lVar4 = PECMD_TokPrefixICmp("-rich", (const WCHAR *)pWVar16, 5);
                            if ((char)lVar4 == '\0') {
                                lVar4 = PECMD_TokPrefixICmp("-nroc", (const WCHAR *)pWVar16, 5);
                                if ((char)lVar4 == '\0') {
                                    lVar4 = PECMD_TokPrefixICmp("-3D", (const WCHAR *)pWVar16, 3);
                                    if ((char)lVar4 == '\0') {
                                        lVar4 = PECMD_TokPrefixICmp("-u2d", (const WCHAR *)pWVar16, 4);
                                        if ((char)lVar4 != '\0') {
                                            uVar8 = (uint64_t)(uVar10 | 0x1000000);
                                        }
                                    }
                                    else {
                                        uVar8 = (uint64_t)(uVar10 | 0x200000);
                                    }
                                }
                                else {
                                    uVar8 = (uint64_t)(uVar10 | 0x400000);
                                }
                            }
                            else {
                                uVar8 = (uint64_t)(uVar10 | 0x800000);
                            }
                        }
                        else {
                            local_res10 = pWVar16 + 9;
                            uVar8 = (uint64_t)(uVar10 | 0x2000000);
                            PECMD_AllocStrSlot(&local_c0);
                            PECMD_ExpandVarDispatch(param_1, local_res10, (WCHAR **)&local_c0, 0, 1);
                            local_a0 = local_c0;
                            PECMD_ParseNumSkipWs((int64_t *)&local_a0, &local_98);
                            /* important detail: local_98 re-cast as (ulonglong)(char) */
                            local_98 = (uint64_t)(char)local_98;
                            PECMD_FreeStrBuf((WCHAR **)&local_c0);
                            pWVar16 = local_res10;
                        }
                    }
                    else {
                        uVar8 = (uint64_t)(uVar10 | 0x2000000);
                    }
                }
                else {
                    uVar13 = 0x20000000;
                }
            }
            else {
                uVar13 = 0x40000000;
            }
            uVar11 = (uint32_t)uVar13;
            uVar10 = (uint32_t)uVar8;
            WVar1 = *pWVar16;
            while ((WVar1 != L'\0' &&
                    (((uint16_t)WVar1 < (uint16_t)(WCHAR)(iVar9 + -0x23)) ||
                     (0xd < (uint16_t)WVar1))) &&
                   (WVar1 != L' '))
            {
                pWVar16 = pWVar16 + 1;
                local_res10 = pWVar16;
                WVar1 = *pWVar16;
            }
            PECMD_SkipLeadingControlChars((WCHAR **)&local_res10);
        } while (WVar15 == *local_res10);
        param_3 = local_res18;
        WVar1 = (WCHAR)local_c4;
    }
    *local_90 = WVar1;
    PECMD_AllocStrSlot(&local_a8);
    PECMD_AllocStrSlot(&local_b8);
    PECMD_AllocStrSlot(&local_a0);
    PECMD_AllocStrSlot(&local_c0);
    PECMD_StrDupAssign(&local_88, (const WCHAR *)g_szEmpty);
    uVar5 = 0;
    local_80 = local_res20;
    (void)local_80;
    local_78[0] = -0x80000000;
    local_78[1] = 0x80000000;
    local_78[2] = 0x80000000;
    local_78[3] = 0x80000000;
    local_90 = (WCHAR *)((uint64_t)local_90 & 0xffffffff00000000);
    local_b0[0] = 0;
    local_c4 = 0;
    local_res18 = local_res18 & 0xffffffff00000000;
    uVar8 = 0;
    WVar15 = *local_res10;
    if (WVar15 == L'*') {
        local_res10 = local_res10 + 1;
        PECMD_SkipLeadingControlChars((WCHAR **)&local_res10);
    }
    PECMD_SplitNextToken(param_1, (WCHAR **)&local_res10, (WCHAR **)&local_a8, 0x2c, 0);
    if (*local_res10 == L'\0') {
        PECMD_FreeStrBuf((WCHAR **)&local_88);
        PECMD_FreeStrBuf((WCHAR **)&local_c0);
        PECMD_FreeStrBuf((WCHAR **)&local_a0);
        PECMD_FreeStrBuf((WCHAR **)&local_b8);
        PECMD_FreeStrBuf(&local_a8);
        uVar5 = 1;
    }
    else {
        plVar6 = PECMD_SplitTokenAssignVar((WCHAR **)&local_60, (WCHAR **)&local_res10, 0x2c, 1);
        PECMD_ParseLtwhParams(plVar6, (uint32_t *)&local_90, local_b0, &local_c4,
                      (uint32_t *)&local_res18);
        if (*local_res10 == L',') {
            local_res10 = local_res10 + 1;
            plVar6 = PECMD_SplitTokenAssignVar((WCHAR **)&local_60, (WCHAR **)&local_res10, 0x2c, 1);
            PECMD_ExtractTokenByDelim(plVar6, (int64_t *)&local_b8, 0);
            if (*local_res10 == L',') {
                local_res10 = local_res10 + 1;
                PECMD_CopyUpToChar((int64_t *)&local_res10, (int64_t *)&local_88, 0x2c);
                if (*local_res10 == L',') {
                    local_res10 = local_res10 + 1;
                    puVar7 = (uint64_t *)PECMD_SplitTokenAssignVar((WCHAR **)&local_60, (WCHAR **)&local_res10, 0x2c, 1);
                    uVar8 = PECMD_ParseSignedNumber((short *)(uintptr_t)*puVar7);
                    if (*local_res10 == L',') {
                        local_res10 = local_res10 + 1;
                        plVar6 = PECMD_SplitTokenAssignVar((WCHAR **)&local_60, (WCHAR **)&local_res10, 0x2c, 1);
                        PECMD_ParseHashNumbers(plVar6, (int64_t)local_78);
                        if (*local_res10 == L',') {
                            local_res10 = local_res10 + 1;
                            plVar6 = PECMD_SplitTokenAssignVar((WCHAR **)&local_60, (WCHAR **)&local_res10, 0x2c, 1);
                            PECMD_SplitTokenTrimWs(plVar6, (int64_t *)&local_c0, 0x2c);
                        }
                    }
                }
            }
        }
        pWVar16 = local_88;
        uVar12 = (uint32_t)uVar8;
        if ((char)(uintptr_t)local_res8 != '\0') {
            local_88 = (WCHAR *)0;
            local_res8 = pWVar16;
            PECMD_ExpandVarDispatch(param_1, pWVar16, (WCHAR **)&local_88, 0, 1);
            PECMD_FreeStrBuf((WCHAR **)&local_res8);
        }
        if ((!bVar2) && ((uVar8 & 4) != 0)) {
            PECMD_ExpandBackslashNewline(local_b8, '\0');
        }
        if (bVar3) {
            PECMD_ReadFileToWide((WCHAR *)local_b8, (int64_t *)&local_b8);
        }
        if ((short)uVar8 == 2) {
            uVar12 = 0x8000;
        }
        else if ((short)uVar8 == 3) {
            uVar12 = 8;
        }
        if (WVar15 != L'*') {
            param_1 = *(int64_t **)((uintptr_t)param_3 + 0x290);
        }
        PECMD_DispatchCreateControl(param_3, (int64_t)param_1, (WCHAR **)&local_a8,
                      (int)(int32_t)(uint64_t)local_90, local_b0[0], local_c4,
                      (int)(int32_t)local_res18, (WCHAR **)&local_b8,
                      (WCHAR **)&local_88, (uVar12 & 0xffff) | uVar10 | uVar11,
                      local_78, local_c0, local_98);
        PECMD_FreeStrBuf((WCHAR **)&local_88);
        PECMD_FreeStrBuf((WCHAR **)&local_c0);
        PECMD_FreeStrBuf((WCHAR **)&local_a0);
        PECMD_FreeStrBuf((WCHAR **)&local_b8);
        PECMD_FreeStrBuf(&local_a8);
    }
LAB_140083250:
    PECMD_FreeStrBuf(&local_60);
    return uVar5;
}
