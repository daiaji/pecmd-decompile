/* ====================================================================
 * core_b8g.c — B8 小函数批 6 (1400e0000-1400fffff)
 *
 * 本批新实现函数全部使用人类可读 PECMD_ 名称，原始地址保留在 @0x 注释。
 *
 * 来源: PECMD原始.EXE (x64, ImageBase=0x140000000)
 *   枚举窗口查找            FUN_1400E3D60 @0x1400e3d60
 *   刷新桌面                FUN_1400E3F94 @0x1400e3f94
 *   树查找并格式化路径      FUN_1400E537C @0x1400e537c
 *   初始化基类对象          FUN_1400E57C0 @0x1400e57c0
 *   是否进程提升权限        FUN_1400E6FB8 @0x1400e6fb8
 *   初始化窗口对象 G        FUN_1400E9048 @0x1400e9048
 *   标签选择变更处理        FUN_1400EC310 @0x1400ec310
 *   设置标签选择            FUN_1400EC428 @0x1400ec428
 *   创建标签控件            FUN_1400EC5E4 @0x1400ec5e4
 *   更新控件显示顺序        FUN_1400EC7C0 @0x1400ec7c0
 *   创建静态控件 B          FUN_1400EFFF8 @0x1400efff8
 *   初始化窗口基类对象      FUN_1400F0648 @0x1400f0648
 *   创建按钮控件            FUN_1400F072C @0x1400f072c
 *   创建日期时间控件        FUN_1400F1378 @0x1400f1378
 *   创建编辑控件            FUN_1400F21C0 @0x1400f21c0
 *   创建 IP 地址控件        FUN_1400F2934 @0x1400f2934
 *   列表项矩形失效          FUN_1400F30C0 @0x1400f30c0
 *   创建列表视图控件        FUN_1400F345C @0x1400f345c
 *   绘制对象文本            FUN_1400F527C @0x1400f527c
 *   按 ID 设置表项          FUN_1400F5B54 @0x1400f5b54
 *   更新映射双值 A          FUN_1400F5E2C @0x1400f5e2c
 *   更新映射双值 B          FUN_1400F5F60 @0x1400f5f60
 *   更新映射双值 C          FUN_1400F8F00 @0x1400f8f00
 *   翻译控件按键            FUN_1400FBAB8 @0x1400fbab8
 *   创建自定义类控件        FUN_1400FBF00 @0x1400fbf00
 *   创建进度条控件          FUN_1400FC060 @0x1400fc060
 *   创建滚动条控件          FUN_1400FC378 @0x1400fc378
 *   绘制控件文本            FUN_1400FC7A0 @0x1400fc7a0
 *   解析 buddy 前缀         FUN_1400FCE6C @0x1400fce6c
 *   创建静态控件核心        FUN_1400FD220 @0x1400fd220
 *   父窗口环绕失效          FUN_1400FD538 @0x1400fd538
 *   应用控件属性            FUN_1400FE4A4 @0x1400fe4a4
 *   创建树视图控件          FUN_1400FF080 @0x1400ff080
 *   发送控件设置信息消息    FUN_1400FF154 @0x1400ff154
 *   发送控件查询消息        FUN_1400FF20C @0x1400ff20c
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
extern WCHAR **FUN_14005B154(WCHAR **pp); /* @0x14005b154 */

/* ---- 已实现公共工具 (其他 core_*.c) ---- */
extern int64_t FUN_1400F40C8(int64_t obj, int id, uint64_t *outValue); /* @0x1400f40c8 */
extern void FUN_1400F2B6C(int64_t obj);                            /* @0x1400f2b6c */
extern uint64_t FUN_1400F2B84(int64_t *array, int64_t index,
                                      int32_t value, uint32_t field1,
                                      int32_t field2);                     /* @0x1400f2b84 */
extern void FUN_1400F5724(int64_t obj, uint32_t key, uint64_t value); /* @0x1400f5724 */
extern void FUN_1400F578C(int64_t obj, uint32_t key, uint64_t value); /* @0x1400f578c */
extern void FUN_1400F586C(int64_t obj, uint32_t key, uint64_t value); /* @0x1400f586c */
extern void FUN_1400F58D4(int64_t obj, uint32_t key, uint64_t value); /* @0x1400f58d4 */
extern uint64_t FUN_1400FC458(int64_t obj, HWND hwnd);        /* @0x1400fc458 */
extern void FUN_14007D0AC(int64_t *a1, LPCWSTR a2,
                                 LPCWSTR a3);                         /* @0x14007d0ac */
extern bool PECMD_ParseUIntValue(WCHAR **pp, int *out);                          /* @0x140067d20 */
extern int32_t FUN_14005C7C4(const char *a, const WCHAR *w);             /* @0x14005c7c4 */

/* ---- win32_stub.h 暂缺的 API ---- */
extern DWORD GetVersion(void);

/* ---- 未实现依赖 (extern + TODO(verify)) ---- */
extern BOOL PECMD_EnumWindowCallback(HWND hwnd, LPARAM lParam);
extern uint32_t FUN_1400E6F18(void);
extern void FUN_140063620(void *ps);
extern void FUN_1400EC0F0(int64_t a1, char a2);
extern int64_t PECMD_ContainerAppend(uint64_t *a1);
extern LRESULT FUN_1400E5890(int64_t a1);
extern void FUN_1400639F0(int64_t *arr, int64_t *cap, int64_t *cnt, void *data,
                          int64_t esize, int32_t mode);
extern int64_t FUN_1400F5584(int64_t a1, int a2, int *a3);
extern int64_t FUN_1400F593C(int64_t a1, int a2, int *a3);
extern int64_t FUN_1400F568C(int64_t a1, int a2, int a3,
                             int *a4);
extern int64_t FUN_1400F5A44(int64_t a1, int a2, int a3,
                             int *a4);
extern int64_t FUN_1400F5608(int64_t a1, int a2, int *a3);
extern int64_t FUN_1400F59C0(int64_t a1, int a2, int *a3);
extern void FUN_1400F57F4(int64_t a1, int a2, int a3,
                          uint64_t a4);
extern void FUN_1400F5ADC(int64_t a1, int a2, int a3,
                          uint64_t a4);
extern uint16_t FUN_1400F172C(int64_t *a1, uint32_t a2,
                              uint64_t a3, uint64_t *a4,
                              int64_t a5, uint32_t a6,
                              int64_t *a7);
extern void FUN_1400F0FA8(uint64_t *a1, uint64_t a2,
                          uint64_t a3);
