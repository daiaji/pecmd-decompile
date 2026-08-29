/* ====================================================================
 * core_b2e.c — B2 批次小函数 (140020000-14003ffff) 第五批
 *
 * 本批新实现函数全部使用人类可读 PECMD_ 名称，原始地址保留在 @0x 注释。
 *
 * 来源: PECMD原始.EXE (x64, ImageBase=0x140000000)
 *   处理热键命令行        PECMD_HotkeyControl      @0x140023640
 *   匹配命令字(带尾界)    PECMD_FindSubBlockNthScan  @0x140023c48
 *   匹配命令字            PECMD_FindSubBlockNth    @0x1400240c0
 *   取下一个命令参数      PECMD_GetNextCommandArg     @0x140024350
 *   配置页面文件          PECMD_CreatePageFile    @0x14002a910
 *   扫描 INF 寻找驱动     FUN_14002B2EC    @0x14002b2ec
 *   复制 INF/SYS 到系统   FUN_14002B9EC      @0x14002b9ec
 *   解析日期时间表达式    PECMD_EvalSpecialToken  @0x14002d33c
 *   显示分辨率弹出菜单    PECMD_DispConfirmPopupMenu @0x14002e790
 *   解析命令路径          FUN_14002FD88     @0x14002fd88
 *   ImDisk 控制请求       PECMD_MountImDiskRamDisk        @0x140035cec
 *   ImDisk 移除/卸载      PECMD_DismountRamDiskDrive         @0x14003634c
 *   处理电源命令          PECMD_ShutPowerAction  @0x14003db00
 *   主 Shell 循环         PECMD_ShellLaunchThread        @0x14003e768
 *
 * 约定:
 *   - 新实现函数使用 PECMD_ 可读名；未实现依赖仍 extern FUN_ + TODO(verify)
 *   - 全局数据符号先用 extern 声明; 完整链接时统一在 core_globals.c 定义
 *   - 反编译中的 Ghidra 扩展宏 (SUB168/SEXT816/CONCATxx) 用等价整数运算替代
 * ==================================================================== */
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>

#include "pecmd_defs.h"

/* ---- 本文件需要的本地补充类型 ---- */
typedef struct {
    USHORT Length;
    USHORT MaximumLength;
    WCHAR *Buffer;
} PECMD_UNICODE_STRING;

/* ---- 已实现公共工具 (其他 core_*.c / core_globals.c) ---- */
extern void FUN_1400633A8(void **ps, int64_t len); /* @0x1400633a8 */
extern uint64_t PECMD_EnumDisplayModes(void **arr, int max, uint32_t flags,
                                       LPCWSTR adapter); /* @0x140023338 */
extern void PECMD_PatchInfDirectives(LPCWSTR path);      /* @0x140021144 */

/* ---- 未实现依赖 (extern + TODO(verify)) ---- */
/* S11: 本地声明与定义冲突已删除, 统一采用 xproto.h 原型 (原: /* S11: 本地声明与定义冲突, 已删除, 统一采用 xproto.h 原型 (原: extern void PECMD_AllocStrSlot(void *ps); * / /* S11: 本地声明与定义冲突, 已) */
/* S11: 本地声明与定义冲突已删除, 统一采用 xproto.h 原型 (原: extern void PECMD_CreateMutexSlot(void *out, LPCWSTR name);) */


extern void PECMD_ReleaseMutex(void *out);
extern void PECMD_InitRamdataRegistry(uint32_t mode);
extern uint64_t PECMD_ParseHotkeyCode(int64_t *pp, uint32_t *a, int64_t *b, char c);
extern void FUN_14007BF44(int64_t *ctx, WCHAR *name, void *out, int mode, uint8_t flag);
extern int64_t PECMD_TokPrefixICmp(char *s, uint16_t *w, int len);
extern uint64_t FUN_14005C7C4(char *s, uint16_t *w);
extern HWND PECMD_RegisterCallbackWnd(int mode);
/* S11: 本地声明与定义冲突已删除, 统一采用 xproto.h 原型 (原: /* S11: 本地声明与定义冲突, 已删除, 统一采用 xproto.h 原型 (原: extern WCHAR *PECMD_ParseIntSkipSepChar(uint64_t *out, int *src, ) */

extern void FUN_1400629B8(int64_t *script, LPCWSTR key, LPCWSTR value);
extern int64_t *FUN_1400637DC(int64_t *ps, LPCSTR src, uint64_t len, uint64_t mode);
extern void FUN_140025f10(int64_t ctx, LPCWSTR msg, uint32_t code, void *p4, void *p5, int64_t *p6);
extern void PECMD_WriteParamRecord(int64_t ctx, char type, LPCWSTR a, LPCWSTR b);
extern void *PECMD_ReadFileToBuffer(LPCWSTR path, void *a, DWORD *b);
extern int16_t *PECMD_TableLookupEntry(int64_t ctx, LPCWSTR s, int n);
extern uint64_t PECMD_MatchPattern(int16_t *s, int64_t ctx);
extern int16_t PECMD_XorStrNCaseCmp(int64_t ctx, uint16_t *s, int16_t *key, int len);
extern char FUN_1400660AC(char *s, int64_t *pp, int len);
extern uint16_t *FUN_140024C48(int64_t *pp, int64_t *len, uint32_t mode);
extern void PECMD_StrBldCopyWideN(WCHAR **pname, LPCWSTR src, int64_t len);
extern uint64_t PECMD_EvalParenExpression(int64_t *pp, uint64_t *out);
extern int64_t PECMD_GetAvailPhysMemoryMB(void);
extern int64_t PECMD_GetDiskFreeMB(LPCWSTR s);
extern int PECMD_FindSuitableDrive(LPWSTR s, int max, WCHAR *out);
extern int16_t *FUN_140103068(int16_t *dst, int16_t *src);
extern WCHAR *PECMD_SprintfRetEnd(WCHAR *dst, uint64_t v, LPCWSTR fmt);
extern void PECMD_FormatI64Dec(LPWSTR dst, int64_t value);
extern DWORD FUN_14005C4E0(HKEY root, LPCWSTR sub, LPCWSTR name, DWORD *type, BYTE *data,
                           DWORD *size);
extern DWORD PECMD_RegSetValueWithOpen(HKEY root, LPCWSTR sub, LPCWSTR name, DWORD type, BYTE *data,
                                       DWORD size);
extern DWORD FUN_14006459C(LPCWSTR path, uint32_t mode, LPWSTR buf, LPWSTR *out);
extern void PECMD_FindFirstFileW(HANDLE *ph, LPCWSTR path, WIN32_FIND_DATAW *fd);
extern int64_t PECMD_WideStrLen(void *s);
extern char PECMD_ParseEnvSwitches(LPCWSTR s, int64_t *ctx, int mode);
extern POINT PECMD_EvalQueryValue(int64_t *ctx, uint64_t p, int64_t a, char c);
extern LARGE_INTEGER PECMD_EvalAtCommand(LARGE_INTEGER a, LARGE_INTEGER b, uint16_t *c, int64_t d);
extern void PECMD_EnumCDRomDrives(int64_t *ctx);
extern void FUN_14007A224(void *ctx, WCHAR *text, WCHAR **out, int c, uint8_t d);
extern uint64_t PECMD_ExpandSpecialDirs(int64_t *ctx, LPCWSTR s, int a, int b, WCHAR *out);
extern uint64_t PECMD_EnviMemReadWrite(uint64_t a, void *b, uint64_t c, WCHAR *d, char e);
extern UINT PECMD_ScanMenuRecursive(HMENU menu, int64_t *arr, int *sel, LPCWSTR buf, int mode);
extern LPCWSTR FUN_1400169BC(int id, void **pp);
extern void *FUN_140063B00(int64_t idx, int64_t *arr, int64_t *cap, uint32_t esize);
extern int64_t *PECMD_InitPtrTable(int64_t *arr);
extern void PECMD_FreeArray_ddf8(int64_t *arr);
extern void PECMD_ProcessScriptBlock(uint64_t script, uint64_t cmd, void *p3, void *p4, void *p5);
extern void FUN_14007BDA8(void *ctx, LPCWSTR text, WCHAR **out, int c, int d);
extern uint16_t *FUN_14007443C(LPCWSTR s, int64_t *out);
extern void PECMD_GetApiProcCached(LPCSTR name, LPCSTR dll, int64_t *slot, int64_t *a);
extern bool PECMD_QueryDeviceControlState(HANDLE h);
extern void FUN_14001c82c(void);
extern void FUN_140103a20(void);
extern BOOL PECMD_DosDeviceMount(LPCWSTR a, LPCWSTR b, WCHAR *c, uint32_t d, char e);
extern void PECMD_FormatImDiskDrive(uint32_t dev, LPCWSTR a, LPCWSTR b);
extern void FUN_140035B40(uint32_t a, int b, int c);
extern void PECMD_DeleteDriveMountPoint(uint16_t ch);
extern uint64_t PECMD_StartWorkerThread(void *script, void **pref, uint32_t a3, uint64_t a4,
                                        uint64_t a5, uint32_t a6, uint64_t a7, int64_t a8, int a9);
extern void PECMD_EnsureMciLoaded(void);
extern int64_t PECMD_PerformSystemShutdown(int mode, uint32_t flags, LPCWSTR name);
extern void PECMD_RunShutdownScript(LPCWSTR name, uint32_t mode);
extern void PECMD_AppendDebugLog(LPCSTR msg);
extern bool PECMD_ParseNumber(int64_t *pp, int *out);
extern DWORD FUN_1400195F0(uint64_t script, int64_t ms, int mode, uint64_t *arg);
extern void PECMD_TlsLogWrite(uint64_t script, LPCWSTR fmt, uint64_t a, uint64_t b);
extern HWND PECMD_ReadValidWindowHandle(void *p);
extern void PECMD_TruncateDebugLog(void);
extern void PECMD_InitIfOldSystem(void);
extern WCHAR *PECMD_SkipSwitches(WCHAR *s);
extern WCHAR *PECMD_TruncateAtCommaSkipSpace(WCHAR *s);
extern LPWSTR PECMD_GetFileName(LPWSTR s);
extern int32_t PECMD_IsSingleInstance(void);
extern void PECMD_LoadPelogonShellCfg(uint64_t shell, int mode);
extern int64_t *FUN_14007034C(int64_t *ps, LPCWSTR src);
extern uint16_t *PECMD_IsExplorerPath(int16_t *s);
extern DWORD FUN_14002D708(LPCWSTR s, uint32_t a, int64_t *b, DWORD c, DWORD d);
extern void PECMD_ApplyDesktopWallpaper(void);
extern uint32_t PECMD_LineIsTeamExecLoad(LPCWSTR s);
extern int64_t PECMD_ExecCmdDispatch(void *script, void *cmd, void *s3, void *s4, uint32_t flag,
                                     void *p6, void *s7, void *p8);
extern void PECMD_ExecIndataCommand(LPCWSTR name, LPCWSTR value);
extern void LoadEnvi(void *a, void *b);

/* ---- 缺失的 Win32 API 桩声明 ---- */
extern BOOL SetThreadPriority(HANDLE hThread, int priority);
extern HMENU LoadMenuW(HINSTANCE hInst, LPCWSTR name);
extern BOOL InsertMenuW(HMENU menu, UINT pos, UINT flags, uintptr_t id, LPCWSTR text);
extern BOOL RemoveMenu(HMENU menu, UINT pos, UINT flags);
extern BOOL ExitWindowsEx(UINT flags, DWORD reason);
extern BOOL SetSystemPowerState(BOOL fSuspend, BOOL fForce);
extern BOOL LockWorkStation(void);
extern LPWSTR lstrcatW(LPWSTR dst, LPCWSTR src);
extern int puts(const char *s);
extern int wsprintfA(LPSTR buf, LPCSTR fmt, ...);

/* ---- 本批引用的全局数据 ---- */
extern WCHAR g_szEmpty[];     /* g_szEmpty 空串 */
extern int16_t g_hotkeyIdx;   /* 热键序号 */
extern uint8_t g_u8CCB1;      /* MAIN_DBG 日志标志 */
extern uint8_t g_topiconFlag; /* TOPICON 标志 */
extern uint8_t DAT_140147002;
extern uint8_t DAT_140147003;
extern HWND g_hwndCF78;
extern HWND g_hwndTray;
extern WCHAR *g_pwszD250;
extern uint8_t g_answerFlag;
extern void *g_pSetSuspendState;                /* SetSuspendState 函数指针 */
extern void (*g_pMciSendStringW)(LPCWSTR, int); /* CD 门函数指针 */
extern void *PTR_u_CallBackhWnd_14011e668_2_14013a288;
extern void *g_pRtlCreateUnicodeString;       /* RtlCreateUnicodeString */
extern void *g_pRtlFreeUnicodeString;         /* RtlFreeUnicodeString */
extern void *g_pRtlDosPathNameToNtPathName_U; /* RtlDosPathNameToNtPathName_U */
extern void *g_pDevOpen;                      /* 设备打开 */
extern void *g_pDevOpen2;                     /* 设备打开2 */

/* 常用 Ghidra 整数容器替换 */
#define PECMD_LO32(x) ((uint32_t)(uint64_t)(x))
#define PECMD_HI32(x) ((uint32_t)((uint64_t)(x) >> 32))
#define PECMD_SETLO32(x, v)                                                                        \
    ((void *)(((uint64_t)(uintptr_t)(x) & 0xffffffff00000000ULL) | (uint32_t)(v)))

/* ========== PECMD_HotkeyControl @0x140023640 ==========
 * 处理热键命令行: 查询/设置/删除注册表 HOTKEY 配置并通知主窗口。
 * TODO(verify): PECMD_RegisterCallbackWnd 的 Ghidra 返回类型与 PECMD_ParseIntSkipSepChar 交互。
 */
