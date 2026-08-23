/* ====================================================================
 * core_b8c.c — B8 小函数批 3 (1400e0000-1400fffff)
 *
 * 本批新实现函数全部使用人类可读 PECMD_ 名称，原始地址保留在 @0x 注释。
 *
 * 来源: PECMD原始.EXE (x64, ImageBase=0x140000000)
 *   查找顶层父窗口    FUN_1400E5788 @0x1400e5788
 *   按进程 ID 找窗口  FUN_1400E6458 @0x1400e6458
 *   存储格式化 I64    PECMD_NewFormattedI64Str @0x1400e96ec
 *   分发窗口消息 F    FUN_1400EF620 @0x1400ef620
 *   取组合框选择      FUN_1400F3640 @0x1400f3640
 *   设置控件布局      FUN_1400F400C @0x1400f400c
 *   设置滑块位置      FUN_1400FC114 @0x1400fc114
 *   销毁 GDI 对象 C   FUN_1400FC33C @0x1400fc33c
 *   销毁对象(带释放) D FUN_1400FE5E0 @0x1400fe5e0
 *   销毁对象(带释放) E FUN_1400FF5A0 @0x1400ff5a0
 *
 * 约定:
 *   - 新实现函数使用 PECMD_ 可读名；未实现依赖仍 extern FUN_ + TODO(verify)
 *   - 全局数据符号先用 extern 声明; 完整链接时统一在 core_globals.c 定义
 * ==================================================================== */
#include <stdlib.h>
#include <stdint.h>

#include "pecmd_defs.h"
/* ---- 已实现公共工具 (其他 core_*.c) ---- */
extern void PECMD_FormatI64Dec(LPWSTR dst, int64_t value);          /* @0x1400e6d68 */

/* ---- 全局数据符号 ---- */
extern uint8_t PTR_FUN_14012cb90[];

/* ---- 未实现依赖 (extern + TODO(verify)) ---- */
extern BOOL FUN_1400E63C8(HWND hWnd, LONG_PTR lParam);
extern int64_t *PECMD_AllocMagicString(WCHAR *s);
extern void FUN_1400F0FA8(uint64_t *obj, uint64_t wParam,
                          uint64_t lParam);
extern void FUN_1400E8940(uint64_t *obj);
extern void FUN_1400FD1A8(uint64_t *obj);
extern void FUN_1400FECFC(uint64_t *obj);

/* ========== FUN_1400E5788 @0x1400e5788 ==========
 * 逐级向上找父窗口，直到遇到非 WS_CHILD 的顶层父窗口。
 */
HWND FUN_1400E5788(HWND hwnd)
{
    do {
        hwnd = GetParent(hwnd);
        if (hwnd == (HWND)0) {
            return (HWND)0;
        }
    } while (((uint32_t)GetWindowLongW(hwnd, -0x10) >> 0x1e & 1) != 0);
    return hwnd;
}

/* ========== FUN_1400E6458 @0x1400e6458 ==========
 * 枚举窗口，返回属于当前进程的窗口句柄（写到 ctx+0x18）。
 */
uint64_t FUN_1400E6458(int64_t ctx)
{
    *(uint64_t *)(ctx + 0x18) = 0;
    *(DWORD *)(ctx + 0x10) = GetCurrentProcessId();
    EnumWindows((void *)FUN_1400E63C8, (LONG_PTR)ctx);
    return *(uint64_t *)(ctx + 0x18);
}

/* ========== PECMD_NewFormattedI64Str @0x1400e96ec ==========
 * 把整数格式化为宽字符串，再保存字符串指针到 out。
 */
uint64_t *PECMD_NewFormattedI64Str(uint64_t *out, uint64_t unused, uint64_t value)
{
    WCHAR buf[104];

    (void)unused;
    PECMD_FormatI64Dec(buf, value);
    *out = 0;
    *out = (uint64_t)(uintptr_t)PECMD_AllocMagicString(buf);
    return out;
}

/* ========== FUN_1400EF620 @0x1400ef620 ==========
 * 根据对象标志位选择直接处理或走 vtable 消息槽。
 */
void FUN_1400EF620(uint64_t *obj, uint64_t wParam, uint64_t lParam)
{
    uint8_t flags = *(uint8_t *)((uint8_t *)obj + OBJ_FLAGS);

    if (((flags & 1) == 0) && ((flags & 6) == 0)) {
        FUN_1400F0FA8(obj, wParam, lParam);
    } else {
        (**(void (**)(uint64_t, uint32_t, uint64_t, uint64_t))(*obj + 8))
            (obj[4], 0xf, wParam, lParam);
    }
}

/* ========== FUN_1400F3640 @0x1400f3640 ==========
 * 取组合框当前选择；若启用了特殊标志则直接读缓存字段。
 */
uint64_t FUN_1400F3640(int64_t obj, int index)
{
    uint64_t result;

    if ((*(uint8_t *)(obj + 0x3f8) & 0x10) == 0) {
        result = (uint64_t)SendMessageW(*(HWND *)(obj + OBJ_HWND), 0x100c,
                                        (WPARAM)(int64_t)index, 2);
    } else {
        result = (uint64_t)*(uint32_t *)(obj + 0x3d4);
    }
    return result;
}

/* ========== FUN_1400F400C @0x1400f400c ==========
 * 更新控件布局字段；负数表示不修改对应项。
 */
void FUN_1400F400C(int64_t obj, int left, int top, int right,
                            uint32_t flags)
{
    if (-1 < left) {
        *(int *)(obj + 0x1f0) = left;
    }
    if (-1 < top) {
        *(int *)(obj + 500) = top;
    }
    if (-1 < right) {
        *(int *)(obj + 0x1f8) = right;
    }
    *(uint32_t *)(obj + 0x1fc) = flags;
}

/* ========== FUN_1400FC114 @0x1400fc114 ==========
 * 设置滑块/进度位置；根据标志选择发消息或直接写缓存。
 */
void FUN_1400FC114(int64_t obj, int pos, uint64_t flags)
{
    if (pos < 0) {
        pos = -0x1000000;
    }
    if ((flags >> 0xf & 1) == 0) {
        SendMessageW(*(HWND *)(obj + OBJ_HWND), 0x2001, 0, (LPARAM)(int64_t)pos);
    } else {
        *(int *)(obj + 0x54) = pos;
    }
}

/* ========== FUN_1400FC33C @0x1400fc33c ==========
 * 设置对象虚表并删除内部 GDI 对象后调用基类析构。
 */
void FUN_1400FC33C(uint64_t *obj)
{
    *obj = (uint64_t)(uintptr_t)PTR_FUN_14012cb90;
    if ((HGDIOBJ)obj[0x1a] != (HGDIOBJ)0) {
        DeleteObject((HGDIOBJ)obj[0x1a]);
    }
    obj[0x1a] = 0;
    FUN_1400E8940(obj);
}

/* ========== FUN_1400FE5E0 @0x1400fe5e0 ==========
 * 销毁对象 D；若 flag 低位置 1 则同时释放对象内存。
 */
uint64_t *FUN_1400FE5E0(uint64_t *obj, uint32_t flags)
{
    FUN_1400FD1A8(obj);
    if ((flags & 1) != 0) {
        free(obj);
    }
    return obj;
}

/* ========== FUN_1400FF5A0 @0x1400ff5a0 ==========
 * 销毁对象 E；若 flag 低位置 1 则同时释放对象内存。
 */
uint64_t *FUN_1400FF5A0(uint64_t *obj, uint32_t flags)
{
    FUN_1400FECFC(obj);
    if ((flags & 1) != 0) {
        free(obj);
    }
    return obj;
}
