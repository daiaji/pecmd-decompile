/* ==========================================================================
 * win32_stub.h — Windows API 桩 (PECMD 重构编译用)
 *
 * 用途: 在 Linux/gcc 环境下对 PECMD 重构代码做 -fsyntax-only 编译验证.
 * 声明与真实 Win32 API 一致的签名, 但不链接.
 * 业务逻辑逐行还原; Win32 API 调用保留原名 (编译期验证参数),
 * 运行时由移植层 (wine/cygwin/原生) 替换.
 *
 * PECMD 为 Unicode GUI 程序, 主要使用 W 版本 API.
 * ========================================================================== */
#ifndef WIN32_STUB_H
#define WIN32_STUB_H

#include <stdint.h>
#include <stddef.h>

/* ---- 基础类型 ---- */
typedef int32_t BOOL;
typedef uint8_t BYTE;
typedef uint16_t WORD;
typedef uint32_t DWORD;
typedef DWORD *LPDWORD;
typedef void *HDEVINFO;

typedef uint32_t UINT;
typedef int32_t LONG;
typedef uint64_t ULONG_PTR;
typedef int64_t LONG_PTR;
typedef uintptr_t HANDLE;
typedef uintptr_t HFONT;
typedef uintptr_t HINSTANCE;
typedef uintptr_t HMODULE;
typedef uintptr_t HWND;
typedef uintptr_t HDC;
typedef uintptr_t HBITMAP;
typedef uintptr_t HPEN;
typedef uintptr_t HBRUSH;
/* B0/P2: duplicate HFONT typedef removed */
typedef uintptr_t HICON;
typedef uintptr_t HCURSOR;
typedef uintptr_t HMENU;
typedef uintptr_t HHOOK;
typedef uintptr_t HKEY;
typedef uint32_t REGSAM;
typedef uintptr_t HGLOBAL;
typedef uintptr_t HLOCAL;
typedef uintptr_t HRGN;
typedef uintptr_t HGDIOBJ;
typedef uintptr_t HPALETTE;
typedef uintptr_t HACCEL;
typedef uintptr_t HTASK;
typedef uintptr_t HKL;
typedef uintptr_t HWINEVENTHOOK;
typedef uintptr_t SC_HANDLE;
typedef uintptr_t HINTERNET;
typedef uintptr_t HIMAGELIST;
typedef uintptr_t HCONV;
typedef uintptr_t HDDEDATA;
typedef uintptr_t HRAWINPUT;
typedef uintptr_t HSZ;
typedef uintptr_t HUDD;
typedef uintptr_t WPARAM;
typedef uintptr_t LPARAM;
typedef intptr_t LRESULT;
typedef void *LPVOID;
typedef const void *LPCVOID;
typedef char CHAR;
typedef int8_t INT8;
typedef int16_t SHORT;
typedef int32_t INT;
typedef int64_t INT64;
typedef uint16_t USHORT;
typedef uint64_t ULONGLONG;
typedef uint64_t DWORDLONG;
typedef uint32_t ULONG;
typedef int32_t HRESULT;
typedef void *HANDLE_PTR;
typedef uint64_t DWORD_PTR;

typedef CHAR *LPSTR;
typedef const CHAR *LPCSTR;
typedef uint16_t WCHAR;
typedef WCHAR *LPWSTR;
typedef const WCHAR *LPCWSTR;
typedef void *LPSECURITY_ATTRIBUTES;
typedef void *LPDEVMODEA;
typedef void *LPRECT;
typedef void *LPMSG;
typedef void *LPPROCESS_INFORMATION;
typedef void *LPSTARTUPINFOW;
typedef void *LPMINMAXINFO;

#define TRUE  1
#define FALSE 0
#define NULL  ((void *)0)
#define INVALID_HANDLE_VALUE ((HANDLE)(uintptr_t)-1)
#define HWND_DESKTOP ((HWND)0)

/* ---- 消息/窗口常量 ---- */
#define WM_CLOSE         0x0010
#define WM_COMMAND       0x0111
#define WM_DESTROY       0x0002
#define WM_SETFOCUS      0x0007
#define WM_KILLFOCUS     0x0008
#define WM_PAINT         0x000F
#define WM_TIMER         0x0113
#define WM_USER          0x0400
#define WM_COPYDATA      0x004A
#define WM_GETMINMAXINFO 0x0024
#define WS_VISIBLE       0x10000000u
#define WS_OVERLAPPEDWINDOW 0x00CF0000u
#define MB_OK            0x0u
#define MB_OKCANCEL      0x1u
#define MB_ABORTRETRYIGNORE 0x2u
#define MB_YESNOCANCEL   0x3u
#define MB_YESNO         0x4u
#define MB_RETRYCANCEL   0x5u
#define MB_ICONERROR     0x10u
#define MB_ICONQUESTION  0x20u
#define MB_ICONWARNING   0x30u
#define MB_ICONINFORMATION 0x40u
#define MB_DEFBUTTON2    0x100u
#define MB_DEFBUTTON3    0x200u
#define MB_TOPMOST       0x40000u
#define MB_SETFOREGROUND 0x10000u
#define IDOK      1
#define IDCANCEL  2
#define IDABORT   3
#define IDRETRY   4
#define IDIGNORE  5
#define IDYES     6
#define IDNO      7

/* ---- 文件 ---- */
#define GENERIC_READ    0x80000000u
#define GENERIC_WRITE   0x40000000u
#define FILE_SHARE_READ 0x1u
#define FILE_SHARE_WRITE 0x2u
#define OPEN_EXISTING   3
#define CREATE_ALWAYS   2
#define OPEN_ALWAYS     4
#define CREATE_NEW      1
#define TRUNCATE_EXISTING 5
#define FILE_BEGIN      0
#define FILE_CURRENT    1
#define FILE_END        2
#define FILE_ATTRIBUTE_NORMAL 0x80u
#define FILE_ATTRIBUTE_DIRECTORY 0x10u
#define FILE_ATTRIBUTE_READONLY 0x1u
#define INVALID_FILE_SIZE ((DWORD)0xFFFFFFFFu)
#define INVALID_SET_FILE_POINTER ((DWORD)0xFFFFFFFFu)
#define MAX_PATH 260

