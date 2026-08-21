/*
 * core_b3r_h3.c — 还原 gh3 的 7 个业务函数
 *
 *   PECMD_HandleControlCommand @0x14009ce20  控件/列表命令处理 (Check 特殊分支)
 *   PECMD_ReadTextLine @0x14009da54  READLINE/文件读行（编码识别/BOM/行扫描）
 *   PECMD_WriteFileEncoded @0x14009f070  写文件（编码转换/定位/追加）
 *   PECMD_WriteVarEncoded @0x1400a03ac  写变量/输出（-m/-bin/-8 编码分支）
 *   PECMD_WriteVarTruncated @0x1400a0644  字符串按长度截断写变量
 *   PECMD_SetVarRange @0x1400a0844  字符串按长度截断写变量(尾部)
 *   PECMD_SearchStringAndLocate @0x1400a0d38  字符串/列表查找与位置计算
 *
 * 说明:
 *   - 仅实现本文件列出的目标函数; 所有辅助函数/全局仅 extern, 不定义。
 *   - FUN_140102a90 是 memset 别名 → 直接调 memset。
 *   - thunk_FUN_1400f429c 是 PECMD_SkipWCharUntil 的 thunk, 统一调用 PECMD_SkipWCharUntil。
 *   - Ghidra 的 CONCAT71/CONCAT44/CONCAT22 等已简化为普通整数/位运算。
 *   - 唤醒返回值 (extraout_var*) 在 int 判定处的 CONCAT71 已简化为直接比较。
 */

#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#include "win32_stub.h"
#include "pecmd_defs.h"

/* ---- 本文件引用的全局 (link_stubs.c / core_globals.c / .rdata 定义) ---- */
extern WCHAR g_szEmpty[];                     /* g_szEmpty 空串 */
extern HANDLE g_hStdIn;                  /* GetStdHandle(STD_INPUT_HANDLE) */
extern HANDLE g_hStdErr;                  /* GetStdHandle(STD_ERROR_HANDLE) */
extern HANDLE g_hStdOut;                  /* GetStdHandle(STD_OUTPUT_HANDLE) */
extern uint8_t g_u8CCB1;                 /* MAIN_DBG 日志标志 */

/* BOM 常量 (.rdata DAT_140124128/12c/130) */
static const uint8_t DAT_140124128[] = { 0xFF, 0xFE };        /* UTF-16LE BOM */
static const uint8_t DAT_14012412c[] = { 0xFE, 0xFF };        /* UTF-16BE BOM */
static const uint8_t DAT_140124130[] = { 0xEF, 0xBB, 0xBF };  /* UTF-8 BOM */

/* ---- 本文件引用的辅助函数 (均为 extern, 不在本文件定义) ---- */
extern int     PECMD_OnDeleteCommand(uint64_t *param_1, LPCWSTR param_2, HWND param_3); /* 列表接受检查 */
extern int     PECMD_DispatchControlCommand(void *a, LPCWSTR b, WPARAM c, HWND d, LPCWSTR e,
                             uint64_t f, int64_t *g, HWND h, int64_t i);
extern uint64_t PECMD_ParseIntRound(int64_t *pp, int *out);
extern void    PECMD_SetVariableWithPrefix(int64_t *ctx, LPCWSTR key, LPCWSTR value);
extern int64_t FUN_14005c72c(const char *a, const WCHAR *w, int n);   /* 前缀比较 */
extern int32_t PECMD_AsciiWideICmp(const char *a, const WCHAR *w);          /* 后缀/封装比较 */
extern uint64_t PECMD_AsciiPrefixICmp(const char *a, const uint16_t *w, int n); /* 固定长度比较 */
extern uint32_t PECMD_ParseStringToken(int64_t *a, int64_t *b, int64_t *c);    /* 分隔符选项解析 */
extern int64_t FUN_140064a88(const WCHAR *p);                         /* WtoI */
extern WCHAR  *FUN_14005b154(WCHAR **pp);                            /* 跳空白 */
extern char    FUN_1400660ac(const char *tok, WCHAR **pp, int n);     /* 前缀匹配 */
extern void    PECMD_StrDupAssign(WCHAR **ps, const WCHAR *src);          /* 串赋值 */
extern int64_t FUN_1400545f8(int64_t *ctx, int64_t *pp, int64_t *out,
                             int16_t c1, int16_t c2);
extern int64_t PECMD_ExpandCommandLine(int64_t *ctx, WCHAR *src, WCHAR **out, int mode, uint8_t flag);
extern void    PECMD_SplitTokenTrimWs(int64_t *src, int64_t *dst, int16_t delim); /* 按定界符切分 */
extern uint64_t FUN_1400745c8(WCHAR **pp, uint64_t *out);
extern void    FUN_14005b104(void *ps);                               /* 释放字符串槽 */
extern void    FUN_1400633a8(void **ps, int64_t len);                 /* 分配槽 */
extern WCHAR  *PECMD_AllocString(WCHAR **ps, int64_t count);              /* 分配串 (StrAlloc) */
extern void    PECMD_ParseSignedNumberStr(void *pp, void *out, int16_t sep);       /* 数值解析 */
extern int64_t *PECMD_SplitTokenAssignVar(WCHAR **src, WCHAR **pp, uint16_t sep, int mode);
extern void    PECMD_AllocSmallObject(void *pp);                               /* 小对象分配 */
extern WCHAR  *PECMD_SkipWCharUntil(WCHAR **pp, uint16_t ch);                /* 分隔符扫描 */
extern void    PECMD_GenerateTimeText(LPCWSTR a, int64_t *out, LPCWSTR b,
                             int64_t c, FILETIME d);
extern uint32_t FUN_1400799f0(LPCWSTR p, int64_t len);                /* 编码识别 → codepage */
extern void   *PECMD_GrowByteBuffer(void **ps, int64_t len);                 /* 分配清零槽数组 */
extern WCHAR  *PECMD_StrDupA(WCHAR **ps, LPCWSTR src, int64_t a, int64_t b);
extern int64_t PECMD_VarLookup(void *script, LPCWSTR name, void *scope,
                             int64_t len, uint64_t *extra);
extern void    PECMD_AllocWStringBuffer(WCHAR **ps, int64_t count);              /* 分配 */
extern WCHAR  *PECMD_StrCopyW(WCHAR **ps, LPCWSTR src, int64_t len);   /* 定长拷贝 */
extern WCHAR  *FUN_1400702d4(WCHAR **out, const WCHAR *src, int64_t len);
extern void    FUN_140063620(WCHAR **out);                           /* 串容器初始化 */
extern WCHAR  *FUN_14006375c(WCHAR **ps, LPCWSTR src);               /* 字符串追加 */
extern void    PECMD_SetVariable(int64_t *script, LPCWSTR key, LPCWSTR value); /* SetVar */
extern char    PECMD_ReadFileLine(void *a, int64_t *b, DWORD c, uint8_t d,
                             uint32_t *e);                            /* 读一行字符 */
extern LPCWSTR PECMD_ConvertStringEncoding(void *a, DWORD b);                      /* 取串槽内容 */
extern int64_t *FUN_1400702f0(int64_t *out, LPCSTR s, uint64_t len);  /* 取串槽 */
extern int64_t *PECMD_WideToAnsiStr(int64_t *ps, LPCWSTR src, int64_t len, uint64_t cap);
extern void    FUN_140060a74(uint8_t *param_1, int param_2);         /* 大小写转换 */
extern int64_t PECMD_WideToAnsiConvert(void *a);                               /* UTF8 长度 */
extern uint32_t FUN_140006a4c(LPCWSTR param_1);                      /* 路径/句柄检查 */
extern int64_t PECMD_OpenFileExisting(uint64_t param_1, uint64_t param_2, uint64_t param_3);
extern HANDLE  PECMD_OpenFileHandle(HANDLE *out, LPCWSTR path, DWORD access, DWORD share,
                             LPSECURITY_ATTRIBUTES sa, DWORD disp, DWORD flags,
                             HANDLE tmpl);
extern LARGE_INTEGER PECMD_SetFilePointer(HANDLE hFile, LARGE_INTEGER pos, DWORD method); /* SetFilePointer */
extern uint32_t PECMD_GetFileSize(HANDLE hFile);                         /* GetFileSize */
extern void    FUN_140018d8c(uint64_t ctx, LPCWSTR fmt, uint64_t a, uint64_t b); /* 日志 */
extern WCHAR  *FUN_1400703e4(int64_t *out, LPCWSTR src);             /* StrCpyW2 */
extern int64_t PECMD_FindVarValue(int64_t *param_1, LPCWSTR param_2, int64_t *param_3,
                             int param_4);                            /* 变量插值 */
extern void    FUN_14006f884(LPCWSTR a, void *b);                     /* 输出/插值 */
extern int64_t *PECMD_AssignString(int64_t *param_1, LPCWSTR param_2);
extern void    FUN_14007bf44(int64_t *param_1, LPCWSTR param_2, int64_t *param_3,
                             int param_4, int param_5);
extern void    PECMD_AppendFormattedI64(int64_t *list, int64_t pos);             /* 记录命中位置 */
extern void    FUN_1400669c4(int64_t *a, uint64_t b, LPCWSTR c);     /* SetVarD */
extern void    PECMD_JoinTokensAndResolve(int64_t *ctx, int64_t *pp, int64_t *out);
extern void    FUN_1400679b0(WCHAR **pp, int *out, WCHAR sep);
extern uint64_t FUN_140060b5c(uint64_t *a, int b, void *c, int d);    /* 位置计算 */
extern void    PECMD_SetEndOfFileWrap(void *a, LARGE_INTEGER b);
extern int64_t PECMD_AllocConsoleBuffers(int64_t a);                              /* __chkstk/刷新 */
extern WCHAR  *FUN_14001be14(WCHAR *s);                              /* 串标签查找 */
extern void    PECMD_DeviceCheckReady(LPCWSTR s);
extern uint32_t FUN_140063060(uint32_t *buf);                         /* 取 BUFFER 大小 */
extern WCHAR  *FUN_1400547bc(int64_t *ctx, int64_t *pp, int64_t *out,
                             int16_t c1, int16_t c2);
extern void    PECMD_RunCommandLine(int64_t *ctx, void *key, int mode);      /* 变量取值 */
extern bool    FUN_1400c11c0(WCHAR **pp, int *out);
extern uint64_t PECMD_IsNumericString(const WCHAR *p);                        /* 数解析 */
extern int64_t PECMD_FindTokenIndex(LPCWSTR a, WCHAR *b, uint32_t c, int d, uint16_t e);

/* ==========================================================================
 * @0x14009ce20  (undefined8 PECMD_HandleControlCommand(undefined8*, longlong*, LPCWSTR,
 *                 LPCWSTR, LPCWSTR, undefined8*))
 * 控件/列表命令处理: 先检查接收, 命中 "Check" 时走状态机, 否则回发消息。
 * ========================================================================== */
uint64_t PECMD_HandleControlCommand(uint64_t *param_1, int64_t *param_2, LPCWSTR param_3,
                       LPCWSTR param_4, LPCWSTR param_5, void **param_6)
{
    HWND hWnd;
    HWND pHVar1;
    LPCRITICAL_SECTION lpCriticalSection;
    int iVar3;
    uint64_t uVar4;
    LRESULT LVar5;
    int local_res8[2];
    LPCWSTR local_res18;

    hWnd = *(HWND *)(param_1[7] + 0x20);
    pHVar1 = *(HWND *)(*(int64_t *)(param_1[10] + 0x40) + 0x20);
    local_res18 = param_3;
    if (PECMD_OnDeleteCommand(param_1, param_4, pHVar1) != 0) {
        lpCriticalSection = (LPCRITICAL_SECTION)*param_6;
        if (lpCriticalSection != NULL) {
            *param_6 = 0;
            LeaveCriticalSection(lpCriticalSection);
        }
        uVar4 = PECMD_DispatchControlCommand((void *)(uintptr_t)param_1[7], param_4, (WPARAM)param_5, hWnd,
                              param_3, param_1[10], param_2, pHVar1, (int64_t)param_1);
        if ((int)uVar4 == 0) {
            if ((param_4 != NULL) && (iVar3 = lstrcmpiW(param_4, WSTR("Check")), iVar3 == 0)) {
                local_res8[0] = -0x7fffff7;
                if (*param_3 == L'\0') {
                    return 0;
                }
                PECMD_ParseIntRound((int64_t *)&local_res18, local_res8);
                if (local_res8[0] == -0x7fffff7) {
                    return 0;
                }
                if (local_res8[0] == 2) {
                    LVar5 = SendMessageW(hWnd, 0x465, 2, 0);
                    ((LPWSTR)param_3)[1] = L'\0';
                    *((LPWSTR)param_3) = (WCHAR)((short)LVar5 + L'0');
                }
                else {
                    SendMessageW(hWnd, 0xf1, (uint64_t)(local_res8[0] != 0), 0);
                }
                if (*(short *)param_1[2] == 0) {
                    return 0;
                }
                PECMD_SetVariableWithPrefix((int64_t *)param_1[10], param_5, param_3);
                return 0;
            }
            if (*param_4 == L'\0') {
                SendMessageW(hWnd, 0x45a, (WPARAM)param_1, (LPARAM)param_3);
            }
        }
    }
    return 0;
}

/* ==========================================================================
 * @0x1400a03ac  (undefined8 PECMD_WriteVarEncoded(longlong*, LPCWSTR))
 * 输出/写变量命令: 按 -m/-bin/-8 分支求值并写回 (param_2 作为键)。
 * ========================================================================== */
uint64_t PECMD_WriteVarEncoded(int64_t *param_1, LPCWSTR param_2)
{
    char cVar1;
    char cVar2;
    char cVar3;
    char cVar4;
    int cchWideChar;
    WCHAR *psVar5;
    WCHAR *plVar6;
    UINT CodePage;
    uint64_t uVar7;
    LPCWSTR local_res10;
    LPCWSTR local_res20;
    LPCWSTR local_a8;
    LPCWSTR local_a0;
    WCHAR local_98[56];

    local_res10 = param_2;
    FUN_14005b154((WCHAR **)&local_res10);
    PECMD_StrDupAssign((WCHAR **)&local_res20, g_szEmpty);
    FUN_14005b154((WCHAR **)&local_res10);
    cVar1 = FUN_1400660ac("*", (WCHAR **)&local_res10, 1);
    cVar2 = FUN_1400660ac("-m", (WCHAR **)&local_res10, 2);
    cVar3 = FUN_1400660ac("-bin", (WCHAR **)&local_res10, 4);
    cVar4 = FUN_1400660ac("-8", (WCHAR **)&local_res10, 2);
    uVar7 = 0;
    if (*local_res10 == L'\0') {
        goto LAB_0a0617;
    }
    psVar5 = FUN_1400547bc(param_1, (int64_t *)&local_res10, (int64_t *)&local_res20, 0x3d, 0);
    if ((*local_res20 == L'\0') || (*psVar5 != 0x3d)) {
        uVar7 = 0xffffffff80070057ULL;
        goto LAB_0a0617;
    }
    local_a8 = (LPCWSTR)0;
    EnterCriticalSection((LPCRITICAL_SECTION)&g_csInit);
    if (cVar1 == '\0') {
LAB_0a0508:
        if (cVar3 != '\0') {
            goto LAB_0a050d;
        }
        if (local_res10 != (LPCWSTR)0) {
            if (cVar2 == '\0') {
                if (cVar4 == '\0') {
                    lstrlenW(local_res10);
                }
                else if (*local_res10 != L'\0') {
                    cchWideChar = lstrlenW(local_res10);
                    CodePage = 0xfde9;
                    goto LAB_0a058a;
                }
            }
            else if (*local_res10 != L'\0') {
                cchWideChar = lstrlenW(local_res10);
                CodePage = 0;
LAB_0a058a:
                WideCharToMultiByte(CodePage, 0, local_res10, cchWideChar, (LPSTR)0, 0,
                                    (LPCSTR)0, (BOOL *)0);
            }
        }
    }
    else {
        if (cVar3 == '\0') {
            PECMD_JoinTokensAndResolve(param_1, (int64_t *)&local_res10, (int64_t *)&local_a8);
            local_res10 = local_a8;
            goto LAB_0a0508;
        }
LAB_0a050d:
        plVar6 = FUN_14005b154((WCHAR **)&local_res10);
        PECMD_StrDupAssign((WCHAR **)&local_a0, (LPCWSTR)(uintptr_t)*plVar6);
        PECMD_RunCommandLine(param_1, &local_a0, 1);
        PECMD_VarLookup(param_1, local_a0, (void *)0, -1, (uint64_t *)0);
        FUN_14005b104(&local_a0);
    }
    wsprintfW(local_98, WSTR("%ld"));
    PECMD_SetVariable(param_1, local_res20, local_98);
    LeaveCriticalSection((LPCRITICAL_SECTION)&g_csInit);
    FUN_14005b104(&local_a8);
LAB_0a0617:
    FUN_14005b104(&local_res20);
    return uVar7;
}

