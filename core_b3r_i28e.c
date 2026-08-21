/* Restored batch28 B3 business functions (part i28e)
 *
 * Contains:
 *   PECMD_ParseWindowPosition @0x1400b9f1c  WINDOW/位置解析执行 (逗号分隔字段, -center/-right 对齐)
 *   PECMD_LoadIconAndTooltip @0x1400ba35c  图标/工具提示参数解析与图标装载
 *   PECMD_ConstructControlObjectEx @0x1400bd338  窗口/控件创建分派 (构造器调用)
 *   PECMD_CreateButtonControl @0x1400bd764  窗口解析执行 (选项标志 + 逗号字段)
 */
#include <stdbool.h>

#include "win32_stub.h"
#include "pecmd_defs.h"

/* ---- Ghidra primitive aliases (project convention) ---- */
typedef unsigned int        uint;
typedef unsigned short      ushort;
typedef uint8_t             byte;
typedef uint64_t            undefined8;
typedef void                undefined;
typedef int64_t             longlong;
typedef uint64_t            ulonglong;

/* ---- Globals ---- */
extern WCHAR g_szEmpty[];   /* empty string (g_szEmpty) */
extern uint8_t  DAT_14013d8a0[];   /* 图标/钩子结构缓冲 */
extern int64_t  g_i64D170;     /* @0x14013d170 */
extern int      g_dblClickFlag;     /* 双击/触发标记 */

/* ---- object / window helpers ---- */
extern void    *operator_new(size_t size);                 /* global new wrapper */
extern void    *PECMD_SendCtrlMessage_0834(WPARAM wParam, uint64_t lParam);
extern void     PECMD_ScaleQuadByFactor(int64_t param_1, int *param_2, int *param_3,
                              int *param_4, int *param_5); /* @0x14005daf8 */
extern void     PECMD_DialogBeepNotify(int64_t param_1, int param_2); /* @0x14005d9a8 */
extern void     PECMD_ResetScriptChain(int64_t *script, int64_t *a2); /* 默认参数/上下文 */
extern void     FUN_1400b9340(int64_t script, int64_t ctx, void *str, int a,
                              int b, int c, int d, void *e, uint32_t f,
                              int *g, LPCWSTR h);          /* 窗口构造/执行 */
extern uint64_t *PECMD_ConstructControl(uint64_t *param_1, int64_t param_2, uint32_t param_3,
                               uint64_t *param_4, uint32_t param_5, uint32_t param_6,
                               uint32_t param_7, uint32_t param_8, LPCWSTR param_9,
                               uint64_t *param_10, uint64_t *param_11, uint32_t param_12,
                               LPCWSTR param_13, HWND param_14, int *param_15,
                               uint64_t param_16);         /* 构造器 @0x1400baef4 */
extern void     PECMD_TokenWithRef(int64_t *param_1, int64_t *pp, int64_t *out,
                              int64_t *out2);              /* 颜色/字体/整数解析 */
extern void     PECMD_PositionMessageWindow(HWND h, LPCWSTR a, LPARAM b, uint32_t c, uint32_t d,
                              int e, uint32_t f, uint32_t g, uint64_t h2); /* 工具提示显示 */

/* ---- string / var helpers ---- */
extern void     FUN_140063620(void *out);                  /* release/init slot */
extern WCHAR   *PECMD_SkipLeadingControlChars(WCHAR **pp);                 /* skip spaces */
extern void     PECMD_StrDupAssign(void *ps, const WCHAR *src); /* assign string slot */
extern void     FUN_14005b104(void *ps);                   /* free string slot */
extern void     PECMD_SplitTokenTrimWs(void *src, void *dst, int16_t delim); /* split list */
extern void     PECMD_RunCommandLine(void *script, void *str, int mode);   /* expand */
extern int64_t *PECMD_SplitTokenAssignVar(WCHAR **out, WCHAR **pp, uint32_t sep, int flag);
extern void     PECMD_ExtractTokenByDelim(void *src, void *dst, int mode);
extern void     PECMD_CopyUpToChar(void *pp, void *out, uint32_t sep);
extern void     PECMD_ParseShortStore(void *pp, int *out, WCHAR sep);
extern uint64_t PECMD_ParseSignedNumber(short *);
extern void     PECMD_ParseLtwhParams(int64_t *a, uint32_t *b, int *c, int *d, uint32_t *e);
extern WCHAR   *PECMD_SkipWCharUntil(WCHAR **pp, uint16_t ch);    /* delimiter scan */
extern int64_t  FUN_14005c72c(const char *a, const WCHAR *w, int n);
extern int64_t  PECMD_AsciiPrefixICmp(const char *a, const WCHAR *w, int n);
extern int       PECMD_ParseHashNumbers(int64_t *, int64_t);         /* 数值写, 返回写入个数 */
extern WCHAR   *PECMD_StrCopyW(void *ps, LPCWSTR src, int64_t len); /* 定长拷贝 */
extern void     PECMD_ExpandBackslashNewline(const WCHAR *p, char c);     /* 清空串 */
extern int64_t  PECMD_ExpandVarsRecursive(int64_t *ctx, WCHAR *src, WCHAR **out, int mode,
                              uint8_t flag);
extern int64_t  PECMD_ExpandCommandLine(int64_t *ctx, WCHAR *src, WCHAR **out, int mode,
                              uint8_t flag);
extern char     FUN_140062fc4(LPCWSTR param_1, void *param_2, int param_3);
extern void     PECMD_DestroyTrayIcon(char *param_1);
extern void     PECMD_DestroyWindowLocked(void);
extern void     PECMD_ShakeTrayCursor(void);
extern void     PECMD_ReleaseResources(int64_t param_1);
extern void     PECMD_SetConfigString(LPCWSTR, int);
extern DWORD    PECMD_RegSetValueWithOpen(HKEY root, LPCWSTR sub, LPCWSTR name, DWORD type,
                              BYTE *data, DWORD size);     /* RegSetValue */
