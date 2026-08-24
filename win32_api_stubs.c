/* win32_api_stubs.c - B0/P3: Win32-API-shaped no-op stubs. FOR LINK VERIFICATION ONLY. */
#include "stubs_common.h"

                        /* 传 PECMD_ExpandCommandLine 输出槽 (后部定义) */

uint64_t AbortSystemShutdownW(void) { return 0; }
uint64_t AddFontMemResourceEx(void) { return 0; }
uint64_t AddFontResourceW(void) { return 0; }
uint64_t AdjustTokenPrivileges(void) { return 0; }
uint64_t AllocConsole(void) { return 0; }
uint64_t AllocateAndInitializeSid(void) { return 0; }
int AppendMenuW(void *m, unsigned int f, uint64_t id, const unsigned short *s) { (void)m;(void)f;(void)id;(void)s; return 0; }
uint64_t AssignProcessToJobObject(void *job, void *proc) { (void)job;(void)proc; return 0; }   /* arity 修正 0->2 (PECMD_ExecCmdDispatch 恢复体) */
uint64_t Beep(void) { return 0; }
uint64_t BeginPaint(void) { return 0; }
uint64_t BitBlt(void) { return 0; }
uint64_t BringWindowToTop(void *h) { (void)h; return 0; }
uint64_t CM_Get_Parent(void) { return 0; }
uint64_t CallNextHookEx(void) { return 0; }
uint64_t CallWindowProcW(void) { return 0; }
uint64_t ChangeDisplaySettingsExW(void) { return 0; }
uint64_t ChangeServiceConfig2W(void) { return 0; }
uint64_t ChangeServiceConfigW(void) { return 0; }
uint64_t CharUpperW(void) { return 0; }
uint64_t CheckTokenMembership(void) { return 0; }
/* --- (原型置于此, 匹配后部定义; 消除与先前隐式 int() 的 conflicting types) --- */
uint64_t ClientToScreen(void *h, void *pt) { (void)h;(void)pt; return 0; }   /* arity 修正 0->2 (PECMD_SetControlGeom 恢复体) */
uint64_t CloseClipboard(void) { return 0; }
uint64_t CloseHandle(void *h) { (void)h; return 0; }
int CloseServiceHandle(void *h) { (void)h; return 0; }
uint64_t ControlService(void *h, unsigned long c, void *st) { (void)h;(void)c;(void)st; return 0; }   /* arity 修正 0->3 (PECMD_StopDeleteService) */
uint64_t CopyFileW(uint64_t a, uint64_t b, int c) { (void)a;(void)b;(void)c; return 1; }
uint64_t CopyImage(void) { return 0; }
uint64_t CreateBitmap(void) { return 0; }
uint64_t CreateCompatibleBitmap(void) { return 0; }
uint64_t CreateCompatibleDC(void) { return 0; }
uint64_t CreateDialogParamW(void) { return 0; }
uint64_t CreateDirectoryW(const uint16_t *path, void *sa) { (void)path;(void)sa; return 0; }   /* arity 修正 0->2 (PECMD_ExpandSpecialDirs 恢复体: 带参调用, 与 core_*.c 2 参调用一致) */
HRGN CreateEllipticRgn(int a, int b, int c, int d) { (void)a;(void)b;(void)c;(void)d; return (HRGN)0; }
void *CreateEventW(void *sa, int manual, int init, const unsigned short *name) { (void)sa;(void)manual;(void)init;(void)name; return (void*)0; }
uint64_t CreateFileMappingA(void) { return 0; }
/* CreateFileMappingW: 签名升级 0->6 参 (FUN_1400e3e38 文件映射辅助直移需要; 体保持桩, 同 win32_stub.h) */
HANDLE CreateFileMappingW(HANDLE h, LPSECURITY_ATTRIBUTES sa, DWORD prot, DWORD maxHi, DWORD maxLo, LPCWSTR name) { (void)h;(void)sa;(void)prot;(void)maxHi;(void)maxLo;(void)name; return (HANDLE)0; }
uint64_t CreateFileW(void) { return 0; }
uint64_t CreateFileA(void) { return 0; }
uint64_t CreateFontW(void) { return 0; }
uint64_t CreateHardLinkW(uint64_t a, uint64_t b, void *c) { (void)a;(void)b;(void)c; return 1; }
uint64_t CreateMutexA(void *a, int b, const char *c) { (void)a;(void)b;(void)c; return (uint64_t)(uintptr_t)1; }   /* arity 修正 0->3 (PECMD_ScriptMainEntry 恢复体) */
uint64_t CreateMutexW(void *a, int b, void *c) { (void)a;(void)b;(void)c; return (uint64_t)(uintptr_t)1; }
uint64_t CreatePopupMenu(void) { return 0; }
uint64_t CreateProcessW(LPCWSTR app, const uint16_t *cmd, LPSECURITY_ATTRIBUTES sa,
        LPSECURITY_ATTRIBUTES ta, BOOL inh, DWORD flags, void *env, LPCWSTR dir,
        void *si, void *pi) {
    (void)app;(void)cmd;(void)sa;(void)ta;(void)inh;(void)flags;(void)env;(void)dir;(void)si;(void)pi;
    return 0;
}   /* arity 修正 0->10 (PECMD_ExecCmdDispatch 恢复体 *extract 子进程) */
uint64_t CreateRoundRectRgn(void) { return 0; }
void *CreateServiceW(void *mgr, const unsigned short *name, const unsigned short *disp, unsigned long acc, unsigned long type, unsigned long start, unsigned long err, const unsigned short *bin, const unsigned short *grp, unsigned long *tag, const unsigned short *deps, const unsigned short *acct, const unsigned short *pwd) { (void)mgr;(void)name;(void)disp;(void)acc;(void)type;(void)start;(void)err;(void)bin;(void)grp;(void)tag;(void)deps;(void)acct;(void)pwd; return (void*)0; }
HBRUSH CreateSolidBrush(COLORREF color) { (void)color; return (HBRUSH)0; }
void *CreateThread(void *sa, size_t st, void *rt, void *p, uint32_t f, uint32_t *tid) { (void)sa;(void)st;(void)rt;(void)p;(void)f;(void)tid; return (void*)0; }
uint64_t CreateToolhelp32Snapshot(DWORD flags, DWORD pid) { (void)flags;(void)pid; return 0; }   /* arity 修正 (PECMD_EnumProcessInfo 体) */
HWND CreateWindowExW(DWORD ex, const WCHAR *cls, const WCHAR *name, DWORD style, int x, int y, int w, int h, HWND parent, HMENU menu, void *inst, void *param) { (void)ex;(void)cls;(void)name;(void)style;(void)x;(void)y;(void)w;(void)h;(void)parent;(void)menu;(void)inst;(void)param; return (HWND)0; }
uint64_t CryptAcquireContextW(void) { return 0; }
uint64_t CryptCreateHash(void) { return 0; }
uint64_t CryptDestroyHash(void) { return 0; }
uint64_t CryptGetHashParam(void) { return 0; }
uint64_t CryptHashData(void) { return 0; }
uint64_t CryptReleaseContext(void) { return 0; }
uint64_t DefWindowProcW(void) { return 0; }
uint64_t DefWindowProcW_exref(void) { return 0; }
uint64_t DefineDosDeviceW(void) { return 0; }
uint64_t DeleteDC(void *a) { (void)a; return 1; }
uint64_t DeleteFileW(uint64_t a) { (void)a; return 1; }
int DeleteObject(void *obj) { (void)obj; return 1; }
uint64_t DeleteService(void *h) { (void)h; return 0; }   /* arity 修正 0->1 (PECMD_StopDeleteService) */
int DestroyIcon(void *ic) { (void)ic; return 0; }
uint64_t DestroyMenu(void *h) { (void)h; return 0; }   /* arity 修正 0->1 */
int DestroyWindow(void *h) { (void)h; return 0; }
int DeviceIoControl(void *h, unsigned long code, void *in, unsigned long inb, void *out, unsigned long outb, unsigned long *ret, void *ov) { (void)h;(void)code;(void)in;(void)inb;(void)out;(void)outb;(void)ret;(void)ov; return 0; }
uint64_t DialogBoxIndirectParamW(void *a, void *b, void *c, void *d, uint64_t e) { (void)a;(void)b;(void)c;(void)d;(void)e; return 0; }
uint64_t DialogBoxParamW(void *a, void *b, void *c, void *d, uint64_t e) { (void)a;(void)b;(void)c;(void)d;(void)e; return 0; }
uint64_t DispatchMessageW(const void *m) { (void)m; return 0; }
uint64_t DragAcceptFiles(void) { return 0; }
uint64_t DrawEdge(void) { return 0; }
uint64_t DrawFocusRect(void) { return 0; }
int DrawFrameControl(void *hdc, RECT *r, UINT t, UINT f) { (void)hdc;(void)r;(void)t;(void)f; return 0; }
uint64_t DrawIcon(void) { return 0; }
uint64_t DrawIconEx(void) { return 0; }
int DrawTextW(void *hdc, LPCWSTR s, int c, RECT *rc, uint fmt) { (void)hdc;(void)s;(void)c;(void)rc;(void)fmt; return 0; }
uint64_t DuplicateHandle(void *hSrcP, void *hSrc, void *hDstP, HANDLE *phDst, DWORD acc, BOOL inh, DWORD opts) { (void)hSrcP;(void)hSrc;(void)hDstP;(void)phDst;(void)acc;(void)inh;(void)opts; return 1; }   /* arity 修正 0->7 (PECMD_ExecCmdDispatch + core_b3k 调用) */
uint64_t Ellipse(void) { return 0; }
uint64_t EmptyClipboard(void) { return 0; }
uint64_t EnableMenuItem(void) { return 0; }
int EnableWindow(HWND w, int en) { (void)w;(void)en; return 0; }
uint64_t EndDialog(uint64_t a, int b) { (void)a;(void)b; return 1; }
uint64_t EndPaint(void) { return 0; }
void EnterCriticalSection(void *cs) { (void)cs; }
int EnumChildWindows(HWND parent, WNDENUMPROC cb, LPARAM lp) { (void)parent;(void)cb;(void)lp; return 0; }
uint64_t EnumDisplayDevicesW(void) { return 0; }
uint64_t EnumDisplaySettingsW(void) { return 0; }
int EnumWindows(WNDENUMPROC cb, LPARAM lp) { (void)cb;(void)lp; return 0; }
void ExitProcess(uint32_t code) { (void)code; }
uint64_t ExitWindowsEx(void) { return 0; }
uint64_t ExitWindowsEx_exref(void) { return 0; }
uint64_t ExpandEnvironmentStringsW(LPCWSTR src, LPWSTR dst, DWORD n) { (void)src;(void)dst;(void)n; return 0; }                                     /* 新增桩 @0x1400229f8 (未映射 helper) */
BOOL ConnectNamedPipe(void *pipe, OVERLAPPED *ol) { (void)pipe;(void)ol; return 1; }          /* 新增桩 */
BOOL GetOverlappedResult(void *pipe, OVERLAPPED *ol, DWORD *bytes, BOOL wait) {               /* 新增桩 */
    (void)pipe;(void)ol;(void)wait; if (bytes) *bytes = 0; return 1;
}                                                /* @0x14013c960 函数槽 (GetProcessID, 运行时装载) */
uint64_t ShellExecuteExW(void *pei) { (void)pei; return 1; }               /* 新增桩 (SEG7 ShellExecuteEx 路径) */
HANDLE CreateNamedPipeA(LPCSTR name, DWORD openMode, DWORD pipeMode, DWORD maxInst,
                        DWORD outSz, DWORD inSz, DWORD timeout, LPSECURITY_ATTRIBUTES sa) {
    (void)name;(void)openMode;(void)pipeMode;(void)maxInst;(void)outSz;(void)inSz;(void)timeout;(void)sa;
    return (HANDLE)0xffffffffffffffff;   /* INVALID_HANDLE_VALUE → 走真实失败分支 */
}
BOOL SetNamedPipeHandleState(HANDLE h, DWORD *mode, DWORD *maxCount, DWORD *collectBytes) {
    (void)h;(void)mode;(void)maxCount;(void)collectBytes; return 1;
}                                            /* core_b2c.c */
int CreatePipe(void *out, void *in, void *sa, unsigned long n) { (void)out;(void)in;(void)sa;(void)n; return 1; }                                        /* DAT_14013cb80 (core_globals.c) */
/* FormatMessageW 库内原缺 → 新增最小桩 (返回 0 → 调用方走 "\r\n" 兜底路径) */
uint32_t FormatMessageW(uint32_t flags, const void *src, uint32_t msg, uint32_t lang,
                        WCHAR *buf, uint32_t size, void *args)
{ (void)flags;(void)src;(void)msg;(void)lang;(void)buf;(void)size;(void)args; return 0; }
COORD GetLargestConsoleWindowSize(void *h) { (void)h; COORD c = { 0, 0 }; return c; }
int SetConsoleWindowInfo(void *h, int b, void *r) { (void)h;(void)b;(void)r; return 1; }
/* advapi32 安全描述符 API 桩 (R1 批新增: PECMD_InitNullDaclSD 真体依赖, 库内原无) */
int InitializeSecurityDescriptor(void *pSD, DWORD dwRevision) { (void)pSD;(void)dwRevision; return 1; }
int SetSecurityDescriptorDacl(void *pSD, BOOL bDaclPresent, void *pDacl) { (void)pSD;(void)bDaclPresent;(void)pDacl; return 1; }
uint64_t FillRect(void) { return 0; }
uint64_t FindClose(HANDLE h) { (void)h; return 0; }                       /* arity 修正 0->1 (PECMD_ToSysCopyFiles 直移) */
uint64_t FindNextFileW(HANDLE h, void *fd) { (void)h;(void)fd; return 0; } /* arity 修正 0->2 (PECMD_ToSysCopyFiles 直移) */
uint64_t FindResourceW(void *a, void *b, void *c) { (void)a;(void)b;(void)c; return 1; }
uint64_t FindWindowA(void) { return 0; }
uint64_t FindWindowExA(void) { return 0; }
uint64_t FindWindowExW(void) { return 0; }
uint64_t FindWindowW(void) { return 0; }
uint64_t FlushFileBuffers(void *h) { (void)h; return 1; }
uint64_t FrameRgn(void) { return 0; }
uint64_t FreeEnvironmentStringsW(LPWSTR env) { (void)env; return 1; }   /* arity 修正 0->1 (PECMD_ExecCmdDispatch 恢复体) */
uint64_t FreeLibrary(void *h) { (void)h; return 1; }
uint64_t FreeSid(void) { return 0; }
uint64_t GetAsyncKeyState(void) { return 0; }
uint64_t GetBkColor(void) { return 0; }
int GetClientRect(void *hWnd, void *lpRect) { (void)hWnd;(void)lpRect; return 0; }
uint64_t GetClipboardData(void) { return 0; }
WCHAR *GetCommandLineW(void) { return (WCHAR *)0; }
uint64_t GetConsoleWindow(void) { return 0; }
DWORD GetCurrentDirectoryW(DWORD n, WCHAR *buf) { (void)n;(void)buf; return 0; }
uint64_t GetCurrentProcess(void) { return 0; }
uint64_t GetCurrentProcessId(void) { return 0; }
uint64_t GetCurrentThread(void) { return 0; }
uint64_t GetCurrentThreadId(void) { return 0; }
int GetCursorPos(void *p) { (void)p; return 0; }
HDC GetDC(void *h) { (void)h; return (HDC)0; }
uint64_t GetDIBits(void) { return 0; }
void *GetDesktopWindow(void) { return (void *)0; }
uint64_t GetDeviceCaps(void) { return 0; }
uint64_t GetDlgCtrlID(void *h) { (void)h; return 0; }   /* arity 修正 0->1 (PECMD_SetControlGeom 恢复体) */
HWND GetDlgItem(void *hWnd, int nIDDlgItem) { (void)hWnd;(void)nIDDlgItem; return 0; }
uint64_t GetDlgItemTextW(void) { return 0; }
uint64_t GetDriveTypeW(void) { return 0; }
uint64_t GetEnvironmentStringsW(void) { return 0; }
uint64_t GetEnvironmentVariableA(void) { return 0; }
DWORD GetEnvironmentVariableW(const WCHAR *n, WCHAR *buf, DWORD sz) { (void)n;(void)buf;(void)sz; return 0; }
uint64_t GetExitCodeProcess(void *h, DWORD *code) { (void)h;(void)code; return 0; }   /* arity 修正 0->2 */
uint64_t GetDiskFreeSpaceExW(void) { return 0; }
uint64_t GetFileAttributesExW(void) { return 0; }
uint64_t GetFileSize(void) { return 0; }
uint64_t GetFileSizeEx(void) { return 0; }
uint64_t GetFocus(void) { return 0; }
uint64_t GetForegroundWindow(void) { return 0; }
uint64_t GetFullPathNameW(void) { return 0; }
uint64_t GetHandleInformation(void) { return 0; }
uint64_t GetIconInfo(void) { return 0; }
uint64_t GetKeyState(void) { return 0; }
uint64_t GetKeyboardState(void) { return 0; }
DWORD GetLastError(void) { return 0; }
uint64_t GetLocaleInfoW(void) { return 0; }
uint64_t GetLogicalDriveStringsW(void) { return 0; }
uint64_t GetLogicalDrives(void) { return 0; }
uint64_t GetMenuItemID(void) { return 0; }
uint64_t GetMenuStringW(void) { return 0; }
int GetMessageW(void *m, void *w, unsigned int a, unsigned int b) { (void)m;(void)w;(void)a;(void)b; return 0; }
unsigned long GetModuleFileNameW(void *h, WCHAR *buf, unsigned long n) { (void)h;(void)buf;(void)n; return 0; }
void *GetModuleHandleA(const char *m) { (void)m; return (void *)0; }
HMODULE GetModuleHandleW(const WCHAR *m) { (void)m; return (HMODULE)0; }
uint64_t GetObjectW(uint64_t a, uint64_t b, void *c) { (void)a;(void)b;(void)c; return 0; }
HWND GetParent(HWND w) { (void)w; return (HWND)0; }
HWND GetActiveWindow(void) { return (HWND)0; }
uint64_t GetPixel(void) { return 0; }
FARPROC GetProcAddress(HMODULE h, const char *name) { (void)h;(void)name; return (FARPROC)0; }
uint64_t GetProcessAffinityMask(void) { return 0; }
uint64_t GetProcessHeap(void) { return 0; }
uint64_t GetProcessTimes(HANDLE h, FILETIME *a, FILETIME *b, FILETIME *c, FILETIME *d) { (void)h;(void)a;(void)b;(void)c;(void)d; return 0; }   /* arity 修正 (PECMD_EnumProcessInfo 体) */
int GetScrollPos(void *w, int n) { (void)w;(void)n; return 0; }
uint64_t GetScrollRange(void) { return 0; }
void GetStartupInfoW(void *d) { (void)d; }
uint64_t GetStdHandle(void) { return 0; }
void *GetStockObject(int i) { (void)i; return (void *)0; }
uint64_t GetSubMenu(void) { return 0; }
DWORD GetSysColor(int idx) { (void)idx; return 0; }
uint64_t GetSystemMenu(void) { return 0; }
int GetSystemMetrics(int idx) { (void)idx; return 0; }
uint64_t GetSystemTimeAsFileTime(void) { return 0; }
uint64_t GetThreadContext_exref(void) { return 0; }
DWORD GetTickCount(void) { return 0; }
uint64_t GlobalMemoryStatusEx(void) { return 0; }
uint64_t GetTokenInformation(void) { return 0; }
uint64_t GetVersion(void) { return 0; }
uint64_t GetVersionExW(void) { return 0; }
uint64_t GetWindowDC(void) { return 0; }
uint64_t GetWindowLongPtrW(void *h, int idx) { (void)h;(void)idx; return 0; }   /* arity 修正 0->2 (PECMD_SetControlGeom 恢复体) */
LONG GetWindowLongW(HWND hWnd, int nIndex) { (void)hWnd;(void)nIndex; return 0; }
int GetWindowRect(void *w, void *r) { (void)w;(void)r; return 0; }
uint64_t GetWindowTextLengthW(void) { return 0; }
uint64_t GetWindowTextW(void *hWnd, WCHAR *lpString, int nMaxCount) { (void)hWnd;(void)lpString;(void)nMaxCount; return 0; }
DWORD GetWindowThreadProcessId(HWND w, DWORD *pid) { (void)w;(void)pid; return 0; }
uint64_t GlobalAlloc(void) { return 0; }
uint64_t GlobalDeleteAtom(void) { return 0; }
uint64_t GlobalFree(uint64_t a) { (void)a; return 1; }
uint64_t GlobalLock(void) { return 0; }
uint64_t GlobalMemoryStatus(void) { return 0; }
uint64_t GlobalUnlock(void) { return 0; }
void *HeapAlloc(void *h, DWORD f, size_t n) { (void)h;(void)f;(void)n; return (void *)0; }
int HeapFree(void *h, DWORD f, void *p) { (void)h;(void)f;(void)p; return 0; }
uint64_t HeapReAlloc(void) { return 0; }
int InitiateSystemShutdownW(void *m, const unsigned short *t, unsigned long to, int f1, int f2) { (void)m;(void)t;(void)to;(void)f1;(void)f2; return 0; }
uint64_t InsertMenuW(void) { return 0; }
int InvalidateRect(void *w, const void *r, int erase) { (void)w;(void)r;(void)erase; return 0; }
int IsDialogMessageW(void *h, const void *m) { (void)h;(void)m; return 0; }
uint64_t IsIconic(void) { return 0; }
int IsWindow(void *h) { (void)h; return 0; }
int IsWindowEnabled(HWND w) { (void)w; return 0; }
int IsWindowVisible(HWND w) { (void)w; return 0; }
void LeaveCriticalSection(void *cs) { (void)cs; }
uint64_t LoadCursorW(void) { return 0; }
uint64_t LoadEnvi(const uint16_t *a, const uint16_t *b) { (void)a;(void)b; return 0; }   /* arity 修正 0->2 (PECMD_ScriptMainEntry 恢复体) */
void *LoadIconW(void *hinst, const unsigned short *name) { (void)hinst;(void)name; return (void*)0; }
void *LoadLibraryA(const char *m) { (void)m; return (void *)0; }
uint64_t LoadLibraryExW(LPCWSTR path, void *file, uint32_t flags) { (void)path;(void)file;(void)flags; return (uint64_t)(uintptr_t)1; }
HMODULE LoadLibraryW(const WCHAR *name) { (void)name; return (HMODULE)0; }
uint64_t LoadMenuW(void) { return 0; }
uint64_t LoadResource(void *a, uint64_t b) { (void)a;(void)b; return 1; }
int LoadStringW(void *h, unsigned int id, WCHAR *buf, int n) { (void)h;(void)id;(void)buf;(void)n; return 0; }
uint64_t LockResource(uint64_t a) { (void)a; return 1; }
uint64_t LockWorkStation(void) { return 0; }
uint64_t LookupPrivilegeValueW(void) { return 0; }
uint64_t MapViewOfFile(void) { return 0; }
int MessageBoxW(void *w, const unsigned short *t, const unsigned short *c, unsigned int f) { (void)w;(void)t;(void)c;(void)f; return 0; }
uint64_t ModifyMenuW(void) { return 0; }
uint64_t MoveFileExW(uint64_t a, uint64_t b, unsigned int c) { (void)a;(void)b;(void)c; return 1; }
uint64_t MoveFileW(uint64_t a, uint64_t b) { (void)a;(void)b; return 1; }
uint64_t MoveWindow(void) { return 0; }
uint64_t MsgWaitForMultipleObjects(DWORD count, HANDLE *handles, BOOL waitAll,
        DWORD timeout, DWORD mask) { (void)count;(void)handles;(void)waitAll;(void)timeout;(void)mask; return 0; }
