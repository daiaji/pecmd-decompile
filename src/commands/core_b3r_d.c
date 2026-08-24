/* ==========================================================================
 * partD.c — Restored PECMD business functions (group7/8 part D)
 *
 * Restored from decompiled Ghidra pseudocode in /tmp/restore_groups/partD.c
 * Functions kept with their Ghidra names FUN_<addr> and `@0x<addr>` header.
 * Uses the project convention: FUN_140102a90 -> memset, WSTR() for wide lits.
 * ========================================================================== */

#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdbool.h>

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

/* CONCAT44(hi,lo): (uint32)hi<<32 | (uint32)lo  (Ghidra register glue) */
#ifndef CONCAT44
#define CONCAT44(hi, lo) (((uint64_t)(uint32_t)(hi)) << 32 | (uint32_t)(lo))
#endif

/* ---- Local minimal types (project stub headers lack these) ---- */
typedef void  *LPITEMIDLIST;
typedef const void *LPCITEMIDLIST;
typedef intptr_t (*BFFCALLBACK)(HWND, UINT, LPARAM, LPARAM);

/* WIN32 BROWSEINFOW (matches _browseinfoW layout used at 0x140087690) */
typedef struct _browseinfoW {
    HWND          hwndOwner;
    LPCITEMIDLIST pidlRoot;
    LPWSTR        pszDisplayName;
    LPCWSTR       lpszTitle;
    UINT          ulFlags;
    BFFCALLBACK   lpfn;
    LPARAM        lParam;
    int           iImage;
} BROWSEINFOW, *PBROWSEINFOW, *LPBROWSEINFOW;

/* ---- Win32 APIs not in win32_stub.h ---- */
extern DWORD     GetClassLongW(HWND, int);
extern LONG_PTR  SetClassLongW(HWND, int, LONG_PTR);
extern LPITEMIDLIST SHBrowseForFolderW(LPBROWSEINFOW);
extern BOOL      SHGetPathFromIDListW(LPCITEMIDLIST, LPWSTR);

/* ---- Helper function externs (bodies are NOT defined here) ---- */
/* PECMD_SetupRoundedRegion helpers */
extern void      PECMD_ReleaseGdiObjects(void *param_1, HWND param_2);
extern void      PECMD_ParseNumTryWriteback(void **pp, int *out);
extern void      PECMD_ParseNumSkipChar_01f8(void **pp, int *out);
extern void     *PECMD_GrowByteBuffer(void **ps, int64_t len);
/* PECMD_ParseLtwhFlags helper */
extern uint64_t  PECMD_EvalParenExpression(int64_t *pp, uint64_t *out);
/* PECMD_ApplyWindowFlags helpers */
extern void      PECMD_EvalExprSkipOneChar(WCHAR **pp, uint64_t *out);
extern int       PECMD_UpdateWindowStyleBits(int64_t a, uint32_t b, uint64_t c);
extern int       PECMD_UpdateWindowExStyle(HWND a, uint32_t b, uint64_t c);
/* PECMD_ParseQuotedArg helpers */
extern WCHAR    *PECMD_SkipWCharUntil(WCHAR **pp, uint16_t ch);
extern WCHAR    *PECMD_SkipLeadingControlChars(WCHAR **pp);
extern WCHAR    *PECMD_NextToken(int64_t *a, int64_t *b, uint32_t c);
extern int64_t   PECMD_ExpandVarsRecursive(void *script, WCHAR *line, WCHAR **out,
                               int mode, uint8_t opt);
extern int64_t   PECMD_ExpandCommandLine(void *script, WCHAR *line, WCHAR **out,
                               int mode, uint8_t opt);
/* PECMD_ShowBrowseFolder string/var helpers */
extern void PECMD_AllocWStringBuffer(WCHAR **ps, int64_t count);
extern void      PECMD_AllocStrSlot(WCHAR **out);
extern char      PECMD_MatchTokenAdvance(const char *tok, WCHAR **pp, int n);
extern int64_t  *PECMD_SplitTokenAssignVar(WCHAR **out, WCHAR **pp, uint32_t sep, int flag);
extern void      PECMD_SplitTokenTrimWs(int64_t *src, WCHAR **dst, short delim);
extern void      PECMD_RunCommandLine(void *script, WCHAR **str, int mode);
extern void     *FUN_140063060(void *p);
extern WCHAR    *PECMD_UnquoteString(WCHAR *s);
extern void      PECMD_ParseShortStore(WCHAR **pp, int *out, WCHAR sep);
extern void      PECMD_ParseSignedNumberStr(void **in, void *out, short delim);
extern void      PECMD_StrDupAssign(WCHAR **ps, const WCHAR *src);
extern int64_t   PECMD_TokPrefixICmp(const char *a, const WCHAR *w, int n);
extern WCHAR    *PECMD_AssignString(WCHAR **ps, const WCHAR *src);
extern WCHAR    *PECMD_AppendWideStr(WCHAR **ps, const WCHAR *src);
extern uint8_t  *PECMD_VarLookup(void *script, LPCWSTR name, void *scope,
                               int64_t len, void **out);
extern void      PECMD_SetVariable(int64_t *script, LPCWSTR key, LPCWSTR value);
extern void PECMD_FreeStrBuf(void *ps);
extern HWND      PECMD_QueryState_f414(int64_t a);
extern WCHAR    *PECMD_AllocString(WCHAR **ps, int64_t count);
extern int64_t * PECMD_StrBldCopyAnsi(int64_t *out, const char *src, uint64_t len);
extern void      PECMD_SyncWorkingDirectory(void);
extern void      PECMD_LoadComDlgApis(void);                    /* @0x14005c904 (core_b3f.c) */
extern int64_t  *PECMD_CopyStrToSlot(uint64_t *a1, uint64_t *a2);
extern short    *PECMD_LastPathSeparator(short *s);
extern void PECMD_StrBldCopyWideN(WCHAR **pname, LPCWSTR src, int64_t len);
extern void *PECMD_StrBldCopyWide(void *a, const WCHAR *b);
extern void      PECMD_GetApiProcCached(const char *func, const char *dll,
                               void **out, uintptr_t *hmod);
extern intptr_t  PECMD_ControlMessage(HWND, UINT, LPARAM, LPARAM);
/* PECMD_EnumeratePhysicalDrives helpers */
extern void      PECMD_AllocStringSlot2(void **ps, int64_t len);
extern DWORD     PECMD_EnumDevices(LPCWSTR path, LPWSTR buf, uint32_t flags,
                               GUID *guid);
