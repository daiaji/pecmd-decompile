/* stubs_common.h - B0/P3: shared declarations extracted from link_stubs.c.
 * Mechanical split per PRODUCTION_ROADMAP §3.B.0 item 4. */
#ifndef STUBS_COMMON_H
#define STUBS_COMMON_H

typedef unsigned int uint;
/* 兼容 setupdi 桩 */
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

typedef intptr_t LRESULT;

typedef unsigned long ULONG_PTR;

typedef uint32_t ACCESS_MASK;

typedef uint16_t WORD;

typedef int16_t SHORT;

typedef struct _SECURITY_ATTRIBUTES {
    ULONG nLength;
    void *lpSecurityDescriptor;
    BOOL bInheritHandle;
} SECURITY_ATTRIBUTES;

typedef SECURITY_ATTRIBUTES *LPSECURITY_ATTRIBUTES;

typedef struct _OVERLAPPED {
    ULONG_PTR Internal;
    ULONG_PTR InternalHigh;
    union {
        struct {
            ULONG Offset;
            ULONG OffsetHigh;
        } s;
        void *Pointer;
    } u;
    void *hEvent;
} OVERLAPPED;

typedef OVERLAPPED *LPOVERLAPPED;

typedef struct _FILETIME {
    DWORD dwLowDateTime;
    DWORD dwHighDateTime;
} FILETIME, *LPFILETIME;

/* PROCESSENTRY32W — 进程枚举结构 (core_b1_remaining.c 同款布局; PECMD_EnumProcessInfo 还原用) */
typedef struct tagPROCESSENTRY32W {
    DWORD dwSize;
    DWORD cntUsage;
    DWORD th32ProcessID;
    uintptr_t th32DefaultHeapID;
    DWORD th32ModuleID;
    DWORD cntThreads;
    DWORD th32ParentProcessID;
    LONG pcPriClassBase;
    DWORD dwFlags;
    WCHAR szExeFile[260];
} PROCESSENTRY32W;

/* --- Ghidra 反编译伪类型映射 (P4 wave-4 移植) --- */
typedef uint64_t undefined8;

typedef uint32_t undefined4;

typedef uint16_t undefined2;

typedef uint8_t undefined1;

typedef uint8_t undefined;

typedef uint16_t ushort;

typedef int64_t longlong;

typedef uint64_t ulonglong;

typedef HKEY *PHKEY;

undefined8 *PECMD_InitControlObjField(undefined8 *param_1, undefined8 param_2, uint param_3,
                                      undefined8 *param_4);
/* 控件对象构造 (定义见文件尾) */

/* ---- P4 wave-4 统一前置声明 (确保早放置的恢复体调用可正确编译) ---- */
typedef void *HGDIOBJ_H;

void *RegisterServiceCtrlHandlerW(const WCHAR *name, void *handler);

int SetServiceStatus(void *h, void *status);

WCHAR *GetCommandLineW(void);

void *CreateThread(void *sa, size_t st, void *rt, void *p, uint32_t f, uint32_t *tid);

void *GetDesktopWindow(void);

HWND GetParent(HWND w);

DWORD GetCurrentThreadId(void);

DWORD GetWindowThreadProcessId(HWND w, DWORD *pid);

void *LoadLibraryA(const char *m);

FARPROC GetProcAddress(void *hm, const char *name);

DWORD GetTickCount(void);
/* 统一签名 (原 unsigned long 与 DWORD 定义冲突, LP64 下 64位) */
uint64_t GetProcessHeap(void);

int64_t *PECMD_AssignString(int64_t *, const uint16_t *);

void *TlsGetValue(unsigned long idx);
extern unsigned long DAT_14013c934;
;
;
extern longlong DAT_14013cb18[8];
;
;

uint64_t SizeofResource(uint64_t a, uint64_t b);

uint64_t PECMD_EncodeDet(long long a, uint64_t b);

uint64_t *PECMD_AssignAnsiString(uint64_t *, char *);

void *FUN_140063224(uint64_t *a, uint64_t b);

uint16_t *StrChrW(const uint16_t *s, uint16_t c);

void *PECMD_BuildFontFromObject(int64_t a, void *b, const void *c);

const uint16_t *PECMD_LoadLocalizedString(void *hinst, uint32_t id, uint16_t *buf, int len);

const uint16_t *PECMD_LangLookupById(int a, const uint16_t **b);

uint64_t EndDialog(uint64_t a, int b);


void *GetModuleHandleW(const WCHAR *m);

int SetCurrentDirectoryW(const WCHAR *dir);

void *HeapAlloc(void *h, unsigned long f, size_t n);

int HeapFree(void *h, unsigned long f, void *p);

void EnterCriticalSection(void *cs);

void LeaveCriticalSection(void *cs);

unsigned long GetCurrentDirectoryW(unsigned long n, WCHAR *buf);

unsigned long GetEnvironmentVariableW(const WCHAR *n, WCHAR *buf, unsigned long sz);

int SetEnvironmentVariableW(const WCHAR *n, const WCHAR *v);

int lstrcmpiW(const WCHAR *a, const WCHAR *b);

int RegCreateKeyExW(void *k, const WCHAR *sub, unsigned long res, void *cls, unsigned long opt,
                    unsigned long acc, void *sa, void *out, unsigned long *disp);

int RegQueryValueExW(void *k, const WCHAR *nm, unsigned long *rs, unsigned long *ty, BYTE *dt,
                     unsigned long *sz);

int RegCloseKey(void *k);

unsigned long GetLastError(void);

int IsWindow(void *h);

uint64_t SetWindowLongPtrW(void *h, int idx, uint64_t v);

int DestroyWindow(void *h);

int DeleteObject(void *h);

int SetWindowRgn(void *h, void *r, int b);

intptr_t SendMessageW(void *h, unsigned int m, uint64_t w, uint64_t l);

int ShowWindow(void *h, int c);

int PeekMessageW(void *msg, void *hwnd, unsigned int a, unsigned int b, unsigned int c);

int TranslateMessage(const void *m);

uint64_t DispatchMessageW(const void *m);

LONG GetWindowLongW(void *h, int idx);

void *CreateWindowExW(unsigned long ex, const WCHAR *cls, const WCHAR *name, unsigned long style,
                      int x, int y, int w, int h, void *parent, void *menu, void *inst,
                      void *param);

void *operator_new(uint64_t n);


void *GetModuleHandleA(const char *m);

UINT RegisterWindowMessageW(const WCHAR *s);

void FUN_140053e78(void);
extern uint64_t PTR_FUN_1401234f0;
;
;
/* vtable 真值 0x1401234f0 地址槽 */
extern int (*DAT_14013c970)(uint32_t, uint32_t, char *, int);

uint64_t FUN_14005c7c4(const char *a, const uint16_t *b);

void FUN_1400702b0(uint16_t **ps, const uint16_t *src);

int64_t *FUN_14007034c(int64_t *param_1, const uint16_t *param_2);


/* helper 补前置声明 */
void PECMD_ExecInterpString(uint64_t a, void *b);

/* PECMD_ExecInterpString 实体内调用 (rename_map 命名; 定义在其它 core_*.c) */
int64_t PECMD_ExecuteCommand(int64_t *p1, LPCWSTR p2, int64_t p3, LPCWSTR p4, uint32_t p5,
                             int64_t p6, int p7);

void PECMD_FreeTripleString(int64_t *p);

uint64_t FUN_14004fb44(longlong *a, ulonglong b);

void FUN_14005d9a8(int64_t a, int b);

uint64_t FUN_1400630d0(int a);

void PECMD_CleanupFrameChain(int64_t a);

void PECMD_ArrayAppend(int64_t script, int64_t obj);
/* rename_map: FUN_14006e8a4; 定义在 core_b3e.c */
DWORD PECMD_RegOpenWithRetryPriv(HKEY param_1, LPCWSTR param_2, PHKEY param_3, REGSAM param_4,
                                 uint param_5);

void *OpenDesktopW(const WCHAR *n, uint64_t f, uint64_t acc, uint64_t flags);

LONG RegSetValueExW(void *k, const unsigned short *n, unsigned long r, unsigned long t,
                    const unsigned char *d, unsigned long c);

HMODULE LoadLibraryW(const WCHAR *name);

unsigned int DragQueryFileW(uint64_t a, uint32_t b, void *c, uint32_t d);

uint16_t *PECMD_AllocStrSlot(uint16_t **out);

uint8_t *PECMD_MemMoveSafe(void *a, longlong b, longlong c);
extern void *DAT_14013cb10;
;
;
extern void *DAT_14013ccf8;
;
;

extern void (*DAT_14013cb48)(...);
extern void *DAT_14013cd18;
;
;
extern void *DAT_14013cd20;
;
;
extern void *DAT_14013cd28;
;
;

extern int (*DAT_14013cd30)(...);
extern int (*DAT_14013cd38)(...);
extern void (*DAT_14013cd40)(...);
extern void *DAT_14013cfb0;
;
;

uint64_t VirtualFree(void *a, uint64_t b, uint64_t c);

uint64_t lstrcatW(void *a, uint64_t b);

int64_t FUN_140065864(int64_t a, int64_t *b, int64_t *c, uint8_t *d, uint32_t e);
extern void *DAT_14013cf50;
;
;
extern void *DAT_1401293c0;
;
;
extern void *DAT_14013d868;
;
;
extern uint64_t DAT_14013e2a8;
;
;
extern void *DAT_14013ca68;
;
;

uint64_t PECMD_OomPrompt(int a);

uint64_t FindResourceW(void *a, void *b, void *c);

uint64_t LoadResource(void *a, uint64_t b);

uint64_t LockResource(uint64_t a);
extern void *DAT_14013e1f8;
;
;
extern void *DAT_14013e200;
;
;

int SetThreadDesktop(void *d);
int SwitchDesktop(void *d);
int CloseDesktop(void *d);

DWORD GetCurrentDirectoryW(DWORD n, WCHAR *buf);

DWORD GetEnvironmentVariableW(const WCHAR *n, WCHAR *buf, DWORD sz);

int SetEnvironmentVariableW(const WCHAR *n, const WCHAR *v);

int lstrcmpiW(const WCHAR *a, const WCHAR *b);

extern long long DAT_14013d130;

int64_t FUN_140018978(int64_t *a, const uint16_t *b, int64_t *c, int64_t d, void *e);

void PECMD_SetVariable(void *a, const WCHAR *b, const WCHAR *c);

void PECMD_DialogBeepNotify(int64_t a, int b);

void PECMD_ScaleQuadByFactor(int64_t a, int *b, int *c, int *d, int *e);

/* ---- PECMD_Dialog2727Proc 恢复体所需前置声明 (定义在文件后部/其它 core_*.c; 命名按
 * tools/rename_map.json) ---- */
extern void *g_hFontE2B0;
/* DAT_14013e2b0 (core_globals.c: HFONT, 已有定义) */
extern void PECMD_GetUiFontById(void **pfont, UINT id);
/* @0x1400e648c 按 lang 创建字体 (core_exec.c) */
extern void PECMD_TrimWorkingSet(char force);
/* @0x14005d4e4 (core_b3e.c) */
int GetSystemMetrics(int idx);

HWND GetDlgItem(void *hWnd, int nIDDlgItem);

int SetWindowTextW(void *hWnd, const void *lpString);

uint64_t GetWindowTextW(void *hWnd, WCHAR *lpString, int nMaxCount);

int LoadStringW(void *h, unsigned int id, WCHAR *buf, int n);

uint64_t lstrcmpW(const uint16_t *a, const uint16_t *b);

int GetWindowRect(void *w, void *r);

int SetWindowPos(void *w, void *after, int x, int y, int cx, int cy, unsigned int f);

int EnableWindow(HWND w, int en);

void FUN_140102a90(uint64_t *dst, uint64_t v, uint64_t n);
/* CRT memset 内联 */
uint64_t PECMD_ShutPowerAction(uint64_t a);
/* 命令入队 (2369 桩) */
extern uint8_t DAT_14011c638[64];
/* 默认命令串缓存 (5600 定义) */
void PECMD_StrDupAssign(uint16_t **ps, const uint16_t *src);
/* @0x1400702b0 */
uint64_t PECMD_EncodeStringId(LPCWSTR name, uint64_t *out, char mode);

extern void (*DAT_14013cd90)(...);

/* ---- 早期放置的 wave-current 还原体所需 Win32 前置声明 (定义见字母桩区) ---- */
void GetStartupInfoW(void *d);

uint64_t SetActiveWindow(void *h);

int SetEndOfFile(void *h);

uint64_t SetForegroundWindow(void *h);

uint64_t BringWindowToTop(void *h);
/* ---- P4 wave-4 (p4_wave1.json[120:160]) 支持块 (并发迁移后重建): 全局真值 + 内部 helper 前置声明
 * ---- 全局初值(真值)来源: python3 tools/pe_data_extract.py ../PECMD.exe <addr> <n> */
extern uint32_t DAT_14013a038;
;
;
/* g_bX64      0x14013a038: 03 00 00 00 */
extern void *DAT_14013c910;
;
;
/* 服务句柄槽   0x14013c910 */
extern uint32_t DAT_14013c918;
;
;
/* SERVICE_STATUS 阵列 0x14013c918 (8 x DWORD) */
extern uint32_t DAT_14013c91c;
;
;
extern uint32_t DAT_14013c920;
;
;
extern uint32_t DAT_14013c924;
;
;
extern uint32_t DAT_14013c928;
;
;
extern uint32_t DAT_14013c92c;
;
;
extern uint32_t DAT_14013c930;
;
;
extern uint8_t DAT_14013c96a;
;
;
/* g_flag16a */
extern uint8_t DAT_14013d260;
;
;
/* 0x14013d260 原始: b4 3a 13 00 (定时清理标志) */
extern uint32_t DAT_14013c9f8;
;
;
/* g_WM_TaskbarButtonCreated */
extern void *DAT_14013cfc8;
;
;
/* g_hUser32 */
extern void *DAT_14013cfd0;
;
;
/* g_hKernel32 */
extern void *DAT_14013cfd8;
;
;
/* g_hGdi32 */
extern uint8_t DAT_14013d300;
;
;
/* RichEdit 选择标志 0x14013d300: 10 */
extern void *DAT_14013d328;
;
;
/* g_hHeap */
extern int (*DAT_14013d800)(int);
/* OleInitialize 槽 */
extern void (*DAT_14013d808)(void);
/* OleUninitialize 槽 */
extern int (*DAT_14013d820)(int);
/* CRITICAL_SECTION (g_csInit) */
extern uint32_t DAT_140147000;
;
;

void FUN_14000500c(void);

void FUN_140005738(int a, const char *b);

uint64_t FUN_140008c5c(uint64_t a, uint64_t b, void *c, void *d);

uint64_t FUN_14000befc(void *p);

uint64_t FUN_14005370c(uint32_t *a, void *b, uint64_t c);

void *FUN_140057334(void *a, longlong b, int c, void *d, int e, int f, int g, int h, void *i,
                    void *j, uint32_t k);

void *FUN_14005762c(void *a, longlong b, int c, void *d, int e, int f, int g, int h, void *i,
                    void *j, uint32_t k);

int FUN_14005ceec(const uint16_t *a, const char *b);

void FUN_14005d694(void);

void *FUN_140060834(uint64_t a, uint64_t b);

void FUN_14006355c(void *a, const WCHAR *b, int c, uint64_t d);

void FUN_140063888(void *a, const WCHAR *b, int64_t c);

void *FUN_14006e3a4(const WCHAR *a);

void *FUN_14006e74c(const WCHAR *a, char b, uint32_t *c);

void *FUN_14009c720(void *a, longlong b, int c, void *d, int e, int f, int g, int h, uint16_t *i,
                    void *j, uint32_t k);

void *FUN_14009cacc(void *a, longlong b, int c, void *d, int e, int f, int g, int h, void *i,
                    void *j, uint32_t k, WCHAR *l);

uint64_t FUN_14009d4b8(uint64_t a, uint64_t b, const WCHAR *c, int16_t d, const WCHAR *e, int f);

void *PECMD_CreateNamedWaitObj(const WCHAR *a, char b);

void *FUN_1400aa144(void *a, longlong b, int c, void *d, int e, int f, int g, int h, uint32_t i,
                    WCHAR *j, uint8_t k);

void *FUN_1400b8f10(void *a, longlong b, int c, void *d, int e, int f, int g, int h, void *i,
                    uint32_t j, int *k, const WCHAR *l);

void *FUN_1400bca60(void *a, longlong b, int c, void *d, int e, int f, int g, int h, void *i,
                    void *j, uint32_t *, uint32_t l, uint32_t m, uint32_t n);

int64_t PECMD_CtlForwardSetBlock(longlong *a, void *b);

void *RegisterServiceCtrlHandlerW(const WCHAR *name, void *handler);

int SHGetSpecialFolderPathA(void *hwnd, char *buf, int folder, int create);

char *lstrcatA(char *dst, const char *src);

typedef union _LARGE_INTEGER {
    struct {
        DWORD LowPart;
        LONG HighPart;
    } s;
    struct {
        DWORD LowPart;
        LONG HighPart;
    } u;
    long long QuadPart;
} LARGE_INTEGER;
/* wave-4 (补 .s 成员供 Ghidra 直移) */
typedef union _ULARGE_INTEGER {
    struct {
        DWORD LowPart;
        DWORD HighPart;
    } s;
    unsigned long long QuadPart;
} ULARGE_INTEGER;

typedef uint64_t undefined7;
/* Ghidra 7字节伪类型 (按 64 位承载) */
/* ---- wave-N p4 restored-helper extra Win32/minimal types (direct-port 支持) ---- */
typedef void *LPTHREAD_START_ROUTINE;

typedef BOOL (*WNDENUMPROC)(HWND, LPARAM);

typedef size_t SIZE_T;

typedef uint8_t byte;

typedef void (*code)(void);

typedef DWORD *LPDWORD;

typedef WCHAR *LPWCH;

typedef struct _RTL_CRITICAL_SECTION {
    void *DebugInfo;
    LONG LockCount;
    LONG RecursionCount;
    HANDLE OwningThread;
    HANDLE LockSemaphore;
    ULONG_PTR SpinCount;
} RTL_CRITICAL_SECTION, CRITICAL_SECTION, *LPCRITICAL_SECTION, *PRTL_CRITICAL_SECTION;

typedef struct _PROCESS_INFORMATION {
    HANDLE hProcess;
    HANDLE hThread;
    DWORD dwProcessId;
    DWORD dwThreadId;
} PROCESS_INFORMATION, *LPPROCESS_INFORMATION;

typedef struct _STARTUPINFOW {
    DWORD cb;
    LPWSTR lpReserved;
    LPWSTR lpDesktop;
    LPWSTR lpTitle;
    DWORD dwX;
    DWORD dwY;
    DWORD dwXSize;
    DWORD dwYSize;
    DWORD dwXCountChars;
    DWORD dwYCountChars;
    DWORD dwFillAttribute;
    DWORD dwFlags;
    WORD wShowWindow;
    WORD cbReserved2;
    void *lpReserved2;
    void *hStdInput;
    void *hStdOutput;
    void *hStdError;
} STARTUPINFOW, *LPSTARTUPINFOW;

typedef struct _WIN32_FIND_DATAW {
    DWORD dwFileAttributes;
    FILETIME ftCreationTime;
    FILETIME ftLastAccessTime;
    FILETIME ftLastWriteTime;
    DWORD nFileSizeHigh;
    DWORD nFileSizeLow;
    DWORD dwReserved0;
    DWORD dwReserved1;
    WCHAR cFileName[260];
    WCHAR cAlternateFileName[14];
} WIN32_FIND_DATAW, *LPWIN32_FIND_DATAW;

typedef struct tagPOINT {
    LONG x;
    LONG y;
} POINT, tagPOINT, *LPPOINT;

typedef void *PACL;
typedef void *PSID;
typedef void *PSECURITY_DESCRIPTOR;

