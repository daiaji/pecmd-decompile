/* ====================================================================
 * core_execscript.c — PECMD 主入口 (MAIN 脚本执行)
 *
 * 来源: PECMD原始.EXE (x64)
 *   FUN_140045C90 @0x140045c90  (互斥体/注册表/脚本加载/消息循环)
 *
 * 流程:
 *   1. 解析命令行前缀 (--user/-user/ 星号星号 u / 星号星号 wait) -> 用户模式/wait 标志
 *   2. 互斥体 Global\PECMD:main(_u) 排他 (ERROR_ALREADY_EXISTS 直接退出)
 *   3. 非用户模式: PELOGON 注册表初始化 (PID/bInMain/CallBackhWnd)
 *   4. 参数表初始化 (script+0x78/+0x80, atexit 清理注册)
 *   5. 命令行非空: 脚本加载 (#!pecmd + --inline-main 检测 /
 *      构造 " --exe:... LOAD *sysinit ..." 命令串)
 *   6. 消息循环 GetMessageW/TranslateMessage/DispatchMessageW
 *
 * Ghidra 类型说明: 命令行 WCHAR* 游标被误标为 pthreadlocinfo*,
 *   字段字节偏移: refcount@0 / lc_codepage@4 / lc_time_cp@0xC / lc_category@0x10,
 *   即: p->refcount=p[0], &p->lc_codepage=p+2, &p->lc_time_cp+2=p+7,
 *       p->lc_category=(uint8_t*)p+0x10.
 * ==================================================================== */
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "pecmd_defs.h"

/* ---- 待重构函数原型 (后续批次, extern) ---- */
void PECMD_InitDynamicImports(void);                     /* @0x140017908 初始化 */
int FUN_1400660AC(const char *s, WCHAR **pp, int n);  /* @0x1400660ac 前缀词比较+推进 (非0=匹配) */
void FUN_14005e7dc(LPVOID *psd);              /* @0x14005e7dc 安全描述符初始化 */
void FUN_140026338(void *script, LPCWSTR path, int64_t flag);  /* @0x140026338 脚本初始化 */
void PECMD_DebugScriptString(void *script, LPCWSTR fmt, ...);  /* @0x140018c6c 调试日志 */
void PECMD_TlsLogWrite(void *script, LPCWSTR fmt, ...);  /* @0x140018d8c 调试日志 */
void PECMD_TruncateDebugLog(void);                     /* @0x140023544 */
void PECMD_InitRamdataRegistry(int n);                    /* @0x14001b888 */
void PECMD_FixKnownDlls32(void);                     /* @0x14002286c */
void PECMD_SwitchToDefaultDesktop(void);                     /* @0x140017724 */
void FUN_14002cc30(void *script, LPCWSTR s, int64_t a, int64_t b, LPCWSTR c); /* @0x14002cc30 */
void LoadEnvi(LPCWSTR a, LPCWSTR b);          /* @0x140069f0 环境初始化 */
void FUN_14005B9A0(void);                     /* @0x14005b9a0 */
void FUN_14003e1f0(void);                     /* @0x14003e1f0 */
DWORD PECMD_RegSetValueWithOpen(HKEY root, LPCWSTR sub, LPCWSTR name, DWORD type, BYTE *data, DWORD size); /* @0x14005c5a0 注册表值写入 */
void FUN_1400e8574(void *script, int flag);   /* @0x1400e8574 */
void PECMD_AllocStrSlot(WCHAR **ps);               /* @0x140063620 分配引用串容器 (16B 0xaa55 头) */
void PECMD_TokenizeQuotedField(void *script, WCHAR **p1, WCHAR **p2, uint64_t c, int64_t d); /* @0x1400545f8 路径解析 */
int FUN_14001ab84(LPCWSTR s);                 /* @0x14001ab84 命令行类型检测 */
void FUN_14001b850(void);                     /* @0x14001b850 */
void PECMD_LoadResourceLines(void);                     /* @0x14002e30c */
void PECMD_RegisterHotkeyEntry(void);                     /* @0x140022e94 */
void FUN_140077358(void);                     /* @0x140077358 */
void FUN_14005b228(int64_t (*cb)(void *), LPVOID arg, uint64_t stack, uint64_t flags,
                   DWORD *tid, LPSECURITY_ATTRIBUTES sa);  /* @0x14005b228 线程创建 */