/* ---- 窗口/控件常量 ---- */
#define SW_HIDE            0
#define SW_SHOWNORMAL      1
#define SW_SHOWMINIMIZED   2
#define SW_SHOWMAXIMIZED   3
#define SW_SHOW            5
#define SW_RESTORE         9
#define SWP_NOSIZE         0x0001
#define SWP_NOMOVE         0x0002
#define SWP_NOZORDER       0x0004
#define SWP_NOACTIVATE     0x0010
#define SWP_SHOWWINDOW     0x0040
#define SWP_HIDEWINDOW     0x0080

#define GWL_STYLE        (-16)
#define GWL_EXSTYLE      (-20)
#define GWLP_WNDPROC     (-4)
#define GWLP_HINSTANCE   (-6)
#define GWLP_HWNDPARENT  (-8)
#define GWLP_USERDATA    (-21)
#define DWLP_MSGRESULT   0
#define GWL_ID           (-12)
#define GWL_HWNDPARENT   (-8)
#define GWL_USERDATA     (-21)
#define GWL_WNDPROC      (-4)

#define WS_CHILD          0x40000000u
#define WS_POPUP          0x80000000u
#define WS_CAPTION        0x00C00000u
#define WS_BORDER         0x00800000u
#define WS_DLGFRAME       0x00400000u
#define WS_SYSMENU        0x00080000u
#define WS_THICKFRAME     0x00040000u
#define WS_MINIMIZEBOX    0x00020000u
#define WS_MAXIMIZEBOX    0x00010000u
#define WS_TABSTOP        0x00010000u
#define WS_DISABLED       0x08000000u
#define WS_HSCROLL        0x00100000u
#define WS_VSCROLL        0x00200000u
#define WS_EX_TOPMOST     0x00000008u
#define WS_EX_TOOLWINDOW  0x00000080u
#define WS_EX_APPWINDOW   0x00040000u
#define WS_EX_LAYERED     0x00080000u

#define CS_HREDRAW        0x0001
#define CS_VREDRAW        0x0002
#define CS_DBLCLKS        0x0008
#define CS_GLOBALCLASS    0x4000

#define CW_USEDEFAULT ((int)0x80000000)

#define COLOR_WINDOW 5
#define COLOR_BTNFACE 15

#define CLR_NONE ((DWORD)0xFFFFFFFFu)

#define TRANSPARENT 1
#define OPAQUE      2

#define PS_SOLID    0
#define PS_DASH     1
#define PS_DOT      2
#define PS_NULL     5

#define SRCCOPY   0x00CC0020u
#define DIB_RGB_COLORS 0
#define BI_RGB    0
#define BI_RLE8   1
#define BI_RLE4   2
#define BI_BITFIELDS 3
#define BI_PNG    4
#define BI_JPEG   5

#define GMEM_FIXED 0
#define GMEM_MOVEABLE 0x2u
#define GMEM_ZEROINIT 0x40u

#define INFINITE 0xFFFFFFFFu

/* ---- 结构 ---- */
typedef struct tagPOINT { LONG x, y; } POINT;
typedef struct tagRECT { LONG left, top, right, bottom; } RECT;
typedef struct tagSIZE { LONG cx, cy; } SIZE;

typedef struct tagMSG {
    HWND   hwnd;
    UINT   message;
    WPARAM wParam;
    LPARAM lParam;
    DWORD  time;
    POINT  pt;
} MSG;

typedef struct tagBITMAPINFOHEADER {
    DWORD biSize;
    LONG  biWidth;
    LONG  biHeight;
    WORD  biPlanes;
    WORD  biBitCount;
    DWORD biCompression;
    DWORD biSizeImage;
    LONG  biXPelsPerMeter;
    LONG  biYPelsPerMeter;
    DWORD biClrUsed;
    DWORD biClrImportant;
} BITMAPINFOHEADER;

typedef struct tagBITMAPFILEHEADER {
    WORD  bfType;
    DWORD bfSize;
    WORD  bfReserved1;
    WORD  bfReserved2;
    DWORD bfOffBits;
} BITMAPFILEHEADER;

typedef struct tagRGBQUAD {
    BYTE rgbBlue;
    BYTE rgbGreen;
    BYTE rgbRed;
    BYTE rgbReserved;
} RGBQUAD;

typedef struct tagBITMAPINFO {
    BITMAPINFOHEADER bmiHeader;
    RGBQUAD          bmiColors[1];
} BITMAPINFO;

typedef struct tagBITMAP {
    LONG  bmType;
    LONG  bmWidth;
    LONG  bmHeight;
    LONG  bmWidthBytes;
    WORD  bmPlanes;
    WORD  bmBitsPixel;
    void *bmBits;
} BITMAP;

typedef uint32_t COLORREF;

typedef struct tagLOGPEN {
    UINT  lopnStyle;
    POINT lopnWidth;
    COLORREF lopnColor;
} LOGPEN;

typedef struct tagWNDCLASSW {
    UINT      style;
    LRESULT (*lpfnWndProc)(HWND, UINT, WPARAM, LPARAM);
    int       cbClsExtra;
    int       cbWndExtra;
    HINSTANCE hInstance;
    HICON     hIcon;
    HCURSOR   hCursor;
    HBRUSH    hbrBackground;
    LPCWSTR   lpszMenuName;
    LPCWSTR   lpszClassName;
} WNDCLASSW;

typedef struct tagWNDCLASSEXW {
    UINT      cbSize;
    UINT      style;
    LRESULT (*lpfnWndProc)(HWND, UINT, WPARAM, LPARAM);
    int       cbClsExtra;
    int       cbWndExtra;
    HINSTANCE hInstance;
    HICON     hIcon;
    HCURSOR   hCursor;
    HBRUSH    hbrBackground;
    LPCWSTR   lpszMenuName;
    LPCWSTR   lpszClassName;
    HICON     hIconSm;
} WNDCLASSEXW;

typedef struct tagCREATESTRUCTW {
    LPVOID    lpCreateParams;
    HINSTANCE hInstance;
    HMENU     hMenu;
    HWND      hwndParent;
    int       cy;
    int       cx;
    int       y;
    int       x;
    LONG      style;
    LPCWSTR   lpszName;
    LPCWSTR   lpszClass;
    DWORD     dwExStyle;
} CREATESTRUCTW;

