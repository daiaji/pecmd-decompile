/* Restored batch28 B3 business functions (part i28a) */
#include <stdbool.h>

#include "win32_stub.h"
#include "pecmd_defs.h"

/* ---- Ghidra primitive aliases (project convention) ---- */
typedef unsigned int        uint;
typedef unsigned short      ushort;
typedef uint8_t             byte;
typedef uint64_t            undefined8;
typedef void                undefined;
typedef int64_t             longlong;
typedef uint64_t            ulonglong;

/* CONCAT44(hi,lo): (uint32)hi<<32 | (uint32)lo  (Ghidra register glue; used as
 * a pointer-identity cast, so simply fold to (uintptr_t)value below) */
#ifndef CONCAT44
#define CONCAT44(hi, lo) (((uint64_t)(uint32_t)(hi)) << 32 | (uint32_t)(lo))
#endif

/* ---- Globals ---- */
extern WCHAR g_szEmpty[];            /* empty string (g_szEmpty) */

/* ---- Helper function externs (bodies live in other core_*.c files) ---- */
extern void    *operator_new(size_t size);  /* global new wrapper */

/* string/var helpers */
extern void      FUN_140063620(void *out);         /* @0x140063620 release slot */
extern WCHAR    *FUN_14005b154(WCHAR **pp);        /* @0x14005b154 skip spaces */
extern void      FUN_1400702b0(void *ps, const WCHAR *src);  /* @0x1400702b0 assign */
extern WCHAR    *FUN_14006375c(WCHAR **ps, const WCHAR *src); /* @0x14006375c cat */
extern void      FUN_14005b104(void *ps);          /* @0x14005b104 free slot */
extern void      PECMD_SplitTokenTrimWs(void *src, void *dst, int16_t delim); /* split list */
extern void      FUN_140003a20(void *script, void *str, int mode);   /* expand */
extern int64_t  *PECMD_SplitTokenAssignVar(WCHAR **out, WCHAR **pp, uint32_t sep, int flag);
extern void      PECMD_ExtractTokenByDelim(void *src, void *dst, int mode);
extern void      PECMD_CopyUpToChar(void *pp, void *out, uint32_t sep);
extern void      FUN_1400679b0(void *pp, int *out, WCHAR sep);
extern uint64_t  PECMD_ParseSignedNumber(short *);
extern void      PECMD_ParseLtwhParams(int64_t *a, uint32_t *b, int *c, int *d, uint32_t *e);
extern char      FUN_1400660ac(const char *tok, void *pp, int n);
extern WCHAR    *FUN_1400f429c(WCHAR **pp, uint16_t ch);   /* delimiter scan */
extern int64_t   FUN_14005c72c(const char *a, const WCHAR *w, int n);
extern int64_t   FUN_14005c788(const char *a, const WCHAR *w, int n);
extern int32_t   FUN_14005c7c4(const char *a, const WCHAR *w);

/* list-control / dialog helpers */
extern int       FUN_14005b2c0(void *p, LPCWSTR w, HWND hwnd);   /* accept check */
extern int       PECMD_DispatchControlCommand(void *a, LPCWSTR b, WPARAM c, HWND d, LPCWSTR e,
                               void *f, int64_t *g, HWND h, int64_t i);
extern int       PECMD_ParseUIntValue(LPCWSTR *pp, int *out);           /* scan int token */
extern void      FUN_14007d0ac(int64_t *ctx, LPCWSTR key, LPCWSTR value);
extern void      PECMD_SetControlTooltip(int64_t param_1, HWND param_2, int param_3,
                               LPCWSTR param_4, char param_5);
extern void      FUN_1400a9650(int64_t param_1, HWND param_2, int param_3,
                               LPCWSTR param_4, int param_5);
extern void      FUN_14001b3a0(int64_t *script, int64_t *a2);   /* default params */
extern ushort   *PECMD_ParseQuotedArg(longlong *param_1, longlong *param_2,
                               longlong *param_3);

/* object-construction helpers (called by the functions below) */
extern void      FUN_14005daf8(int64_t param_1, int *param_2, int *param_3,
                               int *param_4, int *param_5);
