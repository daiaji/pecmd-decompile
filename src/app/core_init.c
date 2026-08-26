/* ====================================================================
 * core_init.c — PECMD 初始化/收尾/脚本结构管理
 *
 * 来源: PECMD原始.EXE (x64)
 *   PECMD_ExitProcessCall @0x14005b21c   (ExitProcess 封装)
 *   PECMD_GetExitCodeGlobal @0x14005b7dc   (返回 *g_pExitCode)
 *   PECMD_InitEnvironmentVars     @0x140027690   (环境变量/内置变量初始化)
 *   PECMD_BroadcastEnvChange   @0x14002ca30   (收尾钩子)
 *   FUN_14004EAA8 @0x14004eaa8   (脚本项数组清理)
 *   FUN_1400637DC @0x1400637dc   (ANSI->Unicode 表复制)
 *
 * 注意: PECMD_SkipLeadingControls / PECMD_HeapFreeWithHeader 与 FUN_14005B154 /
 *       PECMD_FreeStrBuf 字节级相同 (core_string.c), 不重复实现.
 * ==================================================================== */
#include <stdint.h>
#include <stddef.h>

#include "pecmd_defs.h"

/* ---- 待重构函数原型 (后续批次) ---- */
void PECMD_InitDynamicImports(void);                        /* @0x140017908 初始化 */
void FUN_140053E78(void);                                   /* @0x140053e78 */
void FUN_14001D628(void);                                   /* @0x14001d628 */
void FUN_1400186BC(void *s, int a);                         /* @0x1400186bc */
void PECMD_InitSystemApiGlobals(void);                      /* @0x1400083c0 */
extern void PECMD_InitPerfCounterFreq(void);                /* @0x14001a220 */
void FUN_1400629B8(void *s, LPCWSTR k, LPCWSTR v);          /* @0x1400629b8 设置内置变量 */
void PECMD_SetVarAndEnvSync(void *s, LPCWSTR k, LPCWSTR v); /* @0x140062a2c 设置变量 */
void PECMD_VarSetUInt(void *s, uint64_t v, LPCWSTR k);      /* @0x140066978 设置数值变量 */
extern void PECMD_AppendFmtValue(void *script, uint64_t value, LPCWSTR key,
                                 LPCWSTR fmt);                   /* @0x1400668ec */
uint64_t PECMD_GetParentProcessId(DWORD pid);                    /* @0x140006988 父进程 PID */
extern void PECMD_AllocWStringBuffer(WCHAR **ps, int64_t count); /* @0x140063694 分配 */
void PECMD_ImportSystemEnvVars(void);                            /* @0x14001c1d4 */
void PECMD_InitShellFolderEnvVars(void);                         /* @0x14001c01c */
void PECMD_GetTaskbarCreatedMsg(void);                           /* @0x14005b9a0 */
extern uint64_t PECMD_StartWorkerThread(void *script, void **pref, uint32_t a3, uint64_t a4,
                                        uint64_t a5, uint32_t a6, uint64_t a7, int64_t a8,
                                        int a9);   /* @0x140027eac */
void FUN_14004E2CC(void *s, void *item);           /* @0x14004e2cc 释放单项 */
void FUN_14006E8F4(void *s);                       /* @0x14006e8f4 清理结构 */
void PECMD_CheckDebugMsg(void);                    /* @0x140005344 */
WCHAR *PECMD_AllocString(WCHAR **ps, int64_t count); /* @0x140063720 串扩容(S11 归正返回型, 原误标 void/063620) */
void FUN_14007A224(void *s, LPCWSTR a, LPCWSTR *b, int c, int d); /* @0x14007a224 */
extern void *PECMD_StrBldCopyWide(void *a, const WCHAR *b);       /* @0x1400703e4 */
WCHAR *FUN_14000531C(WCHAR *p);                      /* @0x14000531c 跳空白(返回指针) */
int FUN_1400660AC(const char *s, WCHAR **pp, int n); /* @0x1400660ac 前缀词比较 */
void FUN_140008B2C(WCHAR *p);                        /* @0x140008b2c RemoveService */
void PECMD_ServiceEntry(WCHAR *p);                   /* @0x140017048 执行命令 */

