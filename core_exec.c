/* ====================================================================
 * core_exec.c — PECMD 命令执行/消息框/语言表/字体 支撑族
 *
 * 来源: PECMD原始.EXE (x64)
 *   PECMD_SubclassEnterControl   @0x1400011d4   (控件子类化: Enter→WM_COMMAND)
 *   PECMD_CreateControlSubclass   @0x140001258   (创建控件+子类化)
 *   PECMD_MoveBtnTextToID9  @0x1400012fc   (按钮文字移到 ID9 静态框)
 *   FUN_140003A20     @0x140004fd4   (命令行入口: 调 FUN_14004EB34)
 *   PECMD_LoadLanguageFile     @0x1400166b4   (加载 .lang 语言文件)
 *   FUN_1400169BC    @0x1400169bc   (消息表按 ID 查文本)
 *   FUN_14005B6AC      @0x14005b6ac   (取字符串: lang 优先, 回退资源)
 *   PECMD_SynthDlgKeyMsg   @0x14005d968   (合成对话框按键消息)
 *   FUN_14005D9A8      @0x14005d9a8   (错误提示音/键音)
 *   PECMD_GetDpi    @0x140062900   (取 DPI LOGPIXELSX)
 *   PECMD_GetDpiCached       @0x140062950   (DPI 缓存)
 *   PECMD_EnumChildProc @0x140062970  (EnumChildWindows 回调)
 *   PECMD_SetChildFont @0x140062994   (遍历子窗口设字体)
 *   PECMD_ParseUIntValue     @0x140067d20   (数字解析封装)
 *   FUN_14006A7F4    @0x140067b78   (数字解析: 0x/0o/0b 前缀)
 *   FUN_1400E5900  @0x1400e5900   (窗口样式位操作)
 *   PECMD_AllocSmallObject       @0x140063344   (小对象分配 2 字节头)
 *   FUN_1400E648C     @0x1400e648c   (按 lang 创建字体)
 *   FUN_1400B89DC  @0x1400b89dc   (从字体对象克隆新字体)
 *   FUN_1400B1F34  @0x1400b1f34   (字体规格创建字体)
 * ==================================================================== */
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <wchar.h>
#include <stdbool.h>

#include "pecmd_defs.h"
extern WCHAR **FUN_14005B154(WCHAR **pp); /* @0x14005b154 */

/* 全局 */
extern WCHAR *g_pLangBuf;        /* DAT_14013ca78 lang 缓冲 */
extern int64_t g_langLen;        /* DAT_14013ca80 lang 数据长度 */
extern uint32_t g_dpi;           /* g_dpi */
extern void *g_subWndProc;       /* DAT_14013c908 原 WndProc */
extern HANDLE g_pFontBase;      /* DAT_14013e2a8 字体基对象 */

/* .rdata 常量 */
extern double g_fontSizeDef;     /* DAT_1401293c0 = -0x80000000.0 */
extern double g_fontRound;       /* DAT_140126070 = 0.499 */
extern double g_fontMinus0;      /* DAT_140125238 = -0.0 */
extern double g_dpiBase;         /* DAT_140126078 = 72.0 */

extern int64_t PECMD_LoadLanguageFile(void);   /* PECMD_LoadLanguageFile */
extern WCHAR *FUN_1400637DC(WCHAR **ps, LPCSTR src, int64_t srclen, int64_t codepage); /* FUN_1400637DC core_init.c */
extern void FUN_1400633A8(void **ps, int64_t len);  /* FUN_1400633A8 core_thread.c */
extern void FUN_14005B0B8(void *p);                /* FUN_14005B0B8 core_thread.c */

/* 待实现 (B7 后续/声明) */
extern uint64_t FUN_14004EB34(HINSTANCE hinst, uint64_t flag, const WCHAR *cmd);   /* FUN_14004EB34 */
extern int PECMD_ReadFileStr(const WCHAR *path, void **pbuf);                          /* PECMD_ReadFileStr */
extern int PECMD_EvalExpr(WCHAR **pp, double *out);                                    /* PECMD_ParseParenthesizedExpression */

