/* ====================================================================
 * core_proc.c — PECMD 进程/服务/动态导入/系统初始化工具
 *
 * 来源: PECMD原始.EXE (x64)
 *   FUN_14005C828     @0x14005c828   (动态导入缓存)
 *   FUN_14000531C    @0x14000531c   (返回指针版跳空白)
 *   FUN_14000546C @0x14000546c   (跳过引号/普通 token)
 *   PECMD_CheckDebugMsg      @0x140005344   (DEBUGMDG 环境变量)
 *   FUN_140008B2C   @0x140008b2c   (停止并删除服务)
 *   PECMD_ServiceEntry    @0x140017048   (服务分发器入口)
 *   PECMD_GetParentProcessId       @0x140006988   (NtQueryInformationProcess)
 *   PECMD_InitDynamicImports @0x140017908 (RegDeleteKeyExW/SH* 动态导入)
 *   PECMD_ImportSystemEnvVars     @0x14001c1d4   (环境变量 -> &变量)
 *   FUN_14001D628       @0x14001d628   (ntdll 动态导入)
 *   FUN_1400186BC      @0x1400186bc   (脚本结构初始化)
 *   FUN_14001A220         @0x14001a220   (QPC 频率带锁)
 *
 * .rdata 环境变量名表 @0x14013a260: SystemRoot/SystemDrive/windir/TEMP/TMP
 * ==================================================================== */
#include <stdint.h>
#include <stddef.h>
#include <string.h>

#include "pecmd_defs.h"

/* ---- 待重构函数原型 (后续批次) ---- */
void FUN_140053E78(void);            /* @0x140053e78 */
void PECMD_SetRdataTablePtr(void);            /* @0x14005d694 */
extern void PECMD_AppendLongDecimal(void *script, int64_t value, LPCWSTR key);  /* @0x1400669c4 设置变量 */
void PECMD_OpenSharedMapping(int a, const char *s);            /* @0x140005738 */
void PECMD_DetectCodePage(void);            /* @0x14000500c */
void FUN_1400629B8(void *s, LPCWSTR k, LPCWSTR v);   /* @0x1400629b8 */
void PECMD_InstallWindowsService(LPCWSTR cmd);     /* @0x140008834 执行命令文本 */
void FUN_140016ae0(void);            /* @0x140016ae0 服务主过程 */

/* ---- 全局 (core_globals.c) ---- */
extern uint8_t g_flag16b;            /* DAT_14013c96b (DEBUGMDG) */
extern int64_t (*g_pRegDeleteKeyExW)(HKEY, LPCWSTR, uint32_t, uint32_t);   /* DAT_14013d408 */
extern void (*g_pSHGetValueA)(void);       /* DAT_14013cd50 */
extern void (*g_pStrToIntExW)(void);       /* DAT_14013cd58 */
extern HMODULE g_hNtdll;             /* DAT_14013ccf8 */
extern void (*g_pNtOpenFile)(void);        /* DAT_14013cd18 */
extern void (*g_pNtCreateFile)(void);      /* DAT_14013cd20 */
extern void (*g_pNtReadFile)(void);        /* DAT_14013cd28 */
extern void (*g_pNtCreateDirectoryObject)(void); /* DAT_14013cd30 */
extern void (*g_pNtCreateSymbolicLinkObject)(void); /* DAT_14013cd38 */

/* ========== 动态导入 @0x14005c828 ========== */
/* *out = GetProcAddress(模块, name); 模块句柄缓存于 *hmod */
void FUN_14005C828(LPCSTR func, LPCSTR dll, void **out, HMODULE *hmod)
{
    HMODULE h;
    HMODULE local;

    if (*out != NULL) {
        return;
    }
    local = 0;
    if (hmod != NULL) {
        local = *hmod;
    }
    h = local;
    if (!h) {
        h = LoadLibraryA(dll);
        if (hmod != NULL) {
            *hmod = h;
        }
        if (!h) {
            *out = NULL;
            return;
        }
    }
    *out = (void *)GetProcAddress(h, func);
}

