/* ====================================================================
 * core_b8j.c — B8 中批 2 (1400e0000-1400fffff)
 *
 * 本批新实现函数全部使用人类可读 PECMD_ 名称，原始地址保留在 @0x 注释。
 *
 * 来源: PECMD原始.EXE (x64, ImageBase=0x140000000)
 *   创建文件映射视图       PECMD_MapSharedSection @0x1400e3e38
 *   构建电话簿列表         FUN_1400E7840   @0x1400e7840
 *   更新子窗口顺序         FUN_1400EC880   @0x1400ec880
 *   编辑控件按键消息       FUN_1400EEF20    @0x1400eef20
 *   应用控件颜色           FUN_1400EFD80    @0x1400efd80
 *   填充客户区颜色         PECMD_FillCtlBackground  @0x1400f0ca0
 *   查询控件值分发         FUN_1400F1234   @0x1400f1234
 *   控件按键消息处理       FUN_1400F1F78  @0x1400f1f78
 *   控件消息处理 A         FUN_1400F2DD4    @0x1400f2dd4
 *   命中测试控件           PECMD_ListSubItemHitTest       @0x1400f3308
 *   设置映射对条目         FUN_1400F4D1C      @0x1400f4d1c
 *   设置数组项条目         FUN_1400F60A4    @0x1400f60a4
 *   按值设置项             FUN_1400F8FCC     @0x1400f8fcc
 *   控件消息处理 B         FUN_1400FEF3C    @0x1400fef3c
 *   格式化控件范围串       PECMD_BuildTreeIndexPathStr  @0x1400ff2bc
 *
 * 约定:
 *   - 新实现函数使用 PECMD_ 可读名；未实现依赖仍 extern FUN_ + TODO(verify)
 *   - 全局数据符号先用 extern 声明; 定义统一放在 core_globals.c
 * ==================================================================== */
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "pecmd_defs.h"

/* ---- 已实现公共工具 (pecmd_defs.h / core_*.c) ---- */
extern void FUN_1400633A8(void **ps, int64_t len);          /* @0x1400633a8 */
extern void FUN_14007D0AC(int64_t **ctx, LPCWSTR key, LPCWSTR value); /* @0x14007d0ac */
extern bool PECMD_ParseUIntValue(WCHAR **pp, int *out);          /* @0x140067d20 */
extern COLORREF FUN_1400E68E0(HDC hdc, RECT *rc, COLORREF color); /* @0x1400e68e0 */
extern int64_t FUN_140063B00(int64_t idx, int64_t *arr, int64_t *cap, uint32_t esize); /* @0x140063b00 */
extern int64_t *FUN_1400F4CA0(int64_t obj, int64_t key1, int64_t key2);  /* @0x1400f4ca0 */
extern int64_t *FUN_1400F4C28(int64_t obj, int64_t value);             /* @0x1400f4c28 */
extern int64_t PECMD_ItemPropFindIdxNamed(int64_t obj, int id, uint64_t *outValue, WCHAR **outString);                  /* @0x1400f4114 */
extern void FUN_1400FED38(int64_t obj, uint64_t hItem, uint64_t *out); /* @0x1400fed38 */
extern void FUN_1400EC7C0(int64_t obj, char mode);        /* @0x1400ec7c0 */
extern void FUN_1400F2B6C(int64_t obj);                                   /* @0x1400f2b6c */
extern int64_t FUN_1400639F0(int64_t *arr, int64_t *cap, int64_t *cnt,
                                        uint8_t *item, uint32_t esize, int64_t mode); /* @0x1400639f0 */
extern HWND FUN_1400E5788(HWND hwnd);                                  /* @0x1400e5788 */

/* ---- 未实现依赖 (extern + TODO(verify)) ---- */
extern uint16_t FUN_1400F172C(int64_t *map, int msg, uint64_t wParam, uint64_t *lParam,
                              int64_t hwnd, uint8_t mode, uint64_t *out);
extern int64_t FUN_1400E5B0C(int64_t obj, uint64_t wParam, int64_t lParam,
                             int64_t *out);
extern uint64_t FUN_1400C493C(int64_t *obj, int64_t *ctx, LPCWSTR value,
                              uint16_t *name, uint64_t a5, uint64_t a6,
                              int64_t a7);