#define GRANT_ACCESS     1
#define TRUSTEE_IS_NAME  1
#define TRUSTEE_IS_USER  1
#define SE_KERNEL_OBJECT 6
typedef struct _TRUSTEE {
    void *pMultipleTrustee;
    uint32_t MultipleTrusteeOperation;
    uint32_t TrusteeForm;
    uint32_t TrusteeType;
    void *ptstrName;
} TRUSTEE;

typedef struct _EXPLICIT_ACCESS_W {
    uint32_t grfAccessPermissions;
    uint32_t grfAccessMode;
    uint32_t grfInheritance;
    TRUSTEE Trustee;
} EXPLICIT_ACCESS_W;

unsigned int PECMD_WindowRectHitTest(HWND a, POINT b);


bool PECMD_EnumChildFindProc(POINT a, POINT *b);

extern int64_t (*DAT_14013ce30)(void);
extern void *DAT_14013d3b8;
;
;
extern int64_t DAT_14013a24f;
;
;
extern int64_t DAT_14013a24c;
;
;
extern longlong DAT_14013e118;
;
;
extern longlong DAT_14013e120;
;
;
extern longlong DAT_14013e128;
;
;

typedef struct tagRECT {
    LONG left;
    LONG top;
    LONG right;
    LONG bottom;
} RECT, tagRECT, *LPRECT;

typedef struct tagMSG {
    HWND hwnd;
    UINT message;
    ulonglong wParam;
    longlong lParam;
    DWORD time;
    POINT pt;
} MSG, tagMSG;

#ifdef __MINGW32__
/* mingw corecrt.h provides the real pthreadlocinfo */
#else
typedef void *pthreadlocinfo;
#endif

typedef struct _SHELLEXECUTEINFOW {
    DWORD cbSize;
    ULONG fMask;
    HWND hwnd;
    void *lpVerb;
    void *lpFile;
    void *lpParameters;
    void *lpDirectory;
    int nShow;
    void *hInstApp;
    void *lpIDList;
    void *lpClass;
    void *hkeyClass;
    DWORD dwHotKey;
    void *hIconOrMonitor;
    void *hProcess;
} SHELLEXECUTEINFOW, *LPSHELLEXECUTEINFOW;


/* ---- wave-2 restored-helper 内部 FUN_ 前置声明 (定义在文件后部, 需先声明供 new 桩调用) ---- */
void FUN_1400166b4(void);

int PECMD_ParseNumTryWriteback(long long *, int *);

int lstrlenW(const WCHAR *);

long long *PECMD_SkipLeadingControlChars(long long *);

long long FUN_140064a34(uint16_t *);

long long PECMD_WideStrToInt64(uint16_t *);

long long FUN_14006b8fc(long long *);

void PECMD_InitButtonState(long long, uint, uint64_t);

void FUN_140005344(void);

uint16_t *FUN_14000531c(uint16_t *);

uint8_t *PECMD_MemMoveForward(uint8_t *, uint8_t *, int);

void PECMD_StopDeleteService(uint16_t *);

void FUN_140017048(const WCHAR *);

void FUN_140021144(const WCHAR *);

void FUN_140019da8(long long, const WCHAR *, int);

void FUN_140025f10(longlong, const WCHAR *, uint32_t, char *, char *, long long *);

void FUN_140020fd4(long long, char, const WCHAR *, const WCHAR *);

extern uint8_t DAT_14011c638[64];

uint64_t FUN_14005ea5c(void);

uint FUN_14000e0bc(void);

uint64_t FUN_14006042c(void);

uint64_t PECMD_LoadNtdllApis(void);

LPWSTR StrRChrW(const WCHAR *, const WCHAR *, WCHAR);

uint64_t thunk_FUN_1400f429c(void *, short);

extern void *PECMD_GrowByteBuffer(void **ps, int64_t len);
/* @0x140063424 (真实体 core_thread.c) */
void PECMD_HeapFreeWithHeader(long long *param_1);
/* @0x14005b0d4 串槽释放 (桩见文件后部) */
/* ---- wave-7 (P4 [240:288]) 内部依赖前置声明 (真实体见文件后部 wave-7 节) ---- */
int FUN_140067cf4(long long *, uint64_t *);
/* @0x140067cf4 数值解析包装 */
void PECMD_InitContainerFields(uint32_t *);
/* @0x1400706b4 控件表初始化 */
uint FUN_14001b608(uint64_t);
/* @0x14001b608 注册表读取 */
long long FUN_140031454(long long *, pthreadmbcinfo);
/* @0x140031454 INDATA 串执行 */
void PECMD_InitTableSlots(uint64_t *, int64_t *, uint64_t *, uint);
/* @0x140063a6c 表初始化 */
extern long long DAT_14013d130;

uint64_t SetEntriesInAclW(int c, void *ea, void *old, void **newa);

uint64_t GetSecurityInfo(void *h, int t, uint32_t i, void **a, void **b, void **c, void **d,
                         void **e);

uint64_t SetSecurityInfo(void *h, int t, uint32_t i, void *a, void *b, void *c, void *d);

uint64_t LocalFree(uint64_t hMem);


/* ========== PECMD_ExecCmdDispatch @ 14000e26c  size=33864 ========== */
/* signature: _FILETIME __fastcall PECMD_ExecCmdDispatch(longlong*, _FILETIME, longlong*, _FILETIME,
 * uint, _FILETIME*, _FILETIME, undefined8*) 移植: decompiled.c @7726 (3948 行) 分段恢复; 桩签名
 * 8×uint64_t 保持不变. _FILETIME 形参/局部视为 8 字节不透明值 (FTU64 联合: .v 标量, .ft.dwLow/High
 * 字段), CONCAT/字节片按 Ghidra 语义展开; 内部调用按 tools/rename_map.json 命名 (未映射用原名);
 * L"..." 按本文件惯例强转 (const uint16_t*). */
typedef union FTU64 {
    uint64_t v;
    FILETIME ft;
} FTU64;
/* 本函数 _FILETIME 不透明值局部 */
extern uint8_t DAT_14013c968;
;
;
/* @0x14013c968 标志 (初值=f2 数据 0x00) */
extern uint64_t DAT_14013ca90;
;
;
/* @0x14013ca90 惰性 LPCWSTR 槽 (cmd.exe 路径缓存, 运行时初始化) */
extern short DAT_14013ca4c;
;
;
/* @0x14013ca4c cmd.exe 路径长 (short, 运行时赋值) */
/* ---- 前置声明 (恢复体专用; 定义见各 core_*.c / 本文件后部) ---- */
extern WCHAR *g_pNextCmd;
/* DAT_14013cac8 (core_globals.c) */
extern void *PECMD_StrBldCopyWide(void *a, const WCHAR *b);
/* def @4100 */
extern longlong *PECMD_AppendWideStr(void *param_1p, LPCWSTR param_2);
/* def @3844 */
extern uint64_t *PECMD_ResetSlots(uint64_t *p);
/* core_b1_remaining.c */
extern uint64_t PECMD_ParseIntegerString(long long *ps, uint64_t *out);
/* def @3956 */
extern int PECMD_AsciiPrefixICmp(const char *s, const WCHAR *w, int n);
/* def @3722 */
extern int64_t *PECMD_SkipTokenWs(int64_t *pp);
/* core_b1_remaining.c:3271 */
extern int64_t PECMD_RunCommand(void *script, WCHAR *cmdline);
/* @0x140031454 def core_scriptrun.c:71 */
extern uint16_t *PECMD_AllocStrSlot(uint16_t **out);
/* def @本文件后部 */
extern long long *PECMD_SkipLeadingControlChars(long long *);
/* def @3944 */

extern WCHAR *PECMD_StrCopyW(WCHAR **ps, LPCWSTR src, int64_t len);
/* core_var2.c */
extern uint64_t PECMD_RunCommandLine(void *script, WCHAR **str, int mode);
/* 定义本文件@653 (arity 0->3 修正, 返回保持 uint64_t) */
extern char PECMD_MatchTokenAdvance(char *tok, void *pp, int n);
/* def @4432 */
extern char PECMD_MatchAndPad(int64_t key, int64_t *pp, int len);
/* core_b3d.c */
extern bool PECMD_ParseNumSkipWs(WCHAR **pp, uint64_t *out);
/* core_remaining_helpers.c */
extern int64_t PECMD_ParseVarArg(int64_t *, int64_t *, int64_t *, int *, uint32_t *);
/* core_b1_remaining.c */
extern uint64_t PECMD_FindAliveProcessId(uint64_t);
/* core_b1_remaining.c */
extern uint16_t *PECMD_NextToken(int64_t *, int64_t *, uint32_t);
/* def @本文件 1605 */
extern void PECMD_ReleaseObjectResources(int64_t *p);
/* core_b1_remaining.c */
extern void PECMD_ExpandVarDispatch(void *p1, WCHAR *p2, void *p3, int p4, uint8_t p5);
/* def @4698 */
extern const uint8_t DAT_14011dd24[];
;
;
/* @0x14011dd24 真值 "catch" (rdata) — PECMD_MatchAndPad key */


extern uint64_t PECMD_MatchAssignToken(char *key, int64_t *pp, int len);
/* def core_b3f.c */
extern bool PECMD_ParseHexOrDecBool(long long *param_1, int *param_2);
/* def @5161 */
extern bool PECMD_ParseUIntValue(int64_t *pp, int *out);
/* core 定义 */
extern uint64_t PECMD_GetModuleFileNameIntoContainer(uint64_t *out);
/* core_b1_remaining.c */
extern uint64_t PECMD_ExpandPathAlloc2(LPCWSTR p1, uint64_t *p2, int64_t *p3);
/* def @6780 */
extern unsigned long GetModuleFileNameW(void *h, WCHAR *buf, unsigned long n);
/* def @6222 */
extern uint64_t SHGetSpecialFolderPathW(HWND, WCHAR *, int, BOOL);
/* def 本文件后部 */
extern uint64_t ReadFile(void *h, void *buf, unsigned long n, unsigned long *read, void *ol);
/* def 本文件后部 */
extern uint64_t StrCmpNIA(const char *a, const char *b, int n);
/* 桩 本文件后部 (arity 0->3) */
extern uint16_t g_privFlags;
/* DAT_14013e20c (core_globals.c) */
extern uint8_t g_flagCCC9;
/* DAT_14013ccc9 (core_globals.c) */
extern int64_t PECMD_EnableTokenPrivilege(LPCWSTR priv, DWORD attr, uint32_t flag);
/* def core_scriptrun 区 */
extern int64_t *PECMD_InitObjectWithParent(int64_t *ps, int64_t parent);
/* core_b1_remaining.c:9102 */
extern void PECMD_ExpandScriptVariables(int64_t *p1, int64_t *p2, int64_t *p3);
/* core_b3_remaining.c */
extern uint8_t *PECMD_MemMoveForward(uint8_t *dst, uint8_t *src, int n);
/* 既存声明@527; 桩=memmove 本文件后部 */
extern uint64_t PECMD_AddVarDefault(void *script, LPCWSTR name, LPCWSTR val, int len, int64_t flag);
/* 桩本文件后部 (arity 0->5) */
extern void PECMD_ClearTaskTable(uint64_t script, int mode);
/* def 本文件@6472 (undefined8→uint64_t) */
extern uint64_t PECMD_VarLookup(int64_t *a, const uint16_t *b, int64_t *c, int64_t d, void *e);
/* 既存声明@5072 */
extern uint64_t PECMD_NextRandomSeed(void);
/* @0x14005e04c */
extern void PECMD_GetApiProcCached(LPCSTR p1, LPCSTR p2, longlong *slot, longlong *hmod);
/* 桩 本文件后部 */
extern void PECMD_WideToAnsiStr(int64_t *ps, LPCWSTR src, int64_t len, uint64_t cap);
/* core_b3a.c */
extern uint64_t GetEnvironmentStringsW(void);
/* 桩 本文件后部 */
extern uint64_t FreeEnvironmentStringsW(LPWSTR env);
/* 桩 本文件后部 (arity 0->1) */
extern void Sleep(unsigned long ms);
/* 桩 本文件后部 */
extern uint64_t wsprintfA(char *out, const char *fmt, ...);
/* 桩本文件后部 (arity 0->可变) */
extern uint64_t CreateProcessWithLogonW(void *user, void *domain, void *pwd, DWORD flags, void *app,
                                        WCHAR *cmd, DWORD flg2, void *env, void *dir, void *si,
                                        void *pi);
/* 桩本文件后部 (arity 0->11) */
extern BOOL PECMD_CreateProcessAsUser(LPCWSTR, LPWSTR, LPSECURITY_ATTRIBUTES, LPSECURITY_ATTRIBUTES,
                                      BOOL, uint32_t, LPVOID, LPCWSTR, LPSTARTUPINFOW,
                                      uint64_t /*LUID 值*/, uint32_t, LPCWSTR, LPCWSTR, LPCWSTR);

extern short *PECMD_ResolveWildcardPath(uint64_t *p1, short *p2, LPWSTR p3, LPCWSTR p4);
/* core_b1_remaining.c:2348 */
extern bool PECMD_IsFile(LPCWSTR path);
/* def core_b1_remaining.c:2398 */
extern void *PECMD_GrowByteBuffer(void **ps, int64_t len);
/* def core_thread.c:28 */
extern LPCWSTR PECMD_ParseResourceStringRef(LPCWSTR *p1, uint p2);
/* def 本文件@6995 */
extern uint64_t PECMD_ParseItemImageSpec(longlong *a, longlong *b, uint16_t *c);
/* 桩 本文件@7365 */
extern void *DAT_14013cf70;
/* HINSTANCE 槽 (def 行~12531) */
extern uint64_t LoadLibraryExW(LPCWSTR path, void *file, uint32_t flags);
/* 既存声明@7395 前移 */
extern uint64_t FreeLibrary(void *h);
/* 既存声明@7394 前移 */
extern uint8_t *PECMD_LoadEncodedResource(void *a, uint16_t *b, uint16_t *c, int64_t *d,
                                          unsigned int *e);
/* def 本文件@3823 */
extern uint64_t PECMD_SkipWCharUntil(void *pp, uint16_t ch);
/* 桩本文件后部 (arity 0->2) */
extern int64_t *PECMD_AssignString(int64_t *ps, LPCWSTR src);
/* @0x14007034c core_b1_remaining.c */
extern int PECMD_CreateProcessW(LPCWSTR cmd, WCHAR *buf, LPSECURITY_ATTRIBUTES sa,
                                LPSECURITY_ATTRIBUTES da, BOOL inherit, DWORD flags, LPVOID env,
                                LPCWSTR cwd, STARTUPINFOW *si, PROCESS_INFORMATION *pi);
/* @0x140101e04 core_b1_remaining.c */
extern BOOL PECMD_LaunchServiceProcess(LPCWSTR a, LPCWSTR b);
/* def core_b1_remaining.c:875 */
extern int PECMD_IsSysStartuped(void);
/* def core_b2a.c:341 */
extern bool FUN_140101e70(const WCHAR *name);
/* 真体本文件后部 (签名修正: bool(LPCWSTR), 同 core_b3_remaining:652) */
extern uint64_t PECMD_GenerateTimeText(LPCWSTR p1, int64_t *p2, uint64_t p3, uint64_t p4,
                                       uint64_t p5);
/* def core_b3_remaining.c:7183 */
extern HANDLE PECMD_CreateTempMutexDir(int64_t *p1, int64_t *p2, uint64_t *p3, uint64_t name);
/* def core_b1_remaining.c:3462 */
extern void PECMD_FlushFileThrice(HANDLE hFile);
/* def core_b3b.c:217 */
extern void *PECMD_DriverInstall(int64_t *script, LPCWSTR line);
/* def core_b3_remaining.c:1658 (返回 void*) */
extern uint64_t WaitForSingleObject(void *h, uint64_t ms);
/* 桩本文件@9717 */
extern uint64_t WriteFile(void *h, void *buf, unsigned long n, unsigned long *written, void *ol);
/* 桩本文件后部 */
extern uint64_t OpenProcess(DWORD access, BOOL inherit, DWORD pid);
/* 桩本文件后部 */
extern uint64_t ResumeThread(void *hThread);
/* 桩本文件后部 (arity 0->1) */
extern uint64_t CreateProcessW(LPCWSTR app, const uint16_t *cmd, LPSECURITY_ATTRIBUTES sa,
                               LPSECURITY_ATTRIBUTES ta, BOOL inh, DWORD flags, void *env,
                               LPCWSTR dir, void *si, void *pi);
/* 桩本文件后部 (arity 0->10) */
extern uint64_t AssignProcessToJobObject(void *job, void *proc);
/* 桩本文件后部 (arity 0->2) */
extern uint64_t CreateJobObjectW(LPSECURITY_ATTRIBUTES sa, LPCWSTR name);
/* 桩本文件后部 (arity 0->2) */
extern uint64_t CreateProcessAsUserW(void *tok, LPCWSTR app, WCHAR *cmd, LPSECURITY_ATTRIBUTES sa,
                                     LPSECURITY_ATTRIBUTES ta, BOOL inh, DWORD flags, void *env,
                                     LPCWSTR dir, void *si, void *pi);
/* 桩后部 (arity 0->11) */
extern uint64_t GetShortPathNameW(LPCWSTR path, WCHAR *buf, DWORD len);
/* 桩本文件后部 (arity 0->3) */
extern void PECMD_WriteSysAck(uint32_t ack, int mode);
/* def core_b2b.c:1254 */
extern int PECMD_ReadRegBinaryGuarded(HKEY hkey, LPCWSTR path, LPCWSTR name, longlong *buf,
                                      LPDWORD size, long *extra);
/* def 本文件@16216 同型 (LSTATUS≡long, typedef 在后) */
extern void PECMD_TlsLogWrite(uint64_t ctx, LPCWSTR fmt, uint64_t a, uint64_t b);
/* @0x140018d8c */
extern uint64_t PECMD_NotifyMainWindowRefresh(uint64_t a, int b);
/* 既存声明@6298/def@8341 */
extern void PECMD_RunSysInit(void *script, LPCWSTR name);
/* def core_scriptdep.c:166 */
extern LARGE_INTEGER PECMD_ProcessScriptBlock(LARGE_INTEGER p1, LARGE_INTEGER p2, longlong *p3,
                                              longlong *p4, pthreadmbcinfo p5);
/* def 本文件@6341 */
extern WCHAR *PECMD_ResolveVariable(int64_t *a1, LPCWSTR a2, uint64_t *a3, uint32_t a4);
/* def 本文件@8621 */
extern void PECMD_StartOnlyApp(LPCWSTR cmdline);
/* def core_b2b.c:844 */
extern HWND g_hwndC9C8;
/* DAT_14013c9c8 (core_globals.c:529) */
extern uint8_t DAT_14013cb09;
;
;

extern uint64_t MsgWaitForMultipleObjects(DWORD count, HANDLE *handles, BOOL waitAll, DWORD timeout,
                                          DWORD mask);
/* 桩本文件后部 (arity 0->5) */
extern uint64_t WaitForInputIdle(HANDLE proc, DWORD ms);
/* 桩本文件后部 (arity 0->2) */
extern int32_t lstrlenA(const char *s);
/* 桩本文件@10077 */
extern void SetLastError(DWORD e);
/* 桩本文件@9957 */
extern uint64_t SetProcessWorkingSetSize(void *h, uint64_t a, uint64_t b);
/* 桩本文件@9963 */
extern int64_t PECMD_AdjustTokenPrivilege(LPCWSTR priv, DWORD attr, HANDLE h);
/* def core_b1_remaining.c:8038 */
extern HANDLE PECMD_RestrictedTokenSetup(HANDLE base);
/* def core_b1_remaining.c:2311 */
extern void PECMD_VarSetUInt(void *s, uint64_t v, LPCWSTR k);
/* core_b2f.c:101 extern 同型 */
extern void PECMD_WrapParamCall_02d8(int64_t *obj, uint64_t value, LPCWSTR text);
/* def core_b3a.c:66 */
extern void PECMD_ScheduleSelfDelete(LARGE_INTEGER ft, int pid);
/* def core_b1_remaining.c:1224 */
extern uint64_t DAT_14013c960;
;
;

