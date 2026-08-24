/* Restored group 9/10 business functions (partE) */
#include "win32_stub.h"
#include "pecmd_defs.h"

/* ---- extern declarations for symbols not in win32_stub.h / pecmd_defs.h ---- */

/* globals used by PECMD_ParseFontOptions (doubles) */
extern double g_dpiBase;   /* _DAT_140126078 */
extern double g_fontRound;
extern uint32_t g_dpi;
extern double g_fontMinus0;

/* internal helpers (bodies provided elsewhere; refactored PECMD_* aliases are
 * already declared in pecmd_defs.h and are used for the confirmed mappings) */
extern LPCWSTR PECMD_UnquoteString(LPCWSTR);                       /* string tag/prefix lookup */
extern void    PECMD_SkipWCharUntil(void *, int);                   /* delimiter scan ('#') */
extern int     PECMD_ParseNumSkipChar_0224(void *, int *);                 /* parse int field */
extern int     PECMD_ParseNumSkipChar_de4c(void *, double *);              /* parse double field */
extern int     PECMD_OnDeleteCommand(void *, LPCWSTR, HWND);         /* list-control accept check */
extern int     PECMD_DispatchControlCommand(void *, LPCWSTR, WPARAM, HWND, LPCWSTR, void *, int64_t *, HWND, int64_t);
extern int     PECMD_ParseUIntValue(LPCWSTR *, int *);              /* scan list token */
extern void    PECMD_AllocStrSlot(void *);                        /* string release/cleanup */
extern void    PECMD_SplitTokenTrimWs(void *, void *, int);           /* split list string on delim */

#ifndef SE_REGISTRY_KEY
#define SE_REGISTRY_KEY 5
#endif

/* ========== PECMD_SetRegistryKeySecurity @ 140096f84 ========== */
/* signature: LSTATUS __fastcall PECMD_SetRegistryKeySecurity(HKEY param_1, PACL param_2, undefined8 * param_3) */
int PECMD_SetRegistryKeySecurity(HKEY param_1, void *param_2, WCHAR **param_3)
{
    int LVar1;
    DWORD dwIndex;
    DWORD local_res20[2];
    DWORD local_38;
    DWORD local_34;
    LPWSTR local_30;
    HKEY local_28;
    LPWSTR local_20;

    dwIndex = 0;
    local_38 = 0;
    local_res20[0] = 0;
    LVar1 = RegQueryInfoKeyW(param_1, (LPWSTR)0, (DWORD *)0, (DWORD *)0, &local_38,
                             local_res20, (DWORD *)0, (DWORD *)0, (DWORD *)0,
                             (DWORD *)0, (DWORD *)0, (FILETIME *)0);
    if (LVar1 == 0) {
        PECMD_StrAlloc2(&local_20, (int64_t)(local_res20[0] * 4 + 8));
        if (0 < (int)local_38) {
            do {
                local_28 = (HKEY)0;
                local_34 = local_res20[0] << 2;
                *local_20 = L'\0';
                LVar1 = RegEnumKeyExW(param_1, dwIndex, local_20, &local_34, (DWORD *)0,
                                      (LPWSTR)0, (DWORD *)0, (FILETIME *)0);
                if ((LVar1 == 0) && (*local_20 != L'\0')) {
                    PECMD_StrAssign(&local_30, *param_3);
                    PECMD_StrCat(&local_30, WSTR("\\"));
                    PECMD_StrCat(&local_30, local_20);
                    SetNamedSecurityInfoW(local_30, SE_REGISTRY_KEY, 4, (void *)0,
                                          (void *)0, param_2, (void *)0);
                    LVar1 = RegOpenKeyExW(param_1, local_20, 0, 0xf003f, &local_28);
                    if ((local_28 != (HKEY)0) &&
                        (LVar1 = PECMD_SetRegistryKeySecurity(local_28, param_2, &local_30),
                         local_28 != (HKEY)0)) {
                        RegCloseKey(local_28);
                    }
                    PECMD_StrFree(&local_30);
                }
                dwIndex = dwIndex + 1;
            } while ((int)dwIndex < (int)local_38);
        }
        PECMD_StrFree(&local_20);
    }
    return LVar1;
}

