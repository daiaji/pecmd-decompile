/* ====================================================================
 * core_b8f.c — B8 小函数批 6 (1400e0000-1400fffff)
 *
 * 本批新实现函数全部使用人类可读 PECMD_ 名称，原始地址保留在 @0x 注释。
 *
 * 来源: PECMD原始.EXE (x64, ImageBase=0x140000000)
 *   计算 PE 映像总大小     PECMD_CalcPeImageSize @0x1400e4078
 *   树节点按名查找         PECMD_SearchMenuTreeByCaption @0x1400e5458
 *   控件通知消息分发       PECMD_CtlNotifyDispatch @0x1400e59c0
 *   窗口枚举命中测试       FUN_1400E63C8 @0x1400e63c8
 *   关闭对话框/销毁窗口   FUN_1400E6860 @0x1400e6860
 *   是否管理员             PECMD_IsAdminGroupMember @0x1400e6f18
 *   销毁基类窗口对象       FUN_1400E8940 @0x1400e8940
 *   创建 tooltips 窗口     PECMD_CreateTooltipWnd @0x1400ebfe4
 *   添加字符串列表项       FUN_1400EC71C @0x1400ec71c
 *   初始化字符串对象       FUN_1400ECE2C @0x1400ece2c
 *   销毁 DC 对象           PECMD_DtorMemDcCanvas @0x1400eeea0
 *   居中绘制文本           PECMD_DrawVertCenteredText @0x1400ef08c
 *   矩形内绘制图标         PECMD_DrawIconInRect @0x1400eff58
 *   取/建对象画刷          PECMD_GetOrCreateCtlBrush @0x1400f0a3c
 *   刷新滚动条可见性       PECMD_RefreshListScrollbar @0x1400f2c44
 *   滚动列表到指定项       FUN_1400F31CC @0x1400f31cc
 *   滚动列表项到可见区     PECMD_ListScrollIntoView @0x1400f3264
 *   取图标尺寸             PECMD_GetIconSize @0x1400f35b8
 *   发送窗口几何通知       PECMD_ForcePosChanged @0x1400f3674
 *   按 ID 查找项(带串)    PECMD_ItemPropFindIdxNamed @0x1400f4114
 *   查找映射对槽           FUN_1400F4CA0 @0x1400f4ca0
 *   设置列表选择           PECMD_TableSetCurSel @0x1400f5104
 *   设置列表选择(单值)    PECMD_TableSetHoverIdx @0x1400f51d8
 *   发送控件 0x83 消息     PECMD_CtlSendNcCalcSize @0x1400f5338
 *   查找映射值 A           PECMD_ItemPropFindIdxList1 @0x1400f5584
 *   查找映射值 B           PECMD_ItemPropFindIdxList2 @0x1400f5608
 *   查找映射双值 A         PECMD_ItemPropFindIdxSub1 @0x1400f568c
 *   添加映射双值 A         PECMD_TrackItemChangeSub1 @0x1400f57f4
 *   查找映射值 C           PECMD_ItemPropFindIdxList3 @0x1400f593c
 *   查找映射值 D           PECMD_ItemPropFindIdxList4 @0x1400f59c0
 *   查找映射双值 B         PECMD_ItemPropFindIdxSub2 @0x1400f5a44
 *   添加映射双值 B         PECMD_TrackItemChangeSub2 @0x1400f5adc
 *   销毁静态控件对象       FUN_1400FD1A8 @0x1400fd1a8
 *   查询控件值(带类型)    PECMD_TreeGetItemState @0x1400feda4
 *   统计树节点数           PECMD_TreeCountItemsRecursive @0x1400fee94
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

/* ---- 本地小结构 (win32_stub.h 暂缺) ---- */
typedef struct {
    uint8_t Value[6];
} PECMD_SID_AUTHORITY;

typedef struct {
    BOOL     fIcon;
    uint32_t xHotspot;
    uint32_t yHotspot;
    HBITMAP  hbmMask;
    HBITMAP  hbmColor;
} PECMD_ICONINFO;

/* ---- 已实现公共工具 (其他 core_*.c) ---- */
extern BOOL FUN_1400E6314(HWND hwnd, POINT pt);                 /* @0x1400e6314 */
extern bool FUN_1400E6350(HWND hwnd, POINT *ctx);            /* @0x1400e6350 */
extern WCHAR **FUN_14007034C(WCHAR **ps, LPCWSTR src);             /* @0x14007034c */

/* ---- win32_stub.h 暂缺的 API ---- */
extern BOOL EndDialog(HWND hwnd, intptr_t result);
extern BOOL AllocateAndInitializeSid(PECMD_SID_AUTHORITY *pAuthority, BYTE nSubAuthorityCount,
                                     DWORD dwSubAuthority0, DWORD dwSubAuthority1,
                                     DWORD dwSubAuthority2, DWORD dwSubAuthority3,
                                     DWORD dwSubAuthority4, DWORD dwSubAuthority5,
                                     DWORD dwSubAuthority6, DWORD dwSubAuthority7,
                                     void **ppSid);
