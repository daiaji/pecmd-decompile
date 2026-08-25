/* ====================================================================
 * core_b3j.c — B3-B6 小函数批 10 (140040000-1400bffff)
 *
 * 本批新实现函数直接使用人类可读 PECMD_ 名称，原始地址保留在 @0x 注释。
 *
 * 来源: PECMD原始.EXE (x64, ImageBase=0x140000000)
 *   调整列表指针     FUN_14004F948 @0x14004f948
 *   初始化拖放目标   FUN_1400549BC @0x1400549bc
 *   构造窗口对象     PECMD_CreateWindowObjectEx @0x140055368
 *   主题绘制文本     FUN_14005B3E4 @0x14005b3e4
 *   添加托盘图标     FUN_14005B900 @0x14005b900
 *   获取提示窗口     PECMD_GetTooltipWindow @0x14005b9c8
 *   DPI 缩放         FUN_14005DAF8 @0x14005daf8
 *   释放缓存对象     PECMD_ReleaseSlotRef @0x14005e8e8
 *   创建注册表键     PECMD_RegCreateKeyRetryWow64 @0x14005f750
 *   查找空闲字母     PECMD_PickFreeDriveLetter @0x14005f868
 *   链接或复制文件   FUN_1400607A4 @0x1400607a4
 *   安装窗口过程     PECMD_SetWindowProcHook @0x140060870
 *   分配控制台       FUN_140060A94 @0x140060a94
 *   匹配关键字前缀   FUN_140062FC4 @0x140062fc4
 *   分配缓冲重试     FUN_140063A6C @0x140063a6c
 *   判定 visible 串  FUN_1400641D4 @0x1400641d4
 *   判定设备类别     FUN_14006643C @0x14006643c
 *   复制到分隔符     PECMD_CopyTokenTrimmed @0x14006764c
 *   转义 & 字符      PECMD_EscapeLabelAmpersands @0x140067f90
 *   字符串数组排序   PECMD_BubbleSortStringArray @0x140068c04
 *   获取下拉框文本   FUN_14006C4C8 @0x14006c4c8
 *   判定设备就绪     FUN_14006CAF0 @0x14006caf0
 *   释放 COM 对象    FUN_14006E4F4 @0x14006e4f4
 *   解析路径前缀     FUN_14007443C @0x14007443c
 *   解析数字或变量   PECMD_ParseNumOrVar @0x1400746b0
 *   转换字符串       FUN_1400799F0 @0x1400799f0
 *   显示盘型消息     FUN_14007C7EC @0x14007c7ec
 *   创建锁对象       FUN_1400A43C4 @0x1400a43c4
 *   创建命名锁       FUN_1400A4460 @0x1400a4460
 *   发送标题消息     FUN_1400A9650 @0x1400a9650
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

/* ---- 未实现依赖 (extern + TODO(verify)) ---- */
extern LRESULT FUN_1400E5890(int64_t a1);
extern void FUN_140053C5C(int64_t a1, uint32_t a2);
extern void PECMD_SetObjectVisibleVar(int64_t a1, uint32_t a2);
extern void FUN_14007D0AC(int64_t *a1, LPCWSTR a2, LPCWSTR a3);
extern uint64_t *PECMD_ItemBaseInit(uint64_t *obj, uint32_t id, uint64_t data, LPCWSTR name1,
                                    LPCWSTR name2, LPCWSTR name3, uint32_t flags1, uint32_t flags2,
                                    uint32_t flags3, uint32_t flags4);
extern uint16_t FUN_1400F172C(int64_t *a1, uint32_t a2, uint64_t a3, uint64_t *a4, int64_t a5,
                              uint32_t a6, int64_t *a7);
extern int64_t FUN_1400E5B0C(int64_t a1, uint64_t a2, HDC a3, int64_t *a4);
extern void FUN_1400F429C(WCHAR **pp, WCHAR ch);
extern uint64_t PECMD_EvalParenStripped(int64_t *a1, uint64_t *a2);
extern uint8_t PECMD_EjectDrive(WCHAR a1, int a2);
/* S11: 本地声明与定义冲突已删除, 统一采用 xproto.h 原型 (原: /* S11: 本地声明与定义冲突, 已删除, 统一采用 xproto.h 原型 (原: extern void PECMD_DefineDosDevice(uint8_t *a1, WCHAR *a2); * / ext) */

extern uint64_t *FUN_1400A41FC(LPCWSTR a1);
extern int64_t *PECMD_LoadImageFromFile(LPCWSTR a1);
extern void AtlThrowImpl(long a1);

/* ---- 已实现公共工具 (其他 core_*.c / core_globals.c) ---- */
/* S11: 本地声明与定义冲突已删除, 统一采用 xproto.h 原型 (原: /* S11: 本地声明与定义冲突, 已删除, 统一采用 xproto.h 原型 (原: extern int32_t FUN_1400630D0(int mode); * / extern void PECMD_Open) */


