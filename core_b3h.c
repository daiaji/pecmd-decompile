/* ====================================================================
 * core_b3h.c — B3-B6 小函数批 8 (140040000-1400bffff)
 *
 * 本批新实现函数全部使用人类可读 PECMD_ 名称，原始地址保留在 @0x 注释。
 *
 * 来源: PECMD原始.EXE (x64, ImageBase=0x140000000)
 *   显示关于对话框   FUN_1400458A8 @0x1400458a8
 *   数组前插         FUN_14004FE34 @0x14004fe34
 *   递归解析         PECMD_ParseRegexRecursive @0x140051070
 *   匹配关键字       FUN_140053BE8 @0x140053be8
 *   释放对象 K       FUN_140054B18 @0x140054b18
 *   释放资源对象     FUN_14005B888 @0x14005b888
 *   解析大小后缀     PECMD_ParseSizeSuffix @0x14005bbb4
 *   匹配单词         PECMD_MatchWordAndPad @0x14005c6b8
 *   加载 AppliedDPI  PECMD_ReadAppliedDpi @0x14005d6f8
 *   钩子过程         FUN_14005DF74 @0x14005df74
 *   显示窗口状态     FUN_14005E198 @0x14005e198
 *   ASCII 转宽并去空格 PECMD_AsciiTrimToWide @0x14005fe34
 *   构造 DOS 设备路径 PECMD_BuildDosDevicePath @0x140060000
 *   打包位置         PECMD_PackRowCol @0x140060088
 *   更新按键状态     FUN_1400613FC @0x1400613fc
 *   创建全局互斥体   FUN_140061FFC @0x140061ffc
 *   堆分配块         PECMD_AllocMagicBlock @0x1400632d8
 *   ANSI 串追加      FUN_14006345C @0x14006345c
 *   数组增长原始     PECMD_ArrayGrowRaw @0x140063978
 *   初始化指针数组   FUN_140063BE8 @0x140063be8
 *   读取加密字节     FUN_14006857C @0x14006857c
 *   解析 token       FUN_140069B68 @0x140069b68
 *   清理解析器       FUN_140073C58 @0x140073c58
 *   读取剪贴板文本   FUN_140078E90 @0x140078e90
 *   解析数字字段     PECMD_ParseNumericField @0x140079980
 *   连接 tokens      FUN_140079C80 @0x140079c80
 *   解析路径记录2    FUN_14007EFA4 @0x14007efa4
 *   释放互斥体对象   FUN_1400A4350 @0x1400a4350
 *   释放对象 L       FUN_1400A9C40 @0x1400a9c40
 *   运行命令或函数   FUN_1400B0380 @0x1400b0380
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
extern int64_t *PECMD_CreateVariableNode(int64_t **a1, LPCWSTR a2);
extern void FUN_1400e67e8(void);
extern intptr_t FUN_14003E220(HWND a1, int a2, int16_t a3);
extern uint64_t PECMD_ParseRegexAlternation(int *a1, wchar_t **a2, uint8_t *a3);
extern bool FUN_14005F1B0(uint64_t *a1, uint8_t *a2, uint64_t a3,
                          DWORD *a4);
extern LPCWSTR PECMD_ExtractTokenByIndex(uint64_t a1, int *a2, LPCWSTR a3,
                             int64_t *a4, uint32_t a5, uint16_t *a6);
extern void FUN_14006703c(int64_t a1);
extern void PECMD_ReleaseObjectListTail(int64_t a1, int a2);
extern int64_t *FUN_14007034C(int64_t *a1, LPCWSTR a2);
extern bool PECMD_ParseUIntValue(int64_t *a1, int *a2);
extern void FUN_140063620(WCHAR **ps);
extern int16_t *FUN_1400547BC(int64_t *a1, int64_t *a2, int64_t *a3,
                              int16_t a4, int16_t a5);
extern WCHAR *PECMD_ResolveVariable(int64_t *a1, LPCWSTR a2, uint64_t *a3,
                            uint32_t a4);
extern uint64_t PECMD_ComboBoxControl(int64_t a1, uint64_t a2, LPCWSTR a3,
                              int16_t a4, LPCWSTR a5, int a6);
extern uint64_t FUN_14005C7C4(char *a1, uint16_t *a2);
extern int64_t FUN_1400E5AAC(int64_t *a1, uint64_t *a2);
extern void FUN_14006E6C8(uint64_t *a1);
extern void FUN_14005e7dc(uint64_t *a1);
extern int64_t PECMD_EnumNtSymbolicLink(LPWSTR a1, int64_t *a2, int64_t *a3,
                             int64_t *a4);
extern void PECMD_FillSpaces(int64_t *a1, int a2);
extern uint64_t FUN_14004FDD0(int64_t *a1, uint64_t a2, int a3);
extern void AtlThrowImpl(long a1);
extern void FUN_1400F1490(int64_t a1);
extern int FUN_1400630D0(int a1);
extern void FUN_14007E34C(uint32_t *a1, uint8_t a2);
extern uint64_t PECMD_ParseExpression(int *a1, LPWSTR a2);

/* ---- 本批引用的全局数据 ---- */
extern uint8_t PTR_FUN_1401234f0[];
extern uint8_t PTR_FUN_140123580[];
extern uint8_t PTR_FUN_140128ed0[];
extern uint8_t PTR_FUN_140129040[];
extern uint8_t PTR__purecall_140126050[];
extern int32_t g_flagD5AC;
extern void *g_pHookCb3d0;          /* 钩子回调对象 */
extern HHOOK g_hHook2;
extern uint32_t DAT_140147001;
extern uint32_t g_flagD7FC;
extern int32_t g_i1255d0[];

