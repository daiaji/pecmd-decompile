/* Restored batch28 B3 business functions (part i28d)
 *   PECMD_AddControlStar @0x1400b48e0   GUI 控件插入（变体 E 分派，参数含 star 标记）
 *   PECMD_AddControlWide @0x1400b4b58   GUI 控件添加（变体 C，宽参数集）
 *   PECMD_AddControlVariantB @0x1400b50e0   GUI 控件添加（变体 B，del/star 变体）
 *   PECMD_SetControlCommandTrigger @0x1400b6240   GUI 控件触发/脚本执行（控件槽填充）
 *   PECMD_AddTransControl @0x1400b9788   GUI 控件添加（变体 D，宽参数 5 元组）
 */
#include <stdbool.h>
#include <stdint.h>
#include <limits.h>

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

/* ---- Globals ---- */
extern WCHAR g_szEmpty[];            /* empty string (g_szEmpty) */
extern int      g_dblClickFlag;              /* 双击/触发标记 */

/* ---- string/var/parse helpers (bodies live in other core_*.c files) ---- */
extern void      PECMD_AllocStrSlot(void *out);         /* release/init slot */
extern WCHAR    *PECMD_SkipLeadingControlChars(WCHAR **pp);        /* skip spaces */
extern void      PECMD_StrDupAssign(void *ps, const WCHAR *src);  /* assign string slot */
extern void      FUN_14005b104(void *ps);          /* free string slot */
extern void      PECMD_SplitTokenTrimWs(void *src, void *dst, int16_t delim); /* split list */
extern void      PECMD_RunCommandLine(void *script, void *str, int mode);   /* expand */
extern int64_t  *PECMD_SplitTokenAssignVar(WCHAR **out, WCHAR **pp, uint32_t sep, int flag);
extern void      PECMD_ExtractTokenByDelim(void *src, void *dst, int mode);
extern void      PECMD_CopyUpToChar(void *pp, void *out, uint32_t sep);
extern int16_t   FUN_1400677B0(int64_t *pp, int64_t out);  /* 解析最多4个 # 分隔值 */
extern uint64_t  PECMD_ParseSignedNumber(short *);
extern void      PECMD_ParseLtwhParams(int64_t *a, uint32_t *b, int *c, int *d, uint32_t *e);
extern WCHAR    *PECMD_SkipWCharUntil(WCHAR **pp, uint16_t ch);   /* delimiter scan (thunk) */
extern int64_t   FUN_14005c72c(const char *a, const WCHAR *w, int n);
extern int64_t   PECMD_AsciiPrefixICmp(const char *a, const WCHAR *w, int n);
extern int32_t   PECMD_AsciiWideICmp(const char *a, const WCHAR *w);
extern uint64_t  PECMD_ParseHotkeyCode(int64_t *pp, uint32_t *flags, uint64_t p3, char p4);
extern int       PECMD_ParseUIntValue(LPCWSTR *pp, int *out);   /* 扫描整数 token */

/* ---- 脚本/执行 helpers ---- */
extern void      PECMD_ResetScriptChain(void *script, void *a2);   /* 默认参数/上下文 */
extern void      PECMD_DispatchExpressionBlock(void *script, LPCWSTR p);  /* 脚本执行循环 */
extern void      FUN_1400A4020(WCHAR **ps, LPCWSTR src);  /* 引用计数字符串设置 */
extern uint32_t  PECMD_ArgTokenize(int64_t *param_1, LPCWSTR param_2, int param_3);
extern void      PECMD_RefCountRelease(WCHAR **ps);               /* 引用计数释放 */
extern void      PECMD_ExpandBackslashNewline(const WCHAR *p, char c);   /* 清空串 */
extern void      FUN_140067F90(int64_t *ps);              /* '&' -> '&&' 转义 */
extern void      PECMD_ExpandVarDispatch(int64_t *script, LPCWSTR src, int64_t *out,
                               int a4, int a5);

/* ---- 控件对象分派 helpers（已还原，见 core_b3l.c / core_b3m.c） ---- */
extern void FUN_1400B2A4C(WPARAM mgr, LPCWSTR text, uint64_t *p3,
                          int x, int y, int w, int h,
                          int16_t *p8, uint32_t flags);
extern void FUN_1400B2B64(WPARAM mgr, int64_t v2, int64_t *p3, int x,
                          int y, int w, int h, int64_t *p8,
                          LPCWSTR text1, LPCWSTR text2, uint32_t flags,
                          char mode, int extra);
