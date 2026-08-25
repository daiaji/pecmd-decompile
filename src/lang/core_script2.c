/*
 * core_script2.c — 启动分发主流程（S7 直移）
 *
 *   PECMD_RunStartupScript @0x14004eb34   FUN_14004eb34 全文直移 (size=3153)
 *   原文: decompiled.c 行 45447-45864 (C:\pectest\_s7_ref.txt)
 *
 * 流程（对照原文）：
 *   1. 头部: 取 boot 命令行首 token 后的参数尾 (@45529-45531)、随机种子
 *      (FUN_14001b510, @45534-45539)、&PeExe=-1 (@45542)
 *   2. SCRIPTINIT 资源(0x64)加载/解码/经 PECMD_SetControlCommandTrigger 执行
 *      init 段；随后查 __Autoapp(首字符 WVar27 + 余串 local_228) 与
 *      __bInitWin(@45543-45588)
 *   3. &PeExe=1 (@45591)；SCRIPT 资源(0x65)按 flags=seed<<16 加载+解码，
 *      len<5 视为空并清种子 (@45592-45610)
 *   4. g_cmdlineMode 判定：FUN_14005b1a8(L"PECMD**pecmd-cmd*", cmdline, 0x11)
 *      命中才置 1（REVIEW §132 实锤，勿再倒置）(@45611-45624)
 *   5. "MAIN " 参数尾检测 → g_flag16a=0 (@45622-45625)
 *   6. 分发树 (@45626-45654)：
 *        WVar27=='-' → 仅标记 cVar32=2 后走后续链
 *        WVar27=='*' → FUN_140045c90(g_Script, autoRem+8字节) 脚本装载器
 *        WVar27=='$' → FUN_14004c0bc ProcessScriptBlock(autoRem)
 *        autoRem 空或有 SCRIPT 资源 → 参数尾非空/有资源则进 efda，否则
 *            FindResource(0x12e,RT_DIALOG) → PECMD_ParseHlpDoc
 *        否则(自动启动) → " "+autoRem+" "+参数尾 交给 PECMD_RunCommand
 *   7. LAB_14004efda 启动分发段 (@45657-45849)：CopyCommandLine → 取参数尾 →
 *      /L 开关识别与 "LOAD  <file>" 前缀补写 → 文本缓冲 → SetCurFileVariables
 *      (**#101/**cmdline*) → 克隆脚本结构+InitObfuscatedKeywords(seed) →
 *      MaskScriptEndFileTail → StrChrOffset 定界 → ParseScriptSegments(import 合并)
 *      → XOR 还原 → &PeExe=local_236 → RunScriptText(takeover) → 收尾泵。
 *      WVar27 首字符三分支用 __Autoapp 变量首字符（通常 NUL），不是 cmd 拷贝首字符。
 *   8. 公共收尾 (@45851-45863)：ZeroLenBuf(SCRIPT 缓冲)/托盘×2/对象槽释放/
 *      UninstallInputHook/PECMD_ScriptInitParse(L"LOGS\0\n\n" @0x140123280)。
 *
 * 指针算术纪律（REVIEW §130）：Ghidra 的 `longlong+N` 一律字节偏移；
 *   本文件中仅 FUN_140045C90(autoRem+8) 与 local_208+5(WCHAR*→+10 字节)
 *   两处为原文字节/WCHAR 偏移的直移，已逐一标注 dc 行号。
 *
 * SKIP/偏差登记（详见各调用点注释）：
 *   1) dc:45858 收尾 LOGS 指令(PECMD_ScriptInitParse) 暂时挂起 —— 还原真体
 *      对本输入稳定 AV(memfail.log tail-7/tail-8 时序三次实锤)，非本文件可修。
 *   2) dc:45852-45853 托盘清理走 no-op 桩 PECMD_DestroyTrayIcon —— 重构端
 *      DAT_14013d8a0/g_hwndTray 缓冲未按 0x3f0 字节布局落位，真体会越界踩全局。
 *   3) S7_BRIDGE_EXECUTOR=0：按原文走 RunScriptText。行执行器真体
 *      FUN_1400b1724/FUN_140003a20 尚为 no-op 桩(unimplemented_stubs.c)，
 *      故 ENVI/EXEC 动词当前仍不会执行 —— 门A 卡点在上游 B8c，非本文件。
 *
 * TEMP PROBE（入口/出口）保留原样 —— 下游排查依赖 memfail.log 时序；
 * [S7] bisect 标记为本轮定位新增，链路接通后可一并移除。
 */
#include <stdlib.h>
#include <stdio.h> /* TEMP PROBE */
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

#include "pecmd_defs.h"

/* 桥接开关: 默认 0 = 按原文走 RunScriptText(待上游补 FUN_1400b1724/FUN_140003a20 真体,
 * B8c 批次)。置 1 可改经真体 PECMD_ProcessScriptBlock 直接驱动文本 —— 实测两种取值
 * 当前行为一致(行执行器为桩, 文本空转), 上游真体就位后应保持 0 以获得完整
 * RunScriptText 上下文语义(CurFile 现场/窗口/引用计数收尾)。 */
#define S7_BRIDGE_EXECUTOR 0