extern void      FUN_14005d9a8(int64_t param_1, int param_2);
extern void     *PECMD_SendCtrlMessage_0834(WPARAM wParam, uint64_t lParam);
extern uint64_t *PECMD_CreateControlWindow(uint64_t *param_1, int64_t param_2, uint32_t param_3,
                               uint64_t *param_4, uint32_t param_5, uint32_t param_6,
                               uint32_t param_7, uint32_t param_8, uint64_t *param_9,
                               uint64_t *param_10, uint32_t param_11, uint32_t param_12,
                               int param_13, WCHAR *param_14);
extern void      PECMD_CreateControlItem(WPARAM param_1, int64_t param_2, uint64_t *param_3,
                               int param_4, int param_5, int param_6, int param_7,
                               uint64_t *param_8, uint64_t *param_9, uint32_t param_10,
                               uint32_t param_11, WCHAR *param_12);
extern void      FUN_1400aa53c(WPARAM param_1, int64_t param_2, WCHAR **param_3,
                               int param_4, int param_5, int param_6, int param_7,
                               WCHAR **param_8, WCHAR **param_9, uint32_t param_10,
                               LPCWSTR param_11);
extern void      FUN_1400aa7e4(WPARAM param_1, int64_t param_2, WCHAR **param_3,
                               int param_4, int param_5, int param_6, int param_7,
                               int param_8, LPCWSTR param_9, uint32_t param_10);

/* ================================================================
 * @0x1400a97cc  GUI "Title/Tip/Select" 命令处理
 * signature: undefined8 __fastcall PECMD_ProcessTitleTipSelect(undefined8 * param_1,
 *   longlong * param_2, LPCWSTR param_3, LPCWSTR param_4, WPARAM param_5,
 *   undefined8 * param_6)
 */
uint64_t PECMD_ProcessTitleTipSelect(uint64_t *param_1, int64_t *param_2, LPCWSTR param_3,
                       LPCWSTR param_4, WPARAM param_5, uint64_t *param_6)
{
    int iVar1;
    HWND hWnd;
    HWND pHVar2;
    void *lpCriticalSection;
    int iVar3;
    int bVar4;
    uint64_t local_res8;
    LPCWSTR local_a8[2];
    WCHAR local_98[56];

    iVar1 = *(int *)((char *)param_1[7] + 0x118);
    hWnd = *(HWND *)((char *)param_1[7] + 0x20);
    pHVar2 = *(HWND *)(*(int64_t *)((char *)param_1[10] + 0x40) + 0x20);
    bVar4 = FUN_14005b2c0(param_1, param_4, pHVar2);
    if (bVar4 == 0) {
        return 0;
    }
    lpCriticalSection = (void *)(uintptr_t)*param_6;
    if (lpCriticalSection != (void *)0) {
        *param_6 = 0;
        LeaveCriticalSection(lpCriticalSection);
    }
    if (FUN_14005c788("Title", (const WCHAR *)param_4, 5) == 0) {
        if (FUN_14005c788("Tip", (const WCHAR *)param_4, 3) == 0) {
            local_res8 = (uint64_t)(int32_t)-1;
            if (FUN_14005c7c4("Select", (const WCHAR *)param_4) == 0) {
                /* "Select" query handler */
                PECMD_DispatchControlCommand((void *)param_1[7], param_4, param_5, hWnd, param_3,
                              (void *)param_1[10], param_2, pHVar2, (int64_t)param_1);
                return 0;
            }
            local_a8[0] = param_3;
            bVar4 = PECMD_ParseUIntValue((LPCWSTR *)local_a8, (int *)&local_res8);
            iVar3 = (int)(uint32_t)local_res8;
            if ((bVar4 < 1) || ((int)(uint32_t)local_res8 < 1) ||
                (iVar1 < (int)(uint32_t)local_res8)) {
                return 0x80070057;
            }
            /* TODO(verify): Ghidra dropped the vararg of wsprintfW(L"%ld", ...) */
            wsprintfW(local_98, WSTR("%ld"));
            FUN_1400702b0(&local_res8, WSTR("&&"));
            FUN_14006375c((WCHAR **)&local_res8, (LPCWSTR)param_1[2]);
            FUN_14006375c((WCHAR **)&local_res8, WSTR(".Select"));
            if (*(short *)param_1[2] != 0) {
                FUN_14007d0ac((int64_t *)param_1[10],
                              (LPCWSTR)(uintptr_t)local_res8, local_98);
            }
            FUN_14005b104(&local_res8);
            SendMessageW(hWnd, 0x462, (WPARAM)(long long)(iVar3 + -1), 0);
        } else {
            /* "Tip" setter */
            local_a8[0] = param_4 + 3;
            local_res8 = 1;
            bVar4 = PECMD_ParseUIntValue((LPCWSTR *)local_a8, (int *)&local_res8);
            if ((bVar4 < 1) || ((int)(uint32_t)local_res8 < 1) ||
                (iVar1 < (int)(uint32_t)local_res8)) {
                return 0x80070057;
            }
            PECMD_SetControlTooltip((int64_t)param_1, hWnd, (int)(uint32_t)local_res8 + -1,
                          param_3, '\0');
        }
    } else {
        /* "Title" setter */
        local_a8[0] = param_4 + 5;
        local_res8 = 1;
        bVar4 = PECMD_ParseUIntValue((LPCWSTR *)local_a8, (int *)&local_res8);
        if ((bVar4 < 1) || ((int)(uint32_t)local_res8 < 1) ||
            (iVar1 < (int)(uint32_t)local_res8)) {
            return 0x80070057;
        }
        FUN_1400a9650((int64_t)param_1, hWnd, (int)(uint32_t)local_res8 + -1,
                      param_3, 0);
    }
    return 0;
}

