/* ========== PECMD_CalcExpression @0x1400bf358 忠实移植 (dc:118318-119158) ========== */
/* ====================================================================
 * core_calc_expr.c — CALC 表达式主驱动器 @0x1400bf358
 *
 * 来源: PECMD原始.EXE (x64, ImageBase=0x140000000)
 *   逐语句移植 dc FUN_1400bf358 (reference/big_funcs/1400bf358_FUN_1400bf358.c,
 *   即 reference/decompiled.c 118318..119158), 替换 core_b3_remaining.c:28361
 *   的"重构/简化版"PECMD_CalcExpression (该版本成功路径返回堆指针, exit=指针低32, 语义失真)。
 *
 * 移植手法 (参照 core_b3_remaining.c:19277 PECMD_ParseExpression 模板):
 *   - pthreadmbcinfo 指针噪声一律 WCHAR* / LPWSTR 化:
 *       ptr->refcount        = 指针首 WCHAR (*(uint16_t*)p / *p);
 *       ptr->mbulinfo[k-6]   = p[k]   (mbulinfo 槽头位于 byte+12 = 6 个 WCHAR, 与 -6/-0xc 抵消);
 *       (longlong)ptr->mbulinfo + (k-0xc) = (uint8_t*)ptr + k (字节运算);
 *       &ptr->refcount + 2   = p + 1;
 *       &ptr[-1].mblocalename + 6 = p - 1 (槽头伪差, 与 ?N 回写 '?' 的 -2 字节一致);
 *   - CONCAT62/44/71 / SUB84 按等价 64 位运算展开 (本文件顶部宏 + calc_d2u/calc_u2d 位重释);
 *   - unaff_R15 / extraout_var 入口寄存器残留按 0 处理 (模板同款惯例);
 *   - LAB_ 标号命名照 dc 原样保留。
 *
 * 语义注记 (主代理用原版二进制活体对拍定案, 照实现):
 *   - 返回值= pWVar6 (求值器标志字节: 成功 0 / 除0 0x10 / 其他错误位), 实测 CALC R=1/0 → exit 16;
 *   - -err=N 仅影响错误文本 local_b0, 不影响返回值;
 *   - 临界区 g_csInit 围住"求值→赋值"整段。
 * ==================================================================== */
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "pecmd_defs.h"

/* Ghidra 遗留寄存器拼接宏 (参照 core_b3_remaining.c 顶部同族宏) */
#ifndef CONCAT44
#define CONCAT44(hi, lo) (((uint64_t)(uint32_t)(hi)) << 32 | (uint32_t)(lo))
#endif
#ifndef CONCAT62
#define CONCAT62(hi, lo) ((((uint64_t)(hi) & 0xFFFFFFFFFFFFULL) << 16) | (uint16_t)(lo))
#endif
#ifndef CONCAT71
#define CONCAT71(hi, lo) (((int64_t)(hi)) << 8 | (uint8_t)(lo))
#endif
#ifndef CONCAT22
#define CONCAT22(hi, lo) (((uint64_t)(uint32_t)(hi)) << 16 | (uint16_t)(lo))
#endif

/* IEEE754 double <-> uint64 位模式互转 (SUB84(dVar,0)/CONCAT44 拼接的 double 位重释, 参照
 * core_b3_remaining.c 的 b3_d2u/b3_u2d) */
static uint64_t calc_d2u(double d)
{
    uint64_t u;
    memcpy(&u, &d, 8);
    return u;
}
static double calc_u2d(uint64_t u)
{
    double d;
    memcpy(&d, &u, 8);
    return d;
}

/* ---- 局部静态常量 (.rdata 实测: 0x140129848 起 "+txt\0+icon1 0\0" / 0x140129810 起 "+cb\0") ---- */
static WCHAR dt_129848[] = L"+txt+icon1 0"; /* DAT_140129848: +0 "+txt", +4 WCHAR 起 "+icon1 0" */
static WCHAR dt_129810[] = L"+cb";          /* DAT_140129810 */

/* ---- 外部 helper (签名以各自定义处为准; 与 xproto.h 重复声明允许, 类型必须一致) ---- */

/* 全局数据 */
extern WCHAR g_szEmpty[];                                /* DAT_14011c638 .rdata 空串 (core_globals.c:1114) */
extern double g_fontMinus0;                              /* DAT_140125238 -0.0 (core_globals.c) */
/* g_csInit 已由 pecmd_defs.h 提供 (extern CRITICAL_SECTION g_csInit; DAT_14013e190) */

/* 字符串槽工具 (src/kernel/core_string.c) */
extern WCHAR **FUN_14005B154(WCHAR **pp);                /* @0x14005b154 跳过空白 */
extern void FUN_1400702B0(WCHAR **ps, LPCWSTR src);      /* @0x1400702b0 赋值(不释放旧值) */
extern WCHAR *FUN_14006375C(WCHAR **ps, LPCWSTR src);    /* @0x14006375c 追加 */
extern WCHAR *PECMD_AllocString(WCHAR **ps, int64_t count);   /* @0x140063720 保证容量 */
extern void PECMD_FreeStrBuf(WCHAR **ps);                /* @0x14005b104 释放 */

/* 其余 helper */
extern void PECMD_AllocWStringBuffer(WCHAR **ps, int64_t count);    /* @0x140063694 core_var.c */
extern uint16_t *PECMD_AllocStrSlot(uint16_t **out);                /* @0x140063620 restored_bodies.c */
extern LPCWSTR PECMD_StripTrailingSpaces(LPCWSTR s);                /* @0x140018b70 core_b1_remaining.c */
extern bool PECMD_MatchAndAdvance(char *key, int64_t *pp, int len); /* @0x1400661e4 core_b3d.c */
extern void PECMD_SplitTokenTrimWs(WCHAR **src, WCHAR **dst, int16_t delim); /* @0x1400675b8 core_remaining_helpers.c */
extern bool PECMD_ParseUIntValue(WCHAR **pp, int *out);              /* @0x140074838 core_scriptdep.c (def 为 bool) */
extern uint64_t PECMD_EvalParenStripped(int64_t *param_1, uint64_t *param_2); /* @0x1400745c8 restored_bodies.c */
extern uint64_t PECMD_ParseAndSkipSpace_7b54(int64_t *pp, double *out);  /* @0x140067b54 core_b3b.c */
extern uint64_t PECMD_EvalExprSkipOneChar(int64_t *pp, uint64_t *out);   /* @0x1400a9a84 core_remaining_helpers.c */
extern WCHAR **PECMD_FormatDoubleToStr(WCHAR **out, double value, LPCWSTR fmt, uint32_t prec, char trim,
                                       char mode);                      /* @0x1400e69ac core_b8m.c */
