/* ====================================================================
 * core_execmain.c — 脚本执行主入口（ExecMain 族）
 *
 *   FUN_1400B638C @0x1400b638c   脚本执行主入口（7 参数）
 *
 * 语义：执行一段脚本文本。
 *   pScript  : 父脚本对象（flags/变量/__MAIN__ 状态来源）
 *   pText    : 脚本文本（引用计数包装，script+0x78/+0x80）
 *   pName    : 脚本名（传给 ArgBuild 作为命令行，缺省 = pText）
 *   pCurFile : 当前文件路径串（进入时 SetCurFile 并保存
 *              CurDir/CurDrv/CurFile 变量与工作目录，退出时还原）
 *   flags    : bit0  → script+0xe
 *              bit1  → script+0x10 bit7（置位标志）
 *              bit2  → 抑制窗口创建（无窗口执行）
 *              bit4  → script+0xd |= 1，且跳过 &__MAIN__ 处理
 *              bit5  → script+0xda |= 1
 *              bit6  → SetCurFile 的 chdir 标志（清位则退出时恢复目录）
 *              bit8  → 与 g_bInitWin&2 组合触发 script 重置
 *              bit9  → 持久化清理模式选择（0=TaskClear，置位=字段保留）
 *              bit16+→ script+0x48（行分隔符，FindChar 用之）
 *   pFile    : 文件路径（存窗口对象+0x2a0 / 传执行循环）
 *   pPersist : 持久化脚本对象；NULL=新建临时副本执行后销毁；
 *              ==pScript 时收尾恢复原字段；否则复用对象
 * 返回值：script+0xd0（执行结果，调用方 core_scriptrun.c 以 DVar13 接收）。
 *
 * 主要分支：
 *   1. 窗口创建（有父窗口且无 bit2）：分配 0xa98 窗口对象，
 *      vtbl+0x10 虚调用显示，挂 script 到窗口（+0x290/+0x200）
 *   2. 脚本对象选择：pPersist 为空则 ScriptCopy+ScriptInit 新建
 *      0xf0 副本（执行后 TaskClear+free）
 *   3. &__MAIN__ 初始化：未定义置 "1"，值≠"0" 置 "0"（反编译原样）
 *   4. ENTER:/LEAVE: 调试日志（g_logEnter>0 且 script+0x10 位0/3 允许）
 *   5. 窗口模式：显示窗口后调执行循环 PECMD_DispatchExpressionBlock；忙则
 *      PostMessage(WM_CLOSE)+PECMD_ModalDialogPump
 *   6. 非窗口模式：直接调执行循环；满足条件时对旧窗口
 *      PostMessage(WM_CLOSE) 并清 script+0x40
 *   7. 收尾：恢复保存字段或 TaskClear；窗口引用计数 ±1；
 *      还原 CurDir/CurDrv/CurFile 变量与工作目录
 * ==================================================================== */
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "pecmd_defs.h"
#include "win32_stub.h"

/* ---- 已实现辅助（core_string.c / core_var*.c / core_exec*.c）---- */
extern void FUN_1400A4020(WCHAR **ps, LPCWSTR src);          /* @0x1400a4020 core_exec4.c */
extern void PECMD_RefCountRelease(WCHAR **ps);                       /* @0x140028270 core_exec4.c */
extern void FUN_140017CDC(void *dst, void *src);            /* @0x140017cdc */
extern void FUN_1400186BC(void *s, void *parent);           /* @0x1400186bc */
extern void FUN_14004EAA8(void *script, int flag);           /* @0x14004eaa8 */
extern int64_t FUN_14001B4F8(const WCHAR *s, WCHAR c);        /* @0x14001b4f8 */
extern uint8_t *FUN_14001E69C(void *script, LPCWSTR name, void *scope,
                               int64_t len);                   /* @0x14001e69c core_var3.c */
extern uint32_t FUN_140073CCC(void *script, LPCWSTR cmdline, int saveArg); /* @0x140073ccc core_exec5.c */
extern void FUN_14002452C(void *script, LPCWSTR curfile, uint32_t flag); /* @0x14002452c core_exec3.c */
extern void PECMD_SetEnvIfChanged(LPCWSTR name, LPCWSTR value);      /* @0x140061508 core_exec3.c */
extern int32_t FUN_14005C7C4(const char *a, const WCHAR *w); /* @0x14005c7c4 core_exec5.c */
extern void FUN_1400668EC(void *script, uint64_t value, LPCWSTR key,
                           LPCWSTR fmt);                       /* @0x1400668ec core_var.c */