uint64_t PECMD_HotkeyControl(int64_t *script, WCHAR *cmdline, int msgParam)
{
    WCHAR wc;
    HWND hWnd;
    WCHAR *p;
    WCHAR *pEnd;
    WCHAR *keyName;
    WCHAR *expanded = NULL;
    WCHAR *valueBuf[2];
    WCHAR *valueEnd;
    WCHAR *lockObj = NULL;
    WCHAR *queryPath = NULL;
    char dotMode = 0;
    char c;
    int i;
    int i7;
    uint32_t idLow[2] = {0, 0};
    uint32_t idHigh;
    uint32_t valueLen;
    uint32_t valueId;
    uint32_t scanIdx;
    int hitFound; /* R14(#028②): dc uVar19 查询结果(未命中1/命中0) */
    uint32_t emptyCount;
    int32_t bestIdx;
    LPCWSTR bestValue;
    LRESULT lr;
    WCHAR *local_res10 = cmdline;
    int local_res18 = msgParam;
    uint64_t result = 0;

    EnterCriticalSection(&g_csInit);
    idLow[0] = 0;
    hWnd = (HWND)PECMD_RegisterCallbackWnd(0);
    queryPath = NULL;
    dotMode = '\0';
    PECMD_SkipLeadingControls(&local_res10);
    PECMD_AllocWStringBuffer(valueBuf, 0x1813);
    p = local_res10;
    valueEnd = valueBuf[0];
    c = '*';
    if (*local_res10 == L'-') {
        c = '-';
    }
    if (((WCHAR)(uint16_t)c == *local_res10) && (local_res10[1] != L'\0')) {
        i = lstrlenA("delall");
        if ((char)PECMD_TokPrefixICmp("delall", (uint16_t *)(p + 2), i) != 0) {
            ((void (*)(HKEY, LPCWSTR))(uintptr_t)g_pSHDeleteKeyW)(
                (HKEY)(intptr_t)0xffffffff80000002, WSTR("SOFTWARE\\PELOGON\\RAMDATA\\HOTKEY"));
            goto done;
        }
    }

    scanIdx = 1;
    result = 1;
    hitFound = 1; /* R14(batch-A #028②): dc:21297 uVar19=1, 命中置 0(dc:21356); v0 误用循环计数 scanIdx 作查询结果 */
    keyName = p;
    if (*p == L'?') {
        local_res10 = p + 1;
        queryPath = local_res10;
        if (*local_res10 == L'.') {
            local_res10 = p + 2;
            dotMode = '\x01';
            queryPath = local_res10;
        }
        for (; (wc = *local_res10) != L'\0'; local_res10 = local_res10 + 1) {
            if (((8 < (uint16_t)wc) && ((uint16_t)wc < 0xe)) || (wc == L' ')) {
                if (*local_res10 != L'\0') {
                    *local_res10 = L'\0';
                    local_res10 = local_res10 + 1;
                }
                break;
            }
        }
        PECMD_SkipLeadingControls(&local_res10);
        keyName = local_res10;
    }

    PECMD_CreateMutexSlot(&lockObj, "Global\\PECMD:main:lock"); /* R14: 去宽宏(batch-A #028①); dc:21320 窄字面量, 真体=CreateMutexA(LPCSTR), v0 宽串被按 char* 截断成 "G" */
    PECMD_InitRamdataRegistry(2);
    idHigh = (uint32_t)PECMD_ParseHotkeyCode((int64_t *)&keyName, idLow, script, '\0');
    PECMD_AllocStrSlot(&expanded);
    FUN_14007BF44(script, keyName, &expanded, 0, 1);
    keyName = expanded;
    PECMD_SkipLeadingControls(&keyName);

    if ((int)idHigh < 1) {
        PECMD_FreeStrBuf(&expanded);
        PECMD_ReleaseMutex(&lockObj);
    }
    else {
        bestIdx = -1;
        bestValue = g_szEmpty;
        result = 0;
        emptyCount = 0;
        scanIdx = 1;
        do {
            DWORD typeBuf = 1;
            valueLen = 0x3000;
            valueEnd = valueBuf[0] + 0x12;
            *valueEnd = L'\0';
            PECMD_CrtShim(valueBuf[0], 0x140120458, (void *)(uintptr_t)scanIdx, NULL);
            i = ((int (*)(HKEY, LPCWSTR, LPCWSTR, DWORD *, LPVOID, DWORD *))(
                uintptr_t)g_pSHGetValueW)((HKEY)(intptr_t)0xffffffff80000002,
                                          WSTR("SOFTWARE\\PELOGON\\RAMDATA\\HOTKEY"), valueBuf[0],
                                          &typeBuf, valueEnd, &valueLen);
            if ((i == 0) && (valueBuf[0][0x12] != L'\0')) {
                g_hotkeyIdx = (int16_t)scanIdx;
                local_res10 =
                    (WCHAR *)(((uintptr_t)local_res10 & 0xffffffff00000000ULL) | 0xffffffffULL);
                valueId = 0xffffffff;
                PECMD_ParseIntSkipSepChar((uint64_t *)&valueEnd, (int *)&local_res10, 0x2c);
                PECMD_ParseIntSkipSepChar((uint64_t *)&valueEnd, (int *)&valueId, 0x2c);
                result = 0;
                if ((PECMD_LO32(local_res10) == idLow[0]) && (valueId == idHigh)) {
                    bestValue = valueEnd - (intptr_t)dotMode;
                    bestIdx = (int32_t)scanIdx;
                    hitFound = 0; /* dc:21356 */
                    scanIdx = 0;
                    break;
                }
            }
            else {
                if (bestIdx < 0) {
                    bestIdx = (int32_t)scanIdx;
                }
                emptyCount = (uint32_t)(int)result + 1;
                result = emptyCount;
                if (0x19 < (int)emptyCount) {
                    break;
                }
            }
            scanIdx++;
        } while ((int)scanIdx < 10000);

        p = valueBuf[0];
        if (queryPath == NULL) {
            if (bestIdx < 0) {
                g_hotkeyIdx = g_hotkeyIdx + 1;
                bestIdx = (int32_t)g_hotkeyIdx;
            }
            if (bestIdx < 0x3e9) {
                PECMD_CrtShim(valueBuf[0], 0x140120458, (void *)(uintptr_t)bestIdx, NULL);
                c = '*';
                if (*keyName == L'-') {
                    c = '-';
                }
                if (((WCHAR)(uint16_t)c == *keyName) && (keyName[1] != L'\0') &&
                    ((char)FUN_14005C7C4("del", (uint16_t *)(keyName + 2)) != 0)) {
                    i7 = (int)((int (*)(HKEY, LPCWSTR, LPCWSTR))(uintptr_t)g_pSHDeleteValueW)(
                        (HKEY)(intptr_t)0xffffffff80000002,
                        WSTR("SOFTWARE\\PELOGON\\RAMDATA\\HOTKEY"), valueBuf[0]);
                    i = local_res18;
                }
                else {
                    WCHAR *scan;
                    WCHAR *newVal;
                    int64_t lv;

                    scan = keyName;
                    pEnd = keyName;
                    while ((wc = *scan) != L'\0' &&
                           (((uint16_t)wc < 9 || (0xd < (uint16_t)wc)) && (wc != L' '))) {
                        pEnd = pEnd + 1;
                        wc = *pEnd;
                    }
                    i = lstrlenW(keyName);
                    PECMD_AllocWStringBuffer((WCHAR **)&newVal, (int64_t)i + 0x3c);
                    PECMD_CrtShim(newVal, 0x1401204c8, (void *)(uintptr_t)idLow[0],
                                  (void *)(uintptr_t)idHigh);
                    lv = PECMD_WideStrLen(newVal);
                    i7 = (int)((int (*)(HKEY, LPCWSTR, LPCWSTR, int, LPCWSTR, int))(
                        uintptr_t)g_pSHSetValueW)((HKEY)(intptr_t)0xffffffff80000002,
                                                  WSTR("SOFTWARE\\PELOGON\\RAMDATA\\HOTKEY"),
                                                  valueBuf[0], 1, newVal, (int)lv * 2);
                    i = local_res18;
                    if ((local_res18 >= 0) && (hWnd == (HWND)0)) {
                        hWnd = (HWND)PECMD_RunPecmdMain(script, 1);
                    }
                    PECMD_FreeStrBuf(&newVal);
                }
                if ((i >= 0) && (hWnd != (HWND)0)) {
                    lr = SendMessageW(hWnd, 0x44a, 0, 0);
                    i7 = (int)lr;
                }
                PECMD_FreeStrBuf(&expanded);
                PECMD_ReleaseMutex(&lockObj);
                result = (uint64_t)(int64_t)i7;
            }
            else {
                PECMD_FreeStrBuf(&expanded);
                PECMD_ReleaseMutex(&lockObj);
                result = 0xffffffff80070057ULL;
            }
        }
        else {
            FUN_1400629B8(script, queryPath, (WCHAR *)bestValue);
            result = (uint64_t)(int)hitFound; /* R14: dc uVar19 语义(未命中1/命中0) */
            PECMD_FreeStrBuf(&expanded);
            PECMD_ReleaseMutex(&lockObj);
        }
    }

done:
    PECMD_FreeStrBuf(valueBuf);
    LeaveCriticalSection(&g_csInit);
    return result;
}

/* ========== PECMD_FindSubBlockNthScan @0x140023c48 ==========
 * 在命令文本中匹配一个命令字，支持嵌套/回溯；返回命中的命令字指针。
 * TODO(verify): 结构偏移 0x48/0x88..0x94 为脚本命令分隔符配置。
 */
uint16_t *PECMD_FindSubBlockNthScan(int64_t ctx, uint16_t *key, uint16_t *text, int *outIdx,
                                    uint16_t *end)
{
    uint16_t uVar1;
    uint16_t uVar2;
    uint16_t uVar3;
    uint64_t uVar5;
    uint16_t *puVar6 = NULL;
    uint16_t *puVar9;
    uint64_t uVar7;
    int iVar10;
    int iVar12;
    int iVar13;
    int64_t lVar14;
    void **matches = NULL;
    int64_t local_60;
    uint16_t **local_58;
    int64_t local_68;
    int64_t local_70;
    uint16_t local_res8;
    int local_88;
    uint8_t cVar8 = 1;
    uint8_t savedFlag = 1;

    for (; (uVar3 = *text, *(uint16_t *)((uint8_t *)ctx + 0x92) == uVar3 ||
                               (*(uint16_t *)((uint8_t *)ctx + 0x94) == uVar3) ||
                               (*(uint16_t *)((uint8_t *)ctx + 0x8a) == uVar3) ||
                               (*(uint16_t *)((uint8_t *)ctx + 0x90) == uVar3));
         text = text + 1) {
    }

    iVar13 = 0x19;
    PECMD_AllocWStringBuffer((WCHAR **)&matches, 0xe8);
    iVar10 = 1;
    uVar7 = 1;
    matches[0] = text;
    if (text < end) {
        iVar12 = 0x68;
        puVar9 = text;
        uVar7 = 1;
        do {
            uVar5 = PECMD_MatchPattern((int16_t *)puVar9, (int64_t)((uint8_t *)ctx + 0x9a));
            iVar10 = (int)uVar7;
            if ((uVar5 == 0) &&
                ((uVar3 = puVar9[4], uVar3 == *(uint16_t *)((uint8_t *)ctx + 0x92) ||
                                         (uVar3 == *(uint16_t *)((uint8_t *)ctx + 0x94))) ||
                 ((uVar3 == *(uint16_t *)((uint8_t *)ctx + 0x8a) ||
                   (uVar3 == *(uint16_t *)((uint8_t *)ctx + 0x90)))))) {
                if (iVar10 <= iVar13) {
                    iVar13 = iVar10 + 8;
                    PECMD_AllocString(&matches, iVar12);
                }
                uVar7 = (uint64_t)(iVar10 + 1);
                iVar12 = iVar12 + 8;
                matches[uVar7] = puVar9;
                uVar7 = uVar7 + 1;
            }
            else {
                uVar5 = PECMD_MatchPattern((int16_t *)puVar9, (int64_t)((uint8_t *)ctx + 0xaa));
                if ((uVar5 == 0) &&
                    ((((uVar3 = puVar9[4], uVar3 == *(uint16_t *)((uint8_t *)ctx + 0x92) ||
                                               (uVar3 == *(uint16_t *)((uint8_t *)ctx + 0x94))) ||
                       (uVar3 == *(uint16_t *)((uint8_t *)ctx + 0x8a))) ||
                      (uVar3 == *(uint16_t *)((uint8_t *)ctx + 0x90))) &&
                     (0 < (int64_t)uVar7))) {
                    uVar7 = (uint64_t)(iVar10 - 1);
                    iVar12 = iVar12 - 8;
                    uVar7 = uVar7 - 1;
                }
            }
            iVar10 = (int)uVar7;
            while ((uVar3 = *puVar9, uVar3 != *(uint16_t *)((uint8_t *)ctx + 0x88) &&
                                         (uVar3 != *(uint16_t *)((uint8_t *)ctx + 0x8a))) &&
                   (uVar3 != *(uint16_t *)((uint8_t *)ctx + 0x90))) {
                puVar9 = puVar9 + 1;
            }
            while ((uVar3 = *puVar9, *(uint16_t *)((uint8_t *)ctx + 0x92) == uVar3 ||
                                         (*(uint16_t *)((uint8_t *)ctx + 0x94) == uVar3)) ||
                   ((*(uint16_t *)((uint8_t *)ctx + 0x8a) == uVar3 ||
                     (*(uint16_t *)((uint8_t *)ctx + 0x90) == uVar3)))) {
                puVar9 = puVar9 + 1;
            }
        } while (puVar9 < end);
    }

    puVar6 = NULL;
    uVar3 = *key;
    puVar9 = key;
    while ((uVar3 != 0 && ((uVar3 < 9 || (0xd < uVar3)))) && (uVar3 != 0x20)) {
        puVar9 = puVar9 + 1;
        uVar3 = *puVar9;
    }
    local_60 = (int64_t)iVar10;
    matches[local_60] = NULL;
    cVar8 = 1;
    local_58 = (uint16_t **)matches;
    local_68 = (int64_t)*outIdx;
    *outIdx = -0x80000000;
    iVar10 = 1;
    local_res8 = *(uint16_t *)((uint8_t *)ctx + 0x48) ^ 0x2c;
    iVar13 = (int)(puVar9 - key);
    local_70 = (int64_t)iVar13;
    savedFlag = 1;
    local_88 = -100;
    lVar14 = 1;
    if (*text != 0) {
        do {
            uVar5 = PECMD_MatchPattern((int16_t *)text, (int64_t)((uint8_t *)ctx + 0x9a));
            uVar3 = 0;
            if (uVar5 == 0) {
                uVar1 = text[4];
                uVar2 = *(uint16_t *)((uint8_t *)ctx + 0x92);
                if ((((uVar1 != uVar2) && (uVar1 != *(uint16_t *)((uint8_t *)ctx + 0x94))) &&
                     (uVar1 != *(uint16_t *)((uint8_t *)ctx + 0x8a))) &&
                    (uVar1 != *(uint16_t *)((uint8_t *)ctx + 0x90))) {
                    goto else_branch;
                }
                puVar9 = text + 5;
                PECMD_SkipLeadingControls(&puVar9);
                if (cVar8 != 0) {
                    int16_t sVar4 = PECMD_XorStrNCaseCmp(ctx, puVar9, (int16_t *)key, iVar13);
                    uVar3 = 0;
                    if ((sVar4 == 0) ||
                        ((((uVar1 = puVar9[local_70],
                            uVar1 != uVar2 && (uVar1 != *(uint16_t *)((uint8_t *)ctx + 0x94))) &&
                           (uVar1 != *(uint16_t *)((uint8_t *)ctx + 0x8a))) &&
                          (((uVar1 != *(uint16_t *)((uint8_t *)ctx + 0x90) &&
                             (uVar1 != *(uint16_t *)((uint8_t *)ctx + 0x88))) &&
                            (local_res8 != uVar1)))) ||
                         (local_68 < lVar14))) {
                        if ((local_60 <= lVar14) || (text != local_58[lVar14])) {
                            savedFlag = cVar8;
                            local_88 = iVar10;
                            uVar3 = 0;
                            cVar8 = 0;
                        }
                    }
                    else if (*outIdx < iVar10) {
                        *outIdx = iVar10;
                        puVar6 = text;
                    }
                }
                iVar10 = iVar10 + 1;
                lVar14 = lVar14 + 1;
                text = puVar9;
            }
            else {
            else_branch:
                uVar5 = PECMD_MatchPattern((int16_t *)text, (int64_t)((uint8_t *)ctx + 0xaa));
                uVar3 = 0;
                if ((uVar5 == 0) &&
                    (((uVar1 = text[4], uVar1 == *(uint16_t *)((uint8_t *)ctx + 0x92) ||
                                            (uVar1 == *(uint16_t *)((uint8_t *)ctx + 0x94))) ||
                      (uVar1 == *(uint16_t *)((uint8_t *)ctx + 0x8a))) ||
                     (uVar1 == *(uint16_t *)((uint8_t *)ctx + 0x90)))) {
                    iVar10 = iVar10 - 1;
                    lVar14 = lVar14 - 1;
                    if (local_88 == iVar10) {
                        cVar8 = savedFlag;
                    }
                    if (lVar14 < 1) {
                        break;
                    }
                }
            }
            while ((uVar1 = *text, uVar1 != *(uint16_t *)((uint8_t *)ctx + 0x88) &&
                                       (uVar1 != *(uint16_t *)((uint8_t *)ctx + 0x8a))) &&
                   (uVar1 != *(uint16_t *)((uint8_t *)ctx + 0x90))) {
                text = text + 1;
            }
            while ((uVar1 = *text, *(uint16_t *)((uint8_t *)ctx + 0x92) == uVar1 ||
                                       (*(uint16_t *)((uint8_t *)ctx + 0x94) == uVar1)) ||
                   ((*(uint16_t *)((uint8_t *)ctx + 0x8a) == uVar1 ||
                     (*(uint16_t *)((uint8_t *)ctx + 0x90) == uVar1)))) {
                text = text + 1;
            }
        } while (*text != uVar3);
    }
    PECMD_FreeStrBuf((WCHAR **)&matches);
    return puVar6;
}