extern WCHAR *PECMD_SprintfRetEnd(WCHAR *dst, uint64_t v, LPCWSTR fmt); /* @0x1400e6d38 core_strbld.c */
extern uint8_t *PECMD_MemMoveForward(uint8_t *a, uint8_t *b, int n);    /* @0x14001d78c restored_bodies.c 前向 memmove */
extern uint64_t PECMD_MessShowMsgBox(int64_t *param_1, WCHAR *param_2, int64_t *param_3); /* @0x1400bb718 core_b3_remaining.c */
extern void *PECMD_VarLookup(void *script, LPCWSTR name, void *scope, int namelen, void **found); /* @0x140018978 core_var2.c */
extern uint16_t *PECMD_ExpandPercentD(uint16_t *param_1, WCHAR *param_2); /* @0x14005eefc restored_bodies.c */
extern LPWSTR PECMD_ExpandStringMarkers(WCHAR *param_1, WCHAR *param_2);  /* @0x14005efac core_b3_remaining.c */
extern int64_t *PECMD_ReplaceStringSlot(int64_t *ps, uint64_t *src);     /* @0x140070398 core_b3e.c */
extern WCHAR **FUN_14007034C(WCHAR **ps, LPCWSTR src);                   /* @0x14007034c core_scriptdep.c (带头串赋值) */
extern uint64_t *PECMD_CopyStrToSlot(uint64_t *dst, uint64_t *src);      /* @0x140070310 core_b3a.c */
extern uint64_t PECMD_IsIdentifierChar(uint16_t ch);                     /* @0x140060b24 core_b3d.c */
extern int64_t FUN_14007BDA8(void *script, WCHAR *line, WCHAR **out, int mode, uint8_t opt); /* @0x14007bda8 core_execline.c */
extern int64_t FUN_14007A224(void *script, WCHAR *line, WCHAR **out, int mode, uint8_t opt); /* @0x14007a224 core_execline.c */
extern uint64_t PECMD_LexMathExpression(uint16_t *param_1);             /* @0x140065140 core_b3_remaining.c */
extern uint64_t PECMD_ParsePathRecord2(LPWSTR path, uint8_t *flags);    /* @0x14007efa4 core_b3h.c */
extern uint64_t PECMD_ParsePathRecord(LPWSTR path, uint8_t *flags);     /* @0x14007f018 core_b3f.c */
extern void *PECMD_StrBldCopyWide(void *a, const WCHAR *b);             /* @0x1400703e4 restored_bodies.c */
extern int PECMD_ParseNumTryWriteback(long long *pp, int *out);         /* @0x140067d20 restored_bodies.c */
extern bool PECMD_ParseNumSkipWs(WCHAR **pp, uint64_t *out);            /* @0x140067cf4 core_remaining_helpers.c */
extern uint64_t thunk_FUN_1400f429c(void *a, short b); /* 既有跳板 (unimplemented_stubs.c 桩; 原版
                                                        * 0x1400f429c: 将 *a 推进至分界符 b 或串尾) */
extern bool PECMD_ParseHexOrDecBool(long long *param_1, int *param_2);  /* @0x1400c11c0 restored_bodies.c */
extern int64_t PECMD_RunCommand(void *script, WCHAR *cmdline);          /* @0x140031454 core_scriptrun.c */
extern void FUN_1400629B8(void *script, LPCWSTR key, LPCWSTR value);    /* @0x1400629b8 core_var.c 变量写 */

/* ========== PECMD_CalcExpression @0x1400bf358 ========== */
/*
 * CALC 表达式主驱动器: 解析 -txt/-cb/-u/-gui/-base=/-err=/-[ 前缀选项,
 * 全角→半角归一化, 逐语句求值 (数值路径 PECMD_ParsePathRecord2 / 表达式路径
 * PECMD_ParsePathRecord), 结果按 base/格式写回变量名槽并组装 "+txt+cb 值" 文本。
 * param_1 = 脚本上下文, param_2 = 命令行文本 (PSB 传 LVar25+8), param_3 = 输出槽 (PSB 传 NULL)。
 * 返回值 = pWVar6 求值器标志 (成功 0 / 除0 0x10 / 其他错误位); param_3 非空时另写 64 位数值。
 */
