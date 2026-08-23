/* ====================================================================
 * core_b3b.c — B3-B6 小函数批 2 (140040000-1400bffff)
 *
 * 来源: PECMD原始.EXE (x64, ImageBase=0x140000000)
 *   ATL 抛错          AtlThrowImpl @0x14004f908
 *   对象释放          PECMD_ReleaseObject_4490 @0x140054490
 *   选对象入槽        PECMD_SelectObjectSlot_b028 @0x14005b028
 *   选对象入槽        PECMD_SelectObjectSlot_b054 @0x14005b054
 *   浮点取负          PECMD_NegateDouble @0x14005bc74
 *   表初始化          PECMD_InitTable @0x14005bccc
 *   ANSI/W 串比较     PECMD_CompareAnsiWide @0x14005c800
 *   分隔符计数        PECMD_CountSeparators @0x14005cbd8
 *   容器释放          PECMD_FreeContainer @0x14005d5dc
 *   LCG 更新          PECMD_UpdateLcg @0x14005e028
 *   计数+转发         PECMD_IncAndForward_f2b0 @0x14005f2b0
 *   多次 flush        PECMD_FlushFileThrice @0x1400600f8
 *   字母/数字判定     PECMD_IsAlphaNumeric @0x140061204
 *   互斥体释放        PECMD_ReleaseMutexAndObject @0x140062074
 *   对象释放          PECMD_ReleaseObject_2f74 @0x140062f74
 *   自定义消息        PECMD_SendUserMessage @0x140063cc8
 *   解析+跳空白       PECMD_ParseAndSkipSpace_7b54 @0x140067b54
 *   计数+转发         PECMD_IncAndForward_8558 @0x140068558
 *   注册表读取包装    PECMD_RegReadWrap @0x140069218
 *   设备父节点        PECMD_GetDeviceParent @0x14006957c
 *   设备路径前缀      PECMD_StripDevicePrefix @0x14006acb8
 *   容器追加          PECMD_ContainerAppend @0x14006b8fc
 *   截断文件          PECMD_SetEndOfFileWrap @0x14006cb84
 *   数字解析+跳字     PECMD_ParseNumSkipChar_01d4 @0x1400701d4
 *   数字解析+跳字     PECMD_ParseNumSkipChar_01f8 @0x1400701f8
 *   数字解析+跳字     PECMD_ParseNumSkipChar_0224 @0x140070224
 *   数字解析+跳字     PECMD_ParseNumSkipChar_0248 @0x140070248
 *   设备安装          PECMD_InstallDevice @0x140076b68
 *   解析+跳空白       PECMD_ParseAndSkipSpace_7f90 @0x140077f90
 *   数字解析+跳字     PECMD_ParseNumSkipChar_de4c @0x14007de4c
 *
 * 约定:
 *   - 未实现内部函数 extern 挂起 + TODO(verify), 不臆造语义
 *   - 全局数据符号先用 extern 声明; 完整链接时统一在 core_globals.c 定义
 * ==================================================================== */
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>

#include "pecmd_defs.h"
extern WCHAR **FUN_14005B154(WCHAR **pp); /* @0x14005b154 */

/* ---- 未实现依赖 (extern + TODO(verify)) ---- */
extern void FUN_14005D558(void *obj, HWND hwnd);
extern void PECMD_RandSeedAdvance(void);
extern uint64_t FUN_14005F158(uint64_t *obj, uint8_t *data);
extern uint64_t PECMD_ParseNumberToDouble(int64_t *pp, double *out);
extern uint32_t PECMD_EnumDeviceInterfaces(int dev, uint32_t flags, void *ctx);
extern int FUN_14005B184(char *a, int64_t b, int64_t n);
extern void PECMD_SetWindowProcHook(uint64_t *arr, uint64_t value,
                                    LONG_PTR *slot);
extern LARGE_INTEGER PECMD_SetFilePointer(HANDLE hFile, LARGE_INTEGER pos, DWORD method);
extern uint64_t PECMD_ParseIntWithSign(void *pp, int *out);
extern bool PECMD_ParseUIntValue(int64_t *pp, int *out);
extern uint64_t PECMD_ParseIntRound(int64_t *pp, int *out);
extern uint64_t PECMD_ParseInt64Round(int64_t *pp, int64_t *out);
extern uint64_t PECMD_EvalExpressionTree(int64_t *pp, double *out);
extern uint32_t PECMD_EnumDevices(LPCWSTR path, LPWSTR buf, uint32_t flags,
                              void *guid);
