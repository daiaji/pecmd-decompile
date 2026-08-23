/* ====================================================================
 * core_b7a.c — B7 批次小函数 (1400c0000-1400dffff)
 *
 * 本批新实现函数直接使用人类可读 PECMD_ 名称，原始地址保留在 @0x 注释。
 *
 * 来源: PECMD原始.EXE (x64, ImageBase=0x140000000)
 *   解析双精度(括号)      PECMD_CalcEvalParenAtom @0x1400c1000
 *   解析 UInt64(括号)     PECMD_EvalParenthesizedExpr @0x1400c10c0
 *   解析 Int64(括号)      PECMD_EvalParenExprRounded @0x1400c11f4
 *   字符串转字节数组       PECMD_ParseHexByteList @0x1400c12fc
 *   释放 UpDown 控件      PECMD_DtorUpDownControl @0x1400c3ca4
 *   解析 UInt64 并跳格    PECMD_ParseValStepNext @0x1400c44f4
 *   解析 Int64 并跳格     PECMD_ParseCalcStepNext @0x1400c4518
 *   解析 UInt64 并跳非空  PECMD_ParseU64SkipSep @0x1400c453c
 *   解析 Int 并跳格       PECMD_ParseIntStepNext @0x1400c4580
 *   添加 UpDown 控件      PECMD_AddUpDownControl @0x1400c45a4
 *   添加滚动条控件        PECMD_AddScrollBarObject @0x1400c46cc
 *   控件 Enable 命令      FUN_1400C47F4 @0x1400c47f4
 *   解析 Int64 并跳格(带返回值) PECMD_ParseNumberToken @0x1400cada0
 *   解析资源规格          PECMD_ParseItemImageSpec @0x1400d0b2c
 *   重绘控件背景          PECMD_RecomputeWndBackground @0x1400d6f8c
 *
 * 约定:
 *   - 新实现函数使用 PECMD_ 可读名；未实现依赖仍 extern FUN_ + TODO(verify)
 *   - 全局数据符号先用 extern 声明; 完整链接时统一在 core_globals.c 定义
 * ==================================================================== */
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>

#include "pecmd_defs.h"
extern WCHAR **FUN_14005B154(WCHAR **pp); /* @0x14005b154 */

/* ---- 未实现依赖 (extern + TODO(verify)) ---- */
extern uint64_t PECMD_CalcExpression(int64_t *obj, WCHAR *a2, double *a3);
    /* TODO(verify) 表达式求值器: 声明(uint64_t,int64_t*,WCHAR*,double*)与真实定义(LPWSTR,int64_t*,WCHAR*,uint64_t*)不符；
       本文件调用点传 double* 且将 LPWSTR 返回值强转 uint64_t。属复杂近似依赖，保持原声明以免破坏调用点。 */
extern uint64_t PECMD_ParseNumberToDouble(int64_t *obj, double *a2);
extern uint64_t FUN_14006A7F4(int64_t *obj, uint64_t *a2);
extern uint64_t FUN_14006A7F4(int64_t *obj, uint64_t *a2);
extern bool FUN_1400C1194(int64_t *obj, uint64_t *a2);
extern bool FUN_1400C11C0(int64_t *obj, int *a2);
extern uint64_t *PECMD_GrowByteBuffer(uint64_t *obj, int64_t a2);
extern void PECMD_ZeroLenBuf(void *p);
extern uint64_t PECMD_ScanHexNumber(int64_t *obj, int64_t *a2, int *a3);
extern void *PECMD_SendCtrlMessage_0834(WPARAM obj, uint64_t a2);
extern void FUN_14005DAF8(int64_t obj, int *a2, int *a3,
                          int *a4, int *a5);
extern void FUN_14005D9A8(int64_t obj, int a2);
extern uint64_t *PECMD_CreateUpDownCtrl(uint64_t *obj, int64_t a2, uint32_t a3,
                               uint64_t *a4, uint32_t a5, uint32_t a6,
                               uint32_t a7, uint32_t a8, uint64_t *a9,
                               uint64_t *a10, uint64_t *a11, uint32_t a12);
extern uint64_t *PECMD_CreateScrollBarObj(uint64_t *obj, int64_t a2, uint32_t a3,
                               uint64_t *a4, uint32_t a5, uint32_t a6,
                               uint32_t a7, uint32_t a8, uint16_t *a9,
                               uint64_t *a10, uint32_t a11, LPCWSTR a12);