/* ========== 跳空白 (返回指针) @0x14000531c ========== */
WCHAR *FUN_14000531C(WCHAR *p)
{
    while ((*p > 8 && *p < 0xe) || *p == 0x20) {
        p++;
    }
    return p;
}

/* ========== 跳过引号/普通 token @0x14000546c ========== */
/* 引号开头: 扫到闭合引号 (返回引号后); 否则扫到空白 */
WCHAR *FUN_14000546C(WCHAR *p)
{
    WCHAR c;

    c = *p;
    if (c == L'"') {
        do {
            p++;
            if (*p == 0) {
                break;
            }
        } while (*p != L'"');
        if (*p == L'"') {
            p += 2;
        }
    } else {
        while (c != 0 && ((c < 9 || c > 0xd) && c != 0x20)) {
            p++;
            c = *p;
        }
    }
    return p;
}

/* ========== DEBUGMDG 检查 @0x140005344 ========== */
void PECMD_CheckDebugMsg(void)
{
    CHAR buf[120];

    buf[0] = '\0';
    GetEnvironmentVariableA("DEBUGMDG", buf, 99);
    if (buf[0] == '1') {
        g_flag16b = 1;
    }
}

/* ========== 删除服务 @0x140008b2c ========== */
void FUN_140008B2C(WCHAR *name)
{
    WCHAR *end;
    WCHAR nameBuf[1032];
    SC_HANDLE hMgr;
    SC_HANDLE hSvc;
    SERVICE_STATUS st;
    int len;

    end = name;
    while (*end != 0 && ((*end < 9 || *end > 0xd) && *end != 0x20)) {
        end++;
    }
    len = (int)(end - name);
    if (0x400 < len) {
        len = 0x400;
    }
    memcpy(nameBuf, name, (size_t)len * 2);
    nameBuf[len] = L'\0';
    end = FUN_14000531C(end);
    hMgr = OpenSCManagerW(NULL, NULL, SC_MANAGER_ALL_ACCESS);
    if (hMgr) {
        hSvc = OpenServiceW(hMgr, nameBuf, SERVICE_ALL_ACCESS);
        if (hSvc) {
            ControlService(hSvc, SERVICE_CONTROL_STOP, &st);
            Sleep(200);
            DeleteService(hSvc);
            CloseServiceHandle(hSvc);
            Sleep(200);
        }
    } else {
        GetLastError();
    }
    CloseServiceHandle(hMgr);
}

/* ========== 服务入口 @0x140017048 ========== */
void PECMD_ServiceEntry(LPCWSTR cmd)
{
    SERVICE_TABLE_ENTRYW table[2];

    memset(table, 0, sizeof(table));
    table[0].lpServiceName = (LPWSTR)WSTR("Service");
    table[0].lpServiceProc = (void *)FUN_140016ae0;
    StartServiceCtrlDispatcherW(table);
    PECMD_InstallWindowsService(cmd);
}

/* ========== 父进程 PID @0x140006988 ========== */
/* 通过 NtQueryInformationProcess 获取父进程 ID */
uint64_t PECMD_GetParentProcessId(DWORD pid)
{
    uint64_t info[5];
    HANDLE h;

    FUN_14005C828("NtQueryInformationProcess", "NTDLL.DLL",
                      (void **)&g_pNtQueryInfo, NULL);
    if (g_pNtQueryInfo != NULL) {
        memset(info, 0, 0x30);
        h = OpenProcess(0x400, 0, pid);   /* PROCESS_QUERY_INFORMATION */
        if (h) {
            if ((*g_pNtQueryInfo)(h, 0, info, 0x30, NULL) == 0) {
                CloseHandle(h);
                return info[4];            /* PROCESS_BASIC_INFORMATION.ParentProcessId @+0x20 */
            }
            CloseHandle(h);
            return 0;
        }
    }
    return 0;
}

