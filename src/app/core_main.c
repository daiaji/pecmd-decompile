/* ====================================================================
 * core_main.c — PECMD 入口链 (main / mainW / mainB / main1 / main5)
 *
 * 来源: PECMD原始.EXE (x64)
 *   mainW     @0x140009ccc  (WinMain 等价主逻辑)
 *   main5     @0x140016aac
 *   mainB     @0x140016ad0
 *   main1     @0x140017010
 *   main      @0x140017034  (CRT main)
 *   mainB_    @0x140017040  thunk -> mainB, 跳过
 *
 * 流程: 命令行获取 -> PECMD**pecmd-cmd* 前缀检测 (cmd 兼容模式) ->
 *       主循环逐行取 token: EXEC (资源消息执行) / MAIN (脚本执行) /
 *       普通行 (命令处理器 PECMD_RunBootScriptInFiber) -> 收尾
 *
 * 依赖 (后续批次实现):
 *   FUN_140024C48   @0x140024c48  token 分割
 *   PECMD_RunBootScriptInFiber     @0x1400050c8  命令处理器 (核心)
 *   FUN_140045C90  @0x140045c90  脚本执行 (MAIN)
 *   PECMD_InitEnvironmentVars     @0x140027690  实例/ShowWindow 初始化
 *   PECMD_BroadcastEnvChange   @0x14002ca30  收尾钩子
 *   PECMD_GetExitCodeGlobal @0x14005b7dc  退出码
 *   FUN_14004EAA8 @0x14004eaa8  脚本结构重置
 *   FUN_1400637DC @0x1400637dc  环境表复制
 *   FUN_1400084D0     @0x1400084d0  EXEC 消息展开执行
 *   PECMD_HeapFreeWithHeader     @0x14005b0d4  消息处理
 *   PECMD_ExitProcessCall @0x14005b21c
 * ==================================================================== */
#include <stdint.h>
#include <stddef.h>
#include <string.h>

#include "pecmd_defs.h"
extern WCHAR **FUN_14005B154(WCHAR **pp); /* @0x14005b154 */

/* ---- 已实现 (core_string/core_init/core_token) 与待重构原型 ---- */
WCHAR *FUN_140024C48(WCHAR **pp, size_t *plen, uint32_t flags); /* @0x140024c48 core_token.c */
int PECMD_RunBootScriptInFiber(WCHAR *cmdline);                              /* @0x1400050c8 TODO 后续批次 */
int FUN_140045C90(void *script, LPCWSTR text);               /* @0x140045c90 TODO 后续批次 */
void PECMD_InitEnvironmentVars(HINSTANCE hInst, int show);                  /* @0x140027690 core_init.c */
void PECMD_BroadcastEnvChange(void);                                     /* @0x14002ca30 core_init.c */
int PECMD_GetExitCodeGlobal(void);                                    /* @0x14005b7dc core_init.c */
void FUN_14004EAA8(void *script, int keep);                 /* @0x14004eaa8 core_init.c */
WCHAR *FUN_1400637DC(WCHAR **ps, LPCSTR src, int64_t srclen, int64_t codepage); /* @0x1400637dc core_init.c */
int FUN_1400084D0(WCHAR *msg);                                  /* @0x1400084d0 core_token.c */
void PECMD_ExitProcessCall(int code);                               /* @0x14005b21c core_init.c */

/* 全局 (core_globals.c) */
extern WCHAR *g_pNextCmd;     /* DAT_14013cac8 */
extern uint8_t g_flag169;     /* DAT_14013c969 (低字节) */
extern uint8_t g_state138;    /* DAT_14013d138 */
extern uint32_t g_afterMain;  /* DAT_14013ccc8 */
extern uint32_t g_state190;   /* DAT_14013d190 */
extern uint32_t g_state198;   /* DAT_14013d198 */
extern uint8_t g_Env[];       /* DAT_14013d1a0 */
extern WCHAR *g_szMsgBuf;     /* DAT_14013cab8 */