extern BOOL CheckTokenMembership(HANDLE token, void *sid, BOOL *isMember);
extern void FreeSid(void *sid);
extern BOOL GetIconInfo(HICON hIcon, PECMD_ICONINFO *pIconInfo);

/* ---- 未实现依赖 (extern + TODO(verify)) ---- */
extern int64_t PECMD_AlignUpSize(int64_t value, uint32_t align);
extern void *FUN_1400E57C0(void *obj);
extern void PECMD_AllocStrSlot(WCHAR **ps);
extern int64_t * PECMD_InitPtrTable(int64_t *arr);
extern void *FUN_140063B00(int64_t idx, int64_t *arr, int64_t *cap,
                           uint32_t esize);
extern void FUN_1400639F0(int64_t *arr, int64_t *cap, int64_t *cnt,
                          void *data, int64_t esize, int32_t mode);
extern void FUN_14005D558(void *obj, HWND hwnd);
extern void PECMD_FreeContainer(int64_t *container);
extern int64_t PECMD_ContainerAppend(uint64_t *container);
extern void PECMD_DtorCtlCoreObj(uint64_t *obj);
extern HBRUSH PECMD_CtlCalcColorBrush(int64_t obj, HDC hdc);   /* @0x1400fd35c */
extern void FUN_1400F30C0(int64_t obj, int mode, int value);

/* ---- 本批引用的虚表/数据符号 ---- */
extern uint8_t PTR_FUN_14012b240[];
extern uint8_t PTR_FUN_14012bad0[];
extern uint8_t PTR_FUN_14012bd70[];
extern uint8_t PTR_FUN_14012ccc0[];

/* ========== PECMD_CalcPeImageSize @0x1400e4078 ==========
 * 计算 PE 文件映像所需总尺寸：按节对齐累加各节大小，越界时返回 0。
 * TODO(verify): sectionTable 指向节表头，40 字节/项。
 */
int64_t PECMD_CalcPeImageSize(uint64_t file, uint32_t fileSize,
                              int64_t peHeader, int64_t sectionTable)
{
    uint32_t align = *(uint32_t *)(peHeader + 0x38);
    int64_t total = PECMD_AlignUpSize(*(uint32_t *)(peHeader + 0x54), align);
    int count = *(uint16_t *)(peHeader + 6);
    int i;

    (void)file;
    for (i = 0; i < count; i++) {
        uint32_t *entry = (uint32_t *)((char *)sectionTable + 0x10 +
                                       (int64_t)i * 40);
        uint32_t uVar1 = entry[0];

        if (fileSize < uVar1 + entry[1]) {
            return 0;
        }
        if (entry[-1] == 0) {
            if (uVar1 <= entry[-2]) {
                uVar1 = entry[-2];
            }
            total += PECMD_AlignUpSize((int64_t)uVar1, align);
        } else {
            if (entry[-2] != 0) {
                uVar1 = entry[-2];
            }
            total = PECMD_AlignUpSize((int64_t)(entry[-1] + uVar1), align);
        }
    }
    return total;
}

/* ========== PECMD_SearchMenuTreeByCaption @0x1400e5458 ==========
 * 在带 0x80 标记的树/链节点中按名称 (lstrcmpiW) 查找节点。
 */
void *PECMD_SearchMenuTreeByCaption(int64_t node, LPCWSTR name)
{
    int count = *(int *)(node + 4);
    void **array = *(void ***)(node + 8);
    int i;

    for (i = 0; i < count; i++) {
        uint8_t *item = (uint8_t *)array[i];
        if (*(int8_t *)item == -0x80) {
            void *found = PECMD_SearchMenuTreeByCaption(*(int64_t *)(item + 0x10), name);
            if (found != NULL) {
                return found;
            }
        } else if (lstrcmpiW(*(LPCWSTR *)(item + 0x18), name) == 0) {
            return item;
        }
    }
    return NULL;
}

/* ========== PECMD_CtlNotifyDispatch @0x1400e59c0 ==========
 * 把 0..6 的控件通知索引映射为 0x132..0x138 消息并调用虚表 +8。
 */
LRESULT PECMD_CtlNotifyDispatch(int64_t *obj, WPARAM wParam,
                                     LPARAM lParam, int index)
{
    UINT msg;
    LRESULT (*fn)(int64_t, UINT, WPARAM, LPARAM);

    switch (index) {
    case 0: msg = 0x132; break;
    case 1: msg = 0x133; break;
    case 2: msg = 0x134; break;
    case 3: msg = 0x135; break;
    case 4: msg = 0x136; break;
    case 5: msg = 0x137; break;
    case 6: msg = 0x138; break;
    default: return 0;
    }
    fn = *(LRESULT (**)(int64_t, UINT, WPARAM, LPARAM))(*obj + 8);
    return fn(obj[4], msg, wParam, lParam);
}

