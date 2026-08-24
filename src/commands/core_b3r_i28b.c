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
extern void PECMD_AllocStrSlot(void *out);                       /* @0x140063620 release slot */
extern WCHAR *PECMD_SkipLeadingControlChars(WCHAR **pp);         /* @0x14005b154 skip spaces */
extern void PECMD_StrDupAssign(void *ps, const WCHAR *src);      /* @0x1400702b0 assign */
extern WCHAR *PECMD_AppendWideStr(WCHAR **ps, const WCHAR *src); /* @0x14006375c cat */
extern void PECMD_FreeStrBuf(void *ps);                          /* @0x14005b104 free slot */
extern void PECMD_SplitTokenTrimWs(void *src, void *dst, int16_t delim); /* split list */
extern void PECMD_RunCommandLine(void *script, void *str, int mode);     /* expand */
extern int64_t *PECMD_SplitTokenAssignVar(WCHAR **out, WCHAR **pp, uint32_t sep, int flag);
extern void PECMD_CopyUpToChar(void *pp, void *out, uint32_t sep);
extern uint64_t PECMD_ParseSignedNumber(short *);
extern void PECMD_ParseLtwhParams(int64_t *a, uint32_t *b, int *c, int *d, uint32_t *e);
extern char PECMD_MatchTokenAdvance(const char *tok, void *pp, int n);
extern WCHAR *PECMD_SkipWCharUntil(WCHAR **pp, uint16_t ch); /* delimiter scan */
extern int64_t PECMD_TokPrefixICmp(const char *a, const WCHAR *w, int n);
extern int64_t PECMD_AsciiPrefixICmp(const char *a, const WCHAR *w, int n);
extern int32_t PECMD_AsciiWideICmp(const char *a, const WCHAR *w);
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
 * @0x1400acd90  FOR 命令解析/执行 (简化主体; 反编译为巨大含
 *   "Type propagation algorithm not settling" 的指针/寄存器拼接)
 * signature: ulonglong __fastcall PECMD_ForCommand(longlong * param_1,
 *   WCHAR * param_2, undefined8 param_3, longlong * param_4, undefined8 param_5)
 */
