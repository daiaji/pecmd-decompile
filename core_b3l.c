/* ====================================================================
 * core_b3l.c — B3-B6 小函数批 11 (140040000-1400bffff)
 *
 * 本批新实现函数全部使用人类可读 PECMD_ 名称，原始地址保留在 @0x 注释。
 *
 * 来源: PECMD原始.EXE (x64, ImageBase=0x140000000)
 *   解析字符类主体    FUN_140050F58 @0x140050f58
 *   初始化控件标志    PECMD_InitControlFlags @0x140054380
 *   绘制控件重绘      PECMD_DispatchControlMessage @0x140061914
 *   插入控件对象 A    FUN_140063ED4 @0x140063ed4
 *   插入控件对象 B    FUN_140063FF0 @0x140063ff0
 *   更新滑块变量      FUN_140066D18 @0x140066d18
 *   读文本文件为宽串  PECMD_ReadFileToWide @0x140068034
 *   查询磁盘几何      PECMD_QueryDeviceInfo @0x140069464
 *   查找分区布局      FUN_140069BD8 @0x140069bd8
 *   展开盘符字母      PECMD_ExpandDriveList @0x14006aa9c
 *   查询设备布局      FUN_14006ABB8 @0x14006abb8
 *   查找或追加对象槽  FUN_14006B6E8 @0x14006b6e8
 *   应用控件属性      FUN_1400FE4A4 @0x14006b7f0
 *   调用 COM 方法     FUN_1400705AC @0x1400705ac
 *   获取网络连接名    PECMD_ReadConnectionRegistryValue @0x140072814
 *   查找网卡信息      FUN_140072D8C @0x140072d8c
 *   用参数重启自身    FUN_14007724C @0x14007724c
 *   关闭设备句柄扩展  PECMD_CloseDeviceHandle @0x140078408
 *   Base64 编码为宽串 FUN_140078D8C @0x140078d8c
 *   按类型取分区起点  FUN_140079E34 @0x140079e34
 *   设置变量(带&处理) FUN_14007D0AC @0x14007d0ac
 *   图像编码到流      FUN_14007D340 @0x14007d340
 *   解析命令行参数    FUN_14008293C @0x14008293c
 *   保存选择状态      FUN_14009CFBC @0x14009cfbc
 *   设置随机变量      FUN_1400A3F08 @0x1400a3f08
 *   解析 <...> 参数   FUN_1400A40E8 @0x1400a40e8
 *   插入控件对象 C    FUN_1400AA7E4 @0x1400aa7e4
 *   插入控件对象 D    FUN_1400AAA4C @0x1400aaa4c
 *   插入控件对象 E    FUN_1400B2A4C @0x1400b2a4c
 *   蜂鸣并执行        FUN_1400BEF64 @0x1400bef64
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
extern void *FUN_140063118(void *ptr, size_t size); /* @0x140063118 */
extern WCHAR **FUN_14005B154(WCHAR **pp); /* @0x14005b154 */
extern void FUN_1400629B8(void *script, LPCWSTR key, LPCWSTR value); /* @0x1400629b8 */

/* ---- 已实现公共工具 (其他 core_*.c / core_globals.c) ---- */
extern void FUN_1400633A8(void **ps, int64_t len);            /* @0x1400633a8 */
extern WCHAR *FUN_1400637DC(WCHAR **ps, LPCSTR src, int64_t srclen, int64_t codepage); /* @0x1400637dc */
extern void *FUN_14007026C(void **out, const char *src);    /* @0x14007026c */
extern int64_t *FUN_14006345C(int64_t *a1, LPCSTR a2); /* @0x14006345c */
extern int64_t *FUN_1400702F0(int64_t *out, const char *src, uint64_t len); /* @0x1400702f0 */
extern void FUN_1400E6D74(WCHAR *dst, uint64_t v);            /* @0x1400e6d74 */
extern int FUN_14005F96C(HANDLE a1, int a2); /* @0x14005f96c */
extern uint64_t *FUN_14005FEAC(HANDLE a1, uint64_t *a2, uint32_t *a3); /* @0x14005feac */
extern uint64_t FUN_14006C4C8(int64_t a1, int a2, LPARAM *a3); /* @0x14006c4c8 */
extern DWORD PECMD_FlushDeviceIoctl(HANDLE a1);              /* @0x140060120 */
extern void PECMD_SetClipboardUnicode(LPCWSTR a1);         /* @0x140060718 */
extern void PECMD_OpenFileHandle(HANDLE *out, LPCWSTR path, DWORD access, DWORD share,
                           LPSECURITY_ATTRIBUTES sa, DWORD disp, DWORD flags,
                           HANDLE tmpl);                     /* @0x140003864 */

/* ---- 未实现依赖 (extern + TODO(verify)) ---- */
extern void AtlThrowImpl(long a1);
extern void FUN_14005C828(LPCSTR a1, LPCSTR a2, int64_t *a3,
                          int64_t *a4);
extern int FUN_14004FE34(int *a1, int a2);
extern uint64_t FUN_14004FAA8(uint32_t *a1, int64_t *a2,
                              uint16_t *a3, uint16_t *a4);
extern uint16_t FUN_1400F172C(int64_t *a1, uint32_t a2, uint64_t a3,
                              uint64_t *a4, int64_t a5, uint32_t a6,
                              int64_t *a7);
extern HWND FUN_1400E5788(HWND a1);
extern int64_t FUN_1400E5B0C(int64_t a1, uint64_t a2, HDC a3,
                             int64_t *a4);
extern void *PECMD_SendCtrlMessage_0834(WPARAM a1, uint64_t a2);
extern void FUN_14005DAF8(int64_t a1, int *a2, int *a3,
                          int *a4, int *a5);
extern void *PECMD_CreateScrollbarCtl(void *a1, int64_t a2, int a3,
                           uint64_t *a4, int a5, int a6, int a7,
                           int a8, uint64_t *a9, uint64_t *a10,
                           uint32_t a11);
extern void *PECMD_ConstructControlFont(void *a1, int64_t a2, int a3,
                           uint64_t *a4, int a5, int a6, int a7,
                           int a8, uint64_t *a9, uint64_t *a10,
                           uint32_t a11);
extern void FUN_14005D9A8(int64_t a1, int a2);
extern int64_t FUN_1400E693C(HANDLE a1);
extern uint64_t *PECMD_AllocSmallObject(uint64_t *a1);
extern uint64_t *PECMD_GrowByteBuffer(uint64_t *a1, int64_t a2);
extern int FUN_14005B184(char *a1, int64_t a2, int64_t a3);
extern int64_t PECMD_ParsePartitionTable(HANDLE a1, int64_t *a2, int a3,
                             LARGE_INTEGER a4, uint64_t a5,
                             uint32_t a6);
extern int FUN_1400F2384(int64_t a1, LPCWSTR a2, int64_t *a3,
                         int64_t a4, int a5, int a6);
extern void FUN_140061C44(void);
extern void PECMD_ContainerAppend(int64_t *a1);
extern int64_t PECMD_GetCachedBlock(void);
extern bool PECMD_PrefixMatchNoCase(uint16_t *a1, char *a2);
extern void PECMD_LockVolumeByDevice(int a1, int a2);
extern void PECMD_ErrorHandlerWrap(uint8_t *a1, int a2, uint64_t a3,
                          int64_t a4, int a5, WCHAR *a6);
extern void PECMD_ClearFlagAndError(uint8_t *a1);
extern int PECMD_InstallDevice(LPCWSTR a1, uint32_t a2);
extern void FUN_14005D534(void *a1, LPCWSTR a2, LPCWSTR a3);
extern void FUN_14001E6BC(void *a1, LPCWSTR a2, LPCWSTR a3,
                          int a4);
extern void *PECMD_LoadIcon(LPCWSTR a1, uint64_t *a2);
extern int64_t PECMD_GetConfigEntryByName(LPCWSTR a1, uint64_t *a2);
extern void FUN_1400F429C(WCHAR **pp, WCHAR ch);
extern void FUN_14007BF44(int64_t *ctx, WCHAR *name, void *out, int mode,
                          uint8_t flag);
