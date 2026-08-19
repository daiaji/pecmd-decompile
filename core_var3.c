/*
 * core_var3.c — 变量系统核心（B8a）
 *
 *   变量查找核心     PECMD_VarLookup      @0x140018978
 *   变量查找包装     FUN_14001E69C     @0x14001e69c
 *   变量写入         FUN_14001E6BC     @0x14001e6bc
 *   环境变量写入     FUN_14005D534       @0x14005d534
 *   随机种子         FUN_14001B510     @0x14001b510
 *   随机数发生器     FUN_14005DFF4      @0x14005dff4
 *   随机数种子源     FUN_14005E04C   @0x14005e04c
 *   XOR 编码         FUN_14001B5AC       @0x14001b5ac
 *   查找字符偏移     FUN_14001B4F8     @0x14001b4f8
 *   变量写(锁/直写)  FUN_1400629B8      @0x1400629b8 (已声明)
 */
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include <stdbool.h>

#include "pecmd_defs.h"
extern WCHAR **FUN_14005B154(WCHAR **pp); /* @0x14005b154 */


extern void *PECMD_GrowByteBuffer(void **ps, int64_t len); /* @0x140063424 */

/* 内部：单个变量节点插入 (operator_new(0x20) 布局) */
extern int64_t *PECMD_VarNodeNew(int64_t *node, LPCWSTR name, LPCWSTR value, int len, uint64_t cap);
/* 内部：直接写值 (无锁) */
extern void PECMD_VarWriteDirect(void *script, LPCWSTR key, LPCWSTR value);

extern void *PECMD_VarLookup(void *script, LPCWSTR name, void *scope,
                              int64_t len, void **out); /* @0x140018978 */

/* ========== FUN_14001E69C @0x14001e69c ==========
 * VarFind 包装：返回指向 value 字段（节点+8）的指针；未命中返回 NULL。
 */
uint8_t *FUN_14001E69C(void *script, LPCWSTR name, void *scope, int64_t len)
{
    uint8_t *n = PECMD_VarLookup(script, name, scope, len, NULL);
    if (n) n += 8;
    return n;
}

/* ========== FUN_14005D534 @0x14005d534 ==========
 * 环境变量写入（无锁，非 & 变量）。空名返回 0。
 */
static int32_t FUN_14005D534(const char *name, LPCWSTR value)
{
    if (*name == L'\0') return 0;
    return SetEnvironmentVariableW((LPCWSTR)name, value);
}

/* ========== FUN_14005DFF4 @0x14005dff4 ==========
 * 48 位 LCG 单步。
 */
void FUN_14005DFF4(void)
{
    g_lcgState = (g_lcgState * 0x5deece66d + 0xb) & 0xffffffffffff;
}

/* ========== FUN_14005E04C @0x14005e04c ==========
 * 随机种子源：QPC + LCG 混合。
 */
int64_t FUN_14005E04C(void)
{
    int64_t l2;
    int64_t qpc;
    QueryPerformanceCounter(&qpc);
    l2 = g_randState * 0x83 + qpc;
    do {
        FUN_14005DFF4();
        l2 = g_lcgState + l2 * 3;
    } while (l2 == g_randPrev);
    g_randPrev = l2;
    g_randState = l2;
    return l2;
}

/* ========== FUN_14001B510 @0x14001b510 ==========
 * 生成 16 位随机种子：要求偶数、低 8 位非 0、bit15=1、汉明权重在 5..11。
 */
uint16_t FUN_14001B510(void)
{
    uint16_t v = 0;
    int hw = 0;
    do {
        uint16_t t = (uint16_t)(FUN_14005E04C() + GetTickCount());
        int i;
        hw = 0;
        v = t;
        for (i = 0; i < 16; i++) {
            if (v & 1) hw++;
            v >>= 1;
        }
        v = t;
    } while (((v & 1) == 0) || ((v & 0xf0) == 0) || ((v & 0xf00) == 0) ||
             ((v & 0x8000) != 0x8000) || (6 < (uint16_t)(hw - 5U)));
    return v;
}

/* ========== FUN_14001B5AC @0x14001b5ac ==========
 * 用 key 对 buf 前 n 个字异或（n<1 用 strlen+1）。返回 key<<16。
 */
int32_t FUN_14001B5AC(LPCWSTR buf, uint32_t key, int64_t n)
{
    uint16_t k;
    if ((int32_t)key < 0) {
        k = FUN_14001B510();
        key = k;
    }
    if (n < 1) {
        n = lstrlenW(buf) + 1;
    }
    while (n-- > 0) {
        *(WCHAR *)buf = *(WCHAR *)buf ^ (uint16_t)key;
        buf++;
    }
    return (int32_t)(key << 16);
}

/* ========== FUN_14001B4F8 @0x14001b4f8 ==========
 * 返回从 buf 到 ch 的字符偏移（不含 ch）。
 */
uint64_t FUN_14001B4F8(const WCHAR *buf, WCHAR ch)
{
    const WCHAR *p;
    for (p = buf; *p != ch; p++) {
    }
    return (uint64_t)(p - buf);
}