extern bool PECMD_ParseNumSkipWs(WCHAR **pp, int64_t *out);
extern uint64_t FUN_1400C493C(int64_t *a1, int64_t *a2,
                              LPCWSTR a3, uint16_t *a4,
                              uint64_t a5, uint64_t a6,
                              int64_t a7);
extern void FUN_1400f21a8(void);

/* ---- 本批引用的虚表/数据符号 ---- */
extern uint8_t PTR_FUN_14012b240[];
extern uint8_t PTR_FUN_14012b430[];
extern uint8_t PTR_FUN_14012bfb0[];

/* ========== FUN_1400E3D60 @0x1400e3d60 ==========
 * 构造窗口查找参数并枚举顶层窗口或指定父窗口的子窗口。
 * TODO(verify): 回调 PECMD_EnumWindowCallback 写入 local_50 的具体含义。
 */
uint64_t FUN_1400E3D60(LPCWSTR text, char matchCase, char matchWord,
                                  HWND parent, uint32_t flags1,
                                  uint32_t flags2, uint64_t value1,
                                  uint64_t value2)
{
    struct {
        LPCWSTR text;
        uint64_t result;
        HWND parent;
        int field_18;
        int field_1c;
        int field_20;
        uint32_t flags1;
        uint32_t flags2;
        uint64_t value1;
        uint32_t field_38;
        uint64_t value2;
    } info;
    uint64_t result = 0;

    if (text == (LPCWSTR)0) {
        return 0;
    }

    info.field_18 = (int)matchCase;
    info.field_20 = (int)matchWord;
    info.result = 0;
    info.flags1 = flags1;
    info.flags2 = flags2;
    info.value1 = value1;
    info.value2 = value2;
    info.text = text;
    info.parent = parent;

    if (parent == (HWND)0) {
        EnumWindows((void *)PECMD_EnumWindowCallback, (LPARAM)&info);
    } else {
        EnumChildWindows(parent, (void *)PECMD_EnumWindowCallback, (LPARAM)&info);
    }
    result = info.result;
    return result;
}

/* ========== FUN_1400E3F94 @0x1400e3f94 ==========
 * 通知 Shell 刷新并给桌面 ListView 发送 F5 键按下/抬起。
 */
void FUN_1400E3F94(void)
{
    HWND defView;
    HWND listView;

    SHChangeNotify(0x8000000, 0x1000, (LPCVOID)0, (LPCVOID)0);
    defView = FindWindowW(WSTR("Progman"), WSTR("Program Manager"));
    if (defView == (HWND)0) {
        defView = FindWindowW(WSTR("Progman"), (LPCWSTR)0);
    }
    defView = FindWindowExW(defView, (HWND)0, WSTR("SHELLDLL_DefView"), (LPCWSTR)0);
    listView = FindWindowExW(defView, (HWND)0, WSTR("SysListView32"), WSTR("FolderView"));
    if (listView == (HWND)0) {
        listView = FindWindowExW(defView, (HWND)0, WSTR("SysListView32"), (LPCWSTR)0);
    }
    PostMessageW(listView, 0x100, 0x74, 0);
    PostMessageW(listView, 0x101, 0x74, 0);
}

/* ========== FUN_1400E537C @0x1400e537c ==========
 * 在树形结构中递归查找 key；命中时把 key 追加到 outFmt 的格式化串，
 * 并把节点文本写入 matchWord。
 * TODO(verify): wsprintfW 缺少的 %d 参数按匹配 key 还原。
 */
uint64_t FUN_1400E537C(int64_t root, uint64_t searchKey,
                                     int64_t *outText, int64_t *outFmt)
{
    uint8_t *node;
    uint8_t *base;
    int count;
    int64_t i;
    uint64_t key = searchKey;

    count = *(int *)((uint8_t *)(uintptr_t)root + 4);
    base = (uint8_t *)(uintptr_t)*(int64_t *)((uint8_t *)(uintptr_t)root + 8);
    for (i = 0; i < count; i++) {
        node = *(uint8_t **)(base + i * 8);
        if (*node == (uint8_t)0x80) {
            uint64_t r = FUN_1400E537C(
                *(int64_t *)(node + 0x10), key, outText, outFmt);
            if ((int)r != 0) {
                return r;
            }
            key = searchKey & 0xffffffffU;
        } else if ((uint32_t)*(uint16_t *)(node + 2) == (uint32_t)key) {
            if (outFmt != (int64_t *)0) {
                WCHAR **ps = (WCHAR **)outFmt;
                int len = lstrlenW((LPCWSTR)*ps);
                LPCWSTR fmt = WSTR("%d");

                PECMD_AllocString(ps, (int64_t)(len + 0x3c));
                if (0 < len) {
                    fmt = WSTR(" %d");
                }
                wsprintfW((LPWSTR)(*ps + len), fmt, (uint32_t)key);
            }
            FUN_1400703E4((WCHAR **)outText, (LPCWSTR)(node + 0x10));
            return 1;
        }
    }
    return 0;
}

/* ========== FUN_1400E57C0 @0x1400e57c0 ==========
 * 控件/窗口对象公共基类构造：填充虚表与大量默认字段。
 */
uint64_t *FUN_1400E57C0(uint64_t *obj)
{
    uint8_t *p = (uint8_t *)obj;

    obj[0] = (uint64_t)(uintptr_t)PTR_FUN_14012b240;
    obj[4] = 0;
    obj[7] = 0;
    obj[0x18] = 0;
    *(uint8_t *)(p + 0x78) = *(uint8_t *)(p + 0x78) & 0xfc;
    *(uint8_t *)(p + 0xa0) = 0xff;
    *(uint8_t *)(p + 0xb8) = 0xff;
    *(uint16_t *)(p + 0xa2) = 0xffff;
    obj[0x19] = 0;
    *(uint32_t *)(p + OBJ_COLOR) = 0x80000000;
    obj[6] = 0;
    obj[5] = 0;
    *(uint8_t *)(p + 0x61) = 0;
    obj[0x11] = 0;
    obj[0x10] = 0;
    *(uint32_t *)(p + 0xa8) = 0x80000000;
    *(uint8_t *)(p + 0xac) = 0;
    *(uint8_t *)(p + 0xa4) = 0;
    *(uint32_t *)(p + 0x9c) = 0x80000000;
    *(uint32_t *)(p + 0x98) = 0x80000000;
    *(uint32_t *)(p + 0x94) = 0x80000000;
    *(uint32_t *)(p + 0x90) = 0x80000000;
    *(uint8_t *)(p + 0xa5) = 0;
    *(uint8_t *)(p + 0xad) = 0;
    *(uint8_t *)(p + 0xa1) = 0;
    *(uint32_t *)(p + 0x44) = 0x80000000;
    obj[9] = 0;
    *(uint32_t *)(p + 0x54) = 0x80000000;
    *(uint32_t *)(p + 0x58) = 0xdefbac;
    *(uint32_t *)(p + 0x5c) = 0xff0000;
    *(uint32_t *)(p + 0x50) = 0xbf800000;
    *(uint8_t *)(p + 0x60) = 0x4c;
    obj[0xe] = 0;
    obj[0xd] = 0;
    return obj;
}