typedef struct tagMINMAXINFO {
    POINT ptReserved;
    POINT ptMaxSize;
    POINT ptMaxPosition;
    POINT ptMinTrackSize;
    POINT ptMaxTrackSize;
} MINMAXINFO;

typedef struct _STARTUPINFOW {
    DWORD   cb;
    LPWSTR  lpReserved;
    LPWSTR  lpDesktop;
    LPWSTR  lpTitle;
    DWORD   dwX;
    DWORD   dwY;
    DWORD   dwXSize;
    DWORD   dwYSize;
    DWORD   dwXCountChars;
    DWORD   dwYCountChars;
    DWORD   dwFillAttribute;
    DWORD   dwFlags;
    WORD    wShowWindow;
    WORD    cbReserved2;
    BYTE   *lpReserved2;
    HANDLE  hStdInput;
    HANDLE  hStdOutput;
    HANDLE  hStdError;
} STARTUPINFOW;

typedef struct _PROCESS_INFORMATION {
    HANDLE hProcess;
    HANDLE hThread;
    DWORD  dwProcessId;
    DWORD  dwThreadId;
} PROCESS_INFORMATION;

typedef struct _SYSTEMTIME {
    WORD wYear;
    WORD wMonth;
    WORD wDayOfWeek;
    WORD wDay;
    WORD wHour;
    WORD wMinute;
    WORD wSecond;
    WORD wMilliseconds;
} SYSTEMTIME;

typedef struct _FILETIME {
    DWORD dwLowDateTime;
    DWORD dwHighDateTime;
} FILETIME;

typedef struct _WIN32_FIND_DATAW {
    DWORD    dwFileAttributes;
    FILETIME ftCreationTime;
    FILETIME ftLastAccessTime;
    FILETIME ftLastWriteTime;
    DWORD    nFileSizeHigh;
    DWORD    nFileSizeLow;
    DWORD    dwReserved0;
    DWORD    dwReserved1;
    WCHAR    cFileName[MAX_PATH];
    WCHAR    cAlternateFileName[14];
} WIN32_FIND_DATAW;

typedef union _ULARGE_INTEGER {
    struct { DWORD LowPart; DWORD HighPart; };
    uint64_t QuadPart;
} ULARGE_INTEGER;

typedef union _LARGE_INTEGER {
    struct { DWORD LowPart; LONG HighPart; };
    int64_t QuadPart;
} LARGE_INTEGER;

typedef struct _OVERLAPPED {
    ULONG_PTR Internal;
    ULONG_PTR InternalHigh;
    union {
        struct { DWORD Offset; DWORD OffsetHigh; };
        LPVOID  Pointer;
    };
    HANDLE hEvent;
} OVERLAPPED;

typedef struct _SECURITY_ATTRIBUTES {
    DWORD  nLength;
    LPVOID lpSecurityDescriptor;
    BOOL   bInheritHandle;
} SECURITY_ATTRIBUTES;

typedef struct tagPAINTSTRUCT {
    HDC  hdc;
    BOOL fErase;
    RECT rcPaint;
    BOOL fRestore;
    BOOL fIncUpdate;
    BYTE rgbReserved[32];
} PAINTSTRUCT;

typedef struct tagWINDOWPOS {
    HWND hwnd;
    HWND hwndInsertAfter;
    int  x, y, cx, cy;
    UINT flags;
} WINDOWPOS;

typedef struct tagSCROLLINFO {
    UINT cbSize;
    UINT fMask;
    int  nMin, nMax, nPage, nPos, nTrackPos;
} SCROLLINFO;

/* ---- 注册表 ---- */
#define HKEY_CLASSES_ROOT   ((HKEY)(uintptr_t)0x80000000)
#define HKEY_CURRENT_USER   ((HKEY)(uintptr_t)0x80000001)
#define HKEY_LOCAL_MACHINE  ((HKEY)(uintptr_t)0x80000002)
#define HKEY_USERS          ((HKEY)(uintptr_t)0x80000003)
#define KEY_QUERY_VALUE  0x0001u
#define KEY_SET_VALUE    0x0002u
#define KEY_CREATE_SUB_KEY 0x0004u
#define KEY_ENUMERATE_SUB_KEYS 0x0008u
#define KEY_READ 0x20019u
#define KEY_WRITE 0x20006u
#define KEY_ALL_ACCESS 0xF003Fu
#define REG_OPTION_NON_VOLATILE 0
#define REG_SZ         1
#define REG_EXPAND_SZ  2
#define REG_BINARY     3
#define REG_DWORD      4
#define REG_MULTI_SZ   7
#define ERROR_SUCCESS      0
#define ERROR_FILE_NOT_FOUND 2
#define ERROR_PATH_NOT_FOUND 3
#define ERROR_ACCESS_DENIED 5
#define ERROR_INVALID_HANDLE 6
#define ERROR_NOT_ENOUGH_MEMORY 8
#define ERROR_INVALID_PARAMETER 87
#define ERROR_INSUFFICIENT_BUFFER 122
#define ERROR_NO_MORE_FILES 18
#define ERROR_MORE_DATA 234
#define ERROR_MOD_NOT_FOUND 126
#define ERROR_PROC_NOT_FOUND 127

