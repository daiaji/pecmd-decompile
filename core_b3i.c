/* ====================================================================
 * core_b3i.c — B3-B6 小函数批 8 (140040000-1400bffff)
 *
 * 本批新实现函数全部使用人类可读 PECMD_ 名称，原始地址保留在 @0x 注释。
 *
 * 来源: PECMD原始.EXE (x64, ImageBase=0x140000000)
 *   PATH 命令         PECMD_DispatchSpecialDirective @0x140040350
 *   解析范围说明       FUN_14004FAA8 @0x14004faa8
 *   解析字符类         FUN_1400513B8 @0x1400513b8
 *   设置 Enable 状态   FUN_140053C5C @0x140053c5c
 *   追加分隔 token     FUN_1400547BC @0x1400547bc
 *   释放全局缓存       FUN_14005CF3C @0x14005cf3c
 *   发送 UDP 包        FUN_14005D30C @0x14005d30c
 *   释放 GDI 资源      FUN_14005D558 @0x14005d558
 *   初始化 LogPixels   FUN_14005D770 @0x14005d770
 *   复制图像           FUN_14005D8BC @0x14005d8bc
 *   获取扇区大小       FUN_14005F96C @0x14005f96c
 *   获取设备信息       FUN_14005FEAC @0x14005feac
 *   启用备份特权       PECMD_EnableBackupPrivileges @0x140060698
 *   设置剪贴板文本     PECMD_SetClipboardUnicode @0x140060718
 *   去除引号           PECMD_TrimOuterQuotes @0x140060d7c
 *   计算条目行数       FUN_140060EE0 @0x140060ee0
 *   释放对象数组       PECMD_TruncateObjectArray @0x140061704
 *   设置 ANSI 串       FUN_1400634D4 @0x1400634d4
 *   数组追加元素       FUN_1400639F0 @0x1400639f0
 *   初始化数组         FUN_140063B64 @0x140063b64
 *   转发自定义消息     PECMD_ForwardCustomMessage @0x140066e20
 *   分离虚拟磁盘       PECMD_DetachVirtualDisk @0x140069044
 *   释放控件对象       FUN_14006C648 @0x14006c648
 *   释放句柄串对象     FUN_14006E6C8 @0x14006e6c8
 *   获取完整路径       FUN_140078BE8 @0x140078be8
 *   设置 Check 状态    FUN_14007DF90 @0x14007df90
 *   解析单词 token     PECMD_ParseFirstWordToken @0x140085844
 *   释放热键对象       FUN_1400AA484 @0x1400aa484
 *   从对象创建字体     FUN_1400B8960 @0x1400b8960
 *   释放位图控件对象   FUN_1400BCBE0 @0x1400bcbe0
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
extern WCHAR *FUN_14001BE14(WCHAR *s);
extern void FUN_14001a640(WCHAR *buf);
extern void FUN_14001e2cc(void);
extern LARGE_INTEGER FUN_14003C06C(int64_t *a1, LARGE_INTEGER a2, uint32_t a3);
extern uint64_t FUN_14004c0bc(uint64_t a1, uint64_t a2,
                              void *p3, void *p4, void *p5);
extern int FUN_140050F58(int *a1, int64_t *a2, uint32_t a3);
extern void FUN_14006764C(int64_t *a1, int64_t *a2, int16_t a3,
                          int16_t a4);
extern void FUN_140003A20(void *script, WCHAR **out, int mode);
extern void FUN_14007D0AC(int64_t *a1, LPCWSTR a2, LPCWSTR a3);
extern int PECMD_CalcDayOfYear(uint16_t *a1);
extern void PECMD_ReleaseImageHandle(int64_t a1);
extern DWORD FUN_14006459C(LPCWSTR a1, uint32_t a2, LPWSTR a3,
                           LPWSTR *a4);
extern uint64_t PECMD_QueryFileAttributes(LPCWSTR a1, uint32_t *a2, int *a3);
extern void FUN_14007A224(void *script, LPCWSTR text, WCHAR **out, int c, int d);
extern uint64_t *FUN_14007DE70(uint64_t *a1, int64_t *a2, LPCWSTR a3);
extern void FUN_1400F1490(int64_t a1);
extern void *FUN_14001d78c(void *dst, const void *src, int len);
extern DWORD PECMD_CreateDirectory(uint64_t a1, WCHAR *a2);
extern void *PECMD_LoadIcon(LPCWSTR a1, uint64_t *a2);
extern int64_t *PECMD_CopyStrToSlot(uint64_t *a1, uint64_t *a2);
extern HFONT FUN_1400B2218(int *a1, int *a2, LPCWSTR a3);

/* ---- Win32 桩未覆盖的原生 API 声明 (语法编译用) ---- */
extern BOOL EmptyClipboard(void);
extern HANDLE SetClipboardData(UINT, HANDLE);
extern BOOL UnregisterHotKey(HWND, int);
extern ATOM GlobalDeleteAtom(ATOM);
typedef int64_t __time64_t;
extern __time64_t _time64(__time64_t *);
extern int FUN_1400630D0(int a1);

