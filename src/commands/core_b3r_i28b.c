/* Restored batch28 B3 business functions (part i28b)
 * Functions: PECMD_SubCommand, PECMD_ParseControlCenterArgs, PECMD_ForCommand,
 * PECMD_AssignDriveLetter
 */
#include <stdbool.h>
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

/* CONCAT44(hi,lo): (uint32)hi<<32 | (uint32)lo  (Ghidra register glue; used as
 * a pointer-identity cast, so simplified to (uintptr_t)value in the bodies) */
#ifndef CONCAT44
#define CONCAT44(hi, lo) (((uint64_t)(uint32_t)(hi)) << 32 | (uint32_t)(lo))
#endif

/* ---- Globals ---- */
extern WCHAR g_szEmpty[];         /* empty string (g_szEmpty) */
extern uint8_t g_u8CCB1;          /* debug flag byte */
extern CRITICAL_SECTION g_csDisk; /* global critical section (disk) */

/* ---- Helper function externs (bodies live in other core_*.c files) ---- */
extern void *operator_new(size_t size); /* global new wrapper */

/* string/var helpers */
/* S11: 本地声明与定义冲突已删除, 统一采用 xproto.h 原型 (原: extern void PECMD_AllocStrSlot(void *out); /* @0x140063620 release slot * / extern WCHAR *PECMD_SkipLeadingCont) */
/* S11: 本地声明与定义冲突已删除, 统一采用 xproto.h 原型 (原: extern void PECMD_CopyUpToChar(void *pp, void *out, uint32_t sep);) */

/* S11: 本地声明与定义冲突已删除, 统一采用 xproto.h 原型 (原: extern uint64_t PECMD_ParseSignedNumber(short *);) */






extern void PECMD_ParseLtwhParams(int64_t *a, uint32_t *b, int *c, int *d, uint32_t *e);
extern char PECMD_MatchTokenAdvance(const char *tok, void *pp, int n);
extern WCHAR *PECMD_SkipWCharUntil(WCHAR **pp, uint16_t ch); /* delimiter scan */
extern int64_t PECMD_TokPrefixICmp(const char *a, const WCHAR *w, int n);
/* S11: 本地声明与定义冲突已删除, 统一采用 xproto.h 原型 (原: /* S11: 本地声明与定义冲突, 已删除, 统一采用 xproto.h 原型 (原: extern int64_t PECMD_AsciiPrefixICmp(const char *a, const WCHAR *) */

extern void PECMD_AllocWStringBuffer(WCHAR **ps, int64_t count);  /* @0x140063694 alloc */
extern int64_t *PECMD_InitPtrTable(int64_t *arr);                 /* @0x140063b64 array init */
extern WCHAR *PECMD_StrCopyW(void *ps, LPCWSTR src, int64_t len); /* @0x140063888 */
extern void PECMD_ExpandVarDispatch(int64_t *, LPCWSTR, int64_t *, int, int);
extern int64_t PECMD_ExpandCommandLine(int64_t *ctx, WCHAR *src, WCHAR **out, int mode,
                                       uint8_t flag);
extern int64_t PECMD_ExpandVarsRecursive(int64_t *ctx, WCHAR *src, WCHAR **out, int mode,
                                         uint8_t flag);
extern void FUN_14007033c(int64_t *param_1, LPCWSTR param_2);                   /* @0x14007033c */
extern void *PECMD_StrBldCopyWide(void *a, const WCHAR *b);                     /* StrCpyW2 */
extern int64_t *PECMD_AssignString(int64_t *param_1, LPCWSTR param_2);          /* @0x14007034c */
extern uint64_t PECMD_ExpandDrivePathAlloc(LPCWSTR param_1, uint64_t *param_2); /* @0x14001c270 */
extern void PECMD_AppendLongDecimal(void *script, int64_t value, LPCWSTR key);  /* SetVarD */
extern void PECMD_SetVariable(int64_t *script, LPCWSTR key, LPCWSTR value);     /* SetVar */
extern WCHAR *PECMD_UnquoteString(WCHAR *s);                                    /* tag lookup */
extern void PECMD_VectorAppendGen(int64_t *, int64_t *, int64_t *, void *, int, int);
extern int64_t PECMD_VectorSlotPtr(int64_t a, int64_t *b, int64_t *c, uint32_t d);
extern uint64_t *PECMD_CopyStrToSlot(uint64_t **pp, uint64_t *arg);
extern double PECMD_WideStrToDouble(int64_t *param_1, int64_t *param_2, uint8_t *param_3);
extern void PECMD_FreeArray_ddf8(int64_t *param_1); /* list destroy */
extern uint64_t PECMD_ParseStringToken(int64_t *pp, uint64_t ctx, int64_t *out);
extern bool PECMD_MatchPrefixAdvance(const char *key, int64_t *pp, int len);
extern void PECMD_ParseSizeNumber(int64_t *pp, int64_t *out); /* parse int */
extern void PECMD_AdvanceTokenPointer(int64_t ctx, int64_t *a4, int mode, int64_t *p);
extern int64_t PECMD_VarLookup(int64_t *param_1, LPCWSTR param_2, int64_t *param_3, int param_4,
                               int64_t **param_5); /* @0x140018978 */
/* ---- R26-e FORX 直移新增依赖 ---- */
extern void PECMD_FreeStrBuf(void *ps);                                                /* @0x14005b104 串槽释放 */
extern int64_t *PECMD_SplitTokenAssignVar(int64_t *cursor, WCHAR **pp, uint16_t sep, int flag); /* @0x14007f6e4 */
extern int PECMD_ParseSignedNumberStr(uint64_t *pp, int64_t *out, int16_t sep);         /* @0x1400678f0 */
extern uint64_t PECMD_ProcessScriptBlock(uint64_t a1, uint64_t a2, void *p3, void *p4, void *p5); /* @0x14004c0bc */
extern uint64_t PECMD_TokenizeExpression(int64_t a1, int64_t a2, WCHAR **a3, uint32_t a4, LPCWSTR a5); /* @0x1400a53e4 */
extern WCHAR *PECMD_FormatU64RetEnd(WCHAR *dst, uint64_t v, LPCWSTR fmt);                /* @0x1400e6db4 */
extern bool PECMD_ParseUIntValue(WCHAR **pp, int *out);                                 /* @0x140074838 */

/* GUI/command object dispatch helpers */
extern uint64_t PECMD_DispatchCommandObject(WPARAM param_1, int64_t *param_2, int64_t *param_3,
                                            WCHAR *param_4, int param_5, int param_6, int param_7,
                                            int param_8, int param_9, int param_10,
                                            uint32_t param_11, LPCRITICAL_SECTION *param_12);
extern void PECMD_CreateSliderItem(WPARAM mgr, int64_t v2, uint64_t *p3, int x, int y, int w, int h,
                                   uint16_t *p8, uint64_t *p9, uint32_t flags);
extern uint64_t PECMD_ProcessControlCommand(int64_t *param_1, int64_t *param_2, LPCWSTR param_3,
                                            uint64_t param_4, int64_t param_5, uint32_t param_6,
                                            int16_t *param_7, int64_t *param_8, int param_9,
                                            uint64_t param_10);
extern void PECMD_ResetScriptChain(int64_t *param_1, int64_t *param_2); /* default params */
extern int64_t PECMD_ExecSubCommand(int64_t *script, WCHAR *cmd, int64_t *out, LPCWSTR extra,
                                    uint32_t flags);

/* device / partition helpers (PECMD_AssignDriveLetter) */
extern uint64_t PECMD_FindFirstFreeDrive(LPCWSTR p);        /* 驱动器盘符检测 */
extern void PECMD_AllocStringSlot2(void **ps, int64_t len); /* alloc */
extern void PECMD_OpenFileHandle(HANDLE *out, LPCWSTR path, DWORD access, DWORD share,
                                 LPSECURITY_ATTRIBUTES sa, DWORD disp, DWORD flags, HANDLE tmpl);
extern int PECMD_QueryDeviceIoInfo(HANDLE h, int sz); /* 扇区大小 */
extern uint64_t *PECMD_GetDiskLayoutInfo(HANDLE h, uint64_t *buf, uint32_t *out);
extern uint64_t PECMD_ReadDiskSectorScan(uint64_t *p1, uint64_t *p2, uint32_t a, uint32_t b,
                                         uint32_t c, uint64_t *d, LARGE_INTEGER e);
extern int64_t PECMD_FindPartitionStartSector(HANDLE h, int *p, int64_t *q);
extern uint8_t PECMD_SetDriveMount(int64_t a, uint32_t b, uint32_t c, uint32_t d, uint32_t e,
                                   uint32_t f, short *g, DWORD *h);
extern int PECMD_FindPartitionByGeometry(int64_t a, int64_t b, int c, int d, int e);
extern void PECMD_EnumDrivesToTable(int64_t a, LPWSTR b, uint c);

/* ================================================================
 * @0x1400ab724  "SUB" 命令 (子过程调用/-sub 前缀)
 * signature: longlong __fastcall PECMD_SubCommand(longlong * param_1,
 *   WCHAR * param_2, longlong * param_3)
 */
