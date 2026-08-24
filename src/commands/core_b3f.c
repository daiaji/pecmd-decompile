/* ====================================================================
 * core_b3f.c — B3-B6 小函数批 6 (140040000-1400bffff)
 *
 * 本批开始新实现函数直接使用人类可读 PECMD_ 名称，原始地址保留在 @0x 注释。
 *
 * 来源: PECMD原始.EXE (x64, ImageBase=0x140000000)
 *   参数解析扩展     PECMD_CreateVarWithValue @0x1400402ec
 *   去尾反斜杠       PECMD_StripTrailingSlash @0x140040874
 *   加载 RichEdit    PECMD_LoadRichEdit @0x140054110
 *   对象释放         PECMD_MultiSlotDtor @0x140054958
 *   窗口回引用       FUN_14005B77C @0x14005b77c
 *   删除注册表值     FUN_14005C61C @0x14005c61c
 *   加载公共对话框   PECMD_LoadComDlgApis @0x14005c904
 *   图标组判定       FUN_14005D7E8 @0x14005d7e8
 *   释放句柄对象     FUN_14005E310 @0x14005e310
 *   缓冲写字节       FUN_14005F158 @0x14005f158
 *   24 字节交换      PECMD_SwapBufDesc24 @0x14005f3bc
 *   设备 flush       PECMD_FlushDeviceIoctl @0x140060120
 *   删除串中字符     PECMD_RemoveFirstMatchChar @0x140060290
 *   获取编辑选择     PECMD_GetEditSelRange @0x140063c70
 *   设置窗口主题     PECMD_SetWindowTheme @0x140066054
 *   匹配赋值 token   PECMD_MatchAssignToken @0x140066188
 *   解析颜色/数值    PECMD_ParseIntOrColor @0x140066850
 *   解析整数取整     PECMD_ParseIntRound @0x140067e2c
 *   解析 int64 取整  PECMD_ParseInt64Round @0x140067e88
 *   初始化流         PECMD_InitStreamObject @0x140068440
 *   关闭流           FUN_14006849C @0x14006849c
 *   写加密字节       PECMD_WriteEncByte @0x1400684fc
 *   写 XOR 字节      PECMD_WriteXorByte @0x140068880
 *   转换串编码       FUN_14006CA94 @0x14006ca94
 *   打包串记录       PECMD_AllocFileReqEmbedPath @0x14006d788
 *   初始化解析上下文 FUN_1400706B4 @0x1400706b4
 *   重置解析上下文   FUN_14007E34C @0x14007e34c
 *   解析路径记录     PECMD_ParsePathRecord @0x14007f018
 *   解析 $ 移位      PECMD_ParseDollarShift @0x1400857e4
 *   创建字体 DPI     PECMD_CreateFontAdjusted @0x1400b2218
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

/* ---- 未实现依赖 (extern + TODO(verify)) ---- */
extern int64_t *PECMD_CreateVariable(int64_t *obj, uint64_t value, LPCWSTR text, int64_t *extra);
extern LARGE_INTEGER FUN_14003C06C(int64_t *script, LARGE_INTEGER cmd, uint32_t mode);
extern void PECMD_ClearStringItemList(int64_t arr);
extern void PECMD_GetApiProcCached(LPCSTR name, LPCSTR dll, int64_t *out, int64_t *err);
extern bool PECMD_PrefixMatchNoCase(uint16_t *s, char *key);
extern bool FUN_1400C1194(int64_t *pp, uint64_t *out);
extern uint64_t PECMD_EvalExpressionTree(int64_t *pp, double *out);
extern uint64_t *PECMD_AllocSmallObject(uint64_t *arr);
extern uint64_t *PECMD_GrowByteBuffer(uint64_t *arr, int64_t size);
extern int64_t *FUN_1400637DC(int64_t *ps, LPCSTR src, uint64_t srclen, uint64_t codepage);
extern void FUN_140063A6C(uint64_t *arr, int64_t *end, uint64_t *flags, uint32_t esize);
extern uint64_t PECMD_ParseExpression(int *ctx, LPWSTR path);
extern int64_t PECMD_EvalExprSkipOneChar(int64_t *pp, uint64_t *out);
extern HFONT FUN_1400B1F34(int *lf, double *size, LPCWSTR name);
extern void PECMD_FlushFileThrice(HANDLE hFile);
extern void FUN_14005C898(LPCSTR name, LPCSTR dll, int64_t *out, int64_t *err);
extern void *PECMD_GrowArrayWrap(int64_t idx, int64_t *arr, int64_t *end, uint8_t *flags,
                                 uint32_t esize);

