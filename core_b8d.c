/* ====================================================================
 * core_b8d.c — B8 小函数批 4 (1400e0000-1400fffff)
 *
 * 本批新实现函数全部使用人类可读 PECMD_ 名称，原始地址保留在 @0x 注释。
 *
 * 来源: PECMD原始.EXE (x64, ImageBase=0x140000000)
 *   取窗口文本到串      FUN_1400E5730 @0x1400e5730
 *   设置颜色画刷        FUN_1400E5A58 @0x1400e5a58
 *   虚表 +0xf0 分发     FUN_1400E5AAC @0x1400e5aac
 *   点是否在窗口内      FUN_1400E6314 @0x1400e6314
 *   安全格式化串        PECMD_SafeVFormatW @0x1400e6960
 *   格式化并赋值串      FUN_1400E6CBC @0x1400e6cbc
 *   对齐扩容            FUN_1400E6CF8 @0x1400e6cf8
 *   鼠标按下分发        FUN_1400EC9C0 @0x1400ec9c0
 *   释放串指针数组      FUN_1400ECDD8 @0x1400ecdd8
 *   初始化 DC 对象      FUN_1400EEE30 @0x1400eee30
 *   刷新可见窗口        PECMD_RefreshControlVisibility @0x1400ef654
 *   初始化图标对象      FUN_1400EFEC8 @0x1400efec8
 *   销毁图标对象主体    FUN_1400EFF10 @0x1400eff10
 *   绘制对象            FUN_1400F05F8 @0x1400f05f8
 *   初始化控件对象 A    FUN_1400F11F8 @0x1400f11f8
 *   父窗口命令通知(位)  FUN_1400F1F28 @0x1400f1f28
 *   初始化窗口对象 B    FUN_1400F28E8 @0x1400f28e8
 *   消息预翻译          FUN_1400F2A00 @0x1400f2a00
 *   矩形折线绘制        FUN_1400F2BF0 @0x1400f2bf0
 *   按 ID 查找表项      FUN_1400F40C8 @0x1400f40c8
 *   释放 GDI 表项数组   FUN_1400F4208 @0x1400f4208
 *   释放普通表项数组    FUN_1400F425C @0x1400f425c
 *   跳到目标字符之后    FUN_1400F42C4 @0x1400f42c4
 *   父窗口命令通知(值)  FUN_1400FBA68 @0x1400fba68
 *   初始化控件对象 C    FUN_1400FBE58 @0x1400fbe58
 *   初始化带串窗口对象  FUN_1400FBFE0 @0x1400fbfe0
 *   交换关联窗口        FUN_1400FC458 @0x1400fc458
 *   创建 STATIC 控件    FUN_1400FD318 @0x1400fd318
 *   初始化静态控件      FUN_1400FE130 @0x1400fe130
 *   绘制对象 B          FUN_1400FEC58 @0x1400fec58
 *
 * 约定:
 *   - 新实现函数使用 PECMD_ 可读名；未实现依赖仍 extern FUN_ + TODO(verify)
 *   - 全局数据符号先用 extern 声明; 完整链接时统一在 core_globals.c 定义
 * ==================================================================== */
#include <stdlib.h>
#include <stdint.h>

#include "pecmd_defs.h"

/* ---- win32_stub.h 暂缺的 API ---- */
extern BOOL Polyline(HDC, const POINT *, int);

/* ---- 已实现公共工具 (其他 core_*.c) ---- */
extern void FUN_1400F2B6C(int64_t obj); /* @0x1400f2b6c */

/* ---- 未实现依赖 (extern + TODO(verify)) ---- */
extern void PECMD_AllocStrSlot(void *ps);
extern void *FUN_1400E57C0(uint64_t *obj);
extern void FUN_1400E8940(uint64_t *obj);
extern void PECMD_OnTabSelChange(int64_t obj);
extern void *FUN_1400F0648(uint64_t *obj, uint64_t value);
extern void FUN_1400F00F4(int64_t obj, HDC hdc, int64_t target,
                          int64_t overrideObj);