/* ========== PECMD_FindSubBlockNth @0x1400240c0 ==========
 * 匹配命令字的简化版（无 end 参数）。
 */
uint16_t *PECMD_FindSubBlockNth(int64_t ctx, uint16_t *key, uint16_t *text, int *outIdx)
{
    uint16_t uVar1;
    uint16_t uVar2;
    uint16_t uVar4;
    int16_t sVar5;
    uint64_t uVar6;
    int iVar7;
    uint16_t *puVar8;
    uint32_t uVar10;
    uint8_t local_48[16];

    for (; (uVar4 = *text, *(uint16_t *)((uint8_t *)ctx + 0x92) == uVar4 ||
                               (*(uint16_t *)((uint8_t *)ctx + 0x94) == uVar4) ||
                               (*(uint16_t *)((uint8_t *)ctx + 0x8a) == uVar4) ||
                               (*(uint16_t *)((uint8_t *)ctx + 0x90) == uVar4));
         text = text + 1) {
    }
    uVar4 = *key;
    puVar8 = key;
    while ((uVar4 != 0 && ((uVar4 < 9 || (0xd < uVar4)))) && (uVar4 != 0x20)) {
        puVar8 = puVar8 + 1;
        uVar4 = *puVar8;
    }
    iVar7 = (int)(puVar8 - key);
    if (0 < iVar7) {
        int iVar3 = *outIdx;
        uVar4 = *(uint16_t *)((uint8_t *)ctx + 0x48);
        uVar10 = (uint32_t)(0 < iVar3);
        if (*text != *(uint16_t *)((uint8_t *)ctx + 0x88)) {
            do {
                int64_t lVar9 = (int64_t)((uint8_t *)ctx + 0x9a);
                for (puVar8 = text;
                     ((uVar1 = *puVar8, *(uint16_t *)((uint8_t *)ctx + 0x92) == uVar1 ||
                                            (*(uint16_t *)((uint8_t *)ctx + 0x94) == uVar1)) ||
                      ((*(uint16_t *)((uint8_t *)ctx + 0x8a) == uVar1 ||
                        (*(uint16_t *)((uint8_t *)ctx + 0x90) == uVar1))));
                     puVar8 = puVar8 + 1) {
                }
                memcpy(local_48, (const void *)puVar8, 8);
                uVar6 = PECMD_MatchPattern((int16_t *)puVar8, lVar9);
                if (uVar6 == 0) {
                    uVar1 = puVar8[4];
                    uVar2 = *(uint16_t *)((uint8_t *)ctx + 0x92);
                    if ((((uVar1 != uVar2) && (uVar1 != *(uint16_t *)((uint8_t *)ctx + 0x94))) &&
                         (uVar1 != *(uint16_t *)((uint8_t *)ctx + 0x8a))) &&
                        (uVar1 != *(uint16_t *)((uint8_t *)ctx + 0x90))) {
                        goto else_branch;
                    }
                    uVar10 = uVar10 + 1;
                    for (puVar8 = puVar8 + 5;
                         ((uVar1 = *puVar8,
                         uVar2 == uVar1 || (*(uint16_t *)((uint8_t *)ctx + 0x94) == uVar1)) ||
                          ((*(uint16_t *)((uint8_t *)ctx + 0x8a) == uVar1 ||
                            (*(uint16_t *)((uint8_t *)ctx + 0x90) == uVar1))));
                         puVar8 = puVar8 + 1) {
                    }
                    sVar5 = PECMD_XorStrNCaseCmp(ctx, puVar8, (int16_t *)key, iVar7);
                    if ((sVar5 == 0) &&
                        ((((uVar1 = puVar8[iVar7],
                            uVar1 == uVar2 || (uVar1 == *(uint16_t *)((uint8_t *)ctx + 0x94))) ||
                           (uVar1 == *(uint16_t *)((uint8_t *)ctx + 0x8a))) ||
                          (((uVar1 == *(uint16_t *)((uint8_t *)ctx + 0x90) ||
                             (uVar1 == *(uint16_t *)((uint8_t *)ctx + 0x88))) ||
                            ((uVar4 ^ 0x2c) == uVar1)))))) {
                        if (*outIdx < 1) {
                            if (uVar10 == 1) {
                                *outIdx = 0;
                                return text;
                            }
                        }
                        else if ((int)uVar10 <= *outIdx + 1) {
                            return text;
                        }
                    }
                }
                else {
                else_branch:
                    uVar6 = PECMD_MatchPattern((int16_t *)puVar8, (int64_t)((uint8_t *)ctx + 0xaa));
                    if ((uVar6 == 0) &&
                        ((((uVar1 = puVar8[4],
                            uVar1 == *(uint16_t *)((uint8_t *)ctx + 0x92) ||
                                (uVar1 == *(uint16_t *)((uint8_t *)ctx + 0x94))) ||
                           (uVar1 == *(uint16_t *)((uint8_t *)ctx + 0x8a))) ||
                          (uVar1 == *(uint16_t *)((uint8_t *)ctx + 0x90))) &&
                         ((uVar10 = uVar10 - 1, (int)uVar10 < (int)(uint32_t)(0 < iVar3))))) {
                        return NULL;
                    }
                }
                while ((uVar1 = *puVar8, *(uint16_t *)((uint8_t *)ctx + 0x88) != uVar1 &&
                                             (*(uint16_t *)((uint8_t *)ctx + 0x8a) != uVar1)) &&
                       (*(uint16_t *)((uint8_t *)ctx + 0x90) != uVar1)) {
                    puVar8 = puVar8 + 1;
                }
                text = puVar8;
            } while (*puVar8 != *(uint16_t *)((uint8_t *)ctx + 0x88));
        }
    }
    return NULL;
}

/* ========== PECMD_GetNextCommandArg @0x140024350 ==========
 * 取下一个命令参数；带 end 时使用回溯匹配，否则使用简化匹配。
 * 返回匹配到的参数指针，失败时置 *flags = 0x80000000 并返回 NULL。
 */
uint16_t *PECMD_GetNextCommandArg(int64_t ctx, uint16_t *key, uint16_t *text, uint32_t *flags,
                                  uint16_t *end)
{
    uint16_t uVar1;
    uint16_t uVar3;
    uint16_t *puVar2;
    uint16_t *puVar4;
    uint16_t *puVar5;
    uint16_t uVar6 = 0x20;

    for (; (uVar1 = *text, *(uint16_t *)((uint8_t *)ctx + 0x92) == uVar1 ||
                               (*(uint16_t *)((uint8_t *)ctx + 0x94) == uVar1) ||
                               (*(uint16_t *)((uint8_t *)ctx + 0x8a) == uVar1) ||
                               (*(uint16_t *)((uint8_t *)ctx + 0x90) == uVar1));
         text = text + 1) {
    }
    uVar1 = *key;
    puVar5 = key;
    while ((uVar1 != 0 && ((uVar1 < 9 || (0xd < uVar1)))) && (uVar1 != 0x20)) {
        puVar5 = puVar5 + 1;
        uVar1 = *puVar5;
    }

    if (end == NULL) {
        puVar2 = PECMD_FindSubBlockNth(ctx, key, text, (int *)flags);
    }
    else {
        puVar2 = PECMD_FindSubBlockNthScan(ctx, key, text, (int *)flags, end);
    }
    if (puVar2 != NULL) {
        uVar1 = *(uint16_t *)((uint8_t *)ctx + 0x92);
        for (puVar4 = puVar2; ((uVar3 = *puVar4,
                              uVar1 == uVar3 || (*(uint16_t *)((uint8_t *)ctx + 0x94) == uVar3)) ||
                               ((*(uint16_t *)((uint8_t *)ctx + 0x8a) == uVar3 ||
                                 (*(uint16_t *)((uint8_t *)ctx + 0x90) == uVar3))));
             puVar4 = puVar4 + 1) {
        }
        for (puVar4 = puVar4 + 5;
             (((uVar1 == *puVar4 || (*(uint16_t *)((uint8_t *)ctx + 0x94) == *puVar4)) ||
               (*(uint16_t *)((uint8_t *)ctx + 0x8a) == *puVar4)) ||
              (*(uint16_t *)((uint8_t *)ctx + 0x90) == *puVar4));
             puVar4 = puVar4 + 1) {
        }
        for (puVar4 = puVar4 + (int)(puVar5 - key);
             ((*puVar4 == uVar1 || (*puVar4 == *(uint16_t *)((uint8_t *)ctx + 0x94))) ||
              ((*puVar4 == (*(uint16_t *)((uint8_t *)ctx + 0x48) ^ 0xb) ||
                (*puVar4 == (*(uint16_t *)((uint8_t *)ctx + 0x48) ^ 0xc)))));
             puVar4 = puVar4 + 1) {
        }
        uVar1 = *(uint16_t *)((uint8_t *)ctx + 0x48) ^ *puVar4;
        uVar3 = puVar4[1] ^ *(uint16_t *)((uint8_t *)ctx + 0x48);
        if (uVar1 == 0x2c) {
            *flags = *flags | 0x40000000;
            return puVar2;
        }
        if (uVar1 == 0x2a) {
            if (uVar3 == 0) {
                return puVar2;
            }
            if ((8 < uVar3) && (uVar3 < 0xe)) {
                return puVar2;
            }
        }
        else {
            if (uVar1 == 0) {
                return puVar2;
            }
            if (uVar1 == 0xd) {
                return puVar2;
            }
            if (uVar1 == 10) {
                return puVar2;
            }
            if (uVar1 == 0x3b) {
                return puVar2;
            }
            if (uVar1 == 0x60) {
                return puVar2;
            }
            if (uVar1 != 0x2f) {
                goto fail;
            }
            uVar6 = 0x2f;
        }
        if (uVar6 == uVar3) {
            return puVar2;
        }
    }
fail:
    *flags = 0x80000000;
    return NULL;
}

/* ========== PECMD_CreatePageFile @0x14002a910 ==========
 * PAGE 命令: 配置系统页面文件 (NtCreatePagingFile + PagingFiles 注册表)。
 * TODO(verify): 反编译中 pthreadlocinfo 类型实为 WCHAR* 字符串游标；
 *               lc_codepage 偏移按 local_98+0x114 对应的字节偏移保留。
 */
