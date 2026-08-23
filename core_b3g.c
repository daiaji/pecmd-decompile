/* ====================================================================
 * core_b3g.c — B3-B6 小函数批 7 (140040000-1400bffff)
 *
 * 本批新实现函数全部使用人类可读 PECMD_ 名称，原始地址保留在 @0x 注释。
 *
 * 来源: PECMD原始.EXE (x64, ImageBase=0x140000000)
 *   调整缓冲         FUN_14004FD6C @0x14004fd6c
 *   调整缓冲2        FUN_14004FDD0 @0x14004fdd0
 *   数组压入         FUN_14004FEA4 @0x14004fea4
 *   发送 RGB 消息    FUN_1400540A8 @0x1400540a8
 *   释放对象 A       FUN_140054A50 @0x140054a50
 *   释放对象 B       FUN_140054AB4 @0x140054ab4
 *   获取过程地址     FUN_14005C898 @0x14005c898
 *   执行特殊命令     FUN_14005CC4C @0x14005cc4c
 *   初始化标记       FUN_14005D2A4 @0x14005d2a4
 *   查找可用盘符     PECMD_FindFirstFreeDrive @0x14005f904
 *   创建符号链接     PECMD_CreateSymbolicLink @0x140060600
 *   100ns 时间       PECMD_GetTime100ns @0x140060f68
 *   递减任务计数     PECMD_DecrementInitRef @0x140061264
 *   释放句柄数组     FUN_14006306C @0x14006306c
 *   数组增长         FUN_140063B00 @0x140063b00
 *   复制到字符       PECMD_CopyUpToChar @0x140067748
 *   十六进制转字节   PECMD_HexStringToBytes @0x14006ad9c
 *   字符串转 ID      FUN_14006B1E8 @0x14006b1e8
 *   追加引号串       FUN_14006B684 @0x14006b684
 *   释放对象 C       PECMD_DtorTrackbarControl @0x14006bf8c
 *   释放对象 D       FUN_14006C3CC @0x14006c3cc
 *   释放对象 E       FUN_14006C430 @0x14006c430
 *   追加 I64         FUN_14006CC70 @0x14006cc70
 *   释放对象 F       FUN_14006FC4C @0x14006fc4c
 *   释放对象 G       FUN_14006FCB4 @0x14006fcb4
 *   释放 GDI 对象    FUN_14006FFDC @0x14006ffdc
 *   压入字符串 token FUN_1400738D0 @0x1400738d0
 *   释放对象 H       FUN_1400AA094 @0x1400aa094
 *   释放对象 I       FUN_1400AA2FC @0x1400aa2fc
 *   释放对象 J       FUN_1400B916C @0x1400b916c
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
extern uint64_t PECMD_ReserveVec8(int64_t *a1, uint64_t a2);
extern uint64_t PECMD_ReserveObjectArray(int64_t *a1, uint64_t a2);
extern void FUN_1400F1490(int64_t a1);
extern void FUN_14005C828(LPCSTR a1, LPCSTR a2, int64_t *a3,
                          int64_t *a4);
extern uint64_t PECMD_ServiceControl(int64_t *a1, LPCWSTR a2);
extern DWORD FUN_1400195F0(uint64_t a1, int64_t a2, int a3,
                           uint64_t *a4);
extern bool FUN_140101E70(LPCWSTR a1);
extern uint16_t *PECMD_Base64Encode(uint64_t *a1, uint64_t *a2, char a3);
extern LPCWSTR FUN_1400E6D38(LPCWSTR a1, uint64_t a2, LPCWSTR a3);
extern void PECMD_ArrayGrowRaw(uint64_t *a1, int64_t *a2, uint32_t a3,
                          int64_t a4);
extern void *PECMD_AllocSmallObject(void *a1);
extern int64_t *PECMD_WideToAnsiStr(int64_t *a1, LPCWSTR a2, int64_t a3,
                              uint64_t a4);
extern void AtlThrowImpl(long a1);

/* ---- 本批引用的全局数据 ---- */
extern uint8_t PTR_FUN_1401234f0[];
extern uint8_t PTR_FUN_140123540[];
extern uint8_t PTR_FUN_140123560[];
extern uint8_t PTR_FUN_1401266c0[];
extern uint8_t PTR_FUN_1401266e8[];
extern uint8_t PTR_FUN_140126708[];
extern uint8_t PTR_FUN_140126888[];
extern uint8_t PTR_FUN_1401268a8[];
extern uint8_t PTR_FUN_140129060[];
extern uint8_t PTR_FUN_140129080[];
extern uint8_t PTR_FUN_1401294f0[];
extern uint64_t g_u6453d8;
extern uint32_t g_u3253e0;
extern uint8_t g_flagD3EF;
extern void *g_pCreateSymbolicLinkW;          /* CreateSymbolicLinkW 函数指针 */
extern uint8_t g_u8D6F7;
extern int32_t g_i32D7DC;
extern int (*g_pGdipDisposeImage)();          /* 函数指针 */
extern int (*g_pGdipDeleteGraphics)();          /* 函数指针 */

