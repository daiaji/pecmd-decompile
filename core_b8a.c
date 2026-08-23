/* ====================================================================
 * core_b8a.c — B8 尾部/GDI/窗口小函数批 (1400e0000-1400fffff)
 *
 * 本批新实现函数全部使用人类可读 PECMD_ 名称，原始地址保留在 @0x 注释。
 *
 * 来源: PECMD原始.EXE (x64, ImageBase=0x140000000)
 *   映射文件视图      FUN_1400E3F80 @0x1400e3f80
 *   向上对齐          FUN_1400E4064 @0x1400e4064
 *   释放对象槽        FUN_1400E56E4 @0x1400e56e4
 *   隐藏引用窗口      FUN_1400E5708 @0x1400e5708
 *   通知控件(启用时)  FUN_1400E58D8 @0x1400e58d8
 *   验证绘制区域      FUN_1400E5AE4 @0x1400e5ae4
 *   转发取文本长度    FUN_1400E62E4 @0x1400e62e4
 *   取文件大小(32位)  FUN_1400E693C @0x1400e693c
 *   格式化有符号64位  PECMD_FormatI64Dec @0x1400e6d68
 *   销毁事件对象      FUN_1400E759C @0x1400e759c
 *   销毁窗口对象      FUN_1400EBFB4 @0x1400ebfb4
 *   交换子对象        FUN_1400EC698 @0x1400ec698
 *   设置关联对象值    FUN_1400EF078 @0x1400ef078
 *   最大公约数        FUN_1400EF8C8 @0x1400ef8c8
 *   销毁 DC 包装对象  FUN_1400EF8EC @0x1400ef8ec
 *   销毁图标对象      FUN_1400F00C4 @0x1400f00c4
 *   销毁 GDI 对象     FUN_1400F06FC @0x1400f06fc
 *   分发控件消息      FUN_1400EC500 @0x1400f0a24
 *   销毁窗口对象 A    FUN_1400F1F18 @0x1400f1f18
 *   销毁窗口对象 B    FUN_1400F2924 @0x1400f2924
 *   分发 GDI 消息     FUN_1400F2B1C @0x1400f2b1c
 *   设置子窗口        FUN_1400F2B44 @0x1400f2b44
 *   删除 GDI 对象     FUN_1400F2B6C @0x1400f2b6c
 *   发送控件私有消息  FUN_1400F353C @0x1400f353c
 *   设置缩放双精度值  FUN_1400F4040 @0x1400f4040
 *   设置标志字段      FUN_1400F40BC @0x1400f40bc
 *   插入控件项        FUN_1400F5558 @0x1400f5558
 *   销毁窗口对象 C    FUN_1400FBA3C @0x1400fba3c
 *   销毁带串窗口对象  FUN_1400FC034 @0x1400fc034
 *   设置颜色并失效    FUN_1400FE5BC @0x1400fe5bc
 *
 * 约定:
 *   - 新实现函数使用 PECMD_ 可读名；未实现依赖仍 extern FUN_ + TODO(verify)
 *   - 全局数据符号先用 extern 声明; 完整链接时统一在 core_globals.c 定义
 * ==================================================================== */
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

#include "pecmd_defs.h"

/* ---- win32_stub.h 暂缺的 API ---- */
extern DWORD GetFileSize(HANDLE, DWORD *);

/* ---- 未实现依赖 (extern + TODO(verify)) ---- */
extern void *PECMD_MapSharedSection(HANDLE, int64_t, uint32_t, int64_t, LPCWSTR); /* @0x1400e3e38 */
extern void FUN_1400E4E94(uint64_t *obj);
extern void FUN_1400E9138(uint64_t *obj);
extern void PECMD_DtorMemDcCanvas(uint64_t *obj);
extern void PECMD_DtorImageHolder(uint64_t *obj);
extern void FUN_1400E8940(void *obj);
extern int64_t PECMD_ContainerAppend(uint64_t *obj);
extern uint64_t FUN_1400F0814(HDC, uint32_t, HDC, LPCWSTR, int32_t);
extern uint64_t FUN_1400F2A7C(uint64_t *obj, HDC hdc,
                              uint64_t *args);
extern int64_t FUN_1400E5B0C(HDC, uint64_t, HDC, uint64_t *);
extern void PECMD_ListAddItemEntry(int64_t, int32_t, LPCWSTR, uint32_t, int32_t,
                                        int32_t, int64_t); /* @0x1400f53c8 */