/* ========== 控件子类化 @0x1400011d4 ========== */
/* Enter 键 → 向父窗发 WM_COMMAND(id); 其余转发原 WndProc */
void PECMD_SubclassEnterControl(HWND hwnd, int msg, WPARAM wp, LPARAM lp)
{
    if (msg == 0x100 && wp == 0xd) {            /* WM_KEYDOWN VK_RETURN */
        SendMessageW(GetParent(hwnd), 0x111,     /* WM_COMMAND */
                     (WPARAM)(uint16_t)GetDlgCtrlID(hwnd), 0x100);
    }
    if (g_subWndProc) {
        ((LRESULT (*)(HWND, UINT, WPARAM, LPARAM))g_subWndProc)(hwnd, (UINT)msg, wp, lp);
    }
}

/* ========== 创建控件+子类化 @0x140001258 ========== */
HWND PECMD_CreateControlSubclass(LPCWSTR cls, LPCWSTR text, DWORD style, int x, int y,
                      int w, int h, HWND parent, HMENU id, HINSTANCE inst, void *extra)
{
    HWND hw;
    LONG_PTR old;

    hw = CreateWindowExW(0, cls, text, style, x, y, w, h, parent, id, inst, extra);
    old = SetWindowLongPtrW(hw, -4, (LONG_PTR)PECMD_SubclassEnterControl);
    if (g_subWndProc == NULL) {
        g_subWndProc = (void *)old;
    }
    return hw;
}

/* ========== 按钮文字移到 ID9 @0x1400012fc ========== */
void PECMD_MoveBtnTextToID9(void *mbox, int btnId)
{
    HWND wnd = *(HWND *)((uint8_t *)mbox + 0x20);
    HWND btn = GetDlgItem(wnd, btnId);
    HWND dst = GetDlgItem(wnd, 9);
    WCHAR buf[104];
    POINT pt;
    RECT rc;

    buf[0] = L'\0';
    GetWindowTextW(btn, buf, 100);
    SetWindowTextW(dst, buf);
    pt.x = 0;
    pt.y = 0;
    ClientToScreen(wnd, &pt);
    GetWindowRect(btn, &rc);
    rc.left -= pt.x;
    rc.top -= pt.y;
    SetWindowPos(dst, 0, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top, 4);
    MoveWindow(dst, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top, TRUE);
    ShowWindow(btn, 0);
}

/* ========== 命令行入口 @0x140004fd4 ========== */
void FUN_140003A20(const WCHAR *cmd)
{
    SetProcessWorkingSetSize(GetCurrentProcess(), (uint64_t)-1, (uint64_t)-1);
    FUN_14004EB34(g_hInst, 0, cmd);
}

/* ========== 小对象分配 (头 size=2) @0x140063344 ========== */
void *PECMD_AllocSmallObject(void **ps)
{
    uint8_t *hdr;

    *ps = NULL;
    for (;;) {
        hdr = (uint8_t *)HeapAlloc(g_hHeap, 0, 10);
        if (hdr) break;
        FUN_1400630D0(2);
    }
    *(size_t *)hdr = 2;
    *(uint32_t *)(hdr + 4) = 0xaa55;
    *ps = hdr + 8;
    if (*ps) {
        *(uint8_t *)*ps = 0;
    }
    return *ps;
}