/* ========== FUN_1400E63C8 @0x1400e63c8 ==========
 * EnumWindows 回调：命中指定点且属于当前进程的窗口时继续查子窗口。
 * ctx 前 24 字节为调用方上下文，ctx[3] 保存命中的 HWND。
 */
BOOL FUN_1400E63C8(HWND hwnd, LPARAM lParam)
{
    POINT *ctx = (POINT *)lParam;
    DWORD pid[2] = { 0, 0 };

    if (FUN_1400E6314(hwnd, *ctx) != 0 &&
        (GetWindowThreadProcessId(hwnd, pid), pid[0] == (DWORD)ctx[2].x)) {
        uint64_t hit = (uint64_t)SendMessageW(hwnd, 0x45e,
                                              (WPARAM)(int64_t)ctx[1].x,
                                              (LPARAM)(uintptr_t)hwnd);
        if ((hit >> 0x11 & 1) != 0) {
            *(uintptr_t *)(ctx + 3) = (uintptr_t)hwnd;
        }
        EnumChildWindows(hwnd, (void *)FUN_1400E6350, (LPARAM)ctx);
        return *(uintptr_t *)(ctx + 3) == 0;
    }
    return TRUE;
}

/* ========== FUN_1400E6860 @0x1400e6860 ==========
 * 关闭对话框；若对象已标记关闭则销毁关联窗口。
 */
void FUN_1400E6860(uint64_t *obj, int result)
{
    uint8_t flags = *(uint8_t *)((char *)obj + 0x120);

    if ((flags & 1) == 0) {
        HWND hwnd;

        if (flags == 0) {
            hwnd = (HWND)obj[4];
        } else {
            *(uint8_t *)((char *)obj + 0x120) = flags | 0x80;
            PostMessageW((HWND)obj[4], 0, (WPARAM)(uintptr_t)obj,
                         (LPARAM)(intptr_t)result);
            hwnd = (HWND)obj[4];
        }
        EndDialog(hwnd, (intptr_t)result);
    } else {
        HWND hwnd = (HWND)obj[4];

        if (hwnd != (HWND)0) {
            obj[4] = 0;
            if (IsWindow(hwnd) != 0) {
                DestroyWindow(hwnd);
            }
        }
    }
}

/* ========== PECMD_IsAdminGroupMember @0x1400e6f18 ==========
 * 检查当前令牌是否属于 BUILTIN\Administrators (S-1-5-32-544)。
 */
BOOL PECMD_IsAdminGroupMember(void)
{
    PECMD_SID_AUTHORITY authority = { { 0, 0, 0, 0, 0, 5 } };
    void *sid = NULL;
    BOOL isMember = FALSE;

    if (AllocateAndInitializeSid(&authority, 2, 32, 544, 0, 0, 0, 0, 0, 0,
                                 &sid) != 0) {
        CheckTokenMembership((HANDLE)0, sid, &isMember);
        FreeSid(sid);
    }
    return isMember;
}

/* ========== FUN_1400E8940 @0x1400e8940 ==========
 * 基类窗口对象析构：恢复虚表、销毁关联窗口、删除 GDI 资源并释放容器。
 */
void FUN_1400E8940(uint64_t *obj)
{
    HWND hwnd = (HWND)obj[4];

    *obj = (uint64_t)(uintptr_t)PTR_FUN_14012b240;
    if (hwnd != (HWND)0) {
        obj[4] = 0;
        if (IsWindow(hwnd) != 0) {
            SetWindowLongPtrW(hwnd, GWLP_WNDPROC, 0x140001188);
            DestroyWindow(hwnd);
            if ((HGDIOBJ)obj[7] != (HGDIOBJ)0) {
                DeleteObject((HGDIOBJ)obj[7]);
                obj[7] = 0;
            }
            if ((HGDIOBJ)obj[0xd] != (HGDIOBJ)0) {
                DeleteObject((HGDIOBJ)obj[0xd]);
                obj[0xd] = 0;
            }
            if (obj[0x18] != 0) {
                FUN_14005D558(obj + 0x18, (HWND)obj[4]);
            }
        }
    }
    PECMD_FreeContainer((int64_t *)(obj + 0x18));
}

/* ========== PECMD_CreateTooltipWnd @0x1400ebfe4 ==========
 * 创建 tooltips_class32 顶层子窗口并挂到对象 +0x20，随后容器追加。
 */
