/*
 * partF.c — 还原 3 个窗口/消息框业务函数
 *
 *   PECMD_InitTimerObject @0x1400a9aa8  window/timer 对象初始化
 *   PECMD_CreateControlWindow @0x1400a9ce4  window 对象初始化
 *   PECMD_CreateMessageBox @0x1400b5aa4  创建消息框/窗口
 *
 * 说明: 虚拟调用 (**(code**)(*obj + off))(...) 等无法解析虚表处的调用
 * 保留为直接函数指针调用形式 (通过 *(uint64_t*)(*obj+off) 取槽)。
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "pecmd_defs.h"

/* ---- 本文件引用的全局/数据 ---- */
extern uint8_t PTR_FUN_140129040[];            /* 窗口对象虚表 */
extern uint8_t PTR_FUN_140129060[];            /* 窗口对象虚表 */
extern WCHAR g_szEmpty[];                /* .rdata 空串/虚表基址 */

/* ---- 本文件引用的辅助函数 (均为 extern, 不在此定义) ---- */
extern uint64_t *PECMD_ConstructControlObjectB(uint64_t *obj, uint32_t type, uint64_t data,
                               LPCWSTR text, uint64_t *src, LPCWSTR subText,
                               uint32_t p7, uint32_t p8, uint32_t p9, uint32_t p10);
extern void PECMD_ParseKeySizeIconSpec(WCHAR *param_1, int64_t *param_2, int *param_3,
                          int param_4, int64_t param_5);
extern uint64_t *FUN_1400f0648(uint64_t *obj, uint64_t *arg);   /* @0x1400f0648 */
extern uint64_t *PECMD_InitWindowObjectF(uint64_t *obj, uint64_t *arg);   /* @0x1400ecf18 */
extern void FUN_1400f072c(int64_t *param_1, LPCWSTR param_2, DWORD param_3,
                          int *param_4, HWND param_5, uint32_t param_6, DWORD param_7);
extern LRESULT PECMD_GetControlFont(int64_t param_1);       /* @0x1400e5890 取对象字体 */
extern void PECMD_SetVariableWithPrefix(int64_t *ctx, LPCWSTR key, LPCWSTR value); /* @0x14007d0ac */
extern void PECMD_SetCheckVariable(int64_t param_1, int param_2);   /* @0x14007df90 */
extern void PECMD_SetObjectEnable(int64_t param_1, int param_2);   /* @0x140053c5c */
extern void PECMD_SetObjectVisibleVar(int64_t a1, uint32_t a2);        /* @0x140053cec */
extern void PECMD_AllocWStringBuffer(WCHAR **ps, int64_t count);      /* @0x140063694 */
extern void FUN_14005b0b8(void *p);                        /* @0x14005b0b8 (缓冲区构造) */
extern uint64_t PECMD_TrayIconLoadThread(int64_t param_1);            /* @0x1400b568c 线程过程 */
extern void PECMD_VarSetUInt(void *script, uint64_t value, LPCWSTR key); /* @0x140066978 */
extern void FUN_14005b104(int64_t *ps);                    /* @0x14005b104 */
extern uint64_t *PECMD_CreateWindowObject(uint64_t *param_1, uint64_t param_2, uint32_t param_3);
extern int64_t *PECMD_AssignString(int64_t *param_1, LPCWSTR param_2); /* @0x14007034c */
extern uint64_t PECMD_GetWindowObjectRef(int64_t param_1);            /* @0x14005b77c */
extern void FUN_140053e78(void);                           /* @0x140053e78 PECMD_Empty */
extern void PECMD_PositionWindowDispatchCommand(int64_t *param_1, uint32_t param_2, uint64_t param_3,
                          uint32_t *param_4);             /* @0x14005e3ac */
extern uint32_t PECMD_ModalDialogPump(int64_t *param_1, uint64_t param_2); /* @0x1400e91f0 */
extern int64_t  PECMD_ModalMsgPumpEx(int64_t *param_1, uint64_t param_2); /* @0x1400e95f4 */
extern void *operator_new(size_t size);                   /* 全局 new 包装 */

/* ================================================================
 * @0x1400a9aa8  window/timer 对象初始化
 */