extern uint64_t PECMD_GetRandom(void);
extern void FUN_14007BDA8(void *script, LPCWSTR text, WCHAR **out, int c,
                          int d);
extern void FUN_14007A224(void *script, LPCWSTR text, WCHAR **out, int c,
                          int d);
extern void PECMD_ParseNumSkipChar_0248(int64_t *a1, int64_t *a2);
extern void *PECMD_ConstructTreeView(void *a1, int64_t a2, int a3,
                           uint64_t *a4, int a5, int a6, int a7,
                           int a8, uint32_t a9, LPWSTR a10,
                           uint8_t a11);
extern void *PECMD_ConstructSlider(void *a1, int64_t a2, int a3,
                           uint64_t *a4, int a5, int a6, int a7,
                           int a8, uint16_t *a9, uint64_t *a10,
                           uint32_t a11);
extern void *PECMD_CreateDialogControl(void *a1, LPCWSTR a2, int a3,
                           uint64_t *a4, int a5, int a6, int a7,
                           int a8, int16_t *a9, uint32_t a10,
                           int64_t a11);
extern void FUN_1400bb718(void *script, LPCWSTR text, int64_t *a3);
extern void Beep(uint32_t, uint32_t);

/* ---- 本批引用的全局数据 ---- */
extern uint8_t DAT_14013d660;        /* 数组释放哨兵 */
extern uint8_t g_u8D5D0;        /* 对象槽 vtable */
extern uint8_t g_flagD6F5;
extern int64_t g_pComWrite;          /* COM 调用函数指针 */
extern void (*g_pGdipCreateBitmapFromHBITMAP)();          /* 图像解码函数指针 */
extern void *g_pGdipSaveImageToFile;          /* 图像编码函数指针 */
extern int (*g_pGdipDisposeImage)();          /* GdipDisposeImage 函数指针 */
extern int (*g_pGetAdaptersInfo)(int64_t, uint32_t *);          /* GetAdaptersInfo 函数指针 */
extern uint8_t g_b24d10[];
extern uint8_t g_b24d20[];
extern uint8_t g_b297d0[];
extern uint64_t g_u6497e8;
extern uint64_t g_u6497f0;
extern uint64_t g_u6497f8;
extern uint32_t g_u329800;
extern uint16_t g_u169804;
extern uint8_t g_u897806;
extern uint64_t g_u64_124e40;
extern uint64_t g_u64_124e48;

/* ========== FUN_140050F58 @0x140050f58 ==========
 * 解析字符类主体，返回起始槽位或 -1。
 */
int FUN_140050F58(int *arr, int64_t *pp, uint32_t mode)
{
    int iVar1 = FUN_14004FE34(arr, (mode != 0) + 7);
    if (iVar1 < 0) {
        return -1;
    }

    while ((*(int16_t *)*pp != 0) && (*(int16_t *)*pp != 0x5d)) {
        uint16_t local_res18[4] = {0, 0, 0, 0};
        uint16_t local_res20[4] = {0, 0, 0, 0};
        uint64_t uVar4 = FUN_14004FAA8((uint32_t *)arr, pp,
                                       local_res18, local_res20);
        if ((int)uVar4 != 0) {
            return -1;
        }
        int iVar2 = FUN_14004FE34(arr, 0);
        if (iVar2 < 0) {
            return -1;
        }
        int iVar3 = FUN_14004FE34(arr, 0);
        if (iVar3 < 0) {
            return -1;
        }
        if (*(uint64_t *)(arr + 4) <= (uint64_t)iVar2) {
            AtlThrowImpl(-0x7ff8ffa9);
        }
        *(uint64_t *)(*(int64_t *)(arr + 2) + 8 + (int64_t)iVar2 * 0x10) =
            (uint64_t)local_res18[0];
        if (*(uint64_t *)(arr + 4) <= (uint64_t)iVar3) {
            AtlThrowImpl(-0x7ff8ffa9);
        }
        *(uint64_t *)(*(int64_t *)(arr + 2) + 8 + (int64_t)iVar3 * 0x10) =
            (uint64_t)local_res20[0];
    }

    if (*(uint64_t *)(arr + 4) <= (uint64_t)iVar1) {
        AtlThrowImpl(-0x7ff8ffa9);
    }
    *(uint64_t *)(*(int64_t *)(arr + 2) + 8 + (int64_t)iVar1 * 0x10) =
        *(uint64_t *)(arr + 4);
    return iVar1;
}

/* ========== PECMD_InitControlFlags @0x140054380 ==========
 * 根据参数初始化控件对象标志。
 */
void PECMD_InitControlFlags(int64_t obj, uint32_t *spec, int height,
                            LPCWSTR text, int x, int y, int w,
                            uint32_t flags)
{
    *(uint32_t *)(obj + 4) = 0;
    int iVar4 = (int)spec[4];
    char cVar1 = *(char *)((uint8_t *)spec + 0x14);
    char cVar2 = *(char *)((uint8_t *)spec + 0x15);
    char cVar3 = *(char *)((uint8_t *)spec + 0x16);

    if (-1 < x) {
        *(int *)(obj + 0x14) = x;
        *(uint32_t *)(obj + 4) = 0x40000000;
    }
    if (-1 < y) {
        *(uint32_t *)(obj + 4) = *(uint32_t *)(obj + 4) | 0x4000000;
        *(int *)(obj + 0x60) = y;
    }
    if (-1 < height) {
        *(uint32_t *)(obj + 4) = *(uint32_t *)(obj + 4) | 0x80000000;
        *(int *)(obj + 0xc) = height * 0x14;
    }
    if (iVar4 == 700) {
        *(uint32_t *)(obj + 4) = *(uint32_t *)(obj + 4) | 1;
        *(uint32_t *)(obj + 8) = *(uint32_t *)(obj + 8) | 1;
    }
    if (cVar1 != '\0') {
        *(uint32_t *)(obj + 4) = *(uint32_t *)(obj + 4) | 2;
        *(uint32_t *)(obj + 8) = *(uint32_t *)(obj + 8) | 2;
    }
    if (cVar3 != '\0') {
        *(uint32_t *)(obj + 4) = *(uint32_t *)(obj + 4) | 8;
        *(uint32_t *)(obj + 8) = *(uint32_t *)(obj + 8) | 8;
    }
    if (cVar2 != '\0') {
        *(uint32_t *)(obj + 4) = *(uint32_t *)(obj + 4) | 4;
        *(uint32_t *)(obj + 8) = *(uint32_t *)(obj + 8) | 4;
    }
    if (w != 0) {
        *(uint32_t *)(obj + 4) = *(uint32_t *)(obj + 4) | 0x20;
        *(uint32_t *)(obj + 8) = *(uint32_t *)(obj + 8) | 0x20;
    }
    if (0 < (int)spec[4]) {
        *(uint32_t *)(obj + 4) = *(uint32_t *)(obj + 4) | 0x400000;
        *(uint16_t *)(obj + 0x5c) = *(uint16_t *)(spec + 4);
    }
    if ((flags >> 0x1b & 1) != 0) {
        *(uint32_t *)(obj + 4) = *(uint32_t *)(obj + 4) | 0x8000000;
        *(uint8_t *)(obj + 0x18) = *(uint8_t *)((uint8_t *)spec + 0x17);
    }
    *(uint8_t *)(obj + 0x19) = *(uint8_t *)((uint8_t *)spec + 0x1b);
    if ((int)flags < 0) {
        *(uint32_t *)(obj + 4) = *(uint32_t *)(obj + 4) | 0x80000000;
        *(uint32_t *)(obj + 0xc) = *spec;
    }
    if (*text != L'\0') {
        *(uint32_t *)(obj + 4) = *(uint32_t *)(obj + 4) | 0x20000000;
        lstrcpynW((LPWSTR)(obj + 0x1a), text, 0x20);
    }
}

/* ========== PECMD_DispatchControlMessage @0x140061914 ==========
 * 绘制控件，并在需要时发送前后台重绘消息。
 */