extern void      PECMD_OpenFileHandle(HANDLE *out, LPCWSTR path, DWORD access,
                               DWORD share, LPSECURITY_ATTRIBUTES sa,
                               DWORD disp, DWORD flags, HANDLE tmpl);
extern void      PECMD_QueryDevice(HANDLE h, uint64_t drive, WCHAR *buf, int mode);
extern uint64_t *PECMD_GetDiskLayoutInfo(HANDLE h, uint64_t *buf, uint32_t *out);
extern LPWSTR    PECMD_GuidToString(LPWSTR dst, uint32_t *guid, int mode);
extern int64_t   PECMD_GetDiskGeometrySize(HANDLE h, void *out);
extern int       PECMD_QueryDeviceIoInfo(HANDLE h, int size);
extern uint64_t  PECMD_ReadDiskSectorScan(uint64_t *param_1, uint64_t *param_2,
                               uint32_t param_3, DWORD param_4,
                               uint64_t param_5, uint64_t *param_6,
                               LARGE_INTEGER param_7);
extern uint64_t  PECMD_FindPartitionInfo(int64_t a, uint8_t b, uint32_t c, void *d,
                               void *e, DWORD f, int g, void *h);
extern uint32_t  PECMD_GetDiskGeometry(LPCWSTR p, HANDLE h);
extern int       PECMD_QueryDiskGeometry(HANDLE param_1, uint64_t *param_2,
                               int param_3, int param_4);
extern int64_t   PECMD_FindPartitionStartSector(HANDLE param_1, int *param_2, int64_t *param_3);
extern void PECMD_FormatI64Dec(LPWSTR dst, int64_t value);
extern LPCWSTR   PECMD_StripTrailingSpaces(LPCWSTR s);

/* ---- Globals (project convention keeps realistic-ish pointers) ---- */
extern uint8_t  g_bE870[];            /* device-class GUID bytes */
extern uint8_t  DAT_140127738[];
extern uint8_t  DAT_140127740[];
extern uint8_t  g_b127748[];
extern uint32_t g_u32127750;
extern uint16_t g_u1627754;
extern int64_t  g_i64D428;              /* GetOpenFileNameW slot */
extern int (*g_pGetSaveFileNameW)(void);              /* GetSaveFileNameW slot */
extern void    (*g_pCoTaskMemFree)(LPVOID);    /* CoTaskMemFree slot */
extern WCHAR g_szEmpty[];            /* empty string */
extern WCHAR    g_szEmpty[];                /* alias/slot for empty string */

/* ==========================================================================
 * PECMD_SetupRoundedRegion @0x14007f764
 * Rounded-rect window region / brush setup.
 * ========================================================================== */
int64_t *PECMD_SetupRoundedRegion(int64_t *param_1, short *param_2, HWND param_3,
                       ulonglong param_4)
{
    uint64_t *puVar1;
    HGDIOBJ    ho;
    DWORD      DVar2;
    HBRUSH     pHVar3;
    HRGN       pHVar4;
    HWND       pHVar5;
    COLORREF   color;
    short     *local_res10;
    COLORREF   local_res18[2];
    ulonglong  local_res20;
    COLORREF   local_58[2];
    RECT       local_50;

    pHVar5 = param_3;
    if (*param_2 != 0) {
        pHVar5 = (HWND)0x0;
    }
    local_res10 = param_2;
    local_res20 = param_4;
    PECMD_ReleaseGdiObjects(param_1, pHVar5);
    if (*param_2 == 0) {
        if (*param_1 != 0) {
            HeapFree(g_hHeap, 0, (LPVOID)(*param_1 + -8));
            *param_1 = 0;
        }
        param_1 = (int64_t *)0x0;
    } else {
        if (*param_1 == 0) {
            PECMD_GrowByteBuffer((void **)param_1, 0x40);
            memset((void *)*param_1, 0, 0x40);
        }
        puVar1 = (uint64_t *)*param_1;
        local_58[0] = 0;
        *(undefined4 *)((longlong)puVar1 + 0x24) = 0;
        *(undefined4 *)(puVar1 + 4) = 0;
        local_res18[0] = 0xffffffff;
        *(undefined4 *)((longlong)puVar1 + 0x2c) = 8;
        *(undefined4 *)(puVar1 + 5) = 8;
        *(undefined4 *)((longlong)puVar1 + 0x34) = 1;
        *(undefined4 *)(puVar1 + 6) = 1;
        PECMD_ParseNumTryWriteback((void **)&local_res10, (int *)local_58);
        color = 0xffffffff;
        if (*local_res10 == 0x2f) {
            local_res10 = local_res10 + 1;
            PECMD_ParseNumTryWriteback((void **)&local_res10, (int *)local_res18);
            color = local_res18[0];
        }
        local_res10 = local_res10 + 1;
        PECMD_ParseNumSkipChar_01f8((void **)&local_res10, (int *)(puVar1 + 5));
        *(undefined4 *)((longlong)puVar1 + 0x2c) = *(undefined4 *)(puVar1 + 5);
        PECMD_ParseNumSkipChar_01f8((void **)&local_res10, (int *)((longlong)puVar1 + 0x2c));
        PECMD_ParseNumSkipChar_01f8((void **)&local_res10, (int *)(puVar1 + 6));
        *(undefined4 *)((longlong)puVar1 + 0x34) = *(undefined4 *)(puVar1 + 6);
        PECMD_ParseNumSkipChar_01f8((void **)&local_res10, (int *)((longlong)puVar1 + 0x34));
        pHVar3 = CreateSolidBrush(local_58[0]);
        *puVar1 = (uint64_t)pHVar3;
        if (-1 < (int)color) {
            pHVar3 = CreateSolidBrush(color);
            puVar1[2] = (uint64_t)pHVar3;
        }
        ho = (HGDIOBJ)puVar1[1];
        local_50.left = 0;
        local_50.top = 0;
        local_50.right = 0;
        local_50.bottom = 0;
        GetWindowRect(param_3, &local_50);
        local_50.bottom = local_50.bottom - local_50.top;
        local_50.top = 0;
        local_50.right = local_50.right - local_50.left;
        local_50.left = 0;
        *(LONG *)(puVar1 + 4) = local_50.right;
        *(LONG *)((longlong)puVar1 + 0x24) = local_50.bottom;
        pHVar4 = CreateRoundRectRgn(0, 0, local_50.right, local_50.bottom,
                                    *(int *)(puVar1 + 5),
                                    *(int *)((longlong)puVar1 + 0x2c));
        puVar1[1] = (uint64_t)pHVar4;
        if (param_3 != (HWND)0x0) {
            if ((local_res20 & 1) == 0) {
                DVar2 = GetClassLongW(param_3, -0x1a);
                SetClassLongW(param_3, -0x1a, DVar2 & 0xffffff7f);
            }
            SetWindowRgn(param_3, (HRGN)puVar1[1], 1);
            if (ho != (HGDIOBJ)0x0) {
                DeleteObject(ho);
            }
        }
        pHVar4 = CreateRoundRectRgn(local_50.left, local_50.top, local_50.right,
                                    local_50.bottom,
                                    *(int *)(puVar1 + 5),
                                    *(int *)((longlong)puVar1 + 0x2c));
        puVar1[3] = (uint64_t)pHVar4;
    }
    return param_1;
}