/* ---- 本批引用的全局数据 ---- */
extern uint8_t DAT_14013d660;          /* 数组释放哨兵 */
extern uint8_t PTR_FUN_1401234f0[];
extern uint8_t PTR_FUN_140126740[];
extern uint8_t PTR_FUN_1401290a8[];
extern uint8_t PTR_FUN_1401296e8[];
extern int64_t g_pCacheBlock;
extern int32_t g_flagD6F0;
extern int32_t DAT_14013d738;
extern void (*g_pGdipCreateBitmapFromHICON)();          /* GdipCreateBitmapFromHICON */
extern void (*g_pGdipCreateHBITMAPFromBitmap)();          /* GdipCreateHBITMAPFromBitmap */
extern int (*g_pGdipDisposeImage)();          /* GdipDisposeImage */
extern int64_t *g_pi64e118;
extern int64_t g_i64E120;

/* ========== PECMD_DispatchSpecialDirective @0x140040350 ==========
 * PATH 命令处理：@ 恢复当前目录，# 执行脚本，空串写默认 PATH。
 */
uint32_t PECMD_DispatchSpecialDirective(int64_t *pp, uint16_t *s)
{
    if (*s == 0x40) {
        WCHAR *pWVar2 = FUN_14001BE14((WCHAR *)(s + 1));
        FUN_14001a640(pWVar2);
        FUN_14001e2cc();
    } else if (*s != 0x23) {
        if (*s != 0) {
            return PECMD_CreateDirectory((uint64_t)pp, (WCHAR *)s);
        }
        uint8_t local_58[80];
        memcpy(local_58, WSTR("PATH  @%SystemRoot%\\SYSTEM32"), 0x42);
        uint64_t LVar3 = FUN_14004c0bc((uint64_t)(uintptr_t)g_Script,
                                         (uint64_t)(uintptr_t)local_58,
                                         (void *)0, (void *)0, (void *)0);
        return (uint32_t)LVar3;
    } else {
        FUN_14003C06C(pp, (LARGE_INTEGER)(int64_t)(uintptr_t)(s + 1), 0);
    }
    return 0;
}

/* ========== FUN_14004FAA8 @0x14004faa8 ==========
 * 解析 [start-end] 范围说明；返回 0 成功，-1 失败并置错误码。
 */
uint64_t FUN_14004FAA8(uint32_t *errCode, int64_t *pp, uint16_t *start,
                              uint16_t *end)
{
    uint16_t uVar1 = *(uint16_t *)*pp;
    if (uVar1 == 0x5c) {
        uint16_t *puVar2 = (uint16_t *)*pp + 1;
        *pp = (int64_t)puVar2;
        uVar1 = *puVar2;
        if (uVar1 == 0x74) {
            uVar1 = 9;
        }
    }
    *start = uVar1;
    *pp += 2;
    if (*(int16_t *)*pp == 0x2d) {
        uint16_t *puVar2 = (uint16_t *)((int16_t *)*pp + 1);
        *pp = (int64_t)puVar2;
        uVar1 = *puVar2;
        if ((uVar1 == 0) || (uVar1 == 0x5d)) {
            *errCode = 4;
        } else {
            *end = uVar1;
            *pp += 2;
            if (*start <= *end) {
                return 0;
            }
            *errCode = 8;
        }
        return 0xffffffffULL;
    }
    *end = *start;
    return 0;
}

/* ========== FUN_1400513B8 @0x1400513b8 ==========
 * 解析 [^...] 字符类；返回 0 成功，-1 失败并置错误码。
 */
