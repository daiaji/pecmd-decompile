/* ====================================================================
 * core_b3r_g6.c — 还原 3 个业务函数 (batch28)
 *
 *   PECMD_FormatVolume @0x140078f00  FORMAT.COM 包装器
 *   PECMD_SetListParam @0x1400796e4  list/消息参数处理 (color/percent/smooth)
 *   PECMD_FindTokenIndex @0x140079a88  token/变量查值匹配器
 *
 * 说明:
 *   - Ghidra 的 undefinedN/longlong 已按约定映射为定宽整数类型。
 *   - 宽字符串一律使用 WSTR("...")。
 *   - u__Y__X_1401270d0 ("/Y /X " 10 字节宽串) 的 CONCAT26 splice 已折叠为
 *     普通赋值并标注 TODO(verify)。
 *   - local_res8 的 double/float 与 CONCAT44 残留采用按位重解释, 见 TODO。
 * ==================================================================== */

#include "win32_stub.h"
#include "pecmd_defs.h"
#include <stdbool.h>

/* ---- 本文件引用的全局/数据 (extern, 定义于别处) ---- */
extern WCHAR g_szEmpty[];            /* .rdata 空串/虚表基址 */
extern uint16_t u__Y__X_1401270d0[];        /* "/Y /X " 10 字节宽串 (u__Y__X_1401270d0) */
extern double   g_dbl27190;              /* clamp 上界 */
extern double   g_dbl27188;              /* clamp 下界 */

/* ---- 本文件引用的内部辅助函数 (extern, 不在此定义) ---- */
extern WCHAR    *FUN_14005b154(WCHAR **ps);                 /* @0x14005b154 trim 空白 */
extern void      FUN_14005b104(int64_t *ps);                /* @0x14005b104 释放字符串槽 */
extern uint64_t  PECMD_GetPackedSystemVersion(void);                       /* @0x14005ea5c 版本号 */
extern WCHAR    *FUN_14007034c(WCHAR **ps, LPCWSTR src);    /* @0x14007034c 字符串赋值 */
extern WCHAR    *FUN_14006375c(WCHAR **ps, LPCWSTR src);    /* @0x14006375c 字符串追加 */
extern void      FUN_140063620(WCHAR **out);                /* @0x140063620 字符串清理 */
extern void      FUN_1400675b8(WCHAR **src, WCHAR **dst, short delim); /* @0x1400675b8 按分隔切分 */
extern WCHAR    *PECMD_StrCopyW(WCHAR **ps, LPCWSTR src, int64_t len); /* @0x140063888 定长拷贝 */
extern WCHAR    *FUN_140024c48(int64_t *a, int64_t *b, uint32_t c);    /* @0x140024c48 展转义 */
extern void      FUN_1400679b0(WCHAR **pp, int *out, WCHAR sep);       /* @0x1400679b0 解析整数 */
extern WCHAR    *PECMD_CopyStrToSlot(WCHAR **a1, WCHAR **a2);                /* @0x140070310 token 扫描 */
extern int32_t   FUN_14005c7c4(const char *a, const WCHAR *w);         /* @0x14005c7c4 (icmp) */
extern int64_t   FUN_14000e26c(void *script, void *cmd, void *s3, void *s4,
                               uint32_t flag, void *p6, void *s7, void *p8); /* @0x14000e26c 脚本执行 */
extern int       FUN_14005b2c0(void *arg, LPCWSTR w, HWND hwnd);       /* @0x14005b2c0 */
extern int       FUN_140057b80(void *a, LPCWSTR b, WPARAM c, HWND d,
                               LPCWSTR e, void *f, int64_t *g, HWND h,
                               int64_t i);                              /* @0x140057b80 */