/* arity 修正 0->5 (PECMD_ExecCmdDispatch 恢复体 PECMD_MsgWaitForObjects 内联展开) */
uint64_t MulDiv(void) { return 0; }
int OffsetRect(RECT *r, int dx, int dy) { (void)r;(void)dx;(void)dy; return 0; }
uint64_t OpenClipboard(void) { return 0; }
uint64_t OpenFileMappingA(void) { return 0; }
uint64_t OpenFileMappingW(void) { return 0; }
uint64_t OpenProcess(DWORD access, BOOL inherit, DWORD pid) { (void)access;(void)inherit;(void)pid; return 0; }   /* arity 修正 (PECMD_EnumProcessInfo 体) */
uint64_t OpenProcessToken(void) { return 0; }
void *OpenSCManagerW(const void *a, const void *b, unsigned long acc) { (void)a;(void)b;(void)acc; return (void*)0; }
void *OpenServiceW(void *mgr, const unsigned short *name, unsigned long acc) { (void)mgr;(void)name;(void)acc; return (void*)0; }
uint64_t OpenThread(void) { return 0; }
uint64_t PathMatchSpecW(const uint16_t *a, const uint16_t *b) { (void)a;(void)b; return 0; }   /* arity 修正 (PECMD_EnumProcessInfo 体) */
uint64_t PathFileExistsW(void) { return 0; }
int PeekMessageW(void *msg, void *hwnd, UINT a, UINT b, UINT c) { (void)msg;(void)hwnd;(void)a;(void)b;(void)c; return 0; }
uint64_t Polyline(void) { return 0; }
int PostMessageW(void *w, unsigned int m, uint64_t a, uint64_t b) { (void)w;(void)m;(void)a;(void)b; return 0; }
uint64_t PostQuitMessage(void) { return 0; }
uint64_t Process32FirstW(HANDLE h, PROCESSENTRY32W *pe) { (void)h;(void)pe; return 0; }   /* arity 修正 (PECMD_EnumProcessInfo 体) */
uint64_t Process32NextW(HANDLE h, PROCESSENTRY32W *pe) { (void)h;(void)pe; return 0; }   /* arity 修正 (PECMD_EnumProcessInfo 体) */
int PtInRect(const void *r, POINT p) { (void)r;(void)p; return 0; }
uint64_t QueryDosDeviceW(void) { return 0; }
uint64_t SetVolumeMountPointW(void) { return 0; }
uint64_t GetVolumeNameForVolumeMountPointW(void *mount, void *buf, unsigned long n) { (void)mount;(void)buf;(void)n; return 0; }   /* arity 修正 (PECMD_ScriptInitParse 体) */
uint64_t QueryPerformanceCounter(void *out) { if (out != (void *)0) *(uint64_t *)out = 0; return 0; }   /* 带出参签名 (PECMD_AppendLogMessage 时间戳; 桩恒 0) */
uint64_t QueryPerformanceFrequency(void) { return 0; }
uint64_t RaiseException(void) { return 0; }
uint64_t ReadFile(void *h, void *buf, unsigned long n, unsigned long *read, void *ol) { (void)h;(void)buf;(void)n;(void)read;(void)ol; return 1; }   /* arity 修正 0->5 (PECMD_ScriptMainEntry 恢复体) */
uint64_t ReadProcessMemory(void) { return 0; }
uint64_t RedrawWindow(void) { return 0; }
int RegCloseKey(void *k) { (void)k; return 0; }
int RegCreateKeyExW(void *k, const WCHAR *sub, DWORD res, void *cls, DWORD opt, DWORD acc, void *sa, void *out, DWORD *disp) { (void)k;(void)sub;(void)res;(void)cls;(void)opt;(void)acc;(void)sa;(void)out;(void)disp; return 0; }
uint64_t RegDeleteKeyW(void) { return 0; }
int RegDeleteValueW(void *k, const WCHAR *nm) { (void)k;(void)nm; return 0; }
uint64_t RegEnumValueW(void) { return 0; }
int RegOpenKeyExW(void *k, const unsigned short *s, unsigned long o, unsigned long a, void **out) { (void)k;(void)s;(void)o;(void)a;(void)out; return 0; }
uint64_t RegQueryInfoKeyW(void) { return 0; }
uint64_t RegQueryValueExA(void) { return 0; }
int RegQueryValueExW(void *k, const WCHAR *nm, DWORD *rs, DWORD *ty, BYTE *dt, DWORD *sz) { (void)k;(void)nm;(void)rs;(void)ty;(void)dt;(void)sz; return 0; }
uint64_t RegisterClassExW(void) { return 0; }
uint64_t RegisterDeviceNotificationW(void) { return 0; }
UINT RegisterWindowMessageW(const WCHAR *s) { (void)s; return 0; }
uint64_t ReleaseDC(void *h, void *dc) { (void)h;(void)dc; return 0; }
uint64_t ReleaseMutex(void) { return 0; }
uint64_t RemoveDirectoryW(uint64_t a) { (void)a; return 1; }
uint64_t RemoveFontMemResourceEx(void) { return 0; }
uint64_t RemoveFontResourceW(void) { return 0; }
uint64_t RemoveMenu(void) { return 0; }
int ResetEvent(void *h) { (void)h; return 0; }
uint64_t ResumeThread(void *hThread) { (void)hThread; return 0; }   /* arity 修正 0->1 (PECMD_ExecCmdDispatch 恢复体) */
uint64_t SHChangeNotify(void) { return 0; }
uint64_t SHFileOperationW(uint64_t a) { (void)a; return 0; }
uint64_t SHGetSpecialFolderPathW(HWND hwnd, WCHAR *path, int csidl, BOOL fCreate) { (void)hwnd;(void)path;(void)csidl;(void)fCreate; return 0; }   /* arity 修正 (PECMD_EnumProcessInfo 体) */
uint64_t ScreenToClient(void *h, void *p) { (void)h;(void)p; return 0; }
uint64_t SearchPathW(void) { return 0; }
void *SelectObject(void *hdc, void *obj) { (void)hdc;(void)obj; return (void *)0; }
uint64_t SendMessageTimeoutW(void) { return 0; }
intptr_t SendMessageW(void *h, UINT m, uint64_t w, uint64_t l) { (void)h;(void)m;(void)w;(void)l; return 0; }
uint64_t SetActiveWindow(void *h) { (void)h; return 0; }
COLORREF SetBkColor(void *hdc, COLORREF c) { (void)hdc;(void)c; return 0; }
uint64_t SetBkMode(void) { return 0; }
uint64_t SetClipboardData(void) { return 0; }
uint64_t SetConsoleScreenBufferSize(void) { return 0; }
int SetCurrentDirectoryW(const WCHAR *dir) { (void)dir; return 0; }
uint64_t SetCursor(void) { return 0; }
uint64_t SetCursorPos(void) { return 0; }
uint64_t SetDIBits(void) { return 0; }
uint64_t SetDlgItemTextW(void) { return 0; }
int SetEndOfFile(void *h) { (void)h; return 1; }
int SetEnvironmentVariableW(const WCHAR *n, const WCHAR *v) { (void)n;(void)v; return 0; }
int SetEvent(void *h) { (void)h; return 0; }
uint64_t SetFilePointer(void *f, long dist, LONG *distHi, DWORD how) { (void)f;(void)dist;(void)distHi;(void)how; return 0; }   /* arity 修正 0->4 */
uint64_t SetFilePointerEx(void *h, longlong off, longlong *out, unsigned long method) { (void)h;(void)off;(void)out;(void)method; return 0; }   /* arity 修正 (PECMD_ScriptInitParse 体) */
uint64_t SetFocus(void *h) { (void)h; return 0; }
uint64_t SetForegroundWindow(void *h) { (void)h; return 0; }
void SetLastError(DWORD e) { (void)e; }
uint64_t SetLayeredWindowAttributes(void *h, uint32_t cr, uint8_t a, uint32_t f) { (void)h;(void)cr;(void)a;(void)f; return 0; }   /* arity 修正 0->4 (PECMD_SetControlGeom 恢复体) */
uint64_t SetLocalTime(void) { return 0; }
int SetMenuItemBitmaps(void *m, unsigned int id, unsigned int f, void *b1, void *b2) { (void)m;(void)id;(void)f;(void)b1;(void)b2; return 0; }
uint64_t SetParent(void *h, void *p) { (void)h;(void)p; return 0; }   /* arity 修正 0->2 (PECMD_SetControlGeom 恢复体) */
uint64_t SetPixel(void) { return 0; }
uint64_t SetProcessWorkingSetSize(void *h, uint64_t a, uint64_t b) { (void)h;(void)a;(void)b; return 1; }
uint64_t SetScrollInfo(void) { return 0; }
uint64_t SetScrollRange(void) { return 0; }
uint64_t SetSystemPowerState(void) { return 0; }
COLORREF SetTextColor(void *hdc, COLORREF c) { (void)hdc;(void)c; return 0; }
uint64_t SetThreadContext_exref(void) { return 0; }
uint64_t SetThreadPriority(void) { return 0; }
uint64_t SetTimer(HWND h, uint64_t id, UINT ms, TIMERPROC proc) { (void)h;(void)id;(void)ms;(void)proc; return 0; }
uint64_t SetWindowLongPtrW(void *h, int idx, uint64_t v) { (void)h;(void)idx;(void)v; return 0; }
uint64_t SetWindowLongW(void) { return 0; }
int SetWindowPos(void *w, void *after, int x, int y, int cx, int cy, unsigned int f) { (void)w;(void)after;(void)x;(void)y;(void)cx;(void)cy;(void)f; return 0; }
int SetWindowRgn(HWND h, void *r, BOOL b) { (void)h;(void)r;(void)b; return 1; }
int SetWindowTextW(void *hWnd, const void *lpString) { (void)hWnd;(void)lpString; return 0; }
uint64_t SetWindowsHookExW(void) { return 0; }
uint64_t SetupDiCreateDeviceInfoList(void) { return 0; }
uint64_t SetupDiCreateDeviceInfoW(void) { return 0; }
uint64_t SetupDiOpenClassRegKey(void) { return 0; }
uint64_t SetupDiSetDeviceRegistryPropertyW(void) { return 0; }
uint64_t Shell_NotifyIconW(DWORD msg, void *nid) { (void)msg;(void)nid; return 0; }
uint64_t ShowScrollBar(void) { return 0; }
int ShowWindow(void *h, int c) { (void)h;(void)c; return 0; }
uint64_t SizeofResource(uint64_t a, uint64_t b) { (void)a;(void)b; return 0; }
void Sleep(unsigned long ms) { (void)ms; }
uint64_t SleepEx(void) { return 0; }
uint64_t StartServiceCtrlDispatcherW(void) { return 0; }
LPWSTR StrChrW(const WCHAR *s, WCHAR c) { (void)s;(void)c; return (LPWSTR)0; }
uint64_t StrCmpNIA(const char *a, const char *b, int n) { (void)a;(void)b;(void)n; return 0; }   /* arity 修正 0->3 (PECMD_ExecCmdDispatch 恢复体 .cmd 预读) */
int StrCmpNIW() { return 0; }
void *OpenDesktopW(const WCHAR *n, uint64_t f, uint64_t acc, uint64_t flags) { (void)n;(void)f;(void)acc;(void)flags; return (void*)(uintptr_t)1; }
int SetThreadDesktop(void *d) { (void)d; return 1; }
int SwitchDesktop(void *d) { (void)d; return 1; }
int CloseDesktop(void *d) { (void)d; return 1; }
unsigned int DragQueryFileW(uint64_t a, uint32_t b, void *c, uint32_t d) { (void)a;(void)b;(void)c;(void)d; return 0; }
LONG RegSetValueExW(void *k, const unsigned short *n, unsigned long r, unsigned long t, const unsigned char *d, unsigned long c) { (void)k;(void)n;(void)r;(void)t;(void)d;(void)c; return 1; }
uint64_t StrCmpNW(void) { return 0; }
uint64_t StrCpyNW(void) { return 0; }
LPWSTR StrRChrW(const WCHAR *start, const WCHAR *end, WCHAR c) { (void)start;(void)end;(void)c; return (LPWSTR)0; }
uint64_t StrStrA(void) { return 0; }
uint64_t StrStrIW(void) { return 0; }
LPWSTR StrStrW(const WCHAR *a, const WCHAR *b) { (void)a;(void)b; return (LPWSTR)0; }
uint64_t StrToIntExW(void) { return 0; }
uint64_t StretchBlt(void) { return 0; }
uint64_t SystemParametersInfoW(void) { return 0; }
uint64_t TerminateJobObject(void *h, uint32_t code) { (void)h;(void)code; return 0; }   /* arity 修正 0->2 */
uint64_t TerminateProcess(void *h, uint32_t code) { (void)h;(void)code; return 0; }   /* arity 修正 0->2 */
uint64_t TerminateThread(void) { return 0; }
uint64_t TrackPopupMenu(void *menu, uint32_t flags, int x, int y, uint32_t res, void *wnd, void *rect) { (void)menu;(void)flags;(void)x;(void)y;(void)res;(void)wnd;(void)rect; return 0; }   /* arity 修正 0->7 */
int TranslateMessage(const void *m) { (void)m; return 0; }
uint64_t UnhookWindowsHookEx(void *h) { (void)h; return 0; }   /* arity 修正 0->1 (PECMD_ScriptMainEntry 恢复体) */
uint64_t UnmapViewOfFile(void) { return 0; }
int UnregisterHotKey(void *w, int id) { (void)w;(void)id; return 0; }
int UpdateWindow(void *w) { (void)w; return 0; }
uint64_t VirtualAlloc(void) { return 0; }
uint64_t VirtualFree(void *a, uint64_t b, uint64_t c) { (void)a;(void)b;(void)c; return 1; }
uint64_t VirtualProtect(void) { return 0; }
uint64_t VirtualProtectEx(void) { return 0; }
uint64_t VirtualQueryEx(void) { return 0; }
unsigned long WaitForMultipleObjects(unsigned long n, void **h, int all, unsigned long ms) { (void)n;(void)h;(void)all;(void)ms; return 0; }
uint64_t WaitForSingleObject(void *h, uint64_t ms) { (void)h;(void)ms; return 0; }
uint64_t WaitNamedPipeW(void) { return 0; }
uint64_t WideCharToMultiByte(void) { return 0; }
uint64_t WinExec(void) { return 0; }
uint64_t WinExec_exref(void) { return 0; }
uint64_t WriteFile(void *h, void *buf, unsigned long n, unsigned long *written, void *ol) { (void)h;(void)buf;(void)n;(void)written;(void)ol; return 1; }
uint64_t WriteProcessMemory(void) { return 0; }
uint64_t _UNK_140121ff6(void) { return 0; }
uint64_t _localtime64(void) { return 0; }
int _snwprintf(WCHAR *buf, size_t n, const WCHAR *fmt, ...) { (void)buf;(void)n;(void)fmt; return 0; }
uint64_t _time64(void) { return 0; }
uint64_t keybd_event(void) { return 0; }
uint64_t lstrcatW(void *a, uint64_t b) { (void)a;(void)b; return 1; }
uint64_t lstrcmpW(const uint16_t *a, const uint16_t *b) { (void)a;(void)b; return 0; }
int lstrcmpiA(const char *a, const char *b) { (void)a;(void)b; return 0; }
int lstrcmpiW(const WCHAR *a, const WCHAR *b) { (void)a;(void)b; return 0; }
unsigned short *lstrcpyW(unsigned short *dst, const unsigned short *src) { (void)dst;(void)src; return dst; }
uint64_t lstrcpynW(void) { return 0; }
int32_t lstrlenA(const char *s) { (void)s; return 0; }
int lstrlenW(const WCHAR *s) { (void)s; return 0; }
int main(void) { return 0; }
uint64_t mouse_event(void) { return 0; }
void *operator_new(uint64_t n) { (void)n; return 0; }
uint64_t ram0x000140120a48(void) { return 0; }
uint64_t u__26_INDATA_140121fe0(void) { return 0; }
uint64_t u_____D__140120a40(void) { return 0; }
uint64_t wsprintfA(char *out, const char *fmt, ...) { (void)out;(void)fmt; return 0; }   /* arity 修正 0->可变参 (PECMD_ExecCmdDispatch 恢复体) */
int wsprintfW(unsigned short *out, const unsigned short *fmt, ...) { (void)out;(void)fmt; return 0; }