int FUN_1400513B8(uint32_t *errCode, int64_t *pp, uint8_t *out)
{
    int iVar2;
    int16_t *psVar3;

    *out = 0;
    psVar3 = (int16_t *)*pp;
    if (*psVar3 != 0x5d) {
        int16_t sVar1 = *psVar3;
        if (sVar1 == 0x5e) {
            *pp = (int64_t)(psVar3 + 1);
        }
        psVar3 = (int16_t *)*pp;
        if (*psVar3 != 0x5d) {
            iVar2 = FUN_140050F58((int *)errCode, pp, (uint32_t)(sVar1 == 0x5e));
            if (iVar2 < 0) {
                return iVar2;
            }
            if (*(int16_t *)*pp != 0x5d) {
                *errCode = 4;
                return -1;
            }
            *pp = (int64_t)((int16_t *)*pp + 1);
            return iVar2;
        }
    }
    *pp = (int64_t)(psVar3 + 1);
    *errCode = 6;
    return -1;
}

/* ========== FUN_140053C5C @0x140053c5c ==========
 * 将 <name>.Enable 设为 0/1。
 */
void FUN_140053C5C(int64_t obj, int mode)
{
    if (**(int16_t **)(obj + 0x10) != 0) {
        uint64_t *local_res8 = NULL;
        PECMD_CopyStrToSlot(local_res8, (uint64_t *)(obj + 0x10));
        FUN_14006375C((WCHAR **)&local_res8, WSTR(".Enable"));
        if (**(int16_t **)(obj + 0x10) != 0) {
            LPCWSTR pWVar1 = WSTR("0");
            if (mode != 0) {
                pWVar1 = WSTR("1");
            }
            FUN_14007D0AC(*(int64_t **)(obj + 0x50), (LPCWSTR)local_res8, pWVar1);
        }
        FUN_14005B104((WCHAR **)&local_res8);
    }
}

/* ========== FUN_1400547BC @0x1400547bc ==========
 * 追加一个 token；若当前字符为分隔符则跳过。
 */
int16_t *FUN_1400547BC(int64_t *script, int64_t *pp, int64_t *out,
                                    int16_t sep1, int16_t sep2)
{
    FUN_14006764C(pp, out, sep1, sep2);
    int16_t *psVar1 = (int16_t *)*pp;
    if ((*psVar1 != 0) && ((*psVar1 == sep1 || (*psVar1 == sep2)))) {
        *pp = (int64_t)(psVar1 + 1);
    }
    FUN_140003A20(script, (WCHAR **)out, 1);
    return psVar1;
}

/* ========== FUN_14005CF3C @0x14005cf3c ==========
 * 释放全局缓存对象 g_pCacheBlock 及其内部缓冲区。
 */
void FUN_14005CF3C(void)
{
    int64_t lVar1 = g_pCacheBlock;
    if (g_pCacheBlock != 0) {
        if (*(int64_t *)(g_pCacheBlock + 0x10) != 0) {
            HeapFree(g_hHeap, 0, (void *)(*(int64_t *)(g_pCacheBlock + 0x10) - 8));
        }
        if (*(int64_t *)(lVar1 + 0x20) != 0) {
            HeapFree(g_hHeap, 0, (void *)(*(int64_t *)(lVar1 + 0x20) - 8));
        }
        if (g_pCacheBlock != 0) {
            HeapFree(g_hHeap, 0, (void *)(g_pCacheBlock - 8));
            g_pCacheBlock = 0;
        }
    }
}

/* ========== FUN_14005D30C @0x14005d30c ==========
 * 向 UDP socket 发送 0x30 字节探测包。
 */
void FUN_14005D30C(int mode)
{
    int64_t _Var1;
    uint32_t local_38;
    uint32_t local_34;
    uint32_t local_30;
    uint32_t local_10;
    uint32_t local_c;

    memset(&local_38, 0, 0x30);
    (void)local_34; (void)local_30; (void)local_10; (void)local_c;
    local_38 = ((uint32_t (*)(uint32_t))g_phtonl)(0x1b0004fa);
    local_34 = ((uint32_t (*)(uint32_t))g_phtonl)(0x10000);
    local_30 = ((uint32_t (*)(void))g_phtonl)(); /* TODO(verify): 原反编译缺 htonl 参数 */
    _Var1 = _time64((__time64_t *)0);
    local_10 = ((uint32_t (*)(uint32_t))g_phtonl)((uint32_t)_Var1 - 0x7c558180);
    local_c = ((uint32_t (*)(uint32_t))g_phtonl)(0);
    ((int (*)(int, const void *, int))g_psend)(mode, &local_38, 0x30);
}