int64_t PECMD_DispatchControlMessage(HDC obj, uint32_t msg, HDC hdc, uint64_t *lParam)
{
    int64_t local_res8 = 0;
    uint16_t uVar2 = FUN_1400F172C(*(int64_t **)((uint8_t *)obj + 0x34), msg,
                                   (uint64_t)(uintptr_t)hdc, lParam,
                                   *(int64_t *)((uint8_t *)obj + 8), 1, &local_res8);
    int64_t lVar3 = local_res8;

    if ((uVar2 & 4) == 0) {
        int iVar1 = *(int *)(*(int64_t *)((uint8_t *)obj + 0x34) + 0x10);
        HWND hWnd = (HWND)0;
        if (iVar1 >= 1) {
            hWnd = FUN_1400E5788(*(HWND *)((uint8_t *)obj + 8));
        }
        if (hWnd != (HWND)0) {
            SendMessageW(hWnd, 0x450, 4, 0x5aa555aa);
        }
        lVar3 = FUN_1400E5B0C((int64_t)(uintptr_t)obj, (uint64_t)msg,
                              hdc, (int64_t *)lParam);
        if (0 < iVar1) {
            FUN_1400F172C(*(int64_t **)((uint8_t *)obj + 0x34), msg,
                          (uint64_t)(uintptr_t)hdc, lParam,
                          *(int64_t *)((uint8_t *)obj + 8), 2, NULL);
        }
        if (hWnd != (HWND)0) {
            PostMessageW(hWnd, 0x451, 0x204, (LPARAM)(intptr_t)-0x55aaa55b);
        }
    }
    return lVar3;
}

/* ========== FUN_140063ED4 @0x140063ed4 ==========
 * 创建 0x78 控件对象并插入到指定槽。
 */
void FUN_140063ED4(WPARAM mgr, int64_t v2, uint64_t *p3,
                                int x, int y, int w, int h,
                                uint64_t *p8, uint64_t *p9, uint32_t flags)
{
    int local_res20[2] = {0, 0};
    local_res20[0] = x;
    uint64_t *puVar2 = (uint64_t *)PECMD_SendCtrlMessage_0834(mgr, *p3);
    uint64_t *puVar4 = NULL;

    if (puVar2 != NULL) {
        int64_t lVar1 = **(int64_t **)((uint8_t *)mgr + 0x1a0);
        FUN_14005DAF8((int64_t)mgr, local_res20, &y, &w, &h);
        void *puVar3 = calloc(1, 0x78);
        if (puVar3 != NULL) {
            puVar4 = (uint64_t *)PECMD_CreateScrollbarCtl(puVar3, v2,
                        (int)(((uint64_t)(uintptr_t)puVar2 - (uint64_t)lVar1) >> 3) + 0x1000,
                        p3, local_res20[0], y, w, h,
                        p8, p9, flags);
        }
        *puVar2 = (uint64_t)(uintptr_t)puVar4;
        FUN_14005D9A8((int64_t)mgr, 0);
    }
}

/* ========== FUN_140063FF0 @0x140063ff0 ==========
 * 创建 0x70 控件对象并插入到指定槽。
 */
void FUN_140063FF0(WPARAM mgr, int64_t v2, uint64_t *p3,
                                int x, int y, int w, int h,
                                uint64_t *p8, uint64_t *p9, uint32_t flags)
{
    int local_res20[2] = {0, 0};
    local_res20[0] = x;
    uint64_t *puVar2 = (uint64_t *)PECMD_SendCtrlMessage_0834(mgr, *p3);
    uint64_t *puVar4 = NULL;

    if (puVar2 != NULL) {
        int64_t lVar1 = **(int64_t **)((uint8_t *)mgr + 0x1a0);
        FUN_14005DAF8((int64_t)mgr, local_res20, &y, &w, &h);
        void *puVar3 = calloc(1, 0x70);
        if (puVar3 != NULL) {
            puVar4 = (uint64_t *)PECMD_ConstructControlFont(puVar3, v2,
                        (int)(((uint64_t)(uintptr_t)puVar2 - (uint64_t)lVar1) >> 3) + 0x1000,
                        p3, local_res20[0], y, w, h,
                        p8, p9, flags);
        }
        *puVar2 = (uint64_t)(uintptr_t)puVar4;
        FUN_14005D9A8((int64_t)mgr, 0);
    }
}

/* ========== FUN_140066D18 @0x140066d18 ==========
 * 更新滑块位置相关变量并通知控件。
 */
void FUN_140066D18(int64_t obj, int64_t info)
{
    int64_t *plVar3 = *(int64_t **)((uint8_t *)obj + 0x38);
    int iVar1 = *(int *)((uint8_t *)info + 0x1c);
    int64_t lVar7 = (int64_t)iVar1;
    uint32_t uVar2 = *(uint32_t *)((uint8_t *)info + 0x18);
    int64_t local_res8 = 0;
    int64_t local_res10 = 0;

    void (**fn)(int64_t *, int64_t *, int64_t *) =
        (void (**)(int64_t *, int64_t *, int64_t *))(*plVar3 + 0x118);
    (*fn)(plVar3, &local_res8, &local_res10);

    uint32_t uVar4 = (uint32_t)iVar1 + uVar2;
    uint32_t uVar6 = (uint32_t)local_res10;
    uint32_t uVar8 = (uint32_t)local_res8;
    if (local_res10 < local_res8) {
        uVar6 = (uint32_t)local_res8;
        uVar8 = (uint32_t)local_res10;
    }

    uint32_t uVar5;
    if ((*(uint8_t *)((uint8_t *)obj + 0x70) & 0x20) == 0) {
        uVar5 = uVar8;
        if (((int32_t)uVar8 <= (int32_t)uVar4) && (uVar5 = uVar4,
             (int32_t)uVar6 < (int32_t)uVar4)) {
            uVar5 = uVar6;
        }
    } else {
        uVar5 = uVar6;
        if (((int32_t)uVar8 <= (int32_t)uVar4) && (uVar5 = uVar4,
             (int32_t)uVar6 < (int32_t)uVar4)) {
            uVar5 = uVar8;
        }
    }

    if (local_res10 < local_res8) {
        lVar7 = -lVar7;
    }
    FUN_1400669C4((void *)*(int64_t **)((uint8_t *)obj + 0x50),
                  (int64_t)uVar5, *(LPCWSTR *)((uint8_t *)obj + 0x80));
    FUN_1400669C4((void *)*(int64_t **)((uint8_t *)obj + 0x50),
                  (int64_t)(-1 < lVar7), *(LPCWSTR *)((uint8_t *)obj + 0x88));
    FUN_1400669C4((void *)*(int64_t **)((uint8_t *)obj + 0x50),
                  (int64_t)uVar2, *(LPCWSTR *)((uint8_t *)obj + 0x78));
    SendMessageW((HWND)plVar3[4], 0x46a, 0, 0);
}

/* ========== PECMD_ReadFileToWide @0x140068034 ==========
 * 读取整个文件并转换为宽字符串。
 */
void PECMD_ReadFileToWide(LPCWSTR path, int64_t *out)
{
    HANDLE hFile = (HANDLE)0;
    PECMD_OpenFileHandle(&hFile, path, 0x80000000, 7, NULL, 3, 0x80, (HANDLE)0);

    int64_t lVar2 = 0;
    if ((hFile != (HANDLE)0) && (hFile != INVALID_HANDLE_VALUE)) {
        lVar2 = FUN_1400E693C(hFile);
        if (lVar2 < 0) {
            lVar2 = 0;
        }
    }

    char *local_res20 = NULL;
    PECMD_AllocSmallObject((uint64_t *)&local_res20);
    if (0 < lVar2) {
        DWORD DVar1 = (DWORD)lVar2;
        PECMD_GrowByteBuffer((uint64_t *)&local_res20, (int64_t)(int)(DVar1 + 5));
        DWORD DVar3 = 0;
        ReadFile(hFile, local_res20, DVar1, &DVar3, NULL);
        if (lVar2 < (int64_t)DVar3) {
            DVar3 = DVar1;
        }
        local_res20[(int)DVar3] = '\0';
    }

    FUN_1400637DC((WCHAR **)out, local_res20, -1, -1);
    FUN_14005B104((WCHAR **)&local_res20);

    if ((hFile != (HANDLE)0) && (hFile != INVALID_HANDLE_VALUE)) {
        CloseHandle(hFile);
    }
}

