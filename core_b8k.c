/* ====================================================================
 * core_b8k.c — B8 中批 3 (1400e0000-1400fffff), 前半
 *
 * 本批新实现函数全部使用人类可读 PECMD_ 名称，原始地址保留在 @0x 注释。
 *
 * 来源: PECMD原始.EXE (x64, ImageBase=0x140000000)
 *   建进程并读远程内存     FUN_1400E4324 @0x1400e4324
 *   追加菜单位图项          FUN_1400E4F14   @0x1400e4f14
 *   弹出上下文菜单          FUN_1400E54D4       @0x1400e54d4
 *   创建对话框窗口          FUN_1400E6574        @0x1400e6574
 *   注入式建进程            FUN_1400E7414   @0x1400e7414
 *   构建钩子键列表          FUN_1400E8748       @0x1400e8748
 *   布局子窗口              FUN_1400EC0F0     @0x1400ec0f0
 *   像素缩放                FUN_1400ED7A0       @0x1400ed7a0
 *   GDI 消息分发            FUN_1400EEC28         @0x1400eec28
 *   控件消息分发(扩展)      FUN_1400EF694   @0x1400ef694
 *   创建静态子控件对象      FUN_1400EF91C      @0x1400ef91c
 *   创建按钮控件核心        FUN_1400EFB08   @0x1400efb08
 *   通用 GDI 消息分发       FUN_1400F0814  @0x1400f0814
 *   合成半透明位图          FUN_1400F0ABC     @0x1400f0abc
 *   绘制控件(带背景)       FUN_1400F0FA8      @0x1400f0fa8
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
typedef struct {
    void    *BaseAddress;      /* +0x00 */
    void    *AllocationBase;   /* +0x08 */
    uint32_t AllocationProtect;/* +0x10 */
    uint16_t PartitionId;      /* +0x14 */
    size_t   RegionSize;       /* +0x18 */
    uint32_t State;            /* +0x20 */
    uint32_t Protect;          /* +0x24 */
    uint32_t Type;             /* +0x28 */
} PECMD_MBI;   /* x64 MEMORY_BASIC_INFORMATION 布局 (仅用 State/RegionSize) */
extern size_t VirtualQueryEx(HANDLE hProcess, const void *lpAddress, void *mbi,
                             size_t length);
extern BOOL SetMenuItemBitmaps(HMENU, UINT, UINT, HBITMAP, HBITMAP);
extern HWND CreateDialogParamW(HINSTANCE, LPCWSTR, HWND, void *dproc, intptr_t);
extern HRGN CreateEllipticRgn(int, int, int, int);
extern void *operator_new(size_t size);
extern int64_t PECMD_ContainerAppend(int64_t *container);
extern LPCWSTR FUN_1400E429C(uint64_t *obj); /* @0x1400e429c */
extern void *GetThreadContext_exref;
extern void *SetThreadContext_exref;
extern uint64_t FUN_1400F0814(int64_t obj, uint32_t msg, int64_t wParam,
                                            LPCWSTR lParam, int flag);

/* ---- 已实现公共工具 (pecmd_defs.h / core_*.c) ---- */
extern bool PECMD_ParseUIntValue(WCHAR **pp, int *out);             /* @0x140067d20 */
extern uint64_t PECMD_GetPackedSystemVersion(void);                     /* @0x14005ea5c */
extern void FUN_14005C898(LPCSTR name, LPCSTR dll, int64_t *out,
                                int64_t *hmod);               /* @0x14005c898 */
extern int32_t FUN_14005C7C4(const char *a, const WCHAR *w); /* @0x14005c7c4 */
extern void FUN_1400669C4(void *script, int64_t value, LPCWSTR key); /* @0x1400669c4 */
extern void FUN_1400629B8(void *script, LPCWSTR key, LPCWSTR value);  /* @0x1400629b8 */
extern WCHAR **FUN_14005B154(WCHAR **pp);                   /* @0x14005b154 */
extern void *FUN_1400E57C0(void *obj);                 /* @0x1400e57c0 */
extern uint64_t *FUN_1400F5C74(uint64_t *obj);        /* @0x1400f5c74 */
extern int64_t *FUN_140063B64(int64_t *arr);                /* @0x140063b64 */
extern bool FUN_1400EFFF8(int64_t *obj, LPCWSTR text, DWORD style,
                                       int *rect, HWND parent, uint32_t param6); /* @0x1400efff8 */
extern void FUN_1400E8940(void *obj);               /* @0x1400e8940 */
extern LRESULT FUN_1400E5890(int64_t obj);             /* @0x1400e5890 */
extern void FUN_1400E5248(int64_t node, uint16_t *pId, HMENU menu,
                                int64_t script, int64_t cmdCtx, int64_t *varTable); /* @0x1400e5248 */
extern void FUN_1400F0DF4(int64_t obj, HDC hdc, RECT *rc, COLORREF color,
                                 int edge);                   /* @0x1400f0df4 */
extern void FUN_1400F0CA0(HWND hwnd, COLORREF color); /* @0x1400f0ca0 */
extern HWND FUN_1400E5788(HWND hwnd);             /* @0x1400e5788 */
extern int64_t FUN_140063B00(int64_t idx, int64_t *arr, int64_t *cap, uint32_t esize); /* @0x140063b00 */

/* ---- 未实现依赖 (extern + TODO(verify)) ---- */
extern int FUN_140101e04(LPCWSTR cmd, LPWSTR buf, void *sa, void *da, BOOL inherit,
                         uint32_t flags, LPVOID env, LPCWSTR cwd,
                         STARTUPINFOW *si, PROCESS_INFORMATION *pi);
extern int FUN_1400E7124(void *p1, uint32_t p2, int64_t *p3, int64_t *p4,
                                int64_t *p5, size_t *p6);   /* @0x1400e7124 */
extern int FUN_1400E4480(LPWSTR p1, int64_t p2, int64_t p3, LPCVOID p4,
                                          uint32_t p5, BOOL p6, uint32_t p7, LPVOID p8,
                                          LPCWSTR p9, STARTUPINFOW *p10,
                                          PROCESS_INFORMATION *p11, LPCWSTR p12); /* @0x1400e4480 */
