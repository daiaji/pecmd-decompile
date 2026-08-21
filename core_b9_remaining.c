/* Auto-generated simplified stubs for remaining B9 business functions. */
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include "pecmd_defs.h"

/* CRT/内部数据符号（link_stubs.c 提供桩定义） */
extern uint64_t g_u64E598;
extern uint64_t g_u64E600;
extern uint64_t g_u64EEE8;
extern uint64_t g_u64EEF0;
extern uint64_t g_u64B960;
extern uint64_t PTR_s_No_error_14013b800;
extern uint64_t PTR_DAT_14013a8f0;
extern uint64_t g_u64BF50;
extern uint64_t g_u64BF54;
extern uint64_t g_u64BF58;
extern uint64_t PTR_DAT_14013bfe0;
extern uint64_t g_u64EC88;
extern uint64_t g_u64F520;

/* ---- 业务辅助函数声明 (已实现于 core_b8* / core_* ) ---- */
extern WCHAR *PECMD_AllocString(WCHAR **ps, int64_t count);            /* @0x140063720 */
extern int64_t *PECMD_ReplaceStringSlot(int64_t *dst, uint64_t *src);        /* @0x140070398 */
extern void FUN_14005B104(WCHAR **ps);                             /* @0x14005b104 */
extern void FUN_14005b104(WCHAR **ps);                             /* @0x14005b104 (本批构造函数调用用小写) */
extern uint64_t *PECMD_InitScrollObj(uint64_t *param_1, uint64_t param_2); /* @0x1400fcf44 */
extern void PECMD_AllocStrSlot(WCHAR **ps);                             /* @0x140063620 初始化串容器 */
extern int64_t *FUN_1400702f0(int64_t *out, char *src, uint64_t len); /* @0x1400702f0 */
extern uint64_t PTR_FUN_140124e50;                                 /* 虚表指针槽 */
extern COLORREF FUN_1400E68E0(HDC hdc, RECT *rc, COLORREF color);  /* @0x1400e68e0 */
extern void FUN_1400F0FA8(int64_t *obj, uint64_t wParam, uint64_t lParam); /* @0x1400f0fa8 */
extern bool FUN_14006E588(int64_t ctx, LPCWSTR name, int width, int height,
                          uint32_t flags1, uint32_t flags2);       /* @0x14006e588 */
extern uint32_t FUN_1400EDE48(HDC hdc, HICON icon, uint64_t param3, int param4,
                              uint64_t srcW, uint64_t srcH, uint32_t param7,
                              uint32_t param8, uint64_t dstW, uint64_t dstH, COLORREF color); /* @0x1400ede48 */
extern int64_t PECMD_ContainerAppend(uint64_t *obj);                       /* @0x14006b8fc */
extern void PECMD_SetWindowTheme(uint64_t hwnd);                          /* @0x140066054 */
extern HWND FUN_1400E5788(HWND hwnd);                              /* @0x1400e5788 */
extern int64_t FUN_1400E5B0C(int64_t obj, uint64_t p2, int64_t p3, int64_t *p4); /* @0x1400e5b0c */
extern uint16_t FUN_1400F172C(int64_t *map, int msg, uint64_t wParam, uint64_t *lParam,
                              int64_t hwnd, uint8_t mode, uint64_t *out); /* @0x1400f172c */
extern void FUN_1400FD538(HWND hwnd, int mode);                    /* @0x1400fd538 */
extern void PECMD_HandleControlMessage(void *param_1, int param_2, long long param_3, uint64_t param_4); /* @0x1400fdad8 */

/* ---- GDI+ 延迟绑定函数指针槽 (core_b1_remaining.c 初始化) ---- */
extern int (*g_pGdipGetImageWidth)();   /* GdipGetImageWidth  */
extern int (*g_pGdipGetImageHeight)();   /* GdipGetImageHeight */
extern int (*g_pGdipDrawImageRectI)(void *, void *, int, int, int64_t, int);   /* GdipDrawImageRectI */
extern int (*g_pGdipCreateFromHDC)();   /* GdipCreateFromHDC  */
extern void *g_pGdipSetSmoothingMode;   /* GdipSetSmoothingMode */
extern void *g_pGdipDrawImageRectRectI;   /* GdipDrawImageRectRectI */
extern void *g_pGdipDrawImageI;   /* GdipDrawImageI     */
extern int (*g_pGdipDeleteGraphics)();   /* GdipDeleteGraphics */

/* @0x140100ac4 0x472 消息中用于重建 printf 宽格式串的常量字节 */
extern uint64_t g_u64_12d058;
extern uint64_t g_u64_12d060;

/* ---- 本批还原所需: 额外 helper extern int (*g_pGdipDisposeImage)();                                    (GdipDisposeImage 函数指针) */
extern WCHAR *FUN_1400703E4(WCHAR **ps, LPCWSTR src);          /* @0x1400703e4 */
extern void PECMD_FindFirstFileW(HANDLE *ph, LPCWSTR path, WIN32_FIND_DATAW *fd); /* @0x140101db8 */
extern BOOL PathFileExistsW(LPCWSTR);                          /* Win32 API (link_stubs.c) */
extern HBITMAP PECMD_LoadImageBitmap(LPCWSTR param_1, int64_t *param_2, int64_t param_3,
                             uint64_t param_4, uint32_t *param_5, uint64_t *param_6,
                             uint64_t param_7);                 /* @0x1400b3d0c */

int64_t PECMD_AddImagesToImageList(int64_t param_1, LPCWSTR param_2)
{
    /* @0x1401000f0 size=205 */
    WCHAR WVar1;
    int iVar2;
    HBITMAP pHVar3;
    WCHAR *pWVar4;
    int64_t lVar5;
    uint32_t local_res10[2];
    int64_t local_res18[2];

    lVar5 = 0;
    WVar1 = *param_2;
    while ((pWVar4 = (WCHAR *)param_2), WVar1 != L'\0') {
        for (; (WVar1 = *pWVar4, WVar1 != L'\0' &&
                ((((uint16_t)WVar1 < 9 || (0xd < (uint16_t)WVar1)) && (WVar1 != L' '))));
             pWVar4 = pWVar4 + 1) {
        }
        if (*pWVar4 != L'\0') {
            *pWVar4 = L'\0';
            pWVar4 = pWVar4 + 1;
        }
        local_res10[0] = 1;
        local_res18[0] = 0;
        pHVar3 = PECMD_LoadImageBitmap(param_2, local_res18, 0, 0, local_res10, (uint64_t *)0, 0);
        if (local_res10[0] == 0) {
            iVar2 = ((int (*)(int64_t, HBITMAP))g_pImageListAdd)(*(int64_t *)(param_1 + 0xd8),
                                                                pHVar3);
        }
        else {
            iVar2 = ((int (*)(int64_t, int64_t))g_pImageListReplaceIcon)(*(int64_t *)(param_1 + 0xd8),
                                                                (int64_t)0xffffffffffffffff);
        }
        if (iVar2 != 0) {
            lVar5 = lVar5 + 1;
        }
        param_2 = pWVar4;
        WVar1 = *pWVar4;
    }
    return lVar5;
}

int64_t FUN_1401001c0(int64_t param_1, int64_t *param_2, uint64_t param_3, uint64_t param_4)
{
    /* UNIMPLEMENTED @0xFUN_1401001c0 — decompile-failed, body 未还原 */
/* @0x1401001c0 size=2306 TODO: 未还原 stub（FUN_140100ac4 按其真实签名调用） */
    (void)param_1; (void)param_2; (void)param_3; (void)param_4;
    return 0;
}

uint64_t FUN_140100ac4(void)
{
    /* UNIMPLEMENTED @0xFUN_140100ac4 — decompile-failed, body 未还原 */
/* @0x140100ac4 size=4631 */
    return 0;
}

uint64_t PECMD_IsRemovableDrive(uint16_t param_1)
{
    /* @0x140101cdc size=88 判断盘符是否为可移动(非固定)驱动器 */
    WCHAR root[3];
    UINT type;

    root[0] = param_1 & 0xffdf;   /* 转大写 */
    if (root[0] == L'A' || root[0] == L'B') {
        root[1] = L':';
        root[2] = L'\0';
        type = GetDriveTypeW(root);
        return (uint64_t)(type != 2);   /* DRIVE_FIXED=2 */
    }
    return 0xfffffffe;
}

uint64_t PECMD_IsDirectory(LPCWSTR path)
{
    /* @0x140101d34 size=59 判断路径是否为目录 */
    uint8_t info[64] = {0};
    BOOL ok;

    ok = GetFileAttributesExW(path, 0, info);
    if (ok != 0 && (*(uint32_t *)info & FILE_ATTRIBUTE_DIRECTORY) != 0) {
        return 1;
    }
    return 0;
}

uint64_t PECMD_CheckFileAttr(LPCWSTR path)
{
    /* @0x140101d74 size=62 判断路径是否存在(含属性标志) */
    BOOL ok;
    uint32_t info[16];
    uint64_t result;

    ok = GetFileAttributesExW(path, 0, info);
    result = ok ? (uint64_t)((uint64_t)info[0] | 0x100000000u) : 0;
    return result;
}

void PECMD_FindFirstFileW(HANDLE *ph, LPCWSTR path, WIN32_FIND_DATAW *fd)
{
    /* @0x140101db8 size=63 FindFirstFileW 业务包装:失败返回句柄0 */
    HANDLE h;

    h = FindFirstFileW(path, fd);
    if (h == (HANDLE)0xffffffffffffffff) {
        h = (HANDLE)0;
    }
    *ph = h;
}

int PECMD_CreateProcessW(LPCWSTR cmd, LPWSTR buf, LPSECURITY_ATTRIBUTES sa, LPSECURITY_ATTRIBUTES da,
                  BOOL inherit, DWORD flags, LPVOID env, LPCWSTR cwd,
                  LPSTARTUPINFOW si, LPPROCESS_INFORMATION pi)
{
    /* @0x140101e04 size=103 CreateProcessW 业务包装(返回成功标志) */
    return CreateProcessW(cmd, buf, sa, da, inherit, flags, env, cwd, si, pi) != 0;
}

