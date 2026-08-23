/* ====================================================================
 * core_msgbox.c — PECMD 消息框 (MessageBox 自研实现)
 *
 * 来源: PECMD原始.EXE (x64)
 *   PECMD_MsgBoxInit @0x14000142c  (7509B: 消息框窗口初始化)
 *
 * 说明: 反编译显示 PECMD 自绘消息框, 按钮 ID 1-7:
 *   1=确定 2=取消 3=Abort 4=重试 5=忽略 6=是 7=否
 *   按钮集由 type&0xf 决定; 控件子类化转发 Enter 键.
 *   结构 mbox (0x3f0 字节) 由调用方分配, 字段见下.
 *
 * 字段速查 (字节偏移):
 *   +0x20  hwnd       +0x28  x         +0x60 消息文本
 *   +0x61 默认按钮    +0x62~0x64 状态  +0x68/0x69 内边距
 *   +0x70 屏宽       +0x71 剩余高     +0x72/0x73/0x74 布局
 *   +0x79 反向显示    +0xd0 标志       +0x13c x   +0x160 超时 ms
 *   +0x164 状态文本   +0x1a0/0x1a8 模态窗口列表
 *   +0x2c 超时(复制)  +0x2f4 状态位    +0x300 消息文本(别名)
 *   +0x310 按钮序号组 +0x318 定时器周期 +0x31c type 标志
 *   +0x328 超时截止   +0x33c..0x394 布局参数
 *   +0x358 按钮 ID 数组 (short 计数 +0x37c)  +0x3a8 图标
 *   +0x3c8 反向标志   +0x3d0/0x3d8/0x3e0 回调/事件
 * ==================================================================== */
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdbool.h>

#include "pecmd_defs.h"
extern WCHAR **FUN_14005B154(WCHAR **pp); /* @0x14005b154 */

/* 全局 */
extern uint32_t g_dpi;           /* g_dpi */
extern WCHAR *g_pLocale;         /* DAT_14013ca70 */
extern int g_msgboxFlag;         /* g_hPelogonWnd 置顶标志 */
extern uint8_t g_msgboxActive;   /* g_u8CF84 */
extern void *g_pVtblA;           /* DAT_14013d670 */

/* 已实现 (core_exec.c / core_var.c / core_string.c) */
extern void FUN_1400633A8(void **ps, int64_t len);
extern void FUN_14005B0B8(void *p);
extern LPCWSTR FUN_1400169BC(int id, WCHAR **ppEnd);
extern void FUN_1400E648C(void **pfont, UINT id);
extern void PECMD_GetDpiCached(HWND hwnd);
extern void PECMD_MoveBtnTextToID9(void *mbox, int btnId);
extern void FUN_14005D9A8(void *mbox, int mode);
extern void PECMD_SetChildFont(HWND hwnd, int64_t font);
extern bool FUN_1400E5900(HWND hwnd, uint32_t clear, uint64_t set);
extern HWND PECMD_CreateControlSubclass(LPCWSTR cls, LPCWSTR text, DWORD style, int x, int y,
                             int w, int h, HWND parent, HMENU id, HINSTANCE inst, void *extra);
extern void PECMD_AppendFmtValue(void *script, uint64_t value, LPCWSTR key, LPCWSTR fmt);

/* 待实现 (B7 后续) */
extern void FUN_140017CDC(void *dst, void *src);   /* FUN_140017CDC */

/* .rdata 常量表 */
/* DAT_14011c544: 默认按钮禁用表 (7 项, 与按钮 ID 对应) */
static const uint8_t s_btnDisable[7] = { 2, 1, 3, 4, 5, 6, 7 };
/* DAT_14011c570: 图标表 (8 项 × 8 字节; 值=系统图标 ID, 0=无) */
static const uint64_t s_iconTbl[8] = {
    0, 0x7f03, 0x7f03, 0x7f04, 0x7f04, 0x7f02, 0x7f01, 0x7f01
};
/* g_szEmpty: 默认文本 (按钮 1 文本 / 复位文本) */
static const WCHAR s_btnDef[2] = { 0, 0 };