/* ---- 本批引用的全局数据 ---- */
extern HMODULE g_hRichEdit;
extern int64_t g_i64CCB8;
extern void *g_pSetWindowTheme; /* SetWindowTheme 函数指针 */
extern uint8_t g_bD500[];       /* 主题名 */
extern uint8_t PTR_FUN_1401234f0[];
extern int (*g_pGetSaveFileNameW)(void);
extern int64_t g_i64D428;
extern int64_t g_i64D438;
extern double g_dbl20b28;
extern double g_dbl25230;
extern double g_fontRound;

/* ========== PECMD_CreateVarWithValue @0x1400402ec ==========
 * 参数解析扩展：特殊标志时写回参数。
 */
int64_t *PECMD_CreateVarWithValue(int64_t *obj, uint64_t value, LPCWSTR text, int64_t *extra)
{
    int64_t *plVar2 = PECMD_CreateVariable(obj, value, text, extra);
    if (((extra != (int64_t *)0x102) && (extra != (int64_t *)0x101)) && (plVar2 != NULL)) {
        uint64_t *puVar1 = (uint64_t *)plVar2[1];
        *(uint16_t *)((uint8_t *)puVar1 + 8) = 0;
        *puVar1 = value;
    }
    if ((((uint64_t)extra >> 8 & 1) != 0) && (plVar2 != NULL)) {
        *(uint64_t *)(plVar2[2] + 0x10) = (uint64_t)extra & 0xff;
    }
    return plVar2;
}

/* ========== PECMD_StripTrailingSlash @0x140040874 ==========
 * 去掉末尾反斜杠并执行脚本回调。
 */
void PECMD_StripTrailingSlash(int64_t *path)
{
    if (*(LPCWSTR)*path != L'\0') {
        int iVar1 = lstrlenW((LPCWSTR)*path);
        if (*(int16_t *)(*path - 2 + (int64_t)iVar1 * 2) == 0x5c) {
            *(uint16_t *)(*path - 2 + (int64_t)iVar1 * 2) = 0;
        }
        FUN_14003C06C((int64_t *)g_Script, PECMD_LI(*path), 1);
        *(uint16_t *)*path = 0;
    }
}

/* ========== PECMD_LoadRichEdit @0x140054110 ==========
 * 加载 RichEdit DLL 并缓存状态。
 */
char PECMD_LoadRichEdit(void)
{
    if (g_richEditMode == 0) {
        g_hRichEdit = LoadLibraryA("riched20.dll");
        if (g_hRichEdit == (HMODULE)0) {
            g_hRichEdit = LoadLibraryA("RICHED32.DLL");
            g_richEditMode = (uint8_t)(g_hRichEdit != (HMODULE)0);
        }
        else {
            g_richEditMode = 2;
        }
    }
    return (char)g_richEditMode;
}

/* ========== PECMD_MultiSlotDtor @0x140054958 ==========
 * 释放多槽对象。
 */
uint64_t *PECMD_MultiSlotDtor(uint64_t *obj, uint32_t flags)
{
    *obj = (uint64_t)(uintptr_t)PTR_FUN_1401234f0;
    PECMD_ClearStringItemList((int64_t)(obj + 0xb));
    PECMD_FreeStrBuf((WCHAR **)(obj + 5));
    PECMD_FreeStrBuf((WCHAR **)(obj + 4));
    PECMD_FreeStrBuf((WCHAR **)(obj + 3));
    PECMD_FreeStrBuf((WCHAR **)(obj + 2));
    if ((flags & 1) != 0) {
        free(obj);
    }
    return obj;
}

/* ========== FUN_14005B77C @0x14005b77c ==========
 * 获取窗口回引用并计数。
 */
uint64_t FUN_14005B77C(int64_t obj)
{
    uint64_t uVar2 = 0;
    EnterCriticalSection(&g_csInit);
    if ((obj != 0) && (*(HWND *)(obj + OBJ_HWND) != (HWND)0)) {
        LONG_PTR LVar1 = GetWindowLongPtrW(*(HWND *)(obj + OBJ_HWND), -0x15);
        if (LVar1 == obj) {
            uVar2 = *(uint64_t *)(obj + 0x20);
            *(int64_t *)(obj + 0x1c8) = *(int64_t *)(obj + 0x1c8) + 1;
        }
    }
    LeaveCriticalSection(&g_csInit);
    return uVar2;
}