extern int       PECMD_ParseUIntValue(LPCWSTR *pp, int *out);                  /* @0x140074838 解析整数 token */
extern uint64_t  PECMD_ParseAndSkipSpace_7b54(WCHAR **pp, double *out);                /* @0x140067b54 解析 double */
extern LPCWSTR   PECMD_ExtractTokenByIndex(uint64_t a1, int *a2, LPCWSTR a3,
                               int64_t *a4, uint32_t a5, uint16_t *a6); /* @0x140067278 取变量 */

/* Windows API 中 stub 缺声明而本文件用到的符号 */
extern int       StrCmpIW(LPCWSTR, LPCWSTR);

/* ================================================================
 * @0x140078f00  FORMAT.COM 包装器
 * signature: longlong __fastcall PECMD_FormatVolume(longlong *param_1, WCHAR *param_2)
 */
int64_t PECMD_FormatVolume(int64_t *param_1, WCHAR *param_2)
{
    WCHAR WVar1;
    bool bVar2;
    WCHAR *pWVar3;
    uint64_t uVar4;
    LPWSTR pWVar5;
    WCHAR *pWVar6;
    LPCWSTR pWVar9;
    short sVar10;
    WCHAR WVar11;
    int iVar15;
    int iVar16;
    WCHAR *local_res10;
    int local_res20;
    int local_88;
    uint32_t _Var7low;               /* _Var7.dwLowDateTime */
    WCHAR *local_80;                 /* 命令串槽 (局部 _FILETIME 复用) */
    LPCWSTR local_78;
    LPCWSTR local_70;
    LPCWSTR local_68;
    LPCWSTR local_60;
    LPCWSTR local_58;
    WCHAR local_50[16];              /* "/Y /X " 宽串 (CONCAT26 splice 折叠) */
    LPCWSTR pwVar8;

    local_res10 = param_2;
    FUN_140063620((WCHAR **)&local_70);
    FUN_140063620((WCHAR **)&local_60);
    FUN_140063620((WCHAR **)&local_78);
    FUN_140063620((WCHAR **)&local_68);
    FUN_140063620(&local_80);
    iVar15 = 1;
    local_res20 = 1;
    iVar16 = 0;
    local_88 = 0;
    sVar10 = 0;

    uVar4 = PECMD_GetPackedSystemVersion();
    pwVar8 = WSTR("!=FORMAT.COM ");
    bVar2 = (uint32_t)(uVar4 >> 0x10) < 0x50001;
    if (bVar2) {
        pwVar8 = WSTR("!=cmd /c echo Y|FORMAT.COM ");
    }
    FUN_14007034c(&local_80, pwVar8);

    WVar11 = *param_2;
    WVar1 = WVar11;
    while ((WVar1 != L'\0') &&
           (((local_res10 = param_2, ((uint16_t)WVar1 < 9 || (0xd < (uint16_t)WVar1))) &&
             (WVar1 != L' ')))) {
        param_2 = param_2 + 1;
        local_res10 = param_2;
        WVar1 = *param_2;
    }
    FUN_14005b154(&local_res10);
    pWVar6 = local_res10;

    if (WVar11 == L'*') {
        FUN_14006375c(&local_80, local_res10);
        if ((!bVar2) && ((pWVar5 = (LPWSTR)StrStrIW(pWVar6, WSTR("/Y")), pWVar5 == (LPWSTR)0))) {
            FUN_14006375c(&local_80, WSTR(" /Y "));
        }
        pWVar5 = (LPWSTR)StrStrIW(pWVar6, WSTR("/X"));
        if (pWVar5 == (LPWSTR)0) {
            FUN_14006375c(&local_80, WSTR(" /X "));
        }
        pWVar9 = local_70;
        if (!bVar2) goto LAB_140079428;
        if (*pWVar6 == L'/') {
            WVar11 = L'/';
            do {
                /* 跳过参数之间的空白 (9/0xd/' ') — 原 Ghidra 条件已简化 */
                while ((WVar11 != L'\0') &&
                       ((((uint16_t)WVar11 < 9 || (0xd < (uint16_t)WVar11)) &&
                         (WVar11 != L' ')))) {
                    pWVar6 = pWVar6 + 1;
                    local_res10 = pWVar6;
                    WVar11 = *pWVar6;
                }
                FUN_14005b154(&local_res10);
                WVar11 = *local_res10;
                pWVar6 = local_res10;
            } while (WVar11 == L'/');
        }
        pWVar9 = local_70;
        if ((*pWVar6 == L'\0') || (pWVar6[1] != L':')) goto LAB_140079428;
        pWVar6[2] = L'\0';
        local_70 = pWVar6;
        sVar10 = 0;
    } else {
        FUN_1400675b8(&local_res10, (WCHAR **)&local_70, 0x2c);
        if (*local_res10 == L',') {
            local_res10 = local_res10 + 1;
            FUN_1400675b8(&local_res10, (WCHAR **)&local_60, 0x2c);
        }
        sVar10 = 0x2c;
        if ((*local_70 == L'\0') || ((pWVar9 = (LPCWSTR)0), *local_60 == L'\0') ||
            ((*local_70 != L'\\') && (local_70[1] != L':'))) {
            FUN_14005b104((int64_t *)&local_80);
            FUN_14005b104((int64_t *)&local_68);
            FUN_14005b104((int64_t *)&local_78);
            FUN_14005b104((int64_t *)&local_60);
            FUN_14005b104((int64_t *)&local_70);
            return -0x7ff8ffa9;
        }
        if (*local_res10 == L',') {
            local_res10 = local_res10 + 1;
            FUN_14005b154(&local_res10);
            if (*local_res10 == L'\"') {
                local_58 = local_res10;
                local_res10 = (WCHAR *)pWVar9;
                pWVar6 = (WCHAR *)FUN_140024c48((int64_t *)&local_58, (int64_t *)&local_res10, 0x85);
                PECMD_StrCopyW((WCHAR **)&local_78, local_58, (int64_t)local_res10);
            } else {
                FUN_1400675b8(&local_res10, (WCHAR **)&local_78, sVar10);
                pWVar6 = local_res10;
            }
            if (*pWVar6 == L',') {
                local_res10 = pWVar6 + 1;
                FUN_1400679b0(&local_res10, &local_res20, 0x2c);
                iVar15 = local_res20;
                if (*local_res10 == L',') {
                    local_res10 = local_res10 + 1;
                    FUN_1400675b8(&local_res10, (WCHAR **)&local_68, 0x2c);
                    if (*local_res10 == L',') {
                        local_res10 = local_res10 + 1;
                        FUN_1400679b0(&local_res10, &local_88, 0x2c);
                        iVar16 = local_88;
                    }
                }
            }
        }
        FUN_14006375c(&local_80, local_70);
        FUN_14006375c(&local_80, WSTR(" /FS:"));
        FUN_14006375c(&local_80, local_60);
        sVar10 = 0;
        if (*local_78 != L'\0') {
            PECMD_CopyStrToSlot(&local_res10, (WCHAR **)&local_78);
            pWVar3 = local_res10;
            WVar11 = *local_res10;
            pWVar6 = local_res10;
            while (WVar11 != L'\0') {
                if (((8 < (uint16_t)*pWVar6) && ((uint16_t)*pWVar6 < 0xe)) || (*pWVar6 == L' ')) {
                    *pWVar6 = L'_';
                    sVar10 = 0x5f;
                }
                pWVar6 = pWVar6 + 1;
                WVar11 = *pWVar6;
            }
            FUN_14006375c(&local_80, WSTR(" /V:"));
            FUN_14006375c(&local_80, pWVar3);
            FUN_14005b104((int64_t *)&local_res10);
        }
        if (*local_68 != L'\0') {
            FUN_14006375c(&local_80, WSTR(" /A:"));
            FUN_14006375c(&local_80, local_68);
        }
        if (iVar15 != 0) {
            FUN_14006375c(&local_80, WSTR(" /Q"));
        }
        uVar4 = (uint64_t)FUN_14005c7c4("NTFS", (const WCHAR *)local_60);
        if (((char)uVar4 != '\0') && (iVar16 != 0)) {
            FUN_14006375c(&local_80, WSTR(" /C"));
        }

        /* ---- u__Y__X_1401270d0 ("/Y /X " 宽串): CONCAT26 splice 折叠 ---- */
        /* TODO(verify): 原解构为 local_50 + wStack_4a + awStack_48 的 CONCAT26
         * (0x2000000000) splice, 此处折叠为普通字符串赋值。 */
        lstrcpyW(local_50, WSTR("/Y /X "));
        if (bVar2) {
            /* TODO(verify): bVar2 分支对尾部字节的小扰动 (local_50._2_2_ = 0x20) */
            local_50[2] = L' ';
        }
        FUN_14006375c(&local_80, local_50);

        pWVar9 = local_70;
        if (!bVar2) goto LAB_140079428;
    }
    if (local_70 != (LPCWSTR)0) {
        SetVolumeLabelW(local_70, (LPCWSTR)g_szEmpty);
    }

LAB_140079428:
    {
        int64_t _Var7 = FUN_14000e26c((void *)param_1, (void *)local_80, (void *)param_1,
                                      (void *)0, (uint32_t)1, (void *)0, (void *)0, (void *)0);
        _Var7low = (uint32_t)_Var7;                     /* _Var7.dwLowDateTime */
    }
    if (sVar10 != 0) {
        SetVolumeLabelW(pWVar9, local_78);
    }
    FUN_14005b104((int64_t *)&local_80);
    FUN_14005b104((int64_t *)&local_68);
    FUN_14005b104((int64_t *)&local_78);
    FUN_14005b104((int64_t *)&local_60);
    FUN_14005b104((int64_t *)&local_70);
    return (int64_t)(int)_Var7low;
}