/* ========== PECMD_QueryDeviceInfo @0x140069464 ==========
 * 查询磁盘几何/分区格式信息。
 */
uint32_t PECMD_QueryDeviceInfo(LPCWSTR path, uint32_t *outType, uint64_t *outSize)
{
    HANDLE hDevice = (HANDLE)0;
    if (outSize != NULL) {
        *outSize = (uint64_t)-1;
    }
    PECMD_OpenFileHandle(&hDevice, path, 0x100080, 3, NULL, 3, 0x20000000, (HANDLE)0);

    if (hDevice != (HANDLE)0) {
        uint32_t local_1a8 = 0;
        uint32_t local_1a4 = 0xffffffff;
        uint32_t local_1a0 = 0;
        DWORD bytesReturned = 0;
        BOOL BVar2 = DeviceIoControl(hDevice, 0x2d1080, NULL, 0,
                                     &local_1a8, 0xc, &bytesReturned, NULL);
        uint32_t uVar1 = local_1a4;
        if (BVar2 != 0) {
            if (outType != NULL) {
                *outType = local_1a8;
            }
            if (outSize != NULL) {
                *(uint32_t *)((uint8_t *)outSize + 4) = local_1a4;
                *(uint32_t *)outSize = local_1a0;
            }
            if (hDevice != INVALID_HANDLE_VALUE) {
                CloseHandle(hDevice);
            }
            return uVar1;
        }
        if (hDevice != INVALID_HANDLE_VALUE) {
            CloseHandle(hDevice);
        }
    }
    return 0xffffffff;
}

/* ========== FUN_140069BD8 @0x140069bd8 ==========
 * 在设备布局数组中查找分区/扩展信息。
 */
uint32_t FUN_140069BD8(int64_t layout, char type, uint32_t count,
                                   int64_t *outStart, int64_t *outEnd, int sector,
                                   uint32_t result, int64_t *outSize)
{
    int iVar5 = 0;
    if (0 < (int)count) {
        int64_t *plVar4 = (int64_t *)((uint8_t *)layout + 8);
        do {
            if (type == '\0') {
                if ((char)plVar4[3] == 'B') {
                    return result;
                }
            } else {
                char *pcVar1 = (char *)((uint8_t *)layout + 0x20 +
                                        (int64_t)iVar5 * 0x90);
                int iVar2 = FUN_14005B184(pcVar1, (int64_t)(uintptr_t)g_b24d10, 0x10);
                if (iVar2 == 0) {
                    *outStart = (int64_t)sector + *plVar4;
                    *outSize = plVar4[1] - (int64_t)sector;
                } else {
                    iVar2 = FUN_14005B184(pcVar1, (int64_t)(uintptr_t)g_b24d20, 0x10);
                    if (iVar2 == 0) {
                        *outEnd = *plVar4 / (int64_t)sector;
                    }
                }
                if ((*outStart != 0) && (*outEnd != 0)) {
                    return result;
                }
            }
            iVar5++;
            plVar4 += 0x12;
        } while (iVar5 < (int)count);
    }
    return 0xffffffff;
}

/* ========== PECMD_ExpandDriveList @0x14006aa9c ==========
 * 展开盘符列表/范围到连续字母序列。
 */
void PECMD_ExpandDriveList(uint8_t *out, uint16_t *start, uint16_t *end)
{
    uint16_t uVar4 = *start & 0xffdf;
    memset(out, 0, 0x1b);

    uint16_t *local_res10 = start + 1;
    if (*local_res10 == 0x3a) {
        local_res10 = start + 2;
    }
    FUN_14005B154((WCHAR **)&local_res10);

    uint16_t uVar2 = *local_res10;
    char cVar3 = (char)uVar4;
    *(char *)out = cVar3;

    if ((uint16_t)((uVar2 & 0xffdf) - 0x41) < 0x1a) {
        int64_t lVar6 = 1;
        do {
            if (end <= local_res10) {
                return;
            }
            uVar2 = *local_res10 & 0xffdf;
            if (((uint16_t)(uVar2 - 0x41) < 0x1a) && (uVar4 != uVar2)) {
                *(char *)(lVar6 + (int64_t)(uintptr_t)out) = (char)uVar2;
                lVar6++;
                uVar4 = uVar2;
            }
            local_res10++;
        } while (lVar6 < 0x1a);
    } else {
        uVar2 = 0x5a;
        if ((*local_res10 == 0x2d) &&
            ((uint16_t)((local_res10[1] & 0xffdf) - 0x41) < 0x1a)) {
            uVar2 = local_res10[1] & 0xffdf;
        }
        char cVar5 = '\x01';
        char cVar7 = cVar3;
        char cVar1 = (char)uVar2;
        if (uVar2 < uVar4) {
            cVar5 = -1;
            cVar7 = (char)uVar2;
            cVar1 = cVar3;
        }
        int64_t lVar6 = 0;
        do {
            if ((int)cVar1 < (int)(uint32_t)uVar4) {
                return;
            }
            if ((int)(uint32_t)uVar4 < (int)cVar7) {
                return;
            }
            *(char *)(lVar6 + (int64_t)(uintptr_t)out) = (char)uVar4;
            lVar6++;
            uVar4 = (uint16_t)(uVar4 + (uint16_t)cVar5);
        } while (lVar6 < 0x1a);
    }
}

/* ========== FUN_14006ABB8 @0x14006abb8 ==========
 * 获取设备布局信息并计算偏移/大小。
 */
uint64_t *FUN_14006ABB8(HANDLE hDevice, uint64_t *info, uint32_t *count,
                                  uint8_t *outType, uint32_t flags)
{
    uint64_t *puVar3 = FUN_14005FEAC(hDevice, (uint64_t *)*info, count);
    if (puVar3 != NULL) {
        bool bVar5 = (int)*puVar3 == 1;
        uint32_t uVar1 = (uint32_t)FUN_14005F96C(hDevice, 0x200);
        LARGE_INTEGER local_30;
        uint64_t local_38 = 0;
        uint64_t local_res10 = 0;
        local_30.QuadPart = 0;

        uint32_t uVar2 = FUN_140069BD8((int64_t)(uintptr_t)(puVar3 + 6),
                            (char)bVar5, *count, &local_30.QuadPart,
                            (int64_t *)&local_38, (int)uVar1, (uint32_t)*puVar3,
                            (int64_t *)&local_res10);
        *outType = (uint8_t)uVar2;
        if (-1 < (char)(uint8_t)uVar2) {
            puVar3[6] = local_res10;
            int64_t lVar4 = PECMD_ParsePartitionTable(hDevice, (int64_t *)(puVar3 + 6),
                                          (int)(bVar5 | uVar1), local_30,
                                          local_38, flags);
            *count = (uint32_t)lVar4;
            if ((int)(uint32_t)lVar4 < 0) {
                *outType = (uint8_t)(*outType | 0x10);
            }
        }
    }
    return puVar3;
}

/* ========== FUN_14006B6E8 @0x14006b6e8 ==========
 * 查找空闲对象槽；找不到时追加新槽。
 */
