/* ====================================================================
 * core_b2c.c — B2 批次小函数 (140020000-14003ffff) 第三批
 *
 * 本批新实现函数全部使用人类可读 PECMD_ 名称，原始地址保留在 @0x 注释。
 *
 * 来源: PECMD原始.EXE (x64, ImageBase=0x140000000)
 *   写远程进程 WinExec 补丁  PECMD_PatchRemoteWinExec   @0x1400229f8
 *   枚举显示模式数组        PECMD_EnumDisplayModes      @0x140023338
 *   超时消息泵/钩子等待     PECMD_WaitKeyPressHooked @0x140025b10
 *   解析窗口样式关键字       FUN_140025CE0 @0x140025ce0
 *   手工映射加载 DLL        FUN_1400282D4         @0x1400282d4
 *   图形窗口子类过程         PECMD_GraphSubclassWndProc       @0x140028560
 *   即插即用驱动安装         FUN_14002B7F8      @0x14002b7f8
 *   DrvLoad 命令安装驱动     FUN_14002C634 @0x14002c634
 *   创建设备信息             FUN_14002C8A4       @0x14002c8a4
 *   线程栅栏等待             PECMD_WaitOtherThreadsExit     @0x14002ca80
 *   执行子命令               PECMD_ExecSubCommand     @0x140030144
 *   循环执行路径列表         PECMD_ExecPathList      @0x140035860
 *   发送热键消息             FUN_140035B40     @0x140035b40
 *   解压后安装驱动           PECMD_DeviFileExtractThreadProc @0x14003a810
 *   处理 DOWNCFG 关机配置    PECMD_DownCheckShutdown      @0x14003d92c
 *
 * 约定:
 *   - 新实现函数使用 PECMD_ 可读名；未实现依赖仍 extern FUN_ + TODO(verify)
 *   - 全局数据符号先用 extern 声明; 完整链接时统一在 core_globals.c 定义
 *   - 反编译中的 Ghidra 扩展宏 (SUB168/SEXT816/CONCATxx) 用等价整数运算替代
 * ==================================================================== */
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>

#include "pecmd_defs.h"
extern WCHAR **FUN_14005B154(WCHAR **pp); /* @0x14005b154 */

/* ---- 本文件需要的本地补充类型 ---- */
typedef uintptr_t HDROP;
typedef struct {
    LONG x;
    LONG y;
} PECMD_POINTL;
typedef struct {
    WCHAR dmDeviceName[32];
    WORD  dmSpecVersion;
    WORD  dmDriverVersion;
    WORD  dmSize;
    WORD  dmDriverExtra;
    DWORD dmFields;
    union {
        struct {
            SHORT dmOrientation;
            SHORT dmPaperSize;
            SHORT dmPaperLength;
            SHORT dmPaperWidth;
            SHORT dmScale;
            SHORT dmCopies;
            SHORT dmDefaultSource;
            SHORT dmPrintQuality;
        } printer;
        struct {
            PECMD_POINTL dmPosition;
            DWORD dmDisplayOrientation;
            DWORD dmDisplayFixedOutput;
        } display;
    } u;
    SHORT dmColor;
    SHORT dmDuplex;
    SHORT dmYResolution;
    SHORT dmTTOption;
    SHORT dmCollate;
    WCHAR dmFormName[32];
    WORD  dmLogPixels;
    DWORD dmBitsPerPel;
    DWORD dmPelsWidth;
    DWORD dmPelsHeight;
    DWORD dmDisplayFlags;
    DWORD dmDisplayFrequency;
    DWORD dmICMMethod;
    DWORD dmICMIntent;
    DWORD dmMediaType;
    DWORD dmDitherType;
    DWORD dmReserved1;
    DWORD dmReserved2;
    DWORD dmPanningWidth;
    DWORD dmPanningHeight;
} PECMD_DEVMODEW;
typedef struct _SP_DEVINFO_DATA {
    DWORD cbSize;
    GUID  ClassGuid;
    DWORD DevInst;
    uintptr_t Reserved;
} SP_DEVINFO_DATA;

/* ---- 已实现公共工具 (其他 core_*.c / core_globals.c) ---- */
extern void FUN_1400633A8(void **ps, int64_t len);       /* @0x1400633a8 */
extern void PECMD_ZeroLenBuf(void *p);                     /* @0x14005b0b8 */
extern void *PECMD_GrowByteBuffer(void **ps, int64_t len); /* @0x140063424 */
extern void PECMD_OpenFileHandle(HANDLE *out, LPCWSTR path, DWORD access, DWORD share,
                           LPSECURITY_ATTRIBUTES sa, DWORD disp, DWORD flags,
                           HANDLE tmpl);                /* @0x140003864 */
extern void PECMD_BubbleSort(uint8_t *arr, int64_t count, int64_t elem_size,
                          void *cmp);                   /* @0x140023268 排序 */

/* ---- 未实现依赖 (extern + TODO(verify)) ---- */
extern void FUN_14001bbac(void *script, int a, HANDLE *b, DWORD c, DWORD ms,
                          DWORD d);
extern LRESULT PECMD_KeyboardHookProc(int a1, WPARAM a2, char *a3);
extern char PECMD_AdvanceAfterPrefix(LPCWSTR s, int64_t *pp, int len);
extern int64_t FUN_14005B1A8(void *ps, int64_t *pp, int mode);
extern bool PECMD_ParseUIntValue(int64_t *a1, int *a2);
extern int64_t PECMD_ValidatePeHeader(int64_t ctx, short *path, uint64_t flags);
extern uint64_t PECMD_ComputeSectionMapSize(int64_t ctx);
extern void PECMD_CollectPESections(int64_t ctx, void *base, int64_t path);
extern int64_t PECMD_RelocateImage(int64_t ctx, int64_t base);
extern int64_t PECMD_BindDelayImports(int64_t ctx, int64_t base);
extern int64_t PECMD_FindResourceByNameOrId(int64_t ctx, LPCSTR name);
extern void PECMD_HandleDropFile(void *ctx, HDROP drop, char flag);
extern void PECMD_PatchInfDirectives(LPCWSTR path);
extern void PECMD_AppendKeyIfMissing(int64_t ctx, LPCWSTR path, int mode);
extern void FUN_140025f10(int64_t ctx, LPCWSTR msg, uint32_t code, void *p4,
                          void *p5, int64_t *p6);
extern void PECMD_WriteParamRecord(int64_t ctx, char type, LPCWSTR a, LPCWSTR b);
extern int64_t *FUN_1400637DC(int64_t *ps, LPCSTR src, uint64_t a, uint64_t b);
extern int64_t PECMD_ExecCmdDispatch(void *script, void *cmd, void *s3, void *s4,
                             uint32_t flag, void *p6, void *s7, void *p8);
extern void FUN_1400F429C(WCHAR **pp, WCHAR ch);
extern void FUN_1400639F0(int64_t *arr, int64_t *cap, int64_t *cnt, void *data,
                          int64_t esize, int32_t mode);
extern void PECMD_ResizePtrTable(int64_t *arr, int64_t count);
extern int64_t * PECMD_InitPtrTable(int64_t *arr);
extern void FUN_1400195F0(void *script, int a, int b, void *c);
extern void FUN_14002CA30(void);
extern void PECMD_ResetScriptChain(void *script, void *a2);
extern void PECMD_FreeRecordChain(int64_t ctx);
extern LPCWSTR FUN_14002FD88(int64_t *script, WCHAR *s, uint32_t *flags,
                             int64_t *out);
extern void PECMD_ShowAboutDialog(void);
extern int64_t PECMD_CreateWindow(int64_t *script, LPCWSTR a4, int64_t *a3,
                             WCHAR *a2, int flags);
extern int64_t PECMD_ExecWindowThread(int64_t *script, uint64_t a4, int64_t *a3,
                             WCHAR *a2, int flags);