extern uint64_t PECMD_ParseSizeWithUnit(int64_t *pp, double *out);
extern int FUN_1400690C0(HKEY root, LPCWSTR sub, LPCWSTR name, int64_t *out,
                         DWORD *type, LONG *status);

/* ---- 本批引用的全局数据 ---- */
extern double g_fontMinus0;         /* 常量 */
extern uint8_t g_bE870[];      /* GUID 数据 */

/* ========== AtlThrowImpl @0x14004f908 ==========
 * ATL 抛错: 按错误码抛异常。
 */
void AtlThrowImpl(long hr)
{
    DWORD dwExceptionCode = 0xc0000017;
    if (hr != -0x7ff8fff2) {
        dwExceptionCode = 0xc000001d;
    }
    RaiseException(dwExceptionCode, 1, 0, NULL);
    abort();
}

/* ========== PECMD_ReleaseObject_4490 @0x140054490 ==========
 * 调用两个对象释放函数后清空槽。
 */
void PECMD_ReleaseObject_4490(uint64_t *slot)
{
    ((void (*)(void *, int))g_pshutdown)((void *)*slot, 2);
    ((void (*)(void *))g_pclosesocket)((void *)*slot);
    *slot = 0;
}

/* ========== PECMD_SelectObjectSlot_b028 @0x14005b028 ==========
 * 选对象入槽: [2]=hdc [0]=obj [1]=old。
 */
void PECMD_SelectObjectSlot_b028(uint64_t *slot, HDC hdc, HGDIOBJ obj)
{
    HGDIOBJ pvVar1;
    slot[2] = (uint64_t)hdc;
    *slot = (uint64_t)obj;
    pvVar1 = SelectObject(hdc, obj);
    slot[1] = (uint64_t)pvVar1;
}

/* ========== PECMD_SelectObjectSlot_b054 @0x14005b054 ==========
 * 选对象入槽 (偏移版): +0x10=hdc +8=old。
 */
void PECMD_SelectObjectSlot_b054(int64_t ctx, HDC hdc, HGDIOBJ obj)
{
    HGDIOBJ pvVar1;
    *(HDC *)(ctx + 0x10) = hdc;
    pvVar1 = SelectObject(hdc, obj);
    *(HGDIOBJ *)(ctx + 8) = pvVar1;
}

/* ========== PECMD_NegateDouble @0x14005bc74 ==========
 * 浮点取负: 按标志用常量减或整数取负。
 */
void PECMD_NegateDouble(int64_t obj, double *value)
{
    if (*(char *)(obj + 10) != '\0') {
        *value = g_fontMinus0 - *value;
        return;
    }
    *value = (double)-(int64_t)*value;
}

/* ========== PECMD_InitTable @0x14005bccc ==========
 * 两段表初始化 0x1000。
 */
void PECMD_InitTable(int64_t obj)
{
    uint32_t *puVar2 = (uint32_t *)(obj + 0x9030);
    int64_t lVar1;
    for (lVar1 = 0x100; lVar1 != 0; lVar1--) {
        *puVar2 = 0x1000;
        puVar2++;
    }
    puVar2 = (uint32_t *)(obj + 0x9430);
    for (lVar1 = 0x1000; lVar1 != 0; lVar1--) {
        *puVar2 = 0x1000;
        puVar2++;
    }
}

/* ========== PECMD_CompareAnsiWide @0x14005c800 ==========
 * ANSI 与宽字符串比较; 返回差异高位或 1。
 * TODO(verify): Ghidra CONCAT71 尾部按返回 1 近似。
 */
int64_t PECMD_CompareAnsiWide(char *a, uint16_t *w)
{
    while ((*a != '\0') || (*w != 0)) {
        char cVar1 = *a;
        uint16_t uVar2 = *w;
        w++;
        a++;
        if ((uint32_t)(uint8_t)cVar1 != (uint32_t)uVar2) {
            return (int64_t)((uint16_t)(uVar2 >> 8) << 8);
        }
    }
    return 1;
}

/* ========== PECMD_CountSeparators @0x14005cbd8 ==========
 * 将 ;&:, 空格 替换为 \0 并计数。
 */
int PECMD_CountSeparators(char *s)
{
    int count = 0;
    for (; *s != '\0'; s++) {
        if ((((*s == ';') || (*s == '&')) || (*s == ':')) ||
            ((*s == ',' || (*s == ' ')))) {
            count++;
            *s = '\0';
        }
    }
    return count;
}

/* ========== PECMD_FreeContainer @0x14005d5dc ==========
 * 释放容器: 先调 FUN_14005D558 再释放。
 */