/* ==========================================================================
 * PECMD_ParseLtwhFlags @0x14007fc18
 * Flag parser: consumes "l<num> t<num> w<num> h<num>" style tokens.
 * ========================================================================== */
uint32_t PECMD_ParseLtwhFlags(int64_t *param_1, undefined4 *param_2)
{
    uint64_t uVar1;
    uint32_t uVar2;
    int      iVar3;
    ushort   uVar4;
    ulonglong local_res8;

    uVar2 = 0;
    iVar3 = 0;
    while (1) {
        uVar4 = *(ushort *)*param_1;
        if ((((ushort)(uVar4 - 0x30) < 10) || (uVar4 == 0x2c)) || (3 < iVar3)) {
            break;
        }
        local_res8 = 0;
        *param_1 = (longlong)((ushort *)*param_1 + 1);
        uVar4 = (ushort)(uVar4 | 0x20);
        uVar1 = PECMD_EvalParenExpression(param_1, &local_res8);
        if ((int)uVar1 < 1) {
            return uVar2;
        }
        if (uVar4 == 0x6c) {
            uVar2 = uVar2 | 1;
            *param_2 = (undefined4)local_res8;
        } else if (uVar4 == 0x74) {
            uVar2 = uVar2 | 2;
            param_2[1] = (undefined4)local_res8;
        } else if (uVar4 == 0x77) {
            uVar2 = uVar2 | 4;
            param_2[2] = (undefined4)local_res8;
        } else if (uVar4 == 0x68) {
            uVar2 = uVar2 | 8;
            param_2[3] = (undefined4)local_res8;
        }
        iVar3 = iVar3 + 1;
    }
    return uVar2;
}

/* ==========================================================================
 * PECMD_ApplyWindowFlags @0x140080c40
 * Parse & set window position / style.
 * ========================================================================== */
uint64_t PECMD_ApplyWindowFlags(HWND param_1, short *param_2)
{
    short     sVar1;
    int       nIndex;
    short     sVar2;
    short    *local_res10;
    ulonglong local_res18;
    ulonglong local_res20;

    sVar1 = *param_2;
    if (sVar1 == 0x40) {
        param_2 = param_2 + 1;
    }
    sVar2 = 0;
    local_res10 = param_2;
    if (*param_2 == 0x2a) {
        local_res10 = param_2 + 1;
        sVar2 = 0x2a;
    }
    local_res18 = 0;
    local_res20 = 0;
    PECMD_EvalExprSkipOneChar((WCHAR **)&local_res10, &local_res18);
    PECMD_EvalExprSkipOneChar((WCHAR **)&local_res10, &local_res20);
    if (sVar1 == 0x40) {
        nIndex = -0x14;
        if (sVar2 == 0) {
            nIndex = -0x10;
        }
        SetWindowLongPtrW(param_1, nIndex, local_res18);
    } else if (sVar2 == 0) {
        PECMD_UpdateWindowStyleBits((int64_t)param_1, (uint32_t)local_res18,
                      local_res20 & 0xffffffffull);
    } else {
        PECMD_UpdateWindowExStyle(param_1, (uint32_t)local_res18, local_res20 & 0xffffffffull);
    }
    return 0;
}

/* ==========================================================================
 * PECMD_ParseQuotedArg @0x140083a48
 * String tokenizer.
 * ========================================================================== */
ushort *PECMD_ParseQuotedArg(longlong *param_1, longlong *param_2, longlong *param_3)
{
    short   *psVar1;
    int      iVar2;
    ushort  *puVar3;
    ushort  *puVar4;
    ushort  *puVar5;
    ushort  *puVar6;
    ushort  *puVar7;
    ushort  *local_res8;

    puVar3 = (ushort *)*param_2;
    puVar5 = (ushort *)0x0;
    puVar4 = (ushort *)0x0;
    puVar6 = puVar3;
    puVar7 = (ushort *)0x0;
    if (*puVar3 == 0x22) {
        puVar3 = puVar3 + 1;
        *param_2 = (longlong)puVar3;
        PECMD_SkipWCharUntil((WCHAR **)param_2, 0x22);
        psVar1 = (short *)*param_2;
        if (*psVar1 != 0x22) {
            if (*psVar1 != 0) {
                *psVar1 = 0;
                *param_2 = *param_2 + 2;
            }
            *param_2 = (longlong)puVar3;
            goto LAB_140083b8f;
        }
        if (psVar1[1] != 0x3a) {
            *psVar1 = 0;
            *param_2 = *param_2 + 2;
            local_res8 = (ushort *)*param_2;
            PECMD_SkipLeadingControlChars((WCHAR **)&local_res8);
            *param_2 = (longlong)puVar3;
            puVar5 = local_res8;
            goto LAB_140083b8f;
        }
        puVar6 = puVar3 + 1;
        memmove((void *)puVar6, (const void *)puVar3,
                      (int)(((longlong)psVar1 - (longlong)puVar3) >> 1) * 2);
        puVar3 = (ushort *)*param_2;
        puVar4 = puVar3;
        puVar7 = puVar3;
    }
    while ((*puVar3 != 0 &&
           (((puVar3 = (ushort *)*param_2, *puVar3 < 9 || (0xd < *puVar3)) &&
             (*puVar3 != 0x20))))) {
        if (*puVar3 == 0x22) {
            goto LAB_140083b58;
        }
        puVar3 = puVar3 + 1;
        *param_2 = (longlong)puVar3;
    }
    puVar3 = puVar6;
    if (puVar4 != (ushort *)0x0) {
        puVar3 = puVar4;
    }
    *param_2 = (longlong)puVar3;
    puVar6 = (ushort *)((ulonglong)puVar6 & -(ulonglong)(puVar4 != (ushort *)0x0));
    puVar3 = puVar7;
LAB_140083b58:
    puVar5 = PECMD_NextToken(param_2, (int64_t *)0x0, 0x2cd);
    if (puVar6 != (ushort *)0x0) {
        iVar2 = lstrlenW((LPCWSTR)*param_2);
        memmove((void *)puVar3, (const void *)*param_2, iVar2 * 2 + 2);
        *param_2 = (longlong)puVar6;
    }
LAB_140083b8f:
    if ((*(char *)((longlong)param_1 + 0xda) == '\0') &&
        ((*(byte *)((longlong)param_1 + 0xd) & 0xf) == 0)) {
        PECMD_ExpandVarsRecursive((void *)param_1, (WCHAR *)*param_2, (WCHAR **)param_3, 0, 1);
    } else {
        PECMD_ExpandCommandLine((void *)param_1, (WCHAR *)*param_2, (WCHAR **)param_3, 0, 1);
    }
    *param_2 = *param_3;
    return puVar5;
}