extern int64_t PECMD_CreatePopupMenu(int64_t *script, LPCWSTR a4, int64_t *a3,
                             WCHAR *a5, int flags);
extern LPCWSTR PECMD_StripTrailingSpaces(LPCWSTR s);
extern uint64_t FUN_140063060(uint64_t a1);
extern ULARGE_INTEGER PECMD_EvalLoopCondition(int64_t *ctx, LPCWSTR path, int mode,
                                    LPCWSTR extra);
extern uintptr_t PECMD_TokenizeExpression(int64_t ctx, uint64_t a3, int64_t *pp,
                               int mode, LPCWSTR empty);
extern LARGE_INTEGER PECMD_ProcessScriptBlock(uint64_t script, uint64_t cmd, void *p3,
                                   void *p4, void *p5);
extern void PECMD_AdvanceTokenPointer(int64_t ctx, int64_t *a4, int mode, int64_t *p);
extern uint64_t PECMD_StartWorkerThread(void *script, void **pref, uint32_t a3, uint64_t a4, uint64_t a5, uint32_t a6, uint64_t a7, int64_t a8, int a9);
extern void PECMD_ReleaseRefCount(int64_t *p);
extern BOOL FUN_140101E70(LPCWSTR s);
extern uint32_t PECMD_RunProcessCommand(int64_t a1, LPCWSTR a2, uint64_t a3,
                              uint64_t a4, uint64_t a5);
extern DWORD FUN_14002B9EC(int64_t ctx, LPCWSTR path, uint32_t flags);
extern uint32_t PECMD_DevconUpdate(int64_t ctx, LPCWSTR path, LPCWSTR name, int mode);
extern uint32_t PECMD_ReadPelogonFlag(LPCWSTR name);
extern uint32_t PECMD_ReadRamdataDword(LPCWSTR name);
extern void PECMD_InitRamdataRegistry(uint32_t mode);
extern int64_t PECMD_PerformSystemShutdown(uint32_t a, uint32_t b, LPCWSTR menu);
extern void PECMD_RunShutdownScript(LPCWSTR menu, uint32_t mode);
extern LRESULT CallWindowProcW(void *prev, HWND hWnd, UINT msg, WPARAM wParam,
                               LPARAM lParam);

/* ---- SetupAPI 补充声明 (win32_stub.h 尚未提供) ---- */
HDEVINFO SetupDiCreateDeviceInfoList(const GUID *class_guid, HWND hwnd_parent);
BOOL SetupDiCreateDeviceInfoW(HDEVINFO dev_info_set, LPCWSTR device_name,
                              const GUID *class_guid, LPCWSTR device_description,
                              HWND hwnd_parent, DWORD create_flags, void *dev_info_data);
BOOL SetupDiSetDeviceRegistryPropertyW(HDEVINFO dev_info_set, void *dev_info_data,
                                       DWORD property, const BYTE *buffer, DWORD buffer_size);

/* ---- 本批引用的全局数据 ---- */
extern WCHAR g_szEmpty[];            /* g_szEmpty 空串 */
extern int16_t g_transState;        /* 透明值/状态 */
extern uint8_t g_popmenuFlag;        /* popmenu 标志 */
extern uint32_t g_hookBusyFlag;       /* 钩子忙标志 */
extern uint32_t g_hookReentry;       /* 钩子重入计数 */
extern DWORD g_d278[];        /* 线程 ID 表 */
extern uint32_t g_threadTableInitFlag;       /* 线程表初始化标志 */
extern int (*g_pSetupDiGetINFClassW)(LPCWSTR, GUID *, LPWSTR, DWORD, DWORD);
extern BOOL (*g_pSetupDiCallClassInstaller)(DWORD, void *, void *);
extern BOOL (*g_pSetupDiDestroyDeviceInfoListRev)(HDEVINFO);
extern uint8_t g_b21728[];      /* 热键表(6 字节/项) */

/* ========== PECMD_PatchRemoteWinExec @0x1400229f8 ==========
 * 向目标进程写入一段补丁代码，用于重定向 WinExec/ExitWindowsEx。
 * TODO(verify): WinExec_exref/ExitWindowsEx_exref 为导入表地址符号，
 *               与最终 core_globals.c 的数据定义需要主代理统一。
 */
void PECMD_PatchRemoteWinExec(HANDLE hProcess)
{
    extern unsigned char WinExec_exref[];
    extern unsigned char ExitWindowsEx_exref[];

    size_t nSize = 0x3d;
    int local_res10[6];
    unsigned char local_108[52];
    unsigned char local_d4[212];

    local_res10[0] = (int)(uintptr_t)WinExec_exref -
                     (int)(uintptr_t)ExitWindowsEx_exref + -0x38;
    memcpy(local_108, (const void *)(uintptr_t)0x14001a7fc, 0x3d);
    memcpy(local_d4, local_res10, (int)nSize + -0x39);
    WriteProcessMemory(hProcess, (LPVOID)(uintptr_t)ExitWindowsEx_exref,
                       local_108, nSize, (size_t *)0);
}

/* ========== PECMD_EnumDisplayModes @0x140023338 ==========
 * 枚举显示模式，把符合条件的 DEVMODEW 去重后写入 hProcess 指向的数组。
 * 返回写入的模式个数。
 */
uint64_t PECMD_EnumDisplayModes(void **out, int maxCount, uint32_t flags,
                                LPCWSTR adapter)
{
    PECMD_DEVMODEW local_1f8;
    uint64_t local_11c[28];
    uint8_t *puVar1;
    uint64_t uVar5 = 0;
    uint64_t uVar6 = 0;
    uint64_t uVar7 = 0;

    PECMD_GrowByteBuffer(out, (int64_t)(maxCount * 0xdc));
    puVar1 = (uint8_t *)*out;

    memset(&local_1f8, 0, sizeof(local_1f8));
    memset(local_11c, 0, sizeof(local_11c));
    local_1f8.dmSize = 0xdc;
    local_1f8.dmDriverExtra = 0xdc;

    if (0 < maxCount) {
        uint8_t *_Dst = puVar1;
        do {
            BOOL ok = EnumDisplaySettingsW(adapter, (DWORD)uVar7, &local_1f8);
            if (ok == 0) {
                break;
            }
            if ((0x1df < local_1f8.dmPelsHeight) && (7 < local_1f8.dmBitsPerPel) &&
                ((local_1f8.dmPelsWidth < 0x400 || (0xf < local_1f8.dmBitsPerPel)))) {
                if (0 < (int64_t)uVar6) {
                    uint32_t *puVar3 = (uint32_t *)(puVar1 + 0xb0);
                    int64_t lVar4 = 0;
                    do {
                        if ((((puVar3[-1] == local_1f8.dmPelsWidth) &&
                              (*puVar3 == local_1f8.dmPelsHeight)) &&
                             (((flags & 0x10000) != 0) ||
                              ((puVar3[-2] == local_1f8.dmBitsPerPel &&
                                (puVar3[2] == local_1f8.dmDisplayFrequency)))))) {
                            if (((flags & 0x20000) == 0) ||
                                (puVar3[-0x17] == local_1f8.u.display.dmDisplayOrientation)) {
                                goto skip_duplicate;
                            }
                        }
                        lVar4 = lVar4 + 1;
                        puVar3 = puVar3 + 0x37;
                    } while (lVar4 < (int64_t)uVar6);
                }
                memcpy(_Dst, &local_1f8, 0xdc);
                uVar5 = uVar5 + 1;
                uVar6 = uVar6 + 1;
                _Dst = _Dst + 0xdc;
            }
skip_duplicate:
            uVar7 = (uint64_t)((DWORD)uVar7 + 1);
        } while ((int64_t)uVar6 < (int64_t)maxCount);
    }

    if (0 < (short)flags) {
        PECMD_BubbleSort(puVar1, (int64_t)(int)uVar5, 0xdc, (void *)(uintptr_t)0x14001a8a8);
    }
    if ((short)flags < 0) {
        PECMD_BubbleSort(puVar1, (int64_t)(int)uVar5, 0xdc, (void *)(uintptr_t)0x14001a908);
    }
    return uVar5;
}