/* ========== 数字解析: 0x/0o/0b 前缀 @0x140067b78 ========== */
/* 支持: [-]0xHH / 0oOO / 0bBB / 十进制; 解析后 *pp 指向剩余串 */
int FUN_140067B78(WCHAR **pp, uint64_t *out)
{
    WCHAR *p;
    int neg = 0;

    p = *pp;
    FUN_14005B154(pp);
    p = *pp;
    if (*p == L'-') {
        p++;
        neg = -1;
    }
    if (*p == L'0') {
        WCHAR c = p[1] | 0x20;
        if (c == L'x') {
            *out = 0;
            p += 2;
            while (((WCHAR)(*p - L'0') <= 9) || ((WCHAR)((*p | 0x20) - L'a') <= 5)) {
                if ((WCHAR)(*p - L'0') <= 9) {
                    *out = *out * 16 + (uint64_t)(*p - L'0');
                } else {
                    *out = *out * 16 + (uint64_t)((*p | 0x20) - L'a' + 10);
                }
                p++;
            }
            *pp = p;
            goto done;
        }
        if (c == L'o') {
            *out = 0;
            p += 2;
            while (*p >= L'0' && *p <= L'7') {
                *out = (*out << 3) | (uint64_t)(*p - L'0');
                p++;
            }
            *pp = p;
            goto done;
        }
        if (c == L'b') {
            *out = 0;
            p += 2;
            while (*p >= L'0' && *p <= L'1') {
                *out = *out * 2 + (uint64_t)(*p - L'0');
                p++;
            }
            *pp = p;
            goto done;
        }
    }
    if ((WCHAR)(*p - L'0') > 9) {
        return 0;
    }
    *out = FUN_140064A88(p);
    while (*p >= L'0' && *p <= L'9') {
        p++;
    }
    *pp = p;
done:
    if (neg) {
        *out = (uint64_t)-(int64_t)*out;
    }
    return 1;
}

/* ========== 数字解析封装 @0x140067d20 ========== */
static bool PECMD_ParseUIntValue(WCHAR **pp, int *out)
{
    uint64_t v;

    v = (uint64_t)(uint32_t)*out;
    if (!FUN_140067B78(pp, &v)) {
        return false;
    }
    *out = (int)v;
    return true;
}

/* ========== 窗口样式位操作 @0x1400e5900 ========== */
bool FUN_1400E5900(HWND hwnd, uint32_t clear, uint64_t set)
{
    LONG_PTR old = GetWindowLongPtrW(hwnd, -0x10);
    LONG_PTR neu = (old & ~(LONG_PTR)clear) | (LONG_PTR)(set & 0xffffffff);
    SetWindowLongPtrW(hwnd, -0x10, neu);
    return old != neu;
}

/* ========== DPI @0x140062900 ========== */
int PECMD_GetDpi(HWND hwnd)
{
    HDC dc = GetDC(hwnd);
    int v = GetDeviceCaps(dc, 0x5a);    /* LOGPIXELSX */
    if (dc) {
        ReleaseDC(hwnd, dc);
    }
    return v;
}

/* ========== DPI 缓存 @0x140062950 ========== */
void PECMD_GetDpiCached(HWND hwnd)
{
    if (g_dpi < 1) {
        g_dpi = (uint32_t)PECMD_GetDpi(hwnd);
    }
}

/* ========== EnumChildWindows 回调 @0x140062970 ========== */
int PECMD_EnumChildProc(HWND hwnd, WPARAM lp)
{
    SendMessageW(hwnd, 0x452, lp, 1);   /* WM_SETFONT */
    return 1;
}

/* ========== 遍历子窗口设字体 @0x140062994 ========== */
void PECMD_SetChildFont(HWND hwnd, int64_t font)
{
    if (font != 0 && font != -1) {
        EnumChildWindows(hwnd, (void *)PECMD_EnumChildProc, (LONG_PTR)font);
    }
}

/* ========== 合成对话框按键消息 @0x14005d968 ========== */
void PECMD_SynthDlgKeyMsg(HWND hwnd, UINT msg, uint32_t vk, uint32_t lparam)
{
    MSG m;

    m.hwnd = hwnd;
    m.message = msg;
    m.wParam = (WPARAM)vk;
    m.lParam = (LPARAM)lparam;
    m.time = 0;
    m.pt.x = 0;
    m.pt.y = 0;
    IsDialogMessageW(hwnd, &m);
}

