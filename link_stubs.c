typedef unsigned int uint;   /* 兼容 setupdi 桩 */
/* Auto-generated link stubs for undefined symbols (weak/no-op). */
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <string.h>
#include <stdarg.h>
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
typedef void *HBITMAP;
typedef void *SC_HANDLE;
typedef void *PSID;
typedef void *HLOCAL;
typedef void *HDESK;
typedef void *HSERVICE;
typedef uintptr_t UINT_PTR;
typedef long LONG_PTR;
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
undefined8 *PECMD_InitControlObjField(undefined8 *param_1, undefined8 param_2, uint param_3, undefined8 *param_4);   /* 控件对象构造 (定义见文件尾) */

/* ---- P4 wave-4 统一前置声明 (确保早放置的恢复体调用可正确编译) ---- */
typedef void *HGDIOBJ_H;
void   *RegisterServiceCtrlHandlerW(const WCHAR *name, void *handler);
int     SetServiceStatus(void *h, void *status);
WCHAR  *GetCommandLineW(void);
void   *CreateThread(void *sa, size_t st, void *rt, void *p, uint32_t f, uint32_t *tid);
void   *GetDesktopWindow(void);
HWND    GetParent(HWND w);
DWORD   GetCurrentThreadId(void);
DWORD   GetWindowThreadProcessId(HWND w, DWORD *pid);
void   *LoadLibraryA(const char *m);
FARPROC GetProcAddress(void *hm, const char *name);
unsigned long GetTickCount(void);
uint64_t GetProcessHeap(void);
int64_t *PECMD_AssignString(int64_t *, const uint16_t *);
void *TlsGetValue(unsigned long idx);
unsigned long DAT_14013c934; longlong DAT_14013cb18[8];
uint64_t SizeofResource(uint64_t a, uint64_t b);
uint64_t PECMD_EncodeDet(long long a, uint64_t b);
uint64_t *PECMD_AssignAnsiString(uint64_t *, char *);
void *FUN_140063224(uint64_t *a, uint64_t b);
uint16_t *StrChrW(const uint16_t *s, uint16_t c);
void *PECMD_BuildFontFromObject(int64_t a, void *b, const void *c);
const uint16_t *PECMD_LoadLocalizedString(void *hinst, uint32_t id, uint16_t *buf, int len);
const uint16_t *PECMD_LangLookupById(int a, const uint16_t **b);
uint64_t EndDialog(uint64_t a, int b);

void   *GetModuleHandleW(const WCHAR *m);
int     SetCurrentDirectoryW(const WCHAR *dir);
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
DWORD PECMD_RegOpenWithRetryPriv(HKEY param_1, LPCWSTR param_2, PHKEY param_3, REGSAM param_4, uint param_5);
void *OpenDesktopW(const WCHAR *n, uint64_t f, uint64_t acc, uint64_t flags);
LONG RegSetValueExW(void *k, const unsigned short *n, unsigned long r, unsigned long t, const unsigned char *d, unsigned long c);
HMODULE LoadLibraryW(const WCHAR *name);
unsigned int DragQueryFileW(uint64_t a, uint32_t b, void *c, uint32_t d);
uint16_t *PECMD_AllocStrSlot(uint16_t **out);
uint8_t *PECMD_MemMoveSafe(void *a, longlong b, longlong c);
void *DAT_14013cb10; void *DAT_14013ccf8;
void (*DAT_14013cb48)(...); void *DAT_14013cd18; void *DAT_14013cd20; void *DAT_14013cd28;
int (*DAT_14013cd30)(...); int (*DAT_14013cd38)(...); void (*DAT_14013cd40)(...);
void *DAT_14013cfb0;
uint64_t VirtualFree(void *a, uint64_t b, uint64_t c);
uint64_t lstrcatW(void *a, uint64_t b);
int64_t FUN_140065864(int64_t a, int64_t *b, int64_t *c, uint8_t *d, uint32_t e);
void *DAT_14013cf50;
void *DAT_1401293c0;
uint64_t FUN_140103020(const uint16_t *s) { (void)s; return 0; }
void *FUN_1400170b0(void **p) { (void)p; return 0; }
void *FUN_140070044(const char *s) { (void)s; return 0; }
void *DAT_14013d868; uint64_t DAT_14013e2a8; void *DAT_14013ca68;
uint64_t PECMD_OomPrompt(int a);
uint64_t FindResourceW(void *a, void *b, void *c);
uint64_t LoadResource(void *a, uint64_t b);
uint64_t LockResource(uint64_t a);
void *DAT_14013e1f8; void *DAT_14013e200;
int SetThreadDesktop(void *d); int SwitchDesktop(void *d); int CloseDesktop(void *d);
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
void PECMD_ScaleQuadByFactor(int64_t a, int *b, int *c, int *d, int *e);
int FUN_1400678f0(void *a, long long *b, short c) { (void)a;(void)b;(void)c; return 0; }
void PECMD_ParseSizeNumber(int64_t *pp, int64_t *out) { (void)pp; *out = 0; }
/* @0x1400706b4 size=25 — 容器字段初始化(直移) */
void PECMD_InitContainerFields(uint32_t *param_1)
{
  *param_1 = 0; param_1[1] = 0xffffffff;
  uint16_t *p1 = (uint16_t *)FUN_140065864(0,(int64_t *)(param_1 + 4),(int64_t *)(param_1 + 6),(uint8_t *)(param_1 + 10),2);
  *p1 = 0x23;
  uint64_t *p2 = (uint64_t *)FUN_140065864(0,(int64_t *)(param_1 + 0xc),(int64_t *)(param_1 + 0xe),(uint8_t *)(param_1 + 0x12),8);
  *p2 = 0;
  *(uint8_t *)((long long)param_1 + 9) = 0x20;
}
void FUN_1400284d4(long long *a, const void *b) { (void)a;(void)b; }
long long FUN_14003e220(void *a, unsigned int b, uint64_t c, uint64_t d) { (void)a;(void)b;(void)c;(void)d; return 1; }
void FUN_140062950(void *a) { (void)a; }
/* @0x1400e66d4 size=— 资源字体创建(直移) */
void PECMD_BuildResourceFont(longlong *param_1, UINT param_2, void *param_3)
{
  if (*param_1 != -1) {
    uint64_t local_res8 = 0;
    FUN_140063694(&local_res8,0x516);
    *(uint16_t *)local_res8 = 0;
    const uint16_t *w = (const uint16_t *)(uintptr_t)PECMD_LangLookupById(param_2,(const uint16_t **)0);
    if ((uintptr_t)w == 0) {
      PECMD_LoadLocalizedString((void *)(uintptr_t)DAT_14013ca68,param_2,(uint16_t *)(uintptr_t)local_res8,0x514);
      const uint16_t *bar = StrChrW((const uint16_t *)local_res8,(uint16_t)'|');
      w = (const uint16_t *)local_res8;
      if ((uintptr_t)bar != 0) w = bar + 1;
    }
    double d = (double)(intptr_t)DAT_1401293c0;
    void *h = (void *)(uintptr_t)PECMD_BuildFontFromObject((int64_t)(uintptr_t)param_3,&d,(const void *)w);
    *param_1 = (longlong)(intptr_t)h;
    FUN_14005b104(&local_res8);
  }
}
void (*DAT_14013cd90)(...) = 0;
/* @0x1400e6790 size=— 对话框回调 0x30/0x110(直移) */
uint64_t PECMD_AboutDlgProc(void *param_1, int param_2, void *param_3)
{
  if (param_2 == 0x30) {
    if ((uintptr_t)DAT_14013d868 == 0) FUN_140062950(param_1);
    if ((uintptr_t)DAT_14013e2a8 == 0) PECMD_BuildResourceFont((uint64_t *)&DAT_14013e2a8,0x3e9,param_3);
  } else if (param_2 == 0x110) {
    EndDialog((uint64_t)(uintptr_t)param_1,2);
  }
  return 0;
}
void FUN_14001b888(uint64_t a) { (void)a; }
/* ---- 早期放置的 wave-current 还原体所需 Win32 前置声明 (定义见字母桩区) ---- */
void     GetStartupInfoW(void *d);
uint64_t SetActiveWindow(void *h);
int      SetEndOfFile(void *h);
uint64_t SetForegroundWindow(void *h);
uint64_t BringWindowToTop(void *h);

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
void    *PECMD_CreateNamedWaitObj(const WCHAR *a, char b);
void    *FUN_1400aa144(void *a, longlong b, int c, void *d, int e, int f, int g, int h, uint32_t i, WCHAR *j, uint8_t k);
void    *FUN_1400b8f10(void *a, longlong b, int c, void *d, int e, int f, int g, int h, void *i, uint32_t j, int *k, const WCHAR *l);
void    *FUN_1400bca60(void *a, longlong b, int c, void *d, int e, int f, int g, int h, void *i, void *j, uint32_t *, uint32_t l, uint32_t m, uint32_t n);
int64_t  FUN_1400e5aac(longlong *a, void *b);
void    *RegisterServiceCtrlHandlerW(const WCHAR *name, void *handler);
int      SHGetSpecialFolderPathA(void *hwnd, char *buf, int folder, int create);
char    *lstrcatA(char *dst, const char *src);
typedef union _LARGE_INTEGER { struct { DWORD LowPart; LONG HighPart; } u; long long QuadPart; } LARGE_INTEGER;   /* wave-4 */
/* ---- wave-N p4 restored-helper extra Win32/minimal types (direct-port 支持) ---- */
typedef void *LPTHREAD_START_ROUTINE;
typedef BOOL (*WNDENUMPROC)(HWND, LPARAM);
typedef size_t SIZE_T;
typedef uint8_t byte;
typedef void (*code)(void);
typedef DWORD *LPDWORD;
typedef WCHAR *LPWCH;
typedef struct _RTL_CRITICAL_SECTION { void *DebugInfo; LONG LockCount; LONG RecursionCount; HANDLE OwningThread; HANDLE LockSemaphore; ULONG_PTR SpinCount; } RTL_CRITICAL_SECTION, CRITICAL_SECTION, *LPCRITICAL_SECTION, *PRTL_CRITICAL_SECTION;
typedef struct _PROCESS_INFORMATION { HANDLE hProcess; HANDLE hThread; DWORD dwProcessId; DWORD dwThreadId; } PROCESS_INFORMATION, *LPPROCESS_INFORMATION;
typedef struct _STARTUPINFOW { DWORD cb; LPWSTR lpReserved; LPWSTR lpDesktop; LPWSTR lpTitle; DWORD dwX; DWORD dwY; DWORD dwXSize; DWORD dwYSize; DWORD dwXCountChars; DWORD dwYCountChars; DWORD dwFillAttribute; DWORD dwFlags; WORD wShowWindow; WORD cbReserved2; void *lpReserved2; void *hStdInput; void *hStdOutput; void *hStdError; } STARTUPINFOW, *LPSTARTUPINFOW;
typedef struct _WIN32_FIND_DATAW { DWORD dwFileAttributes; FILETIME ftCreationTime; FILETIME ftLastAccessTime; FILETIME ftLastWriteTime; DWORD nFileSizeHigh; DWORD nFileSizeLow; DWORD dwReserved0; DWORD dwReserved1; WCHAR cFileName[260]; WCHAR cAlternateFileName[14]; } WIN32_FIND_DATAW, *LPWIN32_FIND_DATAW;
typedef struct tagPOINT { LONG x; LONG y; } POINT, tagPOINT, *LPPOINT;
unsigned int PECMD_WindowRectHitTest(HWND a, POINT b);
/* @0x1400e6350 size=— 子窗口枚举回调(直移) */
bool PECMD_EnumChildFindProc(POINT param_1, POINT *param_2)
{
  int r = PECMD_WindowRectHitTest((void *)(uintptr_t)(uint64_t)param_1.x,*param_2);
  if (r == 0) return 1;
  uint64_t u = SendMessageW((void *)(uintptr_t)(uint64_t)param_1.x,0x45e,(int64_t)param_2[1].x,(uint64_t)(uintptr_t)param_1.x);
  if ((u >> 0x11 & 1) != 0) param_2[3] = param_1;
  EnumChildWindows((void *)(uintptr_t)(uint64_t)param_1.x,(WNDENUMPROC)PECMD_EnumChildFindProc,(uint64_t)(uintptr_t)param_2);
  return param_2[3].x == 0 && param_2[3].y == 0;
}

bool PECMD_EnumChildFindProc(POINT a, POINT *b);
int64_t (*DAT_14013ce30)(void);
void *DAT_14013d3b8 = 0;
int64_t DAT_14013a24f = 0;
int64_t DAT_14013a24c = 0;
longlong DAT_14013e118 = 0; longlong DAT_14013e120 = 0; longlong DAT_14013e128 = 0;
typedef struct tagRECT { LONG left; LONG top; LONG right; LONG bottom; } RECT, tagRECT, *LPRECT;
typedef struct tagMSG { HWND hwnd; UINT message; ulonglong wParam; longlong lParam; DWORD time; POINT pt; } MSG, tagMSG;
typedef void *pthreadlocinfo;
typedef struct _SHELLEXECUTEINFOW { DWORD cbSize; ULONG fMask; HWND hwnd; void *lpVerb; void *lpFile; void *lpParameters; void *lpDirectory; int nShow; void *hInstApp; void *lpIDList; void *lpClass; void *hkeyClass; DWORD dwHotKey; void *hIconOrMonitor; void *hProcess; } SHELLEXECUTEINFOW, *LPSHELLEXECUTEINFOW;

/* ---- wave-2 restored-helper 内部 FUN_ 前置声明 (定义在文件后部, 需先声明供 new 桩调用) ---- */
void        FUN_1400166b4(void);
int         FUN_140067d20(long long *, int *);
int         lstrlenW(const WCHAR *);
long long  *PECMD_SkipLeadingControlChars(long long *);
long long   FUN_140064a34(uint16_t *);
long long   PECMD_WideStrToInt64(uint16_t *);
long long   FUN_14006b8fc(long long *);
void        FUN_1400ef91c(long long, uint, uint64_t);
void        FUN_140005344(void);
uint16_t   *FUN_14000531c(uint16_t *);
uint8_t    *PECMD_MemMoveForward(uint8_t *, uint8_t *, int);
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
uint64_t    PECMD_LoadNtdllApis(void);
LPWSTR      StrRChrW(const WCHAR *, const WCHAR *, WCHAR);
uint64_t    thunk_FUN_1400f429c(void *, short);
/* ---- wave-7 (P4 [240:288]) 内部依赖前置声明 (真实体见文件后部 wave-7 节) ---- */
int         FUN_140067cf4(long long *, uint64_t *);   /* @0x140067cf4 数值解析包装 */
void        PECMD_InitContainerFields(uint32_t *);                 /* @0x1400706b4 控件表初始化 */
uint        FUN_14001b608(uint64_t);                   /* @0x14001b608 注册表读取 */
long long   FUN_140031454(long long *, pthreadmbcinfo);/* @0x140031454 INDATA 串执行 */
void        PECMD_InitTableSlots(uint64_t *, int64_t *, uint64_t *, uint); /* @0x140063a6c 表初始化 */
extern long long DAT_14013d130;                        /* 传 PECMD_ExpandCommandLine 输出槽 (后部定义) */

uint64_t AbortSystemShutdownW(void) { return 0; }
uint64_t AddFontMemResourceEx(void) { return 0; }
uint64_t AddFontResourceW(void) { return 0; }
uint64_t AdjustTokenPrivileges(void) { return 0; }
uint64_t AllocConsole(void) { return 0; }
uint64_t AllocateAndInitializeSid(void) { return 0; }
int AppendMenuW(void *m, unsigned int f, uint64_t id, const unsigned short *s) { (void)m;(void)f;(void)id;(void)s; return 0; }
uint64_t AssignProcessToJobObject(void) { return 0; }
uint64_t AttachThreadInput(void) { return 0; }
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
uint64_t ClientToScreen(void) { return 0; }
uint64_t CloseClipboard(void) { return 0; }
uint64_t CloseHandle(void *h) { (void)h; return 0; }
int CloseServiceHandle(void *h) { (void)h; return 0; }
uint64_t ControlService(void) { return 0; }
uint64_t CopyFileW(uint64_t a, uint64_t b, int c) { (void)a;(void)b;(void)c; return 1; }
uint64_t CopyImage(void) { return 0; }
uint64_t CreateBitmap(void) { return 0; }
uint64_t CreateCompatibleBitmap(void) { return 0; }
uint64_t CreateCompatibleDC(void) { return 0; }
uint64_t CreateDialogParamW(void) { return 0; }
uint64_t CreateDirectoryW(void) { return 0; }
HRGN CreateEllipticRgn(int a, int b, int c, int d) { (void)a;(void)b;(void)c;(void)d; return (HRGN)0; }
void *CreateEventW(void *sa, int manual, int init, const unsigned short *name) { (void)sa;(void)manual;(void)init;(void)name; return (void*)0; }
uint64_t CreateFileMappingA(void) { return 0; }
uint64_t CreateFileMappingW(void) { return 0; }
uint64_t CreateFileW(void) { return 0; }
uint64_t CreateFileA(void) { return 0; }
uint64_t CreateFontW(void) { return 0; }
uint64_t CreateHardLinkW(uint64_t a, uint64_t b, void *c) { (void)a;(void)b;(void)c; return 1; }
uint64_t CreateMutexA(void) { return 0; }
uint64_t CreateMutexW(void *a, int b, void *c) { (void)a;(void)b;(void)c; return (uint64_t)(uintptr_t)1; }
uint64_t CreatePen(void) { return 0; }
uint64_t CreatePopupMenu(void) { return 0; }
uint64_t CreateProcessW(void) { return 0; }
uint64_t CreateRoundRectRgn(void) { return 0; }
void *CreateServiceW(void *mgr, const unsigned short *name, const unsigned short *disp, unsigned long acc, unsigned long type, unsigned long start, unsigned long err, const unsigned short *bin, const unsigned short *grp, unsigned long *tag, const unsigned short *deps, const unsigned short *acct, const unsigned short *pwd) { (void)mgr;(void)name;(void)disp;(void)acc;(void)type;(void)start;(void)err;(void)bin;(void)grp;(void)tag;(void)deps;(void)acct;(void)pwd; return (void*)0; }
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
uint64_t DeleteDC(void *a) { (void)a; return 1; }
uint64_t DeleteFileW(void) { return 0; }
int DeleteObject(void *obj) { (void)obj; return 1; }
uint64_t DeleteService(void) { return 0; }
uint64_t DestroyIcon(void) { return 0; }
uint64_t DestroyMenu(void) { return 0; }
int DestroyWindow(void *h) { (void)h; return 0; }
int DeviceIoControl(void *h, unsigned long code, void *in, unsigned long inb, void *out, unsigned long outb, unsigned long *ret, void *ov) { (void)h;(void)code;(void)in;(void)inb;(void)out;(void)outb;(void)ret;(void)ov; return 0; }
uint64_t DialogBoxIndirectParamW(void *a, void *b, void *c, void *d, uint64_t e) { (void)a;(void)b;(void)c;(void)d;(void)e; return 0; }
uint64_t DialogBoxParamW(void *a, void *b, void *c, void *d, uint64_t e) { (void)a;(void)b;(void)c;(void)d;(void)e; return 0; }
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
uint64_t ExpandEnvironmentStringsW(void) { return 0; }
uint64_t ExtTextOutW(void) { return 0; }
uint64_t PECMD_RunCommandLine(void) { return 0; }
/* @0x140006554 size=— 拖放文件枚举(直移) */
void PECMD_EnumDropFiles(void *param_1, longlong *param_2, longlong *param_3)
{
  unsigned int n = DragQueryFileW((uint64_t)(uintptr_t)param_1,0xffffffff,0,0);
  unsigned int i = 0;
  if ((int)n > 0) {
    for (;;) {
      unsigned int len = DragQueryFileW((uint64_t)(uintptr_t)param_1,i,0,0);
      void *heap = (void *)(uintptr_t)GetProcessHeap();
      uint16_t *f = (uint16_t *)(uintptr_t)HeapAlloc(heap,8,(long long)(int)(len * 2 + 0x13));
      if ((uintptr_t)f == 0) break;
      DragQueryFileW((uint64_t)(uintptr_t)param_1,i,f,len * 2 + 9);
      if ((uintptr_t)param_2 != 0 && i == 0) PECMD_AssignString((int64_t *)param_2,(const uint16_t *)f);
      if ((uintptr_t)param_3 != 0) {
      }
      HeapFree(heap,8,f);
      if ((uintptr_t)param_3 == 0) return;
      i++;
      if ((int)n <= (int)i) return;
    }
  }
}
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
/* @0x140017724 size=76 — 切换到 Default 桌面(直移) */
void PECMD_SwitchToDefaultDesktop(void)
{
  void *hDesktop = OpenDesktopW((const WCHAR *)L"Default",0,1,0x10000000);
  if ((uintptr_t)hDesktop != 0) {
    if (SetThreadDesktop(hDesktop) != 0) SwitchDesktop(hDesktop);
    CloseDesktop(hDesktop);
  }
}
/* @0x140017f54 size=25 — 释放回调对象(直移) */
void PECMD_ReleaseCallbackObj(int *param_1)
{
  if (*param_1 == 1 && *(long long *)(param_1 + 6) != 0) {
    if ((void *)DAT_14013cf50 == (void *)param_1) DAT_14013cf50 = 0;
    if (*(long long *)(param_1 + 6) != 0)
      ((void (*)(long long, uint64_t, uint64_t))*(void **)(param_1 + 2))(*(long long *)(param_1 + 6),0,0);
    if (*(void **)(param_1 + 6) != 0) VirtualFree(*(void **)(param_1 + 6),0,0x8000);
    *param_1 = 0; param_1[6] = 0; param_1[7] = 0;
  }
}
uint64_t PECMD_ScriptInit(void) { return 0; }
/* PECMD_TlsLogWrite 前置: TLS 日志全局槽 (定义见后部 0x14013c934/0x14013cb18/0x14013cb41 区) */
extern unsigned long DAT_14013c934;
extern longlong DAT_14013cb18[8];
extern uint8_t  DAT_14013cb41;
void *TlsGetValue(unsigned long idx);
/* @0x140018d8c size=— TLS 调试日志写者(直移) */
void PECMD_TlsLogWrite(uint64_t ctx, const uint16_t *fmt, uint64_t a, uint64_t b)
{
  (void)ctx;
  uint64_t local_res18 = a, local_res20 = b;
  longlong *plVar2 = (longlong *)(uintptr_t)TlsGetValue((uint64_t)DAT_14013c934);
  longlong *plVar3 = (longlong *)&DAT_14013cb18;
  if ((uintptr_t)plVar2 != 0 && *plVar2 != 0) plVar3 = plVar2;
  if (*plVar3 != 0) {
    EnterCriticalSection((void *)&DAT_14013e190);
    if (*plVar3 != 0) {
      if ((int)plVar3[3] - (int)plVar3[2] < 0x896) {
        plVar3[3] = plVar3[2] + 0x10896;
        PECMD_AllocString((uint64_t *)(plVar3 + 1),plVar3[2] + 0x10896);
      }
            va_list ap; memset(&ap,0,sizeof(ap)); memcpy(&ap,&local_res18,sizeof(ap));
      int n = _vsnwprintf((uint16_t *)(plVar3[1] + plVar3[2] * 2),0x7fd,fmt,ap);
      if (n < 0) n = 0;
      plVar3[2] = plVar3[2] + (longlong)n;
      *(uint16_t *)(plVar3[1] + plVar3[2] * 2) = 0;
      if ((0x10000 < plVar3[2]) || DAT_14013cb41 != 0) PECMD_FlushLogBuffer(1,plVar3);
    }
    LeaveCriticalSection((void *)&DAT_14013e190);
  }
}
uint64_t PECMD_WaitHandlesOrMessages(uint64_t param_1, int64_t param_2, int param_3, uint64_t *param_4) { (void)param_1;(void)param_2;(void)param_3;(void)param_4; return 0; }
void FUN_14001a56c(int param_1)   /* @0x14001a56c 经 NTDLL.NtShutdownSystem 关机 (decompiled.c 直移) */
{
    HMODULE hModule;
    FARPROC pFVar1;

    hModule = GetModuleHandleW((const WCHAR *)L"NTDLL");
    pFVar1 = GetProcAddress(hModule, "NtShutdownSystem");
    if (pFVar1 != (FARPROC)0) {
        ((int (*)(ulonglong))(uintptr_t)pFVar1)((ulonglong)((param_1 != 1) + 1));
    }
}
void FUN_14001a640(LPCWSTR param_1)   /* @0x14001a640 切换当前目录 (decompiled.c 直移) */
{
    int iVar1;
    WCHAR local_428[528];

    local_428[0] = L'\0';
    GetCurrentDirectoryW(0x208, local_428);
    iVar1 = lstrcmpiW(local_428, param_1);
    if (iVar1 != 0) {
        SetCurrentDirectoryW(param_1);
    }
}
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