extern uint16_t FUN_1400F172C(int64_t *map, uint32_t msg,
                              uint64_t wParam, uint64_t *lParam,
                              int64_t hwnd, uint32_t mode,
                              int64_t *out);
extern BOOL FUN_1400FD220(int64_t *map, DWORD msg, LPCWSTR text,
                          uint32_t flags, int *out, HWND hwnd,
                          uint32_t mode);
extern uint64_t *FUN_1400FCF44(uint64_t *obj, uint64_t value);
extern void FUN_1400FE610(int64_t obj, HDC hdc); /* @0x1400fe610 */

/* ---- 本批引用的虚表/数据符号 ---- */
extern uint8_t PTR_FUN_14012bd70[];
extern uint8_t PTR_FUN_14012be90[];
extern uint8_t PTR_FUN_14012c0e0[];
extern uint8_t PTR_FUN_14012c410[];
extern uint8_t PTR_FUN_14012c900[];
extern uint8_t PTR_FUN_14012ca50[];
extern uint8_t PTR_FUN_14012cde0[];

/* ========== FUN_1400E5730 @0x1400e5730 ==========
 * 取窗口文本长度后分配串容器并读取窗口文本。
 */
void FUN_1400E5730(HWND hwnd, WCHAR **ps)
{
    int len = GetWindowTextLengthW(hwnd);
    PECMD_AllocString(ps, len + 2);
    (*ps)[0] = 0;
    GetWindowTextW(hwnd, *ps, len + 1);
}

/* ========== FUN_1400E5A58 @0x1400e5a58 ==========
 * 设置对象画刷颜色；COLORREF 为 -1 时只保存颜色不创建画刷。
 */
void FUN_1400E5A58(int64_t obj, COLORREF color, int64_t mode)
{
    HGDIOBJ old = *(HGDIOBJ *)(obj + OBJ_BRUSH);
    HBRUSH brush = (HBRUSH)0;

    if (mode == 0) {
        *(COLORREF *)(obj + OBJ_COLOR) = color;
    }
    if ((int)color >= 0) {
        brush = CreateSolidBrush(color);
    }
    *(HBRUSH *)(obj + OBJ_BRUSH) = brush;
    if (old != (HGDIOBJ)0) {
        DeleteObject(old);
    }
    InvalidateRect(*(HWND *)(obj + OBJ_HWND), (RECT *)0, 1);
}

/* ========== FUN_1400E5AAC @0x1400e5aac ==========
 * 调用对象虚表 +0xf0 处的 7 参回调。TODO(verify): 参数数组槽位语义未确认。
 */
int64_t FUN_1400E5AAC(int64_t *obj, uint64_t *args)
{
    int (*fn)(int64_t *, uint64_t, uint64_t, uint64_t, uint64_t, int, uint64_t) =
        *(int (**)(int64_t *, uint64_t, uint64_t, uint64_t, uint64_t, int, uint64_t))
            (*obj + 0xf0);
    return (int64_t)fn(obj, args[0], args[1], args[2], args[3], 0, args[6]);
}

/* ========== FUN_1400E6314 @0x1400e6314 ==========
 * 判断点是否在窗口矩形内。原 Ghidra 误标为 void，调用方实际使用返回值。
 */
BOOL FUN_1400E6314(HWND hwnd, POINT pt)
{
    RECT rc = { 0, 0, 0, 0 };
    GetWindowRect(hwnd, &rc);
    return PtInRect(&rc, pt);
}

/* ========== PECMD_SafeVFormatW @0x1400e6960 ==========
 * 原函数内部为 StringValidateDestW + StringVPrintfWorkerW，这里按项目
 * core_strbld.c 的同一简化方式使用 _snwprintf。TODO(verify): 仅 1 个可变参数。
 */
