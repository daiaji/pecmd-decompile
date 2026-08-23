/* ====================================================================
 * core_b8e.c — B8 小函数批 5 (1400e0000-1400fffff)
 *
 * 本批新实现函数全部使用人类可读 PECMD_ 名称，原始地址保留在 @0x 注释。
 *
 * 来源: PECMD原始.EXE (x64, ImageBase=0x140000000)
 *   调用 ZwUnmapViewOfSection  PECMD_ZwUnmapViewOfSection @0x1400e4228
 *   更新窗口扩展样式位      FUN_1400E5960 @0x1400e5960
 *   子窗口命中测试回调      FUN_1400E6350 @0x1400e6350
 *   填充矩形背景色          FUN_1400E68E0 @0x1400e68e0
 *   初始化窗口查找结构      FUN_1400EC084 @0x1400ec084
 *   发送对象查找消息 0x432   FUN_1400EC6A8 @0x1400ec6a8
 *   销毁对象(带释放) F      PECMD_DtorWinObjWithStrings @0x1400eceb4
 *   初始化窗口对象 F        FUN_1400ECF18 @0x1400ecf18
 *   清空对象数组            PECMD_ClearStringItemList @0x1400f1490
 *   发送控件按下通知        PECMD_PostCtlPressNotify @0x1400f230c
 *   更新数组项值            PECMD_ItemPropUpsertEntry @0x1400f2b84
 *   查询组合框度量          FUN_1400F3554 @0x1400f3554
 *   设置控件高度并失效      FUN_1400F4064 @0x1400f4064
 *   转发取文本长度(带链接)  PECMD_CtlOnSetFontLinked @0x1400f4194
 *   查找数组项槽            FUN_1400F4C28 @0x1400f4c28
 *   添加映射项 A            PECMD_TrackItemChangeList1 @0x1400f5724
 *   添加映射项 B            PECMD_TrackItemChangeList2 @0x1400f578c
 *   添加映射项 C            PECMD_TrackItemChangeList3 @0x1400f586c
 *   添加映射项 D            PECMD_TrackItemChangeList4 @0x1400f58d4
 *   释放 GDI 对象数组       PECMD_ClearNamedPropArray @0x1400f5c10
 *   销毁 GDI 复合对象       FUN_1400F5D50 @0x1400f5d50
 *   取映射双值 A            PECMD_ItemPropGetPair24 @0x1400f5dc4
 *   取映射双值 B            PECMD_ItemPropGetPair13 @0x1400f5ef8
 *   取映射双值 C            PECMD_ItemPropGetPairSub @0x1400f6034
 *   刷新控件当前选择        PECMD_ListSelectFromHit @0x1400f6944
 *   发送控件按下通知 B      PECMD_PostCtlPressNotify2 @0x1400fbde0
 *   初始化 GDI 对象 C       FUN_1400FC2E0 @0x1400fc2e0
 *   初始化 GDI 对象 D       FUN_1400FEC9C @0x1400fec9c
 *   查询控件值              PECMD_TreeGetItemParam @0x1400fed38
 *   查询控件值(扩展)       FUN_1400FEE24 @0x1400fee24
 *
 * 约定:
 *   - 新实现函数使用 PECMD_ 可读名；未实现依赖仍 extern FUN_ + TODO(verify)
 *   - 全局数据符号先用 extern 声明; 完整链接时统一在 core_globals.c 定义
 * ==================================================================== */
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "pecmd_defs.h"

/* ---- 已实现公共工具 (其他 core_*.c) ---- */
extern void FUN_1400F2B6C(int64_t obj);            /* @0x1400f2b6c */

/* ---- 全局数据符号 ---- */
extern uint8_t PTR_FUN_14012bbf0[];
extern uint8_t PTR_FUN_14012c550[];
extern uint8_t PTR_FUN_14012cb90[];
extern uint8_t PTR_FUN_14012cf00[];

/* ---- 未实现依赖 (extern + TODO(verify)) ---- */
extern int FUN_1400E6314(HWND hwnd, POINT pt);
extern void PECMD_FreeNamedEntryArray(uint64_t *arr);
extern void FUN_1400E8940(uint64_t *obj);
extern void FUN_1400F0648(uint64_t *obj, uint64_t value);
extern void FUN_14005B0D4(void *ps);
extern void *FUN_140063B00(int64_t idx, int64_t *arr, int64_t *cap,
                           uint32_t esize);