extern int GetCursorPos(void *p);

/* @0x14011dcb0 真值: 2a 00 00 00 "ppid:" 00 00 00 "hpid..." (仅取址) */


extern uint64_t PECMD_OpenProcessMemory(void *param_1, DWORD param_2, long long param_3);
/* core_b1_remaining.c */
extern uint64_t PECMD_WaitHandlesOrMessages(uint64_t p1, int64_t p2, int p3, uint64_t *p4);
/* def @原832 */
extern uint64_t PECMD_LoadFileToSlot(LPCWSTR path, int64_t *pp);
/* core_b2f.c */
extern uint64_t GetCurrentProcess(void);
/* def @原6187 */
extern uint64_t DuplicateHandle(void *hSrcP, void *hSrc, void *hDstP, HANDLE *phDst, DWORD acc,
                                BOOL inh, DWORD opts);

extern uint64_t SetFilePointer(void *f, long dist, LONG *distHi, DWORD how);

extern HANDLE PECMD_OpenFileHandle(HANDLE *out, LPCWSTR path, DWORD access,
                                   DWORD share, /* core_exec2.c */
                                   LPSECURITY_ATTRIBUTES sa, DWORD disp, DWORD flags, HANDLE tmpl);

extern LPCWSTR PECMD_StripTrailingSpaces(LPCWSTR s);
/* @0x14011d108 真值 L".tmp" (rdata) */

extern HANDLE g_hStdIn;
extern HANDLE g_hStdOut;
extern HANDLE g_hStdErr;
/* def @本文件 2161 */


extern int64_t PECMD_EnumComMenuItems(uint64_t, uint64_t, int *, int64_t **, HMENU);
/* core_b1_remaining.c */
extern void *PECMD_AllocSmallObject(void **ps);
/* core_cmd4.c */
extern LPCWSTR PECMD_ConvertLfToCrlf(LPCWSTR a, int64_t *b);
/* core_b3_remaining.c */
extern int64_t PECMD_DispatchByObjectName(uint64_t *, LPCWSTR, int64_t, uint16_t *, uint64_t,
                                          uint64_t);
/* core_b3_remaining.c */
extern void PECMD_VarWriteLine(void *script, LPCWSTR key, LPCWSTR data, int64_t len, int64_t *pkey,
                               char mode);
/* core_remaining_helpers.c */
extern short *PECMD_UnquoteString(short *param_1);
/* def @2798 */
extern int64_t PECMD_FindVarValue(int64_t *, LPCWSTR, int64_t *, int);
/* core_b3_remaining.c */
extern int64_t *PECMD_ReplaceStringSlot(int64_t *ps, uint64_t *src);
/* core_b8m.c */
extern uint64_t PECMD_LoadFileMappingExec(LPCWSTR, int64_t *, int64_t *, HANDLE *, LPCWSTR);
/* core_b1_remaining.c */
extern WCHAR *PECMD_StrDupA(WCHAR **ps, LPCWSTR src, int64_t a, int64_t b);
/* def @3857 */
extern uint64_t FUN_140063060(uint64_t value);
/* def @6008 */
extern uint64_t PECMD_ResDecode(int64_t *ps, uint32_t flag);
/* core_b1_remaining.c */
extern void PECMD_ResetScriptChain(longlong *param_1, longlong *param_2);
/* def @13537 */
extern LPCWSTR PECMD_PinStartMenuTask(int64_t *, LPCWSTR);
/* core_b3_remaining.c */
extern unsigned long PECMD_RegSetValueWithOpen(void *, const unsigned short *,
                                               const unsigned short *, unsigned long,
                                               unsigned char *, unsigned long);


extern uint64_t GetExitCodeProcess(void *h, DWORD *code);
/* def @8611 (arity 修正 0->2) */
extern uint64_t TerminateJobObject(void *h, uint32_t code);
/* def @8873 */
extern uint64_t TerminateProcess(void *h, uint32_t code);
/* def @8874 */
extern uint64_t TrackPopupMenu(void *m, uint32_t f, int x, int y, uint32_t r, void *w, void *rc);
/* def @8876 */
extern int wsprintfW(unsigned short *out, const unsigned short *fmt, ...);
/* PECMD_LangLookupById — 按参数序号在 argv 表中定位 VALUE (形如 "id=value|id=value|...")
   定位到匹配 id 时返回其值指针(跳过 '|' 前缀); 未命中返回 NULL.
   依赖: FUN_1400166b4 初始化 argv 表; PECMD_ParseNumTryWriteback 数值解析; lstrlenW. */
extern LPCWSTR DAT_14013ca78;
;
;
/* argv 表指针 (由 FUN_1400166b4 惰性初始化, 初 0) */
extern int64_t DAT_14013ca80;
;
;

/* PECMD_TlsLogWrite 前置: TLS 日志全局槽 (定义见后部 0x14013c934/0x14013cb18/0x14013cb41 区) */
extern unsigned long DAT_14013c934;

extern longlong DAT_14013cb18[8];

extern uint8_t DAT_14013cb41;

void *TlsGetValue(unsigned long idx);
extern uint8_t DAT_14013d270;
;
;
/* 一次性初始化标志 (静区, 初 0) */
extern void (*DAT_14013cb48)(...);
/* RtlInitUnicodeString 类槽 (reroute, 初 0) */
extern int (*DAT_14013cd30)(...);
/* ZwOpenKey 类槽 */
extern int (*DAT_14013cd38)(...);
/* ZwQueryKey 类槽 */
extern void (*DAT_14013cd40)(...);

/* ---- R1 批: FUN_140025f10 (PECMD_AppendLogMessage) 还原依赖前置声明/桩 ---- */
extern void PECMD_CrtShim(WCHAR *out, size_t fmt, ...);
/* core_b1_remaining.c 定义 (void SKIP 桩, wsprintf 阴影) */
extern void FUN_140063720(longlong **ps, longlong len);
/* def 本文件后部 5254 有 decl */
extern void FUN_1400185c8(char a, longlong *b);
/* def 本文件后部 (日志落盘) */
extern uint64_t QueryPerformanceCounter(void *out);
/* 本文件后部 def 改带出参签名 */
extern int64_t g_QPFreq;

/* ============================================================
 * PECMD_ScriptInitParse @0x140026338 (size=4875) — 命令串重定向/日志输出控制 (decompiled.c 直移)
 *   signature: ulonglong __fastcall PECMD_ScriptInitParse(longlong * param_1, LPCWSTR param_2,
 * pthreadmbcinfo param_3) 按现桩签名 (uint64_t/uint64_t/uint64_t) 表达, 内部重绑定为类型化别名.
 * ============================================================ */
/* ---- 移植前置: 类型 / 全局符号 / 前置声明 ---- */
typedef struct {
    SHORT X, Y;
} COORD;

typedef struct {
    SHORT Left, Top, Right, Bottom;
} SMALL_RECT;

extern int32_t g_msgLockCount;
/* DAT_14013d080 (core_globals.c) */
extern uint8_t g_u8CCB1;
/* DAT_14013ccb1 (core_globals.c) */
extern uint8_t g_flagD6F6;
/* DAT_14013d6f6 (core_globals.c) */
extern int64_t g_i64CCB8;
/* DAT_14013ccb8 (core_globals.c) */
extern uint8_t DAT_14011c638[64];
/* 默认命令串 (定义见本文件后部) */
int wsprintfW(unsigned short *out, const unsigned short *fmt, ...);
/* 定义见后部 (def @3623) */
int FUN_140003864(void *a, const uint16_t *b, uint32_t c, uint32_t d, void *e, uint32_t f,
                  uint32_t g, void *h);
/* def @6340 (int 返回: 1062 行隐式调用在先) */
unsigned short *lstrcpyW(unsigned short *dst, const unsigned short *src);
/* def 见后部 */
COORD GetLargestConsoleWindowSize(void *h);
/* 新增桩 (定义紧随本函数之后) */
uint64_t GetVolumeNameForVolumeMountPointW(void *mount, void *buf, unsigned long n);
/* def 见后部 */
uint64_t SetFilePointerEx(void *h, longlong off, longlong *out, unsigned long method);
/* def 见后部 */
void FUN_1400185c8(char a, longlong *b);
/* def @7358 */
extern void *PECMD_StrBldCopyWide(void *a, const WCHAR *b);
/* def @3058 */
void FUN_140063720(longlong **_ps, longlong len);
/* def @6922 */
undefined8 FUN_14005c788(char *a, ushort *b, int c);
/* def @7354 */
longlong *PECMD_AppendWideStr(void *param_1p, LPCWSTR param_2);
/* def @2802 */
char PECMD_MatchTokenAdvance(char *tok, void *pp, int n);
/* def @2866 */
void PECMD_StrAssign(uint16_t **ps, const uint16_t *src);
/* def @4836 */
long long PECMD_ExpandCommandLine(long long *a, WCHAR *b, void *c, int d, uint8_t e);
/* def @3129 */
uint64_t PECMD_EvalParenthesizedExpr(int64_t *pp, uint64_t *out);
/* 真实体 core_b7a.c */
void *FUN_14001E5B0(void *script, LPCWSTR name, LPCWSTR value, int namelen, int64_t caplen);
/* core_var2.c */
uint8_t *FUN_14001E69C(void *script, LPCWSTR name, void *scope, int64_t len);
/* core_var3.c */
DWORD FUN_14006459C(LPCWSTR src, uint32_t buflen, LPWSTR buf, LPWSTR *last);
/* core_exec2.c */
int64_t PECMD_DescribePartitionInfo(int64_t *param_1, LPCWSTR param_2, uint64_t param_3,
                                    uint64_t param_4, LARGE_INTEGER *param_5, uint32_t *param_6,
                                    char param_7);
/* core_b3_remaining.c */
void FUN_140060A94(uint64_t flags);
/* core_b3j.c */
uint64_t *PECMD_InitStringObj(uint64_t *obj);
/* core_b1_remaining.c */
int32_t FUN_14005C7C4(const char *a, const uint16_t *b);
/* 真实体 core_exec5.c */
undefined8 *FUN_14001877C(longlong *ps, int count);
/* 新增最小桩 (PECMD_ExpandSpecialDirs 恢复体: 作线程入口指针) */
/* ========== PECMD_ExpandSpecialDirs @ 14002cc30  size=1803 — 环境变量设置命令 (SETENV 语义)
 * ========== decompiled.c @26728 忠实移植: '^' 前缀跳过 → 缓冲分配 (lstrlenW+0x400) →
 * '$'(系统)/'#'(用户) 作用域前缀解析 → 星号/'*' 与 '-' 修饰 (local_78) → 空命令时按 param_4==0/0x23
 * 走重画标志 (local_88=0x10), 否则枚举 8 个特殊目录 (PTR_u__Favorites_14011e6f8) 以 "%s="(DAT 真值
 * 0x1401210e8) 构造 "*<名>=<路径>", CSIDL==0x1a (APPDATA) 时补 "\\Microsoft\\Internet
 * Explorer\\Quick Launch" 并建目录, 写 "HKCU\Environment\" 槽与 &名=路径 变量, 递归本函数 →
 * param_5 修饰串解析 (~ / ~~ / ~. / ~* 等修饰, * , -env/-raw/-ex) → '=' 拆分 → 变量名展开
 * (PECMD_ExpandVarsRecursive / PECMD_ExpandCommandLine) → 按 bVar15 模式经
 * PECMD_ApplyVarWriteModifiers/ PECMD_ExpandVarsLocked 重写值 → SYSTEM\...\Environment
 * (local_74=0x24) / HKCU\Environment (local_70=0x23) 注册表写入
 * (RegDeleteValue/RegSetValueWithOpen) → SetEnvironmentVariableW → 收尾: local_78=='-' → 起线程
 * PECMD_WaitOtherThreadsExit; 否则非 '*'-前缀且重画标志 → PECMD_BroadcastEnvChange / g_afterMain.
 *
 * 关键取舍:
 *  - CONCAT22(x._2_2_,v) 等价为 (x&0xffff0000u)|v; Ghidra 栈地址垃圾 (local_74=返回&0xffff0000,
 *    uVar11=&local_38&0xffffffffffff0000) 按 0 建模 — 高位从未被读, 仅 (short)x 低位参与判定,
 *    行为逐点等价.
 *  - PECMD_AllocWStringBuffer 为 void 桩 → 反编译 local_74=返回值 按 0.
 *  - PECMD_CrtShim 为 void SKIP 桩 → 反编译 iVar5=返回值(写字符数) 按格式 "%s=" 实算
 *    wcslen(name+2)+1, 使 SHGetSpecialFolderPathW 续写偏移与原等价.
 *  - 特殊目录表二进制布局每 16B=[LPCWSTR name][int CSIDL][int pad], 代码以 (p-8) 读 CSIDL
 *    (即上一槽); pe_data_extract 真值配对: &Favorites=0x06 &Desktop=0x10 &StartMenu=0x0b
 *    &Startup=0x07 &Programs=0x02 &SendTo=0x09 &Personal=0x05 &QuickLaunch=0x1a(APPDATA).
 *  - 修饰串循环中 Ghidra 冗余的 if(*pWVar8=='~') 重查为恒真, 去冗余保持同为真.
 *  - PECMD_ExpandVarsLocked 无实现 → 新增签名空桩 (@0x14008569c size=326).
 * 签名保持桩 (arity 修正 0->5); 调用点: PECMD_ScriptMainEntry 恢复体 @6900 (a, DAT_14011c638,
 * 0, 0x24, 0) / core_b2d.c PECMD_ProcessInitCommand (ctx, local_178, 0, 0x24, 0). */
typedef struct {
    const uint16_t *name;
    int csidl;
} EC_FavEntry;

extern uint16_t *wcscat(uint16_t *dest, const uint16_t *src);

extern uint32_t g_afterMain;
/* DAT_14013ccc8 (core_globals.c) */
extern uint8_t g_flagCCB3;
/* DAT_14013ccb3 (core_globals.c) */
extern DWORD g_dwC96C;
/* DAT_14013c96c (core_globals.c) */
extern uint64_t PECMD_CreateDirectoryTree(LPCWSTR path);
/* @0x14001e19c (core_b1_remaining.c) */
extern LPCWSTR PECMD_ApplyVarWriteModifiers(int64_t *, int64_t *, uint16_t *, int64_t *);
/* @0x140084a5c SKIP 桩 (core_b3_remaining.c) */
extern void *PECMD_CheckVarMultiLevelRef(int64_t *, LPCWSTR);
/* @0x1400681ec (core_b3_remaining.c) */
extern uint64_t *PECMD_ExpandVarsLocked(uint64_t *, uint64_t *, uint8_t);
/* @0x14008569c SKIP 桩 (本函数后部定义) */
extern uint64_t PECMD_RegiEditRegistry(uint64_t, uint64_t, uint64_t);
/* @0x1400c13f8 (后部定义) */
extern HANDLE PECMD_CreateDetachedThread(LPTHREAD_START_ROUTINE, LPVOID, size_t, DWORD, LPDWORD,
                                         LPSECURITY_ATTRIBUTES);
/* @0x14005b228 (后部定义) */
extern unsigned long PECMD_RegSetValueWithOpen(void *, const unsigned short *,
                                               const unsigned short *, unsigned long,
                                               unsigned char *, unsigned long);
/* @0x14005c5a0 (后部定义) */
extern DWORD PECMD_RegDeleteValue(HKEY, LPCWSTR, LPCWSTR);
/* @0x14005c61c (后部定义) */
extern long long PECMD_ExpandVarsRecursive(long long *, WCHAR *, void *, int, uint8_t);
/* @0x14007bda8 (后部定义) */
extern uint64_t SHGetSpecialFolderPathW(HWND, WCHAR *, int, BOOL);

/* ============================================================
 * PECMD_ProcessScriptBlock @0x14004c0bc (size=8717) — 资源/脚本执行主分发 (decompiled.c 直移)
 * ============================================================ */
/* ---- 移植前置: 全局槽 / 跨文件真实体 / 本文件后部定义 / 新增最小桩 ---- */
extern uint8_t DAT_00000011;
/* @0x11 地址伪符号 (定义见本文件后部) */
int wsprintfW(unsigned short *out, const unsigned short *fmt, ...);
/* 定义见后部; 此处前置声明避免与隐式 int() 冲突 */
extern uint8_t g_charTableF;
/* DAT_14013a248 (core_globals.c) */
extern int64_t g_i64CCB8;
/* DAT_14013ccb8 (core_globals.c) */
extern WCHAR *g_pNextCmd;
/* DAT_14013cac8 (core_globals.c) */
extern int DAT_14013ccca;
;
;
/* @0x14013ccca 错误消息注册标志 (原无定义, 新增) */
LARGE_INTEGER FUN_14003C06C(int64_t *script, uint64_t cmd, uint32_t flags);

long long PECMD_ExpandCommandLine(long long *a, WCHAR *b, void *c, int d, uint8_t e);

uint64_t PECMD_NotifyMainWindowRefresh(uint64_t a, int b);

uint64_t PECMD_DispatchExpressionBlock(uint64_t a, uint64_t b);

uint64_t PECMD_DdCopyCommand(uint64_t a, uint64_t b);

uint64_t PECMD_ScriptMainEntry(uint64_t a, uint64_t b);

uint64_t PECMD_ParseCommandPath(uint64_t a, uint64_t b, uint32_t *c, int64_t *d);

ushort *FUN_140024c48(longlong *a, longlong *b, uint c);

undefined8 FUN_14005c788(char *a, ushort *b, int c);

longlong PECMD_TokPrefixICmp(char *param_1, ushort *param_2, int param_3);

ulonglong PECMD_EvalParenStripped(longlong *param_1, ulonglong *param_2);

longlong *PECMD_AppendWideStr(void *param_1p, LPCWSTR param_2);

longlong PECMD_CloseRestartByName(longlong *param_1, LPCWSTR param_2, longlong param_3,
                                  ulonglong param_4);

uint64_t PECMD_XorEncode(const uint16_t *a, uint32_t b, uint64_t c);

uint64_t PECMD_ExecCmdDispatch(uint64_t a, uint64_t b, uint64_t c, uint64_t d, uint64_t e,
                               uint64_t f, uint64_t g, uint64_t h);

uint64_t PECMD_ScriptInitParse(uint64_t a, uint64_t b, uint64_t c);

uint64_t PECMD_SetRegistryOwnerRun(int64_t param_1, char param_2);

uint64_t PECMD_WideStrLen(const uint16_t *s);

uint64_t PECMD_VarLookup(int64_t *a, const uint16_t *b, int64_t *c, int64_t d, void *e);

/* 新增最小桩声明 (定义紧随本函数之后) */
uint64_t PECMD_IsPecmdScriptFile(uint64_t);
uint64_t PECMD_UserCmdHandler(uint64_t);

uint64_t PECMD_LinkCreateShortcut(uint64_t, uint64_t);
uint64_t PECMD_CreatePageFile(uint64_t);

uint64_t PECMD_EvalSpecialToken(uint64_t, uint64_t, uint64_t, uint64_t, uint64_t);

uint64_t PECMD_ProcessInitCommand(uint64_t, uint64_t);
uint64_t PECMD_FontCommand(uint64_t, uint64_t, uint64_t, uint64_t);

uint64_t PECMD_DispCommand(uint64_t, uint64_t);
uint64_t PECMD_SendInputEvents(uint64_t);

uint64_t PECMD_DownCheckShutdown(void);
uint64_t PECMD_ShutPowerAction(uint64_t);

uint64_t PECMD_MstrStringOps(uint64_t, uint64_t);
uint64_t PECMD_EvalRandFunction(uint64_t, uint64_t);

uint64_t PECMD_MessShowMsgBox(uint64_t, uint64_t, uint64_t);

uint64_t PECMD_HashCmdCompute(uint64_t, uint64_t, uint64_t);
uint64_t PECMD_RegiEditRegistry(uint64_t, uint64_t, uint64_t);