void PECMD_SafeVFormatW(WCHAR *dest, size_t cchDest, LPCWSTR fmt, uint64_t arg)
{
    (void)_snwprintf(dest, cchDest, fmt, arg);
}

/* ========== FUN_1400E6CBC @0x1400e6cbc ==========
 * 将单个参数按格式串格式化到临时缓冲，再赋值给字符串容器。
 */
void FUN_1400E6CBC(WCHAR **ps, uint64_t arg, LPCWSTR fmt)
{
    WCHAR buf[104];
    PECMD_SafeVFormatW(buf, 99, fmt, arg);
    FUN_1400703E4(ps, buf);
}

/* ========== FUN_1400E6CF8 @0x1400e6cf8 ==========
 * 字符串容器容量不足时按 align 对齐后扩容。
 */
uint64_t *FUN_1400E6CF8(uint64_t *container, int64_t needed,
                                      int64_t align)
{
    if ((int64_t)container[2] <= needed) {
        int64_t cap = ((needed - 1 + align) / align) * align;
        PECMD_AllocString(container, cap);
        container[2] = (uint64_t)cap;
    }
    return container;
}

/* ========== FUN_1400EC9C0 @0x1400ec9c0 ==========
 * 向子窗口发送 0x201 鼠标按下消息，随后执行列表切换等后续处理。
 * TODO(verify): 第 3 个参数的高低字打包为 lParam 的语义。
 */
void FUN_1400EC9C0(int64_t *obj, uint32_t param2, uint64_t param3)
{
    uint32_t packed = (uint32_t)(((uint32_t)(param3 >> 32) << 16) | (uint32_t)param3);
    void (*fn)(int64_t, uint32_t, WPARAM, LPARAM) =
        *(void (**)(int64_t, uint32_t, WPARAM, LPARAM))(*obj + 8);
    fn((int64_t)obj[4], 0x201, (WPARAM)param2,
       (LPARAM)(int64_t)(int32_t)packed);
    PECMD_OnTabSelChange((int64_t)obj);
}

/* ========== FUN_1400ECDD8 @0x1400ecdd8 ==========
 * 释放数组中的每个“串对象”：先释放对象内 +8 的宽字符串，再 free 对象本身。
 */
void FUN_1400ECDD8(uint64_t *arr)
{
    int64_t count = (int64_t)arr[2];
    uint64_t *base = (uint64_t *)arr[0];
    arr[2] = 0;
    while (count > 0) {
        count--;
        void *item = (void *)base[count];
        if (item != (void *)0) {
            FUN_14005B104((WCHAR **)((char *)item + 8));
            free(item);
            base[count] = 0;
        }
    }
}

/* ========== FUN_1400EEE30 @0x1400eee30 ==========
 * 初始化 DC 包装对象并设置虚表 PTR_FUN_14012bd70。
 */
uint64_t *FUN_1400EEE30(uint64_t *obj, uint64_t param2)
{
    FUN_1400F0648(obj, param2);
    *obj = (uint64_t)(uintptr_t)PTR_FUN_14012bd70;
    obj[0x1e] = 0;
    obj[0x1f] = 0;
    obj[0x22] = 0;
    obj[0x21] = 0;
    obj[0x20] = 0;
    *(uint8_t *)((char *)obj + OBJ_FLAGS) = 0;
    *(uint32_t *)((char *)obj + 0xa8) = 0x80000000;
    return obj;
}

/* ========== PECMD_RefreshControlVisibility @0x1400ef654 ==========
 * 窗口带 WS_VISIBLE 时执行一次隐藏再显示，用于刷新外观。
 */
void PECMD_RefreshControlVisibility(int64_t obj)
{
    uint32_t style = (uint32_t)GetWindowLongW(*(HWND *)(obj + OBJ_HWND), GWL_STYLE);
    if ((style >> 0x1c & 1) != 0) {
        ShowWindow(*(HWND *)(obj + OBJ_HWND), 0);
        ShowWindow(*(HWND *)(obj + OBJ_HWND), 5);
    }
}