/* ========== PECMD_ListControlCommand @ 14009d1b0 ========== */
/* signature: undefined8 __fastcall PECMD_ListControlCommand(undefined8 * param_1, longlong * param_2,
 *   short * param_3, LPCWSTR param_4, LPCWSTR param_5, WPARAM param_6, undefined8 * param_7) */
uint64_t PECMD_ListControlCommand(uint64_t *param_1, int64_t *param_2, short *param_3,
                         LPCWSTR param_4, LPCWSTR param_5, WPARAM param_6,
                         uint64_t *param_7)
{
    HWND hWnd;
    HWND pHVar1;
    void *lpCriticalSection;
    uint64_t local_res8;
    LPWSTR local_78;
    int64_t *local_70;
    LPCWSTR local_68;
    int64_t local_60;
    uint64_t *local_58;
    uint64_t local_50;
    uint64_t local_48;
    uint64_t local_40;
    LPCWSTR local_res20;
    int bVar3;
    int iVar4;
    int iVar7;
    char cVar2;
    char cVar6;

    hWnd = *(HWND *)((char *)param_1[7] + 0x20);
    pHVar1 = *(HWND *)(*(int64_t *)((char *)param_1[10] + 0x40) + 0x20);
    local_res20 = param_4;
    bVar3 = PECMD_OnDeleteCommand(param_1, param_5, pHVar1);
    if (bVar3 == 0) {
        return 0;
    }
    lpCriticalSection = (void *)*param_7;
    if (lpCriticalSection != (void *)0) {
        *param_7 = 0;
        LeaveCriticalSection(lpCriticalSection);
    }
    if (PECMD_DispatchControlCommand((void *)param_1[7], param_5, param_6, hWnd, param_4, (void *)param_1[10],
                      param_2, pHVar1, (int64_t)param_1) != 0) {
        return 0;
    }
    iVar4 = lstrcmpiW(param_5, WSTR("QUERY"));
    if (iVar4 == 0) {
        local_res8 = (uint64_t)(uint32_t)-100;
        bVar3 = PECMD_ParseUIntValue(&local_res20, (int *)&local_res8);
        iVar7 = (int)(uint32_t)local_res8 - 1;
        iVar4 = (int)bVar3;
        if ((0 < iVar4) && (-1 < iVar7)) {
            iVar4 = -1;
        }
        PECMD_AllocStrSlot((void *)&local_res8);
        if ((*local_res20 == L';') || (*local_res20 == L':')) {
            local_res20 = local_res20 + 1;
        }
        PECMD_SplitTokenTrimWs((void *)&local_res20, (void *)&local_res8, 0x3b);
        PECMD_AllocStrSlot((void *)&local_78);
        if (iVar4 < 1) {
            SendMessageW(hWnd, 0x462, (WPARAM)(long long)iVar7, (LPARAM)&local_78);
        }
        PECMD_SetVar((void *)param_2, (LPCWSTR)local_res8, local_78);
        PECMD_StrFree(&local_78);
        PECMD_StrFree((WCHAR **)&local_res8);
        return 0;
    }
    cVar6 = '0';
    iVar4 = lstrcmpiW(param_5, WSTR("ADD"));
    if (iVar4 == 0) {
        cVar6 = '*';
        if (*param_4 == L'\r') {
            param_4 = param_4 + 1;
        }
        cVar2 = '*';
        if (*param_4 != L'\n') goto LAB_14009d456;
    } else {
        iVar4 = lstrcmpiW(param_5, WSTR("DEL"));
        if (iVar4 == 0) {
            cVar6 = '-';
            goto LAB_14009d456;
        }
        iVar4 = lstrcmpiW(param_5, WSTR("SEL"));
        if (iVar4 == 0) {
            cVar6 = ':';
            goto LAB_14009d456;
        }
        iVar4 = lstrcmpiW(param_5, WSTR("ADDSEL"));
        if (iVar4 == 0) {
            cVar6 = '+';
            goto LAB_14009d456;
        }
        iVar4 = lstrcmpiW(param_5, WSTR("VAL"));
        if (iVar4 == 0) {
            cVar6 = '\0';
            goto LAB_14009d456;
        }
        iVar4 = lstrcmpiW(param_5, WSTR("ADD1"));
        if (iVar4 == 0) {
            cVar6 = '1';
            goto LAB_14009d456;
        }
        iVar4 = lstrcmpiW(param_5, WSTR("isel"));
        if (iVar4 == 0) {
            cVar6 = '#';
            goto LAB_14009d456;
        }
        if (*param_5 != L'\0') {
            return 0;
        }
        if (((*param_3 != 0x2b) && (*param_3 != 0x2d)) && (*param_3 != 0x3a)) {
            goto LAB_14009d456;
        }
        cVar2 = (char)*param_3;
    }
    cVar6 = cVar2;
    param_4 = param_4 + 1;
LAB_14009d456:
    local_60 = (int64_t)cVar6;
    local_50 = 0;
    local_48 = 0;
    local_40 = 0;
    local_70 = param_2;
    local_68 = param_4;
    local_58 = param_1;
    (void)local_68;
    (void)local_60;
    (void)local_58;
    (void)local_50;
    (void)local_48;
    (void)local_40;
    SendMessageW(hWnd, 0x459, (WPARAM)param_1[7], (LPARAM)&local_70);
    return 0;
}