/* ========== PECMD_WaitKeyPressHooked @0x140025b10 ==========
 * 安装/复用 GETMESSAGE 钩子，在指定超时内派发消息。
 * TODO(verify): g_hookBusyFlag/074 与钩子过程 PECMD_KeyboardHookProc 的交互语义。
 */
int PECMD_WaitKeyPressHooked(uint32_t timeout, uint32_t flags)
{
    DWORD DVar1;
    DWORD DVar2;
    uint32_t uVar3;
    BOOL BVar4;
    HMODULE hmod;
    int iVar5 = 0;
    uint32_t uVar6;
    uint32_t uVar7;
    MSG local_38;

    EnterCriticalSection((LPCRITICAL_SECTION)&g_csInit);
    if (((flags >> 0x10 & 1) == 0) && (g_hookReentry == 0)) {
        g_hookBusyFlag = 0;
    }
    g_hookReentry = g_hookReentry + 1;
    uVar6 = flags & 0xff;
    if (g_hHook == (HHOOK)0) {
        hmod = GetModuleHandleW((LPCWSTR)0);
        g_hHook = SetWindowsHookExW(0xd, (void *)(uintptr_t)PECMD_KeyboardHookProc, hmod, 0);
        if (g_hHook == (HHOOK)0) {
            g_hookReentry = g_hookReentry - 1;
            goto done;
        }
    }
    LeaveCriticalSection((LPCRITICAL_SECTION)&g_csInit);
    DVar1 = GetTickCount();
    uVar7 = timeout;
    if ((uVar6 == 2) && (timeout == 0)) {
        uVar7 = 10;
    }
loop:
    if (g_hookBusyFlag == 0) {
        if (uVar6 != 2) {
            DVar2 = GetTickCount();
            uVar7 = (timeout + DVar1) - DVar2;
            if ((int)uVar7 < 1) goto wait_done;
        }
        if (g_flagA24F < 1) goto wait_done;
        uVar3 = uVar7;
        if (10 < uVar7) {
            uVar3 = 10;
        }
        FUN_14001bbac(&g_Script, 0, (HANDLE *)0, 0, uVar3, 0x4ff);
        while ((g_flagA24F > 0) && (g_hookBusyFlag == 0)) {
            if (uVar6 != 2) {
                DVar2 = GetTickCount();
                uVar7 = (timeout + DVar1) - DVar2;
                if ((int)uVar7 < 1) break;
            }
            BVar4 = PeekMessageW(&local_38, (HWND)0, 0, 0, 1);
            if (BVar4 == 0) break;
            TranslateMessage(&local_38);
            DispatchMessageW(&local_38);
        }
        goto loop;
    }
wait_done:
    EnterCriticalSection((LPCRITICAL_SECTION)&g_csInit);
    iVar5 = g_hookBusyFlag;
    g_hookReentry = g_hookReentry - 1;
    if (g_hookReentry == 0) {
        g_hookBusyFlag = 0;
    }
done:
    LeaveCriticalSection((LPCRITICAL_SECTION)&g_csInit);
    return iVar5;
}

/* ========== FUN_140025CE0 @0x140025ce0 ==========
 * 解析窗口/控件样式关键字：top/bottom/pic/enable/disable/wait/trans:。
 * TODO(verify): trans: 分支的 transBuf 反编译写法按局部 32 位暂存处理。
 */
void FUN_140025CE0(int64_t *pp, uint8_t *outEnabled, uint8_t *style,
                                   uint8_t *outMode, char *prefix, uint8_t *transBuf)
{
    char cVar1;
    int iVar2;
    int64_t uVar3;
    uint16_t *puVar4;
    uint8_t *puVar5;
    uint8_t local_res8[8];

    puVar5 = local_res8;
    if (transBuf != (uint8_t *)0) {
        puVar5 = transBuf;
    }
    g_transState = -1;
    if (*(int16_t *)*pp == 0x2d) {
        *prefix = '-';
    }
    while (((uint32_t)(int)*prefix == (uint32_t)*(uint16_t *)*pp) &&
           ((uint32_t)(int)*prefix != (uint32_t)*(uint16_t *)(*pp + 2))) {
        *pp = *pp + 2;
        uVar3 = FUN_14005B1A8(&g_szEmpty, pp, 0);
        if ((int)uVar3 == 0) {
            cVar1 = PECMD_AdvanceAfterPrefix(WSTR("top"), pp, 3);
            if (cVar1 == '\0') {
                cVar1 = PECMD_AdvanceAfterPrefix(WSTR("top-"), pp, 4);
                if (cVar1 == '\0') {
                    cVar1 = PECMD_AdvanceAfterPrefix(WSTR("bottom"), pp, 6);
                    if (cVar1 == '\0') {
                        cVar1 = PECMD_AdvanceAfterPrefix(WSTR("pic"), pp, 3);
                        if (cVar1 == '\0') {
                            cVar1 = PECMD_AdvanceAfterPrefix(WSTR("enable"), pp, 6);
                            if (cVar1 == '\0') {
                                cVar1 = PECMD_AdvanceAfterPrefix(WSTR("disable"), pp, 7);
                                if (cVar1 == '\0') {
                                    cVar1 = PECMD_AdvanceAfterPrefix(WSTR("wait"), pp, 4);
                                    if (cVar1 == '\0') {
                                        iVar2 = StrCmpNIW(WSTR("trans:"), (LPCWSTR)*pp, 6);
                                        if (iVar2 == 0) {
                                            int32_t trans_val;
                                            *pp = *pp + 0xc;
                                            trans_val = (uint16_t)g_transState |
                                                        ((uint32_t)(uint16_t)(g_transState >> 0xf) << 16);
                                            PECMD_ParseUIntValue(pp, (int *)&trans_val);
                                            g_transState = (int16_t)trans_val;
                                        } else {
                                            puVar4 = (uint16_t *)*pp;
                                            while ((*puVar4 != 0) &&
                                                   (((puVar4 = (uint16_t *)*pp, *puVar4 < 9 ||
                                                      (0xd < *puVar4)) && (*puVar4 != 0x20)))) {
                                                puVar4 = puVar4 + 1;
                                                *pp = (int64_t)puVar4;
                                            }
                                            FUN_14005B154((WCHAR **)pp);
                                        }
                                    } else {
                                        *puVar5 = 1;
                                    }
                                } else {
                                    *outMode = 2;
                                }
                            } else {
                                *outMode = 0;
                            }
                        } else {
                            *style = *style | 0x40;
                        }
                    } else {
                        *style = *style & 0xf0;
                        *style = *style | 0x10;
                    }
                } else {
                    *style = *style & 0xf0;
                }
            } else {
                *style = *style & 0xef;
                *style = *style | 1;
            }
        } else {
            *outEnabled = 1;
        }
    }
}

/* ========== FUN_1400282D4 @0x1400282d4 ==========
 * 手工映射 PE 到进程地址空间并调用 DllMain。
 * TODO(verify): PE 解析辅助函数返回类型/行为按反编译保留。
 */