/* ========== FUN_14004FD6C @0x14004fd6c ==========
 * 调整缓冲容量。
 */
uint64_t FUN_14004FD6C(int64_t *buf, uint64_t size, int mode)
{
    uint64_t in_RAX = 1;
    if (mode != -1) {
        *(int *)(buf + 3) = mode;
    }
    if (size == 0) {
        if ((void *)*buf != NULL) {
            free((void *)*buf);
            *buf = 0;
            in_RAX = 1;
        }
        buf[1] = 0;
        buf[2] = 0;
    } else if ((uint64_t)buf[2] < size) {
        in_RAX = PECMD_ReserveVec8(buf, size);
        if ((char)in_RAX == '\0') {
            return in_RAX;
        }
        buf[1] = (int64_t)size;
    } else {
        buf[1] = (int64_t)size;
    }
    return 1;
}

/* ========== FUN_14004FDD0 @0x14004fdd0 ==========
 * 调整缓冲容量（另一分配器）。
 */
uint64_t FUN_14004FDD0(int64_t *buf, uint64_t size, int mode)
{
    uint64_t in_RAX = 1;
    if (mode != -1) {
        *(int *)(buf + 3) = mode;
    }
    if (size == 0) {
        if ((void *)*buf != NULL) {
            free((void *)*buf);
            *buf = 0;
            in_RAX = 1;
        }
        buf[1] = 0;
        buf[2] = 0;
    } else if ((uint64_t)buf[2] < size) {
        in_RAX = PECMD_ReserveObjectArray(buf, size);
        if ((char)in_RAX == '\0') {
            return in_RAX;
        }
        buf[1] = (int64_t)size;
    } else {
        buf[1] = (int64_t)size;
    }
    return 1;
}

/* ========== FUN_14004FEA4 @0x14004fea4 ==========
 * 向动态数组压入元素。
 */
uint64_t FUN_14004FEA4(int64_t arr, uint64_t value)
{
    *(int64_t *)(arr + 0x48) = *(int64_t *)(arr + 0x48) + 1;
    if ((*(uint64_t *)(arr + 0x30) <= (*(uint64_t *)(arr + 0x48) & 0xffffffff)) &&
        (FUN_14004FD6C((int64_t *)(arr + 0x28),
                            *(uint64_t *)(arr + 0x48) * 2 + 2, -1) == 0)) {
        *(int64_t *)(arr + 0x48) = *(int64_t *)(arr + 0x48) - 1;
        return 0;
    }
    if (*(uint64_t *)(arr + 0x48) < *(uint64_t *)(arr + 0x30)) {
        *(uint64_t *)(*(int64_t *)(arr + 0x28) + *(uint64_t *)(arr + 0x48) * 8) = value;
        return 1;
    }
    AtlThrowImpl(-0x7ff8ffa9);
    return 0;
}

/* ========== FUN_1400540A8 @0x1400540a8 ==========
 * 发送 RGB 颜色消息。
 */
uint64_t FUN_1400540A8(int64_t obj, uint32_t c1, uint32_t c2,
                              uint32_t c3, uint32_t c4)
{
    if ((((int)c1 < 0) || ((int)c2 < 0)) || ((int)c3 < 0) ||
        ((int)c4 < 0)) {
        return 0x80070057;
    }
    SendMessageW(*(HWND *)(*(int64_t *)(obj + 0x38) + OBJ_HWND), 0x465, 0,
                 (uint64_t)((c4 & 0xff) +
                 (((c1 & 0xff) * 0x100 + (c2 & 0xff)) * 0x100 + (c3 & 0xff)) * 0x100));
    return 0;
}