extern void FUN_1400EF91C(int64_t obj, uint32_t flags, uint64_t value);
extern void PECMD_SelectObjectSlot_b028(uint64_t *slot, HDC hdc, HGDIOBJ obj);
extern void PECMD_RestoreAndDeleteObject(uint64_t *slot);
extern void PECMD_BltTransparentBits(HDC hdcDst, int x0, int y0, int w, int h, HDC hdcSrc,
                          int sx, int sy, uint64_t p9, uint64_t p10, COLORREF color);
extern void FUN_14005B0D4(int64_t *ps);
extern int wsprintfW(LPWSTR buf, LPCWSTR fmt, ...);
extern void *operator_new(size_t size);   /* 全局 new 包装 */

/* ---- 本批引用的数据符号 (.rdata) ---- */
extern WCHAR g_szEmpty[];

/* ---- 全局数据 (core_globals.c 定义) ---- */
extern uint32_t (*g_pfnRasEnumEntries)(void *, void *, void *, uint32_t *, uint32_t *); /* DAT_14013e270 */

/* 未来链接用占位 (避免 extern 悬空): 由本文件内 static 提供 */
static const uint8_t *g_dummy = NULL;

/* ========== FUN_1400E3F80 @0x1400e3e38 ==========
 * 创建/打开文件映射并映射视图:
 *   - bit29: name 视为"句柄输出"而非映射名
 *   - bit30/31 控制打开已存在映射时的策略 (0xb7 已存在错误码)
 *   - bit31 兼作 CREATE_ALWAYS 与 FILE_MAP_* 访问标志选择
 * TODO(verify): 标志位组合语义。
 */
LPVOID PECMD_MapSharedSection(HANDLE file, int64_t size, uint32_t flags,
                                             int64_t offset, LPCWSTR name)
{
    LPCWSTR outHandle = NULL;
    LPVOID view = NULL;
    HANDLE map;
    uint32_t access;
    DWORD lastErr;
    DWORD mapAccess;

    if ((flags >> 0x1d & 1) != 0) {
        outHandle = name;
        name = NULL;
    }
    access = flags & 0xc0000000;
    if (access != 0) {
        map = OpenFileMappingW(4, 0, name);
        if (map == 0) {
            if ((int32_t)access < 0)
                return NULL;
        }
        else {
            CloseHandle(map);
            if ((flags >> 0x1e & 1) != 0)
                return NULL;
        }
    }

    map = CreateFileMappingW(file, NULL, flags & 0x1fffffff,
                             (DWORD)((uint64_t)(size + offset) >> 0x20),
                             (DWORD)(size + offset), name);
    if (map != 0) {
        if (access == 0 ||
            ((lastErr = GetLastError(),
              (int32_t)access >= 0 || lastErr == 0xb7) &&
             (((flags >> 0x1e & 1) == 0 || lastErr != 0xb7)))) {
            mapAccess = ((flags & 4) | 8) >> 1;
            if ((flags & 0x1fffffff) == 8)
                mapAccess = 1;
            view = MapViewOfFile(map, mapAccess, (DWORD)((uint64_t)offset >> 0x20),
                                 (DWORD)offset, 0);
            GetLastError();
            if (outHandle == NULL) {
                CloseHandle(map);
                return view;
            }
            *(HANDLE *)outHandle = map;
            return view;
        }
        CloseHandle(map);
    }
    return NULL;
}

/* ========== FUN_1400E7840 @0x1400e7840 ==========
 * 枚举 RAS 电话簿条目, 生成 "条目名\r\n条目名..." 宽字符串。
 * 每次追加前扩容; 返回最终字符串指针。
 * TODO(verify): 条目结构 (stride 0x418, 名称偏移 +4)。
 */
int64_t FUN_1400E7840(int64_t *out, int flag)
{
    uint32_t *blk;
    uint32_t cb;
    uint32_t count;
    LPCWSTR first;
    uint16_t *dst;
    int64_t total;
    int i, len;

    cb = 0x418;
    count = 0;
    uint32_t *_Memory = (uint32_t *)operator_new(0x418);
    _Memory[0] = 0x418;
    g_pfnRasEnumEntries(NULL, NULL, _Memory, &cb, &count);
    free(_Memory);

    FUN_1400633A8((void **)&blk, (int64_t)cb);
    blk[0] = 0x418;
    g_pfnRasEnumEntries(NULL, NULL, blk, &cb, &count);

    first = (LPCWSTR)g_szEmpty;
    if (count != 0)
        first = (LPCWSTR)(uintptr_t)(blk + 1);
    PECMD_StrBldCopyWide((WCHAR **)out, first);
    total = lstrlenW((LPCWSTR)*out);

    if (flag != 0 && 1 < count) {
        i = 1;
        do {
            len = lstrlenW((LPCWSTR)(blk + (int64_t)i * 0x106 + 1));
            PECMD_AllocString((WCHAR **)out, (int64_t)(total + len + 3));
            dst = (uint16_t *)(*out + total * 2);
            dst[0] = 0xd;
            dst[1] = 10;
            memcpy((uint8_t *)(dst + 2),
                          (uint8_t *)(blk + (int64_t)i * 0x106 + 1),
                          (len + 1) * 2);
            i++;
            total = len + total + 2;
        } while ((int32_t)i < (int32_t)count);
    }
    int64_t r = *out;
    PECMD_FreeStrBuf((WCHAR **)&blk);
    return r;
}