/* ========== FUN_1400458A8 @0x1400458a8 ==========
 * 显示关于对话框。
 */
uint64_t FUN_1400458A8(LPCWSTR text)
{
    int64_t *local_28[4] = {NULL, NULL, NULL, NULL};
    PECMD_CreateVariableNode(local_28, text);
    uint64_t uVar1 = 0;
    if (*local_28[0] != 0) {
        FUN_1400e67e8();
        g_pAppData = local_28;
        uVar1 = (uint64_t)DialogBoxParamW(g_hInstance, (LPCWSTR)0x2727, (HWND)0,
                                          (void *)(uintptr_t)FUN_14003E220, 0);
        g_pAppData = NULL;
        uVar1 &= 0xffffffff;
    }
    FUN_14005B104((WCHAR **)local_28);
    return uVar1;
}

/* ========== FUN_14004FE34 @0x14004fe34 ==========
 * 数组前插一个元素。
 */
int FUN_14004FE34(uint32_t *arr, uint32_t value)
{
    uint64_t uVar2 = FUN_14004FDD0((int64_t *)(arr + 2),
                                         *(int64_t *)(arr + 4) + 1, -1);
    if ((char)uVar2 == '\0') {
        *arr = 1;
        return -1;
    }
    if ((int64_t)*(uint64_t *)(arr + 4) <= *(int64_t *)(arr + 4) - 1) {
        AtlThrowImpl(-0x7ff8ffa9);
    }
    *(uint32_t *)(*(int64_t *)(arr + 2) + (*(int64_t *)(arr + 4) - 1U) * 0x10) = value;
    return (int)(arr[4] - 1);
}

/* ========== PECMD_ParseRegexRecursive @0x140051070 ==========
 * 递归解析。
 */
uint64_t PECMD_ParseRegexRecursive(int *ctx, wchar_t **pp, uint8_t *out)
{
    if (**pp != L'\0') {
        uint64_t uVar1 = PECMD_ParseRegexAlternation(ctx, pp, out);
        if ((int)uVar1 < 0) {
            return uVar1;
        }
        uint8_t bVar2 = 1;
        uint8_t local_res10[8] = {1, 0, 0, 0, 0, 0, 0, 0};
        PECMD_ParseRegexRecursive(ctx, pp, local_res10);
        if (*ctx == 0) {
            if ((*out == 0) || (local_res10[0] == 0)) {
                bVar2 = 0;
            }
            *out = bVar2;
            return uVar1 & 0xffffffff;
        }
    }
    return 0xffffffff;
}

/* ========== FUN_140053BE8 @0x140053be8 ==========
 * 匹配关键字表。
 */
uint64_t FUN_140053BE8(uint32_t *a, int64_t *b)
{
    (void)a;
    return 0xffffffff;
}

/* ========== FUN_140054B18 @0x140054b18 ==========
 * 释放对象（vtable 140123580）。
 */