BOOL PECMD_CreateTooltipWnd(int64_t *obj, HWND parent, uint32_t style)
{
    HINSTANCE hInst = (HINSTANCE)GetWindowLongPtrW(parent, GWLP_HINSTANCE);
    HWND hwnd = CreateWindowExW(0, WSTR("tooltips_class32"), NULL,
                                style | 0x80000080u,
                                CW_USEDEFAULT, CW_USEDEFAULT,
                                CW_USEDEFAULT, CW_USEDEFAULT,
                                parent, (HMENU)0, hInst, NULL);

    obj[4] = (int64_t)hwnd;
    (void)PECMD_ContainerAppend((uint64_t *)obj);
    return obj[4] != 0;
}

/* ========== FUN_1400EC71C @0x1400ec71c ==========
 * 分配 16 字节节点，保存值并初始化/赋值字符串，追加到 +0x108 数组。
 */
int64_t FUN_1400EC71C(int64_t obj, LPCWSTR text, uint64_t value)
{
    int count = *(int *)(obj + 0x118);
    uint64_t *node = (uint64_t *)calloc(1, 0x10);
    uint64_t *slot;

    if (node == NULL) {
        return 0;
    }
    node[0] = value;
    PECMD_AllocStrSlot((WCHAR **)(node + 1));
    FUN_14007034C((WCHAR **)(node + 1), text);
    slot = (uint64_t *)FUN_140063B00((int64_t)count,
                                     (int64_t *)(obj + 0x108),
                                     (int64_t *)(obj + 0x110), 8);
    *slot = (uint64_t)node;
    *(int64_t *)(obj + 0x118) = (int64_t)(count + 1);
    return 1;
}

/* ========== FUN_1400ECE2C @0x1400ece2c ==========
 * 初始化带字符串和数组的对象，设置虚表 PTR_FUN_14012bad0 及多个 -1 字段。
 */
uint64_t *FUN_1400ECE2C(uint64_t *obj, uint64_t param2)
{
    (void)FUN_1400E57C0(obj);
    *obj = (uint64_t)(uintptr_t)PTR_FUN_14012bad0;
    PECMD_AllocStrSlot((WCHAR **)(obj + 0x1b));
    obj[0x1d] = param2;
    PECMD_InitPtrTable((int64_t *)(obj + 0x21));
    *(uint32_t *)((char *)obj + 0xfc) = 0xffffffffu;
    *(uint32_t *)((char *)obj + 0x100) = 0xffffffffu;
    *(uint32_t *)((char *)obj + 0xf8) = 0xffffffffu;
    *(uint32_t *)((char *)obj + 0x104) = 0xffffffffu;
    obj[0x1c] = 0;
    *(uint32_t *)((char *)obj + 0x120) = 0;
    *(uint8_t *)((char *)obj + 0x14) = 0;
    obj[0x1e] = 0;
    return obj;
}

/* ========== PECMD_DtorMemDcCanvas @0x1400eeea0 ==========
 * DC 对象析构：还原画刷、释放容器回调、释放 DC 后调用基类析构尾段。
 */
void PECMD_DtorMemDcCanvas(uint64_t *obj)
{
    void *releaseObj;

    *obj = (uint64_t)(uintptr_t)PTR_FUN_14012bd70;
    if ((HDC)obj[0x1e] != (HDC)0 && (HGDIOBJ)obj[0x21] != (HGDIOBJ)0) {
        SelectObject((HDC)obj[0x1e], (HGDIOBJ)obj[0x21]);
    }
    releaseObj = (void *)obj[0x22];
    if (releaseObj != NULL) {
        void (*fn)(void *, int) =
            *(void (**)(void *, int))((char *)*(void **)releaseObj + 0x18);
        fn(releaseObj, 1);
    }
    obj[0x22] = 0;
    if ((HDC)obj[0x1e] != (HDC)0) {
        ReleaseDC((HWND)obj[0x1f], (HDC)obj[0x1e]);
        obj[0x1e] = 0;
    }
    PECMD_DtorCtlCoreObj(obj);
}

/* ========== PECMD_DrawVertCenteredText @0x1400ef08c ==========
 * 先用 DT_CALCRECT 测量，再纵向居中并右移 2 像素后实际绘制。
 */
void PECMD_DrawVertCenteredText(HDC hdc, LPCWSTR text, int length,
                            RECT *rect, uint32_t flags)
{
    RECT measure = *rect;

    DrawTextW(hdc, text, -1, &measure, flags | 0x400);
    OffsetRect(rect, 0, (((rect->bottom - rect->top) - measure.bottom) +
                         measure.top) / 2);
    rect->left += 2;
    DrawTextW(hdc, text, length, rect, flags);
}

/* ========== PECMD_DrawIconInRect @0x1400eff58 ==========
 * 在给定 RECT 内按最小边等比绘制 +0xd0 处的图标。
 */