/* ========== FUN_1400E6FB8 @0x1400e6fb8 ==========
 * 先做前置检查，再在 Vista 以上系统通过 TokenElevation 查询提升状态。
 */
uint32_t FUN_1400E6FB8(void)
{
    uint32_t u;
    DWORD ver;
    HANDLE token;

    u = FUN_1400E6F18();
    if (u == 0) {
        return 0;
    }

    ver = GetVersion();
    if (0x5ff < (uint16_t)(((uint16_t)ver >> 8) | ((uint16_t)ver << 8))) {
        token = (HANDLE)0;
        if (OpenProcessToken(GetCurrentProcess(), 8, &token) != 0) {
            DWORD needed = 0;
            uint32_t elev[2] = { 0, 0 };

            u = 0;
            if ((GetTokenInformation(token, 20 /* TokenElevation */, elev, 4,
                                     &needed) != 0) && (needed == 4)) {
                u = elev[0];
            }
            CloseHandle(token);
        } else {
            u = 0;
        }
    }
    return u;
}

/* ========== FUN_1400E9048 @0x1400e9048 ==========
 * 初始化窗口对象 G：调用基类构造后设置虚表、窗口句柄/参数与内部串容器。
 */
uint64_t *FUN_1400E9048(uint64_t *obj, uint32_t hwnd, uint64_t data)
{
    uint8_t *p = (uint8_t *)obj;

    FUN_1400E57C0(obj);
    *obj = (uint64_t)(uintptr_t)PTR_FUN_14012b430;
    obj[0x1b] = 0;
    *(uint32_t *)(p + 0xec) = 0;
    *(uint32_t *)(p + 0xf8) = 0;
    *(uint32_t *)(p + 0xfc) = 0;
    obj[0x20] = hwnd;
    obj[0x21] = 0;
    obj[0x22] = 0;
    obj[0x23] = data;
    *(uint8_t *)(p + 0x120) = 0;
    *(uint8_t *)(p + 0x121) = 0;
    *(uint8_t *)(p + 0x122) = 0;
    *(uint8_t *)(p + 0x123) = 0;
    FUN_140063620((WCHAR **)(obj + 0x25));
    obj[0x2a] = 0;
    *(uint8_t *)(p + 0x158) = 0;
    *(uint32_t *)(p + 0x148) = 0x80000000;
    *(uint32_t *)(p + 0x144) = 0x80000000;
    *(uint32_t *)(p + 0x140) = 0x80000000;
    *(uint32_t *)(p + 0x13c) = 0x80000000;
    obj[0x26] = 0;
    *(uint32_t *)(p + 0x138) = 0;
    obj[0x1c] = 0;
    *(uint32_t *)(p + OBJ_FLAGS) = 0;
    *(uint8_t *)(p + 0x159) = 0;
    *(uint8_t *)(p + 0x15a) = 0;
    *(uint8_t *)(p + 0xd4) = 0;
    obj[0x1e] = 0;
    return obj;
}

/* ========== FUN_1400EC310 @0x1400ec310 ==========
 * 标签控件当前项变化时隐藏旧项、显示新项，并设置 &&<name>.Select 变量。
 */
void FUN_1400EC310(int64_t obj)
{
    uint8_t *self = (uint8_t *)(uintptr_t)obj;
    int old_sel;
    int new_sel;
    int64_t **slot;
    HWND hwnd;
    WCHAR *var;

    old_sel = *(int *)(self + 0x120);
    new_sel = (int)SendMessageW(*(HWND *)(self + OBJ_HWND), 0x132f, 0, 0);
    if ((new_sel == old_sel) || (new_sel < 0) ||
        ((int64_t)new_sel >= *(int64_t *)(self + 0x118))) {
        return;
    }

    slot = (int64_t **)(*(int64_t **)(self + 0x108) + (int64_t)old_sel * 8);
    if (*(int64_t *)*slot != 0) {
        ShowWindow(*(HWND *)(*(int64_t *)*slot + OBJ_HWND), 0);
    }
    *(int *)(self + 0x120) = new_sel;

    slot = (int64_t **)(*(int64_t **)(self + 0x108) + (int64_t)new_sel * 8);
    if (*(int64_t *)*slot != 0) {
        ShowWindow(*(HWND *)(*(int64_t *)*slot + OBJ_HWND), 5);
    }

    hwnd = *(HWND *)(self + OBJ_HWND);
    if (GetFocus() != hwnd) {
        SetFocus(hwnd);
    }

    var = NULL;
    FUN_1400702B0(&var, WSTR("&&"));
    FUN_14006375C(&var, *(LPCWSTR *)(self + 0xd8));
    FUN_14006375C(&var, WSTR(".Select"));
    FUN_1400669C4(*(void **)(self + 0xe0), (int64_t)(new_sel + 1), var);
    FUN_14005B104(&var);
}

/* ========== FUN_1400EC428 @0x1400ec428 ==========
 * 设置标签控件当前页：隐藏旧页、显示新页，返回旧选择；越界返回 -1。
 */
int FUN_1400EC428(int64_t obj, int index)
{
    uint8_t *self = (uint8_t *)(uintptr_t)obj;
    int old_sel;
    HWND old_hwnd;
    HWND new_hwnd;

    if ((index < 0) || (*(int64_t *)(self + 0x118) <= (int64_t)index)) {
        return -1;
    }

    old_sel = *(int *)(self + 0x120);
    if (old_sel != index) {
        old_hwnd = *(HWND *)(**(int64_t **)(*(int64_t **)(self + 0x108) +
                                            (int64_t)old_sel * 8) + OBJ_HWND);
        new_hwnd = *(HWND *)(**(int64_t **)(*(int64_t **)(self + 0x108) +
                                            (int64_t)index * 8) + OBJ_HWND);
        *(int *)(self + 0x120) = index;
        SetWindowPos(old_hwnd, (HWND)0, 0, 0, 0, 0, 0x97);
        SetWindowPos(new_hwnd, (HWND)0, 0, 0, 0, 0, 0x43);
        SendMessageW(*(HWND *)(self + OBJ_HWND), 0x1330, (WPARAM)index, 0);
    }
    return old_sel;
}