void FUN_14004c0bc(uint64_t script, uint64_t cmd, void *p3, void *p4, void *p5); /* @0x14004c0bc 主脚本执行器 */
uint64_t FUN_14000e26c(uint64_t script, uint64_t cmd, uint64_t s3, uint64_t s4,
                       uint32_t flag, void *p6, uint64_t s7, void *p8);  /* @0x14000e26c 脚本执行 */

/* ---- 已实现引用 (core_exec2.c / core_exec4.c / core_exec5.c) ---- */
extern bool FUN_140101E70(LPCWSTR path);                  /* @0x140101e70 */
extern void PECMD_OpenFileHandle(HANDLE *out, LPCWSTR path, DWORD access, DWORD share,
                           LPSECURITY_ATTRIBUTES sa, DWORD disp, DWORD flags, HANDLE tmpl); /* @0x140003864 */
extern int32_t FUN_14005B1A8(const WCHAR *s, const WCHAR **pw, int n); /* @0x14005b1a8 */
extern void FUN_1400A4020(WCHAR **ps, LPCWSTR src);       /* @0x1400a4020 core_exec4.c */
extern uint32_t FUN_140073CCC(void *script, LPCWSTR cmdline, int saveArg); /* @0x140073ccc core_exec5.c */
extern void FUN_14006159C(void *script, uint64_t seed);   /* @0x14006159c core_exec2.c */

/* ---- 全局 (core_globals.c / 新声明) ---- */
extern uint32_t g_threadId;          /* g_dwC96C 线程 ID 输出 */
extern int64_t (*g_pRegDeleteKeyExW)(HKEY, LPCWSTR, uint32_t, uint32_t);  /* DAT_14013d408 动态导入 */
extern WCHAR g_szEmpty[];            /* g_szEmpty .rdata 空串 */
extern HANDLE g_hMainMutex;          /* DAT_14013cb18 主互斥体句柄 */
extern uint8_t g_logFlag;            /* g_u8CCB1 MAIN_DBG 日志标志 */
extern uint8_t g_flagD032;           /* DAT_14013d032 LOAD 模式标志 */
extern uint32_t g_initFlags;         /* DAT_14013d2a0 初始化标志 */
extern WCHAR *g_pMainArgStr;         /* DAT_14013d298 主参数字符串 (引用计数容器) */
extern int32_t g_flagD6f7;           /* g_u8D6F7 无命令行标志 */
extern HHOOK g_hMsgHook;             /* DAT_14013cf58 消息钩子 */
extern HWND g_hCallBackWnd;          /* g_hwndCF78 回调窗口句柄 */

/* 原 LAB_14011ab90 atexit 清理回调 (未反编译, 空占位 TODO(verify)) */
static void CleanupMainArg(void)
{
    /* TODO(verify): 原 atexit 注册 LAB_14011ab90, 清理函数体未反编译 */
}

/* 原 PECMD_DebugOutput 线程回调 (未反编译, 占位 TODO(verify)) */
static int64_t ThreadCb(void *p)
{
    (void)p;
    return 0;
}