/* ---- 全局 (core_globals.c) ---- */
extern uint32_t *g_pExitCode;                                         /* DAT_14013d180 退出码指针 */
extern int (*g_pConvFunc)(uint32_t, DWORD, LPCSTR, int, LPWSTR, int); /* DAT_14013c970 */
extern uint32_t g_SysCodePage;                                        /* DAT_140147004 */
extern uint32_t g_ShowWindow;                                         /* DAT_14013a234 */

/* ========== ExitProcess 封装 @0x14005b21c ========== */
static void PECMD_ExitProcessCall(int code)
{
    ExitProcess((UINT)code);
}

/* ========== 退出码 @0x14005b7dc ========== */
/* R14b(S17 UNVERIFIABLE 解锁, 活体 dd 实锤): 原版 DAT_14013d180≡g_Script+0x50
 * 同址, ScriptInit(parent==0) 自锚槽值=&this+0x58, PSB 尾部(rb LAB_14004c525 /
 * dc:45139-141)把最后非零动词返回值写进 *(script+0x50) ⇒ 进程退出码。
 * v0 读独立 g_exitCodeCache(无写入者)恒 0 —— golden=2 的 17 案全败根因。
 * WRITE→2 / ENVI,CALC,SUB,TEAM→0 / CALC 除0→16 与触发面表完全吻合。 */
int PECMD_GetExitCodeGlobal(void)
{
    return (int)*(uint32_t *)(*(uint64_t *)(void *)(g_Script + 0x50));
}

/* ========== 初始化 @0x140027690 ========== */
/* 设置内置变量: PECMDVER/&PECMDVER/&PECMDBUILD/&&ERROR/&WM_TaskbarRestart/
 * &__PID/&__PPID/&__HINST/&__TID/&SYSCODEPAGE/&&CurDir/&&_CD/&&CurDrv/&&CurFile */
void PECMD_InitEnvironmentVars(HINSTANCE hInstance, int show)
{
    WCHAR *pVer; /* local_res18[0] */
    WCHAR *pBuf; /* local_res8 */
    WCHAR buf[0x208];
    DWORD pid;

    if (!g_hHeap) {
        g_hHeap = GetProcessHeap();
    }
    PECMD_InitDynamicImports();
    FUN_140053E78();
    FUN_14001D628();
    FUN_1400186BC(&g_Script, 0);
    PECMD_InitSystemApiGlobals();
    PECMD_InitPerfCounterFreq();
    QueryPerformanceCounter(&g_QPC);
    GetSystemTimeAsFileTime((FILETIME *)&g_StartTime);
    /* thunk_PECMD_UpdateLcg = 性能计时, 忽略 */
    /* Ghidra 丢失 wsprintfW 参数 (寄存器残留), TODO(verify):
     * PECMDVER 由 QPC/启动时间生成, 原样保留格式串 */
    wsprintfW(buf, WSTR("%06lu%06lu")); /* TODO(verify): 参数丢失 */
    FUN_1400629B8(&g_Script, WSTR("PECMDVER"), buf);
    FUN_1400629B8(&g_Script, WSTR("&PECMDVER"), buf);
    FUN_1400702B0(&pVer, WSTR("AMD64 U "));
    FUN_14006375C(&pVer, PTR_u_2024_05_12_14013a000); /* .rdata 版本日期串 */
    FUN_1400629B8(&g_Script, WSTR("&PECMDBUILD"), pVer);
    PECMD_SetVarAndEnvSync(&g_Script, WSTR("&&ERROR"), WSTR("0"));
    /* thunk_PECMD_UpdateLcg(0x3a0c) 计时忽略 */
    PECMD_InitShellFolderEnvVars();
    if (!g_hInstance) {
        g_hInstance = hInstance;
    }
    g_ShowWindow = (uint32_t)show;
    g_hInst = hInstance;
    PECMD_GetTaskbarCreatedMsg();
    PECMD_VarSetUInt(&g_Script, g_WM_TaskbarRestart, WSTR("&WM_TaskbarRestart"));
    pid = GetCurrentProcessId();
    PECMD_VarSetUInt(&g_Script, pid, WSTR("&__PID"));
    pid = GetCurrentProcessId();
    PECMD_VarSetUInt(&g_Script, (uint64_t)PECMD_GetParentProcessId(pid), WSTR("&__PPID"));
    PECMD_AppendFmtValue(&g_Script, (uint64_t)(uintptr_t)g_hInst, WSTR("&__HINST"), WSTR("%I64u"));
    g_Tid = GetCurrentThreadId();
    PECMD_VarSetUInt(&g_Script, g_Tid, WSTR("&__TID"));
    pid = GetTickCount();
    /* thunk_PECMD_UpdateLcg(pid + 0x3a17) 计时忽略 */
    PECMD_VarSetUInt(&g_Script, g_SysCodePage, WSTR("&SYSCODEPAGE"));
    PECMD_AllocWStringBuffer((WCHAR **)&pBuf, 0x20a);
    GetCurrentDirectoryW(0x208, pBuf);
    PECMD_SetVarAndEnvSync(&g_Script, WSTR("&&CurDir"), pBuf);
    PECMD_SetVarAndEnvSync(&g_Script, WSTR("&&_CD"), pBuf);
    pBuf[2] = L'\0'; /* 截断为盘符 */
    PECMD_SetVarAndEnvSync(&g_Script, WSTR("&&CurDrv"), pBuf);
    pBuf[0] = L'\0';
    GetModuleFileNameW((HMODULE)0, pBuf, 0x208);
    PECMD_SetVarAndEnvSync(&g_Script, WSTR("&&CurFile"), pBuf);
    PECMD_ImportSystemEnvVars();
    PECMD_FreeStrBuf(&pBuf);
    PECMD_FreeStrBuf(&pVer);
}