/* ========== FUN_1400EC5E4 @0x1400ec5e4 ==========
 * 创建 SysTabControl32 子控件并挂到对象容器。
 */
bool FUN_1400EC5E4(int64_t *obj, DWORD style, int *rect,
                            HWND parent, uint32_t id)
{
    LONG inst = GetWindowLongW(parent, -6);
    HWND hwnd;

    hwnd = CreateWindowExW(0, WSTR("SysTabControl32"), (LPCWSTR)0, style,
                           rect[0], rect[1], rect[2] - rect[0],
                           rect[3] - rect[1], parent,
                           (HMENU)(uintptr_t)id,
                           (HINSTANCE)(intptr_t)inst, (LPVOID)0);
    obj[4] = (uint64_t)(uintptr_t)hwnd;
    PECMD_ContainerAppend((uint64_t *)obj);
    return obj[4] != 0;
}

/* ========== FUN_1400EC7C0 @0x1400ec7c0 ==========
 * 按当前选中项隐藏其余子窗口并显示第一项。
 */
void FUN_1400EC7C0(int64_t obj, char mode)
{
    uint8_t *self = (uint8_t *)(uintptr_t)obj;
    int64_t **items;
    int count;
    int i;

    if (*(int64_t *)(self + 0x118) <= 0) {
        return;
    }

    FUN_1400EC0F0(obj, mode);
    items = *(int64_t ***)(self + 0x108);
    count = (int)*(int64_t *)(self + 0x118);
    i = 1;
    while (i < count) {
        SetWindowPos(*(HWND *)(*(int64_t *)items[i] + OBJ_HWND), (HWND)0, 0, 0, 0, 0, 0x83);
        i++;
    }
    SetWindowPos(*(HWND *)(*(int64_t *)items[0] + OBJ_HWND), (HWND)0, 0, 0, 0, 0, 0x43);
}

/* ========== FUN_1400EFFF8 @0x1400efff8 ==========
 * 创建 STATIC 子控件并挂到对象容器。
 */
bool FUN_1400EFFF8(int64_t *obj, LPCWSTR text, DWORD style,
                                int *rect, HWND parent, uint32_t id)
{
    LONG inst = GetWindowLongW(parent, -6);
    HWND hwnd;

    hwnd = CreateWindowExW(0, WSTR("STATIC"), text, style,
                           rect[0], rect[1], rect[2] - rect[0],
                           rect[3] - rect[1], parent,
                           (HMENU)(uintptr_t)id,
                           (HINSTANCE)(intptr_t)inst, (LPVOID)0);
    obj[4] = (uint64_t)(uintptr_t)hwnd;
    PECMD_ContainerAppend((uint64_t *)obj);
    return obj[4] != 0;
}

/* ========== FUN_1400F0648 @0x1400f0648 ==========
 * 窗口对象基类构造（含虚表 PTR_FUN_14012bfb0）。
 */
uint64_t *FUN_1400F0648(uint64_t *obj, uint64_t data)
{
    uint8_t *p = (uint8_t *)obj;

    FUN_1400E57C0(obj);
    obj[0x1a] = data;
    *(uint16_t *)(p + 0xa2) = 0xffff;
    *obj = (uint64_t)(uintptr_t)PTR_FUN_14012bfb0;
    *(uint8_t *)(p + 0xd8) = 0;
    *(uint32_t *)(p + OBJ_COLOR) = 0x80000000;
    *(uint8_t *)(p + 0x61) = 2;
    *(uint8_t *)(p + 0xad) = 0;
    *(uint32_t *)(p + 0x44) = 0x80000000;
    obj[9] = 0;
    obj[0x1c] = 0;
    obj[0x11] = 0;
    obj[0x10] = 0;
    *(uint32_t *)(p + 0xa8) = 0x80000000;
    *(uint8_t *)(p + 0xa0) = 0;
    *(uint8_t *)(p + 0xac) = 0;
    *(uint8_t *)(p + 0xa4) = 0;
    *(uint32_t *)(p + 0x9c) = 0x80000000;
    *(uint32_t *)(p + 0x98) = 0x80000000;
    *(uint32_t *)(p + 0x94) = 0x80000000;
    *(uint32_t *)(p + 0x90) = 0x80000000;
    *(uint8_t *)(p + 0xa5) = 0;
    *(uint8_t *)(p + 0xa1) = 1;
    return obj;
}

/* ========== FUN_1400F072C @0x1400f072c ==========
 * 创建 BUTTON 子控件并挂到对象容器。
 */
bool FUN_1400F072C(int64_t *obj, LPCWSTR text, DWORD style,
                               int *rect, HWND parent, uint32_t id,
                               DWORD exStyle)
{
    int x0 = rect[0];
    int y0 = rect[1];
    int x1 = rect[2];
    int y1 = rect[3];
    LONG inst = GetWindowLongW(parent, -6);
    HWND hwnd;

    hwnd = CreateWindowExW(exStyle, WSTR("BUTTON"), text, style,
                           x0, y0, x1 - x0, y1 - y0, parent,
                           (HMENU)(uintptr_t)id,
                           (HINSTANCE)(intptr_t)inst, (LPVOID)0);
    obj[4] = (uint64_t)(uintptr_t)hwnd;
    PECMD_ContainerAppend((uint64_t *)obj);
    return obj[4] != 0;
}

/* ========== FUN_1400F1378 @0x1400f1378 ==========
 * 创建 SysDateTimePick32 子控件并挂到对象容器。
 */
bool FUN_1400F1378(int64_t *obj, uint32_t style,
                                 int *rect, HWND parent, uint32_t id)
{
    int x0 = rect[0];
    int y0 = rect[1];
    int x1 = rect[2];
    int y1 = rect[3];
    LONG inst = GetWindowLongW(parent, -6);
    HWND hwnd;

    hwnd = CreateWindowExW(0, WSTR("SysDateTimePick32"), (LPCWSTR)0,
                           style | 0x40000000U, x0, y0, x1 - x0, y1 - y0,
                           parent, (HMENU)(uintptr_t)id,
                           (HINSTANCE)(intptr_t)inst, (LPVOID)0);
    obj[4] = (uint64_t)(uintptr_t)hwnd;
    PECMD_ContainerAppend((uint64_t *)obj);
    return obj[4] != 0;
}