extern void FUN_1400629B8(void *script, LPCWSTR key, LPCWSTR value); /* @0x1400629b8 */
extern void FUN_140053E78(void);                                 /* @0x140053e78 core_sys.c */

/* ---- 全局 ---- */
extern int32_t g_bInitWin;               /* DAT_14013d059 窗口初始化标志 (core_script2.c static) */
extern int32_t g_logEnter;               /* g_i64CCB8 ENTER:/LEAVE: 日志开关 */
extern WCHAR g_szEmpty[];                /* g_szEmpty .rdata 空串 */

/* 未实现（TODO(verify) 挂起） */
extern void PECMD_ResetScriptChain(void *script, void *a2);        /* @0x14001b3a0 脚本重置 */
extern void PECMD_ShowAboutDialog(void);                          /* @0x1400e67e8 窗口系统准备 */
extern void *PECMD_InitControlObjField(void *obj, HWND parent, uint32_t msg,
                           void *a4);                     /* @0x1400731d8 窗口对象构造 */
extern void PECMD_ModalMsgPumpEx(void *win, int a2);             /* @0x1400e95f4 窗口销毁 */
extern void PECMD_ModalDialogPump(void *win, uint32_t msg);       /* @0x1400e91f0 窗口消息处理 */
extern void PECMD_DispatchExpressionBlock(void *script, LPCWSTR p);       /* @0x1400b1724 脚本执行循环 */
extern void FUN_140025f10(void *script, LPCWSTR line, int mode,
                          void *a4, void *a5, void *a6);  /* @0x140025f10 行执行 */
extern void PECMD_WaitTickCount(void);                          /* @0x140061470 */
extern void FUN_14001a640(WCHAR *buf);                    /* @0x14001a640 恢复当前目录 */
extern void PECMD_SyncWorkingDirectory(void);                          /* @0x14001e2cc */
extern void FUN_14009BB28(void *script, int flag);        /* @0x14009bb28 脚本清理 */

/* ========== FUN_1400B638C @0x1400b638c ==========
 * 脚本执行主入口。详见文件头注释。
 */