/* ========== mainW @0x140009ccc ========== */
static int PECMD_MainW(HINSTANCE hInstance, WCHAR *cmdline)
{
    WCHAR *pToken;      /* token 指针 */
    WCHAR *pCmdBuf;     /* 命令行副本 (local_c0) */
    WCHAR *pMsg;        /* EXEC 参数 (local_b8) */
    WCHAR *pMain;       /* MAIN 参数 (local_b0) */
    WCHAR *pMain2;      /* MAIN 参数副本 */
    STARTUPINFOW si;
    int ret;

    if (!cmdline) {
        return 0;
    }
    g_hHeap = GetProcessHeap();
    if (!hInstance) {
        hInstance = g_hInstance;
    }
    if (!g_hInst) {
        si.cb = 0x68;                       /* sizeof(STARTUPINFOW) */
        si.lpReserved = NULL;
        memset(&si.lpDesktop, 0, 0x58);     /* 清零剩余字段 (FUN_140102a90 即 memset 内联) */
        GetStartupInfoW(&si);
        g_hInst = hInstance;
        PECMD_InitEnvironmentVars(hInstance, si.wShowWindow);
    }
    /* 复制命令行 */
    FUN_1400702B0(&pCmdBuf, GetCommandLineW());
    if (StrCmpNIW(WSTR("PECMD**pecmd-cmd*"), pCmdBuf, 0x11) == 0) {
        /* cmd.exe 兼容模式: 前 17 字符匹配 */
        /* 跳过参数前导空白 */
        while (*cmdline != L'\0' && (*cmdline < 9 || *cmdline > 0xd) && *cmdline != L' ')
            cmdline++;
        FUN_14005B154((WCHAR **)&cmdline);
        while (*cmdline != L'\0' && (*cmdline < 9 || *cmdline > 0xd) && *cmdline != L' ')
            cmdline++;
        /* 截断命令行副本到第一个空白 */
        pToken = pCmdBuf;
        while (*pToken != L'\0' && (*pToken < 9 || *pToken > 0xd) && *pToken != L' ')
            pToken++;
        *pToken = L'\0';
        FUN_14006375C(&pCmdBuf, cmdline);    /* 追加参数 */
        cmdline = pCmdBuf;
        hInstance = g_hInst;
    } else {
        /* 正常模式: 命令行副本截断到第一个空白 */
        pToken = pCmdBuf;
        while (*pToken != L'\0' && (*pToken < 9 || *pToken > 0xd) && *pToken != L' ')
            pToken++;
        *pToken = L'\0';
        FUN_14006375C(&pCmdBuf, cmdline);    /* 追加原始参数 */
        cmdline = pCmdBuf;
        FUN_14005B154((WCHAR **)&cmdline);
        hInstance = g_hInst;
    }

    /* ---- 主循环: 逐行执行 ---- */
    for (;;) {
        WCHAR *pStart = cmdline;            /* 行首 */
        pToken = FUN_140024C48(&cmdline, 0, 5);
        pMain = pToken;
        if (*pToken == L'"') {
            pMain = pToken + 1;
        }
        cmdline = pToken;

        /* EXEC 命令: 执行资源消息 (ID 0x2713) */
        if (FUN_14005C788("EXEC", pMain, 4) &&
            ((pMain[4] > 8 && pMain[4] < 0xe) || pMain[4] == L' ')) {
            PECMD_AllocString(&g_szMsgBuf, 0x6592);
            g_szMsgBuf[0] = L'\0';
            LoadStringW(hInstance, 0x2713, g_szMsgBuf, 26000);
            PECMD_AllocString(&g_szMsgBuf, (int64_t)lstrlenW(g_szMsgBuf) + 2);
            FUN_1400702B0(&pMsg, pMain);
            FUN_1400084D0(pMsg);
            PECMD_FreeStrBuf(&pMsg);
            g_runFlag = 0;
            PECMD_FreeStrBuf(&g_szMsgBuf);   /* PECMD_HeapFreeWithHeader = StrFree */
        }

        /* MAIN 命令: 执行脚本 */
        if (FUN_14005C788("MAIN", pToken, 4) &&
            ((pMain = pToken + 4,
              *pMain > 8 && *pMain < 0xe) || (*pMain == L' ' || *pMain == L'*'))) {
            int hasParam = (*pMain > 8 && *pMain < 0xe) || *pMain == L' ';
            g_flag169 = 0;
            FUN_14005B154(&pMain);
            if (!hasParam || FUN_14005C788("**u", pMain, 3) == 0) {
                /* 无参数或 **u 前缀: 直接执行脚本 */
                FUN_1400702B0(&pMain2, pToken + 4);
                FUN_140045C90(&g_Script, pMain2);
                PECMD_FreeStrBuf(&pMain2);
                goto after_main;
            }
        }
        /* 普通命令行 */
        g_flag169 = g_state138;             /* DAT_14013c969 = DAT_14013d138 */
        PECMD_RunBootScriptInFiber(pStart);
        FUN_14004EAA8(&g_Script, 0);
        FUN_1400637DC((WCHAR **)(void *)&g_Env, (LPCSTR)(const void *)g_DefEnv, -1, -1);
        g_state190 = 0;
        g_state198 = 0;
        g_runFlag = 0;
        if (!g_pNextCmd) {
            goto after_main;
        }
        g_flagA24F = 1;
        cmdline = g_pNextCmd;
    }

after_main:
    if (g_afterMain != 0) {
        PECMD_BroadcastEnvChange();
    }
    ret = PECMD_GetExitCodeGlobal();
    PECMD_FreeStrBuf(&pCmdBuf);
    return ret;
}

/* ========== mainB @0x140016ad0 ========== */
static void PECMD_MainB(HINSTANCE hInstance, WCHAR *cmdline)
{
    PECMD_MainW(hInstance, cmdline);
}

/* ========== main1 @0x140017010 ========== */
static void PECMD_Main1(WCHAR *cmdline)
{
    PECMD_MainB(GetModuleHandleW(NULL), cmdline);
}

/* ========== main @0x140017034 ========== */
int PECMD_main(int argc, char **argv)
{
    (void)argv;
    return (int)PECMD_MainW(0, (WCHAR *)(uintptr_t)argc);  /* 反编译显示 arg 复用, TODO(verify) */
}

/* ========== main5 @0x140016aac ========== */
static void PECMD_Main5(uintptr_t a, uintptr_t b, HINSTANCE hInstance, WCHAR *cmdline)
{
    (void)b;
    g_hInst = (HINSTANCE)a;
    PECMD_MainW(hInstance, cmdline);
}
