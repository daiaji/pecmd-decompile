/* ====================================================================
 * core_b8l.c — B8 中批 3 (1400e0000-1400fffff), 后半
 *
 * 本批新实现函数全部使用人类可读 PECMD_ 名称，原始地址保留在 @0x 注释。
 *
 * 来源: PECMD原始.EXE (x64, ImageBase=0x140000000)
 *   发送控件命令串        FUN_1400F1504    @0x1400f1504
 *   调整控件项度量         PECMD_ListGetItemTextData        @0x1400f2f58
 *   设置控件项数据         PECMD_ListAddItemEntry             @0x1400f53c8
 *   初始化列表视窗对象     FUN_1400F9134            @0x1400f9134
 *   销毁列表视窗对象       PECMD_DestroyCtlResources         @0x1400f9324
 *   控件消息分发(临界区)   FUN_1400FC148       @0x1400fc148
 *   设置控件颜色           FUN_1400FD014           @0x1400fd014
 *   取控件画刷             PECMD_CtlCalcColorBrush            @0x1400fd35c
 *   捕获父窗口背景         PECMD_CaptureParentBackground    @0x1400fd5e8
 *   控件悬停定时器         PECMD_ControlHoverTimer   @0x1400fd86c
 *   控件鼠标消息           FUN_1400FDEDC     @0x1400fdedc
 *   树项路径查找           PECMD_FindTreeItemByPath          @0x1400ff414
 *   树路径串构建           FUN_1400FF5D0        @0x1400ff5d0
 *   树路径串构建(扩展)     PECMD_TreeCollectItems      @0x1400ff730
 *   树路径串构建(扩展2)    FUN_1400FF8A8     @0x1400ff8a8
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

/* ---- 缺失平台声明/类型 (本文件局部) ---- */
extern void *operator_new(size_t size);
extern uint8_t PTR_FUN_14012c670[];

/* ---- 已实现公共工具 (pecmd_defs.h / core_*.c) ---- */
extern bool PECMD_ParseUIntValue(WCHAR **pp, int *out);             /* @0x140067d20 */
extern int FUN_140067B78(WCHAR **pp, uint64_t *out);    /* @0x140067b78 */
extern WCHAR **FUN_14005B154(WCHAR **pp);                   /* @0x14005b154 */
extern int64_t FUN_140063B00(int64_t idx, int64_t *arr, int64_t *cap,
                               uint32_t esize);               /* @0x140063b00 */
extern int64_t *FUN_140063B64(int64_t *arr);                /* @0x140063b64 */
extern void FUN_1400633A8(void **ps, int64_t len);             /* @0x1400633a8 */
extern void *FUN_1400E57C0(void *obj);                 /* @0x1400e57c0 */
extern uint64_t *FUN_1400F5C74(uint64_t *obj);        /* @0x1400f5c74 */
extern void FUN_1400E8940(void *obj);               /* @0x1400e8940 */
extern void FUN_1400F425C(uint64_t *arr);               /* @0x1400f425c */
extern void FUN_1400F4208(uint64_t *arr);            /* @0x1400f4208 */
extern void FUN_1400F5C10(int64_t *array);         /* @0x1400f5c10 */
extern void FUN_1400F5D50(uint64_t *obj);   /* @0x1400f5d50 */
extern HWND FUN_1400E5788(HWND hwnd);             /* @0x1400e5788 */
extern void FUN_1400FD538(HWND hwnd, int mode); /* @0x1400fd538 */
extern void PECMD_SetHotTrackWindow(HWND hwnd);           /* @0x1400f1448 */
extern int FUN_1400FEDA4(int64_t obj, uint64_t param2,
                                           uint64_t *out, uint32_t param4); /* @0x1400feda4 */
extern int FUN_1400FEE24(int64_t obj, uint64_t param2,
                                     uint64_t *out);         /* @0x1400fee24 */
extern int64_t PECMD_BuildTreeIndexPathStr(int64_t obj, LRESULT first, int64_t *out); /* @0x1400ff2bc */

/* ---- 未实现依赖 (extern + TODO(verify)) ---- */
extern void PECMD_AllocStrSlot(void *ps);
extern void FUN_14007BF44(int64_t *ctx, WCHAR *name, void *out, int mode,
                          uint8_t flag);
extern void PECMD_ParseSkipSeparator(int64_t *pp, int64_t *out, int16_t sep1,
                          int16_t sep2);
extern int64_t PECMD_ParseTokenResolve(int64_t *ctx, WCHAR *p, uint64_t *out1, uint64_t *out2,
                             bool async, uint64_t *out3);
extern void PECMD_VarSetUInt(void *s, uint64_t v, LPCWSTR k);
extern void PECMD_FreeArray_ddf8(int64_t *arr);
extern void PECMD_SelectObjectSlot_b028(uint64_t *slot, HDC hdc, HGDIOBJ obj);
extern void PECMD_RestoreAndDeleteObject(uint64_t *slot);
extern uint16_t FUN_1400F172C(int64_t *map, int msg, uint64_t wParam, uint64_t *lParam,
                              int64_t hwnd, uint8_t mode, uint64_t *out);
extern int64_t FUN_1400E5B0C(int64_t obj, uint64_t p2, int64_t p3, int64_t *p4);