/* ========== FUN_1400F21C0 @0x1400f21c0 ==========
 * 创建 EDIT 子控件并挂到对象容器；指定标志时安装编辑回调。
 */
bool FUN_1400F21C0(int64_t *obj, uint32_t flags, DWORD style,
                             int *rect, HWND parent, uint32_t id)
{
    LONG inst = GetWindowLongW(parent, -6);
    HWND hwnd;

    hwnd = CreateWindowExW(flags & 0xffffefffU, WSTR("EDIT"), (LPCWSTR)0,
                           style, rect[0], rect[1], rect[2] - rect[0],
                           rect[3] - rect[1], parent,
                           (HMENU)(uintptr_t)id,
                           (HINSTANCE)(intptr_t)inst, (LPVOID)0);
    obj[4] = (uint64_t)(uintptr_t)hwnd;
    PECMD_ContainerAppend((uint64_t *)obj);
    if ((flags >> 0xc & 1U) != 0U) {
        SendMessageW((HWND)obj[4], 0xd0, 0,
                     (LPARAM)(uintptr_t)&FUN_1400f21a8);
    }
    return obj[4] != 0;
}

/* ========== FUN_1400F2934 @0x1400f2934 ==========
 * 创建 SysIPAddress32 子控件并挂到对象容器。
 */
bool FUN_1400F2934(int64_t *obj, DWORD style, int *rect,
                                  HWND parent, uint32_t id)
{
    int x0 = rect[0];
    int y0 = rect[1];
    int x1 = rect[2];
    int y1 = rect[3];
    LONG inst = GetWindowLongW(parent, -6);
    HWND hwnd;

    hwnd = CreateWindowExW(0, WSTR("SysIPAddress32"), (LPCWSTR)0, style,
                           x0, y0, x1 - x0, y1 - y0, parent,
                           (HMENU)(uintptr_t)id,
                           (HINSTANCE)(intptr_t)inst, (LPVOID)0);
    obj[4] = (uint64_t)(uintptr_t)hwnd;
    PECMD_ContainerAppend((uint64_t *)obj);
    return obj[4] != 0;
}

/* ========== FUN_1400F30C0 @0x1400f30c0 ==========
 * 使列表控件指定行/子项所在区域失效。TODO(verify): 消息 0x1038/0x100e 语义。
 */
void FUN_1400F30C0(int64_t obj, int row, int top)
{
    uint8_t *self = (uint8_t *)(uintptr_t)obj;
    RECT rc;
    int64_t data;
    short width = 0;
    WPARAM wParam = (WPARAM)row;
    LONG old_bottom;

    memset(&rc, 0, sizeof(rc));
    if (*(int64_t *)(self + 0x3a0) < 1) {
        data = 0;
    } else {
        data = *(int64_t *)(self + 0x398);
    }
    if (data != 0) {
        width = *(short *)(*(int64_t *)(self + 0x368) +
                           *(int64_t *)(data + wParam * 8) * 2);
    }

    if ((data != 0) && (0 < width)) {
        rc.left = 0;
        rc.top = top;
        SendMessageW(*(HWND *)(self + OBJ_HWND), 0x1038,
                     (WPARAM)(int64_t)(width - 1 + row), (LPARAM)&rc);
    }

    old_bottom = rc.bottom;
    if (top == 0) {
        rc.left = 2;
        SendMessageW(*(HWND *)(self + OBJ_HWND), 0x100e, wParam, (LPARAM)&rc);
        rc.left = 0;
    } else {
        rc.left = 0;
        rc.top = top;
        SendMessageW(*(HWND *)(self + OBJ_HWND), 0x1038, wParam, (LPARAM)&rc);
    }
    if (0 < width) {
        rc.bottom = old_bottom;
    }
    InvalidateRect(*(HWND *)(self + OBJ_HWND), &rc, 1);
}

/* ========== FUN_1400F345C @0x1400f345c ==========
 * 创建 SysListView32 子控件并挂到对象容器，保存自引用后立即更新。
 */
bool FUN_1400F345C(int64_t *obj, uint32_t style,
                                 int *rect, HWND parent, uint32_t id)
{
    int x0 = rect[0];
    int y0 = rect[1];
    int x1 = rect[2];
    int y1 = rect[3];
    LONG inst = GetWindowLongW(parent, -6);
    HWND hwnd;

    obj[0x42] = (uint64_t)(uintptr_t)obj;
    hwnd = CreateWindowExW(0, WSTR("SysListView32"), (LPCWSTR)0,
                           style | 0x400U, x0, y0, x1 - x0, y1 - y0,
                           parent, (HMENU)(uintptr_t)id,
                           (HINSTANCE)(intptr_t)inst, (LPVOID)0);
    obj[4] = (uint64_t)(uintptr_t)hwnd;
    PECMD_ContainerAppend((uint64_t *)obj);
    UpdateWindow((HWND)obj[4]);
    return obj[4] != 0;
}

/* ========== FUN_1400F527C @0x1400f527c ==========
 * 用对象字体/颜色在指定矩形内绘制文本。
 */
void FUN_1400F527C(int64_t obj, HDC hdc, uint64_t *info,
                          LPRECT rc)
{
    HGDIOBJ font;
    HGDIOBJ old_font = (HGDIOBJ)0;
    uint8_t align_byte;
    uint32_t format;

    font = (HGDIOBJ)FUN_1400E5890(obj);
    if (font != (HGDIOBJ)0) {
        old_font = SelectObject(hdc, font);
    }
    SetBkMode(hdc, 1);
    if (-1 < (int)*(COLORREF *)((uint8_t *)info + 0x28)) {
        SetTextColor(hdc, *(COLORREF *)((uint8_t *)info + 0x28));
    }

    align_byte = *(uint8_t *)((uint8_t *)info + 0x1c) & 0x7f;
    format = 1;
    if (align_byte != 0x43) {
        format = 0;
        if (align_byte == 0x52) {
            format = 2;
        }
    }
    DrawTextW(hdc, (LPCWSTR)*info, -1, rc, format | 0x8064U);
    if (old_font != (HGDIOBJ)0) {
        SelectObject(hdc, old_font);
    }
}

/* ========== FUN_1400F5B54 @0x1400f5b54 ==========
 * 按 id 查找数组项；存在则更新/删除，不存在且 value 非 0 时新增节点。
 */