void PECMD_DrawIconInRect(int64_t obj, HDC hdc, RECT *rect, int centerX)
{
    HICON icon = *(HICON *)(obj + OBJ_LINK);
    int width;
    int height;
    int xLeft;
    int yTop;

    if (icon == (HICON)0) {
        return;
    }
    width = rect->right - rect->left;
    height = rect->bottom - rect->top;
    xLeft = rect->left;
    if (height < width) {
        width = height;
    }
    if (width < height) {
        height = width;
    }
    if (centerX != 0) {
        xLeft += (rect->right - rect->left - width) / 2;
    }
    yTop = rect->top + (rect->bottom - rect->top - height) / 2;
    DrawIconEx(hdc, xLeft, yTop, icon, width, height, 0, (HBRUSH)0, 3);
}

/* ========== PECMD_GetOrCreateCtlBrush @0x1400f0a3c ==========
 * 取对象缓存画刷；无缓存时创建，特殊负值模式转交 PECMD_CtlCalcColorBrush。
 */
uint64_t PECMD_GetOrCreateCtlBrush(int64_t obj, HDC hdc)
{
    if (*(int *)(obj + 0xa8) < -1 && *(int *)(obj + 0x44) < 0) {
        return (uint64_t)PECMD_CtlCalcColorBrush(obj, hdc);
    }

    SetBkMode(hdc, TRANSPARENT);
    if (-1 < (int)*(COLORREF *)(obj + 0xa8)) {
        SetTextColor(hdc, *(COLORREF *)(obj + 0xa8));
    }
    if (*(int64_t *)(obj + 0x48) == 0) {
        if (*(int *)(obj + 0x44) < 0) {
            *(uint32_t *)(obj + 0x44) = (uint32_t)GetSysColor(COLOR_BTNFACE);
        }
        *(HBRUSH *)(obj + 0x48) = CreateSolidBrush(*(COLORREF *)(obj + 0x44));
    }
    return *(uint64_t *)(obj + 0x48);
}

/* ========== PECMD_RefreshListScrollbar @0x1400f2c44 ==========
 * 列表/滚动控件刷新：根据消息 0x1004 的结果判断是否需要隐藏/显示滚动条。
 * TODO(verify): 0x1022 输出缓冲布局。
 */
void PECMD_RefreshListScrollbar(int64_t obj)
{
    uint8_t buf[0x10] = { 0 };
    RECT client;
    RECT win;
    LRESULT result;

    if ((*(uint8_t *)(obj + 0x3f8) & 0xc) != 4) {
        return;
    }
    result = SendMessageW(*(HWND *)(obj + OBJ_HWND), 0x1004, 0, 0);
    if (0 < (int)result) {
        SendMessageW(*(HWND *)(obj + OBJ_HWND), 0x1022, 0, (LPARAM)buf);
        GetClientRect(*(HWND *)(obj + OBJ_HWND), &client);
        GetWindowRect(*(HWND *)(obj + 0xf8), &win);
        if (client.bottom < (win.bottom - win.top) + *(int *)(buf + 12)) {
            ShowScrollBar(*(HWND *)(obj + OBJ_HWND), 1, 0);
            ShowScrollBar(*(HWND *)(obj + OBJ_HWND), 1, 1);
        }
    }
}

/* ========== FUN_1400F31CC @0x1400f31cc ==========
 * 使列表/树控件显示指定项；有映射表时展开为多个连续项。
 */
void FUN_1400F31CC(int64_t obj, int index)
{
    int64_t table = 0;
    int64_t i = 0;
    int16_t count;

    if (*(int64_t *)(obj + 0x3a0) > 0) {
        table = *(int64_t *)(obj + 0x398);
    }
    if (table == 0) {
        SendMessageW(*(HWND *)(obj + OBJ_HWND), 0x102a, (WPARAM)(int64_t)index, 0);
        return;
    }

    table = *(int64_t *)(table + (int64_t)index * 8);
    count = *(int16_t *)(*(int64_t *)(obj + 0x368) + table * 2);
    for (i = 0; i < (int64_t)count; i++) {
        SendMessageW(*(HWND *)(obj + OBJ_HWND), 0x102a,
                     (WPARAM)(int64_t)(table + i), 0);
    }
}

/* ========== PECMD_ListScrollIntoView @0x1400f3264 ==========
 * 发送列表项矩形/滚动消息并只失效对应区域。
 */
void PECMD_ListScrollIntoView(int64_t obj, int index)
{
    RECT area = { 0, 0, 0, 0 };
    RECT client = { 0, 0, 0, 0 };
    UINT msg;

    GetClientRect(*(HWND *)(obj + OBJ_HWND), &client);
    if (index == 0) {
        area.left = 2;
        msg = 0x100e;
    } else {
        area.left = 0;
        area.top = index;
        msg = 0x1038;
    }
    SendMessageW(*(HWND *)(obj + OBJ_HWND), msg, 0, (LPARAM)&area);
    area.bottom = (client.right - client.left) + area.top;
    InvalidateRect(*(HWND *)(obj + OBJ_HWND), &area, TRUE);
}