/* ---- 全局 (core_globals.c) ---- */
extern HGDIOBJ g_hStockWhiteBrush;  /* DAT_14013a858 库存白色画刷 */
extern HWND g_hActiveDevWnd;        /* DAT_14013e400 当前设备窗口 */
extern uint8_t g_tooltipThreshold;  /* DAT_14013a861 Tooltip 触发阈值 */
extern uint8_t g_tooltipCount0;     /* DAT_14013a860 Tooltip 计数 0 */

/* ========== FUN_1400F1504 @0x1400f1504 ==========
 * 解析 "[:var;] [#]msg ..." 控件命令串，展开变量后向控件窗口发送
 * PostMessage/SendMessage(Timeout)，并把结果写回命名变量。
 * TODO(verify): PECMD_ParseTokenResolve 参数/返回值语义。
 */
DWORD FUN_1400F1504(uint64_t unused, WCHAR *cmd, int64_t obj,
                                     int64_t *script, uint8_t sync, void **ppcs)
{
    WCHAR *p = cmd;
    WCHAR *expanded = NULL;
    WCHAR *key = NULL;
    uint64_t wParam = 0;
    uint64_t lParam = 0;
    uint64_t timeout[2] = {0, 0};
    DWORD postResult[2] = {0, 0};
    DWORD result = 1;
    HWND hwnd;
    int parsed = 0;
    uint32_t msg = 0;
    int extra = 0;

    (void)unused;
    PECMD_AllocStrSlot(&expanded);
    FUN_14007BF44(script, p, &expanded, 0, 1);
    p = expanded;
    PECMD_AllocStrSlot(&key);
    if (*p == L':') {
        p++;
        PECMD_ParseSkipSeparator((int64_t *)&p, (int64_t *)&key, 0x3b, 0x2c);
    }
    FUN_14005B154(&p);
    if (*p == L'#') {
        p++;
        extra = 0x5000;
    }
    if (PECMD_ParseUIntValue(&p, &parsed) && parsed > 0)
        msg = (uint32_t)parsed + (uint32_t)extra;
    if (msg != 0) {
        if (PECMD_ParseTokenResolve(script, p, &wParam, &lParam, sync != 0, timeout) == 0) {
            hwnd = *(HWND *)(obj + OBJ_HWND);
            if (ppcs != NULL && *ppcs != NULL) {
                LPCRITICAL_SECTION cs = (LPCRITICAL_SECTION)*ppcs;
                *ppcs = NULL;
                LeaveCriticalSection(cs);
            }
            if (sync == 0) {
                postResult[0] = PostMessageW(hwnd, msg, (WPARAM)wParam, (LPARAM)lParam);
                result = 0;
                if (postResult[0] != 1 && (result = GetLastError()) == 0)
                    result = 1;
            } else if (timeout[0] == 0) {
                postResult[0] = (DWORD)SendMessageW(hwnd, msg, (WPARAM)wParam, (LPARAM)lParam);
                result = postResult[0];
            } else {
                SendMessageTimeoutW(hwnd, msg, (WPARAM)wParam, (LPARAM)lParam, 2,
                                    (UINT)timeout[0], (DWORD *)postResult);
                result = postResult[0];
            }
            if (*key != L'\0')
                PECMD_VarSetUInt(script, postResult[0], key);
        } else {
            result = 0x80070057;
        }
    }
    FUN_14005B104(&key);
    FUN_14005B104(&expanded);
    return result;
}

/* ========== PECMD_ListGetItemTextData @0x1400f2f58 ==========
 * 通过 0x1038/0x1200/0x1211 消息测量控件各条目的布局高度/宽度，
 * 返回项数（失败返回 -1）。TODO(verify): 消息与控制类型。
 */
uint64_t PECMD_ListGetItemTextData(int64_t obj, int *rect, int msgParam)
{
    LRESULT lr;
    uint64_t count;
    uint64_t *items = NULL;
    uint64_t wParam;
    int n;
    int iVar2, iVar6;
    int iVar5 = 0;
    int64_t lVar7 = 0;
    int iVar2b = 0;
    int item;

    if (rect != NULL) {
        rect[1] = 0;
        rect[0] = 0;
    }
    lr = SendMessageW(*(HWND *)(obj + OBJ_HWND), 0x1038, (WPARAM)msgParam,
                      (LPARAM)(rect != NULL ? rect : NULL));
    if ((int)lr == 0)
        return (uint64_t)-1;

    iVar2 = rect[2] - rect[0];
    iVar6 = rect[3] - rect[1];
    if (iVar2 < rect[3] - rect[1])
        iVar6 = iVar2;
    rect[2] = rect[0] + iVar6;

    count = SendMessageW(*(HWND *)(obj + 0xf8), 0x1200, 0, 0);
    n = (int)count;
    wParam = (uint64_t)n;
    FUN_1400633A8((void **)&items, (int64_t)(n + 1) << 3);
    memset(items, 0, wParam << 2);
    SendMessageW(*(HWND *)(obj + 0xf8), 0x1211, (WPARAM)wParam, (LPARAM)items);

    if (0 < n) {
        do {
            int metric[4] = {0};
            iVar5 = iVar2b;
            item = ((int *)items)[lVar7];
            metric[0] = 2;
            metric[1] = item;
            lr = SendMessageW(*(HWND *)(obj + OBJ_HWND), 0x1038, (WPARAM)msgParam,
                              (LPARAM)metric);
            if ((int)lr != 0 && item == 0)
                break;
            lVar7++;
            iVar2b = metric[2] - rect[0];
        } while (lVar7 < (int64_t)wParam);
        if (iVar5 != 0)
            iVar5 += 2;
    }
    rect[0] += iVar5;
    rect[2] += iVar5;
    FUN_14005B104((WCHAR **)&items);
    return count & 0xffffffff;
}