extern void PECMD_SetCtlBgBrush(int64_t, COLORREF, uint64_t);
extern void PECMD_InvalidateParentRect(HWND child, int margin);

/* ---- 本批引用的虚表/数据符号 ---- */
extern uint8_t PTR_FUN_14012bfb0[];
extern uint8_t PTR_FUN_14012c2e0[];
extern uint8_t PTR_FUN_14012c410[];
extern uint8_t PTR_FUN_14012c7b0[];
extern uint8_t PTR_FUN_14012ca50[];

/* ========== FUN_1400E3F80 @0x1400e3f80 ==========
 * 把文件映射为视图并返回映射基址。原 Ghidra 误标为 void，调用方实际使用返回值。
 */
void *FUN_1400E3F80(HANDLE hFile, int64_t size, uint32_t prot, int64_t offset)
{
    return PECMD_MapSharedSection(hFile, size, prot, offset, NULL);
}

/* ========== FUN_1400E4064 @0x1400e4064 ==========
 * 将 value 向上对齐到 align 的整数倍。
 */
int64_t FUN_1400E4064(int64_t value, uint32_t align)
{
    uint64_t a = align;
    uint64_t tmp = (a - 1) + (uint64_t)value;
    return (int64_t)((tmp / a) * a);
}

/* ========== FUN_1400E56E4 @0x1400e56e4 ==========
 * 释放 *slot 指向的对象（调用虚表 +0x18 释放函数）并清空槽。
 */
void FUN_1400E56E4(uint64_t *slot)
{
    uint64_t *obj = (uint64_t *)*slot;
    *slot = 0;
    if (obj != NULL) {
        void (*fn)(void *, int) = *(void (**)(void *, int))(*obj + 0x18);
        fn(obj, 1);
    }
}

/* ========== FUN_1400E5708 @0x1400e5708 ==========
 * 引用计数递减；递减后若窗口隐藏条件满足则隐藏窗口。
 * TODO(verify): +0x123 字段语义按引用计数处理。
 */
void FUN_1400E5708(int64_t obj)
{
    uint8_t *ref = (uint8_t *)(obj + 0x123);
    if (*ref > 0) {
        *ref = (uint8_t)(*ref - 1);
        ShowWindow(*(HWND *)(obj + OBJ_HWND), SW_HIDE);
    }
}

/* ========== FUN_1400E58D8 @0x1400e58d8 ==========
 * 若对象 +0x78 的 bit0 未置位，则调用子控件的虚表 +0x8 通知函数。
 */
void FUN_1400E58D8(uint64_t *obj, uint64_t param2)
{
    if ((*(uint8_t *)((char *)obj + 0x78) & 1) == 0) {
        void (*fn)(uint64_t, int, uint64_t) =
            *(void (**)(uint64_t, int, uint64_t))(*obj + 8);
        fn(obj[4], 0x14, param2);
    }
}

/* ========== FUN_1400E5AE4 @0x1400e5ae4 ==========
 * BeginPaint + EndPaint，用于声明/结束一次空绘制（验证绘制区域）。
 */
void FUN_1400E5AE4(HWND hwnd)
{
    PAINTSTRUCT ps;
    BeginPaint(hwnd, &ps);
    EndPaint(hwnd, &ps);
}

/* ========== FUN_1400E62E4 @0x1400e62e4 ==========
 * 对象未进入自定义模式时，把 WM_GETTEXTLENGTH(0x30) 转发给子窗口。
 */
LRESULT FUN_1400E62E4(int64_t obj, WPARAM wParam, LPARAM lParam)
{
    if (*(int64_t *)(obj + OBJ_CUSTOMMODE) == 0) {
        *(WPARAM *)(obj + OBJ_WPARAM) = wParam;
        return SendMessageW(*(HWND *)(obj + OBJ_HWND), 0x30, wParam, lParam);
    }
    return 0;
}

/* ========== FUN_1400E693C @0x1400e693c ==========
 * 取文件大小低 32 位；高位指针按原反编译置 0xffffffff（调用方只使用低 32 位）。
 */
DWORD FUN_1400E693C(HANDLE hFile)
{
    DWORD high = 0xffffffff;
    return GetFileSize(hFile, &high);
}