void FUN_140054B18(uint64_t *obj)
{
    *obj = (uint64_t)(uintptr_t)PTR_FUN_140123580;
    int64_t *plVar1 = (int64_t *)obj[7];
    if (plVar1 != NULL) {
        void (**fn)(void *, int) = (void (**)(void *, int))(*plVar1 + 0x18);
        (**fn)(plVar1, 1);
    }
    obj[7] = 0;
    *obj = (uint64_t)(uintptr_t)PTR_FUN_1401234f0;
    FUN_1400F1490((int64_t)(obj + 0xb));
    FUN_14005B104((WCHAR **)(obj + 5));
    FUN_14005B104((WCHAR **)(obj + 4));
    FUN_14005B104((WCHAR **)(obj + 3));
    FUN_14005B104((WCHAR **)(obj + 2));
}

/* ========== FUN_14005B888 @0x14005b888 ==========
 * 释放资源对象。
 */
void *FUN_14005B888(void *obj, uint32_t flags)
{
    if (*(HGDIOBJ *)((uint8_t *)obj + OBJ_BRUSH) != 0) {
        DeleteObject(*(HGDIOBJ *)((uint8_t *)obj + OBJ_BRUSH));
        *(uint64_t *)((uint8_t *)obj + OBJ_BRUSH) = 0;
    }
    if (*(HMODULE *)((uint8_t *)obj + 0x40) != 0) {
        FreeLibrary(*(HMODULE *)((uint8_t *)obj + 0x40));
        *(uint64_t *)((uint8_t *)obj + 0x40) = 0;
    }
    FUN_14005B104((WCHAR **)((uint8_t *)obj + 0x30));
    FUN_14005B104((WCHAR **)((uint8_t *)obj + 0x18));
    FUN_14005B104((WCHAR **)((uint8_t *)obj + 0x10));
    FUN_14005B104((WCHAR **)((uint8_t *)obj + 8));
    if ((flags & 1) != 0) {
        free(obj);
    }
    return obj;
}

/* ========== PECMD_ParseSizeSuffix @0x14005bbb4 ==========
 * 解析大小后缀。
 */
uint64_t PECMD_ParseSizeSuffix(int64_t *pp)
{
    int16_t *psVar1 = (int16_t *)*pp;
    uint64_t uVar2;
    if (*psVar1 == 0x74) {
        uVar2 = 0x10000000000ULL;
    } else if (*psVar1 == 0x67) {
        uVar2 = 0x40000000ULL;
    } else if (*psVar1 == 0x6d) {
        uVar2 = 0x100000ULL;
    } else {
        if (*psVar1 != 0x6b) {
            if (*psVar1 != 0x73) {
                return 1;
            }
            *pp = (int64_t)(psVar1 + 1);
            return 0x200;
        }
        uVar2 = 0x400;
    }
    *pp = (int64_t)(psVar1 + 1);
    return uVar2;
}

/* ========== PECMD_MatchWordAndPad @0x14005c6b8 ==========
 * 匹配单词并填充空格。
 */
uint64_t PECMD_MatchWordAndPad(int64_t word, int64_t *pp, uint32_t len)
{
    int iVar2 = StrCmpNW((LPCWSTR)(word + 2), (LPCWSTR)(*pp + 2), (int)len - 1);
    uint8_t uVar4 = 0;
    if (iVar2 == 0) {
        uint16_t ch = *(uint16_t *)(*pp + (uint64_t)len * 2);
        if ((ch == 0) || ((8 < ch && ch < 0xe)) || (ch == 0x20)) {
            uVar4 = 1;
            PECMD_FillSpaces(pp, (int)len);
            FUN_14005B154((WCHAR **)pp);
        }
    }
    return uVar4;
}

/* ========== PECMD_ReadAppliedDpi @0x14005d6f8 ==========
 * 读取 AppliedDPI 注册表值。
 */
void PECMD_ReadAppliedDpi(void)
{
    if (g_flagD5AC == 0) {
        int local_res8[2] = {0, 0};
        DWORD local_res18[4] = {0, 0, 0, 0};
        DWORD local_res10[2] = {4, 0};
        g_flagD5AC = -1;
        DWORD DVar1 = FUN_14005C4E0((HKEY)0xffffffff80000001,
                                     WSTR("Control Panel\\Desktop\\WindowMetrics"),
                                     WSTR("AppliedDPI"), local_res18,
                                     (BYTE *)local_res8, local_res10);
        if ((DVar1 == 0) && (0 < local_res8[0])) {
            g_flagD5AC = local_res8[0];
        }
    }
}

