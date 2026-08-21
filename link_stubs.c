typedef unsigned int uint;   /* 兼容 setupdi 桩 */
/* Auto-generated link stubs for undefined symbols (weak/no-op). */
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <string.h>
typedef unsigned long DWORD;
typedef uint16_t WCHAR;
typedef uint32_t REGSAM;
typedef char CHAR;
typedef const CHAR *LPCSTR;
typedef const WCHAR *LPCWSTR;
typedef uint32_t COLORREF;
typedef void *HANDLE;
typedef void *HDC;
typedef uint8_t BYTE;
typedef void *HKEY;
/* --- wave-2 P4 restored-helper Win32/minimal types --- */
typedef void *HWND;
typedef void *HMODULE;
typedef void *HINSTANCE;
typedef void *HICON;
typedef void *HMENU;
typedef void *HRGN;
typedef void *HBRUSH;
typedef void *HCURSOR;
typedef void *LPVOID;
typedef void *FARPROC;
typedef void *TIMERPROC;
typedef void *PVOID;
typedef long LONG;
typedef LONG *LPLONG;
typedef int BOOL;
typedef unsigned int UINT;
typedef unsigned long ULONG;
typedef unsigned long DWORD_PTR;
typedef unsigned long long ULONGLONG;
typedef long long LONGLONG;
typedef WCHAR *LPWSTR;
typedef WCHAR *PWSTR;
typedef UINT WPARAM;
typedef LONG LPARAM;
typedef const WCHAR *PCWSTR;
/* _snwprintf 等 CRT 宽字符格式化辅助型 (仅需指针宽度兼容) */
typedef char *pthreadmbcinfo;
/* --- wave-3 P4 restored-helper extra Win32/minimal types --- */
typedef void *HGDIOBJ;
typedef void *HDROP;
typedef void *HGLOBAL;
typedef void *HRSRC;
typedef void *HACCEL;
typedef void *HHOOK;
typedef void *HCRYPTPROV;
typedef void *SC_HANDLE;
typedef void *PSID;
typedef void *HLOCAL;
typedef void *HDESK;
typedef void *HSERVICE;
typedef uintptr_t UINT_PTR;
typedef unsigned long ULONG_PTR;
typedef uint32_t ACCESS_MASK;
typedef uint16_t WORD;
typedef int16_t SHORT;
typedef struct _SECURITY_ATTRIBUTES { ULONG nLength; void *lpSecurityDescriptor; BOOL bInheritHandle; } SECURITY_ATTRIBUTES;
typedef SECURITY_ATTRIBUTES *LPSECURITY_ATTRIBUTES;
typedef struct _OVERLAPPED { ULONG_PTR Internal; ULONG_PTR InternalHigh; union { struct { ULONG Offset; ULONG OffsetHigh; } s; void *Pointer; } u; void *hEvent; } OVERLAPPED;
typedef OVERLAPPED *LPOVERLAPPED;
typedef struct _FILETIME { DWORD dwLowDateTime; DWORD dwHighDateTime; } FILETIME, *LPFILETIME;
/* --- Ghidra 反编译伪类型映射 (P4 wave-4 移植) --- */
typedef uint64_t undefined8;
typedef uint32_t undefined4;
typedef uint16_t undefined2;
typedef uint8_t  undefined1;
typedef uint8_t  undefined;
typedef uint16_t ushort;
typedef int64_t  longlong;
typedef uint64_t ulonglong;
typedef HKEY *PHKEY;

/* ---- P4 wave-4 统一前置声明 (确保早放置的恢复体调用可正确编译) ---- */
typedef void *HGDIOBJ_H;
void   *RegisterServiceCtrlHandlerW(const WCHAR *name, void *handler);
int     SetServiceStatus(void *h, void *status);
WCHAR  *GetCommandLineW(void);
void   *CreateThread(void *sa, size_t st, void *rt, void *p, uint32_t f, uint32_t *tid);
void   *GetDesktopWindow(void);
void   *LoadLibraryA(const char *m);
FARPROC GetProcAddress(void *hm, const char *name);
void   *HeapAlloc(void *h, unsigned long f, size_t n);
int     HeapFree(void *h, unsigned long f, void *p);
void    EnterCriticalSection(void *cs);
void    LeaveCriticalSection(void *cs);
unsigned long GetCurrentDirectoryW(unsigned long n, WCHAR *buf);
unsigned long GetEnvironmentVariableW(const WCHAR *n, WCHAR *buf, unsigned long sz);
int     SetEnvironmentVariableW(const WCHAR *n, const WCHAR *v);
int     lstrcmpiW(const WCHAR *a, const WCHAR *b);
int     RegCreateKeyExW(void *k, const WCHAR *sub, unsigned long res, void *cls, unsigned long opt, unsigned long acc, void *sa, void *out, unsigned long *disp);
int     RegQueryValueExW(void *k, const WCHAR *nm, unsigned long *rs, unsigned long *ty, BYTE *dt, unsigned long *sz);
int     RegCloseKey(void *k);
unsigned long GetLastError(void);
int     IsWindow(void *h);
uint64_t SetWindowLongPtrW(void *h, int idx, uint64_t v);
int     DestroyWindow(void *h);
int     DeleteObject(void *h);
int     SetWindowRgn(void *h, void *r, int b);
intptr_t SendMessageW(void *h, unsigned int m, uint64_t w, uint64_t l);
int     ShowWindow(void *h, int c);
int     PeekMessageW(void *msg, void *hwnd, unsigned int a, unsigned int b, unsigned int c);
int     TranslateMessage(const void *m);
uint64_t DispatchMessageW(const void *m);
LONG    GetWindowLongW(void *h, int idx);
void   *CreateWindowExW(unsigned long ex, const WCHAR *cls, const WCHAR *name, unsigned long style, int x, int y, int w, int h, void *parent, void *menu, void *inst, void *param);
void   *operator_new(uint64_t n);

void   *GetModuleHandleA(const char *m);
UINT    RegisterWindowMessageW(const WCHAR *s);
void    FUN_140053e78(void);
uint64_t PTR_FUN_1401234f0;   /* vtable 真值 0x1401234f0 地址槽 */
extern int (*DAT_14013c970)(uint32_t, uint32_t, char *, int);
uint64_t FUN_14005c7c4(const char *a, const uint16_t *b);
void     FUN_1400702b0(uint16_t **ps, const uint16_t *src);
int64_t *FUN_14007034c(int64_t *param_1, const uint16_t *param_2);

/* helper 补前置声明 */
void FUN_14004e2cc(uint64_t a, void *b);
uint64_t FUN_14004fb44(longlong *a, ulonglong b);
void FUN_14005d9a8(int64_t a, int b);
uint64_t FUN_1400630d0(int a);
void FUN_14006e8f4(int64_t a);
DWORD FUN_14005c394(HKEY param_1, LPCWSTR param_2, PHKEY param_3, REGSAM param_4, uint param_5);
DWORD GetCurrentDirectoryW(DWORD n, WCHAR *buf);
DWORD GetEnvironmentVariableW(const WCHAR *n, WCHAR *buf, DWORD sz);
int   SetEnvironmentVariableW(const WCHAR *n, const WCHAR *v);
int   lstrcmpiW(const WCHAR *a, const WCHAR *b);
extern long long DAT_14013d130;
int64_t FUN_140018978(int64_t *a, const uint16_t *b, int64_t *c, int64_t d, void *e);
void PECMD_SetVariable(void *a, const WCHAR *b, const WCHAR *c);
void FUN_140063694(void *p, long long n);
void FUN_14005b104(void *ps);
void PECMD_DialogBeepNotify(int64_t a, int b);
void FUN_14005daf8(int64_t a, int *b, int *c, int *d, int *e);

/* ---- P4 wave-4 (p4_wave1.json[120:160]) 支持块 (并发迁移后重建): 全局真值 + 内部 helper 前置声明 ----
 * 全局初值(真值)来源: python3 tools/pe_data_extract.py ../PECMD.exe <addr> <n> */
uint32_t DAT_14013a038 = 3;      /* g_bX64      0x14013a038: 03 00 00 00 */
void    *DAT_14013c910 = 0;      /* 服务句柄槽   0x14013c910 */
uint32_t DAT_14013c918 = 0;      /* SERVICE_STATUS 阵列 0x14013c918 (8 x DWORD) */
uint32_t DAT_14013c91c = 0;
uint32_t DAT_14013c920 = 0;
uint32_t DAT_14013c924 = 0;
uint32_t DAT_14013c928 = 0;
uint32_t DAT_14013c92c = 0;
uint32_t DAT_14013c930 = 0;
uint8_t  DAT_14013c96a = 0;      /* g_flag16a */
uint8_t  DAT_14013d260 = 0xb4;     /* 0x14013d260 原始: b4 3a 13 00 (定时清理标志) */
uint32_t DAT_14013c9f8 = 0;      /* g_WM_TaskbarButtonCreated */
void    *DAT_14013cfc8 = 0;      /* g_hUser32 */
void    *DAT_14013cfd0 = 0;      /* g_hKernel32 */
void    *DAT_14013cfd8 = 0;      /* g_hGdi32 */
uint8_t  DAT_14013d300 = 0x10;   /* RichEdit 选择标志 0x14013d300: 10 */
void    *DAT_14013d328 = 0;      /* g_hHeap */
int    (*DAT_14013d800)(int) = 0;    /* OleInitialize 槽 */
void   (*DAT_14013d808)(void) = 0;   /* OleUninitialize 槽 */
uint64_t DAT_14013e168[8] = {0}; /* CRITICAL_SECTION COM */
uint64_t DAT_14013e190[8] = {0}; /* CRITICAL_SECTION (g_csInit) */
uint32_t DAT_140147000 = 0x100;  /* g_runFlag 0x140147000: 00 01 00 00 */
/* PTR_u 标识符字符短串指针表 (.rdata, NULL 结尾): pe_data_extract 14013a2d0 */
uint64_t PTR_u_a__a_zA_Z0_9___14013a2d0[] = {
    0x1401233c0, 0x1401233e0, 0x1401233f8, 0x140123410, 0x140123428,
    0x140123448, 0x140123460, 0x140123490, 0x1401234b0, 0
};
void     FUN_14000500c(void);
void     FUN_140005738(int a, const char *b);
uint64_t FUN_140008c5c(uint64_t a, uint64_t b, void *c, void *d);
uint64_t FUN_14000befc(void *p);
uint64_t FUN_14005370c(uint32_t *a, void *b, uint64_t c);
void    *FUN_140057334(void *a, longlong b, int c, void *d, int e, int f, int g, int h, void *i, void *j, uint32_t k);
void    *FUN_14005762c(void *a, longlong b, int c, void *d, int e, int f, int g, int h, void *i, void *j, uint32_t k);
int      FUN_14005ceec(const uint16_t *a, const char *b);
void     FUN_14005d694(void);
void    *FUN_140060834(uint64_t a, uint64_t b);
void     FUN_14006355c(void *a, const WCHAR *b, int c, uint64_t d);
void     FUN_140063888(void *a, const WCHAR *b, int64_t c);
void    *FUN_14006e3a4(const WCHAR *a);
void    *FUN_14006e74c(const WCHAR *a, char b, uint32_t *c);
void    *FUN_14009c720(void *a, longlong b, int c, void *d, int e, int f, int g, int h, uint16_t *i, void *j, uint32_t k);
void    *FUN_14009cacc(void *a, longlong b, int c, void *d, int e, int f, int g, int h, void *i, void *j, uint32_t k, WCHAR *l);
uint64_t FUN_14009d4b8(uint64_t a, uint64_t b, const WCHAR *c, int16_t d, const WCHAR *e, int f);
void    *FUN_1400a43c4(const WCHAR *a, char b);
void    *FUN_1400aa144(void *a, longlong b, int c, void *d, int e, int f, int g, int h, uint32_t i, WCHAR *j, uint8_t k);
void    *FUN_1400b8f10(void *a, longlong b, int c, void *d, int e, int f, int g, int h, void *i, uint32_t j, int *k, const WCHAR *l);
void    *FUN_1400bca60(void *a, longlong b, int c, void *d, int e, int f, int g, int h, void *i, void *j, uint32_t *, uint32_t l, uint32_t m, uint32_t n);
int64_t  FUN_1400e5aac(longlong *a, void *b);
void    *RegisterServiceCtrlHandlerW(const WCHAR *name, void *handler);
int      SHGetSpecialFolderPathA(void *hwnd, char *buf, int folder, int create);
char    *lstrcatA(char *dst, const char *src);
typedef union _LARGE_INTEGER { struct { DWORD LowPart; LONG HighPart; } u; long long QuadPart; } LARGE_INTEGER;   /* wave-4 */
/* ---- wave-N p4 restored-helper extra Win32/minimal types (direct-port 支持) ---- */
typedef uint8_t byte;
typedef void (*code)(void);
typedef DWORD *LPDWORD;
typedef WCHAR *LPWCH;
typedef struct _RTL_CRITICAL_SECTION { void *DebugInfo; LONG LockCount; LONG RecursionCount; HANDLE OwningThread; HANDLE LockSemaphore; ULONG_PTR SpinCount; } RTL_CRITICAL_SECTION, CRITICAL_SECTION, *LPCRITICAL_SECTION, *PRTL_CRITICAL_SECTION;
typedef struct _PROCESS_INFORMATION { HANDLE hProcess; HANDLE hThread; DWORD dwProcessId; DWORD dwThreadId; } PROCESS_INFORMATION, *LPPROCESS_INFORMATION;
typedef struct _STARTUPINFOW { DWORD cb; LPWSTR lpReserved; LPWSTR lpDesktop; LPWSTR lpTitle; DWORD dwX; DWORD dwY; DWORD dwXSize; DWORD dwYSize; DWORD dwXCountChars; DWORD dwYCountChars; DWORD dwFillAttribute; DWORD dwFlags; WORD wShowWindow; WORD cbReserved2; void *lpReserved2; void *hStdInput; void *hStdOutput; void *hStdError; } STARTUPINFOW, *LPSTARTUPINFOW;
typedef struct _WIN32_FIND_DATAW { DWORD dwFileAttributes; FILETIME ftCreationTime; FILETIME ftLastAccessTime; FILETIME ftLastWriteTime; DWORD nFileSizeHigh; DWORD nFileSizeLow; DWORD dwReserved0; DWORD dwReserved1; WCHAR cFileName[260]; WCHAR cAlternateFileName[14]; } WIN32_FIND_DATAW, *LPWIN32_FIND_DATAW;
typedef struct tagPOINT { LONG x; LONG y; } POINT, *LPPOINT;
typedef struct tagRECT { LONG left; LONG top; LONG right; LONG bottom; } RECT, *LPRECT;
typedef struct tagMSG { HWND hwnd; UINT message; WPARAM wParam; LPARAM lParam; DWORD time; POINT pt; } MSG, tagMSG;
typedef void *pthreadlocinfo;
typedef struct _SHELLEXECUTEINFOW { DWORD cbSize; ULONG fMask; HWND hwnd; void *lpVerb; void *lpFile; void *lpParameters; void *lpDirectory; int nShow; void *hInstApp; void *lpIDList; void *lpClass; void *hkeyClass; DWORD dwHotKey; void *hIconOrMonitor; void *hProcess; } SHELLEXECUTEINFOW, *LPSHELLEXECUTEINFOW;

/* ---- wave-2 restored-helper 内部 FUN_ 前置声明 (定义在文件后部, 需先声明供 new 桩调用) ---- */
void        FUN_1400166b4(void);
int         FUN_140067d20(long long *, int *);
int         lstrlenW(const WCHAR *);
long long  *FUN_14005b154(long long *);
long long   FUN_140064a34(uint16_t *);
long long   FUN_140064a88(uint16_t *);
long long   FUN_14006b8fc(long long *);
void        FUN_1400ef91c(long long, uint, uint64_t);
void        FUN_140005344(void);
uint16_t   *FUN_14000531c(uint16_t *);
uint8_t    *FUN_14001d78c(uint8_t *, uint8_t *, int);
void        FUN_140008b2c(uint16_t *);
void        FUN_140017048(const WCHAR *);
void        FUN_140021144(const WCHAR *);
void        FUN_140019da8(long long, const WCHAR *, int);
void        FUN_140063694(void *, long long);
void        FUN_140025f10(long long, const WCHAR *, uint32_t, char *, char *, long long *);
void        FUN_140020fd4(long long, char, const WCHAR *, const WCHAR *);
extern uint8_t DAT_14011c638[64];
uint64_t    FUN_14005ea5c(void);
uint        FUN_14000e0bc(void);
uint64_t    FUN_14006042c(void);
uint64_t    FUN_14001d628(void);
LPWSTR      StrRChrW(const WCHAR *, const WCHAR *, WCHAR);
uint64_t    thunk_FUN_1400f429c(void *, short);
/* ---- wave-7 (P4 [240:288]) 内部依赖前置声明 (真实体见文件后部 wave-7 节) ---- */
int         FUN_140067cf4(long long *, uint64_t *);   /* @0x140067cf4 数值解析包装 */
void        FUN_1400706b4(uint32_t *);                 /* @0x1400706b4 控件表初始化 */
uint        FUN_14001b608(uint64_t);                   /* @0x14001b608 注册表读取 */
long long   FUN_140031454(long long *, pthreadmbcinfo);/* @0x140031454 INDATA 串执行 */
void        FUN_140063a6c(uint64_t *, int64_t *, uint64_t *, uint); /* @0x140063a6c 表初始化 */
extern long long DAT_14013d130;                        /* 传 PECMD_ExpandCommandLine 输出槽 (后部定义) */

uint64_t AbortSystemShutdownW(void) { return 0; }
uint64_t AddFontMemResourceEx(void) { return 0; }
uint64_t AddFontResourceW(void) { return 0; }
uint64_t AdjustTokenPrivileges(void) { return 0; }
uint64_t AllocConsole(void) { return 0; }
uint64_t AllocateAndInitializeSid(void) { return 0; }
uint64_t AppendMenuW(void) { return 0; }
uint64_t AssignProcessToJobObject(void) { return 0; }
uint64_t AttachThreadInput(void) { return 0; }
uint64_t Beep(void) { return 0; }
uint64_t BeginPaint(void) { return 0; }
uint64_t BitBlt(void) { return 0; }
uint64_t BringWindowToTop(void) { return 0; }
uint64_t CM_Get_Parent(void) { return 0; }
uint64_t CallNextHookEx(void) { return 0; }
uint64_t CallWindowProcW(void) { return 0; }
uint64_t ChangeDisplaySettingsExW(void) { return 0; }
uint64_t ChangeServiceConfig2W(void) { return 0; }
uint64_t ChangeServiceConfigW(void) { return 0; }
uint64_t CharUpperW(void) { return 0; }
uint64_t CheckTokenMembership(void) { return 0; }
uint64_t ClientToScreen(void) { return 0; }
uint64_t CloseClipboard(void) { return 0; }
uint64_t CloseHandle(void) { return 0; }
uint64_t CloseServiceHandle(void) { return 0; }
uint64_t ControlService(void) { return 0; }
uint64_t CopyFileW(void) { return 0; }
uint64_t CopyImage(void) { return 0; }
uint64_t CreateBitmap(void) { return 0; }
uint64_t CreateCompatibleBitmap(void) { return 0; }
uint64_t CreateCompatibleDC(void) { return 0; }
uint64_t CreateDialogParamW(void) { return 0; }
uint64_t CreateDirectoryW(void) { return 0; }
HRGN CreateEllipticRgn(int a, int b, int c, int d) { (void)a;(void)b;(void)c;(void)d; return (HRGN)0; }
uint64_t CreateEventW(void) { return 0; }
uint64_t CreateFileMappingA(void) { return 0; }
uint64_t CreateFileMappingW(void) { return 0; }
uint64_t CreateFileW(void) { return 0; }
uint64_t CreateFileA(void) { return 0; }
uint64_t CreateFontW(void) { return 0; }
uint64_t CreateHardLinkW(void) { return 0; }
uint64_t CreateMutexA(void) { return 0; }
uint64_t CreateMutexW(void) { return 0; }
uint64_t CreatePen(void) { return 0; }
uint64_t CreatePopupMenu(void) { return 0; }
uint64_t CreateProcessW(void) { return 0; }
uint64_t CreateRoundRectRgn(void) { return 0; }
uint64_t CreateServiceW(void) { return 0; }
HBRUSH CreateSolidBrush(COLORREF color) { (void)color; return (HBRUSH)0; }
void *CreateThread(void *sa, size_t st, void *rt, void *p, uint32_t f, uint32_t *tid) { (void)sa;(void)st;(void)rt;(void)p;(void)f;(void)tid; return (void*)0; }
uint64_t CreateToolhelp32Snapshot(void) { return 0; }
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
uint64_t DeleteDC(void) { return 0; }
uint64_t DeleteFileW(void) { return 0; }
int DeleteObject(void *obj) { (void)obj; return 1; }
uint64_t DeleteService(void) { return 0; }
uint64_t DestroyIcon(void) { return 0; }
uint64_t DestroyMenu(void) { return 0; }
int DestroyWindow(void *h) { (void)h; return 0; }
uint64_t DeviceIoControl(void) { return 0; }
uint64_t DialogBoxIndirectParamW(void) { return 0; }
uint64_t DialogBoxParamW(void) { return 0; }
uint64_t DispatchMessageW(const void *m) { (void)m; return 0; }
uint64_t DragAcceptFiles(void) { return 0; }
uint64_t DrawEdge(void) { return 0; }
uint64_t DrawFocusRect(void) { return 0; }
uint64_t DrawFrameControl(void) { return 0; }
uint64_t DrawIcon(void) { return 0; }
uint64_t DrawIconEx(void) { return 0; }
uint64_t DrawTextW(void) { return 0; }
uint64_t DuplicateHandle(void) { return 0; }
uint64_t Ellipse(void) { return 0; }
uint64_t EmptyClipboard(void) { return 0; }
uint64_t EnableMenuItem(void) { return 0; }
uint64_t EnableWindow(void) { return 0; }
uint64_t EndDialog(void) { return 0; }
uint64_t EndPaint(void) { return 0; }
void EnterCriticalSection(void *cs) { (void)cs; }
uint64_t EnumChildWindows(void) { return 0; }
uint64_t EnumDisplayDevicesW(void) { return 0; }
uint64_t EnumDisplaySettingsW(void) { return 0; }
uint64_t EnumWindows(void) { return 0; }
void ExitProcess(uint32_t code) { (void)code; }
uint64_t ExitWindowsEx(void) { return 0; }
uint64_t ExitWindowsEx_exref(void) { return 0; }
uint64_t ExpandEnvironmentStringsW(void) { return 0; }
uint64_t ExtTextOutW(void) { return 0; }
uint64_t PECMD_RunCommandLine(void) { return 0; }
void FUN_140006554(void *param_1, int64_t *param_2, int64_t *param_3) { (void)param_1; (void)param_2; (void)param_3; }
void FUN_1400083c0(void){
    void *hModule; char local_118[272];
    FUN_140005738(0,"PECMD:MSGSVR:HWND");
    FUN_14000500c();
    DAT_14013c96a=1;
    FUN_140053e78();
    FUN_14005d694();
    DAT_14013c9f8=RegisterWindowMessageW((const WCHAR*)L"TaskbarButtonCreated");
    local_118[0]='\0';
    SHGetSpecialFolderPathA((void*)0,local_118,0x24,0);
    lstrcatA(local_118,"\\System32\\Kernel32");
    hModule=GetModuleHandleA(local_118);
    DAT_14013cfd0=(void*)hModule;
    DAT_14013cfc8=LoadLibraryA("user32");
    DAT_14013cfd8=LoadLibraryA("GDI32");
    DAT_14013c970=(int (*)(uint32_t,uint32_t,char*,int))(uintptr_t)GetProcAddress(hModule,"MultiByteToWideChar");
    FUN_1400669c4((int64_t*)&DAT_14013d130,(uint32_t)DAT_14013a038,(const uint16_t*)L"&bX64");
    FUN_1400669c4((int64_t*)&DAT_14013d130,8,(const uint16_t*)L"&ptrlen");
    FUN_1400669c4((int64_t*)&DAT_14013d130,(uint32_t)DAT_14013c9f8,(const uint16_t*)L"&WM_TASKBARBUTTONCREATED");
}

