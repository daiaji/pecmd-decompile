/* ====================================================================
 * core_b3a.c — B3-B6 小函数批 (140040000-1400bffff)
 *
 * 来源: PECMD原始.EXE (x64, ImageBase=0x140000000)
 *   参数包装         PECMD_WrapParamCall_02d8 @0x1400402d8
 *   参数包装         PECMD_WrapParamCall_02e0 @0x1400402e0
 *   串比较包装       PECMD_CompareStringWrap @0x14004f8e8
 *   结构清零         PECMD_ZeroStruct_f934 @0x14004f934
 *   释放指针         PECMD_FreePointerSlot @0x14004fb2c
 *   区间夹取         PECMD_ClampInt @0x140053f3c
 *   三槽赋值         PECMD_SetThreeSlots @0x14005b01c
 *   释放容器         PECMD_FreeContainer @0x14005b134
 *   读 +0x20         PECMD_ReadPtr20 @0x14005b488
 *   数字/字母判定    PECMD_IsAlphaNumeric @0x14005bc28
 *   浮点存储         PECMD_StoreDouble_bc48 @0x14005bc48
 *   浮点存储         PECMD_StoreDouble_bc5c @0x14005bc5c
 *   空格填充         PECMD_FillSpaces @0x14005c6ab
 *   状态查询         PECMD_QueryState_c95c @0x14005c95c
 *   状态查询         PECMD_QueryState_cfc0 @0x14005cfc0
 *   虚表调用         PECMD_CallVtable @0x14005e500
 *   字节序合并       PECMD_MergeBytesLE @0x14005ffe0
 *   对齐取余         PECMD_AlignRemainder @0x14006006c
 *   消息发送         PECMD_SendCtrlMessage_18f8 @0x1400618f8
 *   魔数判断         PECMD_IsMagicNumber_1a28 @0x140061a28
 *   魔数判断         PECMD_IsMagicNumber_1a34 @0x140061a34
 *   循环计数         PECMD_IncrementCycle @0x140062bcc
 *   消息发送         PECMD_SendCtrlMessage_3d48 @0x140063d48
 *   消息发送         PECMD_SendCtrlMessage_3d64 @0x140063d64
 *   数组增长包装     PECMD_GrowArrayWrap @0x140065864
 *   转发             PECMD_ForwardCall_6820 @0x140066820
 *   转发             PECMD_ForwardCall_6838 @0x140066838
 *   串容器初始化     PECMD_InitStrContainer @0x140070294
 *   串复制到槽       PECMD_CopyStrToSlot @0x140070310
 *   条件转发         PECMD_ConditionalForward @0x1400aa12c
 *
 * 约定:
 *   - 未实现内部函数 extern 挂起 + TODO(verify), 不臆造语义
 *   - 全局数据符号先用 extern 声明; 完整链接时统一在 core_globals.c 定义
 * ==================================================================== */
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>

#include "pecmd_defs.h"

/* ---- 未实现依赖 (extern + TODO(verify)) ---- */
extern void PECMD_CreateVariable(int64_t *obj, uint64_t value, LPCWSTR text, int64_t *out);
extern void PECMD_ZeroLenBuf(void *p);
extern WCHAR **FUN_14005B154(WCHAR **pp);
extern int64_t FUN_140063B00(int64_t idx, int64_t *arr, int64_t *end, uint32_t esize);
extern void PECMD_TruncateObjectArray(int64_t *arr, int *count, int value);
extern void PECMD_RemoveObjectByKey(int64_t *arr, int *count, int64_t key);
/* S11: 本地声明与定义冲突已删除, 统一采用 xproto.h 原型 (原: /* S11: 本地声明与定义冲突, 已删除, 统一采用 xproto.h 原型 (原: extern void PECMD_WideToAnsiStr(int64_t *ps, LPCWSTR src, int64_t) */

extern void FUN_14007DF90(int64_t ctx, int mode);