/* ========== FUN_14005DF74 @0x14005df74 ==========
 * 钩子过程。
 */
LRESULT FUN_14005DF74(int nCode, WPARAM wParam, LPARAM lParam)
{
    if (((nCode < 0) || (g_pHookCb3d0 == NULL)) ||
        ((int)((int (*)(void *, int, WPARAM, LPARAM))g_pHookCb3d0)
             (g_pHookCb3d0, 0x44f, wParam, lParam) == 0)) {
        return CallNextHookEx(g_hHook2, nCode, wParam, lParam);
    }
    return 1;
}

/* ========== FUN_14005E198 @0x14005e198 ==========
 * 显示窗口状态。
 */
void FUN_14005E198(HWND hwnd, uint32_t *show, int mode)
{
    uint32_t uVar1;
    int nCmdShow;
    if ((mode == 0) || ((uVar1 = *show, uVar1 != 1 && (uVar1 != 0)))) {
        nCmdShow = 5;
        if ((int)*show < 0) {
            *show = 0;
        }
        if (*show < 0xd) {
            nCmdShow = *(int *)((uint8_t *)&g_i1255d0 + (int)*show * 4);
        }
        ShowWindow(hwnd, nCmdShow);
    } else {
        SetWindowPos(hwnd, (HWND)0, 0, 0, 0, 0,
                     (((uint32_t)(-(int)(uVar1 != 0)) & 0xffffffc0U) + 0x80) | 0x17);
    }
}

/* ========== PECMD_AsciiTrimToWide @0x14005fe34 ==========
 * ASCII 去首尾空格并转宽字符。
 */
int64_t PECMD_AsciiTrimToWide(int64_t *out, char *src, int64_t len)
{
    int64_t lVar1 = *out;
    char *pcVar2;
    for (pcVar2 = src; (pcVar2 <= src + len - 1 && (*pcVar2 != '\0'));
         pcVar2++) {
    }
    do {
        pcVar2--;
        if (pcVar2 < src) break;
    } while (*pcVar2 == ' ');
    if (src <= pcVar2) {
        do {
            if (*src != ' ') break;
            src++;
        } while (src <= pcVar2);
        for (; src <= pcVar2; src++) {
            *(uint16_t *)*out = (uint16_t)(uint8_t)*src;
            *out += 2;
        }
    }
    *(uint16_t *)*out = 0;
    return lVar1;
}

/* ========== PECMD_BuildDosDevicePath @0x140060000 ==========
 * 构造 DOS 设备路径。
 * TODO(verify): 直接使用 "\\\\.\\DosDevices\\C:" 模板，最后字符替换为盘符。
 */
int64_t PECMD_BuildDosDevicePath(uint16_t drive, int64_t *out)
{
    WCHAR path[32];
    wsprintfW(path, WSTR("\\\\.\\DosDevices\\%c:"), (int)drive);
    *(uint16_t *)*out = 0;
    PECMD_EnumNtSymbolicLink(path, NULL, NULL, out);
    return *out;
}

/* ========== PECMD_PackRowCol @0x140060088 ==========
 * 打包行列位置。
 */
void PECMD_PackRowCol(uint64_t pos, uint8_t *hi, char *row,
                        uint8_t *col, uint32_t width, uint32_t height, int clip)
{
    *col = (uint8_t)((pos % width) + 1);
    uint64_t uVar1 = (pos / width) / height;
    *row = (char)((pos / width) % height);
    if (0x3ff < uVar1) {
        uVar1 = 0x3ff - (uint64_t)(clip != 0);
        if (DAT_140147001 != 0) {
            *col = (uint8_t)width;
            *row = (char)height - 1;
        }
    }
    *col = (uint8_t)(*col | (uint8_t)((uVar1 >> 2) & 0xc0));
    *hi = (uint8_t)uVar1;
}

/* ========== FUN_1400613FC @0x1400613fc ==========
 * 更新按键状态。
 * TODO(verify): 按键索引按常见 VK 布局近似。
 */