/* ========== 动态导入初始化 @0x140017908 ========== */
void PECMD_InitDynamicImports(void)
{
    FUN_14005C828("RegDeleteKeyExW", "Advapi32.DLL",
                      (void **)&g_pRegDeleteKeyExW, NULL);
    if (!g_pSHDeleteKeyW) {
        FUN_14005C828("SHGetValueW", "ShLwAPI.DLL", (void **)&g_pSHGetValueW, NULL);
        FUN_14005C828("SHGetValueA", "ShLwAPI.DLL", (void **)&g_pSHGetValueA, NULL);
        FUN_14005C828("StrToIntExW", "ShLwAPI.DLL", (void **)&g_pStrToIntExW, NULL);
        FUN_14005C828("SHSetValueW", "ShLwAPI.DLL", (void **)&g_pSHSetValueW, NULL);
        FUN_14005C828("SHDeleteValueW", "ShLwAPI.DLL", (void **)&g_pSHDeleteValueW, NULL);
        FUN_14005C828("SHDeleteKeyW", "ShLwAPI.DLL", (void **)&g_pSHDeleteKeyW, NULL);
    }
}

/* ========== 环境变量 -> 内置变量 @0x14001c1d4 ========== */
/* .rdata 表 @0x14013a260: SystemRoot/SystemDrive/windir/TEMP/TMP */
void PECMD_ImportSystemEnvVars(void)
{
    static const char *const names[] = {
        "SystemRoot", "SystemDrive", "windir", "TEMP", "TMP"
    };
    WCHAR key[56];
    WCHAR val[2604];
    int i;

    for (i = 0; i < 5; i++) {
        const char *s = names[i];
        WCHAR *dst = key;
        val[0] = L'\0';
        key[0] = L'&';
        dst = key + 1;
        while (*s != '\0') {
            *dst = (WCHAR)(uint8_t)*s;
            s++;
            dst++;
        }
        *dst = L'\0';
        GetEnvironmentVariableW(key, val, 0xa28);
        FUN_1400629B8(&g_Script, key, val);
    }
}

/* ========== ntdll 动态导入 @0x14001d628 ========== */
int FUN_14001D628(void)
{
    if (!g_pZwOpenSection) {
        if (!g_hNtdll) {
            g_hNtdll = LoadLibraryW(WSTR("ntdll.dll"));
            if (!g_hNtdll) {
                return 0;
            }
        }
        g_pRtlInitUnicodeString = (void *)GetProcAddress(g_hNtdll, "RtlInitUnicodeString");
        g_pNtOpenFile = (void *)GetProcAddress(g_hNtdll, "NtOpenFile");
        g_pNtCreateFile = (void *)GetProcAddress(g_hNtdll, "NtCreateFile");
        g_pNtReadFile = (void *)GetProcAddress(g_hNtdll, "NtReadFile");
        g_pZwOpenSection = (void *)GetProcAddress(g_hNtdll, "ZwOpenSection");
        g_pNtClose = (void *)GetProcAddress(g_hNtdll, "NtClose");
        g_pNtCreateDirectoryObject = (void *)GetProcAddress(g_hNtdll, "NtCreateDirectoryObject");
        g_pNtCreateSymbolicLinkObject = (void *)GetProcAddress(g_hNtdll, "NtCreateSymbolicLinkObject");
    }
    return 1;
}

/* ========== QPC 频率 (带锁) @0x14001a220 ========== */
void FUN_14001A220(void)
{
    EnterCriticalSection((void *)&g_csInit);
    if (g_QPFreq == 0) {
        QueryPerformanceFrequency(&g_QPFreq);
    }
    /* 反编译 jumptable 警告: 实际为锁内初始化 + 退出, 已还原 */
    LeaveCriticalSection((void *)&g_csInit);
}
