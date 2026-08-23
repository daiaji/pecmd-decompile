/* ====================================================================
 * core_b2a.c — B2 批次小函数 (140020000-14003ffff)
 *
 * 来源: PECMD原始.EXE (x64, ImageBase=0x140000000)
 *   字符串前缀检测   PECMD_IsVkPrefix @0x1400226b0
 *   参数表包装       PECMD_ParseValueCommand @0x140025980
 *   一次性初始化     PECMD_EnsureOneTimeInit @0x140027e88
 *   引用计数释放     PECMD_ReleaseRef @0x1400281a4
 *   版本检查         PECMD_InitIfOldSystem @0x14002f1cc
 *   文件大小包装     FUN_140035b08 @0x140035b08
 *   文件大小包装     FUN_140035b24 @0x140035b24
 *   窗口过程转发     FUN_140037b84 @0x140037b84
 *   窗口过程转发     PECMD_ScriptWndProc @0x14003892c
 *   窗口过程转发     PECMD_WndProcForward @0x14003e16c
 *
 * 约定:
 *   - 未实现内部函数 extern 挂起 + TODO(verify), 不臆造语义
 *   - 全局数据符号先用 extern 声明; 完整链接时统一在 core_globals.c 定义
 * ==================================================================== */
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>

#include "pecmd_defs.h"

/* ---- 未实现依赖 (extern + TODO(verify)) ---- */
extern uint64_t PECMD_ParseCommandBlock(void *script, void **args, int count, uint64_t flags);
extern uint64_t PECMD_OneTimeInitBody(void);
extern void PECMD_SetObjectVtable(void *obj);
extern uint64_t PECMD_GetPackedSystemVersion(void);
extern HMENU PECMD_BuildImDiskMenu(int64_t *obj, ULARGE_INTEGER pos, int mode,
                           int64_t *out, uint64_t flags);
extern uint64_t PECMD_HelpDlgProc(void *app, HWND hwnd, uint32_t msg, HDC hdc, HWND wnd);
extern HGDIOBJ PECMD_LogoDlgProc(void *script, HWND hwnd, uint32_t msg, HDC hdc, void *p4);
extern int64_t PECMD_MainMsgWndProc(uintptr_t script, HWND hwnd, uint32_t msg, void *wParam,
                             int64_t lParam);
extern uint32_t PECMD_ParseSize(LPWSTR s, int a, int b, int c);
extern DWORD PECMD_RegSetValueWithOpen(HKEY root, LPCWSTR sub, LPCWSTR name, DWORD type,
                           BYTE *data, DWORD size);
extern uint64_t PECMD_ServiceControl(void *script, LPCWSTR name);
extern void PECMD_RunFbwfHookScript(void);
extern void PECMD_TerminateJobObject(int64_t obj);
extern uint32_t FUN_1400E3288(uint32_t mode, uint32_t flags); /* @0x1400e3288 */
extern void PECMD_BuildExecCommand(int64_t obj);
extern void PECMD_CleanupTaskEnvObject(int64_t obj);
extern void *FUN_14001E69C(void *script, LPCWSTR name, void *scope, int64_t len);
extern uint64_t PECMD_GetPELogonWindowValue(LPCWSTR name);
extern int64_t FUN_14000e26c(void *script, void *cmd, void *s3, void *s4,
                             uint32_t flag, void *p6, void *s7, void *p8);