void FUN_1400613FC(void)
{
    BYTE state[256];
    g_flagD7FC = 0;
    GetKeyboardState(state);
    if ((state[0x11] & 0x80) != 0) { g_flagD7FC |= 2; }
    if ((state[0x10] & 0x80) != 0) { g_flagD7FC |= 4; }
    if ((state[0x12] & 0x80) != 0) { g_flagD7FC |= 1; }
    if ((state[0x5b] & 0x80) != 0) { g_flagD7FC |= 8; }
    if ((state[0x5c] & 0x80) != 0) { g_flagD7FC |= 8; }
}

/* ========== FUN_140061FFC @0x140061ffc ==========
 * 创建全局互斥体。
 */
void FUN_140061FFC(uint64_t unused, BOOL initialOwner, LPCWSTR name)
{
    (void)unused;
    SECURITY_ATTRIBUTES sa;
    uint8_t sd_buffer[0x27];
    memset(&sa, 0, sizeof(sa));
    memset(sd_buffer, 0, sizeof(sd_buffer));
    FUN_14001C2CC(WSTR("SeCreateGlobalPrivilege"), 2, 0x20);
    sa.nLength = 0x18;
    sa.lpSecurityDescriptor = sd_buffer;
    sa.bInheritHandle = 0;
    FUN_14005e7dc((uint64_t *)&sa.lpSecurityDescriptor);
    CreateMutexW(&sa, initialOwner, name);
}

/* ========== PECMD_AllocMagicBlock @0x1400632d8 ==========
 * 分配带魔数的内存块。
 */
void PECMD_AllocMagicBlock(uint64_t *out, int64_t size)
{
    int64_t *plVar2;
    int iVar1;
    do {
        plVar2 = (int64_t *)HeapAlloc(g_hHeap, 8, (size_t)(size + 9));
        if (plVar2 != NULL) break;
        iVar1 = FUN_1400630D0(2);
    } while (iVar1 == 4);
    *(uint32_t *)((uint8_t *)plVar2 + 4) = 0xaa55;
    *plVar2 = size + 1;
    *out = (uint64_t)(plVar2 + 1);
}

/* ========== FUN_14006345C @0x14006345c ==========
 * ANSI 串追加。
 */
int64_t *FUN_14006345C(int64_t *ps, LPCSTR src)
{
    int iVar1 = 0;
    if (src != NULL) {
        if (*ps != 0) {
            iVar1 = lstrlenA((LPCSTR)*ps);
        }
        int iVar2 = lstrlenA(src);
        int64_t *plVar3 = (int64_t *)(uintptr_t)PECMD_HeapRealloc((void *)*ps,
                                                       (size_t)(iVar2 + 1 + iVar1));
        *ps = (int64_t)plVar3;
        memcpy((void *)((int64_t)iVar1 + (int64_t)plVar3), src, iVar2 + 1);
    }
    return ps;
}

/* ========== PECMD_ArrayGrowRaw @0x140063978 ==========
 * 数组增长原始实现。
 */
void PECMD_ArrayGrowRaw(uint64_t *arr, int64_t *cap, uint32_t esize,
                        int64_t grow)
{
    int iVar2 = (int)grow + (int)*cap;
    int64_t *plVar1 = (int64_t *)(uintptr_t)PECMD_HeapRealloc((void *)*arr,
                                                   (size_t)(iVar2 * (int)esize));
    *arr = (uint64_t)plVar1;
    memset((void *)(*cap * (uint64_t)esize + (uint64_t)plVar1), 0,
           (size_t)((uint64_t)esize * (uint64_t)grow));
    *cap = iVar2;
}

/* ========== FUN_140063BE8 @0x140063be8 ==========
 * 初始化指针数组。
 */
void FUN_140063BE8(int64_t *arr, int64_t count)
{
    int64_t lVar3 = 0;
    if (count < 0) {
        count = 0;
    }
    int64_t lVar1 = count + 1;
    int64_t *plVar2 = (int64_t *)(uintptr_t)PECMD_HeapRealloc((void *)*arr, (size_t)(lVar1 * 8));
    *arr = (int64_t)plVar2;
    if (0 < lVar1) {
        do {
            *(uint64_t *)(*arr + lVar3 * 8) = 0;
            lVar3++;
        } while (lVar3 < lVar1);
    }
    arr[2] = count;
    arr[1] = lVar1;
}