extern void PECMD_AllocStrSlot(WCHAR **ps);
extern void PECMD_CopyTokenTrimmed(int64_t *obj, int64_t *a2, int16_t a3,
                          int16_t a4);
extern void PECMD_VarSetUInt(void *s, uint64_t v, LPCWSTR k);
extern char PECMD_CtlLoadPictureRgn(int64_t obj, HDC a2);
extern uint32_t PECMD_IsIconResource(uint16_t *obj);
extern uint32_t PECMD_IsBitmapResource(uint16_t *obj);

/* ---- 已实现依赖 ---- */
extern void PECMD_DtorTrackbarControl(uint64_t *obj);   /* @0x14006bf8c core_b3g.c */
extern uint32_t FUN_14005D7E8(uint16_t *obj); /* @0x14005d7e8 core_b3f.c */

/* ---- 本批引用的全局数据 ---- */
extern double g_dbl25230;         /* 四舍五入 0.5 */

/* ========== PECMD_CalcEvalParenAtom @0x1400c1000 ==========
 * 解析 double；若遇到 '(' 则把括号内表达式交给求值器。
 */
uint64_t PECMD_CalcEvalParenAtom(int64_t *pp, double *out)
{
    FUN_14005B154((WCHAR **)pp);
    WCHAR *p = (WCHAR *)*pp;
    WCHAR *close = NULL;
    if (*p == L'(') {
        WCHAR *expr = p + 1;
        int depth = 1;
        do {
            do {
                while (1) {
                    p++;
                    *pp = (int64_t)p;
                    if (*p == L'\0') {
                        goto evaluate;
                    }
                    if (*p != L'(') {
                        break;
                    }
                    depth++;
                }
            } while (*p != L')');
            depth--;
        } while (0 < depth);
        close = (WCHAR *)*pp;
        *pp = (int64_t)(close + 1);
        *close = L'\0';
evaluate:
        double value = 0.0;
        PECMD_CalcExpression((int64_t *)g_Script, expr, &value);
        *out = value;
        if (close != NULL) {
            *close = L')';
        }
        return 1;
    }
    return PECMD_ParseNumberToDouble(pp, out);
}

/* ========== PECMD_EvalParenthesizedExpr @0x1400c10c0 ==========
 * 解析 UInt64；括号表达式先改写为 '#' 后交给求值器。
 */
uint64_t PECMD_EvalParenthesizedExpr(int64_t *pp, uint64_t *out)
{
    FUN_14005B154((WCHAR **)pp);
    WCHAR *p = (WCHAR *)*pp;
    WCHAR *close = NULL;
    if (*p == L'(') {
        *p = L'#';
        *pp = (int64_t)(p + 1);
        WCHAR *q = (WCHAR *)*pp;
        int depth = 1;
        while (*q != L'\0') {
            if (*q == L'(') {
                depth++;
            } else if (*q == L')') {
                depth--;
                if (depth < 1) {
                    close = (WCHAR *)*pp;
                    *pp = (int64_t)(close + 1);
                    *close = L'\0';
                    break;
                }
            }
            q++;
            *pp = (int64_t)q;
        }
        *out = 0;
        double dummy[2] = {0.0, 0.0};
        *out = (uint64_t)PECMD_CalcExpression((int64_t *)g_Script, p, dummy);
        *p = L'(';
        if (close != NULL) {
            *close = L')';
        }
        return 1;
    }
    return FUN_14006A7F4(pp, out);
}

/* ========== PECMD_EvalParenExprRounded @0x1400c11f4 ==========
 * 解析 Int64；括号表达式求值后做四舍五入。
 */
