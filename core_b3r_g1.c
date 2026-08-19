/*
 * core_b3r_g1.c — Restored batch28 business functions (group g1)
 *
 *   FUN_1400644c8 @0x1400644c8  通过 COM/OLE 全局槽执行一次"写入"操作 (带临界区)
 *   PECMD_PositionMessageWindow @0x140064c38  消息/工具提示窗口定位与前台窗口恢复
 *
 * 说明:
 *   - FUN_140102a90(...) 是 memset 别名, 直接写 memset。
 *   - g_pOleInit / g_pOleUninit 为 OleInit/OleUninit 延迟槽。
 *   - PECMD_DestroyWindowLocked / FUN_14005B9C8 / PECMD_TimerMessage 定义于 core_b3d/e/j.c (大写)。
 *   - 消息缓冲区 (local_84/local_88/local_80/local_78/local_58) 的栈布局偏移
 *     无法从伪代码精确恢复, 以字节缓冲重建并用 TODO(verify) 标注。
 */

#include <stdbool.h>
#include "win32_stub.h"
#include "pecmd_defs.h"

/* ---- 本文件引用的全局 (link_stubs.c / core_globals.c 定义) ---- */
extern void * g_pComWrite;               /* 全局 fnptr 槽 (COM 操作) */
extern CRITICAL_SECTION g_csE138;        /* 全局临界区 (工具提示) */
extern HWND   g_hwndD310;                  /* 当前工具提示/消息窗口 */
extern uint8_t g_msgWndMode;                 /* 消息窗口模式字节 */
extern uint32_t g_msgWndState;                /* 消息窗口状态/定时器标志 */

/* ---- 本文件引用的辅助函数 (定义于其它文件, 仅 extern) ---- */
extern void   FUN_140061c44(void);                            /* @0x140061c44 OLE 初始化协助 */
extern HWND   FUN_14005B9C8(uint32_t flags, int maxWidth);   /* @0x14005b9c8 获取提示窗口 */
extern void   PECMD_DestroyWindowLocked(void);                            /* @0x14005ba6c 销毁窗口 */
extern void   PECMD_TimerMessage(HWND hwnd, uint64_t wParam, int timerId); /* @0x14005bb6c 定时器过程 */
extern DWORD  FUN_1400195f0(uint64_t param_1, int64_t param_2, int param_3,
                            uint64_t *param_4);               /* @0x1400195f0 消息泵/等待 */

/* ================================================================
 * @0x1400644c8  (undefined4 FUN_1400644c8(longlong, undefined8))
 * 通过全局 COM 槽 (g_pComWrite) 执行一次写入操作; 槽未初始化 (<2) 或
 * 目标对象 (param_1+0xd0) 为空时直接返回 1。
 * ================================================================ */
uint32_t FUN_1400644c8(int64_t param_1, uint64_t param_2)
{
    int iVar1;
    uint32_t uVar2;

    if (((int64_t)(uintptr_t)g_pComWrite < 2) || (*(int64_t *)(param_1 + 0xd0) == 0)) {
        uVar2 = 1;
    }
    else {
        EnterCriticalSection((LPCRITICAL_SECTION)&g_csCom);
        FUN_140061c44();
        iVar1 = -3;
        if (g_pOleInit != (int (*)(int))0) {
            iVar1 = (*g_pOleInit)(0);
            if ((iVar1 != 0) && (iVar1 == 1)) {
                (*g_pOleUninit)();
            }
        }
        uVar2 = (uint32_t)((uint32_t (*)(int64_t *, int, uint64_t, int, int, int, int,
                                         int, int))g_pComWrite)(
                    (int64_t *)(param_1 + 0xd0), 2, param_2, 0, 0, 0, 0, 0, 0);
        if ((g_pOleUninit != (void (*)(void))0) && (iVar1 == 0)) {
            (*g_pOleUninit)();
        }
        LeaveCriticalSection((LPCRITICAL_SECTION)&g_csCom);
    }
    return uVar2;
}

/* ================================================================
 * @0x140064c38  (void PECMD_PositionMessageWindow(HWND, undefined8, LPARAM, uint, int,
 *                                  int, uint, uint, undefined8))
 * 移动/定位工具提示窗口; 若处于"非前台/消息窗口"状态则先恢复先前的
 * 前台窗口 (local_90 = GetForegroundWindow(), 结束 SetForegroundWindow)。
 * ================================================================ */