/* ========== 消息框初始化 @0x14000142c ========== */
/* mbox: 调用方分配的结构; type: 参数 uVar4 & 0xf 选按钮集 */
uint64_t PECMD_MsgBoxInit(int64_t *mbox)
{
    uint8_t *m = (uint8_t *)mbox;
    HWND wnd = (HWND)mbox[4];        /* +0x20 */
    uint32_t t = *(uint32_t *)(m + 0x31c);
    uint32_t u19 = t & 0xf;
    uint32_t u28 = t & 0xffff0000;
    HWND hStat;                      /* 0x7575 静态框 */
    HWND hProg;                      /* 0x7572/0x7576 进度区 */
    int i;
    LPCWSTR titleStr = (LPCWSTR)mbox[0xd];
    RECT rc;
    RECT rc2;
    POINT pt;
    int ww, wh, x0, y0;
    int dpi;

    PECMD_GetDpiCached(wnd);
    g_flag16a = 0;
    if (g_pVtblA == NULL) {          /* DAT_14013e2c0: 字体/样式对象懒初始化 */
        FUN_1400E648C(&g_pVtblA, 0x3ec);
    }
    if (titleStr != NULL && titleStr != (LPCWSTR)-1) {
        PECMD_SetChildFont(wnd, (int64_t)titleStr);
    }
    t = *(uint32_t *)(m + 0x31c);
    u19 = t & 0xf;
    u28 = t & 0xffff0000;
    if ((mbox[0x62] & 1U) == 0) {
        mbox[0x62] = 5;              /* 默认: 5 秒? */
    } else {
        mbox[0x62] = mbox[0x62] >> 1;
    }
    {
        uint32_t st = (uint32_t)mbox[0x62];
        (void)st;
    }
    if (u19 == 4) {
        GetSystemMenu(wnd, 0);
        EnableMenuItem(GetSystemMenu(wnd, 0), 0xf060, 1);   /* SC_CLOSE 允许 */
    }
    *(int32_t *)(m + 0x358) = 1;     /* m+0x6b 首按钮 */
    *(int16_t *)(m + 0x37c) = 1;     /* 按钮计数 */
    switch (u19) {
    case 4:                          /* Yes/No */
        *(int32_t *)(m + 0x358) = 6;
        *(int32_t *)(m + 0x35c) = 7;
        *(int16_t *)(m + 0x37c) = 2;
        break;
    case 3:                          /* Yes/No/Cancel */
        *(int32_t *)(m + 0x358) = 6;
        *(int32_t *)(m + 0x35c) = 7;
        *(int16_t *)(m + 0x37c) = 2;
        *(int16_t *)(m + 0x37c) = *(int16_t *)(m + 0x37c) + 1;
        *(int32_t *)(m + 0x358 + (int32_t)*(int16_t *)(m + 0x37c) * 4) = 2;
        break;
    case 1:                          /* OK/Cancel */
        *(int32_t *)(m + 0x358) = 1;
        *(int32_t *)(m + 0x35c) = 2;
        *(int16_t *)(m + 0x37c) = 2;
        break;
    case 5:                          /* Retry/Cancel */
        *(int32_t *)(m + 0x358) = 4;
        *(int32_t *)(m + 0x35c) = 2;
        *(int16_t *)(m + 0x37c) = 2;
        break;
    case 2:                          /* Abort/Retry/Ignore */
        *(int32_t *)(m + 0x358) = 3;
        *(int32_t *)(m + 0x35c) = 4;
        *(int16_t *)(m + 0x37c) = 2;
        *(int16_t *)(m + 0x37c) = *(int16_t *)(m + 0x37c) + 1;
        *(int32_t *)(m + 0x358 + (int32_t)*(int16_t *)(m + 0x37c) * 4) = 5;
        break;
    default:                         /* OK */
        *(int32_t *)(m + 0x358) = 1;
        *(int32_t *)(m + 0x35c) = 2;
        break;
    }
    /* 系统菜单/样式调整 */
    if ((t >> 0x13 & 1) != 0) {
        EnableMenuItem(GetSystemMenu(wnd, 0), 0xf060, 1);
    }
    if ((t >> 0x10 & 1) == 0) {
        FUN_1400E5900(wnd, 0x10000, 0);
    }
    if ((t >> 0x11 & 1) == 0) {
        FUN_1400E5900(wnd, 0x20000, 0);
    }
    if ((t >> 0x12 & 1) != 0) {
        LONG_PTR v = GetWindowLongW(wnd, -0x10);
        SetWindowLongW(wnd, -0x10, v | 0x40000);
    }
    /* 尺寸计算: 边框/DPI 缩放 */
    dpi = (int)g_dpi;
    GetWindowRect(wnd, &rc2);
    GetClientRect(wnd, &rc);
    pt.x = 0;
    pt.y = 0;
    ClientToScreen(wnd, &pt);
    x0 = pt.x - rc2.left;
    y0 = pt.y - rc2.top;
    ww = MulDiv(x0 + 0xf5, dpi, 0x48);
    wh = MulDiv(y0 + 0x91, dpi, 0x48);
    SetWindowPos(wnd, 0, 0, 0, ww, wh, 0x16);
    GetWindowRect(wnd, &rc2);
    GetClientRect(wnd, &rc);
    x0 = ((rc.left - rc2.left) - rc.right) + -6 + rc2.right;
    y0 = ((rc.top - rc2.bottom) - rc2.top) + -0x20 + rc2.bottom;
    {
        uint32_t hasProg = *(uint32_t *)(m + 0x31c) & 0x200000;
        hProg = GetDlgItem(wnd, (uint32_t)(hasProg ? 4 : 0) + 0x7572);
        hStat = (HWND)(uintptr_t)GetDlgItem(wnd, 0x7575);
        if (hProg == 0) {
            int bw = MulDiv(0x7c, dpi, 0x48);
            int bh = MulDiv(0xc2, dpi, 0x48);
            int bx = MulDiv(6, dpi, 0x48);
            int by = MulDiv(6, dpi, 0x48);
            /* 按钮 9: 默认复位按钮 (不可见) */
            PECMD_CreateControlSubclass(WSTR("BUTTON"), s_btnDef, 0x50010001, bx, by, bw, bh,
                             wnd, (HMENU)9, g_hInstance, NULL);
            PECMD_CreateControlSubclass(WSTR("BUTTON"), WSTR("确定&O"), 0x40010000, bx, by, bw, bh,
                             wnd, (HMENU)1, g_hInstance, NULL);
            PECMD_CreateControlSubclass(WSTR("BUTTON"), WSTR("取消&C"), 0x40010000, bx, by, bw, bh,
                             wnd, (HMENU)2, g_hInstance, NULL);
            PECMD_CreateControlSubclass(WSTR("BUTTON"), WSTR("&Abort"), 0x40010000, bx, by, bw, bh,
                             wnd, (HMENU)3, g_hInstance, NULL);
            PECMD_CreateControlSubclass(WSTR("BUTTON"), WSTR("重试&R"), 0x40010000, bx, by, bw, bh,
                             wnd, (HMENU)4, g_hInstance, NULL);
            PECMD_CreateControlSubclass(WSTR("BUTTON"), WSTR("忽略&I"), 0x40010000, bx, by, bw, bh,
                             wnd, (HMENU)5, g_hInstance, NULL);
            PECMD_CreateControlSubclass(WSTR("BUTTON"), WSTR("是&Y"), 0x40010000, bx, by, bw, bh,
                             wnd, (HMENU)6, g_hInstance, NULL);
            PECMD_CreateControlSubclass(WSTR("BUTTON"), WSTR("否&N"), 0x40010000, bx, by, bw, bh,
                             wnd, (HMENU)7, g_hInstance, NULL);
            if (!hasProg) {
                hStat = CreateWindowExW(0, WSTR("STATIC"), NULL, 0x50000003,
                                        MulDiv(7, dpi, 0x48), MulDiv(2, dpi, 0x48),
                                        MulDiv(2, dpi, 0x48), MulDiv(0x15, dpi, 0x48),
                                        wnd, (HMENU)0x7575, g_hInstance, NULL);
            } else {
                hProg = CreateWindowExW(0x20, WSTR("STATIC"), NULL, 0x50000000,
                                        MulDiv(0x20, dpi, 0x48), MulDiv(5, dpi, 0x48),
                                        MulDiv(0xce, dpi, 0x48), MulDiv(0x73, dpi, 0x48),
                                        wnd, (HMENU)((hasProg ? 4 : 0) + 0x7572),
                                        g_hInstance, NULL);
            }
        } else if (hasProg) {
            ShowWindow(GetDlgItem(wnd, 0x7572), 0);
            ShowWindow(hProg, 5);
        }
    }
    /* 设置标题/按钮文本 */
    GetWindowRect(hProg, &rc);
    PECMD_SetChildFont(wnd, (int64_t)titleStr);
    {
        int iBtn;
        for (iBtn = 1; iBtn <= 7; iBtn++) {
            LPCWSTR s = FUN_1400169BC(0x3f3 + iBtn, NULL);
            if (s) {
                SetDlgItemTextW(wnd, iBtn, s);
            }
        }
    }
    /* 图标 */
    GetClientRect(hStat, &rc);
    {
        uint32_t st = (uint32_t)mbox[0x62];
        if (st < 0x21) {
            if (st > 8) {
                goto noicon;
            }
            i = (int)st;
            if (s_iconTbl[i] != 0) {
                HICON hico = LoadIconW(0, (LPCWSTR)(uintptr_t)s_iconTbl[i]);
                if (hico && hStat) {
                    SendMessageW(hStat, 0x170, (WPARAM)hico, 0);
                }
            }
        } else {
            HICON hico = LoadIconW(g_hInst, (LPCWSTR)(uintptr_t)((st & 0x3fffffff) - 1));
            mbox[0x77] = (int64_t)hico;
            if (hico && hStat) {
                SendMessageW(hStat, 0x170, (WPARAM)hico, 0);
            }
        }
    }
noicon:
    /* 超时显示文本 */
    {
        int tm = (int)mbox[0x2c];
        *(uint8_t *)(m + 0x3c8) = (tm < 0);
        if (tm < 0) {
            *(int32_t *)(m + 0x160) = -tm;
        }
        if (tm > 0) {
            wsprintfW((LPWSTR)(m + 0x164 + *(int32_t *)(m + 0x2f4) * 2), WSTR("(%d)ms"));
        }
        if (tm > 1000 && (*(int32_t *)(m + 0x160) / 1000) * 1000 == *(int32_t *)(m + 0x160)) {
            *(int32_t *)(m + 0x318) = 1000;        /* m[99] */
            wsprintfW((LPWSTR)(m + 0x164 + *(int32_t *)(m + 0x2f4) * 2), WSTR("(%ld)"));
        }
    }
    /* 消息文本宽度测量 */
    {
        HDC dc = GetDC(hProg);
        RECT tr = { 0, 0, 0, 0 };
        DrawTextW(dc, (LPCWSTR)(m + 0x164), -1, &tr, 0x420);
        {
            int w = tr.right - tr.left;
            if (w > 0) {
                i = w;
            }
        }
        if (dc) {
            ReleaseDC(hProg, dc);
        }
    }
    SetWindowTextW(hProg, (LPCWSTR)mbox[0x60]);
    /* 布局: 文本区/按钮区 */
    {
        RECT wr;
        int tx, ty;
        GetWindowRect(wnd, &wr);
        pt.x = 0;
        pt.y = 0;
        ClientToScreen(wnd, &pt);
        tx = pt.x - wr.left;
        ty = pt.y - wr.top;
        GetWindowRect(hProg, &rc);
        if (u19 == 0 && hStat) {
            SetWindowPos(hStat, 0, -1, -1, 1, 1, 0x94);
        }
        GetWindowRect(hProg, &rc2);
        SetWindowPos(hProg, 0, rc2.left - (wr.left + tx), rc2.top - (wr.top + ty),
                     rc2.right - rc2.left + 0x28, rc2.bottom - rc2.top, 0x14);
        GetWindowRect(hProg, &rc);
        GetWindowRect(GetDlgItem(wnd, 1), &rc2);
        *(int32_t *)(m + 0x33c) = rc.left - wr.left;
        *(int32_t *)(m + 0x344) = rc.top - wr.top;
        *(int32_t *)(m + 0x394) = MulDiv(0x1e, dpi, 0x48) + *(int32_t *)(m + 0x344);
        *(int32_t *)(m + 0x360) = *(int32_t *)(m + 0x340) + *(int32_t *)(m + 0x33c);
        *(int32_t *)(m + 0x34c) = MulDiv(0x2a, dpi, 0x48);
        *(int32_t *)(m + 0x350) = rc2.right - rc2.left;    /* +0x6a */
        *(int32_t *)(m + 0x354) = rc2.bottom - rc2.top;
        *(int32_t *)(m + 0x380) = GetSystemMetrics(0x3d);   /* +0x70 */
        *(int32_t *)(m + 0x384) = GetSystemMetrics(0x3e);   /* +0x38c+? */
        *(int32_t *)(m + 0x388) = *(int32_t *)(m + 0x380) - *(int32_t *)(m + 0x360);
        *(int32_t *)(m + 0x38c) = *(int32_t *)(m + 0x384) - *(int32_t *)(m + 0x394);
    }
    /* 图标复位 (WM_SETICON) */
    if (mbox[0x75] != 0) {
        SendMessageW(wnd, 0x80, 1, mbox[0x75]);
        SendMessageW(wnd, 0x80, 0, mbox[0x75]);
    }
    /* 消息文本多行测量 → 滚动条决策 */
    {
        int need_h = 0;
        int nlines = 0;
        int lines;
        LPCWSTR msg = (LPCWSTR)mbox[0x60];
        int tlen = (int)lstrlenW(msg);
        int maxw = *(int32_t *)(m + 0x384) / 0x11 * 0x3c;
        int overflow = maxw <= tlen;
        (void)overflow;
        if (u19 != 0 || tlen <= 0) {
            /* 简化: 单行测量 */
            lines = 1;
        } else {
            RECT tr = { 0, 0, 0, 0 };
            HDC dc = GetDC(hProg);
            DrawTextW(dc, msg, -1, &tr, 0x400);
            if (dc) {
                ReleaseDC(hProg, dc);
            }
            lines = 1;
        }
        (void)lines;
        (void)nlines;
        (void)need_h;
    }
    /* 最终尺寸 + 按钮布局 */
    {
        int32_t n = *(int16_t *)(m + 0x37c);
        int32_t hw_all = (int32_t)mbox[0x6a];
        int32_t gap;
        int32_t x;
        int32_t total;
        int32_t bw2 = *(int32_t *)(m + 0x34c);
        int32_t *pb = (int32_t *)(m + 0x358);
        int iBtn;

        total = hw_all * n;
        gap = (total * 3) / 2;
        x = *(int32_t *)(m + 0x388) / n;
        if (gap > 0) {
            x = (gap / (n * 2)) + gap / n;
        }
        for (iBtn = 0; iBtn < n; iBtn++) {
            HWND b = GetDlgItem(wnd, (int)pb[iBtn]);
            SetWindowPos(b, 0,
                         iBtn * x + ((x / 2 - (x * n) / 2) + *(int32_t *)(m + 0x388) / 2) - hw_all / 2,
                         *(int32_t *)(m + 0x38c) - bw2,
                         (int32_t)mbox[0x6a], *(int32_t *)(m + 0x354), 5);
            ShowWindow(GetDlgItem(wnd, (int)pb[iBtn]), 5);
        }
    }
    /* 按钮启用/禁用 */
    {
        int iBtn;
        for (iBtn = 0; iBtn < 7; iBtn++) {
            int bid = s_btnDisable[iBtn];
            bool present = false;
            int32_t n = *(int16_t *)(m + 0x37c);
            int32_t *pb = (int32_t *)(m + 0x358);
            int k;
            for (k = 0; k < n; k++) {
                if (pb[k] == bid) {
                    present = true;
                }
            }
            if (!present) {
                if (bid == 2) {
                    SetWindowTextW(GetDlgItem(wnd, 2), s_btnDef);
                } else {
                    EnableWindow(GetDlgItem(wnd, bid), 0);
                }
            }
        }
    }
    /* 默认按钮 */
    {
        uint8_t def = m[0x61];
        if (def > 7) {
            m[0x61] = 0;
        }
        if (def != 0) {
            PECMD_MoveBtnTextToID9(mbox, def);
        }
        if (def == 0) {
            ShowWindow(GetDlgItem(wnd, 9), 0);
        }
    }
    /* 居中定位 */
    {
        HWND desk = GetDesktopWindow();
        RECT dr, wr;
        int dx, dy;
        GetWindowRect(desk, &dr);
        GetWindowRect(wnd, &wr);
        dx = ((dr.right - wr.right) - dr.left + wr.left) / 2;
        if (*(int32_t *)(m + 0x13c) != (int32_t)0x80000000) {
            dx = *(int32_t *)(m + 0x13c);
        }
        dy = ((wr.top - wr.bottom) - dr.top + dr.bottom) / 2;
        if (*(int32_t *)(m + 0x140) != (int32_t)0x80000000) {
            dy = *(int32_t *)(m + 0x140);
        }
        SetWindowPos(wnd, 0, dx, dy, 0, 0, 0x11);
    }
    /* 超时定时器 */
    {
        int tm = (int)mbox[0x2c];
        uint32_t period;
        DWORD now = GetTickCount();
        if (tm > 0) {
            m[0x328] = (uint8_t)(now + (uint32_t)tm);   /* 简化 */
        }
        period = 0x32;
        if (tm > 1000 && (tm / 1000) * 1000 == tm) {
            period = 1000;
            wsprintfW((LPWSTR)(m + 0x164 + *(int32_t *)(m + 0x2f4) * 2), WSTR("(%ld)"));
        }
        if (m[0x3c8] != 0) {
            *(int32_t *)(m + 0x318) = tm;
        }
        SetWindowTextW(wnd, (LPCWSTR)(m + 0x164));
        if (tm > 0) {
            SetTimer(wnd, 10, period, NULL);
        }
    }
    /* 显示窗口 */
    {
        STARTUPINFOW si;
        WORD show = 5;
        memset(&si, 0, sizeof(si));
        si.cb = sizeof(si);
        GetStartupInfoW(&si);
        if ((si.dwFlags & 1) != 0) {
            show = si.wShowWindow;
        }
        ShowWindow(wnd, (show != 0) ? 5 : 0);
        if (g_msgboxFlag != 0) {
            u28 |= 0x100000;
        }
        if (show != 0) {
            ShowWindow(wnd, 5);
            SetTimer(wnd, 0x16, 1, NULL);
            SetWindowPos(wnd, 0, 0, 0, 0, 0, 3);
            SetWindowPos(wnd, (HWND)-1, 0, 0, 0, 0, 3);
            SetActiveWindow(wnd);
            SetForegroundWindow(wnd);
            BringWindowToTop(wnd);
            if ((u28 >> 0x14 & 1) == 0) {
                SetTimer(wnd, 0x1e, 1, NULL);
            }
        }
        if ((u28 >> 0x14 & 1) != 0) {
            SetWindowPos(wnd, (HWND)-1, 0, 0, 0, 0, 3);
        }
        SetTimer(wnd, 0x270c, 1000, NULL);
        FUN_14005D9A8(mbox, 1);
        if (g_msgboxFlag != 0) {
            if (g_msgboxActive != 0) {
                SetWindowPos(wnd, (HWND)-1, 0, 0, 0, 0, 0x13);
            }
            if (g_msgboxFlag != 0) {
                SetWindowLongPtrW(wnd, -8, g_msgboxFlag);
            }
        }
        if (mbox[0x7b] != 0 && mbox[0x7a] != 0) {
            PECMD_AppendFmtValue((void *)mbox[0x7a], (uint64_t)(uintptr_t)mbox[0x40], (LPCWSTR)(uintptr_t)mbox[0x7b], WSTR("%I64u"));
        }
    }
    return 1;
}