/* ========== FUN_1400EC880 @0x1400ec880 ==========
 * 更新子窗口: 逐项广播 WM_0x133E, 再按数组顺序 SetWindowPos 调整 Z 序
 * (第二项起 SWP_0x97, 第一项 SWP_0x43), 最后刷新控件显示。
 */
void FUN_1400EC880(int64_t obj, char mode)
{
    int64_t *base;
    int64_t *scan;
    int64_t count;
    WPARAM wp;
    int64_t i;

    if (mode != 0)
        *(uint32_t *)((uint8_t *)obj + OBJ_LINK) |= 0x80;

    count = *(int64_t *)((uint8_t *)obj + 0x118);
    if (0 < count) {
        base = *(int64_t **)((uint8_t *)obj + 0x108);
        scan = base;
        wp = 0;
        i = 0;
        do {
            uint32_t local_48 = 1;
            uint64_t local_40 = 0;

            memset(&local_40, 0, 0x20);
            SendMessageW(*(HWND *)((uint8_t *)obj + OBJ_HWND), 0x133e, wp, (LPARAM)&local_48);
            i++;
            scan++;
            wp = (WPARAM)i;
        } while ((int64_t)wp < count);

        scan = base;
        i = 1;
        if (1 < count) {
            do {
                SetWindowPos(*(HWND *)(*(int64_t *)scan[1] + OBJ_HWND), 0, 0, 0, 0, 0, 0x97);
                i++;
                scan++;
            } while ((int64_t)i < count);
        }
        SetWindowPos(*(HWND *)(*(int64_t *)*base + OBJ_HWND), 0, 0, 0, 0, 0, 0x43);
        FUN_1400EC7C0(obj, mode);
    }
}

/* ========== FUN_1400EEF20 @0x1400eef20 ==========
 * 编辑/输入控件的按键消息处理:
 *   - WM_KEYDOWN Tab/Enter 或 WM_LBUTTONDOWN 向上层发 WM_0x45C 导航
 *   - 无 IM 模式下 Enter 后失效重绘
 *   - 最终经 0x233 查映射表, 命中返回映射值
 */
uint32_t FUN_1400EEF20(int64_t obj, int64_t pmsg)
{
    uint16_t r;
    uint64_t local_res8;
    HWND hparent;
    uint64_t wParam;
    LPARAM lParam;
    int msg;

    g_randState *= 0x13;
    msg = *(int *)(pmsg + 8);

    if (msg == 0x100) {
        if ((*(uint8_t *)((uint8_t *)obj + OBJ_FLAGS) & 1) != 0 ||
            (*(uint8_t *)((uint8_t *)obj + OBJ_FLAGS) & 6) != 0) {
            if (*(int64_t *)(pmsg + 0x10) == 9) {
                hparent = GetParent(*(HWND *)((uint8_t *)obj + OBJ_HWND));
                lParam = 6;
                wParam = 1;
            }
            else {
                if (*(int64_t *)(pmsg + 0x10) != 0xd)
                    goto dispatch;
                wParam = (uint64_t)(uint16_t)GetDlgCtrlID(*(HWND *)((uint8_t *)obj + OBJ_HWND));
                hparent = GetParent(*(HWND *)((uint8_t *)obj + OBJ_HWND));
                lParam = 8;
            }
            SendMessageW(hparent, 0x45c, wParam, lParam);
        }
    }
    else if (msg - 0x201U < 2) {
        if ((*(uint8_t *)((uint8_t *)obj + OBJ_FLAGS) & 1) != 0 ||
            (*(uint8_t *)((uint8_t *)obj + OBJ_FLAGS) & 6) != 0) {
            hparent = GetParent(*(HWND *)((uint8_t *)obj + OBJ_HWND));
            SendMessageW(hparent, 0x45c, 1, 6);
        }
        if ((*(uint8_t *)((uint8_t *)obj + OBJ_FLAGS) & 1) == 0 &&
            (*(uint8_t *)((uint8_t *)obj + OBJ_FLAGS) & 6) == 0 &&
            (*(int64_t *)((uint8_t *)obj + 0x110) != 0 ||
             *(int32_t *)((uint8_t *)obj + OBJ_COLOR) >= 0))
            InvalidateRect(*(HWND *)((uint8_t *)obj + OBJ_HWND), NULL, 1);
    }
dispatch:
    local_res8 = 0;
    r = FUN_1400F172C(*(int64_t **)((uint8_t *)obj + OBJ_LINK), 0x233,
                      *(uint64_t *)(pmsg + 0x10), *(uint64_t **)(pmsg + 0x18),
                      *(int64_t *)((uint8_t *)obj + OBJ_HWND), 1, &local_res8);
    return (uint32_t)(-(uint32_t)((r & 4) != 0) & (uint32_t)local_res8);
}