uint64_t FUN_14006B6E8(int64_t obj)
{
    EnterCriticalSection((void *)&g_csInit);
    uint64_t uVar5 = 0;

    if (*(int *)((uint8_t *)obj + 0x1d0) == 0) {
        int iVar1 = **(int **)((uint8_t *)obj + 0x1a8);
        int iVar4 = 0;
        if (0 < iVar1) {
            uint64_t *puVar3 = *(uint64_t **)(**(uint64_t **)((uint8_t *)obj + 0x1a0));
            do {
                iVar4 = (int)uVar5;
                if ((void *)*puVar3 == (void *)&DAT_14013d660) {
                    *(void **)(**(uint64_t **)((uint8_t *)obj + 0x1a0) +
                               (uint64_t)iVar4 * 8) = (void *)&g_u8D5D0;
                    uVar5 = **(uint64_t **)((uint8_t *)obj + 0x1a0) +
                            (uint64_t)iVar4 * 8;
                    goto done;
                }
                uVar5 = (uint64_t)(iVar4 + 1U);
                puVar3++;
            } while ((int)(iVar4 + 1U) < iVar1);
        }

        int64_t *plVar2 = FUN_140063118(
            (void *)**(uint64_t **)((uint8_t *)obj + 0x1a0),
            (size_t)((int64_t)(iVar1 + 1) << 3));
        **(uint64_t **)((uint8_t *)obj + 0x1a0) = (uint64_t)(uintptr_t)plVar2;
        *(void **)(**(uint64_t **)((uint8_t *)obj + 0x1a0) +
                   (uint64_t)**(int **)((uint8_t *)obj + 0x1a8) * 8) =
            (void *)&g_u8D5D0;
        **(int **)((uint8_t *)obj + 0x1a8) =
            **(int **)((uint8_t *)obj + 0x1a8) + 1;
        uVar5 = **(uint64_t **)((uint8_t *)obj + 0x1a0) +
                (uint64_t)(**(int **)((uint8_t *)obj + 0x1a8) + -1) * 8;
    }
done:
    LeaveCriticalSection((void *)&g_csInit);
    return uVar5;
}

/* ========== FUN_1400FE4A4 @0x14006b7f0 ==========
 * 应用控件属性更新（1/3/4/5 类型）。
 */
uint64_t FUN_1400FE4A4(int64_t obj, int64_t *action)
{
    if (*action == 1) {
        if (action[1] == 0) {
            return FUN_14006B6E8(obj);
        }
        return 0;
    }

    if (*action == 4) {
        int iVar5 = 0;
        if (0 < **(int **)((uint8_t *)obj + 0x1a8)) {
            int64_t *plVar3 = *(int64_t **)(**(uint64_t **)((uint8_t *)obj + 0x1a0));
            do {
                if (action[2] == *plVar3) {
                    (*(int64_t **)(**(uint64_t **)((uint8_t *)obj + 0x1a0)))[iVar5] =
                        action[3];
                    break;
                }
                iVar5++;
                plVar3++;
            } while (iVar5 < **(int **)((uint8_t *)obj + 0x1a8));
        }
    } else if (*action == 3) {
        return (uint64_t)FUN_1400F2384(action[2], (LPCWSTR)action[3],
                                       (int64_t *)action[4], action[5], 0, 1);
    } else if (*action == 5) {
        int64_t lVar1 = action[3];
        COLORREF color = *(COLORREF *)(action + 2);
        HGDIOBJ ho = *(HGDIOBJ *)((uint8_t *)lVar1 + OBJ_BRUSH);
        if (ho != (HGDIOBJ)0) {
            *(uint64_t *)((uint8_t *)lVar1 + OBJ_BRUSH) = 0;
            DeleteObject(ho);
        }
        if (-1 < (int)color) {
            HBRUSH pHVar4 = CreateSolidBrush(color);
            *(HBRUSH *)((uint8_t *)lVar1 + OBJ_BRUSH) = pHVar4;
            InvalidateRect(*(HWND *)((uint8_t *)lVar1 + OBJ_HWND), NULL, 1);
        }
    }
    return 1;
}

/* ========== FUN_1400705AC @0x1400705ac ==========
 * 在 COM 临界区中调用 COM 方法。
 */
int64_t FUN_1400705AC(int64_t *obj, uint64_t a2, uint32_t a3,
                              uint64_t a4, uint64_t a5, uint32_t a6,
                              uint32_t a7, uint64_t a8)
{
    int64_t lVar2 = 0;
    if ((int64_t)g_pComWrite < 2) {
        return 0;
    }

    EnterCriticalSection((void *)&g_csCom);
    FUN_140061C44();
    int iVar1 = -3;
    if (g_pOleInit != NULL) {
        iVar1 = ((int (*)(int))g_pOleInit)(0);
        if ((iVar1 != 0) && (iVar1 == 1)) {
            ((void (*)(void))g_pOleUninit)();
        }
    }

    lVar2 = ((int64_t (*)(int64_t *, int, uint64_t, uint32_t, uint64_t, uint64_t,
                          uint32_t, uint32_t, uint64_t))g_pComWrite)
                (obj + 0x1a, 1, a2, a3, a4, a5,
                 a6, a7, a8);
    obj[4] = lVar2;
    PECMD_ContainerAppend(obj);
    lVar2 = obj[4];

    if ((g_pOleUninit != NULL) && (iVar1 == 0)) {
        ((void (*)(void))g_pOleUninit)();
    }
    LeaveCriticalSection((void *)&g_csCom);
    return lVar2;
}

/* ========== PECMD_ReadConnectionRegistryValue @0x140072814 ==========
 * 读取网卡 Connection 注册表值。
 */
void PECMD_ReadConnectionRegistryValue(LPCSTR name, int64_t *out, LPCSTR value)
{
    LPCSTR local_38 = NULL;
    FUN_14007026C((void **)&local_38,
        "SYSTEM\\CurrentControlSet\\Control\\Network\\{4D36E972-E325-11CE-BFC1-08002BE10318}\\");
    FUN_14006345C((int64_t *)&local_38, name);
    FUN_14006345C((int64_t *)&local_38, "\\Connection");

    DWORD local_res10[2] = {0x7f8, 0};
    PECMD_AllocString((WCHAR **)out, 0x7fc);
    DWORD local_res20[2] = {0, 0};
    *(uint16_t *)*out = 0;

    int64_t lVar1 = *out;
    int64_t local_28[2] = {0, 0};
    int64_t local_30 = 0;
    int64_t *plVar3 = FUN_1400702F0(local_28, value, (uint64_t)-1);
    int64_t lVar2 = *plVar3;
    plVar3 = FUN_1400702F0(&local_30, local_38, (uint64_t)-1);
    ((void (*)(int64_t, int64_t, int64_t, void *, int64_t, void *))g_pSHGetValueW)
        ((int64_t)(intptr_t)0xffffffff80000002, *plVar3, lVar2,
         local_res20, lVar1, local_res10);

    FUN_14005B104((WCHAR **)&local_30);
    FUN_14005B104((WCHAR **)local_28);
    *(uint16_t *)(*out + (int64_t)local_res10[0] * 2) = 0;
    FUN_14005B104((WCHAR **)&local_38);
}

/* ========== FUN_140072D8C @0x140072d8c ==========
 * 按类型或名称查找网卡信息。
 */
uint32_t FUN_140072D8C(uint8_t *mac, int64_t *out, int mode)
{
    FUN_14005C828("GetAdaptersInfo", "Iphlpapi.DLL", (int64_t *)(void **)&g_pGetAdaptersInfo, NULL);
    if (g_pGetAdaptersInfo == NULL) {
        return 0;
    }

    int64_t local_res20 = 0;
    uint32_t uVar2 = 0xffffffff;
    int64_t *plVar4 = (int64_t *)PECMD_GetCachedBlock();
    if (plVar4 != NULL) {
        int64_t lVar5 = 6;
        do {
            if (mode == 0) {
                if (*(int *)((uint8_t *)plVar4 + 0x194) == (int)lVar5) {
                    int iVar3 = FUN_14005B184((char *)(plVar4 + 0x33),
                                              (int64_t)(uintptr_t)mac, lVar5);
                    if (iVar3 == 0) {
                        FUN_1400637DC((WCHAR **)out,
                                          (LPCSTR)((uint8_t *)plVar4 + 0xc), -1, -1);
                        uVar2 = *(uint32_t *)((uint8_t *)plVar4 + 0x1a4);
                        goto done;
                    }
                }
            } else {
                bool bVar1 = PECMD_PrefixMatchNoCase((uint16_t *)*out,
                                           (char *)((uint8_t *)plVar4 + 0xc));
                if (bVar1) {
                    memcpy(mac, (void *)(plVar4 + 0x33), (int)lVar5);
                    uVar2 = *(uint32_t *)((uint8_t *)plVar4 + 0x1a4);
                    goto done;
                }
            }
            plVar4 = (int64_t *)*plVar4;
        } while (plVar4 != NULL);
    }

    if (mode == 0) {
        *(uint16_t *)*out = 0;
    }
done:
    FUN_14005B104((WCHAR **)&local_res20);
    return uVar2;
}