uint64_t FUN_1400282D4(int *pe, short *args, uint64_t flag)
{
    int iVar2;
    uint64_t dwSize;
    void *lpAddress;
    uint64_t uVar3 = 0;
    int64_t lVar4;

    if ((((*(int64_t *)((char *)pe + 24) == 0) &&
          (PECMD_ValidatePeHeader((int64_t)pe, args, flag) != 0)) &&
         ((dwSize = PECMD_ComputeSectionMapSize((int64_t)pe), dwSize != 0))) &&
        ((lpAddress = VirtualAlloc(*(LPVOID *)(*(int64_t *)((char *)pe + 40) + 0x30),
                                   dwSize, 0x3000, 0x40),
          lpAddress != (void *)0) ||
         (lpAddress = VirtualAlloc((LPVOID)0, dwSize, 0x3000, 0x40),
          lpAddress != (void *)0))) {
        PECMD_CollectPESections((int64_t)pe, lpAddress, (int64_t)args);
        if ((*(int *)(*(int64_t *)((char *)pe + 40) + 0xb0) == 0) ||
            (*(int *)(*(int64_t *)((char *)pe + 40) + 0xb4) == 0) ||
            ((uVar3 = PECMD_RelocateImage((int64_t)pe, (int64_t)lpAddress), (int)uVar3 != 0))) {
            uVar3 = PECMD_BindDelayImports((int64_t)pe, (int64_t)lpAddress);
            if ((int)uVar3 == 0) {
                VirtualFree(lpAddress, 0, 0x8000);
            } else {
                *(void **)(*(int64_t *)((char *)pe + 40) + 0x30) = lpAddress;
                if ((*(uint16_t *)(*(int64_t *)((char *)pe + 40) + 0x16) & 0x2000) == 0) {
                    iVar2 = *pe;
                    uVar3 = *(uint64_t *)((char *)pe + 24);
                    *pe = 0x200;
                    *(void **)((char *)pe + 24) = lpAddress;
                    lVar4 = PECMD_FindResourceByNameOrId((int64_t)pe, "DllMain");
                    *(int64_t *)((char *)pe + 8) = lVar4;
                    if (lVar4 == 0) {
                        lVar4 = PECMD_FindResourceByNameOrId((int64_t)pe, "_dllMain@24");
                        *(int64_t *)((char *)pe + 8) = lVar4;
                    }
                    *pe = iVar2;
                    *(uint64_t *)((char *)pe + 24) = uVar3;
                } else {
                    *(void **)((char *)pe + 8) =
                        (uint8_t *)lpAddress + *(uint32_t *)(*(int64_t *)((char *)pe + 40) + 0x28);
                }
                if ((*(void **)((char *)pe + 8) == (void *)0) ||
                    (iVar2 = (*(int (**)(void *, int, int))(pe + 2))(lpAddress, 1, 0),
                     iVar2 != 0)) {
                    *(void **)((char *)pe + 24) = lpAddress;
                    *pe = 1;
                    g_pMapBlk = pe;
                    return *(uint64_t *)((char *)pe + 24);
                }
                (*(int (**)(void *, int, int))(pe + 2))(lpAddress, 0, 0);
                VirtualFree(lpAddress, 0, 0x8000);
                pe[2] = 0;
                pe[3] = 0;
            }
        }
    }
    return 0;
}

/* ========== PECMD_GraphSubclassWndProc @0x140028560 ==========
 * 图形窗口子类过程：处理擦背景、滚动/滚轮重绘、拖放、Ctrl+C/X/R 快捷键。
 */
LRESULT PECMD_GraphSubclassWndProc(HWND hwnd, UINT msg, HDROP wParam, LPARAM lParamIn)
{
    HBRUSH hbr;
    uint16_t uVar1;
    HWND hWnd;
    LRESULT LVar2;
    UINT Msg;
    int iVar3;
    LPARAM lParamNew;
    RECT local_28;

    hbr = *(HBRUSH *)((char *)g_pAppData + 0xd8);
    if (msg == 0x14) {
        GetClientRect(hwnd, &local_28);
        FillRect((HDC)wParam, &local_28, hbr);
        return 1;
    }
    if (((msg == 0x115) || (msg == 0x114)) || (msg == 0x20a)) {
        PostMessageW(hwnd, 0x44c, 0, 0);
    }
    if (msg == 0x44c) {
        RedrawWindow(hwnd, (const RECT *)0, (HRGN)0, 5);
        return 1;
    }
    if (msg == 0x233) {
        PECMD_HandleDropFile(g_pAppData, wParam, '\x01');
        return 1;
    }
    if (msg == 0x100) {
        iVar3 = (int)wParam;
        if (((iVar3 == 0x43) || (iVar3 == 0x58)) &&
            (uVar1 = GetAsyncKeyState(0x11), (uVar1 & 0x8000) != 0)) {
            lParamNew = 0;
            Msg = 0x301;
        } else {
            if ((iVar3 != 0x41) || (uVar1 = GetAsyncKeyState(0x11), (uVar1 & 0x8000) == 0)) {
                if (wParam != (HDROP)0x72) goto call_prev;
                hWnd = GetParent(hwnd);
                SendMessageW(hWnd, 0x100, 0x72, lParamNew);
                goto after_key;
            }
            lParamNew = -1;
            Msg = 0xb1;
        }
        SendMessageW(hwnd, Msg, 0, lParamNew);
    } else {
after_key:
        if ((((msg == 0x102) || (msg == 0x300)) || (msg == 0x302)) ||
            ((msg == 0x303 || (msg == 0x304)))) {
            return 0;
        }
    }
call_prev:
    LVar2 = CallWindowProcW(*(void **)((char *)g_pAppData + 0xe0), hwnd, msg,
                            (WPARAM)wParam, lParamNew);
    return LVar2;
}

/* ========== FUN_14002B7F8 @0x14002b7f8 ==========
 * 调用 NewDev.dll 的 UpdateDriverForPlugAndPlayDevicesW 安装驱动。
 */
uint32_t FUN_14002B7F8(int64_t ctx, LPCWSTR hwid, LPCWSTR inf,
                                uint32_t flags, int mode, uint8_t *out)
{
    int iVar1;
    DWORD DVar2;
    HMODULE pHVar3;
    void *pFVar4;
    int local_res8[2];
    WCHAR *local_38[2];

    if (*(char *)(ctx + 0x157) != '\0') {
        return 0;
    }
    if (out == (uint8_t *)0) {
        out = (uint8_t *)(ctx + 0x152);
    }
    DVar2 = 0;
    local_res8[0] = 0;
    if (*(int64_t *)(ctx + 0x138) == 0) {
        pHVar3 = LoadLibraryW(WSTR("NewDev.dll"));
        *(HMODULE *)(ctx + 0x138) = pHVar3;
    }
    SetLastError(0);
    if (*(HMODULE *)(ctx + 0x138) != (HMODULE)0) {
        if (*(int64_t *)(ctx + 0x140) == 0) {
            pFVar4 = GetProcAddress(*(HMODULE *)(ctx + 0x138),
                                    "UpdateDriverForPlugAndPlayDevicesW");
            *(void **)(ctx + 0x140) = pFVar4;
            if (pFVar4 == (void *)0) goto fail;
        }
        if ((flags & 0x30000) != 0) {
            PECMD_PatchInfDirectives(inf);
        }
        iVar1 = (*(int (**)(int, LPCWSTR, LPCWSTR, uint32_t, int *))
                 (ctx + 0x140))(0, hwid, inf, flags & 0xffff, local_res8);
        if (out != (uint8_t *)0) {
            *out = (uint8_t)local_res8[0];
        }
        if (*(int64_t *)(ctx + 0x110) != 0) {
            if (iVar1 == 0) goto fail;
            if (mode != -0x10) {
                PECMD_AppendKeyIfMissing(ctx, hwid, mode);
            }
        }
        if (iVar1 != 0) goto success;
    }
fail:
    DVar2 = GetLastError();
    if (DVar2 == 0) {
        DVar2 = 1;
    }
success:
    {
        LPCWSTR pwVar6 = WSTR("(null)");
        LPCWSTR pwVar5;
        if (hwid != (LPCWSTR)0) {
            pwVar6 = hwid;
        }
        PECMD_AllocWStringBuffer(local_38, 0x2800);
        pwVar5 = g_szEmpty;
        if (local_res8[0] != 0) {
            pwVar5 = WSTR("Reboot");
        }
        _snwprintf(local_38[0], 0x27ff,
                   WSTR("PECMD安装驱动【%s】[%s][%s]"), pwVar6, inf, pwVar5);
        FUN_140025f10(ctx + 8, local_38[0], DVar2, (void *)0x1100, (void *)0, (int64_t *)0);
        PECMD_FreeStrBuf(local_38);
        PECMD_WriteParamRecord(ctx, 'I', pwVar6, inf);
    }
    return DVar2;
}