extern int32_t PECMD_LoadOle32Apis(void);
extern int64_t *PECMD_StrBldCopyAnsi(int64_t *out, const char *src, uint64_t len);
extern WCHAR **FUN_14007034C(WCHAR **ps, LPCWSTR src);
extern uint8_t *FUN_14001E69C(void *script, LPCWSTR name, void *scope, int64_t len);
extern uint64_t PECMD_CreateSymbolicLink(uint64_t a1, uint64_t a2, uint32_t a3);
extern void FUN_14005B7E8(char *nid);

/* ---- 本批引用的全局数据 ---- */
extern WCHAR g_szEmpty[]; /* g_szEmpty 空串 */
extern uint8_t PTR_FUN_1401235a8[];
extern uint8_t PTR_FUN_140128ed0[];
extern uint8_t g_b24e10[];
extern HWND g_hwndD310;
extern uint8_t g_msgWndMode;
extern uint8_t g_flagD6F6;

/* ========== FUN_14004F948 @0x14004f948 ==========
 * 按新基址/旧基址整体平移结构中的指针字段。
 */
void FUN_14004F948(int64_t unused, uint32_t *obj, int64_t newBase, int64_t oldBase)
{
    int64_t lVar1;
    int64_t lVar2;
    uint32_t uVar3;

    (void)unused;
    if (obj == NULL) {
        AtlThrowImpl(-0x7fffbffb);
    }
    *(int64_t *)(obj + 2) = newBase + ((*(int64_t *)(obj + 2) - oldBase) >> 1) * 2;
    *(int64_t *)(obj + 4) = newBase + ((*(int64_t *)(obj + 4) - oldBase) >> 1) * 2;
    uVar3 = 0;
    if (*obj != 0) {
        lVar2 = 0;
        do {
            lVar1 = *(int64_t *)(obj + 8);
            if ((*(int64_t *)(lVar2 + lVar1) != 0) && (*(int64_t *)(lVar2 + 8 + lVar1) != 0)) {
                *(int64_t *)(lVar2 + lVar1) =
                    newBase + ((*(int64_t *)(lVar2 + lVar1) - oldBase) >> 1) * 2;
                *(int64_t *)(lVar2 + 8 + *(int64_t *)(obj + 8)) =
                    newBase +
                    ((*(int64_t *)(lVar2 + 8 + *(int64_t *)(obj + 8)) - oldBase) >> 1) * 2;
            }
            uVar3++;
            lVar2 += 0x10;
        } while (uVar3 < *obj);
    }
}

/* ========== FUN_1400549BC @0x1400549bc ==========
 * 初始化窗口拖放支持并设置相关状态。
 */
void FUN_1400549BC(int64_t obj, uint32_t flags, int64_t fontObj)
{
    LRESULT LVar1;

    if (fontObj != 0) {
        LVar1 = FUN_1400E5890(fontObj);
        int64_t *pObj = *(int64_t **)(obj + 0x38);
        void (**fn)(int64_t *, LRESULT, int) =
            (void (**)(int64_t *, LRESULT, int))(*(int64_t *)((uint8_t *)pObj + 0x108));
        (*fn)(pObj, LVar1, 1);
    }
    FUN_140053C5C(obj, ~(flags >> 0x1b) & 1);
    PECMD_SetObjectVisibleVar(obj, flags & 0x10000000);
    DragAcceptFiles(*(HWND *)(*(int64_t *)(obj + 0x38) + OBJ_HWND), 1);
    if (**(LPCWSTR *)(obj + 0x10) != L'\0') {
        FUN_14007D0AC(*(int64_t **)(obj + 0x50), *(LPCWSTR *)(obj + 0x10),
                      *(LPCWSTR *)(obj + 0x20));
    }
}

/* ========== PECMD_CreateWindowObjectEx @0x140055368 ==========
 * 构造窗口对象并填充扩展字段。
 */
uint64_t *PECMD_CreateWindowObjectEx(uint64_t *obj, uint64_t data1, uint64_t data2, uint64_t *p4,
                                     uint64_t data5, uint64_t data6, uint32_t flags1,
                                     uint32_t flags2, uint64_t data9)
{
    (void)data2;
    PECMD_ItemBaseInit(obj, 0x14, data1, (LPCWSTR)*p4, g_szEmpty, g_szEmpty, 0, 0, 0, 0);
    obj[0xe] = data5;
    *obj = (uint64_t)(uintptr_t)PTR_FUN_1401235a8;
    obj[0xf] = data6;
    *(uint32_t *)(obj + 0x10) = flags1;
    *(uint32_t *)((uint8_t *)obj + 0x84) = flags2;
    obj[0x11] = data9;
    return obj;
}

/* ========== FUN_14005B3E4 @0x14005b3e4 ==========
 * 带主题/后备路径的文本绘制。
 */