/* ========== PECMD_GetIconSize @0x1400f35b8 ==========
 * 从图标位图获取宽高；成功返回 1，*width 与 *height 为 BITMAP 宽高。
 */
BOOL PECMD_GetIconSize(HICON icon, int *width, int *height)
{
    PECMD_ICONINFO info;
    uint8_t bitmap[32];

    memset(&info, 0, sizeof(info));
    *width = 0;
    *height = 0;
    if (GetIconInfo(icon, &info) == 0) {
        return FALSE;
    }
    if (GetObjectW(info.hbmColor, 0x20, bitmap) == 0) {
        return FALSE;
    }
    DeleteObject(info.hbmColor);
    DeleteObject(info.hbmMask);
    *width = *(int *)(bitmap + 4);
    *height = *(int *)(bitmap + 8);
    return TRUE;
}

/* ========== PECMD_ForcePosChanged @0x1400f3674 ==========
 * 发送 0x47 窗口几何/位置通知结构（含 HWND、宽高和标志 0x216）。
 */
void PECMD_ForcePosChanged(HWND hwnd)
{
    uint8_t buf[0x30];
    RECT rc;
    int width;
    int height;

    memset(buf, 0, sizeof(buf));
    GetWindowRect(hwnd, &rc);
    width = rc.right - rc.left;
    height = rc.bottom - rc.top;
    *(HWND *)(buf + 0) = hwnd;
    *(int *)(buf + 0x18) = width;
    *(int *)(buf + 0x1c) = height;
    *(uint32_t *)(buf + 0x20) = 0x216;
    SendMessageW(hwnd, 0x47, 0, (LPARAM)buf);
}

/* ========== PECMD_ItemPropFindIdxNamed @0x1400f4114 ==========
 * 在 +0x2f0 数组中按 ID 查找项，写出值，并可选复制关联字符串。
 */
int64_t PECMD_ItemPropFindIdxNamed(int64_t obj, int id, uint64_t *outValue,
                                     WCHAR **outString)
{
    int count = *(int *)(obj + 0x300);
    int i;

    for (i = 0; i < count; i++) {
        uint8_t *item = *(uint8_t **)(*(int64_t *)(obj + 0x2f0) +
                                      (int64_t)i * 8);
        if (item != NULL && *(int *)item == id) {
            LPCWSTR str;

            if (outValue != NULL) {
                *outValue = *(uint64_t *)(item + 8);
            }
            str = *(LPCWSTR *)(item + 0x10);
            if (outString != NULL && str != NULL && *str != L'\0') {
                PECMD_StrBldCopyWide(outString, str);
            }
            return i;
        }
    }
    return -1;
}

/* ========== FUN_1400F4CA0 @0x1400f4ca0 ==========
 * 在 +0x350 数组中查找 +8/+0x10 双键匹配项，返回槽指针。
 */
int64_t *FUN_1400F4CA0(int64_t obj, int64_t key1, int64_t key2)
{
    int count = *(int *)(obj + 0x360);
    int i;

    for (i = 0; i < count; i++) {
        int64_t *slot = (int64_t *)FUN_140063B00((int64_t)i,
                                                 (int64_t *)(obj + 0x350),
                                                 (int64_t *)(obj + 0x358), 8);
        int64_t item = *slot;

        if (item != 0 && *(int64_t *)(item + 8) == key1 &&
            *(int64_t *)(item + 0x10) == key2) {
            return slot;
        }
    }
    return NULL;
}

/* ========== PECMD_TableSetCurSel @0x1400f5104 ==========
 * 更新列表当前项/滚动项，并刷新旧选择区域。
 */
void PECMD_TableSetCurSel(int64_t obj, int current, int scroll)
{
    int oldCurrent = *(int *)(obj + 0x3d4);
    int oldSel = *(int *)(obj + 0x3d0);
    int oldScroll = *(int *)(obj + 0x3cc);

    *(uint32_t *)(obj + 0x3d4) = 0xfffffffeu;
    *(int *)(obj + 0x3d0) = current;
    *(int *)(obj + 0x3cc) = scroll;

    if (oldCurrent >= 0) {
        FUN_1400F31CC(obj, oldCurrent);
    }
    if (oldSel >= 0 && oldScroll >= 0 && oldCurrent != oldSel &&
        (scroll != oldScroll || current != oldSel)) {
        FUN_1400F30C0(obj, oldSel, oldScroll);
    }
    if (oldCurrent != current && current >= 0) {
        FUN_1400F31CC(obj, current);
    }
}

/* ========== PECMD_TableSetHoverIdx @0x1400f51d8 ==========
 * 只设置当前项，旧选择/滚动清为 -2。
 */