extern HBITMAP PECMD_LoadImageBitmap(LPCWSTR txt, int64_t *outw, int64_t h, int64_t w,
                             uint32_t *param5, uint64_t *param6, int param7);
extern void FUN_1400E537C(int64_t node, uint64_t id, int64_t *out, int64_t *str);
extern int64_t FUN_14004c0bc(int64_t p1, int64_t p2, int64_t *p3, int64_t *p4, void *p5);
extern void PECMD_FreeInitObjectList(int64_t p1);
extern uint64_t PECMD_ParseHotkeyCode(int64_t *ps, uint32_t *flags, uint64_t p3, char p4);
extern void FUN_140025B10(uint64_t p1, uint64_t p2);
extern uint16_t FUN_1400F172C(int64_t *map, int msg, uint64_t wParam, uint64_t *lParam,
                              int64_t hwnd, uint8_t mode, uint64_t *out);
extern int64_t FUN_1400E5B0C(int64_t obj, uint64_t p2, int64_t p3, int64_t *p4);
extern void PECMD_SetControlText(HWND hwnd, int64_t p2, LPCWSTR p3, int p4);
extern void FUN_1400EF654(int64_t p1);
extern void PECMD_RestoreForegroundWindow(void);
extern uint64_t *FUN_1400EFEC8(uint64_t *mem);
extern void FUN_1400E5730(HWND hwnd, int64_t *out);
extern void FUN_140063620(uint64_t *ps);
extern void PECMD_SelectObjectSlot_b028(uint64_t *slot, HDC hdc, HGDIOBJ obj);
extern void PECMD_RestoreAndDeleteObject(uint64_t *slot);
extern uint8_t *FUN_14001d78c(uint8_t *dst, uint8_t *src, int n);
extern int wsprintfW(LPWSTR buf, LPCWSTR fmt, ...);

/* ---- 本批引用的虚表/数据符号 (.rdata) ---- */
extern WCHAR g_szEmpty[];
extern float g_alphaThreshold;   /* DAT_14012f6bc 透明阈值浮点 */
extern float g_paintScale2;      /* DAT_14012c0d4 绘制缩放除数 (TODO(verify)) */

/* ---- 全局数据 (core_globals.c 定义) ---- */
extern uint32_t (*g_pfnVirtualAllocEx)(void);  /* DAT_14013e258 VirtualAllocEx */
extern uint32_t (*g_pfnGetThreadCtx)(HANDLE, int64_t);      /* DAT_14013e248 GetThreadContext */
extern uint32_t (*g_pfnSetThreadCtx)(HANDLE, int64_t);      /* DAT_14013e250 SetThreadContext */
extern uint32_t (*g_pfnIsAppThemed)(void);                  /* DAT_14013e238 IsAppThemed */
extern uint64_t g_remoteTebBase;                            /* DAT_14013e2f9 远程 TEB 基址 */
extern uint64_t g_ptrA84C;                                  /* DAT_14013a84c 布局指针 */
extern uint8_t g_objMode;                                   /* DAT_14013a84d */
extern uint64_t g_savedVtblSlot;                            /* DAT_14013e2c8 保存的虚表槽 */
extern void *g_pHookData;                                   /* DAT_14013d078 钩子数据 */
extern HHOOK g_hHook;                                       /* DAT_14013d068 钩子句柄 */
extern HWND g_hActiveDevWnd;                                /* DAT_14013e400 */
extern HGDIOBJ g_hStockWhiteBrush;                          /* DAT_14013a858 */
extern uint8_t g_tooltipThreshold;                          /* DAT_14013a861 */
extern uint8_t g_tooltipCount0;                             /* DAT_14013a860 */

/* ========== FUN_1400E4324 @0x1400e4324 ==========
 * 创建进程后读取远程线程上下文指向的内存区域尺寸。
 *   - 写 0x10000b 到远程 TEB 槽, 调 GetThreadContext
 *   - 按 g_objMode 取 PEB 数组, ReadProcessMemory 读指针
 *   - VirtualQueryEx 向后扫描直到释放区, 计算区域大小 -> *sizeOut
 * TODO(verify): 远程布局细节。
 */
uint32_t FUN_1400E4324(LPWSTR cmd, int64_t ctxOff, int64_t *outPtr,
                                       int64_t *outSize, BOOL inherit, uint32_t flags,
                                       LPVOID env, LPCWSTR cwd, STARTUPINFOW *si,
                                       PROCESS_INFORMATION *pi, LPCWSTR param11)
{
    size_t read;
    LPCVOID addr;
    PECMD_MBI mbi;
    uint64_t base;
    int ok;
    uint32_t r = 0;

    base = (uint64_t)g_remoteTebBase;
    read = 0;
    ok = FUN_140101e04(param11, cmd, NULL, NULL, inherit, flags | 4,
                       env, cwd, si, pi);
    if (ok != 0) {
        *(uint32_t *)(g_ptrA84C + ctxOff) = 0x10000b;
        r = 1;
        g_pfnGetThreadCtx(pi->hThread, ctxOff);
        GetLastError();
        if (g_objMode == 0x04)
            base = *(uint32_t *)(base + ctxOff);
        else
            base = *(uint64_t *)(base + ctxOff);
        ReadProcessMemory(pi->hProcess, (LPCVOID)(g_objMode * 2 + base),
                          (LPVOID)outPtr, (size_t)g_objMode, &read);
        addr = (LPCVOID)*outPtr;
        for (;;) {
            size_t n = VirtualQueryEx(pi->hProcess, addr, &mbi, 0x30);
            if (n == 0 || mbi.State == 0x10000)
                break;
            addr = (LPCVOID)((int64_t)addr + (int64_t)mbi.RegionSize);
        }
        *outSize = (int64_t)addr - *outPtr;
    }
    return r;
}

/* ========== FUN_1400E4F14 @0x1400e4f14 ==========
 * 追加菜单项并 (可选) 生成 "?*WxH" 加速提示位图 (SetMenuItemBitmaps)。
 * 位图尺寸取系统指标, 主题下 (IsAppThemed) 渲染使用系统色。
 */