int64_t FUN_14005B3E4(int64_t obj, uint32_t msg, HDC hdc, uint64_t *lParam)
{
    uint16_t uVar1;
    int64_t lVar2;
    int64_t local_res8 = 0;

    uVar1 = FUN_1400F172C(*(int64_t **)(obj + 0x38), msg, (uint64_t)(uintptr_t)hdc, lParam,
                          *(int64_t *)(obj + 8), 1, &local_res8);
    if ((uVar1 & 4) == 0) {
        lVar2 = FUN_1400E5B0C(obj, (uint64_t)msg, hdc, (int64_t *)lParam);
        FUN_1400F172C(*(int64_t **)(obj + 0x38), msg, (uint64_t)(uintptr_t)hdc, lParam,
                      *(int64_t *)(obj + 8), 2, NULL);
        local_res8 = lVar2;
    }
    return local_res8;
}

/* ========== FUN_14005B900 @0x14005b900 ==========
 * 添加/更新托盘图标。
 */
void FUN_14005B900(char *data, uint64_t id, LPCWSTR tip, HICON icon, uint32_t flags)
{
    BOOL BVar1;

    FUN_14005B7E8(data);
    if (*(int64_t *)(data + 0x30) == 0) {
        if (icon == (HICON)0) {
            icon = LoadIconW(g_hInst, (LPCWSTR)(uintptr_t)3);
            if (icon != (HICON)0) {
                *(HICON *)(data + 0x30) = icon;
            }
        }
        else {
            *(HICON *)(data + 0x30) = icon;
        }
        data[0x10] = -0x30;
        data[0x11] = '\x03';
        data[0x12] = '\0';
        data[0x13] = '\0';
        lstrcpynW((LPWSTR)(data + 0x38), tip, 0x80);
    }
    *(uint64_t *)(data + 0x18) = id;
    data[0x24] = '\a';
    data[0x25] = '\0';
    data[0x26] = '\0';
    data[0x27] = '\0';
    *(uint32_t *)(data + 0x28) = flags;
    BVar1 = Shell_NotifyIconW(0, (PNOTIFYICONDATAW)(data + 0x10));
    *data = (char)BVar1;
}

/* ========== PECMD_GetTooltipWindow @0x14005b9c8 ==========
 * 获取/创建全局 Tooltips 窗口。
 */
HWND PECMD_GetTooltipWindow(uint32_t flags, int maxWidth)
{
    if (g_hwndD310 == (HWND)0) {
        g_hwndD310 = CreateWindowExW(0, WSTR("Tooltips_Class32"), (LPCWSTR)0,
                                     (DWORD)(((flags & 3) << 6) | 0x80000003u), (int)0x80000000,
                                     (int)0x80000000, (int)0x80000000, (int)0x80000000, (HWND)0,
                                     (HMENU)0, (HINSTANCE)0, (LPVOID)0);
        if (g_hwndD310 != (HWND)0) {
            SendMessageW(g_hwndD310, 0x418, 0, (LPARAM)maxWidth);
            g_msgWndMode = (uint8_t)(flags & 1);
        }
    }
    return g_hwndD310;
}

/* ========== FUN_14005DAF8 @0x14005daf8 ==========
 * 按对象中的 0x60 基数缩放四个整数。
 */
void FUN_14005DAF8(int64_t obj, int *x, int *y, int *w, int *h)
{
    if (0 < *(int *)(obj + 0x17c)) {
        *x = (*x * *(int *)(obj + 0x17c)) / 0x60;
        *y = (*y * *(int *)(obj + 0x17c)) / 0x60;
        *w = (*w * *(int *)(obj + 0x17c)) / 0x60;
        *h = (*h * *(int *)(obj + 0x17c)) / 0x60;
    }
}

/* ========== PECMD_ReleaseSlotRef @0x14005e8e8 ==========
 * 从全局缓存表中释放对象（引用计数减到 0 时删除）。
 */
void PECMD_ReleaseSlotRef(int64_t *slot, int table)
{
    int64_t lVar1;
    int64_t lVar2;
    int64_t *plVar3;
    int64_t lVar4;
    int64_t local_res18[2];

    EnterCriticalSection(&g_csInit);
    plVar3 = &g_cacheTableB[0];
    if (table != 0) {
        plVar3 = &g_cacheTableA[0];
    }
    lVar1 = *plVar3;
    if (lVar1 != 0) {
        lVar2 = *slot;
        lVar4 = ((int64_t)slot - lVar1) >> 3;
        if ((lVar4 <= plVar3[1]) && (lVar2 != 0)) {
            *(int64_t *)(lVar2 + 8) = *(int64_t *)(lVar2 + 8) - 1;
            if (*(int64_t *)(lVar2 + 8) < 1) {
                *(int64_t *)(lVar1 + lVar4 * 8) = 0;
                plVar3[1] = plVar3[1] - 1;
                local_res18[0] = lVar2;
                PECMD_FreeStrBuf((WCHAR **)local_res18);
            }
        }
    }
    LeaveCriticalSection(&g_csInit);
}