/* ========== FUN_14002C634 @0x14002c634 ==========
 * 构造 "!=drvload ..." 命令行并执行驱动安装。
 */
uint32_t FUN_14002C634(int64_t ctx, LPCWSTR inf, LPCWSTR hwid,
                                      int mode)
{
    int iVar1;
    DWORD DVar2;
    LPCWSTR pWVar3;
    FILETIME local_38;
    WCHAR *local_30 = NULL;
    WCHAR *local_28[2];
    uint16_t local_res8[4];

    if (*(char *)(ctx + 0x157) == '\0') {
        PECMD_AllocWStringBuffer((WCHAR **)&local_30, 0);
        FUN_1400637DC((int64_t *)&local_30, "--wd:*\"", 0xffffffffffffffffULL,
                      0xffffffffffffffffULL);
        FUN_14006375C(&local_30, inf);
        FUN_14006375C(&local_30, WSTR("\" "));
        FUN_14006375C(&local_30, WSTR("!=drvload \""));
        FUN_14006375C(&local_30, inf);
        FUN_14006375C(&local_30, WSTR("\""));
        iVar1 = lstrlenW(local_30);
        PECMD_AllocString(&local_30, (int64_t)iVar1 + 5);
        memset(&local_38, 0, sizeof(local_38));
        memset(local_res8, 0, sizeof(local_res8));
        PECMD_ExecCmdDispatch(&g_Script, local_30, &g_Script, local_res8, 0, &local_38, 0, (void *)0);
        if ((*(int64_t *)(ctx + 0x110) != 0) && (local_38.dwLowDateTime == 0)) {
            PECMD_AppendKeyIfMissing(ctx, hwid, mode);
        }
        PECMD_AllocWStringBuffer(local_28, 0x2800);
        pWVar3 = g_szEmpty;
        if (hwid != (LPCWSTR)0) {
            pWVar3 = hwid;
        }
        _snwprintf(local_28[0], 0x27ff, WSTR("DrvLoad安装驱动【(%s)?】[%s]"),
                   pWVar3, inf);
        FUN_140025f10(ctx + 8, local_28[0], local_38.dwLowDateTime,
                      (void *)0x1100, (void *)0, (int64_t *)0);
        PECMD_FreeStrBuf(local_28);
        DVar2 = local_38.dwLowDateTime;
        PECMD_FreeStrBuf(&local_30);
    } else {
        DVar2 = 0;
    }
    return DVar2;
}

/* ========== FUN_14002C8A4 @0x14002c8a4 ==========
 * 解析 "设备实例:类名" 形式的参数并创建设备信息。
 * TODO(verify): 返回的 unaff_RBX 按 0 处理；设备信息清理已保留。
 */
uint64_t FUN_14002C8A4(LPCWSTR spec)
{
    WCHAR *pWVar1;
    int iVar2;
    BOOL BVar3;
    HDEVINFO DeviceInfoSet;
    WCHAR *local_res8;
    GUID local_2a8;
    SP_DEVINFO_DATA local_298;
    WCHAR local_278[40];
    WCHAR local_228[264];

    DeviceInfoSet = (HDEVINFO)0xffffffffffffffffULL;
    local_res8 = (WCHAR *)(uintptr_t)spec;
    FUN_1400F429C((WCHAR **)&local_res8, 0x3a);
    pWVar1 = local_res8;
    if (*local_res8 == L'\0') {
        return 0;
    }
    if (local_res8[1] == L'\0') {
        return 0;
    }
    *local_res8 = L'\0';
    local_res8 = local_res8 + 1;
    memset(local_228, 0, 0x208);
    lstrcpynW(local_228, spec, 0x100);
    iVar2 = (*g_pSetupDiGetINFClassW)(local_res8, &local_2a8, local_278, 0x24, 0);
    if (iVar2 != 0) {
        DeviceInfoSet = SetupDiCreateDeviceInfoList(&local_2a8, (HWND)0);
        if (DeviceInfoSet != (HDEVINFO)0xffffffffffffffffULL) {
            local_298.cbSize = 0x20;
            BVar3 = SetupDiCreateDeviceInfoW(DeviceInfoSet, local_278, &local_2a8,
                                             (LPCWSTR)0, (HWND)0, 1, &local_298);
            if (BVar3 != 0) {
                iVar2 = lstrlenW(local_228);
                BVar3 = SetupDiSetDeviceRegistryPropertyW(DeviceInfoSet, &local_298, 1,
                                                          (const BYTE *)local_228,
                                                          iVar2 * 2 + 4);
                if ((BVar3 != 0) &&
                    (iVar2 = (*g_pSetupDiCallClassInstaller)(0x19, (void *)(uintptr_t)DeviceInfoSet, &local_298), iVar2 != 0))
                    goto done;
            }
        }
        GetLastError();
    }
done:
    *pWVar1 = L':';
    if (DeviceInfoSet != (HDEVINFO)0xffffffffffffffffULL) {
        (*g_pSetupDiDestroyDeviceInfoListRev)(DeviceInfoSet);
    }
    return 0;
}

/* ========== PECMD_WaitOtherThreadsExit @0x14002ca80 ==========
 * 注册当前线程并等待前面的线程完成（配合任务计数/事件）。
 * TODO(verify): 数组容量/计数的 64 位布局按 g_threadTableCap/g_threadTableCnt 合并处理。
 */
uint64_t PECMD_WaitOtherThreadsExit(void)
{
    DWORD DVar3;
    int iVar4;
    DWORD *pDVar5;
    int iVar6;
    int iVar7;
    int64_t local_res10;

    DVar3 = GetCurrentThreadId();
    if ((g_threadTableInitFlag & 1) == 0) {
        g_threadTableInitFlag = g_threadTableInitFlag | 1;
        PECMD_InitPtrTable((int64_t *)g_d278);
        atexit((void (*)(void))(uintptr_t)0x14011ab84);
    }
    EnterCriticalSection((LPCRITICAL_SECTION)&g_csInit);
    g_taskCount = g_taskCount + 1;
    iVar4 = 0;
    pDVar5 = g_d278;
    if (0 < (int64_t)g_threadTableCnt) {
        do {
            if (*pDVar5 == 0) {
                g_d278[iVar4] = DVar3;
                if (DVar3 != 0) goto registered;
                break;
            }
            iVar4 = iVar4 + 1;
            pDVar5 = pDVar5 + 1;
        } while ((int64_t)iVar4 < (int64_t)g_threadTableCnt);
    }
    local_res10 = (int64_t)(int)DVar3;
    FUN_1400639F0((int64_t *)g_d278, (int64_t *)&g_threadTableCap,
                  (int64_t *)&g_threadTableCnt, &local_res10, 8, 1);
registered:
    iVar4 = (int)g_threadTableCnt;
    LeaveCriticalSection((LPCRITICAL_SECTION)&g_csInit);
    iVar6 = 500;
    do {
        FUN_1400195F0(&g_Script, 2, 0, (void *)0);
        EnterCriticalSection((LPCRITICAL_SECTION)&g_csInit);
        {
            int iVar2 = (int)g_threadTableCnt;
            int64_t lVar1 = (int64_t)g_threadTableCnt;
            iVar7 = 0;
            pDVar5 = g_d278;
            if (lVar1 < 1) {
                LeaveCriticalSection((LPCRITICAL_SECTION)&g_csInit);
                goto done_wait;
            }
            while (DVar3 != *pDVar5) {
                iVar7 = iVar7 + 1;
                pDVar5 = pDVar5 + 1;
                if (lVar1 <= iVar7) {
                    LeaveCriticalSection((LPCRITICAL_SECTION)&g_csInit);
                    goto done_wait;
                }
            }
            if ((iVar4 == lVar1) || (iVar6 = iVar6 - 1, iVar6 < 0)) {
                PECMD_ResizePtrTable((int64_t *)g_d278, 0);
                LeaveCriticalSection((LPCRITICAL_SECTION)&g_csInit);
                FUN_14002CA30();
                goto done_wait;
            }
            LeaveCriticalSection((LPCRITICAL_SECTION)&g_csInit);
            iVar4 = iVar2;
        }
    } while (1);
done_wait:
    EnterCriticalSection((LPCRITICAL_SECTION)&g_csInit);
    g_taskCount = g_taskCount - 1;
    LeaveCriticalSection((LPCRITICAL_SECTION)&g_csInit);
    if (g_hWaitEvent != (HANDLE)0) {
        SetEvent(g_hWaitEvent);
    }
    return 0;
}

