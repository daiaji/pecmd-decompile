/*
 * core_strbld.c — 字符串构建器 / 数字格式化 / 变量格式化（B8b）
 *
 *   PECMD_StrBldInitWide    @0x14006d7e8  6 槽字符串构建器初始化
 *   PECMD_TextBufReserve    @0x14006d880  按需扩展（need + 0xe）
 *   PECMD_StrBldGrowWide @0x14006d92c 批量扩展（+0x400）
 *   PECMD_SprintfRetEnd      @0x1400e6d38  %I64X 格式化
 *   PECMD_FormatU64Dec       @0x1400e6d74  %I64u 格式化
 *   PECMD_FormatDblRetEnd     @0x1400e6d80  浮点格式化
 *   PECMD_FormatU64RetEnd        @0x1400e6db4  长浮点格式化
 *   PECMD_FormatTypedMemValue       @0x14006d9d0  变量格式化
 *
 * 6 槽字符串构建器布局（调用方 PECMD_ExpandEnvVars 传递变量地址）:
 *   s[0]=out 变量地址(WCHAR**), s[1]=count 变量地址(int*),
 *   s[2]=cur 写指针变量, s[3]=end 尾指针变量, s[4]=base 基指针变量,
 *   s[5]=limit 警戒指针变量 (limit = end - 0x7e4)
 * 槽内读取 = *(WCHAR**)s[i]（指向的变量当前值）.
 *
 * 说明:
 *   PECMD_SafeVFormatW (StringValidateDestW + StringVPrintfWorkerW) 简化为
 *   swprintf(dst, 0x3e, ...) (0x3e=62 字符上限).
 *   PECMD_MemMoveForward/14001d744 = MSVC 内联 memcpy/memmove, 库替换.
 */
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <wchar.h>

#include "pecmd_defs.h" /* PECMD_AllocStrSlot / FUN_14005C788 / wsprintfW 等 */

extern bool FUN_1400C1194(LPCWSTR *ps, uint64_t *out); /* @0x1400c1194 */

/* ========== PECMD_StrBldInitWide @0x14006d7e8 ==========
 * 初始化 6 槽字符串构建器. count < 0x400 时提升到 0x400 并分配缓冲.
 * end = out 字节地址 - 2 + count*2; limit = end - 0x7e4; cur 初始 = base.
 */
void PECMD_StrBldInitWide(void *s[6], WCHAR **out, int *count, WCHAR **cur, WCHAR **end,
                          WCHAR **base, WCHAR **limit)
{
    s[0] = (void *)out;
    s[1] = (void *)count;
    s[2] = (void *)cur;
    s[3] = (void *)end;
    s[4] = (void *)base;
    s[5] = (void *)limit;
    if (*count < 0x400) {
        *count = 0x400;
        PECMD_AllocString(out, 0x400);
    }
    *end = (WCHAR *)((char *)*out - 2 + (size_t)*count * 2);
    *base = *out;
    *limit = (WCHAR *)((char *)*end - 0x7e4);
    *cur = *base;
}

/* ========== PECMD_TextBufReserve @0x14006d880 ==========
 * 按需扩展: ((need*2 - end) + cur) >> 1 >= 0 时 count += need + 0xe,
 * 重新分配并平移 cur (偏移量 (cur-oldBase)>>1 个 WCHAR).
 */
void PECMD_TextBufReserve(void *s[6], int need)
{
    WCHAR **pOut = (WCHAR **)s[0];
    int *count = (int *)s[1];
    WCHAR **pCur = (WCHAR **)s[2];
    WCHAR **pEnd = (WCHAR **)s[3];
    WCHAR **pBase = (WCHAR **)s[4];
    WCHAR **pLimit = (WCHAR **)s[5];
    WCHAR *cur = *pCur;
    int64_t avail = ((int64_t)need * 2 - (int64_t)(uintptr_t)*pEnd) + (int64_t)(uintptr_t)cur;
    if ((int)(avail >> 1) >= 0) {
        WCHAR *oldBase = *pBase;
        *count += need + 0xe;
        PECMD_AllocString(pOut, *count);
        *pBase = *pOut;
        *pEnd = (WCHAR *)((char *)*pBase - 2 + (size_t)*count * 2);
        *pLimit = (WCHAR *)((char *)*pEnd - 0x7e4);
        *pCur = (WCHAR *)((char *)*pBase +
                          (int)(((int64_t)(uintptr_t)cur - (int64_t)(uintptr_t)oldBase) >> 1) * 2);
    }
}