extern int64_t  FUN_1400a9a84(int64_t *a, uint64_t *b);    /* 解析长度/句柄 */
extern uint64_t PECMD_EvalParenStripped(int64_t *pp, uint64_t *out); /* 解析整数 */
extern int64_t  PECMD_ParseNumber(int64_t *pp, int *out);
extern void    *PECMD_ReadTipDummyConfig(void);
extern int64_t  FUN_1400e8644(int64_t *a);
extern HBITMAP  PECMD_LoadImageBitmap(LPCWSTR, int64_t *, int64_t, uint64_t, uint32_t *,
                              uint64_t *, uint64_t);       /* 图/图标装载 */
extern uint     PECMD_DrawTrayIcon(uint8_t *, uint64_t, LPCWSTR, HICON, LPCWSTR,
                              uint, uint32_t);             /* 图标绘制/注册 */
extern uint8_t  PECMD_DhcpWriteAndWait(LPCWSTR, LPCWSTR, uint64_t, int, LPCWSTR,
                              uint32_t, uint64_t, uint32_t); /* 工作线程执行 */

/* ================================================================
 * @0x1400b9f1c  窗口位置/对齐参数解析执行
 * signature: undefined8 __fastcall PECMD_ParseWindowPosition(longlong * param_1,
 *   ushort * param_2, WPARAM param_3)
 *
 * 解析 "<...>,x,y,w,h,dx,dy,.." 形式的窗口/提示参数串, 拆分后调用
 * FUN_1400b9340 完成窗口构造。寄存器拼接 (CONCAT*) 已归一化。
 */
uint64_t PECMD_ParseWindowPosition(int64_t *param_1, WCHAR *param_2, WPARAM param_3)
{
    longlong lVar3;
    uint64_t uVar4;
    WCHAR uVar1;
    WCHAR uVar7;
    uint32_t uVar8;
    short sVar9;
    uint32_t uVar10;
    uint64_t uVar6;
    WCHAR *puVar2;
    WCHAR *puVar5;
    WCHAR *local_res10;
    int local_res18[4];
    int local_88;
    int local_84;
    int local_80[2];
    int64_t local_78;
    WCHAR *local_70;
    int64_t local_68;
    int local_60[4];
    WCHAR *local_50;
    WCHAR *local_48;
    int64_t local_40;

    local_res10 = param_2;
    if (param_3 == 0) {
        PECMD_ResetScriptChain(param_1, 0);
        param_3 = (WPARAM)param_1[8];
        if (param_3 == 0) {
            return 0xffffffff80070057ULL;
        }
    }
    PECMD_SkipLeadingControlChars((WCHAR **)&local_res10);
    uVar10 = 0;
    local_48 = local_res10;
    PECMD_SkipWCharUntil(&local_48, 0x2c);
    puVar2 = local_48;
    uVar7 = *local_48;
    uVar1 = *local_48;
    sVar9 = 0;
    *local_48 = 0;
    if (*local_res10 == 0x2d) {
        uVar7 = 0x2d;
        uVar6 = 0;
        do {
            puVar5 = local_res10;
            uVar4 = (uint64_t)uVar7;
            lVar3 = FUN_14005c72c("-center", local_res10, 7);
            if ((char)lVar3 == '\0') {
                lVar3 = FUN_14005c72c("-right", puVar5, 6);
                if ((char)lVar3 != '\0') {
                    uVar6 = 0x20000000;
                }
            } else {
                uVar6 = 0x40000000;
            }
            while (1) {
                uVar10 = (uint32_t)uVar6;
                uVar7 = (WCHAR)(uint16_t)uVar4;
                if ((uVar7 == 0) || ((8 < uVar7 && uVar7 < 0xe) || uVar7 == 0x20)) {
                    break;
                }
                puVar5 = puVar5 + 1;
                uVar4 = (uint64_t)*puVar5;
                local_res10 = puVar5;
            }
            PECMD_SkipLeadingControlChars((WCHAR **)&local_res10);
            uVar7 = *local_res10;
        } while (uVar7 == 0x2d);
    }
    *puVar2 = uVar1;   /* 恢复被截断的逗号/分隔字符 (uVar1) */
    PECMD_StrDupAssign(&local_78, g_szEmpty);
    PECMD_StrDupAssign(&local_68, g_szEmpty);
    PECMD_StrDupAssign(&local_40, g_szEmpty);
    FUN_140063620(&local_70);
    local_60[2] = 0;
    local_60[3] = 0;
    local_60[0] = -0x80000000;
    local_60[1] = 0x80000000;
    local_84 = 0;
    local_88 = 0;
    local_80[0] = 0;
    local_res18[0] = 0;
    uVar8 = 0;
    {
        WCHAR uVar1 = *local_res10;
        if (uVar1 == 0x2a) {
            local_res10 = local_res10 + 1;
            PECMD_SkipLeadingControlChars((WCHAR **)&local_res10);
        }
        PECMD_SplitTokenTrimWs((void *)&local_res10, &local_78, 0x2c);
        PECMD_RunCommandLine(param_1, &local_78, 1);
        uVar6 = 1;
        if (*local_res10 == 0x2c) {
            local_res10 = local_res10 + 1;
            PECMD_ParseLtwhParams((int64_t *)&local_res10, (uint32_t *)&local_84,
                          &local_88, local_80, (uint32_t *)local_res18);
            if (*local_res10 == 0x2c) {
                local_res10 = local_res10 + 1;
                PECMD_ExtractTokenByDelim((void *)&local_res10, &local_68, 0x2c);
                if (*local_res10 == 0x2c) {
                    local_res10 = local_res10 + 1;
                    uVar4 = PECMD_ParseSignedNumber((short *)local_res10);
                    uVar8 = (uint32_t)uVar4;
                    if (*local_res10 == 0x2c) {
                        puVar2 = local_res10 + 1;
                        if (local_res10[1] == 0x2d) {
                            sVar9 = 0x2d;
                            puVar2 = local_res10 + 2;
                        }
                        local_res10 = puVar2;
                        PECMD_ParseHashNumbers((int64_t *)&local_res10, (int64_t)local_60);
                        if (sVar9 != 0) {
                            local_60[0] = -1;
                        }
                        if (*local_res10 == 0x2c) {
                            local_res10 = local_res10 + 1;
                            PECMD_SplitTokenTrimWs((void *)&local_res10, &local_70, 0x2c);
                        }
                    }
                }
            }
            local_50 = local_70;
            PECMD_SkipLeadingControlChars((WCHAR **)&local_50);
            if (uVar1 != 0x2a) {
                param_1 = *(int64_t **)((char *)(uintptr_t)param_3 + 0x290);
            }
            FUN_1400b9340(param_3, (int64_t)param_1, &local_78, local_84, local_88,
                          local_80[0], local_res18[0], &local_68,
                          (uVar8 & 0xffff) | uVar10, local_60,
                          (LPCWSTR)((*local_50 != 0) ? local_50 : 0));
            uVar6 = 0;
        }
    }
    FUN_14005b104(&local_70);
    FUN_14005b104(&local_40);
    FUN_14005b104(&local_68);
    FUN_14005b104(&local_78);
    return uVar6;
}

