/* ====================================================================
 * core_b7b.c — B7 批次小函数 2 (1400c0000-1400dffff)
 *
 * 本批新实现函数直接使用人类可读 PECMD_ 名称，原始地址保留在 @0x 注释。
 *
 * 来源: PECMD原始.EXE (x64, ImageBase=0x140000000)
 *   查找控件对象       FUN_1400C36C8 @0x1400c36c8
 *   HOME 命令          FUN_1400C42AC @0x1400c42ac
 *   解析列表项         FUN_1400C99B4 @0x1400c99b4
 *   添加控件           FUN_1400CADEC @0x1400cadec
 *   绘制控件           FUN_1400D95F0 @0x1400d95f0
 *
 * 约定:
 *   - 新实现函数使用 PECMD_ 可读名；未实现依赖仍 extern FUN_ + TODO(verify)
 *   - 全局数据符号先用 extern 声明; 完整链接时统一在 core_globals.c 定义
 * ==================================================================== */
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>

#include "pecmd_defs.h"
extern WCHAR **FUN_14005B154(WCHAR **pp); /* @0x14005b154 */

/* ---- win32_stub.h 中尚未声明的 API (仅语法编译用) ---- */
extern BOOL IsIconic(HWND);
extern HDC GetWindowDC(HWND);
extern BOOL DrawIcon(HDC, int, int, HICON);

/* ---- 未实现依赖 (extern + TODO(verify)) ---- */
extern bool FUN_1400C1194(int64_t *pp, uint64_t *out);
extern WCHAR *PECMD_AllocStrSlot(WCHAR **ps);
extern void FUN_14007BF44(int64_t *ctx, WCHAR *name, void *out, int mode, uint8_t flag);
extern bool FUN_1400C11C0(int64_t *pp, int *out);
extern int64_t FUN_14007A224(void *script, LPCWSTR text, WCHAR **out, int c, int d);
extern WCHAR *FUN_14001BE14(WCHAR *s);
extern HKEY FUN_1400C13F8(HKEY script, HKEY args, char mode);
extern uint64_t FUN_1400C11F4(int64_t *pp, uint64_t *out);
extern bool PECMD_ParseUIntValue(int64_t *pp, int *out);
extern void FUN_1400F429C(WCHAR **pp, WCHAR ch);
extern void *PECMD_SendCtrlMessage_0834(WPARAM wParam, uint64_t lParam);
extern void FUN_14005DAF8(int64_t ctx, int *a, int *b, int *c, int *d);
extern uint64_t *FUN_1400C9B9C(uint64_t *obj, uint64_t a2, uint64_t a3, uint32_t a4,
                               uint64_t *a5, int32_t a6, int32_t a7, int32_t a8,
                               int32_t a9, uint64_t *a10, uint16_t *a11, LPCWSTR a12,
                               int64_t a13, int16_t *a14, LPCWSTR a15, uint64_t *a16,
                               uint64_t a17, uint64_t a18);
extern void FUN_14005D9A8(int64_t ctx, int mode);
extern char FUN_1400D5B48(int64_t obj, HDC hdc);
extern COLORREF FUN_1400E68E0(HDC hdc, RECT *rc, COLORREF color);
extern int64_t PECMD_ExecuteCommand(int64_t *ctx, LPCWSTR text, uint64_t a3,
                             LPCWSTR a4, uint32_t a5, int64_t a6, int a7);
extern void FUN_1400F0FA8(int64_t *obj, uint64_t wParam, uint64_t lParam);

/* ---- 本批引用的全局数据 ---- */
extern WCHAR g_szEmpty[];            /* g_szEmpty 空串 */

/* ========== FUN_1400C36C8 @0x1400c36c8 ==========
 * 按名称路径在控件对象表中查找对象；支持 "name:name:..." 多级路径。
 * 数字/括号串直接交给 UInt64 解析。
 */