/* ================================================================
 * @0x1400aa664  列表控件对象构造/派发 (被 PECMD_ParseListControl 调用)
 * signature: undefined __fastcall PECMD_CreateControlItem(WPARAM param_1,
 *   longlong param_2, undefined8 * param_3, int param_4, int param_5,
 *   int param_6, int param_7, undefined8 * param_8, undefined8 * param_9,
 *   uint param_10, uint param_11, WCHAR * param_12)
 */
void PECMD_CreateControlItem(WPARAM param_1, int64_t param_2, uint64_t *param_3,
                   int param_4, int param_5, int param_6, int param_7,
                   uint64_t *param_8, uint64_t *param_9, uint32_t param_10,
                   uint32_t param_11, WCHAR *param_12)
{
    int64_t lVar1;
    uint32_t uVar2;
    uint64_t *puVar3;
    uint64_t *puVar5;
    uint64_t *puVar6;
    int64_t *plVar4;
    int local_res20[2];
    int entry_count;
    int i;

    local_res20[0] = param_4;
    puVar3 = (uint64_t *)PECMD_SendCtrlMessage_0834(param_1, *param_3);
    puVar6 = (uint64_t *)0;
    if (puVar3 != (uint64_t *)0) {
        plVar4 = *(int64_t **)(param_1 + 0x1a0);
        lVar1 = *plVar4;
        FUN_14005daf8(param_1, local_res20, &param_5, &param_6, &param_7);
        uVar2 = param_11;
        /* 在列表条目数组中查找同类型同序号条目 (Ghidra 用寄存器拼接索引变量,
         * 简化为等价整数循环; TODO(verify)) */
        entry_count = **(int **)(param_1 + 0x1a8);
        if (0 < entry_count) {
            int64_t *p = *(int64_t **)(param_1 + 0x1a0);
            for (i = 0; i < entry_count; i++) {
                int64_t entry = *p;
                if (*(int *)(entry + 8) == 6) {
                    if (param_11 == *(uint32_t *)(entry + 0x74)) {
                        puVar6 = (uint64_t *)(uintptr_t)1;
                        break;
                    }
                }
                p = p + 1;
            }
        }
        puVar5 = (uint64_t *)operator_new(0x78);
        if (puVar5 != (uint64_t *)0) {
            puVar5 = PECMD_CreateControlWindow(puVar5, param_2,
                                   (int)(((int64_t)(uintptr_t)puVar3 - lVar1) >> 3) + 0x1000,
                                   param_3, (uint32_t)local_res20[0], (uint32_t)param_5,
                                   (uint32_t)param_6, (uint32_t)param_7, param_8, param_9,
                                   param_10, uVar2, (int)(puVar6 == (uint64_t *)0),
                                   param_12);
        }
        *puVar3 = (uint64_t)puVar5;
        FUN_14005d9a8(param_1, 0);
    }
}