void FUN_1400E4F14(HMENU menu, UINT flags, uintptr_t id, uint8_t *item)
{
    int iVar4 = 0;
    int w = 0, h = 0;
    uint32_t ux;
    LPCWSTR p;
    HBITMAP bmp;
    int64_t outw[2];

    if ((*item & 8) != 0)
        return;
    AppendMenuW(menu, flags, id, *(LPCWSTR *)(item + 8));

    p = *(LPCWSTR *)(item + 0x30);
    if (*p == L'\0')
        return;
    outw[0] = 0;
    for (; *p == L'?'; p++)
        iVar4++;
    if (*p == L'*') {
        while (*(p = p + 1) == L'?')
            iVar4++;
        PECMD_ParseUIntValue((WCHAR **)&p, &w);
        h = w;
        if (*p == L':') {
            p++;
            PECMD_ParseUIntValue((WCHAR **)&p, &h);
        }
        if (*p == L'*' || *p == L':')
            p++;
    }
    if (iVar4 != 0) {
        ux = (uint32_t)PECMD_GetPackedSystemVersion();
        if (ux >> 0x10 > 0x5ffff) {
            if (g_pfnIsAppThemed == NULL)
                FUN_14005C898("IsAppThemed", "UxTheme.dll",
                                    (int64_t *)&g_pfnIsAppThemed, NULL);
            uint8_t b = 0;
            if ((uint64_t)g_pfnIsAppThemed - 1 < (uint64_t)0xfffffffffffffffe)
                b = (uint8_t)((-(g_pfnIsAppThemed() != 0) & 0x10U) | 1);
            if (b == 0x11 && iVar4 != 2) {
                w = 0;
                h = 0;
                goto build;
            }
        }
        if (w == 0)
            w = GetSystemMetrics(0x47);
        if (h == 0)
            h = GetSystemMetrics(0x48);
    }
build:
    outw[1] = 0;
    bmp = PECMD_LoadImageBitmap(p, outw, (int64_t)w, (int64_t)h, (uint32_t *)(outw + 1),
                        NULL, 0);
    *(HBITMAP *)(item + 0x38) = bmp;
    if (bmp != 0)
        SetMenuItemBitmaps(menu, (UINT)id, 0, bmp, bmp);
}

/* ========== FUN_1400E54D4 @0x1400e54d4 ==========
 * 构建并弹出上下文菜单: 在临界区中建菜单树, TrackPopupMenu 跟踪,
 * 选中项 >0xff 时按 id 查命令, 写 &&__MemuId/&&__MenuInx 变量并执行。
 */
void FUN_1400E54D4(char *node, HWND hwnd, int64_t *info, int64_t msg,
                            int64_t flags)
{
    POINT pt;
    HMENU menu;
    uint16_t idseq[4];
    LARGE_INTEGER q;
    uint64_t sel;
    LPCWSTR str = NULL;
    int64_t *slot;

    (void)msg;
    if (!(0x202 < msg && msg < 0x205))
        return;

    menu = CreatePopupMenu();
    idseq[0] = 0x100;
    EnterCriticalSection(&g_csInit);
    FUN_1400E5248((int64_t)node, idseq, menu, 0, 0, NULL);
    LeaveCriticalSection(&g_csInit);

    slot = (int64_t *)info[3];
    pt.x = 0;
    pt.y = 0;
    GetCursorPos(&pt);
    if (slot == 0) {
        /* 用当前光标 */
    }
    else {
        POINT *pp = (POINT *)slot;
        pt = *pp;
        info = (int64_t *)info[7];
    }
    SetForegroundWindow(hwnd);
    if (flags < 0)
        flags = 0x28;
    sel = TrackPopupMenu(menu, (uint32_t)flags | 0x100, pt.x, pt.y, 0, hwnd, NULL);
    PostMessageW(hwnd, 0, 0, 0);

    q.QuadPart = 0;
    FUN_140063620((uint64_t *)&str);
    if (sel > 0xff) {
        EnterCriticalSection(&g_csInit);
        FUN_1400E537C((int64_t)node, sel & 0xffff, &q.QuadPart, (int64_t *)&str);
        LeaveCriticalSection(&g_csInit);
    }
    if (q.QuadPart != 0) {
        if (FUN_14005C7C4("***", (const WCHAR *)(uintptr_t)q.QuadPart) == 0) {
            FUN_1400669C4(info, (int64_t)(uint64_t)sel, WSTR("&&__MemuId"));
            FUN_1400629B8(info, WSTR("&&__MenuInx"), str);
            FUN_14004c0bc((int64_t)info, q.QuadPart, NULL, NULL, NULL);
        }
        else if (info[3] == 0) {
            PECMD_FreeInitObjectList((int64_t)node);
        }
    }
    DestroyMenu(menu);
    FUN_14005B104((WCHAR **)&str);
}

/* ========== FUN_1400E6574 @0x1400e6574 ==========
 * 创建对话框/自定义窗口: 优先 CreateDialogParamW, 失败退回
 * CreateWindowExW "#32770"; 保存虚表槽到 g_savedVtblSlot。
 */
bool FUN_1400E6574(int64_t *obj, uint32_t id, int64_t parent)
{
    DWORD style = 0xcf0000;
    DWORD exStyle = 0x40000;
    HWND h;

    if (id > 0xffff) {
        style = 0x50000000;
        id = 0;
        exStyle = 0;
    }
    if (id == 0)
        id = *(uint32_t *)(obj + 0x20);
    if (parent != 0)
        obj[0x23] = parent;
    if (obj[0x23] == 0)
        obj[0x23] = (int64_t)(uintptr_t)GetDesktopWindow();
    if (id == 0x271c) {
        style = 0x80000080;
        exStyle = 0x80;
    }
    else if (id == 0x271d) {
        style = 0x80000000;
        exStyle = 0;
    }

    h = CreateDialogParamW(g_hInstance, (LPCWSTR)(uintptr_t)id, (HWND)obj[0x23],
                           NULL, 0);
    obj[4] = (int64_t)(uintptr_t)h;
    if (h == 0) {
        h = CreateWindowExW(exStyle, WSTR("#32770"), (LPCWSTR)g_szEmpty, style,
                            -0x80000000, 0, -0x80000000, 0, (HWND)obj[0x23],
                            (HMENU)0, g_hInst, NULL);
        obj[4] = (int64_t)(uintptr_t)h;
    }
    if (obj[0x23] == 0) {
        obj[0x23] = (int64_t)(uintptr_t)GetParent((HWND)obj[4]);
    }
    PECMD_ContainerAppend((int64_t *)obj);
    g_savedVtblSlot = *(uint64_t *)(*obj + 8);
    return obj[4] != 0;
}