/* ---- 补充桩 (B1/B3 重构新增引用) ---- */
uint64_t FindFirstFileW(void) { return 0; }
uint64_t FindFirstVolumeW(void) { return 0; }
uint64_t FindNextVolumeW(void) { return 0; }
uint64_t FindVolumeClose(void) { return 0; }
uint64_t LoadBitmapW(void) { return 0; }
uint64_t LoadImageW(void) { return 0; }
uint64_t lstrcmpA(void) { return 0; }
uint64_t CM_Get_Device_IDW(void) { return 0; }
uint64_t SetupDiDestroyDeviceInfoList(void) { return 0; }
uint64_t SetupDiEnumDeviceInterfaces(void) { return 0; }
uint64_t SetupDiGetDeviceInstanceIdW(void) { return 0; }
uint64_t SetupDiGetDeviceInterfaceDetailW(void) { return 0; }

/* ---- 追加桩: batch25 还原(140017b8c/00d2a0/0192d0/004ac4 及 B3 13 函数)引用的未定义符号 ---- */
/* Win32 / shlwapi API 桩 */
uint64_t CharUpperA(void) { return 0; }
uint64_t DeleteVolumeMountPointW(void) { return 0; }
uint64_t LookupAccountSidW(void) { return 0; }
uint64_t StrCmpNA(void) { return 0; }
uint64_t StrPBrkW(void) { return 0; }