/* ================================================================
 * @0x1400aab98  列表/组合控件命令解析 (多行/选项形式)
 * signature: undefined8 __fastcall PECMD_ParseComboList(longlong * param_1,
 *   ushort * param_2, WPARAM param_3, undefined8 param_4)
 */
uint64_t PECMD_ParseComboList(longlong *param_1, ushort *param_2, WPARAM param_3,
                       undefined8 param_4)
{
    uint16_t uVar1;
    uint16_t uVar2;
    WCHAR *puVar3;
    LPCWSTR pWVar4;
    int64_t lVar5;
    uint64_t uVar6;
    int64_t *plVar7;
    uint64_t *puVar8;
    uint64_t uVar9;
    short sVar10;
    WCHAR *puVar11;
    LPCWSTR pWVar13;
    uint32_t uVar14;
    uint32_t uVar15;
    uint32_t uVar16;
    int local_res8[2];
    WCHAR *local_res10;
    int local_res18[2];
    undefined8 local_res20;
    int local_a8;
    int local_a4;
    WCHAR *local_a0;
    WCHAR *local_98;
    LPCWSTR local_90;
    WCHAR *local_88;
    WCHAR *local_80;
    WCHAR *local_78;
    WCHAR *local_68;
    uint64_t uVar12 = 0;

    local_res10 = param_2;
    local_res20 = param_4;
    FUN_140063620(&local_68);
    if (param_3 == 0) {
        FUN_14001b3a0(param_1, (int64_t *)0);
        param_3 = param_1[8];
        if (param_3 == 0) {
            uVar12 = 0xffffffff80070057;
            goto LAB_1400ab01b;
        }
    }
    FUN_14005b154(&local_res10);
    puVar11 = local_res10;
    sVar10 = 0x2c;
    local_80 = local_res10;
    uVar16 = 0;
    local_90 = (LPCWSTR)0;
    local_88 = (WCHAR *)0;
    FUN_1400f429c(&local_80, 0x2c);
    puVar3 = local_80;
    pWVar13 = g_szEmpty;
    uVar1 = *local_80;
    *local_80 = 0;
    uVar2 = *puVar11;
    uVar15 = (uint32_t)uVar12;
    while ((uint16_t)(sVar10 + 1U) == uVar2) {
        lVar5 = FUN_14005c72c("-center", (const WCHAR *)puVar11, 7);
        if ((char)lVar5 == 0) {
            lVar5 = FUN_14005c72c("-right", (const WCHAR *)puVar11, 6);
            if ((char)lVar5 != 0) {
                uVar15 = 0x20000000;
                pWVar4 = local_90;
                goto LAB_1400aac88;
            }
            lVar5 = FUN_14005c72c("-scale", (const WCHAR *)puVar11, 6);
            pWVar4 = pWVar13;
            if (((char)lVar5 != 0) ||
                (uVar6 = FUN_14005c788("-scale:", (const WCHAR *)puVar11, 7),
                 pWVar4 = local_90, (char)uVar6 == '\0'))
                goto LAB_1400aac88;
            local_90 = (LPCWSTR)(puVar11 + 7);
            local_res10 = PECMD_ParseQuotedArg(param_1, (longlong *)&local_90,
                                        (longlong *)&local_88);
            pWVar13 = g_szEmpty;
        } else {
            uVar15 = 0x40000000;
            pWVar4 = local_90;
        LAB_1400aac88:
            local_90 = pWVar4;
            uVar2 = *puVar11;
            while ((uVar2 != 0 &&
                    (((local_res10 = puVar11, (uint16_t)uVar2 < 9 ||
                       (0xd < (uint16_t)uVar2)) && (uVar2 != 0x20))))) {
                puVar11 = puVar11 + 1;
                local_res10 = puVar11;
                uVar2 = *puVar11;
            }
            FUN_14005b154(&local_res10);
        }
        uVar16 = (uint32_t)uVar15;
        param_4 = local_res20;
        puVar11 = local_res10;
        uVar2 = *local_res10;
    }
    *puVar3 = uVar1;
    FUN_1400702b0(&local_a0, (const WCHAR *)pWVar13);
    FUN_1400702b0(&local_98, (const WCHAR *)g_szEmpty);
    FUN_1400702b0(&local_78, (const WCHAR *)g_szEmpty);
    local_a8 = 0;
    local_a4 = 0;
    local_res18[0] = 0;
    local_res8[0] = 0;
    uVar14 = 0;
    uVar1 = *puVar11;
    if (uVar1 == 0x2a) {
        local_res10 = puVar11 + 1;
        FUN_14005b154(&local_res10);
    }
    PECMD_SplitTokenTrimWs(&local_res10, &local_a0, 0x2c);
    FUN_140003a20(param_1, &local_a0, 1);
    if (*local_res10 == 0x2c) {
        local_res10 = local_res10 + 1;
        plVar7 = PECMD_SplitTokenAssignVar(&local_68, &local_res10, 0x2c, 1);
        PECMD_ParseLtwhParams(plVar7, (uint32_t *)&local_a8, &local_a4, local_res18,
                      (uint32_t *)local_res8);
        if (*local_res10 == 0x2c) {
            local_res10 = local_res10 + 1;
            plVar7 = PECMD_SplitTokenAssignVar(&local_68, &local_res10, 0x2c, 1);
            PECMD_ExtractTokenByDelim(plVar7, &local_98, 0);
            if (*local_res10 == 0x2c) {
                local_res10 = local_res10 + 1;
                PECMD_CopyUpToChar(&local_res10, &local_78, 0x2c);
                if (*local_res10 == 0x2c) {
                    local_res10 = local_res10 + 1;
                    puVar8 = (uint64_t *)PECMD_SplitTokenAssignVar(&local_68, &local_res10, 0x2c, 1);
                    uVar9 = PECMD_ParseSignedNumber((short *)*puVar8);
                    uVar14 = (uint32_t)uVar9;
                }
            }
        }
        if (uVar1 != 0x2a) {
            param_1 = *(longlong **)(param_3 + 0x290);
        }
        FUN_1400aa53c(param_3, (int64_t)param_1, &local_a0, local_a8, local_a4,
                      local_res18[0], local_res8[0], &local_98, &local_78,
                      (uVar14 & 0xffff) | uVar16, local_90);
        FUN_14005b104(&local_78);
        FUN_14005b104(&local_98);
        FUN_14005b104(&local_a0);
        FUN_14005b104(&local_88);
    } else {
        FUN_14005b104(&local_78);
        FUN_14005b104(&local_98);
        FUN_14005b104(&local_a0);
        FUN_14005b104(&local_88);
        uVar12 = 1;
    }
LAB_1400ab01b:
    FUN_14005b104(&local_68);
    return (uint64_t)uVar12;
}