/* ========== FUN_14005D558 @0x14005d558 ==========
 * 释放 GDI 对象集合（画刷、字体、区域等）。
 */
void FUN_14005D558(uint64_t *container, HWND hwnd)
{
    uint64_t *puVar1 = (uint64_t *)*container;
    if (puVar1 != NULL) {
        if ((HGDIOBJ)*puVar1 != (HGDIOBJ)0) {
            DeleteObject((HGDIOBJ)*puVar1);
            *puVar1 = 0;
        }
        if ((HGDIOBJ)puVar1[2] != (HGDIOBJ)0) {
            DeleteObject((HGDIOBJ)puVar1[2]);
            puVar1[2] = 0;
        }
        if ((puVar1[1] != 0) && (hwnd != (HWND)0)) {
            SetWindowRgn(hwnd, (HRGN)0, 0);
            DeleteObject((HGDIOBJ)puVar1[1]);
            puVar1[1] = 0;
        }
        if ((HGDIOBJ)puVar1[3] != (HGDIOBJ)0) {
            DeleteObject((HGDIOBJ)puVar1[3]);
            puVar1[3] = 0;
        }
    }
}

/* ========== FUN_14005D770 @0x14005d770 ==========
 * 从注册表读取 LogPixels；成功且 >0 时保存负 96 DPI。
 */
void FUN_14005D770(void)
{
    if (g_flagD6F0 == 0) {
        int32_t local_res8[2] = {0, 0};
        DWORD local_res18[4] = {0, 0, 0, 0};
        DWORD local_res10[2] = {4, 0};
        g_flagD6F0 = -1;
        DWORD DVar1 = FUN_14005C4E0(HKEY_CURRENT_USER, WSTR("Control Panel\\Desktop"),
                                     WSTR("LogPixels"), local_res18, (BYTE *)local_res8,
                                     local_res10);
        if ((DVar1 == 0) && (0 < local_res8[0])) {
            g_flagD6F0 = -0x60;
        }
    }
}

/* ========== FUN_14005D8BC @0x14005d8bc ==========
 * 复制图像；CopyImage 失败时通过 GDI+ 转换图标。
 */
HANDLE FUN_14005D8BC(HANDLE src, uint8_t type, int cx, int cy,
                       UINT flags)
{
    HANDLE local_10 = CopyImage(src, (UINT)type, cx, cy, flags);
    if (local_10 == (HANDLE)0) {
        void *pHVar1 = PECMD_LoadIcon((LPCWSTR)0, (uint64_t *)0);
        if (pHVar1 != NULL) {
            uint64_t local_18 = 0;
            ((int (*)(HANDLE, uint64_t *))g_pGdipCreateBitmapFromHICON)(src, &local_18);
            if (local_18 != 0) {
                ((int (*)(uint64_t, HANDLE *, int))g_pGdipCreateHBITMAPFromBitmap)(local_18, &local_10, 0);
                ((void (*)(uint64_t))g_pGdipDisposeImage)(local_18);
                return local_10;
            }
        }
        local_10 = (HANDLE)0;
    }
    return local_10;
}

/* ========== FUN_14005F96C @0x14005f96c ==========
 * 获取设备扇区大小；IOCTL 失败或参数为 0 时返回默认 512。
 */
int FUN_14005F96C(HANDLE hDevice, int defaultSize)
{
    DWORD local_res18[4] = {0, 0, 0, 0};
    uint32_t local_28 = 0;
    uint64_t local_24[2] = {0, 0};
    int local_14 = 0x200;

    memset(local_24, 0, 0x14);
    BOOL BVar1 = DeviceIoControl(hDevice, 0x70000, (LPVOID)0, 0, &local_28, 0x18,
                                 local_res18, NULL);
    if ((BVar1 != 0) || (defaultSize == 0)) {
        defaultSize = local_14;
    }
    return defaultSize;
}

/* ========== FUN_14005FEAC @0x14005feac ==========
 * 获取设备信息并输出偏移 +4 的字段。
 */
uint64_t *FUN_14005FEAC(HANDLE hDevice, uint64_t *info, uint32_t *outSize)
{
    DWORD local_res10[2] = {0, 0};

    *outSize = 0;
    memset(info, 0, 0xc0);
    BOOL BVar1 = DeviceIoControl(hDevice, 0x70050, (LPVOID)0, 0, info, 0x100000,
                                 local_res10, NULL);
    if (BVar1 == 0) {
        info = NULL;
    } else {
        *outSize = *(uint32_t *)((int64_t)info + 4);
    }
    return info;
}