/* ========== 收尾钩子 @0x14002ca30 ========== */
void PECMD_BroadcastEnvChange(void)
{
    extern uint32_t g_afterMain; /* DAT_14013ccc8 */
    g_afterMain = 0;
    PECMD_StartWorkerThread(&g_Script, NULL, 0x1a, 0, (uint64_t)0x140121020, 5000, 1, 1,
                            0); /* .rdata 回调表 */
}

/* ========== 脚本项数组清理 @0x14004eaa8 ========== */
/* script 结构: +0 = 项数组指针, +8 = 项数 */
static void FUN_14004EAA8(void *script, int keep)
{
    int count;

    if (keep < 0) {
        keep = 0;
    }
    if (*(void **)script != NULL) {
        count = *(int *)((uint8_t *)script + 8);
        while (keep < count) {
            *(int *)((uint8_t *)script + 8) = *(int *)((uint8_t *)script + 8) - 1;
            void **arr = *(void ***)script;
            if (arr[count - 1] != NULL) {
                FUN_14004E2CC(script, arr[count - 1]);
                arr[count - 1] = NULL;
            }
            count = *(int *)((uint8_t *)script + 8);
        }
        if (*(int *)((uint8_t *)script + 8) < 1) {
            void **arr = *(void ***)script;
            if (arr != NULL) {
                HeapFree(g_hHeap, 0, (uint8_t *)arr - 8);
            }
            *(void **)script = NULL;
        }
    }
    FUN_14006E8F4(script);
}

/* ========== ANSI->Unicode 表复制 @0x1400637dc ========== */
/* *ps = 转换 src (ANSI 串) 为宽字符; 长度 srclen(-1=自动),
 * 代码页 codepage(-1=系统代码页 g_SysCodePage) */
static WCHAR *FUN_1400637DC(WCHAR **ps, LPCSTR src, int64_t srclen, int64_t codepage)
{
    int n;
    int len;

    if (codepage < 0) {
        codepage = g_SysCodePage;
    }
    if (srclen < 0) {
        srclen = (int64_t)lstrlenA(src);
    }
    PECMD_AllocString(ps, srclen + 3);
    n = (*g_pConvFunc)((uint32_t)codepage, 0, src, (int)srclen, *ps,
                       (int)srclen + 2); /* g_pConvFunc = MultiByteToWideChar */
    len = (n < 0) ? 0 : n;
    (*ps)[len] = L'\0';
    PECMD_AllocString(ps, len + 1);
    return *ps;
}