void PECMD_FreeContainer(int64_t *container)
{
    if (*container != 0) {
        FUN_14005D558(container, (HWND)0);
    }
    PECMD_FreeStrBuf((WCHAR **)container);
}

/* ========== PECMD_UpdateLcg @0x14005e028 ==========
 * LCG 状态更新并刷新。
 */
void PECMD_UpdateLcg(int64_t seed)
{
    g_lcgState = (g_lcgState + seed) & 0xffffffffffffLL;
    PECMD_RandSeedAdvance();
}

/* ========== PECMD_IncAndForward_f2b0 @0x14005f2b0 ==========
 * 计数器 +1 后转发一个字节参数。
 */
void PECMD_IncAndForward_f2b0(int64_t obj, uint8_t value)
{
    uint8_t local_res10[24];
    *(int *)(obj + 0x34) = *(int *)(obj + 0x34) + 1;
    local_res10[0] = value;
    FUN_14005F158((uint64_t *)(obj + 8), local_res10);
}

/* ========== PECMD_FlushFileThrice @0x1400600f8 ==========
 * 三次 FlushFileBuffers (反编译为三次调用)。
 */
void PECMD_FlushFileThrice(HANDLE hFile)
{
    FlushFileBuffers(hFile);
    FlushFileBuffers(hFile);
    FlushFileBuffers(hFile);
}

/* ========== PECMD_IsAlphaNumeric @0x140061204 ==========
 * 字母/数字判定。
 */
uint64_t PECMD_IsAlphaNumeric(uint16_t ch)
{
    if ((((ch < 0x61) || (0x7a < ch)) &&
         ((ch < 0x41 || (0x5a < ch)))) &&
        (9 < (uint16_t)(ch - 0x30))) {
        return 0;
    }
    return 1;
}

/* ========== PECMD_ReleaseMutexAndObject @0x140062074 ==========
 * 释放互斥体并调用对象释放函数。
 */
void PECMD_ReleaseMutexAndObject(uint64_t *obj)
{
    if (*(char *)((uint8_t *)obj + 16) != '\0') {
        ReleaseMutex((HANDLE)obj[1]);
    }
    ((void (*)(void *, int))(uintptr_t)*obj)(obj, 1);
}

/* ========== PECMD_ReleaseObject_2f74 @0x140062f74 ==========
 * 取对象指针后调用其释放函数。
 */
void PECMD_ReleaseObject_2f74(int64_t *slot)
{
    void *puVar1 = (void *)*slot;
    *slot = 0;
    if (puVar1 != NULL) {
        ((void (*)(void *, int))(uintptr_t)puVar1)(puVar1, 1);
    }
}

/* ========== PECMD_SendUserMessage @0x140063cc8 ==========
 * 发送 WM_USER 消息并返回 LRESULT。
 */
int64_t PECMD_SendUserMessage(int64_t obj)
{
    LRESULT LVar1 = SendMessageW(*(HWND *)(obj + OBJ_HWND), 0x400, 0, 0);
    return (int64_t)(int)LVar1;
}

/* ========== PECMD_ParseAndSkipSpace_7b54 @0x140067b54 ==========
 * 解析数字后跳空白。
 */
uint64_t PECMD_ParseAndSkipSpace_7b54(int64_t *pp, double *out)
{
    uint64_t uVar1 = PECMD_ParseNumberToDouble(pp, out);
    uVar1 &= 0xffffffff;
    FUN_14005B154((WCHAR **)pp);
    return uVar1 & 0xffffffff;
}

/* ========== PECMD_IncAndForward_8558 @0x140068558 ==========
 * 转发一个字节参数 (无计数前置)。
 */
void PECMD_IncAndForward_8558(int64_t obj, uint8_t value)
{
    uint8_t local_res10[24];
    local_res10[0] = value;
    FUN_14005F158((uint64_t *)(obj + 8), local_res10);
}

/* ========== PECMD_RegReadWrap @0x140069218 ==========
 * 注册表读取包装。
 */
void PECMD_RegReadWrap(HKEY root, LPCWSTR sub, LPCWSTR name,
                   int64_t *out, DWORD *type, LONG *status)
{
    FUN_1400690C0(root, sub, name, out, type, status);
}

/* ========== PECMD_GetDeviceParent @0x14006957c ==========
 * 取设备父节点。
 */