/* --- r34 follow-up: Win32 API stubs (no real impl) --- */
int SetServiceStatus(void *h, void *status) { return 1; }
int TlsSetValue(unsigned long idx, void *val) { return 1; }
int Rectangle(void *hdc, int a, int b, int c, int d) { return 1; }
void SwitchToFiber(void *fiber) { }
void SwitchToThisWindow(void *hw, int b){ (void)hw;(void)b; }
int GetClassNameW(void *hw, uint16_t *p, int n){ (void)hw;(void)p; return 0; }
int CM_Get_DevNode_Status(uint32_t *a, uint32_t *b, uint32_t c, uint32_t d){ (void)a;(void)b;(void)c;(void)d; return 1; }
int CM_Query_And_Remove_SubTreeW(uint32_t a, int *b, uint32_t c, uint32_t d){ (void)a;(void)b;(void)c;(void)d; return 1; }
int CM_Request_Device_EjectW(){ return 1; }
int CM_Locate_DevNodeW(uint32_t *a, void *b, uint32_t c){ (void)a;(void)b;(void)c; return 1; }
int CM_Reenumerate_DevNode(uint32_t a, uint32_t b){ (void)a;(void)b; return 1; }uint64_t SystemTimeToFileTime(void){ return 0; }
uint64_t RegEnumKeyExW(void){ return 0; }
int wnsprintfW(uint16_t *b, int n, const uint16_t *f, ...){ (void)b;(void)n;(void)f; return 0; }
int GetWindowInfo(void *hWnd, void *pwi) { (void)hWnd;(void)pwi; return 0; }
uint64_t DuplicateTokenEx(void){ return 0; }
uint64_t SetTokenInformation(void){ return 0; }
uint64_t CreateProcessAsUserW(void *tok, LPCWSTR app, WCHAR *cmd, LPSECURITY_ATTRIBUTES sa,
        LPSECURITY_ATTRIBUTES ta, BOOL inh, DWORD flags, void *env, LPCWSTR dir, void *si, void *pi) {
    (void)tok;(void)app;(void)cmd;(void)sa;(void)ta;(void)inh;(void)flags;(void)env;(void)dir;(void)si;(void)pi;
    return 0;
}   /* arity 修正 0->11 (PECMD_ExecCmdDispatch 恢复体) */
uint64_t CreateProcessWithLogonW(void *user, void *domain, void *pwd, DWORD flags,
        void *app, WCHAR *cmd, DWORD flg2, void *env, void *dir, void *si, void *pi) {
    (void)user;(void)domain;(void)pwd;(void)flags;(void)app;(void)cmd;(void)flg2;(void)env;(void)dir;(void)si;(void)pi;
    return 0;
}   /* arity 修正 0->11 (PECMD_ExecCmdDispatch 恢复体) */
uint64_t FileTimeToSystemTime(void){ return 0; }
/* --- r39 follow-up stubs (main-agent closure) --- */
int IsRectEmpty(const void *r) { (void)r; return 0; }
uint64_t CreateDCW(void){ return 0; }
uint64_t ReleaseSemaphore(void){ return 0; }
uint64_t SetupDiSetDeviceInstallParamsW(void){ return 0; }
uint64_t SetupDiBuildDriverInfoList(void){ return 0; }
uint64_t SetupDiEnumDriverInfoW(void){ return 0; }
uint64_t SetupDiGetDriverInfoDetailW(void){ return 0; }
uint64_t SetupDiClassNameFromGuidW(void){ return 0; }
uint64_t GetShortPathNameW(LPCWSTR path, WCHAR *buf, DWORD len) { (void)path;(void)buf;(void)len; return 0; }   /* arity 修正 0->3 (PECMD_ExecCmdDispatch 恢复体) */
uint64_t CreateJobObjectW(LPSECURITY_ATTRIBUTES sa, LPCWSTR name) { (void)sa;(void)name; return 0; }
uint64_t SetupDiGetDeviceInstallParamsW(void){ return 0; }
uint64_t SetupDiClassGuidsFromNameW(void){ return 0; }
uint64_t VerQueryValueW(void){ return 0; }
uint64_t GetFileVersionInfoSizeW(void){ return 0; }
uint64_t GetFileVersionInfoW(void){ return 0; }
uint64_t SetErrorMode(void){ return 0; }
uint64_t TlsAlloc(void){ return 0; }
uint64_t WaitForInputIdle(HANDLE proc, DWORD ms) { (void)proc;(void)ms; return 0; }   /* HINSTANCE 资源句柄 */