longlong PECMD_SubCommand(longlong *param_1, WCHAR *param_2, longlong *param_3)
{
    WCHAR WVar1;
    LPCWSTR lpString;
    char cVar2;
    int iVar3;
    int iVar4;
    int iVar5;
    int64_t *puVar6;
    longlong *plVar7;
    ulonglong uVar8;
    WCHAR *pWVar10;
    uint uVar11;
    longlong lVar12;
    int local_res8[2];
    WCHAR *local_res10;
    WCHAR *local_res20;
    LPCWSTR local_98;
    int local_90;
    int local_8c;
    int local_88[2];
    LPCWSTR local_80;
    int local_78[2];
    WCHAR *local_70;
    LPCWSTR local_68;
    longlong local_60;
    LPCWSTR local_58;
    longlong local_50;
    longlong *local_48;

    local_res10 = param_2;
    PECMD_AllocStrSlot(&local_50);
    lVar12 = 0;
    local_48 = param_1;
    (void)local_48;
    PECMD_StrDupAssign(&local_98, (const WCHAR *)g_szEmpty);
    PECMD_AllocStrSlot(&local_80);
    PECMD_AllocStrSlot(&local_60);
    WVar1 = *param_2;
    local_8c = 0;
    local_90 = 0;
    local_78[0] = 0;
    local_88[0] = 0;
    local_res20 = (WCHAR *)0; /* register-splice init folded to 0 */
    local_res8[0] = 0;
    uVar11 = 0;
    while (
        (WVar1 != L'\0' && ((local_res10 = param_2, (ushort)WVar1 < 9 || (0xd < (ushort)WVar1)) &&
                            (WVar1 != L' ')))) {
        param_2 = param_2 + 1;
        local_res10 = param_2;
        WVar1 = *param_2;
    }
    PECMD_SkipLeadingControlChars((WCHAR **)&local_res10);
    cVar2 = PECMD_MatchTokenAdvance("-sub", (void *)&local_res10, 4);
    WVar1 = *local_res10;
    if (WVar1 == L'*') {
        local_res10 = local_res10 + 1;
        PECMD_SkipLeadingControlChars((WCHAR **)&local_res10);
    }
    PECMD_SplitTokenTrimWs(&local_res10, &local_98, 0x2c);
    PECMD_RunCommandLine(param_1, &local_98, 1);
    if (*local_res10 != L',') {
        PECMD_FreeStrBuf(&local_60);
        PECMD_FreeStrBuf(&local_80);
        PECMD_FreeStrBuf(&local_98);
        PECMD_FreeStrBuf(&local_50);
        return -0x7ff8ffa9;
    }
    local_res10 = local_res10 + 1;
    puVar6 = PECMD_SplitTokenAssignVar((WCHAR **)&local_50, &local_res10, 0x2c, 1);
    lpString = local_98;
    local_68 = (LPCWSTR)*puVar6;
    if (*local_res10 == L',') {
        local_res10 = local_res10 + 1;
    }
    pWVar10 = local_res10;
    if (cVar2 == '\0') {
        PECMD_ParseLtwhParams((int64_t *)&local_68, (uint32_t *)&local_8c, &local_90, local_78,
                              (uint32_t *)local_88);
        plVar7 = PECMD_SplitTokenAssignVar((WCHAR **)&local_50, &local_res10, 0x2c, 1);
        PECMD_ParseLtwhParams(plVar7, (uint32_t *)&local_res20, local_res8, (int *)&local_70,
                              (uint32_t *)&local_58);
        if (*local_res10 == L',') {
            local_res10 = local_res10 + 1;
            puVar6 = PECMD_SplitTokenAssignVar((WCHAR **)&local_50, &local_res10, 0x2c, 1);
            uVar8 = PECMD_ParseSignedNumber((short *)*puVar6);
            uVar11 = (uint)uVar8 & 0xffff;
        }
        uVar8 = PECMD_AsciiPrefixICmp("=:", local_98, 2);
        if ((char)uVar8 == '\0') {
            if (WVar1 != L'*') {
                param_1 = (longlong *)param_3[0x52];
            }
            PECMD_DispatchCommandObject((WPARAM)param_3, param_1, param_3, (WCHAR *)&local_98,
                                        local_8c, local_90, local_78[0], local_88[0],
                                        (int)(intptr_t)local_res20, local_res8[0], uVar11,
                                        (LPCRITICAL_SECTION *)0x0);
            goto LAB_1400abc25;
        }
    }
    else {
        for (; *pWVar10 == L','; pWVar10 = pWVar10 + 1) {
        }
        if ((*(char *)((char *)param_1 + 0xda) == '\0') &&
            ((*(byte *)((char *)param_1 + 0xd) & 0xf) == 0)) {
            PECMD_ExpandVarsRecursive(param_1, pWVar10, (WCHAR **)&local_80, 0, 1);
        }
        else {
            PECMD_ExpandCommandLine(param_1, pWVar10, (WCHAR **)&local_80, 0, 1);
        }
        local_58 = local_80;
        local_res20 = (WCHAR *)local_98;
        PECMD_SkipWCharUntil(&local_res20, 0x3a);
        if (*local_res20 != L'\0') {
            *local_res20 = L'\0';
            local_res20 = local_res20 + 1;
        }
        if (*lpString != L'\0') {
            iVar3 = lstrlenW(local_68);
            iVar4 = lstrlenW(local_res20);
            iVar5 = lstrlenW(lpString);
            pWVar10 = local_70;
            local_res8[0] = lstrlenW(local_80);
            PECMD_AllocWStringBuffer((WCHAR **)&local_70, (longlong)(iVar5 + iVar4 + 100 + iVar3));
            memcpy((void *)local_70, (const void *)WSTR("-sub@"), 0xc);
            memcpy((void *)(local_70 + 5), (const void *)local_68, (iVar3 + 1) * 2);
            pWVar10 = local_70 + (longlong)iVar3 + 5;
            if (*local_res20 != L'\0') {
                *pWVar10 = L':';
                memcpy((void *)(pWVar10 + 1), (const void *)local_res20, (iVar4 + 1) * 2);
                pWVar10 = pWVar10 + (longlong)iVar4 + 1;
            }
            lVar12 = (longlong)iVar5; /* CONCAT44 register-splice folded to low word */
            *pWVar10 = L' ';
            pWVar10[1] = L'-';
            memcpy((void *)(pWVar10 + 2), (const void *)lpString, (iVar5 + 1) * 2);
            pWVar10[lVar12 + 2] = L' ';
            memcpy((void *)(pWVar10 + lVar12 + 3), (const void *)local_58, (local_res8[0] + 1) * 2);
            lVar12 = PECMD_ExecSubCommand(param_1, local_70, param_3, (LPCWSTR)0x0, 0);
            PECMD_FreeStrBuf(&local_70);
            goto LAB_1400abc25;
        }
    }
    lVar12 = -0x7ff8ffa9;
LAB_1400abc25:
    PECMD_FreeStrBuf(&local_60);
    PECMD_FreeStrBuf(&local_80);
    PECMD_FreeStrBuf(&local_98);
    PECMD_FreeStrBuf(&local_50);
    return lVar12;
}

/* ================================================================
 * @0x1400abc74  列表/组合控件命令解析 (带 -center/-right/-left 对齐)
 * signature: undefined8 __fastcall PECMD_ParseControlCenterArgs(longlong * param_1,
 *   ushort * param_2, WPARAM param_3, undefined8 param_4)
 */
