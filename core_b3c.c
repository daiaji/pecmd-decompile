/* ====================================================================
 * core_b3c.c — B3-B6 小函数批 3 (140040000-1400bffff)
 *
 * 来源: PECMD原始.EXE (x64, ImageBase=0x140000000)
 *   释放+free         PECMD_FreeAndFree @0x14004f8b4
 *   释放包装          PECMD_ReleaseWrap_4b88 @0x140054b88
 *   释放包装          PECMD_ReleaseWrap_75fc @0x1400575fc
 *   释放包装          PECMD_ReleaseWrap_7910 @0x140057910
 *   释放包装          PECMD_ReleaseWrap_86fc @0x1400586fc
 *   结构清零          PECMD_ZeroStruct_bc98 @0x14005bc98
 *   删除盘符          PECMD_DeleteDosDevice @0x14005d4b4
 *   颜色缓存          PECMD_CacheSysColors @0x14005d934
 *   托盘窗口查找      PECMD_FindTrayWindow @0x14005e97c
 *   特殊目录首字符    PECMD_GetSpecialDirFirstChar @0x14006042c
 *   任务计数          PECMD_IncTaskCount @0x140061230
 *   模式匹配          PECMD_MatchPattern @0x14006156c
 *   释放包装          PECMD_ReleaseWrap_c498 @0x14006c498
 *   释放包装          PECMD_ReleaseWrap_c6c8 @0x14006c6c8
 *   释放包装          PECMD_ReleaseWrap_03f0 @0x1400703f0
 *   释放包装          PECMD_ReleaseWrap_0420 @0x140070420
 *   释放包装          PECMD_ReleaseWrap_057c @0x14007057c
 *   释放包装          PECMD_ReleaseWrap_94f4 @0x1400794f4
 *   释放包装          PECMD_ReleaseWrap_96b4 @0x1400796b4
 *   释放包装          PECMD_ReleaseWrap_cf24 @0x14007cf24
 *   释放包装          PECMD_ReleaseWrap_cf54 @0x14007cf54
 *   释放包装          PECMD_ReleaseWrap_cf84 @0x14007cf84
 *   释放包装          PECMD_ReleaseWrap_4320 @0x1400a4320
 *   释放包装          PECMD_ReleaseWrap_9620 @0x1400a9620
 *   释放包装          PECMD_ReleaseWrap_9cb4 @0x1400a9cb4
 *   释放包装          PECMD_ReleaseWrap_a0fc @0x1400aa0fc
 *   释放包装          PECMD_ReleaseWrap_a364 @0x1400aa364
 *   释放包装          PECMD_ReleaseWrap_a50c @0x1400aa50c
 *   释放包装          PECMD_ReleaseWrap_ab68 @0x1400aab68
 *   释放包装          PECMD_ReleaseWrap_91d4 @0x1400b91d4
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
/* ---- 未实现依赖 (extern + TODO(verify)) ---- */
extern void PECMD_CleanupTaskEnvObject(int64_t obj);
extern void FUN_140054B18(void *obj);
extern void FUN_140054A50(void *obj);
extern void FUN_140054AB4(void *obj);
extern void PECMD_DestroyMappedObject(void *obj);
extern void FUN_1400E8940(void *obj);
extern void FUN_14006C648(void *obj);
extern void PECMD_DtorTrackbarControl(void *obj);
extern void FUN_14006C3CC(void *obj);
extern void PECMD_ReleaseComObject(void *obj);
extern void PECMD_ReleaseDeviceObj(void *obj);
extern void FUN_14006C430(void *obj);
extern void FUN_14006FC4C(void *obj);
extern void FUN_14006FCB4(void *obj);
extern void PECMD_ReleaseCtrlObject(void *obj);
extern void PECMD_DestroyWindowObject(void *obj);
extern void FUN_1400A9C40(void *obj);
extern void FUN_1400AA094(void *obj);
extern void FUN_1400AA2FC(void *obj);
extern void FUN_1400AA484(void *obj);
extern void PECMD_ReleaseControlObject(void *obj);
extern void FUN_1400B916C(void *obj);