/* ========== FUN_1400EFD80 @0x1400efd80 ==========
 * 应用控件属性: 无参数时设置窗口文本并赋变量; "color" 时解析颜色值
 * 写入对象槽或关联窗口, 触发重绘。
 */
uint64_t FUN_1400EFD80(int64_t *obj, int64_t *ctx, LPCWSTR name,
                                 LPWSTR param, uint64_t a5, uint64_t a6,
                                 int64_t a7)
{
    LPCWSTR value = name;
    int parsed;
    int64_t win;
    HWND hwnd;

    if (*param == L'\0') {
        if (obj[0x22] == 0)
            return 0;
        SetWindowTextW(*(HWND *)(obj[0x22] + OBJ_HWND), name);
        if (*(LPCWSTR *)(a7 + 0x10) != NULL &&
            **(LPCWSTR *)(a7 + 0x10) != L'\0')
            FUN_14007D0AC((int64_t **)(a7 + 0x50),
                                 *(LPCWSTR *)(a7 + 0x10), value);
    }
    else {
        if (lstrcmpW(WSTR("color"), param) != 0)
            return FUN_1400C493C(obj, ctx, value, (uint16_t *)param,
                                 a5, a6, a7);

        parsed = -0x80000000;
        *param = L'\0';
        PECMD_ParseUIntValue((WCHAR **)&value, &parsed);
        win = obj[0x22];
        if ((*(uint8_t *)((uint8_t *)obj + OBJ_FLAGS) & 1) == 0 &&
            (*(uint8_t *)((uint8_t *)obj + OBJ_FLAGS) & 6) == 0) {
            FUN_1400EF91C((int64_t)obj, 0x10000000, 0);
            if (obj[0x22] == 0)
                return 0;
            *(int *)(obj[0x22] + 0xd8) = parsed;
            if (win != 0)
                return 0;
            hwnd = *(HWND *)(obj[0x22] + OBJ_HWND);
            goto inv;
        }
        *(int *)((uint8_t *)obj + 0xa8) = parsed;
    }
    hwnd = (HWND)obj[4];
inv:
    InvalidateRect(hwnd, NULL, 1);
    return 0;
}

/* ========== PECMD_FillCtlBackground @0x1400f0ca0 ==========
 * 用纯色填充客户区: 兼容 DC + 位图离屏绘制, 经柔化/混合后 BitBlt 上屏。
 * TODO(verify): PECMD_BltTransparentBits 的两个 CONCAT44 参数。
 */
void PECMD_FillCtlBackground(HWND hwnd, COLORREF color)
{
    RECT rc;
    int cx, cy;
    HDC hdc, hdcSrc;
    HBITMAP bmp;
    uint64_t sel[4];
    DWORD sysColor;

    if ((int32_t)color < 0)
        return;

    rc.left = 0;
    rc.top = 0;
    rc.right = 0;
    rc.bottom = 0;
    GetClientRect(hwnd, &rc);
    cx = rc.right - rc.left;
    cy = rc.bottom - rc.top;

    hdc = GetDC(hwnd);
    sysColor = GetSysColor(0xf);
    hdcSrc = CreateCompatibleDC(hdc);
    bmp = CreateCompatibleBitmap(hdc, cx, cy);
    PECMD_SelectObjectSlot_b028(sel, hdcSrc, bmp);
    FUN_1400E68E0(hdcSrc, &rc, color);
    PECMD_BltTransparentBits(hdcSrc, 0, 0, cx, cy, hdc, 0, 0,
                  (uint64_t)(uint32_t)cx, (uint64_t)(uint32_t)cy, sysColor);
    BitBlt(hdc, 0, 0, cx, cy, hdcSrc, 0, 0, 0xcc0020);
    PECMD_RestoreAndDeleteObject(sel);
    if (hdcSrc != 0)
        DeleteDC(hdcSrc);
    if (hdc != 0)
        ReleaseDC(hwnd, hdc);
}