/* ========== PECMD_ExecSubCommand @0x140030144 ==========
 * 执行子命令分发：--popmenu、-sub 前缀和 - * + ~ ^ 修饰符。
 */
int64_t PECMD_ExecSubCommand(int64_t *script, WCHAR *cmd, int64_t *out,
                                LPCWSTR extra, uint32_t flags)
{
    WCHAR WVar1;
    int iVar2;
    int64_t uVar3;
    int64_t lVar4;
    WCHAR *pWVar5;
    uint32_t uVar6;
    WCHAR WVar7;
    WCHAR *pWVar8;
    LPCWSTR pWVar9;
    WCHAR *pWVar10;
    WCHAR *local_res10;
    WCHAR local_38;
    uint32_t local_34;
    int64_t local_30;

    pWVar8 = (WCHAR *)0;
    if (g_flagA24F < 1) {
        return 0;
    }
    WVar7 = L'\0';
    local_res10 = cmd;
    if (*cmd == L'\0') {
        PECMD_ResetScriptChain(script, (void *)0);
        return 0;
    }
    g_flag16a = 0;
    pWVar5 = (WCHAR *)(uintptr_t)g_popmenuFlag;
    if ((int8_t)g_popmenuFlag < 0) {
        pWVar5 = pWVar8;
    }
    local_38 = L'\0';
    g_popmenuFlag = (uint8_t)(uintptr_t)pWVar5;
    pWVar10 = (WCHAR *)0;
    pWVar5 = pWVar8;
    if ((*cmd == L'-') && (cmd[1] == L'-')) {
        if (cmd[2] == L'\0') {
            PECMD_FreeRecordChain((int64_t)script);
            return 0;
        }
        iVar2 = StrCmpNIW(cmd + 2, WSTR("popmenu"), 7);
        pWVar8 = pWVar10;
        pWVar5 = (WCHAR *)0;
        if (iVar2 == 0) {
            pWVar5 = cmd + 9;
            WVar1 = *pWVar5;
            local_res10 = pWVar5;
            while ((cmd = local_res10, WVar1 != L'\0')) {
                if (((8 < (uint16_t)WVar1) && ((uint16_t)WVar1 < 0xe)) || (WVar1 == L' ')) {
                    if (*local_res10 != L'\0') {
                        local_38 = *local_res10;
                        *local_res10 = L'\0';
                        cmd = local_res10 + 1;
                        pWVar8 = local_res10;
                        local_res10 = cmd;
                    }
                    break;
                }
                local_res10 = local_res10 + 1;
                WVar1 = *local_res10;
            }
        }
    }
    pWVar9 = (LPCWSTR)0;
    pWVar10 = cmd;
    if (*cmd == L'-') {
        uVar3 = FUN_14005C788("-sub", cmd, 4);
        pWVar9 = (LPCWSTR)0;
        if ((char)uVar3 != '\0') {
            WVar1 = *cmd;
            while ((WVar1 != L'\0' &&
                    ((local_res10 = pWVar10, (uint16_t)WVar1 < 9 || (0xd < (uint16_t)WVar1)))) &&
                   (WVar1 != L' ')) {
                pWVar10 = pWVar10 + 1;
                local_res10 = pWVar10;
                WVar1 = *pWVar10;
            }
            FUN_14005B154((WCHAR **)&local_res10);
            pWVar10 = local_res10;
        }
    }
    if (pWVar5 == (WCHAR *)pWVar9) {
        if (*pWVar10 == L'-') {
            WVar1 = L'-';
        } else {
            WVar1 = L'*';
            if (((*pWVar10 != L'*') && (WVar1 = L'+', *pWVar10 != L'+')) &&
                (WVar1 = L'~', *pWVar10 != L'~')) {
                goto after_modifier;
            }
        }
        WVar7 = WVar1;
        pWVar10 = pWVar10 + 1;
        local_res10 = pWVar10;
    }
after_modifier:
    WVar1 = *pWVar10;
    while (WVar1 == L'^') {
        pWVar10 = pWVar10 + 1;
        local_res10 = pWVar10;
        WVar1 = *pWVar10;
    }
    if (WVar7 != 0) {
        WVar1 = *pWVar10;
        while (WVar7 == WVar1) {
            pWVar10 = pWVar10 + 1;
            local_res10 = pWVar10;
            WVar1 = *pWVar10;
        }
    }
    FUN_14005B154((WCHAR **)&local_res10);
    local_34 = flags;
    if (*local_res10 == 0) {
        return 0;
    }
    local_30 = script[5];
    if (extra == pWVar9) {
        extra = FUN_14002FD88(script, local_res10, &local_34, &local_30);
        pWVar9 = (LPCWSTR)0;
        if (extra == (LPCWSTR)0) {
            return 1;
        }
    }
    uVar6 = local_34;
    if ((int)local_34 < (int)(uint32_t)(uintptr_t)pWVar9) {
        uVar6 = (uint32_t)(uintptr_t)pWVar9;
    }
    PECMD_ShowAboutDialog();
    if (pWVar5 == (WCHAR *)0) {
        iVar2 = (int)(uVar6 & 0xbfffffff) + 1;
        if (WVar7 == L'\0') {
            lVar4 = PECMD_CreateWindow(script, extra, out, cmd, iVar2);
            iVar2 = (int)lVar4;
        } else {
            lVar4 = PECMD_ExecWindowThread(script, (uint64_t)extra, out, cmd, iVar2);
            iVar2 = (int)lVar4;
        }
        return (int64_t)iVar2;
    }
    if (pWVar8 != (WCHAR *)0) {
        *pWVar8 = local_38;
    }
    return PECMD_CreatePopupMenu(script, extra, out, pWVar5,
                         (int)(uVar6 & 0xbfffffff));
}

/* ========== PECMD_ExecPathList @0x140035860 ==========
 * 按分隔符循环执行路径列表，支持 ! 取反。
 */