uint64_t PECMD_HomeSetStartPage(uint64_t, uint64_t);

uint64_t PECMD_SpinAddControl(uint64_t, uint64_t, uint64_t, uint64_t);

uint64_t PECMD_SbarAddControl(uint64_t, uint64_t, uint64_t, uint64_t);

uint64_t PECMD_TablCreateControl(uint64_t, uint64_t, uint64_t);

uint64_t PECMD_PartShowHideDrive(uint64_t, uint64_t);

uint64_t PECMD_SiteSetFileAttrTime(uint64_t, uint64_t, uint64_t);
uint64_t PECMD_GetfReadData(uint64_t, uint64_t);

uint64_t PECMD_DiskFindLoadHives(uint64_t, uint64_t);

uint64_t PECMD_MounMountWimImage(uint64_t, uint64_t, uint64_t);
uint64_t PECMD_SockCreateIpcObj(uint64_t, uint64_t, uint64_t);


longlong FUN_140003a20(longlong *param_1, undefined8 *param_2, byte param_3);
/* OLE32 API 槽 (R1 批新增, PECMD_LoadOle32Apis 真体依赖; 运行时槽按纪律置 0+判空,
   由 PECMD_GetApiProcCached("xxx","OLE32",槽,&DAT_14013cfe0) 装载; DAT_14013d800/808/820
   已有定义见上). 注: core_globals.c 以别名持有部分同址槽 (g_pCoCreateInstance=14013d830 等),
   本文件沿 DAT_ 直名先例 (同 433-436 行) 自持存储. */
extern longlong DAT_14013cfe0;
;
;
/* g_hOle32 模块句柄槽 */
extern longlong DAT_14013d810;
;
;
/* CoInitializeEx */
extern longlong DAT_14013d818;
;
;
/* CoInitialize */
extern longlong DAT_14013d828;
;
;
/* CoInitializeSecurity */
extern longlong DAT_14013d830;
;
;
/* CoCreateInstance */
extern longlong DAT_14013d838;
;
;
/* CreateStreamOnHGlobal */
extern longlong DAT_14013d840;
;
;
/* StringFromGUID2 (兼装载完成标志) */
extern longlong DAT_14013d848;
;
;
/* CoLoadLibrary */
extern longlong DAT_14013d850;
;
;
/* CoFreeLibrary */
extern longlong DAT_14013d858;
;
;

/* R1 批前置声明: PECMD_SprintfRetEnd 真体(桩)在本文件后部 10337 */
extern const uint16_t *PECMD_SprintfRetEnd(const uint16_t *a, uint64_t b, const uint16_t *c);

extern void FUN_1400629b8(void *a, const WCHAR *b, const WCHAR *c);

/* R1 批前置声明 (真体在本文件后部/其它 TU): StrBld 追加族 */
extern void FUN_140063888(void *a, const WCHAR *b, int64_t c);
/* def @本文件后部 14711 (已有 decl@456) */
extern longlong *FUN_1400637dc(longlong *a, LPCSTR b, ulonglong c, ulonglong d);

/* ========== PECMD_DispatchExpressionBlock @ 1400b1724  size=1733 ==========
 * signature: undefined8 __fastcall PECMD_DispatchExpressionBlock(LARGE_INTEGER param_1, ulonglong
 * param_2) 忠实移植 decompiled.c @110300; 外部签名保持桩 (uint64_t,uint64_t) (arity 修正 0->2),
 * 体内用局部 param_1/param_2 对齐 Ghidra 变量 (x64 寄存器传参等价: param_1.QuadPart≡a,
 * param_2≡b).
 * 内部调用按 rename_map: FUN_14006156c→PECMD_MatchPattern, FUN_1400a53e4→
 * PECMD_TokenizeExpression, FUN_14001b23c→PECMD_ExtractTableSegment, FUN_14006e030→
 * PECMD_CopyPathToken(桩 arity 0→4 修正), FUN_14004c0bc→PECMD_ProcessScriptBlock;
 * PECMD_InitObfuscatedKeywords/PECMD_CheckFirstStartupFlag/FUN_140063060/PECMD_StrBldCopyWideN/PECMD_FreeStrBuf
 * 保持原名. DAT_14013d080→g_msgLockCount, DAT_14013d058→g_sysinitState, DAT_14011c638 用符号. 取舍:
 * PECMD_CheckFirstStartupFlag(undefined8) 调用传 param_1.QuadPart (Ghidra 将 LARGE_INTEGER
 * 按单寄存器传参); (LPCWSTR)lpStr1 结构体强转指针非法 → (LPCWSTR)(uintptr_t)lpStr1.QuadPart.
 * 注意: 本函数自身不执行, 由 PECMD_ProcessScriptBlock 内部 / core 调用方经
 * uint64_t 签名进入; 返回 0 (decompiled 亦 return 0). */
/* ---- PECMD_DispatchExpressionBlock 恢复体所需前置声明 (定义见 core_*.c / 本文件后部桩) ---- */
extern uint64_t PECMD_MatchPattern(int16_t *pattern, int64_t ctx);
/* @0x14006156c core_b3c.c */
extern LPCWSTR PECMD_ExtractTableSegment(int64_t, uint64_t *, uint16_t *, int64_t *, char);
/* @0x14001b23c core_b1_remaining.c */
extern int64_t PECMD_TokenizeExpression(LARGE_INTEGER, int64_t, int64_t *, uint32_t, WCHAR *);
/* @0x1400a53e4 core_b3_remaining.c */
extern uint64_t PECMD_CopyPathToken(longlong a, longlong *b, longlong *c, longlong d);
/* @0x14006e030 (定义见后部桩) */
extern uint64_t FUN_140063060(uint64_t value);
/* @0x140063060 core_b3_remaining.c */
extern void PECMD_InitObfuscatedKeywords(void *script, uint64_t seed);
/* @0x14006159c 真体 core_exec2.c */
extern void PECMD_CheckFirstStartupFlag(undefined8 param_1);
/* @0x1400251ac 新增桩 (本文件) */
extern int32_t g_sysinitState;
/* 行尾整理 (leaf stub) */
extern void (*DAT_14013d380)(void *h, long long len);
/* SetFileValidData 装载槽 (未装载→跳过) */
/* ---------- PECMD_DdCopyCommand 调用面前置声明 (定义在本文件后部/core 文件, 避免隐式 int 截断)
 * ---------- */
extern HANDLE PECMD_OpenFileHandle(HANDLE *out, LPCWSTR path, DWORD access, DWORD share,
                                   LPSECURITY_ATTRIBUTES sa, DWORD disp, DWORD flags, HANDLE tmpl);

int PECMD_ParseHexOrDec();

uint64_t PECMD_MatchAssignToken(char *key, int64_t *pp, int len);
/* def core_b3f.c */
WCHAR *PECMD_ResolveVariable(int64_t *param_1, LPCWSTR param_2, uint64_t *param_3,
                             uint32_t param_4);
/* def core_b3_remaining.c */
void PECMD_TrimWhitespace(uint16_t *s);
/* def core_b1_remaining.c */
uint32_t PECMD_FindFileOrDir(LPWSTR param_1, uint32_t param_2);
/* def core_b9_remaining.c */
LARGE_INTEGER PECMD_SetFilePointer(HANDLE h, LARGE_INTEGER dist, DWORD method);
/* def core_remaining_helpers.c */
int64_t PECMD_GetDeviceSize(HANDLE h, uint8_t mtype);
/* def core_exec4.c */
uint64_t PECMD_ScanHexNumber(uint16_t **ps, uint64_t *acc, int *max);
/* def core_b3_remaining.c */
int64_t PECMD_ReadModifyWrite(HANDLE param_1, uint8_t *param_2, int64_t param_3, int param_4,
                              uint32_t param_5);
/* def core_b3r_c.c */
bool PECMD_WriteFileChunked(HANDLE hFile, const void *data, uint32_t len, int *written,
                            uint32_t minChunk);
/* def core_b3_remaining.c */
void FUN_14006355c(void *a, const WCHAR *b, int c, uint64_t d);

void *FUN_14005b374(void *a, int16_t b, int16_t c);

int FUN_140067cf4(long long *, uint64_t *);

WCHAR *PECMD_TokenizeQuotedField(longlong *param_1, longlong *param_2, longlong *param_3,
                                 WCHAR param_4, ushort param_5);

uint64_t PECMD_DeviceCheckReady(LPCWSTR p);

uint64_t FUN_140063344(void *p);

int PECMD_QueryDeviceIoInfo(uintptr_t h, int size);

void FUN_140102a90(uint64_t *dst, uint64_t v, uint64_t n);

uint64_t ReadFile(void *h, void *buf, unsigned long n, unsigned long *read, void *ol);

uint64_t WriteFile(void *h, void *buf, unsigned long n, unsigned long *written, void *ol);

uint64_t FlushFileBuffers(void *h);

uint64_t FreeLibrary(void *h);

uint64_t LoadLibraryExW(LPCWSTR path, void *file, uint32_t flags);

DWORD GetLastError(void);

int SetEndOfFile(void *h);

uint64_t SetFilePointerEx(void *h, longlong off, longlong *out, unsigned long method);

int32_t lstrlenA(const char *s);

extern void *DAT_14013cf70;
/* HINSTANCE 槽 (def 行~12531) */
#define CONCAT44(hi, lo) (((uint64_t)(uint32_t)(hi) << 32) | (uint32_t)(lo))


undefined4 PECMD_ModalDialogPump(longlong *param_1, ulonglong param_2);

extern int64_t (*DAT_14013d408)(HKEY, LPCWSTR, uint32_t, uint32_t);
extern uint64_t PTR_s_No_error_14013b800;
;
;
extern uint64_t PTR_DAT_14013a8f0;
;
;
extern uint64_t PTR_FUN_14011eb78;
;
;
extern uint64_t PTR_FUN_14011eb88;
;
;
extern uint64_t PTR_FUN_14011c410;
;
;
extern uint64_t PTR_DAT_14013bfe0;
;
;
extern uint64_t DAT_14013d660;
;
;
extern uint64_t DAT_14013d738;
;
;
/* @0x14007724c 自删除/延迟执行桩 */
extern uint64_t DAT_140147001;
;
;
extern uint64_t DAT_140147002;
;
;
extern uint64_t DAT_140147003;
;
;
extern uint64_t DAT_140147010;
;
;
extern uint64_t PTR_DAT_14013a250;
;
;
extern uint64_t PTR_FUN_1401234f0;
;
;
extern uint64_t PTR_FUN_140123540;
;
;
extern uint64_t PTR_FUN_140123560;
;
;
extern uint64_t PTR_FUN_140123580;
;
;
extern uint64_t PTR_FUN_1401235a8;
;
;
extern uint64_t PTR_FUN_140124e50;
;
;
extern uint64_t PTR_FUN_1401250d0;
;
;
extern uint64_t PTR_FUN_140126060;
;
;
extern uint64_t PTR_FUN_140126690;
;
;
extern uint64_t PTR_FUN_1401266c0;
;
;
extern uint64_t PTR_FUN_1401266e8;
;
;
extern uint64_t PTR_FUN_140126708;
;
;
extern uint64_t PTR_FUN_140126740;
;
;
extern uint64_t PTR_FUN_140126888;
;
;
extern uint64_t PTR_FUN_1401268a8;
;
;
extern uint64_t PTR_FUN_140128ed0;
;
;
extern uint64_t PTR_FUN_140129040;
;
;
extern uint64_t PTR_FUN_140129060;
;
;
extern uint64_t PTR_FUN_140129080;
;
;
extern uint64_t PTR_FUN_1401290a8;
;
;
extern uint64_t PTR_FUN_1401294f0;
;
;
extern uint64_t PTR_FUN_1401296e8;
;
;
extern uint64_t PTR_FUN_14012b1c0;
;
;
extern uint64_t PTR_FUN_14012b240;
;
;
extern uint64_t PTR_FUN_14012b430;
;
;
extern uint64_t PTR_FUN_14012bad0;
;
;
extern uint64_t PTR_FUN_14012bbf0;
;
;
extern uint64_t PTR_FUN_14012bd70;
;
;
extern uint64_t PTR_FUN_14012be90;
;
;
extern uint64_t PTR_FUN_14012bfb0;
;
;
extern uint64_t PTR_FUN_14012c0e0;
;
;
extern uint64_t PTR_FUN_14012c2e0;
;
;
extern uint64_t PTR_FUN_14012c410;
;
;
extern uint64_t PTR_FUN_14012c550;
;
;
extern uint64_t PTR_FUN_14012c670;
;
;
extern uint64_t PTR_FUN_14012c7b0;
;
;
extern uint64_t PTR_FUN_14012c900;
;
;
extern uint64_t PTR_FUN_14012ca50;
;
;
extern uint64_t PTR_FUN_14012cb90;
;
;
extern uint64_t PTR_FUN_14012ccc0;
;
;
extern uint64_t PTR_FUN_14012cde0;
;
;
extern uint64_t PTR_FUN_14012cf00;
;
;
extern uint64_t PTR__purecall_140126050;
;
;
extern uint64_t PTR_u_CallBackhWnd_14011e668_2_14013a288;
;
;
extern uint64_t PTR_u_Shell_14011ddb8_2_14013a290;
;
;
extern uint8_t GetCommandLineW_exref[8];
;
;
extern uint64_t PTR_FUN_140126080;
;
;
extern uint64_t PTR_FUN_1401272a8;
;
;
extern uint64_t g_alphaThreshold;
;
;
extern uint64_t g_bInitWin;
;
;
extern double g_dpiBase;
;
;
extern uint64_t g_flagC96A;
;
;
extern uint64_t g_flagD032;
;
;
extern uint64_t g_flagD6f7;
;
;
extern double g_fontMinus0;
;
;
extern double g_fontRound;
;
;
extern uint64_t g_hCallBackWnd;
;
;
extern uint64_t g_hMainMutex;
;
;
extern uint64_t g_initFlags;
;
;
extern uint64_t g_langLen;
;
;
extern uint64_t g_logEnter;
;
;
extern uint64_t g_logFlag;
;
;
extern uint64_t g_msgboxActive;
;
;
extern uint64_t g_msgboxFlag;
;
;
extern uint64_t g_pLangBuf;
;
;
extern uint64_t g_pMainArgStr;
;
;
extern uint64_t g_paintScale2;
;
;
extern uint64_t g_pfnIsAppThemed;
;
;
extern uint64_t g_savedVtblSlot;
;
;
extern uint64_t g_subWndProc;
;
;
extern uint64_t g_threadId;
;
;


/* ---- 补充桩：当前 core_*.c 仍未实现，先补最小可链接桩 (TODO(verify)) ---- */
#include <stddef.h>
/* arity 修正 0->2 (PECMD_ExecCmdDispatch 恢复体) */

/* 数据符号桩 */
extern uint64_t PTR_FUN_14011fb08[128];
extern uint64_t PTR_FUN_140120a50[128];
;
;
extern uint64_t PTR_FUN_140120a60[128];
;
;
extern uint64_t PTR_FUN_140120a70[128];
;
;
extern uint64_t PTR_FUN_14013a020[4];
;
;
extern uint64_t PTR_s_VK_LBUTTON_140124140[512];
;
;
extern uint64_t PTR_WndProc1_14013a008[4];
;
;
extern void *DAT_14013a858;
;
;
/* 数据 / 虚表 / API 槽符号桩 */
extern uint64_t _UNK_1401265b8;
;
;
extern uint64_t PTR_FUN_140125be0;
;
;
extern uint64_t PTR_FUN_1401261e0;
;
;
extern uint64_t Ordinal_418_exref;
;
;
/* B1 业务还原新增数据符号 */
extern uint64_t PTR_DAT_14013a040;
;
;
/* ---- B3 业务还原 (core_b3_remaining.c) 引用的未定义数据符号桩 ---- */
extern int32_t _DAT_14013d8a4;
;
;
extern uint8_t PTR_FUN_140125d00[8];
;
;
extern uint8_t DAT_14013d8a0[256];
;
;
extern uint8_t PTR_FUN_140126b20[8];
;
;
extern uint8_t s_ABCDEFGHIJKLMNOPQRSTUVWXYZabcdef_140124db0[128];
;
;
extern uint8_t PTR_FUN_1401268c8[8];
;
;

#include <stdarg.h>

/* ========== PECMD_SetControlGeom @ 0x1400db648 size=3525 ==========
 * 控件位置/尺寸设置 (decompiled.c 直移; 桩签名保持: (uintptr_t hwnd, uint16_t *s, int64_t p3,
 * int64_t *p4, uintptr_t p5, uint32_t color, int64_t p7); 调用方 core_b3_remaining.c extern 同形)。
 * 移植取舍:
 *  - CONCAT44(hi,lo) 双局部 (local_138/uStack_134, local_res8/uStackX_c 等) 按文件既定惯例
 *    合成 uint64_t 串槽; 各点仅低32位被读, 高位残留不读的 CONCAT44 拼接按低32位近似
 *    (同 core_b3_remaining.c / core_b3r_d.c 对 CONCAT44 noise 的处理)。
 *  - local_c8/local_b8/local_c0/local_80/local_40..local_78 为 Ghidra 死存储 (仅写不读)
 *    → 省略; 其中 SendMessageW(local_d8,0x466,5,lParam=param_1 八字节拷贝) 的效果保留
 *    (lParam 即 hwnd 值); 原 FUN_140102a90(&local_c8,0,0x34) memset 亦死 → 省略。
 *  - 原 GetWindowInfo(PWINDOWINFO local_d0) 结果未被再读 → 传哑缓冲。
 *  - PECMD_EvalParenExprRounded/PECMD_ParseNumberToken (数字解析, decompiled.c 有体但本 tree
 * 未移植) 新增桩, 按 PECMD_ParseHexOrDec 桩惯例返回 1 视为"已解析" (不修改 *param_2)。
 */
typedef struct tagWINDOWINFO {
    DWORD cbSize;
    RECT rcWindow;
    RECT rcClient;
    DWORD dwStyle;
    DWORD dwExStyle;
    DWORD dwWindowStatus;
    UINT cxWindowBorders;
    UINT cyWindowBorders;
    WORD atomWindowType;
    WORD wCreatorVersion;
} WINDOWINFO, *PWINDOWINFO;
extern uint8_t DAT_140127738[8];
;
;
extern uint8_t DAT_140127740[8];
;
;

extern int (*DAT_14013c970)(uint32_t, uint32_t, char *, int);

/* ========== PECMD_DrvLoadInstall @0x14002c634 size=424 ==========
 * "!=drvload ..." 驱动安装 (decompiled.c @26501 直移; 签名按本文件既定桩保持:
 * uint64_t PECMD_DrvLoadInstall(int64_t, void *, void *, int)).
 *   构造 "--wd:*\"<inf>\" !=drvload \"<inf>\"" 命令行 → PECMD_ExecCmdDispatch 执行,
 *   成功 (退出码 0) 且已登记安装时 PECMD_AppendKeyIfMissing 记录,
 *   末尾 FUN_140025f10 提示 "DrvLoad安装驱动【(%s)?】[%s]", 返回退出码.
 * 改名 (tools/rename_map.json): FUN_140063620→PECMD_AllocStrSlot,
 *   FUN_1400637dc→PECMD_StrDupA, FUN_14006375c→PECMD_AppendWideStr,
 *   FUN_140063720→PECMD_AllocString, PECMD_AllocWStringBuffer→PECMD_AllocWStringBuffer,
 *   FUN_140019da8→PECMD_AppendKeyIfMissing;
 * 保持原名: PECMD_ExecCmdDispatch / FUN_140025f10 / PECMD_FreeStrBuf (link_stubs 桩).
 * 取舍: 反编译 "--wd:*\"" 为 ANSI 字面量 (LPCSTR), 而项目 PECMD_StrDupA 声明
 *   LPCWSTR → (LPCWSTR) 强转保原值 (桩 no-op, 无运行时影响);
 *   DAT_14013d130 (脚本结构) 按本文件既有符号引用, DAT_14011c638 (默认命令串
 *   缓存) 同理; _snwprintf 第 3 参按本文件约定 (const WCHAR *) 强转.
 */