/* ---- 本批引用的全局数据 ---- */
extern uint32_t g_sysColor8;
extern uint8_t g_u8D6F7;
extern int32_t g_i32D7DC;

/* ========== PECMD_FreeAndFree @0x14004f8b4 ==========
 * 释放对象并 free, 槽清零。
 */
void PECMD_FreeAndFree(uint64_t *slot)
{
    void *_Memory = (void *)*slot;
    if (_Memory != NULL) {
        PECMD_CleanupTaskEnvObject((int64_t)_Memory);
        free(_Memory);
    }
    *slot = 0;
}

/* ========== PECMD_ReleaseWrap_4b88 @0x140054b88 ==========
 * 释放包装: 清理后按位释放。
 */
uint64_t *PECMD_ReleaseWrap_4b88(uint64_t *obj, uint32_t flags)
{
    FUN_140054B18(obj);
    if ((flags & 1) != 0) {
        free(obj);
    }
    return obj;
}

/* ========== PECMD_ReleaseWrap_75fc @0x1400575fc ==========
 * 释放包装。
 */
uint64_t *PECMD_ReleaseWrap_75fc(uint64_t *obj, uint32_t flags)
{
    FUN_140054A50(obj);
    if ((flags & 1) != 0) {
        free(obj);
    }
    return obj;
}

/* ========== PECMD_ReleaseWrap_7910 @0x140057910 ==========
 * 释放包装。
 */
uint64_t *PECMD_ReleaseWrap_7910(uint64_t *obj, uint32_t flags)
{
    FUN_140054AB4(obj);
    if ((flags & 1) != 0) {
        free(obj);
    }
    return obj;
}

/* ========== PECMD_ReleaseWrap_86fc @0x1400586fc ==========
 * 释放包装。
 */
uint64_t *PECMD_ReleaseWrap_86fc(uint64_t *obj, uint32_t flags)
{
    PECMD_DestroyMappedObject(obj);
    if ((flags & 1) != 0) {
        free(obj);
    }
    return obj;
}

/* ========== PECMD_ZeroStruct_bc98 @0x14005bc98 ==========
 * 大结构关键槽清零。
 */
uint32_t *PECMD_ZeroStruct_bc98(uint32_t *obj)
{
    *(uint8_t *)((uint8_t *)obj + 0x101d) = 0;
    obj[0] = 0;
    obj[1] = 0;
    obj[2] = 0;
    *(uint8_t *)(obj + 3) = 0;
    obj[0x408] = 0;
    obj[0x409] = 0;
    *(uint64_t *)(obj + 0x350e) = 0;
    *(uint64_t *)(obj + 0x3510) = 0;
    return obj;
}

/* ========== PECMD_DeleteDosDevice @0x14005d4b4 ==========
 * 删除指定盘符的 DOS 设备。
 */
void PECMD_DeleteDosDevice(WCHAR drive)
{
    WCHAR local_res10[12];
    local_res10[1] = 0x3a;
    local_res10[2] = 0;
    local_res10[0] = drive;
    DefineDosDeviceW(2, local_res10, NULL);
}

/* ========== PECMD_CacheSysColors @0x14005d934 ==========
 * 缓存系统颜色。
 */
void PECMD_CacheSysColors(void)
{
    if ((int)g_helpVerFlag < 0) {
        g_helpVerFlag = (int32_t)GetSysColor(0xf);
        g_sysColor8 = GetSysColor(8);
    }
}

/* ========== PECMD_FindTrayWindow @0x14005e97c ==========
 * 查找托盘通知窗口。
 */
HWND PECMD_FindTrayWindow(void)
{
    HWND pHVar1 = FindWindowA("Shell_TrayWnd", NULL);
    return FindWindowExA(pHVar1, 0, "TrayNotifyWnd", NULL);
}

/* ========== PECMD_GetSpecialDirFirstChar @0x14006042c ==========
 * 取特殊目录路径并返回首字符。
 * TODO(verify): Ghidra CONCAT62 按返回首字符近似。
 */