uint32_t PECMD_GetDeviceParent(int dev, uint32_t flags, void *ctx)
{
    DWORD DVar1;
    uint32_t uVar2;
    uint32_t local_res20[2] = {0, 0};
    DVar1 = PECMD_EnumDeviceInterfaces(dev, flags, ctx);
    uVar2 = 0;
    if (DVar1 != 0) {
        CM_Get_Parent(local_res20, (void *)(uintptr_t)DVar1, 0);
        uVar2 = local_res20[0];
    }
    return uVar2;
}

/* ========== PECMD_StripDevicePrefix @0x14006acb8 ==========
 * 去掉 "\\??\\" 前缀。
 */
int64_t PECMD_StripDevicePrefix(int64_t path)
{
    int iVar1 = FUN_14005B184((char *)WSTR("\\??\\"), path, 8);
    if (iVar1 == 0) {
        path += 8;
    }
    return path;
}

/* ========== PECMD_ContainerAppend @0x14006b8fc ==========
 * 向容器追加一个元素并返回新位置。
 */
int64_t PECMD_ContainerAppend(int64_t *container)
{
    PECMD_SetWindowProcHook((uint64_t *)container, (uint64_t)*container, (int64_t *)((uint8_t *)*container + 8));
    return *container + 8;
}

/* ========== PECMD_SetEndOfFileWrap @0x14006cb84 ==========
 * 设置文件指针并截断。
 */
void PECMD_SetEndOfFileWrap(uint64_t *file, LARGE_INTEGER pos)
{
    PECMD_SetFilePointer((HANDLE)*file, pos, 0);
    SetEndOfFile((HANDLE)*file);
}

/* ========== PECMD_ParseNumSkipChar_01d4 @0x1400701d4 ==========
 * 解析数字后若非结束则前进一个字符。
 */
void PECMD_ParseNumSkipChar_01d4(uint64_t *pp, int *out)
{
    PECMD_ParseIntWithSign(pp, out);
    if (*(uint16_t *)*pp != 0) {
        *pp = (uint64_t)((uint16_t *)*pp + 1);
    }
}

/* ========== PECMD_ParseNumSkipChar_01f8 @0x1400701f8 ==========
 * 解析数字后若非结束则前进一个字符。
 */
void PECMD_ParseNumSkipChar_01f8(int64_t *pp, int *out)
{
    PECMD_ParseUIntValue(pp, out);
    if (*(uint16_t *)*pp != 0) {
        *pp = (int64_t)((uint16_t *)*pp + 1);
    }
}

/* ========== PECMD_ParseNumSkipChar_0224 @0x140070224 ==========
 * 解析数字后若非结束则前进一个字符。
 */
void PECMD_ParseNumSkipChar_0224(int64_t *pp, int *out)
{
    PECMD_ParseIntRound(pp, out);
    if (*(uint16_t *)*pp != 0) {
        *pp = (int64_t)((uint16_t *)*pp + 1);
    }
}

/* ========== PECMD_ParseNumSkipChar_0248 @0x140070248 ==========
 * 解析数字后若非结束则前进一个字符。
 */
void PECMD_ParseNumSkipChar_0248(int64_t *pp, int64_t *out)
{
    PECMD_ParseInt64Round(pp, out);
    if (*(uint16_t *)*pp != 0) {
        *pp = (int64_t)((uint16_t *)*pp + 1);
    }
}

/* ========== PECMD_InstallDevice @0x140076b68 ==========
 * 设备安装入口 (带固定 GUID)。
 */
void PECMD_InstallDevice(LPCWSTR path, uint32_t flags)
{
    PECMD_EnumDevices(path, NULL, flags, (void *)g_bE870);
    GetLastError();
}

/* ========== PECMD_ParseAndSkipSpace_7f90 @0x140077f90 ==========
 * 解析数字后按需跳空白。
 */
uint64_t PECMD_ParseAndSkipSpace_7f90(int64_t *pp, double *out)
{
    uint64_t uVar1 = PECMD_ParseSizeWithUnit(pp, out);
    uint64_t uVar2 = uVar1 & 0xffffffff;
    if ((int)uVar1 != 0) {
        FUN_14005B154((WCHAR **)pp);
    }
    return uVar2 & 0xffffffff;
}

/* ========== PECMD_ParseNumSkipChar_de4c @0x14007de4c ==========
 * 解析数字后若非结束则前进一个字符。
 */
void PECMD_ParseNumSkipChar_de4c(int64_t *pp, double *out)
{
    PECMD_EvalExpressionTree(pp, out);
    if (*(uint16_t *)*pp != 0) {
        *pp = (int64_t)((uint16_t *)*pp + 1);
    }
}