uint64_t *PECMD_InitTimerObject(uint64_t *param_1, uint64_t param_2, uint32_t param_3,
                        uint64_t *param_4, uint64_t *param_5, int param_6,
                        uint64_t param_7, uint32_t param_8)
{
    uint32_t uVar1;
    uint32_t uVar2;
    WCHAR local_48[32];

    PECMD_ConstructControlObjectB(param_1, 4, param_2, (LPCWSTR)(uintptr_t)*param_4, param_5,
                  (LPCWSTR)(uintptr_t)g_szEmpty, 0, 0, 0, 0);
    *(uint32_t *)(param_1 + 0xe) = param_3;
    *param_1 = (uint64_t)(uintptr_t)PTR_FUN_140129040;
    *(int *)((char *)param_1 + 0x74) = param_6;
    param_1[0xf] = param_7;
    uVar1 = 1;
    if ((param_6 < 1) || ((int)param_8 < 0)) {
        uVar1 = 0;
    }
    *(uint32_t *)(param_1 + 0x10) = uVar1;
    *(uint32_t *)((char *)param_1 + 0x84) = param_8;
    EnterCriticalSection(&g_csInit);
    uVar2 = 0xcfffffff;
    if (param_6 < 1) {
        uVar2 = 0x8fffffff;
    }
    *(uint32_t *)((char *)param_1 + 0x84) = (param_8 & uVar2);
    uVar2 = param_8 & uVar2 & 0xbfffffff;
    if (*(int *)(param_1 + 0x10) != 0) {
        SetTimer((HWND)param_1[0xf], (uintptr_t)*(int *)(param_1 + 0xe),
                 *(UINT *)((char *)param_1 + 0x74), (void *)0);
    }
    if (*(int *)((char *)param_1 + 0x84) < 0) {
        *(uint32_t *)((char *)param_1 + 0x84) = 0;
    }
    /* TODO(verify): 反编译仅剩格式串, 无格式化实参 */
    wsprintfW(local_48, WSTR("%ld"));
    if (*(const WCHAR *)(uintptr_t)param_1[2] != 0) {
        PECMD_SetVariableWithPrefix((int64_t *)(uintptr_t)param_1[10],
                      (LPCWSTR)(uintptr_t)param_1[2], local_48);
    }
    if ((((param_8 >> 0x1c) & 6U) != 0) && ((int)uVar2 > 0)) {
        for (; (int)uVar2 > 0; uVar2--) {
            SendMessageW((HWND)param_1[0xf], 0x113,
                         (WPARAM)*(int *)(param_1 + 0xe), 0);
        }
    }
    LeaveCriticalSection(&g_csInit);
    return param_1;
}

/* ================================================================
 * @0x1400a9ce4  window 对象初始化
 */