uint64_t FUN_140008834(void) { return 0; }
uint64_t FUN_14000e26c(void) { return 0; }
/* PECMD_LangLookupById — 按参数序号在 argv 表中定位 VALUE (形如 "id=value|id=value|...")
   定位到匹配 id 时返回其值指针(跳过 '|' 前缀); 未命中返回 NULL.
   依赖: FUN_1400166b4 初始化 argv 表; FUN_140067d20 数值解析; lstrlenW. */
LPCWSTR DAT_14013ca78;   /* argv 表指针 (由 FUN_1400166b4 惰性初始化, 初 0) */
int64_t DAT_14013ca80;   /* argv 表末端字节偏移 (初 0) */
void FUN_1400166b4(void) { /* 初始化 argv 表 (缺失 helper, no-op 桩) */ }
LPCWSTR PECMD_LangLookupById(int param_1, LPCWSTR *param_2)
{
    int   iVar2;
    WCHAR WVar3;
    LPCWSTR pWVar4;
    int   local_res18[2] = {0};
    LPCWSTR local_res20;

    if (DAT_14013ca78 == (LPCWSTR)0) {
        FUN_1400166b4();
    }
    if (*DAT_14013ca78 != L'\0') {
        pWVar4 = (LPCWSTR)((int64_t)DAT_14013ca80 + (int64_t)DAT_14013ca78);
        local_res20 = DAT_14013ca78;
        while (local_res20 < pWVar4) {
            local_res18[0] = (int)0x80000000u;
            if (FUN_140067d20((long long *)&local_res20, local_res18) &&
                (local_res18[0] == param_1)) {
                local_res20 = local_res20 + 1;
                WVar3 = *local_res20;
                pWVar4 = local_res20;
                while (1) {
                    if (WVar3 == L'\0') {
                        return local_res20;
                    }
                    if (WVar3 == L'|') break;
                    pWVar4 = pWVar4 + 1;
                    WVar3 = *pWVar4;
                }
                if (param_2 != (LPCWSTR *)0) {
                    *param_2 = local_res20;
                }
                return pWVar4 + 1;
            }
            iVar2 = lstrlenW(local_res20);
            for (local_res20 = local_res20 + (long long)iVar2 + 1; *local_res20 == L'\0';
                 local_res20 = local_res20 + 1) {
                if (pWVar4 <= local_res20) {
                    return (LPCWSTR)0;
                }
            }
        }
    }
    return (LPCWSTR)0;
}
void FUN_140016ae0(undefined8 param_1, undefined8 *param_2){
    WCHAR *lpParameter; DWORD local_res18[4];
    (void)param_1;
    DAT_14013c928=0; DAT_14013c924=0; DAT_14013c92c=0; DAT_14013c930=0;
    DAT_14013c918=0x30; DAT_14013c91c=2; DAT_14013c920=3;
    DAT_14013c910=RegisterServiceCtrlHandlerW((const WCHAR*)(uintptr_t)*param_2,(void*)FUN_140008c5c);
    DAT_14013c92c=0; DAT_14013c930=0; DAT_14013c91c=4;
    SetServiceStatus(DAT_14013c910,(void*)&DAT_14013c918);
    lpParameter=GetCommandLineW(); local_res18[0]=0; DAT_140147000=0;
    CreateThread((void*)0,0,(void*)FUN_14000befc,lpParameter,0,(uint32_t*)local_res18);
    DAT_14013c92c=0; DAT_14013c930=0; DAT_14013c91c=4;
    SetServiceStatus(DAT_14013c910,(void*)&DAT_14013c918);
}

void FUN_1400171a4(int64_t a) { (void)a; }
uint64_t FUN_140017724(void) { return 0; }
void FUN_140017f54(int *p) { (void)p; }
uint64_t FUN_140018220(void) { return 0; }
uint64_t PECMD_ScriptInit(void) { return 0; }
uint64_t FUN_140018c6c(void) { return 0; }
void FUN_140018d8c(uint64_t ctx, const uint16_t *fmt, uint64_t a, uint64_t b) { (void)ctx;(void)fmt;(void)a;(void)b; }
uint64_t PECMD_WaitHandlesOrMessages(uint64_t param_1, int64_t param_2, int param_3, uint64_t *param_4) { (void)param_1;(void)param_2;(void)param_3;(void)param_4; return 0; }
uint64_t FUN_14001a56c(void) { return 0; }
uint64_t FUN_14001a640(void) { return 0; }
undefined4 FUN_14001ab84(const WCHAR *param_1){
    WCHAR WVar1; int iVar2;
    iVar2=StrCmpNIW((const WCHAR*)L"TEAM",param_1,4);
    if (((iVar2!=0)||((((WVar1=param_1[4],(ushort)WVar1<9)||(0xd<(ushort)WVar1))&&(WVar1!=L' '))&&(WVar1!=L'\0')))) {
        iVar2=StrCmpNIW((const WCHAR*)L"EXEC",param_1,4);
        if (iVar2!=0) {
            iVar2=StrCmpNIW((const WCHAR*)L"LOAD",param_1,4);
            if (iVar2!=0) return 0;
            WVar1=param_1[4];
            if ((((ushort)WVar1<9)||(0xd<(ushort)WVar1))&&((WVar1!=L' ')&&(WVar1!=L'\0'))) return 0;
        }
    }
    return 1;
}

uint64_t FUN_14001b3a0(void) { return 0; }
uint64_t PECMD_XorEncode(void) { return 0; }
void FUN_14001b660(void *script)   /* @0x14001b660 定时清理注册表: 一次性 "ClearTmpMBROS" (decompiled.c) */
{
    uint uVar1;
    WCHAR local_b8[30];

    if (DAT_14013d260 == '\0') {
        DAT_14013d260 = 1;
        uVar1 = FUN_14001b608((uint64_t)(uintptr_t)L"CheckTmpMBROS");
        if (uVar1 == 0) {
            memcpy(local_b8, L"#22:INDATA  ClearTmpMBROSAll", 0x3c);
            FUN_140031454((long long *)script, (pthreadmbcinfo)local_b8);
        }
    }
}
uint64_t FUN_14001b850(void) { return 0; }
uint64_t FUN_14001b888(void) { return 0; }
uint64_t FUN_14001be14(void) { return 0; }
uint64_t PECMD_ExpandDrivePathAlloc(void) { return 0; }
void PECMD_SyncWorkingDirectory(void)
{
    WCHAR   *local_res8[2];
    WCHAR   local_218[264];
    int     iVar1;
    longlong lVar2;

    EnterCriticalSection((void *)&DAT_14013e190);
    FUN_140063694((void *)local_res8, 0x20a);
    GetCurrentDirectoryW(0x208, local_res8[0]);
    local_218[0] = L'\\0';
    GetEnvironmentVariableW((const WCHAR *)L"_CD", local_218, 0x208);
    iVar1 = lstrcmpiW(local_res8[0], local_218);
    if (iVar1 != 0) {
        SetEnvironmentVariableW((const WCHAR *)L"_CD", local_res8[0]);
    }
    lVar2 = FUN_140018978((int64_t *)&DAT_14013d130, (const uint16_t *)L"&_CD", (longlong *)0, -1, (void *)0);
    if (lVar2 != 0) {
        iVar1 = lstrcmpiW(local_res8[0], *(LPCWSTR *)(uintptr_t)(lVar2 + 8));
        if (iVar1 == 0) goto LAB_14001e3a7;
    }
    PECMD_SetVariable((void *)&DAT_14013d130, (const WCHAR *)L"&_CD", local_res8[0]);
LAB_14001e3a7:
    FUN_14005b104((longlong *)local_res8);
    LeaveCriticalSection((void *)&DAT_14013e190);
}
uint64_t PECMD_AddVarDefault(void) { return 0; }
uint64_t PECMD_FindVarValue(void) { return 0; }
uint64_t PECMD_SetVarCore(void) { return 0; }
uint8_t *FUN_14001ea18(void *a, uint16_t *b, uint16_t *c, int64_t *d, unsigned int *e) { (void)a;(void)b;(void)c;(void)d;(void)e; return (uint8_t*)0; }
/* FUN_14002286c — KnownDlls32 环境修复: 首个调用时读取系统模式标志, 在 64 位
   系统中注册 \\KnownDlls32 路径(经 ntdll 函数指针槽), 仅执行一次. */
uint FUN_14000e0bc(void) { return 0; }           /* 操作系统位宽探测 (no-op) */
uint64_t FUN_14006042c(void) { return 0; }       /* 系统目录盘符 (no-op) */
uint64_t FUN_14001d628(void) { return 0; }       /* (no-op) */
uint8_t DAT_14013d270;                           /* 一次性初始化标志 (静区, 初 0) */
void (*DAT_14013cb48)(...) = 0;                  /* RtlInitUnicodeString 类槽 (reroute, 初 0) */
int  (*DAT_14013cd30)(...) = 0;                  /* ZwOpenKey 类槽 */
int  (*DAT_14013cd38)(...) = 0;                  /* ZwQueryKey 类槽 */
void (*DAT_14013cd40)(...) = 0;                  /* ZwClose 类槽 */
void FUN_14002286c(void)
{
    uint uVar1;
    int iVar2;
    ulonglong uVar3;
    undefined8 uVar4;
    longlong local_res8;
    longlong local_res10 [3];
    undefined4 local_c8 [2];
    undefined8 local_c0;
    undefined1 *local_b8;
    undefined4 local_b0;
    undefined8 local_a8;
    undefined8 local_a0;
    undefined4 local_98 [2];
    undefined8 local_90;
    undefined1 *local_88;
    undefined4 local_80;
    undefined8 local_78;
    undefined8 local_70;
    undefined1 local_68 [16];
    undefined1 local_58 [16];
    undefined1 local_48 [16];
    undefined2 local_38 [28];

    if (DAT_14013d270 == 0) {
        DAT_14013d270 = 1;
        uVar3 = FUN_14005ea5c();
        if (0x9ffffffffULL < uVar3) {
            uVar1 = FUN_14000e0bc();
            if (uVar1 != 0) {
                memcpy(local_38, (const void *)L"X:\\Windows\\SysWow64", 0x28);
                uVar4 = FUN_14006042c();
                local_38[0] = (undefined2)uVar4;
                FUN_14001d628();
                (*DAT_14013cb48)(local_48, (void *)L"\\KnownDlls32");
                (*DAT_14013cb48)(local_58, (void *)L"\\KnownDlls32\\KnownDllPath");
                (*DAT_14013cb48)(local_68, local_38);
                local_90 = 0;
                local_78 = 0;
                local_70 = 0;
                local_c0 = 0;
                local_a8 = 0;
                local_a0 = 0;
                local_res8 = 0;
                local_res10[0] = 0;
                local_88 = local_48;
                local_b8 = local_58;
                local_80 = 0x50;
                local_b0 = 0x50;
                local_98[0] = 0x30;
                local_c8[0] = 0x30;
                iVar2 = (*DAT_14013cd30)(&local_res8, 0xf000f, local_98);
                if ((-1 < iVar2) && (local_res8 != 0)) {
                    iVar2 = (*DAT_14013cd38)(local_res10, 0xf0001, local_c8, local_68);
                    if ((-1 < iVar2) && (local_res10[0] != 0)) {
                        (*DAT_14013cd40)();
                    }
                    (*DAT_14013cd40)(local_res8);
                }
            }
        }
    }
}
uint64_t FUN_140022e94(void) { return 0; }
uint64_t FUN_140023544(void) { return 0; }
uint16_t *PECMD_NextToken(int64_t *a, int64_t *b, uint32_t c) { (void)a;(void)b;(void)c; return (uint16_t *)0; }
uint64_t FUN_1400250f0(void) { return 0; }
void FUN_140025f10(long long p1, const WCHAR *p2, uint32_t p3, char *p4, char *p5, long long *p6) { (void)p1;(void)p2;(void)p3;(void)p4;(void)p5;(void)p6; }
uint64_t FUN_140026338(void) { return 0; }
uint64_t FUN_14002ca30(void) { return 0; }
uint64_t FUN_14002cc30(void) { return 0; }
uint64_t FUN_14002e30c(void) { return 0; }
void PECMD_SendHotkeyKeyMessage(uint32_t a, int b, int c) { (void)a;(void)b;(void)c; }   /* 签名修正: uint32_t,int,int */
uint64_t FUN_14003e1f0(void) { return 0; }
void FUN_14004c0bc(uint64_t script, uint64_t cmd, void *p3, void *p4, void *p5) { (void)script;(void)cmd;(void)p3;(void)p4;(void)p5; }
void PECMD_ClearTaskTable(undefined8 param_1, int param_2){
    int iVar1;
    if (param_2<0) param_2=0;
    if (*(longlong*)(uintptr_t)param_1!=0) {
        iVar1=*(int*)(uintptr_t)(param_1+8);
        while (param_2<iVar1) {
            *(int*)(uintptr_t)(param_1+8)=*(int*)(uintptr_t)(param_1+8)-1;
            if (*(longlong*)(uintptr_t)(*(longlong*)(uintptr_t)param_1+(longlong)*(int*)(uintptr_t)(param_1+8)*8)!=0) {
                FUN_14004e2cc(param_1,*(longlong**)(uintptr_t)(*(longlong*)(uintptr_t)param_1+(longlong)*(int*)(uintptr_t)(param_1+8)*8));
                *(undefined8*)(uintptr_t)(*(longlong*)(uintptr_t)param_1+(longlong)*(int*)(uintptr_t)(param_1+8)*8)=0;
            }
            iVar1=*(int*)(uintptr_t)(param_1+8);
        }
        if (*(int*)(uintptr_t)(param_1+8)<1) {
            if (*(longlong*)(uintptr_t)param_1!=0) HeapFree(DAT_14013d328,0,(void*)(uintptr_t)(*(longlong*)(uintptr_t)param_1-8));
            *(undefined8*)(uintptr_t)param_1=0;
        }
    }
    FUN_14006e8f4((int64_t)param_1);
}

uint64_t PECMD_ArrayPrepend(void) { return 0; }
void PECMD_SetObjectEnable(int64_t a, unsigned int b) { (void)a; (void)b; }
undefined8 *FUN_140053dc8(undefined8 *param_1, undefined4 param_2, undefined8 param_3, LPCWSTR param_4, LPCWSTR param_5, LPCWSTR param_6, undefined4 param_7, undefined4 param_8, undefined4 param_9, undefined4 param_10){
    *(undefined4*)(param_1+1)=(undefined4)param_2;
    *param_1=(undefined8)&PTR_FUN_1401234f0;
    FUN_1400702b0((uint16_t**)(param_1+2),param_4);
    param_1[3]=0;
    FUN_14006355c(param_1+3,param_4,-1,0xffffffffffffffffULL);
    FUN_1400702b0((uint16_t**)(param_1+4),param_6);
    FUN_1400702b0((uint16_t**)(param_1+5),param_5);
    param_1[6]=0; param_1[7]=0; param_1[10]=param_3;
    *(undefined4*)(param_1+8)=param_7;
    *(undefined4*)((longlong)param_1+0x44)=param_8;
    *(undefined4*)(param_1+9)=param_9;
    *(undefined4*)((longlong)param_1+0x4c)=param_10;
    param_1[0xb]=param_3; param_1[0xc]=0;
    *(undefined4*)(param_1+0xd)=0;
    return param_1;
}

void FUN_140053e78(void) { }
uint64_t FUN_1400545f8(void) { return 0; }
uint64_t FUN_1400547bc(void) { return 0; }
uint64_t FUN_14005b0b8(void) { return 0; }
void FUN_14005b104(void *ps) { (void)ps; }
int FUN_14005b184(char *a, int64_t b, int64_t c) { (void)a;(void)b;(void)c; return 0; }
undefined8 PECMD_MatchPrefixN(ushort *param_1, undefined8 *param_2, int param_3){
    ushort uVar1,uVar2; ushort *local_res10[3];
    local_res10[0]=(ushort*)(uintptr_t)*param_2;
    while (param_3=param_3-1, -1<param_3) {
        uVar1=*param_1; param_1=param_1+1;
        uVar2=*local_res10[0]; local_res10[0]=local_res10[0]+1;
        if (uVar1!=uVar2) return 0;
    }
    if ((*local_res10[0]!=0)&&(((*local_res10[0]<9)||(0xd<*local_res10[0]))&&(*local_res10[0]!=0x20))) return 0;
    FUN_14005b154((long long*)local_res10);
    *param_2=(undefined8)(uintptr_t)local_res10[0];
    return 1;
}

uint64_t FUN_14005b228(void) { return 0; }
uint64_t PECMD_LoadLocalizedString(void) { return 0; }
void PECMD_DestroyTrayIcon(char *param_1) { (void)param_1; }
uint64_t PECMD_UpdateTrayIcon(void) { return 0; }
DWORD PECMD_QueryRegValueWithRetry(HKEY param_1, const WCHAR *param_2, const WCHAR *param_3, DWORD *param_4, BYTE *param_5, DWORD *param_6){
    DWORD DVar1; uint uVar2; intptr_t hlocal[2];
    uVar2=4; hlocal[0]=0;
    do {
        DVar1=FUN_14005c394(param_1,param_2,(HKEY*)hlocal,0x20019,uVar2);
        if (DVar1!=5) {
            if (DVar1!=0) return DVar1;
            DVar1=RegQueryValueExW((void*)(uintptr_t)hlocal[0],param_3,(DWORD*)0,param_4,param_5,param_6);
            RegCloseKey((void*)(uintptr_t)hlocal[0]);
            if (DVar1!=5) return DVar1;
        }
        if (uVar2==0) return DVar1;
        uVar2=0;
    } while (1);
}

uint64_t FUN_14005c5a0(void) { return 0; }
uint64_t PECMD_RegDeleteValue(void) { return 0; }
longlong FUN_14005c72c(char *param_1, ushort *param_2, int param_3){
    char cVar1; ushort uVar2; uint uVar3; longlong lVar4;
    lVar4=0;
    do {
        param_3=param_3-1;
        if (param_3<0) {
            if (((8<*param_2)&&(*param_2<0xe))||((*param_2==0x20)||(*param_2==0))) lVar4=1;
            return lVar4;
        }
        cVar1=*param_1; uVar3=(uint)cVar1; uVar2=*param_2;
        param_1=param_1+1; param_2=param_2+1;
    } while (((0x40<(ushort)(short)cVar1)||(uVar2==(ushort)(short)cVar1))&&((ushort)((short)cVar1|0x20)==(uVar2|0x20)));
    return ((ulonglong)(uVar3>>8))<<8;
}

int PECMD_AsciiPrefixICmp(const char *a, const uint16_t *w, int n) { (void)a;(void)w;(void)n; return 0; }
uint64_t PECMD_AsciiWideICmp(void) { return 0; }
void FUN_14005c828(LPCSTR param_1, LPCSTR param_2, longlong *param_3, longlong *param_4){
    void *pFVar1; longlong *plVar2; longlong local_res18; void *hm;
    pFVar1=(void*)0; local_res18=0;
    if (*param_3!=0) return;
    plVar2=&local_res18;
    if (param_4!=(longlong*)0) plVar2=param_4;
    hm=(void*)(uintptr_t)*plVar2;
    if (hm==(void*)0) { hm=LoadLibraryA(param_2); *plVar2=(longlong)hm; if (hm==(void*)0) goto LAB_14005c880; }
    pFVar1=GetProcAddress(hm,param_1);
LAB_14005c880:
    *param_3=(longlong)pFVar1;
}