uint64_t FUN_1400C36C8(LPCWSTR name, int64_t ctx, int filter)
{
    if (*name == L'\0') {
        return 0;
    }

    LPCWSTR s = name;
    if (((uint16_t)*s < 0x30 || 0x39 < (uint16_t)*s) && *s != L'(') {
        for (;;) {
            int count = **(int **)(ctx + 0x1a8);
            LPCWSTR p = s;
            uint64_t len = 0;
            while (*p != L'\0' && *p != L':') {
                len++;
                p++;
            }

            uint64_t i = 0;
            int idx = 0;
            for (;;) {
                idx = (int)i;
                if (count - 1 <= idx) {
                    return 0;
                }
                int64_t entry = *(int64_t *)(**(int64_t **)(ctx + 0x1a0) + i * 8);
                LPCWSTR cand = NULL;
                if ((filter < 1 || *(int *)(entry + 8) == filter) &&
                    (cand = *(LPCWSTR *)(entry + 0x10), cand != NULL &&
                     StrCmpNIW(s, cand, (int)len) == 0 && cand[len] == L'\0')) {
                    break;
                }
                i++;
            }

            if (s[len] == L'\0') {
                return *(uint64_t *)(**(int64_t **)(ctx + 0x1a0) + (int64_t)idx * 8);
            }
            s = s + len + 1;
            if (*s == L'\0') {
                return *(uint64_t *)(**(int64_t **)(ctx + 0x1a0) + (int64_t)idx * 8);
            }
            ctx = *(int64_t *)(*(int64_t *)(**(int64_t **)(ctx + 0x1a0) + i * 8) + 0x38);
        }
    }

    LPCWSTR p = name;
    uint64_t value = 0;
    FUN_1400C1194((int64_t *)&p, &value);
    return value;
}

/* ========== FUN_1400C42AC @0x1400c42ac ==========
 * HOME 命令：设置 IE 主页，并可选设置 HomePage/DisableRegistryTools 策略。
 * 参数格式: "主页[,HomePage策略[,DisableRegistryTools策略]]"。
 */
uint64_t FUN_1400C42AC(HKEY script, LPCWSTR args)
{
    LPCWSTR s = args;
    FUN_14005B154((WCHAR **)&s);

    WCHAR *p1 = StrRChrW(s, NULL, L',');
    if (p1 != NULL) {
        *p1 = L'\0';
        p1++;
    }
    WCHAR *p2 = StrRChrW(s, NULL, L',');
    if (p2 != NULL) {
        *p2 = L'\0';
        p2++;
    }

    int disableRegTools = 0;
    int disableHomePage = 0;
    WCHAR *expanded = NULL;
    PECMD_AllocStrSlot(&expanded);

    if (p2 != NULL) {
        *expanded = L'\0';
        FUN_14007BF44((int64_t *)script, p1, &expanded, 0, 1);
        WCHAR *p = expanded;
        FUN_1400C11C0((int64_t *)&p, &disableRegTools);
        p1 = p2;
    }

    if (p1 != NULL) {
        *expanded = L'\0';
        FUN_14007BF44((int64_t *)script, p1, &expanded, 0, 1);
        WCHAR *p = expanded;
        FUN_1400C11C0((int64_t *)&p, &disableHomePage);
    }

    WCHAR *res = NULL;
    FUN_1400702B0(&res, WSTR("$HKCU\\Software\\Microsoft\\Internet Explorer\\Main\\Start Page="));
    *expanded = L'\0';
    FUN_14007A224((void *)(uintptr_t)script, s, &expanded, 0, 0);
    WCHAR *value = FUN_14001BE14(expanded);
    FUN_14006375C(&res, value);
    FUN_1400C13F8(script, (HKEY)(uintptr_t)res, '\x01');

    FUN_1400702B0(&res,
        WSTR("#HKCU\\Software\\Policies\\Microsoft\\Internet Explorer\\Control Panel\\HomePage="));
    LPCWSTR homeValue = g_szEmpty;
    if (disableHomePage != 0) {
        homeValue = WSTR("1");
    }
    FUN_14006375C(&res, homeValue);
    FUN_1400C13F8(script, (HKEY)((uintptr_t)res + (disableHomePage == 0) * 2), '\0');

    FUN_1400702B0(&res,
        WSTR("#HKCU\\Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\System\\DisableRegistryTools="));
    LPCWSTR regValue = g_szEmpty;
    if (disableRegTools != 0) {
        regValue = WSTR("1");
    }
    FUN_14006375C(&res, regValue);
    FUN_1400C13F8(script, (HKEY)((uintptr_t)res + (disableRegTools == 0) * 2), '\0');

    FUN_14005B104(&res);
    FUN_14005B104(&expanded);
    return 0;
}