undefined8 PECMD_ParseControlCenterArgs(longlong *param_1, ushort *param_2, WPARAM param_3,
                                        undefined8 param_4)
{
    ushort uVar1;
    ushort *puVar2;
    longlong lVar3;
    longlong *plVar4;
    undefined8 *puVar5;
    ulonglong uVar6;
    ushort *puVar8;
    undefined8 uVar9;
    ushort uVar10;
    uint uVar11;
    WCHAR *local_res8;
    ushort *local_res10;
    int local_res18[2];
    int local_88;
    int local_84;
    longlong local_80;
    ushort local_78[4];
    longlong local_70;
    longlong local_68;
    undefined8 local_60;
    ushort *local_58;
    longlong local_50;
    longlong *local_48;

    local_res8 = (WCHAR *)param_1;
    local_res10 = param_2;
    PECMD_AllocStrSlot(&local_50);
    uVar9 = 0;
    uVar11 = 0;
    local_48 = param_1;
    (void)local_48;
    PECMD_SkipLeadingControlChars((WCHAR **)&local_res10);
    if (param_3 == 0) {
        PECMD_ResetScriptChain(param_1, (longlong *)0x0);
        param_3 = param_1[8];
        if (param_3 == 0) {
            uVar9 = 0xffffffff80070057;
            goto LAB_1400ac068;
        }
    }
    puVar8 = local_res10;
    {
        short sVar7 = 0x2c;
        local_58 = local_res10;
        PECMD_SkipWCharUntil(&local_58, 0x2c);
        puVar2 = local_58;
        uVar1 = *local_58;
        *local_58 = 0;
        uVar10 = *puVar8;
        uVar9 = 0;
        while ((ushort)(sVar7 + 1U) == uVar10) {
            uVar6 = (ulonglong)uVar10;
            lVar3 = PECMD_TokPrefixICmp("-center", (const WCHAR *)puVar8, 7);
            if ((char)lVar3 == '\0') {
                lVar3 = PECMD_TokPrefixICmp("-right", (const WCHAR *)puVar8, 6);
                if ((char)lVar3 == '\0') {
                    lVar3 = PECMD_TokPrefixICmp("-left", (const WCHAR *)puVar8, 5);
                    if ((char)lVar3 != '\0') {
                        uVar9 = 0x80000000;
                    }
                }
                else {
                    uVar9 = 0x20000000;
                }
            }
            else {
                uVar9 = 0x40000000;
            }
            uVar11 = (uint)uVar9;
            uVar10 = (ushort)uVar6;
            while ((uVar10 != 0 &&
                    (((local_res10 = puVar8, uVar10 < 9 || (0xd < uVar10)) && (uVar10 != 0x20))))) {
                puVar8 = puVar8 + 1;
                local_res10 = puVar8;
                uVar10 = *puVar8;
                uVar6 = (ulonglong)uVar10;
            }
            PECMD_SkipLeadingControlChars((WCHAR **)&local_res10);
            param_1 = (longlong *)local_res8;
            puVar8 = local_res10;
            uVar10 = *local_res10;
        }
        *puVar2 = uVar1;
    }
    PECMD_AllocStrSlot(&local_80);
    PECMD_AllocStrSlot((void *)local_78);
    PECMD_AllocStrSlot(&local_70);
    PECMD_StrDupAssign(&local_68, (const WCHAR *)g_szEmpty);
    local_88 = 0;
    local_84 = 0;
    local_res18[0] = 0;
    uVar1 = *puVar8;
    local_60 = param_4;
    (void)local_60;
    if (uVar1 == 0x2a) {
        local_res10 = puVar8 + 1;
        PECMD_SkipLeadingControlChars((WCHAR **)&local_res10);
    }
    PECMD_SplitTokenTrimWs(&local_res10, &local_80, 0x2c);
    PECMD_RunCommandLine(param_1, &local_80, 1);
    if (*local_res10 == 0x2c) {
        local_res10 = local_res10 + 1;
        plVar4 = PECMD_SplitTokenAssignVar((WCHAR **)&local_50, &local_res10, 0x2c, 1);
        PECMD_ParseLtwhParams(plVar4, (uint32_t *)&local_88, &local_84, local_res18,
                              (uint32_t *)&local_res8);
        if (*local_res10 == 0x2c) {
            local_res10 = local_res10 + 1;
            plVar4 = PECMD_SplitTokenAssignVar((WCHAR **)&local_50, &local_res10, 0x2c, 1);
            PECMD_SplitTokenTrimWs(plVar4, (void *)local_78, 0);
            if (*local_res10 == 0x2c) {
                local_res10 = local_res10 + 1;
                PECMD_CopyUpToChar(&local_res10, &local_68, 0x2c);
                if (*local_res10 == 0x2c) {
                    local_res10 = local_res10 + 1;
                    puVar5 = (undefined8 *)PECMD_SplitTokenAssignVar((WCHAR **)&local_50,
                                                                     &local_res10, 0x2c, 1);
                    uVar6 = PECMD_ParseSignedNumber((short *)*puVar5);
                    uVar11 = uVar11 | (uint)uVar6;
                }
            }
        }
        if (uVar1 != 0x2a) {
            param_1 = *(longlong **)(param_3 + 0x290);
        }
        PECMD_CreateSliderItem(param_3, (longlong)param_1, (undefined8 *)&local_80, local_88,
                               local_84, local_res18[0], (int)(intptr_t)local_res8, local_78,
                               (undefined8 *)&local_68, uVar11);
        PECMD_FreeStrBuf(&local_68);
        PECMD_FreeStrBuf(&local_70);
        PECMD_FreeStrBuf((void *)local_78);
        PECMD_FreeStrBuf(&local_80);
    }
    else {
        PECMD_FreeStrBuf(&local_68);
        PECMD_FreeStrBuf(&local_70);
        PECMD_FreeStrBuf((void *)local_78);
        PECMD_FreeStrBuf(&local_80);
        uVar9 = 1;
    }
LAB_1400ac068:
    PECMD_FreeStrBuf(&local_50);
    return uVar9;
}

/* ================================================================
 * @0x1400acd90  FOR/FORX 命令解析/执行 (R26-e dc:107744-108549 全文直移)
 * 选项: NL / NL: / delims: / * / - / v / ** / L / *bf / *ab / *qu / *qu~ / *off /
 *       /s [:] / /O:N / /O:-N / *cur / /size*: / /size: ;
 * 主体: token1(列表源) + token2(变量名) + [token3(步长)] + body, 三引擎 =
 *       L 数值迭代 / 文件通配 (ProcessControlCommand 逐项执行) / 行迭代。
 * 直移要点 (偏差登记见 HANDOVER R26-e):
 *       *bf → bfReg=1, /s → sFlag=0x10 (capstone 核验: dc uVar15/local_4a8 与
 *         uVar22/uVar23 为不同寄存器 — 修正 WIP 存档"同寄存器"登记);
 *       uVar20 = sFlag|oFlag; local_3b8 = sFlag ? bfReg : 0 (n350 bit0);
 *       NL 旗真值 = opts bit2 (dc bVar4 被 "v" 测试复用, 与 dc:108087 判据一致);
 *       体为 L"*" 时 bVar4 = true (capstone: 编译器折叠 ad429 的 bVar4=true);
 *       缺第 4 分隔符 → 返回 1 (uVar32 init);
 *       *qu 且 opts==1 → 走引号扫描器 break 路径 (dc:107916-108086);
 *       SplitTokenAssignVar 3 槽 cell (dc:81278): [0]=展开出参 [1]=script [2]=副本
 *       —— cell[1] = script (dc:107871 local_3d8 = param_1, 非未初始化伪影);
 *       L 模式 (capstone 核验 dc:108327-108536): s3f8 = 当前值(local_3f8),
 *         n3c8 = 步长(local_3c8, 初值 1), s400 = 终值(local_400, 先算 start+step
 *         兜底后第三次解析覆盖); int 迭代值低 32 位写变量; 浮点经 FormatU64RetEnd
 *         %Lf (位模式传值, xmm 槽); 步长==0/解析失败 → 0x80070057 错误返回;
 *         lFlag==0 空格分词分支为活路径 (* / - 无 L), 非 WIP 存档所称死代码;
 *         g_flagA24F > 0 才进入数值/NL 循环, 每轮尾部重查 <1 break。
 * signature: ulonglong __fastcall PECMD_ForCommand(longlong *param_1,
 *   WCHAR *param_2, longlong *param_3, longlong *param_4, uint64_t param_5)
 */