uint64_t PECMD_EvalParenExprRounded(int64_t *pp, uint64_t *out)
{
    FUN_14005B154((WCHAR **)pp);
    WCHAR *p = (WCHAR *)*pp;
    WCHAR *close = NULL;
    if (*p == L'(') {
        WCHAR *expr = p + 1;
        int depth = 1;
        do {
            do {
                while (1) {
                    p++;
                    *pp = (int64_t)p;
                    if (*p == L'\0') {
                        goto evaluate;
                    }
                    if (*p != L'(') {
                        break;
                    }
                    depth++;
                }
            } while (*p != L')');
            depth--;
        } while (0 < depth);
        close = (WCHAR *)*pp;
        *pp = (int64_t)(close + 1);
        *close = L'\0';
evaluate:
        double value = (double)(int64_t)*out;
        PECMD_CalcExpression((int64_t *)g_Script, expr, &value);
        if (close != NULL) {
            *close = L')';
        }
        FUN_14005B154((WCHAR **)pp);
        if (0.0 <= value) {
            if (0.0 < value) {
                value += g_dbl25230;
            }
        } else {
            value -= g_dbl25230;
        }
        *out = (uint64_t)(int64_t)value;
        return 1;
    }
    return FUN_14006A7F4(pp, out);
}

/* ========== PECMD_ParseHexByteList @0x1400c12fc ==========
 * 把字符串解析为字节数组；mode=0 时支持括号/表达式，否则按十六进制。
 */
uint64_t PECMD_ParseHexByteList(LPCWSTR str, int64_t *outBuf, int mode)
{
    LPCWSTR s = str;
    FUN_14005B154((WCHAR **)&s);
    LPCWSTR p = s;
    int len = lstrlenW(s);
    PECMD_GrowByteBuffer((uint64_t *)outBuf, (int64_t)(len + 8));
    PECMD_ZeroLenBuf((uint64_t *)*outBuf);
    uint8_t *out = (uint8_t *)*outBuf;
    uint64_t count = 0;
    uint64_t idx = 0;
    if (0 < len) {
        do {
            if (*p == L'\0') {
                return count;
            }
            uint64_t value = 0;
            if (mode == 0) {
                FUN_1400C1194((int64_t *)&s, &value);
            } else {
                if ((*p == L'0') && ((WCHAR)(p[1] | 0x20) == 0x78)) {
                    s = p + 2;
                }
                int base[4] = {2, 0, 0, 0};
                PECMD_ScanHexNumber((int64_t *)&s, (int64_t *)&value, base);
                FUN_14005B154((WCHAR **)&s);
            }
            count = (uint64_t)((int)count + 1);
            out[idx] = (uint8_t)value;
            idx++;
            p = s;
        } while ((int64_t)idx < (int64_t)len);
    }
    return count;
}

/* ========== PECMD_DtorUpDownControl @0x1400c3ca4 ==========
 * 释放 UpDown 控件的三个字符串槽和对象本体。
 */
uint64_t *PECMD_DtorUpDownControl(uint64_t *s, uint32_t out)
{
    PECMD_FreeStrBuf((WCHAR **)(s + 0x11));
    PECMD_FreeStrBuf((WCHAR **)(s + 0x10));
    PECMD_FreeStrBuf((WCHAR **)(s + 0xf));
    PECMD_DtorTrackbarControl(s);
    if ((out & 1) != 0) {
        free(s);
    }
    return s;
}

/* ========== PECMD_ParseValStepNext @0x1400c44f4 ==========
 * 解析 UInt64 后，若当前字符非 NUL 则前进一步。
 * 注: Ghidra 丢失了第二个输出参数，这里按调用语义补全。
 */
void PECMD_ParseValStepNext(int64_t *a, uint64_t *b)
{
    FUN_1400C1194(a, b);
    if (*(WCHAR *)*a != L'\0') {
        *a = (int64_t)((WCHAR *)*a + 1);
    }
}

/* ========== PECMD_ParseCalcStepNext @0x1400c4518 ==========
 * 解析 Int64 后，若当前字符非 NUL 则前进一步。
 */
void PECMD_ParseCalcStepNext(int64_t *a, uint64_t *b)
{
    PECMD_EvalParenExprRounded(a, b);
    if (*(WCHAR *)*a != L'\0') {
        *a = (int64_t)((WCHAR *)*a + 1);
    }
}

/* ========== PECMD_ParseU64SkipSep @0x1400c453c ==========
 * 解析 UInt64 后，仅当当前字符不是空白/空格时前进一步。
 * 注: Ghidra 丢失了第二个输出参数，这里按调用语义补全。
 */
uint32_t PECMD_ParseU64SkipSep(int64_t *a, uint64_t *b)
{
    uint64_t r = PECMD_EvalParenthesizedExpr(a, b);
    WCHAR *p = (WCHAR *)*a;
    if ((*p != L'\0') && (((*p < 9) || (0xd < *p)) && (*p != L' '))) {
        *a = (int64_t)(p + 1);
    }
    return (uint32_t)r;
}