extern int64_t PECMD_RegisterCallbackWnd(uint32_t mode);
extern void PECMD_AllocStrSlot(WCHAR **ps);
extern WCHAR *FUN_14001BE14(WCHAR *s);
extern WCHAR *FUN_14001C270(LPCWSTR src, WCHAR **out);
extern int64_t PECMD_WideStrLen(void *p);
extern uint64_t PECMD_SetDesktopWallpaper(void *p, int mode);
extern void PECMD_InitRamdataRegistry(int n);
extern uint32_t PECMD_ReadRamdataDword(LPCWSTR name);
extern uint64_t PECMD_CreateDirectoryTree(LPCWSTR path);
extern void FUN_140017F54(int *p);
extern int64_t PECMD_MatchPatternSwap(LPCWSTR a, LPCWSTR b);
extern void FUN_14006F884(LPCWSTR name, WCHAR **out);         /* @0x14006f884 已实现 */
extern uint64_t *PECMD_SkipLeadingControls(WCHAR **pp);
extern void FUN_1400F429C(WCHAR **pp, WCHAR ch);
extern WCHAR **FUN_14007034C(void *ps, LPCWSTR src);
extern bool FUN_1400C11C0(WCHAR **pp, int *out);
extern uint64_t PECMD_ApplyTextWindowLayout(uint64_t value, LPCWSTR text);
extern int64_t PECMD_RunCommand(void *script, WCHAR *cmd);

/* ---- 本批引用的全局数据 ---- */
extern uint8_t g_initOnceFlag;        /* 一次性初始化标志 */
extern CRITICAL_SECTION g_csThreadTbl;  /* 线程表临界区 */
extern DWORD g_dwC96C;          /* 线程 ID 输出 */
extern uint8_t g_sysStartuped;        /* SysStartuped 标志 */

/* ========== PECMD_IsVkPrefix @0x1400226b0 ==========
 * 判断串是否为 "vk_" 前缀 (忽略前两字符大小写)。
 */
int64_t PECMD_IsVkPrefix(WCHAR *s)
{
    if ((((*s | 0x20) == 0x76) && ((s[1] | 0x20) == 0x6b)) &&
        (s[2] == 0x5f)) {
        return 1;
    }
    return 0;
}

/* ========== PECMD_ParseValueCommand @0x140025980 ==========
 * 参数表包装: 构造 3 槽 arr[0]=value, 调 PECMD_ParseCommandBlock(...,2,...)。
 */
void PECMD_ParseValueCommand(void *script, void *value, uint64_t flags)
{
    void *arr[3];
    arr[0] = value;
    PECMD_ParseCommandBlock(script, arr, 2, flags);
}

/* ========== PECMD_EnsureOneTimeInit @0x140027e88 ==========
 * 一次性初始化门闩: 首次调用执行 PECMD_OneTimeInitBody()。
 */
int64_t PECMD_EnsureOneTimeInit(void)
{
    if (g_initOnceFlag == 0) {
        g_initOnceFlag = 1;
        PECMD_OneTimeInitBody();
    }
    return 1;
}

/* ========== PECMD_ReleaseRef @0x1400281a4 ==========
 * 引用计数释放包装: 先 PECMD_SetObjectVtable, 若 flags bit0 置位则 free。
 */
void *PECMD_ReleaseRef(void *obj, uint8_t flags)
{
    void *mem = obj;
    PECMD_SetObjectVtable(obj);
    if ((flags & 1) != 0) {
        free(mem);
    }
    return obj;
}

/* ========== PECMD_InitIfOldSystem @0x14002f1cc ==========
 * 版本检查: 高 16 位 < 0x60000 时执行一次性初始化。
 */
void PECMD_InitIfOldSystem(void)
{
    uint64_t u = PECMD_GetPackedSystemVersion();
    if ((uint32_t)(u >> 16) < 0x60000) {
        PECMD_EnsureOneTimeInit();
    }
}

/* ========== FUN_140035b08 @0x140035b08 ==========
 * @0x140035b08 size=25 — 文件大小/位置包装(asm→C): 纯尾调用包装,
 *   将第4参(r8)移入第4槽(r9)、置第3参 r8b=1, call PECMD_BuildImDiskMenu。
 */
void FUN_140035b08(int64_t *obj, ULARGE_INTEGER pos,
                   int64_t *out, uint64_t flags)
{
    PECMD_BuildImDiskMenu(obj, pos, 1, out, flags);
}