/* ==========================================================================
 * @0x1400a0644  (undefined8 PECMD_WriteVarTruncated(longlong*, LPCWSTR))
 * 变量按长度截断写 (前部): key=value 取首段并限长。
 * ========================================================================== */
uint64_t PECMD_WriteVarTruncated(int64_t *param_1, LPCWSTR param_2)
{
    char cVar1;
    int iVar2;
    uint64_t uVar3;
    LPCWSTR pWVar4;
    WCHAR WVar5;
    uint64_t uVar6;
    LPCWSTR local_res10[2];
    int local_res20[2];
    LPCWSTR local_38;
    LPCWSTR local_30;
    LPCWSTR local_28;
    LPCWSTR local_20;

    local_res10[0] = param_2;
    FUN_14005b154((WCHAR **)local_res10);
    PECMD_StrDupAssign((WCHAR **)&local_38, g_szEmpty);
    FUN_14005b154((WCHAR **)local_res10);
    cVar1 = FUN_1400660ac("*", (WCHAR **)local_res10, 1);
    uVar6 = 0;
    if (*local_res10[0] == L'\0') {
        FUN_14005b104(&local_38);
        return 0;
    }
    PECMD_SplitTokenTrimWs((int64_t *)local_res10, (int64_t *)&local_38, 0x3d);
    PECMD_RunCommandLine(param_1, &local_38, 0);
    if (*local_38 != L'\0') {
        if (*local_res10[0] == L'=') {
            ((LPWSTR)local_res10[0])[0] = L'\0';
            local_res10[0] = local_res10[0] + 1;
            WVar5 = L',';
            local_res20[0] = 0;
            local_30 = local_res10[0];
            PECMD_SkipWCharUntil((WCHAR **)local_res10, 0x2c);
            if (*local_res10[0] == WVar5) {
                ((LPWSTR)local_res10[0])[0] = L'\0';
                local_res10[0] = local_res10[0] + 1;
                local_28 = (LPCWSTR)0;
                if (cVar1 != '\0') {
                    PECMD_JoinTokensAndResolve(param_1, (int64_t *)local_res10, (int64_t *)&local_28);
                    local_res10[0] = local_28;
                }
                local_20 = local_30;
                FUN_14005b154((WCHAR **)&local_20);
                if (*local_20 != L'(') {
                    pWVar4 = local_20;
                    if ((*local_20 == L'-') && ((uint16_t)(local_20[1] + 0xffd0) < 10)) {
                        pWVar4 = local_20 + 1;
                    }
                    uVar3 = PECMD_IsNumericString(pWVar4);
                    if (((int)uVar3 == 0) &&
                        (uVar3 = PECMD_IsNumericString(local_res10[0]), pWVar4 = local_res10[0],
                         (int)uVar3 != 0)) {
                        local_res10[0] = local_30;
                        local_30 = pWVar4;
                    }
                }
                FUN_1400c11c0((WCHAR **)&local_30, local_res20);
                iVar2 = lstrlenW(local_res10[0]);
                if ((local_res20[0] < 1) || (iVar2 < local_res20[0])) {
                    local_res20[0] = iVar2;
                }
                ((LPWSTR)local_res10[0])[local_res20[0]] = L'\0';
                PECMD_SetVariable(param_1, local_38, local_res10[0]);
                FUN_14005b104(&local_28);
                goto LAB_0a0829;
            }
        }
        uVar6 = 0xffffffff80070057ULL;
    }
LAB_0a0829:
    FUN_14005b104(&local_38);
    return uVar6;
}

/* ==========================================================================
 * @0x1400a0844  (undefined8 PECMD_SetVarRange(longlong*, LPCWSTR))
 * 变量按长度截断写 (尾部): 同 0a0644 但保留右段。
 * ========================================================================== */
uint64_t PECMD_SetVarRange(int64_t *param_1, LPCWSTR param_2)
{
    char cVar1;
    int iVar2;
    uint64_t uVar3;
    LPCWSTR pWVar4;
    WCHAR WVar5;
    uint64_t uVar6;
    LPCWSTR local_res10[2];
    int local_res20[2];
    LPCWSTR local_38;
    LPCWSTR local_30;
    LPCWSTR local_28;
    LPCWSTR local_20;

    local_res10[0] = param_2;
    PECMD_StrDupAssign((WCHAR **)&local_38, g_szEmpty);
    FUN_14005b154((WCHAR **)local_res10);
    cVar1 = FUN_1400660ac("*", (WCHAR **)local_res10, 1);
    uVar6 = 0;
    if (*local_res10[0] == L'\0') {
        FUN_14005b104(&local_38);
        return 0;
    }
    PECMD_SplitTokenTrimWs((int64_t *)local_res10, (int64_t *)&local_38, 0x3d);
    PECMD_RunCommandLine(param_1, &local_38, 0);
    if (*local_38 != L'\0') {
        if (*local_res10[0] == L'=') {
            ((LPWSTR)local_res10[0])[0] = L'\0';
            local_res10[0] = local_res10[0] + 1;
            WVar5 = L',';
            local_res20[0] = 0;
            local_30 = local_res10[0];
            PECMD_SkipWCharUntil((WCHAR **)local_res10, 0x2c);
            if (*local_res10[0] == WVar5) {
                ((LPWSTR)local_res10[0])[0] = L'\0';
                local_res10[0] = local_res10[0] + 1;
                local_28 = (LPCWSTR)0;
                if (cVar1 != '\0') {
                    PECMD_JoinTokensAndResolve(param_1, (int64_t *)local_res10, (int64_t *)&local_28);
                    local_res10[0] = local_28;
                }
                local_20 = local_30;
                FUN_14005b154((WCHAR **)&local_20);
                if (*local_20 != L'(') {
                    pWVar4 = local_20;
                    if ((*local_20 == L'-') && ((uint16_t)(local_20[1] + 0xffd0) < 10)) {
                        pWVar4 = local_20 + 1;
                    }
                    uVar3 = PECMD_IsNumericString(pWVar4);
                    if (((int)uVar3 == 0) &&
                        (uVar3 = PECMD_IsNumericString(local_res10[0]), pWVar4 = local_res10[0],
                         (int)uVar3 != 0)) {
                        local_res10[0] = local_30;
                        local_30 = pWVar4;
                    }
                }
                FUN_1400c11c0((WCHAR **)&local_30, local_res20);
                iVar2 = lstrlenW(local_res10[0]);
                if ((local_res20[0] < 1) || (iVar2 < local_res20[0])) {
                    local_res20[0] = iVar2;
                }
                local_res10[0] = local_res10[0] + (iVar2 - local_res20[0]);
                PECMD_SetVariable(param_1, local_38, local_res10[0]);
                FUN_14005b104(&local_28);
                goto LAB_0a0a21;
            }
        }
        uVar6 = 0xffffffff80070057ULL;
    }
LAB_0a0a21:
    FUN_14005b104(&local_38);
    return uVar6;
}

/* ==========================================================================
 * @0x1400a0d38  (LPCWSTR* PECMD_SearchStringAndLocate(longlong*, WCHAR*, undefined8, byte))
 * 字符串/列表查找: 解析分隔符/大小写/星号等修饰, 计算命中位置并写回变量。
 * 返回错误码 (0xffffffff80070057) 或命中指针, 以 LPCWSTR* 位模式承载。
 * ========================================================================== */