/* ========== FUN_1400E7414 @0x1400e7414 ==========
 * 通过远程内存注入方式创建进程: 加载 VirtualAllocEx/线程上下文 API,
 * 在目标进程分配内存写入代码, 然后创建远程线程执行。
 * TODO(verify): 注入细节 (FUN_1400E7124/1400e4480)。
 */
int FUN_1400E7414(uint16_t *p1, uint32_t p2, LPWSTR p3, BOOL p4,
                                uint32_t p5, LPVOID p6, LPCWSTR p7,
                                STARTUPINFOW *p8, PROCESS_INFORMATION *p9, LPCWSTR p10)
{
    HMODULE hMod;
    int64_t local_28 = 0;
    int64_t local_10 = 0;
    LPCVOID local_20 = NULL;
    size_t local_18 = 0;
    int64_t saved;
    uint64_t r;
    size_t dwSize;
    int rc = 0;

    if (g_pfnVirtualAllocEx == NULL) {
        hMod = GetModuleHandleA("Kernel32.dll");
        g_pfnVirtualAllocEx = (uint32_t (*)(void))GetProcAddress(hMod, "VirtualAllocEx");
    }
    g_pfnGetThreadCtx = (uint32_t (*)(HANDLE, int64_t))(uintptr_t)&GetThreadContext_exref;
    g_pfnSetThreadCtx = (uint32_t (*)(HANDLE, int64_t))(uintptr_t)&SetThreadContext_exref;

    r = (uint64_t)FUN_1400E7124(p1, p2, &local_28, &local_10, (int64_t *)&local_20,
                                        (size_t *)&local_18);
    saved = local_28;
    if ((int)r != 0) {
        local_28 = 0;
        if (p10 == NULL)
            p10 = FUN_1400E429C((uint64_t *)&local_28);
        dwSize = local_18;
        rc = FUN_1400E4480(p3, saved, local_10, local_20, (uint32_t)local_18, p4,
                           p5, p6, p7, p8, p9, p10);
        VirtualFree((LPVOID)local_20, dwSize, 0x4000);
        FUN_14005B104((WCHAR **)&local_28);
    }
    return rc;
}

/* ========== FUN_1400E8748 @0x1400e8748 ==========
 * 解析按键串 (空格/制表分隔) 为钩子键表 (紧凑 ushort 数组), 装入全局钩子数据;
 * 先替换旧数据。返回键数 (0xffffffff=空, 0xfffffffe=内存不足)。
 */
uint64_t FUN_1400E8748(uint16_t *text, uint64_t p2)
{
    uint16_t buf[1008];
    uint16_t *p;
    uint16_t ch;
    uint16_t flags;
    uint64_t cnt = 0;
    uint32_t tokFlags;
    uint8_t *mem = NULL;
    int i;

    p = text;
    FUN_14005B154((WCHAR **)&p);
    if (*p != 0) {
        do {
            if (cnt > 999)
                break;
            tokFlags = 0;
            flags = 0;
            uint16_t *tok = p;
            for (ch = *p; ch != 0; p++) {
                if ((ch > 8 && ch < 0xe) || ch == 0x20) {
                    if (*p != 0) {
                        *p = 0;
                        p++;
                    }
                    break;
                }
            }
            FUN_14005B154((WCHAR **)&p);
            uint64_t v = PECMD_ParseHotkeyCode((int64_t *)&tok, &tokFlags, p2, 0);
            if ((int)v < 1)
                break;
            if ((tokFlags & 1) != 0)
                flags |= 0x100;
            if ((tokFlags & 2) != 0)
                flags |= 0x200;
            if ((tokFlags & 4) != 0)
                flags |= 0x400;
            buf[cnt + 1] = (uint16_t)((v & 0xff) | flags);
            cnt++;
        } while (*p != 0);

        if (cnt == 0)
            return 0xffffffff;
        buf[cnt] = 0;
        i = (int)cnt + 4 + (uint32_t)cnt;
        buf[0] = (uint16_t)cnt;
        mem = HeapAlloc(GetProcessHeap(), 0, (uint64_t)(int64_t)i);
        if (mem == NULL)
            return 0xfffffffe;
        FUN_14001d78c(mem, (uint8_t *)buf, i);
    }

    EnterCriticalSection(&g_csInit);
    EnterCriticalSection(&g_csHook);
    void *old = g_pHookData;
    g_pHookData = mem;
    if (g_hHook == 0)
        FUN_140025B10(0, 0);
    LeaveCriticalSection(&g_csHook);
    LeaveCriticalSection(&g_csInit);
    if (old != 0)
        HeapFree(GetProcessHeap(), 0, old);
    return cnt;
}

/* ========== FUN_1400EC0F0 @0x1400ec0f0 ==========
 * 计算并应用子窗口布局: 由系统度量与父窗口位置得出目标矩形,
 * 变化时 SetWindowPos 全部子窗口并失效父窗口对应区域。
 */