/* ========== PECMD_RegCreateKeyRetryWow64 @0x14005f750 ==========
 * 创建注册表键，必要时以易失重试。
 */
LONG PECMD_RegCreateKeyRetryWow64(HKEY root, LPCWSTR sub, int isVolatile)
{
    LONG LVar1;
    HKEY local_res20;

    while (1) {
        local_res20 = (HKEY)0;
        LVar1 = RegCreateKeyExW(root, sub, 0, (LPWSTR)0, (DWORD)(0 < isVolatile), 0x2001b,
                                (LPSECURITY_ATTRIBUTES)0, &local_res20, NULL);
        if (local_res20 != (HKEY)0) {
            RegCloseKey(local_res20);
        }
        if ((LVar1 != 0x3fd) || (isVolatile != 0))
            break;
        isVolatile = 1;
    }
    return LVar1;
}

/* ========== PECMD_PickFreeDriveLetter @0x14005f868 ==========
 * 在盘符位图中查找未占用的字母。
 */
int PECMD_PickFreeDriveLetter(uint32_t *bitmap, int16_t start, char mode, char *exclude)
{
    uint64_t in_RAX = 0;
    int16_t sVar1;
    uint32_t uVar2;
    uint64_t uVar3 = 0;

    if (mode < '\0') {
        in_RAX = 0;
        if (start == 0xc) {
            uVar3 = 2;
        }
    }
    else {
        uVar3 = (uint64_t)(uint32_t)(int)mode;
    }
    if (exclude == NULL) {
        if ((int16_t)uVar3 < 0x1a) {
            uVar2 = *bitmap;
            do {
                in_RAX = uVar3 & 0xff;
                sVar1 = (int16_t)uVar3;
                if ((uVar2 >> ((uint32_t)in_RAX & 0x1f) & 1) == 0) {
                    *bitmap = (1u << ((uint8_t)sVar1 & 0x1f)) | uVar2;
                    return sVar1 + 0x41;
                }
                uVar3 = (uint64_t)(uint16_t)(sVar1 + 1U);
            } while ((int16_t)(sVar1 + 1U) < 0x1a);
        }
    }
    else {
        in_RAX = (uint64_t)(uint8_t)*exclude;
        if (*exclude != '\0') {
            uVar2 = *bitmap;
            do {
                sVar1 = (int16_t)(int8_t)in_RAX - 0x41;
                if ((uVar2 >> ((uint8_t)sVar1 & 0x1f) & 1) == 0) {
                    *bitmap = (1u << ((uint8_t)sVar1 & 0x1f)) | uVar2;
                    return sVar1 + 0x41;
                }
                exclude++;
                in_RAX = (uint64_t)(uint8_t)*exclude;
            } while (*exclude != '\0');
        }
    }
    return (int)(in_RAX >> 0x10) << 0x10;
}

/* ========== FUN_1400607A4 @0x1400607a4 ==========
 * 创建符号链接/硬链接，失败则复制文件。
 */
void FUN_1400607A4(LPCWSTR src, LPCWSTR dst, int len)
{
    int iVar1;

    if (2 < len) {
        iVar1 = StrCmpNIW(src, dst, len);
        if (iVar1 == 0)
            goto link_or_copy;
    }
    if (((*src == *dst) && (src[1] == dst[1])) && (dst[1] == L':')) {
        len = 2;
    }
    else {
        len = 0;
    }
link_or_copy:
    PECMD_CreateSymbolicLink((uint64_t)(uintptr_t)dst, (uint64_t)(uintptr_t)(src + len), 0);
    CreateHardLinkW(dst, src, (LPSECURITY_ATTRIBUTES)0);
    CopyFileW(src, dst, 1);
}

/* ========== PECMD_SetWindowProcHook @0x140060870 ==========
 * 安装窗口过程钩子（修改代码页权限并替换窗口过程）。
 */
void PECMD_SetWindowProcHook(uint64_t *obj, uint64_t value, LONG_PTR *outOld)
{
    LONG_PTR LVar1;
    DWORD local_res8[2];

    local_res8[0] = 0;
    VirtualProtect((LPVOID)(uintptr_t)*obj, 0x18, 0x40, local_res8);
    VirtualProtect(obj + 1, 0x18, 0x40, local_res8);
    memcpy((void *)(obj + 1), g_b24e10, 0x18);
    *(uint64_t *)((uint8_t *)obj + 0x15) = value;
    if ((HWND)(uintptr_t)obj[4] != (HWND)0) {
        LVar1 = SetWindowLongPtrW((HWND)(uintptr_t)obj[4], -4, (LONG_PTR)(obj + 1));
        if (outOld != NULL) {
            *outOld = LVar1;
        }
    }
}

/* ========== FUN_140060A94 @0x140060a94 ==========
 * 分配控制台并设置标志。
 */