extern void PECMD_AppendKeyIfMissing(int64_t obj, LPCWSTR key, int len);

/* ========== PECMD_ToSysCopyFiles @ 14002b9ec size=1625 ==========
 * ToSys 复制到系统目录 worker (decompiled.c 直移):
 *   展开源路径 → 构造 INF / DRIVERS / SYSTEM32 三目标目录 (param_1+0x168 自定义基准或
 * %SystemRoot%), CopyFileW 主文件 (param_3&0x30000 时 PECMD_PatchInfDirectives), 尾部
 * FindFirstFileW/FindNextFileW 枚举源目录所有文件, 按 .INF/.SYS 扩展名分流复制. 改名
 * (tools/rename_map.json): PECMD_AllocWStringBuffer→PECMD_AllocWStringBuffer,
 *   FUN_14006459c→PECMD_ExpandDrivePath, FUN_14001d78c→PECMD_MemMoveForward,
 *   FUN_140021144→PECMD_PatchInfDirectives, FUN_140101db8→PECMD_FindFirstFileW,
 *   FUN_140103020→PECMD_WideStrLen.
 * 保持原名: FUN_140025f10 / PECMD_FreeStrBuf / FUN_140102a90 (link_stubs 桩).
 * 取舍: DAT_140120dd8 (源 "*" 通配串, 8 字节) 按 core_b2e.c 既有约定以裸地址引用;
 *       _wcsicmp 自补 2 参 extern — 真实体 @0x140103244 现为 0 参 no-op 桩, 链接取其符号.
 */
extern void PECMD_FindFirstFileW(HANDLE *ph, LPCWSTR path, WIN32_FIND_DATAW *fd);
/* @0x140101db8 (core_b9_remaining.c) */
extern void PECMD_PatchInfDirectives(LPCWSTR path);
/* @0x140021144 (core_b2d.c) */
extern int _wcsicmp(const WCHAR *a, const WCHAR *b);
extern uint8_t DAT_00000011;
;
;
extern char *PTR_s___disverify_14013a2c8;
;
;
extern char *PTR_s__AutoDisverify_14013a2c0;
;
;
extern uint16_t u__SystemRoot___140123190[64];
;
;
extern uint64_t PTR_FUN_14011cbe8;
;
;
extern void *DAT_14013ca68;
;
;
extern uint8_t DAT_140124100[64];
;
;
extern uint64_t DAT_14013a360;
;
;
/* 48位随机种子 */


/* DATA globals */
extern int64_t DAT_14013d870;
;
;

undefined8 FUN_1400748a0(HWND param_1, undefined8 *param_2);


/* PECMD_* 人类可读别名 (对应 FUN_<addr> 槽位; 委托到真实业务体) */
/* 真实业务体原型 (定义于 core_string.c / core_var.c / core_sys.c) */
WCHAR *FUN_14006375C(WCHAR **ps, LPCWSTR src);
/* @0x14006375c 字符串追加 */
void FUN_1400702B0(WCHAR **ps, LPCWSTR src);

WCHAR **FUN_14005B154(WCHAR **pp);
/* @0x14005b154 跳过空白 */
void FUN_1400629B8(void *script, LPCWSTR key, LPCWSTR value);
/* @0x1400629b8 设置变量 */
DWORD FUN_14005C394(HKEY root, LPCWSTR subkey, HKEY *out, REGSAM access, uint32_t opt);
/* @0x14005c394 注册表打开 */
DWORD FUN_14005C4E0(HKEY root, LPCWSTR subkey, LPCWSTR name, DWORD *type, BYTE *data, DWORD *size);
/* 控件尺寸计算 (no-op) */
/* --- PECMD_InitButtonState 直移还原 (decompiled.c @1400ef91c size=489,
 * 按钮状态初始化/内绕坐标/标志) --- 依赖: PECMD_InitImageHolder (core_b8d.c) /
 * PECMD_CreateStaticWindow (core_b8g.c) / PECMD_* 串工具; CONCAT44 双局部已按 rcPack[4] 数组展开
 * (同 core_b8k.c FUN_1400EF91C 既定模式). */
uint64_t *PECMD_InitImageHolder(uint64_t *param_1);
/* @0x1400efec8 图标对象初始化 (core_b8d.c) */
bool PECMD_CreateStaticWindow(int64_t *obj, LPCWSTR text, DWORD style, int *rect, HWND parent,
                              uint32_t id);
extern uint8_t PTR_FUN_140124f70[8];
;
;
extern uint8_t PTR_FUN_140125e20[8];
;
;
extern char s_0_0_0_0_140126958[16];
;
;

/* ========== PECMD_EnumProcessInfo @0x14002d708 size=3074 (主体直移) ==========
 * 枚举进程/输出进程信息表/按名匹配并终止 (与 core_b2f.c FUN_14002D708 同源还原;
 * 此桩按任务恢复真实体, 保持既有 5 参签名).
 * 还原取舍 (歧义自决):
 *  - Process32FirstW/NextW 补回被 Ghidra 丢弃的 PROCESSENTRY32W* 第二实参,
 *    堆栈槽 local_2c8/local_2c0/local_2a8/local_29c 按 PROCESSENTRY32W 字段
 *    (dwSize/th32ProcessID/th32ParentProcessID/szExeFile) 映射 (同 core_b2f.c).
 *  - NtQuerySystemInformation(2,...) 第三实参按 0x134 补齐 (同 core_b2f.c).
 *  - CONCAT71 位提取简化为 (mode & 0x80) != 0.
 *  - local_440 为 指针槽/长度 复用槽 (同 core_b2f.c int64_t).
 *  - wsprintfW("%lu...") 原反编译即缺 vararg 实参, 保持原样 (同 core_b2f.c).
 *  - DAT_14013d040/14013cd78/14013ca49/14013cb80/140121178 → core_globals.c
 *    既有 g_* 全局; 内部调用按 tools/rename_map.json 用 PECMD_* 名.
 */
/* 全局槽 (core_globals.c) */
extern void *g_pNtQuerySystemInformation;
/* DAT_14013d040 */
extern void *g_pGetProcessMemoryInfo;
/* DAT_14013cd78 */
extern uint8_t g_u8CA49;
/* DAT_14013ca49 */
extern int64_t g_QPFreq;
/* DAT_14013cb80 */
extern double g_dbl1178;
/* DAT_140121178 */
/* 内部 helper (core_b1_remaining.c / core_b8d.c / core_b9_remaining.c 定义) */
extern uint16_t *PECMD_FindLastPathSep(uint16_t *s);
/* @0x140017634 */
extern int8_t PECMD_GetProcessorCount(void);
/* @0x1400066e4 */
extern bool PECMD_CmpStrN(LPCWSTR a, LPCWSTR b, int len, int case_sens);
/* @0x14001a25c */
extern bool PECMD_IsProcessUser(LPCWSTR user, DWORD pid, int case_sens);
/* @0x14001a488 */
extern void PECMD_TerminateProcessById(DWORD pid, UINT exitCode, uint64_t *out);
/* @0x14001a510 */
extern uint16_t *PECMD_GetProcessModuleFile(DWORD pid, uint16_t *path);
/* @0x140006770 */
extern DWORD PECMD_ReadProcessCommandLine(DWORD pid, uint64_t *buf, int size);
/* @0x1400098dc */
extern LPWSTR PECMD_GetProcessUserName(DWORD pid, LPWSTR buf);
/* @0x14001a284 */
extern void PECMD_StrAppendFormat(int64_t *ps, double a2, LPCWSTR fmt);
/* @0x1400e6cbc (core_b8d.c) */
/* Win32 桩 (无参桩配带参调用, 按惯例修 arity; 定义见本文件后部) */
extern uint64_t GetCurrentProcessId(void);

extern uint64_t GetProcessTimes(HANDLE h, FILETIME *a, FILETIME *b, FILETIME *c, FILETIME *d);

extern uint64_t OpenProcess(DWORD access, BOOL inherit, DWORD pid);

extern uint64_t PathMatchSpecW(const uint16_t *a, const uint16_t *b);

extern uint64_t Process32FirstW(HANDLE h, PROCESSENTRY32W *pe);

extern uint64_t Process32NextW(HANDLE h, PROCESSENTRY32W *pe);

extern uint64_t SHGetSpecialFolderPathW(HWND hwnd, WCHAR *path, int csidl, BOOL fCreate);
/* 服务安装处理 */
extern uint32_t DAT_14013c9fc;
;
;
/* GetTickCount 结果槽 (PECMD.exe 静态清零区, 初 0) */
extern long long DAT_14013d130;
;
;

/* ---- PECMD_ScriptMainEntry 恢复体所需前置声明 (定义在文件后部/其它 core_*.c; 命名按 rename_map)
 * ---- */
void PECMD_InitDynamicImports(void);
/* @0x140017908 core_proc.c */
void PECMD_DebugScriptString(undefined8 param_1, wchar_t *param_2, undefined8 param_3,
                             undefined8 param_4);
/* def @8966 */
void PECMD_InitRamdataRegistry(uint param_1);
/* @0x14001b888 def @9064 */
void PECMD_RegisterHotkeyEntry(void);
/* @0x140022e94 def @9331 */
WCHAR *PECMD_TokenizeQuotedField(longlong *param_1, longlong *param_2, longlong *param_3,
                                 WCHAR param_4, ushort param_5);
/* def @9115 */
extern void PECMD_DebugOutput(uint64_t a, uint64_t b, uint64_t c, uint64_t d);
/* @0x140045868 core_b3d.c */
extern HANDLE PECMD_OpenFileHandle(HANDLE *out, LPCWSTR path, DWORD access,
                                   DWORD share, /* def core_exec2.c / 同款 extern @7282 */
                                   LPSECURITY_ATTRIBUTES sa, DWORD disp, DWORD flags, HANDLE tmpl);

extern HHOOK g_hMsgHook;
/* DAT_14013cf58 core_globals.c */
extern HWND DAT_14013cf78;
extern uint64_t _UNK_14012d1d0;
;
;
extern uint64_t PTR_FUN_140126540;
;
;
extern uint64_t PTR_FUN_140126560;
;
;

/* ========== PECMD_IndataCopyDirs @0x14003c06c size=2427 ==========
 * 文件操作命令: 解析 -simpleprogress/-progress/-file/-forceq/-force/-su/-q/-delme/-rd/-delay
 * 选项, 拆分 ';' 分组的 "源>目标" 列表, 按 wFunc(2=拷贝/1=移动/3=删除/4=改名)
 * 调用 SHFileOperationW/MoveFile/DeleteFile/PECMD_RunCommand 等执行。
 * 移植自 decompiled.c PECMD_IndataCopyDirs (仅编辑 link_stubs.c; 内部调用按 rename_map 命名)。
 * signature: LARGE_INTEGER __fastcall PECMD_IndataCopyDirs(longlong *param_1, LARGE_INTEGER
 * param_2, uint param_3)
 */
typedef struct {
    void *hwnd;
    UINT wFunc;
    LPCWSTR pFrom;
    LPCWSTR pTo;
    WORD fFlags;
    BOOL fAnyOperationsAborted;
    void *hNameMappings;
    LPCWSTR lpszProgressTitle;
} _SHFILEOPSTRUCTW;


/* PECMD_RunCommand/PECMD_SkipLeadingControls 在本TU已被更早的隐式 int() 声明(1635/1245行),
   此处继续隐式调用, 不再重复 extern 以避免 conflicting types。 */
extern void PECMD_AllocString();

extern void PECMD_AppendParamToken(long long *list, LPCWSTR token, char filter);

extern int64_t PECMD_DeleteDirectoryTree(LPCWSTR path, uint32_t flags);

extern int16_t *PECMD_LastPathSeparator(int16_t *path);

extern HANDLE PECMD_OpenFileHandle(HANDLE *out, LPCWSTR path, DWORD access, DWORD share,
                                   LPSECURITY_ATTRIBUTES sa, DWORD disp, DWORD flags, HANDLE tmpl);
extern uint64_t DAT_14013e2a8;
;
;
extern uint64_t g_pFontBase;
;
;
/* ============================================================
   ---- 新增叶依赖数据/桩 (FUN_140076b88 / FUN_140097714 恢复所需) ----
   ※ 本批仅添加真正缺失的 DAT_ 全局 与 缺失的 WinAPI 导入桩;
     内容按原二进制用法推定, 以待数据迁移后核对.
   ============================================================ */
/* DAT_140126e88 (4 WCHAR, "\\??\\" 符号链接前缀前段) — 与 g_u6426e70/e78/e80 同区 */
extern uint64_t u_____C__140126e88;
;
;
/* DAT_140126e90 (4 WCHAR: [盘符占位, ':', 0, 0] 符号链接模板后段, 低 16 位被盘符覆盖) */
extern uint64_t ram0x000140126e90;
;
;
/* "C:\\0%c:\\0" 前8字节(LE) */
/* DAT_14013d388 — RegSaveKeyExW 延迟加载函数指针槽 */
extern void *DAT_14013d388;
;
;
extern double DAT_1401237e0;
;
;
extern double DAT_1401237e8;
;
;
extern double DAT_1401237f0;
;
;
extern double DAT_1401237f8;
;
;
extern double DAT_140123800;
;
;
/* 跳过前导 数字后读后缀 */
extern double DAT_140124110;
;
;
/* 常数(解析) */
/* DAT_140124118 = 3.141592653589793 (π, 0x400921fb54442d18) — 来自 PECMD.exe .rdata */
extern double DAT_140124118;
;
;
/* DAT_1401263a0 = 10.0 (0x4024000000000000) */
extern double DAT_1401263a0;
;
;
/* DAT_1401261a0 = 1.0 (0x3ff0000000000000) */
extern double DAT_1401261a0;
;
;
extern double DAT_140126398;
;
;
/* 舍入阈值0.5 */
extern double DAT_140126390;
;
;
/* 小数缩放 */
extern double DAT_140121668;
;
;
/* 常数(解析) */

/* ============================================================
   ---- core_b3_remaining.c 本轮还原 (FUN_14005ce04/061dac/062bdc/
        072924/073934/077190/081238) 新增依赖桩/数据符号 ----
   ============================================================ */
/* GDI+ 渲染 (FUN_140062bdc) 所需函数指针槽 (no-proto, 初值 0) */
extern int64_t (*DAT_14013ce08)(void);

extern int64_t (*DAT_14013ce10)(void);

extern int64_t (*DAT_14013ce28)(void);

extern int64_t (*DAT_14013ce30)(void);
/* GDI+ 渲染默认色 / 插值矩阵数据 */
extern uint32_t DAT_14013a34c;
;
;
/* b1-tier deps — core_b1_remaining.c FUN_140006660 还原所需的最小数据槽 */
extern uintptr_t g_hFontE2B8;
;
;


/* ============================================================
   ---- core_b3_remaining.c 本轮还原 (PECMD_SetAclPermission 安全描述符/ACL 设置) 缺失
        Advapi32 安全 API 与内部 helper 桩 ----
   真实逻辑: GetNamedSecurityInfoW + BuildExplicitAccessWithNameW + SetEntriesInAclW +
   SetNamedSecurityInfoW 为 NTFS/注册表 ACL 授权; GetUserNameW/LookupAccountNameW 取当前用户 SID.
   此处以 no-op 桩保持签名, 使调用方编译/链接通过且控制流按返回码推进.
   ============================================================ */
typedef long LSTATUS;
/* ============================================================
   ---- core_b3_remaining.c 本轮还原 (PECMD_ExpressionArithmetic 计算器浮点运算分派) 新增
        常数与栈写入 helper 桩 ----
   ※ 真实值提取自 PECMD.exe .rdata/.text, 见各注释.
   ============================================================ */
/* DAT_140124120 = 1.5707963267948966 (π/2, 0x3ff921fb54442d18) */
extern double DAT_140124120;
;
;
/* DAT_1401268f0 = 180.0 (0x4066800000000000, 弧度↔角度转换) */
extern double DAT_1401268f0;
;
;
/* ============================================================
   ---- core_b3_remaining.c 本轮还原 (PECMD_PinStartMenuTask Win10 开始菜单/任务栏 PINT 固定)
        新增全局槽与缺失 helper/WinAPI 桩 ----
   ※ 全局槽初始值取自已提取 PECMD.exe 映像(静态清零区, 初值 0).
   ============================================================ */
extern uint32_t DAT_14013a848;
;
;
/* PINT 固定方向/标志(用户注册表缓存, 初 0) */
extern uint32_t DAT_14013e1f0;
;
;
/* PINT 一次性清理注册标志位 */
extern int64_t DAT_14013e1e8;
;
;
/* (后台清理槽, 无初始化) */
extern int64_t DAT_14013e1e0;
;
;
extern int64_t DAT_14013e1d8;
;
;
extern int64_t DAT_14013e1d0;
;
;
extern uint8_t DAT_14011c638[64];
;
;


extern int DAT_14013d7fc;

undefined4 PECMD_WindowRectHitTest(HWND a, POINT b);

void *PECMD_FindWindowByPid(longlong a);

void FUN_1400f172c(longlong *a, UINT b, ulonglong c, ulonglong *d, longlong e, int f,
                   undefined8 *g);

void PECMD_UpdateModifierKeys(void);
/* ---- wave-current p4 restore support globals + leaf stubs ---- */
extern int DAT_14013d7fc;
;
;
/* 消息派发 (leaf stub) */
/* 键状态刷新 (leaf stub) */

/* ---- wave-current support: e7758 deps ---- */
extern void (*DAT_14013e290)(void *, void *, int *);
/* ---- wave-current support: e4f14/e5120 deps ---- */
extern code *DAT_14013e238;
;
;
extern unsigned long DAT_14013c934;
;
;
/* 0x14013cb18 TLS 输出缓冲 (pe_data_extract: bc 4e 00 00 0c 50 13 00) */
extern uint8_t DAT_14013cb41;
;
;
/* 0x14013cb41 强制刷新标志 */
extern uint8_t DAT_14013d261;
;
;
extern LPCWSTR DAT_14013ca10;
;
;
/* 热键序号表初始化 (leaf stub) */
/* 整数值解析 (leaf stub) */
extern int (*DAT_14013cd48)(longlong, const unsigned short *, void *, void *, void *, void *);
/* 0x14013cd48 热键枚举回调槽 */
extern HWND DAT_14013cf78;
;
;
/* 0x14013cf78 主窗 HWND 槽 (pe_data_extract: 04 10 13 00 ...) */
extern ushort DAT_14013cf80;
;
;
/* 串替换 (leaf stub) */
extern longlong DAT_14013d4d8;
;
;
/* 缓冲复制 (leaf stub) */
extern void *DAT_14013cf70;
;
;
/* 工作线程入口 (leaf stub) */
extern void *DAT_14013e1f8;
;
;
/* 0x14013e1f8 事件槽 A */
extern void *DAT_14013e200;
;
;
/* 0x14013e200 事件槽 B */
extern void *DAT_14013e208;
;
;
/* 0x14013e208 缓冲句柄槽 */
extern uint64_t DAT_14013e268;
;
;
/* 0x14013e268 工作线程入口表 (pe_data_extract) */
extern uint64_t DAT_14013e280;
;
;
/* 0x14013e280 错误格式槽 (pe_data_extract) */
extern longlong DAT_14013e2a0;
;
;
/* 0x14013e2a0 当前接收方 */
extern longlong DAT_14013e298;
;
;
/* ---- wave-current support: 082520 ---- */
extern uint32_t DAT_14013e20c;
;
;