/* ========== FUN_140054A50 @0x140054a50 ==========
 * 释放对象（vtable 140123540）。
 */
void FUN_140054A50(uint64_t *obj)
{
    *obj = (uint64_t)(uintptr_t)PTR_FUN_140123540;
    int64_t *plVar1 = (int64_t *)obj[7];
    if (plVar1 != NULL) {
        void (**fn)(void *, int) = (void (**)(void *, int))(*plVar1 + 0x18);
        (**fn)(plVar1, 1);
    }
    *obj = (uint64_t)(uintptr_t)PTR_FUN_1401234f0;
    FUN_1400F1490((int64_t)(obj + 0xb));
    PECMD_FreeStrBuf((WCHAR **)(obj + 5));
    PECMD_FreeStrBuf((WCHAR **)(obj + 4));
    PECMD_FreeStrBuf((WCHAR **)(obj + 3));
    PECMD_FreeStrBuf((WCHAR **)(obj + 2));
}

/* ========== FUN_140054AB4 @0x140054ab4 ==========
 * 释放对象（vtable 140123560）。
 */
void FUN_140054AB4(uint64_t *obj)
{
    *obj = (uint64_t)(uintptr_t)PTR_FUN_140123560;
    int64_t *plVar1 = (int64_t *)obj[7];
    if (plVar1 != NULL) {
        void (**fn)(void *, int) = (void (**)(void *, int))(*plVar1 + 0x18);
        (**fn)(plVar1, 1);
    }
    *obj = (uint64_t)(uintptr_t)PTR_FUN_1401234f0;
    FUN_1400F1490((int64_t)(obj + 0xb));
    PECMD_FreeStrBuf((WCHAR **)(obj + 5));
    PECMD_FreeStrBuf((WCHAR **)(obj + 4));
    PECMD_FreeStrBuf((WCHAR **)(obj + 3));
    PECMD_FreeStrBuf((WCHAR **)(obj + 2));
}

/* ========== FUN_14005C898 @0x14005c898 ==========
 * 延迟加载 GetProcAddress。
 */
void FUN_14005C898(LPCSTR name, LPCSTR dll, int64_t *out,
                         int64_t *hmod)
{
    int64_t local_res18[2] = {0, 0};
    if (*out != 0) {
        return;
    }
    int64_t *plVar2 = local_res18;
    if (hmod != NULL) {
        plVar2 = hmod;
    }
    HMODULE hModule = (HMODULE)*plVar2;
    void *pFVar1;
    if (hModule == (HMODULE)0) {
        hModule = LoadLibraryA(dll);
        *plVar2 = (int64_t)hModule;
        if (hModule == (HMODULE)0) {
            pFVar1 = (void *)(intptr_t)-1;
            *out = (int64_t)pFVar1;
            return;
        }
    }
    pFVar1 = GetProcAddress(hModule, name);
    *out = (int64_t)(intptr_t)pFVar1;
}

/* ========== FUN_14005CC4C @0x14005cc4c ==========
 * 执行特殊命令。
 */
void FUN_14005CC4C(void)
{
    uint64_t local_18 = g_u6453d8;
    uint32_t local_10 = g_u3253e0;
    (void)local_10;
    uint64_t r = PECMD_ServiceControl((int64_t *)g_Script, (LPCWSTR)&local_18);
    if (r != 0x426) {
        FUN_1400195F0((uint64_t)(uintptr_t)g_Script, 1000, 0, NULL);
    }
    PECMD_ServiceControl((int64_t *)g_Script, (LPCWSTR)((uint8_t *)&local_18 + 2));
}

/* ========== FUN_14005D2A4 @0x14005d2a4 ==========
 * 一次性初始化。
 */
uint64_t FUN_14005D2A4(uint64_t value)
{
    uint8_t local_1a8[416];
    if (g_flagD3EF == 0) {
        g_flagD3EF = 0;
        int iVar1 = ((int (*)(int, void *))g_pWSAStartup)(0x202, local_1a8);
        GetLastError();
        if (iVar1 == 0) {
            g_flagD3EF = 1;
        }
    }
    return value;
}

/* ========== PECMD_FindFirstFreeDrive @0x14005f904 ==========
 * 查找第一个可用盘符。
 */