extern void FUN_1400639F0(int64_t *arr, int64_t *cap, int64_t *cnt, void *data,
                          int64_t esize, int32_t mode);
extern void PECMD_FreeArray_ddf8(int64_t *arr);
extern void PECMD_ForcePosChanged(HWND hwnd);
extern int64_t PECMD_ItemPropFindIdxList2(int64_t obj, int idx, int *out);
extern int64_t PECMD_ItemPropFindIdxList4(int64_t obj, int idx, int *out);
extern int64_t PECMD_ItemPropFindIdxList1(int64_t obj, int idx, int *out);
extern int64_t PECMD_ItemPropFindIdxList3(int64_t obj, int idx, int *out);
extern int64_t PECMD_ItemPropFindIdxSub1(int64_t obj, int idx, int mode,
                             int *out);
extern int64_t PECMD_ItemPropFindIdxSub2(int64_t obj, int idx, int mode,
                             int *out);
extern void PECMD_ListSubItemHitTest(int64_t obj, int *out_index, int *out_flag); /* @0x1400f3308 */
extern void PECMD_TableSetCurSel(int64_t obj, int current, int scroll);
extern void *FUN_1400E57C0(void *obj);

/* ========== PECMD_ZwUnmapViewOfSection @0x1400e4228 ==========
 * 动态加载 ntdll!ZwUnmapViewOfSection 并调用；返回 NTSTATUS==0。
 */
bool PECMD_ZwUnmapViewOfSection(HANDLE process, void *baseAddress)
{
    typedef int32_t (*ZwUnmapViewOfSectionFn)(HANDLE, void *);
    bool ok = false;
    HMODULE hNtdll = LoadLibraryA("ntdll.dll");

    if (hNtdll != (HMODULE)0) {
        void *proc = GetProcAddress(hNtdll, "ZwUnmapViewOfSection");
        if (proc != NULL) {
            ZwUnmapViewOfSectionFn fn = (ZwUnmapViewOfSectionFn)proc;
            ok = fn(process, baseAddress) == 0;
        }
        FreeLibrary(hNtdll);
    }
    return ok;
}

/* ========== FUN_1400E5960 @0x1400e5960 ==========
 * 按 clearBits/setBits 更新窗口扩展样式，返回样式是否发生变化。
 */
bool FUN_1400E5960(HWND hwnd, uint32_t clearBits, uint32_t setBits)
{
    LONG_PTR oldStyle = GetWindowLongPtrW(hwnd, GWL_EXSTYLE);
    LONG_PTR newStyle = (LONG_PTR)(((uint64_t)oldStyle & ~(uint64_t)clearBits) |
                                   (uint64_t)setBits);

    SetWindowLongPtrW(hwnd, GWL_EXSTYLE, newStyle);
    return oldStyle != newStyle;
}

/* ========== FUN_1400E6350 @0x1400e6350 ==========
 * EnumChildWindows 回调：在子窗口中查找指定点命中的窗口。
 * TODO(verify): FUN_1400E6314 的命中语义；ctx[3] 存放命中的 HWND。
 */
bool FUN_1400E6350(HWND hwnd, POINT *ctx)
{
    bool keepGoing;

    if (FUN_1400E6314(hwnd, *ctx) == 0) {
        keepGoing = true;
    } else {
        uint64_t hit = SendMessageW(hwnd, 0x45e, (WPARAM)(int64_t)ctx[1].x,
                                    (LPARAM)(uintptr_t)hwnd);
        if ((hit >> 0x11 & 1) != 0) {
            *(uintptr_t *)(ctx + 3) = (uintptr_t)hwnd;
        }
        EnumChildWindows(hwnd, (void *)FUN_1400E6350, (LPARAM)(uintptr_t)ctx);
        keepGoing = *(uintptr_t *)(ctx + 3) == 0;
    }
    return keepGoing;
}

