/* ====================================================================
 * core_sys.c — PECMD 系统级工具 (代码页/共享映射/路径变量/注册表/权限)
 *
 * 来源: PECMD原始.EXE (x64)
 *   PECMD_DetectCodePage   @0x14000500c  (LC_ALL/GetLocaleInfoW 代码页检测)
 *   PECMD_OpenSharedMapping      @0x140005738  (命名共享内存映射)
 *   PECMD_InitShellFolderEnvVars   @0x14001c01c  (&PE_IDBASE/&MyName/&Desktop 等)
 *   FUN_140053E78          @0x140053e78  (空函数)
 *   PECMD_GetTaskbarCreatedMsg @0x14005b9a0 (RegisterWindowMessageW TaskbarCreated)
 *   PECMD_SetRdataTablePtr     @0x14005d694  (设置 .rdata 表指针)
 *   FUN_140064A88           @0x140064a88  (宽字符十进制解析)
 *   PECMD_LookupShellFolderVar  @0x14001bf20  (特殊目录 -> &变量)
 *   PECMD_RegValueToVar  @0x14001bea8  (注册表值 -> 环境变量/&变量)
 *   FUN_14005C4E0       @0x14005c4e0  (注册表查询带权限重试)
 *   FUN_14005C394        @0x14005c394  (RegCreateKeyExW 封装)
 *   FUN_14001C2CC @0x14001c2cc (令牌权限启用)
 *   PECMD_StrBldCopyWide       @0x1400703e4  (= PECMD_StrCopyW 别名)
 * ==================================================================== */
#include <stdint.h>
#include <stddef.h>
#include <wchar.h>
#include <string.h>

#include "pecmd_defs.h"
static WCHAR *PECMD_StrBldCopyWide(WCHAR **ps, LPCWSTR src);         /* 前置(首用先于定义) */
extern WCHAR **FUN_14005B154(WCHAR **pp);                            /* @0x14005b154 */
extern void FUN_1400629B8(void *script, LPCWSTR key, LPCWSTR value); /* @0x1400629b8 */

/* 前向声明 (定义在下方, 调用在前) */
WCHAR *PECMD_StrCopyW(WCHAR **ps, LPCWSTR src, int64_t len);
int64_t FUN_140064A88(const WCHAR *p);
DWORD FUN_14005C4E0(HKEY root, LPCWSTR subkey, LPCWSTR name, DWORD *type, BYTE *data, DWORD *size);
DWORD FUN_14005C394(HKEY root, LPCWSTR subkey, HKEY *out, REGSAM access, uint32_t opt);
int64_t FUN_14001C2CC(LPCWSTR priv, DWORD attr, uint32_t flag);


/* ---- 待重构函数原型 (后续批次) ---- */
void PECMD_EnablePrivilege_impl(void); /* 本文件实现 */

/* ---- 全局 (core_globals.c) ---- */
extern WCHAR *g_pLocale;   /* DAT_14013ca70 代码页字符串 */
extern void *g_pSharedMap; /* DAT_14013ca30 共享映射 */
extern uint8_t g_flag13f;  /* DAT_14013d13f */
extern void *g_pVtblA;     /* DAT_14013d670 */
extern void *g_pVtblB;     /* DAT_14013d5e0 */
extern int32_t g_val668;   /* DAT_14013d668 = 0xffffff9c */
extern int32_t g_val5d8;   /* DAT_14013d5d8 = 0xffffff9c */

/* 特殊目录变量表 (.rdata @0x140121020 附近, TODO(verify): 见 PECMD_StartWorkerThread) */

/* ========== 代码页检测 @0x14000500c ========== */
void PECMD_DetectCodePage(void)
{
    WCHAR buf[40];
    LPCWSTR cp;

    buf[0] = L'\0';
    GetEnvironmentVariableW(WSTR("LC_ALL"), buf, 0x20);
    if (buf[0] == L'\0') {
        GetLocaleInfoW(0x800, 0xb, buf,
                       0x20); /* LOCALE_SYSTEM_DEFAULT, LOCALE_IDEFAULTANSICODEPAGE */
    }
    cp = WSTR("936"); /* 默认 GBK */
    if (buf[0] != L'\0') {
        cp = buf;
    }
    PECMD_StrBldCopyWide(&g_pLocale, cp);
    if (FUN_140064A88(g_pLocale) > 1) {
        g_SysCodePage = (uint32_t)FUN_140064A88(g_pLocale);
    }
}