int64_t PECMD_EnableTokenPrivilege(LPCWSTR a, DWORD b, uint32_t c) { (void)a;(void)b;(void)c; return 0; }   /* @0x14001c2cc EnablePrivilege 桩 */
void PECMD_ReleaseGdiObjects(undefined8 *param_1, HWND param_2){
    void **puVar1;
    puVar1=(void**)(uintptr_t)*param_1;
    if (puVar1!=(void**)0) {
        if ((void*)puVar1[0]!=(void*)0) { DeleteObject((void*)puVar1[0]); puVar1[0]=0; }
        if ((void*)puVar1[2]!=(void*)0) { DeleteObject((void*)puVar1[2]); puVar1[2]=0; }
        if ((puVar1[1]!=0)&&(param_2!=(HWND)0)) { SetWindowRgn(param_2,(void*)0,0); DeleteObject((void*)puVar1[1]); puVar1[1]=0; }
        if ((void*)puVar1[3]!=(void*)0) { DeleteObject((void*)puVar1[3]); puVar1[3]=0; }
    }
}

void PECMD_DialogBeepNotify(int64_t a, int b) { (void)a;(void)b; }
void FUN_14005daf8(int64_t a, int *b, int *c, int *d, int *e) { (void)a;(void)b;(void)c;(void)d;(void)e; }
uint64_t FUN_14005dec4(void) { return 0; }
uint64_t FUN_14005ded4(void) { return 0; }
uint64_t PECMD_NextRandomSeed(void) { return 0; }
void FUN_14005e7dc(uint64_t *param_1) { (void)param_1; }
uint64_t FUN_140061470(void) { return 0; }
void FUN_140061c44(void) { }
uint64_t FUN_140061ffc(uint64_t a, int b, uint16_t *c) { (void)a;(void)b;(void)c; return 0; }
void PECMD_SetVariable(void *a, const WCHAR *b, const WCHAR *c) { (void)a;(void)b;(void)c; }
void FUN_1400633a8(void **ps, int64_t len) { (void)ps;(void)len; }
uint16_t *FUN_140063620(uint16_t **out) { (void)out; return (uint16_t *)0; }
uint16_t *FUN_14006375c(uint16_t **ps, const uint16_t *src) { (void)ps;(void)src; return (uint16_t *)0; }
WCHAR *PECMD_StrDupA(WCHAR **ps, LPCWSTR src, int64_t a, int64_t b){ (void)ps;(void)src;(void)a;(void)b; return (WCHAR*)0; } /* @0x1400637dc 字符串追加 */
void FUN_1400639f0(int64_t *a, int64_t *b, int64_t *c, void *d, int e, int f) { (void)a;(void)b;(void)c;(void)d;(void)e;(void)f; }
void FUN_140063a6c(uint64_t *param_1, int64_t *param_2, uint64_t *param_3, uint param_4) { (void)param_1;(void)param_2;(void)param_3;(void)param_4; }
int64_t FUN_140063b00(int64_t param_1, int64_t *param_2, int64_t *param_3, unsigned int param_4) { (void)param_1;(void)param_2;(void)param_3;(void)param_4; return 0; }
longlong *FUN_140063b64(longlong *param_1){
    longlong lVar1,lVar4; int iVar2; undefined8 *puVar3;
    param_1[2]=0; *param_1=0; param_1[1]=1;
    do { puVar3=(undefined8*)HeapAlloc(DAT_14013d328,0,0x10); if (puVar3!=0) break; iVar2=FUN_1400630d0(2); } while (iVar2==4);
    *(undefined4*)((longlong)puVar3+4)=0xaa55; *(undefined8*)puVar3=8;
    lVar1=param_1[1]; lVar4=0; *param_1=(longlong)(puVar3+1);
    if (0<lVar1) { do { *(undefined8*)(uintptr_t)(*param_1+lVar4*8)=0; lVar4++; } while (lVar4<lVar1); }
    return param_1;
}

uint64_t PECMD_ExpandDrivePath(void) { return 0; }
char FUN_1400660ac(char *a, long long *b, int c) { (void)a;(void)b;(void)c; return 0; }
uint64_t FUN_1400668ec(void) { return 0; }
void FUN_14006764c(longlong *param_1, longlong *param_2, short param_3, short param_4){
    WCHAR WVar1; const WCHAR *pWVar2; short *psVar3; int iVar4; const WCHAR *pWVar5;
    FUN_14005b154((long long*)param_1);
    pWVar2=(const WCHAR*)(uintptr_t)*param_1;
    WVar1=*pWVar2;
    while ((WVar1!=L'\0') && (psVar3=(short*)(uintptr_t)*param_1, *psVar3!=param_3) && (*psVar3!=param_4)) {
        *param_1=(longlong)(psVar3+1);
        WVar1=(WCHAR)psVar3[1];
    }
    pWVar5=(const WCHAR*)(uintptr_t)*param_1;
    do { pWVar5=pWVar5-1; if (pWVar5<pWVar2) break; } while (((8<(ushort)*pWVar5)&&((ushort)*pWVar5<0xe))||(*pWVar5==L' '));
    iVar4=(int)(((longlong)pWVar5+(2-(longlong)pWVar2))>>1);
    FUN_140063888(param_2,pWVar2,(longlong)(iVar4+1));
    *(undefined2*)(uintptr_t)(*param_2+(longlong)iVar4*2)=0;
}

uint64_t FUN_1400679b0(void) { return 0; }
/* PECMD_ParseIntegerString — 解析带符号/进制前缀 (0x/0o/0b) 的十进制-整数字串.
   跳过前导空白后解析并写回 *param_2; 失败返回 0. */
long long *FUN_14005b154(long long *param_1) { (void)param_1; return param_1; } /* 跳前导空白 */
long long FUN_140064a34(uint16_t *s) { (void)s; return 0; } /* 解析 16 进制数字串 */
uint64_t PECMD_ParseIntegerString(long long *param_1, uint64_t *param_2)
{
    uint64_t  uVar1;
    uint16_t *puVar2;
    uint16_t  uVar3;
    long long *plVar4;
    char      cVar5;

    plVar4 = param_1;
    FUN_14005b154(param_1);
    puVar2 = (uint16_t *)*plVar4;
    cVar5 = '\0';
    if (*puVar2 == 0x2d) { puVar2 = puVar2 + 1; cVar5 = (char)-1; }
    if (*puVar2 == 0x30) {
        uVar3 = (uint16_t)(puVar2[1] | 0x20);
        if (uVar3 == 0x78) {
            *param_2 = 0;
            puVar2 = puVar2 + 2;
            uVar1 = FUN_140064a34(puVar2);
            *param_2 = uVar1;
            *param_1 = (long long)puVar2;
            while (1) {
                uVar3 = *(uint16_t *)*param_1;
                if (((uVar3 < 0x30) || (0x39 < uVar3)) &&
                    (5 < (uint16_t)((uVar3 | 0x20) - 0x61))) break;
                *param_1 = (long long)((uint16_t *)*param_1 + 1);
            }
            goto LAB_140067cc9;
        }
        if (uVar3 == 0x6f) {
            *param_2 = 0;
            puVar2 = puVar2 + 2;
            *param_1 = (long long)puVar2;
            while ((0x2f < *puVar2 && (*(uint16_t *)*param_1 < 0x38))) {
                *param_2 = (long long)(int)(*(uint16_t *)*param_1 - 0x30) | *param_2 << 3;
                *param_1 = *param_1 + 2;
                puVar2 = (uint16_t *)*param_1;
            }
            goto LAB_140067cc9;
        }
        if (uVar3 == 0x62) {
            *param_2 = 0;
            puVar2 = puVar2 + 2;
            *param_1 = (long long)puVar2;
            while ((0x2f < *puVar2 && (*(uint16_t *)*param_1 < 0x32))) {
                *param_2 = (long long)(int)(*(uint16_t *)*param_1 - 0x30) | *param_2 * 2;
                *param_1 = *param_1 + 2;
                puVar2 = (uint16_t *)*param_1;
            }
            goto LAB_140067cc9;
        }
    }
    if (9 < (uint16_t)(*puVar2 - 0x30)) {
        return 0;
    }
    uVar1 = FUN_140064a88(puVar2);
    *param_2 = uVar1;
    *param_1 = (long long)puVar2;
    uVar3 = *puVar2;
    while ((0x2f < uVar3 && (*(uint16_t *)*param_1 < 0x3a))) {
        puVar2 = (uint16_t *)*param_1 + 1;
        *param_1 = (long long)puVar2;
        uVar3 = *puVar2;
    }
LAB_140067cc9:
    if (cVar5 != '\0') {
        *param_2 = -*param_2;
    }
    return 1;
}
int FUN_140067d20(long long *param_1, int *param_2)   /* @0x140067d20 数值解析包装 (decompiled.c 直移) */
{
    int bVar1;
    uint64_t local_res10[3];

    local_res10[0] = (uint64_t)(uint32_t)*param_2;
    bVar1 = FUN_140067cf4(param_1, local_res10);
    if (bVar1 != 0) {
        *param_2 = (int)local_res10[0];
    }
    return bVar1 != 0;
}
uint64_t FUN_14006a7f4(int64_t *a, uint64_t *b) { (void)a;(void)b; return 0; }
uint64_t PECMD_EncodeStringId(void) { return 0; }
uint64_t FUN_14006f884(void) { return 0; }
void FUN_14006fd1c(int64_t *a,int64_t *b,int64_t *c,int64_t *d,int64_t *e,int64_t *f) { (void)a;(void)b;(void)c;(void)d;(void)e;(void)f; }
uint64_t FUN_140070154(void) { return 0; }
void PECMD_StrDupAssign(uint16_t **ps, const uint16_t *src) { (void)ps;(void)src; }
uint16_t *FUN_1400702d4(uint16_t **out, const uint16_t *src, int64_t len) { (void)out;(void)src;(void)len; return (uint16_t *)0; }
int64_t *FUN_1400702f0(int64_t *a, char *b, uint64_t c) { (void)a;(void)b;(void)c; return a; }
int64_t *PECMD_AssignString(int64_t *param_1, const uint16_t *param_2) { (void)param_1;(void)param_2; return (int64_t *)0; }
void FUN_1400703e4(long long *a, const WCHAR *b) { (void)a;(void)b; }
void *FUN_1400731d8(void *a, void *b, uint32_t c, void *d) { (void)a;(void)b;(void)c;(void)d; return (void*)0; }
uint64_t PECMD_ParsePrefixColon(void) { return 0; }
uint64_t FUN_1400745c8(void) { return 0; }
uint64_t FUN_140077358(void) { return 0; }
/* PECMD_ParseResourceStringRef — 解析 "路径#索引" 形式的资源串引用: 定位 '#' 与数字索引并返回其
   起始位置; 带引号时剥离引号. 未匹配返回 NULL. */
LPCWSTR PECMD_ParseResourceStringRef(LPCWSTR *param_1, uint param_2)
{
    WCHAR   WVar4;
    WCHAR  *pWVar1;
    WCHAR  *pWVar2;
    WCHAR  *pWVar3;
    WCHAR  *local_res8;
    WCHAR  *local_res18[2];

    pWVar2 = (WCHAR *)*param_1;
    WVar4 = L'"';
    if ((param_2 & 2) == 0) {
        if (*pWVar2 != L'"') {
            pWVar2 = StrRChrW(pWVar2, (LPCWSTR)0, L'\\');
            if (pWVar2 == (WCHAR *)0) {
                pWVar2 = (WCHAR *)*param_1;
            }
            pWVar3 = StrRChrW(pWVar2, (LPCWSTR)0, L'#');
            if (pWVar3 == (WCHAR *)0) {
                return (LPCWSTR)0;
            }
            if (*pWVar3 != L'#') {
                return (LPCWSTR)0;
            }
            if (9 < (uint16_t)(pWVar3[1] + (WCHAR)0xF01F)) {
                if ((param_2 & 1) == 0) {
                    return (LPCWSTR)0;
                }
                if (pWVar3[1] != L'.') {
                    return (LPCWSTR)0;
                }
                if (9 < (uint16_t)(pWVar3[2] + (WCHAR)0xF01F)) {
                    return (LPCWSTR)0;
                }
                return pWVar3;
            }
            return pWVar3;
        }
        pWVar2 = pWVar2 + 1;
    }
    local_res8 = pWVar2;
    thunk_FUN_1400f429c(&local_res8, 0x22);
    pWVar1 = local_res8;
    if (*local_res8 != L'\0') {
        local_res8 = local_res8 + 1;
    }
    if (WVar4 == *local_res8) {
        local_res8 = local_res8 + 1;
    }
    if (*local_res8 != L'#') {
        return (LPCWSTR)0;
    }
    if (*pWVar1 != L'\0') {
        *pWVar1 = L'\0';
    }
    *param_1 = pWVar2;
    pWVar2 = local_res8;
    local_res18[0] = local_res8;
    thunk_FUN_1400f429c(local_res18, WVar4);
    if (*local_res18[0] != L'\0') {
        *local_res18[0] = L'\0';
        return local_res8;
    }
    return pWVar2;
}
long long PECMD_ExpandCommandLine(long long *a, WCHAR *b, void *c, int d, uint8_t e) { (void)a;(void)b;(void)c;(void)d;(void)e; return 0; }
uint64_t PECMD_ExpandVarsRecursive(void) { return 0; }
uint64_t FUN_14007bf44(void) { return 0; }
uint64_t PECMD_SetVariableWithPrefix(void) { return 0; }
uint64_t FUN_14007de70(void) { return 0; }
uint64_t PECMD_SetCheckVariable(void) { return 0; }
uint64_t FUN_140082520(void) { return 0; }
uint64_t PECMD_NotifyMainWindowRefresh(void) { return 0; }
int64_t PECMD_QueryFontInfo(int64_t a, int *b, const void *c) { (void)a;(void)b;(void)c; return 0; }
uint64_t FUN_1400b1724(void) { return 0; }
uint64_t PECMD_CreateFont(void) { return 0; }
uint64_t PECMD_ParseHexOrDec(void) { return 0; }
uint64_t FUN_1400c11c0(void) { return 0; }
uint64_t FUN_1400d2e90(void) { return 0; }
uint64_t FUN_1400e3cd4(const uint16_t *param_1, uint64_t *param_2, int64_t *param_3) { (void)param_1;(void)param_2;(void)param_3; return 0; }
uint64_t FUN_1400e3f80(void) { return 0; }
uint64_t FUN_1400e4f14(void) { return 0; }
uint64_t PECMD_GetWindowTextAlloc(void) { return 0; }
uint64_t PECMD_GetOwnerWindow(uint64_t param_1) { (void)param_1; return 0; }
uint64_t FUN_1400e67e8(void) { return 0; }
uint64_t PECMD_GetFileSize(void) { return 0; }
const uint16_t *FUN_1400e6d38(const uint16_t *a, uint64_t b, const uint16_t *c) { (void)b; return a; }
void FUN_1400e6d68(const uint16_t *a, uint64_t b) { (void)a;(void)b; }
uint64_t PECMD_ResDecode(void) { return 0; }
HWND FUN_1400e8574(undefined8 *param_1, longlong param_2){
    HWND pHVar4; void *puVar3; void *pHVar2; longlong lVar1; uint8_t *pb;
    pHVar4=(HWND)param_1[8];
    if (pHVar4==(HWND)0) {
        FUN_1400e67e8(); pHVar2=0;
        if ((void*)param_1!=(void*)&DAT_14013d130) pHVar2=GetDesktopWindow();
        puVar3=operator_new(0xa98); pHVar4=(HWND)0;
        if (puVar3!=(void*)0) pHVar4=(HWND)FUN_1400731d8(puVar3,pHVar2,0x271b,(undefined8*)0);
        pb=(uint8_t*)pHVar4; lVar1=*(longlong*)pHVar4;
        pb[0x121]=1; *(undefined8**)(pb+0x290)=param_1; pb[0x122]=1; pb[0x120]=0x10;
        *(longlong*)(pb+0x2a0)=param_2;
        ((void(*)(void*,uint64_t,uint64_t))(uintptr_t)(lVar1+0x10))(pHVar4,0x271d,0);
    }
    return pHVar4;
}