/* ========== FUN_14005C61C @0x14005c61c ==========
 * 打开注册表键并删除值。
 */
DWORD FUN_14005C61C(HKEY root, LPCWSTR sub, LPCWSTR name)
{
    DWORD DVar1;
    HKEY local_res8[4] = {(HKEY)0, 0, 0, 0};
    if (((sub == NULL) || (DVar1 = FUN_14005C394(root, sub, &local_res8[0], 2, 4),
                           root = local_res8[0], DVar1 == 0)) &&
        (DVar1 = RegDeleteValueW(root, name), local_res8[0] != (HKEY)0)) {
        RegCloseKey(local_res8[0]);
    }
    return DVar1;
}

/* ========== PECMD_LoadComDlgApis @0x14005c904 ==========
 * 加载通用对话框函数。
 */
void PECMD_LoadComDlgApis(void)
{
    if (g_pGetSaveFileNameW == 0) {
        PECMD_GetApiProcCached("GetOpenFileNameW", "COMDLG32.DLL", (int64_t *)&g_i64D428,
                               (int64_t *)&g_i64D438);
        PECMD_GetApiProcCached("GetSaveFileNameW", "COMDLG32.DLL",
                               (int64_t *)(void **)&g_pGetSaveFileNameW, (int64_t *)&g_i64D438);
    }
}

/* ========== FUN_14005D7E8 @0x14005d7e8 ==========
 * 图标组资源名判定。
 */
uint32_t FUN_14005D7E8(uint16_t *s)
{
    if (!PECMD_PrefixMatchNoCase(s, "icon Group")) {
        if (!PECMD_PrefixMatchNoCase(s, "iconGroup")) {
            if (!PECMD_PrefixMatchNoCase(s, "#14")) {
                return 0;
            }
        }
    }
    return 1;
}

/* ========== FUN_14005E310 @0x14005e310 ==========
 * 释放句柄对象并归还堆。
 */
void FUN_14005E310(int64_t *slot)
{
    if (slot != NULL) {
        if (*slot != 0) {
            EnterCriticalSection(&g_csInit);
            g_i64CCB8--;
            LeaveCriticalSection(&g_csInit);
            HANDLE hObject = (HANDLE)*slot;
            *slot = 0;
            CloseHandle(hObject);
        }
        HeapFree(g_hHeap, 0, slot - 1);
    }
}

/* ========== FUN_14005F158 @0x14005f158 ==========
 * 向缓冲写入一个字节，满 0x1000 落盘。
 */
uint64_t FUN_14005F158(uint64_t *stream, uint8_t *data)
{
    int64_t lVar1 = stream[2];
    stream[2] = lVar1 + 1;
    *(uint8_t *)(lVar1 + stream[1]) = *data;
    if (0xfff < (int64_t)stream[2]) {
        DWORD local_res8[8];
        WriteFile((HANDLE)*stream, (LPCVOID)stream[1], *(DWORD *)(stream + 2), local_res8, NULL);
        stream[2] = 0;
    }
    return 1;
}

/* ========== PECMD_SwapBufDesc24 @0x14005f3bc ==========
 * 交换 24 字节。
 */
void PECMD_SwapBufDesc24(uint8_t *a, uint8_t *b)
{
    uint8_t local_28[32];
    memcpy(local_28, a, 0x18);
    memcpy(a, b, 0x18);
    memcpy(b, local_28, 0x18);
}

/* ========== PECMD_FlushDeviceIoctl @0x140060120 ==========
 * flush 设备并执行 IOCTL。
 */
DWORD PECMD_FlushDeviceIoctl(HANDLE hDevice)
{
    DWORD DVar2 = 0;
    DWORD local_res10[6] = {0};
    PECMD_FlushFileThrice(hDevice);
    BOOL BVar1 = DeviceIoControl(hDevice, 0x70140, NULL, 0, NULL, 0, local_res10, NULL);
    if (BVar1 == 0) {
        DVar2 = GetLastError();
    }
    return DVar2;
}

/* ========== PECMD_RemoveFirstMatchChar @0x140060290 ==========
 * 删除串中第一个匹配字符。
 */
