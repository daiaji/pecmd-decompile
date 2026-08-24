/*
 * core_b3r_g2.c — 还原 3 个设备/网络业务函数
 *
 *   PECMD_FormatDeviceStatus @0x1400662a4  设备信息列表细节获取并格式化状态串
 *   PECMD_ParseNtpSetSystemTime @0x140066594  解析接收缓冲(网络字节序)并设置本地时间
 *   PECMD_NtpSyncLoop @0x140066674  NTP/网络时间同步主循环 (socket 组包)
 *
 * 说明:
 *   - 网络/WS2 槽 (g_psock...g_pinet_addr) 为延迟加载函数指针,
 *     按调用点展开为对应 socket API 的函数指针类型。
 *   - CONCAT42/CONCAT62 寄存器拼接仅用于构造未再使用的 local_246,
 *     已归一化为普通形式并标注 TODO(verify)。
 *   - PECMD_FormatDeviceStatus 中 wsprintfW 的变参在 Ghidra 伪码中被截断,
 *     前两组 (%s %s) 之后按格式串与语义补充 status/problem/param_5/尾串,
 *     以 TODO(verify) 标注。
 */

#include <stdint.h>
#include <stddef.h>
#include <string.h>

#include "win32_stub.h"
#include "pecmd_defs.h"

/* ---- 本文件引用的数据全局 ---- */
extern WCHAR g_szEmpty[];       /* .rdata 空串 (g_szEmpty) */
extern uint8_t DAT_1401210f8[]; /* .rdata 另一前缀串 */
extern uint8_t DAT_14013d5c0[]; /* 网络接收缓冲 (0x5dc 字节) */
extern char *g_timeServer;      /* 字符串/默认指针全局 */

/* ---- 本文件引用的 WS2 延迟加载函数指针槽 ---- */
extern uint32_t (*g_pntohl)(uint32_t); /* ntohl */
extern uint16_t (*g_phtons)(uint16_t); /* htons */
/* inet_addr 使用 pecmd_defs.h 声明 int (*g_pinet_addr)(char*) */
extern int (*g_pclosesocket)(uintptr_t);                                /* closesocket */
extern uintptr_t (*g_psock)(int, int, int);                             /* socket */
extern int (*g_pconnect)(uintptr_t, void *, int);                       /* connect */
extern int (*g_pselect)(int, void *, void *, void *, void *);           /* select */
extern int (*g_precvfrom)(uintptr_t, void *, int, int, void *, void *); /* recvfrom */

/* ---- 本文件引用的辅助函数 (extern, 不在此定义) ---- */
extern WCHAR *PECMD_AllocString(WCHAR **ps, int64_t count); /* @0x140063720 字符串扩容 */
extern void PECMD_SendPingPacket(int param_1);              /* @0x14005d30c 组发送包 */
extern void PECMD_SetSystemTimeFromUnix(int *param_1);      /* @0x14005d390 设置本地时间 */

/* ---- 本文件使用、桩中未声明的 Win32/SetupAPI ---- */
extern BOOL SetupDiGetDeviceInfoListDetailW(void *DeviceInfoSet, void *DeviceInfoSetDetailData);
extern uint32_t CM_Get_DevNode_Status_Ex(uint32_t *pulStatus, uint32_t *pulProblemNumber,
                                         uint32_t dnDevInst, uint32_t ulFlags);

/* 本文件局部: _SP_DEVINFO_LIST_DETAIL_DATA_W_pue 结构布局
 * 总大小 0x230: cbSize(4) + ClassGuid(16) + rest(540) */
typedef struct {
    DWORD cbSize;                       /* 0x00 */
    uint8_t ClassGuid[16];              /* 0x04 */
    uint8_t _rest[0x230 - 0x04 - 0x10]; /* 0x14 .. 0x230 */
} SP_DEVINFO_LIST_DETAIL_DATA_W_local;

/* ================================================================
 * @0x1400662a4  设备信息列表细节获取并格式化状态串
 */
uint64_t PECMD_FormatDeviceStatus(int64_t param_1, void *param_2, int64_t *param_3, LPCWSTR param_4,
                                  LPCWSTR param_5, int param_6)
{
    BOOL BVar1;
    DWORD DVar2;
    int iVar3;
    int iVar4;
    int iVar5;
    int iVar6;
    int iVar8;
    LPCWSTR puVar9;
    uint32_t uVar7;
    uint32_t local_268;
    uint32_t local_264[3];
    SP_DEVINFO_LIST_DETAIL_DATA_W_local local_258;

    local_258.cbSize = 0x230;
    memset(&local_258.ClassGuid, 0, 0x22c);
    iVar8 = 0;
    local_264[0] = 0;
    local_268 = 0;
    BVar1 = SetupDiGetDeviceInfoListDetailW(param_2, &local_258);
    if (BVar1 == 0) {
        DVar2 = GetLastError();
        uVar7 = (uint32_t)DVar2;
        if (DVar2 == 0) {
            uVar7 = 1;
        }
        return (uint64_t)uVar7;
    }

    iVar3 = (int)CM_Get_DevNode_Status_Ex(local_264, &local_268,
                                          *(uint32_t *)((char *)param_1 + 0x14), 0);
    if (iVar3 != 0) {
        return (uint64_t)(int64_t)iVar3;
    }

    puVar9 = (LPCWSTR)(uintptr_t)g_szEmpty;
    iVar3 = 0;
    local_268 = (uint32_t)(-(uint32_t)((local_264[0] & 0x400) != 0) & local_268);
    if (param_6 == 0) {
        if (*(const short *)(uintptr_t)*param_3 != 0) {
            iVar8 = 2;
            puVar9 = (LPCWSTR)(uintptr_t)DAT_1401210f8;
        }
    }
    else {
        iVar3 = 2;
    }

    iVar4 = lstrlenW((LPCWSTR)(uintptr_t)*param_3);
    iVar5 = lstrlenW(param_4);
    iVar6 = lstrlenW(param_5);
    PECMD_AllocString((WCHAR **)param_3, (int64_t)iVar8 + (int64_t)iVar3 + (int64_t)iVar6 +
                                             (int64_t)iVar5 + 0x3f + (int64_t)iVar4);
    /* TODO(verify): Ghidra 变参被截断, 仅保留前两组 (puVar9,param_4);
     * 后续按格式串补 status/problem(0x%X) 与 param_5/尾串。 */
    wsprintfW((LPWSTR)((uintptr_t)*param_3 + (int64_t)iVar4 * 2),
              WSTR("%s\"%s\" status: 0x%X problem: 0x%X \"%s\"%s"), puVar9, param_4,
              (unsigned int)local_268, (unsigned int)local_264[0], param_5, WSTR(""));
    return 0;
}