/* ---- 本批引用的全局数据 ---- */
extern uint64_t (*g_pWIMCreateFile)(uint64_t wim, uint32_t mode, int f3, int f4, int f5,
                                    uint32_t *out);
extern int (*g_pWIMHandleOp480)(uint64_t h, uint64_t data, uint32_t mode);

/* ========== PECMD_WrapParamCall_02d8 @0x1400402d8 ==========
 * 参数包装: 调 PECMD_CreateVariable(..., NULL)。
 */
void PECMD_WrapParamCall_02d8(int64_t *obj, uint64_t value, LPCWSTR text)
{
    PECMD_CreateVariable(obj, value, text, NULL);
}

/* ========== PECMD_WrapParamCall_02e0 @0x1400402e0 ==========
 * 参数包装: 调 PECMD_CreateVariable(..., (int64_t*)1)。
 */
void PECMD_WrapParamCall_02e0(int64_t *obj, uint64_t value, LPCWSTR text)
{
    PECMD_CreateVariable(obj, value, text, (int64_t *)1);
}

/* ========== PECMD_CompareStringWrap @0x14004f8e8 ==========
 * 串比较包装: caseSensitive==0 忽略大小写, 否则区分大小写。
 */
int64_t PECMD_CompareStringWrap(LPCWSTR a, LPCWSTR b, int n, int caseSensitive)
{
    int iVar1;
    if (caseSensitive == 0) {
        iVar1 = StrCmpNIW(a, b, n);
    }
    else {
        iVar1 = StrCmpNW(a, b, n);
    }
    return (int64_t)iVar1;
}

/* ========== PECMD_ZeroStruct_f934 @0x14004f934 ==========
 * 28 字节结构清零。
 */
uint64_t *PECMD_ZeroStruct_f934(uint64_t *obj)
{
    obj[0] = 0;
    obj[1] = 0;
    obj[2] = 0;
    *(uint32_t *)(obj + 3) = 0;
    return obj;
}

/* ========== PECMD_FreePointerSlot @0x14004fb2c ==========
 * 释放首槽指针。
 */
void PECMD_FreePointerSlot(uint64_t *slot)
{
    if ((void *)slot[0] != NULL) {
        free((void *)slot[0]);
    }
}

/* ========== PECMD_ClampInt @0x140053f3c ==========
 * 将 *value 夹取到 [min, max]。
 */
void PECMD_ClampInt(int *value, int min, int max)
{
    if (*value < min) {
        *value = min;
        return;
    }
    if (max < *value) {
        *value = max;
    }
}

/* ========== PECMD_SetThreeSlots @0x14005b01c ==========
 * 三槽赋值: [2]=a [0]=b [1]=c。
 */
void PECMD_SetThreeSlots(uint64_t *slot, uint64_t a, uint64_t b, uint64_t c)
{
    slot[2] = a;
    slot[0] = b;
    slot[1] = c;
}

/* ========== PECMD_FreeContainer @0x14005b134 ==========
 * 释放容器内部指针后释放容器本身。
 */
static void PECMD_FreeContainer(int64_t *container)
{
    PECMD_ZeroLenBuf((uint64_t *)*container);
    PECMD_FreeStrBuf((WCHAR **)container);
}

/* ========== PECMD_ReadPtr20 @0x14005b488 ==========
 * 读结构 +0x20 的 8 字节。
 */
uint64_t PECMD_ReadPtr20(int64_t obj)
{
    uint64_t uVar1 = 0;
    if (obj != 0) {
        uVar1 = *(uint64_t *)(obj + 0x20);
    }
    return uVar1;
}

/* ========== PECMD_IsAlphaNumeric @0x14005bc28 ==========
 * 字母/数字判定: a-z/0-9 返回 1 (R14 注记: dc:54316 语义**不含 A-Z**,
 * 与甲体 @140061204/dc:58965 相区分 —— rename_map 撞名, 仲裁裁定合法共存;
 * 导出独占名 PECMD_IsAlnumLowerDigit 供 stubs 转发, 禁止借用甲体语义)。
 */