/* ========== 错误提示音/键音 @0x14005d9a8 ========== */
void FUN_14005D9A8(void *mbox, int mode)
{
    HWND wnd;

    if ((*(uint32_t *)((uint8_t *)mbox + 0xd0) >> 0xf & 1) != 0) {
        return;
    }
    if (mode == 0) {
        if ((*(uint32_t *)((uint8_t *)mbox + 0xd0) >> 0xd & 1) != 0) {
            return;
        }
    } else if (mode != 0x10) {
        goto beep;
    }
    {
        int i = 0;
        int n = **(int **)((uint8_t *)mbox + 0x1a8);
        if (n > 0) {
            uint8_t *base = *(uint8_t **)((uint8_t *)mbox + 0x1a0);
            for (i = 0; i < n; i++) {
                uint8_t *p = *(uint8_t **)(base + (size_t)i * 8);
                if (p && (wnd = *(HWND *)(p + 0x38)) != 0) {
                    LONG_PTR st = GetWindowLongW(wnd, -0x10);
                    if ((st & 0x18010000) == 0x10010000) {
                        if (!wnd) {
                            return;
                        }
                        *(uint32_t *)((uint8_t *)mbox + 0xd0) |= 0x2000;
                        goto beep;
                    }
                }
            }
        }
    }
    return;
beep:
    wnd = *(HWND *)((uint8_t *)mbox + 0x20);
    PECMD_SynthDlgKeyMsg(wnd, 0x100, 9, 1);
    PECMD_SynthDlgKeyMsg(wnd, 0x101, 9, 1);
    PECMD_SynthDlgKeyMsg(wnd, 0x100, 0x10, 1);
    PECMD_SynthDlgKeyMsg(wnd, 0x100, 9, 1);
    PECMD_SynthDlgKeyMsg(wnd, 0x101, 9, 1);
    PECMD_SynthDlgKeyMsg(wnd, 0x101, 0x10, 1);
}

/* ========== 消息表按 ID 查文本 @0x1400169bc ========== */
/* lang 缓冲格式: "ID:文本|ID:文本|..." 各条 \0 分隔, 最后双 \0 */
LPCWSTR FUN_1400169BC(int id, WCHAR **ppEnd)
{
    if (g_pLangBuf == NULL) {
        PECMD_LoadLanguageFile();
    }
    if (*g_pLangBuf != L'\0') {
        WCHAR *p = g_pLangBuf;
        WCHAR *end = (WCHAR *)((uint8_t *)g_pLangBuf + g_langLen);
        while (p < end) {
            int cur = -0x80000000;
            if (PECMD_ParseUIntValue(&p, &cur) && cur == id) {
                p++;
                {
                    WCHAR *q = p;
                    WCHAR c;
                    while ((c = *q) != L'\0' && c != L'|') {
                        q++;
                    }
                    if (c == L'\0') {
                        return p;
                    }
                    if (ppEnd) {
                        *ppEnd = p;
                    }
                    return q + 1;
                }
            }
            {
                int n = (int)lstrlenW(p);
                p += n + 1;
                while (*p == L'\0') {
                    if (end <= p) {
                        return NULL;
                    }
                    p++;
                }
            }
        }
    }
    return NULL;
}

/* ========== 取字符串: lang 优先, 回退资源 @0x14005b6ac ========== */
LPCWSTR FUN_14005B6AC(HINSTANCE inst, UINT id, LPWSTR buf, int buflen)
{
    LPCWSTR p = FUN_1400169BC((int)id, NULL);
    if (p == NULL) {
        *buf = L'\0';
        LoadStringW(inst, id, buf, buflen);
        p = buf;
    }
    return p;
}