uint64_t PECMD_FindFirstFreeDrive(LPCWSTR buf)
{
    DWORD DVar2 = GetLogicalDrives();
    uint64_t uVar3 = DVar2;
    uint64_t uVar4 = DVar2;
    uint8_t bVar5 = 0x41;
    do {
        if (DVar2 == 0) {
            return uVar3 & 0xffffffffffff0000ULL;
        }
        if ((uVar4 & 1) != 0) {
            *(uint16_t *)buf = (uint16_t)(char)bVar5;
            bool bVar1 = FUN_140101E70(buf);
            if (bVar1) {
                return (uint64_t)bVar5;
            }
        }
        uVar4 >>= 1;
        bVar5++;
        DVar2 = (DWORD)uVar4;
    } while (1);
}

/* ========== PECMD_CreateSymbolicLink @0x140060600 ==========
 * 创建符号链接（延迟加载）。
 */
uint64_t PECMD_CreateSymbolicLink(uint64_t src, uint64_t dst, uint32_t flags)
{
    FUN_14005C828("CreateSymbolicLinkW", "KERNEL32.DLL", (int64_t *)(void **)&g_pCreateSymbolicLinkW, NULL);
    if (g_pCreateSymbolicLinkW == NULL) {
        return 0;
    }
    return ((uint64_t (*)(uint64_t, uint64_t, uint32_t))g_pCreateSymbolicLinkW)(src, dst, flags);
}

/* ========== PECMD_GetTime100ns @0x140060f68 ==========
 * 获取 100ns 时间。
 */
void PECMD_GetTime100ns(int64_t *out)
{
    int64_t counter;
    QueryPerformanceCounter(&counter);
    uint64_t q = (uint64_t)(counter - g_QPC);
    uint64_t freq = (uint64_t)g_QPFreq;
    uint64_t uVar1 = q / freq;
    *out = (int64_t)(uVar1 * 10000000ULL + g_StartTime +
             ((q - uVar1 * freq) * 10000000ULL) / freq);
}

/* ========== PECMD_DecrementInitRef @0x140061264 ==========
 * 递减任务计数，必要时退出进程。
 */
void PECMD_DecrementInitRef(uint8_t force)
{
    if ((g_u8D6F7 & 1) != 0) {
        EnterCriticalSection(&g_csInit);
        g_i32D7DC--;
        if ((((g_u8D6F7 | force) & 0xfe) != 0) &&
            (g_u8D6F7 = (uint8_t)(g_u8D6F7 | 0x10), g_i32D7DC < 1)) {
            ExitProcess(0);
        }
        LeaveCriticalSection(&g_csInit);
    }
}

/* ========== FUN_14006306C @0x14006306c ==========
 * 释放句柄数组。
 */
void FUN_14006306C(int64_t *arr)
{
    int64_t lVar2 = arr[2];
    int64_t lVar1 = *arr;
    arr[2] = 0;
    while (0 < lVar2) {
        lVar2--;
        uint64_t *_Memory = *(uint64_t **)(lVar1 + lVar2 * 8);
        if (_Memory != NULL) {
            if ((HANDLE)*_Memory != 0) {
                CloseHandle((HANDLE)*_Memory);
            }
            PECMD_FreeStrBuf((WCHAR **)(_Memory + 1));
            free(_Memory);
            *(uint64_t *)(lVar1 + lVar2 * 8) = 0;
        }
    }
}

/* ========== FUN_140063B00 @0x140063b00 ==========
 * 数组增长并返回新位置。
 */
int64_t FUN_140063B00(int64_t idx, int64_t *arr, int64_t *cap,
                        uint32_t esize)
{
    if (idx < 0) {
        idx = 0;
    }
    if (*cap - 1 <= idx) {
        PECMD_ArrayGrowRaw((uint64_t *)arr, cap, esize, (idx - *cap) + 2);
    }
    return (int64_t)((uint64_t)esize * (uint64_t)idx + (uint64_t)*arr);
}

/* ========== PECMD_CopyUpToChar @0x140067748 ==========
 * 复制到指定字符前。
 */