uint64_t PECMD_XorEncode(const uint16_t *a, uint32_t b, uint64_t c) { (void)a;(void)b;(void)c; return 0; }
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
/* @0x14001be14 size=— 剥离成对引号(直移) */
short *PECMD_UnquoteString(short *param_1)
{
  short *local_res8[4]; local_res8[0] = param_1;
  uint64_t u = FUN_140103020(param_1);
  short s = *param_1;
  short *end = param_1 + ((u & 0xffffffff) - 1);
  for (; (s == 0x22 && (local_res8[0] = param_1, *end == 0x22)); end--) {
    param_1 = param_1 + 1;
    *end = 0;
    s = *param_1;
    local_res8[0] = param_1;
  }
  FUN_1400170b0((void **)local_res8);
  return local_res8[0];
}
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
/* @0x14001ea18 size=— 资源数据加载/解码(直移) */
uint8_t *PECMD_LoadEncodedResource(void *a, uint16_t *b, uint16_t *c, int64_t *d, unsigned int *e)
{
  uint32_t local_38[2]; local_38[0] = 0x20;
  uint8_t *puVar6 = 0;
  uint32_t *puVar7 = local_38;
  if ((uintptr_t)e != 0) puVar7 = e;
  uint16_t uVar1 = *(uint16_t *)((long long)puVar7 + 2);
  uint64_t ri = FindResourceW(a,b,c);
  uint64_t uVar3 = 0;
  if ((uintptr_t)ri != 0) uVar3 = SizeofResource((uint64_t)a,(uint64_t)ri);
  uint64_t uVar8 = (uint64_t)uVar3;
  uint8_t *hResData = puVar6;
  if ((uintptr_t)ri != 0) hResData = (uint8_t *)(uintptr_t)LoadResource(a,(uint64_t)ri);
  char cVar2 = 0;
  if ((*puVar7 & 1) != 0) {
    if ((uintptr_t)hResData == 0) goto Lbad;
    hResData = (uint8_t *)(uintptr_t)LockResource((uint64_t)(uintptr_t)hResData);
  }
  if ((uintptr_t)hResData == 0 || uVar8 == 0) goto Lbad;
  cVar2 = 0;
  if ((*puVar7 & 0x40) == 0) {
    uint64_t uVar4 = PECMD_EncodeDet((long long)hResData,uVar3);
    cVar2 = (char)uVar4;
    if (cVar2 == 0) goto Lb2f;
    if (((*puVar7 & 0x20) != 0) && ((uVar4 & 2) != 0)) return 0;
    PECMD_AssignAnsiString((uint64_t *)&local_38[0],0);
    uint64_t local_30 = (uint64_t)(uintptr_t)hResData;
    uint64_t local_28 = uVar8, local_20 = uVar8;
    FUN_140068984((long long *)&local_30,d,(char)uVar1);
    local_30 = 0;
    FUN_14005b104((long long *)&local_30);
  } else {
Lb2f:
    long long *p = (long long *)FUN_140063224((uint64_t *)*d,uVar8 + 0x411);
    d[2] = uVar8; d[1] = uVar8; *d = (long long)p;
    PECMD_MemMoveForward((uint8_t *)p,hResData,uVar3);
    if (uVar1 != 0) PECMD_XorEncode((const uint16_t *)*d,(uint32_t)uVar1,(uVar8 + 1) >> 1);
  }
  puVar6 = (uint8_t *)*d;
  d[2] = d[1] + 0x10;
  memset(puVar6 + d[1],0,0x10);
  memset(puVar6 + d[1],(uint64_t)uVar1,0xc);
Lbad:
  *puVar7 = *puVar7 | (int)cVar2 & 2U;
  return puVar6;
}
/* PECMD_FixKnownDlls32 — KnownDlls32 环境修复: 首个调用时读取系统模式标志, 在 64 位
   系统中注册 \\KnownDlls32 路径(经 ntdll 函数指针槽), 仅执行一次. */
uint FUN_14000e0bc(void) { return 0; }           /* 操作系统位宽探测 (no-op) */
uint64_t FUN_14006042c(void) { return 0; }       /* 系统目录盘符 (no-op) */
/* @0x14001d628 size=— ntdll 原生 API 惰加载(直移) */
uint64_t PECMD_LoadNtdllApis(void)
{
  if ((uintptr_t)DAT_14013cb10 == 0) {
    if ((uintptr_t)DAT_14013ccf8 == 0 &&
        (DAT_14013ccf8 = (void *)(uintptr_t)LoadLibraryW((const uint16_t *)L"ntdll.dll"),
         (uintptr_t)DAT_14013ccf8 == 0)) return 0;
    DAT_14013cb48 = (void (*)(...))GetProcAddress(DAT_14013ccf8,"RtlInitUnicodeString");
    DAT_14013cd18 = (void *)(uintptr_t)GetProcAddress(DAT_14013ccf8,"NtOpenFile");
    DAT_14013cd20 = (void *)(uintptr_t)GetProcAddress(DAT_14013ccf8,"NtCreateFile");
    DAT_14013cd28 = (void *)(uintptr_t)GetProcAddress(DAT_14013ccf8,"NtReadFile");
    DAT_14013cb10 = (void *)(uintptr_t)GetProcAddress(DAT_14013ccf8,"ZwOpenSection");
    DAT_14013cd40 = (void (*)(...))GetProcAddress(DAT_14013ccf8,"NtClose");
    DAT_14013cd30 = (int (*)(...))GetProcAddress(DAT_14013ccf8,"NtCreateDirectoryObject");
    DAT_14013cd38 = (int (*)(...))GetProcAddress(DAT_14013ccf8,"NtCreateSymbolicLinkObject");
  }
  return 1;
}
uint8_t DAT_14013d270;                           /* 一次性初始化标志 (静区, 初 0) */
void (*DAT_14013cb48)(...) = 0;                  /* RtlInitUnicodeString 类槽 (reroute, 初 0) */
int  (*DAT_14013cd30)(...) = 0;                  /* ZwOpenKey 类槽 */
int  (*DAT_14013cd38)(...) = 0;                  /* ZwQueryKey 类槽 */
void (*DAT_14013cd40)(...) = 0;                  /* ZwClose 类槽 */
void PECMD_FixKnownDlls32(void)
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
                PECMD_LoadNtdllApis();
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
/* @0x140023544 size=107 — 清空 D:\DebugPeMainU.LOG 调试日志(直移) */
void PECMD_TruncateDebugLog(void)
{
  void *h[4]; h[0] = 0;
  FUN_140003864((void **)h,(const unsigned short *)L"D:\\DebugPeMainU.LOG",0xc0000000,7,0,3,0x80,0);
  if ((uintptr_t)h[0] != 0) {
    SetEndOfFile(h[0]);
    if ((uintptr_t)h[0] != (uintptr_t)-1) CloseHandle((void *)(uintptr_t)h[0]);
  }
}
uint16_t *PECMD_NextToken(int64_t *a, int64_t *b, uint32_t c) { (void)a;(void)b;(void)c; return (uint16_t *)0; }
/* @0x1400250f0 size=— 启动注册表检查+自动挂载行(直移) */
void PECMD_AutoMountStartup(long long *param_1, LPCWSTR param_2)
{
  uint8_t buf[0x2d8]; uint8_t *p = buf;
  FUN_14001b888(0);
  if (FUN_14001b608((uint64_t)(uintptr_t)L"SysStartuped") == 0) {
    FUN_140077358();
    memset(buf,0,0x2d8);
    memcpy(p,(const unsigned short *)L"#22:INDATA*AutoMount ",0x2c);
    if ((uintptr_t)param_2 != 0) lstrcatW((void *)(p+0x10),(uint64_t)param_2);
    (void)FUN_140031454(param_1,(pthreadmbcinfo)p);
  }
}
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

longlong FUN_140003a20(longlong *param_1, undefined8 *param_2, byte param_3);
void FUN_140053e78(void) { }
short *FUN_1400547bc(longlong *param_1, longlong *param_2, longlong *param_3, short param_4,
                    short param_5)   /* @0x1400547bc 分隔符/引号跳过 (decompiled.c) */
{
    short *psVar1;

    FUN_14006764c(param_2, param_3, param_4, param_5);
    psVar1 = (short *)*param_2;
    if ((*psVar1 != 0) && ((*psVar1 == param_4 || (*psVar1 == param_5)))) {
        *param_2 = (longlong)(psVar1 + 1);
    }
    FUN_140003a20(param_1, param_3, 1);
    return psVar1;
}
uint64_t FUN_14005b0b8(void *a) { (void)a; return 0; }
void FUN_14005b104(void *ps) { (void)ps; }
/* @0x14005b184 size=— ANSI 串比较(直移) */
int PECMD_AnsiStrNCompare(char *param_1, longlong param_2, longlong param_3)
{
  longlong n = param_3 - 1;
  if (n >= 0) {
    longlong off = param_2 - (longlong)param_1;
    do {
      char *q = param_1 + off;
      char c = *param_1; param_1++;
      if ((char)(*q - c) != 0) return (int)(char)(*q - c);
      n--;
    } while (n >= 0);
  }
  return 0;
}
undefined8 PECMD_MatchPrefixN(ushort *param_1, undefined8 *param_2, int param_3){
    ushort uVar1,uVar2; ushort *local_res10[3];
    local_res10[0]=(ushort*)(uintptr_t)*param_2;
    while (param_3=param_3-1, -1<param_3) {
        uVar1=*param_1; param_1=param_1+1;
        uVar2=*local_res10[0]; local_res10[0]=local_res10[0]+1;
        if (uVar1!=uVar2) return 0;
    }
    if ((*local_res10[0]!=0)&&(((*local_res10[0]<9)||(0xd<*local_res10[0]))&&(*local_res10[0]!=0x20))) return 0;
    PECMD_SkipLeadingControlChars((long long*)local_res10);
    *param_2=(undefined8)(uintptr_t)local_res10[0];
    return 1;
}