uint64_t GlobalAddAtomA(void) { return 0; }
uint64_t lstrcpyA(void) { return 0; }
int RegisterHotKey(void *w, unsigned int id, unsigned int mods, unsigned int vk) { (void)w;(void)id;(void)mods;(void)vk; return 0; }
void *TlsGetValue(unsigned long idx) { (void)idx; return (void*)0; }
uint64_t WriteConsoleW(void) { return 0; }
/* ---- Tier 依赖预补 ---- */
uint64_t LCMapStringA(void){ return 0; }

/* Win32 API 补充 */
uint64_t GetClassLongW(void){ return 0; }
uint64_t GetSystemInfo(void){ return 0; }
uint64_t lstrcpynA(void){ return 0; }
uint64_t RealizePalette(void){ return 0; }
uint64_t RegOpenKeyExA(void){ return 0; }
uint64_t RegSetValueExA(void){ return 0; }
uint64_t SelectPalette(void){ return 0; }
uint64_t SetClassLongW(void){ return 0; }
uint64_t SetNamedSecurityInfoW(void){ return 0; }
uint64_t SHBrowseForFolderW(void){ return 0; }
uint64_t SHGetPathFromIDListW(void){ return 0; }
uint64_t StrCmpIW(void){ return 0; }

uint64_t DestroyCursor(void){ return 0; }
uint64_t EnumResourceNamesW(void){ return 0; }
uint64_t CreateIconFromResource(void){ return 0; }
uint64_t GetLastInputInfo(void){ return 0; }