/* ==========================================================================
 * PECMD_ShowBrowseFolder @0x140087690
 * Folder browse dialog (SHBrowseForFolderW).
 * ========================================================================== */
uint64_t PECMD_ShowBrowseFolder(longlong *param_1, ushort *param_2, longlong param_3,
                       longlong *param_4)
{
    undefined2  uVar22;
    WCHAR       WVar2;
    WCHAR       WVar24;
    bool        bVar3;
    bool        bVar26;
    BFFCALLBACK pBVar20;
    char        cVar5;
    char        cVar23;
    int         iVar6;
    BOOL        BVar7;
    longlong    lVar8;
    longlong   *plVar9;
    short      *psVar10;
    WCHAR      *pWVar11;
    longlong   *puVar12;
    HWND        pHVar13;
    WCHAR      *pWVar14;
    WCHAR      *pWVar15;
    int        *piVar16;
    LPITEMIDLIST pidl;
    ulonglong   uVar18;
    WCHAR      *puVar19;
    uint        uVar25;
    WCHAR      *pBVar17;      /* path string pointer (was BFFCALLBACK) */
    WCHAR      *pBVar21;      /* path string pointer (was BFFCALLBACK) */
    WCHAR      *pBVar4;       /* scan pointer (was BFFCALLBACK) */
    WCHAR      *local_res10;
    WCHAR      *local_5b0;
    WCHAR      *local_5a8;    /* callback/string union (data pointer) */
    WCHAR      *local_5a0;
    uint        local_598[2];
    WCHAR      *local_590;
    WCHAR      *local_588;    /* callback/string union (data pointer) */
    WCHAR      *local_580;    /* path string (was BFFCALLBACK) */
    WCHAR      *local_578;
    WCHAR      *local_570;
    WCHAR      *local_568;
    WCHAR      *local_560;
    longlong    local_558;
    longlong   *local_550;
    undefined8  local_548;
    WCHAR      *local_540;
    undefined4  local_538[2];
    HWND        local_530;
    ulonglong   local_528;
    WCHAR      *local_520;
    undefined4  local_50c;
    BFFCALLBACK local_508;
    int         local_500;
    undefined8  local_4f8;
    undefined4  local_4f0;
    undefined8  local_4e8;
    ulonglong   local_4e0;
    uint        local_4d8;
    BFFCALLBACK local_4c0;
    BROWSEINFOW local_498;
    WCHAR       local_458[264];
    WCHAR       local_248[264];

    local_res10 = (WCHAR *)param_2;
    PECMD_AllocWStringBuffer((WCHAR **)&local_568, 0x20a);
    GetCurrentDirectoryW(0x208, local_568);
    local_5a8 = (WCHAR *)0x0;
    local_588 = (WCHAR *)0x0;
    PECMD_AllocStrSlot((WCHAR **)&local_558);
    local_548 = 0;
    (void)local_548;
    local_550 = param_1;
    (void)local_550;
    PECMD_SkipLeadingControlChars(&local_res10);
    cVar5 = PECMD_MatchTokenAdvance("-fix", &local_res10, 4);
    PECMD_StrDupAssign(&local_590, (const WCHAR *)g_szEmpty);
    PECMD_StrDupAssign(&local_560, (const WCHAR *)g_szEmpty);
    PECMD_StrDupAssign(&local_578, (const WCHAR *)g_szEmpty);
    PECMD_StrDupAssign(&local_570, (const WCHAR *)g_szEmpty);
    local_598[0] = 0;
    cVar23 = '\0';
    bVar3 = false;
    puVar19 = local_res10;
    lVar8 = PECMD_TokPrefixICmp("*raw", (const WCHAR *)local_res10, 4);
    bVar26 = ((char)lVar8 != '\0');
    if (bVar26) {
        local_res10 = puVar19 + 4;
        PECMD_SkipLeadingControlChars(&local_res10);
    }
    plVar9 = PECMD_SplitTokenAssignVar((WCHAR **)&local_558, &local_res10, 0x2c, 1);
    PECMD_SplitTokenTrimWs(plVar9, &local_590, 0);
    PECMD_RunCommandLine((void *)param_1, &local_590, 0);
    puVar19 = local_res10;
    if (*local_590 == L'\0') goto LAB_14008829d;
    psVar10 = (short *)FUN_140063060(local_res10);
    if (*psVar10 == 0x2c) {
        local_res10 = puVar19 + 1;
    }
    PECMD_SkipLeadingControlChars(&local_res10);
    WVar2 = *local_res10;
    while (WVar2 != 0) {
        if (WVar2 == 0x26) {
            bVar3 = true;
        } else {
            if (WVar2 != 0x2a) break;
            cVar23 = (char)(cVar23 + 1);
        }
        local_res10 = local_res10 + 1;
        WVar2 = *local_res10;
    }
    PECMD_SkipLeadingControlChars(&local_res10);
    plVar9 = PECMD_SplitTokenAssignVar((WCHAR **)&local_558, &local_res10, 0x2c, 1);
    PECMD_SplitTokenTrimWs(plVar9, &local_560, 0);
    pWVar11 = PECMD_UnquoteString(local_560);
    puVar19 = local_res10;
    psVar10 = (short *)FUN_140063060(local_res10);
    if (*psVar10 == 0x2c) {
        local_res10 = puVar19 + 1;
        plVar9 = PECMD_SplitTokenAssignVar((WCHAR **)&local_558, &local_res10, 0x2c, 1);
        PECMD_SplitTokenTrimWs(plVar9, &local_578, 0);
        puVar19 = local_res10;
    }
    psVar10 = (short *)FUN_140063060(puVar19);
    if (*psVar10 == 0x2c) {
        local_res10 = puVar19 + 1;
        plVar9 = PECMD_SplitTokenAssignVar((WCHAR **)&local_558, &local_res10, 0x2c, 1);
        PECMD_SplitTokenTrimWs(plVar9, &local_570, 0);
        puVar19 = local_res10;
    }
    psVar10 = (short *)FUN_140063060(puVar19);
    uVar25 = 0;
    if (*psVar10 == 0x2c) {
        local_res10 = puVar19 + 1;
        puVar12 = PECMD_SplitTokenAssignVar((WCHAR **)&local_558, &local_res10, 0x2c, 1);
        PECMD_ParseShortStore((WCHAR **)puVar12, (int *)local_598, 0x2c);
        puVar19 = local_res10;
        uVar25 = local_598[0];
    }
    psVar10 = (short *)FUN_140063060(puVar19);
    if (*psVar10 == 0x2c) {
        local_res10 = puVar19 + 1;
        puVar12 = PECMD_SplitTokenAssignVar((WCHAR **)&local_558, &local_res10, 0x2c, 1);
        PECMD_ParseSignedNumberStr((void **)puVar12, (void *)&local_5a8, 0x2c);
        puVar19 = local_res10;
    }
    psVar10 = (short *)FUN_140063060(puVar19);
    if (*psVar10 == 0x2c) {
        local_res10 = puVar19 + 1;
        puVar12 = PECMD_SplitTokenAssignVar((WCHAR **)&local_558, &local_res10, 0x2c, 1);
        PECMD_ParseSignedNumberStr((void **)puVar12, (void *)&local_588, 0x2c);
    }
    PECMD_StrDupAssign(&local_580, pWVar11);
    pBVar17 = local_580;
    pBVar21 = (WCHAR *)0x0;
    if ((((*local_580 != L'\0') && (pBVar21 = pBVar17, cVar5 != '\0')) &&
        (iVar6 = lstrlenW((LPCWSTR)local_580), *pBVar17 != L'\0')) &&
       (((pBVar17[1] == L':' && (pBVar17[2] == L'\\')) && (pBVar17[3] != L'.')))) {
        PECMD_AllocString(&local_580, (longlong)(iVar6 + 8));
        pBVar17 = local_580;
        memmove((void *)(local_580 + 4), (const void *)(local_580 + 2),
                      (iVar6 + 2) * 2);
        pBVar17[3] = L'.';
        pBVar21 = pBVar17;
    }
    pWVar11 = StrChrW(local_590, L';');
    if (pWVar11 != (LPWSTR)0x0) {
        *pWVar11 = L'\0';
        pWVar11 = pWVar11 + 1;
    }
    PECMD_AllocStrSlot(&local_5b0);
    if ((((*(byte *)((longlong)param_1 + 0xd) & 0xf) == 0) && (*local_590 != L'&')) ||
        (lVar8 = (longlong)PECMD_VarLookup((void *)param_1, local_590, (void *)0x0,
                                         -1, (void **)0x0), lVar8 == 0)) {
        PECMD_SetVariable(param_1, local_590, (LPCWSTR)g_szEmpty);
    } else {
        uVar18 = *(ulonglong *)(lVar8 + 0x18) & 0x3fffffffffffffff;
        if (uVar18 == 1) {
            **(undefined1 **)(lVar8 + 8) = 0;
        } else if (1 < uVar18) {
            **(undefined2 **)(lVar8 + 8) = 0;
        }
    }
    pBVar20 = (BFFCALLBACK)local_5a8;
    if (local_5a8 != (WCHAR *)0x0) {
        local_5a0 = (WCHAR *)0x0;
        if (pWVar11 == (LPWSTR)0x0) {
            PECMD_AssignString(&local_5a0, local_590);
            PECMD_AppendWideStr(&local_5a0, WSTR(".flg"));
            pWVar11 = local_5a0;
            lVar8 = (longlong)PECMD_VarLookup((void *)param_1, local_5a0, (void *)0x0,
                                             -1, (void **)0x0);
            if (lVar8 != 0) goto LAB_140087be8;
        } else {
LAB_140087be8:
            PECMD_SetVariable(param_1, pWVar11, WSTR("0"));
        }
        PECMD_FreeStrBuf((void *)&local_5a0);
    }
    pHVar13 = (HWND)PECMD_QueryState_f414(param_3);
    if (cVar23 == '\x01') {
        memset((void *)&local_498.pidlRoot, 0, 0x38);
        local_498.ulFlags = uVar25 ^ 0x40;
        local_498.pidlRoot = (LPCITEMIDLIST)0x0;
        local_458[0] = L'\0';
        local_248[0] = L'\0';
        local_498.pszDisplayName = local_248;
        local_498.lpszTitle = local_578;
        local_498.lpfn = pBVar20;
        local_498.lParam = (LPARAM)local_588;
        if (pBVar21 != (WCHAR *)0x0) {
            local_498.lpfn = PECMD_ControlMessage;
            local_498.lParam = (LPARAM)pBVar21;
        }
        local_498.hwndOwner = pHVar13;
        pidl = SHBrowseForFolderW(&local_498);
        if (pidl != (LPITEMIDLIST)0x0) {
            BVar7 = SHGetPathFromIDListW(pidl, local_458);
            if (BVar7 != 0) {
                PECMD_AssignString(&local_5b0, (const WCHAR *)local_458);
            }
            PECMD_GetApiProcCached("CoTaskMemFree", "OLE32", (void **)&g_pCoTaskMemFree,
                          (uintptr_t *)&g_hOle32);
            if (g_pCoTaskMemFree != (void (*)(LPVOID))0x0) {
                g_pCoTaskMemFree(pidl);
            }
        }
LAB_14008822a:
        if (pBVar20 == (BFFCALLBACK)0x0) goto LAB_14008822f;
    } else {
        PECMD_StrBldCopyAnsi((void *)&local_5a0, "*.", 0xffffffffffffffffull);
        WVar24 = *local_570;
        pWVar11 = (WCHAR *)0x0;
        if (WVar24 != L'\0') {
            pWVar14 = StrChrW(local_570, L'|');
            if (pWVar14 == (LPWSTR)0x0) {
                PECMD_AppendWideStr(&local_5a0, local_570);
                iVar6 = lstrlenW(local_5a0);
                PECMD_AllocString(&local_5a0, (longlong)(iVar6 * 2 + 6));
                pWVar11 = local_5a0;
                memcpy((void *)(local_5a0 + (iVar6 + 1)),
                              (const void *)local_5a0, (iVar6 + 1) * 2);
                pWVar11[(longlong)iVar6 * 2 + 2] = L'\0';
            } else {
                PECMD_AssignString(&local_5a0, local_570);
                PECMD_AppendWideStr(&local_5a0, WSTR("|"));
                WVar2 = *local_5a0;
                pWVar15 = local_5a0;
                while (pWVar11 = local_5a0, WVar24 = L'\0', WVar2 != L'\0') {
                    if (*pWVar15 == L'|') {
                        *pWVar15 = L'\0';
                    }
                    pWVar15 = pWVar15 + 1;
                    WVar2 = *pWVar15;
                }
            }
        }
        iVar6 = lstrlenW((LPCWSTR)pBVar17);
        local_500 = iVar6 * 2 + 0x10;
        if (local_500 < 0x3f7a1) {
            local_500 = 0x3f7a2;
        }
        PECMD_AllocString(&local_580, (longlong)(local_500 + 8));
        memset((void *)(local_580 + (longlong)iVar6 * 2), 0,
               (longlong)((local_500 - iVar6) + 8) * 2);
        local_538[0] = 0x98;
        memset((void *)&local_528, 0, 0x88);
        pBVar20 = (BFFCALLBACK)local_5a8;
        local_508 = (BFFCALLBACK)local_580;
        local_4e0 = -(ulonglong)(*local_578 != L'\0') & (ulonglong)local_578;
        (void)local_4e0;
        local_50c = 0;
        (void)local_50c;
        local_4f8 = 0;
        (void)local_4f8;
        local_4f0 = 0;
        (void)local_4f0;
        local_4e8 = 0;
        (void)local_4e8;
        local_598[0] = 0;
        local_4d8 = local_4d8 | uVar25;
        if (local_5a8 != (WCHAR *)0x0) {
            local_4d8 = local_4d8 | 0x20;
            local_4c0 = (BFFCALLBACK)local_5a8;
            (void)local_4c0;
        }
        if ((local_4d8 >> 0x12 & 1) == 0) {
            local_4d8 = local_4d8 | 0x200000;
        }
        local_530 = pHVar13;
        (void)local_530;
        local_520 = pWVar11;
        (void)local_520;
        PECMD_LoadComDlgApis();
        if (bVar3) {
            local_598[0] = (uint)((int (*)(void))g_pGetSaveFileNameW)();
        } else {
            local_598[0] = (uint)((int (*)(void *))g_i64D428)(local_538);
        }
        if (pBVar20 != (BFFCALLBACK)0x0) {
LAB_140088096:
            if (((((local_4d8 >> 9 & 1) == 0) && (!bVar26)) && (bVar3)) && (WVar24 != L'\0')) {
                PECMD_CopyStrToSlot((uint64_t *)&local_5a8, (uint64_t *)&local_5b0);
                psVar10 = PECMD_LastPathSeparator((short *)local_5a8);
                pBVar17 = (WCHAR *)local_5a8;
                if (psVar10 != (short *)0x0) {
                    pBVar17 = (WCHAR *)(psVar10 + 1);
                }
                pWVar14 = StrRChrW((LPCWSTR)pBVar17, (LPCWSTR)0x0, L'.');
                if (pWVar14 == (LPWSTR)0x0) {
                    PECMD_AppendWideStr(&local_5b0, WSTR("."));
                    PECMD_AppendWideStr(&local_5b0, local_570);
                }
                PECMD_FreeStrBuf((void *)&local_5a8);
            }
            PECMD_FreeStrBuf((void *)&local_5a0);
            goto LAB_14008822a;
        }
        piVar16 = (int *)FUN_140063060(local_598);
        pBVar17 = (WCHAR *)local_508;
        if (*piVar16 == 1) {
            if ((local_4d8 >> 9 & 1) == 0) {
                PECMD_AssignString(&local_5b0, (LPCWSTR)local_508);
            } else if ((local_4d8 >> 0x13 & 1) == 0) {
                WVar2 = *(WCHAR *)local_508;
                local_5a8 = (WCHAR *)local_508;
                while ((WVar2 != L'\0' &&
                       ((((ushort)WVar2 < 9 || (0xd < (ushort)WVar2)) && (WVar2 != L' '))))) {
                    local_5a8 = local_5a8 + 1;
                    WVar2 = *(WCHAR *)local_5a8;
                }
                iVar6 = (int)(local_5a8 - (WCHAR *)local_508);
                PECMD_StrBldCopyWideN(&local_588, (const WCHAR *)local_508, (int64_t)(iVar6 + 1));
                pBVar21 = local_588;
                if ((iVar6 < 1) || (uVar22 = 0x5c,
                    *(short *)(pBVar17 + (longlong)iVar6 + -1) == 0x5c)) {
                    uVar22 = 0;
                }
                local_588[(longlong)iVar6] = (WCHAR)uVar22;
                PECMD_SkipLeadingControlChars(&local_5a8);
                if (*(WCHAR *)local_5a8 == L'\0') {
                    PECMD_AssignString(&local_5b0, (LPCWSTR)local_508);
                } else {
                    do {
                        pBVar17 = local_5a8;
                        if (*local_5b0 != L'\0') {
                            PECMD_AppendWideStr(&local_5b0, WSTR("\r\n"));
                        }
                        PECMD_AppendWideStr(&local_5b0, (const WCHAR *)pBVar21);
                        WVar2 = *(WCHAR *)pBVar17;
                        pBVar4 = pBVar17;
                        while (((WVar2 != L'\0' && (((ushort)WVar2 < 9 ||
                                (0xd < (ushort)WVar2)))) && (WVar2 != L' '))) {
                            local_5a8 = pBVar4 + 1;
                            pBVar4 = local_5a8;
                            WVar2 = *(WCHAR *)local_5a8;
                        }
                        PECMD_StrBldCopyWideN(&local_540, (const WCHAR *)pBVar17,
                                      (int64_t)(pBVar4 - pBVar17));
                        PECMD_AppendWideStr(&local_5b0, (const WCHAR *)local_540);
                        PECMD_SkipLeadingControlChars(&local_5a8);
                        PECMD_FreeStrBuf((void *)&local_540);
                    } while (*(WCHAR *)local_5a8 != L'\0');
                }
                PECMD_FreeStrBuf((void *)&local_588);
            } else {
                iVar6 = lstrlenW((LPCWSTR)local_508);
                pBVar21 = pBVar17 + (longlong)iVar6 + 1;
                if (*(WCHAR *)pBVar21 == L'\0') {
                    PECMD_AssignString(&local_5b0, (LPCWSTR)local_508);
                    pBVar20 = (BFFCALLBACK)local_5a8;
                } else {
                    do {
                        if (*local_5b0 != L'\0') {
                            PECMD_AppendWideStr(&local_5b0, WSTR("\r\n"));
                        }
                        PECMD_AppendWideStr(&local_5b0, (const WCHAR *)pBVar17);
                        pWVar11 = local_5b0;
                        iVar6 = lstrlenW(local_5b0);
                        if ((0 < iVar6) && (pWVar11[(longlong)iVar6 + -1] != L'\\')) {
                            PECMD_AppendWideStr(&local_5b0, WSTR("\\"));
                        }
                        PECMD_AppendWideStr(&local_5b0, (const WCHAR *)pBVar21);
                        iVar6 = lstrlenW((const WCHAR *)pBVar21);
                        pBVar21 = pBVar21 + (longlong)iVar6 + 1;
                        pBVar20 = (BFFCALLBACK)local_5a8;
                    } while (*(WCHAR *)pBVar21 != L'\0');
                }
            }
            goto LAB_140088096;
        }
        PECMD_FreeStrBuf((void *)&local_5a0);
LAB_14008822f:
        if (param_4 == (longlong *)0x0) {
            PECMD_StrDupAssign(&local_588, local_5b0);
            PECMD_SetVariable(param_1, local_590, (LPCWSTR)local_588);
            PECMD_FreeStrBuf((void *)&local_588);
        } else {
            PECMD_StrBldCopyWide(param_4, local_5b0);
        }
    }
    SetCurrentDirectoryW(local_568);
    PECMD_SyncWorkingDirectory();
    PECMD_FreeStrBuf((void *)&local_5b0);
    PECMD_FreeStrBuf((void *)&local_580);
LAB_14008829d:
    PECMD_FreeStrBuf((void *)&local_570);
    PECMD_FreeStrBuf((void *)&local_578);
    PECMD_FreeStrBuf((void *)&local_560);
    PECMD_FreeStrBuf((void *)&local_590);
    PECMD_FreeStrBuf((void *)&local_558);
    PECMD_FreeStrBuf((void *)&local_568);
    return 0;
}