/* ========== FUN_14006857C @0x14006857c ==========
 * 读取加密字节。
 */
uint64_t FUN_14006857C(int64_t stream)
{
    uint8_t local_res8[8] = {0};
    DWORD local_res10[6] = {0};
    bool bVar1 = FUN_14005F1B0((void *)(stream + 8), local_res8, 1, local_res10);
    if (!bVar1 || local_res10[0] == 0) {
        return 0xffffffff;
    }
    uint32_t uVar3 = (*(int *)(stream + 0x34) * 2 + 3) * *(int *)(stream + 0x30);
    *(int *)(stream + 0x34) = *(int *)(stream + 0x34) + 1;
    return (uint64_t)(uint8_t)(local_res8[0] ^
                  ((uint8_t)(uVar3 >> 0x15) & 0x7e) ^ (uint8_t)(uVar3 >> 0xe) ^
                  (uint8_t)(uVar3 >> 7) ^ (uint8_t)uVar3);
}

/* ========== FUN_140069B68 @0x140069b68 ==========
 * 解析 token。
 */
void FUN_140069B68(uint64_t ctx, int64_t *pp, uint32_t flags)
{
    int16_t sVar1 = (int16_t)flags;
    LPCWSTR local_res10 = (LPCWSTR)*pp;
    *pp = 0;
    LPCWSTR local_res20 = local_res10;
    if ((flags & 0x70000000) == 0x10000000) {
        FUN_14005B154((WCHAR **)&local_res10);
    }
    int local_res18[2] = {sVar1 - 1, 0};
    PECMD_ExtractTokenByIndex(ctx, local_res18, local_res10, pp, 0x40000, NULL);
    FUN_14005B104((WCHAR **)&local_res20);
}

/* ========== FUN_140073C58 @0x140073c58 ==========
 * 清理解析器。
 */
void FUN_140073C58(int64_t parser)
{
    EnterCriticalSection(&g_csInit);
    FUN_14006703c(parser);
    if (*(int64_t *)(parser + 400) != 0) {
        PECMD_ReleaseObjectListTail(parser, 0);
    }
    if (*(int64_t *)(parser + 400) != 0) {
        HeapFree(g_hHeap, 0, (void *)(*(int64_t *)(parser + 400) - 8));
        *(uint64_t *)(parser + 400) = 0;
        *(uint32_t *)(parser + 0x198) = 0;
    }
    LeaveCriticalSection(&g_csInit);
}

/* ========== FUN_140078E90 @0x140078e90 ==========
 * 读取剪贴板文本。
 */
int64_t *FUN_140078E90(int64_t *out)
{
    *(uint16_t *)*out = 0;
    if (OpenClipboard((HWND)0)) {
        HANDLE hMem = GetClipboardData(0xd);
        if (hMem != 0) {
            LPCWSTR pWVar2 = (LPCWSTR)GlobalLock(hMem);
            FUN_14007034C(out, pWVar2);
            GlobalUnlock(hMem);
        }
        CloseClipboard();
    }
    return out;
}

/* ========== PECMD_ParseNumericField @0x140079980 ==========
 * 解析数字字段。
 */
uint64_t PECMD_ParseNumericField(int64_t obj, uint64_t unused, int16_t *start,
                                int16_t *end)
{
    (void)unused;
    if ((*end == 0) && (*start != 0)) {
        int local_res20[2] = {-0x7fffff7, 0};
        int16_t *local_res18 = start;
        PECMD_ParseUIntValue((int64_t *)&local_res18, local_res20);
        if (local_res20[0] != -0x7fffff7) {
            if (local_res20[0] < 1) {
                *(uint32_t *)(obj + 0x70) = 0xffffffff;
                *(uint16_t *)(obj + 0x7c) = 0;
            } else {
                *(uint16_t *)(obj + 0x7c) = *(uint16_t *)(obj + 0x7e);
                *(uint32_t *)(obj + 0x70) = *(uint32_t *)(obj + 0x74);
            }
        }
    }
    return 0;
}

/* ========== FUN_140079C80 @0x140079c80 ==========
 * 连接 tokens。
 */