/* --- batch r42: B1/B3/B9 remaining restore link stubs --- */
uint64_t GetDiskFreeSpaceW(void){ return 0; }
uint64_t GetVolumeInformationW(void){ return 0; }
/* Win32 / SetupAPI */
uint64_t SetVolumeLabelW(void){ return 0; }
uint64_t SetupDiGetDeviceInfoListDetailW(void){ return 0; }
uint64_t GetTempPathW(void){ return 0; }
uint64_t CM_Get_DevNode_Status_Ex(void){ return 0; }

/* --- batch28 restored-function deps --- */
uint64_t CreateMenu(void) { return 0; }
uint64_t FileTimeToLocalFileTime(void) { return 0; }
uint64_t GetLongPathNameW(void) { return 0; }
uint64_t GetMenu(void) { return 0; }
uint64_t GetTimeZoneInformation(void) { return 0; }
uint64_t LocalFileTimeToFileTime(void) { return 0; }
uint64_t SetMenu(void) { return 0; }
uint64_t SHAppBarMessage(void) { return 0; }

/* RegLoadKeyW / RegUnLoadKeyW — Advapi32 直接导入, 原 refactor 未引用故缺桩 (house no-op 模式) */
uint64_t RegLoadKeyW(void) { return 0; }
uint64_t RegUnLoadKeyW(void) { return 0; }