/* ========== FUN_14007724C @0x14007724c ==========
 * 用指定参数启动当前程序的新实例。
 */
void FUN_14007724C(LPWSTR cmdline)
{
    WCHAR *local_res10[3] = {NULL, NULL, NULL};
    PECMD_AllocWStringBuffer((WCHAR **)local_res10, 0x1cc);
    GetModuleFileNameW((HMODULE)0, local_res10[0], 0x104);

    PROCESS_INFORMATION local_98;
    STARTUPINFOW local_78;
    memset(&local_98, 0, sizeof(local_98));
    memset(&local_78, 0, sizeof(local_78));
    local_78.cb = sizeof(local_78);
    local_78.lpReserved = NULL;
    memset(&local_78.lpDesktop, 0, 0x58);
    local_78.dwFlags = 0x81;
    local_78.wShowWindow = 0;
    local_78.dwXCountChars = 800;
    local_78.dwYCountChars = 500;

    CreateProcessW(local_res10[0], cmdline, NULL, NULL, 0, 0, NULL, NULL,
                   &local_78, &local_98);
    if (local_98.hProcess != (HANDLE)0) {
        CloseHandle(local_98.hProcess);
    }
    if (local_98.hThread != (HANDLE)0) {
        CloseHandle(local_98.hThread);
    }
    FUN_14005B104((WCHAR **)local_res10);
}

/* ========== PECMD_CloseDeviceHandle @0x140078408 ==========
 * 按标志关闭设备句柄并执行后续处理。
 */
int64_t PECMD_CloseDeviceHandle(int64_t *handle, int dev, uint8_t flags,
                                  uint8_t *state, int64_t *err, LPCWSTR path)
{
    int iVar2 = 0;

    if ((flags & 0x11) == 0x11) {
        if (((g_flagD6F5 & 0x20) == 0) && ((*state & 0x20) == 0)) {
            PECMD_LockVolumeByDevice(dev, 1);
        }
        *state = (uint8_t)(*state | 2);
        if ((g_flagD6F5 & 0x10) == 0) {
            PECMD_ErrorHandlerWrap(state, dev, 0, -1, 1, NULL);
        }
        *state = (uint8_t)(*state | 1);
    }

    if ((flags & 2) != 0) {
        DWORD DVar1 = PECMD_FlushDeviceIoctl((HANDLE)*handle);
        if ((flags & 4) != 0) {
            *err = (int64_t)(int)DVar1;
        }
    }

    if ((*handle != 0) && (*handle != -1)) {
        CloseHandle((HANDLE)*handle);
    }
    *handle = 0;

    if ((flags & 8) != 0) {
        PECMD_ClearFlagAndError(state);
        if ((flags & 0x10) != 0) {
            iVar2 = PECMD_InstallDevice(path,
                        (uint32_t)(-(uint32_t)((flags & 0x80) != 0) & 0x17) + 1U);
        }
    }

    if ((flags & 0x40) == 0) {
        return 0xffffffff;
    }
    if ((flags & 0x20) != 0) {
        return (int64_t)iVar2;
    }
    return *err;
}

/* ========== FUN_140078D8C @0x140078d8c ==========
 * 将字节串按 Base64 编码为宽字符串。
 */
uint64_t FUN_140078D8C(uint8_t *src, uint64_t *out)
{
    int iVar2 = lstrlenA((LPCSTR)src);
    char *local_res8 = NULL;
    FUN_1400633A8((void **)&local_res8, (int64_t)((iVar2 + 1) * 3));

    uint64_t uVar4 = 0;
    char *pCVar5 = local_res8 + 1;
    *local_res8 = ' ';
    *pCVar5 = '\0';
    uint8_t bVar3 = 0;
    uint64_t uVar6 = 0;

    while (1) {
        if (*src == 0) {
            *pCVar5 = '\0';
            FUN_1400637DC((WCHAR **)out, local_res8, -1, 0xfde9);
            uVar4 = *out;
            break;
        }

        uint8_t bVar1 = *src;
        if (bVar1 == 0x3d) {
            if ((char)bVar3 > 0) {
                *pCVar5 = (char)uVar6;
                bVar3 = 0;
                pCVar5++;
                uVar6 = uVar4;
            }
        } else {
            if ((bVar1 < 0x41) || (0x5a < bVar1)) {
                if ((bVar1 < 0x61) || (0x7a < bVar1)) {
                    if ((bVar1 < 0x30) || (0x39 < bVar1)) {
                        if (bVar1 == 0x2b) {
                            bVar1 = 0x3e;
                        } else {
                            if (bVar1 != 0x2f) {
                                *(uint16_t *)*out = 0;
                                goto done;
                            }
                            bVar1 = 0x3f;
                        }
                    } else {
                        bVar1 = (uint8_t)(bVar1 + 4);
                    }
                } else {
                    bVar1 = (uint8_t)(bVar1 + 0xb9);
                }
            } else {
                bVar1 = (uint8_t)(bVar1 + 0xbf);
            }

            uVar6 = (uint64_t)((uint32_t)uVar6 | (uint32_t)bVar1 << (bVar3 & 0x1f));
            bVar3 = (uint8_t)(bVar3 + 6);
            while (((char)bVar3 > 7) || (((char)bVar3 > 0) && (*src == 0))) {
                *pCVar5 = (char)uVar6;
                pCVar5++;
                uVar6 >>= 8;
                bVar3 = (uint8_t)(bVar3 - 8);
            }
        }
        src++;
    }

done:
    FUN_14005B104((WCHAR **)&local_res8);
    return uVar4;
}

/* ========== FUN_140079E34 @0x140079e34 ==========
 * 按分区类型查询分区起始偏移。
 */
uint64_t FUN_140079E34(HANDLE hDevice, int type, uint64_t *outOffset)
{
    void *local_30 = NULL;
    DWORD local_38[2] = {0, 0};
    FUN_1400633A8(&local_30, 0x100000);

    uint8_t out[0x18];
    memset(out, 0, sizeof(out));
    BOOL BVar1 = DeviceIoControl(hDevice, 0x70000, NULL, 0, out, 0x18,
                                 local_38, NULL);
    int local_14 = *(int *)(out + 0x14);

    if ((BVar1 != 0) && (0 < local_14)) {
        uint32_t local_res20[2] = {0, 0};
        uint64_t *puVar2 = FUN_14005FEAC(hDevice, (uint64_t *)local_30,
                                               local_res20);
        if (*(int *)((uint8_t *)puVar2 + 4) < type) {
            uint64_t uVar5 = 0xffffffff80070057ULL;
            FUN_14005B104((WCHAR **)&local_30);
            return uVar5;
        }

        if (0 < (int)local_res20[0]) {
            uint64_t *puVar3 = puVar2 + 9;
            int iVar4 = 0;
            do {
                if (type == (int)*puVar3) {
                    *outOffset = puVar2[(int64_t)iVar4 * 0x12 + 8];
                    uint64_t uVar5 = puVar2[(int64_t)iVar4 * 0x12 + 7];
                    FUN_14005B104((WCHAR **)&local_30);
                    return uVar5;
                }
                iVar4++;
                puVar3 += 0x12;
            } while (iVar4 < (int)local_res20[0]);
        }
    }

    FUN_14005B104((WCHAR **)&local_30);
    return 0xffffffffffffffffULL;
}

/* ========== FUN_14007D0AC @0x14007d0ac ==========
 * 设置变量；按需要补 & 前缀并在全局脚本上执行。
 */