/* ========== PECMD_EnableBackupPrivileges @0x140060698 ==========
 * 启用备份/还原所需特权。
 */
void PECMD_EnableBackupPrivileges(void)
{
    if ((g_privFlags & 2) == 0) {
        g_privFlags = (uint16_t)(g_privFlags | 0x12);
        FUN_14001C2CC(WSTR("SeRestorePrivilege"), 2, 0);
        FUN_14001C2CC(WSTR("SeSecurityPrivilege"), 2, 0);
        FUN_14001C2CC(WSTR("SeTakeOwnershipPrivilege"), 2, 0);
    }
    FUN_14001C2CC(WSTR("SeBackupPrivilege"), 2, 0);
    FUN_14001C2CC(WSTR("SeBackupPrivilege"), 2, 1);
}

/* ========== PECMD_SetClipboardUnicode @0x140060718 ==========
 * 将 Unicode 文本放入剪贴板。
 */
void PECMD_SetClipboardUnicode(LPCWSTR name)
{
    BOOL BVar1 = OpenClipboard((HWND)0);
    if (BVar1 != 0) {
        EmptyClipboard();
        int iVar2 = lstrlenW(name);
        size_t dwBytes = (size_t)iVar2 * 2 + 2;
        HGLOBAL hMem = GlobalAlloc(0x2000, dwBytes);
        void *puVar3 = GlobalLock(hMem);
        FUN_14001d78c(puVar3, (const void *)name, (int)dwBytes);
        GlobalUnlock(hMem);
        SetClipboardData(0xd, hMem);
        CloseClipboard();
    }
}

/* ========== PECMD_TrimOuterQuotes @0x140060d7c ==========
 * 去除字符串首尾的成对引号；mode>1 时逐个去除。
 */
void PECMD_TrimOuterQuotes(int64_t *pp, uint8_t mode, int16_t sep)
{
    int iVar2 = lstrlenW((LPCWSTR)*pp);
    int16_t *psVar1;
    if (((iVar2 < 2) || (psVar1 = (int16_t *)*pp, sep != *psVar1)) ||
        (sep != psVar1[(int64_t)iVar2 - 1])) {
        if (1 < mode) {
            if (sep == *(int16_t *)*pp) {
                iVar2--;
                *pp = (int64_t)((int16_t *)*pp + 1);
            }
            if (0 < iVar2) {
                if (sep == *(int16_t *)(*pp - 2 + (int64_t)iVar2 * 2)) {
                    *(uint16_t *)(*pp - 2 + (int64_t)iVar2 * 2) = 0;
                }
            }
        }
    } else {
        psVar1[(int64_t)iVar2 - 1] = 0;
        *pp += 2;
    }
}

/* ========== FUN_140060EE0 @0x140060ee0 ==========
 * 根据字体度量计算条目占用行数。
 */
int FUN_140060EE0(uint16_t *s, int mode)
{
    int iVar1 = PECMD_CalcDayOfYear(s) - 1;
    int iVar2 = ((iVar1 / 7) * 7 + (int)s[2]) - iVar1;
    if (iVar2 < 0) {
        iVar2 += 7;
    }
    if (mode == 0) {
        iVar1 = (iVar2 + iVar1) / 7 + 1;
    } else {
        if (iVar2 == 0) {
            iVar2 = 7;
        }
        iVar1 = (iVar2 + 6 + iVar1) / 7;
    }
    return iVar1;
}

/* ========== PECMD_TruncateObjectArray @0x140061704 ==========
 * 释放动态对象数组中 [start, 原计数) 范围内的对象。
 */
void PECMD_TruncateObjectArray(int64_t *arr, int *count, int start)
{
    EnterCriticalSection(&g_csInit);
    while (start < *count) {
        *count = *count - 1;
        int64_t lVar3 = (int64_t)*count;
        int64_t lVar1 = *arr;
        if ((*(int64_t *)(lVar1 + lVar3 * 8) != 0) &&
            (*(void **)(lVar1 + lVar3 * 8) != (void *)&DAT_14013d660)) {
            uint64_t *puVar2 = *(uint64_t **)(lVar1 + lVar3 * 8);
            *(uint64_t *)(lVar1 + lVar3 * 8) = 0;
            if (puVar2 != NULL) {
                (**(void (**)(uint64_t *, int))(*puVar2))(puVar2, 1);
            }
        }
    }
    LeaveCriticalSection(&g_csInit);
}