/* ========== PECMD_ListAddItemEntry @0x1400f53c8 ==========
 * 把字符串/值写入对象的项数组并向控件发送 0x1061 消息。
 * TODO(verify): 0x1061 消息结构字段含义。
 */
void PECMD_ListAddItemEntry(int64_t obj, int index, LPCWSTR text, uint32_t data,
                          int param5, int param6, int64_t param7)
{
    WCHAR **slot;
    WCHAR **old;
    WCHAR **item;
    int64_t *valueSlot;
    int len;
    WCHAR ch[2];
    uint8_t info[0x40] = {0};
    uint32_t d = data;

    slot = (WCHAR **)(uintptr_t)FUN_140063B00(index, (int64_t *)(obj + 0x1a8),
                                                (int64_t *)(obj + 0x1b0), 8);
    old = (WCHAR **)*slot;
    if (d == 0)
        d = 0x30;
    item = (WCHAR **)operator_new(8);
    if (item != NULL)
        FUN_1400702B0(item, text);
    *slot = (WCHAR *)item;
    if (old != NULL) {
        FUN_14005B104(old);
        free(old);
    }

    len = lstrlenW(*(LPCWSTR *)(obj + 0x1d8));
    ch[0] = (WCHAR)d;
    ch[1] = 0;
    if (index < len) {
        *(WCHAR *)(*(int64_t *)(obj + 0x1d8) + (int64_t)len * 2) = ch[0];
    } else {
        FUN_14006375C((WCHAR **)(obj + 0x1d8), ch);
    }

    valueSlot = (int64_t *)(uintptr_t)FUN_140063B00(index, (int64_t *)(obj + 0x1c0),
                                                      (int64_t *)(obj + 0x1c8), 8);
    *valueSlot = param7;

    *(uint32_t *)(info + 0x00) = 5;
    if (d >> 8 != 0)
        d >>= 8;
    *(uint32_t *)(info + 0x04) = d & 0xf;
    if (param5 != -1) {
        *(uint32_t *)(info + 0x00) |= 2;
        *(uint32_t *)(info + 0x08) = (uint32_t)param5;
    }
    if (param6 != -1) {
        *(uint32_t *)(info + 0x00) |= 8;
        *(int *)(info + 0x18) = param6;
    }
    *(LPCWSTR *)(info + 0x10) = text;
    SendMessageW(*(HWND *)(obj + OBJ_HWND), 0x1061, (WPARAM)(int64_t)index, (LPARAM)info);
}

/* ========== FUN_1400F9134 @0x1400f9134 ==========
 * 初始化列表视窗内部对象（虚表 PTR_FUN_14012c670）。
 * TODO(verify): 各字段含义与默认值。
 */
uint64_t *FUN_1400F9134(uint64_t *obj, uint64_t param2, uint64_t param3)
{
    uint8_t *b = (uint8_t *)obj;

    FUN_1400E57C0(obj);
    obj[0] = (uint64_t)(uintptr_t)&PTR_FUN_14012c670;
    obj[0x1a] = param3;
    FUN_1400F5C74(obj + 0x1b);
    *(uint32_t *)(b + 0x218) = 0;
    FUN_140063B64((int64_t *)(obj + 0x4c));
    FUN_140063B64((int64_t *)(obj + 0x4f));
    FUN_140063B64((int64_t *)(obj + 0x52));
    FUN_140063B64((int64_t *)(obj + 0x55));
    FUN_140063B64((int64_t *)(obj + 0x58));
    FUN_140063B64((int64_t *)(obj + 0x5b));
    FUN_140063B64((int64_t *)(obj + 0x5e));
    FUN_140063B64((int64_t *)(obj + 0x61));
    FUN_140063B64((int64_t *)(obj + 0x64));
    FUN_140063B64((int64_t *)(obj + 0x67));
    FUN_140063B64((int64_t *)(obj + 0x6a));
    obj[0x6d] = 0;
    obj[0x6e] = 0;
    obj[0x6f] = 0;
    obj[0x70] = 0;
    obj[0x71] = 0;
    obj[0x72] = 0;
    obj[0x73] = 0;
    obj[0x74] = 0;
    obj[0x75] = 0;
    obj[0x7b] = 0x4028000000000000ULL;
    *(uint32_t *)(b + OBJ_FLAGS_3B4) = 0;
    *(uint32_t *)(b + 0x3bc) = 0xffffffffU;
    *(uint32_t *)(b + 0x3b8) = 0xffffffffU;
    *(uint32_t *)(b + 0x3d4) = 0xffffffe0U;
    *(uint32_t *)(b + 0x3d0) = 0xffffffe0U;
    *(uint32_t *)(b + 0x3cc) = 0xffffffe0U;
    obj[0x7c] = 0;
    *(uint16_t *)(b + 0x3f8) = 0;
    *(uint8_t *)(b + 0xa0) = 0;
    *(uint8_t *)(b + 0x250) = 0;
    *(uint32_t *)(b + 0x21c) = 0;
    obj[0x46] = 0;
    obj[0x45] = 0;
    obj[0x44] = 0;
    *(uint16_t *)(b + 0x3b0) = 0;
    *(uint8_t *)(b + 0x258) = 0;
    obj[0x49] = 0;
    obj[0x48] = 0;
    *(uint16_t *)(b + 0x23a) = 0;
    *(uint16_t *)(b + 0x238) = 0;
    *(uint32_t *)(b + 0x3c0) = 0xfffff000U;
    obj[0x7d] = param2;
    *(uint32_t *)(b + 0x3c8) = 0x80000000U;
    *(uint32_t *)(b + 0x3c4) = 0x00ffffffU;
    obj[0x42] = (uint64_t)(uintptr_t)obj;
    obj[0x7e] = 0xfffffffffffffff0ULL;
    *(uint32_t *)(b + 0x254) = 0x80000000U;
    return obj;
}