LPCWSTR *PECMD_SearchStringAndLocate(int64_t *param_1, WCHAR *param_2, uint64_t param_3, uint8_t param_4)
{
    WCHAR WVar1;
    bool bVar2;
    char cVar3;
    char cVar4;
    char cVar5;
    uint32_t uVar6;
    int iVar7;
    int iVar8;
    int iVar9;
    int iVar10;
    uint32_t uVar11;
    uint64_t uVar12;
    LPCWSTR pWVar13;
    uintptr_t ppWVar14;
    int iVar15;
    LPCWSTR pWVar16;
    WCHAR *pWVar17;
    WCHAR WVar18;
    LPCWSTR pWVar19;
    uintptr_t ppWVar20;
    uintptr_t ppWVar21;
    WCHAR *local_res10;
    LPCWSTR local_f0;
    LPCWSTR local_e8;
    int local_e0;
    uint32_t local_d0;
    uint32_t local_cc;
    int local_c8[2];
    LPCWSTR local_c0;
    int local_b8;
    LPCWSTR local_b0;
    LPCWSTR local_a8;
    LPCWSTR local_a0;
    uint64_t local_98;
    LPCWSTR local_90;
    LPCWSTR local_88;
    uint64_t local_80;
    LPCWSTR local_78;
    LPCWSTR local_70;
    uint64_t local_68;
    uint64_t local_60;
    LPCWSTR local_58;
    uint64_t local_50;
    uint64_t local_48;

    ppWVar14 = 0;
    local_cc = 0;
    local_d0 = 0;
    ppWVar20 = 0;
    ppWVar21 = ppWVar14;
    local_res10 = param_2;
    if (*param_2 == L'*') {
        if ((param_2[1] == L'*') && (param_2[2] == L'*')) {
            param_4 = param_4 | 2;
            local_res10 = param_2 + 3;
            ppWVar21 = (uintptr_t)0x10;
        }
        else {
            ppWVar21 = ppWVar20;
            if (*param_2 == L'*') {
                if (param_2[1] == L'*') {
                    param_4 = param_4 | 2;
                    local_res10 = param_2 + 2;
                    ppWVar21 = ppWVar14;
                }
                else if (*param_2 == L'*') {
                    param_4 = param_4 | 1;
                    local_res10 = param_2 + 1;
                }
            }
        }
    }
    WVar18 = *local_res10;
    if (WVar18 == L'$') {
        local_cc = 0x20;
        ppWVar21 = (uintptr_t)0x10;
    }
    if (WVar18 == L'#') {
        local_cc = (local_cc & 0xFFFF0000u) | 9;
        ppWVar21 = (uintptr_t)0x10;
    }
    while ((uVar6 = (uint32_t)ppWVar21, WVar18 != L'\0' &&
           ((((uint16_t)WVar18 < 9) || (0xd < (uint16_t)WVar18)) && (WVar18 != L' ')))) {
        local_res10 = local_res10 + 1;
        WVar18 = *local_res10;
    }
    FUN_14005b154(&local_res10);
    cVar3 = FUN_1400660ac("*", (WCHAR **)&local_res10, 1);
    cVar4 = '\0';
    if (cVar3 != '\0') {
        cVar4 = FUN_1400660ac("*", (WCHAR **)&local_res10, 1);
    }
    local_a8 = g_szEmpty;
    PECMD_StrDupAssign((WCHAR **)&local_b0, g_szEmpty);
    ppWVar20 = ppWVar14;
    if (*local_res10 == L'-') {
        do {
            cVar5 = FUN_1400660ac("-qu", (WCHAR **)&local_res10, 3);
            if (cVar5 == '\0') {
                cVar5 = FUN_1400660ac("-case", (WCHAR **)&local_res10, 5);
                if (cVar5 == '\0') {
                    pWVar17 = local_res10;
                    uVar12 = PECMD_AsciiPrefixICmp("-delims:", (uint16_t *)local_res10, 8);
                    if ((char)uVar12 != '\0') {
                        local_res10 = pWVar17 + 8;
                        ppWVar21 = (uintptr_t)0x10;
                        uVar6 = PECMD_ParseStringToken((int64_t *)&local_res10, param_1, (int64_t *)0);
                        local_cc = (((uint32_t)(uint16_t)(uVar6 >> 16)) << 16) |
                                   (uint16_t)(*local_res10);
                        pWVar17 = local_res10;
                    }
                    WVar18 = *pWVar17;
                    while ((WVar18 != L'\0' && (((uint16_t)WVar18 < 9) ||
                           (0xd < (uint16_t)WVar18))) && (WVar18 != L' ')) {
                        pWVar17 = pWVar17 + 1;
                        local_res10 = pWVar17;
                        WVar18 = *pWVar17;
                    }
                    FUN_14005b154(&local_res10);
                }
                else {
                    ppWVar20 = (uintptr_t)0x1;
                }
            }
            else {
                ppWVar21 = (uintptr_t)0x10;
            }
            uVar6 = (uint32_t)ppWVar21;
        } while (*local_res10 == L'-');
        local_d0 = (uint32_t)ppWVar20;
    }
    pWVar16 = g_szEmpty;
    if (*local_res10 == L'\0') {
LAB_0a0f9b:
        FUN_14005b104(&local_b0);
        return (LPCWSTR *)ppWVar14;
    }
    local_88 = (LPCWSTR)0;
    FUN_140063620((WCHAR **)&local_90);
    PECMD_SplitTokenTrimWs((int64_t *)&local_res10, (int64_t *)&local_b0, 0x3d);
    PECMD_RunCommandLine(param_1, &local_b0, 1);
    local_a0 = local_b0;
    local_78 = local_b0;
    PECMD_SkipWCharUntil((WCHAR **)&local_78, 0x2c);
    if (*local_78 != L'\0') {
        *((LPWSTR)local_78) = L'\0';
        pWVar16 = local_78 + 1;
        local_a8 = pWVar16;
        local_78 = pWVar16;
    }
    FUN_140063620((WCHAR **)&local_58);
    local_50 = 0;
    local_48 = 0;
    FUN_140063620((WCHAR **)&local_70);
    local_68 = 0;
    local_60 = 0;
    (void)local_50;
    (void)local_48;
    (void)local_68;
    (void)local_60;
    ppWVar21 = ppWVar14;
    if (*local_a0 == L'*') {
        local_a0 = local_a0 + 1;
        ppWVar21 = (uintptr_t)&local_58;
    }
    ppWVar20 = ppWVar14;
    if (*pWVar16 == L'*') {
        pWVar16 = pWVar16 + 1;
        ppWVar20 = (uintptr_t)&local_70;
        local_a8 = pWVar16;
    }
    local_b8 = 1;
    local_e0 = 1;
    if (*local_a0 == L'~') {
        local_a0 = local_a0 + 1;
        local_b8 = -1;
    }
    local_98 = -(uint64_t)(*local_a0 != L'\0') & (uint64_t)ppWVar21;
    if (*pWVar16 == L'~') {
        pWVar16 = pWVar16 + 1;
        local_e0 = -1;
        local_a8 = pWVar16;
    }
    local_80 = -(uint64_t)(*pWVar16 != L'\0') & (uint64_t)ppWVar20;
    bVar2 = (local_98 == 0) && (local_80 == 0);
    if ((*local_a0 == L'\0') && (*pWVar16 == L'\0')) {
        FUN_14005b104(&local_70);
        FUN_14005b104(&local_58);
        FUN_14005b104(&local_90);
        FUN_14005b104(&local_88);
        goto LAB_0a0f9b;
    }
    if (*local_res10 != L'=') {
        goto LAB_0a18da;
    }
    pWVar16 = local_res10 + 1;
    local_res10 = (WCHAR *)pWVar16;
    do {
        pWVar13 = local_res10;
        local_res10 = (WCHAR *)(pWVar13 + 1);
        local_e8 = pWVar16;
        if (*local_res10 == L'\0') {
            if (*local_res10 != L',') {
                goto LAB_0a12fd;
            }
            break;
        }
    } while (*local_res10 != L',');
    *local_res10 = L'\0';
    pWVar13 = pWVar13 + 2;
    local_res10 = (WCHAR *)pWVar13;
    if (param_4 == 0) {
        local_f0 = pWVar16;
        FUN_14005b154((WCHAR **)&local_f0);
        if ((*local_f0 != L'\0') &&
            (((8 < (uint16_t)*pWVar16 && ((uint16_t)*pWVar16 < 0xe)) || (*pWVar16 == L' ')))) {
            local_e8 = local_f0;
            pWVar16 = local_f0;
        }
        pWVar13 = pWVar13 + -2;
        if (pWVar16 < pWVar13) {
            do {
                if ((((uint16_t)*pWVar13 < 9) || (0xd < (uint16_t)*pWVar13)) &&
                    (*pWVar13 != L' ')) {
                    break;
                }
                *((LPWSTR)pWVar13) = L'\0';
                pWVar13 = pWVar13 + -1;
            } while (local_e8 < pWVar13);
        }
    }
    FUN_140063620((WCHAR **)&local_c0);
    FUN_14007bf44(param_1, local_e8, (int64_t *)&local_c0, 0, 1);
    local_e8 = local_c0;
    if (cVar4 != '\0') {
        local_f0 = local_c0;
        local_c0 = (LPCWSTR)0;
        PECMD_JoinTokensAndResolve(param_1, (int64_t *)&local_e8, (int64_t *)&local_c0);
        local_e8 = local_c0;
        FUN_14005b104(&local_f0);
    }
    WVar18 = *local_e8;
    uVar11 = 0;
    ppWVar21 = ppWVar14;
    if (WVar18 == L'\0') {
        iVar8 = (int)(intptr_t)local_f0;
        pWVar16 = local_res10;
        ppWVar14 = (uintptr_t)0xffffffff80070057ULL;
        pWVar17 = local_res10;
    }
    else {
        if ((local_e8[1] == L'\0') || (param_4 = param_4 | 1, local_e8[1] == L'\0')) {
            param_4 = param_4 & 0xfe;
        }
        FUN_14007bf44(param_1, local_res10, (int64_t *)&local_90, 0, 1);
        local_res10 = (WCHAR *)local_90;
        local_c8[0] = 1;
        FUN_1400679b0(&local_res10, local_c8, 0x2c);
        if (*local_res10 != L',') {
            FUN_14005b104((int64_t *)&local_c0);
LAB_0a12fd:
            ppWVar14 = (uintptr_t)0xffffffff80070057ULL;
            goto LAB_0a18da;
        }
        local_res10 = local_res10 + 1;
        pWVar16 = local_res10;
        if (cVar3 != '\0') {
            PECMD_JoinTokensAndResolve(param_1, (int64_t *)&local_res10, (int64_t *)&local_88);
            pWVar16 = local_88;
        }
        local_res10 = (WCHAR *)pWVar16;
        iVar7 = lstrlenW(pWVar16);
        iVar8 = local_b8;
        local_f0 = (LPCWSTR)(int64_t)iVar7;
        pWVar17 = (WCHAR *)((int64_t)local_f0 + 1);
        if (local_d0 == 0) {
            if ((uint16_t)(WVar18 + 0xffbf) < 0x1a) {
                WVar18 = (WCHAR)(WVar18 | 0x20);
            }
            cVar4 = '\x01';
            if (0x7a < (uint16_t)WVar18) {
                goto LAB_0a13ff;
            }
        }
        else {
LAB_0a13ff:
            cVar4 = '\0';
        }
        if ((param_4 & 2) == 0) {
            if (param_4 == 0) {
                if (local_c8[0] < 1) {
                    pWVar13 = local_f0;
                    uVar6 = local_d0;
                    for (local_res10 = (WCHAR *)(pWVar16 + (int64_t)local_f0 + -1);
                         pWVar19 = local_f0, pWVar16 <= local_res10;
                         local_res10 = local_res10 + -1) {
                        iVar7 = (int)(intptr_t)pWVar13;
                        WVar1 = *local_res10;
                        if ((WVar1 == WVar18) ||
                            (((uVar6 == 0 && ((uint16_t)WVar1 < 0x7b)) &&
                              ((WCHAR)(WVar1 | 0x20U) == WVar18)))) {
                            ppWVar21 = (uintptr_t)(((int64_t)local_res10 -
                                                   (int64_t)(pWVar16 + -1)) >> 1);
                            if (local_98 != 0) {
                                ppWVar20 = ppWVar21;
                                if ((iVar8 < 1) && (0 < (int64_t)ppWVar21)) {
                                    ppWVar20 = (uintptr_t)((int64_t)pWVar17 - (int64_t)ppWVar21);
                                }
                                PECMD_AppendFormattedI64((int64_t *)&local_58, (int64_t)ppWVar20);
                                iVar7 = (int)(intptr_t)local_f0;
                            }
                            if (local_80 != 0) {
                                iVar7 = (int)FUN_140060b5c((uint64_t *)pWVar16,
                                                          (int)ppWVar21, (void *)(intptr_t)local_e0,
                                                          iVar7);
                                PECMD_AppendFormattedI64((int64_t *)&local_70, (int64_t)iVar7);
                            }
                            pWVar13 = local_f0;
                            pWVar19 = local_f0;
                            uVar6 = local_d0;
                            if (bVar2) {
                                break;
                            }
                        }
                    }
                }
                else {
                    WVar1 = *pWVar16;
                    pWVar13 = local_f0;
                    local_res10 = (WCHAR *)pWVar16;
                    iVar7 = local_c8[0];
                    while (pWVar19 = local_f0, WVar1 != L'\0') {
                        iVar9 = (int)(intptr_t)pWVar13;
                        if (((*local_res10 == WVar18) ||
                             ((cVar4 != '\0' && ((WCHAR)(*local_res10 | 0x20U) == WVar18)))) &&
                            (iVar7 = iVar7 + -1, iVar7 < 1)) {
                            ppWVar21 = (uintptr_t)(((int64_t)local_res10 -
                                                   (int64_t)(pWVar16 + -1)) >> 1);
                            if (local_98 != 0) {
                                ppWVar20 = ppWVar21;
                                if ((iVar8 < 1) && (0 < (int64_t)ppWVar21)) {
                                    ppWVar20 = (uintptr_t)((int64_t)pWVar17 - (int64_t)ppWVar21);
                                }
                                PECMD_AppendFormattedI64((int64_t *)&local_58, (int64_t)ppWVar20);
                                iVar9 = (int)(intptr_t)local_f0;
                            }
                            if (local_80 != 0) {
                                iVar9 = (int)FUN_140060b5c((uint64_t *)pWVar16, (int)ppWVar21,
                                                          (void *)(intptr_t)local_e0, iVar9);
                                PECMD_AppendFormattedI64((int64_t *)&local_70, (int64_t)iVar9);
                            }
                            pWVar13 = local_f0;
                            pWVar19 = local_f0;
                            if (bVar2) {
                                break;
                            }
                        }
                        local_res10 = local_res10 + 1;
                        WVar1 = *local_res10;
                    }
                }
            }
            else {
                iVar7 = lstrlenW(local_e8);
                pWVar19 = local_f0;
                iVar9 = (int)(intptr_t)local_f0;
                if (local_c8[0] < 1) {
                    local_res10 = (WCHAR *)(pWVar16 + ((int64_t)local_f0 - (int64_t)iVar7));
                    if (pWVar16 <= local_res10) {
                        do {
                            if (local_d0 == 0) {
                                iVar15 = StrCmpNIW(local_res10, local_e8, iVar7);
                            }
                            else {
                                iVar15 = StrCmpNW(local_res10, local_e8, iVar7);
                            }
                            if (iVar15 == 0) {
                                ppWVar21 = (uintptr_t)(((int64_t)local_res10 -
                                                       (int64_t)(pWVar16 + -1)) >> 1);
                                if (local_98 != 0) {
                                    ppWVar20 = ppWVar21;
                                    if ((iVar8 < 1) && (0 < (int64_t)ppWVar21)) {
                                        ppWVar20 = (uintptr_t)((int64_t)pWVar17 -
                                                               (int64_t)ppWVar21);
                                    }
                                    PECMD_AppendFormattedI64((int64_t *)&local_58, (int64_t)ppWVar20);
                                }
                                if (local_80 != 0) {
                                    iVar15 = (int)FUN_140060b5c((uint64_t *)pWVar16,
                                                               (int)ppWVar21,
                                                               (void *)(intptr_t)local_e0, iVar9);
                                    PECMD_AppendFormattedI64((int64_t *)&local_70, (int64_t)iVar15);
                                }
                                if (bVar2) {
                                    break;
                                }
                            }
                            local_res10 = local_res10 + -1;
                        } while (pWVar16 <= local_res10);
                    }
                }
                else {
                    WVar18 = *pWVar16;
                    local_res10 = (WCHAR *)pWVar16;
                    iVar15 = local_c8[0];
                    while (WVar18 != L'\0') {
                        if (local_d0 == 0) {
                            iVar10 = StrCmpNIW(local_res10, local_e8, iVar7);
                        }
                        else {
                            iVar10 = StrCmpNW(local_res10, local_e8, iVar7);
                        }
                        if ((iVar10 == 0) && (iVar15 = iVar15 + -1, iVar15 < 1)) {
                            ppWVar21 = (uintptr_t)(((int64_t)local_res10 -
                                                   (int64_t)(pWVar16 + -1)) >> 1);
                            if (local_98 != 0) {
                                ppWVar20 = ppWVar21;
                                if ((iVar8 < 1) && (0 < (int64_t)ppWVar21)) {
                                    ppWVar20 = (uintptr_t)((int64_t)pWVar17 - (int64_t)ppWVar21);
                                }
                                PECMD_AppendFormattedI64((int64_t *)&local_58, (int64_t)ppWVar20);
                            }
                            if (local_80 != 0) {
                                iVar10 = (int)FUN_140060b5c((uint64_t *)pWVar16, (int)ppWVar21,
                                                            (void *)(intptr_t)local_e0, iVar9);
                                PECMD_AppendFormattedI64((int64_t *)&local_70, (int64_t)iVar10);
                            }
                            if (bVar2) {
                                break;
                            }
                        }
                        local_res10 = local_res10 + 1;
                        WVar18 = *local_res10;
                    }
                }
            }
        }
        else {
            iVar8 = (int)PECMD_FindTokenIndex(local_e8, (WCHAR *)pWVar16, (uint32_t)uVar6 | local_d0, local_c8[0],
                                       (uint16_t)local_cc);
            ppWVar21 = (uintptr_t)iVar8;
            pWVar19 = local_f0;
        }
        iVar8 = (int)(intptr_t)pWVar19;
    }
    pWVar13 = local_a8;
    if (*local_70 == L'\0') {
        if (*local_a8 != L'\0') {
            if (0 < (int64_t)ppWVar21) {
                uVar11 = (uint32_t)FUN_140060b5c((uint64_t *)pWVar16, (int)ppWVar21,
                                                 (void *)(intptr_t)local_e0, iVar8);
            }
            FUN_1400669c4(param_1, (uint64_t)uVar11, pWVar13);
        }
    }
    else {
        PECMD_SetVariable(param_1, local_a8, local_70);
    }
    if (*local_58 == L'\0') {
        if ((local_b8 < 1) && (0 < (int64_t)ppWVar21)) {
            ppWVar21 = (uintptr_t)((int64_t)pWVar17 - (int64_t)ppWVar21);
        }
        FUN_1400669c4(param_1, (uint64_t)(uint32_t)ppWVar21, local_a0);
    }
    else {
        PECMD_SetVariable(param_1, local_a0, local_58);
    }
    FUN_14005b104(&local_c0);
LAB_0a18da:
    FUN_14005b104(&local_70);
    FUN_14005b104(&local_58);
    FUN_14005b104(&local_90);
    FUN_14005b104(&local_88);
    FUN_14005b104(&local_b0);
    return (LPCWSTR *)ppWVar14;
}

/* ==========================================================================
 * @0x14009da54  (LARGE_INTEGER PECMD_ReadTextLine(longlong*, _FILETIME))
 * READLINE/读文件行: BOM/编码识别、行扫描、*fix *nl *left 修饰。
 * 注意: Ghidra 将大量局部量类型化为 _FILETIME 但实际是"编码标志寄存器 +
 * FILETIME 计数 + WCHAR 指针"的多态栈槽, 此处以 uint64_t 承载并用位运算
 * 访问字段 (TODO(verify))。
 * ========================================================================== */