extern void FUN_1400B1DEC(int64_t mgr, int64_t v2, int64_t *p3,
                          uint32_t flags1, uint32_t flags2, int mode);
extern void FUN_1400B9204(WPARAM mgr, int64_t v2, int64_t *p3, int x,
                          int y, int w, int h, int64_t *p8,
                          int64_t *p9, int64_t *p10, LPCWSTR text,
                          uint32_t flags);
extern void PECMD_RemoveObjectByType(int64_t mgr, int p2, int p3, int p4); /* 删除动作 */

/* ========== PECMD_AddControlStar @ 1400b48e0 ==========
 * signature: ulonglong __fastcall PECMD_AddControlStar(LPCWSTR param_1,
 *   ushort * param_2, WPARAM param_3)
 * 在 param_3 指向的窗口上插入一个控件（变体 E）。返回 1 表示无坐标参数，
 * 0 表示已解析坐标参数。
 */
uint64_t PECMD_AddControlStar(LPCWSTR param_1, ushort *param_2, WPARAM param_3)
{
    ushort uVar1;
    int uVar4;
    uint64_t uVar5;
    int local_74;
    int local_78;
    int local_res20;
    int local_res8;
    LPCWSTR local_50;
    undefined8 local_48;
    WCHAR *local_70;   /* 主文本槽 */
    WCHAR *local_68;   /* 文本槽 2 */
    WCHAR *local_60;   /* 释放槽 */
    WCHAR *local_58;   /* 解析输出槽 */
    WCHAR *local_res10;
    int64_t *plVar2;
    int64_t *puVar3;

    local_res10 = (WCHAR *)param_2;
    PECMD_AllocStrSlot(&local_58);
    uVar5 = 0;
    local_48 = 0;
    local_50 = param_1;
    (void)local_48; (void)local_50;
    PECMD_StrDupAssign(&local_70, g_szEmpty);
    PECMD_AllocStrSlot(&local_68);
    PECMD_AllocStrSlot(&local_60);
    uVar1 = *param_2;
    local_74 = 0;
    local_78 = 0;
    local_res20 = 0;
    local_res8 = 0;
    while ((uVar1 != 0) &&
           ((local_res10 = (WCHAR *)param_2, (uVar1 < 9 || (0xd < uVar1))) &&
            (uVar1 != 0x20))) {
        param_2 = param_2 + 1;
        local_res10 = (WCHAR *)param_2;
        uVar1 = *param_2;
    }
    PECMD_SkipLeadingControlChars(&local_res10);
    uVar1 = *local_res10;
    if (uVar1 == 0x2a) {                 /* '*' 通配标记 */
        local_res10 = local_res10 + 1;
        PECMD_SkipLeadingControlChars(&local_res10);
    }
    PECMD_SplitTokenTrimWs(&local_res10, &local_70, 0x2c);
    PECMD_RunCommandLine((void *)param_1, &local_70, 1);
    uVar4 = 1;
    if (*local_res10 == 0x2c) {
        local_res10 = local_res10 + 1;
        plVar2 = PECMD_SplitTokenAssignVar(&local_58, &local_res10, 0x2c, 1);
        PECMD_ParseLtwhParams(plVar2, (uint32_t *)&local_74, &local_78, &local_res20,
                      (uint32_t *)&local_res8);
        uVar4 = (int)uVar5;
        if (*local_res10 == 0x2c) {
            local_res10 = local_res10 + 1;
            PECMD_SplitTokenTrimWs(&local_res10, &local_68, 0x2c);
            if (*local_res10 == 0x2c) {
                local_res10 = local_res10 + 1;
                puVar3 = PECMD_SplitTokenAssignVar(&local_58, &local_res10, 0x2c, 1);
                uVar4 = (int)PECMD_ParseSignedNumber((short *)*puVar3);
            }
        }
        if (uVar1 != 0x2a) {
            param_1 = *(LPCWSTR *)(param_3 + 0x290);
        }
        FUN_1400B2A4C(param_3, param_1, (uint64_t *)&local_70, local_74, local_78,
                      local_res20, local_res8, (int16_t *)&local_68, (uint32_t)uVar4);
        uVar4 = (int)uVar5;
    }
    FUN_14005b104(&local_60);
    FUN_14005b104(&local_68);
    FUN_14005b104(&local_70);
    FUN_14005b104(&local_58);
    return (uint64_t)uVar4;
}