void PECMD_RemoveFirstMatchChar(uint16_t ch, char *s)
{
    int16_t sVar2 = 0;
    char cVar1 = *s;
    while (1) {
        if (cVar1 == '\0') {
            return;
        }
        if (0x19 < sVar2)
            break;
        if ((uint32_t)ch == (uint32_t)(int)s[sVar2]) {
            memmove(s + sVar2, s + sVar2 + 1, 0x1a - sVar2);
            return;
        }
        sVar2++;
        cVar1 = s[sVar2];
    }
}

/* ========== PECMD_GetEditSelRange @0x140063c70 ==========
 * 获取编辑框选择范围。
 */
void PECMD_GetEditSelRange(int64_t obj, LRESULT *start, LRESULT *end)
{
    *start = SendMessageW(*(HWND *)(obj + OBJ_HWND), 0x401, 0, 0);
    *end = SendMessageW(*(HWND *)(obj + OBJ_HWND), 0x402, 0, 0);
}

/* ========== PECMD_SetWindowTheme @0x140066054 ==========
 * 设置窗口主题。
 */
void PECMD_SetWindowTheme(uint64_t hwnd)
{
    if (g_pSetWindowTheme == NULL) {
        FUN_14005C898("SetWindowTheme", "UxTheme.dll", (int64_t *)&g_pSetWindowTheme, NULL);
    }
    if ((int64_t)g_pSetWindowTheme - 1 < (int64_t)-2) {
        ((void (*)(uint64_t, void *, void *))g_pSetWindowTheme)(hwnd, g_bD500, g_bD500);
    }
}

/* ========== PECMD_MatchAssignToken @0x140066188 ==========
 * 匹配赋值 token（= 或 :）。
 */
uint64_t PECMD_MatchAssignToken(char *key, int64_t *pp, int len)
{
    uint16_t *puVar1 = (uint16_t *)*pp;
    uint64_t uVar3 = (uint64_t)len;
    uint64_t uVar2 = FUN_14005C788(key, puVar1, len);
    if (((char)uVar2 == '\0') ||
        ((puVar1[uVar3] != 0x3d && (uVar2 = uVar3, puVar1[uVar3] != 0x3a)))) {
        uVar2 &= 0xffffffffffffff00ULL;
    }
    else {
        *pp = (int64_t)(puVar1 + ((int)uVar3 + 1));
        uVar2 = 1;
    }
    return uVar2;
}

/* ========== PECMD_ParseIntOrColor @0x140066850 ==========
 * 解析颜色或数值。
 */
uint32_t PECMD_ParseIntOrColor(uint64_t *pp, uint64_t *out)
{
    bool bVar3 = *(uint16_t *)*pp == 0x40;
    if (bVar3) {
        *pp = (uint64_t)((uint16_t *)*pp + 1);
    }
    bool bVar1 = FUN_1400C1194((int64_t *)pp, out);
    if (bVar3) {
        DWORD DVar2 = GetSysColor((int)*out);
        *out = DVar2;
    }
    return bVar1 ? 1 : 0;
}

/* ========== PECMD_ParseIntRound @0x140067e2c ==========
 * 解析整数并四舍五入。
 */
uint64_t PECMD_ParseIntRound(int64_t *pp, int *out)
{
    double local_res10[3];
    local_res10[0] = (double)*out;
    uint64_t uVar1 = PECMD_EvalExpressionTree(pp, local_res10);
    if ((int)uVar1 < 1) {
        return 0;
    }
    if (local_res10[0] < g_dbl20b28) {
        local_res10[0] -= g_dbl25230;
    }
    else {
        local_res10[0] += g_dbl25230;
    }
    *out = (int)local_res10[0];
    return 1;
}

/* ========== PECMD_ParseInt64Round @0x140067e88 ==========
 * 解析 int64 并四舍五入。
 */
uint64_t PECMD_ParseInt64Round(int64_t *pp, int64_t *out)
{
    double local_res10[3];
    local_res10[0] = (double)*out;
    uint64_t uVar1 = PECMD_EvalExpressionTree(pp, local_res10);
    if ((int)uVar1 < 1) {
        return 0;
    }
    if (local_res10[0] < g_dbl20b28) {
        local_res10[0] -= g_dbl25230;
    }
    else {
        local_res10[0] += g_dbl25230;
    }
    *out = (int64_t)local_res10[0];
    return 1;
}