/* ========== PECMD_FormatI64Dec @0x1400e6d68 ==========
 * 将有符号 64 位整数按 "%I64d" 格式化到 dst。
 * 原 FUN_1400E6D38 的 wsprintfW 丢第 3 参，此处按调用语义补回数值。
 */
void PECMD_FormatI64Dec(LPWSTR dst, int64_t value)
{
    wsprintfW(dst, WSTR("%I64d"), value);
}

/* ========== FUN_1400E759C @0x1400e759c ==========
 * 清理事件/全局句柄对象；flags&1 时同时 free 容器。
 */
uint64_t *FUN_1400E759C(uint64_t *obj, uint32_t flags)
{
    FUN_1400E4E94(obj);
    if ((flags & 1) != 0) {
        free(obj);
    }
    return obj;
}

/* ========== FUN_1400EBFB4 @0x1400ebfb4 ==========
 * 清理窗口对象；flags&1 时同时 free 容器。
 */
uint64_t *FUN_1400EBFB4(uint64_t *obj, uint32_t flags)
{
    FUN_1400E9138(obj);
    if ((flags & 1) != 0) {
        free(obj);
    }
    return obj;
}

/* ========== FUN_1400EC698 @0x1400ec698 ==========
 * 设置 +0xf0 子对象指针，返回旧值。
 */
uint64_t FUN_1400EC698(int64_t obj, uint64_t child)
{
    uint64_t old = *(uint64_t *)(obj + OBJ_SUBWND);
    *(uint64_t *)(obj + OBJ_SUBWND) = child;
    return old;
}

/* ========== FUN_1400EF078 @0x1400ef078 ==========
 * 若 +0x110 关联对象非空，则向其 +0xd0 写入 value。
 */
void FUN_1400EF078(int64_t obj, uint64_t value)
{
    int64_t link = *(int64_t *)(obj + 0x110);
    if (link != 0) {
        *(uint64_t *)(link + OBJ_LINK) = value;
    }
}

/* ========== FUN_1400EF8C8 @0x1400ef8c8 ==========
 * 欧几里得最大公约数。原 Ghidra 误标为 void，调用方实际使用返回值。
 */
int64_t FUN_1400EF8C8(uint64_t a, uint64_t b)
{
    uint64_t r = a % b;
    while (r != 0) {
        uint64_t next = b % r;
        b = r;
        r = next;
    }
    return (int64_t)b;
}

/* ========== FUN_1400EF8EC @0x1400ef8ec ==========
 * 清理 DC 包装对象；flags&1 时同时 free 容器。
 */
uint64_t *FUN_1400EF8EC(uint64_t *obj, uint32_t flags)
{
    PECMD_DtorMemDcCanvas(obj);
    if ((flags & 1) != 0) {
        free(obj);
    }
    return obj;
}

/* ========== FUN_1400F00C4 @0x1400f00c4 ==========
 * 清理图标/图像对象；flags&1 时同时 free 容器。
 */
uint64_t *FUN_1400F00C4(uint64_t *obj, uint32_t flags)
{
    PECMD_DtorImageHolder(obj);
    if ((flags & 1) != 0) {
        free(obj);
    }
    return obj;
}

/* ========== FUN_1400F06FC @0x1400f06fc ==========
 * 设置 GDI 派生对象虚表，删除 +0x48 处的 GDI 对象后调用基类析构。
 */
void FUN_1400F06FC(uint64_t *obj)
{
    *obj = (uint64_t)(uintptr_t)PTR_FUN_14012bfb0;
    if ((HGDIOBJ)obj[9] != (HGDIOBJ)0) {
        DeleteObject((HGDIOBJ)obj[9]);
    }
    FUN_1400E8940(obj);
}

/* ========== FUN_1400EC500 @0x1400f0a24 ==========
 * 带 0x10 附加标志的控件消息分发包装。
 */
void FUN_1400EC500(HDC obj, uint32_t msg, HDC param3, LPCWSTR param4)
{
    FUN_1400F0814(obj, msg, param3, param4, 0x10);
}

/* ========== FUN_1400F1F18 @0x1400f1f18 ==========
 * 设置派生窗口对象虚表后调用基类析构。
 */
void FUN_1400F1F18(uint64_t *obj)
{
    *obj = (uint64_t)(uintptr_t)PTR_FUN_14012c2e0;
    FUN_1400E8940(obj);
}

/* ========== FUN_1400F2924 @0x1400f2924 ==========
 * 设置派生窗口对象虚表后调用基类析构。
 */