uint64_t PECMD_IsAlnumLowerDigit(uint16_t ch)
{
    if (((ch < 0x61) || (0x7a < ch)) && (9 < (uint16_t)(ch - 0x30))) {
        return 0;
    }
    return 1;
}

static uint64_t PECMD_IsAlphaNumeric(uint16_t ch)
{
    return PECMD_IsAlnumLowerDigit(ch);
}

/* ========== PECMD_StoreDouble_bc48 @0x14005bc48 ==========
 * 按标志存 double 或截断为 longlong。
 */
void PECMD_StoreDouble_bc48(int64_t obj, double *out, double value)
{
    if (*(char *)(obj + 10) != '\0') {
        *out = value;
        return;
    }
    *out = (double)(int64_t)value;
}

/* ========== PECMD_StoreDouble_bc5c @0x14005bc5c ==========
 * 按标志存 double 或截断为 longlong (与 48 相反)。
 */
void PECMD_StoreDouble_bc5c(int64_t obj, double *out, double value)
{
    if (*(char *)(obj + 10) != '\0') {
        *out = (double)(int64_t)value;
        return;
    }
    *out = value;
}

/* ========== PECMD_FillSpaces @0x14005c6ab ==========
 * 向串写入 count 个空格并跳过空白。
 */
void PECMD_FillSpaces(int64_t *cursor, int count)
{
    while (count = count - 1, -1 < count) {
        *(uint16_t *)*cursor = 0x20;
        *cursor += 2;
    }
    FUN_14005B154((WCHAR **)cursor);
}

/* ========== PECMD_QueryState_c95c @0x14005c95c ==========
 * 状态查询: 未初始化返回 -1, 否则返回是否可用。
 */
uint64_t PECMD_QueryState_c95c(void)
{
    if (g_pWIMCreateFile == 0) {
        return 0xffffffff;
    }
    return (uint64_t)(g_pWIMHandleOp480 != 0);
}

/* ========== PECMD_QueryState_cfc0 @0x14005cfc0 ==========
 * 状态查询: g_psock != 0。
 */
bool PECMD_QueryState_cfc0(void)
{
    return g_psock != 0;
}

/* ========== PECMD_CallVtable @0x14005e500 ==========
 * 若标志为 1 则调用对象虚表 +0x18 的函数。
 */
void PECMD_CallVtable(int64_t *obj)
{
    if ((char)obj[0x24] == 1) {
        void (**fn)(void) = (void (**)(void))(*obj + 0x18);
        (**fn)();
    }
}

/* ========== PECMD_MergeBytesLE @0x14005ffe0 ==========
 * 小端字节序合并为 uint64。
 */
uint64_t PECMD_MergeBytesLE(uint8_t *bytes, uint32_t count)
{
    uint64_t uVar2 = 0;
    if (0 < (int)count) {
        uint64_t uVar3 = count;
        do {
            uint8_t bVar1 = *bytes++;
            uVar2 = (uVar2 << 8) | bVar1;
            uVar3--;
        } while (uVar3 != 0);
    }
    return uVar2;
}

/* ========== PECMD_AlignRemainder @0x14006006c ==========
 * 返回对齐到 align 所需的补零长度。
 */
int64_t PECMD_AlignRemainder(int64_t value, int64_t align)
{
    int64_t lVar1 = 0;
    if (value % align != 0) {
        lVar1 = align - value % align;
    }
    return lVar1;
}

/* ========== PECMD_SendCtrlMessage_18f8 @0x1400618f8 ==========
 * 向窗口发送 0x467 消息。
 */
void PECMD_SendCtrlMessage_18f8(int64_t obj, LPARAM lParam)
{
    SendMessageW(*(HWND *)(obj + OBJ_HWND), 0x467, (WPARAM)(uintptr_t)g_hInst, lParam);
}

/* ========== PECMD_IsMagicNumber_1a28 @0x140061a28 ==========
 * 魔数判断: 0x10000。
 */