uint64_t PECMD_CreatePageFile(WCHAR *cmdline)
{
    WCHAR *cmd = cmdline;
    WCHAR *dup = NULL;
    uint16_t *token;
    int64_t tokenLen = 0;
    uint64_t minSize = 0;
    uint64_t maxSize = 0;
    char isForce;
    uint64_t uVar7;
    WCHAR *buf = NULL;
    WCHAR *regCur;
    WCHAR *psz;
    WCHAR *newEntry = NULL;
    WCHAR *entryList = NULL;
    uint64_t result = 0;
    int i;
    int nChar;
    uint64_t local_res10 = 0;
    DWORD regSize;
    HMODULE hModule;
    void *pNtCreate;

    PECMD_SkipLeadingControls(&cmd);
    isForce = FUN_1400660AC("*force", (int64_t *)&cmd, -1);
    token = FUN_140024C48((int64_t *)&cmd, &tokenLen, 1);
    PECMD_StrBldCopyWideN(&dup, cmd, tokenLen);
    cmd = dup;
    if (((8 < *token) && (*token < 0xe)) || (*token == 0x20)) {
        *token = 0;
        token = token + 1;
        PECMD_SkipLeadingControls(&token);
        PECMD_EvalParenExpression((int64_t *)&token, &minSize);
        if (((uint16_t)(*token - 0x30) < 10) || (*token == 0x28)) {
            PECMD_EvalParenExpression((int64_t *)&token, &maxSize);
        }
    }
    if ((int64_t)maxSize < (int64_t)minSize) {
        maxSize = minSize;
    }
    if ((int64_t)maxSize < 0) {
        result = 0xffffffff80070057ULL;
        goto cleanup;
    }

    uVar7 = (uint64_t)PECMD_GetAvailPhysMemoryMB();
    if (((int64_t)maxSize < 1) || ((isForce == '\0') && (maxSize <= uVar7))) {
        goto cleanup;
    }

    PECMD_AllocWStringBuffer((WCHAR **)&buf, 0xe52);
    buf[0] = cmd[0];
    buf[1] = cmd[1];
    buf[2] = L'\0';
    regCur = buf + 0x31c;
    psz = buf + 0x218;
    *regCur = L'\0';
    uVar7 = (uint64_t)PECMD_GetDiskFreeMB(buf);
    if ((isForce == '\0') && ((uVar7 & 0xffffffff) < maxSize)) {
        local_res10 = local_res10 & 0xffff000000000000ULL;
        i = PECMD_FindSuitableDrive(buf, (int)maxSize, (WCHAR *)&local_res10);
        buf[0] = *(WCHAR *)((uint8_t *)&local_res10 + i * 2);
    }
    uVar7 = 1;
    if (buf[0] != L'\0') {
        cmd[0] = buf[0];
        FUN_140103068((int16_t *)(buf + 0x114), (int16_t *)(cmd + 0x114));
        QueryDosDeviceW(buf, buf + 0x10, 0x104);
        lstrcatW(buf + 0x10, buf + 0x114);
        StrCpyNW(psz, buf + 0x10, 0x104);
        {
            int64_t lv = PECMD_WideStrLen(psz);
            int16_t len16 = (int16_t)lv * 2;
            int64_t initialBytes = (int64_t)(minSize << 0x14);
            int64_t maxBytes = (int64_t)(maxSize << 0x14);
            int status;

            hModule = GetModuleHandleW(WSTR("NTDLL"));
            pNtCreate = GetProcAddress(hModule, "NtCreatePagingFile");
            if (pNtCreate != NULL) {
                int (*ntCreate)(int16_t *, int64_t *, int64_t *);
                *(void **)&ntCreate = pNtCreate;
                FUN_14001C2CC(WSTR("SeCreatePagefilePrivilege"), 2, 0);
                status = ntCreate(&len16, &initialBytes, &maxBytes);
                uVar7 = (uint64_t)(int64_t)status;
                if (status == 0) {
                    local_res10 = (local_res10 & 0xffffffff00000000ULL) | 0x1450;
                    regSize = FUN_14005C4E0((HKEY)(intptr_t)0xffffffff80000002,
                                            WSTR("SYSTEM\\CurrentControlSet\\Control\\Session "
                                                 "Manager\\Memory Management"),
                                            WSTR("PagingFiles"), NULL, (BYTE *)regCur,
                                            (DWORD *)&local_res10);
                    if (regSize == 0) {
                        uint32_t uVar12 = (uint32_t)local_res10 >> 1;
                        uVar7 = uVar12;
                        local_res10 = (local_res10 & 0xffffffff00000000ULL) | uVar12;
                        if (uVar12 == 0) {
                            goto after_reg;
                        }
                        do {
                            uVar12 = (uint32_t)uVar7 - 1;
                            uVar7 = uVar12;
                            if (regCur[uVar12] != L'\0') {
                                break;
                            }
                            local_res10 = (local_res10 & 0xffffffff00000000ULL) | uVar12;
                        } while (uVar12 != 0);
                        if (((int)uVar7 == 0) || (regCur[uVar7] == L'\0')) {
                            goto after_reg;
                        }
                        regCur[uVar7] = L'\0';
                        uVar7 = (uint64_t)((uint32_t)local_res10 + 1);
                    }
                    else {
                        buf[0x420] = L'\0';
                        uVar7 = result;
                    }
                    local_res10 = (local_res10 & 0xffffffff00000000ULL) | (uint32_t)uVar7;
                after_reg:
                    regCur[uVar7] = L'\0';
                    {
                        WCHAR *regEnd = regCur + (uint32_t)local_res10;
                        WCHAR *pCur = regEnd;
                        int nCharLen;
                        int local_res18;

                        nCharLen = lstrlenW(cmd);
                        nChar = nCharLen;
                        PECMD_AllocWStringBuffer((WCHAR **)&newEntry, (int64_t)(nCharLen + 0x3c));
                        {
                            WCHAR *pw = newEntry;
                            int k = PECMD_CrtShim(newEntry, 0x140120b48, cmd, NULL);
                            WCHAR *pW = PECMD_SprintfRetEnd(pw + k, minSize, WSTR("%I64d "));
                            PECMD_FormatI64Dec(pW, maxSize);
                            i = lstrlenW(newEntry);
                            i = i + 1;
                            local_res18 = i;
                        }
                        PECMD_AllocStrSlot(&entryList);
                        uVar7 = result;
                        do {
                            int i6 = (int)result;
                            int i18 = (int)uVar7;
                            if (pCur <= regCur) {
                                goto append_done;
                            }
                            i = StrCmpNIW(regCur, cmd, nChar);
                            if ((i != 0) || ((((uint16_t)regCur[nChar] < 9 ||
                                               (0xd < (uint16_t)regCur[nChar])) &&
                                              (regCur[nChar] != L' ')))) {
                                i = lstrlenW(regCur);
                                i = i + 1;
                                newEntry = regCur;
                                goto append_entry;
                            }
                            if (i18 == 0) {
                                while (1) {
                                    i6 = (int)result;
                                    uVar7 = 1;
                                append_entry:
                                    i18 = (int)uVar7;
                                    PECMD_AllocString(&entryList, i + i6 + 2);
                                    memcpy(entryList + i6 * 2, newEntry, i * 2);
                                    result = (uint64_t)(uint32_t)(i + i6);
                                    pCur = regEnd;
                                    nChar = nCharLen;
                                    i = local_res18;
                                    if (regCur < regEnd) {
                                        break;
                                    }
                                append_done:
                                    if (i18 != 0) {
                                        entryList[(int)result * 2] = L'\0';
                                        entryList[(int)result * 2 + 1] = L'\0';
                                        PECMD_RegSetValueWithOpen(
                                            (HKEY)(intptr_t)0xffffffff80000002,
                                            WSTR("SYSTEM\\CurrentControlSet\\Control\\Session "
                                                 "Manager\\Memory Management"),
                                            WSTR("PagingFiles"), 7, (BYTE *)entryList,
                                            (int)result * 2 + 2);
                                        PECMD_FreeStrBuf(&entryList);
                                        PECMD_FreeStrBuf(&newEntry);
                                        uVar7 = 0; /* NtCreatePagingFile 成功 */
                                        goto cleanup2;
                                    }
                                }
                            }
                            i = lstrlenW(regCur);
                            regCur = regCur + i + 1;
                        } while (1);
                    }
                }
            }
        }
    }
cleanup2:
    result = uVar7;
    PECMD_FreeStrBuf(&buf);
cleanup:
    PECMD_FreeStrBuf(&dup);
    return result;
}

/* ========== FUN_14002B2EC @0x14002b2ec ==========
 * 扫描 INF 文本，寻找与命令行匹配的驱动行并触发安装。
 * TODO(verify): 反编译中 goto 布局原样保留，语义为逐行解析 INF。
 */
LPCWSTR FUN_14002B2EC(int64_t *ctx, uint64_t flags, LPCSTR infText)
{
    uint8_t dbg = *(uint8_t *)((uint8_t *)ctx + 0x36);
    int64_t base = *ctx;
    LPCWSTR infPath = (LPCWSTR)(flags & 0xfffffffffffffffeULL);
    WCHAR *logBuf;
    char *fileData = NULL;
    char *rawFile = NULL;
    char *line;
    char *lineOut;
    short *psVar13;
    short *psVar19;
    char cVar2;
    char cVar3;
    uint8_t bVar4;
    uint16_t uVar8;
    uint16_t uVar15;
    WCHAR WVar6;
    WCHAR WVar7;
    uint16_t *puVar11;
    uint16_t *puVar12;
    uint16_t *puVar14;
    LPCWSTR pWVar16;
    LPCWSTR pWVar17;
    LPCWSTR result = NULL;

    if ((dbg & 2) != 0) {
        PECMD_AllocWStringBuffer((WCHAR **)&logBuf, 0x1000);
        wsprintfW(logBuf, WSTR("CheckInf--Bigin:%s"));
        FUN_140025f10((int64_t)(ctx + 1), logBuf, 0, (void *)0x1100, NULL, NULL);
        PECMD_FreeStrBuf(&logBuf);
    }

    if (infText == NULL) {
        void *raw = PECMD_ReadFileToBuffer(infPath, NULL, NULL);
        fileData = (char *)raw;
        if (raw != NULL && ((char *)raw)[3] != '\0') {
            fileData = NULL;
            rawFile = (char *)raw;
            FUN_1400637DC((int64_t *)&fileData, rawFile, 0xffffffffffffffffULL,
                          0xffffffffffffffffULL);
            PECMD_FreeStrBuf((WCHAR **)&rawFile);
        }
        if (fileData == NULL) {
            goto no_data;
        }
        fileData[0] = '\n';
        fileData[1] = '\0';
        line = (char *)(fileData + 2);
        lineOut = line;
        if (*line != 0) {
        LAB_14002b41b:
            psVar13 = (short *)line;
            if (-1 < (int)ctx[0x34]) {
                short sVar5;
                do {
                    sVar5 = *psVar13;
                    if (sVar5 == 0) {
                        break;
                    }
                    psVar13 = psVar13 + 1;
                } while (sVar5 != 10);
                if ((*line != 0x3b) && (*line != 0x5b)) {
                    for (; line < (char *)psVar13; line = line + 1) {
                        if ((*line == 0x3d) || (*line == 0x2c)) {
                            if (*line == 0x3d) {
                                goto LAB_14002b48e;
                            }
                            break;
                        }
                    }
                }
                goto LAB_14002b4c8;
            }
        }
    LAB_14002b4d2:
        *lineOut = 0;
        infText = fileData;
        if (fileData == NULL) {
            goto no_data;
        }
    }

    cVar2 = infText[3];
    puVar14 = (uint16_t *)(infText + 2);
    pWVar16 = (LPCWSTR)(base + 2);
    while ((pWVar16[-1] != L'\0' && (-1 < (int)ctx[0x34]))) {
        WVar6 = pWVar16[-1];
        WVar7 = pWVar16[-2];
        if ((*pWVar16 != L'\0') &&
            (psVar19 = PECMD_TableLookupEntry((int64_t)ctx, pWVar16, (uint32_t)(uint16_t)WVar7),
             psVar19 == NULL)) {
            uVar15 = (uint16_t)WVar6;
            puVar11 = puVar14;
            if (cVar2 == '\0') {
                if (*puVar14 != 0) {
                    int64_t diff = (int64_t)pWVar16 - (int64_t)puVar14;
                    do {
                        uint32_t uVar18 = 0;
                        if (((puVar11[-1] == 0x20) || (puVar11[-1] == 0x2c)) ||
                            (puVar11[-1] == 10)) {
                            puVar12 = puVar11;
                            if (uVar15 != 0) {
                                do {
                                    uVar8 = *(uint16_t *)(diff + (int64_t)puVar12);
                                    if ((*puVar12 != uVar8) &&
                                        ((0x7a < *puVar12 ||
                                          (((uint32_t)*puVar12 != (uint32_t)uVar8 + 0x20 &&
                                            ((uint32_t)*puVar12 != (uint32_t)uVar8 - 0x20)))))) {
                                        goto next_ansi;
                                    }
                                    uVar18 = uVar18 + 1;
                                    puVar12 = puVar12 + 1;
                                } while (uVar18 < uVar15);
                            }
                            uVar8 = puVar11[(uint16_t)WVar6];
                            if (((8 < uVar8) && (uVar8 < 0xe)) ||
                                ((uVar8 == 0x20 || (uVar8 == 0x26)))) {
                                goto found_driver;
                            }
                        }
                    next_ansi:
                        puVar11 = puVar11 + 1;
                        diff = diff - 2;
                    } while (*puVar11 != 0);
                }
            }
            else {
                cVar3 = (char)*puVar14;
                while (cVar3 != '\0') {
                    uint32_t uVar18 = 0;
                    cVar3 = *(char *)((int64_t)puVar11 - 1);
                    if (((cVar3 == ' ') || (cVar3 == ',')) || (cVar3 == '\n')) {
                        puVar12 = puVar11;
                        pWVar17 = pWVar16;
                        if (uVar15 != 0) {
                            do {
                                cVar3 = (char)*puVar12;
                                if ((short)cVar3 != *pWVar17) {
                                    if (0x7a < (uint16_t)(short)cVar3) {
                                        goto next_ansi2;
                                    }
                                    if ((((uint32_t)cVar3 & 0xffffU) !=
                                         (uint32_t)(uint16_t)*pWVar17 + 0x20) &&
                                        (((uint32_t)cVar3 & 0xffffU) !=
                                         (uint32_t)(uint16_t)*pWVar17 - 0x20)) {
                                        goto next_ansi2;
                                    }
                                }
                                uVar18 = uVar18 + 1;
                                puVar12 = (uint16_t *)((int64_t)puVar12 + 1);
                                pWVar17 = pWVar17 + 1;
                            } while (uVar18 < uVar15);
                        }
                        bVar4 = *(uint8_t *)((uint64_t)(uint16_t)WVar6 + (int64_t)puVar11);
                        if (((8 < bVar4) && (bVar4 < 0xe)) ||
                            ((bVar4 == 0x20 || (bVar4 == 0x26)))) {
                            goto found_driver;
                        }
                    }
                next_ansi2:
                    puVar11 = (uint16_t *)((int64_t)puVar11 + 1);
                    cVar3 = *(char *)puVar11;
                }
            }
            puVar11 = NULL;
        found_driver:
            if (puVar11 != NULL) {
                if ((flags & 1) != 0) {
                    SetThreadPriority(GetCurrentThread(), 2);
                }
                PECMD_AllocWStringBuffer((WCHAR **)&logBuf, 0x2800);
                _snwprintf(logBuf, 0x27ff, WSTR("找到驱动: %s, INF 文件: %s"), pWVar16, infPath);
                FUN_140025f10((int64_t)(ctx + 1), logBuf, 0, (void *)0x11, NULL, NULL);
                PECMD_FreeStrBuf(&logBuf);
                PECMD_WriteParamRecord((int64_t)ctx, 'F', pWVar16, infPath);
                goto done;
            }
        }
        pWVar16 = pWVar16 + (uint64_t)(uint16_t)WVar7 + 3;
    }

no_data:
    if ((dbg & 2) != 0) {
        PECMD_AllocWStringBuffer((WCHAR **)&logBuf, 0x1000);
        wsprintfW(logBuf, WSTR("CheckInf--End:%s"));
        FUN_140025f10((int64_t)(ctx + 1), logBuf, 0, (void *)0x1100, NULL, NULL);
        PECMD_FreeStrBuf(&logBuf);
    }
    result = NULL;
done:
    PECMD_FreeStrBuf((WCHAR **)&fileData);
    return result;

    /* 以下标签通过上面的 goto 可达: INF 行解析 */
LAB_14002b48e: {
    char *psVar10 = line;
    line = psVar10 + 1;
    if (psVar13 <= (short *)line) {
        goto LAB_14002b4c8;
    }
    psVar10 = psVar10 + 2;
    if (*line == 0x2c) {
        for (; *psVar10 == 0x20; psVar10 = psVar10 + 1) {
        }
        if (*psVar10 != 10) {
            for (; psVar10 < (char *)psVar13; psVar10 = psVar10 + 1) {
                *lineOut = *psVar10;
                lineOut = lineOut + 1;
            }
        }
    }
}
LAB_14002b4c8:
    line = (char *)psVar13;
    if (*(char *)psVar13 == 0) {
        goto LAB_14002b4d2;
    }
    goto LAB_14002b41b;
}

/* ========== FUN_14002B9EC @0x14002b9ec ==========
 * 将 INF/SYS 文件复制到系统目录，并可选执行 INF 处理。
 */