/* ========== FUN_1400C99B4 @0x1400c99b4 ==========
 * 解析一个列表项/控件项：可选 #id[#id2]、修饰符 (=, +, *)、
 * 数值/类型、以及分隔符后的文本。
 * 注: mode 在反编译中同时被用作 8 字节暂存槽，这里保留该行为。
 */
int FUN_1400C99B4(int64_t item, int64_t *pp, int64_t *out, int *id,
                        char *mode, uint32_t *value, int16_t sep)
{
    char *pcVar3 = mode;
    char cVar1 = *mode;
    *mode = '\0';
    *id = -0x80000000;
    *(uint32_t *)(item + 4) = 0;
    int result = -0x80000000;
    *(uint32_t *)&mode = 0x32;

    WCHAR *p = (WCHAR *)*pp;
    if (*p == L'#') {
        p++;
        *pp = (int64_t)p;
        FUN_1400C11C0(pp, &result);
        p = (WCHAR *)*pp;
        if (*p == L'#') {
            p++;
            *pp = (int64_t)p;
            FUN_1400C11C0(pp, id);
            p = (WCHAR *)*pp;
        }
        if (*p == L':') {
            p++;
            *pp = (int64_t)p;
        }
    }

    for (;;) {
        WCHAR ch = *(WCHAR *)*pp;
        if (ch == L'=') {
            *(uint32_t *)(item + 4) = (*(uint32_t *)(item + 4) << 8) | 0x32;
        } else if (ch == L'+') {
            *(uint32_t *)(item + 4) = (*(uint32_t *)(item + 4) << 8) | 0x31;
        } else if (ch != L'*') {
            if (*(uint32_t *)(item + 4) == 0) {
                *(uint32_t *)(item + 4) = 0x30;
            }
            PECMD_ParseUIntValue(pp, (int *)&mode);
            uint32_t *puVar4 = value;
            *value = 0xffffffff;
            while (*(WCHAR *)*pp == L'/') {
                *pp = (int64_t)((WCHAR *)*pp + 1);
                uint64_t chunk = 0;
                uint64_t r = FUN_1400C11F4(pp, &chunk);
                if (0 < (int)r) {
                    *puVar4 = (*puVar4 << 0x10) | (uint32_t)(uint16_t)chunk;
                }
            }
            if ((int16_t)*puVar4 < 0) {
                *(uint16_t *)puVar4 = 2;
            }
            if (*(WCHAR *)*pp == L':') {
                *pp = (int64_t)((WCHAR *)*pp + 1);
            }
            *(uint32_t *)(item + 8) = *(uint32_t *)&mode;

            if (cVar1 == '\0') {
                LPCWSTR text = (LPCWSTR)*pp;
                FUN_1400F429C((WCHAR **)pp, (WCHAR)sep);
                PECMD_StrCopyW((WCHAR **)out, text,
                                 (int64_t)((WCHAR *)*pp - text));
                if (*(WCHAR *)*pp != L'\0') {
                    *pp = (int64_t)((WCHAR *)*pp + 1);
                }
            } else {
                FUN_1400703E4((WCHAR **)out, (LPCWSTR)*pp);
            }
            *(int64_t *)(item + 0x10) = *out;
            return result;
        } else {
            *(uint32_t *)(item + 4) = (*(uint32_t *)(item + 4) << 8) | 0x30;
            *pcVar3 = '\x10';
        }
        *pp = (int64_t)((WCHAR *)*pp + 1);
    }
}