/* ========== PECMD_ParseFontOptions @ 1400a8664 ========== */
/* signature: undefined __fastcall PECMD_ParseFontOptions(longlong param_1, LPWSTR param_2, uint * param_3) */
void PECMD_ParseFontOptions(int64_t param_1, LPWSTR param_2, uint32_t *param_3)
{
    WCHAR *lpStart;
    int iVar1;
    LPCWSTR lpString2;
    LPWSTR pWVar2;
    WCHAR WVar3;
    double dVar4;
    int local_res10[4];
    WCHAR *local_res20;
    double local_38[4];

    if ((param_2 != (LPWSTR)0) && (*param_2 != L'\0')) {
        WVar3 = *param_2;
        local_res20 = param_2;
        while (WVar3 != L'\0') {
            if ((WVar3 == L'*') && (local_res20[1] == L'*')) {
                *local_res20 = L'\0';
                local_res20 = local_res20 + 2;
                break;
            }
            local_res20 = local_res20 + 1;
            WVar3 = *local_res20;
        }
        lpString2 = PECMD_UnquoteString(param_2);
        if (lpString2 != param_2) {
            lstrcpyW(param_2, lpString2);
        }
        lpStart = local_res20;
        if (*local_res20 != L'\0') {
            PECMD_SkipWCharUntil(&local_res20, 0x23);
            if (*local_res20 != L'\0') {
                *local_res20 = L'\0';
                local_res20 = local_res20 + 1;
            }
            pWVar2 = StrChrW(lpStart, L'B');
            if (pWVar2 != (LPWSTR)0) {
                *(uint32_t *)(param_1 + 0x10) = 700;
            }
            pWVar2 = StrChrW(lpStart, L'b');
            if (pWVar2 != (LPWSTR)0) {
                *(uint32_t *)(param_1 + 0x10) = 400;
            }
            pWVar2 = StrChrW(lpStart, L'I');
            if (pWVar2 != (LPWSTR)0) {
                *(uint8_t *)(param_1 + 0x14) = 1;
            }
            pWVar2 = StrChrW(lpStart, L'i');
            if (pWVar2 != (LPWSTR)0) {
                *(uint8_t *)(param_1 + 0x14) = 0;
            }
            pWVar2 = StrChrW(lpStart, L'U');
            if (pWVar2 != (LPWSTR)0) {
                *(uint8_t *)(param_1 + 0x15) = 1;
            }
            pWVar2 = StrChrW(lpStart, L'u');
            if (pWVar2 != (LPWSTR)0) {
                *(uint8_t *)(param_1 + 0x15) = 0;
            }
            pWVar2 = StrChrW(lpStart, L'S');
            if (pWVar2 != (LPWSTR)0) {
                *(uint8_t *)(param_1 + 0x16) = 1;
            }
            pWVar2 = StrChrW(lpStart, L's');
            if (pWVar2 != (LPWSTR)0) {
                *(uint8_t *)(param_1 + 0x16) = 0;
            }
            local_38[0] = 0.0;
            local_res10[0] = 0;
            iVar1 = PECMD_ParseNumSkipChar_0224(&local_res20, local_res10);
            if (((0 < iVar1) && (local_res10[0] != 0)) &&
                (*(int *)(param_1 + 0x10) = local_res10[0], param_3 != (uint32_t *)0)) {
                *param_3 = *param_3 | 0x80000000u;
            }
            iVar1 = PECMD_ParseNumSkipChar_de4c(&local_res20, local_38);
            if ((0 < iVar1) && (local_38[0] != 0.0)) {
                dVar4 = local_38[0];
                if (g_dpi != 0) {
                    if (local_38[0] < 0.0) {
                        dVar4 = g_fontMinus0 - local_38[0];
                    }
                    dVar4 = ((double)g_dpi * dVar4) / g_dpiBase;
                }
                *(int *)(param_1 + 4) = -(int)(dVar4 + g_fontRound);
            }
            iVar1 = PECMD_ParseNumSkipChar_0224(&local_res20, local_res10);
            if (((0 < iVar1) && (-1 < local_res10[0])) &&
                (*(char *)(param_1 + 0x17) = (char)local_res10[0], param_3 != (uint32_t *)0)) {
                *param_3 = *param_3 | 0x8000000u;
            }
            iVar1 = PECMD_ParseNumSkipChar_0224(&local_res20, local_res10);
            if ((0 < iVar1) && (-1 < local_res10[0])) {
                *(char *)(param_1 + 0x1a) = (char)local_res10[0];
            }
            iVar1 = PECMD_ParseNumSkipChar_0224(&local_res20, local_res10);
            if ((0 < iVar1) && (-1 < local_res10[0])) {
                *(int *)(param_1 + 0xc) = local_res10[0];
            }
            iVar1 = PECMD_ParseNumSkipChar_0224(&local_res20, local_res10);
            if ((0 < iVar1) && (-1 < local_res10[0])) {
                *(int *)(param_1 + 8) = local_res10[0];
            }
            iVar1 = PECMD_ParseNumSkipChar_0224(&local_res20, local_res10);
            if ((0 < iVar1) && (-1 < local_res10[0])) {
                *(char *)(param_1 + 0x18) = (char)local_res10[0];
            }
            iVar1 = PECMD_ParseNumSkipChar_0224(&local_res20, local_res10);
            if ((0 < iVar1) && (-1 < local_res10[0])) {
                *(char *)(param_1 + 0x19) = (char)local_res10[0];
            }
            iVar1 = PECMD_ParseNumSkipChar_0224(&local_res20, local_res10);
            if ((0 < iVar1) && (-1 < local_res10[0])) {
                *(char *)(param_1 + 0x1b) = (char)local_res10[0];
            }
        }
    }
    return;
}

/* ========== PECMD_SetControlTooltip @ 1400a96e4 ========== */
/* signature: undefined __fastcall PECMD_SetControlTooltip(longlong param_1, HWND param_2,
 *   int param_3, LPCWSTR param_4, char param_5) */
void PECMD_SetControlTooltip(int64_t param_1, HWND param_2, int param_3, LPCWSTR param_4,
                   char param_5)
{
    LPWSTR local_res8;
    WCHAR local_78[56];

    if ((-1 < param_3) && (param_3 < *(int *)(*(int64_t *)(param_1 + 0x38) + 0x118))) {
        wsprintfW(local_78, WSTR(".Tip%ld"));
        PECMD_StrAssign(&local_res8, WSTR("&&"));
        PECMD_StrCat(&local_res8, *(LPCWSTR *)(param_1 + 0x10));
        PECMD_StrCat(&local_res8, local_78);
        PECMD_SetVar(*(int64_t **)(param_1 + 0x50), local_res8, param_4);
        if (param_5 == '\0') {
            SendMessageW(param_2, 0x463, (WPARAM)param_4, (LPARAM)(long long)(param_3 + 3000));
        }
        PECMD_StrFree(&local_res8);
    }
    return;
}