/* ========== PECMD_AddControlWide @ 1400b4b58 ==========
 * signature: ulonglong __fastcall PECMD_AddControlWide(longlong * param_1,
 *   WCHAR * param_2, WPARAM param_3, undefined8 param_4)
 * 添加控件（变体 C）。
 */
uint64_t PECMD_AddControlWide(longlong *param_1, WCHAR *param_2, WPARAM param_3,
                       undefined8 param_4)
{
    WCHAR WVar2;
    WCHAR *pWVar10;
    int iVar9;
    uint32_t uVar16;
    uint32_t uVar7;
    undefined8 uVar4;
    WCHAR WVar8;
    WCHAR *local_res10;
    int local_res18;
    int local_a8;
    int local_88;
    int local_res8;
    int local_80;
    uint32_t uVar15;
    LPCWSTR local_a0;
    LPCWSTR local_98;
    longlong local_90;
    longlong local_78;
    undefined8 local_70;
    WCHAR *local_60;
    longlong *local_58;
    undefined8 local_50;
    WCHAR savedChar;
    WCHAR *opt;
    uint8_t bVar11;
    int64_t *plVar5;
    int64_t *puVar6;

    local_res10 = param_2;
    uVar15 = 0;
    local_50 = 0;
    local_58 = param_1;
    (void)local_50; (void)local_58;
    local_70 = param_4;
    PECMD_AllocStrSlot(&local_60);
    if (param_3 == 0) {
        PECMD_ResetScriptChain(param_1, (void *)0);
        param_3 = param_1[8];
        if (param_3 == 0) {
            FUN_14005b104(&local_60);
            return (uint64_t)0xffffffff80070057;
        }
    }
    pWVar10 = local_res10;
    iVar9 = 0x2c;
    local_a8 = 0;
    local_res18 = 0;
    uVar16 = 0;
    /* 选项区（以 '-' 开头或为参数开始） */
    {
        opt = local_res10;
        PECMD_SkipWCharUntil(&opt, 0x2c);   /* 跳过第一个 ',' */
        WVar2 = *opt;
        *opt = L'\0';
        WVar8 = (WCHAR)(iVar9 + 1);  /* '-' */
        savedChar = WVar2;
        bVar11 = (byte)(iVar9 - 0x2b);   /* 1 */
        if (WVar8 == *pWVar10) {
            do {
                longlong lVar3;
                lVar3 = FUN_14005c72c("-center", pWVar10, 7);
                if ((char)lVar3 == '\0') {
                    lVar3 = FUN_14005c72c("-right", pWVar10, 6);
                    if ((char)lVar3 == '\0') {
                        lVar3 = FUN_14005c72c("-h", pWVar10, 2);
                        if ((char)lVar3 == '\0') {
                            uVar4 = PECMD_AsciiPrefixICmp("-vcenter:", pWVar10, iVar9 + -0x23);
                            if ((char)uVar4 != '\0') {
                                pWVar10 = pWVar10 + 9;
                                local_res10 = pWVar10;
                                PECMD_AllocStrSlot(&local_a0);
                                PECMD_ExpandVarDispatch(param_1, pWVar10, (int64_t *)&local_a0, 0,
                                              bVar11);
                                local_98 = local_a0;
                                PECMD_ParseUIntValue(&local_98, &local_a8);
                                FUN_14005b104(&local_a0);
                            }
                        } else {
                            local_res18 = 0x4000000;
                        }
                    } else {
                        uVar16 = 0x20000000;
                    }
                } else {
                    uVar16 = 0x40000000;
                }
                WVar2 = *pWVar10;
                while ((WVar2 != L'\0') &&
                       ((local_res10 = pWVar10, (ushort)WVar2 < 9 || (0xd < (ushort)WVar2)) &&
                        (WVar2 != L' '))) {
                    pWVar10 = pWVar10 + 1;
                    local_res10 = pWVar10;
                    WVar2 = *pWVar10;
                }
                PECMD_SkipLeadingControlChars(&local_res10);
                pWVar10 = local_res10;
            } while (WVar8 == *local_res10);
            param_4 = local_70;
            WVar2 = savedChar;
            uVar15 = (uint32_t)local_a8;
        }
    }
    local_res18 = (int)(uVar16 | (uint32_t)local_res18);
    /* 恢复选项区末尾被置空的逗号分隔符 */
    *opt = WVar2;
    PECMD_StrDupAssign(&local_90, g_szEmpty);
    PECMD_StrDupAssign((void *)&local_a0, g_szEmpty);
    PECMD_StrDupAssign((void *)&local_98, g_szEmpty);
    PECMD_StrDupAssign(&local_78, g_szEmpty);
    uVar16 = 0;
    local_80 = 0;
    local_88 = 0;
    local_a8 = 0;
    local_res8 = 0x1000000;
    uVar7 = 0;
    local_70 = param_4;
    if (*pWVar10 == L'*') {             /* '*' 通配标记 */
        local_res10 = pWVar10 + 1;
        PECMD_SkipLeadingControlChars(&local_res10);
        uVar7 = 1;
    }
    PECMD_SplitTokenTrimWs(&local_res10, &local_90, 0x2c);
    PECMD_RunCommandLine(param_1, &local_90, bVar11);
    if (*local_res10 == L',') {
        local_res10 = local_res10 + 1;
        plVar5 = PECMD_SplitTokenAssignVar(&local_60, &local_res10, 0x2c, bVar11);
        PECMD_ParseLtwhParams(plVar5, (uint32_t *)&local_80, &local_88, &local_a8,
                      (uint32_t *)&local_res8);
        if (*local_res10 == L',') {
            local_res10 = local_res10 + 1;
            plVar5 = PECMD_SplitTokenAssignVar(&local_60, &local_res10, 0x2c, bVar11);
            PECMD_ExtractTokenByDelim(plVar5, (void *)&local_a0, 0);
            if (*local_res10 == L',') {
                local_res10 = local_res10 + 1;
                PECMD_CopyUpToChar(&local_res10, &local_78, 0x2c);
                if (*local_res10 == L',') {
                    local_res10 = local_res10 + 1;
                    plVar5 = PECMD_SplitTokenAssignVar(&local_60, &local_res10, 0x2c, bVar11);
                    PECMD_ExtractTokenByDelim(plVar5, (void *)&local_98, 0);
                    if (*local_res10 == L',') {
                        local_res10 = local_res10 + 1;
                        puVar6 = PECMD_SplitTokenAssignVar(&local_60, &local_res10, 0x2c, bVar11);
                        uVar7 = (uint32_t)PECMD_ParseSignedNumber((short *)*puVar6);
                        uVar16 = uVar7;
                    }
                }
            }
        }
        if ((char)uVar7 == '\0') {
            param_1 = *(longlong **)(param_3 + 0x290);
        }
        FUN_1400B2B64(param_3, (int64_t)param_1, (int64_t *)&local_90, local_80,
                      local_88, local_a8, local_res8, (int64_t *)&local_78,
                      local_a0, local_98, uVar16 | (uint32_t)local_res18,
                      '\0', (int)uVar15);
        FUN_14005b104(&local_78);
        FUN_14005b104((void *)&local_98);
        FUN_14005b104((void *)&local_a0);
        FUN_14005b104(&local_90);
    } else {
        FUN_14005b104(&local_78);
        FUN_14005b104((void *)&local_98);
        FUN_14005b104((void *)&local_a0);
        FUN_14005b104(&local_90);
        return 1;
    }
    FUN_14005b104(&local_60);
    return 0;
}