void FUN_1400EC0F0(int64_t obj, char track)
{
    RECT wndRc, cli;
    POINT ofs;
    uint64_t *child;
    int smCx, smCy, smCx2, smCy2;
    int a, b, c, d;
    int n = 0;

    wndRc.left = wndRc.top = wndRc.right = wndRc.bottom = 0;
    cli.left = cli.top = cli.right = cli.bottom = 0;
    a = b = c = d = 0;
    ofs.x = ofs.y = 0;

    GetWindowRect(*(HWND *)((uint8_t *)obj + OBJ_HWND), &wndRc);
    GetParent(*(HWND *)((uint8_t *)obj + OBJ_HWND));
    ClientToScreen(GetParent(*(HWND *)((uint8_t *)obj + OBJ_HWND)), &ofs);
    smCx = GetSystemMetrics(5);
    smCy = GetSystemMetrics(6);
    smCx2 = GetSystemMetrics(0x2d);
    smCy2 = GetSystemMetrics(0x2e);
    GetClientRect(*(HWND *)((uint8_t *)obj + OBJ_HWND), &cli);
    SendMessageW(*(HWND *)((uint8_t *)obj + OBJ_HWND), 0x130a, 0, (LPARAM)&a);

    if (track == 0) {
        int w = a;
        int h = d + smCy;
        c = w;
        b = h;
    }
    else {
        int w = c + smCx;
        int h = b;
        c = w;
        b = h;
    }
    int ax, ay, aw, ah;
    aw = (cli.right - smCx2 - smCx) - c;
    ax = c + (wndRc.left - ofs.x);
    ah = (cli.bottom - smCy2 - smCy) - b;
    ay = b + (wndRc.top - ofs.y);

    if (*(int *)((uint8_t *)obj + 0xf8) != ax ||
        *(int *)((uint8_t *)obj + 0xfc) != ay ||
        *(int *)((uint8_t *)obj + 0x100) != aw ||
        *(int *)((uint8_t *)obj + 0x104) != ah) {
        child = *(uint64_t **)((uint8_t *)obj + 0x108);
        *(int *)((uint8_t *)obj + 0xf8) = ax;
        *(int *)((uint8_t *)obj + 0xfc) = ay;
        *(int *)((uint8_t *)obj + 0x100) = aw;
        *(int *)((uint8_t *)obj + 0x104) = ah;
        if (0 < *(int64_t *)((uint8_t *)obj + 0x118)) {
            do {
                SetWindowPos(*(HWND *)(*(uint64_t *)*child + OBJ_HWND), 0, ax, ay, aw, ah, 4);
                n++;
                child++;
            } while ((int64_t)n < *(int64_t *)((uint8_t *)obj + 0x118));
        }
        OffsetRect(&wndRc, -ofs.x, -ofs.y);
        InvalidateRect(GetParent(*(HWND *)((uint8_t *)obj + OBJ_HWND)), &wndRc, 1);
    }
}

/* ========== FUN_1400ED7A0 @0x1400ed7a0 ==========
 * 最邻近均值缩放: 把 src..+w × srcH..+h 区域按比例采样,
 * 每个目标像素取块内 RGB 平均后 SetPixel 写出。业务: 缩略图/放大预览。
 * TODO(verify): 边界与整除语义。
 */
void FUN_1400ED7A0(HDC hdcDst, int x0, uint32_t wFor, int x1, int y1,
                            HDC hdcSrc, int sx, int sy, int scaleX, int scaleY)
{
    int64_t step = (int64_t)(scaleX / x1);
    int64_t vstep = step * (int64_t)(scaleY / y1);
    int totalW = (int)wFor + y1;
    uint64_t rowBase = (uint64_t)(int)wFor;
    uint64_t base = rowBase;
    int i;

    for (i = (int)wFor; i < totalW; i++) {
        if ((int64_t)x0 < (int64_t)(x0 + x1)) {
            uint64_t x = (uint64_t)sx;
            int64_t cur = x0;
            do {
                x += (uint64_t)step;
                uint64_t y = (uint64_t)(((i - (int)base) * (int64_t)scaleY) / y1 + sy);
                int rSum = 0, gSum = 0, bSum = 0;
                int n = 0;
                int64_t xEnd = (int64_t)x + (int64_t)step;
                if ((int64_t)y < (int64_t)(y + (uint64_t)(int64_t)scaleY)) {
                    for (; (int64_t)y < (int64_t)xEnd; y++) {
                        uint64_t px = x;
                        int r2 = 0, g2 = 0, b2 = 0;
                        for (; (int64_t)px < (int64_t)xEnd; px++) {
                            COLORREF c = GetPixel(hdcSrc, (int)px, (int)y);
                            r2 += (int)(c & 0xff);
                            b2 += (int)(c >> 0x10 & 0xff);
                            g2 += (int)(c >> 8 & 0xff);
                        }
                        n += r2;
                        (void)n;
                        rSum = g2;
                        gSum = b2;
                    }
                }
                SetPixel(hdcDst, (int)cur, i,
                         ((uint32_t)((int64_t)gSum / vstep) & 0xff) << 8 |
                         ((uint32_t)((int64_t)bSum / vstep) & 0xff) << 0x10 |
                         ((uint32_t)((int64_t)rSum / vstep) & 0xff));
                cur++;
                x += (uint64_t)step;
            } while (cur < x0 + x1);
            base = (uint64_t)wFor;
        }
    }
}

/* ========== FUN_1400EEC28 @0x1400eec28 ==========
 * GDI/控件消息分发: 0x45a 走专用; 0xf0/0xf1 切换状态; 0xbd11 特殊; 其余
 * 0x233 查映射, 未中走通用 FUN_1400F0814。
 */
uint64_t FUN_1400EEC28(int64_t obj, uint32_t msg, int64_t wParam, LPCWSTR lParam)
{
    int count;
    uint16_t r;
    uint64_t out = 0;
    HWND top;
    bool generic = true;
    uint64_t res;

    if (msg == 0x45a) {
        PECMD_SetControlText(*(HWND *)((uint8_t *)obj + 8), wParam, lParam, 1);
        res = 1;
        return res;
    }
    if (msg == 0xf0) {
        generic = false;
        res = (uint64_t)(int64_t)*(int8_t *)((uint8_t *)obj + 0x3a);
    }
    else {
        res = 0;
        if (msg == 0xf1) {
            *(uint8_t *)((uint8_t *)obj + 0x3a) = wParam != 0;
            InvalidateRect(*(HWND *)((uint8_t *)obj + 8), NULL, 1);
        }
        else if (msg == 0xbd11 && *(int8_t *)((uint8_t *)obj + 0x36) == 3 &&
                 (int16_t)(wParam >> 0x10) == 0) {
            bool toggle = *(uint8_t *)((uint8_t *)obj + 0x3a) == 0;
            *(uint8_t *)((uint8_t *)obj + 0x3a) = toggle;
            SendMessageW(*(HWND *)((uint8_t *)obj + 8), 0xf1, toggle, 0);
            res = 0;
        }
    }

    top = 0;
    count = *(int *)(*(int64_t *)((uint8_t *)obj + 0x34) + 0x10);
    if (0 < count)
        top = FUN_1400E5788(*(HWND *)((uint8_t *)obj + 8));
    if (top != 0)
        SendMessageW(top, 0x450, 4, 0x5aa555aa);
    if (0 < count) {
        out = 0;
        r = FUN_1400F172C(*(int64_t **)((uint8_t *)obj + 0x34), msg, (uint64_t)wParam,
                          (uint64_t *)lParam, *(int64_t *)((uint8_t *)obj + 8), 1, &out);
        if ((r & 4) != 0) {
            if (top == 0)
                return out;
            PostMessageW(top, 0x451, 0x204, -0x55aaa55b);
            return out;
        }
    }
    if (generic)
        res = FUN_1400F0814(obj, msg, wParam, lParam, 0);
    if (0 < count)
        FUN_1400F172C(*(int64_t **)((uint8_t *)obj + 0x34), msg, (uint64_t)wParam,
                      (uint64_t *)lParam, *(int64_t *)((uint8_t *)obj + 8), 2, NULL);
    if (top != 0)
        PostMessageW(top, 0x451, 0x204, -0x55aaa55b);
    return res;
}