LARGE_INTEGER PECMD_ReadTextLine(int64_t *param_1, FILETIME param_2)
{
  CHAR CVar1;
  uint16_t uVar2;
  int16_t sVar3;
  char cVar4;
  bool bVar5;
  DWORD DVar6;
  uint32_t uVar7;
  uint32_t uVar8;
  int64_t lVar9;
  LARGE_INTEGER LVar10;
  void *p_Var11;
  WCHAR WVar12;
  int iVar13;
  LPCSTR pCVar14;
  uint64_t _Var15;
  uint8_t bVar16;
  LPCWSTR pWVar17;
  LARGE_INTEGER hObject;
  uint32_t uVar18;
  LARGE_INTEGER LVar19;
  DWORD DVar20;
  char *pCVar21;
  uint64_t uVar22;
  LARGE_INTEGER LVar23;
  uint64_t _Var24;
  uint64_t _Var25;
  uint64_t _Var26;
  uint64_t _Var27;
  WCHAR WVar28;
  uint8_t bVar29;
  uint32_t uVar30;
  DWORD DVar31;
  DWORD DVar32;
  uint32_t uVar33;
  uint64_t local_res10;
  uint64_t local_res20;
  char local_e8;
  uint64_t local_e0;
  LARGE_INTEGER local_d8;
  uint64_t local_d0;
  uint64_t local_c8;
  uint32_t local_c0;
  uint8_t local_bc;
  LPCWSTR local_b8;
  LARGE_INTEGER local_b0;
  LARGE_INTEGER local_a8;
  uint64_t local_a0;
  LARGE_INTEGER local_98;
  LPCWSTR local_90;
  uint64_t local_88;
  uint64_t local_80;
  uint64_t local_78;
  int64_t local_70;
  char *local_68;
  uint64_t local_60;
  HANDLE local_58;
  uint64_t local_50;
  uint64_t local_48;
  uint64_t pp;

  pp = ((uint64_t)(uint32_t)param_2.dwHighDateTime << 32) | (uint32_t)param_2.dwLowDateTime;

  LVar19.QuadPart = 0;
  DVar6 = 0;
  DVar20 = 0;
  bVar16 = 0;
  local_e8 = '\0';
  uVar22 = local_c8 >> 0x20;
  local_c8 = local_c8 & 0xffffffff00000000ULL;
  local_res20 = local_res20 & 0xffffffffffffff00ULL;
  DVar32 = 1;
  _Var27 = 1;
  uVar18 = 0;
  LVar10.QuadPart = LVar19.QuadPart;
  local_res10 = pp;

  if (((*(uint16_t *)(uintptr_t)pp < 9) || (0xd < *(uint16_t *)(uintptr_t)pp)) &&
      (*(int16_t *)(uintptr_t)pp != 0x20)) {
    lVar9 = FUN_14005c72c("-UNI", (const WCHAR *)(uintptr_t)pp, 4);
    if (((char)lVar9 == '\0') &&
        (lVar9 = FUN_14005c72c("-UNICODE", (const WCHAR *)(uintptr_t)pp, 8),
         (char)lVar9 == '\0')) {
      lVar9 = FUN_14005c72c("-UNIB", (const WCHAR *)(uintptr_t)pp, 5);
      if (((char)lVar9 == '\0') &&
          (lVar9 = FUN_14005c72c("-UNICODEB", (const WCHAR *)(uintptr_t)pp, 9),
           (char)lVar9 == '\0')) {
        lVar9 = FUN_14005c72c("-ANSI", (const WCHAR *)(uintptr_t)pp, 5);
        if ((char)lVar9 == '\0') {
          lVar9 = FUN_14005c72c("-GBK", (const WCHAR *)(uintptr_t)pp, 4);
          if ((char)lVar9 == '\0') {
            lVar9 = FUN_14005c72c("-BIG5", (const WCHAR *)(uintptr_t)pp, 5);
            if ((char)lVar9 == '\0') {
              lVar9 = FUN_14005c72c("-UTF8", (const WCHAR *)(uintptr_t)pp, 5);
              if ((char)lVar9 == '\0') {
                lVar9 = FUN_14005c72c("-UTF7", (const WCHAR *)(uintptr_t)pp, 5);
                if ((char)lVar9 == '\0') {
                  LVar10.QuadPart = 0xffff0000;
                  if ((*(uint16_t *)(uintptr_t)pp == 0x2d) &&
                      (_Var24 = (uint64_t)((int64_t)pp + 2),
                       (uint16_t)(*(uint16_t *)(uintptr_t)_Var24 - 0x30) < 10)) {
                    local_res10 = _Var24;
                    LVar10.QuadPart = FUN_140064a88((const WCHAR *)(uintptr_t)_Var24);
                    pp = _Var24;
                  }
                }
                else {
                  LVar10.QuadPart = 65000;
                }
              }
              else {
                LVar10.QuadPart = 0xfde9;
              }
            }
            else {
              LVar10.QuadPart = 0x3b6;
            }
          }
          else {
            LVar10.QuadPart = 0x3a8;
          }
        }
      }
      else {
        LVar10.QuadPart = 0x4b1;
      }
    }
    else {
      LVar10.QuadPart = 0x4b0;
    }
  }
  else {
    local_c8 = ((uint64_t)(uint32_t)uVar22 << 32) | 1;
  }

  while (1) {
    DVar31 = LVar10.LowPart;
    uVar2 = *(uint16_t *)(uintptr_t)pp;
    if ((uVar2 == 0) || (((8 < uVar2) && (uVar2 < 0xe)) || (uVar2 == 0x20))) break;
    local_res10 = (uint64_t)((int64_t)pp + 2);
    pp = local_res10;
  }

  FUN_14005b154((WCHAR **)&local_res10);
  bVar29 = 0;
  while (*(uint16_t *)(uintptr_t)local_res10 == 0x2a) {
    cVar4 = FUN_1400660ac("*fix", (WCHAR **)&local_res10, 4);
    if (cVar4 == '\0') {
      cVar4 = FUN_1400660ac("*nl", (WCHAR **)&local_res10, 3);
      if (cVar4 == '\0') {
        cVar4 = FUN_1400660ac("*left", (WCHAR **)&local_res10, 5);
        if (cVar4 == '\0') break;
        bVar29 = (uint8_t)(bVar29 | 0xc);
      }
      else {
        bVar29 = (uint8_t)(bVar29 | 4);
      }
    }
    else {
      bVar29 = (uint8_t)(bVar29 | 2);
    }
  }
  local_bc = bVar29 & 0xc;

  PECMD_StrDupAssign((WCHAR **)&local_90, g_szEmpty);
  PECMD_StrDupAssign((WCHAR **)&local_b8, WSTR("READLINE"));
  local_e0 = 1;
  local_88 = 1;
  local_98.QuadPart = 1;
  local_d8.QuadPart = 1;
  local_d0 = 0;
  local_res10 = (uint64_t)FUN_1400545f8(param_1, (int64_t *)&local_res10,
                                        (int64_t *)&local_90, L',', 0);
  pWVar17 = local_90;

  uVar7 = PECMD_AsciiWideICmp("-*?", (const WCHAR *)local_90);
  if ((char)uVar7 != '\0') {
    bVar16 = 9;
    local_d0 = 1;
    local_res20 = (local_res20 & 0xffffffffffffff00ULL) | (uint8_t)9;
  }
  uVar7 = PECMD_AsciiWideICmp("-*", (const WCHAR *)pWVar17);
  if ((char)uVar7 == '\0') {
    if ((*pWVar17 == L'\0') ||
        (uVar7 = PECMD_AsciiWideICmp("-", (const WCHAR *)pWVar17),
         (char)uVar7 != '\0'))
      goto LAB_14009dd8f;
    uVar7 = PECMD_AsciiWideICmp("--", (const WCHAR *)pWVar17);
    if ((char)uVar7 != '\0') {
      bVar16 = 2;
      bVar29 = (uint8_t)(bVar29 | 0x10);
      goto LAB_14009dd92;
    }
  }
  else {
    local_d0 = 1;
LAB_14009dd8f:
    bVar16 = (uint8_t)(bVar16 | 1);
LAB_14009dd92:
    local_res20 = (local_res20 & 0xffffffffffffff00ULL) | bVar16;
  }

  local_68 = (char *)0x0;
  FUN_140063620((WCHAR **)&local_60);
  uVar7 = 0;
  PECMD_ExpandCommandLine(param_1, (WCHAR *)(uintptr_t)local_res10, (WCHAR **)&local_60, 0, 1);
  local_res10 = local_60;

  if (*(WCHAR *)(uintptr_t)local_60 == L',') {
    FUN_140063620((WCHAR **)&local_b0);
    local_res10 = (uint64_t)((int64_t)local_res10 + 2);
    PECMD_SplitTokenTrimWs((int64_t *)&local_res10, &local_b0.QuadPart, 0x2c);
    local_a8 = local_b0;
    LVar10 = local_b0;
    uVar8 = PECMD_AsciiWideICmp("*r", (const WCHAR *)(uintptr_t)local_b0.QuadPart);
    if ((char)uVar8 == '\0') {
      uVar8 = PECMD_AsciiWideICmp("**1", (const WCHAR *)(uintptr_t)LVar10.QuadPart);
      if ((char)uVar8 == '\0') {
        uVar8 = PECMD_AsciiWideICmp("**", (const WCHAR *)(uintptr_t)LVar10.QuadPart);
        if ((char)uVar8 == '\0') {
          uVar8 = PECMD_AsciiWideICmp("*", (const WCHAR *)(uintptr_t)LVar10.QuadPart);
          if ((char)uVar8 == '\0') {
            FUN_1400745c8((WCHAR **)&local_a8.QuadPart, (uint64_t *)&local_88);
            if (*(int16_t *)(uintptr_t)local_a8.QuadPart == 0x2b) {
              local_a8.QuadPart = local_a8.QuadPart + 2;
              FUN_1400745c8((WCHAR **)&local_a8.QuadPart, (uint64_t *)&local_d8.QuadPart);
              local_98 = local_d8;
            }
            local_e0 = local_88;
            _Var27 = local_88;
          }
          else {
            local_e8 = '\x01';
            local_68 = "\n";
          }
        }
        else {
          local_e8 = '\x03';
          _Var27 = 0;
          local_e0 = 0;
          local_98.QuadPart = 0;
        }
      }
      else {
        local_e8 = '\x03';
      }
    }
    else {
      local_e8 = '\x02';
      local_68 = "\r\n";
      bVar16 = (uint8_t)local_res20;
    }
    FUN_14005b104((void *)&local_b0.QuadPart);
    if (*(WCHAR *)(uintptr_t)local_res10 == L',') {
      local_res10 = (uint64_t)((int64_t)local_res10 + 2);
      PECMD_SplitTokenTrimWs((int64_t *)&local_res10, (int64_t *)&local_b8, 0x2c);
    }
  }

  PECMD_RunCommandLine(param_1, (void *)&local_b8, 0);
  FUN_1400633a8((void **)&local_d8, 8);
  LVar10 = local_d8;
  *(int16_t *)(uintptr_t)local_d8.QuadPart = 0;

  if (-1 < (int)DVar31) {
    PECMD_AllocSmallObject((void *)&local_80);
    local_78 = 0;
    local_70 = 0;
    if (((*local_90 == L'u') && (local_90[1] == L'd')) &&
        (WVar12 = L':', local_90[2] == L':')) {
      _Var24 = (uint64_t)(uintptr_t)(local_90 + 3);
      WVar28 = L'\"';
      *(WCHAR *)(uintptr_t)local_res10 = L'\0';
      if (*(WCHAR *)(uintptr_t)_Var24 == L'\"') {
        _Var24 = (uint64_t)(uintptr_t)(local_90 + 4);
      }
      local_res20 = _Var24;
      PECMD_SkipWCharUntil((WCHAR **)&local_res20, 0x3a);
      if ((local_res20 != _Var24) &&
          (*(WCHAR *)(uintptr_t)((int64_t)local_res20 + -2) == WVar28)) {
        *(WCHAR *)(uintptr_t)((int64_t)local_res20 + -2) = L'\0';
      }
      if (WVar12 == *(WCHAR *)(uintptr_t)local_res20) {
        *(WCHAR *)(uintptr_t)local_res20 = L'\0';
        local_res20 = (uint64_t)((int64_t)local_res20 + 2);
      }
      _Var15 = local_res20;
      if (WVar28 == *(WCHAR *)(uintptr_t)local_res20) {
        _Var15 = (uint64_t)((int64_t)local_res20 + 2);
        do {
          local_res20 = (uint64_t)((int64_t)local_res20 + 2);
          if (WVar28 == *(WCHAR *)(uintptr_t)local_res20) {
            *(WCHAR *)(uintptr_t)local_res20 = L'\0';
          }
        } while (*(WCHAR *)(uintptr_t)local_res20 != L'\0');
      }
      PECMD_GenerateTimeText((LPCWSTR)(uintptr_t)_Var24, (int64_t *)&local_80,
                    (LPCWSTR)(uintptr_t)_Var15, 0x10000, (FILETIME){0, 0});
      if (0 < (int)local_78) {
        local_res10 = (uint64_t)((int64_t)local_80 + local_70);
      }
      iVar13 = (int)*(char *)(uintptr_t)local_res10;
      if ((iVar13 == 0xff) && (*(char *)(uintptr_t)((int64_t)local_res10 + 1) == 0xfe)) {
        DVar31 = 0x4b0;
        LVar10.QuadPart = 2;
      }
      else if ((iVar13 == 0xfe) && (*(char *)(uintptr_t)((int64_t)local_res10 + 1) == 0xff)) {
        DVar31 = 0x4b1;
        LVar10.QuadPart = 2;
      }
      else {
        LVar10.QuadPart = LVar19.QuadPart;
        if (((iVar13 == 0xef) &&
             (*(char *)(uintptr_t)((int64_t)local_res10 + 1) == 0xbb)) &&
            (LVar10.QuadPart = 0,
             *(char *)(uintptr_t)((int64_t)local_res10 + 2) == 0xbf)) {
          LVar10.QuadPart = 3;
          DVar31 = 0xfde9;
        }
      }
      iVar13 = (int)local_78 - LVar10.LowPart;
      pCVar21 = (char *)(uintptr_t)(LVar10.QuadPart + (int64_t)local_res10);
      uVar33 = (uint32_t)local_c8;
      if (DVar31 == 0x4b1) {
        pCVar14 = pCVar21 + ((int64_t)iVar13 & 0xfffffffffffffffeLL);
        for (; pCVar21 <= pCVar14; pCVar21 = pCVar21 + 2) {
          CVar1 = *pCVar21;
          *pCVar21 = pCVar21[1];
          pCVar21[1] = CVar1;
        }
      }
      else if (DVar31 != 0x4b0) {
        local_a0 = local_80;
        if ((uint32_t)local_c8 != 0) {
          uVar7 = FUN_1400799f0((LPCWSTR)pCVar21, (int64_t)iVar13);
          DVar31 = (DWORD)uVar7;
          local_a0 = local_80;
        }
        local_80 = 0;
        PECMD_GrowByteBuffer((void **)&local_80, (int64_t)iVar13 * 2 + 0x20);
        PECMD_StrDupA((WCHAR **)&local_80, (LPCWSTR)pCVar21, (int64_t)iVar13,
                      (int64_t)(int)DVar31);
        local_res10 = local_80;
        FUN_14005b104((void *)&local_a0);
      }
      uVar33 = uVar33 | 4;
      bVar16 = 0;
LAB_14009e301:
      local_e0 = 0;
      if (local_d0 != 0) {
        FUN_14005b154((WCHAR **)&local_res10);
        _Var15 = local_res10;
        WVar12 = *(WCHAR *)(uintptr_t)local_res10;
        _Var24 = local_res10;
        while (WVar12 != L'\0') {
          if (((8 < (uint16_t)WVar12) && ((uint16_t)WVar12 < 0xe)) || (WVar12 == L' ')) {
            if (*(WCHAR *)(uintptr_t)_Var24 != L'\0') {
              *(WCHAR *)(uintptr_t)_Var24 = L'\0';
            }
            break;
          }
          _Var24 = (uint64_t)((int64_t)_Var24 + 2);
          WVar12 = *(WCHAR *)(uintptr_t)_Var24;
        }
        sVar3 = *(int16_t *)(uintptr_t)local_res10;
        local_res10 = _Var24;
        if ((sVar3 != 0) &&
            (lVar9 = PECMD_VarLookup(param_1, (LPCWSTR)(uintptr_t)_Var15, (void *)0, -1,
                                   (uint64_t *)0),
             lVar9 != 0)) {
          pWVar17 = *(LPCWSTR *)(uintptr_t)(lVar9 + 8);
          uVar22 = *(uint64_t *)(uintptr_t)(lVar9 + 0x18) & 0x3fffffffffffffffULL;
          if ((bVar16 & 8) != 0) {
            uVar30 = uVar18;
            if (((char)*pWVar17 == -1) && (*(char *)(uintptr_t)((int64_t)pWVar17 + 1) == -2)) {
              LVar10.QuadPart = 2;
              DVar31 = 0x4b0;
            }
            else if (((char)*pWVar17 == -2) && (*(char *)(uintptr_t)((int64_t)pWVar17 + 1) == -1)) {
              LVar10.QuadPart = 2;
              DVar31 = 0x4b1;
            }
            else {
              LVar10.QuadPart = LVar19.QuadPart;
              uVar30 = uVar33;
              if ((((char)*pWVar17 == -0x11) && (*(char *)(uintptr_t)((int64_t)pWVar17 + 1) == -0x45)) &&
                  (LVar10.QuadPart = 0, (char)pWVar17[1] == -0x41)) {
                LVar10.QuadPart = 3;
                DVar31 = 0xfde9;
                uVar30 = uVar18;
              }
            }
            uVar22 = uVar22 - (int64_t)LVar10.QuadPart;
            pWVar17 = (LPCWSTR)((intptr_t)pWVar17 + LVar10.QuadPart);
            uVar33 = uVar30;
            if (((DVar31 != 0x4b1) && (DVar31 != 0x4b0)) &&
                (uVar33 = uVar18, uVar30 != 0)) {
              uVar7 = FUN_1400799f0(pWVar17, uVar22);
              DVar31 = (DWORD)uVar7;
            }
          }
          if (((int)uVar33 < 1) && (DVar31 != 0x4b0)) {
            PECMD_StrDupA((WCHAR **)&local_e0, pWVar17, uVar22, (int64_t)(int)DVar31);
          }
          else {
            PECMD_StrCopyW((WCHAR **)&local_e0, pWVar17, uVar22 >> 1);
          }
          local_res10 = local_e0;
        }
      }
      local_res20 = local_res10;
      _Var24 = local_res10;
      if (local_e8 == '\x03') {
LAB_14009e4a3:
        PECMD_SetVariable(param_1, local_b8, (LPCWSTR)(uintptr_t)_Var24);
      }
      else {
        if (local_e8 == '\0') {
          if ((int64_t)_Var27 < 0) {
            WVar12 = *(WCHAR *)(uintptr_t)local_res10;
            while (WVar12 != L'\0') {
              local_res10 = (uint64_t)((int64_t)local_res10 + 2);
              WVar12 = *(WCHAR *)(uintptr_t)local_res10;
            }
            PECMD_AllocWStringBuffer((WCHAR **)&local_res20, 0x20);
            wsprintfW((LPWSTR)(uintptr_t)local_res20, WSTR("%d"));
          }
          else {
            if (_Var27 == 0) {
              WVar12 = *(WCHAR *)(uintptr_t)local_res10;
              _Var27 = local_res10;
              _Var15 = local_res10;
              _Var26 = local_res10;
              if (WVar12 != L'\0') {
                do {
                  _Var25 = _Var26;
                  _Var24 = _Var27;
                  if (WVar12 == L'\n') {
                    _Var24 = _Var25;
                    _Var15 = _Var27;
                  }
                  _Var26 = (uint64_t)((int64_t)_Var25 + 2);
                  WVar12 = *(WCHAR *)(uintptr_t)_Var26;
                  _Var27 = _Var24;
                } while (WVar12 != L'\0');
                if (((uint64_t)local_res10 < (uint64_t)_Var26) &&
                    (*(WCHAR *)(uintptr_t)_Var25 != L'\n')) {
                  local_res10 = _Var26;
                  if (*(WCHAR *)(uintptr_t)_Var24 == L'\n') {
                    _Var24 = (uint64_t)((int64_t)_Var24 + 2);
                  }
                  goto LAB_14009e4a3;
                }
                bVar5 = (uint64_t)local_res10 < (uint64_t)_Var24;
                local_res10 = _Var26;
                if ((bVar5) && (*(WCHAR *)(uintptr_t)((int64_t)_Var24 + -2) == L'\r')) {
                  _Var24 = (uint64_t)((int64_t)_Var24 + -2);
                }
              }
              if (*(WCHAR *)(uintptr_t)_Var15 == L'\n') {
                _Var15 = (uint64_t)((int64_t)_Var15 + 2);
              }
              lVar9 = (int64_t)_Var24 - (int64_t)_Var15;
            }
            else {
              for (; (_Var15 = local_res10,
                      1 < (int64_t)_Var27 && (*(WCHAR *)(uintptr_t)local_res10 != L'\0'));
                   local_res10 = (uint64_t)((int64_t)local_res10 + 2)) {
                if (*(WCHAR *)(uintptr_t)local_res10 == L'\n') {
                  _Var27 = (uint64_t)((int64_t)_Var27 + -1);
                }
              }
              WVar12 = *(WCHAR *)(uintptr_t)local_res10;
              _Var27 = local_res10;
              LVar10 = local_98;
              while ((WVar12 != L'\0' &&
                      (((intptr_t)local_98.QuadPart == 0) ||
                       (LVar10.QuadPart = LVar10.QuadPart + -1, -1 < LVar10.QuadPart)))) {
                while ((WVar12 != L'\0' && ((WVar12 != L'\r' && (WVar12 != L'\n'))))) {
                  _Var27 = (uint64_t)((int64_t)_Var27 + 2);
                  local_res10 = _Var27;
                  WVar12 = *(WCHAR *)(uintptr_t)_Var27;
                }
                if (((intptr_t)local_98.QuadPart != 0) &&
                    ((intptr_t)LVar10.QuadPart == 0)) break;
                if (*(WCHAR *)(uintptr_t)_Var27 == L'\r') {
                  _Var27 = (uint64_t)((int64_t)_Var27 + 2);
                  local_res10 = _Var27;
                }
                if (*(WCHAR *)(uintptr_t)_Var27 == L'\n') {
                  _Var27 = (uint64_t)((int64_t)_Var27 + 2);
                  local_res10 = _Var27;
                }
                WVar12 = *(WCHAR *)(uintptr_t)_Var27;
              }
              lVar9 = (int64_t)_Var27 - (int64_t)_Var15;
            }
            FUN_1400702d4((WCHAR **)&local_res20, (const WCHAR *)(uintptr_t)_Var15,
                          lVar9 >> 1);
          }
        }
        else {
          pWVar17 = WSTR("\r\n");
          if (local_e8 == '\x01') {
            pWVar17 = WSTR("\n");
          }
          FUN_140063620((WCHAR **)&local_res20);
          WVar12 = *(WCHAR *)(uintptr_t)local_res10;
          _Var27 = local_res10;
          while (WVar12 != L'\0') {
            if (WVar12 == L'\r') {
              if (*(WCHAR *)(uintptr_t)((int64_t)local_res10 + 2) == L'\n') {
                *(WCHAR *)(uintptr_t)local_res10 = L'\0';
                local_res10 = (uint64_t)((int64_t)local_res10 + 2);
              }
              else {
LAB_14009e52d:
                *(WCHAR *)(uintptr_t)local_res10 = L'\0';
              }
              if (local_e8 == '\0') {
                FUN_14006375c((WCHAR **)&local_res20, pWVar17);
              }
              FUN_14006375c((WCHAR **)&local_res20, (LPCWSTR)(uintptr_t)_Var27);
              _Var27 = (uint64_t)((int64_t)local_res10 + 2);
              local_e8 = '\0';
            }
            else if (WVar12 == L'\n') goto LAB_14009e52d;
            local_res10 = (uint64_t)((int64_t)local_res10 + 2);
            WVar12 = *(WCHAR *)(uintptr_t)local_res10;
          }
          if (*(WCHAR *)(uintptr_t)_Var27 != L'\0') {
            if (local_e8 == '\0') {
              FUN_14006375c((WCHAR **)&local_res20, pWVar17);
            }
            FUN_14006375c((WCHAR **)&local_res20, (LPCWSTR)(uintptr_t)_Var27);
          }
        }
        PECMD_SetVariable(param_1, local_b8, (LPCWSTR)(uintptr_t)local_res20);
        FUN_14005b104((void *)&local_res20);
      }
      FUN_14005b104((void *)&local_e0);
LAB_14009ebed:
      FUN_14005b104((void *)&local_80);
      goto LAB_14009f024;
    }
    if (*local_b8 == L'\0') {
      FUN_14005b104((void *)&local_80);
      FUN_14005b104((void *)&local_d8.QuadPart);
      FUN_14005b104((void *)&local_60);
      FUN_14005b104((void *)&local_b8);
      FUN_14005b104((void *)&local_90);
      return (LARGE_INTEGER){ .QuadPart = 0 };
    }
    if ((bVar16 == 1) && (*(WCHAR *)(uintptr_t)local_res10 != L',')) {
      DVar32 = 0x80070057;
    }
    else {
      local_res10 = (uint64_t)((int64_t)local_res10 + 2);
      if ((bVar16 & 1) != 0) {
        uVar33 = (uint32_t)local_c8;
        bVar16 = (uint8_t)local_res20;
        goto LAB_14009e301;
      }
      FUN_1400633a8((void **)&local_a8, 8);
      cVar4 = (char)(uint8_t)local_res20;
      local_48 = 0;
      (void)local_48;
      *(int16_t *)(uintptr_t)LVar10.QuadPart = 0;
      local_50 = g_hStdIn;
      _Var27 = (uint64_t)(uintptr_t)&local_50;
      if ((uint8_t)local_res20 != '\x02') {
        _Var27 = (uint64_t)(uintptr_t)&local_b0;
      }
      local_b0.QuadPart = 0;
      local_88 = _Var27;
      if (g_u8CCB1 != '\0') {
        FUN_140018d8c((uint64_t)(uintptr_t)g_Script, WSTR("DBG %d:\r\n"), 0x556a, uVar7);
      }
      local_c0 = 0;
      hObject.QuadPart = LVar19.QuadPart;
      if (cVar4 != '\x02') {
        LVar23.QuadPart = 0;
        iVar13 = FUN_140006a4c(local_90);
        if (iVar13 == 0) {
          PECMD_OpenFileHandle((HANDLE *)&local_b0, local_90, 0x80000000, 7,
                        (LPSECURITY_ATTRIBUTES)0, 3, 0x2000000, (HANDLE)0);
        }
        else {
          local_b0.QuadPart = PECMD_OpenFileExisting((uint64_t)(uintptr_t)local_90, 0x80000000, 7);
        }
        hObject = local_b0;
        if (*(int64_t *)(uintptr_t)_Var27 != 0) {
          local_d0 = local_d0 & 0xffffff0000000000ULL;
          local_a0 = 0;
          ReadFile((HANDLE)*(int64_t *)(uintptr_t)_Var27, (LPVOID)&local_d0, 3,
                   (DWORD *)&local_a0, (void *)0);
          if ((char)local_d0 == -1) {
            LVar23.QuadPart = LVar19.QuadPart;
            if ((uint8_t)(local_d0 >> 8) == (uint8_t)-2) {
              LVar23.QuadPart = 2;
              DVar31 = 0x4b0;
              goto LAB_14009e945;
            }
          }
          else if ((char)local_d0 == -2) {
            LVar23.QuadPart = LVar19.QuadPart;
            if ((uint8_t)(local_d0 >> 8) == (uint8_t)-1) {
              LVar23.QuadPart = 2;
              DVar31 = 0x4b1;
LAB_14009e945:
              local_c8 = local_c8 & 0xffffffff00000000ULL;
            }
          }
          else if ((((char)local_d0 == -0x11) && ((uint8_t)(local_d0 >> 8) == (uint8_t)-0x45)) &&
                   ((uint8_t)(local_d0 >> 16) == (uint8_t)-0x41)) {
            LVar23.QuadPart = 3;
            DVar31 = 0xfde9;
            goto LAB_14009e945;
          }
          LVar23 = PECMD_SetFilePointer((HANDLE)*(int64_t *)(uintptr_t)_Var27, LVar23, 0);
          if (-1 < LVar23.QuadPart) {
            if ((uint32_t)local_c8 != 0) {
              DVar32 = PECMD_GetFileSize((HANDLE)*(int64_t *)(uintptr_t)_Var27);
              uVar22 = DVar32;
              if ((int64_t)uVar22 < 0x1000001) {
                PECMD_GrowByteBuffer((void **)&local_80, uVar22 + 0x10);
                ReadFile((HANDLE)*(int64_t *)(uintptr_t)_Var27, (LPVOID)local_80, DVar32,
                         (DWORD *)&local_a0, (void *)0);
                PECMD_SetFilePointer((HANDLE)*(int64_t *)(uintptr_t)_Var27, LVar23, 0);
                uVar7 = FUN_1400799f0((LPCWSTR)(uintptr_t)local_80, uVar22);
                DVar31 = (DWORD)uVar7;
              }
            }
            cVar4 = (char)(uint8_t)local_res20;
            goto LAB_14009ea18;
          }
        }
        if ((hObject.QuadPart != 0) && (hObject.QuadPart != -1)) {
          CloseHandle((HANDLE)hObject.QuadPart);
        }
        FUN_14005b104((void *)&local_a8.QuadPart);
        FUN_14005b104((void *)&local_80);
        DVar32 = GetLastError();
        if (DVar32 == 0) {
          DVar32 = 1;
        }
        goto LAB_14009f00c;
      }
LAB_14009ea18:
      local_58 = *(HANDLE *)(uintptr_t)_Var27;
      if (local_58 == (HANDLE)0) {
        DVar32 = 6;
      }
      else {
        local_a0 = 100000;
        SetLastError(0);
        if (local_e8 == '\x03') {
          local_c8 = 0;
          local_d0 = 0;
          uVar33 = (-(uint32_t)(local_e0 != 0) & 0xfffc0001u) + 0x40000;
          do {
            PECMD_GrowByteBuffer((void **)&local_c8, (uint64_t)uVar33 + 1 + local_d0);
            uVar22 = local_c8;
            local_68 = (char *)(uintptr_t)(local_d0 + local_c8);
            local_b0.QuadPart = 0;
            local_c0 = ReadFile(local_58, (LPVOID)local_68, uVar33,
                                (DWORD *)&local_b0.QuadPart, (void *)0);
            LVar10.HighPart = 0;
            LVar10.LowPart = uVar33;
            if (LVar10.QuadPart < local_b0.QuadPart) {
              local_b0 = LVar10;
            }
            if (local_b0.QuadPart == 0) {
              if (local_c0 != 0) {
                if (local_d0 == 0) {
                  uVar18 = 0x26;
                }
                break;
              }
              if (((uint8_t)local_res20 == '\x02') &&
                  (DVar32 = GetLastError(), DVar32 == 0x6d)) {
                uVar18 = 0x6d;
                break;
              }
              local_a0 = (uint64_t)((int64_t)local_a0 + -1);
              if ((int64_t)local_a0 < 0) break;
            }
            cVar4 = *local_68;
            local_d0 = local_d0 + local_b0.QuadPart;
            *(uint8_t *)(uintptr_t)(local_d0 + uVar22) = 0;
          } while ((((local_e0 == 0) || (local_c0 == 0)) || (cVar4 != '\n')) ||
                   (local_98.QuadPart = local_98.QuadPart + -1, 0 < local_98.QuadPart));
          pWVar17 = PECMD_ConvertStringEncoding((void *)&local_c8, DVar31);
          PECMD_SetVariable(param_1, local_b8, pWVar17);
          LVar19.QuadPart = (int64_t)(int)uVar18;
          FUN_14005b104((void *)&local_c8);
          if ((hObject.QuadPart != 0) && (hObject.QuadPart != -1)) {
            CloseHandle((HANDLE)hObject.QuadPart);
          }
          FUN_14005b104((void *)&local_a8.QuadPart);
          goto LAB_14009ebed;
        }
        if (local_e8 == '\0') {
          if ((int64_t)local_e0 < 0) {
            while (bVar5 = PECMD_ReadFileLine((void *)(uintptr_t)_Var27, &local_d8.QuadPart,
                                         DVar31, bVar29, &local_c0),
                   bVar5 != 0) {
              LVar19.QuadPart = LVar19.QuadPart + 1;
            }
            if (((uint8_t)local_res20 == '\x02') &&
                (DVar32 = GetLastError(), DVar32 == 0x6d)) {
              DVar20 = 0x6d;
            }
            else if (LVar19.QuadPart == 0) {
              DVar20 = 0x26;
            }
            DVar31 = 0x4b0;
            PECMD_GrowByteBuffer((void **)&local_d8, 0x20);
            wsprintfW((LPWSTR)(uintptr_t)local_d8.QuadPart, WSTR("%d"));
            DVar32 = DVar20;
          }
          else if (local_e0 == 0) {
            do {
              LVar19 = local_a8;
              local_a8 = LVar10;
              bVar5 = PECMD_ReadFileLine((void *)(uintptr_t)_Var27, &local_a8.QuadPart, DVar31,
                                    (uint8_t)(bVar29 | 1), &local_c0);
              LVar10 = LVar19;
            } while (bVar5 != 0);
            local_d8 = LVar19;
            DVar32 = 0;
            if (cVar4 == '\x02') {
              DVar6 = GetLastError();
              DVar32 = DVar20;
LAB_14009eeae:
              if (DVar6 == 0x6d) {
                DVar32 = 0x6d;
              }
            }
          }
          else {
            DVar32 = 0;
            if (1 < (int64_t)local_e0) {
              lVar9 = 1;
              do {
                bVar5 = PECMD_ReadFileLine((void *)(uintptr_t)local_88, &local_d8.QuadPart,
                                      DVar31, bVar29, &local_c0);
                if (bVar5 == 0) {
                  if (((uint8_t)local_res20 == '\x02') &&
                      (DVar32 = GetLastError(), DVar32 == 0x6d)) {
                    LVar10 = local_d8;
                    DVar32 = 0x6d;
                  }
                  else {
                    LVar10 = local_d8;
                    DVar32 = 0;
                    if (lVar9 < 2) {
                      DVar32 = 0x26;
                    }
                  }
                  break;
                }
                lVar9 = lVar9 + 1;
                LVar10 = local_d8;
                DVar32 = 0;
              } while (lVar9 < (int64_t)local_e0);
            }
            *(uint8_t *)(uintptr_t)LVar10.QuadPart = 0;
            if (local_98.QuadPart != 1) {
              FUN_140063620((WCHAR **)&local_e0);
              local_a0 = (uint64_t)(int64_t)(int)local_98.LowPart;
              local_c8 = ((uint64_t)(uint32_t)(local_c8 >> 32) << 32) | 1;
              while (1) {
                DVar6 = DVar32;
                if ((local_a0 != 0) &&
                    (local_98.QuadPart = local_98.QuadPart + -1, local_98.QuadPart < 0))
                  goto LAB_14009efbe;
                *(WCHAR *)(uintptr_t)local_e0 = L'\0';
                bVar5 = PECMD_ReadFileLine((void *)(uintptr_t)local_88, (int64_t *)&local_e0,
                                      DVar31, (uint8_t)(bVar29 | 1), &local_c0);
                if (bVar5 == 0) break;
                pWVar17 = PECMD_ConvertStringEncoding((void *)&local_e0, DVar31);
                local_c8 = local_c8 & 0xffffffff00000000ULL;
                FUN_14006375c((WCHAR **)&local_d8.QuadPart, pWVar17);
                if ((local_a0 != 0) && (local_98.QuadPart < 1)) goto LAB_14009efbe;
                if (local_bc == 0) {
                  FUN_14006375c((WCHAR **)&local_d8.QuadPart, WSTR("\r\n"));
                }
              }
              if (((uint8_t)local_res20 == '\x02') &&
                  (DVar32 = GetLastError(), DVar32 == 0x6d)) {
                DVar6 = 0x6d;
              }
              else if ((uint32_t)local_c8 != 0) {
                DVar6 = 0x26;
              }
LAB_14009efbe:
              p_Var11 = (void *)&local_e0;
              goto LAB_14009efc9;
            }
            bVar5 = PECMD_ReadFileLine((void *)(uintptr_t)local_88, &local_d8.QuadPart, DVar31,
                                  (uint8_t)(bVar29 | 1), &local_c0);
            if ((bVar5 == 0) && ((uint8_t)local_res20 == '\x02')) {
              DVar6 = GetLastError();
              goto LAB_14009eeae;
            }
          }
        }
        else {
          pCVar21 = (char *)g_DefEnv;
          if (local_68 != (char *)0) {
            pCVar21 = local_68;
          }
          FUN_1400702f0((int64_t *)&local_a0, pCVar21, 0xffffffffffffffffULL);
          FUN_140063620((WCHAR **)&local_e0);
          while (1) {
            bVar5 = PECMD_ReadFileLine((void *)(uintptr_t)_Var27, (int64_t *)&local_e0,
                                  DVar31, (uint8_t)((bVar29 & 0xf3) | 1), &local_c0);
            iVar13 = (int)bVar5;
            if (((iVar13 == 0) && ((uint8_t)local_res20 == '\x02')) &&
                (DVar32 = GetLastError(), DVar32 == 0x6d)) {
              DVar6 = 0x6d;
              goto LAB_14009ecd9;
            }
            pWVar17 = PECMD_ConvertStringEncoding((void *)&local_e0, DVar31);
            LVar19.QuadPart = LVar19.QuadPart + iVar13;
            FUN_14006375c((WCHAR **)&local_d8.QuadPart, pWVar17);
            *(WCHAR *)(uintptr_t)local_e0 = L'\0';
            if (iVar13 == 0) break;
            FUN_14006375c((WCHAR **)&local_d8.QuadPart, (LPCWSTR)(uintptr_t)local_a0);
            _Var27 = local_88;
          }
          if (LVar19.QuadPart == 0) {
            DVar6 = 0x26;
          }
LAB_14009ecd9:
          FUN_14005b104((void *)&local_e0);
          p_Var11 = (void *)&local_a0;
LAB_14009efc9:
          DVar31 = 0x4b0;
          FUN_14005b104(p_Var11);
          DVar32 = DVar6;
        }
        LVar10.QuadPart = (int64_t)(uintptr_t)PECMD_ConvertStringEncoding((void *)&local_d8.QuadPart, DVar31);
      }
      if ((hObject.QuadPart != 0) && (hObject.QuadPart != -1)) {
        CloseHandle((HANDLE)hObject.QuadPart);
      }
      FUN_14005b104((void *)&local_a8.QuadPart);
    }
    FUN_14005b104((void *)&local_80);
  }
LAB_14009f00c:
  PECMD_SetVariable(param_1, local_b8, (LPCWSTR)(uintptr_t)LVar10.QuadPart);
  LVar19.QuadPart = (int64_t)(int)DVar32;
LAB_14009f024:
  FUN_14005b104((void *)&local_d8.QuadPart);
  FUN_14005b104((void *)&local_60);
  FUN_14005b104((void *)&local_b8);
  FUN_14005b104((void *)&local_90);
  return (LARGE_INTEGER){ .QuadPart = LVar19.QuadPart };
}