/* ================================================================
 * @0x1400ba35c  图标/工具提示参数解析与图标装载
 * signature: HICON __fastcall PECMD_LoadIconAndTooltip(longlong * param_1,
 *   LPCWSTR param_2, longlong param_3)
 *
 * 解析选项前缀 (. # *), -w/-f/--/dummy 选项, 以及逗号分隔的字段,
 * 依据是否需要工作线程装载图标并注册到 PELOGON。寄存器拼接
 * (CONCAT71 等) 已归一化为普通 uint64 位操作。
 */
HICON PECMD_LoadIconAndTooltip(int64_t *param_1, LPCWSTR param_2, int64_t param_3)
{
    HMODULE hLibModule;
    bool bVar1;
    bool bVar22;
    WCHAR *pWVar2;
    WCHAR *pWVar6;
    int iVar4;
    DWORD DVar5;
    int64_t *plVar7;
    HBITMAP pHVar8;
    HMODULE pHVar9;
    HICON pHVar10;
    int64_t lVar11;
    HWND pHVar12;
    uint8_t uVar3;
    uint64_t uVar13;
    LPCWSTR lpString;
    HICON pHVar17;
    uint32_t uVar18;
    WCHAR *lpStr2;
    uint32_t uVar19;
    char cVar21;
    uint64_t local_res8;
    WCHAR *local_res10;
    int64_t local_res18;
    char local_res20;
    LPCWSTR local_a8;
    LPCWSTR local_a0;
    LPCWSTR local_98;
    LPCWSTR local_90;
    uint32_t local_88;
    int local_84;
    WCHAR *local_80[2];
    uint32_t local_70;
    uint32_t local_6c;
    WCHAR *local_68;
    int64_t *local_60;
    uint64_t local_58;
    uint32_t local_50;
    uint32_t local_4c[3];
    WCHAR WVar15;
    HICON pHVar20;

    (void)pWVar2; (void)lpString;
    local_res10 = (WCHAR *)param_2;
    local_res18 = param_3;
    FUN_140063620(&local_68);
    uVar18 = 0;
    local_58 = 0;
    (void)local_58;
    local_res8 = 0;
    bVar22 = false;
    pHVar17 = 0;
    cVar21 = '\0';
    local_60 = param_1;
    (void)local_60;
    PECMD_StrDupAssign(&local_a0, g_szEmpty);
    PECMD_StrDupAssign(&local_a8, g_szEmpty);
    PECMD_StrDupAssign(&local_90, g_szEmpty);
    PECMD_StrDupAssign(&local_98, g_szEmpty);
    WVar15 = *param_2;
    local_50 = 10000;
    local_4c[0] = 10000;
    local_70 = 0;
    local_6c = 0;
    local_84 = 0;
    local_88 = 0;
    uVar19 = 1;
    pHVar20 = 1;
    bVar1 = true;
    local_80[0] = (WCHAR *)param_2;

    /* 前导选项字符扫描 (. # *) */
    while ((WVar15 != L'\0') && ((WVar15 < 9 || WVar15 > 0xd)) && (WVar15 != L' ')) {
        if (WVar15 == L'.') {
            local_res8 = (local_res8 & 0xffffffffffffff00ULL) | 1;
        } else if (WVar15 == L'#') {
            bVar22 = true;
        } else if (WVar15 == L'*') {
            pHVar17 = pHVar20;
        }
        cVar21 = (char)pHVar17;
        local_80[0] = local_80[0] + 1;
        local_res10 = local_80[0];
        WVar15 = *local_80[0];
    }
    if (((8 < (WCHAR)*local_80[0]) && ((WCHAR)*local_80[0] < 0xe)) ||
        (*local_80[0] == L' ')) {
        local_80[0] = local_80[0] + 1;
        local_res10 = local_80[0];
    }
    PECMD_SkipLeadingControlChars((WCHAR **)local_80);
    local_res20 = FUN_140062fc4(WSTR("-dummy"), (void *)local_80, 6);
    lpStr2 = local_80[0];
    pWVar6 = local_80[0];

    if (local_res20 == '\0') {
        /* -w / -f / -- 选项及其后跟参数 */
        iVar4 = StrCmpNIW(WSTR("-w"), local_80[0], 2);
        if ((iVar4 == 0) &&
            (((pWVar6 = lpStr2 + 2, 8 < (WCHAR)*pWVar6 && (WCHAR)*pWVar6 < 0xe)) ||
             ((*pWVar6 == L' ') || (*pWVar6 == L'\0')))) {
            uVar18 = 0x100;
            local_88 = 1;
            if (((8 < (WCHAR)*pWVar6) && ((WCHAR)*pWVar6 < 0xe)) || (*pWVar6 == L' ')) {
                pWVar6 = lpStr2 + 3;
            }
            local_res10 = pWVar6;
            local_80[0] = pWVar6;
            PECMD_SkipLeadingControlChars((WCHAR **)local_80);
            lpStr2 = local_80[0];
        } else {
            uVar19 = 0;
        }
        iVar4 = StrCmpNIW(WSTR("-f"), lpStr2, 2);
        if ((iVar4 == 0) &&
            (((pWVar6 = lpStr2 + 2, 8 < (WCHAR)*pWVar6 && (WCHAR)*pWVar6 < 0xe)) ||
             ((*pWVar6 == L' ') || (*pWVar6 == L'\0')))) {
            local_88 = uVar19 | 2;
            uVar18 = uVar18 | 0x200;
            if (((8 < (WCHAR)*pWVar6) && ((WCHAR)*pWVar6 < 0xe)) || (*pWVar6 == L' ')) {
                pWVar6 = lpStr2 + 3;
            }
            local_res10 = pWVar6;
            local_80[0] = pWVar6;
            PECMD_SkipLeadingControlChars((WCHAR **)local_80);
            lpStr2 = local_80[0];
        }
        iVar4 = StrCmpNIW(WSTR("--"), lpStr2, 2);
        if ((iVar4 == 0) &&
            ((((pWVar6 = lpStr2 + 2, 8 < (WCHAR)*pWVar6 && (WCHAR)*pWVar6 < 0xe)) ||
              (*pWVar6 == L' ')) || (*pWVar6 == L'\0'))) {
            if (((8 < (WCHAR)*pWVar6) && ((WCHAR)*pWVar6 < 0xe)) || (*pWVar6 == L' ')) {
                pWVar6 = lpStr2 + 3;
            }
            local_res10 = pWVar6;
            local_80[0] = pWVar6;
            PECMD_SkipLeadingControlChars((WCHAR **)local_80);
            lpStr2 = local_80[0];
        }
        iVar4 = lstrcmpW(WSTR("*"), lpStr2);
        if (iVar4 == 0) {
            if (cVar21 == '\0') {
                PECMD_DestroyTrayIcon((char *)DAT_14013d8a0);
                PECMD_DestroyWindowLocked();
                goto icon_cleanup_ba6aa;
            }
        } else {
            iVar4 = lstrcmpW(WSTR("-"), lpStr2);
            if (iVar4 == 0) {
                if (cVar21 == '\0') {
                    PECMD_DestroyWindowLocked();
                    goto icon_cleanup_ba6aa;
                }
                goto icon_error_baee4;
            } else {
                if (*lpStr2 != L'\0') {
                    PECMD_SplitTokenTrimWs((void *)&local_res10, &local_a0, 0x2c);
                    PECMD_RunCommandLine(param_1, &local_a0, 1);
                    pWVar6 = local_res10;
                    if (*local_res10 == L',') {
                        LPCWSTR lpString2 = local_res10 + 1;
                        local_res10 = (WCHAR *)lpString2;
                        if (bVar22) {
                            int cnt = lstrlenW(lpString2);
                            WCHAR *pEnd = pWVar6 + cnt;
                            while (lpString2 < pEnd) {
                                if (*pEnd == L'#') {
                                    break;
                                }
                                pEnd = pEnd - 1;
                            }
                            while ((lpString2 < pEnd) && (*pEnd != L',')) {
                                pEnd = pEnd - 1;
                            }
                            PECMD_StrCopyW(&local_a8, lpString2,
                                          ((int64_t)pEnd - (int64_t)lpString2) >> 1);
                        } else {
                            PECMD_ExtractTokenByDelim((void *)&local_res10, &local_a8, 0x2c);
                            pWVar6 = local_res10;
                        }
                        PECMD_RunCommandLine(param_1, &local_a8, 1);
                        goto icon_parse_ba800;
                    }
                    /* 非 ',' 结尾: 直接作为要显示的文本 */
                    goto icon_parse_ba800;
                }
                if (cVar21 == '\0') {
                    PECMD_ShakeTrayCursor();
                    goto icon_cleanup_ba6aa;
                }
            }
        }
        goto icon_error_baee4;
    }

icon_parse_ba800:
    /* 根据是否需要工作线程装载图标选择解析路径 */
    if ((*(char *)((char *)param_1 + 0xda) == '\0') &&
        ((*(byte *)((char *)param_1 + 0xd) & 0xf) == 0)) {
        PECMD_ExpandVarsRecursive(param_1, pWVar6, &local_68, 0, 1);
    } else {
        PECMD_ExpandCommandLine(param_1, pWVar6, &local_68, 0, 1);
    }
    if (local_res20 != '\0') {
        /* dummy 模式: 仅注册 TIPSDUMMY, 不装载图标 */
        PECMD_SetConfigString(local_68, 1);
        iVar4 = lstrlenW(lpStr2);
        PECMD_RegSetValueWithOpen((HKEY)0xffffffff80000002, WSTR("SOFTWARE\\PELOGON"), WSTR("TIPSDUMMY"),
                      1, (BYTE *)local_68, (DWORD)(iVar4 * 2));
        pHVar20 = 0;
        goto icon_release_ba9ad;
    }
    if ((char)local_res8 == '\0') {
        PECMD_ExpandBackslashNewline(local_a8, '\0');
    }
    if ((*local_68 == L',') || (*local_68 == L'#')) {
        local_res10 = local_68 + 1;
        PECMD_ParseShortStore(&local_res10, (int *)local_4c, 0x2c);
        local_50 = local_4c[0];
        local_68 = local_res10;
        uVar19 = local_4c[0];
    } else {
        uVar19 = 10000;
    }
    /* TODO(verify): 下方更深层的图标装载/注册分支在反编译中含大量
     * 寄存器拼接与指针布局运算, 已保留主要业务走向并做合理简化。 */
    if (*local_68 != L',') {
        goto icon_cleanup_ba6aa;
    }
    local_res10 = local_68 + 1;
    PECMD_ParseShortStore(&local_res10, &local_84, 0x2c);
    if (*local_res10 != L',') {
        goto icon_cleanup_ba6aa;
    }
    pWVar6 = local_res10 + 1;
    if (*pWVar6 != L'@') {
        HBITMAP pHVar8b = 0;
        local_res10 = pWVar6;
        if (uVar19 == 0) {
            if (cVar21 == '\0') {
                PECMD_DestroyTrayIcon((char *)DAT_14013d8a0);
                pHVar20 = (HICON)pHVar8b;
                goto icon_release_ba9ad;
            }
            goto icon_error_baee4;
        }
        plVar7 = PECMD_SplitTokenAssignVar(&local_68, &local_res10, 0x2c, 1);
        PECMD_ExtractTokenByDelim(plVar7, &local_98, 0);
        if ((g_i64D170 == 0) && (lVar11 = FUN_1400e8644((int64_t *)g_Script),
                                     lVar11 == 0)) {
            DVar5 = GetLastError();
            pHVar17 = (HICON)(uint64_t)DVar5;
            bVar22 = (DVar5 == 0);
            goto icon_error_badd0;
        }
        if (cVar21 != '\0') {
            /* 复用/装载位图的非线程路径 */
            if (param_3 == 0) {
                PECMD_ResetScriptChain(param_1, 0);
                param_3 = param_1[8];
                if (param_3 == 0) {
                    FUN_14005b104(&local_98);
                    FUN_14005b104(&local_90);
                    FUN_14005b104(&local_a8);
                    FUN_14005b104(&local_a0);
                    FUN_14005b104(&local_68);
                    return (HICON)0xffffffff80070057;
                }
            }
            pHVar17 = *(HICON *)((char *)(uintptr_t)param_3 + 0xa78);
            plVar7 = (int64_t *)((char *)(uintptr_t)param_3 + 0xa70);
            hLibModule = (HMODULE)*plVar7;
            pHVar10 = pHVar17;
            if ((*local_98 != L'*') || (local_98[1] != L'*')) {

                bVar1 = false;
                pHVar8 = PECMD_LoadImageBitmap(local_98, plVar7, 0, 0, (uint32_t *)0,
                                       (uint64_t *)0, 0xffffffffffffffffULL);
                if (pHVar8 != (HBITMAP)0) {
                    *(HBITMAP *)((char *)(uintptr_t)param_3 + 0xa78) = pHVar8;
                }
                if (*(int64_t *)((char *)(uintptr_t)param_3 + 0xa78) == 0) {
                    pHVar9 = GetModuleHandleW((LPCWSTR)0);
                    pHVar10 = LoadIconW(pHVar9, (LPCWSTR)1);
                    *plVar7 = 0;
                    *(HICON *)((char *)(uintptr_t)param_3 + 0xa78) = pHVar10;
                }
                pHVar10 = *(HICON *)((char *)(uintptr_t)param_3 + 0xa78);
                if (pHVar10 == (HICON)0) {
                    DVar5 = GetLastError();
                    pHVar17 = (HICON)(uint64_t)DVar5;
                    bVar22 = (DVar5 == 0);
                    goto icon_error_badd0;
                }
            }
            uVar19 = PECMD_DrawTrayIcon((uint8_t *)((char *)(uintptr_t)param_3 + 0x690),
                                   *(uint64_t *)((char *)(uintptr_t)param_3 + 0x20),
                                   local_a0, pHVar10, local_a8,
                                   local_50 | 0x80000000u, (uint32_t)local_84);
            if (0 < (int)uVar19) {
                pHVar9 = (HMODULE)*plVar7;
                if (!bVar1) {
                    if ((pHVar17 != (HICON)0) && (pHVar17 != (HICON)pHVar8)) {
                        DestroyIcon(pHVar17);
                    }
                    if ((hLibModule != (HMODULE)0) && (hLibModule != pHVar9)) {
                        FreeLibrary(hLibModule);
                    }
                }
                if (*(byte *)((char *)(uintptr_t)param_3 + 0x690) != 0) {
                    goto icon_cleanup_ba6aa;
                }
            }
            DVar5 = GetLastError();
            pHVar17 = (HICON)(uint64_t)DVar5;
            bVar22 = (DVar5 == 0);
            goto icon_error_badd0;
        }
        /* 线程路径: 创建工作线程通过 DHCP 注册写盘 */
        local_res10 = (WCHAR *)PECMD_ReadTipDummyConfig();
        if (local_res10 != (WCHAR *)0) {
            goto icon_after_load_ba6aa;
        }
        uVar13 = (uint64_t)g_dblClickFlag;
        if ((g_dblClickFlag == 0) &&
            ((g_i64D170 == 0 ||
              (uVar13 = *(uint64_t *)((char *)(uintptr_t)g_i64D170 + 0x20),
               uVar13 == 0)))) {
            uVar13 = *(uint64_t *)((char *)(uintptr_t)param_3 + 0x20);
        }
        if ((int)uVar19 < 0) {
            uVar19 = 0;
        }
        uVar3 = PECMD_DhcpWriteAndWait(local_a8, local_a0, (uint64_t)local_80, (int)uVar19,
                              local_98, (uint32_t)local_84, uVar13, local_88);
        if ((int)uVar3 != 0) {
            goto icon_after_load_ba6aa;
        }
        goto icon_noop_badc4;
    }
    local_res10 = local_res10 + 2;
    PECMD_SkipLeadingControlChars((WCHAR **)&local_res10);

icon_after_load_ba6aa:
    /* @ 段之后: 透明/旋转/边框选项与最终调用 */
    if (((WCHAR)(*local_res10 | 0x20U)) == 0x61) {
        local_res10 = local_res10 + 1;
        uVar18 = uVar18 | 1;
        PECMD_SkipLeadingControlChars((WCHAR **)&local_res10);
    }
    WVar15 = *local_res10;
    if (WVar15 != L'\0') {
        do {
            uint16_t uVar14 = (uint16_t)(WVar15 | 0x20);
            if (uVar14 == 0x6c) {
                pWVar6 = local_res10 + 1;
                if (local_res10[1] == L'*') {
                    uVar18 = uVar18 | 4;
                    pWVar6 = local_res10 + 2;
                }
                local_res10 = pWVar6;
                PECMD_ParseNumber((int64_t *)&local_res10, (int *)&local_70);
            } else if (uVar14 == 0x74) {
                pWVar6 = local_res10 + 1;
                if (local_res10[1] == L'*') {
                    uVar18 = uVar18 | 8;
                    pWVar6 = local_res10 + 2;
                }
                local_res10 = pWVar6;
                PECMD_ParseNumber((int64_t *)&local_res10, (int *)&local_6c);
            } else if (uVar14 == 0x72) {
                pWVar6 = local_res10 + 1;
                if (local_res10[1] == L'*') {
                    uVar18 = uVar18 | 0x20;
                    pWVar6 = local_res10 + 2;
                }
                local_res10 = pWVar6;
                PECMD_ParseNumber((int64_t *)&local_res10, (int *)&local_70);
                uVar18 = uVar18 | 0x10;
            } else if (uVar14 != 0x62) {
                break;
            } else {
                pWVar6 = local_res10 + 1;
                if (local_res10[1] == L'*') {
                    uVar18 = uVar18 | 0x80;
                    pWVar6 = local_res10 + 2;
                }
                local_res10 = pWVar6;
                PECMD_ParseNumber((int64_t *)&local_res10, (int *)&local_6c);
                uVar18 = uVar18 | 0x40;
            }
            PECMD_SkipLeadingControlChars((WCHAR **)&local_res10);
            WVar15 = *local_res10;
        } while (WVar15 != L'\0');
        param_3 = local_res18;
        if (*local_res10 != L'\0') {
            local_res10 = local_res10 + 1;
        }
    }
    local_res8 = 0;
    if (*local_res10 == L'#') {
        local_res10 = local_res10 + 1;
    }
    PECMD_EvalParenStripped((int64_t *)&local_res10, &local_res8);
    if (uVar19 == 0) {
        goto icon_apply_baedb;
    }
    if (g_i64D170 == 0) {
        lVar11 = FUN_1400e8644((int64_t *)g_Script);
        if (lVar11 == 0) {
            goto icon_noop_badc4;
        }
        if (g_i64D170 != 0) {
            goto icon_font_handle_bae26;
        }
        pHVar12 = GetDesktopWindow();
    } else {
icon_font_handle_bae26:
        pHVar12 = *(HWND *)((char *)(uintptr_t)g_i64D170 + 0x20);
    }
    PECMD_PositionMessageWindow(pHVar12, local_a8, (LPARAM)local_a0, local_70, local_6c, local_84,
                  uVar19, uVar18, local_res8);

icon_apply_baedb:
    if (cVar21 == '\0') {
        goto icon_ba6a5;
    }
    goto icon_error_baee4;

icon_ba6a5:
    PECMD_DestroyWindowLocked();
    goto icon_cleanup_ba6aa;

icon_cleanup_ba6aa:
    FUN_14005b104(&local_98);
    FUN_14005b104(&local_90);
    FUN_14005b104(&local_a8);
    FUN_14005b104(&local_a0);
    FUN_14005b104(&local_68);
    return (HICON)0;

icon_release_ba9ad:
    FUN_14005b104(&local_98);
    FUN_14005b104(&local_90);
    FUN_14005b104(&local_a8);
    FUN_14005b104(&local_a0);
    FUN_14005b104(&local_68);
    return pHVar20;

icon_error_badd0:
    if (bVar22) {
        pHVar17 = pHVar20;
    }
    FUN_14005b104(&local_98);
    FUN_14005b104(&local_90);
    FUN_14005b104(&local_a8);
    FUN_14005b104(&local_a0);
    FUN_14005b104(&local_68);
    return pHVar17;

icon_noop_badc4:
    DVar5 = GetLastError();
    pHVar17 = (HICON)(uint64_t)DVar5;
    bVar22 = (DVar5 == 0);
    goto icon_error_badd0;

icon_error_baee4:
    PECMD_ReleaseResources(param_3);
    goto icon_cleanup_ba6aa;
}