bool PECMD_IsMagicNumber_1a28(int *magic)
{
    return *magic == 0x10000;
}

/* ========== PECMD_IsMagicNumber_1a34 @0x140061a34 ==========
 * 魔数判断: 0x4d42 ('BM')。
 */
bool PECMD_IsMagicNumber_1a34(int16_t *magic)
{
    return *magic == 0x4d42;
}

/* ========== PECMD_IncrementCycle @0x140062bcc ==========
 * 循环计数: +0x1c 自增, 到 +0x18 后归零。
 */
void PECMD_IncrementCycle(int64_t obj)
{
    *(int *)(obj + 0x1c) = *(int *)(obj + 0x1c) + 1;
    if (*(int *)(obj + 0x18) <= *(int *)(obj + 0x1c)) {
        *(uint32_t *)(obj + 0x1c) = 0;
    }
}

/* ========== PECMD_SendCtrlMessage_3d48 @0x140063d48 ==========
 * 向窗口发送 0x405 消息。
 */
void PECMD_SendCtrlMessage_3d48(int64_t obj, LPARAM lParam)
{
    SendMessageW(*(HWND *)(obj + OBJ_HWND), 0x405, 1, lParam);
}

/* ========== PECMD_SendCtrlMessage_3d64 @0x140063d64 ==========
 * 向窗口发送 0x415 消息。
 */
void PECMD_SendCtrlMessage_3d64(int64_t obj, LPARAM lParam)
{
    SendMessageW(*(HWND *)(obj + OBJ_HWND), 0x415, 0, lParam);
}

/* ========== PECMD_GrowArrayWrap @0x140065864 ==========
 * 数组增长包装: 负数索引归零并置标志。
 */
void *PECMD_GrowArrayWrap(int64_t idx, int64_t *arr, int64_t *end, uint8_t *flags, uint32_t esize)
{
    if (idx < 0) {
        idx = 0;
        *flags |= 8;
    }
    return (void *)FUN_140063B00(idx, arr, end, esize);
}

/* ========== PECMD_ForwardCall_6820 @0x140066820 ==========
 * 转发到 PECMD_TruncateObjectArray。
 */
void PECMD_ForwardCall_6820(int64_t obj, int value)
{
    PECMD_TruncateObjectArray(*(int64_t **)(obj + 0x1b8), *(int **)(obj + 0x1c0), value);
}

/* ========== PECMD_ForwardCall_6838 @0x140066838 ==========
 * 转发到 PECMD_RemoveObjectByKey。
 */
void PECMD_ForwardCall_6838(int64_t obj, int64_t key)
{
    PECMD_RemoveObjectByKey(*(int64_t **)(obj + 0x1b8), *(int **)(obj + 0x1c0), key);
}

/* ========== PECMD_InitStrContainer @0x140070294 ==========
 * 串容器初始化并调用 PECMD_WideToAnsiStr。
 */
int64_t *PECMD_InitStrContainer(int64_t *ps, LPCWSTR src, int64_t len, uint64_t cap)
{
    *ps = 0;
    PECMD_WideToAnsiStr(ps, src, len, cap);
    return ps;
}

/* ========== PECMD_CopyStrToSlot @0x140070310 ==========
 * 复制 *src 指向的串到新容器并存入 *dst。
 */
uint64_t *PECMD_CopyStrToSlot(uint64_t *dst, uint64_t *src)
{
    int64_t *plVar1 = (int64_t *)PECMD_AllocMagicString((LPCWSTR)*src);
    *dst = (uint64_t)plVar1;
    return dst;
}

/* ========== PECMD_ConditionalForward @0x1400aa12c ==========
 * 条件转发: 首槽非空时调 FUN_14007DF90。
 */
void PECMD_ConditionalForward(int64_t obj, int mode)
{
    if (**(int16_t **)(obj + 0x10) != 0) {
        FUN_14007DF90(obj, mode);
    }
}