/* ========== PECMD_DestroyCtlResources @0x1400f9324 ==========
 * 销毁列表视窗内部对象：释放 GDI 对象、各数组、复合对象并调基类析构。
 */
void PECMD_DestroyCtlResources(uint64_t *obj)
{
    uint8_t *b = (uint8_t *)obj;

    obj[0] = (uint64_t)(uintptr_t)&PTR_FUN_14012c670;
    if ((HGDIOBJ)obj[0x49] != 0) {
        DeleteObject((HGDIOBJ)obj[0x49]);
        obj[0x49] = 0;
    }
    FUN_14005B104((WCHAR **)(obj + 0x73));
    FUN_14005B104((WCHAR **)(obj + 0x70));
    FUN_14005B104((WCHAR **)(obj + 0x6d));
    PECMD_FreeArray_ddf8((int64_t *)(obj + 0x6a));
    FUN_14005B104((WCHAR **)(obj + 0x6a));
    PECMD_FreeArray_ddf8((int64_t *)(obj + 0x67));
    FUN_14005B104((WCHAR **)(obj + 0x67));
    FUN_1400F425C(obj + 0x64);
    FUN_14005B104((WCHAR **)(obj + 0x64));
    FUN_1400F4208(obj + 0x61);
    FUN_14005B104((WCHAR **)(obj + 0x61));
    FUN_1400F5C10((int64_t *)(obj + 0x5e));
    FUN_14005B104((WCHAR **)(obj + 0x5e));
    FUN_1400F425C(obj + 0x5b);
    FUN_14005B104((WCHAR **)(obj + 0x5b));
    FUN_1400F425C(obj + 0x58);
    FUN_14005B104((WCHAR **)(obj + 0x58));
    FUN_1400F425C(obj + 0x55);
    FUN_14005B104((WCHAR **)(obj + 0x55));
    FUN_1400F425C(obj + 0x52);
    FUN_14005B104((WCHAR **)(obj + 0x52));
    FUN_1400F425C(obj + 0x4f);
    FUN_14005B104((WCHAR **)(obj + 0x4f));
    FUN_1400F425C(obj + 0x4c);
    FUN_14005B104((WCHAR **)(obj + 0x4c));
    FUN_1400F5D50(obj + 0x1b);
    FUN_1400E8940(obj);
    (void)b;
}

/* ========== FUN_1400FC148 @0x1400fc148 ==========
 * 控件消息分发变体：0x462 特殊路径进入临界区调用虚表回调，
 * 其余走 0x233 映射 + FUN_1400E5B0C 兜底。
 */
int64_t FUN_1400FC148(int64_t obj, uint32_t msg, uint64_t wParam,
                                   uint64_t *lParam)
{
    int count;
    uint16_t r;
    uint64_t out = 0;
    int64_t result = 0;
    HWND top;

    if (msg == 0x462 && (HDC)wParam == (HDC)obj) {
        EnterCriticalSection(&g_csInit);
        (*(void (**)(uint64_t, int, uint64_t))((uint64_t **)lParam[0])[4])(
            lParam[0], (int)lParam[1], lParam[2]);
        LeaveCriticalSection(&g_csInit);
        return 1;
    }

    count = *(int *)(*(int64_t *)(obj + 0x34) + 0x10);
    if (count < 1)
        top = 0;
    else
        top = FUN_1400E5788(*(HWND *)(obj + 8));
    if (top != 0)
        SendMessageW(top, 0x450, 4, 0x5aa555aa);

    if (0 < count) {
        out = 0;
        r = FUN_1400F172C(*(int64_t **)(obj + 0x34), (int)msg, (uint64_t)wParam,
                          lParam, *(int64_t *)(obj + 8), 1, &out);
        result = (int64_t)out;
        if ((r & 4) != 0) {
            if (top == 0)
                return (int64_t)out;
            PostMessageW(top, 0x451, 0x204, -0x55aaa55b);
            return result;
        }
    }
    result = FUN_1400E5B0C(obj, (uint64_t)msg, (int64_t)wParam, (int64_t *)lParam);
    if (0 < count)
        FUN_1400F172C(*(int64_t **)(obj + 0x34), (int)msg, (uint64_t)wParam,
                      lParam, *(int64_t *)(obj + 8), 2, NULL);
    if (top != 0)
        PostMessageW(top, 0x451, 0x204, -0x55aaa55b);
    return result;
}