/* ========== FUN_140035b24 @0x140035b24 ==========
 * @0x140035b24 size=25 — 文件大小/位置包装(asm→C): 同 b08, 第3参 xor r8d=0,
 *   call PECMD_BuildImDiskMenu。
 */
void FUN_140035b24(int64_t *obj, ULARGE_INTEGER pos,
                   int64_t *out, uint64_t flags)
{
    PECMD_BuildImDiskMenu(obj, pos, 0, out, flags);
}

/* ========== FUN_140037b84 @0x140037b84 ==========
 * @0x140037b84 size=35 — 窗口过程转发(asm→C): rcx=DAT_14013cfb0(g_pAppData),
 *   rdx=hwnd, r8d=msg, r9=hdc, [rsp+0x20]=wnd,
 *   call PECMD_HelpDlgProc(g_pAppData, hwnd, msg, hdc, wnd)。
 */
void FUN_140037b84(HWND hwnd, uint32_t msg, HDC hdc, HWND wnd)
{
    PECMD_HelpDlgProc(g_pAppData, hwnd, msg, hdc, wnd);
}

/* ========== PECMD_ScriptWndProc @0x14003892c ==========
 * 窗口过程转发到 PECMD_LogoDlgProc(g_Script, ...)。
 */
void PECMD_ScriptWndProc(HWND hwnd, uint32_t msg, HDC hdc, void *p4)
{
    PECMD_LogoDlgProc(g_Script, hwnd, msg, hdc, p4);
}

/* ========== PECMD_WndProcForward @0x14003e16c ==========
 * 窗口过程转发到 PECMD_MainMsgWndProc((uintptr_t)g_Script, ...)。
 */
LRESULT PECMD_WndProcForward(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    /* UNIMPLEMENTED @0xFUN_14003e16c — decompile-failed, body 未还原 */
PECMD_MainMsgWndProc((uintptr_t)g_Script, hwnd, msg,
                  (void *)(uintptr_t)wParam, (int64_t)lParam);
    return 0;
}

/* ========== PECMD_SetFbwfThreshold @0x140020e40 ==========
 * FBWF 缓存阈值设置: 写注册表后初始化 "FBWF" 服务。
 * TODO(verify): 反编译返回 eax 残留, 这里返回 0。
 */
int64_t PECMD_SetFbwfThreshold(uint64_t unused, LPWSTR spec)
{
    uint32_t val[4];
    (void)unused;
    val[0] = PECMD_ParseSize(spec, 4, 0x400, 0x20);
    PECMD_RegSetValueWithOpen((HKEY)0xffffffff80000002,
                  WSTR("SYSTEM\\ControlSet001\\Services\\FBWF"),
                  WSTR("WinPECacheThreshold"), 4, (BYTE *)val, 4);
    PECMD_ServiceControl(g_Script, WSTR("FBWF"));
    PECMD_RunFbwfHookScript();
    return 0;
}

/* ========== PECMD_CleanupTaskThread @0x140020eb4 ==========
 * 线程体: 等待标志归零后清理任务对象并递减任务计数。
 */
int64_t PECMD_CleanupTaskThread(void *task)
{
    uint8_t *p = (uint8_t *)task;
    while (*(int *)(p + 0x1a4) > 0) {
        Sleep(10);
    }
    PECMD_TerminateJobObject((int64_t)task);
    if (*(int *)(p + 0x230) > 0) {
        EnterCriticalSection(&g_csThreadTbl);
        FUN_1400E3288(*(uint32_t *)(p + 0x230), 0);
        *(uint32_t *)(p + 0x230) = 0;
        LeaveCriticalSection(&g_csThreadTbl);
    }
    PECMD_BuildExecCommand((int64_t)task);
    PECMD_CleanupTaskEnvObject((int64_t)task);
    free(task);
    EnterCriticalSection(&g_csInit);
    g_taskCount--;
    LeaveCriticalSection(&g_csInit);
    return 0;
}