/* ========== FUN_1400EFEC8 @0x1400efec8 ==========
 * 初始化图标对象并设置虚表 PTR_FUN_14012be90。
 */
uint64_t *FUN_1400EFEC8(uint64_t *obj)
{
    FUN_1400E57C0(obj);
    obj[0x1a] = 0;
    *(uint8_t *)((char *)obj + 0x60) = 0;
    obj[0x1c] = 0;
    *(uint32_t *)((char *)obj + 0xd8) = 0x80000000;
    *(uint8_t *)((char *)obj + 0xdc) = 1;
    *obj = (uint64_t)(uintptr_t)PTR_FUN_14012be90;
    return obj;
}

/* ========== FUN_1400EFF10 @0x1400eff10 ==========
 * 图标对象析构主体：按 +0xdc 标志选择 DestroyIcon 或 DeleteObject。
 */
void FUN_1400EFF10(uint64_t *obj)
{
    HICON icon;

    *obj = (uint64_t)(uintptr_t)PTR_FUN_14012be90;
    icon = (HICON)obj[0x1a];
    if (icon != (HICON)0) {
        if (*(char *)((char *)obj + 0xdc) == '\x01') {
            DestroyIcon(icon);
        } else if (*(char *)((char *)obj + 0xdc) == '\0') {
            DeleteObject((HGDIOBJ)icon);
        }
    }
    FUN_1400E8940(obj);
}

/* ========== FUN_1400F05F8 @0x1400f05f8 ==========
 * BeginPaint 后调用绘图回调 FUN_1400F00F4，再 EndPaint。
 */
void FUN_1400F05F8(int64_t obj)
{
    HWND hwnd = *(HWND *)(obj + OBJ_HWND);
    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(hwnd, &ps);
    FUN_1400F00F4(obj, hdc, obj, 0);
    EndPaint(hwnd, &ps);
}

/* ========== FUN_1400F11F8 @0x1400f11f8 ==========
 * 初始化控件对象 A 并设置虚表 PTR_FUN_14012c0e0。
 */
uint64_t *FUN_1400F11F8(uint64_t *obj, uint64_t param2)
{
    FUN_1400E57C0(obj);
    obj[0x1a] = param2;
    *obj = (uint64_t)(uintptr_t)PTR_FUN_14012c0e0;
    *(uint8_t *)((char *)obj + 0xa0) = 0;
    return obj;
}

/* ========== FUN_1400F1F28 @0x1400f1f28 ==========
 * 若 +0xd0 的 bit0 置位，则向父窗口发送 WM_COMMAND(控件ID, 0x203)。
 */
void FUN_1400F1F28(int64_t obj)
{
    if ((*(uint8_t *)(obj + OBJ_LINK) & 1) != 0) {
        HWND hwnd = *(HWND *)(obj + OBJ_HWND);
        int id = GetDlgCtrlID(hwnd);
        PostMessageW(GetParent(hwnd), WM_COMMAND, (WPARAM)(uint16_t)id,
                     (LPARAM)0x203);
    }
}

/* ========== FUN_1400F28E8 @0x1400f28e8 ==========
 * 初始化窗口对象 B 并设置虚表 PTR_FUN_14012c410。
 */
uint64_t *FUN_1400F28E8(uint64_t *obj, uint64_t param2)
{
    FUN_1400E57C0(obj);
    obj[0x1a] = param2;
    *obj = (uint64_t)(uintptr_t)PTR_FUN_14012c410;
    *(uint8_t *)((char *)obj + 0xa0) = 0;
    return obj;
}

/* ========== FUN_1400F2A00 @0x1400f2a00 ==========
 * 消息预翻译/过滤：走 FUN_1400F172C(0x233) 通道，命中时返回结果，否则 0x100。
 */