uint32_t PECMD_FindFileOrDir(LPWSTR param_1, uint32_t param_2)
{
    /* @0x140101ed8 size=676 查找文件/目录并返回匹配标志 */
    int iVar1;
    BOOL BVar2;
    HANDLE hFindFile;
    int64_t lVar3;
    uint32_t uVar4;
    uint32_t uVar5;
    LPWSTR local_res8;
    uint32_t local_res10;
    HANDLE local_res18[2];
    WIN32_FIND_DATAW local_298;

    local_res10 = 0;
    uVar4 = param_2 & 1;
    if (*param_1 == L'\0') {
        return local_res10;
    }
    iVar1 = lstrlenW(param_1);
    lVar3 = (int64_t)iVar1;
    local_res18[0] = (HANDLE)0x0;
    local_298.dwFileAttributes = 0;
    memset(&local_298.ftCreationTime, 0, 0x24c);
    local_res8 = (LPWSTR)0;
    PECMD_FindFirstFileW(local_res18, param_1, &local_298);
    hFindFile = local_res18[0];
    uVar5 = uVar4;
    if (local_res18[0] == (HANDLE)0x0) {
        if ((param_2 & 0xf0) != 0) {
            param_1[lVar3] = L'\\';
            param_1[lVar3 + 1] = L'\0';
            local_res10 = PathFileExistsW(param_1);
            param_1[lVar3] = L'\0';
            if (local_res10 != 0) {
                FUN_14005B104((WCHAR **)&local_res8);
                return local_res10;
            }
        }
        if ((lVar3 < 2) ||
           ((param_1[lVar3 + -1] != L':' &&
            (((*param_1 != L'\\' || (param_1[1] != L'\\')) || (param_1[2] != L'?')))))) {
        LAB_1401020e8:
            BVar2 = GetFileAttributesExW(param_1, 1, &local_298);
            uVar5 = uVar4;
            if (BVar2 == 0) goto LAB_140102142;
        }
        else {
            FUN_1400703E4((WCHAR **)&local_res8, param_1);
            PECMD_AllocString(&local_res8, lVar3 + 8);
            param_1 = local_res8;
            local_res8[lVar3] = L'\\';
            local_res8[lVar3 + 1] = L'*';
            local_res8[lVar3 + 2] = L'\0';
            PECMD_FindFirstFileW(local_res18, local_res8, &local_298);
            hFindFile = local_res18[0];
            uVar5 = uVar4 | 0x10;
            if (local_res18[0] == (HANDLE)0x0) {
                local_res8[lVar3 + 1] = L'*';
                local_res8[lVar3 + 2] = L'.';
                local_res8[lVar3 + 1] = L'*';
                local_res8[lVar3 + 3] = L'\0';
                PECMD_FindFirstFileW(local_res18, param_1, &local_298);
                hFindFile = local_res18[0];
                if (local_res18[0] == (HANDLE)0x0) {
                    local_res8[lVar3] = L'\0';
                    goto LAB_1401020e8;
                }
            }
        }
    }
    if (uVar5 == 1) {
        do {
            local_res10 = local_298.dwFileAttributes >> 4 & 1;
            if (local_res10 != 0) break;
            BVar2 = FindNextFileW(hFindFile, &local_298);
        } while (BVar2 != 0);
    }
    else {
        local_res10 = 1;
    }
LAB_140102142:
    FUN_14005B104((WCHAR **)&local_res8);
    if ((hFindFile != (HANDLE)0x0) && (hFindFile != (HANDLE)0xffffffffffffffff)) {
        FindClose(hFindFile);
    }
    /* TODO(verify): 原反编译返回未定寄存器 unaff_EDI, 按语义还原为匹配标志 local_res10 */
    return local_res10;
}

uint64_t *PECMD_ConstructObject(uint64_t *param_1, uint64_t param_2)
{
    /* @0x140102184 size=190 对象构造函数 */
    int64_t local_res8;

    PECMD_InitScrollObj(param_1, param_2);
    *param_1 = (uint64_t)(uintptr_t)&PTR_FUN_140124e50;
    PECMD_AllocStrSlot((WCHAR **)(param_1 + 0x1e));
    param_1[0x20] = 0;
    *((uint8_t *)param_1 + 0xf8) = 0;      /* *(undefined1 *)(param_1 + 0x1f) */
    *((uint8_t *)param_1 + 0xf9) = 0;
    *((uint32_t *)((uint8_t *)param_1 + 0x10c)) = 0x80000000;
    *((uint32_t *)((uint8_t *)param_1 + 0x108)) = 0x80000000;   /* *(undefined4 *)(param_1 + 0x21) */
    *((uint8_t *)param_1 + 0xfa) = 0xff;
    *((uint8_t *)param_1 + 0xfb) = 0;
    *((uint8_t *)param_1 + 0xfc) = 0;
    FUN_1400702f0((int64_t *)&local_res8, (char *)&g_DefEnv, 0xffffffffffffffffULL);
    PECMD_ReplaceStringSlot((int64_t *)(param_1 + 0x1e), (uint64_t *)&local_res8);
    FUN_14005b104((WCHAR **)&local_res8);
    *((uint32_t *)((uint8_t *)param_1 + 0x11c)) = 0;
    *((uint32_t *)((uint8_t *)param_1 + 0x118)) = 0;            /* *(undefined4 *)(param_1 + 0x23) */
    *((uint32_t *)((uint8_t *)param_1 + 0x114)) = 0;
    *((uint32_t *)((uint8_t *)param_1 + 0x110)) = 0;            /* *(undefined4 *)(param_1 + 0x22) */
    return param_1;
}

void PECMD_ReleaseGdiImage(int64_t param_1)
{
    /* @0x140102244 size=41 释放 GDI 图像对象并清空槽 */
    if (*(int64_t *)(param_1 + 0x100) != 0) {
        ((void (*)(void))g_pGdipDisposeImage)();
        *(int64_t *)(param_1 + 0x100) = 0;
    }
    return;
}

void PECMD_PaintControl(int64_t *param_1, uint64_t param_2, uint64_t param_3)
{
    /* @0x140102270 size=1288 控件绘制(WndProc WM_PAINT 路径) */
    int iVar3;
    HDC hDC;
    HGDIOBJ h_00;
    HGDIOBJ pvVar4;
    HPEN h;
    COLORREF color;
    bool bVar2;
    uint32_t uVar5;
    uint32_t uVar8;
    uint32_t uVar7;
    HWND pHVar6;
    uint32_t local_res8[2];
    uint32_t local_res20[2];
    int64_t local_a8;
    RECT local_a0;
    PAINTSTRUCT local_88;

    (void)param_2; (void)param_3;
    hDC = (HDC)0;
    color = 0xffffffff;
    bVar2 = false;
    if (*((char *)param_1 + 0xf9) > 0 &&
        (iVar3 = *((char *)param_1 + 0xfa),
         (iVar3 < 0 ||
          (color = *(COLORREF *)((char *)param_1 + iVar3 * 4 + 0x108), (int)color < 0)) &&
         iVar3 > 0)) {
        color = 0xc00000;
    }
    local_a0.left = 0;
    local_a0.top = 0;
    local_a0.bottom = 0;
    local_a0.right = 0;
    if (((param_1[0x20] != 0) || (*((short *)param_1[0x1e]) != 0 &&
        (*((char *)param_1 + 0x1f7) == 0))) ||
        (*((int *)&param_1[8]) >= 0 || (param_1[7] != 0))) {
        bVar2 = true;
        hDC = BeginPaint((HWND)param_1[4], &local_88);
        GetClientRect((HWND)param_1[4], &local_a0);
        if ((HBRUSH)param_1[7] == (HBRUSH)0) {
            if (*((int *)&param_1[8]) >= 0) {
                FUN_1400E68E0(hDC, &local_a0, *(COLORREF *)&param_1[8]);
            }
        }
        else {
            FillRect(hDC, &local_a0, (HBRUSH)param_1[7]);
        }
        if ((int)color >= 0) {
            h = CreatePen(0, *((char *)param_1 + 0xf9), color);
            h_00 = SelectObject(hDC, h);
            pvVar4 = SelectObject(hDC, GetStockObject(5));
            Rectangle(hDC, local_a0.left, local_a0.top, local_a0.right, local_a0.bottom);
            SelectObject(hDC, pvVar4);
            SelectObject(hDC, h_00);
            DeleteObject(h);
        }
    }
    if (param_1[0x20] == 0) {
        if ((*((short *)param_1[0x1e]) == 0) || (*((char *)param_1 + 0x1f7) != 0)) {
            if (!bVar2) {
                FUN_1400F0FA8(param_1, param_2, param_3);
                return;
            }
        }
        else {
            GetWindowRect((HWND)param_1[4], &local_a0);
            iVar3 = *((char *)param_1 + 0xf9);
            local_a0.left += iVar3;
            local_a0.top += iVar3;
            local_a0.right -= iVar3;
            local_a0.bottom -= iVar3;
            if (hDC == (HDC)0) {
                hDC = BeginPaint((HWND)param_1[4], &local_88);
            }
            bVar2 = FUN_14006E588((int64_t)hDC, (LPCWSTR)param_1[0x1e],
                                  local_a0.right - local_a0.left,
                                  local_a0.bottom - local_a0.top, 0, 0);
            if (!bVar2) {
                *((char *)param_1 + 0x1f7) = 1;
            }
        }
    }
    else {
        iVar3 = *((char *)param_1 + 0xf9);
        local_a0.top += iVar3;
        local_a0.bottom -= iVar3;
        local_a0.left += iVar3;
        local_a0.right -= iVar3;
        local_res20[0] = *(uint32_t *)((char *)param_1 + 0x11c);
        uVar8 = (uint32_t)(local_a0.bottom - local_a0.top);
        local_res8[0] = *(uint32_t *)&param_1[0x23];
        uVar7 = (uint32_t)(local_a0.right - local_a0.left);
        local_a8 = 0;
        if (local_res8[0] == 0) {
            ((void (*)(int64_t, uint32_t *))g_pGdipGetImageWidth)(param_1[0x20], local_res8);
        }
        uVar5 = local_res20[0];
        if ((param_1[0x20] != 0) && (local_res20[0] == 0)) {
            ((void (*)(int64_t, uint32_t *))g_pGdipGetImageHeight)(param_1[0x20], local_res20);
            uVar5 = local_res20[0];
        }
        if (((*((char *)param_1 + 0xfc) == 0) || (local_res8[0] == 0)) ||
            ((uVar5 == 0) || ((local_res8[0] <= uVar7 && (uVar5 <= uVar8)))) ||
            (*((char *)param_1 + 0xfb) != 0)) {
            iVar3 = ((int (*)(HDC, int64_t *))g_pGdipCreateFromHDC)(hDC, &local_a8);
            if (iVar3 == 0) {
                if (*((char *)param_1 + 0xfb) == 0) {
                    if ((local_res8[0] != uVar7) || (local_res20[0] != uVar8)) {
                        ((void (*)(int64_t, int))g_pGdipSetSmoothingMode)(local_a8, 7);
                    }
                    if (*((char *)param_1 + 0xfb) == 0) {
                        if (*((int *)&param_1[0x23]) < 1) {
                            ((void (*)(int64_t, int64_t, LONG, int, int, int))g_pGdipDrawImageRectI)(
                                local_a8, param_1[0x20], local_a0.left, local_a0.top,
                                (int)uVar7, (int)uVar8);
                        }
                        else {
                            ((void (*)(int64_t))g_pGdipDrawImageRectRectI)(local_a8);
                        }
                    }
                }
                else {
                    ((void (*)(int64_t, int64_t, LONG, int))g_pGdipDrawImageI)(
                        local_a8, param_1[0x20], local_a0.left, local_a0.top);
                }
            }
        }
        else {
            FUN_1400EDE48(hDC, (HICON)param_1[0x20], (uint64_t)local_a0.left, local_a0.top,
                          (uint64_t)uVar7, (uint64_t)uVar8, (uint32_t)param_1[0x22],
                          *(uint32_t *)((char *)param_1 + 0x114), local_res8[0], uVar5,
                          0x80000000);
        }
        if (local_a8 != 0) {
            ((void (*)(void))g_pGdipDeleteGraphics)();
        }
        pHVar6 = GetFocus();
        if ((HWND)param_1[4] == pHVar6) {
            DrawFocusRect(hDC, &local_a0);
        }
    }
    EndPaint((HWND)param_1[4], &local_88);
}