uint64_t PECMD_GetSpecialDirFirstChar(void)
{
    WCHAR local_218[268];
    local_218[0] = L'\0';
    SHGetSpecialFolderPathW((HWND)0, local_218, 0x24, 0);
    return (uint64_t)(uint16_t)local_218[0];
}

/* ========== PECMD_IncTaskCount @0x140061230 ==========
 * 任务计数: 标志位为 1 时临界区自增。
 */
void PECMD_IncTaskCount(void)
{
    if ((g_u8D6F7 & 1) != 0) {
        EnterCriticalSection(&g_csInit);
        g_i32D7DC++;
        LeaveCriticalSection(&g_csInit);
    }
}

/* ========== PECMD_MatchPattern @0x14006156c ==========
 * 模式匹配: 连续 4 个字符匹配则 0。
 */
uint64_t PECMD_MatchPattern(int16_t *pattern, int64_t ctx)
{
    int64_t lVar1 = 0;
    int64_t lVar2 = ctx - (int64_t)pattern;
    while ((*pattern == *(int16_t *)(lVar2 + (int64_t)pattern)) ||
           (*pattern == *(int16_t *)(lVar2 + 8 + (int64_t)pattern))) {
        lVar1++;
        pattern++;
        if (3 < lVar1) {
            return 0;
        }
    }
    return 1;
}

/* ========== PECMD_ReleaseWrap_c498 @0x14006c498 ==========
 * 释放包装。
 */
uint64_t *PECMD_ReleaseWrap_c498(uint64_t *obj, uint32_t flags)
{
    FUN_1400E8940(obj);
    if ((flags & 1) != 0) {
        free(obj);
    }
    return obj;
}

/* ========== PECMD_ReleaseWrap_c6c8 @0x14006c6c8 ==========
 * 释放包装。
 */
uint64_t *PECMD_ReleaseWrap_c6c8(uint64_t *obj, uint32_t flags)
{
    FUN_14006C648(obj);
    if ((flags & 1) != 0) {
        free(obj);
    }
    return obj;
}

/* ========== PECMD_ReleaseWrap_03f0 @0x1400703f0 ==========
 * 释放包装。
 */
uint64_t *PECMD_ReleaseWrap_03f0(uint64_t *obj, uint32_t flags)
{
    PECMD_DtorTrackbarControl(obj);
    if ((flags & 1) != 0) {
        free(obj);
    }
    return obj;
}

/* ========== PECMD_ReleaseWrap_0420 @0x140070420 ==========
 * 释放包装。
 */
uint64_t *PECMD_ReleaseWrap_0420(uint64_t *obj, uint32_t flags)
{
    FUN_14006C3CC(obj);
    if ((flags & 1) != 0) {
        free(obj);
    }
    return obj;
}

/* ========== PECMD_ReleaseWrap_057c @0x14007057c ==========
 * 释放包装。
 */
uint64_t *PECMD_ReleaseWrap_057c(uint64_t *obj, uint32_t flags)
{
    PECMD_ReleaseComObject(obj);
    if ((flags & 1) != 0) {
        free(obj);
    }
    return obj;
}

/* ========== PECMD_ReleaseWrap_94f4 @0x1400794f4 ==========
 * 释放包装。
 */
uint64_t *PECMD_ReleaseWrap_94f4(uint64_t *obj, uint32_t flags)
{
    PECMD_ReleaseDeviceObj(obj);
    if ((flags & 1) != 0) {
        free(obj);
    }
    return obj;
}

/* ========== PECMD_ReleaseWrap_96b4 @0x1400796b4 ==========
 * 释放包装。
 */
uint64_t *PECMD_ReleaseWrap_96b4(uint64_t *obj, uint32_t flags)
{
    FUN_14006C430(obj);
    if ((flags & 1) != 0) {
        free(obj);
    }
    return obj;
}

/* ========== PECMD_ReleaseWrap_cf24 @0x14007cf24 ==========
 * 释放包装。
 */