int64_t PECMD_ExecPathList(int64_t *script, LPCWSTR paths, uint64_t flags,
                               int64_t *out, void *ctx)
{
    WCHAR WVar1;
    SHORT sVar2;
    bool bVar3;
    bool bVar4;
    LPWSTR pWVar5;
    int64_t lVar6;
    SHORT *psVar7;
    ULARGE_INTEGER UVar8;
    SHORT *psVar9;
    LARGE_INTEGER LVar10;
    LPCWSTR pWVar11;
    char cVar12;
    int iVar13;
    LARGE_INTEGER local_res10;
    uint64_t local_res18;
    LPCWSTR local_58;
    ULARGE_INTEGER local_50;
    SHORT *local_48;
    int64_t local_40;

    cVar12 = ',';
    if ((*paths == L';') || (*paths == L':')) {
        cVar12 = (char)*paths;
        paths = paths + 1;
    }
    psVar9 = (SHORT *)0;
    for (local_58 = paths;
         (WVar1 = *local_58, WVar1 != L'\0' &&
          ((((uint16_t)WVar1 < 9 || (0xd < (uint16_t)WVar1)) && (WVar1 != L' '))));
         local_58 = local_58 + 1) {
    }
    local_res18 = flags;
    FUN_14005B154((WCHAR **)&local_58);
    pWVar11 = local_58;
    pWVar5 = StrChrW(local_58, (WCHAR)(SHORT)cVar12);
    if (pWVar5 == (LPWSTR)0) {
        lVar6 = 1;
    } else {
        *pWVar5 = L'\0';
        PECMD_StripTrailingSpaces(pWVar11);
        local_res10.QuadPart = (int64_t)(pWVar5 + 1);
        FUN_14005B154((WCHAR **)&local_res10);
        LVar10 = local_res10;
        psVar7 = (SHORT *)(uintptr_t)FUN_140063060((uint64_t)local_res10.QuadPart);
        bVar4 = true;
        sVar2 = *psVar7;
        if (sVar2 == 0x21) {
            local_res10.QuadPart = LVar10.QuadPart + 2;
            FUN_14005B154((WCHAR **)&local_res10);
            LVar10 = local_res10;
        }
        local_40 = 0;
        local_48 = (SHORT *)0;
        psVar7 = psVar9;
        if (out != (int64_t *)0) {
            psVar7 = (SHORT *)(uintptr_t)*out;
        }
        if (((*(WCHAR *)(uintptr_t)LVar10.QuadPart != L'\0') || (psVar7 == (SHORT *)0)) ||
            (*(SHORT *)((char *)script + 0x96) != *psVar7)) {
            bVar4 = false;
        }
        if (*(WCHAR *)(uintptr_t)LVar10.QuadPart == L'\0') {
            if (!bVar4) {
                PECMD_FreeStrBuf((WCHAR **)&local_40);
                return -0x7ff8ffa9;
            }
            PECMD_AdvanceTokenPointer((int64_t)script, out, 0, (int64_t *)0);
        }
        *(uint16_t *)((char *)script + 0x19) =
            *(uint16_t *)((char *)script + 0x19) & 0x87;
        while (1) {
            iVar13 = (int)(uintptr_t)psVar9;
            FUN_1400702B0((WCHAR **)&local_50.QuadPart, pWVar11);
            UVar8 = PECMD_EvalLoopCondition(script, (LPCWSTR)local_50.QuadPart, 2, (LPCWSTR)0);
            bVar3 = 0 < (int64_t)UVar8.QuadPart;
            PECMD_FreeStrBuf((WCHAR **)&local_50.QuadPart);
            if (sVar2 == 0x21) {
                bVar3 = !bVar3;
            }
            if (!bVar3) break;
            if ((*(SHORT *)(uintptr_t)LVar10.QuadPart == 0) && (bVar4)) {
                local_48 = psVar7 + 1;
                psVar9 = (SHORT *)(uintptr_t)PECMD_TokenizeExpression(
                    (int64_t)script, local_res18, (int64_t *)&local_48, 0, g_szEmpty);
            } else {
                LVar10 = PECMD_ProcessScriptBlock((uint64_t)(uintptr_t)script,
                                       (uint64_t)LVar10.QuadPart, (void *)0, (void *)0,
                                       ctx);
                psVar9 = (SHORT *)(uintptr_t)(LVar10.QuadPart & 0xffffffffULL);
            }
            iVar13 = (int)(uintptr_t)psVar9;
            if ((*(uint16_t *)((char *)script + 0x19) & 0x100) != 0) {
                *(uint16_t *)((char *)script + 0x19) =
                    *(uint16_t *)((char *)script + 0x19) & 0xfeff;
            }
            if ((g_flagA24F < 1) ||
                (pWVar11 = local_58, LVar10 = local_res10,
                 (*(uint8_t *)((char *)script + 0x19) & 0xef) != 0)) {
                break;
            }
        }
        *(uint16_t *)((char *)script + 0x19) =
            *(uint16_t *)((char *)script + 0x19) & 0x87;
        PECMD_FreeStrBuf((WCHAR **)&local_40);
        lVar6 = (int64_t)iVar13;
    }
    return lVar6;
}

/* ========== FUN_140035B40 @0x140035b40 ==========
 * 向热键子系统发送按键消息。
 * TODO(verify): g_b21728 表按 ushort 数组读取。
 */
void FUN_140035B40(uint32_t key, int mode, int repeat)
{
    uint64_t *puVar1;
    int64_t lVar2;
    uint32_t uVar3;
    uint64_t uVar4;
    uint64_t *local_res20;

    lVar2 = (int64_t)mode;
    FUN_1400633A8((void **)&local_res20, 0x24);
    PECMD_ZeroLenBuf(local_res20);
    if (local_res20 != (uint64_t *)0) {
        *local_res20 = 1;
        puVar1 = local_res20 + 1;
        if (local_res20 != (uint64_t *)0) goto init_done;
    }
    puVar1 = (uint64_t *)0;
init_done:
    *(uint32_t *)puVar1 = 0x14;
    uVar3 = (key | 0x20) - 0x61;
    *(uint32_t *)((char *)puVar1 + 4) = 2;
    *(int *)((char *)puVar1 + 0xc) = 1 << ((uint8_t)uVar3 & 0x1f);
    if (uVar3 < 0x1a) {
        uVar4 = (uint64_t)(uint32_t)(repeat * 5);
        PECMD_StartWorkerThread((void *)(uintptr_t)g_Script, (void **)local_res20, 0x219,
                      (uint64_t)*(uint16_t *)(g_b21728 + lVar2 * 6),
                      (uint64_t)(uintptr_t)puVar1, 3, uVar4, 1, 0);
        if (mode != 0) {
            *(uint16_t *)(puVar1 + 2) = 1;
        }
        if (*(uint16_t *)(g_b21728 + lVar2 * 6 + 4) != 0) {
            PECMD_StartWorkerThread((void *)(uintptr_t)g_Script, (void **)local_res20, 0x219,
                          (uint64_t)*(uint16_t *)(g_b21728 + lVar2 * 6 + 4),
                          (uint64_t)(uintptr_t)puVar1, 3, uVar4, 1, 0);
        }
        if (*(uint16_t *)(g_b21728 + lVar2 * 6 + 4) != 0) {
            PECMD_StartWorkerThread((void *)(uintptr_t)g_Script, (void **)0, 0x219,
                          (uint64_t)*(uint16_t *)(g_b21728 + lVar2 * 6 + 4),
                          0, 3, uVar4, 1, 0);
        }
    }
    PECMD_ReleaseRefCount((int64_t *)&local_res20);
    local_res20 = (uint64_t *)0;
    PECMD_FreeStrBuf((WCHAR **)&local_res20);
}

/* ========== PECMD_DeviFileExtractThreadProc @0x14003a810 ==========
 * 解压后执行驱动安装（NewDev/DrvLoad/devcon 三种路径）。
 */