/* ========== FUN_1400EF694 @0x1400ef694 ==========
 * 控件消息分发(扩展): 处理 0x202/0x30/0x45a/0x113/0x0a 特殊逻辑后
 * 走 0x233 映射表 + 通用 FUN_1400F0814。
 */
uint64_t FUN_1400EF694(int64_t obj, uint32_t msg, int64_t wParam, LPCWSTR lParam)
{
    int count;
    uint16_t r;
    uint64_t out;
    HWND top;

    if ((*(uint8_t *)((uint8_t *)obj + 0x3a) & 8) != 0 && msg == 0x202) {
        if ((*(uint8_t *)((uint8_t *)obj + 0x3a) & 0xe0) != 0)
            goto dispatch;
        PECMD_RestoreForegroundWindow();
    }
    if (msg == 0x30) {
        if (*(int64_t *)((uint8_t *)obj + 0x44) != 0) {
            SendMessageW(*(HWND *)(*(int64_t *)((uint8_t *)obj + 0x44) + OBJ_HWND), 0x30,
                         (WPARAM)wParam, (LPARAM)lParam);
            FUN_1400EF654((int64_t)obj);
        }
    }
    if (msg == 0x45a) {
        PECMD_SetControlText(*(HWND *)((uint8_t *)obj + 8), wParam, lParam, 1);
        return 1;
    }
    if (msg == 0x113 && (int)wParam == 0x2715) {
        uint32_t style = GetWindowLongW(*(HWND *)((uint8_t *)obj + 8), -0x10);
        if ((style >> 0x1c & 1) != 0) {
            LRESULT lr = SendMessageW(*(HWND *)((uint8_t *)obj + 8), 0x449, 0, 0);
            FUN_1400EF654((int64_t)obj);
            if ((int)lr < 0)
                goto dispatch;
        }
        KillTimer(*(HWND *)((uint8_t *)obj + 8), 0x2715);
    }
dispatch:
    {
        count = *(int *)(*(int64_t *)((uint8_t *)obj + 0x34) + 0x10);
        if (count < 1)
            top = 0;
        else
            top = FUN_1400E5788(*(HWND *)((uint8_t *)obj + 8));
        if (top != 0)
            SendMessageW(top, 0x450, 4, 0x5aa555aa);
        if (msg == 10 && *(int64_t *)((uint8_t *)obj + 0x44) != 0)
            FUN_1400EF654((int64_t)obj);
        if (0 < count) {
            out = 0;
            r = FUN_1400F172C(*(int64_t **)((uint8_t *)obj + 0x34), msg, (uint64_t)wParam,
                              (uint64_t *)lParam, *(int64_t *)((uint8_t *)obj + 8), 1, &out);
            if ((r & 4) != 0) {
                if (top == 0)
                    return out;
                PostMessageW(top, 0x451, 0x204, -0x55aaa55b);
                return out;
            }
        }
        uint64_t res = FUN_1400F0814(obj, msg, wParam, lParam, 0);
        if (0 < count)
            FUN_1400F172C(*(int64_t **)((uint8_t *)obj + 0x34), msg, (uint64_t)wParam,
                          (uint64_t *)lParam, *(int64_t *)((uint8_t *)obj + 8), 2, NULL);
        if (top != 0)
            PostMessageW(top, 0x451, 0x204, -0x55aaa55b);
        return res;
    }
}

/* ========== FUN_1400EF91C @0x1400ef91c ==========
 * 创建静态子控件对象 (明确 0xe8 对象 + 0x90 初始化数据), 应用
 * ES_* 样式与字体, 挂到父窗口。
 * TODO(verify): 样式位与字段含义。
 */
void FUN_1400EF91C(int64_t obj, uint32_t style, uint64_t flags)
{
    RECT rc;
    uint64_t *child;
    uint64_t local_res8 = 0;
    LPCWSTR str = WSTR(" ");
    LRESULT font;
    uint32_t uVar5;

    if (*(int64_t *)((uint8_t *)obj + 0x110) != 0)
        return;

    rc.left = rc.top = rc.right = rc.bottom = 0;
    GetClientRect(*(HWND *)((uint8_t *)obj + OBJ_HWND), &rc);
    if ((style >> 0xb & 1) == 0) {
        rc.top += 4;
        rc.bottom -= 4;
        rc.left += 4;
        rc.right -= 4;
    }
    child = FUN_1400EFEC8((uint64_t *)operator_new(0xe8));
    if (child == NULL)
        return;
    *(int64_t **)((uint8_t *)obj + 0x110) = (int64_t *)child;
    InvalidateRect(*(HWND *)((uint8_t *)obj + OBJ_HWND), NULL, 1);

    uVar5 = style | 0x200;
    uint32_t w = GetWindowLongW(*(HWND *)((uint8_t *)obj + OBJ_HWND), -0x10);
    if ((w & 0x300) == 0x300) {
        uVar5 = style | 0x201;
        *(uint8_t *)(child + 0xc) = 0x11;
    }
    else if ((w >> 9 & 1) == 0) {
        *(uint8_t *)(child + 0xc) = 0x10;
    }
    else {
        uVar5 = style | 0x202;
        *(uint8_t *)(child + 0xc) = 0x12;
    }

    FUN_140063620(&local_res8);
    if ((*(uint8_t *)((uint8_t *)obj + OBJ_FLAGS) & 8) != 0)
        str = (LPCWSTR)g_szEmpty;
    FUN_1400E5730(*(HWND *)((uint8_t *)obj + OBJ_HWND), (int64_t *)&local_res8);
    if ((flags & 1) == 0)
        SetWindowTextW(*(HWND *)((uint8_t *)obj + OBJ_HWND), str);

    int rcPack[4] = { rc.left, rc.top, rc.right, rc.bottom };
    FUN_1400EFFF8((int64_t *)child, (LPCWSTR)local_res8, uVar5 | 0x40000020,
                               rcPack, *(HWND *)((uint8_t *)obj + OBJ_HWND), 0x7d2);
    if ((flags & 2) != 0)
        FUN_14001d78c((uint8_t *)(child + 0x12), (uint8_t *)((uint8_t *)obj + 0x90), 0x10);
    SetWindowPos((HWND)child[4], 0, 0, 0, 0, 0, 3);
    font = FUN_1400E5890((int64_t)obj);
    ((void (*)(uint64_t *, LRESULT, int))((void **)*child)[0x21])(child, font, 1);
    InvalidateRect((HWND)child[4], NULL, 1);
    FUN_14005B104((WCHAR **)&local_res8);
}