uint64_t FUN_1400e91f0(void) { return 0; }
uint64_t FUN_1400e95f4(void) { return 0; }
uint64_t FUN_1400f21a8(void) { return 0; }
uint64_t FUN_140101e70(void) { return 0; }	/* CRT 标准库内联 */
uint64_t FillRect(void) { return 0; }
uint64_t FindClose(void) { return 0; }
uint64_t FindNextFileW(void) { return 0; }
uint64_t FindResourceW(void) { return 0; }
uint64_t FindWindowA(void) { return 0; }
uint64_t FindWindowExA(void) { return 0; }
uint64_t FindWindowExW(void) { return 0; }
uint64_t FindWindowW(void) { return 0; }
uint64_t FlushFileBuffers(void) { return 0; }
uint64_t FrameRgn(void) { return 0; }
uint64_t FreeEnvironmentStringsW(void) { return 0; }
uint64_t FreeLibrary(void) { return 0; }
uint64_t FreeSid(void) { return 0; }
uint64_t GetAsyncKeyState(void) { return 0; }
uint64_t GetBkColor(void) { return 0; }
uint64_t GetClientRect(void) { return 0; }
uint64_t GetClipboardData(void) { return 0; }
WCHAR *GetCommandLineW(void) { return (WCHAR *)0; }
uint64_t GetConsoleWindow(void) { return 0; }
DWORD GetCurrentDirectoryW(DWORD n, WCHAR *buf) { (void)n;(void)buf; return 0; }
uint64_t GetCurrentProcess(void) { return 0; }
uint64_t GetCurrentProcessId(void) { return 0; }
uint64_t GetCurrentThread(void) { return 0; }
uint64_t GetCurrentThreadId(void) { return 0; }
uint64_t GetCursorPos(void) { return 0; }
uint64_t GetDC(void) { return 0; }
uint64_t GetDIBits(void) { return 0; }
void *GetDesktopWindow(void) { return (void *)0; }
uint64_t GetDeviceCaps(void) { return 0; }
uint64_t GetDlgCtrlID(void) { return 0; }
uint64_t GetDlgItem(void) { return 0; }
uint64_t GetDlgItemTextW(void) { return 0; }
uint64_t GetDriveTypeW(void) { return 0; }
uint64_t GetEnvironmentStringsW(void) { return 0; }
uint64_t GetEnvironmentVariableA(void) { return 0; }
DWORD GetEnvironmentVariableW(const WCHAR *n, WCHAR *buf, DWORD sz) { (void)n;(void)buf;(void)sz; return 0; }
uint64_t GetExitCodeProcess(void) { return 0; }
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
uint64_t GetMessageW(void) { return 0; }
uint64_t GetModuleFileNameW(void) { return 0; }
void *GetModuleHandleA(const char *m) { (void)m; return (void *)0; }
uint64_t GetModuleHandleW(void) { return 0; }
uint64_t GetObjectW(void) { return 0; }
uint64_t GetParent(void) { return 0; }
uint64_t GetPixel(void) { return 0; }
FARPROC GetProcAddress(HMODULE h, const char *name) { (void)h;(void)name; return (FARPROC)0; }
uint64_t GetProcessAffinityMask(void) { return 0; }
uint64_t GetProcessHeap(void) { return 0; }
uint64_t GetProcessTimes(void) { return 0; }
uint64_t GetScrollInfo(void) { return 0; }
uint64_t GetScrollPos(void) { return 0; }
uint64_t GetScrollRange(void) { return 0; }
uint64_t GetStartupInfoW(void) { return 0; }
uint64_t GetStdHandle(void) { return 0; }
void *GetStockObject(int i) { (void)i; return (void *)0; }
uint64_t GetSubMenu(void) { return 0; }
uint64_t GetSysColor(void) { return 0; }
uint64_t GetSystemMenu(void) { return 0; }
uint64_t GetSystemMetrics(void) { return 0; }
uint64_t GetSystemTimeAsFileTime(void) { return 0; }
uint64_t GetThreadContext_exref(void) { return 0; }
DWORD GetTickCount(void) { return 0; }
uint64_t GlobalMemoryStatusEx(void) { return 0; }
uint64_t GetTokenInformation(void) { return 0; }
uint64_t GetVersion(void) { return 0; }
uint64_t GetVersionExW(void) { return 0; }
uint64_t GetWindowDC(void) { return 0; }
uint64_t GetWindowLongPtrW(void) { return 0; }
LONG GetWindowLongW(HWND hWnd, int nIndex) { (void)hWnd;(void)nIndex; return 0; }
uint64_t GetWindowRect(void) { return 0; }
uint64_t GetWindowTextLengthW(void) { return 0; }
uint64_t GetWindowTextW(void) { return 0; }
uint64_t GetWindowThreadProcessId(void) { return 0; }
uint64_t GlobalAlloc(void) { return 0; }
uint64_t GlobalDeleteAtom(void) { return 0; }
uint64_t GlobalFree(void) { return 0; }
uint64_t GlobalLock(void) { return 0; }
uint64_t GlobalMemoryStatus(void) { return 0; }
uint64_t GlobalUnlock(void) { return 0; }
void *HeapAlloc(void *h, DWORD f, size_t n) { (void)h;(void)f;(void)n; return (void *)0; }
int HeapFree(void *h, DWORD f, void *p) { (void)h;(void)f;(void)p; return 0; }
uint64_t HeapReAlloc(void) { return 0; }
uint64_t InitiateSystemShutdownW(void) { return 0; }
uint64_t InsertMenuW(void) { return 0; }
uint64_t InvalidateRect(void) { return 0; }
uint64_t IsDialogMessageW(void) { return 0; }
uint64_t IsIconic(void) { return 0; }
int IsWindow(void *h) { (void)h; return 0; }
uint64_t IsWindowEnabled(void) { return 0; }
uint64_t IsWindowVisible(void) { return 0; }
uint64_t KillTimer(void) { return 0; }
void LeaveCriticalSection(void *cs) { (void)cs; }
uint64_t LoadCursorW(void) { return 0; }
uint64_t LoadEnvi(void) { return 0; }
uint64_t LoadIconW(void) { return 0; }
void *LoadLibraryA(const char *m) { (void)m; return (void *)0; }
uint64_t LoadLibraryExW(void) { return 0; }
HMODULE LoadLibraryW(const WCHAR *name) { (void)name; return (HMODULE)0; }
uint64_t LoadMenuW(void) { return 0; }
uint64_t LoadResource(void) { return 0; }
uint64_t LoadStringW(void) { return 0; }
uint64_t LockResource(void) { return 0; }
uint64_t LockWorkStation(void) { return 0; }
uint64_t LookupPrivilegeValueW(void) { return 0; }
uint64_t MapViewOfFile(void) { return 0; }
uint64_t MessageBoxW(void) { return 0; }
uint64_t ModifyMenuW(void) { return 0; }
uint64_t MoveFileExW(void) { return 0; }
uint64_t MoveFileW(void) { return 0; }
uint64_t MoveWindow(void) { return 0; }
uint64_t MsgWaitForMultipleObjects(void) { return 0; }
uint64_t MulDiv(void) { return 0; }
uint64_t OffsetRect(void) { return 0; }
uint64_t OpenClipboard(void) { return 0; }
uint64_t OpenFileMappingA(void) { return 0; }
uint64_t OpenFileMappingW(void) { return 0; }
uint64_t OpenProcess(void) { return 0; }
uint64_t OpenProcessToken(void) { return 0; }
uint64_t OpenSCManagerW(void) { return 0; }
uint64_t OpenServiceW(void) { return 0; }
uint64_t OpenThread(void) { return 0; }
uint64_t PECMD_EvalExpr(void) { return 0; }
uint64_t PECMD_ExecCmd(void) { return 0; }
uint64_t PECMD_MemCmp(void) { return 0; }
uint64_t PECMD_ResPrefA(void) { return 0; }
uint64_t PECMD_ResPrefB(void) { return 0; }
uint64_t PECMD_ResPrefC(void) { return 0; }
uint64_t PECMD_ResPrefD(void) { return 0; }
uint64_t PECMD_ResReenc(void) { return 0; }
uint64_t PECMD_SwapBytes(void) { return 0; }
uint64_t PECMD_XorDecode(void) { return 0; }
uint64_t PathMatchSpecW(void) { return 0; }
uint64_t PathFileExistsW(void) { return 0; }
int PeekMessageW(void *msg, void *hwnd, UINT a, UINT b, UINT c) { (void)msg;(void)hwnd;(void)a;(void)b;(void)c; return 0; }
uint64_t Polyline(void) { return 0; }
uint64_t PostMessageW(void) { return 0; }
uint64_t PostQuitMessage(void) { return 0; }
uint64_t Process32FirstW(void) { return 0; }
uint64_t Process32NextW(void) { return 0; }
uint64_t PtInRect(void) { return 0; }
uint64_t QueryDosDeviceW(void) { return 0; }
uint64_t SetVolumeMountPointW(void) { return 0; }
uint64_t GetVolumeNameForVolumeMountPointW(void) { return 0; }
uint64_t QueryPerformanceCounter(void) { return 0; }
uint64_t QueryPerformanceFrequency(void) { return 0; }
uint64_t RaiseException(void) { return 0; }
uint64_t ReadFile(void) { return 0; }
uint64_t ReadProcessMemory(void) { return 0; }
uint64_t RedrawWindow(void) { return 0; }
int RegCloseKey(void *k) { (void)k; return 0; }
int RegCreateKeyExW(void *k, const WCHAR *sub, DWORD res, void *cls, DWORD opt, DWORD acc, void *sa, void *out, DWORD *disp) { (void)k;(void)sub;(void)res;(void)cls;(void)opt;(void)acc;(void)sa;(void)out;(void)disp; return 0; }
uint64_t RegDeleteKeyW(void) { return 0; }
int RegDeleteValueW(void *k, const WCHAR *nm) { (void)k;(void)nm; return 0; }
uint64_t RegEnumValueW(void) { return 0; }
uint64_t RegOpenKeyExW(void) { return 0; }
uint64_t RegQueryInfoKeyW(void) { return 0; }
uint64_t RegQueryValueExA(void) { return 0; }
int RegQueryValueExW(void *k, const WCHAR *nm, DWORD *rs, DWORD *ty, BYTE *dt, DWORD *sz) { (void)k;(void)nm;(void)rs;(void)ty;(void)dt;(void)sz; return 0; }
uint64_t RegisterClassExW(void) { return 0; }
uint64_t RegisterDeviceNotificationW(void) { return 0; }
UINT RegisterWindowMessageW(const WCHAR *s) { (void)s; return 0; }
uint64_t ReleaseDC(void) { return 0; }
uint64_t ReleaseMutex(void) { return 0; }
uint64_t RemoveDirectoryW(void) { return 0; }
uint64_t RemoveFontMemResourceEx(void) { return 0; }
uint64_t RemoveFontResourceW(void) { return 0; }
uint64_t RemoveMenu(void) { return 0; }
uint64_t ResetEvent(void) { return 0; }
uint64_t ResumeThread(void) { return 0; }
uint64_t SHChangeNotify(void) { return 0; }
uint64_t SHFileOperationW(void) { return 0; }
uint64_t SHGetSpecialFolderPathW(void) { return 0; }
uint64_t ScreenToClient(void) { return 0; }
uint64_t SearchPathW(void) { return 0; }
uint64_t SelectObject(void) { return 0; }
uint64_t SendMessageTimeoutW(void) { return 0; }
intptr_t SendMessageW(void *h, UINT m, uint64_t w, uint64_t l) { (void)h;(void)m;(void)w;(void)l; return 0; }
uint64_t SetActiveWindow(void) { return 0; }
uint64_t SetBkColor(void) { return 0; }
uint64_t SetBkMode(void) { return 0; }
uint64_t SetClipboardData(void) { return 0; }
uint64_t SetConsoleScreenBufferSize(void) { return 0; }
uint64_t SetCurrentDirectoryW(void) { return 0; }
uint64_t SetCursor(void) { return 0; }
uint64_t SetCursorPos(void) { return 0; }
uint64_t SetDIBits(void) { return 0; }
uint64_t SetDlgItemTextW(void) { return 0; }
uint64_t SetEndOfFile(void) { return 0; }
int SetEnvironmentVariableW(const WCHAR *n, const WCHAR *v) { (void)n;(void)v; return 0; }
uint64_t SetEvent(void) { return 0; }
uint64_t SetFilePointer(void) { return 0; }
uint64_t SetFilePointerEx(void) { return 0; }
uint64_t SetFocus(void) { return 0; }
uint64_t SetForegroundWindow(void) { return 0; }
void SetLastError(DWORD e) { (void)e; }
uint64_t SetLayeredWindowAttributes(void) { return 0; }
uint64_t SetLocalTime(void) { return 0; }
uint64_t SetMenuItemBitmaps(void) { return 0; }
uint64_t SetParent(void) { return 0; }
uint64_t SetPixel(void) { return 0; }
uint64_t SetProcessWorkingSetSize(void) { return 0; }
uint64_t SetScrollInfo(void) { return 0; }
uint64_t SetScrollPos(void) { return 0; }
uint64_t SetScrollRange(void) { return 0; }
uint64_t SetSystemPowerState(void) { return 0; }
uint64_t SetTextColor(void) { return 0; }
uint64_t SetThreadContext_exref(void) { return 0; }
uint64_t SetThreadPriority(void) { return 0; }
uint64_t SetTimer(HWND h, uint64_t id, UINT ms, TIMERPROC proc) { (void)h;(void)id;(void)ms;(void)proc; return 0; }
uint64_t SetWindowLongPtrW(void *h, int idx, uint64_t v) { (void)h;(void)idx;(void)v; return 0; }
uint64_t SetWindowLongW(void) { return 0; }
uint64_t SetWindowPos(void) { return 0; }
int SetWindowRgn(HWND h, void *r, BOOL b) { (void)h;(void)r;(void)b; return 1; }
uint64_t SetWindowTextW(void) { return 0; }
uint64_t SetWindowsHookExW(void) { return 0; }
uint64_t SetupDiCreateDeviceInfoList(void) { return 0; }
uint64_t SetupDiCreateDeviceInfoW(void) { return 0; }
uint64_t SetupDiOpenClassRegKey(void) { return 0; }
uint64_t SetupDiSetDeviceRegistryPropertyW(void) { return 0; }
uint64_t Shell_NotifyIconW(void) { return 0; }
uint64_t ShowScrollBar(void) { return 0; }
int ShowWindow(void *h, int c) { (void)h;(void)c; return 0; }
uint64_t SizeofResource(void) { return 0; }
uint64_t Sleep(void) { return 0; }
uint64_t SleepEx(void) { return 0; }
uint64_t StartServiceCtrlDispatcherW(void) { return 0; }
LPWSTR StrChrW(const WCHAR *s, WCHAR c) { (void)s;(void)c; return (LPWSTR)0; }
uint64_t StrCmpNIA(void) { return 0; }
int StrCmpNIW(const WCHAR *a, const WCHAR *b, int n) { (void)a;(void)b;(void)n; return 0; }
uint64_t StrCmpNW(void) { return 0; }
uint64_t StrCpyNW(void) { return 0; }
LPWSTR StrRChrW(const WCHAR *start, const WCHAR *end, WCHAR c) { (void)start;(void)end;(void)c; return (LPWSTR)0; }
uint64_t StrStrA(void) { return 0; }
uint64_t StrStrIW(void) { return 0; }
LPWSTR StrStrW(const WCHAR *a, const WCHAR *b) { (void)a;(void)b; return (LPWSTR)0; }
uint64_t StrToIntExW(void) { return 0; }
uint64_t StretchBlt(void) { return 0; }
uint64_t SystemParametersInfoW(void) { return 0; }
uint64_t TerminateJobObject(void) { return 0; }
uint64_t TerminateProcess(void) { return 0; }
uint64_t TerminateThread(void) { return 0; }
uint64_t TrackPopupMenu(void) { return 0; }
int TranslateMessage(const void *m) { (void)m; return 0; }
uint64_t UnhookWindowsHookEx(void) { return 0; }
uint64_t UnmapViewOfFile(void) { return 0; }
uint64_t UnregisterHotKey(void) { return 0; }
uint64_t UpdateWindow(void) { return 0; }
uint64_t VirtualAlloc(void) { return 0; }
uint64_t VirtualFree(void) { return 0; }
uint64_t VirtualProtect(void) { return 0; }
uint64_t VirtualProtectEx(void) { return 0; }
uint64_t VirtualQueryEx(void) { return 0; }
uint64_t WaitForMultipleObjects(void) { return 0; }
uint64_t WaitForSingleObject(void) { return 0; }
uint64_t WaitNamedPipeW(void) { return 0; }
uint64_t WideCharToMultiByte(void) { return 0; }
uint64_t WinExec(void) { return 0; }
uint64_t WinExec_exref(void) { return 0; }
uint64_t WriteFile(void) { return 0; }
uint64_t WriteProcessMemory(void) { return 0; }
uint64_t _UNK_140121ff6(void) { return 0; }
uint64_t _localtime64(void) { return 0; }
int _snwprintf(WCHAR *buf, size_t n, const WCHAR *fmt, ...) { (void)buf;(void)n;(void)fmt; return 0; }
uint64_t _time64(void) { return 0; }
uint64_t keybd_event(void) { return 0; }
uint64_t lstrcatW(void) { return 0; }
uint64_t lstrcmpW(void) { return 0; }
uint64_t lstrcmpiA(void) { return 0; }
int lstrcmpiW(const WCHAR *a, const WCHAR *b) { (void)a;(void)b; return 0; }
uint64_t lstrcpyW(void) { return 0; }
uint64_t lstrcpynW(void) { return 0; }
int32_t lstrlenA(const char *s) { (void)s; return 0; }
int lstrlenW(const WCHAR *s) { (void)s; return 0; }
int main(void) { return 0; }
uint64_t mouse_event(void) { return 0; }
void *operator_new(uint64_t n) { (void)n; return 0; }
uint64_t ram0x000140120a48(void) { return 0; }
uint64_t thunk_FUN_140072814(void) { return 0; }
uint64_t u__26_INDATA_140121fe0(void) { return 0; }
uint64_t u_____D__140120a40(void) { return 0; }
uint64_t wsprintfA(void) { return 0; }
uint64_t wsprintfW(void) { return 0; }
int64_t (*DAT_14013d408)(HKEY, LPCWSTR, uint32_t, uint32_t) = 0;
uint64_t PTR_s_No_error_14013b800;
uint64_t PTR_DAT_14013a8f0;
uint64_t PTR_FUN_14011eb78;
uint64_t PTR_FUN_14011eb88;
uint64_t PTR_FUN_14011c410;
uint64_t PTR_DAT_14013bfe0;
uint64_t DAT_14013d660;
uint64_t DAT_14013d738;
/* 本批(B1)新增定义 */
void PECMD_RestartSelf(uint16_t *param_1) { (void)param_1; }   /* @0x14007724c 自删除/延迟执行桩 */
uint64_t DAT_140147001;
uint64_t DAT_140147002;
uint64_t DAT_140147003;
uint64_t DAT_140147010;
uint64_t PTR_DAT_14013a250;
uint64_t PTR_FUN_1401234f0;
uint64_t PTR_FUN_140123540;
uint64_t PTR_FUN_140123560;
uint64_t PTR_FUN_140123580;
uint64_t PTR_FUN_1401235a8;
uint64_t PTR_FUN_140124e50;
uint64_t PTR_FUN_1401250d0;
uint64_t PTR_FUN_140126060;
uint64_t PTR_FUN_140126690;
uint64_t PTR_FUN_1401266c0;
uint64_t PTR_FUN_1401266e8;
uint64_t PTR_FUN_140126708;
uint64_t PTR_FUN_140126740;
uint64_t PTR_FUN_140126888;
uint64_t PTR_FUN_1401268a8;
uint64_t PTR_FUN_140128ed0;
uint64_t PTR_FUN_140129040;
uint64_t PTR_FUN_140129060;
uint64_t PTR_FUN_140129080;
uint64_t PTR_FUN_1401290a8;
uint64_t PTR_FUN_1401294f0;
uint64_t PTR_FUN_1401296e8;
uint64_t PTR_FUN_14012b1c0;
uint64_t PTR_FUN_14012b240;
uint64_t PTR_FUN_14012b430;
uint64_t PTR_FUN_14012bad0;
uint64_t PTR_FUN_14012bbf0;
uint64_t PTR_FUN_14012bd70;
uint64_t PTR_FUN_14012be90;
uint64_t PTR_FUN_14012bfb0;
uint64_t PTR_FUN_14012c0e0;
uint64_t PTR_FUN_14012c2e0;
uint64_t PTR_FUN_14012c410;
uint64_t PTR_FUN_14012c550;
uint64_t PTR_FUN_14012c670;
uint64_t PTR_FUN_14012c7b0;
uint64_t PTR_FUN_14012c900;
uint64_t PTR_FUN_14012ca50;
uint64_t PTR_FUN_14012cb90;
uint64_t PTR_FUN_14012ccc0;
uint64_t PTR_FUN_14012cde0;
uint64_t PTR_FUN_14012cf00;
uint64_t PTR__purecall_140126050;
uint64_t PTR_u_CallBackhWnd_14011e668_2_14013a288;
uint64_t PTR_u_Shell_14011ddb8_2_14013a290;
uint8_t  GetCommandLineW_exref[8];
uint64_t PTR_FUN_140126080;
uint64_t PTR_FUN_1401272a8;
uint64_t g_alphaThreshold;
uint64_t g_bInitWin;
double g_dpiBase;
uint64_t g_flagC96A;
uint64_t g_flagD032;
uint64_t g_flagD6f7;
double g_fontMinus0;
double g_fontRound;
uint64_t g_hCallBackWnd;
uint64_t g_hMainMutex;
uint64_t g_initFlags;
uint64_t g_langLen;
uint64_t g_logEnter;
uint64_t g_logFlag;
uint64_t g_msgboxActive;
uint64_t g_msgboxFlag;
uint64_t g_pLangBuf;
uint64_t g_pMainArgStr;
uint64_t g_paintScale2;
uint64_t g_pfnIsAppThemed;
uint64_t g_savedVtblSlot;
uint64_t g_subWndProc;
uint64_t g_threadId;

/* ---- 补充桩：当前 core_*.c 仍未实现，先补最小可链接桩 (TODO(verify)) ---- */
#include <stddef.h>
int64_t FUN_14005C72C(char *a, const void *b, int n) { (void)a; (void)b; (void)n; return 0; }
int FUN_14005B184(const char *a, const char *b, int n) { (void)a; (void)b; (void)n; return 0; }
void FUN_14005B0D4(void *ps) { (void)ps; }
void thunk_PECMD_GetNetworkConnectionName(const char *a, void *b, const char *c) { (void)a; (void)b; (void)c; }
uint16_t *PECMD_StrAlloc(uint16_t **ps, size_t count) { (void)ps; (void)count; return 0; }
void FUN_140060A74(uint8_t *buf, int len) { (void)buf; (void)len; }
int FUN_1400050C8(uint16_t *cmdline) { (void)cmdline; return 0; }
int32_t FUN_1400E7994(uint32_t spec, const uint8_t *src, int srclen,
                      uint16_t *dst, int dstcap, uint32_t key)
{ (void)spec; (void)src; (void)srclen; (void)dst; (void)dstcap; (void)key; return 0; }

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

uint64_t PECMD_ScriptCopy(void) { return 0; }
uint64_t PECMD_ParseCommandPath(void) { return 0; }
uint64_t FUN_140061E98(void) { return 0; }
uint64_t FUN_1400688E0(void) { return 0; }
uint64_t FUN_14006A81C(void) { return 0; }
uint64_t PECMD_CopyPathToken(void) { return 0; }
uint64_t FUN_140073ccc(void) { return 0; }
uint64_t FUN_1400e6d74(void) { return 0; }

/* ---- 追加桩: batch25 还原(140017b8c/00d2a0/0192d0/004ac4 及 B3 13 函数)引用的未定义符号 ---- */
/* Win32 / shlwapi API 桩 */
uint64_t CharUpperA(void) { return 0; }
uint64_t DeleteVolumeMountPointW(void) { return 0; }
uint64_t LookupAccountSidW(void) { return 0; }
uint64_t StrCmpNA(void) { return 0; }
uint64_t StrPBrkW(void) { return 0; }

/* FUN_ helper 无操作桩 */
uint64_t FUN_140001188(void) { return 0; }
uint64_t FUN_14000C764(void) { return 0; }
uint64_t PECMD_ResizeBuffer(void) { return 0; }
uint64_t PECMD_GetWindowObjectRef(void) { return 0; }
uint64_t PECMD_EncodeDet(void) { return 0; }
uint64_t PECMD_ParseHashNumbers(void) { return 0; }
uint64_t PECMD_GetComboItemText(void) { return 0; }
uint64_t FUN_14007e34c(void) { return 0; }
uint64_t PECMD_SaveSelectionToVar(void) { return 0; }
uint64_t PECMD_SkipWCharUntil(void) { return 0; }

/* 数据符号桩 */
uint64_t PTR_FUN_14011fb08[128];
uint64_t PTR_FUN_140120a50[128];
uint64_t PTR_FUN_140120a60[128];
uint64_t PTR_FUN_140120a70[128];
uint64_t PTR_FUN_14013a020[4];
uint64_t PTR_s_VK_LBUTTON_140124140[512];
uint64_t PTR_WndProc1_14013a008[4];

/* ---- 追加桩: BATCH25 (core_b1_remaining / core_b3_remaining 还原) 引用的未定义符号 ---- */
/* FUN_ helper 无操作桩 */
uint64_t FUN_1400540a8(void) { return 0; }
uint64_t PECMD_InitDragDrop(void) { return 0; }
uint64_t PECMD_IsSetupClass(void) { return 0; }
uint64_t PECMD_SetControlState(void) { return 0; }
uint64_t FUN_1400b89dc(int64_t a, void *b, const void *c) { (void)a;(void)b;(void)c; return 0; }
uint64_t PECMD_ShowContextMenu(void) { return 0; }
uint64_t *FUN_1400e57c0(uint64_t *a) { (void)a; return a; }
uint64_t PECMD_GetControlFont(void) { return 0; }
int PECMD_UpdateWindowStyleBits(int64_t a, unsigned int b, uint64_t c) { (void)a;(void)b;(void)c; return 0; }
uint64_t FUN_1400e8644(void) { return 0; }
uint64_t *FUN_1400ece2c(uint64_t *a, uint64_t b) { (void)a;(void)b; return a; }
uint64_t FUN_1400ec698(int64_t a, uint64_t b) { (void)a;(void)b; return 0; }
void FUN_1400ec6a8(int64_t a,int64_t b,uint64_t c,unsigned char *d,uint64_t e) { (void)a;(void)b;(void)c;(void)d;(void)e; }
undefined8 PECMD_AppendValueNode(longlong param_1, const WCHAR *param_2, undefined8 param_3){
    int iVar1; undefined8 *puVar2,*puVar3;
    iVar1=*(int*)(param_1+0x118);
    puVar2=operator_new(0x10);
    if (puVar2==(undefined8*)0) puVar2=(undefined8*)0;
    else { *puVar2=0; FUN_140063620((uint16_t**)(puVar2+1)); }
    *puVar2=param_3;
    FUN_14007034c((int64_t*)(puVar2+1),param_2);
    puVar3=(undefined8*)FUN_140063b00((longlong)iVar1,(longlong*)(param_1+0x108),(longlong*)(param_1+0x110),8);
    *puVar3=(undefined8)(uintptr_t)puVar2;
    *(longlong*)(param_1+0x118)=(longlong)(iVar1+1);
    return 1;
}