/* ================================================================
 * @0x140066594  解析接收缓冲(网络字节序)并设置本地时间
 */
void PECMD_ParseNtpSetSystemTime(uint64_t param_1, uint32_t *param_2)
{
    int iVar1;
    uint32_t uVar2;
    int local_res10;
    int local_res14;

    (void)param_1;

    (*g_pntohl)(*param_2);
    (*g_pntohl)(*param_2);
    (*g_pntohl)(*param_2);
    (*g_pntohl)(*param_2);
    (*g_pntohl)(*param_2);
    (*g_pntohl)(*param_2);
    (*g_pntohl)(param_2[1]);
    (*g_pntohl)(param_2[2]);
    (*g_pntohl)(param_2[3]);
    (*g_pntohl)(param_2[4]);
    (*g_pntohl)(param_2[5]);
    (*g_pntohl)(param_2[6]);
    (*g_pntohl)(param_2[7]);
    (*g_pntohl)(param_2[8]);
    (*g_pntohl)(param_2[9]);
    iVar1 = (int)(*g_pntohl)(param_2[10]);
    uVar2 = (*g_pntohl)(param_2[0xb]);
    local_res10 = iVar1 + 0x7c558180;
    local_res14 = (int)((uVar2 >> 0xc) + (((uVar2 >> 10) + 0x8000) >> 0x10) * -0x2f7);
    (void)local_res14;
    PECMD_SetSystemTimeFromUnix(&local_res10);
    return;
}

/* ================================================================
 * @0x140066674  NTP/网络时间同步主循环 (socket 组包)
 */
void PECMD_NtpSyncLoop(uint32_t *param_1)
{
    uint16_t uVar1;
    uint32_t uVar2;
    int iVar3;
    int iVar4;
    uint64_t uVar5;
    uint32_t local_res8[2];
    uint32_t local_res10;
    uint32_t local_res14;
    uint16_t local_248;
    uint32_t local_23e;
    uint16_t local_23a;
    uint16_t local_238;
    uint8_t local_236[8];
    uint32_t local_22e;
    uint16_t local_22a;
    uint32_t local_228[2];
    uint64_t local_220;
    uint16_t local_246; /* CONCAT42/62 拼接载体 (未用, TODO(verify)) */

    uVar5 = 0;
    local_246 = 0;
    local_23e = 0;
    local_23a = 0;
    (void)local_246;
    (void)local_23e;
    (void)local_23a;
    if (param_1 == (uint32_t *)0) {
        param_1 = (uint32_t *)g_timeServer;
    }
    local_248 = 2;
    if ((uintptr_t)param_1 == 1) {
        uVar2 = *(uint32_t *)g_timeServer;
    }
    else {
        uVar2 = (*g_pinet_addr)((char *)0); /* TODO(verify) inet_addr 实参来源 */
    }
    /* TODO(verify): 原 CONCAT42(uVar2,local_246) / CONCAT62(...,uVar1)
     * 仅用于构造未使用的 local_246, 归一化丢弃。 */
    (void)uVar2;
    uVar1 = (*g_phtons)(0x7b);
    (void)uVar1;

    do {
        if (0 < (int)uVar5) {
            (*g_pclosesocket)(uVar5);
        }
        uVar5 = (*g_psock)(2, 2, 0x11);
        iVar3 = (int)uVar5;
        if (-1 < iVar3) {
            iVar4 = (*g_pconnect)(uVar5, &local_248, 0x10);
            if (iVar4 != -1) {
                local_238 = 0;
                local_236[0] = local_236[1] = local_236[2] = local_236[3] = 0;
                local_236[4] = local_236[5] = local_236[6] = local_236[7] = 0;
                local_22e = 0;
                local_22a = 0;
                local_res8[0] = 0x10;
                PECMD_SendPingPacket(iVar3);
                local_res14 = 0;
                local_228[0] = 1;
                local_res10 = 1;
                local_220 = uVar5;
                (void)local_220;
                (void)local_22e;
                (void)local_22a;
                (void)local_res14;
                iVar4 = (*g_pselect)(iVar3 + 1, local_228, 0, 0, &local_res10);
                if (iVar4 == 1) {
                    SetLastError(0);
                    iVar4 = (*g_precvfrom)(uVar5, DAT_14013d5c0, 0x5dc, 0, &local_238, local_res8);
                    GetLastError();
                    if (0xb < iVar4) {
                        PECMD_ParseNtpSetSystemTime(uVar5 & 0xffffffff, (uint32_t *)DAT_14013d5c0);
                        if (0 < iVar3) {
                            (*g_pclosesocket)(uVar5);
                        }
                        return;
                    }
                }
            }
        }
        Sleep(100);
    } while (1);
}