/* ============================================================
   ---- 新增叶依赖桩 (FUN_14005ccb0 SetupDi 设备变更恢复所需) ----
   ============================================================ */
int SetupDiSetClassInstallParamsW(void *a, void *b, void *c, uint32_t d) { (void)a;(void)b;(void)c;(void)d; return 1; }
int SetupDiChangeState(void *a, void *b) { (void)a;(void)b; return 1; }
int SetupDiClassNameFromGuidA(const void *guid, char *buf, uint32_t buflen, uint32_t *req)
{ (void)guid;(void)buf;(void)buflen;(void)req; return 1; }

/* 图标资源 (FUN_140073934) 缺失 WinAPI 桩 */
uint64_t CreateIconFromResourceEx(void){ return 0; }   /* DAT_14013e2b8 缓存字体 (HFONT, 初值 0, 惰性创建) */
/* ---- 075c7c/00cedc 依赖 ---- */
uint64_t LCMapStringW(void){ return 0; }
uint64_t GetNamedSecurityInfoW(void) { return 0; }      /* 0=ERROR_SUCCESS 走主授权分支 */
uint64_t BuildExplicitAccessWithNameW(void) { return 0; }
uint64_t SetEntriesInAclW(int c, void *ea, void *old, void **newa) { (void)c;(void)ea;(void)old;(void)newa; return 0; }
uint64_t GetSecurityInfo(void *h, int t, uint32_t i, void **a, void **b, void **c, void **d, void **e) { (void)h;(void)t;(void)i;(void)a;(void)b;(void)c;(void)d;(void)e; return 1; }
uint64_t SetSecurityInfo(void *h, int t, uint32_t i, void *a, void *b, void *c, void *d) { (void)h;(void)t;(void)i;(void)a;(void)b;(void)c;(void)d; return 1; }
uint64_t GetUserNameW(void) { return 0; }
uint64_t LookupAccountNameW(void) { return 0; }
/* LocalFree — 安全描述符/ACL 释放 (PECMD_SetAclPermission 使用), 声明于 win32_stub.h, 原无定义 */
uint64_t LocalFree(uint64_t hMem) { (void)hMem; return 0; } /* 执行固定操作 */
/* WinAPI 桩: 资源串读取 / CRT atexit */
uint64_t LoadStringA(void){ return 0; }
int      atexit(void (*fn)(void)) { (void)fn; return 0; }