void FUN_1400F5B54(int64_t obj, int id, int64_t value)
{
    uint8_t *self = (uint8_t *)(uintptr_t)obj;
    uint64_t ignored = 0;
    int64_t idx;
    int64_t *base;
    void *item;

    idx = FUN_1400F40C8(obj, id, &ignored);
    (void)ignored;
    if (idx < 0) {
        if (value != 0) {
            int *node = (int *)calloc(1, 0x10);

            node[0] = id;
            *(int64_t *)(node + 2) = value;
            FUN_1400639F0((int64_t *)(self + 0x308),
                          (int64_t *)(self + 0x310),
                          (int64_t *)(self + 0x318), &node, 8, 1);
        }
        return;
    }

    base = *(int64_t **)(self + 0x308);
    item = *(void **)(base + idx * 8);
    if (item != (void *)0) {
        if (value == 0) {
            *(int64_t *)(base + idx * 8) = 0;
            FUN_1400F2B6C((int64_t)item);
            free(item);
        } else {
            *(int64_t *)((uint8_t *)item + 8) = value;
            **(int **)(base + idx * 8) = id;
        }
    }
}

/* ========== FUN_1400F5E2C @0x1400f5e2c ==========
 * 更新两组单键映射值；未找到且值非负/更新成功时补加新项。
 */
void FUN_1400F5E2C(int64_t obj, int key, uint32_t val1,
                                uint32_t val2)
{
    uint8_t *self = (uint8_t *)(uintptr_t)obj;
    int dummy;
    int changed;

    if (-2 < (int)val2) {
        int64_t idx = FUN_1400F5584(obj, key, &dummy);
        if (idx < 0) {
            changed = -1 < (int)val2;
        } else {
            uint64_t r = FUN_1400F2B84((int64_t *)(self + 0x260),
                                               idx, (int32_t)val2,
                                               (uint32_t)key, -3);
            changed = (char)r != 0;
        }
        if (changed != 0) {
            FUN_1400F5724(obj, (uint32_t)key, val2);
        }
    }

    if (-2 < (int)val1) {
        int64_t idx = FUN_1400F593C(obj, key, &dummy);
        if (idx < 0) {
            changed = -1 < (int)val1;
        } else {
            uint64_t r = FUN_1400F2B84((int64_t *)(self + 0x2a8),
                                               idx, (int32_t)val1,
                                               (uint32_t)key, -3);
            changed = (char)r != 0;
        }
        if (changed != 0) {
            FUN_1400F586C(obj, (uint32_t)key, val1);
        }
    }
}

/* ========== FUN_1400F5F60 @0x1400f5f60 ==========
 * 更新两组双键映射值；未找到且值非负/更新成功时补加新项。
 */
void FUN_1400F5F60(int64_t obj, int key1, int key2,
                                uint32_t val1, uint32_t val2)
{
    uint8_t *self = (uint8_t *)(uintptr_t)obj;
    int dummy;
    int changed;

    if (-2 < (int)val2) {
        int64_t idx = FUN_1400F568C(obj, key1, key2, &dummy);
        if (idx < 0) {
            changed = -1 < (int)val2;
        } else {
            uint64_t r = FUN_1400F2B84((int64_t *)(self + 0x290),
                                               idx, (int32_t)val2,
                                               (uint32_t)key1, key2);
            changed = (char)r != 0;
        }
        if (changed != 0) {
            FUN_1400F57F4(obj, key1, key2, val2);
        }
    }

    if (-2 < (int)val1) {
        int64_t idx = FUN_1400F5A44(obj, key1, key2, &dummy);
        if (idx < 0) {
            changed = -1 < (int)val1;
        } else {
            uint64_t r = FUN_1400F2B84((int64_t *)(self + 0x2d8),
                                               idx, (int32_t)val1,
                                               (uint32_t)key1, key2);
            changed = (char)r != 0;
        }
        if (changed != 0) {
            FUN_1400F5ADC(obj, key1, key2, val1);
        }
    }
}

/* ========== FUN_1400F8F00 @0x1400f8f00 ==========
 * 更新另一组单键映射值；未找到且值非负/更新成功时补加新项。
 */
void FUN_1400F8F00(int64_t obj, int key, uint32_t val1,
                                uint32_t val2)
{
    uint8_t *self = (uint8_t *)(uintptr_t)obj;
    int dummy;
    int changed;

    if (-2 < (int)val2) {
        int64_t idx = FUN_1400F5608(obj, key, &dummy);
        if (idx < 0) {
            changed = -1 < (int)val2;
        } else {
            uint64_t r = FUN_1400F2B84((int64_t *)(self + 0x278),
                                               idx, (int32_t)val2,
                                               (uint32_t)key, -3);
            changed = (char)r != 0;
        }
        if (changed != 0) {
            FUN_1400F578C(obj, (uint32_t)key, val2);
        }
    }

    if (-2 < (int)val1) {
        int64_t idx = FUN_1400F59C0(obj, key, &dummy);
        if (idx < 0) {
            changed = -1 < (int)val1;
        } else {
            uint64_t r = FUN_1400F2B84((int64_t *)(self + 0x2c0),
                                               idx, (int32_t)val1,
                                               (uint32_t)key, -3);
            changed = (char)r != 0;
        }
        if (changed != 0) {
            FUN_1400F58D4(obj, (uint32_t)key, val1);
        }
    }
}

/* ========== FUN_1400FBAB8 @0x1400fbab8 ==========
 * 控件按键翻译：Enter 触发父命令，Esc 返回 1，其余走消息过滤通道。
 */
uint32_t FUN_1400FBAB8(int64_t obj, int64_t pmsg)
{
    uint8_t *self = (uint8_t *)(uintptr_t)obj;
    int msg = *(int *)(pmsg + 8);
    uint64_t vk = *(uint64_t *)(pmsg + 0x10);
    uint64_t result = 0;
    uint16_t flags;

    if ((msg == 0x100) && (vk == 0xd) && (*(uint8_t *)(self + OBJ_LINK) != 0)) {
        int id = GetDlgCtrlID(*(HWND *)(self + OBJ_HWND));
        HWND parent = GetParent(*(HWND *)(self + OBJ_HWND));
        PostMessageW(parent, 0x111, (WPARAM)(uint16_t)id, 0x100);
    }

    if ((vk == 0x1b) && ((msg == 0x100) || (msg == 0x101))) {
        return 1;
    }

    FUN_1400F172C(*(int64_t **)(self + 0xd8), (uint32_t)msg, vk,
                  *(uint64_t **)(pmsg + 0x18), *(int64_t *)(self + OBJ_HWND),
                  0x80, (int64_t *)0);
    flags = FUN_1400F172C(*(int64_t **)(self + 0xd8), 0x233, vk,
                          *(uint64_t **)(pmsg + 0x18),
                          *(int64_t *)(self + OBJ_HWND), 1, (int64_t *)&result);
    return ((flags & 4) != 0) ? (uint32_t)result : 0;
}