/* ================================================================
 * @0x1400ab040  列表/组合控件命令解析 (单选形式; 返回 0=已处理,1=未匹配)
 * signature: undefined8 __fastcall PECMD_ParseListControl(longlong * param_1,
 *   ushort * param_2, WPARAM param_3, undefined8 param_4)
 */
uint64_t PECMD_ParseListControl(longlong *param_1, ushort *param_2, WPARAM param_3,
                       undefined8 param_4)
{
    uint16_t uVar1;
    uint16_t uVar2;
    WCHAR *puVar3;
    LPCWSTR pWVar4;
    int64_t lVar5;
    int64_t *plVar6;
    uint64_t *puVar7;
    uint64_t uVar8;
    uint64_t uVar9;
    short sVar10;
    WCHAR *puVar11;
    LPCWSTR pWVar12;
    uint32_t uVar13;
    uint32_t uVar14;
    uint32_t uVar15 = 0;
    uint32_t uVar16;
    WCHAR *local_res10;
    uint32_t local_res18[2];
    undefined8 local_res20;
    int local_98;
    int local_94;
    int local_90;
    int local_8c;
    WCHAR *local_88;
    LPCWSTR local_80;
    WCHAR *local_78;
    WCHAR *local_70;
    WCHAR *local_68;
    WCHAR *local_60;
    WCHAR *local_48;

    local_res10 = param_2;
    local_res20 = param_4;
    if (param_3 == 0) {
        FUN_14001b3a0(param_1, (int64_t *)0);
        param_3 = param_1[8];
        if (param_3 == 0) {
            return 0xffffffff80070057;
        }
    }
    FUN_140063620(&local_60);
    FUN_14005b154(&local_res10);
    puVar11 = local_res10;
    sVar10 = 0x2c;
    local_70 = local_res10;
    uVar16 = 0;
    local_80 = (LPCWSTR)0;
    local_68 = (WCHAR *)0;
    FUN_1400f429c(&local_70, 0x2c);
    puVar3 = local_70;
    uVar1 = *local_70;
    *local_70 = 0;
    pWVar12 = g_szEmpty;
    uVar2 = *puVar11;
    do {
        if ((uint16_t)(sVar10 + 1U) != uVar2) {
            *puVar3 = uVar1;
            FUN_1400702b0(&local_88, (const WCHAR *)pWVar12);
            FUN_1400702b0(&local_78, (const WCHAR *)g_szEmpty);
            FUN_1400702b0(&local_48, (const WCHAR *)g_szEmpty);
            uVar13 = 0;
            local_8c = 0;
            local_94 = 0;
            local_98 = 0;
            local_90 = 0;
            uVar9 = 1;
            local_res18[0] = 0;
            uVar1 = *puVar11;
            if (uVar1 == 0x2a) {
                local_res10 = puVar11 + 1;
                FUN_14005b154(&local_res10);
            }
            PECMD_SplitTokenTrimWs(&local_res10, &local_88, 0x2c);
            FUN_140003a20(param_1, &local_88, 1);
            if (*local_res10 == 0x2c) {
                local_res10 = local_res10 + 1;
                plVar6 = PECMD_SplitTokenAssignVar(&local_60, &local_res10, 0x2c, 1);
                PECMD_ParseLtwhParams(plVar6, (uint32_t *)&local_8c, &local_94, &local_98,
                              (uint32_t *)&local_90);
                uVar14 = 0;
                if (*local_res10 == 0x2c) {
                    local_res10 = local_res10 + 1;
                    plVar6 = PECMD_SplitTokenAssignVar(&local_60, &local_res10, 0x2c, 1);
                    PECMD_ExtractTokenByDelim(plVar6, &local_78, 0);
                    uVar14 = uVar13;
                    if (*local_res10 == 0x2c) {
                        local_res10 = local_res10 + 1;
                        PECMD_CopyUpToChar(&local_res10, &local_48, 0x2c);
                        if (*local_res10 == 0x2c) {
                            local_res10 = local_res10 + 1;
                            puVar7 = (uint64_t *)PECMD_SplitTokenAssignVar(&local_60, &local_res10, 0x2c, 1);
                            uVar8 = PECMD_ParseSignedNumber((short *)*puVar7);
                            uVar15 = (uint32_t)uVar8;
                            if (*local_res10 == 0x2c) {
                                local_res10 = local_res10 + 1;
                                puVar7 = (uint64_t *)PECMD_SplitTokenAssignVar(&local_60, &local_res10, 0x2c, 1);
                                FUN_1400679b0(puVar7, (int *)local_res18, 0x2c);
                                uVar14 = local_res18[0];
                            }
                        }
                    }
                }
                if (uVar1 != 0x2a) {
                    param_1 = *(longlong **)(param_3 + 0x290);
                }
                PECMD_CreateControlItem(param_3, (int64_t)param_1, (uint64_t *)&local_88,
                              local_8c, local_94, local_98, local_90,
                              (uint64_t *)&local_78, (uint64_t *)&local_48,
                              (uVar15 & 0xffff) | uVar16, uVar14,
                              (WCHAR *)(uintptr_t)local_80);
                uVar9 = 0;
            }
            FUN_14005b104(&local_48);
            FUN_14005b104(&local_78);
            FUN_14005b104(&local_88);
            FUN_14005b104(&local_68);
            FUN_14005b104(&local_60);
            return (uint64_t)uVar9;
        }
        lVar5 = FUN_14005c72c("-center", (const WCHAR *)puVar11, 7);
        if ((char)lVar5 == 0) {
            lVar5 = FUN_14005c72c("-right", (const WCHAR *)puVar11, 6);
            if ((char)lVar5 != 0) {
                uVar16 = 0x20000000;
                pWVar4 = local_80;
                goto LAB_1400ab134;
            }
            lVar5 = FUN_14005c72c("-scale", (const WCHAR *)puVar11, 6);
            pWVar4 = pWVar12;
            if (((char)lVar5 != 0) ||
                (uVar9 = FUN_14005c788("-scale:", (const WCHAR *)puVar11, 7),
                 pWVar4 = local_80, (char)uVar9 == '\0'))
                goto LAB_1400ab134;
            local_80 = (LPCWSTR)(puVar11 + 7);
            local_res10 = PECMD_ParseQuotedArg(param_1, (longlong *)&local_80,
                                        (longlong *)&local_68);
            pWVar12 = g_szEmpty;
        } else {
            uVar16 = 0x40000000;
            pWVar4 = local_80;
        LAB_1400ab134:
            local_80 = pWVar4;
            uVar2 = *puVar11;
            while ((uVar2 != 0 &&
                    (((local_res10 = puVar11, (uint16_t)uVar2 < 9 ||
                       (0xd < (uint16_t)uVar2)) && (uVar2 != 0x20))))) {
                puVar11 = puVar11 + 1;
                local_res10 = puVar11;
                uVar2 = *puVar11;
            }
            FUN_14005b154(&local_res10);
        }
        uVar2 = *local_res10;
        param_4 = local_res20;
        puVar11 = local_res10;
    } while (true);
}