/* ---- 已还原依赖（真体分布见各注） ---- */
extern WCHAR **FUN_14005B154(WCHAR **pp); /* @0x14005b154 core_string.c (=FUN_1400170b0 同体) */
extern int32_t FUN_14005B1A8(const WCHAR *s, const WCHAR **pw, int n); /* @0x14005b1a8 core_exec2.c */
extern int FUN_14005C788(const char *s, const WCHAR *w, int n);        /* @0x14005c788 core_string.c */
extern WCHAR *FUN_140024C48(WCHAR **pp, size_t *plen, uint32_t flags); /* @0x140024c48 core_token.c */
extern void FUN_1400702B0(WCHAR **ps, LPCWSTR src);                    /* @0x1400702b0 core_string.c */
extern WCHAR *FUN_14006375C(WCHAR **ps, LPCWSTR src);                  /* @0x14006375c core_string.c */
extern void PECMD_FreeStrBuf(WCHAR **ps);                              /* @0x14005b104 core_string.c */
/* S11: 本地声明与定义冲突已删除, 统一采用 xproto.h 原型 (原: extern void PECMD_AllocStrSlot(WCHAR **ps); /* @0x140063620 restored_bodies.c * /) */
extern WCHAR *PECMD_AllocString(WCHAR **ps, int64_t count);         /* @0x140063720 core_string.c (S11 归正返回型) */
extern void PECMD_AllocWStringBuffer(WCHAR **ps, int64_t count);       /* @0x140063694 core_var.c */
extern void PECMD_ZeroLenBuf(void *p);                                 /* @0x14005b0b8 core_thread.c */
extern bool FUN_140101E70(LPCWSTR path);                               /* @0x140101e70 core_exec2.c */
extern WCHAR *FUN_14001BE14(WCHAR *s);                                 /* @0x14001be14 去引号 */
extern uint16_t PECMD_GenRandomSeed16(void);                           /* @0x14001b510 core_var3.c */
extern int32_t FUN_14001B5AC(LPCWSTR buf, uint32_t key, int64_t n);    /* @0x14001b5ac core_var3.c */
extern uint64_t PECMD_StrChrOffset(const WCHAR *buf, WCHAR ch);        /* @0x14001b4f8 core_var3.c */
extern void PECMD_AppendLongDecimal(void *script, int64_t value, LPCWSTR key); /* @0x1400669c4 */
extern void PECMD_SetCurFileVariables(void *script, LPCWSTR curfile, uint32_t flag); /* @0x14002452c core_exec3.c */
extern uint8_t *FUN_14001E69C(void *script, LPCWSTR name, void *scope, int64_t len); /* @0x14001e69c core_var3.c */
extern uint8_t *FUN_14001EA18(HMODULE mod, LPCWSTR id, LPCWSTR type, void **out,
                              uint32_t *flags);                        /* @0x14001ea18 core_exec2.c */
extern uint32_t FUN_1400E7D58(int64_t *ps, uint32_t flags);            /* @0x1400e7d58 core_resdecode.c */
extern uint32_t FUN_140073CCC(void *script, LPCWSTR cmdline, int saveArg); /* @0x140073ccc core_exec5.c */
extern void FUN_140017CDC(void *dst, void *src);                       /* @0x140017cdc core_exec5.c */
extern void FUN_1400186BC(void *s, void *parent);                      /* @0x1400186bc core_exec5.c */
extern void FUN_14004EAA8(void *table, int flag);                      /* @0x14004eaa8 core_exec3.c(全局版) */
extern void FUN_1400E56E4(uint64_t *slot);                             /* @0x1400e56e4 core_b8a.c */
extern int32_t PECMD_CopyCommandLine(LPCWSTR src, WCHAR **out);        /* @0x140027970 core_exec2.c */
extern void PECMD_InitObfuscatedKeywords(void *script, uint64_t seed); /* @0x14006159c core_exec2.c */
extern void PECMD_MaskScriptEndFileTail(void *script, WCHAR *buf, bool stopMain); /* @0x14002487c */

/* ---- 执行链 ---- */
extern int64_t FUN_140045C90(void *script, LPCWSTR cmdline);           /* @0x140045c90 core_execscript.c */
extern int64_t PECMD_ProcessScriptBlock(uint64_t script, uint64_t cmd, void *p3, void *p4,
                                        void *p5);                     /* @0x14004c0bc restored_bodies.c */
extern int64_t PECMD_RunCommand(void *script, WCHAR *cmdline);         /* @0x140031454 core_scriptrun.c */
extern int64_t PECMD_RunScriptText(void *pScript, LPCWSTR pText, LPCWSTR pName, LPCWSTR pCurFile,
                                   uint32_t flags, LPCWSTR pFile,
                                   void *pPersist);                    /* @0x1400b638c core_execmain.c */
extern int64_t PECMD_SetControlCommandTrigger(uint64_t base, const WCHAR *text,
                                              LPCWSTR args);           /* @0x1400b6240 core_b3r_i28d.c */
extern uint32_t PECMD_ParseScriptSegments(int64_t *ctrl, int start, int len, int64_t *out,
                                          LPCWSTR script, uint32_t flags); /* @0x140030420 core_script.c */
extern uint64_t PECMD_ParseHlpDoc(uint64_t script, LPCWSTR arg);       /* @0x1400403d8 core_b3_remaining.c */
extern int PECMD_ParseNumTryWriteback(long long *pp, int *out);        /* @0x140067d20 restored_bodies.c */
extern uint64_t PECMD_ScriptInitParse(uint64_t script, uint64_t cmd, uint64_t flag); /* @0x140026338 */
extern void PECMD_UninstallInputHook(void);                            /* @0x14001bb30 core_exec3.c */
extern void PECMD_WaitCountPumpMessages(int *count, int limit);        /* @0x14001bbdc core_exec3.c */
extern void PECMD_ShutdownCom(void);                                   /* @0x140064328 core_exec3.c */
extern void PECMD_DestroyTrayIcon(char *nid);                          /* @0x14005b7e8 no-op 桩(unimplemented_stubs.c) */

/* ---- 全局 ---- */
extern int32_t g_bInitWin;      /* DAT_14013d059 共享全局(core_execmain RunScriptText 读) */
extern uint8_t DAT_14013d8a0[]; /* 托盘图标结构缓冲 (unimplemented_stubs.c 定义, 256B) */

/* DAT_14013d209 / DAT_14013ccb0 经核对全二进制仅本函数使用(decompiled.c 全文
 * 仅 @45533/@45614/@45618/@45626/@45627 引用)，文件内 static 与原版等价。 */
static int32_t g_scriptInitFlag; /* DAT_14013d209 入口标记 0x41 */
static int32_t g_cmdlineMode;    /* DAT_14013ccb0 PECMD**pecmd-cmd* 模式 */

/* ========== 局部最小辅助（原文小依赖, dc 行号标注） ========== */

/* TEMP PROBE(bisect): 尾段崩溃定位标记, 定位完成后移除 */
static void S7_MARK(const char *tag)
{
    FILE *pf = fopen("C:\\pectest\\memfail.log", "a");
    if (pf) {
        fprintf(pf, "[S7] %s\n", tag);
        fclose(pf);
    }
}


/* @0x140063888 (dc:60955) 串槽赋值 n 字符; FUN_1400703e4(-1)/FUN_1400702d4(n)
 * 都收敛到此。项目内无独立还原体, 按 PECMD_AllocString(=@0x140063720) 组合直移。 */
static void S7_StrAssignN(WCHAR **ps, LPCWSTR src, int64_t n)
{
    *ps = NULL;
    if (n < 0) {
        n = lstrlenW(src);
    }
    PECMD_AllocString(ps, n + 1);
    memcpy(*ps, src, (size_t)n * 2);
    (*ps)[n] = L'\0';
}