void PECMD_PositionMessageWindow(HWND param_1, uint64_t param_2, LPARAM param_3, unsigned int param_4,
                   int param_5, int param_6, unsigned int param_7, unsigned int param_8,
                   uint64_t param_9)
{
    bool bVar1;
    HWND hWnd;
    int iVar2;
    int iVar3;
    DWORD DVar4;
    DWORD DVar5;
    uintptr_t UVar6;
    short sVar7;
    RECT local_b8;
    unsigned int local_a8;
    unsigned int local_a4;
    unsigned int local_a0;
    unsigned int local_9c;
    unsigned int local_98;
    HWND local_90;
    uint8_t local_buf[0x50];      /* 消息缓冲区 (重建, 见文件头说明) */
    uint32_t *local_88;
    uint32_t *local_84;
    HWND     *local_80;
    uint64_t *local_78;
    uint64_t *local_58;

    local_9c = param_8 & 0x40;
    local_a0 = param_8 & 0x80;
    local_98 = param_8 & 4;
    local_a8 = param_8 & 8;
    local_a4 = param_8 & 0x100;
    /* 原逻辑: 若 bit0x10 或 bit0x20 任一为 0 则 bVar1=false;
       等价于 bVar1 为 true 当且仅当两者同时置位。 */
    bVar1 = ((param_8 & 0x10) != 0) && ((param_8 & 0x20) != 0);

    sVar7 = 0;
    EnterCriticalSection((LPCRITICAL_SECTION)&g_csInit);
    if ((g_hwndD310 != (HWND)0) && ((int)g_msgWndMode != (int)(param_8 & 1))) {
        PECMD_DestroyWindowLocked();
    }
    LeaveCriticalSection((LPCRITICAL_SECTION)&g_csInit);

    if ((g_msgWndState & 2) != 0) {
        EnterCriticalSection((LPCRITICAL_SECTION)&g_csInit);
        if ((g_msgWndState & 2) != 0) {
            g_msgWndState = (g_msgWndState & 0xfffc) + 6;
        }
        LeaveCriticalSection((LPCRITICAL_SECTION)&g_csInit);
        while ((g_msgWndState & 2) != 0) {
            FUN_1400195f0((uint64_t)(uintptr_t)g_Script, 1, 0, (uint64_t *)0);
        }
    }

    EnterCriticalSection((LPCRITICAL_SECTION)&g_csInit);
    local_90 = GetForegroundWindow();
    g_msgWndState = 0;
    if ((g_hwndD310 != (HWND)0) ||
        (FUN_14005B9C8(-(unsigned int)bVar1 & 2 | (param_8 & 1), 0x514),
         g_hwndD310 != (HWND)0)) {
        hWnd = g_hwndD310;
        /* 消息缓冲区布局 (TODO(verify): 偏移无法精确恢复) */
        local_88 = (uint32_t *)(void *)local_buf;            /* offset 0  */
        local_84 = (uint32_t *)(void *)(local_buf + 4);      /* offset 4  */
        local_80 = (HWND *)(void *)(local_buf + 8);          /* offset 8  */
        local_78 = (uint64_t *)(void *)(local_buf + 0x0c);   /* offset 12 (TODO(verify)) */
        local_58 = (uint64_t *)(void *)(local_buf + 0x2c);   /* offset 44 */
        memset(local_buf, 0, 0x44);
        *local_88 = 0x2c;
        *local_78 = param_9;
        *local_84 = 0x123;
        *local_80 = param_1;
        *local_58 = param_2;

        SendMessageW(hWnd, 0x432, 0, (LPARAM)&local_88[0]);
        SendMessageW(g_hwndD310, 0x411, 0, 0);
        SendMessageW(g_hwndD310, 0x413, 0xe0f0f0, 0);
        SendMessageW(g_hwndD310, 0x439, 0, (LPARAM)&local_88[0]);
        SendMessageW(g_hwndD310, 0x421, (WPARAM)(uint64_t)param_6, param_3);

        local_b8.left = 0;
        local_b8.top = 0;
        local_b8.right = 0;
        local_b8.bottom = 0;
        if (!bVar1) {
            SendMessageW(g_hwndD310, 0x412, 0,
                         (LPARAM)(int64_t)(int)(param_5 << 0x10 | param_4 & 0xffff));
        }
        SetWindowPos(g_hwndD310, (HWND)0, 0, 0, 0, 0, 0x13);
        SetWindowPos(g_hwndD310, (HWND)0xffffffffffffffffULL, 0, 0, 0, 0, 0x13);
        SendMessageW(g_hwndD310, 0x411, 1, (LPARAM)&local_88[0]);
        KillTimer(param_1, 0xabb);
        if ((0 < (int)param_7) &&
            (UVar6 = SetTimer(param_1, 0xabb, param_7, (void *)PECMD_TimerMessage),
             (int)UVar6 != 0)) {
            sVar7 = 1;
            g_msgWndState = 1;
        }
        iVar2 = GetSystemMetrics(0x3d);
        iVar3 = GetSystemMetrics(0x3e);
        GetWindowRect(g_hwndD310, &local_b8);
        if ((param_8 & 0x10) == 0) {
            if (local_98 != 0) {
                param_4 = param_4 + (unsigned int)iVar2;
            }
        }
        else if ((param_8 & 0x20) == 0) {
            param_4 = param_4 + (unsigned int)(local_b8.left - local_b8.right);
        }
        else {
            param_4 = param_4 + (unsigned int)(local_b8.left - local_b8.right) + (unsigned int)iVar2;
        }
        if (local_9c == 0) {
            if (local_a8 != 0) {
                param_5 = param_5 + iVar3;
            }
        }
        else {
            iVar2 = local_b8.top - local_b8.bottom;
            if (local_a0 != 0) {
                iVar2 = iVar2 + iVar3;
            }
            param_5 = param_5 + iVar2;
        }
        if (bVar1) {
            SetWindowPos(g_hwndD310, (HWND)0, (int)param_4, param_5, 0, 0, 0x15);
        }
        else {
            SendMessageW(g_hwndD310, 0x412, 0,
                         (LPARAM)(int64_t)(int)(param_5 << 0x10 | param_4 & 0xffff));
        }
    }
    SetForegroundWindow(local_90);
    LeaveCriticalSection((LPCRITICAL_SECTION)&g_csInit);

    if ((local_a4 != 0) && (g_hwndD310 != (HWND)0)) {
        DVar4 = GetTickCount();
        while ((((sVar7 == (short)g_msgWndState) && ('\0' < g_flagA24F)) &&
                (0 < (int)param_7)) &&
               (DVar5 = GetTickCount(), DVar5 - DVar4 < param_7)) {
            FUN_1400195f0((uint64_t)(uintptr_t)g_Script, 1, 0, (uint64_t *)0);
        }
    }
    LeaveCriticalSection((LPCRITICAL_SECTION)&g_csE138);
}