int64_t FUN_140079C80(int64_t *ctx, int64_t *pp, int64_t *out)
{
    LPCWSTR local_res10 = NULL;
    FUN_140063620((WCHAR **)&local_res10);
    FUN_1400547BC(ctx, pp, (int64_t *)&local_res10, 0x2c, 0);
    PECMD_ResolveVariable(ctx, local_res10, (uint64_t *)out, 0);
    int64_t lVar1 = *out;
    *pp = lVar1;
    FUN_14005B104((WCHAR **)&local_res10);
    return lVar1;
}

/* ========== FUN_14007EFA4 @0x14007efa4 ==========
 * 解析路径记录（变体）。
 */
uint64_t FUN_14007EFA4(LPWSTR path, uint8_t *flags)
{
    int local_68[2];
    uint8_t local_60 = 0;
    int64_t local_58[3] = {0, 0, 0};
    uint8_t local_40 = 0;
    int64_t local_38[3] = {0, 0, 0};
    uint8_t local_20 = 0;
    FUN_14007E34C((uint32_t *)local_68, 1);
    uint64_t *puVar2 = (uint64_t *)PECMD_ParseExpression(local_68, path);
    uint64_t uVar1 = *puVar2;
    *flags = (uint8_t)(local_60 | local_40 | local_20);
    FUN_14005B104((WCHAR **)local_38);
    FUN_14005B104((WCHAR **)local_58);
    return uVar1;
}

/* ========== FUN_1400A4350 @0x1400a4350 ==========
 * 释放互斥体对象。
 */
uint64_t *FUN_1400A4350(uint64_t *obj, uint64_t flags)
{
    *obj = (uint64_t)(uintptr_t)PTR_FUN_140128ed0;
    if (*(char *)((uint8_t *)obj + 16) != '\0') {
        ReleaseMutex((HANDLE)obj[1]);
        *(uint8_t *)((uint8_t *)obj + 16) = 0;
        ((void (*)(void *, int))(uintptr_t)*obj)(obj, 1);
    }
    if (obj[3] != 0) {
        FUN_14006E6C8((uint64_t *)obj[3]);
        obj[3] = 0;
    }
    *obj = (uint64_t)(uintptr_t)PTR__purecall_140126050;
    if ((flags & 1) != 0) {
        free(obj);
    }
    return obj;
}

/* ========== FUN_1400A9C40 @0x1400a9c40 ==========
 * 释放对象（vtable 140129040）。
 */
void FUN_1400A9C40(uint64_t *obj)
{
    *obj = (uint64_t)(uintptr_t)PTR_FUN_140129040;
    if (*(int *)((uint8_t *)obj + 0x10 * 8) != 0) {
        KillTimer((HWND)obj[0xf], (uintptr_t)*(int *)((uint8_t *)obj + 0xe * 8));
    }
    *(uint32_t *)((uint8_t *)obj + 0x10 * 8) = 0;
    *(uint32_t *)((uint8_t *)obj + 0x84) = 0;
    *obj = (uint64_t)(uintptr_t)PTR_FUN_1401234f0;
    FUN_1400F1490((int64_t)(obj + 0xb));
    FUN_14005B104((WCHAR **)(obj + 5));
    FUN_14005B104((WCHAR **)(obj + 4));
    FUN_14005B104((WCHAR **)(obj + 3));
    FUN_14005B104((WCHAR **)(obj + 2));
}

/* ========== FUN_1400B0380 @0x1400b0380 ==========
 * 运行命令或函数。
 */
int64_t FUN_1400B0380(int64_t *ctx, uint64_t unused, uint64_t *args)
{
    (void)unused;
    uint16_t *puVar1 = (uint16_t *)args[2];
    LPCWSTR pWVar4 = (LPCWSTR)args[1];
    int64_t lVar3;
    if (puVar1 < (uint16_t *)0x10000) {
        uint64_t uVar2 = PECMD_ComboBoxControl(args[3], *args, pWVar4,
                                       (int16_t)(char)(uintptr_t)puVar1, NULL, 0);
        lVar3 = (int64_t)(int)uVar2;
    } else {
        uint64_t uVar2 = FUN_14005C7C4("cmd", puVar1);
        if ((char)uVar2 == '\0') {
            lVar3 = FUN_1400E5AAC(ctx, args);
        } else {
            FUN_1400703E4((int64_t *)(args[6] + 0x28), pWVar4);
            lVar3 = 0;
        }
    }
    return lVar3;
}