/* ========== PECMD_CreateCleanupThread @0x140020f60 ==========
 * 创建清理线程 (CREATE_SUSPENDED 0x4 | 0x10000), 成功后计数+1 并恢复。
 */
void PECMD_CreateCleanupThread(void *task)
{
    HANDLE hThread = CreateThread(NULL, 0x10000, (void *)PECMD_CleanupTaskThread,
                                  task, 0x10004, &g_dwC96C);
    if (hThread != (HANDLE)0) {
        EnterCriticalSection(&g_csInit);
        g_taskCount++;
        LeaveCriticalSection(&g_csInit);
        ResumeThread(hThread);
        CloseHandle(hThread);
    }
}

/* ========== PECMD_GetPathType @0x14002162c ==========
 * 盘符/UNC 前缀类型: 0=本地盘 2=UNC 4=UNC+ 8=+ 其它 -1。
 */
int32_t PECMD_GetPathType(WCHAR *s)
{
    uint16_t u = *s;
    if ((u == 0) || (s[1] != 0x3a)) {
        return -1;
    }
    if (u == 0x40) return 2;
    u = (uint16_t)(u | 0x20);
    if (u == 0x68) return 0;
    if (u == 99) return 4;
    if (u == 0x2b) return 8;
    return -1;
}

/* ========== PECMD_DerefVarName @0x140021684 ==========
 * 变量名解引用: 若串以 '&' 开头则查变量表并替换。
 */
LPCWSTR PECMD_DerefVarName(void *script, WCHAR **pp)
{
    LPCWSTR s = *pp;
    if (*s == L'&') {
        void *node = FUN_14001E69C(script, s, NULL, -1);
        if (node == NULL) {
            s = *pp;
        } else {
            s = *(LPCWSTR *)((uint8_t *)node + 8);
            *pp = (WCHAR *)s;
        }
    }
    return s;
}

/* ========== PECMD_IsSingleInstance @0x140022e24 ==========
 * 单实例检查: 已有 UCallBackhWnd 且互斥体占用则返回 0, 否则 1。
 */
int32_t PECMD_IsSingleInstance(void)
{
    DWORD err;
    HANDLE h;
    if (((g_runFlag & 1) != 0) && (PECMD_GetPELogonWindowValue(WSTR("UCallBackhWnd")) != 0)) {
        h = CreateMutexA(NULL, 1, "Global\\PECMD:main_u");
        if (h != (HANDLE)0) {
            err = GetLastError();
            CloseHandle(h);
            if (err != 0xb7) {
                return 1;
            }
        }
    }
    return 0;
}

/* ========== PECMD_RunPecmdMain @0x1400235b0 ==========
 * 以 "--incmd PECMD MAIN" 参数执行脚本, 最多等待 2 秒。
 */
int64_t PECMD_RunPecmdMain(void *script, uint32_t mode)
{
    char local_38[48];
    int64_t r;
    int i;
    memcpy(local_38, " --incmd PECMD MAIN", 0x2a);
    FUN_14000e26c(script, local_38, script, NULL, 0, NULL, NULL, NULL);
    i = 199;
    do {
        r = PECMD_RegisterCallbackWnd(mode & 0x40);
        if (r != 0) {
            return r;
        }
        Sleep(10);
        i--;
    } while (i >= 0);
    return 0;
}

/* ========== PECMD_ApplyWallpaper @0x140023bc4 ==========
 * 设置 Wallpaper.PECMD 注册表值并应用壁纸。
 */
uint64_t PECMD_ApplyWallpaper(WCHAR *path)
{
    WCHAR *local_res10 = NULL;
    WCHAR *ps;
    int64_t len;
    uint64_t r;
    PECMD_AllocStrSlot(&local_res10);
    ps = FUN_14001BE14(path);
    ps = FUN_14001C270(ps, &local_res10);
    len = PECMD_WideStrLen(ps);
    PECMD_RegSetValueWithOpen((HKEY)0xffffffff80000001, WSTR("Control Panel\\Desktop"),
                  WSTR("Wallpaper.PECMD"), 1, (BYTE *)ps, (int)len * 2);
    r = PECMD_SetDesktopWallpaper(ps, 1);
    PECMD_FreeStrBuf(&local_res10);
    return r;
}