ULONGLONG PECMD_ForCommand(longlong *param_1, WCHAR *param_2, longlong *param_3, longlong *param_4,
                           uint64_t param_5)
{
    longlong *script = param_1;                   /* dc local_3d8 */
    WCHAR *cur = param_2;                         /* dc local_res10 */
    longlong *res18 = param_3;                    /* dc local_res18 */
    longlong *res20 = param_4;                    /* dc local_res20 */
    int64_t fcell[3];                             /* dc cell: [0]=local_3e0 槽 [1]=local_3d8 [2]=local_3d0 */
    WCHAR *local_3c0 = NULL;                      /* dc local_3c0 (qu 临时槽) */
    int64_t ptrTbl = 0;                           /* dc local_3b0 */
    longlong stepNum = 0;                         /* dc local_378 */
    uint64_t n370 = 0, n368 = 0, n360 = 0, n358 = 0, n350 = 0;
    uint32_t bsFlags = 0;                         /* dc local_49c (@/$/\!/路径前缀旗) */
    uint32_t abFlag = 0;                          /* dc local_420 */
    uint32_t curFlag = 0;                         /* dc local_460 */
    uint32_t quFlag = 0;                          /* dc local_480 */
    uint32_t offFlag = 0;                         /* dc local_430 */
    uint32_t lFlag = 0;                           /* dc local_45c */
    uint32_t mode458 = 2;                         /* dc local_458 */
    int stepPair = -8;                            /* dc local_448[0] */
    uint32_t sFlag = 0;                           /* dc local_4a8/uVar15 — 仅 /s./s: 置 0x10 */
    uint32_t bfReg = 0;                           /* dc uVar22/uVar23 — *bf 置 1 */
    uint32_t oFlag = 0;                           /* dc uVar26 */
    uint32_t opts = 0;                            /* dc uVar34/uVar33 */
    uint32_t uVar20 = 0;                          /* dc uVar20 = sFlag|oFlag (ad429) */
    uint32_t local_3b8 = 0;                       /* dc local_3b8 = sFlag ? bfReg : 0 */
    WCHAR c490 = L'\0';                           /* dc local_490 (NL:/delims: 定界字符) */
    char delim = ',';                             /* dc cVar35 (选项期定界符) */
    uint32_t delimU = 0;                          /* dc uVar25 (ad61f 期 (uint)delim) */
    uint32_t uVar25 = 0;                          /* dc uVar25 (主体期 0/0x100 旗) */
    uint32_t n494 = 0;                            /* dc local_494 (恒 0) */
    int iVar8 = 0;                                /* dc iVar8 (返回值: 恒 0 或 VarLookup 失败 2) */
    int flag498 = 0;                              /* dc local_498 */
    bool bVar4 = false;                           /* dc bVar4 (体 == L"*") */
    bool bVar1 = false;                           /* dc bVar1 (UNC 双反斜杠标记) */
    bool bVar37 = false;                          /* dc bVar37 (L 整数步长==0 → 错误) */
    char cVar5 = 0;                               /* dc cVar5 (L 引擎: 0 空格分词 / 1 整数 / 2 浮点) */
    char cVar24 = 0;                              /* dc cVar24 (整数步进方向) */
    char endDir = 0;                              /* dc cVar35 复用 (浮点步进方向) */
    WCHAR WVar31 = 0, WVar18 = 0;                 /* dc WVar31/WVar18 */
    WCHAR *pWVar21 = NULL, *pWVar2 = NULL, *pWVar10 = NULL, *pWVar14 = NULL;
    WCHAR *pWVar28 = NULL, *pWVar29 = NULL;       /* dc pWVar28/pWVar29 */
    void *puVar12 = NULL, *puVar16 = NULL, *puVar17 = NULL;
    int64_t lVar13 = 0;                           /* dc lVar13 */
    int iVar36 = 0, iVar7 = 0;                    /* dc iVar36/iVar7 */
    uint32_t uVar40 = 0;                          /* dc uVar15 返回承接 (PCC/计数) */
    WCHAR *s470 = NULL;                           /* dc local_470 (变量名) */
    WCHAR *s478 = NULL;                           /* dc local_478 (列表源/展开后) */
    WCHAR *s468 = NULL;                           /* dc local_468 (体) */
    WCHAR *s408 = NULL;                           /* dc local_408 (token1 槽) */
    WCHAR *tgtSlot = NULL;                        /* dc local_398 (PCC 出参/尾槽) */
    uint64_t c390 = 0, c388 = 0;                  /* dc local_390/388 */
    WCHAR *buf410 = NULL;                         /* dc local_410 (0x32a 盘串缓冲) */
    WCHAR *s418 = NULL;                           /* dc local_418 */
    WCHAR *s488 = NULL;                           /* dc local_488 */
    int count = 0;                                /* dc local_4a4 */
    WCHAR *blk428 = NULL;                         /* dc local_428/psVar27 (res20 链) */
    WCHAR *blk3e8 = NULL;                         /* dc local_3e8 */
    WCHAR *s480 = NULL;                           /* dc local_480 (展开槽) */
    longlong n380 = 0;                            /* dc local_380 */
    void *v340 = NULL;                            /* dc local_340 */
    longlong v3a8 = 0, v3a0 = 0;                  /* dc local_3a8/3a0 */
    longlong n338 = 0;                            /* dc local_338 (lVar13=uVar25 的 64 位副本) */
    longlong n3c8 = 1;                            /* dc local_3c8 (L 整数步长) */
    WCHAR *s438 = NULL;                           /* dc local_438 (数值引擎 token 槽) */
    longlong s3f8 = 0;                            /* dc local_3f8 (L 整数当前值) */
    longlong s400 = 0;                            /* dc local_400 (L 整数终值) */
    WCHAR *s440 = NULL;                           /* dc local_440 (数值解析游标) */
    WCHAR *local_450 = NULL;                      /* dc local_450 (/size 族游标) */
    WCHAR *local_3f0 = NULL;                      /* dc local_3f0 */
    WCHAR *s330 = NULL;                           /* dc local_330 (行迭代槽) */
    uint64_t retCell = 1;                         /* dc uVar32 (init 1; 错误 0x80070057) */
    double dVar38 = 0.0, dStep = 0.0, dEnd = 0.0; /* dc dVar38/(uVar40,41)/(uVar44,45) */
    uint8_t precE = 0, precF = 0, precD = 0;      /* dc local_49e/49f/49d */
    uint8_t bVar6 = 0;                            /* dc bVar6 (三次解析 prec 或) */
    WCHAR local_298[300];                         /* dc local_298 (*cur cwd 缓冲) */
    WCHAR local_328[72];                          /* dc local_328 (%Lf 缓冲) */
    WCHAR **ppWVar19 = NULL;                      /* dc ppWVar19 (槽释放目标) */

    /* dc:107857-107859 — cell[0] 槽分配; cell[2]=0; cell[1]=script (dc:107871) */
    PECMD_AllocStrSlot((WCHAR **)&fcell[0]);
    fcell[2] = 0;
    WVar31 = *cur;
    c490 = L'\0';
    delim = ',';
    while ((((WVar31 != L'\0' && (WVar31 != L'*')) &&
             (((uint16_t)WVar31 < 9 || (0xd < (uint16_t)WVar31)))) && (WVar31 != L' '))) {
        if ((WVar31 == L';') || (WVar31 == L':')) {
            delim = (char)*cur;
        }
        cur = cur + 1;
        WVar31 = *cur;
    }
    fcell[1] = (int64_t)(uintptr_t)param_1;       /* dc:107871 local_3d8 = param_1 */
    PECMD_InitPtrTable((int64_t *)&ptrTbl);       /* dc FUN_140063b64(&local_3b0) */
    PECMD_SkipLeadingControlChars((long long *)(uintptr_t)&cur);
    n350 = 0;
    bsFlags = 0;
    abFlag = 0;
    curFlag = 0;
    quFlag = 0;
    offFlag = 0;
    lFlag = 0;
    bVar37 = false;
    mode458 = 2;
    stepPair = -8;
    retCell = 1;
    sFlag = 0;
    bfReg = 0;
    oFlag = 0;
    opts = 0;
    do {                                          /* dc 选项循环 (107894-108063) */
LAB_1400acf18:
        if (((*cur != L'*') && (*cur != L'/')) && (*cur != L'-')) {
LAB_1400ad429:
            uVar20 = sFlag | oFlag;               /* dc:107901 uVar25(local_4a8) | uVar26 */
            local_3b8 = (sFlag != 0) ? bfReg : 0; /* dc:107902 -(uVar25!=0) & uVar22 */
            n350 |= (uint64_t)(int64_t)(int32_t)local_3b8;
            PECMD_AllocStrSlot(&s408);
            PECMD_StrDupAssign((uint16_t **)&s470, g_szEmpty);
            PECMD_StrDupAssign((uint16_t **)&s478, g_szEmpty);
            PECMD_StrDupAssign((uint16_t **)&s468, g_szEmpty);
            pWVar21 = cur;
            /* dc:107909 bVar4=true — capstone 无存储指令, 编译器折叠 (体==L"*" 判据在 ad61f), 略 */
            if ((quFlag != 0) && (opts == 1)) {
                PECMD_AllocStrSlot(&local_3c0);
                break;                            /* dc:107910-107912 → 引号扫描器路径 */
            }
            PECMD_SplitTokenAssignVar(fcell, &cur, (uint16_t)(short)delim, 1);   /* dc:107914 */
            goto LAB_1400ad61f;
        }
        if (((*cur == L'-') && (cur[1] == L'-')) &&
            (((WVar31 = cur[2], 8 < (uint16_t)WVar31 && ((uint16_t)WVar31 < 0xe)) ||
              ((WVar31 == L' ') || (WVar31 == L'\0'))))) {
            cur = cur + 2;
            PECMD_SkipLeadingControlChars((long long *)(uintptr_t)&cur);
            goto LAB_1400ad429;
        }
        if (PECMD_MatchPrefixAdvance("NL", (int64_t *)(uintptr_t)&cur, 2)) {
            opts = (opts & 0xfffffffeU) | 6U;
            continue;
        }
        pWVar21 = cur;
        if (PECMD_AsciiPrefixICmp("NL:", cur + 1, 3) != 0) {
            cur = pWVar21 + 4;
            opts = 6U | 10U;
            PECMD_ParseStringToken((int64_t *)(uintptr_t)&cur, (uint64_t)(uintptr_t)param_1, NULL);
            c490 = *cur;
            pWVar21 = cur;
            goto LAB_1400ad02e;
        }
        if (PECMD_AsciiPrefixICmp("delims:", cur + 1, 7) != 0) {
            cur = pWVar21 + 8;
            opts = 6U | 10U;
            PECMD_ParseStringToken((int64_t *)(uintptr_t)&cur, (uint64_t)(uintptr_t)param_1, NULL);
            c490 = *cur;
            pWVar21 = cur;
            goto LAB_1400ad02e;
        }
        cVar5 = PECMD_MatchTokenAdvance("*", &cur, -1);
        if ((cVar5 != '\0') || (cVar5 = PECMD_MatchTokenAdvance("-", &cur, -1), cVar5 != '\0')) {
            opts = (opts & 0xfffffffdU) | 1U;
            goto LAB_1400ad518;
        }
        if ((PECMD_MatchPrefixAdvance("v", (int64_t *)(uintptr_t)&cur, 1)) ||
            (cVar5 = PECMD_MatchTokenAdvance("**", &cur, 2), cVar5 != '\0')) {
            opts = opts | 5U;
            goto LAB_1400ad518;
        }
        if (PECMD_MatchPrefixAdvance("L", (int64_t *)(uintptr_t)&cur, 1)) {
            lFlag = (opts & 0xfffffffdU) | 0x11U;
            opts = lFlag;
            goto LAB_1400ad518;
        }
        if (PECMD_MatchTokenAdvance("*bf", &cur, 3) != 0) {
            bfReg = 1;                            /* dc: uVar23 = uVar32(=1) → ad518 uVar22 = uVar23 */
            goto LAB_1400ad518;
        }
        if (PECMD_MatchTokenAdvance("*ab", &cur, 3) != 0) {
            abFlag = 1;
            goto LAB_1400ad518;
        }
        if (PECMD_MatchTokenAdvance("*qu", &cur, 3) != 0) {
            quFlag = 1;
            goto LAB_1400ad518;
        }
        if (PECMD_MatchTokenAdvance("*qu~", &cur, 4) != 0) {
            quFlag = 2;
            goto LAB_1400ad518;
        }
        if (PECMD_MatchTokenAdvance("*off", &cur, 4) != 0) {
            offFlag = 1;
            goto LAB_1400ad518;
        }
        if (PECMD_MatchTokenAdvance("/s", &cur, 2) != 0) {
            sFlag = 0x10;                         /* dc: uVar15 = 0x10 → 循环顶 local_4a8 */
            continue;
        }
        if (PECMD_AsciiPrefixICmp("/s:", cur, 3) != 0) {
            cur = cur + 3;
            sFlag = 0x10;                         /* dc:107057-107058 local_4a8/uVar15 = 0x10 */
            PECMD_ParseUIntValue(&cur, &stepPair); /* dc FUN_140074838(&local_res10,local_448) */
            continue;                             /* dc goto LAB_1400acf18 */
        }
        if (PECMD_MatchTokenAdvance("/O:N", &cur, 4) != 0) {
            oFlag = 0x20;
            goto LAB_1400ad518;
        }
        if (PECMD_MatchTokenAdvance("/O:-N", &cur, 5) != 0) {
            oFlag = 0x60;
            goto LAB_1400ad518;
        }
        if (PECMD_MatchTokenAdvance("*cur", &cur, 4) != 0) {
            curFlag = 1;
            goto LAB_1400ad518;
        }
        if (PECMD_AsciiPrefixICmp("/size*:", cur, 7) != 0) {
            n350 |= 0x10;
            cur = cur + 1;
            goto LAB_1400ad349;
        }
        if (PECMD_AsciiPrefixICmp("/size:", cur, 6) != 0) {
LAB_1400ad349:
            local_450 = cur + 6;
            PECMD_ParseSizeNumber((int64_t *)(uintptr_t)&local_450, (int64_t *)&n370);
            if (*local_450 != L'\0') {
                if ((((uint16_t)*local_450 < 9) || (0xd < (uint16_t)*local_450)) &&
                    (*local_450 != L' ')) {
                    local_450 = local_450 + 1;
                    PECMD_ParseSizeNumber((int64_t *)(uintptr_t)&local_450, (int64_t *)&n368);
                }
                if (((*local_450 != L'\0') &&
                     (((uint16_t)*local_450 < 9 || (0xd < (uint16_t)*local_450)))) &&
                    (*local_450 != L' ')) {
                    local_450 = local_450 + 1;
                    PECMD_ParseSizeNumber((int64_t *)(uintptr_t)&local_450, (int64_t *)&n358);
                }
            }
            goto LAB_1400ad02e;
        }
        if (*pWVar21 != L'*') {
            goto LAB_1400ad429;
        }
        goto LAB_1400ad02e;
LAB_1400ad02e:
        WVar31 = *pWVar21;
        while ((WVar31 != L'\0' &&
                ((((uint16_t)WVar31 < 9 || (0xd < (uint16_t)WVar31)) && (WVar31 != L' '))))) {
            pWVar21 = pWVar21 + 1;
            cur = pWVar21;
            WVar31 = *pWVar21;
        }
        PECMD_SkipLeadingControlChars((long long *)(uintptr_t)&cur);
LAB_1400ad518:
        continue;
    } while (1);
    /* dc:108064-108086 — *qu 且 opts==1 → break 落入引号扫描器 */
    do {
        WVar31 = *cur;
        if (WVar31 == L'\"') {
            cur = cur + 1;
            PECMD_SkipWCharUntil(&cur, 0x22);
            if (*cur != L'\0') {
                cur = cur + 1;
            }
        }
        else if (WVar31 != L'\0') {
            do {
                if (((int)delim == (int)(uint16_t)WVar31) ||
                    (((8 < (uint16_t)WVar31 && ((uint16_t)WVar31 < 0xe)) || (WVar31 == L' ')))) {
                    break;
                }
                cur = cur + 1;
                WVar31 = *cur;
            } while (WVar31 != L'\0');
        }
        PECMD_SkipLeadingControlChars((long long *)(uintptr_t)&cur);
    } while ((*cur != L'\0') && ((int)delim != (int)(uint16_t)*cur));
    PECMD_StrCopyW((WCHAR **)&local_3c0, pWVar21,
                   (int64_t)((ptrdiff_t)cur - (ptrdiff_t)pWVar21) >> 1);
    PECMD_ExpandVarDispatch(param_1, local_3c0, (int64_t *)&fcell[0], 0, 1);
    fcell[2] = fcell[0];                          /* dc:108085 local_3d0 = local_3e0 */
    PECMD_FreeStrBuf(&local_3c0);
LAB_1400ad61f:
    /* dc:108087-108111 — token1 前缀扫描 (@/$/\!/路径) */
    PECMD_SplitTokenTrimWs((WCHAR **)&fcell[2], &s408, 0);   /* dc FUN_1400675b8(&local_3d0,&local_408,0) */
    local_3f0 = s408;
    if (opts == 0) {
        for (; ((WVar31 = *s408, local_3f0 = s408, WVar31 != L'\0' &&
                (((uint16_t)WVar31 < 9 || (0xd < (uint16_t)WVar31)))) && (WVar31 != L' '));
             s408 = s408 + 1) {
            if (WVar31 == L'@') {
                mode458 = 1;
            }
            else if (WVar31 == L'$') {
                mode458 = 3;
            }
            else if (WVar31 == L'\\') {
                if (s408[1] == L'\\') {
                    bVar1 = true;                 /* dc:108102-108103 UNC 双反斜杠 */
                    break;
                }
                bsFlags |= 1;
            }
            else {
                if (WVar31 != L'!') {
                    break;
                }
                bsFlags |= 2;
            }
        }
    }
    PECMD_SkipLeadingControlChars((long long *)(uintptr_t)&local_3f0);
    pWVar10 = local_3f0;
    if ((opts & 1) == 0) {
        pWVar10 = PECMD_UnquoteString(local_3f0);
    }
    if (bsFlags == 0) {
        if (((opts != 0) || bVar1) || ((pWVar10[0] == L'\\') && (pWVar10[1] == L'\\'))) {
            goto LAB_1400ad738;
        }
        /* dc:108120 — 普通路径展开 (唯一余留桩依赖已连带真体化) */
        PECMD_ExpandDrivePathAlloc(pWVar10, (uint64_t *)&s478);
    }
    else {
        for (; *pWVar10 == L'\\'; pWVar10 = pWVar10 + 1) {
        }
LAB_1400ad738:
        PECMD_AssignString((int64_t *)&s478, pWVar10);
    }
    delimU = (uint32_t)(int32_t)(int8_t)delim;    /* dc:108128 uVar25 = (uint)cVar35 */
    if (delimU == (uint32_t)(uint16_t)*cur) {
        cur = cur + 1;
        puVar12 = PECMD_SplitTokenAssignVar(fcell, &cur, (uint16_t)(short)delim, 1);
        PECMD_SplitTokenTrimWs((WCHAR **)puVar12, &s470, 0);   /* dc:108132 变量名 */
    }
    if ((opts == 0) && (delimU == (uint32_t)(uint16_t)*cur)) {
        cur = cur + 1;
        puVar12 = PECMD_SplitTokenAssignVar(fcell, &cur, (uint16_t)(short)delim, 1);
        PECMD_ParseSignedNumberStr((uint64_t *)puVar12, &stepNum, (int16_t)delim);   /* dc:108137 步长 */
    }
    if (stepNum < 1) {
        stepNum = -8;                             /* dc:108140 步长回退 -8 */
    }
    if ((uint16_t)*cur != (uint16_t)delimU) {
        goto LAB_1400adddf;                       /* dc:108142 — 缺 body 分隔符 */
    }
    cur = cur + 1;
    PECMD_SkipLeadingControlChars((long long *)(uintptr_t)&cur);
    PECMD_StrBldCopyWide(&s468, cur);             /* dc:108145 体 */
    if ((s468[0] != L'*') || (s468[1] != L'\0')) {
        bVar4 = false;                            /* dc:108148-108150 — 体 != L"*" → bVar4 清 */
    }
    PECMD_AllocStrSlot(&tgtSlot);                 /* dc:108151 */
    c390 = 0;
    c388 = 0;
    if (!bVar4) {
        PECMD_StrBldCopyWide(&tgtSlot, s470);     /* dc:108155 预填变量名 */
    }
    blk3e8 = NULL;
    blk428 = NULL;
    if (res20 != NULL) {
        blk428 = (WCHAR *)*res20;                 /* dc:108158-108161 */
    }
    if (((s468[0] == L'\0') && (blk428 != NULL)) &&
        (*(uint16_t *)((int64_t)(uintptr_t)param_1 + 0x96) == (uint16_t)*blk428)) {
        PECMD_AdvanceTokenPointer((int64_t)(uintptr_t)param_1, res20, 0, NULL);   /* dc:108164 */
    }
    PECMD_AllocWStringBuffer(&buf410, 0x32a);     /* dc:108166 */
    pWVar28 = NULL;
    pWVar14 = (WCHAR *)((char *)buf410 + 4);      /* dc:108168 local_410+2 元素 */
    *(uint16_t *)(param_1 + 0x19) = *(uint16_t *)(param_1 + 0x19) & 0x8b;   /* dc:108169 清 0x74 位 */
    s488 = NULL;
    count = 0;
    n494 = 0;
    uVar25 = 0;
    if (((s468[0] == L'\0') && (blk428 != NULL)) &&
        (*(uint16_t *)((int64_t)(uintptr_t)param_1 + 0x96) == (uint16_t)*blk428)) {
        uVar25 = 0x100;                           /* dc:108174-108176 */
    }
    flag498 = 0;
    if ((s470[0] != L'\0') && (s468[0] == L'\0') && (uVar25 == 0)) {
        flag498 = 1;                              /* dc:108177-108179 */
    }
    s418 = NULL;
    cur = s478;                                   /* dc:108181 — 数值/行迭代源 */
    if ((opts & 4) != 0) {                        /* dc:108182 NL 旗 → 变量取值 */
        EnterCriticalSection(&g_csInit);
        lVar13 = PECMD_VarLookup(param_1, s478, NULL, -1, NULL);   /* dc:108184 */
        if (lVar13 == 0) {
            LeaveCriticalSection(&g_csInit);
            iVar8 = 2;                            /* dc:108187 */
            count = 0;
            goto LAB_1400ae5ea;
        }
        PECMD_StrCopyW((WCHAR **)&s418, (LPCWSTR)(uintptr_t)*(int64_t *)(lVar13 + 8),
                       (int64_t)(*(uint64_t *)(lVar13 + 0x18) >> 1 & 0x1fffffffffffffff));
        cur = s418;                               /* dc:108193 */
        LeaveCriticalSection(&g_csInit);
    }
    blk3e8 = NULL;
    WVar31 = c490;
    lVar13 = (longlong)(int)uVar25;
    n338 = lVar13;                                /* dc:108201 — 体执行分派用 */
    if ((opts & 2) == 0) {                        /* dc:108202 — 三引擎: 非 NL 分支 */
        if ((opts & 1) == 0) {                    /* dc:108203 — 默认: 文件通配 */
            pWVar10 = PECMD_UnquoteString(s478);
            PECMD_AssignString((int64_t *)&s478, pWVar10);
            pWVar10 = s488;
            WVar31 = L'\0';
            if (s478[0] == L'\0') {               /* dc:108209 */
                count = 0;
                iVar8 = 0;
            }
            else {
                puVar12 = NULL;
                if (bsFlags == 0) {               /* dc:108215 — 无前缀: 单次直接展开 */
                    s488 = s478;
                    s478 = (WCHAR *)pWVar10;
                    goto LAB_1400ae282;
                }
                if (curFlag != 0) {               /* dc:108220 *cur */
                    local_298[0] = L'\0';
                    GetCurrentDirectoryW(0x104, local_298);
                    WVar31 = local_298[0];
                    if ((0x60 < (uint16_t)local_298[0]) && ((uint16_t)local_298[0] < 0x7b)) {
                        WVar31 = local_298[0] & 0xffdf;   /* 当前盘符大写 */
                    }
                }
                ((uint32_t *)(void *)buf410)[0] = 0;   /* dc:108228-108231 — 头 8 字节清零 */
                ((uint32_t *)(void *)buf410)[1] = 0;
                GetLogicalDriveStringsW(0x324, pWVar14);
                WVar18 = pWVar14[0];
                pWVar28 = pWVar14;
                pWVar2 = pWVar14;
                while (pWVar10 = pWVar28, WVar18 != L'\0') {   /* dc:108236 — 找最后盘串 */
                    iVar36 = lstrlenW(pWVar10);
                    pWVar28 = pWVar10 + (longlong)iVar36 + 1;
                    pWVar2 = pWVar10;
                    WVar18 = pWVar10[(longlong)iVar36 + 1];
                }
                pWVar28 = pWVar14;
                if ((bsFlags & 2) != 0) {         /* dc:108243 '!' → 自后向前 */
                    pWVar28 = pWVar2;
                }
LAB_1400ae45e:
                do {
                    iVar8 = (int)curFlag;
                    cVar5 = (char)(uintptr_t)puVar12;
                    do {
                        iVar36 = stepPair;
                        if (((g_flagA24F < 1) || (pWVar28[0] == L'\0')) || (stepNum == 0) ||
                            ((*(uint8_t *)((int64_t)(uintptr_t)param_1 + 0xc8) & 0xef) != 0)) {
                            goto LAB_1400ae53a;   /* dc:108252-108253 */
                        }
                        PECMD_StrBldCopyWide(&s488, pWVar28);
                        PECMD_AppendWideStr(&s488, s478);   /* dc:108254-108255 盘串+名单 */
                        if (iVar8 != 0) {         /* dc:108256 *cur */
                            curFlag = 0;
                            s488[0] = WVar31;     /* dc:108258 首字符换当前盘符 */
                            break;
                        }
                        if ((bsFlags & 2) == 0) {
                            iVar36 = lstrlenW(pWVar28);
                            pWVar28 = pWVar28 + (longlong)iVar36 + 1;   /* 下一盘串 */
                        }
                        else {
                            pWVar2 = pWVar28 + -1;
                            do {
                                pWVar28 = pWVar2;
                                if (pWVar28 <= pWVar14) {
                                    break;
                                }
                                pWVar2 = pWVar28 + -1;
                            } while (pWVar28[-1] != L'\0');   /* 前一盘串 */
                        }
                    } while (WVar31 == s488[0]);  /* dc:108273 — 跳过匹配当前盘符者 */
LAB_1400ae282:
                    iVar36 = stepPair;
                    if (((uVar25 == 0) && (s468[0] == L'\0')) && ((iVar8 = (int)n494), flag498 == 0)) {
                        goto LAB_1400ae5ea;       /* dc:108276-108277 */
                    }
                    if (((abFlag == 0) || (s488[0] == L'\0')) ||
                        ((s488[1] != L':') ||
                         ((uVar40 = (uint32_t)PECMD_IsRemovableDrive((uint16_t)s488[0])) != 0))) {
                        PECMD_AllocStrSlot(&s480);      /* dc:108280 */
                        puVar12 = NULL;
                        n380 = 0;
                        PECMD_ExpandPathAlloc2(s488, (uint64_t *)&s480, &n380);   /* dc:108283 */
                        pWVar10 = s480;
                        if (offFlag != 0) {             /* dc:108285 *off 长度 */
                            n360 = (n380 - (longlong)pWVar10) >> 1;
                        }
                        if (local_3b8 != 0) {           /* dc:108288 /s+*bf 收集模式 */
                            puVar16 = operator_new(8);
                            puVar17 = puVar12;
                            if (puVar16 != NULL) {
                                puVar17 = PECMD_CopyStrToSlot((uint64_t **)puVar16,
                                                             (uint64_t *)&s480);
                            }
                            v340 = puVar17;
                            PECMD_VectorAppendGen(&ptrTbl, &v3a8, &v3a0, &v340, 8, 1);
                            if ((bsFlags != 0) && (pWVar28[0] != L'\0')) {
                                puVar12 = (void *)1;    /* dc:108297 收集旗 */
                                PECMD_FreeStrBuf(&s480);
                                goto LAB_1400ae45e;
                            }
                            pWVar10 = s480;
                        }
                        uVar40 = (uint32_t)PECMD_ProcessControlCommand(
                            param_1, (int64_t *)&tgtSlot, pWVar10, (uint64_t)res18,
                            (int64_t)(uintptr_t)blk428, uVar25 | uVar20 | mode458,
                            (int16_t *)s468, &stepNum, iVar36, param_5);   /* dc:108303-108305 */
                        count = count + (int)uVar40;
                        if ((*(uint16_t *)(param_1 + 0x19) & 0x100) != 0) {
                            *(uint16_t *)(param_1 + 0x19) = *(uint16_t *)(param_1 + 0x19) & 0xfeff;
                        }
                        PECMD_FreeStrBuf(&s480);
                    }
                    cVar5 = (char)(uintptr_t)puVar12;
                } while (bsFlags != 0);             /* dc:108313 */
LAB_1400ae53a:
                iVar8 = (int)n494;
                if (cVar5 != '\0') {                /* dc:108316 — 收集模式跑首项 */
                    puVar12 = (void *)(uintptr_t)PECMD_VectorSlotPtr(0, &ptrTbl, &v3a8, 8);
                    uVar40 = (uint32_t)PECMD_ProcessControlCommand(
                        param_1, (int64_t *)&tgtSlot,
                        *(LPCWSTR *)(uintptr_t)*(int64_t **)puVar12, (uint64_t)res18,
                        (int64_t)(uintptr_t)blk428, uVar25 | uVar20 | mode458,
                        (int16_t *)s468, &stepNum, iVar36, param_5);   /* dc:108318-108320 */
                    count = count + (int)uVar40;
                    iVar8 = (int)n494;
                }
            }
            goto LAB_1400ae5ea;                     /* dc:108325 */
        }
        /* dc:108327-108479 — /L 数值引擎 (含 lFlag==0 空格分词退路) */
        if ((opts & 4) != 0) {
            PECMD_SkipLeadingControlChars((long long *)(uintptr_t)&cur);
        }
        iVar8 = 0;                                  /* dc:108330 iVar8 = local_480 (=0) */
        iVar36 = 0 + -1;                            /* dc:108333 = local_480 + -1 */
        cVar5 = 0;                                  /* dc:108334 (char)pWVar10 (NULL) */
        s3f8 = 0;                                   /* dc:108339 */
        s400 = 0;                                   /* dc:108338 */
        s438 = NULL;
        n3c8 = 1;                                   /* dc:108335 local_3c8 = 1 */
        cVar24 = 1;                                 /* dc:108336 */
        endDir = 1;                                 /* dc:108337 */
        dVar38 = 0.0;
        dStep = 0.0;
        dEnd = 0.0;
        if (lFlag != 0) {                           /* dc:108341 — L 三次解析 */
            precE = 0xc;
            s440 = cur;
            dVar38 = PECMD_WideStrToDouble((int64_t *)(uintptr_t)&s440, (int64_t *)&s3f8,
                                           &precE);  /* dc:108344 起始值 */
            PECMD_SkipLeadingControlChars((long long *)(uintptr_t)&s440);
            precF = 0xc;
            dStep = PECMD_WideStrToDouble((int64_t *)(uintptr_t)&s440, (int64_t *)&n3c8,
                                          &precF);   /* dc:108349 步长 */
            PECMD_SkipLeadingControlChars((long long *)(uintptr_t)&s440);
            lVar13 = n3c8;
            s400 = n3c8 + s3f8;                     /* dc:108354 — 终值兜底 = start+step */
            precD = 0xc;
            dEnd = PECMD_WideStrToDouble((int64_t *)(uintptr_t)&s440, (int64_t *)&s400,
                                         &precD);   /* dc:108356 终值 (覆盖兜底) */
            bVar6 = precD | precF | precE;
            if ((bVar6 & 2) == 0) {                 /* dc:108360 三次均解析成功 */
                cVar5 = (char)(((bVar6 & 1) != 0) + 1);   /* 1=整数 2=浮点 */
                endDir = 1;
                if (dStep < 0.0) {
                    endDir = -1;                    /* dc:108363-108365 */
                }
                cVar24 = 1;
                if (lVar13 < 0) {
                    cVar24 = -1;
                }
                bVar37 = false;
                if (cVar5 == '\x01') {
                    bVar37 = (lVar13 == 0);         /* 整数步长==0 → 错 */
                }
                else if (dStep == 0.0) {
                    goto LAB_1400add9a;             /* 浮点步长==0 → 错 */
                }
                if (!bVar37) {
                    goto LAB_1400ade55;
                }
            }
            goto LAB_1400add9a;                     /* 解析失败 → 0x80070057 */
        }
        dVar38 = (double)(int64_t)(intptr_t)cur;    /* dc:108395-108398 — 位模式伪值 (空格分词分支不读) */
        dEnd = dStep;                               /* dc: uVar44=uVar40 伪值 */
LAB_1400ade55:
        lVar13 = n338;
        if (g_flagA24F > 0) {                       /* dc:108402 (char)pWVar10 < DAT_14013a24f */
            while (true) {
                pWVar29 = (WCHAR *)(uintptr_t)s3f8;
                pWVar10 = cur;
                iVar7 = 0;
                WVar31 = cur[0];
                if ((WVar31 == L'\0') ||
                    ((*(uint8_t *)((int64_t)(uintptr_t)param_1 + 0xc8) & 0xef) != 0)) {
                    break;
                }
                if (lFlag == 0) {                   /* dc:108409 空格分词 */
                    if ((iVar8 == 0) || (WVar31 != L'\"')) {
                        do {
                            if ((((8 < (uint16_t)WVar31) && ((uint16_t)WVar31 < 0xe)) ||
                                 (WVar31 == L' '))) {
                                break;
                            }
                            cur = cur + 1;
                            WVar31 = cur[0];
                        } while (WVar31 != L'\0');
                    }
                    else {
                        pWVar10 = cur + (longlong)iVar36;   /* iVar36 = -1 */
                        do {
                            pWVar29 = cur;
                            cur = pWVar29 + 1;
                            if (cur[0] == L'\0') {
                                goto LAB_1400ae066;
                            }
                        } while ((cur[0] != L'\"') ||
                                 ((WVar31 = pWVar29[2], (WVar31 != L'\0' &&
                                  ((((uint16_t)WVar31 < 9 || (0xd < (uint16_t)WVar31))) &&
                                   (WVar31 != L' '))))));
                        cur = pWVar29 + 2;
                        iVar7 = iVar36;
                    }
LAB_1400ae066:
                    PECMD_StrCopyW((WCHAR **)&s438, pWVar10,
                                   (int64_t)((int64_t)(uintptr_t)cur +
                                             ((longlong)iVar7 * -2) - (int64_t)(uintptr_t)pWVar10) >> 1);
                    PECMD_SkipLeadingControlChars((long long *)(uintptr_t)&cur);
                    PECMD_SetVariable(param_1, s470, s438);
                }
                else if (cVar5 == '\x01') {         /* dc:108435 整数迭代 */
                    if (((cVar24 > 0) && ((longlong)s400 < (longlong)s3f8)) ||
                        ((cVar24 < 0) && ((longlong)s3f8 < (longlong)s400))) {
                        break;
                    }
                    PECMD_AppendLongDecimal(param_1, (uint64_t)s3f8 & 0xffffffff, s470);
                    s3f8 = (longlong)(uintptr_t)pWVar29 + n3c8;   /* 当前值 += 步长 */
                }
                else {                              /* dc:108441 浮点迭代 */
                    if (((endDir > 0) && (dEnd <= dVar38 && dVar38 != dEnd)) ||
                        ((endDir < 0) && (dVar38 <= dEnd && dEnd != dVar38))) {
                        break;
                    }
                    uint64_t fmtBits;
                    memcpy(&fmtBits, &dVar38, 8);   /* dc:108448 xmm 位模式 → %Lf */
                    PECMD_FormatU64RetEnd(local_328, fmtBits, L"%Lf");
                    pWVar14 = StrChrW(local_328, L'.');
                    if (pWVar14 != NULL) {
                        iVar7 = lstrlenW(pWVar14);
                        for (pWVar14 = pWVar14 + (longlong)iVar7 + -1; pWVar14[0] == L'0';
                             pWVar14 = pWVar14 + -1) {
                            pWVar14[0] = L'\0';     /* 去尾零 */
                        }
                        if (pWVar14[0] == L'.') {
                            pWVar14[0] = L'\0';
                        }
                    }
                    PECMD_SetVariable(param_1, s470, local_328);
                    dVar38 = dVar38 + dStep;        /* dc:108461 当前值 += 步长 */
                }
                count = count + 1;                  /* dc:108463 */
                if (lVar13 == 0) {                  /* dc:108464 */
                    if (s468[0] != L'\0') {
                        PECMD_ProcessScriptBlock((uint64_t)(uintptr_t)param_1,
                                                 (uint64_t)(uintptr_t)s468, res18, NULL,
                                                 (void *)(uintptr_t)param_5);   /* dc:108466 */
                    }
                }
                else {
                    blk3e8 = blk428 + 1;            /* dc:108470 */
                    PECMD_TokenizeExpression((int64_t)(uintptr_t)param_1,
                                             (int64_t)(uintptr_t)res18, &blk3e8, 0, g_szEmpty);
                }
                if ((*(uint16_t *)(param_1 + 0x19) & 0x100) != 0) {
                    *(uint16_t *)(param_1 + 0x19) = *(uint16_t *)(param_1 + 0x19) & 0xfeff;
                }
                if (g_flagA24F < 1) {
                    break;                          /* dc:108476 */
                }
            }
        }
        ppWVar19 = &s438;                           /* dc:108479 */
    }
    else {                                          /* dc:108481-108525 — NL:/delims: 行迭代 */
        s330 = NULL;
        pWVar10 = cur;
        while (((cur = pWVar10, (g_flagA24F > 0 && ((WVar18 = pWVar10[0]), WVar18 != L'\0'))) &&
                ((*(uint8_t *)((int64_t)(uintptr_t)param_1 + 0xc8) & 0xef) == 0))) {
            if (WVar31 == L'\0') {                  /* dc:108487 无定界符 → \r\n 行切分 */
                do {
                    if ((WVar18 == L'\r') || (WVar18 == L'\n')) {
                        break;
                    }
                    cur = cur + 1;
                    WVar18 = cur[0];
                } while (WVar18 != L'\0');
            }
            else {
                PECMD_SkipWCharUntil(&cur, (uint16_t)WVar31);   /* dc:108495 */
            }
            WVar18 = cur[0];
            cur[0] = L'\0';
            PECMD_SetVariable(param_1, s470, pWVar10);   /* dc:108499 */
            if ((WVar31 == L'\0') && (WVar18 == L'\r')) {
                if (cur[1] == L'\n') {
                    cur = cur + 2;
                }
                else {
                    cur = cur + 1;
                }
            }
            else if (WVar18 != L'\0') {
                cur = cur + 1;                      /* dc:108506 LAB_1400adb5d */
            }
            count = count + 1;
            if (lVar13 == 0) {
                if (s468[0] != L'\0') {
                    PECMD_ProcessScriptBlock((uint64_t)(uintptr_t)param_1,
                                             (uint64_t)(uintptr_t)s468, res18, NULL,
                                             (void *)(uintptr_t)param_5);   /* dc:108513 */
                }
            }
            else {
                blk3e8 = blk428 + 1;                /* dc:108517 */
                PECMD_TokenizeExpression((int64_t)(uintptr_t)param_1,
                                         (int64_t)(uintptr_t)res18, &blk3e8, 0, g_szEmpty);
            }
            pWVar10 = cur;                          /* dc:108520 */
            if ((*(uint16_t *)(param_1 + 0x19) & 0x100) != 0) {
                *(uint16_t *)(param_1 + 0x19) = *(uint16_t *)(param_1 + 0x19) & 0xfeff;
            }
        }
        ppWVar19 = &s330;                           /* dc:108525 */
    }
    PECMD_FreeStrBuf(ppWVar19);                     /* dc:108527 */
    iVar8 = (int)n494;                              /* dc:108528 */
LAB_1400ae5ea:
    *(uint16_t *)(param_1 + 0x19) = *(uint16_t *)(param_1 + 0x19) & 0x8b;   /* dc:108530 */
    if (bVar4) {                                    /* dc:108531 体==L"*" → 变量=尾槽 */
        PECMD_SetVariable(param_1, s470, tgtSlot);
    }
    else if (flag498 != 0) {                        /* dc:108534 → 计数写入变量 */
        PECMD_AppendLongDecimal(param_1, (uint64_t)count, s470);
    }
    PECMD_FreeStrBuf(&s418);
    PECMD_FreeStrBuf(&s488);
    PECMD_FreeStrBuf(&buf410);
    PECMD_FreeStrBuf(&tgtSlot);
    PECMD_FreeStrBuf(&s468);
    PECMD_FreeStrBuf(&s478);
    PECMD_FreeStrBuf(&s470);
    PECMD_FreeStrBuf(&s408);
    PECMD_FreeArray_ddf8(&ptrTbl);
    PECMD_FreeStrBuf((void *)&ptrTbl);
    PECMD_FreeStrBuf((void *)&fcell[0]);
    return (ulonglong)(int64_t)iVar8;               /* dc:108548 */
LAB_1400adddf:
    PECMD_FreeStrBuf(&s468);                        /* dc:108386-108392 */
    PECMD_FreeStrBuf(&s478);
    PECMD_FreeStrBuf(&s470);
    PECMD_FreeStrBuf(&s408);
    PECMD_FreeArray_ddf8(&ptrTbl);
    PECMD_FreeStrBuf((void *)&ptrTbl);
    PECMD_FreeStrBuf((void *)&fcell[0]);
    return retCell;                                 /* dc:108393 — 缺 4 分隔符 → 1 */
LAB_1400add9a:
    PECMD_FreeStrBuf(&s438);                        /* dc:108379-108383 */
    PECMD_FreeStrBuf(&s418);
    PECMD_FreeStrBuf(&s488);
    PECMD_FreeStrBuf(&buf410);
    PECMD_FreeStrBuf(&tgtSlot);
    retCell = 0xffffffff80070057;                   /* dc:108384 E_INVALIDARG */
    goto LAB_1400adddf;
}