uint64_t *PECMD_CreateControlWindow(uint64_t *param_1, int64_t param_2, uint32_t param_3,
                        uint64_t *param_4, uint32_t param_5, uint32_t param_6,
                        uint32_t param_7, uint32_t param_8, uint64_t *param_9,
                        uint64_t *param_10, uint32_t param_11, uint32_t param_12,
                        int param_13, WCHAR *param_14)
{
    int64_t lVar1;
    uint32_t uVar2;
    int iVar3;
    uint64_t *puVar4;
    int64_t *plVar5;
    uint64_t *puVar6;
    LRESULT LVar7;
    uint32_t uVar8;
    uint32_t uVar9;
    uint8_t bVar10;
    bool bVar11;
    int local_48;
    uint32_t local_44;
    int local_40;
    uint64_t local_38;

    PECMD_ConstructControlObjectB(param_1, 6, param_2, (LPCWSTR)(uintptr_t)*param_4, param_10,
                  (LPCWSTR)(uintptr_t)*param_9, param_5, param_6, param_7, param_8);
    uVar2 = param_11;
    *(uint32_t *)(param_1 + 0xe) = param_11;
    *(uint32_t *)((char *)param_1 + 0x74) = param_12;
    *param_1 = (uint64_t)(uintptr_t)PTR_FUN_140129060;
    EnterCriticalSection(&g_csInit);
    lVar1 = *(int64_t *)(param_2 + 0x40);
    local_38 = param_1[8];
    plVar5 = (int64_t *)0x0;
    /* TODO(verify): 反编译有一个未使用的合成栈槽 iStack_30, 此处省略 (dead stack) */
    param_11 = (((param_11 >> 16) << 16) | (uint16_t)uVar2) & 0xffff8000;
    bVar11 = ((uint16_t)uVar2 & 0x7fff) == 1;
    uVar8 = (uint32_t)bVar11;
    uVar9 = 0x40000000;
    if ((uVar2 & 0x10) == 0) {
        uVar9 = 0x50000000;
    }
    param_12 = (uint32_t)bVar11;
    if (((uVar2 >> 0x1e) & 1) == 0) {
        if (((uVar2 >> 0x1d) & 1) != 0) {
            uVar9 = uVar9 | 0x200;
        }
    } else {
        uVar9 = uVar9 | 0x300;
    }
    if (param_13 != 0) {
        uVar9 = uVar9 | ((~(*(int *)(lVar1 + 0xd0) << 2)) & 0x10000U);
    }
    if (((uVar2 >> 0xf) & 1) != 0) {
        uVar9 = uVar9 | 0x8000000;
    }
    param_9 = (uint64_t *)0x0;
    bVar10 = 0;
    local_48 = 0;
    local_44 = 0;
    local_40 = 0;
    if ((param_14 == NULL) ||
        (PECMD_ParseKeySizeIconSpec(param_14, (int64_t *)&param_9, &local_48,
                       *(int *)(lVar1 + 0x17c), 0),
         iVar3 = local_40, puVar6 = param_9,
         (local_40 < 1) && (param_9 == NULL))) {
        puVar6 = (uint64_t *)operator_new(0xe8);
        if (puVar6 != NULL) {
            plVar5 = (int64_t *)FUN_1400f0648(puVar6, param_1 + 0xb);
        }
        param_1[7] = (uint64_t)plVar5;
        *(uint8_t *)((char *)plVar5 + 0xd8) = 4;
        uVar9 = uVar9 | 4;
        FUN_1400f072c(plVar5, (LPCWSTR)(uintptr_t)param_1[4], uVar9, (int *)&local_38,
                      *(HWND *)(lVar1 + 0x20), param_3, 0);
    } else {
        bVar10 = 0x80;
        puVar4 = (uint64_t *)operator_new(0x100);
        if (puVar4 != NULL) {
            plVar5 = (int64_t *)PECMD_InitWindowObjectF(puVar4, param_1 + 0xb);
        }
        param_1[7] = (uint64_t)plVar5;
        plVar5[0x1e] = (int64_t)puVar6;
        *(uint8_t *)((char *)plVar5 + 0xd8) = 9;
        local_40 = iVar3;
        if (local_40 < 1) {
            local_40 = 0x60;
        }
        *(int16_t *)((char *)plVar5 + 0xea) = (int16_t)local_40;
        *(uint16_t *)((char *)plVar5 + 0xfc) = (uint16_t)local_48;
        *(uint16_t *)((char *)plVar5 + 0xfe) = (uint16_t)local_44;
        FUN_1400f072c(plVar5, (LPCWSTR)(uintptr_t)param_1[4], uVar9 | 9, (int *)&local_38,
                      *(HWND *)(lVar1 + 0x20), param_3, 0);
        uVar9 = uVar9 | 0xb;
        SetWindowLongW((HWND)plVar5[4], -0x10, (LONG)uVar9);
        uVar8 = param_12;
    }
    plVar5 = (int64_t *)param_1[7];
    *(uint8_t *)((char *)plVar5 + 0x61) = (uint8_t)(bVar10 | 6);
    LVar7 = PECMD_GetControlFont(lVar1);
    /* 虚调用 FUN_ 0x108 (设置字体/资源) */
    ((void (*)(int64_t *, LRESULT, int))*(uint64_t *)(*plVar5 + 0x108))(plVar5, LVar7, 0);
    if (*(const WCHAR *)(uintptr_t)param_1[2] != 0) {
        PECMD_SetVariableWithPrefix((int64_t *)(uintptr_t)param_1[10],
                      (LPCWSTR)(uintptr_t)param_1[2], (LPCWSTR)(uintptr_t)param_1[4]);
    }
    /* 虚调用 FUN_ 0x118 */
    ((void (*)(int64_t *, int))*(uint64_t *)(*plVar5 + 0x118))(plVar5, (int)(int16_t)uVar8);
    PECMD_SetCheckVariable((int64_t)param_1, (int)(int16_t)uVar8);
    PECMD_SetObjectEnable((int64_t)param_1, (uint32_t)((int16_t)param_11 == 0));
    PECMD_SetObjectVisibleVar((int64_t)param_1, (uVar9 >> 0x1c) & 1);
    DragAcceptFiles((HWND)plVar5[4], 1);
    LeaveCriticalSection(&g_csInit);
    return param_1;
}

