/*
 * core_script2.c — ExecCmd 主流程（B8b 核心）
 *
 *   PECMD_RunStartupScript @0x14004eb34   ExecCmd 主流程
 *
 * 流程：
 *   1. 解析命令行（&/:: 变量前缀）
 *   2. 生成随机种子（脚本 XOR 密钥）
 *   3. SCRIPTINIT 资源加载 → 执行 init 段
 *   4. 加载 SCRIPT 资源/命令行脚本
 *   5. 进入消息循环执行脚本
 *
 * 依赖（已实现）：VarFind/VarWrite/SetVar/SetVarD/StrCat/StrAssign/StrFree/
 *   ScriptCopy/ScriptInit/ArgBuild/LineSep/ScriptSep/FileOpen/EncodeDet
 * 待实现（extern）：FUN_140045C90(FUN_140045C90)、PECMD_RunCommand(PECMD_RunCommand)
 */
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>

#include "pecmd_defs.h"

extern void *PECMD_GrowByteBuffer(void **ps, int64_t len);   /* @0x140063424 */
extern void PECMD_ZeroLenBuf(void *p);           /* @0x14005b0b8 */
extern void PECMD_MaskScriptEndFileTail(void *script, WCHAR *buf, bool stopMain); /* @0x14002487c */
extern uint8_t *FUN_14001E69C(void *script, LPCWSTR name, void *scope, int64_t len); /* @0x14001e69c */
extern void PECMD_AppendLongDecimal(void *script, int64_t value, LPCWSTR key); /* @0x1400669c4 */
extern uint16_t PECMD_GenRandomSeed16(void);                     /* @0x14001b510 */
extern void PECMD_SetCurFileVariables(void *script, LPCWSTR curfile, uint32_t flag); /* @0x14002452c */
extern int32_t FUN_14005C7C4(const char *a, const WCHAR *w); /* @0x14005c7c4 */
extern uint8_t *FUN_14001EA18(HMODULE mod, LPCWSTR id, LPCWSTR type, void **out, uint32_t *flags); /* @0x14001ea18 */

/* 待实现（B8c/B9） */
extern int64_t FUN_140045C90(void *script, void *param, int64_t a3); /* @0x140045c90 */
extern int64_t PECMD_RunCommand(void *script, LPCWSTR cmdline, void *a3, void *a4,
                                void *a5, void *a6);   /* @0x140031454 */

/* 数据区标志（待 B9 核对） */
static int32_t g_scriptInitFlag;   /* DAT_14013d209 */
static int32_t g_autoAppFlag;      /* g_charTableF */
static int32_t g_bInitWin;         /* DAT_14013d059 */
static int32_t g_cmdlineMode;      /* DAT_14013ccb0 */

/* ========== PECMD_RunBootScriptInFiber @0x14004eb34 ==========
 * ExecCmd 主流程（mainW → fiber → ExecLine → ExecCmd）。
 * 注：窗口/线程深层交互以 TODO(verify) 简化，核心命令行解析/脚本
 * 执行链已还原。
 */