/* ================================================================
 * @0x1400ab51c  进度条/平滑控件命令解析
 * signature: undefined8 __fastcall PECMD_ParseControlCommand(longlong * param_1,
 *   LPWSTR param_2, WPARAM param_3)
 */
uint64_t PECMD_ParseControlCommand(longlong *param_1, LPWSTR param_2, WPARAM param_3)
{
    WCHAR WVar1;
    char cVar2;
    LPWSTR pWVar3;
    uint64_t uVar4;
    int iVar5;
    WCHAR *local_res10;
    int local_res18[2];
    int local_res20[2];
    int local_58;
    int local_54;
    int local_50[2];
    WCHAR *local_48;

    local_res10 = param_2;
    if (param_3 == 0) {
        FUN_14001b3a0(param_1, (int64_t *)0);
        param_3 = param_1[8];
        if (param_3 == 0) {
            return 0xffffffff80070057;
        }
    }
    FUN_14005b154(&local_res10);
    FUN_1400702b0(&local_48, (const WCHAR *)g_szEmpty);
    local_50[0] = 0;
    local_54 = 0;
    local_58 = 0;
    local_res20[0] = 0;
    uVar4 = 1;
    local_res18[0] = 0;
    WVar1 = *local_res10;
    if (WVar1 == L'*') {
        local_res10 = local_res10 + 1;
        FUN_14005b154(&local_res10);
    }
    cVar2 = FUN_1400660ac("-smooth", &local_res10, 7);
    PECMD_SplitTokenTrimWs(&local_res10, &local_48, 0x2c);
    FUN_140003a20(param_1, &local_48, 1);
    if (*local_res10 == L',') {
        local_res10 = local_res10 + 1;
        PECMD_ParseLtwhParams((int64_t *)&local_res10, (uint32_t *)local_50, &local_54,
                      &local_58, (uint32_t *)local_res20);
        iVar5 = 0;
        if (*local_res10 == L',') {
            local_res10 = local_res10 + 1;
            FUN_1400679b0(&local_res10, local_res18, 0x2c);
            iVar5 = local_res18[0];
        }
        pWVar3 = (LPWSTR)0;
        if (*local_res10 == L',') {
            pWVar3 = local_res10 + 1;
            local_res10 = pWVar3;
        }
        if (WVar1 != L'*') {
            param_1 = *(longlong **)(param_3 + 0x290);
        }
        FUN_1400aa7e4(param_3, (int64_t)param_1, &local_48, local_50[0], local_54,
                      local_58, local_res20[0], iVar5, (LPCWSTR)pWVar3,
                      (uint32_t)(cVar2 != '\0'));
        uVar4 = 0;
    }
    FUN_14005b104(&local_48);
    return (uint64_t)uVar4;
}