uint64_t FUN_1400F2A00(int64_t obj, int64_t msg)
{
    int64_t result = 0;
    uint16_t flags = FUN_1400F172C(*(int64_t **)(obj + OBJ_LINK), 0x233,
                                   *(uint64_t *)(msg + 0x10),
                                   *(uint64_t **)(msg + 0x18),
                                   *(int64_t *)(obj + OBJ_HWND),
                                   1, &result);
    if ((flags & 4) != 0) {
        return (uint64_t)result & 0xffffffff;
    }
    return 0x100;
}

/* ========== FUN_1400F2BF0 @0x1400f2bf0 ==========
 * 用 5 个折线点绘制矩形边框：(x0,y0)-(x1,y0)-(x1,y1)-(x0,y1)-(x0,y0)。
 */
void FUN_1400F2BF0(HDC hdc, LONG *rect)
{
    POINT pts[5];
    pts[0].x = rect[0]; pts[0].y = rect[1];
    pts[1].x = rect[2]; pts[1].y = rect[1];
    pts[2].x = rect[2]; pts[2].y = rect[3];
    pts[3].x = rect[0]; pts[3].y = rect[3];
    pts[4].x = rect[0]; pts[4].y = rect[1];
    Polyline(hdc, pts, 5);
}

/* ========== FUN_1400F40C8 @0x1400f40c8 ==========
 * 在数组 +0x308/count +0x318 中按项首 id 查找，返回下标并写出项数据。
 */
int64_t FUN_1400F40C8(int64_t obj, int id, uint64_t *outValue)
{
    int count = *(int *)(obj + 0x318);
    int64_t i;

    for (i = 0; i < count; i++) {
        int *item = *(int **)(*(int64_t *)(obj + 0x308) + i * 8);
        if ((item != (int *)0) && (item[0] == id)) {
            *outValue = *(uint64_t *)((char *)item + 8);
            return i;
        }
    }
    return -1;
}

/* ========== FUN_1400F4208 @0x1400f4208 ==========
 * 释放数组中的每个 GDI 表项：先 FUN_1400F2B6C 再 free。
 */
void FUN_1400F4208(uint64_t *arr)
{
    int64_t count = (int64_t)arr[2];
    uint64_t *base = (uint64_t *)arr[0];
    arr[2] = 0;
    while (count > 0) {
        count--;
        void *item = (void *)base[count];
        if (item != (void *)0) {
            FUN_1400F2B6C((int64_t)item);
            free(item);
            base[count] = 0;
        }
    }
}

/* ========== FUN_1400F425C @0x1400f425c ==========
 * 释放数组中的普通表项（仅 free，无额外清理）。
 */
void FUN_1400F425C(uint64_t *arr)
{
    int64_t count = (int64_t)arr[2];
    uint64_t *base = (uint64_t *)arr[0];
    arr[2] = 0;
    while (count > 0) {
        count--;
        void *item = (void *)base[count];
        if (item != (void *)0) {
            free(item);
            base[count] = 0;
        }
    }
}

/* ========== FUN_1400F42C4 @0x1400f42c4 ==========
 * 从当前 WCHAR 指针位置前进到指定字符，并越过该字符（若存在）。
 */
uint64_t *FUN_1400F42C4(uint64_t *slot, uint16_t ch)
{
    WCHAR **pp = (WCHAR **)slot;
    WCHAR *p = *pp;

    if (p != (WCHAR *)0) {
        while ((*p != 0) && (ch != *p)) {
            p++;
        }
        if (*p != 0) {
            p++;
        }
        *pp = p;
    }
    return slot;
}

/* ========== FUN_1400FBA68 @0x1400fba68 ==========
 * 若 +0xd0 非零，则向父窗口发送 WM_COMMAND(控件ID, 0x203)。
 */