int64_t FUN_1400B638C(void *pScript, LPCWSTR pText, LPCWSTR pName, LPCWSTR pCurFile,
                       uint32_t flags, LPCWSTR pFile, void *pPersist)
{
    uint8_t cInitFlag;          /* 初始 g_flag16a */
    uint32_t noWin;             /* flags & 4 */
    uint32_t persistMode;       /* flags&0x200 时的收尾模式 */
    int64_t persistVal8;        /* 原 pPersist[8]（+0x40） */
    void *pPersistSaved;        /* 进入时的 pPersist */
    void *pWinOld;              /* 原 script+0x40 */
    bool hasWin;
    void *pWinRef;              /* 引用计数窗口（原窗口，cInitFlag==0 时） */
    void *pWinRef2;             /* 已 +1 计数的窗口 */
    void *pWinNew;              /* 新建窗口对象 */
    void *pWin;                 /* 实际使用的窗口 */
    WCHAR *bufRef;              /* 脚本文本引用容器（0x10 头，[0]=文本指针） */
    WCHAR *textPtr;             /* *bufRef 文本指针 */
    WCHAR *savedCwd = NULL;     /* 当前工作目录 */
    WCHAR *savedCurDir = NULL;  /* &CurDir 变量值 */
    WCHAR *savedCurDrv = NULL;  /* &CurDrv 变量值 */
    WCHAR *savedCurFile = NULL; /* &CurFile 变量值 */
    LPCWSTR nameTmp;            /* 反编译 local_res18（pName，ArgBuild/LEAVE 用） */
    LPCWSTR curFileTmp;         /* 反编译 local_res20（pCurFile，ENTER 用） */
    void *saved78, *saved80;    /* script+0x78/+0x80 原值 */
    uint8_t savedD, savedDa;    /* script+0xd/+0xda 原值 */
    int32_t saved8;             /* script+8 原值 */
    int64_t oldWinSaved;        /* script+0x40 原值 */
    int64_t sepPos;             /* FindChar 结果 → script+0x8c */
    LPCWSTR pWinName;           /* 窗口名（= pCurFile ?: 空串） */
    bool bEnterLog;             /* 允许 ENTER:/LEAVE: 日志 */
    void *pExec;                /* 实际执行脚本对象（pPersist ?: pScript） */
    void *pScriptNew;           /* 新建的临时脚本对象 */
    void *pRefWin, *pRefWin2;   /* 窗口计数收尾 */
    int64_t ret;
    uint8_t *node;
    int64_t curWin;

    cInitFlag = g_flag16a;
    noWin = flags & 4;
    persistMode = 0x200;
    if (pPersist == pScript) {
        persistMode = 1;
    }
    persistMode = (flags & 0x200) ? persistMode : 0;
    pPersistSaved = pPersist;

    /* script+0x10 高位置 (flags>>1)&1 */
    *(uint8_t *)((char *)pScript + 0x10) &= 0x7f;
    *(uint8_t *)((char *)pScript + 0x10) |=
        (uint8_t)((uint16_t)flags >> 1) << 7;

    persistVal8 = (pPersist != NULL) ? *(int64_t *)((char *)pPersist + 0x40) : 0;

    curFileTmp = pCurFile;
    /* 窗口初始化状态：g_bInitWin 置 0x80；(原&2) 或 flags&0x100 时重置脚本 */
    if ((int8_t)(uint8_t)flags >= 0 && g_bInitWin != 0) {
        int32_t prevInit = g_bInitWin & 2;
        g_bInitWin |= 0x80;
        if (prevInit != 0 || (flags & 0x100) != 0) {
            g_bInitWin = 0;
            PECMD_ResetScriptChain(pScript, NULL);
        }
    }

    pWinOld = *(void **)((char *)pScript + 0x40);
    if (pName == NULL) {
        pName = pText;
    }
    if (pFile == NULL) {
        pFile = pText;
    }
    hasWin = (pWinOld != NULL);
    bufRef = NULL;
    pWinRef = NULL;
    if (cInitFlag == '\0') {
        pWinRef = pWinOld;
    }
    nameTmp = pName;
    FUN_1400A4020(&bufRef, pText);
    if (pCurFile != NULL) {
        PECMD_AllocString(&savedCwd, 0x20a);
        GetCurrentDirectoryW(0x208, savedCwd);
    }
    if (pCurFile != NULL) {
        EnterCriticalSection(&g_csInit);
        node = FUN_14001E69C(pScript, WSTR("&CurDir"), NULL, -1);
        if (node != NULL) {
            FUN_1400703E4(&savedCurDir, (LPCWSTR)*(WCHAR **)node);
        }
        node = FUN_14001E69C(pScript, WSTR("&CurDrv"), NULL, -1);
        if (node != NULL) {
            FUN_1400703E4(&savedCurDrv, (LPCWSTR)*(WCHAR **)node);
        }
        node = FUN_14001E69C(pScript, WSTR("&CurFile"), NULL, -1);
        if (node != NULL) {
            FUN_1400703E4(&savedCurFile, (LPCWSTR)*(WCHAR **)node);
        }
        LeaveCriticalSection(&g_csInit);
    }

    /* 窗口创建（有父窗口且无 noWindow 标志） */
    pWinNew = NULL;
    pWinRef2 = NULL;
    if (hasWin && (flags & 4) == 0) {
        void *obj;
        PECMD_ShowAboutDialog();
        obj = calloc(1, 0xa98);          /* operator_new(0xa98) 窗口对象 */
        if (obj == NULL) {
            pWinNew = NULL;
        } else {
            pWinNew = PECMD_InitControlObjField(obj, GetDesktopWindow(), 0x271c, NULL);
        }
    }
    pWin = pWinOld;
    if (pWinNew != NULL) {
        pWin = pWinNew;
    }

    /* 持久化脚本对象：无则新建临时副本（ScriptCopy+ScriptInit） */
    pScriptNew = NULL;
    if (pPersist == NULL) {
        void *obj = calloc(1, 0xf0);     /* operator_new(0xf0) 脚本对象 */
        pPersist = NULL;
        if (obj != NULL) {
            FUN_140017CDC(obj, pScript);
            FUN_1400186BC(obj, pScript);
            *(void **)((char *)obj + 0x40) = pWin;
            pScriptNew = obj;
            pPersist = obj;
        }
    } else {
        *(void **)((char *)pPersist + 0x40) = pWin;
    }
    pExec = pScript;
    if (pPersist != NULL) {
        pExec = pPersist;
    }

    *(uint16_t *)((char *)pExec + 0x48) = (uint16_t)(flags >> 0x10);
    oldWinSaved = *(int64_t *)((char *)pExec + 0x40);
    *(void **)((char *)pExec + 0x40) = pWin;

    /* 保存执行期间被替换的脚本字段，收尾恢复 */
    EnterCriticalSection(&g_csInit);
    saved78 = *(void **)((char *)pExec + 0x78);
    saved80 = *(void **)((char *)pExec + 0x80);
    savedD = *(uint8_t *)((char *)pExec + 0xd);
    saved8 = *(int32_t *)((char *)pExec + 8);
    savedDa = *(uint8_t *)((char *)pExec + 0xda);
    LeaveCriticalSection(&g_csInit);
    *(void **)((char *)pExec + 0x78) = &bufRef;   /* 引用调用方文本缓冲 */
    textPtr = *(WCHAR **)bufRef;                  /* 容器 [0] = 文本指针 */
    *(void **)((char *)pExec + 0x80) = textPtr;
    FUN_140073CCC(pExec, nameTmp, 1);
    sepPos = FUN_14001B4F8(textPtr, *(uint16_t *)((char *)pExec + 0x48));
    *(int32_t *)((char *)pExec + 0x8c) = (int32_t)sepPos;

    if (hasWin) {
        *(LPCWSTR *)((char *)pWin + 0x2a0) = pFile;
    }
    *(uint32_t *)((char *)pExec + 0x24) = 0;
    if ((flags & 0x10) != 0) {
        *(uint8_t *)((char *)pExec + 0xd) |= 1;
    }
    if ((flags & 0x20) != 0) {
        *(uint8_t *)((char *)pExec + 0xda) |= 1;
    }
    if (pCurFile != NULL) {
        FUN_14002452C(pExec, curFileTmp, (uint16_t)flags & 0x40);
    }
    pWinName = g_szEmpty;
    if (curFileTmp != NULL) {
        pWinName = curFileTmp;
    }

    /* 窗口挂接脚本对象并计数 */
    if (hasWin && pWinNew != NULL) {
        *(void **)((char *)pWin + 0x290) = pExec;
        *(int64_t *)(*(void **)((char *)pWin + 0x200)) = (int64_t)pExec;
        *(uint8_t *)((char *)pWin + 0x122) = 1;
        *(uint8_t *)((char *)pWin + 0x121) = 0xff;
        FUN_140053E78();
        if (pWinRef != NULL) {
            EnterCriticalSection(&g_csInit);
            *(int64_t *)((char *)pWinRef + 0x1c8) += 1;
            pWinRef2 = pWinRef;
            LeaveCriticalSection(&g_csInit);
        }
    }

    if (*(char *)((char *)pScript + 0xf) != '\0') {
        *(char *)((char *)pExec + 0xf) = *(char *)((char *)pScript + 0xf);
    }
    *(uint8_t *)((char *)pExec + 0xe) = (uint8_t)flags & 1;

    /* &__MAIN__ 初始化（未定义→"1"，值≠"0"→"0"，反编译原样 TODO(verify)） */
    if ((flags & 0x10) == 0) {
        EnterCriticalSection(&g_csInit);
        node = FUN_14001E69C(pScript, WSTR("&__MAIN__"), NULL, -1);
        if (node == NULL) {
            FUN_1400629B8(pExec, WSTR("&&__MAIN__"), WSTR("1"));
        } else {
            if ((char)FUN_14005C7C4("0", (LPCWSTR)*(WCHAR **)node) == '\0') {
                FUN_1400629B8(pExec, WSTR("&&__MAIN__"), WSTR("0"));
            }
        }
        LeaveCriticalSection(&g_csInit);
    }
    if ((*(uint8_t *)((char *)pScript + 0x10) & 0x80) != 0) {
        *(uint8_t *)((char *)pExec + 0x10) &= 0xfe;
    }

    /* ENTER: 日志（调试开关且 script+0x10 位3 清零/位0 置位/未进入过） */
    bEnterLog = false;
    if (pWinName[0] != L'\0' &&
        (*(uint8_t *)((char *)pExec + 0x10) & 8) == 0 &&
        *(char *)((char *)pExec + 0xd8) != '\0') {
        bEnterLog = ((*(uint8_t *)((char *)pExec + 0x10) & 1) != 0);
    }
    if (g_logEnter > 0 && bEnterLog) {
        FUN_1400702B0((WCHAR **)&curFileTmp, WSTR("ENTER:"));
        FUN_14006375C((WCHAR **)&curFileTmp, pWinName);
        FUN_140025f10(pExec, curFileTmp, 0, (void *)(intptr_t)0x11, NULL, NULL);
        FUN_14005B104((WCHAR **)&curFileTmp);
    }
    *(uint8_t *)((char *)pExec + 0xd8) = 1;

    /* 窗口模式：显示窗口并执行脚本 */
    if (hasWin && pWinNew != NULL) {
        if (cInitFlag == '\0') {
            *(uint8_t *)((char *)pWin + 0x120) = 0;
            PECMD_ModalMsgPumpEx(pWin, 0);
        } else {
            void *vtbl;
            *(uint8_t *)((char *)pWin + 0x121) = 0x81;
            *(uint8_t *)((char *)pWin + 0x120) = 0x10;
            vtbl = *(void **)pWin;
            (*(void (**)(void *, uint32_t, int))(*(void **)((char *)vtbl + 0x10)))(
                pWin, 0x271d, 0);
            FUN_1400668EC(pExec, *(int64_t *)((char *)pWin + 0x20),
                            WSTR("&&__WinID"), WSTR("0x%I64X"));
            *(int64_t *)((char *)pWin + 0x2a0) = 0;
            PECMD_DispatchExpressionBlock(pExec, pFile);
            if ((*(int64_t *)((char *)pWin + 0x1c8) > 0 ||
                 **(int32_t **)((char *)pWin + 0x1a8) > 0) ||
                **(int32_t **)((char *)pWin + 0x1c0) > 0) {
                *(uint8_t *)((char *)pWin + 0x121) = 0xff;
                PostMessageW(*(HWND *)((char *)pWin + 0x20), 0x10, 0, 0); /* WM_CLOSE */
                PECMD_ModalDialogPump(pWin, 0x20);
                *(uint8_t *)((char *)pWin + 0x121) = 0x81;
            }
        }
    } else {
        /* 非窗口模式：直接执行 */
        PECMD_DispatchExpressionBlock(pExec, pFile);
        curWin = *(int64_t *)((char *)pExec + 0x40);
        if (curWin != 0 &&
            noWin == 0 &&
            (*(uint8_t *)((char *)pExec + 0xd9) & 1) != 0 &&
            curWin != *(int64_t *)(*(void **)((char *)pExec + 0x38) + 0x40) &&
            oldWinSaved != curWin &&
            (void *)pExec != pScript &&
            curWin != *(int64_t *)((char *)pScript + 0x40)) {
            PostMessageW(*(HWND *)(curWin + 0x20), 0x10, 0, 0);
            pWinNew = (void *)curWin;
            *(int64_t *)((char *)pExec + 0x40) = 0;
        }
        if (oldWinSaved == 0 && *(int64_t *)((char *)pExec + 0x40) != 0) {
            oldWinSaved = *(int64_t *)((char *)pExec + 0x40);
        }
    }

    /* LEAVE: 日志（条件同 ENTER） */
    if (pWinName[0] != L'\0' && bEnterLog) {
        FUN_1400702B0((WCHAR **)&nameTmp, WSTR("LEAVE:"));
        FUN_14006375C((WCHAR **)&nameTmp, pWinName);
        FUN_140025f10(pExec, nameTmp, 0, (void *)(intptr_t)0x11, NULL, NULL);
        FUN_14005B104((WCHAR **)&nameTmp);
    }
    ret = *(int32_t *)((char *)pExec + 0xd0);

    /* 收尾：恢复被替换字段（pPersist==NULL 或 persistMode==1）或 TaskClear */
    if (pPersist == NULL || persistMode == 1) {
        EnterCriticalSection(&g_csInit);
        *(void **)((char *)pExec + 0x80) = saved80;
        *(uint8_t *)((char *)pExec + 0xd) = savedD;
        *(void **)((char *)pExec + 0x78) = saved78;
        *(uint8_t *)((char *)pExec + 0xda) = savedDa;
        LeaveCriticalSection(&g_csInit);
    } else if (persistMode == 0) {
        FUN_14004EAA8(pExec, saved8);
    }
    if (*(int64_t *)((char *)pExec + 0x40) != 0) {
        *(int64_t *)((char *)pExec + 0x40) = oldWinSaved;
    }
    if (pPersistSaved != NULL) {
        *(int64_t *)((char *)pPersistSaved + 0x40) = persistVal8;
    }
    if (pScriptNew != NULL) {
        FUN_14004EAA8(pScriptNew, 0);
        FUN_14005B104((WCHAR **)((char *)pScriptNew + 0x70));
        free(pScriptNew);
    }
    if (pWinNew != NULL) {
        void *vtbl = *(void **)pWinNew;
        (*(void (**)(void *))(*(void **)((char *)vtbl + 0x18)))(pWinNew);
    }

    /* 窗口引用计数递减；状态非 0x81 时通知窗口（0x43d） */
    pRefWin = pWinRef2;
    pRefWin2 = pWinRef;
    if (pWinRef2 == NULL) {
        if (pWinRef == NULL) {
            goto ref_done;
        }
        EnterCriticalSection(&g_csInit);
    } else {
        EnterCriticalSection(&g_csInit);
        *(int64_t *)((char *)pRefWin + 0x1c8) -= 1;
        pRefWin2 = pRefWin;
    }
    if (*(int8_t *)((char *)pRefWin2 + 0x121) != (int8_t)0x81) {
        PostMessageW(*(HWND *)((char *)pRefWin2 + 0x20), 0x43d, 0, 0);
    }
    LeaveCriticalSection(&g_csInit);
ref_done:
    PECMD_WaitTickCount();

    /* 还原 CurDir/CurDrv/CurFile 变量；persistMode 时同步持久化对象 */
    if (pCurFile != NULL) {
        bool syncPersist;
        EnterCriticalSection(&g_csInit);
        syncPersist = (persistMode != 0) && (pPersistSaved != NULL);
        node = FUN_14001E69C(pScript, WSTR("&CurDir"), NULL, -1);
        if (node != NULL) {
            PECMD_SetEnvIfChanged(WSTR("CurDir"), savedCurDir);
            if (syncPersist) {
                FUN_1400629B8(pPersistSaved, WSTR("&&CurDir"), savedCurDir);
            }
        }
        node = FUN_14001E69C(pScript, WSTR("&CurDrv"), NULL, -1);
        if (node != NULL) {
            PECMD_SetEnvIfChanged(WSTR("CurDrv"), savedCurDrv);
            if (syncPersist) {
                FUN_1400629B8(pPersistSaved, WSTR("&&CurDrv"), savedCurDrv);
            }
        }
        node = FUN_14001E69C(pScript, WSTR("&CurFile"), NULL, -1);
        if (node != NULL) {
            PECMD_SetEnvIfChanged(WSTR("CurFile"), savedCurFile);
            if (syncPersist) {
                FUN_1400629B8(pPersistSaved, WSTR("&&CurFile"), savedCurFile);
            }
        }
        if ((flags & 0x40) == 0) {
            FUN_14001a640(savedCwd);
            PECMD_SyncWorkingDirectory();
        }
        LeaveCriticalSection(&g_csInit);
    }
    if (*(int64_t *)((char *)pScript + 0x40) != 0) {
        *(int64_t *)((char *)pScript + 0x40) = (int64_t)pWinOld;
    }
    if (cInitFlag == '\0') {
        FUN_14009BB28(pScript, 0);
    }
    FUN_14005B104(&savedCurFile);
    FUN_14005B104(&savedCurDrv);
    FUN_14005B104(&savedCurDir);
    FUN_14005B104(&savedCwd);
    PECMD_RefCountRelease(&bufRef);
    return ret;
}