void FUN_140060A94(uint64_t flags)
{
    HANDLE hConsoleOutput;
    HWND hWnd;

    if ((g_flagD6F6 & 1) == 0) {
        if (!AllocConsole()) {
            return;
        }
        hConsoleOutput = GetStdHandle((DWORD)0xfffffff5);
        COORD size = {0x800, 0x2710};
        SetConsoleScreenBufferSize(hConsoleOutput, size);
    }
    if (((flags & 0x100) != 0) && (hWnd = GetConsoleWindow(), hWnd != (HWND)0)) {
        SetWindowPos(hWnd, (HWND)0, 0, 0, 0, 0, 1);
    }
    g_flagD6F6 = (uint8_t)(g_flagD6F6 | (uint8_t)flags | 1);
}

/* ========== FUN_140062FC4 @0x140062fc4 ==========
 * 匹配关键字前缀，命中后跳过空白。
 */
uint8_t FUN_140062FC4(LPCWSTR key, int64_t *pp, int len)
{
    WCHAR WVar1;
    LPCWSTR lpStr2;
    int iVar2;
    uint8_t uVar3 = 0;

    if (len < 0) {
        len = lstrlenW(key);
    }
    lpStr2 = (LPCWSTR)(uintptr_t)*pp;
    iVar2 = StrCmpNIW(key, lpStr2, len);
    if (iVar2 == 0) {
        WVar1 = lpStr2[len];
        if ((((8 < (uint16_t)WVar1) && ((uint16_t)WVar1 < 0xe)) || (WVar1 == L' ')) ||
            (WVar1 == L'\0')) {
            *pp = *pp + (int64_t)len * 2;
            uVar3 = 1;
            FUN_14005B154((WCHAR **)pp);
        }
    }
    return uVar3;
}

/* ========== FUN_140063A6C @0x140063a6c ==========
 * 分配带头缓冲，内存不足时重试。
 */
void FUN_140063A6C(uint64_t *out, int64_t *count, uint64_t *flags, uint32_t esize)
{
    int iVar1;
    int64_t *plVar2;
    int64_t lVar3;

    *flags = 0;
    *count = 1;
    *out = 0;
    lVar3 = *count * (uint64_t)esize;
    do {
        plVar2 = (int64_t *)HeapAlloc(g_hHeap, 0, (size_t)lVar3 + 8);
        if (plVar2 != NULL)
            break;
        iVar1 = FUN_1400630D0(2);
    } while (iVar1 == 4);
    *(uint32_t *)((uint8_t *)plVar2 + 4) = 0xaa55;
    *plVar2 = lVar3;
    *out = (uint64_t)(uintptr_t)(plVar2 + 1);
    memset(plVar2 + 1, 0, (size_t)esize);
}

/* ========== FUN_1400641D4 @0x1400641d4 ==========
 * 判断宽字符串是否为 "visible"/"visable"。
 */
uint64_t FUN_1400641D4(uint16_t *s)
{
    uint16_t uVar1;
    uint8_t uVar4 = 0;

    if ((((*s | 0x20) == 0x76) && ((s[1] | 0x20) == 0x69)) && ((s[2] | 0x20) == 0x73)) {
        uVar1 = s[3] | 0x20;
        if ((((uVar1 == 0x69) || (uVar1 == 0x61)) && ((s[4] | 0x20) == 0x62) &&
             ((s[5] | 0x20) == 0x6c) && ((s[6] | 0x20) == 0x65)) &&
            (s[7] == 0)) {
            uVar4 = 1;
        }
    }
    return (uint64_t)uVar4;
}

/* ========== FUN_14006643C @0x14006643c ==========
 * 检查设备类 GUID 对应的 Class 名。
 */
uint32_t FUN_14006643C(const GUID *guid, LPCSTR className)
{
    LONG LVar1;
    int iVar2;
    HKEY hKey;
    DWORD type = 1;
    DWORD size = 0x32;
    BYTE data[64];

    hKey = SetupDiOpenClassRegKey(guid, 0x20019);
    if (hKey != (HKEY)0) {
        LVar1 = RegQueryValueExA(hKey, "Class", NULL, &type, data, &size);
        RegCloseKey(hKey);
        if ((LVar1 == 0) && (iVar2 = lstrcmpiA((LPCSTR)data, className), iVar2 == 0)) {
            return 1;
        }
    }
    return 0;
}

/* ========== PECMD_CopyTokenTrimmed @0x14006764c ==========
 * 复制到指定分隔符/空白为止的 token。
 */