DWORD FUN_14002B9EC(int64_t ctx, LPCWSTR srcPath, uint32_t flags)
{
    WCHAR *buf = NULL;
    LPWSTR fileName = NULL;
    WCHAR *srcDir;
    WCHAR *infDir;
    WCHAR *logBuf;
    int infLen;
    int drvLen;
    int sysLen;
    HANDLE hFind = (HANDLE)0;
    WIN32_FIND_DATAW fd;
    DWORD err;
    WCHAR envBuf1[20];
    WCHAR envBuf2[24];
    WCHAR envBuf3[32];

    PECMD_AllocWStringBuffer((WCHAR **)&buf, 0x19c8);
    fileName = NULL;
    FUN_14006459C(srcPath, 0x527, buf, &fileName);
    if (fileName == NULL) {
        err = GetLastError();
        if (err == 0) {
            err = 1;
        }
        goto done;
    }

    srcDir = buf + 0x528;
    {
        int lenBytes = (int)(fileName - buf) * 2;
        memcpy(srcDir, buf, lenBytes);
        *(uint16_t *)((uint8_t *)srcDir + lenBytes) = 0;
    }
    infDir = buf + 0xa50;
    if (*(LPCWSTR *)((uint8_t *)ctx + 0x168) == NULL) {
        memcpy(envBuf1, L"%SystemRoot%\\INF\\", 0x24);
        ExpandEnvironmentStringsW(envBuf1, infDir, 0x104);
        memcpy(envBuf2, L"%SystemRoot%\\SYSTEM32\\DRIVERS\\", 0x3e);
        ExpandEnvironmentStringsW(envBuf2, buf + 0xf78, 0x104);
        memcpy(envBuf3, L"%SystemRoot%\\SYSTEM32\\", 0x2e);
        ExpandEnvironmentStringsW(envBuf3, buf + 0x14a0, 0x104);
    }
    else {
        LPCWSTR base = *(LPCWSTR *)((uint8_t *)ctx + 0x168);
        int baseLen = lstrlenW(base);
        if ((base[baseLen - 1] == L'\\') || (base[baseLen - 1] == L'/')) {
            baseLen = baseLen - 1;
        }
        {
            int lenBytes = baseLen * 2;
            memcpy(infDir, base, lenBytes);
            memcpy((uint8_t *)infDir + lenBytes, WSTR("\\INF\\"), 0xc);
            memcpy(buf + 0xf78, base, lenBytes);
            memcpy((uint8_t *)(buf + 0x14a0) + lenBytes, WSTR("\\SYSTEM32\\DRIVERS\\"), 0x26);
            memcpy(buf + 0x14a0, base, lenBytes);
            memcpy((uint8_t *)(buf + 0x14a0) + lenBytes, WSTR("\\SYSTEM32\\"), 0x16);
        }
    }

    infLen = lstrlenW(infDir);
    drvLen = lstrlenW(buf + 0xf78);
    sysLen = lstrlenW(buf + 0x14a0);
    lstrcpyW(infDir + infLen, fileName);

    PECMD_AllocWStringBuffer((WCHAR **)&logBuf, 0x2800);
    _snwprintf(logBuf, 0x27ff, WSTR("ToSys:Copy <%s>"), srcPath);
    FUN_140025f10(ctx + 8, logBuf, 0, (void *)0x11, NULL, NULL);
    PECMD_FreeStrBuf(&logBuf);
    CopyFileW(srcPath, infDir, 0);
    GetLastError();
    if ((flags & 0x30000) != 0) {
        PECMD_AllocWStringBuffer((WCHAR **)&logBuf, 0x2800);
        _snwprintf(logBuf, 0x27ff, WSTR("ToSys:TreatINF <%s>"), infDir);
        FUN_140025f10(ctx + 8, logBuf, 0, (void *)0x11, NULL, NULL);
        PECMD_FreeStrBuf(&logBuf);
        PECMD_PatchInfDirectives(infDir);
    }
    PECMD_AllocWStringBuffer((WCHAR **)&logBuf, 0x2800);
    _snwprintf(logBuf, 0x27ff, WSTR("ToSys:END <%s>"), srcPath);
    FUN_140025f10(ctx + 8, logBuf, 0, (void *)0x11, NULL, NULL);
    PECMD_FreeStrBuf(&logBuf);

    memcpy(fileName, (const void *)0x140120dd8, 8);
    memset(&fd, 0, sizeof(fd));
    PECMD_FindFirstFileW(&hFind, buf, &fd);
    if (hFind != (HANDLE)0) {
        do {
            if (((fd.dwFileAttributes & 0x10) == 0) &&
                ((fd.cFileName[0] != L'.' ||
                  ((fd.cFileName[1] != L'\0' &&
                    ((fd.cFileName[1] != L'.' || (fd.cFileName[2] != L'\0')))))))) {
                int nameLen = lstrlenW(fd.cFileName);
                WCHAR *dstDir;
                int dstBaseLen;
                memcpy(srcDir, fd.cFileName, (nameLen + 1) * 2);
                {
                    int64_t lv = PECMD_WideStrLen(fd.cFileName);
                    int extStart = (int)lv;
                    if (extStart < 4) {
                        extStart = 4;
                    }
                    if (lstrcmpiW(fd.cFileName + extStart - 4, WSTR(".INF")) == 0) {
                        dstDir = infDir;
                        dstBaseLen = infLen;
                    }
                    else {
                        dstDir = buf + 0x14a0;
                        dstBaseLen = sysLen;
                    }
                    if (lstrcmpiW(fd.cFileName + extStart - 4, WSTR(".SYS")) == 0) {
                        dstDir = buf + 0xf78;
                        dstBaseLen = drvLen;
                    }
                    lstrcpyW(dstDir + dstBaseLen, fd.cFileName);
                }
                PECMD_AllocWStringBuffer((WCHAR **)&logBuf, 0x2800);
                _snwprintf(logBuf, 0x27ff, WSTR("ToSys:Copy <%s>"), srcDir);
                FUN_140025f10(ctx + 8, logBuf, 0, (void *)0x11, NULL, NULL);
                PECMD_FreeStrBuf(&logBuf);
                CopyFileW(srcDir, dstDir, 0);
                if ((infDir == dstDir) && ((flags & 0x30000) != 0)) {
                    PECMD_AllocWStringBuffer((WCHAR **)&logBuf, 0x2800);
                    _snwprintf(logBuf, 0x27ff, WSTR("ToSys:TreatINF <%s>"), infDir);
                    FUN_140025f10(ctx + 8, logBuf, 0, (void *)0x11, NULL, NULL);
                    PECMD_FreeStrBuf(&logBuf);
                    PECMD_PatchInfDirectives(infDir);
                }
                PECMD_AllocWStringBuffer((WCHAR **)&logBuf, 0x2800);
                _snwprintf(logBuf, 0x27ff, WSTR("ToSys:End <%s>"), srcDir);
                FUN_140025f10(ctx + 8, logBuf, 0, (void *)0x11, NULL, NULL);
                PECMD_FreeStrBuf(&logBuf);
                fd.cFileName[2] = L'\0';
            }
        } while (FindNextFileW(hFind, &fd) != 0);
        if ((hFind != (HANDLE)0) && (hFind != (HANDLE)(uintptr_t)-1)) {
            FindClose(hFind);
        }
    }
    err = 0;
done:
    PECMD_FreeStrBuf(&buf);
    return err;
}

/* ========== PECMD_EvalSpecialToken @0x14002d33c ==========
 * 解析日期/时间表达式，支持 -mode、?、$、#、&&&、@、*、-std/-add 等。
 * TODO(verify): 反编译中 FILETIME 被用作 64 位字符串指针容器，已改为 WCHAR*。
 */
LARGE_INTEGER PECMD_EvalSpecialToken(LARGE_INTEGER script, LPCWSTR arg, char mode, LPCWSTR arg2,
                                     int64_t param5)
{
    WCHAR *p = (WCHAR *)arg;
    WCHAR *p2 = (WCHAR *)arg2;
    WCHAR *outStr = NULL;
    char cVar1 = 0;
    uint8_t bVar3 = 0;
    LARGE_INTEGER result;

    while ((*p != 0) && (((uint16_t)*p < 9 || (0xd < (uint16_t)*p)) && (*p != 0x20))) {
        p = p + 1;
    }
    p = p + 0; /* original skip-space call */
    {
        WCHAR **pp = &p;
        PECMD_SkipLeadingControls(pp);
        p = *pp;
    }
    while ((*p2 != 0) && (((uint16_t)*p2 < 9 || (0xd < (uint16_t)*p2)) && (*p2 != 0x20))) {
        p2 = p2 + 1;
    }
    {
        WCHAR **pp = &p2;
        PECMD_SkipLeadingControls(pp);
        p2 = *pp;
    }

    /* dc:27047 (longlong)p+2 处 ushort = p[1] 取下一字符再 |0x20 小写化 == 'm';
     * 旧移植 (*p+1) 误用当前字符('-'=0x2d→0x2e) → 分支恒死 (R25-f §4b/D-19) */
    if ((*arg == L'^') || ((*p == L'-') && ((uint16_t)p[1] | 0x20) == 0x6d &&
                           (FUN_1400660AC("-mode", (int64_t *)&p, 5) != 0))) {
        cVar1 = PECMD_ParseEnvSwitches(p, (int64_t *)script.QuadPart, 0x26);
        result.QuadPart = 0;
        result.LowPart = (cVar1 == '\0');
        return result;
    }

    if (*p == L'?') {
        POINT pt = PECMD_EvalQueryValue((int64_t *)script.QuadPart, (uint64_t)(uintptr_t)(p + 1),
                                        param5, mode);
        result.LowPart = (DWORD)pt.x;
        result.HighPart = pt.y;
        return result;
    }

    if ((*p == L'$') || (*p == L'#')) {
        cVar1 = *(char *)p;
    }
    else if ((*p == L'&') && (p[1] == L'&') && (p[2] == L'&')) {
        if (PECMD_ParseEnvSwitches(p, (int64_t *)script.QuadPart, 0) != 0) {
            result.QuadPart = 0;
            return result;
        }
    }

    if (*p2 == L'@') {
        return PECMD_EvalAtCommand(script, PECMD_LI((intptr_t)(p2 + 1)), (uint16_t *)arg2,
                                   param5);
    }

    if ((*p == L'*') && (p[1] == 0)) {
        PECMD_EnumCDRomDrives((int64_t *)script.QuadPart);
        result.QuadPart = 0;
        return result;
    }

    outStr = NULL;
    if (*arg == L'-') {
        if (p2[0] == L's') {
            if ((p2[1] == L't') && (p2[2] == L'd')) {
                goto std_path;
            }
        }
        else if (((*p == L';') && (p[1] == L'&')) ||
                 ((((uint16_t)p2[0] | 0x20) == 0x61) &&
                  ((((uint16_t)p2[1] | 0x20) == 100) &&
                   ((((uint16_t)p2[2] | 0x20) == 100) && ((uint16_t)p2[3] | 0x20) == 0x72)))) {
            goto add_path;
        }
        goto not_std;
    }

std_path:
    FUN_14007A224((void *)script.QuadPart, (WCHAR *)arg2, &outStr, 0, bVar3 | 1);
    {
        WCHAR *q = outStr;
        while ((*q != 0) && (((uint16_t)*q < 9 || (0xd < (uint16_t)*q)) && (*q != 0x20))) {
            q = q + 1;
        }
        {
            WCHAR **pp = &q;
            PECMD_SkipLeadingControls(pp);
            q = *pp;
        }
        if ((cVar1 != 0) || ((mode == '\0') && (*q != L'&'))) {
            result.QuadPart =
                (int64_t)PECMD_ExpandSpecialDirs((int64_t *)script.QuadPart, q + (cVar1 != 0),
                                                 (int)cVar1, (int)cVar1, (WCHAR *)arg2);
            goto done;
        }
        goto call_c6324;
    }

add_path:
    bVar3 = 0x40;
    goto std_path;

not_std:
    if (cVar1 != 0) {
        goto direct;
    }
    if ((*(char *)(script.QuadPart + 0xd) != '\0') || (*arg == L'?')) {
        if ((*arg == L'-') && (arg[1] == L'e') && (arg[2] == L'n') && (arg[3] == L'v')) {
            bVar3 = 0x40;
            goto std_path;
        }
        if (*p != L'^') {
            goto call_c6324;
        }
    }
    if ((mode == '\0') && (*p != L'&')) {
    direct:
        result.QuadPart = (int64_t)PECMD_ExpandSpecialDirs(
            (int64_t *)script.QuadPart, p + (cVar1 != 0), (int)cVar1, (int)cVar1, (WCHAR *)arg2);
        return result;
    }

call_c6324:
    result.QuadPart =
        (int64_t)PECMD_EnviMemReadWrite(script.QuadPart, (void *)p, 0, (WCHAR *)arg2, mode);
done:
    PECMD_FreeStrBuf(&outStr);
    return result;
}

/* ========== PECMD_DispConfirmPopupMenu @0x14002e790 ==========
 * 在光标位置弹出显示器分辨率菜单，选择后执行对应的 DISP 命令。
 */