void PECMD_TableSetHoverIdx(int64_t obj, int current)
{
    int oldCurrent = *(int *)(obj + 0x3d4);
    int oldSel = *(int *)(obj + 0x3d0);
    int oldScroll = *(int *)(obj + 0x3cc);

    *(int *)(obj + 0x3d4) = current;
    *(uint32_t *)(obj + 0x3cc) = 0xfffffffeu;
    *(uint32_t *)(obj + 0x3d0) = 0xfffffffeu;

    if (oldCurrent >= 0 && oldCurrent != current) {
        FUN_1400F31CC(obj, oldCurrent);
    }
    if (oldSel >= 0 && oldScroll >= 0 && oldSel != oldCurrent &&
        oldSel != current) {
        FUN_1400F30C0(obj, oldSel, oldScroll);
    }
    if (current != oldCurrent && current >= 0) {
        FUN_1400F31CC(obj, current);
    }
}

/* ========== PECMD_CtlSendNcCalcSize @0x1400f5338 ==========
 * 发送 WM_SETREDRAW(0x83) 前清除控件滚动样式位。
 */
void PECMD_CtlSendNcCalcSize(int64_t *obj, WPARAM wParam, LPARAM lParam)
{
    uint32_t bits = 0;
    uint32_t style;

    if ((*(uint16_t *)((char *)obj + 0x3f8) & 4) != 0) {
        bits = 0x100000;
    }
    if ((*(uint16_t *)((char *)obj + 0x3f8) & 8) != 0) {
        bits |= 0x200000;
    }
    if (bits != 0) {
        style = (uint32_t)GetWindowLongW((HWND)obj[4], GWL_STYLE);
        SetWindowLongW((HWND)obj[4], GWL_STYLE, (LONG)(style & ~bits));
    }
    {
        LRESULT (*fn)(int64_t, UINT, WPARAM, LPARAM) =
            *(LRESULT (**)(int64_t, UINT, WPARAM, LPARAM))(*obj + 8);
        fn(obj[4], 0x83, wParam, lParam);
    }
}

/* ---- 映射数组查找/添加辅助 ---- */
static int64_t PECMD_FindSingleMapValue(int64_t obj, int dataOff, int capOff,
                                        int countOff, int key, int *outValue)
{
    int count = *(int *)(obj + countOff);
    int i;

    for (i = 0; i < count; i++) {
        int64_t *slot = (int64_t *)FUN_140063B00((int64_t)i,
                                                 (int64_t *)(obj + dataOff),
                                                 (int64_t *)(obj + capOff), 8);
        int *node = (int *)*slot;

        if (node != NULL && *node == key) {
            *outValue = node[2];
            return i;
        }
    }
    return -1;
}

static int64_t PECMD_FindDoubleMapValue(int64_t obj, int dataOff, int capOff,
                                        int countOff, int key1, int key2,
                                        int *outValue)
{
    int count = *(int *)(obj + countOff);
    int i;

    for (i = 0; i < count; i++) {
        int64_t *slot = (int64_t *)FUN_140063B00((int64_t)i,
                                                 (int64_t *)(obj + dataOff),
                                                 (int64_t *)(obj + capOff), 8);
        int *node = (int *)*slot;

        if (node != NULL && *node == key1 && node[4] == key2) {
            *outValue = node[2];
            return i;
        }
    }
    return -1;
}

static void PECMD_AddDoubleMapEntry(int64_t obj, int dataOff, int capOff,
                                    int countOff, int key1, int key2,
                                    uint64_t value)
{
    uint32_t *node = (uint32_t *)calloc(1, 0x18);
    int64_t *arr = (int64_t *)(obj + dataOff);
    int64_t *cap = (int64_t *)(obj + capOff);
    int64_t *cnt = (int64_t *)(obj + countOff);

    node[0] = (uint32_t)key1;
    node[4] = (uint32_t)key2;
    *(uint64_t *)(node + 2) = value & 0xffffffff;
    FUN_1400639F0(arr, cap, cnt, &node, 8, 1);
}

/* ========== PECMD_ItemPropFindIdxList1 @0x1400f5584 ========== */
int64_t PECMD_ItemPropFindIdxList1(int64_t obj, int key, int *outValue)
{
    return PECMD_FindSingleMapValue(obj, 0x260, 0x268, 0x270, key, outValue);
}

/* ========== PECMD_ItemPropFindIdxList2 @0x1400f5608 ========== */
int64_t PECMD_ItemPropFindIdxList2(int64_t obj, int key, int *outValue)
{
    return PECMD_FindSingleMapValue(obj, 0x278, 0x280, 0x288, key, outValue);
}