/* ========== PECMD_IsSysStartuped @0x1400250bc ==========
 * SysStartuped 状态缓存。
 */
int PECMD_IsSysStartuped(void)
{
    uint32_t u;
    if (g_sysStartuped == 0) {
        PECMD_InitRamdataRegistry(0);
        u = PECMD_ReadRamdataDword(WSTR("SysStartuped"));
        g_sysStartuped = (uint8_t)(u == 0);
    }
    return (int)g_sysStartuped;
}

/* ========== PECMD_CollapseRepeatedChars @0x140025404 ==========
 * 删除连续重复字符 ch (只保留一个)。
 */
int64_t PECMD_CollapseRepeatedChars(LPCWSTR s, WCHAR ch)
{
    int len = lstrlenW(s);
    LPCWSTR end = s + len;
    while (*s != 0) {
        if (*s == ch && s[1] == ch) {
            LPCWSTR q = s + 1;
            memmove((void *)s, (void *)q,
                    (int)((end - s) / 2) * 2);
            end--;
            while (*q == ch) {
                s = q;
            }
        }
        s++;
    }
    return 0;
}

/* ========== PECMD_CreateDirectory @0x140027644 ==========
 * 创建目录; 成功 0, 已存在/失败返回错误码 (0 修正为 1)。
 */
DWORD PECMD_CreateDirectory(uint64_t unused, WCHAR *path)
{
    LPCWSTR p;
    BOOL ok;
    DWORD err;
    (void)unused;
    p = FUN_14001BE14(path);
    PECMD_CreateDirectoryTree(p);
    ok = CreateDirectoryW(p, NULL);
    if (ok == 1) {
        return 0;
    }
    err = GetLastError();
    if (err == 0) {
        err = 1;
    }
    return err;
}

/* ========== PECMD_ReleaseRefCounted @0x14002816c ==========
 * 引用计数释放包装 (带子对象清理)。
 */
void *PECMD_ReleaseRefCounted(void *obj, uint32_t flags)
{
    void *mem = obj;
    FUN_140017F54((int *)((uint8_t *)obj + 8));
    PECMD_SetObjectVtable(obj);
    if ((flags & 1) != 0) {
        free(mem);
    }
    return obj;
}

/* ========== PECMD_MatchPatternWithQuotes @0x1400281c8 ==========
 * 带引号感知的匹配查找。
 */
int64_t PECMD_MatchPatternWithQuotes(LPCWSTR text, LPCWSTR pattern)
{
    int64_t r = 0;
    for (;;) {
        LPCWSTR quote_end = NULL;
        WCHAR qc = *pattern;
        WCHAR saved = 0;
        if (qc == 0x22 || qc == 0x27) {
            pattern++;
            quote_end = pattern;
            FUN_1400F429C((WCHAR **)&quote_end, qc);
            saved = *quote_end;
            if (saved != 0) {
                *(WCHAR *)quote_end = 0;
            }
        }
        r = PECMD_MatchPatternSwap(text, pattern);
        if (saved != 0) {
            *(WCHAR *)quote_end = saved;
            quote_end++;
        }
        if (r != 0) break;
        if (quote_end == NULL || (pattern = quote_end, *quote_end == 0)) {
            return 0;
        }
    }
    return r;
}

/* ========== PECMD_ResolveScriptVariable @0x140028484 ==========
 * 变量/环境取值: 非环境模式且非 '&' 开头查环境, 否则查变量表。
 */