/* ========== FUN_1400F1234 @0x1400f1234 ==========
 * 控件查询分发: 0x233 优先查映射表, 未命中走通用查询并通知上层
 * (WM_0x450/0x451 往返消息)。
 */
int64_t FUN_1400F1234(int64_t obj, uint32_t msg, int64_t wParam,
                                 uint64_t *lParam)
{
    uint16_t r;
    HWND hwnd;
    uint64_t local_res8;
    int64_t result;
    int count;

    count = *(int *)(*(int64_t *)((uint8_t *)obj + 0x34) + 0x10);
    if (count < 1)
        hwnd = 0;
    else
        hwnd = FUN_1400E5788(*(HWND *)((uint8_t *)obj + 8));

    if (hwnd != 0)
        SendMessageW(hwnd, 0x450, 4, 0x5aa555aa);

    if (0 < count) {
        local_res8 = 0;
        r = FUN_1400F172C(*(int64_t **)((uint8_t *)obj + 0x34), msg, wParam,
                          lParam, *(int64_t *)((uint8_t *)obj + 8), 1, &local_res8);
        result = (int64_t)local_res8;
        if ((r & 4) != 0) {
            if (hwnd == 0)
                return (int64_t)local_res8;
            PostMessageW(hwnd, 0x451, 0x204, -0x55aaa55b);
            return result;
        }
    }

    result = FUN_1400E5B0C(obj, (uint64_t)msg, wParam, (int64_t *)lParam);
    if (0 < count)
        FUN_1400F172C(*(int64_t **)((uint8_t *)obj + 0x34), msg, wParam,
                      lParam, *(int64_t *)((uint8_t *)obj + 8), 2, NULL);
    if (hwnd != 0)
        PostMessageW(hwnd, 0x451, 0x204, -0x55aaa55b);
    return result;
}

/* ========== FUN_1400F1F78 @0x1400f1f78 ==========
 * 控件按键消息: Enter 且开启风格时向父窗口发 WM_COMMAND (0x111),
 * 随后经 0x80 通用分发; 返回处理后消息码 (0x102 / 0x10102)。
 */
uint64_t FUN_1400F1F78(int64_t obj, int64_t pmsg)
{
    int id;
    HWND hparent;
    int msg;
    uint64_t r;

    g_randState *= 0x1f;
    msg = *(int *)(pmsg + 8);

    if (msg == 0x100 && *(int64_t *)(pmsg + 0x10) == 0xd &&
        (*(uint8_t *)((uint8_t *)obj + OBJ_LINK) & 1) != 0) {
        id = GetDlgCtrlID(*(HWND *)((uint8_t *)obj + OBJ_HWND));
        hparent = GetParent(*(HWND *)((uint8_t *)obj + OBJ_HWND));
        PostMessageW(hparent, 0x111, (uint64_t)(uint16_t)id, 0x100);
    }

    FUN_1400F172C(*(int64_t **)((uint8_t *)obj + 0xd8), msg,
                  *(uint64_t *)(pmsg + 0x10), *(uint64_t **)(pmsg + 0x18),
                  *(int64_t *)((uint8_t *)obj + OBJ_HWND), 0x80, NULL);

    r = 0x102;
    if (msg == 0x100 || msg == 0x101 || msg == 0x102 ||
        msg == 0x201 || msg == 0x202 || msg == 0x204 || msg == 0x205 ||
        msg == 0x207 || msg == 0x208 || msg == 0x200 || msg == 0x203 ||
        msg == 0x1000 || msg == 0x2a3 || msg == 0x2a1)
        r = 0x10102;
    return r;
}

/* ========== PECMD_IsClickOrKeyMsg @内部辅助 ==========
 * 判断消息是否属于点击/按键类 (供消息分发判定是否需要 0x80 通用下发)。
 */
static bool PECMD_IsClickOrKeyMsg(int msg)
{
    return msg == 0x100 || msg == 0x101 || msg == 0x102 ||
           msg == 0x201 || msg == 0x202 || msg == 0x204 || msg == 0x205 ||
           msg == 0x207 || msg == 0x208 || msg == 0x200 || msg == 0x203 ||
           msg == 0x1000 || msg == 0x2a3 || msg == 0x2a1;
}