/* ---- 文件 API ---- */
HANDLE CreateFileA(LPCSTR, DWORD, DWORD, LPSECURITY_ATTRIBUTES, DWORD, DWORD, HANDLE);
HANDLE CreateFileW(LPCWSTR, DWORD, DWORD, LPSECURITY_ATTRIBUTES, DWORD, DWORD, HANDLE);
BOOL   CloseHandle(HANDLE);
DWORD  WaitForSingleObject(HANDLE, DWORD);
BOOL   ReadFile(HANDLE, LPVOID, DWORD, DWORD *, void *);
BOOL   WriteFile(HANDLE, const void *, DWORD, DWORD *, void *);
DWORD  SetFilePointer(HANDLE, LONG, LONG *, DWORD);
BOOL   SetFilePointerEx(HANDLE, int64_t, int64_t *, DWORD);
BOOL   GetFileSizeEx(HANDLE, void *);
BOOL   DeleteFileA(LPCSTR);
BOOL   DeleteFileW(LPCWSTR);
BOOL   MoveFileW(LPCWSTR, LPCWSTR);
BOOL   CopyFileW(LPCWSTR, LPCWSTR, BOOL);
BOOL   CreateDirectoryW(LPCWSTR, LPSECURITY_ATTRIBUTES);
BOOL   RemoveDirectoryW(LPCWSTR);
BOOL   SetCurrentDirectoryW(LPCWSTR);
DWORD  GetCurrentDirectoryW(DWORD, LPWSTR);
BOOL   SetEndOfFile(HANDLE);
BOOL   FlushFileBuffers(HANDLE);
BOOL   SetFileTime(HANDLE, const FILETIME *, const FILETIME *, const FILETIME *);
HANDLE FindFirstFileW(LPCWSTR, WIN32_FIND_DATAW *);
BOOL   FindNextFileW(HANDLE, WIN32_FIND_DATAW *);
BOOL   FindClose(HANDLE);
DWORD  GetFileAttributesW(LPCWSTR);
BOOL   GetFileAttributesExW(LPCWSTR, int, void *);
BOOL   SetFileAttributesW(LPCWSTR, DWORD);
DWORD  GetLogicalDriveStringsW(DWORD, LPWSTR);
DWORD  GetLogicalDrives(void);
UINT   GetDriveTypeW(LPCWSTR);
DWORD  GetVolumeInformationW(LPCWSTR, LPWSTR, DWORD, DWORD *, DWORD *, DWORD *, LPWSTR, DWORD);
BOOL   SetVolumeLabelW(LPCWSTR, LPCWSTR);
HANDLE CreateFileMappingW(HANDLE, LPSECURITY_ATTRIBUTES, DWORD, DWORD, DWORD, LPCWSTR);
HANDLE OpenFileMappingW(DWORD, BOOL, LPCWSTR);
HANDLE CreateFileMappingA(HANDLE, LPSECURITY_ATTRIBUTES, DWORD, DWORD, DWORD, LPCSTR);
HANDLE OpenFileMappingA(DWORD, BOOL, LPCSTR);
LPVOID MapViewOfFile(HANDLE, DWORD, DWORD, DWORD, size_t);
BOOL   UnmapViewOfFile(LPCVOID);
HANDLE FindFirstVolumeW(LPWSTR, DWORD);
BOOL   FindNextVolumeW(HANDLE, LPWSTR, DWORD);
BOOL   FindVolumeClose(HANDLE);
BOOL   DeleteVolumeMountPointW(LPCWSTR);
BOOL   GetVolumeNameForVolumeMountPointW(LPCWSTR, LPWSTR, DWORD);
BOOL   SetVolumeMountPointW(LPCWSTR, LPCWSTR);
BOOL   DefineDosDeviceW(DWORD, LPCWSTR, LPCWSTR);
BOOL   QueryDosDeviceW(LPCWSTR, LPWSTR, DWORD);
DWORD  GetShortPathNameW(LPCWSTR, LPWSTR, DWORD);
DWORD  GetFullPathNameW(LPCWSTR, DWORD, LPWSTR, LPWSTR *);

/* ---- 关键段 ---- */
void InitializeCriticalSection(void *);
void DeleteCriticalSection(void *);
typedef struct { intptr_t DebugInfo; long LockCount; long RecursionCount; HANDLE OwningThread; HANDLE LockSemaphore; intptr_t SpinCount; } CRITICAL_SECTION;
typedef CRITICAL_SECTION *LPCRITICAL_SECTION;
typedef struct { intptr_t dummy[0x82]; } NOTIFYICONDATAW_IMP;
typedef NOTIFYICONDATAW_IMP *PNOTIFYICONDATAW;
void EnterCriticalSection(void *);
void LeaveCriticalSection(void *);

/* ---- 内存 ---- */
HANDLE GetProcessHeap(void);
void  *HeapAlloc(HANDLE, DWORD, size_t);
BOOL   HeapFree(HANDLE, DWORD, void *);
LPVOID HeapReAlloc(HANDLE, DWORD, LPVOID, size_t);
HGLOBAL GlobalAlloc(DWORD, size_t);
HGLOBAL GlobalFree(HGLOBAL);
void   *GlobalLock(HGLOBAL);
BOOL   GlobalUnlock(HGLOBAL);
BOOL    OpenClipboard(HWND);
BOOL    CloseClipboard(void);
HANDLE  GetClipboardData(UINT);
LPVOID VirtualAlloc(LPVOID, size_t, DWORD, DWORD);
BOOL   VirtualFree(LPVOID, size_t, DWORD);
HLOCAL LocalFree(HLOCAL);

/* ---- 字符串/区域 ---- */
int      lstrlenW(LPCWSTR);
int      lstrlenA(LPCSTR);
LPWSTR   lstrcpyW(LPWSTR, LPCWSTR);
LPWSTR   lstrcpyA(LPSTR, LPCSTR);
int      lstrcmpW(LPCWSTR, LPCWSTR);
int      lstrcmpiW(LPCWSTR, LPCWSTR);
LPWSTR   CharUpperW(LPWSTR);
int      MultiByteToWideChar(UINT, DWORD, LPCSTR, int, LPWSTR, int);
int      WideCharToMultiByte(UINT, DWORD, LPCWSTR, int, LPSTR, int, LPCSTR, BOOL *);
UINT     GetACP(void);
int      GetLocaleInfoW(DWORD, DWORD, LPWSTR, int);
UINT     GetOEMCP(void);
BOOL     GetCPInfo(UINT, void *);
DWORD    GetStringTypeW(DWORD, LPCWSTR, int, WORD *);
DWORD    LCMapStringW(DWORD, DWORD, LPCWSTR, int, LPWSTR, int);
DWORD    LCMapStringA(DWORD, DWORD, LPCSTR, int, LPSTR, int);
int      wsprintfW(LPWSTR, LPCWSTR, ...);
int      _snwprintf(WCHAR *, size_t, const WCHAR *, ...);
int      wvsprintfW(LPWSTR, LPCWSTR, const void *);
BOOL     ExpandEnvironmentStringsW(LPCWSTR, LPWSTR, DWORD);
LPWSTR   GetCommandLineW(void);
BOOL     GetStartupInfoW(LPSTARTUPINFOW);
DWORD    GetEnvironmentVariableW(LPCWSTR, LPWSTR, DWORD);
BOOL     SetEnvironmentVariableW(LPCWSTR, LPCWSTR);
DWORD    GetEnvironmentVariableA(LPCSTR, LPSTR, DWORD);
LPWSTR   GetEnvironmentStringsW(void);
BOOL     FreeEnvironmentStringsW(LPWSTR);
UINT     GetSystemDefaultLCID(void);