/* ================================================================
 * @0x1400796e4  list/消息参数处理 (color / percent / smooth)
 * signature: undefined8 __fastcall PECMD_SetListParam(undefined8 *param_1,
 *   longlong *param_2, LPCWSTR param_3, LPCWSTR param_4, WPARAM param_5,
 *   undefined8 *param_6)
 */
uint64_t PECMD_SetListParam(uint64_t *param_1, int64_t *param_2, LPCWSTR param_3,
                       LPCWSTR param_4, WPARAM param_5, uint64_t *param_6)
{
    HWND hWnd;
    HWND pHVar1;
    LPCRITICAL_SECTION lpCriticalSection;
    double dVar2;
    double dVar10;
    int bVar3;
    int iVar4;
    int64_t uVar5;
    uint64_t uVar6;
    LPCWSTR pWVar7;
    UINT Msg;
    uint8_t bVar8;
    int64_t lVar9;
    double local_res8;
    LPCWSTR local_res18;
    uint64_t *local_78;
    int64_t local_70;
    LPCWSTR local_68;
    int64_t local_60;
    LPCWSTR local_58;
    int64_t local_50;
    int64_t local_48;

    hWnd = *(HWND *)((char *)param_1[7] + 0x20);
    pHVar1 = *(HWND *)(*(int64_t *)((char *)param_1[10] + 0x40) + 0x20);
    local_res18 = param_3;
    bVar3 = FUN_14005b2c0(param_1, param_4, pHVar1);
    pWVar7 = (LPCWSTR)0;
    if (bVar3 == 0) {
        return 0;
    }
    lVar9 = (int64_t)param_1[7];
    bVar8 = *(uint8_t *)((char *)lVar9 + 0xe4);
    uVar5 = (int64_t)FUN_14005c7c4("percent", (const WCHAR *)param_4);
    if ((char)uVar5 == '\0') {
LAB_140079792:
        lpCriticalSection = (LPCRITICAL_SECTION)*param_6;
        if (lpCriticalSection != (LPCRITICAL_SECTION)0) {
            *param_6 = 0;
            LeaveCriticalSection(lpCriticalSection);
        }
        uVar5 = FUN_140057b80((void *)(intptr_t)param_1[7], param_4, param_5, hWnd, param_3,
                              (void *)(intptr_t)param_1[10], param_2, pHVar1, (int64_t)param_1);
        if ((int)uVar5 == 0) {
            iVar4 = lstrcmpW(WSTR("color"), param_4);
            if (iVar4 == 0) {
                /* local_res8 同时按 double 与低 32 位 int 使用:
                 *   local_res8 = CONCAT44(local_res8<hi32>, 0x80000000) — TODO(verify) */
                local_res8 = 0.0;
                *(int32_t *)&local_res8 = (int32_t)0x80000000;
                bVar3 = PECMD_ParseUIntValue(&local_res18, (int *)&local_res8);
                if (bVar3 != 0) {
                    if (*(int32_t *)&local_res8 < 0) {
                        *(int32_t *)&local_res8 = (int32_t)0x80000000; /* -0x80000000 */
                    }
                    if ((bVar8 & 1) == 0) {
                        *(int32_t *)((char *)param_1[7] + 0xe0) = *(int32_t *)&local_res8;
                    } else {
                        *(int32_t *)((char *)param_1[7] + 0x5c) = *(int32_t *)&local_res8;
                    }
                }
            } else if (*param_4 == L'\0') {
                if (*param_3 == L'\0') {
                    return 0x80070057;
                }
                local_res8 = 0.0;
                iVar4 = -0x7fffff7;
                uVar6 = PECMD_ParseAndSkipSpace_7b54((WCHAR **)&local_res18, &local_res8);
                if (0 < (int)uVar6) {
                    dVar2 = g_dbl27190;
                    if ((g_dbl27190 < local_res8) ||
                        ((dVar10 = local_res8, dVar2 = g_dbl27188, local_res8 < g_dbl27188))) {
                        dVar10 = dVar2;
                    }
                    iVar4 = (int)dVar10;
                }
                if (*local_res18 != L'\0') {
                    pWVar7 = local_res18 + 1;
                }
                if ((bVar8 & 1) == 0) {
                    if ((iVar4 == -0x7fffff7) && (pWVar7 == (LPCWSTR)0)) {
                        return 0;
                    }
                    local_70 = (int64_t)iVar4;
                    local_60 = 0;
                    local_58 = (LPCWSTR)0;
                    local_50 = 0;
                    Msg = 0x462;
                    local_78 = param_1;
                    local_68 = pWVar7;
                } else {
                    local_50 = 0xffffffffffffffffLL;
                    local_78 = (uint64_t *)(int64_t)(iVar4 * 10000000);
                    Msg = 0x45b;
                    local_70 = -0x80000000;
                    local_68 = (LPCWSTR)(intptr_t)(int64_t)(int32_t)0x80000000; /* 0xffffffff80000000 */
                    local_60 = (int64_t)(intptr_t)(int64_t)(int32_t)0x80000000;
                    local_58 = pWVar7;
                }
                local_48 = 0;
                SendMessageW(hWnd, Msg, (WPARAM)param_1[7], (LPARAM)&local_78);
            }
        }
    } else {
        uVar5 = (int64_t)FUN_14005c7c4("smooth", (const WCHAR *)param_3);
        if ((char)uVar5 == '\0') {
            uVar5 = (int64_t)FUN_14005c7c4("-smooth", (const WCHAR *)param_3);
            if ((char)uVar5 == '\0') goto LAB_140079792;
            bVar8 = bVar8 & 0xfe;
        } else {
            bVar8 = bVar8 | 1;
        }
        *(uint8_t *)((char *)lVar9 + 0xe4) = bVar8;
    }
    return 0;
}