void PECMD_CopyTokenTrimmed(int64_t *pp, int64_t *out, int16_t sep1, int16_t sep2)
{
    WCHAR WVar1;
    LPCWSTR pWVar2;
    int16_t *psVar3;
    int iVar4;
    LPCWSTR pWVar5;

    FUN_14005B154((WCHAR **)pp);
    pWVar2 = (LPCWSTR)(uintptr_t)*pp;
    WVar1 = *pWVar2;
    while ((WVar1 != L'\0' && (psVar3 = (int16_t *)(uintptr_t)*pp, *psVar3 != sep1)) &&
           (*psVar3 != sep2)) {
        *pp = (int64_t)(uintptr_t)(psVar3 + 1);
        WVar1 = psVar3[1];
    }
    pWVar5 = (LPCWSTR)(uintptr_t)*pp;
    do {
        pWVar5--;
        if (pWVar5 < pWVar2)
            break;
    } while (((8 < (uint16_t)*pWVar5) && ((uint16_t)*pWVar5 < 0xe)) || (*pWVar5 == L' '));
    iVar4 = (int)(((int64_t)(uintptr_t)pWVar5 + (2 - (int64_t)(uintptr_t)pWVar2)) >> 1);
    PECMD_StrCopyW((WCHAR **)out, pWVar2, (int64_t)(iVar4 + 1));
    *(uint16_t *)((uint8_t *)(uintptr_t)*out + (int64_t)iVar4 * 2) = 0;
}

/* ========== PECMD_EscapeLabelAmpersands @0x140067f90 ==========
 * 将 & 转义为 &&（用于菜单/标题显示）。
 */
void PECMD_EscapeLabelAmpersands(int64_t *ps)
{
    int iVar1;
    int iVar2;
    int iVar3;
    uint64_t uVar4;
    int iVar5;
    uint64_t uVar6;
    int iVar7;

    iVar2 = lstrlenW((LPCWSTR)(uintptr_t)*ps);
    uVar4 = 0;
    if (0 < iVar2) {
        iVar7 = iVar2 + 3;
        uVar6 = uVar4;
        iVar1 = 0;
        do {
            iVar5 = iVar1 + 1;
            iVar3 = (int)uVar4;
            if (*(int16_t *)((uint8_t *)(uintptr_t)*ps + uVar6) == 0x26) {
                PECMD_AllocString((WCHAR **)ps, (int64_t)iVar7);
                iVar2++;
                iVar7++;
                memmove((void *)((uint8_t *)(uintptr_t)*ps + (int64_t)iVar5 * 2),
                        (void *)((uint8_t *)(uintptr_t)*ps + (int64_t)iVar3 * 2),
                        ((iVar2 - iVar3) + 1) * 2);
                iVar3++;
                iVar5 = iVar1 + 2;
                uVar6 += 2;
            }
            uVar4 = (uint64_t)(iVar3 + 1U);
            uVar6 += 2;
            iVar1 = iVar5;
        } while ((int)(iVar3 + 1U) < iVar2);
    }
}

/* ========== PECMD_BubbleSortStringArray @0x140068c04 ==========
 * 对字符串指针数组做冒泡排序。
 */
void PECMD_BubbleSortStringArray(uint64_t *arr, int count, int dir)
{
    uint64_t uVar1;
    int iVar2;
    uint64_t *puVar3;
    int64_t lVar4;
    int64_t lVar5;
    int64_t lVar6;

    if (0 < count) {
        lVar4 = 1;
        lVar6 = (int64_t)count;
        lVar5 = lVar4;
        puVar3 = arr;
        do {
            for (; lVar4 < count; lVar4++) {
                iVar2 =
                    lstrcmpiW(*(LPCWSTR *)(uintptr_t)*puVar3, *(LPCWSTR *)(uintptr_t)arr[lVar4]);
                if (0 < iVar2 * dir) {
                    uVar1 = *puVar3;
                    *puVar3 = arr[lVar4];
                    arr[lVar4] = uVar1;
                }
            }
            lVar4 = lVar5 + 1;
            puVar3++;
            lVar6--;
            lVar5 = lVar4;
        } while (lVar6 != 0);
    }
}

/* ========== FUN_14006C4C8 @0x14006c4c8 ==========
 * 获取下拉框指定项的文本。
 */
uint64_t FUN_14006C4C8(int64_t obj, int index, LPARAM *out)
{
    int iVar1;
    uint64_t uVar2;
    int iVar3;

    uVar2 = (uint64_t)SendMessageW(*(HWND *)(obj + OBJ_HWND), 0x149, (WPARAM)index, 0);
    iVar1 = (int)uVar2;
    iVar3 = iVar1;
    if (iVar1 == -1) {
        iVar3 = 0;
    }
    PECMD_AllocString((WCHAR **)out, (int64_t)(iVar3 + 5));
    if (iVar1 == -1) {
        uVar2 = 0xffffffff;
        *(uint16_t *)(uintptr_t)*out = 0;
    }
    else {
        SendMessageW(*(HWND *)(obj + OBJ_HWND), 0x148, (WPARAM)index, (LPARAM)(uintptr_t)*out);
        uVar2 = uVar2 & 0xffffffff;
    }
    return uVar2;
}

/* ========== FUN_14006CAF0 @0x14006caf0 ==========
 * 检查设备是否就绪（IOCTL_STORAGE_CHECK_VERIFY）。
 */