/* ---- 模块/资源 ---- */
typedef uintptr_t HRSRC;

HMODULE GetModuleHandleA(LPCSTR);
HMODULE GetModuleHandleW(LPCWSTR);
DWORD   GetModuleFileNameW(HMODULE, LPWSTR, DWORD);
HMODULE LoadLibraryA(LPCSTR);
HMODULE LoadLibraryW(LPCWSTR);
HMODULE LoadLibraryExW(LPCWSTR, HANDLE, DWORD);
BOOL    FreeLibrary(HMODULE);
void    *GetProcAddress(HMODULE, LPCSTR);
int     LoadStringW(HINSTANCE, UINT, LPWSTR, int);
HRSRC   FindResourceW(HMODULE, LPCWSTR, LPCWSTR);
HGLOBAL LoadResource(HMODULE, HRSRC);
LPVOID  LockResource(HGLOBAL);
DWORD   SizeofResource(HMODULE, HRSRC);
BOOL    EnumResourceNamesW(HMODULE, LPCWSTR, void *, LONG_PTR);
HICON   LoadIconW(HINSTANCE, LPCWSTR);
HCURSOR LoadCursorW(HINSTANCE, LPCWSTR);
typedef uint16_t ATOM;
ATOM    RegisterClassExW(const WNDCLASSEXW *);
HANDLE  LoadImageW(HINSTANCE, LPCWSTR, UINT, int, int, UINT);
HICON   CopyImage(HANDLE, UINT, int, int, UINT);
BOOL    DestroyIcon(HICON);
#define MAKEINTRESOURCEW(i) ((LPCWSTR)(uintptr_t)((WORD)(i)))
#define MAKEINTRESOURCEA(i) ((LPCSTR)(uintptr_t)((WORD)(i)))

/* ---- 注册表 ---- */
LONG RegOpenKeyExW(HKEY, LPCWSTR, DWORD, DWORD, HKEY *);
LONG RegCreateKeyExW(HKEY, LPCWSTR, DWORD, LPWSTR, DWORD, DWORD, void *, HKEY *, DWORD *);
LONG RegCloseKey(HKEY);
LONG RegDeleteValueW(HKEY, LPCWSTR);
LONG RegEnumKeyExW(HKEY, DWORD, LPWSTR, DWORD *, DWORD *, LPWSTR, DWORD *, FILETIME *);
LONG RegEnumValueW(HKEY, DWORD, LPWSTR, DWORD *, DWORD *, DWORD *, BYTE *, DWORD *);
LONG RegQueryInfoKeyW(HKEY, LPWSTR, DWORD *, DWORD *, DWORD *, DWORD *, DWORD *, DWORD *, DWORD *, DWORD *, DWORD *, FILETIME *);
LONG RegQueryValueExA(HKEY, LPCSTR, DWORD *, DWORD *, BYTE *, DWORD *);
LONG RegQueryValueExW(HKEY, LPCWSTR, DWORD *, DWORD *, BYTE *, DWORD *);
LONG RegSetValueExA(HKEY, LPCSTR, DWORD, DWORD, const BYTE *, DWORD);
LONG RegSetValueExW(HKEY, LPCWSTR, DWORD, DWORD, const BYTE *, DWORD);

/* ---- 窗口 API ---- */
HWND    CreateWindowExW(DWORD, LPCWSTR, LPCWSTR, DWORD, int, int, int, int, HWND, HMENU, HINSTANCE, LPVOID);
BOOL    DestroyWindow(HWND);
BOOL    ShowWindow(HWND, int);
BOOL    UpdateWindow(HWND);
BOOL    MoveWindow(HWND, int, int, int, int, BOOL);
BOOL    SetWindowPos(HWND, HWND, int, int, int, int, UINT);
BOOL    SetWindowTextW(HWND, LPCWSTR);
int     GetWindowTextW(HWND, LPWSTR, int);
int     GetWindowTextLengthW(HWND);
BOOL    GetWindowRect(HWND, RECT *);
BOOL    GetClientRect(HWND, RECT *);
HWND    GetParent(HWND);
HWND    SetParent(HWND, HWND);
BOOL    IsWindow(HWND);
BOOL    IsWindowVisible(HWND);
BOOL    IsWindowEnabled(HWND);
BOOL    EnableWindow(HWND, BOOL);
BOOL    SetWindowRgn(HWND, HRGN, BOOL);
BOOL    InvalidateRect(HWND, const RECT *, BOOL);
BOOL    SetActiveWindow(HWND);
HWND    SetFocus(HWND);
HWND    GetFocus(void);
HWND    GetForegroundWindow(void);
BOOL    SetForegroundWindow(HWND);
HWND    GetDesktopWindow(void);
HWND    GetWindow(HWND, UINT);
HWND    GetDlgItem(HWND, int);
UINT    GetDlgCtrlID(HWND);
BOOL    SetDlgItemTextW(HWND, int, LPCWSTR);
BOOL    GetWindowLongW(HWND, int);
LONG_PTR GetWindowLongPtrW(HWND, int);
LONG_PTR SetWindowLongW(HWND, int, LONG);
LONG_PTR SetWindowLongPtrW(HWND, int, LONG_PTR);
HWND    FindWindowW(LPCWSTR, LPCWSTR);
HWND    FindWindowA(LPCSTR, LPCSTR);
HWND    FindWindowExW(HWND, HWND, LPCWSTR, LPCWSTR);
HWND    FindWindowExA(HWND, HWND, LPCSTR, LPCSTR);
BOOL    EnumWindows(void *, LONG_PTR);
BOOL    EnumChildWindows(HWND, void *, LONG_PTR);
BOOL    IsDialogMessageW(HWND, LPMSG);
HFONT   CreateFontW(int, int, int, int, int, DWORD, DWORD, DWORD, DWORD, DWORD, DWORD, DWORD, DWORD, LPCWSTR);
DWORD   GetWindowThreadProcessId(HWND, DWORD *);
BOOL    ScreenToClient(HWND, POINT *);
BOOL    ClientToScreen(HWND, POINT *);
BOOL    GetCursorPos(POINT *);
BOOL    SetCursorPos(int, int);
HCURSOR SetCursor(HCURSOR);
BOOL    SetLayeredWindowAttributes(HWND, COLORREF, BYTE, DWORD);
BOOL    RedrawWindow(HWND, const RECT *, HRGN, UINT);
BOOL    BringWindowToTop(HWND);
BOOL    SetScrollInfo(HWND, int, const SCROLLINFO *, BOOL);
int     GetScrollInfo(HWND, int, SCROLLINFO *);
int     GetScrollPos(HWND, int);
int     SetScrollPos(HWND, int, int, BOOL);
void    ShowScrollBar(HWND, int, BOOL);
BOOL    BeginPaint(HWND, PAINTSTRUCT *);
BOOL    EndPaint(HWND, const PAINTSTRUCT *);
HWND    GetDlgItem(HWND, int);
HACCEL  CreateAcceleratorTableW(void *, int);