/* ================================================================
 * @0x140079a88  token/变量查值匹配器
 * signature: int __fastcall PECMD_FindTokenIndex(LPCWSTR param_1, LPCWSTR param_2,
 *   uint param_3, int param_4, ushort param_5)
 */
int PECMD_FindTokenIndex(LPCWSTR param_1, LPCWSTR param_2, unsigned int param_3,
                  int param_4, unsigned short param_5)
{
    WCHAR *lpStr2;
    WCHAR *pWVar1;
    WCHAR WVar2;
    int iVar3;
    int iVar4;
    WCHAR *pWVar5;
    int iVar6;
    WCHAR *local_res8;
    WCHAR *local_res10;
    int local_res18;

    iVar6 = 1;
    if (param_4 < 0) {
        param_4 = 1;
    }
    local_res8 = (WCHAR *)param_1;
    local_res10 = (WCHAR *)param_2;
    FUN_14005b154(&local_res8);
    FUN_14005b154(&local_res10);
    lpStr2 = local_res8;
    WVar2 = *local_res8;
    pWVar5 = local_res8;
    while ((WVar2 != L'\0') &&
           ((((uint16_t)WVar2 < 9 || (0xd < (uint16_t)WVar2))) && (WVar2 != L' '))) {
        pWVar5 = pWVar5 + 1;
        WVar2 = *pWVar5;
    }
    iVar3 = lstrlenW(local_res8);

    if ((param_3 & 0x10) == 0) {
        while ((pWVar5 = local_res10), *local_res10 != L'\0') {
            if ((param_3 & 0xf) == 0) {
                iVar4 = StrCmpNIW(local_res10, lpStr2, iVar3);
            } else {
                iVar4 = StrCmpNW(local_res10, lpStr2, iVar3);
            }
            if (((iVar4 == 0) &&
                 (((WVar2 = pWVar5[iVar3], WVar2 == L'\0') ||
                   ((8 < (uint16_t)WVar2) && ((uint16_t)WVar2 < 0xe))) ||
                  (WVar2 == L' ')))) {
                param_4 = param_4 + -1;
                if (param_4 < 1) {
                    return iVar6;
                }
            }
            WVar2 = *pWVar5;
            while ((WVar2 != L'\0') &&
                   ((local_res10 = pWVar5, ((uint16_t)WVar2 < 9 || (0xd < (uint16_t)WVar2))) &&
                    (WVar2 != L' '))) {
                pWVar5 = pWVar5 + 1;
                local_res10 = pWVar5;
                WVar2 = *pWVar5;
            }
            FUN_14005b154(&local_res10);
            iVar6 = iVar6 + 1;
        }
    } else {
        FUN_140063620(&local_res8);
        pWVar1 = local_res10;
        WVar2 = *local_res10;
        pWVar5 = local_res8;
        while (WVar2 != L'\0') {
            local_res18 = iVar6 + -1;
            *pWVar5 = L'\0';
            PECMD_ExtractTokenByIndex((uint64_t)(uintptr_t)g_Script, &local_res18, pWVar1,
                          (int64_t *)&local_res8, (uint32_t)param_5, (uint16_t *)0);
            pWVar5 = local_res8;
            if (*local_res8 == L'\0') break;
            if ((param_3 & 0xf) == 0) {
                iVar3 = lstrcmpiW(local_res8, lpStr2);
            } else {
                iVar3 = lstrcmpW(local_res8, lpStr2);
            }
            if (iVar3 == 0) {
                param_4 = param_4 + -1;
                if (param_4 < 1) {
                    FUN_14005b104((int64_t *)&local_res8);
                    return iVar6;
                }
            }
            iVar6 = iVar6 + 1;
            WVar2 = *pWVar1;
        }
        FUN_14005b104((int64_t *)&local_res8);
    }
    return 0;
}