/* ========== PECMD_HandleControlMsgImpl @内部辅助 ==========
 * 0x233 优先查映射; 未命中且为点击/按键类且焦点在控件时走 0x80 通用分发。
 */
static uint64_t PECMD_HandleControlMsgImpl(int64_t obj, int64_t pmsg, int64_t mapOff)
{
    uint16_t r;
    uint64_t local_res8;
    HWND focus;
    int msg;

    g_randState *= 0x1f;
    msg = *(int *)(pmsg + 8);
    focus = GetFocus();
    local_res8 = 0;

    r = FUN_1400F172C(*(int64_t **)((uint8_t *)obj + mapOff), 0x233,
                      *(uint64_t *)(pmsg + 0x10), *(uint64_t **)(pmsg + 0x18),
                      *(int64_t *)((uint8_t *)obj + OBJ_HWND), 1, &local_res8);
    if ((r & 4) == 0) {
        if (PECMD_IsClickOrKeyMsg(msg) && focus == *(HWND *)((uint8_t *)obj + OBJ_HWND))
            FUN_1400F172C(*(int64_t **)((uint8_t *)obj + mapOff), msg,
                          *(uint64_t *)(pmsg + 0x10), *(uint64_t **)(pmsg + 0x18),
                          (int64_t)*(HWND *)((uint8_t *)obj + OBJ_HWND), 0x80, NULL);
        local_res8 = 0;
    }
    else {
        local_res8 &= 0xffffffffU;
    }
    return local_res8;
}

/* ========== FUN_1400F2DD4 @0x1400f2dd4 ==========
 * 控件消息处理 A: 映射表位于对象 +1000 字节处。
 */
uint64_t FUN_1400F2DD4(int64_t obj, int64_t pmsg)
{
    return PECMD_HandleControlMsgImpl(obj, pmsg, 1000);
}

/* ========== FUN_1400FEF3C @0x1400fef3c ==========
 * 控件消息处理 B: 映射表位于对象 +0x108 字节处。
 */
uint64_t FUN_1400FEF3C(int64_t obj, int64_t pmsg)
{
    return PECMD_HandleControlMsgImpl(obj, pmsg, 0x108);
}

/* ========== PECMD_ListSubItemHitTest @0x1400f3308 ==========
 * 命中测试: 以光标位置发 WM_0x1039 (结构中 8 字节起始含坐标 + 4 长度),
 * 结果写索引/标志; 命中首行时再用 WM_0x100E 矩形复核。
 * TODO(verify): 结构字段与 0x1038 的用途。
 */
void PECMD_ListSubItemHitTest(int64_t obj, int *out_index, int *out_flag)
{
    typedef struct {
        int32_t x, y;
        int32_t cb;
        int32_t idx;
        int32_t flag;
    } HitTest;
    POINT pt;
    RECT rc;
    LRESULT lr;
    HitTest ht;

    GetCursorPos(&pt);
    ScreenToClient(*(HWND *)((uint8_t *)obj + OBJ_HWND), &pt);
    memset((uint64_t *)((uint8_t *)&ht + 4), 0, 0x14);
    ht.x = pt.x;
    ht.y = pt.y;
    ht.cb = 4;

    lr = SendMessageW(*(HWND *)((uint8_t *)obj + OBJ_HWND), 0x1039, 0, (LPARAM)&ht);
    if ((int)lr < 0) {
        *out_flag = (int)0xfffffffe;
        *out_index = -2;
    }
    else {
        *out_index = ht.idx;
        *out_flag = ht.flag;
        if (*out_index == 0 && (*(uint16_t *)((uint8_t *)obj + 0x3f8) & 0x100) != 0) {
            rc.left = 0;
            rc.top = 0;
            rc.right = 0;
            rc.bottom = 0;
            SendMessageW(*(HWND *)((uint8_t *)obj + OBJ_HWND), 0x100e, 0, (LPARAM)&rc);
            if (PtInRect(&rc, pt) == 0) {
                *out_flag = (int)0xfffffffd;
                *out_index = -3;
            }
            else {
                uint32_t local_38 = 0;
                SendMessageW(*(HWND *)((uint8_t *)obj + OBJ_HWND), 0x1038,
                             (int64_t)*out_index, (LPARAM)&local_38);
                if (PtInRect(&rc, pt) == 0)
                    *out_flag = (int)0xfffffffd;
            }
        }
    }
}