/* ========== PECMD_ParseIntStepNext @0x1400c4580 ==========
 * 解析 Int 后，若当前字符非 NUL 则前进一步。
 */
void PECMD_ParseIntStepNext(int64_t *a, int *b)
{
    FUN_1400C11C0(a, b);
    if (*(WCHAR *)*a != L'\0') {
        *a = (int64_t)((WCHAR *)*a + 1);
    }
}

/* ========== PECMD_AddUpDownControl @0x1400c45a4 ==========
 * 在控件数组中加入一个新的 UpDown 控件。
 */
void PECMD_AddUpDownControl(WPARAM mgr, int64_t v2, uint64_t *v3,
                            int x, int y, int w, int h,
                            uint64_t *p8, uint64_t *p9, uint64_t *p10,
                            uint32_t flags)
{
    int rc[2] = {x, 0};
    uint64_t *slot = (uint64_t *)PECMD_SendCtrlMessage_0834(mgr, *v3);
    uint64_t *obj = NULL;
    if (slot != NULL) {
        int64_t base = **(int64_t **)((uint8_t *)mgr + 0x1a0);
        FUN_14005DAF8((int64_t)mgr, rc, &y, &w, &h);
        uint64_t *mem = (uint64_t *)malloc(0x90);
        if (mem != NULL) {
            obj = PECMD_CreateUpDownCtrl(mem, v2,
                                (uint32_t)(((int64_t)slot - base) >> 3) + 0x1000,
                                v3, (uint32_t)rc[0], (uint32_t)y,
                                (uint32_t)w, (uint32_t)h, p8, p9,
                                p10, flags);
        }
        *slot = (uint64_t)obj;
        FUN_14005D9A8((int64_t)mgr, 0);
    }
}

/* ========== PECMD_AddScrollBarObject @0x1400c46cc ==========
 * 在控件数组中加入一个新的滚动条控件。
 */
void PECMD_AddScrollBarObject(WPARAM mgr, int64_t v2, uint64_t *v3,
                               int x, int y, int w, int h,
                               uint16_t *p8, uint64_t *p9, uint32_t p10,
                               LPCWSTR text)
{
    int rc[2] = {x, 0};
    uint64_t *slot = (uint64_t *)PECMD_SendCtrlMessage_0834(mgr, *v3);
    uint64_t *obj = NULL;
    if (slot != NULL) {
        int64_t base = **(int64_t **)((uint8_t *)mgr + 0x1a0);
        FUN_14005DAF8((int64_t)mgr, rc, &y, &w, &h);
        uint64_t *mem = (uint64_t *)malloc(0x70);
        if (mem != NULL) {
            obj = PECMD_CreateScrollBarObj(mem, v2,
                                (uint32_t)(((int64_t)slot - base) >> 3) + 0x1000,
                                v3, (uint32_t)rc[0], (uint32_t)y,
                                (uint32_t)w, (uint32_t)h, p8, p9,
                                p10, text);
        }
        *slot = (uint64_t)obj;
        FUN_14005D9A8((int64_t)mgr, 0);
    }
}

/* ========== FUN_1400C47F4 @0x1400c47f4 ==========
 * 控件 Enable 命令：'?' 查询 IsWindowEnabled 并写变量，否则发送/投递 0x462。
 */
uint64_t FUN_1400C47F4(int64_t *ctx, HWND hwnd, HWND target,
                             LPCWSTR spec, WPARAM wParam, uint64_t lParam)
{
    if (*spec == L'\0') {
        return 0xf8000009;
    }
    LPCWSTR s = spec;
    if (*spec == L'?') {
        PECMD_AllocStrSlot((WCHAR **)&s);
        LPCWSTR key = spec + 1;
        PECMD_CopyTokenTrimmed((int64_t *)&key, (int64_t *)&s, 0x3b, 0x3a);
        uint32_t enabled = (uint32_t)IsWindowEnabled(target);
        PECMD_VarSetUInt(ctx, (uint64_t)enabled, s);
        PECMD_FreeStrBuf((WCHAR **)&s);
    } else {
        WCHAR first = *spec;
        if (first == L'#') {
            s = spec + 1;
        }
        int id = -0x7fffff7;
        FUN_1400C11C0((int64_t *)&s, &id);
        if (id == -0x7fffff7) {
            return 0xf8000009;
        }
        struct {
            HWND hwnd;
            uint64_t flag;
            uint64_t param6;
            uint64_t zero;
        } msg;
        msg.hwnd = target;
        msg.flag = (uint64_t)(id != 0);
        msg.param6 = lParam;
        msg.zero = 0;
        if (first != L'#') {
            SendMessageW(hwnd, 0x462, wParam, (LPARAM)&msg);
        } else {
            PostMessageW(hwnd, 0x462, wParam, (LPARAM)&msg);
        }
    }
    return 0;
}