uint64_t *PECMD_ReleaseWrap_cf24(uint64_t *obj, uint32_t flags)
{
    FUN_14006FC4C(obj);
    if ((flags & 1) != 0) {
        free(obj);
    }
    return obj;
}

/* ========== PECMD_ReleaseWrap_cf54 @0x14007cf54 ==========
 * 释放包装。
 */
uint64_t *PECMD_ReleaseWrap_cf54(uint64_t *obj, uint32_t flags)
{
    FUN_14006FCB4(obj);
    if ((flags & 1) != 0) {
        free(obj);
    }
    return obj;
}

/* ========== PECMD_ReleaseWrap_cf84 @0x14007cf84 ==========
 * 释放包装。
 */
uint64_t *PECMD_ReleaseWrap_cf84(uint64_t *obj, uint32_t flags)
{
    PECMD_ReleaseCtrlObject(obj);
    if ((flags & 1) != 0) {
        free(obj);
    }
    return obj;
}

/* ========== PECMD_ReleaseWrap_4320 @0x1400a4320 ==========
 * 释放互斥体并调用对象释放函数。
 */
void PECMD_ReleaseWrap_4320(uint64_t *obj)
{
    if (*(char *)((uint8_t *)obj + 16) != '\0') {
        ReleaseMutex((HANDLE)obj[1]);
    }
    *(uint8_t *)((uint8_t *)obj + 16) = 0;
    ((void (*)(void *, int))(uintptr_t)*obj)(obj, 1);
}

/* ========== PECMD_ReleaseWrap_9620 @0x1400a9620 ==========
 * 释放包装。
 */
uint64_t *PECMD_ReleaseWrap_9620(uint64_t *obj, uint32_t flags)
{
    PECMD_DestroyWindowObject(obj);
    if ((flags & 1) != 0) {
        free(obj);
    }
    return obj;
}

/* ========== PECMD_ReleaseWrap_9cb4 @0x1400a9cb4 ==========
 * 释放包装。
 */
uint64_t *PECMD_ReleaseWrap_9cb4(uint64_t *obj, uint32_t flags)
{
    FUN_1400A9C40(obj);
    if ((flags & 1) != 0) {
        free(obj);
    }
    return obj;
}

/* ========== PECMD_ReleaseWrap_a0fc @0x1400aa0fc ==========
 * 释放包装。
 */
uint64_t *PECMD_ReleaseWrap_a0fc(uint64_t *obj, uint32_t flags)
{
    FUN_1400AA094(obj);
    if ((flags & 1) != 0) {
        free(obj);
    }
    return obj;
}

/* ========== PECMD_ReleaseWrap_a364 @0x1400aa364 ==========
 * 释放包装。
 */
uint64_t *PECMD_ReleaseWrap_a364(uint64_t *obj, uint32_t flags)
{
    FUN_1400AA2FC(obj);
    if ((flags & 1) != 0) {
        free(obj);
    }
    return obj;
}

/* ========== PECMD_ReleaseWrap_a50c @0x1400aa50c ==========
 * 释放包装。
 */
uint64_t *PECMD_ReleaseWrap_a50c(uint64_t *obj, uint32_t flags)
{
    FUN_1400AA484(obj);
    if ((flags & 1) != 0) {
        free(obj);
    }
    return obj;
}

/* ========== PECMD_ReleaseWrap_ab68 @0x1400aab68 ==========
 * 释放包装。
 */
uint64_t *PECMD_ReleaseWrap_ab68(uint64_t *obj, uint32_t flags)
{
    PECMD_ReleaseControlObject(obj);
    if ((flags & 1) != 0) {
        free(obj);
    }
    return obj;
}

/* ========== PECMD_ReleaseWrap_91d4 @0x1400b91d4 ==========
 * 释放包装。
 */
uint64_t *PECMD_ReleaseWrap_91d4(uint64_t *obj, uint32_t flags)
{
    FUN_1400B916C(obj);
    if ((flags & 1) != 0) {
        free(obj);
    }
    return obj;
}