/* ========== FUN_1400634D4 @0x1400634d4 ==========
 * 将 ANSI 串存入带堆头的串容器。
 */
int64_t *FUN_1400634D4(int64_t *ps, LPCSTR src, int64_t len)
{
    int iVar2;
    int64_t *plVar3;

    if (len < 1) {
        iVar2 = lstrlenA(src);
        len = (int64_t)iVar2;
    }
    void *puVar1 = (void *)*ps;
    plVar3 = (int64_t *)FUN_140063118(puVar1, (size_t)(len + 1));
    *ps = (int64_t)plVar3;
    FUN_14001d78c((void *)plVar3, (const void *)src, (int)len);
    if (puVar1 != NULL) {
        HeapFree(g_hHeap, 0, (uint8_t *)puVar1 - 8);
    }
    *(uint8_t *)(len + *ps) = 0;
    return ps;
}

/* ========== FUN_1400639F0 @0x1400639f0 ==========
 * 向动态数组追加一个元素；容量不足时增长。
 */
int64_t FUN_1400639F0(int64_t *arr, int64_t *cap, int64_t *cnt,
                                 void *data, uint32_t esize, int64_t grow)
{
    int64_t lVar1;
    int64_t lVar2;
    int64_t *plVar3;

    if (*cap - 1 <= *cnt) {
        lVar1 = *cap + grow;
        *cap = lVar1;
        plVar3 = (int64_t *)FUN_140063118((void *)*arr, (size_t)((uint64_t)esize * lVar1));
        *arr = (int64_t)plVar3;
    }
    FUN_14001d78c((void *)(*cnt * (uint64_t)esize + *arr), data, esize);
    lVar1 = *cnt;
    lVar2 = *arr;
    *cnt = lVar1 + 1;
    return lVar2 + lVar1;
}

/* ========== FUN_140063B64 @0x140063b64 ==========
 * 初始化动态数组容器，容量初始为 1。
 */
int64_t *FUN_140063B64(int64_t *arr)
{
    int iVar2;
    uint64_t *puVar3;
    int64_t lVar1;
    int64_t lVar4;

    arr[2] = 0;
    *arr = 0;
    arr[1] = 1;
    do {
        puVar3 = (uint64_t *)HeapAlloc(g_hHeap, 0, 0x10);
        if (puVar3 != NULL) break;
        iVar2 = FUN_1400630D0(2);
    } while (iVar2 == 4);
    *(uint32_t *)((uint8_t *)puVar3 + 4) = 0xaa55;
    *puVar3 = 8;
    lVar1 = arr[1];
    lVar4 = 0;
    *arr = (int64_t)(puVar3 + 1);
    if (0 < lVar1) {
        do {
            *(uint64_t *)(*arr + lVar4 * 8) = 0;
            lVar4++;
        } while (lVar4 < lVar1);
    }
    return arr;
}

/* ========== PECMD_ForwardCustomMessage @0x140066e20 ==========
 * 转发 WM_ 0x1000 起始的自定义消息，再调用 vtable 消息处理。
 */
void PECMD_ForwardCustomMessage(int64_t *obj, WPARAM wParam, LPARAM lParam)
{
    int iVar2 = (int)((uint32_t)wParam & 0xffff) - 0x1000;
    if (((-1 < iVar2) && ((int64_t)iVar2 < (int64_t)*(int *)obj[0x35])) &&
        (*(int64_t *)(*(int64_t *)(*(int64_t *)obj[0x34] + (int64_t)iVar2 * 8) + 0x38) != 0)) {
        HWND hWnd = *(HWND *)(*(int64_t *)(*(int64_t *)(*(int64_t *)obj[0x34] +
                                                         (int64_t)iVar2 * 8) + 0x38) + OBJ_HWND);
        if (hWnd != (HWND)0) {
            SendMessageW(hWnd, 0xbc2c, wParam, lParam);
        }
    }
    (**(void (**)(int64_t, int, WPARAM, LPARAM))(*obj + 8))(obj[4], 0x2c,
                                                                  wParam, lParam);
}