/* ==================================================================== */
/* ==== wave-INDIR (本轮独占): 三个 .rdata 指针槽间接可达巨函真体 ==== */
/*   FUN_140100ac4 @0x140100ac4 size=4631 (槽 RVA 0x12cf00)            */
/*   FUN_1400d9818 @0x1400d9818 size=7726 (槽 RVA 0x126b20)            */
/*   FUN_1400f6db0 @0x1400f6db0 size=8526 (虚表 PTR_FUN_14012c670 家族) */
/* 证据: tools/dead_stub_evidence.json (verdict=INDIR, 不可 SKIP)。     */
/* 布局约定:                                                            */
/*   - HDC 型对象指针为 undefined4 元素指针(Ghidra +k → 字节 4k; 对齐统计 */
/*     与 pecmd_offsets.h OBJ_HWND=0x20 双重验证), 展开写作 (o1 + k*4)。 */
/*   - param_3 同为 ×4 缩放, 保留 (uint32_t*)param_3 + k 形式。          */
/*   - Ghidra "指针盒"栈槽(local_138/local_120/local_118/tVar22 等)统一 */
/*     用 ulonglong 标量承载, 解引用处显式 (T*)(uintptr_t) 强转;         */
/*   - DAT_14012d058 真值 = L"@0x%p"                                    */
/*     (pe_data_extract: 5f 00 30 00 78 00 25 00 70 00 00)。            */
/* ==================================================================== */

/* ---- 依赖前置声明 (复用库内已有实现, 真体在其它 TU, 不重复定义) ---- */
extern uint64_t PECMD_FindTreeItemByPath(int64_t obj, WCHAR *path, uint64_t *out);
/* FUN_1400ff414 core_b8l.c */
extern int PECMD_TreeGetItemState(int64_t obj, uint64_t item, uint64_t *out, uint32_t mask);
/* FUN_1400feda4 core_b8f.c */
extern int PECMD_TreeGetItemStateEx(int64_t obj, uint64_t item, uint64_t *out);
/* FUN_1400fee24 core_b8e.c */
extern int64_t PECMD_TreeCountItemsRecursive(int64_t obj, LRESULT hItem, int64_t recurse);
/* FUN_1400fee94 core_b8f.c */
extern void PECMD_TreeSetItemInfo(int64_t obj, uint64_t a2, uint32_t a3, uint64_t a4, uint32_t a5,
                                  uint32_t a6, uint32_t a7, uint32_t a8, uint64_t a9, uint32_t a10);
/* FUN_1400ff20c core_b8g.c */
extern int64_t PECMD_BuildTreeIndexPathStr(int64_t obj, LRESULT first, int64_t *out);
/* FUN_1400ff2bc core_b8j.c */
extern int64_t PECMD_TreeCollectCheckedText(int64_t obj, int64_t *out, uint64_t hItem,
                                            int64_t mode);
/* FUN_1400ff5d0 core_b8l.c */
extern int64_t PECMD_TreeCollectItems(int64_t obj, int64_t *out, WPARAM hItem, uint64_t flags);
/* FUN_1400ff730 core_b8l.c */
extern int64_t PECMD_TreeScanCheckedSiblings(int64_t obj, int64_t *out, uint64_t hItem,
                                             uint64_t flags);
/* FUN_1400ff8a8 core_b8l.c */
extern uint64_t PECMD_TreeDumpItemsText(int64_t obj, uint16_t *path, int64_t *out, LPCWSTR prefix,
                                        LPCWSTR suffix, uint64_t flags);
/* FUN_1400ffb64 core_b8m.c */
extern void PECMD_ParseValStepNext(int64_t *a, uint64_t *b);
/* FUN_1400c44f4 core_b7a.c */
extern bool FUN_1400C1194(longlong *ps, ulonglong *out);
/* FUN_1400c1194 core_exec5.c(简化体) */
extern int64_t FUN_1401001c0(int64_t obj, int64_t *pp, uint64_t item, uint64_t flags);
/* 控件文本设置 core_b9_remaining.c 占位桩(他文件禁碰) */
extern HWND FUN_1400E5788(HWND w);
/* FUN_1400e5788 core_b8c.c */
int64_t PECMD_ControlWindowProc(uint64_t param_1, uint64_t param_2, uint64_t param_3,
                                int64_t *param_4);
/* FUN_1400e5b0c 本文件后部 */
extern HWND g_hwndCF78;
/* DAT_14013cf78 托盘消息窗口 */
void FUN_140066eac(longlong a1, int a2);
/* @0x140066eac 脚本块卸载 (最小桩后续波次) */
ulonglong FUN_140082520(longlong *a1, WCHAR *a2, longlong a3, int a4);


/* ==== wave-INDIR 追加: FUN_1400d9818 依赖补齐 (最小桩/类型) ==== */
typedef struct tagSCROLLINFO {
    UINT cbSize;
    UINT fMask;
    int nMin;
    int nMax;
    UINT nPage;
    int nPos;
    int nTrackPos;
} SCROLLINFO, *LPSCROLLINFO;

typedef struct _NOTIFYICONDATAW_MIN {
    DWORD cbSize;
    HWND hWnd;
    UINT uID;
    UINT uFlags;
    UINT uCallbackMessage;
    HICON hIcon;
    WCHAR szTip[128];
    DWORD dwState;
    DWORD dwStateMask;
    WCHAR szInfo[256];
    union {
        UINT uTimeout;
        UINT uVersion;
    } DUMMYUNIONNAME;
    WCHAR szInfoTitle[64];
    DWORD dwInfoFlags;
    uint32_t guidItem[4];
    HICON hBalloonIcon;
} NOTIFYICONDATAW_M, *PNOTIFYICONDATAW_M;


/* d9818 引用的运行时槽/标志 (真值见 core_globals.c 命名映射; 未定义者置 0 判空) */
extern int8_t g_flagA24F;
/* DAT_14013a24f (审计#1: 已改有符号, dc 写入 -1/0xff/0xfe 哨兵) */
extern uint8_t g_topiconFlag;
/* DAT_14013cccb */
extern uint32_t g_WM_TaskbarRestart;
/* DAT_14013d3e8 */
extern uint8_t g_iconRefCount;
/* DAT_14013d322 */
extern uint8_t g_fgWndLock;
/* DAT_14013d324 */
extern void *g_ownerWnd;
/* DAT_14013d3d8 */
extern HWND g_hwndTray;
/* DAT_14013dd00 */
extern void *DAT_14013d3e0;
;
;
/* 主窗口句柄槽 (未定义 → 置 0 判空) */
extern uint8_t DAT_14013e1f5;
;
;
/* 鼠标捕获模式标志 */
extern uint8_t DAT_14013d8a0[];
/* 托盘图标结构缓冲 (core_b3_remaining.c 同款 extern) */

/* d9818 的 FUN_* 叶子依赖 — 库内无实现, 本文件补最小签名桩 (TODO(verify) 后续波次还原真体):
 * 各桩语义按 decompiled.c 调用点注记; 体量标注取自函数横幅。 */
void FUN_14005b900(char *slot, void *hwnd, LPCWSTR tip, HICON icon, uint msg);
/* @0x14005b900 size=159 托盘图标添加 */
uint32_t FUN_14007e15c(byte *slot, void *hwnd, LPCWSTR a3, HICON a4, LPCWSTR a5, uint a6, int a7);
/* @0x14007e15c size=493 图标对象销毁 */
void FUN_14001aae8(longlong hwnd);
/* @0x14001aae8 size=87 */
void FUN_14005e204(void);
/* @0x14005e204 size=78 取消前景锁定 */
void FUN_140073c58(longlong obj);
/* @0x140073c58 size=115 关窗前清理 */
void FUN_1400e6860(WPARAM a1, int a2);
/* @0x1400e6860 size=127 */
void FUN_14006703c(longlong obj);
/* @0x14006703c size=571 定时器/动画启动 */
ulonglong FUN_1400b1724(LARGE_INTEGER li, ulonglong a2);
/* @0x1400b1724 size=1733 脚本块释放 */
void FUN_14009bb28(longlong *a1, int a2);
/* @0x14009bb28 size=147 */
void FUN_140066d18(longlong a1, longlong a2);
/* @0x140066d18 size=262 */
void FUN_14005e198(HWND h, uint *a2, int a3);
/* @0x14005e198 size=108 控件禁用/置灰 */
ulonglong FUN_14006b7f0(longlong obj, longlong *a2);
/* @0x14006b7f0 size=265 */
void FUN_14007df90(longlong a1, int a2);
/* @0x14007df90 size=139 */
void FUN_14005e0e0(HWND h);
/* @0x14005e0e0 size=184 前景恢复 */
void FUN_1400e54d4(char *slot, HWND h, longlong *a3, longlong a4, longlong a5);
/* @0x1400e54d4 size=528 托盘热区消息 */
void FUN_14002e790(HWND h);
/* @0x14002e790 size=1714 */
char FUN_1400d5b48(longlong obj, HDC dc);
/* @0x1400d5b48 size=5185 大桩: 后续波次 */
ulonglong FUN_1400e89fc(HDC obj, ulonglong a2, HDC a3, longlong *a4);


/* ==== wave-INDIR 追加: FUN_1400f6db0 依赖 ==== */
typedef void *HFONT;

typedef struct tagTEXTMETRICW_MIN {
    LONG tmHeight;
    LONG tmAscent;
    LONG tmDescent;
    LONG tmInternalLeading;
    LONG tmExternalLeading;
    LONG tmAveCharWidth;
    LONG tmMaxCharWidth;
    LONG tmWeight;
    LONG tmOverhang;
    LONG tmDigitizedAspectX;
    LONG tmDigitizedAspectY;
    WCHAR tmFirstChar;
    WCHAR tmLastChar;
    WCHAR tmDefaultChar;
    WCHAR tmBreakChar;
    BYTE tmItalic;
    BYTE tmUnderlined;
    BYTE tmStruckOut;
    BYTE tmCharSet;
    DWORD tmPitchAndFamily;
} TEXTMETRICW_M;

typedef TEXTMETRICW_M tagTEXTMETRICW_M;
/* @0x1401293c0 = 00 00 00 00 00 00 e0 c1 */
extern void *DAT_14013e458;
;
;
/* 分隔线画笔缓存槽 (初 0 判空) */
extern void (*DAT_14013ce78)(HICON, float *, float *);
/* 图标尺寸回调槽 (初 0 判空调用) */
extern int FUN_140107944(int v);
/* 取整/绝对值 core_b9_remaining.c */
extern HFONT FUN_1400b89dc(void *font, double *ph, LPCWSTR face);
/* 字体克隆调参 core_b7c.c 真体 */
extern longlong FUN_140063b00(longlong idx, longlong *pdata, longlong *pcapa, uint32_t elemsz);
/* 向量元素 core_b7c.c 真体 */
extern void FUN_1400703e4(void *slot, LPCWSTR src);


/* ---- f6db0 的列表控件子函数依赖 — 库内无实现, 最小签名桩 (TODO(verify) 后续波次) ---- */
longlong FUN_1400f5584(longlong self, int item, int *out);
/* size=129 前景色表查询 */
longlong FUN_1400f593c(longlong self, int item, int *out);
/* size=129 背景色表查询 */
longlong FUN_1400f40c8(longlong self, int item, void **pfont);
/* size=76  项字体查询 */
ulonglong FUN_1400f2f58(longlong self, int *rc4, int item);
/* size=360 视口原点计算 */
longlong *FUN_1400f4c28(longlong self, longlong item);
/* size=117 子项属性数组指针 */
longlong *FUN_1400f4ca0(longlong self, longlong item, longlong sub);
/* size=123 子项属性(带列) */
longlong FUN_1400f568c(longlong self, int sub, int item, int *out);
/* size=149 子项前景色 */
longlong FUN_1400f5a44(longlong self, int sub, int item, int *out);
/* size=149 子项文本色 */
longlong FUN_1400f4114(longlong self, int sub, void **pfont, void **pface);
/* size=127 子项字体/字体名 */
void FUN_1400f527c(longlong self, HDC dc, void *attr, LPRECT rc);
/* size=186 焦点/状态覆盖绘制 */
undefined8 FUN_1400f35b8(HICON ic, longlong *pw, longlong *ph);
/* size=133 图标尺寸(缩放路径) */
HICON FUN_14001f1d4(LPCWSTR path, ulonglong *extra);
/* size=2809 图标装载(大桩后续波次) */
undefined4 FUN_1400ede48(HDC dc, HICON ic, undefined8 a3, int a4, ulonglong a5, ulonglong a6,
                         undefined4 a7, undefined4 a8, ulonglong a9, ulonglong a10, COLORREF a11);
/* ==== B0/P3 auto-generated cross-TU prototypes & data externs ====
 * Call sites that relied on unprototyped declarations keep empty ( ) lists. */

uint64_t FUN_140103020(const uint16_t *s);
void *FUN_1400170b0(void **p);
void *FUN_140070044(const char *s);
int FUN_1400678f0(void *a, long long *b, short c);
void PECMD_ParseSizeNumber(int64_t *pp, int64_t *out);
void FUN_1400284d4(long long *a, const void *b);
long long PECMD_Dialog2727Proc(void *a, unsigned int b, uint64_t c, uint64_t d);
void FUN_140062950(void *a);
void PECMD_BuildResourceFont(longlong *param_1, UINT param_2, void *param_3);
uint64_t PECMD_AboutDlgProc(void *param_1, int param_2, void *param_3);
void FUN_14001b888(uint64_t a);
uint64_t AbortSystemShutdownW(void);
uint64_t AddFontMemResourceEx(void);
uint64_t AddFontResourceW(void);
uint64_t AdjustTokenPrivileges(void);
uint64_t AllocConsole(void);
uint64_t AllocateAndInitializeSid(void);
int AppendMenuW(void *m, unsigned int f, uint64_t id, const unsigned short *s);
uint64_t Beep(void);
uint64_t BeginPaint(void);
uint64_t BitBlt(void);
uint64_t CM_Get_Parent(void);
uint64_t CallNextHookEx(void);
uint64_t CallWindowProcW(void);
uint64_t ChangeDisplaySettingsExW(void);
uint64_t ChangeServiceConfig2W(void);
uint64_t ChangeServiceConfigW(void);
uint64_t CharUpperW(void);
uint64_t CheckTokenMembership(void);
uint64_t ClientToScreen(void *h, void *pt);
uint64_t CloseClipboard(void);
uint64_t CloseHandle(void *h);
int CloseServiceHandle(void *h);
uint64_t ControlService(void *h, unsigned long c, void *st);
uint64_t CopyFileW(uint64_t a, uint64_t b, int c);
uint64_t CopyImage(void);
uint64_t CreateBitmap(void);
uint64_t CreateCompatibleBitmap(void);
uint64_t CreateCompatibleDC(void);
uint64_t CreateDialogParamW(void);
uint64_t CreateDirectoryW(const uint16_t *path, void *sa);
HRGN CreateEllipticRgn(int a, int b, int c, int d);
void *CreateEventW(void *sa, int manual, int init, const unsigned short *name);
uint64_t CreateFileMappingA(void);
HANDLE CreateFileMappingW(HANDLE h, LPSECURITY_ATTRIBUTES sa, DWORD prot, DWORD maxHi, DWORD maxLo,
                          LPCWSTR name);
uint64_t CreateFileW(void);
uint64_t CreateFileA(void);
uint64_t CreateFontW(void);
uint64_t CreateHardLinkW(uint64_t a, uint64_t b, void *c);
uint64_t CreateMutexA(void *a, int b, const char *c);
uint64_t CreateMutexW(void *a, int b, void *c);
uint64_t CreatePopupMenu(void);
uint64_t CreateRoundRectRgn(void);
void *CreateServiceW(void *mgr, const unsigned short *name, const unsigned short *disp,
                     unsigned long acc, unsigned long type, unsigned long start, unsigned long err,
                     const unsigned short *bin, const unsigned short *grp, unsigned long *tag,
                     const unsigned short *deps, const unsigned short *acct,
                     const unsigned short *pwd);
HBRUSH CreateSolidBrush(COLORREF color);
uint64_t CreateToolhelp32Snapshot(DWORD flags, DWORD pid);
uint64_t CryptAcquireContextW(void);
uint64_t CryptCreateHash(void);
uint64_t CryptDestroyHash(void);
uint64_t CryptGetHashParam(void);
uint64_t CryptHashData(void);
uint64_t CryptReleaseContext(void);
uint64_t DefWindowProcW(void);
uint64_t DefWindowProcW_exref(void);
uint64_t DefineDosDeviceW(void);
uint64_t DeleteDC(void *a);
uint64_t DeleteFileW(uint64_t a);
uint64_t DeleteService(void *h);
int DestroyIcon(void *ic);
uint64_t DestroyMenu(void *h);
int DeviceIoControl(void *h, unsigned long code, void *in, unsigned long inb, void *out,
                    unsigned long outb, unsigned long *ret, void *ov);
uint64_t DialogBoxIndirectParamW(void *a, void *b, void *c, void *d, uint64_t e);
uint64_t DialogBoxParamW(void *a, void *b, void *c, void *d, uint64_t e);
uint64_t DragAcceptFiles(void);
uint64_t DrawEdge(void);
uint64_t DrawFocusRect(void);
int DrawFrameControl(void *hdc, RECT *r, UINT t, UINT f);
uint64_t DrawIcon(void);
uint64_t DrawIconEx(void);
int DrawTextW(void *hdc, LPCWSTR s, int c, RECT *rc, uint fmt);
uint64_t Ellipse(void);
uint64_t EmptyClipboard(void);
uint64_t EnableMenuItem(void);
uint64_t EndPaint(void);
int EnumChildWindows(HWND parent, WNDENUMPROC cb, LPARAM lp);
uint64_t EnumDisplayDevicesW(void);
uint64_t EnumDisplaySettingsW(void);
int EnumWindows(WNDENUMPROC cb, LPARAM lp);
void ExitProcess(uint32_t code);
uint64_t ExitWindowsEx(void);
uint64_t ExitWindowsEx_exref(void);
uint64_t ExpandEnvironmentStringsW(LPCWSTR src, LPWSTR dst, DWORD n);
void PECMD_EnumDropFiles(void *param_1, longlong *param_2, longlong *param_3);
void PECMD_InitSystemApiGlobals(void);
BOOL ConnectNamedPipe(void *pipe, OVERLAPPED *ol);
BOOL GetOverlappedResult(void *pipe, OVERLAPPED *ol, DWORD *bytes, BOOL wait);
uint64_t ShellExecuteExW(void *pei);
HANDLE CreateNamedPipeA(LPCSTR name, DWORD openMode, DWORD pipeMode, DWORD maxInst, DWORD outSz,
                        DWORD inSz, DWORD timeout, LPSECURITY_ATTRIBUTES sa);
BOOL SetNamedPipeHandleState(HANDLE h, DWORD *mode, DWORD *maxCount, DWORD *collectBytes);
int CreatePipe(void *out, void *in, void *sa, unsigned long n);
void PECMD_ServiceMainEntry(undefined8 param_1, undefined8 *param_2);
void PECMD_GrantCurrentUserAccess(int64_t a);
void PECMD_SwitchToDefaultDesktop(void);
void PECMD_ReleaseCallbackObj(int *param_1);
uint64_t PECMD_ScriptInit(void);
void PECMD_NtShutdownSystemCall(int param_1);
void PECMD_SetCurrentDirIfChanged(LPCWSTR param_1);
undefined4 PECMD_LineIsTeamExecLoad(const WCHAR *param_1);
void PECMD_RunClearTmpMbrosOnce(void *script);
uint64_t PECMD_InstallKeyboardHook(void);
uint64_t PECMD_ExpandDrivePathAlloc(void);
void PECMD_SyncWorkingDirectory(void);
uint64_t PECMD_SetVarCore(void);
void PECMD_FixKnownDlls32(void);
void PECMD_TruncateDebugLog(void);
void PECMD_AutoMountStartup(long long *param_1, LPCWSTR param_2);
uint32_t FormatMessageW(uint32_t flags, const void *src, uint32_t msg, uint32_t lang, WCHAR *buf,
                        uint32_t size, void *args);
void FUN_14001d78c(unsigned char *dst, const unsigned char *src, longlong n);
int SetConsoleWindowInfo(void *h, int b, void *r);
uint64_t PECMD_ExpandSpecialDirs(void *param_1, const uint16_t *param_2, longlong param_3,
                                 longlong param_4, const uint16_t *param_5);