bool PECMD_CreateStaticControl(int64_t *param_1, LPCWSTR param_2, DWORD param_3, int *param_4,
                   HWND param_5, unsigned int param_6)
{
    /* @0x140102778 size=207 创建 STATIC 静态文本子控件并挂钩容器 */
    int iVar1;
    int iVar2;
    int iVar3;
    int iVar4;
    LONG LVar5;
    HWND pHVar6;

    iVar1 = param_4[3];
    iVar2 = param_4[2];
    iVar3 = param_4[1];
    iVar4 = *param_4;
    LVar5 = GetWindowLongW(param_5, -6);
    pHVar6 = CreateWindowExW(0, WSTR("STATIC"), param_2, param_3, *param_4, param_4[1],
                             iVar2 - iVar4, iVar1 - iVar3, param_5,
                             (HMENU)(uintptr_t)param_6, (HINSTANCE)(intptr_t)LVar5, (void *)0);
    param_1[4] = (int64_t)pHVar6;
    PECMD_ContainerAppend((uint64_t *)param_1);
    return param_1[4] != 0;
}

bool PECMD_CreateButtonCtl(int64_t *param_1, LPCWSTR param_2, unsigned int param_3, int *param_4,
                   HWND param_5, unsigned int param_6)
{
    /* @0x140102848 size=221 创建 BUTTON 命令按钮子控件并挂钩容器 */
    int iVar1;
    int iVar2;
    int iVar3;
    int iVar4;
    LONG LVar5;
    HWND pHVar6;

    iVar1 = param_4[3];
    iVar2 = param_4[2];
    iVar3 = param_4[1];
    iVar4 = *param_4;
    LVar5 = GetWindowLongW(param_5, -6);
    pHVar6 = CreateWindowExW(0, WSTR("BUTTON"), param_2, param_3 | 0x10000, *param_4,
                             param_4[1], iVar2 - iVar4, iVar1 - iVar3, param_5,
                             (HMENU)(uintptr_t)param_6, (HINSTANCE)(intptr_t)LVar5, (void *)0);
    param_1[4] = (int64_t)pHVar6;
    PECMD_ContainerAppend((uint64_t *)param_1);
    PECMD_SetWindowTheme((uint64_t)param_1[4]);
    return param_1[4] != 0;
}

void PECMD_ControlHoverWndProc(int64_t *param_1, int param_2, long long param_3, uint64_t param_4)
{
    /* @0x140102928 size=309 控件 WM_PAINT/WM_TIMER/WM_MOUSEMOVE 处理 */
    POINT pt;
    BOOL BVar1;
    POINT local_28;
    RECT local_20;
    void (*fn)(int64_t *, long long, uint64_t);

    if (param_2 == 0xf) {
        fn = *(void (**)(int64_t *, long long, uint64_t))(*param_1 + 0x28);
        fn(param_1, param_3, param_4);
    }
    else if (param_2 == 0x113) {
        if ((*((char *)param_1 + 0xf9) > 0 && *((char *)param_1 + 0xfa) > 0) &&
            (param_3 == 0x2714)) {
            local_28.x = 0;
            local_28.y = 0;
            GetCursorPos(&local_28);
            local_20.left = 0;
            local_20.top = 0;
            local_20.right = 0;
            local_20.bottom = 0;
            GetWindowRect((HWND)param_1[4], &local_20);
            pt.y = local_28.y;
            pt.x = local_28.x;
            BVar1 = PtInRect(&local_20, pt);
            if (BVar1 == 0) {
                *((char *)param_1 + 0xfa) = 0;
                FUN_1400FD538((HWND)param_1[4], 1);
            }
        }
    }
    else if ((param_2 == 0x200) && (*((char *)param_1 + 0xf9) > 0) &&
             (*((char *)param_1 + 0xfa) == 0)) {
        *((char *)param_1 + 0xfa) = 1;
        InvalidateRect((HWND)param_1[4], (RECT *)0, 1);
        SetTimer((HWND)param_1[4], 0x2714, 0x32, (void *)0);
    }
    PECMD_HandleControlMessage(param_1, param_2, param_3, param_4);
}

void *FUN_140102d08(void *param_1, uint32_t param_2)
{
    /* UNIMPLEMENTED @0xFUN_140102d08 — decompile-failed, body 未还原 */
/* @0x140102d08 size=57 */
    return 0;
}

void *FUN_140102d44(void *param_1, void *param_2)
{
    /* UNIMPLEMENTED @0xFUN_140102d44 — decompile-failed, body 未还原 */
/* @0x140102d44 size=33 */
    return 0;
}

uint64_t __onexitinit(void)
{
    /* @0x140102e04 size=66 */
    return 0;
}

uint64_t _onexit(void)
{
    /* @0x140102e48 size=261 */
    return 0;
}

uint64_t _vswprintf_l(void)
{
    /* @0x140102f68 size=170 */
    return 0;
}

/* @0x140103014 size=11 SKIP(CRT): 反编译体仅调用 _vswprintf_l(param_1,param_2,NULL,param_3,param_5)。
   即 MSVC vswprintf_l 包装，属 CRT，不还原。 */
void FUN_140103014(wchar_t *param_1, size_t param_2, void *param_3, uint64_t param_4,
                   void *param_5)
{
    (void)param_1;
    (void)param_2;
    (void)param_3;
    (void)param_4;
    (void)param_5;
}

int64_t PECMD_WideStrLen(const uint16_t *s)
{
    /* @0x140103020 宽字符串长度 */
    const uint16_t *p = s;
    while (*p != 0) {
        p++;
    }
    return (int64_t)(p - s) - 1;
}

WCHAR *FUN_140103068(WCHAR *param_1, const WCHAR *param_2)
{
    /* UNIMPLEMENTED @0xFUN_140103068 — decompile-failed, body 未还原 */
/* @0x140103068 size=27 宽串复制 (wcscpy 等价) */
    WCHAR sVar1;
    WCHAR *psVar2;

    psVar2 = param_1;
    do {
        sVar1 = *param_2;
        param_2 = param_2 + 1;
        *psVar2 = sVar1;
        psVar2 = psVar2 + 1;
    } while (sVar1 != 0);
    return param_1;
}

uint64_t _LocaleUpdate(void)
{
    /* @0x140103084 size=169 */
    return 0;
}

uint64_t _wcsicmp(void)
{
    /* @0x140103244 size=148 */
    return 0;
}

uint64_t _wcsupr_s_l_stat(void)
{
    /* @0x1401032d8 size=534 */
    return 0;
}

uint64_t _wcsupr(void)
{
    /* @0x14010353c size=134 */
    return 0;
}

void **FUN_140103a20(void)
{
    /* UNIMPLEMENTED @0xFUN_140103a20 — decompile-failed, body 未还原 */
/* @0x140103a20 size=8 返回 stdio 表指针 */
    return (void **)(uintptr_t)&PTR_DAT_14013a8f0;
}

uint64_t __initstdio(void)
{
    /* @0x140103a28 size=226 */
    return 0;
}

uint64_t __endstdio(void)
{
    /* @0x140103b0c size=39 */
    return 0;
}

uint64_t _lock_file(void)
{
    /* @0x140103b34 size=95 */
    return 0;
}

uint64_t _lock_file2(void)
{
    /* @0x140103b94 size=45 */
    return 0;
}

uint64_t _unlock_file(void)
{
    /* @0x140103bc4 size=86 */
    return 0;
}

uint64_t _unlock_file2(void)
{
    /* @0x140103c1c size=39 */
    return 0;
}

uint64_t perror(void)
{
    /* @0x140103c78 size=173 */
    return 0;
}

uint64_t FUN_140103e10(void)
{
    /* UNIMPLEMENTED @0xFUN_140103e10 — decompile-failed, body 未还原 */
/* @0x140103e10 size=134 */
    return 0;
}

uint64_t wcstol(void)
{
    /* @0x140104348 size=48 */
    return 0;
}

uint64_t wcstoul(void)
{
    /* @0x140104378 size=51 */
    return 0;
}

int FUN_14010443c(uint32_t param_1)
{
    /* UNIMPLEMENTED @0xFUN_14010443c — decompile-failed, body 未还原 */
/* @0x14010443c size=10 SKIP(CRT): 反编译体仅调用 iswctype(c, 4)(MSVC _ALPHA)。
       业务代码(core_b2f)依赖其返回值判断首字符是否为字母。用同义 ASCII 判定实现。 */
    uint32_t c = param_1 & 0xffff;
    return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z');
}

uint64_t _wcslwr_s_l_stat(void)
{
    /* @0x140104544 size=534 */
    return 0;
}

void FUN_1401047a8(wchar_t *param_1, size_t param_2)
{
    /* UNIMPLEMENTED @0xFUN_1401047a8 — decompile-failed, body 未还原 */
/* @0x1401047a8 size=8 */
    /* SKIP(CRT): 内部调用 _wcslwr_s_l，MSVC CRT 小写转换 */
    (void)param_1;
    (void)param_2;
}

uint64_t _wcsnicmp(void)
{
    /* @0x14010490c size=165 */
    return 0;
}

double FUN_140104b00(double param_1)
{
    /* UNIMPLEMENTED @0xFUN_140104b00 — decompile-failed, body 未还原 */
/* @0x140104b00 size=773 */
    /* SKIP(CRT): MSVC CRT 三角函数 (asin/acos 多项式展开, 含 _controlfp_s/
       SQRT/系数表 _DAT_14012d2b0 与 FUN_14010da74 打印辅助). 使用 CRT 等价,
       不逐行还原内联展开. */
    (void)param_1;
    return 0.0;
}

double FUN_140104e08(double param_1)
{
    /* UNIMPLEMENTED @0xFUN_140104e08 — decompile-failed, body 未还原 */
/* @0x140104e08 size=787 */
    /* SKIP(CRT): MSVC CRT 三角函数 (asin/acos 多项式展开, 含 _controlfp_s/
       SQRT/系数表 _DAT_14012d2b0 与 FUN_14010da74 打印辅助). 使用 CRT 等价. */
    (void)param_1;
    return 0.0;
}