/* ---- 消息 API ---- */
BOOL    PostMessageW(HWND, UINT, WPARAM, LPARAM);
LRESULT SendMessageW(HWND, UINT, WPARAM, LPARAM);
LRESULT SendMessageTimeoutW(HWND, UINT, WPARAM, LPARAM, UINT, UINT, DWORD *);
BOOL    PostQuitMessage(int);
BOOL    GetMessageW(MSG *, HWND, UINT, UINT);
BOOL    PeekMessageW(MSG *, HWND, UINT, UINT, UINT);
BOOL    TranslateMessage(const MSG *);
LRESULT DispatchMessageW(const MSG *);
UINT    MsgWaitForMultipleObjects(DWORD, const HANDLE *, BOOL, DWORD, DWORD);
HHOOK   SetWindowsHookExW(int, void *, HINSTANCE, DWORD);
BOOL    UnhookWindowsHookEx(HHOOK);
LRESULT CallNextHookEx(HHOOK, int, WPARAM, LPARAM);
DWORD   WaitForInputIdle(HANDLE, DWORD);

/* ---- 菜单 ---- */
HMENU   GetMenu(HWND);
BOOL    SetMenu(HWND, HMENU);
HMENU   GetSystemMenu(HWND, BOOL);
HMENU   CreatePopupMenu(void);
BOOL    DestroyMenu(HMENU);
BOOL    AppendMenuW(HMENU, UINT, uintptr_t, LPCWSTR);
BOOL    TrackPopupMenu(HMENU, UINT, int, int, int, HWND, const RECT *);
BOOL    EnableMenuItem(HMENU, UINT, UINT);
BOOL    DrawFrameControl(HDC, const RECT *, UINT, UINT);
UINT    GetSubMenu(HMENU, int);
UINT    GetMenuItemID(HMENU, int);
int     GetMenuStringW(HMENU, UINT, LPWSTR, int, UINT);
BOOL    ModifyMenuW(HMENU, UINT, UINT, uintptr_t, LPCWSTR);
BOOL    DrawIconEx(HDC, int, int, HICON, int, int, UINT, HBRUSH, UINT);

/* ---- GDI ---- */
HDC     GetDC(HWND);
int     ReleaseDC(HWND, HDC);
HDC     CreateCompatibleDC(HDC);
HBITMAP CreateCompatibleBitmap(HDC, int, int);
HBITMAP CreateBitmap(int, int, UINT, UINT, const void *);
HBITMAP CreateDIBSection(HDC, const BITMAPINFO *, uint32_t, void **, void *, DWORD);
HGDIOBJ SelectObject(HDC, HGDIOBJ);
BOOL    DeleteDC(HDC);
BOOL    DeleteObject(HGDIOBJ);
int     AddFontResourceW(LPCWSTR);
int     RemoveFontResourceW(LPCWSTR);
int     GetDeviceCaps(HDC, int);
int     GetObjectW(HGDIOBJ, int, void *);
int     GetObjectA(HGDIOBJ, int, void *);
BOOL    StretchBlt(HDC, int, int, int, int, HDC, int, int, int, int, DWORD);
BOOL    BitBlt(HDC, int, int, int, int, HDC, int, int, DWORD);
BOOL    StretchDIBits(HDC, int, int, int, int, int, int, int, int, const void *, const BITMAPINFO *, uint32_t, DWORD);
int     SetDIBits(HDC, HBITMAP, uint32_t, uint32_t, const void *, const BITMAPINFO *, uint32_t);
int     GetDIBits(HDC, HBITMAP, uint32_t, uint32_t, void *, BITMAPINFO *, uint32_t);
HPEN    CreatePen(int, int, COLORREF);
HBRUSH  CreateSolidBrush(COLORREF);
HRGN    CreateRoundRectRgn(int, int, int, int, int, int);
BOOL    Ellipse(HDC, int, int, int, int);
BOOL    LineTo(HDC, int, int);
BOOL    MoveToEx(HDC, int, int, void *);
BOOL    Rectangle(HDC, int, int, int, int);
BOOL    FillRect(HDC, const RECT *, HBRUSH);
BOOL    FrameRgn(HDC, HRGN, HBRUSH, int, int);
BOOL    DrawEdge(HDC, LPRECT, UINT, UINT);
BOOL    DrawTextW(HDC, LPCWSTR, int, RECT *, UINT);
BOOL    ExtTextOutW(HDC, int, int, UINT, const RECT *, LPCWSTR, UINT, const int *);
COLORREF SetTextColor(HDC, COLORREF);
COLORREF SetBkColor(HDC, COLORREF);
int     SetBkMode(HDC, int);
COLORREF GetBkColor(HDC);
int     GetStockObject(int);
COLORREF GetSysColor(int);
int     GetSystemMetrics(int);
BOOL    SetPixel(HDC, int, int, COLORREF);
COLORREF GetPixel(HDC, int, int);
BOOL    PtInRect(const RECT *, POINT);
void    OffsetRect(RECT *, int, int);
BOOL    DrawFocusRect(HDC, const RECT *);
BOOL    SetWindowOrgEx(HDC, int, int, void *);
BOOL    SetViewportOrgEx(HDC, int, int, void *);
UINT    SetMapMode(HDC, int);
int     MulDiv(int, int, int);