void PECMD_LoadResourceLines(void);
void PECMD_SendHotkeyKeyMessage(uint32_t a, int b, int c);
uint64_t PECMD_EnsureCallbackWindow(void);
uint64_t PECMD_UserCmdHandler(uint64_t a);
uint64_t PECMD_ArrayPrepend(void);
void PECMD_SetObjectEnable(int64_t a, unsigned int b);
int PECMD_AnsiStrNCompare(char *param_1, longlong param_2, longlong param_3);
undefined8 PECMD_MatchPrefixN(ushort *param_1, undefined8 *param_2, int param_3);
void PECMD_DestroyTrayIcon(char *param_1);
uint64_t PECMD_UpdateTrayIcon(void);
DWORD PECMD_QueryRegValueWithRetry(HKEY param_1, const WCHAR *param_2, const WCHAR *param_3,
                                   DWORD *param_4, BYTE *param_5, DWORD *param_6);
uint64_t PECMD_AsciiWideICmp(const char *a, const uint16_t *b);
void PECMD_ReleaseGdiObjects(undefined8 *param_1, HWND param_2);
uint64_t PECMD_GetObjField20(longlong param_1);
void PECMD_BringWindowToFront(void *param_1);
int InitializeSecurityDescriptor(void *pSD, DWORD dwRevision);
int SetSecurityDescriptorDacl(void *pSD, BOOL bDaclPresent, void *pDacl);
void PECMD_InitNullDaclSD(uint64_t *param_1);
void PECMD_WaitTickCount(void);
uint64_t PECMD_CreateNamedMutex(uint64_t a, int b, uint16_t *c);
void PECMD_AllocStringSlot2(void **param_1, longlong param_2);
longlong PECMD_VectorAppendGen(longlong *param_1, longlong *param_2, longlong *param_3,
                               uint8_t *param_4, uint param_5, longlong param_6);
int64_t PECMD_VectorSlotPtr(int64_t param_1, int64_t *param_2, int64_t *param_3,
                            unsigned int param_4);
uint64_t PECMD_ExpandDrivePath(const uint16_t *a, uint64_t b, uint16_t *c, longlong *d);
void PECMD_ParseShortStore(uint64_t *param_1, int *param_2, short param_3);
uint64_t PECMD_ParseSizeAndSkipWs(int64_t *param_1, uint64_t *param_2);
void PECMD_GetEnvVarToStr(LPCWSTR param_1, uint64_t *param_2);
longlong *PECMD_AllocMagicString(LPCWSTR param_1);
uint64_t PECMD_ParsePrefixColon(void);
void PECMD_RelaunchLoadFirstUsb(void);
uint64_t PECMD_SetVariableWithPrefix(void);
uint64_t *PECMD_StrSetOrConcat(uint64_t *param_1, uint64_t *param_2, LPCWSTR param_3);
uint64_t PECMD_SetCheckVariable(void);
int64_t PECMD_QueryFontInfo(int64_t a, int *b, const void *c);
uint64_t PECMD_CreateFont(void *a, void *b, void *c);
static HANDLE PECMD_HandleDuplicateValid(HANDLE h, LARGE_INTEGER *out);
static longlong PECMD_GetPartitionLayoutEntry(uintptr_t h, int mode, ulonglong *out);
void PECMD_GetWindowTextAlloc(HWND param_1, uint64_t *param_2);
uint64_t PECMD_GetOwnerWindow(uint64_t param_1);
void PECMD_ShowAboutDialog(void);
uint64_t PECMD_GetFileSize(void);
HWND PECMD_GetOrCreateHiddenWnd(undefined8 *param_1, longlong param_2);
longlong PECMD_ModalMsgPumpEx(longlong *param_1, ulonglong param_2);
uint64_t PECMD_RichEditWordBreakProc(void);
uint64_t FillRect(void);
uint64_t FindClose(HANDLE h);
uint64_t FindNextFileW(HANDLE h, void *fd);
uint64_t FindWindowA(void);
uint64_t FindWindowExA(void);
uint64_t FindWindowExW(void);
uint64_t FindWindowW(void);
uint64_t FrameRgn(void);
uint64_t FreeSid(void);
uint64_t GetAsyncKeyState(void);
uint64_t GetBkColor(void);
int GetClientRect(void *hWnd, void *lpRect);
uint64_t GetClipboardData(void);
uint64_t GetConsoleWindow(void);
uint64_t GetCurrentThread(void);
HDC GetDC(void *h);
uint64_t GetDIBits(void);
uint64_t GetDeviceCaps(void);
uint64_t GetDlgCtrlID(void *h);
uint64_t GetDlgItemTextW(void);
uint64_t GetDriveTypeW(void);
uint64_t GetEnvironmentVariableA(void);
uint64_t GetDiskFreeSpaceExW(void);
uint64_t GetFileAttributesExW(void);
uint64_t GetFileSize(void);
uint64_t GetFileSizeEx(void);
uint64_t GetFocus(void);
uint64_t GetForegroundWindow(void);
uint64_t GetFullPathNameW(void);
uint64_t GetHandleInformation(void);
uint64_t GetIconInfo(void);
uint64_t GetKeyState(void);
uint64_t GetKeyboardState(void);
uint64_t GetLocaleInfoW(void);
uint64_t GetLogicalDriveStringsW(void);
uint64_t GetLogicalDrives(void);
uint64_t GetMenuItemID(void);
uint64_t GetMenuStringW(void);
int GetMessageW(void *m, void *w, unsigned int a, unsigned int b);
uint64_t GetObjectW(uint64_t a, uint64_t b, void *c);
HWND GetActiveWindow(void);
uint64_t GetPixel(void);
uint64_t GetProcessAffinityMask(void);
int GetScrollPos(void *w, int n);
uint64_t GetScrollRange(void);
uint64_t GetStdHandle(void);
void *GetStockObject(int i);
uint64_t GetSubMenu(void);
DWORD GetSysColor(int idx);
uint64_t GetSystemMenu(void);
uint64_t GetSystemTimeAsFileTime(void);
uint64_t GetThreadContext_exref(void);
uint64_t GlobalMemoryStatusEx(void);
uint64_t GetTokenInformation(void);
uint64_t GetVersion(void);
uint64_t GetVersionExW(void);
uint64_t GetWindowDC(void);
uint64_t GetWindowLongPtrW(void *h, int idx);
uint64_t GetWindowTextLengthW(void);
uint64_t GlobalAlloc(void);
uint64_t GlobalDeleteAtom(void);
uint64_t GlobalFree(uint64_t a);
uint64_t GlobalLock(void);
uint64_t GlobalMemoryStatus(void);
uint64_t GlobalUnlock(void);
uint64_t HeapReAlloc(void);
int InitiateSystemShutdownW(void *m, const unsigned short *t, unsigned long to, int f1, int f2);
uint64_t InsertMenuW(void);
int InvalidateRect(void *w, const void *r, int erase);
int IsDialogMessageW(void *h, const void *m);
uint64_t IsIconic(void);
int IsWindowEnabled(HWND w);
int IsWindowVisible(HWND w);
uint64_t LoadCursorW(void);
uint64_t LoadEnvi(const uint16_t *a, const uint16_t *b);
void *LoadIconW(void *hinst, const unsigned short *name);
uint64_t LoadMenuW(void);
uint64_t LockWorkStation(void);
uint64_t LookupPrivilegeValueW(void);
uint64_t MapViewOfFile(void);
int MessageBoxW(void *w, const unsigned short *t, const unsigned short *c, unsigned int f);
uint64_t ModifyMenuW(void);
uint64_t MoveFileExW(uint64_t a, uint64_t b, unsigned int c);
uint64_t MoveFileW(uint64_t a, uint64_t b);
uint64_t MoveWindow(void);
uint64_t MulDiv(void);
int OffsetRect(RECT *r, int dx, int dy);
uint64_t OpenClipboard(void);
uint64_t OpenFileMappingA(void);
uint64_t OpenFileMappingW(void);
uint64_t OpenProcessToken(void);
void *OpenSCManagerW(const void *a, const void *b, unsigned long acc);
void *OpenServiceW(void *mgr, const unsigned short *name, unsigned long acc);
uint64_t OpenThread(void);
uint64_t PECMD_EvalExpr(void);
uint64_t PECMD_ExecCmd(void);
uint64_t PECMD_MemCmp(void);
uint64_t PECMD_ResPrefA(void);
uint64_t PECMD_ResPrefB(void);
uint64_t PECMD_ResPrefC(void);
uint64_t PECMD_ResPrefD(void);
uint64_t PECMD_ResReenc(void);
uint64_t PECMD_SwapBytes(void);
uint64_t PECMD_XorDecode(void);
uint64_t PathFileExistsW(void);
uint64_t Polyline(void);
int PostMessageW(void *w, unsigned int m, uint64_t a, uint64_t b);
uint64_t PostQuitMessage(void);
int PtInRect(const void *r, POINT p);
uint64_t QueryDosDeviceW(void);
uint64_t SetVolumeMountPointW(void);
uint64_t QueryPerformanceFrequency(void);
uint64_t RaiseException(void);
uint64_t ReadProcessMemory(void);
uint64_t RedrawWindow(void);
uint64_t RegDeleteKeyW(void);
int RegDeleteValueW(void *k, const WCHAR *nm);
uint64_t RegEnumValueW(void);
int RegOpenKeyExW(void *k, const unsigned short *s, unsigned long o, unsigned long a, void **out);
uint64_t RegQueryInfoKeyW(void);
uint64_t RegQueryValueExA(void);
uint64_t RegisterClassExW(void);
uint64_t RegisterDeviceNotificationW(void);
uint64_t ReleaseDC(void *h, void *dc);
uint64_t ReleaseMutex(void);
uint64_t RemoveDirectoryW(uint64_t a);
uint64_t RemoveFontMemResourceEx(void);
uint64_t RemoveFontResourceW(void);
uint64_t RemoveMenu(void);
int ResetEvent(void *h);
uint64_t SHChangeNotify(void);
uint64_t SHFileOperationW(uint64_t a);
uint64_t ScreenToClient(void *h, void *p);
uint64_t SearchPathW(void);
void *SelectObject(void *hdc, void *obj);
uint64_t SendMessageTimeoutW(void);
COLORREF SetBkColor(void *hdc, COLORREF c);
uint64_t SetBkMode(void);
uint64_t SetClipboardData(void);
uint64_t SetConsoleScreenBufferSize(void);
uint64_t SetCursor(void);
uint64_t SetCursorPos(void);
uint64_t SetDIBits(void);
uint64_t SetDlgItemTextW(void);
int SetEvent(void *h);
uint64_t SetFocus(void *h);
uint64_t SetLayeredWindowAttributes(void *h, uint32_t cr, uint8_t a, uint32_t f);
uint64_t SetLocalTime(void);
int SetMenuItemBitmaps(void *m, unsigned int id, unsigned int f, void *b1, void *b2);
uint64_t SetParent(void *h, void *p);
uint64_t SetPixel(void);
uint64_t SetScrollInfo(void);
uint64_t SetScrollRange(void);
uint64_t SetSystemPowerState(void);
COLORREF SetTextColor(void *hdc, COLORREF c);
uint64_t SetThreadContext_exref(void);
uint64_t SetThreadPriority(void);
uint64_t SetTimer(HWND h, uint64_t id, UINT ms, TIMERPROC proc);
uint64_t SetWindowLongW(void);
uint64_t SetWindowsHookExW(void);
uint64_t SetupDiCreateDeviceInfoList(void);
uint64_t SetupDiCreateDeviceInfoW(void);
uint64_t SetupDiOpenClassRegKey(void);
uint64_t SetupDiSetDeviceRegistryPropertyW(void);
uint64_t Shell_NotifyIconW(DWORD msg, void *nid);
uint64_t ShowScrollBar(void);
uint64_t SleepEx(void);
uint64_t StartServiceCtrlDispatcherW(void);
int StrCmpNIW();
int SwitchDesktop(void *d);
int CloseDesktop(void *d);
int FUN_140068984();
void FUN_140023640(uint64_t a, uint64_t b, int c);
bool PECMD_EnumWindowFindProc(POINT param_1, POINT *param_2);
void PECMD_FillMsgEnvelope(uint64_t param_1, uint64_t *param_2, long long param_3,
                           uint64_t param_4);
uint64_t StrCmpNW(void);
uint64_t StrCpyNW(void);
uint64_t StrStrA(void);
uint64_t StrStrIW(void);
LPWSTR StrStrW(const WCHAR *a, const WCHAR *b);
uint64_t StrToIntExW(void);
uint64_t StretchBlt(void);
uint64_t SystemParametersInfoW(void);
uint64_t TerminateThread(void);
uint64_t UnhookWindowsHookEx(void *h);
uint64_t UnmapViewOfFile(void);
int UnregisterHotKey(void *w, int id);
int UpdateWindow(void *w);
uint64_t VirtualAlloc(void);
uint64_t VirtualProtect(void);
uint64_t VirtualProtectEx(void);
uint64_t VirtualQueryEx(void);
unsigned long WaitForMultipleObjects(unsigned long n, void **h, int all, unsigned long ms);
uint64_t WaitNamedPipeW(void);
uint64_t WideCharToMultiByte(void);
uint64_t WinExec(void);
uint64_t WinExec_exref(void);
uint64_t WriteProcessMemory(void);
uint64_t _UNK_140121ff6(void);
uint64_t _localtime64(void);
int _snwprintf(WCHAR *buf, size_t n, const WCHAR *fmt, ...);
uint64_t _time64(void);
uint64_t keybd_event(void);
int lstrcmpiA(const char *a, const char *b);
uint64_t lstrcpynW(void);
int main(void);
uint64_t mouse_event(void);
uint64_t ram0x000140120a48(void);
uint64_t thunk_FUN_140072814(void *a, longlong *b, const char *c);
uint64_t u__26_INDATA_140121fe0(void);
uint64_t u_____D__140120a40(void);
void PECMD_RestartSelf(uint16_t *param_1);
int FUN_14005B184(const char *a, const char *b, int n);
void thunk_PECMD_GetNetworkConnectionName(const char *a, void *b, const char *c);
uint16_t *PECMD_StrAlloc(uint16_t **ps, size_t count);
void FUN_140060A74(uint8_t *buf, int len);
int PECMD_RunBootScriptInFiber(uint16_t *cmdline);
int32_t PECMD_DecodeEncTextToUtf16(uint32_t spec, const uint8_t *src, int srclen, uint16_t *dst,
                                   int dstcap, uint32_t key);
uint64_t FindFirstFileW(void);
uint64_t FindFirstVolumeW(void);
uint64_t FindNextVolumeW(void);
uint64_t FindVolumeClose(void);
uint64_t LoadBitmapW(void);
uint64_t LoadImageW(void);
uint64_t lstrcmpA(void);
uint64_t CM_Get_Device_IDW(void);
uint64_t SetupDiDestroyDeviceInfoList(void);
uint64_t SetupDiEnumDeviceInterfaces(void);
uint64_t SetupDiGetDeviceInstanceIdW(void);
uint64_t SetupDiGetDeviceInterfaceDetailW(void);
uint64_t PECMD_ScriptCopy(void);
uint64_t FUN_140061E98(void);
uint64_t FUN_1400688E0(void);
uint64_t FUN_14006A81C(void);
uint64_t CharUpperA(void);
uint64_t DeleteVolumeMountPointW(void);
uint64_t LookupAccountSidW(void);
uint64_t StrCmpNA(void);
uint64_t StrPBrkW(void);
uint64_t FUN_140001188(void);
uint64_t FUN_14000C764(void);
uint64_t PECMD_ResizeBuffer(void);
uint64_t PECMD_GetWindowObjectRef(void);
uint64_t PECMD_ParseHashNumbers(void);
uint64_t PECMD_GetComboItemText(void);
uint32_t *PECMD_InitFieldContainer(uint32_t *param_1, uint8_t param_2);
uint64_t PECMD_SaveSelectionToVar(void);
uint64_t PECMD_SendColorMsg(long long param_1, uint param_2, uint param_3, uint param_4,
                            uint param_5);
uint64_t PECMD_InitDragDrop(void);
uint64_t PECMD_IsSetupClass(void);
uint64_t PECMD_SetControlState(void);
uint64_t PECMD_ShowContextMenu(void);
uint64_t *PECMD_InitWinObjBase(uint64_t *param_1);
LRESULT PECMD_GetControlFont(int64_t param_1);
int PECMD_UpdateWindowStyleBits(int64_t a, unsigned int b, uint64_t c);
longlong PECMD_GetWinIdLocked(longlong *param_1);
uint64_t *PECMD_InitObjectSlotC(uint64_t *param_1, uint64_t param_2);
uint64_t FUN_1400ec698(int64_t a, uint64_t b);
void PECMD_Send423ToWindow(long long param_1, long long param_2, uint64_t param_3,
                           unsigned char *param_4, uint64_t param_5);
undefined8 PECMD_AppendValueNode(longlong param_1, const WCHAR *param_2, undefined8 param_3);
void PECMD_LayoutChildWindows(int64_t a, unsigned char b);
uint64_t PECMD_InitWindowObjectF(void);
uint64_t *PECMD_InitWebViewObj(uint64_t *param_1, uint64_t param_2);
uint64_t PECMD_CreateDateTimePickCtl(void);
bool PECMD_CreateIpAddressCtl(longlong *param_1, DWORD param_2, int *param_3, void *param_4,
                              uint param_5);
uint64_t PECMD_InitControlObjC(void);
bool PECMD_CreateCustomChildControl(longlong *param_1, uint param_2, int *param_3, HWND param_4,
                                    uint param_5, LPCWSTR param_6);
uint64_t *PECMD_InitControlObjO(uint64_t *param_1, uint64_t param_2);
uint64_t *PECMD_InitScrollObj(uint64_t *a, uint64_t b);
void PECMD_SetControlColors(long long param_1, int *param_2);
bool PECMD_InitStaticControl(longlong *param_1, DWORD param_2, LPCWSTR param_3, uint param_4,
                             int *param_5, HWND param_6, uint param_7);
void PECMD_CreateStaticControlFromRect(int64_t *a, const void *b, unsigned int c, uint64_t *d,
                                       int64_t e, unsigned int f);
uint64_t *PECMD_InitStaticCtl(uint64_t *a, uint64_t b);
uint64_t PECMD_CreateProgressCtl(void);
uint64_t thunk_FUN_140064b78(void);
void PECMD_EndDialogDeferred(uint64_t param_1, int param_2);
uint64_t PECMD_ProcessWindowObjMessage(void);
undefined8 PECMD_PumpMessagesTimeout(int param_1, longlong param_2);
void PECMD_TrimWorkingSetAndExec(LPCWSTR param_1);
uint16_t PECMD_DispatchMessageMap(int64_t *param_1, int param_2, uint64_t param_3,
                                  uint64_t *param_4, int64_t param_5, uint8_t param_6,
                                  uint64_t *param_7);
uint64_t PECMD_MessageHookProc(int param_1, uint64_t param_2, uint64_t param_3);
void PECMD_CloseThreadSlot(int64_t *param_1);
void PECMD_FreeResourceSet(longlong *param_1);
void PECMD_DestroyStaticControl(uint64_t *param_1);
void PECMD_ReleaseSlotObject(int64_t *param_1);
int TlsSetValue(unsigned long idx, void *val);
int Rectangle(void *hdc, int a, int b, int c, int d);
void SwitchToFiber(void *fiber);
void PECMD_CleanupParser(int64_t p);
void PECMD_DestroyControlObj(undefined8 *param_1);
void SwitchToThisWindow(void *hw, int b);
int GetClassNameW(void *hw, uint16_t *p, int n);
int CM_Get_DevNode_Status(uint32_t *a, uint32_t *b, uint32_t c, uint32_t d);
int CM_Query_And_Remove_SubTreeW(uint32_t a, int *b, uint32_t c, uint32_t d);
int CM_Request_Device_EjectW();
int CM_Locate_DevNodeW(uint32_t *a, void *b, uint32_t c);
int CM_Reenumerate_DevNode(uint32_t a, uint32_t b);
uint64_t SystemTimeToFileTime(void);
uint64_t RegEnumKeyExW(void);
int wnsprintfW(uint16_t *b, int n, const uint16_t *f, ...);
void PECMD_DestroyWindowObj(uint64_t *a);
void *PECMD_FreeResourceObject(void *a, unsigned int b);
uint16_t *PECMD_DriveTypeName(int i, uint16_t *out, int max);
uint64_t PECMD_ParseControlMessage(int64_t *a, uintptr_t b, uintptr_t c, const uint16_t *d,
                                   uintptr_t e, int64_t f);