void FUN_14007D0AC(int64_t *script, LPCWSTR name, LPCWSTR value)
{
    int16_t sVar2 = 0;
    if ((*name == L'#') && (name[1] == L'&')) {
        name++;
        sVar2 = 0x23;
    }

    if (((*name != L'&') && ((*(uint8_t *)((uint8_t *)script + 0xd) & 0xf) == 0)) &&
        ((script[8] == 0) || (**(int16_t **)(script[8] + 0x128) != 0x26))) {
        if (*name == L'\0') {
            return;
        }
        FUN_14005D534(script, name, value);
        return;
    }

    LPCWSTR local_res10 = NULL;
    LPCWSTR pWVar3 = NULL;
    bool direct = false;
    if (sVar2 == 0) {
        if (*name == L'&') {
            if (name[1] == L'&') {
                direct = true;
            } else {
                pWVar3 = WSTR("&");
            }
        } else {
            pWVar3 = WSTR("&&");
        }
    } else {
        if (*name == L'&') {
            direct = true;
        } else {
            pWVar3 = WSTR("&");
        }
    }

    if (!direct) {
        FUN_1400703E4((WCHAR **)&local_res10, pWVar3);
        FUN_14006375C((WCHAR **)&local_res10, name);
        name = local_res10;
    }

    if ((int64_t *)script[0x18] != NULL) {
        script = (int64_t *)script[0x18];
    }
    EnterCriticalSection((void *)&g_csInit);
    FUN_14001E6BC(script, name, value, -1);
    LeaveCriticalSection((void *)&g_csInit);
    FUN_14005B104((WCHAR **)&local_res10);
}

/* ========== FUN_14007D340 @0x14007d340 ==========
 * 使用 COM 图像编码器将图像写入流。
 */
uint32_t FUN_14007D340(int64_t obj, uint64_t stream,
                                   WCHAR *mime, int flags)
{
    int64_t lVar4 = 0;
    if ((mime == NULL) || (*mime == L'\0')) {
        mime = (WCHAR *)WSTR("image/jpeg");
    }

    uint32_t uVar2 = 0x80004001;
    PECMD_LoadIcon((LPCWSTR)(uintptr_t)-1, NULL);
    if (g_pGdipSaveImageToFile != NULL) {
        int64_t local_68 = 0;
        int64_t lVar1 = obj;
        if (flags == 0) {
            ((void (*)(int64_t, int, void *))g_pGdipCreateBitmapFromHBITMAP)(obj, 0, &local_68);
            lVar4 = local_68;
            lVar1 = local_68;
        }
        local_68 = lVar1;

        uint64_t local_60[2] = {0, 0};
        int64_t uVar3 = PECMD_GetConfigEntryByName(mime, local_60);
        uVar2 = 0x80004001;
        if (-1 < (int)uVar3) {
            uint32_t local_res18[4] = {0, 0, 0, 0};
            uint32_t local_50[2] = {0, 0};
            local_50[0] = 1;
            local_res18[0] = 100;
            (void)local_res18;
            uVar2 = ((uint32_t (*)(int64_t, uint64_t, uint64_t *, uint32_t *))g_pGdipSaveImageToFile)
                        (local_68, stream, local_60, local_50);
        }

        if (lVar4 != 0) {
            ((void (*)(int64_t))g_pGdipDisposeImage)(lVar4);
        }
    }
    return uVar2;
}

/* ========== FUN_14008293C @0x14008293c ==========
 * 解析一个命令行参数（引号/空白），并展开 %var%。
 */
void FUN_14008293C(int64_t *script, int64_t *pp, int64_t *start,
                           int64_t *out)
{
    int64_t lVar3 = 0;
    *start = *pp;
    uint16_t *puVar1 = (uint16_t *)*pp;

    if (*puVar1 == 0x22) {
        *pp = (int64_t)(uintptr_t)(puVar1 + 1);
        *start = *pp;
        FUN_1400F429C((WCHAR **)pp, 0x22);
        if (*(int16_t *)*pp != 0) {
            *(int16_t *)*pp = 0;
            *pp += 2;
        }
    } else {
        for (;;) {
            while ((*pp = (int64_t)(uintptr_t)puVar1, *puVar1 != 0 &&
                   (((puVar1 = (uint16_t *)*pp, *puVar1 < 9) || (0xd < *puVar1)) &&
                    (*puVar1 != 0x20)))) {
                puVar1++;
            }
            uint16_t *local_res10 = (uint16_t *)*pp;
            FUN_14005B154((WCHAR **)&local_res10);
            if (lVar3 == 0) {
                lVar3 = *pp;
            }
            if ((*local_res10 == 0) || (*local_res10 == 0x2d)) {
                break;
            }
            puVar1 = local_res10;
        }
        if (*(int16_t *)*pp == 0) {
            *pp = lVar3;
        }
    }

    if (*(int16_t *)*pp != 0) {
        *(int16_t *)*pp = 0;
        *pp += 2;
    }
    FUN_14005B154((WCHAR **)pp);

    LPWSTR pWVar2 = StrChrW((LPCWSTR)*start, L'%');
    if (pWVar2 != NULL) {
        FUN_14007BF44(script, (WCHAR *)*start, (void *)out, 0, 1);
        *start = *out;
    }
}

/* ========== FUN_14009CFBC @0x14009cfbc ==========
 * 保存下拉框/编辑框选择状态到变量。
 */
void FUN_14009CFBC(int64_t obj, int mode)
{
    int64_t lVar1 = *(int64_t *)((uint8_t *)obj + 0x38);
    LRESULT LVar3 = SendMessageW(*(HWND *)((uint8_t *)lVar1 + OBJ_HWND), 0x147, 0, 0);

    if ((mode != 0) && (-1 < (int)LVar3)) {
        LPCWSTR local_res8 = NULL;
        PECMD_AllocWStringBuffer((WCHAR **)&local_res8, 0);
        FUN_14006C4C8(lVar1, (int)LVar3, (LPARAM *)&local_res8);
        if (**(LPCWSTR *)((uint8_t *)obj + 0x10) != L'\0') {
            FUN_14007D0AC(*(int64_t **)((uint8_t *)obj + 0x50),
                                 *(LPCWSTR *)((uint8_t *)obj + 0x10), local_res8);
        }
        FUN_14005B104((WCHAR **)&local_res8);
    }

    int iVar2 = lstrlenW(*(LPCWSTR *)((uint8_t *)obj + 0x10));
    LPCWSTR local_res8 = NULL;
    PECMD_AllocWStringBuffer((WCHAR **)&local_res8, (int64_t)(iVar2 + 8));
    memcpy((void *)local_res8, *(void **)((uint8_t *)obj + 0x10),
           iVar2 * 2);
    memcpy((void *)(local_res8 + iVar2), (void *)WSTR(".isel"), 0xc);

    WCHAR local_48[24];
    wsprintfW(local_48, WSTR("%d"), (int)LVar3);
    if (**(int16_t **)((uint8_t *)obj + 0x10) != 0) {
        FUN_14007D0AC(*(int64_t **)((uint8_t *)obj + 0x50),
                             local_res8, local_48);
    }
    FUN_14005B104((WCHAR **)&local_res8);
}

/* ========== FUN_1400A3F08 @0x1400a3f08 ==========
 * 设置 RANDOM 或指定名称的随机数变量。
 */
uint64_t FUN_1400A3F08(int64_t *script, WCHAR *text)
{
    WCHAR *local_res10 = text;
    FUN_14005B154((WCHAR **)&local_res10);
    FUN_14005B154((WCHAR **)&local_res10);

    WCHAR WVar3 = *local_res10;
    WCHAR *pWVar1 = local_res10;
    LPCWSTR pwVar4 = WSTR("RANDOM");
    if (WVar3 != L'\0') {
        do {
            if (((8 < (uint16_t)WVar3) && ((uint16_t)WVar3 < 0xe)) || (WVar3 == L' ')) {
                break;
            }
            pWVar1++;
            WVar3 = *pWVar1;
        } while (WVar3 != L'\0');
        *pWVar1 = L'\0';
        pwVar4 = local_res10;
    }

    LPCWSTR local_res8 = NULL;
    PECMD_AllocWStringBuffer((WCHAR **)&local_res8, 0);
    if ((*(char *)((uint8_t *)script + 0xda) == '\0') &&
        ((*(uint8_t *)((uint8_t *)script + 0xd) & 0xf) == 0)) {
        FUN_14007BDA8(script, pwVar4, (WCHAR **)&local_res8, 0, 1);
    } else {
        FUN_14007A224(script, pwVar4, (WCHAR **)&local_res8, 0, 1);
    }

    uint64_t uVar2 = PECMD_GetRandom();
    WCHAR local_88[64];
    FUN_1400E6D74(local_88, uVar2);
    FUN_1400629B8(script, local_res8, local_88);
    FUN_14005B104((WCHAR **)&local_res8);
    return 0;
}