/* ========== FUN_1400F4D1C @0x1400f4d1c ==========
 * 写入"映射对"条目 (key1/key2): 不存在则建槽并初始化节点,
 * 设置文本与各参数字段; mode < -1 且无文本时删除条目。
 */
void FUN_1400F4D1C(int64_t obj, int64_t key1, int64_t key2, int mode,
                           uint32_t v5, uint32_t v6, uint32_t v7,
                           uint32_t v8, LPCWSTR text)
{
    int64_t *slot;
    int64_t count;
    uint64_t *node;

    slot = FUN_1400F4CA0(obj, key1, key2);
    if (mode < -1 && text == NULL) {
        if (slot != NULL) {
            node = (uint64_t *)*slot;
            *slot = 0;
            if (node != NULL) {
                PECMD_FreeStrBuf((WCHAR **)node);
                free(node);
            }
        }
    }
    else {
        if (slot == NULL) {
            count = *(int64_t *)((uint8_t *)obj + 0x360);
            *(int64_t *)((uint8_t *)obj + 0x360) = count + 1;
            slot = (int64_t *)FUN_140063B00(count, (int64_t *)((uint8_t *)obj + 0x350),
                                              (int64_t *)((uint8_t *)obj + 0x358), 8);
        }
        if (*slot == 0) {
            node = (uint64_t *)operator_new(0x30);
            if (node != NULL) {
                node[0] = 0;
                node[1] = 0xffffffffffffffff;
                node[2] = 0xffffffffffffffff;
                *(uint32_t *)((uint8_t *)node + 0x1c) = 0;
                *(uint32_t *)((uint8_t *)node + 0x28) = 0x80000000;
                *(uint32_t *)((uint8_t *)node + 0x24) = 0x80000000;
                *(uint32_t *)((uint8_t *)node + 0x20) = 0x80000000;
            }
            *slot = (int64_t)(uintptr_t)node;
        }
        slot = (int64_t *)*slot;
        if (text == NULL) {
            if (*slot != 0) {
                FUN_14005B0D4(slot);
                *slot = 0;
            }
        }
        else {
            PECMD_StrBldCopyWide((WCHAR **)slot, text);
        }
        slot[1] = key1;
        slot[2] = key2;
        *(uint32_t *)((uint8_t *)slot + 0x1c) = v8;
        *(int *)(slot + 3) = mode;
        *(uint32_t *)((uint8_t *)slot + 0x20) = v5;
        *(uint32_t *)((uint8_t *)slot + 0x24) = v6;
        *(uint32_t *)((uint8_t *)slot + 0x28) = v7;
    }
}

/* ========== FUN_1400F60A4 @0x1400f60a4 ==========
 * 写入"数组项"条目 (按 value 定位槽): 逻辑同 FUN_1400F4D1C,
 * 但数组位于 +0x338/+0x340/+0x348。
 */
void FUN_1400F60A4(int64_t obj, int64_t value, int mode,
                             uint32_t v4, uint32_t v5, uint32_t v6,
                             uint32_t v7, LPCWSTR text)
{
    int64_t *slot;
    int64_t count;
    uint64_t *node;

    slot = FUN_1400F4C28(obj, value);
    if (mode < -1 && text == NULL) {
        if (slot != NULL) {
            node = (uint64_t *)*slot;
            *slot = 0;
            if (node != NULL) {
                PECMD_FreeStrBuf((WCHAR **)node);
                free(node);
            }
        }
    }
    else {
        if (slot == NULL) {
            count = *(int64_t *)((uint8_t *)obj + 0x348);
            *(int64_t *)((uint8_t *)obj + 0x348) = count + 1;
            slot = (int64_t *)FUN_140063B00(count, (int64_t *)((uint8_t *)obj + 0x338),
                                              (int64_t *)((uint8_t *)obj + 0x340), 8);
        }
        if (*slot == 0) {
            node = (uint64_t *)operator_new(0x30);
            if (node != NULL) {
                node[0] = 0;
                node[1] = 0xffffffffffffffff;
                node[2] = 0xffffffffffffffff;
                *(uint32_t *)((uint8_t *)node + 0x1c) = 0;
                *(uint32_t *)((uint8_t *)node + 0x28) = 0x80000000;
                *(uint32_t *)((uint8_t *)node + 0x24) = 0x80000000;
                *(uint32_t *)((uint8_t *)node + 0x20) = 0x80000000;
            }
            *slot = (int64_t)(uintptr_t)node;
        }
        slot = (int64_t *)*slot;
        if (text == NULL) {
            if (*slot != 0) {
                FUN_14005B0D4(slot);
                *slot = 0;
            }
        }
        else {
            PECMD_StrBldCopyWide((WCHAR **)slot, text);
        }
        slot[1] = value;
        *(int *)(slot + 3) = mode;
        *(uint32_t *)((uint8_t *)slot + 0x1c) = v7;
        *(uint32_t *)((uint8_t *)slot + 0x20) = v4;
        *(uint32_t *)((uint8_t *)slot + 0x24) = v5;
        *(uint32_t *)((uint8_t *)slot + 0x28) = v6;
    }
}