/* ========== FUN_1400FBF00 @0x1400fbf00 ==========
 * 创建指定类名的自定义子控件并挂到对象容器。
 */
bool FUN_1400FBF00(int64_t *obj, uint32_t style, int *rect,
                               HWND parent, uint32_t id, LPCWSTR class_name)
{
    int x0 = rect[0];
    int y0 = rect[1];
    int x1 = rect[2];
    int y1 = rect[3];
    LONG inst = GetWindowLongW(parent, -6);
    HWND hwnd;

    hwnd = CreateWindowExW(0, class_name, (LPCWSTR)0, style,
                           x0, y0, x1 - x0, y1 - y0, parent,
                           (HMENU)(uintptr_t)id,
                           (HINSTANCE)(intptr_t)inst, (LPVOID)0);
    obj[4] = (uint64_t)(uintptr_t)hwnd;
    PECMD_ContainerAppend((uint64_t *)obj);
    if ((style >> 0x1c & 1U) != 0U) {
        ShowWindow((HWND)obj[4], 5);
    }
    return obj[4] != 0;
}

/* ========== FUN_1400FC060 @0x1400fc060 ==========
 * 创建 msctls_progress32 进度条控件并挂到对象容器。
 */
bool FUN_1400FC060(int64_t *obj, DWORD style, int *rect,
                                    HWND parent, uint32_t id)
{
    LONG inst = GetWindowLongW(parent, -6);
    HWND hwnd;

    hwnd = CreateWindowExW(0, WSTR("msctls_progress32"), (LPCWSTR)0, style,
                           rect[0], rect[1], rect[2] - rect[0],
                           rect[3] - rect[1], parent,
                           (HMENU)(uintptr_t)id,
                           (HINSTANCE)(intptr_t)inst, (LPVOID)0);
    obj[4] = (uint64_t)(uintptr_t)hwnd;
    PECMD_ContainerAppend((uint64_t *)obj);
    return obj[4] != 0;
}

/* ========== FUN_1400FC378 @0x1400fc378 ==========
 * 创建 ScrollBar 子控件并挂到对象容器。
 */
bool FUN_1400FC378(int64_t *obj, uint32_t style, int *rect,
                                  HWND parent, uint32_t id)
{
    int x0 = rect[0];
    int y0 = rect[1];
    int x1 = rect[2];
    int y1 = rect[3];
    LONG inst = GetWindowLongW(parent, -6);
    HWND hwnd;

    hwnd = CreateWindowExW(0, WSTR("ScrollBar"), (LPCWSTR)0, style,
                           x0, y0, x1 - x0, y1 - y0, parent,
                           (HMENU)(uintptr_t)id,
                           (HINSTANCE)(intptr_t)inst, (LPVOID)0);
    obj[4] = (uint64_t)(uintptr_t)hwnd;
    PECMD_ContainerAppend((uint64_t *)obj);
    if ((style >> 0x1c & 1U) != 0U) {
        ShowWindow((HWND)obj[4], 5);
    }
    return obj[4] != 0;
}

/* ========== FUN_1400FC7A0 @0x1400fc7a0 ==========
 * 先做公共绘制前处理，再在客户区绘制控件内部文本。
 */
void FUN_1400FC7A0(int64_t *obj, uint64_t wParam, uint64_t lParam)
{
    HWND hwnd;
    HDC hdc;
    LPCWSTR text;
    HGDIOBJ font;
    HGDIOBJ old_font = (HGDIOBJ)0;
    RECT rc;

    FUN_1400F0FA8((uint64_t *)obj, wParam, lParam);
    if (*(short *)obj[0x1b] == 0) {
        return;
    }

    hwnd = (HWND)obj[4];
    hdc = GetDC(hwnd);
    memset(&rc, 0, sizeof(rc));
    GetClientRect((HWND)obj[4], &rc);
    SendMessageW((HWND)obj[4], 0x408, 0, 0);

    text = (LPCWSTR)obj[0x1b];
    font = (HGDIOBJ)FUN_1400E5890((int64_t)obj);
    if (font != (HGDIOBJ)0) {
        old_font = SelectObject(hdc, font);
    }
    SetBkMode(hdc, 1);
    if (-1 < (int)*(COLORREF *)((uint8_t *)obj + OBJ_TEXTCOLOR)) {
        SetTextColor(hdc, *(COLORREF *)((uint8_t *)obj + OBJ_TEXTCOLOR));
    }
    DrawTextW(hdc, text, -1, &rc, 0x25);
    if (old_font != (HGDIOBJ)0) {
        SelectObject(hdc, old_font);
    }
    if (hdc != (HDC)0) {
        ReleaseDC(hwnd, hdc);
    }
}

/* ========== FUN_1400FCE6C @0x1400fce6c ==========
 * 识别 "cmd<buddy>..." 前缀：解析 buddy 窗口并交换关联窗口后返回 0；
 * 否则转发到普通控件创建/解析流程。
 */
uint64_t FUN_1400FCE6C(int64_t *obj, int64_t *ctx,
                                LPCWSTR text, uint16_t *name,
                                uint64_t a5, uint64_t a6,
                                int64_t a7)
{
    WCHAR *p = (WCHAR *)text;
    int64_t buddy = 0;
    bool ok;

    if (((char)FUN_14005C7C4("cmd", (const WCHAR *)name) != 0) &&
        (FUN_14005C788("<buddy>", p, 7) != 0)) {
        p += 7;
        ok = PECMD_ParseNumSkipWs(&p, &buddy);
        if (ok && (buddy != 0)) {
            FUN_1400FC458((int64_t)obj, (HWND)(uintptr_t)buddy);
        }
        *name = 0;
        return 0;
    }
    return FUN_1400C493C(obj, ctx, p, name, a5, a6, a7);
}

/* ========== FUN_1400FD220 @0x1400fd220 ==========
 * STATIC 控件核心创建：保存样式低字节标志，创建时清除 SS_TYPEMASK 位。
 */