/* ========== 共享映射 @0x140005738 ========== */
/* 命名映射文件 (100 字节), 首字写 tag; 已存在则复用 */
void PECMD_OpenSharedMapping(uint64_t tag, LPCSTR name)
{
    HANDLE hMap;
    BOOL created;
    DWORD err;

    SetLastError(0);
    hMap = CreateFileMappingA((HANDLE)(uintptr_t)-1, NULL, 0x8000004, 0, 100, name);
    created = TRUE;
    if (!hMap) {
        hMap = OpenFileMappingA(0x8000004, 0, name);
        if (!hMap) {
            return;
        }
    }
    else {
        err = GetLastError();
        if (err == 0xb7) { /* ERROR_ALREADY_EXISTS */
            created = FALSE;
        }
    }
    g_pSharedMap = MapViewOfFile(hMap, 6, 0, 0, 0);
    CloseHandle(hMap);
    if (g_pSharedMap != NULL) {
        if (created) {
            memset(g_pSharedMap, 0, 100);
        }
        *(uint64_t *)g_pSharedMap = tag;
    }
}

/* ========== 路径变量初始化 @0x14001c01c ========== */
void PECMD_InitShellFolderEnvVars(void)
{
    WCHAR *pBuf;
    WCHAR *lpFilename;
    LPWSTR p;

    g_flag13f = 1;
    PECMD_AllocWStringBuffer((WCHAR **)&pBuf, 0x411);
    memset(pBuf, 0, 0x822);
    lpFilename = pBuf + 0x82;
    GetModuleFileNameW((HMODULE)0, lpFilename, 0x104);
    PECMD_AppendLongDecimal(&g_Script, 0x1000, WSTR("&PE_IDBASE"));
    PECMD_AppendLongDecimal(&g_Script, 0x5000, WSTR("&WM_PE_BASE"));
    PECMD_AppendLongDecimal(&g_Script, 0x100, WSTR("&PE_MENU_IDBASE"));
    SetEnvironmentVariableW(WSTR("MyName"), lpFilename);
    FUN_1400629B8(&g_Script, WSTR("&MyName"), lpFilename);
    p = StrRChrW(lpFilename, NULL, L'\\');
    if (p != NULL) {
        SetEnvironmentVariableW(WSTR("MyName0"), p + 1);
        FUN_1400629B8(&g_Script, WSTR("&MyName0"), p + 1);
    }
    PECMD_LookupShellFolderVar(WSTR("&Desktop"), NULL, NULL);
    PECMD_LookupShellFolderVar(WSTR("&Favorites"), NULL, NULL);
    PECMD_LookupShellFolderVar(WSTR("&Programs"), NULL, NULL);
    PECMD_LookupShellFolderVar(WSTR("&SendTo"), NULL, NULL);
    PECMD_LookupShellFolderVar(WSTR("&Personal"), NULL, NULL);
    PECMD_LookupShellFolderVar(WSTR("&StartMenu"), WSTR("Start Menu"), NULL);
    PECMD_LookupShellFolderVar(WSTR("&IECache"), WSTR("Cache"), NULL);
    PECMD_LookupShellFolderVar(WSTR("&Startup"), NULL, NULL);
    PECMD_LookupShellFolderVar(WSTR("&QuickLaunch"), WSTR("AppData"),
                               WSTR("\\Microsoft\\Internet Explorer\\Quick Launch"));
    PECMD_FreeStrBuf(&pBuf);
}

/* ========== 空函数 @0x140053e78 ========== */
void FUN_140053E78(void)
{
    return;
}

/* ========== TaskbarCreated 注册 @0x14005b9a0 ========== */
void PECMD_GetTaskbarCreatedMsg(void)
{
    if (g_WM_TaskbarRestart == 0) {
        g_WM_TaskbarRestart = (uint32_t)RegisterWindowMessageW(WSTR("TaskbarCreated"));
    }
}

/* ========== 设置 .rdata 表指针 @0x14005d694 ========== */
void PECMD_SetRdataTablePtr(void)
{
    g_pVtblA = (void *)0x14011c638; /* g_szEmpty (.rdata) */
    g_pVtblB = (void *)0x14011c638;
    g_val668 = 0xffffff9c;
    g_val5d8 = 0xffffff9c;
}

/* ========== 宽字符十进制解析 @0x140064a88 ========== */
int64_t FUN_140064A88(const WCHAR *p)
{
    int64_t v = 0;

    while ((WCHAR)(*p - L'0') < 10) {
        v = (int64_t)(*p - L'0') + v * 10;
        p++;
    }
    return v;
}

/* ========== 特殊目录变量 @0x14001bf20 ========== */
/* var: &变量名; env: 环境变量名 (NULL=变量名去 &);
 * 环境变量 -> &变量; 无环境变量 -> 注册表 User Shell Folders -> 展开 */