void PECMD_CopyUpToChar(int64_t *pp, int64_t *out, int16_t sep)
{
    FUN_14005B154((WCHAR **)pp);
    LPCWSTR pWVar2 = (LPCWSTR)*pp;
    WCHAR WVar1 = *pWVar2;
    while ((WVar1 != L'\0' && (*(int16_t *)*pp != sep))) {
        int16_t *pWVar4 = (int16_t *)*pp + 1;
        *pp = (int64_t)pWVar4;
        WVar1 = *pWVar4;
    }
    int iVar3 = (int)((*pp - (int64_t)pWVar2) / 2);
    PECMD_StrCopyW((WCHAR **)out, pWVar2, (int64_t)(iVar3 + 1));
    *(uint16_t *)(*out + (int64_t)iVar3 * 2) = 0;
}

/* ========== PECMD_HexStringToBytes @0x14006ad9c ==========
 * 十六进制字符串转字节数组。
 */
void PECMD_HexStringToBytes(uint8_t *src, uint8_t *dst)
{
    uint8_t bVar1 = *src;
    do {
        if (bVar1 == 0) {
            return;
        }
        if (bVar1 == 0x2d) {
            src++;
        }
        bVar1 = *src;
        if (9 < (uint8_t)(bVar1 - 0x30)) {
            if (5 < (uint8_t)((bVar1 | 0x20) + 0x9f)) {
                return;
            }
            bVar1 = (uint8_t)(bVar1 - 7);
        }
        uint8_t bVar2 = src[1];
        if (9 < (uint8_t)(bVar2 - 0x30)) {
            if (5 < (uint8_t)((bVar2 | 0x20) + 0x9f)) {
                return;
            }
            bVar2 = (uint8_t)(bVar2 - 7);
        }
        src += 2;
        *dst = (uint8_t)((bVar2 & 0xf) | (bVar1 << 4));
        bVar1 = *src;
        dst++;
    } while (1);
}

/* ========== FUN_14006B1E8 @0x14006b1e8 ==========
 * 字符串转 ID。
 */
uint16_t *FUN_14006B1E8(LPCWSTR name, uint64_t *out, char mode)
{
    uint64_t *local_res20 = NULL;
    PECMD_AllocSmallObject(&local_res20);
    PECMD_WideToAnsiStr((int64_t *)&local_res20, name, -1, 0xfde9);
    uint16_t *puVar1 = PECMD_Base64Encode(local_res20, out, mode);
    PECMD_FreeStrBuf((WCHAR **)&local_res20);
    return puVar1;
}

/* ========== FUN_14006B684 @0x14006b684 ==========
 * 追加带引号字符串。
 */
void FUN_14006B684(int64_t *pp, uint8_t *text, int len)
{
    *(uint16_t *)*pp = 0x22;
    *pp += 2;
    memcpy((void *)*pp, text, (int)((int64_t)len * 2));
    *pp += (int64_t)len * 2;
    *(uint16_t *)*pp = 0x22;
    *pp += 2;
    *(uint16_t *)*pp = 0x20;
    *pp += 2;
    *(uint16_t *)*pp = 0;
}

/* ========== PECMD_DtorTrackbarControl @0x14006bf8c ==========
 * 释放对象（vtable 1401266c0）。
 */
void PECMD_DtorTrackbarControl(uint64_t *obj)
{
    *obj = (uint64_t)(uintptr_t)PTR_FUN_1401266c0;
    int64_t *plVar1 = (int64_t *)obj[7];
    if (plVar1 != NULL) {
        void (**fn)(void *, int) = (void (**)(void *, int))(*plVar1 + 0x18);
        (**fn)(plVar1, 1);
    }
    *obj = (uint64_t)(uintptr_t)PTR_FUN_1401234f0;
    FUN_1400F1490((int64_t)(obj + 0xb));
    PECMD_FreeStrBuf((WCHAR **)(obj + 5));
    PECMD_FreeStrBuf((WCHAR **)(obj + 4));
    PECMD_FreeStrBuf((WCHAR **)(obj + 3));
    PECMD_FreeStrBuf((WCHAR **)(obj + 2));
}

/* ========== FUN_14006C3CC @0x14006c3cc ==========
 * 释放对象（vtable 1401266e8）。
 */
void FUN_14006C3CC(uint64_t *obj)
{
    *obj = (uint64_t)(uintptr_t)PTR_FUN_1401266e8;
    int64_t *plVar1 = (int64_t *)obj[7];
    if (plVar1 != NULL) {
        void (**fn)(void *, int) = (void (**)(void *, int))(*plVar1 + 0x18);
        (**fn)(plVar1, 1);
    }
    *obj = (uint64_t)(uintptr_t)PTR_FUN_1401234f0;
    FUN_1400F1490((int64_t)(obj + 0xb));
    PECMD_FreeStrBuf((WCHAR **)(obj + 5));
    PECMD_FreeStrBuf((WCHAR **)(obj + 4));
    PECMD_FreeStrBuf((WCHAR **)(obj + 3));
    PECMD_FreeStrBuf((WCHAR **)(obj + 2));
}