void FUN_1400F2924(uint64_t *obj)
{
    *obj = (uint64_t)(uintptr_t)PTR_FUN_14012c410;
    FUN_1400E8940(obj);
}

/* ========== FUN_1400F2B1C @0x1400f2b1c ==========
 * GDI 消息分发：0x1205 走专用处理，其余走通用处理。
 */
void FUN_1400F2B1C(HDC obj, uint64_t msg, HDC param3, uint64_t *param4)
{
    if ((int32_t)msg == 0x1205) {
        FUN_1400F2A7C((uint64_t *)(uintptr_t)obj, param3, param4);
    } else {
        FUN_1400E5B0C(obj, msg, param3, param4);
    }
}

/* ========== FUN_1400F2B44 @0x1400f2b44 ==========
 * 设置 +0x20 子窗口并刷新对象，返回设置是否成功。
 */
bool FUN_1400F2B44(uint64_t *obj, uint64_t hwnd)
{
    obj[4] = hwnd;
    PECMD_ContainerAppend(obj);
    return obj[4] != 0;
}

/* ========== FUN_1400F2B6C @0x1400f2b6c ==========
 * 删除对象 +0x8 处保存的 GDI 对象（若非空）。
 */
void FUN_1400F2B6C(int64_t obj)
{
    HGDIOBJ h = *(HGDIOBJ *)(obj + 8);
    if (h != (HGDIOBJ)0) {
        DeleteObject(h);
    }
}

/* ========== FUN_1400F353C @0x1400f353c ==========
 * 向对象子窗口发送 0x1036 私有控件消息，wParam=0。
 */
void FUN_1400F353C(int64_t obj, uint32_t value)
{
    SendMessageW(*(HWND *)(obj + OBJ_HWND), 0x1036, 0, (LPARAM)value);
}

/* ========== FUN_1400F4040 @0x1400f4040 ==========
 * 将浮点值存入 +0x200 的 double 字段；负数先向零取整。
 */
void FUN_1400F4040(int64_t obj, float value)
{
    if (value < 0.0f) {
        value = (float)(int)value;
    }
    *(double *)(obj + OBJ_DOUBLE_200) = (double)value;
}

/* ========== FUN_1400F40BC @0x1400f40bc ==========
 * 写入 +0x3b4 标志字段并返回 1。
 */
uint64_t FUN_1400F40BC(int64_t obj, uint32_t value)
{
    *(uint32_t *)(obj + OBJ_FLAGS_3B4) = value;
    return 1;
}

/* ========== FUN_1400F5558 @0x1400f5558 ==========
 * 从 item 结构提取文本/参数，插入控件项。
 * TODO(verify): +0x10 文本、+0x4 类型、+0x8 数值的字段语义。
 */
void FUN_1400F5558(int64_t obj, int32_t index, int64_t item,
                             uint64_t arg4, int32_t param5)
{
    (void)arg4;
    PECMD_ListAddItemEntry(obj, index,
                  *(LPCWSTR *)(item + 0x10),
                  *(uint32_t *)(item + 4),
                  *(int32_t *)(item + 8),
                  -1,
                  param5);
}

/* ========== FUN_1400FBA3C @0x1400fba3c ==========
 * 设置派生窗口对象虚表，销毁子窗口后调用基类析构。
 */
void FUN_1400FBA3C(uint64_t *obj)
{
    *obj = (uint64_t)(uintptr_t)PTR_FUN_14012c7b0;
    DestroyWindow((HWND)obj[4]);
    FUN_1400E8940(obj);
}

/* ========== FUN_1400FC034 @0x1400fc034 ==========
 * 设置派生窗口对象虚表，释放 +0xd8 处字符串后调用基类析构。
 */
void FUN_1400FC034(uint64_t *obj)
{
    *obj = (uint64_t)(uintptr_t)PTR_FUN_14012ca50;
    PECMD_FreeStrBuf((WCHAR **)(obj + 0x1b));
    FUN_1400E8940(obj);
}

/* ========== FUN_1400FE5BC @0x1400fe5bc ==========
 * 设置控件颜色/画刷并使其父窗口失效重绘。
 */
void FUN_1400FE5BC(int64_t obj, COLORREF color, uint64_t mode)
{
    PECMD_SetCtlBgBrush(obj, color, mode);
    PECMD_InvalidateParentRect(*(HWND *)(obj + OBJ_HWND), 1);
}
