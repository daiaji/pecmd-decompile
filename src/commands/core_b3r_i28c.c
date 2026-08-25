/* Restored batch28 B3 business functions (part i28c)
 *
 * Contains:
 *   PECMD_PcipCommand @0x1400aee2c  PCIP 缓存/地址配置命令解析执行
 *   PECMD_CreateTimerObject @0x1400b02c4  对象构造派发 (被 PECMD_PcipDispatch 调用)
 *   PECMD_LaunchAsyncScriptThread @0x1400b0bf8  异步脚本/线程启动 (CreateThread 派发)
 *   PECMD_PcipDispatch @0x1400b4600  PCIP 命令分派 (SET/DHCP 等)
 */
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "win32_stub.h"
#include "pecmd_defs.h"

/* ---- Ghidra primitive aliases (project convention) ---- */
typedef unsigned int uint;
typedef unsigned short ushort;
typedef uint8_t byte;
typedef uint64_t undefined8;
typedef void undefined;
typedef int64_t longlong;
typedef uint64_t ulonglong;
typedef uint8_t *LPBYTE;

/* CONCAT44(hi,lo): (uint32)hi<<32 | (uint32)lo  (Ghidra register glue used for
 * pointer-identity casts; folded to plain values where used) */
#ifndef CONCAT44
#define CONCAT44(hi, lo) (((uint64_t)(uint32_t)(hi)) << 32 | (uint32_t)(lo))
#endif
/*
 * CONCAT71(hi7, lo8): (uint64)hi7<<8 | (uint8)lo  — used only to poke the
 * low byte of a pointer-shaped variable; normalized below.
 */
#ifndef CONCAT71
#define CONCAT71(hi7, lo8) (((uint64_t)(hi7) << 8) | (uint8_t)(lo8))
#endif

/* ---- Globals ---- */
extern WCHAR g_szEmpty[];     /* empty string (g_szEmpty) */
extern int64_t g_pCacheBlock; /* PCIP 缓存标记 */
extern int (*g_pDhcpNotifyConfigChange)(uint64_t, int64_t, int, uint32_t, int, uint32_t,
                                        uint8_t); /* dhcpcsvc 指针槽 */
extern int32_t g_i32D7EC;                         /* 活动任务计数 (条件) */
extern uint8_t g_foregroundFlag;                  /* 任务计数标志 */

/* ---- object/thread helpers ---- */
extern void *operator_new(size_t size); /* global new wrapper */
extern void *PECMD_SendCtrlMessage_0834(WPARAM wParam, uint64_t lParam);
extern uint64_t *PECMD_InitTimerObject(uint64_t *param_1, uint64_t param_2, uint32_t param_3,
                                       uint64_t *param_4, uint64_t *param_5, int param_6,
                                       uint64_t param_7, uint32_t param_8);
extern void PECMD_ScriptCopy(uint64_t *obj, uint64_t *p);
extern void PECMD_ScriptInit(uint64_t *obj, int64_t p);
extern uint32_t PECMD_ArgTokenize(int64_t *param_1, LPCWSTR param_2, int param_3);
extern void PECMD_ExpandScriptVariables(int64_t *param_1, int64_t *param_2, int64_t *param_3);
extern void PECMD_ImportEnvironment(int64_t *param_1, int64_t *param_2, int param_3);
extern uint64_t PECMD_GetWindowObjectRef(int64_t param_1);
extern void PECMD_VarSetUInt(void *script, uint64_t value, LPCWSTR key);
extern int64_t PECMD_WrapParamCall_02d8(int64_t *script, HANDLE hThread, LPCWSTR name);
extern void PECMD_DispatchSystemCommandLine(int64_t *param_1, WCHAR *buf);
extern void PECMD_ClearTaskTable(int64_t script, int mode);
extern void PECMD_RefCountRelease(WCHAR **ps);
extern void PECMD_ZeroLenBuf(void *p); /* 对象初始化 */
extern void PECMD_ParseSizeAndSkipWs(int64_t *param_1, uint64_t *param_2);
extern uint64_t PECMD_ScriptThreadProc(int64_t *param_1); /* 线程入口 (CreateThread 目标) */

/* ---- string / var helpers ---- */
/* S11: 本地声明与定义冲突已删除, 统一采用 xproto.h 原型 (原: extern void PECMD_AllocStrSlot(void *out); /* release slot * / extern WCHAR *PECMD_SkipLeadingControlChars(WCHA) */
/* S11: 本地声明与定义冲突已删除, 统一采用 xproto.h 原型 (原: extern int64_t *PECMD_SplitTokenAssignVar(WCHAR **out, WCHAR **pp, uint32_t sep, int flag);) */
/* S11: 本地声明与定义冲突已删除, 统一采用 xproto.h 原型 (原: extern void PECMD_ParseShortStore(void *pp, int *out, WCHAR sep);) */





extern char PECMD_MatchTokenAdvance(const char *tok, void *pp, int n);
extern WCHAR *PECMD_SkipWCharUntil(WCHAR **pp, uint16_t ch); /* delimiter scan */
/* S11: 本地声明与定义冲突已删除, 统一采用 xproto.h 原型 (原: /* S11: 本地声明与定义冲突, 已删除, 统一采用 xproto.h 原型 (原: extern int64_t PECMD_AsciiPrefixICmp(const char *a, const WCHAR *) */

extern WCHAR *PECMD_StrCopyW(WCHAR **ps, LPCWSTR src, int64_t len); /* 定长拷贝 */
/* S11: 本地声明与定义冲突已删除, 统一采用 xproto.h 原型 (原: /* S11: 本地声明与定义冲突, 已删除, 统一采用 xproto.h 原型 (原: extern void PECMD_WideToAnsiStr(int64_t *ps, LPCWSTR src, int64_t) */



extern int64_t *PECMD_StrBldCopyAnsi(int64_t *out, const char *src, uint64_t len); /* 取串槽 */
extern WCHAR *PECMD_StrDupA(WCHAR **ps, LPCSTR src, ulonglong a, ulonglong b);
extern uint64_t PECMD_GetPackedSystemVersion(void); /* 版本号 */
extern void PECMD_LoadSetupApiFunctions(void);      /* 初始化 */
extern void PECMD_InitWinsock(void *p);             /* 懒加载 WS2_32 */
extern int32_t PECMD_QueryState_cfc0(void);
extern void PECMD_FreeCacheBlock(void); /* 网络初始化失败 */
extern void PECMD_GetApiProcCached(const char *func, const char *dll, void **out, uintptr_t *hmod);
extern DWORD PECMD_WaitHandlesOrMessages(uint64_t param_1, int64_t param_2, int param_3,
                                         undefined8 *param_4);
extern void PECMD_ExecSpecialCommand(void);
extern uint64_t PECMD_SetIpConfig(LPCSTR param_1, LPCSTR param_2, LPBYTE param_3, LPBYTE param_4,
                                  LPBYTE param_5, LPBYTE param_6, short *param_7);
extern uint64_t PECMD_ProcessCommandLine(LPCSTR a, int b, LPCSTR c, char *d, LPBYTE e, LPBYTE f,
                                         int g);
extern int64_t PECMD_EnumNetworkDevices(LPCSTR s, int64_t a, uint64_t b);
/* S11: 本地声明与定义冲突已删除, 统一采用 xproto.h 原型 (原: /* S11: 本地声明与定义冲突, 已删除, 统一采用 xproto.h 原型 (原: extern void PECMD_NotifyMainWindowRefresh(int64_t *param_1, int f) */

extern uint32_t *PECMD_ResolveMacAddress(int param_1, uint64_t *param_2, uint64_t *param_3,
                                         int64_t *param_4, uint64_t param_5, LPWSTR param_6,
                                         LPWSTR param_7, int64_t *param_8, LPWSTR param_9);
extern uint8_t *PECMD_VarLookup(void *script, LPCWSTR name, void *scope, int64_t len, void **out);
extern void PECMD_VarWriteLine(int64_t *script, LPCWSTR name, LPCWSTR v, int64_t len, int64_t *out,
                               char flag);
extern void PECMD_AllocSmallObject(void *arr); /* 小对象分配 */
extern void *PECMD_SplitNextToken(void *ctx, int64_t *pp, int64_t *out, int sep, int flag);
extern int64_t PECMD_ExpandCommandLine(void *ctx, WCHAR *src, WCHAR **out, int mode, uint8_t flag);
extern char PECMD_MatchAndPad(const void *key, WCHAR **pp, int len);

/* ================================================================
 * @0x1400aee2c  PCIP 缓存/地址配置命令解析执行
 * signature: longlong __fastcall PECMD_PcipCommand(longlong * param_1, LPCWSTR param_2)
 * 解析 "PCIP" 命令 (fresh/nfresh/many/safe/qkos/qk/sfsw/all/freecah/cah: 前缀,
 * 逗号分隔字段), 并执行注册表/DHCP/缓存读写。
 * 注: 原始寄存器拼接 (CONCAT71/CONCAT44) 已按项目规范归一化为普通整数/指针;
 * 命令分发/网络配置主体保留, 深层驱动字节布局部分用 TODO(verify) 标注。
 */