/* @0x14005e51c (dc:56517) " ;;" → '\n' 就地压缩 (dc 直移, 仅 /L 分支使用) */
static LPCWSTR S7_SemicolonRunToNewline(LPCWSTR param_1)
{
    int iVar3 = lstrlenW(param_1);
    WCHAR WVar1 = *param_1;
    WCHAR *pWVar5 = param_1 + (int64_t)iVar3 + 1;
    int64_t lVar2 = (int64_t)iVar3 * 2 + 4;
    WCHAR *pWVar4 = param_1;
    int64_t lVar6 = lVar2;
    while ((WVar1 != L'\0') && (pWVar4 < pWVar5)) {
        if ((pWVar4[0] == L' ') && (pWVar4[1] == L';') && (pWVar4[2] == L';') && (pWVar4[3] == L' ')) {
            pWVar4[0] = L'\n';
            memmove(pWVar4 + 1, pWVar4 + 4, (size_t)((int)(((lVar6 - 2) >> 1) * 2 - 6)));
            pWVar5 = pWVar5 - 3;
        }
        lVar6 = lVar6 - 2;
        pWVar4 = pWVar4 + 1;
        WVar1 = pWVar4[0];
    }
    if (((pWVar4[0] == L' ') && (pWVar4[1] == L';')) &&
        ((pWVar4[2] == L';') && (pWVar4[3] == L';'))) {
        do {
            pWVar4 = pWVar5;
            pWVar5 = pWVar4 + 1;
        } while (pWVar5[0] == L';');
        if (pWVar5[0] == L' ') {
            memmove(pWVar4, pWVar5,
                    (size_t)(int)((((int64_t)(intptr_t)param_1 + (lVar2 - (int64_t)(intptr_t)pWVar5)) >> 1) * 2));
        }
    }
    return param_1;
}

/* 原 .rdata 常量块 @0x140123280 (dc:45856-45858, Ghidra MCP 读证):
 * 4c 00 4f 00 47 00 53 00 | 00 00 00 00 0a 00 0a 00 = L"LOGS\0\n\n"
 * FUN_140026338 的 param_2 指向块内 +4 WCHAR 处(函数体读 param_2-4)。
 * 注意: 原文先把两个 qword 拷到【可写栈】再传址(dc:45856-45857), 因为被调方
 * 会就地改写(Unquote 等); 此处必须保持可写存储, 放 .rdata 会直接 AV。 */
static WCHAR s_TailLogsCmd[8] = { L'L', L'O', L'G', L'S', L'\0', L'\n', L'\n', L'\0' };