double FUN_14010511c(double param_1)
{
    /* UNIMPLEMENTED @0xFUN_14010511c — decompile-failed, body 未还原 */
/* @0x14010511c size=651 */
    /* SKIP(CRT): atan 数学函数，使用 CRT atan 句柄 */
    (void)param_1;
    return 0.0;
}

double FUN_1401053a8(double param_1)
{
    /* UNIMPLEMENTED @0xFUN_1401053a8 — decompile-failed, body 未还原 */
/* @0x1401053a8 size=276 */
    /* SKIP(CRT): MSVC CRT ceil 实现 (幂尾数掩码 + 有小数位时 +1.0, 含
       FUN_14010da74 打印辅助/非法参数路径). 使用 CRT ceil 等价, 不还原内联. */
    (void)param_1;
    return 0.0;
}

void FUN_1401054bc(double param_1, double param_2, double param_3, double param_4,
                   double param_5, int *param_6, double *param_7, double *param_8)
{
    /* @0x1401054bc size=302 */
    /* SKIP(CRT): MSVC CRT exp 内核 (range-reduction + 多项式), 不还原内联. */
    (void)param_1; (void)param_2; (void)param_3; (void)param_4; (void)param_5;
    (void)param_6; (void)param_7; (void)param_8;
}

double FUN_1401055ec(double param_1)
{
    /* UNIMPLEMENTED @0xFUN_1401055ec — decompile-failed, body 未还原 */
/* @0x1401055ec size=734 */
    /* SKIP(CRT): MSVC CRT exp 数学函数内联实现 */
    (void)param_1;
    return 0.0;
}

double FUN_1401058cc(double param_1)
{
    /* UNIMPLEMENTED @0xFUN_1401058cc — decompile-failed, body 未还原 */
/* @0x1401058cc size=756 */
    /* SKIP(CRT): MSVC CRT atan 数学函数内联实现 */
    (void)param_1;
    return 0.0;
}

double FUN_140105bc0(uint64_t param_1)
{
    /* UNIMPLEMENTED @0xFUN_140105bc0 — decompile-failed, body 未还原 */
/* @0x140105bc0 size=846 */
    /* SKIP(CRT): 反编译为 MSVC CRT log10 数学函数内联实现 */
    (void)param_1;
    return 0.0;
}

double FUN_140105f10(double param_1, double param_2)
{
    /* UNIMPLEMENTED @0xFUN_140105f10 — decompile-failed, body 未还原 */
/* @0x140105f10 size=1494 */
    /* SKIP(CRT): pow 数学函数，使用 log2/f2xm1/fscale 内联实现 */
    (void)param_1;
    (void)param_2;
    return 0.0;
}

double FUN_140106500(double param_1, double param_2)
{
    /* UNIMPLEMENTED @0xFUN_140106500 — decompile-failed, body 未还原 */
/* @0x140106500 size=162 */
    /* SKIP(CRT): sin 三角函数，使用 CRT 内联实现 */
    (void)param_1;
    (void)param_2;
    return 0.0;
}

double FUN_1401065a4(double param_1, double param_2)
{
    /* UNIMPLEMENTED @0xFUN_1401065a4 — decompile-failed, body 未还原 */
/* @0x1401065a4 size=175 */
    /* SKIP(CRT): cos 三角函数，使用 CRT 内联实现 */
    (void)param_1;
    (void)param_2;
    return 0.0;
}

double FUN_140106654(double param_1)
{
    /* UNIMPLEMENTED @0xFUN_140106654 — decompile-failed, body 未还原 */
/* @0x140106654 size=807 */
    /* SKIP(CRT): MSVC CRT sin 数学函数内联实现 */
    (void)param_1;
    return 0.0;
}

double FUN_14010697c(double param_1)
{
    /* UNIMPLEMENTED @0xFUN_14010697c — decompile-failed, body 未还原 */
/* @0x14010697c size=266 */
    /* SKIP(CRT): MSVC CRT sqrt 数学函数内联实现 */
    (void)param_1;
    return 0.0;
}

double FUN_140106a88(double param_1, double param_2, int param_3)
{
    /* UNIMPLEMENTED @0xFUN_140106a88 — decompile-failed, body 未还原 */
/* @0x140106a88 size=468 */
    /* SKIP(CRT): atan/asin 数学函数，使用 CRT 内联实现 */
    (void)param_1;
    (void)param_2;
    (void)param_3;
    return 0.0;
}

double FUN_140106c5c(double param_1)
{
    /* UNIMPLEMENTED @0xFUN_140106c5c — decompile-failed, body 未还原 */
/* @0x140106c5c size=791 */
    /* SKIP(CRT): MSVC CRT cos 数学函数内联实现 */
    (void)param_1;
    return 0.0;
}

uint64_t _purecall(void)
{
    /* @0x140107350 size=54 */
    return 0;
}

double FUN_140107468(double param_1, double param_2)
{
    /* UNIMPLEMENTED @0xFUN_140107468 — decompile-failed, body 未还原 */
/* @0x140107468 size=644 */
    /* SKIP(CRT): MSVC CRT _hypot 数学函数 */
    (void)param_1;
    (void)param_2;
    return 0.0;
}

int FUN_140107944(int param_1)
{
    /* UNIMPLEMENTED @0xFUN_140107944 — decompile-failed, body 未还原 */
/* @0x140107944 size=10 */
    int iVar1;

    iVar1 = -param_1;
    if (-1 < param_1) {
        iVar1 = param_1;
    }
    return iVar1;
}

uint64_t _GetEstablisherFrame(void)
{
    /* @0x140107950 size=235 */
    return 0;
}

uint64_t _GetImageBase(void)
{
    /* @0x140107a3c size=21 */
    return 0;
}

uint64_t _GetThrowImageBase(void)
{
    /* @0x140107a54 size=21 */
    return 0;
}

uint64_t _SetImageBase(void)
{
    /* @0x140107a6c size=27 */
    return 0;
}

void FUN_140107a88(uint64_t param_1)
{
    /* UNIMPLEMENTED @0xFUN_140107a88 — decompile-failed, body 未还原 */
/* @0x140107a88 size=27 */
    /* SKIP(CRT): _SetThrowImageBase, MSVC CRT SEH helper */
    (void)param_1;
}

uint64_t __FrameUnwindToEmptyState(void)
{
    /* @0x140107aa4 size=172 */
    return 0;
}

uint64_t _CallSETranslator(void)
{
    /* @0x140107c7c size=81 */
    return 0;
}

uint64_t _GetRangeOfTrysToCheck(void)
{
    /* @0x140107cd0 size=293 */
    return 0;
}

uint64_t _IsExceptionObjectToBeDestroyed(void)
{
    /* @0x140107e3c size=50 */
    return 0;
}

uint64_t _UnwindNestedFrames(void)
{
    /* @0x140107ed0 size=230 */
    return 0;
}

void *FUN_140107fb8(void *param_1, uint32_t param_2)
{
    /* UNIMPLEMENTED @0xFUN_140107fb8 — decompile-failed, body 未还原 */
/* @0x140107fb8 size=57 */
    return 0;
}

uint64_t __tmainCRTStartup(void)
{
    /* @0x140108014 size=441 */
    return 0;
}

uint64_t entry(void)
{
    /* @0x1401081ec size=18 */
    return 0;
}

uint64_t exception(void)
{
    /* @0x140108274 size=25 */
    return 0;
}

char *FUN_140108334(int64_t param_1)
{
    /* UNIMPLEMENTED @0xFUN_140108334 — decompile-failed, body 未还原 */
/* @0x140108334 size=17 */
    return 0;
}

void *FUN_140108348(void *param_1, uint32_t param_2)
{
    /* UNIMPLEMENTED @0xFUN_140108348 — decompile-failed, body 未还原 */
/* @0x140108348 size=47 */
    return 0;
}

void FUN_140108378(uint64_t value)
{
    /* UNIMPLEMENTED @0xFUN_140108378 — decompile-failed, body 未还原 */
/* @0x140108378 size=8 设置 CRT new-handler 指针 */
    g_u64E598 = value;
}

uint64_t _callnewh(void)
{
    /* @0x140108380 size=50 */
    return 0;
}

uint64_t _CxxThrowException(void)
{
    /* @0x1401083b4 size=145 */
    return 0;
}

/* @0x140108450 size=9 SKIP(CRT): MSVC CRT EncodePointer 包装 (jump table)，不还原 */
void FUN_140108450(void)
{
    /* UNIMPLEMENTED @0xFUN_140108450 — decompile-failed, body 未还原 */
}

uint64_t _mtterm(void)
{
    /* @0x140108464 size=37 */
    return 0;
}

uint64_t _initptd(void)
{
    /* @0x14010848c size=178 */
    return 0;
}

uint64_t _getptd_noexit(void)
{
    /* @0x140108540 size=132 */
    return 0;
}

uint64_t _freefls(void)
{
    /* @0x1401085e8 size=309 */
    return 0;
}

uint64_t _mtinit(void)
{
    /* @0x140108720 size=129 */
    return 0;
}

uint64_t _malloc_crt(void)
{
    /* @0x1401087a4 size=108 */
    return 0;
}

uint64_t _calloc_crt(void)
{
    /* @0x140108810 size=131 */
    return 0;
}

uint64_t _realloc_crt(void)
{
    /* @0x140108894 size=133 */
    return 0;
}

uint64_t _recalloc_crt(void)
{
    /* @0x14010891c size=136 */
    return 0;
}

uint64_t _msize(void)
{
    /* @0x1401089a4 size=70 */
    return 0;
}

uint64_t __crtExitProcess(void)
{
    /* @0x140108a58 size=21 */
    return 0;
}

void FUN_140108a70(void)
{
    /* UNIMPLEMENTED @0xFUN_140108a70 — decompile-failed, body 未还原 */
/* @0x140108a70 size=10 */
    /* SKIP(CRT): _lock(8) 内部调用 */
}



void FUN_140108a7c(void)
{
    /* UNIMPLEMENTED @0xFUN_140108a7c — decompile-failed, body 未还原 */
/* @0x140108a7c size=10 */
    /* SKIP(CRT): 调用 FUN_14010b504(8)，MSVC CRT abort 处理器 */
}

uint64_t _cinit(void)
{
    /* @0x140108af8 size=175 */
    return 0;
}

uint64_t doexit(void)
{
    /* @0x140108ba8 size=393 */
    return 0;
}

void FUN_140108d34(uint32_t param_1)
{
    /* UNIMPLEMENTED @0xFUN_140108d34 — decompile-failed, body 未还原 */
/* @0x140108d34 size=10 */
    /* SKIP(CRT): doexit 包装, MSVC CRT 退出处理 */
    (void)param_1;
}

