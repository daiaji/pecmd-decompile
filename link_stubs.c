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
typedef intptr_t LRESULT;
typedef unsigned long ULONG_PTR;
typedef uint32_t ACCESS_MASK;
typedef uint16_t WORD;
typedef int16_t SHORT;
typedef struct _SECURITY_ATTRIBUTES { ULONG nLength; void *lpSecurityDescriptor; BOOL bInheritHandle; } SECURITY_ATTRIBUTES;
typedef SECURITY_ATTRIBUTES *LPSECURITY_ATTRIBUTES;
typedef struct _OVERLAPPED { ULONG_PTR Internal; ULONG_PTR InternalHigh; union { struct { ULONG Offset; ULONG OffsetHigh; } s; void *Pointer; } u; void *hEvent; } OVERLAPPED;
typedef OVERLAPPED *LPOVERLAPPED;
typedef struct _FILETIME { DWORD dwLowDateTime; DWORD dwHighDateTime; } FILETIME, *LPFILETIME;
/* PROCESSENTRY32W — 进程枚举结构 (core_b1_remaining.c 同款布局; PECMD_EnumProcessInfo 还原用) */
typedef struct tagPROCESSENTRY32W {
    DWORD      dwSize;
    DWORD      cntUsage;
    DWORD      th32ProcessID;
    uintptr_t  th32DefaultHeapID;
    DWORD      th32ModuleID;
    DWORD      cntThreads;
    DWORD      th32ParentProcessID;
    LONG       pcPriClassBase;
    DWORD      dwFlags;
    WCHAR      szExeFile[260];
} PROCESSENTRY32W;
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
static void PECMD_AppendLongDecimal(int64_t *a, uint64_t b, const uint16_t *c);
static void PECMD_FormatI64Dec(const uint16_t *dst, uint64_t v);
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
void PECMD_ExecInterpString(uint64_t a, void *b);
/* PECMD_ExecInterpString 实体内调用 (rename_map 命名; 定义在其它 core_*.c) */
int64_t  PECMD_ExecuteCommand(int64_t *p1, LPCWSTR p2, int64_t p3, LPCWSTR p4, uint32_t p5, int64_t p6, int p7);
void     PECMD_FreeTripleString(int64_t *p);
static void PECMD_AllocWStringBuffer(void *p, long long count);
uint64_t FUN_14004fb44(longlong *a, ulonglong b);
void FUN_14005d9a8(int64_t a, int b);
uint64_t FUN_1400630d0(int a);
void PECMD_CleanupFrameChain(int64_t a);
void PECMD_ArrayAppend(int64_t script, int64_t obj); /* rename_map: FUN_14006e8a4; 定义在 core_b3e.c */
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
static void PECMD_AllocWStringBuffer(void *p, long long count);
static void PECMD_FreeStrBuf(void *ps);
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
/* ---- PECMD_Dialog2727Proc 恢复体所需前置声明 (定义在文件后部/其它 core_*.c; 命名按 tools/rename_map.json) ---- */
extern void *g_hFontE2B0;                          /* DAT_14013e2b0 (core_globals.c: HFONT, 已有定义) */
extern void PECMD_GetUiFontById(void **pfont, UINT id);  /* @0x1400e648c 按 lang 创建字体 (core_exec.c) */
extern void PECMD_TrimWorkingSet(char force);      /* @0x14005d4e4 (core_b3e.c) */
int      GetSystemMetrics(int idx);
HWND     GetDlgItem(void *hWnd, int nIDDlgItem);
int      SetWindowTextW(void *hWnd, const void *lpString);
uint64_t GetWindowTextW(void *hWnd, WCHAR *lpString, int nMaxCount);
int      LoadStringW(void *h, unsigned int id, WCHAR *buf, int n);
uint64_t lstrcmpW(const uint16_t *a, const uint16_t *b);
int      GetWindowRect(void *w, void *r);
int      SetWindowPos(void *w, void *after, int x, int y, int cx, int cy, unsigned int f);
int      EnableWindow(HWND w, int en);
void     FUN_140102a90(uint64_t *dst, uint64_t v, uint64_t n);          /* CRT memset 内联 */
uint64_t PECMD_ShutPowerAction(uint64_t a);                                     /* 命令入队 (2369 桩) */
extern uint8_t DAT_14011c638[64];                                       /* 默认命令串缓存 (5600 定义) */
void     PECMD_StrDupAssign(uint16_t **ps, const uint16_t *src);        /* @0x1400702b0 */
uint64_t PECMD_EncodeStringId(LPCWSTR name, uint64_t *out, char mode);  /* @0x14006b1e8 */
/* ========== PECMD_Dialog2727Proc @0x14003e220 ==========
 * “确认/密码”模态对话框过程 (dialog 0x2727, DialogBoxParamW 回调):
 *   0x110 初始化居中 + 用 PECMD_LangLookupById 填文本; 0x111 按钮分支
 *   (2=清空命令串, 0xd=置 "-force " 并执行, 1=校验输入命令/密码).
 * 忠实移植 decompiled.c @36417; 内部调用按 rename_map: FUN_1400169bc→
 * PECMD_LangLookupById, FUN_140063620→PECMD_AllocStrSlot, FUN_14006b1e8→
 * PECMD_EncodeStringId, FUN_1400702b0→PECMD_StrDupAssign, FUN_14005d4e4→
 * PECMD_TrimWorkingSet; PECMD_ShutPowerAction/FUN_140102a90/PECMD_FreeStrBuf 保持原名.
 * 上下文: DAT_14013cfb0 (PECMD_ShowIdDialog 置入的 4 槽数组), 槽[1] 低32位为
 * 剩余次数, 槽[0] 为原始命令串; g_hFontE2B0 字体缓存; DAT_14011c638 默认命令串.
 * 返回: 0x110 处理返回 1, 其余 0 (与对话框 proc 约定一致). */
long long PECMD_Dialog2727Proc(void *a, unsigned int b, uint64_t c, uint64_t d)
{
    (void)d;
    HWND hwnd = (HWND)a;
    uint64_t *puVar1;
    int iVar2;
    int iVar3;
    HWND pHVar4;
    LPWSTR pWVar5;
    LPCWSTR pWVar6;
    WCHAR *lpString1;
    union {
        struct { LONG left; LONG top; LONG right; LONG bottom; } r;   /* tagRECT 布局 (typedef 在文件后部) */
        uint64_t p;
    } local_918;
    WCHAR local_908[104];
    WCHAR local_838[1024];
    uint16_t local_38 = 0;

    puVar1 = (uint64_t *)DAT_14013cfb0;
    if (b == 0x110) {
        iVar2 = GetSystemMetrics(0x3d);
        iVar3 = GetSystemMetrics(0x3e);
        if (g_hFontE2B0 == 0) {
            PECMD_GetUiFontById((void **)&g_hFontE2B0, 0x3ea);
        }
        puVar1[2] = (uint64_t)(uintptr_t)g_hFontE2B0;
        GetWindowRect(hwnd, &local_918.r);
        SetWindowPos(hwnd, (HWND)(intptr_t)-2,
                     ((local_918.r.left - local_918.r.right) + iVar2) / 2,
                     ((local_918.r.top - local_918.r.bottom) + iVar3) / 2,
                     0, 0, 0x41);
        pWVar6 = PECMD_LangLookupById(0x3f4, 0);
        if (pWVar6 != 0) {
            pHVar4 = GetDlgItem(hwnd, 1);
            SetWindowTextW(pHVar4, pWVar6);
        }
        pWVar6 = PECMD_LangLookupById(0x3f5, 0);
        if (pWVar6 != 0) {
            pHVar4 = GetDlgItem(hwnd, 2);
            SetWindowTextW(pHVar4, pWVar6);
        }
        pWVar6 = PECMD_LangLookupById(0x3fb, 0);
        if (pWVar6 != 0) {
            pHVar4 = GetDlgItem(hwnd, 0xb);
            SetWindowTextW(pHVar4, pWVar6);
        }
        pWVar6 = PECMD_LangLookupById(0x2788, 0);
        if (pWVar6 != 0) {
            pHVar4 = GetDlgItem(hwnd, 0xc);
            SetWindowTextW(pHVar4, pWVar6);
        }
        pWVar6 = PECMD_LangLookupById(0x3fd, 0);
        if (pWVar6 != 0) {
            pHVar4 = GetDlgItem(hwnd, 0xd);
            SetWindowTextW(pHVar4, pWVar6);
        }
        PECMD_TrimWorkingSet('\0');
        return 1;
    }
    if (b == 0x111) {
        if ((uint16_t)c == 2) {
            pHVar4 = GetDlgItem(hwnd, 0x757d);
            SetWindowTextW(pHVar4, (LPCWSTR)(void *)DAT_14011c638);
        }
        else {
            if ((uint16_t)c == 0xd) {
                PECMD_StrDupAssign((uint16_t **)&local_918.p, (const uint16_t *)L"-force ");
                PECMD_ShutPowerAction(local_918.p);
            }
            else {
                if ((uint16_t)c != 1) {
                    return 0;
                }
                local_838[0] = L'\0';
                PECMD_AllocStrSlot((uint16_t **)&local_918.p);
                pHVar4 = GetDlgItem(hwnd, 0x757d);
                GetWindowTextW(pHVar4, local_838, 0x400);
                local_38 = 0;
                PECMD_EncodeStringId(local_838, (uint64_t *)&local_918.p, '\0');
                FUN_140102a90((ulonglong *)local_838, 0, 0x802);
                pWVar5 = StrChrW(local_838, L',');
                if (pWVar5 == 0) {
                    local_908[0] = L'\0';
                    LoadStringW(DAT_14013ca68, 0x2729, local_908, 100);
                    for (lpString1 = local_908;
                         (*lpString1 == L'*' ||
                          (((8 < (ushort)*lpString1 && ((ushort)*lpString1 < 0xe)) ||
                            (*lpString1 == L' '))));
                         lpString1 = lpString1 + 1) {
                    }
                    iVar2 = lstrcmpW((LPCWSTR)(uintptr_t)*puVar1,
                                     (LPCWSTR)((uint8_t *)(uintptr_t)local_918.p + 2));
                    if ((iVar2 == 0) ||
                        ((*lpString1 != L'\0' &&
                          (iVar2 = lstrcmpW(lpString1,
                                            (LPCWSTR)((uint8_t *)(uintptr_t)local_918.p + 2)),
                           iVar2 == 0)))) {
                        EndDialog((uint64_t)(uintptr_t)hwnd, 1);
                    }
                    else if (*(int *)(puVar1 + 1) == 1) {
                        pHVar4 = GetDlgItem(hwnd, 0x757d);
                        SetWindowTextW(pHVar4, (LPCWSTR)(void *)DAT_14011c638);
                        pHVar4 = GetDlgItem(hwnd, 1);
                        EnableWindow(pHVar4, 0);
                        pHVar4 = GetDlgItem(hwnd, 2);
                        ShowWindow(pHVar4, 0);
                        pHVar4 = GetDlgItem(hwnd, 1);
                        ShowWindow(pHVar4, 0);
                        pHVar4 = GetDlgItem(hwnd, 0xd);
                        ShowWindow(pHVar4, 5);
                    }
                    else {
                        *(int *)(puVar1 + 1) = *(int *)(puVar1 + 1) - 1;
                    }
                }
            }
            PECMD_FreeStrBuf((WCHAR **)&local_918);
        }
    }
    return 0;
}
void FUN_140062950(void *a) { (void)a; }
/* @0x1400e66d4 size=— 资源字体创建(直移) */
void PECMD_BuildResourceFont(longlong *param_1, UINT param_2, void *param_3)
{
  if (*param_1 != -1) {
    uint64_t local_res8 = 0;
    PECMD_AllocWStringBuffer((WCHAR **)&local_res8,0x516);
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
    PECMD_FreeStrBuf(&local_res8);
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
int    (*DAT_14013d820)(int) = 0;    /* CoUninitialize 槽 (PECMD_GetApiProcCached("CoUninitialize",...) 装载) */
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
int64_t  PECMD_CtlForwardSetBlock(longlong *a, void *b);
void    *RegisterServiceCtrlHandlerW(const WCHAR *name, void *handler);
int      SHGetSpecialFolderPathA(void *hwnd, char *buf, int folder, int create);
char    *lstrcatA(char *dst, const char *src);
typedef union _LARGE_INTEGER { struct { DWORD LowPart; LONG HighPart; } s; struct { DWORD LowPart; LONG HighPart; } u; long long QuadPart; } LARGE_INTEGER;   /* wave-4 (补 .s 成员供 Ghidra 直移) */
typedef union _ULARGE_INTEGER { struct { DWORD LowPart; DWORD HighPart; } s; unsigned long long QuadPart; } ULARGE_INTEGER;
typedef uint64_t undefined7;   /* Ghidra 7字节伪类型 (按 64 位承载) */
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
typedef void *PACL; typedef void *PSID; typedef void *PSECURITY_DESCRIPTOR;
#define GRANT_ACCESS 1
#define TRUSTEE_IS_NAME 1
#define TRUSTEE_IS_USER 1
#define SE_KERNEL_OBJECT 6
typedef struct _TRUSTEE { void *pMultipleTrustee; uint32_t MultipleTrusteeOperation; uint32_t TrusteeForm; uint32_t TrusteeType; void *ptstrName; } TRUSTEE;
typedef struct _EXPLICIT_ACCESS_W { uint32_t grfAccessPermissions; uint32_t grfAccessMode; uint32_t grfInheritance; TRUSTEE Trustee; } EXPLICIT_ACCESS_W;
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
int         PECMD_ParseNumTryWriteback(long long *, int *);
int         lstrlenW(const WCHAR *);
long long  *PECMD_SkipLeadingControlChars(long long *);
long long   FUN_140064a34(uint16_t *);
long long   PECMD_WideStrToInt64(uint16_t *);
long long   FUN_14006b8fc(long long *);
void        PECMD_InitButtonState(long long, uint, uint64_t);
void        FUN_140005344(void);
uint16_t   *FUN_14000531c(uint16_t *);
uint8_t    *PECMD_MemMoveForward(uint8_t *, uint8_t *, int);
void        PECMD_StopDeleteService(uint16_t *);
void        FUN_140017048(const WCHAR *);
void        FUN_140021144(const WCHAR *);
void        FUN_140019da8(long long, const WCHAR *, int);
static void PECMD_AllocWStringBuffer(void *p, long long count);
void        FUN_140025f10(longlong, const WCHAR *, uint32_t, char *, char *, long long *);
void        FUN_140020fd4(long long, char, const WCHAR *, const WCHAR *);
extern uint8_t DAT_14011c638[64];
uint64_t    FUN_14005ea5c(void);
uint        FUN_14000e0bc(void);
uint64_t    FUN_14006042c(void);
uint64_t    PECMD_LoadNtdllApis(void);
LPWSTR      StrRChrW(const WCHAR *, const WCHAR *, WCHAR);
uint64_t    thunk_FUN_1400f429c(void *, short);
extern void *PECMD_GrowByteBuffer(void **ps, int64_t len);   /* @0x140063424 (真实体 core_thread.c) */
void        PECMD_HeapFreeWithHeader(long long *param_1);               /* @0x14005b0d4 串槽释放 (桩见文件后部) */
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
uint64_t AssignProcessToJobObject(void *job, void *proc) { (void)job;(void)proc; return 0; }   /* arity 修正 0->2 (PECMD_ExecCmdDispatch 恢复体) */
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
uint64_t SetEntriesInAclW(int c, void *ea, void *old, void **newa);
uint64_t GetSecurityInfo(void *h, int t, uint32_t i, void **a, void **b, void **c, void **d, void **e);
uint64_t SetSecurityInfo(void *h, int t, uint32_t i, void *a, void *b, void *c, void *d);
uint64_t LocalFree(uint64_t hMem);
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
uint64_t CreatePen(void) { return 0; }
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
uint64_t DestroyIcon(void) { return 0; }
uint64_t DestroyMenu(void *h) { (void)h; return 0; }   /* arity 修正 0->1 */
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
uint64_t ExpandEnvironmentStringsW(LPCWSTR src, LPWSTR dst, DWORD n) { (void)src;(void)dst;(void)n; return 0; }   /* arity 修正 0->3 (PECMD_ToSysCopyFiles 直移) */
uint64_t ExtTextOutW(void) { return 0; }
uint64_t PECMD_RunCommandLine(void *script, WCHAR **str, int mode) { (void)script;(void)str;(void)mode; return 0; }   /* arity 修正 0->3 (PECMD_ExecCmdDispatch 恢复体 2 处调用) */
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
void PECMD_InitSystemApiGlobals(void){
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
    PECMD_AppendLongDecimal((int64_t*)&DAT_14013d130,(uint32_t)DAT_14013a038,(const uint16_t*)L"&bX64");
    PECMD_AppendLongDecimal((int64_t*)&DAT_14013d130,8,(const uint16_t*)L"&ptrlen");
    PECMD_AppendLongDecimal((int64_t*)&DAT_14013d130,(uint32_t)DAT_14013c9f8,(const uint16_t*)L"&WM_TASKBARBUTTONCREATED");
}

/* ========== PECMD_ExecCmdDispatch @ 14000e26c  size=33864 ========== */
/* signature: _FILETIME __fastcall PECMD_ExecCmdDispatch(longlong*, _FILETIME, longlong*, _FILETIME, uint,
 *            _FILETIME*, _FILETIME, undefined8*)
 * 移植: decompiled.c @7726 (3948 行) 分段恢复; 桩签名 8×uint64_t 保持不变.
 * _FILETIME 形参/局部视为 8 字节不透明值 (FTU64 联合: .v 标量, .ft.dwLow/High 字段),
 * CONCAT/字节片按 Ghidra 语义展开; 内部调用按 tools/rename_map.json 命名 (未映射用原名);
 * L"..." 按本文件惯例强转 (const uint16_t*). */
typedef union FTU64 { uint64_t v; FILETIME ft; } FTU64;   /* 本函数 _FILETIME 不透明值局部 */
uint8_t DAT_14013c968 = 0;                                 /* @0x14013c968 标志 (初值=f2 数据 0x00) */
uint64_t DAT_14013ca90 = 0;                                /* @0x14013ca90 惰性 LPCWSTR 槽 (cmd.exe 路径缓存, 运行时初始化) */
short DAT_14013ca4c = 0;                                   /* @0x14013ca4c cmd.exe 路径长 (short, 运行时赋值) */
/* ---- 前置声明 (恢复体专用; 定义见各 core_*.c / 本文件后部) ---- */
extern WCHAR *g_pNextCmd;                                  /* DAT_14013cac8 (core_globals.c) */
extern void *PECMD_StrBldCopyWide(void *a, const WCHAR *b);   /* def @4100 */
extern longlong *PECMD_AppendWideStr(void *param_1p, LPCWSTR param_2);  /* def @3844 */
extern uint64_t *PECMD_ResetSlots(uint64_t *p);            /* core_b1_remaining.c */
extern uint64_t PECMD_ParseIntegerString(long long *ps, uint64_t *out);  /* def @3956 */
extern int  PECMD_AsciiPrefixICmp(const char *s, const WCHAR *w, int n);  /* def @3722 */
extern int64_t *PECMD_SkipTokenWs(int64_t *pp);                            /* core_b1_remaining.c:3271 */
extern int64_t PECMD_RunCommand(void *script, WCHAR *cmdline);          /* @0x140031454 def core_scriptrun.c:71 */
extern uint16_t *PECMD_AllocStrSlot(uint16_t **out);       /* def @本文件后部 */
extern long long *PECMD_SkipLeadingControlChars(long long *);    /* def @3944 */

extern WCHAR *PECMD_StrCopyW(WCHAR **ps, LPCWSTR src, int64_t len);        /* core_var2.c */
extern uint64_t PECMD_RunCommandLine(void *script, WCHAR **str, int mode);   /* 定义本文件@653 (arity 0->3 修正, 返回保持 uint64_t) */
extern char PECMD_MatchTokenAdvance(char *tok, void *pp, int n);             /* def @4432 */
extern char PECMD_MatchAndPad(int64_t key, int64_t *pp, int len);            /* core_b3d.c */
extern bool PECMD_ParseNumSkipWs(WCHAR **pp, uint64_t *out);                 /* core_remaining_helpers.c */
extern int64_t PECMD_ParseVarArg(int64_t *, int64_t *, int64_t *, int *, uint32_t *);  /* core_b1_remaining.c */
extern uint64_t PECMD_FindAliveProcessId(uint64_t);                          /* core_b1_remaining.c */
extern uint16_t *PECMD_NextToken(int64_t *, int64_t *, uint32_t);            /* def @本文件 1605 */
extern void PECMD_ReleaseObjectResources(int64_t *p);                        /* core_b1_remaining.c */
extern void PECMD_ExpandVarDispatch(void *p1, WCHAR *p2, void *p3, int p4, uint8_t p5); /* def @4698 */
const uint8_t DAT_14011dd24[] = "catch";   /* @0x14011dd24 真值 "catch" (rdata) — PECMD_MatchAndPad key */


extern uint64_t PECMD_MatchAssignToken(char *key, int64_t *pp, int len);   /* def core_b3f.c */
extern bool PECMD_ParseHexOrDecBool(long long *param_1, int *param_2);      /* def @5161 */
extern bool PECMD_ParseUIntValue(int64_t *pp, int *out);                    /* core 定义 */
extern uint64_t PECMD_GetModuleFileNameIntoContainer(uint64_t *out);        /* core_b1_remaining.c */
extern uint64_t PECMD_ExpandPathAlloc2(LPCWSTR p1, uint64_t *p2, int64_t *p3); /* def @6780 */
extern unsigned long GetModuleFileNameW(void *h, WCHAR *buf, unsigned long n); /* def @6222 */
extern uint64_t SHGetSpecialFolderPathW(HWND, WCHAR *, int, BOOL);         /* def 本文件后部 */
extern uint64_t ReadFile(void *h, void *buf, unsigned long n, unsigned long *read, void *ol); /* def 本文件后部 */
extern uint64_t StrCmpNIA(const char *a, const char *b, int n);            /* 桩 本文件后部 (arity 0->3) */
extern uint16_t g_privFlags;                                               /* DAT_14013e20c (core_globals.c) */
extern uint8_t g_flagCCC9;                                                 /* DAT_14013ccc9 (core_globals.c) */
extern int64_t PECMD_EnableTokenPrivilege(LPCWSTR priv, DWORD attr, uint32_t flag); /* def core_scriptrun 区 */
extern int64_t *PECMD_InitObjectWithParent(int64_t *ps, int64_t parent);   /* core_b1_remaining.c:9102 */
extern void PECMD_ExpandScriptVariables(int64_t *p1, int64_t *p2, int64_t *p3);     /* core_b3_remaining.c */
extern uint8_t *PECMD_MemMoveForward(uint8_t *dst, uint8_t *src, int n);   /* 既存声明@527; 桩=memmove 本文件后部 */
extern uint64_t PECMD_AddVarDefault(void *script, LPCWSTR name, LPCWSTR val, int len, int64_t flag); /* 桩本文件后部 (arity 0->5) */
extern void PECMD_ClearTaskTable(uint64_t script, int mode);               /* def 本文件@6472 (undefined8→uint64_t) */
extern uint64_t PECMD_VarLookup(int64_t *a, const uint16_t *b, int64_t *c, int64_t d, void *e); /* 既存声明@5072 */
extern uint64_t PECMD_NextRandomSeed(void);                                /* @0x14005e04c */
extern void PECMD_GetApiProcCached(LPCSTR p1, LPCSTR p2, longlong *slot, longlong *hmod);  /* def 本文件@6284 */
static uint64_t PECMD_ZeroLenBuf(void *a);                                    /* 桩 本文件后部 */
extern void PECMD_WideToAnsiStr(int64_t *ps, LPCWSTR src, int64_t len, uint64_t cap); /* core_b3a.c */
extern uint64_t GetEnvironmentStringsW(void);                              /* 桩 本文件后部 */
extern uint64_t FreeEnvironmentStringsW(LPWSTR env);                       /* 桩 本文件后部 (arity 0->1) */
extern void Sleep(unsigned long ms);                                       /* 桩 本文件后部 */
extern uint64_t wsprintfA(char *out, const char *fmt, ...);                /* 桩本文件后部 (arity 0->可变) */
extern uint64_t CreateProcessWithLogonW(void *user, void *domain, void *pwd, DWORD flags,
        void *app, WCHAR *cmd, DWORD flg2, void *env, void *dir, void *si, void *pi); /* 桩本文件后部 (arity 0->11) */
extern BOOL PECMD_CreateProcessAsUser(LPCWSTR, LPWSTR, LPSECURITY_ATTRIBUTES, LPSECURITY_ATTRIBUTES,
        BOOL, uint32_t, LPVOID, LPCWSTR, LPSTARTUPINFOW, uint64_t /*LUID 值*/,
        uint32_t, LPCWSTR, LPCWSTR, LPCWSTR);                                /* def core_b1_remaining.c:3305 */
static int64_t PECMD_CreateInjectedProcess(void *p1, uint32_t p2, WCHAR *p3, DWORD p4, DWORD p5, void *p6,
                      LPCWSTR p7, void *p8, void *p9, LPCWSTR p10) {
    (void)p1;(void)p2;(void)p3;(void)p4;(void)p5;(void)p6;(void)p7;(void)p8;(void)p9;(void)p10;
    return 0;                                                                /* 新增桩 @0x1400e7414 (未还原 helper, 失败保守) */
}
extern short *PECMD_ResolveWildcardPath(uint64_t *p1, short *p2, LPWSTR p3, LPCWSTR p4); /* core_b1_remaining.c:2348 */
extern bool PECMD_IsFile(LPCWSTR path);                                      /* def core_b1_remaining.c:2398 */
extern void *PECMD_GrowByteBuffer(void **ps, int64_t len);                   /* def core_thread.c:28 */
extern LPCWSTR PECMD_ParseResourceStringRef(LPCWSTR *p1, uint p2);           /* def 本文件@6995 */
extern uint64_t PECMD_ParseItemImageSpec(longlong *a, longlong *b, uint16_t *c);        /* 桩 本文件@7365 */
extern void *DAT_14013cf70;                                                  /* HINSTANCE 槽 (def 行~12531) */
extern uint64_t LoadLibraryExW(LPCWSTR path, void *file, uint32_t flags);    /* 既存声明@7395 前移 */
extern uint64_t FreeLibrary(void *h);                                        /* 既存声明@7394 前移 */
extern uint8_t *PECMD_LoadEncodedResource(void *a, uint16_t *b, uint16_t *c, int64_t *d, unsigned int *e); /* def 本文件@3823 */
extern uint64_t PECMD_SkipWCharUntil(void *pp, uint16_t ch);                 /* 桩本文件后部 (arity 0->2) */
extern int64_t *PECMD_AssignString(int64_t *ps, LPCWSTR src);                /* @0x14007034c core_b1_remaining.c */
extern int PECMD_CreateProcessW(LPCWSTR cmd, WCHAR *buf, LPSECURITY_ATTRIBUTES sa,
        LPSECURITY_ATTRIBUTES da, BOOL inherit, DWORD flags, LPVOID env, LPCWSTR cwd,
        STARTUPINFOW *si, PROCESS_INFORMATION *pi);                          /* @0x140101e04 core_b1_remaining.c */
extern BOOL PECMD_LaunchServiceProcess(LPCWSTR a, LPCWSTR b);                /* def core_b1_remaining.c:875 */
extern int PECMD_IsSysStartuped(void);                                       /* def core_b2a.c:341 */
extern bool FUN_140101e70(const WCHAR *name);                                /* 真体本文件后部 (签名修正: bool(LPCWSTR), 同 core_b3_remaining:652) */
extern uint64_t PECMD_GenerateTimeText(LPCWSTR p1, int64_t *p2, uint64_t p3, uint64_t p4, uint64_t p5); /* def core_b3_remaining.c:7183 */
extern HANDLE PECMD_CreateTempMutexDir(int64_t *p1, int64_t *p2, uint64_t *p3, uint64_t name); /* def core_b1_remaining.c:3462 */
extern void PECMD_FlushFileThrice(HANDLE hFile);                             /* def core_b3b.c:217 */
extern void *PECMD_DriverInstall(int64_t *script, LPCWSTR line);            /* def core_b3_remaining.c:1658 (返回 void*) */
extern uint64_t WaitForSingleObject(void *h, uint64_t ms);                   /* 桩本文件@9717 */
extern uint64_t WriteFile(void *h, void *buf, unsigned long n, unsigned long *written, void *ol); /* 桩本文件后部 */
extern uint64_t OpenProcess(DWORD access, BOOL inherit, DWORD pid);          /* 桩本文件后部 */
extern uint64_t ResumeThread(void *hThread);                                 /* 桩本文件后部 (arity 0->1) */
extern uint64_t CreateProcessW(LPCWSTR app, const uint16_t *cmd, LPSECURITY_ATTRIBUTES sa,
        LPSECURITY_ATTRIBUTES ta, BOOL inh, DWORD flags, void *env, LPCWSTR dir,
        void *si, void *pi);                                                 /* 桩本文件后部 (arity 0->10) */
extern uint64_t AssignProcessToJobObject(void *job, void *proc);             /* 桩本文件后部 (arity 0->2) */
extern uint64_t CreateJobObjectW(LPSECURITY_ATTRIBUTES sa, LPCWSTR name);    /* 桩本文件后部 (arity 0->2) */
extern uint64_t CreateProcessAsUserW(void *tok, LPCWSTR app, WCHAR *cmd, LPSECURITY_ATTRIBUTES sa,
        LPSECURITY_ATTRIBUTES ta, BOOL inh, DWORD flags, void *env, LPCWSTR dir, void *si, void *pi); /* 桩后部 (arity 0->11) */
extern uint64_t GetShortPathNameW(LPCWSTR path, WCHAR *buf, DWORD len);      /* 桩本文件后部 (arity 0->3) */
extern void PECMD_WriteSysAck(uint32_t ack, int mode);                       /* def core_b2b.c:1254 */
extern int PECMD_ReadRegBinaryGuarded(HKEY hkey, LPCWSTR path, LPCWSTR name,
        longlong *buf, LPDWORD size, long *extra);                           /* def 本文件@16216 同型 (LSTATUS≡long, typedef 在后) */
extern void PECMD_TlsLogWrite(uint64_t ctx, LPCWSTR fmt, uint64_t a, uint64_t b); /* @0x140018d8c */
extern uint64_t PECMD_NotifyMainWindowRefresh(uint64_t a, int b);           /* 既存声明@6298/def@8341 */
extern void PECMD_RunSysInit(void *script, LPCWSTR name);                    /* def core_scriptdep.c:166 */
extern LARGE_INTEGER PECMD_ProcessScriptBlock(LARGE_INTEGER p1, LARGE_INTEGER p2,
        longlong *p3, longlong *p4, pthreadmbcinfo p5);                      /* def 本文件@6341 */
extern WCHAR *PECMD_ResolveVariable(int64_t *a1, LPCWSTR a2, uint64_t *a3, uint32_t a4); /* def 本文件@8621 */
extern void PECMD_StartOnlyApp(LPCWSTR cmdline);                             /* def core_b2b.c:844 */
extern HWND g_hwndC9C8;                                                      /* DAT_14013c9c8 (core_globals.c:529) */
uint8_t DAT_14013cb09 = 0;                                                   /* @0x14013cb09 标志 (运行时置位) */
static void PECMD_PatchRemoteWinExec(void *h) { (void)h; }                                     /* 新增桩 @0x1400229f8 (未映射 helper) */
BOOL ConnectNamedPipe(void *pipe, OVERLAPPED *ol) { (void)pipe;(void)ol; return 1; }          /* 新增桩 */
BOOL GetOverlappedResult(void *pipe, OVERLAPPED *ol, DWORD *bytes, BOOL wait) {               /* 新增桩 */
    (void)pipe;(void)ol;(void)wait; if (bytes) *bytes = 0; return 1;
}
extern uint64_t MsgWaitForMultipleObjects(DWORD count, HANDLE *handles, BOOL waitAll,
        DWORD timeout, DWORD mask);                                          /* 桩本文件后部 (arity 0->5) */
extern uint64_t WaitForInputIdle(HANDLE proc, DWORD ms);                     /* 桩本文件后部 (arity 0->2) */
extern int32_t lstrlenA(const char *s);                                      /* 桩本文件@10077 */
extern void SetLastError(DWORD e);                                           /* 桩本文件@9957 */
extern uint64_t SetProcessWorkingSetSize(void *h, uint64_t a, uint64_t b);   /* 桩本文件@9963 */
extern int64_t PECMD_AdjustTokenPrivilege(LPCWSTR priv, DWORD attr, HANDLE h); /* def core_b1_remaining.c:8038 */
extern HANDLE PECMD_RestrictedTokenSetup(HANDLE base);                       /* def core_b1_remaining.c:2311 */
extern void PECMD_VarSetUInt(void *s, uint64_t v, LPCWSTR k);                /* core_b2f.c:101 extern 同型 */
extern void PECMD_WrapParamCall_02d8(int64_t *obj, uint64_t value, LPCWSTR text); /* def core_b3a.c:66 */
extern void PECMD_ScheduleSelfDelete(LARGE_INTEGER ft, int pid);             /* def core_b1_remaining.c:1224 */
uint64_t DAT_14013c960 = 0;                                                /* @0x14013c960 函数槽 (GetProcessID, 运行时装载) */
uint64_t ShellExecuteExW(void *pei) { (void)pei; return 1; }               /* 新增桩 (SEG7 ShellExecuteEx 路径) */
HANDLE CreateNamedPipeA(LPCSTR name, DWORD openMode, DWORD pipeMode, DWORD maxInst,
                        DWORD outSz, DWORD inSz, DWORD timeout, LPSECURITY_ATTRIBUTES sa) {
    (void)name;(void)openMode;(void)pipeMode;(void)maxInst;(void)outSz;(void)inSz;(void)timeout;(void)sa;
    return (HANDLE)0xffffffffffffffff;   /* INVALID_HANDLE_VALUE → 走真实失败分支 */
}
BOOL SetNamedPipeHandleState(HANDLE h, DWORD *mode, DWORD *maxCount, DWORD *collectBytes) {
    (void)h;(void)mode;(void)maxCount;(void)collectBytes; return 1;
}
extern int GetCursorPos(void *p);                                           /* def @6907 */
     /* 新增桩 @0x1400c10c0 */
const uint8_t DAT_14011dcb0[16] = {0x2a,0,0,0,'p','p','i','d',':',0,0,0,'h','p','i','d'};
    /* @0x14011dcb0 真值: 2a 00 00 00 "ppid:" 00 00 00 "hpid..." (仅取址) */


extern uint64_t PECMD_OpenProcessMemory(void *param_1, DWORD param_2, long long param_3); /* core_b1_remaining.c */
extern uint64_t PECMD_WaitHandlesOrMessages(uint64_t p1, int64_t p2, int p3, uint64_t *p4);     /* def @原832 */
extern uint64_t PECMD_LoadFileToSlot(LPCWSTR path, int64_t *pp);                                /* core_b2f.c */
extern uint64_t GetCurrentProcess(void);                                                        /* def @原6187 */
extern uint64_t DuplicateHandle(void *hSrcP, void *hSrc, void *hDstP, HANDLE *phDst, DWORD acc, BOOL inh, DWORD opts);
extern uint64_t SetFilePointer(void *f, long dist, LONG *distHi, DWORD how);
extern HANDLE PECMD_OpenFileHandle(HANDLE *out, LPCWSTR path, DWORD access, DWORD share,         /* core_exec2.c */
                                   LPSECURITY_ATTRIBUTES sa, DWORD disp, DWORD flags, HANDLE tmpl);
extern LPCWSTR PECMD_StripTrailingSpaces(LPCWSTR s);                                            /* core_b2c.c */
int CreatePipe(void *out, void *in, void *sa, unsigned long n) { (void)out;(void)in;(void)sa;(void)n; return 1; } /* 新增桩 */
const uint16_t DAT_14011d108[] = {'.','t','m','p',0};   /* @0x14011d108 真值 L".tmp" (rdata) */

extern HANDLE g_hStdIn;  extern HANDLE g_hStdOut;  extern HANDLE g_hStdErr;   /* DAT_14013a330/340/338 (core_globals.c) */
static uint16_t *PECMD_StrBldCopyWideN(void *out, const uint16_t *src, int64_t len);   /* def @本文件 2161 */


extern int64_t PECMD_EnumComMenuItems(uint64_t, uint64_t, int *, int64_t **, HMENU);      /* core_b1_remaining.c */
extern void *PECMD_AllocSmallObject(void **ps);                                                  /* core_cmd4.c */
extern LPCWSTR PECMD_ConvertLfToCrlf(LPCWSTR a, int64_t *b);                                     /* core_b3_remaining.c */
extern int64_t PECMD_DispatchByObjectName(uint64_t *, LPCWSTR, int64_t, uint16_t *, uint64_t, uint64_t); /* core_b3_remaining.c */
extern void PECMD_VarWriteLine(void *script, LPCWSTR key, LPCWSTR data, int64_t len, int64_t *pkey, char mode); /* core_remaining_helpers.c */
extern short *PECMD_UnquoteString(short *param_1);                                               /* def @2798 */
extern int64_t PECMD_FindVarValue(int64_t *, LPCWSTR, int64_t *, int);                           /* core_b3_remaining.c */
extern int64_t *PECMD_ReplaceStringSlot(int64_t *ps, uint64_t *src);                            /* core_b8m.c */
extern uint64_t PECMD_LoadFileMappingExec(LPCWSTR, int64_t *, int64_t *, HANDLE *, LPCWSTR);     /* core_b1_remaining.c */
extern WCHAR *PECMD_StrDupA(WCHAR **ps, LPCWSTR src, int64_t a, int64_t b);                      /* def @3857 */
extern uint64_t FUN_140063060(uint64_t value);                                                   /* core 真实体 */
static int64_t *PECMD_StrBldCopyAnsi(int64_t *out, char *src, uint64_t len);                                  /* def @6008 */
extern uint64_t PECMD_ResDecode(int64_t *ps, uint32_t flag);                                     /* core_b1_remaining.c */
extern void PECMD_ResetScriptChain(longlong *param_1, longlong *param_2);                        /* def @13537 */
extern LPCWSTR PECMD_PinStartMenuTask(int64_t *, LPCWSTR);                                       /* core_b3_remaining.c */
extern unsigned long PECMD_RegSetValueWithOpen(void *, const unsigned short *, const unsigned short *, unsigned long, unsigned char *, unsigned long);


extern uint64_t GetExitCodeProcess(void *h, DWORD *code);              /* def @8611 (arity 修正 0->2) */
extern uint64_t TerminateJobObject(void *h, uint32_t code);           /* def @8873 */
extern uint64_t TerminateProcess(void *h, uint32_t code);             /* def @8874 */
extern uint64_t TrackPopupMenu(void *m, uint32_t f, int x, int y, uint32_t r, void *w, void *rc); /* def @8876 */
extern int wsprintfW(unsigned short *out, const unsigned short *fmt, ...);  /* def @3580/6508 */

uint64_t PECMD_ExecCmdDispatch(uint64_t a, uint64_t b, uint64_t c, uint64_t d, uint64_t e, uint64_t f, uint64_t g, uint64_t h)
{
  longlong *param_1 = (longlong *)(uintptr_t)a;
  uint64_t param_2 = b;
  longlong *param_3 = (longlong *)(uintptr_t)c;
  uint64_t param_4 = d;
  uint param_5 = (uint)e;
  uint64_t *param_6 = (uint64_t *)(uintptr_t)f;
  uint64_t param_7 = g;
  uint64_t *param_8 = (uint64_t *)(uintptr_t)h;
  uint8_t *puVar1;
  bool bVar2;
  uint8_t bVar3;
  bool bVar4, bVar5, bVar6, bVar7;
  WCHAR *pWVar8;
  int64_t lVar9;
  void *pcVar10;
  DWORD DVar11;
  uint64_t uVar12;
  char cVar13;
  bool bVar14;
  char cVar15;
  bool bVar16;
  ushort uVar17, uVar18;
  short sVar19;
  BOOL BVar20;
  DWORD DVar21;
  uint32_t uVar22;
  uint64_t uVar23;
  LPWSTR pWVar24;
  uint64_t extraout_var;
  ulonglong uVar25;
  HANDLE *ppvVar26;
  uint64_t *puVar27;
  WCHAR *pWVar28;
  int64_t *plVar29;
  LPCWSTR pWVar30;
  uint64_t extraout_var_00;
  LPWSTR pWVar31;
  LPCWSTR lpCurrentDirectory;
  uint64_t extraout_var_01, extraout_var_02, extraout_var_03, extraout_var_04,
           extraout_var_05, extraout_var_06, extraout_var_07, extraout_var_08;
  HANDLE pvVar32;
  short *psVar33;
  ushort uVar34;
  uint uVar35, uVar36;
  LPCWSTR *ppWVar37;
  uint64_t *p_Var38;
  FTU64 _Var39;
  int64_t lVar40;
  WCHAR wMatch;
  FTU64 _Var41;
  WCHAR *pWVar42;
  FTU64 _Var43;
  WCHAR *pWVar44;
  HANDLE pvVar45;
  FTU64 lpString;
  uint8_t bVar46;
  FTU64 _Var47, _Var48;
  short sVar49;
  uint16_t *puVar50;
  short sVar51;
  int iVar52;
  FTU64 _Var53, _Var54, _Var55, _Var56;
  short sVar57;
  STARTUPINFOW *p_Var58;
  FTU64 _Var59, _Var60;
  int64_t lVar61;
  WCHAR WVar62;
  FTU64 local_res10;
  longlong *local_res18;
  FTU64 local_res20;
  uint8_t local_e08;
  char local_e07, local_e06;
  uint local_e00;
  int local_dfc;
  DWORD local_df8;
  uint local_df4, local_df0;
  DWORD local_de8;
  uint8_t local_de4;
  uint local_de0;
  uint32_t local_ddc;
  DWORD local_dd8[2];
  FTU64 local_dd0, local_dc8;
  char local_dc0;
  uint8_t local_dbf;
  uint local_dbc;
  DWORD local_db8, local_db4, local_db0;
  int64_t local_da8;
  char local_da0, local_d9f, local_d9e, local_d9d, local_d9c, local_d9b, local_d9a;
  DWORD local_d98[2];
  FTU64 local_d90, local_d88;
  HANDLE local_d80;
  uint16_t local_d78;
  FTU64 local_d70, local_d68;
  DWORD local_d60;
  LPCWSTR local_d58;
  uint local_d50;
  char local_d4c;
  DWORD local_d48;
  char local_d44, local_d43;
  DWORD local_d40;
  char local_d3c, local_d3b;
  LPCWSTR local_d38;
  LARGE_INTEGER local_d30;
  FTU64 local_d28;
  LPCWSTR local_d20;
  FTU64 local_d18, local_d10;
  DWORD local_d08;
  uint local_d04;
  int local_d00;
  uint local_cfc;
  FTU64 local_cf8;
  HANDLE local_cf0;
  uint local_ce8[2];
  LPCWSTR local_ce0;
  WCHAR *local_cd8;
  LPCWSTR local_cd0;
  FTU64 local_cc8;
  LPCWSTR local_cc0;
  FTU64 local_cb8;
  int64_t local_cb0;
  uint8_t local_ca8[32];
  FTU64 local_c88;
  DWORD local_c80;
  FTU64 local_c78;
  HANDLE local_c70;
  LPCWSTR local_c68;
  FTU64 local_c60;
  LPCWSTR local_c58;
  FTU64 local_c50;
  LPCWSTR local_c48;
  FTU64 local_c40, local_c38, local_c30;
  DWORD local_c28;
  FTU64 local_c20;
  uint local_c18[2];
  FTU64 local_c10;
  WCHAR *local_c08;
  HANDLE *local_c00;
  FTU64 local_bf8;
  HANDLE local_bf0;
  LPWSTR local_be8;
  FTU64 local_be0, local_bd8, local_bd0;
  HANDLE local_bc8;
  FTU64 local_bc0;
  int64_t local_bb8;
  WCHAR *local_bb0;
  FTU64 local_ba8;
  ulonglong local_ba0;
  longlong local_b98;
  FTU64 local_b90, local_b88, local_b80;
  LPCWSTR local_b78;
  FTU64 local_b70;
  LPCWSTR local_b68;
  longlong local_b60;
  FTU64 local_b58, local_b50, local_b48, local_b40;
  POINT local_b38;
  FTU64 local_b30;
  LPCWSTR local_b28;
  FTU64 local_b20, local_b18;
  longlong local_b10;
  FTU64 local_b08;
  uint64_t local_b00;
  PROCESS_INFORMATION local_af8;
  uint64_t local_ae0, local_ad8, local_ad0;
  HANDLE local_ac8;
  LPWSTR local_ac0;
  FTU64 local_ab8, local_ab0;
  ulonglong local_aa8;
  FTU64 local_aa0;
  ulonglong local_a98;
  FTU64 local_a90;
  LPCWSTR local_a88;
  longlong local_a80, local_a78;
  short *local_a68;
  uint64_t local_a60, local_a58;
  FTU64 local_a50, local_a48, local_a40, local_a38, local_a30;
  longlong local_a28;
  FTU64 local_a20, local_a18;
  LARGE_INTEGER local_a10;
  WCHAR *local_a08, *local_a00;
  LPCSTR local_9f8;
  HANDLE local_9f0;
  DWORD local_9e8[2];
  FTU64 local_9e0;
  DWORD local_9d8[2];
  LPCWSTR local_9d0;
  uint32_t local_9c8[2];
  longlong local_9c0;
  uint8_t *local_9b8;
  LPCWSTR local_9b0;
  longlong local_9a8;
  int local_9a0;
  LPCSTR local_998;
  uint64_t *local_990;
  uint local_988;
  FTU64 local_980, local_978;
  HANDLE local_970[2];
  FTU64 local_960, local_958;
  longlong local_950, local_948;
  FTU64 local_940, local_938;
  LARGE_INTEGER local_930;
  LPCWSTR local_928;
  HANDLE local_920;
  FTU64 local_918;
  SECURITY_ATTRIBUTES local_910;
  LPCWSTR local_8f8;
  OVERLAPPED local_8f0;
  PROCESS_INFORMATION local_8d0;
  LPCWSTR local_8b8;
  FTU64 local_8b0;
  SECURITY_ATTRIBUTES local_8a8;
  STARTUPINFOW local_888;
  ulonglong local_818[14];
  SHELLEXECUTEINFOW local_7a8;
  CHAR local_738[32];
  MSG local_718, local_6e8;
  longlong local_6b8;
  int local_6b0;
  longlong local_648[16];
  STARTUPINFOW local_5c8;
  WCHAR local_558[32];
  RTL_CRITICAL_SECTION local_518[2];
  uint16_t local_4a8[206];   /* 原 undefined2 标量; 实为 ~400B 文件读缓冲 (ReadFile 400/memset 0x191) */
  WCHAR local_308[104];
  WCHAR local_238[268];

  _Var39.ft.dwLowDateTime = 0;
  _Var39.ft.dwHighDateTime = 0;
  local_d08 = 0x61;
  local_ce8[0] = 99;
  local_c28 = 0;
  local_c18[0] = 0;
  cVar15 = '\0';
  local_dc0 = '\0';
  local_d9f = '\0';
  local_d9e = '\0';
  local_ac0 = (LPWSTR)0x0;
  local_a10.QuadPart = 0;
  local_db0 = 0xd;
  local_res10.v = param_2;
  local_res18 = param_3;
  local_res20.v = param_4;
  if (((param_4 != 0) &&
      (PECMD_SkipLeadingControlChars((long long *)&local_res20.v), local_res20.v != 0)) &&
     (*(short *)(uintptr_t)local_res20.v == 0x2f)) {
    uVar34 = *(ushort *)((uintptr_t)local_res20.v + 2) | 0x20;
    if ((((((((uVar34 == 0x69) && ((*(ushort *)((uintptr_t)local_res20.v + 4) | 0x20) == 0x6e)) &&
            (((*(ushort *)((uintptr_t)local_res20.v + 6) | 0x20) == 0x73 &&
             (((*(ushort *)((uintptr_t)local_res20.v + 8) | 0x20) == 0x74 &&
              ((ushort)local_d08 == (*(ushort *)((uintptr_t)local_res20.v + 10) | 0x20))))))) &&
           ((*(ushort *)((uintptr_t)local_res20.v + 0xc) | 0x20) == 0x6c)) &&
          ((((*(ushort *)((uintptr_t)local_res20.v + 0xe) | 0x20) == 0x6c &&
            ((*(ushort *)((uintptr_t)local_res20.v + 0x10) | 0x20) == 0x73)) &&
           ((*(ushort *)((uintptr_t)local_res20.v + 0x12) | 0x20) == 0x65)))) &&
         (((*(ushort *)((uintptr_t)local_res20.v + 0x14) | 0x20) == 0x72 &&
          ((*(ushort *)((uintptr_t)local_res20.v + 0x16) | 0x20) == 0x76)))) &&
        ((((*(ushort *)((uintptr_t)local_res20.v + 0x18) | 0x20) == 0x69 &&
          (((ushort)local_ce8[0] == (*(ushort *)((uintptr_t)local_res20.v + 0x1a) | 0x20) &&
           ((*(ushort *)((uintptr_t)local_res20.v + 0x1c) | 0x20) == 0x65)))) &&
         (((8 < *(ushort *)((uintptr_t)local_res20.v + 0x1e) &&
           (*(ushort *)((uintptr_t)local_res20.v + 0x1e) < 0xe)) ||
          (*(short *)((uintptr_t)local_res20.v + 0x1e) == 0x20)))))) ||
       ((((((((uVar34 == 0x73 && ((*(ushort *)((uintptr_t)local_res20.v + 4) | 0x20) == 0x65)) &&
             ((*(ushort *)((uintptr_t)local_res20.v + 6) | 0x20) == 0x72)) &&
            (((*(ushort *)((uintptr_t)local_res20.v + 8) | 0x20) == 0x76 &&
             ((*(ushort *)((uintptr_t)local_res20.v + 10) | 0x20) == 0x69)))) &&
           ((ushort)local_ce8[0] == (*(ushort *)((uintptr_t)local_res20.v + 0xc) | 0x20))) &&
          ((*(ushort *)((uintptr_t)local_res20.v + 0xe) | 0x20) == 0x65)) &&
         (((8 < *(ushort *)((uintptr_t)local_res20.v + 0x10) &&
           (*(ushort *)((uintptr_t)local_res20.v + 0x10) < 0xe)) ||
          (*(short *)((uintptr_t)local_res20.v + 0x10) == 0x20)))) ||
        (((((((uVar34 == 0x72 && ((*(ushort *)((uintptr_t)local_res20.v + 4) | 0x20) == 0x65)) &&
             ((*(ushort *)((uintptr_t)local_res20.v + 6) | 0x20) == 0x6d)) &&
            (((*(ushort *)((uintptr_t)local_res20.v + 8) | 0x20) == 0x6f &&
             ((*(ushort *)((uintptr_t)local_res20.v + 10) | 0x20) == 0x76)))) &&
           ((((*(ushort *)((uintptr_t)local_res20.v + 0xc) | 0x20) == 0x65 &&
             (((*(ushort *)((uintptr_t)local_res20.v + 0xe) | 0x20) == 0x73 &&
              ((*(ushort *)((uintptr_t)local_res20.v + 0x10) | 0x20) == 0x65)))) &&
            ((*(ushort *)((uintptr_t)local_res20.v + 0x12) | 0x20) == 0x72)))) &&
          (((((*(ushort *)((uintptr_t)local_res20.v + 0x14) | 0x20) == 0x76 &&
             ((*(ushort *)((uintptr_t)local_res20.v + 0x16) | 0x20) == 0x69)) &&
            ((*(ushort *)((uintptr_t)local_res20.v + 0x18) | 0x20) == 99)) &&
           ((*(ushort *)((uintptr_t)local_res20.v + 0x1a) | 0x20) == 0x65)))) &&
         (((8 < *(ushort *)((uintptr_t)local_res20.v + 0x1c) &&
           (*(ushort *)((uintptr_t)local_res20.v + 0x1c) < 0xe)) ||
          (*(short *)((uintptr_t)local_res20.v + 0x1c) == 0x20)))))))) {
      PECMD_StrBldCopyWide((long long *)&g_pNextCmd,(const uint16_t *)L"PECMD EXEC ");
      PECMD_AppendWideStr(&g_pNextCmd,(LPCWSTR)(uintptr_t)local_res20.v);
      *(uint16_t *)((char *)param_1 + 0xc8) = 2;
      goto LAB_14001668b;
    }
  }
  local_c80 = 0;
  bVar7 = false;
  bVar2 = false;
  local_e00 = param_5 & 1;
  uVar35 = param_5 >> 1;
  param_5 = ((((param_5 >> 10) & 0xffffffu) << 8) | (uint8_t)((param_5 >> 2) & 0xffu)) & 0xffffff01u;
  local_d48 = uVar35 & 1;
  PECMD_AllocStrSlot((uint16_t **)&local_cc8.v);
  if (local_res10.v != 0) {
    PECMD_StrBldCopyWide((long long *)&local_cc8.v,(LPCWSTR)(uintptr_t)local_res10.v);
    local_res10.v = local_cc8.v;
    PECMD_SkipLeadingControlChars((long long *)&local_res10.v);
  }
  PECMD_AllocStrSlot((uint16_t **)&local_cb8.v);
  if (local_res20.v != 0) {
    PECMD_StrBldCopyWide((long long *)&local_cb8.v,(LPCWSTR)(uintptr_t)local_res20.v);
    local_res20.v = local_cb8.v;
  }
  local_990 = (uint64_t *)&local_bc0.v;
  if (param_6 != (uint64_t *)0) {
    local_990 = param_6;
  }
  local_9c8[0] = 0x5e;
  local_b18.v = local_res10.v;
  if (((local_res10.v == 0) && (local_res10.v = local_res20.v, local_res20.v != 0)
      ) && (_Var59.v = local_res20.v, uVar23 = PECMD_AsciiPrefixICmp("-shel",(ushort *)(uintptr_t)local_res20.v,5),
           (char)uVar23 != '\0')) {
    local_res20.v = (uint64_t)(uintptr_t)local_9c8;
    local_e00 = 1;
    local_d48 = 1;
    param_5 = (uint)((_Var59.v & ~0xffULL) | 1);
  }
  local_ddc = local_ddc & 0xffff0000;
  local_aa8 = 0;
  local_de4 = 0;
  local_d43 = '\0';
  local_d50 = local_d50 & 0xffffff00;
  local_d3b = '\0';
  local_d78 = (uint16_t)(local_d78 & 0xff00);
  bVar5 = false;
  bVar4 = false;
  bVar6 = false;
  bVar16 = false;
  local_d44 = '\0';
  PECMD_AllocStrSlot((uint16_t **)&local_ce0);
  local_b38.x = 0;
  local_b38.y = 0;
  local_d3c = '\0';
  local_d98[0] = -(uint)(DAT_14013c968 != '\0') & 0x10000;
  local_d40 = local_d40 & 0xffffff00;
  local_d9a = '\0';
  local_d9c = '\0';
  local_d9b = '\x01';
  local_d4c = '\0';
  local_de8 = 0;
  local_dbc = 0;
  local_dfc = 0;
  local_d60 = 0;
  PECMD_AllocStrSlot((uint16_t **)&local_cc0);
  local_d00 = 0;
  local_e08 = 0;
  PECMD_AllocStrSlot((uint16_t **)&local_d58);
  PECMD_AllocStrSlot((uint16_t **)&local_d38);
  PECMD_AllocStrSlot((uint16_t **)&local_d20);
  local_d04 = 0xffffffff;
  local_e06 = '\0';
  local_d9d = '\0';
  local_980.ft.dwLowDateTime = 0;
  local_980.ft.dwHighDateTime = 0;
  local_ce8[0] = 0;
  local_de0 = 0;
  PECMD_AllocStrSlot((uint16_t **)&local_cb0);
  local_c48 = (LPCWSTR)0x0;
  local_bd8.ft.dwLowDateTime = 0;
  local_bd8.ft.dwHighDateTime = 0;
  local_888.cb = 0x68;
  local_888.lpReserved = (LPWSTR)0x0;
  FUN_140102a90((ulonglong *)&local_888.lpDesktop,0,0x60);
  FUN_140102a90(local_818,0,0x70);
  local_af8.hProcess = (HANDLE)0x0;
  local_af8.hThread = (HANDLE)0x0;
  local_af8.dwProcessId = 0;
  local_af8.dwThreadId = 0;
  local_ae0 = 0;
  local_ad8 = 0;
  local_ad0 = 0;
  PECMD_ResetSlots((uint64_t *)&local_a80);
  local_c40.ft.dwLowDateTime = 0;
  local_c40.ft.dwHighDateTime = 0;
  local_db4 = 0;
  if (local_res20.v == 0) {
    local_de0 = 1;
  }
  else {
    if (*(WCHAR *)(uintptr_t)local_res20.v == L'#') {
      if (local_res20.v == local_res10.v) {
        local_res10.v = local_res10.v + 2;
      }
      local_res20.v = local_res20.v + 2;
    }
    else {
      local_de0 = 0;
      if ((DAT_140147000 & 3) != 0) {
        local_de0 = 3;
      }
    }
    if (local_res20.v != 0) {
      if (*(WCHAR *)(uintptr_t)local_res20.v == L'^') {
        if (local_res20.v == local_res10.v) {
          local_res10.v = local_res10.v + 2;
        }
        local_res20.v = local_res20.v + 2;
        param_5 = 1;
      }
      if ((local_res20.v != 0) && (*(WCHAR *)(uintptr_t)local_res20.v == L'$')) {
        if (local_res20.v == local_res10.v) {
          local_res10.v = local_res10.v + 2;
        }
        local_res20.v = local_res20.v + 2;
        bVar4 = true;
      }
    }
  }
  _Var54.ft.dwLowDateTime = 1;
  _Var54.ft.dwHighDateTime = 0;
  _Var53.ft.dwLowDateTime = 0;
  _Var53.ft.dwHighDateTime = 0;
  local_dd8[0] = 0;
  _Var59.v = _Var39.v;
  if ((local_res20.v != 0) && (_Var59.v = _Var53.v, *(WCHAR *)(uintptr_t)local_res20.v == L'|')) {
    if (local_res20.v == local_res10.v) {
      local_res10.v = local_res10.v + 2;
    }
    local_res20.v = local_res20.v + 2;
    local_dd8[0] = 1;
    _Var59.v = _Var54.v;
  }
  local_cfc = 0x100;
  local_da8 = 4;
  if ((param_4 != 0) && (*(short *)(uintptr_t)param_4 == 0x2a)) {
    local_res10.v = local_res10.v + 2;
    uVar35 = 0x10;
    WVar62 = *(WCHAR *)(uintptr_t)local_res10.v;
    local_ddc = 0x10;
    if (WVar62 != L'\0') {
      do {
        _Var60.ft.dwLowDateTime = 1;
        _Var60.ft.dwHighDateTime = 0;
        if (WVar62 == L'|') {
          local_res10.v = local_res10.v + 2;
          _Var59.v = _Var60.v;
        }
        else if (((ushort)WVar62 < 0x30) || (0x39 < (ushort)WVar62)) {
          if ((WCHAR)(WVar62 | 0x20U) == 0x6e) {
            local_res10.v = local_res10.v + 2;
            uVar35 = uVar35 | 2;
          }
          else if (WVar62 == L'-') {
            local_res10.v = local_res10.v + 2;
            uVar35 = (uint)(ushort)((ushort)uVar35 | (ushort)local_da8);
          }
          else {
            if (WVar62 != L'#') break;
            local_res10.v = local_res10.v + 2;
            local_dc0 = '\x01';
          }
        }
        else {
          uVar34 = (ushort)uVar35 | 1;
          PECMD_ParseIntegerString((long long *)&local_res10.v,&local_aa8);
          if ((longlong)local_aa8 < 1) {
            local_aa8 = 1;
          }
          if (*(WCHAR *)(uintptr_t)local_res10.v == L'$') {
            uVar17 = 0x100;
LAB_14000eb4f:
            uVar34 = uVar34 | uVar17;
            local_res10.v = local_res10.v + 2;
          }
          else if (*(WCHAR *)(uintptr_t)local_res10.v == L'^') {
            uVar17 = 0x200;
            goto LAB_14000eb4f;
          }
          uVar35 = (uint)uVar34;
        }
        WVar62 = *(WCHAR *)(uintptr_t)local_res10.v;
      } while (WVar62 != L'\0');
      local_ddc = uVar35;
      local_dd8[0] = _Var59.ft.dwLowDateTime;
      WVar62 = *(WCHAR *)(uintptr_t)local_res10.v;
      while ((WVar62 != L'\0' &&
             ((((ushort)WVar62 < 9 || (0xd < (ushort)WVar62)) && (WVar62 != L' '))))) {
        local_res10.v = local_res10.v + 2;
        WVar62 = *(WCHAR *)(uintptr_t)local_res10.v;
      }
    }
  }
  uVar35 = local_ddc;
  PECMD_SkipLeadingControlChars((long long *)&local_res10.v);
  if ((short)uVar35 == 0) goto LAB_14000ee1e;
LAB_14000ec8e:
  local_e00 = 1;
  WVar62 = *(WCHAR *)(uintptr_t)local_res10.v;
  _Var53.v = local_res10.v;
  if (WVar62 != L'\0') {
    do {
      if (WVar62 == L'=') break;
      _Var53.v = _Var53.v + 2;
      WVar62 = *(WCHAR *)(uintptr_t)_Var53.v;
    } while (WVar62 != L'\0');
    if ((_Var53.v != local_res10.v) && (*(WCHAR *)(uintptr_t)_Var53.v == L'=')) {
      _Var54.v = _Var53.v - 2;
      if (*(WCHAR *)(uintptr_t)_Var54.v == L'+') {
        local_d3c = *(char *)(uintptr_t)_Var54.v;
        _Var54.v = _Var53.v - 4;
      }
      _Var53.v = _Var54.v;
      if ((ulonglong)local_res10.v < (ulonglong)_Var54.v) {
        do {
          if ((((ushort)*(WCHAR *)(uintptr_t)_Var53.v < 9) || (0xd < (ushort)*(WCHAR *)(uintptr_t)_Var53.v)) &&
             (*(WCHAR *)(uintptr_t)_Var53.v != L' ')) break;
          _Var53.v = _Var53.v - 2;
        } while ((ulonglong)local_res10.v < (ulonglong)_Var53.v);
        _Var59.ft.dwHighDateTime = 0;
        _Var59.ft.dwLowDateTime = local_dd8[0];
      }
      local_b20.v = _Var54.v;
      if ((((ushort)*(WCHAR *)(uintptr_t)_Var53.v < 9) || (0xd < (ushort)*(WCHAR *)(uintptr_t)_Var53.v)) &&
         (*(WCHAR *)(uintptr_t)_Var53.v != L' ')) {
        _Var60.v = _Var53.v;
        if ((ulonglong)local_res10.v < (ulonglong)_Var53.v) {
          while ((_Var41.v = _Var60.v - 2, (ushort)*(WCHAR *)(uintptr_t)_Var41.v < 9 ||
                 (0xd < (ushort)*(WCHAR *)(uintptr_t)_Var41.v))) {
            if ((*(WCHAR *)(uintptr_t)_Var41.v == L' ') ||
               (_Var60.v = _Var41.v, (ulonglong)_Var41.v <= (ulonglong)local_res10.v)) break;
          }
        }
        PECMD_StrCopyW((WCHAR **)&local_d58,(LPCWSTR)(uintptr_t)_Var60.v,
                      (longlong)((uintptr_t)_Var53.v + 2) - (longlong)_Var60.v >> 1);
        PECMD_RunCommandLine((void *)param_1,(WCHAR **)&local_d58,0);
        if ((*(char *)((uintptr_t)param_1 + 0xda) != '\0') ||
           ((*(byte *)((uintptr_t)param_1 + 0xd) & 0xf) != 0)) {
          PECMD_RunCommandLine((void *)param_1,(WCHAR **)&local_d58,0);
        }
        WVar62 = *(WCHAR *)(uintptr_t)_Var54.v;
        while (WVar62 != L'\0') {
          if (WVar62 == L'=') goto LAB_14000ee05;
          _Var54.v = _Var54.v + 2;
          WVar62 = *(WCHAR *)(uintptr_t)_Var54.v;
        }
        if (*(WCHAR *)(uintptr_t)_Var54.v == L'=') {
LAB_14000ee05:
          local_980.v = _Var54.v + 2;
          PECMD_SkipLeadingControlChars((long long *)&local_980.v);
LAB_14000ee1e:
          DVar21 = _Var59.ft.dwLowDateTime;
          PECMD_SkipLeadingControlChars((long long *)&local_res10.v);
          local_d90.v = local_res10.v;
          if ((local_c48 == (LPCWSTR)0x0) || ((short)local_ddc == 0)) {
            do {
              _Var53.v = local_d90.v;
              DVar21 = _Var59.ft.dwLowDateTime;
              if ((*(WCHAR *)(uintptr_t)local_d90.v == L'-') && (*(WCHAR *)((uintptr_t)local_d90.v + 2) == L'-')) {
                local_d90.v = local_d90.v + 2;
              }
              cVar13 = PECMD_MatchTokenAdvance("-",(void *)&local_d90.v,1);
              if ((cVar13 != '\0') ||
                 ((*(WCHAR *)(uintptr_t)local_d90.v != L'-' && (*(WCHAR *)(uintptr_t)local_d90.v != L'/')))) break;
              local_d90.v = local_d90.v + 2;
              cVar13 = PECMD_MatchAndPad((int64_t)(uintptr_t)DAT_14011dd24,(longlong *)&local_d90.v,5);
              if (cVar13 == '\0') {
                uVar23 = PECMD_AsciiPrefixICmp("cmd:",(ushort *)(uintptr_t)local_d90.v,(uint)local_da8);
                if ((char)uVar23 != '\0') {
                  local_d90.v = local_d90.v + 8;
                  local_c48 = (LPCWSTR)PECMD_ParseVarArg(param_1,(longlong *)&local_d90.v,&local_cb0,
                                                     (int *)&local_d04,local_ce8);
                  if ((ulonglong)_Var53.v < (ulonglong)local_d90.v) {
                    do {
                      *(WCHAR *)(uintptr_t)_Var53.v = L' ';
                      _Var53.v = _Var53.v + 2;
                    } while ((ulonglong)_Var53.v < (ulonglong)local_d90.v);
                    _Var59.ft.dwHighDateTime = 0;
                    _Var59.ft.dwLowDateTime = local_dd8[0];
                  }
                  DVar21 = _Var59.ft.dwLowDateTime;
                  if ((short)local_ddc == 0) goto LAB_14000ef02;
                  break;
                }
                uVar23 = PECMD_AsciiPrefixICmp("wd:",(ushort *)(uintptr_t)local_d90.v,3);
                if ((char)uVar23 == '\0') {
                  uVar23 = PECMD_AsciiPrefixICmp("exe:",(ushort *)(uintptr_t)local_d90.v,(uint)local_da8);
                  if ((char)uVar23 == '\0') {
                    uVar23 = PECMD_AsciiPrefixICmp("logs:",(ushort *)(uintptr_t)local_d90.v,5);
                    if ((char)uVar23 == '\0') goto LAB_14000ef02;
                    local_d90.v = local_d90.v + 10;
                  }
                  else {
                    for (local_d90.v = local_d90.v + 8;
                        *(WCHAR *)(uintptr_t)local_d90.v == L'*';
                        local_d90.v = local_d90.v + 2) {
                    }
                  }
                }
                else {
                  for (local_d90.v = local_d90.v + 6; *(WCHAR *)(uintptr_t)local_d90.v == L'*'
                      ; local_d90.v = local_d90.v + 2) {
                  }
                }
                if (*(WCHAR *)(uintptr_t)local_d90.v != L'\"') goto LAB_14000ef02;
                local_d90.v = local_d90.v + 2;
                WVar62 = *(WCHAR *)(uintptr_t)local_d90.v;
                if (WVar62 != L'\0') {
                  do {
                    local_d90.v = local_d90.v + 2;
                    if (WVar62 == L'\"') break;
                    WVar62 = *(WCHAR *)(uintptr_t)local_d90.v;
                  } while (WVar62 != L'\0');
                  goto LAB_14000ef02;
                }
              }
              else {
                cVar13 = (char)local_ddc;
                local_ddc = local_ddc | 0x10;
                if (cVar13 == '\0') goto LAB_14000ec8e;
LAB_14000ef02:
                WVar62 = *(WCHAR *)(uintptr_t)local_d90.v;
                if (WVar62 != L'\0') {
                  do {
                    if (((8 < (ushort)WVar62) && ((ushort)WVar62 < 0xe)) || (WVar62 == L' ')) break;
                    local_d90.v = local_d90.v + 2;
                    WVar62 = *(WCHAR *)(uintptr_t)local_d90.v;
                  } while (WVar62 != L'\0');
                  _Var59.ft.dwHighDateTime = 0;
                  _Var59.ft.dwLowDateTime = local_dd8[0];
                }
              }
              PECMD_SkipLeadingControlChars((long long *)&local_d90.v);
            } while( true );
          }
          PECMD_SkipLeadingControlChars((long long *)&local_res10.v);
          if (local_c48 != (LPCWSTR)0x0) {
            local_c48 = (LPCWSTR)(-(ulonglong)(*local_c48 != L'\0') & (ulonglong)local_c48);
          }
          if ((DVar21 == 0) && (param_7 == 0)) {
            local_da0 = '\0';
            cVar13 = cVar15;
          }
          else {
            cVar13 = '\x01';
            local_da0 = '\x01';
          }
          local_a98 = 0xffffffffffffffff;
          local_920 = (HANDLE)0x0;
          local_c70 = (HANDLE)0x0;
          local_cf8.ft.dwLowDateTime = 0;
          local_cf8.ft.dwHighDateTime = 0;
          local_d68.ft.dwLowDateTime = 0;
          local_d68.ft.dwHighDateTime = 0;
          local_dd0.ft.dwLowDateTime = 0;
          local_dd0.ft.dwHighDateTime = 0;
          local_c58 = (LPCWSTR)0x0;
          local_c68 = (LPCWSTR)0x0;
          PECMD_AllocStrSlot((uint16_t **)(local_ca8 + 0x10));
          PECMD_AllocStrSlot((uint16_t **)&local_c78.v);
          uVar17 = 0;
          local_d10.ft.dwLowDateTime = 0;
          local_d10.ft.dwHighDateTime = 0;
          uVar34 = 0;
          *(uint32_t *)&local_ca8[0] = 0;
          *(uint32_t *)&local_ca8[4] = 0;
          bVar3 = 0;
          uVar35 = local_de0;
          if ((cVar13 == '\0') && (local_b18.v == 0)) {
            PECMD_ExpandVarDispatch((void *)param_1,(WCHAR *)(uintptr_t)local_res10.v,(void *)local_ca8,0,1);
            local_res10.ft.dwLowDateTime = *(uint32_t *)&local_ca8[0];
            local_res10.ft.dwHighDateTime = *(uint32_t *)&local_ca8[4];
            uVar35 = local_de0;
          }
          goto LAB_14000f242;
        }
      }
    }
  }
  PECMD_ReleaseObjectResources(&local_a80);
  PECMD_FreeStrBuf(&local_cb0);
  PECMD_FreeStrBuf((WCHAR **)&local_d20);
  PECMD_FreeStrBuf((WCHAR **)&local_d38);
  PECMD_FreeStrBuf((WCHAR **)&local_d58);
  PECMD_FreeStrBuf((WCHAR **)&local_cc0);
  PECMD_FreeStrBuf((WCHAR **)&local_ce0);
  PECMD_FreeStrBuf((WCHAR **)&local_cb8.v);
  PECMD_FreeStrBuf((WCHAR **)&local_cc8.v);
  _Var39.ft.dwLowDateTime = 1;
  _Var39.ft.dwHighDateTime = 0;
  goto LAB_14001668b;
LAB_14000f242:
  _Var59.v = local_res10.v;
  if ((*(WCHAR *)(uintptr_t)local_res10.v == L'-') && (*(WCHAR *)((uintptr_t)local_res10.v + 2) == L'-')) {
    local_res10.v = local_res10.v + 2;
  }
  p_Var58 = (STARTUPINFOW *)0x0;
  cVar13 = PECMD_MatchTokenAdvance("-",(void *)&local_res10.v,1);
  iVar52 = 0;
  if ((cVar13 != '\0') || ((*(WCHAR *)(uintptr_t)local_res10.v != L'-' && (*(WCHAR *)(uintptr_t)local_res10.v != L'/'))))
  goto LAB_140010994;
  local_res10.v = local_res10.v + 2;
  cVar13 = PECMD_MatchTokenAdvance("catch",(void *)&local_res10.v,5);
  if (cVar13 == '\0') {
    uVar23 = PECMD_AsciiPrefixICmp("clone:",(ushort *)(uintptr_t)local_res10.v,6);
    if ((char)uVar23 == '\0') goto LAB_14000f2e0;
    _Var59.v = local_res10.v + 0xc;
    WVar62 = *(WCHAR *)(uintptr_t)_Var59.v;
    local_res10.v = _Var59.v;
    while (WVar62 != L'\0') {
      if (((8 < (ushort)WVar62) && ((ushort)WVar62 < 0xe)) || (WVar62 == L' ')) {
        if (*(WCHAR *)(uintptr_t)local_res10.v != L'\0') {
          *(WCHAR *)(uintptr_t)local_res10.v = L'\0';
          local_res10.v = local_res10.v + 2;
        }
        break;
      }
      local_res10.v = local_res10.v + 2;
      WVar62 = *(WCHAR *)(uintptr_t)local_res10.v;
    }
    _Var53.v = local_res10.v;
    if (*(WCHAR *)(uintptr_t)_Var59.v != L'\0') {
      local_bd8.v = _Var59.v;
    }
    goto LAB_140010982;
  }
  local_ddc = (local_ddc & 0xffff0000) | 0x10;
LAB_14000f2e0:
  cVar13 = PECMD_MatchTokenAdvance("sh",(void *)&local_res10.v,2);
  if (cVar13 == '\0') {
    cVar13 = PECMD_MatchTokenAdvance("shel",(void *)&local_res10.v,(uint)local_da8);
    if (cVar13 != '\0') {
      param_5 = param_5 | 2;
      local_de0 = uVar35 & 0xfffffffe;
      local_d48 = 1;
      local_e00 = 1;
      uVar35 = local_de0;
      goto LAB_14000f242;
    }
    uVar23 = PECMD_AsciiPrefixICmp("shel:",(ushort *)(uintptr_t)local_res10.v,5);
    if ((char)uVar23 == '\0') {
      uVar23 = PECMD_AsciiPrefixICmp("runrmenu:",(ushort *)(uintptr_t)local_res10.v,9);
      if ((char)uVar23 == '\0') {
        uVar23 = PECMD_AsciiPrefixICmp("poprmenu",(ushort *)(uintptr_t)local_res10.v,8);
        if ((char)uVar23 == '\0') {
          uVar23 = PECMD_AsciiPrefixICmp("pid:",(ushort *)(uintptr_t)local_res10.v,(uint)local_da8);
          if ((char)uVar23 == '\0') {
            uVar23 = PECMD_AsciiPrefixICmp("hpid:",(ushort *)(uintptr_t)local_res10.v,5);
            if ((char)uVar23 == '\0') {
              uVar23 = PECMD_AsciiPrefixICmp("ppid:",(ushort *)(uintptr_t)local_res10.v,5);
              if ((char)uVar23 != '\0') {
                local_res10.v = local_res10.v + 10;
                local_918.ft.dwLowDateTime = 0;
                local_918.ft.dwHighDateTime = 0;
                PECMD_ParseNumSkipWs((WCHAR **)&local_res10.v,(uint64_t *)&local_918.v);
                local_c40.v = local_918.v;
                if ((longlong)local_918.v < 0) {
                  local_c40.v = (uint64_t)PECMD_FindAliveProcessId((ulonglong)local_918.v);
                }
                goto LAB_14000f242;
              }
              uVar23 = PECMD_AsciiPrefixICmp("exe:",(ushort *)(uintptr_t)local_res10.v,(uint)local_da8);
              if ((char)uVar23 == '\0') {
                uVar23 = PECMD_AsciiPrefixICmp("wd:",(ushort *)(uintptr_t)local_res10.v,3);
                if ((char)uVar23 == '\0') {
                  uVar23 = PECMD_AsciiPrefixICmp("logs:",(ushort *)(uintptr_t)local_res10.v,5);
                  if ((char)uVar23 != '\0') {
                    _Var53.v = local_res10.v + 10;
                    WVar62 = *(WCHAR *)(uintptr_t)_Var53.v;
                    _Var59.v = _Var53.v;
                    if (WVar62 == L'?') {
                      _Var59.v = local_res10.v + 0xc;
                    }
                    local_ac8 = (HANDLE)0x0;
                    local_res10.v = _Var53.v;
                    local_aa0.v = _Var59.v;
                    local_res10.v = (uint64_t)PECMD_NextToken((longlong *)&local_aa0.v,(longlong *)&local_ac8,1);
                    _Var53.v = local_res10.v;
                    if (((longlong)local_ac8 < 1) || ((WVar62 == L'?' && (DAT_14013cb18 == 0))))
                    goto LAB_140010982;
                    ppWVar37 = &local_cc0;
                    pvVar32 = local_ac8;
                    goto LAB_14001070a;
                  }
                  cVar13 = PECMD_MatchTokenAdvance("nc",(void *)&local_res10.v,2);
                  if (cVar13 != '\0') {
                    local_d44 = '\x01';
                    goto LAB_14000f242;
                  }
                  cVar13 = PECMD_MatchTokenAdvance("nowin",(void *)&local_res10.v,5);
                  if (cVar13 != '\0') {
                    bVar16 = true;
                    goto LAB_14000f242;
                  }
                  cVar13 = PECMD_MatchTokenAdvance("hide",(void *)&local_res10.v,(uint)local_da8);
                  if (cVar13 != '\0') {
                    local_dfc = 0x10000000;
                    goto LAB_14000f242;
                  }
                  cVar13 = PECMD_MatchTokenAdvance("wait",(void *)&local_res10.v,(uint)local_da8);
                  if (cVar13 != '\0') {
                    local_c80 = local_c80 + 1;
                    local_e00 = 1;
                    goto LAB_14000f242;
                  }
                  cVar13 = PECMD_MatchTokenAdvance("waiti",(void *)&local_res10.v,5);
                  if (cVar13 != '\0') {
                    bVar2 = true;
                    goto LAB_14000f242;
                  }
                  cVar13 = PECMD_MatchTokenAdvance("err",(void *)&local_res10.v,3);
                  if (cVar13 != '\0') {
                    local_de4 = local_de4 | 1;
                    goto LAB_14000f242;
                  }
                  cVar13 = PECMD_MatchTokenAdvance("u2d",(void *)&local_res10.v,3);
                  if (cVar13 != '\0') {
                    local_d9f = '\x01';
                    goto LAB_14000f242;
                  }
                  cVar13 = PECMD_MatchTokenAdvance("d2u",(void *)&local_res10.v,3);
                  if (cVar13 != '\0') {
                    local_d9e = '\x01';
                    goto LAB_14000f242;
                  }
                  cVar13 = PECMD_MatchTokenAdvance("err+",(void *)&local_res10.v,(uint)local_da8);
                  if (cVar13 != '\0') {
                    local_de4 = local_de4 | 3;
                    goto LAB_14000f242;
                  }
                  cVar13 = PECMD_MatchTokenAdvance("limit",(void *)&local_res10.v,5);
                  if (cVar13 != '\0') {
                    local_d78 = (uint16_t)((local_d78 & 0xff00) | 1);
                    goto LAB_14000f242;
                  }
                  cVar13 = PECMD_MatchTokenAdvance("job",(void *)&local_res10.v,3);
                  if (cVar13 != '\0') {
                    bVar5 = true;
                    goto LAB_14000f242;
                  }
                  cVar13 = PECMD_MatchTokenAdvance("hook",(void *)&local_res10.v,(uint)local_da8);
                  if (cVar13 != '\0') {
                    local_d48 = 1;
                    goto LAB_14000f242;
                  }
                  cVar13 = PECMD_MatchTokenAdvance("min",(void *)&local_res10.v,3);
                  if (cVar13 != '\0') {
                    local_dfc = 6;
                    goto LAB_14000f242;
                  }
                  cVar13 = PECMD_MatchTokenAdvance("max",(void *)&local_res10.v,3);
                  if (cVar13 != '\0') {
                    local_dfc = 3;
                    goto LAB_14000f242;
                  }
                  cVar13 = PECMD_MatchTokenAdvance("show",(void *)&local_res10.v,(uint)local_da8);
                  if (cVar13 != '\0') {
                    local_dfc = 1;
                    goto LAB_14000f242;
                  }
                  cVar13 = PECMD_MatchTokenAdvance("runs",(void *)&local_res10.v,(uint)local_da8);
                  if (cVar13 != '\0') {
                    local_d3b = '\x01';
                    goto LAB_14000f242;
                  }
                  cVar13 = PECMD_MatchTokenAdvance("64",(void *)&local_res10.v,2);
                  if (cVar13 != '\0') goto LAB_14000f242;
                  cVar13 = PECMD_MatchTokenAdvance("no64",(void *)&local_res10.v,(uint)local_da8);
                  if (cVar13 != '\0') goto LAB_14000f242;
                  cVar13 = PECMD_MatchTokenAdvance("ex1",(void *)&local_res10.v,3);
                  if (cVar13 != '\0') {
                    local_d43 = '\x01';
                    goto LAB_14000f242;
                  }
                  cVar13 = PECMD_MatchTokenAdvance("io",(void *)&local_res10.v,2);
                  if (cVar13 != '\0') {
                    local_d50 = (local_d50 & 0xffffff00) | 1;
                    goto LAB_14000f242;
                  }
                  cVar13 = PECMD_MatchTokenAdvance("incmd",(void *)&local_res10.v,5);
                  if (cVar13 != '\0') {
                    local_d9c = '\x01';
                    goto LAB_14000f242;
                  }
                  cVar13 = PECMD_MatchTokenAdvance("svr-",(void *)&local_res10.v,3);
                  if (cVar13 != '\0') {
                    local_d98[0] = 0;
                    goto LAB_14000f242;
                  }
                  cVar13 = PECMD_MatchTokenAdvance("svrsys",(void *)&local_res10.v,6);
                  if (cVar13 != '\0') {
                    local_d98[0] = 0x20000;
                    goto LAB_14000f242;
                  }
                  cVar13 = PECMD_MatchTokenAdvance("svrsys?",(void *)&local_res10.v,7);
                  if (cVar13 != '\0') {
                    local_d98[0] = -(uint)(DAT_14013c968 != '\0') & 0x20000;
                    goto LAB_14000f242;
                  }
                  cVar13 = PECMD_MatchTokenAdvance("svrusr",(void *)&local_res10.v,6);
                  if (cVar13 != '\0') {
                    local_d98[0] = local_d98[0] | 1;
                    goto LAB_14000f242;
                  }
                  cVar13 = PECMD_MatchTokenAdvance("svrusr?",(void *)&local_res10.v,7);
                  if (cVar13 != '\0') {
                    local_d98[0] = (DWORD)(DAT_14013c968 != '\0');
                    goto LAB_14000f242;
                  }
                  cVar13 = PECMD_MatchTokenAdvance("hidecmd",(void *)&local_res10.v,7);
                  if (cVar13 != '\0') {
                    local_d9b = '\x01';
                    goto LAB_14000f242;
                  }
                  cVar13 = PECMD_MatchTokenAdvance("hidecmd-",(void *)&local_res10.v,8);
                  if (cVar13 != '\0') {
                    local_d9b = '\0';
                    goto LAB_14000f242;
                  }
                  cVar13 = PECMD_MatchTokenAdvance("sys",(void *)&local_res10.v,3);
                  if (cVar13 != '\0') {
                    local_dbc = 1;
                    goto LAB_14000f242;
                  }
                  uVar23 = PECMD_AsciiPrefixICmp("uac",(ushort *)(uintptr_t)local_res10.v,3);
                  if ((char)uVar23 == '\0') {
                    uVar23 = PECMD_AsciiPrefixICmp("su",(ushort *)(uintptr_t)local_res10.v,2);
                    if ((char)uVar23 == '\0') {
                      uVar23 = PECMD_AsciiPrefixICmp("doc",(ushort *)(uintptr_t)local_res10.v,3);
                      if ((char)uVar23 != '\0') {
                        bVar4 = true;
                        _Var53.v = local_res10.v + 6;
                        if (*(WCHAR *)((uintptr_t)local_res10.v + 6) == L':') {
                          _Var59.v = local_res10.v + 8;
                          WVar62 = *(WCHAR *)(uintptr_t)_Var59.v;
                          local_res10.v = _Var59.v;
                          _Var53.v = _Var59.v;
                          if (WVar62 != L'\0') {
                            do {
                              if (((8 < (ushort)WVar62) && ((ushort)WVar62 < 0xe)) ||
                                 (WVar62 == L' ')) break;
                              local_res10.v = local_res10.v + 2;
                              WVar62 = *(WCHAR *)(uintptr_t)local_res10.v;
                            } while (WVar62 != L'\0');
                            _Var53.v = local_res10.v;
                            uVar17 = uVar34;
                            if (_Var59.v != local_res10.v) {
                              ppWVar37 = (LPCWSTR *)(local_ca8 + 0x10);
                              goto LAB_140010704;
                            }
                          }
                        }
                        goto LAB_140010982;
                      }
                      uVar23 = PECMD_AsciiPrefixICmp("code:",(ushort *)(uintptr_t)local_res10.v,5);
                      if ((char)uVar23 == '\0') {
                        uVar25 = PECMD_MatchAssignToken("timeout:",(longlong *)&local_res10.v,7);
                        if ((char)uVar25 == '\0') {
                          uVar23 = PECMD_AsciiPrefixICmp("user:",(ushort *)(uintptr_t)local_res10.v,5);
                          _Var54.v = local_res10.v;
                          if ((char)uVar23 != '\0') {
                            _Var59.v = local_res10.v + 10;
                            WVar62 = *(WCHAR *)(uintptr_t)_Var59.v;
                            local_res10.v = _Var59.v;
                            _Var53.v = _Var59.v;
                            local_b70.v = _Var59.v;
                            if (WVar62 != L'\0') {
                              do {
                                if (((8 < (ushort)WVar62) && ((ushort)WVar62 < 0xe)) ||
                                   (WVar62 == L' ')) break;
                                local_res10.v = local_res10.v + 2;
                                WVar62 = *(WCHAR *)(uintptr_t)local_res10.v;
                              } while (WVar62 != L'\0');
                              _Var53.v = local_res10.v;
                              uVar17 = uVar34;
                              if (_Var59.v != local_res10.v) {
                                PECMD_StrCopyW((WCHAR **)&local_d38,(LPCWSTR)(uintptr_t)_Var59.v,
                                              (longlong)local_res10.v - (longlong)_Var59.v >> 1);
                                _Var53.v = local_res10.v;
                                if ((*(short *)(uintptr_t)_Var59.v == 0x23) &&
                                   (*(LPCWSTR)((uintptr_t)_Var54.v + 0xc) == L'#')) {
                                  local_d9d = '\x01';
                                }
                                else {
                                  local_d9d = '\0';
                                }
                              }
                            }
                            goto LAB_140010982;
                          }
                          uVar23 = PECMD_AsciiPrefixICmp("passwd:",(ushort *)(uintptr_t)local_res10.v,7);
                          if ((char)uVar23 == '\0') {
                            uVar23 = PECMD_AsciiPrefixICmp("passwd*",(ushort *)(uintptr_t)local_res10.v,7);
                            if ((char)uVar23 == '\0') {
                              cVar13 = PECMD_MatchTokenAdvance("REALTIME",(void *)&local_res10.v,8);
                              if (cVar13 == '\0') {
                                cVar13 = PECMD_MatchTokenAdvance("HIGH",(void *)&local_res10.v,
                                                       (uint)local_da8);
                                if (cVar13 == '\0') {
                                  cVar13 = PECMD_MatchTokenAdvance("ABOVENORMAL",(void *)&local_res10.v,0xb)
                                  ;
                                  if (cVar13 == '\0') {
                                    cVar13 = PECMD_MatchTokenAdvance("NORMAL",(void *)&local_res10.v,6);
                                    if (cVar13 != '\0') {
                                      local_d60 = 0x20;
                                      goto LAB_14000f242;
                                    }
                                    cVar13 = PECMD_MatchTokenAdvance("BELOWNORMAL",(void *)&local_res10.v,
                                                           0xb);
                                    if (cVar13 == '\0') {
                                      cVar13 = PECMD_MatchTokenAdvance("LOW",(void *)&local_res10.v,3);
                                      _Var53.v = local_res10.v;
                                      if (cVar13 == '\0') {
                                        iVar52 = StrCmpNIW(L"IDLE",(LPCWSTR)(uintptr_t)local_res10.v,
                                                           (uint)local_da8);
                                        if ((iVar52 != 0) ||
                                           ((((uVar18 = *(ushort *)((uintptr_t)_Var53.v + 8),
                                              uVar18 < 9 || (0xd < uVar18)) && (uVar18 != 0x20)) &&
                                            (uVar18 != 0)))) {
                                          uVar35 = 0;
                                          cVar13 = PECMD_MatchTokenAdvance("mem",(void *)&local_res10.v,3);
                                          if (cVar13 == '\0') {
                                            cVar13 = PECMD_MatchTokenAdvance("raw",(void *)&local_res10.v,3)
                                            ;
                                            if (cVar13 == '\0') {
                                              p_Var58 = (STARTUPINFOW *)0x0;
                                              cVar13 = PECMD_MatchTokenAdvance("nfb",(void *)&local_res10.v,
                                                                     3);
                                              if (cVar13 == '\0') {
                                                local_res10.v = _Var59.v;
                                                if (*(WCHAR *)(uintptr_t)_Var59.v == L'-') goto LAB_14001094a;
                                                goto LAB_140010994;
                                              }
                                              local_d4c = '\x01';
                                              uVar35 = local_de0;
                                            }
                                            else {
                                              local_e08 = 0;
                                              local_d00 = 0;
                                              local_d9a = '\x01';
                                              uVar35 = local_de0;
                                            }
                                          }
                                          else {
                                            local_d40 = (local_d40 & 0xffffff00) | 1;
                                            uVar35 = local_de0;
                                          }
                                          goto LAB_14000f242;
                                        }
                                        local_res10.v = local_res10.v + 8;
                                        PECMD_SkipLeadingControlChars((long long *)&local_res10.v);
                                      }
                                      local_d60 = 0x40;
                                      uVar35 = local_de0;
                                      goto LAB_14000f242;
                                    }
                                    local_d60 = 0x4000;
                                  }
                                  else {
                                    local_d60 = 0x8000;
                                  }
                                }
                                else {
                                  local_d60 = 0x80;
                                }
                              }
                              else {
                                local_d60 = 0x100;
                              }
                              goto LAB_14000f242;
                            }
LAB_140010696:
                            local_e06 = '\x01';
                          }
                          else {
                            if ((*(LPCWSTR)((uintptr_t)local_res10.v + 0xe) == L'#') &&
                               (*(LPCWSTR)((uintptr_t)local_res10.v + 0x10) == L'#'))
                            goto LAB_140010696;
                            local_e06 = '\0';
                          }
                          _Var59.v = local_res10.v + 0xe;
                          WVar62 = *(WCHAR *)(uintptr_t)_Var59.v;
                          local_res10.v = _Var59.v;
                          _Var53.v = _Var59.v;
                          if (WVar62 != L'\0') {
                            do {
                              if (((8 < (ushort)WVar62) && ((ushort)WVar62 < 0xe)) ||
                                 (WVar62 == L' ')) break;
                              local_res10.v = local_res10.v + 2;
                              WVar62 = *(WCHAR *)(uintptr_t)local_res10.v;
                            } while (WVar62 != L'\0');
                            _Var53.v = local_res10.v;
                            uVar17 = uVar34;
                            if (_Var59.v != local_res10.v) {
                              ppWVar37 = &local_d20;
                              goto LAB_140010704;
                            }
                          }
                          goto LAB_140010982;
                        }
                        local_b48.v = local_res10.v;
                        if (*(WCHAR *)(uintptr_t)local_res10.v == L'#') {
                          cVar13 = '\x01';
LAB_1400104bf:
                          local_b48.v = local_res10.v + 2;
                        }
                        else {
                          cVar13 = cVar15;
                          if (*(WCHAR *)(uintptr_t)local_res10.v == L'*') {
                            cVar13 = '\x02';
                            goto LAB_1400104bf;
                          }
                        }
                        local_c10.ft.dwLowDateTime = 0;
                        local_c10.ft.dwHighDateTime = 0;
uint64_t  PECMD_EvalParenthesizedExpr(int64_t *pp, uint64_t *out);
                        uVar23 = PECMD_EvalParenthesizedExpr((void *)&local_b48.v,(void *)&local_c10.v);
                        if ((0 < (int)uVar23) && (0 < (int)local_c10.ft.dwLowDateTime)) {
                          local_c28 = local_c10.ft.dwLowDateTime;
                          local_e00 = local_e00 | 0x10;
                          local_dc0 = cVar13;
                          if (*(WCHAR *)(uintptr_t)local_b48.v == L':') {
                            local_b48.v = local_b48.v + 2;
                            PECMD_ParseHexOrDecBool((long long *)&local_b48.v,(int *)local_c18);
                          }
                        }
                      }
                      else {
                        local_res10.v = local_res10.v + 10;
                        local_bd0.v = local_res10.v;
                        cVar13 = PECMD_MatchTokenAdvance("UTF8",(void *)&local_bd0.v,-1);
                        if (cVar13 == '\0') {
                          cVar13 = PECMD_MatchTokenAdvance("ANSI",(void *)&local_bd0.v,-1);
                          if (cVar13 == '\0') {
                            cVar13 = PECMD_MatchTokenAdvance("UNI",(void *)&local_bd0.v,-1);
                            if (cVar13 == '\0') {
                              bVar14 = PECMD_ParseHexOrDecBool((long long *)&local_bd0.v,&local_d00);
                              if (0 < (int)bVar14) goto LAB_1400103e7;
                            }
                            else {
                              local_e08 = 2;
                              local_d00 = 0x4b0;
                            }
                            goto LAB_14001094a;
                          }
                          local_d00 = 0;
                        }
                        else {
                          local_d00 = 0xfde9;
                        }
LAB_1400103e7:
                        local_e08 = 1;
                      }
LAB_14001094a:
                      WVar62 = *(WCHAR *)(uintptr_t)local_res10.v;
                      while ((_Var53.v = local_res10.v, WVar62 != L'\0' &&
                             ((((ushort)WVar62 < 9 || (uVar17 = uVar34, 0xd < (ushort)WVar62)) &&
                              (uVar17 = uVar34, WVar62 != L' '))))) {
                        local_res10.v = local_res10.v + 2;
                        WVar62 = *(WCHAR *)(uintptr_t)local_res10.v;
                      }
                      goto LAB_140010982;
                    }
                    _Var53.v = local_res10.v + (longlong)local_da8;
                    uVar35 = 8;
                  }
                  else {
                    uVar35 = 0x108;
                    _Var53.v = local_res10.v + 6;
                  }
                  WVar62 = *(WCHAR *)(uintptr_t)_Var53.v;
                  local_dbc = uVar35;
                  while (WVar62 != L'\0') {
                    local_res10.v = _Var53.v;
                    if (((8 < (ushort)WVar62) && ((ushort)WVar62 < 0xe)) || (WVar62 == L' '))
                    goto LAB_14001094a;
                    uVar18 = WVar62 | 0x20;
                    if (uVar18 == 0x61) {
                      uVar35 = uVar35 | 0x40;
                      local_dbc = uVar35;
                    }
                    else if (uVar18 == 0x65) {
                      uVar35 = uVar35 | 0x80;
                      local_dbc = uVar35;
                    }
                    else if (uVar18 == 99) {
                      uVar35 = uVar35 | 0x100;
                      local_dbc = uVar35;
                    }
                    else if (uVar18 == 100) {
                      uVar35 = uVar35 | 0x200;
                      local_dbc = uVar35;
                    }
                    _Var53.v = _Var53.v + 2;
                    WVar62 = *(WCHAR *)(uintptr_t)_Var53.v;
                  }
                }
                else {
                  _Var55.v = _Var39.v;
                  for (local_res10.v = local_res10.v + 6;
                      DVar21 = _Var55.ft.dwLowDateTime, *(WCHAR *)(uintptr_t)local_res10.v == L'*';
                      local_res10.v = local_res10.v + 2) {
                    _Var55.ft.dwLowDateTime = DVar21 + 1;
                    _Var55.ft.dwHighDateTime = 0;
                  }
                  local_9a8 = 0;
                  local_958.v = local_res10.v;
                  if ((int)DVar21 < 2) {
                    if ((*(WCHAR *)(uintptr_t)local_res10.v != L'\0') &&
                       (((((ushort)*(WCHAR *)(uintptr_t)local_res10.v < 9 ||
                          (0xd < (ushort)*(WCHAR *)(uintptr_t)local_res10.v)) && (*(WCHAR *)(uintptr_t)local_res10.v != L' '))
                        && (local_res10.v = (uint64_t)PECMD_NextToken((longlong *)&local_958.v,&local_9a8,1),
                           0 < local_9a8)))) {
                      PECMD_StrCopyW((WCHAR **)&local_d10.v,(LPCWSTR)(uintptr_t)local_958.v,local_9a8);
                      local_dd0.v = local_d10.v;
                    }
                    _Var59.v = local_d10.v;
                    _Var53.v = local_res10.v;
                    if (DVar21 == 1) {
                      if (local_9a8 < 1) {
LAB_14000fb79:
                        local_dd0.v = (uint64_t)(uintptr_t)&DAT_14011dcb0;
                      }
                      else {
                        local_8b0.v = local_d10.v;
                        local_d10.ft.dwLowDateTime = 0;
                        local_d10.ft.dwHighDateTime = 0;
                        local_dd0.v = (uint64_t)PECMD_ExpandPathAlloc2((LPCWSTR)(uintptr_t)_Var59.v,&local_d10.v,(int64_t *)0x1);
                        PECMD_FreeStrBuf((WCHAR **)&local_8b0.v);
                        _Var53.v = local_res10.v;
                      }
                    }
                    else if (0 < (int)DVar21) goto LAB_14000fb79;
                  }
                  else {
                    PECMD_AllocString(&local_d10.v,0x105);
                    GetModuleFileNameW((HMODULE)0x0,(LPWSTR)(uintptr_t)local_d10.v,0x104);
                    pWVar24 = StrRChrW((LPCWSTR)(uintptr_t)local_d10.v,(LPCWSTR)0x0,L'\\');
                    if (pWVar24 != (LPWSTR)0x0) {
                      *pWVar24 = L'\0';
                    }
                    local_dd0.v = local_d10.v;
                    _Var53.v = local_res10.v;
                  }
                }
              }
              else {
                _Var59.v = local_res10.v + 8;
                WVar62 = *(WCHAR *)(uintptr_t)_Var59.v;
                if (WVar62 == L'#') {
                  uVar17 = 1;
                  uVar34 = 1;
                }
                _Var53.v = _Var59.v;
                if (WVar62 == L'\"') {
                  local_b70.v = local_res10.v + 10;
                  WVar62 = *(WCHAR *)(uintptr_t)local_b70.v;
                  _Var53.v = local_b70.v;
                  _Var41.v = _Var39.v;
                  while (WVar62 != L'\0') {
                    if ((WVar62 == L'\"') &&
                       (_Var41.ft.dwLowDateTime = _Var41.ft.dwLowDateTime + 1, _Var41.ft.dwHighDateTime = 0,
                       (_Var41.ft.dwLowDateTime & 1) != 0)) {
                      WVar62 = *(WCHAR *)((uintptr_t)_Var53.v + 2);
                      uVar34 = uVar17;
                      if (((WVar62 == L'\0') ||
                          ((8 < (ushort)WVar62 && ((ushort)WVar62 <= (ushort)(WCHAR)local_db0)))) ||
                         (WVar62 == L' ')) break;
                      uVar17 = uVar17 | 2;
                    }
                    _Var53.v = _Var53.v + 2;
                    uVar34 = uVar17;
                    WVar62 = *(WCHAR *)(uintptr_t)_Var53.v;
                  }
                  if (*(WCHAR *)(uintptr_t)_Var53.v == L'\"') {
                    _Var54.v = _Var53.v + 2;
                    bVar3 = 1;
                    _Var59.v = local_b70.v;
                    if ((*(WCHAR *)((uintptr_t)_Var53.v - 2) == L'\"') ||
                       ((iVar52 = -1, _Var53.v = _Var54.v, *(WCHAR *)(uintptr_t)local_b70.v == L'\"' &&
                        ((*(LPCWSTR)((uintptr_t)local_res10.v + 0xc) == L'\"' ||
                         (iVar52 = -1, *(WCHAR *)(uintptr_t)local_b70.v == L'\"')))))) {
                      bVar3 = 0;
                      iVar52 = -1;
                      _Var53.v = _Var54.v;
                    }
                  }
                }
                else {
                  while ((WVar62 != L'\0' &&
                         (((uVar17 = uVar34, (ushort)WVar62 < 9 || (0xd < (ushort)WVar62)) &&
                          (WVar62 != L' '))))) {
                    WVar62 = *(WCHAR *)((uintptr_t)_Var53.v + 2);
                    _Var53.v = _Var53.v + 2;
                  }
                }
                local_res10.v = _Var53.v;
                iVar52 = iVar52 + (int)((longlong)local_res10.v - (longlong)_Var59.v >> 1);
                _Var53.v = local_res10.v;
                if (0 < iVar52) {
                  PECMD_StrBldCopyWide((long long *)&local_cf8.v,(LPCWSTR)((uintptr_t)_Var59.v - 10));
                  local_d68.v = local_cf8.v + 10;
                  *(WCHAR *)((uintptr_t)local_d68.v + (longlong)iVar52 * 2) = L'\0';
                  _Var53.v = local_res10.v;
                  if ((*(WCHAR *)(uintptr_t)local_d68.v == L'*') && (*(short *)((uintptr_t)local_cf8.v + 0xc) == 0))
                  {
                    local_d68.v = (uint64_t)PECMD_GetModuleFileNameIntoContainer((uint64_t *)&local_cf8.v);
                    _Var53.v = local_res10.v;
                  }
                }
              }
            }
            else {
              _Var59.v = local_res10.v + 10;
              WVar62 = *(WCHAR *)(uintptr_t)_Var59.v;
              local_res10.v = _Var59.v;
              _Var53.v = _Var59.v;
              if (WVar62 != L'\0') {
                do {
                  if (((8 < (ushort)WVar62) && ((ushort)WVar62 < 0xe)) || (WVar62 == L' ')) break;
                  local_res10.v = local_res10.v + 2;
                  WVar62 = *(WCHAR *)(uintptr_t)local_res10.v;
                } while (WVar62 != L'\0');
                _Var53.v = local_res10.v;
                uVar17 = uVar34;
                if (_Var59.v != local_res10.v) {
                  ppWVar37 = &local_c58;
                  goto LAB_140010704;
                }
              }
            }
          }
          else {
            _Var59.v = local_res10.v + 8;
            WVar62 = *(WCHAR *)(uintptr_t)_Var59.v;
            _Var53.v = _Var59.v;
            while ((WVar62 != L'\0' &&
                   ((((ushort)WVar62 < 9 || (uVar17 = uVar34, 0xd < (ushort)WVar62)) &&
                    (uVar17 = uVar34, WVar62 != L' '))))) {
              _Var53.v = _Var53.v + 2;
              WVar62 = *(WCHAR *)(uintptr_t)_Var53.v;
            }
            local_b70.v = _Var59.v;
            if ((*(WCHAR *)(uintptr_t)_Var59.v == L'(') ||
               ((*(WCHAR *)(uintptr_t)_Var59.v == L'0' &&
                ((WCHAR)(*(LPCWSTR)((uintptr_t)local_res10.v + 10) | 0x20U) == 0x78)))) {
              local_res10.v = _Var53.v;
              PECMD_ParseHexOrDec((longlong *)&local_b70.v,&local_a98);
              _Var53.v = local_res10.v;
            }
            else if (_Var59.v != _Var53.v) {
              ppWVar37 = &local_c68;
              local_res10.v = _Var53.v;
LAB_140010704:
              pvVar32 = (HANDLE)((longlong)local_res10.v - (longlong)_Var59.v >> 1);
LAB_14001070a:
              PECMD_StrCopyW((WCHAR **)ppWVar37,(LPCWSTR)(uintptr_t)_Var59.v,(longlong)pvVar32);
              _Var53.v = local_res10.v;
            }
          }
LAB_140010982:
          local_res10.v = _Var53.v;
          PECMD_SkipLeadingControlChars((long long *)&local_res10.v);
          uVar35 = local_de0;
          goto LAB_14000f242;
        }
        local_b50.v = local_res10.v + 0x10;
        WVar62 = *(WCHAR *)(uintptr_t)local_b50.v;
        local_res10.v = local_b50.v;
        while (WVar62 != L'\0') {
          if (((8 < (ushort)WVar62) && ((ushort)WVar62 < 0xe)) || (WVar62 == L' ')) {
            if (*(WCHAR *)(uintptr_t)local_res10.v != L'\0') {
              local_res10.v = local_res10.v + 2;
            }
            break;
          }
          local_res10.v = local_res10.v + 2;
          WVar62 = *(WCHAR *)(uintptr_t)local_res10.v;
        }
        PECMD_SkipLeadingControlChars((long long *)&local_res10.v);
        GetCursorPos(&local_b38);
        bVar6 = true;
        if ((*(WCHAR *)(uintptr_t)local_b50.v != L'\0') &&
           (((ushort)*(WCHAR *)(uintptr_t)local_b50.v < 0x30 || (0x39 < (ushort)*(WCHAR *)(uintptr_t)local_b50.v)))) {
          local_b50.v = local_b50.v + 2;
        }
        PECMD_ParseUIntValue((longlong *)&local_b50.v,(int *)&local_b38.x);
        if ((*(WCHAR *)(uintptr_t)local_b50.v != L'\0') &&
           (((ushort)*(WCHAR *)(uintptr_t)local_b50.v < 0x30 || (0x39 < (ushort)*(WCHAR *)(uintptr_t)local_b50.v)))) {
          local_b50.v = local_b50.v + 2;
        }
        PECMD_ParseUIntValue((longlong *)&local_b50.v,(int *)&local_b38.y);
      }
      else {
        _Var59.v = local_res10.v + 0x12;
        WVar62 = *(WCHAR *)(uintptr_t)_Var59.v;
        local_res10.v = _Var59.v;
        while (((WVar62 != L'\0' &&
                (((ushort)WVar62 < 9 || (uVar17 = uVar34, 0xd < (ushort)WVar62)))) &&
               (uVar17 = uVar34, WVar62 != L' '))) {
          local_res10.v = local_res10.v + 2;
          WVar62 = *(WCHAR *)(uintptr_t)local_res10.v;
        }
        PECMD_StrCopyW((WCHAR **)&local_ce0,(LPCWSTR)(uintptr_t)_Var59.v,
                      (longlong)local_res10.v - (longlong)_Var59.v >> 1);
        PECMD_SkipLeadingControlChars((long long *)&local_res10.v);
      }
    }
    else {
      param_5 = param_5 | 2;
      local_d48 = 1;
      local_e00 = 1;
      local_res10.v = local_res10.v + 10;
      uVar35 = uVar35 & 0xfffffffe;
      local_948 = 0;
      local_de0 = uVar35;
      local_940.v = local_res10.v;
      local_res10.v = (uint64_t)PECMD_NextToken((longlong *)&local_940.v,&local_948,5);
      if (0 < local_948) {
        PECMD_StrCopyW((WCHAR **)&local_a10.QuadPart,(LPCWSTR)(uintptr_t)local_940.v,local_948);
      }
    }
    goto LAB_14000f242;
  }
  local_de8 = 1;
  goto LAB_14000f242;
LAB_140010994:
  if (local_dbc != 0) {
    if (bVar16) {
      local_dbc = local_dbc | (uint)local_da8;
    }
    if (local_dbc != 0) {
      if (bVar2) {
        local_dbc = local_dbc | 0x20;
      }
      if ((local_dbc != 0) && (local_d9c != '\0')) {
        local_dbc = local_dbc | 0x400;
      }
    }
  }
  if (local_c40.ft.dwLowDateTime != 0) {
    p_Var58 = &local_888;
    local_888.cb = 0x70;
    PECMD_OpenProcessMemory((void *)&local_a80,(DWORD)local_c40.ft.dwLowDateTime,(long long)(uintptr_t)p_Var58);
    local_db4 = -(uint)(local_a78 != 0) & 0x80000;
  }
  local_988 = (uint)local_e08;
  if (local_d00 == 0x4b0) {
    local_988 = 2;
  }
  if (((local_980.v != 0) && (local_res10.v = local_980.v, local_da0 == '\0')) &&
     (local_b18.v == 0)) {
    p_Var58 = (STARTUPINFOW *)local_ca8;
    PECMD_ExpandVarDispatch(param_1,(WCHAR *)(uintptr_t)local_980.v,(undefined8 *)p_Var58,0,1);
    local_res10.ft.dwLowDateTime = *(uint32_t *)&local_ca8[0];
    local_res10.ft.dwHighDateTime = *(uint32_t *)&local_ca8[4];
  }
  local_a68 = (short *)0x0;
  local_a60 = 0;
  local_a58 = 0;
  local_c00 = (HANDLE *)0x0;
  if (((local_d68.v == 0) || (*(WCHAR *)(uintptr_t)local_d68.v == L'\0')) || (uVar17 != 0)) {
    _Var47.ft.dwLowDateTime = 0;
    _Var47.ft.dwHighDateTime = 0;
    local_d40 = 0;
  }
  else {
    _Var47.ft.dwHighDateTime = 0;
    _Var47.ft.dwLowDateTime = local_d40;
  }
  uVar25 = (ulonglong)p_Var58 & 0xffffffffffff0000;
  lVar61 = ((_Var59.v >> 16) & 0xffffffffffffffULL) << 16;
  local_db8 = uVar35 & 0xffff0000;
  local_aa0.v = (uint64_t)(uintptr_t)DAT_14011d108;   /* @0x14011d108 = L".tmp" */
  local_db0 = (DWORD)uVar25;
  local_dd8[0] = (DWORD)lVar61;
  if (local_d68.v != 0) {
    if (*(WCHAR *)(uintptr_t)local_d68.v == L'*') {
      do {
        local_d68.v = local_d68.v + 2;
        uVar25 = (uint64_t)(((int64_t)(uVar25 >> 16) << 16) | (uint16_t)((uint16_t)uVar25 + 1));
      } while (*(WCHAR *)(uintptr_t)local_d68.v == L'*');
      local_db0 = (DWORD)uVar25;
    }
    if (*(WCHAR *)(uintptr_t)local_d68.v == L'?') {
      local_aa0.v = local_d68.v + 2;
      WVar62 = *(WCHAR *)(uintptr_t)local_aa0.v;
      _Var53.v = local_aa0.v;
      while (local_d68.v = _Var53.v, WVar62 != L'\0') {
        local_d68.v = _Var53.v + 2;
        if (WVar62 == L':') {
          *(WCHAR *)(uintptr_t)_Var53.v = L'\0';
          break;
        }
        _Var53.v = local_d68.v;
        WVar62 = *(WCHAR *)(uintptr_t)local_d68.v;
      }
    }
    if ((((WCHAR)(*(WCHAR *)(uintptr_t)local_d68.v | 0x20U) == 99) &&
        ((WCHAR)(*(WCHAR *)((uintptr_t)local_d68.v + 2) | 0x20U) == (ushort)local_d08)) &&
       (((WCHAR)(*(WCHAR *)((uintptr_t)local_d68.v + 4) | 0x20U) == 0x62 &&
        (*(WCHAR *)((uintptr_t)local_d68.v + 6) == L':')))) {
      lVar61 = 1;
      local_dd8[0] = 1;
      local_d68.v = local_d68.v + 8;
    }
    else if ((((WCHAR)(*(WCHAR *)(uintptr_t)local_d68.v | 0x20U) == 0x65) &&
             ((WCHAR)(*(WCHAR *)((uintptr_t)local_d68.v + 2) | 0x20U) == 0x78)) &&
            (*(WCHAR *)((uintptr_t)local_d68.v + 4) == L':')) {
      local_d68.v = local_d68.v + 6;
      lVar61 = (_Var59.v & ~0xffffULL) | 0x100;
      local_dd8[0] = (DWORD)lVar61;
    }
    if ((((local_d68.v != 0) && ((WCHAR)(*(WCHAR *)(uintptr_t)local_d68.v | 0x20U) == 0x75)) &&
        ((WCHAR)(*(WCHAR *)((uintptr_t)local_d68.v + 2) | 0x20U) == 100)) &&
       (*(WCHAR *)((uintptr_t)local_d68.v + 4) == L':')) {
      local_d68.v = local_d68.v + 6;
      local_db8 = 1;
    }
  }
  sVar57 = (short)uVar25;
  sVar49 = (short)lVar61;
  sVar51 = (short)local_db8;
  if ((((char)_Var47.ft.dwLowDateTime != '\0') && (local_d68.v != 0)) &&
     ((sVar49 == 0 && (sVar51 == 0)))) {
    sVar19 = *(short *)(uintptr_t)local_d68.v;
    uVar35 = (uint)bVar3;
    _Var59.v = local_d68.v;
    if (sVar19 != 0) {
      do {
        if (((sVar19 == 0x22) && (uVar35 = uVar35 + 1, 1 < (int)uVar35)) &&
           (_Var47.v = _Var47.v & 0xff, *(short *)((uintptr_t)_Var59.v + 2) != 0))
        {
          _Var47.v = _Var39.v;
        }
        sVar19 = *(short *)((uintptr_t)_Var59.v + 2);
        _Var59.v = _Var59.v + 2;
      } while (sVar19 != 0);
      sVar57 = (short)local_db0;
      local_d40 = _Var47.ft.dwLowDateTime;
    }
  }
  if (((sVar57 != 0) || (sVar49 != 0)) || (sVar51 != 0)) {
    bVar3 = 0;
  }
  if ((local_d68.v != 0) &&
     (ppvVar26 = (HANDLE *)StrChrW((LPCWSTR)(uintptr_t)local_d68.v,L'*'), local_c00 = ppvVar26,
     ppvVar26 != (HANDLE *)0x0)) {
    local_c00 = (HANDLE *)((uintptr_t)ppvVar26 + 2);
    *(WCHAR *)ppvVar26 = L'\0';
  }
  if (((char)_Var47.ft.dwLowDateTime != '\0') && (sVar49 == 0 && sVar51 == 0)) {
    PECMD_LoadFileToSlot((LPCWSTR)(uintptr_t)local_d68.v,(longlong *)&local_a68);
    local_db0 = 0;
  }
  *(uint32_t *)local_990 = 1;
  if (local_bd8.v != 0) {
    local_de8 = 1;
  }
  local_970[1] = (HANDLE)0x0;
  local_8f0.InternalHigh = 0;
  local_8f0.u.Pointer = (PVOID)0x0;
  local_8f0.hEvent = (HANDLE)0x0;
  local_d30.QuadPart = 0;
  local_df4 = 0;
  local_df8 = 1;
  local_d08 = 0;
  local_bf0 = (HANDLE)0x0;
  local_be0.ft.dwLowDateTime = 0;
  local_be0.ft.dwHighDateTime = 0;
  local_b20.ft.dwLowDateTime = 0;
  local_b20.ft.dwHighDateTime = 0;
  local_cf0 = (HANDLE)0x0;
  local_c88.ft.dwLowDateTime = 0;
  local_c88.ft.dwHighDateTime = 0;
  local_c20.ft.dwLowDateTime = 0;
  local_c20.ft.dwHighDateTime = 0;
  local_d80 = (HANDLE)0x0;
  local_d70.ft.dwLowDateTime = 0;
  local_d70.ft.dwHighDateTime = 0;
  local_970[0] = (HANDLE)0x0;
  local_8f0.Internal = 0;
  local_910.nLength = 0x18;
  local_910.lpSecurityDescriptor = (LPVOID)0x0;
  local_910.bInheritHandle = 1;
  local_c60.ft.dwLowDateTime = 0;
  local_c60.ft.dwHighDateTime = 0;
  if ((char)local_d50 != '\0') {
    local_de8 = 1;
    local_888.dwFlags = local_888.dwFlags | 0x100;
    local_888.hStdOutput = g_hStdErr;
    local_888.hStdError = g_hStdOut;
    local_888.hStdInput = g_hStdIn;
  }
  if (local_da0 != '\0') {
    local_bd0.ft.dwLowDateTime = 0;
    local_bd0.ft.dwHighDateTime = 0;
    if (*(WCHAR *)(uintptr_t)local_res10.v == L'|') {
      do {
        do {
          local_res10.v = local_res10.v + 2;
        } while (*(WCHAR *)(uintptr_t)local_res10.v == L'|');
      } while (((8 < (ushort)*(WCHAR *)(uintptr_t)local_res10.v) && ((ushort)*(WCHAR *)(uintptr_t)local_res10.v < 0xe)) ||
              (*(WCHAR *)(uintptr_t)local_res10.v == L' '));
    }
    pvVar32 = (HANDLE)0x0;
    local_c40.v = local_res10.v;
    do {
      do {
        local_dc8.ft.dwLowDateTime = 0;
        local_dc8.ft.dwHighDateTime = 0;
        _Var59.v = local_res10.v;
        _Var53.v = _Var39.v;
        if (*(WCHAR *)(uintptr_t)local_res10.v == L'\0') {
LAB_140011032:
          local_dc8.ft.dwLowDateTime = 0;
          local_dc8.ft.dwHighDateTime = 0;
          bVar14 = false;
          goto LAB_14001103c;
        }
        do {
          _Var54.v = _Var53.v;
          _Var60.v = local_dc8.v;
          if (*(WCHAR *)(uintptr_t)_Var59.v == L'|') {
            WVar62 = *(LPCWSTR)((uintptr_t)_Var59.v + 2);
            _Var54.v = _Var59.v;
            _Var60.v = _Var59.v;
            while (WVar62 == L'|') {
              _Var59.v = _Var59.v + 2;
              _Var54.v = _Var53.v;
              _Var60.v = local_dc8.v;
              WVar62 = *(WCHAR *)(uintptr_t)_Var59.v;
            }
          }
          local_dc8.v = _Var60.v;
          _Var59.v = _Var59.v + 2;
          _Var53.v = _Var54.v;
        } while (*(WCHAR *)(uintptr_t)_Var59.v != L'\0');
        if (_Var54.v == 0) goto LAB_140011032;
        *(WCHAR *)(uintptr_t)_Var54.v = L'\0';
        local_dc8.v = local_dc8.v + 2;
        PECMD_SkipLeadingControlChars((long long *)&local_dc8.v);
        WVar62 = *(WCHAR *)(uintptr_t)local_dc8.v;
        local_c10.v = local_dc8.v;
      } while (WVar62 == L'\0');
      do {
        if ((WVar62 == L'|') &&
           (local_b70.v = local_c10.v + 2, *(WCHAR *)(uintptr_t)local_b70.v == L'|')) {
          iVar52 = lstrlenW((LPCWSTR)(uintptr_t)local_c10.v);
          PECMD_MemMoveSafe((undefined1 *)local_c10.v,(longlong)local_b70.v,iVar52 * 2);
          for (; *(short *)(uintptr_t)local_c10.v == 0x7c; local_c10.v = local_c10.v + 2) {
          }
        }
        local_c10.v = local_c10.v + 2;
        WVar62 = *(WCHAR *)(uintptr_t)local_c10.v;
      } while (WVar62 != L'\0');
      local_bf8.v = local_dc8.v;
    } while (*(WCHAR *)(uintptr_t)local_dc8.v == L'\0');
    cVar13 = '\x01';
    bVar14 = true;
    if (*(WCHAR *)(uintptr_t)local_c40.v == L'\0') {
LAB_14001103c:
      cVar13 = cVar15;
      if (!bVar14) {
        local_dc8.v = local_c40.v;
      }
    }
    _Var59.v = _Var39.v;
    _Var54.v = local_dc8.v;
    local_bf8.v = local_dc8.v;
    _Var53.v = _Var39.v;
    if (cVar13 != '\0') {
      local_ac8 = (HANDLE)(uintptr_t)GetCurrentProcess();
      local_8a8.nLength = 0x18;
      local_8a8.lpSecurityDescriptor = (LPVOID)0x0;
      local_8a8.bInheritHandle = 1;
      BVar20 = CreatePipe((void *)&local_d80,(void *)&local_d70.v,(void *)&local_8a8,0);
      if (BVar20 == 0) {
        PECMD_WaitHandlesOrMessages((uint64_t)(uintptr_t)param_1,1,0,(uint64_t *)0);
      }
      DuplicateHandle((void *)(uintptr_t)local_ac8,local_d80,(void *)(uintptr_t)local_ac8,&local_d80,0,0,3);
      local_d50 = local_d50 & 0xffff0000;
      if ((local_d70.v != 0) &&
         (PECMD_ExecCmdDispatch((uint64_t)(uintptr_t)param_1,local_c40.v,(uint64_t)(uintptr_t)param_1,
                      (uint64_t)(uintptr_t)&local_d50,0,(uint64_t)0,local_d70.v,(uint64_t)0), local_d70.v != 0)) {
        CloseHandle((HANDLE)(uintptr_t)local_d70.v);
      }
      local_d70.ft.dwLowDateTime = 0;
      local_d70.ft.dwHighDateTime = 0;
      DuplicateHandle((void *)(uintptr_t)local_ac8,local_d80,(void *)(uintptr_t)local_ac8,&local_d80,0,1,3);
      _Var54.v = local_bf8.v;
      local_dc8.v = local_bf8.v;
      if (local_d80 != (HANDLE)0x0) {
        local_de8 = 1;
        local_888.dwFlags = local_888.dwFlags | 0x100;
        local_888.hStdInput = local_d80;
      }
    }
    while (WVar62 = *(WCHAR *)(uintptr_t)_Var54.v, WVar62 != L'\0') {
      do {
        if ((WVar62 == L'<') || (WVar62 == L'>')) break;
        _Var54.v = _Var54.v + 2;
        WVar62 = *(WCHAR *)(uintptr_t)_Var54.v;
        local_dc8.v = _Var54.v;
      } while (WVar62 != L'\0');
      if (*(WCHAR *)(uintptr_t)_Var54.v == L'\0') break;
      if (*(WCHAR *)(uintptr_t)_Var54.v == L'<') {
        *(WCHAR *)(uintptr_t)_Var54.v = L'\0';
        local_dc8.v = local_dc8.v + 2;
        PECMD_SkipLeadingControlChars((long long *)&local_dc8.v);
        _Var54.v = local_dc8.v;
        if (*(short *)(uintptr_t)local_dc8.v != 0) {
          local_ab0.v = local_dc8.v;
          WVar62 = *(WCHAR *)(uintptr_t)local_dc8.v;
          _Var60.v = local_dc8.v;
          while (((WVar62 != L'\0' && (_Var59.v = local_c88.v, WVar62 != L'<')) && (WVar62 != L'>'))) {
            _Var60.v = _Var60.v + 2;
            WVar62 = *(WCHAR *)(uintptr_t)_Var60.v;
          }
          lVar61 = (longlong)_Var60.v - (longlong)local_dc8.v;
          local_dc8.v = _Var60.v;
          PECMD_StrBldCopyWideN((uint16_t **)&local_a08,(LPCWSTR)(uintptr_t)_Var54.v,lVar61 >> 1);
          PECMD_AllocStrSlot((uint16_t **)&local_b80.v);
          PECMD_ExpandVarDispatch(param_1,(WCHAR *)(uintptr_t)local_ab0.v,&local_b80.v,0,1);
          local_ab0.v = local_b80.v;
          PECMD_StripTrailingSpaces((LPCWSTR)(uintptr_t)local_b80.v);
          PECMD_SkipLeadingControlChars((long long *)&local_ab0.v);
          if (pvVar32 != (HANDLE)0x0) {
            CloseHandle(pvVar32);
          }
          local_cf0 = (HANDLE)0x0;
          PECMD_OpenFileHandle(&local_cf0,(LPCWSTR)(uintptr_t)local_ab0.v,0xc0000000,7,&local_910,3,0,(HANDLE)0x0);
          if (local_cf0 == (HANDLE)0x0) {
            PECMD_OpenFileHandle(&local_cf0,(LPCWSTR)(uintptr_t)local_ab0.v,0xc0000000,7,&local_910,3,0x2000000,
                          (HANDLE)0x0);
          }
          pvVar32 = local_cf0;
          PECMD_FreeStrBuf((WCHAR **)&local_b80.v);
          PECMD_FreeStrBuf((WCHAR **)&local_a08);
          _Var54.v = local_dc8.v;
        }
      }
      if (*(WCHAR *)(uintptr_t)_Var54.v == L'>') {
        *(WCHAR *)(uintptr_t)_Var54.v = L'\0';
        local_e08 = 1;
        if (*(LPCWSTR)((uintptr_t)local_dc8.v - 2) == L'2') {
          *(WCHAR *)((uintptr_t)local_dc8.v - 2) = L'\0';
          local_e08 = 2;
        }
        _Var54.v = local_dc8.v + 2;
        WVar62 = *(WCHAR *)(uintptr_t)_Var54.v;
        if (WVar62 == L'>') {
          _Var54.v = local_dc8.v + 4;
        }
        local_d50 = (uint)(WVar62 == L'>');
        local_dc8.v = _Var54.v;
        if (*(WCHAR *)(uintptr_t)_Var54.v == L'&') {
          local_dc8.v = _Var54.v + 2;
          local_e08 = 3;
        }
        PECMD_SkipLeadingControlChars((long long *)&local_dc8.v);
        _Var54.v = local_dc8.v;
        if (*(short *)(uintptr_t)local_dc8.v != 0) {
          local_b40.v = local_dc8.v;
          WVar62 = *(WCHAR *)(uintptr_t)local_dc8.v;
          _Var60.v = local_dc8.v;
          while (((WVar62 != L'\0' && (_Var53.v = local_c20.v, WVar62 != L'<')) && (WVar62 != L'>'))) {
            _Var60.v = _Var60.v + 2;
            WVar62 = *(WCHAR *)(uintptr_t)_Var60.v;
          }
          lVar61 = (longlong)_Var60.v - (longlong)local_dc8.v;
          local_dc8.v = _Var60.v;
          PECMD_StrBldCopyWideN((uint16_t **)&local_bb0,(LPCWSTR)(uintptr_t)_Var54.v,lVar61 >> 1);
          PECMD_AllocStrSlot((uint16_t **)&local_a38.v);
          PECMD_ExpandVarDispatch(param_1,(WCHAR *)(uintptr_t)local_b40.v,&local_a38.v,0,1);
          local_b40.v = local_a38.v;
          PECMD_StripTrailingSpaces((LPCWSTR)(uintptr_t)local_a38.v);
          PECMD_SkipLeadingControlChars((long long *)&local_b40.v);
          if ((local_e08 & 1) != 0) {
            if (_Var59.v != 0) {
              CloseHandle((HANDLE)(uintptr_t)_Var59.v);
            }
            local_c88.ft.dwLowDateTime = 0;
            local_c88.ft.dwHighDateTime = 0;
            _Var59.v = _Var39.v;
          }
          if ((local_e08 & 2) != 0) {
            if (_Var53.v != 0) {
              CloseHandle((HANDLE)(uintptr_t)_Var53.v);
            }
            local_c20.ft.dwLowDateTime = 0;
            local_c20.ft.dwHighDateTime = 0;
            local_bd0.ft.dwLowDateTime = 0;
            local_bd0.ft.dwHighDateTime = 0;
            _Var53.v = _Var39.v;
          }
          local_a00 = (WCHAR *)0x0;
          local_c38.v = (uint64_t)PECMD_OpenFileHandle((HANDLE *)&local_a00,(LPCWSTR)(uintptr_t)local_b40.v,0x40000000,7,&local_910,
                                    (DWORD)local_da8,0,(HANDLE)0x0);
          if (local_c38.v == 0) {
            local_b60 = 0;
            local_c38.v = (uint64_t)PECMD_OpenFileHandle((HANDLE *)&local_b60,(LPCWSTR)(uintptr_t)local_b40.v,0x40000000,7,&local_910,
                                      (DWORD)local_da8,0x2000000,(HANDLE)0x0);
            if (local_c38.v != 0) goto LAB_140011699;
            local_d88.ft.dwLowDateTime = 0;
            local_d88.ft.dwHighDateTime = 0;
            local_c38.v = (uint64_t)PECMD_OpenFileHandle((HANDLE *)&local_d88.v,(LPCWSTR)(uintptr_t)local_b40.v,0x40000000,7,&local_910,3,
                                      0x2000000,(HANDLE)0x0);
            if (local_c38.v != 0) goto LAB_140011699;
          }
          else {
LAB_140011699:
            if (local_d50 != 0) {
              SetFilePointer((HANDLE)(uintptr_t)local_c38.v,0,(LONG *)0x0,2);
            }
            if ((local_e08 == 1) || (local_bd0.v = local_c38.v, local_e08 == 3)) {
              local_c88.v = local_c38.v;
              _Var59.v = local_c38.v;
            }
            else {
              local_c20.v = local_c38.v;
              _Var53.v = local_c38.v;
            }
          }
          PECMD_FreeStrBuf((WCHAR **)&local_a38.v);
          PECMD_FreeStrBuf((WCHAR **)&local_bb0);
          _Var54.v = local_dc8.v;
        }
      }
    }
    if (pvVar32 != (HANDLE)0x0) {
      local_888.dwFlags = local_888.dwFlags | 0x100;
      local_de8 = 1;
      local_888.hStdInput = pvVar32;
    }
    if (_Var59.v != 0) {
      local_888.dwFlags = local_888.dwFlags | 0x100;
      local_de8 = 1;
      local_888.hStdOutput = (HANDLE)(uintptr_t)_Var59.v;
    }
    if (local_bd0.v != 0) {
      local_888.dwFlags = local_888.dwFlags | 0x100;
      local_de8 = 1;
      local_888.hStdError = (HANDLE)(uintptr_t)local_bd0.v;
    }
    PECMD_ExpandVarDispatch(param_1,(WCHAR *)(uintptr_t)local_bf8.v,&local_c60.v,0,1);
    local_res10.v = local_c60.v;
  }
  bVar46 = 0;
  do {
    _Var53.v = local_c20.v;
    _Var59.v = local_c88.v;
    pvVar32 = local_cf0;
    WVar62 = *(WCHAR *)(uintptr_t)local_res10.v;
    if ((WVar62 == L'!') || (WVar62 == L'*')) {
LAB_140011813:
      local_dfc = 0x10000000;
    }
    else {
      if (WVar62 == L'@') {
        PECMD_StrBldCopyWide((long long *)&local_ac0,(const uint16_t *)L"WinSta0\\WinLogon");
        local_888.lpDesktop = local_ac0;
        bVar46 = bVar46 + 1;
        goto LAB_140011813;
      }
      if (WVar62 == L'=') {
        local_e00 = local_e00 | 1;
        local_c80 = local_c80 + 1;
      }
      else if (WVar62 == L'$') {
        bVar4 = true;
      }
      else if (WVar62 == L'&') {
        local_dfc = 3;
      }
      else {
        if ((WVar62 != L'^') && (WVar62 != L'_')) goto LAB_140011875;
        local_dfc = 6;
      }
    }
    local_res10.v = local_res10.v + 2;
  } while( true );
LAB_140015a5e:
  PECMD_FreeStrBuf((WCHAR **)&local_b18.v);
LAB_140015c69:
  pvVar32 = local_c70;
  *(uint8_t *)((uintptr_t)local_dd0.v + 2 + (uintptr_t)_Var39.v) = 0;
  ((uint8_t *)((uintptr_t)local_dd0.v + 1))[(uintptr_t)_Var39.v] = 0;
  *(uint8_t *)((uintptr_t)local_dd0.v + (uintptr_t)_Var39.v) = 0;
  local_db4 = 0;
  if (((local_e00 != 0xffffff9c) && ('\0' < local_e07)) && (local_dc0 != '\x01')) {
    if (local_dc0 == '\x02') {
      wsprintfW((LPWSTR)local_518,(const uint16_t *)L"-force -exitcode=%d -tree *%d",(ulonglong)local_c18[0],
                (ulonglong)local_af8.dwProcessId);
      PECMD_CmdKill(param_1,local_518);
      pvVar32 = local_c70;
    }
    else {
      if (local_c70 != (HANDLE)0x0) {
        TerminateJobObject(local_c70,local_c18[0]);
      }
      TerminateProcess(local_af8.hProcess,local_c18[0]);
    }
  }
  GetExitCodeProcess(local_af8.hProcess,&local_db4);
  *(uint32_t *)local_990 = local_db4;
  if ((short)local_ddc != 0) {
    if (((*local_d58 == L'@') || (local_d9a == '\0')) ||
       ((*local_d58 != L'&' && ((*(byte *)((uintptr_t)param_1 + 0xd) & 0xf) == 0)))) {
      bVar2 = false;
    }
    else {
      bVar2 = true;
    }
    PECMD_AllocSmallObject((void **)&local_a50.v);
    local_a48.ft.dwLowDateTime = 0;
    local_a48.ft.dwHighDateTime = 0;
    local_d18.v = local_dd0.v;
    local_a40.ft.dwLowDateTime = 0;
    local_a40.ft.dwHighDateTime = 0;
    local_b08.v = _Var39.v;
    if (!bVar2) {
      if ((char)local_988 == '\x02') {
        *(uint16_t *)((uintptr_t)local_dd0.v + (uintptr_t)_Var39.v) = 0;
        lpString.v = local_dd0.v;
LAB_140015efd:
        if (lpString.v == 0) goto LAB_140015f02;
      }
      else {
        if ((char)local_988 != '\x01') {
          PECMD_AllocString(&local_a50.v,(longlong)((uintptr_t)_Var39.v + 0x18));
          local_a48.v = _Var39.v;
          local_a40.v = _Var39.v;
          PECMD_MemMoveForward((uint8_t *)(uintptr_t)local_a50.v,(uint8_t *)(uintptr_t)local_dd0.v,_Var39.ft.dwLowDateTime + 1);
          FUN_140102a90((ulonglong *)((longlong)local_a50.v + (uintptr_t)_Var39.v),0,0x10);
          PECMD_ResDecode((longlong *)&local_a50.v,0x2004);
          local_d18.v = local_a50.v;
          local_b08.v = local_a48.v;
          goto LAB_140015efd;
        }
        PECMD_StrDupA((WCHAR **)&local_a50.v,(LPCWSTR)(uintptr_t)local_dd0.v,(ulonglong)_Var39.v,(longlong)local_d00
                     );
        local_d18.v = local_a50.v;
        iVar52 = lstrlenW((LPCWSTR)(uintptr_t)local_a50.v);
        local_b08.v = (uint64_t)((longlong)iVar52 * 2);
LAB_140015f02:
        lpString.v = local_a50.v;
      }
      if ((byte)((byte)local_da8 & (byte)local_ddc) == 0) {
        if ((local_ddc & 1) == 0) {
          if ((local_ddc & 2) == 0) goto LAB_140015f6c;
          WVar62 = *(WCHAR *)(uintptr_t)lpString.v;
          while (WVar62 != L'\0') {
            if ((WVar62 == L'\r') || (WVar62 == L'\n')) {
              iVar52 = lstrlenW((LPCWSTR)(uintptr_t)lpString.v);
              PECMD_MemMoveSafe((uint8_t *)(uintptr_t)lpString.v,(longlong)((uintptr_t)lpString.v + 2),iVar52 * 2);
            }
            else {
              lpString.v = lpString.v + 2;
            }
            pvVar32 = local_c70;
            WVar62 = *(WCHAR *)(uintptr_t)lpString.v;
          }
        }
        else {
          WVar62 = *(WCHAR *)(uintptr_t)lpString.v;
          uVar25 = local_aa8;
          while (WVar62 != L'\0') {
            pvVar32 = local_c70;
            if ((WVar62 == L'\r') && (*(LPCWSTR)((uintptr_t)lpString.v + 2) == L'\n')) {
              if (1 < (longlong)uVar25) {
                lpString.v = lpString.v + 2;
              }
LAB_140015fd4:
              uVar25 = uVar25 - 1;
              if ((longlong)uVar25 < 1) break;
            }
            else if (WVar62 == L'\n') goto LAB_140015fd4;
            lpString.v = lpString.v + 2;
            WVar62 = *(WCHAR *)(uintptr_t)lpString.v;
          }
          *(WCHAR *)(uintptr_t)lpString.v = L'\0';
        }
        local_b08.v = (uint64_t)((uintptr_t)lpString.v - (uintptr_t)local_d18.v);
      }
      else {
        iVar52 = lstrlenW((LPCWSTR)(uintptr_t)lpString.v);
        for (local_b08.v = (uintptr_t)lpString.v - 2 + (longlong)iVar52 * 2;
            ((ulonglong)lpString.v <= (ulonglong)local_b08.v &&
            ((pvVar32 = local_c70, *(short *)(uintptr_t)local_b08.v == 0xd || (*(short *)(uintptr_t)local_b08.v == 10))));
            local_b08.v = (uintptr_t)local_b08.v - 2) {
          *(short *)(uintptr_t)local_b08.v = 0;
        }
        local_b08.v = (uint64_t)((uintptr_t)local_b08.v - (uintptr_t)lpString.v);
      }
    }
LAB_140015f6c:
    if (local_d9f == '\0') {
      _Var39.v = local_d18.v;
      if (local_d9e != '\0') {
        _Var39.v = (uint64_t)FUN_140063060((uint64_t)(uintptr_t)local_d18.v);
      }
    }
    else {
      _Var39.v = (uint64_t)PECMD_ConvertLfToCrlf((LPCWSTR)(uintptr_t)local_d18.v,&local_b10);
    }
    if (*local_d58 == L'@') {
      PECMD_DispatchByObjectName((uint64_t *)param_1,local_8b8,(longlong)local_8b8,(uint16_t *)&DAT_14011c638,_Var39.v,_Var39.v);
    }
    else if (bVar2) {
      PECMD_VarWriteLine(param_1,local_d58,(LPCWSTR)(uintptr_t)_Var39.v,(ulonglong)local_b08.v & 0xffffffff,
                    (longlong *)0x0,'\0');
    }
    else {
      PECMD_SetVariable(param_1,local_d58,(LPCWSTR)(uintptr_t)local_a50.v);
    }
    PECMD_FreeStrBuf((WCHAR **)&local_a50.v);
  }
  local_df8 = local_db4;
  local_558[0] = L'\0';
  wsprintfW(local_558,(const uint16_t *)L"%ld",(ulonglong)local_db4);
  PECMD_SetVariable((void *)param_1,(const uint16_t *)L"&&ERRORLEVEL",local_558);
  if ((pvVar32 != (HANDLE)0x0) && (local_dc0 != '\x01')) {
    TerminateJobObject(pvVar32,local_c18[0]);
  }
  PECMD_FreeStrBuf(&local_b10);
  PECMD_FreeStrBuf((WCHAR **)&local_dd0.v);
  pvVar45 = local_bc8;
  DVar21 = local_e00;
  goto LAB_140014f96;
LAB_140011875:
  local_dbf = bVar46;
  if ((local_dfc != 0x10000000) && ((char)local_de0 != '\0')) {
    PECMD_StrBldCopyWide((long long *)&local_ac0,(const uint16_t *)L"WinSta0\\Default");
    local_888.lpDesktop = local_ac0;
  }
  if (((local_888.lpDesktop != (LPWSTR)0x0) && ((local_dbf & 1) != 0)) &&
     (uVar35 = PECMD_DetectMinintBoot(), (int)uVar35 < 1)) {
    local_888.lpDesktop = (LPWSTR)0x0;
  }
  if (param_7 != 0) {
    local_888.dwFlags = local_888.dwFlags | 0x100;
    local_e00 = 0;
    local_ddc = local_ddc & 0xffff0000;
    local_de8 = 1;
    local_888.hStdOutput = (HANDLE)(uintptr_t)param_7;
  }
  if ((local_dbc != 0) && (local_e00 != 0)) {
    local_dbc = local_dbc | 0x10;
  }
  if ((bVar4) && (*(WCHAR *)(uintptr_t)*(uint64_t *)&local_ca8[16] == L'\0')) {
    PECMD_AssignString((int64_t *)(local_ca8 + 0x10),(const uint16_t *)L"open");
  }
  if (local_d3b != '\0') {
    pWVar24 = StrChrW((LPCWSTR)(uintptr_t)local_res10.v,L'\\');
    _Var39.v = local_res10.v;
    if (pWVar24 != (LPWSTR)0x0) {
      _Var39.v = (uint64_t)(uintptr_t)(pWVar24 + 1);
    }
    iVar52 = lstrlenW((LPCWSTR)(uintptr_t)local_res10.v);
    DVar21 = PECMD_RegSetValueWithOpen((HKEY)((ulonglong)(local_e00 != 0) - 0x7fffffff),
                           (LPCWSTR)(uintptr_t)L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run",(LPCWSTR)(uintptr_t)_Var39.v,1,
                           (BYTE *)local_res10.v,iVar52 * 2);
    local_d88.v = (uint64_t)(int64_t)(int)DVar21;
LAB_140011a14:
    PECMD_FreeStrBuf((WCHAR **)&local_c60.v);
LAB_140011a23:
    if ((local_d70.v != 0) && (local_d70.v != 0xffffffffffffffffULL)) {
      CloseHandle((HANDLE)(uintptr_t)local_d70.v);
    }
    local_d70.ft.dwLowDateTime = 0;
    local_d70.ft.dwHighDateTime = 0;
    if ((local_d80 != (HANDLE)0x0) && (local_d80 != (HANDLE)0xffffffffffffffff)) {
      CloseHandle(local_d80);
    }
    local_d80 = (HANDLE)0x0;
    if ((_Var53.v != 0) && (_Var53.v != 0xffffffffffffffffULL)) {
      CloseHandle((HANDLE)(uintptr_t)_Var53.v);
    }
    if ((_Var59.v != 0) && (_Var59.v != 0xffffffffffffffffULL)) {
      CloseHandle((HANDLE)(uintptr_t)_Var59.v);
    }
    if ((pvVar32 != (HANDLE)0x0) && (pvVar32 != (HANDLE)0xffffffffffffffff)) {
      CloseHandle(pvVar32);
    }
    PECMD_FreeStrBuf(&local_d30.QuadPart);
    PECMD_FreeStrBuf((WCHAR **)&local_a68);
    PECMD_FreeStrBuf((WCHAR **)&local_ca8);
    PECMD_FreeStrBuf((WCHAR **)&local_d10.v);
    PECMD_FreeStrBuf((WCHAR **)&local_c78.v);
    PECMD_FreeStrBuf((WCHAR **)(local_ca8 + 0x10));
    PECMD_FreeStrBuf((WCHAR **)&local_c68);
    PECMD_FreeStrBuf((WCHAR **)&local_c58);
    PECMD_FreeStrBuf((WCHAR **)&local_cf8.v);
    PECMD_ReleaseObjectResources(&local_a80);
    PECMD_FreeStrBuf(&local_cb0);
    PECMD_FreeStrBuf((WCHAR **)&local_d20);
    PECMD_FreeStrBuf((WCHAR **)&local_d38);
    PECMD_FreeStrBuf((WCHAR **)&local_d58);
    PECMD_FreeStrBuf((WCHAR **)&local_cc0);
    PECMD_FreeStrBuf((WCHAR **)&local_ce0);
    PECMD_FreeStrBuf((WCHAR **)&local_cb8.v);
    PECMD_FreeStrBuf((WCHAR **)&local_cc8.v);
    _Var39.v = local_d88.v;
    goto LAB_14001668b;
  }
  PECMD_SkipLeadingControlChars((long long *)&local_res10.v);
  local_bc8 = (HANDLE)0x0;
  local_c50.ft.dwLowDateTime = 0;
  local_c50.ft.dwHighDateTime = 0;
  if (local_bd8.v != 0) {
    bVar4 = false;
    PECMD_AllocStrSlot((uint16_t **)&local_a88);
    puVar27 = (undefined8 *)PECMD_FindVarValue(param_1,(LPCWSTR)(uintptr_t)local_bd8.v,(longlong *)0x0,-1);
    if ((puVar27 != (undefined8 *)0x0) && (*(short *)*puVar27 != 0)) {
      PECMD_ReplaceStringSlot((int64_t *)&local_a88,puVar27);
      PECMD_StrBldCopyWide((long long *)&local_c50.v,(LPCWSTR)(uintptr_t)local_res10.v);
      uVar23 = PECMD_LoadFileMappingExec(local_a88,(longlong *)&local_c50.v,(longlong *)&local_cf8.v,&local_bc8,
                             local_cc0);
      local_d68.v = local_cf8.v;
      local_res10.v = local_c50.v;
      if ((int)uVar23 == 0) {
        PECMD_FreeStrBuf((WCHAR **)&local_a88);
        goto LAB_140011ce2;
      }
      local_d88.v = (uint64_t)(int64_t)(int)uVar23;
      PECMD_FreeStrBuf((WCHAR **)&local_a88);
      PECMD_FreeStrBuf((WCHAR **)&local_c50.v);
      if ((local_bc8 != (HANDLE)0x0) && (local_bc8 != (HANDLE)0xffffffffffffffff)) {
        CloseHandle(local_bc8);
      }
      PECMD_FreeStrBuf((WCHAR **)&local_c60.v);
      goto LAB_140011a23;
    }
    PECMD_FreeStrBuf((WCHAR **)&local_a88);
    PECMD_FreeStrBuf((WCHAR **)&local_c50.v);
    PECMD_FreeStrBuf((WCHAR **)&local_c60.v);
LAB_140011de0:
    if ((local_d70.v != 0) && (local_d70.v != 0xffffffffffffffffULL)) {
      CloseHandle((HANDLE)(uintptr_t)local_d70.v);
    }
    local_d70.ft.dwLowDateTime = 0;
    local_d70.ft.dwHighDateTime = 0;
    if ((local_d80 != (HANDLE)0x0) && (local_d80 != (HANDLE)0xffffffffffffffff)) {
      CloseHandle(local_d80);
    }
    local_d80 = (HANDLE)0x0;
    if ((_Var53.v != 0) && (_Var53.v != 0xffffffffffffffffULL)) {
      CloseHandle((HANDLE)(uintptr_t)_Var53.v);
    }
    if ((_Var59.v != 0) && (_Var59.v != 0xffffffffffffffffULL)) {
      CloseHandle((HANDLE)(uintptr_t)_Var59.v);
    }
joined_r0x000140011e4d:
    if ((pvVar32 != (HANDLE)0x0) && (pvVar32 != (HANDLE)0xffffffffffffffff)) {
      CloseHandle(pvVar32);
    }
    PECMD_FreeStrBuf(&local_d30.QuadPart);
    PECMD_FreeStrBuf((WCHAR **)&local_a68);
    PECMD_FreeStrBuf((WCHAR **)&local_ca8);
    PECMD_FreeStrBuf((WCHAR **)&local_d10.v);
    PECMD_FreeStrBuf((WCHAR **)&local_c78.v);
    PECMD_FreeStrBuf((WCHAR **)(local_ca8 + 0x10));
    PECMD_FreeStrBuf((WCHAR **)&local_c68);
    PECMD_FreeStrBuf((WCHAR **)&local_c58);
    PECMD_FreeStrBuf((WCHAR **)&local_cf8.v);
    PECMD_ReleaseObjectResources(&local_a80);
    PECMD_FreeStrBuf(&local_cb0);
    PECMD_FreeStrBuf((WCHAR **)&local_d20);
    PECMD_FreeStrBuf((WCHAR **)&local_d38);
    PECMD_FreeStrBuf((WCHAR **)&local_d58);
    PECMD_FreeStrBuf((WCHAR **)&local_cc0);
    PECMD_FreeStrBuf((WCHAR **)&local_ce0);
    PECMD_FreeStrBuf((WCHAR **)&local_cb8.v);
    PECMD_FreeStrBuf((WCHAR **)&local_cc8.v);
    _Var39.ft.dwLowDateTime = 0x80070057;
    _Var39.ft.dwHighDateTime = 0xffffffff;
    goto LAB_14001668b;
  }
LAB_140011ce2:
  pvVar45 = local_bc8;
  if (*local_ce0 != L'\0') {
    local_res10.v = (uint64_t)PECMD_UnquoteString((short *)(uintptr_t)local_res10.v);
    PECMD_StrBldCopyAnsi((longlong *)&local_a90.v," -k:0 ",0xffffffffffffffff);
    PECMD_AppendWideStr((longlong *)&local_a90.v,(LPCWSTR)(uintptr_t)local_res10.v);
    PECMD_AppendWideStr(&local_a90.v,(const uint16_t *)L",");
    PECMD_AppendWideStr((longlong *)&local_a90.v,local_ce0);
    local_d88.v = (uint64_t)PECMD_PinStartMenuTask((int64_t *)param_1,(LPCWSTR)(uintptr_t)local_a90.v);
    p_Var38 = &local_a90.v;
LAB_140011d86:
    PECMD_FreeStrBuf((WCHAR **)&p_Var38);
    PECMD_FreeStrBuf((WCHAR **)&local_c50.v);
LAB_140011d98:
    if ((pvVar45 != (HANDLE)0x0) && (pvVar45 != (HANDLE)0xffffffffffffffff)) {
      CloseHandle(pvVar45);
    }
    goto LAB_140011a14;
  }
  if (bVar6) {
    PECMD_AllocStrSlot((uint16_t **)&local_b30.v);
    local_dd8[0] = 0;
    local_res10.v = (uint64_t)PECMD_UnquoteString((short *)(uintptr_t)local_res10.v);
    PECMD_AllocStrSlot((uint16_t **)&local_b28);
    local_b90.ft.dwLowDateTime = 0;
    local_b90.ft.dwHighDateTime = 0;
    PECMD_ExpandPathAlloc2((LPCWSTR)(uintptr_t)local_res10.v,(uint64_t *)&local_b28,(longlong *)&local_b90.v);
    if (local_b90.v != 0) {
      *(WCHAR *)((uintptr_t)local_b90.v - 2) = L'\0';
      local_c10.v = (uint64_t)(uintptr_t)CreatePopupMenu();
      PECMD_StrDupAssign((WCHAR **)&local_a18.v,local_b28);
      local_d88.v = local_a18.v;
      PECMD_StrDupAssign((WCHAR **)&local_a28,(LPCWSTR)(uintptr_t)local_b90.v);
      PECMD_EnumComMenuItems(local_d88.v,(uint64_t)(uintptr_t)local_a28,(int *)local_dd8,(longlong **)&local_b30.v,(HMENU)(uintptr_t)local_c10.v);
      if (0 < (int)local_dd8[0]) {
        if (param_1[8] == 0) {
          PECMD_ResetScriptChain((longlong *)param_1,(longlong *)0);
        }
        local_b80.ft.dwLowDateTime = 0;
        local_b80.ft.dwHighDateTime = 0;
        if (param_1[8] != 0) {
          local_b80.v = *(uint64_t *)((uintptr_t)param_1[8] + 0x20);
        }
        SetForegroundWindow((HWND)local_b80.v);
        local_cfc = TrackPopupMenu((HMENU)local_c10.v,0x128,local_b38.x,local_b38.y,0,(HWND)local_b80.v,
                                   (RECT *)0x0);
        PostMessageW((HWND)local_b80.v,0,0,0);
      }
      DestroyMenu((HMENU)local_c10.v);
      uVar35 = local_cfc - 0x100;
      if ((-1 < (int)uVar35) && ((int)uVar35 < (int)local_dd8[0])) {
        if (0 < (int)uVar35) {
          uVar25 = (ulonglong)uVar35;
          do {
            for (; ((WVar62 = *(WCHAR *)(uintptr_t)local_b30.v, WVar62 != L'\0' && (WVar62 != L'\r')) &&
                   (WVar62 != L'\n')); local_b30.v = local_b30.v + 2) {
            }
            if ((*(WCHAR *)(uintptr_t)local_b30.v == L'\n') || (*(WCHAR *)(uintptr_t)local_b30.v == L'\r')) {
              local_b30.v = local_b30.v + 2;
            }
            if ((*(WCHAR *)(uintptr_t)local_b30.v == L'\n') || (*(WCHAR *)(uintptr_t)local_b30.v == L'\r')) {
              local_b30.v = local_b30.v + 2;
            }
            uVar25 = uVar25 - 1;
            pvVar32 = local_cf0;
          } while (uVar25 != 0);
        }
        WVar62 = *(WCHAR *)(uintptr_t)local_b30.v;
        _Var39.v = local_b30.v;
        while (((WVar62 != L'\0' && (_Var59.v = local_c88.v, WVar62 != L'\r')) && (WVar62 != L'\n'))) {
          _Var39.v = _Var39.v + 2;
          WVar62 = *(WCHAR *)(uintptr_t)_Var39.v;
        }
        *(WCHAR *)(uintptr_t)_Var39.v = L'\0';
        local_d88.v = local_b30.v;
        if (*(WCHAR *)(uintptr_t)local_b30.v != L'\0') {
          PECMD_StrBldCopyAnsi((longlong *)&local_cd8," -k:0 ",0xffffffffffffffff);
          PECMD_AppendWideStr((longlong *)&local_cd8,(LPCWSTR)(uintptr_t)local_res10.v);
          PECMD_AppendWideStr(&local_cd8,(const uint16_t *)L",");
          PECMD_AppendWideStr((longlong *)&local_cd8,(LPCWSTR)(uintptr_t)local_d88.v);
          local_d88.v = (uint64_t)PECMD_PinStartMenuTask((int64_t *)param_1,local_cd8);
          PECMD_FreeStrBuf((WCHAR **)&local_cd8);
          PECMD_FreeStrBuf(&local_a28);
          PECMD_FreeStrBuf((WCHAR **)&local_a18.v);
          PECMD_FreeStrBuf((WCHAR **)&local_b28);
          p_Var38 = &local_b30.v;
          goto LAB_140011d86;
        }
      }
      PECMD_FreeStrBuf(&local_a28);
      PECMD_FreeStrBuf((WCHAR **)&local_a18.v);
      PECMD_FreeStrBuf((WCHAR **)&local_b28);
      PECMD_FreeStrBuf((WCHAR **)&local_b30.v);
      PECMD_FreeStrBuf((WCHAR **)&local_c50.v);
      if ((pvVar45 != (HANDLE)0x0) && (pvVar45 != (HANDLE)0xffffffffffffffff)) {
        CloseHandle(pvVar45);
      }
      PECMD_FreeStrBuf((WCHAR **)&local_c60.v);
      _Var39.ft.dwLowDateTime = 0;
      _Var39.ft.dwHighDateTime = 0;
      if ((local_d70.v != 0) && (local_d70.v != 0xffffffffffffffffULL)) {
        CloseHandle((HANDLE)(uintptr_t)local_d70.v);
      }
      local_d70.ft.dwLowDateTime = 0;
      local_d70.ft.dwHighDateTime = 0;
      if ((local_d80 != (HANDLE)0x0) && (local_d80 != (HANDLE)0xffffffffffffffff)) {
        CloseHandle(local_d80);
      }
      local_d80 = (HANDLE)0x0;
      if ((_Var53.v != 0) && (_Var53.v != 0xffffffffffffffffULL)) {
        CloseHandle((HANDLE)(uintptr_t)_Var53.v);
      }
      if ((_Var59.v != 0) && (_Var59.v != 0xffffffffffffffffULL)) {
        CloseHandle((HANDLE)(uintptr_t)_Var59.v);
      }
      if ((pvVar32 != (HANDLE)0x0) && (pvVar32 != (HANDLE)0xffffffffffffffff)) {
        CloseHandle(pvVar32);
      }
      PECMD_FreeStrBuf(&local_d30.QuadPart);
      PECMD_FreeStrBuf((WCHAR **)&local_a68);
      PECMD_FreeStrBuf((WCHAR **)&local_ca8);
      PECMD_FreeStrBuf((WCHAR **)&local_d10.v);
      PECMD_FreeStrBuf((WCHAR **)&local_c78.v);
      PECMD_FreeStrBuf((WCHAR **)(local_ca8 + 0x10));
      PECMD_FreeStrBuf((WCHAR **)&local_c68);
      PECMD_FreeStrBuf((WCHAR **)&local_c58);
      PECMD_FreeStrBuf((WCHAR **)&local_cf8.v);
      PECMD_ReleaseObjectResources(&local_a80);
      PECMD_FreeStrBuf(&local_cb0);
      PECMD_FreeStrBuf((WCHAR **)&local_d20);
      PECMD_FreeStrBuf((WCHAR **)&local_d38);
      PECMD_FreeStrBuf((WCHAR **)&local_d58);
      PECMD_FreeStrBuf((WCHAR **)&local_cc0);
      PECMD_FreeStrBuf((WCHAR **)&local_ce0);
      PECMD_FreeStrBuf((WCHAR **)&local_cb8.v);
      PECMD_FreeStrBuf((WCHAR **)&local_cc8.v);
      goto LAB_14001668b;
    }
    PECMD_FreeStrBuf((WCHAR **)&local_b28);
    PECMD_FreeStrBuf((WCHAR **)&local_b30.v);
    PECMD_FreeStrBuf((WCHAR **)&local_c50.v);
    if ((pvVar45 != (HANDLE)0x0) && (pvVar45 != (HANDLE)0xffffffffffffffff)) {
      CloseHandle(pvVar45);
    }
    PECMD_FreeStrBuf((WCHAR **)&local_c60.v);
    goto LAB_140011de0;
  }
  /* ---- SEG6 (decompiled 2384-2520): cmd/cmd.exe 探测与 .cmd/.bat 预读 ---- */
  PECMD_StrDupAssign((uint16_t **)&local_b88.v,(LPCWSTR)(uintptr_t)local_res10.v);
  if (local_d9c == '\0') {
    if ((local_d9b != '\0') && (local_dfc != 0x10000000)) {
      local_c30.ft.dwLowDateTime = 0;
      local_c30.ft.dwHighDateTime = 0;
      local_res10.v = local_b88.v;
      local_cd0 = PECMD_NextToken((int64_t *)&local_res10.v,(int64_t *)0x0,0);
      local_c30.v = local_res10.v;
      cVar15 = PECMD_MatchTokenAdvance("cmd",(long long *)&local_c30.v,3);
      if ((cVar15 == '\0') &&
          (cVar15 = PECMD_MatchTokenAdvance("cmd.exe",(long long *)&local_c30.v,7), cVar15 == '\0')) {
        if (DAT_14013ca90 == 0) {
          local_238[0] = 0;
          SHGetSpecialFolderPathW((HWND)0x0,local_238,0x24,0);
          PECMD_StrBldCopyWide((long long *)(void *)&DAT_14013ca90,local_238);
          PECMD_AppendWideStr(&DAT_14013ca90,(LPCWSTR)L"\\System32\\cmd.exe");
          iVar52 = lstrlenW((LPCWSTR)(uintptr_t)DAT_14013ca90);
          DAT_14013ca4c = (short)iVar52;
        }
        local_de0 = 0;
        iVar52 = StrCmpNIW((LPCWSTR)(uintptr_t)DAT_14013ca90,(LPCWSTR)(uintptr_t)local_c30.v,
                           (int)DAT_14013ca4c);
        if ((iVar52 == 0) &&
            ((_Var39.v = local_c30.v + (uint64_t)((int64_t)DAT_14013ca4c * 2),
             *(WCHAR *)(uintptr_t)_Var39.v == L'\"' ||
             (((8 < (ushort)*(WCHAR *)(uintptr_t)_Var39.v && ((ushort)*(WCHAR *)(uintptr_t)_Var39.v < 0xe)) ||
              (*(WCHAR *)(uintptr_t)_Var39.v == L' ')))))) {
          local_c30.v = _Var39.v;
          PECMD_SkipTokenWs((int64_t *)&local_c30.v);
          goto LAB_14001270f;
        }
      }
      else {
        local_de0 = param_5;
LAB_14001270f:
        cVar15 = PECMD_MatchTokenAdvance("/c",(long long *)&local_c30.v,2);
        if (cVar15 != '\0') {
          if (*(WCHAR *)(uintptr_t)local_c30.v == L'\"') {
            local_a20.v = local_c30.v + 2;
            PECMD_SkipLeadingControlChars((long long *)&local_a20.v);
            if (*(WCHAR *)(uintptr_t)local_a20.v != L'\"') {
              cVar15 = PECMD_MatchTokenAdvance("call",(long long *)&local_a20.v,(uint)local_da8);
              if (cVar15 == '\0') goto LAB_14001279e;
            }
            local_c30.v = local_a20.v;
          }
LAB_14001279e:
          local_res10.v = local_c30.v;
          local_cd0 = PECMD_NextToken((int64_t *)&local_res10.v,(int64_t *)0x0,0);
          local_de0 = 1;
        }
      }
      if ((int)local_da8 < (int)((longlong)local_cd0 - (longlong)local_res10.v >> 1)) {
        iVar52 = StrCmpNIW(local_cd0 + -4,(LPCWSTR)L".cmd",(int)local_da8);
        if (iVar52 != 0) {
          iVar52 = StrCmpNIW(local_cd0 + -4,(LPCWSTR)L".bat",(uint)local_da8);
          if (iVar52 != 0) goto LAB_14001253a;
        }
        WVar62 = *local_cd0;
        *(uint16_t *)(uintptr_t)local_cd0 = 0;
        FUN_140102a90((ulonglong *)local_4a8,0,0x191);
        local_bb0 = local_4a8;
        local_d50 = local_d50 & 0xffffff00;
        local_c08 = (LPWCH)0x0;
        PECMD_OpenFileHandle((HANDLE *)(void *)&local_c08,(LPCWSTR)(uintptr_t)local_res10.v,
                             0xc0000000,7,(LPSECURITY_ATTRIBUTES)0x0,3,0x80,(HANDLE)0x0);
        pWVar44 = local_c08;
        ReadFile(local_c08,local_4a8,400,(LPDWORD)&local_be8,(LPOVERLAPPED)0x0);
        iVar52 = StrCmpNIA("::hidecmd::",(LPCSTR)local_4a8,0xb);
        if (iVar52 == 0) {
          local_dfc = 0x10000000;
          pWVar28 = local_bb0;
          if ((char)local_4a8[0] != '\0') {
            pWVar28 = local_4a8;
            do {
              local_bb0 = pWVar28;
              if (((char)local_4a8[0] == '\r') || ((char)local_4a8[0] == '\n')) break;
              pWVar28 = (LPWCH)((uintptr_t)pWVar28 + 1);
              *(char *)local_4a8 = *(char *)pWVar28;   /* 原 local_4a8._0_1_ 字节片 */
              local_bb0 = pWVar28;
            } while ((char)local_4a8[0] != '\0');
          }
          for (; ((char)*pWVar28 == '\n' || ((char)*pWVar28 == '\r'));
              pWVar28 = (LPWCH)((uintptr_t)pWVar28 + 1)) {
          }
        }
        else {
          pWVar28 = local_4a8;
        }
        if (local_de0 == 0) {
          if (((char)*pWVar28 == ':') && (*(char *)((uintptr_t)pWVar28 + 1) == ':')) {
            pWVar28 = pWVar28 + 1;
          }
          cVar15 = (char)*pWVar28;
          while (cVar15 == '@') {
            pWVar28 = (LPWCH)((uintptr_t)pWVar28 + 1);
            cVar15 = *(char *)pWVar28;
          }
          iVar52 = StrCmpNIA("#!pecmd",(LPCSTR)pWVar28,7);
          local_d50 = local_d50 & 0xff;
          if (iVar52 == 0) {
            local_d50 = 1;
          }
        }
        if ((pWVar44 != (LPWCH)0x0) && (pWVar44 != (LPWCH)0xffffffffffffffff)) {
          CloseHandle(pWVar44);
        }
        *(uint16_t *)(uintptr_t)local_cd0 = WVar62;
        if ((char)local_d50 != '\0') {
          local_d88.v = PECMD_RunCommand(param_1,(WCHAR *)(uintptr_t)local_res10.v);
          PECMD_FreeStrBuf((WCHAR **)&local_b88.v);
          PECMD_FreeStrBuf((WCHAR **)&local_c50.v);
          pvVar45 = local_bc8;
          goto LAB_140011d98;
        }
      }
    }
  }
  else {
    PECMD_StrBldCopyWide((long long *)&local_b88.v,(const uint16_t *)L"PECMD**pecmd-cmd* ");
    local_res10.v = (uint64_t)(uintptr_t)PECMD_NextToken((int64_t *)&local_res10.v,(int64_t *)0x0,1);
    PECMD_AppendWideStr(&local_b88.v,(LPCWSTR)(uintptr_t)local_res10.v);
    local_d68.v = PECMD_GetModuleFileNameIntoContainer(&local_cf8.v);
  }
LAB_14001253a:
  bVar6 = false;
  local_res10.v = local_b88.v;
  if ((local_dfc == 0x10000000) || (local_d4c != '\0')) {
    local_888.dwFlags = local_888.dwFlags | 0x80;
  }
  if (local_dfc != 0) {
    local_888.dwFlags = local_888.dwFlags | 1;
    local_888.wShowWindow = (ushort)local_dfc;
  }
  if (((local_dbf & 3) == 3) || (bVar16)) {
    local_888.dwFlags = local_888.dwFlags | 0x8000000;
  }
  pvVar45 = local_bc8;
  /* ---- SEG7 (decompiled 2521-2800): 环境块构建 / 命名管道 / ShellExecuteEx 路径 ---- */
  if ((longlong)local_a98 < 1) {
    if ((local_888.lpDesktop == (LPWSTR)0x0) || (bVar16 = true, (char)local_dbf < '\x01')) {
      bVar16 = false;
    }
    local_df0 = 0;
    if (bVar16) {
      local_df0 = (uint)local_da8;
    }
    local_de0 = (uint)bVar16;
    if (local_d44 != '\0') {
      local_df0 = local_df0 | 0x10;
    }
    bVar14 = false;
    if ((bVar16 != false) && ((byte)((byte)local_da8 & (byte)g_privFlags) == 0)) {
      g_privFlags = g_privFlags | (ushort)local_da8;
      bVar6 = true;
      bVar14 = true;
      PECMD_EnableTokenPrivilege((LPCWSTR)L"SeDebugPrivilege",2,0);
    }
    local_c08 = (LPWCH)&DAT_14013d130;
    if ((g_flagCCC9 == '\0') &&
        ((local_res18 == (longlong *)0x0 || (*(char *)((uintptr_t)local_res18 + 0xc) == '\0')))) {
      bVar16 = false;
      if ((local_d43 != '\0') && (local_res18 != (longlong *)0x0)) {
        local_c08 = (LPWCH)local_res18[7];
        goto LAB_140012b66;
      }
    }
    else {
LAB_140012b66:
      bVar16 = true;
    }
    PECMD_StrDupAssign((uint16_t **)&local_a30.v,(LPCWSTR)0x0);
    local_dc8.ft.dwLowDateTime = 0;
    local_dc8.ft.dwHighDateTime = 0;
    if (bVar16) {
      EnterCriticalSection((void *)&DAT_14013e190);
      PECMD_InitObjectWithParent(&local_6b8,0);
      if (local_res18 != (longlong *)0x0) {
        PECMD_ExpandScriptVariables(&local_6b8,local_res18,(longlong *)(uintptr_t)local_c08);
      }
      local_c08 = (LPWSTR)(uintptr_t)GetEnvironmentStringsW();   /* 桩返回 uint64_t, 仅作指针 */
      PECMD_AllocWStringBuffer((WCHAR **)&local_928,0x106);
      WVar62 = *local_c08;
      pWVar44 = local_c08;
      pWVar42 = local_a08;
      while (local_a08 = pWVar44, WVar62 != L'\0') {
        pWVar42 = local_a08 + 1;
        WVar62 = *pWVar42;
        while ((WVar62 != L'\0' && (WVar62 != L'='))) {
          pWVar42 = pWVar42 + 1;
          WVar62 = *pWVar42;
        }
        local_b60 = (longlong)pWVar42 - (longlong)local_a08 >> 1;
        local_cd8 = pWVar42;
        if (*pWVar42 != L'\0') {
          local_a00 = pWVar42 + 1;
          if ((*local_a00 != L'\0') && ((int)local_b60 < 0x105)) {
            local_d88.v = (uint64_t)((longlong)(int)local_b60 * 2);
            PECMD_MemMoveForward((uint8_t *)(uintptr_t)local_928,(uint8_t *)(uintptr_t)local_a08,
                                 (int)local_d88.ft.dwLowDateTime);
            *(uint16_t *)(uintptr_t)(local_d88.v + (uintptr_t)local_928) = 0;
            lVar61 = (int64_t)PECMD_VarLookup(&local_6b8,local_928,(int64_t *)0x0,-1,(void *)0x0);
            pWVar42 = local_cd8;
            if (lVar61 == 0) {
              PECMD_AddVarDefault(&local_6b8,local_a08,local_a00,(int)local_b60,-1);
              pWVar42 = local_cd8;
            }
          }
          for (; *pWVar42 != L'\0'; pWVar42 = pWVar42 + 1) {
          }
        }
        WVar62 = pWVar42[1];
        pvVar32 = local_cf0;
        pWVar44 = pWVar42 + 1;
        pWVar42 = local_a08;
        bVar6 = bVar14;
      }
      local_a08 = pWVar42;
      FreeEnvironmentStringsW(local_c08);
      pWVar42 = (WCHAR *)(longlong)local_6b0;
      local_df8 = 0;
      local_bb0 = (LPWCH)0x0;
      local_cd8 = pWVar42;
      if (0 < (longlong)pWVar42) {
        do {
          pWVar44 = (LPWCH)((uintptr_t)local_bb0 + 1);
          local_c08 = pWVar44;
          if ((longlong)pWVar44 < (longlong)pWVar42) {
            _Var39.v = (uint64_t)((longlong)local_bb0 << 3);
            pWVar28 = local_bb0;
            local_d88.v = _Var39.v;
            do {
              iVar52 = lstrcmpW(
                 (LPCWSTR)*(uint64_t *)(uintptr_t)*(uint64_t *)(uintptr_t)
                    (local_6b8 + (longlong)pWVar44 * 8),
                 (LPCWSTR)*(uint64_t *)(uintptr_t)*(uint64_t *)(uintptr_t)
                    (local_6b8 + (uintptr_t)_Var39.v));
              if (iVar52 < 0) {
                _Var39.v = (uint64_t)((longlong)pWVar44 << 3);
                pWVar28 = pWVar44;
              }
              pWVar44 = (LPWCH)((uintptr_t)pWVar44 + 1);
            } while ((longlong)pWVar44 < (longlong)pWVar42);
            if (pWVar28 != local_bb0) {
              uVar23 = *(uint64_t *)(uintptr_t)(local_d88.v + local_6b8);
              *(uint64_t *)(uintptr_t)(local_d88.v + local_6b8) =
                   *(uint64_t *)(uintptr_t)(local_6b8 + (longlong)pWVar28 * 8);
              *(uint64_t *)(uintptr_t)(local_6b8 + (longlong)pWVar28 * 8) = uVar23;
            }
          }
          pWVar8 = local_cd8;
          local_bb0 = local_c08;
        } while ((longlong)local_c08 < (longlong)pWVar42);
        lVar61 = 0;
        _Var53.v = local_c20.v;
        pvVar32 = local_cf0;
        _Var59.v = local_c88.v;
        bVar6 = bVar14;
        if (0 < (longlong)local_cd8) {
          do {
            iVar52 = lstrlenW(
               (LPCWSTR)*(uint64_t *)(uintptr_t)*(uint64_t *)(uintptr_t)
                  (local_6b8 + lVar61 * 8));
            local_cfc = local_df8 + 1 + iVar52;
            iVar52 = lstrlenW((LPCWSTR)*(uint64_t *)(uintptr_t)
                                (*(int64_t *)(uintptr_t)(local_6b8 + lVar61 * 8) + 8));
            lVar61 = lVar61 + 1;
            local_df8 = local_cfc + 3 + iVar52;
            _Var53.v = local_c20.v;
            pvVar32 = local_cf0;
            _Var59.v = local_c88.v;
          } while (lVar61 < (longlong)pWVar8);
        }
      }
      PECMD_AllocString(&local_a30.v,(longlong)(int)(local_df8 + 4));
      pWVar42 = local_cd8;
      local_dc8.v = local_a30.v;
      lVar61 = 0;
      if (0 < (longlong)local_cd8) {
        do {
          local_d88.v = *(uint64_t *)(uintptr_t)*(uint64_t *)(uintptr_t)
                          (local_6b8 + lVar61 * 8);
          iVar52 = lstrlenW((LPCWSTR)(uintptr_t)local_d88.v);
          local_b60 = (longlong)iVar52 * 2;
          PECMD_MemMoveForward((uint8_t *)(uintptr_t)local_a30.v,(uint8_t *)(uintptr_t)local_d88.v,
                               (int)local_b60);
          *(uint16_t *)(uintptr_t)(local_a30.v + local_b60) = 0x3d;
          puVar50 = (uint16_t *)(uintptr_t)(local_a30.v + local_b60) + 1;
          local_d88.v = *(uint64_t *)(uintptr_t)
                          (*(int64_t *)(uintptr_t)(local_6b8 + lVar61 * 8) + 8);
          iVar52 = lstrlenW((LPCWSTR)(uintptr_t)local_d88.v);
          local_b60 = (longlong)iVar52 * 2;
          PECMD_MemMoveForward((uint8_t *)(uintptr_t)puVar50,(uint8_t *)(uintptr_t)local_d88.v,
                               (int)local_b60);
          puVar50 = (uint16_t *)((uintptr_t)puVar50 + local_b60);
          lVar61 = lVar61 + 1;
          *puVar50 = 0;
          local_a30.v = (uint64_t)(uintptr_t)(puVar50 + 1);
          _Var53.v = local_c20.v;
          pvVar32 = local_cf0;
          _Var59.v = local_c88.v;
          local_b90.v = local_a30.v;
          bVar6 = bVar14;
        } while (lVar61 < (longlong)pWVar42);
      }
      *(WCHAR *)(uintptr_t)local_a30.v = L'\0';
      *(WCHAR *)((uintptr_t)local_a30.v + 2) = L'\0';
      PECMD_FreeStrBuf((WCHAR **)&local_928);
      PECMD_ClearTaskTable((uint64_t)(uintptr_t)&local_6b8,0);
      PECMD_FreeStrBuf(local_648);
      LeaveCriticalSection((void *)&DAT_14013e190);
    }
    if (*local_d58 == L'@') {
      cVar15 = '\x01';
    }
    else {
      cVar15 = '\0';
      if (local_c48 != (LPCWSTR)0x0) {
        cVar15 = '\x02';
      }
    }
    if ((short)local_ddc != 0) {
      local_e00 = local_e00 | 1;
      local_ddc = local_ddc | 8;
      local_cfc = (-(uint)(cVar15 != '\0') & 0xfff01000) + 0x100000;
      lVar61 = PECMD_NextRandomSeed();
      wsprintfA(local_738,"\\\\.\\pipe\\pecmd_exec_%u",lVar61);
      local_bf0 = CreateNamedPipeA(local_738,0x40000003,6,0xff,local_cfc,local_cfc,1,
                                   (LPSECURITY_ATTRIBUTES)0x0);
      if ((local_bf0 == (HANDLE)0x0) || (local_bf0 == (HANDLE)0xffffffffffffffff)) {
        Sleep(1);
      }
      else {
        plVar29 = PECMD_StrBldCopyAnsi((longlong *)&local_b68,local_738,0xffffffffffffffff);
        PECMD_OpenFileHandle((HANDLE *)&local_be0.v,(LPCWSTR)*plVar29,0xc0000000,7,&local_910,3,0,
                             (HANDLE)0x0);
        PECMD_FreeStrBuf((WCHAR **)&local_b68);
      }
      local_9e8[0] = 2;
      SetNamedPipeHandleState((HANDLE)(uintptr_t)local_be0.v,local_9e8,(LPDWORD)0x0,(LPDWORD)0x0);
      local_970[0] = CreateEventW((LPSECURITY_ATTRIBUTES)0x0,0,1,(LPCWSTR)0x0);
      local_d08 = 1;
      local_888.dwFlags = local_888.dwFlags | 0x100;
      local_de8 = 1;
      if (((local_de4 & 2) != 0) || (local_de4 == 0)) {
        local_888.hStdOutput = (HANDLE)(uintptr_t)local_be0.v;
      }
      local_8f0.hEvent = local_970[0];
      if ((local_de4 & 1) != 0) {
        local_888.hStdError = (HANDLE)(uintptr_t)local_be0.v;
      }
    }
    _Var39.v = local_dd0.v;
    bVar16 = true;
    local_cd0 = (LPCWSTR)0x0;
    *(uint32_t *)&local_ca8[8] = 0;      /* 原 local_ca8._8_4_ 字节片 */
    *(uint32_t *)&local_ca8[12] = 0;     /* 原 local_ca8._12_4_ 字节片 */
    local_c38.ft.dwLowDateTime = 0;
    local_c38.ft.dwHighDateTime = 0;
    local_df0 = local_df0 | -(uint)(local_dc8.v != 0) & 0x400 | local_d60;
    if (bVar5) {
      local_df0 = local_df0 | 0x1000004;
    }
    if ((bVar4) && (local_d68.v == 0)) {
LAB_140014ade:
      local_7a8.cbSize = 0x70;
      FUN_140102a90((ulonglong *)&local_7a8.hwnd,0,0x68);
      local_7a8.fMask = 0x4540;
      uVar34 = 5;
      if (local_dfc != 0) {
        uVar34 = (ushort)local_dfc;
      }
      local_7a8.nShow = (int)uVar34;
      if ((_Var39.v != 0) && (*(short *)(uintptr_t)_Var39.v != 0)) {
        local_7a8.lpDirectory = (void *)(uintptr_t)_Var39.v;
      }
      local_7a8.lpVerb = (LPCWSTR)*(uint64_t *)&local_ca8[16];   /* 原 local_ca8._16_8_ 字节片 */
      local_960.v = local_res10.v;
      local_7a8.lpFile = (LPCWSTR)(uintptr_t)local_res10.v;
      local_950 = 0;
      pWVar30 = (LPCWSTR)PECMD_NextToken((longlong *)&local_960.v,&local_950,1);
      if ((*pWVar30 != L'\0') || (local_960.v != local_res10.v)) {
        PECMD_StrCopyW((WCHAR **)&local_c78.v,(LPCWSTR)(uintptr_t)local_960.v,local_950);
        local_7a8.lpFile = (LPCWSTR)(uintptr_t)local_c78.v;
      }
      if (*pWVar30 != L'\0') {
        local_7a8.lpParameters = pWVar30;
      }
      PECMD_GetApiProcCached("GetProcessID","Kernel32.DLL",(longlong *)&DAT_14013c960,(longlong *)0x0);
      pcVar10 = (void *)(uintptr_t)DAT_14013c960;
      local_df4 = ShellExecuteExW(&local_7a8);
      local_af8.hProcess = local_7a8.hProcess;
      local_7a8.hProcess = (HANDLE)0x0;
      if (pcVar10 != (void *)0x0) {
        uVar22 = ((uint32_t (*)(void))pcVar10)();
        local_af8.dwProcessId = uVar22;
      }
LAB_140014c72:
      local_df8 = 0;
      if (local_df4 == 0) {
        local_df8 = GetLastError();
      }
      *(uint32_t *)(void *)local_990 = local_df8;
    }
    else {
      sVar51 = (short)local_dd8[0];
      sVar49 = (short)local_db8;
      if (((char)local_d78 == '\0') || ((sVar51 != 0 || (sVar49 != 0)))) {
        if ((*local_d38 != L'\0') && ((sVar51 == 0 && (sVar49 == 0)))) {
          local_df4 = 0;
          if (local_d9d != '\0') {
            local_9b8 = (byte *)0x0;
            PECMD_WideToAnsiStr((longlong *)&local_9b8,local_d38,-1,(uint64_t)0xffffffffffffffff);
            PECMD_AllocSmallObject((void **)&local_9f8);
            PECMD_ZeroLenBuf((void *)(uintptr_t)local_d38);
            lVar61 = PECMD_Base64Decode(local_9b8,(uint64_t *)(void *)&local_9f8);
            bVar16 = lVar61 != 0;
            PECMD_StrDupA((WCHAR **)(void *)&local_d38,(LPCWSTR)local_9f8,
                          0xffffffffffffffff,0xffffffffffffffff);
            PECMD_FreeContainer((longlong *)(void *)&local_9f8);
            PECMD_FreeContainer((longlong *)(void *)&local_9b8);
          }
          if (local_e06 != '\0') {
            local_c00 = (HANDLE *)0x0;
            PECMD_WideToAnsiStr((longlong *)&local_c00,local_d20,-1,(uint64_t)0xffffffffffffffff);
            PECMD_AllocSmallObject((void **)&local_998);
            PECMD_ZeroLenBuf((void *)(uintptr_t)local_d20);
            if (bVar16) {
              lVar61 = PECMD_Base64Decode((byte *)local_c00,(uint64_t *)(void *)&local_998);
              bVar16 = true;
              if (lVar61 == 0) goto LAB_140013555;
            }
            else {
LAB_140013555:
              bVar16 = false;
            }
            PECMD_StrDupA((WCHAR **)(void *)&local_d20,(LPCWSTR)local_998,
                          0xffffffffffffffff,0xffffffffffffffff);
            PECMD_FreeContainer((longlong *)(void *)&local_998);
            PECMD_FreeContainer((longlong *)(void *)&local_c00);
          }
          pWVar24 = local_d38;
          if (bVar16) {
            pWVar31 = StrChrW(local_d38,L'@');
            if ((pWVar31 == (LPWSTR)0x0) &&
                (pWVar31 = StrChrW(pWVar24,L'\\'), pWVar31 == (LPWSTR)0x0)) {
              pWVar30 = (LPCWSTR)L".";
            }
            else {
              *pWVar31 = 0;
              pWVar24 = pWVar31 + 1;
              pWVar30 = local_d38;
            }
            lpCurrentDirectory =
                 (LPCWSTR)PECMD_ResolveWildcardPath((uint64_t *)(void *)&local_cd0,
                              (short *)(uintptr_t)local_dd0.v,(LPWSTR)(uintptr_t)local_d68.v,
                              (LPCWSTR)(uintptr_t)local_res10.v);
            bVar16 = PECMD_IsFile(lpCurrentDirectory);
            if (bVar16 != false) goto LAB_1400132d5;   /* 原 CONCAT71(extraout_var,bVar16) */
            if (local_d98[0] == 0) {
              local_df4 = CreateProcessWithLogonW
                                    (pWVar24,pWVar30,local_d20,local_db4 | 1,(LPCWSTR)(uintptr_t)local_d68.v,
                                     (LPWSTR)(uintptr_t)local_res10.v,local_df0,(LPVOID)(uintptr_t)local_dc8.v,
                                     lpCurrentDirectory,&local_888,&local_af8);
            }
            else {
              local_df4 = PECMD_CreateProcessAsUser((LPCWSTR)(uintptr_t)local_d68.v,(LPWSTR)(uintptr_t)local_res10.v,
                                        (LPSECURITY_ATTRIBUTES)0x0,(LPSECURITY_ATTRIBUTES)0x0,0,
                                        local_db4 | 1,(LPVOID)(uintptr_t)local_dc8.v,lpCurrentDirectory,
                                        &local_888,(uint64_t)(uintptr_t)&local_af8,local_d98[0] & 0xffff,
                                        pWVar24,pWVar30,local_d20);
            }
          }
          PECMD_ZeroLenBuf((void *)(uintptr_t)local_d20);
          PECMD_ZeroLenBuf((void *)(uintptr_t)local_d38);
          goto LAB_140014c72;
        }
        if (((char)local_d40 != '\0') &&
           ((((local_d68.v == 0 || (*(short *)(uintptr_t)local_d68.v != 0x26)) &&
             ((WCHAR)local_db0 == 0)) && ((sVar51 == 0 && (sVar49 == 0)))))) {
          bVar16 = PECMD_IsFile((LPCWSTR)(uintptr_t)local_dd0.v);
          if (bVar16 != false) goto LAB_1400132d5;     /* 原 CONCAT71(extraout_var,bVar16) */
          iVar52 = PECMD_CreateInjectedProcess(local_a68,(uint)local_a60,(WCHAR *)(uintptr_t)local_res10.v,local_de8,
                                 local_df0,(LPVOID)(uintptr_t)local_dc8.v,(LPCWSTR)(uintptr_t)_Var39.v,
                                 &local_888,&local_af8,(LPCWSTR)(uintptr_t)local_c00);
          local_df4 = (uint)(0 < iVar52);
          goto LAB_140014c72;
        }
        local_be8 = (LPWSTR)0x0;
        local_df4 = 0;
        local_b78 = (LPCWSTR)0x0;
        local_bb8 = 0;
        *(uint32_t *)&local_ca8[24] = local_d68.ft.dwLowDateTime;   /* 原 _24_4_ 字节片 */
        *(uint32_t *)&local_ca8[28] = local_d68.ft.dwHighDateTime;  /* 原 _28_4_ 字节片 */
        local_b58.v = local_d68.v;
        PECMD_AllocSmallObject((void **)&local_ba8.v);
        local_c00 = &local_9f0;
        local_ba0 = 0;
        local_b98 = 0;
        local_ab8.v = (uint64_t)(uintptr_t)&local_d28.v;
        local_bc0.ft.dwLowDateTime = 0;
        local_bc0.ft.dwHighDateTime = 0;
        local_9f0 = (HANDLE)0x0;
        local_d28.ft.dwLowDateTime = 0;
        local_d28.ft.dwHighDateTime = 0;
        if ((short)local_db8 == 0) {
          if (*(uint64_t *)&local_ca8[24] != 0) {                   /* 原 local_ca8._24_8_ */
            if (*(short *)(uintptr_t)*(uint64_t *)&local_ca8[24] == 0x26) {
              local_b68 = (LPCWSTR)PECMD_VarLookup(param_1,
                                 (LPCWSTR)*(uint64_t *)&local_ca8[24],(int64_t *)0x0,-1,(void *)0x0);
              if (local_b68 == (LPCWSTR)0x0) {
                PECMD_FreeStrBuf((WCHAR **)&local_d28.v);
                PECMD_FreeStrBuf((WCHAR **)&local_ba8.v);
                PECMD_FreeStrBuf(&local_bb8);
                PECMD_FreeStrBuf((WCHAR **)&local_c38.v);
                PECMD_FreeStrBuf((WCHAR **)(local_ca8 + 8));
                PECMD_FreeStrBuf((WCHAR **)&local_cd0);
                PECMD_FreeStrBuf((WCHAR **)&local_a30.v);
                if (bVar6) {
                  g_privFlags = g_privFlags & 0xfffb;
                  PECMD_EnableTokenPrivilege((LPCWSTR)L"SeDebugPrivilege",(uint)local_da8,0);
                }
                PECMD_FreeStrBuf((WCHAR **)&local_b88.v);
                PECMD_FreeStrBuf((WCHAR **)&local_c50.v);
                if ((local_bc8 != (HANDLE)0x0) && (local_bc8 != (HANDLE)0xffffffffffffffff)) {
                  CloseHandle(local_bc8);
                }
                PECMD_FreeStrBuf((WCHAR **)&local_c60.v);
                if ((local_d70.v != 0) && (local_d70.v != 0xffffffffffffffff)) {
                  CloseHandle((HANDLE)(uintptr_t)local_d70.v);
                }
                local_d70.ft.dwLowDateTime = 0;
                local_d70.ft.dwHighDateTime = 0;
                if ((local_d80 != (HANDLE)0x0) && (local_d80 != (HANDLE)0xffffffffffffffff)) {
                  CloseHandle(local_d80);
                }
                local_d80 = (HANDLE)0x0;
                if ((_Var53.v != 0) && (_Var53.v != 0xffffffffffffffff)) {
                  CloseHandle((HANDLE)(uintptr_t)_Var53.v);
                }
                if ((_Var59.v != 0) && (_Var59.v != 0xffffffffffffffff)) {
                  CloseHandle((HANDLE)(uintptr_t)_Var59.v);
                }
                if ((pvVar32 != (HANDLE)0x0) && (pvVar32 != (HANDLE)0xffffffffffffffff)) {
                  CloseHandle(pvVar32);
                }
                pvVar32 = local_bf0;
                if ((local_be0.v != 0) && (local_be0.v != 0xffffffffffffffff)) {
                  CloseHandle((HANDLE)(uintptr_t)local_be0.v);
                  pvVar32 = local_bf0;
                }
                goto joined_r0x000140011e4d;
              }
              local_ba0 = *(uint64_t *)(uintptr_t)
                            ((longlong)(uintptr_t)local_b68 + 0xc) & 0x3fffffffffffffff;
              _Var48.v = (uint64_t)(uintptr_t)
                           PECMD_GrowByteBuffer((void **)&local_ba8.v,local_ba0 + 0x10);
              local_bc0.v = _Var48.v;
              PECMD_MemMoveForward((uint8_t *)(uintptr_t)local_ba8.v,
                             *(uint8_t **)(uintptr_t)((longlong)(uintptr_t)local_b68 + 4),
                             (int)local_ba0 + 1);
              goto LAB_1400141ec;   /* 定义于 SEG9 (raw 3115); 占位见尾部 */
            }
            local_b78 = PECMD_ParseResourceStringRef((LPCWSTR *)&local_b58.v,-(uint)bVar3 & 2);
          }
          DVar21 = 2;
          _Var48.ft.dwLowDateTime = 0;
          _Var48.ft.dwHighDateTime = 0;
          *(uint32_t *)&local_ca8[24] = 0;
          *(uint32_t *)&local_ca8[28] = 0;
          local_8f8 = (LPCWSTR)0x0;
          if (local_b78 != (LPCWSTR)0x0) {
LAB_1400140bc:
            pWVar30 = local_b78;
            if (((short)local_dd8[0] != 0) &&
               (local_be8 = StrRChrW(local_b78,(LPCWSTR)0x0,L':'), local_be8 != (LPWSTR)0x0)) {
              *local_be8 = 0;
              local_be8 = local_be8 + 1;
            }
            local_b78 = local_b78 + 1;
            local_b68 = (LPCWSTR)(uintptr_t)PECMD_ParseItemImageSpec((longlong *)&local_b78,(longlong *)&local_8f8,
                                               (ushort *)L"EXEDATA");
            _Var39.v = (uint64_t)(uintptr_t)DAT_14013cf70;
            local_d78 = *pWVar30;
            *(uint16_t *)(uintptr_t)pWVar30 = 0;
            local_cfc = 0x20;
            _Var59.v = _Var48.v;
            if (((*(short *)(uintptr_t)local_b58.v != 0x2e) ||
                 (*(short *)((uintptr_t)local_b58.v + 2) != 0)) &&
                (*(short *)(uintptr_t)local_b58.v != 0)) {
              _Var39.v = LoadLibraryExW((LPCWSTR)(uintptr_t)local_b58.v,(HANDLE)0x0,2);
              _Var59.v = _Var39.v;
            }
            *(uint16_t *)(uintptr_t)pWVar30 = local_d78;
            if (_Var39.v != 0) {
              local_cfc = 0x20;
              _Var48.v = (uint64_t)(uintptr_t)
                       PECMD_LoadEncodedResource((HMODULE)(uintptr_t)_Var39.v,
                            (uint16_t *)(uintptr_t)local_8f8,(uint16_t *)(uintptr_t)local_b68,
                            (longlong *)&local_ba8.v,&local_cfc);
              local_bc0.v = _Var48.v;
            }
            if (_Var59.v != 0) {
              FreeLibrary((HMODULE)(uintptr_t)_Var59.v);
            }
            goto LAB_1400141ec;     /* 定义于 SEG9 (raw 3115); 占位见尾部 */
          }
          *(uint32_t *)&local_ca8[24] = local_res10.ft.dwLowDateTime;
          *(uint32_t *)&local_ca8[28] = local_res10.ft.dwHighDateTime;
          if ((local_res10.v != 0) && (*(WCHAR *)(uintptr_t)local_res10.v == L'*')) {
            local_db0 = (local_db0 & 0xffff0000u) | (uint32_t)(uint16_t)((WCHAR)local_db0 + 1);
                                                 /* 原 CONCAT22(local_db0._2_2_,+1) */
            *(uint64_t *)&local_ca8[24] = (longlong)local_res10.v + 2;
          }
          if ((char)local_d40 != '\0') {
            local_db0 = local_db0 & 0xffff0000u;
          }
          if ((short)local_dd8[0] != 0) {
            local_be8 = StrRChrW((LPCWSTR)(uintptr_t)local_b58.v,(LPCWSTR)0x0,L':');
            if (local_be8 != (LPWSTR)0x0) {
              pWVar24 = local_be8 + 1;
              *local_be8 = 0;
              local_be8 = pWVar24;
              if (pWVar24 != (LPWSTR)0x0) {
                local_db8 = 2;
                local_c00 = (HANDLE *)0x0;
                _Var39.v = local_b58.v;
                if (*(short *)(uintptr_t)local_b58.v == 0x22) {
                  _Var39.v = local_b58.v + 2;
                  local_b58.v = _Var39.v;
                  local_ab8.v = _Var39.v;
                  PECMD_SkipWCharUntil(&local_ab8.v,0x22);
                  if (*(short *)(uintptr_t)local_ab8.v != 0) {
                    *(short *)(uintptr_t)local_ab8.v = 0;
                    _Var39.v = local_b58.v;
                  }
                }
                PECMD_AssignString((longlong *)&local_d28.v,(LPCWSTR)(uintptr_t)_Var39.v);
                _Var39.v = _Var48.v;
                goto LAB_140014239;   /* 定义于 SEG9 (raw 3122); 占位见尾部 */
              }
            }
            if (local_b78 != (LPCWSTR)0x0) goto LAB_1400140bc;
          }
          _Var59.v = local_d68.v;
          _Var39.v = (uint64_t)(uintptr_t)
                   PECMD_ResolveWildcardPath((uint64_t *)(void *)&local_cd0,
                        (short *)(uintptr_t)local_dd0.v,(LPWSTR)(uintptr_t)local_d68.v,
                        (LPCWSTR)(uintptr_t)local_res10.v);
          bVar16 = PECMD_IsFile((LPCWSTR)(uintptr_t)_Var39.v);
          if (bVar16 != false) {                     /* 原 CONCAT71(extraout_var,bVar16) */
LAB_140014a6b:
            local_df8 = 3;
            PECMD_FreeStrBuf((WCHAR **)&local_d28.v);
            PECMD_FreeStrBuf((WCHAR **)&local_ba8.v);
            PECMD_FreeStrBuf(&local_bb8);
            goto LAB_140014c93;       /* 定义于 SEG9 (raw 3363); 占位见尾部 */
          }
          if ((short)local_ddc == 0) {
            pWVar24 = StrRChrW((LPCWSTR)(uintptr_t)local_res10.v,(LPCWSTR)0x0,L'\\');
            _Var53.v = local_res10.v;
            if (pWVar24 != (LPWSTR)0x0) {
              _Var53.v = (uint64_t)(uintptr_t)(pWVar24 + 1);
            }
            uVar23 = PECMD_AsciiPrefixICmp("winpeshl",(ushort *)(uintptr_t)_Var53.v,8);
            if (((char)uVar23 != '\0') &&
               (((uVar34 = *(ushort *)((uintptr_t)_Var53.v + 0x10), uVar34 == 0 || (uVar34 == 0x2e)) ||
                (((8 < uVar34 && (uVar34 < 0xe)) || (uVar34 == 0x20)))))) {
              bVar7 = true;
            }
            uVar23 = PECMD_AsciiPrefixICmp("wpeinit",(ushort *)(uintptr_t)_Var53.v,8);
            _Var59.v = local_d68.v;
            if (((char)uVar23 != '\0') &&
               ((((uVar34 = *(ushort *)((uintptr_t)_Var53.v + 0xe), uVar34 == 0 || (uVar34 == 0x2e)) ||
                 ((8 < uVar34 && (uVar34 < 0xe)))) || (uVar34 == 0x20)))) {
              bVar7 = true;
            }
          }
          if (((param_5 & 1) != 0) || (local_d48 != 0)) {
            iVar52 = lstrlenW((LPCWSTR)(uintptr_t)local_res10.v);
            PECMD_RegSetValueWithOpen((HKEY)0xffffffff80000002,
                          (const unsigned short *)L"SOFTWARE\\PELOGON",
                          (const unsigned short *)L"Shell.x",1,
                          (BYTE *)local_res10.v,iVar52 * 2);
          }
          if (local_dbc == 0) {
            if (local_d98[0] == 0) {
              local_df4 = PECMD_CreateProcessW((LPCWSTR)(uintptr_t)_Var59.v,(LPWSTR)(uintptr_t)local_res10.v,
                                        (LPSECURITY_ATTRIBUTES)0x0,(LPSECURITY_ATTRIBUTES)0x0,
                                        local_de8,local_db4 | local_df0,(LPVOID)(uintptr_t)local_dc8.v,
                                        (LPCWSTR)(uintptr_t)_Var39.v,&local_888,&local_af8);
            }
            else {
              local_df4 = PECMD_CreateProcessAsUser((LPCWSTR)(uintptr_t)_Var59.v,(LPWSTR)(uintptr_t)local_res10.v,
                                        (LPSECURITY_ATTRIBUTES)0x0,(LPSECURITY_ATTRIBUTES)0x0,
                                        local_de8,local_db4 | local_df0,(LPVOID)(uintptr_t)local_dc8.v,
                                        (LPCWSTR)(uintptr_t)_Var39.v,&local_888,
                                        (uint64_t)(uintptr_t)&local_af8,
                                        local_d98[0] & 0xffff,(LPCWSTR)0x0,(LPCWSTR)L".",(LPCWSTR)0x0);
            }
          }
          else {
            bVar16 = PECMD_LaunchServiceProcess((LPCWSTR)(uintptr_t)_Var59.v,
                                                (LPCWSTR)(uintptr_t)local_res10.v);
            local_df4 = (uint)(bVar16 != false);   /* 原 CONCAT71(extraout_var,bVar16) */
          }
          bVar7 = (bool)((local_df4 != 0) & bVar7);
          if (bVar7) {
            iVar52 = PECMD_IsSysStartuped();
            if (iVar52 != 0) {
              local_e00 = local_e00 | 1;
            }
            if (local_e00 == 0) {
              bVar16 = (bool)FUN_140101e70((const uint16_t *)L"waitpeinit.pecmd");
              local_e00 = (uint)(bVar16 != false); /* 原 CONCAT71(extraout_var,bVar16) */
            }
          }
          if ((((local_df4 == 0) && (!bVar4)) && (_Var59.v == 0)) &&
             (DVar21 = GetLastError(), DVar21 == 0xc1)) {
LAB_14001408b:
            PECMD_FreeStrBuf((WCHAR **)&local_d28.v);
            PECMD_FreeStrBuf((WCHAR **)&local_ba8.v);
            PECMD_FreeStrBuf(&local_bb8);
            goto LAB_140014ade;
          }
        }
        else {
          WVar62 = L'\"';
          if (*(short *)(uintptr_t)*(uint64_t *)&local_ca8[24] == 0x22) {
            *(uint64_t *)&local_ca8[24] = *(uint64_t *)&local_ca8[24] + 2;
          }
          uVar23 = *(uint64_t *)&local_ca8[24];
          wMatch = L':';
          PECMD_SkipWCharUntil((void *)(local_ca8 + 0x18),0x3a);
          if ((*(uint64_t *)&local_ca8[24] != uVar23) &&
              (*(WCHAR *)(uintptr_t)(*(uint64_t *)&local_ca8[24] - 2) == WVar62)) {
            *(WCHAR *)(uintptr_t)(*(uint64_t *)&local_ca8[24] - 2) = L'\0';
          }
          if (wMatch == *(WCHAR *)(uintptr_t)*(uint64_t *)&local_ca8[24]) {
            *(WCHAR *)(uintptr_t)*(uint64_t *)&local_ca8[24] = L'\0';
            *(uint64_t *)&local_ca8[24] = *(uint64_t *)&local_ca8[24] + 2;
          }
          _Var39.v = *(uint64_t *)&local_ca8[24];
          if (WVar62 == *(WCHAR *)(uintptr_t)*(uint64_t *)&local_ca8[24]) {
            _Var39.v = *(uint64_t *)&local_ca8[24] + 2;
            do {
              *(uint64_t *)&local_ca8[24] = *(uint64_t *)&local_ca8[24] + 2;
              if (WVar62 == *(WCHAR *)(uintptr_t)*(uint64_t *)&local_ca8[24]) {
                *(WCHAR *)(uintptr_t)*(uint64_t *)&local_ca8[24] = L'\0';
              }
            } while (*(WCHAR *)(uintptr_t)*(uint64_t *)&local_ca8[24] != L'\0');
          }
          if (((short)local_dd8[0] != 0) &&
             (local_be8 = StrChrW((LPCWSTR)(uintptr_t)_Var39.v,wMatch), local_be8 != (LPWSTR)0x0)) {
            *local_be8 = 0;
            local_be8 = local_be8 + 1;
          }
          PECMD_GenerateTimeText((LPCWSTR)(uintptr_t)uVar23,(longlong *)&local_ba8.v,_Var39.v,0x10000,0);
          _Var48.v = (uint64_t)(local_b98 + (uintptr_t)local_ba8.v);
          local_bc0.v = _Var48.v;
LAB_1400141ec:
          if ((local_ba0 != 0) && (_Var48.v != 0)) {
            DVar21 = 1;
            local_db8 = 1;
            _Var39.v = local_ab8.v;
            pWVar24 = local_be8;
            if (((WCHAR)local_db0 == 0) && ((short)local_dd8[0] == 0)) goto LAB_140014928;
LAB_140014239:
            PECMD_CreateTempMutexDir((int64_t *)&local_d30.QuadPart,
                                     (longlong *)(uintptr_t)_Var39.v,(uint64_t *)(void *)local_c00,
                                     (uint64_t)(uintptr_t)(const uint16_t *)L"exedat");
            PECMD_ReplaceStringSlot((longlong *)(local_ca8 + 8),(uint64_t *)(void *)&local_d30);
            pvVar32 = local_9f0;
            local_c00 = &local_9f0;
            local_ab8.v = (uint64_t)(uintptr_t)&local_d28.v;
            local_9d8[0] = 0;
            if (local_9f0 == (HANDLE)0x0) {
              if ((int)DVar21 < 2) goto LAB_140014c4b;
            }
            else {
              WriteFile(local_9f0,(void *)(uintptr_t)local_bc0.v,(DWORD)local_ba0,local_9d8,
                        (LPOVERLAPPED)0x0);
              PECMD_FlushFileThrice(pvVar32);
              CloseHandle(pvVar32);
            }
            _Var39.v = local_d28.v;
            local_e06 = '\0';
            if (pWVar24 == (LPWSTR)0x0) {
              PECMD_StrBldCopyWide((long long *)(local_ca8 + 8),(LPCWSTR)(uintptr_t)local_d30.QuadPart);
            }
            else {
              local_d28.ft.dwLowDateTime = 0;
              local_d28.ft.dwHighDateTime = 0;
              local_9e0.v = _Var39.v;
              PECMD_StrBldCopyWide((long long *)&local_d28.v,(LPCWSTR)(uintptr_t)local_d30.QuadPart);
              PECMD_AppendWideStr(&local_d28.v,(LPCWSTR)L"\\.ex");
              PECMD_ReplaceStringSlot((longlong *)(local_ca8 + 8),&local_d28.v);
              pWVar31 = pWVar24;
              if (*pWVar24 != L'\\') {
                pWVar31 = pWVar24 + -1;
                *pWVar31 = L'\\';
              }
              CreateDirectoryW((LPCWSTR)*(uint64_t *)&local_ca8[8],(LPSECURITY_ATTRIBUTES)0x0);
              if ((short)local_dd8[0] == 1) {
                PECMD_StrDupAssign((uint16_t **)&local_bd8.v,(LPCWSTR)L"*extract ");
                PECMD_AppendWideStr(&local_bd8.v,(LPCWSTR)(uintptr_t)local_9e0.v);
                PECMD_AppendWideStr(&local_bd8.v,(LPCWSTR)L",,");
                PECMD_AppendWideStr(&local_bd8.v,(LPCWSTR)(uintptr_t)local_d28.v);
                PECMD_DriverInstall(param_1,(LPCWSTR)(uintptr_t)local_bd8.v);
                PECMD_FreeStrBuf((WCHAR **)&local_bd8.v);
              }
              else if ((short)local_dd8[0] == 0x100) {
                local_5c8.cb = 0x68;
                local_5c8.lpReserved = (LPWSTR)0x0;
                FUN_140102a90((ulonglong *)&local_5c8.lpDesktop,0,0x58);
                local_8d0.hThread = (HANDLE)0x0;
                local_8d0.dwProcessId = 0;
                local_8d0.dwThreadId = 0;
                local_5c8.dwFlags = 0x80;
                local_8d0.hProcess = (HANDLE)0x0;
                CreateProcessW((LPCWSTR)(uintptr_t)local_9e0.v,(const uint16_t *)L"*ex",
                               (LPSECURITY_ATTRIBUTES)0x0,(LPSECURITY_ATTRIBUTES)0x0,0,local_db4,
                               (LPVOID)0x0,(LPCWSTR)*(uint64_t *)&local_ca8[8],&local_5c8,&local_8d0);
                WaitForSingleObject(local_8d0.hProcess,0xffffffff);
                if (local_8d0.hProcess != (HANDLE)0x0) {
                  CloseHandle(local_8d0.hProcess);
                }
                if (local_8d0.hThread != (HANDLE)0x0) {
                  CloseHandle(local_8d0.hThread);
                }
              }
              PECMD_AppendWideStr(&local_d28.v,pWVar31);
              PECMD_ReplaceStringSlot((longlong *)&local_cd0,&local_d28.v);
              PECMD_FreeStrBuf((WCHAR **)&local_9e0.v);
            }
            uVar23 = *(uint64_t *)&local_ca8[8];
            _Var39.v = local_res10.v;
            if (*(WCHAR *)(uintptr_t)local_res10.v == L'\"') {
              _Var39.v = local_res10.v + 2;
            }
            if (*(WCHAR *)(uintptr_t)_Var39.v == L'?') {
              pWVar30 = (LPCWSTR)((uintptr_t)_Var39.v + 2);
              if (*pWVar30 == L'?') {
                pWVar30 = (LPCWSTR)((uintptr_t)_Var39.v + 4);
                _Var39.v = local_d28.v;
              }
              else {
                if (*pWVar30 != L'\\') goto LAB_140014652;
                _Var39.v = *(uint64_t *)&local_ca8[8];
                if (*(WCHAR *)(uintptr_t)local_res10.v != L'\"') {
                  *(uint32_t *)&local_ca8[8] = 0;
                  *(uint32_t *)&local_ca8[12] = 0;
                  uVar12 = *(uint64_t *)&local_ca8[8];
                  *(uint32_t *)&local_ca8[8] = (uint32_t)uVar23;          /* 原 _8_4_/SUB84 字节片 */
                  *(uint32_t *)&local_ca8[12] = (uint32_t)(uVar23 >> 32); /* 原 SUB84(uVar23,4) */
                  local_938.ft.dwLowDateTime = *(uint32_t *)&local_ca8[8];
                  local_938.ft.dwHighDateTime = *(uint32_t *)&local_ca8[12];
                  *(uint64_t *)&local_ca8[8] = uVar12;
                  PECMD_AllocString((undefined8 *)(local_ca8 + 8),0x104);
                  *(uint16_t *)(uintptr_t)*(uint64_t *)&local_ca8[8] = 0;
                  GetShortPathNameW((LPCWSTR)(uintptr_t)local_938.v,
                                    (LPWSTR)*(uint64_t *)&local_ca8[8],0x104);
                  PECMD_FreeStrBuf((WCHAR **)&local_938.v);
                  _Var39.v = *(uint64_t *)&local_ca8[8];
                }
              }
              if (*(WCHAR *)(uintptr_t)local_res10.v == L'\"') {
                PECMD_StrBldCopyWide((long long *)&local_c38.v,(const uint16_t *)L"\"");
              }
              PECMD_AppendWideStr(&local_c38.v,(LPCWSTR)(uintptr_t)_Var39.v);
              PECMD_AppendWideStr(&local_c38.v,pWVar30);
              local_res10.v = local_c38.v;
            }
LAB_140014652:
            if (pWVar24 == (LPWSTR)0x0) {
LAB_1400146c2:
              _Var39.v = local_d28.v;
              if (pWVar24 != (LPWSTR)0x0) {
                _Var39.v = (ulonglong)local_d28.v & -(ulonglong)(*pWVar24 != L'\0');
              }
              _Var59.v = local_dd0.v;
              if (local_dd0.v == 0) {
                _Var59.v = *(uint64_t *)&local_ca8[8];
              }
              _Var59.v = (uint64_t)(uintptr_t)
                       PECMD_ResolveWildcardPath((uint64_t *)(void *)&local_cd0,
                            (short *)(uintptr_t)_Var59.v,(LPWSTR)(uintptr_t)local_d28.v,
                            (LPCWSTR)(uintptr_t)local_res10.v);
              local_dd0.v = _Var59.v;
              bVar16 = PECMD_IsFile((LPCWSTR)(uintptr_t)_Var59.v);
              if (bVar16 != false) goto LAB_140014a6b;   /* 原 CONCAT71(extraout_var,bVar16) */
              if (local_dbc == 0) {
                if (local_d98[0] == 0) {
                  local_df4 = PECMD_CreateProcessW((LPCWSTR)(uintptr_t)_Var39.v,(LPWSTR)(uintptr_t)local_res10.v,
                                            (LPSECURITY_ATTRIBUTES)0x0,(LPSECURITY_ATTRIBUTES)0x0,
                                            local_de8,local_db4 | local_df0,(LPVOID)(uintptr_t)local_dc8.v,
                                            (LPCWSTR)(uintptr_t)_Var59.v,&local_888,&local_af8);
                }
                else {
                  local_df4 = PECMD_CreateProcessAsUser((LPCWSTR)(uintptr_t)_Var39.v,(LPWSTR)(uintptr_t)local_res10.v,
                                            (LPSECURITY_ATTRIBUTES)0x0,(LPSECURITY_ATTRIBUTES)0x0,
                                            local_de8,local_db4 | local_df0,(LPVOID)(uintptr_t)local_dc8.v,
                                            (LPCWSTR)(uintptr_t)_Var59.v,&local_888,
                                            (uint64_t)(uintptr_t)&local_af8,
                                            local_d98[0] & 0xffff,(LPCWSTR)0x0,(LPCWSTR)L".",(LPCWSTR)0x0);
                }
              }
              else {
                bVar16 = PECMD_LaunchServiceProcess((LPCWSTR)(uintptr_t)_Var39.v,
                                                    (LPCWSTR)(uintptr_t)local_res10.v);
                local_df4 = (uint)(bVar16 != false);     /* 原 CONCAT71(extraout_var,bVar16) */
              }
              if (((char)local_db0 < '\x02') || ((short)local_dd8[0] != 0)) goto LAB_140014c4b;
            }
            else if ((ushort)local_db0 == 0) {
              local_be8 = (LPWSTR)0x0;
              PECMD_LoadFileToSlot(local_cd0,(longlong *)&local_ba8.v);
              pWVar24 = (LPWSTR)0x0;
              if (local_ba0 == 0) goto LAB_1400146c2;
              local_bc0.v = local_ba8.v;
              local_e06 = '\x01';
            }
            else {
              if (!bVar4) goto LAB_1400146c2;
              local_be8 = (LPWSTR)0x0;
              local_e06 = '\x02';
              if (local_dd0.v == 0) {
                local_dd0.v = *(uint64_t *)&local_ca8[8];
              }
              _Var39.v = local_dd0.v;
              if ((ushort)local_db0 < 2) goto LAB_14001408b;
            }
            while( true ) {
              _Var48.v = local_bc0.v;
              if (local_e06 == '\x01') {
                DVar21 = 0xffffffff;
              }
              else {
                DVar21 = local_af8.dwProcessId;
              }
              PECMD_ScheduleSelfDelete(local_d30,DVar21);
              *(uint16_t *)(uintptr_t)local_d30.QuadPart = 0;
              if (local_e06 != '\x01') {
                if (local_e06 != '\x02') goto LAB_140014c4b;
                PECMD_FreeStrBuf((WCHAR **)&local_d28.v);
                PECMD_FreeStrBuf((WCHAR **)&local_ba8.v);
                PECMD_FreeStrBuf(&local_bb8);
                _Var39.v = local_dd0.v;
                goto LAB_140014ade;
              }
LAB_140014928:
              _Var39.v = local_res10.v;
              if (*(WCHAR *)(uintptr_t)local_res10.v == L'\"') {
                _Var39.v = local_res10.v + 2;
              }
              WVar62 = L'\0';
              _Var59.v = local_res10.v;
              if (((*(WCHAR *)(uintptr_t)_Var39.v == L'?') &&
                   (*(LPCWSTR *)((uintptr_t)_Var39.v + 2) == L'\\')) &&
                  (_Var59.v = _Var39.v + 4, *(WCHAR *)(uintptr_t)_Var59.v == L'\"')) {
                _Var59.v = _Var39.v + 2;
                WVar62 = *(WCHAR *)(uintptr_t)_Var59.v;
                *(WCHAR *)(uintptr_t)_Var59.v = L'\"';
              }
              bVar16 = PECMD_IsFile((LPCWSTR)(uintptr_t)local_dd0.v);
              if (bVar16 != false) goto LAB_140014a6b; /* 原 CONCAT71(extraout_var,bVar16) */
              iVar52 = PECMD_CreateInjectedProcess((short *)(uintptr_t)_Var48.v,(DWORD)local_ba0,
                                     (WCHAR *)(uintptr_t)_Var59.v,local_de8,
                                     local_df0,(LPVOID)(uintptr_t)local_dc8.v,
                                     (LPCWSTR)(uintptr_t)local_dd0.v,&local_888,
                                     &local_af8,(LPCWSTR)0x0);
              DVar21 = local_db8;
              if (WVar62 != L'\0') {
                *(WCHAR *)(uintptr_t)_Var59.v = WVar62;
              }
              _Var39.v = local_ab8.v;
              pWVar24 = local_be8;
              if (((iVar52 < 0) || (local_af8.dwProcessId == 0)) ||
                 (pvVar32 = (HANDLE)(uintptr_t)OpenProcess(0x400,0,local_af8.dwProcessId),
                  _Var39.v = local_ab8.v, pWVar24 = local_be8, pvVar32 == (HANDLE)0x0)) break;
              CloseHandle(pvVar32);
              local_df4 = (uint)(0 < iVar52);
              if ((((LPCWSTR)(uintptr_t)local_d30.QuadPart == (LPCWSTR)0x0) ||
                   (*(WCHAR *)(uintptr_t)local_d30.QuadPart == L'\0')))
              goto LAB_140014c4b;
            }
            goto LAB_140014239;
          }
        }
LAB_140014c4b:
        PECMD_FreeStrBuf((WCHAR **)&local_d28.v);
        PECMD_FreeStrBuf((WCHAR **)&local_ba8.v);
        PECMD_FreeStrBuf(&local_bb8);
        goto LAB_140014c72;
      }
      pWVar30 = (LPCWSTR)PECMD_ResolveWildcardPath((uint64_t *)(void *)&local_cd0,
                      (short *)(uintptr_t)local_dd0.v,(LPWSTR)(uintptr_t)local_d68.v,
                      (LPCWSTR)(uintptr_t)local_res10.v);
      bVar16 = PECMD_IsFile(pWVar30);
      if (bVar16 == false) {                     /* 原 CONCAT71(extraout_var_00,bVar16)==0 */
        if (local_d98[0] == 0) {
          local_920 = PECMD_RestrictedTokenSetup((HANDLE)0x0);
          local_df4 = CreateProcessAsUserW
                                (local_920,(LPCWSTR)(uintptr_t)local_d68.v,(LPWSTR)(uintptr_t)local_res10.v,
                                 (LPSECURITY_ATTRIBUTES)0x0,(LPSECURITY_ATTRIBUTES)0x0,local_de8,
                                 local_db4 | local_df0,(LPVOID)(uintptr_t)local_dc8.v,pWVar30,&local_888,
                                 &local_af8);
        }
        else {
          local_df4 = PECMD_CreateProcessAsUser((LPCWSTR)(uintptr_t)local_d68.v,(LPWSTR)(uintptr_t)local_res10.v,
                                    (LPSECURITY_ATTRIBUTES)0x0,(LPSECURITY_ATTRIBUTES)0x0,local_de8,
                                    local_db4 | local_df0,(LPVOID)(uintptr_t)local_dc8.v,pWVar30,&local_888,
                                    (uint64_t)(uintptr_t)&local_af8,local_d98[0] & 0xffff | 2,
                                    (LPCWSTR)0x0,(LPCWSTR)L".",(LPCWSTR)0x0);
        }
        goto LAB_140014c72;
      }
LAB_1400132d5:
      local_df4 = 0;
      local_df8 = 3;
    }
LAB_140014c93:
    if ((local_bc8 != (HANDLE)0x0) && (local_bc8 != (HANDLE)0xffffffffffffffff)) {
      CloseHandle(local_bc8);
    }
    local_bc8 = (HANDLE)0x0;
    if (local_d70.v != 0) {
      CloseHandle((HANDLE)(uintptr_t)local_d70.v);
    }
    local_d70.ft.dwLowDateTime = 0;
    local_d70.ft.dwHighDateTime = 0;
    if (local_c68 != (LPCWSTR)0x0) {
      PECMD_VarSetUInt(param_1,(uint64_t)local_af8.dwProcessId,local_c68);
    }
    if (local_c58 != (LPCWSTR)0x0) {
      WVar62 = *local_c58;
      pWVar42 = local_c58;
      pWVar8 = local_c58;
      while (WVar62 == L'&') {
        WVar62 = pWVar8[1];
        pWVar42 = pWVar8;
        pWVar8 = pWVar8 + 1;
      }
      if (*pWVar42 == L'&') {
        pWVar42 = pWVar42 + 1;
      }
      for (; *pWVar42 == L':'; pWVar42 = pWVar42 + 1) {
      }
      if (*pWVar42 != L'\0') {
        PECMD_WrapParamCall_02d8(param_1,(uint64_t)(uintptr_t)local_af8.hProcess,local_c58);
      }
    }
    if ((bVar5) && (local_af8.hProcess != (HANDLE)0x0)) {
      local_c70 = (HANDLE)(uintptr_t)CreateJobObjectW((LPSECURITY_ATTRIBUTES)0x0,(LPCWSTR)0x0);
      AssignProcessToJobObject(local_c70,local_af8.hProcess);
    }
    PECMD_VarSetUInt(param_1,(uint64_t)local_af8.dwProcessId,(const uint16_t *)L"&&__LastPID");
    PECMD_VarSetUInt(param_1,(uint64_t)local_af8.dwThreadId,(const uint16_t *)L"&&__LastTID");
    if (local_de0 != 0) {
      PECMD_AdjustTokenPrivilege((LPCWSTR)L"SeDebugPrivilege",(uint)local_da8,local_af8.hProcess);
    }
    if ((((byte)local_df0 & (byte)local_da8) != 0) && (local_af8.hThread != (HANDLE)0x0)) {
      ResumeThread(local_af8.hThread);
    }
    PECMD_FreeStrBuf((WCHAR **)&local_c38.v);
    PECMD_FreeStrBuf((WCHAR **)(local_ca8 + 8));
    PECMD_FreeStrBuf((WCHAR **)&local_cd0);
    PECMD_FreeStrBuf((WCHAR **)&local_a30.v);
    if (bVar14) {
      g_privFlags = g_privFlags & 0xfffb;
      PECMD_EnableTokenPrivilege((LPCWSTR)L"SeDebugPrivilege",(uint)local_da8,0);
    }
    pvVar45 = (HANDLE)0x0;
    if (local_be0.v != 0) {
      CloseHandle((HANDLE)(uintptr_t)local_be0.v);
    }
  }
  /* ---- SEG10 (decompiled 3419-3944): 等待/管道输出分发与全局清理+真实返回 ---- */
  lpString.ft.dwLowDateTime = 0;
  lpString.ft.dwHighDateTime = 0;
  local_be0.ft.dwLowDateTime = 0;
  local_be0.ft.dwHighDateTime = 0;
  if (local_d80 != (HANDLE)0x0) {
    CloseHandle(local_d80);
  }
  local_d80 = (HANDLE)0x0;
  if (local_cf0 != (HANDLE)0x0) {
    CloseHandle(local_cf0);
  }
  local_cf0 = (HANDLE)0x0;
  if (local_c88.v != 0) {
    CloseHandle((HANDLE)(uintptr_t)local_c88.v);
  }
  local_c88.v = 0;
  if (local_c20.v != 0) {
    CloseHandle((HANDLE)(uintptr_t)local_c20.v);
  }
  local_c20.ft.dwLowDateTime = 0;
  local_c20.ft.dwHighDateTime = 0;
  if (((char)param_5 != '\0') || (bVar2)) {
    if (local_af8.hProcess != (HANDLE)0x0) {
      WaitForInputIdle(local_af8.hProcess,5000);
    }
    if ((char)param_5 != '\0') {
      if ((param_5 & 1) != 0) {
        PECMD_WriteSysAck(-(uint)(local_af8.hProcess != (HANDLE)0x0) & 2 | 1,
                          (int)local_af8.dwProcessId);
      }
      if (local_a10.QuadPart != 0) {
        PECMD_ProcessScriptBlock((LARGE_INTEGER){ .QuadPart = (long long)(uintptr_t)param_1 },
                      local_a10,(longlong *)0x0,(longlong *)0x0,(pthreadmbcinfo)0x0);
      }
    }
  }
  if ((local_df4 == 0) && ((longlong)local_a98 < 1)) {
    pvVar32 = local_c70;
    DVar21 = local_e00;
    if (*local_d58 == L'\0') goto LAB_140014f96;
    PECMD_SetVariable(param_1,local_d58,(LPCWSTR)(uintptr_t)DAT_14011c638);
    _Var43.ft.dwHighDateTime = 0;
    _Var43.ft.dwLowDateTime = local_e00;
  }
  else {
    local_e00 = -(uint)(local_af8.hProcess != (HANDLE)0x0) & local_e00;
    local_970[(int)local_d08] = local_af8.hProcess;
    local_d08 = local_d08 + 1;
    if (local_af8.hThread != (HANDLE)0x0) {
      CloseHandle(local_af8.hThread);
    }
    local_af8.hThread = (HANDLE)0x0;
    if (local_d48 != 0) {
      PECMD_WaitHandlesOrMessages((uint64_t)(uintptr_t)param_1,1000,0,(uint64_t *)0x0);
      pvVar32 = local_af8.hProcess;
      if (local_af8.hProcess == (HANDLE)0x0) {
        if ((longlong)local_a98 < 1) goto LAB_14001512a;
        pvVar32 = (HANDLE)(uintptr_t)OpenProcess(0x1fffff,0,(DWORD)local_a98);
        local_e00 = 0;
        _Var43.v = lpString.v;
        if (pvVar32 == (HANDLE)0x0) goto LAB_140014f8e;
      }
      PECMD_PatchRemoteWinExec(pvVar32);
      if (local_af8.hProcess == (HANDLE)0x0) {
        CloseHandle(pvVar32);
      }
    }
LAB_14001512a:
    local_e07 = DAT_14013a24f;
    _Var43.ft.dwHighDateTime = 0;
    _Var43.ft.dwLowDateTime = local_e00;
    if (local_e00 != 0) {
      _Var56.ft.dwLowDateTime = 0x100000;
      _Var56.ft.dwHighDateTime = 0;
      local_d18.ft.dwLowDateTime = 0x100000;
      local_d18.ft.dwHighDateTime = 0;
      PECMD_AllocStringSlot2((void **)&local_dd0.v,0x100004);
      local_8b8 = local_d58 + 1;
      local_b10 = 0;
      local_b08.ft.dwLowDateTime = 0;
      local_b08.ft.dwHighDateTime = 0;
      local_b00 = 0;
      local_9b0 = local_d58;
      param_5 = param_5 & 0xffffff00u;           /* 原 param_5._0_1_='\0' */
      if (*local_d58 == L'@') {
        param_5 = (param_5 & 0xffffff00u) | 1;   /* 原 param_5._0_1_='\x01' */
        local_9b0 = local_8b8;
      }
      else if (local_c48 != (LPCWSTR)0x0) {
        param_5 = (param_5 & 0xffffff00u) | 2;   /* 原 param_5._0_1_='\x02' */
      }
      _Var39.v = lpString.v;
      if (local_d3c != '\0') {
        local_9d0 = (LPCWSTR)0x0;
        PECMD_ResolveVariable(param_1,local_9b0,(uint64_t *)(void *)&local_9d0,0);
        PECMD_WideToAnsiStr((longlong *)&local_dd0.v,local_9d0,-1,(uint64_t)0xffffffffffffffff);
        iVar52 = lstrlenA((LPCSTR)(uintptr_t)local_dd0.v);
        _Var39.v = (uint64_t)(int64_t)iVar52;
        uVar25 = ((longlong)_Var39.v + 0x3ffU) & 0xfffffffffffffc00;
        _Var56.v = uVar25 + 0x100000;
        local_d18.v = _Var56.v;
        PECMD_GrowByteBuffer((void **)&local_dd0.v,uVar25 + 0x100008);
        PECMD_FreeStrBuf((WCHAR **)&local_9d0);
      }
      pvVar32 = (HANDLE)(uintptr_t)GetCurrentProcess();
      SetProcessWorkingSetSize(pvVar32,0xffffffffffffffff,0xffffffffffffffff);
      local_df8 = GetTickCount();
      uVar35 = local_d04;
      if ((int)local_d04 < 1) {
        uVar35 = 1000;
      }
      local_de0 = (uint)(((ushort)local_ddc & 0x101) == 0x101);
      local_d04 = uVar35;
      local_db0 = local_df8;
      if ((local_ddc & 8) != 0) {
        if (((*local_d58 == L'@') || (local_d9a == '\0')) ||
           ((*local_d58 != L'&' && ((*(uint8_t *)((uintptr_t)param_1 + 0xd) & 0xf) == 0)))) {
          bVar2 = false;
        }
        else {
          bVar2 = true;
        }
        local_de8 = 0;
        SetLastError(0);
        ConnectNamedPipe(local_bf0,&local_8f0);
        bVar16 = true;
        if ((char)param_5 == '\0') {
          DVar21 = 0x100000;
        }
        else {
          DVar21 = _Var56.ft.dwLowDateTime - _Var39.ft.dwLowDateTime;
        }
        local_9c0 = (longlong)(int)DVar21;
        local_978.v = _Var39.v;
        ReadFile(local_bf0,(void *)((longlong)local_dd0.v + (uintptr_t)_Var39.v),DVar21,&local_de8,
                 &local_8f0);
        local_df4 = local_db0;
        local_d48 = 0;
        bVar4 = false;
        PECMD_AllocStrSlot((uint16_t **)&local_b18.v);
        local_d40 = 1;
LAB_1400153e3:
        local_e07 = DAT_14013a24f;
        if ((DAT_14013a24f < '\x01') && ((int)local_c80 < 2)) goto LAB_140015a5e;
        if (0 < (int)local_c28) {
          DVar21 = GetTickCount();
          if (local_c28 <= DVar21 - local_db0) goto LAB_140015a5e;
          uVar36 = local_c28 - (DVar21 - local_db0);
          if (uVar36 < local_d04) {
            local_d04 = uVar36;
          }
        }
        /* 原 local_9a0 = PECMD_MsgWaitForObjects(...): 其还原体(core_b1_remaining.c:7956)返回 void
         * 不透传 MsgWait 结果; 此处按包装体内联展开以保留反编译视图的数据流 TODO(verify) */
        local_9a0 = (int)MsgWaitForMultipleObjects(local_d08,local_970,0,local_d04,0x4ff);
        local_dbc = 0;
        if (local_9a0 == 0) {
          local_db8 = 0;
          GetOverlappedResult(local_bf0,&local_8f0,&local_db8,0);
          if (0 < (int)local_db8) {
            if (local_9c0 < (int)local_db8) {
              local_db8 = (uint)local_9c0;
            }
            if (bVar16) {
              _Var39.v = (longlong)_Var39.v + (ulonglong)local_db8;
              *(uint8_t *)(uintptr_t)((longlong)local_dd0.v + (uintptr_t)_Var39.v) = 0;
            }
          }
          _Var59.v = _Var56.v;
          if ((longlong)_Var56.v <= (longlong)((uintptr_t)_Var39.v + 0x100000)) {
            _Var59.v = _Var56.v + 0x100000;
            local_d18.v = _Var59.v;
            PECMD_GrowByteBuffer((void **)&local_dd0.v,(longlong)_Var56.v + 0x100004);
          }
          if ((char)param_5 == '\0') {
            DVar21 = 0x100000;
          }
          else {
            DVar21 = _Var59.ft.dwLowDateTime - _Var39.ft.dwLowDateTime;
          }
          local_9c0 = (longlong)(int)DVar21;
          if (local_de0 == 0) {
            ReadFile(local_bf0,(void *)((longlong)local_dd0.v + (uintptr_t)_Var39.v),DVar21,
                     &local_db8,&local_8f0);
          }
          else {
            local_dbc = 1;
          }
        }
        local_d60 = GetTickCount();
        if (local_ce8[0] != 0) {
          local_ce8[0] = (uVar35 - local_d60) + local_df8;
          if ((int)local_ce8[0] < 1) {
            local_d40 = 1;
            do {
              local_ce8[0] = local_ce8[0] + uVar35;
              local_df8 = local_df8 + uVar35;
            } while ((int)local_ce8[0] < 1);
          }
          else {
            local_d40 = 0;
          }
        }
        uVar36 = local_ce8[0];
        if ((char)param_5 == '\0') goto LAB_140015620;
        if ((int)(local_d60 - local_df4) < (int)local_d04) {
          if (_Var39.v == (uint64_t)(int64_t)(int)local_d48) goto LAB_140015a34;
          if ((int)(local_d60 - local_df4) < 0x14) goto LAB_140015a34;
        }
        do {
          while( true ) {
            local_df4 = local_d60;
            local_d48 = _Var39.ft.dwLowDateTime;
            _Var59.v = local_dd0.v;
            local_b08.v = _Var39.v;
            if (!bVar2) {
              if (local_d00 == 0x4b0) {
                *(uint16_t *)(uintptr_t)((longlong)((uintptr_t)local_dd0.v + 2) + (uintptr_t)_Var39.v) = 0;
                *(uint16_t *)(uintptr_t)((longlong)((uintptr_t)local_dd0.v + 2) + (uintptr_t)_Var39.v) = 0;
              }
              else {
                PECMD_StrDupA((WCHAR **)(void *)&local_b18.v,(LPCWSTR)(uintptr_t)local_dd0.v,
                              (ulonglong)_Var39.v,(longlong)local_d00);
                _Var59.v = local_b18.v;
                iVar52 = lstrlenW((LPCWSTR)(uintptr_t)local_b18.v);
                local_b08.v = (uint64_t)((longlong)iVar52 * 2);
              }
            }
            if (local_d9f == '\0') {
              _Var53.v = local_dd0.v;
              if (local_d9e != '\0') {
                _Var59.v = FUN_140063060(_Var59.v);
              }
            }
            else {
              _Var59.v = (uint64_t)(uintptr_t)
                           PECMD_ConvertLfToCrlf((const uint16_t *)(uintptr_t)_Var59.v,&local_b10);
              _Var53.v = local_dd0.v;
            }
            if ((char)param_5 == '\x01') {
              PECMD_DispatchByObjectName(param_1,local_9b0,(longlong)(uintptr_t)local_9b0,
                                         (uint16_t *)(uintptr_t)DAT_14011c638,_Var59.v,_Var59.v);
            }
            else if (bVar2) {
              PECMD_VarWriteLine(param_1,local_d58,(LPCWSTR)(uintptr_t)_Var53.v,
                             (ulonglong)local_b08.v & 0xffffffff,
                             (longlong *)0x0,'\0');
            }
            else {
              PECMD_SetVariable(param_1,local_9b0,(LPCWSTR)(uintptr_t)_Var59.v);
            }
            if ((local_c48 != (LPCWSTR)0x0) && (local_d40 != 0)) {
              PECMD_StrDupAssign((uint16_t **)(void *)&local_930,local_c48);
              PECMD_ProcessScriptBlock((LARGE_INTEGER){ .QuadPart = (long long)(uintptr_t)param_1 },
                            local_930,(longlong *)0x0,(longlong *)0x0,(pthreadmbcinfo)0x0);
              PECMD_FreeStrBuf(&local_930.QuadPart);
            }
LAB_140015a34:
            if (bVar4) goto LAB_140015a5e;
LAB_140015620:
            if (uVar36 != 0) {
              local_d04 = uVar36;
            }
            if (((int)local_d08 < 2) || (local_d08 - 1 != local_9a0)) break;
            bVar4 = true;
            if (((char)param_5 == '\0') || (_Var39.v == (uint64_t)(int64_t)(int)local_d48))
            goto LAB_140015a5e;
            local_d40 = 1;
          }
          if (((local_ddc & 1) != 0) && ((longlong)local_978.v < (uintptr_t)_Var39.v)) {
            lVar40 = (longlong)_Var39.v - (longlong)local_978.v;
            puVar1 = (undefined1 *)((longlong)local_978.v + (uintptr_t)local_dd0.v);
            uVar25 = local_aa8;
            lVar61 = lVar40;
            if (local_de0 == 0) {
              if ((local_ddc & 0x200) == 0) {
                _Var59.v = lpString.v;
                if (0 < lVar40) {
                  do {
                    if ((puVar1[(longlong)_Var59.v] == '\n') &&
                       (uVar25 = uVar25 - 1, (longlong)uVar25 < 1)) goto LAB_140015a5e;
                    _Var59.v = _Var59.v + 1;
                  } while ((longlong)_Var59.v < lVar40);
                }
              }
              else {
                _Var59.v = lpString.v;
                if (0 < lVar40) {
                  do {
                    if ((puVar1[(longlong)_Var59.v] == '\n') &&
                       (uVar25 = uVar25 - 1, (longlong)uVar25 < 1)) {
                      psVar33 = (short *)((longlong)_Var59.v + -1);
                      if ((0 < (longlong)psVar33) &&
                         (*(char *)((longlong)psVar33 + (longlong)puVar1) == '\r')) {
                        psVar33 = (short *)((longlong)_Var59.v + -2);
                      }
                      *(undefined1 *)((longlong)psVar33 + (longlong)puVar1) = 0;
                      _Var39.v = (uint64_t)((longlong)local_978.v + (longlong)psVar33);
                      bVar16 = false;
                      break;
                    }
                    _Var59.v = _Var59.v + 1;
                  } while ((longlong)_Var59.v < lVar40);
                }
              }
            }
            else {
              do {
                lVar9 = lVar61;
                lVar61 = lVar9 + -1;
                if (lVar61 < 0) goto LAB_14001578b;
              } while ((puVar1[lVar61] != '\n') || (uVar25 = uVar25 - 1, 0 < (longlong)uVar25));
              PECMD_MemMoveSafe(puVar1,(longlong)(puVar1 + lVar9),((int)lVar40 - (int)lVar9) + 1);
              _Var39.v = (longlong)_Var39.v - lVar9;
              puVar1[(longlong)_Var39.v - (longlong)local_978.v] = 0;
            }
LAB_14001578b:
            if (local_dbc != 0) {
              ReadFile(local_bf0,(void *)((longlong)local_dd0.v + (uintptr_t)_Var39.v),
                       (DWORD)local_9c0,&local_de8,&local_8f0);
            }
          }
          iVar52 = 5;
          while( true ) {
            _Var56.v = local_d18.v;
            iVar52 = iVar52 + -1;
            if (((iVar52 < 0) || (DAT_14013a24f < '\x01')) ||
               (BVar20 = PeekMessageW(&local_6e8,(HWND)0x0,0,0,1), BVar20 == 0)) goto LAB_1400153e3;
            if (local_6e8.message == 0x12) break;
            TranslateMessage(&local_6e8);
            DispatchMessageW(&local_6e8);
          }
          local_e07 = '\0';
          bVar4 = true;
          _Var56.v = local_d18.v;
        } while (((char)param_5 != '\0') &&
                (uVar36 = local_ce8[0], _Var39.v != (uint64_t)(int64_t)(int)local_d48));
        goto LAB_1400153e3;
      }
      if (local_e00 != 0xffffff9c) {
        local_d18.v = (uint64_t)(int64_t)(int)local_c80;
        local_c48 = (LPCWSTR)(longlong)(int)local_c28;
LAB_140015a9b:
        local_e07 = DAT_14013a24f;
        if ((DAT_14013a24f < '\x01') && ((longlong)local_d18.v < 2)) goto LAB_140015c69;
        DVar21 = 1000;
        if (0 < (longlong)local_c48) {
          DVar21 = GetTickCount();
          if (local_c28 <= DVar21 - local_db0) goto LAB_140015c69;
          uVar35 = local_c28 - (DVar21 - local_db0);
          DVar21 = 1000;
          if (uVar35 < 1000) {
            DVar21 = uVar35;
          }
        }
        DVar11 = local_d08;
        /* 原 PECMD_MsgWaitForObjects 包装内联 (同上 TODO(verify)) */
        iVar52 = (int)MsgWaitForMultipleObjects(local_d08,local_970,0,DVar21,0x4ff);
        if (DVar11 - 1 == iVar52) goto LAB_140015c69;
        if ((1 < (int)DVar11) && (iVar52 == 0)) {
          _Var59.v = _Var56.v;
          if ((longlong)_Var56.v <= (longlong)((uintptr_t)_Var39.v + 0x100000)) {
            _Var59.v = _Var56.v + 0x100000;
            PECMD_GrowByteBuffer((void **)&local_dd0.v,(longlong)_Var56.v + 0x100004);
          }
          local_d98[0] = 0;
          BVar20 = ReadFile(local_bf0,(void *)((longlong)local_dd0.v + (uintptr_t)_Var39.v),1,
                            local_d98,(LPOVERLAPPED)0x0);
          _Var56.v = _Var59.v;
          if ((BVar20 != 0) && (local_d98[0] != 0)) {
            _Var39.v = (longlong)_Var39.v + (ulonglong)local_d98[0];
            *(uint8_t *)(uintptr_t)((longlong)local_dd0.v + (uintptr_t)_Var39.v) = 0;
          }
        }
        while ((('\0' < DAT_14013a24f &&
                (BVar20 = PeekMessageW(&local_718,(HWND)0x0,0,0,1), BVar20 != 0)) &&
               (local_718.message != 0x12))) {
          TranslateMessage(&local_718);
          DispatchMessageW(&local_718);
        }
        goto LAB_140015a9b;
      }
      goto LAB_140015c69;
    }
  }
LAB_140014f8e:
  pvVar32 = local_c70;
  DVar21 = _Var43.ft.dwLowDateTime;
LAB_140014f96:
  if (param_8 != (uint64_t *)0x0) {
    *param_8 = (uint64_t)(uintptr_t)local_af8.hProcess;
  }
  local_af8.hProcess = (HANDLE)0x0;
  if (((bVar7) && (local_af8.dwProcessId != 0)) && (DAT_14013cb09 == '\0')) {
    DAT_14013cb09 = '\x01';
    if (DVar21 == 0) {
      PECMD_AllocWStringBuffer((WCHAR **)&local_c40.v,0x1000);
      local_d68.v = (ulonglong)local_d68.v & 0xffffffff00000000;
      *(WCHAR *)(uintptr_t)local_c40.v = L'\0';
      p_Var38 = &local_c40.v;
      PECMD_ReadRegBinaryGuarded((HKEY)0xffffffff80000002,
                    (LPCWSTR)L"SYSTEM\\CurrentControlSet\\Control",
                    (LPCWSTR)L"SystemStartOptions",(longlong *)p_Var38,
                    &local_d68.ft.dwLowDateTime,(void *)0x0);
      local_bf8.v = local_c40.v;
      PECMD_SkipLeadingControlChars((long long *)&local_bf8.v);
      while (*(short *)(uintptr_t)local_bf8.v != 0) {
        iVar52 = StrCmpNIW((LPCWSTR)L"PECMDHB=",(LPCWSTR)(uintptr_t)local_bf8.v,8);
        if ((iVar52 == 0) ||
            (iVar52 = StrCmpNIW((LPCWSTR)L"WIMHB=",(LPCWSTR)(uintptr_t)local_bf8.v,6), iVar52 == 0)) {
          local_e00 = 1;
          break;
        }
        PECMD_SkipTokenWs((int64_t *)&local_bf8.v);
      }
      PECMD_FreeStrBuf((WCHAR **)&local_c40.v);
      if (local_e00 == 0) {
        wsprintfW(local_308,(const unsigned short *)
                  L" -nfb -incmd pecmd TEAM WAIT -InitSys* -ncd *%lu",
                  local_af8.dwProcessId);   /* 原 local_af8._16_8_&0xffffffff */
        PECMD_TlsLogWrite((uint64_t)(uintptr_t)param_1,
                          (LPCWSTR)L"DELAY bwinpeshl  [%s]\r\n",
                          (uint64_t)(uintptr_t)local_308,(uint64_t)(uintptr_t)p_Var38);
        local_d78 = 0;
        PECMD_ExecCmdDispatch((uint64_t)(uintptr_t)param_1,(uint64_t)(uintptr_t)local_308,
                      (uint64_t)(uintptr_t)param_1,
                      (uint64_t)(uintptr_t)&local_d78,0,0,0,
                      (uint64_t)(uintptr_t)&g_hwndC9C8);
        goto LAB_1400162b9;
      }
    }
    iVar52 = PECMD_IsSysStartuped();
    if (iVar52 != 0) {
      PECMD_RunSysInit(param_1,(LPCWSTR)L"exec");
    }
    PECMD_StartOnlyApp((LPCWSTR)0x0);
  }
LAB_1400162b9:
  if ((local_d30.QuadPart != 0) && (*(short *)(uintptr_t)local_d30.QuadPart != 0)) {
    PECMD_ScheduleSelfDelete(local_d30,(int)local_af8.dwProcessId);
  }
  if (local_cf0 != (HANDLE)0x0) {
    CloseHandle(local_cf0);
  }
  local_cf0 = (HANDLE)0x0;
  if (local_c88.v != 0) {
    CloseHandle((HANDLE)(uintptr_t)local_c88.v);
  }
  local_c88.ft.dwLowDateTime = 0;
  local_c88.ft.dwHighDateTime = 0;
  if (local_c20.v != 0) {
    CloseHandle((HANDLE)0x0);
  }
  local_c20.ft.dwLowDateTime = 0;
  local_c20.ft.dwHighDateTime = 0;
  if (local_be0.v != 0) {
    CloseHandle((HANDLE)(uintptr_t)local_be0.v);
  }
  local_be0.ft.dwLowDateTime = 0;
  local_be0.ft.dwHighDateTime = 0;
  if (local_bf0 != (HANDLE)0x0) {
    CloseHandle(local_bf0);
  }
  local_bf0 = (HANDLE)0x0;
  if (local_b20.v != 0) {
    CloseHandle((HANDLE)(uintptr_t)local_b20.v);
  }
  local_b20.ft.dwLowDateTime = 0;
  local_b20.ft.dwHighDateTime = 0;
  if (local_8f0.hEvent != (HANDLE)0x0) {
    CloseHandle(local_8f0.hEvent);
  }
  if (local_af8.hThread != (HANDLE)0x0) {
    CloseHandle(local_af8.hThread);
  }
  local_af8.hThread = (HANDLE)0x0;
  PECMD_NotifyMainWindowRefresh((uint64_t)(uintptr_t)param_1,0);
  _Var39.ft.dwHighDateTime = 0;
  _Var39.ft.dwLowDateTime = local_df8;
  PECMD_FreeStrBuf((WCHAR **)&local_b88.v);
  PECMD_FreeStrBuf((WCHAR **)&local_c50.v);
  if ((pvVar45 != (HANDLE)0x0) && (pvVar45 != (HANDLE)0xffffffffffffffff)) {
    CloseHandle(pvVar45);
  }
  PECMD_FreeStrBuf((WCHAR **)&local_c60.v);
  if ((local_d70.v != 0) && (local_d70.v != 0xffffffffffffffff)) {
    CloseHandle((HANDLE)(uintptr_t)local_d70.v);
  }
  local_d70.ft.dwLowDateTime = 0;
  local_d70.ft.dwHighDateTime = 0;
  if ((local_d80 != (HANDLE)0x0) && (local_d80 != (HANDLE)0xffffffffffffffff)) {
    CloseHandle(local_d80);
  }
  local_d80 = (HANDLE)0x0;
  if ((local_c20.v != 0) && (local_c20.v != 0xffffffffffffffff)) {
    CloseHandle((HANDLE)(uintptr_t)local_c20.v);
  }
  if ((local_c88.v != 0) && (local_c88.v != 0xffffffffffffffff)) {
    CloseHandle((HANDLE)(uintptr_t)local_c88.v);
  }
  if ((local_cf0 != (HANDLE)0x0) && (local_cf0 != (HANDLE)0xffffffffffffffff)) {
    CloseHandle(local_cf0);
  }
  if ((local_b20.v != 0) && (local_b20.v != 0xffffffffffffffff)) {
    CloseHandle((HANDLE)(uintptr_t)local_b20.v);
  }
  if ((local_be0.v != 0) && (local_be0.v != 0xffffffffffffffff)) {
    CloseHandle((HANDLE)(uintptr_t)local_be0.v);
  }
  if ((local_bf0 != (HANDLE)0x0) && (local_bf0 != (HANDLE)0xffffffffffffffff)) {
    CloseHandle(local_bf0);
  }
  PECMD_FreeStrBuf(&local_d30.QuadPart);
  PECMD_FreeStrBuf((WCHAR **)&local_a68);
  PECMD_FreeStrBuf((WCHAR **)&local_ca8);
  PECMD_FreeStrBuf((WCHAR **)&local_d10.v);
  PECMD_FreeStrBuf((WCHAR **)&local_c78.v);
  PECMD_FreeStrBuf((WCHAR **)(local_ca8 + 0x10));
  PECMD_FreeStrBuf((WCHAR **)&local_c68);
  PECMD_FreeStrBuf((WCHAR **)&local_c58);
  PECMD_FreeStrBuf((WCHAR **)&local_cf8.v);
  if ((pvVar32 != (HANDLE)0x0) && (pvVar32 != (HANDLE)0xffffffffffffffff)) {
    CloseHandle(pvVar32);
  }
  if ((local_920 != (HANDLE)0x0) && (local_920 != (HANDLE)0xffffffffffffffff)) {
    CloseHandle(local_920);
  }
  PECMD_ReleaseObjectResources(&local_a80);
  PECMD_FreeStrBuf(&local_cb0);
  PECMD_FreeStrBuf((WCHAR **)&local_d20);
  PECMD_FreeStrBuf((WCHAR **)&local_d38);
  PECMD_FreeStrBuf((WCHAR **)&local_d58);
  PECMD_FreeStrBuf((WCHAR **)&local_cc0);
  PECMD_FreeStrBuf((WCHAR **)&local_ce0);
  PECMD_FreeStrBuf((WCHAR **)&local_cb8.v);
  PECMD_FreeStrBuf((WCHAR **)&local_cc8.v);
LAB_14001668b:
  PECMD_FreeStrBuf(&local_a10.QuadPart);
  PECMD_FreeStrBuf((WCHAR **)&local_ac0);
  return ((uint64_t)(uint32_t)_Var39.ft.dwLowDateTime) |
         ((uint64_t)_Var39.ft.dwHighDateTime << 32);
}

/* PECMD_LangLookupById — 按参数序号在 argv 表中定位 VALUE (形如 "id=value|id=value|...")
   定位到匹配 id 时返回其值指针(跳过 '|' 前缀); 未命中返回 NULL.
   依赖: FUN_1400166b4 初始化 argv 表; PECMD_ParseNumTryWriteback 数值解析; lstrlenW. */
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
            if (PECMD_ParseNumTryWriteback((long long *)&local_res20, local_res18) &&
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
void PECMD_ServiceMainEntry(undefined8 param_1, undefined8 *param_2){
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

/* @0x1400171a4 size=— 授予当前用户内核对象访问(直移) */
void PECMD_GrantCurrentUserAccess(int64_t a)
{
  void *dacl = 0, *sd = 0, *newdacl = 0;
  uint64_t r = GetSecurityInfo((void *)(uintptr_t)a,SE_KERNEL_OBJECT,4,0,0,&dacl,0,&sd);
  if (r != 0) { if ((uintptr_t)sd != 0) LocalFree((uint64_t)(uintptr_t)sd); if ((uintptr_t)dacl != 0) LocalFree((uint64_t)(uintptr_t)dacl); }
  EXPLICIT_ACCESS_W ea; memset(&ea,0,0x30);
  ea.grfInheritance = 0;
  ea.grfAccessPermissions = 2;
  ea.Trustee.ptstrName = (void *)(uintptr_t)L"CURRENT_USER";
  ea.grfAccessMode = GRANT_ACCESS;
  ea.Trustee.TrusteeForm = TRUSTEE_IS_NAME;
  ea.Trustee.TrusteeType = TRUSTEE_IS_USER;
  r = SetEntriesInAclW(1,&ea,dacl,&newdacl);
  if (r != 0) { if ((uintptr_t)sd != 0) LocalFree((uint64_t)(uintptr_t)sd); if ((uintptr_t)newdacl != 0) LocalFree((uint64_t)(uintptr_t)newdacl); }
  r = SetSecurityInfo((void *)(uintptr_t)a,SE_KERNEL_OBJECT,4,0,0,newdacl,0);
  if (r != 0) { if ((uintptr_t)sd != 0) LocalFree((uint64_t)(uintptr_t)sd); if ((uintptr_t)newdacl != 0) LocalFree((uint64_t)(uintptr_t)newdacl); }
}
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
void PECMD_NtShutdownSystemCall(int param_1)   /* @0x14001a56c 经 NTDLL.NtShutdownSystem 关机 (decompiled.c 直移) */
{
    HMODULE hModule;
    FARPROC pFVar1;

    hModule = GetModuleHandleW((const WCHAR *)L"NTDLL");
    pFVar1 = GetProcAddress(hModule, "NtShutdownSystem");
    if (pFVar1 != (FARPROC)0) {
        ((int (*)(ulonglong))(uintptr_t)pFVar1)((ulonglong)((param_1 != 1) + 1));
    }
}
void PECMD_SetCurrentDirIfChanged(LPCWSTR param_1)   /* @0x14001a640 切换当前目录 (decompiled.c 直移) */
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
undefined4 PECMD_LineIsTeamExecLoad(const WCHAR *param_1){
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
void PECMD_RunClearTmpMbrosOnce(void *script)   /* @0x14001b660 定时清理注册表: 一次性 "ClearTmpMBROS" (decompiled.c) */
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
uint64_t PECMD_InstallKeyboardHook(void) { return 0; }
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
    PECMD_AllocWStringBuffer((void *)local_res8, 0x20a);
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
    PECMD_FreeStrBuf((WCHAR **)&local_res8);
    LeaveCriticalSection((void *)&DAT_14013e190);
}
uint64_t PECMD_AddVarDefault(void *script, LPCWSTR name, LPCWSTR val, int len, int64_t flag) { (void)script;(void)name;(void)val;(void)len;(void)flag; return 0; }   /* arity 修正 0->5 (PECMD_ExecCmdDispatch 恢复体) */
int64_t PECMD_FindVarValue(int64_t *a, LPCWSTR b, int64_t *c, int d) { (void)a;(void)b;(void)c;(void)d; return 0; }   /* arity 修正 0->4 (PECMD_ExecCmdDispatch 恢复体) */
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
    PECMD_FreeStrBuf((WCHAR **)&local_30);
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
    PECMD_RelaunchLoadFirstUsb();
    memset(buf,0,0x2d8);
    memcpy(p,(const unsigned short *)L"#22:INDATA*AutoMount ",0x2c);
    if ((uintptr_t)param_2 != 0) lstrcatW((void *)(p+0x10),(uint64_t)param_2);
    (void)FUN_140031454(param_1,(pthreadmbcinfo)p);
  }
}
/* ---- R1 批: FUN_140025f10 (PECMD_AppendLogMessage) 还原依赖前置声明/桩 ---- */
extern void PECMD_CrtShim(WCHAR *out, size_t fmt, ...);        /* core_b1_remaining.c 定义 (void SKIP 桩, wsprintf 阴影) */
extern void FUN_140063720(longlong **ps, longlong len);         /* def 本文件后部 5254 有 decl */
extern void FUN_1400185c8(char a, longlong *b);                 /* def 本文件后部 (日志落盘) */
extern uint64_t QueryPerformanceCounter(void *out);             /* 本文件后部 def 改带出参签名 */
extern int64_t g_QPFreq;                                        /* DAT_14013cb80 (core_globals.c) */
/* FormatMessageW 库内原缺 → 新增最小桩 (返回 0 → 调用方走 "\r\n" 兜底路径) */
uint32_t FormatMessageW(uint32_t flags, const void *src, uint32_t msg, uint32_t lang,
                        WCHAR *buf, uint32_t size, void *args)
{ (void)flags;(void)src;(void)msg;(void)lang;(void)buf;(void)size;(void)args; return 0; }
/* rename_map: FUN_14001d78c→PECMD_MemMoveForward (MSVC 内联 memmove/memcpy, 实现本文件后部
   @13044) → 一字转发保持反编译符号视图 */
void FUN_14001d78c(unsigned char *dst, const unsigned char *src, longlong n)
{
  PECMD_MemMoveForward(dst,src,(int)n);
}
/* @0x140025f10 size=1062 — 日志行拼装 (PECMD_AppendLogMessage, decompiled.c @22907 直移 +
   汇编核对 0x1400260b5-0x14002631a):
   DAT_14013cb18 结构=[0x8]=缓冲基址 [0x10]=写游标(字符数) [0x18]=容量;
   param_4 低 32 位为标志位: 0x20 位→时间戳头(QPC/g_QPFreq 秒.微秒.纳秒+Tid, 格式
   0x140120790 "%lu\t%u.%06lu%06lu\t%s"), 否则仅前缀串(0x140120784 "%s"); 前缀来自
   0x1401207c0 "%ld\t"(param_5 遍历计数, 汇编: 基址=*(param_1+0x80), 界=+count*2 —
   Ghidra 的 ptVar10->mbulinfo 为伪字段); 0x1010 位使括号 '[' 变 '{'; 尾括号 ']'/'}'
   仅在 (flags&0x2020)==0 时写; param_4 低字节==0 时经 0x140120770 "\t<%lu>\t:" 写
   错误码并 FormatMessageW(FORMAT_MESSAGE_FROM_SYSTEM) 追加系统消息, 否则补 "\r\n";
   游标>0x10000 或 [struct+0x29]!=0 → FUN_1400185c8('\x01') 落盘.
   取舍: PECMD_CrtShim 为 void SKIP 桩 → 其返回值(写字符数)按 0 建模(先例 ls@6070),
   g_QPFreq 未初始化时时间戳各分量按 0 (运行时槽判空纪律). */
void FUN_140025f10(longlong param_1,LPCWSTR param_2,uint32_t param_3,char *param_4,
                   char *param_5,long long *param_6)
{
  bool bVar3;
  bool bVar4;
  int iVar5;
  int iVar7;
  DWORD DVar6;
  longlong *plVar8;
  ulonglong uVar9;
  char *ptVar10;
  longlong *plVar15;
  WCHAR *lpBuffer;
  WCHAR *pwVar1;
  WCHAR *pWVar11;
  int ptVar12;
  longlong lVar13;
  longlong lVarBase;
  longlong lVarEnd;
  WCHAR *pWVarScan;
  uint16_t mVar88;
  uint16_t mVar8a;
  uint16_t mVar90;
  uint uVar16;
  LARGE_INTEGER local_60;
  uint64_t qRem;
  uint32_t dUsec;
  uint32_t dNsec;
  WCHAR local_58[64];

  ptVar10 = param_4;
  EnterCriticalSection((void *)&DAT_14013e190);
  plVar15 = (longlong *)DAT_14013cb18;
  if (param_6 == (long long *)0x0) {
    plVar8 = (longlong *)(uintptr_t)TlsGetValue(DAT_14013c934);
    if ((plVar8 != (longlong *)0x0) && (*plVar8 != 0)) {
      plVar15 = plVar8;
    }
  }
  else if (param_6 != (long long *)0xffffffffffffffff) {
    plVar15 = (longlong *)param_6;
  }
  uVar16 = (uint)(uintptr_t)param_4;
  bVar3 = false;
  bVar4 = false;
  if (((*(uint8_t *)((uintptr_t)param_1 + 0x10) & 0x20) != 0) || ((uVar16 >> 0x10 & 1) != 0)) {
    bVar4 = true;
  }
  if (((*(uint8_t *)((uintptr_t)param_1 + 0x10) & 0x40) != 0) || ((uVar16 >> 0x11 & 1) != 0)) {
    bVar3 = true;
  }
  local_58[0] = L'\0';
  iVar5 = lstrlenW(param_2);
  if ((int)plVar15[3] - (int)plVar15[2] < iVar5 + 0x532) {
    lVar13 = (longlong)iVar5 + 0x1049c + plVar15[2];
    plVar15[3] = lVar13;
    FUN_140063720((longlong **)(plVar15 + 1),lVar13);
  }
  ptVar12 = 0;
  if (((ulonglong)uVar16 & 0x2020ULL) == 0) {
    /* 前缀计数扫描: 汇编证实 基址=*(u64*)(param_1+0x80), 上界=基址+(int)*(int*)(param_1+0x8c)*2 */
    if (((param_5 != (char *)0x0) && bVar3)) {
      lVarBase = *(longlong *)((uintptr_t)param_1 + 0x80);
      if ((char *)lVarBase <= param_5) {
        lVarEnd = lVarBase + (longlong)(int)*(int *)((uintptr_t)param_1 + 0x8c) * 2;
        if (param_5 < (char *)lVarEnd) {
          if (!((ulonglong)lVarBase > (ulonglong)(intptr_t)param_5)) {
            mVar88 = *(uint16_t *)((uintptr_t)param_1 + 0x88);
            mVar8a = *(uint16_t *)((uintptr_t)param_1 + 0x8a);
            mVar90 = *(uint16_t *)((uintptr_t)param_1 + 0x90);
            pWVarScan = (WCHAR *)(uintptr_t)lVarBase;
            do {
              while (*pWVarScan != mVar88 && *pWVarScan != mVar8a && *pWVarScan != mVar90) {
                pWVarScan++;
              }
              if (*pWVarScan == mVar8a) pWVarScan++;
              if (*pWVarScan == mVar90) pWVarScan++;
              ptVar12 = ptVar12 + 1;
            } while ((char *)pWVarScan <= param_5);
          }
          PECMD_CrtShim(local_58,(size_t)0x1401207c0 /* L"%ld\t" */,
                        (void *)(uintptr_t)ptVar12,(void *)param_5);
        }
      }
    }
    if (bVar4) {
      QueryPerformanceCounter(&local_60);
      if (g_QPFreq != 0) {
        uVar9 = (ulonglong)local_60.QuadPart / (ulonglong)g_QPFreq;
        qRem  = (ulonglong)local_60.QuadPart % (ulonglong)g_QPFreq;
        dUsec = (uint32_t)((qRem * 1000000ULL) / (ulonglong)g_QPFreq);
        dNsec = (uint32_t)(((qRem * 1000000ULL) % (ulonglong)g_QPFreq) * 1000000ULL / (ulonglong)g_QPFreq);
      }
      else { uVar9 = 0; qRem = 0; dUsec = 0; dNsec = 0; }   /* 频率槽未装载 → 按 0 判空 */
      DVar6 = GetCurrentThreadId();
      /* 汇编 vararg 布局: %lu=Tid(r8d) %u=秒低32(r9d) .%06lu=微秒 %06lu=纳秒 %s=&local_58 */
      PECMD_CrtShim((WCHAR *)(uintptr_t)(plVar15[1] + plVar15[2] * 2),
                    (size_t)0x140120790 /* L"%lu\t%u.%06lu%06lu\t%s" */,
                    (void *)(uintptr_t)DVar6,(void *)(uintptr_t)(uint32_t)uVar9,
                    (void *)(uintptr_t)dUsec,(void *)(uintptr_t)dNsec,(void *)local_58);
      iVar7 = 0;   /* CrtShim void SKIP 桩: 写字符数按 0 建模 */
    }
    else {
      iVar7 = 0;
      PECMD_CrtShim((WCHAR *)(uintptr_t)(plVar15[1] + plVar15[2] * 2),
                    (size_t)0x140120784 /* L"%s" */,(void *)local_58,param_5);
    }
    ptVar10 = param_5;
    plVar15[2] = plVar15[2] + (longlong)(iVar7 + 1);
    *(uint16_t *)(uintptr_t)(plVar15[1] + -2 + plVar15[2] * 2) =
         (uint16_t)(int16_t)(int8_t)((-(((ulonglong)uVar16 & 0x1010ULL) != 0) & 0x20U) + 0x5b);
  }
  lVar13 = (longlong)iVar5;
  FUN_14001d78c((unsigned char *)(uintptr_t)(plVar15[1] + plVar15[2] * 2),
                (const unsigned char *)param_2,lVar13 * 2);
  plVar15[2] = plVar15[2] + lVar13;
  lVar13 = plVar15[2];
  if (((ulonglong)uVar16 & 0x2020ULL) == 0) {
    plVar15[2] = lVar13 + 1;
    *(uint16_t *)(uintptr_t)(plVar15[1] + lVar13 * 2) =
         (uint16_t)(int16_t)(int8_t)((-(((ulonglong)uVar16 & 0x1010ULL) != 0) & 0x20U) + 0x5d);
  }
  *(uint16_t *)(uintptr_t)(plVar15[1] + plVar15[2] * 2) = 0;
  lVar13 = plVar15[2];
  if ((char)(uintptr_t)param_4 == '\0') {
    pwVar1 = (WCHAR *)(uintptr_t)(plVar15[1] + lVar13 * 2);
    PECMD_CrtShim(pwVar1,(size_t)0x140120770 /* L"\t<%lu>\t:" */,
                  (void *)(uintptr_t)param_3,(void *)ptVar10);
    iVar7 = 0;   /* CrtShim 返回值按 0 */
    lpBuffer = pwVar1;
    if (0 < iVar7) {
      lpBuffer = pwVar1 + iVar7;
    }
    DVar6 = FormatMessageW(0x1200,(LPCWSTR)(uintptr_t)0x0,param_3,0,lpBuffer,0x400,(va_list *)0x0);
    if ((int)DVar6 < 1) {
      *lpBuffer = L'\r';
      pWVar11 = lpBuffer + 2;
      lpBuffer[1] = L'\n';
    }
    else {
      pWVar11 = lpBuffer + (int)DVar6;
    }
    plVar15[2] = plVar15[2] + ((longlong)(intptr_t)pWVar11 - (longlong)(intptr_t)pwVar1 >> 1);
    *(uint16_t *)(uintptr_t)(plVar15[1] + plVar15[2] * 2) = 0;
  }
  else {
    plVar15[2] = lVar13 + 1;
    *(uint16_t *)(uintptr_t)(plVar15[1] + lVar13 * 2) = 0xd;
    lVar13 = plVar15[2];
    plVar15[2] = lVar13 + 1;
    *(uint16_t *)(uintptr_t)(plVar15[1] + lVar13 * 2) = 10;
    *(uint16_t *)(uintptr_t)(plVar15[1] + plVar15[2] * 2) = 0;
  }
  if ((0x10000 < plVar15[2]) || (*(int8_t *)((char *)(uintptr_t)((intptr_t)plVar15 + 0x29)) != 0)) {
    FUN_1400185c8('\x01',(longlong *)param_6);
  }
  LeaveCriticalSection((void *)&DAT_14013e190);
}
/* ============================================================
 * PECMD_ScriptInitParse @0x140026338 (size=4875) — 命令串重定向/日志输出控制 (decompiled.c 直移)
 *   signature: ulonglong __fastcall PECMD_ScriptInitParse(longlong * param_1, LPCWSTR param_2, pthreadmbcinfo param_3)
 *   按现桩签名 (uint64_t/uint64_t/uint64_t) 表达, 内部重绑定为类型化别名.
 * ============================================================ */
/* ---- 移植前置: 类型 / 全局符号 / 前置声明 ---- */
typedef struct { SHORT X, Y; } COORD;
typedef struct { SHORT Left, Top, Right, Bottom; } SMALL_RECT;
extern int32_t  g_msgLockCount;            /* DAT_14013d080 (core_globals.c) */
extern uint8_t  g_u8CCB1;                  /* DAT_14013ccb1 (core_globals.c) */
extern uint8_t  g_flagD6F6;                /* DAT_14013d6f6 (core_globals.c) */
extern int64_t  g_i64CCB8;                 /* DAT_14013ccb8 (core_globals.c) */
extern uint8_t  DAT_14011c638[64];         /* 默认命令串 (定义见本文件后部) */
int      wsprintfW(unsigned short *out, const unsigned short *fmt, ...);   /* 定义见后部 (def @3623) */
int      FUN_140003864(void *a, const uint16_t *b, uint32_t c, uint32_t d, void *e, uint32_t f, uint32_t g, void *h);  /* def @6340 (int 返回: 1062 行隐式调用在先) */
unsigned short *lstrcpyW(unsigned short *dst, const unsigned short *src);  /* def 见后部 */
COORD    GetLargestConsoleWindowSize(void *h);                             /* 新增桩 (定义紧随本函数之后) */
uint64_t GetVolumeNameForVolumeMountPointW(void *mount, void *buf, unsigned long n);  /* def 见后部 */
uint64_t SetFilePointerEx(void *h, longlong off, longlong *out, unsigned long method); /* def 见后部 */
void     FUN_1400185c8(char a, longlong *b);                               /* def @7358 */
extern void *PECMD_StrBldCopyWide(void *a, const WCHAR *b);                      /* def @3058 */
void     FUN_140063720(longlong **_ps, longlong len);                      /* def @6922 */
undefined8 FUN_14005c788(char *a, ushort *b, int c);                       /* def @7354 */
longlong *PECMD_AppendWideStr(void *param_1p, LPCWSTR param_2);            /* def @2802 */
char      PECMD_MatchTokenAdvance(char *tok, void *pp, int n);             /* def @2866 */
void      PECMD_StrAssign(uint16_t **ps, const uint16_t *src);             /* def @4836 */
long long PECMD_ExpandCommandLine(long long *a, WCHAR *b, void *c, int d, uint8_t e);  /* def @3129 */
uint64_t  PECMD_EvalParenthesizedExpr(int64_t *pp, uint64_t *out);                       /* 真实体 core_b7a.c */
void     *FUN_14001E5B0(void *script, LPCWSTR name, LPCWSTR value, int namelen, int64_t caplen); /* core_var2.c */
uint8_t  *FUN_14001E69C(void *script, LPCWSTR name, void *scope, int64_t len);   /* core_var3.c */
DWORD     FUN_14006459C(LPCWSTR src, uint32_t buflen, LPWSTR buf, LPWSTR *last); /* core_exec2.c */
int64_t   PECMD_DescribePartitionInfo(int64_t *param_1, LPCWSTR param_2, uint64_t param_3, uint64_t param_4, LARGE_INTEGER *param_5, uint32_t *param_6, char param_7); /* core_b3_remaining.c */
void      FUN_140060A94(uint64_t flags);                                   /* core_b3j.c */
uint64_t *PECMD_InitStringObj(uint64_t *obj);                              /* core_b1_remaining.c */
int32_t   FUN_14005C7C4(const char *a, const uint16_t *b);                 /* 真实体 core_exec5.c */
undefined8 *FUN_14001877C(longlong *ps, int count);                        /* 新增最小桩 (定义紧随本函数之后) */

uint64_t PECMD_ScriptInitParse(uint64_t a, uint64_t b, uint64_t c)
{
  longlong *param_1 = (longlong *)a;
  LPCWSTR param_2 = (LPCWSTR)b;
  pthreadmbcinfo param_3 = (pthreadmbcinfo)c;
  LPWSTR pWVar1;
  byte bVar2;
  undefined1 uVar3;
  WCHAR WVar4;
  bool bVar5;
  bool bVar6;
  bool bVar7;
  bool bVar8;
  WCHAR WVar9;
  LPWSTR lpszVolumeName;
  byte bVar10;
  char cVar11;
  int iVar12;
  BOOL BVar13;
  COORD CVar14;
  longlong *plVar15;
  undefined8 uVar16;
  LPCWSTR pWVar17;
  longlong *plVar18;
  undefined8 *puVar19;
  WCHAR *pWVar20;
  longlong lVar21;
  uint uVar22;
  DWORD DVar23;
  uint uVar24;
  ulonglong uVar25;
  ulonglong uVar26;
  undefined8 *puVar27;
  byte bVar28;
  byte bVar29;
  longlong *plVar30;
  uint uVar31;
  longlong *plVar32;
  bool bVar34;
  undefined8 local_res8;
  LPCWSTR local_res10;
  pthreadmbcinfo local_res18;
  byte local_res20;
  char local_f7;
  byte local_f5;
  char local_f4;
  byte local_f3;
  byte local_f2;
  byte local_f1;
  undefined1 local_f0;
  bool local_ef;
  bool local_ee;
  char local_eb;
  bool local_e9;
  DWORD local_e8;
  uint local_e0;
  SMALL_RECT local_d8;
  longlong *local_d0;
  uint local_c8;
  undefined4 local_c0;
  undefined2 local_bc;
  undefined2 local_ba;
  WCHAR local_b8;
  LPCWSTR local_b0;
  ulonglong local_a8;
  WCHAR *local_a0;
  WCHAR *local_98;
  ulonglong local_90;
  LPCWSTR local_88;
  LPCWSTR local_80;
  LPCWSTR local_78;
  LPWSTR local_70;
  WCHAR *local_68;
  LPCWSTR local_60;
  LARGE_INTEGER local_58;
  LARGE_INTEGER local_50 [2];
  ulonglong uVar33;

  local_80 = param_2 + -4;
  local_res8 = (undefined8)(uintptr_t)(WCHAR *)param_1;
  local_res10 = param_2;
  local_res18 = param_3;
  FUN_1400170b0((void **)&local_res10);
  cVar11 = '*';
  if (*local_res10 == L'-') {
    cVar11 = '-';
  }
  local_b8 = (short)cVar11;
  EnterCriticalSection((LPCRITICAL_SECTION)&DAT_14013e190);
  uVar25 = 0;
  iVar12 = 0;
  uVar24 = 0;
  if (((*(byte *)((longlong)param_1 + 0x11) >> 1 & 1) != 0 || g_msgLockCount != 0) &&
     (((short)cVar11 != *local_res10 || (local_res10[1] != L'\0')))) {
    LeaveCriticalSection((LPCRITICAL_SECTION)&DAT_14013e190);
    return 0;
  }
  plVar15 = (longlong *)TlsGetValue(DAT_14013c934);
  bVar8 = false;
  local_f7 = '\0';
  local_f4 = '\0';
  bVar6 = false;
  local_res20 = 0;
  bVar7 = false;
  local_f0 = 0;
  local_d8.Left = 0;
  local_d8.Top = 0;
  local_e0 = 0;
  bVar10 = 0;
  WVar4 = *local_res10;
  uVar26 = uVar25;
  local_98 = (WCHAR *)local_res10;
  while ((WVar4 == cVar11 &&
         (iVar12 = (int)uVar26, param_1 = (longlong *)(uintptr_t)local_res8, local_98[1] == (short)cVar11))) {
    pWVar20 = local_98 + 2;
    local_98 = pWVar20;
    uVar16 = FUN_14005c788("thread",(ushort *)pWVar20,6);
    if ((char)uVar16 == '\0') {
      uVar26 = 1;
      local_d8.Left = 1;
      local_d8.Top = 0;
    }
    else {
      uVar26 = (ulonglong)((uint)(uint16_t)local_d8.Left | ((uint)(uint16_t)local_d8.Top << 16));
      if (local_res10[6] == L'+') {
        bVar10 = 3;
      }
      else {
        bVar10 = (byte)((local_res10[6] == L'@') + 1);
      }
    }
    WVar4 = *pWVar20;
    while ((WVar4 != L'\0' && ((((ushort)WVar4 < 9 || (0xd < (ushort)WVar4)) && (WVar4 != L' ')))))
    {
      pWVar20 = pWVar20 + 1;
      local_98 = pWVar20;
      WVar4 = *pWVar20;
    }
    FUN_1400170b0((void **)&local_98);
    iVar12 = (int)uVar26;
    WVar4 = *local_98;
    param_1 = (longlong *)(uintptr_t)local_res8;
  }
  if ((iVar12 == 0) || (*local_98 != L'\0')) {
    bVar5 = false;
  }
  else {
    bVar5 = true;
  }
  pWVar20 = local_98;
  bVar34 = false;
  if (*local_98 == L'*') {
    local_a0 = local_98 + 1;
    local_e0 = 1;
    bVar34 = *local_a0 == L'?';
    if (bVar34) {
      local_a0 = local_98 + 2;
    }
    FUN_1400170b0((void **)&local_a0);
    pWVar20 = local_a0;
  }
  pWVar17 = (LPCWSTR)PECMD_UnquoteString((short *)pWVar20);
  if ((((char)local_e0 == '\0') || (bVar34)) || (*pWVar17 != L'\0')) {
    local_res8 = (undefined8)((uint64_t)local_res8 & 0xffffffffffffff00ULL);
    if (((((uint)(uint16_t)local_d8.Left | ((uint)(uint16_t)local_d8.Top << 16)) == 0) && (*pWVar17 == L'\0')) && (bVar10 == 0)) {
      bVar10 = 3;
    }
  }
  else {
    local_res8 = (undefined8)(((uint64_t)local_res8 & 0xffffffffffffff00ULL) | 1ULL);
  }
  bVar10 = (byte)(-(plVar15 != (longlong *)0x0) & bVar10);
  plVar30 = (longlong *)&DAT_14013cb18[0];
  local_d0 = (longlong *)(uintptr_t)0xffffffffffffffff;
  if ((bVar10 & 1) != 0) {
    plVar30 = plVar15;
    local_d0 = plVar15;
  }
  if (((*(byte *)(param_1 + 2) & 1) == 0) || (bVar34 = true, *plVar30 == 0)) {
    bVar34 = false;
  }
  local_e9 = ((ulonglong)local_res18 & 1) != 0;
  if (local_e9) {
    local_res18 = (pthreadmbcinfo)((ulonglong)local_res18 & 0xfffffffffffffffeULL);
    bVar34 = false;
  }
  local_e9 = !local_e9;
  local_a8 = 0xfffffffffff80000;
  local_60 = (LPCWSTR)(void *)DAT_14011c638;
  if (local_res18 == (pthreadmbcinfo)0xffffffffffffffff) {
    local_a8 = plVar30[4];
    local_f7 = '\x02';
    local_res18 = (pthreadmbcinfo)(void *)DAT_14011c638;
  }
  local_e8 = 0;
  local_c8 = (local_c8 & 0xffffff00U) | (uint)(*local_res10 != L'\0');
  local_a0 = (WCHAR *)pWVar17;
  PECMD_AllocStrSlot((uint16_t **)&local_b0);
  bVar2 = *(byte *)(param_1 + 2);
  local_70 = (LPWSTR)0x0;
  local_f2 = 0xff;
  local_ee = (bool)(bVar2 >> 2 & 1);
  bVar28 = *(byte *)(param_1 + 2) >> 6 & 1;
  local_f1 = bVar2 >> 1 & 1;
  local_eb = -1;
  local_c0 = 0;
  local_ef = (bool)(*(byte *)(param_1 + 2) >> 3 & 1);
  local_f3 = *(byte *)(param_1 + 2) >> 4 & 1;
  local_f5 = *(byte *)(param_1 + 2) >> 5 & 1;
  EnterCriticalSection((LPCRITICAL_SECTION)&DAT_14013e190);
  WVar9 = local_b8;
  WVar4 = *local_res10;
  uVar26 = uVar25;
  uVar33 = uVar25;
  uVar22 = uVar24;
  uVar31 = local_c0;
  while (WVar4 == WVar9) {
    uVar22 = (uint)uVar26;
    pWVar17 = (LPCWSTR)local_a0;
    uVar31 = (uint)uVar33;
    if (local_res10[1] != WVar9) break;
    local_res10 = local_res10 + 2;
    uVar31 = (uint)uVar33 + 1;
    uVar33 = (ulonglong)uVar31;
    uVar16 = FUN_14005c788("thread",(ushort *)local_res10,6);
    bVar29 = bVar28;
    if ((char)uVar16 == '\0') {
      uVar16 = FUN_14005c788("2",(ushort *)local_res10,1);
      if ((char)uVar16 != '\0') {
        if (local_res10[1] == L'-') {
          local_f3 = 0;
        }
        else if ((*(byte *)((longlong)param_1 + 0x11) >> 1 & 1) == 0 && g_msgLockCount == 0) {
          local_f3 = 1;
        }
        goto LAB_140026cde;
      }
      uVar16 = FUN_14005c788("lock:",(ushort *)local_res10,5);
      if ((char)uVar16 != '\0') {
        local_res10 = local_res10 + 5;
        local_f4 = local_f4 + '\x01';
        local_90 = 0;
        PECMD_EvalParenthesizedExpr((longlong *)&local_res10,&local_90);
        if ((plVar30[4] < 1) || (local_f7 == '\x01')) {
          local_a8 = local_90;
          if ((longlong)local_90 < 1) {
            bVar8 = true;
            local_a8 = 0xfffffffffff80000;
          }
        }
        else if ((local_f4 == '\x01') && (plVar30[4] == local_90)) {
          local_a8 = local_90;
          local_f7 = '\x01';
        }
LAB_14002684d:
        WVar4 = *local_res10;
        while (((WVar4 != L'\0' && (((ushort)WVar4 < 9 || (0xd < (ushort)WVar4)))) &&
               (WVar4 != L' '))) {
          local_res10 = local_res10 + 1;
          WVar4 = *local_res10;
        }
        goto LAB_140026d14;
      }
      uVar16 = FUN_14005c788("ln=",(ushort *)local_res10,3);
      if ((char)uVar16 != '\0') {
        bVar29 = 0;
        if (((0x30 < (ushort)local_res10[3]) && ((ushort)local_res10[3] < 0x3a)) &&
           (bVar29 = bVar28,
           (*(byte *)((longlong)param_1 + 0x11) >> 1 & 1) == 0 && g_msgLockCount == 0)) {
          bVar29 = 1;
        }
        goto LAB_140026cde;
      }
      uVar16 = FUN_14005c788("p",(ushort *)local_res10,1);
      if ((char)uVar16 != '\0') {
        local_ee = local_res10[1] != L'-';
        goto LAB_140026cde;
      }
      uVar16 = FUN_14005c788("np",(ushort *)local_res10,2);
      if ((char)uVar16 != '\0') {
        local_ef = local_res10[2] != L'-';
        goto LAB_140026cde;
      }
      uVar16 = FUN_14005c788("t=",(ushort *)local_res10,2);
      if ((char)uVar16 != '\0') {
        if (((ushort)local_res10[2] < 0x31) || (0x39 < (ushort)local_res10[2])) {
          local_f5 = 0;
        }
        else if ((*(byte *)((longlong)param_1 + 0x11) >> 1 & 1) == 0 && g_msgLockCount == 0) {
          local_f5 = 1;
        }
        goto LAB_140026cde;
      }
      uVar16 = FUN_14005c788("dbg=",(ushort *)local_res10,4);
      if ((char)uVar16 != '\0') {
        if (((ushort)local_res10[4] < 0x31) || (0x39 < (ushort)local_res10[4])) {
          g_u8CCB1 = 0;
        }
        else if ((*(byte *)((longlong)param_1 + 0x11) >> 1 & 1) == 0 && g_msgLockCount == 0) {
          g_u8CCB1 = 1;
        }
        goto LAB_140026cde;
      }
      uVar16 = FUN_14005c788("RAW=",(ushort *)local_res10,4);
      if ((char)uVar16 != '\0') {
        if (((ushort)local_res10[4] < 0x31) || (0x39 < (ushort)local_res10[4])) {
          local_f1 = 0;
        }
        else if ((*(byte *)((longlong)param_1 + 0x11) >> 1 & 1) == 0 && g_msgLockCount == 0) {
          local_f1 = 1;
        }
        goto LAB_140026cde;
      }
      uVar16 = FUN_14005c788("rt=",(ushort *)local_res10,3);
      if ((char)uVar16 != '\0') {
        if (((ushort)local_res10[3] < 0x31) || (local_e0 = 3, 0x39 < (ushort)local_res10[3])) {
          local_e0 = uVar24;
        }
        local_eb = (char)local_e0;
        goto LAB_140026cde;
      }
      cVar11 = PECMD_MatchTokenAdvance("rt",(longlong *)&local_res10,2);
      if ((cVar11 == '\0') &&
         (cVar11 = PECMD_MatchTokenAdvance("rt?",(longlong *)&local_res10,3), cVar11 == '\0')) {
        uVar16 = FUN_14005c788("ON=",(ushort *)local_res10,3);
        if ((char)uVar16 != '\0') {
          uVar26 = (ulonglong)(uVar22 + 1);
          if (((ushort)local_res10[3] < 0x31) || (0x39 < (ushort)local_res10[3])) {
            *(byte *)(param_1 + 2) = *(byte *)(param_1 + 2) & 0xfe;
            local_f2 = 0;
          }
          else if ((*(byte *)((longlong)param_1 + 0x11) >> 1 & 1) == 0 && g_msgLockCount == 0) {
            local_f2 = 1;
          }
          goto LAB_140026cde;
        }
        cVar11 = PECMD_MatchTokenAdvance("a",(longlong *)&local_res10,1);
        if (cVar11 == '\0') {
          cVar11 = PECMD_MatchTokenAdvance("vol",(longlong *)&local_res10,3);
          if (cVar11 == '\0') {
            cVar11 = PECMD_MatchTokenAdvance("vol+",(longlong *)&local_res10,4);
            if (cVar11 == '\0') {
              cVar11 = PECMD_MatchTokenAdvance("part",(longlong *)&local_res10,4);
              if (cVar11 == '\0') {
                cVar11 = PECMD_MatchTokenAdvance("tmp",(longlong *)&local_res10,3);
                if (cVar11 == '\0') {
                  cVar11 = PECMD_MatchTokenAdvance("abak",(longlong *)&local_res10,4);
                  if (cVar11 == '\0') goto LAB_14002684d;
                  local_f0 = 1;
                }
                else {
                  bVar7 = true;
                }
              }
              else {
                local_res20 = 2;
              }
            }
            else {
              local_res20 = 3;
            }
          }
          else {
            local_res20 = 1;
          }
        }
        else {
          bVar6 = true;
        }
      }
      else {
        local_e0 = 1;
      }
    }
    else {
LAB_140026cde:
      WVar4 = *local_res10;
      while ((bVar28 = bVar29, WVar4 != L'\0' &&
             ((((ushort)WVar4 < 9 || (0xd < (ushort)WVar4)) && (WVar4 != L' '))))) {
        local_res10 = local_res10 + 1;
        WVar4 = *local_res10;
      }
LAB_140026d14:
      FUN_1400170b0((void **)&local_res10);
    }
    uVar22 = (uint)uVar26;
    pWVar17 = (LPCWSTR)local_a0;
    WVar4 = *local_res10;
  }
  local_c0 = uVar31;
  plVar32 = plVar30 + 1;
  LeaveCriticalSection((LPCRITICAL_SECTION)&DAT_14013e190);
  plVar15 = local_d0;
  uVar31 = uVar24;
  if ((bVar5) && (bVar6)) {
    uVar31 = 1;
  }
  cVar11 = (char)local_res8;
  local_res10 = pWVar17;
  if ((((*pWVar17 == L'\0') && (((uint)(uint16_t)local_d8.Left | ((uint)(uint16_t)local_d8.Top << 16)) == 0)) && ((char)local_res8 != '\0')) ||
     ((plVar30[4] < 1 || (local_f7 != '\0')))) {
    if ((!bVar5) || (DVar23 = 0, uVar31 != 0)) goto LAB_140026e20;
LAB_1400275a9:
    if (-1 < (char)local_f2) {
      *(byte *)(param_1 + 2) = *(byte *)(param_1 + 2) & 0xfe;
      *(byte *)(param_1 + 2) = *(byte *)(param_1 + 2) | local_f2 & 1;
    }
    if (-1 < local_eb) {
      *(char *)((longlong)plVar30 + 0x29) = local_eb;
    }
    *(byte *)(param_1 + 2) = *(byte *)(param_1 + 2) & 0x81;
    *(byte *)(param_1 + 2) =
         *(byte *)(param_1 + 2) |
         (((((bVar28 * '\x02' | local_f5) * '\x02' | local_f3) * '\x02' | local_ef) * '\x02' |
          local_ee) * '\x02' | local_f1) * '\x02';
    if ((longlong)local_a8 < 1) {
      if (bVar8) {
        plVar30[4] = -0x80000;
      }
    }
    else {
      plVar30[4] = local_a8;
    }
  }
  else {
    if (((*pWVar17 != L'\0') || ((bVar2 & 1) != (*(byte *)(param_1 + 2) & 1))) ||
       (local_c0 != uVar22)) {
      uVar25 = 5;
      FUN_140025f10((longlong)param_1,local_80,5,(pthreadmbcinfo)0x0,(pthreadmbcinfo)0x0,(long long *)local_d0);
      EnterCriticalSection((LPCRITICAL_SECTION)&DAT_14013e190);
      FUN_1400185c8('\x01',plVar15);
      LeaveCriticalSection((LPCRITICAL_SECTION)&DAT_14013e190);
      goto LAB_140026ff5;
    }
    if (uVar31 == 0) goto LAB_140026ff5;
LAB_140026e20:
    if (((char)param_1[0x1b] == '\0') || (bVar5 = true, *pWVar17 == L'\0')) {
      bVar5 = false;
    }
    if ((((char)local_c8 != '\0') && ((*(byte *)(param_1 + 2) & 1) != 0)) && (local_e9)) {
      FUN_140025f10((longlong)param_1,local_80,0,(pthreadmbcinfo)0x0,(pthreadmbcinfo)0x0,(long long *)local_d0);
    }
    if (bVar5) {
      *(undefined1 *)(param_1 + 0x1b) = 0;
      PECMD_AllocStrSlot((uint16_t **)&local_88);
      PECMD_StrAssign((uint16_t **)&local_68,(const uint16_t *)L"%&CurFile%");
      PECMD_ExpandCommandLine((long long *)param_1,local_68,(void *)&local_88,0,1);
      PECMD_FreeStrBuf((WCHAR **)&local_68);
      PECMD_StrAssign((uint16_t **)&local_78,(const uint16_t *)L"ENTER:");
      PECMD_AppendWideStr((void *)&local_78,local_88);
      lVar21 = (longlong)(int)plVar30[2];
      if ((*(byte *)(param_1 + 2) & 1) == 0) {
        if (*plVar32 == 0) {
          FUN_140063720((longlong **)plVar32,0x10000);
        }
      }
      else {
        FUN_140025f10((longlong)param_1,local_78,0,(pthreadmbcinfo)(void *)(uintptr_t)0x11,local_res18,
                     (long long *)plVar15);
      }
      FUN_14007034c((longlong *)&local_b0,(LPCWSTR)(*plVar32 + lVar21 * 2));
      plVar30[2] = lVar21;
      *(undefined2 *)(*plVar32 + lVar21 * 2) = 0;
      local_c8 = local_c8 & 0xff;
      if (bVar34) {
        local_c8 = uVar24;
      }
      PECMD_FreeStrBuf((WCHAR **)&local_78);
      PECMD_FreeStrBuf((WCHAR **)&local_88);
    }
    FUN_1400185c8('\x01',plVar15);
    DVar23 = 0;
    if (cVar11 == '\0') {
      if ((bVar6) && (*local_res10 == L'\0')) {
        local_50[0].QuadPart = 0;
        if (*plVar30 != 0) {
          SetFilePointerEx((HANDLE)(uintptr_t)*plVar30,0,(longlong *)local_50,2);
        }
      }
      else {
        if ((((char)plVar30[5] != '\0' || bVar7) &&
            ((plVar18 = (longlong *)FUN_14001E5B0((void *)param_1,(const uint16_t *)L"*",
                            (LPCWSTR)(void *)DAT_14011c638,1,0), plVar18 != (longlong *)0x0
             && (*plVar18 != 0)))) &&
           (puVar19 = FUN_14001877C(plVar18 + 2,0x38), puVar19 != (undefined8 *)0x0))
        {
          puVar27 = puVar19;
          PECMD_InitStringObj((uint64_t *)puVar19);
          puVar19[5] = (undefined8)(uintptr_t)plVar30;
          plVar18 = puVar19 + 3;
          *plVar18 = 0;
          uVar3 = *(undefined1 *)((longlong)plVar30 + 0x29);
          *plVar18 = 0;
          *(undefined1 *)((longlong)puVar27 + 0x21) = uVar3;
          *(char *)(puVar27 + 4) = (char)plVar30[5];
          puVar27[1] = plVar30[4];
          puVar27[2] = *plVar30;
          *plVar30 = 0;
          puVar19 = (undefined8 *)FUN_14001E69C((void *)param_1,(const uint16_t *)L"&__LOGS",
                                                (void *)&DAT_14013d130,-1);
          if (puVar19 != (undefined8 *)0x0) {
            PECMD_StrBldCopyWide((long long *)plVar18,(LPCWSTR)(uintptr_t)*puVar19);
          }
        }
        if (*plVar30 != 0) {
          CloseHandle((HANDLE)(uintptr_t)*plVar30);
          g_i64CCB8 = g_i64CCB8 + -1;
        }
        *plVar30 = 0;
        plVar30[2] = 0;
        plVar30[4] = -0x80000;
        *(undefined1 *)(plVar30 + 5) = 0;
        if (bVar10 == 3) {
          if (*local_res10 == L'\0') {
            if (DAT_14013cb18[0] != 0) {
              FUN_1400185c8('\x01',(longlong *)(uintptr_t)0xffffffffffffffff);
              CloseHandle((HANDLE)(uintptr_t)DAT_14013cb18[0]);
              DAT_14013cb18[0] = 0;
              g_i64CCB8 = g_i64CCB8 + -1;
            }
            goto LAB_14002715e;
          }
LAB_14002716f:
          *(byte *)((longlong)plVar30 + 0x29) = *(byte *)((longlong)plVar30 + 0x29) & 0xfe;
          *(byte *)((longlong)plVar30 + 0x29) =
               *(byte *)((longlong)plVar30 + 0x29) | (char)local_e0 != '\0';
          if (local_res20 != 0) {
            iVar12 = lstrlenW(local_res10);
            DVar23 = iVar12 + 0x108;
            FUN_140063720((longlong **)&local_70,(longlong)(int)(DVar23 * 2));
            lpszVolumeName = local_70;
            pWVar1 = local_70 + (int)DVar23;
            *pWVar1 = L'\0';
            *local_70 = L'\0';
            FUN_14006459C(local_res10,0x105,pWVar1,(LPWSTR *)0x0);
            plVar15 = local_d0;
            if ((*pWVar1 != L'\0') && (pWVar1[1] == L':')) {
              local_c0 = (undefined4)((0x3aU << 16) | (uint16_t)*pWVar1);
              local_bc = 0x5c;
              local_ba = 0;
              local_res10 = pWVar1;
              if (((local_res20 & 1) == 0) ||
                 ((BVar13 = GetVolumeNameForVolumeMountPointW((LPCWSTR)&local_c0,lpszVolumeName,DVar23),
                   BVar13 == 0 || (*lpszVolumeName == L'\0')))) {
                plVar15 = local_d0;
                if ((local_res20 & 2) != 0) {
                  PECMD_AllocWStringBuffer((WCHAR **)&local_res8,0x1e0);
                  *(WCHAR *)(uintptr_t)local_res8 = *pWVar1;
                  ((WCHAR *)(uintptr_t)local_res8)[1] = L'\0';
                  PECMD_DescribePartitionInfo((int64_t *)&local_res8,
                                (LPCWSTR)0xffffffffffffffff,0xffffffffffffffff,0xac,
                                (LARGE_INTEGER *)0x0,(uint *)0x0,'\0');
                  if ((-1 < *(longlong *)(uintptr_t)local_res8) && (0 < *(longlong *)((uintptr_t)local_res8 + 4))) {
                    wsprintfW(lpszVolumeName,(const unsigned short *)L"\\\\?\\Harddisk%uPartition%u\\");
                    PECMD_FreeStrBuf(&local_res8);
                    goto LAB_1400272ea;
                  }
                  PECMD_FreeStrBuf(&local_res8);
                  plVar15 = local_d0;
                }
              }
              else {
LAB_1400272ea:
                iVar12 = lstrlenW(lpszVolumeName);
                lstrcpyW(lpszVolumeName + iVar12,pWVar1 + 3);
                local_res10 = lpszVolumeName;
                plVar15 = local_d0;
              }
            }
          }
          iVar12 = lstrlenW(local_b0);
          if (plVar30[3] < (longlong)(iVar12 + 0x9e)) {
            plVar30[3] = (longlong)(iVar12 + 0x10008);
            FUN_140063720((longlong **)plVar32,(longlong)(iVar12 + 0x10008));
          }
          *(undefined2 *)*plVar32 = 0;
          uVar16 = FUN_14005C7C4("CONOUT$",(ushort *)local_res10);
          if ((char)uVar16 != '\0') {
            FUN_140060A94(0x102);
          }
          local_58.QuadPart = 0;
          lVar21 = FUN_140003864(plVar30,local_res10,0x40000000,7,(LPSECURITY_ATTRIBUTES)0x0,
                                 (-(uint)bVar6 & 2) + 2,0,(HANDLE)0x0);
          *(undefined1 *)(plVar30 + 5) = local_f0;
          if (lVar21 == 0) {
            local_e8 = GetLastError();
            if (local_e8 == 0) {
              local_e8 = 1;
            }
          }
          else {
            local_e8 = 0;
          }
          if (*plVar30 != 0) {
            g_i64CCB8 = g_i64CCB8 + 1;
            local_60 = local_res10;
            if (bVar6) {
              SetFilePointerEx((HANDLE)(uintptr_t)*plVar30,0,(longlong *)&local_58,2);
            }
          }
          if (((char)uVar16 == '\0') && (local_58.QuadPart < 1)) {
            *(undefined2 *)*plVar32 = 0xfeff;
            *(undefined2 *)(*plVar32 + 2) = 0;
            plVar30[2] = 1;
          }
          PECMD_MemMoveForward((uint8_t *)(*plVar32 + plVar30[2] * 2),(uint8_t *)local_b0,
                        (iVar12 + 1) * 2);
          plVar30[2] = plVar30[2] + (longlong)iVar12;
          if (((char)local_c8 != '\0') && ((*(byte *)(param_1 + 2) & 1) != 0)) {
            FUN_140025f10((longlong)param_1,local_80,0,(pthreadmbcinfo)0x0,local_res18,(long long *)plVar15);
          }
          if ((*plVar30 != 0) && ((g_flagD6F6 & 2) != 0)) {
            CVar14 = GetLargestConsoleWindowSize((HANDLE)(uintptr_t)*plVar30);
            local_res8 = (undefined8)(((uint64_t)local_res8 & 0xffffffff00000000ULL) |
                          ((uint32_t)(uint16_t)CVar14.X | ((uint32_t)(uint16_t)CVar14.Y << 16)));
            pWVar20 = (WCHAR *)(uintptr_t)local_res8;
            local_res8 = (undefined8)(((uint64_t)local_res8 & 0xffffffffffff0000ULL) |
                          (uint16_t)CVar14.X);
            if (0x54 < (short)local_res8) {
              local_res8 = (undefined8)(((uint64_t)local_res8 & 0xffffffffffff0000ULL) |
                            (uint16_t)((short)local_res8 + -5));
            }
            local_res8 = (undefined8)(((uint64_t)local_res8 & 0xffffffff0000ffffULL) |
                          ((uint32_t)(uint16_t)CVar14.Y << 16));
            if (0x1d < (uint16_t)((uint64_t)local_res8 >> 16)) {
              local_res8 = (undefined8)(((uint64_t)local_res8 & 0xffffffff0000ffffULL) |
                            ((uint32_t)((uint16_t)((uint64_t)local_res8 >> 16) - 5) << 16));
            }
            local_d8.Right = (short)local_res8 + -1;
            local_d8.Bottom = (SHORT)((int)(uint16_t)((uint64_t)local_res8 >> 16) - 1);
            local_d8.Left = 0;
            local_d8.Top = 0;
            local_res8 = (undefined8)(uintptr_t)pWVar20;
            SetConsoleWindowInfo((HANDLE)(uintptr_t)*plVar30,1,&local_d8);
          }
          pWVar17 = local_60;
          if ((*local_60 != L'\0') && (local_res20 == 0)) {
            FUN_140063720((longlong **)&local_b0,0x10e);
            FUN_14006459C(pWVar17,0x105,local_b0,(LPWSTR *)0x0);
            pWVar17 = local_b0;
          }
        }
        else {
LAB_14002715e:
          if (*local_res10 != L'\0') goto LAB_14002716f;
          pWVar17 = (LPCWSTR)(void *)DAT_14011c638;
        }
        FUN_1400629b8((void *)&DAT_14013d130,(const uint16_t *)L"&__LOGS",pWVar17);
        DVar23 = local_e8;
        if (local_e8 == 0) goto LAB_1400275a9;
      }
    }
    else if (((uint)(uint16_t)local_d8.Left | ((uint)(uint16_t)local_d8.Top << 16)) != 0) goto LAB_1400275a9;
  }
  uVar25 = (ulonglong)(int)DVar23;
LAB_140026ff5:
  PECMD_FreeStrBuf((WCHAR **)&local_70);
  PECMD_FreeStrBuf((WCHAR **)&local_b0);
  LeaveCriticalSection((LPCRITICAL_SECTION)&DAT_14013e190);
  return uVar25;
}
/* ---- 新增最小桩 (PECMD_ScriptInitParse 体新增引用) ---- */
undefined8 *FUN_14001877C(longlong *ps, int count) { (void)ps;(void)count; return (undefined8 *)0; }
COORD GetLargestConsoleWindowSize(void *h) { (void)h; COORD c = { 0, 0 }; return c; }
int SetConsoleWindowInfo(void *h, int b, void *r) { (void)h;(void)b;(void)r; return 1; }

static uint64_t PECMD_WaitOtherThreadsExit(void) { return 0; }   /* 新增最小桩 (PECMD_ExpandSpecialDirs 恢复体: 作线程入口指针) */
/* ========== PECMD_ExpandSpecialDirs @ 14002cc30  size=1803 — 环境变量设置命令 (SETENV 语义) ==========
 * decompiled.c @26728 忠实移植: '^' 前缀跳过 → 缓冲分配 (lstrlenW+0x400) → '$'(系统)/'#'(用户)
 * 作用域前缀解析 → 星号/'*' 与 '-' 修饰 (local_78) → 空命令时按 param_4==0/0x23 走重画标志
 * (local_88=0x10), 否则枚举 8 个特殊目录 (PTR_u__Favorites_14011e6f8) 以 "%s="(DAT 真值
 * 0x1401210e8) 构造 "*<名>=<路径>", CSIDL==0x1a (APPDATA) 时补 "\\Microsoft\\Internet
 * Explorer\\Quick Launch" 并建目录, 写 "HKCU\Environment\" 槽与 &名=路径 变量, 递归本函数 →
 * param_5 修饰串解析 (~ / ~~ / ~. / ~* 等修饰, * , -env/-raw/-ex) → '=' 拆分 → 变量名展开
 * (PECMD_ExpandVarsRecursive / PECMD_ExpandCommandLine) → 按 bVar15 模式经 PECMD_ApplyVarWriteModifiers/
 * PECMD_ExpandVarsLocked 重写值 → SYSTEM\...\Environment (local_74=0x24) / HKCU\Environment
 * (local_70=0x23) 注册表写入 (RegDeleteValue/RegSetValueWithOpen) → SetEnvironmentVariableW →
 * 收尾: local_78=='-' → 起线程 PECMD_WaitOtherThreadsExit; 否则非 '*'-前缀且重画标志 → PECMD_BroadcastEnvChange / g_afterMain.
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
typedef struct { const uint16_t *name; int csidl; } EC_FavEntry;
static const EC_FavEntry EC_favTable[8] = {
    { (const uint16_t *)L"&Favorites", 0x06 }, { (const uint16_t *)L"&Desktop", 0x10 }, { (const uint16_t *)L"&StartMenu", 0x0b },
    { (const uint16_t *)L"&Startup", 0x07 },  { (const uint16_t *)L"&Programs", 0x02 }, { (const uint16_t *)L"&SendTo", 0x09 },
    { (const uint16_t *)L"&Personal", 0x05 }, { (const uint16_t *)L"&QuickLaunch", 0x1a },
};
extern uint16_t *wcscat(uint16_t *dest, const uint16_t *src);
extern uint32_t g_afterMain;                       /* DAT_14013ccc8 (core_globals.c) */
extern uint8_t  g_flagCCB3;                        /* DAT_14013ccb3 (core_globals.c) */
extern DWORD    g_dwC96C;                          /* DAT_14013c96c (core_globals.c) */
extern uint64_t PECMD_CreateDirectoryTree(LPCWSTR path);      /* @0x14001e19c (core_b1_remaining.c) */
extern LPCWSTR  PECMD_ApplyVarWriteModifiers(int64_t *, int64_t *, uint16_t *, int64_t *);  /* @0x140084a5c SKIP 桩 (core_b3_remaining.c) */
extern void    *PECMD_CheckVarMultiLevelRef(int64_t *, LPCWSTR);             /* @0x1400681ec (core_b3_remaining.c) */
extern uint64_t *PECMD_ExpandVarsLocked(uint64_t *, uint64_t *, uint8_t);            /* @0x14008569c SKIP 桩 (本函数后部定义) */
extern uint64_t  PECMD_RegiEditRegistry(uint64_t, uint64_t, uint64_t);               /* @0x1400c13f8 (后部定义) */
extern HANDLE    PECMD_CreateDetachedThread(LPTHREAD_START_ROUTINE, LPVOID, size_t, DWORD, LPDWORD, LPSECURITY_ATTRIBUTES);  /* @0x14005b228 (后部定义) */
extern unsigned long PECMD_RegSetValueWithOpen(void *, const unsigned short *, const unsigned short *,
                                               unsigned long, unsigned char *, unsigned long);   /* @0x14005c5a0 (后部定义) */
extern DWORD PECMD_RegDeleteValue(HKEY, LPCWSTR, LPCWSTR);                  /* @0x14005c61c (后部定义) */
extern long long PECMD_ExpandVarsRecursive(long long *, WCHAR *, void *, int, uint8_t);          /* @0x14007bda8 (后部定义) */
extern uint64_t SHGetSpecialFolderPathW(HWND, WCHAR *, int, BOOL);          /* @后部定义 */
uint64_t PECMD_ExpandSpecialDirs(void *param_1, const uint16_t *param_2, longlong param_3, longlong param_4,
                       const uint16_t *param_5)
{
    WCHAR *pwVar1;          /* 特殊目录路径续写位置 */
    bool bVar2;             /* -raw */
    bool bVar3;             /* -ex */
    LPCWSTR lpName;         /* 展开后变量名 */
    byte bVar4;             /* 修饰标志携带 */
    int iVar5;
    BOOL BVar6;
    uint uVar7;
    WCHAR *pWVar8;          /* 修饰串游标 */
    longlong *plVar9;       /* 串槽 */
    ulonglong uVar11;       /* 返回值 */
    WCHAR WVar12;
    longlong lVar13;        /* 特殊目录循环计数 */
    byte bVar14;            /* -env 标志 (0x40) */
    byte bVar15;            /* ~ 与 * 修饰位 (1=~,2=~~,4=*,8=~.,9=~*) */
    DWORD DVar16;
    pthreadmbcinfo ptVar18;
    byte bVar19;
    LPCWSTR local_res10;    /* 命令串游标 (跳过 $/# 与空白、'*'/'-' 修饰后) */
    char local_88;          /* param_3!=0 标志 / 0x10 重画 */
    LPWSTR local_80;        /* '=' 后值串 / 重写结果 */
    WCHAR local_78;         /* '*' 或 '-' 前缀 */
    uint local_74;          /* 系统作用域标记 0x24 (低16位参与判定) */
    undefined4 local_70;    /* 用户作用域标记 0x23 */
    HKEY local_68;          /* "HKCU\Environment\" 串槽 (以 HKEY 型承载) */
    LPCWSTR local_60;       /* 变量名展开结果 */
    ulonglong local_58;
    WCHAR *local_50;        /* 缓冲索引 1: "*<名>=路径" 起点 */
    longlong local_48;      /* 重写串槽 */
    LPWSTR local_40;        /* 缓冲首址 (递归用, 可写: *local_40=L'*') */
    WCHAR *local_38;        /* 大缓冲 (PECMD_AllocWStringBuffer) */
    longlong local_30[3];   /* PECMD_ApplyVarWriteModifiers 长度槽 (local_30[0]/[1] 使用, [2] 未用) */
    const EC_FavEntry *ppuVar17;

    if (*param_2 == L'^') {
        param_2 = param_2 + 1;
    }
    iVar5 = lstrlenW(param_2);
    PECMD_AllocWStringBuffer((WCHAR **)&local_38, (longlong)iVar5 + 0x400);   /* 反编译 local_74=返回值, 桩 void → 0 */
    ptVar18 = (pthreadmbcinfo)0x24;
    local_74 = 0;              /* Ghidra: = 返回 & 0xffff0000 (栈噪声, 等效 0) */
    local_40 = local_38;
    local_78 = L'\0';
    local_70 = 0;              /* Ghidra: = (uint32)(&local_38 & 0xffffffffffff0000), 仅低16位参与判定 */
    if (param_3 == 0x24) {
        local_74 = 0x24;
    }
    else if (param_3 == 0x23) {
        local_70 = 0x23;
    }
    if ((*param_2 == L'$') && ((uint16_t)local_74 == 0)) {
        local_74 = (local_74 & 0xffff0000u) | 0x24;
LAB_14002ccee:
        local_res10 = param_2 + 1;
    }
    else {
        local_res10 = param_2;
        if ((*param_2 == L'#') && ((uint16_t)local_70 == 0)) {
            local_70 = (local_70 & 0xffff0000u) | 0x23;
            goto LAB_14002ccee;
        }
    }
    if (*local_res10 == L' ') {
        local_res10 = local_res10 + 1;
    }
    if ((*local_res10 == L'*') || (*local_res10 == L'-')) {
        local_78 = *local_res10;
        local_res10 = local_res10 + 1;
    }
    local_88 = (char)param_3 != '\0';
    local_58 = 0;
    PECMD_AllocStrSlot((uint16_t **)&local_68);
    if (*local_res10 == L'\0') {
        if ((param_4 == 0) || (param_4 == 0x23)) {
            local_88 = '\x10';
        }
        else {
            local_50 = local_38 + 1;
            *local_40 = L'*';
            lVar13 = 8;
            ppuVar17 = EC_favTable;
            do {
                pwVar1 = local_50;
                /* 格式 "%s=" @0x1401210e8 (DAT 真值); PECMD_CrtShim 为 void SKIP 桩,
                   反编译 iVar5=返回值(写字符数) 按格式实算 = wcslen(name+2)+1 */
                PECMD_CrtShim(local_50, (size_t)0x1401210e8,
                              (void *)(uintptr_t)((const char *)(ppuVar17->name) + 2),
                              (void *)ptVar18);
                iVar5 = 1;
                {
                    const uint16_t *q = (const uint16_t *)((uintptr_t)(ppuVar17->name) + 2);
                    while (*q) { iVar5++; q++; }
                }
                ptVar18 = (pthreadmbcinfo)0x1;
                pwVar1 = pwVar1 + iVar5;
                BVar6 = SHGetSpecialFolderPathW((HWND)0x0, pwVar1, ppuVar17->csidl, 1);
                if (BVar6 != 0) {
                    if (ppuVar17->csidl == 0x1a) {
                        wcscat(pwVar1, (const uint16_t *)L"\\Microsoft\\Internet Explorer\\Quick Launch");
                        PECMD_CreateDirectoryTree((LPCWSTR)pwVar1);
                        CreateDirectoryW(pwVar1, (LPSECURITY_ATTRIBUTES)0x0);
                    }
                    PECMD_AssignString((int64_t *)&local_68, (const uint16_t *)L"HKCU\\Environment\\");
                    PECMD_AppendWideStr(&local_68, local_50);
                    PECMD_RegiEditRegistry((uint64_t)(uintptr_t)&DAT_14013d130, (uint64_t)(uintptr_t)local_68,
                                  (uint64_t)'\x01');
                    PECMD_SetVariable((void *)&DAT_14013d130, (const WCHAR *)ppuVar17->name, pwVar1);
                    ptVar18 = (pthreadmbcinfo)0x0;
                    PECMD_ExpandSpecialDirs(param_1, local_40, param_3, 0, (const uint16_t *)0);
                }
                ppuVar17 = ppuVar17 + 1;
                lVar13 = lVar13 + -1;
            } while (lVar13 != 0);
        }
    }
    bVar19 = 0;
    bVar15 = 0;
    bVar14 = 0;
    bVar2 = false;
    bVar3 = false;
    if ((param_5 != (const uint16_t *)0) && (pWVar8 = (WCHAR *)param_5, bVar4 = 0, *param_5 != L'\0')) {
        do {
            bVar15 = bVar4;
            bVar19 = bVar14;
            if (((8 < (uint16_t)*pWVar8) && ((uint16_t)*pWVar8 < 0xe)) || (*pWVar8 == L' ')) break;
            if (((*pWVar8 != L'.') && (*pWVar8 != L'$')) && (*pWVar8 != L'%')) {
                if (*pWVar8 == L'*') {
                    bVar15 = bVar15 | 4;
                }
                else {
                    if (*pWVar8 == L'~') {
                        if (pWVar8[1] == L'~') {
                            bVar15 = bVar15 | 2;
LAB_14002cf1e:
                            pWVar8 = pWVar8 + 1;
                            goto LAB_14002cfa8;
                        }
                        if (pWVar8[1] == L'.') { bVar15 = bVar15 | 8; goto LAB_14002cf1e; }
                        if (pWVar8[1] == L'*') { bVar15 = bVar15 | 9; goto LAB_14002cf1e; }
                        bVar15 = bVar15 | 1;
                        goto LAB_14002cfa8;
                    }
                    if (((*pWVar8 == L'-') && (pWVar8[1] == L'e')) &&
                        ((pWVar8[2] == L'n') && (pWVar8[3] == L'v'))) {
                        bVar14 = 0x40;
LAB_14002cf99:
                        pWVar8 = pWVar8 + 3;
                    }
                    else if (*pWVar8 == L'-') {
                        if (((pWVar8[1] == L'r') && (pWVar8[2] == L'a')) && (pWVar8[3] == L'w')) {
                            bVar2 = true;
                            goto LAB_14002cf99;
                        }
                        if (((*pWVar8 == L'-') && (pWVar8[1] == L'e')) && (pWVar8[2] == L'x')) {
                            bVar3 = true;
                            pWVar8 = pWVar8 + 2;
                        }
                    }
                }
            }
LAB_14002cfa8:
            pWVar8 = pWVar8 + 1;
            bVar19 = bVar14;
            bVar4 = bVar15;
        } while (*pWVar8 != L'\0');
        WVar12 = *param_5;
        if (bVar2) {
            bVar15 = 0;
            do {
                if (((8 < (uint16_t)WVar12) && ((uint16_t)WVar12 < 0xe)) || (WVar12 == L' ')) break;
                param_5 = param_5 + 1;
                WVar12 = *param_5;
            } while (WVar12 != L'\0');
            PECMD_SkipLeadingControls((uint64_t *)&param_5);
            local_res10 = param_5;
        }
    }
    pWVar8 = (WCHAR *)param_5;
    local_80 = StrChrW(local_res10, L'=');
    if (local_80 != (LPWSTR)0x0) {
        *local_80 = L'\0';
        local_80 = local_80 + 1;
    }
    local_60 = (LPCWSTR)0x0;
    if ((*(char *)((uintptr_t)param_1 + 0xda) == '\0') &&
        ((*(byte *)((uintptr_t)param_1 + 0xd) & 0xf) == 0)) {
        PECMD_ExpandVarsRecursive((long long *)param_1, (WCHAR *)local_res10, (void *)&local_60, 0, 1);
    }
    else {
        PECMD_ExpandCommandLine((long long *)param_1, (WCHAR *)local_res10, (void *)&local_60, 0, 1);
    }
    lpName = local_60;
    local_48 = 0;
    local_30[0] = 0;
    local_30[1] = 0;
    if (bVar15 == 0) {
LAB_14002d167:
        if ((local_80 != (LPCWSTR)0x0) &&
            (PECMD_ApplyVarWriteModifiers((int64_t *)&local_80, local_30, (uint16_t *)pWVar8, local_30 + 1),
             local_80 != (LPCWSTR)0x0)) {
            iVar5 = lstrlenW(local_80);
            DVar16 = iVar5 * 2;
            goto LAB_14002d1b0;
        }
    }
    else if (local_80 != (LPCWSTR)0x0) {
        plVar9 = PECMD_AssignString(&local_48, (const uint16_t *)local_80);
        local_80 = (LPWSTR)PECMD_ExpandVarsLocked((uint64_t *)param_1, (uint64_t *)plVar9, (uint8_t)(bVar19 | bVar15));
        goto LAB_14002d167;
    }
    DVar16 = 0;
LAB_14002d1b0:
    uVar11 = local_58;
    if (*lpName != L'\0') {
        if ((uint16_t)local_74 != 0) {
            if (local_80 == (LPCWSTR)0x0) {
                PECMD_RegDeleteValue((HKEY)0xffffffff80000002,
                                     (LPCWSTR)L"SYSTEM\\CurrentControlSet\\Control\\Session Manager\\Environment",
                                     lpName);
            }
            else {
                PECMD_RegSetValueWithOpen((HKEY)0xffffffff80000002,
                                          (LPCWSTR)L"SYSTEM\\CurrentControlSet\\Control\\Session Manager\\Environment",
                                          lpName, 1, (BYTE *)local_80, DVar16);
            }
        }
        uVar11 = local_58;
        if (*lpName != L'\0') {
            if ((uint16_t)local_70 != 0) {
                if (local_80 == (LPCWSTR)0x0) {
                    PECMD_RegDeleteValue((HKEY)0xffffffff80000001, (LPCWSTR)L"Environment", lpName);
                }
                else {
                    PECMD_RegSetValueWithOpen((HKEY)0xffffffff80000001, (LPCWSTR)L"Environment", lpName, 1,
                                              (BYTE *)local_80, DVar16);
                }
            }
            uVar11 = local_58;
            if (*lpName != L'\0') {
                if (bVar3) {
                    local_80 = (LPWSTR)PECMD_CheckVarMultiLevelRef((int64_t *)param_1, (LPCWSTR)local_80);
                }
                BVar6 = SetEnvironmentVariableW(lpName, local_80);
                uVar7 = 0;
                if (BVar6 == 0) {
                    uVar7 = GetLastError();
                }
                uVar11 = (ulonglong)uVar7;
            }
        }
    }
    if ((local_88 != '\0') && (local_78 != L'*')) {
        if ((local_78 == L'-') || (g_flagCCB3 == (uint8_t)L'-')) {
            PECMD_CreateDetachedThread((LPTHREAD_START_ROUTINE)PECMD_WaitOtherThreadsExit, (LPVOID)0x0, (size_t)0x10000,
                          (DWORD)0x10000, (LPDWORD)&g_dwC96C, (LPSECURITY_ATTRIBUTES)0x0);
        }
        else if ((local_88 == '\x10') || (g_flagCCB3 == '\0')) {
            PECMD_BroadcastEnvChange();
        }
        else {
            g_afterMain = 1;
        }
    }
    PECMD_FreeStrBuf((void *)local_30);
    PECMD_FreeStrBuf((WCHAR **)&local_48);
    PECMD_FreeStrBuf((WCHAR **)&local_60);
    PECMD_FreeStrBuf((WCHAR **)&local_68);
    PECMD_FreeStrBuf((WCHAR **)&local_38);
    return uVar11;
}

/* @0x14002e30c size=— 资源串按行拆分入表(直移) */
void PECMD_LoadResourceLines(void)
{
  uint16_t buf[4100]; uint64_t ls;
  buf[0] = 0;
  const uint16_t *p = PECMD_LoadLocalizedString((void *)(uintptr_t)DAT_14013ca68,0x271f,(uint16_t *)buf,0x1000);
  uint16_t w = *p;
  const uint16_t *line = p;
  while ((line = p, w != 0)) {
    const uint16_t *q = line;
    for (; (w = *q, w != 0 && (w != L'\r')) && (w != L'\n'); q++) {}
    if (*q != 0) { *(uint16_t *)q = 0; q++; }
    if ((uintptr_t)q != 0) { for (; (*q == L'\n' || *q == L'\r'); q++) {} }
    PECMD_StrDupAssign((uint16_t **)&ls,(const uint16_t *)line);
    FUN_140023640((uint64_t)(uintptr_t)&DAT_14013d130,(uint64_t)ls,-1);
    PECMD_FreeStrBuf((WCHAR **)&ls);
    p = q; w = *q;
  }
}
void PECMD_SendHotkeyKeyMessage(uint32_t a, int b, int c) { (void)a;(void)b;(void)c; }   /* 签名修正: uint32_t,int,int */
uint64_t PECMD_EnsureCallbackWindow(void) { return 0; }
/* ============================================================
 * PECMD_ProcessScriptBlock @0x14004c0bc (size=8717) — 资源/脚本执行主分发 (decompiled.c 直移)
 * ============================================================ */
/* ---- 移植前置: 全局槽 / 跨文件真实体 / 本文件后部定义 / 新增最小桩 ---- */
extern uint8_t DAT_00000011;                 /* @0x11 地址伪符号 (定义见本文件后部) */
int wsprintfW(unsigned short *out, const unsigned short *fmt, ...);   /* 定义见后部; 此处前置声明避免与隐式 int() 冲突 */
extern uint8_t g_charTableF;                 /* DAT_14013a248 (core_globals.c) */
extern int64_t g_i64CCB8;                    /* DAT_14013ccb8 (core_globals.c) */
extern WCHAR *g_pNextCmd;                    /* DAT_14013cac8 (core_globals.c) */
int    DAT_14013ccca = 0;                    /* @0x14013ccca 错误消息注册标志 (原无定义, 新增) */
LARGE_INTEGER FUN_14003C06C(int64_t *script, uint64_t cmd, uint32_t flags);  /* 真实体在 core_b2f.c */
/* 本文件后部定义的前置声明 (与定义完全一致) */
static uint16_t *PECMD_StrBldCopyWideN(void *out, const uint16_t *src, int64_t len);
long long   PECMD_ExpandCommandLine(long long *a, WCHAR *b, void *c, int d, uint8_t e);
uint64_t    PECMD_NotifyMainWindowRefresh(uint64_t a, int b);
uint64_t    PECMD_DispatchExpressionBlock(uint64_t a, uint64_t b);
uint64_t    PECMD_DdCopyCommand(uint64_t a, uint64_t b);
uint64_t    PECMD_ScriptMainEntry(uint64_t a, uint64_t b);
uint64_t    PECMD_ParseCommandPath(uint64_t a, uint64_t b, uint32_t *c, int64_t *d);
ushort     *FUN_140024c48(longlong *a, longlong *b, uint c);
static uint64_t PECMD_ZeroLenBuf(void *a);
undefined8  FUN_14005c788(char *a, ushort *b, int c);
longlong    PECMD_TokPrefixICmp(char *param_1, ushort *param_2, int param_3);
ulonglong   PECMD_EvalParenStripped(longlong *param_1, ulonglong *param_2);
longlong   *PECMD_AppendWideStr(void *param_1p, LPCWSTR param_2);
longlong    PECMD_CloseRestartByName(longlong *param_1, LPCWSTR param_2, longlong param_3, ulonglong param_4);
uint64_t    PECMD_XorEncode(const uint16_t *a, uint32_t b, uint64_t c);
uint64_t    PECMD_ExecCmdDispatch(uint64_t a, uint64_t b, uint64_t c, uint64_t d, uint64_t e, uint64_t f, uint64_t g, uint64_t h);
uint64_t    PECMD_ScriptInitParse(uint64_t a, uint64_t b, uint64_t c);
uint64_t    PECMD_SetRegistryOwnerRun(int64_t param_1, char param_2);
uint64_t    PECMD_WideStrLen(const uint16_t *s);
uint64_t    PECMD_VarLookup(int64_t *a, const uint16_t *b, int64_t *c, int64_t d, void *e);
/* 新增最小桩声明 (定义紧随本函数之后) */
uint64_t    PECMD_IsPecmdScriptFile(uint64_t); uint64_t PECMD_UserCmdHandler(uint64_t);
uint64_t    PECMD_LinkCreateShortcut(uint64_t, uint64_t); uint64_t PECMD_CreatePageFile(uint64_t);
uint64_t    PECMD_EvalSpecialToken(uint64_t, uint64_t, uint64_t, uint64_t, uint64_t);
uint64_t    PECMD_ProcessInitCommand(uint64_t, uint64_t); uint64_t PECMD_FontCommand(uint64_t, uint64_t, uint64_t, uint64_t);
uint64_t    PECMD_DispCommand(uint64_t, uint64_t); uint64_t PECMD_SendInputEvents(uint64_t);
uint64_t    PECMD_DownCheckShutdown(void); uint64_t PECMD_ShutPowerAction(uint64_t);
uint64_t    PECMD_MstrStringOps(uint64_t, uint64_t); uint64_t PECMD_EvalRandFunction(uint64_t, uint64_t);
uint64_t    PECMD_MessShowMsgBox(uint64_t, uint64_t, uint64_t);
uint64_t    PECMD_HashCmdCompute(uint64_t, uint64_t, uint64_t); uint64_t PECMD_RegiEditRegistry(uint64_t, uint64_t, uint64_t);
uint64_t    PECMD_HomeSetStartPage(uint64_t, uint64_t);
uint64_t    PECMD_SpinAddControl(uint64_t, uint64_t, uint64_t, uint64_t);
uint64_t    PECMD_SbarAddControl(uint64_t, uint64_t, uint64_t, uint64_t);
uint64_t    PECMD_TablCreateControl(uint64_t, uint64_t, uint64_t);
uint64_t    PECMD_PartShowHideDrive(uint64_t, uint64_t);
uint64_t    PECMD_SiteSetFileAttrTime(uint64_t, uint64_t, uint64_t); uint64_t PECMD_GetfReadData(uint64_t, uint64_t);
uint64_t    PECMD_DiskFindLoadHives(uint64_t, uint64_t);
uint64_t    PECMD_MounMountWimImage(uint64_t, uint64_t, uint64_t); uint64_t PECMD_SockCreateIpcObj(uint64_t, uint64_t, uint64_t);

/* ========== PECMD_ProcessScriptBlock @ 14004c0bc  size=8717 ========== */
/* signature: LARGE_INTEGER __fastcall PECMD_ProcessScriptBlock(LARGE_INTEGER param_1, LARGE_INTEGER param_2, longlong * param_3, longlong * param_4, pthreadmbcinfo param_5) */

/* WARNING: Type propagation algorithm not settling */

LARGE_INTEGER
PECMD_ProcessScriptBlock(LARGE_INTEGER param_1,LARGE_INTEGER param_2,longlong *param_3,longlong *param_4,
             pthreadmbcinfo param_5)

{
  ushort *puVar1;
  WCHAR WVar2;
  LONG LVar3;
  byte bVar4;
  bool bVar5;
  char cVar6;
  ushort uVar7;
  int iVar8;
  int iVar9;
  longlong *plVar10;
  LARGE_INTEGER LVar11;
  ulonglong uVar12;
  undefined4 extraout_var_00;
  HRSRC pHVar13;
  undefined4 extraout_var_01;
  undefined7 extraout_var;
  LARGE_INTEGER extraout_RAX;
  LARGE_INTEGER extraout_RAX_00;
  undefined8 uVar14;
  LARGE_INTEGER LVar15;
  undefined1 uVar16;
  LPCWSTR *ppWVar17;
  byte *pbVar18;
  byte *pbVar19;
  short sVar20;
  uint uVar21;
  FILETIME _Var22;
  WCHAR *pWVar23;
  WCHAR WVar24;
  LARGE_INTEGER LVar25;
  short sVar26;
  DWORD DVar27;
  FILETIME _Var28;
  undefined4 unaff_R13D;
  uint uVar29;
  LPCWSTR pWVar30;
  LARGE_INTEGER LVar31;
  longlong lVar32;
  LARGE_INTEGER LVar33;
  LARGE_INTEGER local_res8;
  LARGE_INTEGER local_res10;
  uint local_res18 [2];
  longlong *local_res20;
  LARGE_INTEGER local_188;
  LARGE_INTEGER local_180;
  LPCWSTR local_178;
  undefined8 local_170;
  LARGE_INTEGER local_168;
  LARGE_INTEGER local_160;
  undefined4 local_158;
  byte local_154 [4];
  undefined2 local_150 [4];
  LPCWSTR local_148;
  LARGE_INTEGER local_140;
  ushort *local_138;
  LPCWSTR local_130;
  LARGE_INTEGER local_128;
  LARGE_INTEGER local_120;
  int local_118 [2];
  LARGE_INTEGER local_110;
  LARGE_INTEGER local_108;
  LPCWSTR local_100;
  longlong local_f8;
  LPCWSTR local_f0;
  LARGE_INTEGER local_e8;
  longlong local_e0;
  LARGE_INTEGER local_d8;
  longlong local_d0;
  WCHAR local_c8 [68];
  
  LVar15.QuadPart = 0;
  if (param_3 == (void *)0) {
    param_3 = *(longlong **)(param_1.QuadPart + 0x40);
  }
  local_res10 = param_2;
  local_res20 = param_4;
  PECMD_RandSeedAdvance();
  PECMD_SkipLeadingControls(&local_res10);
  if (((*(byte *)(param_1.QuadPart + 0x10) & 1) == 0) || (uVar16 = '\x01', g_i64CCB8 < 1)) {
    uVar16 = '\0';
  }
  if (*(WCHAR *)(uintptr_t)local_res10.QuadPart == L'@') {
    local_res10.QuadPart = local_res10.QuadPart + 2;
    uVar16 = '\0';
  }
  (*(uint8_t *)&local_res8.s.LowPart = (uVar16));
  local_180.QuadPart = -0x7fffbfff;
  sVar26 = 0;
  if (*(WCHAR *)(uintptr_t)local_res10.QuadPart == L'\0') {
    return (LARGE_INTEGER){0};
  }
  uVar29 = 0;
  local_128.QuadPart = 0;
  local_188.QuadPart = 0;
  cVar6 = '\0';
  local_170 = (undefined8)((((uint64_t)((uint32_t)(local_170 >> 32)) << 32) | (uint32_t)unaff_R13D) & 0xffffffffffffff00ULL);
  LVar25 = local_res10;
  if (*(WCHAR *)(uintptr_t)local_res10.QuadPart != L'-') goto LAB_14004c1b5;
LAB_14004c196:
  cVar6 = *(char *)(uintptr_t)LVar25.QuadPart;
  local_res10.QuadPart = LVar25.QuadPart + 2;
  if (*(WCHAR *)(uintptr_t)local_res10.QuadPart == L' ') {
    local_res10.QuadPart = LVar25.QuadPart + 4;
  }
LAB_14004c1b5:
  if (uVar16 != '\0') {
    if ((*(byte *)(param_1.QuadPart + 0x10) & 2) == 0) {
      PECMD_ExpandVarDispatch((longlong *)(uintptr_t)param_1.QuadPart,(WCHAR *)(uintptr_t)local_res10.QuadPart,&local_128,0,1);
      local_188 = local_128;
    }
    else {
      plVar10 = PECMD_AssignString((longlong *)&local_128.QuadPart,(LPCWSTR)(uintptr_t)local_res10.QuadPart);
      local_188.QuadPart = *plVar10;
    }
    if ((cVar6 == '\0') &&
       (((*(byte *)(param_1.QuadPart + 0x10) & 4) != 0 ||
        ((*(byte *)(param_1.QuadPart + 0x10) & 0x10) != 0)))) {
      FUN_140025f10(param_1.QuadPart,(LPCWSTR)(uintptr_t)local_188.QuadPart,0,(pthreadmbcinfo)&DAT_00000011,param_5,
                    (void *)0);
      if ((*(byte *)(param_1.QuadPart + 0x10) & 0x10) == 0) {
        local_188.QuadPart = 0;
      }
    }
    else {
      uVar29 = uVar29 & 0xff;
      if ((*(byte *)(param_1.QuadPart + 0x10) & 8) == 0) {
        uVar29 = 2;
      }
      local_170 = (undefined8)((((uint64_t)((uint32_t)(local_170 >> 32)) << 32) | (uint32_t)uVar29));
    }
  }
  local_168.QuadPart = 0;
  if (cVar6 != '\0') {
    if (local_188.QuadPart != 0) {
      FUN_140025f10(param_1.QuadPart,(LPCWSTR)(uintptr_t)local_188.QuadPart,0,(pthreadmbcinfo)0x21,param_5,
                    (void *)0);
    }
    local_180.QuadPart = 0;
LAB_14004e27a:
    LVar15 = local_180;
    if ((*(byte *)(param_1.QuadPart + 0x11) & 1) != 0) {
      PECMD_ZeroLenBuf((ulonglong *)(uintptr_t)local_128.QuadPart);
      PECMD_ZeroLenBuf((ulonglong *)(uintptr_t)local_168.QuadPart);
      LVar15 = local_180;
    }
LAB_14004e29b:
    PECMD_FreeStrBuf(&local_168.QuadPart);
    PECMD_FreeStrBuf(&local_128.QuadPart);
    return LVar15;
  }
  local_res18[0] = ((local_res18[0] & 0xffffff00U) | 10U);
  PECMD_SkipLeadingControls(&local_res10);
  PECMD_AllocStrSlot((uint16_t **)&local_160);
  LVar25 = local_res10;
  sVar20 = sVar26;
  while( true ) {
    for (; *(WCHAR *)(uintptr_t)LVar25.QuadPart == L'^'; LVar25.QuadPart = LVar25.QuadPart + 2) {
      sVar20 = sVar20 + 1;
    }
    while (sVar20 = sVar20 + -1, -1 < sVar20) {
      local_110.QuadPart = 0;
      PECMD_ExpandVarDispatch((longlong *)(uintptr_t)param_1.QuadPart,(WCHAR *)(uintptr_t)LVar25.QuadPart,&local_110,0,1);
      LVar25 = local_110;
      LVar11 = local_168;
      local_168.s = local_110.s;
      local_110.s = LVar11.s;
      PECMD_FreeStrBuf(&local_110.QuadPart);
    }
    WVar24 = *(WCHAR *)(uintptr_t)LVar25.QuadPart;
    LVar11 = LVar25;
    while ((WVar24 != L'\0' &&
           ((((ushort)WVar24 < 9 || (0xd < (ushort)WVar24)) && (WVar24 != L' '))))) {
      LVar11.QuadPart = LVar11.QuadPart + 2;
      WVar24 = *(WCHAR *)(uintptr_t)LVar11.QuadPart;
    }
    iVar8 = (int)(LVar11.QuadPart - (longlong)LVar25.QuadPart >> 1) + -1;
    LVar11 = LVar25;
    sVar20 = sVar26;
    if (iVar8 < 0) break;
    do {
      if (*(WCHAR *)(uintptr_t)LVar11.QuadPart == L'%') {
        sVar20 = sVar20 + 1;
      }
      LVar11.QuadPart = LVar11.QuadPart + 2;
      iVar8 = iVar8 + -1;
    } while (-1 < iVar8);
    if (sVar20 < 2) break;
    WVar24 = *(WCHAR *)(uintptr_t)LVar25.QuadPart;
    LVar11 = LVar25;
    while (((WVar24 != L'\0' && (((ushort)WVar24 < 9 || (0xd < (ushort)WVar24)))) &&
           (WVar24 != L' '))) {
      LVar11.QuadPart = LVar11.QuadPart + 2;
      WVar24 = *(WCHAR *)(uintptr_t)LVar11.QuadPart;
    }
    PECMD_StrDupAssign((uint16_t **)&local_130,(LPCWSTR)(uintptr_t)LVar11.QuadPart);
    iVar8 = lstrlenW((LPCWSTR)(uintptr_t)LVar11.QuadPart);
    local_148 = (WCHAR *)0x0;
    *(WCHAR *)(uintptr_t)LVar11.QuadPart = L'\0';
    PECMD_ExpandVarDispatch((longlong *)(uintptr_t)param_1.QuadPart,(WCHAR *)(uintptr_t)LVar25.QuadPart,&local_148,0,1);
    WVar24 = *local_148;
    pWVar30 = local_148;
    while (WVar24 != L'\0') {
      if (WVar24 != L'@') {
        if (*(WCHAR *)(uintptr_t)LVar25.QuadPart == L'-') {
          PECMD_FreeStrBuf((WCHAR **)&local_148);
          PECMD_FreeStrBuf((WCHAR **)&local_130);
          PECMD_FreeStrBuf(&local_160.QuadPart);
          PECMD_FreeStrBuf(&local_168.QuadPart);
          uVar16 = (*(uint8_t *)&local_res8.s.LowPart);
          uVar29 = (uint)local_170;
          goto LAB_14004c196;
        }
        break;
      }
      pWVar30 = pWVar30 + 1;
      local_res8.QuadPart = local_res8.QuadPart & 0xffffffffffffff00;
      WVar24 = *pWVar30;
    }
    iVar9 = lstrlenW(pWVar30);
    PECMD_AllocString(&local_168,(longlong)iVar9 + 3 + (longlong)iVar8);
    LVar25 = local_168;
    PECMD_MemMoveForward((undefined1 *)(uintptr_t)local_168.QuadPart,(undefined1 *)pWVar30,iVar9 * 2);
    PECMD_MemMoveForward((undefined1 *)(local_168.QuadPart + (longlong)iVar9 * 2),(undefined1 *)local_130,
                  (iVar8 + 1) * 2);
    if (*(WCHAR *)(uintptr_t)local_168.QuadPart != L'^') {
      PECMD_FreeStrBuf((WCHAR **)&local_148);
      PECMD_FreeStrBuf((WCHAR **)&local_130);
      break;
    }
    cVar6 = (char)local_res18[0] + -1;
    local_res18[0] = ((local_res18[0] & 0xffffff00U) | (uint8_t)cVar6);
    if (cVar6 < '\0') {
      local_180.QuadPart = -0x7ff8ffa9;
      PECMD_FreeStrBuf((WCHAR **)&local_148);
      ppWVar17 = &local_130;
      goto LAB_14004c51b;
    }
    PECMD_FreeStrBuf((WCHAR **)&local_148);
    PECMD_FreeStrBuf((WCHAR **)&local_130);
    sVar20 = sVar26;
  }
  if (*(short *)(uintptr_t)LVar25.QuadPart == 0x28) {
    sVar20 = 0x29;
    *(undefined2 *)(uintptr_t)LVar25.QuadPart = 0x7b;
LAB_14004c59d:
    iVar8 = lstrlenW((LPCWSTR)(uintptr_t)LVar25.QuadPart);
    if (sVar20 == *(short *)(LVar25.QuadPart + -2 + (longlong)iVar8 * 2)) {
      *(undefined2 *)(LVar25.QuadPart + -2 + (longlong)iVar8 * 2) = 0;
    }
    local_d8.s = local_168.s;
    local_168.QuadPart = 0;
    PECMD_ExpandCommandLine((long long *)(uintptr_t)param_1.QuadPart,(WCHAR *)(uintptr_t)LVar25.QuadPart,&local_168,0,1);
    PECMD_FreeStrBuf(&local_d8.QuadPart);
  }
  else {
    if (*(short *)(uintptr_t)LVar25.QuadPart == 0x7b) {
      sVar20 = 0x7d;
      goto LAB_14004c59d;
    }
    if (local_168.QuadPart != LVar25.QuadPart) {
      PECMD_StrBldCopyWide(&local_168.QuadPart,(LPCWSTR)(uintptr_t)LVar25.QuadPart);
    }
  }
  local_res10 = local_168;
  PECMD_SkipLeadingControls(&local_res10);
  LVar25 = local_res10;
  if ((*(short *)(uintptr_t)local_res10.QuadPart == 0x23) || (*(short *)(uintptr_t)local_res10.QuadPart == 0)) {
LAB_14004e244:
    PECMD_FreeStrBuf(&local_160.QuadPart);
    goto LAB_14004e29b;
  }
  if ((*(short *)(uintptr_t)local_res10.QuadPart == 0x7b) ||
     ((*(short *)(uintptr_t)local_res10.QuadPart == 0x5b && (*(short *)(local_res10.QuadPart + 2) == 0x5d)))) {
    PECMD_XorEncode((LPCWSTR)(uintptr_t)local_res10.QuadPart,(uint)*(ushort *)(param_1.QuadPart + 0x48),0);
    LVar15.QuadPart = (long long)PECMD_DispatchExpressionBlock((uint64_t)param_1.QuadPart,(uint64_t)(LVar25.QuadPart | 1));
    goto LAB_14004e244;
  }
  local_140 = local_res10;
  if ((*(short *)(uintptr_t)local_res10.QuadPart != 0x46) ||
     ((((*(short *)(local_res10.QuadPart + 2) != 0x49 ||
        (*(short *)(local_res10.QuadPart + 4) != 0x4e)) ||
       (*(short *)(local_res10.QuadPart + 6) != 0x44)) ||
      (LVar11 = local_res10, *(short *)(local_res10.QuadPart + 8) != 0x20)))) {
    PECMD_ExpandVarDispatch((longlong *)(uintptr_t)param_1.QuadPart,(WCHAR *)(uintptr_t)local_res10.QuadPart,&local_160,0,1);
    LVar11 = local_160;
  }
  DVar27 = 0;
  WVar24 = L'\"';
  if ((*(WCHAR *)(uintptr_t)local_res10.QuadPart == L'\"') || (WVar24 = L'|', *(WCHAR *)(uintptr_t)local_res10.QuadPart == L'|')) {
    uVar12 = PECMD_WideStrLen((short *)(uintptr_t)local_res10.QuadPart);
    WVar2 = *(WCHAR *)(uintptr_t)local_res10.QuadPart;
    LVar31.s.LowPart = DVar27;
    LVar33.QuadPart = LVar15.QuadPart;
    for (pWVar23 = (WCHAR *)(local_res10.QuadPart + -2) + (uVar12 & 0xffffffff);
        (WVar2 == WVar24 &&
        (LVar31.s.LowPart = LVar33.s.LowPart, local_140 = LVar25, *pWVar23 == WVar24));
        pWVar23 = pWVar23 + -1) {
      LVar25.QuadPart = LVar25.QuadPart + 2;
      LVar31.s.LowPart = LVar31.s.LowPart + 1;
      LVar31.s.HighPart = 0;
      WVar2 = *(WCHAR *)(uintptr_t)LVar25.QuadPart;
      LVar33 = LVar31;
      local_140.QuadPart = LVar25.QuadPart;
    }
    pWVar23[1] = L'\0';
    if (0 < (int)LVar31.s.LowPart) {
      uVar12 = PECMD_WideStrLen((short *)(uintptr_t)LVar11.QuadPart);
      *(undefined2 *)(LVar11.QuadPart + ((uVar12 & 0xffffffff) - (longlong)(int)LVar31.s.LowPart) * 2) =
           0;
    }
    PECMD_SkipLeadingControls(&local_140);
    LVar11.QuadPart = LVar11.QuadPart + (local_140.QuadPart - (longlong)local_res10.QuadPart >> 1) * 2;
    LVar25 = local_140;
  }
  local_120 = LVar25;
  PECMD_SkipIdentifier(&local_120.QuadPart);
  local_140.QuadPart = local_120.QuadPart - (longlong)LVar25.QuadPart >> 1;
  local_120 = LVar11;
  PECMD_SkipIdentifier(&local_120.QuadPart);
  LVar33 = local_120;
  lVar32 = local_120.QuadPart - (longlong)LVar11.QuadPart;
  PECMD_WideStrLen((short *)(uintptr_t)LVar25.QuadPart);
  local_res8 = LVar33;
  PECMD_SkipLeadingControls(&local_res8);
  LVar33 = local_188;
  uVar29 = local_140.s.LowPart;
  local_158 = 0;
  uVar21 = uVar29;
  if (uVar29 - 3 < 0x7d) {
    uVar7 = *(ushort *)(uintptr_t)LVar25.QuadPart;
    bVar4 = (byte)uVar7;
    if ((ushort)(uVar7 - 0x61) < 0x1a) {
      bVar4 = bVar4 & 0xdf;
LAB_14004c81e:
      pbVar19 = (byte *)((longlong)&local_158 + 1);
    }
    else {
      if (uVar7 < 0x100) goto LAB_14004c81e;
      uVar21 = uVar29 + 1;
      local_158 = (uint)(byte)(uVar7 >> 8) << 8;
      pbVar19 = (byte *)((longlong)&local_158 + 2);
    }
    uVar7 = *(ushort *)(LVar25.QuadPart + 2);
    local_158 = ((local_158 & 0xffffff00U) | (uint8_t)bVar4);
    if ((ushort)(uVar7 - 0x61) < 0x1a) {
      *pbVar19 = (byte)uVar7 & 0xdf;
      pbVar18 = pbVar19 + 1;
    }
    else {
      *pbVar19 = (byte)uVar7;
      pbVar18 = pbVar19 + 1;
      if (0xff < uVar7) {
        *pbVar18 = (byte)(uVar7 >> 8);
        pbVar18 = pbVar19 + 2;
        uVar21 = uVar21 + 1;
      }
    }
    uVar7 = *(ushort *)(LVar25.QuadPart + 4);
    if ((ushort)(uVar7 - 0x61) < 0x1a) {
      *pbVar18 = (byte)uVar7 & 0xdf;
      pbVar19 = pbVar18 + 1;
    }
    else {
      *pbVar18 = (byte)uVar7;
      pbVar19 = pbVar18 + 1;
      if (0xff < uVar7) {
        *pbVar19 = (byte)(uVar7 >> 8);
        pbVar19 = pbVar18 + 2;
        uVar21 = uVar21 + 1;
      }
    }
    if (3 < (int)uVar29) {
      uVar7 = *(ushort *)(LVar25.QuadPart + 6);
      if ((ushort)(uVar7 - 0x61) < 0x1a) {
        *pbVar19 = (byte)uVar7 & 0xdf;
      }
      else {
        *pbVar19 = (byte)uVar7;
        if (0xff < uVar7) {
          uVar21 = uVar21 + 1;
          pbVar19[1] = (byte)(uVar7 >> 8);
        }
      }
    }
  }
  uVar21 = uVar21 << 8 | uVar29;
  if (uVar21 == 0x303) {
    if (local_158 == 0x444553) {
      local_180.QuadPart = (longlong)PECMD_GenerateTextContent((uintptr_t)param_1.QuadPart,LVar25.QuadPart + 6);
      LVar33 = local_188;
    }
    else if (local_158 == 0x524944) {
      local_180.QuadPart = (LONGLONG)PECMD_DirWildcardExpand((uintptr_t)param_1.QuadPart,LVar25.QuadPart + 6);
      LVar33 = local_188;
    }
    else {
      if (local_158 != 0x544553) goto LAB_14004df3c;
      _Var28.dwLowDateTime = (DWORD)(uintptr_t)(LVar25.QuadPart + 6);
      _Var28.dwHighDateTime = (DWORD)((uint64_t)(uintptr_t)(LVar25.QuadPart + 6) >> 32);
      _Var22.dwLowDateTime = (DWORD)(uintptr_t)(LVar11.QuadPart + 6);
      _Var22.dwHighDateTime = (DWORD)((uint64_t)(uintptr_t)(LVar11.QuadPart + 6) >> 32);
      cVar6 = '\x01';
LAB_14004c930:
      local_180.QuadPart = (longlong)PECMD_EvalSpecialToken((uintptr_t)param_1.QuadPart,
    ((uint64_t)_Var22.dwLowDateTime | (uint64_t)_Var22.dwHighDateTime << 32),
    (uint64_t)(uint8_t)cVar6,
    ((uint64_t)_Var28.dwLowDateTime | (uint64_t)_Var28.dwHighDateTime << 32),
    (uint64_t)param_3);
      LVar33 = local_188;
    }
  }
  else {
    if (uVar21 == 0x606) {
      if ((local_158 != 0x45524854) ||
         (uVar14 = FUN_14005c788("THREAD",(ushort *)(uintptr_t)LVar25.QuadPart,6), LVar33 = local_188,
         (char)uVar14 == '\0')) goto LAB_14004df3c;
      if ((char)local_170 != '\0') {
        FUN_140025f10(param_1.QuadPart,(LPCWSTR)(uintptr_t)local_188.QuadPart,0,(pthreadmbcinfo)&DAT_00000011,param_5,
                      (void *)0);
      }
      pWVar23 = (WCHAR *)(LVar25.QuadPart + 0xc);
LAB_14004c9cc:
      local_180.QuadPart = (longlong)PECMD_LaunchAsyncScriptThread((uintptr_t)param_1.QuadPart,pWVar23,(ulonglong)param_5);
      goto LAB_14004c525;
    }
    if (uVar21 != 0x404) {
LAB_14004df3c:
      local_180.QuadPart = 0;
      if (*(short *)(uintptr_t)LVar11.QuadPart == 0x24) {
        PECMD_ParseDateTimeSpec((longlong *)(uintptr_t)param_1.QuadPart,(uint64_t)(uintptr_t)(LVar11.QuadPart + 2),0,0);
        LVar33 = local_188;
        goto LAB_14004c525;
      }
      LVar33 = local_188;
      if (*(short *)(uintptr_t)LVar11.QuadPart == 0) goto LAB_14004c525;
      LVar3 = local_res8.s.HighPart;
      local_res8.QuadPart = local_res8.QuadPart & 0xffffffffffff0000;
      if (*(short *)(uintptr_t)LVar11.QuadPart == 0x40) {
        local_res8.s.HighPart = LVar3;
        local_res8.s.LowPart = 0x40;
        LVar11.QuadPart = LVar11.QuadPart + 2;
      }
      if (((*(short *)(uintptr_t)LVar11.QuadPart == 0x2a) || (*(short *)(uintptr_t)LVar11.QuadPart == 0x2d)) || (*(short *)(uintptr_t)LVar11.QuadPart == 0x2b))
      {
        sVar26 = *(short *)(uintptr_t)LVar11.QuadPart;
      }
      local_d0 = *(longlong *)(param_1.QuadPart + 0x28);
      local_res18[0] = 0;
      PECMD_ParseCommandPath((uint64_t)(uintptr_t)param_1.QuadPart,(uint64_t)(uintptr_t)LVar11.QuadPart,local_res18,&local_d0);
      if ((sVar26 == 0 && ((local_res8.s.LowPart & 0xffffU) == 0)) && ((int)local_res18[0] < 0)) {
        DVar27 = local_140.s.LowPart;
        PECMD_StrBldCopyWideN((uint16_t **)&local_f0,(LPCWSTR)(uintptr_t)LVar25.QuadPart,(int64_t)(int)DVar27);
        if (((int)DVar27 < 1) ||
           (cVar6 = PECMD_DispatchBuiltin((longlong *)(uintptr_t)param_1.QuadPart,param_3,local_f0,
                                  (LPCWSTR)(LVar25.QuadPart + (longlong)(int)DVar27 * 2),
                                  (FILETIME *)&local_180.s), cVar6 == '\0')) {
          PECMD_ExpandVarDispatch((longlong *)(uintptr_t)param_1.QuadPart,(WCHAR *)(uintptr_t)local_res10.QuadPart,&local_160,0,1);
          local_res8 = local_160;
          PECMD_SkipLeadingControls(&local_res8);
          LVar15 = local_res8;
          local_e8 = local_res8;
          local_e0 = 0;
          FUN_140024c48((longlong *)&local_e8.QuadPart,(longlong *)&local_e0,0);
          PECMD_StrBldCopyWideN((uint16_t **)&local_f8,(LPCWSTR)(uintptr_t)local_e8.QuadPart,(int64_t)local_e0);
          local_150[0] = 0;
          uVar14 = PECMD_IsPecmdScriptFile((uint64_t)(uintptr_t)&local_f8);
          if ((int)uVar14 == 1) {
            local_180.QuadPart = (longlong)PECMD_RunCommand((void *)(uintptr_t)param_1.QuadPart,(WCHAR *)(uintptr_t)LVar15.QuadPart);
            PECMD_FreeStrBuf(&local_f8);
            PECMD_FreeStrBuf((WCHAR **)&local_f0);
            LVar33 = local_188;
            goto LAB_14004c53b;
          }
          local_180.QuadPart = (longlong)
                        PECMD_ExecCmdDispatch((uint64_t)param_1.QuadPart,(uint64_t)local_160.QuadPart,(uint64_t)param_1.QuadPart,
    (((uint64_t)(uint16_t)local_150[0]) | ((uint64_t)(uint16_t)local_150[1] << 16) |
     ((uint64_t)(uint16_t)local_150[2] << 32) | ((uint64_t)(uint16_t)local_150[3] << 48)),
    1,(uint64_t)0,(uint64_t)0,(uint64_t)0);          PECMD_FreeStrBuf(&local_f8);
        }
        ppWVar17 = &local_f0;
LAB_14004c51b:
        PECMD_FreeStrBuf((WCHAR **)&ppWVar17);
        LVar33 = local_188;
        goto LAB_14004c525;
      }
LAB_14004e164:
      LVar33 = local_188;
      if ((char)local_170 != '\0') {
        FUN_140025f10(param_1.QuadPart,(LPCWSTR)(uintptr_t)local_188.QuadPart,0,(pthreadmbcinfo)&DAT_00000011,param_5,
                      (void *)0);
      }
      local_180.QuadPart = (longlong)PECMD_DispatchCommand((uintptr_t)param_1.QuadPart,LVar11.s,param_3);
      goto LAB_14004c53b;
    }
    if (local_158 < 0x4d45544a) {
      if (local_158 == 0x4d455449) {
        if (param_3 != (void *)0) {
          local_180.QuadPart = (longlong)
                        PECMD_CreateButtonControl((uintptr_t)param_1.QuadPart,(ushort *)(LVar25.QuadPart + 8),(WPARAM)param_3,
                                      param_5);
          LVar33 = local_188;
        }
      }
      else if (local_158 < 0x455a4954) {
        if (local_158 == 0x455a4953) {
          local_180.QuadPart = PECMD_GetPathSizeToVar((uintptr_t)param_1.QuadPart,LVar11.QuadPart + 8);
          LVar33 = local_188;
        }
        else if (local_158 < 0x45444949) {
          if (local_158 == 0x45444948) {
            local_180.QuadPart = (longlong)PECMD_WritePELogonDword((uintptr_t)local_res8.QuadPart,(const uint16_t *)L"Hide",1);
            LVar33 = local_188;
          }
          else if (local_158 < 0x44494c54) {
            if (local_158 == 0x44494c53) {
              local_180.QuadPart = (longlong)
                            PECMD_ParseControlCenterArgs((uintptr_t)param_1.QuadPart,(ushort *)(LVar25.QuadPart + 8),(WPARAM)param_3,
                                          param_5);
              LVar33 = local_188;
            }
            else if (local_158 == 0x43454a45) {
              local_180.QuadPart = (longlong)PECMD_DriveLetterSet((uintptr_t)param_1.QuadPart,(LPCWSTR)(uintptr_t)(LVar11.QuadPart + 8));
              LVar33 = local_188;
            }
            else if (local_158 == 0x43455845) {
              local_180.QuadPart =
                   (LONGLONG)
                   PECMD_ExecCmdDispatch((uint64_t)param_1.QuadPart,(uint64_t)0,(uint64_t)param_1.QuadPart,
    (uint64_t)(LVar25.QuadPart + 8),0,(uint64_t)0,(uint64_t)0,(uint64_t)0);              LVar33 = local_188;
            }
            else if (local_158 == 0x434c4143) {
              local_180.QuadPart = (longlong)
                            PECMD_CalcExpression((uintptr_t)param_1.QuadPart,(pthreadmbcinfo)(LVar25.QuadPart + 8),
                                          (undefined8 *)0x0);
              LVar33 = local_188;
            }
            else {
              if (local_158 == 0x4350544e) {
                uVar14 = PECMD_SntpResolveServer((longlong *)(uintptr_t)param_1.QuadPart,(LPCWSTR)(LVar11.QuadPart + 8));
                iVar8 = (int)uVar14;
                goto LAB_14004cb4c;
              }
              if (local_158 == 0x44414552) {
                uVar14 = FUN_14005c788("READ",(ushort *)(uintptr_t)LVar11.QuadPart,4);
                LVar33 = local_188;
                if (((char)uVar14 != '\0') &&
                   (((int)(lVar32 >> 1) == 4 || (*(short *)(LVar11.QuadPart + 8) == 0x2d)))) {
                  local_180.QuadPart = (longlong)PECMD_ReadTextLine((uintptr_t)param_1.QuadPart,LVar25.QuadPart + 8);
                  LVar33 = local_188;
                }
              }
              else {
                if (local_158 == 0x44414f4c) {
                  if ((char)local_170 != '\0') {
                    FUN_140025f10(param_1.QuadPart,(LPCWSTR)(uintptr_t)local_188.QuadPart,0,
                                  (pthreadmbcinfo)&DAT_00000011,param_5,(void *)0);
                  }
                  local_188.QuadPart = LVar25.QuadPart + 8;
                  PECMD_SkipLeadingControls(&local_188);
                  local_180.QuadPart = (longlong)PECMD_RunCommand((void *)(uintptr_t)param_1.QuadPart,(WCHAR *)(uintptr_t)local_188.QuadPart);
                  PECMD_NotifyMainWindowRefresh((uint64_t)(uintptr_t)param_1.QuadPart,1);
                  goto LAB_14004c53b;
                }
                if (local_158 != 0x44415049) goto LAB_14004df3c;
                local_180.QuadPart = (longlong)
                              PECMD_MsgDispatch((uintptr_t)param_1.QuadPart,(ushort *)(LVar25.QuadPart + 8),
                                            (WPARAM)param_3,param_5);
                LVar33 = local_188;
              }
            }
          }
          else if (local_158 == 0x444d4152) {
            local_180.QuadPart = (longlong)PECMD_RunRamdriv((uintptr_t)param_1.QuadPart,(uintptr_t)local_res8.QuadPart);
            LVar33 = local_188;
          }
          else if (local_158 == 0x444e4152) {
            local_180.QuadPart = (longlong)PECMD_EvalRandFunction((uintptr_t)param_1.QuadPart,(uint64_t)(uintptr_t)(LVar11.QuadPart + 8));
            LVar33 = local_188;
          }
          else if (local_158 == 0x444e4553) {
            local_180.QuadPart = (longlong)PECMD_SendInputEvents((uintptr_t)local_res8.QuadPart);
            LVar33 = local_188;
          }
          else if (local_158 == 0x444e4946) {
            if ((char)local_170 != '\0') {
              FUN_140025f10(param_1.QuadPart,(LPCWSTR)(uintptr_t)local_188.QuadPart,0,(pthreadmbcinfo)&DAT_00000011,
                            param_5,(void *)0);
            }
            plVar10 = local_res20;
            if (*(short *)(LVar25.QuadPart + 8U) == 0x7c) {
              plVar10 = (void *)0;
            }
            local_180.QuadPart = (longlong)
                          PECMD_FindCmdHandler((uintptr_t)param_1.QuadPart,(uint64_t)(uintptr_t)(LVar25.QuadPart + 8U),plVar10,
                                        param_5);
          }
          else {
            if (local_158 == 0x44524854) {
              if ((char)local_170 != '\0') {
                FUN_140025f10(param_1.QuadPart,(LPCWSTR)(uintptr_t)local_188.QuadPart,0,(pthreadmbcinfo)&DAT_00000011,
                              param_5,(void *)0);
              }
              pWVar23 = (WCHAR *)(LVar25.QuadPart + 8);
              goto LAB_14004c9cc;
            }
            if (local_158 != 0x4542414c) goto LAB_14004df3c;
            if (param_3 != (void *)0) {
              local_180.QuadPart = (longlong)
                            PECMD_AddTransControl((uintptr_t)param_1.QuadPart,(ushort *)(LVar25.QuadPart + 8),(WPARAM)param_3,
                                          param_5);
              LVar33 = local_188;
            }
          }
        }
        else if (local_158 < 0x454d4f44) {
          if (local_158 == 0x454d4f43) {
LAB_14004ce16:
            local_170 = (undefined8)(uintptr_t)(LVar11.QuadPart + 8);
            PECMD_SkipLeadingControls(&local_170);
            pWVar30 = (LPCWSTR)(uintptr_t)local_170;
            if ((((ushort)*(const uint16_t *)(uintptr_t)local_170 < 0x31) || (0x39 < (ushort)*(const uint16_t *)(uintptr_t)local_170)) &&
               (iVar8 = StrCmpNIW(local_170,L"ON",2), iVar8 != 0)) {
              if ((*pWVar30 == L'0') || (iVar8 = StrCmpNIW(pWVar30,(const uint16_t *)L"OFF",3), iVar8 == 0)) {
                g_charTableF = 0;
              }
            }
            else {
              g_charTableF = 1;
            }
            local_180.QuadPart = 0;
            LVar33 = local_188;
          }
          else if (local_158 == 0x45444f43) {
            local_180.QuadPart = (longlong)PECMD_CodeConvertCommand((uintptr_t)param_1.QuadPart,(WCHAR *******)(LVar25.QuadPart + 8));
            LVar33 = local_188;
          }
          else if (local_158 == 0x45455254) {
            local_180.QuadPart = (longlong)
                          PECMD_ParseControlDef((uintptr_t)param_1.QuadPart,(LPCWSTR)(LVar25.QuadPart + 8),(WPARAM)param_3);
            LVar33 = local_188;
          }
          else if (local_158 == 0x45474150) {
            local_180.QuadPart = (longlong)PECMD_CreatePageFile((uintptr_t)local_res8.QuadPart);
            LVar33 = local_188;
          }
          else if (local_158 == 0x454c4946) {
            local_180 = FUN_14003C06C((int64_t *)(uintptr_t)param_1.QuadPart,(uint64_t)local_res8.QuadPart,0);
            LVar33 = local_188;
          }
          else {
            if (local_158 == 0x454d414e) goto LAB_14004d206;
            if (local_158 != 0x454d4954) goto LAB_14004df3c;
            if (param_3 != (void *)0) {
              local_180.QuadPart = (longlong)
                            PECMD_PcipDispatch((uintptr_t)param_1.QuadPart,(ushort *)(LVar25.QuadPart + 8),(WPARAM)param_3,
                                          param_5);
              LVar33 = local_188;
            }
          }
        }
        else if (local_158 == 0x454d4f48) {
          local_180.QuadPart = (longlong)PECMD_HomeSetStartPage((uintptr_t)param_1.QuadPart,(uint64_t)(uintptr_t)(LVar25.QuadPart + 8));
          LVar33 = local_188;
        }
        else if (local_158 == 0x45534142) {
          local_180.QuadPart = (longlong)PECMD_AttachControlImage((uintptr_t)param_1.QuadPart,(LPCWSTR)(LVar25.QuadPart + 8));
          LVar33 = local_188;
        }
        else if (local_158 == 0x45544144) {
          local_180.QuadPart = (longlong)PECMD_FormatTimeString((uintptr_t)param_1.QuadPart,(WCHAR *)(LVar11.QuadPart + 8));
          LVar33 = local_188;
        }
        else if (local_158 == 0x45544953) {
          local_180.QuadPart = (LONGLONG)PECMD_SiteSetFileAttrTime((uintptr_t)param_1.QuadPart,(uint64_t)(LVar25.QuadPart + 8),(uint64_t)(uintptr_t)param_3);
          LVar33 = local_188;
        }
        else {
          if (local_158 == 0x45544f4e) goto LAB_14004ce16;
          if (local_158 != 0x45564948) goto LAB_14004df3c;
          local_180.QuadPart = (longlong)PECMD_LoadUnloadRegistryHive((uintptr_t)param_1.QuadPart,(ushort *)(LVar11.QuadPart + 8));
          LVar33 = local_188;
        }
      }
      else if (local_158 < 0x4b4d554f) {
        if (local_158 == 0x4b4d554e) {
          local_180.QuadPart = (longlong)PECMD_SetNumLockState((uintptr_t)local_res8.QuadPart);
          LVar33 = local_188;
        }
        else if (local_158 < 0x49474553) {
          if (local_158 == 0x49474552) {
            local_180.QuadPart = (longlong)PECMD_RegiEditRegistry((uintptr_t)param_1.QuadPart,(uint64_t)(uintptr_t)(LVar25.QuadPart + 8),(uint64_t)'\0');
            LVar33 = local_188;
          }
          else if (local_158 == 0x46544547) {
            local_180.QuadPart = (longlong)PECMD_GetfReadData((uintptr_t)param_1.QuadPart,(uint64_t)(LVar25.QuadPart + 8));
            LVar33 = local_188;
          }
          else if (local_158 == 0x46545550) {
            local_180.QuadPart = (longlong)PECMD_DdCopyCommand((uintptr_t)param_1.QuadPart,(uint64_t)(LVar25.QuadPart + 8));
            LVar33 = local_188;
          }
          else if (local_158 == 0x46574246) {
            local_180.QuadPart = (longlong)PECMD_SetFbwfThreshold(param_1,(uintptr_t)local_res8.QuadPart);
            LVar33 = local_188;
          }
          else if (local_158 == 0x47414d49) {
            if (uVar29 == 4) {
              local_180.QuadPart = (longlong)
                            PECMD_ImageCommand((uintptr_t)param_1.QuadPart,(LPCWSTR)(LVar25.QuadPart + 8),(WPARAM)param_3);
              LVar33 = local_188;
            }
          }
          else if (local_158 == 0x48534148) {
            local_180.QuadPart = (longlong)
                          PECMD_HashCmdCompute((uintptr_t)param_1.QuadPart,(uint64_t)(uintptr_t)(LVar11.QuadPart + 8),(uint64_t)0);
            LVar33 = local_188;
          }
          else if (local_158 == 0x48544150) {
            local_180.s.LowPart = PECMD_DispatchSpecialDirective((longlong *)(uintptr_t)param_1.QuadPart,(short *)(uintptr_t)local_res8.QuadPart);
            local_180.s.HighPart = extraout_var_00;
            LVar33 = local_188;
          }
          else {
            if (local_158 != 0x49444152) goto LAB_14004df3c;
            if ((param_3 != (void *)0) && (uVar29 == 4)) {
              local_180.QuadPart = (longlong)
                            PECMD_ParseListControl((uintptr_t)param_1.QuadPart,(ushort *)(LVar25.QuadPart + 8),(WPARAM)param_3,
                                          param_5);
              LVar33 = local_188;
            }
          }
        }
        else if (local_158 == 0x49564544) {
          local_180.QuadPart = (longlong)PECMD_DriverInstall((int64_t *)(uintptr_t)param_1.QuadPart,(LPCWSTR)(uintptr_t)local_res8.QuadPart);
          LVar33 = local_188;
        }
        else {
          if (local_158 == 0x49564e45) {
            _Var28.dwLowDateTime = (DWORD)(uintptr_t)(LVar25.QuadPart + 8);
            _Var28.dwHighDateTime = (DWORD)((uint64_t)(uintptr_t)(LVar25.QuadPart + 8) >> 32);
            _Var22.dwLowDateTime = (DWORD)(uintptr_t)(LVar11.QuadPart + 8);
            _Var22.dwHighDateTime = (DWORD)((uint64_t)(uintptr_t)(LVar11.QuadPart + 8) >> 32);
            cVar6 = '\0';
            goto LAB_14004c930;
          }
          if (local_158 == 0x4a425553) {
            local_180.QuadPart = (longlong)PECMD_DefineDosDevice((uintptr_t)param_1.QuadPart,(WCHAR *)(LVar11.QuadPart + 8));
            LVar33 = local_188;
          }
          else if (local_158 == 0x4b434f4c) {
            local_180.QuadPart = (longlong)PECMD_LoadTasksWait((uintptr_t)param_1.QuadPart,(LPCWSTR)(LVar11.QuadPart + 8),0);
            LVar33 = local_188;
          }
          else if (local_158 == 0x4b434f53) {
            local_180.QuadPart = (longlong)PECMD_SockCreateIpcObj((uintptr_t)param_1.QuadPart,(uint64_t)(uintptr_t)(LVar11.QuadPart + 8),(WPARAM)param_3);
            LVar33 = local_188;
          }
          else {
            if (local_158 != 0x4b454843) goto LAB_14004df3c;
            if (param_3 != (void *)0) {
              local_180.QuadPart = (longlong)
                            PECMD_ParseComboList((uintptr_t)param_1.QuadPart,(ushort *)(LVar25.QuadPart + 8),(WPARAM)param_3,
                                          param_5);
              LVar33 = local_188;
            }
          }
        }
      }
      else if (local_158 < 0x4c4c4144) {
        if (local_158 == 0x4c4c4143) {
          LVar11 = local_res8;
          if (*(short *)(uintptr_t)local_res8.QuadPart != 0x24) goto LAB_14004e164;
          local_180.QuadPart = (longlong)PECMD_ParseDateTimeSpec((uintptr_t)param_1.QuadPart,local_res8.QuadPart + 2,0,LVar25.s);
          LVar33 = local_188;
        }
        else if (local_158 == 0x4b4e494c) {
          local_180.QuadPart = (longlong)PECMD_LinkCreateShortcut((uintptr_t)param_1.QuadPart,(uint64_t)(uintptr_t)(LVar25.QuadPart + 8));
          LVar33 = local_188;
        }
        else if (local_158 == 0x4b4e4c46) {
          local_180.QuadPart = (longlong)PECMD_LinkFile((uintptr_t)param_1.QuadPart,(LPCWSTR)(LVar25.QuadPart + 8));
          LVar33 = local_188;
        }
        else if (local_158 == 0x4b534944) {
          local_180.QuadPart = (longlong)PECMD_DiskFindLoadHives((uintptr_t)param_1.QuadPart,(uint64_t)(uintptr_t)(LVar11.QuadPart + 8));
          LVar33 = local_188;
        }
        else if (local_158 == 0x4b544f48) {
          local_180.QuadPart = (longlong)PECMD_HotkeyControl((uintptr_t)param_1.QuadPart,(wchar_t *)(LVar25.QuadPart + 8),0);
          LVar33 = local_188;
        }
        else if (local_158 == 0x4c424154) {
          local_180.QuadPart = (longlong)
                        PECMD_TablCreateControl((uintptr_t)param_1.QuadPart,(uint64_t)(uintptr_t)(LVar25.QuadPart + 8),(WPARAM)param_3);
          LVar33 = local_188;
        }
        else {
          if (local_158 != 0x4c454853) goto LAB_14004df3c;
          local_180.QuadPart = (longlong)PECMD_LaunchShell((uintptr_t)local_res8.QuadPart);
          LVar33 = local_188;
        }
      }
      else if (local_158 == 0x4c4c4157) {
        local_180.QuadPart = (longlong)PECMD_ApplyWallpaper((uintptr_t)local_res8.QuadPart);
        LVar33 = local_188;
      }
      else if (local_158 == 0x4c4c494b) {
        local_res8.QuadPart = LVar25.QuadPart + 8;
        uVar12 = 1;
        if (*(short *)(uintptr_t)local_res8.QuadPart == 0x2a) {
          local_res8.QuadPart = LVar25.QuadPart + 10;
          uVar12 = 1;
          if (*(short *)(uintptr_t)local_res8.QuadPart == 0x2a) {
            uVar12 = 2;
            do {
              local_res8.QuadPart = local_res8.QuadPart + 2;
            } while (*(short *)(uintptr_t)local_res8.QuadPart == 0x2a);
          }
        }
        PECMD_SkipLeadingControls(&local_res8);
        LVar15 = local_res8;
        if ((*(short *)(uintptr_t)local_res8.QuadPart != 0x5c) && (*(short *)(uintptr_t)local_res8.QuadPart != 0x40)) {
          iVar8 = lstrlenA("*svr");
          lVar32 = PECMD_TokPrefixICmp("*svr",(ushort *)(uintptr_t)LVar15.QuadPart,iVar8);
          if ((char)lVar32 == '\0') {
            local_180.QuadPart = (longlong)PECMD_CmdKill((uintptr_t)param_1.QuadPart,(uintptr_t)LVar15.QuadPart);
            LVar33 = local_188;
            goto LAB_14004c525;
          }
        }
        local_180.QuadPart = (longlong)PECMD_CloseRestartByName((longlong *)(uintptr_t)param_1.QuadPart,(LPCWSTR)(uintptr_t)LVar15.QuadPart,(longlong)param_3,uVar12);
        LVar33 = local_188;
      }
      else if (local_158 == 0x4c525453) {
        local_180.QuadPart = (longlong)PECMD_WriteVarEncoded((uintptr_t)param_1.QuadPart,(LPCWSTR)(LVar11.QuadPart + 8));
        LVar33 = local_188;
      }
      else if (local_158 == 0x4c534441) {
        local_180.QuadPart = (longlong)PECMD_WlanManage((uintptr_t)param_1.QuadPart,(LPCWSTR)(LVar11.QuadPart + 8));
        LVar33 = local_188;
      }
      else {
        if (local_158 == 0x4d414554) {
          local_180.QuadPart = PECMD_ParseValueCommand(param_1.QuadPart,LVar25.QuadPart + 8,param_5);
          LVar33 = local_188;
          goto LAB_14004c53b;
        }
        if (local_158 != 0x4d414e46) goto LAB_14004df3c;
LAB_14004d206:
        local_180.QuadPart = (longlong)PECMD_ParseExtPathArg((uintptr_t)param_1.QuadPart,(short *)(LVar11.QuadPart + 8));
        LVar33 = local_188;
      }
    }
    else if (local_158 < 0x534f504d) {
      if (local_158 == 0x534f504c) {
        bVar4 = 0;
LAB_14004d781:
        local_180.QuadPart = (longlong)
                      PECMD_SearchStringAndLocate((uintptr_t)param_1.QuadPart,(WCHAR *)(LVar25.QuadPart + 8),param_3,bVar4);
        LVar33 = local_188;
      }
      else if (local_158 < 0x504f4f4d) {
        if (local_158 == 0x504f4f4c) {
          if ((char)local_170 != '\0') {
            FUN_140025f10(param_1.QuadPart,(LPCWSTR)(uintptr_t)local_188.QuadPart,0,(pthreadmbcinfo)&DAT_00000011,
                          param_5,(void *)0);
          }
          local_180.QuadPart = (longlong)
                        PECMD_ExecPathList((uintptr_t)param_1.QuadPart,(LPCWSTR)(LVar25.QuadPart + 8),param_3,local_res20,
                                      param_5);
        }
        else if (local_158 < 0x4e574f45) {
          if (local_158 == 0x4e574f44) {
            local_180.QuadPart = PECMD_DownCheckShutdown();
            LVar33 = local_188;
          }
          else if (local_158 == 0x4d495444) {
            local_180.QuadPart = (longlong)
                          PECMD_ParseMsgDispatch((uintptr_t)param_1.QuadPart,(ushort *)(LVar25.QuadPart + 8),(WPARAM)param_3,
                                        param_5);
            LVar33 = local_188;
          }
          else if (local_158 == 0x4d524f46) {
            local_180.QuadPart = (longlong)PECMD_GetDiskFreeSpace((uintptr_t)param_1.QuadPart,(WCHAR *)(LVar11.QuadPart + 8));
            LVar33 = local_188;
          }
          else {
            if (local_158 == 0x4e49414d) {
              LVar25 = local_res8;
              if ((char)local_170 != '\0') {
                FUN_140025f10(param_1.QuadPart,(LPCWSTR)(uintptr_t)local_188.QuadPart,0,(pthreadmbcinfo)&DAT_00000011,
                              param_5,(void *)0);
                LVar25 = local_res8;
              }
              for (; *(short *)(uintptr_t)LVar25.QuadPart == 0x2a; LVar25.QuadPart = LVar25.QuadPart + 2) {
                lVar32 = PECMD_TokPrefixICmp("**u",(ushort *)(uintptr_t)LVar25.QuadPart,3);
                if ((char)lVar32 != '\0') goto LAB_14004d479;
              }
              if (*(short *)(uintptr_t)LVar25.QuadPart != 0) {
                PECMD_StrBldCopyWide((long long *)&g_pNextCmd,(const uint16_t *)L"PECMD ");
                PECMD_AppendWideStr(&g_pNextCmd,(LPCWSTR)(uintptr_t)LVar11.QuadPart);
                *(undefined2 *)(param_1.QuadPart + 200) = 2;
                goto LAB_14004e244;
              }
LAB_14004d479:
              local_180.QuadPart =
                   PECMD_ScriptMainEntry((uint64_t)(uintptr_t)&DAT_14013d130,(uint64_t)(uintptr_t)(LVar11.QuadPart + 8));
              goto LAB_14004c525;
            }
            if (local_158 == 0x4e495053) {
              local_180.QuadPart = (longlong)
                            PECMD_SpinAddControl((uintptr_t)param_1.QuadPart,(uint64_t)(uintptr_t)(LVar25.QuadPart + 8),(WPARAM)param_3,
                                          (uint64_t)(uintptr_t)param_5);
              LVar33 = local_188;
            }
            else if (local_158 == 0x4e495753) {
              if (param_3 != (void *)0) {
                local_180.QuadPart = (longlong)
                              PECMD_SubCommand((uintptr_t)param_1.QuadPart,(WCHAR *)(LVar11.QuadPart + 8),param_3);
                LVar33 = local_188;
              }
            }
            else if (local_158 == 0x4e524353) {
              local_180.QuadPart = (longlong)PECMD_ScreenAreaCommand((uintptr_t)param_1.QuadPart,(LPCWSTR)(LVar11.QuadPart + 8));
              LVar33 = local_188;
            }
            else {
              if (local_158 != 0x4e554f4d) goto LAB_14004df3c;
              local_180.QuadPart =
                   (LONGLONG)PECMD_MounMountWimImage((uintptr_t)param_1.QuadPart,LVar25.QuadPart + 8,(WPARAM)param_3);
              LVar33 = local_188;
            }
          }
        }
        else if (local_158 == 0x4f474f4c) {
          local_180.QuadPart = (longlong)PECMD_LaunchCommand((ushort *)(uintptr_t)(LVar11.QuadPart + 8));
          LVar33 = local_188;
        }
        else if (local_158 == 0x4f4d454d) {
          if (param_3 != (void *)0) {
            local_180.QuadPart = (longlong)
                          PECMD_MsgCommand((uintptr_t)param_1.QuadPart,(WCHAR *)(LVar25.QuadPart + 8),(WPARAM)param_3,
                                        param_5);
            LVar33 = local_188;
          }
        }
        else if (local_158 == 0x50494350) {
          local_180.QuadPart = (longlong)PECMD_PcipCommand((uintptr_t)param_1.QuadPart,(LPCWSTR)(LVar25.QuadPart + 8));
          LVar33 = local_188;
        }
        else {
          if (local_158 == 0x504c4548) {
            pHVar13 = (HRSRC)(uintptr_t)FindResourceW(DAT_14013ca68,(LPCWSTR)(uintptr_t)0x12e,(LPCWSTR)(uintptr_t)5);
            LVar15.QuadPart = 0;
            if (pHVar13 != (HRSRC)0x0) {
              uVar14 = PECMD_ParseHlpDoc(param_1,(LPCWSTR)(LVar11.QuadPart + 8));
              iVar8 = (int)uVar14;
              goto LAB_14004cb4c;
            }
            goto LAB_14004e244;
          }
          if (local_158 == 0x504d4554) {
            local_180.QuadPart =
                 (LONGLONG)PECMD_TempPathCommand((uintptr_t)param_1.QuadPart,LVar11.QuadPart + 8,(longlong)param_3);
            LVar33 = local_188;
          }
          else {
            if (local_158 != 0x504e5055) goto LAB_14004df3c;
            uVar12 = PECMD_RunExeIndata((LPCWSTR)(LVar11.QuadPart + 8));
            iVar8 = (int)uVar12;
LAB_14004cb4c:
            local_180.QuadPart = (longlong)iVar8;
            LVar33 = local_188;
          }
        }
      }
      else if (local_158 < 0x5254534d) {
        if (local_158 == 0x5254534c) {
          local_180.QuadPart = (longlong)PECMD_WriteVarTruncated((uintptr_t)param_1.QuadPart,(LPCWSTR)(LVar11.QuadPart + 8));
          LVar33 = local_188;
        }
        else {
          if (local_158 == 0x50534944) {
            uVar12 = PECMD_DispCommand((uint64_t)param_1.QuadPart,(uint64_t)(uintptr_t)local_res8.QuadPart);
            iVar8 = (int)uVar12;
            goto LAB_14004cb4c;
          }
          if (local_158 == 0x52414250) {
            if (param_3 != (void *)0) {
              local_180.QuadPart = (longlong)
                            PECMD_ParseControlCommand((uintptr_t)param_1.QuadPart,(LPWSTR)(LVar11.QuadPart + 8),(WPARAM)param_3);
              LVar33 = local_188;
            }
          }
          else if (local_158 == 0x52414253) {
            local_180.QuadPart = (longlong)
                          PECMD_SbarAddControl((uintptr_t)param_1.QuadPart,(uint64_t)(uintptr_t)(LVar25.QuadPart + 8),(WPARAM)param_3,0)
            ;
            LVar33 = local_188;
          }
          else if (local_158 == 0x52455355) {
            local_180.QuadPart = PECMD_UserCmdHandler(LVar25.QuadPart + 8);
            LVar33 = local_188;
          }
          else if (local_158 == 0x52494446) {
            local_180.QuadPart = (longlong)PECMD_NormalizePath((uintptr_t)param_1.QuadPart,(LPCWSTR)(LVar11.QuadPart + 8));
            LVar33 = local_188;
          }
          else {
            if (local_158 != 0x5249444d) goto LAB_14004df3c;
            local_180.s.LowPart = PECMD_CreateDirectory(param_1,(short *)(uintptr_t)local_res8.QuadPart);
            local_180.s.HighPart = extraout_var_01;
            LVar33 = local_188;
          }
        }
      }
      else if (local_158 == 0x5254534d) {
        local_180.QuadPart = (longlong)PECMD_MstrStringOps((uintptr_t)param_1.QuadPart,(uint64_t)(uintptr_t)(LVar11.QuadPart + 8));
        LVar33 = local_188;
      }
      else if (local_158 == 0x52545352) {
        local_180.QuadPart = (longlong)PECMD_SetVarRange((uintptr_t)param_1.QuadPart,(LPCWSTR)(LVar11.QuadPart + 8));
        LVar33 = local_188;
      }
      else {
        if (local_158 == 0x52545353) {
          bVar4 = 1;
          goto LAB_14004d781;
        }
        if (local_158 == 0x53424154) {
          if (param_3 != (void *)0) {
            local_180.QuadPart = (longlong)
                          PECMD_AddControlStar((uintptr_t)param_1.QuadPart,(ushort *)(LVar11.QuadPart + 8),(WPARAM)param_3);
            LVar33 = local_188;
          }
        }
        else if (local_158 == 0x53474f4c) {
          local_180.QuadPart = (longlong)
                        PECMD_ScriptInitParse((uintptr_t)param_1.QuadPart,(uint64_t)(uintptr_t)(LVar11.QuadPart + 8),
                                      (uint64_t)(uintptr_t)((pthreadmbcinfo)((ulonglong)(local_188.QuadPart == 0) | (ulonglong)param_5)));
          LVar33.QuadPart = LVar15.QuadPart;
        }
        else {
          if (local_158 != 0x534e5552) goto LAB_14004df3c;
          local_180.QuadPart = (LONGLONG)PECMD_SetRegistryOwnerRun((uintptr_t)(LVar25.QuadPart + 8),'\x04');
          LVar33 = local_188;
        }
      }
    }
    else if (local_158 < 0x5453494d) {
      if (local_158 == 0x5453494c) {
        if ((param_3 != (void *)0) && (uVar29 == 4)) {
          local_180.QuadPart = (longlong)
                        PECMD_AddControlWide((uintptr_t)param_1.QuadPart,(WCHAR *)(LVar25.QuadPart + 8),(WPARAM)param_3,
                                      param_5);
          LVar33 = local_188;
        }
      }
      else if (local_158 < 0x54494e4a) {
        if (local_158 == 0x54494e49) {
          local_180.QuadPart = (longlong)PECMD_ProcessInitCommand((uintptr_t)param_1.QuadPart,(uintptr_t)local_res8.QuadPart);
          LVar33 = local_188;
        }
        else if (local_158 == 0x534f5052) {
          local_180.QuadPart = (longlong)PECMD_VarSearchReplace((uintptr_t)param_1.QuadPart,(LPCWSTR)(LVar25.QuadPart + 8));
          LVar33 = local_188;
        }
        else if (local_158 == 0x53504954) {
          local_180.QuadPart = (longlong)
                        PECMD_LoadIconAndTooltip((uintptr_t)param_1.QuadPart,(LPCWSTR)(LVar25.QuadPart + 8),(longlong)param_3);
          LVar33 = local_188;
        }
        else if (local_158 == 0x53504d43) {
          local_180.QuadPart = (longlong)PECMD_ProcessEncodedFile((uintptr_t)param_1.QuadPart,(WCHAR *)(LVar11.QuadPart + 8));
          LVar33 = local_188;
        }
        else if (local_158 == 0x5353454d) {
          local_180.QuadPart = (longlong)PECMD_MessShowMsgBox((uintptr_t)param_1.QuadPart,(uint64_t)(uintptr_t)(LVar25.QuadPart + 8),(uint64_t)(uintptr_t)param_3);
          LVar33 = local_188;
        }
        else if (local_158 == 0x54494157) {
          local_180.QuadPart = (longlong)PECMD_DispatchSystemCommandLine((uintptr_t)param_1.QuadPart,(uintptr_t)local_res8.QuadPart);
          LVar33 = local_188;
        }
        else {
          if (local_158 != 0x54494445) goto LAB_14004df3c;
          if (param_3 != (void *)0) {
            local_180.QuadPart = (longlong)
                          PECMD_CreateTextControl((uintptr_t)param_1.QuadPart,(WCHAR *)(LVar25.QuadPart + 8),(WPARAM)param_3,
                                        param_5);
            LVar33 = local_188;
          }
        }
      }
      else if (local_158 == 0x54495257) {
        local_180.QuadPart = (longlong)PECMD_WriteFileEncoded((uintptr_t)param_1.QuadPart,LVar25.QuadPart + 8);
        LVar33 = local_188;
      }
      else {
        if (local_158 == 0x54495845) {
          puVar1 = (ushort *)(LVar11.QuadPart + 8);
          if (*puVar1 == 0x3d) {
            local_100 = (LPCWSTR)(LVar11.QuadPart + 10);
            local_180.QuadPart = 0;
            local_108.QuadPart = 0;
            PECMD_SkipLeadingControls(&local_100);
            uVar14 = FUN_14005c7c4("error",(ushort *)local_100);
            if ((char)uVar14 == '\0') {
              uVar12 = PECMD_EvalParenStripped((longlong *)&local_100,(ulonglong *)&local_108.QuadPart);
              if ((((int)uVar12 < 1) && (*local_100 != L'\0')) &&
                 (lVar32 = PECMD_VarLookup((longlong *)(uintptr_t)param_1.QuadPart,local_100,(void *)0,-1,
                                         (undefined8 *)0x0), lVar32 != 0)) {
                local_108 = (LARGE_INTEGER)((LARGE_INTEGER *)(lVar32 + 8))->QuadPart;
              }
            }
            else {
              local_108 = (LARGE_INTEGER)(*(LARGE_INTEGER **)(param_1.QuadPart + 0x50))->QuadPart;
            }
            (*(LARGE_INTEGER **)(uintptr_t)(param_1.QuadPart + 0x50))->QuadPart = (LONGLONG)local_108.QuadPart;
            ((LARGE_INTEGER *)(uintptr_t)(param_1.QuadPart + 0xd0))->QuadPart = (LONGLONG)local_108.QuadPart;
            *(undefined1 *)(param_1.QuadPart + 0xca) = 1;
            LVar33 = local_188;
            local_180 = local_108;
            goto LAB_14004c53b;
          }
          if (((*puVar1 < 9) || (0xd < *puVar1)) && ((*puVar1 != 0x20 && (*puVar1 != 0))))
          goto LAB_14004c525;
          local_138 = puVar1;
          PECMD_SkipLeadingControls(&local_138);
          local_118[0] = *(int *)(param_1.QuadPart + 0xd0);
          PECMD_AllocStrSlot((uint16_t **)&local_178);
          PECMD_SplitTokenTrimWs((longlong *)&local_138,(longlong *)&local_178,0x2c);
          if (*local_138 == 0x2c) {
            local_138 = local_138 + 1;
            bVar5 = PECMD_ParseUIntValue((longlong *)&local_138,local_118);
            DVar27 = (DWORD)(((uint64_t)extraout_var << 8) | (uint8_t)bVar5);
          }
          if (*local_178 == L'\0') {
            uVar7 = 0x40;
          }
          else {
            uVar14 = FUN_14005c7c4("_SUB",(ushort *)local_178);
            if (((char)uVar14 == '\0') &&
               (uVar14 = FUN_14005c7c4("1",(ushort *)local_178), (char)uVar14 == '\0')) {
              uVar14 = FUN_14005c7c4("FILE",(ushort *)local_178);
              if (((char)uVar14 == '\0') &&
                 (uVar14 = FUN_14005c7c4("2",(ushort *)local_178), (char)uVar14 == '\0')) {
                uVar14 = FUN_14005c7c4("FORX",(ushort *)local_178);
                if (((char)uVar14 == '\0') &&
                   (uVar14 = FUN_14005c7c4("3",(ushort *)local_178), (char)uVar14 == '\0')) {
                  uVar14 = FUN_14005c7c4("LOOP",(ushort *)local_178);
                  if (((char)uVar14 == '\0') &&
                     (uVar14 = FUN_14005c7c4("4",(ushort *)local_178), (char)uVar14 == '\0')) {
                    uVar14 = FUN_14005c7c4("-",(ushort *)local_178);
                    if (((char)uVar14 == '\0') &&
                       (uVar14 = FUN_14005c7c4("BLOCK",(ushort *)local_178), (char)uVar14 == '\0'))
                    {
                      uVar14 = FUN_14005c7c4("ToWIN",(ushort *)local_178);
                      if ((char)uVar14 == '\0') {
                        uVar14 = FUN_14005c7c4("WIN",(ushort *)local_178);
                        if ((char)uVar14 == '\0') {
                          uVar14 = FUN_14005c7c4("CONTINUE",(ushort *)local_178);
                          if ((char)uVar14 == '\0') {
                            uVar14 = FUN_14005c7c4("BREAK",(ushort *)local_178);
                            if ((char)uVar14 == '\0') {
                              ppWVar17 = &local_178;
                              goto LAB_14004c51b;
                            }
                            uVar7 = 0x20;
                          }
                          else {
                            uVar7 = 0x100;
                          }
                        }
                        else {
                          *(ushort *)(param_1.QuadPart + 200) =
                               *(ushort *)(param_1.QuadPart + 200) | 0x80;
                          local_res10.s.LowPart = ((local_res10.s.LowPart & 0xffff0000U) | 0x80);
                          PECMD_CloseRestartByName((longlong *)(uintptr_t)param_1.QuadPart,(const uint16_t *)L"\\",(longlong)param_3,1);
                          uVar7 = (ushort)local_res10.s.LowPart;
                        }
                      }
                      else {
                        uVar7 = 0x80;
                      }
                    }
                    else {
                      uVar7 = 0x10;
                    }
                  }
                  else {
                    uVar7 = 8;
                  }
                }
                else {
                  uVar7 = 4;
                }
              }
              else {
                uVar7 = 2;
              }
            }
            else {
              uVar7 = 1;
            }
          }
          *(ushort *)(param_1.QuadPart + 200) = *(ushort *)(param_1.QuadPart + 200) | uVar7;
          if (0 < (int)DVar27) {
            **(longlong **)(param_1.QuadPart + 0x50) = (longlong)local_118[0];
            uVar14 = **(undefined8 **)(param_1.QuadPart + 0x50);
            *(undefined1 *)(param_1.QuadPart + 0xca) = 1;
            *(undefined8 *)(param_1.QuadPart + 0xd0) = uVar14;
          }
          if ((char)local_170 != '\0') {
            FUN_140025f10(param_1.QuadPart,(LPCWSTR)(uintptr_t)local_188.QuadPart,0,(pthreadmbcinfo)&DAT_00000011,
                          param_5,(void *)0);
          }
          PECMD_FreeStrBuf((WCHAR **)&local_178);
          LVar15.QuadPart = 0;
          goto LAB_14004e244;
        }
        if (local_158 == 0x544d4644) {
          local_180.QuadPart = (longlong)PECMD_FormatVolume((uintptr_t)param_1.QuadPart,(WCHAR *)(LVar11.QuadPart + 8));
          LVar33 = local_188;
        }
        else if (local_158 == 0x544e4950) {
          local_180.QuadPart = (longlong)PECMD_PinStartMenuTask((int64_t *)(uintptr_t)param_1.QuadPart,(LPCWSTR)(uintptr_t)(LVar25.QuadPart + 8));
          LVar33 = local_188;
        }
        else if (local_158 == 0x544e4f46) {
          local_180.QuadPart = (longlong)PECMD_FontCommand((uintptr_t)param_1.QuadPart,(uintptr_t)local_res8.QuadPart,4,(uintptr_t)local_140.QuadPart);
          LVar33 = local_188;
        }
        else {
          if (local_158 != 0x54524150) goto LAB_14004df3c;
          local_180.QuadPart = (longlong)PECMD_ExecCommandLine((uintptr_t)param_1.QuadPart,(LPCWSTR)(LVar11.QuadPart + 8));
          LVar33 = local_188;
        }
      }
    }
    else if (local_158 < 0x56524554) {
      if (local_158 == 0x56524553) {
        PECMD_ServiceControl((longlong *)(uintptr_t)param_1.QuadPart,(LPCWSTR)(uintptr_t)local_res8.QuadPart);
        LVar33 = local_188;
        local_180 = extraout_RAX;
      }
      else if (local_158 == 0x54554853) {
        local_180.QuadPart = (longlong)PECMD_ShutPowerAction((uintptr_t)local_res8.QuadPart);
        LVar33 = local_188;
      }
      else if (local_158 == 0x54584546) {
        local_180.QuadPart = (longlong)PECMD_SetFileNameExtension((uintptr_t)param_1.QuadPart,(short *)(LVar11.QuadPart + 8));
        LVar33 = local_188;
      }
      else if (local_158 == 0x54584554) {
        local_180.QuadPart = (longlong)PECMD_ShowScreenText((uintptr_t)param_1.QuadPart,(LPCWSTR)(LVar25.QuadPart + 8));
        LVar33 = local_188;
      }
      else if (local_158 == 0x554e454d) {
        local_180.QuadPart = (longlong)PECMD_CreateMenuItem((uintptr_t)param_1.QuadPart,(short *)(LVar25.QuadPart + 8));
        LVar33 = local_188;
      }
      else if (local_158 == 0x554f5247) {
        if (param_3 != (void *)0) {
          local_180.QuadPart = (longlong)
                        PECMD_ParseWindowPosition((uintptr_t)param_1.QuadPart,(ushort *)(LVar11.QuadPart + 8),(WPARAM)param_3);
          LVar33 = local_188;
        }
      }
      else {
        if (local_158 != 0x56524446) goto LAB_14004df3c;
        local_180.QuadPart = (longlong)PECMD_VolumeDeviceCommand((uintptr_t)param_1.QuadPart,(LPCWSTR)(LVar11.QuadPart + 8));
        LVar33 = local_188;
      }
    }
    else if (local_158 == 0x574f4853) {
      PECMD_PartShowHideDrive((uint64_t)param_1.QuadPart,(uint64_t)(uintptr_t)(LVar11.QuadPart + 8));
      LVar33 = local_188;
      local_180 = extraout_RAX_00;
    }
    else if (local_158 == 0x574f5242) {
      local_180.QuadPart = (longlong)
                    PECMD_ShowBrowseFolder((uintptr_t)param_1.QuadPart,(ushort *)(LVar25.QuadPart + 8),(longlong)param_3,
                                  (void *)0);
      LVar33 = local_188;
    }
    else if (local_158 == 0x58454649) {
      if ((char)local_170 != '\0') {
        FUN_140025f10(param_1.QuadPart,(LPCWSTR)(uintptr_t)local_188.QuadPart,0,(pthreadmbcinfo)&DAT_00000011,param_5,
                      (void *)0);
      }
      plVar10 = local_res20;
      if (*(short *)(LVar25.QuadPart + 8U) == 0x7c) {
        plVar10 = (void *)0;
      }
      local_180.QuadPart = (longlong)
                    PECMD_IfexCmdHandler((uintptr_t)param_1.QuadPart,(uint64_t)(uintptr_t)(LVar25.QuadPart + 8U),plVar10,param_5);
    }
    else if (local_158 == 0x58524f46) {
      if ((char)local_170 != '\0') {
        FUN_140025f10(param_1.QuadPart,(LPCWSTR)(uintptr_t)local_188.QuadPart,0,(pthreadmbcinfo)&DAT_00000011,param_5,
                      (void *)0);
      }
      local_180.QuadPart = (longlong)
                    PECMD_ForCommand((uintptr_t)param_1.QuadPart,(WCHAR *)(LVar25.QuadPart + 8),param_3,local_res20,
                                  param_5);
    }
    else if (local_158 == 0x59434552) {
      local_180.QuadPart = (longlong)PECMD_QueryRecycleBinVolume((uintptr_t)param_1.QuadPart,(short *)(LVar11.QuadPart + 8));
      LVar33 = local_188;
    }
    else {
      if (local_158 != 0x59454b48) goto LAB_14004df3c;
      local_180.QuadPart = (longlong)
                    PECMD_AddControlVariantB((uintptr_t)param_1.QuadPart,(WCHAR *)(LVar25.QuadPart + 8),(longlong)param_3,param_5
                                 );
      LVar33 = local_188;
    }
  }
LAB_14004c525:
  if (local_180.QuadPart != 0) {
    (*(LARGE_INTEGER **)(uintptr_t)(param_1.QuadPart + 0x50))->QuadPart = (LONGLONG)local_180.QuadPart;
  }
LAB_14004c53b:
  if (local_180.QuadPart < 0) {
    wsprintfW(local_c8,(const unsigned short *)L"0x%p");
  }
  else {
    PECMD_FormatI64Dec(local_c8,(uint64_t)local_180.QuadPart);
  }
  if ((*(byte *)(param_1.QuadPart + 0xd) & 0xf) == 0) {
    PECMD_SetVariable((void *)(uintptr_t)param_1.QuadPart,(const uint16_t *)L"ERROR",local_c8);
  }
  PECMD_SetVariable((void *)(uintptr_t)param_1.QuadPart,(const uint16_t *)L"&&ERROR",local_c8);
  DAT_14013ccca = 1;
  if ((0 < g_i64CCB8) && (LVar33.QuadPart != 0)) {
    FUN_140025f10(param_1.QuadPart,(LPCWSTR)(uintptr_t)LVar33.QuadPart,local_180.s.LowPart,(pthreadmbcinfo)0x0,param_5,
                  (void *)0);
  }
  PECMD_FreeStrBuf(&local_160.QuadPart);
  goto LAB_14004e27a;
}
/* ---- PECMD_ProcessScriptBlock 移植新增最小桩 (原未定义符号, 按调用点签名) ---- */
uint64_t PECMD_IsPecmdScriptFile(uint64_t a) { (void)a; return 0; }
uint64_t PECMD_UserCmdHandler(uint64_t a) { (void)a; return 0; }









uint64_t PECMD_MstrStringOps(uint64_t a, uint64_t b) { (void)a;(void)b; return 0; }














void PECMD_ClearTaskTable(undefined8 param_1, int param_2){
    int iVar1;
    if (param_2<0) param_2=0;
    if (*(longlong*)(uintptr_t)param_1!=0) {
        iVar1=*(int*)(uintptr_t)(param_1+8);
        while (param_2<iVar1) {
            *(int*)(uintptr_t)(param_1+8)=*(int*)(uintptr_t)(param_1+8)-1;
            if (*(longlong*)(uintptr_t)(*(longlong*)(uintptr_t)param_1+(longlong)*(int*)(uintptr_t)(param_1+8)*8)!=0) {
                PECMD_ExecInterpString(param_1,*(longlong**)(uintptr_t)(*(longlong*)(uintptr_t)param_1+(longlong)*(int*)(uintptr_t)(param_1+8)*8));
                *(undefined8*)(uintptr_t)(*(longlong*)(uintptr_t)param_1+(longlong)*(int*)(uintptr_t)(param_1+8)*8)=0;
            }
            iVar1=*(int*)(uintptr_t)(param_1+8);
        }
        if (*(int*)(uintptr_t)(param_1+8)<1) {
            if (*(longlong*)(uintptr_t)param_1!=0) HeapFree(DAT_14013d328,0,(void*)(uintptr_t)(*(longlong*)(uintptr_t)param_1-8));
            *(undefined8*)(uintptr_t)param_1=0;
        }
    }
    PECMD_CleanupFrameChain((int64_t)param_1);
}

uint64_t PECMD_ArrayPrepend(void) { return 0; }
void PECMD_SetObjectEnable(int64_t a, unsigned int b) { (void)a; (void)b; }


longlong FUN_140003a20(longlong *param_1, undefined8 *param_2, byte param_3);
void FUN_140053e78(void) { }
/* @0x14005b0b8 size=25 — 带长度前缀缓冲区清零 (decompiled.c @53603 直移):
   槽[-1] 存分配长度 (PECMD_AllocWStringBuffer/PECMD_AllocStrSlot 头), 对数据区整块置 0.
   返回类型保持既有 uint64_t(调用点全部作语句, 语义等价 decompiled void). */
static uint64_t PECMD_ZeroLenBuf(void *a)
{
  ulonglong *param_1 = (ulonglong *)a;
  if (param_1 != (ulonglong *)0x0) {
    FUN_140102a90(param_1,0,param_1[-1]);
  }
  return 0;
}
/* @0x14005b104 size=46 — 串槽释放 (decompiled.c @53632 直移): 槽[0]-8 为
   PECMD_AllocWStringBuffer 分配头([0]=len*2+2, [4]=0xaa55), HeapFree 后槽清 0. */
static void PECMD_FreeStrBuf(void *ps)
{
  longlong *param_1 = (longlong *)ps;
  if (*param_1 != 0) {
    HeapFree(DAT_14013d328,0,(LPVOID)(uintptr_t)(*param_1 + -8));
  }
  *param_1 = 0;
}
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

HANDLE PECMD_CreateDetachedThread(LPTHREAD_START_ROUTINE param_1, LPVOID param_2, size_t param_3, DWORD param_4,
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
DWORD PECMD_RegDeleteValue(HKEY root, LPCWSTR subkey, LPCWSTR name) { (void)root;(void)subkey;(void)name; return 0; }   /* arity 修正 0->3 (PECMD_ExpandSpecialDirs 恢复体带参调用; core_b1_remaining.c extern 同签名) */
longlong PECMD_TokPrefixICmp(char *param_1, ushort *param_2, int param_3){
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
uint64_t PECMD_AsciiWideICmp(const char *a, const uint16_t *b) { (void)a;(void)b; return 0; }


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
/* advapi32 安全描述符 API 桩 (R1 批新增: PECMD_InitNullDaclSD 真体依赖, 库内原无) */
int InitializeSecurityDescriptor(void *pSD, DWORD dwRevision) { (void)pSD;(void)dwRevision; return 1; }
int SetSecurityDescriptorDacl(void *pSD, BOOL bDaclPresent, void *pDacl) { (void)pSD;(void)bDaclPresent;(void)pDacl; return 1; }
/* @0x14005e7dc size=48 — SD 初始化+NULL DACL(人人可访问) (decompiled.c @56627 直移;
   Ghidra "jumptable" 警告为尾调用伪影, 语义即两连调) */
void PECMD_InitNullDaclSD(uint64_t *param_1)
{
  InitializeSecurityDescriptor((PSECURITY_DESCRIPTOR)(uintptr_t)*param_1,1);
  SetSecurityDescriptorDacl((void *)(uintptr_t)*param_1,1,0);
}
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
/* OLE32 API 槽 (R1 批新增, PECMD_LoadOle32Apis 真体依赖; 运行时槽按纪律置 0+判空,
   由 PECMD_GetApiProcCached("xxx","OLE32",槽,&DAT_14013cfe0) 装载; DAT_14013d800/808/820 已有定义见上).
   注: core_globals.c 以别名持有部分同址槽 (g_pCoCreateInstance=14013d830 等), 本文件沿
   DAT_ 直名先例 (同 433-436 行) 自持存储. */
longlong DAT_14013cfe0 = 0;   /* g_hOle32 模块句柄槽 */
longlong DAT_14013d810 = 0;   /* CoInitializeEx */
longlong DAT_14013d818 = 0;   /* CoInitialize */
longlong DAT_14013d828 = 0;   /* CoInitializeSecurity */
longlong DAT_14013d830 = 0;   /* CoCreateInstance */
longlong DAT_14013d838 = 0;   /* CreateStreamOnHGlobal */
longlong DAT_14013d840 = 0;   /* StringFromGUID2 (兼装载完成标志) */
longlong DAT_14013d848 = 0;   /* CoLoadLibrary */
longlong DAT_14013d850 = 0;   /* CoFreeLibrary */
longlong DAT_14013d858 = 0;   /* CoFreeUnusedLibrariesEx */
/* @0x140061c44 size=358 — OLE32 API 惰性装载 (decompiled.c @59529 直移;
   d840(StringFromGUID2) 最后装载故兼作已初始化标志, 返回其值) */
static uint64_t PECMD_LoadOle32Apis(void)
{
  if (DAT_14013d840 == 0) {
    PECMD_GetApiProcCached("CoInitializeEx","OLE32",&DAT_14013d810,&DAT_14013cfe0);
    PECMD_GetApiProcCached("CoInitializeSecurity","OLE32",&DAT_14013d828,&DAT_14013cfe0);
    PECMD_GetApiProcCached("CoInitialize","OLE32",(longlong *)&DAT_14013d818,&DAT_14013cfe0);
    PECMD_GetApiProcCached("CoUninitialize","OLE32",(longlong *)&DAT_14013d820,&DAT_14013cfe0);
    PECMD_GetApiProcCached("CreateStreamOnHGlobal","OLE32",&DAT_14013d838,&DAT_14013cfe0);
    PECMD_GetApiProcCached("OleInitialize","OLE32",(longlong *)&DAT_14013d800,&DAT_14013cfe0);
    PECMD_GetApiProcCached("OleUninitialize","OLE32",(longlong *)&DAT_14013d808,&DAT_14013cfe0);
    PECMD_GetApiProcCached("CoCreateInstance","OLE32",&DAT_14013d830,&DAT_14013cfe0);
    PECMD_GetApiProcCached("CoLoadLibrary","OLE32",&DAT_14013d848,&DAT_14013cfe0);
    PECMD_GetApiProcCached("CoFreeLibrary","OLE32",&DAT_14013d850,&DAT_14013cfe0);
    PECMD_GetApiProcCached("CoFreeUnusedLibrariesEx","OLE32",(longlong *)&DAT_14013d858,&DAT_14013cfe0);
    PECMD_GetApiProcCached("StringFromGUID2","OLE32",&DAT_14013d840,&DAT_14013cfe0);
  }
  return (uint64_t)DAT_14013d840;
}
/* @0x140061ffc size=117 — 全局互斥锁(带安全描述符)(直移) */
uint64_t PECMD_CreateNamedMutex(uint64_t a, int b, uint16_t *c)
{
  SECURITY_ATTRIBUTES local_48; uint8_t sd[0x27+1]; long long res[4];
  PECMD_EnableTokenPrivilege((const unsigned short *)L"SeCreateGlobalPrivilege",2,0x20);
  memset(res,0,0x27); sd[0]=0;
  local_48.bInheritHandle = 0;
  local_48.lpSecurityDescriptor = sd;
  local_48.nLength = 0x18;
  PECMD_InitNullDaclSD((uint64_t *)&local_48.lpSecurityDescriptor);
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


uint64_t PECMD_ExpandDrivePath(const uint16_t *a, uint64_t b, uint16_t *c, longlong *d) { (void)a;(void)b;(void)c;(void)d; return 0; }
/* @0x1400660ac size=— 匹配 token 并前进(直移) */
char PECMD_MatchTokenAdvance(char *param_1,void *param_2p,int param_3)
{
  longlong *param_2 = (longlong *)param_2p;
  if (param_3 < 0) param_3 = lstrlenA(param_1);
  uint16_t *p = (uint16_t *)*param_2;
  char c = (char)PECMD_TokPrefixICmp(param_1,p,param_3);
  if (c != 0) { *param_2 = (longlong)(p + param_3); PECMD_SkipLeadingControlChars((long long *)param_2); }
  return c;
}
/* R1 批前置声明: PECMD_SprintfRetEnd 真体(桩)在本文件后部 10337 */
extern const uint16_t *PECMD_SprintfRetEnd(const uint16_t *a, uint64_t b, const uint16_t *c);
extern void FUN_1400629b8(void *a, const WCHAR *b, const WCHAR *c);       /* def 本文件后部 (变量赋值) */
/* @0x1400668ec size=65 — 追加"名=<格式化值>"变量 (decompiled.c @63244 直移):
   先按 param_4 格式串把 param_2 格式化进栈缓冲, 再 FUN_1400629b8 赋给槽 */
static uint64_t PECMD_AppendFmtValue(void *a, uint64_t b, const void *c, const void *d)
{
  longlong *param_1 = (longlong *)a;
  WCHAR local_88[64];

  PECMD_SprintfRetEnd(local_88,b,(const uint16_t *)d);
  FUN_1400629b8(param_1,(const uint16_t *)c,local_88);
  return 0;
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
int PECMD_ParseNumTryWriteback(long long *param_1, int *param_2)   /* @0x140067d20 数值解析包装 (decompiled.c 直移) */
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
uint64_t PECMD_EncodeStringId(LPCWSTR name, uint64_t *out, char mode) { (void)name;(void)out;(void)mode; return 0; }   /* @0x14006b1e8 原体见 core_b3g.c FUN_14006B1E8 */
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
/* R1 批前置声明 (真体在本文件后部/其它 TU): StrBld 追加族 */
extern void FUN_140063888(void *a, const WCHAR *b, int64_t c);            /* def @本文件后部 14711 (已有 decl@456) */
extern longlong *FUN_1400637dc(longlong *a, LPCSTR b, ulonglong c, ulonglong d); /* def 本文件后部; ANSI 追加/替换 */
/* ---- StrBld 拷贝族真体 (decompiled.c @70455/@70468/@70555 直移) ---- */
/* @0x1400702d4 size=27 — 宽串追加(定长), 返回槽指针 */
static uint16_t *PECMD_StrBldCopyWideN(void *out, const uint16_t *src, int64_t len)
{
  longlong *param_1 = (longlong *)out;
  *param_1 = 0;
  FUN_140063888(param_1,src,len);
  return (uint16_t *)out;
}
/* @0x1400702f0 size=31 — ANSI 串追加(定长, -1 截断语义由 FUN_1400637dc 承担), 返回槽指针 */
static int64_t *PECMD_StrBldCopyAnsi(int64_t *a, char *b, uint64_t c)
{
  *a = 0;
  FUN_1400637dc((longlong *)a,b,c,0xffffffffffffffffULL);
  return a;
}
int64_t *PECMD_AssignString(int64_t *param_1, const uint16_t *param_2) { (void)param_1;(void)param_2; return (int64_t *)0; }
/* @0x1400703e4 size=9 — 宽串整串赋值(len=-1) */
void *PECMD_StrBldCopyWide(void *a, const WCHAR *b)
{
  FUN_140063888((void *)a,b,-1);
  return a;
}
void PECMD_HeapFreeWithHeader(long long *a) { (void)a; }   /* @0x14005b0d4 串槽释放 (leaf stub), 原体见 decompiled.c:53617 */
uint64_t PECMD_ParsePrefixColon(void) { return 0; }
void PECMD_RelaunchLoadFirstUsb(void) { return; }
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
uint64_t PECMD_NotifyMainWindowRefresh(uint64_t a, int b) { (void)a;(void)b; return 0; }   /* arity 修正 0->2 (core 调用方 + PECMD_ProcessScriptBlock 移入) */
int64_t PECMD_QueryFontInfo(int64_t a, int *b, const void *c) { (void)a;(void)b;(void)c; return 0; }
/* ========== PECMD_DispatchExpressionBlock @ 1400b1724  size=1733 ==========
 * signature: undefined8 __fastcall PECMD_DispatchExpressionBlock(LARGE_INTEGER param_1, ulonglong param_2)
 * 忠实移植 decompiled.c @110300; 外部签名保持桩 (uint64_t,uint64_t) (arity 修正 0->2),
 * 体内用局部 param_1/param_2 对齐 Ghidra 变量 (x64 寄存器传参等价: param_1.QuadPart≡a,
 * param_2≡b).
 * 内部调用按 rename_map: FUN_14006156c→PECMD_MatchPattern, FUN_1400a53e4→
 * PECMD_TokenizeExpression, FUN_14001b23c→PECMD_ExtractTableSegment, FUN_14006e030→
 * PECMD_CopyPathToken(桩 arity 0→4 修正), FUN_14004c0bc→PECMD_ProcessScriptBlock;
 * PECMD_InitObfuscatedKeywords/PECMD_CheckFirstStartupFlag/FUN_140063060/PECMD_StrBldCopyWideN/PECMD_FreeStrBuf 保持原名.
 * DAT_14013d080→g_msgLockCount, DAT_14013d058→g_sysinitState, DAT_14011c638 用符号.
 * 取舍: PECMD_CheckFirstStartupFlag(undefined8) 调用传 param_1.QuadPart (Ghidra 将 LARGE_INTEGER
 * 按单寄存器传参); (LPCWSTR)lpStr1 结构体强转指针非法 → (LPCWSTR)(uintptr_t)lpStr1.QuadPart.
 * 注意: 本函数自身不执行, 由 PECMD_ProcessScriptBlock 内部 / core 调用方经
 * uint64_t 签名进入; 返回 0 (decompiled 亦 return 0). */
/* ---- PECMD_DispatchExpressionBlock 恢复体所需前置声明 (定义见 core_*.c / 本文件后部桩) ---- */
extern uint64_t PECMD_MatchPattern(int16_t *pattern, int64_t ctx);            /* @0x14006156c core_b3c.c */
extern LPCWSTR  PECMD_ExtractTableSegment(int64_t, uint64_t *, uint16_t *, int64_t *, char); /* @0x14001b23c core_b1_remaining.c */
extern int64_t  PECMD_TokenizeExpression(LARGE_INTEGER, int64_t, int64_t *, uint32_t, WCHAR *); /* @0x1400a53e4 core_b3_remaining.c */
extern uint64_t PECMD_CopyPathToken(longlong a, longlong *b, longlong *c, longlong d);   /* @0x14006e030 (定义见后部桩) */
extern uint64_t FUN_140063060(uint64_t value);                                /* @0x140063060 core_b3_remaining.c */
extern void PECMD_InitObfuscatedKeywords(void *script, uint64_t seed);   /* @0x14006159c 真体 core_exec2.c */
extern void     PECMD_CheckFirstStartupFlag(undefined8 param_1);                            /* @0x1400251ac 新增桩 (本文件) */
extern int32_t  g_sysinitState;                                               /* DAT_14013d058 core_scriptdep.c */
uint64_t PECMD_DispatchExpressionBlock(uint64_t a, uint64_t b)
{
  ushort uVar1;
  ushort uVar2;
  int iVar3;
  WCHAR *pWVar4;
  undefined8 uVar5;
  LPCWSTR pWVar6;
  LARGE_INTEGER lpStr1;
  WCHAR WVar7;
  WCHAR WVar8;
  longlong lVar9;
  int iVar10;
  WCHAR *pWVar11;
  WCHAR *pWVar12;
  LPCWSTR *ppWVar13;
  WCHAR *local_res10;
  ulonglong *local_res18;
  WCHAR *local_res20;
  LPCWSTR local_58;
  LPCWSTR local_50 [2];
  LARGE_INTEGER param_1;
  ulonglong param_2 = b;
  param_1.QuadPart = (long long)a;
  uVar1 = *(ushort *)(param_1.QuadPart + 0x48);
  iVar10 = 0;
  pWVar11 = (WCHAR *)(param_2 & 0xfffffffffffffffe);
  local_res18 = (ulonglong *)0x0;
  if ((param_2 & 1) == 0) {
    PECMD_InitObfuscatedKeywords((void *)param_1.QuadPart,(uint64_t)(*(ushort *)(param_1.QuadPart + 0x48) | 0x10000));
    *(undefined2 *)(param_1.QuadPart + 200) = 0;
    for (; (((WVar8 = *pWVar11, *(WCHAR *)(param_1.QuadPart + 0x92) == WVar8 ||
             (*(WCHAR *)(param_1.QuadPart + 0x94) == WVar8)) ||
            (*(WCHAR *)(param_1.QuadPart + 0x8a) == WVar8)) ||
           (*(WCHAR *)(param_1.QuadPart + 0x90) == WVar8)); pWVar11 = pWVar11 + 1) {
    }
  }
  local_58 = (LPCWSTR)0x0;
  if (((*(byte *)(param_1.QuadPart + 0x10) & 0x80) == 0) ||
     (ppWVar13 = &local_58, g_msgLockCount == 0)) {
    ppWVar13 = local_50;
  }
  local_res10 = pWVar11;
  if (*pWVar11 != *(WCHAR *)(param_1.QuadPart + 0x88)) {
LAB_1400b17e7:
    do {
      if ((*(byte *)(param_1.QuadPart + 0x11) & 1) != 0) {
        PECMD_ZeroLenBuf(local_res18);
      }
      if ((*(byte *)(param_1.QuadPart + 200) & 0x42) != 0) break;
      WVar8 = *(WCHAR *)(param_1.QuadPart + 0x88);
      local_res10 = pWVar11;
      pWVar4 = (WCHAR *)FUN_140063060((uint64_t)(uintptr_t)pWVar11);
      if (*pWVar4 == WVar8) break;
      if (iVar10 < 1) {
        if (*(WCHAR *)(param_1.QuadPart + 0x96) == *pWVar11) {
          pWVar4 = pWVar11 + 1;
          for (pWVar11 = pWVar4;
              ((WVar7 = *pWVar11, *(WCHAR *)(param_1.QuadPart + 0x92) == WVar7 ||
               (*(WCHAR *)(param_1.QuadPart + 0x94) == WVar7)) ||
              ((*(WCHAR *)(param_1.QuadPart + 0x8a) == WVar7 ||
               (*(WCHAR *)(param_1.QuadPart + 0x90) == WVar7)))); pWVar11 = pWVar11 + 1) {
          }
          local_res10 = pWVar11;
          if (((*pWVar11 != WVar8) &&
              (uVar5 = PECMD_MatchPattern((int16_t *)pWVar11,param_1.QuadPart + 0xaa), (int)uVar5 != 0)) &&
             (uVar5 = PECMD_MatchPattern((int16_t *)pWVar11,param_1.QuadPart + 0x9a), (int)uVar5 != 0)) {
            local_res10 = pWVar4;
            PECMD_TokenizeExpression(param_1,*(undefined8 *)(param_1.QuadPart + 0x40),
                                     (longlong *)&local_res10,0,(WCHAR *)(uintptr_t)&DAT_14011c638);
            for (; ((WVar8 = *local_res10, *(WCHAR *)(param_1.QuadPart + 0x92) == WVar8 ||
                    (*(WCHAR *)(param_1.QuadPart + 0x94) == WVar8)) ||
                   ((*(WCHAR *)(param_1.QuadPart + 0x8a) == WVar8 ||
                    (pWVar11 = local_res10, *(WCHAR *)(param_1.QuadPart + 0x90) == WVar8))));
                local_res10 = local_res10 + 1) {
            }
          }
          goto LAB_1400b17e7;
        }
        if ((*pWVar11 == *(WCHAR *)(param_1.QuadPart + 0xba)) &&
           (pWVar11[1] == *(WCHAR *)(param_1.QuadPart + 0xbc))) {
          for (local_res10 = pWVar11 + 2;
              (((WVar7 = *local_res10, *(WCHAR *)(param_1.QuadPart + 0x92) == WVar7 ||
                (*(WCHAR *)(param_1.QuadPart + 0x94) == WVar7)) ||
               (*(WCHAR *)(param_1.QuadPart + 0x8a) == WVar7)) ||
              (*(WCHAR *)(param_1.QuadPart + 0x90) == WVar7)); local_res10 = local_res10 + 1) {
          }
          WVar7 = *local_res10;
          if (WVar7 != WVar8) {
            do {
              if (((WVar7 == *(WCHAR *)(param_1.QuadPart + 0x8a)) ||
                  (WVar7 == *(WCHAR *)(param_1.QuadPart + 0x90))) ||
                 (*(WCHAR *)(param_1.QuadPart + 0x96) == WVar7)) break;
              local_res10 = local_res10 + 1;
              WVar7 = *local_res10;
            } while (WVar7 != *(WCHAR *)(param_1.QuadPart + 0x88));
          }
          for (pWVar4 = local_res10;
              ((*(WCHAR *)(param_1.QuadPart + 0x92) == *pWVar4 ||
               (*(WCHAR *)(param_1.QuadPart + 0x94) == *pWVar4)) ||
              ((*(WCHAR *)(param_1.QuadPart + 0x8a) == *pWVar4 ||
               (*(WCHAR *)(param_1.QuadPart + 0x90) == *pWVar4)))); pWVar4 = pWVar4 + 1) {
          }
          if ((uVar1 ^ 0x7b) == *pWVar4) {
            PECMD_StrBldCopyWideN((uint16_t **)&local_res20,pWVar11,
                          (longlong)local_res10 - (longlong)pWVar11 >> 1);
            for (lVar9 = (longlong)((int)((longlong)local_res10 - (longlong)pWVar11 >> 1) + -1);
                -1 < lVar9; lVar9 = lVar9 + -1) {
              local_res20[lVar9] = local_res20[lVar9] ^ *(ushort *)(param_1.QuadPart + 0x48);
            }
            for (pWVar11 = pWVar4 + 1;
                (((WVar8 = *pWVar11, *(WCHAR *)(param_1.QuadPart + 0x92) == WVar8 ||
                  (*(WCHAR *)(param_1.QuadPart + 0x94) == WVar8)) ||
                 (*(WCHAR *)(param_1.QuadPart + 0x8a) == WVar8)) ||
                (*(WCHAR *)(param_1.QuadPart + 0x90) == WVar8)); pWVar11 = pWVar11 + 1) {
            }
            local_res10 = pWVar11;
            if (((*pWVar11 != *(WCHAR *)(param_1.QuadPart + 0x88)) &&
                (pWVar12 = local_res20, uVar5 = PECMD_MatchPattern((int16_t *)pWVar11,param_1.QuadPart + 0xaa),
                (int)uVar5 != 0)) &&
               (uVar5 = PECMD_MatchPattern((int16_t *)pWVar11,param_1.QuadPart + 0x9a), (int)uVar5 != 0)) {
              local_res10 = pWVar4 + 1;
              PECMD_TokenizeExpression(param_1,*(undefined8 *)(param_1.QuadPart + 0x40),
                                       (longlong *)&local_res10,0,pWVar12);
              pWVar11 = local_res10;
              PECMD_FreeStrBuf((WCHAR **)&local_res20);
              goto LAB_1400b17e7;
            }
            PECMD_FreeStrBuf((WCHAR **)&local_res20);
          }
          WVar8 = *local_res10;
          if (((WVar8 == *(WCHAR *)(param_1.QuadPart + 0x92)) ||
              (WVar8 == *(WCHAR *)(param_1.QuadPart + 0x94))) ||
             ((WVar8 == *(WCHAR *)(param_1.QuadPart + 0x8a) ||
              (WVar8 == *(WCHAR *)(param_1.QuadPart + 0x90))))) {
            while (((*(WCHAR *)(param_1.QuadPart + 0x88) != WVar8 &&
                    (*(WCHAR *)(param_1.QuadPart + 0x8a) != WVar8)) &&
                   (*(WCHAR *)(param_1.QuadPart + 0x90) != WVar8))) {
              local_res10 = local_res10 + 1;
              WVar8 = *local_res10;
            }
          }
          for (; ((WVar8 = *local_res10, *(WCHAR *)(param_1.QuadPart + 0x92) == WVar8 ||
                  (*(WCHAR *)(param_1.QuadPart + 0x94) == WVar8)) ||
                 ((*(WCHAR *)(param_1.QuadPart + 0x8a) == WVar8 ||
                  (pWVar11 = local_res10, *(WCHAR *)(param_1.QuadPart + 0x90) == WVar8))));
              local_res10 = local_res10 + 1) {
          }
          goto LAB_1400b17e7;
        }
      }
      local_res10 = (WCHAR *)0x0;
      local_50[0] = pWVar11;
      pWVar6 = PECMD_ExtractTableSegment(param_1.QuadPart,(uint64_t *)&local_res18,(ushort *)pWVar11,
                                         (longlong *)&local_res10,'\0');
      if (((*pWVar6 != L'\0') && (*pWVar6 != L'/')) && ((*pWVar6 != L';' && (*pWVar6 != L'`')))) {
        lpStr1.QuadPart =
             PECMD_CopyPathToken(param_1.QuadPart,(longlong *)&local_res10,(longlong *)&local_res18,
                           (longlong)pWVar6);
        iVar3 = StrCmpNIW((LPCWSTR)(uintptr_t)lpStr1.QuadPart,(const uint16_t *)L"_SUB",4);
        if ((iVar3 == 0) &&
           ((((uVar2 = *(ushort *)(lpStr1.QuadPart + 8), 8 < uVar2 && (uVar2 < 0xe)) ||
             (uVar2 == 0x20)) || (uVar2 == 0)))) {
          iVar10 = iVar10 + 1;
        }
        else {
          iVar3 = StrCmpNIW((LPCWSTR)(uintptr_t)lpStr1.QuadPart,(const uint16_t *)L"_END",4);
          if ((iVar3 == 0) &&
             (((uVar2 = *(ushort *)(lpStr1.QuadPart + 8), 8 < uVar2 && (uVar2 < 0xe)) ||
              ((uVar2 == 0x20 || (uVar2 == 0)))))) {
            if (0 < iVar10) {
              iVar10 = iVar10 + -1;
            }
          }
          else if (iVar10 < 1) {
            if ((((g_sysinitState == '\x03') &&
                 ((iVar3 = StrCmpNIW((const uint16_t *)L"REGI",(LPCWSTR)(uintptr_t)lpStr1.QuadPart,4), iVar3 != 0 ||
                  (((uVar2 = *(ushort *)(lpStr1.QuadPart + 8), uVar2 < 9 || (0xd < uVar2)) &&
                   ((uVar2 != 0x20 && (uVar2 != 0)))))))) &&
                ((iVar3 = StrCmpNIW((const uint16_t *)L"ENVI",(LPCWSTR)(uintptr_t)lpStr1.QuadPart,4), iVar3 != 0 ||
                 ((((uVar2 = *(ushort *)(lpStr1.QuadPart + 8), uVar2 < 9 || (0xd < uVar2)) &&
                   (uVar2 != 0x20)) && (uVar2 != 0)))))) &&
               ((iVar3 = StrCmpNIW((const uint16_t *)L"SET",(LPCWSTR)(uintptr_t)lpStr1.QuadPart,3), iVar3 != 0 ||
                (((uVar2 = *(ushort *)(lpStr1.QuadPart + 6), uVar2 < 9 || (0xd < uVar2)) &&
                 ((uVar2 != 0x20 && (uVar2 != 0)))))))) {
              PECMD_CheckFirstStartupFlag(param_1.QuadPart);
            }
            PECMD_ProcessScriptBlock(param_1,lpStr1,(longlong *)0x0,(longlong *)&local_res10,
                                     (pthreadmbcinfo)*ppWVar13);
          }
        }
      }
      for (; (*local_res10 == *(WCHAR *)(param_1.QuadPart + 0x8a) ||
             (*local_res10 == *(WCHAR *)(param_1.QuadPart + 0x90))); local_res10 = local_res10 + 1)
      {
      }
      pWVar11 = local_res10;
    } while (*local_res10 != *(WCHAR *)(param_1.QuadPart + 0x88));
  }
  if ((param_2 & 1) == 0) {
    *(undefined2 *)(param_1.QuadPart + 200) = 0;
  }
  PECMD_FreeStrBuf((WCHAR **)&local_res18);
  return 0;
}
/* @0x14006159c size=357 — 脚本上下文命令入口标记 (XOR 键/结束标记) 新增桩:
   原体未移植 (decompiled.c @59192); 签名 void PECMD_InitObfuscatedKeywords(longlong, ulonglong). */
/* @0x1400251ac size=198 — 重置/清理 (decompiled.c @22328) 新增桩:
   签名 void PECMD_CheckFirstStartupFlag(undefined8 param_1); 调用约定经 QuadPart 传值. */

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
/* ========== PECMD_DdCopyCommand @ 1400d2e90  size=11447 恢复体 (decompiled.c @129675) ==========
 * 磁盘备份/镜像命令 (dd 式: -bs/-ibs/-obs/-count/-len/-conv/-skip/-skipb/-sparse/-quick/
 *   -qspace/-va/-free/-fd/-file/-dir/-dd/-v, 可选 0x/0x hex 语法与 EXEDATA 编码插件).
 * 签名保持原桩: uint64_t PECMD_DdCopyCommand(uint64_t a, uint64_t b)
 *   (a=param_1 脚本对象 longlong*, b=param_2 LARGE_INTEGER 命令串指针; 返回按 QuadPart 位值,
 *   调用方行 2908/8271 以 QuadPart 承接).
 * 移植取舍(歧义自决):
 *  - LARGE_INTEGER 全按本文件 union 型; Ghidra 标量式 .s 运算/赋值归一为 .QuadPart
 *    (union 保证 LowPart/HighPart/QuadPart 同位); .s.LowPart._0_1_ 字节位写展开为掩码位写.
 *  - CONCAT31(._1_3_,v) 展开为 (x & 0xffffff00u)|v; CONCAT44 局部宏 (函数后 #undef);
 *    CONCAT71(extraout_var*,bVarX) 为 Ghidra 寄存器拼接残留, 按文件既定模式(core_b3r_g3.c)
 *    归一为 bVarX. L'￐'(U+FFD0) 写为 0xffd0.
 *  - FUN_140003864→PECMD_OpenFileHandle (rename_map; core_exec2.c 真实体, HANDLE 返回,
 *    结果以 (long long)(uintptr_t) 承接); FUN_1400702b0→PECMD_StrDupAssign (本文件已定义).
 *  - DAT_14013cf70 复用本文件既有全局 (void* HINSTANCE 槽, 行~12531);
 *    DAT_14013d380 为运行时装载槽 (映像初值=映像内地址, 无跨会话常量意义) → 置 0,
 *    SetFileValidData 未装载 → 调用点跳过 (与真实行为一致).
 *  - FUN_140063344/PECMD_AsciiWideICmp/PECMD_DeviceCheckReady/WriteFile/FlushFileBuffers/
 *    FreeLibrary/LoadLibraryExW 无参桩配带参调用 → 按惯例修 arity (本文件已有先例).
 */
/* ---------- PECMD_DdCopyCommand 新增桩 (decompiled.c 调用面; rename_map 目标已在 core_* 真实定义,
 *   仅余 6 个无既有定义者在此补 leaf 桩) ---------- */
  /* 参数表初始化 (leaf stub) */
static HANDLE PECMD_HandleDuplicateValid(HANDLE h, LARGE_INTEGER *out) { (void)h;(void)out; return (HANDLE)0; }  /* 目标句柄克隆 (leaf stub) */
  /* 资源名定位 (leaf stub) */
  /* 设备/文件类型探测 (leaf stub) */
static longlong PECMD_GetPartitionLayoutEntry(uintptr_t h, int mode, ulonglong *out) { (void)h;(void)mode;(void)out; return 0; }  /* 段/盘容量查询 (leaf stub) */
static char PECMD_NormalizeDiskDevicePath(longlong *p) { (void)p; return '\0'; }  /* 行尾整理 (leaf stub) */
void (*DAT_14013d380)(void *h, long long len) = 0;  /* SetFileValidData 装载槽 (未装载→跳过) */
/* ---------- PECMD_DdCopyCommand 调用面前置声明 (定义在本文件后部/core 文件, 避免隐式 int 截断) ---------- */
extern HANDLE PECMD_OpenFileHandle(HANDLE *out, LPCWSTR path, DWORD access, DWORD share,
                                   LPSECURITY_ATTRIBUTES sa, DWORD disp, DWORD flags, HANDLE tmpl);
int      PECMD_ParseHexOrDec(long long *a, uint64_t *b);
uint64_t PECMD_MatchAssignToken(char *key, int64_t *pp, int len);          /* def core_b3f.c */
WCHAR   *PECMD_ResolveVariable(int64_t *param_1, LPCWSTR param_2, uint64_t *param_3, uint32_t param_4);  /* def core_b3_remaining.c */
void     PECMD_TrimWhitespace(uint16_t *s);                                /* def core_b1_remaining.c */
uint32_t PECMD_FindFileOrDir(LPWSTR param_1, uint32_t param_2);            /* def core_b9_remaining.c */
LARGE_INTEGER PECMD_SetFilePointer(HANDLE h, LARGE_INTEGER dist, DWORD method);  /* def core_remaining_helpers.c */
int64_t  PECMD_GetDeviceSize(HANDLE h, uint8_t mtype);                     /* def core_exec4.c */
uint64_t PECMD_ScanHexNumber(uint16_t **ps, uint64_t *acc, int *max);      /* def core_b3_remaining.c */
int64_t  PECMD_ReadModifyWrite(HANDLE param_1, uint8_t *param_2, int64_t param_3, int param_4, uint32_t param_5);  /* def core_b3r_c.c */
bool     PECMD_WriteFileChunked(HANDLE hFile, const void *data, uint32_t len, int *written, uint32_t minChunk);   /* def core_b3_remaining.c */
void     FUN_14006355c(void *a, const WCHAR *b, int c, uint64_t d);
void    *FUN_14005b374(void *a, int16_t b, int16_t c);
int      FUN_140067cf4(long long *, uint64_t *);
WCHAR   *PECMD_TokenizeQuotedField(longlong *param_1, longlong *param_2, longlong *param_3, WCHAR param_4, ushort param_5);
uint64_t PECMD_DeviceCheckReady(LPCWSTR p);
uint64_t FUN_140063344(void *p);
int      PECMD_QueryDeviceIoInfo(uintptr_t h, int size);
void     FUN_140102a90(uint64_t *dst, uint64_t v, uint64_t n);
uint64_t ReadFile(void *h, void *buf, unsigned long n, unsigned long *read, void *ol);
uint64_t WriteFile(void *h, void *buf, unsigned long n, unsigned long *written, void *ol);
uint64_t FlushFileBuffers(void *h);
uint64_t FreeLibrary(void *h);
uint64_t LoadLibraryExW(LPCWSTR path, void *file, uint32_t flags);
DWORD    GetLastError(void);
int      SetEndOfFile(void *h);
uint64_t SetFilePointerEx(void *h, longlong off, longlong *out, unsigned long method);
int32_t  lstrlenA(const char *s);
extern void *DAT_14013cf70;                  /* HINSTANCE 槽 (def 行~12531) */
#define CONCAT44(hi, lo) (((uint64_t)(uint32_t)(hi) << 32) | (uint32_t)(lo))
uint64_t PECMD_DdCopyCommand(uint64_t a, uint64_t b)
{
  longlong *param_1 = (longlong *)(uintptr_t)a;
  LARGE_INTEGER param_2; param_2.QuadPart = (long long)b;
{
  ushort uVar1;
  WCHAR WVar2;
  bool bVar3;
  undefined1 uVar4;
  byte bVar5;
  char cVar6;
  bool bVar7;
  bool bVar8;
  int extraout_EAX;
  DWORD DVar9;
  uint uVar10;
  BOOL BVar11;
  HANDLE pvVar12;
  WCHAR *pWVar13;
  undefined7 extraout_var;
  LPCWSTR pWVar14;
  longlong lVar15;
  LPCWSTR pWVar16;
  LARGE_INTEGER LVar17;
  LARGE_INTEGER LVar18;
  ulonglong uVar19;
  undefined7 extraout_var_00;
  undefined7 extraout_var_01;
  undefined7 extraout_var_02;
  undefined1 uVar20;
  LARGE_INTEGER *pLVar21;
  uint uVar22;
  LARGE_INTEGER LVar23;
  LARGE_INTEGER LVar24;
  LARGE_INTEGER LVar25;
  undefined4 uVar26;
  LARGE_INTEGER LVar27;
  LARGE_INTEGER LVar28;
  LARGE_INTEGER LVar29;
  undefined8 uVar30;
  WCHAR WVar31;
  int iVar32;
  ulonglong uVar33;
  LARGE_INTEGER LVar34;
  ushort uVar35;
  LARGE_INTEGER LVar36;
  LARGE_INTEGER LVar37;
  uint uVar38;
  LARGE_INTEGER local_res10;
  LARGE_INTEGER local_res20;
  char local_178;
  byte local_176;
  LARGE_INTEGER local_170;
  char local_168;
  LPCWSTR local_160;
  byte local_158;
  uint local_154;
  DWORD local_150;
  uint local_14c;
  char local_148;
  byte local_147;
  LARGE_INTEGER local_140;
  DWORD local_138;
  uint local_134;
  LARGE_INTEGER local_130;
  uint local_128;
  byte local_124;
  LARGE_INTEGER local_120;
  LARGE_INTEGER local_118;
  LARGE_INTEGER local_110;
  LARGE_INTEGER local_108;
  LARGE_INTEGER local_100;
  uint local_f8;
  LARGE_INTEGER local_f0;
  LARGE_INTEGER local_e8;
  LARGE_INTEGER local_e0;
  LARGE_INTEGER local_d8;
  LARGE_INTEGER local_d0;
  LARGE_INTEGER local_c8;
  LARGE_INTEGER local_c0;
  LARGE_INTEGER local_b8;
  uint local_b0;
  int local_ac;
  LARGE_INTEGER local_a8;
  LARGE_INTEGER local_a0;
  longlong local_98;
  LARGE_INTEGER local_90;
  undefined8 local_88;
  LPCWSTR local_80;
  LARGE_INTEGER local_78;
  LPCWSTR local_70;
  uint local_68 [2];
  longlong local_60;
  LARGE_INTEGER local_58;
  longlong local_50 [3];
  
  LVar37.QuadPart = 0;
  LVar23.QuadPart = 0;
  local_50[0] = 0;
  local_res10 = param_2;
  PECMD_AllocFileReqEmbedPath(local_50,&local_res10);
  PECMD_EnableTokenPrivilege((const uint16_t *)L"SeBackupPrivilege",2,1);
  PECMD_EnableTokenPrivilege((const uint16_t *)L"SeRestorePrivilege",2,0x10);
  bVar8 = false;
  LVar25.s.LowPart = 0;
  local_176 = 0;
  local_124 = 0;
  local_147 = 0;
  local_res20.QuadPart = local_res20.QuadPart & 0xffffffffffffff00;
  local_b0 = local_b0 & 0xffffff00;
  bVar3 = false;
  local_168 = '\0';
  local_150 = 0;
  local_120.QuadPart = 0;
  local_108.QuadPart = 0;
  for (; (uVar35 = *(ushort *)(uintptr_t)local_res10.QuadPart, uVar35 != 0 &&
         (((uVar35 < 9 || (0xd < uVar35)) && (uVar35 != 0x20))));
      local_res10.QuadPart = local_res10.QuadPart + 2) {
    if (uVar35 == 0x2a) {
      LVar25.s.LowPart = (uint)*(byte *)(uintptr_t)local_res10.QuadPart;
    }
    else if (uVar35 == 0x2e) {
      local_176 = *(byte *)(uintptr_t)local_res10.QuadPart;
    }
    else if (uVar35 == 0x23) {
      local_124 = *(byte *)(uintptr_t)local_res10.QuadPart;
    }
  }
  PECMD_SkipLeadingControlChars(&local_res10.QuadPart);
  LVar24 = local_res10;
  local_d8.QuadPart = local_res10.QuadPart;
  local_c0.QuadPart = local_res10.QuadPart;
  PECMD_AllocStrSlot((uint16_t **)&local_130);
  uVar30 = 0x2d;
  if (*(ushort *)(uintptr_t)LVar24.QuadPart == 0x2d) {
    lVar15 = 2;
    uVar35 = 0x2d;
    do {
      uVar1 = *(ushort *)(LVar24.QuadPart + 2);
      if ((((uVar1 == 0) || ((8 < uVar1 && (uVar1 < 0xe)))) || (uVar1 == 0x20)) || (uVar1 == 0x2c))
      break;
      do {
        if (((8 < uVar35) && (uVar35 < 0xe)) || (uVar35 == 0x20)) break;
        LVar24.QuadPart = LVar24.QuadPart + lVar15;
        uVar35 = *(ushort *)(uintptr_t)LVar24.QuadPart;
        local_c0.QuadPart = LVar24.QuadPart;
      } while (uVar35 != 0);
      PECMD_SkipLeadingControlChars(&local_c0.QuadPart);
      uVar35 = *(ushort *)(uintptr_t)local_c0.QuadPart;
      LVar24 = local_c0;
    } while ((ushort)uVar30 == uVar35);
    local_d8 = LVar24;
    if (LVar24.QuadPart != local_res10.QuadPart) {
      *(ushort *)(LVar24.QuadPart + -2) = 0;
      PECMD_ExpandVarDispatch(param_1,(WCHAR *)(uintptr_t)local_res10.QuadPart,&local_130,0,1);
    }
  }
  uVar10 = 0;
  local_res10 = local_130;
  PECMD_AllocWStringBuffer((WCHAR **)&local_160,1);
  local_e8.QuadPart = -1;
  local_e0.QuadPart = 0;
  local_170.QuadPart = 0;
  local_f0.QuadPart = 0x7fffffffffffffff;
  local_c8.QuadPart = 0;
  local_80 = (LPCWSTR)0x0;
  local_148 = '\0';
  local_c0.QuadPart = 0x200;
  local_a0.QuadPart = 0x200;
  local_a8.QuadPart = 0x200;
  local_140.s.LowPart = local_140.s.LowPart & 0xffffff00;
  local_154 = local_154 & 0xffffff00;
  iVar32 = 0;
  local_70 = (LPCWSTR)0x0;
  LVar29.QuadPart = LVar37.QuadPart;
  while (uVar22 = 0, *(WCHAR *)(uintptr_t)local_res10.QuadPart == L'-') {
    cVar6 = PECMD_MatchTokenAdvance("-16",&local_res10.QuadPart,3);
    if (cVar6 == '\0') {
      cVar6 = PECMD_MatchTokenAdvance("-dir",&local_res10.QuadPart,4);
      if (cVar6 == '\0') {
        uVar33 = PECMD_MatchAssignToken("-free",(int64_t *)&local_res10.QuadPart,5);
        if ((char)uVar33 != '\0') {
          local_b8.QuadPart = local_res10.QuadPart;
          WVar2 = *(WCHAR *)(uintptr_t)local_res10.QuadPart;
          while ((WVar2 != L'\0' &&
                 (((LVar29 = local_108, (ushort)WVar2 < 9 || (0xd < (ushort)WVar2)) &&
                  (WVar2 != L' '))))) {
            local_res10.QuadPart = local_res10.QuadPart + 2;
            WVar2 = *(WCHAR *)(uintptr_t)local_res10.QuadPart;
          }
          PECMD_SkipLeadingControlChars(&local_res10.QuadPart);
          local_res20.QuadPart = 0;
          PECMD_ParseHexOrDec(&local_b8.QuadPart,(ulonglong *)&local_res20.QuadPart);
          if (((HANDLE)local_res20.QuadPart != (HANDLE)0xffffffffffffffff) &&
             ((HANDLE)local_res20.QuadPart != (HANDLE)0x0)) {
            CloseHandle((HANDLE)(uintptr_t)local_res20.QuadPart);
          }
          PECMD_FreeStrBuf((WCHAR **)&local_160);
          PECMD_FreeStrBuf(&local_130.QuadPart);
          if (((WCHAR *)LVar29.QuadPart != (WCHAR *)0x0) &&
             ((WCHAR *)LVar29.QuadPart != (WCHAR *)0xffffffffffffffff)) {
            CloseHandle((HANDLE)(uintptr_t)LVar29.QuadPart);
          }
          goto LAB_1400d3d7f;
        }
        uVar33 = PECMD_MatchAssignToken("-fd",(int64_t *)&local_res10.QuadPart,3);
        if ((char)uVar33 == '\0') {
          cVar6 = PECMD_MatchTokenAdvance("-va",&local_res10.QuadPart,3);
          if (cVar6 == '\0') {
            cVar6 = PECMD_MatchTokenAdvance("-v",&local_res10.QuadPart,2);
            if (cVar6 == '\0') {
              cVar6 = PECMD_MatchTokenAdvance("-sparse",&local_res10.QuadPart,7);
              if (cVar6 == '\0') {
                cVar6 = PECMD_MatchTokenAdvance("-quick",&local_res10.QuadPart,6);
                if (cVar6 == '\0') {
                  cVar6 = PECMD_MatchTokenAdvance("-dd",&local_res10.QuadPart,3);
                  if (cVar6 != '\0') {
                    local_154 = (local_154 & 0xffffff00u) | 1u;
                    LVar25.s.LowPart = 1;
                    local_158 = 1;
                    LVar23.QuadPart = LVar37.QuadPart;
                    LVar18.QuadPart = LVar37.QuadPart;
                    uVar26 = local_140.s.LowPart;
                    uVar38 = local_b0;
                    if (*(WCHAR *)(uintptr_t)local_res10.QuadPart == L'-') goto LAB_1400d3733;
                    break;
                  }
                  cVar6 = PECMD_MatchTokenAdvance("-file",&local_res10.QuadPart,5);
                  if (cVar6 == '\0') {
                    WVar2 = *(WCHAR *)(uintptr_t)local_res10.QuadPart;
                    while ((WVar2 != L'\0' &&
                           (((LVar24 = local_d8, (ushort)WVar2 < 9 || (0xd < (ushort)WVar2)) &&
                            (WVar2 != L' '))))) {
                      local_res10.QuadPart = local_res10.QuadPart + 2;
                      WVar2 = *(WCHAR *)(uintptr_t)local_res10.QuadPart;
                    }
                    PECMD_SkipLeadingControlChars(&local_res10.QuadPart);
                  }
                  else {
                    local_168 = '\x01';
                  }
                }
                else {
                  iVar32 = 1;
                }
              }
              else {
                bVar3 = true;
              }
            }
            else {
              uVar10 = 1;
              local_res20.s.LowPart = (local_res20.s.LowPart & 0xffffff00u) | 1u;
            }
          }
          else {
            local_b0 = (local_b0 & 0xffffff00u) | 1u;
          }
        }
        else {
          WVar31 = L'\t';
          WVar2 = *(WCHAR *)(uintptr_t)local_res10.QuadPart;
          local_d0.QuadPart = local_res10.QuadPart;
          LVar18 = local_res10;
          while (((WVar2 != L'\0' &&
                  ((LVar24 = local_d8, (ushort)WVar2 < 9 || (0xd < (ushort)WVar2)))) &&
                 (WVar2 != L' '))) {
            LVar18.QuadPart = LVar18.QuadPart + 2;
            WVar2 = *(WCHAR *)(uintptr_t)LVar18.QuadPart;
          }
          LVar27 = local_res10;
          local_res10 = LVar18;
          PECMD_SkipLeadingControlChars(&local_res10.QuadPart);
          local_100.QuadPart = 0;
          local_118.QuadPart = 0;
          if (*(WCHAR *)(uintptr_t)LVar27.QuadPart == L'?') {
            local_70 = (LPCWSTR)(LVar27.QuadPart + 2);
            WVar2 = *local_70;
            pWVar13 = local_70;
            while (((WVar2 != L'\0' &&
                    ((LVar29 = local_108, (ushort)WVar2 < (ushort)WVar31 || (0xd < (ushort)WVar2))))
                   && (WVar2 != L' '))) {
              pWVar13 = pWVar13 + 1;
              WVar2 = *pWVar13;
            }
            if (*pWVar13 != L'\0') {
              *pWVar13 = L'\0';
            }
            local_d0.QuadPart = (longlong)PECMD_ResolveVariable(param_1,local_70,(uint64_t *)&local_118,0);
          }
          PECMD_ParseHexOrDec(&local_d0.QuadPart,(ulonglong *)&local_100.QuadPart);
          LVar18 = local_100;
          if (((WCHAR *)local_100.QuadPart != (WCHAR *)0xffffffffffffffff) &&
             ((WCHAR *)local_100.QuadPart != (WCHAR *)0x0)) {
            local_70 = (LPCWSTR)0x0;
            pvVar12 = PECMD_HandleDuplicateValid((HANDLE)(uintptr_t)local_100.QuadPart,&local_108);
            LVar29 = local_108;
            if ((pvVar12 != (HANDLE)0x0) &&
               (local_120 = LVar18, (WCHAR *)local_108.QuadPart != (WCHAR *)0x0)) {
              local_120 = local_108;
            }
          }
          PECMD_FreeStrBuf(&local_118.QuadPart);
        }
      }
      else {
        local_150 = 0x80000000;
      }
    }
    else {
      local_147 = 1;
    }
  }
  goto LAB_1400d3475;
  while( true ) {
    WVar2 = *(WCHAR *)(uintptr_t)local_res10.QuadPart;
    local_100 = local_res10;
    while (((WVar2 != L'\0' && (((ushort)WVar2 < 9 || (0xd < (ushort)WVar2)))) && (WVar2 != L' ')))
    {
      local_100.QuadPart = local_100.QuadPart + 2;
      WVar2 = *(WCHAR *)(uintptr_t)local_100.QuadPart;
    }
    LVar24 = local_res10;
    PECMD_SkipLeadingControlChars(&local_100.QuadPart);
    uVar30 = PECMD_AsciiPrefixICmp("-bs=",(ushort *)(uintptr_t)LVar24.QuadPart,4);
    if ((char)uVar30 == '\0') {
      uVar30 = PECMD_AsciiPrefixICmp("-ibs=",(ushort *)(uintptr_t)LVar24.QuadPart,5);
      if ((char)uVar30 == '\0') {
        uVar30 = PECMD_AsciiPrefixICmp("-obs=",(ushort *)(uintptr_t)LVar24.QuadPart,5);
        if ((char)uVar30 == '\0') {
          uVar30 = PECMD_AsciiPrefixICmp("-count=",(ushort *)(uintptr_t)LVar24.QuadPart,7);
          if ((char)uVar30 == '\0') {
            uVar30 = PECMD_AsciiPrefixICmp("-len=",(ushort *)(uintptr_t)LVar24.QuadPart,5);
            if ((char)uVar30 == '\0') {
              uVar30 = PECMD_AsciiPrefixICmp("-conv=",(ushort *)(uintptr_t)LVar24.QuadPart,6);
              if ((char)uVar30 == '\0') {
                uVar30 = PECMD_AsciiPrefixICmp("-skip=",(ushort *)(uintptr_t)LVar24.QuadPart,6);
                if ((char)uVar30 == '\0') {
                  uVar30 = PECMD_AsciiPrefixICmp("-skipb=",(ushort *)(uintptr_t)LVar24.QuadPart,7);
                  if ((char)uVar30 == '\0') {
                    cVar6 = PECMD_MatchTokenAdvance("-sparse",&local_res10.QuadPart,7);
                    if (cVar6 == '\0') {
                      cVar6 = PECMD_MatchTokenAdvance("-quick",&local_res10.QuadPart,6);
                      if (cVar6 == '\0') {
                        cVar6 = PECMD_MatchTokenAdvance("-qspace",&local_res10.QuadPart,7);
                        if (cVar6 == '\0') {
                          cVar6 = PECMD_MatchTokenAdvance("-va",&local_res10.QuadPart,3);
                          uVar38 = uVar38 & 0xff;
                          if (cVar6 != '\0') {
                            uVar38 = 1;
                          }
                        }
                        else {
                          iVar32 = 0x10;
                        }
                      }
                      else {
                        iVar32 = 1;
                      }
                    }
                    else {
                      bVar3 = true;
                    }
                  }
                  else {
                    local_res10.QuadPart = LVar24.QuadPart + 0xe;
                    local_170.QuadPart = 0;
                    uVar33 = PECMD_EvalParenStripped((longlong *)&local_res10.QuadPart,(ulonglong *)&local_170.QuadPart);
                    LVar18.QuadPart = LVar37.QuadPart;
                    if (0 < (int)uVar33) {
                      LVar23 = local_170;
                      LVar18.QuadPart = 0;
                    }
                  }
                }
                else {
                  local_res10.QuadPart = LVar24.QuadPart + 0xc;
                  local_170.QuadPart = 0;
                  uVar33 = PECMD_EvalParenStripped((longlong *)&local_res10.QuadPart,(ulonglong *)&local_170.QuadPart);
                  LVar23.QuadPart = -1;
                  if (0 < (int)uVar33) {
                    LVar18 = local_170;
                  }
                }
              }
              else {
                LVar24.QuadPart = LVar24.QuadPart + 0xc;
                local_res10.QuadPart = LVar24.QuadPart;
                uVar30 = PECMD_AsciiPrefixICmp("notrunc",(ushort *)(uintptr_t)LVar24.QuadPart,7);
                uVar26 = uVar26 & 0xff;
                if ((char)uVar30 != '\0') {
                  uVar26 = 1;
                }
                WVar2 = *(WCHAR *)(uintptr_t)LVar24.QuadPart;
                while (((WVar2 != L'\0' && (((ushort)WVar2 < 9 || (0xd < (ushort)WVar2)))) &&
                       (WVar2 != L' '))) {
                  LVar24.QuadPart = LVar24.QuadPart + 2;
                  WVar2 = *(WCHAR *)(uintptr_t)LVar24.QuadPart;
                }
              }
            }
            else {
              local_res10.QuadPart = LVar24.QuadPart + 10;
              local_170.QuadPart = 0;
              uVar33 = PECMD_EvalParenStripped((longlong *)&local_res10.QuadPart,(ulonglong *)&local_170.QuadPart);
              if (0 < (int)uVar33) {
                local_e8.QuadPart = local_170.QuadPart;
              }
            }
          }
          else {
            local_res10.QuadPart = LVar24.QuadPart + 0xe;
            local_170.QuadPart = 0;
            uVar33 = PECMD_EvalParenStripped((longlong *)&local_res10.QuadPart,(ulonglong *)&local_170.QuadPart);
            if (0 < (int)uVar33) {
              local_f0.QuadPart = local_170.QuadPart;
            }
          }
        }
        else {
          local_res10.QuadPart = LVar24.QuadPart + 10;
          local_170.QuadPart = 0;
          uVar33 = PECMD_EvalParenStripped((longlong *)&local_res10.QuadPart,(ulonglong *)&local_170.QuadPart);
          if (0 < (int)uVar33) {
            local_a8.QuadPart = (longlong)(int)local_170.s.LowPart;
          }
        }
      }
      else {
        local_res10.QuadPart = LVar24.QuadPart + 10;
        local_170.QuadPart = 0;
        uVar33 = PECMD_EvalParenStripped((longlong *)&local_res10.QuadPart,(ulonglong *)&local_170.QuadPart);
        if (0 < (int)uVar33) {
          local_a0.QuadPart = (longlong)(int)local_170.s.LowPart;
        }
      }
    }
    else {
      local_res10.QuadPart = LVar24.QuadPart + 8;
      local_170.QuadPart = 0;
      uVar33 = PECMD_EvalParenStripped((longlong *)&local_res10.QuadPart,(ulonglong *)&local_170.QuadPart);
      if (0 < (int)uVar33) {
        local_a8.QuadPart = (longlong)(int)local_170.s.LowPart;
        local_a0 = local_a8;
      }
    }
    local_res10 = local_100;
    if (*(WCHAR *)(uintptr_t)local_100.QuadPart != L'-') break;
LAB_1400d3733:
    WVar2 = *(WCHAR *)(local_res10.QuadPart + 2);
    if ((WVar2 == L'\0') ||
       ((((8 < (ushort)WVar2 && ((ushort)WVar2 < 0xe)) || (WVar2 == L' ')) || (WVar2 == L','))))
    break;
  }
  LVar25.s.LowPart = (uint)local_158;
  uVar10 = (uint)(byte)(local_res20.s.LowPart & 0xff);
  LVar24 = local_d8;
  LVar29 = local_108;
  local_140.s.LowPart = uVar26;
  local_c8 = LVar23;
  local_b0 = uVar38;
  if ((LVar23.QuadPart < 0) &&
     (local_c8.QuadPart = local_a0.QuadPart * LVar18.QuadPart,
     local_a0.QuadPart * LVar18.QuadPart < 0)) {
    local_c8.QuadPart = LVar37.QuadPart;
  }
LAB_1400d3475:
  cVar6 = PECMD_MatchTokenAdvance("-quick",&local_res10.QuadPart,6);
  if (cVar6 != '\0') {
    iVar32 = 1;
  }
  local_res10 = LVar24;
  local_ac = iVar32;
  pWVar13 = PECMD_TokenizeQuotedField(param_1,(longlong *)&local_res10.QuadPart,(longlong *)&local_160,L',',0);
  if ((*pWVar13 == L',') && (*local_160 != L'\0')) {
    if (*local_160 == L'&') {
      uVar10 = 1;
    }
    local_f8 = LVar25.s.LowPart;
    if ((char)uVar10 != '\0') {
      local_f8 = uVar22;
    }
    local_134 = uVar10;
    PECMD_ExpandVarDispatch(param_1,(WCHAR *)(uintptr_t)local_res10.QuadPart,&local_130,0,1);
    local_res10 = local_130;
    PECMD_EvalParenStripped((longlong *)&local_res10.QuadPart,(ulonglong *)&local_e0.QuadPart);
    local_100.QuadPart = local_e0.QuadPart;
    if (-1 < local_e0.QuadPart) {
      local_158 = 0;
      local_res20.QuadPart = -4;
      if (*(WCHAR *)(uintptr_t)local_res10.QuadPart == L'#') {
        local_res10.QuadPart = local_res10.QuadPart + 2;
        PECMD_SkipLeadingControlChars(&local_res10.QuadPart);
        local_158 = *(WCHAR *)(uintptr_t)local_res10.QuadPart == L'*';
        PECMD_EvalParenStripped((longlong *)&local_res10.QuadPart,(ulonglong *)&local_res20.QuadPart);
      }
      LVar24 = local_res20;
      FUN_14005b374(&local_res10,0x2c,0x3b);
      LVar23 = local_e8;
      if (*(WCHAR *)(uintptr_t)local_res10.QuadPart != L'\0') {
        local_res10.QuadPart = local_res10.QuadPart + 2;
      }
      if ((-1 < local_e8.QuadPart) && (local_e8.QuadPart < LVar24.QuadPart)) {
        LVar24 = local_e8;
      }
      PECMD_SkipLeadingControlChars(&local_res10.QuadPart);
      PECMD_TrimWhitespace((uint16_t *)(uintptr_t)local_res10.QuadPart);
      LVar18 = local_res10;
      local_110.QuadPart = local_res10.QuadPart;
      local_d0.QuadPart = local_res10.QuadPart;
      local_78.QuadPart = 0;
      uVar30 = PECMD_AsciiWideICmp("zero",(ushort *)(uintptr_t)local_res10.QuadPart);
      local_res20.s.LowPart = (local_res20.s.LowPart & 0xffffff00u) | (uint32_t)((char)uVar30 & 0xff);
      if ((((char)uVar30 == '\0') &&
          (bVar7 = FUN_140067cf4(&local_d0.QuadPart,(ulonglong *)&local_170.QuadPart),
          (int)bVar7 != 0)) &&
         ((*(WCHAR *)(uintptr_t)local_d0.QuadPart == L'\0' || (*(WCHAR *)(uintptr_t)local_d0.QuadPart == L',')))) {
        local_res20.s.LowPart = (local_res20.s.LowPart & 0xffffff00u) | 2u;
        local_78 = local_170;
      }
      local_b8.QuadPart = LVar18.QuadPart;
      if (*(short *)(uintptr_t)LVar18.QuadPart == 0) {
LAB_1400d36c5:
        PECMD_FreeStrBuf((WCHAR **)&local_160);
        PECMD_FreeStrBuf(&local_130.QuadPart);
        if (((WCHAR *)LVar29.QuadPart != (WCHAR *)0x0) &&
           ((WCHAR *)LVar29.QuadPart != (WCHAR *)0xffffffffffffffff)) {
          CloseHandle((HANDLE)(uintptr_t)LVar29.QuadPart);
        }
        LVar23.QuadPart = 1;
        goto LAB_1400d3d7f;
      }
      local_d8.QuadPart = LVar18.QuadPart;
      pWVar14 = PECMD_ParseResourceStringRef((LPCWSTR *)&local_d8,1);
      FUN_140063344(&local_98);
      local_90.QuadPart = 0;
      local_88 = 0;
      local_178 = '\0';
      local_d0.QuadPart = 0;
      if (local_124 != 0) {
        EnterCriticalSection((LPCRITICAL_SECTION)&DAT_14013e190);
        PECMD_AllocStrSlot((uint16_t **)&local_d8);
        lVar15 = PECMD_VarLookup(param_1,(LPCWSTR)(uintptr_t)LVar18.QuadPart,(longlong *)0x0,-1,(undefined8 *)0x0);
        LVar18 = local_c8;
        if (lVar15 != 0) {
          LVar27.QuadPart = *(ulonglong *)(lVar15 + 0x18) & 0x3fffffffffffffff;
          if (((LVar24.QuadPart < 1) || (LVar27.QuadPart < local_c8.QuadPart + LVar24.QuadPart)) &&
             (LVar24.QuadPart = LVar27.QuadPart - local_c8.QuadPart, (longlong)(LVar27.QuadPart - local_c8.QuadPart) < 1)
             ) {
            LVar24.QuadPart = LVar37.QuadPart;
          }
          if ((-1 < LVar23.QuadPart) && (LVar23.QuadPart < LVar24.QuadPart)) {
            LVar24 = LVar23;
          }
          PECMD_GrowByteBuffer((void **)&local_98,(longlong)(LVar24.QuadPart + 0x200000));
          if (local_98 == 0) {
            PECMD_FreeStrBuf(&local_d8.QuadPart);
            LeaveCriticalSection((LPCRITICAL_SECTION)&DAT_14013e190);
            goto LAB_1400d3e1b;
          }
          local_110.QuadPart = local_98 + 0x100000U & 0xfffffffffff00000;
          local_c8 = LVar18;
          if (LVar27.QuadPart < LVar18.QuadPart) {
            local_c8.QuadPart = LVar27.QuadPart;
          }
          pWVar13 = (WCHAR *)(*(longlong *)(lVar15 + 8) + local_c8.QuadPart);
          local_d0 = LVar24;
          local_90 = LVar24;
          if ((0 < LVar24.QuadPart) && ((WCHAR *)local_110.QuadPart != pWVar13)) {
            PECMD_MemMoveSafe((undefined1 *)(uintptr_t)local_110.QuadPart,(longlong)pWVar13,LVar24.s.LowPart);
          }
          local_178 = '\x01';
          local_148 = '\x01';
          PECMD_FreeStrBuf(&local_d8.QuadPart);
          LeaveCriticalSection((LPCRITICAL_SECTION)&DAT_14013e190);
          LVar18 = LVar24;
          goto LAB_1400d4077;
        }
        pLVar21 = &local_d8;
LAB_1400d3d2e:
        PECMD_FreeStrBuf(&pLVar21->QuadPart);
        LeaveCriticalSection((LPCRITICAL_SECTION)&DAT_14013e190);
LAB_1400d3d40:
        PECMD_FreeStrBuf(&local_98);
        goto LAB_1400d3d4d;
      }
      LVar18.QuadPart = LVar37.QuadPart;
      if (pWVar14 != (LPCWSTR)0x0) {
        local_d0.QuadPart = (LONGLONG)(pWVar14 + 1);
        local_148 = '\x02';
        local_178 = '\x02';
        local_68[0] = 0x20;
        if (*(WCHAR *)(uintptr_t)local_d0.QuadPart == L'.') {
          local_d0.QuadPart = (LONGLONG)(pWVar14 + 2);
          local_68[0] = 0x40;
        }
        pWVar16 = (LPCWSTR)PECMD_ParseItemImageSpec((longlong *)&local_d0.QuadPart,(longlong *)&local_80,
                                         (ushort *)(const uint16_t *)L"EXEDATA");
        LVar18.QuadPart = (long long)(uintptr_t)DAT_14013cf70;
        WVar2 = *pWVar14;
        *(WCHAR *)pWVar14 = L'\0';
        LVar27.QuadPart = LVar37.QuadPart;
        if (((*(short *)(uintptr_t)local_d8.QuadPart != 0x2e) ||
            (LVar17.QuadPart = (long long)(uintptr_t)DAT_14013ca68, *(short *)(local_d8.QuadPart + 2) != 0)) &&
           (LVar17 = LVar18, *(short *)(uintptr_t)local_d8.QuadPart != 0)) {
          LVar17.QuadPart = (long long)LoadLibraryExW((LPCWSTR)(uintptr_t)local_d8.QuadPart,(HANDLE)0x0,2);
          LVar27 = LVar17;
        }
        *(WCHAR *)pWVar14 = WVar2;
        LVar18 = local_110;
        if (LVar17.QuadPart != 0) {
          LVar18.QuadPart = (long long)PECMD_LoadEncodedResource((void *)(uintptr_t)LVar17.QuadPart,local_80,pWVar16,&local_98,local_68);
        }
        if (LVar27.QuadPart != 0) {
          FreeLibrary((HMODULE)(uintptr_t)LVar27.QuadPart);
        }
        LVar17 = local_90;
        LVar27 = local_c8;
        if ((WCHAR *)LVar18.QuadPart != (WCHAR *)0x0) {
          if (((LVar24.QuadPart < 1) || (local_90.QuadPart < local_c8.QuadPart + LVar24.QuadPart))
             && (LVar24.QuadPart = local_90.QuadPart - local_c8.QuadPart,
                (longlong)(local_90.QuadPart - local_c8.QuadPart) < 1)) {
            LVar24.QuadPart = LVar37.QuadPart;
          }
          PECMD_GrowByteBuffer((void **)&local_98,(longlong)(local_90.QuadPart + 0x200000));
          if (local_98 != 0) {
            FUN_140102a90((ulonglong *)(LVar17.QuadPart + local_98),0,0x1ff);
            local_c8 = LVar27;
            if (LVar17.QuadPart < LVar27.QuadPart) {
              local_c8 = LVar17;
            }
            pWVar13 = (WCHAR *)(local_98 + local_c8.QuadPart);
            if ((-1 < LVar23.QuadPart) && (LVar23.QuadPart < LVar24.QuadPart)) {
              LVar24 = LVar23;
            }
            local_110.QuadPart = (ulonglong)(pWVar13 + 0x80000) & 0xfffffffffff00000;
            LVar18 = LVar24;
            local_d0 = LVar24;
            local_90 = LVar24;
            if ((0 < LVar24.QuadPart) && ((WCHAR *)local_110.QuadPart != pWVar13)) {
              PECMD_MemMoveSafe((undefined1 *)(uintptr_t)local_110.QuadPart,(longlong)pWVar13,LVar24.s.LowPart);
            }
            goto LAB_1400d4077;
          }
        }
LAB_1400d3e1b:
        PECMD_FreeStrBuf(&local_98);
        goto LAB_1400d36c5;
      }
LAB_1400d4077:
      LVar27 = local_120;
      local_80 = (LPCWSTR)((ulonglong)local_80 & 0xffffffffffffff00);
      local_170.QuadPart = 0;
      local_118.QuadPart = local_120.QuadPart;
      if ((char)local_b0 != '\0') {
        PECMD_EnableTokenPrivilege((const uint16_t *)L"SeManageVolumePrivilege",2,0x40);
      }
      local_138 = 0x200;
      local_14c = 0;
      local_d8.QuadPart = 0;
      local_128 = (uint)((WCHAR *)LVar27.QuadPart == (WCHAR *)0x0);
      if ((char)local_154 != '\0') {
        local_80 = (LPCWSTR)(((uintptr_t)local_80 & ~(uintptr_t)0xff) |
                 (uintptr_t)((local_140.s.LowPart & 0xff) == '\0'));
        local_154 = local_154 & 0xff;
        if (local_178 == '\0') {
          local_154 = 2;
        }
      }
      LVar17.QuadPart = LVar37.QuadPart;
      LVar23.QuadPart = LVar37.QuadPart;
      if ((char)local_134 == '\0') {
        if (local_168 == '\0') {
          cVar6 = PECMD_NormalizeVolumeDevPath((longlong *)&local_160);
          local_14c = (uint)cVar6;
          if ((int)local_14c < 0) goto LAB_1400d3d40;
        }
        else {
          local_14c = 0;
        }
        if (((char)local_f8 == '\0') &&
           ((((((local_14c != 0 || (*local_160 != L'\\')) || (local_160[1] != L'\\')) ||
              ((WVar2 = local_160[2], WVar2 != L'?' && (WVar2 != L'.')))) || (local_160[3] != L'\\')
             ) || (((WVar2 == L'?' && (local_160[4] == L'?')) && (local_160[5] == L':')))))) {
LAB_1400d4301:
          if ((WCHAR *)LVar27.QuadPart == (WCHAR *)0x0) {
            LVar27.QuadPart =
                 (long long)(uintptr_t)PECMD_OpenFileHandle((HANDLE *)&local_170,local_160,local_150 | 0xc0000000,3,
                               (LPSECURITY_ATTRIBUTES)0x0,3,0,(HANDLE)0x0);
            LVar17 = local_170;
            local_118.QuadPart = LVar27.QuadPart;
          }
          if (bVar8) goto LAB_1400d4350;
          uVar33 = (ulonglong)local_14c;
LAB_1400d43bb:
          if ((WCHAR *)LVar27.QuadPart == (WCHAR *)0x0) {
            LVar27.QuadPart =
                 (long long)(uintptr_t)PECMD_OpenFileHandle((HANDLE *)&local_170,local_160,local_150 | 0xc0000000,3,
                               (LPSECURITY_ATTRIBUTES)0x0,3,0x2000000,(HANDLE)0x0);
            LVar17 = local_170;
            local_118.QuadPart = LVar27.QuadPart;
          }
        }
        else {
          bVar8 = true;
          if ((char)local_f8 == '\0') {
            LVar29.s.LowPart = PECMD_FindFileOrDir((LPWSTR)local_160,0);
            uVar20 = (char)local_f8;
            if (extraout_EAX != 0) {
              uVar20 = 0x20;
            }
            local_f8 = (local_f8 & 0xffffff00u) | (uint32_t)(uVar20 & 0xff);
          }
          if ((WCHAR *)LVar27.QuadPart == (WCHAR *)0x0) {
            LVar27.QuadPart =
                 (long long)(uintptr_t)PECMD_OpenFileHandle((HANDLE *)&local_170,local_160,local_150 | 0xc0000000,7,
                               (LPSECURITY_ATTRIBUTES)0x0,4,0,(HANDLE)0x0);
            LVar17 = local_170;
            local_118.QuadPart = LVar27.QuadPart;
            goto LAB_1400d4301;
          }
LAB_1400d4350:
          uVar33 = (ulonglong)local_14c;
          if (((byte)local_154 == '\0') || (0 < (int)local_14c)) goto LAB_1400d43bb;
          if ((WCHAR *)LVar27.QuadPart == (WCHAR *)0x0) {
            LVar27.QuadPart =
                 (long long)(uintptr_t)PECMD_OpenFileHandle((HANDLE *)&local_170,local_160,local_150 | 0xc0000000,3,
                               (LPSECURITY_ATTRIBUTES)0x0,4,0x2000000,(HANDLE)0x0);
            LVar17 = local_170;
            local_118.QuadPart = LVar27.QuadPart;
            goto LAB_1400d43bb;
          }
        }
        if ((local_70 != (LPCWSTR)0x0) && ((WCHAR *)LVar17.QuadPart != (WCHAR *)0x0)) {
          PECMD_AppendFmtValue(param_1,(uint64_t)(uintptr_t)LVar17.QuadPart,local_70,(const void *)L"0x%I64X");
          local_170.QuadPart = 0;
          LVar17.QuadPart = LVar37.QuadPart;
        }
        LVar36.QuadPart = LVar37.QuadPart;
        if (((int)uVar33 != 0) && ((WCHAR *)LVar27.QuadPart != (WCHAR *)0x0)) {
          local_138 = PECMD_QueryDeviceIoInfo((uintptr_t)LVar27.QuadPart,0x200);
          LVar36.QuadPart = 0;
          if ((int)local_138 < 0x200) {
            local_138 = 0x200;
          }
        }
LAB_1400d4465:
        iVar32 = (int)uVar33;
        local_140.s.LowPart = -(uint)(iVar32 != 0) & local_138;
        if ((byte)local_154 != '\x02') {
          uVar33 = uVar33 & 0xffffffffffff0000;
          local_res20.QuadPart = 0;
          local_128 = (uint)uVar33;
          local_b8 = LVar36;
          if ((char)local_134 == '\0') {
            if (local_14c == 0x11) {
              local_res20.QuadPart = (long long)PECMD_GetPartitionLayoutEntry((uintptr_t)LVar27.QuadPart,0,(ulonglong *)&local_b8.QuadPart);
              LVar36 = local_b8;
              if (local_res20.QuadPart < 0) goto LAB_1400d453f;
            }
            else if ((WCHAR *)LVar27.QuadPart != (WCHAR *)0x0) {
              if (local_14c == 0) {
                LVar36 = PECMD_SetFilePointer((HANDLE)(uintptr_t)LVar27.QuadPart,(LARGE_INTEGER){0},2);
                if ((LVar36.QuadPart < (longlong)(local_e0.QuadPart + -0x400)) &&
                   ((WCHAR *)local_120.QuadPart == (WCHAR *)0x0)) {
                  CloseHandle((HANDLE)(uintptr_t)LVar17.QuadPart);
                }
              }
              else {
                LVar36.QuadPart = (long long)PECMD_GetDeviceSize((HANDLE)(uintptr_t)LVar27.QuadPart,(byte)local_14c);
                if (LVar36.QuadPart < 0) {
                  LVar36.QuadPart = 0x7fffffffffffffff;
                }
              }
            }
            if (((local_158 != 0) && (LVar24.s.LowPart == 0xfffffffc)) &&
               (LVar24.QuadPart = LVar36.QuadPart - local_e0.QuadPart,
               (longlong)(LVar36.QuadPart - local_e0.QuadPart) < 0)) {
              LVar24.QuadPart = LVar37.QuadPart;
            }
            if ((WCHAR *)LVar27.QuadPart != (WCHAR *)0x0) {
              if ((char)local_f8 == '\0') {
LAB_1400d5582:
                if (local_14c == 0) goto LAB_1400d55a8;
              }
              else if (local_14c == 0) goto LAB_1400d5578;
              uVar33 = (local_e0.QuadPart & 0xffffffffU) % (ulonglong)local_138;
              local_128 = (uint)uVar33;
              goto LAB_1400d55a8;
            }
            if (local_14c == 0) {
              if (bVar3) {
                PECMD_DeviceCheckReady(local_160);
              }
              LVar27.QuadPart =
                   (long long)(uintptr_t)PECMD_OpenFileHandle((HANDLE *)&local_170,local_160,0xc0000000,3,(LPSECURITY_ATTRIBUTES)0x0,4,0,
                                 (HANDLE)0x0);
              LVar17 = local_170;
              local_118.QuadPart = LVar27.QuadPart;
              if (((WCHAR *)LVar27.QuadPart != (WCHAR *)0x0) ||
                 (LVar27.QuadPart =
                       (long long)(uintptr_t)PECMD_OpenFileHandle((HANDLE *)&local_170,local_160,0xc0000000,3,(LPSECURITY_ATTRIBUTES)0x0,4,
                                     0x2000000,(HANDLE)0x0), LVar17 = local_170,
                 local_118.QuadPart = LVar27.QuadPart, (WCHAR *)LVar27.QuadPart != (WCHAR *)0x0)) {
LAB_1400d5578:
                LVar36.QuadPart = 0x7fffffffffffffff;
                goto LAB_1400d5582;
              }
            }
            DVar9 = GetLastError();
            LVar23.s.HighPart = 0;
            LVar23.s.LowPart = DVar9;
          }
          else {
LAB_1400d55a8:
            local_c0.QuadPart = 0;
            local_100.QuadPart = local_e0.QuadPart + local_res20.QuadPart;
            LVar36.QuadPart = LVar36.QuadPart - local_100.QuadPart;
            if (((char)local_134 != '\0') ||
               (LVar18 = PECMD_SetFilePointer((HANDLE)(uintptr_t)LVar27.QuadPart,
                                                (LARGE_INTEGER){ .QuadPart = local_100.QuadPart - (uVar33 & 0xffff) },0),
               -1 < LVar18.QuadPart)) {
              if (local_148 == '\0') {
                if ((-1 < LVar24.QuadPart) && (LVar24.QuadPart < LVar36.QuadPart)) {
                  LVar36 = LVar24;
                }
                local_res10.QuadPart = local_110.QuadPart;
                if (local_176 == 0) {
                  PECMD_AllocStringSlot2((void **)&local_78,(ulonglong)(local_138 * 2));
                  iVar32 = local_138 - ((uint)uVar33 & 0xffff);
                  local_176 = 0;
                  local_res20.s.LowPart = (local_res20.s.LowPart & 0xffffff00u) | (uint32_t)(local_147 & 0xff);
                  LVar24.QuadPart =
                       (ulonglong)local_138 + local_78.QuadPart & (ulonglong)~(local_138 - 1);
                  LVar18.QuadPart = LVar37.QuadPart;
                  uVar20 = local_176;
                  uVar4 = local_147;
                  bVar5 = local_147;
                  LVar28 = local_100;
                  while ((local_147 = bVar5, local_120 = LVar24, 0 < LVar36.QuadPart &&
                         ((uVar33 = (ulonglong)(byte)uVar4, LVar17 = local_170, LVar29 = local_108,
                          0 < LVar18.QuadPart || (*(WCHAR *)(uintptr_t)local_res10.QuadPart != L'\0'))))) {
                    LVar34.QuadPart = (longlong)iVar32;
                    local_60 = 0;
                    LVar25.QuadPart = LVar37.QuadPart;
                    local_176 = uVar20;
                    local_d0 = LVar34;
                    if (LVar34.QuadPart < 1) break;
                    do {
                      uVar30 = 0x30;
                      if (LVar36.QuadPart < 1) break;
                      if (LVar18.QuadPart < 1) {
                        if (*(WCHAR *)(uintptr_t)local_res10.QuadPart == L'\0') break;
                        PECMD_SkipLeadingControlChars(&local_res10.QuadPart);
                        local_58.QuadPart = local_res10.QuadPart;
                        local_100.QuadPart = 0;
                        if (*(WCHAR *)(uintptr_t)local_res10.QuadPart == L'*') {
                          local_b8.QuadPart = 1;
                          local_res10.QuadPart = local_res10.QuadPart + 2;
                          PECMD_EvalParenStripped((longlong *)&local_res10.QuadPart,(ulonglong *)&local_b8.QuadPart);
                          uVar33 = local_res20.QuadPart & 0xff;
                          uVar30 = 0x30;
                          LVar18 = local_b8;
                        }
                        while( true ) {
                          WVar2 = *(WCHAR *)(uintptr_t)local_res10.QuadPart;
                          if (WVar2 == L'\0') break;
                          if (WVar2 == L'*') goto LAB_1400d5957;
                          if (WVar2 == L'(') {
                            local_100.QuadPart = 0;
                            PECMD_EvalParenStripped((longlong *)&local_res10.QuadPart,(ulonglong *)&local_100.QuadPart);
                            goto LAB_1400d5957;
                          }
                          if ((((ushort)(WCHAR)uVar30 <= (ushort)WVar2) && ((ushort)WVar2 < 0x3a))
                             || (((char)uVar33 != '\0' &&
                                 ((0x60 < (ushort)(WVar2 | 0x20U) &&
                                  ((ushort)(WVar2 | 0x20U) < 0x7b)))))) break;
                          local_res10.QuadPart = local_res10.QuadPart + 2;
                        }
                        if (((WCHAR)uVar30 == *(WCHAR *)(uintptr_t)local_res10.QuadPart) &&
                           ((WCHAR)(*(WCHAR *)(local_res10.QuadPart + 2) | 0x20U) == 0x78)) {
                          local_res10.QuadPart = local_res10.QuadPart + 4;
                          local_res20.s.LowPart = (local_res20.s.LowPart & 0xffffff00u) | 1u;
LAB_1400d58d6:
                          local_100.QuadPart = 0;
                          local_ac = 2;
                          PECMD_ScanHexNumber((uint16_t **)&local_res10.QuadPart,(uint64_t *)&local_100.QuadPart,&local_ac);
                          if (9 < (ushort)(*(WCHAR *)(uintptr_t)local_res10.QuadPart + 0xffd0)) {
                            local_res20.s.LowPart = (local_res20.s.LowPart & 0xffffff00u) | (uint32_t)(bVar5 & 0xff);
                          }
                        }
                        else {
                          if ((char)uVar33 != '\0') goto LAB_1400d58d6;
                          bVar8 = FUN_140074838(&local_res10.QuadPart,(int *)&local_100.QuadPart);
                          LVar24 = local_120;
                          if ((int)bVar8 < 1) break;
                        }
                        LVar24 = local_120;
                        if (local_res10.QuadPart == local_58.QuadPart) break;
LAB_1400d5957:
                        uVar33 = local_res20.QuadPart & 0xff;
                        local_176 = (local_100.s.LowPart & 0xff);
                        LVar24 = local_120;
                        LVar34 = local_d0;
                        uVar20 = (local_100.s.LowPart & 0xff);
                      }
                      *(undefined1 *)(LVar24.QuadPart + local_60) = uVar20;
                      local_60 = local_60 + 1;
                      LVar18.QuadPart = LVar18.QuadPart + -1;
                      LVar25.s.LowPart = LVar25.s.LowPart + 1;
                      LVar25.s.HighPart = 0;
                      LVar36.QuadPart = LVar36.QuadPart + -1;
                    } while (local_60 < LVar34.QuadPart);
                    LVar27 = local_118;
                    uVar10 = local_128;
                    LVar17 = local_170;
                    LVar29 = local_108;
                    if ((int)LVar25.s.LowPart < 1) break;
                    if ((char)local_134 == '\0') {
                      PECMD_ReadModifyWrite((HANDLE)(uintptr_t)local_118.QuadPart,(undefined1 *)(uintptr_t)LVar24.QuadPart,
                                    (longlong)(int)LVar25.s.LowPart,local_128 & 0xffff,
                                    local_140.s.LowPart);
                    }
                    else {
                      PECMD_MemMoveSafe((byte *)(local_d8.QuadPart + LVar28.QuadPart),LVar24.QuadPart,
                                    LVar25.s.LowPart);
                      LVar28.QuadPart = LVar28.QuadPart + (int)LVar25.s.LowPart;
                    }
                    local_128 = uVar10 & 0xffff0000;
                    iVar32 = 0x200;
                    LVar24 = local_120;
                    LVar17 = local_170;
                    LVar29 = local_108;
                    uVar20 = local_176;
                    uVar4 = (local_res20.s.LowPart & 0xff);
                    bVar5 = local_147;
                  }
                  PECMD_FreeStrBuf(&local_78.QuadPart);
                }
                else {
                  FUN_14006355c(&local_c0.QuadPart,(LPCWSTR)(uintptr_t)local_110.QuadPart,-1,0xffffffffffffffff);
                  DVar9 = lstrlenA((LPCSTR)(uintptr_t)local_c0.QuadPart);
                  if (LVar36.QuadPart < (int)DVar9) {
                    DVar9 = LVar36.s.LowPart;
                  }
                  if (0 < (int)DVar9) {
                    LVar36.QuadPart = (longlong)(int)DVar9;
                    LVar37 = local_c0;
                    uVar26 = local_14c;
                    goto LAB_1400d565e;
                  }
                }
              }
              else {
                if (local_d0.QuadPart < LVar36.QuadPart) {
                  LVar36 = local_d0;
                }
                if ((char)local_134 == '\0') {
                  LVar37 = local_110;
                  uVar26 = local_140.s.LowPart;
                  if (0 < LVar36.QuadPart) {
LAB_1400d565e:
                    PECMD_ReadModifyWrite((HANDLE)(uintptr_t)LVar27.QuadPart,(undefined1 *)(uintptr_t)LVar37.QuadPart,LVar36.QuadPart,
                                  (uint)uVar33 & 0xffff,uVar26);
                  }
                }
                else {
                  PECMD_MemMoveSafe((byte *)(local_d8.QuadPart + local_100.QuadPart),local_110.QuadPart,
                                LVar36.s.LowPart);
                }
              }
              if ((WCHAR *)LVar27.QuadPart != (WCHAR *)0x0) {
                FlushFileBuffers((HANDLE)(uintptr_t)LVar27.QuadPart);
              }
              PECMD_FreeStrBuf(&local_c0.QuadPart);
              goto LAB_1400d5a79;
            }
            DVar9 = GetLastError();
            LVar23.s.HighPart = 0;
            LVar23.s.LowPart = DVar9;
            PECMD_FreeStrBuf(&local_c0.QuadPart);
          }
LAB_1400d47eb:
          if (((WCHAR *)LVar17.QuadPart != (WCHAR *)0x0) &&
             ((WCHAR *)LVar17.QuadPart != (WCHAR *)0xffffffffffffffff)) {
            CloseHandle((HANDLE)(uintptr_t)LVar17.QuadPart);
          }
          PECMD_FreeStrBuf(&local_98);
          PECMD_FreeStrBuf((WCHAR **)&local_160);
          PECMD_FreeStrBuf(&local_130.QuadPart);
          if (((WCHAR *)LVar29.QuadPart != (WCHAR *)0x0) &&
             ((WCHAR *)LVar29.QuadPart != (WCHAR *)0xffffffffffffffff)) {
            CloseHandle((HANDLE)(uintptr_t)LVar29.QuadPart);
          }
          goto LAB_1400d3d7f;
        }
        if ((local_a0.QuadPart < 1) || (local_a8.QuadPart < 1)) {
LAB_1400d48eb:
          if (((WCHAR *)LVar17.QuadPart != (WCHAR *)0x0) &&
             ((WCHAR *)LVar17.QuadPart != (WCHAR *)0xffffffffffffffff)) {
            CloseHandle((HANDLE)(uintptr_t)LVar17.QuadPart);
          }
          goto LAB_1400d3d40;
        }
        if (local_e8.QuadPart < 0) {
          if ((WCHAR *)local_f0.QuadPart == (WCHAR *)0x7fffffffffffffff) {
            local_e8.QuadPart = 0x7fffffffffffffff;
          }
          else {
            local_e8.QuadPart = local_a0.QuadPart * local_f0.QuadPart;
          }
        }
        else {
          local_f0.QuadPart = (local_a0.QuadPart + -1 + local_e8.QuadPart) / local_a0.QuadPart;
        }
        LVar23 = local_e8;
        pWVar14 = PECMD_UnquoteString((short *)(uintptr_t)local_b8.QuadPart);
        PECMD_StrDupAssign((uint16_t **)&local_140,pWVar14);
        local_110.QuadPart = local_a0.QuadPart;
        if (((char)local_134 < '\x01') && ((WCHAR *)LVar27.QuadPart == (WCHAR *)0x0)) {
LAB_1400d4535:
          PECMD_FreeStrBuf(&local_140.QuadPart);
LAB_1400d453f:
          if (((WCHAR *)LVar17.QuadPart != (WCHAR *)0x0) &&
             ((WCHAR *)LVar17.QuadPart != (WCHAR *)0xffffffffffffffff)) {
            CloseHandle((HANDLE)(uintptr_t)LVar17.QuadPart);
          }
          goto LAB_1400d3e1b;
        }
        if (((char)local_ac == '\0') || ((0 < iVar32 || ('\0' < (char)local_134)))) {
LAB_1400d4746:
          if ((local_f0.QuadPart < 1) || (local_e8.QuadPart < 1)) {
            if (local_e0.QuadPart != 0) {
              PECMD_SetFilePointer((HANDLE)(uintptr_t)LVar27.QuadPart,(LARGE_INTEGER){ .QuadPart = local_e0.QuadPart },0);
              LVar37.QuadPart = 0;
            }
          }
          else {
            local_f8 = 0;
            local_108.QuadPart = 0;
            local_d0.QuadPart = 0;
            if (((local_res20.s.LowPart & 0xff) != '\0') || (bVar8 = true, local_178 != '\0')) {
              bVar8 = false;
            }
            if (0 < iVar32) {
              local_108.QuadPart = (longlong)((int)local_e0.s.LowPart % (int)local_138);
            }
            LVar23.QuadPart = local_e0.QuadPart - local_108.QuadPart;
            if (((0 < LVar23.QuadPart) && ((char)local_134 < '\x01')) &&
               (LVar24 = PECMD_SetFilePointer((HANDLE)(uintptr_t)LVar27.QuadPart,(LARGE_INTEGER){ .QuadPart = LVar23.QuadPart },0), LVar24.QuadPart != LVar23.QuadPart)) {
              DVar9 = GetLastError();
              LVar23.s.HighPart = 0;
              LVar23.s.LowPart = DVar9;
              PECMD_FreeStrBuf(&local_140.QuadPart);
              goto LAB_1400d47eb;
            }
            pWVar14 = (LPCWSTR)CONCAT44(local_140.s.HighPart,local_140.s.LowPart);
            local_60 = 0;
            local_168 = *pWVar14 == L'&';
            local_70 = (LPCWSTR)0x0;
            local_120.QuadPart = 0;
            if (!(bool)local_168) {
LAB_1400d49ca:
              LVar24.QuadPart = LVar37.QuadPart;
              LVar25.s.LowPart = uVar22;
              if ((!bVar8) || ('\0' < local_168)) {
LAB_1400d4b59:
                if (0 < (int)local_14c) {
                  lVar15 = (longlong)(int)local_138;
                  local_a8.QuadPart = ((lVar15 + -1 + local_a8.QuadPart) / lVar15) * lVar15;
                }
                if (0 < (int)LVar25.s.LowPart) {
                  lVar15 = (longlong)(int)local_c0.s.LowPart;
                  local_a0.QuadPart = ((lVar15 + -1 + local_a0.QuadPart) / lVar15) * lVar15;
                }
                LVar23 = local_a0;
                if (local_a0.QuadPart != local_a8.QuadPart) {
                  LVar23.QuadPart = local_a0.QuadPart + local_a8.QuadPart;
                }
                LVar23.QuadPart = LVar23.QuadPart + 0x8000;
                uVar26 = local_c0.s.LowPart;
                if ((int)local_c0.s.LowPart < (int)local_138) {
                  uVar26 = local_138;
                }
                uVar33 = (longlong)(int)uVar26 + 0x2800 + LVar23.QuadPart;
                LVar36 = local_a8;
                local_e0.QuadPart = LVar23.QuadPart;
                PECMD_AllocStringSlot2((void **)&local_b8,uVar33 + 0x100000);
                LVar18.QuadPart = (ulonglong)(local_b8.QuadPart + 0x100000) & 0xfffffffffff00000;
                local_118.QuadPart = (longlong)(LVar18.QuadPart + 0x2800) + LVar23.QuadPart;
                local_58.QuadPart = LVar18.QuadPart;
                FUN_140102a90((ulonglong *)(uintptr_t)LVar18.QuadPart,local_78.QuadPart & 0xffffffff,uVar33);
                LVar23 = local_d0;
                uVar33 = LVar36.QuadPart & 0xffffffffffffff00;
                local_154 = (uint)uVar33;
                LVar36.QuadPart = LVar37.QuadPart;
                if (0 < local_d0.QuadPart) {
                  DVar9 = local_c0.s.LowPart;
                  local_110.QuadPart = 0;
                  ReadFile((HANDLE)(uintptr_t)LVar24.QuadPart,(LPVOID)(uintptr_t)LVar18.QuadPart,DVar9,(LPDWORD)&local_110.QuadPart,
                           (LPOVERLAPPED)0x0);
                  LVar36.QuadPart = (longlong)(int)DVar9 - LVar23.QuadPart;
                  local_e8.QuadPart = local_e8.QuadPart - LVar36.QuadPart;
                  if (local_e8.QuadPart < 0) {
                    LVar36.QuadPart = LVar36.QuadPart + local_e8.QuadPart;
                  }
                  PECMD_MemMoveSafe((undefined1 *)(uintptr_t)LVar18.QuadPart,LVar18.QuadPart + LVar23.QuadPart,
                                LVar36.s.LowPart);
                  uVar33 = (ulonglong)local_154;
                }
                if (0 < local_108.QuadPart) {
                  LVar23 = PECMD_SetFilePointer((HANDLE)(uintptr_t)LVar27.QuadPart,(LARGE_INTEGER){0},1);
                  local_110.QuadPart = 0;
                  ReadFile((HANDLE)(uintptr_t)LVar27.QuadPart,(LPVOID)(uintptr_t)local_118.QuadPart,local_138,(LPDWORD)&local_110.QuadPart,
                           (LPOVERLAPPED)0x0);
                  PECMD_MemMoveSafe((undefined1 *)(LVar18.QuadPart + local_108.QuadPart),LVar18.QuadPart
                                ,LVar36.s.LowPart);
                  PECMD_MemMoveSafe((undefined1 *)(uintptr_t)LVar18.QuadPart,local_118.QuadPart,local_108.s.LowPart);
                  LVar36.QuadPart = LVar36.QuadPart + local_108.QuadPart;
                  PECMD_SetFilePointer((HANDLE)(uintptr_t)LVar27.QuadPart,(LARGE_INTEGER){ .QuadPart = LVar23.QuadPart },0);
                  local_154 = local_154 & 0xff;
                  if ((local_res20.s.LowPart & 0xff) != '\0') {
                    local_154 = 1;
                  }
                  uVar33 = (ulonglong)local_154;
                }
                bVar3 = true;
                LVar23.QuadPart = LVar18.QuadPart;
                if (LVar36.QuadPart < 1) goto LAB_1400d4de7;
                do {
                  for (; LVar36.QuadPart < local_a8.QuadPart;
                      LVar36.QuadPart = LVar36.QuadPart + local_110.QuadPart) {
                    local_108 = LVar18;
                    if ((0 < LVar36.QuadPart) && (LVar23.QuadPart != LVar18.QuadPart)) {
                      PECMD_MemMoveSafe((undefined1 *)(uintptr_t)LVar23.QuadPart,LVar18.QuadPart,LVar36.s.LowPart);
                    }
LAB_1400d4de7:
                    LVar18 = LVar23;
                    if ((local_e8.QuadPart < 1) || (!bVar3)) {
LAB_1400d4edb:
                      if (0 < LVar36.QuadPart) {
                        uVar33 = (ulonglong)local_154;
                        bVar3 = false;
                        break;
                      }
LAB_1400d5368:
                      if ((uVar22 != 0) ||
                         ((0 < local_e8.QuadPart &&
                          ((WCHAR *)local_e8.QuadPart != (WCHAR *)0x7fffffffffffffff))))
                      goto LAB_1400d4fca;
                      goto LAB_1400d4fdd;
                    }
                    if (bVar8) {
                      LVar23 = local_e8;
                      if (local_a0.QuadPart <= local_e8.QuadPart) {
                        LVar23 = local_a0;
                      }
                      if (local_e0.QuadPart < LVar23.QuadPart + LVar36.QuadPart) {
                        LVar23.QuadPart = local_e0.QuadPart - LVar36.QuadPart;
                      }
                      if (0 < (int)local_f8) {
                        LVar23.QuadPart =
                             (int)local_c0.s.LowPart + LVar23.QuadPart &
                             (longlong)(int)~(local_c0.s.LowPart - 1);
                      }
                      if (local_168 == '\0') {
                        local_110.QuadPart = 0;
                        ReadFile((HANDLE)(uintptr_t)LVar24.QuadPart,(LPVOID)(LVar36.QuadPart + LVar18.QuadPart),
                                 LVar23.s.LowPart,(LPDWORD)&local_110.QuadPart,(LPOVERLAPPED)0x0);
                      }
                      else {
                        LVar28.QuadPart = (longlong)local_70 - local_c8.QuadPart;
                        if (LVar23.QuadPart < (longlong)((longlong)local_70 - local_c8.QuadPart)) {
                          LVar28 = LVar23;
                        }
                        if (0 < LVar23.QuadPart) {
                          PECMD_MemMoveSafe((undefined1 *)(LVar36.QuadPart + LVar18.QuadPart),
                                        local_c8.QuadPart + local_60,LVar28.s.LowPart);
                        }
                        local_c8.QuadPart = local_c8.QuadPart + LVar28.QuadPart;
                        local_110 = LVar28;
                      }
                      if (local_110.QuadPart < 1) goto LAB_1400d4edb;
                      if (local_e8.QuadPart < local_110.QuadPart) {
                        local_110.QuadPart = local_e8.QuadPart;
                      }
                    }
                    else {
                      local_110 = local_e8;
                      if (local_a8.QuadPart <= local_e8.QuadPart) {
                        local_110 = local_a8;
                      }
                      if (local_e0.QuadPart < LVar36.QuadPart + local_110.QuadPart) {
                        local_110.QuadPart = local_e0.QuadPart - LVar36.QuadPart;
                      }
                    }
                    if ((WCHAR *)local_e8.QuadPart != (WCHAR *)0x7fffffffffffffff) {
                      local_e8.QuadPart = local_e8.QuadPart - local_110.QuadPart;
                    }
                    uVar33 = (ulonglong)local_154;
                    LVar23 = LVar18;
                  }
                  local_150 = 0;
                  local_120 = LVar36;
                  if (local_a8.QuadPart <= LVar36.QuadPart) {
                    local_120 = local_a8;
                  }
                  if ((int)local_14c < 1) {
                    local_f0.QuadPart = 0;
                  }
                  else {
                    local_f0.QuadPart = local_120.QuadPart % (longlong)(int)local_138;
                  }
                  LVar23.QuadPart = local_120.QuadPart - local_f0.QuadPart;
                  local_108 = LVar18;
                  local_78.QuadPart = LVar23.QuadPart;
                  if (LVar23.QuadPart < 1) goto LAB_1400d500b;
                  if ((char)local_134 == '\0') {
                    bVar7 = PECMD_WriteFileChunked((HANDLE)(uintptr_t)LVar27.QuadPart,(const void *)(uintptr_t)LVar18.QuadPart,LVar23.s.LowPart,
                                          (int *)&local_150,local_138);
                    if ((int)bVar7 != 0) goto LAB_1400d522e;
                    if ((char)local_128 != '\0') goto LAB_1400d5126;
LAB_1400d4fc4:
                    GetLastError();
LAB_1400d4fca:
                    LVar25.s.LowPart = GetLastError();
                    if (LVar25.s.LowPart == 0) {
                      LVar25.s.LowPart = 1;
                    }
                    LVar37.s.HighPart = 0;
                    LVar37.s.LowPart = LVar25.s.LowPart;
LAB_1400d4fdd:
                    PECMD_FreeStrBuf(&local_b8.QuadPart);
                    if (((WCHAR *)LVar24.QuadPart != (WCHAR *)0x0) &&
                       ((WCHAR *)LVar24.QuadPart != (WCHAR *)0xffffffffffffffff)) {
                      CloseHandle((HANDLE)(uintptr_t)LVar24.QuadPart);
                    }
                    goto LAB_1400d46ff;
                  }
                  PECMD_MemMoveSafe((byte *)(local_d8.QuadPart + local_100.QuadPart),LVar18.QuadPart,
                                LVar23.s.LowPart);
                  local_100.QuadPart = local_100.QuadPart + LVar23.QuadPart;
                  uVar19 = LVar23.QuadPart & 0xffffffff;
                  while( true ) {
                    uVar33 = (ulonglong)local_154;
                    if (LVar23.s.LowPart < (uint)uVar19) {
                      uVar19 = LVar23.QuadPart & 0xffffffff;
                    }
                    local_150 = (DWORD)uVar19;
                    if ((char)local_154 != '\0') {
                      FUN_140102a90((ulonglong *)(uintptr_t)LVar18.QuadPart,0,uVar19);
                      uVar19 = (ulonglong)local_150;
                      uVar33 = uVar33 & 0xffffffffffffff00;
                      local_154 = (uint)uVar33;
                    }
                    LVar36.QuadPart = LVar36.QuadPart - uVar19;
                    local_120.QuadPart = local_120.QuadPart - uVar19;
                    local_108.QuadPart = LVar18.QuadPart + LVar23.QuadPart;
                    local_128 = local_128 & 0xffffff00;
LAB_1400d500b:
                    if (0 < local_f0.QuadPart) {
                      LVar23.QuadPart = (longlong)(int)local_138;
                      local_120 = LVar23;
                      FUN_140102a90((ulonglong *)(uintptr_t)local_118.QuadPart,0,LVar23.QuadPart);
                      lVar15 = LVar23.QuadPart - local_f0.QuadPart;
                      LVar23 = PECMD_SetFilePointer((HANDLE)(uintptr_t)LVar27.QuadPart,(LARGE_INTEGER){0},1);
                      local_110.QuadPart = 0;
                      ReadFile((HANDLE)(uintptr_t)LVar27.QuadPart,(LPVOID)(uintptr_t)local_118.QuadPart,local_138,
                               (LPDWORD)&local_110.QuadPart,(LPOVERLAPPED)0x0);
                      PECMD_MemMoveSafe((undefined1 *)(local_108.QuadPart + LVar36.QuadPart),
                                    local_f0.QuadPart + local_118.QuadPart,(int)lVar15);
                      LVar36.QuadPart = LVar36.QuadPart + lVar15;
                      PECMD_SetFilePointer((HANDLE)(uintptr_t)LVar27.QuadPart,(LARGE_INTEGER){ .QuadPart = LVar23.QuadPart },0);
                      uVar33 = (ulonglong)(byte)local_154;
                      if ((local_res20.s.LowPart & 0xff) != '\0') {
                        uVar33 = 1;
                      }
                      local_154 = (uint)uVar33;
                      LVar23 = local_78;
                    }
                    LVar18 = local_108;
                    local_150 = 0;
                    if (local_120.QuadPart < 1) goto LAB_1400d52a2;
                    BVar11 = WriteFile((HANDLE)(uintptr_t)LVar27.QuadPart,(const void *)(uintptr_t)local_108.QuadPart,local_120.s.LowPart,
                                       &local_150,(LPOVERLAPPED)0x0);
                    if (BVar11 != 0) break;
                    if ((char)local_128 == '\0') {
                      uVar22 = 1;
                      goto LAB_1400d5368;
                    }
                    local_128 = (local_128 & 0xffffff00u) | 2u;
LAB_1400d5126:
                    GetLastError();
                    LVar25.s.LowPart = local_128;
                    local_128 = local_128 & 0xffffff00;
                    local_150 = 0;
                    if (((WCHAR *)LVar17.QuadPart != (WCHAR *)0x0) &&
                       ((WCHAR *)LVar17.QuadPart != (WCHAR *)0xffffffffffffffff)) {
                      CloseHandle((HANDLE)(uintptr_t)LVar17.QuadPart);
                    }
                    local_170.QuadPart = 0;
                    LVar27.QuadPart =
                         (long long)(uintptr_t)PECMD_OpenFileHandle((HANDLE *)&local_170,local_160,0xc0000000,0,(LPSECURITY_ATTRIBUTES)0x0,
                                       4,0,(HANDLE)0x0);
                    if (((WCHAR *)LVar27.QuadPart == (WCHAR *)0x0) &&
                       (LVar27.QuadPart =
                             (long long)(uintptr_t)PECMD_OpenFileHandle((HANDLE *)&local_170,local_160,0xc0000000,0,
                                           (LPSECURITY_ATTRIBUTES)0x0,3,0,(HANDLE)0x0),
                       (WCHAR *)LVar27.QuadPart == (WCHAR *)0x0)) {
                      LVar27.QuadPart =
                           (long long)(uintptr_t)PECMD_OpenFileHandle((HANDLE *)&local_170,local_160,0xc0000000,0,
                                         (LPSECURITY_ATTRIBUTES)0x0,3,0x2000000,(HANDLE)0x0);
                    }
                    bVar7 = PECMD_WriteFileChunked((HANDLE)(uintptr_t)LVar27.QuadPart,(const void *)(uintptr_t)LVar18.QuadPart,LVar23.s.LowPart,
                                          (int *)&local_150,local_138);
                    LVar17 = local_170;
                    if ((int)bVar7 == 0) goto LAB_1400d4fc4;
                    if ((char)LVar25.s.LowPart == '\x02') break;
LAB_1400d522e:
                    uVar19 = (ulonglong)local_150;
                  }
                  uVar33 = (ulonglong)local_154;
LAB_1400d52a2:
                  if ((char)uVar33 != '\0') {
                    FUN_140102a90((ulonglong *)(uintptr_t)LVar18.QuadPart,0,(ulonglong)local_150);
                    uVar33 = uVar33 & 0xffffffffffffff00;
                    local_154 = (uint)uVar33;
                  }
                  LVar18.QuadPart = LVar18.QuadPart + (ulonglong)local_150;
                  LVar36.QuadPart = LVar36.QuadPart - (ulonglong)local_150;
                  LVar23 = local_58;
                } while( true );
              }
              cVar6 = PECMD_NormalizeDiskDevicePath((longlong *)&local_140.QuadPart);
              LVar25.s.LowPart = (uint)cVar6;
              local_f8 = LVar25.s.LowPart;
              PECMD_OpenFileHandle((HANDLE *)&local_120,(LPCWSTR)CONCAT44(local_140.s.HighPart,local_140.s.LowPart),
                            0x80000000,3,(LPSECURITY_ATTRIBUTES)0x0,3,0,(HANDLE)0x0);
              if (((WCHAR *)local_120.QuadPart != (WCHAR *)0x0) ||
                 (PECMD_OpenFileHandle((HANDLE *)&local_120,
                                (LPCWSTR)CONCAT44(local_140.s.HighPart,local_140.s.LowPart),
                                0x80000000,3,(LPSECURITY_ATTRIBUTES)0x0,3,0x2000000,(HANDLE)0x0),
                 (WCHAR *)local_120.QuadPart != (WCHAR *)0x0)) {
                LVar24 = local_120;
                if (LVar25.s.LowPart == 0) {
                  uVar10 = 0x200;
                }
                else {
                  uVar10 = PECMD_QueryDeviceIoInfo((uintptr_t)local_120.QuadPart,0x200);
                  local_c0.s.HighPart = 0;
                  local_c0.s.LowPart = uVar10;
                }
                LVar23.QuadPart = LVar37.QuadPart;
                if (0 < (int)LVar25.s.LowPart) {
                  local_d0.QuadPart = (longlong)(int)((longlong)((ulonglong)(uint)((int)local_c8.s.LowPart >> 0x1f) << 0x20 |
                        local_c8.QuadPart & 0xffffffffU) % (longlong)(int)uVar10);
                  LVar23 = local_d0;
                }
                LVar23.QuadPart = local_c8.QuadPart - LVar23.QuadPart;
                if ((LVar23.QuadPart < 1) ||
                   (LVar18 = PECMD_SetFilePointer((HANDLE)(uintptr_t)LVar24.QuadPart,(LARGE_INTEGER){ .QuadPart = LVar23.QuadPart },0), LVar18.QuadPart == LVar23.QuadPart))
                goto LAB_1400d4b59;
                DVar9 = GetLastError();
                LVar23.s.HighPart = 0;
                LVar23.s.LowPart = DVar9;
                if (((WCHAR *)LVar24.QuadPart != (WCHAR *)0x0) &&
                   ((WCHAR *)LVar24.QuadPart != (WCHAR *)0xffffffffffffffff)) {
                  CloseHandle((HANDLE)(uintptr_t)LVar24.QuadPart);
                }
                PECMD_FreeStrBuf(&local_140.QuadPart);
                if (((WCHAR *)LVar17.QuadPart != (WCHAR *)0x0) &&
                   ((WCHAR *)LVar17.QuadPart != (WCHAR *)0xffffffffffffffff)) {
                  CloseHandle((HANDLE)(uintptr_t)LVar17.QuadPart);
                }
                PECMD_FreeStrBuf(&local_98);
                PECMD_FreeStrBuf((WCHAR **)&local_160);
                PECMD_FreeStrBuf(&local_130.QuadPart);
                if (((WCHAR *)LVar29.QuadPart != (WCHAR *)0x0) &&
                   ((WCHAR *)LVar29.QuadPart != (WCHAR *)0xffffffffffffffff)) {
                  CloseHandle((HANDLE)(uintptr_t)LVar29.QuadPart);
                }
                goto LAB_1400d3d7f;
              }
              goto LAB_1400d4535;
            }
            EnterCriticalSection((LPCRITICAL_SECTION)&DAT_14013e190);
            PECMD_AllocStrSlot((uint16_t **)&local_118);
            lVar15 = PECMD_VarLookup(param_1,pWVar14,(longlong *)0x0,-1,(undefined8 *)0x0);
            if (lVar15 == 0) {
              PECMD_FreeStrBuf(&local_118.QuadPart);
              LeaveCriticalSection((LPCRITICAL_SECTION)&DAT_14013e190);
              PECMD_FreeStrBuf(&local_140.QuadPart);
              goto LAB_1400d48eb;
            }
            local_60 = *(longlong *)(lVar15 + 8);
            local_70 = (LPCWSTR)(*(ulonglong *)(lVar15 + 0x18) & 0x3fffffffffffffff);
            if (local_70 == (LPCWSTR)0x0) {
              LVar37.QuadPart = -0x7ff8ffa9;
            }
            else {
              if ((char)local_134 == '\0') {
                PECMD_FreeStrBuf(&local_118.QuadPart);
                LeaveCriticalSection((LPCRITICAL_SECTION)&DAT_14013e190);
                goto LAB_1400d49ca;
              }
              LVar23.QuadPart = LVar36.QuadPart - local_e0.QuadPart;
              if (local_e8.QuadPart < (longlong)(LVar36.QuadPart - local_e0.QuadPart)) {
                LVar23 = local_e8;
              }
              if ((longlong)((longlong)local_70 - local_c8.QuadPart) < LVar23.QuadPart) {
                LVar23.QuadPart = (longlong)local_70 - local_c8.QuadPart;
              }
              if (0 < LVar23.QuadPart) {
                PECMD_MemMoveSafe((byte *)(local_d8.QuadPart + local_e0.QuadPart),
                              local_60 + local_c8.QuadPart,LVar23.s.LowPart);
              }
            }
            PECMD_FreeStrBuf(&local_118.QuadPart);
            LeaveCriticalSection((LPCRITICAL_SECTION)&DAT_14013e190);
          }
        }
        else {
          if ((WCHAR *)local_120.QuadPart == (WCHAR *)0x0) {
            if (((WCHAR *)LVar17.QuadPart != (WCHAR *)0x0) &&
               ((WCHAR *)LVar17.QuadPart != (WCHAR *)0xffffffffffffffff)) {
              CloseHandle((HANDLE)(uintptr_t)LVar17.QuadPart);
            }
            local_170.QuadPart = 0;
            LVar17.QuadPart = LVar37.QuadPart;
          }
          if (bVar3) {
            PECMD_DeviceCheckReady(local_160);
          }
          if ((WCHAR *)local_120.QuadPart == (WCHAR *)0x0) {
            LVar27.QuadPart =
                 (long long)(uintptr_t)PECMD_OpenFileHandle((HANDLE *)&local_170,local_160,0xc0000000,3,(LPSECURITY_ATTRIBUTES)0x0,4,0,
                               (HANDLE)0x0);
            LVar17 = local_170;
            if ((WCHAR *)LVar27.QuadPart == (WCHAR *)0x0) {
              LVar27.QuadPart =
                   (long long)(uintptr_t)PECMD_OpenFileHandle((HANDLE *)&local_170,local_160,0xc0000000,3,(LPSECURITY_ATTRIBUTES)0x0,4,
                                 0x2000000,(HANDLE)0x0);
              LVar17 = local_170;
              goto LAB_1400d463a;
            }
          }
          else {
LAB_1400d463a:
            if ((WCHAR *)LVar27.QuadPart == (WCHAR *)0x0) goto LAB_1400d4535;
          }
          if (((local_res20.s.LowPart & 0xff) == '\0') || (local_f0.QuadPart < 0)) goto LAB_1400d4746;
          LVar24.QuadPart = local_a8.QuadPart * local_f0.QuadPart;
          if ((-1 < LVar23.QuadPart) &&
             (LVar24.QuadPart - LVar23.QuadPart != 0 && LVar23.QuadPart <= LVar24.QuadPart)) {
            LVar24 = LVar23;
          }
          LVar24.QuadPart = LVar24.QuadPart + local_e0.QuadPart;
          if (0x7ffffffffffffffe < local_f0.QuadPart) {
            LVar24.QuadPart = 0x7ffffffffffffdff;
          }
          if ((char)local_80 == '\0') {
            LVar23 = PECMD_SetFilePointer((HANDLE)(uintptr_t)LVar27.QuadPart,(LARGE_INTEGER){0},2);
            LVar25.s.LowPart = (uint)local_80 & 0xff;
            if (LVar23.QuadPart < LVar24.QuadPart) {
              LVar25.s.LowPart = 1;
            }
            local_80 = (LPCWSTR)(((uintptr_t)local_80 & 0xffffffff00000000ULL) | (uint32_t)LVar25.s.LowPart);
          }
          LVar23 = PECMD_SetFilePointer((HANDLE)(uintptr_t)LVar27.QuadPart,(LARGE_INTEGER){ .QuadPart = LVar24.QuadPart },0);
          if (LVar23.QuadPart != LVar24.QuadPart) {
            if (local_f0.QuadPart < 0x7fffffffffffffff) {
              PECMD_SetFilePointer((HANDLE)(uintptr_t)LVar27.QuadPart,(LARGE_INTEGER){0},0);
            }
LAB_1400d4737:
            goto LAB_1400d4746;
          }
          if (0x7ffffffffffffffe < local_f0.QuadPart) goto LAB_1400d4737;
        }
LAB_1400d46ff:
        PECMD_FreeStrBuf(&local_140.QuadPart);
        LVar23 = LVar37;
      }
      else {
        EnterCriticalSection((LPCRITICAL_SECTION)&DAT_14013e190);
        PECMD_AllocStrSlot((uint16_t **)&local_140);
        lVar15 = PECMD_VarLookup(param_1,local_160,(longlong *)0x0,-1,(undefined8 *)0x0);
        if (lVar15 == 0) {
LAB_1400d4156:
          pLVar21 = &local_140;
          goto LAB_1400d3d2e;
        }
        local_d8.QuadPart = ((LARGE_INTEGER *)(lVar15 + 8))->QuadPart;
        LVar36.QuadPart = *(ulonglong *)(lVar15 + 0x18) & 0x3fffffffffffffff;
        if ((WCHAR *)LVar36.QuadPart == (WCHAR *)0x0) goto LAB_1400d4156;
        if (local_178 == '\0') {
          PECMD_FreeStrBuf(&local_140.QuadPart);
          LeaveCriticalSection((LPCRITICAL_SECTION)&DAT_14013e190);
          uVar33 = (ulonglong)local_14c;
          goto LAB_1400d4465;
        }
        LVar37.QuadPart = LVar36.QuadPart - local_e0.QuadPart;
        if (LVar24.QuadPart < (longlong)(LVar36.QuadPart - local_e0.QuadPart)) {
          LVar37 = LVar24;
        }
        if (0 < LVar37.QuadPart) {
          PECMD_MemMoveSafe((byte *)(local_d8.QuadPart + local_e0.QuadPart),local_110.QuadPart,
                        LVar18.s.LowPart);
        }
        PECMD_FreeStrBuf(&local_140.QuadPart);
        LeaveCriticalSection((LPCRITICAL_SECTION)&DAT_14013e190);
      }
LAB_1400d5a79:
      if (((char)local_80 != '\0') && ((WCHAR *)LVar27.QuadPart != (WCHAR *)0x0)) {
        SetEndOfFile((HANDLE)(uintptr_t)LVar27.QuadPart);
      }
      if (((((char)local_b0 != '\0') && ((WCHAR *)LVar27.QuadPart != (WCHAR *)0x0)) &&
          (BVar11 = SetFilePointerEx((HANDLE)(uintptr_t)LVar27.QuadPart,0x0,(longlong *)&local_res20,2), BVar11 != 0)
          ) && (PECMD_GetApiProcCached("SetFileValidData","Kernel32.DLL",(longlong *)&DAT_14013d380,
                              (longlong *)0x0), DAT_14013d380 != (code *)0x0)) {
        (*DAT_14013d380)((void *)(uintptr_t)LVar27.QuadPart,local_res20.QuadPart);
      }
      if (((WCHAR *)LVar17.QuadPart != (WCHAR *)0x0) &&
         ((WCHAR *)LVar17.QuadPart != (WCHAR *)0xffffffffffffffff)) {
        CloseHandle((HANDLE)(uintptr_t)LVar17.QuadPart);
      }
      PECMD_FreeStrBuf(&local_98);
      PECMD_FreeStrBuf((WCHAR **)&local_160);
      PECMD_FreeStrBuf(&local_130.QuadPart);
      if (((WCHAR *)LVar29.QuadPart != (WCHAR *)0x0) &&
         ((WCHAR *)LVar29.QuadPart != (WCHAR *)0xffffffffffffffff)) {
        CloseHandle((HANDLE)(uintptr_t)LVar29.QuadPart);
      }
      goto LAB_1400d3d7f;
    }
  }
LAB_1400d3d4d:
  PECMD_FreeStrBuf((WCHAR **)&local_160);
  PECMD_FreeStrBuf(&local_130.QuadPart);
  if (((WCHAR *)LVar29.QuadPart != (WCHAR *)0x0) &&
     ((WCHAR *)LVar29.QuadPart != (WCHAR *)0xffffffffffffffff)) {
    CloseHandle((HANDLE)(uintptr_t)LVar29.QuadPart);
  }
  LVar23.QuadPart = -0x7ff8ffa9;
LAB_1400d3d7f:
  PECMD_FreeStrBuf(local_50);
  return (uint64_t)LVar23.QuadPart;
}
}
#undef CONCAT44

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

void PECMD_GetWindowTextAlloc(HWND param_1, uint64_t *param_2) { (void)param_1;(void)param_2; }
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

/* @0x1400e6d68 size=12 — %I64d 格式化到串槽 (decompiled.c @141333 直移) */
static void PECMD_FormatI64Dec(const uint16_t *param_1, uint64_t param_2)
{
  PECMD_SprintfRetEnd(param_1,param_2,(const uint16_t *)L"%I64d");
}
uint64_t PECMD_ResDecode(int64_t *ps, uint32_t flag) { (void)ps;(void)flag; return 0; }   /* arity 修正 0->2 */
HWND PECMD_GetOrCreateHiddenWnd(undefined8 *param_1, longlong param_2){
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
uint64_t PECMD_RichEditWordBreakProc(void) { return 0; }
/* @0x140101e70 size=103 — 按名探测文件存在并可写 (decompiled.c @158409 直移; 真签名 bool(LPCWSTR),
   与 core_b3_remaining.c:652 extern 一致; 经 FUN_140003864(CreateFileW 包装, def 本文件后部) 打开
   disposition=3(OPEN_EXISTING)/share=7 后 CloseHandle) */
bool FUN_140101e70(LPCWSTR param_1)
{
  bool bVar1;
  HANDLE local_res10[3];

  local_res10[0] = (HANDLE)(uintptr_t)0x0;
  FUN_140003864(local_res10,param_1,0,7,(LPSECURITY_ATTRIBUTES)0x0,3,0,(HANDLE)(uintptr_t)0x0);
  bVar1 = local_res10[0] != (HANDLE)(uintptr_t)0x0;
  if ((local_res10[0] != (HANDLE)(uintptr_t)0x0) &&
      (local_res10[0] != (HANDLE)(uintptr_t)0xffffffffffffffff)) {
    CloseHandle(local_res10[0]);
  }
  return bVar1;
}
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
uint64_t GetDC(void) { return 0; }
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
uint64_t KillTimer(void) { return 0; }
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
uint64_t OffsetRect(void) { return 0; }
uint64_t OpenClipboard(void) { return 0; }
uint64_t OpenFileMappingA(void) { return 0; }
uint64_t OpenFileMappingW(void) { return 0; }
uint64_t OpenProcess(DWORD access, BOOL inherit, DWORD pid) { (void)access;(void)inherit;(void)pid; return 0; }   /* arity 修正 (PECMD_EnumProcessInfo 体) */
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
uint64_t ReleaseDC(void) { return 0; }
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
uint64_t SetFilePointer(void *f, long dist, LONG *distHi, DWORD how) { (void)f;(void)dist;(void)distHi;(void)how; return 0; }   /* arity 修正 0->4 */
uint64_t SetFilePointerEx(void *h, longlong off, longlong *out, unsigned long method) { (void)h;(void)off;(void)out;(void)method; return 0; }   /* arity 修正 (PECMD_ScriptInitParse 体) */
uint64_t SetFocus(void) { return 0; }
uint64_t SetForegroundWindow(void *h) { (void)h; return 0; }
void SetLastError(DWORD e) { (void)e; }
uint64_t SetLayeredWindowAttributes(void *h, uint32_t cr, uint8_t a, uint32_t f) { (void)h;(void)cr;(void)a;(void)f; return 0; }   /* arity 修正 0->4 (PECMD_SetControlGeom 恢复体) */
uint64_t SetLocalTime(void) { return 0; }
int SetMenuItemBitmaps(void *m, unsigned int id, unsigned int f, void *b1, void *b2) { (void)m;(void)id;(void)f;(void)b1;(void)b2; return 0; }
uint64_t SetParent(void *h, void *p) { (void)h;(void)p; return 0; }   /* arity 修正 0->2 (PECMD_SetControlGeom 恢复体) */
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
int SetWindowTextW(void *hWnd, const void *lpString) { (void)hWnd;(void)lpString; return 0; }
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
uint64_t StrCmpNIA(const char *a, const char *b, int n) { (void)a;(void)b;(void)n; return 0; }   /* arity 修正 0->3 (PECMD_ExecCmdDispatch 恢复体 .cmd 预读) */
int StrCmpNIW(const WCHAR *a, const WCHAR *b, int n) { (void)a;(void)b;(void)n; return 0; }
void *OpenDesktopW(const WCHAR *n, uint64_t f, uint64_t acc, uint64_t flags) { (void)n;(void)f;(void)acc;(void)flags; return (void*)(uintptr_t)1; }
int SetThreadDesktop(void *d) { (void)d; return 1; }
int SwitchDesktop(void *d) { (void)d; return 1; }
int CloseDesktop(void *d) { (void)d; return 1; }
unsigned int DragQueryFileW(uint64_t a, uint32_t b, void *c, uint32_t d) { (void)a;(void)b;(void)c;(void)d; return 0; }
void FUN_140068984(long long *a, long long *b, char c) { (void)a;(void)b;(void)c; }

void FUN_140023640(uint64_t a, uint64_t b, int c) { (void)a;(void)b;(void)c; }
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
uint64_t thunk_FUN_140072814(void *a, longlong *b, const char *c) { (void)a;(void)b;(void)c; return 0; }
uint64_t u__26_INDATA_140121fe0(void) { return 0; }
uint64_t u_____D__140120a40(void) { return 0; }
uint64_t wsprintfA(char *out, const char *fmt, ...) { (void)out;(void)fmt; return 0; }   /* arity 修正 0->可变参 (PECMD_ExecCmdDispatch 恢复体) */
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
int FUN_14005B184(const char *a, const char *b, int n) { (void)a; (void)b; (void)n; return 0; }
void thunk_PECMD_GetNetworkConnectionName(const char *a, void *b, const char *c) { (void)a; (void)b; (void)c; }
uint16_t *PECMD_StrAlloc(uint16_t **ps, size_t count) { (void)ps; (void)count; return 0; }
void FUN_140060A74(uint8_t *buf, int len) { (void)buf; (void)len; }
int PECMD_RunBootScriptInFiber(uint16_t *cmdline) { (void)cmdline; return 0; }
int32_t PECMD_DecodeEncTextToUtf16(uint32_t spec, const uint8_t *src, int srclen,
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
uint64_t PECMD_ParseCommandPath(uint64_t a, uint64_t b, uint32_t *c, int64_t *d) { (void)a;(void)b;(void)c;(void)d; return 0; }   /* arity 修正 0->4 (core 调用方 + PECMD_ProcessScriptBlock 移入) */
uint64_t FUN_140061E98(void) { return 0; }
uint64_t FUN_1400688E0(void) { return 0; }
uint64_t FUN_14006A81C(void) { return 0; }
uint64_t PECMD_CopyPathToken(longlong a, longlong *b, longlong *c, longlong d) { (void)a;(void)b;(void)c;(void)d; return 0; }   /* arity 修正 0->4 (PECMD_DispatchExpressionBlock 恢复体 + core_b3_remaining 调用方) */
/* @0x1400e6d74 size=12 — %I64u 格式化到串槽 (decompiled.c @141345 直移; arity 修正 0->2, 与 core_b3_remaining:134/core_b3r_g3/h1/h4 extern 一致) */
static void PECMD_FormatU64Dec(const uint16_t *param_1, uint64_t param_2)
{
  PECMD_SprintfRetEnd(param_1,param_2,(const uint16_t *)L"%I64u");
}

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
uint64_t PECMD_SkipWCharUntil(void *pp, uint16_t ch) { (void)pp;(void)ch; return 0; }   /* arity 修正 0->2 (PECMD_ExecCmdDispatch 恢复体) */

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
  PECMD_ZeroLenBuf((uint64_t *)lp);
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
LRESULT PECMD_GetControlFont(int64_t param_1) { (void)param_1; return 0; }
int PECMD_UpdateWindowStyleBits(int64_t a, unsigned int b, uint64_t c) { (void)a;(void)b;(void)c; return 0; }
/* @0x1400e8644 size=— 临界区保护的窗口对象获取(直移) */
longlong PECMD_GetWinIdLocked(longlong *param_1)
{
  EnterCriticalSection((void *)&DAT_14013e190);
  longlong v = param_1[8];
  if (v == 0) {
    void *h = (void *)PECMD_GetOrCreateHiddenWnd(param_1,0);
    param_1[8] = (longlong)h;
    if ((uintptr_t)h != 0)
      PECMD_AppendFmtValue(param_1,*(uint64_t *)((char *)h + 8),(const void *)L"&&__WinID",(const void *)L"0x%I64X");
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
  PECMD_InitPtrTable((uint64_t *)(param_1 + 0x21));
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



uint64_t PECMD_CreateDateTimePickCtl(void) { return 0; }

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
undefined8 PECMD_PumpMessagesTimeout(int param_1, longlong param_2){
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
  extern int64_t PECMD_RunStartupScript(void *, uint64_t, const WCHAR *);
  SetProcessWorkingSetSize((void *)(uintptr_t)GetCurrentProcess(),(uint64_t)-1,(uint64_t)-1);
  PECMD_RunStartupScript(DAT_14013cf70,0,param_1);
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
  PECMD_FreeStrBuf((WCHAR **)&param_1);
}
void PECMD_DestroyStaticControl(uint64_t *param_1) { (void)param_1; }
void PECMD_ReleaseSlotObject(int64_t *param_1) { (void)param_1; }
int64_t PECMD_ControlWindowProc(uint64_t param_1, uint64_t param_2, uint64_t param_3, int64_t *param_4)
{ (void)param_1;(void)param_2;(void)param_3;(void)param_4; return 0; }



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
    PECMD_FreeStrBuf((void*)(param_1+0x25));
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
 *  - PECMD_EvalParenExprRounded/PECMD_ParseNumberToken (数字解析, decompiled.c 有体但本 tree 未移植) 新增桩,
 *    按 PECMD_ParseHexOrDec 桩惯例返回 1 视为"已解析" (不修改 *param_2)。
 */
typedef struct tagWINDOWINFO { DWORD cbSize; RECT rcWindow; RECT rcClient; DWORD dwStyle;
  DWORD dwExStyle; DWORD dwWindowStatus; UINT cxWindowBorders; UINT cyWindowBorders;
  WORD atomWindowType; WORD wCreatorVersion; } WINDOWINFO, *PWINDOWINFO;
int GetWindowInfo(void *hWnd, void *pwi) { (void)hWnd;(void)pwi; return 0; }   /* PECMD_SetControlGeom 恢复体新增桩 */



uint64_t PECMD_SetControlGeom(uintptr_t hwnd, uint16_t *s, int64_t p3, int64_t *p4, uintptr_t p5, uint32_t color, int64_t p7)
{
  (void)p3;
  HWND hw = (HWND)(uintptr_t)hwnd;              /* param_1 */
  uint16_t *local_178 = s;                      /* 串游标 (param_2) */
  uint64_t local_res10 = 0;                     /* 混合槽: '@' 时先为串指针, 后低32位坐标 */
  uint64_t local_138 = 0, local_128 = 0, local_130 = 0;
  uint64_t local_f8 = 0, local_110 = 0, local_108 = 0;
  uint64_t local_118 = 0, local_100 = 0, local_120 = 0;
  uint64_t local_res8 = 0, local_140 = 0;
  uint32_t local_f0 = 0;
  uint32_t local_148[2] = {0, 0};
  uint32_t local_a4 = 0;
  void    *local_d8 = NULL;
  void    *local_88 = NULL;
  RECT local_158, local_e8, local_168;
  POINT local_170;
  uint32_t uVar2, uVar3, uVar4, uVar6, uVar15, uVar16, uVar17, uVar18, dwFlags, crKey, Msg;
  uint64_t uVar12, uVar13;
  uint16_t uVar14;
  uint8_t bAlpha;
  int iVar7, iVar8, iVar9, iVar10, bVar1;
  void *pHVar11 = NULL, *hWnd = NULL;
  void **lParam;

  uVar6 = (*local_178 == 0x7e) ? 1 : 0;
  if (uVar6 != 0) local_178 = local_178 + 1;
  local_108 = uVar6;
  if (*local_178 == 0x40) {                     /* '@' — 父窗口表达式 @(...) */
    local_res10 = (uint64_t)(uintptr_t)(local_178 + 1);
    PECMD_EvalParenStripped((int64_t *)&local_res10, (uint64_t *)&local_d8);
    local_178 = (uint16_t *)(uintptr_t)local_res10;
  }
  if ((uVar6 != 0) && (p7 != 0)) {
    if (*(int *)(uintptr_t)(p7 + 8) == 9) local_108 = 0x10;
  }
  memset(&local_158, 0, sizeof(local_158));
  memset(&local_e8, 0, sizeof(local_e8));
  memset(&local_168, 0, sizeof(local_168));
  GetWindowRect(hw, &local_158);
  local_170.x = 0;
  local_170.y = 0;
  if (p5 == 0) {
    GetDlgCtrlID(hw);
    uVar6 = (uint32_t)GetWindowLongW(hw, -0x10);
    hWnd = NULL;
    if (((uVar6 >> 0x1e & 1) != 0) && ((pHVar11 = GetParent(hw)) != NULL)) {
      ClientToScreen(pHVar11, &local_170);      /* LAB_1400db776 */
      hWnd = pHVar11;
    }
  } else {
    pHVar11 = (void *)(uintptr_t)p5;
    ClientToScreen(pHVar11, &local_170);        /* LAB_1400db776 */
    hWnd = pHVar11;
  }
  GetClientRect(hw, &local_168);
  bVar1 = ((local_168.top - local_168.bottom) - local_158.top) + local_158.bottom < 10;
  uVar17 = (uint32_t)(~-(uint32_t)bVar1) & 6;
  uVar16 = (uint32_t)(local_158.right - local_158.left);
  uVar15 = (uint32_t)(local_158.bottom - local_158.top);
  local_f0 = (uint32_t)(~-(uint32_t)bVar1) & 0x20;
  uVar6 = (uint32_t)(local_158.left - local_170.x);
  local_130 = 0;
  local_res8 = uVar16;
  local_148[0] = uVar17;
  local_138 = uVar15;
  if (p5 == (uintptr_t)hwnd) {                  /* param_5 == param_1 */
    local_170.x = local_170.x - local_158.left;
    local_170.y = local_170.y - local_158.top;
    local_128 = (uint64_t)(uint32_t)local_158.left;
    local_res10 = (uint64_t)(uint32_t)local_158.top;
    uVar6 = (uint32_t)local_158.left;
  } else {
    local_128 = (uint64_t)uVar6;
    local_res10 = (uint64_t)(uint32_t)(local_158.top - local_170.y);
    if (hWnd != NULL) {
      GetWindowRect(hWnd, &local_e8);
      local_170.x = local_170.x - local_e8.left;
      local_170.y = local_170.y - local_e8.top;
    }
  }
  uVar18 = (uint32_t)local_res10;
  iVar10 = 0;
  bVar1 = 0;
  if (*local_178 == 0x3f) {                     /* '?' — 变量式位置设置 */
    uint16_t *puVar5 = local_178 + 1;
    if (local_178[1] == 0x40) {                 /* '@' */
      uVar6 = (uint32_t)local_158.left;
      puVar5 = local_178 + 2;
      local_res10 = (uint64_t)(uint32_t)local_158.top;
    }
    local_178 = puVar5;
    PECMD_SkipLeadingControlChars((long long *)&local_178);
    PECMD_AllocStrSlot((uint16_t **)&local_138);
    PECMD_AllocStrSlot((uint16_t **)&local_128);
    PECMD_AllocStrSlot((uint16_t **)&local_130);
    PECMD_AllocStrSlot((uint16_t **)&local_f8);
    PECMD_AllocStrSlot((uint16_t **)&local_110);
    PECMD_AllocStrSlot((uint16_t **)&local_108);
    PECMD_AllocStrSlot((uint16_t **)&local_118);
    PECMD_AllocStrSlot((uint16_t **)&local_res8);
    PECMD_AllocStrSlot((uint16_t **)&local_100);
    PECMD_AllocStrSlot((uint16_t **)&local_120);
    PECMD_ParseSkipSeparator((int64_t *)&local_178, (int64_t *)&local_138, 0x3a, 0);
    PECMD_ParseSkipSeparator((int64_t *)&local_178, (int64_t *)&local_128, 0x3a, 0);
    uVar14 = *local_178;
    if (uVar14 == 0x40) local_178 = local_178 + 1;   /* '@' */
    PECMD_ParseSkipSeparator((int64_t *)&local_178, (int64_t *)&local_130, 0x3a, 0);
    PECMD_ParseSkipSeparator((int64_t *)&local_178, (int64_t *)&local_f8,  0x3a, 0);
    PECMD_ParseSkipSeparator((int64_t *)&local_178, (int64_t *)&local_110, 0x3a, 0);
    PECMD_ParseSkipSeparator((int64_t *)&local_178, (int64_t *)&local_108, 0x3a, 0);
    if (*local_178 != 0) local_178 = local_178 + 1;
    PECMD_ParseSkipSeparator((int64_t *)&local_178, (int64_t *)&local_118, 0x3a, 0);
    PECMD_ParseSkipSeparator((int64_t *)&local_178, (int64_t *)&local_res8, 0x3a, 0);
    PECMD_ParseSkipSeparator((int64_t *)&local_178, (int64_t *)&local_100, 0x3a, 0);
    PECMD_ParseSkipSeparator((int64_t *)&local_178, (int64_t *)&local_120, 0x3a, 0);
    {
      WINDOWINFO wi;                            /* 原 local_d0; GetWindowInfo 结果未被再读 */
      memset(&wi, 0, sizeof(wi));
      wi.cbSize = 0x3c;
      local_a4 = 0;
      GetWindowInfo(hw, &wi);
    }
    /* 原 FUN_140102a90(&local_c8,0,0x34) memset — 目标 local_c8 仅写不读(死存储) → 省略 */
    if ((uVar14 != 0x40) && ((hWnd == NULL) || (p5 == (uintptr_t)hwnd))) {
      uVar16 = uVar16 + (uint32_t)((local_168.right - local_158.right) - local_168.left + local_158.left) + local_148[0];
      uVar15 = uVar15 + (uint32_t)((local_168.bottom - local_168.top) - local_158.bottom + local_158.top) + local_f0;
    }
    if (*(const uint16_t *)(uintptr_t)local_138 != 0) {
      PECMD_AppendLongDecimal(p4, (uint64_t)uVar6, (const uint16_t *)(uintptr_t)local_138);
    }
    if (*(const uint16_t *)(uintptr_t)local_128 != 0) {
      PECMD_AppendLongDecimal(p4, (uint64_t)(uint32_t)local_res10, (const uint16_t *)(uintptr_t)local_128);
    }
    if (*(const uint16_t *)(uintptr_t)local_130 != 0) {
      PECMD_AppendLongDecimal(p4, (uint64_t)uVar16, (const uint16_t *)(uintptr_t)local_130);
    }
    if (*(const uint16_t *)(uintptr_t)local_f8 != 0) {
      PECMD_AppendLongDecimal(p4, (uint64_t)uVar15, (const uint16_t *)(uintptr_t)local_f8);
    }
    if (*(const uint16_t *)(uintptr_t)local_110 != 0) {
      PECMD_AppendLongDecimal(p4, (uint64_t)(uint32_t)local_170.x, (const uint16_t *)(uintptr_t)local_110);
    }
    if (*(const uint16_t *)(uintptr_t)local_108 != 0) {
      PECMD_AppendLongDecimal(p4, (uint64_t)(uint32_t)local_170.y, (const uint16_t *)(uintptr_t)local_108);
    }
    if (*(const uint16_t *)(uintptr_t)local_118 != 0) {
      PECMD_AppendLongDecimal(p4, (uint64_t)local_a4, (const uint16_t *)(uintptr_t)local_118);
    }
    if (*(const uint16_t *)(uintptr_t)local_res8 != 0) {   /* 原 *(short*)CONCAT44(uStackX_c,local_res8) */
      pHVar11 = GetParent(hw);
      PECMD_VarSetUInt((void *)p4, (uint64_t)(uintptr_t)pHVar11, (const uint16_t *)(uintptr_t)local_res8);
    }
    if (*(const uint16_t *)(uintptr_t)local_100 != 0) {
      PECMD_VarSetUInt((void *)p4, (uint64_t)(uint32_t)(local_168.right - local_168.left), (const uint16_t *)(uintptr_t)local_100);
    }
    if (*(const uint16_t *)(uintptr_t)local_120 != 0) {
      PECMD_VarSetUInt((void *)p4, (uint64_t)(uint32_t)(local_168.bottom - local_168.top), (const uint16_t *)(uintptr_t)local_120);
    }
    PECMD_FreeStrBuf(&local_120);
    PECMD_FreeStrBuf(&local_100);
    PECMD_FreeStrBuf(&local_res8);
    PECMD_FreeStrBuf(&local_118);
    PECMD_FreeStrBuf(&local_108);
    PECMD_FreeStrBuf(&local_110);
    PECMD_FreeStrBuf(&local_f8);
    PECMD_FreeStrBuf(&local_130);
    PECMD_FreeStrBuf(&local_128);
    PECMD_FreeStrBuf(&local_138);
    return 0;
  }
  local_118 = (uint64_t)(uint32_t)-1;           /* low32 = -1 */
  local_100 = (uint64_t)(uint32_t)-1;
  local_120 = 0;
  local_148[0] = 0;
  local_f8 = 0;
  local_110 = 0;
  if (*local_178 == 0x23) {                     /* '#' — 仅移动标志 */
    local_178 = local_178 + 1;
    local_130 = 8;
  }
  iVar7 = PECMD_ParseNumberToken((int64_t *)&local_178, (int *)&local_128);
  uVar6 = (uint32_t)(0 < iVar7);
  local_res10 = (uint64_t)(int64_t)(int32_t)uVar18;
  uVar12 = PECMD_EvalParenExprRounded((int64_t *)&local_178, &local_res10);
  if ((int32_t)uVar12 != 0) uVar18 = (uint32_t)local_res10;
  local_res10 = (uint64_t)uVar18;               /* CONCAT44 高位残留不读 → 低32位近似 */
  if ((int32_t)uVar12 > 0) uVar6 = (uint32_t)((0 < iVar7) | 2);
  iVar7 = iVar10;
  if (*local_178 == 0x40) {                     /* '@' */
    local_178 = local_178 + 1;
    iVar7 = 0x40;
  }
  iVar8 = 0;
  {
    uint16_t *puVar5 = local_178;
    if (*local_178 == 0x3a) {                   /* ':' */
      iVar8 = 0x3a;
      puVar5 = local_178 + 1;
      if (local_178[1] == 0x40) {               /* '@' */
        iVar10 = 0x40;
        puVar5 = local_178 + 2;
      }
    }
    local_178 = puVar5;
  }
  if (*local_178 == 0x2a) {                     /* '*' */
    local_178 = local_178 + 1;
    iVar10 = 0x2a;
  }
  if (((iVar8 == 0) && (*local_178 != 0)) && ((*local_178 < 0x30) || (0x39 < *local_178))) {
    local_178 = local_178 + 1;
  }
  iVar8 = PECMD_ParseNumberToken((int64_t *)&local_178, (int *)&local_res8);
  if (0 < iVar8) uVar6 = uVar6 | 4;
  iVar8 = PECMD_ParseNumberToken((int64_t *)&local_178, (int *)&local_138);
  if (0 < iVar8) uVar6 = uVar6 | 8;
  local_140 = 0xfffffffff8000009ULL;
  uVar12 = PECMD_EvalParenExprRounded((int64_t *)&local_178, &local_140);
  iVar8 = -0x7fffff7;
  if ((int32_t)uVar12 != 0) iVar8 = (int32_t)local_140;
  for (;;) {                                    /* LAB_1400dbe4d / LAB_1400dbe52: 跳到 '$' 或数字/结束 */
    if (*local_178 == 0x24) {                   /* '$' */
      local_178 = local_178 + 1;
      bVar1 = 1;
      break;
    }
    if ((*local_178 == 0) || ((*local_178 > 0x2f) && (*local_178 < 0x3a))) break;
    local_178 = local_178 + 1;
  }
  local_140 = 0xffffffffffffff9cULL;
  uVar12 = PECMD_EvalParenExprRounded((int64_t *)&local_178, &local_140);
  uVar14 = 0;
  uVar13 = 0xffffff9cULL;
  if ((int32_t)uVar12 != 0) uVar13 = local_140 & 0xffffffff;
  local_140 = uVar13;
  if (!bVar1) {
    uVar14 = 0;
    iVar9 = PECMD_ConvertValue((int32_t)uVar13);
    local_140 = (uint64_t)(uint32_t)iVar9;      /* CONCAT44 高位为 0 → 低32位近似 */
  }
  if ((*local_178 != uVar14) && ((*local_178 < 0x30) || (0x39 < *local_178))) {
    local_178 = local_178 + 1;
  }
  PECMD_ParseNumSkipChar_01f8((int64_t *)&local_178, (int *)&local_118);
  PECMD_ParseNumberToken((int64_t *)&local_178, (int *)&local_100);
  PECMD_ParseNumberToken((int64_t *)&local_178, (int *)&local_120);
  PECMD_ParseNumberToken((int64_t *)&local_178, (int *)local_148);
  iVar9 = PECMD_ParseNumberToken((int64_t *)&local_178, (int *)&local_f8);
  if (0 < iVar9) uVar6 = uVar6 | 0x10;
  iVar9 = PECMD_ParseNumberToken((int64_t *)&local_178, (int *)&local_110);
  if (0 < iVar9) uVar6 = uVar6 | 0x20;
  uVar15 = (uint32_t)local_138;
  if ((((uVar6 & 0xc) != 0) && (iVar10 == 0)) && ((hWnd == NULL) || (p5 == (uintptr_t)hwnd))) {
    iVar10 = (local_158.right - local_168.right) - local_158.left + local_168.left;
    iVar9  = (local_168.top - local_168.bottom) - local_158.top + local_158.bottom;
    if (iVar7 == 0) {
      iVar10 = iVar10 - (int32_t)uVar17;
      iVar9  = iVar9  - (int32_t)local_f0;
    }
    if ((uVar6 & 4) != 0) local_res8 = local_res8 + (uint64_t)(int64_t)iVar10;
    if ((uVar6 & 8) != 0) uVar15 = (uint32_t)local_138 + (uint32_t)iVar9;
  }
  uVar16 = (uint32_t)local_res8;
  if ((uVar6 & 0x30) != 0) {
    if ((uVar6 & 0x10) != 0) uVar16 = (uint32_t)local_f8;
    if ((uVar6 & 0x20) != 0) uVar15 = (uint32_t)local_110;
    if ((hWnd == NULL) || (p5 == (uintptr_t)hwnd)) {
      if ((uVar6 & 0x10) != 0) uVar16 = uVar16 + (uint32_t)((local_158.right - local_168.right) - local_158.left + local_168.left);
      if ((uVar6 & 0x20) != 0) uVar15 = uVar15 + (uint32_t)((local_168.top - local_168.bottom) - local_158.top + local_158.bottom);
    }
  }
  local_res8 = 0;
  uVar14 = 0;
  if (*local_178 != 0) {
    if (((*local_178 < 0x30) || (0x39 < *local_178)) && (*local_178 != 0x28)) {   /* '(' */
      local_178 = local_178 + 1;
    }
    if ((*local_178 != 0) && ((*local_178 < 0x30) || (0x39 < *local_178)) && (*local_178 != 0x28)) {
      uVar14 = *local_178;
      local_178 = local_178 + 1;
    }
  }
  PECMD_ParseHexOrDecBool((long long *)&local_178, (int *)&local_res8);
  uVar17 = (uint32_t)local_128;
  if ((local_d8 != NULL) && ((uVar6 & 3) != 0)) {
    SendMessageW((void *)local_d8, 0x466, 5, (uint64_t)(uintptr_t)hw);   /* 原 lParam = param_1 八字节拷贝 */
    uVar6 = uVar6 & 0xfffffffc;
  }
  iVar7 = (int32_t)local_100;
  iVar10 = (int32_t)local_118;
  pHVar11 = NULL;
  uVar18 = (uint32_t)local_130;
  if ((uVar6 & 3) == 0) uVar18 = (uint32_t)local_130 | 2;
  if ((uVar6 & 0x3c) == 0) uVar18 = uVar18 | 1;
  iVar9 = 0;
  local_88 = NULL;
  if ((uint32_t)(iVar8 - 1) < 4) {
    iVar9 = iVar8;
    if (iVar8 == 1) local_88 = (void *)(uintptr_t)1;
    else if (iVar8 == 2) local_88 = (void *)(uintptr_t)0xfffffffffffffffeULL;
    else if ((iVar8 != 3) && (iVar8 == 4)) local_88 = (void *)(uintptr_t)-1;
  }
  if ((uVar6 != 0) || (iVar9 != 0)) {           /* LAB_1400dc156 */
    iVar8 = 1;
    pHVar11 = (void *)(uintptr_t)1;
  }
  if (iVar9 == 0) {
    iVar8 = (int32_t)(intptr_t)pHVar11;         /* 0 或 1 */
    uVar18 = uVar18 | 4;
  }
  if (((int32_t)local_108 > 1) && ((uVar6 & 0xf) != 0)) {
    uVar2 = *(uint32_t *)(uintptr_t)(p7 + 0x88);
    if ((uVar6 & 1) != 0) uVar2 = uVar17;
    iVar9 = *(int32_t *)(uintptr_t)(p7 + 0x8c);
    if ((uVar6 & 2) != 0) iVar9 = (int32_t)(uint32_t)local_res10;
    uVar3 = *(uint32_t *)(uintptr_t)(p7 + 0x90);
    if ((uVar6 & 4) != 0) uVar3 = uVar16;
    uVar4 = *(uint32_t *)(uintptr_t)(p7 + 0x94);
    if ((uVar6 & 8) != 0) uVar4 = uVar15;
    if (((uVar6 & 3) == 3) && (((int32_t)uVar2 < -0x6fffffff) || (-0x70000000 < iVar9))) {
      *(uint32_t *)(uintptr_t)(p7 + 0x88) = uVar2;
      *(int32_t  *)(uintptr_t)(p7 + 0x8c) = iVar9;
    }
    if ((((uint8_t)uVar6 & 0xc) == 0xc) && (((int32_t)uVar3 < 0) || (-1 < (int32_t)uVar4))) {
      *(uint32_t *)(uintptr_t)(p7 + 0x90) = uVar3;
      *(uint32_t *)(uintptr_t)(p7 + 0x94) = uVar4;
    }
  }
  if (p5 == 0) {
    if (iVar8 != 0) {
      SetWindowPos(hw, local_88, (int32_t)uVar17, (int32_t)(uint32_t)local_res10, (int32_t)uVar16, (int32_t)uVar15, (uint32_t)uVar18);
    }
    crKey = 0;
    if ((int32_t)color >= 0) crKey = color;
    dwFlags = ((int32_t)color >= 0) ? 1 : 0;
    if ((uint32_t)local_140 < 0x100) dwFlags = (((int32_t)color >= 0) ? 1 : 0) | 2;
    if (dwFlags != 0) {
      iVar8 = (int32_t)(0xffu - (uint32_t)local_140);
      if (iVar8 < 0) bAlpha = 0;
      else {
        bAlpha = (uint8_t)iVar8;
        if (0xff < iVar8) bAlpha = 0xff;
      }
      SetLayeredWindowAttributes(hw, crKey, bAlpha, dwFlags);
    }
    if (0 < iVar10) SetForegroundWindow(hw);
    if (0 < iVar7)  SetActiveWindow(hw);
    if ((int32_t)local_120 != 0) SetParent(hw, (void *)(uintptr_t)(int32_t)local_120);
    uVar6 = (uint32_t)local_res8;
    uVar12 = (uint64_t)(int64_t)(int32_t)local_res8;
    if (((uint32_t)local_res8 != 0) || (uVar14 != 0)) {
      uVar13 = GetWindowLongPtrW(hw, -0x14);
      if (uVar14 == 0x2d) uVar13 = uVar13 & (uint64_t)(int64_t)(int32_t)(~(int32_t)uVar6);   /* '-' 清位 */
      if (uVar14 == 0x2b) uVar12 = uVar13 | uVar12;                                           /* '+' 置位 */
      SetWindowLongPtrW(hw, -0x14, uVar12);
      SetWindowPos(hw, (void *)(uintptr_t)1, 0, 0, 0, 0, 0x23);
    }
    pHVar11 = (void *)(uintptr_t)(int32_t)local_148[0];
    if (local_148[0] == 0) return 0;
    lParam = NULL;
    Msg = 0x46f;
    p5 = (uintptr_t)hw;
  } else {
    lParam = &local_88;
    Msg = 0x464;
    pHVar11 = hw;
  }
  SendMessageW((void *)(uintptr_t)p5, Msg, (uint64_t)(uintptr_t)pHVar11, (uint64_t)(uintptr_t)lParam);
  return 0;
}
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
int (*DAT_14013c970)(uint32_t, uint32_t, char *, int);
/* --- r39 follow-up stubs (main-agent closure) --- */
uint64_t IsRectEmpty(void){ return 0; }
uint64_t CreateDCW(void){ return 0; }
/* @0x1400669c4 size=73 — 追加"名=<十进制长整数>"变量 (decompiled.c @63289 直移):
   Ghidra 丢可变参, 汇编证实为 wsprintfW(buf,"%ld",param_2) (AppendLongDecimal);
   %ld 取低 32 位有符号, 与 x64 ABI 一致. */
static void PECMD_AppendLongDecimal(int64_t *a, uint64_t b, const uint16_t *c)
{
  WCHAR local_78[56];

  wsprintfW(local_78,(const uint16_t *)L"%ld",(longlong)(int32_t)(uint32_t)b);
  FUN_1400629b8((void *)a,c,local_78);
}
uint64_t ReleaseSemaphore(void){ return 0; }
void *PECMD_InitSimpleWindow(void *a, uint64_t *b, uint64_t *c){ (void)a;(void)b;(void)c; return a; }
void *PECMD_InitListViewObject(void *a, uint64_t *b, uint64_t *c){ (void)a;(void)b;(void)c; return a; }


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
    PECMD_AllocWStringBuffer(local_38, 0x2800);
    pwVar5 = (WCHAR *)DAT_14011c638;
    if (local_res8[0] != 0) {
        pwVar5 = (WCHAR *)L"Reboot";
    }
    _snwprintf(local_38[0], 0x27ff, (const WCHAR *)L"PECMD安装驱动【%s】[%s][%s]", pwVar6, param_3, pwVar5);
    FUN_140025f10(param_1 + 8, local_38[0], DVar2, (pthreadmbcinfo)0x1100, (pthreadmbcinfo)0,
                  (long long *)0);
    PECMD_FreeStrBuf((WCHAR **)&local_38);
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
extern void PECMD_AppendKeyIfMissing(int64_t obj, LPCWSTR key, int len); /* @0x140019da8 (core_b1_remaining.c) */

uint64_t PECMD_DrvLoadInstall(int64_t a, void *b, void *c, int d)
{
  LPCWSTR param_2;
  LPCWSTR param_3;
  int param_4;
  int iVar1;
  DWORD DVar2;
  LPCWSTR pWVar3;
  uint16_t local_res8[4];
  FILETIME local_38;
  WCHAR *local_30;
  WCHAR *local_28[2];

  param_2 = (LPCWSTR)b;
  param_3 = (LPCWSTR)c;
  param_4 = d;
  local_30 = (WCHAR *)0;
  if (*(char *)(a + 0x157) == '\0') {
    PECMD_AllocStrSlot((uint16_t **)&local_30);
    PECMD_StrDupA(&local_30, (LPCWSTR)"--wd:*\"", (int64_t)-1, (int64_t)-1);
    PECMD_AppendWideStr(&local_30, param_2);
    PECMD_AppendWideStr(&local_30, (LPCWSTR)L"\" ");
    PECMD_AppendWideStr(&local_30, (LPCWSTR)L"!=drvload \"");
    PECMD_AppendWideStr(&local_30, param_2);
    PECMD_AppendWideStr(&local_30, (LPCWSTR)L"\"");
    iVar1 = lstrlenW(local_30);
    PECMD_AllocString(&local_30, (int64_t)iVar1 + 5);
    local_38.dwLowDateTime = 0;
    local_res8[0] = 0;
    PECMD_ExecCmdDispatch((uint64_t)(uintptr_t)&DAT_14013d130, (uint64_t)(uintptr_t)local_30,
                  (uint64_t)(uintptr_t)&DAT_14013d130, (uint64_t)(uintptr_t)local_res8, 0,
                  (uint64_t)(uintptr_t)&local_38, 0, 0);
    if ((*(long long *)(a + 0x110) != 0) && (local_38.dwLowDateTime == 0)) {
      PECMD_AppendKeyIfMissing(a, param_3, param_4);
    }
    PECMD_AllocWStringBuffer(local_28, 0x2800);
    pWVar3 = (WCHAR *)DAT_14011c638;
    if (param_3 != (LPCWSTR)0) {
      pWVar3 = param_3;
    }
    _snwprintf(local_28[0], 0x27ff, (const WCHAR *)L"DrvLoad安装驱动【(%s)?】[%s]", pWVar3,
               param_2);
    FUN_140025f10(a + 8, local_28[0], local_38.dwLowDateTime, (pthreadmbcinfo)0x1100,
                  (pthreadmbcinfo)0, (long long *)0);
    PECMD_FreeStrBuf((WCHAR **)&local_28);
    DVar2 = local_38.dwLowDateTime;
    PECMD_FreeStrBuf((WCHAR **)&local_30);
  }
  else {
    DVar2 = 0;
  }
  return (uint64_t)DVar2;
}
/* ========== PECMD_ToSysCopyFiles @ 14002b9ec size=1625 ==========
 * ToSys 复制到系统目录 worker (decompiled.c 直移):
 *   展开源路径 → 构造 INF / DRIVERS / SYSTEM32 三目标目录 (param_1+0x168 自定义基准或 %SystemRoot%),
 *   CopyFileW 主文件 (param_3&0x30000 时 PECMD_PatchInfDirectives),
 *   尾部 FindFirstFileW/FindNextFileW 枚举源目录所有文件, 按 .INF/.SYS 扩展名分流复制.
 * 改名 (tools/rename_map.json): PECMD_AllocWStringBuffer→PECMD_AllocWStringBuffer,
 *   FUN_14006459c→PECMD_ExpandDrivePath, FUN_14001d78c→PECMD_MemMoveForward,
 *   FUN_140021144→PECMD_PatchInfDirectives, FUN_140101db8→PECMD_FindFirstFileW,
 *   FUN_140103020→PECMD_WideStrLen.
 * 保持原名: FUN_140025f10 / PECMD_FreeStrBuf / FUN_140102a90 (link_stubs 桩).
 * 取舍: DAT_140120dd8 (源 "*" 通配串, 8 字节) 按 core_b2e.c 既有约定以裸地址引用;
 *       _wcsicmp 自补 2 参 extern — 真实体 @0x140103244 现为 0 参 no-op 桩, 链接取其符号.
 */
extern void    PECMD_FindFirstFileW(HANDLE *ph, LPCWSTR path, WIN32_FIND_DATAW *fd);  /* @0x140101db8 (core_b9_remaining.c) */
extern void    PECMD_PatchInfDirectives(LPCWSTR path);                                /* @0x140021144 (core_b2d.c) */
extern int     _wcsicmp(const WCHAR *a, const WCHAR *b);                              /* @0x140103244 (core_b9_remaining.c 桩) */

DWORD PECMD_ToSysCopyFiles(longlong param_1, LPCWSTR param_2, uint param_3)
{
  LPCWSTR lpExistingFileName;
  LPCWSTR lpDst;
  undefined1 *puVar1;
  DWORD DVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  int iVar6;
  BOOL BVar7;
  LPCWSTR pWVar8;
  int iVar9;
  longlong lVar10;
  WCHAR *local_368;
  HANDLE local_360;
  LPWSTR local_358;
  longlong local_350;
  WCHAR *local_348;
  int local_340;
  WCHAR *local_338;
  LPWSTR local_330;
  undefined2 *local_328;
  WCHAR local_320 [20];
  WCHAR local_2f8 [24];
  WCHAR local_2c8 [32];
  WIN32_FIND_DATAW local_288;

  PECMD_AllocWStringBuffer((WCHAR **)&local_330, 0x19c8);
  local_358 = (LPWSTR)0;
  PECMD_ExpandDrivePath(param_2, 0x527, local_330, (longlong *)&local_358);
  if (local_358 == (LPCWSTR)0) {
    DVar2 = GetLastError();
    if (DVar2 == 0) {
      DVar2 = 1;
    }
  }
  else {
    lpExistingFileName = local_330 + 0x528;
    lVar10 = (longlong)(int)((longlong)local_358 - (longlong)local_330 >> 1) * 2;
    PECMD_MemMoveForward((undefined1 *)lpExistingFileName,(undefined1 *)local_330,(int)lVar10);
    local_328 = (undefined2 *)(lVar10 + (longlong)lpExistingFileName);
    *local_328 = 0;
    lpDst = local_330 + 0xa50;
    if (*(LPCWSTR *)(param_1 + 0x168) == (LPCWSTR)0) {
      memcpy(local_320,(const WCHAR *)L"%SystemRoot%\\INF\\",0x24);
      ExpandEnvironmentStringsW(local_320,(LPWSTR)lpDst,0x104);
      memcpy(local_2c8,(const WCHAR *)L"%SystemRoot%\\SYSTEM32\\DRIVERS\\",0x3e);
      ExpandEnvironmentStringsW(local_2c8,(LPWSTR)(local_330 + 0xf78),0x104);
      memcpy(local_2f8,(const WCHAR *)L"%SystemRoot%\\SYSTEM32\\",0x2e);
      ExpandEnvironmentStringsW(local_2f8,(LPWSTR)(local_330 + 0x14a0),0x104);
    }
    else {
      iVar3 = lstrlenW(*(LPCWSTR *)(param_1 + 0x168));
      puVar1 = *(undefined1 **)(param_1 + 0x168);
      if ((*(short *)(puVar1 + (longlong)iVar3 * 2 + -2) == 0x5c) ||
         (*(short *)(puVar1 + (longlong)iVar3 * 2 + -2) == 0x2f)) {
        iVar3 = iVar3 + -1;
      }
      lVar10 = (longlong)iVar3 * 2;
      PECMD_MemMoveForward((undefined1 *)lpDst,puVar1,(int)lVar10);
      PECMD_MemMoveForward((undefined1 *)(lVar10 + (longlong)lpDst),(undefined1 *)L"\\INF\\",0xc);
      PECMD_MemMoveForward((undefined1 *)(local_330 + 0xf78),puVar1,(int)lVar10);
      PECMD_MemMoveForward((undefined1 *)(lVar10 + 0xa50 + (longlong)lpDst),
                           (undefined1 *)L"\\SYSTEM32\\DRIVERS\\",0x26);
      pWVar8 = local_330 + 0x14a0;
      PECMD_MemMoveForward((undefined1 *)pWVar8,puVar1,(int)lVar10);
      PECMD_MemMoveForward((undefined1 *)((longlong)pWVar8 + lVar10),(undefined1 *)L"\\SYSTEM32\\",0x16);
    }
    iVar3 = lstrlenW(lpDst);
    local_340 = lstrlenW(local_330 + 0xf78);
    iVar4 = lstrlenW(local_330 + 0x14a0);
    lstrcpyW((WCHAR *)(lpDst + iVar3),local_358);
    PECMD_AllocWStringBuffer((WCHAR **)&local_368,0x2800);
    _snwprintf(local_368,0x27ff,(const WCHAR *)L"ToSys:Copy <%s>",param_2);
    lVar10 = param_1 + 8;
    local_350 = lVar10;
    FUN_140025f10(lVar10,local_368,0,(pthreadmbcinfo)&DAT_00000011,(pthreadmbcinfo)0x0,
                  (long long *)0x0);
    PECMD_FreeStrBuf((WCHAR **)&local_368);
    CopyFileW((uint64_t)param_2,(uint64_t)lpDst,0);
    GetLastError();
    if ((param_3 & 0x30000) != 0) {
      PECMD_AllocWStringBuffer((WCHAR **)&local_368,0x2800);
      _snwprintf(local_368,0x27ff,(const WCHAR *)L"ToSys:TreatINF <%s>",lpDst);
      FUN_140025f10(lVar10,local_368,0,(pthreadmbcinfo)&DAT_00000011,(pthreadmbcinfo)0x0,
                    (long long *)0x0);
      PECMD_FreeStrBuf((WCHAR **)&local_368);
      PECMD_PatchInfDirectives(lpDst);
    }
    PECMD_AllocWStringBuffer((WCHAR **)&local_368,0x2800);
    _snwprintf(local_368,0x27ff,(const WCHAR *)L"ToSys:END <%s>",param_2);
    FUN_140025f10(lVar10,local_368,0,(pthreadmbcinfo)&DAT_00000011,(pthreadmbcinfo)0x0,
                  (long long *)0x0);
    PECMD_FreeStrBuf((WCHAR **)&local_368);
    PECMD_MemMoveForward((undefined1 *)local_358,(undefined1 *)(uintptr_t)0x140120dd8,8);
    local_288.dwFileAttributes = 0;
    FUN_140102a90((ulonglong *)&local_288.ftCreationTime,0,0x24c);
    local_360 = (HANDLE)0x0;
    PECMD_FindFirstFileW(&local_360,local_330,&local_288);
    if (local_360 != (HANDLE)0x0) {
      do {
        if (((local_288.dwFileAttributes & 0x10) == 0) &&
           ((local_288.cFileName[0] != L'.' ||
            ((local_288.cFileName[1] != L'\0' &&
             ((local_288.cFileName[1] != L'.' || (local_288.cFileName[2] != L'\0')))))))) {
          iVar5 = lstrlenW(local_288.cFileName);
          PECMD_MemMoveForward((undefined1 *)local_328,(undefined1 *)local_288.cFileName,(iVar5 + 1) * 2);
          lVar10 = PECMD_WideStrLen(local_288.cFileName);
          iVar5 = (int)lVar10;
          if ((int)lVar10 < 4) {
            iVar5 = 4;
          }
          iVar6 = _wcsicmp(local_288.cFileName + (longlong)iVar5 + -4,(const WCHAR *)L".INF");
          pWVar8 = local_330 + 0x14a0;
          iVar9 = iVar4;
          if (iVar6 == 0) {
            pWVar8 = lpDst;
            iVar9 = iVar3;
          }
          iVar5 = _wcsicmp(local_288.cFileName + (longlong)iVar5 + -4,(const WCHAR *)L".SYS");
          if (iVar5 == 0) {
            pWVar8 = local_330 + 0xf78;
            iVar9 = local_340;
          }
          lstrcpyW((WCHAR *)(pWVar8 + iVar9),local_288.cFileName);
          PECMD_AllocWStringBuffer((WCHAR **)&local_368,0x2800);
          _snwprintf(local_368,0x27ff,(const WCHAR *)L"ToSys:Copy <%s>",lpExistingFileName);
          FUN_140025f10(local_350,local_368,0,(pthreadmbcinfo)&DAT_00000011,(pthreadmbcinfo)0x0,
                        (long long *)0x0);
          PECMD_FreeStrBuf((WCHAR **)&local_368);
          CopyFileW((uint64_t)lpExistingFileName,(uint64_t)pWVar8,0);
          lVar10 = local_350;
          if ((lpDst == pWVar8) && ((param_3 & 0x30000) != 0)) {
            PECMD_AllocWStringBuffer((WCHAR **)&local_338,0x2800);
            _snwprintf(local_338,0x27ff,(const WCHAR *)L"ToSys:TreatINF <%s>",lpDst);
            lVar10 = local_350;
            FUN_140025f10(local_350,local_338,0,(pthreadmbcinfo)&DAT_00000011,(pthreadmbcinfo)0x0,
                          (long long *)0x0);
            PECMD_FreeStrBuf((WCHAR **)&local_338);
            PECMD_PatchInfDirectives(lpDst);
          }
          PECMD_AllocWStringBuffer((WCHAR **)&local_348,0x2800);
          _snwprintf(local_348,0x27ff,(const WCHAR *)L"ToSys:End <%s>",lpExistingFileName);
          FUN_140025f10(lVar10,local_348,0,(pthreadmbcinfo)&DAT_00000011,(pthreadmbcinfo)0x0,
                        (long long *)0x0);
          PECMD_FreeStrBuf((WCHAR **)&local_348);
          local_288.cFileName[2] = L'\0';
        }
        BVar7 = FindNextFileW(local_360,&local_288);
      } while (BVar7 != 0);
      if ((local_360 != (HANDLE)0x0) && (local_360 != (HANDLE)(uintptr_t)-1)) {
        FindClose(local_360);
      }
    }
    local_360 = (HANDLE)0x0;
    DVar2 = 0;
  }
  PECMD_FreeStrBuf((WCHAR **)&local_330);
  return DVar2;
}
uint8_t DAT_00000011;
uint64_t SetupDiSetDeviceInstallParamsW(void){ return 0; }
uint64_t SetupDiBuildDriverInfoList(void){ return 0; }
uint64_t SetupDiEnumDriverInfoW(void){ return 0; }
uint64_t SetupDiGetDriverInfoDetailW(void){ return 0; }
uint64_t SetupDiClassNameFromGuidW(void){ return 0; }
uint64_t GetShortPathNameW(LPCWSTR path, WCHAR *buf, DWORD len) { (void)path;(void)buf;(void)len; return 0; }   /* arity 修正 0->3 (PECMD_ExecCmdDispatch 恢复体) */
uint64_t CreateJobObjectW(LPSECURITY_ATTRIBUTES sa, LPCWSTR name) { (void)sa;(void)name; return 0; }   /* arity 修正 0->2 (PECMD_ExecCmdDispatch 恢复体) */
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
uint64_t WaitForInputIdle(HANDLE proc, DWORD ms) { (void)proc;(void)ms; return 0; }   /* arity 修正 0->2 (PECMD_ExecCmdDispatch 恢复体) */

/* ---- B3 还原批次 (core_b3r_*.c) 依赖补充 ---- */
uint64_t PECMD_ManualMapPeImage(int *a, int16_t *b, uint64_t c){ (void)a;(void)b; return c; }
/* ---- P0 补缺失符号桩 ---- */
/* ---- 批1-01ed5c 依赖桩 ---- */
uint64_t FUN_14001ebdc(void){ return 0; }
/* @0x140063694 size=139 — 分配宽串槽 (PECMD_AllocWStringBuffer, decompiled.c @60848 直移):
   HeapAlloc(len*2+10) 失败时经 FUN_1400630d0(2)(内存告警) 重试; 头 8B=容量 len*2+2,
   随后 4B=0xaa55 魔数, 数据区首字符置 NUL; 槽[0]=数据指针(头+8). 负长直接置空槽. */
static void PECMD_AllocWStringBuffer(void *p, long long param_2)
{
  longlong *plVar1;
  int iVar2;
  longlong *plVar3;
  undefined8 *param_1 = (undefined8 *)p;

  *param_1 = 0;
  if (-1 < param_2) {
    do {
      plVar3 = (longlong *)(uintptr_t)HeapAlloc(DAT_14013d328,0,param_2 * 2 + 10);
      if (plVar3 != (longlong *)0x0) break;
      iVar2 = (int)FUN_1400630d0(2);
    } while (iVar2 == 4);
    plVar1 = plVar3 + 1;
    *(uint32_t *)((char *)plVar3 + 4) = 0xaa55;
    *plVar3 = param_2 * 2 + 2;
    *param_1 = (uintptr_t)plVar1;
    if (plVar1 != (longlong *)0x0) {
      *(uint16_t *)(uintptr_t)plVar1 = 0;
    }
  }
}
uint64_t FUN_140063344(void *p){ (void)p; return 0; }
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
uint64_t PECMD_UpdateWindowExStyle(void){ return 0; }

/* PECMD_* 人类可读别名 (对应 FUN_<addr> 槽位; 委托到真实业务体) */
/* 真实业务体原型 (定义于 core_string.c / core_var.c / core_sys.c) */
WCHAR *FUN_14006375C(WCHAR **ps, LPCWSTR src);          /* @0x14006375c 字符串追加 */
void FUN_1400702B0(WCHAR **ps, LPCWSTR src);            /* @0x1400702b0 字符串赋值 */
static void PECMD_FreeStrBuf(void *ps);
WCHAR **FUN_14005B154(WCHAR **pp);                      /* @0x14005b154 跳过空白 */
void FUN_1400629B8(void *script, LPCWSTR key, LPCWSTR value); /* @0x1400629b8 设置变量 */
DWORD FUN_14005C394(HKEY root, LPCWSTR subkey, HKEY *out, REGSAM access, uint32_t opt); /* @0x14005c394 注册表打开 */
DWORD FUN_14005C4E0(HKEY root, LPCWSTR subkey, LPCWSTR name, DWORD *type, BYTE *data, DWORD *size); /* @0x14005c4e0 注册表查询 */

uint16_t *PECMD_StrCat(uint16_t **ps, const uint16_t *src){ return FUN_14006375C(ps, src); }
void PECMD_StrAssign(uint16_t **ps, const uint16_t *src){ FUN_1400702B0(ps, src); }
void PECMD_StrFree(uint16_t **ps){ PECMD_FreeStrBuf(ps); }
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
   依赖: FUN_14006b8fc (控件尺寸计算), PECMD_InitButtonState (按钮状态初始化, 内绕坐标/标志). */
long long FUN_14006b8fc(long long *param_1) { (void)param_1; return 1; } /* 控件尺寸计算 (no-op) */
/* --- PECMD_InitButtonState 直移还原 (decompiled.c @1400ef91c size=489, 按钮状态初始化/内绕坐标/标志) ---
 * 依赖: PECMD_InitImageHolder (core_b8d.c) / PECMD_CreateStaticWindow (core_b8g.c) / PECMD_* 串工具;
 * CONCAT44 双局部已按 rcPack[4] 数组展开 (同 core_b8k.c FUN_1400EF91C 既定模式). */
uint64_t *PECMD_InitImageHolder(uint64_t *param_1);   /* @0x1400efec8 图标对象初始化 (core_b8d.c) */
bool PECMD_CreateStaticWindow(int64_t *obj, LPCWSTR text, DWORD style, int *rect, HWND parent,
                   uint32_t id);              /* @0x1400efff8 创建 STATIC 子控件 (core_b8g.c) */
void PECMD_InitButtonState(long long param_1, uint param_2, uint64_t param_3)
{
    uint        uVar1;
    undefined8 *puVar2;
    longlong   *plVar3;
    LRESULT     LVar4;
    LPCWSTR     lpString;
    uint        uVar5;
    LPCWSTR     local_res8;
    tagRECT     local_38;
    int         rcPack[4];

    if (*(longlong *)(param_1 + 0x110) == 0) {
        local_38.left = 0;
        local_38.top = 0;
        local_38.right = 0;
        local_38.bottom = 0;
        GetClientRect(*(HWND *)(param_1 + 0x20), &local_38);
        if ((param_2 >> 0xb & 1) == 0) {
            local_38.top = local_38.top + 4;
            local_38.bottom = local_38.bottom + -4;
            local_38.left = local_38.left + 4;
            local_38.right = local_38.right + -4;
        }
        puVar2 = operator_new(0xe8);
        if (puVar2 == (undefined8 *)0x0) {
            plVar3 = (longlong *)0x0;
        }
        else {
            plVar3 = (longlong *)PECMD_InitImageHolder(puVar2);
        }
        *(longlong **)(param_1 + 0x110) = plVar3;
        InvalidateRect(*(HWND *)(param_1 + 0x20), (RECT *)0x0, 1);
        uVar5 = param_2 | 0x200;
        uVar1 = (uint)GetWindowLongW(*(HWND *)(param_1 + 0x20), -0x10);
        if ((uVar1 & 0x300) == 0x300) {
            uVar5 = param_2 | 0x201;
            *(undefined1 *)((char *)plVar3 + 0x60) = 0x11;
        }
        else if ((uVar1 >> 9 & 1) == 0) {
            *(undefined1 *)((char *)plVar3 + 0x60) = 0x10;
        }
        else {
            uVar5 = param_2 | 0x202;
            *(undefined1 *)((char *)plVar3 + 0x60) = 0x12;
        }
        PECMD_AllocStrSlot((uint16_t **)&local_res8);
        lpString = (LPCWSTR)L" ";
        if ((*(uint8_t *)(param_1 + 0xe8) & 8) != 0) {
            lpString = (LPCWSTR)(void *)DAT_14011c638;
        }
        PECMD_GetWindowTextAlloc(*(HWND *)(param_1 + 0x20), (uint64_t *)&local_res8);
        if ((param_3 & 1) == 0) {
            SetWindowTextW(*(HWND *)(param_1 + 0x20), lpString);
        }
        rcPack[0] = local_38.left;
        rcPack[1] = local_38.top;
        rcPack[2] = local_38.right;
        rcPack[3] = local_38.bottom;
        PECMD_CreateStaticWindow((int64_t *)plVar3, local_res8, uVar5 | 0x40000020, rcPack,
                      *(HWND *)(param_1 + 0x20), 0x7d2);
        if ((param_3 & 2) != 0) {
            PECMD_MemMoveForward((uint8_t *)((char *)plVar3 + 0x90), (uint8_t *)(param_1 + 0x90), 0x10);
        }
        SetWindowPos((HWND)plVar3[4], (HWND)0x0, 0, 0, 0, 0, 3);
        LVar4 = PECMD_GetControlFont(param_1);
        ((void (*)(int64_t *, LRESULT, int))(*(uint64_t *)(*plVar3 + 0x108)))(plVar3, LVar4, 1);
        InvalidateRect((HWND)plVar3[4], (RECT *)0x0, 1);
        PECMD_FreeStrBuf((WCHAR **)&local_res8);
    }
}
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
        PECMD_InitButtonState((long long)param_1, (uVar12 & 0x400) * 2, uVar11);
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
    PECMD_LoadOle32Apis();
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
  PECMD_LoadOle32Apis();
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
extern void *g_pNtQuerySystemInformation;          /* DAT_14013d040 */
extern void *g_pGetProcessMemoryInfo;              /* DAT_14013cd78 */
extern uint8_t g_u8CA49;                           /* DAT_14013ca49 */
extern int64_t g_QPFreq;                           /* DAT_14013cb80 */
extern double g_dbl1178;                           /* DAT_140121178 */
/* 内部 helper (core_b1_remaining.c / core_b8d.c / core_b9_remaining.c 定义) */
extern uint16_t *PECMD_FindLastPathSep(uint16_t *s);         /* @0x140017634 */
extern int8_t PECMD_GetProcessorCount(void);                 /* @0x1400066e4 */
extern bool PECMD_CmpStrN(LPCWSTR a, LPCWSTR b, int len, int case_sens);   /* @0x14001a25c */
extern bool PECMD_IsProcessUser(LPCWSTR user, DWORD pid, int case_sens);   /* @0x14001a488 */
extern void PECMD_TerminateProcessById(DWORD pid, UINT exitCode, uint64_t *out); /* @0x14001a510 */
extern uint16_t *PECMD_GetProcessModuleFile(DWORD pid, uint16_t *path);    /* @0x140006770 */
extern DWORD PECMD_ReadProcessCommandLine(DWORD pid, uint64_t *buf, int size); /* @0x1400098dc */
extern LPWSTR PECMD_GetProcessUserName(DWORD pid, LPWSTR buf);             /* @0x14001a284 */
extern void PECMD_StrAppendFormat(int64_t *ps, double a2, LPCWSTR fmt);            /* @0x1400e6cbc (core_b8d.c) */
/* Win32 桩 (无参桩配带参调用, 按惯例修 arity; 定义见本文件后部) */
extern uint64_t GetCurrentProcessId(void);
extern uint64_t GetProcessTimes(HANDLE h, FILETIME *a, FILETIME *b, FILETIME *c, FILETIME *d);
extern uint64_t OpenProcess(DWORD access, BOOL inherit, DWORD pid);
extern uint64_t PathMatchSpecW(const uint16_t *a, const uint16_t *b);
extern uint64_t Process32FirstW(HANDLE h, PROCESSENTRY32W *pe);
extern uint64_t Process32NextW(HANDLE h, PROCESSENTRY32W *pe);
extern uint64_t SHGetSpecialFolderPathW(HWND hwnd, WCHAR *path, int csidl, BOOL fCreate);

uint64_t PECMD_EnumProcessInfo(uint16_t *s, int a, int64_t *p, int b, unsigned long d){
    LPCWSTR filter = (LPCWSTR)s;      /* param_1 */
    uint32_t mode = (uint32_t)a;      /* param_2 */
    longlong *out = p;                /* param_3 */
    DWORD pid = (DWORD)b;             /* param_4 */
    DWORD parentPid = (DWORD)d;       /* param_5 */
    WCHAR *pWVar19 = (WCHAR *)0;
    LPCWSTR pWVar1;
    double dVar2;
    int iVar3;
    int iVar9;
    DWORD DVar4 = 0;
    DWORD DVar5;
    BOOL BVar10;
    HMODULE hModule;
    HANDLE hProcess;
    HANDLE pvVar6;
    longlong lVar16;
    longlong lVar17;
    LPCWSTR lpString1;
    WCHAR WVar18;
    uint64_t uVar11 = 0;
    uint64_t uVar12;
    uint64_t uVar20;
    uint32_t uVar8;
    uint32_t uVar13;
    DWORD DVar14;
    DWORD DVar15 = 0;
    DWORD *pDVar21;
    char local_res10;
    LPCWSTR local_438;
    int64_t local_440 = 0;
    WCHAR *local_430;
    uint64_t local_428 = 0;
    uint32_t local_420;
    uint32_t local_41c;
    DWORD *local_418 = (DWORD *)0;
    DWORD *local_3d8 = (DWORD *)0;
    UINT local_410;
    DWORD local_40c;
    HANDLE local_408;
    WCHAR *local_400;
    WCHAR *local_3f8;
    longlong local_3f0;
    longlong local_3e8;
    WCHAR *local_3e0;
    longlong local_3d0;
    uint64_t local_3c8 = 0x400;
    WCHAR *local_3c0 = (WCHAR *)0;
    longlong local_3b8;
    WCHAR *local_3b0;
    longlong local_3a8;
    WCHAR *local_3a0;
    WCHAR *local_398;
    FILETIME local_390;
    FILETIME local_388;
    FILETIME local_380;
    undefined4 local_378;
    undefined1 local_374[28];
    FILETIME local_358[2];
    undefined4 local_348;
    undefined4 local_344;
    ulonglong local_340[15];
    undefined1 local_2c8[8];
    ulonglong local_190[42];
    PROCESSENTRY32W pe32;

    if ((((mode == 0) && (out == (longlong *)0)) && (pid == 0)) && (parentPid == 0)) {
        filter = (LPCWSTR)PECMD_FindLastPathSep((uint16_t *)filter);
    }
    local_438 = filter;
    thunk_FUN_1400f429c((WCHAR **)&local_438, 0x7c);
    WVar18 = (WCHAR)(uintptr_t)pWVar19;
    if (*local_438 != WVar18) {
        *(WCHAR *)local_438 = WVar18;
        local_438 = local_438 + 1;
        if (*local_438 != WVar18) goto LAB_14002d791;
    }
    local_438 = (LPCWSTR)pWVar19;
LAB_14002d791:
    local_410 = (UINT)(uintptr_t)pWVar19;
    uVar8 = mode & 0x10000;
    uVar13 = mode & 0x40;
    iVar3 = (int)((mode & 0x80) == 0x80);   /* 原 CONCAT71 位提取简化 */
    local_41c = mode & 0x80000;
    local_420 = uVar13;
    local_40c = (DWORD)GetCurrentProcessId();
    local_3c8 = 0x400;
    local_428 = 0;
    local_3d8 = (DWORD *)0;
    local_418 = (DWORD *)0;
    if (iVar3 != 0) {
        local_410 = (UINT)(uintptr_t)out;
        out = (longlong *)0;
    }
    if ((uVar13 != 0) && (iVar3 != 0)) {
        PECMD_GrowByteBuffer((void **)&local_418, 0x1000);
        local_3d8 = local_418;
        out = (longlong *)0;
    }
    local_res10 = (char)(uVar13 != 0);
    if (((uVar13 != 0) && (iVar3 != 0)) && (parentPid != 0)) {
        *local_3d8 = parentPid;
        local_428 = 1;
        local_res10 = '\x02';
    }
    local_418 = local_3d8;
    if (g_pNtQuerySystemInformation == (void *)0) {
        hModule = (HMODULE)GetModuleHandleA("ntdll");
        g_pNtQuerySystemInformation = (void *)(uintptr_t)GetProcAddress(hModule, "NtQuerySystemInformation");
    }
    PECMD_EnableTokenPrivilege((LPCWSTR)L"SeDebugPrivilege", 2, 4);
    DVar4 = 0;
    PECMD_AllocWStringBuffer((WCHAR **)&local_3c0, 0x48f3);
    local_400 = local_3c0;
    local_3f8 = local_3c0 + 0x50;
    local_430 = local_3c0 + 0x82;
    local_3b0 = local_3c0 + 0x7dd;
    *local_3b0 = L'\t';
    local_398 = local_3c0 + 0x7de;
    *local_398 = L'\0';
    local_3e0 = local_3c0 + 0x8e3;
    *local_3e0 = L'\0';
    local_3c0[0x5d3] = L'\0';
    SHGetSpecialFolderPathW((HWND)0, local_3c0 + 0x5d3, 0x24, 0);
    iVar9 = lstrlenW(local_430 + 0x551);
    local_3a0 = local_430 + (longlong)iVar9 + 0x551;
    if (out != (longlong *)0) {
        local_378 = 0;
        FUN_140102a90((ulonglong *)local_374, 0, 0x1c);
        FUN_140102a90((ulonglong *)(local_2c8 + 4), 0, 0x134);
        FUN_140102a90(local_190, 0, 0x138);
        FUN_140102a90((ulonglong *)local_2c8, 0, 8);
        ((void (*)(int, void *, unsigned long))(uintptr_t)g_pNtQuerySystemInformation)(3, (void *)&local_378, 0x20);
        ((void (*)(int, void *, unsigned long))(uintptr_t)g_pNtQuerySystemInformation)(2, (void *)local_2c8, 0x134);
        PECMD_FormatI64Dec(local_400, *(uint64_t *)local_2c8);
        PECMD_FormatI64Dec(local_3f8, *(uint64_t *)(local_374 + 4));
    }
    DVar15 = 0;
    if (((mode & 1) == 0) && (*filter == L'\0')) {
        PECMD_AssignString(out, local_400);
        PECMD_AppendWideStr(out, (LPCWSTR)L"\t");
        PECMD_AppendWideStr(out, local_3f8);
        if (g_u8CA49 == '\0') {
            PECMD_GetProcessorCount();
        }
        wsprintfW((LPWSTR)&local_348, (const unsigned short *)L"\t%d\t", (uint64_t)(uint8_t)g_u8CA49);
        PECMD_AppendWideStr(out, (LPCWSTR)&local_348);
        PECMD_FormatI64Dec((LPCWSTR)&local_348, (uint64_t)g_QPFreq);
        PECMD_AppendWideStr(out, (LPCWSTR)&local_348);
        dVar2 = g_dbl1178 / (double)g_QPFreq;
        PECMD_AllocStrSlot((void *)&local_440);
        PECMD_StrAppendFormat((longlong *)&local_440, dVar2, (LPCWSTR)L"\t%lf");
        PECMD_AppendWideStr(out, (LPCWSTR)(uintptr_t)local_440);
        PECMD_FreeStrBuf((WCHAR **)&local_440);
    }
    local_440 = (int64_t)PECMD_WideStrLen(filter);
    local_408 = (HANDLE)(uintptr_t)CreateToolhelp32Snapshot(2, parentPid);
    if (local_408 == (HANDLE)(uintptr_t)0xffffffffffffffffULL) {
        local_408 = (HANDLE)0;
    }
    if (local_408 != (HANDLE)(uintptr_t)0xffffffffffffffffULL) {
        local_3e8 = (longlong)(int)(mode & 0x20);
        local_3f0 = (longlong)(int)(mode & 1);
        local_3a8 = (longlong)(int)(mode & 0x100);
        local_3b8 = (longlong)(int)(mode & 2);
        local_3d0 = (longlong)(int)(mode & 0x10);
        uVar20 = local_428;
        pDVar21 = local_3d8;
        do {
            lVar16 = local_3f0;
            pe32.dwSize = 0x238;   /* 原 local_2c8._0_4_ */
            local_428 = uVar20;
            iVar9 = (int)Process32FirstW(local_408, &pe32);
            pvVar6 = local_408;
            uVar11 = uVar20;
            lVar17 = local_3e8;
            DVar5 = DVar15;
            DVar14 = DVar4;
            while (local_3e8 = lVar17, DVar4 = DVar14, DVar15 = DVar5, iVar9 != 0) {
                uVar11 = 0;
                if (local_res10 == '\x02') {
                    uVar12 = uVar11;
                    if (uVar20 != 0) {
                        do {
                            if (pDVar21[uVar12] == pe32.th32ProcessID) goto LAB_14002e2de;
                            uVar12 = uVar12 + 1;
                        } while (uVar12 < uVar20);
                    }
                    if (uVar20 != 0) {
                        do {
                            if (pDVar21[uVar11] == pe32.th32ParentProcessID) goto LAB_14002dfb4;
                            uVar11 = uVar11 + 1;
                        } while (uVar11 < uVar20);
                    }
                }
                else if (((pid == 0) || (pid == pe32.th32ParentProcessID)) &&
                         ((parentPid == 0) || (parentPid == pe32.th32ProcessID))) {
                    if (lVar17 == 0) {
                        if (local_3a8 != 0) {
                            BVar10 = (BOOL)PathMatchSpecW(pe32.szExeFile, filter);
                            if (BVar10 != 0) goto LAB_14002dfb4;
                            goto LAB_14002dcd5;
                        }
                        iVar9 = (int)local_440;
                        if (local_3b8 != 0) {
                            if (lVar16 == 0) {
                                if ((int)local_440 == 0) {
                                    if (pe32.szExeFile[0] == L'\0') goto LAB_14002dcd5;
                                }
                                else {
                                    if (uVar8 == 0) {
                                        iVar9 = lstrcmpiW(pe32.szExeFile, filter);
                                    }
                                    else {
                                        iVar9 = (int)lstrcmpW(pe32.szExeFile, filter);
                                    }
                                    if (iVar9 == 0) goto LAB_14002dfb4;
                                }
                            }
                            else if (((int)local_440 == 0) ||
                                     (PECMD_CmpStrN(pe32.szExeFile, filter, (int)local_440, (int)uVar8))) {
                                goto LAB_14002dfb4;
                            }
                            goto LAB_14002e2de;
                        }
                        if ((lVar16 == 0) ||
                            (((int)local_440 != 0 &&
                              !PECMD_CmpStrN(pe32.szExeFile, filter, (int)local_440, (int)uVar8)))) {
                            if ((iVar9 != 0) &&
                                PECMD_CmpStrN(pe32.szExeFile, filter, iVar9, (int)uVar8) &&
                                (local_3f0 == 0)) {
                                if ((pe32.szExeFile[(int)local_440] == L'\0') ||
                                    (pe32.szExeFile[(int)local_440] == L'.'))
                                    goto LAB_14002dfb4;
                            }
                            goto LAB_14002e2de;
                        }
LAB_14002dfb4:
                        if ((((out == (longlong *)0) && (local_3d0 != 0)) &&
                             (local_40c == pe32.th32ProcessID)) ||
                            ((local_438 != (LPCWSTR)0) &&
                             (PECMD_IsProcessUser(local_438, pe32.th32ProcessID, (int)uVar8) == 0)))
                            goto LAB_14002e2de;
                        if (out == (longlong *)0) {
                            uVar11 = local_428;
                            DVar4 = pe32.th32ProcessID;
                            DVar15 = pe32.th32ProcessID;
                            if (iVar3 != 0 || local_420 != 0) {
                                if (pDVar21 != (DWORD *)0) {
                                    if (local_3c8 <= uVar20) {
                                        local_3c8 = local_3c8 + 0x400;
                                        PECMD_GrowByteBuffer((void **)&local_418, local_3c8 * 4);
                                    }
                                    local_418[uVar20] = pe32.th32ProcessID;
                                    uVar20 = uVar20 + 1;
                                    pDVar21 = local_418;
                                }
                                if (iVar3 != 0) {
                                    PECMD_TerminateProcessById(pe32.th32ProcessID, local_410, (uint64_t *)0);
                                }
                                DVar14 = DVar5 + 1;
                                goto LAB_14002e2de;
                            }
                            break;
                        }
                        DVar14 = DVar5 + 1;
                        lVar17 = local_3e8;
                    }
                    else {
LAB_14002dcd5:
                        if ((local_438 != (LPCWSTR)0) &&
                            (PECMD_IsProcessUser(local_438, pe32.th32ProcessID, (int)uVar8) == 0))
                            goto LAB_14002e2de;
                    }
                    lVar16 = 0;
                    if ((out != (longlong *)0) || (lVar17 != 0)) {
                        if ((out != (longlong *)0) &&
                            (hProcess = (HANDLE)(uintptr_t)OpenProcess(0x400, 0, pe32.th32ProcessID),
                             hProcess != (HANDLE)0)) {
                            PECMD_GetApiProcCached("GetProcessMemoryInfo", "PSAPI.DLL",
                                          (longlong *)&g_pGetProcessMemoryInfo, (longlong *)0);
                            local_348 = 0x48;
                            local_344 = 0;
                            FUN_140102a90(local_340, 0, 0x40);
                            if (g_pGetProcessMemoryInfo != (void *)0) {
                                ((void (*)(HANDLE, void *, DWORD))(uintptr_t)g_pGetProcessMemoryInfo)(
                                    hProcess, (void *)&local_348, 0x48);
                            }
                            BVar10 = (BOOL)GetProcessTimes(hProcess, local_358, &local_380, &local_390, &local_388);
                            if (BVar10 != 0) {
                                lVar16 = (longlong)(((uint64_t)local_390.dwLowDateTime |
                                                     ((uint64_t)local_390.dwHighDateTime << 32)) +
                                                    ((uint64_t)local_388.dwLowDateTime |
                                                     ((uint64_t)local_388.dwHighDateTime << 32)));
                            }
                            CloseHandle(hProcess);
                        }
                        lpString1 = local_430 + 0x656;
                        PECMD_GetProcessModuleFile(pe32.th32ProcessID, local_430 + 0x656);
                        if ((((local_430[0x656] == L'\\') && (local_430[0x657] == L'?')) &&
                             (local_430[0x658] == L'?')) &&
                            (((local_430[0x659] == L'\\' && (local_430[0x65a] != L'\0')) &&
                              (local_430[0x65b] == L':')))) {
                            lpString1 = local_430 + 0x65a;
                        }
                        else {
                            iVar9 = StrCmpNIW((const unsigned short *)L"\\SystemRoot\\", local_430 + 0x656, 0xc);
                            if (iVar9 == 0) {
                                lpString1 = local_430 + 0x551;
                                lstrcpyW(local_3a0, local_430 + 0x661);
                            }
                        }
                        if (local_3e8 == 0) {
LAB_14002e1cd:
                            if (local_41c != 0) {
                                PECMD_GetProcessUserName(pe32.th32ProcessID, local_398);
                            }
                            PECMD_ReadProcessCommandLine(pe32.th32ProcessID, (uint64_t *)local_3e0, 0x8000);
                            local_3e0[0x4000] = L'\0';
                            PECMD_FormatI64Dec(local_430, (uint64_t)lVar16);
                            pWVar1 = local_430 + 0x46;
                            wsprintfW((LPWSTR)pWVar1, (const unsigned short *)L"%lu\t%lu\t%lu\t%s\t%s%s\t");
                            if (*(uint16_t *)(uintptr_t)*out != 0) {
                                PECMD_AppendWideStr(out, (LPCWSTR)L"\r\n");
                            }
                            PECMD_AppendWideStr(out, pWVar1);
                            PECMD_AppendWideStr(out, lpString1);
                            PECMD_AppendWideStr(out, (LPCWSTR)L"\t");
                            PECMD_AppendWideStr(out, local_3e0);
                        }
                        else if (((uVar8 == 0) &&
                                  (iVar9 = lstrcmpiW(lpString1, filter), iVar9 == 0)) ||
                                 (iVar9 = (int)lstrcmpW(lpString1, filter), iVar9 == 0)) {
                            if (out != (longlong *)0) {
                                DVar14 = DVar14 + 1;
                                goto LAB_14002e1cd;
                            }
                            uVar11 = local_428;
                            DVar4 = pe32.th32ProcessID;
                            DVar15 = pe32.th32ProcessID;
                            if ((local_3d0 == 0) || (local_40c != pe32.th32ProcessID)) break;
                        }
                    }
                }
LAB_14002e2de:
                iVar9 = (int)Process32NextW(pvVar6, &pe32);
                uVar11 = local_428;
                lVar16 = local_3f0;
                lVar17 = local_3e8;
                DVar5 = DVar14;
            }
            local_res10 = '\x02';
        } while ((local_420 != 0) && (uVar11 < uVar20));
    }
    if ((local_408 != (HANDLE)0) && (local_408 != (HANDLE)(uintptr_t)0xffffffffffffffffULL)) {
        CloseHandle(local_408);
    }
    PECMD_FreeStrBuf((WCHAR **)&local_3c0);
    PECMD_FreeStrBuf((WCHAR **)&local_418);
    return DVar15;
}
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
/* @0x140008b2c size=— 按名停止并删除服务(直移) */
void PECMD_StopDeleteService(uint16_t *param_1)
{
  uint16_t w = *param_1;
  uint16_t *p = param_1;
  while ((w != 0 && ((w < 9 || 0xd < w) && (w != 0x20)))) { p++; w = *p; }
  int n = (int)((long long)p - (long long)param_1 >> 1);
  if (0x400 < n) n = 0x400;
  uint16_t name[1032];
  PECMD_MemMoveForward((uint8_t *)name,(uint8_t *)param_1,n * 2);
  name[n] = 0;
  for (; ((8 < *p && *p < 0xe) || *p == 0x20); p++) {}
  void *mgr = (void *)(uintptr_t)OpenSCManagerW(0,0,0xf003f);
  if ((uintptr_t)mgr != 0) {
    void *svc = (void *)(uintptr_t)OpenServiceW((void *)(uintptr_t)mgr,name,0x10020);
    if ((uintptr_t)svc != 0) {
      uint8_t st[0x38]; memset(st,0,0x38);
      ControlService((void *)(uintptr_t)svc,1,st);
      Sleep(200);
      DeleteService(svc);
      CloseServiceHandle(svc);
      Sleep(200);
      goto Lc33;
    }
  }
  GetLastError();
Lc33:
  CloseServiceHandle(mgr);
}
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
    PECMD_AllocStrSlot((uint16_t **)&local_res10);
    PECMD_ExpandCommandLine(&DAT_14013d130, local_res20, &local_res10, 0, 1);
    PECMD_StrBldCopyWide((long long *)&local_res18, local_res10);
    local_res8 = local_res18;
    PECMD_FreeStrBuf((WCHAR **)&local_res10);
    PECMD_FreeStrBuf((WCHAR **)&local_res20);
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
                PECMD_StopDeleteService((uint16_t *)local_res8);
                ExitProcess(0);
            }
            PECMD_FreeStrBuf((WCHAR **)&local_res18);
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
    u = DialogBoxParamW((void *)DAT_14013ca68,(void *)(uintptr_t)0x2727,0,PECMD_Dialog2727Proc,0);
    DAT_14013cfb0 = 0;
    u = u & 0xffffffff;
  }
  PECMD_FreeStrBuf((WCHAR **)&local_28);
  return u;
}
/* ---- PECMD_ScriptMainEntry 恢复体所需前置声明 (定义在文件后部/其它 core_*.c; 命名按 rename_map) ---- */
void     PECMD_InitDynamicImports(void);              /* @0x140017908 core_proc.c */
void     PECMD_DebugScriptString(undefined8 param_1, wchar_t *param_2, undefined8 param_3, undefined8 param_4);  /* def @8966 */
void     PECMD_InitRamdataRegistry(uint param_1);     /* @0x14001b888 def @9064 */
void     PECMD_RegisterHotkeyEntry(void);             /* @0x140022e94 def @9331 */
WCHAR   *PECMD_TokenizeQuotedField(longlong *param_1, longlong *param_2, longlong *param_3, WCHAR param_4, ushort param_5);  /* def @9115 */
extern void  PECMD_DebugOutput(uint64_t a, uint64_t b, uint64_t c, uint64_t d);  /* @0x140045868 core_b3d.c */
extern HANDLE PECMD_OpenFileHandle(HANDLE *out, LPCWSTR path, DWORD access, DWORD share,   /* def core_exec2.c / 同款 extern @7282 */
                                   LPSECURITY_ATTRIBUTES sa, DWORD disp, DWORD flags, HANDLE tmpl);
extern HHOOK g_hMsgHook;                              /* DAT_14013cf58 core_globals.c */
extern HWND  DAT_14013cf78;                           /* 主窗 HWND (def @9327) */
/* ========== PECMD_ScriptMainEntry @ 140045c90  size=2989 ==========
 * MAIN 主体 (decompiled.c @40510 忠实移植): 命令行前缀解析 (-user/-u/ **user/**u/**wait) →
 * 互斥体 Global\PECMD:main(_u) 排他 (ERROR_ALREADY_EXISTS 退出) → PELOGON 注册表初始化 →
 * 参数表初始化 (script+0x78/+0x80, atexit 清理注册) → 命令行脚本加载
 * (#!pecmd + --inline-main 检测 / 构造 " --exe:... " + "LOAD *sysinit ..." 命令串) →
 * GetMessageW 消息循环.
 * Ghidra pthreadlocinfo 游标按 WCHAR* 语义 (与 core_execscript.c 同款映射):
 *   p->refcount=p[0], &p->lc_codepage=p+2, &p->lc_time_cp+2=p+7, p->lc_category=(uint8_t*)p+0x10.
 * 内部调用按 rename_map: FUN_140017908→PECMD_InitDynamicImports, FUN_1400170b0→
 * PECMD_SkipLeadingControls, FUN_1400660ac→PECMD_MatchTokenAdvance, FUN_140026338→
 * PECMD_ScriptInitParse, FUN_140018c6c→PECMD_DebugScriptString, FUN_140018d8c→
 * PECMD_TlsLogWrite, FUN_140023544→PECMD_TruncateDebugLog, FUN_14001b888→
 * PECMD_InitRamdataRegistry, FUN_14002286c→PECMD_FixKnownDlls32, FUN_140017724→
 * PECMD_SwitchToDefaultDesktop, FUN_14005c5a0→PECMD_RegSetValueWithOpen, FUN_140063620→
 * PECMD_AllocStrSlot, PECMD_AllocWStringBuffer→PECMD_AllocWStringBuffer, FUN_1400545f8→
 * PECMD_TokenizeQuotedField, FUN_14005b1a8→PECMD_MatchPrefixN, FUN_14004c0bc→
 * PECMD_ProcessScriptBlock, FUN_1400702b0→PECMD_StrDupAssign, FUN_14005c788→
 * PECMD_AsciiPrefixICmp, FUN_140045868→PECMD_DebugOutput, PECMD_ExecCmdDispatch/LoadEnvi/
 * PECMD_ExpandSpecialDirs/PECMD_CreateDetachedThread/FUN_14001708c 保持原名或既定别名.
 * DAT 真值 (pe_data_extract ../PECMD.exe): 0x140122958 = " --exe:\"%s\"  \"%s\"  ",
 * 0x140122980 = "LOAD *sysinit %s" (均 UTF-16LE, 经 PECMD_CrtShim wsprintf 阴影).
 * 暂略死代码: memcpy(local_78, L"LOGS * D:\\DebugPeMain2.log", 0x36) / local_4e=0x55
 * (local_78/local_4e 无后续使用) 与 &DAT_0000000d 寄存器残留 (仅形参, 注出). */
static void CleanupMainArg_045c90(void) { /* 原 LAB_14011ab90 atexit 清理回调; 体未反编译 TODO(verify) */ }

uint64_t PECMD_ScriptMainEntry(uint64_t a, uint64_t b)   /* 签名保持桩 (arity 0->2); 真体直移 @0x140045c90 */
{
    WCHAR    *p;            /* local_res10 命令行游标 */
    WCHAR    *pSaved;       /* local_110 / local_118 快照 */
    WCHAR    *pPath;        /* local_f8 路径串槽 */
    WCHAR    *pContent;     /* local_108 文件内容缓冲 */
    WCHAR    *pContent2;    /* local_118 内容游标 */
    WCHAR    *pBuf;         /* local_100 大缓冲 */
    WCHAR    *pExe;         /* lpFilename 模块全路径 */
    WCHAR    *pExeName;     /* ptVar14 模块文件名部分 */
    WCHAR    *pTmp;         /* local_res18 串槽 (参数表) */
    WCHAR    *pCmd;         /* local_res18 直接执行串槽 */
    WCHAR     path[14];     /* local_6a (Ghidra 丢初始化 TODO(verify)) */
    WCHAR     pArg[3];      /* local_70 (Ghidra 丢初始化 TODO(verify)) */
    MSG       msg;          /* local_d0 */
    SECURITY_ATTRIBUTES sa; /* local_e8 */
    uint8_t   sd[0x28];     /* local_a0/local_9f 安全描述符缓冲 */
    HANDLE    hMutex;       /* pvVar9 (互斥体/文件句柄复用) */
    HANDLE    hFile;        /* local_f0 */
    HKEY      hKey;         /* local_res18 注册表键 */
    BYTE      bInMain;      /* local_res18.s.LowPart._0_1_ */
    LARGE_INTEGER liRead;   /* local_res18 ReadFile 计数 */
    DWORD     pid;          /* local_res20 */
    DWORD     dwVal;        /* local_res20 */
    LARGE_INTEGER *lpNumberOfBytesRead;
    const char *lpName;     /* 互斥体名 */
    const WCHAR *pwVar18;   /* 注册表值名 */
    char      cVar3;
    WCHAR     first;        /* uVar5 */
    WCHAR     sVar1;        /* sVar1 */
    WCHAR     c;            /* WVar12 */
    bool      bUser;        /* bVar2 */
    bool      bStar;        /* bVar19 */
    char      cVar17;       /* wait 模式 */
    uint64_t  uVar13;       /* flagCh: '*' 或 '-' (调试日志前) */
    uint64_t  uVar16;       /* 寄存器残留/返回值 */
    int       iVar7;
    int64_t  *pS;           /* param_1 视图 */

    cVar17 = '\0';
    p = (WCHAR *)(uintptr_t)b;
    pS = (int64_t *)(uintptr_t)a;
    PECMD_InitDynamicImports();
    DAT_14013c96a = 0;
    first = p[0];
    uVar13 = 0x2a;
    bStar = (first == 0x2a);
    while (first != 0 && ((first < 9 || 0xd < first) && first != 0x20)) {
        p++;
        first = p[0];
    }
    PECMD_SkipLeadingControls((uint64_t *)&p);
    uVar13 = uVar13 & 0xff;
    bUser = false;
    if ((int16_t)p[0] == 0x2d) {
        uVar13 = 0x2d;
    }
    for (;;) {
        while ((pSaved = p,
                (cVar3 = PECMD_MatchTokenAdvance("-user", (void *)&p, 5)) != '\0' ||
                (cVar3 = PECMD_MatchTokenAdvance("-u", (void *)&p, 2)) != '\0')) {
            bUser = true;
        }
        if ((int)(char)uVar13 != (uint)(uint16_t)pSaved[0] ||
            (int)(char)uVar13 != (uint)(uint16_t)pSaved[1]) break;
        pSaved = pSaved + 2;   /* &lc_codepage (偏移 4 字节 = 2 WCHAR) */
        cVar3 = PECMD_MatchTokenAdvance("user", (void *)&pSaved, 4);
        if (cVar3 == '\0' && (cVar3 = PECMD_MatchTokenAdvance("u", (void *)&pSaved, 1)) == '\0') {
            cVar3 = PECMD_MatchTokenAdvance("wait", (void *)&pSaved, 4);
            if (cVar3 == '\0') break;
            cVar17 = '\x01';
        }
        else {
            bUser = true;
        }
        p = pSaved;
    }
    pwVar18 = (const WCHAR *)(uintptr_t)PTR_u_CallBackhWnd_14011e668_2_14013a288;
    if (bUser) {
        pwVar18 = (const WCHAR *)L"UCallBackhWnd";
    }
    SetLastError(0);
    sd[0] = 0;
    FUN_140102a90((uint64_t *)(sd + 1), 0, 0x27);
    sa.lpSecurityDescriptor = sd;
    sa.nLength = 0x18;
    sa.bInheritHandle = 0;
    PECMD_InitNullDaclSD((uint64_t *)&sa.lpSecurityDescriptor);
    lpName = "Global\\PECMD:main";
    if (bUser) {
        lpName = "Global\\PECMD:main_u";
    }
    hMutex = (HANDLE)(uintptr_t)CreateMutexA(&sa, 1, lpName);
    if (hMutex != (HANDLE)0x0 && GetLastError() == 0xb7) {
        return 0xb7;   /* ERROR_ALREADY_EXISTS */
    }
    if (bUser) {
        DAT_140147000 = (uint32_t)L' ';
    }
    else {
        DAT_140147000 = (uint32_t)((p[0] != 0) + 0x10);
    }
    /* 反编译死代码省略: memcpy(local_78,L"LOGS * D:\\DebugPeMain2.log",0x36);
     * local_4e = 0x55; (local_78/local_4e 无后续使用) */
    if ((void *)DAT_14013cb18 == 0 && FUN_140101E70(path)) {
        PECMD_ScriptInitParse((uint64_t)(uintptr_t)&DAT_14013d130, (uint64_t)(uintptr_t)pArg,
                              (uint64_t)(uintptr_t)-1);
    }
    if (g_u8CCB1 != '\0') {
        uVar13 = (ulonglong)(uint)(int)DAT_140147000;
        PECMD_DebugScriptString((undefined8)(uintptr_t)&DAT_14013d130,
                                (wchar_t *)L"{MAIN [%s]} 0x%X\r\n",
                                (undefined8)(uintptr_t)b, (undefined8)uVar13);
        if (g_u8CCB1 != '\0') {
            PECMD_DebugScriptString((undefined8)(uintptr_t)&DAT_14013d130,
                                    (wchar_t *)L"MAIN: pecmd::RegDeleteKeyExW=0x%p\r\n",
                                    (undefined8)(uintptr_t)DAT_14013d408, (undefined8)uVar13);
        }
    }
    PECMD_TruncateDebugLog();
    if (bStar) {
        DAT_140147003 = 1;
    }
    if (!bUser) {
        if (p[0] == 0) goto L_after_init;
        PECMD_InitRamdataRegistry(3);
        uVar16 = 0;
        hKey = (HKEY)0;
        RegCreateKeyExW((HKEY)0xffffffff80000002, (const WCHAR *)L"SOFTWARE\\PELOGON", 0,
                        (LPWSTR)0, 0, 1, (LPSECURITY_ATTRIBUTES)0x0, (PHKEY)&hKey, (LPDWORD)0x0);
        if (hKey != (HKEY)0) {
            RegCloseKey(hKey);
        }
        if (g_u8CCB1 != '\0') {
            PECMD_TlsLogWrite((uint64_t)(uintptr_t)&DAT_14013d130,
                              (const uint16_t *)L"MAIN_DBG:%d\r\n", 0x2f1c, uVar16);
        }
        PECMD_FixKnownDlls32();
        if (g_u8CCB1 != '\0') {
            PECMD_TlsLogWrite((uint64_t)(uintptr_t)&DAT_14013d130,
                              (const uint16_t *)L"MAIN_DBG:%d\r\n", 0x2f1e, uVar16);
        }
        PECMD_SwitchToDefaultDesktop();
        if (g_u8CCB1 != '\0') {
            PECMD_TlsLogWrite((uint64_t)(uintptr_t)&DAT_14013d130,
                              (const uint16_t *)L"MAIN_DBG:%d\r\n", 0x2f20, uVar16);
        }
    }
    if (p[0] != 0) {
        PECMD_ExpandSpecialDirs((void *)(uintptr_t)a, (const uint16_t *)DAT_14011c638, 0, 0x24,
                      (const uint16_t *)0);
    }
L_after_init:
    sVar1 = p[0];
    if (sVar1 == L'*' && p[1] != L'#') {
        p = p + 1;
        PECMD_SkipLeadingControls((uint64_t *)&p);
    }
    LoadEnvi((const uint16_t *)DAT_14011c638, (const uint16_t *)DAT_14011c638);
    if (p[0] == 0) {
        g_flagD6f7 = 1;
    }
    PECMD_GetTaskbarCreatedMsg();
    PECMD_EnsureCallbackWindow();
    pid = (DWORD)GetCurrentProcessId();
    uVar16 = 4;
    PECMD_RegSetValueWithOpen((HKEY)0xffffffff80000002, (const uint16_t *)L"SOFTWARE\\PELOGON",
                              (const uint16_t *)L"MainPECMDPID", 4, (BYTE *)&pid, 4);
    if (g_u8CCB1 != '\0') {
        PECMD_TlsLogWrite((uint64_t)(uintptr_t)&DAT_14013d130,
                          (const uint16_t *)L"MAIN_DBG:%d\r\n", 0x2f2a, uVar16);
    }
    PECMD_GetOrCreateHiddenWnd((undefined8 *)&DAT_14013d130, 0);
    if (pS[0xf] == 0) {
        if ((g_initFlags & 1) == 0) {
            g_initFlags |= 1;
            g_pMainArgStr = 0;
            atexit(&CleanupMainArg_045c90);   /* 原注册 LAB_14011ab90 */
        }
        pS[0xf] = (int64_t)(uintptr_t)&g_pMainArgStr;
        PECMD_InitObfuscatedKeywords((void *)(uintptr_t)a, 0x10000);
        PECMD_AllocStrSlot(&pTmp);
        PECMD_AdoptRefCountedString((WCHAR **)&g_pMainArgStr, (LPCWSTR)(uintptr_t)pTmp);
        pTmp = 0;
        pS[0x10] = (int64_t)(uintptr_t)*(WCHAR **)g_pMainArgStr;
        FUN_140073CCC((void *)(uintptr_t)a, (LPCWSTR)(uintptr_t)b, 1);
        *(uint32_t *)((uint8_t *)pS + 0x8c) = 0;
        PECMD_FreeStrBuf(&pTmp);
    }
    if (g_u8CCB1 != '\0') {
        PECMD_TlsLogWrite((uint64_t)(uintptr_t)&DAT_14013d130,
                          (const uint16_t *)L"MAIN_DBG:%d\r\n", 0x2f36, uVar16);
    }
    if (!bUser) {
        bInMain = (BYTE)DAT_140147000;
        uVar16 = 3;
        PECMD_RegSetValueWithOpen((HKEY)0xffffffff80000002, (const uint16_t *)L"SOFTWARE\\PELOGON",
                                  (const uint16_t *)L"bInMain", 3, &bInMain, 1);
    }
    if (g_u8CCB1 != '\0') {
        PECMD_TlsLogWrite((uint64_t)(uintptr_t)&DAT_14013d130,
                          (const uint16_t *)L"MAIN_DBG:%d\r\n", 0x2f3a, uVar16);
    }
    if (p[0] != 0) {
        PECMD_InstallKeyboardHook();
    }
    uVar16 = 0xb;
    PECMD_RegSetValueWithOpen((HKEY)0xffffffff80000002, (const uint16_t *)L"SOFTWARE\\PELOGON",
                              pwVar18, 0xb, (BYTE *)&DAT_14013cf78, 8);
    if (sVar1 != 0 || bUser) {
        PECMD_LoadResourceLines();
    }
    PECMD_RegisterHotkeyEntry();
    PECMD_RelaunchLoadFirstUsb();
    if (p[0] != 0 || bUser) {
        if (g_u8CCB1 != '\0') {
            PECMD_TlsLogWrite((uint64_t)(uintptr_t)&DAT_14013d130,
                              (const uint16_t *)L"MAIN_DBG:%d\r\n", 0x2f45, uVar16);
        }
        PECMD_CreateDetachedThread((LPTHREAD_START_ROUTINE)PECMD_DebugOutput, (LPVOID)0x0, 0x80000, 0x10000,
                      (LPDWORD)&g_threadId, (LPSECURITY_ATTRIBUTES)0x0);
    }
    uVar16 = 0x2a;
    if (p[0] == 0x2a) {
        do {
            if (p[1] == 0x23) break;
            p = p + 1;
            PECMD_SkipLeadingControls((uint64_t *)&p);
        } while (p[0] == (WCHAR)uVar16);
    }
    if (p[0] != 0) {
        if (g_u8CCB1 != '\0') {
            PECMD_TlsLogWrite((uint64_t)(uintptr_t)&DAT_14013d130,
                              (const uint16_t *)L"MAIN_DBG:%d\r\n", 0x2f4d, uVar16);
        }
        iVar7 = lstrlenW((LPCWSTR)p);
        PECMD_AllocWStringBuffer((WCHAR **)&pBuf, (int64_t)(iVar7 + 0x5c8));
        pBuf[0] = 0;
        pExe = pBuf + 0x140;
        GetModuleFileNameW((HMODULE)0x0, pExe, 0x104);
        pSaved = StrRChrW((LPCWSTR)pExe, (LPCWSTR)0x0, L'\\');
        pExeName = pExe;
        if (pSaved != (LPWSTR)0x0) {
            pExeName = pSaved + 1;
        }
        iVar7 = PECMD_LineIsTeamExecLoad((LPCWSTR)p);
        if (iVar7 == 0) {
            /* ---- 文件模式: 读取内容, 检测 #!pecmd / --inline-main ---- */
            bStar = false;
            pSaved = p;
            PECMD_AllocStrSlot(&pPath);
            PECMD_TokenizeQuotedField((longlong *)(uintptr_t)a, (longlong *)&pSaved,
                                      (longlong *)&pPath, 0x1000, 0);
            PECMD_AllocWStringBuffer((WCHAR **)&pContent, 0x400);
            pContent[0] = 0;
            pContent2 = pContent;
            liRead.QuadPart = 0;
            hFile = (HANDLE)0x0;
            PECMD_OpenFileHandle(&hFile, pPath, 0x80000000, 7, (LPSECURITY_ATTRIBUTES)0x0, 3, 0,
                                 (HANDLE)0x0);
            hMutex = hFile;               /* pvVar9 = local_f0 */
            lpNumberOfBytesRead = &liRead;
            ReadFile(hFile, pContent, 0x7fe, (LPDWORD)&liRead.QuadPart, (LPOVERLAPPED)0x0);
            pContent2[(ulonglong)liRead.QuadPart >> 1 & 0x7fffffff] = 0;   /* 终止符 (lc_category 偏移运算) */
            if (pContent[0] == 0xFEFF) {
                pContent2 = pContent2 + 1;   /* 跳过 UTF-16 BOM */
            }
            iVar7 = StrCmpNIW((const WCHAR *)L"#!pecmd", (LPCWSTR)pContent2, 7);
            if (iVar7 == 0) {
                pContent2 = pContent2 + 7;   /* &lc_time_cp + 2 (偏移 0xC+2 字节 = 7 WCHAR) */
                do {
                    c = pContent2[0];
                    if (c == L'\0') goto L_close_file;
                    do {
                        /* 原 &DAT_0000000d 寄存器残留赋值 (形参, 无语义) */
                        lpNumberOfBytesRead = (LARGE_INTEGER *)(void *)(uintptr_t)0xd;
                        if ((8 < c && c < 0xe) || c == L' ') break;
                        pContent2++;
                        c = pContent2[0];
                    } while (c != L'\0');
                    for (; pContent2[0] == L' ' || pContent2[0] == L'\t'; pContent2++) {
                    }
                    if (pContent2[0] == L'\n' || pContent2[0] == L'\r') goto L_close_file;
                    uVar16 = (uint64_t)PECMD_MatchPrefixN((ushort *)L"--inline-main",
                                                          (undefined8 *)&pContent2, 0xd);
                } while ((int)uVar16 == 0);
                bStar = true;   /* 发现 --inline-main */
            }
        L_close_file:
            if (hFile != (HANDLE)0x0 && hFile != (HANDLE)(uintptr_t)0xffffffffffffffffULL) {
                CloseHandle(hFile);
            }
            PECMD_FreeStrBuf(&pContent);
            PECMD_FreeStrBuf(&pPath);
            if (bStar) {
                PECMD_CrtShim(pBuf + 900, (size_t)0x140122980 /* "LOAD *sysinit %s" (DAT 真值) */,
                              p, (void *)&lpNumberOfBytesRead->s);
                g_flagD032 = 1;
                PECMD_ProcessScriptBlock((LARGE_INTEGER){ .QuadPart = (long long)a },
                                         (LARGE_INTEGER){ .QuadPart = (long long)(uintptr_t)(pBuf + 900) },
                                         (longlong *)0x0, (longlong *)0x0, (pthreadmbcinfo)0x0);
            }
            else {
                PECMD_CrtShim(pBuf + 900, (size_t)0x140122958 /* " --exe:\"%s\"  \"%s\"  " (DAT 真值) */,
                              pExe, pExeName);
                iVar7 = lstrlenW(pBuf + 900);
                PECMD_CrtShim(pBuf + iVar7 + 900, (size_t)0x140122980, p, pExeName);
                if (pBuf[0] != 0) {   /* 反编译恒假分支, 原样保留 */
                    PECMD_InitRamdataRegistry(1);
                    dwVal = 1;
                    PECMD_RegSetValueWithOpen((HKEY)0xffffffff80000002,
                                              (const uint16_t *)L"SOFTWARE\\PELOGON\\RAMDATA",
                                              (const uint16_t *)L"SysStartuped", 4,
                                              (BYTE *)&dwVal, 4);
                }
                PECMD_ExecCmdDispatch((uint64_t)a, (uint64_t)(uintptr_t)(pBuf + 900), (uint64_t)a, 0,
                              (uint64_t)(int)cVar17, 0, 0, 0);
            }
        }
        else {
            /* ---- 直接命令模式 (LOAD 前缀检测) ---- */
            PECMD_StrDupAssign(&pCmd, p);
            uVar16 = (uint64_t)PECMD_AsciiPrefixICmp("LOAD", (const uint16_t *)p, 4);
            if ((char)uVar16 != '\0') {
                g_flagD032 = 1;
            }
            PECMD_ProcessScriptBlock((LARGE_INTEGER){ .QuadPart = (long long)a },
                                     (LARGE_INTEGER){ .QuadPart = (long long)(uintptr_t)pCmd },
                                     (longlong *)0x0, (longlong *)0x0, (pthreadmbcinfo)0x0);
            PECMD_FreeStrBuf(&pCmd);
        }
        PECMD_FreeStrBuf(&pBuf);
    }
    /* ---- 消息循环 ---- */
    while (GetMessageW(&msg, (HWND)0x0, 0, 0) != 0) {
        TranslateMessage(&msg);
        DispatchMessageW(&msg);
    }
    UnhookWindowsHookEx(g_hMsgHook);
    return (longlong)(int)msg.wParam;
}
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
/* ========== PECMD_IndataCopyDirs @0x14003c06c size=2427 ==========
 * 文件操作命令: 解析 -simpleprogress/-progress/-file/-forceq/-force/-su/-q/-delme/-rd/-delay
 * 选项, 拆分 ';' 分组的 "源>目标" 列表, 按 wFunc(2=拷贝/1=移动/3=删除/4=改名)
 * 调用 SHFileOperationW/MoveFile/DeleteFile/PECMD_RunCommand 等执行。
 * 移植自 decompiled.c PECMD_IndataCopyDirs (仅编辑 link_stubs.c; 内部调用按 rename_map 命名)。
 * signature: LARGE_INTEGER __fastcall PECMD_IndataCopyDirs(longlong *param_1, LARGE_INTEGER param_2, uint param_3)
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
extern void        PECMD_AllocString(WCHAR **ps, int64_t count);
extern void        PECMD_AppendParamToken(long long *list, LPCWSTR token, char filter);
extern int64_t     PECMD_DeleteDirectoryTree(LPCWSTR path, uint32_t flags);
extern int16_t    *PECMD_LastPathSeparator(int16_t *path);
extern HANDLE      PECMD_OpenFileHandle(HANDLE *out, LPCWSTR path, DWORD access, DWORD share,
                                        LPSECURITY_ATTRIBUTES sa, DWORD disp, DWORD flags, HANDLE tmpl);

LARGE_INTEGER PECMD_IndataCopyDirs(longlong *param_1,LARGE_INTEGER param_2,uint param_3)
{
  uint16_t uVar1;
  bool bVar2;
  bool bVar3;
  bool bVar4;
  bool bVar5;
  bool bVar6;
  char cVar7;
  uint8_t uVar8;
  int iVar9;
  uint uVar10;
  DWORD DVar11;
  BOOL BVar12;
  longlong lVar13;
  LARGE_INTEGER LVar14;
  short *psVar15;
  LARGE_INTEGER *pLVar16;
  LARGE_INTEGER LVar17;
  int iVar18;
  LARGE_INTEGER LVar19;
  uint16_t uVar20;
  LARGE_INTEGER LVar21;
  LARGE_INTEGER LVar22;
  WCHAR WVar24;
  uint8_t bVar25;
  LARGE_INTEGER local_res10;
  LARGE_INTEGER local_res20;
  char local_c5;
  LARGE_INTEGER local_c0;
  uint local_b8;
  _SHFILEOPSTRUCTW local_b0;
  WCHAR *local_78;
  LARGE_INTEGER local_70;
  longlong local_68;
  uint64_t local_60;
  LARGE_INTEGER local_58;
  longlong local_50;
  uint64_t local_48;
  LARGE_INTEGER LVar23;

  LVar23.QuadPart = 0;
  iVar18 = 0;
  LVar22.QuadPart = 0;
  uVar20 = 4;
  bVar25 = 0;
  local_res20.QuadPart = local_res20.QuadPart & 0xffffffffffffff00;
  local_c5 = '\0';
  bVar2 = false;
  bVar5 = false;
  bVar6 = false;
  bVar4 = false;
  local_res10 = param_2;
  while (uVar10 = param_3, *(uint16_t *)(uintptr_t)local_res10.QuadPart == 0x2d) {
    if (*(uint16_t *)(uintptr_t)(local_res10.QuadPart + 2) == 0x2d) {
      local_res10.QuadPart = local_res10.QuadPart + 2;
    }
    cVar7 = PECMD_MatchTokenAdvance("-",(void *)&local_res10.QuadPart,1);
    if (cVar7 != '\0') break;
    cVar7 = PECMD_MatchTokenAdvance("-simpleprogress",(void *)&local_res10.QuadPart,0xf);
    param_3 = uVar10;
    if (cVar7 == '\0') {
      cVar7 = PECMD_MatchTokenAdvance("-progress",(void *)&local_res10.QuadPart,9);
      if (cVar7 == '\0') {
        cVar7 = PECMD_MatchTokenAdvance("-file",(void *)&local_res10.QuadPart,5);
        if (cVar7 == '\0') {
          cVar7 = PECMD_MatchTokenAdvance("-m",(void *)&local_res10.QuadPart,2);
          if (cVar7 == '\0') {
            cVar7 = PECMD_MatchTokenAdvance("-forceq",(void *)&local_res10.QuadPart,7);
            param_3 = 3;
            if ((cVar7 == '\0') &&
               (cVar7 = PECMD_MatchTokenAdvance("-force",(void *)&local_res10.QuadPart,6), param_3 = 1, cVar7 == '\0')
               ) {
              cVar7 = PECMD_MatchTokenAdvance("-su",(void *)&local_res10.QuadPart,3);
              if (cVar7 == '\0') {
                cVar7 = PECMD_MatchTokenAdvance("-q",(void *)&local_res10.QuadPart,2);
                if (cVar7 == '\0') {
                  cVar7 = PECMD_MatchTokenAdvance("-delme",(void *)&local_res10.QuadPart,6);
                  if (cVar7 == '\0') {
                    cVar7 = PECMD_MatchTokenAdvance("-rd",(void *)&local_res10.QuadPart,3);
                    if (cVar7 == '\0') {
                      cVar7 = PECMD_MatchTokenAdvance("-delay",(void *)&local_res10.QuadPart,6);
                      if (cVar7 == '\0') {
                        uVar1 = *(uint16_t *)(uintptr_t)local_res10.QuadPart;
                        while ((uVar1 != 0 && (((uVar1 < 9 || (0xd < uVar1)) && (uVar1 != 0x20)))))
                        {
                          local_res10.QuadPart = local_res10.QuadPart + 2;
                          uVar1 = *(uint16_t *)(uintptr_t)local_res10.QuadPart;
                        }
                        PECMD_SkipLeadingControls((uint64_t *)&local_res10.QuadPart);
                        param_3 = uVar10;
                      }
                      else {
                        bVar4 = true;
                        param_3 = uVar10;
                      }
                    }
                    else {
                      bVar25 = 2;
                      bVar5 = true;
                      param_3 = uVar10;
                    }
                  }
                  else {
                    bVar2 = true;
                    param_3 = uVar10;
                  }
                }
                else {
                  local_c5 = '\x10';
                  param_3 = uVar10;
                }
              }
              else {
                bVar6 = true;
                bVar25 = bVar25 | 0x10;
                param_3 = uVar10;
              }
            }
            else {
              *(uint8_t *)&local_res20.s.LowPart = 1;
            }
          }
          else {
            bVar25 = 1;
          }
        }
        else {
          uVar20 = 0;
        }
      }
      else {
        uVar20 = 0x100;
      }
    }
  }
  PECMD_SkipLeadingControls((uint64_t *)&local_res10.QuadPart);
  if (bVar2) {
    PECMD_StrDupAssign((uint16_t **)&local_res20.QuadPart,(const uint16_t *)L"#23:INDATA ");
    PECMD_AppendWideStr(&local_res20.QuadPart,(LPCWSTR)(uintptr_t)local_res10.QuadPart);
    lVar13 = PECMD_RunCommand((void *)&DAT_14013d130,(WCHAR *)(uintptr_t)local_res20.QuadPart);
    PECMD_FreeStrBuf(&local_res20.QuadPart);
    return (LARGE_INTEGER){ .QuadPart = (long long)lVar13 };
  }
  if (*(int16_t *)(uintptr_t)local_res10.QuadPart == 0) {
    return (LARGE_INTEGER){ .QuadPart = (long long)-0x7ff8ffa9 };
  }
  iVar9 = lstrlenW((LPCWSTR)(uintptr_t)local_res10.QuadPart);
  PECMD_AllocWStringBuffer((WCHAR **)&local_78,(long long)((iVar9 + 1) * 2 + 4));
  lVar13 = (long long)(iVar9 + 1) * 2;
  PECMD_MemMoveForward((uint8_t *)local_78,(uint8_t *)(uintptr_t)local_res10.QuadPart,(int)lVar13);
  local_res10.QuadPart = (long long)(uintptr_t)local_78;
  *(uint16_t *)(lVar13 + (long long)(uintptr_t)local_78) = 0;
  local_b0.hwnd = (HWND)0x0;
  FUN_140102a90((ulonglong *)&local_b0.wFunc,0,0x30);
  PECMD_SkipLeadingControls((uint64_t *)&local_res10.QuadPart);
  LVar17 = local_res10;
  local_b0.fFlags = uVar20 | 0x610;
  local_b0.wFunc = 3;
  local_b0.pFrom = (LPCWSTR)(uintptr_t)local_res10.QuadPart;
  local_b8 = 0;
  bVar2 = false;
  PECMD_AllocStrSlot((uint16_t **)&local_70.QuadPart);
  local_68 = 0;
  local_60 = 0;
  PECMD_AllocStrSlot((uint16_t **)&local_58.QuadPart);
  local_50 = 0;
  local_48 = 0;
  pLVar16 = &local_70;
  PECMD_AllocString((WCHAR **)&local_70.QuadPart,10);
  PECMD_AllocString((WCHAR **)&local_58.QuadPart,10);
  WVar24 = *(WCHAR *)(uintptr_t)LVar17.QuadPart;
  LVar21 = LVar17;
  bVar3 = false;
  if (WVar24 != L'\0') {
    LVar19.QuadPart = LVar23.QuadPart;
    uVar8 = (uint8_t)local_res20.s.LowPart;
    do {
      if ((uVar8 == '\0') || (WVar24 != L';')) {
        if (WVar24 == L'>') {
          if (*(WCHAR *)(uintptr_t)(LVar17.QuadPart + -2) == L'=') {
            local_b0.wFunc = 2;
          }
          else if (*(WCHAR *)(uintptr_t)(LVar17.QuadPart + -2) == L'-') {
            local_b0.wFunc = 1;
          }
          else if (*(WCHAR *)(uintptr_t)(LVar17.QuadPart + 2) == L'>') {
            LVar17.QuadPart = LVar17.QuadPart + 2;
            local_b0.wFunc = 4;
          }
          LVar14.QuadPart = LVar17.QuadPart + -4;
          if ((unsigned long long)LVar21.QuadPart <= (unsigned long long)LVar14.QuadPart) {
            do {
              if ((((uint16_t)*(WCHAR *)(uintptr_t)LVar14.QuadPart < 9) || (0xd < (uint16_t)*(WCHAR *)(uintptr_t)LVar14.QuadPart)) &&
                 (*(WCHAR *)(uintptr_t)LVar14.QuadPart != L' ')) break;
              *(WCHAR *)(uintptr_t)LVar14.QuadPart = L'\0';
              LVar14.QuadPart = LVar14.QuadPart + -2;
            } while ((unsigned long long)LVar21.QuadPart <= (unsigned long long)LVar14.QuadPart);
            LVar19.s.HighPart = 0;
            LVar19.s.LowPart = local_b8;
          }
          *(WCHAR *)(uintptr_t)(LVar17.QuadPart + -2) = L'\0';
          *(WCHAR *)(uintptr_t)LVar17.QuadPart = L'\0';
          local_c0.QuadPart = LVar17.QuadPart + 2;
          PECMD_SkipLeadingControls((uint64_t *)&local_c0.QuadPart);
          LVar17 = local_c0;
          local_b0.pTo = (LPCWSTR)(uintptr_t)local_c0.QuadPart;
          if (*(WCHAR *)(uintptr_t)LVar21.QuadPart != L'\0') {
            PECMD_AppendParamToken(&pLVar16->QuadPart,(LPCWSTR)(uintptr_t)LVar21.QuadPart,'\0');
          }
          bVar2 = true;
          pLVar16 = &local_58;
          LVar21 = LVar17;
          uVar8 = (uint8_t)local_res20.s.LowPart;
        }
      }
      else {
        *(WCHAR *)(uintptr_t)LVar17.QuadPart = L'\0';
        local_c0.QuadPart = LVar17.QuadPart + 2;
        if (*(WCHAR *)(uintptr_t)LVar21.QuadPart != L'\0') {
          if ((bVar2) || (cVar7 = '\x01', bVar25 == 0)) {
            cVar7 = '\0';
          }
          PECMD_AppendParamToken(&pLVar16->QuadPart,(LPCWSTR)(uintptr_t)LVar21.QuadPart,cVar7);
        }
        PECMD_SkipLeadingControls((uint64_t *)&local_c0.QuadPart);
        LVar17 = local_c0;
        LVar21 = local_c0;
        uVar8 = (uint8_t)local_res20.s.LowPart;
        if (bVar2) {
          LVar19.s.LowPart = LVar19.s.LowPart + 1;
          LVar19.s.HighPart = 0;
          local_b8 = LVar19.s.LowPart;
        }
      }
      iVar18 = (int)LVar19.QuadPart;
      LVar17.QuadPart = LVar17.QuadPart + 2;
      WVar24 = *(WCHAR *)(uintptr_t)LVar17.QuadPart;
      bVar3 = bVar2;
    } while (WVar24 != L'\0');
  }
  if ((*(WCHAR *)(uintptr_t)LVar21.QuadPart != L'\0') && (PECMD_AppendParamToken(&pLVar16->QuadPart,(LPCWSTR)(uintptr_t)LVar21.QuadPart,'\0'), bVar3)
     ) {
    iVar18 = iVar18 + 1;
  }
  *(WCHAR *)(uintptr_t)(local_70.QuadPart + local_68 * 2) = L'\0';
  *(int16_t *)(uintptr_t)(local_58.QuadPart + local_50 * 2) = 0;
  local_b0.pFrom = (LPCWSTR)(uintptr_t)local_70.QuadPart;
  if (0 < iVar18) {
    local_b0.pTo = (LPCWSTR)(uintptr_t)local_58.QuadPart;
  }
  SetLastError(0);
  if ((uVar10 != 0) && (local_b0.wFunc == 3)) {
    if (((uVar10 & 2) == 0) ||
       (iVar18 = (int)SHFileOperationW((uint64_t)(uintptr_t)&local_b0), LVar22.QuadPart = LVar23.QuadPart, iVar18 != 0)) {
      LVar22.QuadPart = PECMD_DeleteDirectoryTree((LPCWSTR)(uintptr_t)local_70.QuadPart,(uint32_t)(uint8_t)local_c5);
    }
    goto LAB_14003c9a2;
  }
  if (bVar4) {
    if ((local_b0.pTo == (LPCWSTR)0x0) || (LVar22.QuadPart = (long long)(uintptr_t)local_b0.pTo, *local_b0.pTo == 0)
       ) {
      LVar22.QuadPart = LVar23.QuadPart;
    }
    iVar18 = (int)MoveFileExW((uint64_t)(uintptr_t)local_b0.pFrom,(uint64_t)(uintptr_t)LVar22.QuadPart,6);
  }
  else {
    if (bVar25 == 0) {
LAB_14003c906:
      if (local_b0.wFunc != 4) {
        if (1 < iVar18) {
          local_b0.fFlags = local_b0.fFlags | 1;
        }
        iVar18 = (int)SHFileOperationW((uint64_t)(uintptr_t)&local_b0);
        LVar22.QuadPart = iVar18;
        goto LAB_14003c9a2;
      }
      PECMD_StrDupAssign((uint16_t **)&local_c0.QuadPart,local_b0.pFrom);
      psVar15 = PECMD_LastPathSeparator((short *)(uintptr_t)local_c0.QuadPart);
      LVar22 = local_c0;
      if (psVar15 != (short *)0x0) {
        LVar22.QuadPart = (long long)(uintptr_t)(psVar15 + 1);
      }
      *(WCHAR *)(uintptr_t)LVar22.QuadPart = L'\0';
      psVar15 = PECMD_LastPathSeparator((short *)local_b0.pTo);
      LVar22.QuadPart = (long long)(uintptr_t)local_b0.pTo;
      if (psVar15 != (short *)0x0) {
        LVar22.QuadPart = (long long)(uintptr_t)(psVar15 + 1);
      }
      PECMD_AppendWideStr(&local_c0.QuadPart,(LPCWSTR)(uintptr_t)LVar22.QuadPart);
      BVar12 = MoveFileW((uint64_t)(uintptr_t)local_b0.pFrom,(uint64_t)(uintptr_t)local_c0.QuadPart);
      if (BVar12 != 1) {
        uVar10 = GetLastError();
        if (uVar10 == 0) {
          uVar10 = 1;
        }
        LVar23.s.HighPart = 0;
        LVar23.s.LowPart = uVar10;
      }
      pLVar16 = &local_c0;
      LVar22 = LVar23;
LAB_14003c8ca:
      PECMD_FreeStrBuf(&pLVar16->QuadPart);
      goto LAB_14003c9a2;
    }
    if (bVar5) {
      iVar18 = (int)RemoveDirectoryW((uint64_t)(uintptr_t)local_b0.pFrom);
    }
    else if (local_b0.wFunc == 3) {
      if (bVar6) {
        local_res20.QuadPart = 0;
        lVar13 = (long long)(uintptr_t)PECMD_OpenFileHandle((HANDLE *)&local_res20.QuadPart,local_b0.pFrom,0x80000000,7,(LPSECURITY_ATTRIBUTES)0x0,3,0x6000080,(HANDLE)0x0);
        DVar11 = GetLastError();
        if (lVar13 == 0) {
          if (DVar11 == 2) {
            if (((pthreadmbcinfo)local_res20.QuadPart != (pthreadmbcinfo)0x0) &&
               ((pthreadmbcinfo)local_res20.QuadPart != (pthreadmbcinfo)0xffffffffffffffff)) {
              CloseHandle((HANDLE)(uintptr_t)local_res20.QuadPart);
            }
            LVar22.QuadPart = 2;
            goto LAB_14003c9a2;
          }
        }
        else {
          if (((pthreadmbcinfo)local_res20.QuadPart != (pthreadmbcinfo)0x0) &&
             ((pthreadmbcinfo)local_res20.QuadPart != (pthreadmbcinfo)0xffffffffffffffff)) {
            CloseHandle((HANDLE)(uintptr_t)local_res20.QuadPart);
          }
          local_res20.QuadPart = 0;
          lVar13 = (long long)(uintptr_t)PECMD_OpenFileHandle((HANDLE *)&local_res20.QuadPart,local_b0.pFrom,0x80000000,7,(LPSECURITY_ATTRIBUTES)0x0,3,0x6000080,(HANDLE)0x0);
          DVar11 = GetLastError();
          if ((lVar13 == 0) && (DVar11 == 2)) {
            if (((pthreadmbcinfo)local_res20.QuadPart != (pthreadmbcinfo)0x0) &&
               ((pthreadmbcinfo)local_res20.QuadPart != (pthreadmbcinfo)0xffffffffffffffff)) {
              CloseHandle((HANDLE)(uintptr_t)local_res20.QuadPart);
            }
            goto LAB_14003c9a2;
          }
        }
        if (((pthreadmbcinfo)local_res20.QuadPart != (pthreadmbcinfo)0x0) &&
           ((pthreadmbcinfo)local_res20.QuadPart != (pthreadmbcinfo)0xffffffffffffffff)) {
          CloseHandle((HANDLE)(uintptr_t)local_res20.QuadPart);
        }
      }
      iVar18 = (int)DeleteFileW((uint64_t)(uintptr_t)local_b0.pFrom);
    }
    else if (local_b0.wFunc == 2) {
      if (bVar6) {
        PECMD_StrDupAssign((uint16_t **)&local_res20.QuadPart,(const uint16_t *)L" -dd -bs=8M \"");
        PECMD_AppendWideStr(&local_res20.QuadPart,local_b0.pTo);
        PECMD_AppendWideStr(&local_res20.QuadPart,(const uint16_t *)L"\",0,\"");
        PECMD_AppendWideStr(&local_res20.QuadPart,local_b0.pFrom);
        PECMD_AppendWideStr(&local_res20.QuadPart,(const uint16_t *)L"\"");
        LVar22.QuadPart = (long long)PECMD_DdCopyCommand((uint64_t)(uintptr_t)param_1,(uint64_t)local_res20.QuadPart);
        pLVar16 = &local_res20;
        goto LAB_14003c8ca;
      }
      iVar18 = (int)CopyFileW((uint64_t)(uintptr_t)local_b0.pFrom,(uint64_t)(uintptr_t)local_b0.pTo,0);
    }
    else {
      if (local_b0.wFunc != 1) goto LAB_14003c906;
      iVar18 = (int)MoveFileW((uint64_t)(uintptr_t)local_b0.pFrom,(uint64_t)(uintptr_t)local_b0.pTo);
    }
  }
  LVar22.QuadPart = LVar23.QuadPart;
  if (iVar18 != 1) {
    uVar10 = GetLastError();
    if (uVar10 == 0) {
      uVar10 = 1;
    }
    LVar22.s.HighPart = 0;
    LVar22.s.LowPart = uVar10;
  }
LAB_14003c9a2:
  PECMD_FreeStrBuf(&local_58.QuadPart);
  PECMD_FreeStrBuf(&local_70.QuadPart);
  PECMD_FreeStrBuf((WCHAR **)&local_78);
  return LVar22;
}
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
static void PECMD_CreateDtimItem(WPARAM param_1, longlong param_2, undefined8 *param_3, int param_4, int param_5, int param_6, int param_7, undefined8 *param_8, undefined8 *param_9, uint param_10){
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
uint64_t PECMD_DeviceCheckReady(LPCWSTR p) { (void)p; return 0; }
/* @0x1400799f0 size=— 多字节转换(函数指针槽)(直移) */
uint64_t PECMD_MultiByteConvert(uint64_t param_1, uint64_t param_2)
{
  uint64_t local_res10 = 0;
  PECMD_AllocWStringBuffer((WCHAR **)&local_res10,param_2 * 2 + 0x20);
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
  PECMD_FreeStrBuf(&local_res10);
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
        PECMD_StrBldCopyAnsi((int64_t*)&local_res8,(char*)pcVar3,0xffffffffffffffffULL);
        PECMD_AppendWideStr((uint16_t**)&local_res8,pWVar4);
        puVar2=FUN_14006e74c((const WCHAR*)(uintptr_t)local_res8,param_2,param_3);
        PECMD_FreeStrBuf((longlong*)&local_res8);
    } else {
        puVar2=PECMD_CreateNamedWaitObj(param_1,param_2);
    }
    return (undefined8*)puVar2;
}


uint64_t PECMD_LoadRasApi(void) { return 0; }
/* @0x1400e4e94 size=71 — 设虚表并关句柄(直移) */
void PECMD_DestroyEventSlots(uint64_t *param_1)
{
  *param_1 = (uint64_t)&PTR_FUN_14012b1c0;
  if (DAT_14013e1f8 != 0) { CloseHandle(DAT_14013e1f8); DAT_14013e1f8 = 0; }
  if (DAT_14013e200 != 0) { CloseHandle(DAT_14013e200); DAT_14013e200 = 0; }
}
/* @0x1400e5248 size=— 递归弹出菜单构建(直移) */
void PECMD_BuildPopMenuRecursive(longlong param_1, uint16_t *param_2, void *param_3, longlong param_4, longlong param_5, longlong *param_6)
{
  if ((uintptr_t)param_3 == 0) param_3 = (void *)(uintptr_t)CreatePopupMenu();
  int n = *(int *)(param_1 + 4);
  uint64_t i = 0, cnt = 0;
  if (n > 0) {
    do {
      uint8_t *pb = *(uint8_t **)(*(longlong *)(param_1 + 8) + i * 8);
      if (*pb == 0x80) {
        longlong sub = *(longlong *)(pb + 0x10);
        void *submenu = (void *)(uintptr_t)CreatePopupMenu();
        PECMD_BuildPopMenuRecursive(sub,param_2,(void *)(uintptr_t)submenu,param_4,param_5,param_6);
        AppendMenuW((void *)(uintptr_t)param_3,0x10,(uint64_t)(uintptr_t)submenu,(const unsigned short *)(uintptr_t)*(uint64_t *)(pb + 8));
        *param_2 = *param_2 + 1;
      } else {
        PECMD_MenuRegisterCommand(pb,param_2,param_3,param_4,param_5,param_6);
        *(int *)(pb + 0x28) = (int)cnt;
      }
      i++;
      cnt = (uint64_t)((int)cnt + 1);
    } while ((longlong)i < (longlong)n);
  }
}
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
            else { PECMD_StrBldCopyWide((long long*)plVar1,param_3); param_4[2]=*plVar1; }
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
      PECMD_FreeStrBuf((void *)&local_res8);
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
static void PECMD_CreateProgressItem(WPARAM param_1, longlong param_2, undefined8 *param_3, int param_4, int param_5, int param_6, int param_7, undefined4 param_8, LPWSTR param_9, undefined4 param_10){
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

static void PECMD_CreateSliderItem(WPARAM param_1, longlong param_2, undefined8 *param_3, int param_4, int param_5, int param_6, int param_7, ushort *param_8, undefined8 *param_9, uint param_10){
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

static void PECMD_CreateGroupItem(WPARAM param_1, longlong param_2, undefined8 *param_3, int param_4, int param_5, int param_6, int param_7, undefined8 *param_8, uint param_9, int *param_10, LPCWSTR param_11){
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

static void PECMD_CreateImageItem(WPARAM param_1, undefined8 param_2, undefined8 *param_3, int param_4, int param_5, int param_6, int param_7, undefined8 *param_8, undefined8 *param_9, undefined4 *param_10, undefined4 param_11, uint param_12){
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
/* @0x1400513b8 size=— 正则式括号组解析(直移) */
int PECMD_ParseRegexGroup(int *a, uint16_t **b, char *c)
{
  *c = 0;
  uint16_t *p = *b;
  if (*p != 0x5d) {
    int16_t s = *p;
    if (s == 0x5e) *b = (uint16_t *)(p + 1);
    p = *b;
    if (*p != 0x5d) {
      int r = PECMD_ParseCharClassRanges(a,b,(uint)(s == 0x5e));
      if (r < 0) return r;
      if (*(uint16_t *)*b != 0x5d) { *a = 4; return -1; }
      *b = (uint16_t *)((uint16_t *)*b + 1);
      return r;
    }
  }
  *b = (uint16_t *)(p + 1);
  *a = 6;
  return -1;
}
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
uint64_t SetEntriesInAclW(int c, void *ea, void *old, void **newa) { (void)c;(void)ea;(void)old;(void)newa; return 0; }
uint64_t GetSecurityInfo(void *h, int t, uint32_t i, void **a, void **b, void **c, void **d, void **e) { (void)h;(void)t;(void)i;(void)a;(void)b;(void)c;(void)d;(void)e; return 1; }
uint64_t SetSecurityInfo(void *h, int t, uint32_t i, void *a, void *b, void *c, void *d) { (void)h;(void)t;(void)i;(void)a;(void)b;(void)c;(void)d; return 1; }
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
int      FUN_140003864(void *a, const uint16_t *b, uint32_t c, uint32_t d, void *e, uint32_t f, uint32_t g, void *h) { (void)a;(void)b;(void)c;(void)d;(void)e;(void)f;(void)g;(void)h; return 0; }
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
void *PECMD_NamedObjLookupOrCreate(const uint16_t *a)
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
    *nw = (uint64_t)PECMD_CreateNamedMutex(0,0,0);
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
    uint64_t *p2 = (uint64_t *)PECMD_NamedObjLookupOrCreate(a);
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

void    *RegisterServiceCtrlHandlerW(const WCHAR *name, void *handler) { (void)name;(void)handler; return (void*)0; }
int      SHGetSpecialFolderPathA(void *hwnd, char *buf, int folder, int create) { (void)hwnd;(void)buf;(void)folder;(void)create; return 0; }
char    *lstrcatA(char *dst, const char *src) { (void)dst;(void)src; return dst; }

/* ---- P4 wave-4 补桩: 声明存在但缺定义的内部 helper ---- */
uint      FUN_14001b608(uint64_t a) { (void)a; return 0; }
long long FUN_140031454(long long *a, pthreadmbcinfo b) { (void)a;(void)b; return 0; }
int       FUN_140067cf4(long long *a, uint64_t *b) { (void)a;(void)b; return 0; }

/* ---- P4 wave-4 helper 补定义 (声明已存在但缺实体) ---- */
/* @0x14004e2cc size=441 串插值分发: 释放三串任务槽, 槽2 带前导 "*" 时按对象/脚本重分发 (decompiled.c 直移) */
/* 注: 反编译签名 (LARGE_INTEGER param_1, longlong* param_2) 按本文件既有桩签名 (uint64_t a, void* b) 表达,
   a 即 param_1.QuadPart (上下文地址值), b 即 param_2 (三串槽指针). */
void PECMD_ExecInterpString(uint64_t a, void *b)
{
  longlong *param_2 = (longlong *)b;
  LPCWSTR lpString;
  LPCWSTR lpString_00;
  LPCWSTR lpString_01;
  LPWSTR pWVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  longlong lVar5;
  LPWSTR local_res8;
  ulonglong *local_res10;

  lpString = (LPCWSTR)param_2[2];
  lpString_00 = (LPCWSTR)param_2[1];
  lpString_01 = (LPCWSTR)*param_2;
  if (lpString != (LPCWSTR)0x0) {
    iVar2 = lstrlenW(lpString);
    iVar3 = lstrlenW(lpString_01);
    iVar4 = lstrlenW(lpString_00);
    local_res10 = (ulonglong *)param_2[2];
    param_2[2] = 0;
    if (*lpString == L'*') {
      if (((*(longlong *)(lpString + -4) < 5) || (lpString[1] != L'\0')) || (lpString[2] != L'^')) {
        PECMD_ProcessScriptBlock((LARGE_INTEGER){ .QuadPart = (long long)a },
                                 (LARGE_INTEGER){ .QuadPart = (long long)(uintptr_t)(lpString + 1) },
                                 (longlong *)0x0,(longlong *)0x0,(pthreadmbcinfo)0x0);
      }
      else {
        ((void (*)(void *, longlong, longlong *))(uintptr_t)(**(code **)(*(longlong *)(lpString + 4) + 8)))
            (lpString + 4,(long long)a,param_2);
      }
    }
    else {
      PECMD_AllocWStringBuffer((WCHAR **)&local_res8,(long long)(iVar4 + 1 + iVar3 + 1 + iVar2 + 2));
      pWVar1 = local_res8;
      lstrcpyW(local_res8,lpString);
      lVar5 = (long long)(iVar2 + 2);
      pWVar1[lVar5 + -1] = L'&';
      pWVar1[lVar5 + -2] = L' ';
      lstrcpyW(pWVar1 + lVar5,lpString_01);
      lVar5 = (iVar3 + 1) + lVar5;
      pWVar1[lVar5 + -1] = L' ';
      lstrcpyW(pWVar1 + lVar5,lpString_00);
      PECMD_ExecuteCommand((int64_t *)(uintptr_t)a,local_res8,0,(LPCWSTR)0x0,0,0,0);
      if ((*(byte *)(a + 0x11) & 1) != 0) {
        PECMD_ZeroLenBuf((ulonglong *)local_res8);
      }
      PECMD_FreeStrBuf((WCHAR **)&local_res8);
    }
    if ((*(byte *)(a + 0x11) & 1) != 0) {
      PECMD_ZeroLenBuf(local_res10);
    }
    PECMD_FreeStrBuf((WCHAR **)&local_res10);
  }
  if ((*(byte *)(a + 0x11) & 1) != 0) {
    PECMD_ZeroLenBuf((ulonglong *)*param_2);
  }
  PECMD_FreeTripleString(param_2);
  free(param_2);
  return;
}
uint64_t FUN_14004fb44(longlong *a, ulonglong b) { (void)a;(void)b; return 1; }
void FUN_14005d9a8(int64_t a, int b) { (void)a;(void)b; }
uint64_t FUN_1400630d0(int a) { (void)a; return 0; }
/* @0x14006e8f4 size=439 — 脚本对象释放: COM/临界区清理 + 数组元素回收 (decompiled.c 直移) */
void PECMD_CleanupFrameChain(int64_t a)
{
  int *piVar1;
  int iVar2;
  longlong *plVar3;
  longlong lVar4;
  int iVar5;
  int iVar6;
  int iVar7;
  longlong *plVar8;

  iVar7 = 0;
  if (*(byte *)(a + 0x12) != 0) {
    if (((*(byte *)(a + 0x12) & 4) != 0) && (DAT_14013d820 != (void *)0)) {
      ((void (*)(void))(uintptr_t)DAT_14013d820)();   /* 实为 CoUninitialize(无参); 槽按既有约定声明 int(*)(int), 调用处强转 */
    }
    if ((*(byte *)(a + 0x12) & 2) != 0) {
      if (DAT_14013d808 != (void *)0) {
        (*DAT_14013d808)();
      }
      LeaveCriticalSection((void *)&DAT_14013e168);
    }
    if ((*(byte *)(a + 0x12) & 1) != 0) {
      LeaveCriticalSection((void *)&DAT_14013e190);
    }
    *(undefined1 *)(a + 0x12) = 0;
  }
  plVar3 = *(longlong **)(a + 0xe0);
  if (plVar3 != (longlong *)0) {
    iVar5 = iVar7;
    if (0 < *(int *)(a + 0xe8)) {
      iVar5 = *(int *)(*plVar3 + 0x14);
    }
    if ((*(longlong *)(a + 0x38) == 0) || (*(undefined8 **)(a + 0x38) == &DAT_14013d130)) {
      iVar5 = iVar7;
    }
    iVar6 = 1;
    plVar8 = plVar3;
    if (1 < *(int *)(a + 0xe8)) {
      do {
        lVar4 = plVar8[1];
        iVar2 = *(int *)(lVar4 + 0x14);
        if (iVar2 < 0) {
          *(undefined4 *)(lVar4 + 0x14) = 0;
        }
        else if (iVar5 < iVar2) {
          *(int *)(lVar4 + 0x14) = iVar5;
        }
        else if (iVar2 < iVar5) {
          iVar5 = iVar2;
        }
        iVar6 = iVar6 + 1;
        plVar8 = plVar8 + 1;
      } while (iVar6 < *(int *)(a + 0xe8));
    }
    while (0 < *(int *)(a + 0xe8)) {
      if ((0 < *(int *)(plVar3[(longlong)*(int *)(a + 0xe8) + -1] + 0x14)) &&
          (*(longlong *)(a + 0x38) != 0)) goto LAB_14006ea23;
      iVar5 = *(int *)(a + 0xe8) + -1;
      *(int *)(a + 0xe8) = iVar5;
      (**(code **)(*(longlong *)plVar3[iVar5] + 8))();
      plVar3[*(int *)(a + 0xe8)] = 0;
    }
    if (*(longlong *)(a + 0x38) != 0) {
LAB_14006ea23:
      plVar8 = plVar3;
      if (0 < *(int *)(a + 0xe8)) {
        do {
          if (0 < *(int *)(*plVar8 + 0x14)) {
            piVar1 = (int *)(*plVar8 + 0x14);
            *piVar1 = *piVar1 + -1;
          }
          PECMD_ArrayAppend(*(longlong *)(a + 0x38),*plVar8);
          iVar7 = iVar7 + 1;
          plVar3[*(int *)(a + 0xe8)] = 0;
          plVar8 = plVar8 + 1;
        } while (iVar7 < *(int *)(a + 0xe8));
      }
    }
    if (*(longlong *)(a + 0xe0) != 0) {
      HeapFree(DAT_14013d328,0,(void *)(uintptr_t)(*(longlong *)(a + 0xe0) + -8));
    }
    *(undefined8 *)(a + 0xe0) = 0;
    *(undefined4 *)(a + 0xe8) = 0;
  }
  return;
}

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
void       PECMD_UpdateModifierKeys(void);
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
                PECMD_UpdateModifierKeys();
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
                                            /* 键状态刷新 (leaf stub) */

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
  PECMD_FreeStrBuf((WCHAR **)&local_res18);
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
/* @0x1400f2384 size=1378 命令注册/反注册表维护 (decompiled.c 直移)
 * 签名保持原桩: (longlong a=param_1 对象, const unsigned short *b=param_2 命令串,
 *   longlong *c=param_3 扩展接收器, longlong d=param_4 命令值, int e=param_5 注册参数,
 *   int f=param_6 标志(ushort))
 * 取舍(歧义自决):
 *  - CONCAT71(extraout_var,bVarX) 为 Ghidra 寄存器拼接残留, 按既定模式(core_b3r_g3.c)归一化为
 *    底层 bool 判断 (PECMD_ParseNumTryWriteback 即返回 0/1, 反汇编确认调用后直接 test eax)。
 *  - iVar5 = FUN_1400701f8(...) : rename_map 目标 PECMD_ParseNumSkipChar_01f8 为 void 桩
 *    (core_b3b.c), 会丢失该调用点必需的成功标志 — 反汇编证明 FUN_1400701f8 尾部只动 rdx/rbx,
 *    eax 保留内层 PECMD_ParseNumTryWriteback 的解析结果被调用方直接 test。故按二进制语义内联复刻
 *    (PECMD_ParseNumTryWriteback + 未到串尾再跳一字), 不复用 void 桩。
 *  - L'￐'(U+FFD0) 写为 0xffd0 (即 (ushort)(WVar1-0x30) 的环回写法)。
 *  - 返回类型保持原桩 void (decompiled 返回 iVar18: 0 成功 / 0x80070057 E_INVALIDARG 失败;
 *    本文件唯一调用方行 6694 忽略返回值)。
 */
void  PECMD_DeferDialogAction2(longlong a, const unsigned short *b, longlong *c, longlong d, int e, int f)
{
  WCHAR WVar1;
  bool bVar2;
  WCHAR *pWVar3;
  bool bVar4;
  int iVar5;
  undefined8 uVar6;
  ushort *puVar7;
  undefined2 *puVar8;
  ushort uVar9;
  int iVar10;
  LPCWSTR pWVar11;
  longlong lVar12;
  longlong lVar13;
  ushort uVar14;
  longlong lVar15;
  char cVar16;
  int iVar17;
  int iVar18;
  int local_res8 [2];
  LPCWSTR local_res10;
  WCHAR *local_58;
  WCHAR *local_50;
  WCHAR *local_48;
  longlong local_40;

  iVar10 = 0;
  iVar18 = 0;
  lVar13 = *(longlong *)(a + 8);
  iVar5 = 0;
  local_40 = 0;
  cVar16 = '\0';
  bVar2 = false;
  local_res10 = b;
  uVar14 = (ushort)f;
  if (e < 1) {
    for (; (WVar1 = *local_res10, WVar1 != L'\0' && (WVar1 != L':')); local_res10 = local_res10 + 1)
    {
      if (WVar1 == L'%') goto LAB_1400f240b;
    }
    if (*local_res10 == L'%') {
LAB_1400f240b:
      if (((local_res10[1] == L'&') && (local_res10[2] == L':')) ||
         ((*(char *)((longlong)c + 0xd) != '\0' && (local_res10[1] == L':')))) {
        for (pWVar11 = local_res10 + 2; (WVar1 = *pWVar11) != L'\0'; pWVar11 = pWVar11 + 1) {
          if (WVar1 == L'%') goto LAB_1400f2459;
          if ((WVar1 == L'=') || (((8 < (ushort)WVar1) && ((ushort)WVar1 < 0xe)) || (WVar1 == L' ')))
             break;
        }
        if (*pWVar11 == L'%') {
LAB_1400f2459:
          local_res10 = pWVar11 + 1;
        }
      }
    }
    thunk_FUN_1400f429c(&local_res10,0x3a);
    PECMD_StrBldCopyWideN((uint16_t **)&local_48,b,
                  (longlong)(int)(((longlong)local_res10 - (longlong)b) >> 1));
    PECMD_AllocStrSlot((uint16_t **)&local_50);
    local_58 = local_48;
    PECMD_ExpandVarDispatch(c,local_48,&local_50,0,1);
    local_58 = local_50;
    PECMD_SkipLeadingControlChars((long long *)&local_58);
    if (*local_res10 == L'\0') {
      if (*local_58 == L'$') goto LAB_1400f2511;
    }
    else {
      local_res10 = local_res10 + 1;
      PECMD_SkipLeadingControlChars((long long *)&local_res10);
LAB_1400f2511:
      WVar1 = *local_58;
      uVar14 = (ushort)f;
      while (((WVar1 != L'\0' && (WVar1 != L'#')) && (9 < (ushort)(WVar1 + 0xffd0)))) {
        if (WVar1 == L'_') {
          uVar14 = 2;
        }
        else if (WVar1 == L'$') {
          uVar14 = 5;
        }
        else if (WVar1 == L'*') {
          cVar16 = '\x10';
        }
        else {
          if (WVar1 != L'+') break;
          uVar14 = 0x80;
        }
        local_58 = local_58 + 1;
        WVar1 = *local_58;
      }
      local_res8[0] = 0;
      uVar14 = uVar14 | (short)cVar16;
      iVar18 = 0;
      if (*local_58 == L'#') {
        local_58 = local_58 + 1;
        iVar18 = 0x5000;
      }
      iVar17 = 0x111;
      uVar6 = FUN_14005c788("COMMAND",(ushort *)local_58,7);
      if ((char)uVar6 == '\0') {
        uVar6 = FUN_14005c788("NOTIFY",(ushort *)local_58,6);
        if ((char)uVar6 != '\0') {
          uVar9 = 0x200;
          iVar17 = 0x4e;
          local_58 = local_58 + 6;
          goto LAB_1400f260b;
        }
        bVar4 = PECMD_ParseNumTryWriteback((long long *)&local_58,&e);
        iVar5 = 0;
        if (!bVar4) goto LAB_1400f271a;
LAB_1400f270c:
        e = e + iVar18;
        if (0 < e) {
          PECMD_FreeStrBuf((WCHAR **)&local_50);
          PECMD_FreeStrBuf((WCHAR **)&local_48);
          goto LAB_1400f277d;
        }
      }
      else {
        uVar9 = 0x100;
        local_58 = local_58 + 7;
LAB_1400f260b:
        e = iVar17;
        bVar4 = bVar2;
        if (*local_58 == L'=') {
          uVar14 = uVar14 | uVar9;
          bVar4 = true;
          pWVar3 = local_58 + 1;
          if (local_58[1] == L'#') {
            iVar18 = 0x5000;
            pWVar3 = local_58 + 2;
          }
          local_58 = pWVar3;
          PECMD_ParseNumTryWriteback((long long *)&local_58,&e);
        }
        if (*local_58 == L':') {
          iVar5 = iVar10;
          if (!bVar4) {
            e = iVar17;
            iVar5 = 0;
          }
          goto LAB_1400f270c;
        }
        if (*local_58 == L'#') {
          local_58 = local_58 + 1;
          /* 原 iVar5 = FUN_1400701f8(&local_58,local_res8): 按二进制语义内联
           * (解析成功标志=eax 传递; 尾部未到串尾再跳一字), 见文件头取舍注释 */
          iVar5 = PECMD_ParseNumTryWriteback((long long *)&local_58,local_res8);
          if (*local_58 != 0) local_58 = local_58 + 1;
          if (0 < iVar5) {
            local_40 = (longlong)local_res8[0];
            uVar14 = uVar14 | 0x20;
          }
          bVar4 = PECMD_ParseNumTryWriteback((long long *)&local_58,local_res8);
          iVar5 = 0;
          if (bVar4) {
            uVar14 = uVar14 | 0x40;
            iVar5 = local_res8[0];
          }
          if ((uVar14 & 0x60) != 0) goto LAB_1400f270c;
        }
      }
LAB_1400f271a:
      iVar18 = -0x7ff8ffa9;
    }
    PECMD_FreeStrBuf((WCHAR **)&local_50);
    PECMD_FreeStrBuf((WCHAR **)&local_48);
  }
  else {
LAB_1400f277d:
    lVar12 = local_40;
    iVar18 = *(int *)(a + 0x10);
    if (0 < iVar18) {
      puVar7 = (ushort *)(lVar13 + 0x18);
      iVar10 = 0;
      do {
        if (((e == *(int *)(puVar7 + -8)) && (d == *(longlong *)(puVar7 + -0xc))) &&
           ((uVar14 == *puVar7 &&
            ((local_40 == *(longlong *)(puVar7 + 4) && (iVar5 == *(int *)(puVar7 + 2)))))))
        goto LAB_1400f27cc;
        iVar10 = iVar10 + 1;
        puVar7 = puVar7 + 0x14;
      } while (iVar10 < iVar18);
    }
    if (*local_res10 != L'*') {
      bVar2 = true;
LAB_1400f27cc:
      if (*local_res10 != L'*') {
        if (bVar2) {
          PECMD_GrowByteBuffer((void **)(a + 8),(longlong)(iVar18 + 1) * 0x28);
          iVar10 = *(int *)(a + 0x10);
          lVar13 = *(longlong *)(a + 8);
          *(undefined8 *)(lVar13 + (longlong)iVar10 * 0x28) = 0;
          *(undefined4 *)(lVar13 + 8 + (longlong)*(int *)(a + 0x10) * 0x28) = 0;
          *(undefined2 *)(lVar13 + 0x18 + (longlong)*(int *)(a + 0x10) * 0x28) = 0;
          *(undefined8 *)(lVar13 + 0x10 + (longlong)*(int *)(a + 0x10) * 0x28) = 0;
        }
        lVar15 = (longlong)iVar10;
        *(longlong *)(lVar13 + lVar15 * 0x28) = d;
        *(longlong *)(lVar13 + 0x20 + lVar15 * 0x28) = lVar12;
        *(int *)(lVar13 + 0x1c + lVar15 * 0x28) = iVar5;
        *(ushort *)(lVar13 + 0x18 + lVar15 * 0x28) = uVar14;
        *(int *)(lVar13 + 8 + lVar15 * 0x28) = e;
        PECMD_StrBldCopyWide((long long *)(lVar13 + 0x10 + lVar15 * 0x28),local_res10);
        if (bVar2) {
          *(int *)(a + 0x10) = *(int *)(a + 0x10) + 1;
        }
      }
      else {
        lVar15 = (longlong)iVar10;
        PECMD_HeapFreeWithHeader((long long *)(lVar13 + 0x10 + lVar15 * 0x28));
        lVar12 = (longlong)(*(int *)(a + 0x10) + -1);
        if (lVar15 < lVar12) {
          lVar12 = lVar12 - lVar15;
          puVar8 = (undefined2 *)(lVar13 + 0x40 + lVar15 * 0x28);
          do {
            *(undefined8 *)(puVar8 + -0x20) = *(undefined8 *)(puVar8 + -0xc);
            *(undefined4 *)(puVar8 + -0x1c) = *(undefined4 *)(puVar8 + -8);
            lVar12 = lVar12 + -1;
            puVar8[-0x14] = *puVar8;
            *(undefined8 *)(puVar8 + -0x18) = *(undefined8 *)(puVar8 + -4);
            puVar8 = puVar8 + 0x14;
          } while (lVar12 != 0);
        }
        *(undefined8 *)(lVar13 + -0x18 + (longlong)*(int *)(a + 0x10) * 0x28) = 0;
        *(int *)(a + 0x10) = *(int *)(a + 0x10) + -1;
      }
    }
    iVar18 = 0;
  }
  return;   /* decompiled: return iVar18 (0 / 0x80070057); 桩签名 void 保留 */
}

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
    PECMD_ParseNumTryWriteback((long long *)&local_40,local_48);
    local_res20[0] = local_48[0];
    if (*local_40 == L':') {
      local_40 = local_40 + 1;
      PECMD_ParseNumTryWriteback((long long *)&local_40,local_res20);
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
  PECMD_AllocWStringBuffer((WCHAR **)&local_res8,100);
  wsprintfW(local_res8,(const unsigned short *)L"&%s.id");
  if (param_6 != (longlong *)0x0) {
    PECMD_AppendLongDecimal(param_6,(ulonglong)*(ushort *)(param_1 + 2),local_res8);
  }
  if ((param_4 != 0) && (**(short **)(param_1 + 0x10) != 0)) {
    wsprintfW(local_res8,(const unsigned short *)L"_COMMAND#%d:");
    PECMD_AppendWideStr((uint16_t **)&local_res8,*(LPCWSTR *)(param_1 + 0x10));
    PECMD_DeferDialogAction2(param_5,local_res8,param_6,param_4,0,1);
  }
  PECMD_FreeStrBuf((WCHAR **)&local_res8);
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
    PECMD_FreeStrBuf(&local_res8);
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
    PECMD_CopyTokenTrimmed(param_2,param_3,param_4,0);
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
  lVar6 = PECMD_TokPrefixICmp("--gui-",(ushort *)local_res10,6);
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
  PECMD_AllocWStringBuffer((WCHAR **)&local_res8,(longlong)(iVar4 + 0x227));
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
  PECMD_FreeStrBuf((WCHAR **)&local_res8);
}


/* ---- wave-current support: 022e94 ---- */
unsigned int FUN_140022e24(void) { return 0; }        /* 热键序号表初始化 (leaf stub) */
                  /* 整数值解析 (leaf stub) */
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
  PECMD_AllocWStringBuffer(local_38,0x1851);
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
      PECMD_CrtShim((WCHAR *)local_38[0],0x140120458,(void *)(ulonglong)id,in_R9);
      *pwVar1 = L'\0';
      in_R9 = (void *)local_48;
      local_40 = pwVar1;
      iVar2 = (*DAT_14013cd48)(0xffffffff80000002,(const unsigned short *)L"SOFTWARE\\PELOGON\\RAMDATA\\HOTKEY",
                               local_38[0],in_R9,pwVar1,local_res20);
      if ((iVar2 == 0) && (*local_40 != L'\0')) {
        PECMD_ParseIntSkipSepChar((undefined8 *)&local_40,(int *)local_res18,0x2c);
        PECMD_ParseIntSkipSepChar((undefined8 *)&local_40,(int *)local_res10,0x2c);
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
  PECMD_FreeStrBuf((WCHAR **)&local_38);
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
    PECMD_FreeStrBuf((WCHAR **)&local_res8);
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
  PECMD_FreeStrBuf((WCHAR **)&local_res20);
  PECMD_FreeStrBuf((WCHAR **)&local_28);
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
  PECMD_StrBldCopyWide((long long *)&local_res10,(const WCHAR *)pWVar3);
  PECMD_AppendWideStr((uint16_t **)&local_res10,param_2);
  param_2 = local_res10;
LAB_14007d16a:
  if ((longlong *)param_1[0x18] != (longlong *)0x0) {
    param_1 = (longlong *)param_1[0x18];
  }
  EnterCriticalSection((LPCRITICAL_SECTION)&DAT_14013e190);
  FUN_14001e6bc(param_1,param_2,param_3,-1);
  LeaveCriticalSection((LPCRITICAL_SECTION)&DAT_14013e190);
  PECMD_FreeStrBuf((WCHAR **)&local_res10);
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
    PECMD_StrBldCopyWide((long long *)(lVar1 + 0xd8),(const WCHAR *)param_3);
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

  PECMD_GetApiProcCached("GetAdaptersInfo","Iphlpapi.DLL",&DAT_14013d4d8,(longlong *)0x0);
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
    PECMD_FreeStrBuf(&local_res20);
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
  PECMD_StrBldCopyAnsi(&local_res8,(char *)(uintptr_t)&DAT_14011d308,0xffffffffffffffffULL);
  FUN_140070398((longlong *)(param_1 + 0x4c),&local_res8);
  PECMD_FreeStrBuf((WCHAR **)&local_res8);
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
static void  PECMD_RasDialStatusCallback(void) { }                 /* 工作线程入口 (leaf stub) */
void *DAT_14013e1f8 = 0;    /* 0x14013e1f8 事件槽 A */
void *DAT_14013e200 = 0;    /* 0x14013e200 事件槽 B */
void *DAT_14013e208 = 0;    /* 0x14013e208 缓冲句柄槽 */
uint64_t DAT_14013e268 = 0x000005b708001323fcULL;  /* 0x14013e268 工作线程入口表 (pe_data_extract) */
uint64_t DAT_14013e280 = 0x000005b7e8001358ULL;    /* 0x14013e280 错误格式槽 (pe_data_extract) */
longlong DAT_14013e2a0 = 0;  /* 0x14013e2a0 当前接收方 */
longlong DAT_14013e298 = 0;  /* 0x14013e298 参数槽 */
uint8_t  DAT_14013e300[512] = {0}; /* 0x14013e300 错误消息缓冲 */

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

  PECMD_AllocWStringBuffer((WCHAR **)&local_68,4);
  plVar1 = param_1 + 0xe;
  PECMD_StrBldCopyWide((long long *)plVar1,param_2);
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
  PECMD_FreeStrBuf((WCHAR **)&local_68);
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
            PECMD_FreeStrBuf((WCHAR **)&local_res10);
            goto LAB_14008283d;
          }
          param_1 = (longlong *)param_1[7];
          lVar3 = param_3;
        } while (param_1 != (longlong *)0x0);
        LeaveCriticalSection((LPCRITICAL_SECTION)&DAT_14013e190);
        PECMD_FreeStrBuf((WCHAR **)&local_res10);
        lVar3 = 1;
        if (bVar8) {
          DAT_14013e20c = DAT_14013e20c & 0xfffb;
          FUN_14001c2cc((const unsigned short *)L"SeDebugPrivilege",4,0);
        }
        goto LAB_140082878;
      }
      PECMD_FreeStrBuf((WCHAR **)&local_res10);
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
        PECMD_FreeStrBuf((WCHAR **)&local_48);
        return lVar3;
      }
    }
    hWnd = (HWND)PECMD_FindTargetWindow(local_48,'\0','\0',(HWND)0x0,0,0,0,0);
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
  PECMD_FreeStrBuf((WCHAR **)&local_48);
  return 0;
}