void    *RegisterServiceCtrlHandlerW(const WCHAR *name, void *handler) { (void)name;(void)handler; return (void*)0; }
int      SHGetSpecialFolderPathA(void *hwnd, char *buf, int folder, int create) { (void)hwnd;(void)buf;(void)folder;(void)create; return 0; }
char    *lstrcatA(char *dst, const char *src) { (void)dst;(void)src; return dst; }                     /* 注册表分支清理 (leaf stub) */
int _vsnwprintf() { return 0; }


/* ---- wave-current support: 008834 ---- */
int StartServiceW(void *svc, unsigned long argc, const unsigned short **argv) { (void)svc;(void)argc;(void)argv; return 0; }
uint64_t WindowFromPoint(POINT p) { (void)p; return 0; }
int IsChild(void *parent, void *child) { (void)parent;(void)child; return 0; }
/* 以下桩在本文件原为 (void) 签名, 按带参调用点就地修正 (仅本文件内可见调用): */
uint64_t AttachThreadInput(DWORD a, DWORD b, int c) { (void)a;(void)b;(void)c; return 0; }
int GetScrollInfo(void *h, int bar, SCROLLINFO *si) { (void)h;(void)bar;(void)si; return 0; }
int SetScrollPos(void *h, int bar, int pos, int redraw) { (void)h;(void)bar;(void)pos;(void)redraw; return 0; }
uint64_t KillTimer(void *h, uintptr_t id) { (void)h;(void)id; return 0; }
int ExtTextOutW(void *hdc,int x,int y,uint opt,void *rc,const uint16_t *s,int c,const int *dx)
    { (void)hdc;(void)x;(void)y;(void)opt;(void)rc;(void)s;(void)c;(void)dx; return 0; }
int GetTextMetricsW(void *hdc, void *tm) { (void)hdc;(void)tm; return 0; }
int SubtractRect(RECT *dst,const RECT *a,const RECT *b) { (void)a;(void)b; if(dst){dst->left=dst->top=dst->right=dst->bottom=0;} return 0; }
uint64_t MoveToEx(void *hdc,int x,int y,void *old) { (void)hdc;(void)x;(void)y;(void)old; return 0; }
uint64_t LineTo(void *hdc,int x,int y) { (void)hdc;(void)x;(void)y; return 0; }
void *CreatePen(int style,int width,COLORREF color) { (void)style;(void)width;(void)color; return (void *)0; }