/* ---- 进程/线程 ---- */
HANDLE  GetCurrentProcess(void);
HANDLE  GetCurrentThread(void);
DWORD   GetCurrentProcessId(void);
DWORD   GetCurrentThreadId(void);
HANDLE  OpenProcess(DWORD, BOOL, DWORD);
BOOL    TerminateProcess(HANDLE, UINT);
DWORD   GetExitCodeProcess(HANDLE, DWORD *);
HANDLE  CreateThread(void *, size_t, void *, LPVOID, DWORD, DWORD *);
BOOL    TerminateThread(HANDLE, DWORD);
BOOL    CreateProcessW(LPCWSTR, LPWSTR, LPSECURITY_ATTRIBUTES, LPSECURITY_ATTRIBUTES, BOOL, DWORD, LPVOID, LPCWSTR, LPSTARTUPINFOW, LPPROCESS_INFORMATION);
BOOL    CreateProcessAsUserW(HANDLE, LPCWSTR, LPWSTR, LPSECURITY_ATTRIBUTES, LPSECURITY_ATTRIBUTES, BOOL, DWORD, LPVOID, LPCWSTR, LPSTARTUPINFOW, LPPROCESS_INFORMATION);
BOOL    ReadProcessMemory(HANDLE, LPCVOID, LPVOID, size_t, size_t *);
BOOL    WriteProcessMemory(HANDLE, LPVOID, const void *, size_t, size_t *);
BOOL    DuplicateHandle(HANDLE, HANDLE, HANDLE, HANDLE *, DWORD, BOOL, DWORD);
BOOL    GetHandleInformation(HANDLE, DWORD *);
BOOL    SetProcessWorkingSetSize(HANDLE, size_t, size_t);
void    ExitProcess(UINT);
BOOL    AssignProcessToJobObject(HANDLE, HANDLE);
HANDLE  CreateJobObjectW(void *, LPCWSTR);
BOOL    TerminateJobObject(HANDLE, UINT);
void    Sleep(DWORD);
void    SleepEx(DWORD, BOOL);
DWORD   WaitForMultipleObjects(DWORD, const HANDLE *, BOOL, DWORD);
DWORD   ResumeThread(HANDLE);
DWORD   SuspendThread(HANDLE);
BOOL    SetEvent(HANDLE);
BOOL    ResetEvent(HANDLE);
BOOL    CreateEventW(void *, BOOL, BOOL, LPCWSTR);
HANDLE  CreateMutexW(void *, BOOL, LPCWSTR);
HANDLE  CreateMutexA(void *, BOOL, LPCSTR);
BOOL    ReleaseMutex(HANDLE);
BOOL    ConnectNamedPipe(HANDLE, void *);
BOOL    WaitNamedPipeW(LPCWSTR, DWORD);

/* ---- 键盘/输入 ---- */
SHORT   GetAsyncKeyState(int);
SHORT   GetKeyState(int);
BOOL    GetKeyboardState(BYTE *);
BOOL    GetLastInputInfo(void *);
UINT    SendInput(UINT, void *, int);
void    keybd_event(BYTE, BYTE, DWORD, ULONG_PTR);

/* ---- 系统 ---- */
DWORD   GetTickCount(void);
void    GetSystemTimeAsFileTime(FILETIME *);
BOOL    SystemTimeToFileTime(const SYSTEMTIME *, FILETIME *);
BOOL    FileTimeToSystemTime(const FILETIME *, SYSTEMTIME *);
BOOL    GetSystemTime(SYSTEMTIME *);
BOOL    GetLocalTime(SYSTEMTIME *);
BOOL    QueryPerformanceCounter(int64_t *);
BOOL    QueryPerformanceFrequency(int64_t *);
UINT    GetLastError(void);
void    SetLastError(DWORD);
void    RaiseException(DWORD, DWORD, DWORD, const ULONG_PTR *);
BOOL    GlobalAddAtomA(LPCSTR);
BOOL    SetUnhandledExceptionFilter(void *);
void    UnhandledExceptionFilter(void *);
void    RtlCaptureContext(void *);
void    RtlUnwindEx(void *, void *, void *, void *, void *, void *);
void    RtlLookupFunctionEntry(void *, void *, void *);
void    EncodePointer(void *);
void    DecodePointer(void *);
BOOL    GetSystemPowerStatus(void *);
BOOL    InitiateSystemShutdownW(LPCWSTR, LPCWSTR, DWORD, BOOL, BOOL);
BOOL    AbortSystemShutdownW(LPCWSTR);
BOOL    AdjustTokenPrivileges(HANDLE, BOOL, void *, DWORD, void *, DWORD *);
BOOL    OpenProcessToken(HANDLE, DWORD, HANDLE *);
BOOL    LookupPrivilegeValueW(LPCWSTR, LPCWSTR, void *);
BOOL    GetTokenInformation(HANDLE, int, void *, DWORD, DWORD *);
BOOL    LookupPrivilegeValueW(LPCWSTR, LPCWSTR, void *);
BOOL    SHChangeNotify(LONG, UINT, const void *, const void *);
BOOL    SHGetSpecialFolderPathW(HWND, LPWSTR, int, BOOL);
BOOL    SetNamedSecurityInfoW(LPCWSTR, int, DWORD, void *, void *, void *, void *);
BOOL    PathMatchSpecW(LPCWSTR, LPCWSTR);
LPWSTR  StrChrW(LPCWSTR, WCHAR);
LPWSTR  StrRChrW(LPCWSTR, LPCWSTR, WCHAR);
BOOL    StrPBrkW(LPCWSTR, LPCWSTR);
int     StrCmpNIW(LPCWSTR, LPCWSTR, int);
int     StrCmpNIA(LPCSTR, LPCSTR, int);
int     StrCmpNW(LPCWSTR, LPCWSTR, int);
BOOL    StrStrW(LPCWSTR, LPCWSTR);
BOOL    StrStrIW(LPCWSTR, LPCWSTR);
BOOL    StrToIntExW(LPCWSTR, DWORD, int *);
LPWSTR  StrCpyNW(LPWSTR, LPCWSTR, int);