void PECMD_DispConfirmPopupMenu(HWND hwnd)
{
    HMENU menu;
    HMENU subMenu;
    HMENU currentMenu;
    int64_t modes = 0;
    int64_t modeCap = 0;
    WCHAR *titleBuf = NULL;
    WCHAR *title = NULL;
    int selCount[2] = {0, 0};
    UINT baseCmd = 0;
    WCHAR *cmdBuf = NULL;
    LPCWSTR fmt = WSTR("分辨率 %d×%d 颜色%d位 刷新率%d");
    POINT pt;
    int screenW;
    int perCol;
    int screenH;
    int dpi;
    int curModeCount;
    int curH = 0, curB = 0, curR = 0;
    int modeCount = 0;
    uint32_t currentWidth = 0;
    UINT uVar4;
    WCHAR *pCmd;

    menu = LoadMenuW(g_hInstance, (LPCWSTR)0x2725);
    menu = (HMENU)GetSubMenu(menu, 0);
    subMenu = menu;
    currentMenu = menu;
    PECMD_InitPtrTable(&modes);
    PECMD_AllocWStringBuffer((WCHAR **)&titleBuf, 0x200);
    title = titleBuf;
    selCount[0] = 0;
    baseCmd = PECMD_ScanMenuRecursive(menu, &modes, selCount, titleBuf, 30000);
    PECMD_AllocWStringBuffer((WCHAR **)&cmdBuf, 0x4c8);
    cmdBuf[100] = L'\0';
    pCmd = cmdBuf;
    {
        LPCWSTR res = FUN_1400169BC(22000, NULL);
        if (res != NULL) {
            fmt = res;
        }
    }
    pt.x = 0;
    pt.y = 0;
    GetCursorPos(&pt);
    screenW = GetSystemMetrics(0);
    perCol = (screenW - 5) / 0xbe;
    screenH = GetSystemMetrics(1);
    dpi = GetSystemMetrics(0xf);
    curModeCount = (int)PECMD_EnumDisplayModes((void **)&modes, 100, 1, NULL);
    modeCount = curModeCount;
    if (0 < curModeCount) {
        uint8_t *mode = (uint8_t *)modes;
        int n = curModeCount;
        curH = 0;
        do {
            int h = *(int *)(mode + 0xb0);
            int w = *(int *)(mode + 0xac);
            int r = *(int *)(mode + 0xb8);
            if (((h == 600) && (w == 800)) ||
                ((h == 0x1e0) && ((w == 0x280) && (currentWidth != 800)))) {
                int score = r - 0x3c;
                int curScore = curR - 0x3c;
                char better = (int)currentWidth < (int)w;
                if (*(int *)(mode + 0xa8) == 0x10) {
                    better = 2;
                }
                int a = -score;
                if (score >= 0) {
                    a = score * 2;
                }
                int b = -curScore;
                if (curScore >= 0) {
                    b = curScore * 2;
                }
                if (a < b) {
                    better = 4;
                }
                if (better != 0) {
                    currentWidth = w;
                    curB = *(int *)(mode + 0xa8);
                    curH = h;
                    curR = r;
                }
            }
            n--;
            mode += 0xdc;
        } while (n != 0);
        title = titleBuf;
    }

    {
        int rows = screenH / dpi;
        int available = perCol * rows - (selCount[0] - 1);
        if ((curModeCount < 10) && (curModeCount < available)) {
            baseCmd = baseCmd - 1;
            RemoveMenu(subMenu, baseCmd, 0x400);
        }
        else {
            currentMenu = (HMENU)GetSubMenu(subMenu, baseCmd - 1);
            baseCmd = 0;
            RemoveMenu(currentMenu, 0, 0x400);
            selCount[0] = 0;
        }
    }

    {
        int visible = (screenH / dpi) * perCol - selCount[0];
        if (((screenW < 800) && (visible < curModeCount)) ||
            ((screenH / dpi) * perCol - selCount[0] < curModeCount)) {
            dpi = curModeCount + 1000;
        }
        else if (curModeCount <= visible) {
            /* keep */
        }
        else {
            dpi = (screenH / dpi) * perCol - selCount[0];
            pt.y = 0;
        }
    }

    if (0 < modeCount) {
        uint16_t cmdId = 22000;
        uint8_t *mode = (uint8_t *)modes;
        UINT pos = baseCmd;
        int n = modeCount;
        do {
            LPCWSTR cmdFmt = WSTR("#,DISP -confirm W%d H%d B%d");
            if (*(int *)(mode + 0xb8) > 1) {
                cmdFmt = WSTR("#,EXEC -incmd PECMD DISP -confirm W%d H%d B%d F%d");
            }
            wsprintfW(cmdBuf + 200, cmdFmt, *(int *)(mode + 0xac), *(int *)(mode + 0xb0));
            wsprintfW(cmdBuf, fmt, *(int *)(mode + 0xac), *(int *)(mode + 0xb0));
            {
                void **item = (void **)calloc(1, 8);
                if (item != NULL) {
                    FUN_1400702B0((WCHAR **)item, cmdBuf + 200);
                }
                {
                    int64_t idx = selCount[0];
                    selCount[0] = selCount[0] + 1;
                    void **slot = (void **)FUN_140063B00(idx, &modes, &modeCap, 8);
                    if (slot != NULL) {
                        *slot = item;
                    }
                }
                baseCmd = baseCmd + 1;
                if (item != NULL) {
                    *(uint16_t *)*item = cmdId;
                }
                {
                    uint32_t menuFlags = 0;
                    if (dpi <= (int)baseCmd) {
                        menuFlags = 0x20;
                    }
                    if (menuFlags != 0) {
                        baseCmd = 0;
                    }
                    InsertMenuW(currentMenu, pos, menuFlags | 0x400, (uintptr_t)cmdId, cmdBuf);
                }
                pos = pos + 1;
                cmdId = cmdId + 1;
            }
            n--;
            mode += 0xdc;
        } while (n != 0);
    }

    PECMD_FreeStrBuf((WCHAR **)&modes);
    if ((int)currentWidth == 0) {
        currentWidth = 800;
        curH = 600;
        curB = 0x10;
    }
    wsprintfW(cmdBuf + 100, WSTR("#,EXEC -incmd PECMD DISP -confirm -reset W%d H%d B%d F%d"),
              currentWidth, curH, curB);
    SetForegroundWindow(hwnd);
    uVar4 = TrackPopupMenu(menu, 0x128, pt.x, pt.y, 0, hwnd, NULL);
    PostMessageW(hwnd, 0, 0, 0);
    *title = L'\0';
    pCmd = title;
    {
        int totalItems = selCount[0];
        if ((uVar4 != 0) && (pCmd = cmdBuf + 100, uVar4 != 0x799d) && (0 < totalItems)) {
            int k;
            for (k = 0; k < totalItems; k++) {
                void **slot = (void **)FUN_140063B00(k, &modes, &modeCap, 8);
                if (slot != NULL && uVar4 == **(uint16_t **)*slot) {
                    void **slot2 = (void **)FUN_140063B00(k, &modes, &modeCap, 8);
                    if (slot2 != NULL) {
                        pCmd = (WCHAR *)(*(uint16_t **)*slot2 + 1);
                    }
                    break;
                }
            }
        }
    }
    {
        WCHAR *comma = StrChrW(pCmd, L',');
        if (comma != NULL) {
            PECMD_ProcessScriptBlock((uint64_t)(uintptr_t)g_Script,
                                     (uint64_t)(uintptr_t)(comma + 1), NULL, NULL, NULL);
        }
    }
    PECMD_FreeStrBuf(&cmdBuf);
    PECMD_FreeStrBuf(&titleBuf);
    PECMD_FreeArray_ddf8(&modes);
    PECMD_FreeStrBuf((WCHAR **)&modes);
}

/* ========== FUN_14002FD88 @0x14002fd88 ==========
 * 解析命令路径/参数，支持 ::、冒号分隔符和命令字匹配。
 * TODO(verify): stateFlags 原为 Ghidra 中 local_res8 指针变量的低 32 位。
 */
uint16_t *FUN_14002FD88(int64_t *ctx, WCHAR *text, uint32_t *flags, int64_t *out)
{
    uint16_t *result = NULL;
    WCHAR *expanded = NULL;
    WCHAR *argBuf = NULL;
    WCHAR *cursor;
    WCHAR *start;
    WCHAR *p;
    WCHAR *p2;
    WCHAR *p9;
    uint32_t stateFlags = 0;
    uint16_t uVar4;
    uint16_t *cmdWord;
    uint16_t *tail = NULL;
    uint16_t *last = NULL;

    *flags = 0x80000000;
    if (ctx[0x10] == 0) {
        return NULL;
    }
    PECMD_AllocStrSlot(&expanded);
    if ((*(char *)((uint8_t *)ctx + 0xda) == '\0') &&
        ((*(uint8_t *)((uint8_t *)ctx + 0xd) & 0xf) == 0)) {
        FUN_14007BDA8(ctx, text, &expanded, 0, 1);
    }
    else {
        FUN_14007A224(ctx, text, &expanded, 0, 1);
    }
    tail = (uint16_t *)ctx[0x10];
    cursor = expanded;
    PECMD_SkipLeadingControls(&cursor);
    start = cursor;
    p = cursor;
    while ((*p != L'\0') && ((((uint16_t)*p < 9 || (0xd < (uint16_t)*p)) && (*p != L' ')))) {
        if (((*p == L'\\') || (*p == L'/')) || (*p == L'=') || (*p == L'*')) {
            goto cleanup;
        }
        p = p + 1;
    }
    PECMD_AllocStrSlot(&argBuf);
    FUN_14007443C(start, (int64_t *)&argBuf);
    stateFlags = *(uint32_t *)((uint8_t *)ctx + 0x24);
    last = NULL;
    if ((*argBuf == L':') && (argBuf[1] == L':')) {
        argBuf = argBuf + 2;
        stateFlags = 0;
        *out = 0;
    }
    else if ((0 < *(int *)((uint8_t *)ctx + 0x24)) && (ctx[5] != 0)) {
        uVar4 = *argBuf;
        p2 = argBuf;
        while ((uVar4 != 0) && (*p2 != L':')) {
            p2 = p2 + 1;
            uVar4 = *p2;
        }
        uVar4 = *p2;
        *p2 = 0;
        {
            uint16_t *pAfterColon = p2;
            do {
                pAfterColon = pAfterColon + 1;
            } while (*pAfterColon == L':');
            *flags = *(uint32_t *)((uint8_t *)ctx + 0x24);
            cmdWord = PECMD_GetNextCommandArg((int64_t)ctx, argBuf, (uint16_t *)ctx[5], flags,
                                              (uint16_t *)ctx[6]);
            if (cmdWord == NULL) {
                stateFlags = stateFlags & 0xffffffff00000000ULL;
                *flags = 0xffffffff;
                *out = 0;
            }
            else {
                argBuf = pAfterColon;
                last = NULL;
                if (*pAfterColon == 0) {
                    tail = cmdWord;
                    last = cmdWord;
                }
            }
            *p2 = uVar4;
            if (last != NULL) {
                goto done;
            }
        }
    }
    if ((int)*flags < 0) {
        *flags = 0xffffffff;
    }
    do {
        uVar4 = *argBuf;
        p9 = argBuf;
        while (uVar4 != 0) {
            if (uVar4 == L':') {
                goto found_colon;
            }
            p9 = p9 + 1;
            uVar4 = *p9;
        }
        if (*p9 == L':') {
        found_colon:
            if ((p9[1] != L':') || (p9[2] == L':')) {
                break;
            }
            for (; *p9 == L':'; p9 = p9 + 1) {
                *p9 = 0;
            }
        }
        cmdWord = PECMD_GetNextCommandArg((int64_t)ctx, argBuf, tail, &stateFlags, NULL);
        {
            uint32_t newState = stateFlags + 1;
            stateFlags = newState;
            if (cmdWord != NULL) {
                *flags = (newState & 0x40000000) | (*flags + 1);
                while ((uVar4 = *cmdWord, *(uint16_t *)((uint8_t *)ctx + 0x92) == uVar4 ||
                                              (*(uint16_t *)((uint8_t *)ctx + 0x94) == uVar4) ||
                                              (*(uint16_t *)((uint8_t *)ctx + 0x8a) == uVar4) ||
                                              (*(uint16_t *)((uint8_t *)ctx + 0x90) == uVar4))) {
                    cmdWord = cmdWord + 1;
                }
                if (*cmdWord != *(uint16_t *)((uint8_t *)ctx + 0x88)) {
                    cmdWord = cmdWord + 1;
                }
            }
            if ((*out == 0) && (cmdWord != NULL)) {
                *out = (int64_t)(uintptr_t)cmdWord;
                while (*(uint16_t *)((uint8_t *)ctx + 0x88) != *cmdWord) {
                    int16_t sVar1 = *(int16_t *)(uintptr_t)*out;
                    if ((*(int16_t *)((uint8_t *)ctx + 0x8a) == sVar1) ||
                        (*(int16_t *)((uint8_t *)ctx + 0x90) == sVar1)) {
                        break;
                    }
                    *out = (int64_t)(uintptr_t)((uint16_t *)(uintptr_t)*out + 1);
                    uVar4 = *(uint16_t *)(uintptr_t)*out;
                }
                while (1) {
                    int16_t sVar1 = *(int16_t *)(uintptr_t)*out;
                    if ((((*(int16_t *)((uint8_t *)ctx + 0x92) != sVar1) &&
                          (*(int16_t *)((uint8_t *)ctx + 0x94) != sVar1)) &&
                         (*(int16_t *)((uint8_t *)ctx + 0x8a) != sVar1)) &&
                        (*(int16_t *)((uint8_t *)ctx + 0x90) != sVar1)) {
                        break;
                    }
                    *out = (int64_t)(uintptr_t)((uint16_t *)(uintptr_t)*out + 1);
                }
            }
        }
        if (*p9 == 0) {
            result = cmdWord;
        }
        if ((*p9 == 0) || ((argBuf = p9, cmdWord == NULL))) {
            break;
        }
    } while (1);

    if (result == NULL) {
        *flags = 0x80000000;
    }
done:
    PECMD_FreeStrBuf(&argBuf);
cleanup:
    PECMD_FreeStrBuf(&expanded);
    return result;
}

/* ========== PECMD_MountImDiskRamDisk @0x140035cec ==========
 * 向 ImDisk 驱动发送 IOCTL 控制请求，可挂载/创建虚拟磁盘。
 * TODO(verify): 函数指针全局符号按 void* 存储，调用处用局部函数指针还原。
 */