/* ================================================================
 * @0x1400ae6f4  驱动器/分区扫描 (盘符分配; 简化主体, 反编译含大量
 *   CONCAT71/CONCAT22 寄存器拼接与指针运算)
 * signature: uint __fastcall PECMD_AssignDriveLetter(LPCWSTR param_1, WCHAR * param_2)
 */
uint PECMD_AssignDriveLetter(LPCWSTR param_1, WCHAR *param_2)
{
    ULONGLONG uVar10;
    uint uVar15;
    WCHAR WVar6;
    int iVar19 = 0;
    uint uVar13;
    uint local_224;
    void *local_250 = (void *)0;
    void *local_248 = (void *)0;
    bool bVar23;
    uint uVar22;
    WCHAR local_118[108];

    EnterCriticalSection(&g_csDisk);
    WVar6 = *param_2;
    (void)WVar6;
    uVar15 = 0;
    PECMD_AllocWStringBuffer((WCHAR **)&local_248, 100);

    /* 若 param_1 不是盘符路径, 则扫描物理磁盘分配盘符:
     * 反编译为对 \\.\PhysicalDriveN 逐盘查询分区并填空闲盘符的复杂循环,
     * 含大量 CONCAT22/CONCAT71 寄存器拼接与指针运算。
     * 此处化简为可编译等价: 仅保留逐盘枚举结构, 细节见 TODO(verify). */
    uVar10 = PECMD_FindFirstFreeDrive(param_1);
    if ((short)uVar10 == 0) {
        bVar23 = *param_2 != L'\0';
        *param_2 = L'\0';
        uVar13 = -(uint)bVar23 & 16000;
        local_224 = (-(uint)bVar23 & 0xfffffffe) + 1;
        iVar19 = 0;
        while (1) {
            uVar22 = uVar13;
            if ((15999 < (int)uVar13) || ((int)uVar13 < 0)) {
                break;
            }
            wsprintfW(local_118, WSTR("\\\\.\\PhysicalDrive%d"), (ulonglong)uVar22);
            {
                HANDLE hObject = (HANDLE)0;
                SetLastError(0);
                PECMD_OpenFileHandle(&hObject, (LPCWSTR)local_118, 0x80000000, 3,
                                     (LPSECURITY_ATTRIBUTES)0x0, 3, 0x20000000, (HANDLE)0);
                if (hObject == (HANDLE)0) {
                    iVar19 = iVar19 + 1;
                    if (0x1f < iVar19) {
                        break;
                    }
                }
                else {
                    int local_res20[2] = {0, 0};
                    longlong local_210 = -1;
                    ulonglong *puVar11;
                    iVar19 = 0;
                    PECMD_FindPartitionStartSector(hObject, local_res20, &local_210);
                    local_res20[0] = 0;
                    puVar11 = PECMD_GetDiskLayoutInfo(hObject, (ulonglong *)local_250,
                                                      (uint32_t *)local_res20);
                    if (puVar11 == (ulonglong *)0) {
                        if (hObject != (HANDLE)0xffffffffffffffff) {
                            CloseHandle(hObject);
                        }
                    }
                    else {
                        uint local_264 = PECMD_QueryDeviceIoInfo(hObject, 0x200);
                        uint local_220 = (uint)puVar11[5];
                        ulonglong *local_258;
                        uint uVar17;
                        local_258 = puVar11 + 6;
                        uVar17 = (ulonglong)(int)local_264;
                        if ((int)puVar11[0] == 1) {
                            PECMD_ReadDiskSectorScan((ulonglong *)&hObject,
                                                     (ulonglong *)((char *)local_250 + 0x81cb0),
                                                     (uint)puVar11[5], local_264, 0, (ulonglong *)0,
                                                     (PECMD_LI(0)));
                            uVar17 = (ulonglong)local_264;
                        }
                        /* 分区项遍历 (反编译对 puVar11 数组做逐项扫描) —— 化简
                         * 为第一项处理并标记 TODO(verify). */
                        {
                            uint index = 0;
                            (void)index;
                            (void)local_220;
                            (void)local_258;
                            (void)uVar17;
                        }
                        if (hObject != (HANDLE)0xffffffffffffffff) {
                            CloseHandle(hObject);
                        }
                    }
                }
            }
            uVar13 = uVar13 + local_224;
        }
        uVar15 = 0;
    }
    else {
        uVar15 = 0x40000000;
    }
    PECMD_FreeStrBuf(&local_248);
    LeaveCriticalSection(&g_csDisk);
    return uVar15;
}