/* ========== FUN_1400FD014 @0x1400fd014 ==========
 * 按 4 个颜色值设置控件前景/背景色槽，并重建画刷。
 * TODO(verify): 颜色槽交错写入方式。
 */
void FUN_1400FD014(int64_t obj, int *colors)
{
    uint32_t i = 0;
    COLORREF color;
    HBRUSH brush;

    if (colors == NULL)
        return;
    if (colors[0] < 0 && colors[1] < 0 && colors[2] < 0 && colors[3] < 0) {
        *(uint16_t *)(obj + 0xa2) = 0xffff;
        return;
    }
    do {
        uint32_t next = i + 1;
        int v = *colors++;
        int idx = (int)(((i & 1) ^ ((int)i >> 0x1f)) - ((int)i >> 0x1f) + ((int)i / 2) * 2);
        *(int *)(obj + ((int64_t)idx + 0x24) * 4) = v;
        i = next;
    } while ((int)i < 4);

    if (*(HGDIOBJ *)(obj + 0x80) != 0)
        DeleteObject(*(HGDIOBJ *)(obj + 0x80));
    *(uint64_t *)(obj + 0x80) = 0;
    if (*(HGDIOBJ *)(obj + 0x88) != 0)
        DeleteObject(*(HGDIOBJ *)(obj + 0x88));
    *(uint64_t *)(obj + 0x88) = 0;

    if (g_hStockWhiteBrush == (HGDIOBJ)0xffffffffffffffffULL)
        g_hStockWhiteBrush = (HGDIOBJ)GetStockObject(5);
    color = *(COLORREF *)(obj + 0x94);
    if ((int)color < 0 && color != 0x80000000)
        *(HGDIOBJ *)(obj + 0x80) = g_hStockWhiteBrush;
    if (*(int *)(obj + 0x9c) < 0 && *(int *)(obj + 0x9c) != INT32_MIN)
        *(HGDIOBJ *)(obj + 0x88) = g_hStockWhiteBrush;
    if (-1 < (int)color) {
        brush = CreateSolidBrush(color);
        *(HBRUSH *)(obj + 0x80) = brush;
    }
    if (-1 < *(int *)(obj + 0x9c)) {
        brush = CreateSolidBrush(*(COLORREF *)(obj + 0x9c));
        *(HBRUSH *)(obj + 0x88) = brush;
    }
    if (*(int16_t *)(obj + 0xa2) < 0)
        *(uint16_t *)(obj + 0xa2) = 0;
    if (*(int *)(obj + 0x9c) >= 0 &&
        *(int *)(obj + 0x9c) == *(int *)(obj + 0x94) &&
        *(int *)(obj + 0x98) == *(int *)(obj + 0x90) &&
        *(int8_t *)(obj + 0xac) > 0) {
        *(uint8_t *)(obj + 0xac) |= 0x80;
    }
}

/* ========== PECMD_CtlCalcColorBrush @0x1400fd35c ==========
 * 取当前索引对应的画刷；必要时按窗口状态合成前景色并补建画刷。
 */
HBRUSH PECMD_CtlCalcColorBrush(int64_t obj, HDC hdc)
{
    COLORREF c;
    HBRUSH brush;
    int16_t idx = *(int16_t *)(obj + 0xa2);
    BOOL enabled;

    if ((*(uint8_t *)(obj + 0xa4) & 7) != 0 ||
        (idx >= 0 && *(int64_t *)(obj + 0x80 + (int64_t)idx * 8) == (int64_t)g_hStockWhiteBrush)) {
        SetBkMode(hdc, 1);
    }
    if (idx < 0 || (*(uint8_t *)(obj + 0xa4) & 0x10) != 0)
        return *(HBRUSH *)(obj + OBJ_BRUSH);

    if (*(int64_t *)(obj + 0x80) == 0 && *(int *)(obj + 0x94) < 0) {
        c = GetBkColor(hdc);
        *(COLORREF *)(obj + 0x94) = c;
        if (*(HGDIOBJ *)(obj + 0x80) != 0)
            DeleteObject(*(HGDIOBJ *)(obj + 0x80));
        brush = CreateSolidBrush(c);
        *(HBRUSH *)(obj + 0x80) = brush;
    }
    if (*(int64_t *)(obj + 0x88) == 0 && *(int *)(obj + 0x9c) < 0) {
        c = GetBkColor(hdc);
        *(COLORREF *)(obj + 0x9c) = c;
        if (*(HGDIOBJ *)(obj + 0x88) != 0)
            DeleteObject(*(HGDIOBJ *)(obj + 0x88));
        brush = CreateSolidBrush(c);
        *(HBRUSH *)(obj + 0x88) = brush;
    }

    enabled = IsWindowEnabled(*(HWND *)(obj + OBJ_HWND));
    c = *(COLORREF *)(obj + 0x90 + (int64_t)idx * 8);
    if (-1 < (int)c) {
        if (!enabled) {
            DWORD sys = GetSysColor(0x16);
            c = (COLORREF)(((int)((c >> 8 & 0xff) + (sys >> 8 & 0xff)) >> 1) << 8 |
                           ((int)((sys >> 0x10 & 0xffU) + (c >> 0x10 & 0xffU)) >> 1) << 0x10 |
                           (int)((sys & 0xff) + (c & 0xff)) >> 1);
        }
        SetTextColor(hdc, c);
    }
    c = *(COLORREF *)(obj + 0x94 + (int64_t)idx * 8);
    if (-1 < (int)c)
        SetBkColor(hdc, c);
    return *(HBRUSH *)(obj + 0x80 + (int64_t)idx * 8);
}