/* ========== PECMD_InitStreamObject @0x140068440 ==========
 * 初始化流对象。
 */
int64_t *PECMD_InitStreamObject(int64_t *stream, int64_t handle)
{
    if (handle == -1) {
        handle = 0;
    }
    *stream = handle;
    PECMD_AllocSmallObject((uint64_t *)(stream + 1));
    stream[2] = 0;
    stream[3] = 0;
    *(uint32_t *)((uint8_t *)stream + 0x24) = 10000;
    *(uint8_t *)(stream + 4) = 0;
    PECMD_GrowByteBuffer((uint64_t *)(stream + 1), 0x1000);
    return stream;
}

/* ========== FUN_14006849C @0x14006849c ==========
 * 关闭流并释放资源。
 */
void FUN_14006849C(uint64_t *stream)
{
    DWORD local_res8[8];
    if ((*(char *)((uint8_t *)stream + 32) != '\0') && (0 < (int64_t)stream[2])) {
        WriteFile((HANDLE)*stream, (LPCVOID)stream[1], *(DWORD *)(stream + 2), local_res8, NULL);
    }
    stream[2] = 0;
    PECMD_FreeStrBuf((WCHAR **)(stream + 1));
    HANDLE hObject = (HANDLE)*stream;
    if ((hObject != (HANDLE)0) && (hObject != INVALID_HANDLE_VALUE)) {
        CloseHandle(hObject);
    }
    *stream = 0;
}

/* ========== PECMD_WriteEncByte @0x1400684fc ==========
 * 写加密字节。
 */
void PECMD_WriteEncByte(int64_t stream, uint8_t value)
{
    uint8_t local_res10[24];
    uint32_t uVar1 = (*(int *)(stream + 0x34) * 2 + 3) * *(int *)(stream + 0x30);
    *(int *)(stream + 0x34) = *(int *)(stream + 0x34) + 1;
    local_res10[0] =
        (uint8_t)(value ^ (((uint8_t)(uVar1 >> 0x15) & 0x7e) ^ (uint8_t)(uVar1 >> 0xe) ^
                           (uint8_t)(uVar1 >> 7) ^ (uint8_t)uVar1));
    FUN_14005F158((uint64_t *)(stream + 8), local_res10);
}

/* ========== PECMD_WriteXorByte @0x140068880 ==========
 * 写 XOR 字节到流。
 */
uint64_t PECMD_WriteXorByte(int64_t stream, uint8_t value, int len)
{
    int64_t lVar1 = *(int64_t *)(stream + 0x18);
    if (lVar1 <= (int64_t)len + 3 + *(int64_t *)(stream + 0x10)) {
        *(int64_t *)(stream + 0x18) = lVar1 + 0x400;
        PECMD_GrowByteBuffer((uint64_t *)(stream + 8), lVar1 + 0x404);
    }
    *(uint8_t *)(*(int64_t *)(stream + 8) + *(int64_t *)(stream + 0x10)) =
        *(uint8_t *)(stream + 0x24) ^ value;
    *(int64_t *)(stream + 0x10) = *(int64_t *)(stream + 0x10) + 1;
    return 0;
}

/* ========== FUN_14006CA94 @0x14006ca94 ==========
 * 转换字符串编码并替换容器。
 */
int64_t FUN_14006CA94(int64_t *ps, uint32_t codepage)
{
    int64_t local_res8[4] = {0, 0, 0, 0};
    if (((codepage != 0x4b0) && (codepage != 0x4b1)) && (-1 < (int)codepage)) {
        FUN_1400637DC(local_res8, (LPCSTR)*ps, 0xffffffffffffffffULL, (uint64_t)(uint16_t)codepage);
        int64_t lVar1 = *ps;
        *ps = local_res8[0];
        local_res8[0] = lVar1;
        PECMD_FreeStrBuf((WCHAR **)local_res8);
    }
    return *ps;
}

/* ========== PECMD_AllocFileReqEmbedPath @0x14006d788 ==========
 * 打包字符串记录。
 */