/* ========== PECMD_AddControlVariantB @ 1400b50e0 ==========
 * signature: undefined8 __fastcall PECMD_AddControlVariantB(longlong * param_1,
 *   WCHAR * param_2, longlong param_3, undefined8 param_4)
 * 添加控件（变体 B），支持 '$'/'*' 前缀与 '-del'-系列。
 */
uint64_t PECMD_AddControlVariantB(longlong *param_1, WCHAR *param_2, longlong param_3,
                       undefined8 param_4)
{
    WCHAR WVar9;
    char cVar6;
    WCHAR *pWVar2;
    WCHAR *local_res10;
    int local_48;
    LPCSTR local_40;
    undefined8 local_38;
    ulonglong uVar3;
    bool bVar10;
    int iVar5;
    int iVar8;

    WVar9 = L'*';
    cVar6 = '$';
    local_48 = 0;
    local_40 = NULL;
    local_38 = param_4;
    (void)local_38;
    if (*param_2 == L'*') {
        cVar6 = '\0';
    } else {
        if (*param_2 != L'$') goto LAB_1400b5137;
        cVar6 = (char)*param_2;
    }
    param_2 = param_2 + 1;
LAB_1400b5137:
    local_res10 = param_2;
    {
        WCHAR WVar1 = *param_2;
        while ((WVar1 != L'\0') &&
               ((((ushort)WVar1 < 9 || (0xd < (ushort)WVar1)) && (WVar1 != L' ')))) {
            local_res10 = local_res10 + 1;
            WVar1 = *local_res10;
        }
    }
    if (param_3 == 0) {
        PECMD_ResetScriptChain(param_1, (void *)0);
        param_3 = param_1[8];
        if (param_3 == 0) {
            return (uint64_t)0xffffffff80070057;
        }
    }
    PECMD_SkipLeadingControlChars(&local_res10);
    pWVar2 = local_res10;
    if (*local_res10 != L'\0') {
        bVar10 = (*pWVar2 == L'*');
        if (bVar10) {
            local_res10 = pWVar2 + 1;
            PECMD_SkipLeadingControlChars(&local_res10);
        }
        uVar3 = PECMD_ParseHotkeyCode((int64_t *)&local_res10, (uint32_t *)&local_48,
                              (uint64_t)param_1, '\0');
        pWVar2 = local_res10;
        PECMD_StrDupAssign(&local_40, local_res10);
        if (*pWVar2 == L'-') {
            WVar9 = L'-';
        }
        iVar8 = 0;
        iVar5 = (int)uVar3;
        if (((WVar9 == *pWVar2) && (pWVar2[1] != L'\0')) &&
            (PECMD_AsciiWideICmp("del", pWVar2 + 2) != iVar8)) {
            PECMD_RemoveObjectByType(param_3, iVar5, local_48, (int)cVar6);
        } else if ((*pWVar2 != (WCHAR)iVar8) && (iVar8 < iVar5)) {
            if (bVar10 == false) {
                param_1 = *(longlong **)(param_3 + 0x290);
            }
            FUN_1400B1DEC(param_3, (int64_t)param_1, (int64_t *)&local_40, iVar5,
                          (uint32_t)local_48, (int)cVar6);
        }
        FUN_14005b104(&local_40);
    }
    return 0;
}