void PECMD_LayoutChildWindows(int64_t a, unsigned char b) { (void)a;(void)b; }
uint64_t PECMD_InitWindowObjectF(void) { return 0; }
uint64_t *FUN_1400f0648(uint64_t *a, uint64_t *b) { (void)a;(void)b; return a; }
bool FUN_1400f072c(longlong *param_1, const WCHAR *param_2, DWORD param_3, int *param_4, HWND param_5, uint param_6, DWORD param_7){
    int iVar1,iVar2,iVar3,iVar4; LONG LVar5; HWND pHVar6;
    iVar1=param_4[3]; iVar2=param_4[2]; iVar3=param_4[1]; iVar4=*param_4;
    LVar5=GetWindowLongW(param_5,-6);
    if (param_7==0) param_7=0;
    pHVar6=CreateWindowExW(param_7,(const WCHAR*)L"BUTTON",(LPCWSTR)param_2?0:((const WCHAR*)0),param_3,iVar4,iVar3,iVar2-iVar4,iVar1-iVar3,param_5,(void*)(ulonglong)param_6,(void*)(longlong)LVar5,(LPVOID)0);
    param_1[4]=(longlong)pHVar6;
    FUN_14006b8fc((long long*)param_1);
    return param_1[4]!=0;
}

uint64_t FUN_1400f11f8(void) { return 0; }
uint64_t PECMD_CreateDateTimePickCtl(void) { return 0; }
uint64_t FUN_1400f28e8(void) { return 0; }
uint64_t FUN_1400f2934(void) { return 0; }
uint64_t PECMD_InitControlObjC(void) { return 0; }
bool PECMD_CreateCustomChildControl(longlong *param_1, uint param_2, int *param_3, HWND param_4, uint param_5, LPCWSTR param_6){
    int iVar1,iVar2,iVar3,iVar4; LONG LVar5; HWND pHVar6;
    iVar1=param_3[3]; iVar2=param_3[2]; iVar3=param_3[1]; iVar4=*param_3;
    LVar5=GetWindowLongW(param_4,-6);
    pHVar6=CreateWindowExW(0,param_6,(LPCWSTR)0,param_2,iVar4,iVar3,iVar2-iVar4,iVar1-iVar3,param_4,(void*)(ulonglong)param_5,(void*)(longlong)LVar5,(LPVOID)0);
    param_1[4]=(longlong)pHVar6;
    FUN_14006b8fc((long long*)param_1);
    if ((param_2>>28&1)!=0) ShowWindow((void*)param_1[4],5);
    return param_1[4]!=0;
}

uint64_t FUN_1400fbfe0(void) { return 0; }
uint64_t *FUN_1400fcf44(uint64_t *a, uint64_t b) { (void)a;(void)b; return a; }
void *DAT_14013a858;   /* 默认画笔/色刷缓存 (GetStockObject, PECMD.exe 静态区, 初 0) */
/* PECMD_SetControlColors — 应用 param_2 的 4×RGBA 调色板与前景/背景/边框刷子到控件 struct 偏移槽. */
void PECMD_SetControlColors(long long param_1, int *param_2)
{
    int      iVar1;
    COLORREF color;
    HBRUSH   pHVar2;
    long long lVar3;
    uint64_t *puVar4;
    uint     uVar5 = 0, uVar6;

    if (param_2 != (int *)0) {
        if ((((*param_2 < 0) && (param_2[1] < 0)) && (param_2[2] < 0)) && (param_2[3] < 0)) {
            *(uint16_t *)(param_1 + 0xa2) = 0xffff;
        }
        else {
            do {
                uVar6 = uVar5 + 1;
                iVar1 = *param_2;
                param_2 = param_2 + 1;
                *(int *)(param_1 +
                    ((long long)(int)((uVar5 & 1 ^ (int)uVar5 >> 0x1f) - ((int)uVar5 >> 0x1f)) + 0x24 +
                     (long long)((int)uVar5 / 2) * 2) * 4) = iVar1;
                uVar5 = uVar6;
            } while ((int)uVar6 < 4);
            puVar4 = (uint64_t *)(param_1 + 0x80);
            lVar3 = 2;
            do {
                if ((void *)*puVar4 != (void *)0) {
                    DeleteObject((void *)*puVar4);
                }
                *puVar4 = 0;
                puVar4 = puVar4 + 1;
                lVar3 = lVar3 - 1;
            } while (lVar3 != 0);
            if (DAT_14013a858 == (void *)-1) {
                DAT_14013a858 = GetStockObject(5);
            }
            color = *(COLORREF *)(param_1 + 0x94);
            if (((int)color < 0) && (color != 0x80000000)) {
                *(void **)(param_1 + 0x80) = DAT_14013a858;
            }
            if ((*(int *)(param_1 + 0x9c) < 0) && (*(int *)(param_1 + 0x9c) != -0x80000000)) {
                *(void **)(param_1 + 0x88) = DAT_14013a858;
            }
            if (-1 < (int)color) {
                pHVar2 = CreateSolidBrush(color);
                *(HBRUSH *)(param_1 + 0x80) = pHVar2;
            }
            if (-1 < (int)*(COLORREF *)(param_1 + 0x9c)) {
                pHVar2 = CreateSolidBrush(*(COLORREF *)(param_1 + 0x9c));
                *(HBRUSH *)(param_1 + 0x88) = pHVar2;
            }
            if (*(short *)(param_1 + 0xa2) < 0) {
                *(uint16_t *)(param_1 + 0xa2) = 0;
            }
            if ((((-1 < *(int *)(param_1 + 0x9c)) &&
                 (*(int *)(param_1 + 0x9c) == *(int *)(param_1 + 0x94))) &&
                (*(int *)(param_1 + 0x98) == *(int *)(param_1 + 0x90))) &&
               ('\0' < (char)*(uint8_t *)(param_1 + 0xac))) {
                *(uint8_t *)(param_1 + 0xac) = *(uint8_t *)(param_1 + 0xac) | 0x80;
            }
        }
    }
}
bool PECMD_InitStaticControl(longlong *param_1, DWORD param_2, LPCWSTR param_3, uint param_4, int *param_5, HWND param_6, uint param_7){
    int iVar1,iVar2,iVar3,iVar4; LONG LVar5; HWND pHVar6;
    iVar1=param_5[3]; iVar2=param_5[2]; iVar3=param_5[1]; iVar4=*param_5;
    *(uint8_t*)((longlong)param_1+0xd1)=(uint8_t)(param_4&0xa3);
    LVar5=GetWindowLongW(param_6,-6);
    pHVar6=CreateWindowExW(param_2,(const WCHAR*)L"STATIC",param_3,param_4&0xffffff5f,iVar4,iVar3,iVar2-iVar4,iVar1-iVar3,param_6,(void*)(ulonglong)param_7,(void*)(longlong)LVar5,(LPVOID)0);
    param_1[4]=(longlong)pHVar6;
    FUN_14006b8fc((long long*)param_1);
    return param_1[4]!=0;
}

void PECMD_CreateStaticControlFromRect(int64_t *a, const void *b, unsigned int c, uint64_t *d, int64_t e, unsigned int f) { (void)a;(void)b;(void)c;(void)d;(void)e;(void)f; }
uint64_t *PECMD_InitStaticCtl(uint64_t *a, uint64_t b) { (void)a;(void)b; return a; }
uint64_t PECMD_CreateProgressCtl(void) { return 0; }
uint64_t thunk_FUN_140064b78(void) { return 0; }

/* B1 业务还原新增 helper 桩 (core_b1_remaining.c 中按真实签名 extern 调用) */
uint64_t FUN_1400e6860(void) { return 0; }
uint64_t PECMD_ProcessWindowObjMessage(void) { return 0; }
undefined8 FUN_140004e34(int param_1, longlong param_2){
    unsigned int DVar1,DVar3; int BVar2; uint16_t local_38[32];
    DVar1=GetTickCount();
    do {
        BVar2=PeekMessageW((void*)local_38,(void*)0,0,0,2);
        if (BVar2==0) return 0;
        if (*(uint32_t*)((char*)local_38+4)==0x12) { *(char*)(param_2+1)=0x20; return 1; }
        TranslateMessage((const void*)local_38);
        DispatchMessageW((const void*)local_38);
        if (*(char*)(param_2+1)!='\0') return 0;
        DVar3=GetTickCount();
    } while (param_1+(int)DVar1!=(int)DVar3);
    return 0;
}

uint64_t FUN_140004fd4(void) { return 0; }

/* 数据 / 虚表 / API 槽符号桩 */
uint64_t _UNK_1401265b8;
uint64_t PTR_FUN_140125be0;
uint64_t PTR_FUN_1401261e0;
uint64_t Ordinal_418_exref;

/* B1 业务还原新增数据符号 */
uint64_t PTR_DAT_14013a040;

/* ---- B3 业务还原 (core_b3_remaining.c) 引用的未定义 helper 桩 ---- */
uint16_t PECMD_DispatchMessageMap(int64_t *param_1, int param_2, uint64_t param_3, uint64_t *param_4,
                       int64_t param_5, uint8_t param_6, uint64_t *param_7)
{ (void)param_1;(void)param_2;(void)param_3;(void)param_4;(void)param_5;(void)param_6;(void)param_7; return 0; }
uint64_t PECMD_MessageHookProc(int param_1, uint64_t param_2, uint64_t param_3)
{ (void)param_1;(void)param_2;(void)param_3; return 0; }
void PECMD_CloseThreadSlot(int64_t *param_1) { (void)param_1; }
void FUN_14006ffdc(int64_t *param_1) { (void)param_1; }
void PECMD_DestroyStaticControl(uint64_t *param_1) { (void)param_1; }
void PECMD_ReleaseSlotObject(int64_t *param_1) { (void)param_1; }
int64_t PECMD_ControlWindowProc(uint64_t param_1, uint64_t param_2, uint64_t param_3, int64_t *param_4)
{ (void)param_1;(void)param_2;(void)param_3;(void)param_4; return 0; }
longlong FUN_1400b0380(longlong *param_1, undefined8 param_2, undefined8 *param_3){
    uint16_t *puVar1; undefined8 uVar2; longlong lVar3; const WCHAR *pWVar4;
    (void)param_2;
    puVar1=(uint16_t*)(uintptr_t)param_3[2];
    pWVar4=(const WCHAR*)(uintptr_t)param_3[1];
    if ((uintptr_t)puVar1 < 0x10000) {
        uVar2=FUN_14009d4b8(param_3[3],*param_3,pWVar4,(short)(char)(uintptr_t)puVar1,(const WCHAR*)0,0);
        lVar3=(longlong)(int)uVar2;
    } else {
        uVar2=FUN_14005c7c4((const char*)"cmd",puVar1);
        if ((char)(uVar2&0xff)==0) {
            lVar3=FUN_1400e5aac(param_1,param_3);
        } else {
            FUN_1400703e4((long long*)(uintptr_t)(param_3[6]+0x28),pWVar4);
            lVar3=0;
        }
    }
    return lVar3;
}


/* ---- B3 业务还原 (core_b3_remaining.c) 引用的未定义数据符号桩 ---- */
int32_t  _DAT_14013d8a4;
uint8_t  PTR_FUN_140125d00[8];
uint8_t  DAT_14013d8a0[256];

/* --- r34 follow-up: Win32 API stubs (no real impl) --- */
int SetServiceStatus(void *h, void *status) { return 1; }
int TlsSetValue(unsigned long idx, void *val) { return 1; }
int Rectangle(void *hdc, int a, int b, int c, int d) { return 1; }
void SwitchToFiber(void *fiber) { }

/* --- r36 follow-up stubs --- */
void PECMD_CleanupParser(int64_t p){ (void)p; }
void PECMD_DestroyControlObj(undefined8 *param_1){
    HWND hWnd; int BVar1;
    hWnd=(HWND)param_1[4]; *param_1=(undefined8)&PTR_FUN_14012b430;
    if (hWnd!=(HWND)0) {
        param_1[4]=0; BVar1=IsWindow(hWnd);
        if (BVar1!=0) {
            *(undefined4*)(param_1+0x1d)=0;
            if (param_1[0x1c]!=0) { HeapFree(DAT_14013d328,0,(void*)(uintptr_t)(param_1[0x1c]-8)); param_1[0x1c]=0; }
            SetWindowLongPtrW(hWnd,-4,0x140001188ULL); DestroyWindow(hWnd);
        }
    }
    if ((void*)(uintptr_t)param_1[0x1e]!=(void*)0) { DeleteObject((void*)(uintptr_t)param_1[0x1e]); param_1[0x1e]=0; }
    FUN_14005b104((void*)(param_1+0x25));
    FUN_1400e8940(param_1);
}

uint8_t PTR_FUN_140126b20[8];
void SwitchToThisWindow(void *hw, int b){ (void)hw;(void)b; }
int GetClassNameW(void *hw, uint16_t *p, int n){ (void)hw;(void)p; return 0; }
uint64_t *PECMD_AssignAnsiString(uint64_t *a, char *b){ (void)a;(void)b; return a; }
uint8_t s_ABCDEFGHIJKLMNOPQRSTUVWXYZabcdef_140124db0[128];
uint8_t PTR_FUN_1401268c8[8];
int CM_Get_DevNode_Status(uint32_t *a, uint32_t *b, uint32_t c, uint32_t d){ (void)a;(void)b;(void)c;(void)d; return 1; }
int CM_Query_And_Remove_SubTreeW(uint32_t a, int *b, uint32_t c, uint32_t d){ (void)a;(void)b;(void)c;(void)d; return 1; }
int CM_Request_Device_EjectW(){ return 1; }
int CM_Locate_DevNodeW(uint32_t *a, void *b, uint32_t c){ (void)a;(void)b;(void)c; return 1; }
int CM_Reenumerate_DevNode(uint32_t a, uint32_t b){ (void)a;(void)b; return 1; }
#include <stdarg.h>
uint64_t SystemTimeToFileTime(void){ return 0; }
uint64_t RegEnumKeyExW(void){ return 0; }
DWORD FUN_14005c394(HKEY param_1, LPCWSTR param_2, PHKEY param_3, REGSAM param_4, uint param_5)
{
    DWORD DVar1;
    DWORD local_res18[2];

    PECMD_EnableTokenPrivilege((LPCWSTR)L"SeBackupPrivilege", 2, 1);
    PECMD_EnableTokenPrivilege((LPCWSTR)L"SeRestorePrivilege", 2, 0x10);
    local_res18[0] = 0;
    DVar1 = RegCreateKeyExW(param_1, param_2, 0, (LPWSTR)0, param_5, 0xf003f,
                            (LPSECURITY_ATTRIBUTES)0, param_3, local_res18);
    if (*param_3 == (HKEY)0) {
        if ((param_5 & 4) != 0) {
            DVar1 = RegCreateKeyExW(param_1, param_2, 0, (LPWSTR)0, param_5 & 0xfffffffb, 0xf003f,
                                    (LPSECURITY_ATTRIBUTES)0, param_3, local_res18);
        }
        if (*param_3 == (HKEY)0) {
            DVar1 = RegCreateKeyExW(param_1, param_2, 0, (LPWSTR)0, 0, param_4,
                                    (LPSECURITY_ATTRIBUTES)0, param_3, local_res18);
            if ((*param_3 == (HKEY)0) && (DVar1 == 0)) {
                DVar1 = GetLastError();
                if (DVar1 == 0) DVar1 = 1;
            }
        }
    }
    return DVar1;
}
int wnsprintfW(uint16_t *b, int n, const uint16_t *f, ...){ (void)b;(void)n;(void)f; return 0; }
void FUN_1400e8940(uint64_t *a){ (void)a; }
void *PECMD_FreeResourceObject(void *a, unsigned int b){ (void)a;(void)b; return a; }

/* ---- 新增辅助桩 (core_b3 12 函数依赖; 签名与 decompiled 一致, 基类型等价) ---- */
void FUN_140102a90(uint64_t *dst, uint64_t v, uint64_t n){ (void)dst;(void)v;(void)n; }	/* CRT 标准库内联 */
uint16_t *PECMD_DriveTypeName(int i, uint16_t *out, int max){ (void)i;(void)max; return out; }
int FUN_1400690c0(uintptr_t root, const uint16_t *a, const uint16_t *b, int64_t *c, uint32_t *d, void *e){ (void)root;(void)a;(void)b;(void)c;(void)d;(void)e; return 0; }
uint64_t PECMD_ParseControlMessage(int64_t *a, uintptr_t b, uintptr_t c, const uint16_t *d, uintptr_t e, int64_t f){ (void)a;(void)b;(void)c;(void)d;(void)e;(void)f; return 0; }
uint64_t PECMD_ControlEnableCommand(int64_t *a, uintptr_t b, uintptr_t c, const uint16_t *d, uintptr_t e, int64_t f){ (void)a;(void)b;(void)c;(void)d;(void)e;(void)f; return 0; }
uint64_t FUN_1400db648(uintptr_t hwnd, uint16_t *s, int64_t p3, int64_t *p4, uintptr_t p5, uint32_t color, int64_t p7){ (void)hwnd;(void)s;(void)p3;(void)p4;(void)p5;(void)color;(void)p7; return 0; }
uint8_t DAT_140127738[8];
uint8_t DAT_140127740[8];

int FUN_14005f96c(uintptr_t h, int size){ (void)h;(void)size; return 0; }
uint64_t *PECMD_GetDiskLayoutInfo(uintptr_t h, uint64_t *buf, uint32_t *out){ (void)h;(void)buf;(void)out; return (uint64_t*)0; }
uint32_t PECMD_GetDiskGeometry(const uint16_t *p, uintptr_t h){ (void)p;(void)h; return 0; }

/* --- r37 follow-up stubs (main-agent closure after subagent interrupt) --- */
uint32_t FUN_140006a4c(const uint16_t *p){ (void)p; return 0; }
int64_t PECMD_OpenFileExisting(uint64_t a, uint64_t b, uint64_t c){ (void)a;(void)b;(void)c; return 0; }
uint64_t DuplicateTokenEx(void){ return 0; }
uint64_t SetTokenInformation(void){ return 0; }
uint64_t CreateProcessAsUserW(void){ return 0; }
uint64_t CreateProcessWithLogonW(void){ return 0; }
uint64_t FileTimeToSystemTime(void){ return 0; }
int (*DAT_14013c970)(uint32_t, uint32_t, char *, int);
/* --- r39 follow-up stubs (main-agent closure) --- */
uint64_t IsRectEmpty(void){ return 0; }
uint64_t CreateDCW(void){ return 0; }
void FUN_1400669c4(int64_t *a, uint64_t b, const uint16_t *c){ (void)a;(void)b;(void)c; }
uint64_t ReleaseSemaphore(void){ return 0; }
void *PECMD_InitSimpleWindow(void *a, uint64_t *b, uint64_t *c){ (void)a;(void)b;(void)c; return a; }
void *PECMD_InitListViewObject(void *a, uint64_t *b, uint64_t *c){ (void)a;(void)b;(void)c; return a; }
bool FUN_1400fbcb0(longlong *param_1, uint param_2, int *param_3, HWND param_4, uint param_5){
    int iVar1,iVar2,iVar3,iVar4; LONG LVar5; HWND pHVar6; const WCHAR *lpClassName;
    iVar1=param_3[3]; iVar2=param_3[2]; iVar3=param_3[1]; iVar4=*param_3;
    LVar5=GetWindowLongW(param_4,-6);
    lpClassName=(const WCHAR*)L"RECHEDIT";
    if (DAT_14013d300==2) lpClassName=(const WCHAR*)L"RichEdit20W";
    pHVar6=CreateWindowExW(0,lpClassName,(const WCHAR*)0,param_2&0xffffbfff,iVar4,iVar3,iVar2-iVar4,iVar1-iVar3,param_4,(void*)(ulonglong)param_5,(void*)(longlong)LVar5,(LPVOID)0);
    param_1[4]=(longlong)pHVar6;
    FUN_14006b8fc((long long*)param_1);
    if ((param_2>>14&1)!=0) SendMessageW((void*)param_1[4],0xd0,0,0x1400f21a8ULL);
    return param_1[4]!=0;
}

uint64_t FUN_140060b5c(uint64_t *a, int b, void *c, int d){ (void)a;(void)b;(void)c;(void)d; return 0; }
void PECMD_AppendFormattedI64(int64_t *a, int64_t b){ (void)a;(void)b; }
void *PECMD_InstallInfDevice(void *p){ return p; }
void FUN_140021144(const WCHAR *p) { (void)p; }        /* 设备驱动安装后端 (no-op) */
void FUN_140019da8(long long p1, const WCHAR *p2, int p3) { (void)p1;(void)p2;(void)p3; }  /* 字体/安装回调 (no-op) */
void FUN_140020fd4(long long p1, char c, const WCHAR *p2, const WCHAR *p3) { (void)p1;(void)c;(void)p2;(void)p3; }  /* 记录安装结果 (no-op) */
/* PECMD_InstallPlugPlayDriver — 经 UpdateDriverForPlugAndPlayDevicesW 安装驱动并打印结果日志,
   返回错误码 (0=失败置 1). */