/* ========== PECMD_DetachVirtualDisk @0x140069044 ==========
 * 分离/卸载虚拟磁盘；优先 DetachVirtualDisk，其次 UnmountVHD。
 */
uint64_t PECMD_DetachVirtualDisk(LPCWSTR name, uint32_t flags)
{
    uint32_t local_res10[6];
    local_res10[0] = flags;

    if ((g_pDetachVirtualDisk == NULL) || (DAT_14013d738 == 0)) {
        if (g_pUnmountVHD == NULL) {
            return 0x80004001;
        }
        return ((uint64_t (*)(LPCWSTR, int))g_pUnmountVHD)(name, 3);
    } else {
        HANDLE hObject = (HANDLE)PECMD_QueryFileAttributes(name, local_res10, (int *)0);
        if (hObject == (HANDLE)0) {
            return 0;
        }
        uint32_t uVar1 = ((uint32_t (*)(HANDLE, int, int))g_pDetachVirtualDisk)(hObject, 0, 0);
        CloseHandle(hObject);
        return (uint64_t)uVar1;
    }
}

/* ========== FUN_14006C648 @0x14006c648 ==========
 * 释放控件对象（vtable 140126740）。
 */
void FUN_14006C648(uint64_t *obj)
{
    *obj = (uint64_t)(uintptr_t)PTR_FUN_140126740;
    int64_t *plVar1 = (int64_t *)obj[7];
    if (plVar1 != NULL) {
        (**(void (**)(int64_t *, int))(*plVar1 + 0x18))(plVar1, 1);
    }
    obj[7] = 0;
    FUN_14005B104((WCHAR **)(obj + 0x10));
    FUN_14005B104((WCHAR **)(obj + 0xe));
    *obj = (uint64_t)(uintptr_t)PTR_FUN_1401234f0;
    FUN_1400F1490((int64_t)(obj + 0xb));
    FUN_14005B104((WCHAR **)(obj + 5));
    FUN_14005B104((WCHAR **)(obj + 4));
    FUN_14005B104((WCHAR **)(obj + 3));
    FUN_14005B104((WCHAR **)(obj + 2));
}

/* ========== FUN_14006E6C8 @0x14006e6c8 ==========
 * 释放带句柄+串的对象；计数归零时从全局表移除。
 */
void FUN_14006E6C8(uint64_t *obj)
{
    EnterCriticalSection(&g_csInit);
    *(int *)(obj + 2) = *(int *)(obj + 2) - 1;
    if (*(int *)(obj + 2) < 1) {
        int iVar2 = 0;
        int64_t lVar1 = 0;
        if (0 < g_intE128) {
            do {
                if (*(uint64_t **)(g_pi64e118 + lVar1 * 8) == obj) {
                    *(uint64_t *)(g_pi64e118 + (int64_t)iVar2 * 8) = 0;
                    if ((HANDLE)*obj != (HANDLE)0) {
                        CloseHandle((HANDLE)*obj);
                    }
                    FUN_14005B104((WCHAR **)(obj + 1));
                    free(obj);
                    break;
                }
                lVar1++;
                iVar2++;
            } while (lVar1 < g_intE128);
        }
    }
    LeaveCriticalSection(&g_csInit);
}

/* ========== FUN_140078BE8 @0x140078be8 ==========
 * 获取完整路径字符串并复制到目标容器。
 */
int64_t FUN_140078BE8(LPCWSTR name, int64_t *out)
{
    LPWSTR local_res10 = NULL;
    LPWSTR local_res18 = NULL;

    PECMD_AllocWStringBuffer(&local_res10, 0x2411);
    FUN_14006459C(name, 0x2410, local_res10, &local_res18);
    if (local_res18 != NULL) {
        local_res18[-1] = L'\0';
    }
    FUN_1400703E4((WCHAR **)out, local_res10);
    int64_t lVar1 = *out;
    FUN_14005B104((WCHAR **)&local_res10);
    return lVar1;
}

/* ========== FUN_14007DF90 @0x14007df90 ==========
 * 将 <name>.Check 设为 0/1。
 */