/* ========== FUN_1400E68E0 @0x1400e68e0 ==========
 * 设置背景色并用 ExtTextOutW(ETO_OPAQUE, NULL) 填充矩形，返回旧背景色。
 */
COLORREF FUN_1400E68E0(HDC hdc, RECT *rect, COLORREF color)
{
    COLORREF oldColor = SetBkColor(hdc, color);

    ExtTextOutW(hdc, 0, 0, 2, rect, NULL, 0, NULL);
    return oldColor;
}

/* ========== FUN_1400EC084 @0x1400ec084 ==========
 * 初始化 0x38 字节的窗口查找/配对消息结构。
 * TODO(verify): 首参原函数未使用；字段语义按消息 0x432/0x113e 推断。
 */
void FUN_1400EC084(uint64_t *out, int64_t obj, uint64_t param4)
{
    HWND hwnd;
    HWND parent;

    memset(out, 0, 0x38);
    *(uint32_t *)out = 0x38;
    hwnd = *(HWND *)(obj + OBJ_HWND);
    if (param4 == 0) {
        parent = GetParent(hwnd);
        *(uint32_t *)((char *)out + 4) = 1;
        out[2] = (uint64_t)hwnd;
        out[1] = (uint64_t)parent;
    } else {
        *(uint32_t *)((char *)out + 4) = 0;
        out[1] = (uint64_t)hwnd;
        out[2] = param4;
    }
}

/* ========== FUN_1400EC6A8 @0x1400ec6a8 ==========
 * 构造 0x38 字节查找结构，附带最多 0x10 字节附加数据，发送消息 0x432。
 */
void FUN_1400EC6A8(int64_t obj, int64_t target, uint64_t param3,
                                   const void *extra, uint64_t param5)
{
    uint64_t msg[7];

    FUN_1400EC084(msg, target, param5);
    if (extra != NULL) {
        memcpy((char *)msg + 0x18, extra, 0x10);
    }
    *(uint64_t *)((char *)msg + 0x30) = param3;
    SendMessageW(*(HWND *)(obj + OBJ_HWND), 0x432, 0, (LPARAM)msg);
}

/* ========== PECMD_DtorWinObjWithStrings @0x1400eceb4 ==========
 * 销毁对象 F；清理 +0x108 与 +0xd8 处资源后调用基类析构。
 * flags&1 时同时 free 容器。
 */
uint64_t *PECMD_DtorWinObjWithStrings(uint64_t *obj, uint32_t flags)
{
    PECMD_FreeNamedEntryArray(obj + 0x21);
    PECMD_FreeStrBuf((WCHAR **)(obj + 0x21));
    PECMD_FreeStrBuf((WCHAR **)(obj + 0x1b));
    FUN_1400E8940(obj);
    if ((flags & 1) != 0) {
        free(obj);
    }
    return obj;
}

/* ========== FUN_1400ECF18 @0x1400ecf18 ==========
 * 初始化窗口对象 F：调用基类构造后设置标志/尺寸/虚表。
 */
uint64_t *FUN_1400ECF18(uint64_t *obj, uint64_t param2)
{
    FUN_1400F0648(obj, param2);
    *(uint32_t *)((char *)obj + 0xf8) = 0x80000000;
    *(uint16_t *)((char *)obj + 0xea) = 0x60;
    *(uint8_t *)((char *)obj + OBJ_FLAGS) = 0;
    obj[0x1e] = 0;
    *obj = (uint64_t)(uintptr_t)PTR_FUN_14012bbf0;
    *(uint16_t *)((char *)obj + 0xfe) = 0;
    *(uint16_t *)((char *)obj + 0xfc) = 0;
    *(uint8_t *)((char *)obj + 0xe9) = 0;
    return obj;
}

/* ========== PECMD_ClearStringItemList @0x1400f1490 ==========
 * 清空数组：逐个释放元素内的子对象，再释放数组缓冲。
 */
void PECMD_ClearStringItemList(int64_t arr)
{
    uint8_t *base = (uint8_t *)arr;
    int64_t data = *(int64_t *)(base + 8);
    int count = *(int *)(base + 0x10);
    int i;

    for (i = 0; i < count; i++) {
        if (*(int64_t *)(data + 0x10 + (int64_t)i * 0x28) != 0) {
            FUN_14005B0D4((void *)(data + 0x10 + (int64_t)i * 0x28));
        }
    }
    PECMD_FreeStrBuf((WCHAR **)(base + 8));
}