/* ========== PECMD_SetControlCommandTrigger @ 1400b6240 ==========
 * signature: longlong __fastcall PECMD_SetControlCommandTrigger(LARGE_INTEGER param_1,
 *   short * param_2, LPCWSTR param_3)
 * 设置控件命令字符串并触发脚本执行循环（控件槽填充）。
 * TODO(verify): param_1 按值传入但按基址使用（Ghidra 大指针寄存器拼接），
 *   此处以 param_1.QuadPart 为基地址进行字段访问。
 */
int64_t PECMD_SetControlCommandTrigger(LARGE_INTEGER param_1, short *param_2, LPCWSTR param_3)
{
    int64_t lVar5;
    LPCWSTR lpString;
    WCHAR *local_res10;
    int64_t *base;
    uint64_t uVar1;
    uint64_t uVar2;
    uint64_t uVar3;
    int iVar4;

    lVar5 = 0;
    local_res10 = NULL;
    FUN_1400A4020(&local_res10, (LPCWSTR)param_2);
    if (*param_2 != 0) {
        base = (int64_t *)(uintptr_t)param_1.QuadPart;
        uVar1 = *(uint64_t *)((char *)base + 0x78);
        uVar2 = *(uint64_t *)((char *)base + 0x80);
        *(void ***)((char *)base + 0x78) = (void **)&local_res10;
        lpString = (LPCWSTR)local_res10;
        *(LPCWSTR *)((char *)base + 0x80) = lpString;
        PECMD_ArgTokenize((int64_t *)(uintptr_t)param_1.QuadPart, param_3, 1);
        iVar4 = lstrlenW(lpString);
        lVar5 = *(int64_t *)((char *)base + 0x40);
        *(uint8_t *)((char *)base + 0xe) = 0;
        *(int *)((char *)base + 0x8c) = iVar4;
        if (lVar5 == 0) {
            PECMD_DispatchExpressionBlock((void *)base, lpString);
        } else {
            *(uint32_t *)(lVar5 + 0x148) = 1;
            *(uint32_t *)(lVar5 + 0x144) = 1;
            *(int32_t *)(lVar5 + 0x140) = -10;
            *(int32_t *)(lVar5 + 0x13c) = -10;
            *(LARGE_INTEGER *)(lVar5 + 0x290) = param_1;
            **(LARGE_INTEGER **)(lVar5 + 0x200) = param_1;
            uVar3 = *(uint64_t *)((char *)base + 0x80);
            *(uint8_t *)(lVar5 + 0x122) = 1;
            *(uint8_t *)(lVar5 + 0x159) = 1;
            *(uint8_t *)(lVar5 + 0x120) = 0x30;
            *(uint64_t *)(lVar5 + 0x2a0) = uVar3;
            *(uint8_t *)(lVar5 + 0x121) = 0x81;
            PECMD_DispatchExpressionBlock((void *)base, lpString);
            *(uint8_t *)(lVar5 + 0x121) = 0x81;
            *(uint64_t *)(lVar5 + 0x290) = 0;
        }
        g_dblClickFlag = 0;
        *(uint16_t *)((char *)base + 200) = 0;
        lVar5 = (int64_t)(*(int *)((char *)base + 0xd0));
        *(uint64_t *)((char *)base + 0x78) = uVar1;
        *(uint64_t *)((char *)base + 0x80) = uVar2;
    }
    PECMD_RefCountRelease(&local_res10);
    return lVar5;
}