/* ========== PECMD_CaptureParentBackground @0x1400fd5e8 ==========
 * 捕获父窗口中当前控件所在区域为位图，存入对象 +0xe8。
 */
void PECMD_CaptureParentBackground(int64_t obj)
{
    HWND child = *(HWND *)(obj + OBJ_HWND);
    HWND parent = GetParent(child);
    RECT rc;
    POINT pt;
    HDC hdc;
    HDC hdcMem;
    HBITMAP bmp;
    uint64_t slot[4] = {0};
    int cx, cy;

    if (parent == 0)
        return;
    if (*(HGDIOBJ *)(obj + OBJ_BITMAP) != 0)
        DeleteObject(*(HGDIOBJ *)(obj + OBJ_BITMAP));
    *(uint64_t *)(obj + OBJ_BITMAP) = 0;

    rc.left = rc.top = rc.right = rc.bottom = 0;
    GetWindowRect(child, &rc);
    pt.x = 0;
    pt.y = 0;
    ClientToScreen(parent, &pt);
    cx = rc.right - rc.left;
    cy = rc.bottom - rc.top;
    OffsetRect(&rc, -pt.x, -pt.y);

    hdc = GetDC(parent);
    hdcMem = CreateCompatibleDC(hdc);
    bmp = CreateCompatibleBitmap(hdc, cx, cy);
    *(HBITMAP *)(obj + OBJ_BITMAP) = bmp;
    PECMD_SelectObjectSlot_b028(slot, hdcMem, (HGDIOBJ)bmp);
    slot[0] = 0;
    BitBlt(hdcMem, 0, 0, cx, cy, hdc, rc.left, rc.top, 0xcc0020);
    PECMD_RestoreAndDeleteObject(slot);
    if (hdcMem != 0)
        DeleteDC(hdcMem);
    if (hdc != 0)
        ReleaseDC(parent, hdc);
}

/* ========== PECMD_ControlHoverTimer @0x1400fd86c ==========
 * 控件悬停/按下定时器 (0x2713/0x2711)：根据光标是否在窗口内调整
 * 状态字节并触发失效/通知。
 */
void PECMD_ControlHoverTimer(int64_t *obj, uint32_t timerId)
{
    HWND hwnd = (HWND)obj[4];
    POINT pt;
    RECT rc;
    BOOL inside;

    if (timerId == 0x2713) {
        GetCursorPos(&pt);
        rc.left = rc.top = rc.right = rc.bottom = 0;
        GetWindowRect(hwnd, &rc);
        inside = PtInRect(&rc, pt);
        if (!inside) {
            if (*(int8_t *)((uint8_t *)obj + 0xad) < 1) {
                int8_t c = *(int8_t *)((uint8_t *)obj + 0xad) - 1;
                *(int8_t *)((uint8_t *)obj + 0xad) = c;
                if (c < -100) {
                    *(uint8_t *)((uint8_t *)obj + 0xad) = 0;
                    KillTimer(hwnd, 0x2713);
                }
                goto done;
            }
            *(uint8_t *)((uint8_t *)obj + 0xad) = 0;
        } else {
            if (((GetAsyncKeyState(1) & 0x8000) == 0) ||
                *(int8_t *)((uint8_t *)obj + 0xad) > 0)
                goto done;
            *(uint8_t *)((uint8_t *)obj + 0xad) = 1;
        }
        if (*(int8_t *)((uint8_t *)obj + 0xac) > 0)
            InvalidateRect(hwnd, NULL, 1);
    } else if (timerId == 0x2711 && *(int8_t *)((uint8_t *)obj + 0xa0) > 0) {
        GetCursorPos(&pt);
        rc.left = rc.top = rc.right = rc.bottom = 0;
        GetWindowRect(hwnd, &rc);
        inside = PtInRect(&rc, pt);
        if (!inside) {
            KillTimer(hwnd, 0x2711);
            *(uint8_t *)((uint8_t *)obj + 0xa0) = 0;
            if (*(int16_t *)((uint8_t *)obj + 0xa2) >= 0) {
                *(uint16_t *)((uint8_t *)obj + 0xa2) = 0;
                if (*(int8_t *)((uint8_t *)obj + 0xac) > 0)
                    *(uint8_t *)((uint8_t *)obj + 0xa5) =
                        *(uint8_t *)((uint8_t *)obj + 0xa4) & 2;
                if (*(int8_t *)((uint8_t *)obj + 0xa1) != 0 ||
                    *(int8_t *)((uint8_t *)obj + 0xac) > 0)
                    InvalidateRect(hwnd, NULL, 1);
            }
            g_hActiveDevWnd = 0;
            PostMessageW(hwnd, 0x2a3, 0, 0);
            if ((uint64_t)obj[0x10] == (uint64_t)g_hStockWhiteBrush)
                FUN_1400FD538(hwnd, 1);
        } else {
            if (g_tooltipThreshold == (int8_t)obj[0x14])
                PECMD_SetHotTrackWindow(hwnd);
            if ((int8_t)obj[0x14] <= g_tooltipCount0)
                *(int8_t *)((uint8_t *)obj + 0xa0) = (int8_t)obj[0x14] + 1;
            if (g_tooltipCount0 == (int8_t)obj[0x14])
                PostMessageW(hwnd, 0x2a1, 0, 0);
        }
    }
done:
    if ((int8_t)obj[0x17] < 0) {
        (*(void (**)(uint64_t, uint32_t, uint32_t, uint64_t))((uint64_t **)obj[0])[1])(
            obj[4], 0x113, timerId, 0);
    }
}

