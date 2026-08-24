/* crt_shims.c - B0/P4: CRT isolation.
 * Collected from core_b9_remaining.c per PRODUCTION_ROADMAP §3.B.0 item 3:
 *   (a) MSVC CRT internal mechanism family (_xxx/__xxx)
 *   (b) libc same-name strong definitions (strtol/perror/raise/iswctype/wcstol/...)
 *   (c) math empty stubs (double FUN_xxx float-expansion placeholders)
 * FOR LINK VERIFICATION ONLY - must be removed before deployment linking
 * against a real system CRT. Bodies are byte-identical to their origin. */

#include "pecmd_defs.h"

/* P0-2 MSVC trim (190 removed): system CRT provides the real bodies for
 * the CRT-internal mechanism family and libc same-name strong definitions
 * (_onexit/__initstdio/entry/exception/perror/wcstol/...). Call sites were
 * verified to use correct prototypes. Kept: 12 FUN_ math-gap stubs -
 * real call sites in core_b3_remaining.c, no system counterpart exists. */


double FUN_140104b00(double param_1)
{
    /* UNIMPLEMENTED @FUN_140104b00 — decompile-failed, body 未还原 */
/* @0x140104b00 size=773 */
    /* SKIP(CRT): MSVC CRT 三角函数 (asin/acos 多项式展开, 含 _controlfp_s/
       SQRT/系数表 _DAT_14012d2b0 与 FUN_14010da74 打印辅助). 使用 CRT 等价,
       不逐行还原内联展开. */
    (void)param_1;
    return 0.0;
}


double FUN_140104e08(double param_1)
{
    /* UNIMPLEMENTED @FUN_140104e08 — decompile-failed, body 未还原 */
/* @0x140104e08 size=787 */
    /* SKIP(CRT): MSVC CRT 三角函数 (asin/acos 多项式展开, 含 _controlfp_s/
       SQRT/系数表 _DAT_14012d2b0 与 FUN_14010da74 打印辅助). 使用 CRT 等价. */
    (void)param_1;
    return 0.0;
}


double FUN_14010511c(double param_1)
{
    /* UNIMPLEMENTED @FUN_14010511c — decompile-failed, body 未还原 */
/* @0x14010511c size=651 */
    /* SKIP(CRT): atan 数学函数，使用 CRT atan 句柄 */
    (void)param_1;
    return 0.0;
}


double FUN_1401053a8(double param_1)
{
    /* UNIMPLEMENTED @FUN_1401053a8 — decompile-failed, body 未还原 */
/* @0x1401053a8 size=276 */
    /* SKIP(CRT): MSVC CRT ceil 实现 (幂尾数掩码 + 有小数位时 +1.0, 含
       FUN_14010da74 打印辅助/非法参数路径). 使用 CRT ceil 等价, 不还原内联. */
    (void)param_1;
    return 0.0;
}


double FUN_1401055ec(double param_1)
{
    /* UNIMPLEMENTED @FUN_1401055ec — decompile-failed, body 未还原 */
/* @0x1401055ec size=734 */
    /* SKIP(CRT): MSVC CRT exp 数学函数内联实现 */
    (void)param_1;
    return 0.0;
}


double FUN_1401058cc(double param_1)
{
    /* UNIMPLEMENTED @FUN_1401058cc — decompile-failed, body 未还原 */
/* @0x1401058cc size=756 */
    /* SKIP(CRT): MSVC CRT atan 数学函数内联实现 */
    (void)param_1;
    return 0.0;
}


double FUN_140105bc0(uint64_t param_1)
{
    /* UNIMPLEMENTED @FUN_140105bc0 — decompile-failed, body 未还原 */
/* @0x140105bc0 size=846 */
    /* SKIP(CRT): 反编译为 MSVC CRT log10 数学函数内联实现 */
    (void)param_1;
    return 0.0;
}


double FUN_140105f10(double param_1, double param_2)
{
    /* UNIMPLEMENTED @FUN_140105f10 — decompile-failed, body 未还原 */
/* @0x140105f10 size=1494 */
    /* SKIP(CRT): pow 数学函数，使用 log2/f2xm1/fscale 内联实现 */
    (void)param_1;
    (void)param_2;
    return 0.0;
}


double FUN_140106654(double param_1)
{
    /* UNIMPLEMENTED @FUN_140106654 — decompile-failed, body 未还原 */
/* @0x140106654 size=807 */
    /* SKIP(CRT): MSVC CRT sin 数学函数内联实现 */
    (void)param_1;
    return 0.0;
}


double FUN_14010697c(double param_1)
{
    /* UNIMPLEMENTED @FUN_14010697c — decompile-failed, body 未还原 */
/* @0x14010697c size=266 */
    /* SKIP(CRT): MSVC CRT sqrt 数学函数内联实现 */
    (void)param_1;
    return 0.0;
}


double FUN_140106c5c(double param_1)
{
    /* UNIMPLEMENTED @FUN_140106c5c — decompile-failed, body 未还原 */
/* @0x140106c5c size=791 */
    /* SKIP(CRT): MSVC CRT cos 数学函数内联实现 */
    (void)param_1;
    return 0.0;
}


double FUN_140107468(double param_1, double param_2)
{
    /* UNIMPLEMENTED @FUN_140107468 — decompile-failed, body 未还原 */
/* @0x140107468 size=644 */
    /* SKIP(CRT): MSVC CRT _hypot 数学函数 */
    (void)param_1;
    (void)param_2;
    return 0.0;
}