void PECMD_AllocFileReqEmbedPath(int64_t *out, uint64_t *src)
{
    int iVar3 = lstrlenW((LPCWSTR)*src);
    PECMD_AllocString((WCHAR **)out, (int64_t)(iVar3 + 0x21));
    uint8_t *puVar1 = (uint8_t *)*src;
    *(uint32_t *)(*out + 0x3c) = 0;
    int64_t lVar2 = *out;
    *src = (uint64_t)(lVar2 + 0x40);
    memcpy((void *)(lVar2 + 0x40), puVar1, (iVar3 + 1) * 2);
}

/* ========== FUN_1400706B4 @0x1400706b4 ==========
 * 初始化解析上下文。
 */
void FUN_1400706B4(uint32_t *ctx)
{
    *ctx = 0;
    ctx[1] = 0xffffffff;
    uint16_t *puVar1 = (uint16_t *)PECMD_GrowArrayWrap(
        0, (int64_t *)(ctx + 4), (int64_t *)(ctx + 6), (uint8_t *)(ctx + 10), 2);
    *puVar1 = 0x23;
    uint64_t *puVar2 = (uint64_t *)PECMD_GrowArrayWrap(
        0, (int64_t *)(ctx + 0xc), (int64_t *)(ctx + 0xe), (uint8_t *)(ctx + 0x12), 8);
    *puVar2 = 0;
    *(uint8_t *)((uint8_t *)ctx + 9) = 0x20;
}

/* ========== FUN_14007E34C @0x14007e34c ==========
 * 重置解析上下文。
 */
uint32_t *FUN_14007E34C(uint32_t *ctx, uint8_t mode)
{
    *(uint8_t *)((uint8_t *)ctx + 10) = mode;
    *(uint8_t *)((uint8_t *)ctx + 8) = 0;
    *(uint8_t *)((uint8_t *)ctx + 10) = 0;
    FUN_140063A6C((uint64_t *)(ctx + 4), (int64_t *)(ctx + 6), (uint64_t *)(ctx + 8), 2);
    *(uint8_t *)((uint8_t *)ctx + 0x12) = 0;
    FUN_140063A6C((uint64_t *)(ctx + 0xc), (int64_t *)(ctx + 0xe), (uint64_t *)(ctx + 0x10), 8);
    FUN_1400706B4(ctx);
    return ctx;
}

/* ========== PECMD_ParsePathRecord @0x14007f018 ==========
 * 解析路径记录。
 */
uint64_t PECMD_ParsePathRecord(LPWSTR path, uint8_t *flags)
{
    int local_58[2];
    uint8_t local_50 = 0;
    int64_t local_48[3] = {0, 0, 0};
    uint8_t local_30 = 0;
    int64_t local_28[3] = {0, 0, 0};
    uint8_t local_10 = 0;
    FUN_14007E34C((uint32_t *)local_58, 0);
    uint64_t *puVar2 = (uint64_t *)PECMD_ParseExpression(local_58, path);
    uint64_t uVar1 = *puVar2;
    *flags = (uint8_t)(local_50 | local_30 | local_10);
    PECMD_FreeStrBuf((WCHAR **)local_28);
    PECMD_FreeStrBuf((WCHAR **)local_48);
    return uVar1;
}

/* ========== PECMD_ParseDollarShift @0x1400857e4 ==========
 * 解析 $ 移位计数。
 */
void PECMD_ParseDollarShift(int64_t *pp, uint64_t *out, uint8_t *shift)
{
    if (*(uint16_t *)*pp == 0x24) {
        shift[0] = 0;
        shift[1] = 0;
        while (*(uint16_t *)*pp == 0x24) {
            *(int64_t *)shift = *(int64_t *)shift + 1;
            *pp += 2;
        }
    }
    int iVar1 = (int)PECMD_EvalExprSkipOneChar(pp, out);
    if ((0 < iVar1) && (0 < *(int64_t *)shift)) {
        *out = *out << (*(int64_t *)shift & 0x3f);
    }
}

/* ========== PECMD_CreateFontAdjusted @0x1400b2218 ==========
 * 按 DPI 创建字体。
 */
HFONT PECMD_CreateFontAdjusted(int *lf, int *size, LPCWSTR name)
{
    double local_res10[3];
    local_res10[0] = (double)*size;
    HFONT pHVar1 = FUN_1400B1F34(lf, local_res10, name);
    if (local_res10[0] < g_dbl20b28) {
        local_res10[0] -= g_fontRound;
    }
    else {
        local_res10[0] += g_fontRound;
    }
    *size = (int)local_res10[0];
    return pHVar1;
}