/* ========== FUN_1400A40E8 @0x1400a40e8 ==========
 * 解析 <a,b,c,d,e> 形式的 5 个数值参数。
 */
uint64_t FUN_1400A40E8(uint64_t *pp, int64_t *out1, int64_t *out2,
                              int64_t *out3, int64_t *out4)
{
    uint16_t *psVar2 = (uint16_t *)*pp;
    uint16_t *puVar4 = NULL;

    if ((psVar2 != NULL) && (*psVar2 == 0x3c)) {
        uint16_t *local_res8 = psVar2 + 1;
        if (((*local_res8 | 0x20) < 0x61) || (0x7a < (*local_res8 | 0x20))) {
            *pp = (uint64_t)(uintptr_t)local_res8;
            uint16_t *puVar3 = local_res8;
            uint16_t *puVar5 = NULL;
            uint16_t uVar1;
            for (;;) {
                puVar5 = puVar3;
                if (*puVar5 == 0) {
                    break;
                }
                uVar1 = *puVar5;
                *pp = (uint64_t)(uintptr_t)(puVar5 + 1);
                puVar3 = puVar5 + 1;
                if (uVar1 == 0x3e) {
                    *puVar5 = 0;
                    puVar4 = puVar5;
                    break;
                }
            }

            PECMD_ParseNumSkipChar_0248((int64_t *)&local_res8, out1);
            if (*out1 < 0) {
                *out1 = 0;
            }
            PECMD_ParseNumSkipChar_0248((int64_t *)&local_res8, out2);
            if (*out2 < 0) {
                *out2 = 0;
            }
            PECMD_ParseNumSkipChar_0248((int64_t *)&local_res8, out3);
            if (*out3 < 0) {
                *out3 = 0;
            }
            PECMD_ParseNumSkipChar_0248((int64_t *)&local_res8, out4);
            if (*out4 < 0) {
                *out4 = 0;
            }
            if (puVar4 != NULL) {
                *puVar4 = 0x3e;
            }
            return 1;
        }
    }
    return 0;
}

/* ========== FUN_1400AA7E4 @0x1400aa7e4 ==========
 * 创建 0x78 控件对象（变体 C）。
 */
void FUN_1400AA7E4(WPARAM mgr, int64_t v2, uint64_t *p3,
                                int x, int y, int w, int h,
                                uint32_t flags, LPWSTR text, uint32_t extra)
{
    int local_res20[2] = {0, 0};
    local_res20[0] = x;
    uint64_t *puVar2 = (uint64_t *)PECMD_SendCtrlMessage_0834(mgr, *p3);
    uint64_t *puVar4 = NULL;

    if (puVar2 != NULL) {
        int64_t lVar1 = **(int64_t **)((uint8_t *)mgr + 0x1a0);
        FUN_14005DAF8((int64_t)mgr, local_res20, &y, &w, &h);
        void *puVar3 = calloc(1, 0x78);
        if (puVar3 != NULL) {
            puVar4 = (uint64_t *)PECMD_ConstructTreeView(puVar3, v2,
                        (int)(((uint64_t)(uintptr_t)puVar2 - (uint64_t)lVar1) >> 3) + 0x1000,
                        p3, local_res20[0], y, w, h,
                        flags, text, (uint8_t)extra);
        }
        *puVar2 = (uint64_t)(uintptr_t)puVar4;
    }
}

/* ========== FUN_1400AAA4C @0x1400aaa4c ==========
 * 创建 0x70 控件对象（变体 D）。
 */
void FUN_1400AAA4C(WPARAM mgr, int64_t v2, uint64_t *p3,
                                int x, int y, int w, int h,
                                uint16_t *p8, uint64_t *p9, uint32_t flags)
{
    int local_res20[2] = {0, 0};
    local_res20[0] = x;
    uint64_t *puVar2 = (uint64_t *)PECMD_SendCtrlMessage_0834(mgr, *p3);
    uint64_t *puVar4 = NULL;

    if (puVar2 != NULL) {
        int64_t lVar1 = **(int64_t **)((uint8_t *)mgr + 0x1a0);
        FUN_14005DAF8((int64_t)mgr, local_res20, &y, &w, &h);
        void *puVar3 = calloc(1, 0x70);
        if (puVar3 != NULL) {
            puVar4 = (uint64_t *)PECMD_ConstructSlider(puVar3, v2,
                        (int)(((uint64_t)(uintptr_t)puVar2 - (uint64_t)lVar1) >> 3) + 0x1000,
                        p3, local_res20[0], y, w, h,
                        p8, p9, flags);
        }
        *puVar2 = (uint64_t)(uintptr_t)puVar4;
        FUN_14005D9A8((int64_t)mgr, 0);
    }
}

/* ========== FUN_1400B2A4C @0x1400b2a4c ==========
 * 创建 0x90 控件对象（变体 E）。
 */
void FUN_1400B2A4C(WPARAM mgr, LPCWSTR text, uint64_t *p3,
                                int x, int y, int w, int h,
                                int16_t *p8, uint32_t flags)
{
    int local_res20[2] = {0, 0};
    local_res20[0] = x;
    uint64_t *puVar2 = (uint64_t *)PECMD_SendCtrlMessage_0834(mgr, *p3);
    uint64_t *puVar4 = NULL;

    if (puVar2 != NULL) {
        int64_t lVar1 = **(int64_t **)((uint8_t *)mgr + 0x1a0);
        FUN_14005DAF8((int64_t)mgr, local_res20, &y, &w, &h);
        void *puVar3 = calloc(1, 0x90);
        if (puVar3 != NULL) {
            puVar4 = (uint64_t *)PECMD_CreateDialogControl(puVar3, text,
                        (int)(((uint64_t)(uintptr_t)puVar2 - (uint64_t)lVar1) >> 3) + 0x1000,
                        p3, local_res20[0], y, w, h,
                        p8, flags, (int64_t)(uintptr_t)puVar2);
        }
        *puVar2 = (uint64_t)(uintptr_t)puVar4;
        FUN_14005D9A8((int64_t)mgr, 0);
    }
}

/* ========== FUN_1400BEF64 @0x1400bef64 ==========
 * 蜂鸣并执行一段构造的脚本文本。
 */
void FUN_1400BEF64(LPCWSTR cmd)
{
    PECMD_SetClipboardUnicode(cmd + 1);

    WCHAR *local_res8 = NULL;
    FUN_1400702B0((WCHAR **)&local_res8, cmd);

    uint8_t local_28[31];
    *(uint64_t *)(local_28 + 0) = g_u6497e8;
    *(uint64_t *)(local_28 + 8) = g_u6497f0;
    *(uint64_t *)(local_28 + 16) = g_u6497f8;
    *(uint32_t *)(local_28 + 24) = g_u329800;
    *(uint16_t *)(local_28 + 28) = g_u169804;
    *(uint8_t *)(local_28 + 30) = g_u897806;

    int iVar1 = lstrlenW(cmd);
    if (iVar1 < 0x1e) {
        int64_t local_res10[3] = {0, 0, 0};
        int64_t *plVar2 = FUN_1400702F0(local_res10,
                            (LPCSTR)(local_28 + (0x1e - iVar1)), (uint64_t)-1);
        FUN_14006375C((WCHAR **)&local_res8, (LPCWSTR)*plVar2);
        FUN_14005B104((WCHAR **)local_res10);
    }

    int64_t local_res10[3] = {0, 0, 0};
    int64_t *plVar2 = FUN_1400702F0(local_res10, (LPCSTR)g_b297d0, (uint64_t)-1);
    FUN_14006375C((WCHAR **)&local_res8, (LPCWSTR)*plVar2);
    FUN_14005B104((WCHAR **)local_res10);

    Beep(1000, 0x1e);
    FUN_1400bb718((void *)g_Script, local_res8, NULL);
    FUN_14005B104((WCHAR **)&local_res8);
}