uint64_t PECMD_ControlEnableCommand(int64_t *a, uintptr_t b, uintptr_t c, const uint16_t *d,
                                    uintptr_t e, int64_t f);
int GetWindowInfo(void *hWnd, void *pwi);
uint64_t PECMD_SetControlGeom(uintptr_t hwnd, uint16_t *s, int64_t p3, int64_t *p4, uintptr_t p5,
                              uint32_t color, int64_t p7);
uint64_t *PECMD_GetDiskLayoutInfo(uintptr_t h, uint64_t *buf, uint32_t *out);
uint32_t PECMD_GetDiskGeometry(const uint16_t *p, uintptr_t h);
uint32_t PECMD_IsDevicePathPrefix(LPCWSTR param_1);
int64_t PECMD_OpenFileExisting(uint64_t a, uint64_t b, uint64_t c);
uint64_t DuplicateTokenEx(void);
uint64_t SetTokenInformation(void);
uint64_t FileTimeToSystemTime(void);
int IsRectEmpty(const void *r);
uint64_t CreateDCW(void);
uint64_t ReleaseSemaphore(void);
void *PECMD_InitSimpleWindow(void *a, uint64_t *b, uint64_t *c);
void *PECMD_InitListViewObject(void *a, uint64_t *b, uint64_t *c);
int PECMD_CountNewlines(ulonglong *param_1, int param_2, int param_3, int param_4);
void PECMD_AppendFormattedI64(int64_t *a, int64_t b);
void *PECMD_InstallInfDevice(void *p);
uint PECMD_InstallPlugPlayDriver(long long param_1, LPCWSTR param_2, LPCWSTR param_3, uint param_4,
                                 int param_5, uint8_t *param_6);
undefined8 PECMD_SetVectorSize(longlong *param_1, ulonglong param_2, int param_3);
void PECMD_AllocExpandPath(void *a, int64_t *b);
void *PECMD_CheckInfDriver(int64_t *a, uint64_t b, char *c);
uint64_t PECMD_DrvLoadInstall(int64_t a, void *b, void *c, int d);
DWORD PECMD_ToSysCopyFiles(longlong param_1, LPCWSTR param_2, uint param_3);
uint64_t SetupDiSetDeviceInstallParamsW(void);
uint64_t SetupDiBuildDriverInfoList(void);
uint64_t SetupDiEnumDriverInfoW(void);
uint64_t SetupDiGetDriverInfoDetailW(void);
uint64_t SetupDiClassNameFromGuidW(void);
void PECMD_ScanDeviDirInfFiles(void *a, uint64_t b, uint64_t c, int d, void *e);
uint64_t SetupDiGetDeviceInstallParamsW(void);
uint64_t PECMD_UpdateDriverSigning(int a, int b);
void PECMD_ParseScopeKeywords(int64_t *param_1, uint8_t *param_2, uint8_t *param_3,
                              uint8_t *param_4, char *param_5, uint8_t *param_6);
uint64_t SetupDiClassGuidsFromNameW(void);
uint64_t VerQueryValueW(void);
uint64_t GetFileVersionInfoSizeW(void);
uint64_t GetFileVersionInfoW(void);
int PECMD_CmdKill();
uint64_t SetErrorMode(void);
uint64_t TlsAlloc(void);
int PECMD_DispatchSystemCommandLine();
uint64_t PECMD_ManualMapPeImage(int *a, int16_t *b, uint64_t c);
uint64_t FUN_14001ebdc(void);
uint64_t GlobalAddAtomA(void);
uint64_t lstrcpyA(void);
int RegisterHotKey(void *w, unsigned int id, unsigned int mods, unsigned int vk);
uint64_t WriteConsoleW(void);
uint64_t LCMapStringA(void);
bool PECMD_OnDeleteCommand(undefined8 *param_1, const WCHAR *param_2, HWND param_3);
uint64_t PECMD_LoadLogPixelsDpi(void);
uint64_t PECMD_GetDiskGeometrySize(void);
uint64_t PECMD_TaskShowMessage(uint64_t *p);
longlong *PECMD_AppendAnsiStr(longlong *param_1, LPCSTR param_2);
uint64_t PECMD_FindPartitionInfo(void);
uint64_t PECMD_EncodeImageToStream(void);
uint64_t PECMD_UpdateWindowExStyle(void);
uint16_t *PECMD_StrCat(uint16_t **ps, const uint16_t *src);
void PECMD_StrFree(uint16_t **ps);
void PECMD_StrAlloc2(uint16_t **ps, int64_t count);
void PECMD_SetVar(void *script, const uint16_t *key, const uint16_t *value);
uint64_t GetClassLongW(void);
uint64_t GetSystemInfo(void);
uint64_t lstrcpynA(void);
uint64_t RealizePalette(void);
uint64_t RegOpenKeyExA(void);
uint64_t RegSetValueExA(void);
uint64_t SelectPalette(void);
uint64_t SetClassLongW(void);
uint64_t SetNamedSecurityInfoW(void);
uint64_t SHBrowseForFolderW(void);
uint64_t SHGetPathFromIDListW(void);
uint64_t StrCmpIW(void);
bool PECMD_CreateButtonSubCtl(long long *param_1, LPCWSTR param_2, uint64_t param_3, int *param_4,
                              HWND param_5, uint param_6, DWORD param_7);
uint64_t *PECMD_InitControlObjQ(uint64_t *param_1, uint64_t param_2, uint64_t param_3);
int PECMD_CreateTreeViewControl(int64_t *a, unsigned long b, int *c, void *d, uint32_t e);
bool PECMD_FindAndReleaseComObject(const WCHAR *param_1);
uint64_t *PECMD_InitDcObject(uint64_t *a, uint64_t b);
undefined4 PECMD_IsIconGroupName(ushort *param_1);
uint64_t DestroyCursor(void);
uint64_t EnumResourceNamesW(void);
uint64_t CreateIconFromResource(void);
uint64_t PECMD_ParseAngleNumbers(uint64_t *a, int64_t *b, int64_t *c, int64_t *d, int64_t *e);
int64_t PECMD_DispatchGlobalMessage(int64_t *a, const uint16_t *b, uint32_t c, uint64_t d,
                                    uint64_t e, uint32_t f, uint32_t g, uint64_t h);
uint64_t PECMD_EnumProcessInfo(uint16_t *s, int a, int64_t *p, int b, unsigned long d);
uint64_t GetLastInputInfo(void);
uint64_t GetDiskFreeSpaceW(void);
uint64_t GetVolumeInformationW(void);
uint64_t PECMD_HandleServiceCommandLine(short *param_1);
uint64_t PECMD_ShowIdDialog(LPCWSTR param_1);
void PECMD_RelocatePointerArray(undefined8 param_1, uint *param_2, longlong param_3,
                                longlong param_4);
uint64_t PECMD_VectorPushBack(longlong param_1, uint64_t param_2);
void PECMD_ReclaimPages(longlong param_1, int param_2);
uint64_t SetVolumeLabelW(void);
uint64_t SetupDiGetDeviceInfoListDetailW(void);
uint64_t GetTempPathW(void);
uint64_t CM_Get_DevNode_Status_Ex(void);
WCHAR **PECMD_SkipSpace(WCHAR **pp);
DWORD PECMD_RegOpen(HKEY root, LPCWSTR subkey, HKEY *out, REGSAM access, uint32_t opt);
DWORD PECMD_RegQuery(HKEY root, LPCWSTR subkey, LPCWSTR name, DWORD *type, BYTE *data, DWORD *size);
uint64_t PECMD_CheckDriveType(void);
uint64_t PECMD_OpenCloseDrive(void);
ulonglong PECMD_IsVisibleKeyword(ushort *param_1);
uint8_t PECMD_MatchTokPrefixAdv(LPCWSTR param_1, longlong *param_2, int param_3);
void PECMD_SwapBytePairs(uint8_t *param_1, int param_2);
uint64_t PECMD_ShowWindowMode(void);
uint64_t PECMD_SendPingPacket(void);
uint64_t PECMD_InitWinsockOnce(void);
uint64_t CreateMenu(void);
uint64_t FileTimeToLocalFileTime(void);
uint64_t GetLongPathNameW(void);
uint64_t GetMenu(void);
uint64_t GetTimeZoneInformation(void);
uint64_t LocalFileTimeToFileTime(void);
uint64_t SetMenu(void);
uint64_t SHAppBarMessage(void);
LARGE_INTEGER PECMD_IndataCopyDirs(longlong *param_1, LARGE_INTEGER param_2, uint param_3);
uint64_t PECMD_LzDecompress(void);
uint16_t *PECMD_ExpandPercentD(uint16_t *param_1, WCHAR *param_2);
uint64_t PECMD_BufferedWriteByte(void);
uint64_t PECMD_StreamReadByte(void);
uint64_t PECMD_AllocConsoleBuffers(void);
uint64_t PECMD_CalcCalendarMonthRows(void);
void PECMD_InsertControlObject(WPARAM param_1, longlong param_2, undefined8 *param_3, int param_4,
                               int param_5, int param_6, int param_7, undefined8 *param_8,
                               undefined8 *param_9, uint param_10);
uint64_t PECMD_StreamClose(void);
uint64_t PECMD_ConvertStringEncoding(void);
uint64_t PECMD_MultiByteConvert(uint64_t param_1, uint64_t param_2);
uint64_t PECMD_JoinTokensAndResolve(void);
uint64_t PECMD_ParseCommaNumbers(void);
undefined8 *PECMD_CreateNamedLock(const WCHAR *param_1, char param_2, uint32_t *param_3);
uint64_t PECMD_LoadRasApi(void);
void PECMD_DestroyEventSlots(uint64_t *param_1);
void PECMD_BuildPopMenuRecursive(longlong param_1, uint16_t *param_2, void *param_3,
                                 longlong param_4, longlong param_5, longlong *param_6);
uint64_t PECMD_FindRasConnection(void);
uint64_t PECMD_EnumRasConnections(void);
uint64_t PECMD_EnumPhonebookEntries(void);
uint64_t PECMD_ExecSpecialCommand(void);
uint64_t PECMD_FreeCacheBlock(void);
uint64_t PECMD_ShakeTrayCursor(void);
uint64_t FUN_14007033c(void);
undefined8 PECMD_PushStringToken(undefined8 param_1, undefined8 param_2, const WCHAR *param_3,
                                 undefined8 *param_4);
uint64_t PECMD_EnumNetworkDevices(void);
uint64_t PECMD_DecodeBase64ToWideStr(uint8_t *param_1, uint64_t *param_2);
uint64_t PECMD_ImportEnvironment(void);
uint64_t PECMD_SendTitleMessage(void);
void PECMD_AddControlA(WPARAM param_1, longlong param_2, undefined8 *param_3, int param_4,
                       int param_5, int param_6, int param_7, undefined8 *param_8,
                       undefined8 *param_9, uint param_10, WCHAR *param_11);
uint64_t PECMD_ClipboardCommand(void);
uint64_t RegLoadKeyW(void);
uint64_t RegUnLoadKeyW(void);
int FUN_14006156c(const uint16_t *a, const uint16_t *b);
uint64_t FUN_1400a53e4(int64_t a, void *b, void *c, int d, const uint16_t *e);
const uint16_t *FUN_14001b23c(int64_t a, void *b, const uint16_t *c, void *d, char e);
int FUN_1400510e8(int *a, uint16_t **b, char *c);
int FUN_1400512f4(int *a, uint16_t **b, char *c);
int PECMD_ParseRegexGroup(int *a, uint16_t **b, char *c);
undefined8 PECMD_MatchKeywordTable(undefined4 *param_1, longlong *param_2);
uint64_t FUN_140067b54(uint16_t *a, double *b);
void FUN_140007bcc(uint16_t *a, int *b);
int FUN_140074838();
int SetupDiSetClassInstallParamsW(void *a, void *b, void *c, uint32_t d);
int SetupDiChangeState(void *a, void *b);
void FUN_14005e36c(int64_t a, uint32_t *b);
int FUN_14005bc28(uint16_t a);
int64_t FUN_14005bbb4(int64_t *a);
uint64_t PECMD_FindTokenInList(const uint16_t *src, const uint16_t *devid, int len, uint flags);
uint64_t FUN_1400662a4(int64_t *p1, void *p2, int64_t *p3, const uint16_t *p4, const uint16_t *p5,
                       uint p6);
int SetupDiClassNameFromGuidA(const void *guid, char *buf, uint32_t buflen, uint32_t *req);
uint64_t CreateIconFromResourceEx(void);
uint64_t LCMapStringW(void);
void FUN_1400675b8(int64_t *param_1, int64_t *param_2, int16_t param_3);
void FUN_1400676e4(int64_t *param_1, int64_t *param_2, int16_t param_3);
void FUN_140067748(int64_t *param_1, int64_t *param_2, int16_t param_3);
uint32_t FUN_1400734e4(int64_t param_1, void *param_2, int64_t *param_3);
uint64_t GetNamedSecurityInfoW(void);
uint64_t BuildExplicitAccessWithNameW(void);
uint64_t GetUserNameW(void);
uint64_t LookupAccountNameW(void);
LSTATUS FUN_140096f84(void *param_1, void *param_2, void *param_3);
uint64_t FUN_140063424(void *param_1, int64_t param_2);
void FUN_140101db8(void *param_1, const void *param_2, void *param_3);
void FUN_14005bc48(int64_t param_1, double *param_2, double param_3);
void FUN_14005bc5c(int64_t param_1, double *param_2, double param_3);
uint64_t FUN_14005e0a0(void);
uint64_t FUN_14001b7f4(const uint16_t *a);
void PECMD_AppendQuotedString(int64_t *a, void *b, int c);
uint64_t FUN_1400048c4(int64_t *a);
void PECMD_HardlinkOrCopy(LPCWSTR param_1, LPCWSTR param_2, int param_3);
void *FUN_140017770(void *a, const char *b);
void FUN_1400177b8(void *a);
uint32_t FUN_14000bfcc(uint64_t a, uint64_t b, const uint16_t *c);
uint64_t LoadStringA(void);
int atexit(void (*fn)(void));
void FUN_14005b540(uint16_t *a, int b);
void *PECMD_NamedObjLookupOrCreate(const uint16_t *a);
short FUN_1400677b0(longlong *param_1, longlong param_2);
BOOL FUN_14006f908(char param_1, int param_2);
longlong FUN_1400e7758(longlong *param_1);
void PECMD_LazyLoadProc(const char *name, const char *dll, longlong **slot, longlong *x);
void *FUN_1400b3d0c(const unsigned short *a, longlong *b, longlong c, longlong d, unsigned int *e,
                    undefined8 *f, int g);
void PECMD_DeferDialogAction2(longlong a, const unsigned short *b, longlong *c, longlong d, int e,
                              int f);
void PECMD_MenuItemWithIcon(HMENU param_1, UINT param_2, UINT_PTR param_3, byte *param_4);
void PECMD_MenuRegisterCommand(byte *param_1, ushort *param_2, HMENU param_3, longlong param_4,
                               longlong param_5, longlong *param_6);
void FUN_140018148(HMODULE a, LPCSTR b);
void FUN_140018178(HMODULE a, LPWSTR b, DWORD c);
HMODULE FUN_1400181b8(LPCWSTR a);
void FUN_140017908(void);
DWORD FUN_1400717f0(HKEY a, LPCWSTR b);
int _vsnwprintf();
undefined8 PECMD_BindDelayImports(longlong param_1, longlong param_2);
int StartServiceW(void *svc, unsigned long argc, const unsigned short **argv);
void PECMD_InstallWindowsService(LPCWSTR param_1);
unsigned int FUN_140022e24(void);
void FUN_14001c2cc(const unsigned short *a, int b, int c);
undefined8 FUN_1400612cc(longlong *param_1);
int FUN_140072924(uint param_1, LPCWSTR param_2, uint param_3, longlong *param_4, ushort *param_5);
int FUN_1400664dc(undefined8 param_1, int param_2);
DWORD FUN_1400195f0(undefined8 param_1, longlong param_2, int param_3, undefined8 *param_4);
ulonglong FUN_140079cf8(undefined8 *param_1, undefined2 *param_2, char param_3);
int FUN_140078c68(LPCSTR param_1, int param_2, uint param_3);
int PECMD_SetEnvIfNonEmpty(undefined8 param_1, LPCWSTR param_2, LPCWSTR param_3);
void FUN_14001e6bc(longlong *param_1, LPCWSTR param_2, LPCWSTR param_3, longlong param_4);
void FUN_14007d0ac(longlong *param_1, LPCWSTR param_2, LPCWSTR param_3);
void FUN_14007e01c(longlong param_1, uint param_2, LPWSTR param_3);
longlong FUN_140072cc0(void);
void PECMD_TokenWithRef(longlong *param_1, longlong *param_2, longlong *param_3, longlong *param_4);
undefined4 PECMD_AdapterMacIpLookup(undefined1 *param_1, longlong *param_2, int param_3);
LPWSTR FUN_1400bf358(longlong *a, pthreadmbcinfo b, undefined8 *c);
uint64_t *PECMD_InitObjSlot2A(uint64_t *param_1, uint param_2, uint64_t param_3);
undefined1 *FUN_14005b848(undefined1 *a);
longlong *FUN_140070398(longlong *a, undefined8 *b);
void FUN_1400e7664(LPCWSTR a);
void *FUN_1400e7840(longlong *a, int b);
uint PECMD_ArgTokenize(longlong *param_1, LPCWSTR param_2, int param_3);
ulonglong FUN_1400ffa38(longlong param_1, longlong *param_2, byte *param_3, longlong *param_4);
uint64_t WindowFromPoint(POINT p);
int IsChild(void *parent, void *child);
uint64_t AttachThreadInput(DWORD a, DWORD b, int c);
int GetScrollInfo(void *h, int bar, SCROLLINFO *si);
int SetScrollPos(void *h, int bar, int pos, int redraw);
uint64_t KillTimer(void *h, uintptr_t id);
HWND FUN_1400d9818(HDC param_1, uint param_2, HDC param_3, POINT param_4);
int ExtTextOutW(void *hdc, int x, int y, uint opt, void *rc, const uint16_t *s, int c,
                const int *dx);
int GetTextMetricsW(void *hdc, void *tm);
int SubtractRect(RECT *dst, const RECT *a, const RECT *b);
uint64_t MoveToEx(void *hdc, int x, int y, void *old);
uint64_t LineTo(void *hdc, int x, int y);
void *CreatePen(int style, int width, COLORREF color);
COLORREF FUN_1400e68e0(HDC param_1, RECT *param_2, COLORREF param_3);
undefined8 FUN_140067e88(longlong *param_1, longlong *param_2);
void FUN_1400f6db0(longlong param_1, longlong param_2);
int FUN_1400633a8();
extern uint64_t DAT_14013e168[8];
extern uint64_t DAT_14013e190[8];
extern uint64_t PTR_u_a__a_zA_Z0_9___14013a2d0[];
extern const uint8_t DAT_14011dcb0[16];
extern const uint16_t DAT_14011d108[];
extern uint8_t DAT_14013d5c0[8];
extern uint8_t DAT_1401210f8[8];
extern uint8_t DAT_14013a838[16];
extern longlong DAT_14013cb18[8];
extern uint8_t DAT_14011d308[8];
extern uint8_t DAT_14013e300[512];
extern uint8_t DAT_1401206f4[8];

#endif /* STUBS_COMMON_H */