/* ========== PECMD_PostCtlPressNotify @0x1400f230c ==========
 * 通过虚表把 0x201 按下事件发给子控件；若未屏蔽则向父窗口投递 WM_COMMAND。
 */
void PECMD_PostCtlPressNotify(uint64_t *obj, uint32_t wParam, uint64_t packedXY)
{
    void (*fn)(uint64_t, uint32_t, uint32_t, uint64_t) =
        *(void (**)(uint64_t, uint32_t, uint32_t, uint64_t))(*obj + 8);
    int32_t lParam32 = (int32_t)(((uint32_t)(packedXY >> 32) << 16) |
                                 (uint32_t)packedXY);

    fn(obj[4], 0x201, wParam, (int64_t)lParam32);
    if ((*(uint8_t *)((char *)obj + OBJ_LINK) & 1) == 0) {
        int id = GetDlgCtrlID((HWND)obj[4]);
        HWND parent = GetParent((HWND)obj[4]);
        PostMessageW(parent, 0x111, (WPARAM)(uint16_t)id, (LPARAM)0x201);
    }
}

/* ========== PECMD_ItemPropUpsertEntry @0x1400f2b84 ==========
 * 更新数组中已存在项：value<0 时释放并清空；否则更新 key/value/扩展字段。
 * TODO(verify): 返回值保留 Ghidra 的指针低字节清零形式，调用方仅关心 AL。
 */
uint64_t PECMD_ItemPropUpsertEntry(int64_t *array, int64_t index, int32_t value,
                               uint32_t field1, int32_t field2)
{
    void **slot;
    void *item;

    if (index < 0) {
        return 0;
    }
    slot = (void **)FUN_140063B00(index, array, array + 1, 8);
    item = *slot;
    if (item != NULL) {
        if (value < 0) {
            *slot = NULL;
            free(item);
        } else {
            *(int64_t *)((char *)item + 8) = value;
            *(uint32_t *)item = field1;
            if (field2 > -2) {
                *(int32_t *)((char *)item + 16) = field2;
            }
        }
    }
    return (uint64_t)(uintptr_t)slot & 0xffffffffffffff00ULL;
}

/* ========== FUN_1400F3554 @0x1400f3554 ==========
 * 查询并设置组合框/下拉框度量值。TODO(verify): 消息 0x104d/0x102b 字段语义。
 */
uint64_t FUN_1400F3554(int64_t obj, LPARAM param2)
{
    uint8_t buf[20];
    uint64_t value;

    value = (uint64_t)SendMessageW(*(HWND *)(obj + OBJ_HWND), 0x104d, 0, param2);
    *(uint32_t *)(buf + 12) = 0x1000;
    *(uint32_t *)(buf + 16) = 0xf000;
    SendMessageW(*(HWND *)(obj + OBJ_HWND), 0x102b,
                 (WPARAM)(int64_t)(int32_t)value, (LPARAM)buf);
    return value & 0xffffffff;
}

/* ========== FUN_1400F4064 @0x1400f4064 ==========
 * 设置控件高度字段；按 mode 决定是否/如何使控件失效重绘。
 * TODO(verify): +0x218 字段语义为高度/尺寸。
 */
void FUN_1400F4064(int64_t obj, int height, int mode)
{
    HWND hwnd;
    BOOL erase;

    if (height > -2) {
        *(int *)(obj + 0x218) = height;
    }
    if (mode != 1) {
        hwnd = *(HWND *)(obj + OBJ_HWND);
        if (mode == 2) {
            erase = FALSE;
        } else {
            PECMD_ForcePosChanged(hwnd);
            if (height > -2) {
                return;
            }
            hwnd = *(HWND *)(obj + OBJ_HWND);
            erase = TRUE;
        }
        InvalidateRect(hwnd, NULL, erase);
    }
}