void FUN_140108d40(uint32_t param_1)
{
    /* UNIMPLEMENTED @0xFUN_140108d40 — decompile-failed, body 未还原 */
/* @0x140108d40 size=12 */
    /* SKIP(CRT): 内部调用 doexit，MSVC CRT 退出处理 */
    (void)param_1;
}

uint64_t _init_pointers(void)
{
    /* @0x140108d6c size=103 */
    return 0;
}

uint64_t __C_specific_handler(void)
{
    /* @0x140108dd4 size=417 */
    return 0;
}

uint64_t _flsbuf(void)
{
    /* @0x140108f78 size=397 */
    return 0;
}

uint64_t write_char(void)
{
    /* @0x140109108 size=55 */
    return 0;
}

uint64_t write_multi_char(void)
{
    /* @0x140109140 size=81 */
    return 0;
}

uint64_t write_string(void)
{
    /* @0x140109194 size=131 */
    return 0;
}

uint64_t _woutput_l(void)
{
    /* @0x140109218 size=2829 */
    return 0;
}

void FUN_140109d28(uint64_t value)
{
    /* UNIMPLEMENTED @0xFUN_140109d28 — decompile-failed, body 未还原 */
/* @0x140109d28 size=8 设置 CRT watson 回调指针 */
    g_u64E600 = value;
}

uint64_t _invalid_parameter(void)
{
    /* @0x140109e58 size=133 */
    return 0;
}

uint64_t _get_errno_from_oserr(void)
{
    /* @0x140109ee0 size=71 */
    return 0;
}

uint64_t __doserrno(void)
{
    /* @0x140109f28 size=32 */
    return 0;
}

uint64_t __doserrno_140109f48(void)
{
    /* @0x140109f48 size=32 */
    return 0;
}

uint64_t _dosmaperr(void)
{
    /* @0x140109f68 size=70 */
    return 0;
}

uint64_t setSBCS(void)
{
    /* @0x140109fb0 size=146 */
    return 0;
}

uint64_t setSBUpLow(void)
{
    /* @0x14010a044 size=483 */
    return 0;
}

uint64_t __updatetmbcinfo(void)
{
    /* @0x14010a228 size=188 */
    return 0;
}

uint64_t getSystemCP(void)
{
    /* @0x14010a2e4 size=144 */
    return 0;
}

uint64_t _setmbcp_nolock(void)
{
    /* @0x14010a374 size=631 */
    return 0;
}

uint64_t _setmbcp(void)
{
    /* @0x14010a5ec size=504 */
    return 0;
}

uint64_t __initmbctable(void)
{
    /* @0x14010a7e4 size=40 */
    return 0;
}

uint64_t __freetlocinfo(void)
{
    /* @0x14010a80c size=378 */
    return 0;
}

uint64_t __addlocaleref(void)
{
    /* @0x14010a988 size=149 */
    return 0;
}

uint64_t __removelocaleref(void)
{
    /* @0x14010aa20 size=165 */
    return 0;
}

uint64_t _updatetlocinfoEx_nolock(void)
{
    /* @0x14010aac8 size=87 */
    return 0;
}

uint64_t __updatetlocinfo(void)
{
    /* @0x14010ab20 size=117 */
    return 0;
}

uint64_t __GSHandlerCheckCommon(void)
{
    /* @0x14010ac94 size=99 */
    return 0;
}

uint64_t __GSHandlerCheck(void)
{
    /* @0x14010acf8 size=29 */
    return 0;
}

void FUN_14010ae40(uint16_t *param_1, uint64_t param_2)
{
    /* UNIMPLEMENTED @0xFUN_14010ae40 — decompile-failed, body 未还原 */
/* @0x14010ae40 size=31 */
    /* SKIP(CRT): wcsnlen 风格的 MSVC CRT 长度函数 */
    (void)param_1;
    (void)param_2;
}

uint64_t __security_check_cookie(void)
{
    /* @0x14010ae70 size=31 */
    return 0;
}

uint64_t _ioinit(void)
{
    /* @0x14010ae90 size=746 */
    return 0;
}

uint64_t _fcloseall(void)
{
    /* @0x14010b180 size=176 */
    return 0;
}

uint64_t _flush(void)
{
    /* @0x14010b230 size=121 */
    return 0;
}

uint64_t _fflush_nolock(void)
{
    /* @0x14010b2ac size=71 */
    return 0;
}

uint64_t flsall(void)
{
    /* @0x14010b2f4 size=244 */
    return 0;
}

void FUN_14010b3e8(void)
{
    /* UNIMPLEMENTED @0xFUN_14010b3e8 — decompile-failed, body 未还原 */
/* @0x14010b3e8 size=10 */
    /* SKIP(CRT): flsall(1) 内部调用 */
}



uint64_t _mtinitlocks(void)
{
    /* @0x14010b3f4 size=138 */
    return 0;
}

uint64_t _mtdeletelocks(void)
{
    /* @0x14010b480 size=131 */
    return 0;
}

/* @0x14010b504 size=24 SKIP(CRT): 反编译体仅对 CRT 锁表 &DAT_14013b5c0[param_1*2]
   调用 LeaveCriticalSection，属 CRT 锁包装，不还原。 */
void FUN_14010b504(int param_1)
{
    /* UNIMPLEMENTED @0xFUN_14010b504 — decompile-failed, body 未还原 */
(void)param_1;
}

uint64_t _mtinitlocknum(void)
{
    /* @0x14010b51c size=232 */
    return 0;
}

uint64_t _lock(void)
{
    /* @0x14010b604 size=68 */
    return 0;
}

void *FUN_14010b648(void)
{
    /* UNIMPLEMENTED @0xFUN_14010b648 — decompile-failed, body 未还原 */
/* @0x14010b648 size=8 返回 CRT 错误信息表 */
    return (void *)(uintptr_t)&g_u64B960;
}

void **FUN_14010b650(void)
{
    /* UNIMPLEMENTED @0xFUN_14010b650 — decompile-failed, body 未还原 */
/* @0x14010b650 size=8 返回 CRT "No error" 串指针表 */
    return (void **)(uintptr_t)&PTR_s_No_error_14013b800;
}

uint64_t _free_osfhnd(void)
{
    /* @0x14010b658 size=172 */
    return 0;
}

uint64_t _get_osfhandle(void)
{
    /* @0x14010b704 size=132 */
    return 0;
}

uint64_t __lock_fhandle(void)
{
    /* @0x14010b788 size=166 */
    return 0;
}

uint64_t _unlock_fhandle(void)
{
    /* @0x14010b830 size=40 */
    return 0;
}

uint64_t _write_nolock(void)
{
    /* @0x14010b858 size=1886 */
    return 0;
}

uint64_t _write(void)
{
    /* @0x14010bfb8 size=305 */
    return 0;
}

uint64_t _fassign_l(void)
{
    /* @0x14010c2e0 size=66 */
    return 0;
}

uint64_t _cftoe2_l(void)
{
    /* @0x14010c33c size=599 */
    return 0;
}

uint64_t _cftoe_l(void)
{
    /* @0x14010c594 size=268 */
    return 0;
}

uint64_t _cftof_l(void)
{
    /* @0x14010cc90 size=230 */
    return 0;
}

uint64_t _cftog_l(void)
{
    /* @0x14010cd78 size=324 */
    return 0;
}

uint64_t _cfltcvt_l(void)
{
    /* @0x14010cebc size=149 */
    return 0;
}

uint64_t _cfltcvt(void)
{
    /* @0x14010cf54 size=36 */
    return 0;
}

uint64_t _initp_misc_cfltcvt_tab(void)
{
    /* @0x14010cf78 size=54 */
    return 0;
}

uint64_t _stbuf(void)
{
    /* @0x14010cfb0 size=209 */
    return 0;
}

uint64_t _ftbuf(void)
{
    /* @0x14010d084 size=53 */
    return 0;
}

uint64_t _fileno(void)
{
    /* @0x14010d258 size=54 */
    return 0;
}

uint64_t _wchartodigit(void)
{
    /* @0x14010d290 size=405 */
    return 0;
}

uint64_t _iswctype_l(void)
{
    /* @0x14010d428 size=186 */
    return 0;
}

uint64_t iswctype(void)
{
    /* @0x14010d4e4 size=144 */
    return 0;
}

uint64_t _heap_init(void)
{
    /* @0x14010d574 size=76 */
    return 0;
}

uint64_t _calloc_impl(void)
{
    /* @0x14010d5c0 size=175 */
    return 0;
}

uint64_t _NMSG_WRITE(void)
{
    /* @0x14010d670 size=549 */
    return 0;
}

uint64_t _FF_MSGBANNER(void)
{
    /* @0x14010d898 size=67 */
    return 0;
}

uint64_t _controlfp_s(void)
{
    /* @0x14010d8dc size=115 */
    return 0;
}

uint64_t FUN_14010d950(int param_1)
{
    /* UNIMPLEMENTED @0xFUN_14010d950 — decompile-failed, body 未还原 */
/* @0x14010d950 size=101 */
    /* SKIP(CRT): _ctrlfp/_set_errno_from_matherr 浮点异常处理 */
    (void)param_1;
    return 0;
}

uint64_t _exception_enabled(void)
{
    /* @0x14010d9b8 size=185 */
    return 0;
}

/* @0x14010da74 size=335 SKIP(CRT): MSVC CRT 浮点 (_ctrlfp/_exception_enabled/_raise_exc/_set_errno_from_matherr)，不还原 */
uint64_t FUN_14010da74(uint64_t param_1, unsigned int param_2, uint64_t param_3, int param_4,
                       unsigned int param_5, uint64_t param_6, uint64_t param_7,
                       uint64_t param_8, int param_9)
{
    (void)param_1; (void)param_2; (void)param_3; (void)param_4; (void)param_5;
    (void)param_6; (void)param_7; (void)param_8; (void)param_9;
    return 0;
}

/* @0x14010dbc4 size=782 SKIP(CRT): MSVC CRT 浮点 (调用 FUN_14010da74)，不还原 */
uint64_t FUN_14010dbc4(uint64_t param_1, uint64_t param_2, int param_3, uint64_t param_4)
{
    /* UNIMPLEMENTED @0xFUN_14010dbc4 — decompile-failed, body 未还原 */
(void)param_1; (void)param_2; (void)param_3; (void)param_4;
    return 0;
}

void FUN_14010ded4(uint64_t param_1, double *param_2, double *param_3, uint32_t *param_4)
{
    /* UNIMPLEMENTED @0xFUN_14010ded4 — decompile-failed, body 未还原 */
/* @0x14010ded4 size=1281 */
    /* SKIP(CRT): long double 转换，MSVC CRT 内联实现 */
    (void)param_1;
    (void)param_2;
    (void)param_3;
    (void)param_4;
}

uint64_t _tzset_nolock(void)
{
    /* @0x14010e3d8 size=1175 */
    return 0;
}