/* ========== PECMD_StrBldGrowWide @0x14006d92c ==========
 * 批量扩展: (cur - limit) >> 1 >= 0 时 count += 0x400, 其余同 PECMD_TextBufReserve.
 */
void PECMD_StrBldGrowWide(void *s[6])
{
    WCHAR **pOut = (WCHAR **)s[0];
    int *count = (int *)s[1];
    WCHAR **pCur = (WCHAR **)s[2];
    WCHAR **pEnd = (WCHAR **)s[3];
    WCHAR **pBase = (WCHAR **)s[4];
    WCHAR **pLimit = (WCHAR **)s[5];
    WCHAR *cur = *pCur;
    int64_t dist = (int64_t)(uintptr_t)cur - (int64_t)(uintptr_t)*pLimit;
    if ((int)(dist >> 1) >= 0) {
        WCHAR *oldBase = *pBase;
        *count += 0x400;
        PECMD_AllocString(pOut, *count);
        *pBase = *pOut;
        *pEnd = (WCHAR *)((char *)*pBase - 2 + (size_t)*count * 2);
        *pLimit = (WCHAR *)((char *)*pEnd - 0x7e4);
        *pCur = (WCHAR *)((char *)*pBase +
                          (int)(((int64_t)(uintptr_t)cur - (int64_t)(uintptr_t)oldBase) >> 1) * 2);
    }
}

/* ========== PECMD_SprintfRetEnd @0x1400e6d38 ==========
 * wsprintfW(dst, fmt, v) (Ghidra 丢第 3 参, 已补).
 * 返回写结束指针 dst + lstrlenW(dst).
 */
WCHAR *PECMD_SprintfRetEnd(WCHAR *dst, uint64_t v, LPCWSTR fmt)
{
    wsprintfW(dst, fmt, v);
    return dst + lstrlenW(dst);
}

/* ========== PECMD_FormatU64Dec @0x1400e6d74 ==========
 * 无符号 64 位十进制输出, 即 PECMD_SprintfRetEnd(dst, v, "%I64u").
 */
void PECMD_FormatU64Dec(WCHAR *dst, uint64_t v)
{
    PECMD_SprintfRetEnd(dst, v, WSTR("%I64u"));
}

/* ========== PECMD_FormatDblRetEnd @0x1400e6d80 ==========
 * 浮点输出. 原 PECMD_SafeVFormatW 内部为 StringValidateDestW +
 * StringVPrintfWorkerW(dst, 0x3e, NULL, fmt, (double)v), 简化用
 * swprintf(dst, 0x3e, fmt, (double)v).
 * TODO(verify): WCHAR 转 wchar_t 强转 (Linux wchar_t=4B), 由移植层承担.
 */
WCHAR *PECMD_FormatDblRetEnd(WCHAR *dst, float v, LPCWSTR fmt)
{
    (void)swprintf((wchar_t *)(void *)dst, 0x3e, (const wchar_t *)(const void *)fmt, (double)v);
    return dst + lstrlenW(dst);
}

/* ========== PECMD_FormatU64RetEnd @0x1400e6db4 ==========
 * 长浮点/64 位值输出 (%Lf/%lf 等), 同 PECMD_SafeVFormatW 简化.
 * TODO(verify): %Lf 在 Linux 下期望 16B long double, 与 Windows
 * (long double=double=8B) 不同, 运行时由移植层处理.
 */
WCHAR *PECMD_FormatU64RetEnd(WCHAR *dst, uint64_t v, LPCWSTR fmt)
{
    (void)swprintf((wchar_t *)(void *)dst, 0x3e, (const wchar_t *)(const void *)fmt, v);
    return dst + lstrlenW(dst);
}

/* ========== PECMD_FormatTypedMemValue @0x14006d9d0 ==========
 * 变量格式化: node=VarNode(值@+8, 长度掩码@+0x18 & 0x3fffffffffffffff 为字节长).
 * spec 前缀类型: char→1 / wchar|short→2 / long|int64|ptr→8 / float→4(浮点) /
 * double|ldouble→8(双浮点) / int→4(第4字符为大写字母且非 'S' 时回退默认 1).
 * 然后 '0' 跳过, '@'→"%I64X", 's'→按元素宽走 %d; width: '~' 前缀乘元素宽 +
 * 数字偏移; 截断: 数据不足时偏移 = max(0, 元素宽-数据长).
 * 复制到 8 字节局部后按格式输出并回填 *lenOut = lstrlenW(dst).
 */