/* ========== PECMD_ItemPropFindIdxSub1 @0x1400f568c ========== */
int64_t PECMD_ItemPropFindIdxSub1(int64_t obj, int key1, int key2, int *outValue)
{
    return PECMD_FindDoubleMapValue(obj, 0x290, 0x298, 0x2a0,
                                    key1, key2, outValue);
}

/* ========== PECMD_TrackItemChangeSub1 @0x1400f57f4 ========== */
void PECMD_TrackItemChangeSub1(int64_t obj, int key1, int key2, uint64_t value)
{
    PECMD_AddDoubleMapEntry(obj, 0x290, 0x298, 0x2a0,
                            key1, key2, value);
}

/* ========== PECMD_ItemPropFindIdxList3 @0x1400f593c ========== */
int64_t PECMD_ItemPropFindIdxList3(int64_t obj, int key, int *outValue)
{
    return PECMD_FindSingleMapValue(obj, 0x2a8, 0x2b0, 0x2b8, key, outValue);
}

/* ========== PECMD_ItemPropFindIdxList4 @0x1400f59c0 ========== */
int64_t PECMD_ItemPropFindIdxList4(int64_t obj, int key, int *outValue)
{
    return PECMD_FindSingleMapValue(obj, 0x2c0, 0x2c8, 0x2d0, key, outValue);
}

/* ========== PECMD_ItemPropFindIdxSub2 @0x1400f5a44 ========== */
int64_t PECMD_ItemPropFindIdxSub2(int64_t obj, int key1, int key2, int *outValue)
{
    return PECMD_FindDoubleMapValue(obj, 0x2d8, 0x2e0, 0x2e8,
                                    key1, key2, outValue);
}

/* ========== PECMD_TrackItemChangeSub2 @0x1400f5adc ========== */
void PECMD_TrackItemChangeSub2(int64_t obj, int key1, int key2, uint64_t value)
{
    PECMD_AddDoubleMapEntry(obj, 0x2d8, 0x2e0, 0x2e8,
                            key1, key2, value);
}

/* ========== FUN_1400FD1A8 @0x1400fd1a8 ==========
 * 静态控件对象析构：销毁图标/画刷/位图后调用基类析构。
 */
void FUN_1400FD1A8(uint64_t *obj)
{
    *obj = (uint64_t)(uintptr_t)PTR_FUN_14012ccc0;
    if ((HICON)obj[0x1c] != (HICON)0) {
        DestroyIcon((HICON)obj[0x1c]);
        obj[0x1c] = 0;
    }
    if ((HGDIOBJ)obj[0x1d] != (HGDIOBJ)0) {
        DeleteObject((HGDIOBJ)obj[0x1d]);
    }
    obj[0x1d] = 0;
    if ((HGDIOBJ)obj[0x10] != (HGDIOBJ)0) {
        DeleteObject((HGDIOBJ)obj[0x10]);
    }
    if ((HGDIOBJ)obj[0x11] != (HGDIOBJ)0) {
        DeleteObject((HGDIOBJ)obj[0x11]);
    }
    FUN_1400E8940(obj);
}

/* ========== PECMD_TreeGetItemState @0x1400feda4 ==========
 * 发送 0x113e 查询结构；成功时把 +0x10 处的 32 位结果写入 *out。
 */
int PECMD_TreeGetItemState(int64_t obj, uint64_t param2,
                                 uint64_t *out, uint32_t param4)
{
    uint8_t buf[0x38];
    LRESULT result;

    memset(buf, 0, sizeof(buf));
    *out = 0;
    *(uint32_t *)(buf + 0) = 8;
    *(uint64_t *)(buf + 8) = param2;
    *(uint32_t *)(buf + 0x14) = param4;
    result = SendMessageW(*(HWND *)(obj + OBJ_HWND), 0x113e, 0, (LPARAM)buf);
    if ((int)result != 0) {
        *out = *(uint32_t *)(buf + 0x10);
    }
    return (int)result;
}

/* ========== PECMD_TreeCountItemsRecursive @0x1400fee94 ==========
 * 统计树控件从指定项开始的兄弟节点数；recurse!=0 时不深入子节点。
 * 传入 0 返回 -1。
 */
int64_t PECMD_TreeCountItemsRecursive(int64_t obj, LRESULT hItem, int64_t recurse)
{
    int64_t total = 0;

    if (hItem == 0) {
        return -1;
    }
    while (hItem != 0) {
        total++;
        if (recurse == 0) {
            LRESULT child = SendMessageW(*(HWND *)(obj + OBJ_HWND), 0x110a, 4,
                                         hItem);
            if (child != 0) {
                int64_t childCount = PECMD_TreeCountItemsRecursive(obj, child, 0);
                if (childCount < 0) {
                    return -1;
                }
                total += childCount;
            }
        }
        hItem = SendMessageW(*(HWND *)(obj + OBJ_HWND), 0x110a, 1, hItem);
    }
    return total;
}