/* ========== FUN_14006C430 @0x14006c430 ==========
 * 释放对象（vtable 140126708）。
 */
void FUN_14006C430(uint64_t *obj)
{
    *obj = (uint64_t)(uintptr_t)PTR_FUN_140126708;
    int64_t *plVar1 = (int64_t *)obj[7];
    if (plVar1 != NULL) {
        void (**fn)(void *, int) = (void (**)(void *, int))(*plVar1 + 0x18);
        (**fn)(plVar1, 1);
    }
    obj[7] = 0;
    *obj = (uint64_t)(uintptr_t)PTR_FUN_1401234f0;
    FUN_1400F1490((int64_t)(obj + 0xb));
    PECMD_FreeStrBuf((WCHAR **)(obj + 5));
    PECMD_FreeStrBuf((WCHAR **)(obj + 4));
    PECMD_FreeStrBuf((WCHAR **)(obj + 3));
    PECMD_FreeStrBuf((WCHAR **)(obj + 2));
}

/* ========== FUN_14006CC70 @0x14006cc70 ==========
 * 追加 I64 格式化串。
 */
void FUN_14006CC70(int64_t *pp, uint64_t value)
{
    WCHAR local_88[64];
    int16_t sVar1 = *(int16_t *)*pp;
    FUN_1400E6D38(local_88, value, WSTR(" %I64d"));
    int iVar2 = lstrlenW(local_88 + (sVar1 == 0));
    pp[2] += iVar2;
    pp[1] += iVar2;
    FUN_14006375C((WCHAR **)pp, local_88 + (sVar1 == 0));
}

/* ========== FUN_14006FC4C @0x14006fc4c ==========
 * 释放对象（vtable 140126888）。
 */
void FUN_14006FC4C(uint64_t *obj)
{
    *obj = (uint64_t)(uintptr_t)PTR_FUN_140126888;
    int64_t *plVar1 = (int64_t *)obj[7];
    if (plVar1 != NULL) {
        void (**fn)(void *, int) = (void (**)(void *, int))(*plVar1 + 0x18);
        (**fn)(plVar1, 1);
    }
    obj[7] = 0;
    *obj = (uint64_t)(uintptr_t)PTR_FUN_1401234f0;
    FUN_1400F1490((int64_t)(obj + 0xb));
    PECMD_FreeStrBuf((WCHAR **)(obj + 5));
    PECMD_FreeStrBuf((WCHAR **)(obj + 4));
    PECMD_FreeStrBuf((WCHAR **)(obj + 3));
    PECMD_FreeStrBuf((WCHAR **)(obj + 2));
}

/* ========== FUN_14006FCB4 @0x14006fcb4 ==========
 * 释放对象（vtable 1401268a8）。
 */
void FUN_14006FCB4(uint64_t *obj)
{
    *obj = (uint64_t)(uintptr_t)PTR_FUN_1401268a8;
    int64_t *plVar1 = (int64_t *)obj[7];
    if (plVar1 != NULL) {
        void (**fn)(void *, int) = (void (**)(void *, int))(*plVar1 + 0x18);
        (**fn)(plVar1, 1);
    }
    obj[7] = 0;
    *obj = (uint64_t)(uintptr_t)PTR_FUN_1401234f0;
    FUN_1400F1490((int64_t)(obj + 0xb));
    PECMD_FreeStrBuf((WCHAR **)(obj + 5));
    PECMD_FreeStrBuf((WCHAR **)(obj + 4));
    PECMD_FreeStrBuf((WCHAR **)(obj + 3));
    PECMD_FreeStrBuf((WCHAR **)(obj + 2));
}

/* ========== FUN_14006FFDC @0x14006ffdc ==========
 * 释放 GDI 相关对象。
 */