/* ========== FUN_140045C90 @0x140045c90 ========== */
int64_t FUN_140045C90(void *script, LPCWSTR cmdline)
{
    WCHAR *p;               /* local_res10 命令行游标 */
    WCHAR *pSaved;          /* local_110/local_118 快照/路径游标 */
    WCHAR *pExe;            /* lpFilename 模块全路径 */
    WCHAR *pExeName;        /* ptVar14 模块文件名部分 */
    WCHAR *pContent;        /* local_108 文件内容缓冲 */
    WCHAR *pContent2;       /* local_118 内容游标 */
    WCHAR *pPath;           /* local_f8 引用串 (文件路径) */
    WCHAR *pTmp;            /* local_res18 复用槽 */
    WCHAR *pBuf;            /* local_100 大缓冲 */
    WCHAR *pArg;            /* local_70 (Ghidra 丢初始化 TODO(verify)) */
    WCHAR path[14];         /* local_6a (Ghidra 丢初始化 TODO(verify)) */
    MSG msg;                /* local_d0 */
    SECURITY_ATTRIBUTES sa; /* local_e8 */
    uint8_t sd[0x28];       /* local_a0/local_9f 安全描述符缓冲 */
    HANDLE hMutex;          /* pvVar9 互斥体 */
    HANDLE hFile;           /* local_f0 */
    DWORD nRead;            /* local_res18 (ReadFile 字节数) */
    HKEY hKey;              /* local_res18 (注册表键) */
    DWORD pid;              /* local_res20 */
    DWORD dwVal;            /* local_res20 */
    BYTE bInMain;           /* bInMain 注册表字节 */
    const char *mutexName;  /* lpName 互斥体名 */
    LPCWSTR pRegName;       /* pwVar18 注册表值名 */
    WCHAR first;            /* uVar5 首字符 */
    WCHAR sVar1;            /* sVar1 */
    WCHAR flagCh;           /* uVar13: '*' 或 '-' */
    WCHAR c;                /* WVar12 */
    bool bUser;             /* bVar2 用户模式 */
    bool bStar;             /* bVar19 首字符 '*' 标志 */
    char cWait;             /* cVar17 wait 模式 */
    uint64_t r;             /* uVar16 寄存器残留 */
    int i;                  /* iVar7 */
    int64_t *pS;            /* 输入数据视图 */

    cWait = 0;
    p = (WCHAR *)cmdline;
    pS = (int64_t *)script;
    PECMD_InitDynamicImports();
    g_flag16a = 0;
    first = p[0];
    flagCh = L'*';
    bStar = (first == L'*');
    /* 跳过第一个 token (非空白) */
    while (first != 0 && (first < 9 || first > 0xd) && first != 0x20) {
        p++;
        first = p[0];
    }
    FUN_14005B154(&p);            /* PECMD_SkipLeadingControls */
    bUser = false;
    if (p[0] == L'-') {
        flagCh = L'-';
    }
    /* ---- 参数循环: "-user"/"-u" 及 "**u"/"**wait"/"--user" 形式 ---- */
    for (;;) {
        while ((pSaved = p, FUN_1400660AC("-user", &p, 5) != 0 ||
                FUN_1400660AC("-u", &p, 2) != 0)) {
            bUser = true;
        }
        if (pSaved[0] != flagCh || pSaved[1] != flagCh) break;
        pSaved = pSaved + 2;        /* &lc_codepage (偏移 4 字节 = 2 WCHAR) */
        if (FUN_1400660AC("user", &pSaved, 4) == 0 &&
            FUN_1400660AC("u", &pSaved, 1) == 0) {
            if (FUN_1400660AC("wait", &pSaved, 4) == 0) break;
            cWait = 1;
        } else {
            bUser = true;
        }
        p = pSaved;
    }
    pRegName = WSTR("CallBackhWnd");
    if (bUser) {
        pRegName = WSTR("UCallBackhWnd");
    }
    /* ---- 互斥体排他 ---- */
    SetLastError(0);
    memset(sd + 1, 0, 0x27);
    sd[0] = 0;
    sa.lpSecurityDescriptor = sd;
    sa.nLength = 0x18;
    sa.bInheritHandle = 0;
    FUN_14005e7dc(&sa.lpSecurityDescriptor);
    mutexName = "Global\\PECMD:main";
    if (bUser) {
        mutexName = "Global\\PECMD:main_u";
    }
    hMutex = CreateMutexA(&sa, 1, mutexName);
    if (hMutex != (HANDLE)0 && GetLastError() == 0xb7) {
        return 0xb7;                /* ERROR_ALREADY_EXISTS */
    }
    /* ---- 运行状态 ---- */
    if (bUser) {
        g_runFlag = (uint32_t)L' ';
    } else {
        g_runFlag = (uint32_t)((p[0] != 0) + 0x10);
    }
    /* 反编译寄存器残留死代码省略:
     *   memcpy(local_78, L"LOGS * D:\\DebugPeMain2.log", 0x36);
     *   local_4e = 0x55;
     * (local_78/local_4e 无后续使用) */
    if (g_hMainMutex == (HANDLE)0 && FUN_140101E70(path)) {
        FUN_140026338(&g_Script, pArg, (int64_t)-1);
    }
    /* ---- 调试日志 ---- */
    if (g_logFlag != 0) {
        r = (uint64_t)(uint32_t)g_runFlag;
        PECMD_DebugScriptString(&g_Script, WSTR("{MAIN [%s]} 0x%X\r\n"), cmdline, r);
        if (g_logFlag != 0) {
            PECMD_DebugScriptString(&g_Script, WSTR("MAIN: pecmd::RegDeleteKeyExW=0x%p\r\n"),
                          (void *)g_pRegDeleteKeyExW, r);
        }
    }
    PECMD_TruncateDebugLog();
    if (bStar) {
        ((uint8_t *)&g_runFlag)[3] = 1;   /* DAT_140147003 = 1 */
    }
    if (!bUser) {
        if (p[0] == 0) goto after_init;
        PECMD_InitRamdataRegistry(3);
        r = 0;
        hKey = (HKEY)0;
        RegCreateKeyExW(HKEY_LOCAL_MACHINE, WSTR("SOFTWARE\\PELOGON"), 0, (LPWSTR)0, 0, 1,
                        (LPSECURITY_ATTRIBUTES)0, &hKey, (DWORD *)0);
        if (hKey != (HKEY)0) {
            RegCloseKey(hKey);
        }
        if (g_logFlag != 0) {
            PECMD_TlsLogWrite(&g_Script, WSTR("MAIN_DBG:%d\r\n"), 0x2f1c, r);
        }
        PECMD_FixKnownDlls32();
        if (g_logFlag != 0) {
            PECMD_TlsLogWrite(&g_Script, WSTR("MAIN_DBG:%d\r\n"), 0x2f1e, r);
        }
        PECMD_SwitchToDefaultDesktop();
        if (g_logFlag != 0) {
            PECMD_TlsLogWrite(&g_Script, WSTR("MAIN_DBG:%d\r\n"), 0x2f20, r);
        }
    }
    if (p[0] != 0) {
        FUN_14002cc30(script, g_szEmpty, 0, 0x24, (LPCWSTR)0);
    }
after_init:
    sVar1 = p[0];
    if (sVar1 == L'*' && p[1] != L'#') {
        p = p + 1;
        FUN_14005B154(&p);
    }
    LoadEnvi(g_szEmpty, g_szEmpty);
    if (p[0] == 0) {
        g_flagD6f7 = 1;
    }
    FUN_14005B9A0();
    FUN_14003e1f0();
    pid = GetCurrentProcessId();
    r = 4;
    PECMD_RegSetValueWithOpen(HKEY_LOCAL_MACHINE, WSTR("SOFTWARE\\PELOGON"), WSTR("MainPECMDPID"), 4,
                  (BYTE *)&pid, 4);
    if (g_logFlag != 0) {
        PECMD_TlsLogWrite(&g_Script, WSTR("MAIN_DBG:%d\r\n"), 0x2f2a, r);
    }
    FUN_1400e8574(&g_Script, 0);
    /* ---- 参数表初始化 (script+0x78/+0x80) ---- */
    if (pS[0xf] == 0) {
        if ((g_initFlags & 1) == 0) {
            g_initFlags |= 1;
            g_pMainArgStr = 0;
            atexit(CleanupMainArg);         /* 原注册 LAB_14011ab90 */
        }
        pS[0xf] = (int64_t)(uintptr_t)&g_pMainArgStr;
        FUN_14006159C(script, 0x10000);
        PECMD_AllocStrSlot(&pTmp);
        FUN_1400A4020(&g_pMainArgStr, (LPCWSTR)(uintptr_t)pTmp);
        pTmp = 0;
        pS[0x10] = (int64_t)(uintptr_t)*(WCHAR **)g_pMainArgStr;
        FUN_140073CCC(script, cmdline, 1);
        *(uint32_t *)((uint8_t *)script + 0x8c) = 0;
        FUN_14005B104(&pTmp);
    }
    if (g_logFlag != 0) {
        PECMD_TlsLogWrite(&g_Script, WSTR("MAIN_DBG:%d\r\n"), 0x2f36, r);
    }
    /* ---- 注册表: bInMain / CallBackhWnd ---- */
    if (!bUser) {
        bInMain = (BYTE)g_runFlag;
        r = 3;
        PECMD_RegSetValueWithOpen(HKEY_LOCAL_MACHINE, WSTR("SOFTWARE\\PELOGON"), WSTR("bInMain"), 3,
                      &bInMain, 1);
    }
    if (g_logFlag != 0) {
        PECMD_TlsLogWrite(&g_Script, WSTR("MAIN_DBG:%d\r\n"), 0x2f3a, r);
    }
    if (p[0] != 0) {
        FUN_14001b850();
    }
    r = 0xb;
    PECMD_RegSetValueWithOpen(HKEY_LOCAL_MACHINE, WSTR("SOFTWARE\\PELOGON"), pRegName, 0xb,
                  (BYTE *)&g_hCallBackWnd, 8);
    if ((sVar1 != 0) || bUser) {
        PECMD_LoadResourceLines();
    }
    PECMD_RegisterHotkeyEntry();
    FUN_140077358();
    if ((p[0] != 0) || bUser) {
        if (g_logFlag != 0) {
            PECMD_TlsLogWrite(&g_Script, WSTR("MAIN_DBG:%d\r\n"), 0x2f45, r);
        }
        FUN_14005b228(ThreadCb, (LPVOID)0, 0x80000, 0x10000, &g_threadId,
                      (LPSECURITY_ATTRIBUTES)0);
    }
    r = 0x2a;
    if (p[0] == L'*') {
        do {
            if (p[1] == L'#') break;
            p = p + 1;
            FUN_14005B154(&p);
        } while (p[0] == L'*');
    }
    /* ---- 命令行非空: 加载脚本 ---- */
    if (p[0] != 0) {
        i = lstrlenW(p);
        PECMD_AllocWStringBuffer(&pBuf, (int64_t)(i + 0x5c8));
        pBuf[0] = 0;
        pExe = pBuf + 0x140;
        GetModuleFileNameW((HMODULE)0, pExe, 0x104);
        pSaved = StrRChrW(pExe, (LPCWSTR)0, L'\\');
        pExeName = pExe;
        if (pSaved != (LPWSTR)0) {
            pExeName = pSaved + 1;
        }
        i = FUN_14001ab84(p);
        if (i == 0) {
            /* ---- 文件模式: 读取内容, 检测 #!pecmd / --inline-main ---- */
            bStar = false;
            pSaved = p;                 /* 命令行快照 */
            PECMD_AllocStrSlot(&pPath);
            PECMD_TokenizeQuotedField(script, &pSaved, &pPath, 0x1000, 0);
            PECMD_AllocWStringBuffer(&pContent, 0x400);
            pContent[0] = 0;
            pContent2 = pContent;
            nRead = 0;
            hFile = (HANDLE)0;
            PECMD_OpenFileHandle(&hFile, pPath, 0x80000000, 7, (LPSECURITY_ATTRIBUTES)0, 3, 0,
                           (HANDLE)0);
            ReadFile(hFile, pContent, 0x7fe, &nRead, (void *)0);
            pContent2[nRead >> 1] = 0;  /* 终止符 (原 lc_category 偏移运算) */
            if (pContent[0] == 0xFEFF) {
                pContent2 = pContent2 + 1;   /* 跳过 UTF-16 BOM */
            }
            i = StrCmpNIW(WSTR("#!pecmd"), pContent2, 7);
            if (i == 0) {
                pContent2 = pContent2 + 7;   /* &lc_time_cp + 2 (偏移 0xC+2 字节 = 7 WCHAR) */
                do {
                    c = pContent2[0];
                    if (c == 0) goto close_file;
                    do {
                        if ((c > 8 && c < 0xe) || c == L' ') break;
                        pContent2++;
                        c = pContent2[0];
                    } while (c != 0);
                    for (; pContent2[0] == L' ' || pContent2[0] == L'\t'; pContent2++) {
                    }
                    if (pContent2[0] == L'\n' || pContent2[0] == L'\r') goto close_file;
                    r = (uint64_t)FUN_14005B1A8(WSTR("--inline-main"),
                                                   (const WCHAR **)&pContent2, 13);
                } while (r == 0);
                bStar = true;           /* 发现 --inline-main */
            }
        close_file:
            if (hFile != (HANDLE)0 && hFile != (HANDLE)(uintptr_t)-1) {
                CloseHandle(hFile);
            }
            FUN_14005B104(&pContent);
            FUN_14005B104(&pPath);
            if (bStar) {
                wsprintfW(pBuf + 900, WSTR("LOAD *sysinit %s"), p, (void *)&nRead);
                g_flagD032 = 1;
                FUN_14004c0bc((uint64_t)(uintptr_t)script, (uint64_t)(uintptr_t)(pBuf + 900),
                              (void *)0, (void *)0, (void *)0);
            } else {
                wsprintfW(pBuf + 900, WSTR(" --exe:\"%s\"  \"%s\"  "), pExe, pExeName);
                i = lstrlenW(pBuf + 900);
                wsprintfW(pBuf + 900 + i, WSTR("LOAD *sysinit %s"), p, pExeName);
                if (pBuf[0] != 0) {     /* 反编译恒假分支, 原样保留 */
                    PECMD_InitRamdataRegistry(1);
                    dwVal = 1;
                    PECMD_RegSetValueWithOpen(HKEY_LOCAL_MACHINE, WSTR("SOFTWARE\\PELOGON\\RAMDATA"),
                                  WSTR("SysStartuped"), 4, (BYTE *)&dwVal, 4);
                }
                FUN_14000e26c((uint64_t)(uintptr_t)script, (uint64_t)(uintptr_t)(pBuf + 900),
                              (uint64_t)(uintptr_t)script, 0, (uint32_t)cWait,
                              (void *)0, 0, (void *)0);
            }
        } else {
            /* ---- 命令行直接执行 (LOAD 前缀检测) ---- */
            FUN_1400702B0(&pTmp, p);
            r = (uint64_t)FUN_14005C788("LOAD", p, 4);
            if (r != 0) {
                g_flagD032 = 1;
            }
            FUN_14004c0bc((uint64_t)(uintptr_t)script, (uint64_t)(uintptr_t)pTmp,
                          (void *)0, (void *)0, (void *)0);
            FUN_14005B104(&pTmp);
        }
        FUN_14005B104(&pBuf);
    }
    /* ---- 消息循环 ---- */
    while (GetMessageW(&msg, (HWND)0, 0, 0) != 0) {
        TranslateMessage(&msg);
        DispatchMessageW(&msg);
    }
    UnhookWindowsHookEx(g_hMsgHook);
    return (int64_t)(int)msg.wParam;
}