int64_t PECMD_ResolveScriptVariable(void *script, LPCWSTR name, void **out)
{
    void *node;
    int64_t v;
    if (*(uint8_t *)((uint8_t *)script + 0xd) == 0 && *name != L'&') {
        FUN_14006F884(name, (WCHAR **)out);
        v = (int64_t)(uintptr_t)*out;
    } else {
        node = FUN_14001E69C(script, name, NULL, -1);
        if (node == NULL) {
            v = 0;
        } else {
            v = *(int64_t *)((uint8_t *)node + 8);
        }
    }
    return v;
}

/* ========== PECMD_CreateVariableNode @0x1400284d4 ==========
 * 构造变量节点: 分配容器、类型=3、去引号并解析逗号分隔参数。
 */
void *PECMD_CreateVariableNode(void *node, LPCWSTR text)
{
    WCHAR *local_res10 = (WCHAR *)text;
    WCHAR *local_res8;
    PECMD_AllocStrSlot((WCHAR **)node);
    *(uint32_t *)((uint8_t *)node + 8) = 3;
    *(int64_t *)((uint8_t *)node + 16) = 0;
    PECMD_SkipLeadingControls(&local_res10);
    FUN_14007034C(node, local_res10);
    local_res8 = *(WCHAR **)node;
    FUN_1400F429C(&local_res8, 0x2c);
    if (*local_res8 == 0x2c) {
        *local_res8 = 0;
        local_res8++;
    }
    FUN_1400C11C0(&local_res8, (int *)((uint8_t *)node + 8));
    return node;
}

/* ========== PECMD_TruncateAtCommaSkipSpace @0x14002f298 ==========
 * 截断到逗号并跳空白, 返回剩余串。
 */
WCHAR *PECMD_TruncateAtCommaSkipSpace(WCHAR *s)
{
    WCHAR *p[4];
    p[0] = s;
    FUN_1400F429C(p, 0x2c);
    if (*p[0] != 0) {
        *p[0] = 0;
        p[0]++;
    }
    PECMD_SkipLeadingControls(p);
    return p[0];
}

/* ========== PECMD_SetPELogonParamText @0x140038950 ==========
 * PELOGON 文本参数: 无窗口写注册表, 有窗口发消息 0x453。
 */
void PECMD_SetPELogonParamText(uint64_t value, LPCWSTR text, HWND hwnd)
{
    int len;
    if (hwnd == (HWND)0) {
        PECMD_ApplyTextWindowLayout(value, text);
    } else {
        len = lstrlenW(text);
        PECMD_RegSetValueWithOpen((HKEY)0xffffffff80000002, WSTR("SOFTWARE\\PELOGON"),
                      WSTR("Text.Paramd"), 1, (BYTE *)text, len * 2 + 2);
        SendMessageW(hwnd, 0x453, 0, 0);
    }
}

/* ========== PECMD_RegisterCallbackWindowClass @0x14003e190 ==========
 * 注册 PELogon 回调窗口类。
 */
void PECMD_RegisterCallbackWindowClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wc;
    wc.cbSize = 0x50;
    wc.style = 3;
    wc.lpfnWndProc = PECMD_WndProcForward;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hIcon = (HICON)0;
    wc.hCursor = (HCURSOR)0;
    wc.hbrBackground = (HBRUSH)0;
    wc.lpszMenuName = (LPCWSTR)0;
    wc.lpszClassName = WSTR("PELogon CallBack Window");
    wc.hIconSm = (HICON)0;
    wc.hInstance = hInstance;
    RegisterClassExW(&wc);
}

/* ========== PECMD_ExecIndataCommand @0x14003e620 ==========
 * 构造 "#34:INDATA  <p1> <p2>" 并交给 ScriptRunA。
 */
void PECMD_ExecIndataCommand(LPCWSTR key, LPCWSTR value)
{
    WCHAR *line = NULL;
    FUN_1400702B0(&line, WSTR("#34:INDATA  "));
    FUN_14006375C(&line, key);
    FUN_14006375C(&line, WSTR(" "));
    FUN_14006375C(&line, value);
    PECMD_RunCommand(g_Script, line);
    PECMD_FreeStrBuf(&line);
}