/* ========== FUN_1400EFB08 @0x1400efb08 ==========
 * 创建 BUTTON 子控件 (含椭圆区域与按下状态), 非普通样式时
 * 生成静态子控件并设置定时器。
 */
bool FUN_1400EFB08(int64_t *obj, LPCWSTR text, uint64_t style, int *rc,
                                HWND parent, uint32_t id, DWORD exStyle)
{
    int x1, y1, x2, y2;
    bool multiline, special;
    LONG lparent;
    HRGN rgn;
    HWND h;
    uint32_t uVar13;

    multiline = false;
    special = false;
    if ((style & 0x40) != 0 || (*(uint8_t *)((uint8_t *)obj + OBJ_FLAGS) & 8) != 0)
        special = true;
    x2 = rc[2];
    y2 = rc[3];
    x1 = rc[0];
    y1 = rc[1];
    lparent = GetWindowLongW(parent, -6);
    if ((int)obj[0x12] >= 0 || (int)obj[0x13] >= 0)
        multiline = true;
    bool anyline = false;
    uVar13 = (uint32_t)(-(uint32_t)(((uint8_t)(style & 0xfffffbbf) & 0xb) != 0xb) & 2);
    if ((!special) && ((uVar13 != 0 && (anyline = multiline, multiline)) ||
                       StrStrW(text, WSTR("\r\n")) != 0)) {
        special = true;
    }
    if ((*(int *)((uint8_t *)obj + 0x94) >= 0 || *(int *)((uint8_t *)obj + 0x9c) >= 0) &&
        StrChrW(text, L'&') != NULL) {
        uint64_t u = uVar13 | 1;
        (void)u;
    }
    if (exStyle == 0)
        exStyle = 0;

    h = CreateWindowExW(exStyle, WSTR("BUTTON"), text, (DWORD)(style & 0xfffffbbf),
                        rc[0], rc[1], x2 - x1, y2 - y1, parent,
                        (HMENU)(uintptr_t)id, (HINSTANCE)(intptr_t)lparent, NULL);
    obj[4] = (int64_t)(uintptr_t)h;
    PECMD_ContainerAppend((int64_t *)obj);
    if ((*(uint8_t *)((uint8_t *)obj + OBJ_FLAGS) & 1) != 0) {
        rgn = CreateEllipticRgn(0, 0, x2 - x1, y2 - y1);
        SetWindowRgn((HWND)obj[4], rgn, 1);
        DeleteObject(rgn);
    }
    if (special) {
        *(uint8_t *)((uint8_t *)obj + OBJ_FLAGS) &= 0xef;
        *(uint8_t *)((uint8_t *)obj + OBJ_FLAGS) |=
            ((uint8_t)(style >> 10) & 1) << 4;
        FUN_1400EF91C((int64_t)obj, (style & 0x400) * 2, 0);
        if (anyline && obj[0x22] != 0)
            *(int64_t **)(obj[0x22] + 0xe0) = (int64_t *)obj;
        if ((*(uint8_t *)((uint8_t *)obj + OBJ_FLAGS) & 8) != 0 &&
            (style >> 0x1c & 1) != 0) {
            SetTimer((HWND)obj[4], 0x2715, 10, NULL);
        }
    }
    return obj[4] != 0;
}

/* ========== FUN_1400F0814 @0x1400f0814 ==========
 * 通用 GDI/控件消息: 0x45a 专用, 0x2b 走虚表, 0x465 查询切换,
 * 其余 0x233 查映射 + FUN_1400E5B0C 兜底。
 */
uint64_t FUN_1400F0814(int64_t obj, uint32_t msg, int64_t wParam,
                                     LPCWSTR lParam, int flag)
{
    int count;
    uint16_t r;
    uint64_t out;
    HWND top;
    bool generic = false;
    uint64_t res = 0;

    if (msg == 0x45a) {
        PECMD_SetControlText(*(HWND *)((uint8_t *)obj + 8), wParam, lParam, 0);
        return 1;
    }
    if (msg == 0x2b) {
        res = ((uint64_t (*)(int64_t))((void **)*(int64_t *)obj)[1])(*(int64_t *)((uint8_t *)obj + 8));
    }
    else if (msg == 0x465) {
        LRESULT lr = SendMessageW(*(HWND *)((uint8_t *)obj + 8), 0xf0, 0, 0);
        res = (uint64_t)((int)lr == 0);
        SendMessageW(*(HWND *)((uint8_t *)obj + 8), 0xf1, (uint64_t)(res != 0), 0);
    }
    else {
        generic = true;
        res = 0;
        if (msg == 0x201)
            *(uint8_t *)((uint8_t *)obj + 0x2c) = 1;
        else if (msg == 0x202)
            *(uint8_t *)((uint8_t *)obj + 0x2c) = 0;
    }

    count = *(int *)(*(int64_t *)((uint8_t *)obj + 0x34) + 0x10);
    if (count < 1)
        top = 0;
    else
        top = FUN_1400E5788(*(HWND *)((uint8_t *)obj + 8));
    if (top != 0)
        SendMessageW(top, 0x450, 4, 0x5aa555aa);

    if (0 < count && flag != 0) {
        out = 0;
        r = FUN_1400F172C(*(int64_t **)((uint8_t *)obj + 0x34), msg, (uint64_t)wParam,
                          (uint64_t *)lParam, *(int64_t *)((uint8_t *)obj + 8), 1, &out);
        if ((r & 4) != 0) {
            if (top == 0)
                return out;
            PostMessageW(top, 0x451, 0x204, -0x55aaa55b);
            return out;
        }
    }
    if (generic)
        res = (uint64_t)FUN_1400E5B0C(obj, (uint64_t)msg, wParam, (int64_t *)lParam);
    if (0 < count && flag != 0)
        FUN_1400F172C(*(int64_t **)((uint8_t *)obj + 0x34), msg, (uint64_t)wParam,
                      (uint64_t *)lParam, *(int64_t *)((uint8_t *)obj + 8), 2, NULL);
    if (top != 0)
        PostMessageW(top, 0x451, 0x204, -0x55aaa55b);
    return res;
}