/* ========== PECMD_CtlOnSetFontLinked @0x1400f4194 ==========
 * 对象未进入自定义模式时，把 WM_GETTEXTLENGTH(0x30) 转发给子窗口，
 * 并向 +0xf8 关联窗口发送 0x452。
 */
LRESULT PECMD_CtlOnSetFontLinked(int64_t obj, WPARAM wParam, LPARAM lParam)
{
    LRESULT result;

    if (*(int64_t *)(obj + OBJ_CUSTOMMODE) == 0) {
        *(WPARAM *)(obj + OBJ_WPARAM) = wParam;
        result = SendMessageW(*(HWND *)(obj + OBJ_HWND), 0x30, wParam, lParam);
        SendMessageW(*(HWND *)(obj + 0xf8), 0x452, wParam, lParam);
    } else {
        result = 0;
    }
    return result;
}

/* ========== FUN_1400F4C28 @0x1400f4c28 ==========
 * 在 +0x338/+0x340 数组里按 +8 处值查找项，返回项槽指针；未找到返回 NULL。
 */
int64_t *FUN_1400F4C28(int64_t obj, int64_t value)
{
    int count = *(int *)(obj + 0x348);
    int i;

    for (i = 0; i < count; i++) {
        int64_t *slot = (int64_t *)FUN_140063B00(i, (int64_t *)(obj + 0x338),
                                                 (int64_t *)(obj + 0x340), 8);
        if ((*slot != 0) && (*(int64_t *)(*slot + 8) == value)) {
            return slot;
        }
    }
    return NULL;
}

/* ---- 内部辅助：把 {key,value} 节点追加到映射数组 ---- */
static void PECMD_AddMapEntryToArray(int64_t *array, int64_t *cap, int64_t *cnt,
                                     uint32_t key, uint64_t value)
{
    uint32_t *node = (uint32_t *)calloc(1, 0x18);

    node[0] = key;
    *(uint64_t *)(node + 2) = value & 0xffffffff;
    FUN_1400639F0(array, cap, cnt, &node, 8, 1);
}

/* ========== PECMD_TrackItemChangeList1 @0x1400f5724 ==========
 * 向 +0x260 映射数组添加 {key,value}。
 */
void PECMD_TrackItemChangeList1(int64_t obj, uint32_t key, uint64_t value)
{
    PECMD_AddMapEntryToArray((int64_t *)(obj + 0x260), (int64_t *)(obj + 0x268),
                             (int64_t *)(obj + 0x270), key, value);
}

/* ========== PECMD_TrackItemChangeList2 @0x1400f578c ==========
 * 向 +0x278 映射数组添加 {key,value}。
 */
void PECMD_TrackItemChangeList2(int64_t obj, uint32_t key, uint64_t value)
{
    PECMD_AddMapEntryToArray((int64_t *)(obj + 0x278), (int64_t *)(obj + 0x280),
                             (int64_t *)(obj + 0x288), key, value);
}

/* ========== PECMD_TrackItemChangeList3 @0x1400f586c ==========
 * 向 +0x2a8 映射数组添加 {key,value}。
 */
void PECMD_TrackItemChangeList3(int64_t obj, uint32_t key, uint64_t value)
{
    PECMD_AddMapEntryToArray((int64_t *)(obj + 0x2a8), (int64_t *)(obj + 0x2b0),
                             (int64_t *)(obj + 0x2b8), key, value);
}

/* ========== PECMD_TrackItemChangeList4 @0x1400f58d4 ==========
 * 向 +0x2c0 映射数组添加 {key,value}。
 */
void PECMD_TrackItemChangeList4(int64_t obj, uint32_t key, uint64_t value)
{
    PECMD_AddMapEntryToArray((int64_t *)(obj + 0x2c0), (int64_t *)(obj + 0x2c8),
                             (int64_t *)(obj + 0x2d0), key, value);
}

/* ========== PECMD_ClearNamedPropArray @0x1400f5c10 ==========
 * 释放数组中的 GDI 对象：释放内部字符串、删除 GDI 对象并 free 节点。
 */