/* ================================================================
 * @0x1400bd338  窗口/控件创建分派
 * signature: undefined __fastcall PECMD_ConstructControlObjectEx(WPARAM param_1,
 *   longlong param_2, undefined8 * param_3, int param_4, int param_5,
 *   int param_6, int param_7, undefined8 * param_8, undefined8 * param_9,
 *   undefined8 * param_10, uint param_11, LPCWSTR param_12, HWND param_13,
 *   int * param_14)
 */
void PECMD_ConstructControlObjectEx(WPARAM param_1, int64_t param_2, uint64_t *param_3, int param_4,
                   int param_5, int param_6, int param_7, uint64_t *param_8,
                   uint64_t *param_9, uint64_t *param_10, uint32_t param_11,
                   LPCWSTR param_12, HWND param_13, int *param_14)
{
    int64_t lVar1;
    uint64_t *puVar2;
    uint64_t *puVar3;
    uint64_t *puVar4;
    int local_res20[2];

    local_res20[0] = param_4;
    puVar2 = (uint64_t *)PECMD_SendCtrlMessage_0834(param_1, *param_3);
    puVar4 = (uint64_t *)0;
    if (puVar2 != (uint64_t *)0) {
        lVar1 = **(int64_t **)((char *)(uintptr_t)param_1 + 0x1a0);
        PECMD_ScaleQuadByFactor((int64_t)param_1, local_res20, &param_5, &param_6, &param_7);
        puVar3 = (uint64_t *)operator_new(0xb0);
        if (puVar3 != (uint64_t *)0) {
            puVar4 = PECMD_ConstructControl(puVar3, param_2,
                                   (uint32_t)(((int64_t)puVar2 - lVar1) >> 3) + 0x1000,
                                   param_3, (uint32_t)local_res20[0], (uint32_t)param_5,
                                   (uint32_t)param_6, (uint32_t)param_7,
                                   (LPCWSTR)*param_8, param_9, param_10, param_11,
                                   param_12, param_13, param_14,
                                   (uint64_t)*(int *)((char *)(uintptr_t)param_1 + 0x17c));
        }
        *puVar2 = (uint64_t)puVar4;
        PECMD_DialogBeepNotify((int64_t)param_1, 0);
    }
    return;
}