/* ========== FUN_1400F8FCC @0x1400f8fcc ==========
 * 按 ID 查找项并设置值/文本:
 *   - 未找到且 value 非空: 新建节点追加到 +0x2f0 数组
 *   - 找到: value 为空则删除项, 否则更新值与文本
 */
void FUN_1400F8FCC(int64_t obj, int id, int64_t value, LPCWSTR text)
{
    int64_t idx;
    int64_t *slot;
    void *m;

    idx = PECMD_ItemPropFindIdxNamed(obj, id, NULL, NULL);
    if (idx < 0) {
        if (value != 0) {
            int64_t *node = (int64_t *)operator_new(0x18);
            if (node == NULL)
                return;
            node[4] = 0;
            node[5] = 0;
            node[0] = id;
            *(int64_t *)((uint8_t *)node + 8) = value;
            if (text != NULL)
                PECMD_StrBldCopyWide((WCHAR **)(node + 4), text);
            FUN_1400639F0((int64_t *)((uint8_t *)obj + 0x2f0),
                                     (int64_t *)((uint8_t *)obj + 0x2f8),
                                     (int64_t *)((uint8_t *)obj + 0x300),
                                     (uint8_t *)&node, 8, 1);
        }
    }
    else {
        slot = (int64_t *)FUN_140063B00(idx, (int64_t *)((uint8_t *)obj + 0x2f0),
                                          (int64_t *)((uint8_t *)obj + 0x2f8), 8);
        m = (void *)*slot;
        if (m != NULL) {
            if (value == 0) {
                *slot = 0;
                PECMD_FreeStrBuf((WCHAR **)((uint8_t *)m + 0x10));
                FUN_1400F2B6C((int64_t)(intptr_t)m);
                free(m);
            }
            else {
                *(int64_t *)((uint8_t *)m + 8) = value;
                *(int *)*slot = id;
                if (text == NULL)
                    FUN_14005B0D4((int64_t *)(*slot + 0x10));
                else
                    PECMD_StrBldCopyWide((WCHAR **)(*slot + 0x10), text);
            }
        }
    }
}

/* ========== PECMD_BuildTreeIndexPathStr @0x1400ff2bc ==========
 * 遍历上行/下行范围 (WM_0x110A), 生成 "起始号." 序列串拼接进 *out。
 * 首项前补空格; 返回最终串长度。
 * TODO(verify): wsprintfW 缺实参 (原始反编译) — 补 0 占位。
 */
int64_t PECMD_BuildTreeIndexPathStr(int64_t obj, LRESULT first, int64_t *out)
{
    uint8_t *dst;
    uint64_t local_res10;
    WCHAR buf[64];
    int64_t off;
    int64_t written;
    bool firstFlag;
    int len;

    off = out[1];
    written = 0;
    firstFlag = off != 0;
    local_res10 = 0xffffffffffffffff;

    if (first != 0) {
        do {
            FUN_1400FED38(obj, (uint64_t)first, &local_res10);
            first = SendMessageW(*(HWND *)((uint8_t *)obj + OBJ_HWND), 0x110a, 3, first);
            len = wsprintfW(buf, WSTR("%d."), 0);          /* TODO(verify) 缺实参 */
            PECMD_StrAlloc((WCHAR **)out, (size_t)(out[1] + 6 + len));
            if (firstFlag) {
                off = out[1];
                out[1] = off + 1;
                *(uint16_t *)(*out + off * 2) = 0x20;      /* 空格 */
                off = out[1];
                firstFlag = false;
            }
            dst = (uint8_t *)(*out + off * 2);
            if (written != 0)
                memmove(dst + len * 2, dst, (int)written * 2 + 2);
            memmove(dst, buf, ((uint32_t)(written == 0) + len) * 2);
            out[1] += len;
            written += len;
        } while (first != 0);
        if (0 < written)
            out[1]--;
    }
    *(uint16_t *)(*out + out[1] * 2) = 0;
    return out[1];
}