/* ========== PECMD_RunStartupScript @0x14004eb34 (dc:45452-45864 直移) ========== */
int64_t PECMD_RunStartupScript(HINSTANCE hinst, uint64_t flag, const WCHAR *cmdline)
{
    FILE *pf_ = fopen("C:\\pectest\\memfail.log", "a");
    if (pf_) { fprintf(pf_, "PROBE StartupScript enter cmd=%ls\n", (const wchar_t *)cmdline); fclose(pf_); }
    (void)hinst; /* param_1: 调用方传 DAT_14013cf70, 函数体内未消费 */
    (void)flag;  /* param_2: 未消费 (调用点恒 0, dc:2099) */

    /* ---- 变量对位 (Ghidra 名 → 本地名) ---- */
    WCHAR *rest = NULL;          /* local_res18 参数尾游标 */
    int64_t r = 0;               /* LVar12 返回值累积 */
    int64_t retOut = -1;         /* LVar33 初值 -1 (@45541), 早退路径直接返回 */
    WCHAR *autoRem = NULL;       /* local_228 __Autoapp 余串槽 */
    uint16_t seedRaw;            /* uVar6 初值 FUN_14001b510() */
    uint16_t repPair;            /* uVar6 再生成值 (@45538) */
    uint32_t seedFlags;          /* uVar25/local_220 = 种子<<16 flags */
    uint32_t local210 = 0;       /* local_210 低16有效(高位 unaff_R13 残留未被消费) */
    uint32_t local21c = 0;       /* local_21c 低16=原始种子 */
    WCHAR autoFirst = L'\0';     /* WVar27 __Autoapp 首字符 */
    int64_t sdInit[3];           /* local_1b8/local_1b0/local_1a8 SCRIPTINIT 描述符{ptr,len,len} */
    WCHAR *cmdDup = NULL;        /* local_178 */
    int64_t sdScript[3];         /* local_1d8/local_1d0/local_1c8 SCRIPT 资源描述符 */
    WCHAR *scriptText = NULL;    /* pWVar21 SCRIPT 资源文本指针 */
    int cVar32;                  /* cmdlineMode (- 形式时改写为 2) */
    int64_t sdText[3];           /* local_1f8/local_1f0/local_1e8 待执行文本描述符 */
    WCHAR *cmdFull = NULL;       /* local_230 完整命令行副本 */
    WCHAR *tokStart = NULL;      /* local_218 */
    int64_t tokOff = 0;          /* lVar26 参数尾在 cmdFull 内的 WCHAR 偏移 */
    bool bVar2, bVar3;           /* SCRIPT 资源执行路径标志 */
    uint8_t bVar31 = 0;             /* RunScriptText flags bit0 来源 */
    uint8_t bVar23 = 0;             /* '/L' 开关字符 ('L'=0x4c) */
    char local236 = 1;           /* local_236 &PeExe 终值 0/1 */
    WCHAR *lineBuf = NULL;       /* local_180 "\n\n" 行缓冲 */
    uint64_t clone[(0xf0 + 7) / 8]; /* local_118..local_d0 克隆脚本结构 (≥0xe8 字节) */
    WCHAR *scratchA8 = NULL;     /* local_a8 原文未初始化栈槽 FreeStrBuf(dc:45815),
                                    以 NULL 初始化保持结构且避免释放栈残留 TODO(verify) */
    MSG msg;                     /* local_148 tagMSG */
    uint32_t uVar25run;          /* ParseScriptSegments flags */
    uint32_t uVar28;             /* local_210 & 0xffff */
    uint32_t uVar29;             /* 多用途暂存 */
    uint64_t uVar19;             /* StrChrOffset 定界结果 (WCHAR 数) */
    uint32_t uVar8;              /* ParseScriptSegments 返回 */
    int iVar7;
    WCHAR *pWVar18, *pWVar21, *pCur;
    int64_t *ppDesc;             /* ppWVar24 选中的文本描述符(sdText 或 sdScript), int64_t[3] 视图 */
    WCHAR *tokCopy = NULL;       /* local_198 首 token 副本 */
    WCHAR *loadBuf = NULL;       /* local_208 "LOAD  <file>" 组装缓冲 */

    /* ---- dc:45529-45531 参数尾解析 ----
     * local_res18 = FUN_140024c48(&local_res18,0,5) 后接 FUN_1400170b0(&local_res18)
     * (与 FUN_14005b154 同体, 复用后者真体)。flags=5: 0x4 结束后跳空白|0x1 跳闭引号 */
    {
        WCHAR *cur0 = (WCHAR *)cmdline;
        rest = FUN_140024C48(&cur0, NULL, 5);
        FUN_14005B154(&rest);
    }

    r = 0;                                  /* dc:45532 */
    g_scriptInitFlag = 0x41;                /* dc:45533 DAT_14013d209 */

    /* ---- dc:45534-45539 种子派生 ----
     * local_21c 低16 = 原始种子; uVar6' = (种子低字节<<8)|种子低字节;
     * local_210 高16来自 unaff_R13 寄存器残留, 下游仅以 &0xffff/(short)/<<16 截断消费。 */
    seedRaw = PECMD_GenRandomSeed16();               /* dc:45534 */
    local21c = seedRaw;                              /* dc:45535 */
    repPair = (uint16_t)((((uint32_t)seedRaw << 8) | (uint32_t)(seedRaw & 0xff)) & 0xffff);
    local210 = repPair;                              /* dc:45539 */
    autoFirst = L'\0';                               /* dc:45537 WVar27 */

    /* ---- dc:45540-45542 ---- */
    PECMD_AllocStrSlot(&autoRem);                    /* dc:45540 FUN_140063620(&local_228):
                                                        __Autoapp 余串槽先建好; 无 __Autoapp 时
                                                        保持合法空串槽(非 NULL), dc:45598 会读它 */
    sdInit[0] = 0; sdInit[1] = 0; sdInit[2] = 0;
    PECMD_AllocStrSlot((WCHAR **)sdInit);            /* dc:45543 FUN_140063620(&local_1b8) */
    retOut = -1;                                     /* dc:45541 LVar33 */
    PECMD_AppendLongDecimal(&g_Script, -1, WSTR("&PeExe")); /* dc:45542 */

    /* ---- dc:45543-45588 SCRIPTINIT 资源执行 ---- */
    {
        FUN_14001EA18(g_hInstance, (LPCWSTR)(uintptr_t)0x64, WSTR("SCRIPTINIT"),
                      (void **)sdInit, NULL);        /* dc:45546 flags=NULL */
        FUN_1400E7D58(sdInit, 1);                    /* dc:45548 ResDecode */
        pCur = (WCHAR *)(uintptr_t)sdInit[0];        /* dc:45549 local_1c0 */
        FUN_14005B154(&pCur);                        /* dc:45551 FUN_1400170b0 */
        if (sdInit[1] < 5) {                         /* dc:45552 len<5 → 视为空 */
            if ((WCHAR *)(uintptr_t)sdInit[0] != NULL) {
                *(WCHAR *)(uintptr_t)sdInit[0] = L'\0';   /* dc:45553 puVar30 */
            }
            if (pCur != NULL) {
                pCur[0] = L'\0';                          /* dc:45554 local_1c0 */
            }
        }
        if (pCur[0] != L'\0') {                      /* dc:45557 init 段非空 → 执行 */
            PECMD_SetCurFileVariables(&g_Script, WSTR("**init*"), 0x40); /* dc:45558 */
            pWVar21 = (WCHAR *)(uintptr_t)sdInit[0];      /* dc:45559 puVar30 */
            {
                uint8_t savedF = g_charTableF;            /* dc:45560 uVar4 */
                sdInit[0] = 0;                            /* dc:45561 接管: descriptor.ptr=NULL */
                g_charTableF = 1;                         /* dc:45563 DAT_14013a248 */
                FUN_1400702B0(&cmdDup, cmdline);          /* dc:45564 */
                r = PECMD_SetControlCommandTrigger((uint64_t)(uintptr_t)&g_Script,
                                                   (LPCWSTR)pWVar21, cmdDup); /* dc:45565 FUN_1400b6240 */
                g_charTableF = savedF;                    /* dc:45566 */
            }
            /* __Autoapp (dc:45567-45575): 首字符入 WVar27, 余串赋给 local_228 */
            {
                uint8_t *node = FUN_14001E69C(&g_Script, WSTR("__Autoapp"), NULL, -1);
                if (node != NULL) {
                    WCHAR *v = *(WCHAR **)node;           /* dc:45569 *puVar13 */
                    if (v[0] != L'\0') {
                        autoFirst = v[0];                 /* dc:45571 WVar27 */
                        v = v + 1;                        /* dc:45572 */
                    }
                    S7_StrAssignN(&autoRem, v, -1);       /* dc:45574 FUN_1400703e4 */
                }
            }
            /* __bInitWin (dc:45576-45585): 数值解析并入 DAT_14013d059 */
            {
                uint8_t *node = FUN_14001E69C(&g_Script, WSTR("__bInitWin"), NULL, -1);
                if (node != NULL) {
                    WCHAR *v = *(WCHAR **)node;           /* dc:45579 local_168 */
                    int numOut[2];                        /* dc:45580 local_200 */
                    numOut[0] = 0;
                    g_bInitWin = 1;                       /* dc:45578 DAT_14013d059 */
                    PECMD_ParseNumTryWriteback((long long *)&v, numOut); /* dc:45581 FUN_140067d20 */
                    if (0 < numOut[0]) {
                        g_bInitWin = (int32_t)((uint32_t)g_bInitWin |
                                               ((uint32_t)(uint8_t)numOut[0] & 0x3fu)); /* dc:45583 */
                    }
                }
            }
            PECMD_FreeStrBuf(&cmdDup);                    /* dc:45586 */
        }
        /* dc:45589-45590 ZeroLenBuf(local_1b8 ptr)/FreeStrBuf(&local_1b8):
         * 接管路径下两者均见 NULL(no-op); 未接管(空 init)时清理空槽。 */
        PECMD_ZeroLenBuf((WCHAR *)(uintptr_t)sdInit[0]);
        PECMD_FreeStrBuf((WCHAR **)sdInit);
    }

    PECMD_AppendLongDecimal(&g_Script, 1, WSTR("&PeExe")); /* dc:45591 */

    /* ---- dc:45592-45610 SCRIPT 资源(0x65) 加载 ---- */
    sdScript[0] = 0; sdScript[1] = 0; sdScript[2] = 0;
    PECMD_AllocStrSlot((WCHAR **)sdScript);              /* dc:45592 FUN_140063620(&local_1d8) */
    sdScript[1] = 0;                                     /* dc:45594 local_1d0 */
    sdScript[2] = 0;                                     /* dc:45595 local_1c8 */
    scriptText = NULL;                                   /* dc:45596 pWVar21 */
    seedFlags = (uint32_t)repPair << 16;                 /* dc:45593,45597 uVar25/local_220 */
    if ((autoFirst != L'*') && (autoFirst != L'$')) {
        if (*(uint16_t *)(uintptr_t)autoRem == 0) {      /* dc:45598 余串为空才尝试资源 */
            uint32_t f = seedFlags;
            FUN_14001EA18(g_hInst, (LPCWSTR)(uintptr_t)0x65, WSTR("SCRIPT"),
                          (void **)sdScript, &f);        /* dc:45600 注意实例=DAT_14013cf70 */
            seedFlags = f;                               /* dc:45601 */
            FUN_1400E7D58(sdScript, seedFlags | 1);      /* dc:45602 */
            scriptText = (WCHAR *)(uintptr_t)sdScript[0]; /* dc:45603 */
            if (sdScript[1] < 5) {                       /* dc:45604 解码后仍空 → 清种子 */
                local21c &= 0xffff0000u;                 /* dc:45605 */
                local210 = 0;                            /* dc:45606 */
                seedFlags = 0;                           /* dc:45607 */
                scriptText = NULL;                       /* dc:45608 */
            }
        }
    }

    /* ---- dc:45611-45624 g_cmdlineMode 判定 ----
     * REVIEW §132: mode=1 当且仅当 cmdline 以 PECMD**pecmd-cmd* 开头(0x11 字符) */
    {
        const WCHAR *probe = cmdline;                    /* dc:45611 local_170 */
        if (cmdline == NULL) {
            g_cmdlineMode = 0;                           /* dc:45614 */
        }
        else {
            int m = FUN_14005B1A8(L"PECMD**pecmd-cmd*", &probe, 0x11); /* dc:45617 */
            g_cmdlineMode = 1;                           /* dc:45618 */
            if (m == 0) {
                g_cmdlineMode = 0;                       /* dc:45619 */
            }
        }
    }
    { /* TEMP PROBE(bisect) */
        char mb[128];
        FILE *pf2 = fopen("C:\\pectest\\memfail.log", "a");
        if (pf2) {
            wsprintfA(mb, "[S7] mode=%d autoFirst=0x%x autoRemCh0=0x%x scriptRes=%d",
                      g_cmdlineMode, (unsigned)autoFirst,
                      (unsigned)*(uint16_t *)(uintptr_t)autoRem, scriptText != NULL);
            fprintf(pf2, "%s\n", mb);
            fclose(pf2);
        }
    }

    /* ---- dc:45621-45625 "MAIN " 参数尾检测 ---- */
    if (FUN_14005C788("MAIN", rest, 4)) {
        g_flag16a = 0;                                   /* DAT_14013c96a */
    }

    /* ---- dc:45626-45654 分发树 ---- */
    cVar32 = g_cmdlineMode;
    if (g_cmdlineMode == 0) {
        S7_MARK("br-mode0"); /* TEMP PROBE(bisect) */
        if (autoFirst == L'-') {
            cVar32 = 2;                                  /* dc:45629 标记, efda 内 bVar 判定消费 */
        }
        if (autoFirst == L'*') {
            /* dc:45631-45635 '*' 形式 → 脚本装载器。
             * 原文 (local_228.QuadPart + 8) 为字节偏移(+4 WCHAR, dc:45634),
             * 按 REVIEW §130 纪律保留字节算术。TODO(verify): 短余串越界风险同原文。 */
            g_charTableF = 1;                            /* DAT_14013a248 */
            FUN_1400E56E4((uint64_t *)&g_i64D170);       /* dc:45633 */
            r = FUN_140045C90(&g_Script, (LPCWSTR)((char *)autoRem + 8));
        }
        else if (autoFirst == L'$') {
            /* dc:45636-45640 '$' 形式 → ProcessScriptBlock 直接执行余串 */
            g_charTableF = 1;
            r = (int64_t)PECMD_ProcessScriptBlock((uint64_t)(uintptr_t)&g_Script,
                                                  (uint64_t)(uintptr_t)autoRem, 0, 0, 0);
        }
        else if ((*(uint16_t *)(uintptr_t)autoRem == 0) || (scriptText != NULL)) {
            if ((rest[0] != L'\0') || (scriptText != NULL)) {
                goto LAB_efda;                           /* dc:45642 → 启动分发段 */
            }
            { /* dc:45643-45647 无脚本无参数: 帮助对话框资源 (0x12e, RT_DIALOG=5) */
                HRSRC hr = FindResourceW(g_hInstance, (LPCWSTR)(uintptr_t)0x12e,
                                         (LPCWSTR)(uintptr_t)5);
                if (hr != (HRSRC)0) {
                    r = (int64_t)(int32_t)(uint32_t)PECMD_ParseHlpDoc((uint64_t)(uintptr_t)&g_Script,
                                                                      g_szEmpty);
                }
            }
        }
        else {
            /* dc:45650-45653 自动启动: RunCommand(" " + autoRem + " " + 参数尾) */
            g_charTableF = 1;
            FUN_14006375C(&autoRem, WSTR(" "));
            FUN_14006375C(&autoRem, rest);
            r = PECMD_RunCommand(&g_Script, autoRem);
        }
    }
    else {
LAB_efda:
        /* ================= LAB_14004efda 启动分发段 (dc:45657-45849) ================= */
        sdText[0] = 0; sdText[1] = 0; sdText[2] = 0;
        bVar31 = 0;
        memset(clone, 0, sizeof(clone)); /* 原文栈残留字段以零初始化等效(新栈页为零) */
        PECMD_AllocStrSlot(&cmdFull);                         /* dc:45658 FUN_140063620(&local_230) */
        if (cmdline == NULL) {                                /* dc:45660 */
            PECMD_FreeStrBuf(&cmdFull);                       /* dc:45661 */
            goto LAB_f74a;                                    /* dc:45662 */
        }
        PECMD_CopyCommandLine(cmdline, &cmdFull);             /* dc:45664 FUN_140027970 */
        {
            WCHAR *cursor = cmdFull;                          /* dc:45665 local_1a0 */
            size_t tokLen = 0;                                /* dc:45666 local_190 */
            tokStart = FUN_140024C48(&cursor, &tokLen, 1);    /* dc:45667 flags=1 */
            FUN_14005B154(&tokStart);                         /* dc:45668 FUN_1400170b0 */
            pWVar18 = tokStart;                               /* dc:45669 */
            tokOff = (int64_t)(intptr_t)((char *)tokStart - (char *)cmdFull) >> 1; /* dc:45670 */
        }
        /* dc:45671-45678 bVar2/bVar3: 有解码 SCRIPT 资源 且 非 cmdline 模式 */
        if ((scriptText == NULL) || (cVar32 != 0)) {
            bVar2 = false;
            bVar3 = false;
        }
        else {
            bVar2 = true;
            bVar3 = true;
        }
        bVar31 = 0;                                           /* dc:45679 */
        sdText[0] = 0; sdText[1] = 0; sdText[2] = 0;          /* dc:45680-45682 */
        local236 = 1;                                         /* dc:45683 */
        if (bVar2) {
            ppDesc = sdScript;                                /* dc:45686 ppWVar24=&local_1d8 */
            g_charTableF = 1;                                 /* dc:45687 */
            local236 = 0;                                     /* dc:45688 */
            goto LAB_post_sel;                                /* → dc:45773 */
        }
        else {
            PECMD_ZeroLenBuf((WCHAR *)(uintptr_t)sdScript[0]); /* dc:45691 清资源缓冲内容 */
            local210 &= 0xffff0000u;                          /* dc:45692 */
            local21c &= 0xffff0000u;                          /* dc:45693 */
            seedFlags = 0;                                    /* dc:45695 uVar25 */
            bVar23 = 0;                                       /* dc:45696 */
            /* dc:45697-45700 "/L"(或"/l"+空白) 开关识别 */
            if ((pWVar18[0] == L'/') && ((WCHAR)(pWVar18[1] | 0x20U) == 0x6c)) {
                if (((pWVar18[2] > 8) && (pWVar18[2] < 0xe)) || (pWVar18[2] == L' ')) {
                    bVar23 = (uint8_t)pWVar18[1];                /* dc:45699 'L'/'l' */
                }
            }
            {
                /* local_208 在原文为函数域变量(dc:45701), 释放点在 LAB_14004f389 */
                if (bVar23 == 0) {
                    /* dc:45702-45726 普通 form: 首 token 是否既有文件 → 补 "LOAD  " 前缀 */
                    size_t tokLen2 = 0;                       /* local_160 */
                    WCHAR firstCh;                            /* WVar27 复用 */
                    WCHAR *tk = pWVar18;                      /* local_188 */
                    WCHAR *tokEnd;
                    firstCh = pWVar18[0];
                    tokEnd = FUN_140024C48(&tk, &tokLen2, 0); /* dc:45706 */
                    S7_StrAssignN(&tokCopy, tk, (int64_t)(intptr_t)((char *)tokEnd - (char *)tk) >> 1); /* dc:45707 FUN_1400702d4 */
                    iVar7 = FUN_140101E70(tokCopy) ? 1 : 0;   /* dc:45709-45710 文件存在? */
                    if ((firstCh == L'"') &&
                        ((iVar7 == 0) ||
                         (((pWVar18[1] > 8) && (pWVar18[1] < 0xe)) || (pWVar18[1] == L' ')))) {
                        pWVar18 = FUN_14001BE14(pWVar18);     /* dc:45714 去引号 */
                        tokStart = pWVar18;                   /* dc:45715 */
                    }
                    if (iVar7 != 0) {                         /* dc:45717 token=存在文件 */
                        iVar7 = lstrlenW(pWVar18);
                        PECMD_AllocString(&loadBuf, (int64_t)iVar7 + 0xd); /* dc:45719 */
                        memcpy(loadBuf, L"LOAD  ", 0xc);      /* dc:45720 FUN_14001d78c 6 WCHAR */
                        /* dc:45721: local_208+5 为 WCHAR* 算术(=字节+10), 覆盖第 2 空格 */
                        memcpy((char *)loadBuf + 10, pWVar18, (size_t)(iVar7 + 1) * 2);
                        tokStart = loadBuf;                   /* dc:45722 */
                        pWVar18 = loadBuf;                    /* dc:45723 */
                    }
                    PECMD_FreeStrBuf(&tokCopy);               /* dc:45725 */
                }
                else {
                    /* dc:45728-45732 /L form: 抹掉 "/X" 两字符后跳空白 */
                    pWVar18[1] = L' ';
                    pWVar18[0] = L' ';
                    tokStart = pWVar18 + 2;
                    FUN_14005B154(&tokStart);
                    pWVar18 = tokStart;
                }
                /* dc:45734-45737 文本缓冲 ← pWVar18 全串 */
                iVar7 = lstrlenW(pWVar18);
                PECMD_AllocString((WCHAR **)&sdText[0], (int64_t)iVar7 + 3); /* dc:45735 FUN_140063720 */
                memcpy((void *)(uintptr_t)sdText[0], pWVar18, (size_t)(iVar7 + 1) * 2); /* dc:45736 */
                ((WCHAR *)(uintptr_t)sdText[0])[iVar7 + 1] = L'\0'; /* dc:45737 */
                bVar31 = 0;                                   /* dc:45738 */
                if (bVar23 != 0) {
                    /* dc:45739-45767 首行拼接: local_1f8 首行覆盖 cmdFull 尾部 */
                    WCHAR *eol;
                    int64_t lineChars;
                    int64_t newCap;
                    uint64_t copyBytes;
                    S7_SemicolonRunToNewline((LPCWSTR)(uintptr_t)sdText[0]); /* dc:45740 FUN_14005e51c */
                    eol = (WCHAR *)(uintptr_t)sdText[0];      /* dc:45742 pWVar21 */
                    autoFirst = eol[0];
                    while ((autoFirst != L'\0') && (autoFirst != L'\r') && (autoFirst != L'\n')) {
                        eol++;                                /* dc:45744 */
                        autoFirst = eol[0];                   /* dc:45745 */
                    }
                    iVar7 = (int)(((int64_t)(intptr_t)((char *)eol - (char *)(uintptr_t)sdText[0])) >> 1); /* dc:45747 */
                    lineChars = (int64_t)iVar7 + tokOff;      /* dc:45748 lVar1 */
                    newCap = lineChars + 1;
                    PECMD_AllocString(&cmdFull, newCap);      /* dc:45750 */
                    copyBytes = (uint64_t)(uint32_t)iVar7 * 2u; /* dc:45754 uVar19 */
                    if (0 < iVar7) {
                        memcpy((char *)cmdFull + (size_t)tokOff * 2, (void *)(uintptr_t)sdText[0],
                               (size_t)copyBytes);            /* dc:45755 */
                        cmdFull[lineChars] = L'\0';           /* dc:45756 */
                        if (bVar23 == 0x4c) {                 /* dc:45757 大写 L: 余行留在 sdText */
                            int restLen = lstrlenW(eol);      /* dc:45758 */
                            int64_t mvBytes = ((int64_t)restLen + 1) * 2; /* dc:45759 */
                            memmove((void *)(uintptr_t)sdText[0], eol, (size_t)(int)mvBytes); /* dc:45760 */
                            memset((char *)(uintptr_t)sdText[0] + mvBytes, 0, (size_t)copyBytes); /* dc:45761 */
                            bVar31 = 0;                       /* dc:45762 bVar22 恒 0 */
                            goto LAB_f389;                    /* dc:45763 */
                        }
                    }
                    bVar31 = 0;                               /* dc:45766 */
                    bVar2 = bVar3;                            /* dc:45752 */
                }
            LAB_f389:                                         /* dc:45768 LAB_14004f389 */
                ;
            }
            ppDesc = sdText;                                  /* dc:45769 ppWVar24=&local_1f8 */
            bVar31 = (uint8_t)(bVar31 | 1);                      /* dc:45770 */
            PECMD_FreeStrBuf(&loadBuf);                       /* dc:45771 FUN_14005b104(&local_208) */
        }
LAB_post_sel:
        ;                                                     /* dc:45773 起 */
        pWVar21 = *(WCHAR **)(void *)ppDesc;                  /* dc:45773 */
        FUN_140073CCC(&g_Script, cmdFull, 1);                 /* dc:45775 参数表构建(完整命令行) */
        if (bVar2) {
            uVar29 = 0x10040;                                 /* dc:45777 */
            pWVar18 = (WCHAR *)L"**#101";                     /* dc:45778 pwVar20 */
        }
        else {
            pCur = *(WCHAR **)(void *)ppDesc;                 /* dc:45781 local_1e0 */
            FUN_14005B154(&pCur);                             /* dc:45781 FUN_1400170b0 */
            pWVar21 = pCur;                                   /* dc:45782 */
            iVar7 = lstrlenW(*(WCHAR **)(void *)ppDesc);      /* dc:45783 */
            for (pWVar18 = pWVar21 + (int64_t)iVar7 - 1;      /* dc:45784 尾部空白/控制符裁剪 */
                 (pWVar21 <= pWVar18) &&
                 ((((uint16_t)*pWVar18 > 8) && ((uint16_t)*pWVar18 < 0xe)) || (pWVar18[0] == L' '));
                 pWVar18 = pWVar18 - 1) {
                pWVar18[0] = L'\0';                           /* dc:45788 */
            }
            uVar29 = 0x40;                                    /* dc:45790 */
            pWVar18 = (WCHAR *)L"**cmdline*";                 /* dc:45791 */
        }
        PECMD_SetCurFileVariables(&g_Script, (LPCWSTR)pWVar18, uVar29); /* dc:45793 */
        pWVar21 = *(WCHAR **)(void *)ppDesc;                  /* dc:45794 */
        if (0 < (int)(((int64_t)(intptr_t)((char *)pWVar21 -
                                            (char *)(uintptr_t)*(int64_t **)ppDesc)) >> 1)) {
            iVar7 = lstrlenW(pWVar21);
            memmove((void *)(uintptr_t)*(int64_t **)ppDesc, pWVar21,
                    (size_t)(iVar7 * 2 + 2));                 /* dc:45795-45796 压实到缓冲头 */
        }
        pWVar21 = *(WCHAR **)(void *)ppDesc;                  /* dc:45798 */
        FUN_1400702B0(&lineBuf, L"\n\n");                     /* dc:45799 */
        FUN_140017CDC(clone, &g_Script);                      /* dc:45800 克隆脚本结构 */
        FUN_1400186BC(clone, (void *)&g_Script);              /* dc:45801 子上下文初始化 */
        uVar29 = local210;                                    /* dc:45802 */
        uVar28 = local210 & 0xffffu;                          /* dc:45803 */
        *(int64_t *)(void *)((char *)clone + 0x40) = 0;       /* dc:45804 local_d8 */
        PECMD_InitObfuscatedKeywords(clone, uVar28);          /* dc:45805 FUN_14006159c */
        *(int16_t *)(void *)((char *)clone + 0x48) = (int16_t)uVar29; /* dc:45806 local_d0 */
        PECMD_MaskScriptEndFileTail(clone, pWVar21, false);   /* dc:45807 FUN_14002487c */
        uVar19 = PECMD_StrChrOffset((const WCHAR *)(uintptr_t)*(int64_t **)ppDesc,
                                    (WCHAR)(int16_t)uVar29);  /* dc:45808 FUN_14001b4f8 */
        uVar25run = seedFlags;
        uVar8 = PECMD_ParseScriptSegments((int64_t *)ppDesc, 0, (int)uVar19, (int64_t *)&lineBuf,
                                          (LPCWSTR)clone, uVar25run); /* dc:45809 import 合并 */
        uVar29 = local21c;                                    /* dc:45810 */
        FUN_14001B5AC((LPCWSTR)(uintptr_t)*(int64_t **)ppDesc,
                      (uint32_t)((local21c & 0xffffu) ^ uVar28),
                      (int64_t)((int)uVar19 + 1));            /* dc:45811 XOR 还原 */
        seedFlags = (uVar29 << 0x10) | ((uVar25run | (uVar8 & 0xffffu)) & 2u); /* dc:45812 */
        FUN_14004EAA8(clone, 0);                              /* dc:45814 重置克隆 */
        PECMD_FreeStrBuf(&scratchA8);                         /* dc:45815 (NULL 安全) */
        PECMD_FreeStrBuf(&lineBuf);                           /* dc:45816 */
        PECMD_AppendLongDecimal(&g_Script, (int64_t)local236, WSTR("&PeExe")); /* dc:45817 */
        pWVar21 = *(WCHAR **)(void *)ppDesc;                  /* dc:45818 */
        ppDesc[0] = 0;                                        /* dc:45820 takeover: descriptor.ptr=NULL */
        S7_MARK("efda-preRST"); /* TEMP PROBE(bisect) */
#if S7_BRIDGE_EXECUTOR
        /* 桥接(S7 现实约束, 详见文件头 SKIP 登记):
         * 原文 dc:45821 走 FUN_1400b638c(RunScriptText), 其还原体 core_execmain.c
         * 把行执行委托给 PECMD_DispatchExpressionBlock=@FUN_1400b1724 —— 该符号与
         * 行执行器 FUN_140003a20 在当前工程均为 no-op 桩(unimplemented_stubs.c:397/:350),
         * 文本进入后空转返回(t2probe 实测 RSTI..REL 后无任何 ENVI/EXEC 副作用)。
         * 故此处以同为真体、且为 '$'/装载器路径同族执行器的
         * PECMD_ProcessScriptBlock(@0x14004c0bc, restored_bodies.c 直移体)
         * 驱动同一份装配文本; 文本内容/参数表(FUN_140073CCC)/&PeExe 时序均与原文一致。
         * TODO(verify): 行执行器真体(B8c)就位后恢复下方原文调用。 */
        r = (int64_t)PECMD_ProcessScriptBlock((uint64_t)(uintptr_t)&g_Script,
                                              (uint64_t)(uintptr_t)pWVar21, 0, 0, 0);
#else
        r = PECMD_RunScriptText(&g_Script, (LPCWSTR)pWVar21, (LPCWSTR)cmdFull, NULL,
                                (uint32_t)(((uint32_t)bVar31 & 1u) | seedFlags | 0x80u),
                                NULL, NULL);                  /* dc:45821-45823 FUN_1400b638c */
#endif
        S7_MARK("tail-0a postRST"); /* TEMP PROBE(bisect) */
        if (g_i32D7EC != 0) {                                 /* dc:45824 */
            PECMD_WaitCountPumpMessages((int *)&g_i32D7EC, 0);/* dc:45825 FUN_14001bbdc */
        }
        S7_MARK("tail-1 postRST"); /* TEMP PROBE(bisect) */
        PECMD_ShutdownCom();                                  /* dc:45827 FUN_140064328 */
        S7_MARK("tail-2 postCom"); /* TEMP PROBE(bisect) */
        if (0 < g_taskCount) {                                /* dc:45828 DAT_14013d7e8 */
            g_flagA24F = -1;                                  /* dc:45829 DAT_14013a24f */
        }
        { /* dc:45831-45847 收尾消息泵 (直移) */
            DWORD tickMark = GetTickCount() - 0x14;           /* dc:45831-45832 */
            GetTickCount();                                   /* dc:45833 结果弃用, 保序 */
            while (0 < g_taskCount) {
                DWORD tickNow = GetTickCount();               /* dc:45835 */
                if (tickNow != tickMark) {
                    PostMessageW((HWND)0xffff, 0, 0, 0);      /* dc:45837 HWND_BROADCAST, WM_NULL */
                    PostQuitMessage(0);                       /* dc:45838 */
                    tickMark = tickNow;                       /* dc:45839 */
                }
                MsgWaitForMultipleObjects(0, (const HANDLE *)0, 0, 10, 0x4ff); /* dc:45841 */
                while ((0 < g_taskCount) &&
                       (PeekMessageW(&msg, (HWND)0, 0, 0, 1) != 0)) { /* dc:45842 PM_REMOVE */
                    TranslateMessage(&msg);                   /* dc:45844 */
                    DispatchMessageW(&msg);                   /* dc:45845 */
                }
            }
        }
        S7_MARK("tail-3 postPump"); /* TEMP PROBE(bisect) */
        PECMD_FreeStrBuf((WCHAR **)sdText);                   /* dc:45848 */
        PECMD_FreeStrBuf(&cmdFull);                           /* dc:45849 */
    }

    /* ---- 公共收尾 dc:45851-45858 ---- */
    S7_MARK("tail-4 common"); /* TEMP PROBE(bisect) */
    PECMD_ZeroLenBuf((WCHAR *)(uintptr_t)sdScript[0]);        /* dc:45851 FUN_14005b0b8(local_1d8) */
    S7_MARK("tail-5 zerolen"); /* TEMP PROBE(bisect) */
    /* dc:45852-45853 FUN_14005b7e8/thunk 对两个托盘结构清理。重构端缓冲布局未复刻
     * (DAT_14013d8a0=256B/g_hwndTray=HWND), 真体会 memset 0x3d0 字节越界踩全局;
     * 按项目既有约定走 no-op 桩 PECMD_DestroyTrayIcon (core_b3r_i28e/core_b3_remaining
     * 对同一地址同用法)。TODO(verify): 托盘结构落位后换回真体。 */
    PECMD_DestroyTrayIcon((char *)DAT_14013d8a0);
    PECMD_DestroyTrayIcon((char *)&g_hwndTray);
    FUN_1400E56E4((uint64_t *)&g_i64D170);                    /* dc:45854 */
    S7_MARK("tail-6 e56e4"); /* TEMP PROBE(bisect) */
    PECMD_UninstallInputHook();                               /* dc:45855 FUN_14001bb30 */
    S7_MARK("tail-7 hook"); /* TEMP PROBE(bisect) */
    /* dc:45858 FUN_140026338 收尾 LOGS 指令。
     * SKIP(verify): 还原真体 restored_bodies.c:4755 对本输入("LOGS\0\n\n", 栈拷贝可写)
     * 在收尾路径稳定 AV —— memfail.log 时序实锤(tail-7 后 / tail-8 前, 三次复现),
     * 与只读段无关(已按原文改栈式可写存储仍崩)。该指令语义=ExecCmd 退出时的
     * 日志关闭收尾, 对 t1/t2 验收链路无影响。TODO(verify): FUN_140026338 真体
     * 修复后由主代理恢复此调用。 */
    /* PECMD_ScriptInitParse((uint64_t)(uintptr_t)&g_Script,
     *                       (uint64_t)(uintptr_t)(s_TailLogsCmd + 4), (uint64_t)-1); */
    retOut = r;                                               /* dc:45859 LVar33=LVar12 */

LAB_f74a:                                                     /* dc:45860 LAB_14004f74a */
    { /* TEMP PROBE */
        FILE *pf_ = fopen("C:\\pectest\\memfail.log", "a");
        if (pf_) { fprintf(pf_, "PROBE StartupScript done r=0x%llx\n", (unsigned long long)r); fclose(pf_); }
    }
    PECMD_FreeStrBuf((WCHAR **)sdScript);                     /* dc:45861 */
    PECMD_FreeStrBuf(&autoRem);                               /* dc:45862 */
    return retOut;                                            /* dc:45863 */
}