uint PECMD_InstallPlugPlayDriver(long long param_1, LPCWSTR param_2, LPCWSTR param_3, uint param_4,
                  int param_5, uint8_t *param_6)
{
    int      iVar1;
    DWORD    DVar2;
    HMODULE  pHVar3;
    FARPROC  pFVar4;
    WCHAR   *pwVar5;
    WCHAR   *pwVar6;
    int      local_res8[2] = {0};
    WCHAR   *local_38[2] = {0};

    if (*(char *)(param_1 + 0x157) != '\0') {
        return 0;
    }
    if (param_6 == (uint8_t *)0) {
        param_6 = (uint8_t *)(param_1 + 0x152);
    }
    DVar2 = 0;
    local_res8[0] = 0;
    if (*(long long *)(param_1 + 0x138) == 0) {
        pHVar3 = LoadLibraryW((LPCWSTR)L"NewDev.dll");
        *(HMODULE *)(param_1 + 0x138) = pHVar3;
    }
    SetLastError(0);
    if (*(HMODULE *)(param_1 + 0x138) != (HMODULE)0) {
        if (*(long long *)(param_1 + 0x140) == 0) {
            pFVar4 = GetProcAddress(*(HMODULE *)(param_1 + 0x138), "UpdateDriverForPlugAndPlayDevicesW");
            *(FARPROC *)(param_1 + 0x140) = pFVar4;
            if (pFVar4 == (FARPROC)0) goto LAB_14002b91f;
        }
        if ((param_4 & 0x30000) != 0) {
            FUN_140021144(param_3);
        }
        iVar1 = ((int (*)(void *, LPCWSTR, LPCWSTR, uint32_t, int *))(*(void **)(param_1 + 0x140)))
                    (0, param_2, param_3, param_4 & 0xffff, local_res8);
        if (param_6 != (uint8_t *)0) {
            *param_6 = (uint8_t)local_res8[0];
        }
        if (*(long long *)(param_1 + 0x110) != 0) {
            if (iVar1 == 0) goto LAB_14002b91f;
            if (param_5 != -0x10) {
                FUN_140019da8(param_1, param_2, param_5);
            }
        }
        if (iVar1 != 0) goto LAB_14002b931;
    }
LAB_14002b91f:
    DVar2 = GetLastError();
    if (DVar2 == 0) {
        DVar2 = 1;
    }
LAB_14002b931:
    pwVar6 = (WCHAR *)L"(null)";
    if (param_2 != (LPCWSTR)0) {
        pwVar6 = (WCHAR *)param_2;
    }
    FUN_140063694(local_38, 0x2800);
    pwVar5 = (WCHAR *)DAT_14011c638;
    if (local_res8[0] != 0) {
        pwVar5 = (WCHAR *)L"Reboot";
    }
    _snwprintf(local_38[0], 0x27ff, (const WCHAR *)L"PECMD安装驱动【%s】[%s][%s]", pwVar6, param_3, pwVar5);
    FUN_140025f10(param_1 + 8, local_38[0], DVar2, (pthreadmbcinfo)0x1100, (pthreadmbcinfo)0,
                  (long long *)0);
    FUN_14005b104((long long *)local_38);
    FUN_140020fd4(param_1, 'I', pwVar6, param_3);
    return DVar2;
}
undefined8 PECMD_SetVectorSize(longlong *param_1, ulonglong param_2, int param_3){
    undefined8 rVar1; rVar1=1;
    if (param_3!=-1) *(int*)(param_1+3)=param_3;
    if (param_2==0) {
        if ((void*)*param_1!=(void*)0) { free((void*)*param_1); *param_1=0; }
        param_1[1]=0; param_1[2]=0; return 1;
    }
    if ((ulonglong)param_1[2]<param_2) {
        rVar1=FUN_14004fb44(param_1,param_2);
        if ((rVar1&0xff)==0) return rVar1;
        param_1[1]=(longlong)param_2; return 1;
    }
    param_1[1]=(longlong)param_2; return 1;
}

void PECMD_AllocExpandPath(void *a, int64_t *b){ (void)a;(void)b; }
void *PECMD_CheckInfDriver(int64_t *a, uint64_t b, char *c){ (void)a;(void)b;(void)c; return 0; }
uint64_t FUN_14002c634(int64_t a, void *b, void *c, int d){ (void)a;(void)b;(void)c;(void)d; return 0; }
void FUN_14002b9ec(int64_t a, void *b, unsigned c){ (void)a;(void)b;(void)c; }
uint8_t DAT_00000011;
uint64_t SetupDiSetDeviceInstallParamsW(void){ return 0; }
uint64_t SetupDiBuildDriverInfoList(void){ return 0; }
uint64_t SetupDiEnumDriverInfoW(void){ return 0; }
uint64_t SetupDiGetDriverInfoDetailW(void){ return 0; }
uint64_t SetupDiClassNameFromGuidW(void){ return 0; }
uint64_t GetShortPathNameW(void){ return 0; }
uint64_t CreateJobObjectW(void){ return 0; }
void PECMD_ScanDeviDirInfFiles(void *a, uint64_t b, uint64_t c, int d, void *e){ (void)a;(void)b;(void)c;(void)d;(void)e; }
char *PTR_s___disverify_14013a2c8;
char *PTR_s__AutoDisverify_14013a2c0;
uint64_t SetupDiGetDeviceInstallParamsW(void){ return 0; }
uint64_t PECMD_UpdateDriverSigning(int a, int b){ (void)a;(void)b; return 0; }
void FUN_140025ce0(int64_t *a, char *b, uint8_t *c, char *d, char *e, char *f){ (void)a;(void)b;(void)c;(void)d;(void)e;(void)f; }
uint16_t u__SystemRoot___140123190[64];
uint64_t SetupDiClassGuidsFromNameW(void){ return 0; }
uint64_t PTR_FUN_14011cbe8;
uint64_t VerQueryValueW(void){ return 0; }
uint64_t GetFileVersionInfoSizeW(void){ return 0; }
uint64_t GetFileVersionInfoW(void){ return 0; }
void PECMD_CmdKill(void *a, void *b){ (void)a;(void)b; }
uint64_t SetErrorMode(void){ return 0; }
uint64_t TlsAlloc(void){ return 0; }
void PECMD_DispatchSystemCommandLine(void *a, const uint16_t *b){ (void)a;(void)b; }
uint64_t WaitForInputIdle(void){ return 0; }

/* ---- B3 还原批次 (core_b3r_*.c) 依赖补充 ---- */
uint64_t PECMD_ManualMapPeImage(int *a, int16_t *b, uint64_t c){ (void)a;(void)b; return c; }
/* ---- P0 补缺失符号桩 ---- */
/* ---- 批1-01ed5c 依赖桩 ---- */
uint64_t FUN_14001ebdc(void){ return 0; }
void FUN_140063694(void *p, long long n) { (void)p;(void)n; }
uint64_t FUN_140063344(void){ return 0; }
void *DAT_14013ca68;   /* HINSTANCE 资源句柄 */

uint64_t GlobalAddAtomA(void) { return 0; }
uint64_t lstrcpyA(void) { return 0; }
uint64_t RegisterHotKey(void) { return 0; }
uint64_t TlsGetValue(void) { return 0; }
uint64_t WriteConsoleW(void) { return 0; }
uint8_t DAT_140124100[64];   /* 磁盘类型/分区对照表(未实现) */
uint64_t PECMD_OomPrompt(int a) { (void)a; return 0; }
/* ---- Tier 依赖预补 ---- */
uint64_t LCMapStringA(void){ return 0; }
uint64_t DAT_14013a360;   /* 48位随机种子 */
uint32_t FUN_14001b4f8(int16_t *s, int16_t ch){ int32_t i; for(i=0; s[i]!=ch; i++){} return (uint32_t)(uint32_t)(int32_t)i; }
void FUN_14005dff4(void){ DAT_14013a360 = (DAT_14013a360 * 0x5deece66d + 0xb) & 0xffffffffffff; }
/* DATA globals */
int64_t   DAT_14013d870;          /* bitmap data size */
/* function-pointer slots */

/* FUN_ helpers (business/unknown -> no-op typed stubs; symbol-only for link) */
bool PECMD_OnDeleteCommand(undefined8 *param_1, const WCHAR *param_2, HWND param_3){
    int iVar1; uint64_t local_58, local_50, local_38; undefined *local_40; undefined8 *local_48; uint64_t local_30[5];
    iVar1=lstrcmpiW(param_2,(const WCHAR*)L"*del");
    if (iVar1==0) {
        local_58=4; local_50=0; local_40=(undefined*)&DAT_14013d660; local_38=0; local_48=param_1;
        FUN_140102a90((uint64_t*)local_30,0,0x28);
        *(undefined1*)(uintptr_t)param_1[3]=0;
        *(undefined2*)(uintptr_t)param_1[2]=0;
        SendMessageW(param_3,0x465,(uint64_t)(param_1[10]+0x40),(uint64_t)&local_58);
        ((void(*)(void*,int))(uintptr_t)*param_1)(param_1,1);
    }
    return iVar1!=0;
}

uint64_t PECMD_LoadLogPixelsDpi(void){ return 0; }
uint64_t PECMD_GetDiskGeometrySize(void){ return 0; }
uint64_t PECMD_TaskShowMessage(uint64_t *p){ (void)p; return 0; }
void FUN_14006345c(void **ps, const char *s){ (void)ps;(void)s; }
uint64_t PECMD_FindPartitionInfo(void){ return 0; }
uint64_t PECMD_EncodeImageToStream(void){ return 0; }
uint64_t FUN_1400e3d60(void){ return 0; }
uint64_t PECMD_UpdateWindowExStyle(void){ return 0; }

/* PECMD_* 人类可读别名 (对应 FUN_<addr> 槽位; 委托到真实业务体) */
/* 真实业务体原型 (定义于 core_string.c / core_var.c / core_sys.c) */
WCHAR *FUN_14006375C(WCHAR **ps, LPCWSTR src);          /* @0x14006375c 字符串追加 */
void FUN_1400702B0(WCHAR **ps, LPCWSTR src);            /* @0x1400702b0 字符串赋值 */
void FUN_14005B104(WCHAR **ps);                         /* @0x14005b104 字符串释放 */
WCHAR **FUN_14005B154(WCHAR **pp);                      /* @0x14005b154 跳过空白 */
void FUN_1400629B8(void *script, LPCWSTR key, LPCWSTR value); /* @0x1400629b8 设置变量 */
DWORD FUN_14005C394(HKEY root, LPCWSTR subkey, HKEY *out, REGSAM access, uint32_t opt); /* @0x14005c394 注册表打开 */
DWORD FUN_14005C4E0(HKEY root, LPCWSTR subkey, LPCWSTR name, DWORD *type, BYTE *data, DWORD *size); /* @0x14005c4e0 注册表查询 */

uint16_t *PECMD_StrCat(uint16_t **ps, const uint16_t *src){ return FUN_14006375C(ps, src); }
void PECMD_StrAssign(uint16_t **ps, const uint16_t *src){ FUN_1400702B0(ps, src); }
void PECMD_StrFree(uint16_t **ps){ FUN_14005B104(ps); }
void PECMD_StrAlloc2(uint16_t **ps, int64_t count){ (void)ps;(void)count; }
void PECMD_SetVar(void *script, const uint16_t *key, const uint16_t *value){ FUN_1400629B8(script, key, value); }

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

/* --- r41 follow-up stubs (main-agent closure) --- */
/* PECMD_CreateButtonSubCtl — 创建 BUTTON 控件窗口并按其样式/标志初始化 (func: 按钮/超链接样式).
   依赖: FUN_14006b8fc (控件尺寸计算), FUN_1400ef91c (按钮状态初始化, 内绕坐标/标志). */
long long FUN_14006b8fc(long long *param_1) { (void)param_1; return 1; } /* 控件尺寸计算 (no-op) */
void FUN_1400ef91c(long long param_1, uint param_2, uint64_t param_3) { (void)param_1;(void)param_2;(void)param_3; } /* 按钮状态初始化 (no-op) */
bool PECMD_CreateButtonSubCtl(long long *param_1, LPCWSTR param_2, uint64_t param_3, int *param_4,
                  HWND param_5, uint param_6, DWORD param_7)
{
    int     iVar1, iVar2, iVar3, iVar4;
    bool    bVar5, bVar6, bVar7;
    LONG    LVar8;
    LPWSTR  pWVar9;
    HWND    pHVar10;
    HRGN    hRgn;
    uint64_t uVar11;
    uint    uVar12, uVar13;

    uVar12 = (uint)param_3;
    uVar11 = 0;
    bVar6 = false;
    bVar5 = false;
    if (((param_3 & 0x40) != 0) || ((*(uint8_t *)(param_1 + 0x1d) & 8) != 0)) {
        bVar5 = true;
    }
    iVar1 = param_4[2];
    iVar2 = *param_4;
    iVar3 = param_4[3];
    iVar4 = param_4[1];
    LVar8 = GetWindowLongW(param_5, -6);
    if ((-1 < (int)param_1[0x12]) || (-1 < (int)param_1[0x13])) {
        bVar6 = true;
    }
    bVar7 = false;
    uVar13 = (uint)(-((uint)(((uint8_t)(uVar12 & 0xfffffbbf) & 0xb) != 0xb)) & 2);
    if ((!bVar5) &&
       (((uVar13 != 0 && (bVar7 = bVar6, bVar6)) ||
        (pWVar9 = StrStrW(param_2, (LPCWSTR)L"\r\n"), pWVar9 != (LPWSTR)0)))) {
        bVar5 = true;
    }
    if (((-1 < *(int *)((long long)param_1 + 0x94)) || (-1 < *(int *)((long long)param_1 + 0x9c)) ) &&
       (pWVar9 = StrChrW(param_2, (WCHAR)L'&'), pWVar9 != (LPWSTR)0)) {
        uVar11 = (uint64_t)(uVar13 | 1);
    }
    if (param_7 == 0) { param_7 = 0; }
    pHVar10 = CreateWindowExW(param_7, (LPCWSTR)L"BUTTON", param_2, uVar12 & 0xfffffbbf,
                              *param_4, param_4[1], iVar1 - iVar2, iVar3 - iVar4, param_5,
                              (HMENU)(uintptr_t)param_6, (HINSTANCE)(intptr_t)LVar8, (LPVOID)0);
    param_1[4] = (long long)pHVar10;
    FUN_14006b8fc((long long*)param_1);
    if ((*(uint8_t *)(param_1 + 0x1d) & 1) != 0) {
        hRgn = CreateEllipticRgn(0, 0, iVar1 - iVar2, iVar3 - iVar4);
        SetWindowRgn((HWND)param_1[4], hRgn, 1);
        DeleteObject(hRgn);
    }
    if (bVar5) {
        *(uint8_t *)(param_1 + 0x1d) = *(uint8_t *)(param_1 + 0x1d) & 0xef;
        *(uint8_t *)(param_1 + 0x1d) = *(uint8_t *)(param_1 + 0x1d) | ((uint8_t)(param_3 >> 10) & 1) << 4;
        FUN_1400ef91c((long long)param_1, (uVar12 & 0x400) * 2, uVar11);
        if ((bVar7) && (param_1[0x22] != 0)) {
            *(long long **)(param_1[0x22] + 0xe0) = param_1;
        }
        if (((*(uint8_t *)(param_1 + 0x1d) & 8) != 0)) {
            *(int *)(param_1[0x22] + 0x50) = 0;
            if ((uVar12 >> 0x1c & 1) != 0) {
                SetTimer((HWND)param_1[4], 0x2715, 10, (TIMERPROC)0);
            }
        }
    }
    return param_1[4] != 0;
}
void FUN_1400ef078(int64_t a, uint64_t b){ (void)a;(void)b; }
uint64_t *FUN_1400fec9c(uint64_t *a, uint64_t b, uint64_t c){ (void)a;(void)b;(void)c; return (uint64_t*)0; }
int PECMD_CreateTreeViewControl(int64_t *a, unsigned long b, int *c, void *d, uint32_t e){ (void)a;(void)b;(void)c;(void)d;(void)e; return 1; }
bool PECMD_FindAndReleaseComObject(const WCHAR *param_1){
    int iVar1; longlong *plVar2;
    EnterCriticalSection((void*)&DAT_14013e168);
    FUN_140061c44();
    iVar1=-3;
    if (DAT_14013d800!=(void*)0) {
        iVar1=(*DAT_14013d800)(0);
        if ((iVar1!=0)&&(iVar1==1)) { if (DAT_14013d808) (*DAT_14013d808)(); }
    }
    plVar2=(longlong*)FUN_14006e3a4(param_1);
    if (plVar2!=(longlong*)0) { (**(void(**)(void*))((uintptr_t)*plVar2+0x10))(plVar2); }
    if ((DAT_14013d808!=(void*)0)&&(iVar1==0)) { (*DAT_14013d808)(); }
    LeaveCriticalSection((void*)&DAT_14013e168);
    return plVar2!=(longlong*)0;
}

uint64_t *PECMD_InitDcObject(uint64_t *a, uint64_t b){ (void)a;(void)b; return (uint64_t*)0; }
uint8_t PTR_FUN_140124f70[8];
uint8_t PTR_FUN_140125e20[8];
undefined4 PECMD_IsIconGroupName(ushort *param_1){
    int bVar1; undefined8 uVar3; int iVar2;
    bVar1=FUN_14005ceec(param_1,"icon Group");
    iVar2=bVar1;
    uVar3=0;
    if (iVar2==0) {
        bVar1=FUN_14005ceec(param_1,"iconGroup");
        iVar2=bVar1;
        if (iVar2==(int)uVar3) {
            bVar1=FUN_14005ceec(param_1,"#14");
            iVar2=bVar1;
            if (iVar2==(int)uVar3) goto LAB_14005d833;
        }
    }
    uVar3=1;
LAB_14005d833:
    return (undefined4)(char)uVar3;
}

uint64_t DestroyCursor(void){ return 0; }
uint64_t EnumResourceNamesW(void){ return 0; }
uint64_t CreateIconFromResource(void){ return 0; }
uint64_t PECMD_ParseAngleNumbers(uint64_t *a, int64_t *b, int64_t *c, int64_t *d, int64_t *e){ (void)a;(void)b;(void)c;(void)d;(void)e; return 0; }
int64_t FUN_1400705ac(int64_t *a, const uint16_t *b, uint32_t c, uint64_t d, uint64_t e, uint32_t f, uint32_t g, uint64_t h){ (void)a;(void)b;(void)c;(void)d;(void)e;(void)f;(void)g;(void)h; return 0; }
char s_0_0_0_0_140126958[16];
uint64_t FUN_14002d708(uint16_t *s, int a, int64_t *p, int b, unsigned long d){ (void)s;(void)a;(void)p;(void)b;(void)d; return 0; }
uint64_t GetLastInputInfo(void){ return 0; }

/* --- batch r42: B1/B3/B9 remaining restore link stubs --- */
uint64_t GetDiskFreeSpaceW(void){ return 0; }
uint64_t GetVolumeInformationW(void){ return 0; }
/* PECMD_HandleServiceCommandLine — 命令行后处理: 识别 /InstallService 等开关并把 argv 元素改写
   为服务名(前缀 ~+类型字母), 随后启动服务安装流程并 Exit. */