WCHAR *PECMD_FormatTypedMemValue(int64_t node, uint64_t *lenOut, WCHAR *spec, WCHAR *dst,
                                 WCHAR *width)
{
    WCHAR *value = *(WCHAR **)(node + 8);
    uint64_t dataLen = *(uint64_t *)(node + 0x18) & 0x3fffffffffffffffULL;
    *lenOut = dataLen / 2;
    if (spec == NULL) {
        return value;
    }
    uint8_t elmW = 1;       /* 元素宽 (字节) */
    bool isFloat = false;   /* float 标志 */
    bool isLDouble = false; /* ldouble 标志 */
    uint8_t dblFlag = 0;    /* double/ldouble → 8 */
    LPCWSTR fmt = WSTR("0x%I64X");
    uint64_t off = 0;   /* 偏移 (元素) */
    uint64_t sflag = 0; /* 's' 修饰时 = 元素宽 */
    uint8_t mulW = 0;   /* '~' 宽度乘数 */
    uint64_t val = 0;   /* 8 字节值缓冲 (local_58) */

    if (FUN_14005C788("char", spec, 4)) {
        spec += 4;
    }
    else if (FUN_14005C788("wchar", spec, 5) || FUN_14005C788("short", spec, 5)) {
        spec += 5;
        elmW = 2;
    }
    else if (FUN_14005C788("long", spec, 4)) {
        spec += 4;
        elmW = 4;
    }
    else if (FUN_14005C788("int64", spec, 5)) {
        spec += 5;
        elmW = 8;
    }
    else if (FUN_14005C788("ptr", spec, 3)) {
        spec += 3;
        elmW = 8;
    }
    else if (FUN_14005C788("float", spec, 5)) {
        spec += 5;
        elmW = 4;
        isFloat = true;
    }
    else if (FUN_14005C788("double", spec, 6)) {
        spec += 6;
        elmW = 8;
        dblFlag = 8;
    }
    else if (FUN_14005C788("ldouble", spec, 7)) {
        spec += 7;
        elmW = 8;
        dblFlag = 8;
        isLDouble = true;
    }
    else if (FUN_14005C788("int", spec, 3)) {
        /* 第 4 字符是大写字母 (非 'S') → 非 int 类型, 保持默认宽 1 */
        WCHAR c4 = (WCHAR)(spec[3] & 0xffdf);
        if (c4 == 0x53 || c4 <= 0x40 || c4 >= 0x5b) {
            spec += 3;
            elmW = 4;
        }
    }
    if (*spec == L'0') {
        spec++;
    }
    if (*spec == L'@') {
        spec++;
        fmt = WSTR("%I64X");
    }
    if (*spec == L's') {
        sflag = elmW;
    }
    if (width != NULL) {
        if (*width == L'~') {
            width++;
            mulW = elmW;
        }
        FUN_1400C1194((LPCWSTR *)&width, &off);
        if (mulW != 0) {
            off = off * (int64_t)(int8_t)mulW;
        }
    }
    /* 截断: 数据长(dataLen 字节) < 元素宽 + 偏移 时, 偏移 = 元素宽 - 数据长
     * (负则归零, 即偏移不超过数据末尾 - 元素宽) */
    if (dataLen < (uint64_t)((int64_t)(int8_t)elmW + (int64_t)off)) {
        off = (uint64_t)((int64_t)(int8_t)elmW - (int64_t)dataLen);
        if ((int64_t)off < 0) {
            off = 0;
        }
    }
    /* dc:68740 偏移 off 为字节域(dataLen 字节、'~' 乘 elmW 字节), value+off 必须按字节寻址;
     * msvc value 为 WCHAR* → 元素域 +2×off 倍差 (R25-h S4) */
    memcpy(&val, (uint8_t *)value + off, elmW);
    dst[0] = L'\0';
    if (isLDouble) {
        PECMD_FormatU64RetEnd(dst, val, WSTR("%Lf"));
    }
    else if (dblFlag == 0) {
        if (isFloat) {
            /* TODO(verify): (float)val 为整数→浮点转换, 与位模式重解释
             * (*(float *)&val) 语义不同, 原反编译显示 (float) 转换 */
            PECMD_FormatDblRetEnd(dst, (float)val, WSTR("%f"));
        }
        else if (sflag == 1 || sflag == 2 || sflag == 4) {
            /* Ghidra 丢参, 已补第 3 参 */
            wsprintfW(dst, WSTR("%d"), (int)val);
        }
        else {
            PECMD_SprintfRetEnd(dst, val, fmt);
        }
    }
    else {
        PECMD_FormatU64RetEnd(dst, val, WSTR("%lf"));
    }
    *lenOut = (uint64_t)lstrlenW(dst);
    return dst;
}