/* ========== 字体规格创建字体 @0x1400b1f34 ========== */
/* lf = LOGFONT 布局缓冲(0x5c), size = 字号 double, name = 字面 */
HFONT FUN_1400B1F34(int *lf, double *size, LPCWSTR name)
{
    double d;
    int dpi = (int)g_dpi;
    double s;
    int h;
    int flag;
    flag = 0;
    int special = -0x80000000;
    int weight;
    WCHAR *face;
    WCHAR *t;
    WCHAR *buf;
    (void)flag;

    d = g_fontRound;                    /* 0.499 */
    s = *size;
    if (s < 0.0) {
        s = s - g_fontRound;
    } else {
        s = s + g_fontRound;
    }
    weight = *lf;
    h = (int)s;
    flag = 0;
    if (weight < 0) {
        weight = -weight;
    }
    FUN_1400702B0(&buf, name);
    s = g_fontMinus0;                   /* -0.0 */
    special = -0x80000000;
    t = buf;
    if ((h < 1) || (*lf == (int)0x80000000)) {
        if (h == (int)0x80000000) {
            /* 默认字号: 表达式求值或 -0x80000000 */
            double v = 0.0;
            if (PECMD_EvalExpr(&t, &v)) {
                if (v < 0.0) {
                    v = s - v;
                }
                *size = v;
                h = (int)(v + g_fontRound);
            }
            flag = 1;
        } else if (h == -1) {
            h = (int)0x80000000;
        }
        if (h != special) {
            *lf = h;
            weight = h;
        }
        face = buf;
    } else {
        int dpi2 = dpi;
        if (*t == L'~') {               /* 固定字号模式 */
            t++;
            if (dpi != 0) {
                dpi2 = 0x60;
            }
            FUN_14005B154((WCHAR **)&t);
            buf = t;
            if (*t != L'\0') {
                buf = t + 1;
            }
        } else if (*t == L'^') {        /* DPI 缩放模式 */
            t++;
            flag = 4;
            if (dpi == 0) {
                FUN_14005B154((WCHAR **)&t);
                buf = t;
                if (*t != L'\0') {
                    buf = t + 1;
                }
            } else {
                dpi2 = 0x48;
            }
        }
        face = buf;
        if (h != special) {
            if (dpi != 0) {
                if (*size <= 0.0 && *size != 0.0) {
                    *size = s - *size;
                }
                d = ((double)dpi2 * *size) / g_dpiBase;   /* /72 */
                *size = d;
                weight = (int)(d + g_fontRound);
                *lf = -weight;
                if (lf[1] != 0) {
                    lf[1] = -weight / 2;
                }
            } else {
                if (h != special) {
                    *lf = h;
                    weight = h;
                }
            }
        }
    }
    if (weight != special && lf[1] != 0) {
        int v = -weight;
        if (v <= weight) {
            v = weight;
        }
        lf[1] = -(v / 2);
    }
    if (weight < 0 && weight != special) {
        weight = -weight;
    }
    {
        uint32_t b5 = (uint32_t)lf[5];
        uint32_t b6 = (uint32_t)lf[6];
        HFONT f = CreateFontW(-weight, lf[1], lf[2], lf[3], lf[4],
                              (uint8_t)b5, (uint8_t)(b5 >> 8),
                              (uint8_t)(b5 >> 16), (uint8_t)(b5 >> 24),
                              (uint8_t)b6, (uint8_t)(b6 >> 8),
                              (uint8_t)(b6 >> 16), (uint8_t)(b6 >> 24),
                              face);
        PECMD_FreeStrBuf(&buf);
        return f;
    }
}

/* ========== 从字体对象克隆新字体 @0x1400b89dc ========== */
HFONT FUN_1400B89DC(HANDLE obj, double *size, LPCWSTR name)
{
    void *buf;
    HFONT f;

    FUN_1400633A8(&buf, 0x5c);
    FUN_14005B0B8(buf);
    *(uint32_t *)((uint8_t *)buf + 8) = 400;    /* lfWeight 默认 */
    GetObjectW(obj, 0x5c, buf);
    *(uint32_t *)buf = 0;                       /* lfHeight */
    *(uint8_t *)((uint8_t *)buf + 0x17) = 1;    /* DEFAULT_CHARSET */
    f = FUN_1400B1F34((int *)buf, size, name);
    PECMD_FreeStrBuf((WCHAR **)&buf);
    return f;
}

/* ========== 按 lang 创建字体 @0x1400e648c ========== */
void FUN_1400E648C(void **pfont, UINT id)
{
    LPWSTR tmp;
    LPCWSTR face;
    HANDLE base;
    double sz;
    HFONT f;

    if (*pfont == (void *)-1) {
        return;
    }
    PECMD_AllocString(&tmp, 0x516);
    *tmp = L'\0';
    face = FUN_1400169BC((int)id, NULL);
    if (face == NULL) {
        FUN_14005B6AC(g_hInst, id, tmp, 0x514);
        {
            LPWSTR bar = (LPWSTR)(uintptr_t)StrChrW(tmp, L'|');
            face = tmp;
            if (bar) {
                face = bar + 1;
            }
        }
    }
    sz = g_fontSizeDef;
    base = (HANDLE)-1;
    if (g_pFontBase) {                          /* DAT_14013e2a8 */
        base = g_pFontBase;
    }
    if (g_dpi < 1) {
        g_dpi = (uint32_t)PECMD_GetDpi(GetDesktopWindow());
    }
    f = FUN_1400B89DC(base, &sz, face);
    *pfont = (void *)f;
    PECMD_FreeStrBuf(&tmp);
}