uint32_t PECMD_MountImDiskRamDisk(uint32_t *unit, uint64_t *data1, uint64_t *data2, uint32_t flags,
                                  LPCWSTR path, int mode, WCHAR *drive, LPCWSTR mountPoint,
                                  uint32_t opts)
{
    PECMD_UNICODE_STRING uni;
    PECMD_UNICODE_STRING uni2;
    WCHAR *tmpStr = NULL;
    uint8_t *ioBuf = NULL;
    HANDLE hObject;
    HANDLE hFile = (HANDLE)0;
    DWORD bytesRet[2] = {0, 0};
    DWORD err;
    uint32_t uVar7;
    uint32_t uVar10;
    int i;
    int status;
    uint32_t ret = 0xffffffff;
    WCHAR devName[260];
    WCHAR wc;
    LPCWSTR prefix;
    int (*pRtlCreate)(PECMD_UNICODE_STRING *, LPCWSTR);
    int (*pRtlFree)(PECMD_UNICODE_STRING *);
    int (*pRtlInit)(PECMD_UNICODE_STRING *, LPCWSTR);
    int (*pRtlDos)(LPCWSTR, PECMD_UNICODE_STRING *, void *, void *);
    HANDLE (*pOpen)(void *, DWORD);
    int (*pLoadDrv)(LPCWSTR);
    void (*pClose)(HANDLE);
    int (*pDefine)(LPCWSTR, LPCWSTR);

    memset(&uni, 0, sizeof(uni));
    memset(&uni2, 0, sizeof(uni2));
    PECMD_GetApiProcCached("RtlCreateUnicodeString", "NTDLL.DLL",
                           (int64_t *)(void **)&g_pRtlCreateUnicodeString, NULL);
    PECMD_GetApiProcCached("RtlFreeUnicodeString", "NTDLL.DLL",
                           (int64_t *)(void **)&g_pRtlFreeUnicodeString, NULL);
    if (g_pRtlInitUnicodeString == NULL) {
        return 3;
    }
    if ((((opts & 0x10) != 0) && (drive != NULL)) &&
        ((wc = *drive, 0x40 < (uint16_t)wc) && ((uint16_t)wc < 0x5b))) {
        *drive = (WCHAR)(wc | 0x20);
    }
    *(void **)&pRtlInit = g_pRtlInitUnicodeString;
    pRtlInit(&uni, WSTR("\\Device\\ImDiskCtl"));
    *(void **)&pOpen = g_pImDiskOpenDevice;
    hObject = pOpen(&uni, 0xc0000000);
    ret = 0xffffffff;
    while (hObject == (HANDLE)(uintptr_t)-1) {
        err = GetLastError();
        if (err != 2) {
            return 6;
        }
        *(void **)&pLoadDrv = g_pImDiskStartService;
        if (pLoadDrv(WSTR("ImDisk")) == 0) {
            GetLastError();
            return 4;
        }
        Sleep(0);
        hObject = pOpen(&uni, 0xc0000000);
    }
    if (!PECMD_QueryDeviceControlState(hObject)) {
        ret = 5;
        goto close_out;
    }

    uVar10 = flags & 0xf000;
    uVar7 = flags & 0xf00;
    if ((uVar10 != 0x1000) || (uVar7 != 0x100 && uVar7 != 0)) {
        if ((uVar7 == 0x300 && (uVar10 == 0x2000 || uVar10 == 0x1000)) &&
            (WaitNamedPipeW(WSTR("\\\\.\\PIPE\\ImDskSvc"), 0) == 0) &&
            ((err = GetLastError()) == 2)) {
            *(void **)&pLoadDrv = g_pImDiskStartService;
            if (pLoadDrv(WSTR("ImDskSvc")) == 0) {
                GetLastError();
                ret = 7;
                goto close_out;
            }
            while ((WaitNamedPipeW(WSTR("\\\\.\\PIPE\\ImDskSvc"), 0) == 0 &&
                    ((err = GetLastError()) == 2))) {
                Sleep(200);
            }
        }
    }
    else {
        *(void **)&pRtlInit = g_pRtlInitUnicodeString;
        pRtlInit(&uni2, WSTR("\\Device\\AWEAlloc"));
        *(void **)&pOpen = g_pImDiskOpenDevice;
        hFile = pOpen(&uni2, 0xc0000000);
        while (hFile == (HANDLE)(uintptr_t)-1) {
            err = GetLastError();
            if (err != 2) {
                goto after_awe;
            }
            *(void **)&pLoadDrv = g_pImDiskStartService;
            if (pLoadDrv(WSTR("AWEAlloc")) == 0) {
                GetLastError();
                ret = 7;
                goto close_out;
            }
            puts("AWEAlloc driver was loaded into the kernel.");
            hFile = pOpen(&uni2, 0xc0000000);
        }
        *(void **)&pClose = g_pNtClose;
        pClose(hFile);
    }
after_awe:
    if (path == NULL) {
        *(void **)&pRtlInit = g_pRtlInitUnicodeString;
        pRtlInit(&uni, NULL);
    }
    else {
        if (mode == 0) {
            if ((uVar7 == 0x300) && (uVar10 == 0x3000)) {
                hFile = CreateFileW(WSTR("\\\\?\\Global"), 0, 1, NULL, 3, 0x80, (HANDLE)0);
                err = GetLastError();
                prefix = WSTR("\\BaseNamedObjects\\");
                if ((hFile != (HANDLE)(uintptr_t)-1) || (err != 2)) {
                    prefix = WSTR("\\BaseNamedObjects\\Global\\");
                }
                if (hFile != (HANDLE)(uintptr_t)-1) {
                    CloseHandle(hFile);
                }
                i = lstrlenW(prefix);
                status = lstrlenW(path);
                PECMD_AllocWStringBuffer((WCHAR **)&tmpStr, (int64_t)(status + 2 + i));
                if (tmpStr != NULL) {
                    lstrcpyW(tmpStr, prefix);
                    lstrcatW(tmpStr, path);
                    *(void **)&pRtlCreate = g_pRtlCreateUnicodeString;
                    if (pRtlCreate(&uni, tmpStr) != 0) {
                        PECMD_FreeStrBuf(&tmpStr);
                        goto io_ready;
                    }
                }
                CloseHandle(hObject);
                PECMD_FreeStrBuf(&tmpStr);
                return 0xffffffff;
            }
            PECMD_GetApiProcCached("RtlDosPathNameToNtPathName_U", "NTDLL.DLL",
                                   (int64_t *)(void **)&g_pRtlDosPathNameToNtPathName_U, NULL);
            *(void **)&pRtlDos = g_pRtlDosPathNameToNtPathName_U;
            status = pRtlDos(path, &uni, NULL, NULL);
        }
        else {
            *(void **)&pRtlCreate = g_pRtlCreateUnicodeString;
            status = pRtlCreate(&uni, path);
        }
        if (status == 0) {
            goto close_out;
        }
    }
io_ready:
    FUN_1400633A8((void **)&ioBuf, (int64_t)((uint32_t)uni.Length * 2 + 0x38));
    memset(ioBuf, 0, (size_t)uni.Length + 0x38);
    if ((drive != NULL) && ((opts & 0x10) == 0) &&
        (((i = lstrlenW(drive), i == 2 && (drive[1] == L':')) ||
          ((i == 3 && (drive[1] == L':')) && (drive[2] == L'\\'))))) {
        *(WCHAR *)(ioBuf + 0x2c) = *drive;
    }
    *(uint32_t *)ioBuf = *unit;
    *(uint64_t *)(ioBuf + 8) = data1[0];
    *(uint64_t *)(ioBuf + 0x10) = data1[1];
    *(uint64_t *)(ioBuf + 0x18) = data1[2];
    *(uint64_t *)(ioBuf + 0x20) = *data2;
    *(uint32_t *)(ioBuf + 0x28) = flags;
    *(uint16_t *)(ioBuf + 0x2e) = uni.Length;
    if (uni.Length != 0) {
        memcpy(ioBuf + 0x30, uni.Buffer, (size_t)uni.Length);
        *(void **)&pRtlFree = g_pRtlFreeUnicodeString;
        pRtlFree(&uni);
    }
    {
        DWORD ioLen = *(uint16_t *)(ioBuf + 0x2e) + 0x38;
        BOOL ok = DeviceIoControl(hObject, 0x8372e004, ioBuf, ioLen, ioBuf, ioLen, bytesRet, NULL);
        if (ok == 0) {
            CloseHandle(hObject);
            PECMD_FreeStrBuf((WCHAR **)&ioBuf);
            return 3;
        }
    }
    CloseHandle(hObject);
    *unit = *(uint32_t *)ioBuf;
    if (drive != NULL) {
        devName[0] = L'\0';
        _snwprintf(devName, 0x103, WSTR("\\Device\\ImDisk%u"), (uint32_t)*unit);
        if (*(int16_t *)(ioBuf + 0x2c) == 0) {
            *(void **)&pDefine = g_pImDiskCreateMountPoint;
            if (pDefine(drive, devName) == 0) {
                err = GetLastError();
                if ((((err != 1) && (err != 0x57)) &&
                     ((err != 0x91 && (err != 0x10b && (err != 0x1126))))) &&
                    (err == 0x1128)) {
                    FUN_140103a20();
                    FUN_14001c82c();
                }
                *drive = L'\0';
            }
        }
        else {
            PECMD_DosDeviceMount(devName, drive, NULL, 1, '\0');
        }
    }
    if (mountPoint != NULL) {
        PECMD_FormatImDiskDrive((uint32_t)*unit, drive, mountPoint);
    }
    PECMD_FreeStrBuf((WCHAR **)&ioBuf);
    return 0;

close_out:
    CloseHandle(hObject);
    return ret;
}

/* ========== PECMD_DismountRamDiskDrive @0x14003634c ==========
 * ImDisk 磁盘移除/卸载：先锁定/卸载设备，再删除 DOS 设备名。
 * TODO(verify): 设备路径模板与常量按语义简化为 \\.\X: 形式。
 */
uint64_t PECMD_DismountRamDiskDrive(uint32_t unit, WCHAR *drive, int mode, int stop, int count,
                                    uint32_t opts)
{
    uint8_t *buf = NULL;
    HANDLE hDevice;
    DWORD bytesRet[2] = {0, 0};
    DWORD err;
    BOOL ok;
    int i;
    uint64_t ret;
    WCHAR driveLetter;
    WCHAR devPath[8];
    HANDLE (*pOpenDev1)(uint32_t, DWORD);
    HANDLE (*pOpenDev2)(LPCWSTR, DWORD);
    int (*pStopDev)(HANDLE, int);
    int (*pDeleteDev)(LPCWSTR);

    if ((((opts & 0x10) != 0) && (drive != NULL)) &&
        ((driveLetter = *drive, 0x40 < (uint16_t)driveLetter) && ((uint16_t)driveLetter < 0x5b))) {
        *drive = (WCHAR)(driveLetter | 0x20);
    }

    if (stop != 0) {
        *(void **)&pStopDev = g_pImDiskForceRemove;
        if (pStopDev(0, (int)unit) == 0) {
            return 2;
        }
        goto cleanup_dos;
    }

    FUN_1400633A8((void **)&buf, 0x448);
    if (drive == NULL) {
        *(void **)&pOpenDev1 = g_pDevOpen;
        hDevice = pOpenDev1(unit, 0xc0000000);
        if ((hDevice != (HANDLE)(uintptr_t)-1) ||
            (hDevice = pOpenDev1(unit, 0x80000000), hDevice != (HANDLE)(uintptr_t)-1)) {
            goto opened;
        }
        hDevice = pOpenDev1(unit, 0x80);
        goto open_retry;
    }
    else {
        i = lstrlenW(drive);
        if (i == 2) {
            ok = drive[1] == L':';
        }
        else if (i == 3) {
            ok = lstrcmpW(drive + 1, WSTR(":\\")) == 0;
        }
        else {
            ok = 0;
        }
        if (ok) {
            driveLetter = *drive;
            wsprintfW(devPath, WSTR("\\\\.\\%c:"), driveLetter);
            if (((opts & 0x10) == 0) && (0x40 < (uint16_t)driveLetter) &&
                ((uint16_t)driveLetter < 0x5b) && ((opts & 0x11) == 0)) {
                FUN_140035B40((uint32_t)(uint16_t)driveLetter, 0, count);
            }
            hDevice = CreateFileW(devPath, 0xc0000000, 3, NULL, 3, 0x20000000, (HANDLE)0);
            if ((hDevice == (HANDLE)(uintptr_t)-1) &&
                (hDevice = CreateFileW(devPath, 0x80000000, 3, NULL, 3, 0x20000000, (HANDLE)0),
                 hDevice == (HANDLE)(uintptr_t)-1)) {
                hDevice = CreateFileW(devPath, 0x80, 3, NULL, 3, 0x20000000, (HANDLE)0);
                goto open_retry;
            }
            goto opened;
        }
        *(void **)&pOpenDev2 = g_pDevOpen2;
        hDevice = pOpenDev2(drive, 0xc0000000);
        if ((hDevice == (HANDLE)(uintptr_t)-1) &&
            (hDevice = pOpenDev2(drive, 0x80000000), hDevice == (HANDLE)(uintptr_t)-1)) {
            hDevice = pOpenDev2(drive, 0x80);
            if (hDevice == (HANDLE)(uintptr_t)-1) {
                err = GetLastError();
                if ((err == 1) || (err == 0x57)) {
                    goto free_ret9;
                }
                if ((err == 0x91) || (err == 0x10b) || (err == 0x1126)) {
                    FUN_140103a20();
                    FUN_14001c82c();
                    goto free_ret9;
                }
                ret = 9;
                goto cleanup_out;
            }
            goto opened;
        }
        goto opened;
    }

open_retry:
    if (hDevice != (HANDLE)(uintptr_t)-1) {
        goto opened;
    }
    err = GetLastError();
    if (err == 2) {
        ret = 1;
        goto cleanup_out;
    }
    ret = 9;
    goto cleanup_out;

opened:
    if (!PECMD_QueryDeviceControlState(hDevice)) {
        CloseHandle(hDevice);
        ret = 5;
        goto cleanup_out;
    }
    ok = DeviceIoControl(hDevice, 0x83722008, NULL, 0, buf, 0x448, bytesRet, NULL);
    if ((ok == 0) || (bytesRet[0] < 0x36)) {
        FUN_140103a20();
        FUN_14001c82c();
        goto ret2;
    }
    {
        int16_t sVar2 = *(int16_t *)(buf + 0x2c);
        if ((drive == NULL) && (sVar2 != 0)) {
            driveLetter = (WCHAR)sVar2;
            drive = &driveLetter;
        }
    }
    FlushFileBuffers(hDevice);
    ok = DeviceIoControl(hDevice, 0x90018, NULL, 0, NULL, 0, bytesRet, NULL);
    if (ok == 0) {
        if (mode != 0) {
            DeviceIoControl(hDevice, 0x90020, NULL, 0, NULL, 0, bytesRet, NULL);
            DeviceIoControl(hDevice, 0x90018, NULL, 0, NULL, 0, bytesRet, NULL);
            goto do_unmount;
        }
    }
    else {
        ok = DeviceIoControl(hDevice, 0x90020, NULL, 0, NULL, 0, bytesRet, NULL);
        if (ok != 0) {
            goto do_unmount;
        }
    }
    goto ret2;

do_unmount:
    ok = DeviceIoControl(hDevice, 0x2d4808, NULL, 0, NULL, 0, bytesRet, NULL);
    if (ok == 0) {
        if (mode != 0) {
            *(void **)&pStopDev = g_pImDiskForceRemove;
            ok = (pStopDev((HANDLE)hDevice, 0) == 0);
        }
        if (ok) {
            goto ret2;
        }
    }
    DeviceIoControl(hDevice, 0x9001c, NULL, 0, NULL, 0, bytesRet, NULL);
    CloseHandle(hDevice);
    PECMD_FreeStrBuf((WCHAR **)&buf);
    goto cleanup_dos;

ret2:
    ret = 2;
    goto cleanup_out;

free_ret9:
    PECMD_FreeStrBuf((WCHAR **)&buf);
    return 9;

cleanup_dos:
    if (drive != NULL) {
        i = lstrlenW(drive);
        if ((i == 2) && (drive[1] == L':')) {
            driveLetter = *drive;
            DefineDosDeviceW(2, &driveLetter, NULL);
            PECMD_DeleteDriveMountPoint(*drive);
            if ((opts & 0x11) == 0) {
                PECMD_StartWorkerThread((void *)(uintptr_t)g_Script, NULL, 0x219, 7, 0, 3,
                                        (uint64_t)(uint32_t)(count * 5), 1, 0);
            }
        }
        else {
            *(void **)&pDeleteDev = g_pImDiskRemoveMountPoint;
            if ((pDeleteDev(drive) == 0) && ((err = GetLastError()) != 1) && (err != 0x57) &&
                (((err == 0x91) || (err == 0x10b)) || (err == 0x1126))) {
                FUN_140103a20();
                FUN_14001c82c();
            }
        }
    }
    return 0;

cleanup_out:
    PECMD_FreeStrBuf((WCHAR **)&buf);
    return ret;
}

/* ========== PECMD_ShutPowerAction @0x14003db00 ==========
 * 解析并执行电源命令: SHUTDOWN/LOGOUT/HIBERNATE/SLEEP/LOCK/EJECT/CLOSE。
 */