void PECMD_ClearNamedPropArray(int64_t *array)
{
    uint8_t *base = (uint8_t *)array[0];
    int64_t count = array[2];

    array[2] = 0;
    while (count > 0) {
        void *item;
        count--;
        item = *(void **)(base + count * 8);
        if (item != NULL) {
            PECMD_FreeStrBuf((WCHAR **)((char *)item + 0x10));
            FUN_1400F2B6C((int64_t)item);
            free(item);
            *(uint64_t *)(base + count * 8) = 0;
        }
    }
}

/* ========== FUN_1400F5D50 @0x1400f5d50 ==========
 * 销毁 GDI 复合对象：删除 GDI 对象、释放多个字符串/数组后调用基类析构。
 */
void FUN_1400F5D50(uint64_t *obj)
{
    *obj = (uint64_t)(uintptr_t)PTR_FUN_14012c550;
    if ((HGDIOBJ)obj[0x22] != (HGDIOBJ)0) {
        DeleteObject((HGDIOBJ)obj[0x22]);
        obj[0x22] = 0;
    }
    PECMD_FreeStrBuf((WCHAR **)(obj + 0x20));
    PECMD_FreeStrBuf((WCHAR **)(obj + 0x1d));
    PECMD_FreeArray_ddf8((int64_t *)(obj + 0x1a));
    PECMD_FreeStrBuf((WCHAR **)(obj + 0x1a));
    FUN_1400E8940(obj);
}

/* ========== PECMD_ItemPropGetPair24 @0x1400f5dc4 ==========
 * 分别查 +0x278 与 +0x2c0 映射数组；找到时写出/覆盖返回值。
 */
int PECMD_ItemPropGetPair24(int64_t obj, int key, int fallback, int *out)
{
    int value;
    int64_t idx;

    idx = PECMD_ItemPropFindIdxList2(obj, key, &value);
    if (idx >= 0) {
        *out = value;
    }
    idx = PECMD_ItemPropFindIdxList4(obj, key, &value);
    if (idx >= 0) {
        fallback = value;
    }
    return fallback;
}

/* ========== PECMD_ItemPropGetPair13 @0x1400f5ef8 ==========
 * 分别查 +0x260 与 +0x2a8 映射数组；找到时写出/覆盖返回值。
 */
int PECMD_ItemPropGetPair13(int64_t obj, int key, int fallback, int *out)
{
    int value;
    int64_t idx;

    idx = PECMD_ItemPropFindIdxList1(obj, key, &value);
    if (idx >= 0) {
        *out = value;
    }
    idx = PECMD_ItemPropFindIdxList3(obj, key, &value);
    if (idx >= 0) {
        fallback = value;
    }
    return fallback;
}

/* ========== PECMD_ItemPropGetPairSub @0x1400f6034 ==========
 * 分别查 +0x290 与 +0x2d8 双键映射数组；找到时写出/覆盖返回值。
 */
int PECMD_ItemPropGetPairSub(int64_t obj, int key1, int key2, int fallback, int *out)
{
    int value;
    int64_t idx;

    idx = PECMD_ItemPropFindIdxSub1(obj, key1, key2, &value);
    if (idx >= 0) {
        *out = value;
    }
    idx = PECMD_ItemPropFindIdxSub2(obj, key1, key2, &value);
    if (idx >= 0) {
        fallback = value;
    }
    return fallback;
}

/* ========== PECMD_ListSelectFromHit @0x1400f6944 ==========
 * 刷新控件当前选择/项索引；根据标志从缓存数组取值并回调更新函数。
 */
void PECMD_ListSelectFromHit(int64_t obj)
{
    int local_res8[6];
    int local_res20[2];

    local_res8[0] = -2;
    local_res20[0] = -2;
    PECMD_ListSubItemHitTest(obj, local_res8, local_res20);
    if ((*(int *)(obj + 0x3c8) >= 0) && ((*(uint8_t *)(obj + 0x3f8) & 2) != 0)) {
        int64_t data;
        int value;

        if (*(int64_t *)(obj + 0x388) < 1) {
            data = 0;
        } else {
            data = *(int64_t *)(obj + 0x380);
        }
        value = local_res8[0];
        if (data != 0) {
            value = *(int *)(data + (int64_t)local_res8[0] * 8);
        }
        PECMD_TableSetCurSel(obj, value, local_res20[0]);
    }
}