/* ========== 语言文件加载 @0x1400166b4 ========== */
/* 解析 "<exe>.lang": #code= 行 + #str: 段, 转义 \r \n */
int64_t PECMD_LoadLanguageFile(void)
{
    WCHAR *p;
    WCHAR *buf;
    WCHAR *src;
    WCHAR *dst;
    int len;
    int i;
    void *ftmp;
    int64_t r;

    if (g_pLangBuf == NULL) {
        FUN_1400637DC(&g_pLangBuf, "", -1, -1);    /* 空表 */
        g_langLen = 0;
        PECMD_AllocString(&buf, 0x105);
        *buf = L'\0';
        GetModuleFileNameW(g_hInst, buf, 0x104);
        len = (int)lstrlenW(buf);
        {
            WCHAR *s = NULL;
            FUN_1400637DC(&s, ".", -1, -1);
            FUN_14006375C((WCHAR **)&buf, s);
            PECMD_FreeStrBuf(&s);
        }
        FUN_14006375C((WCHAR **)&buf, g_pLocale);       /* DAT_14013ca70 代码页串 */
        {
            WCHAR *s = NULL;
            FUN_1400637DC(&s, ".lang", -1, -1);
            FUN_14006375C((WCHAR **)&buf, s);
            PECMD_FreeStrBuf(&s);
        }
        ftmp = NULL;
        r = PECMD_ReadFileStr(buf, &ftmp);
        if (r == -2) {
            buf[len] = L'\0';                        /* 回退: 去掉扩展名再试 */
            WCHAR *s = NULL;
            FUN_1400637DC(&s, ".lang", -1, -1);
            FUN_14006375C((WCHAR **)&buf, s);
            PECMD_FreeStrBuf(&s);
            r = PECMD_ReadFileStr(buf, &ftmp);
        }
        if (r > 0) {
            src = (WCHAR *)ftmp;
            dst = (WCHAR *)ftmp;
            {
                /* 先找 #code= 行 */
                WCHAR *q = src;
                if (FUN_14005C788("#code=", q, 6) == 0) {
                    while (*q != L'\0' && *q != L'\r' && *q != L'\n') {
                        q++;
                    }
                    while (*q == L'\n' || *q == L'\r') {
                        q++;
                    }
                }
                src = q;
                p = src;
                /* #str: 段逐条解析, \r \n \\) 转义 */
                while (*p != L'\0' &&
                       (FUN_14005C788("#str:", p, 5) == 0)) {
                    WCHAR *line = p;
                    while (*line != L'\0' && *line != L'\r' && *line != L'\n') {
                        line++;
                    }
                    p += 5;
                    if ((WCHAR)(*p + 0xff00) < 10) {   /* 16 进制数字开头 */
                        while (p < line) {
                            if (*p == L'\\') {
                                WCHAR c1 = p[1];
                                if (c1 == L'r') {
                                    *dst++ = L'\r';
                                    p += 2;
                                } else if (c1 == L'n') {
                                    *dst++ = L'\n';
                                    p += 2;
                                } else {
                                    *dst++ = L'\\';
                                    if (c1 == L'\\') {
                                        p += 2;
                                    } else {
                                        p++;
                                    }
                                }
                            } else {
                                *dst++ = *p++;
                            }
                        }
                        *dst++ = L'\0';
                    }
                    /* 行尾 \r\n 清成 \0 */
                    while (*line == L'\n' || *line == L'\r') {
                        *line++ = L'\0';
                    }
                    p = line;
                }
                i = (int)(dst - src);
                PECMD_StrCopyW(&g_pLangBuf, src, i);
                g_langLen = (int64_t)i * 2;
            }
        }
        PECMD_FreeStrBuf((WCHAR **)&ftmp);
        PECMD_FreeStrBuf(&buf);
    }
    return (int64_t)(uintptr_t)g_pLangBuf;
}