bool FUN_1400FD220(int64_t *obj, DWORD exStyle,
                                   LPCWSTR text, uint32_t style, int *rect,
                                   HWND parent, uint32_t id)
{
    int x0 = rect[0];
    int y0 = rect[1];
    int x1 = rect[2];
    int y1 = rect[3];
    LONG inst = GetWindowLongW(parent, -6);
    HWND hwnd;

    *(uint8_t *)((uint8_t *)obj + 0xd1) = (uint8_t)style & 0xa3;
    hwnd = CreateWindowExW(exStyle, WSTR("STATIC"), text, style & 0xffffff5fU,
                           x0, y0, x1 - x0, y1 - y0, parent,
                           (HMENU)(uintptr_t)id,
                           (HINSTANCE)(intptr_t)inst, (LPVOID)0);
    obj[4] = (uint64_t)(uintptr_t)hwnd;
    PECMD_ContainerAppend((uint64_t *)obj);
    return obj[4] != 0;
}

/* ========== FUN_1400FD538 @0x1400fd538 ==========
 * 使父窗口内围绕子窗口（可带外边距）的矩形区域失效。
 */
void FUN_1400FD538(HWND child, int margin)
{
    HWND parent;
    RECT rc;
    POINT pt;

    parent = GetParent(child);
    if (parent == (HWND)0) {
        return;
    }

    memset(&rc, 0, sizeof(rc));
    GetWindowRect(child, &rc);
    if (margin != 0) {
        rc.left -= margin;
        rc.top -= margin;
        rc.right += margin;
        rc.bottom += margin;
    }

    pt.x = 0;
    pt.y = 0;
    ClientToScreen(parent, &pt);
    OffsetRect(&rc, -pt.x, -pt.y);
    InvalidateRect(parent, &rc, 1);
}

/* ========== FUN_1400FE4A4 @0x1400fe4a4 ==========
 * 处理控件属性：空文本/color/enable 就地应用，其余转发到通用解析器。
 */
uint64_t PECMD_ApplyControlPropertyFe4a4(int64_t *obj, int64_t *ctx,
                                    LPCWSTR text, LPWSTR param,
                                    uint64_t a5, uint64_t a6,
                                    int64_t a7)
{
    WCHAR *s = (WCHAR *)text;

    if (*param == L'\0') {
        SetWindowTextW((HWND)obj[4], text);
        if (**(LPCWSTR *)(a7 + 0x10) != L'\0') {
            FUN_14007D0AC(*(int64_t **)(a7 + 0x50),
                                 *(LPCWSTR *)(a7 + 0x10), s);
        }
    } else {
        if (lstrcmpW(WSTR("color"), param) == 0) {
            int value[2] = { (int)0x80000000, 0 };

            *param = L'\0';
            PECMD_ParseUIntValue(&s, value);
            *(int *)((uint8_t *)obj + 0xa8) = value[0];
        } else if (lstrcmpW(WSTR("enable"), param) != 0) {
            return FUN_1400C493C(obj, ctx, s, (uint16_t *)param,
                                 a5, a6, a7);
        } else {
            int value[2] = { (int)0x80000000, 0 };

            *param = L'\0';
            PECMD_ParseUIntValue(&s, value);
        }
        FUN_1400FD538((HWND)obj[4], 1);
    }
    return 0;
}

/* ========== FUN_1400FF080 @0x1400ff080 ==========
 * 创建 SysTreeView32 子控件并挂到对象容器，立即更新。
 */
bool FUN_1400FF080(int64_t *obj, DWORD style, int *rect,
                                 HWND parent, uint32_t id)
{
    int x0 = rect[0];
    int y0 = rect[1];
    int x1 = rect[2];
    int y1 = rect[3];
    LONG inst = GetWindowLongW(parent, -6);
    HWND hwnd;

    hwnd = CreateWindowExW(0, WSTR("SysTreeView32"), (LPCWSTR)0, style,
                           x0, y0, x1 - x0, y1 - y0, parent,
                           (HMENU)(uintptr_t)id,
                           (HINSTANCE)(intptr_t)inst, (LPVOID)0);
    obj[4] = (uint64_t)(uintptr_t)hwnd;
    PECMD_ContainerAppend((uint64_t *)obj);
    UpdateWindow((HWND)obj[4]);
    return obj[4] != 0;
}

/* ========== FUN_1400FF154 @0x1400ff154 ==========
 * 组装 0x60 字节参数块并发送消息 0x1132。TODO(verify): 字段语义。
 */
void FUN_1400FF154(int64_t obj, uint32_t a2,
                                     uint64_t a3, uint32_t a4,
                                     uint32_t a5, uint32_t a6,
                                     uint32_t a7, uint64_t a8,
                                     uint64_t a9, uint64_t a10)
{
    uint8_t msg[0x60];

    memset(msg, 0, sizeof(msg));
    *(uint32_t *)(msg + 0x38) = a5;
    *(uint64_t *)(msg + 0x00) = a9;
    *(uint64_t *)(msg + 0x08) = a10;
    *(uint64_t *)(msg + 0x40) = a8;
    *(uint32_t *)(msg + 0x20) = a6;
    *(uint32_t *)(msg + 0x24) = a7;
    *(uint32_t *)(msg + 0x10) = a2;
    *(uint64_t *)(msg + 0x28) = a3;
    *(uint32_t *)(msg + 0x34) = a4;
    SendMessageW(*(HWND *)(obj + OBJ_HWND), 0x1132, 0, (LPARAM)msg);
}

/* ========== FUN_1400FF20C @0x1400ff20c ==========
 * 组装 0x50 字节参数块并发送消息 0x113f。TODO(verify): 字段语义。
 */
void FUN_1400FF20C(int64_t obj, uint64_t a2,
                                   uint32_t a3, uint64_t a4,
                                   uint32_t a5, uint32_t a6,
                                   uint32_t a7, uint32_t a8,
                                   uint64_t a9, uint32_t a10)
{
    uint8_t msg[0x50];

    memset(msg, 0, sizeof(msg));
    *(uint32_t *)(msg + 0x34) = a5;
    *(uint32_t *)(msg + 0x38) = a6;
    *(uint32_t *)(msg + 0x20) = a7;
    *(uint64_t *)(msg + 0x40) = a9;
    *(uint32_t *)(msg + 0x24) = a8;
    *(uint32_t *)(msg + 0x4c) = a10;
    *(uint32_t *)(msg + 0x10) = a3;
    *(uint64_t *)(msg + 0x18) = a2;
    *(uint64_t *)(msg + 0x28) = a4;
    SendMessageW(*(HWND *)(obj + OBJ_HWND), 0x113f, 0, (LPARAM)msg);
}