uint64_t PECMD_DeviFileExtractThreadProc(int64_t *task)
{
    int *piVar1;
    WCHAR *pWVar2;
    uint16_t uVar3;
    WCHAR WVar4;
    WCHAR WVar5;
    int64_t lVar6;
    LPCWSTR pWVar7;
    BOOL bVar8;
    int iVar9;
    uint32_t uVar10;
    int64_t lVar11;
    int iVar12;
    LPCWSTR pWVar13;
    HANDLE local_res10;
    WCHAR *local_res18;

    lVar6 = *task;
    pWVar7 = (LPCWSTR)task[5];
    uVar3 = *(uint16_t *)(lVar6 + 0x14c);
    pWVar2 = (WCHAR *)(pWVar7 + task[4]);
    local_res18 = (WCHAR *)(pWVar7 + task[4] + 0x27);
    task[4] = 0;
    WVar4 = *pWVar2;
    pWVar13 = (LPCWSTR)(task[1] + 6);
    iVar12 = 0;
    if (*(char *)(lVar6 + 400) == '@') {
        memcpy((void *)pWVar2, (const void *)WSTR("!!!PECMD_SUB_OK"), 0x20);
        bVar8 = FUN_140101E70(pWVar7);
        if (bVar8 == 0) {
            iVar9 = lstrlenW(*(LPCWSTR *)(lVar6 + 0xf8));
            WVar5 = pWVar2[-1];
            pWVar2[-1] = L'\0';
            if (*(int *)(lVar6 + 0x1a0) < 0) {
                return 0;
            }
            PECMD_RunProcessCommand(lVar6, WSTR("7z x -y -aos "),
                          *(uint64_t *)(lVar6 + 0x170),
                          *(int64_t *)(lVar6 + 0xf8) + 1,
                          (uint64_t)(uintptr_t)(pWVar7 + iVar9));
            pWVar2[-1] = WVar5;
            local_res10 = (HANDLE)0;
            PECMD_OpenFileHandle(&local_res10, pWVar7, 0xc0000000, 7,
                           (LPSECURITY_ATTRIBUTES)0, 2, 0x80, (HANDLE)0);
            if ((local_res10 != (HANDLE)0) &&
                (local_res10 != (HANDLE)0xffffffffffffffffULL)) {
                CloseHandle(local_res10);
            }
            local_res10 = (HANDLE)0;
        }
        memcpy((void *)pWVar2, (const void *)local_res18,
               ((uint16_t)WVar4 + 1) * 2);
    }
    if (-1 < *(int *)(lVar6 + 0x1a0)) {
        if (*(char *)(lVar6 + 0x158) == '\0') {
            if (((*(char *)(lVar6 + 0x154) == '\x03') || (iVar9 = 0, *(char *)(lVar6 + 0x156) != '\0')) &&
                (iVar9 = 0, *(char *)(lVar6 + 0x191) == '\0')) {
                goto install_again;
            }
            while (1) {
                if (*(int *)(lVar6 + 0x1a0) < 0) {
                    return 0;
                }
                uVar10 = FUN_14002B7F8(lVar6, pWVar13, pWVar7, (uint32_t)uVar3, -1, (uint8_t *)0);
                iVar12 = iVar9 + 1;
                task[4] = (int64_t)(int)uVar10;
                if ((1 < iVar12) || (task[4] == 0)) break;
install_again:
                if (*(int *)(lVar6 + 0x1a0) < 0) {
                    return 0;
                }
                if (*(char *)(lVar6 + 0x156) == '\0') {
                    if (*(char *)(lVar6 + 0x154) == '\x03') {
                        uVar10 = FUN_14002C634(lVar6, pWVar7, pWVar13, -1);
                        lVar11 = (int64_t)uVar10;
                        goto set_result;
                    }
                } else {
                    uVar10 = PECMD_DevconUpdate(lVar6, pWVar7, pWVar13, -1);
                    lVar11 = (int64_t)uVar10;
set_result:
                    task[4] = lVar11;
                }
                iVar9 = iVar12 + 1;
                if ((1 < iVar9) || (task[4] == 0)) break;
            }
        } else {
            if (*(int *)(lVar6 + 0x1a0) < 0) {
                return 0;
            }
            FUN_14002B9EC(lVar6, pWVar7, (uint32_t)uVar3 << 0x10);
        }
        if ((task[4] == 0) && (0 < *(int *)(lVar6 + 0x1a0))) {
            EnterCriticalSection((LPCRITICAL_SECTION)&g_csInit);
            piVar1 = (int *)(lVar6 + 0x1a0);
            *piVar1 = *piVar1 - 1;
            if (*piVar1 == 0) {
                *(uint32_t *)(lVar6 + 0x1a0) = 0xffffffffU;
            }
            LeaveCriticalSection((LPCRITICAL_SECTION)&g_csInit);
        }
    }
    return 0;
}

/* ========== PECMD_DownCheckShutdown @0x14003d92c ==========
 * 读取 PELOGON 的 DOWNCFG 配置，决定 MENU 或 MENU-SKIP 流程。
 */
int64_t PECMD_DownCheckShutdown(void)
{
    uint32_t uVar1;
    DWORD DVar2;
    DWORD DVar3;
    int64_t lVar4;
    uint64_t uVar5;
    uint32_t uVar6;
    uint32_t uVar7;
    uint32_t uVar8;
    DWORD local_res10[2];
    DWORD local_res18[2];
    uint64_t local_res20;
    WCHAR local_a8[68];

    uVar7 = g_ShowWindow & 2;
    uVar8 = (g_ShowWindow & 4) << 0xc;
    uVar6 = 0;
    if (g_ShowWindow == 0) {
        uVar6 = 2;
    }
    if ((g_ShowWindow & 1) != 0) {
        uVar6 = 0x20;
    }
    if ((g_ShowWindow & 8) != 0) {
        uVar6 = 0x40;
    }
    if ((g_ShowWindow & 0x42) != 0) {
        uVar6 = 0x10;
    }
    if ((g_ShowWindow & 4) == 0) {
        uVar8 = PECMD_ReadPelogonFlag(WSTR("FORCESHUTDOWN"));
    }
    if (uVar6 == 0) {
        uVar6 = (-(uint32_t)(uVar7 != 0) & 0xffffffd0U) + 0x40;
    }
    uVar1 = PECMD_ReadRamdataDword(WSTR("LOCKDOWN"));
    DVar2 = GetTickCount();
    local_res20 = 0;
    local_res18[0] = 0;
    local_res10[0] = 8;
    DVar3 = FUN_14005C4E0(HKEY_LOCAL_MACHINE, WSTR("SOFTWARE\\PELOGON"),
                           WSTR("DOWNCFG"), local_res18, (BYTE *)&local_res20, local_res10);
    uVar5 = 0;
    if (DVar3 == 0) {
        uVar5 = local_res20;
    }
    if ((uVar5 & 1) == 0) {
        if (uVar1 != 0) {
            if ((DVar2 & 0xfffffffeU) - (uVar1 & 0xfffffffeU) < 3000) {
                if (g_ShowWindow == 0) goto skip_menu;
            }
        }
        wsprintfW(local_a8, WSTR("MENU:0x%X:0x%X"), g_ShowWindow, (uint64_t)uVar1);
        if ((uVar6 & 0x70) != 0) {
            PECMD_InitRamdataRegistry(0);
            ((LONG (*)(HKEY, LPCWSTR, LPCWSTR, DWORD))g_pSHSetValueW)
                (HKEY_LOCAL_MACHINE, WSTR("SOFTWARE\\PELOGON\\RAMDATA"), WSTR("LOCKDOWN"), 4);
        }
        lVar4 = PECMD_PerformSystemShutdown(uVar7, uVar6 | uVar8 | 1, local_a8);
    } else {
skip_menu:
        wsprintfW(local_a8, WSTR("MENU-SKIP:0x%X:0x%X/%u"), g_ShowWindow,
                  (uint64_t)uVar1);
        PECMD_RunShutdownScript(local_a8, uVar6);
        lVar4 = 0;
    }
    return lVar4;
}