/* ---- 服务 ---- */
/* B0/P2: duplicate SC_HANDLE typedef removed */
typedef struct _SERVICE_STATUS {
    DWORD dwServiceType;
    DWORD dwCurrentState;
    DWORD dwControlsAccepted;
    DWORD dwWin32ExitCode;
    DWORD dwServiceSpecificExitCode;
    DWORD dwCheckPoint;
    DWORD dwWaitHint;
} SERVICE_STATUS;
typedef struct _SERVICE_TABLE_ENTRYW {
    LPWSTR  lpServiceName;
    void   *lpServiceProc;
} SERVICE_TABLE_ENTRYW;
#define SERVICE_WIN32_OWN_PROCESS 0x10u
#define SERVICE_RUNNING 4
#define SERVICE_ACCEPT_STOP 0x1u
#define SERVICE_ACCEPT_SHUTDOWN 0x4u
#define SC_MANAGER_ALL_ACCESS 0xF003Fu
#define SERVICE_QUERY_STATUS 0x4u
#define SERVICE_STOP 0x20u
#define SERVICE_CHANGE_CONFIG 0x2u
#define SERVICE_ALL_ACCESS 0xF01FFu
#define SERVICE_CONTROL_STOP 1

SC_HANDLE OpenSCManagerW(LPCWSTR, LPCWSTR, DWORD);
SC_HANDLE OpenServiceW(SC_HANDLE, LPCWSTR, DWORD);
BOOL    ControlService(SC_HANDLE, DWORD, SERVICE_STATUS *);
BOOL    DeleteService(SC_HANDLE);
BOOL    CloseServiceHandle(SC_HANDLE);
BOOL    StartServiceCtrlDispatcherW(const SERVICE_TABLE_ENTRYW *);
BOOL    SetServiceStatus(void *, void *);

/* ---- 设备管理 ---- */
BOOL    SetupDiDestroyDeviceInfoList(void *);
BOOL    SetupDiEnumDeviceInterfaces(void *, void *, const void *, DWORD, void *);
BOOL    SetupDiGetDeviceInterfaceDetailW(void *, void *, void *, DWORD, DWORD *, void *);
BOOL    SetupDiGetDeviceInstanceIdW(void *, void *, LPWSTR, DWORD, DWORD *);
BOOL    CM_Get_Parent(void *, void *, DWORD);

/* ---- 拖放 ---- */
void    DragAcceptFiles(HWND, BOOL);
UINT    DragQueryFileW(HANDLE, UINT, LPWSTR, UINT);
void    DragFinish(HANDLE);

/* ---- 定时器 ---- */
uintptr_t SetTimer(HWND, uintptr_t, UINT, void *);
BOOL    KillTimer(HWND, uintptr_t);

/* ---- 窗口钩子辅助 ---- */
BOOL    AttachThreadInput(DWORD, DWORD, BOOL);
void    SetWinEventHook(DWORD, DWORD, HMODULE, void *, DWORD, DWORD, DWORD);

/* ---- 系统消息 ---- */
UINT RegisterWindowMessageW(LPCWSTR);

/* ---- 托盘 ---- */
BOOL    Shell_NotifyIconW(DWORD, void *);

/* ---- 多显示器 ---- */
BOOL    EnumDisplaySettingsW(LPCWSTR, DWORD, void *);
LONG    ChangeDisplaySettingsExW(LPCWSTR, void *, HWND, DWORD, void *);

/* ---- 其他 ---- */
HANDLE  GetStdHandle(DWORD);
BOOL    SetConsoleTitleW(LPCWSTR);
void    SetConsoleCtrlHandler(void *, BOOL);
BOOL    DeviceIoControl(HANDLE, DWORD, LPVOID, DWORD, LPVOID, DWORD, DWORD *, void *);
HANDLE  CreateFileW(LPCWSTR, DWORD, DWORD, LPSECURITY_ATTRIBUTES, DWORD, DWORD, HANDLE);

/* ---- 对话框 ---- */
int MessageBoxA(HWND, LPCSTR, LPCSTR, uint32_t);
int MessageBoxW(HWND, LPCWSTR, LPCWSTR, uint32_t);
intptr_t DialogBoxParamW(HINSTANCE, LPCWSTR, HWND, void *, LPARAM);

/* ---- 崩溃处理 ---- */
#define __debugbreak() __asm__ __volatile__("int3")

/* ---- 宽字符串字面量 (Linux wchar_t=4B vs Windows WCHAR=2B) ---- */
/* L"..." 在 Linux 是 int[]; 小端下字节布局与 UTF-16LE 一致, 强转即可.
 * 注意: 仅用于常量字符串 (ASCII/UTF-16LE 内容). */
#define WSTR(s) ((const WCHAR *)(const void *)L##s)

/* ---- 批重构补充类型/API ---- */
typedef struct { SHORT X, Y; } COORD;
typedef struct {
    uint32_t Data1;
    uint16_t Data2;
    uint16_t Data3;
    uint8_t  Data4[8];
} GUID;

BOOL    VirtualProtect(LPVOID, DWORD, DWORD, DWORD *);
BOOL    CreateHardLinkW(LPCWSTR, LPCWSTR, LPSECURITY_ATTRIBUTES);
BOOL    AllocConsole(void);
HWND    GetConsoleWindow(void);
BOOL    SetConsoleScreenBufferSize(HANDLE, COORD);
LPWSTR  lstrcpynW(LPWSTR, LPCWSTR, int);
int     lstrcmpiA(LPCSTR, LPCSTR);
HKEY    SetupDiOpenClassRegKey(const GUID *, DWORD);

#endif /* WIN32_STUB_H */