void FUN_14006FFDC(int64_t *obj)
{
    if (obj[7] != 0) {
        ((void (*)(void))g_pGdipDisposeImage)();
        obj[7] = 0;
    }
    if ((HGLOBAL)obj[8] != (HGLOBAL)0) {
        GlobalFree((HGLOBAL)obj[8]);
        obj[8] = 0;
    }
    if (obj[10] != 0) {
        ((void (*)(void))g_pGdipDeleteGraphics)();
        obj[10] = 0;
    }
    if ((HDC)obj[9] != (HDC)0) {
        DeleteDC((HDC)obj[9]);
        obj[9] = 0;
    }
    PECMD_FreeStrBuf((WCHAR **)obj);
}

/* ========== FUN_1400738D0 @0x1400738d0 ==========
 * 压入字符串 token。
 */
uint64_t FUN_1400738D0(uint64_t a, uint64_t b, LPCWSTR text,
                               uint64_t *ctrl)
{
    (void)a;
    (void)b;
    int64_t *plVar1 = (int64_t *)*ctrl;
    if (text < (LPCWSTR)0x10000) {
        ctrl[3] = (uint64_t)text;
    }
    if ((int64_t)ctrl[1] < 1) {
        if (plVar1 != NULL) {
            if (text < (LPCWSTR)0x10000) {
                ctrl[2] = (uint64_t)text;
            } else {
                PECMD_StrBldCopyWide(plVar1, text);
                ctrl[2] = (uint64_t)*plVar1;
            }
        }
        return 0;
    }
    ctrl[1]--;
    return 1;
}

/* ========== FUN_1400AA094 @0x1400aa094 ==========
 * 释放对象（vtable 140129060）。
 */
void FUN_1400AA094(uint64_t *obj)
{
    *obj = (uint64_t)(uintptr_t)PTR_FUN_140129060;
    int64_t *plVar1 = (int64_t *)obj[7];
    if (plVar1 != NULL) {
        void (**fn)(void *, int) = (void (**)(void *, int))(*plVar1 + 0x18);
        (**fn)(plVar1, 1);
    }
    obj[7] = 0;
    *obj = (uint64_t)(uintptr_t)PTR_FUN_1401234f0;
    FUN_1400F1490((int64_t)(obj + 0xb));
    PECMD_FreeStrBuf((WCHAR **)(obj + 5));
    PECMD_FreeStrBuf((WCHAR **)(obj + 4));
    PECMD_FreeStrBuf((WCHAR **)(obj + 3));
    PECMD_FreeStrBuf((WCHAR **)(obj + 2));
}

/* ========== FUN_1400AA2FC @0x1400aa2fc ==========
 * 释放对象（vtable 140129080）。
 */
void FUN_1400AA2FC(uint64_t *obj)
{
    *obj = (uint64_t)(uintptr_t)PTR_FUN_140129080;
    int64_t *plVar1 = (int64_t *)obj[7];
    if (plVar1 != NULL) {
        void (**fn)(void *, int) = (void (**)(void *, int))(*plVar1 + 0x18);
        (**fn)(plVar1, 1);
    }
    obj[7] = 0;
    *obj = (uint64_t)(uintptr_t)PTR_FUN_1401234f0;
    FUN_1400F1490((int64_t)(obj + 0xb));
    PECMD_FreeStrBuf((WCHAR **)(obj + 5));
    PECMD_FreeStrBuf((WCHAR **)(obj + 4));
    PECMD_FreeStrBuf((WCHAR **)(obj + 3));
    PECMD_FreeStrBuf((WCHAR **)(obj + 2));
}

/* ========== FUN_1400B916C @0x1400b916c ==========
 * 释放对象（vtable 1401294f0）。
 */
void FUN_1400B916C(uint64_t *obj)
{
    *obj = (uint64_t)(uintptr_t)PTR_FUN_1401294f0;
    int64_t *plVar1 = (int64_t *)obj[7];
    if (plVar1 != NULL) {
        void (**fn)(void *, int) = (void (**)(void *, int))(*plVar1 + 0x18);
        (**fn)(plVar1, 1);
    }
    obj[7] = 0;
    *obj = (uint64_t)(uintptr_t)PTR_FUN_1401234f0;
    FUN_1400F1490((int64_t)(obj + 0xb));
    PECMD_FreeStrBuf((WCHAR **)(obj + 5));
    PECMD_FreeStrBuf((WCHAR **)(obj + 4));
    PECMD_FreeStrBuf((WCHAR **)(obj + 3));
    PECMD_FreeStrBuf((WCHAR **)(obj + 2));
}