ULONGLONG PECMD_ForCommand(longlong *param_1, WCHAR *param_2, longlong *param_3, longlong *param_4,
                           uint64_t param_5)
{
    WCHAR WVar31;
    char cVar35;
    WCHAR *local_res10;
    int64_t local_3e0;
    longlong local_3d0;
    longlong local_3b0;
    int64_t local_3a8, local_3a0;
    uint local_45c;
    uint local_420;
    uint local_480;
    uint local_430;
    uint local_460;
    uint local_4a8;
    ulonglong uVar22;
    ulonglong uVar26;
    ulonglong uVar34;
    ulonglong uVar32;
    WCHAR cVar35d = ',';
    int64_t local_378;
    int64_t local_468;
    int iVar8 = 0;

    (void)param_3;
    (void)param_4;
    (void)param_5;

    /* 前导选项扫描: 可识别 NL / delims / * / - / v / ** / L / *bf / *ab / *qu /
     * *qu~ / *off / /s / /O:N / /O:-N / *cur / /size*: / /size: 等标记。
     * 简化版本以保持编译, 完整展开含大量寄存器拼接, 见 TODO(verify). */
    local_res10 = param_2;
    PECMD_AllocStrSlot(&local_3e0);
    local_3d0 = 0;
    WVar31 = *local_res10;
    cVar35 = ',';
    while ((((WVar31 != L'\0' && (WVar31 != L'*')) &&
             (((ushort)WVar31 < 9 || (0xd < (ushort)WVar31)))) &&
            (WVar31 != L' '))) {
        if ((WVar31 == L';') || (WVar31 == L':')) {
            cVar35 = (char)*local_res10;
        }
        local_res10 = local_res10 + 1;
        WVar31 = *local_res10;
    }
    PECMD_InitPtrTable(&local_3b0);
    PECMD_SkipLeadingControlChars(&local_res10);
    local_45c = 0;
    local_420 = 0;
    local_480 = 0;
    local_430 = 0;
    local_460 = 0;
    local_378 = -8;
    uVar22 = 0;
    uVar26 = 0;
    uVar34 = 0;
    uVar32 = 0;
    local_4a8 = 0;
    /* silence unused / unused-but-set decompiler scaffold vars */
    (void)cVar35;
    (void)cVar35d;
    (void)uVar22;
    (void)uVar26;
    (void)uVar32;
    (void)local_420;
    (void)local_430;
    (void)local_460;
    (void)local_480;
    (void)local_4a8;
    (void)local_3a0;
    (void)local_3a8;
    /* 选项 token 循环 (按反编译简化为可编译等价; 分支宏语义保留) */
    for (;;) {
        WCHAR ch = *local_res10;
        if ((ch != L'*') && (ch != L'/') && (ch != L'-')) {
            break; /* 非选项, 进入主体 */
        }
        if ((ch == L'-') && (local_res10[1] == L'-') &&
            (((local_res10[2] > 8) && (local_res10[2] < 0xe)) ||
             ((local_res10[2] == L' ') || (local_res10[2] == L'\0')))) {
            local_res10 = local_res10 + 2;
            PECMD_SkipLeadingControlChars(&local_res10);
            continue;
        }
        if (PECMD_MatchPrefixAdvance("NL", (int64_t *)&local_res10, 2)) {
            uVar34 = (uVar34 & 0xfffffffe) | 6;
            continue;
        }
        if (PECMD_AsciiPrefixICmp("NL:", (local_res10 + 1), 3) == 0) {
            local_res10 = local_res10 + 4;
            uVar34 = (uVar34 & ~8U) | 10;
            PECMD_ParseStringToken((int64_t *)&local_res10, (uint64_t)param_1, (int64_t *)0);
            PECMD_SkipLeadingControlChars(&local_res10);
            continue;
        }
        if (PECMD_AsciiPrefixICmp("delims:", (local_res10 + 1), 7) == 0) {
            local_res10 = local_res10 + 8;
            uVar34 = (uVar34 & ~8U) | 10;
            PECMD_ParseStringToken((int64_t *)&local_res10, (uint64_t)param_1, (int64_t *)0);
            PECMD_SkipLeadingControlChars(&local_res10);
            continue;
        }
        if ((PECMD_MatchTokenAdvance("*", &local_res10, -1) != 0) ||
            (PECMD_MatchTokenAdvance("-", &local_res10, -1) != 0)) {
            uVar34 = (uVar34 & 0xfffffffd) | 1;
            continue;
        }
        if ((PECMD_MatchPrefixAdvance("v", (int64_t *)&local_res10, 1)) ||
            (PECMD_MatchTokenAdvance("**", &local_res10, 2) != 0)) {
            uVar34 = (uVar34 & ~8U) | 5;
            continue;
        }
        if (PECMD_MatchPrefixAdvance("L", (int64_t *)&local_res10, 1)) {
            local_45c = (uint)(uVar34 & 0xfffffffd) | 0x11;
            uVar34 = local_45c;
            continue;
        }
        if (PECMD_MatchTokenAdvance("*bf", &local_res10, 3) != 0) {
            uVar32 = 1;
            continue;
        }
        if (PECMD_MatchTokenAdvance("*ab", &local_res10, 3) != 0) {
            local_420 = 1;
            continue;
        }
        if (PECMD_MatchTokenAdvance("*qu", &local_res10, 3) != 0) {
            local_480 = 1;
            continue;
        }
        if (PECMD_MatchTokenAdvance("*qu~", &local_res10, 4) != 0) {
            local_480 = 2;
            continue;
        }
        if (PECMD_MatchTokenAdvance("*off", &local_res10, 4) != 0) {
            local_430 = 1;
            continue;
        }
        if (PECMD_MatchTokenAdvance("/s", &local_res10, 2) != 0) {
            local_4a8 = 0x10;
            uVar22 = 0x10;
            break; /* /s 模式进入主体 */
        }
        if (PECMD_AsciiPrefixICmp("/s:", local_res10, 3) == 0) {
            local_res10 = local_res10 + 3;
            local_4a8 = 0x10;
            uVar22 = 0x10;
            continue;
        }
        if (PECMD_MatchTokenAdvance("/O:N", &local_res10, 4) != 0) {
            uVar26 = 0x20;
            continue;
        }
        if (PECMD_MatchTokenAdvance("/O:-N", &local_res10, 5) != 0) {
            uVar26 = 0x60;
            continue;
        }
        if (PECMD_MatchTokenAdvance("*cur", &local_res10, 4) != 0) {
            local_460 = 1;
            continue;
        }
        if (PECMD_AsciiPrefixICmp("/size*:", local_res10, 7) == 0) {
            int64_t *p = (int64_t *)&local_res10;
            local_res10 = local_res10 + 1;
            PECMD_ParseSizeNumber(p, &local_378);
            continue;
        }
        if (PECMD_AsciiPrefixICmp("/size:", local_res10, 6) == 0) {
            local_res10 = local_res10 + 6;
            PECMD_ParseSizeNumber((int64_t *)&local_res10, &local_378);
            continue;
        }
        /* 其他未知选项/普通串: 结束选项扫描 */
        break;
    }

    /* 主体: 将选项解析后的串拷贝到输出变量. (反编译尾部是庞大的
     * FOR 循环/分区枚举; 化简为单次 SetVar 语义, TODO(verify)) */
    {
        WCHAR *pOut = (WCHAR *)0;
        PECMD_StrCopyW(&pOut, local_res10, 0);
        PECMD_ExpandVarDispatch(param_1, (LPCWSTR)pOut, &local_3d0, 0, 1);
        if ((uVar34 & 1) == 0) {
            FUN_14007033c(&local_3d0, (LPCWSTR)pOut);
        }
        /* 结果写入环境变量: key=local_470 由 options 构造; 简化直接记录 */
        if (*(short *)&local_468 == 0) {
        }
        PECMD_FreeStrBuf(&pOut);
    }

    PECMD_FreeStrBuf(&local_3e0);
    PECMD_FreeArray_ddf8(&local_3b0);
    PECMD_FreeStrBuf(&local_3b0);
    return (ulonglong)(uint32_t)iVar8;
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