void PECMD_LookupShellFolderVar(LPCWSTR var, LPCWSTR env, const WCHAR *suffix)
{
    WCHAR *pBuf;
    LPWSTR buf;
    LPCWSTR name;
    DWORD type;
    DWORD size;
    DWORD r;

    PECMD_AllocWStringBuffer((WCHAR **)&pBuf, 0xc11);
    memset(pBuf, 0, 0x822);
    buf = pBuf + 0x208;
    *buf = L'\0';
    name = var + 1;
    if (env != NULL) {
        name = env;
    }
    size = 0x410;
    type = 0;
    GetEnvironmentVariableW(name, (LPWSTR)buf, 0x104);
    if (*buf == L'\0') {
        r = FUN_14005C4E0(
            (HKEY)0xffffffff80000001, /* HKEY_CURRENT_USER */
            WSTR("Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\User Shell Folders"),
            name, &type, (BYTE *)buf, &size);
        if (r == 0) {
            PECMD_RegValueToVar(var, (int)type, buf, pBuf, suffix);
        }
    }
    else {
        FUN_1400629B8(&g_Script, var, buf);
    }
    PECMD_FreeStrBuf(&pBuf);
}

/* ========== 注册表值 -> 变量 @0x14001bea8 ========== */
void PECMD_RegValueToVar(LPCWSTR var, int type, LPCWSTR value, LPWSTR tmp, const WCHAR *suffix)
{
    if (type != 1) {     /* REG_SZ */
        if (type != 2) { /* REG_EXPAND_SZ */
            return;
        }
        ExpandEnvironmentStringsW(value, tmp, 0x104);
        value = tmp;
    }
    if (suffix != NULL) {
        wcscat((wchar_t *)value, (const wchar_t *)suffix);
    }
    SetEnvironmentVariableW(var + 1, value);
    FUN_1400629B8(&g_Script, var, value);
}

/* ========== 注册表查询 (带权限重试) @0x14005c4e0 ========== */
DWORD FUN_14005C4E0(HKEY root, LPCWSTR subkey, LPCWSTR name, DWORD *type, BYTE *data, DWORD *size)
{
    HKEY hKey;
    uint32_t opt;
    DWORD r;

    hKey = 0;
    opt = 4; /* REG_OPTION_BACKUP_RESTORE */
    for (;;) {
        r = FUN_14005C394(root, subkey, &hKey, 0x20019, opt);
        if (r != 5) { /* ERROR_ACCESS_DENIED */
            if (r != 0) {
                return r;
            }
            r = RegQueryValueExW(hKey, name, NULL, type, data, size);
            RegCloseKey(hKey);
            if (r != 5) {
                return r;
            }
        }
        if (opt == 0) {
            return r;
        }
        opt = 0;
    }
}

/* ========== RegCreateKeyExW 封装 @0x14005c394 ========== */
DWORD FUN_14005C394(HKEY root, LPCWSTR subkey, HKEY *out, REGSAM access, uint32_t opt)
{
    DWORD r;
    DWORD disp;

    FUN_14001C2CC(WSTR("SeBackupPrivilege"), 2, 1); /* SE_PRIVILEGE_ENABLED */
    FUN_14001C2CC(WSTR("SeRestorePrivilege"), 2, 0x10);
    disp = 0;
    r = RegCreateKeyExW(root, subkey, 0, NULL, opt, 0xf003f, NULL, out, &disp);
    if (!*out) {
        if ((opt & 4) != 0) {
            r = RegCreateKeyExW(root, subkey, 0, NULL, opt & 0xfffffffb, 0xf003f, NULL, out, &disp);
        }
        if (!*out) {
            r = RegCreateKeyExW(root, subkey, 0, NULL, 0, access, NULL, out, &disp);
            if (!*out && r == 0) {
                r = GetLastError();
                if (r == 0) {
                    r = 1;
                }
            }
        }
    }
    return r;
}

/* ========== 令牌权限启用 @0x14001c2cc ========== */
int64_t FUN_14001C2CC(LPCWSTR priv, DWORD attr, uint32_t flag)
{
    HANDLE hToken;
    HANDLE hProc;
    BOOL ok;
    int64_t ret;
    struct {
        uint32_t PrivilegeCount;
        uint64_t Luid;
        uint32_t Attributes;
    } tp;

    EnterCriticalSection(&g_csInit);
    if ((flag & g_privFlags) == 0) {
        ret = 0;
        hToken = 0;
        hProc = GetCurrentProcess();
        ok = OpenProcessToken(hProc, 0x28, &hToken); /* TOKEN_ADJUST_PRIVILEGES|TOKEN_QUERY */
        if (ok) {
            tp.PrivilegeCount = 1;
            tp.Attributes = attr;
            LookupPrivilegeValueW(NULL, priv, &tp.Luid);
            ok = AdjustTokenPrivileges(hToken, 0, (void *)&tp, 0x10, NULL, NULL);
            if (ok && flag != 0) {
                g_privFlags = (uint16_t)(g_privFlags | (uint16_t)flag);
            }
            ret = (int64_t)ok;
        }
        LeaveCriticalSection(&g_csInit);
    }
    else {
        LeaveCriticalSection(&g_csInit);
        ret = 0;
    }
    return ret;
}

/* ========== StrCpyW 自动长度 @0x1400703e4 ========== */
static WCHAR *PECMD_StrBldCopyWide(WCHAR **ps, LPCWSTR src)
{
    return PECMD_StrCopyW(ps, src, -1);
}