/* ========== PECMD_AddTransControl @ 1400b9788 ==========
 * signature: undefined8 __fastcall PECMD_AddTransControl(longlong * param_1,
 *   ushort * param_2, WPARAM param_3, undefined8 param_4)
 * 添加控件（变体 D，宽参数：位置/颜色/文本 5 元组）。
 * TODO(verify): 原伪码用寄存器拼接把 '*' 标记的低字节塞进 local_b0 槽；
 *   此处以 bool bStar 表示（语义等价，便于验证）。
 */
uint64_t PECMD_AddTransControl(longlong *param_1, ushort *param_2, WPARAM param_3,
                       undefined8 param_4)
{
    ushort uVar1;
    WCHAR *puVar20;
    WCHAR *puVar5;
    int16_t sVar6;
    int64_t *plVar8;
    uint32_t uVar16;
    uint32_t uVar23;
    uint32_t uVar17;
    uint32_t uVar26;
    uint32_t uVar18;
    uint32_t uVar13;
    WPARAM local_res18;
    WCHAR *local_res10;
    WCHAR *local_b0;
    LPCWSTR local_90;
    LPCWSTR local_68;
    LPCWSTR local_70;
    WCHAR *local_50;
    int local_a8;
    int local_a4;
    int local_a0;
    int local_9c;
    int local_98;
    int local_84;
    int local_88;
    int local_80;
    const WCHAR *local_78;
    longlong *local_48;
    undefined8 local_40;
    bool bVar2;
    bool bVar3;
    bool bStar;
    int iVar11, iVar24, iVar21, iVar14;
    int iVar12, iVar25, iVar22, iVar15;

    uVar26 = 0;
    local_res10 = (WCHAR *)param_2;
    local_res18 = param_3;
    if (param_3 == 0) {
        PECMD_ResetScriptChain(param_1, (void *)0);
        local_res18 = param_1[8];
        if (local_res18 == 0) {
            return (uint64_t)0xffffffff80070057;
        }
    }
    plVar8 = (int64_t *)&local_50;
    PECMD_AllocStrSlot(plVar8);
    local_40 = 0;
    uVar23 = 0;
    bVar2 = false;
    bVar3 = false;
    uVar18 = 1;
    uVar13 = 0;
    bStar = false;
    uVar17 = 0;
    uVar1 = *param_2;
    while ((uVar16 = uVar17, uVar1 != 0) &&
           ((local_res10 = (WCHAR *)param_2, (uVar1 < 9 || (0xd < uVar1))) &&
            (uVar1 != 0x20))) {
        if (uVar1 == 0x2d) {          /* '-' */
            uVar17 = uVar17 | 0x8000000;
        } else if (uVar1 == 0x7c) {   /* '|' */
            uVar17 = uVar17 | 0x10000000;
        } else if (uVar1 == 0x2b) {   /* '+' */
            uVar17 = uVar17 | 0x4000000;
        } else if (uVar1 == 0x2e) {   /* '.' */
            bVar2 = true;
        } else if (uVar1 == 0x3e) {   /* '>' */
            bVar3 = true;
        } else {
            uVar13 = 0;
            if (uVar1 == 0x2a) {      /* '*' */
                uVar13 = uVar18;
                bStar = true;
            }
        }
        param_2 = param_2 + 1;
        local_res10 = (WCHAR *)param_2;
        uVar1 = *param_2;
    }
    uVar16 = uVar17;
    local_b0 = NULL;
    local_48 = param_1;
    (void)local_48;
    PECMD_SkipLeadingControlChars(&local_res10);
    puVar20 = local_res10;
    puVar5 = puVar20;
    (void)puVar5;
    /* 选项区（以 '-' 开头） */
    {
        uVar13 = 0;
        uVar1 = *puVar20;
        if ((WCHAR)0x2d == uVar1) {
            do {
                int64_t lVar7;
                lVar7 = FUN_14005c72c("-center", puVar20, 7);
                if ((char)lVar7 == '\0') {
                    lVar7 = FUN_14005c72c("-right", puVar20, 6);
                    if ((char)lVar7 == '\0') {
                        lVar7 = FUN_14005c72c("-left", puVar20, 5);
                        if ((char)lVar7 == '\0') {
                            lVar7 = FUN_14005c72c("-trans", puVar20, 6);
                            uVar23 = uVar17;
                            if ((char)lVar7 == '\0') {
                                lVar7 = FUN_14005c72c("-w", puVar20, 2);
                                if ((char)lVar7 == '\0') {
                                    lVar7 = FUN_14005c72c("-wx", puVar20, 3);
                                    if ((char)lVar7 == '\0') {
                                        lVar7 = FUN_14005c72c("-nf", puVar20, 3);
                                        if ((char)lVar7 == '\0') {
                                            lVar7 = FUN_14005c72c("-vcenter", puVar20, 8);
                                            if ((char)lVar7 == '\0') {
                                                lVar7 = FUN_14005c72c("-ncmd", puVar20, 5);
                                                if ((char)lVar7 == '\0') {
                                                    lVar7 = FUN_14005c72c("-3D", puVar20, 3);
                                                    if ((char)lVar7 == '\0') {
                                                        lVar7 = FUN_14005c72c("-mod", puVar20, 4);
                                                        if ((char)lVar7 != '\0') {
                                                            uVar17 = uVar23 | 0x400000;
                                                        }
                                                    } else {
                                                        uVar17 = uVar23 | 0x200000;
                                                    }
                                                } else {
                                                    uVar17 = uVar23 | 0x40;
                                                }
                                            } else {
                                                uVar17 = uVar23 | 0x100;
                                            }
                                        } else {
                                            uVar17 = uVar23 | 0x1000000;
                                        }
                                    } else {
                                        uVar17 = (uVar23 & 0xffffff7f) | 0x20;
                                    }
                                } else {
                                    uVar17 = uVar23 | 0x80;
                                }
                            } else {
                                uVar17 = uVar23 | 0x2000000;
                            }
                        } else {
                            uVar13 = 0x80000000;
                        }
                    } else {
                        uVar13 = 0x20000000;
                    }
                } else {
                    uVar13 = 0x40000000;
                }
                uVar16 = uVar17;
                uVar23 = uVar13;
                while ((uVar1 != 0) && (((ushort)uVar1 < 9 || (0xd < (ushort)uVar1)) &&
                                        (uVar1 != 0x20))) {
                    puVar20 = puVar20 + 1;
                    local_res10 = puVar20;
                    uVar1 = *puVar20;
                }
                PECMD_SkipLeadingControlChars(&local_res10);
                uVar1 = *local_res10;
                puVar20 = local_res10;
            } while ((WCHAR)0x2d == uVar1);
            uVar18 = 1;
        }
    }
    uVar16 = uVar16 | uVar23;
    if (*puVar20 == 0x2a) {             /* 末尾 '*' 通配标记 */
        local_res10 = puVar20 + 1;
        PECMD_SkipLeadingControlChars(&local_res10);
        uVar26 = uVar18;
    }
    PECMD_StrDupAssign(&local_78, g_szEmpty);
    PECMD_StrDupAssign(&local_90, g_szEmpty);
    PECMD_AllocStrSlot(&local_70);
    PECMD_StrDupAssign(&local_68, g_szEmpty);
    local_98 = 0;
    local_84 = 0;
    local_88 = 0;
    local_80 = 0;
    local_a8 = -0x80000000;
    iVar11 = -0x80000000;
    iVar24 = -0x80000000;
    local_a4 = -0x80000000;
    iVar21 = -0x80000000;
    local_a0 = -0x80000000;
    iVar14 = -0x80000000;
    local_9c = -0x80000000;
    local_40 = param_4;
    (void)local_40;
    PECMD_SplitTokenTrimWs(&local_res10, &local_78, 0x2c);
    PECMD_RunCommandLine(param_1, &local_78, 1);
    iVar12 = -0x80000000;
    iVar25 = -0x80000000;
    iVar22 = -0x80000000;
    iVar15 = -0x80000000;
    if (*local_res10 == 0x2c) {
        local_res10 = local_res10 + 1;
        plVar8 = PECMD_SplitTokenAssignVar(&local_50, &local_res10, 0x2c, 1);
        PECMD_ParseLtwhParams(plVar8, (uint32_t *)&local_98, &local_84, &local_88,
                      (uint32_t *)&local_80);
        iVar12 = iVar11;
        iVar25 = iVar24;
        iVar22 = iVar21;
        iVar15 = iVar14;
        if (*local_res10 == 0x2c) {
            local_res10 = local_res10 + 1;
            plVar8 = PECMD_SplitTokenAssignVar(&local_50, &local_res10, 0x2c, 1);
            PECMD_ExtractTokenByDelim(plVar8, (void *)&local_90, 0);
            if (*local_res10 == 0x2c) {
                local_res10 = local_res10 + 1;
                PECMD_CopyUpToChar(&local_res10, &local_68, 0x2c);
                if (*local_res10 == 0x2c) {
                    local_res10 = local_res10 + 1;
                    plVar8 = PECMD_SplitTokenAssignVar(&local_50, &local_res10, 0x2c, 1);
                    sVar6 = FUN_1400677B0(plVar8, (int64_t)&local_a8);
                    iVar12 = local_a4;
                    iVar25 = local_a8;
                    iVar22 = local_a0;
                    if (sVar6 < 3) {
                        iVar22 = local_a8;
                    }
                    iVar15 = local_9c;
                    if (sVar6 < 4) {
                        iVar15 = local_a4;
                    }
                    local_a0 = iVar22;
                    local_9c = iVar15;
                    if (*local_res10 == 0x2c) {
                        local_res10 = local_res10 + 1;
                        plVar8 = PECMD_SplitTokenAssignVar(&local_50, &local_res10, 0x2c, 1);
                        PECMD_ExtractTokenByDelim(plVar8, (void *)&local_70, 0);
                    }
                }
            }
        }
    }
    if (bStar) {
        WCHAR *pWVar4;
        PECMD_AllocStrSlot(&local_b0);
        pWVar4 = (WCHAR *)local_68;
        PECMD_ExpandVarDispatch(param_1, local_68, (int64_t *)&local_b0, 0, 1);
        local_68 = local_b0;
        local_b0 = pWVar4;
        FUN_14005b104(&local_b0);
    }
    if (!bVar2) {
        PECMD_ExpandBackslashNewline(local_90, '\0');
    }
    if (bVar3) {
        uVar16 = uVar16 | 0x1000000;
    }
    FUN_140067F90((int64_t *)&local_90);
    if (((-1 < iVar25) || (-1 < iVar12)) || ((-1 < iVar22) || (-1 < iVar15))) {
        local_a4 = iVar12;
        if (iVar12 == INT_MIN) {
            local_a4 = g_helpVerFlag;
        }
        local_9c = iVar15;
        if (iVar15 == INT_MIN) {
            local_9c = g_helpVerFlag;
        }
    }
    if ((char)uVar26 == '\0') {
        param_1 = *(longlong **)(local_res18 + 0x290);
    }
    FUN_1400B9204(local_res18, (int64_t)param_1, (int64_t *)&local_78, local_98,
                  local_84, local_88, local_80, (int64_t *)&local_90,
                  (int64_t *)&local_68, (int64_t *)&local_a8, local_70, uVar16);
    FUN_14005b104((void *)&local_68);
    FUN_14005b104((void *)&local_70);
    FUN_14005b104(&local_90);
    FUN_14005b104(&local_78);
    FUN_14005b104(&local_50);
    return 0;
}