/* ========== FUN_1400FDEDC @0x1400fdedc ==========
 * 处理控件鼠标消息结构：按下/移动/抬起时维护悬停状态、通知父窗口，
 * 最后走对象消息映射。
 */
uint64_t FUN_1400FDEDC(int64_t obj, int64_t *msg)
{
    int msgId = *(int *)((uint8_t *)msg + 8);
    uint64_t wParam = *(uint64_t *)((uint8_t *)msg + 0x10);
    uint64_t lParam = *(uint64_t *)((uint8_t *)msg + 0x18);
    HWND hwnd = *(HWND *)(obj + OBJ_HWND);
    POINT pt;
    RECT rc;
    uint16_t x = (uint16_t)lParam;
    uint16_t y = (uint16_t)(lParam >> 16);

    g_randState = g_randState * 0xd;
    if (*(int8_t *)(obj + OBJ_LINK) == 0 || (*(uint8_t *)(obj + 0xd1) & 0x40) != 0)
        goto dispatch;

    if (msgId == 0x202) {
        int id;
        HWND parent;
        *(uint8_t *)(obj + 0xad) = 0;
        InvalidateRect(hwnd, NULL, 1);
        GetDlgCtrlID(hwnd);
        id = GetDlgCtrlID(hwnd);
        parent = GetParent(hwnd);
        SendMessageW(parent, 0x441, (WPARAM)(uint64_t)(uint16_t)id | 0x2020000, 0x202);
    }

    if (msgId == 0x200) {
        if (*(int8_t *)(obj + 0xad) > 0) {
            if ((wParam & 1) != 0) {
                if (*(int8_t *)(obj + 0xad) < 1)
                    goto label_b2;
                goto label_bb;
            }
            goto label_1c;
        }
label_b2:
        if ((wParam & 1) == 0) {
label_bb:
            if (*(int8_t *)(obj + OBJ_LINK) == 2) {
                GetCursorPos(&pt);
                GetWindowRect(hwnd, &rc);
            } else {
                pt.x = x;
                pt.y = y;
                GetClientRect(hwnd, &rc);
            }
            if (PtInRect(&rc, pt) != 0)
                goto dispatch;
            goto label_1c;
        }
    } else {
label_1c:
        if (msgId != 0x201)
            goto dispatch;
        if (*(int8_t *)(obj + 0xad) > 0) {
            *(uint8_t *)(obj + 0xad) = 0;
            InvalidateRect(hwnd, NULL, 1);
        }
        if (*(int8_t *)(obj + OBJ_LINK) == 2) {
            GetCursorPos(&pt);
            GetWindowRect(hwnd, &rc);
        } else {
            pt.x = x;
            pt.y = y;
            GetClientRect(hwnd, &rc);
        }
        if (PtInRect(&rc, pt) == 0)
            goto dispatch;
    }
    SetTimer(hwnd, 0x2713, 10, NULL);
    *(uint8_t *)(obj + 0xad) = 1;
    InvalidateRect(hwnd, NULL, 1);

dispatch:
    FUN_1400F172C(*(int64_t **)(obj + 0xd8), msgId, wParam,
                  (uint64_t *)lParam, (int64_t)hwnd, 0x80, NULL);
    return 0;
}

/* ========== PECMD_FindTreeItemByPath @0x1400ff414 ==========
 * 按路径数字串（"1.2.3" 或 "@n"）在树控件中查找 HTREEITEM。
 */
uint64_t PECMD_FindTreeItemByPath(int64_t obj, WCHAR *path, uint64_t *out)
{
    WCHAR *p = path;
    uint64_t hItem = 0;
    uint64_t child;
    uint64_t n;
    uint64_t localOut = 0;
    uint64_t *dst;
    uint16_t first;
    uint16_t c;
    HWND hwnd = *(HWND *)(obj + OBJ_HWND);

    if (path == NULL)
        return 0;
    hItem = SendMessageW(hwnd, 0x110a, 0, 0);
    dst = out != NULL ? out : &localOut;
    first = *p;
    if (first == 0x40)
        p++;
    *dst = 0;
    c = *p;
    while (c != 0) {
        n = (uint64_t)-1;
        FUN_140067B78(&p, &n);
        c = *p;
        if (c != 0)
            p++;
        if (first == 0x40)
            return n;
        if ((int64_t)n < 1) {
            hItem = 0;
            break;
        }
        while ((int64_t)(n = n - 1) > 0) {
            child = SendMessageW(hwnd, 0x110a, 1, hItem);
            if (child == 0) {
                hItem = 0;
                break;
            }
            *dst = hItem;
            hItem = child;
        }
        if (hItem == 0)
            break;
        if (c == 0)
            return hItem;
        if ((8 < c) && (c < 0xe))
            return hItem;
        if (c == 0x20)
            return hItem;
        hItem = SendMessageW(hwnd, 0x110a, 4, hItem);
        if (hItem == 0)
            break;
        c = *p;
    }
    return hItem;
}