void FUN_14007DF90(int64_t ctx, int mode)
{
    if (**(int16_t **)(ctx + 0x10) != 0) {
        int64_t local_res18[2] = {0, 0};
        uint64_t *local_res8 = NULL;
        uint64_t *puVar1 = FUN_14007DE70((uint64_t *)(ctx + 0x10), local_res18, WSTR(".Check"));
        PECMD_CopyStrToSlot(local_res8, puVar1);
        FUN_14005B104((WCHAR **)local_res18);
        if (**(int16_t **)(ctx + 0x10) != 0) {
            LPCWSTR pWVar2 = WSTR("0");
            if (mode != 0) {
                pWVar2 = WSTR("1");
            }
            FUN_14007D0AC(*(int64_t **)(ctx + 0x50), (LPCWSTR)local_res8, pWVar2);
        }
        FUN_14005B104((WCHAR **)&local_res8);
    }
}

/* ========== PECMD_ParseFirstWordToken @0x140085844 ==========
 * 取首个非空白单词并交给解析器，随后跳过空白。
 */
uint64_t PECMD_ParseFirstWordToken(int64_t *a, int64_t *b, uint64_t *c)
{
    WCHAR *pWVar2 = (WCHAR *)*b;
    WCHAR WVar1 = *pWVar2;
    while ((WVar1 != L'\0') &&
           ((*((uint16_t *)*b) < 9 ||
             (0xd < *((uint16_t *)*b))) &&
            (*((uint16_t *)*b) != 0x20))) {
        *b = (int64_t)((uint16_t *)*b + 1);
        WVar1 = *((uint16_t *)*b);
    }
    if (*(int16_t *)*b != 0) {
        *(int16_t *)*b = 0;
        *b += 2;
    }
    FUN_14007A224((void *)a, pWVar2, (WCHAR **)c, 0, 1);
    FUN_14005B154((WCHAR **)b);
    return *c;
}

/* ========== FUN_1400AA484 @0x1400aa484 ==========
 * 释放注册了热键的对象。
 */
void FUN_1400AA484(uint64_t *obj)
{
    *obj = (uint64_t)(uintptr_t)PTR_FUN_1401290a8;
    if (*(int16_t *)((uint8_t *)obj + 0x7e) != 0) {
        UnregisterHotKey(*(HWND *)(obj[0x10] + OBJ_HWND),
                         (int)*(uint16_t *)((uint8_t *)obj + 0x7e));
        GlobalDeleteAtom(*(ATOM *)((uint8_t *)obj + 0x7e));
        *(uint16_t *)((uint8_t *)obj + 0x7c) = 0;
        *(uint16_t *)((uint8_t *)obj + 0x7e) = 0;
    }
    *obj = (uint64_t)(uintptr_t)PTR_FUN_1401234f0;
    FUN_1400F1490((int64_t)(obj + 0xb));
    FUN_14005B104((WCHAR **)(obj + 5));
    FUN_14005B104((WCHAR **)(obj + 4));
    FUN_14005B104((WCHAR **)(obj + 3));
    FUN_14005B104((WCHAR **)(obj + 2));
}

/* ========== FUN_1400B8960 @0x1400b8960 ==========
 * 从 GDI 对象读取字体信息并按 DPI 创建字体。
 */
void FUN_1400B8960(HANDLE hFont, int *size, LPCWSTR name)
{
    int local_68 = 0;
    uint64_t local_64 = 0;
    uint32_t local_58 = 400;
    uint8_t local_51 = 1;
    memset(&local_64, 0, 0x58);
    GetObjectW((HGDIOBJ)hFont, 0x5c, &local_68);
    local_64 = (local_64 & ~0xffffffffULL) | 0;  /* _0_4_ = 0 */
    FUN_1400B2218(&local_68, size, name);
    (void)local_58;
    (void)local_51;
}

/* ========== FUN_1400BCBE0 @0x1400bcbe0 ==========
 * 释放位图控件对象（vtable 1401296e8）。
 */
uint64_t *FUN_1400BCBE0(uint64_t *obj, uint32_t flags)
{
    *obj = (uint64_t)(uintptr_t)PTR_FUN_1401296e8;
    PECMD_ReleaseImageHandle((int64_t)obj);
    FUN_14005B104((WCHAR **)(obj + 0x11));
    *obj = (uint64_t)(uintptr_t)PTR_FUN_1401234f0;
    FUN_1400F1490((int64_t)(obj + 0xb));
    FUN_14005B104((WCHAR **)(obj + 5));
    FUN_14005B104((WCHAR **)(obj + 4));
    FUN_14005B104((WCHAR **)(obj + 3));
    FUN_14005B104((WCHAR **)(obj + 2));
    if ((flags & 1) != 0) {
        free(obj);
    }
    return obj;
}