uint64_t cvtdate(void)
{
    /* @0x14010e878 size=696 */
    return 0;
}

uint64_t _isindst_nolock(void)
{
    /* @0x14010eb30 size=673 */
    return 0;
}

uint64_t __tzset(void)
{
    /* @0x14010edd4 size=59 */
    return 0;
}

uint64_t _isindst(void)
{
    /* @0x14010ee10 size=48 */
    return 0;
}

uint64_t _gmtime64_s(void)
{
    /* @0x14010ee40 size=716 */
    return 0;
}

uint64_t _get_daylight(void)
{
    /* @0x14010f10c size=63 */
    return 0;
}

uint64_t _get_dstbias(void)
{
    /* @0x14010f14c size=63 */
    return 0;
}

uint64_t _get_timezone(void)
{
    /* @0x14010f18c size=63 */
    return 0;
}

uint32_t *FUN_14010f1cc(void)
{
    /* UNIMPLEMENTED @0xFUN_14010f1cc — decompile-failed, body 未还原 */
/* @0x14010f1cc size=8 返回 CRT 时间缓冲区指针 */
    return (uint32_t *)(uintptr_t)&g_u64BF54;
}

uint32_t *FUN_14010f1d4(void)
{
    /* UNIMPLEMENTED @0xFUN_14010f1d4 — decompile-failed, body 未还原 */
/* @0x14010f1d4 size=8 返回 CRT 时间缓冲区指针 */
    return (uint32_t *)(uintptr_t)&g_u64BF58;
}

uint32_t *FUN_14010f1dc(void)
{
    /* UNIMPLEMENTED @0xFUN_14010f1dc — decompile-failed, body 未还原 */
/* @0x14010f1dc size=8 返回 CRT 时间缓冲区指针 */
    return (uint32_t *)(uintptr_t)&g_u64BF50;
}

void **FUN_14010f1e4(void)
{
    /* UNIMPLEMENTED @0xFUN_14010f1e4 — decompile-failed, body 未还原 */
/* @0x14010f1e4 size=8 返回 CRT 全局表指针 */
    return (void **)(uintptr_t)&PTR_DAT_14013bfe0;
}

uint64_t __getgmtimebuf(void)
{
    /* @0x14010f1ec size=70 */
    return 0;
}

uint64_t _set_abort_behavior(void)
{
    /* @0x14010f2d8 size=28 */
    return 0;
}

void FUN_14010f2f4(uint64_t value)
{
    /* UNIMPLEMENTED @0xFUN_14010f2f4 — decompile-failed, body 未还原 */
/* @0x14010f2f4 size=8 设置 CRT 全局值 */
    g_u64EC88 = value;
}

uint64_t __check_float_string(void)
{
    /* @0x14010f2fc size=150 */
    return 0;
}

uint64_t _inc(void)
{
    /* @0x14010f394 size=38 */
    return 0;
}

uint64_t _input_l(void)
{
    /* @0x14010f3bc size=4480 */
    return 0;
}

uint64_t __StateFromIp(void)
{
    /* @0x14011053c size=138 */
    return 0;
}

void FUN_1401105c8(void *param_1, void *param_2)
{
    /* UNIMPLEMENTED @0xFUN_1401105c8 — decompile-failed, body 未还原 */
/* @0x1401105c8 size=8 */
    /* SKIP(CRT): CRT SEH/展开 __StateFromIp 包装 */
    (void)param_1;(void)param_2;
}

uint64_t __GetCurrentState(void)
{
    /* @0x1401105d0 size=39 */
    return 0;
}

uint64_t __SetState(void)
{
    /* @0x1401105f8 size=12 */
    return 0;
}

uint64_t __SetUnwindTryBlock(void)
{
    /* @0x140110604 size=59 */
    return 0;
}

uint64_t __GetUnwindTryBlock(void)
{
    /* @0x140110640 size=41 */
    return 0;
}

void *FUN_14011067c(void *param_1, uint32_t param_2)
{
    /* UNIMPLEMENTED @0xFUN_14011067c — decompile-failed, body 未还原 */
/* @0x14011067c size=57 */
    return 0;
}

uint64_t __TypeMatch(void)
{
    /* @0x1401106b8 size=291 */
    return 0;
}

uint64_t __DestructExceptionObject(void)
{
    /* @0x1401109bc size=61 */
    return 0;
}

uint64_t __AdjustPointer(void)
{
    /* @0x140110a00 size=35 */
    return 0;
}

uint8_t FUN_140110a24(int64_t param_1, int *param_2)
{
    /* UNIMPLEMENTED @0xFUN_140110a24 — decompile-failed, body 未还原 */
/* @0x140110a24 size=210 */
    /* SKIP(CRT): C++ SEH/异常类型匹配，MSVC CRT 辅助 */
    (void)param_1;
    (void)param_2;
    return 0;
}

uint64_t __BuildCatchObject(void)
{
    /* @0x140110f60 size=170 */
    return 0;
}

uint64_t CatchIt(void)
{
    /* @0x140111010 size=193 */
    return 0;
}

void FUN_140111318(uint64_t param_1, uint64_t param_2, uint64_t param_3,
                   uint64_t param_4, uint64_t param_5, uint8_t param_6, int param_7,
                   uint64_t param_8)
{
    /* @0x140111318 size=1367 */
    /* SKIP(CRT): MSVC C++ 异常处理 __InternalCxxFrameHandler 核心, 不还原. */
    (void)param_1; (void)param_2; (void)param_3; (void)param_4; (void)param_5;
    (void)param_6; (void)param_7; (void)param_8;
}

void *FUN_140111874(void *param_1, void *param_2)
{
    /* UNIMPLEMENTED @0xFUN_140111874 — decompile-failed, body 未还原 */
/* @0x140111874 size=33 */
    return 0;
}

uint64_t __InternalCxxFrameHandler(void)
{
    /* @0x140111898 size=549 */
    return 0;
}

/* @0x140111ae4 size=30 SKIP(CRT): MSVC CRT DecodePointer + terminate，不还原 */
void FUN_140111ae4(void)
{
    /* UNIMPLEMENTED @0xFUN_140111ae4 — decompile-failed, body 未还原 */
}

uint64_t _initp_eh_hooks(void)
{
    /* @0x140111b08 size=28 */
    return 0;
}

uint64_t _Type_info_dtor(void)
{
    /* @0x140111b24 size=113 */
    return 0;
}

uint64_t FUN_140111c60(uint64_t *param_1)
{
    /* UNIMPLEMENTED @0xFUN_140111c60 — decompile-failed, body 未还原 */
/* @0x140111c60 size=64 */
    return 0;
}

uint64_t FUN_140111ca4(void)
{
    /* UNIMPLEMENTED @0xFUN_140111ca4 — decompile-failed, body 未还原 */
/* @0x140111ca4 size=24 */
    return 0;
}

uint64_t _XcptFilter(void)
{
    /* @0x140111cbc size=462 */
    return 0;
}

uint64_t _wwincmdln(void)
{
    /* @0x140111e8c size=90 */
    return 0;
}

uint64_t _wsetenvp(void)
{
    /* @0x140111ee8 size=310 */
    return 0;
}

uint64_t wparse_cmdline(void)
{
    /* @0x140112020 size=408 */
    return 0;
}

uint64_t _wsetargv(void)
{
    /* @0x1401121b8 size=240 */
    return 0;
}

uint64_t __crtGetEnvironmentStringsW(void)
{
    /* @0x1401122a8 size=137 */
    return 0;
}

uint64_t _RTC_Initialize(void)
{
    /* @0x14011233c size=56 */
    return 0;
}

uint64_t FUN_140112374(void)
{
    /* UNIMPLEMENTED @0xFUN_140112374 — decompile-failed, body 未还原 */
/* @0x140112374 size=56 */
    return 0;
}

uint64_t __security_init_cookie(void)
{
    /* @0x1401123ac size=179 */
    return 0;
}

uint64_t _ValidateImageBase(void)
{
    /* @0x140112650 size=47 */
    return 0;
}

uint64_t _FindPESection(void)
{
    /* @0x140112680 size=70 */
    return 0;
}

uint64_t _IsNonwritableInCurrentImage(void)
{
    /* @0x1401126d0 size=63 */
    return 0;
}

uint64_t _initp_misc_winsig(void)
{
    /* @0x140112714 size=29 */
    return 0;
}

void FUN_140112734(void)
{
    /* UNIMPLEMENTED @0xFUN_140112734 — decompile-failed, body 未还原 */
/* @0x140112734 size=12 */
    /* SKIP(CRT): DecodePointer 调用, MSVC CRT */
}



uint64_t raise(void)
{
    /* @0x140112740 size=625 */
    return 0;
}

void FUN_1401129b4(uint64_t value)
{
    /* UNIMPLEMENTED @0xFUN_1401129b4 — decompile-failed, body 未还原 */
/* @0x1401129b4 size=8 设置 CRT 全局值 */
    g_u64EEE8 = value;
}

void FUN_1401129bc(uint64_t value)
{
    /* UNIMPLEMENTED @0xFUN_1401129bc — decompile-failed, body 未还原 */
/* @0x1401129bc size=8 设置 CRT 全局值 */
    g_u64EEF0 = value;
}

uint64_t __crtInitCritSecAndSpinCount(void)
{
    /* @0x1401129c4 size=27 */
    return 0;
}

uint64_t FUN_140112a10(void *param_1, void *param_2)
{
    /* UNIMPLEMENTED @0xFUN_140112a10 — decompile-failed, body 未还原 */
/* @0x140112a10 size=36 */
    return 0;
}

void FUN_140112a40(void)
{
    /* UNIMPLEMENTED @0xFUN_140112a40 — decompile-failed, body 未还原 */
/* @0x140112a40 size=25 */
    return;
}

uint64_t _lseeki64_nolock(void)
{
    /* @0x140112a74 size=149 */
    return 0;
}

uint64_t _lseeki64(void)
{
    /* @0x140112b0c size=311 */
    return 0;
}

uint64_t _getbuf(void)
{
    /* @0x140112c44 size=83 */
    return 0;
}

uint64_t _isatty(void)
{
    /* @0x140112c98 size=111 */
    return 0;
}

uint64_t _fputwc_nolock(void)
{
    /* @0x140112d08 size=487 */
    return 0;
}

uint64_t _get_printf_count_output(void)
{
    /* @0x140112ef0 size=24 */
    return 0;
}

void FUN_14011307c(wchar_t *param_1, char *param_2, size_t param_3)
{
    /* UNIMPLEMENTED @0xFUN_14011307c — decompile-failed, body 未还原 */
/* @0x14011307c size=8 */
    /* SKIP(CRT): 内部调用 _mbtowc_l，MSVC CRT 多字节转宽字符 */
    (void)param_1;
    (void)param_2;
    (void)param_3;
}