/* ========== FUN_1400F0ABC @0x1400f0abc ==========
 * 用 3 个兼容位图做多步 BitBlt ROP 合成 (近似半透明/柔光) 后写出。
 */
void FUN_1400F0ABC(HDC hdcDst, int x, int y, int w, int h, HDC hdcSrc,
                              int sx, int sy, uint64_t p9, uint64_t p10, COLORREF color)
{
    HDC hdcA, hdcB;
    HBITMAP bmp;
    uint64_t selA[3], selB[3];

    (void)p9;
    (void)p10;
    hdcA = CreateCompatibleDC(hdcDst);
    hdcB = CreateCompatibleDC(hdcDst);
    bmp = CreateCompatibleBitmap(hdcDst, w, h);
    PECMD_SelectObjectSlot_b028(selA, hdcA, bmp);
    bmp = CreateBitmap(w, h, 1, 1, NULL);
    PECMD_SelectObjectSlot_b028(selB, hdcB, bmp);
    SetBkColor(hdcA, color);
    BitBlt(hdcA, 0, 0, w, h, hdcSrc, sx, sy, 0xcc0020);
    BitBlt(hdcB, 0, 0, w, h, hdcA, 0, 0, 0xcc0020);
    BitBlt(hdcA, 0, 0, w, h, hdcB, 0, 0, 0x220000);
    BitBlt(hdcDst, x, y, w, h, hdcB, 0, 0, 0x8800c6);
    BitBlt(hdcDst, x, y, w, h, hdcA, 0, 0, 0xee0086);
    PECMD_RestoreAndDeleteObject(selB);
    PECMD_RestoreAndDeleteObject(selA);
    if (hdcB != 0)
        DeleteDC(hdcB);
    if (hdcA != 0)
        DeleteDC(hdcA);
}

/* ========== FUN_1400F0FA8 @0x1400f0fa8 ==========
 * 绘制控件: alpha 阈值内 BeginPaint + DrawText (或百分比条), 否则
 * 走虚表消息 0x0f 后用 FUN_1400F0CA0 填背景。
 */
void FUN_1400F0FA8(int64_t *obj, uint64_t p2, uint64_t p3)
{
    PAINTSTRUCT ps;
    RECT rc, rc2;
    HDC hdc;
    HGDIOBJ h = 0, old = 0;
    LPCWSTR txt = NULL;
    uint32_t dtFlags;
    COLORREF c;
    uint8_t b;

    if (g_alphaThreshold <= *(float *)((uint8_t *)obj + 0x50)) {
        hdc = BeginPaint((HWND)obj[4], &ps);
        old = 0;
        dtFlags = 0;
        rc.left = rc.top = rc.right = rc.bottom = 0;
        GetClientRect((HWND)obj[4], &rc);
        b = *(uint8_t *)((uint8_t *)obj + 0x60);
        if (*(float *)((uint8_t *)obj + 0x50) <= g_paintScale2) {
            FUN_1400F0DF4((int64_t)obj, hdc, &rc, 0x80000000,
                                 (int)(int8_t)*(uint8_t *)((uint8_t *)obj + 0x60) & 8);
        }
        h = (HGDIOBJ)obj[0xe];
        if (h == 0 && (h = (HGDIOBJ)obj[0xd], h == 0))
            h = (HGDIOBJ)FUN_1400E5890((int64_t)obj);
        if (h != 0)
            old = SelectObject(hdc, h);
        FUN_140063620((uint64_t *)&txt);
        FUN_1400E5730((HWND)obj[4], (int64_t *)&txt);
        SetBkMode(hdc, 1);
        if (*(int32_t *)((uint8_t *)obj + 0x5c) >= 0)
            SetTextColor(hdc, *(COLORREF *)((uint8_t *)obj + 0x5c));
        if ((*(uint8_t *)((uint8_t *)obj + 0x60) & 0x10) != 0)
            dtFlags = (uint32_t)((int8_t)*(uint8_t *)((uint8_t *)obj + 0x60) & 3);
        if ((b & 0x20) == 0 && StrChrW(txt, L'\n') == NULL) {
            dtFlags |= 0x24;
        }
        else {
            rc2 = rc;
            DrawTextW(hdc, txt, -1, &rc2, dtFlags | 0x400);
            OffsetRect(&rc, 0, ((rc.bottom - rc2.bottom - rc.top) + rc2.top) / 2);
        }
        DrawTextW(hdc, txt, -1, &rc, dtFlags | 0x8040);
        if (old != 0)
            SelectObject(hdc, old);
        EndPaint((HWND)obj[4], &ps);
        FUN_14005B104((WCHAR **)&txt);
        return;
    }

    if (*(int16_t *)((uint8_t *)obj + 0xa2) >= 0 &&
        *(int8_t *)((uint8_t *)obj + 0xac) == -0x10) {
        c = *(COLORREF *)((uint8_t *)obj + 0x94 +
                          (int64_t)*(int16_t *)((uint8_t *)obj + 0xa2) * 8);
    }
    else {
        c = *(COLORREF *)((uint8_t *)obj + 8);
    }
    ((uint64_t (*)(int64_t, int, uint64_t, uint64_t))((void **)*obj)[1])(
        obj[4], 0xf, p2, p3);
    if ((int)c >= 0)
        FUN_1400F0CA0((HWND)obj[4], c);
}