BOOL FUN_14006CAF0(LPCWSTR path)
{
    BOOL BVar1;
    DWORD bytesReturned = 0;
    HANDLE hDevice = 0;

    PECMD_OpenFileHandle(&hDevice, path, 0xc0000000, 3, (LPSECURITY_ATTRIBUTES)0, 4, 0, (HANDLE)0);
    BVar1 = DeviceIoControl(hDevice, 0x900c4, NULL, 0, NULL, 0, &bytesReturned, NULL);
    if ((hDevice != (HANDLE)0) && (hDevice != (HANDLE)0xffffffffffffffffULL)) {
        CloseHandle(hDevice);
    }
    return BVar1;
}

/* ========== FUN_14006E4F4 @0x14006e4f4 ==========
 * 在 COM 临界区内查找并释放对象。
 */
bool FUN_14006E4F4(LPCWSTR name)
{
    int iVar1;
    int64_t *plVar2;

    EnterCriticalSection(&g_csCom);
    PECMD_LoadOle32Apis();
    iVar1 = -3;
    if (g_pOleInit != NULL) {
        int (*initFn)(int) = (int (*)(int))(void *)g_pOleInit;
        iVar1 = initFn(0);
        if (iVar1 == 1) {
            if (g_pOleUninit != NULL) {
                ((void (*)(void))g_pOleUninit)();
            }
        }
    }
    plVar2 = PECMD_LoadImageFromFile(name);
    if (plVar2 != NULL) {
        void (**fn)(int64_t *) = (void (**)(int64_t *))(*(int64_t *)((uint8_t *)*plVar2 + 0x10));
        (*fn)(plVar2);
    }
    if ((g_pOleUninit != NULL) && (iVar1 == 0)) {
        ((void (*)(void))g_pOleUninit)();
    }
    LeaveCriticalSection(&g_csCom);
    return plVar2 != NULL;
}

/* ========== FUN_14007443C @0x14007443c ==========
 * 解析路径前缀，返回冒号后的剩余部分。
 */
uint16_t *FUN_14007443C(LPCWSTR text, int64_t *out)
{
    uint16_t *puVar1;
    uint16_t *puVar2;
    int16_t *psVar3;
    uint16_t uVar4;
    LPCWSTR local_res8[4];

    local_res8[0] = text;
    FUN_14005B154((WCHAR **)local_res8);
    FUN_14007034C((WCHAR **)out, local_res8[0]);
    puVar1 = (uint16_t *)(uintptr_t)*out;
    uVar4 = *puVar1;
    puVar2 = puVar1;
    while ((uVar4 != 0 && (((uVar4 < 9) || (0xd < uVar4)) && (uVar4 != 0x20)))) {
        puVar2++;
        uVar4 = *puVar2;
    }
    if (*puVar2 != 0) {
        *puVar2 = 0;
    }
    psVar3 = (int16_t *)(uintptr_t)*out;
    do {
        if (*psVar3 == 0) {
            return puVar1;
        }
        if (*psVar3 == 0x3a) {
            if (psVar3[1] != 0x3a) {
                *psVar3 = 0;
                return (uint16_t *)(psVar3 + 1);
            }
            psVar3++;
        }
        psVar3++;
    } while (1);
}

/* ========== PECMD_ParseNumOrVar @0x1400746b0 ==========
 * 解析数字，或以冒号为界解析变量 token。
 */
uint64_t PECMD_ParseNumOrVar(int64_t *pp, uint64_t *out, int64_t *script)
{
    int16_t sVar1;
    uint64_t uVar2;
    LPCWSTR pWVar3;

    pWVar3 = (LPCWSTR)(uintptr_t)*pp;
    if ((((uint16_t)*pWVar3 < 0x30) || (0x39 < (uint16_t)*pWVar3)) && (*pWVar3 != L'(')) {
        FUN_1400F429C((WCHAR **)pp, 0x3a);
        sVar1 = *(int16_t *)(uintptr_t)*pp;
        if (sVar1 != 0) {
            *(int16_t *)(uintptr_t)*pp = 0;
        }
        uVar2 = (uint64_t)(uintptr_t)FUN_14001E69C(script, pWVar3, NULL, -1);
        if (sVar1 != 0) {
            *(int16_t *)(uintptr_t)*pp = sVar1;
        }
        if (uVar2 != 0) {
            *out = uVar2;
        }
        uVar2 = (uint64_t)(uVar2 != 0);
    }
    else {
        uVar2 = PECMD_EvalParenStripped(pp, out);
    }
    return uVar2;
}

/* ========== FUN_1400799F0 @0x1400799f0 ==========
 * 使用全局转换回调转换字符串，失败时按备用代码页重试。
 */