/* ========== PECMD_ParseNumberToken @0x1400cada0 ==========
 * 解析 Int64，成功后写回 int，再前进一步。
 */
void PECMD_ParseNumberToken(int64_t *pp, int *out)
{
    uint64_t local = (uint64_t)*out;
    uint64_t r = PECMD_EvalParenExprRounded(pp, &local);
    if ((int)r != 0) {
        *out = (int)local;
    }
    if (*(WCHAR *)*pp != L'\0') {
        *pp = (int64_t)((WCHAR *)*pp + 1);
    }
}

/* ========== PECMD_ParseItemImageSpec @0x1400d0b2c ==========
 * 解析资源名/ID；返回资源字符串或整数资源类型指针。
 */
uint16_t *PECMD_ParseItemImageSpec(int64_t *pp, int64_t *out, uint16_t *token)
{
    WCHAR *p = (WCHAR *)*pp;
    WCHAR *id_start = p - 1;
    *id_start = L'#';
    if (9 < (uint16_t)(*p - L'0')) {
        *out = (int64_t)p;
    } else {
        *out = (int64_t)id_start;
    }
    WCHAR *q = p;
    while ((*q != L'\0') && (*q != L'|') && (*q != L':')) {
        q++;
        *pp = (int64_t)q;
    }
    WCHAR sep = *q;
    *(WCHAR *)q = L'\0';
    if (sep != L'\0') {
        *pp = (int64_t)(q + 1);
        token = (uint16_t *)*pp;
        WCHAR *end = (WCHAR *)token;
        WCHAR ch = *end;
        while ((ch != L'\0') && (((ch < 9) || (0xd < ch)) && (ch != L' '))) {
            end++;
            ch = *end;
        }
        *end = L'\0';
        if (*token == L'|') {
            return token + 1;
        }
        if (*(WCHAR *)*pp == L'#') {
            uint16_t *num = NULL;
            *pp = (int64_t)((WCHAR *)*pp + 1);
            if (FUN_1400C1194(pp, (uint64_t *)&num)) {
                return num;
            }
        }
    }
    if ((uint16_t *)0xffff < token) {
        if ((char)PECMD_IsIconResource(token) != '\0') {
            return (uint16_t *)0x3;
        }
        if ((char)FUN_14005D7E8(token) != '\0') {
            return (uint16_t *)0xe;
        }
        if ((char)PECMD_IsBitmapResource(token) != '\0') {
            return (uint16_t *)0x2;
        }
    }
    return token;
}

/* ========== PECMD_RecomputeWndBackground @0x1400d6f8c ==========
 * 清零背景/布局状态并调用背景绘制。
 */
uint64_t PECMD_RecomputeWndBackground(int64_t pp)
{
    *(uint32_t *)(pp + 0x244) = 0;
    *(uint32_t *)(pp + 0x24c) = 0;
    *(uint32_t *)(pp + 0x23c) = 0;
    *(uint32_t *)(pp + 0x248) = 0;
    *(uint32_t *)(pp + 0x240) = 0;
    *(uint32_t *)(pp + 0x238) = 0;
    GetWindowLongW(*(HWND *)(pp + OBJ_HWND), -0x10);
    HWND hwnd = *(HWND *)(pp + OBJ_HWND);
    *(uint8_t *)(pp + 0x250) = 2;
    HDC hdc = GetDC(hwnd);
    PECMD_CtlLoadPictureRgn(pp, hdc);
    if (hdc != (HDC)0) {
        ReleaseDC(hwnd, hdc);
    }
    return 1;
}