uint64_t PECMD_ShutPowerAction(WCHAR *cmdline)
{
    WCHAR *cmd = cmdline;
    WCHAR *rest = cmdline;
    uint32_t flags = 0;
    uint32_t actionFlags = 0x20;
    uint32_t shutdownType = 0;
    int mode = 0;
    char isForce;
    uint16_t ch;
    uint32_t timeout[2] = {0, 0};
    uint8_t forceClose = 1;
    int status;
    char *logBuf;
    WCHAR *token;
    int64_t (*pSetSuspend)(int, int, int);
    void (*pCdDoor)();

    PECMD_EnsureMciLoaded();
    PECMD_GetApiProcCached("SetSuspendState", "powrprof.DLL",
                           (int64_t *)(void **)&g_pSetSuspendState, NULL);
    isForce = FUN_1400660AC("-force", (int64_t *)&cmd, 6);
    timeout[0] = 0;
    if (isForce != '\0') {
        flags = 0x4000;
    }
    mode = 0;
    actionFlags = 0x20;
    forceClose = 1;
    shutdownType = 0;
    ch = *cmd;
    rest = cmd;
    for (;;) {
        if (ch == 0) {
        skip_to_exec:
            PECMD_SkipLeadingControls(&rest);
            return (uint64_t)PECMD_PerformSystemShutdown(mode, actionFlags | flags, WSTR("CMD"));
        }
        isForce = FUN_1400660AC("--", (int64_t *)&cmd, 2);
        if (isForce != '\0') {
            rest = cmd;
            goto skip_to_exec;
        }
        for (; *cmd == L'-'; cmd = cmd + 1) {
        }
        ch = (uint16_t)(*cmd & 0xffdf);
        isForce = FUN_1400660AC("SHUTDOWN", (int64_t *)&cmd, 8);
        if (isForce != '\0') {
            ch = *cmd;
            rest = cmd;
            break;
        }
        if (ch == 0x52) { /* R: reboot? */
            actionFlags = 0x10;
            mode = 1;
            rest = cmd + 1;
            goto next_char;
        }
        if (ch == 0x4c) { /* L: logout */
            rest = cmd + 1;
            PECMD_SkipLeadingControls(&rest);
            token = rest;
            PECMD_RunShutdownScript(token, 2);
            FUN_1400633A8((void **)&logBuf, 300);
            wsprintfA(logBuf, "\nLOGOUT (0x%X, 0x%X, \"CMD\") Main=0x%X", 0, 2);
            PECMD_AppendDebugLog(logBuf);
            status = ExitWindowsEx(0x10, 0);
            PECMD_FreeStrBuf((WCHAR **)&logBuf);
            return (uint64_t)(status == 0);
        }
        if (ch == 0x48) { /* H: hibernate */
            rest = cmd + 1;
            PECMD_SkipLeadingControls(&rest);
            token = rest;
            PECMD_RunShutdownScript(token, 8);
            FUN_14001C2CC(WSTR("SeShutdownPrivilege"), 2, 0);
            status = SetSystemPowerState(0, 1);
            return (uint64_t)(status == 0);
        }
        if (ch == 0x53) { /* S: sleep */
            rest = cmd + 1;
            if (g_pSetSuspendState == NULL) {
                return 1;
            }
            PECMD_SkipLeadingControls(&rest);
            token = rest;
            PECMD_RunShutdownScript(token, 4);
            *(void **)&pSetSuspend = g_pSetSuspendState;
            status = pSetSuspend(0, 1, 0);
            return (uint64_t)(status == 0);
        }
        if (ch == 0x4b) { /* K: lock */
            rest = cmd + 1;
            if (g_pSetSuspendState == NULL) {
                return 1;
            }
            PECMD_SkipLeadingControls(&rest);
            token = rest;
            PECMD_RunShutdownScript(token, 0x80);
            status = LockWorkStation();
            return (uint64_t)(status == 0);
        }
        if (ch == 0x45) { /* E: eject open */
            rest = cmd + 1;
            pCdDoor = g_pMciSendStringW;
            if (pCdDoor != NULL) {
                pCdDoor(WSTR("Set cdAudio door open"), 0);
                {
                    uint64_t delay = 10000;
                    FUN_1400195F0((uint64_t)(uintptr_t)g_Script, (int64_t)delay, 0, NULL);
                }
            }
        }
        else if (ch == 0x4f) { /* O: eject open with number */
            rest = cmd + 1;
            {
                uint64_t delay = 10000;
                if (((uint16_t)(*rest - 0x30) < 9) || (*rest == 0x39)) {
                    delay = 0;
                }
                for (; ((uint16_t)(cmd[1] - 0x30) < 9) || (cmd[1] == 0x39); cmd = cmd + 1) {
                    delay = (uint64_t)((cmd[1] - 0x30) + (int)delay * 10);
                }
                pCdDoor = g_pMciSendStringW;
                if (pCdDoor != NULL) {
                    pCdDoor(WSTR("Set cdAudio door open"), 0);
                    FUN_1400195F0((uint64_t)(uintptr_t)g_Script, (int64_t)delay, 0, NULL);
                }
            }
        }
        else {
            if (ch != 0x43) { /* C: close */
                goto skip_to_exec;
            }
            rest = cmd + 1;
            pCdDoor = g_pMciSendStringW;
            if (pCdDoor != NULL) {
                pCdDoor(WSTR("Set cdAudio door close"), 0, 0);
            }
        }
    next_char:
        cmd = cmd + 1;
        ch = *cmd;
    }

    /* 下面解析 SHUTDOWN 参数 */
    if (ch != L'-') {
        goto do_shutdown;
    }
    if (cmd[1] == L'-') {
        rest = cmd + 2;
        while ((ch = *rest) != 0 && (((uint16_t)ch < 9 || (0xd < (uint16_t)ch)) && (ch != L' '))) {
            rest = rest + 1;
        }
        goto do_shutdown;
    }
    isForce = FUN_1400660AC("-s", (int64_t *)&cmd, 2);
    if (isForce == '\0') {
        isForce = FUN_1400660AC("-r", (int64_t *)&cmd, 2);
        if (isForce != '\0') {
            shutdownType = 1;
            goto shutdown_flag;
        }
        isForce = FUN_1400660AC("-f", (int64_t *)&cmd, 2);
        if (isForce != '\0') {
            forceClose = 1;
            goto shutdown_flag;
        }
        isForce = FUN_1400660AC("--f", (int64_t *)&cmd, 3);
        if (isForce == '\0') {
            isForce = FUN_1400660AC("-t", (int64_t *)&cmd, 2);
            if (isForce != '\0') {
                PECMD_ParseNumber((int64_t *)&cmd, (int *)timeout);
                goto shutdown_flag;
            }
            while ((ch = *cmd) != 0 &&
                   (((uint16_t)ch < 9 || (0xd < (uint16_t)ch)) && (ch != L' '))) {
                cmd = cmd + 1;
            }
            PECMD_SkipLeadingControls(&cmd);
        }
        else {
            forceClose = 0;
            rest = cmd + 3;
        }
    }
    else {
        shutdownType = 0;
    shutdown_flag:
        rest = cmd + 2;
    }
    ch = *cmd;
    goto shutdown_parse;

do_shutdown:
shutdown_parse:
    flags = (uint32_t)(-(int)(shutdownType != 0) & 0xfffffff0) + 0x20;
    PECMD_SkipLeadingControls(&rest);
    token = rest;
    PECMD_RunShutdownScript(token, flags);
    FUN_14001C2CC(WSTR("SeShutdownPrivilege"), 2, 0);
    FUN_14001C2CC(WSTR("SeRemoteShutdownPrivilege"), 2, 0);
    FUN_1400633A8((void **)&logBuf, 300);
    wsprintfA(logBuf, "\nSHUTDOWN (0x%X, 0x%X, \"CMD\") Main=0x%X", shutdownType, flags);
    PECMD_AppendDebugLog(logBuf);
    InitiateSystemShutdownW(NULL, g_szEmpty, timeout[0], forceClose, (int)shutdownType);
    PECMD_FreeStrBuf((WCHAR **)&logBuf);
    return 0;
}

/* ========== PECMD_ShellLaunchThread @0x14003e768 ==========
 * 主 Shell 循环: 等待/执行 Shell 命令，处理重试与退出条件。
 */
void PECMD_ShellLaunchThread(LARGE_INTEGER script, uint64_t a2, uint64_t a3, uint64_t a4)
{
    (void)a2;
    (void)a3;
    WCHAR *shellCmd = NULL;
    WCHAR *cmdCopy = NULL;
    WCHAR *shellName = NULL;
    LPCWSTR pWVar6;
    WCHAR *pWVar4;
    HWND hWnd;
    DWORD startTick;
    uint32_t cmdType;
    char retry = 2;
    int64_t delay = 500;

    if (g_u8CCB1 != 0) {
        PECMD_TlsLogWrite((uint64_t)(uintptr_t)g_Script, WSTR("MAIN_DBG:%d\r\n"), 0x26ba, a4);
    }
    if ((g_runFlag & 0x20) != 0) {
        hWnd = PECMD_ReadValidWindowHandle(PTR_u_CallBackhWnd_14011e668_2_14013a288);
        a4 = 0;
        SendMessageW(hWnd, 0x44a, 0, 0);
    }
    if (g_u8CCB1 != 0) {
        PECMD_TlsLogWrite((uint64_t)(uintptr_t)g_Script, WSTR("MAIN_DBG:%d\r\n"), 0x26c0, a4);
    }
    SendMessageW(g_hwndCF78, 0x44a, 0, 0);
    if (g_u8CCB1 != 0) {
        PECMD_TlsLogWrite((uint64_t)(uintptr_t)g_Script, WSTR("MAIN_DBG:%d\r\n"), 0x26c2, 0);
    }
    if ((DAT_140147003 != 0) && (DAT_140147002 == 0)) {
        DAT_140147002 = 1;
        PECMD_InitIfOldSystem();
    }
    if (g_u8CCB1 != 0) {
        PECMD_TlsLogWrite((uint64_t)(uintptr_t)g_Script, WSTR("MAIN_DBG:%d\r\n"), 0x26c4, 0);
    }
    while ((g_pwszD250 == NULL) || (*g_pwszD250 == L'\0')) {
        PECMD_LoadPelogonShellCfg((uint64_t)script.QuadPart, 0);
        if (g_u8CCB1 != 0) {
            PECMD_TlsLogWrite((uint64_t)(uintptr_t)g_Script, WSTR("MAIN_DBG:%d %u MyShell[%s]\r\n"),
                              0x26c8, (uint64_t)GetCurrentProcessId());
        }
        if ((g_pwszD250 == NULL) || (*g_pwszD250 == L'\0')) {
            FUN_1400195F0(script.QuadPart, 100, 0, NULL);
        }
        if ((g_topiconFlag != 0) && (g_hwndTray == 0)) {
            SendMessageW(g_hwndCF78, 1099, 0, 0);
        }
    }
    if (g_u8CCB1 != 0) {
        PECMD_TlsLogWrite((uint64_t)(uintptr_t)g_Script, WSTR("MAIN_DBG:%d\r\n"), 0x26ce, 0);
    }
    SleepEx(10, 1);
    PECMD_TruncateDebugLog();
    if ((DAT_140147003 != 0) && (DAT_140147002 == 0)) {
        DAT_140147002 = 1;
        PECMD_InitIfOldSystem();
    }
    FUN_1400702B0(&shellCmd, g_pwszD250);
    pWVar4 = PECMD_SkipSwitches(shellCmd);
    pWVar4 = PECMD_TruncateAtCommaSkipSpace(pWVar4);
    shellName = PECMD_GetFileName(pWVar4);
    SleepEx(10, 1);
    retry = 2;
    if (*shellCmd == 0) {
        PECMD_FreeStrBuf(&shellCmd);
        return;
    }
    for (;;) {
        while (1) {
            if (g_u8CCB1 != 0) {
                PECMD_TlsLogWrite((uint64_t)(uintptr_t)g_Script, WSTR("MAIN_DBG:%d\r\n"), 0x26e9,
                                  0);
            }
            if (PECMD_IsSingleInstance() == 0) {
                break;
            }
        loop_retry:
            FUN_1400195F0(script.QuadPart, 500, 0, NULL);
            PECMD_LoadPelogonShellCfg((uint64_t)script.QuadPart, -1);
            FUN_14007034C((int64_t *)&shellCmd, g_pwszD250);
        }
        if (g_u8CCB1 != 0) {
            PECMD_TlsLogWrite((uint64_t)(uintptr_t)g_Script, WSTR("MAIN_DBG:%d\r\n"), 0x26eb, 0);
        }
        if ((g_topiconFlag != 0) && (g_hwndTray == 0)) {
            SendMessageW(g_hwndCF78, 1099, 0, 0);
        }
        if (*shellCmd == 0) {
            goto loop_retry;
        }
        if (g_u8CCB1 != 0) {
            PECMD_TlsLogWrite((uint64_t)(uintptr_t)g_Script, WSTR("MAIN_DBG:%d\r\n"), 0x26ee, 0);
        }
        pWVar6 = (LPCWSTR)PECMD_IsExplorerPath((int16_t *)shellName);
        if (g_u8CCB1 != 0) {
            PECMD_TlsLogWrite((uint64_t)(uintptr_t)g_Script, WSTR("MAIN_DBG:%d\r\n"), 0x26f0, 0);
        }
        if ((g_runFlag & 0x80) != 0) {
            goto loop_retry;
        }
        if (pWVar6 != NULL) {
            DWORD d = FUN_14002D708(pWVar6, 0, NULL, 0, 0);
            if (d != 0) {
                goto loop_retry;
            }
        }
        PECMD_ApplyDesktopWallpaper();
        if (g_u8CCB1 != 0) {
            PECMD_TlsLogWrite((uint64_t)(uintptr_t)g_Script, WSTR("MAIN_DBG:%d\r\n"), 0x26f3, 0);
        }
        SendMessageW(g_hwndCF78, 0x44a, 0, 0);
        if (g_u8CCB1 != 0) {
            PECMD_TlsLogWrite((uint64_t)(uintptr_t)g_Script, WSTR("MAIN_DBG:%d\r\n"), 0x26f5, 0);
        }
        LoadEnvi(g_szEmpty, g_szEmpty);
        startTick = GetTickCount();
        FUN_1400702B0(&cmdCopy, shellCmd);
        cmdType = PECMD_LineIsTeamExecLoad(cmdCopy);
        if (g_u8CCB1 != 0) {
            PECMD_TlsLogWrite((uint64_t)(uintptr_t)g_Script, WSTR("MAIN_DBG:%d\r\n"), 0x26fc, 0);
            if (g_u8CCB1 != 0) {
                PECMD_TlsLogWrite((uint64_t)(uintptr_t)g_Script,
                                  WSTR("MAIN_DBG:%d %u MyShell[%s]\r\n"), 0x26fd,
                                  (uint64_t)GetCurrentProcessId());
            }
        }
        pWVar6 = WSTR("0");
        if (retry < 2) {
            pWVar6 = WSTR("1");
        }
        PECMD_ExecIndataCommand(WSTR("BeforeStart"), pWVar6);
        g_answerFlag = (uint8_t)retry;
        if (cmdType == 0) {
            PECMD_ExecCmdDispatch((void *)script.QuadPart, cmdCopy, (void *)script.QuadPart,
                                  (void *)(uintptr_t)0x5e, 7, 0, 0, 0);
        }
        else {
            PECMD_ProcessScriptBlock(script.QuadPart, (uint64_t)(uintptr_t)cmdCopy, NULL, NULL,
                                     NULL);
        }
        g_answerFlag = 0;
        PECMD_ExecIndataCommand(WSTR("AfterTerm"), WSTR("1"));
        if (retry > 1) {
            retry = retry - 1;
        }
        if (GetTickCount() - startTick > 1999) {
            goto after_wait;
        }
        delay = 500;
        while (1) {
            FUN_1400195F0(script.QuadPart, delay, 0, NULL);
        after_wait:
            PECMD_LoadPelogonShellCfg((uint64_t)script.QuadPart, 0);
            if ((g_pwszD250 == NULL) || (*g_pwszD250 == L'\0')) {
                goto free_copy;
            }
            if ((*g_pwszD250 != L'*') || (g_pwszD250[1] != L'\0')) {
                break;
            }
            delay = 100;
        }
        FUN_14007034C((int64_t *)&shellCmd, g_pwszD250);
        if (g_u8CCB1 != 0) {
            PECMD_TlsLogWrite((uint64_t)(uintptr_t)g_Script, WSTR("MAIN_DBG:%d MyShell[%s]\r\n"),
                              0x270b, (uint64_t)(uintptr_t)shellCmd);
        }
        pWVar4 = PECMD_SkipSwitches(shellCmd);
        pWVar4 = PECMD_TruncateAtCommaSkipSpace(pWVar4);
        shellName = PECMD_GetFileName(pWVar4);
        if (g_u8CCB1 != 0) {
            PECMD_TlsLogWrite((uint64_t)(uintptr_t)g_Script, WSTR("MAIN_DBG:%d ShellName[%s]\r\n"),
                              9999, (uint64_t)(uintptr_t)shellName);
        }
    free_copy:
        PECMD_FreeStrBuf(&cmdCopy);
    }
}