void FUN_1400FBA68(int64_t obj)
{
    if (*(uint8_t *)(obj + OBJ_LINK) != 0) {
        HWND hwnd = *(HWND *)(obj + OBJ_HWND);
        int id = GetDlgCtrlID(hwnd);
        PostMessageW(GetParent(hwnd), WM_COMMAND, (WPARAM)(uint16_t)id,
                     (LPARAM)0x203);
    }
}

/* ========== FUN_1400FBE58 @0x1400fbe58 ==========
 * 初始化控件对象 C 并设置虚表 PTR_FUN_14012c900。
 */
uint64_t *FUN_1400FBE58(uint64_t *obj, uint64_t param2)
{
    FUN_1400E57C0(obj);
    obj[0x1a] = param2;
    *obj = (uint64_t)(uintptr_t)PTR_FUN_14012c900;
    *(uint8_t *)((char *)obj + 0xa0) = 0;
    *(uint8_t *)((char *)obj + 0x61) = 0x11;
    *(uint8_t *)((char *)obj + 0xb8) = 0;
    return obj;
}

/* ========== FUN_1400FBFE0 @0x1400fbfe0 ==========
 * 初始化带内部宽字符串的窗口对象，设置虚表 PTR_FUN_14012ca50。
 */
uint64_t *FUN_1400FBFE0(uint64_t *obj, uint64_t param2)
{
    FUN_1400E57C0(obj);
    obj[0x1a] = param2;
    *obj = (uint64_t)(uintptr_t)PTR_FUN_14012ca50;
    PECMD_AllocStrSlot(obj + 0x1b);
    *(uint8_t *)((char *)obj + 0xe4) = 0;
    *(uint32_t *)((char *)obj + OBJ_TEXTCOLOR) = 0x80000000;
    return obj;
}

/* ========== FUN_1400FC458 @0x1400fc458 ==========
 * 交换 +0xe8 关联窗口，返回旧值；新窗口非空时发送自定义消息。
 */
uint64_t FUN_1400FC458(int64_t obj, HWND hwnd)
{
    uint64_t old = *(uint64_t *)(obj + OBJ_FLAGS);
    *(HWND *)(obj + OBJ_FLAGS) = hwnd;
    if (hwnd != (HWND)0) {
        uint32_t msg = (uint32_t)((~(*(uint32_t *)(obj + OBJ_SUBWND) >> 6) & 1) | 0x114);
        SendMessageW(hwnd, msg, 5, *(LPARAM *)(obj + OBJ_HWND));
    }
    return old;
}

/* ========== FUN_1400FD318 @0x1400fd318 ==========
 * 使用 RECT 参数包装 FUN_1400FD220，创建 STATIC 控件。
 */
void FUN_1400FD318(int64_t *obj, LPCWSTR text, uint32_t style,
                               RECT *rect, HWND parent, uint32_t id)
{
    RECT rc = *rect;
    FUN_1400FD220(obj, 0, text, style, (int *)&rc, parent, id);
}

/* ========== FUN_1400FE130 @0x1400fe130 ==========
 * 初始化静态控件对象，设置虚表 PTR_FUN_14012cde0 并选用 NULL_BRUSH。
 */
uint64_t *FUN_1400FE130(uint64_t *obj, uint64_t param2)
{
    FUN_1400FCF44(obj, param2);
    *(uint8_t *)((char *)obj + 0xa4) = 3;
    *obj = (uint64_t)(uintptr_t)PTR_FUN_14012cde0;
    obj[7] = (uint64_t)(uintptr_t)(intptr_t)GetStockObject(5);
    return obj;
}

/* ========== FUN_1400FEC58 @0x1400fec58 ==========
 * BeginPaint 后调用文本/控件绘图回调 FUN_1400FE610，再 EndPaint。
 */
void FUN_1400FEC58(int64_t obj)
{
    HWND hwnd = *(HWND *)(obj + OBJ_HWND);
    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(hwnd, &ps);
    FUN_1400FE610(obj, hdc);
    EndPaint(hwnd, &ps);
}