void FUN_140113084(void)
{
    /* UNIMPLEMENTED @0xFUN_140113084 — decompile-failed, body 未还原 */
/* @0x140113084 size=8 清零 CRT 全局标志 */
    g_u64F520 = 0;
}

uint64_t __crtLCMapStringA_stat(void)
{
    /* @0x14011308c size=1341 */
    return 0;
}

uint64_t __crtGetStringTypeA_stat(void)
{
    /* @0x140113664 size=618 */
    return 0;
}

uint64_t __free_lconv_num(void)
{
    /* @0x140113b48 size=72 */
    return 0;
}

uint64_t __free_lconv_mon(void)
{
    /* @0x140113b90 size=149 */
    return 0;
}

uint64_t ___lc_codepage_func(void)
{
    /* @0x140113c28 size=55 */
    return 0;
}

uint64_t __report_gsfailure(void)
{
    /* @0x140113e98 size=330 */
    return 0;
}

uint64_t _commit(void)
{
    /* @0x1401140e8 size=275 */
    return 0;
}

uint64_t _putwch_nolock(void)
{
    /* @0x1401141fc size=276 */
    return 0;
}

uint64_t __dtold(void)
{
    /* @0x140114938 size=214 */
    return 0;
}

uint64_t _fltout2(void)
{
    /* @0x140114a10 size=192 */
    return 0;
}

uint64_t __crtMessageBoxA(void)
{
    /* @0x140114b40 size=500 */
    return 0;
}

uint64_t _set_error_mode(void)
{
    /* @0x140114d34 size=80 */
    return 0;
}

uint64_t _control87(void)
{
    /* @0x140114d84 size=627 */
    return 0;
}

uint64_t _raise_exc_ex(void)
{
    /* @0x140115020 size=825 */
    return 0;
}

uint64_t _raise_exc(void)
{
    /* @0x14011535c size=39 */
    return 0;
}

uint64_t _set_errno_from_matherr(void)
{
    /* @0x140115384 size=45 */
    return 0;
}

uint64_t _statfp(void)
{
    /* @0x1401153b4 size=17 */
    return 0;
}

uint64_t _clrfp(void)
{
    /* @0x1401153c8 size=29 */
    return 0;
}

uint64_t _ctrlfp(void)
{
    /* @0x1401153e8 size=102 */
    return 0;
}

uint64_t _set_statfp(void)
{
    /* @0x140115468 size=31 */
    return 0;
}

void FUN_140115488(char *param_1)
{
    /* UNIMPLEMENTED @0xFUN_140115488 — decompile-failed, body 未还原 */
/* @0x140115488 size=11 */
    /* SKIP(CRT): strtol 包装 */
    (void)param_1;
}

uint64_t _getenv_helper_nolock(void)
{
    /* @0x140115494 size=161 */
    return 0;
}

uint64_t _filbuf(void)
{
    /* @0x140115538 size=346 */
    return 0;
}

uint64_t _ungetc_nolock(void)
{
    /* @0x140115694 size=283 */
    return 0;
}

uint64_t _flswbuf(void)
{
    /* @0x1401157c4 size=399 */
    return 0;
}

uint64_t wctomb_s(void)
{
    /* @0x140115b64 size=20 */
    return 0;
}

uint64_t strtol(void)
{
    /* @0x140115dfc size=48 */
    return 0;
}

uint64_t __ansicp(void)
{
    /* @0x140115e2c size=83 */
    return 0;
}

uint64_t __convertcp(void)
{
    /* @0x140115e80 size=645 */
    return 0;
}

uint64_t __ascii_strnicmp(void)
{
    /* @0x140116108 size=71 */
    return 0;
}

uint64_t _close_nolock(void)
{
    /* @0x140116264 size=188 */
    return 0;
}

uint64_t _close(void)
{
    /* @0x140116320 size=280 */
    return 0;
}

uint64_t _freebuf(void)
{
    /* @0x140116438 size=55 */
    return 0;
}

uint64_t __initconout(void)
{
    /* @0x140116470 size=59 */
    return 0;
}

uint64_t __termcon(void)
{
    /* @0x1401164ac size=59 */
    return 0;
}

uint64_t __strgtold12_l(void)
{
    /* @0x1401171fc size=2297 */
    return 0;
}

/* @0x140118660 size=10 SKIP(CRT): MSVC CRT 设置 MXCSR 寄存器，不还原 */
void FUN_140118660(unsigned int param_1)
{
    /* UNIMPLEMENTED @0xFUN_140118660 — decompile-failed, body 未还原 */
(void)param_1;
}

uint64_t _fclrf(void)
{
    /* @0x14011866a size=20 */
    return 0;
}

void FUN_140118790(unsigned char *param_1, unsigned char *param_2, size_t param_3)
{
    /* UNIMPLEMENTED @0xFUN_140118790 — decompile-failed, body 未还原 */
/* @0x140118790 size=8 */
    /* SKIP(CRT): 内部调用 _mbsnbicoll_l，MSVC CRT 多字节字符串比较 */
    (void)param_1;
    (void)param_2;
    (void)param_3;
}

uint64_t __wtomb_environ(void)
{
    /* @0x140118798 size=242 */
    return 0;
}

uint64_t _read_nolock(void)
{
    /* @0x14011888c size=1919 */
    return 0;
}

uint64_t _read(void)
{
    /* @0x14011900c size=370 */
    return 0;
}

uint64_t __mtold12(void)
{
    /* @0x140119180 size=542 */
    return 0;
}

uint64_t __crtCompareStringA_stat(void)
{
    /* @0x1401193a0 size=1242 */
    return 0;
}

uint64_t __crtsetenv(void)
{
    /* @0x140119a20 size=944 */
    return 0;
}

void FUN_140119f38(uint8_t *s, uint32_t c)
{
    /* UNIMPLEMENTED @0xFUN_140119f38 — decompile-failed, body 未还原 */
/* @0x140119f38 size=8 _mbschr_l 包装；原调用丢弃返回值，无副作用 */
    (void)s;
    (void)c;
}

char *FUN_140119f49(char *param_1, char param_2)
{
    /* UNIMPLEMENTED @0xFUN_140119f49 — decompile-failed, body 未还原 */
/* @0x140119f49 size=24 */
    /* SKIP(CRT): strchr 实现 (import thunk 别名), 不还原. */
    (void)param_1;
    (void)param_2;
    return 0;
}

uint64_t StringVPrintfWorkerW(void)
{
    /* @0x140119f6c size=117 */
    return 0;
}

uint64_t StringLengthWorkerW(void)
{
    /* @0x140119fe4 size=56 */
    return 0;
}

uint64_t StringValidateDestW(void)
{
    /* @0x14011a01c size=46 */
    return 0;
}

uint64_t _CallSettingFrame(void)
{
    /* @0x14011a100 size=64 */
    return 0;
}

uint64_t _get_fpsr(void)
{
    /* @0x14011a150 size=16 */
    return 0;
}

uint64_t FUN_14011a24e(void)
{
    /* UNIMPLEMENTED @0xFUN_14011a24e — decompile-failed, body 未还原 */
/* @0x14011a24e size=20 */
    return 0;
}

uint64_t FUN_14011a262(void)
{
    /* UNIMPLEMENTED @0xFUN_14011a262 — decompile-failed, body 未还原 */
/* @0x14011a262 size=27 */
    return 0;
}

uint64_t FUN_14011a27d(void)
{
    /* UNIMPLEMENTED @0xFUN_14011a27d — decompile-failed, body 未还原 */
/* @0x14011a27d size=37 */
    return 0;
}

uint64_t FUN_14011a2e6(void *param_1)
{
    /* UNIMPLEMENTED @0xFUN_14011a2e6 — decompile-failed, body 未还原 */
/* @0x14011a2e6 size=28 */
    return 0;
}

uint64_t FUN_14011a302(void)
{
    /* UNIMPLEMENTED @0xFUN_14011a302 — decompile-failed, body 未还原 */
/* @0x14011a302 size=25 */
    return 0;
}

uint64_t FUN_14011a322(void)
{
    /* UNIMPLEMENTED @0xFUN_14011a322 — decompile-failed, body 未还原 */
/* @0x14011a322 size=25 */
    return 0;
}

uint64_t FUN_14011a33b(uint64_t param_1, int64_t param_2)
{
    /* UNIMPLEMENTED @0xFUN_14011a33b — decompile-failed, body 未还原 */
/* @0x14011a33b size=34 */
    return 0;
}

uint64_t FUN_14011a35d(void)
{
    /* UNIMPLEMENTED @0xFUN_14011a35d — decompile-failed, body 未还原 */
/* @0x14011a35d size=25 */
    return 0;
}

uint64_t FUN_14011a376(void)
{
    /* UNIMPLEMENTED @0xFUN_14011a376 — decompile-failed, body 未还原 */
/* @0x14011a376 size=25 */
    return 0;
}

uint64_t FUN_14011a38f(void)
{
    /* UNIMPLEMENTED @0xFUN_14011a38f — decompile-failed, body 未还原 */
/* @0x14011a38f size=25 */
    return 0;
}

uint64_t FUN_14011a3a8(uint64_t param_1, int64_t param_2)
{
    /* UNIMPLEMENTED @0xFUN_14011a3a8 — decompile-failed, body 未还原 */
/* @0x14011a3a8 size=38 */
    return 0;
}

uint64_t FUN_14011a3d8(void)
{
    /* UNIMPLEMENTED @0xFUN_14011a3d8 — decompile-failed, body 未还原 */
/* @0x14011a3d8 size=25 */
    return 0;
}

uint64_t FUN_14011a3f1(void)
{
    /* UNIMPLEMENTED @0xFUN_14011a3f1 — decompile-failed, body 未还原 */
/* @0x14011a3f1 size=28 */
    return 0;
}

uint64_t FUN_14011a40d(void)
{
    /* UNIMPLEMENTED @0xFUN_14011a40d — decompile-failed, body 未还原 */
/* @0x14011a40d size=25 */
    return 0;
}

uint64_t FUN_14011a426(void)
{
    /* UNIMPLEMENTED @0xFUN_14011a426 — decompile-failed, body 未还原 */
/* @0x14011a426 size=25 */
    return 0;
}

uint64_t FUN_14011a43f(void)
{
    /* UNIMPLEMENTED @0xFUN_14011a43f — decompile-failed, body 未还原 */
/* @0x14011a43f size=25 */
    return 0;
}

uint64_t FUN_14011a458(uint64_t *param_1)
{
    /* UNIMPLEMENTED @0xFUN_14011a458 — decompile-failed, body 未还原 */
/* @0x14011a458 size=20 */
    return 0;
}

bool FUN_14011a4a0(uint64_t param_1, int64_t param_2)
{
    /* UNIMPLEMENTED @0xFUN_14011a4a0 — decompile-failed, body 未还原 */
/* @0x14011a4a0 size=23 */
    return 0;
}