void FUN_140005344(void) { /* no-op */ }
uint16_t *FUN_14000531c(uint16_t *s) { (void)s; return s; }          /* 跳过空白 */
uint8_t *FUN_14001d78c(uint8_t *a, uint8_t *b, int n) { (void)a;(void)b;(void)n; return a; } /* 元组填充 */
void FUN_140008b2c(uint16_t *s) { (void)s; }                          /* 移除服务(子树) */
void FUN_140017048(const WCHAR *s) { (void)s; }                       /* 服务安装处理 */
uint32_t DAT_14013c9fc;   /* GetTickCount 结果槽 (PECMD.exe 静态清零区, 初 0) */
long long DAT_14013d130;   /* 传 PECMD_ExpandCommandLine 的输出槽 */
uint64_t PECMD_HandleServiceCommandLine(short *param_1)
{
    uint16_t sVar1;
    WCHAR    WVar2;
    char     cVar3;
    int      iVar4;
    WCHAR   *pWVar5;
    WCHAR   *local_res8;
    WCHAR   *local_res10;
    WCHAR   *local_res18;
    WCHAR   *local_res20;

    sVar1 = (uint16_t)*param_1;
    local_res8 = (WCHAR *)(param_1 + 4);
    if ((((uint16_t)*local_res8 < 9) || (0xd < (uint16_t)*local_res8)) && (*local_res8 != L' ')) {
        local_res8 = (WCHAR *)(param_1 + 5);
    }
    FUN_140005344();
    DAT_14013c9fc = GetTickCount();
    local_res18 = (WCHAR *)0;
    PECMD_StrDupAssign(&local_res20, local_res8);
    FUN_140063620(&local_res10);
    PECMD_ExpandCommandLine(&DAT_14013d130, local_res20, &local_res10, 0, 1);
    FUN_1400703e4((long long *)&local_res18, local_res10);
    local_res8 = local_res18;
    FUN_14005b104((long long *)&local_res10);
    FUN_14005b104((long long *)&local_res20);
    pWVar5 = (WCHAR *)FUN_14000531c((uint16_t *)local_res8);
    local_res8 = pWVar5;
    iVar4 = StrCmpNIW((LPCWSTR)L"/InstallService", pWVar5, 0xf);
    if ((iVar4 == 0) &&
       (((WVar2 = pWVar5[0xf], 8 < (uint16_t)WVar2 && ((uint16_t)WVar2 < 0xe)) ||
         ((WVar2 == L' ') || (WVar2 == L'\0'))))) {
        cVar3 = 'I';
        FUN_14001d78c((uint8_t *)local_res8, (uint8_t *)L"/Service       ", 0x1e);
        local_res8 = (WCHAR *)FUN_14000531c((uint16_t *)(local_res8 + 0xf));
    }
    else {
        cVar3 = FUN_1400660ac("/Service", (long long *)&local_res8, 8);
        if (cVar3 == '\0') {
            cVar3 = FUN_1400660ac("/RemoveService", (long long *)&local_res8, 0xe);
            if (cVar3 != '\0') {
                FUN_140008b2c((uint16_t *)local_res8);
                ExitProcess(0);
            }
            FUN_14005b104((long long *)&local_res18);
            return 0;
        }
        cVar3 = 'S';
    }
    if (sVar1 == 0x22) {
        iVar4 = lstrlenW(local_res8);
        for (pWVar5 = local_res8 + (long long)iVar4 + -1; local_res8 < pWVar5; pWVar5 = pWVar5 + -1) {
            if ((((uint16_t)*pWVar5 < 9) || (0xd < (uint16_t)*pWVar5)) && (*pWVar5 != L' ')) {
                if ((local_res8 < pWVar5) && (*pWVar5 == L'\"')) {
                    *pWVar5 = L'\0';
                }
                break;
            }
        }
    }
    local_res8[-1] = (WCHAR)cVar3;
    local_res8 = local_res8 + -2;
    *local_res8 = L'~';
    FUN_140017048(local_res8);
    ExitProcess(0);
    return 0;
}
uint64_t FUN_1400458a8(void){ return 0; }
uint64_t FUN_140045c90(void){ return 0; }
void PECMD_RelocatePointerArray(undefined8 param_1, uint *param_2, longlong param_3, longlong param_4)
{
    uint8_t *base;
    longlong lVar1;
    longlong lVar2;
    uint     uVar3;

    (void)param_1;
    if (param_2 == (uint *)0) {
        AtlThrowImpl(-0x7fffbffb);
    }
    base = (uint8_t *)param_2;
    *(longlong *)(base + 8) = param_3 + (*(longlong *)(base + 8) - param_4 >> 1) * 2;
    *(longlong *)(base + 16) = param_3 + (*(longlong *)(base + 16) - param_4 >> 1) * 2;
    uVar3 = 0;
    if (*param_2 != 0) {
        lVar2 = 0;
        do {
            lVar1 = *(longlong *)(base + 32);
            if ((*(longlong *)(lVar2 + lVar1) != 0) && (*(longlong *)(lVar2 + 8 + lVar1) != 0)) {
                *(longlong *)(lVar2 + lVar1) = param_3 + (*(longlong *)(lVar2 + lVar1) - param_4 >> 1) * 2;
                *(longlong *)(lVar2 + 8 + *(longlong *)(base + 32)) =
                    param_3 + (*(longlong *)(lVar2 + 8 + *(longlong *)(base + 32)) - param_4 >> 1) * 2;
            }
            uVar3 = uVar3 + 1;
            lVar2 = lVar2 + 0x10;
        } while (uVar3 < *param_2);
    }
}
uint64_t PECMD_VectorPushBack(void){ return 0; }
uint64_t FUN_14005b7dc(void){ return 0; }
uint64_t FUN_14005be68(void){ return 0; }
/* --- batch28 restored-function (core_b3r_g*.c) link stubs --- */
/* data globals */
uint8_t  DAT_14013d5c0[8] = {0};
uint8_t  DAT_1401210f8[8] = {0};
uint64_t _UNK_14012d1d0 = 0;
/* Win32 / SetupAPI */
uint64_t SetVolumeLabelW(void){ return 0; }
uint64_t SetupDiGetDeviceInfoListDetailW(void){ return 0; }
uint64_t GetTempPathW(void){ return 0; }
uint64_t CM_Get_DevNode_Status_Ex(void){ return 0; }

/* PECMD refactored aliases -> delegate to real business bodies (FUN_14005B154/394/C4E0) */
WCHAR **PECMD_SkipSpace(WCHAR **pp){ return FUN_14005B154(pp); }
DWORD PECMD_RegOpen(HKEY root, LPCWSTR subkey, HKEY *out, REGSAM access, uint32_t opt){ return FUN_14005C394(root, subkey, out, access, opt); }
DWORD PECMD_RegQuery(HKEY root, LPCWSTR subkey, LPCWSTR name, DWORD *type, BYTE *data, DWORD *size){ return FUN_14005C4E0(root, subkey, name, type, data, size); }

/* helper FUN_ stubs referenced by restored bodies */
uint64_t PECMD_CheckDriveType(void){ return 0; }
uint64_t FUN_140072d8c(void){ return 0; }
uint64_t PECMD_OpenCloseDrive(void){ return 0; }
ulonglong PECMD_IsVisibleKeyword(ushort *param_1){    ushort uVar1; ulonglong uVar2; undefined1 uVar4;
    uVar2=(ulonglong)(*param_1|0x20); uVar4=0;
    if ((((*param_1|0x20)==0x76)&&(param_1[1]|0x20)==0x69)&&(param_1[2]|0x20)==0x73) {
        uVar1=param_1[3]|0x20; uVar2=(ulonglong)uVar1;
        if ((((uVar1==0x69)||(uVar1==0x61))&&((param_1[4]|0x20)==0x62))&&((param_1[5]|0x20)==0x6c)&&((param_1[6]|0x20)==0x65)) {
            if (param_1[7]==0) uVar4=1;
        }
    }
    return (uVar2&0xffffffffffffff00ULL)|uVar4;

}

uint64_t FUN_140062fc4(void){ return 0; }
uint64_t FUN_140060a74(void){ return 0; }
uint64_t PECMD_ShowWindowMode(void){ return 0; }
uint64_t PECMD_SendPingPacket(void){ return 0; }
uint64_t PECMD_InitWinsockOnce(void){ return 0; }

/* --- batch28 restored-function deps --- */
uint64_t CreateMenu(void) { return 0; }
uint64_t FileTimeToLocalFileTime(void) { return 0; }
uint64_t GetLongPathNameW(void) { return 0; }
uint64_t GetMenu(void) { return 0; }
uint64_t GetTimeZoneInformation(void) { return 0; }
uint64_t LocalFileTimeToFileTime(void) { return 0; }
uint64_t SetMenu(void) { return 0; }
uint64_t SHAppBarMessage(void) { return 0; }
uint64_t PTR_FUN_140126540;
uint64_t PTR_FUN_140126560;
uint64_t FUN_14003c06c(void) { return 0; }
uint64_t PECMD_LzDecompress(void) { return 0; }
uint64_t FUN_14005eefc(void) { return 0; }
uint64_t PECMD_BufferedWriteByte(void) { return 0; }
uint64_t PECMD_StreamReadByte(void) { return 0; }
uint64_t PECMD_AllocConsoleBuffers(void) { return 0; }
uint64_t PECMD_CalcCalendarMonthRows(void) { return 0; }
void FUN_140063ed4(WPARAM param_1, longlong param_2, undefined8 *param_3, int param_4, int param_5, int param_6, int param_7, undefined8 *param_8, undefined8 *param_9, uint param_10){
    longlong lVar1; undefined8 *puVar2,*puVar3,*puVar4; int local_res20[2];
    local_res20[0]=param_4;
    puVar2=(undefined8*)FUN_140060834(param_1,*param_3);
    puVar4=(undefined8*)0;
    if (puVar2!=(undefined8*)0) {
        lVar1=**(longlong**)(param_1+0x1a0);
        FUN_14005daf8(param_1,local_res20,&param_5,&param_6,&param_7);
        puVar3=(undefined8*)operator_new(0x78);
        if (puVar3!=(undefined8*)0) {
            puVar4=FUN_140057334(puVar3,param_2,(int)((longlong)puVar2-lVar1>>3)+0x1000,param_3,local_res20[0],param_5,param_6,param_7,param_8,param_9,param_10);
        }
        *puVar2=(undefined8)(uintptr_t)puVar4;
        FUN_14005d9a8(param_1,0);
    }
}

void PECMD_InsertControlObject(WPARAM param_1, longlong param_2, undefined8 *param_3, int param_4, int param_5, int param_6, int param_7, undefined8 *param_8, undefined8 *param_9, uint param_10){
    longlong lVar1; undefined8 *puVar2,*puVar3,*puVar4; int local_res20[2];
    local_res20[0]=param_4;
    puVar2=(undefined8*)FUN_140060834(param_1,*param_3);
    puVar4=(undefined8*)0;
    if (puVar2!=(undefined8*)0) {
        lVar1=**(longlong**)(param_1+0x1a0);
        FUN_14005daf8(param_1,local_res20,&param_5,&param_6,&param_7);
        puVar3=(undefined8*)operator_new(0x70);
        if (puVar3!=(undefined8*)0) {
            puVar4=FUN_14005762c(puVar3,param_2,(int)((longlong)puVar2-lVar1>>3)+0x1000,param_3,local_res20[0],param_5,param_6,param_7,param_8,param_9,param_10);
        }
        *puVar2=(undefined8)(uintptr_t)puVar4;
        FUN_14005d9a8(param_1,0);
    }
}

long long FUN_140064a88(uint16_t *s) { (void)s; return 0; } /* 解析十进制数字串 */
uint64_t PECMD_StreamClose(void) { return 0; }
uint64_t PECMD_ConvertStringEncoding(void) { return 0; }
uint64_t PECMD_DeviceCheckReady(void) { return 0; }
uint64_t FUN_1400799f0(void) { return 0; }
uint64_t PECMD_JoinTokensAndResolve(void) { return 0; }
uint64_t PECMD_ParseCommaNumbers(void) { return 0; }
undefined8 *PECMD_CreateNamedLock(const WCHAR *param_1, char param_2, uint32_t *param_3){
    WCHAR WVar1; void *puVar2; const char *pcVar3; const WCHAR *pWVar4; int64_t local_res8;
    local_res8=0;
    if (*param_1==L'#') {
        pWVar4=param_1+1; WVar1=*pWVar4;
        if (WVar1==L'#') pWVar4=param_1+2;
        pcVar3="Local\\pecmd2012.lock.";
        if (WVar1==L'#') pcVar3="Global\\pecmd2012.lock.";
        FUN_1400702f0((int64_t*)&local_res8,(char*)pcVar3,0xffffffffffffffffULL);
        FUN_14006375c((uint16_t**)&local_res8,pWVar4);
        puVar2=FUN_14006e74c((const WCHAR*)(uintptr_t)local_res8,param_2,param_3);
        FUN_14005b104((longlong*)&local_res8);
    } else {
        puVar2=FUN_1400a43c4(param_1,param_2);
    }
    return (undefined8*)puVar2;
}

uint64_t FUN_1400e4d84(void) { return 0; }
uint64_t PECMD_LoadRasApi(void) { return 0; }
uint64_t FUN_1400e4e94(void) { return 0; }
uint64_t FUN_1400e5120(void) { return 0; }
uint64_t FUN_1400e5248(void) { return 0; }
uint64_t PECMD_FindRasConnection(void) { return 0; }
uint64_t PECMD_EnumRasConnections(void) { return 0; }
uint64_t PECMD_EnumPhonebookEntries(void) { return 0; }
uint64_t FUN_1400ebd30(void) { return 0; }
uint64_t PECMD_ExecSpecialCommand(void) { return 0; }
uint64_t PECMD_FreeCacheBlock(void) { return 0; }
uint64_t PECMD_ShakeTrayCursor(void) { return 0; }
uint64_t PECMD_ParseSizeNumber(void) { return 0; }
uint64_t FUN_14007033c(void) { return 0; }
undefined8 PECMD_PushStringToken(undefined8 param_1, undefined8 param_2, const WCHAR *param_3, undefined8 *param_4){
    longlong *plVar1; undefined8 uVar2;
    (void)param_1;(void)param_2;
    plVar1=(longlong*)*param_4;
    if (param_3<(const WCHAR*)0x10000) param_4[3]=(undefined8)(uintptr_t)param_3;
    if ((longlong)param_4[1]<1) {
        if (plVar1!=(longlong*)0) {
            if (param_3<(const WCHAR*)0x10000) param_4[2]=(undefined8)(uintptr_t)param_3;
            else { FUN_1400703e4((long long*)plVar1,param_3); param_4[2]=*plVar1; }
        }
        uVar2=0;
    } else { param_4[1]=param_4[1]-1; uVar2=1; }
    return uVar2;
}

uint64_t PECMD_EnumNetworkDevices(void) { return 0; }
/* @0x140078d8c 类 base64/十六进制位串解码 (size=259) */
uint64_t PECMD_DecodeBase64ToWideStr(uint8_t *param_1, uint64_t *param_2)
{
  int iVar2;
  uint8_t bVar1;
  uint8_t bVar3;
  uint64_t uVar4;
  uint64_t uVar6;
  uint8_t *pCVar5;
  uint8_t *local_res8;
  LPCSTR pstr;

  iVar2 = lstrlenA((LPCSTR)param_1);
  FUN_1400633a8((void **)&local_res8,(int64_t)((iVar2 + 1) * 3));
  uVar4 = 0;
  pCVar5 = local_res8 + 1;
  *local_res8 = ' ';
  *pCVar5 = '\0';
  bVar3 = 0;
  uVar6 = uVar4;
  do {
    if (*param_1 == 0) {
      *pCVar5 = '\0';
      PECMD_StrDupA((WCHAR **)param_2, (LPCWSTR)local_res8,0xffffffffffffffffLL,0xfde9);
      uVar4 = *param_2;
LAB_140078e06:
      FUN_14005b104((void *)&local_res8);
      return uVar4;
    }
    bVar1 = *param_1;
    if (bVar1 == 0x3d) {
      if ((char)bVar3 > 0) {
        *pCVar5 = (uint8_t)uVar6;
        bVar3 = 0;
        pCVar5 = pCVar5 + 1;
        uVar6 = uVar4;
      }
    }
    else {
      if ((bVar1 < 0x41) || (0x5a < bVar1)) {
        if ((bVar1 < 0x61) || (0x7a < bVar1)) {
          if ((bVar1 < 0x30) || (0x39 < bVar1)) {
            if (bVar1 == 0x2b) {
              bVar1 = 0x3e;
            }
            else {
              if (bVar1 != 0x2f) {
                *(uint16_t *)*param_2 = 0;
                goto LAB_140078e06;
              }
              bVar1 = 0x3f;
            }
          }
          else {
            bVar1 = bVar1 + 4;
          }
        }
        else {
          bVar1 = bVar1 + 0xb9;
        }
      }
      else {
        bVar1 = bVar1 + 0xbf;
      }
      uVar6 = (uint64_t)((uint32_t)uVar6 | (uint32_t)bVar1 << (bVar3 & 0x1f));
      bVar3 = bVar3 + 6;
      while (((char)bVar3 > 7) || (((char)bVar3 > 0) && (*param_1 == 0))) {
        *pCVar5 = (uint8_t)uVar6;
        pCVar5 = pCVar5 + 1;
        uVar6 = uVar6 >> 8;
        bVar3 = bVar3 - 8;
      }
    }
    param_1 = param_1 + 1;
  } while( 1 );
}
uint64_t PECMD_ImportEnvironment(void) { return 0; }
uint64_t FUN_14008293c(void) { return 0; }
uint64_t PECMD_SendTitleMessage(void) { return 0; }
void PECMD_AddControlA(WPARAM param_1, longlong param_2, undefined8 *param_3, int param_4, int param_5, int param_6, int param_7, undefined8 *param_8, undefined8 *param_9, uint param_10, WCHAR *param_11){
    longlong lVar1; undefined8 *puVar2,*puVar3,*puVar4; int local_res20[2];
    local_res20[0]=param_4;
    puVar2=(undefined8*)FUN_140060834(param_1,*param_3);
    puVar4=(undefined8*)0;
    if (puVar2!=(undefined8*)0) {
        lVar1=**(longlong**)(param_1+0x1a0);
        FUN_14005daf8(param_1,local_res20,&param_5,&param_6,&param_7);
        puVar3=(undefined8*)operator_new(0x78);
        if (puVar3!=(undefined8*)0) {
            puVar4=FUN_14009cacc(puVar3,param_2,(int)((longlong)puVar2-lVar1>>3)+0x1000,param_3,local_res20[0],param_5,param_6,param_7,param_8,param_9,param_10,param_11);
        }
        *puVar2=(undefined8)(uintptr_t)puVar4;
        FUN_14005d9a8(param_1,0);
    }
}
void FUN_1400aa7e4(WPARAM param_1, longlong param_2, undefined8 *param_3, int param_4, int param_5, int param_6, int param_7, undefined4 param_8, LPWSTR param_9, undefined4 param_10){
    longlong lVar1; undefined8 *puVar2,*puVar3,*puVar4; int local_res20[2];
    local_res20[0]=param_4;
    puVar2=(undefined8*)FUN_140060834(param_1,*param_3);
    puVar4=(undefined8*)0;
    if (puVar2!=(undefined8*)0) {
        lVar1=**(longlong**)(param_1+0x1a0);
        FUN_14005daf8(param_1,local_res20,&param_5,&param_6,&param_7);
        puVar3=(undefined8*)operator_new(0x78);
        if (puVar3!=(undefined8*)0) {
            puVar4=FUN_1400aa144(puVar3,param_2,(int)((longlong)puVar2-lVar1>>3)+0x1000,param_3,local_res20[0],param_5,param_6,param_7,param_8,param_9,(uint8_t)param_10);
        }
        *puVar2=(undefined8)(uintptr_t)puVar4;
    }
}

void FUN_1400aaa4c(WPARAM param_1, longlong param_2, undefined8 *param_3, int param_4, int param_5, int param_6, int param_7, ushort *param_8, undefined8 *param_9, uint param_10){
    longlong lVar1; undefined8 *puVar2,*puVar3,*puVar4; int local_res20[2];
    local_res20[0]=param_4;
    puVar2=(undefined8*)FUN_140060834(param_1,*param_3);
    puVar4=(undefined8*)0;
    if (puVar2!=(undefined8*)0) {
        lVar1=**(longlong**)(param_1+0x1a0);
        FUN_14005daf8(param_1,local_res20,&param_5,&param_6,&param_7);
        puVar3=(undefined8*)operator_new(0x70);
        if (puVar3!=(undefined8*)0) {
            puVar4=FUN_14009c720(puVar3,param_2,(int)((longlong)puVar2-lVar1>>3)+0x1000,param_3,local_res20[0],param_5,param_6,param_7,param_8,param_9,param_10);
        }
        *puVar2=(undefined8)(uintptr_t)puVar4;
        FUN_14005d9a8(param_1,0);
    }
}

void FUN_1400b9340(WPARAM param_1, longlong param_2, undefined8 *param_3, int param_4, int param_5, int param_6, int param_7, undefined8 *param_8, uint param_9, int *param_10, LPCWSTR param_11){
    longlong lVar1; undefined8 *puVar2,*puVar3,*puVar4; int local_res20[2];
    local_res20[0]=param_4;
    puVar2=(undefined8*)FUN_140060834(param_1,*param_3);
    puVar4=(undefined8*)0;
    if (puVar2!=(undefined8*)0) {
        lVar1=**(longlong**)(param_1+0x1a0);
        FUN_14005daf8(param_1,local_res20,&param_5,&param_6,&param_7);
        puVar3=(undefined8*)operator_new(0x78);
        if (puVar3!=(undefined8*)0) {
            puVar4=FUN_1400b8f10(puVar3,param_2,(int)((longlong)puVar2-lVar1>>3)+0x1000,param_3,local_res20[0],param_5,param_6,param_7,param_8,param_9,param_10,param_11);
        }
        *puVar2=(undefined8)(uintptr_t)puVar4;
    }
}

void FUN_1400bd620(WPARAM param_1, undefined8 param_2, undefined8 *param_3, int param_4, int param_5, int param_6, int param_7, undefined8 *param_8, undefined8 *param_9, undefined4 *param_10, undefined4 param_11, uint param_12){
    longlong lVar1; undefined8 *puVar2,*puVar3,*puVar4; int local_res20[2];
    local_res20[0]=param_4;
    puVar2=(undefined8*)FUN_140060834(param_1,*param_3);
    puVar4=(undefined8*)0;
    if (puVar2!=(undefined8*)0) {
        lVar1=**(longlong**)(param_1+0x1a0);
        FUN_14005daf8(param_1,local_res20,&param_5,&param_6,&param_7);
        puVar3=(undefined8*)operator_new(0xb0);
        if (puVar3!=(undefined8*)0) {
            puVar4=FUN_1400bca60(puVar3,param_2,(int)((longlong)puVar2-lVar1>>3)+0x1000,param_3,local_res20[0],param_5,param_6,param_7,param_8,param_9,param_10,param_11,param_12,*(undefined4*)(param_1+0x17c));
        }
        *puVar2=(undefined8)(uintptr_t)puVar4;
        FUN_14005d9a8(param_1,0);
    }
}

uint64_t PECMD_ClipboardCommand(void) { return 0; }
uint64_t DAT_14013e2a8;
uint64_t g_pFontBase;

/* ============================================================
   ---- 新增叶依赖数据/桩 (FUN_140076b88 / FUN_140097714 恢复所需) ----
   ※ 本批仅添加真正缺失的 DAT_ 全局 与 缺失的 WinAPI 导入桩;
     内容按原二进制用法推定, 以待数据迁移后核对.
   ============================================================ */