/* ==========================================================================
 * PECMD_EnumeratePhysicalDrives @0x14008ac48
 * Enumerate physical drives (\\\\.\\PhysicalDriveN).
 * ========================================================================== */
uint32_t PECMD_EnumeratePhysicalDrives(longlong *param_1, uint param_2)
{
    bool       bVar1;
    WCHAR     *pWVar2;
    HANDLE     hObject;
    uint       uVar3;
    DWORD      DVar4;
    undefined4 uVar5;
    ulonglong *puVar6;
    undefined *puVar7;
    longlong   lVar8;
    const WCHAR *pwVar9;
    uint       uVar10;
    int        iVar11;
    ulonglong  uVar12;
    uint       uVar13;
    uint       uVar14;
    ulonglong *puVar15;
    int        iVar16;
    uint       local_res10[2];
    uint       local_res18;
    uint       local_res20;
    WCHAR     *local_308;
    uint       local_300;
    HANDLE     local_2f8;
    uint       local_2f0;
    ulonglong *local_2e8;
    uint       local_2e0;
    ulonglong *local_2d8;
    WCHAR     *local_2d0;
    WCHAR     *local_2c8;
    WCHAR     *local_2c0;
    WCHAR     *local_2b8;
    undefined *local_2b0;
    longlong   local_2a8;
    undefined4 local_2a0;
    undefined4 auStack_29c[9];
    undefined4 local_278;
    undefined2 local_274;
    ulonglong  local_272[21];
    WCHAR      local_1c8[64];
    WCHAR      local_148[64];
    WCHAR      local_c8[68];

    (void)uVar10;
    EnterCriticalSection((LPCRITICAL_SECTION)&g_csInit);
    iVar16 = 0;
    uVar3 = param_2 >> 0x10 & 1;
    uVar14 = param_2 >> 0x13 & 1;
    uVar10 = param_2 >> 0x11 & 1;
    local_300 = param_2 >> 0x12 & 1;
    local_res10[0] = 0;
    local_2e0 = param_2 & 0x200000;
    uVar13 = param_2 & 0xc000ffff;
    local_2f0 = param_2 & 0x100000;
    *(undefined2 *)*param_1 = 0;
    local_res18 = uVar14;
    local_res20 = uVar10;
    PECMD_AllocStringSlot2((void **)&local_2d8, 0x1100d8);
    local_2e8 = local_2d8 + 0x20002;
    local_308 = (WCHAR *)(local_2d8 + 0x21802);
    *local_308 = L'\0';
    local_2b8 = (WCHAR *)(local_2d8 + 0x2181b);
    local_2d0 = local_2b8 + (int)(uVar3 << 10);
    local_2c0 = local_2d0 + (int)(uVar14 << 10);
    local_2c8 = local_2c0 + (int)(uVar10 << 10);
    *local_2c8 = L' ';
    *local_2c0 = L' ';
    *local_2d0 = L' ';
    *local_2b8 = L' ';
    local_2c8[1] = L'\0';
    local_2c0[1] = L'\0';
    local_2d0[1] = L'\0';
    *(undefined2 *)((longlong)local_2d8 + 0x10c0da) = 0;
    if ((int)uVar13 < 0) {
        uVar12 = 0;
        goto LAB_14008ada9;
    }
    uVar12 = (ulonglong)uVar13;
    do {
        pWVar2 = local_308;
        iVar11 = (int)uVar12;
        /* TODO(verify): format needs drive number (arg recovered by Ghidra) */
        wsprintfW(local_308, WSTR("\\\\.\\PhysicalDrive%d"), (int)uVar12);
        if ((uVar3 != 0) && (-1 < (int)uVar13)) {
            PECMD_EnumDevices((LPCWSTR)(longlong)iVar11, pWVar2 + 0x65, 0x13,
                          (GUID *)&g_bE870);
        }
        if ((uVar14 != 0) && (-1 < (int)uVar13)) {
            PECMD_EnumDevices((LPCWSTR)(longlong)iVar11, local_2d0 + 1, 0x14,
                          (GUID *)&g_bE870);
        }
        local_2f8 = (HANDLE)0x0;
        if ((local_res20 == 0) || (bVar1 = true, (int)uVar13 < 0)) {
            bVar1 = false;
        }
        PECMD_OpenFileHandle(&local_2f8, pWVar2,
                      (-(uint)bVar1 & 0x40000000) + 0x80000000, 3,
                      (LPSECURITY_ATTRIBUTES)0x0, 3, 0x20000000, (HANDLE)0x0);
        hObject = local_2f8;
        if (bVar1) {
            PECMD_QueryDevice(local_2f8, uVar12, local_2c0 + 1, 0);
        }
        if (local_300 == 0) {
LAB_14008aeda:
            if (-1 < (int)uVar13) {
                uVar10 = local_res10[0];
                if (hObject != (HANDLE)0x0) {
                    SetLastError(0);
                    memset((void *)local_2d8, 0, 0xc0);
                    local_res10[0] = 0;
                    puVar6 = PECMD_GetDiskLayoutInfo(hObject, local_2d8, local_res10);
                    puVar15 = local_2d8;
                    if (puVar6 != (ulonglong *)0x0) {
                        puVar15 = puVar6;
                    }
                    local_278 = g_u32127750;
                    local_274 = g_u1627754;
                    (void)local_274;
                    memset(local_272, 0, 0x9e);
                    iVar16 = (int)*puVar15;
                    puVar7 = &DAT_140127740;
                    if (iVar16 != 0) {
                        puVar7 = &g_b127748;
                    }
                    local_2b0 = &DAT_140127738;
                    (void)local_2b0;
                    if (iVar16 != 1) {
                        local_2b0 = puVar7;
                        (void)local_2b0;
                    }
                    if (iVar16 == 0) {
                        wsprintfW((LPWSTR)&local_278, WSTR("%08lX"),
                                  (int)puVar15[1]);
                    } else if (iVar16 == 1) {
                        PECMD_GuidToString((LPWSTR)&local_278,
                                      (uint32_t *)(puVar15 + 1), 0);
                    }
                    local_2a0 = 0;
                    memset(auStack_29c, 0, 0x24);
                    local_2a8 = PECMD_GetDiskGeometrySize(hObject, (void *)&local_2a0);
                    DVar4 = PECMD_QueryDeviceIoInfo(hObject, 0x200);
                    if (iVar16 == 1) {
                        PECMD_ReadDiskSectorScan((uint64_t *)&local_2f8, local_2e8,
                                      (uint32_t)puVar15[5], DVar4, 0,
                                      (uint64_t *)0x0,
                                      (LARGE_INTEGER){ .QuadPart = 0 });
                    }
                    local_308 = (WCHAR *)0x0;
                    local_2e8 = (ulonglong *)0x0;
                    uVar5 = PECMD_FindPartitionInfo((int64_t)(uintptr_t)(puVar15 + 6),
                                          (uint8_t)(iVar16 == 1),
                                          local_res10[0], (void *)&local_308,
                                          (void *)&local_2e8, DVar4, iVar16,
                                          (void *)&local_2f8);
                    PECMD_GetDiskGeometry((LPCWSTR)0x0, hObject);
                    PECMD_AllocString((WCHAR **)param_1, 200);
                    PECMD_QueryDiskGeometry(hObject, (ulonglong *)0x0, 1, 0x800);
                    local_1c8[0] = L'\0';
                    if (local_2e0 != 0) {
                        local_308 = (WCHAR *)0xffffffffffffffffull;
                        local_res10[0] = 0;
                        lVar8 = PECMD_FindPartitionStartSector(hObject, (int *)local_res10,
                                              (longlong *)&local_308);
                        wsprintfW(local_1c8, WSTR(" %I64d %u %I64d"),
                                  lVar8, local_res10[0]);
                    }
                    PECMD_FormatI64Dec(local_c8, (uint64_t)local_2a8);
                    PECMD_FormatI64Dec(local_148,
                                  CONCAT44(auStack_29c[0], local_2a0));
                    pwVar9 = WSTR("%2d %14s %14s %6lu %4lu %4lu %4lu %s %2d %s %c%s");
                    if (local_2f0 != 0) {
                        pwVar9 = WSTR("%-2d %14s %14s %6lu %4lu %4lu %4lu %s %2d %s %c%s");
                    }
                    /* TODO(verify): format has 12 specs; only uVar12 & local_c8
                       recovered by Ghidra; remaining varargs lost. */
                    uVar10 = wsprintfW((LPWSTR)*param_1, pwVar9,
                                       (int)uVar12, local_c8);
                    if (uVar3 != 0) {
                        PECMD_AppendWideStr((WCHAR **)param_1, local_2b8);
                    }
                    if (local_res18 != 0) {
                        PECMD_AppendWideStr((WCHAR **)param_1, local_2d0);
                    }
                    if (local_res20 != 0) {
                        PECMD_AppendWideStr((WCHAR **)param_1, local_2c0);
                    }
                    if (local_300 != 0) {
                        PECMD_AppendWideStr((WCHAR **)param_1, local_2c8);
                    }
                    if (-1 < (char)uVar5) {
                        PECMD_AppendWideStr((WCHAR **)param_1, WSTR(" D"));
                    }
                    if (hObject != (HANDLE)0xffffffffffffffff) {
                        CloseHandle(hObject);
                    }
                }
                break;
            }
        } else if (-1 < (int)uVar13) {
            PECMD_QueryDevice(hObject, uVar12, local_2c8 + 1, 1);
            goto LAB_14008aeda;
        }
        pWVar2 = local_308;
        if (hObject == (HANDLE)0x0) {
            iVar16 = iVar16 + 1;
            uVar10 = local_res10[0];
            if (0x1f < iVar16) break;
        } else {
            iVar16 = 0;
            wsprintfW(local_308, WSTR("%d "), (int)uVar12);
            PECMD_AppendWideStr((WCHAR **)param_1, pWVar2);
            local_res10[0] = local_res10[0] + 1;
            if (hObject != (HANDLE)0xffffffffffffffff) {
                CloseHandle(hObject);
            }
        }
        uVar12 = (ulonglong)(iVar11 + 1);
        uVar14 = local_res18;
LAB_14008ada9:
        uVar10 = local_res10[0];
    } while ((int)uVar12 < 16000);
    PECMD_StripTrailingSpaces((LPCWSTR)*param_1);
    PECMD_FreeStrBuf((void *)&local_2d8);
    LeaveCriticalSection((LPCRITICAL_SECTION)&g_csInit);
    return uVar10;
}