uint32_t FUN_14011a4b7(uint64_t param_1, int64_t param_2)
{
    /* UNIMPLEMENTED @0xFUN_14011a4b7 — decompile-failed, body 未还原 */
/* @0x14011a4b7 size=243 */
    return 0;
}

uint64_t FUN_14011a647(void)
{
    /* UNIMPLEMENTED @0xFUN_14011a647 — decompile-failed, body 未还原 */
/* @0x14011a647 size=25 */
    return 0;
}

bool FUN_14011a660(uint64_t *param_1)
{
    /* UNIMPLEMENTED @0xFUN_14011a660 — decompile-failed, body 未还原 */
/* @0x14011a660 size=33 */
    return 0;
}

uint64_t FUN_14011a681(uint64_t param_1, int64_t param_2)
{
    /* UNIMPLEMENTED @0xFUN_14011a681 — decompile-failed, body 未还原 */
/* @0x14011a681 size=28 */
    return 0;
}

bool FUN_14011a69d(uint64_t *param_1)
{
    /* UNIMPLEMENTED @0xFUN_14011a69d — decompile-failed, body 未还原 */
/* @0x14011a69d size=34 */
    return 0;
}

uint64_t FUN_14011a6bf(uint64_t param_1, int64_t param_2)
{
    /* UNIMPLEMENTED @0xFUN_14011a6bf — decompile-failed, body 未还原 */
/* @0x14011a6bf size=24 */
    return 0;
}

uint64_t FUN_14011a6d7(uint64_t *param_1)
{
    /* UNIMPLEMENTED @0xFUN_14011a6d7 — decompile-failed, body 未还原 */
/* @0x14011a6d7 size=47 */
    return 0;
}

uint64_t FUN_14011a706(uint64_t *param_1)
{
    /* UNIMPLEMENTED @0xFUN_14011a706 — decompile-failed, body 未还原 */
/* @0x14011a706 size=43 */
    return 0;
}

uint64_t FUN_14011a731(uint64_t param_1, int64_t param_2)
{
    /* UNIMPLEMENTED @0xFUN_14011a731 — decompile-failed, body 未还原 */
/* @0x14011a731 size=23 */
    return 0;
}

uint64_t FUN_14011a748(uint64_t param_1, int64_t param_2)
{
    /* UNIMPLEMENTED @0xFUN_14011a748 — decompile-failed, body 未还原 */
/* @0x14011a748 size=23 */
    return 0;
}

uint64_t FUN_14011a808(void)
{
    /* UNIMPLEMENTED @0xFUN_14011a808 — decompile-failed, body 未还原 */
/* @0x14011a808 size=33 */
    return 0;
}

uint64_t FUN_14011a844(void)
{
    /* UNIMPLEMENTED @0xFUN_14011a844 — decompile-failed, body 未还原 */
/* @0x14011a844 size=33 */
    return 0;
}

uint64_t FUN_14011a868(void)
{
    /* UNIMPLEMENTED @0xFUN_14011a868 — decompile-failed, body 未还原 */
/* @0x14011a868 size=34 */
    return 0;
}

uint64_t FUN_14011a88c(void)
{
    /* UNIMPLEMENTED @0xFUN_14011a88c — decompile-failed, body 未还原 */
/* @0x14011a88c size=35 */
    return 0;
}

uint64_t FUN_14011a8b0(void)
{
    /* UNIMPLEMENTED @0xFUN_14011a8b0 — decompile-failed, body 未还原 */
/* @0x14011a8b0 size=33 */
    return 0;
}

uint64_t FUN_14011a8d4(void)
{
    /* UNIMPLEMENTED @0xFUN_14011a8d4 — decompile-failed, body 未还原 */
/* @0x14011a8d4 size=33 */
    return 0;
}

uint64_t FUN_14011a8f8(void)
{
    /* UNIMPLEMENTED @0xFUN_14011a8f8 — decompile-failed, body 未还原 */
/* @0x14011a8f8 size=32 */
    return 0;
}

uint64_t FUN_14011a918(void)
{
    /* UNIMPLEMENTED @0xFUN_14011a918 — decompile-failed, body 未还原 */
/* @0x14011a918 size=32 */
    return 0;
}

uint64_t FUN_14011a938(void)
{
    /* UNIMPLEMENTED @0xFUN_14011a938 — decompile-failed, body 未还原 */
/* @0x14011a938 size=33 */
    return 0;
}

uint64_t FUN_14011a968(void)
{
    /* UNIMPLEMENTED @0xFUN_14011a968 — decompile-failed, body 未还原 */
/* @0x14011a968 size=33 */
    return 0;
}

uint64_t FUN_14011a9b0(void)
{
    /* UNIMPLEMENTED @0xFUN_14011a9b0 — decompile-failed, body 未还原 */
/* @0x14011a9b0 size=33 */
    return 0;
}

uint64_t FUN_14011a9d4(void)
{
    /* UNIMPLEMENTED @0xFUN_14011a9d4 — decompile-failed, body 未还原 */
/* @0x14011a9d4 size=33 */
    return 0;
}

uint64_t FUN_14011a9f8(void)
{
    /* UNIMPLEMENTED @0xFUN_14011a9f8 — decompile-failed, body 未还原 */
/* @0x14011a9f8 size=33 */
    return 0;
}

uint64_t FUN_14011aa80(void)
{
    /* UNIMPLEMENTED @0xFUN_14011aa80 — decompile-failed, body 未还原 */
/* @0x14011aa80 size=33 */
    return 0;
}

uint64_t FUN_14011aaa4(void)
{
    /* UNIMPLEMENTED @0xFUN_14011aaa4 — decompile-failed, body 未还原 */
/* @0x14011aaa4 size=12 */
    return 0;
}

uint64_t FUN_14011aab0(void)
{
    /* UNIMPLEMENTED @0xFUN_14011aab0 — decompile-failed, body 未还原 */
/* @0x14011aab0 size=12 */
    return 0;
}

uint64_t FUN_14011aabc(void)
{
    /* UNIMPLEMENTED @0xFUN_14011aabc — decompile-failed, body 未还原 */
/* @0x14011aabc size=12 */
    return 0;
}

uint64_t FUN_14011aac8(void)
{
    /* UNIMPLEMENTED @0xFUN_14011aac8 — decompile-failed, body 未还原 */
/* @0x14011aac8 size=12 */
    return 0;
}

uint64_t FUN_14011aad4(void)
{
    /* UNIMPLEMENTED @0xFUN_14011aad4 — decompile-failed, body 未还原 */
/* @0x14011aad4 size=12 */
    return 0;
}

uint64_t FUN_14011aae0(void)
{
    /* UNIMPLEMENTED @0xFUN_14011aae0 — decompile-failed, body 未还原 */
/* @0x14011aae0 size=12 */
    return 0;
}

uint64_t FUN_14011aaec(void)
{
    /* UNIMPLEMENTED @0xFUN_14011aaec — decompile-failed, body 未还原 */
/* @0x14011aaec size=12 */
    return 0;
}

uint64_t FUN_14011aaf8(void)
{
    /* UNIMPLEMENTED @0xFUN_14011aaf8 — decompile-failed, body 未还原 */
/* @0x14011aaf8 size=12 */
    return 0;
}

uint64_t FUN_14011ab04(void)
{
    /* UNIMPLEMENTED @0xFUN_14011ab04 — decompile-failed, body 未还原 */
/* @0x14011ab04 size=12 */
    return 0;
}

uint64_t FUN_14011ab10(void)
{
    /* UNIMPLEMENTED @0xFUN_14011ab10 — decompile-failed, body 未还原 */
/* @0x14011ab10 size=12 */
    return 0;
}

uint64_t FUN_14011ab1c(void)
{
    /* UNIMPLEMENTED @0xFUN_14011ab1c — decompile-failed, body 未还原 */
/* @0x14011ab1c size=12 */
    return 0;
}

uint64_t FUN_14011ab28(void)
{
    /* UNIMPLEMENTED @0xFUN_14011ab28 — decompile-failed, body 未还原 */
/* @0x14011ab28 size=12 */
    return 0;
}

uint64_t FUN_14011ab34(void)
{
    /* UNIMPLEMENTED @0xFUN_14011ab34 — decompile-failed, body 未还原 */
/* @0x14011ab34 size=12 */
    return 0;
}

uint64_t FUN_14011ab40(void)
{
    /* UNIMPLEMENTED @0xFUN_14011ab40 — decompile-failed, body 未还原 */
/* @0x14011ab40 size=12 */
    return 0;
}

uint64_t FUN_14011ab5c(void)
{
    /* UNIMPLEMENTED @0xFUN_14011ab5c — decompile-failed, body 未还原 */
/* @0x14011ab5c size=12 */
    return 0;
}

uint64_t FUN_14011ab68(void)
{
    /* UNIMPLEMENTED @0xFUN_14011ab68 — decompile-failed, body 未还原 */
/* @0x14011ab68 size=12 */
    return 0;
}

uint64_t FUN_14011ab9c(void)
{
    /* UNIMPLEMENTED @0xFUN_14011ab9c — decompile-failed, body 未还原 */
/* @0x14011ab9c size=34 */
    return 0;
}

uint64_t FUN_14011abf0(void)
{
    /* UNIMPLEMENTED @0xFUN_14011abf0 — decompile-failed, body 未还原 */
/* @0x14011abf0 size=12 */
    return 0;
}

uint64_t FUN_14011ac0c(void)
{
    /* UNIMPLEMENTED @0xFUN_14011ac0c — decompile-failed, body 未还原 */
/* @0x14011ac0c size=12 */
    return 0;
}

uint64_t FUN_14011ac18(void)
{
    /* UNIMPLEMENTED @0xFUN_14011ac18 — decompile-failed, body 未还原 */
/* @0x14011ac18 size=12 */
    return 0;
}

uint64_t FUN_14011ac24(void)
{
    /* UNIMPLEMENTED @0xFUN_14011ac24 — decompile-failed, body 未还原 */
/* @0x14011ac24 size=12 */
    return 0;
}

uint64_t FUN_14011ac90(void)
{
    /* UNIMPLEMENTED @0xFUN_14011ac90 — decompile-failed, body 未还原 */
/* @0x14011ac90 size=12 */
    return 0;
}

uint64_t FUN_14011acb4(void)
{
    /* UNIMPLEMENTED @0xFUN_14011acb4 — decompile-failed, body 未还原 */
/* @0x14011acb4 size=32 */
    return 0;
}

uint64_t FUN_14011acd4(void)
{
    /* UNIMPLEMENTED @0xFUN_14011acd4 — decompile-failed, body 未还原 */
/* @0x14011acd4 size=12 */
    return 0;
}

uint64_t FUN_14011acf0(void)
{
    /* UNIMPLEMENTED @0xFUN_14011acf0 — decompile-failed, body 未还原 */
/* @0x14011acf0 size=26 */
    return 0;
}