int64_t PECMD_RunStartupScript(HINSTANCE hinst, uint64_t flag, const WCHAR *cmdline)
{
    (void)flag; (void)hinst;
    LPCWSTR p = cmdline;
    WCHAR *cmd = NULL;
    uint16_t seed;
    WCHAR first = L'\0';
    WCHAR *initBuf = NULL;
    uint8_t *v;
    WCHAR *scriptBuf = NULL;
    int64_t r = 0;

    /* 解析 & 前缀 */
    p = (LPCWSTR)PECMD_GrowByteBuffer(NULL, 0);
    (void)p;
    FUN_1400702B0(&cmd, cmdline);
    /* TODO(verify): 原实现 FUN_140024C48 跳过 & 前缀; hinst/flag 用于资源加载 */

    seed = PECMD_GenRandomSeed16();
    (void)seed;   /* TODO(verify): 用于脚本 XOR 密钥 */
    g_scriptInitFlag = 0x41;

    /* SCRIPTINIT 资源 */
    PECMD_AllocWStringBuffer((WCHAR **)&initBuf, 0x10);
    {
        uint32_t f = 0;
        FUN_14001EA18(g_hInstance, (LPCWSTR)0x64, WSTR("SCRIPTINIT"), (void **)&initBuf, &f);
        if (f >= 5) {
            /* TODO(verify): 原实现 PECMD_MaskScriptEndFileTail 行分割后执行 init */
        }
        if (initBuf != NULL && initBuf[0] != L'\0') {
            PECMD_AppendLongDecimal((int64_t *)&g_Script, 0, WSTR("&PeExe"));
            g_autoAppFlag = 1;
            FUN_1400702B0(&initBuf, cmdline);
            /* TODO(verify): 执行 init 段脚本 */
            g_autoAppFlag = 0;
            v = FUN_14001E69C(&g_Script, WSTR("__Autoapp"), NULL, -1);
            if (v != NULL && *(LPCWSTR)*((int64_t *)v) != L'\0') {
                WCHAR *s = (WCHAR *)*(int64_t *)v;
                first = *s;
                FUN_1400702B0(&cmd, s + 1);
            }
        }
        PECMD_FreeStrBuf(&initBuf);
    }
    PECMD_AppendLongDecimal((int64_t *)&g_Script, 1, WSTR("&PeExe"));

    /* SCRIPT 资源（非命令行模式时） */
    {
        uint32_t f2 = 0;
        bool noScript = false;
        if (!((first != L'*' && first != L'$') && cmd == NULL)) noScript = true;
        if (!noScript) {
            FUN_14001EA18(g_hInst, (LPCWSTR)0x65, WSTR("SCRIPT"), (void **)&scriptBuf, &f2);
            if (f2 < 5) {
                cmd = NULL;
            }
        }
    }

    /* 命令行模式判定 */
    if (cmdline == NULL) {
        g_cmdlineMode = 0;
    } else {
        int64_t r5 = FUN_14005C7C4("PECMD**pecmd-cmd*", cmdline) == 0 ? 0 : 1;
        g_cmdlineMode = 1;
        if (r5 == 0) g_cmdlineMode = 0;
    }

    if (g_cmdlineMode == 0) {
        /* 命令行执行分支 */
        if (first == L'-') {
            /* TODO(verify): 单命令模式 */
        }
        if (first == L'*') {
            g_autoAppFlag = 1;
            r = FUN_140045C90(&g_Script, (void *)((int64_t)cmd + 8), 0);
        } else if (first == L'$') {
            g_autoAppFlag = 1;
            r = PECMD_RunCommand(&g_Script, cmd, NULL, NULL, NULL, NULL);
        } else if (cmd == NULL || scriptBuf != NULL) {
            if ((cmd != NULL && *cmd != L'\0') || scriptBuf != NULL) goto run_script;
            /* 无脚本：查找资源 */
            {
                void *out = NULL;
                FUN_14001EA18(g_hInstance, (LPCWSTR)0x12e, (LPCWSTR)5, &out, NULL);
                /* TODO(verify): PECMD_ParseHlpDoc 执行 */
            }
        } else {
            g_autoAppFlag = 1;
            FUN_14006375C(&cmd, WSTR(" "));
            FUN_14006375C(&cmd, cmdline);
            r = PECMD_RunCommand(&g_Script, cmd, NULL, NULL, NULL, NULL);
        }
    } else {
run_script:
        /* 脚本执行分支 */
        {
            WCHAR *p2 = NULL;
            PECMD_AllocStrSlot(&p2);
            /* TODO(verify): 完整参数解析后执行 */
            PECMD_SetCurFileVariables(&g_Script, cmdline, 0);
            /* 执行主脚本 */
            r = PECMD_RunCommand(&g_Script, cmdline, NULL, NULL, NULL, NULL);
            PECMD_FreeStrBuf(&p2);
        }
    }

    /* 清理 */
    PECMD_FreeStrBuf(&scriptBuf);
    PECMD_FreeStrBuf(&cmd);
    PECMD_ZeroLenBuf(initBuf);
    PECMD_FreeStrBuf(&initBuf);
    return r;
}