HANDLE FUN_14005b228(LPTHREAD_START_ROUTINE param_1, LPVOID param_2, size_t param_3, DWORD param_4,
                     LPDWORD param_5, LPSECURITY_ATTRIBUTES param_6)   /* @0x14005b228 建线程并关闭句柄 (decompiled.c) */
{
    HANDLE hObject;

    hObject = CreateThread(param_6, param_3, param_1, param_2, param_4, (uint32_t *)param_5);
    if (hObject != (HANDLE)0) {
        CloseHandle(hObject);
    }
    return hObject;
}
void PECMD_DestroyTrayIcon(char *param_1) { (void)param_1; }
uint64_t PECMD_UpdateTrayIcon(void) { return 0; }
DWORD PECMD_QueryRegValueWithRetry(HKEY param_1, const WCHAR *param_2, const WCHAR *param_3, DWORD *param_4, BYTE *param_5, DWORD *param_6){
    DWORD DVar1; uint uVar2; intptr_t hlocal[2];
    uVar2=4; hlocal[0]=0;
    do {
        DVar1=PECMD_RegOpenWithRetryPriv(param_1,param_2,(HKEY*)hlocal,0x20019,uVar2);
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

/* @0x14005c5a0 size=123 — 注册表值写入(带打开/成功关闭)(直移) */
unsigned long PECMD_RegSetValueWithOpen(void *k, const unsigned short *a, const unsigned short *b, unsigned long c, unsigned char *d, unsigned long e)
{
  uint64_t r;
  void *lk = 0;
  if (a == 0 || (r = PECMD_RegOpenWithRetryPriv((HKEY)k, (LPCWSTR)a, (PHKEY)&lk, 0x20006, 4), k = lk, r == 0)) {
    r = RegSetValueExW((HKEY)k, (LPCWSTR)b, 0, c, d, e);
    if (lk != 0) RegCloseKey((HKEY)lk);
  }
  return r;
}
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
/* @0x14005daf8 size=159 — 按工作区缩放四格(直移) */
void PECMD_ScaleQuadByFactor(int64_t a, int *b, int *c, int *d, int *e)
{
  int sc = *(int *)(a + 0x17c);
  if (0 < sc) {
    *b = (*b * sc) / 0x60; *c = (*c * sc) / 0x60;
    *d = (*d * sc) / 0x60; *e = (*e * sc) / 0x60;
  }
}
/* @0x14005dec4 size=16 — 取对象偏移实例字段(直移) */
uint64_t PECMD_GetObjField20(longlong param_1)
{
  if (*(longlong *)(param_1 + 0x40) != 0)
    return *(uint64_t *)(*(longlong *)(param_1 + 0x40) + 0x20);
  return 0;
}
/* @0x14005ded4 size=157 — 启动时前台化窗口(直移) */
void PECMD_BringWindowToFront(void *param_1)
{
  STARTUPINFOW local_78; memset(&local_78,0,0x68);
  local_78.cb = 0x68;
  GetStartupInfoW((void *)&local_78);
  if ((((uint8_t)local_78.dwFlags & 1) == 0) || (local_78.wShowWindow != 0)) {
    ShowWindow(param_1,5);
    SetWindowPos(param_1,0,0,0,0,0,3);
    SetActiveWindow(param_1);
    SetForegroundWindow(param_1);
    BringWindowToTop(param_1);
  }
}
uint64_t PECMD_NextRandomSeed(void) { return 0; }
void FUN_14005e7dc(uint64_t *param_1) { (void)param_1; }
/* @0x140061470 size=— 10ms 待标志/超时循环(直移) */
void PECMD_WaitTickCount(void)
{
  uint32_t t0 = GetTickCount();
  for (;;) {
    if (t0 + 10 == GetTickCount()) return;
    if (DAT_14013a24f < 1) return;
    if ((int)((t0 + 10) - GetTickCount()) < 1) return;
    DAT_14013a24f = DAT_14013a24f - 1;
  }
}
void FUN_140061c44(void) { }
/* @0x140061ffc size=117 — 全局互斥锁(带安全描述符)(直移) */
uint64_t FUN_140061ffc(uint64_t a, int b, uint16_t *c)
{
  SECURITY_ATTRIBUTES local_48; uint8_t sd[0x27+1]; long long res[4];
  PECMD_EnableTokenPrivilege((const unsigned short *)L"SeCreateGlobalPrivilege",2,0x20);
  memset(res,0,0x27); sd[0]=0;
  local_48.bInheritHandle = 0;
  local_48.lpSecurityDescriptor = sd;
  local_48.nLength = 0x18;
  FUN_14005e7dc((uint64_t *)&local_48.lpSecurityDescriptor);
  CreateMutexW(&local_48,b,c);
  (void)a;
}
void PECMD_SetVariable(void *a, const WCHAR *b, const WCHAR *c) { (void)a;(void)b;(void)c; }
/* @0x1400633a8 size=— 动态串分配(直移) */
void PECMD_AllocStringSlot2(void **param_1, longlong param_2)
{
  *param_1 = 0;
  if (param_2 >= 0) {
    longlong *p;
    for (;;) {
      p = (longlong *)(uintptr_t)HeapAlloc((void *)(uintptr_t)DAT_14013d328,0,param_2 + 9);
      if ((uintptr_t)p != 0) break;
      if (PECMD_OomPrompt(2) != 4) break;
    }
    longlong *pl = p + 1;
    *(uint32_t *)((long long)p + 4) = 0xaa55;
    *p = param_2 + 1;
    *param_1 = (void *)pl;
    if ((uintptr_t)pl != 0) *(uint8_t *)pl = 0;
  }
}
/* @0x140063620 size=— 动态串槽分配(直移) */
uint16_t *PECMD_AllocStrSlot(uint16_t **out)
{
  *out = 0;
  uint64_t *p;
  for (;;) {
    p = (uint64_t *)(uintptr_t)HeapAlloc((void *)(uintptr_t)DAT_14013d328,0,10);
    if ((uintptr_t)p != 0) break;
    if (PECMD_OomPrompt(2) != 4) break;
  }
  uint64_t *pu = p + 1;
  *(uint32_t *)((long long)p + 4) = 0xaa55;
  *p = 2;
  *out = (uint16_t *)pu;
  if ((uintptr_t)pu != 0) *(uint16_t *)pu = 0;
  return *out;
}
/* @0x14006375c size=— 宽串追加(直移) */
longlong *PECMD_AppendWideStr(void *param_1p,LPCWSTR param_2)
{
  longlong *param_1 = (longlong *)param_1p;
  int n1 = 0;
  if ((uintptr_t)param_2 != 0) {
    if (*param_1 != 0) n1 = lstrlenW((const uint16_t *)*param_1);
    int n2 = lstrlenW(param_2);
    void *p = (void *)(uintptr_t)PECMD_HeapRealloc((void *)(uintptr_t)*param_1,(long long)((n2 + 1 + n1) * 2));
    *param_1 = (longlong)p;
    PECMD_MemMoveSafe((uint8_t *)((longlong)p + (longlong)n1 * 2),(long long)(uintptr_t)param_2,n2 * 2 + 2);
  }
  return param_1;
}
WCHAR *PECMD_StrDupA(WCHAR **ps, LPCWSTR src, int64_t a, int64_t b){ (void)ps;(void)src;(void)a;(void)b; return (WCHAR*)0; } /* @0x1400637dc 字符串追加 */
/* @0x1400639f0 size=— 泛型向量追加(直移) */
longlong PECMD_VectorAppendGen(longlong *param_1,longlong *param_2,longlong *param_3,uint8_t *param_4,uint param_5,longlong param_6)
{
  if (*param_2 - 1 <= *param_3) {
    longlong n = *param_2 + param_6;
    *param_2 = n;
    void *p = (void *)(uintptr_t)PECMD_HeapRealloc((void *)(uintptr_t)*param_1,(uint64_t)param_5 * (uint64_t)n);
    *param_1 = (longlong)p;
  }
  PECMD_MemMoveForward((uint8_t *)(*param_3 * (uint64_t)param_5 + *param_1),param_4,param_5);
  longlong v = *param_3, base = *param_1;
  *param_3 = v + 1;
  return base + v;
}
/* @0x140063a6c size=— 定长槽表分配(直移) */
void PECMD_InitTableSlots(uint64_t *param_1, int64_t *param_2, uint64_t *param_3, uint param_4)
{
  *param_3 = 0; *param_2 = 1; *param_1 = 0;
  uint64_t n = *param_2 * (uint64_t)param_4;
  int64_t *p;
  for (;;) {
    p = (int64_t *)(uintptr_t)HeapAlloc((void *)(uintptr_t)DAT_14013d328,0,n + 8);
    if ((uintptr_t)p != 0) break;
    if (PECMD_OomPrompt(2) != 4) break;
  }
  *(uint32_t *)((long long)p + 4) = 0xaa55;
  *p = n;
  *param_1 = (uint64_t)(uintptr_t)(p + 1);
  memset(p + 1,0,(uint64_t)param_4);
}
/* @0x140063b00 size=98 — 向量槽指针计算(必要时扩容)(直移) */
int64_t PECMD_VectorSlotPtr(int64_t param_1, int64_t *param_2, int64_t *param_3, unsigned int param_4)
{
  if (param_1 < 0) param_1 = 0;
  if (*param_3 - 1 <= param_1)
    PECMD_ArrayGrowRaw(param_2,param_3,param_4,(unsigned int)(param_1 - *param_3) + 2);
  return (uint64_t)param_4 * (uint64_t)param_1 + *param_2;
}
longlong *FUN_140063b64(longlong *param_1){
    longlong lVar1,lVar4; int iVar2; undefined8 *puVar3;
    param_1[2]=0; *param_1=0; param_1[1]=1;
    do { puVar3=(undefined8*)HeapAlloc(DAT_14013d328,0,0x10); if (puVar3!=0) break; iVar2=FUN_1400630d0(2); } while (iVar2==4);
    *(undefined4*)((longlong)puVar3+4)=0xaa55; *(undefined8*)puVar3=8;
    lVar1=param_1[1]; lVar4=0; *param_1=(longlong)(puVar3+1);
    if (0<lVar1) { do { *(undefined8*)(uintptr_t)(*param_1+lVar4*8)=0; lVar4++; } while (lVar4<lVar1); }
    return param_1;
}

uint64_t PECMD_ExpandDrivePath(const uint16_t *a, uint64_t b, uint16_t *c, longlong *d) { (void)a;(void)b;(void)c;(void)d; return 0; }
/* @0x1400660ac size=— 匹配 token 并前进(直移) */
char PECMD_MatchTokenAdvance(char *param_1,void *param_2p,int param_3)
{
  longlong *param_2 = (longlong *)param_2p;
  if (param_3 < 0) param_3 = lstrlenA(param_1);
  uint16_t *p = (uint16_t *)*param_2;
  char c = (char)FUN_14005c72c(param_1,p,param_3);
  if (c != 0) { *param_2 = (longlong)(p + param_3); PECMD_SkipLeadingControlChars((long long *)param_2); }
  return c;
}
uint64_t FUN_1400668ec(void *a, uint64_t b, const void *c, const void *d) { (void)a;(void)b;(void)c;(void)d; return 0; }
void FUN_14006764c(longlong *param_1, longlong *param_2, short param_3, short param_4){
    WCHAR WVar1; const WCHAR *pWVar2; short *psVar3; int iVar4; const WCHAR *pWVar5;
    PECMD_SkipLeadingControlChars((long long*)param_1);
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

/* @0x1400679b0 size=43 — short 解析并写回(直移) */
void PECMD_ParseShortStore(uint64_t *param_1, int *param_2, short param_3)
{
  long long res[3]; res[0] = (long long)*param_2;
  int r = FUN_1400678f0(param_1,res,param_3);
  if (r > 0) *param_2 = (int)res[0];
}
/* PECMD_ParseIntegerString — 解析带符号/进制前缀 (0x/0o/0b) 的十进制-整数字串.
   跳过前导空白后解析并写回 *param_2; 失败返回 0. */
/* @0x14005b154 size=48 — 跳过前导控制/空白字符(直移) */
long long *PECMD_SkipLeadingControlChars(long long *param_1)
{
  if (*param_1 != 0) {
    for (;;) {
      uint16_t *p = (uint16_t *)*param_1;
      if (!((8 < *p && *p < 0xe) || *p == 0x20)) break;
      *param_1 = (long long)(p + 1);
    }
  }
  return param_1;
}
long long FUN_140064a34(uint16_t *s) { (void)s; return 0; } /* 解析 16 进制数字串 */
uint64_t PECMD_ParseIntegerString(long long *param_1, uint64_t *param_2)
{
    uint64_t  uVar1;
    uint16_t *puVar2;
    uint16_t  uVar3;
    long long *plVar4;
    char      cVar5;

    plVar4 = param_1;
    PECMD_SkipLeadingControlChars((long long *)param_1);
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
    uVar1 = PECMD_WideStrToInt64(puVar2);
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
/* @0x14006a7f4 size=38 — 解析数值, 成功(非0)时跳过前导空白, 返回低32位 (直移; ParseSizeNumber 为叶桩) */
uint64_t PECMD_ParseSizeAndSkipWs(int64_t *param_1, uint64_t *param_2)
{
  int64_t v = 0;
  PECMD_ParseSizeNumber(param_1, &v);
  if ((int)v != 0) PECMD_SkipLeadingControlChars((long long *)param_1);
  return (uint64_t)v & 0xffffffff;
}
uint64_t PECMD_EncodeStringId(void) { return 0; }
/* @0x14006f884 size=131 — 读环境变量到动态串(直移) */
void PECMD_GetEnvVarToStr(LPCWSTR param_1, uint64_t *param_2)
{
  unsigned long n;
  PECMD_AllocString(param_2,0x104);
  *(uint16_t *)*param_2 = 0;
  n = GetEnvironmentVariableW(param_1,(uint16_t *)(uintptr_t)*param_2,0x104);
  if ((int)n > 0 && *(short *)*param_2 == 0) {
    PECMD_AllocString(param_2,(long long)(int)(n + 2));
    GetEnvironmentVariableW(param_1,(uint16_t *)(uintptr_t)*param_2,n + 1);
  }
}
/* @0x14006fd1c size=— 冒号分隔字段切分(直移) */
void FUN_14006fd1c(longlong *param_1,longlong *param_2,longlong *param_3,longlong *param_4,longlong *param_5,longlong *param_6)
{
  longlong *pl = param_1;
  PECMD_SkipLeadingControlChars((long long *)param_1);
  *(uint16_t *)*param_6 = 0;
  PECMD_SplitTokenTrimWs(pl,param_6,0x3b);
  if (*(short *)*param_1 == 0x3b) *param_1 = (longlong)((short *)*param_1 + 1);
  PECMD_SkipLeadingControlChars((long long *)param_1);
  longlong v = *param_6;
  *param_2 = v; *param_3 = v; *param_4 = v;
  short *p = (short *)*param_3;
  while (*p != 0 && *(short *)*param_3 != 0x3a) { p = (short *)*param_3 + 1; *param_3 = (longlong)p; }
  if (*(short *)*param_3 != 0) { *(short *)*param_3 = 0; *param_3 = *param_3 + 2; }
  p = (short *)*param_3;
  while ((*param_4 = (longlong)p, *p != 0 && *(short *)*param_4 != 0x3a)) { p = (short *)*param_4 + 1; }
  if (*(short *)*param_4 != 0) { *(short *)*param_4 = 0; *param_4 = *param_4 + 2; }
  p = (short *)*param_4;
  while ((*param_5 = (longlong)p, *p != 0 && *(short *)*param_5 != 0x3a)) { p = (short *)*param_5 + 1; }
  if (*(short *)*param_5 != 0) { *(short *)*param_5 = 0; *param_5 = *param_5 + 2; }
}
/* @0x140070154 size=— 堆分配 0xaa55 魔法串(直移) */
longlong *PECMD_AllocMagicString(LPCWSTR param_1)
{
  int n = lstrlenW(param_1);
  longlong len = (longlong)n * 2 + 2;
  longlong *p;
  for (;;) {
    p = (longlong *)(uintptr_t)HeapAlloc((void *)(uintptr_t)DAT_14013d328,0,(long long)n * 2 + 10);
    if ((uintptr_t)p != 0) break;
    if (PECMD_OomPrompt(2) != 4) break;
  }
  *(uint32_t *)((long long)p + 4) = 0xaa55;
  *p = len;
  PECMD_MemMoveForward((uint8_t *)(p + 1),(uint8_t *)(uintptr_t)param_1,(int)len);
  return p + 1;
}
void PECMD_StrDupAssign(uint16_t **ps, const uint16_t *src) { (void)ps;(void)src; }
uint16_t *FUN_1400702d4(uint16_t **out, const uint16_t *src, int64_t len) { (void)out;(void)src;(void)len; return (uint16_t *)0; }
int64_t *FUN_1400702f0(int64_t *a, char *b, uint64_t c) { (void)a;(void)b;(void)c; return a; }
int64_t *PECMD_AssignString(int64_t *param_1, const uint16_t *param_2) { (void)param_1;(void)param_2; return (int64_t *)0; }
void FUN_1400703e4(long long *a, const WCHAR *b) { (void)a;(void)b; }
uint64_t PECMD_ParsePrefixColon(void) { return 0; }
void FUN_140077358(void) { return; }
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
long long PECMD_ExpandVarsRecursive(long long *a, WCHAR *b, void *c, int d, uint8_t e) { (void)a;(void)b;(void)c;(void)d;(void)e; return 0; }
/* @0x14007bf44 size=52 — 变量展开分派(递归/单趟)(直移) */
void PECMD_ExpandVarDispatch(void *param_1, WCHAR *param_2, void *param_3, int param_4, uint8_t param_5)
{
  long long *p = (long long *)param_1;
  if (*(char *)((long long)p + 0xda) == 0 && (*(uint8_t *)((long long)p + 0xd) & 0xf) == 0)
    PECMD_ExpandVarsRecursive(p,param_2,param_3,param_4,param_5);
  else
    PECMD_ExpandCommandLine(p,param_2,param_3,param_4,param_5);
}
uint64_t PECMD_SetVariableWithPrefix(void) { return 0; }
/* @0x14007de70 size=— 串覆盖或合并(直移) */
uint64_t *PECMD_StrSetOrConcat(uint64_t *param_1,uint64_t *param_2,LPCWSTR param_3)
{
  if ((uintptr_t)param_3 == 0) {
    PECMD_StrDupAssign((uint16_t **)param_2,(const uint16_t *)*param_1);
  } else {
    int n1 = 0;
    if ((uintptr_t)*param_1 != 0) n1 = lstrlenW((const uint16_t *)*param_1);
    int n2 = lstrlenW(param_3);
    long long len = (long long)((n2 + 1 + n1) * 2);
    long long *p;
    for (;;) {
      p = (long long *)(uintptr_t)HeapAlloc((void *)(uintptr_t)DAT_14013d328,0,len + 8);
      if ((uintptr_t)p != 0) break;
      if (PECMD_OomPrompt(2) != 4) break;
    }
    PECMD_MemMoveForward((uint8_t *)(p + 1),(uint8_t *)(uintptr_t)*param_1,(int)(n1*2));
    PECMD_MemMoveForward((uint8_t *)((long long)(p+1) + n1*2),(uint8_t *)(uintptr_t)param_3,(int)(n2*2));
    *param_2 = (uint64_t)(uintptr_t)(p+1);
  }
  return param_2;
}
uint64_t PECMD_SetCheckVariable(void) { return 0; }
uint64_t PECMD_NotifyMainWindowRefresh(void) { return 0; }
int64_t PECMD_QueryFontInfo(int64_t a, int *b, const void *c) { (void)a;(void)b;(void)c; return 0; }
uint64_t FUN_1400b1724(void) { return 0; }
uint64_t PECMD_CreateFont(void *a, void *b, void *c) { (void)a;(void)b;(void)c; return (uint64_t)(uintptr_t)1; }
int PECMD_ParseHexOrDec(long long *a, uint64_t *b) { (void)a; (void)b; return 1; }
/* @0x1400c11c0 size=52 — 十六进制/十进制解析 bool 封装(直移) */
bool PECMD_ParseHexOrDecBool(long long *param_1, int *param_2)
{
  uint64_t res[3]; res[0] = (uint64_t)*param_2;
  bool b = PECMD_ParseHexOrDec(param_1,res);
  if ((int)b > 0) { *param_2 = (int)res[0]; return 1; }
  return 0;
}
uint64_t FUN_1400d2e90(void) { return 0; }
/* @0x1400e3cd4 size=— 宽路径分配并展开(直移) */
uint64_t PECMD_ExpandPathAlloc2(LPCWSTR param_1, uint64_t *param_2, int64_t *param_3)
{
  int n = lstrlenW(param_1);
  PECMD_AllocString(param_2,(long long)(n * 2 + 0x105));
  longlong local_res10 = 0;
  PECMD_ExpandDrivePath(param_1,n * 2 + 0x104,(uint16_t *)(uintptr_t)*param_2,(longlong *)&local_res10);
  if (((uint64_t)(uintptr_t)param_3 & 1) == 0) {
    if ((uintptr_t)param_3 != 0) *param_3 = (int64_t)local_res10;
  } else if ((uintptr_t)local_res10 != 0) {
    ((uint16_t *)(uintptr_t)local_res10)[-1] = 0;
  }
  return *param_2;
}
uint64_t FUN_1400e3f80(void) { return 0; }
uint64_t PECMD_GetWindowTextAlloc(void) { return 0; }
uint64_t PECMD_GetOwnerWindow(uint64_t param_1) { (void)param_1; return 0; }
/* @0x1400e67e8 size=— 加载对话框资源并显示(直移) */
void PECMD_ShowAboutDialog(void)
{
  if ((uintptr_t)DAT_14013d868 == 0 || (uintptr_t)DAT_14013e2a8 == 0) {
    uint64_t ri = FindResourceW(DAT_14013ca68,(void *)(uintptr_t)0x271b,(void *)(uintptr_t)5);
    void *rd = (void *)(uintptr_t)LoadResource(DAT_14013ca68,(uint64_t)(uintptr_t)ri);
    if ((uintptr_t)rd != 0) {
      void *dt = (void *)(uintptr_t)LockResource((uint64_t)(uintptr_t)rd);
      if ((uintptr_t)dt != 0)
        DialogBoxIndirectParamW((void *)(uintptr_t)DAT_14013ca68,dt,0,PECMD_AboutDlgProc,0);
    }
  }
}
uint64_t PECMD_GetFileSize(void) { return 0; }
const uint16_t *FUN_1400e6d38(const uint16_t *a, uint64_t b, const uint16_t *c) { (void)b; return a; }
void FUN_1400e6d68(const uint16_t *a, uint64_t b) { (void)a;(void)b; }
uint64_t PECMD_ResDecode(void) { return 0; }
HWND FUN_1400e8574(undefined8 *param_1, longlong param_2){
    HWND pHVar4; void *puVar3; void *pHVar2; longlong lVar1; uint8_t *pb;
    pHVar4=(HWND)param_1[8];
    if (pHVar4==(HWND)0) {
        PECMD_ShowAboutDialog(); pHVar2=0;
        if ((void*)param_1!=(void*)&DAT_14013d130) pHVar2=GetDesktopWindow();
        puVar3=operator_new(0xa98); pHVar4=(HWND)0;
        if (puVar3!=(void*)0) pHVar4=(HWND)PECMD_InitControlObjField(puVar3,(undefined8)(uintptr_t)pHVar2,0x271b,(undefined8*)0);
        pb=(uint8_t*)pHVar4; lVar1=*(longlong*)pHVar4;
        pb[0x121]=1; *(undefined8**)(pb+0x290)=param_1; pb[0x122]=1; pb[0x120]=0x10;
        *(longlong*)(pb+0x2a0)=param_2;
        ((void(*)(void*,uint64_t,uint64_t))(uintptr_t)(lVar1+0x10))(pHVar4,0x271d,0);
    }
    return pHVar4;
}

undefined4 PECMD_ModalDialogPump(longlong *param_1, ulonglong param_2);
/* @0x1400e95f4 模态对话框消息泵 (decompiled.c 直移) */
longlong PECMD_ModalMsgPumpEx(longlong *param_1,ulonglong param_2)
{
  bool bVar1;
  bool bVar2;
  DWORD DVar3;
  DWORD DVar4;
  BOOL BVar5;
  int iVar6;
  HWND hWnd;
  HWND pHVar7;
  DWORD local_res8 [2];

  if (param_1[4] == 0) {
    *(undefined1 *)(param_1 + 0x24) = 0x10;
    ((void (*)(void *, int, int))(uintptr_t)(**(code **)(*param_1 + 0x10)))(param_1,0,0);
  }
  hWnd = GetParent((HWND)param_1[4]);
  if (hWnd == (HWND)0x0) {
    hWnd = (HWND)param_1[0x23];
  }
  pHVar7 = GetDesktopWindow();
  bVar2 = false;
  if (hWnd == pHVar7) {
    hWnd = (HWND)0x0;
  }
  bVar1 = false;
  if ((hWnd != (HWND)0x0) && (bVar1 = bVar2, (param_2 & 1) == 0)) {
    local_res8[0] = 0;
    DVar3 = GetWindowThreadProcessId(hWnd,local_res8);
    DVar4 = GetCurrentThreadId();
    if (DVar3 == DVar4) {
      BVar5 = IsWindowEnabled(hWnd);
      if (BVar5 != 0) {
        BVar5 = IsWindowVisible(hWnd);
        if (BVar5 != 0) {
          EnableWindow(hWnd,0);
          bVar1 = true;
        }
      }
    }
  }
  iVar6 = PECMD_ModalDialogPump(param_1,0x10000);
  if (bVar1) {
    BVar5 = IsWindowEnabled(hWnd);
    if (BVar5 == 0) {
      EnableWindow(hWnd,1);
    }
  }
  return (longlong)iVar6;
}
uint64_t FUN_1400f21a8(void) { return 0; }
uint64_t FUN_140101e70(void) { return 0; }	/* CRT 标准库内联 */
uint64_t FillRect(void) { return 0; }
uint64_t FindClose(void) { return 0; }
uint64_t FindNextFileW(void) { return 0; }
uint64_t FindResourceW(void *a, void *b, void *c) { (void)a;(void)b;(void)c; return 1; }
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
int GetCursorPos(void *p) { (void)p; return 0; }
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
uint64_t GetProcessTimes(void) { return 0; }
uint64_t GetScrollInfo(void) { return 0; }
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
uint64_t GetWindowLongPtrW(void) { return 0; }
LONG GetWindowLongW(HWND hWnd, int nIndex) { (void)hWnd;(void)nIndex; return 0; }
int GetWindowRect(void *w, void *r) { (void)w;(void)r; return 0; }
uint64_t GetWindowTextLengthW(void) { return 0; }
uint64_t GetWindowTextW(void) { return 0; }
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
uint64_t KillTimer(void) { return 0; }
void LeaveCriticalSection(void *cs) { (void)cs; }
uint64_t LoadCursorW(void) { return 0; }
uint64_t LoadEnvi(void) { return 0; }
void *LoadIconW(void *hinst, const unsigned short *name) { (void)hinst;(void)name; return (void*)0; }
void *LoadLibraryA(const char *m) { (void)m; return (void *)0; }
uint64_t LoadLibraryExW(void) { return 0; }
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
void *OpenSCManagerW(const void *a, const void *b, unsigned long acc) { (void)a;(void)b;(void)acc; return (void*)0; }
void *OpenServiceW(void *mgr, const unsigned short *name, unsigned long acc) { (void)mgr;(void)name;(void)acc; return (void*)0; }
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
int PostMessageW(void *w, unsigned int m, uint64_t a, uint64_t b) { (void)w;(void)m;(void)a;(void)b; return 0; }
uint64_t PostQuitMessage(void) { return 0; }
uint64_t Process32FirstW(void) { return 0; }
uint64_t Process32NextW(void) { return 0; }
int PtInRect(const void *r, POINT p) { (void)r;(void)p; return 0; }
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
int RegOpenKeyExW(void *k, const unsigned short *s, unsigned long o, unsigned long a, void **out) { (void)k;(void)s;(void)o;(void)a;(void)out; return 0; }
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
int ResetEvent(void *h) { (void)h; return 0; }
uint64_t ResumeThread(void) { return 0; }
uint64_t SHChangeNotify(void) { return 0; }
uint64_t SHFileOperationW(void) { return 0; }
uint64_t SHGetSpecialFolderPathW(void) { return 0; }
uint64_t ScreenToClient(void) { return 0; }
uint64_t SearchPathW(void) { return 0; }
uint64_t SelectObject(void) { return 0; }
uint64_t SendMessageTimeoutW(void) { return 0; }
intptr_t SendMessageW(void *h, UINT m, uint64_t w, uint64_t l) { (void)h;(void)m;(void)w;(void)l; return 0; }
uint64_t SetActiveWindow(void *h) { (void)h; return 0; }
uint64_t SetBkColor(void) { return 0; }
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
uint64_t SetFilePointer(void) { return 0; }
uint64_t SetFilePointerEx(void) { return 0; }
uint64_t SetFocus(void) { return 0; }
uint64_t SetForegroundWindow(void *h) { (void)h; return 0; }
void SetLastError(DWORD e) { (void)e; }
uint64_t SetLayeredWindowAttributes(void) { return 0; }
uint64_t SetLocalTime(void) { return 0; }
int SetMenuItemBitmaps(void *m, unsigned int id, unsigned int f, void *b1, void *b2) { (void)m;(void)id;(void)f;(void)b1;(void)b2; return 0; }
uint64_t SetParent(void) { return 0; }
uint64_t SetPixel(void) { return 0; }
uint64_t SetProcessWorkingSetSize(void *h, uint64_t a, uint64_t b) { (void)h;(void)a;(void)b; return 1; }
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
int SetWindowPos(void *w, void *after, int x, int y, int cx, int cy, unsigned int f) { (void)w;(void)after;(void)x;(void)y;(void)cx;(void)cy;(void)f; return 0; }
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
uint64_t SizeofResource(uint64_t a, uint64_t b) { (void)a;(void)b; return 0; }
void Sleep(unsigned long ms) { (void)ms; }
uint64_t SleepEx(void) { return 0; }
uint64_t StartServiceCtrlDispatcherW(void) { return 0; }
LPWSTR StrChrW(const WCHAR *s, WCHAR c) { (void)s;(void)c; return (LPWSTR)0; }
uint64_t StrCmpNIA(void) { return 0; }
int StrCmpNIW(const WCHAR *a, const WCHAR *b, int n) { (void)a;(void)b;(void)n; return 0; }
void *OpenDesktopW(const WCHAR *n, uint64_t f, uint64_t acc, uint64_t flags) { (void)n;(void)f;(void)acc;(void)flags; return (void*)(uintptr_t)1; }
int SetThreadDesktop(void *d) { (void)d; return 1; }
int SwitchDesktop(void *d) { (void)d; return 1; }
int CloseDesktop(void *d) { (void)d; return 1; }
unsigned int DragQueryFileW(uint64_t a, uint32_t b, void *c, uint32_t d) { (void)a;(void)b;(void)c;(void)d; return 0; }
void FUN_140068984(long long *a, long long *b, char c) { (void)a;(void)b;(void)c; }
/* @0x1400e63c8 size=— 枚举窗口回调(直移) */
bool PECMD_EnumWindowFindProc(POINT param_1, POINT *param_2)
{
  int r = PECMD_WindowRectHitTest((void *)(uintptr_t)(uint64_t)param_1.x,*param_2);
  if (r != 0) {
    uint32_t pid[2];
    GetWindowThreadProcessId((void *)(uintptr_t)(uint64_t)param_1.x,(unsigned long *)pid);
    if (pid[0] == (uint32_t)param_2[2].x) {
      uint64_t u = SendMessageW((void *)(uintptr_t)(uint64_t)param_1.x,0x45e,(int64_t)param_2[1].x,(uint64_t)(uintptr_t)param_1.x);
      if ((u >> 0x11 & 1) != 0) param_2[3] = param_1;
      EnumChildWindows((void *)(uintptr_t)(uint64_t)param_1.x,(WNDENUMPROC)PECMD_EnumChildFindProc,(uint64_t)(uintptr_t)param_2);
      return param_2[3].x == 0 && param_2[3].y == 0;
    }
  }
  return 1;
}
/* @0x1400ec084 size=— 消息信封结构填充(直移) */
void PECMD_FillMsgEnvelope(uint64_t param_1, uint64_t *param_2, long long param_3, uint64_t param_4)
{
  (void)param_1;
  memset(param_2,0,0x38);
  *(uint32_t *)param_2 = 0x38;
  void *h = *(void **)(param_3 + 0x20);
  if (param_4 == 0) {
    void *parent = (void *)(uintptr_t)GetParent(h);
    *(uint32_t *)((long long)param_2 + 4) = 1;
    param_2[2] = (uint64_t)(uintptr_t)h;
    param_2[1] = (uint64_t)(uintptr_t)parent;
  } else {
    *(uint32_t *)((long long)param_2 + 4) = 0;
    param_2[1] = (uint64_t)(uintptr_t)h;
    param_2[2] = param_4;
  }
}
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
uint64_t TerminateJobObject(void) { return 0; }
uint64_t TerminateProcess(void) { return 0; }
uint64_t TerminateThread(void) { return 0; }
uint64_t TrackPopupMenu(void) { return 0; }
int TranslateMessage(const void *m) { (void)m; return 0; }
uint64_t UnhookWindowsHookEx(void) { return 0; }
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
uint64_t WriteFile(void) { return 0; }
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
uint64_t thunk_FUN_140072814(void *a, longlong *b, const char *c) { (void)a;(void)b;(void)c; return 0; }
uint64_t u__26_INDATA_140121fe0(void) { return 0; }
uint64_t u_____D__140120a40(void) { return 0; }
uint64_t wsprintfA(void) { return 0; }
int wsprintfW(unsigned short *out, const unsigned short *fmt, ...) { (void)out;(void)fmt; return 0; }
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
uint64_t PECMD_EncodeDet(long long a, uint64_t b) { (void)a;(void)b; return 1; }
uint64_t PECMD_ParseHashNumbers(void) { return 0; }
uint64_t PECMD_GetComboItemText(void) { return 0; }
/* @0x14007e34c size=87 — 容器/对象字段初始化(直移) */
uint32_t *PECMD_InitFieldContainer(uint32_t *param_1, uint8_t param_2)
{
  *(uint8_t *)((long long)param_1 + 10) = param_2;
  *(uint8_t *)(param_1 + 2) = 0;
  *(uint8_t *)((long long)param_1 + 10) = 0;
  PECMD_InitTableSlots((uint64_t *)(param_1 + 4),(int64_t *)(param_1 + 6),(uint64_t *)(param_1 + 8),2);
  *(uint8_t *)(param_1 + 0x12) = 0;
  PECMD_InitTableSlots((uint64_t *)(param_1 + 0xc),(int64_t *)(param_1 + 0xe),(uint64_t *)(param_1 + 0x10),8);
  PECMD_InitContainerFields(param_1);
  return param_1;
}
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
/* @0x1400540a8 size=24 — 颜色/坐标消息封装(直移) */
uint64_t PECMD_SendColorMsg(long long param_1, uint param_2, uint param_3, uint param_4, uint param_5)
{
  if ((int)param_2 < 0 || (int)param_3 < 0 || (int)param_4 < 0 || (int)param_5 < 0) return 0x80070057;
  SendMessageW((void *)*(void **)(*(long long *)(param_1 + 0x38) + 0x20),0x465,0,
    (uint64_t)((param_5 & 0xff) + (((param_2 & 0xff) * 0x100 + (param_3 & 0xff)) * 0x100 + (param_4 & 0xff)) * 0x100));
  return 0;
}
uint64_t PECMD_InitDragDrop(void) { return 0; }
uint64_t PECMD_IsSetupClass(void) { return 0; }
uint64_t PECMD_SetControlState(void) { return 0; }
/* @0x1400b89dc size=139 — 基于字体对象构建 LOGFONT 并建新字体(直移) */
void *PECMD_BuildFontFromObject(int64_t a, void *b, const void *c)
{
  uint8_t lb[0x5c]; uint8_t *lp = lb;
  memset(lp,0,0x5c);
  FUN_14005b0b8((uint64_t *)lp);
  *(uint32_t *)(lp + 16) = 400;
  GetObjectW((uint64_t)a,0x5c,lp);
  *(uint32_t *)(lp + 4) = 0;
  *(uint8_t *)(lp + 0x17) = 1;
  return (void *)(uintptr_t)PECMD_CreateFont((int *)lp,(double *)b,(void *)c);
}
uint64_t PECMD_ShowContextMenu(void) { return 0; }
/* @0x1400e57c0 size=— 窗口对象基构造(直移) */
uint64_t *PECMD_InitWinObjBase(uint64_t *param_1)
{
  *param_1 = (uint64_t)&PTR_FUN_14012b240;
  param_1[4] = 0; param_1[7] = 0; param_1[0x18] = 0;
  *(uint8_t *)(param_1 + 0xf) = *(uint8_t *)(param_1 + 0xf) & 0xfc;
  *(uint8_t *)(param_1 + 0x14) = 0xff; *(uint8_t *)(param_1 + 0x17) = 0xff;
  *(uint16_t *)((long long)param_1 + 0xa2) = 0xffff;
  param_1[0x19] = 0;
  *(uint32_t *)(param_1 + 8) = 0x80000000;
  param_1[6] = 0; param_1[5] = 0;
  *(uint8_t *)((long long)param_1 + 0x61) = 0;
  param_1[0x11] = 0; param_1[0x10] = 0;
  *(uint32_t *)(param_1 + 0x15) = 0x80000000;
  *(uint8_t *)((long long)param_1 + 0xac) = 0; *(uint8_t *)((long long)param_1 + 0xa4) = 0;
  *(uint32_t *)((long long)param_1 + 0x9c) = 0x80000000;
  *(uint32_t *)(param_1 + 0x13) = 0x80000000;
  *(uint32_t *)((long long)param_1 + 0x94) = 0x80000000;
  *(uint32_t *)(param_1 + 0x12) = 0x80000000;
  *(uint8_t *)((long long)param_1 + 0xa5) = 0;
  *(uint8_t *)((long long)param_1 + 0xad) = 0; *(uint8_t *)((long long)param_1 + 0xa1) = 0;
  *(uint32_t *)((long long)param_1 + 0x44) = 0x80000000;
  param_1[9] = 0;
  *(uint32_t *)((long long)param_1 + 0x54) = 0x80000000;
  *(uint32_t *)(param_1 + 0xb) = 0xdefbac;
  *(uint32_t *)((long long)param_1 + 0x5c) = 0xff0000;
  *(uint32_t *)(param_1 + 10) = 0xbf800000;
  *(uint8_t *)(param_1 + 0xc) = 0x4c;
  param_1[0xe] = 0; param_1[0xd] = 0;
  return param_1;
}
uint64_t PECMD_GetControlFont(void) { return 0; }
int PECMD_UpdateWindowStyleBits(int64_t a, unsigned int b, uint64_t c) { (void)a;(void)b;(void)c; return 0; }
/* @0x1400e8644 size=— 临界区保护的窗口对象获取(直移) */
longlong PECMD_GetWinIdLocked(longlong *param_1)
{
  EnterCriticalSection((void *)&DAT_14013e190);
  longlong v = param_1[8];
  if (v == 0) {
    void *h = (void *)FUN_1400e8574(param_1,0);
    param_1[8] = (longlong)h;
    if ((uintptr_t)h != 0)
      FUN_1400668ec(param_1,*(uint64_t *)((char *)h + 8),(const void *)L"&&__WinID",(const void *)L"0x%I64X");
    v = param_1[8];
  }
  LeaveCriticalSection((void *)&DAT_14013e190);
  return v;
}
/* @0x1400ece2c size=— 对象槽初始化(C)(直移) */
uint64_t *PECMD_InitObjectSlotC(uint64_t *param_1, uint64_t param_2)
{
  PECMD_InitWinObjBase(param_1);
  *param_1 = (uint64_t)&PTR_FUN_14012bad0;
  PECMD_AllocStrSlot((uint16_t **)(param_1 + 0x1b));
  param_1[0x1d] = param_2;
  FUN_140063b64((uint64_t *)(param_1 + 0x21));
  *(uint32_t *)((long long)param_1 + 0xfc) = 0xffffffff;
  *(uint32_t *)(param_1 + 0x20) = 0xffffffff;
  *(uint32_t *)(param_1 + 0x1f) = 0xffffffff;
  *(uint32_t *)((long long)param_1 + 0x104) = 0xffffffff;
  param_1[0x1c] = 0;
  *(uint32_t *)(param_1 + 0x24) = 0;
  *(uint8_t *)(param_1 + 0x14) = 0;
  param_1[0x1e] = 0;
  return param_1;
}
uint64_t FUN_1400ec698(int64_t a, uint64_t b) { (void)a;(void)b; return 0; }
/* @0x1400ec6a8 size=115 — 构造本地结构并 SendMessageW 0x432(直移) */
void PECMD_Send423ToWindow(long long param_1,long long param_2,uint64_t param_3,unsigned char *param_4,uint64_t param_5)
{
  uint64_t local_58[3]; uint8_t local_40[24]; uint64_t local_28;
  PECMD_FillMsgEnvelope(param_1,local_58,param_2,param_5);
  if ((uintptr_t)param_4 != 0) PECMD_MemMoveForward(local_40,param_4,0x10);
  local_28 = param_3;
  SendMessageW((void *)*(void **)(param_1 + 0x20),0x432,0,(uint64_t)local_58);
}
undefined8 PECMD_AppendValueNode(longlong param_1, const WCHAR *param_2, undefined8 param_3){
    int iVar1; undefined8 *puVar2,*puVar3;
    iVar1=*(int*)(param_1+0x118);
    puVar2=operator_new(0x10);
    if (puVar2==(undefined8*)0) puVar2=(undefined8*)0;
    else { *puVar2=0; PECMD_AllocStrSlot((uint16_t**)(puVar2+1)); }
    *puVar2=param_3;
    FUN_14007034c((int64_t*)(puVar2+1),param_2);
    puVar3=(undefined8*)PECMD_VectorSlotPtr((longlong)iVar1,(longlong*)(param_1+0x108),(longlong*)(param_1+0x110),8);
    *puVar3=(undefined8)(uintptr_t)puVar2;
    *(longlong*)(param_1+0x118)=(longlong)(iVar1+1);
    return 1;
}

void PECMD_LayoutChildWindows(int64_t a, unsigned char b) { (void)a;(void)b; }
uint64_t PECMD_InitWindowObjectF(void) { return 0; }
/* @0x1400f0648 size=— 对象槽 WebView 初始化(直移) */
uint64_t *PECMD_InitWebViewObj(uint64_t *param_1, uint64_t param_2)
{
  PECMD_InitWinObjBase(param_1);
  param_1[0x1a] = param_2;
  *(uint16_t *)((long long)param_1 + 0xa2) = 0xffff;
  *param_1 = (uint64_t)&PTR_FUN_14012bfb0;
  *(uint8_t *)(param_1 + 0x1b) = 0;
  *(uint32_t *)(param_1 + 8) = 0x80000000;
  *(uint8_t *)((long long)param_1 + 0x61) = 2;
  *(uint8_t *)((long long)param_1 + 0xad) = 0;
  *(uint32_t *)((long long)param_1 + 0x44) = 0x80000000;
  param_1[9] = 0; param_1[0x1c] = 0; param_1[0x11] = 0; param_1[0x10] = 0;
  *(uint32_t *)(param_1 + 0x15) = 0x80000000;
  *(uint8_t *)(param_1 + 0x14) = 0;
  *(uint8_t *)((long long)param_1 + 0xac) = 0;
  *(uint8_t *)((long long)param_1 + 0xa4) = 0;
  *(uint32_t *)((long long)param_1 + 0x9c) = 0x80000000;
  *(uint32_t *)(param_1 + 0x13) = 0x80000000;
  *(uint32_t *)((long long)param_1 + 0x94) = 0x80000000;
  *(uint32_t *)(param_1 + 0x12) = 0x80000000;
  *(uint8_t *)((long long)param_1 + 0xa5) = 0;
  *(uint8_t *)((long long)param_1 + 0xa1) = 1;
  return param_1;
}
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
/* @0x1400f2934 size=— 创建 SysIPAddress32 控件(直移) */
bool PECMD_CreateIpAddressCtl(longlong *param_1, DWORD param_2, int *param_3, void *param_4, uint param_5)
{
  int i1=param_3[3], i2=param_3[2], i3=param_3[1], i4=*param_3;
  long L = GetWindowLongW(param_4,-6);
  void *h = (void *)(uintptr_t)CreateWindowExW(0,(const uint16_t *)L"SysIPAddress32",0,param_2,*param_3,(unsigned long)i3,(unsigned long)(i2-i4),(unsigned long)(i1-i3),param_4,(void *)(uintptr_t)param_5,(void *)(intptr_t)L,0);
  param_1[4] = (longlong)(intptr_t)h;
  FUN_14006b8fc((void *)param_1);
  return param_1[4] != 0;
}
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

/* @0x1400fbfe0 size=81 — 控件对象O初始化(直移) */
uint64_t *PECMD_InitControlObjO(uint64_t *param_1,uint64_t param_2)
{
  PECMD_InitWinObjBase(param_1);
  param_1[0x1a] = param_2;
  *param_1 = (uint64_t)&PTR_FUN_14012ca50;
  (void)PECMD_AllocStrSlot((uint16_t **)(param_1 + 0x1b));
  *(uint8_t *)((long long)param_1 + 0xe4) = 0;
  *(uint32_t *)(param_1 + 0x1c) = 0x80000000;
  return param_1;
}
/* @0x1400fcf44 size=— 对象槽初始化(B)(直移) */
uint64_t *PECMD_InitScrollObj(uint64_t *a, uint64_t b)
{
  PECMD_InitWinObjBase(a);
  a[0x1b] = b;
  *(uint8_t *)((long long)a + 0x61) = 1;
  *(uint8_t *)((long long)a + 0xd2) = 0;
  a[0x11] = 0; a[0x10] = 0;
  *a = (uint64_t)&PTR_FUN_14012ccc0;
  *(uint32_t *)(a + 0x15) = 0x80000000;
  *(uint8_t *)(a + 0x14) = 0;
  *(uint16_t *)((long long)a + 0xa2) = 0xffff;
  *(uint8_t *)((long long)a + 0xac) = 0;
  *(uint8_t *)((long long)a + 0xa4) = 0;
  *(uint32_t *)((long long)a + 0x9c) = 0x80000000;
  *(uint32_t *)(a + 0x13) = 0x80000000;
  *(uint32_t *)((long long)a + 0x94) = 0x80000000;
  *(uint32_t *)(a + 0x12) = 0x80000000;
  *(uint8_t *)((long long)a + 0xa5) = 0;
  *(uint8_t *)(a + 0x1a) = 0;
  *(uint8_t *)((long long)a + 0xad) = 0;
  *(uint8_t *)((long long)a + 0xd1) = 0;
  a[0x1c] = 0;
  return a;
}
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
/* @0x1400e6860 size=— 对话框结束编排(直移) */
void PECMD_EndDialogDeferred(uint64_t param_1, int param_2)
{
  uint8_t b = *(uint8_t *)(param_1 + 0x120);
  if ((b & 1) == 0) {
    void *h;
    if (b == 0) h = *(void **)(param_1 + 0x20);
    else {
      *(uint8_t *)(param_1 + 0x120) = b | 0x80;
      PostMessageW(*(void **)(param_1 + 0x20),0,param_1,(long long)param_2);
      h = *(void **)(param_1 + 0x20);
    }
    EndDialog((uint64_t)(uintptr_t)h,(long long)param_2);
  } else {
    void *h = *(void **)(param_1 + 0x20);
    if ((uintptr_t)h != 0) {
      *(uint64_t *)(param_1 + 0x20) = 0;
      if (IsWindow(h) != 0) DestroyWindow((void *)h);
    }
  }
}
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

/* @0x140004fd4 size=56 — 压缩工作集后执行命令入口(直移) */
void PECMD_TrimWorkingSetAndExec(LPCWSTR param_1)
{
  extern void *DAT_14013cf70;
  extern int64_t FUN_14004EB34(void *, uint64_t, const WCHAR *);
  SetProcessWorkingSetSize((void *)(uintptr_t)GetCurrentProcess(),(uint64_t)-1,(uint64_t)-1);
  FUN_14004EB34(DAT_14013cf70,0,param_1);
}

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
/* @0x14006ffdc size=— 资源释放器(直移) */
void PECMD_FreeResourceSet(longlong *param_1)
{
  if (param_1[7] != 0) { DAT_14013cd90(); param_1[7] = 0; }
  if ((uintptr_t)param_1[8] != 0) { GlobalFree((uint64_t)(uintptr_t)param_1[8]); param_1[8] = 0; }
  if (param_1[10] != 0) { (*DAT_14013ce30)(); param_1[10] = 0; }
  if ((uintptr_t)param_1[9] != 0) { DeleteDC((void *)(uintptr_t)param_1[9]); param_1[9] = 0; }
  FUN_14005b104((long long *)param_1);
}
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
    PECMD_DestroyWindowObj(param_1);
}

uint8_t PTR_FUN_140126b20[8];
void SwitchToThisWindow(void *hw, int b){ (void)hw;(void)b; }
int GetClassNameW(void *hw, uint16_t *p, int n){ (void)hw;(void)p; return 0; }
/* @0x14007026c=PECMD_AssignAnsiString — ANSI 串复制(直移) */
uint64_t *PECMD_AssignAnsiString(uint64_t *a, char *b)
{
  *a = 0;
  if ((uintptr_t)b != 0) *a = (uint64_t)(uintptr_t)FUN_140070044(b);
  return a;
}
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
DWORD PECMD_RegOpenWithRetryPriv(HKEY param_1, LPCWSTR param_2, PHKEY param_3, REGSAM param_4, uint param_5)
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
/* @0x1400e8940 size=— 窗口对象销毁(直移) */
void PECMD_DestroyWindowObj(uint64_t *a)
{
  void *h = (void *)(uintptr_t)a[4];
  *a = (uint64_t)&PTR_FUN_14012b240;
  if ((uintptr_t)h != 0) {
    a[4] = 0;
    if (IsWindow(h) != 0) {
      SetWindowLongPtrW(h,-4,0x140001188);
      DestroyWindow((void *)(uintptr_t)h);
      if ((uintptr_t)a[7] != 0) { DeleteObject((void *)(uintptr_t)a[7]); a[7] = 0; }
      if ((uintptr_t)a[0xd] != 0) { DeleteObject((void *)(uintptr_t)a[0xd]); a[0xd] = 0; }
      if ((uintptr_t)a[0x18] != 0) PECMD_ReleaseGdiObjects((uint64_t *)(a + 0x18),(void *)(uintptr_t)a[4]);
    }
  }
}
void *PECMD_FreeResourceObject(void *a, unsigned int b){ (void)a;(void)b; return a; }

/* ---- 新增辅助桩 (core_b3 12 函数依赖; 签名与 decompiled 一致, 基类型等价) ---- */
void FUN_140102a90(uint64_t *dst, uint64_t v, uint64_t n){ (void)dst;(void)v;(void)n; }	/* CRT 标准库内联 */
uint16_t *PECMD_DriveTypeName(int i, uint16_t *out, int max){ (void)i;(void)max; return out; }
uint64_t PECMD_ParseControlMessage(int64_t *a, uintptr_t b, uintptr_t c, const uint16_t *d, uintptr_t e, int64_t f){ (void)a;(void)b;(void)c;(void)d;(void)e;(void)f; return 0; }
uint64_t PECMD_ControlEnableCommand(int64_t *a, uintptr_t b, uintptr_t c, const uint16_t *d, uintptr_t e, int64_t f){ (void)a;(void)b;(void)c;(void)d;(void)e;(void)f; return 0; }
uint64_t FUN_1400db648(uintptr_t hwnd, uint16_t *s, int64_t p3, int64_t *p4, uintptr_t p5, uint32_t color, int64_t p7){ (void)hwnd;(void)s;(void)p3;(void)p4;(void)p5;(void)color;(void)p7; return 0; }
uint8_t DAT_140127738[8];
uint8_t DAT_140127740[8];

/* @0x14005f96c size=— 磁盘几何控制查询(直移) */
int PECMD_QueryDeviceIoInfo(uintptr_t h, int size)
{
  uint32_t res[4]; uint32_t out; uint64_t buf[2]; int ret = 0x200;
  res[0]=0; out=0; memset(buf,0,0x14);
  int ok = DeviceIoControl((void *)h,0x70000,0,0,&out,0x18,(unsigned long *)res,0);
  if (ok != 0 || size == 0) size = ret;
  return size;
}
uint64_t *PECMD_GetDiskLayoutInfo(uintptr_t h, uint64_t *buf, uint32_t *out){ (void)h;(void)buf;(void)out; return (uint64_t*)0; }
uint32_t PECMD_GetDiskGeometry(const uint16_t *p, uintptr_t h){ (void)p;(void)h; return 0; }

/* --- r37 follow-up stubs (main-agent closure after subagent interrupt) --- */
/* @0x140006a4c size=83 — 判断设备路径前缀 (\Device\ / \ArcName\) (直移) */
uint32_t PECMD_IsDevicePathPrefix(LPCWSTR param_1)
{
  if (StrCmpNIW((const WCHAR *)L"\\Device\\",param_1,8) != 0 && StrCmpNIW((const WCHAR *)L"\\ArcName\\",param_1,9) != 0)
    return 0;
  return 1;
}
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

/* @0x140060b5c 统计换行/制表符数目 (decompiled.c 直移, 自包含) */
int PECMD_CountNewlines(ulonglong *param_1,int param_2,int param_3,int param_4)
{
  ulonglong *puVar1;
  char cVar2;
  ulonglong *puVar3;
  int iVar4;
  ulonglong *puVar5;
  int iVar6;

  puVar1 = (ulonglong *)((longlong)param_1 + (longlong)param_2 * 2);
  iVar4 = 1;
  iVar6 = param_3 >> 8;
  puVar3 = (ulonglong *)((longlong)puVar1 + -2);
  cVar2 = '\x01';
  puVar5 = param_1;
  if ((char)param_3 < '\x01') {
    puVar3 = (ulonglong *)((longlong)param_1 + (longlong)param_4 * 2 + -2);
    cVar2 = -1;
    param_1 = puVar3;
    puVar5 = puVar1;
  }
  if (iVar6 < 3) {
    for (; (param_1 <= puVar3 && (puVar5 <= param_1));
        param_1 = (ulonglong *)((longlong)param_1 + (longlong)cVar2 * 2)) {
      if ((short)*param_1 == 10) {
        iVar4 = iVar4 + 1;
      }
    }
  }
  else {
    cVar2 = (char)((uint)param_3 >> 8) * cVar2;
    puVar3 = (ulonglong *)((longlong)puVar3 + (longlong)iVar6 * -2 + 2);
    if ((char)param_3 < '\x01') {
      puVar3 = (ulonglong *)
               ((longlong)puVar5 +
               (((longlong)((param_4 / iVar6) * iVar6) - (longlong)iVar6) - (longlong)param_2) * 2);
      param_1 = puVar3;
    }
    if (iVar6 == 3) {
      for (; (param_1 <= puVar3 && (puVar5 <= param_1));
          param_1 = (ulonglong *)((longlong)param_1 + (longlong)cVar2 * 2)) {
        if ((*param_1 & 0xffffffffffff) == 0x4100300020) {
          iVar4 = iVar4 + 1;
        }
      }
    }
    else {
      for (; (param_1 <= puVar3 && (puVar5 <= param_1));
          param_1 = (ulonglong *)((longlong)param_1 + (longlong)cVar2 * 2)) {
        if (((short)param_1[1] == 0x41) && ((*param_1 & 0xffffffffff) == 0x30003000300020)) {
          iVar4 = iVar4 + 1;
        }
      }
    }
  }
  return iVar4;
}
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
/* @0x140025ce0 size=— 位置/状态关键字分派(直移) */
void PECMD_ParseScopeKeywords(int64_t *param_1, uint8_t *param_2, uint8_t *param_3, uint8_t *param_4, char *param_5, uint8_t *param_6)
{
  uint8_t local_res8[8]; uint8_t *puVar5 = local_res8;
  if ((uintptr_t)param_6 != 0) puVar5 = param_6;
  DAT_14013a24c = -1;
  if (*(short *)*param_1 == 0x2d) *param_5 = '-';
  while (((int)*param_5 == (uint)*(ushort *)*param_1 && ((int)*param_5 != (uint)*(ushort *)(*param_1 + 2)))) {
    *param_1 = *param_1 + 2;
    if ((int)PECMD_MatchPrefixN((void *)(uintptr_t)&DAT_14011c638,param_1,0) == 0) {
      if (PECMD_AdvanceAfterPrefix((const WCHAR *)L"top",param_1,3) == 0) {
        if (PECMD_AdvanceAfterPrefix((const WCHAR *)L"top-",param_1,4) == 0) {
          if (PECMD_AdvanceAfterPrefix((const WCHAR *)L"bottom",param_1,6) == 0) {
            if (PECMD_AdvanceAfterPrefix((const WCHAR *)L"pic",param_1,3) == 0) {
              if (PECMD_AdvanceAfterPrefix((const WCHAR *)L"enable",param_1,6) == 0) {
                if (PECMD_AdvanceAfterPrefix((const WCHAR *)L"disable",param_1,7) == 0) {
                  if (PECMD_AdvanceAfterPrefix((const WCHAR *)L"wait",param_1,4) == 0) {
                    if (StrCmpNIW((const WCHAR *)L"trans:",(const uint16_t *)*param_1,6) == 0) {
                      *param_1 = *param_1 + 0xc;
                      *(uint16_t *)param_6 = (uint16_t)DAT_14013a24c;
                      *(uint16_t *)((char *)param_6 + 2) = (uint16_t)(DAT_14013a24c >> 0xf);
                      FUN_140074838(param_1,(int *)&param_6);
                      DAT_14013a24c = (int16_t)(uint64_t)param_6;
                    } else {
                      uint16_t *p = (uint16_t *)*param_1;
                      while (*p != 0 && ((*p < 9 || 0xd < *p) && *p != 0x20)) { p = (uint16_t *)*param_1 + 1; *param_1 = (longlong)p; }
                      FUN_1400170b0((void **)param_1);
                    }
                  } else { *puVar5 = 1; }
                } else { *param_4 = 2; }
              } else { *param_4 = 0; }
            } else { *param_3 = *param_3 | 0x40; }
          } else { *param_3 = *param_3 & 0xf0; *param_3 = *param_3 | 0x10; }
        } else { *param_3 = *param_3 & 0xf0; }
      } else { *param_3 = *param_3 & 0xef; *param_3 = *param_3 | 1; }
    } else { *param_2 = 1; }
  }
}
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
int RegisterHotKey(void *w, unsigned int id, unsigned int mods, unsigned int vk) { (void)w;(void)id;(void)mods;(void)vk; return 0; }
void *TlsGetValue(unsigned long idx) { (void)idx; return (void*)0; }
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
/* @0x14006345c size=— ANSI 串追加(直移) */
longlong *PECMD_AppendAnsiStr(longlong *param_1,LPCSTR param_2)
{
  int n1 = 0;
  if ((uintptr_t)param_2 != 0) {
    if (*param_1 != 0) n1 = lstrlenA((const char *)*param_1);
    int n2 = lstrlenA(param_2);
    void *p = (void *)(uintptr_t)PECMD_HeapRealloc((void *)(uintptr_t)*param_1,(long long)(n2 + 1 + n1));
    *param_1 = (longlong)p;
    PECMD_MemMoveForward((uint8_t *)((long long)n1 + (long long)p), (uint8_t *)(uintptr_t)param_2, n2 + 1);
  }
  return param_1;
}
uint64_t PECMD_FindPartitionInfo(void){ return 0; }
uint64_t PECMD_EncodeImageToStream(void){ return 0; }
undefined8 FUN_1400748a0(HWND param_1, undefined8 *param_2);   /* 窗口枚举回调 (leaf stub, 定义见文件尾) */
/* @0x1400e3d60 遍历(子)窗口执行回调 (decompiled.c 直移) */
undefined8
FUN_1400e3d60(LPCWSTR param_1,char param_2,char param_3,HWND param_4,undefined4 param_5,
             undefined4 param_6,undefined8 param_7,undefined8 param_8)
{
  LPCWSTR local_58;
  undefined8 local_50;
  HWND local_48;
  int local_40;
  int local_3c;
  int local_38;
  undefined4 local_34;
  undefined4 local_30;
  undefined8 local_28;
  undefined4 local_20;
  undefined8 local_18;

  if (param_1 == (LPCWSTR)0x0) {
    local_50 = 0;
  }
  else {
    local_3c = lstrlenW(param_1);
    local_20 = 0;
    local_40 = (int)param_2;
    local_38 = (int)param_3;
    local_50 = 0;
    local_34 = param_5;
    local_30 = param_6;
    local_28 = param_7;
    local_18 = param_8;
    local_58 = param_1;
    local_48 = param_4;
    if (param_4 == (HWND)0x0) {
      EnumWindows((WNDENUMPROC)FUN_1400748a0,(LPARAM)&local_58);
    }
    else {
      EnumChildWindows(param_4,(WNDENUMPROC)FUN_1400748a0,(LPARAM)&local_58);
    }
  }
  return local_50;
}
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
/* @0x1400fec9c size=93 — 控件对象Q初始化(直移) */
uint64_t *PECMD_InitControlObjQ(uint64_t *param_1,uint64_t param_2,uint64_t param_3)
{
  PECMD_InitWinObjBase(param_1);
  param_1[0x1a] = param_3;
  *(uint8_t *)(param_1 + 0x14) = 0;
  param_1[0x1b] = 0;
  *(uint32_t *)(param_1 + 0x1c) = 0;
  param_1[0x21] = param_2;
  *param_1 = (uint64_t)&PTR_FUN_14012cf00;
  return param_1;
}
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
/* @0x1400705ac size=— 全局消息分派(直移) */
int64_t PECMD_DispatchGlobalMessage(int64_t *a, const uint16_t *b, uint32_t c, uint64_t d, uint64_t e, uint32_t f, uint32_t g, uint64_t h)
{
  (void)b;(void)c;(void)d;(void)e;(void)f;(void)g;(void)h;
  if ((long long)DAT_14013d3b8 < 2) return 0;
  EnterCriticalSection((void *)&DAT_14013e168);
  FUN_140061c44();
  int r = -3;
  if ((uintptr_t)DAT_14013d800 != 0) {
    r = ((int (*)(uint64_t))(uintptr_t)DAT_14013d800)(0);
    if (r != 0 && r == 1) ((void (*)())(uintptr_t)DAT_14013d808)();
  }
  int64_t v = ((int64_t (*)(void*,uint64_t,uint64_t,uint32_t,uint64_t,uint64_t,uint32_t,uint32_t,uint64_t))(uintptr_t)DAT_14013d3b8)(a + 0x1a,1,(uint64_t)(uintptr_t)b,c,d,e,f,g,h);
  a[4] = v;
  FUN_14006b8fc((void *)a);
  LeaveCriticalSection((void *)&DAT_14013e168);
  return v;
}
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
/* @0x14001d78c size=36 — 重叠安全前向拷贝 memmove 封装(直移) */
uint8_t *PECMD_MemMoveForward(uint8_t *a, uint8_t *b, int n)
{
  if (a != b && n - 1 >= 0) {
    uint8_t *d = a;
    do { *d = d[(long long)b - (long long)a]; d++; n--; } while (n - 1 >= 0);
  }
  return a;
}
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
    PECMD_AllocStrSlot(&local_res10);
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
        PECMD_MemMoveForward((uint8_t *)local_res8, (uint8_t *)L"/Service       ", 0x1e);
        local_res8 = (WCHAR *)FUN_14000531c((uint16_t *)(local_res8 + 0xf));
    }
    else {
        cVar3 = PECMD_MatchTokenAdvance("/Service", (long long *)&local_res8, 8);
        if (cVar3 == '\0') {
            cVar3 = PECMD_MatchTokenAdvance("/RemoveService", (long long *)&local_res8, 0xe);
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
/* @0x1400458a8 size=— 显示 ID 对话框(直移) */
uint64_t PECMD_ShowIdDialog(LPCWSTR param_1)
{
  short *local_28[4]; uint64_t u = 0;
  FUN_1400284d4((long long *)local_28,param_1);
  if (*local_28[0] != 0) {
    PECMD_ShowAboutDialog();
    DAT_14013cfb0 = (void *)local_28;
    u = DialogBoxParamW((void *)DAT_14013ca68,(void *)(uintptr_t)0x2727,0,FUN_14003e220,0);
    DAT_14013cfb0 = 0;
    u = u & 0xffffffff;
  }
  FUN_14005b104((long long *)local_28);
  return u;
}
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
/* @0x14004fea4=PECMD_VectorPushBack — 向量尾插(直移) */
uint64_t PECMD_VectorPushBack(longlong param_1, uint64_t param_2)
{
  *(long long *)(param_1 + 0x48) = *(long long *)(param_1 + 0x48) + 1;
  if (*(uint64_t *)(param_1 + 0x30) <= (*(uint64_t *)(param_1 + 0x48) & 0xffffffff) &&
      (char)PECMD_SetVectorSize((void *)(param_1 + 0x28),*(uint64_t *)(param_1 + 0x48) * 2 + 2,-1) == 0) {
    *(long long *)(param_1 + 0x48) = *(long long *)(param_1 + 0x48) - 1;
    return 0;
  }
  if (*(uint64_t *)(param_1 + 0x48) < *(uint64_t *)(param_1 + 0x30)) {
    *(uint64_t *)(*(long long *)(param_1 + 0x28) + *(uint64_t *)(param_1 + 0x48) * 8) = param_2;
    return 1;
  }
  AtlThrowImpl(-0x7ff8ffa9);
  return 0;
}
uint64_t FUN_14005b7dc(void){ return 0; }
/* @0x14005be68 页表回收/双向链表维护 (decompiled.c 直移, 自包含) */
void PECMD_ReclaimPages(longlong param_1,int param_2)
{
  int iVar1;
  int *piVar2;
  longlong lVar3;
  int iVar4;
  longlong lVar5;

  lVar5 = (longlong)param_2;
  if (*(int *)(param_1 + 0x9430 + lVar5 * 4) != 0x1000) {
    iVar4 = *(int *)(param_1 + 0x502c + lVar5 * 4);
    if (iVar4 == 0x1000) {
      iVar4 = *(int *)(param_1 + 0x1028 + lVar5 * 4);
    }
    else {
      iVar1 = *(int *)(param_1 + 0x1028 + lVar5 * 4);
      lVar3 = (longlong)iVar1;
      if (iVar1 != 0x1000) {
        piVar2 = (int *)(param_1 + 0x502c + lVar3 * 4);
        iVar4 = iVar1;
        if (*piVar2 != 0x1000) {
          do {
            iVar4 = *piVar2;
            lVar3 = (longlong)iVar4;
            piVar2 = (int *)(param_1 + 0x502c + lVar3 * 4);
          } while (*piVar2 != 0x1000);
          *(undefined4 *)(param_1 + 0x502c + (longlong)*(int *)(param_1 + 0x9430 + lVar3 * 4) * 4) =
               *(undefined4 *)(param_1 + 0x1028 + lVar3 * 4);
          *(undefined4 *)(param_1 + 0x9430 + (longlong)*(int *)(param_1 + 0x1028 + lVar3 * 4) * 4) =
               *(undefined4 *)(param_1 + 0x9430 + lVar3 * 4);
          *(undefined4 *)(param_1 + 0x1028 + lVar3 * 4) =
               *(undefined4 *)(param_1 + 0x1028 + lVar5 * 4);
          *(int *)(param_1 + 0x9430 + (longlong)*(int *)(param_1 + 0x1028 + lVar5 * 4) * 4) = iVar4;
        }
        *(undefined4 *)(param_1 + 0x502c + lVar3 * 4) =
             *(undefined4 *)(param_1 + 0x502c + lVar5 * 4);
        *(int *)(param_1 + 0x9430 + (longlong)*(int *)(param_1 + 0x502c + lVar5 * 4) * 4) = iVar4;
      }
    }
    *(undefined4 *)(param_1 + 0x9430 + (longlong)iVar4 * 4) =
         *(undefined4 *)(param_1 + 0x9430 + lVar5 * 4);
    lVar3 = (longlong)*(int *)(param_1 + 0x9430 + lVar5 * 4);
    if (*(int *)(param_1 + 0x502c + lVar3 * 4) == param_2) {
      *(int *)(param_1 + 0x502c + lVar3 * 4) = iVar4;
    }
    else {
      *(int *)(param_1 + 0x1028 + lVar3 * 4) = iVar4;
    }
    *(undefined4 *)(param_1 + 0x9430 + lVar5 * 4) = 0x1000;
  }
}
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

/* @0x140062fc4 size=— token 前缀匹配并前进(直移) */
uint8_t PECMD_MatchTokPrefixAdv(LPCWSTR param_1, longlong *param_2, int param_3)
{
  uint8_t r = 0;
  if (param_3 < 0) param_3 = lstrlenW(param_1);
  const uint16_t *p = (const uint16_t *)*param_2;
  if (StrCmpNIW(param_1,p,param_3) == 0) {
    uint16_t w = p[param_3];
    if (((8 < w && w < 0xe) || w == L' ') || w == 0) {
      *param_2 = *param_2 + (long long)param_3 * 2;
      r = 1;
      PECMD_SkipLeadingControlChars((long long *)param_2);
    }
  }
  return r;
}
/* @0x140060a74 size=31 — 相邻双字节对换(直移) */
void PECMD_SwapBytePairs(uint8_t *param_1,int param_2)
{
  uint8_t *end = param_1 + (long long)param_2 - 1;
  for (; param_1 < end; param_1 += 2) {
    uint8_t u = *param_1; *param_1 = param_1[1]; param_1[1] = u;
  }
}
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
/* @0x14005eefc size=173 — 展开 %% 与 %d(直移) */
uint16_t *PECMD_ExpandPercentD(uint16_t *param_1, WCHAR *param_2)
{
  uint16_t w = *param_2;
  for (;;) {
    if (w == 0) { *param_1 = 0; return param_1; }
    if (w == L'%') {
      if (param_2[1] == L'%') { *param_1 = L'%'; param_1++; }
      else {
        if (param_2[1] != L'd') goto L73;
        param_1 += wsprintfW(param_1,(const uint16_t *)L"%d");
      }
      param_2 += 2;
    } else {
L73:
      *param_1 = w; param_1++; param_2++;
    }
    w = *param_2;
  }
}
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
        PECMD_ScaleQuadByFactor(param_1,local_res20,&param_5,&param_6,&param_7);
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
        PECMD_ScaleQuadByFactor(param_1,local_res20,&param_5,&param_6,&param_7);
        puVar3=(undefined8*)operator_new(0x70);
        if (puVar3!=(undefined8*)0) {
            puVar4=FUN_14005762c(puVar3,param_2,(int)((longlong)puVar2-lVar1>>3)+0x1000,param_3,local_res20[0],param_5,param_6,param_7,param_8,param_9,param_10);
        }
        *puVar2=(undefined8)(uintptr_t)puVar4;
        FUN_14005d9a8(param_1,0);
    }
}

/* @0x140064a88 size=47 — 宽串十进制数字解析(直移) */
long long PECMD_WideStrToInt64(uint16_t *s)
{
  long long v = 0;
  for (; (uint16_t)(*s - 0x30) < 10; s++)
    v = (long long)(int)(*s - 0x30) + v * 10;
  return v;
}
uint64_t PECMD_StreamClose(void) { return 0; }
uint64_t PECMD_ConvertStringEncoding(void) { return 0; }
uint64_t PECMD_DeviceCheckReady(void) { return 0; }
/* @0x1400799f0 size=— 多字节转换(函数指针槽)(直移) */
uint64_t PECMD_MultiByteConvert(uint64_t param_1, uint64_t param_2)
{
  uint64_t local_res10 = 0;
  FUN_140063694(&local_res10,param_2 * 2 + 0x20);
  int n = (int)param_2 + 1;
  int r = ((int (*)(void*,unsigned long,uint64_t,unsigned long,uint64_t,int))DAT_14013c970)((void*)0,8,param_1,(unsigned long)(param_2 & 0xffffffff),(uint64_t)local_res10,n);
  uint64_t out;
  if (r < 1) {
    out = 0xfde9;
    r = ((int (*)(void*,unsigned long,uint64_t,unsigned long,uint64_t,int))DAT_14013c970)((void*)(uintptr_t)0xfde9,8,param_1,(unsigned long)(param_2 & 0xffffffff),(uint64_t)local_res10,n);
    if (r != 0) goto L67;
  }
  out = 0;
L67:
  FUN_14005b104(&local_res10);
  return out;
}
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
        PECMD_AppendWideStr((uint16_t**)&local_res8,pWVar4);
        puVar2=FUN_14006e74c((const WCHAR*)(uintptr_t)local_res8,param_2,param_3);
        FUN_14005b104((longlong*)&local_res8);
    } else {
        puVar2=PECMD_CreateNamedWaitObj(param_1,param_2);
    }
    return (undefined8*)puVar2;
}