LPWSTR PECMD_CalcExpression(int64_t *param_1, WCHAR *param_2, uint64_t *param_3)
{
    bool bVar1;
    int iVar2;
    int iVar3;
    int iVar4;
    LPWSTR pWVar5;
    LPWSTR pWVar6;
    LPWSTR pWVar7;
    uint64_t uVar8;
    WCHAR **plVar9;
    uint64_t uVar10;
    WCHAR WVar11;
    WCHAR **pptVar12;
    int16_t sVar13;
    WCHAR *puVar14;
    char cVar15;
    WCHAR *ptVar16;
    WCHAR *ptVar17;
    WCHAR *pWVar18;
    char cVar19;
    WCHAR *ptVar20;
    WCHAR *puVar21;
    WCHAR *ptVar22;
    LPCWSTR pwVar23;
    int64_t lVar24;
    WCHAR *ptVar25;
    WCHAR *ptVar26;
    WCHAR *ptVar27;
    WCHAR *ptVar28;
    LPCWSTR pWVar29;
    WCHAR *ptVar30;
    int64_t lVar31;
    LPCWSTR pWVar32;
    uint16_t uVar33;
    uint16_t uVar34;
    uint64_t unaff_R15;
    uint64_t uVar35;
    double dVar36;
    uint32_t uVar37;
    uint32_t uVar38;
    uint32_t uVar39;
    uint32_t uVar40;
    uint64_t extraout_var;
    WCHAR *local_res10;
    uint64_t *local_res18;
    WCHAR *local_res20;
    WCHAR *local_188;
    uint32_t local_180;
    int local_17c;
    WCHAR *local_178;
    int local_170;
    uint32_t local_16c;
    WCHAR *local_168;
    int local_160[2];
    WCHAR *local_158;
    WCHAR *local_150;
    WCHAR *local_148;
    int local_140;
    int local_13c;
    WCHAR *local_138;
    LPWSTR local_130;
    WCHAR *local_128;
    WCHAR *local_120;
    LPWSTR local_118;
    int local_110;
    WCHAR *local_108;
    uint64_t local_100;
    WCHAR *local_f8;
    WCHAR *local_f0;
    WCHAR *local_e8;
    WCHAR *local_e0;
    int64_t local_d8;
    WCHAR *local_d0;
    WCHAR *local_c8;
    WCHAR *local_c0;
    WCHAR *local_b8;
    LPWSTR local_b0;
    WCHAR *local_a8;
    WCHAR *local_a0;
    WCHAR *local_98;
    WCHAR *local_90;
    const WCHAR *local_88;
    int64_t local_80[8];

    pWVar5 = (LPWSTR)0x0;
    uVar10 = 1;
    lVar31 = -1;
    local_120 = (WCHAR *)0x0;
    local_e0 = (WCHAR *)0x0;
    local_100 = 1;
    local_e8 = (WCHAR *)0x0;
    local_128 = (WCHAR *)0x0;
    local_res20 = (WCHAR *)0x0;
    local_d8 = -1;
    local_res10 = param_2;
    local_res18 = param_3;
    FUN_14005B154(&local_res10);
    local_f0 = (WCHAR *)0x0;
    FUN_1400702B0(&local_138, g_szEmpty);
    FUN_1400702B0((WCHAR **)local_80, g_szEmpty); /* dc: FUN_1400702b0(local_80, ...) 仅第 0 槽 */
    FUN_1400702B0(&local_90, g_szEmpty);
    uVar39 = 0;
    uVar40 = 0;
    local_150 = (WCHAR *)0x0;
    uVar33 = 0;
    unaff_R15 = 0; /* 反编译寄存器残留 (调用方 R15 入口值), 按 0 处理 */
    uVar35 = unaff_R15 & 0xffffffffffff0000ULL;
    extraout_var = 0; /* 反编译寄存器残留 (CONCAT71 高位), 按 0 处理 (模板同款) */
    local_17c = 0;
    local_140 = 0;
    local_170 = 0;
    local_b0 = (LPWSTR)0x0;
    local_180 = (uint32_t)uVar35;
    pWVar6 = pWVar5;
    if ((int16_t)*local_res10 == 0x2d) {
        do {
            pWVar7 = (LPWSTR)0x1;
            iVar2 = (int)(uintptr_t)pWVar6;
            bVar1 = PECMD_MatchAndAdvance("-txt", (int64_t *)&local_res10, 4);
            if (bVar1) {
                local_17c = 4;
                pWVar7 = pWVar6;
            LAB_1400bf694:
                FUN_14005B154(&local_res10);
                pWVar6 = pWVar7;
            }
            else {
                bVar1 = PECMD_MatchAndAdvance("-cb", (int64_t *)&local_res10, 3);
                if (bVar1) {
                    local_140 = 3;
                    pWVar7 = pWVar6;
                    goto LAB_1400bf694;
                }
                bVar1 = PECMD_MatchAndAdvance("-u", (int64_t *)&local_res10, 2);
                if (bVar1) {
                    uVar35 = 0xa3;
                    local_180 = 0xa3;
                    pWVar7 = pWVar6;
                    goto LAB_1400bf694;
                }
                bVar1 = PECMD_MatchAndAdvance("-gui", (int64_t *)&local_res10, 4);
                if (bVar1) {
                    goto LAB_1400bf694;
                }
                bVar1 = PECMD_MatchAndAdvance("-base=", (int64_t *)&local_res10, 6);
                if (bVar1) {
                    if ((uint16_t)((uint16_t)*local_res10 | 0x20) == 0x75) { /* 'u' → 无符号 */
                        local_res10 = local_res10 + 1;
                        uVar35 = CONCAT62((int64_t)(uVar35 >> 0x10), 0xa3);
                        local_180 = (uint32_t)uVar35;
                    }
                    local_170 = 10;
                    PECMD_ParseUIntValue((WCHAR **)&local_res10, &local_170);
                }
                else {
                    bVar1 = PECMD_MatchAndAdvance("-err=", (int64_t *)&local_res10, 5);
                    if (!bVar1) {
                        bVar1 = PECMD_MatchAndAdvance("-[", (int64_t *)&local_res10, 2);
                        if (bVar1) {
                            PECMD_EvalExprSkipOneChar((int64_t *)&local_res10, (uint64_t *)&local_128);
                            PECMD_EvalExprSkipOneChar((int64_t *)&local_res10, (uint64_t *)&local_120);
                            ptVar17 = local_128;
                            local_e8 = local_128;
                            local_res20 = local_128;
                            PECMD_EvalExprSkipOneChar((int64_t *)&local_res10, (uint64_t *)&local_res20);
                            PECMD_EvalExprSkipOneChar((int64_t *)&local_res10, &local_100);
                            if ((int64_t)local_100 < 1) {
                                local_100 = 1;
                            }
                            lVar31 = (int64_t)(uintptr_t)local_120 + (1 - (int64_t)(uintptr_t)ptVar17);
                            pWVar7 = pWVar6;
                            uVar10 = local_100;
                            local_d8 = lVar31;
                            goto LAB_1400bf694;
                        }
                        break;
                    }
                    local_f8 = local_res10;
                    PECMD_EvalParenStripped((int64_t *)&local_f8, (uint64_t *)&local_b0);
                    /* local_150 复用为 double 载体: 位模式低32/高32 由 uVar39/uVar40 提前锁存 */
                    PECMD_ParseAndSkipSpace_7b54((int64_t *)&local_res10, (double *)&local_150);
                }
            }
            iVar2 = (int)(uintptr_t)pWVar6;
        } while ((int16_t)*local_res10 == 0x2d);
        ptVar17 = local_res10;
        if (iVar2 != 0) {
            FUN_1400702B0(&local_res10, L"#27:INDATA ");
            FUN_14006375C(&local_res10, ptVar17);
            pWVar5 = (LPWSTR)PECMD_RunCommand(param_1, local_res10);
            pptVar12 = &local_res10;
        LAB_1400c0a74:
            PECMD_FreeStrBuf((WCHAR **)pptVar12);
            goto LAB_1400c0a79;
        }
        /* SUB84(local_150,0) / >>0x20: -err= 解析 double 的位模式半字 (原版走 XMM7 全程保存该 double) */
        {
            uint64_t u150bits;
            memcpy(&u150bits, &local_150, 8); /* local_150 槽内为 ParseAndSkipSpace_7b54 写入的 double 位 */
            uVar39 = (uint32_t)u150bits;
            uVar40 = (uint32_t)(u150bits >> 0x20);
        }
        local_e0 = local_res20;
    }
    if ((int16_t)*local_res10 == 0x23) { /* '#' 强制按表达式路径 */
        uVar33 = (uint16_t)uVar35 | 0x23;
    LAB_1400bf73c:
        uVar35 = CONCAT62((int64_t)(uVar35 >> 0x10), uVar33);
        local_res10 = local_res10 + 1;
        local_180 = (uint32_t)uVar35;
    }
    else if ((int16_t)*local_res10 == 0x24) { /* '$' 同 '#' */
        goto LAB_1400bf73c;
    }
    FUN_14005B154(&local_res10);
    ptVar17 = local_res10;
    if ((int16_t)*local_res10 != 0) {
        PECMD_StripTrailingSpaces(local_res10);
        FUN_1400702B0(&local_150, ptVar17);
        iVar2 = lstrlenW(ptVar17);
        PECMD_AllocString((WCHAR **)&local_150, (int64_t)iVar2 + 10);
        ptVar17 = local_150;
        local_res10 = local_150;
        pWVar6 = StrChrW(local_150, L';');
        if (pWVar6 != (LPWSTR)0x0) {
            *pWVar6 = L'\0';
        }
        /* 变量名/赋值点检测: 首字符非数字非 '(' 非 "-数字", 且存在 '=' → 按 name=expr 切分 */
        if ((((((uint16_t)*ptVar17 < 0x30) || (0x39 < (uint16_t)*ptVar17)) &&
              ((uint16_t)*ptVar17 != 0x28)) &&
             (((uint16_t)*ptVar17 != 0x2d ||
               (9 < (uint16_t)((uint16_t)ptVar17[1] - 0x30))))) &&
            (pWVar7 = StrChrW(ptVar17, L'='), pWVar7 != (LPWSTR)0x0)) {
            if (((pWVar7[1] == L'=') || (pWVar7[-1] == L'>')) || (pWVar7[-1] == L'<')) {
                pWVar7 = pWVar5;
            }
            if (pWVar7 != (LPWSTR)0x0) {
                PECMD_SplitTokenTrimWs((WCHAR **)&local_res10, (WCHAR **)&local_138, 0x3d);
                ptVar17 = local_res10;
            }
        }
        FUN_1400702B0(&local_res20, local_138);
        if ((*(char *)((uint8_t *)param_1 + 0xda) == '\0') &&
            ((*(uint8_t *)((uint8_t *)param_1 + 0xd) & 0xf) == 0)) {
            FUN_14007BDA8(param_1, local_res20, &local_138, 0, 1);
        }
        else {
            FUN_14007A224(param_1, local_res20, &local_138, 0, 1);
        }
        PECMD_FreeStrBuf((WCHAR **)&local_res20);
        if (pWVar6 != (LPWSTR)0x0) {
            *pWVar6 = L';';
        }
        if ((int16_t)*ptVar17 == 0x3d) {
            local_res10 = ptVar17 + 1;
        }
        FUN_14005B154(&local_res10);
        ptVar17 = local_res10;
        local_128 = local_138;
        local_168 = local_res10;
        local_98 = local_res10;
        local_f8 = local_138;
        local_108 = (WCHAR *)0x0;
        local_c0 = (WCHAR *)0x0;
        local_b8 = (WCHAR *)0x0;
        local_13c = 0;
        local_110 = 1;
        ptVar30 = local_138;
        if (lVar31 < 1) {
            goto LAB_1400bfa2d;
        }
        /* ?N 前缀: 变量名以 '?' 开头且后随数字 (对应 -[N] 重复段) */
        if (((int16_t)*local_138 == 0x3f) &&
            (ptVar30 = local_138 + 1, (uint16_t)((uint16_t)*ptVar30 - 0x30) < 10)) {
            local_res10 = ptVar30;
            PECMD_ParseNumTryWriteback((long long *)&local_res10, &local_13c);
            local_138 = local_res10;
            if ((int16_t)*local_res10 == 0x7e) { /* '~' */
                local_138 = local_res10 + 1;
                local_110 = local_13c;
            }
            if (((int16_t)*local_138 == 0x3b) || ((int16_t)*local_138 == 0x3a)) {
                local_138 = local_138 + 1;
            }
            if ((int16_t)*local_138 != 0x3f) {
                /* &local_138[-1].mblocalename + 6 = 当前串前 1 WCHAR (槽头伪差), 补 '?' 前缀 */
                local_138 = local_138 - 1;
                *local_138 = 0x3f;
            }
        }
        local_f8 = local_138;
        iVar2 = lstrlenW(local_138);
        iVar3 = lstrlenW(ptVar17);
        PECMD_AllocString((WCHAR **)&local_108, (int64_t)iVar3 + (int64_t)iVar2 + 0x7d2);
        /* dc: local_b8 = local_108[1].mbctype + iVar2*2 + 0xe2 — [1].mbctype 槽头 = byte+0x240,
         * 即 (iVar2 + 0x191) 个 WCHAR (原版 asm: LEA RDI,[RAX+RBX*2], RBX=len+0x191) */
        local_b8 = local_108 + (iVar2 + 0x191);
        local_c0 = local_108;
    LAB_1400c0948: /* -[ 重复段迭代头 (每次重复重跑 语句展开) */
        ptVar17 = local_b8;
        ptVar30 = local_c0;
        ptVar22 = local_e8;
        local_d8 = lVar31 + -1;
        local_128 = local_c0;
        PECMD_ExpandPercentD((uint16_t *)local_c0, local_f8); /* 展开 %d */
        ptVar20 = local_e0;
        local_168 = ptVar17;
        PECMD_ExpandStringMarkers(ptVar17, (WCHAR *)local_98); /* 展开 %... 标记 */
        /* dc: (longlong)ptVar22->mbulinfo + (uVar10-0xc) — mbulinfo 槽头 12 字节与 -0xc 抵消
         * = ptVar22 + uVar10 (字节); 原版 asm: ADD RBX/R13,R14 (R14=uVar10) */
        local_e8 = (WCHAR *)((uint8_t *)ptVar22 + (size_t)uVar10);
        local_e0 = (WCHAR *)((uint8_t *)ptVar20 + (size_t)uVar10);
    LAB_1400bfa2d:
        PECMD_AllocStrSlot((uint16_t **)&local_148);
        PECMD_AllocStrSlot((uint16_t **)&local_158);
        local_88 = dt_129848 + (4 - local_17c); /* dc: &DAT_140129848 + (4-local_17c)*2 (字节) */
        EnterCriticalSection(&g_csInit);
        local_d0 = local_148;
        puVar21 = local_158;
        ptVar22 = local_148;
        do {
            uVar34 = (uint16_t)uVar35;
            uVar10 = CONCAT62((int64_t)(uVar10 >> 0x10), uVar34);
            /* local_res20 低字节清零: 作求值器标志位载体 */
            local_res20 = (WCHAR *)((uintptr_t)local_res20 & 0xffffffffffffff00ULL);
            local_16c = (uint32_t)uVar10;
            sVar13 = (int16_t)*ptVar17;
            ptVar20 = ptVar17;
            while (sVar13 != 0) { /* 语句切分: ';'/换行/回车 处截断 */
                if (((sVar13 == 0x3b) || (sVar13 == 10)) || (sVar13 == 0xd)) {
                    *ptVar20 = 0;
                    local_res10 = ptVar20 + 1;
                    FUN_14005B154(&local_res10);
                    local_168 = local_res10;
                    ptVar20 = local_res10;
                    break;
                }
                ptVar20 = ptVar20 + 1;
                local_168 = ptVar20;
                sVar13 = (int16_t)*ptVar20;
            }
            if ((*(char *)((uint8_t *)param_1 + 0xda) == '\0') &&
                ((*(uint8_t *)((uint8_t *)param_1 + 0xd) & 0xf) == 0)) {
                FUN_14007BDA8(param_1, ptVar17, &local_f0, 0, 0);
            }
            else {
                FUN_14007A224(param_1, ptVar17, &local_f0, 0, 0);
            }
            pWVar18 = local_f0;
            iVar2 = lstrlenW(local_f0);
            PECMD_AllocWStringBuffer((WCHAR **)&local_178, (int64_t)(iVar2 * 2 + 4));
            WVar11 = *pWVar18;
            ptVar27 = (WCHAR *)0x0;
            lVar31 = 2;
            ptVar17 = local_178;
            if (WVar11 != L'\0') {
                do { /* 全角→半角归一化拷贝 */
                    if ((0x40 < (uint16_t)WVar11) && ((uint16_t)WVar11 < 0x5b)) { /* 0x41-0x5a */
                        WVar11 = WVar11 + L' ';
                        goto LAB_1400bfbaa;
                    }
                    if (((uint16_t)WVar11 < 0xff21) || (0xff3a < (uint16_t)WVar11)) {
                        if (((0xff40 < (uint16_t)WVar11) && ((uint16_t)WVar11 < 0xff5b)) ||
                            ((0xff0f < (uint16_t)WVar11 && ((uint16_t)WVar11 < 0xff1a)))) {
                            sVar13 = 0x120;
                            goto LAB_1400bfbcc;
                        }
                        if (WVar11 == L'＋') {
                            WVar11 = L'+';
                            goto LAB_1400bfbaa;
                        }
                        if ((WVar11 == L'－') || (WVar11 == L'—')) {
                            *ptVar17 = 0x2d;
                            goto LAB_1400bffd0;
                        }
                        if ((WVar11 == L'×') || (WVar11 == L'＊')) {
                            WVar11 = L'*';
                            goto LAB_1400bfbaa;
                        }
                        if (((WVar11 == L'÷') || (WVar11 == L'／')) || (WVar11 == L'∕')) {
                            WVar11 = L'/';
                            goto LAB_1400bfbaa;
                        }
                        if ((WVar11 == L'％') || (WVar11 == L'﹪')) {
                            WVar11 = L'%';
                            goto LAB_1400bfbaa;
                        }
                        if (WVar11 == L'＆') {
                            *ptVar17 = 0x26;
                            goto LAB_1400bffd0;
                        }
                        if ((WVar11 == L'＠') || (WVar11 == L'﹫')) {
                            WVar11 = L'@';
                            goto LAB_1400bfbaa;
                        }
                        if (WVar11 == L'＾') {
                            WVar11 = L'^';
                            goto LAB_1400bfbaa;
                        }
                        if ((WVar11 == L'＝') || (WVar11 == L'﹦')) {
                        LAB_1400bff30:
                            *ptVar17 = 0x3d;
                            goto LAB_1400bffd0;
                        }
                        if (WVar11 == L'＃') {
                            *ptVar17 = 0x23;
                            goto LAB_1400bffd0;
                        }
                        if ((((WVar11 == L'（') || (WVar11 == L'﹙')) || (WVar11 == L'﹝')) ||
                            (WVar11 == L'〔')) {
                            WVar11 = L'(';
                            goto LAB_1400bfbaa;
                        }
                        if (((WVar11 == L'）') || (WVar11 == L'﹚')) ||
                            ((WVar11 == L'﹞' || (WVar11 == L'〕')))) {
                            WVar11 = L')';
                            goto LAB_1400bfbaa;
                        }
                        if ((WVar11 == L'［') || (WVar11 == L'【')) {
                            WVar11 = L'[';
                            goto LAB_1400bfbaa;
                        }
                        if ((WVar11 == L'］') || (WVar11 == L'】')) {
                            WVar11 = L']';
                            goto LAB_1400bfbaa;
                        }
                        if ((WVar11 == L'｛') || (WVar11 == L'﹛')) {
                            WVar11 = L'{';
                            goto LAB_1400bfbaa;
                        }
                        if ((WVar11 == L'｝') || (WVar11 == L'﹜')) {
                            WVar11 = L'}';
                            goto LAB_1400bfbaa;
                        }
                        if (((WVar11 == L'《') || (WVar11 == L'〈')) || (WVar11 == L'＜')) {
                            *ptVar17 = 0x3c;
                        }
                        else {
                            if (((WVar11 != L'》') && (WVar11 != L'〉')) && (WVar11 != L'＞')) {
                                if (WVar11 == L'，') {
                                    WVar11 = L',';
                                }
                                else {
                                    if ((WVar11 != L'．') && (WVar11 != L'。')) {
                                        if (WVar11 == L'；') {
                                            *ptVar17 = 0x3b;
                                        }
                                        else {
                                            if (WVar11 == L'\x3000') { /* 全角空格 */
                                                WVar11 = L' ';
                                                goto LAB_1400bfbaa;
                                            }
                                            if (WVar11 == L'～') {
                                                *ptVar17 = 0x7e;
                                            }
                                            else {
                                                if ((WVar11 == L'！') || (WVar11 == L'﹗')) {
                                                    WVar11 = L'!';
                                                    goto LAB_1400bfbaa;
                                                }
                                                if ((((WVar11 != L'§') && (WVar11 != L'｜')) &&
                                                     (WVar11 != L'|')) &&
                                                    ((WVar11 != L'∣' && (WVar11 != L'￤')))) {
                                                    if ((WVar11 == L'≤') ||
                                                        ((WVar11 == L'≦' || (WVar11 == L'≯')))) {
                                                        *ptVar17 = 0x3c;
                                                    }
                                                    else {
                                                        if (((WVar11 != L'≥') && (WVar11 != L'≧')) &&
                                                            (WVar11 != L'≮')) {
                                                            goto LAB_1400bfbaa;
                                                        }
                                                        *ptVar17 = 0x3e;
                                                    }
                                                    ptVar17 = ptVar17 + 1;
                                                    goto LAB_1400bff30;
                                                }
                                                *ptVar17 = 0x7c;
                                            }
                                        }
                                        goto LAB_1400bffd0;
                                    }
                                    WVar11 = L'.';
                                }
                                goto LAB_1400bfbaa;
                            }
                            *ptVar17 = 0x3e;
                        }
                    }
                    else {
                        sVar13 = 0x140;
                    LAB_1400bfbcc:
                        WVar11 = WVar11 + sVar13;
                    LAB_1400bfbaa:
                        *ptVar17 = WVar11;
                    }
                LAB_1400bffd0:
                    pWVar18 = pWVar18 + 1;
                    ptVar17 = ptVar17 + 1;
                    WVar11 = *pWVar18;
                } while (WVar11 != L'\0');
                uVar10 = (uint64_t)local_16c;
                uVar34 = (uint16_t)local_180;
                ptVar20 = local_168;
                puVar21 = local_158;
                ptVar30 = local_128;
                ptVar22 = local_d0;
            }
            *ptVar17 = 0;
            local_res10 = local_178;
            ptVar28 = ptVar27;
            FUN_14005B154(&local_res10);
            if ((int16_t)*local_res10 == 0x23) { /* '#' */
                uVar10 = CONCAT62((int64_t)(uVar10 >> 0x10), uVar34) | 0x23;
                local_16c = (uint32_t)uVar10;
            LAB_1400c0061:
                /* dc: (longlong)local_res10->mbulinfo + lVar31 + -0xc — 槽头抵消 = +lVar31 字节 (lVar31=2) */
                ptVar16 = (WCHAR *)((uint8_t *)local_res10 + (size_t)lVar31);
            }
            else {
                ptVar16 = local_res10;
                if ((int16_t)*local_res10 == 0x24) { /* '$' */
                    local_16c = (uint32_t)(uintptr_t)ptVar27;
                    uVar10 = (uint64_t)(uintptr_t)ptVar27 & 0xffffffff;
                    goto LAB_1400c0061;
                }
            }
            sVar13 = (int16_t)*ptVar16;
            ptVar25 = ptVar16;
            while (sVar13 == 0x26) { /* 跳过 '&' 前缀 */
                ptVar25 = ptVar25 + 1;
                sVar13 = (int16_t)*ptVar25;
            }
            /* 标识符/变量名扫描: '_'/'$'/小写字母/高位字符 起始 → 定位 '=' 赋值点 */
            if (((((int16_t)*ptVar25 == 0x5f) || ((int16_t)*ptVar25 == 0x24)) ||
                 ((0x60 < (uint16_t)*ptVar25) && ((uint16_t)*ptVar25 < 0x7b))) ||
                (0x7f < (uint16_t)*ptVar25)) {
                ptVar26 = ptVar25 + 1;
                local_res10 = ptVar26;
                uVar8 = PECMD_IsIdentifierChar((uint16_t)*ptVar26);
                ptVar25 = local_res10;
                if ((char)uVar8 != (char)(uintptr_t)ptVar27) { /* ptVar27=0: 非标识符起始 */
                    do {
                        ptVar26 = ptVar26 + 1;
                        uVar8 = PECMD_IsIdentifierChar((uint16_t)*ptVar26);
                        ptVar25 = ptVar26;
                    } while ((char)uVar8 != (char)(uintptr_t)ptVar27);
                }
                local_res10 = ptVar25;
                FUN_14005B154(&local_res10);
                if ((int16_t)*local_res10 == 0x3d) {
                    ptVar17 = local_res10;
                    ptVar28 = ptVar26;
                }
            }
            if ((int16_t)*ptVar17 == 0x3d) {
                *ptVar28 = 0;
                ptVar17 = ptVar17 + 1;
                local_res10 = ptVar16;
            }
            else {
                ptVar17 = ptVar16;
                local_res10 = (WCHAR *)g_szEmpty;
                if ((int16_t)*ptVar20 == (int16_t)(uintptr_t)ptVar27) { /* *ptVar20==0 */
                    local_res10 = ptVar30;
                }
            }
            pWVar5 = (LPWSTR)0x0;
            *puVar21 = 0;
            *ptVar22 = 0;
            local_130 = StrChrW(ptVar17, L'#');
            if (local_130 != (LPWSTR)0x0) {
                *local_130 = L'\0';
                local_130 = local_130 + 1;
            }
            iVar2 = lstrlenW(ptVar17);
            lVar31 = (int64_t)(iVar2 + -1);
            uVar33 = ptVar17[lVar31]; /* dc: ptVar17->mbulinfo[lVar31-6] → p[lVar31] 末字符 */
            if (((uVar33 == 0x2b) || (uVar33 == 0x2d)) ||
                ((uVar33 == 0x2a ||
                  (((((uVar33 == 0x2f || (uVar33 == 0x25)) || (uVar33 == 0x5e)) ||
                     ((uVar33 == 0x40 || (uVar33 == 0x26)))) || (uVar33 == 0x7c)))))) {
                ptVar17[lVar31] = 0; /* 尾运算符截掉 */
            }
            else if (((uVar33 == 0x3d) || (uVar33 == 0x3e)) || (uVar33 == 0x3c)) {
                ptVar17[lVar31 + 1] = 0x30; /* 后缀补 "0" 供求值 */
                ptVar17[lVar31 + 2] = 0;
            }
            PECMD_AllocWStringBuffer((WCHAR **)&local_188, 100);
            *(ptVar17 - 1) = 0; /* dc: *(undefined2*)((longlong)&ptVar17[-1].mblocalename + 6) = 0 (前一 WCHAR) */
            local_118 = (LPWSTR)0x0;
            uVar8 = PECMD_LexMathExpression((uint16_t *)ptVar17);
            cVar19 = '\0';
            uVar33 = (uint16_t)uVar10;
            if ((int)uVar8 < 0) { /* 词法失败 → 错误路径 */
                pWVar6 = (LPWSTR)0xffffffff80070057;
                if (uVar33 != 0) {
                LAB_1400c0241:
                    pWVar5 = local_b0;
                    goto LAB_1400c0251;
                }
            LAB_1400c02bd: /* 数值/错误值 (uVar37/uVar38 由各入口先行赋值) */
                uVar37 = uVar39;
                uVar38 = uVar40;
            LAB_1400c02c1:
                iVar2 = local_170;
                dVar36 = calc_u2d(CONCAT44(uVar38, uVar37)); /* (double)CONCAT44 为位重释 */
                if (local_res18 != (uint64_t *)0x0) {
                    *local_res18 = CONCAT44(uVar38, uVar37);
                    pWVar5 = (LPWSTR)(int64_t)dVar36;
                    PECMD_FreeStrBuf((WCHAR **)&local_188);
                    PECMD_FreeStrBuf((WCHAR **)&local_178);
                    LeaveCriticalSection(&g_csInit);
                    PECMD_FreeStrBuf((WCHAR **)&local_158);
                    PECMD_FreeStrBuf((WCHAR **)&local_148);
                    PECMD_FreeStrBuf((WCHAR **)&local_108);
                    pptVar12 = &local_150;
                    goto LAB_1400c0a74;
                }
                cVar15 = '\0';
                if (1 < local_170) { /* -base= 且 base>1: %f 十进制数字化后按 base 输出 */
                    bVar1 = (calc_u2d(CONCAT44(uVar38, uVar37)) <= 0.0) &&
                            (calc_u2d(CONCAT44(uVar38, uVar37)) != 0.0);
                    if (bVar1) {
                        dVar36 = g_fontMinus0 - calc_u2d(CONCAT44(uVar38, uVar37));
                    }
                    PECMD_FormatDoubleToStr((WCHAR **)&local_c8, dVar36, (LPCWSTR)0x0, 0, '\0', 'f');
                    puVar21 = local_158;
                    for (puVar14 = local_c8; (uint16_t)(*puVar14 - 0x30) < 10; puVar14 = puVar14 + 1) {
                        pWVar5 = (LPWSTR)((int64_t)(int)(*puVar14 - 0x30) + (int64_t)(uintptr_t)pWVar5 * 10);
                    }
                    if (bVar1) {
                        pWVar5 = (LPWSTR)-(int64_t)(uintptr_t)pWVar5;
                    }
                    local_118 = pWVar5;
                    PECMD_FreeStrBuf((WCHAR **)&local_c8);
                    goto LAB_1400c0382;
                }
                local_160[0] = -1; /* 0xffffffff */
                if (local_130 != (LPWSTR)0x0) { /* 解析尾缀 '#' 之后的格式说明 */
                    if (*local_130 == L'#') {
                        cVar15 = (char)*local_130; /* 原版此处置 '#'(0x23) 作 trim 标记, 照移 */
                        local_130 = local_130 + 1;
                    }
                    bVar1 = PECMD_ParseHexOrDecBool((long long *)&local_130, (int *)local_160);
                    if (((*local_130 == L'e') || (*local_130 == L'f')) ||
                        (cVar19 = '\0', *local_130 == L'g')) {
                        cVar19 = (char)*local_130;
                    }
                    if ((0 < (int)CONCAT71(extraout_var, bVar1)) && ((int)local_160[0] < 0)) {
                        cVar15 = '\0';
                        local_160[0] = 0;
                    }
                }
                plVar9 = PECMD_FormatDoubleToStr((WCHAR **)&local_a0,
                                                  calc_u2d(CONCAT44(uVar38, uVar37)),
                                                  (LPCWSTR)0x0, (uint32_t)local_160[0], cVar15, cVar19);
                PECMD_ReplaceStringSlot((int64_t *)&local_188, (uint64_t *)plVar9);
                PECMD_FreeStrBuf((WCHAR **)&local_a0);
                pWVar18 = local_188;
            }
            else {
                if (uVar33 < 0x23) { /* 数值路径 */
                    uVar8 = PECMD_ParsePathRecord2(ptVar17, (uint8_t *)&local_res20);
                    pWVar6 = (LPWSTR)(int64_t)(int8_t)(uintptr_t)local_res20;
                    uVar37 = (uint32_t)uVar8;
                    uVar38 = (uint32_t)((uint64_t)uVar8 >> 0x20);
                    if ((char)(uintptr_t)local_res20 != '\0') {
                        goto LAB_1400c02bd;
                    }
                    goto LAB_1400c02c1;
                }
                /* 表达式路径 */
                pWVar5 = (LPWSTR)PECMD_ParsePathRecord(ptVar17, (uint8_t *)&local_res20);
                pWVar6 = (LPWSTR)(int64_t)(int8_t)(uintptr_t)local_res20;
                if ((char)(uintptr_t)local_res20 != '\0') {
                    goto LAB_1400c0241;
                }
            LAB_1400c0251:
                iVar2 = local_170;
                local_118 = pWVar5;
                if (local_res18 != (uint64_t *)0x0) {
                    PECMD_FreeStrBuf((WCHAR **)&local_188);
                    PECMD_FreeStrBuf((WCHAR **)&local_178);
                    LeaveCriticalSection(&g_csInit);
                    PECMD_FreeStrBuf((WCHAR **)&local_158);
                    PECMD_FreeStrBuf((WCHAR **)&local_148);
                    PECMD_FreeStrBuf((WCHAR **)&local_108);
                    PECMD_FreeStrBuf((WCHAR **)&local_150);
                    break;
                }
            LAB_1400c0382: /* ?N 前缀处理 (变量名以 '?' 开头 → 变量值区间写回) */
                pWVar18 = local_188;
                if ((int16_t)*ptVar30 == 0x3f) {
                    ptVar30 = ptVar30 + 1;
                    local_res20 = ptVar30;
                    thunk_FUN_1400f429c(&local_res20, (short)0x3f); /* 原版: 推进至下一个 '?' 或串尾 */
                    iVar2 = local_110;
                    ptVar17 = local_res20;
                    if ((int16_t)*local_res20 == 0) {
                        PECMD_FreeStrBuf((WCHAR **)&local_188);
                        PECMD_FreeStrBuf((WCHAR **)&local_178);
                        goto LAB_1400c0720;
                    }
                    local_res20 = local_res20 + 1;
                    local_168 = (WCHAR *)0x0;
                    /* dc: CONCAT44(local_res10._4_4_, local_13c) — 低32位装入 local_13c, 高32位保留 */
                    local_res10 = (WCHAR *)(((uintptr_t)local_res10 & 0xFFFFFFFF00000000ULL) |
                                            (uint32_t)local_13c);
                    iVar3 = local_13c;
                    if ((uint16_t)((uint16_t)*local_res20 - 0x30) < 10) {
                        PECMD_ParseNumTryWriteback((long long *)&local_res20, (int *)&local_res10);
                        iVar3 = (int)(uintptr_t)local_res10;
                    }
                    if (*local_res20 == 0) {
                    LAB_1400c07a4:
                        if (*local_res20 == 0x7e) {
                            goto LAB_1400c07a9;
                        }
                    }
                    else {
                        if (*local_res20 != 0x7e) {
                            local_res20 = local_res20 + 1;
                            goto LAB_1400c07a4;
                        }
                    LAB_1400c07a9:
                        local_res20 = local_res20 + 1;
                        iVar2 = iVar3;
                    }
                    PECMD_ParseNumSkipWs((WCHAR **)&local_res20, (uint64_t *)&local_168);
                    ptVar22 = (WCHAR *)(uintptr_t)local_168;
                    if (1 < iVar2) {
                        ptVar22 = (WCHAR *)((int64_t)(uintptr_t)local_168 * (int64_t)iVar2);
                    }
                    iVar3 = (int16_t)*ptVar17;
                    *ptVar17 = 0; /* 临时截断变量名 */
                    lVar31 = (int64_t)(uintptr_t)PECMD_VarLookup(param_1, ptVar30, (void *)0x0, -1,
                                                                 (void **)0x0);
                    *ptVar17 = (WCHAR)(uint16_t)iVar3; /* 恢复 */
                    if (lVar31 != 0) {
                        lVar24 = (*(uint64_t *)(lVar31 + 0x18) & 0x3fffffffffffffff) -
                                 (int64_t)(uintptr_t)ptVar22;
                        if (iVar2 < lVar24) {
                            lVar24 = (int64_t)iVar2;
                        }
                        if (0 < lVar24) {
                            /* dc: (longlong)ptVar22->mbulinfo + *(var+8) + -0xc — 槽头抵消
                             * = ptVar22 + *(var+8) (字节), 从变量值区拷入 local_118 */
                            PECMD_MemMoveForward((uint8_t *)ptVar22 + (size_t)*(int64_t *)(lVar31 + 8),
                                                 (uint8_t *)&local_118, (int)lVar24);
                        }
                    }
                    PECMD_FreeStrBuf((WCHAR **)&local_188);
                    PECMD_FreeStrBuf((WCHAR **)&local_178);
                    LeaveCriticalSection(&g_csInit);
                    pWVar5 = pWVar6;
                    goto LAB_1400c0906;
                }
                if (iVar2 < 2) { /* base<=1 → 十进制 %I64d/%I64u */
                    pwVar23 = L"%I64d";
                    if (0x23 < uVar33) {
                        pwVar23 = L"%I64u";
                    }
                    PECMD_SprintfRetEnd(local_188, (uint64_t)(uintptr_t)local_118, pwVar23);
                }
                else { /* base 2/8/16 (或其余进制) 逐位数字输出 */
                    if (iVar2 == 2) {
                        pWVar29 = L"0b";
                    }
                    else if (iVar2 == 8) {
                        pWVar29 = L"0o";
                    }
                    else {
                        pWVar29 = L"0x";
                        if (iVar2 != 0x10) {
                            pWVar29 = g_szEmpty;
                        }
                    }
                    pWVar32 = g_szEmpty;
                    if (((uint16_t)local_16c < 0x24) && ((int64_t)(uintptr_t)local_118 < 0)) {
                        local_118 = (LPWSTR)-(int64_t)(uintptr_t)local_118;
                        pWVar32 = L"-";
                    }
                    if (local_118 == (LPWSTR)0x0) {
                        FUN_14007034C((WCHAR **)&local_188, L"0");
                    }
                    else {
                        pWVar5 = local_118;
                        do {
                            PECMD_CopyStrToSlot((uint64_t *)&local_120, (uint64_t *)&local_188);
                            /* dc: pWVar5/='base' 后 wsprintfW(pWVar18,"%X") — Ghidra 丢弃第3参(余数);
                             * 原版 asm: DIV → RDX 余数 → wsprintfW(pWVar18,"%X",余数) */
                            uVar37 = (uint32_t)((uint64_t)(uintptr_t)pWVar5 %
                                                (uint64_t)(int64_t)iVar2);
                            pWVar5 = (LPWSTR)((uint64_t)(uintptr_t)pWVar5 /
                                              (uint64_t)(int64_t)iVar2);
                            wsprintfW(pWVar18, L"%X", uVar37);
                            iVar3 = lstrlenW(local_120);
                            iVar4 = lstrlenW(pWVar18);
                            PECMD_MemMoveForward((uint8_t *)(pWVar18 + iVar4),
                                                 (uint8_t *)local_120, (iVar3 + 1) * 2);
                            PECMD_FreeStrBuf((WCHAR **)&local_120);
                            puVar21 = local_158;
                        } while (pWVar5 != (LPWSTR)0x0);
                    }
                    PECMD_CopyStrToSlot((uint64_t *)&local_a8, (uint64_t *)&local_188);
                    FUN_14007034C((WCHAR **)&local_188, pWVar32);
                    FUN_14006375C(&local_188, pWVar29);
                    FUN_14006375C(&local_188, local_a8);
                    PECMD_FreeStrBuf((WCHAR **)&local_a8);
                    pWVar18 = local_188;
                    ptVar30 = local_128;
                }
            }
            PECMD_StripTrailingSpaces(pWVar18);
            iVar2 = lstrlenW(pWVar18);
            lVar31 = (int64_t)(local_140 + 1 + local_17c);
            PECMD_AllocString((WCHAR **)&local_148, lVar31 + (int64_t)iVar2 + 1);
            ptVar22 = local_148;
            puVar14 = local_148 + lVar31; /* dc: local_148->mbulinfo[lVar31-6] → p[lVar31] */
            local_d0 = local_148;
            if (local_17c != 0) {
                PECMD_MemMoveForward((uint8_t *)local_148, (uint8_t *)L"+txt", 8);
            }
            uVar10 = (uint64_t)local_140;
            if (local_140 != 0) {
                PECMD_MemMoveForward((uint8_t *)(ptVar22 + local_17c), (uint8_t *)dt_129810, 6);
            }
            ptVar22[local_17c + (int32_t)uVar10] = L' '; /* dc: mbulinfo[local_17c+uVar10-6] */
            PECMD_MemMoveForward((uint8_t *)puVar14, (uint8_t *)pWVar18, ((int64_t)iVar2 + 1) * 2);
            ptVar17 = local_168;
            if ((int16_t)*local_168 == 0) {
                goto LAB_1400c0876;
            }
            if ((int16_t)*local_res10 != 0) {
                FUN_1400629B8(param_1, local_res10, puVar14);
            }
            PECMD_FreeStrBuf((WCHAR **)&local_188);
            PECMD_FreeStrBuf((WCHAR **)&local_178);
            uVar35 = (uint64_t)local_180;
        } while (true);
    }
LAB_1400c0a79:
    PECMD_FreeStrBuf((WCHAR **)&local_90);
    PECMD_FreeStrBuf((WCHAR **)local_80); /* dc: FUN_14005b104(local_80) — 数组退化为 &local_80[0], 只释放第 0 槽 */
    PECMD_FreeStrBuf((WCHAR **)&local_138);
    PECMD_FreeStrBuf((WCHAR **)&local_f0);
    return pWVar5;
LAB_1400c0876: /* 语句流耗尽: 赋值变量或弹消息框 */
    PECMD_FreeStrBuf((WCHAR **)&local_188);
    PECMD_FreeStrBuf((WCHAR **)&local_178);
    if ((int16_t)*ptVar30 == 0) {
        LeaveCriticalSection(&g_csInit);
        local_res10 = (WCHAR *)0x0;
        if (pWVar6 != (LPWSTR)0x0) {
            PECMD_StrBldCopyWide(&local_res10, local_88);
            ptVar22 = local_res10;
        }
        PECMD_MessShowMsgBox(param_1, ptVar22, (int64_t *)0x0);
        PECMD_FreeStrBuf((WCHAR **)&local_res10);
        pWVar5 = pWVar6;
    }
    else {
        FUN_1400629B8(param_1, ptVar30, puVar14);
    LAB_1400c0720:
        LeaveCriticalSection(&g_csInit);
        pWVar5 = pWVar6;
    }
LAB_1400c0906: /* -[ 重复段收尾: 还有剩余语句则回 LAB_1400c0948 重跑 */
    lVar31 = local_d8;
    PECMD_FreeStrBuf((WCHAR **)&local_158);
    PECMD_FreeStrBuf((WCHAR **)&local_148);
    if (lVar31 < 1) {
        goto LAB_1400c0a08;
    }
    uVar35 = (uint64_t)local_180;
    uVar10 = local_100;
    goto LAB_1400c0948;
LAB_1400c0a08:
    PECMD_FreeStrBuf((WCHAR **)&local_108);
    PECMD_FreeStrBuf((WCHAR **)&local_150);
    goto LAB_1400c0a79;
}