/* ==========================================================================
 * @0x14009f070  (LARGE_INTEGER PECMD_WriteFileEncoded(longlong*, LARGE_INTEGER))
 * 写文件: 编码 codepage 判定、*fix *v *fv *c *sparse *nobom 修饰、文件定位/
 * 追加/截断、BOM 写入。local_res20 为字节标志寄存器 (bWriteMode 等)。
 * ========================================================================== */
LARGE_INTEGER PECMD_WriteFileEncoded(int64_t *param_1, LARGE_INTEGER param_2)
{
    bool bVar1;
    bool bVar2;
    bool bVar3;
    bool bVar4;
    bool bVar5;
    bool bVar8;
    LARGE_INTEGER LVar6;
    char cVar7;
    WCHAR WVar9;
    int iVar10;
    uint32_t uVar11;
    DWORD DVar12;
    DWORD DVar13;
    int64_t *plVar14;
    LPCWSTR pWVar15;
    uint64_t uVar16;
    int64_t lVar17;
    uint64_t *puVar18;
    uint16_t *puVar19;
    WCHAR *pWVar20;
    LARGE_INTEGER LVar21;
    uint32_t nNumberOfBytesToRead;
    uint8_t *puVar22;
    DWORD DVar23;
    LARGE_INTEGER LVar24;
    LARGE_INTEGER LVar25;
    uint16_t uVar26;
    uint16_t uVar27;
    int64_t puVar28;
    char cVar29;
    LARGE_INTEGER LVar30;
    LARGE_INTEGER LVar31;
    int iVar32;
    uint8_t bVar33;
    LARGE_INTEGER LVar34;
    int64_t *local_res8;
    LARGE_INTEGER local_res10;
    uint8_t local_res20;
    uint8_t bWriteMode;
    char local_c8;
    LARGE_INTEGER local_c0;
    LARGE_INTEGER local_b8;
    char local_b0;
    char local_af;
    char local_ae;
    uint16_t local_ad;
    LARGE_INTEGER local_a8;
    DWORD local_a0[2];
    uint16_t *local_98;
    int16_t *local_90;
    LARGE_INTEGER local_88;
    LARGE_INTEGER local_80;
    LPCWSTR local_78;
    int64_t local_70;
    uint64_t local_68;
    int64_t local_60;
    int64_t local_58;
    int64_t *local_50;
    uint64_t local_48;

    local_res8 = param_1;
    local_res10 = param_2;
    FUN_140063620((WCHAR **)&local_58);
    LVar30.QuadPart = 0;
    local_48 = 0;
    cVar29 = '\0';
    bVar8 = false;
    local_50 = param_1;
    (void)local_48;
    (void)local_50;
    uVar11 = 0;
    if (((((uint16_t)*(const uint16_t *)(uintptr_t)param_2.QuadPart < 9) ||
          (0xd < (uint16_t)*(const uint16_t *)(uintptr_t)param_2.QuadPart)) &&
         ((int16_t)*(const int16_t *)(uintptr_t)param_2.QuadPart != 0x20)) &&
        (lVar17 = FUN_14005c72c("-UNI", (const WCHAR *)(uintptr_t)param_2.QuadPart, 4),
         uVar11 = 0x4b0, (char)lVar17 == '\0')) {
        iVar32 = 5;
        lVar17 = FUN_14005c72c("-UNIB", (const WCHAR *)(uintptr_t)param_2.QuadPart, 5);
        if ((char)lVar17 == '\0') {
            lVar17 = FUN_14005c72c("-UNICODE", (const WCHAR *)(uintptr_t)param_2.QuadPart, 8);
            if ((char)lVar17 == '\0') {
                lVar17 = FUN_14005c72c("-UNICODEB", (const WCHAR *)(uintptr_t)param_2.QuadPart, 9);
                if ((char)lVar17 != '\0') goto LAB_14009f125;
                lVar17 = FUN_14005c72c("-UTF8", (const WCHAR *)(uintptr_t)param_2.QuadPart, iVar32);
                if ((char)lVar17 == '\0') {
                    lVar17 = FUN_14005c72c("-UTF7", (const WCHAR *)(uintptr_t)param_2.QuadPart, iVar32);
                    if ((char)lVar17 == '\0') {
                        lVar17 = FUN_14005c72c("-GBK", (const WCHAR *)(uintptr_t)param_2.QuadPart, 4);
                        if ((char)lVar17 == '\0') {
                            lVar17 = FUN_14005c72c("-BIG5", (const WCHAR *)(uintptr_t)param_2.QuadPart, iVar32);
                            if ((char)lVar17 == '\0') {
                                lVar17 = FUN_14005c72c("-ANSI", (const WCHAR *)(uintptr_t)param_2.QuadPart, iVar32);
                                uVar11 = 0;
                                if ((char)lVar17 == '\0') {
                                    if (((int16_t)*(const int16_t *)(uintptr_t)param_2.QuadPart == 0x2d) &&
                                        ((param_2.QuadPart = param_2.QuadPart + 2),
                                         (uint16_t)((int16_t)*(const int16_t *)(uintptr_t)param_2.QuadPart - 0x30u) < 10)) {
                                        local_res10.QuadPart = param_2.QuadPart;
                                        lVar17 = FUN_140064a88((const WCHAR *)(uintptr_t)param_2.QuadPart);
                                        uVar11 = (uint32_t)lVar17;
                                        if (-1 < (int)(uint32_t)lVar17) goto LAB_14009f25f;
                                    }
                                    LVar31.QuadPart = 1;
                                    goto LAB_1400a037e;
                                }
                            } else {
                                uVar11 = 0x3b6;
                            }
                        } else {
                            uVar11 = 0x3a8;
                        }
                    } else {
                        uVar11 = 65000;
                    }
                } else {
                    uVar11 = 0xfde9;
                }
            }
        } else {
LAB_14009f125:
            uVar11 = 0x4b1;
        }
    }
LAB_14009f25f:
    while (((uVar26 = (uint16_t)*(const uint16_t *)(uintptr_t)param_2.QuadPart),
            uVar26 != 0 && ((uVar26 < 9) || (0xd < uVar26))) && (uVar26 != 0x20)) {
        param_2.QuadPart = param_2.QuadPart + 2;
        local_res10.QuadPart = param_2.QuadPart;
    }
    FUN_14005b154((WCHAR **)&local_res10.QuadPart);
    local_res20 = 0;
    bVar1 = false;
    bVar2 = false;
    bVar3 = false;
    bVar4 = false;
    bVar5 = false;
    iVar32 = 1;
    LVar31.QuadPart = 1;
    while ((cVar7 = '\x01'), (uint16_t)*(const uint16_t *)(uintptr_t)local_res10.QuadPart == 0x2a) {
        cVar7 = FUN_1400660ac("*fix", (WCHAR **)&local_res10.QuadPart, 4);
        if (cVar7 == '\0') {
            cVar7 = FUN_1400660ac("*-nl", (WCHAR **)&local_res10.QuadPart, 4);
            if (cVar7 == '\0') {
                cVar7 = FUN_1400660ac("*v", (WCHAR **)&local_res10.QuadPart, 2);
                if (cVar7 == '\0') {
                    cVar7 = FUN_1400660ac("*fv", (WCHAR **)&local_res10.QuadPart, 3);
                    if (cVar7 == '\0') {
                        cVar7 = FUN_1400660ac("*c", (WCHAR **)&local_res10.QuadPart, 2);
                        if (cVar7 == '\0') {
                            cVar7 = FUN_1400660ac("*sparse", (WCHAR **)&local_res10.QuadPart, 7);
                            if (cVar7 == '\0') {
                                cVar7 = FUN_1400660ac("*nobom", (WCHAR **)&local_res10.QuadPart, 6);
                                if (cVar7 == '\0') {
                                    uVar26 = (uint16_t)*(const uint16_t *)(uintptr_t)local_res10.QuadPart;
                                    while ((uVar26 != 0 && (((uVar26 < 9) || (0xd < uVar26)) &&
                                           (uVar26 != 0x20)))) {
                                        local_res10.QuadPart = local_res10.QuadPart + 2;
                                        uVar26 = (uint16_t)*(const uint16_t *)(uintptr_t)local_res10.QuadPart;
                                    }
                                    FUN_14005b154((WCHAR **)&local_res10.QuadPart);
                                } else {
                                    bVar5 = true;
                                }
                            } else {
                                bVar8 = true;
                            }
                        } else {
                            bVar4 = true;
                        }
                    } else {
                        bVar1 = true;
                    }
                } else {
                    bVar3 = true;
                }
            } else {
                bVar2 = true;
            }
        } else {
            local_res20 = 2;
        }
    }
    pWVar20 = (WCHAR *)(uintptr_t)g_szEmpty;
    PECMD_StrDupAssign((WCHAR **)&local_90, (LPCWSTR)(uintptr_t)g_szEmpty);
    PECMD_StrDupAssign((WCHAR **)&local_98, WSTR("+0"));
    local_88.QuadPart = 0;
    local_a8.QuadPart = 0;
    plVar14 = PECMD_SplitTokenAssignVar((WCHAR **)&local_58, (WCHAR **)&local_res10.QuadPart, 0x2c, 1);
    PECMD_SplitTokenTrimWs(plVar14, (int64_t *)&local_90, 0);
    pWVar15 = FUN_14001be14((WCHAR *)local_90);
    local_78 = pWVar15;
    if (*pWVar15 == L'\0') {
        FUN_14005b104((void *)&local_98);
        FUN_14005b104((void *)&local_90);
        LVar31.QuadPart = -0x7ff8ffa9;
        goto LAB_1400a037e;
    }
    uVar26 = 0x2c;
    uVar27 = 0x2c;
    if ((uint16_t)*(const uint16_t *)(uintptr_t)local_res10.QuadPart == 0x2c) {
        local_res10.QuadPart = local_res10.QuadPart + 2;
        FUN_14005b154((WCHAR **)&local_res10.QuadPart);
        if ((uint16_t)*(const uint16_t *)(uintptr_t)local_res10.QuadPart != uVar26) {
            plVar14 = PECMD_SplitTokenAssignVar((WCHAR **)&local_58, (WCHAR **)&local_res10.QuadPart, uVar26, 1);
            PECMD_SplitTokenTrimWs(plVar14, (int64_t *)&local_98, 0);
            uVar27 = 0x2c;
            uVar26 = 0x2c;
            if ((uint16_t)*(const uint16_t *)(uintptr_t)local_res10.QuadPart != 0x2c) goto LAB_14009f52b;
        }
        uVar27 = uVar26;
        pWVar20 = (WCHAR *)(uintptr_t)(local_res10.QuadPart + 2);
    }
LAB_14009f52b:
    local_c8 = '\0';
    local_res10.QuadPart = (int64_t)(uintptr_t)local_98;
    if (*local_98 == 0x24) {
        cVar29 = (char)*local_98;
        local_res10.QuadPart = (int64_t)(uintptr_t)(local_98 + 1);
    }
    FUN_14005b154((WCHAR **)&local_res10.QuadPart);
    if (((uint16_t)*(const uint16_t *)(uintptr_t)local_res10.QuadPart == 0x2b) ||
        ((uint16_t)*(const uint16_t *)(uintptr_t)local_res10.QuadPart == 0x2d)) {
        local_c8 = *(const char *)(uintptr_t)local_res10.QuadPart;
        local_res10.QuadPart = local_res10.QuadPart + 2;
    }
    FUN_14005b154((WCHAR **)&local_res10.QuadPart);
    LVar25.QuadPart = LVar30.QuadPart;
    if ((uint16_t)((uint16_t)*(const uint16_t *)(uintptr_t)local_res10.QuadPart - 0x30) < 10) {
        PECMD_ParseSignedNumberStr((void **)&local_res10, (void *)&local_a8.QuadPart, (short)uVar27);
        local_88.QuadPart = local_a8.QuadPart;
        LVar25 = local_a8;
        if (-1 < local_a8.QuadPart) goto LAB_14009f5ef;
    } else {
LAB_14009f5ef:
        FUN_140063620((WCHAR **)&local_b8);
        plVar14 = local_res8;
        if (bVar1) {
            if ((*pWVar20 == L'&') || ((*(uint8_t *)((int64_t)local_res8 + 0xd) & 0xf) != 0)) {
                EnterCriticalSection((LPCRITICAL_SECTION)&g_csInit);
                puVar18 = (uint64_t *)PECMD_FindVarValue(plVar14, pWVar20, (int64_t *)0, -1);
                if (puVar18 != (uint64_t *)0) {
                    FUN_1400703e4(&local_b8.QuadPart, (LPCWSTR)*puVar18);
                }
                LeaveCriticalSection((LPCRITICAL_SECTION)&g_csInit);
            } else {
                FUN_14006f884(pWVar20, (void *)&local_b8);
            }
        } else if (cVar29 == '\0') {
            PECMD_AssignString(&local_b8.QuadPart, pWVar20);
        } else {
            FUN_14007bf44(local_res8, pWVar20, &local_b8.QuadPart, 0, 1);
        }
        local_res10.QuadPart = local_res10.QuadPart & 0xffffffffffffff00;
        bWriteMode = 0;
        local_c0.QuadPart = 0;
        uVar16 = (uint64_t)(int64_t)PECMD_AsciiWideICmp("--", pWVar15);
        if ((char)uVar16 == '\0') {
            uVar16 = (uint64_t)(int64_t)PECMD_AsciiWideICmp("-", pWVar15);
            LVar34.QuadPart = LVar30.QuadPart;
            cVar29 = '\0';
            if ((char)uVar16 != '\0') {
                bWriteMode = 1;
                LVar34.QuadPart = (int64_t)g_hStdErr;
                cVar29 = cVar7;
            }
        } else {
            bWriteMode = 2;
            LVar34.QuadPart = (int64_t)g_hStdOut;
            cVar29 = '\x02';
        }
        if ((!bVar2) && (!bVar3)) {
            FUN_14006375c((WCHAR **)&local_b8.QuadPart, WSTR("\r\n"));
        }
        pWVar20 = (WCHAR *)local_78;
        LVar21.QuadPart = LVar30.QuadPart;
        LVar24.QuadPart = LVar30.QuadPart;
        if (bVar3) {
            uVar11 = 0x4b0;
LAB_14009fa02:
            iVar10 = lstrlenW((LPCWSTR)(uintptr_t)local_b8.QuadPart);
            LVar6 = local_b8;
            lVar17 = (int64_t)iVar10 * 2;
            if (uVar11 != 0x4b0) {
                if (uVar11 == 0x4b1) {
                    FUN_140060a74((uint8_t *)(uintptr_t)local_b8.QuadPart, (int)lVar17);
                } else if (uVar11 == 0xfde9) {
                    lVar17 = PECMD_WideToAnsiConvert((void *)&local_b8.QuadPart);
                } else {
                    local_80 = local_b8;
                    local_b8.QuadPart = 0;
                    PECMD_WideToAnsiStr(&local_b8.QuadPart, (LPCWSTR)(uintptr_t)LVar6.QuadPart, -1,
                                  (uint64_t)(uVar11 & 0xffff));
                    iVar10 = lstrlenA((LPCSTR)(uintptr_t)local_b8.QuadPart);
                    lVar17 = (int64_t)iVar10;
                    FUN_14005b104((void *)&local_80.QuadPart);
                }
            }
            local_a8 = local_b8;
            DVar13 = (DWORD)lVar17;
            if (bWriteMode == 0) {
                FUN_140063620((WCHAR **)&local_80);
                FUN_140063620((WCHAR **)&local_70);
                PECMD_AllocSmallObject((void *)&local_60);
                PECMD_AllocSmallObject((void *)local_a0);
                bVar33 = local_res20;
                if (bVar3) {
                    FUN_140063620((WCHAR **)&local_res20);
                    pWVar20 = (WCHAR *)local_78;
                    if ((*local_78 == L'&') || ((*(uint8_t *)((int64_t)local_res8 + 0xd) & 0xf) != 0)) {
                        EnterCriticalSection((LPCRITICAL_SECTION)&g_csInit);
                        puVar18 = (uint64_t *)PECMD_FindVarValue(local_res8, pWVar20, (int64_t *)0, -1);
                        if (puVar18 != (uint64_t *)0) {
                            FUN_1400703e4((int64_t *)&local_res20, (LPCWSTR)*puVar18);
                        }
                        LeaveCriticalSection((LPCRITICAL_SECTION)&g_csInit);
                    } else {
                        FUN_14006f884(local_78, (void *)&local_res20);
                    }
                    pWVar20 = (WCHAR *)*(WCHAR **)(void *)&local_res20;
                    uVar11 = lstrlenW(pWVar20);
                    local_res10.QuadPart = (int64_t)(int32_t)uVar11;
                    local_c0.QuadPart = 0;
                    local_a8.QuadPart = 0;
                    if (local_c8 == '-') {
                        local_88.QuadPart = 0;
                    } else {
                        local_88.QuadPart = lVar17 / 2;
                    }
                    LVar31.QuadPart = LVar30.QuadPart;
                    if (LVar25.QuadPart == 0) {
                        LVar25 = local_res10;
                        LVar34.QuadPart = LVar30.QuadPart;
                        if (local_c8 == '+') goto LAB_14009fe37;
                        if (local_res10.QuadPart != 0) {
                            LVar24 = local_res10;
                            if (pWVar20[(int64_t)(local_res10.QuadPart + -1)] == L'\n') {
                                LVar24.QuadPart = local_res10.QuadPart + -1;
                                LVar25.QuadPart = LVar24.QuadPart;
                                if (LVar24.QuadPart == 0) goto LAB_14009fcf1;
                                if (pWVar20[(int64_t)(local_res10.QuadPart + -2)] == L'\r') {
                                    LVar24.QuadPart = local_res10.QuadPart + -2;
                                }
                            }
                            LVar25 = LVar24;
                            if (LVar24.QuadPart != 0) {
                                for (pWVar15 = pWVar20 + (int64_t)(LVar24.QuadPart + -1);
                                     (LVar25 = LVar24), pWVar20 <= pWVar15;
                                     pWVar15 = pWVar15 + -1) {
                                    if (*pWVar15 == L'\n') {
                                        if (local_c8 == '-') {
                                            LVar25.QuadPart = LVar24.QuadPart + -1;
                                            if ((pWVar20 <= pWVar15 + -1) && (pWVar15[-1] == L'\r')) {
                                                LVar25.QuadPart = LVar24.QuadPart + -2;
                                            }
                                        }
                                        break;
                                    }
                                    LVar24.QuadPart = LVar24.QuadPart + -1;
                                }
                            }
                        }
LAB_14009fcf1:
                        pWVar20[LVar25.QuadPart] = L'\0';
                        if (local_c8 != '-') goto LAB_14009fe41;
                    } else {
                        LVar34.HighPart = 0;
                        LVar34.LowPart = uVar11;
                        if (((LVar25.QuadPart != 1) ||
                             (uVar11 = 0, LVar34.QuadPart = LVar30.QuadPart, local_c8 != '+'))){
                            LVar31 = local_res10;
                        }
                        if (((((local_c8 != '-') || (LVar25.QuadPart != 1)) &&
                              (iVar10 = 0, iVar32 = 0, LVar25.QuadPart == 1)) &&
                             ((local_c8 == '+' && (iVar32 = iVar10, local_88.QuadPart != 0)))) &&
                            (local_a8.QuadPart = LVar30.QuadPart,
                             (uint16_t *)(uintptr_t)(local_b8.QuadPart + -2))[local_88.QuadPart] != 10) {
                            local_a8.QuadPart = 2;
                        }
                        puVar28 = (int64_t)((local_c8 == '+') + 1);
                        pWVar15 = pWVar20;
                        while (puVar28 <= LVar25.QuadPart) {
                            uVar11 = LVar34.LowPart;
                            WVar9 = *pWVar15;
                            if (WVar9 == L'\0') break;
                            do {
                                if (WVar9 == L'\n') {
                                    if (puVar28 == LVar25.QuadPart) {
                                        LVar31.QuadPart =
                                             (((int64_t)(uintptr_t)pWVar15 - (int64_t)(uintptr_t)pWVar20) >> 1) + (int64_t)iVar32;
                                    }
                                    if (LVar25.QuadPart == 2) {
                                        LVar34.QuadPart =
                                             (((int64_t)(uintptr_t)pWVar15 - (int64_t)(uintptr_t)pWVar20) >> 1) & 0xffffffff;
                                    }
LAB_14009fe0a:
                                    LVar25.QuadPart = LVar25.QuadPart + -1;
                                    pWVar15 = pWVar15 + 1;
                                    break;
                                }
                                if ((WVar9 == L'\r') &&
                                    (pWVar15 = pWVar15 + 1, *pWVar15 == L'\n')) {
                                    if (puVar28 == LVar25.QuadPart) {
                                        LVar31.QuadPart =
                                             (((int64_t)(uintptr_t)pWVar15 - (int64_t)(uintptr_t)pWVar20) >> 1) + -1 + (int64_t)(iVar32 * 2);
                                    }
                                    if (LVar25.QuadPart == 2) {
                                        LVar34.LowPart = (DWORD)((int)(((int64_t)(uintptr_t)pWVar15 - (int64_t)(uintptr_t)pWVar20) >> 1) - 1);
                                        LVar34.HighPart = 0;
                                    }
                                    goto LAB_14009fe0a;
                                }
                                pWVar15 = pWVar15 + 1;
                                WVar9 = *pWVar15;
                            } while (WVar9 != L'\0');
                            uVar11 = LVar34.LowPart;
                        }
                        LVar25.QuadPart = (int64_t)(int32_t)uVar11;
                        LVar34.QuadPart = local_res10.QuadPart - LVar31.QuadPart;
LAB_14009fe37:
                        if (local_c8 != '-') {
LAB_14009fe41:
                            if ((LVar25.QuadPart != 0) &&
                                (local_c0.QuadPart = LVar30.QuadPart,
                                 pWVar20[(int64_t)(LVar25.QuadPart + -1)] != L'\n')) {
                                local_c0.QuadPart = 2;
                            }
                        }
                        puVar22 = (uint8_t *)(uintptr_t)(local_88.QuadPart + local_a8.QuadPart) + local_c0.QuadPart;
                        LVar24.QuadPart =
                             (int64_t)(uintptr_t)puVar22 +
                             (int64_t)(uintptr_t)((uint8_t *)(uintptr_t)(LVar34.QuadPart + 1) + LVar25.QuadPart);
                        lVar17 = (int64_t)(uintptr_t)*(WCHAR **)(void *)&local_res20 + LVar25.QuadPart * 2;
                        if ((((uint64_t)LVar24.QuadPart <= (uint64_t)local_res10.QuadPart) &&
                             (LVar34.QuadPart != 0))){
                            memmove((uint8_t *)(uintptr_t)(lVar17 + ((int64_t)(uintptr_t)puVar22) * 2),
                                          (void *)(uintptr_t)(lVar17 + (LVar31.QuadPart - LVar25.QuadPart) * 2),
                                          LVar34.LowPart * 2);
                        }
                        PECMD_AllocString((WCHAR **)&local_res20, LVar24.QuadPart);
                        puVar19 = (uint16_t *)(uintptr_t)((int64_t)(uintptr_t)*(WCHAR **)(void *)&local_res20 + LVar25.QuadPart * 2);
                        if ((((uint64_t)local_res10.QuadPart < (uint64_t)LVar24.QuadPart) &&
                             (LVar34.QuadPart != 0))){
                            memmove((uint8_t *)(uintptr_t)((int64_t)(uintptr_t)puVar19 + (int64_t)(uintptr_t)puVar22),
                                          (void *)(uintptr_t)((int64_t)(uintptr_t)puVar19 + (LVar31.QuadPart - LVar25.QuadPart)),
                                          LVar34.LowPart * 2);
                        }
                        LVar31 = local_88;
                        if (local_c0.QuadPart != 0) {
                            *puVar19 = 0xd;
                            puVar19[1] = 10;
                            puVar19 = puVar19 + 2;
                        }
                        if (local_88.QuadPart != 0) {
                            memcpy((void *)puVar19, (const void *)(uintptr_t)local_b8.QuadPart,
                                          local_88.LowPart * 2);
                            puVar19 = puVar19 + (int64_t)LVar31.QuadPart;
                        }
                        if (local_a8.QuadPart != 0) {
                            *puVar19 = 0xd;
                            puVar19[1] = 10;
                            puVar19 = puVar19 + 2;
                        }
                        puVar19[LVar34.QuadPart] = 0;
                    }
                    PECMD_SetVariable(local_res8, local_78, (LPCWSTR)*(WCHAR **)(void *)&local_res20);
                    FUN_14005b104((void *)&local_res20);
                } else {
                    if (LVar25.QuadPart == 0) {
                        if (local_c8 == '+') {
                            PECMD_SetFilePointer((HANDLE)LVar21.QuadPart, (LARGE_INTEGER){ .QuadPart = 0 }, 2);
                            bVar33 = local_res20;
                        } else {
                            PECMD_SetFilePointer((HANDLE)LVar21.QuadPart, LVar24, 0);
                            LVar31 = PECMD_SetFilePointer((HANDLE)LVar21.QuadPart, (LARGE_INTEGER){ .QuadPart = 0 }, 1);
                            bVar8 = PECMD_ReadFileLine((void *)&local_c0, (int64_t *)local_a0, uVar11, local_res20, (uint32_t *)0);
                            bVar33 = local_res20;
                            iVar32 = (int)bVar8;
                            while ((LVar34 = LVar31), iVar32 != 0) {
                                LVar31 = PECMD_SetFilePointer((HANDLE)LVar21.QuadPart, (LARGE_INTEGER){ .QuadPart = 0 }, 1);
                                bVar8 = PECMD_ReadFileLine((void *)&local_c0, (int64_t *)local_a0, uVar11, bVar33, (uint32_t *)0);
                                LVar25 = local_88;
                                LVar24 = LVar34;
                                iVar32 = (int)bVar8;
                            }
                            PECMD_SetFilePointer((HANDLE)LVar21.QuadPart, LVar24, 0);
                            PECMD_SetEndOfFileWrap((void *)&local_c0, LVar24);
                            bVar33 = local_res20;
                        }
                    } else if (1 < LVar25.QuadPart) {
                        do {
                            bVar8 = PECMD_ReadFileLine((void *)&local_c0, (int64_t *)local_a0, uVar11, bVar33, (uint32_t *)0);
                            if ((int)bVar8 == 0) break;
                            LVar31.QuadPart = LVar31.QuadPart + 1;
                        } while (LVar31.QuadPart < LVar25.QuadPart);
                    }
                    if (local_c8 == '+') {
                        if (LVar25.QuadPart == 0) {
LAB_1400a0253:
                            WriteFile((HANDLE)LVar21.QuadPart, (LPCVOID)(uintptr_t)local_a8.QuadPart, DVar13,
                                      (DWORD *)&local_res10.QuadPart, (OVERLAPPED *)0);
                        } else {
                            LVar31 = PECMD_SetFilePointer((HANDLE)LVar21.QuadPart, (LARGE_INTEGER){ .QuadPart = 0 }, 1);
                            DVar12 = PECMD_GetFileSize((HANDLE)LVar21.QuadPart);
                            DVar23 = LVar31.LowPart;
                            DVar12 = DVar12 - DVar23;
                            nNumberOfBytesToRead = DVar12;
                            if ((int)DVar12 < 0) {
                                nNumberOfBytesToRead = 0;
                            }
                            FUN_1400633a8((void **)&local_res20, (int64_t)(int)(DVar12 + 0xca));
                            PECMD_SetFilePointer((HANDLE)LVar21.QuadPart, (LARGE_INTEGER){ .QuadPart = (int64_t)(int32_t)DVar23 }, 0);
                            local_res10.QuadPart = local_res10.QuadPart & 0xffffffff00000000;
                            ReadFile((HANDLE)LVar21.QuadPart, (LPVOID)*(WCHAR **)(void *)&local_res20,
                                     nNumberOfBytesToRead, (DWORD *)&local_res10.QuadPart, (OVERLAPPED *)0);
                            PECMD_SetFilePointer((HANDLE)LVar21.QuadPart, (LARGE_INTEGER){ .QuadPart = (int64_t)(int32_t)DVar23 }, 0);
                            WriteFile((HANDLE)LVar21.QuadPart, (LPCVOID)(uintptr_t)local_a8.QuadPart, DVar13,
                                      (DWORD *)&local_res8, (OVERLAPPED *)0);
                            if (0 < (int)nNumberOfBytesToRead) {
LAB_1400a010d:
                                WriteFile((HANDLE)LVar21.QuadPart, (LPCVOID)*(WCHAR **)(void *)&local_res20,
                                          nNumberOfBytesToRead, (DWORD *)&local_res8, (OVERLAPPED *)0);
                            }
LAB_1400a0366:
                            FUN_14005b104((void *)&local_res20);
                        }
                    } else {
                        if (local_c8 != '-') {
                            if (LVar25.QuadPart == 0) goto LAB_1400a0253;
                            LVar31 = PECMD_SetFilePointer((HANDLE)LVar21.QuadPart, (LARGE_INTEGER){ .QuadPart = 0 }, 1);
                            PECMD_ReadFileLine((void *)&local_c0, (int64_t *)local_a0, uVar11, bVar33, (uint32_t *)0);
                            LVar25 = PECMD_SetFilePointer((HANDLE)LVar21.QuadPart, (LARGE_INTEGER){ .QuadPart = 0 }, 1);
                            DVar12 = PECMD_GetFileSize((HANDLE)LVar21.QuadPart);
                            iVar32 = (int)(DVar12 - LVar25.LowPart);
                            FUN_1400633a8((void **)&local_res20, (int64_t)(iVar32 + 0xca));
                            local_res10.QuadPart = local_res10.QuadPart & 0xffffffff00000000;
                            ReadFile((HANDLE)LVar21.QuadPart, (LPVOID)*(WCHAR **)(void *)&local_res20, iVar32 + 200,
                                     (DWORD *)&local_res10.QuadPart, (OVERLAPPED *)0);
                            PECMD_SetFilePointer((HANDLE)LVar21.QuadPart, LVar31, 0);
                            WriteFile((HANDLE)LVar21.QuadPart, (LPCVOID)(uintptr_t)local_a8.QuadPart, DVar13,
                                      (DWORD *)&local_res8, (OVERLAPPED *)0);
                            WriteFile((HANDLE)LVar21.QuadPart, (LPCVOID)*(WCHAR **)(void *)&local_res20,
                                      local_res10.LowPart, (DWORD *)&local_res8, (OVERLAPPED *)0);
                            LVar31 = PECMD_SetFilePointer((HANDLE)LVar21.QuadPart, (LARGE_INTEGER){ .QuadPart = 0 }, 1);
                            PECMD_SetEndOfFileWrap((void *)&local_c0, (LARGE_INTEGER){ .QuadPart = (int64_t)(int32_t)LVar31.LowPart });
                            goto LAB_1400a0366;
                        }
                        if (LVar25.QuadPart != 0) {
                            LVar31 = PECMD_SetFilePointer((HANDLE)LVar21.QuadPart, (LARGE_INTEGER){ .QuadPart = 0 }, 1);
                            PECMD_ReadFileLine((void *)&local_c0, (int64_t *)local_a0, uVar11, bVar33, (uint32_t *)0);
                            LVar25 = PECMD_SetFilePointer((HANDLE)LVar21.QuadPart, (LARGE_INTEGER){ .QuadPart = 0 }, 1);
                            DVar13 = PECMD_GetFileSize((HANDLE)LVar21.QuadPart);
                            iVar32 = (int)(DVar13 - LVar25.LowPart);
                            FUN_1400633a8((void **)&local_res20, (int64_t)(iVar32 + 0xca));
                            local_res10.QuadPart = local_res10.QuadPart & 0xffffffff00000000;
                            ReadFile((HANDLE)LVar21.QuadPart, (LPVOID)*(WCHAR **)(void *)&local_res20, iVar32 + 200,
                                     (DWORD *)&local_res10.QuadPart, (OVERLAPPED *)0);
                            PECMD_SetFilePointer((HANDLE)LVar21.QuadPart, LVar31, 0);
                            PECMD_SetEndOfFileWrap((void *)&local_c0, LVar31);
                            nNumberOfBytesToRead = local_res10.LowPart;
                            goto LAB_1400a010d;
                        }
                    }
                }
                FUN_14005b104((void *)local_a0);
                FUN_14005b104((void *)&local_60);
                FUN_14005b104((void *)&local_70);
                FUN_14005b104((void *)&local_80.QuadPart);
            } else {
                if (LVar34.QuadPart == -1) {
                    if (((LVar21.QuadPart != 0) && (LVar21.QuadPart != -1))) {
                        CloseHandle((HANDLE)LVar21.QuadPart);
                    }
                    FUN_14005b104((void *)&local_b8.QuadPart);
                    FUN_14005b104((void *)&local_98);
                    FUN_14005b104((void *)&local_90);
                    goto LAB_1400a037e;
                }
                local_res10.QuadPart = local_res10.QuadPart & 0xffffffff00000000;
                WriteFile((HANDLE)LVar34.QuadPart, (LPCVOID)(uintptr_t)local_b8.QuadPart, DVar13,
                          (DWORD *)&local_res10.QuadPart, (OVERLAPPED *)0);
                FlushFileBuffers((HANDLE)LVar34.QuadPart);
                FlushFileBuffers((HANDLE)LVar34.QuadPart);
                FlushFileBuffers((HANDLE)LVar34.QuadPart);
                FlushFileBuffers((HANDLE)LVar34.QuadPart);
            }
            if (((LVar21.QuadPart != 0) && (LVar21.QuadPart != -1))) {
                CloseHandle((HANDLE)LVar21.QuadPart);
            }
        } else {
            if (cVar29 != '\0') goto LAB_14009fa02;
            uVar16 = (uint64_t)(int64_t)PECMD_AsciiWideICmp("CONOUT$", local_78);
            if ((char)uVar16 == '\0') {
                if (bVar8) {
                    PECMD_DeviceCheckReady(pWVar20);
                }
            } else {
                PECMD_AllocConsoleBuffers(0x100);
                cVar7 = '\0';
            }
            iVar10 = FUN_140006a4c(pWVar20);
            if ((((iVar10 == 0) ||
                  (local_c0.QuadPart = PECMD_OpenFileExisting((uint64_t)(uintptr_t)pWVar20, 0xffffffffc0000000, 7),
                   local_c0.QuadPart == 0)) &&
                 (PECMD_OpenFileHandle((HANDLE *)&local_c0, pWVar20, 0xc0000000, 7, (LPSECURITY_ATTRIBUTES)0, 3, 0x80,
                                (HANDLE)0), local_c0.QuadPart == 0)) &&
                ((PECMD_OpenFileHandle((HANDLE *)&local_c0, pWVar20, 0xc0000000, 7, (LPSECURITY_ATTRIBUTES)0, 3, 0x2000080,
                                (HANDLE)0), local_c0.QuadPart == 0 &&
                 (PECMD_OpenFileHandle((HANDLE *)&local_c0, pWVar20, 0xc0000000, 7, (LPSECURITY_ATTRIBUTES)0, 4, 0x80,
                                (HANDLE)0), local_c0.QuadPart == 0)))) {
                PECMD_OpenFileHandle((HANDLE *)&local_c0, pWVar20, 0xc0000000, 7, (LPSECURITY_ATTRIBUTES)0, 4, 0x2000080,
                              (HANDLE)0);
            }
            LVar21 = local_c0;
            if (local_c0.QuadPart != 0) {
                local_68 = 0;
                local_b0 = '\0';
                local_af = '\0';
                local_ad = 0;
                (void)local_ad;
                local_ae = '\0';
                if (cVar7 != '\0') {
                    ReadFile((HANDLE)local_c0.QuadPart, &local_b0, 3, (DWORD *)&local_68, (OVERLAPPED *)0);
                }
                if (local_b0 == -1) {
                    if (local_af == -2) {
                        uVar11 = 0x4b0;
                        LVar24.QuadPart = 2;
                        goto LAB_14009f931;
                    }
                } else {
LAB_14009f931:
                    if (local_b0 == -2) {
                        if (local_af != -1) goto LAB_14009f95b;
                        uVar11 = 0x4b1;
                        LVar24.QuadPart = 2;
                    }
                    if (((local_b0 == -0x11) && (local_af == -0x45)) && (local_ae == -0x41)) {
                        uVar11 = 0xfde9;
                        LVar24.QuadPart = 3;
                    }
                }
LAB_14009f95b:
                plVar14 = (int64_t *)(uintptr_t)FUN_140063060((uint32_t *)&local_68);
                LVar34 = LVar21;
                if ((*plVar14 == 0) || (bVar4)) {
                    PECMD_SetFilePointer((HANDLE)LVar21.QuadPart, (LARGE_INTEGER){ .QuadPart = 0 }, 0);
                    if (bVar4) {
                        SetEndOfFile((HANDLE)LVar21.QuadPart);
                    }
                    if (!bVar5) {
                        if (uVar11 == 0x4b0) {
                            LVar24.QuadPart = 2;
                            puVar19 = (uint16_t *)(void *)DAT_140124128;
                        } else if (uVar11 == 0x4b1) {
                            LVar24.QuadPart = 2;
                            puVar19 = (uint16_t *)(void *)DAT_14012412c;
                        } else {
                            if (uVar11 != 0xfde9) goto LAB_14009fa02;
                            LVar24.QuadPart = 3;
                            puVar19 = (uint16_t *)(void *)DAT_140124130;
                        }
                        WriteFile((HANDLE)LVar21.QuadPart, (LPCVOID)puVar19, LVar24.LowPart, local_a0,
                                  (OVERLAPPED *)0);
                    }
                } else {
                    PECMD_SetFilePointer((HANDLE)LVar21.QuadPart, LVar24, 0);
                }
                goto LAB_14009fa02;
            }
            DVar13 = GetLastError();
            LVar30.HighPart = 0;
            LVar30.LowPart = DVar13;
        }
        FUN_14005b104((void *)&local_b8.QuadPart);
    }
    LVar31 = LVar30;
    FUN_14005b104((void *)&local_98);
    FUN_14005b104((void *)&local_90);
LAB_1400a037e:
    FUN_14005b104((void *)&local_58);
    return LVar31;
}