uint64_t FUN_1400e4d84(void) { return 0; }
uint64_t PECMD_LoadRasApi(void) { return 0; }
/* @0x1400e4e94 size=71 — 设虚表并关句柄(直移) */
void PECMD_DestroyEventSlots(uint64_t *param_1)
{
  *param_1 = (uint64_t)&PTR_FUN_14012b1c0;
  if (DAT_14013e1f8 != 0) { CloseHandle(DAT_14013e1f8); DAT_14013e1f8 = 0; }
  if (DAT_14013e200 != 0) { CloseHandle(DAT_14013e200); DAT_14013e200 = 0; }
}
uint64_t FUN_1400e5248(void) { return 0; }
uint64_t PECMD_FindRasConnection(void) { return 0; }
uint64_t PECMD_EnumRasConnections(void) { return 0; }
uint64_t PECMD_EnumPhonebookEntries(void) { return 0; }
uint64_t PECMD_ExecSpecialCommand(void) { return 0; }
uint64_t PECMD_FreeCacheBlock(void) { return 0; }
uint64_t PECMD_ShakeTrayCursor(void) { return 0; }
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
  PECMD_AllocStringSlot2((void **)&local_res8,(int64_t)((iVar2 + 1) * 3));
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
uint64_t PECMD_SendTitleMessage(void) { return 0; }
void PECMD_AddControlA(WPARAM param_1, longlong param_2, undefined8 *param_3, int param_4, int param_5, int param_6, int param_7, undefined8 *param_8, undefined8 *param_9, uint param_10, WCHAR *param_11){
    longlong lVar1; undefined8 *puVar2,*puVar3,*puVar4; int local_res20[2];
    local_res20[0]=param_4;
    puVar2=(undefined8*)FUN_140060834(param_1,*param_3);
    puVar4=(undefined8*)0;
    if (puVar2!=(undefined8*)0) {
        lVar1=**(longlong**)(param_1+0x1a0);
        PECMD_ScaleQuadByFactor(param_1,local_res20,&param_5,&param_6,&param_7);
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
        PECMD_ScaleQuadByFactor(param_1,local_res20,&param_5,&param_6,&param_7);
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
        PECMD_ScaleQuadByFactor(param_1,local_res20,&param_5,&param_6,&param_7);
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
        PECMD_ScaleQuadByFactor(param_1,local_res20,&param_5,&param_6,&param_7);
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
        PECMD_ScaleQuadByFactor(param_1,local_res20,&param_5,&param_6,&param_7);
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
int FUN_140074838(longlong *a, int *b) { (void)a;(void)b; return 1; }
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
void *FUN_140063224(uint64_t *a, uint64_t b) { (void)a;(void)b; return 0; }
uint64_t thunk_FUN_1400f429c(void *a, short b) { (void)a;(void)b; return 0; }

/* ============================================================
   ---- core_b3_remaining.c 本轮还原 (FUN_140070710) 新增依赖 ----
   ※ 本批仅添加真正缺失的 helper: FUN_140065864 (真实逻辑, 内绕
     PECMD_VectorSlotPtr 栈求值器). 其余候选函数 (140070da8/084a5c/0987ec/
     0b2ca8/0bb718/0a8a30/0ac094) 因混淆/C++对象/巨型依赖按 SKIP 保留.
   ============================================================ */
/* 取值自 PECMD原始.EXE — FUN_140065864 计算器栈定位: param_1<0 时清 0 并置
 * param_4 低字节 bit3(8); 随后按 PECMD_VectorSlotPtr 定位栈元素并返回其指针(RAX). */
int64_t FUN_140065864(int64_t param_1, int64_t *param_2, int64_t *param_3,
                      uint8_t *param_4, uint32_t param_5)
{
    if (param_1 < 0) {
        param_1 = 0;
        *param_4 = (uint8_t)(*param_4 | 8);
    }
    return PECMD_VectorSlotPtr(param_1, param_2, param_3, param_5);
}

/* ============================================================
   ---- core_b3_remaining.c 本轮还原 (PECMD_ParseWindowCommandOpts 命令串解析) 缺失 token 解析 helper 桩 ----
   字符串 token 切分辅助, 供 PECMD_ParseWindowCommandOpts 解析 "名称,值" 序对使用.
   真实逻辑为: 按分隔符扫描/拷贝子串到容器(00[0]); 此处保留签名与副作用(不推进指针),
   使调用方编译/链接通过并按返回码走控制流.
   ============================================================ */
void FUN_1400675b8(int64_t *param_1, int64_t *param_2, int16_t param_3) { (void)param_1,(void)param_2,(void)param_3; }
void FUN_1400676e4(int64_t *param_1, int64_t *param_2, int16_t param_3) { (void)param_1,(void)param_2,(void)param_3; }
void FUN_140067748(int64_t *param_1, int64_t *param_2, int16_t param_3) { (void)param_1,(void)param_2,(void)param_3; }
uint32_t FUN_1400734e4(int64_t param_1, void *param_2, int64_t *param_3) { (void)param_1,(void)param_2,(void)param_3; return 0; }

/* ============================================================
   ---- core_b3_remaining.c 本轮还原 (PECMD_SetAclPermission 安全描述符/ACL 设置) 缺失
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
/* LocalFree — 安全描述符/ACL 释放 (PECMD_SetAclPermission 使用), 声明于 win32_stub.h, 原无定义 */
uint64_t LocalFree(uint64_t hMem) { (void)hMem; return 0; }

/* ============================================================
   ---- core_b3_remaining.c 本轮还原 (PECMD_ExpressionArithmetic 计算器浮点运算分派) 新增
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
   ---- core_b3_remaining.c 本轮还原 (PECMD_PinStartMenuTask Win10 开始菜单/任务栏 PINT 固定)
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
/* @0x1400607a4 size=— 同驱双路径处理: 硬链接或拷贝(直移) */
void PECMD_HardlinkOrCopy(LPCWSTR param_1,LPCWSTR param_2,int param_3)
{
  if (2 < param_3) {
    if (StrCmpNIW(param_1,param_2,param_3) == 0) goto Ldone;
  }
  if ((*param_1 == *param_2 && param_1[1] == param_2[1]) && param_2[1] == L':') param_3 = 2;
  else param_3 = 0;
Ldone:
  PECMD_CreateSymbolicLink(param_2,(uint64_t)(uintptr_t)(param_1 + param_3),0);
  CreateHardLinkW((uint64_t)param_2,(uint64_t)param_1,0);
  CopyFileW((uint64_t)param_1,(uint64_t)param_2,1);
}
void    *FUN_140017770(void *a, const char *b) { (void)a;(void)b; return (void *)0; }        /* 命名互斥/事件 */
void     FUN_1400177b8(void *a) { (void)a; }                                                 /* 释放互斥/事件 */
uint32_t FUN_14000bfcc(uint64_t a, uint64_t b, const uint16_t *c) { (void)a;(void)b;(void)c; return 0; } /* 执行固定操作 */
/* WinAPI 桩: 资源串读取 / CRT atexit */
uint64_t LoadStringA(void){ return 0; }
int      atexit(void (*fn)(void)) { (void)fn; return 0; }
/* FUN_14005b540 — 去除串内前导/尾随空白 (PECMD_PinStartMenuTask 使用) */
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
/* @0x1400a41fc size=— 命名对象注册表查找/登记(直移) */
void *FUN_1400a41fc(const uint16_t *a)
{
  EnterCriticalSection((void *)&DAT_14013e190);
  longlong base = DAT_14013e118;
  longlong cnt = (longlong)DAT_14013e128;
  uint64_t *found = 0;
  longlong i = 0;
  if (cnt > 0) {
    for (; i < cnt; i++) {
      longlong e = *(longlong *)(base + i * 8);
      if (e != 0) {
        if (lstrcmpW((const uint16_t *)a,*(const uint16_t **)(e + 8)) == 0) {
          int *p = (int *)(*(longlong *)(base + i * 8) + 0x10);
          *p = *p + 1;
          found = *(uint64_t **)(base + i * 8);
          goto Ldone;
        }
      }
    }
  }
  uint64_t *nw = (uint64_t *)(uintptr_t)operator_new(0x18);
  if ((uintptr_t)nw == 0) nw = 0;
  else {
    *nw = 0;
    PECMD_StrDupAssign((uint16_t **)(nw + 1),(const uint16_t *)a);
    *(uint32_t *)(nw + 2) = 1;
    *nw = (uint64_t)FUN_140061ffc(0,0,0);
  }
  longlong j = 0;
  if (cnt > 0) {
    for (; j < cnt; j++) {
      if (*(longlong *)(base + j * 8) == 0) { *(uint64_t **)(base + j * 8) = nw; goto Ldone; }
    }
  }
  { uint64_t *slot = nw;
    PECMD_VectorAppendGen(&DAT_14013e118,&DAT_14013e120,&DAT_14013e128,(uint8_t *)&slot,8,1);
  }
  found = nw;
Ldone:
  LeaveCriticalSection((void *)&DAT_14013e190);
  return found;
}
/* @0x1400a43c4 size=— 命名对象构造(直移) */
void *PECMD_CreateNamedWaitObj(const WCHAR *a, char b)
{
  longlong ls[2];
  PECMD_StrDupAssign((uint16_t **)ls,a);
  uint64_t *p = (uint64_t *)(uintptr_t)operator_new(0x20);
  if ((uintptr_t)p == 0) p = 0;
  else {
    p[1] = 0;
    *(uint32_t *)((long long)p + 0x14) = 0;
    *(uint8_t *)(p + 2) = 0;
    *p = (uint64_t)&PTR_FUN_140128ed0;
    uint64_t *p2 = (uint64_t *)FUN_1400a41fc(a);
    p[3] = (uint64_t)(uintptr_t)p2;
    p[1] = *p2;
  }
  if ((uintptr_t)p != 0 && b != 0) {
    (void)WaitForSingleObject((void *)(uintptr_t)p[1],0xffffffff);
    *(uint8_t *)(p + 2) = 1;
    return p;
  }
  return p;
}
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

/* ---- wave-current: leaf stubs for missing helpers surfaced by restored bodies ---- */
LPCWSTR  PECMD_LoadLocalizedString(void *hinst, unsigned int id, WCHAR *buf, int len) { (void)hinst;(void)id;(void)buf;(void)len; return (LPCWSTR)0; } /* @0x14005b6ac 资源串 */
longlong FUN_140003a20(longlong *param_1, undefined8 *param_2, byte param_3) { (void)param_1;(void)param_2;(void)param_3; return 0; }
undefined8 FUN_1400748a0(HWND param_1, undefined8 *param_2) { (void)param_1;(void)param_2; return 1; } /* 窗口枚举回调 (leaf stub) */

/* @0x1400677b0 解析形如 "@xx,yyy..." 的颜色/坐标增量 (decompiled.c 直移) */
short FUN_1400677b0(longlong *param_1,longlong param_2)
{
  ushort uVar1;
  DWORD DVar2;
  ushort *puVar3;
  longlong lVar4;
  longlong *plVar5;
  ushort *puVar6;
  longlong lVar7;
  short sVar8;
  char cVar9;
  char cVar10;
  ushort *local_res8;

  plVar5 = param_1;
  PECMD_SkipLeadingControlChars((long long *)param_1);
  puVar6 = (ushort *)*plVar5;
  uVar1 = *puVar6;
  while ((uVar1 != 0 && (*(short *)*param_1 != 0x2c))) {
    puVar3 = (ushort *)((short *)*param_1 + 1);
    *param_1 = (longlong)puVar3;
    uVar1 = *puVar3;
  }
  sVar8 = 0;
  while( true ) {
    cVar9 = '\x01';
    cVar10 = *puVar6 == 0x40;
    if ((bool)cVar10) {
      puVar6 = puVar6 + 1;
    }
    if (*puVar6 == 0x2d) {
      cVar9 = -1;
      puVar6 = puVar6 + 1;
    }
    if (((0x2f < *puVar6) && (*puVar6 < 0x3a)) ||
       ((0x60 < (*puVar6 | 0x20) && ((*puVar6 | 0x20) < 0x67)))) {
      if ((*puVar6 == 0x30) && ((puVar6[1] | 0x20) == 0x78)) {
        puVar6 = puVar6 + 2;
      }
      lVar4 = FUN_140064a34(puVar6);
      lVar7 = (longlong)sVar8;
      *(int *)(param_2 + lVar7 * 4) = (int)lVar4;
      if (cVar9 < '\0') {
        *(int *)(param_2 + lVar7 * 4) = -(int)lVar4;
      }
      if (cVar10 != '\0') {
        DVar2 = GetSysColor(*(int *)(param_2 + lVar7 * 4));
        *(DWORD *)(param_2 + lVar7 * 4) = DVar2;
      }
    }
    for (; (puVar6 < (ushort *)*param_1 && (*puVar6 != 0x23)); puVar6 = puVar6 + 1) {
    }
    sVar8 = sVar8 + 1;
    if (*puVar6 != 0x23) break;
    local_res8 = puVar6 + 1;
    PECMD_SkipLeadingControlChars((long long *)&local_res8);
    puVar6 = local_res8;
    if (3 < sVar8) {
      return sVar8;
    }
  }
  return sVar8;
}

/* @0x14006f908 创建 \\\\.\\X 设备并下发控制码 (decompiled.c 直移) */
BOOL FUN_14006f908(char param_1,int param_2)
{
  BOOL BVar1;
  HANDLE hObject;
  DWORD local_res8 [2];
  HANDLE local_res18;
  WCHAR local_28 [4];
  short local_20;
  undefined2 local_1e;
  undefined2 local_1c;
  undefined2 local_1a;
  undefined2 local_18;
  undefined2 local_16;

  memcpy(local_28,L"\\\\.\\",8);
  local_20 = (short)param_1;
  local_1e = 0x3a;
  local_res18 = (HANDLE)0x0;
  local_res8[0] = 0;
  local_1c = 0;
  local_1a = 0;
  local_18 = 0;
  local_16 = 0;
  FUN_140003864(&local_res18,local_28,0xc0000000,3,(LPSECURITY_ATTRIBUTES)0x0,3,0x80,(HANDLE)0x0);
  if (local_res18 == (HANDLE)0x0) {
    FUN_140003864(&local_res18,local_28,0x80000000,3,(LPSECURITY_ATTRIBUTES)0x0,3,0x80,(HANDLE)0x0);
    hObject = local_res18;
    if (local_res18 != (HANDLE)0x0) goto LAB_14006f9e1;
  }
  else {
LAB_14006f9e1:
    hObject = local_res18;
    BVar1 = DeviceIoControl(local_res18,(-(uint)(param_2 != 0) & 0xfffffffc) + 0x2d480c,(LPVOID)0x0,
                            0,(LPVOID)0x0,0,local_res8,(LPOVERLAPPED)0x0);
    if (BVar1 != 0) goto LAB_14006fa1d;
  }
  BVar1 = 0;
LAB_14006fa1d:
  if ((hObject != (HANDLE)0x0) && (hObject != (HANDLE)0xffffffffffffffff)) {
    CloseHandle(hObject);
  }
  return BVar1;
}

extern int DAT_14013d7fc;
undefined4 PECMD_WindowRectHitTest(HWND a, POINT b);
void      *PECMD_FindWindowByPid(longlong a);
void       FUN_1400f172c(longlong *a, UINT b, ulonglong c, ulonglong *d, longlong e, int f, undefined8 *g);
void       FUN_1400613fc(void);
/* @0x1400e91f0 模态对话框消息泵 (decompiled.c 直移, CONCAT44/复合字节访问已手工展开) */
undefined4 PECMD_ModalDialogPump(longlong *param_1,ulonglong param_2)
{
  longlong *plVar1;
  longlong lVar2;
  POINT pt;
  bool bVar3;
  LONG LVar4;
  int iVar5;
  uint uVar6;
  DWORD DVar7;
  BOOL BVar8;
  HWND pHVar9;
  ulonglong uVar10;
  uint uVar11;
  int iVar12;
  int iVar13;
  longlong lVar14;
  longlong lVar15;
  byte bVar16;
  tagPOINT local_res8;
  byte local_res10;
  DWORD local_res18 [2];
  tagRECT local_98;
  POINT local_88;
  UINT local_80;
  tagMSG local_68;

  bVar16 = *(byte *)(param_1 + 0x24);
  *(undefined1 *)(param_1 + 0x24) = 0x10;
  local_res10 = bVar16;
  if ((param_2 & 0x20) == 0) {
    UpdateWindow((HWND)param_1[4]);
  }
  local_68.hwnd = (HWND)0x0;
  memset(&local_68.message,0,0x28);
  local_res8.x = (LONG)(DWORD)GetCurrentThreadId();
  if ((param_2 & 0x20) == 0) {
    SendMessageW((HWND)param_1[4],0x110,0,(LPARAM)param_1);
  }
  iVar5 = GetMessageW(&local_68,(HWND)0x0,0,0);
  LVar4 = local_res8.x;
  do {
    if ((iVar5 == 0) || (bVar16 = local_res10, (char)param_1[0x24] < '\0')) {
      if (((HWND)param_1[4] != (HWND)0x0) && ((bVar16 & 0x20) == 0)) {
        SetWindowLongPtrW((HWND)param_1[4],-4,*(LONG_PTR *)(*param_1 + 8));
        DestroyWindow((HWND)param_1[4]);
      }
      return (undefined4)local_68.lParam;
    }
    uVar10 = 0;
    bVar3 = false;
    local_res18[0] = 0;
    if ((local_68.message == 0x20a) || (local_68.message == 0x20e)) {
      GetScrollPos((HWND)0x0,1);
      local_80 = local_68.message;
      GetCursorPos(&local_88);
      uVar6 = (uint)GetWindowLongW(local_68.hwnd,-0x10);
      uVar11 = 0x100000;
      if (local_68.message == 0x20a) {
        uVar11 = 0x200000;
      }
      if (((uVar11 & uVar6) == 0) || (iVar5 = PECMD_WindowRectHitTest(local_68.hwnd,local_88), iVar5 == 0)) {
        pHVar9 = (HWND)PECMD_FindWindowByPid((longlong)&local_88);
        if (pHVar9 != (HWND)0x0) {
          local_68.hwnd = pHVar9;
        }
      }
      else {
        uVar10 = 1;
      }
    }
    pHVar9 = GetActiveWindow();
    if (((pHVar9 != (HWND)0x0) &&
        (DVar7 = GetWindowThreadProcessId(pHVar9,local_res18), LVar4 == DVar7)) &&
       (BVar8 = IsDialogMessageW(pHVar9,&local_68), BVar8 != 0)) {
      bVar3 = true;
    }
    if (local_68.message != 0x448) {
      uVar10 = SendMessageW(local_68.hwnd,0x448,(WPARAM)&local_68,param_1[4]);
      if (((uint)uVar10 >> 0x10 & 1) != 0) {
        FUN_1400f172c((longlong *)param_1[0x40],local_68.message,
                      (ulonglong)local_68.wParam,(ulonglong *)local_68.lParam,
                      param_1[4],0x80,(undefined8 *)0x0);
      }
      if (0 < (int)param_1[0x1d]) {
        local_98.left = 0;
        local_98.top = 0;
        local_98.right = 0;
        local_98.bottom = 0;
        GetCursorPos(&local_res8);
        iVar5 = 0;
        if (0 < (int)param_1[0x1d]) {
          lVar14 = 0;
          do {
            pHVar9 = *(HWND *)(lVar14 + param_1[0x1c]);
            if ((pHVar9 != (HWND)0x0) && (pHVar9 != local_68.hwnd)) {
              GetWindowRect(pHVar9,&local_98);
              pt.y = local_res8.y;
              pt.x = local_res8.x;
              BVar8 = PtInRect(&local_98,pt);
              if (BVar8 != 0) {
                SendMessageW(pHVar9,0x44e,(WPARAM)&local_68,param_1[4]);
              }
            }
            iVar5 = iVar5 + 1;
            lVar14 = lVar14 + 8;
          } while (iVar5 < (int)param_1[0x1d]);
        }
      }
      iVar5 = (int)local_68.wParam;
      if ((HWND)param_1[4] != local_68.hwnd) {
        if (((uint)uVar10 >> 8 & 1) == 0) {
          uVar6 = ((uint (*)(void *, void *))(uintptr_t)(**(code **)(*param_1 + 0xe0)))(param_1,&local_68);
          uVar10 = (ulonglong)uVar6;
        }
        else if ((0 < *(int *)((longlong)param_1 + 0xec)) &&
                ((local_68.message == 0x100 || (local_68.message == 0x104)))) {
          plVar1 = (longlong *)param_1[0x37];
          iVar13 = 0;
          if (0 < *(int *)((longlong)param_1 + 0xec)) {
            lVar14 = *plVar1;
            lVar15 = 0;
            iVar12 = DAT_14013d7fc;
            do {
              lVar2 = *(longlong *)(lVar15 + lVar14);
              if (((*(int *)(lVar2 + 8) == 10) && (*(int *)(lVar2 + 0x70) == iVar5)) &&
                 (*(int *)(lVar2 + 0x78) == iVar12)) {
                FUN_1400613fc();
                lVar14 = *plVar1;
                iVar12 = DAT_14013d7fc;
                if (*(int *)(*(longlong *)(lVar15 + lVar14) + 0x78) == DAT_14013d7fc) {
                  PostMessageW(*(HWND *)(*(longlong *)(param_1[0x52] + 0x40) + 0x20),0x442,
                               (longlong)iVar13,(longlong)iVar13);
                  break;
                }
              }
              iVar13 = iVar13 + 1;
              lVar15 = lVar15 + 8;
            } while (iVar13 < *(int *)((longlong)param_1 + 0xec));
          }
        }
      }
    }
    if ((!bVar3) && ((uVar10 & 1) == 0)) {
      TranslateMessage(&local_68);
      DispatchMessageW(&local_68);
    }
    iVar5 = GetMessageW(&local_68,(HWND)0x0,0,0);
    bVar16 = local_res10;
  } while( true );
}

/* ---- wave-current p4 restore support globals + leaf stubs ---- */
int DAT_14013d7fc = 0x22a6e;   /* 0x14013d7fc 原始: 6e 2a 02 00 (字符串表/虚拟键偏移表基址) */
/* @0x1400e6314 size=59 — 窗口矩形命中判定(直移) */
undefined4 PECMD_WindowRectHitTest(HWND a, POINT b)
{
  RECT r; memset(&r,0,4*4);
  GetWindowRect((void *)a,&r);
  return PtInRect(&r,b);
}
/* @0x1400e6458 size=49 — 枚举窗口找当前进程首个命中(直移) */
void *PECMD_FindWindowByPid(longlong param_1)
{
  *(uint64_t *)(param_1 + 0x18) = 0;
  *(uint32_t *)(param_1 + 0x10) = GetCurrentProcessId();
  EnumWindows((WNDENUMPROC)PECMD_EnumWindowFindProc,(LPARAM)param_1);
  return (void *)(uintptr_t)*(uint64_t *)(param_1 + 0x18);
}
void       FUN_1400f172c(longlong *a, UINT b, ulonglong c, ulonglong *d, longlong e, int f, undefined8 *g) { (void)a;(void)b;(void)c;(void)d;(void)e;(void)f;(void)g; } /* 消息派发 (leaf stub) */
void       FUN_1400613fc(void) { }                                            /* 键状态刷新 (leaf stub) */

/* ---- wave-current support: e7758 deps ---- */
void (*DAT_14013e290)(void *, void *, int *) = 0;   /* 0x14013e290 原始: 48 b8 05 00 86 b8 05 00 (RTL_UTF8... 文本转储回调表) */
void  FUN_140063720(longlong **_ps, longlong len) { (void)_ps;(void)len; }   /* 字符串缓冲扩展 (leaf stub) */

/* @0x1400e7758 产出一段以 \r\n 分隔的标注文本 (decompiled.c 直移) */
longlong FUN_1400e7758(longlong *param_1)
{
  undefined2 *puVar1;
  longlong lVar2;
  int iVar3;
  undefined2 *puVar4;
  int iVar5;
  int iVar6;
  int local_res8 [2];
  undefined4 local_res10 [2];
  undefined4 *local_res18;

  PECMD_AllocStringSlot2((void **)&local_res18,0x21e30);
  *local_res18 = 0x56c;
  local_res8[0] = 0;
  local_res10[0] = 0x21e30;
  (*DAT_14013e290)(local_res18,local_res10,local_res8);
  iVar5 = 0;
  iVar6 = 0;
  if (0 < local_res8[0]) {
    do {
      iVar3 = lstrlenW((LPCWSTR)(local_res18 + (longlong)iVar6 * 0x15b + 3));
      FUN_140063720((longlong **)param_1,(longlong)(iVar3 + 3 + iVar5));
      puVar1 = (undefined2 *)(*param_1 + (longlong)iVar5 * 2);
      puVar4 = puVar1;
      if (iVar6 != 0) {
        *puVar1 = 0xd;
        puVar4 = puVar1 + 2;
        puVar1[1] = 10;
        iVar5 = iVar5 + 2;
      }
      PECMD_MemMoveForward((undefined1 *)puVar4,(undefined1 *)(local_res18 + (longlong)iVar6 * 0x15b + 3),
                    (iVar3 + 1) * 2);
      iVar6 = iVar6 + 1;
      iVar5 = iVar5 + iVar3;
    } while (iVar6 < local_res8[0]);
  }
  lVar2 = *param_1;
  FUN_14005b104((longlong *)&local_res18);
  return lVar2;
}

/* ---- wave-current support: e4f14/e5120 deps ---- */
code *DAT_14013e238;   /* 0x14013e238 UxTheme!IsAppThemed 槽 (pe_data_extract) */
/* @0x14005c898 size=— 延迟加载 GetProcAddress 包装(直移) */
void PECMD_LazyLoadProc(const char *name, const char *dll, longlong **slot, longlong *x)
{
  longlong ls[2]; ls[0]=0;
  if (*slot != 0) return;
  longlong *p = ls;
  if ((uintptr_t)x != 0) p = x;
  void *hm = (void *)(uintptr_t)*p;
  if ((uintptr_t)hm == 0) {
    hm = (void *)(uintptr_t)LoadLibraryA(dll);
    *p = (longlong)(uintptr_t)hm;
    if ((uintptr_t)hm == 0) { return; }
  }
  void *fn = (void *)(uintptr_t)GetProcAddress(hm,name);
  if ((uintptr_t)fn != 0) *slot = (longlong *)(uintptr_t)fn;
}

void *FUN_1400b3d0c(const unsigned short *a, longlong *b, longlong c, longlong d, unsigned int *e, undefined8 *f, int g) { (void)a;(void)b;(void)c;(void)d;(void)e;(void)f;(void)g; return (void*)0; } /* 位图绘制 (leaf stub) */
void  FUN_1400f2384(longlong a, const unsigned short *b, longlong *c, longlong d, int e, int f) { (void)a;(void)b;(void)c;(void)d;(void)e;(void)f; } /* 命令注册 (leaf stub) */

/* @0x1400e4f14 创建菜单项及可选图标位图 (decompiled.c 直移) */
void PECMD_MenuItemWithIcon(HMENU param_1,UINT param_2,UINT_PTR param_3,byte *param_4)
{
  int iVar1;
  ulonglong uVar2;
  HBITMAP hBitmapUnchecked;
  byte bVar3;
  int iVar4;
  int local_res20 [2];
  int local_48 [2];
  LPCWSTR local_40;
  longlong local_38 [2];

  if ((*param_4 & 8) != 0) {
    return;
  }
  AppendMenuW(param_1,param_2,param_3,*(LPCWSTR *)(param_4 + 8));
  local_40 = *(LPCWSTR *)(param_4 + 0x30);
  iVar4 = 0;
  if (*local_40 == L'\0') {
    return;
  }
  local_38[0] = 0;
  local_48[0] = 0;
  local_res20[0] = 0;
  for (; *local_40 == L'?'; local_40 = local_40 + 1) {
    iVar4 = iVar4 + 1;
  }
  if (*local_40 == L'*') {
    while (local_40 = local_40 + 1, *local_40 == L'?') {
      iVar4 = iVar4 + 1;
    }
    FUN_140067d20((long long *)&local_40,local_48);
    local_res20[0] = local_48[0];
    if (*local_40 == L':') {
      local_40 = local_40 + 1;
      FUN_140067d20((long long *)&local_40,local_res20);
    }
    if ((*local_40 == L'*') || (*local_40 == L':')) {
      local_40 = local_40 + 1;
    }
  }
  if (iVar4 != 0) {
    uVar2 = FUN_14005ea5c();
    if (0x5ffff < (uint)(uVar2 >> 0x10)) {
      if (DAT_14013e238 == (code *)0x0) {
        PECMD_LazyLoadProc("IsAppThemed","UxTheme.dll",(longlong **)&DAT_14013e238,(longlong *)0x0);
      }
      bVar3 = 0;
      if ((code *)(DAT_14013e238 + -1) < (code *)0xfffffffffffffffe) {
        iVar1 = ((int (*)(void))(uintptr_t)(DAT_14013e238))();
        bVar3 = -(iVar1 != 0) & 0x10U | 1;
      }
      if ((bVar3 == 0x11) && (iVar4 != 2)) {
        local_48[0] = 0;
        local_res20[0] = 0;
        goto LAB_1400e50bb;
      }
    }
    if (local_48[0] == 0) {
      local_48[0] = GetSystemMetrics(0x47);
    }
    if (local_res20[0] == 0) {
      local_res20[0] = GetSystemMetrics(0x48);
    }
  }
LAB_1400e50bb:
  local_48[1] = 0;
  hBitmapUnchecked =
       FUN_1400b3d0c(local_40,local_38,(longlong)local_48[0],(longlong)local_res20[0],
                     (uint *)(local_48 + 1),(undefined8 *)0x0,0);
  *(HBITMAP *)(param_4 + 0x38) = hBitmapUnchecked;
  if (hBitmapUnchecked != (HBITMAP)0x0) {
    SetMenuItemBitmaps(param_1,(UINT)param_3,0,hBitmapUnchecked,hBitmapUnchecked);
  }
}

/* @0x1400e5120 添加菜单项并注册命令 (decompiled.c 直移) */
void PECMD_MenuRegisterCommand(byte *param_1,ushort *param_2,HMENU param_3,longlong param_4,longlong param_5,
                  longlong *param_6)
{
  byte bVar1;
  ushort uVar2;
  uint uVar3;
  LPWSTR local_res8;

  bVar1 = *param_1;
  uVar3 = 0;
  if ((bVar1 & 1) != 0) {
    uVar3 = 3;
  }
  if ((bVar1 & 4) != 0) {
    uVar3 = uVar3 | 8;
  }
  if ((bVar1 & 2) != 0) {
    uVar3 = uVar3 | 0x800;
  }
  if ((bVar1 & 0x20) != 0) {
    uVar3 = uVar3 | 0x20;
  }
  uVar2 = *param_2;
  *(ushort *)(param_1 + 2) = uVar2;
  PECMD_MenuItemWithIcon(param_3,uVar3,(ulonglong)uVar2,param_1);
  *(HMENU *)(param_1 + 0x20) = param_3;
  *param_2 = *param_2 + 1;
  FUN_140063694(&local_res8,100);
  wsprintfW(local_res8,(const unsigned short *)L"&%s.id");
  if (param_6 != (longlong *)0x0) {
    FUN_1400669c4(param_6,(ulonglong)*(ushort *)(param_1 + 2),local_res8);
  }
  if ((param_4 != 0) && (**(short **)(param_1 + 0x10) != 0)) {
    wsprintfW(local_res8,(const unsigned short *)L"_COMMAND#%d:");
    PECMD_AppendWideStr((uint16_t **)&local_res8,*(LPCWSTR *)(param_1 + 0x10));
    FUN_1400f2384(param_5,local_res8,param_6,param_4,0,1);
  }
  FUN_14005b104((longlong *)&local_res8);
}

/* ---- wave-current support: 018220/018c6c/01b3a0/01b888 deps ---- */
undefined8 FUN_14005c788(char *a, ushort *b, int c) { (void)a;(void)b;(void)c; return 0; }   /* 模块名前缀判定 (leaf stub) */
void  FUN_140018148(HMODULE a, LPCSTR b) { (void)a;(void)b; }                              /* 重定向 thunk: GetProcAddress (leaf stub) */
void  FUN_140018178(HMODULE a, LPWSTR b, DWORD c) { (void)a;(void)b;(void)c; }             /* 重定向 thunk: GetModuleFileNameW (leaf stub) */
HMODULE FUN_1400181b8(LPCWSTR a) { (void)a; return (HMODULE)0; }                          /* 重定向 thunk: GetModuleHandleW (leaf stub) */
void  FUN_1400185c8(char a, longlong *b) { (void)a;(void)b; }                             /* 输出缓冲刷新 (leaf stub) */
void  FUN_140017908(void) { }                                                             /* 提权检查 (leaf stub) */
DWORD FUN_1400717f0(HKEY a, LPCWSTR b) { (void)a;(void)b; return 0; }                     /* 注册表分支清理 (leaf stub) */
int   _vsnwprintf(wchar_t *s, size_t n, const wchar_t *fmt, void *ap) { (void)s;(void)n;(void)fmt;(void)ap; return 0; }
unsigned long DAT_14013c934 = 0;         /* 0x14013c934 TLS 槽索引: 00 00 00 00 */
longlong DAT_14013cb18[8] = {0};         /* 0x14013cb18 TLS 输出缓冲 (pe_data_extract: bc 4e 00 00 0c 50 13 00) */
uint8_t  DAT_14013cb41 = 0;              /* 0x14013cb41 强制刷新标志 */
uint8_t  DAT_14013d261 = 0;              /* 0x14013d261 RAMDATA 初始化标志 */

/* @0x140018220 延迟绑定导入表 (decompiled.c 直移) */
undefined8 PECMD_BindDelayImports(longlong param_1,longlong param_2)
{
  ulonglong *puVar1;
  uint uVar2;
  uint uVar3;
  bool bVar4;
  WCHAR WVar5;
  int iVar6;
  HMODULE hModule;
  undefined8 uVar7;
  code *pcVar8;
  ulonglong uVar9;
  longlong lVar10;
  uint *puVar11;
  LPCSTR lpString2;
  longlong lVar12;
  ulonglong *puVar13;
  WCHAR local_248 [8];
  short local_238;

  uVar9 = (ulonglong)*(uint *)(*(longlong *)(param_1 + 0x28) + 0x90);
  if (uVar9 != 0) {
    puVar11 = (uint *)(uVar9 + param_2);
    while( true ) {
      lVar12 = 0;
      if (*puVar11 == 0) break;
      uVar2 = puVar11[4];
      uVar3 = puVar11[3];
      puVar13 = (ulonglong *)((ulonglong)*puVar11 + param_2);
      local_248[0] = L'\0';
      lVar10 = lVar12;
      do {
        WVar5 = (WCHAR)*(byte *)(lVar10 + (ulonglong)uVar3 + param_2);
        local_248[lVar10] = WVar5;
        if (WVar5 == L'\0') break;
        lVar10 = lVar10 + 1;
      } while (lVar10 < 0x104);
      hModule = GetModuleHandleW(local_248);
      if ((hModule == (HMODULE)0x0) && (hModule = LoadLibraryW(local_248), hModule == (HMODULE)0x0))
      {
        return 0;
      }
      uVar7 = FUN_14005c788("KERNEL32",(ushort *)local_248,8);
      bVar4 = false;
      if (((char)uVar7 != '\0') && ((local_238 == 0 || (local_238 == 0x2e)))) {
        bVar4 = true;
      }
      uVar9 = *puVar13;
      puVar1 = puVar13;
      lVar10 = lVar12;
      while (uVar9 != 0) {
        uVar9 = *puVar1;
        if ((uVar9 & 0x8000000000000000) == 0) {
          lpString2 = (LPCSTR)(uVar9 + 2 + param_2);
          if (!bVar4) goto LAB_140018390;
          iVar6 = lstrcmpiA("GetProcAddress",lpString2);
          if (iVar6 == 0) {
            pcVar8 = (code *)FUN_140018148;
          }
          else {
            iVar6 = lstrcmpiA("GetModuleFileNameW",lpString2);
            if (iVar6 == 0) {
              pcVar8 = (code *)FUN_140018178;
            }
            else {
              iVar6 = lstrcmpiA("GetModuleHandleW",lpString2);
              if (iVar6 != 0) goto LAB_140018390;
              pcVar8 = (code *)FUN_1400181b8;
            }
          }
          if (pcVar8 == (FARPROC)0x0) goto LAB_140018390;
        }
        else {
          lpString2 = (LPCSTR)(uVar9 & 0xffff);
LAB_140018390:
          pcVar8 = (code *)GetProcAddress(hModule,lpString2);
          if (pcVar8 == (FARPROC)0x0) {
            return 0;
          }
        }
        lVar12 = lVar12 + 1;
        *(code **)(lVar10 + (ulonglong)uVar2 + param_2) = pcVar8;
        lVar10 = lVar12 * 8;
        puVar1 = puVar13 + lVar12;
        uVar9 = *puVar1;
      }
      puVar11 = puVar11 + 5;
    }
  }
  return 1;
}

/* @0x140018c6c 格式化写入 TLS 输出缓冲 (decompiled.c 直移) */
void PECMD_DebugScriptString(undefined8 param_1,wchar_t *param_2,undefined8 param_3,undefined8 param_4)
{
  LPWSTR lpBuffer;
  int iVar1;
  longlong *plVar2;
  longlong *plVar3;
  undefined8 local_res18;
  undefined8 local_res20;

  local_res18 = param_3;
  local_res20 = param_4;
  plVar2 = (longlong *)TlsGetValue(DAT_14013c934);
  plVar3 = &DAT_14013cb18[0];
  if ((plVar2 != (longlong *)0x0) && (*plVar2 != 0)) {
    plVar3 = plVar2;
  }
  if (*plVar3 != 0) {
    EnterCriticalSection((LPCRITICAL_SECTION)&DAT_14013e190);
    if (*plVar3 != 0) {
      if ((int)plVar3[3] - (int)plVar3[2] < 0xc96) {
        plVar3[3] = plVar3[2] + 0x10c96;
        FUN_140063720((longlong **)(plVar3 + 1),plVar3[2] + 0x10c96);
      }
      lpBuffer = (LPWSTR)(plVar3[1] + 0x1000 + plVar3[2] * 2);
      *lpBuffer = L'\0';
      if ((uintptr_t)param_2 < 0x10000) {
        LoadStringW(DAT_14013ca68,(UINT)(uintptr_t)param_2,lpBuffer,0x400);
        param_2 = (wchar_t *)lpBuffer;
      }
      iVar1 = _vsnwprintf((wchar_t *)(plVar3[1] + plVar3[2] * 2),0x7fd,param_2,(void *)&local_res18);
      if (iVar1 < 0) {
        iVar1 = 0;
      }
      plVar3[2] = plVar3[2] + (longlong)iVar1;
      *(undefined2 *)(plVar3[1] + plVar3[2] * 2) = 0;
      if ((0x10000 < plVar3[2]) || (DAT_14013cb41 != '\0')) {
        FUN_1400185c8('\x01',plVar3);
      }
    }
    LeaveCriticalSection((LPCRITICAL_SECTION)&DAT_14013e190);
  }
}

/* @0x14001b3a0 遍历窗口链并按条件回收 (decompiled.c 直移) */
void PECMD_ResetScriptChain(longlong *param_1,longlong *param_2)
{
  longlong lVar1;
  longlong lVar2;
  longlong lVar3;
  longlong *plVar4;
  int iVar5;
  longlong local_res8;

  if (param_1[8] == 0) {
    PECMD_AllocStringSlot2((void **)&local_res8,0x3ffff0);
    iVar5 = 0;
    plVar4 = param_1;
    if (param_2 != (longlong *)0x0) {
      lVar2 = 0;
      plVar4 = param_2;
      do {
        if (plVar4 == param_1) break;
        *(longlong **)(local_res8 + lVar2 * 8) = plVar4;
        plVar4 = (longlong *)plVar4[7];
        lVar2 = lVar2 + 1;
        iVar5 = iVar5 + 1;
      } while (lVar2 < 0x7fffe);
    }
    lVar2 = (longlong)iVar5;
    while ((lVar2 < 0x7fffe &&
           (((*(byte *)((longlong)plVar4 + 0xd9) & 1) == 0 ||
            ((((*(byte *)((longlong)plVar4 + 0xd9) & 0x48) == 0 && (plVar4[8] == 0)) &&
             (*(longlong *)(plVar4[7] + 0x40) == 0))))))) {
      *(longlong **)(local_res8 + lVar2 * 8) = plVar4;
      plVar4 = (longlong *)plVar4[7];
      iVar5 = iVar5 + 1;
      lVar2 = lVar2 + 1;
    }
    if ((*(byte *)((longlong)plVar4 + 0xd9) & 1) != 0) {
      if (plVar4[8] == 0) {
        PECMD_GetWinIdLocked(plVar4);
      }
      lVar2 = plVar4[8];
      if ((lVar2 != 0) && (lVar3 = (longlong)(iVar5 + -1), -1 < iVar5 + -1)) {
        do {
          lVar1 = *(longlong *)(local_res8 + lVar3 * 8);
          if (*(longlong *)(lVar1 + 0x40) == 0) {
            *(longlong *)(lVar1 + 0x40) = lVar2;
          }
          lVar3 = lVar3 + -1;
        } while (-1 < lVar3);
      }
    }
    FUN_14005b104(&local_res8);
  }
}

/* @0x14001b888 初始化 PELOGON RAMDATA 注册表分支 (decompiled.c 直移) */
void PECMD_InitRamdataRegistry(uint param_1)
{
  long LVar1;
  HKEY local_res10;

  if ((DAT_14013d261 == '\0') || (param_1 != 0)) {
    local_res10 = (HKEY)0x0;
    FUN_140017908();
    if ((param_1 & 1) != 0) {
      local_res10 = (HKEY)0x0;
      LVar1 = RegCreateKeyExW((HKEY)0xffffffff80000002,(const WCHAR *)L"SOFTWARE\\PELOGON\\RAMDATA\\__PECMD_TEST",0
                              ,(LPWSTR)0x0,0,1,(LPSECURITY_ATTRIBUTES)0x0,&local_res10,(LPDWORD)0x0);
      if (local_res10 != (HKEY)0x0) {
        RegCloseKey(local_res10);
      }
      if (LVar1 != 0x3fd) {
        FUN_1400717f0((HKEY)0xffffffff80000002,(const WCHAR *)L"SOFTWARE\\PELOGON\\RAMDATA");
      }
    }
    local_res10 = (HKEY)0x0;
    RegCreateKeyExW((HKEY)0xffffffff80000002,(const WCHAR *)L"SOFTWARE\\PELOGON\\RAMDATA",0,(LPWSTR)0x0,1,1,
                    (LPSECURITY_ATTRIBUTES)0x0,&local_res10,(LPDWORD)0x0);
    if (local_res10 != (HKEY)0x0) {
      RegCloseKey(local_res10);
    }
    DAT_14013d261 = '\x01';
    if ((param_1 & 2) != 0) {
      local_res10 = (HKEY)0x0;
      LVar1 = RegCreateKeyExW((HKEY)0xffffffff80000002,
                              (const WCHAR *)L"SOFTWARE\\PELOGON\\RAMDATA\\HOTKEY\\__PECMD_TEST",0,(LPWSTR)0x0,0,1,
                              (LPSECURITY_ATTRIBUTES)0x0,&local_res10,(LPDWORD)0x0);
      if (local_res10 != (HKEY)0x0) {
        RegCloseKey(local_res10);
      }
      if (LVar1 != 0x3fd) {
        FUN_1400717f0((HKEY)0xffffffff80000002,(const WCHAR *)L"SOFTWARE\\PELOGON\\RAMDATA\\HOTKEY");
      }
    }
    local_res10 = (HKEY)0x0;
    RegCreateKeyExW((HKEY)0xffffffff80000002,(const WCHAR *)L"SOFTWARE\\PELOGON\\RAMDATA\\HOTKEY",0,(LPWSTR)0x0,1,1
                    ,(LPSECURITY_ATTRIBUTES)0x0,&local_res10,(LPDWORD)0x0);
    if (local_res10 != (HKEY)0x0) {
      RegCloseKey(local_res10);
    }
  }
}

/* ---- wave-current support: 0545f8 ---- */
ushort *FUN_140024c48(longlong *a, longlong *b, uint c) { (void)a;(void)b;(void)c; return (ushort*)0; } /* 转义串解析 (leaf stub) */

/* @0x1400545f8 从串中切出一个(可能带引号)的字段 (decompiled.c 直移) */
WCHAR * PECMD_TokenizeQuotedField(longlong *param_1,longlong *param_2,longlong *param_3,WCHAR param_4,
                     ushort param_5)
{
  short sVar1;
  WCHAR *pWVar2;
  ushort *puVar3;
  LPCWSTR pWVar4;
  LPCWSTR pWVar5;
  short *psVar6;
  LPCWSTR pWVar7;
  longlong lVar8;
  short *psVar9;
  longlong local_res10;
  LPCWSTR local_res18 [2];

  PECMD_SkipLeadingControlChars((long long *)param_2);
  pWVar7 = (LPCWSTR)*param_2;
  if (*pWVar7 == L'\"') {
    pWVar4 = pWVar7;
    if ((param_5 & 1) != 0) {
LAB_1400546f8:
      do {
        pWVar5 = pWVar4;
        pWVar4 = pWVar5 + 1;
        *param_2 = (longlong)pWVar4;
        if (*pWVar4 == L'\0') goto LAB_140054709;
      } while (*pWVar4 != L'\"');
      pWVar5 = pWVar5 + 2;
      do {
        *param_2 = (longlong)pWVar5;
LAB_140054709:
        pWVar4 = (LPCWSTR)*param_2;
        if (*pWVar4 == L'\0') {
LAB_140054724:
          lVar8 = (longlong)pWVar4 - (longlong)pWVar7 >> 1;
          goto LAB_14005472d;
        }
        if (*pWVar4 == L'\"') goto LAB_1400546f8;
        if (param_4 == *pWVar4) goto LAB_140054724;
        pWVar5 = pWVar4 + 1;
      } while( true );
    }
    local_res10 = 0;
    local_res18[0] = pWVar7;
    puVar3 = FUN_140024c48((longlong *)local_res18,&local_res10,0x85);
    *param_2 = (longlong)puVar3;
    pWVar7 = local_res18[0];
    lVar8 = local_res10;
LAB_14005472d:
    FUN_140063888(param_3,pWVar7,lVar8);
  }
  else if ((param_5 & 0x1000) == 0) {
    FUN_14006764c(param_2,param_3,param_4,0);
  }
  else {
    if (*pWVar7 != L'\0') {
      while (pWVar2 = (WCHAR *)*param_2, param_4 != *pWVar2) {
        if ((((8 < (ushort)*pWVar2) && ((ushort)*pWVar2 < 0xe)) || (*pWVar2 == L' ')) ||
           (*param_2 = (longlong)(pWVar2 + 1), pWVar2[1] == L'\0')) break;
      }
    }
    FUN_140063888(param_3,pWVar7,*param_2 - (longlong)pWVar7 >> 1);
    PECMD_SkipLeadingControlChars((long long *)param_2);
  }
  pWVar2 = (WCHAR *)*param_2;
  if ((param_5 & 8) == 0) {
    if ((((param_5 & 4) != 0) || (*pWVar2 == L'\0')) || (*pWVar2 != param_4)) goto LAB_140054760;
  }
  else if (*pWVar2 == L'\0') goto LAB_140054760;
  *param_2 = (longlong)(pWVar2 + 1);
LAB_140054760:
  FUN_140003a20(param_1,param_3,1);
  psVar6 = (short *)*param_3;
  if (((param_5 & 2) == 0) && (*psVar6 == 0x22)) {
    psVar9 = psVar6 + 1;
    do {
      *psVar6 = 0;
      while( true ) {
        sVar1 = *psVar9;
        psVar9 = psVar9 + 1;
        if (sVar1 == 0x22) break;
        *psVar6 = sVar1;
        psVar6 = psVar6 + 1;
        if (sVar1 == 0) {
          return pWVar2;
        }
      }
    } while( true );
  }
  return pWVar2;
}


/* ---- wave-current support: 008834 ---- */
int StartServiceW(void *svc, unsigned long argc, const unsigned short **argv) { (void)svc;(void)argc;(void)argv; return 0; }
/* @0x14001d744 size=— 双向安全串移动(直移) */
uint8_t *PECMD_MemMoveSafe(void *a, longlong b, longlong c)
{
  uint8_t *d = (uint8_t *)a;
  int diff = (int)d - (int)b;
  if (diff < 0) {
    int n = (int)c - 1;
    if (n >= 0) {
      uint8_t *p = d;
      do { *p = p[b - (long long)d]; p++; n--; } while (n >= 0);
    }
  } else if (diff > 0) {
    int n = (int)c - 1;
    uint8_t *p = d + c;
    if (n >= 0) {
      do { p--; *p = p[b - (long long)d]; n--; } while (n >= 0);
    }
  }
  return d;
}

LPCWSTR DAT_14013ca10 = 0;   /* 0x14013ca10 服务名槽 */

/* @0x140008834 安装/启动 Windows 服务 (decompiled.c 直移) */
void PECMD_InstallWindowsService(LPCWSTR param_1)
{
  WCHAR WVar1;
  bool bVar2;
  int iVar3;
  int iVar4;
  DWORD DVar5;
  longlong lVar6;
  SC_HANDLE hSCManager;
  SC_HANDLE pSVar7;
  LPCWSTR lpString;
  uint uVar8;
  LPCWSTR pWVar9;
  bool bVar10;
  LPWSTR local_res8;
  LPCWSTR local_res10;
  LPCWSTR local_res18;
  WCHAR local_858 [1040];

  bVar10 = false;
  DAT_14013ca10 = param_1;
  lpString = param_1;
  if (*param_1 == L'~') {
    DAT_14013ca10 = param_1 + 1;
    bVar10 = *DAT_14013ca10 == L'I';
    param_1 = param_1 + 2;
    lpString = param_1;
  }
  for (; (WVar1 = *param_1, WVar1 != L'\0' &&
         ((((ushort)WVar1 < 9 || (0xd < (ushort)WVar1)) && (WVar1 != L' ')))); param_1 = param_1 + 1
      ) {
  }
  iVar4 = (int)((longlong)param_1 - (longlong)lpString >> 1);
  if (0x400 < iVar4) {
    iVar4 = 0x400;
  }
  local_res10 = param_1;
  PECMD_SkipLeadingControlChars((long long *)&local_res10);
  lVar6 = FUN_14005c72c("--gui-",(ushort *)local_res10,6);
  uVar8 = 0x100;
  if ((char)lVar6 != '\0') {
    local_res18 = local_res10 + 6;
    PECMD_SkipLeadingControlChars((long long *)&local_res18);
    iVar3 = lstrlenW(local_res18);
    PECMD_MemMoveSafe((void *)local_res10,(longlong)local_res18,(iVar3 + 1) * 2);
    uVar8 = 0;
  }
  PECMD_MemMoveForward((undefined1 *)local_858,(undefined1 *)lpString,iVar4 * 2);
  local_858[iVar4] = L'\0';
  DAT_140147000 = 0;
  iVar4 = lstrlenW(lpString);
  FUN_140063694(&local_res8,(longlong)(iVar4 + 0x227));
  DVar5 = GetModuleFileNameW((HMODULE)0x0,local_res8 + 1,0x1cc);
  if (0 < (int)DVar5) {
    bVar2 = false;
    local_res8[(longlong)(int)DVar5 + 1] = L'\0';
    *local_res8 = L'\"';
    pWVar9 = local_res8;
    while (pWVar9 = pWVar9 + 1, pWVar9 < local_res8 + (longlong)(int)DVar5 + 1) {
      if (((8 < (ushort)*pWVar9) && ((ushort)*pWVar9 < 0xe)) || (*pWVar9 == L' ')) {
        bVar2 = true;
      }
    }
    pWVar9 = local_res8 + 1;
    if (bVar2) {
      local_res8[(longlong)(int)DVar5 + 1] = L'\"';
      DVar5 = DVar5 + 2;
      pWVar9 = local_res8;
    }
    PECMD_MemMoveForward((undefined1 *)(pWVar9 + (int)DVar5),(undefined1 *)L" EXEC /Service ",0x1e);
    PECMD_MemMoveForward((undefined1 *)(pWVar9 + (int)DVar5 + 0xf),(undefined1 *)lpString,(iVar4 + 1) * 2);
    hSCManager = OpenSCManagerW((LPCWSTR)0x0,(LPCWSTR)0x0,0xf003f);
    if (((hSCManager != (SC_HANDLE)0x0) &&
        (pSVar7 = CreateServiceW(hSCManager,local_858,local_858,0xf01ff,uVar8 | 0x10,2,0,pWVar9,
                                 (LPCWSTR)0x0,(LPDWORD)0x0,(LPCWSTR)0x0,(LPCWSTR)0x0,(LPCWSTR)0x0),
        pSVar7 != (SC_HANDLE)0x0)) && (bVar10)) {
      CloseServiceHandle(pSVar7);
      pSVar7 = OpenServiceW(hSCManager,local_858,0x20000000);
      if (pSVar7 != (SC_HANDLE)0x0) {
        StartServiceW(pSVar7,0,(LPCWSTR *)0x0);
        CloseServiceHandle(pSVar7);
      }
    }
    CloseServiceHandle(hSCManager);
  }
  FUN_14005b104((longlong *)&local_res8);
}


/* ---- wave-current support: 022e94 ---- */
unsigned int FUN_140022e24(void) { return 0; }        /* 热键序号表初始化 (leaf stub) */
void  FUN_1400679dc(undefined8 *a, int *b, short c) { (void)a;(void)b;(void)c; }                  /* 整数值解析 (leaf stub) */
int (*DAT_14013cd48)(longlong, const unsigned short *, void *, void *, void *, void *) = 0;  /* 0x14013cd48 热键枚举回调槽 */
HWND  DAT_14013cf78 = 0;            /* 0x14013cf78 主窗 HWND 槽 (pe_data_extract: 04 10 13 00 ...) */
ushort DAT_14013cf80 = 0;           /* 0x14013cf80 热键计数槽 (pe_data_extract: 1e 74 01 00 ...=0x1741e) */

/* @0x140022e94 注册表热键安装循环 (decompiled.c 直移) */
void PECMD_RegisterHotkeyEntry(void)
{
  wchar_t *pwVar1;
  int iVar2;
  int iVar3;
  void *in_R9;
  uint id;
  uint local_res10 [2];
  UINT local_res18 [2];
  undefined4 local_res20 [2];
  undefined1 local_48 [8];
  wchar_t *local_40;
  wchar_t *local_38 [2];

  EnterCriticalSection((LPCRITICAL_SECTION)&DAT_14013e190);
  FUN_140063694(local_38,0x1851);
  local_40 = (wchar_t *)0x0;
  id = 1;
  pwVar1 = local_38[0] + 0x50;
  while (1 < DAT_14013cf80) {
    DAT_14013cf80 = DAT_14013cf80 + -1;
    UnregisterHotKey(DAT_14013cf78,(int)DAT_14013cf80);
  }
  DAT_14013cf80 = 0;
  iVar2 = FUN_140022e24();
  iVar3 = 0;
  if (iVar2 == 0) {
    do {
      *(undefined4 *)local_48 = 1;
      local_res20[0] = 0x3000;
      local_res18[0] = 0;
      local_res10[0] = 0;
      PECMD_CrtShim(local_38[0],0x140120458,(void *)(ulonglong)id,in_R9);
      *pwVar1 = L'\0';
      in_R9 = (void *)local_48;
      local_40 = pwVar1;
      iVar2 = (*DAT_14013cd48)(0xffffffff80000002,(const unsigned short *)L"SOFTWARE\\PELOGON\\RAMDATA\\HOTKEY",
                               local_38[0],in_R9,pwVar1,local_res20);
      if ((iVar2 == 0) && (*local_40 != L'\0')) {
        FUN_1400679dc((undefined8 *)&local_40,(int *)local_res18,0x2c);
        FUN_1400679dc((undefined8 *)&local_40,(int *)local_res10,0x2c);
        in_R9 = (void *)(ulonglong)local_res10[0];
        iVar3 = 0;
        if ((0 < (int)local_res10[0]) && (iVar3 = 0, *local_40 != L'\0')) {
          RegisterHotKey(DAT_14013cf78,id,local_res18[0],local_res10[0]);
          DAT_14013cf80 = (short)id + 1;
        }
      }
      else {
        iVar3 = iVar3 + 1;
        if (0x13 < iVar3) break;
      }
      id = id + 1;
    } while ((int)id < 10000);
    PECMD_RegSetValueWithOpen((void *)0xffffffff80000002,(const unsigned short *)L"SOFTWARE\\PELOGON",
                  (const unsigned short *)(uintptr_t)PTR_u_CallBackhWnd_14011e668_2_14013a288,0xb,(BYTE *)&DAT_14013cf78,8);
  }
  FUN_14005b104((long long *)local_38);
  LeaveCriticalSection((LPCRITICAL_SECTION)&DAT_14013e190);
}


/* ---- wave-current support: 0612cc ---- */
void FUN_14001c2cc(const unsigned short *a, int b, int c) { (void)a;(void)b;(void)c; }  /* 特权启用 (leaf stub) */

/* @0x1400612cc 关机/消息框确认工作项 (decompiled.c 直移) */
undefined8 FUN_1400612cc(longlong *param_1)
{
  int iVar1;
  LPCWSTR lpText;
  LPCWSTR lpCaption;
  longlong lVar2;
  int BVar3;
  uint uVar4;
  undefined8 uVar5;
  int iVar6;
  longlong *local_res8;

  if (*param_1 < 1) {
    uVar5 = 0x80070057;
  }
  else {
    lVar2 = param_1[1];
    lpText = (LPCWSTR)param_1[2];
    lpCaption = (LPCWSTR)param_1[3];
    uVar4 = *(uint *)(param_1 + 4);
    *(undefined4 *)((longlong)param_1 + 0x2c) = 0;
    local_res8 = param_1;
    if ((HANDLE)param_1[7] != (HANDLE)0x0) {
      SetEvent((HANDLE)param_1[7]);
    }
    SetLastError(0);
    if ((int)lVar2 == 2) {
      FUN_14001c2cc((const unsigned short *)L"SeShutdownPrivilege",2,0);
      BVar3 = InitiateSystemShutdownW((void *)0x0,lpText,0x12cc02f6,0,1);
      *(int *)((longlong)param_1 + 0x34) = BVar3;
    }
    else {
      uVar4 = (uint)MessageBoxW((HWND)0x0,lpText,lpCaption,uVar4 | 0x200000);
      *(uint *)((longlong)param_1 + 0x34) = uVar4 & 0xffff;
    }
    GetLastError();
    iVar1 = (int)param_1[6];
    iVar6 = 1000;
    while ((iVar1 != 0 && (iVar6 = iVar6 + -1, -1 < iVar6))) {
      Sleep(10);
      iVar1 = (int)param_1[6];
    }
    EnterCriticalSection((LPCRITICAL_SECTION)&DAT_14013e190);
    *param_1 = *param_1 + -1;
    if (0 < *param_1) {
      local_res8 = (longlong *)0x0;
    }
    LeaveCriticalSection((LPCRITICAL_SECTION)&DAT_14013e190);
    FUN_14005b104((long long *)&local_res8);
    uVar5 = 0;
  }
  return uVar5;
}

/* @0x1400690c0 读取注册表二进制值并保证 NUL 结尾 (decompiled.c 直移) */
int PECMD_ReadRegBinaryGuarded(HKEY param_1,LPCWSTR param_2,LPCWSTR param_3,longlong *param_4,LPDWORD param_5,
                 LSTATUS *param_6)
{
  longlong lVar1;
  long LVar2;
  longlong lVar3;
  int iVar4;
  DWORD local_28 [2];
  HKEY local_20;

  local_28[0] = 0xffffffff;
  local_20 = (HKEY)0x0;
  iVar4 = 0;
  LVar2 = RegOpenKeyExW(param_1,(const unsigned short *)param_2,0,0x20019,&local_20);
  if (LVar2 == 0) {
    *param_5 = 0xffa;
    FUN_140063424(param_4,0x1000);
    *(undefined1 *)*param_4 = 0;
    LVar2 = RegQueryValueExW(local_20,(const unsigned short *)param_3,(LPDWORD)0x0,local_28,(BYTE *)*param_4,param_5);
    if (LVar2 == 0xea) {
      *param_5 = *param_5 + 10;
      FUN_140063424(param_4,(longlong)(int)(*param_5 + 6));
      LVar2 = RegQueryValueExW(local_20,(const unsigned short *)param_3,(LPDWORD)0x0,local_28,(BYTE *)*param_4,param_5);
    }
    RegCloseKey(local_20);
    iVar4 = -1;
    if (LVar2 == 0) {
      if ((int)*param_5 < 0) {
        *param_5 = 0;
      }
      *(undefined1 *)((longlong)(int)*param_5 + *param_4) = 0;
      lVar3 = (longlong)(int)((longlong)(int)*param_5 + 1U >> 1);
      lVar1 = *param_4;
      *(undefined2 *)(lVar1 + 2 + lVar3 * 2) = 0;
      *(undefined2 *)(lVar1 + lVar3 * 2) = 0;
      FUN_140063424(param_4,(longlong)(int)(*param_5 + 5));
      return local_28[0];
    }
  }
  *param_5 = 0;
  if (param_6 != (LSTATUS *)0x0) {
    *param_6 = LVar2;
  }
  return iVar4 + -1;
}


/* ---- wave-current support: 079cf8/078c68 ---- */
int FUN_140072924(uint param_1, LPCWSTR param_2, uint param_3, longlong *param_4, ushort *param_5) { (void)param_1;(void)param_2;(void)param_3;(void)param_4;(void)param_5; return 0; }  /* SetupDi 枚举 (leaf stub) */
int FUN_1400664dc(undefined8 param_1, int param_2) { (void)param_1;(void)param_2; return 0; }  /* 设备重扫描 (leaf stub) */
DWORD FUN_1400195f0(undefined8 param_1, longlong param_2, int param_3, undefined8 *param_4) { (void)param_1;(void)param_2;(void)param_3;(void)param_4; return 0; }  /* 提示/日志 (leaf stub) */

/* @0x140079cf8 解析以引号/分隔的 8 段数字 (decompiled.c 直移, 复合字节访问已展开) */
ulonglong FUN_140079cf8(undefined8 *param_1,undefined2 *param_2,char param_3)
{
  ushort uVar1;
  longlong lVar2;
  ulonglong uVar3;
  ushort *puVar4;
  int iVar5;
  ulonglong uVar6;
  undefined2 local_18;
  undefined8 local_16;
  undefined4 local_e;
  undefined2 local_a;

  puVar4 = (ushort *)*param_1;
  uVar1 = *puVar4;
  if ((uVar1 == 0x22) || (uVar1 == 0x27)) {
    puVar4 = puVar4 + 1;
    *param_1 = (undefined8)(uintptr_t)puVar4;
    thunk_FUN_1400f429c(param_1,uVar1);
  }
  uVar3 = 0;
  local_16 = 0;
  local_e = 0;
  local_a = 0;
  local_18 = 0;
  uVar6 = uVar3;
  do {
    iVar5 = (int)uVar6;
    if (9 < (ushort)(*puVar4 - 0x30)) break;
    lVar2 = PECMD_WideStrToInt64(puVar4);
    ((ushort *)&local_18)[uVar3] = (ushort)lVar2;
    for (; (ushort)(*puVar4 - 0x30) < 10; puVar4 = puVar4 + 1) {
    }
    if (*puVar4 != 0) {
      puVar4 = puVar4 + 1;
    }
    uVar3 = uVar3 + 1;
    uVar6 = (ulonglong)(iVar5 + 1);
  } while ((longlong)uVar3 < 8);
  *param_2 = local_18;
  param_2[1] = (undefined2)local_16;
  param_2[3] = (undefined2)((ulonglong)local_16 >> 16);
  if (param_3 == '\0') {
    param_2[2] = (undefined2)((ulonglong)local_16 >> 32);
    param_2[4] = (undefined2)((ulonglong)local_16 >> 48);
    param_2[5] = (undefined2)local_e;
    param_2[6] = (undefined2)((uint)local_e >> 16);
    param_2[7] = local_a;
  }
  else {
    param_2[4] = (undefined2)((ulonglong)local_16 >> 32);
    param_2[5] = (undefined2)((ulonglong)local_16 >> 48);
    param_2[6] = (undefined2)local_e;
    param_2[7] = (undefined2)((uint)local_e >> 16);
    param_2[2] = local_a;
  }
  return uVar6;
}

/* @0x140078c68 按 PnP 实例 ID 枚举并提交设备重扫描 (decompiled.c 直移) */
int FUN_140078c68(LPCSTR param_1,int param_2,uint param_3)
{
  LPCWSTR pWVar1;
  int iVar2;
  LPCWSTR local_res20;
  LPCWSTR local_28 [2];

  PECMD_AllocStrSlot((uint16_t **)local_28);
  local_res20 = (LPCWSTR)0x0;
  iVar2 = 0;
  if (-1 < param_2) {
    iVar2 = param_2;
  }
  if ((param_3 & 2) == 0) {
    if (*param_1 != '\0') {
      thunk_FUN_140072814((void *)param_1,(longlong *)local_28,"PnpInstanceID");
    }
    FUN_14006355c((longlong *)&local_res20,local_28[0],-1,0xffffffffffffffffULL);
    pWVar1 = local_res20;
    if ((char)*local_res20 != '\0') {
      iVar2 = FUN_140072924((uint)(iVar2 != 0),local_res20,param_3 & 1,(longlong *)0x0,(ushort *)0x0);
      if (param_2 < 0) {
        FUN_1400195f0((undefined8)(uintptr_t)&DAT_14013d130,0x32,0,(undefined8 *)0x0);
        FUN_140072924(1,pWVar1,param_3 & 1,(longlong *)0x0,(ushort *)0x0);
      }
      goto LAB_140078d61;
    }
  }
  iVar2 = FUN_1400664dc(0xffffffff,iVar2);
  if (param_2 < 0) {
    FUN_1400664dc(0xffffffff,1);
  }
LAB_140078d61:
  FUN_14005b104((longlong *)&local_res20);
  FUN_14005b104((longlong *)local_28);
  return iVar2;
}


/* ---- wave-current support: 07d0ac/07e01c ---- */
/* @0x14005d534 size=34 — 非空名则设置环境变量(直移) */
int PECMD_SetEnvIfNonEmpty(undefined8 param_1, LPCWSTR param_2, LPCWSTR param_3)
{
  (void)param_1;
  if (*param_2 == 0) return 0;
  return SetEnvironmentVariableW(param_2,param_3);
}
void FUN_14001e6bc(longlong *param_1, LPCWSTR param_2, LPCWSTR param_3, longlong param_4) { (void)param_1;(void)param_2;(void)param_3;(void)param_4; }  /* 内联命令执行 (leaf stub) */

/* @0x14007d0ac 预处理 &/&& 命令前缀后入队执行 (decompiled.c 直移) */
void FUN_14007d0ac(longlong *param_1,LPCWSTR param_2,LPCWSTR param_3)
{
  WCHAR WVar1;
  short sVar2;
  LPCWSTR pWVar3;
  LPCWSTR local_res10;

  sVar2 = 0;
  if ((*param_2 == L'#') && (param_2[1] == L'&')) {
    param_2 = param_2 + 1;
    sVar2 = 0x23;
  }
  if (((*param_2 != L'&') && ((*(byte *)((longlong)param_1 + 0xd) & 0xf) == 0)) &&
     ((param_1[8] == 0 || (**(short **)(param_1[8] + 0x128) != 0x26)))) {
    if (*param_2 == L'\0') {
      return;
    }
    PECMD_SetEnvIfNonEmpty((undefined8)(uintptr_t)param_1,param_2,param_3);
    return;
  }
  local_res10 = (LPCWSTR)0x0;
  if (sVar2 == 0) {
    if (*param_2 == L'&') {
      WVar1 = param_2[1];
      goto LAB_14007d145;
    }
    pWVar3 = (LPCWSTR)L"&&";
  }
  else {
    WVar1 = *param_2;
LAB_14007d145:
    if (WVar1 == L'&') goto LAB_14007d16a;
    pWVar3 = (LPCWSTR)L"&";
  }
  FUN_1400703e4((long long *)&local_res10,(const WCHAR *)pWVar3);
  PECMD_AppendWideStr((uint16_t **)&local_res10,param_2);
  param_2 = local_res10;
LAB_14007d16a:
  if ((longlong *)param_1[0x18] != (longlong *)0x0) {
    param_1 = (longlong *)param_1[0x18];
  }
  EnterCriticalSection((LPCRITICAL_SECTION)&DAT_14013e190);
  FUN_14001e6bc(param_1,param_2,param_3,-1);
  LeaveCriticalSection((LPCRITICAL_SECTION)&DAT_14013e190);
  FUN_14005b104((longlong *)&local_res10);
}

/* @0x14007e01c 更新窗口标题/位置并触达消息 (decompiled.c 直移) */
void FUN_14007e01c(longlong param_1,uint param_2,LPWSTR param_3)
{
  longlong lVar1;
  HWND hWnd;
  int bVar2;
  int iVar3;
  LPWSTR pWVar4;
  int nCmdShow;
  LPWSTR local_res8;
  int local_res10 [2];
  WCHAR local_58 [24];

  if (param_2 != 0xf8000009) {
    *(uint *)(param_1 + 0x70) = param_2;
  }
  lVar1 = *(longlong *)(param_1 + 0x38);
  nCmdShow = 0;
  hWnd = *(HWND *)(lVar1 + 0x20);
  if (param_3 != (LPWSTR)0x0) {
    if (*param_3 == L'#') {
      pWVar4 = StrChrW(param_3,L':');
      if (pWVar4 != (LPWSTR)0x0) {
        local_res8 = param_3 + 1;
        *pWVar4 = L'\0';
        local_res10[0] = -0x80000000;
        param_3 = pWVar4 + 1;
        bVar2 = FUN_140074838((longlong *)&local_res8,local_res10);
        if (bVar2 != 0) {
          iVar3 = local_res10[0];
          if (local_res10[0] < 0) {
            iVar3 = -0x80000000;
          }
          *(int *)(lVar1 + 0xe0) = iVar3;
        }
      }
    }
    FUN_1400703e4((long long *)(lVar1 + 0xd8),(const WCHAR *)param_3);
  }
  if (-1 < *(int *)(param_1 + 0x70)) {
    nCmdShow = 5;
  }
  ShowWindow(hWnd,nCmdShow);
  if (param_2 < 0x65) {
    SendMessageW(hWnd,0x402,(longlong)*(int *)(param_1 + 0x70),0);
  }
  if (param_3 != (LPCWSTR)0x0) {
    InvalidateRect(hWnd,(RECT *)0x0,1);
  }
  wsprintfW(local_58,(const unsigned short *)L"%ld");
  if (**(LPCWSTR *)(param_1 + 0x10) != L'\0') {
    FUN_14007d0ac(*(longlong **)(param_1 + 0x50),*(LPCWSTR *)(param_1 + 0x10),local_58);
  }
}


/* ---- wave-current support: 08293c/072d8c ---- */
longlong FUN_140072cc0(void) { return (longlong)0; }   /* 适配器链头 (leaf stub) */
longlong *FUN_1400637dc(longlong *a, LPCSTR b, ulonglong c, ulonglong d) { (void)a;(void)b;(void)c;(void)d; return (longlong*)0; } /* 串替换 (leaf stub) */
longlong DAT_14013d4d8 = 0;   /* 0x14013d4d8 GetAdaptersInfo 槽 (初 0) */

/* @0x14008293c 取一个参数并处理 % 引用 (decompiled.c 直移) */
void PECMD_TokenWithRef(longlong *param_1,longlong *param_2,longlong *param_3,longlong *param_4)
{
  ushort *puVar1;
  LPWSTR pWVar2;
  longlong lVar3;
  ushort *local_res10;

  lVar3 = 0;
  *param_3 = *param_2;
  puVar1 = (ushort *)*param_2;
  if (*puVar1 == 0x22) {
    *param_2 = (longlong)(uintptr_t)(puVar1 + 1);
    *param_3 = (longlong)(uintptr_t)(puVar1 + 1);
    thunk_FUN_1400f429c(param_2,0x22);
    if (*(short *)*param_2 != 0) {
      *(short *)*param_2 = 0;
      *param_2 = *param_2 + 2;
    }
  }
  else {
    do {
      while ((*param_2 = (longlong)puVar1, *puVar1 != 0 &&
             (((puVar1 = (ushort *)*param_2, *puVar1 < 9 || (0xd < *puVar1)) && (*puVar1 != 0x20))))
            ) {
        puVar1 = puVar1 + 1;
      }
      local_res10 = (ushort *)*param_2;
      PECMD_SkipLeadingControlChars((long long *)&local_res10);
      if (lVar3 == 0) {
        lVar3 = *param_2;
      }
    } while ((*local_res10 != 0) && (puVar1 = local_res10, *local_res10 != 0x2d));
    if (*(short *)*param_2 == 0) {
      *param_2 = lVar3;
    }
  }
  if (*(short *)*param_2 != 0) {
    *(short *)*param_2 = 0;
    *param_2 = *param_2 + 2;
  }
  PECMD_SkipLeadingControlChars((long long *)param_2);
  pWVar2 = StrChrW((LPCWSTR)*param_3,L'%');
  if (pWVar2 != (LPWSTR)0x0) {
    PECMD_ExpandVarDispatch(param_1,(WCHAR *)*param_3,param_4,0,1);
    *param_3 = *param_4;
  }
}

/* @0x140072d8c 查找适配器并回填 MAC/IP (decompiled.c 直移) */
undefined4 PECMD_AdapterMacIpLookup(undefined1 *param_1,longlong *param_2,int param_3)
{
  int bVar1;
  undefined4 uVar2;
  int iVar3;
  longlong *plVar4;
  longlong lVar5;
  longlong local_res20;

  FUN_14005c828("GetAdaptersInfo","Iphlpapi.DLL",&DAT_14013d4d8,(longlong *)0x0);
  if (DAT_14013d4d8 == 0) {
    uVar2 = 0;
  }
  else {
    local_res20 = 0;
    uVar2 = 0xffffffff;
    plVar4 = (longlong *)FUN_140072cc0();
    if (plVar4 != (longlong *)0x0) {
      lVar5 = 6;
      do {
        if (param_3 == 0) {
          if (*(int *)((longlong)plVar4 + 0x194) == (int)lVar5) {
            iVar3 = PECMD_AnsiStrNCompare((char *)(plVar4 + 0x33),(longlong)param_1,lVar5);
            if (iVar3 == 0) {
              FUN_1400637dc(param_2,(LPCSTR)((longlong)plVar4 + 0xc),0xffffffffffffffffULL,
                            0xffffffffffffffffULL);
              goto LAB_140072e1f;
            }
          }
        }
        else {
          bVar1 = FUN_14005ceec((ushort *)*param_2,(char *)((longlong)plVar4 + 0xc));
          if (bVar1 != 0) {
            PECMD_MemMoveForward(param_1,(undefined1 *)(plVar4 + 0x33),(int)lVar5);
LAB_140072e1f:
            uVar2 = *(undefined4 *)((longlong)plVar4 + 0x1a4);
            goto LAB_140072e5b;
          }
        }
        plVar4 = (longlong *)*plVar4;
      } while (plVar4 != (longlong *)0x0);
    }
    if (param_3 == 0) {
      *(undefined2 *)*param_2 = 0;
    }
LAB_140072e5b:
    FUN_14005b104(&local_res20);
  }
  return uVar2;
}


/* ---- wave-current support: 0745c8 ---- */
LPWSTR  FUN_1400bf358(longlong *a, pthreadmbcinfo b, undefined8 *c) { (void)a;(void)b;(void)c; return (LPWSTR)0; } /* 括号内表达式求值 (leaf stub) */

/* @0x1400745c8 剥离外层括号后求值 (decompiled.c 直移; pthreadmbcinfo->refcount 用偏移 0 的 short 访问) */
ulonglong PECMD_EvalParenStripped(longlong *param_1,ulonglong *param_2)
{
  int iVar2;
  LPWSTR pWVar3;
  ulonglong uVar4;
  longlong *plVar5;
  short *psVar6;
  undefined2 *puVar7;
  undefined8 local_28 [2];

  plVar5 = param_1;
  PECMD_SkipLeadingControlChars((long long *)param_1);
  puVar7 = (undefined2 *)0x0;
  if (*(short *)(void *)*plVar5 == 0x28) {
    *(undefined2 *)(void *)*plVar5 = 0x23;
    *plVar5 = *plVar5 + 2;
    psVar6 = (short *)*plVar5;
    iVar2 = 1;
    while (*psVar6 != 0) {
      if (*psVar6 == 0x28) {
        iVar2 = iVar2 + 1;
      }
      else if (*psVar6 == 0x29) {
        iVar2 = iVar2 + -1;
        if (iVar2 < 1) {
          puVar7 = (undefined2 *)*param_1;
          *param_1 = (longlong)(uintptr_t)(puVar7 + 1);
          *puVar7 = 0;
          break;
        }
      }
      psVar6 = psVar6 + 1;
      *param_1 = (longlong)(uintptr_t)psVar6;
    }
    pWVar3 = FUN_1400bf358((longlong *)&DAT_14013d130,(pthreadmbcinfo)*plVar5,local_28);
    *param_2 = (ulonglong)(uintptr_t)pWVar3;
    *(undefined2 *)(void *)*plVar5 = 0x28;
    if (puVar7 != (undefined2 *)0x0) {
      *puVar7 = 0x29;
    }
    PECMD_SkipLeadingControlChars((long long *)param_1);
    uVar4 = 1;
  }
  else {
    uVar4 = PECMD_ParseSizeAndSkipWs(plVar5,param_2);
  }
  return uVar4;
}


/* ---- wave-current support: 0731d8 ---- */
/* @0x1400e9048 size=— 对象槽 0x2a 系列初始化(直移) */
uint64_t *PECMD_InitObjSlot2A(uint64_t *param_1, uint param_2, uint64_t param_3)
{
  PECMD_InitWinObjBase(param_1);
  *param_1 = (uint64_t)&PTR_FUN_14012b430;
  param_1[0x1b] = 0;
  *(uint32_t *)((long long)param_1 + 0xec) = 0;
  *(uint32_t *)(param_1 + 0x1f) = 0;
  *(uint32_t *)((long long)param_1 + 0xfc) = 0;
  param_1[0x20] = (uint64_t)param_2;
  param_1[0x21] = 0; param_1[0x22] = 0;
  param_1[0x23] = param_3;
  *(uint8_t *)(param_1 + 0x24) = 0;
  *(uint8_t *)((long long)param_1 + 0x121) = 0;
  *(uint8_t *)((long long)param_1 + 0x122) = 0;
  *(uint8_t *)((long long)param_1 + 0x123) = 0;
  (void)PECMD_AllocStrSlot((uint16_t **)(param_1 + 0x25));
  param_1[0x2a] = 0;
  *(uint8_t *)(param_1 + 0x2b) = 0;
  *(uint32_t *)(param_1 + 0x29) = 0x80000000;
  *(uint32_t *)((long long)param_1 + 0x144) = 0x80000000;
  *(uint32_t *)(param_1 + 0x28) = 0x80000000;
  *(uint32_t *)((long long)param_1 + 0x13c) = 0x80000000;
  param_1[0x26] = 0;
  *(uint32_t *)(param_1 + 0x27) = 0;
  param_1[0x1c] = 0;
  *(uint32_t *)(param_1 + 0x1d) = 0;
  *(uint8_t *)((long long)param_1 + 0x159) = 0;
  *(uint8_t *)((long long)param_1 + 0x15a) = 0;
  *(uint8_t *)((long long)param_1 + 0xd4) = 0;
  param_1[0x1e] = 0;
  return param_1;
}
undefined1 *FUN_14005b848(undefined1 *a) { (void)a; return a; }                                       /* 串清零 (leaf stub) */
longlong *FUN_140070398(longlong *a, undefined8 *b) { (void)a;(void)b; return (longlong*)0; }        /* 缓冲复制 (leaf stub) */
void *DAT_14013cf70 = 0;    /* 0x14013cf70 HINSTANCE 槽 */
uint8_t DAT_14011d308[8] = {0};   /* 0x14011d308 缺省串 */

/* @0x1400731d8 控件对象构造/字段初始化 (decompiled.c 直移) */
undefined8 * PECMD_InitControlObjField(undefined8 *param_1,undefined8 param_2,uint param_3,undefined8 *param_4)
{
  undefined8 *puVar1;
  void *pHVar2;
  longlong local_res8;

  PECMD_InitObjSlot2A(param_1,param_3,param_2);
  *param_1 = (undefined8)(uintptr_t)&PTR_FUN_140126b20;
  PECMD_AllocStrSlot((uint16_t **)(param_1 + 0x2c));
  PECMD_AllocStrSlot((uint16_t **)(param_1 + 0x2d));
  PECMD_AllocStrSlot((uint16_t **)(param_1 + 0x2e));
  param_1[0x3b] = 0;
  param_1[0x3c] = 0;
  param_1[0x34] = (undefined8)(uintptr_t)(param_1 + 0x32);
  param_1[0x35] = (undefined8)(uintptr_t)(param_1 + 0x33);
  param_1[0x37] = (undefined8)(uintptr_t)(param_1 + 0x36);
  param_1[0x38] = (undefined8)(uintptr_t)((undefined8 *)((longlong)param_1 + 0xec));
  param_1[0x3d] = 0;
  param_1[0x3e] = 0;
  *(undefined4 *)(param_1 + 0x3f) = 0;
  puVar1 = param_1 + 0x3d;
  if (param_4 != (undefined8 *)0x0) {
    puVar1 = param_4;
  }
  param_1[0x40] = (undefined8)(uintptr_t)puVar1;
  PECMD_AllocStrSlot((uint16_t **)(param_1 + 0x44));
  param_1[0x4b] = 0;
  PECMD_AllocStrSlot((uint16_t **)(param_1 + 0x4c));
  *(undefined4 *)((longlong)param_1 + 0x274) = 0;
  *(undefined4 *)(param_1 + 0x4f) = 0;
  *(undefined4 *)((longlong)param_1 + 0x27c) = 0;
  *(undefined4 *)(param_1 + 0x50) = 0;
  param_1[0x51] = 0;
  param_1[0x54] = (undefined8)(uintptr_t)&DAT_14011c638;
  param_1[0x52] = 0;
  FUN_14005b848((undefined1 *)(param_1 + 0x56));
  FUN_14005b848((undefined1 *)(param_1 + 0xd2));
  param_1[0x26] = 0;
  *(undefined2 *)(param_1 + 0x43) = 0xc000;
  *(undefined4 *)(param_1 + 0x27) = 0;
  param_1[0x1c] = 0;
  *(undefined4 *)(param_1 + 0x1d) = 0;
  *(undefined1 *)((longlong)param_1 + 0x61) = 0x19;
  pHVar2 = LoadIconW(DAT_14013cf70,(const unsigned short *)(uintptr_t)0x1);
  param_1[0x41] = 0;
  *(undefined4 *)((longlong)param_1 + 0x214) = 0;
  param_1[0x151] = (undefined8)(uintptr_t)pHVar2;
  *(undefined4 *)(param_1 + 0x42) = 0;
  param_1[0x46] = 0;
  param_1[0x45] = 0;
  *(undefined1 *)((longlong)param_1 + 0x123) = 1;
  *(undefined4 *)(param_1 + 0x29) = 0x80000000;
  *(undefined4 *)((longlong)param_1 + 0x144) = 0x80000000;
  *(undefined4 *)(param_1 + 0x28) = 0x80000000;
  *(undefined4 *)((longlong)param_1 + 0x13c) = 0x80000000;
  *(undefined4 *)(param_1 + 0x55) = 0x80000000;
  *(undefined1 *)((longlong)param_1 + 0x15a) = 0;
  *(undefined1 *)((longlong)param_1 + 0x159) = 0;
  *(undefined4 *)(param_1 + 0x2f) = 0xffffff9c;
  param_1[0x32] = 0;
  *(undefined4 *)(param_1 + 0x33) = 0;
  param_1[0x36] = 0;
  *(undefined4 *)((longlong)param_1 + 0xec) = 0;
  param_1[0x39] = 0;
  *(undefined4 *)(param_1 + 0x3a) = 0;
  param_1[0x1b] = 0;
  *(undefined1 *)((longlong)param_1 + 0x121) = 0;
  *(undefined1 *)((longlong)param_1 + 0x122) = 0;
  param_1[0x51] = 0;
  param_1[0x150] = 0;
  *(undefined4 *)(param_1 + 0x47) = 0;
  *(undefined4 *)((longlong)param_1 + 0x23c) = 0;
  *(undefined4 *)(param_1 + 0x48) = 0;
  *(undefined4 *)((longlong)param_1 + 0x244) = 0;
  *(undefined4 *)(param_1 + 0x49) = 0;
  *(undefined4 *)((longlong)param_1 + 0x24c) = 0;
  *(undefined1 *)(param_1 + 0x4a) = 0;
  FUN_1400702f0(&local_res8,(char *)(uintptr_t)&DAT_14011d308,0xffffffffffffffffULL);
  FUN_140070398((longlong *)(param_1 + 0x4c),&local_res8);
  FUN_14005b104((long long *)&local_res8);
  *(undefined1 *)(param_1 + 0x4d) = 0;
  *(undefined4 *)((longlong)param_1 + 0x26c) = 0xffffffff;
  *(undefined4 *)(param_1 + 0x4e) = 0xffffffff;
  *(undefined1 *)(param_1 + 0x14) = 0;
  *(undefined1 *)(param_1 + 0x53) = 0;
  *(undefined4 *)(param_1 + 0x1a) = 0;
  param_1[0x14e] = 0;
  param_1[0x14f] = 0;
  param_1[6] = 0;
  param_1[5] = 0;
  param_1[0xd] = 0;
  *(undefined4 *)(param_1 + 0x30) = 0;
  *(undefined2 *)((longlong)param_1 + 0x184) = 0;
  *(undefined2 *)((longlong)param_1 + 0x186) = 0;
  *(undefined2 *)(param_1 + 0x31) = 0;
  *(undefined4 *)((longlong)param_1 + 0x17c) = 0xffffffff;
  return param_1;
}


/* ---- wave-current support: 0ebd30 ---- */
void  FUN_1400e7664(LPCWSTR a) { (void)a; }   /* ASL 串释放 (leaf stub) */
void *FUN_1400e7840(longlong *a, int b) { (void)a;(void)b; return (void*)0; }  /* 缺省名构造 (leaf stub) */
void  FUN_1400e4edc(void) { }                 /* 工作线程入口 (leaf stub) */
void *DAT_14013e1f8 = 0;    /* 0x14013e1f8 事件槽 A */
void *DAT_14013e200 = 0;    /* 0x14013e200 事件槽 B */
void *DAT_14013e208 = 0;    /* 0x14013e208 缓冲句柄槽 */
uint64_t DAT_14013e268 = 0x000005b708001323fcULL;  /* 0x14013e268 工作线程入口表 (pe_data_extract) */
uint64_t DAT_14013e280 = 0x000005b7e8001358ULL;    /* 0x14013e280 错误格式槽 (pe_data_extract) */
longlong DAT_14013e2a0 = 0;  /* 0x14013e2a0 当前接收方 */
longlong DAT_14013e298 = 0;  /* 0x14013e298 参数槽 */
uint8_t  DAT_14013e300[512] = {0}; /* 0x14013e300 错误消息缓冲 */

/* @0x1400ebd30 建立 ASL 接收线程并等待事件 (decompiled.c 直移) */
undefined8
FUN_1400ebd30(longlong param_1,LPCWSTR param_2,LPCWSTR param_3,LPCWSTR param_4,LPCWSTR param_5,
             LPCWSTR param_6)
{
  int iVar1;
  unsigned long DVar2;
  LPCWSTR pWVar3;
  unsigned short *lpString2;
  longlong local_228 [2];
  WCHAR local_218 [264];

  if (DAT_14013e2a0 != 0) {
    FUN_1400e7664((LPCWSTR)0x0);
  }
  if (DAT_14013e1f8 != (void *)0x0) {
    ResetEvent(DAT_14013e1f8);
  }
  if (DAT_14013e200 != (void *)0x0) {
    ResetEvent(DAT_14013e200);
  }
  if ((DAT_14013e1f8 == (void *)0x0) &&
     (DAT_14013e1f8 = CreateEventW((void *)0x0,0,0,(const unsigned short *)0x0),
     DAT_14013e1f8 == (void *)0x0)) {
    return 0;
  }
  if ((DAT_14013e200 == (void *)0x0) &&
     (DAT_14013e200 = CreateEventW((void *)0x0,0,0,(const unsigned short *)0x0),
     DAT_14013e200 == (void *)0x0)) {
    return 0;
  }
  local_228[0] = 0;
  *(undefined4 *)(param_1 + 8) = 0x83c;
  pWVar3 = param_2;
  if (param_2 == (LPCWSTR)0x0) {
    pWVar3 = (LPCWSTR)FUN_1400e7840(local_228,0);
    lpString2 = (unsigned short *)L"HelloASL";
    if (pWVar3 == (LPCWSTR)0x0) goto LAB_1400ebe23;
  }
  lpString2 = (unsigned short *)pWVar3;
LAB_1400ebe23:
  lstrcpyW((unsigned short *)(param_1 + 0xc),lpString2);
  if (param_3 == (LPCWSTR)0x0) {
    param_3 = (LPCWSTR)(void *)DAT_14011c638;
  }
  lstrcpyW((unsigned short *)(param_1 + 0x20e),param_3);
  if (param_4 == (LPCWSTR)0x0) {
    param_4 = (LPCWSTR)(void *)DAT_14011c638;
  }
  lstrcpyW((unsigned short *)(param_1 + 0x412),param_4);
  if (param_5 == (LPCWSTR)0x0) {
    param_5 = (LPCWSTR)(void *)DAT_14011c638;
  }
  lstrcpyW((unsigned short *)(param_1 + 0x614),param_5);
  if (param_6 == (LPCWSTR)0x0) {
    param_6 = (LPCWSTR)(void *)DAT_14011c638;
  }
  lstrcpyW((unsigned short *)(param_1 + 0x816),param_6);
  iVar1 = ((int (*)(longlong,longlong,void*,longlong,code,longlong *))(uintptr_t)DAT_14013e268)
          (0,0,(void *)(param_1 + 8),0,FUN_1400e4edc,(longlong *)(param_1 + 0xa78));
  if (iVar1 == 0) {
    DAT_14013e2a0 = *(longlong *)(param_1 + 0xa78);
    DAT_14013e298 = param_1;
    DVar2 = WaitForMultipleObjects(2,(void **)&DAT_14013e1f8,0,50000);
    if (DVar2 == 0) {
      FUN_14005b104((long long *)local_228);
      return 1;
    }
    if ((DVar2 == 1) || (DVar2 == 0x102)) {
      ((int (*)(void *, void *, int))(uintptr_t)DAT_14013e280)(DAT_14013e208,local_218,0x100);
      FUN_140063720((longlong **)&DAT_14013e300,100);
      wsprintfW((unsigned short *)DAT_14013e300,(const unsigned short *)L"Error: %d - ");
      PECMD_AppendWideStr((uint16_t **)&DAT_14013e300,local_218);
      PECMD_AppendWideStr((uint16_t **)&DAT_14013e300,(const unsigned short *)L"\n");
    }
    Sleep(10);
  }
  FUN_14005b104((long long *)local_228);
  return 0;
}


/* ---- wave-current support: 073ccc ---- */
void FUN_1400629b8(void *a, const WCHAR *b, const WCHAR *c) { (void)a;(void)b;(void)c; }  /* 变量赋值 (leaf stub) */
uint8_t DAT_1401206f4[8] = {0};   /* 0x1401206f4 "[]" 常量串 */

/* @0x140073ccc 拆分参数串为 token 数组并登记 __arg (decompiled.c 直移) */
uint PECMD_ArgTokenize(longlong *param_1,LPCWSTR param_2,int param_3)
{
  ushort uVar3;
  longlong lVar4;
  ushort uVar5;
  int iVar6;
  uint uVar7;
  undefined8 *puVar8;
  ulonglong uVar9;
  ulonglong uVar10;
  ushort *puVar11;
  ushort *puVar12;
  ulonglong uVar13;
  longlong lVar14;
  int iVar15;
  ulonglong uVar16;
  uint local_res20;
  ushort *local_70;
  undefined8 *local_68;
  ushort *local_60;
  longlong local_58;
  longlong local_50;
  ulonglong local_48;
  longlong *plVar1;
  longlong lVar17;

  FUN_140063694(&local_68,4);
  plVar1 = param_1 + 0xe;
  FUN_1400703e4((long long *)plVar1,param_2);
  iVar6 = lstrlenW(param_2);
  uVar10 = 0;
  iVar6 = (int)((longlong)
                ((ulonglong)((uint)((longlong)iVar6 + 8 >> 0x3f) & 3) + (longlong)iVar6 + 8) >> 2);
  uVar7 = iVar6 * 4;
  local_48 = (ulonglong)uVar7;
  iVar6 = iVar6 * 8;
  local_50 = (longlong)(iVar6 + 0x20);
  FUN_140063720((longlong **)plVar1,local_50);
  local_58 = (longlong)(int)uVar7 * 2;
  PECMD_MemMoveForward((undefined1 *)*plVar1 + local_58,(undefined1 *)*plVar1,(int)local_58);
  local_70 = (ushort *)*plVar1;
  PECMD_SkipLeadingControlChars((long long *)&local_70);
  puVar12 = local_70;
  local_60 = local_70;
  puVar8 = local_68;
  uVar9 = uVar10;
  uVar16 = uVar10;
  if ((*local_70 == 0x5b) && (local_70[1] == 0x5d)) {
    FUN_140063720((longlong **)&local_68,0xc);
    puVar8 = local_68;
    puVar12 = puVar12 + 2;
    *local_68 = (undefined8)(uintptr_t)&DAT_1401206f4;
    local_70 = puVar12;
    local_60 = puVar12;
    PECMD_SkipLeadingControlChars((long long *)&local_70);
    uVar9 = 1;
    uVar16 = (longlong)local_70 - *plVar1 >> 1;
  }
  iVar15 = (int)uVar16;
  uVar5 = *local_70;
  uVar7 = (uint)uVar9;
  lVar14 = (longlong)(int)uVar7;
  if (uVar5 != 0) {
    local_res20 = uVar7 * 4 + 0xc;
    uVar13 = (ulonglong)local_res20;
    puVar11 = local_70;
    puVar12 = local_60;
    do {
      local_70 = puVar11 + 1;
      uVar3 = *local_70;
      if (uVar5 == 0x22) {
        while ((uVar3 != 0 &&
               ((uVar3 != 0x22 ||
                (((uVar5 = local_70[1], uVar5 != 0 && ((uVar5 < 9 || (0xd < uVar5)))) &&
                 (uVar5 != 0x20))))))) {
          local_70 = local_70 + 1;
          uVar3 = *local_70;
        }
        if (*local_70 == 0x22) {
          local_70 = local_70 + 1;
        }
      }
      else {
        while ((uVar3 != 0 && (((uVar3 < 9 || (0xd < uVar3)) && (uVar3 != 0x20))))) {
          local_70 = local_70 + 1;
          uVar3 = *local_70;
        }
      }
      if (lVar14 == 0) {
        puVar12 = local_70;
      }
      if (*local_70 != 0) {
        *local_70 = 0;
        local_70 = local_70 + 1;
      }
      local_60 = puVar11;
      FUN_140063720((longlong **)&local_68,(longlong)(int)uVar13);
      puVar8 = local_68;
      local_res20 = local_res20 + 4;
      uVar13 = (ulonglong)local_res20;
      local_68[lVar14] = (undefined8)(uintptr_t)local_60;
      uVar7 = (int)uVar9 + 1;
      uVar9 = (ulonglong)uVar7;
      lVar14 = lVar14 + 1;
      PECMD_SkipLeadingControlChars((long long *)&local_70);
      if (lVar14 == 1) {
        uVar16 = (longlong)local_70 - *plVar1 >> 1 & 0xffffffff;
      }
      iVar15 = (int)uVar16;
      uVar5 = *local_70;
      puVar11 = local_70;
      local_60 = puVar12;
    } while (uVar5 != 0);
  }
  if (param_3 != 0) {
    FUN_1400629b8(param_1,(const unsigned short *)L"&&__arg",(LPCWSTR)((longlong)puVar12 + local_58));
  }
  lVar17 = *plVar1;
  *(uint *)(param_1 + 0xc) = uVar7;
  FUN_140063720((longlong **)plVar1,(longlong)(int)(iVar6 + 0x7a + uVar7 * 4));
  wsprintfW((unsigned short *)(*plVar1 + (longlong)iVar6 * 2),(const unsigned short *)L"%d");
  lVar4 = *plVar1;
  lVar14 = (longlong)(int)uVar7;
  param_1[0xd] = lVar4 + local_50 * 2;
  if (0 < lVar14) {
    uVar10 = 0;
    do {
      *(longlong *)(param_1[0xd] + uVar10 * 8) = (longlong)puVar8[uVar10] + (lVar4 - lVar17 >> 1) * 2;
      uVar10 = uVar10 + 1;
    } while ((longlong)uVar10 < lVar14);
  }
  *(longlong *)(param_1[0xd] + lVar14 * 8) = *plVar1 + (longlong)((int)local_48 + iVar15) * 2;
  *(longlong *)(param_1[0xd] + 8 + lVar14 * 8) = local_58 + *plVar1;
  *(longlong *)(param_1[0xd] + 0x10 + lVar14 * 8) = *plVar1 + (longlong)iVar6 * 2;
  FUN_14005b104((long long *)&local_68);
  return uVar7;
}


/* ---- wave-current support: 082520 ---- */
uint32_t DAT_14013e20c = 0;  /* 0x14013e20c SeDebug 已启用标志 */

/* @0x140082520 按名称关闭/重启窗口 (decompiled.c 直移) */
longlong PECMD_CloseRestartByName(longlong *param_1,LPCWSTR param_2,longlong param_3,ulonglong param_4)
{
  int iVar1;
  undefined8 uVar2;
  longlong lVar3;
  undefined8 *puVar4;
  unsigned int Msg;
  WCHAR *pWVar5;
  HWND hWnd;
  LPCWSTR pWVar6;
  int iVar7;
  int bVar8;
  LPCWSTR local_res10 [2];
  int local_res20 [2];
  LPCWSTR local_48;
  LPCWSTR local_40;

  local_res10[0] = param_2;
  PECMD_AllocStrSlot((uint16_t **)local_res10);
  lVar3 = 0;
  iVar7 = -1;
  if ((param_4 & 1) != 0) {
    iVar7 = 1;
  }
  if ((param_4 & 2) != 0) {
    iVar7 = 0;
  }
  PECMD_MatchTokenAdvance("*svr",(long long *)local_res10,-1);
  bVar8 = (DAT_14013e20c & 4) == 0;
  if (bVar8) {
    DAT_14013e20c = DAT_14013e20c | 4;
    FUN_14001c2cc((const unsigned short *)L"SeDebugPrivilege",2,0);
  }
  if (*local_res10[0] == L'@') {
    if (local_res10[0][1] != L'@') {
      if (*local_res10[0] != L'@') goto LAB_14008278b;
      PECMD_ExpandVarDispatch(param_1,(WCHAR *)local_res10[0] + 1,(undefined8 *)&local_48,0,1);
      local_res10[0] = local_48;
      PECMD_AllocStrSlot((uint16_t **)local_res10);
      local_40 = local_48;
      FUN_1400675b8((int64_t *)&local_40,(int64_t *)local_res10,0);
      FUN_140003a20(param_1,(undefined8 *)local_res10,0);
      pWVar6 = local_res10[0];
      uVar2 = FUN_14005c7c4("this",(ushort *)local_res10[0]);
      if (((char)uVar2 == '\0') && (*pWVar6 != L'\0')) {
        EnterCriticalSection((LPCRITICAL_SECTION)&DAT_14013e190);
        do {
          param_3 = param_1[8];
          if (((param_3 != 0) && (param_3 != lVar3)) &&
             (iVar1 = lstrcmpiW(*(LPCWSTR *)(param_3 + 0x128),local_res10[0]), iVar1 == 0)) {
            LeaveCriticalSection((LPCRITICAL_SECTION)&DAT_14013e190);
            FUN_14005b104((long long *)local_res10);
            goto LAB_14008283d;
          }
          param_1 = (longlong *)param_1[7];
          lVar3 = param_3;
        } while (param_1 != (longlong *)0x0);
        LeaveCriticalSection((LPCRITICAL_SECTION)&DAT_14013e190);
        FUN_14005b104((long long *)local_res10);
        lVar3 = 1;
        if (bVar8) {
          DAT_14013e20c = DAT_14013e20c & 0xfffb;
          FUN_14001c2cc((const unsigned short *)L"SeDebugPrivilege",4,0);
        }
        goto LAB_140082878;
      }
      FUN_14005b104((long long *)local_res10);
      goto LAB_1400827d0;
    }
    PECMD_ExpandVarDispatch(param_1,(WCHAR *)local_res10[0] + 2,(undefined8 *)&local_48,0,1);
    local_res10[0] = local_48;
    local_res20[0] = 0;
    FUN_140074838((longlong *)local_res10,local_res20);
    hWnd = (HWND)(longlong)local_res20[0];
    if (hWnd == (HWND)0x0) {
      if (bVar8) {
        DAT_14013e20c = DAT_14013e20c & 0xfffb;
        FUN_14001c2cc((const unsigned short *)L"SeDebugPrivilege",4,0);
      }
      lVar3 = -0x7ff8ffa9;
      goto LAB_140082878;
    }
  }
  else {
LAB_14008278b:
    pWVar5 = (WCHAR *)local_res10[0] + 1;
    if (((*pWVar5 == L'\\') && (local_res10[0][2] == L'\0')) &&
       (pWVar5 = (WCHAR *)local_res10[0] + 2, iVar7 < 0)) {
      iVar7 = 1;
    }
    PECMD_ExpandVarDispatch(param_1,pWVar5,(undefined8 *)&local_48,0,1);
    if (*local_48 == L'\0') {
LAB_1400827d0:
      if (param_3 != 0) {
        while ((*(uint *)(param_3 + 0xd0) >> 0xf & 1) != 0) {
          for (puVar4 = *(undefined8 **)(*(longlong *)(param_3 + 0x290) + 0x38);
              ((puVar4 != (undefined8 *)0x0 && (puVar4 != &DAT_14013d130)) &&
              ((puVar4[8] == 0 || (param_3 == puVar4[8])))); puVar4 = (undefined8 *)puVar4[7]) {
          }
          if (puVar4[8] == 0) break;
          param_3 = puVar4[8];
        }
        SetWindowPos(*(HWND *)(param_3 + 0x20),(HWND)0x0,0,0,0,0,0x97);
LAB_14008283d:
        hWnd = *(HWND *)(param_3 + 0x20);
        Msg = 0x43c;
        goto LAB_1400828dc;
      }
      if (*local_48 == L'\0') {
        if (bVar8) {
          DAT_14013e20c = DAT_14013e20c & 0xfffb;
          FUN_14001c2cc((const unsigned short *)L"SeDebugPrivilege",4,0);
        }
LAB_140082878:
        FUN_14005b104((long long *)&local_48);
        return lVar3;
      }
    }
    hWnd = (HWND)FUN_1400e3d60(local_48,'\0','\0',(HWND)0x0,0,0,0,0);
  }
  SetWindowPos(hWnd,(HWND)0x0,0,0,0,0,0x97);
  Msg = 0x10;
LAB_1400828dc:
  if (iVar7 < 1) {
    SendMessageW(hWnd,Msg,0,0);
  }
  else {
    PostMessageW(hWnd,Msg,0,0);
  }
  if (bVar8) {
    DAT_14013e20c = DAT_14013e20c & 0xfffb;
    FUN_14001c2cc((const unsigned short *)L"SeDebugPrivilege",4,0);
  }
  FUN_14005b104((long long *)&local_48);
  return 0;
}