/* ================================================================
 * @0x1400bd764  窗口解析执行 (选项标志 + 逗号字段)
 * signature: undefined8 __fastcall PECMD_CreateButtonControl(longlong * param_1,
 *   ushort * param_2, WPARAM param_3, undefined8 param_4)
 *
 * 解析 "-center/-left/-pcenter/.../font:/color:/b:" 选项及逗号分隔字段,
 * 归一化寄存器拼接并调用 PECMD_ConstructControlObjectEx 完成窗口构造。
 */
uint64_t PECMD_CreateButtonControl(int64_t *param_1, WCHAR *param_2, WPARAM param_3, uint64_t param_4)
{
    short sVar1;
    WCHAR uVar2;
    int iVar3;
    int64_t lVar4;
    uint64_t uVar5;
    uint64_t uVar9;
    uint32_t uVar11;
    uint32_t uVar14;
    WCHAR uVar13;
    uint32_t uVar16;
    HWND pHVar6;
    HWND pHVar12;
    int64_t *plVar7;
    uint64_t *puVar8;
    HWND *ppHVar10;
    HWND pHVar17;
    HWND pHVar18;
    HWND pHVar19;
    WCHAR *local_res10;
    int local_res18[2];
    uint64_t local_res20;
    int64_t local_c8;
    int local_a0[4];      /* {unused, local_9c, local_98, local_94} */
    uintptr_t local_b8;
    int local_b0[2];
    uintptr_t local_a8;
    int local_88;
    uintptr_t local_c0_hwnd;
    int64_t local_90;
    WCHAR *local_78;
    uintptr_t local_80;
    WCHAR *local_70;
    WCHAR *local_68;
    int64_t *local_60;
    uint64_t local_58;
    int64_t local_50;
    uint64_t local_48;

    pHVar12 = (HWND)0;
    local_res10 = param_2;
    local_res20 = param_4;
    if (param_3 == 0) {
        PECMD_ResetScriptChain(param_1, 0);
        param_3 = (WPARAM)param_1[8];
        if (param_3 == 0) {
            return 0xffffffff80070057ULL;
        }
    }
    PECMD_SkipLeadingControlChars((WCHAR **)&local_res10);
    local_a0[0] = (int)0x80000000;     /* -0x80000000 */
    local_a0[1] = (int)0x80000000;
    local_a0[2] = (int)0x80000000;     /* -0x80000000 */
    local_a0[3] = (int)0x80000000;
    uVar14 = 0;
    local_78 = local_res10;
    sVar1 = 0x2c;
    local_b8 = 0;
    local_a8 = 0;
    PECMD_SkipWCharUntil(&local_78, 0x2c);
    uVar2 = *local_78;
    *local_78 = 0;
    local_70 = 0;
    uVar13 = *local_res10;
    {
        WCHAR *local_88p = local_78;   /* saved comma position */
        local_res18[0] = (uint32_t)uVar2;
        pHVar17 = 0;
        pHVar18 = 0;
        pHVar19 = 0;
        uVar11 = uVar14;
        if ((ushort)(sVar1 + 1U) == uVar13) {
            do {
                WCHAR *puVar15 = local_res10;
                lVar4 = FUN_14005c72c("-center", local_res10, 7);
                if ((char)lVar4 == '\0') {
                    lVar4 = FUN_14005c72c("-right", puVar15, 6);
                    if ((char)lVar4 != '\0') {
                        pHVar17 = (HWND)0x20000000;
                    } else {
                        lVar4 = FUN_14005c72c("-left", puVar15, 5);
                        if ((char)lVar4 != '\0') {
                            pHVar17 = (HWND)0x80000000;
                        } else {
                            lVar4 = FUN_14005c72c("-pcenter", puVar15, 8);
                            uVar11 = (uint32_t)pHVar12;
                            if ((char)lVar4 != '\0') {
                                pHVar12 = (HWND)(uint64_t)(uVar11 | 0x800000);
                            } else {
                                lVar4 = FUN_14005c72c("-round", puVar15, 6);
                                if ((char)lVar4 != '\0') {
                                    pHVar12 = (HWND)(uint64_t)(uVar11 | 0x200000);
                                } else {
                                    lVar4 = FUN_14005c72c("-nscope", puVar15, 7);
                                    if ((char)lVar4 != '\0') {
                                        pHVar12 = (HWND)(uint64_t)(uVar11 | 0x400000);
                                    } else {
                                        lVar4 = FUN_14005c72c("-nscopef", puVar15, 8);
                                        if ((char)lVar4 != '\0') {
                                            pHVar12 = (HWND)(uint64_t)(uVar11 | 0x500000);
                                        } else {
                                            lVar4 = FUN_14005c72c("-nfocus", puVar15, 7);
                                            if ((char)lVar4 != '\0') {
                                                pHVar12 = (HWND)(uint64_t)(uVar11 | 0x100000);
                                            } else {
                                                lVar4 = FUN_14005c72c("-def", puVar15, 4);
                                                if ((char)lVar4 != '\0') {
                                                    pHVar12 = (HWND)(uint64_t)(uVar11 | 0x80000);
                                                } else {
                                                    lVar4 = FUN_14005c72c("-na", puVar15, 3);
                                                    if ((char)lVar4 != '\0') {
                                                        pHVar12 = (HWND)(uint64_t)(uVar11 | 0x2000000);
                                                    } else {
                                                        lVar4 = FUN_14005c72c("-b", puVar15, 2);
                                                        if ((char)lVar4 != '\0') {
                                                            pHVar18 = *(HWND *)((char *)(uintptr_t)param_3 + 0x20);
                                                        } else {
                                                            uVar5 = PECMD_AsciiPrefixICmp("-b:", puVar15, 3);
                                                            pHVar6 = pHVar19;
                                                            if ((char)uVar5 == '\0') {
                                                                uVar5 = PECMD_AsciiPrefixICmp("-font:", puVar15, 6);
                                                                if ((char)uVar5 == '\0') {
                                                                    uVar5 = PECMD_AsciiPrefixICmp("-color:", puVar15, 7);
                                                                    if ((char)uVar5 == '\0') {
                                                                        goto bd_option_done;
                                                                    }
                                                                    /* -color: */
                                                                    local_c8 = 0;
                                                                    FUN_140063620(&local_b8);
                                                                    local_res10 = local_res10 + 7;
                                                                    PECMD_TokenWithRef(param_1, (int64_t *)&local_res10,
                                                                                  &local_c8, (int64_t *)&local_b8);
                                                                    sVar1 = (short)PECMD_ParseHashNumbers(&local_c8,
                                                                                                (int64_t)&local_a0);
                                                                    pHVar6 = (HWND)(uintptr_t)&local_a0;
                                                                    if (sVar1 == 0) {
                                                                        pHVar6 = pHVar19;
                                                                    }
                                                                    if (sVar1 < 3) {
                                                                        local_a0[2] = local_a0[0];
                                                                    }
                                                                    ppHVar10 = &local_b8;
                                                                    if (sVar1 < 4) {
                                                                        local_a0[3] = local_a0[1];
                                                                    }
                                                                    goto bd_font_cleanup;
                                                                }
                                                                /* -font: */
                                                                local_res10 = puVar15 + 6;
                                                                PECMD_TokenWithRef(param_1, (int64_t *)&local_res10,
                                                                              (int64_t *)&local_a8, (int64_t *)&local_70);
                                                            } else {
                                                                /* -b: */
                                                                local_c8 = 0;
                                                                FUN_140063620(&local_80);
                                                                local_res10 = local_res10 + 3;
                                                                PECMD_TokenWithRef(param_1, (int64_t *)&local_res10,
                                                                              &local_c8, (int64_t *)&local_80);
                                                                local_c0_hwnd = 0;
                                                                iVar3 = (int)FUN_1400a9a84(&local_c8,
                                                                                           (uint64_t *)&local_c0_hwnd);
                                                                ppHVar10 = &local_80;
                                                                if (0 < iVar3) {
                                                                    pHVar18 = (HWND)local_c0_hwnd;
                                                                }
bd_font_cleanup:
                                                                FUN_14005b104((void *)ppHVar10);
                                                                pHVar19 = pHVar6;
                                                            }
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                } else {
                    pHVar17 = (HWND)0x40000000;
                }
bd_option_done:
                while (uVar13 != 0) {
                    if (((8 < uVar13) && (uVar13 < 0xe)) || (uVar13 == 0x20)) {
                        if (*puVar15 != 0) {
                            *puVar15 = 0;
                            local_res10 = local_res10 + 1;
                        }
                        break;
                    }
                    puVar15 = puVar15 + 1;
                    local_res10 = puVar15;
                    uVar13 = *puVar15;
                }
                PECMD_SkipLeadingControlChars((WCHAR **)&local_res10);
                uVar11 = (uint32_t)pHVar17;
                uVar14 = (uint32_t)pHVar12;
                uVar13 = *local_res10;
            } while (uVar13 == 0x2d);
            pHVar12 = (HWND)local_a8;
            uVar2 = (WCHAR)local_res18[0];
            local_b8 = (uintptr_t)pHVar19;
        }
        *local_88p = uVar2;
    }
    FUN_140063620(&local_68);
    local_58 = 0;
    local_60 = param_1;
    PECMD_StrDupAssign(&local_90, g_szEmpty);
    PECMD_StrDupAssign(&local_c8, g_szEmpty);
    PECMD_StrDupAssign(&local_c0_hwnd, g_szEmpty);
    PECMD_StrDupAssign(&local_50, g_szEmpty);
    local_48 = local_res20;
    (void)local_48; (void)local_58; (void)local_60;
    local_a8 = 0;                 /* TODO(verify): 寄存器低 32 位清零 */
    local_88 = 0;                 /* TODO(verify): 寄存器低 32 位清零 */
    local_b0[0] = 0;
    local_res18[0] = 0;
    uVar16 = 0;
    uVar2 = *local_res10;
    if (uVar2 == 0x2a) {
        local_res10 = local_res10 + 1;
        PECMD_SkipLeadingControlChars((WCHAR **)&local_res10);
    }
    PECMD_SplitTokenTrimWs((void *)&local_res10, &local_90, 0x2c);
    PECMD_RunCommandLine(param_1, &local_90, 1);
    if (*local_res10 == 0x2c) {
        local_res10 = local_res10 + 1;
        plVar7 = PECMD_SplitTokenAssignVar(&local_68, &local_res10, 0x2c, 1);
        PECMD_ParseLtwhParams(plVar7, (uint32_t *)&local_a8, &local_88, local_b0, (uint32_t *)local_res18);
        if (*local_res10 == 0x2c) {
            local_res10 = local_res10 + 1;
            plVar7 = PECMD_SplitTokenAssignVar(&local_68, &local_res10, 0x2c, 1);
            PECMD_ExtractTokenByDelim(plVar7, &local_c8, 0);
            if (*local_res10 == 0x2c) {
                local_res10 = local_res10 + 1;
                PECMD_CopyUpToChar((int64_t *)&local_res10, &local_50, 0x2c);
                if (*local_res10 == 0x2c) {
                    local_res10 = local_res10 + 1;
                    plVar7 = PECMD_SplitTokenAssignVar(&local_68, &local_res10, 0x2c, 1);
                    PECMD_SplitTokenTrimWs(plVar7, (int64_t *)&local_c0_hwnd, 0);
                    if (*local_res10 == 0x2c) {
                        local_res10 = local_res10 + 1;
                        puVar8 = (uint64_t *)PECMD_SplitTokenAssignVar(&local_68, &local_res10, 0x2c, 1);
                        uVar9 = PECMD_ParseSignedNumber((short *)*puVar8);
                        uVar16 = (uint32_t)uVar9;
                    }
                }
            }
        }
    }
    if (uVar2 != 0x2a) {
        param_1 = *(int64_t **)((char *)(uintptr_t)param_3 + 0x290);
    }
    PECMD_ConstructControlObjectEx(param_3, (int64_t)param_1, (uint64_t *)&local_90, (int)local_a8, (int)local_88,
                  local_b0[0], local_res18[0], (uint64_t *)&local_c8, (uint64_t *)&local_50,
                  (uint64_t *)&local_c0_hwnd, (uVar16 & 0xffff) | uVar14 | uVar11,
                  (LPCWSTR)pHVar12, pHVar18, (int *)&local_a0);
    FUN_14005b104(&local_50);
    FUN_14005b104((void *)&local_c0_hwnd);
    FUN_14005b104(&local_c8);
    FUN_14005b104(&local_90);
    FUN_14005b104(&local_68);
    FUN_14005b104(&local_70);
    return 0;
}