/* ========== FUN_1400FF5D0 @0x1400ff5d0 ==========
 * 递归把树项路径格式化为 "@0x%p" / 数字范围串，结果追加到 *out。
 */
int64_t FUN_1400FF5D0(int64_t obj, int64_t *out, uint64_t hItem,
                                  int64_t mode)
{
    uint64_t value = 0;
    int ok;
    int64_t off;
    int written;
    HWND hwnd = *(HWND *)(obj + OBJ_HWND);

    for (; hItem != 0; hItem = SendMessageW(hwnd, 0x110a, 1, hItem)) {
        ok = FUN_1400FEDA4(obj, hItem, &value, 2);
        value = (uint64_t)-(int64_t)(ok != 0) & value;
        if ((value & 2) != 0) {
            if (mode == 0) {
                PECMD_BuildTreeIndexPathStr(obj, hItem, out);
            } else {
                PECMD_AllocString(out, out[1] + 0x3f);
                off = out[1];
                if (off != 0) {
                    out[1] = off + 1;
                    *(uint16_t *)(*out + off * 2) = 0x20;
                }
                written = wsprintfW((LPWSTR)(*out + out[1] * 2), WSTR("@0x%p"),
                                    (void *)(uintptr_t)hItem);
                out[1] += written;
            }
        }
        {
            uint64_t next = SendMessageW(hwnd, 0x110a, 4, hItem);
            if (next != 0)
                FUN_1400FF5D0(obj, out, next, mode);
        }
    }
    return out[1];
}

/* ========== PECMD_TreeCollectItems @0x1400ff730 ==========
 * 树路径串构建扩展：用 0x1127 判断项类型，支持 stop 标志。
 */
int64_t PECMD_TreeCollectItems(int64_t obj, int64_t *out, WPARAM hItem,
                                    uint64_t flags)
{
    int64_t startLen = out[1];
    uint32_t stopFlag = ~(uint32_t)(flags >> 1) & 1;
    LRESULT lr;
    int64_t off;
    int written;
    HWND hwnd = *(HWND *)(obj + OBJ_HWND);

    for (; hItem != 0; hItem = SendMessageW(hwnd, 0x110a, 1, hItem)) {
        lr = SendMessageW(hwnd, 0x1127, hItem, 0xf000);
        if ((uint32_t)lr >> 0xc != 1) {
            if ((flags & 0x10) == 0) {
                PECMD_BuildTreeIndexPathStr(obj, hItem, out);
            } else {
                PECMD_AllocString(out, out[1] + 0x3f);
                off = out[1];
                if (off != 0) {
                    out[1] = off + 1;
                    *(uint16_t *)(*out + off * 2) = 0x20;
                }
                written = wsprintfW((LPWSTR)(*out + out[1] * 2), WSTR("@0x%p"),
                                    (void *)(uintptr_t)hItem);
                out[1] += written;
            }
            if (stopFlag != 0)
                break;
        }
        {
            WPARAM next = SendMessageW(hwnd, 0x110a, 4, hItem);
            if (next != 0) {
                int64_t r = PECMD_TreeCollectItems(obj, out, next, flags);
                if (stopFlag != 0 && r > 0)
                    break;
            }
        }
    }
    return out[1] - startLen;
}

/* ========== FUN_1400FF8A8 @0x1400ff8a8 ==========
 * 树路径串构建扩展 2：用 FUN_1400FEE24 判断项并递归到 Ex。
 */
int64_t FUN_1400FF8A8(int64_t obj, int64_t *out, uint64_t hItem,
                                     uint64_t flags)
{
    int64_t startLen = out[1];
    uint32_t stopFlag = ~(uint32_t)(flags >> 1) & 1;
    uint64_t value = 0;
    int ok;
    int64_t off;
    int written;
    HWND hwnd = *(HWND *)(obj + OBJ_HWND);

    for (; hItem != 0; hItem = SendMessageW(hwnd, 0x110a, 1, hItem)) {
        value = 0;
        ok = FUN_1400FEE24(obj, hItem, &value);
        if (ok != 0 && (value & 2) != 0) {
            if ((flags & 0x10) == 0) {
                PECMD_BuildTreeIndexPathStr(obj, hItem, out);
            } else {
                PECMD_AllocString(out, out[1] + 0x3f);
                off = out[1];
                if (off != 0) {
                    out[1] = off + 1;
                    *(uint16_t *)(*out + off * 2) = 0x20;
                }
                written = wsprintfW((LPWSTR)(*out + out[1] * 2), WSTR("@0x%p"),
                                    (void *)(uintptr_t)hItem);
                out[1] += written;
            }
            if (stopFlag != 0)
                break;
        }
        {
            WPARAM next = SendMessageW(hwnd, 0x110a, 4, hItem);
            if (next != 0) {
                int64_t r = PECMD_TreeCollectItems(obj, out, next, flags);
                if (stopFlag != 0 && r > 0)
                    break;
            }
        }
    }
    return out[1] - startLen;
}