/* ========== PECMD_PostCtlPressNotify2 @0x1400fbde0 ==========
 * 通过虚表发送 0x201；若 +0xd0 低字节为 0 则向父窗口投递 WM_COMMAND。
 */
void PECMD_PostCtlPressNotify2(uint64_t *obj, uint32_t wParam, uint64_t packedXY)
{
    void (*fn)(uint64_t, uint32_t, uint32_t, uint64_t) =
        *(void (**)(uint64_t, uint32_t, uint32_t, uint64_t))(*obj + 8);
    int32_t lParam32 = (int32_t)(((uint32_t)(packedXY >> 32) << 16) |
                                 (uint32_t)packedXY);

    fn(obj[4], 0x201, wParam, (int64_t)lParam32);
    if (*(uint8_t *)((char *)obj + OBJ_LINK) == 0) {
        int id = GetDlgCtrlID((HWND)obj[4]);
        HWND parent = GetParent((HWND)obj[4]);
        PostMessageW(parent, 0x111, (WPARAM)(uint16_t)id, (LPARAM)0x201);
    }
}

/* ========== FUN_1400FC2E0 @0x1400fc2e0 ==========
 * 初始化 GDI 对象 C：调用基类构造后设置字符串/句柄字段和虚表。
 */
uint64_t *FUN_1400FC2E0(uint64_t *obj, uint64_t param2)
{
    FUN_1400E57C0(obj);
    obj[0x1b] = param2;
    *(uint8_t *)((char *)obj + 0xa0) = 0;
    *(uint8_t *)((char *)obj + 0xb8) = 0;
    obj[0x1a] = 0;
    obj[0x1d] = 0;
    obj[0x1e] = 0;
    *obj = (uint64_t)(uintptr_t)PTR_FUN_14012cb90;
    *(uint8_t *)((char *)obj + 0x61) = 0x18;
    return obj;
}

/* ========== FUN_1400FEC9C @0x1400fec9c ==========
 * 初始化 GDI 对象 D：调用基类构造后设置句柄/附加指针字段和虚表。
 */
uint64_t *FUN_1400FEC9C(uint64_t *obj, uint64_t param2, uint64_t param3)
{
    FUN_1400E57C0(obj);
    obj[0x1a] = param3;
    *(uint8_t *)((char *)obj + 0xa0) = 0;
    obj[0x1b] = 0;
    *(uint32_t *)((char *)obj + OBJ_TEXTCOLOR) = 0;
    obj[0x21] = param2;
    *obj = (uint64_t)(uintptr_t)PTR_FUN_14012cf00;
    return obj;
}

/* ========== PECMD_TreeGetItemParam @0x1400fed38 ==========
 * 发送 0x113e 查询结构；成功时把 +0x30 处的 64 位结果写入 *out。
 */
void PECMD_TreeGetItemParam(int64_t obj, uint64_t param2, uint64_t *out)
{
    uint8_t buf[0x38];
    LRESULT result;

    memset(buf, 0, sizeof(buf));
    *out = (uint64_t)-1;
    *(uint32_t *)(buf + 0) = 4;
    *(uint64_t *)(buf + 8) = param2;
    result = SendMessageW(*(HWND *)(obj + OBJ_HWND), 0x113e, 0, (LPARAM)buf);
    if ((int32_t)result != 0) {
        *out = *(uint64_t *)(buf + 0x30);
    }
}

/* ========== FUN_1400FEE24 @0x1400fee24 ==========
 * 发送 0x113e 查询结构；成功时把 +0x3c 处的 32 位结果写入 *out。
 */
int FUN_1400FEE24(int64_t obj, uint64_t param2, uint64_t *out)
{
    uint8_t buf[0x50];
    LRESULT result;

    memset(buf, 0, sizeof(buf));
    *out = 0;
    *(uint32_t *)(buf + 0) = 0x100;
    *(uint64_t *)(buf + 8) = param2;
    result = SendMessageW(*(HWND *)(obj + OBJ_HWND), 0x113e, 0, (LPARAM)buf);
    if ((int32_t)result != 0) {
        *out = *(uint32_t *)(buf + 0x3c);
    }
    return (int32_t)result;
}