int64_t PECMD_PcipCommand(int64_t *param_1, LPCWSTR param_2)
{
    BYTE *pBVar1;
    BYTE BVar2;
    LPBYTE pBVar3;
    DWORD DVar4;
    char cVar5;
    int bVar6;
    int iVar7;
    int iVar8;
    uint uVar9;
    DWORD DVar10;
    int64_t *plVar11;
    ulonglong uVar12;
    int64_t *puVar13;
    int64_t lVar14;
    BYTE *pBVar15;
    WCHAR WVar17;
    LPWSTR pWVar18;
    uint uVar19;
    byte bVar20;
    LPWSTR pWVar21;
    int64_t lVar22;
    uint uVar23;
    LPWSTR pWVar24;
    byte bVar25;
    DWORD DVar26;
    LPCWSTR local_res10;
    /* local_res20: 按 Ghidra 是 LPBYTE 型, 低字节保存 "*fresh" 标志; 复用为
     * 字节缓冲区指针。归一化: 用 fresh 保存低字节标志, local_res20 作指针。 */
    LPBYTE local_res20;
    int fresh;
    char local_188;
    char local_187;
    LPWSTR local_180;
    char local_178;
    byte local_177;
    LPCWSTR local_170;
    char local_168;
    byte local_167;
    uint local_164;
    LPWSTR local_160;
    uint64_t local_158;
    uint local_150;
    uint64_t local_148;
    int local_140[2];
    LPCWSTR local_138;
    LPWSTR local_130;
    LPWSTR local_128;
    WCHAR *local_120;
    int64_t *local_118;
    uint64_t local_110;
    LPWSTR local_108;
    uint local_100;
    LPCWSTR local_f8;
    LPCWSTR local_f0;
    LPCWSTR local_e8;
    LPCSTR local_e0;
    LPCWSTR local_d8;
    LPCWSTR local_d0;
    LPCWSTR local_c8;
    LPCWSTR local_c0;
    LPCSTR local_b8;
    LPCWSTR local_b0;
    LPCWSTR local_a8;
    LPCWSTR local_a0;
    LPCWSTR local_98;
    LPCWSTR local_90;
    LPCWSTR local_88;
    LPCWSTR local_80;
    uint64_t *local_78;
    LPWSTR local_70;
    int64_t local_68;
    LPWSTR local_60;
    LPWSTR local_58;
    LPWSTR local_50;
    uint64_t *local_48;
    uint64_t in_stack_resid;

    local_res10 = param_2;
    PECMD_AllocStrSlot(&local_120);
    DVar10 = 0;
    DVar26 = 0;
    local_110 = 0;
    (void)local_110;
    local_118 = param_1;
    (void)local_118;
    PECMD_SkipLeadingControlChars((WCHAR **)&local_res10);
    PECMD_AllocStrSlot(&local_e8);
    PECMD_AllocStrSlot(&local_130);
    PECMD_AllocStrSlot(&local_f0);
    PECMD_AllocStrSlot(&local_d8);
    PECMD_AllocStrSlot(&local_c0);
    PECMD_AllocStrSlot(&local_f8);
    PECMD_AllocStrSlot(&local_c8);
    PECMD_AllocStrSlot(&local_b0);
    PECMD_AllocStrSlot(&local_d0);
    PECMD_AllocStrSlot(&local_88);
    PECMD_AllocStrSlot(&local_80);
    PECMD_AllocStrSlot(&local_90);
    local_140[0] = 0;
    local_150 = 0xffff0000;
    local_158 = 0;
    PECMD_AllocStrSlot(&local_170);
    PECMD_AllocSmallObject(&local_b8);
    lVar14 = 1;
    plVar11 = PECMD_SplitTokenAssignVar(&local_120, (WCHAR **)&local_res10, 0x2c, 1);
    PECMD_SplitTokenTrimWs(plVar11, &local_130, 0);
    pWVar24 = local_130;
    uVar23 = 0;
    local_148 = 0;
    local_164 = 0;
    local_res20 = (LPBYTE)1; /* low byte = fresh flag = 1 */
    fresh = 1;
    local_180 = local_130;
    bVar25 = 0;
    local_187 = '\0';
    local_178 = '\0';
    local_177 = 0;
    PECMD_StrDupAssign(&local_a0, WSTR("&&__PECMD.PCIP.DEAULT_CACHE"));
    WVar17 = *pWVar24;
    bVar20 = bVar25;
    pWVar18 = pWVar24;
    while (WVar17 == L'*') {
        cVar5 = PECMD_MatchTokenAdvance("*fresh", &local_180, 6);
        if (cVar5 == '\0') {
            cVar5 = PECMD_MatchTokenAdvance("*nfresh", &local_180, 7);
            if (cVar5 == '\0') {
                cVar5 = PECMD_MatchTokenAdvance("*many", &local_180, 5);
                if (cVar5 == '\0') {
                    cVar5 = PECMD_MatchTokenAdvance("*safe", &local_180, 5);
                    if (cVar5 == '\0') {
                        cVar5 = PECMD_MatchTokenAdvance("*qkos", &local_180, 5);
                        if (cVar5 == '\0') {
                            cVar5 = PECMD_MatchTokenAdvance("*qk", &local_180, 3);
                            if (cVar5 == '\0') {
                                cVar5 = PECMD_MatchTokenAdvance("*sfsw", &local_180, 5);
                                if (cVar5 == '\0') {
                                    cVar5 = PECMD_MatchTokenAdvance("*all", &local_180, 4);
                                    if (cVar5 == '\0') {
                                        cVar5 = PECMD_MatchTokenAdvance("*freecah", &local_180, 8);
                                        if (cVar5 != '\0') {
                                            PECMD_FreeCacheBlock();
                                            lVar14 = 0;
                                            goto LAB_1400b01d8;
                                        }
                                        cVar5 = PECMD_MatchTokenAdvance("*cah:", &local_180, 5);
                                        pWVar18 = local_180;
                                        if (cVar5 == '\0') {
                                            WVar17 = *local_180;
                                            while ((WVar17 != L'\0' && ((((ushort)WVar17 < 9 ||
                                                                          (0xd < (ushort)WVar17)) &&
                                                                         (WVar17 != L' '))))) {
                                                local_180 = local_180 + 1;
                                                WVar17 = *local_180;
                                            }
                                        }
                                        else {
                                            WVar17 = *local_180;
                                            pWVar21 = local_180;
                                            while (
                                                ((WVar17 != L'\0' && (((ushort)WVar17 < 9 ||
                                                                       (0xd < (ushort)WVar17)))) &&
                                                 (WVar17 != L' '))) {
                                                pWVar21 = pWVar21 + 1;
                                                WVar17 = *pWVar21;
                                            }
                                            lVar22 = (int64_t)pWVar21 - (int64_t)local_180;
                                            local_180 = pWVar21;
                                            PECMD_StrCopyW((WCHAR **)&local_a0, pWVar18,
                                                           lVar22 >> 1);
                                        }
                                        PECMD_SkipLeadingControlChars((WCHAR **)&local_180);
                                    }
                                    else {
                                        local_177 = 0x40;
                                    }
                                }
                                else {
                                    bVar20 = 0x10;
                                }
                            }
                            else {
                                local_178 = '\x02';
                            }
                        }
                        else {
                            local_164 = local_164 & 0xffff0000;
                        }
                    }
                    else {
                        local_164 = 2;
                    }
                }
                else {
                    local_187 = '\x01';
                }
            }
            else {
                local_res20 = (LPBYTE)((intptr_t)local_res20 & 0xffffffffffffff00ULL);
                fresh = 0;
            }
        }
        else {
            local_res20 = (LPBYTE)1;
            fresh = 1;
        }
        pWVar18 = local_180;
        WVar17 = *local_180;
    }
    PECMD_LoadSetupApiFunctions();
    PECMD_InitWinsock(&local_188);
    bVar6 = PECMD_QueryState_cfc0();
    if (bVar6 == 0)
        goto LAB_1400b01d8;
    uVar19 = uVar23;
    if (*pWVar18 == L'?') {
        uVar19 = 1;
        if (pWVar18[1] == L'*') {
            uVar19 = 2;
        }
        local_148 = (uint64_t)(uint32_t)uVar19;
    }
    for (pWVar18 = pWVar18 + (int)uVar19; (((*pWVar18 == L'?' || (*pWVar18 == L'*')) ||
                                            ((8 < (ushort)*pWVar18 && ((ushort)*pWVar18 < 0xe)))) ||
                                           (*pWVar18 == L' '));
         pWVar18 = pWVar18 + 1) {
    }
    PECMD_AssignString((WCHAR **)&local_e8, pWVar18);
    *pWVar24 = L'\0';
    uVar12 = PECMD_GetPackedSystemVersion();
    iVar8 = (int)(uint32_t)local_148;
    local_100 = 0;
    local_168 = (uint)(uVar12 >> 0x10) < 0x60000;
    local_167 = (byte)(-local_168 & (uint)bVar20);
    if (*local_res10 == L',') {
        local_res10 = local_res10 + 1;
        plVar11 = PECMD_SplitTokenAssignVar(&local_120, (WCHAR **)&local_res10, 0x2c, 1);
        PECMD_SplitTokenTrimWs(plVar11, &local_130, 0);
        pWVar24 = local_130;
        if (*local_res10 == L',') {
            local_res10 = local_res10 + 1;
            plVar11 = PECMD_SplitTokenAssignVar(&local_120, (WCHAR **)&local_res10, 0x2c, 1);
            PECMD_SplitTokenTrimWs(plVar11, &local_f0, 0);
            if (*local_res10 == L',') {
                local_res10 = local_res10 + 1;
                plVar11 = PECMD_SplitTokenAssignVar(&local_120, (WCHAR **)&local_res10, 0x2c, 1);
                PECMD_SplitTokenTrimWs(plVar11, &local_d8, 0);
            }
        }
    }
    PECMD_AllocStrSlot(&local_138);
    if (*local_res10 == L',') {
        PECMD_AllocStrSlot(&local_160);
        local_res10 = local_res10 + 1;
        plVar11 = PECMD_SplitTokenAssignVar(&local_120, (WCHAR **)&local_res10, 0x2c, 1);
        PECMD_SplitTokenTrimWs(plVar11, &local_160, 0);
        WVar17 = L'?';
        local_108 = local_160;
        local_128 = local_160;
        pWVar18 = local_160;
        PECMD_SkipWCharUntil((WCHAR **)&local_128, 0x3f);
        if (WVar17 == *local_128) {
            *local_128 = L'\0';
            local_128 = local_128 + 1;
            pWVar18 = local_108;
        }
        bVar6 = 0;
        iVar7 = 0;
        if ((9 < (ushort)(*pWVar18 + 0xff10)) ||
            (iVar7 = lstrlenW(pWVar18), pWVar18 = local_108, 0x10 < iVar7)) {
            while (*pWVar18 == L'*') {
                *pWVar18 = L'\0';
                local_108 = local_108 + 1;
                pWVar18 = local_108;
            }
            if (!bVar6) {
                PECMD_StrBldCopyWide((int64_t *)&local_170, pWVar18);
            }
            if (iVar8 != 0) {
                PECMD_AssignString((WCHAR **)&local_138, local_128);
            }
            PECMD_FreeStrBuf(&local_160);
            goto LAB_1400af515;
        }
        PECMD_ParseHexOrDecBool(&local_108, local_140);
        bVar6 = 1;
        pWVar18 = local_108;
        if (-1 < local_140[0])
            goto LAB_1400af4cc;
        PECMD_FreeStrBuf(&local_160);
    }
    else {
        /* "query" 前缀: 解析逗号字段进入查询分支 */
    LAB_1400af4cc:
        while (*pWVar18 == L'*') {
            *pWVar18 = L'\0';
            local_108 = local_108 + 1;
            pWVar18 = local_108;
        }
        if (!bVar6) {
            PECMD_StrBldCopyWide((int64_t *)&local_170, pWVar18);
        }
        if (iVar8 != 0) {
            PECMD_AssignString((WCHAR **)&local_138, local_128);
        }
        PECMD_FreeStrBuf(&local_160);
        goto LAB_1400af515;
    }
LAB_1400af515:
    if (iVar8 == 0) {
        if (*local_res10 == L',') {
            local_res10 = local_res10 + 1;
            iVar8 = StrCmpNIW(local_res10, WSTR("fresh"), 5);
            if (iVar8 == 0) {
                local_150 = 8;
                PECMD_SkipWCharUntil((WCHAR **)&local_res10, 0x2c);
            }
            else {
                puVar13 = PECMD_SplitTokenAssignVar(&local_120, (WCHAR **)&local_res10, 0x2c, 1);
                PECMD_ParseShortStore(puVar13, (int *)&local_158, 0x2c);
                iVar8 = lstrlenW(local_res10); /* 近似: 原函数返回计数值, 此处归一时用非零串长 */
                if (0 < iVar8) {
                    local_150 = (uint)((short)local_158 != 0);
                }
            }
        }
    }
    else if (*local_res10 == L',') {
        local_res10 = local_res10 + 1;
        plVar11 = PECMD_SplitTokenAssignVar(&local_120, (WCHAR **)&local_res10, 0x2c, 1);
        PECMD_SplitTokenTrimWs(plVar11, &local_c0, 0);
        if (*local_res10 == L',') {
            local_res10 = local_res10 + 1;
            plVar11 = PECMD_SplitTokenAssignVar(&local_120, (WCHAR **)&local_res10, 0x2c, 1);
            PECMD_SplitTokenTrimWs(plVar11, &local_f8, 0);
            if (*local_res10 == L',') {
                local_res10 = local_res10 + 1;
                plVar11 = PECMD_SplitTokenAssignVar(&local_120, (WCHAR **)&local_res10, 0x2c, 1);
                PECMD_SplitTokenTrimWs(plVar11, &local_c8, 0);
                if (*local_res10 == L',') {
                    local_res10 = local_res10 + 1;
                    plVar11 =
                        PECMD_SplitTokenAssignVar(&local_120, (WCHAR **)&local_res10, 0x2c, 1);
                    PECMD_SplitTokenTrimWs(plVar11, &local_b0, 0);
                    if (*local_res10 == L',') {
                        local_res10 = local_res10 + 1;
                        plVar11 =
                            PECMD_SplitTokenAssignVar(&local_120, (WCHAR **)&local_res10, 0x2c, 1);
                        PECMD_SplitTokenTrimWs(plVar11, &local_d0, 0);
                        if (*local_res10 == L',') {
                            local_res10 = local_res10 + 1;
                            plVar11 = PECMD_SplitTokenAssignVar(&local_120, (WCHAR **)&local_res10,
                                                                0x2c, 1);
                            PECMD_SplitTokenTrimWs(plVar11, &local_88, 0);
                            if (*local_res10 == L',') {
                                local_res10 = local_res10 + 1;
                                plVar11 = PECMD_SplitTokenAssignVar(
                                    &local_120, (WCHAR **)&local_res10, 0x2c, 1);
                                PECMD_SplitTokenTrimWs(plVar11, &local_80, 0);
                                if (*local_res10 == L',') {
                                    local_res10 = local_res10 + 1;
                                    plVar11 = PECMD_SplitTokenAssignVar(
                                        &local_120, (WCHAR **)&local_res10, 0x2c, 1);
                                    PECMD_SplitTokenTrimWs(plVar11, &local_90, 0);
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    if ((((-1 < local_140[0]) || (*local_170 != L'\0')) || (*local_c0 != L'\0')) ||
        (*local_f8 != L'\0')) {
        EnterCriticalSection((LPCRITICAL_SECTION)&g_csInit);
        local_188 = '\0';
        if (local_a0 == (LPCWSTR)0x0) {
            PECMD_FreeCacheBlock();
        }
        else {
            in_stack_resid = 0;
            (void)in_stack_resid;
            lVar14 = (int64_t)PECMD_VarLookup(param_1, local_a0, (void *)0, -1, (void **)0);
            if ((lVar14 == 0) || ((*(ulonglong *)(lVar14 + 0x18) & 0x3fffffffffffffffULL) < 0x10)) {
                PECMD_FreeCacheBlock();
            }
            else if ((**(longlong **)(lVar14 + 8) != 0xa55aaa55) ||
                     ((lVar14 = (*(longlong **)(lVar14 + 8))[1],
                       lVar14 == 0 || (lVar14 != g_pCacheBlock)))) {
                local_188 = '\0';
                PECMD_FreeCacheBlock();
            }
            else {
                local_188 = '\x01';
            }
        }
        iVar8 = lstrlenW(local_170);
        bVar20 = bVar25;
        /* 简化: 拆出 hex/dec 字符区间判断, 避免深层括号拼写错误 (语义同原) */
        {
            ushort c1 = (ushort)local_170[1];
            ushort c2 = (ushort)local_170[2];
            int isDigit1 = (0x2f < c1 && c1 < 0x3a);
            int isHex1 = (0x60 < (ushort)(c1 | 0x20U) && (ushort)(c1 | 0x20U) < 0x67);
            int isDigit2 = (0x2f < c2 && c2 < 0x3a);
            int isHex2 = (0x60 < (ushort)(c2 | 0x20U) && (ushort)(c2 | 0x20U) < 0x67);
            if ((*local_170 == L'{') && (local_170[iVar8 + -1] == L'}') && (isDigit1 || isHex1) &&
                (isDigit2 || isHex2)) {
                bVar20 = 1;
            }
        }
        local_e0 = (LPCSTR)0x0;
        PECMD_WideToAnsiStr((int64_t *)&local_e0, local_c0, -1, 0xffffffffffffffffULL);
        local_a8 = (LPCWSTR)PECMD_CopyStrToSlot((WCHAR **)&local_a8, (WCHAR **)&local_f8);
        PECMD_AllocStrSlot(&local_98);
        local_160 = (LPWSTR)0;
        PECMD_AllocStringSlot2((void **)&local_78, 0x10d2);
        memset(local_78, 0, 0x10d2);
        *(uint8_t *)((intptr_t)local_78 + 2) = 0;
        local_158 = (uint64_t)((intptr_t)local_78 + 1);
        *(char *)local_158 = '\0';
        local_48 = local_78 + 0x195;
        local_58 = (LPWSTR)((intptr_t)local_78 + 0x1012);
        local_60 = (LPWSTR)((intptr_t)local_78 + 0x1052);
        local_70 = (LPWSTR)((intptr_t)local_78 + 0x1092);
        local_50 = (LPWSTR)((intptr_t)local_78 + 0xfd2);
        *(uint16_t *)((intptr_t)local_78 + 0xfd4) = 0;
        *local_50 = L'\0';
        *local_58 = L'\0';
        *local_60 = L'\0';
        *local_70 = L'\0';
        /* TODO(verify): PECMD_ResolveMacAddress 大 MAC/格式化函数的第5参为寄存器拼接标志,
         * 归一化为 (local_164 & 0xff) | local_177 */
        if (((bVar20 == 0) && ((-1 < local_140[0] || (*local_170 != L'\0')))) ||
            (((int)(uint32_t)local_148 != 0 && ((*local_c0 != L'\0' || (*local_f8 != L'\0')))))) {
            local_160 = (LPWSTR)(uintptr_t)PECMD_ResolveMacAddress(
                local_140[0], (uint64_t *)&local_170, (uint64_t *)&local_e0, (int64_t *)&local_a8,
                (uint64_t)((local_164 & 0xff) | local_177), local_58, local_60,
                (int64_t *)&local_98, local_70);
        }
        PECMD_WideToAnsiStr((int64_t *)&local_b8, local_170, -1, 0xffffffffffffffffULL);
        lstrlenA(local_b8);
        if ((int)(uint32_t)local_148 == 0) {
            PECMD_GetApiProcCached("DhcpNotifyConfigChange", "dhcpcsvc.DLL",
                                   (void **)&g_pDhcpNotifyConfigChange, (uintptr_t *)0);
            if (g_pDhcpNotifyConfigChange == 0) {
                DVar26 = 1;
            }
            else {
                if (((*local_e8 != L'\0') || (*local_f0 != L'\0')) || (*local_d8 != L'\0')) {
                    bVar25 = 1;
                }
                if (((int32_t)local_150 < 0) || (bVar25 != 0)) {
                    if ((*local_e8 != L'-') || (uVar19 = 1, local_178 != '\0')) {
                        uVar19 = uVar23;
                    }
                    if (*local_b8 == '\0') {
                        DVar26 = 0;
                        if (fresh != 0)
                            goto LAB_1400b00d9;
                        DVar26 = 1;
                    }
                    else {
                        PECMD_SkipWCharUntil((WCHAR **)&local_d8, ',');
                        cVar5 = ';';
                        PECMD_SkipWCharUntil((WCHAR **)&local_e8, ';');
                        PECMD_SkipWCharUntil((WCHAR **)&pWVar24, cVar5);
                        DVar4 = 0;
                        if (bVar25 != 0) {
                            local_158 = 0;
                            PECMD_WideToAnsiStr((int64_t *)&local_158, local_e8, -1,
                                                0xffffffffffffffffULL);
                            local_180 = (LPWSTR)0;
                            PECMD_WideToAnsiStr((int64_t *)&local_180, pWVar24, -1,
                                                0xffffffffffffffffULL);
                            local_148 = 0;
                            PECMD_WideToAnsiStr((int64_t *)&local_148, local_f0, -1,
                                                0xffffffffffffffffULL);
                            local_160 = (LPWSTR)0;
                            PECMD_WideToAnsiStr((int64_t *)&local_160, local_d8, -1,
                                                0xffffffffffffffffULL);
                            iVar8 = lstrlenA((LPCSTR)(intptr_t)local_180);
                            PECMD_AppendWideStr((WCHAR **)&local_180,
                                                (LPCWSTR)(intptr_t)(iVar8 + 0xdc));
                            iVar8 = lstrlenA((LPCSTR)(intptr_t)local_158);
                            PECMD_AppendWideStr((WCHAR **)&local_158,
                                                (LPCWSTR)(intptr_t)(iVar8 + 0xdc));
                            pBVar3 = (LPBYTE)(intptr_t)local_148;
                            pWVar18 = (LPWSTR)(intptr_t)local_160;
                            uVar12 = PECMD_SetIpConfig(
                                local_b8, (LPCSTR)(intptr_t)local_158, (LPBYTE)(intptr_t)local_180,
                                (LPBYTE)(intptr_t)local_148, (BYTE *)(intptr_t)local_160, (LPBYTE)0,
                                (short *)0);
                            local_100 = (uint)uVar12;
                            uVar23 = local_100 & 0xffff;
                            uVar9 = (uint)((uVar12 & 0xffff) != 0);
                            if (((byte)local_164 != '\0') ||
                                (((fresh == 0 && (uVar23 == 1)) && (uVar19 == 0)))) {
                                uVar12 = PECMD_ProcessCommandLine(
                                    local_b8, 0, (LPCSTR)(intptr_t)local_158,
                                    (char *)(intptr_t)local_180, pBVar3,
                                    (BYTE *)(intptr_t)local_160, (int)(char)local_167);
                                uVar9 = (uint)uVar12;
                            }
                            if (-1 < (int32_t)local_150) {
                                PECMD_FreeStrBuf(&local_160);
                                PECMD_FreeStrBuf(&local_148);
                                PECMD_FreeStrBuf(&local_180);
                                PECMD_FreeStrBuf(&local_158);
                                goto LAB_1400afff3;
                            }
                            if (uVar9 == 0) {
                                DVar10 = GetLastError();
                            }
                            PECMD_FreeStrBuf(&local_160);
                            PECMD_FreeStrBuf(&local_148);
                            PECMD_FreeStrBuf(&local_180);
                            PECMD_FreeStrBuf(&local_158);
                            DVar4 = DVar10;
                        }
                        DVar26 = DVar4;
                        if ((((fresh == 0) && (g_pDhcpNotifyConfigChange != 0)) &&
                             (local_168 != '\0')) &&
                            ((uVar19 == 0 && (uVar23 < 2)))) {
                            if (0xffff < (int)local_100) {
                                PECMD_WaitHandlesOrMessages((uint64_t)param_1, 500, 0,
                                                            (undefined8 *)0);
                                PECMD_ExecSpecialCommand();
                            }
                        }
                        else {
                        LAB_1400b00d9:
                            PECMD_EnumNetworkDevices(
                                local_b8, -1, (local_164 & 0xff) | (uint64_t)(uint8_t)local_187);
                        }
                    }
                }
                else {
                LAB_1400afff3:
                    uVar23 = local_164;
                    if (0xff < (ushort)local_164) {
                        uVar23 = 0;
                    }
                    uVar19 = local_150;
                    if (local_150 == 8) {
                        uVar19 = 0xffffffff;
                    }
                    iVar8 = (int)PECMD_EnumNetworkDevices(
                        local_b8, (int64_t)uVar19, (uVar23 & 0xff) | (uint64_t)(uint8_t)local_187);
                    if (iVar8 == 0) {
                        DVar26 = GetLastError();
                    }
                }
                PECMD_NotifyMainWindowRefresh(param_1, 0);
            }
        }
        else {
            if (*local_138 != L'\0') {
                PECMD_SetVariable(param_1, local_138, local_170);
            }
            if (*local_c0 != L'\0') {
                plVar11 =
                    PECMD_StrBldCopyAnsi((int64_t *)&local_res20, local_e0, 0xffffffffffffffffULL);
                PECMD_SetVariable(param_1, local_c0, (LPCWSTR)*plVar11);
                PECMD_FreeStrBuf(&local_res20);
            }
            if (*local_f8 != L'\0') {
                PECMD_SetVariable(param_1, local_f8, local_a8);
            }
            local_res20 = (LPBYTE)((intptr_t)local_78 + 0x654);
            pBVar1 = (BYTE *)((intptr_t)local_78 + 0x97e);
            WVar17 = L'*';
            if (1 < (int)(uint32_t)local_148) {
                WVar17 = L'?';
            }
            *(uint8_t *)local_78 = (uint8_t)WVar17;
            if (*local_b8 != '\0') {
                PECMD_SetIpConfig(local_b8, (LPCSTR)local_78, (LPBYTE)((intptr_t)local_78 + 0x32a),
                                  local_res20, pBVar1, (LPBYTE)((intptr_t)local_78 + 0x195),
                                  (short *)((intptr_t)local_78 + 0xfd2));
            }
            if (*local_e8 != L'\0') {
                PECMD_StrDupA((WCHAR **)&local_170, (LPCSTR)(intptr_t)local_158,
                              0xffffffffffffffffULL, 0xffffffffffffffffULL);
                PECMD_SetVariable(param_1, local_e8, local_170);
            }
            if (*pWVar24 != L'\0') {
                PECMD_StrDupA((WCHAR **)&local_170, (LPCSTR)((intptr_t)local_78 + 0x32a),
                              0xffffffffffffffffULL, 0xffffffffffffffffULL);
                PECMD_SetVariable(param_1, pWVar24, local_170);
            }
            if (*local_f0 != L'\0') {
                PECMD_StrDupA((WCHAR **)&local_170, (LPCSTR)local_res20, 0xffffffffffffffffULL,
                              0xffffffffffffffffULL);
                PECMD_SetVariable(param_1, local_f0, local_170);
            }
            BVar2 = *pBVar1;
            pBVar15 = pBVar1;
            while (BVar2 != '\0') {
                if (*pBVar15 == ',') {
                    *pBVar15 = ' ';
                }
                pBVar15 = pBVar15 + 1;
                BVar2 = *pBVar15;
            }
            if (*local_d8 != L'\0') {
                PECMD_StrDupA((WCHAR **)&local_170, (LPCSTR)pBVar1, 0xffffffffffffffffULL,
                              0xffffffffffffffffULL);
                PECMD_SetVariable(param_1, local_d8, local_170);
            }
            if (*local_c8 != L'\0') {
                PECMD_StrDupA((WCHAR **)&local_170, (LPCSTR)local_48, 0xffffffffffffffffULL,
                              0xffffffffffffffffULL);
                PECMD_SetVariable(param_1, local_c8, local_170);
            }
            if (*local_b0 != L'\0') {
                PECMD_SetVariable(param_1, local_b0, local_50);
            }
            if (*local_d0 != L'\0') {
                PECMD_SetVariable(param_1, local_d0, local_58);
            }
            if (*local_88 != L'\0') {
                PECMD_SetVariable(param_1, local_88, local_60);
            }
            if (*local_80 != L'\0') {
                PECMD_SetVariable(param_1, local_80, local_98);
            }
            if (*local_90 != L'\0') {
                PECMD_SetVariable(param_1, local_90, local_70);
            }
            bVar6 = local_160 != (LPCWSTR)0x0;
            local_160 = (LPWSTR)(intptr_t)-(intptr_t)local_160;
            DVar26 = ~-(uint)bVar6 & 0x37;
        }
        if (local_a0 == (LPCWSTR)0x0) {
            if (local_188 == '\0') {
                PECMD_FreeCacheBlock();
            }
        }
        else if (local_188 == '\0') {
            local_70 = (LPWSTR)0xa55aaa55;
            local_68 = g_pCacheBlock;
            (void)local_68;
            PECMD_VarWriteLine(param_1, local_a0, (LPCWSTR)&local_70, 0x10, (int64_t *)0, '\0');
            local_188 = '\x01';
            lVar14 = (int64_t)PECMD_VarLookup(param_1, local_a0, (void *)0, -1, (void **)0);
            PECMD_StrBldCopyWide((int64_t *)(lVar14 + 0x10), WSTR("*PCIP *freecah"));
        }
        else {
            /* local_a0 != 0 && local_188 != 0: 正常 */
        }
        PECMD_FreeStrBuf(&local_78);
        PECMD_FreeStrBuf(&local_98);
        PECMD_FreeStrBuf(&local_a8);
        PECMD_FreeStrBuf(&local_e0);
        LeaveCriticalSection((LPCRITICAL_SECTION)&g_csInit);
        PECMD_FreeStrBuf(&local_138);
        lVar14 = (int64_t)(int)DVar26;
        goto LAB_1400b01d8;
    }
    PECMD_FreeStrBuf(&local_138);
    lVar14 = -0x7ff8ffa9;
LAB_1400b01d8:
    PECMD_FreeStrBuf(&local_a0);
    PECMD_FreeStrBuf(&local_b8);
    PECMD_FreeStrBuf(&local_170);
    PECMD_FreeStrBuf(&local_90);
    PECMD_FreeStrBuf(&local_80);
    PECMD_FreeStrBuf(&local_88);
    PECMD_FreeStrBuf(&local_d0);
    PECMD_FreeStrBuf(&local_b0);
    PECMD_FreeStrBuf(&local_c8);
    PECMD_FreeStrBuf(&local_f8);
    PECMD_FreeStrBuf(&local_c0);
    PECMD_FreeStrBuf(&local_d8);
    PECMD_FreeStrBuf(&local_f0);
    PECMD_FreeStrBuf(&local_130);
    PECMD_FreeStrBuf(&local_e8);
    PECMD_FreeStrBuf(&local_120);
    return lVar14;
}

/* ================================================================
 * @0x1400b02c4  对象构造派发 (被 PECMD_PcipDispatch 调用)
 * signature: undefined __fastcall PECMD_CreateTimerObject(WPARAM param_1,
 *   undefined8 param_2, undefined8 * param_3, undefined8 * param_4,
 *   int param_5, uint param_6)
 * param_1 为 WPARAM 型, 但按反汇编实为对象基址 (WPARAM+0x1a0 等)。
 */
void PECMD_CreateTimerObject(WPARAM param_1, uint64_t param_2, uint64_t *param_3, uint64_t *param_4,
                             int param_5, uint32_t param_6)
{
    longlong lVar1;
    uint64_t *puVar2;
    uint64_t *puVar3;
    uint64_t *puVar4;
    WPARAM base;

    base = (WPARAM)param_1;
    puVar2 = (uint64_t *)PECMD_SendCtrlMessage_0834(param_1, *param_3);
    puVar4 = (uint64_t *)0;
    if (puVar2 != (uint64_t *)0) {
        lVar1 = **(longlong **)(uintptr_t)(base + 0x1a0);
        puVar3 = (uint64_t *)operator_new(0x88);
        if (puVar3 != (uint64_t *)0) {
            puVar4 = PECMD_InitTimerObject(
                puVar3, param_2,
                (uint32_t)((int64_t)(((uintptr_t)puVar2 - (uint64_t)lVar1) >> 3) + 0x1000), param_3,
                param_4, param_5, *(uint64_t *)(uintptr_t)(base + 0x20), param_6);
        }
        *puVar2 = (uint64_t)(uintptr_t)puVar4;
    }
    return;
}

/* ================================================================
 * @0x1400b0bf8  异步脚本/线程启动 (CreateThread 派发)
 * signature: longlong __fastcall PECMD_LaunchAsyncScriptThread(longlong * param_1,
 *   WCHAR * param_2, ulonglong param_3)
 * 解析 "$"/"*"/"+"... 前缀与开关参数, 构造执行对象并用 CreateThread
 * 异步启动 PECMD_ScriptThreadProc。
 */
int64_t PECMD_LaunchAsyncScriptThread(int64_t *param_1, WCHAR *param_2, uint64_t param_3)
{
    int *piVar1;
    WCHAR WVar2;
    uint uVar3;
    uint64_t *lpParameter;
    char cVar4;
    DWORD DVar5;
    uint64_t uVar6;
    LARGE_INTEGER _Memory;
    LARGE_INTEGER _Memory_00;
    int64_t *plVar7;
    HANDLE hThread;
    uint64_t uVar8;
    int iVar9;
    uint uVar10;
    uint uVar11;
    WCHAR *pWVar12;
    LPCWSTR pWVar13;
    WCHAR *pWVar14;
    WCHAR *pWVar15;
    uint uVar16;
    WCHAR *pWVar17;
    int64_t lVar18;
    int64_t *plVar19;
    WCHAR *pWVar20;
    uint uVar21;
    byte bVar22;
    uint uVar23;
    WCHAR WVar24;
    WCHAR *pWVar25;
    WCHAR *pWVar26;
    short sVar27;
    int bVar28;
    WCHAR *local_res10;
    uint64_t local_res18;
    uint local_res20;
    uint local_108;
    uint local_104;
    WCHAR *local_100;
    uint local_f8;
    DWORD local_f4;
    uint64_t local_f0;
    uint64_t local_e8;
    LPCWSTR local_e0;
    WCHAR *local_d8;
    int local_d0;
    LPCWSTR local_c8;
    int local_c0;
    uint local_bc;
    uint64_t local_b8;
    uint64_t local_b0;
    wchar_t *local_a8;
    LPCWSTR local_a0;
    LARGE_INTEGER local_98;
    LARGE_INTEGER local_90;
    WCHAR local_88[36];

    pWVar14 = (WCHAR *)0x0;
    uVar21 = 0;
    local_108 = 0;
    uVar10 = 0;
    local_res20 = 0;
    local_f8 = 0;
    local_e8 = 0;
    local_f0 = 0;
    local_c0 = 0;
    local_104 = 0;
    sVar27 = 0;
    local_b0 = 0x400000;
    local_b8 = 0x400000;
    pWVar15 = (WCHAR *)0x1;
    WVar2 = *param_2;
    local_100 = param_2;
    pWVar12 = pWVar14;
    pWVar17 = pWVar14;
    pWVar26 = pWVar14;
    while ((WVar2 != L'\0' && (((local_res20 = (uint)(uintptr_t)pWVar12,
                                 (ushort)*local_100 < 9 || (0xd < (ushort)*local_100)) &&
                                (*local_100 != L' '))))) {
        pWVar25 = pWVar15;
        if (*local_100 != L'$') {
            pWVar25 = pWVar26;
            if (*local_100 == L'*') {
                local_f8 = (uint)(char)*local_100;
                if (local_100[1] == L'?') {
                    local_f8 = local_f8 | 0x100;
                    local_100 = local_100 + 1;
                }
                pWVar17 = (WCHAR *)(uintptr_t)local_f8;
                pWVar12 = pWVar14;
            }
            else if (*local_100 == L'+') {
                pWVar12 = (WCHAR *)(uintptr_t)(uint)(int)(char)*local_100;
                local_f8 = local_f8 & 0xffffff00;
                pWVar17 = (WCHAR *)(uintptr_t)local_f8;
            }
            else if (*local_100 == L'&') {
                uVar8 = (uint64_t)local_e8 >> 0x20;
                local_e8 = CONCAT44((uint32_t)uVar8, 1);
            }
            else if (*local_100 == L'#') {
                local_c0 = (int)(char)*local_100;
            }
        }
        local_res20 = (uint)(uintptr_t)pWVar12;
        local_100 = local_100 + 1;
        WVar2 = *local_100;
        pWVar26 = pWVar25;
    }
    uVar11 = local_res20;
    uVar16 = (uint)(uintptr_t)pWVar17;
    iVar9 = (int)(uintptr_t)pWVar26;
    local_e0 = (LPCWSTR)0x0;
    local_c8 = (LPCWSTR)0x0;
    local_a0 = (LPCWSTR)0x0;
    local_a8 = (wchar_t *)WSTR("&&__LastTID");
    pWVar20 = pWVar14;
    pWVar12 = pWVar14;
    local_res18 = param_3;
    PECMD_SkipLeadingControlChars((WCHAR **)&local_100);
    pWVar25 = local_100;
    bVar22 = (byte)(uintptr_t)pWVar15;
    local_d0 = -0x80000000;
    cVar4 = '*';
    if (*local_100 == L'-') {
        cVar4 = '-';
    }
    WVar24 = (WCHAR)cVar4;
    WVar2 = *local_100;
    local_res10 = local_100;
    uVar3 = (uint)(uintptr_t)pWVar20;
    while (WVar24 == WVar2) {
        cVar4 = PECMD_MatchAndPad((const void *)0x140120180, &local_100, 5);
        pWVar12 = (WCHAR *)0x0;
        if (cVar4 != '\0') {
            bVar22 = 1;
            local_108 = 1;
            goto LAB_1400b10dd;
        }
        cVar4 = PECMD_MatchAndPad((const void *)0x140129384, &local_100, 6);
        pWVar12 = (WCHAR *)0x0;
        if (cVar4 == '\0') {
            cVar4 = PECMD_MatchAndPad((const void *)0x14012937c, &local_100, 5);
            pWVar12 = (WCHAR *)0x0;
            if (cVar4 != '\0') {
                uVar10 = (uint)(uintptr_t)pWVar14;
                pWVar14 = (WCHAR *)(uintptr_t)(uVar10 | 0x100);
                local_f0 = CONCAT44((uint32_t)uVar10, uVar10) | 0x100;
                goto LAB_1400b10d2;
            }
            cVar4 = PECMD_MatchAndPad((const void *)0x140121f58, &local_100, 6);
            pWVar12 = (WCHAR *)0x0;
            if (cVar4 != '\0') {
                sVar27 = 0x200;
                goto LAB_1400b10d2;
            }
            cVar4 = PECMD_MatchAndPad((const void *)0x140129374, &local_100, 6);
            pWVar12 = (WCHAR *)0x0;
            if (cVar4 != '\0') {
                bVar22 = 1;
                local_104 = 0x400;
                goto LAB_1400b10dd;
            }
            cVar4 = PECMD_MatchAndPad((const void *)0x14012936c, &local_100, 5);
            pWVar12 = (WCHAR *)0x0;
            if (cVar4 != '\0') {
                local_d0 = -0xf;
                goto LAB_1400b10d2;
            }
            cVar4 = PECMD_MatchAndPad((const void *)0x140129364, &local_100, 4);
            pWVar12 = (WCHAR *)0x0;
            if (cVar4 != '\0') {
                bVar22 = 1;
                pWVar26 = (WCHAR *)0x1;
                goto LAB_1400b10dd;
            }
            cVar4 = PECMD_MatchAndPad((const void *)0x14012935c, &local_100, 6);
            if (cVar4 == '\0') {
                cVar4 = PECMD_MatchAndPad((const void *)0x140129354, &local_100, 5);
                pWVar15 = local_100;
                pWVar12 = (WCHAR *)0x0;
                if (cVar4 == '\0') {
                    uVar6 = PECMD_AsciiPrefixICmp("tid:", (const WCHAR *)(local_100 + 1), 4);
                    if ((char)uVar6 == (char)(uintptr_t)pWVar12) {
                        uVar6 = PECMD_AsciiPrefixICmp("htid:", (const WCHAR *)(pWVar15 + 1), 5);
                        if ((char)uVar6 == (char)(uintptr_t)pWVar12) {
                            WVar2 = *pWVar15;
                            while (((WVar2 != (WCHAR)(uintptr_t)pWVar12 &&
                                     (((ushort)WVar2 < 9 || (0xd < (ushort)WVar2)))) &&
                                    (WVar2 != L' '))) {
                                pWVar15 = pWVar15 + 1;
                                local_100 = pWVar15;
                                WVar2 = *pWVar15;
                            }
                            PECMD_SkipLeadingControlChars((WCHAR **)&local_100);
                            pWVar14 = (WCHAR *)(local_f0 & 0xffffffff);
                            goto LAB_1400b10d2;
                        }
                        pWVar13 = pWVar15 + 6;
                        WVar2 = *pWVar13;
                        pWVar14 = (WCHAR *)pWVar13;
                        while (((WVar2 != (WCHAR)(uintptr_t)pWVar12 &&
                                 (((ushort)WVar2 < 9 || (0xd < (ushort)WVar2)))) &&
                                (WVar2 != L' '))) {
                            pWVar14 = pWVar14 + 1;
                            WVar2 = *pWVar14;
                        }
                        local_100 = pWVar14;
                        PECMD_StrCopyW((WCHAR **)&local_c8, pWVar13,
                                       ((int64_t)pWVar14 - (int64_t)pWVar13) >> 1);
                        local_a0 = local_c8;
                    }
                    else {
                        pWVar13 = pWVar15 + 5;
                        WVar2 = *pWVar13;
                        pWVar14 = (WCHAR *)pWVar13;
                        while (
                            (WVar2 != (WCHAR)(uintptr_t)pWVar12 &&
                             ((((ushort)WVar2 < 9 || (0xd < (ushort)WVar2)) && (WVar2 != L' '))))) {
                            pWVar14 = pWVar14 + 1;
                            WVar2 = *pWVar14;
                        }
                        local_100 = pWVar14;
                        PECMD_StrCopyW((WCHAR **)&local_e0, pWVar13,
                                       ((int64_t)pWVar14 - (int64_t)pWVar13) >> 1);
                        local_a8 = (wchar_t *)(uintptr_t)local_e0;
                    }
                    bVar22 = 1;
                    if (pWVar15 < pWVar14) {
                        lVar18 = ((uint64_t)((int64_t)pWVar14 + (-1 - (int64_t)pWVar15)) >> 1) + 1;
                        for (; lVar18 != 0; lVar18 = lVar18 + -1) {
                            *pWVar15 = L' ';
                            pWVar15 = pWVar15 + 1;
                        }
                    }
                    PECMD_SkipLeadingControlChars((WCHAR **)&local_100);
                    pWVar14 = (WCHAR *)(local_f0 & 0xffffffff);
                    pWVar12 = (WCHAR *)0x0;
                    goto LAB_1400b10dd;
                }
                pWVar17 = (WCHAR *)0x2a;
            }
            else {
                pWVar17 = (WCHAR *)0x12a;
            }
            pWVar15 = (WCHAR *)0x0;
            local_res20 = 0;
            local_f8 = (uint)(uintptr_t)pWVar17;
            bVar22 = 1;
            pWVar12 = pWVar15;
        }
        else {
            local_108 = 2;
        LAB_1400b10d2:
            bVar22 = 1;
        LAB_1400b10dd:
            pWVar15 = (WCHAR *)(uintptr_t)local_res20;
        }
        uVar16 = (uint)(uintptr_t)pWVar17;
        iVar9 = (int)(uintptr_t)pWVar26;
        uVar10 = (uint)(uintptr_t)pWVar14;
        uVar11 = (uint)(uintptr_t)pWVar15;
        uVar21 = local_104;
        uVar3 = local_108;
        WVar2 = *local_100;
    }
    uVar23 = (uint)(uintptr_t)pWVar12;
    if ((uVar11 != uVar23) || (uVar3 != uVar23)) {
        uVar21 = uVar23;
        local_104 = uVar23;
    }
    local_bc = (uint)(int)sVar27 | uVar21 | uVar10 | (uint)local_e8;
    PECMD_SkipLeadingControlChars((WCHAR **)&local_100);
    if (*local_100 != (WCHAR)(uintptr_t)pWVar12) {
        local_d8 = pWVar12;
        if (iVar9 != (int)(uintptr_t)pWVar12) {
            PECMD_ExpandCommandLine(param_1, pWVar25, &local_d8, 0, bVar22);
            pWVar12 = (WCHAR *)0x0;
            local_res10 = local_d8;
        }
        PECMD_SkipLeadingControlChars((WCHAR **)&local_res10);
        pWVar17 = local_res10;
        WVar2 = *local_res10;
        local_100 = local_res10;
        if (WVar2 != (WCHAR)(uintptr_t)pWVar12) {
            if (WVar24 == WVar2) {
                if ((WVar24 == local_res10[1]) &&
                    (uVar6 = PECMD_AsciiPrefixICmp("st:", (const WCHAR *)(local_res10 + 2), 3),
                     (char)uVar6 != (char)(uintptr_t)pWVar12)) {
                    local_100 = pWVar17 + 5;
                    PECMD_ParseSizeAndSkipWs((int64_t *)&local_100, &local_b8);
                    local_b8 = (local_b8 + 0x1fff) & 0xffffffffffffe000ULL;
                    pWVar12 = (WCHAR *)0x0;
                    local_b0 = local_b8;
                }
                while (WVar24 == WVar2) {
                    while ((WVar2 != (WCHAR)(uintptr_t)pWVar12 &&
                            (((local_res10 = pWVar17, (ushort)WVar2 < 9 || (0xd < (ushort)WVar2)) &&
                              (WVar2 != L' '))))) {
                        pWVar17 = pWVar17 + 1;
                        local_res10 = pWVar17;
                        WVar2 = *pWVar17;
                    }
                    PECMD_SkipLeadingControlChars((WCHAR **)&local_res10);
                    pWVar17 = local_res10;
                    WVar2 = *local_res10;
                }
            }
            pWVar26 = pWVar12;
            if ((char)local_f8 != (char)(uintptr_t)pWVar12) {
                pWVar26 = (WCHAR *)(uintptr_t)param_1[8];
            }
            uVar10 = (uint)(uintptr_t)pWVar12;
            if ((((uint32_t)local_f0 == uVar10) || (local_108 == uVar10)) &&
                ((pWVar26 != pWVar12 && ((bVar28 = *(uint *)((uintptr_t)pWVar26 + 0xe8) != uVar10,
                                          uVar10 = 1, bVar28 && ((uVar16 >> 8 & 1) != 0)))))) {
                lVar18 = -0x7ff8ffa9;
            }
            else {
                _Memory = PECMD_LI((int64_t)(uintptr_t)operator_new(8));
                if ((long long *)(uintptr_t)_Memory.QuadPart == (long long *)0) {
                    _Memory.QuadPart = 0;
                }
                else {
                    plVar19 = (int64_t *)(uintptr_t)param_1[0xf];
                    *(long long *)(uintptr_t)_Memory.QuadPart = 0;
                    if (plVar19 != (int64_t *)0) {
                        EnterCriticalSection((LPCRITICAL_SECTION)&g_csInit);
                        if (*plVar19 != 0) {
                            piVar1 = (int *)(*plVar19 + 8);
                            *piVar1 = *piVar1 + 1;
                            *(long long *)(uintptr_t)_Memory.QuadPart = *plVar19;
                        }
                        LeaveCriticalSection((LPCRITICAL_SECTION)&g_csInit);
                    }
                }
                lVar18 = param_1[0x10];
                local_98 = _Memory;
                (void)local_98;
                _Memory_00 = PECMD_LI((int64_t)(uintptr_t)operator_new(0xf0));
                if ((long long *)(uintptr_t)_Memory_00.QuadPart == (long long *)0) {
                    _Memory_00.QuadPart = 0;
                }
                else {
                    PECMD_ScriptCopy((uint64_t *)(uintptr_t)_Memory_00.QuadPart,
                                     (uint64_t *)param_1);
                    PECMD_ScriptInit((uint64_t *)(uintptr_t)_Memory_00.QuadPart, (int64_t)param_1);
                    *(WCHAR **)(uintptr_t)(_Memory_00.QuadPart + 0x40) = pWVar26;
                }
                if (((uint32_t)local_f0 == 0) &&
                    (*(uint8_t *)(uintptr_t)(_Memory_00.QuadPart + 0xd9) = 1,
                     pWVar26 == (WCHAR *)0x0)) {
                    *(uint64_t **)(uintptr_t)(_Memory_00.QuadPart + 0x38) = (uint64_t *)&g_Script;
                }
                *(long long *)(uintptr_t)(_Memory_00.QuadPart + 0x80) = lVar18;
                local_90 = _Memory_00;
                (void)local_90;
                PECMD_ArgTokenize((int64_t *)(uintptr_t)_Memory_00.QuadPart, local_d8, 0);
                iVar9 = (uint32_t)local_f0;
                ((LARGE_INTEGER *)(uintptr_t)(_Memory_00.QuadPart + 0x78))->QuadPart =
                    (int64_t)_Memory.QuadPart;
                plVar19 = param_1;
                if (((uint32_t)local_f0 == 0) &&
                    (plVar19 = (int64_t *)&g_Script, pWVar26 != (WCHAR *)0x0)) {
                    plVar19 = *(int64_t **)(uintptr_t)((uintptr_t)pWVar26 + 0x148);
                    *(int64_t **)(uintptr_t)(_Memory_00.QuadPart + 0x38) = plVar19;
                }
                EnterCriticalSection((LPCRITICAL_SECTION)&g_csInit);
                if (((iVar9 == 0) && ((char)local_f8 != '\0')) && (local_108 != 0)) {
                    iVar9 = 0x7fffd;
                    plVar7 = param_1;
                    do {
                        if (plVar7 == (int64_t *)0)
                            break;
                        if (((*(byte *)((intptr_t)plVar7 + 0xd9) & 1) != 0) &&
                            ((*(byte *)((intptr_t)plVar7 + 0xd9) & 0x42) != 0)) {
                            *(int64_t **)(uintptr_t)(_Memory_00.QuadPart + 0x38) = plVar7;
                            plVar19 = plVar7;
                            break;
                        }
                        iVar9 = iVar9 + -1;
                        plVar7 = (int64_t *)(uintptr_t)plVar7[7];
                    } while (-1 < iVar9);
                }
                PECMD_ExpandScriptVariables((int64_t *)(uintptr_t)_Memory_00.QuadPart, param_1,
                                            plVar19);
                uVar21 = (uint32_t)local_e8;
                if ((uint32_t)local_e8 != 0) {
                    PECMD_ImportEnvironment((int64_t *)(uintptr_t)_Memory_00.QuadPart, (int64_t *)0,
                                            0);
                }
                LeaveCriticalSection((LPCRITICAL_SECTION)&g_csInit);
                *(uint8_t *)(uintptr_t)(_Memory_00.QuadPart + 0xda) =
                    *(uint8_t *)((intptr_t)param_1 + 0xda);
                *(uint8_t *)(uintptr_t)(_Memory_00.QuadPart + 0xd) =
                    *(uint8_t *)((intptr_t)param_1 + 0xd);
                if (uVar21 != 0) {
                    *(uint8_t *)(uintptr_t)(_Memory_00.QuadPart + 0xd) = 1;
                }
                PECMD_StrDupAssign(&local_f0, pWVar17);
                PECMD_AllocStringSlot2((void **)&local_e8, 0x88);
                lpParameter = (uint64_t *)(uintptr_t)local_e8;
                PECMD_ZeroLenBuf(lpParameter);
                lpParameter[1] = local_f0;
                lpParameter[2] = 0;
                lpParameter[3] = 4;
                lpParameter[4] = 0;
                lpParameter[5] = local_res18;
                lpParameter[6] = 0;
                lpParameter[7] = (uint64_t)(uintptr_t)pWVar26;
                lpParameter[8] = (uint64_t)(int64_t)(char)local_f8;
                lpParameter[9] = (uint64_t)(int64_t)(int)local_res20;
                lpParameter[10] = (uint64_t)(int64_t)(int)(local_bc | uVar21);
                lpParameter[0xb] = (uint64_t)(int64_t)local_c0;
                ((LARGE_INTEGER *)(lpParameter + 0xc))->QuadPart = (int64_t)_Memory.QuadPart;
                ((LARGE_INTEGER *)(lpParameter + 0xd))->QuadPart = (int64_t)_Memory_00.QuadPart;
                lpParameter[0xe] = (uint64_t)(uintptr_t)pWVar26;
                lpParameter[0xf] = (uint64_t)(uintptr_t)pWVar26;
                local_f4 = 0;
                bVar28 = 0;
                hThread =
                    CreateThread((LPSECURITY_ATTRIBUTES)0, (size_t)local_b0,
                                 (void *)PECMD_ScriptThreadProc, lpParameter, 0x10004, &local_f4);
                if (hThread == (HANDLE)0) {
                    DVar5 = GetLastError();
                    if (DVar5 == 0) {
                        DVar5 = 1;
                    }
                }
                else {
                    if (uVar10 == 0) {
                        uVar8 = 0;
                    }
                    else {
                        uVar8 = PECMD_GetWindowObjectRef((int64_t)(uintptr_t)pWVar26);
                    }
                    _Memory_00.QuadPart = 0;
                    lpParameter[6] = uVar8;
                    lpParameter[4] = (uint64_t)local_f4;
                    lpParameter[0] = (uint64_t)(uintptr_t)param_1;
                    EnterCriticalSection((LPCRITICAL_SECTION)&g_csInit);
                    if ((local_res20 == 0) && (g_taskCount = g_taskCount + 1, local_104 != 0)) {
                        g_i32D7EC = g_i32D7EC + 1;
                    }
                    if (g_foregroundFlag == '\0') {
                        g_foregroundFlag = '\x01';
                    }
                    LeaveCriticalSection((LPCRITICAL_SECTION)&g_csInit);
                    if ((unsigned int)local_d0 != 0x80000000u) {
                        SetThreadPriority(hThread, local_d0);
                    }
                    ResumeThread(hThread);
                    local_f0 = 0;
                    local_e8 = 0;
                    _Memory.QuadPart = _Memory_00.QuadPart;
                    DVar5 = 0;
                }
                if (local_e0 != (LPCWSTR)0) {
                    PECMD_VarSetUInt(param_1, (uint64_t)local_f4, (LPCWSTR)(uintptr_t)local_a8);
                }
                PECMD_VarSetUInt(param_1, (uint64_t)local_f4, WSTR("&&__LastTID"));
                if (((local_a0 != (LPCWSTR)0) && (*local_a0 != L'\0')) &&
                    (lVar18 = PECMD_WrapParamCall_02d8(param_1, hThread, local_a0), lVar18 != 0)) {
                    bVar28 = 1;
                }
                if ((hThread != (HANDLE)0) && (!bVar28)) {
                    CloseHandle(hThread);
                }
                if (((local_108 != 0) && (hThread != (HANDLE)0)) && (local_f4 != 0)) {
                    wsprintfW(local_88, WSTR(" -ncd *=%u"));
                    PECMD_DispatchSystemCommandLine(param_1, local_88);
                }
                lVar18 = (int64_t)(int)DVar5;
                PECMD_FreeStrBuf(&local_e8);
                PECMD_FreeStrBuf(&local_f0);
                if (_Memory_00.QuadPart != 0) {
                    PECMD_ClearTaskTable((int64_t)(uintptr_t)_Memory_00.QuadPart, 0);
                    PECMD_FreeStrBuf((void *)(_Memory_00.QuadPart + 0x70));
                    free((void *)(uintptr_t)_Memory_00.QuadPart);
                }
                if (_Memory.QuadPart != 0) {
                    PECMD_RefCountRelease((WCHAR **)(uintptr_t)_Memory.QuadPart);
                    free((void *)(uintptr_t)_Memory.QuadPart);
                }
            }
            PECMD_FreeStrBuf(&local_d8);
            PECMD_FreeStrBuf(&local_c8);
            PECMD_FreeStrBuf(&local_e0);
            return lVar18;
        }
        PECMD_FreeStrBuf(&local_d8);
    }
    PECMD_FreeStrBuf(&local_c8);
    PECMD_FreeStrBuf(&local_e0);
    return -0x7ff8ffa9;
}

/* ================================================================
 * @0x1400b4600  PCIP 命令分派 (SET / DHCP / 查询)
 * signature: undefined8 __fastcall PECMD_PcipDispatch(longlong * param_1,
 *   ushort * param_2, WPARAM param_3, undefined8 param_4)
 * param_2: 命令行; param_3: 分派上下文 (0 时用 param_1[8] 默认)。
 */
uint64_t PECMD_PcipDispatch(int64_t *param_1, ushort *param_2, WPARAM param_3, uint64_t param_4)
{
    ushort uVar1;
    int iVar2;
    int bVar3;
    int iVar4;
    uint64_t uVar5;
    WCHAR *plVar6;
    uint64_t *puVar7;
    uint uVar8;
    uint uVar9;
    ushort *puVar10;
    uint local_res8[2];
    ushort *local_res10;
    int local_res18[4];
    int64_t local_78;
    ushort *local_70;
    int64_t local_68;
    uint64_t local_60;
    WCHAR *local_58;
    int64_t *local_50;
    uint64_t local_48;

    local_res10 = param_2;
    PECMD_AllocStrSlot(&local_58);
    uVar9 = 0;
    local_48 = 0;
    local_50 = param_1;
    if (param_3 == 0) {
        PECMD_ResetScriptChain(param_1, (int64_t *)0);
        param_3 = (WPARAM)param_1[8];
        if (param_3 == 0) {
            PECMD_FreeStrBuf(&local_58);
            return 0xffffffff80070057ULL;
        }
    }
    PECMD_SkipLeadingControlChars((WCHAR **)&local_res10);
    PECMD_StrDupAssign(&local_78, (const WCHAR *)g_szEmpty);
    PECMD_StrDupAssign(&local_68, (const WCHAR *)g_szEmpty);
    local_res18[0] = 0;
    local_res8[0] = 0;
    puVar10 = local_res10;
    local_60 = param_4;
    (void)local_48;
    (void)local_50;
    (void)local_60;
    uVar5 = PECMD_AsciiPrefixICmp("-t:", (const WCHAR *)local_res10, 3);
    if ((char)uVar5 == '\0') {
    LAB_1400b47a6:
        uVar1 = *puVar10;
        if (uVar1 == 0x2a) {
            local_res10 = puVar10 + 1;
            PECMD_SkipLeadingControlChars((WCHAR **)&local_res10);
        }
        PECMD_SplitNextToken(param_1, (int64_t *)&local_res10, &local_78, 0x2c, 0);
        uVar5 = 1;
        if (*local_res10 == 0)
            goto LAB_1400b48af;
        puVar7 = (uint64_t *)PECMD_SplitTokenAssignVar(&local_58, (WCHAR **)&local_res10, 0x2c, 1);
        PECMD_ParseShortStore(puVar7, local_res18, 0x2c);
        iVar4 = (local_res18[0] != 0); /* TODO(verify): 原函数返回字段计数, void 签名归一 */
        iVar2 = local_res18[0];
        if (iVar4 != 0) {
            if ((local_res18[0] < 1) && (0 < (int)uVar9)) {
                uVar9 = uVar9 | 0x40000000;
            }
            if (*local_res10 == 0x2c) {
                local_res10 = local_res10 + 1;
                plVar6 = PECMD_SkipLeadingControlChars((WCHAR **)&local_res10);
                PECMD_AssignString((WCHAR **)&local_68, (LPCWSTR)(uintptr_t)*plVar6);
            }
            if (uVar1 != 0x2a) {
                param_1 = *(int64_t **)(param_3 + 0x290);
            }
            PECMD_CreateTimerObject(param_3, (uint64_t)param_1, (uint64_t *)&local_78,
                                    (uint64_t *)&local_68, iVar2, uVar9);
            uVar5 = 0;
            goto LAB_1400b48af;
        }
    }
    else {
        puVar10 = puVar10 + 3;
        for (local_res10 = puVar10;
             (uVar1 = *local_res10,
             uVar1 != 0 && (((uVar1 < 9 || (0xd < uVar1)) && (uVar1 != 0x20))));
             local_res10 = local_res10 + 1) {
        }
        local_70 = puVar10;
        PECMD_SkipLeadingControlChars((WCHAR **)&local_res10);
        uVar1 = *local_res10;
        *local_res10 = 0;
        if (*puVar10 == 0x2b) {
            local_70 = puVar10 + 1;
            uVar9 = 0x40000000;
        }
        plVar6 = (WCHAR *)PECMD_SplitTokenAssignVar(&local_58, (WCHAR **)&local_70, 0x2c, 1);
        bVar3 = (int)PECMD_ParseUIntValue((LPCWSTR *)&local_70, (int *)local_res8);
        if (bVar3 != 0) {
            *local_res10 = uVar1;
            uVar8 = local_res8[0];
            if ((int)local_res8[0] < 1) {
                uVar8 = 0x8fffffff;
                uVar9 = 0;
            }
            uVar9 = uVar8 | uVar9;
            puVar10 = local_res10;
            goto LAB_1400b47a6;
        }
    }
    uVar5 = 0xffffffff80070057ULL;
LAB_1400b48af:
    PECMD_FreeStrBuf(&local_68);
    PECMD_FreeStrBuf(&local_78);
    PECMD_FreeStrBuf(&local_58);
    return uVar5;
}