uint64_t FUN_1400799F0(uint64_t src, uint64_t len)
{
    int iVar1;
    uint64_t uVar2;
    int iVar3;
    int64_t local_res10 = 0;

    PECMD_AllocWStringBuffer((WCHAR **)&local_res10, (int64_t)(len * 2 + 0x20));
    iVar3 = (int)len + 1;
    iVar1 = g_pConvFunc(0, 8, (LPCSTR)(uintptr_t)src, (DWORD)(len & 0xffffffff),
                        (LPWSTR)(uintptr_t)local_res10, iVar3);
    if (iVar1 < 1) {
        uVar2 = 0xfde9;
        iVar1 = g_pConvFunc(0xfde9, 8, (LPCSTR)(uintptr_t)src, (DWORD)(len & 0xffffffff),
                            (LPWSTR)(uintptr_t)local_res10, iVar3);
        if (iVar1 != 0)
            goto converted;
    }
    uVar2 = 0;
converted:
    PECMD_FreeStrBuf((WCHAR **)&local_res10);
    return uVar2;
}

/* ========== FUN_14007C7EC @0x14007c7ec ==========
 * 显示/检查盘符类型并输出相应消息。
 */
void FUN_14007C7EC(WCHAR drive, int mode)
{
    uint8_t bVar1;
    UINT UVar2;
    WCHAR local_res18[8];
    WCHAR local_18[4];

    bVar1 = PECMD_EjectDrive(drive, mode);
    if (mode < 0) {
        local_18[1] = 0x2a;
        local_18[0] = L' ';
        local_18[2] = 0x20;
        local_res18[1] = 0x3a;
        local_res18[2] = 0x5c;
        local_res18[3] = 0;
        local_res18[0] = drive;
        local_18[3] = drive;
        UVar2 = GetDriveTypeW(local_res18);
        if ((UVar2 == 1) || (bVar1 != 2)) {
            PECMD_DefineDosDevice((uint8_t *)g_Script, local_18);
        }
    }
}

/* ========== FUN_1400A43C4 @0x1400a43c4 ==========
 * 创建锁对象包装。
 */
uint64_t *FUN_1400A43C4(LPCWSTR name, char wait)
{
    uint64_t *puVar1;
    uint64_t *puVar2;
    int64_t local_res18[2] = {0, 0};

    FUN_1400702B0((WCHAR **)local_res18, name);
    puVar1 = (uint64_t *)calloc(1, 0x20);
    if (puVar1 != NULL) {
        puVar1[1] = 0;
        *(uint32_t *)((uint8_t *)puVar1 + 0x14) = 0;
        *(uint8_t *)((uint8_t *)puVar1 + 0x10) = 0;
        *puVar1 = (uint64_t)(uintptr_t)PTR_FUN_140128ed0;
        puVar2 = FUN_1400A41FC(name);
        puVar1[3] = (uint64_t)(uintptr_t)puVar2;
        puVar1[1] = puVar2 != NULL ? *puVar2 : 0;
    }
    if ((puVar1 != NULL) && (wait != '\0')) {
        WaitForSingleObject((HANDLE)(uintptr_t)puVar1[1], 0xffffffff);
        *(uint8_t *)((uint8_t *)puVar1 + 0x10) = 1;
    }
    PECMD_FreeStrBuf((WCHAR **)local_res18);
    return puVar1;
}

/* ========== FUN_1400A4460 @0x1400a4460 ==========
 * 创建命名锁（支持 #/# 本地/全局前缀）。
 */
uint64_t *FUN_1400A4460(LPCWSTR name, char wait, uint32_t *out)
{
    WCHAR WVar1;
    uint64_t *puVar2;
    const char *pcVar3;
    LPCWSTR pWVar4;
    LPCWSTR local_res8 = NULL;

    if (*name == L'#') {
        pWVar4 = name + 1;
        WVar1 = *pWVar4;
        if (WVar1 == L'#') {
            pWVar4 = name + 2;
        }
        pcVar3 = "Local\\pecmd2012.lock.";
        if (WVar1 == L'#') {
            pcVar3 = "Global\\pecmd2012.lock.";
        }
        PECMD_StrBldCopyAnsi((int64_t *)&local_res8, pcVar3, (uint64_t)-1);
        FUN_14006375C((WCHAR **)&local_res8, pWVar4);
        puVar2 = PECMD_CreateGlobalMutex(local_res8, wait, out);
        PECMD_FreeStrBuf((WCHAR **)&local_res8);
    }
    else {
        puVar2 = FUN_1400A43C4(name, wait);
    }
    return puVar2;
}

/* ========== FUN_1400A9650 @0x1400a9650 ==========
 * 发送标题消息并同步标题变量。
 */
void FUN_1400A9650(int64_t obj, HWND hwnd, int id, LPCWSTR text, uint8_t unused)
{
    int64_t local_98;
    WCHAR local_78[56];

    (void)unused;
    local_98 = (int64_t)id;
    SendMessageW(hwnd, 0x465, (WPARAM)&local_98, 0);
    wsprintfW(local_78, WSTR(".Title%ld"), (int32_t)id);
    if (**(int16_t **)(obj + 0x10) != 0) {
        FUN_14007D0AC(*(int64_t **)(obj + 0x50), local_78, text);
    }
}