/* DAT_140126e88 (4 WCHAR, "\\??\\" 符号链接前缀前段) — 与 g_u6426e70/e78/e80 同区 */
uint64_t u_____C__140126e88 = 0x005C003F003F005CULL;
/* DAT_140126e90 (4 WCHAR: [盘符占位, ':', 0, 0] 符号链接模板后段, 低 16 位被盘符覆盖) */
uint64_t ram0x000140126e90 = 0x00000000003A0043ULL;   /* "C:\\0%c:\\0" 前8字节(LE) */
/* DAT_14013d388 — RegSaveKeyExW 延迟加载函数指针槽 */
void    *DAT_14013d388;

/* RegLoadKeyW / RegUnLoadKeyW — Advapi32 直接导入, 原 refactor 未引用故缺桩 (house no-op 模式) */
uint64_t RegLoadKeyW(void) { return 0; }
uint64_t RegUnLoadKeyW(void) { return 0; }

/* ============================================================
   ---- 新增叶依赖桩 (FUN_14004e488 行解析器恢复所需) ----
   仅供链接满足符号, 无真实逻辑.
   ============================================================ */
int      FUN_14006156c(const uint16_t *a, const uint16_t *b) { (void)a;(void)b; return 0; }
uint64_t FUN_1400a53e4(int64_t a, void *b, void *c, int d, const uint16_t *e) { (void)a;(void)b;(void)c;(void)d;(void)e; return 0; }
const uint16_t *FUN_14001b23c(int64_t a, void *b, const uint16_t *c, void *d, char e) { (void)a;(void)b;(void)d;(void)e; return c; }

/* ============================================================
   ---- 新增叶依赖桩 (FUN_140051610 表达式解析恢复所需) ----
   ============================================================ */
int      FUN_1400510e8(int *a, uint16_t **b, char *c) { (void)a;(void)b;(void)c; return 0; }
int      FUN_1400512f4(int *a, uint16_t **b, char *c) { (void)a;(void)b;(void)c; return 0; }
int      FUN_1400513b8(int *a, uint16_t **b, char *c) { (void)a;(void)b;(void)c; return 0; }
undefined8 PECMD_MatchKeywordTable(undefined4 *param_1, longlong *param_2){
    uint64_t *ppuVar2; uint16_t *psVar3; uint16_t *local_res20; uint64_t uVar1;
    ppuVar2=&PTR_u_a__a_zA_Z0_9___14013a2d0[0];
    if (PTR_u_a__a_zA_Z0_9___14013a2d0[0]!=0) {
        psVar3=(uint16_t*)(uintptr_t)PTR_u_a__a_zA_Z0_9___14013a2d0[0];
        do {
            if (**(uint16_t**)param_2==*psVar3) {
                local_res20=(uint16_t*)(uintptr_t)(*ppuVar2+2);
                uVar1=FUN_14005370c(param_1,&local_res20,0);
                if ((int)uVar1<0) { *param_1=5; return uVar1; }
                *(uint16_t**)param_2=(uint16_t*)((uintptr_t)*(uint16_t**)param_2+4);
                return uVar1;
            }
            ppuVar2=ppuVar2+1;
            psVar3=(uint16_t*)(uintptr_t)*ppuVar2;
        } while (psVar3!=(uint16_t*)0);
    }
    return 0xffffffff;
}


/* ============================================================
   ---- 新增叶依赖桩/数据 (FUN_140057940 字体控制消息恢复所需) ----
   ============================================================ */
uint64_t FUN_140067b54(uint16_t *a, double *b) { (void)a;(void)b; return 0; }
void     FUN_140007bcc(uint16_t *a, int *b) { (void)a;(void)b; }
void     FUN_140074838(uint16_t *a, int *b) { (void)a;(void)b; }
double   DAT_1401237e0;
double   DAT_1401237e8;
double   DAT_1401237f0;
double   DAT_1401237f8;
double   DAT_140123800;

/* ============================================================
   ---- 新增叶依赖桩 (FUN_14005ccb0 SetupDi 设备变更恢复所需) ----
   ============================================================ */
int SetupDiSetClassInstallParamsW(void *a, void *b, void *c, uint32_t d) { (void)a;(void)b;(void)c;(void)d; return 1; }
int SetupDiChangeState(void *a, void *b) { (void)a;(void)b; return 1; }

/* ============================================================
   ---- 新增叶依赖桩 (FUN_14005e254/FUN_14005e3ac 恢复所需) ----
   ============================================================ */
int64_t FUN_140018978(int64_t *a, const uint16_t *b, int64_t *c, int64_t d, void *e) { (void)a;(void)b;(void)c;(void)d;(void)e; return (int64_t)0; }
void    FUN_14005e36c(int64_t a, uint32_t *b) { (void)a;(void)b; }

/* ============================================================
   ---- 新增叶依赖桩/数据 (FUN_140064694/14006587c/140065c04 数字解析族恢复所需) ----
   ============================================================ */
int      FUN_14005bc28(uint16_t a) { (void)a; return 0; }      /* 字符分类: 返回0表示字符合法 */
int64_t  FUN_14005bbb4(int64_t *a) { (void)a; return 0; }      /* 跳过前导 数字后读后缀 */
double   DAT_140124110;                                        /* 常数(解析) */
/* DAT_140124118 = 3.141592653589793 (π, 0x400921fb54442d18) — 来自 PECMD.exe .rdata */
double   DAT_140124118 = 3.1415926535897932384626433832795;
/* DAT_1401263a0 = 10.0 (0x4024000000000000) */
double   DAT_1401263a0 = 10.0;
/* DAT_1401261a0 = 1.0 (0x3ff0000000000000) */
double   DAT_1401261a0 = 1.0;
double   DAT_140126398;                                        /* 舍入阈值0.5 */
double   DAT_140126390;                                        /* 小数缩放 */
double   DAT_140121668;                                        /* 常数(解析) */

/* ============================================================
   ---- core_b3_remaining.c 本轮还原 (FUN_14005ce04/061dac/062bdc/
        072924/073934/077190/081238) 新增依赖桩/数据符号 ----
   ============================================================ */
/* GDI+ 渲染 (FUN_140062bdc) 所需函数指针槽 (no-proto, 初值 0) */
int64_t (*DAT_14013ce08)(void) = 0;
int64_t (*DAT_14013ce10)(void) = 0;
int64_t (*DAT_14013ce28)(void) = 0;
int64_t (*DAT_14013ce30)(void) = 0;
/* GDI+ 渲染默认色 / 插值矩阵数据 */
uint32_t DAT_14013a34c = 0x80000000;   /* 默认色 CLR_NONE */
uint8_t DAT_14013a838[16] = {0x6d,0xbd,0xed,0x6a,0xb5,0x3f,0x8a,0x41,0x83,0xa6,0x7f,0x45,0x22,0x9d,0xc8,0x72};   /* GDI+ 格式 GUID(自.raw) */

/* SetupDi 设备枚举 (FUN_140072924) 缺失 helper 桩 */
uint64_t PECMD_FindTokenInList(const uint16_t *src, const uint16_t *devid, int len, uint flags)
{ (void)src;(void)devid;(void)len;(void)flags; return 0; }
uint64_t FUN_1400662a4(int64_t *p1, void *p2, int64_t *p3, const uint16_t *p4,
                       const uint16_t *p5, uint p6)
{ (void)p1;(void)p2;(void)p3;(void)p4;(void)p5;(void)p6; return 0; }
int SetupDiClassNameFromGuidA(const void *guid, char *buf, uint32_t buflen, uint32_t *req)
{ (void)guid;(void)buf;(void)buflen;(void)req; return 1; }

/* 图标资源 (FUN_140073934) 缺失 WinAPI 桩 */
uint64_t CreateIconFromResourceEx(void){ return 0; }

/* b1-tier deps — core_b1_remaining.c FUN_140006660 还原所需的最小数据槽 */
uintptr_t g_hFontE2B8;   /* DAT_14013e2b8 缓存字体 (HFONT, 初值 0, 惰性创建) */
/* ---- 075c7c/00cedc 依赖 ---- */
uint64_t LCMapStringW(void){ return 0; }
uint64_t FUN_140063224(void){ return 0; }
uint64_t thunk_FUN_1400f429c(void *a, short b) { (void)a;(void)b; return 0; }

/* ============================================================
   ---- core_b3_remaining.c 本轮还原 (FUN_140070710) 新增依赖 ----
   ※ 本批仅添加真正缺失的 helper: FUN_140065864 (真实逻辑, 内绕
     FUN_140063b00 栈求值器). 其余候选函数 (140070da8/084a5c/0987ec/
     0b2ca8/0bb718/0a8a30/0ac094) 因混淆/C++对象/巨型依赖按 SKIP 保留.
   ============================================================ */
/* 取值自 PECMD原始.EXE — FUN_140065864 计算器栈定位: param_1<0 时清 0 并置
 * param_4 低字节 bit3(8); 随后按 FUN_140063b00 定位栈元素并返回其指针(RAX). */
int64_t FUN_140065864(int64_t param_1, int64_t *param_2, int64_t *param_3,
                      uint8_t *param_4, uint32_t param_5)
{
    if (param_1 < 0) {
        param_1 = 0;
        *param_4 = (uint8_t)(*param_4 | 8);
    }
    return FUN_140063b00(param_1, param_2, param_3, param_5);
}

/* ============================================================
   ---- core_b3_remaining.c 本轮还原 (FUN_140081238 命令串解析) 缺失 token 解析 helper 桩 ----
   字符串 token 切分辅助, 供 FUN_140081238 解析 "名称,值" 序对使用.
   真实逻辑为: 按分隔符扫描/拷贝子串到容器(00[0]); 此处保留签名与副作用(不推进指针),
   使调用方编译/链接通过并按返回码走控制流.
   ============================================================ */
void FUN_1400675b8(int64_t *param_1, int64_t *param_2, int16_t param_3) { (void)param_1,(void)param_2,(void)param_3; }
void FUN_1400676e4(int64_t *param_1, int64_t *param_2, int16_t param_3) { (void)param_1,(void)param_2,(void)param_3; }
void FUN_140067748(int64_t *param_1, int64_t *param_2, int16_t param_3) { (void)param_1,(void)param_2,(void)param_3; }
uint32_t FUN_1400734e4(int64_t param_1, void *param_2, int64_t *param_3) { (void)param_1,(void)param_2,(void)param_3; return 0; }

/* ============================================================
   ---- core_b3_remaining.c 本轮还原 (FUN_140097150 安全描述符/ACL 设置) 缺失
        Advapi32 安全 API 与内部 helper 桩 ----
   真实逻辑: GetNamedSecurityInfoW + BuildExplicitAccessWithNameW + SetEntriesInAclW +
   SetNamedSecurityInfoW 为 NTFS/注册表 ACL 授权; GetUserNameW/LookupAccountNameW 取当前用户 SID.
   此处以 no-op 桩保持签名, 使调用方编译/链接通过且控制流按返回码推进.
   ============================================================ */
typedef long LSTATUS;
uint64_t GetNamedSecurityInfoW(void) { return 0; }      /* 0=ERROR_SUCCESS 走主授权分支 */
uint64_t BuildExplicitAccessWithNameW(void) { return 0; }
uint64_t SetEntriesInAclW(void) { return 0; }
uint64_t GetUserNameW(void) { return 0; }
uint64_t LookupAccountNameW(void) { return 0; }
LSTATUS  FUN_140096f84(void *param_1, void *param_2, void *param_3) { (void)param_1,(void)param_2,(void)param_3; return 0; }
uint64_t FUN_140063424(void *param_1, int64_t param_2) { (void)param_1,(void)param_2; return 0; }
void     FUN_140101db8(void *param_1, const void *param_2, void *param_3) { (void)param_1,(void)param_2,(void)param_3; }
/* LocalFree — 安全描述符/ACL 释放 (FUN_140097150 使用), 声明于 win32_stub.h, 原无定义 */
uint64_t LocalFree(uint64_t hMem) { (void)hMem; return 0; }

/* ============================================================
   ---- core_b3_remaining.c 本轮还原 (FUN_140070da8 计算器浮点运算分派) 新增
        常数与栈写入 helper 桩 ----
   ※ 真实值提取自 PECMD.exe .rdata/.text, 见各注释.
   ============================================================ */
/* DAT_140124120 = 1.5707963267948966 (π/2, 0x3ff921fb54442d18) */
double DAT_140124120 = 1.5707963267948966192313216916398;
/* DAT_1401268f0 = 180.0 (0x4066800000000000, 弧度↔角度转换) */
double DAT_1401268f0 = 180.0;
/* DAT_140125230 -> g_dbl25230 = 0.5 (core_globals.c 定义), DAT_140125238 -> g_fontMinus0 = -0.0 */
/* 计算器栈写入辅助: 将值 param_3 写入栈元素 param_2 并递减栈指针 (param_1+4) */
void FUN_14005bc48(int64_t param_1, double *param_2, double param_3)
{ (void)param_1;(void)param_2;(void)param_3; }
void FUN_14005bc5c(int64_t param_1, double *param_2, double param_3)
{ (void)param_1;(void)param_2;(void)param_3; }
/* 取计算器栈顶当前值 (取模运算种子) */
uint64_t FUN_14005e0a0(void) { return 0; }

/* ============================================================
   ---- core_b3_remaining.c 本轮还原 (FUN_1400987ec Win10 开始菜单/任务栏 PINT 固定)
        新增全局槽与缺失 helper/WinAPI 桩 ----
   ※ 全局槽初始值取自已提取 PECMD.exe 映像(静态清零区, 初值 0).
   ============================================================ */
uint32_t DAT_14013a848;                    /* PINT 固定方向/标志(用户注册表缓存, 初 0) */
uint32_t DAT_14013e1f0;                    /* PINT 一次性清理注册标志位 */
int64_t  DAT_14013e1e8;                    /* (后台清理槽, 无初始化) */
int64_t  DAT_14013e1e0;
int64_t  DAT_14013e1d8;
int64_t  DAT_14013e1d0;
uint8_t  DAT_14011c638[64];                /* 默认命令串缓存区(静态, 初 0) */

/* 缺失 helper 桩 (无调用方校验, 仅满足符号) */
uint64_t FUN_14005ea5c(void) { return 0; }                      /* 取命令行模式标志 */
uint64_t FUN_14001b7f4(const uint16_t *a) { (void)a; return (uint64_t)-1; }  /* 注册表读取值 */
void     FUN_140003864(void *a, const uint16_t *b, uint32_t c, uint32_t d, void *e, uint32_t f, uint32_t g, void *h) { (void)a;(void)b;(void)c;(void)d;(void)e;(void)f;(void)g;(void)h; }
void    *FUN_14005b374(void *a, int16_t b, int16_t c) { (void)a;(void)b;(void)c; return a; } /* 行切分 */
void     PECMD_AppendQuotedString(int64_t *a, void *b, int c) { (void)a;(void)b;(void)c; }              /* 串填充 */
uint64_t FUN_1400048c4(int64_t *a) { (void)a; return 0; }
void     FUN_1400607a4(const uint16_t *a, const uint16_t *b, int c) { (void)a;(void)b;(void)c; }
void    *FUN_140017770(void *a, const char *b) { (void)a;(void)b; return (void *)0; }        /* 命名互斥/事件 */
void     FUN_1400177b8(void *a) { (void)a; }                                                 /* 释放互斥/事件 */
uint32_t FUN_14000bfcc(uint64_t a, uint64_t b, const uint16_t *c) { (void)a;(void)b;(void)c; return 0; } /* 执行固定操作 */
/* WinAPI 桩: 资源串读取 / CRT atexit */
uint64_t LoadStringA(void){ return 0; }
int      atexit(void (*fn)(void)) { (void)fn; return 0; }
/* FUN_14005b540 — 去除串内前导/尾随空白 (FUN_1400987ec 使用) */
void     FUN_14005b540(uint16_t *a, int b) { (void)a;(void)b; }

/* ---- P4 wave-4 helper 定义 (与头部前置声明对应) ---- */
void     FUN_14000500c(void) {}
void     FUN_140005738(int a, const char *b) { (void)a;(void)b; }
uint64_t FUN_140008c5c(uint64_t a, uint64_t b, void *c, void *d) { (void)a;(void)b;(void)c;(void)d; return 0; }
uint64_t FUN_14000befc(void *p) { (void)p; return 0; }
uint64_t FUN_14005370c(uint32_t *a, void *b, uint64_t c) { (void)a;(void)b;(void)c; return 0; }
void    *FUN_140057334(void *a, longlong b, int c, void *d, int e, int f, int g, int h, void *i, void *j, uint32_t k) { (void)a;(void)b;(void)c;(void)d;(void)e;(void)f;(void)g;(void)h;(void)i;(void)j;(void)k; return (void*)0; }
void    *FUN_14005762c(void *a, longlong b, int c, void *d, int e, int f, int g, int h, void *i, void *j, uint32_t k) { (void)a;(void)b;(void)c;(void)d;(void)e;(void)f;(void)g;(void)h;(void)i;(void)j;(void)k; return (void*)0; }
int      FUN_14005ceec(const uint16_t *a, const char *b) { (void)a;(void)b; return 1; }
void     FUN_14005d694(void) {}
void    *FUN_140060834(uint64_t a, uint64_t b) { (void)a;(void)b; return (void*)0; }
void     FUN_14006355c(void *a, const WCHAR *b, int c, uint64_t d) { (void)a;(void)b;(void)c;(void)d; }
void     FUN_140063888(void *a, const WCHAR *b, int64_t c) { (void)a;(void)b;(void)c; }
void    *FUN_14006e3a4(const WCHAR *a) { (void)a; return (void*)0; }
void    *FUN_14006e74c(const WCHAR *a, char b, uint32_t *c) { (void)a;(void)b;(void)c; return (void*)0; }
void    *FUN_14009c720(void *a, longlong b, int c, void *d, int e, int f, int g, int h, uint16_t *i, void *j, uint32_t k) { (void)a;(void)b;(void)c;(void)d;(void)e;(void)f;(void)g;(void)h;(void)i;(void)j;(void)k; return (void*)0; }
void    *FUN_14009cacc(void *a, longlong b, int c, void *d, int e, int f, int g, int h, void *i, void *j, uint32_t k, WCHAR *l) { (void)a;(void)b;(void)c;(void)d;(void)e;(void)f;(void)g;(void)h;(void)i;(void)j;(void)k;(void)l; return (void*)0; }
uint64_t FUN_14009d4b8(uint64_t a, uint64_t b, const WCHAR *c, int16_t d, const WCHAR *e, int f) { (void)a;(void)b;(void)c;(void)d;(void)e;(void)f; return 0; }
void    *FUN_1400a43c4(const WCHAR *a, char b) { (void)a;(void)b; return (void*)0; }
void    *FUN_1400aa144(void *a, longlong b, int c, void *d, int e, int f, int g, int h, uint32_t i, WCHAR *j, uint8_t k) { (void)a;(void)b;(void)c;(void)d;(void)e;(void)f;(void)g;(void)h;(void)i;(void)j;(void)k; return (void*)0; }
void    *FUN_1400b8f10(void *a, longlong b, int c, void *d, int e, int f, int g, int h, void *i, uint32_t j, int *k, const WCHAR *l) { (void)a;(void)b;(void)c;(void)d;(void)e;(void)f;(void)g;(void)h;(void)i;(void)j;(void)k;(void)l; return (void*)0; }
void    *FUN_1400bca60(void *a, longlong b, int c, void *d, int e, int f, int g, int h, void *i, void *j, uint32_t *k, uint32_t l, uint32_t m, uint32_t n) { (void)a;(void)b;(void)c;(void)d;(void)e;(void)f;(void)g;(void)h;(void)i;(void)j;(void)k;(void)l;(void)m;(void)n; return (void*)0; }
int64_t  FUN_1400e5aac(longlong *a, void *b) { (void)a;(void)b; return 0; }
void    *RegisterServiceCtrlHandlerW(const WCHAR *name, void *handler) { (void)name;(void)handler; return (void*)0; }
int      SHGetSpecialFolderPathA(void *hwnd, char *buf, int folder, int create) { (void)hwnd;(void)buf;(void)folder;(void)create; return 0; }
char    *lstrcatA(char *dst, const char *src) { (void)dst;(void)src; return dst; }

/* ---- P4 wave-4 补桩: 声明存在但缺定义的内部 helper ---- */
uint      FUN_14001b608(uint64_t a) { (void)a; return 0; }
long long FUN_140031454(long long *a, pthreadmbcinfo b) { (void)a;(void)b; return 0; }
int       FUN_140067cf4(long long *a, uint64_t *b) { (void)a;(void)b; return 0; }

/* ---- P4 wave-4 helper 补定义 (声明已存在但缺实体) ---- */
void FUN_14004e2cc(uint64_t a, void *b) { (void)a;(void)b; }
uint64_t FUN_14004fb44(longlong *a, ulonglong b) { (void)a;(void)b; return 1; }
void FUN_14005d9a8(int64_t a, int b) { (void)a;(void)b; }
uint64_t FUN_1400630d0(int a) { (void)a; return 0; }
void FUN_14006e8f4(int64_t a) { (void)a; }

/* ---- P4 wave-4 helper 补定义 (2) ---- */
uint64_t FUN_14005c7c4(const char *a, const uint16_t *b) { (void)a;(void)b; return 0; }
void FUN_1400702b0(uint16_t **ps, const uint16_t *src) { (void)ps;(void)src; }
int64_t *FUN_14007034c(int64_t *param_1, const uint16_t *param_2) { (void)param_1;(void)param_2; return (int64_t*)0; }