/* ================================================================
 * @0x1400b5aa4  创建消息框/窗口
 */
int64_t *PECMD_CreateMessageBox(int64_t *param_1, int64_t *param_2, LPCWSTR param_3,
                       LPCWSTR param_4, uint32_t param_5, uint32_t param_6,
                       DWORD param_7, uint64_t param_8, uint32_t *param_9,
                       uint32_t param_10, uint32_t param_11, uint64_t param_12,
                       HWND param_13, uint64_t *param_14)
{
    uint32_t *puVar1;
    uint64_t *lpParameter;
    DWORD DVar2;
    int iVar3;
    HWND pHVar4;
    HANDLE hThread;
    HANDLE hHandle;
    uint64_t *puVar5;
    int64_t *plVar6;
    HWND pHVar7;
    int64_t lVar8;
    uint32_t uVar9;
    int64_t *plVar10;
    uint8_t bVar11;
    uint64_t local_48;

    pHVar7 = param_13;
    puVar1 = param_9;
    if (param_14 == NULL) {
        local_48 = 0;
    } else {
        local_48 = *param_14;
    }
    bVar11 = (uint8_t)(param_7 & 0xf);
    uVar9 = param_7 & 0x1c0;
    pHVar4 = param_13;
    if (param_13 == 0) {
        if ((param_9[4] == 0) || (param_2 == NULL)) {
            pHVar4 = GetDesktopWindow();
        } else {
            pHVar4 = (HWND)param_2[4];
        }
        if (pHVar4 == 0) {
            puVar1[4] = 0;
        }
    }
    DVar2 = lstrlenW(param_4);
    if (0xb4 < (int)DVar2) {
        DVar2 = 0xb4;
    }
    param_7 = DVar2;
    if ((uint8_t)uVar9 == 0x80) {
        iVar3 = lstrlenW(param_3);
        hHandle = (HANDLE)0;
        param_7 = 0;
        PECMD_AllocWStringBuffer((WCHAR **)&param_14, (int64_t)iVar3 + 0x201);
        lpParameter = param_14;
        FUN_14005b0b8((void *)param_14);
        *(DWORD *)((char *)lpParameter + 0x2f4) = DVar2;
        *(uint32_t *)((char *)lpParameter + 0x31c) = param_5;
        lstrcpyW((LPWSTR)(lpParameter + 0x80), param_3);
        lpParameter[0x60] = (uint64_t)(lpParameter + 0x80);
        *(uint32_t *)(lpParameter + 0x2c) = param_6;
        lpParameter[0x62] = param_8;
        *(uint32_t *)((char *)lpParameter + 0x13c) = param_10;
        *(uint8_t *)(lpParameter + 0x79) = 0xf0;
        *(uint32_t *)(lpParameter + 0x28) = param_11;
        *(uint8_t *)(lpParameter + 0x61) = bVar11;
        lpParameter[0xd] = local_48;
        memcpy((uint8_t *)((char *)lpParameter + 0x164),
                      (const uint8_t *)param_4, DVar2 * 2);
        *(uint16_t *)((char *)lpParameter + (int64_t)(int)DVar2 * 2 + 0x164) = 0;
        lpParameter[0x7b] = param_12;
        lpParameter[0x7a] = (uint64_t)param_1;
        lpParameter[0x78] = (uint64_t)(lpParameter + 0x2c);
        hThread = CreateThread((LPSECURITY_ATTRIBUTES)0x0, 0x20000,
                               (void *)PECMD_TrayIconLoadThread, lpParameter, 0x10004, &param_7);
        g_lastThreadId = param_7;
        if (hThread != (HANDLE)0) {
            uVar9 = ~(uVar9 >> 8) & 1;
            if (uVar9 != 0) {
                hHandle = CreateEventW((LPSECURITY_ATTRIBUTES)0x0, 1, 0, (LPCWSTR)0x0);
            }
            lpParameter[0x7c] = (uint64_t)hHandle;
            param_14 = (uint64_t *)0x0;
            EnterCriticalSection(&g_csInit);
            g_taskCount = g_taskCount + 1;
            LeaveCriticalSection(&g_csInit);
            ResumeThread(hThread);
            CloseHandle(hThread);
            if ((uVar9 != 0) && (hHandle != (HANDLE)0)) {
                WaitForSingleObject(hHandle, 0xffffffff);
                CloseHandle(hHandle);
            }
        }
        PECMD_VarSetUInt(param_1, (uint64_t)param_7, WSTR("&&__LastTID"));
        FUN_14005b104((int64_t *)&param_14);
    } else {
        plVar6 = (int64_t *)0x0;
        if ((param_2 != NULL) && ((int)param_2[0x3a] != 0)) {
            param_2 = plVar6;
        }
        param_13 = pHVar4;
        if (pHVar4 == 0) {
            param_13 = GetDesktopWindow();
        }
        if (((uVar9 != 0x40) && (param_2 != NULL)) && (pHVar7 == 0)) {
            param_13 = (HWND)param_2[4];
        }
        pHVar7 = param_13;
        puVar5 = (uint64_t *)operator_new(0x3f0);
        if (puVar5 != NULL) {
            plVar6 = (int64_t *)PECMD_CreateWindowObject(puVar5, (uint64_t)pHVar7, 0x2720);
        }
        *(DWORD *)((char *)plVar6 + 0x2f4) = param_7;
        *(uint32_t *)((char *)plVar6 + 0x31c) = param_5;
        PECMD_AssignString(plVar6 + 0x5f, param_3);
        lVar8 = (int64_t)(int)param_7;
        plVar6[0x60] = plVar6[0x5f];
        plVar6[0x62] = param_8;
        *(uint32_t *)((char *)plVar6 + 0x13c) = param_10;
        *(uint32_t *)(plVar6 + 0x2c) = param_6;
        *(uint32_t *)(plVar6 + 0x28) = param_11;
        *(uint8_t *)(plVar6 + 0x61) = bVar11;
        plVar6[0xd] = local_48;
        memcpy((uint8_t *)((char *)plVar6 + 0x164),
                      (const uint8_t *)param_4, param_7 * 2);
        *(uint16_t *)((char *)plVar6 + lVar8 * 2 + 0x164) = 0;
        pHVar7 = (HWND)PECMD_GetWindowObjectRef((int64_t)param_2);
        FUN_140053e78();
        if (param_2 != NULL) {
            SetFocus((HWND)param_2[4]);
        }
        if (*(LPCWSTR *)(puVar1 + 6) != NULL) {
            PECMD_AssignString(plVar6 + 0x25, *(LPCWSTR *)(puVar1 + 6));
        }
        *(uint8_t *)((char *)plVar6 + 0x121) = 0;
        plVar6[0x2a] = (int64_t)pHVar4;
        plVar6[0x7b] = param_12;
        plVar6[0x7a] = (int64_t)param_1;
        plVar10 = (int64_t *)0x0;
        *(uint8_t *)(plVar6 + 0x24) = 0;
        if (uVar9 == 0xc0) {
            plVar6[0x1b] = (int64_t)param_2;
            *(uint8_t *)(plVar6 + 0x24) = 1;
            if (pHVar7 != 0) {
                EnterCriticalSection(&g_csInit);
                param_2[0x39] = param_2[0x39] + 1;
                *(uint8_t *)(plVar6 + 0x24) = 9;
                LeaveCriticalSection(&g_csInit);
            }
        } else {
            *(uint8_t *)(plVar6 + 0x24) = 0x10;
            plVar10 = plVar6;
        }
        if ((param_2 == NULL) || (puVar1[4] == 0)) {
            puVar1[4] = 0;
        } else {
            plVar6[0x2a] = param_2[4];
        }
        puVar1[5] = puVar1[5] & 0xfffffff3;
        puVar1[4] = puVar1[4] & 0xfffffff3;
        PECMD_PositionWindowDispatchCommand(plVar6, 0, (uint64_t)param_13, puVar1);
        if (uVar9 == 0x40) {
            PECMD_ModalDialogPump(plVar6, 0);
        } else if (uVar9 == 0xc0) {
            SendMessageW((HWND)plVar6[4], 0x110, 0, 0);
        } else {
            PECMD_ModalMsgPumpEx(plVar6, 0);
        }
        if (pHVar7 != 0) {
            SendMessageW(pHVar7, 0x451, 1, 0);
        }
        if (plVar10 != NULL) {
            /* 虚调用 FUN_ 0x18 */
            ((void (*)(int64_t *, int))*(uint64_t *)(*plVar10 + 0x18))(plVar10, 1);
        }
    }
    /* 反编译返回未初始化 unaff_RBX -> 返回 0 */
    return 0;
}