/* ========== FUN_1400CADEC @0x1400cadec ==========
 * 在控件列表尾部添加一个通用控件对象。
 */
uint64_t FUN_1400CADEC(WPARAM mgr, uint64_t a2, uint64_t a3,
                          uint64_t *a4, int a5, int a6, int a7,
                          int a8, uint64_t *a9, uint16_t *a10,
                          LPCWSTR a11, int64_t a12, int16_t *a13,
                          LPCWSTR a14, uint64_t a15, uint64_t a16)
{
    uint64_t *slot = (uint64_t *)PECMD_SendCtrlMessage_0834(mgr,
                                               (uint64_t)(uintptr_t)g_szEmpty);
    uint64_t *obj = NULL;
    if (slot == NULL) {
        return 0;
    }

    int64_t base = **(int64_t **)((uint8_t *)mgr + 0x1a0);
    FUN_14005DAF8((int64_t)mgr, &a5, &a6, &a7, &a8);
    uint64_t *mem = (uint64_t *)malloc(0x78);
    if (mem != NULL) {
        obj = FUN_1400C9B9C(mem, a2, a3,
                            (uint32_t)(((int64_t)slot - base) >> 3) + 0x1000,
                            a4, (int32_t)a5, (int32_t)a6,
                            (int32_t)a7, (int32_t)a8, a9, a10,
                            a11, a12, a13, a14, slot, a15,
                            a16);
    }
    *slot = (uint64_t)obj;
    FUN_14005D9A8((int64_t)mgr, 0);
    return *slot;
}

/* ========== FUN_1400D95F0 @0x1400d95f0 ==========
 * 控件 WM_PAINT 处理：普通状态绘制背景/标题，最小化状态绘制图标。
 */
void FUN_1400D95F0(int64_t *self, uint64_t wParam, uint64_t lParam)
{
    if (IsIconic((HWND)self[4]) == 0) {
        if ((*(uint8_t *)(self + 0x4a) & 1) != 0) {
            HDC hdc = GetWindowDC((HWND)self[4]);
            FUN_1400D5B48((int64_t)self, hdc);
            *(uint8_t *)(self + 0x4a) = *(uint8_t *)(self + 0x4a) & 0xfe;
        }

        int16_t title = *(int16_t *)self[0x44];
        if ((-1 < (int)self[0x55]) || (title != 0)) {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint((HWND)self[4], &ps);
            RECT rc;
            rc.left = 0;
            rc.top = 0;
            rc.right = 0;
            rc.bottom = 0;
            GetClientRect((HWND)self[4], &rc);
            if (-1 < (int)*(COLORREF *)(self + 0x55)) {
                FUN_1400E68E0(hdc, &rc, *(COLORREF *)(self + 0x55));
            }
            if (title != 0) {
                WCHAR buf[304];
                wsprintfW(buf, WSTR("%.260s 0x%p %ld  %ld"),
                          (LPCWSTR)self[0x44], (void *)0, 0L, 0L);
                PECMD_ExecuteCommand((int64_t *)self[0x52], buf,
                              (uint64_t)(uintptr_t)self, NULL, 0, 0, 0);
            }
            EndPaint((HWND)self[4], &ps);
        }
        FUN_1400F0FA8(self, wParam, lParam);
        return;
    }

    PAINTSTRUCT ps;
    HDC hdc = BeginPaint((HWND)self[4], &ps);
    SendMessageW((HWND)self[4], 0x27, (WPARAM)hdc, 0);
    int cx = GetSystemMetrics(0xb);
    int cy = GetSystemMetrics(0xc);
    RECT rc;
    rc.left = 0;
    rc.top = 0;
    rc.right = 0;
    rc.bottom = 0;
    GetClientRect((HWND)self[4], &rc);
    DrawIcon(hdc, ((rc.right - rc.left) - cx + 1) / 2,
             ((rc.bottom - rc.top) - cy + 1) / 2, (HICON)self[0x151]);
    EndPaint((HWND)self[4], &ps);
}
