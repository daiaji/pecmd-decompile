/* ====================================================================
 * core_b2d.c — B2 批次小函数 4 (140020000-14003ffff)
 *
 * 本批新实现函数全部使用人类可读 PECMD_ 名称，原始地址保留在 @0x 注释。
 *
 * 来源: PECMD原始.EXE (x64, ImageBase=0x140000000)
 *   补丁 INF 复制/删除指令 PECMD_PatchInfDirectives @0x140021144
 *   设置 PELOGON Shell    FUN_140022A70 @0x140022a70
 *   解析命令块/变量段    PECMD_ParseCommandBlock @0x140025474
 *   格式化 ImDisk 虚拟盘 FUN_1400279D8 @0x1400279d8
 *   编辑框子类窗口过程   PECMD_EditSubclassWndProc @0x140028708
 *   应用 PELOGON 布局字体 FUN_14002A508 @0x14002a508
 *   安装/解压 INF/SYS 文件 FUN_14002C048 @0x14002c048
 *   PELOGON 初始化扩展    FUN_14002E3D4 @0x14002e3d4
 *   PELOGON 主窗口过程    FUN_14002EE44 @0x14002ee44
 *   应用 LOGO 配置        FUN_1400389C4 @0x1400389c4
 *   处理单个安装项        FUN_14003AAD0 @0x14003aad0
 *   处理安装脚本队列      FUN_14003B010 @0x14003b010
 *   发送按键/鼠标命令     FUN_14003C9E8 @0x14003c9e8
 *   系统关机/重启处理     FUN_14003D608 @0x14003d608
 *   确认对话框过程        FUN_14003E220 @0x14003e220
 *
 * 约定:
 *   - 新实现函数使用 PECMD_ 可读名；未实现依赖仍 extern FUN_ + TODO(verify)
 *   - 全局数据符号先用 extern 声明; 完整链接时统一在 core_globals.c 定义
 *   - 反编译中的 Ghidra 扩展宏 (SUB168/SEXT816/CONCATxx) 用等价整数运算替代
 *   - 个别反编译缺参/寄存器残留处按调用语义还原并标 TODO(verify)
 * ==================================================================== */
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>

#include "pecmd_defs.h"

/* ---- 局部补充类型/API 声明 ---- */
typedef void *HDROP;
typedef LRESULT (*WNDPROC)(HWND, UINT, WPARAM, LPARAM);
extern LPWSTR lstrcatW(LPWSTR, LPCWSTR);
extern int wsprintfA(LPSTR, LPCSTR, ...);
extern LPSTR StrStrA(LPCSTR, LPCSTR);
extern LRESULT CallWindowProcW(WNDPROC, HWND, UINT, WPARAM, LPARAM);
extern LRESULT DefWindowProcW(HWND, UINT, WPARAM, LPARAM);
extern void mouse_event(DWORD, DWORD, DWORD, DWORD, int64_t);
extern BOOL EndDialog(HWND, intptr_t);
extern intptr_t DialogBoxIndirectParamW(HINSTANCE, const void *, HWND, void *, LPARAM);

/* ---- 已实现公共工具 (其他 core_*.c / core_globals.c) ---- */
extern void PECMD_OpenFileHandle(HANDLE *out, LPCWSTR path, DWORD access, DWORD share,
                           LPSECURITY_ATTRIBUTES sa, DWORD disp, DWORD flags,
                           HANDLE tmpl);                     /* @0x140003864 */
extern void FUN_1400633A8(void **ps, int64_t len);            /* @0x1400633a8 */
extern int64_t *FUN_1400702F0(int64_t *out, const char *src, uint64_t len); /* @0x1400702f0 */
extern WCHAR *FUN_1400702D4(WCHAR **out, LPCWSTR src, int64_t len);
extern WCHAR *FUN_1400679DC(uint64_t *a1, int *a2, int16_t a3);

/* ---- 未实现依赖 (extern + TODO(verify)) ---- */
extern void PECMD_ParseIntThenSkip(int64_t *a1, int *a2);
extern int64_t FUN_14000e26c(void *script, void *cmd, void *s3, void *s4,
                             uint32_t flag, void *p6, void *s7, void *p8);
extern LPCWSTR FUN_1400169BC(int id, void **pp);
extern void FUN_14001708c(WCHAR *out, uint64_t a2, LPCWSTR a3,
                          void *a4);
extern WCHAR **PECMD_SkipLeadingControls(WCHAR **pp);
extern int64_t *PECMD_SkipWsByte(int64_t *a1);
extern void *PECMD_CreateMutexToSlot(void *out, LPCWSTR name);
extern void PECMD_ReleaseMutex(void *out);
extern void PECMD_AppendDebugLog(LPCSTR msg);
extern void FUN_140018d8c(uint64_t a1, LPCWSTR a2, uint64_t a3,
                          uint64_t a4);
extern uint32_t PECMD_GetWinlogonBackground(void);
extern uint64_t *PECMD_NormalizeNewlines(uint64_t *a1);
extern uint64_t PECMD_DrawTooltipText(uint64_t a1, HDC a2, HWND a3);
extern HWND PECMD_GetPELogonWindow(void);
extern DWORD FUN_1400195F0(uint64_t a1, int64_t a2, int a3,
                           uint64_t *a4);
extern void PECMD_RunFbwfHookScript(void);
extern void PECMD_TerminateJobObject(int64_t a1);
extern bool PECMD_MatchDeviceClass(int64_t a1, WCHAR *a2, uint64_t a3);
extern void FUN_14001a56c(int a1);
extern uint32_t PECMD_ReadPelogonFlag(LPCWSTR a1);
extern void PECMD_EnumCDRomDrives(int64_t *a1);
extern void PECMD_ReadPelogonReg(LPCWSTR a1, WCHAR *a2, int a3);
extern void PECMD_ApplyDesktopWallpaper(void);
extern uint64_t PECMD_WritePELogonDword(uint64_t a1, LPCWSTR a2, uint32_t a3);
extern void FUN_14001b850(void);
extern uint32_t PECMD_DeviceIoControlQuery(WCHAR *a1);
extern char PECMD_DevLockUnlock(uint64_t a1, LPCWSTR a2, uint32_t a3,
                          uint32_t a4);
extern void PECMD_ParseNumberWs(int64_t *a1, double *a2);
extern uint64_t PECMD_CreateDirectoryTree(LPCWSTR path);
extern uint64_t PECMD_ExecLoadCommand(LPCWSTR a1, LPCWSTR a2);
extern void PECMD_HandleDropFile(uint8_t *a1, void *a2, char a3);
extern HICON PECMD_LoadIcon(LPCWSTR a1, uint64_t *a2);
extern uint32_t PECMD_ParseVkKeyName(LPCWSTR a1, char a2);
extern void FUN_14002286c(void);
extern void FUN_140022e94(void);
extern void PECMD_RunShutdownScript(LPCWSTR a1, uint32_t a2);
extern WCHAR *PECMD_RemoveDuplicateChar(LPCWSTR a1, WCHAR a2);
extern LPCWSTR PECMD_TrimTrailingSeparator(int64_t *a1, LPCWSTR a2, WCHAR a3);
extern void FUN_140025CE0(int64_t *a1, char *a2, uint8_t *a3,
                          char *a4, char *a5, void *a6);
extern void FUN_140025f10(int64_t ctx, LPCWSTR msg, uint32_t code, void *p4,
                          void *p5, int64_t *p6);
extern uint64_t FUN_140027EAC(uint64_t a, int64_t *b, uint32_t c, uint64_t d,
                              uint64_t e, uint32_t f, uint64_t g, int64_t h,
                              int i);
extern void PECMD_StartOnlyApp(LPCWSTR a1);
extern uint32_t PECMD_RunProcessCommand(int64_t a1, LPCWSTR a2, uint64_t a3,
                              uint64_t a4, uint64_t a5);
extern LPCWSTR FUN_14002B2EC(int64_t *a1, uint64_t a2, LPCSTR a3);
extern uint32_t FUN_14002B7F8(int64_t a1, LPCWSTR a2, LPCWSTR a3,
                              uint32_t a4, int a5, void *a6);
extern DWORD FUN_14002B9EC(int64_t a1, LPCWSTR a2, uint32_t a3);
extern uint32_t PECMD_DevconUpdate(int64_t a1, LPCWSTR a2, LPCWSTR a3,
                              int a4);
extern uint32_t FUN_14002C634(int64_t a1, LPCWSTR a2, LPCWSTR a3,
                              int a4);
extern uint64_t FUN_14002cc30(int64_t *a1, LPCWSTR a2, int a3,
                              int a4, WCHAR *a5);
extern void FUN_14002E790(HWND a1);
extern int64_t PECMD_RunCommand(void *script, WCHAR *cmd);
extern void PECMD_ScriptWndProc(HWND a1, uint32_t a2, HDC a3,
                          void *a4);
extern uint64_t FUN_14003DB00(WCHAR *a1);
extern uint64_t FUN_14003A810(int64_t *a1);
extern uint64_t FUN_14004c0bc(uint64_t a1, uint64_t a2, void *p3,
                              void *p4, void *p5);
extern void FUN_14005B900(void *a1, uint64_t a2, LPCWSTR a3,
                          HICON a4, uint32_t a5);
extern DWORD FUN_14005c5a0(HKEY root, LPCWSTR sub, LPCWSTR name, DWORD type,
                           BYTE *data, DWORD size);
extern LARGE_INTEGER PECMD_SetFilePointer(HANDLE a1, LARGE_INTEGER a2,
                                   DWORD a3);
extern void PECMD_TrimWorkingSet(char a1);
extern uint64_t PECMD_GetPackedSystemVersion(void);
extern void FUN_1400675b8(int64_t *src, int64_t *dst, int16_t delim);
extern void FUN_1400679b0(WCHAR **pp, int *out, WCHAR sep);
extern uint16_t *FUN_14006B1E8(LPCWSTR a1, uint64_t *a2, char a3);
extern char FUN_1400660AC(char *a1, int64_t *a2, int a3);
extern BOOL PECMD_DosDeviceMount(LPCWSTR a1, LPCWSTR a2, WCHAR *a3,
                          uint32_t a4, char a5);
extern void FUN_140077358(void);
extern bool PECMD_ParseNumber(int64_t *a1, int *a2);
extern int64_t *FUN_140078E90(int64_t *a1);
extern int64_t FUN_14007A224(void *script, WCHAR *text, WCHAR **out, int c,
                             uint8_t d);
extern uint64_t PECMD_TokenizeExpression(int64_t a1, int64_t a2, WCHAR **a3,
                              uint32_t a4, WCHAR *a5);
extern HFONT FUN_1400B1F34(int *a1, double *a2, LPCWSTR a3);
extern int64_t FUN_1400B638C(void *script, LPCWSTR buf, LPCWSTR a3, LPCWSTR a4,
                             uint32_t flags, LPCWSTR a6, int64_t *a7);
extern void FUN_1400e3cd4(LPCWSTR src, WCHAR **out, int64_t *pos);
extern void FUN_1400E648C(uint64_t *a1, UINT a2);
extern DWORD FUN_1400E693C(HANDLE a1);
extern void *FUN_1400F429C(WCHAR **pp, WCHAR ch);
extern bool FUN_140101E70(LPCWSTR a1);
extern int64_t PECMD_WideStrLen(void *a1);
extern void LoadEnvi(void *a, void *b);

/* ---- 本批引用的全局数据 ---- */
extern WCHAR g_szEmpty[];                    /* g_szEmpty */
extern uint8_t g_bE6b8[];              /* 对话框模板 */
extern double g_dbl20b28;                 /* 默认字体大小 */
extern WCHAR u_____D__140120a40[];           /* .rdata 设备路径串 */
extern uint64_t ram0x000140120a48;           /* .rdata 设备路径串续 */
extern int g_intA238;
extern int g_intA23C;
extern int g_intA240;
extern int g_intA244;
extern uint8_t g_u8A24A;
extern int8_t g_flagA24B;
extern uint8_t g_flagA24E;
extern int g_tooltipTimeout;
extern uint8_t g_flagA298;
extern DWORD g_dwC96C;                  /* 线程 ID 输出 */
extern uint32_t g_bitFlags;
extern uint8_t g_u8CCB1;                /* MAIN_DBG 日志标志 */
extern uint8_t g_topiconFlag;                /* TOPICON 标志 */
extern uint8_t g_logoActiveFlag;                /* LOGO 活动标志 */
extern int (*g_pGdipDisposeImage)();                  /* GdipDisposeImage 函数指针 */
extern HWND g_hPelogonWnd;
extern HWND g_hwndCF78;
extern uint8_t g_tipAlign;
extern uint8_t g_u8CF83;
extern uint8_t g_u8CF84;
extern uint8_t g_u8CF85;
extern HICON g_hIconCF88;
extern int g_tipOffX;
extern int g_tipOffY;
extern HFONT g_hFontFast;
extern HWND g_hTooltipParent;
extern WCHAR *g_pwszD220;
extern WCHAR *g_pwszD250;
extern WCHAR *g_pwszD258;
extern HWND g_hwndTray;
extern HFONT g_hFontE2B0;
extern uint8_t DAT_140147003;
extern COLORREF g_dwTipBkColor;
extern double DAT_140147010;
extern const WCHAR *PTR_u_Shell_14011ddb8_2_14013a290;

/* ========== PECMD_PatchInfDirectives @0x140021144 ==========
 * 扫描 INF 文件中的 "opyfiles"/"elfiles" 行，在系统 INF 目录内外切换
 * C/D 前缀与注释 ';'。TODO(verify): 非 UTF-16 分支的字节读取按反编译原样保留。
 */
void PECMD_PatchInfDirectives(LPCWSTR path)
{
    HANDLE hFile;
    uint8_t *local_68 = NULL;
    WCHAR *local_80 = NULL;
    WCHAR local_60[18];
    uint32_t local_70;
    DWORD bytesRead[2] = {0, 0};
    uint64_t local_res18 = 0;
    uint8_t *local_78;
    uint8_t *puVar6;
    uint8_t *puVar7;
    DWORD DVar3;
    uint16_t uVar10;
    WCHAR local_98[9] = {0};
    char cVar1 = 0;
    int iVar2;

    PECMD_AllocWStringBuffer(&local_80, 0x209);
    memcpy(local_60, WSTR("%SystemRoot%\\INF\\"), 0x24);
    FUN_14007A224((void *)g_Script, local_60, &local_80, 0, 1);
    iVar2 = lstrlenW(local_80);
    iVar2 = StrCmpNIW(path, local_80, iVar2);
    local_70 = (uint32_t)(iVar2 == 0);
    local_res18 = 0;
    PECMD_OpenFileHandle((HANDLE *)&local_res18, path, 0xc0000000, 3, NULL, 3, 0, (HANDLE)0);
    hFile = (HANDLE)local_res18;
    if (hFile != (HANDLE)0) {
        DVar3 = FUN_1400E693C(hFile);
        PECMD_SetFilePointer(hFile, (LARGE_INTEGER)(int64_t)0, 0);
        FUN_1400633A8((void **)&local_68, (int64_t)(int)(DVar3 + 0xc));
        local_78 = local_68;
        bytesRead[0] = 0;
        ReadFile(hFile, local_68, DVar3, bytesRead, NULL);
        if (3 < bytesRead[0]) {
            memset((uint64_t *)(local_68 + bytesRead[0]), 0, 6);
            puVar6 = local_68 + bytesRead[0];
            uVar10 = 0xff;
            DVar3 = 1;
            if (*(uint16_t *)local_68 == 0xfeff) {
                uVar10 = 0xffff;
                DVar3 = 2;
            }
            cVar1 = (*(uint16_t *)local_68 == 0xfeff);
            if (*(uint16_t *)local_68 == 0xfffe) {
                uVar10 = 0xffff;
                DVar3 = 2;
                cVar1 = 3;
            }
            puVar7 = local_68;
            while (puVar7 < puVar6) {
                int64_t n;
                uint8_t *puVar9 = puVar7;
                for (n = 0; n < 9; n++) {
                    if (cVar1 == 3) {
                        local_98[n] = (WCHAR)((WCHAR)puVar9[1] + (WCHAR)puVar9[0] * 0x100);
                    } else if (DVar3 == 2) {
                        local_98[n] = (WCHAR)(uVar10 & *(uint16_t *)puVar9);
                    } else {
                        local_98[n] = (WCHAR)(uVar10 & *puVar9);
                    }
                    puVar9 += DVar3;
                }
                for (n = 0; n < 6; n++) {
                    local_98[n] = (WCHAR)(local_98[n] | 0x20);
                }
                local_res18 &= ~(uint64_t)0xffffULL;
                if (local_70 == 0) {
                    if (local_98[0] == 0x3b) {
                        if (local_98[1] == 0x6f && local_98[2] == 0x70 &&
                            local_98[3] == 0x79 && local_98[4] == 0x66 &&
                            local_98[5] == 0x69 && local_98[6] == 0x6c &&
                            local_98[7] == 0x65 && local_98[8] == 0x73) {
                            PECMD_SetFilePointer(hFile, (LARGE_INTEGER)(int64_t)(puVar7 - local_78), 0);
                            bytesRead[0] = 0;
                            local_res18 = (local_res18 & ~(uint64_t)0xffffULL) | 0x43;
                            WriteFile(hFile, &local_res18, DVar3, bytesRead, NULL);
                        } else if (local_98[1] == 0x65 && local_98[2] == 0x6c &&
                                   local_98[3] == 0x66 && local_98[4] == 0x69 &&
                                   local_98[5] == 0x6c && local_98[6] == 0x65 &&
                                   local_98[7] == 0x73) {
                            PECMD_SetFilePointer(hFile, (LARGE_INTEGER)(int64_t)(puVar7 - local_78), 0);
                            local_res18 = (local_res18 & ~(uint64_t)0xffffULL) | 0x44;
                            WriteFile(hFile, &local_res18, DVar3, bytesRead, NULL);
                        }
                    }
                } else {
                    bool isCopy = local_98[0] == 0x63 &&
                                  local_98[1] == 0x6f && local_98[2] == 0x70 &&
                                  local_98[3] == 0x79 && local_98[4] == 0x66 &&
                                  local_98[5] == 0x69 && local_98[6] == 0x6c &&
                                  local_98[7] == 0x65 && local_98[8] == 0x73;
                    bool isDel = local_98[0] == 0x64 &&
                                 local_98[1] == 0x65 && local_98[2] == 0x6c &&
                                 local_98[3] == 0x66 && local_98[4] == 0x69 &&
                                 local_98[5] == 0x6c && local_98[6] == 0x65 &&
                                 local_98[7] == 0x73;
                    if (isCopy || isDel) {
                        PECMD_SetFilePointer(hFile, (LARGE_INTEGER)(int64_t)(puVar7 - local_78), 0);
                        local_res18 = (local_res18 & ~(uint64_t)0xffffULL) | 0x3b;
                        ReadFile(hFile, &local_res18, DVar3, bytesRead, NULL);
                    }
                }
                if (puVar6 <= puVar7) {
                    break;
                }
                do {
                    WCHAR ch;
                    if (cVar1 == 3) {
                        ch = (WCHAR)((WCHAR)puVar7[0] * 0x100 + puVar7[1]);
                    } else if (DVar3 == 2) {
                        ch = (WCHAR)(uVar10 & *(uint16_t *)puVar7);
                    } else {
                        ch = (WCHAR)(uVar10 & *puVar7);
                    }
                    puVar7 += DVar3;
                    if (ch == 10 || puVar7 >= puVar6) {
                        break;
                    }
                } while (true);
            }
        }
        FUN_14005B104((WCHAR **)&local_68);
        if (hFile != (HANDLE)-1) {
            CloseHandle(hFile);
        }
    }
    FUN_14005B104(&local_80);
}

/* ========== FUN_140022A70 @0x140022a70 ==========
 * 保存/读取 PELOGON Shell 与 FirstUsb、Hide、HOOKKBD、TOPICON、AUTOUSB 设置。
 */
void FUN_140022A70(uint64_t shell, int mode)
{
    WCHAR *pWVar1;
    WCHAR *puVar2;
    WCHAR *psVar3;
    int iVar4;
    int64_t lVar5;
    const WCHAR *pwVar6;
    bool bVar7;
    int local_res10[2];
    uint32_t local_res18[2];
    uint32_t local_res20[2];
    WCHAR *local_48 = NULL;

    (void)shell;
    psVar3 = g_pwszD250;
    if (mode == 1) {
        if (g_pwszD250 != NULL) {
            bVar7 = false;
            if ((g_runFlag & 0x20) != 0) {
                goto label_shell_name;
            }
            lVar5 = PECMD_WideStrLen((void *)g_pwszD250);
            ((void (*)(HKEY, LPCWSTR, LPCWSTR, DWORD, void *, DWORD))g_pSHSetValueW)(
                HKEY_LOCAL_MACHINE, WSTR("SOFTWARE\\PELOGON"), WSTR("SysShell"), 1,
                psVar3, (int)lVar5 * 2);
        }
        bVar7 = (g_runFlag & 0x20) == 0;
label_shell_name:
        pwVar6 = PTR_u_Shell_14011ddb8_2_14013a290;
        if (!bVar7) {
            pwVar6 = WSTR("UShell");
        }
        ((void (*)(HKEY, LPCWSTR, LPCWSTR))g_pSHDeleteValueW)(
            HKEY_LOCAL_MACHINE, WSTR("SOFTWARE\\PELOGON"), pwVar6);
        return;
    }

    PECMD_AllocWStringBuffer(&local_48, 0x34d3);
    pWVar1 = local_48 + 0x20a;
    pwVar6 = PTR_u_Shell_14011ddb8_2_14013a290;
    if ((g_runFlag & 0x20) != 0) {
        pwVar6 = WSTR("UShell");
    }
    puVar2 = local_48 + 0x105;
    local_res20[0] = 1;
    local_res18[0] = 26000;
    PECMD_ReadPelogonReg(pwVar6, pWVar1, 13000);
    if (g_u8CCB1 != 0) {
        FUN_140018d8c((uint64_t)(uintptr_t)g_Script,
                      WSTR("MAIN_DBG:%d/0x%X MyShell[%s] [%s] 0x%X\r\n"), 0x242c,
                      (uint64_t)(uint32_t)(int8_t)g_runFlag);
    }
    if ((*pWVar1 != L'\0') && ((g_runFlag & 0x20) == 0) &&
        (PECMD_ReadPelogonReg(WSTR("UShell"), local_48, 0x104), *local_48 != 0)) {
        *pWVar1 = L'\0';
    }
    FUN_1400703E4(&g_pwszD250, pWVar1);
    if ((mode == -1) && (*pWVar1 == L'\0')) {
        goto label_done;
    }
    local_res20[0] = 4;
    local_res10[0] = 0;
    local_res18[0] = 4;
    iVar4 = ((LONG (*)(HKEY, LPCWSTR, LPCWSTR, uint32_t *, void *, uint32_t *))g_pSHGetValueW)(
        HKEY_LOCAL_MACHINE, WSTR("SOFTWARE\\PELOGON"), WSTR("Hide"), local_res20,
        local_res10, local_res18);
    if ((iVar4 == 0) && (local_res10[0] != 0)) {
        DAT_140147003 = 1;
    }
    local_res10[0] = 0;
    local_res18[0] = 4;
    iVar4 = ((LONG (*)(HKEY, LPCWSTR, LPCWSTR, uint32_t *, void *, uint32_t *))g_pSHGetValueW)(
        HKEY_LOCAL_MACHINE, WSTR("SOFTWARE\\PELOGON"), WSTR("HOOKKBD"), local_res20,
        local_res10, local_res18);
    if ((iVar4 == 0) && (local_res10[0] == 1)) {
        FUN_14001b850();
    }
    local_res10[0] = 0;
    local_res18[0] = 4;
    iVar4 = ((LONG (*)(HKEY, LPCWSTR, LPCWSTR, uint32_t *, void *, uint32_t *))g_pSHGetValueW)(
        HKEY_LOCAL_MACHINE, WSTR("SOFTWARE\\PELOGON"), WSTR("TOPICON"), local_res20,
        local_res10, local_res18);
    if ((iVar4 == 0) && (local_res10[0] == 1)) {
        g_topiconFlag = 1;
    }
    local_res10[0] = 0;
    local_res18[0] = 4;
    iVar4 = ((LONG (*)(HKEY, LPCWSTR, LPCWSTR, uint32_t *, void *, uint32_t *))g_pSHGetValueW)(
        HKEY_LOCAL_MACHINE, WSTR("SOFTWARE\\PELOGON"), WSTR("AUTOUSB"), local_res20,
        local_res10, local_res18);
    if (iVar4 == 0) {
        if (local_res10[0] == 1) {
            if (g_flagA24B >= 0) {
                goto label_firstusb;
            }
            g_flagA24B = 1;
        }
        if ((local_res10[0] == 0) && (0 < g_flagA24B)) {
            g_flagA24B = -1;
        }
    }
label_firstusb:
    *puVar2 = 0;
    local_res18[0] = 0x208;
    iVar4 = ((LONG (*)(HKEY, LPCWSTR, LPCWSTR, uint32_t *, void *, uint32_t *))g_pSHGetValueW)(
        HKEY_LOCAL_MACHINE, WSTR("SOFTWARE\\PELOGON"), WSTR("FirstUsb"), local_res20,
        puVar2, local_res18);
    if (iVar4 == 0) {
        *puVar2 = (WCHAR)(*puVar2 & 0xffdf);
    }
    if ((0x40 < *puVar2) && (*puVar2 < 0x5b)) {
        g_flagA24E = (uint8_t)*puVar2;
    }
label_done:
    FUN_14005B104(&local_48);
}

/* ========== PECMD_ParseCommandBlock @0x140025474 ==========
 * 解析命令行中的 {} / [] 块与变量展开，返回最后一条命令/变量的结果。
 * TODO(verify): threadInfo 在反编译中是 CRT 线程信息，这里按 void* 保留。
 */
uint64_t PECMD_ParseCommandBlock(int64_t script, WCHAR **pp, uint32_t flags,
                                 void *threadInfo)
{
    uint64_t uVar4 = 0;
    bool bVar2 = false;
    WCHAR WVar12 = L'|';
    int16_t sVar1;
    WCHAR *pWVar5;
    WCHAR *pWVar9;
    WCHAR *pWVar10;
    WCHAR *pWVar11;
    WCHAR WVar8;
    LARGE_INTEGER local_res10;
    LPCWSTR local_58;
    LPCWSTR local_48;
    WCHAR *local_40 = NULL;
    WCHAR *local_50 = NULL;

    if ((flags & 2) != 0) {
        sVar1 = *(int16_t *)*pp;
        bVar2 = false;
        while ((sVar1 != 0) &&
               ((pWVar5 = *pp, (uint16_t)*pWVar5 < 9 || (0xd < (uint16_t)*pWVar5)) &&
                (*pWVar5 != L' '))) {
            if (*pWVar5 == L'<') {
                bVar2 = true;
                *pp = pWVar5 + 1;
            } else if (*pWVar5 == L'^' || *pWVar5 == L'#' || *pWVar5 == L'~' ||
                       *pWVar5 == L'+' || *pWVar5 == L'-' || *pWVar5 == L'@' ||
                       *pWVar5 == L'$' || *pWVar5 == L'*' || *pWVar5 == L'=' ||
                       *pWVar5 == L'?' || *pWVar5 == L'/' || *pWVar5 == L'\\' ||
                       *pWVar5 == L'%' || *pWVar5 == L';' || *pWVar5 == L',' ||
                       *pWVar5 == L'.' || *pWVar5 == 0x2222 || *pWVar5 == L'\'') {
                WVar12 = *pWVar5;
                *pp = pWVar5 + 1;
            }
            sVar1 = *(int16_t *)*pp;
        }
    }

    do {
        WVar8 = L'{';
        if (*pp == NULL) {
            return (uint64_t)(int)uVar4;
        }
        PECMD_SkipLeadingControls(pp);
        pWVar9 = *pp;
        if ((pWVar9 == NULL) || (*pWVar9 == L'\0')) {
            return uVar4;
        }
        if (WVar8 == *pWVar9) {
            pWVar9 = pWVar9 + 1;
            *pp = pWVar9;
            PECMD_SkipLeadingControls(pp);
            if (*(int16_t *)*pp != 0) {
                *pp = pWVar9;
                uVar4 = PECMD_TokenizeExpression(script, *(int64_t *)(script + 0x40),
                                      pp, 1, g_szEmpty);
            }
            pWVar5 = PECMD_RemoveDuplicateChar(*pp, WVar12);
            *pp = pWVar5;
            if (pWVar5 != NULL) {
                *pp = pWVar5 + 1;
            }
            continue;
        }
        if (StrCmpNIW(WSTR("[]"), pWVar9, 2) == 0) {
            pWVar9 = *pp;
            *pp = pWVar9 + 2;
            PECMD_SkipLeadingControls(pp);
            WVar8 = **pp;
            while ((WVar8 != L'\0') && (pWVar5 = *pp, *pWVar5 != WVar12) &&
                   (*pWVar5 != L'{')) {
                *pp = pWVar5 + 1;
                WVar8 = pWVar5[1];
            }
            if (*(int16_t *)*pp != 0x7b) {
                pWVar5 = PECMD_RemoveDuplicateChar(*pp, WVar12);
                *pp = pWVar5;
                if (pWVar5 != NULL) {
                    *pp = pWVar5 + 1;
                }
                continue;
            }
            FUN_1400702D4(&local_40, pWVar9, (*pp - pWVar9) >> 1);
            pWVar11 = *pp + 2;
            *pp = pWVar11;
            PECMD_SkipLeadingControls(pp);
            pWVar9 = *pp;
            local_58 = pWVar11;
            PECMD_AllocWStringBuffer(&local_50, 1);
            WVar8 = *pWVar9;
            while (WVar8 != L'\0') {
                pWVar5 = PECMD_RemoveDuplicateChar(pWVar9, WVar12);
                pWVar11 = pWVar5;
                if (pWVar5 == NULL) {
                    *pp = NULL;
                    break;
                }
                do {
                    pWVar11 = pWVar11 - 1;
                    if (pWVar11 < pWVar9) {
                        goto label_025783;
                    }
                } while ((8 < (uint16_t)*pWVar11 && (uint16_t)*pWVar11 < 0xe) ||
                         *pWVar11 == L' ');
                if ((pWVar9 <= pWVar11) && (*pWVar11 == L'}')) {
                    PECMD_StrCopyW(&local_50, local_58, (int64_t)(pWVar11 - local_58) >> 1);
                    *pp = pWVar5 + 1;
                    break;
                }
label_025783:
                pWVar9 = pWVar5;
                WVar8 = *pWVar5;
            }
            local_58 = local_50;
            local_48 = local_50;
            PECMD_SkipLeadingControls((WCHAR **)&local_48);
            if (*local_48 != L'\0') {
                uVar4 = PECMD_TokenizeExpression(script, *(int64_t *)(script + 0x40),
                                      (WCHAR **)&local_58, 1, local_40);
                uVar4 = uVar4 & 0xffffffff;
            }
            FUN_14005B104(&local_50);
            FUN_14005B104(&local_40);
        } else {
            if (((flags & 1) != 0) && (*(int16_t *)*pp == 0x7d)) {
                *pp = *pp + 2;
                PECMD_SkipLeadingControls(pp);
                return uVar4;
            }
            memset(&local_res10, 0, sizeof(local_res10));
            FUN_140063620(&local_res10);
            if (bVar2) {
                pWVar5 = (WCHAR *)PECMD_TrimTrailingSeparator((int64_t *)&local_res10, *pp, WVar12);
            } else {
                pWVar5 = PECMD_RemoveDuplicateChar(*pp, WVar12);
                pWVar10 = pWVar5;
                if ((flags & 1) != 0) {
                    if (pWVar5 == NULL) {
                        int iVar3 = lstrlenW(*pp);
                        pWVar10 = *pp - 2 + iVar3 * 2;
                    } else {
                        pWVar10 = pWVar5 - 1;
                    }
                    if (*pp <= pWVar10) {
                        do {
                            if (((uint16_t)*pWVar10 < 9 || (0xd < (uint16_t)*pWVar10)) &&
                                (*pWVar10 != L' ')) {
                                break;
                            }
                            pWVar10 = pWVar10 - 1;
                        } while (*pp <= pWVar10);
                        if ((*pp <= pWVar10) && (*pWVar10 == L'}')) {
                            goto label_0258c3;
                        }
                    }
                    pWVar10 = pWVar5;
                }
label_0258c3:
                PECMD_StrCopyW((WCHAR **)&local_res10, *pp,
                                 (int64_t)(pWVar10 - *pp) >> 1);
            }
            {
                LARGE_INTEGER LVar6;
                LVar6.QuadPart = (int64_t)FUN_14004c0bc((uint64_t)script,
                                                        (uint64_t)local_res10.QuadPart,
                                                        NULL, NULL, threadInfo);
                uVar4 = (uint64_t)(int32_t)LVar6.LowPart;
            }
            if ((pWVar5 == NULL) || (*(int16_t *)(script + 200) != 0)) {
                FUN_14005B104((WCHAR **)&local_res10);
                return uVar4;
            }
            *pp = pWVar5;
            if (!bVar2) {
                *pp = pWVar5 + 1;
            }
            FUN_14005B104((WCHAR **)&local_res10);
        }
    } while (true);
}

/* ========== FUN_1400279D8 @0x1400279d8 ==========
 * 调用 format.com 格式化 ImDisk 虚拟盘，并在需要时建立/删除 DOS 设备名。
 * TODO(verify): _snwprintf 的 %u 实参按语义补 script。
 */
DWORD FUN_1400279D8(uint32_t unit, WCHAR *drive, LPCWSTR label)
{
    WCHAR WVar1;
    WCHAR WVar7;
    int iVar2;
    int iVar4;
    BOOL BVar3;
    HANDLE pvVar5;
    DWORD DVar8 = 8;
    DWORD DVar9;
    LPCWSTR lpString;
    DWORD local_res10[4];
    WCHAR local_res20[4];
    LPWSTR local_2f8 = NULL;
    HANDLE local_2f0;
    PROCESS_INFORMATION local_2e8;
    WCHAR local_2d0[4];
    uint64_t uStack_2c8;
    STARTUPINFOW local_2b8;
    WCHAR local_248[264];

    WVar7 = L'\0';
    if (((drive != NULL) && (*drive != L'\0')) && (drive[1] == L':') &&
        ((drive[2] == L'\0' || (drive[2] == L'\\' && drive[3] == L'\0')))) {
        WVar7 = *drive;
    }
    local_res20[1] = 0x3a;
    local_res20[0] = 0xff;
    local_res20[2] = 0;
    iVar2 = lstrlenW(label);
    FUN_1400633A8((void **)&local_2f8, (int64_t)(iVar2 * 2 + 0x4a));
    local_2b8.cb = 0x68;
    memset(&local_2b8.lpReserved, 0, 0x60);
    local_2b8.dwFlags = local_2b8.dwFlags | 1;
    local_2b8.wShowWindow = 0;
    local_2d0[0] = u_____D__140120a40[0];
    local_2d0[1] = u_____D__140120a40[1];
    local_2d0[2] = u_____D__140120a40[2];
    local_2d0[3] = u_____D__140120a40[3];
    uStack_2c8 = ram0x000140120a48;
    pvVar5 = CreateMutexW(NULL, 0, WSTR("ImDiskFormat"));
    WVar1 = WVar7;
    local_2f0 = pvVar5;
    if (WVar7 == L'\0') {
        unit = PECMD_DeviceIoControlQuery(drive);
        if ((int)unit < 0) {
            FUN_14005B104((WCHAR **)&local_2f8);
            return -unit;
        }
        WVar1 = ((WCHAR (*)(void))g_pImDiskFindFreeLetter)();
    }
    local_res20[0] = WVar1;
    _snwprintf(local_248, 0x103, WSTR("\\Device\\ImDisk%u"), unit);
    if ((local_res20[0] != L'\0') &&
        ((WVar7 != L'\0' ||
          (BVar3 = PECMD_DosDeviceMount(local_248, local_res20, NULL, 1, '\0'), BVar3 != 0)))) {
        lstrcpyW(local_2f8, WSTR("format.com "));
        lstrcatW(local_2f8, local_res20);
        lstrcatW(local_2f8, WSTR(" "));
        iVar2 = 2;
        lpString = label;
        if (*label == L'\"') {
            lpString = label + 1;
            iVar4 = lstrlenW(lpString);
            if ((0 < iVar4) && (label[iVar4] == L'\"')) {
                ((LPWSTR)label)[iVar4] = L'\0';
            }
        }
        lstrcatW(local_2f8, lpString);
        if (!StrStrIW(lpString, WSTR("/X"))) {
            lstrcatW(local_2f8, WSTR(" /Y "));
        }
        if (!StrStrIW(lpString, WSTR("/X"))) {
            lstrcatW(local_2f8, WSTR(" /X "));
        }
        iVar4 = 299;
        uStack_2c8 = (uStack_2c8 & ~(uint64_t)0xffffULL) | local_res20[0];
        do {
            pvVar5 = CreateFileW(local_2d0, 0x80000000, 0, NULL, 3, 0, (HANDLE)0);
            if (pvVar5 != (HANDLE)-1) {
                CloseHandle(pvVar5);
                break;
            }
            FUN_1400195F0((uint64_t)(uintptr_t)g_Script, 100, 0, NULL);
            iVar4 = iVar4 - 1;
        } while (-1 < iVar4);
        iVar4 = 0x1d;
        do {
            BVar3 = CreateProcessW(NULL, local_2f8, NULL, NULL, 1, 0, NULL, NULL,
                                   &local_2b8, &local_2e8);
            if (BVar3 == 0) {
                iVar2 = iVar2 - 1;
                DVar9 = 8;
                if (iVar2 < 0) {
                    goto label_done;
                }
            } else {
                local_res10[0] = 0;
                CloseHandle(local_2e8.hThread);
                WaitForSingleObject(local_2e8.hProcess, INFINITE);
                GetExitCodeProcess(local_2e8.hProcess, local_res10);
                DVar8 = local_res10[0];
                CloseHandle(local_2e8.hProcess);
                if (local_res10[0] == 0) {
                    break;
                }
            }
            FUN_1400195F0((uint64_t)(uintptr_t)g_Script, 100, 0, NULL);
            iVar4 = iVar4 - 1;
        } while (-1 < iVar4);
        if (WVar7 == L'\0') {
            DefineDosDeviceW(7, local_res20, local_248);
        }
        PECMD_DevLockUnlock((uint64_t)unit, NULL, 0x10000, 0);
        pvVar5 = local_2f0;
    }
    DVar9 = DVar8;
    if (pvVar5 != (HANDLE)0) {
        ReleaseMutex(pvVar5);
        CloseHandle(pvVar5);
    }
label_done:
    FUN_14005B104((WCHAR **)&local_2f8);
    return DVar9;
}

/* ========== PECMD_EditSubclassWndProc @0x140028708 ==========
 * 编辑框子类窗口过程：处理回车/Ctrl+A/粘贴、命令行执行和拖放辅助消息。
 */
LRESULT PECMD_EditSubclassWndProc(HWND hwnd, UINT msg, void *wParam, LPARAM lParam)
{
    LPWSTR pWVar4;
    uint8_t uVar2;
    LPWSTR pWVar1;
    int iVar6;
    LPARAM *pLVar7;
    LRESULT LVar8;
    HWND hWnd;
    uint64_t uVar9;
    LPWSTR local_38 = NULL;
    int64_t local_30 = 0;
    LPWSTR local_28 = NULL;
    uint8_t *lVar3 = g_pAppData;

    if ((msg == 0x100) || (msg == 0x102)) {
        if (msg == 0x102) {
            if (*(char *)(lVar3 + 0xd4) != '\0') {
                *(uint8_t *)(lVar3 + 0xd4) = 0;
                return 0;
            }
        } else {
            if (msg != 0x100) {
                goto label_0288a6;
            }
            if ((int)(intptr_t)wParam == 0xd) {
                uint16_t uVar5 = GetAsyncKeyState(0x11);
                if ((uVar5 & 0x8000) != 0) {
                    *(uint8_t *)(lVar3 + 0xd4) = 1;
                    PostMessageW(hwnd, 0x44d, 0, 0);
                    return 0;
                }
            } else if ((int)(intptr_t)wParam == 0x41) {
                uint16_t uVar5 = GetAsyncKeyState(0x11);
                if ((uVar5 & 0x8000) != 0) {
                    SendMessageW(hwnd, 0xb1, 0, -1);
                }
            } else if (wParam == (void *)0x72) {
                hWnd = GetParent(hwnd);
                SendMessageW(hWnd, 0x100, 0x72, lParam);
                goto label_0288a6;
            }
        }
label_0287f3:
        local_30 = 0;
        if (msg == 0x302) {
            wParam = (void *)1;
            PECMD_AllocString(&local_30, 1);
            FUN_140078E90(&local_30);
            msg = 0xc2;
            pLVar7 = PECMD_NormalizeNewlines((uint64_t *)&local_30);
            lParam = *pLVar7;
        }
        LVar8 = CallWindowProcW((WNDPROC)(uintptr_t)(*(uint64_t *)(lVar3 + 0xe8)),
                                hwnd, msg, (WPARAM)wParam, lParam);
        FUN_14005B104((WCHAR **)&local_30);
    } else {
label_0288a6:
        if (msg == 0x44d) {
            iVar6 = GetWindowTextLengthW(hwnd);
            PECMD_AllocWStringBuffer(&local_38, (int64_t)iVar6 + 0x4b);
            pWVar4 = local_38;
            *local_38 = L'\0';
            iVar6 = GetWindowTextW(hwnd, local_38, iVar6 + 0xb);
            if (iVar6 < 0) {
                iVar6 = 0;
            }
            memset((uint64_t *)(local_38 + iVar6), 0, 0x30);
            iVar6 = lstrlenW(local_38);
            pWVar1 = local_38 + iVar6;
            *pWVar1 = L'\r';
            pWVar1[1] = L'\n';
            pWVar1[2] = L'\0';
            memset((uint64_t *)(pWVar1 + 3), 0, 0x20);
            uVar9 = PECMD_ExecLoadCommand(NULL, local_38);
            if ((int)uVar9 != 0) {
                PECMD_AllocWStringBuffer(&local_28, 0x105);
                GetCurrentDirectoryW(0x104, local_28);
                uVar2 = g_charTableF;
                g_charTableF = 1;
                local_38 = NULL;
                FUN_1400B638C((void *)g_Script, pWVar4, g_szEmpty, NULL, 0, NULL, NULL);
                g_charTableF = uVar2;
                SetCurrentDirectoryW(local_28);
                FUN_14005B104(&local_28);
                FUN_14005B104(&local_38);
                return 1;
            }
            FUN_14005B104(&local_38);
        } else {
            if (msg != 0x233) {
                goto label_0287f3;
            }
            PECMD_HandleDropFile(g_pAppData, wParam, '\0');
        }
        LVar8 = 1;
    }
    return LVar8;
}

/* ========== FUN_14002A508 @0x14002a508 ==========
 * 解析 PELOGON 配置串（位置/字体/延迟），更新窗口、字体与重绘区域。
 */
uint64_t FUN_14002A508(uint64_t value, LPCWSTR text)
{
    WCHAR WVar1;
    int iVar2;
    int iVar3;
    int iVar4;
    int iVar5;
    double dVar6;
    LPCWSTR pWVar7;
    BOOL BVar8;
    RECT *lpRect;
    LPCWSTR pWVar9;
    WCHAR *local_res10;
    int local_res18[2];
    int local_res20[2];
    union {
        double d;
        RECT r;
    } local_b8;
    struct {
        LONG lfHeight;
        LONG lfWidth;
        LONG lfEscapement;
        LONG lfOrientation;
        LONG lfWeight;
        BYTE lfItalic;
        BYTE lfUnderline;
        BYTE lfStrikeOut;
        BYTE lfCharSet;
        BYTE lfOutPrecision;
        BYTE lfClipPrecision;
        BYTE lfQuality;
        BYTE lfPitchAndFamily;
        WCHAR lfFaceName[32];
    } local_lf;

    dVar6 = DAT_140147010;
    iVar5 = g_intA244;
    iVar4 = g_intA240;
    iVar3 = g_intA23C;
    iVar2 = g_intA238;
    local_res18[0] = (int)g_tipAlign;
    WVar1 = *text;
    local_res20[0] = 0;
    local_res10 = (WCHAR *)text;
    PECMD_ParseIntThenSkip((int64_t *)&local_res10, &g_intA238);
    PECMD_ParseIntThenSkip((int64_t *)&local_res10, &g_intA23C);
    PECMD_ParseIntThenSkip((int64_t *)&local_res10, &g_intA240);
    PECMD_ParseIntThenSkip((int64_t *)&local_res10, &g_intA244);
    PECMD_ParseIntThenSkip((int64_t *)&local_res10, local_res20);
    local_res18[0] = (int)g_tipAlign;
    PECMD_ParseIntThenSkip((int64_t *)&local_res10, local_res18);
    g_tipAlign = (uint8_t)local_res18[0];
    PECMD_ParseNumberWs((int64_t *)&local_res10, &DAT_140147010);
    pWVar7 = local_res10;
    FUN_1400F429C(&local_res10, L',');
    if (*local_res10 != L'\0') {
        *local_res10 = L'\0';
        local_res10 = local_res10 + 1;
    }
    FUN_1400703E4(&g_pwszD258, local_res10);
    pWVar9 = WSTR("宋体");
    if (*pWVar7 != L'\0') {
        pWVar9 = pWVar7;
    }
    lpRect = NULL;
    if ((g_flagCF86 != 0) &&
        ((iVar2 != g_intA238 || iVar3 != g_intA23C ||
          iVar4 != g_intA240 || iVar5 != g_intA244))) {
        SetWindowPos(g_hPelogonWnd, (HWND)0, g_intA238, g_intA23C,
                     g_intA240, g_intA244, 0x14);
    }
    if (((DAT_140147010 != g_dbl20b28) && (dVar6 != DAT_140147010)) ||
        (g_flagA249 == 0)) {
        if (g_flagA249 == 0) {
            g_flagA249 = 2;
        }
        DeleteObject(g_hFontFast);
        g_hFontFast = (HFONT)0;
        if (DAT_140147010 == g_dbl20b28) {
            local_b8.d = dVar6;
        } else {
            local_b8.d = DAT_140147010;
        }
        memset(&local_lf, 0, sizeof(local_lf));
        local_lf.lfHeight = -0x80000000;
        if (0 <= g_flagA249) {
            local_lf.lfItalic = 3;
        }
        local_lf.lfWeight = 400;
        lstrcpyW(local_lf.lfFaceName, WSTR("宋体"));
        g_hFontFast = FUN_1400B1F34((int *)&local_lf, (double *)&local_b8, pWVar9);
    }
    if (local_res20[0] != 0) {
        if (WVar1 == L'\0') {
            return 0;
        }
        PECMD_DrawTooltipText(value, (HDC)0, g_hPelogonWnd);
        FUN_1400195F0(value, 0, 90000, NULL);
        g_flagA298 = 1;
        if (g_hTooltipParent != 0) {
            g_flagA298 = 1;
            return 0;
        }
        FUN_1400195F0(value, (int64_t)(int)((uint32_t)g_u8A24A * 10), 0, NULL);
        return 0;
    }
    local_b8.r.top = g_intA23C;
    local_b8.r.left = g_intA238;
    local_b8.r.right = g_intA240;
    if (g_intA240 < 1) {
        local_b8.r.right = ((g_intA240 * 2 >> 1) - g_intA238) + g_tipOffX;
    }
    local_b8.r.bottom = g_intA244;
    if (g_intA244 < 1) {
        local_b8.r.bottom = ((g_intA244 * 2 >> 1) - g_intA23C) + g_tipOffY;
    }
    if (g_flagCF86 == 0) {
        if (g_flagA298 == 0) {
            goto label_02a82c;
        }
    } else {
        local_b8.r.right = local_b8.r.right - g_intA238;
        local_b8.r.bottom = local_b8.r.bottom - g_intA23C;
        local_b8.r.left = 0;
        local_b8.r.top = 0;
    }
    lpRect = &local_b8.r;
label_02a82c:
    BVar8 = InvalidateRect(g_hPelogonWnd, lpRect, 1);
    if (BVar8 == 0) {
        GetLastError();
    }
    g_flagA298 = 0;
    FUN_1400195F0(value, 0, 90000, NULL);
    if (g_hTooltipParent == 0) {
        FUN_1400195F0(value, (int64_t)(int)((uint32_t)g_u8A24A * 10), 0, NULL);
    }
    if (WVar1 != L'\0') {
        g_flagA298 = 1;
    }
    return 0;
}

/* ========== FUN_14002C048 @0x14002c048 ==========
 * 将 INF/SYS 文件解压/复制到系统目录并登记到安装列表。
 * TODO(verify): 列表槽格式与 0x14e/0x29 偏移按反编译保留。
 */
char FUN_14002C048(int64_t *ctx, int mode, void *fileInfo,
                             uint64_t flags)
{
    WCHAR *lpDst;
    WCHAR WVar1;
    bool bVar2;
    int iVar3;
    int iVar4;
    WCHAR *pWVar5;
    int64_t lVar6;
    WCHAR *pWVar7;
    int iVar8;
    WCHAR *pwVar9;
    WCHAR *pWVar10;
    WCHAR *pWVar11;
    WCHAR *pWVar12;
    char cVar13;
    WCHAR *local_res20 = NULL;
    WCHAR local_c8[20];
    WCHAR local_a0[24];
    WCHAR local_70[36];

    if (mode == 0x11) {
        pWVar7 = (WCHAR *)ctx[0x1f];
        pWVar12 = *(WCHAR **)fileInfo;
        lpDst = (WCHAR *)((uint8_t *)fileInfo + 0x16);
        cVar13 = '\0';
        if (pWVar7 == NULL) {
            pWVar5 = StrRChrW(pWVar12, NULL, L'\\');
            if (pWVar5 != NULL) {
                pWVar12 = pWVar5 + 1;
            }
            lVar6 = PECMD_WideStrLen(pWVar12);
            iVar3 = (int)lVar6;
            if (iVar3 < 4) {
                iVar3 = 4;
            }
            pWVar7 = pWVar12 + iVar3 - 4;
        }
        memcpy(local_c8, WSTR("%SystemRoot%\\INF\\"), 0x24);
        memcpy(local_70, WSTR("%SystemRoot%\\SYSTEM32\\DRIVERS\\"), 0x3e);
        memcpy(local_a0, WSTR("%SystemRoot%\\SYSTEM32\\"), 0x2e);
        if (ctx[0x1f] == 0) {
            iVar3 = lstrcmpiW(pWVar7, WSTR(".INF"));
            cVar13 = (char)(iVar3 == 0);
            if (cVar13 != 0) {
                if (*(int16_t *)((uint8_t *)ctx + 0x14e) < 1) {
                    pWVar7 = local_c8;
                } else {
                    cVar13 = 2;
                    pWVar7 = (WCHAR *)ctx[0x20];
                }
            } else {
                if ((int)ctx[0x29] == 0) {
                    return 2;
                }
                iVar3 = lstrcmpiW(pWVar7, WSTR(".SYS"));
                pWVar7 = local_70;
                if (iVar3 != 0) {
                    pWVar7 = local_a0;
                }
            }
        }
        ExpandEnvironmentStringsW(pWVar7, lpDst, 0x104);
        iVar3 = lstrlenW(lpDst);
        lstrcatW(lpDst, pWVar12);
        if (cVar13 != 2) {
            for (pwVar9 = lpDst + iVar3; *pwVar9 != L'\0'; pwVar9 = pwVar9 + 1) {
                if (*pwVar9 == L'#') {
                    *pwVar9 = L'\\';
                }
            }
        }
        PECMD_CreateDirectoryTree(lpDst);
        if (cVar13 != 0) {
            DeleteFileW(lpDst);
        }
        bVar2 = FUN_140101E70(lpDst);
        return (char)((bVar2 != 0) + 1);
    }

    if (mode != 0x13) {
        return 0;
    }
    if ((char)ctx[0x2a] != 0) {
        return 0;
    }
    pWVar12 = *(WCHAR **)fileInfo;
    local_res20 = NULL;
    lVar6 = PECMD_WideStrLen(pWVar12);
    if (((int)lVar6 < 5) ||
        (iVar3 = lstrcmpiW(pWVar12 + (int)lVar6 - 4, WSTR(".INF")), iVar3 != 0)) {
        ctx[0x29] = ctx[0x29] + 1;
    } else {
        if ((((void *)(uintptr_t)ctx[0x23] != NULL &&
              (bVar2 = PECMD_MatchDeviceClass((int64_t)ctx, (WCHAR *)ctx[0x23],
                                     (uint64_t)(uintptr_t)pWVar12), !bVar2)) ||
             (pWVar7 = (WCHAR *)FUN_14002B2EC(ctx, (uint64_t)(uintptr_t)pWVar12, NULL),
              pWVar7 == NULL))) {
            if (((int)ctx[0x29] != 1) && (ctx[0x1f] == 0)) {
                *(uint32_t *)(ctx + 0x29) = 0;
                DeleteFileW(pWVar12);
            }
            goto label_02c489;
        }
        if (0 < *(int16_t *)((uint8_t *)ctx + 0x14e)) {
            PECMD_AllocString(&local_res20, 0x514);
            memcpy(local_c8, WSTR("%SystemRoot%\\INF\\"), 0x24);
            ExpandEnvironmentStringsW(local_c8, local_res20, 0x104);
            iVar3 = lstrlenW(local_res20);
            lstrcatW(local_res20 + iVar3,
                     pWVar12 + *(int16_t *)((uint8_t *)ctx + 0x14e));
            pWVar10 = local_res20 + iVar3;
            WVar1 = *pWVar10;
            while (WVar1 != L'\0') {
                if (*pWVar10 == L'#') {
                    *pWVar10 = L'\\';
                }
                pWVar10 = pWVar10 + 1;
                WVar1 = *pWVar10;
            }
            PECMD_CreateDirectoryTree(local_res20);
            CopyFileW(pWVar12, local_res20, 0);
            DeleteFileW(pWVar12);
            pWVar12 = local_res20;
        }
        pWVar10 = (WCHAR *)ctx[0x26];
        pWVar11 = (WCHAR *)ctx[0x25];
        ctx[0x29] = 1;
        WVar1 = pWVar7[-2];
        iVar3 = lstrlenW(pWVar12);
        iVar8 = iVar3 + 8 + (uint16_t)WVar1;
        if (pWVar10 < (WCHAR *)ctx[0x25]) {
            do {
                if (pWVar7 < *(WCHAR **)pWVar10) {
                    memmove((uint8_t *)(pWVar10 + (int)(iVar3 + 8 + (uint16_t)WVar1)),
                                  (void *)pWVar10,
                                  (int)ctx[0x25] - (int)(intptr_t)pWVar10);
                    pWVar11 = pWVar10;
                    break;
                }
                iVar8 = lstrlenW(pWVar10 + 4);
                iVar4 = lstrlenW(pWVar10 + iVar8 + 5);
                pWVar10 = pWVar10 + iVar8 + 5 + iVar4 + 1;
            } while (pWVar10 < (WCHAR *)ctx[0x25]);
            iVar8 = iVar3 + 8 + (uint16_t)WVar1;
        }
        *(WCHAR **)pWVar11 = pWVar7;
        pWVar11[4] = WVar1;
        pWVar10 = pWVar11 + 5;
        do {
            pWVar11 = pWVar10;
            *pWVar11 = *pWVar7;
            WVar1 = *pWVar7;
            pWVar7 = pWVar7 + 1;
            pWVar10 = pWVar11 + 1;
        } while (WVar1 != L'\0');
        pWVar11[1] = (WCHAR)iVar3;
        pWVar11 = pWVar11 + 2;
        pWVar7 = pWVar12;
        do {
            *pWVar11 = *pWVar7;
            WVar1 = *pWVar7;
            pWVar11 = pWVar11 + 1;
            pWVar7 = pWVar7 + 1;
        } while (WVar1 != L'\0');
        ctx[0x25] = ctx[0x25] + (int64_t)iVar8 * 2;
    }
    if ((ctx[0x1f] == 0) && (g_u8CCB1 != 0)) {
        FUN_140018d8c((uint64_t)(uintptr_t)g_Script, WSTR("已解压出:[%s]\r\n"),
                      (uint64_t)(uintptr_t)pWVar12, flags);
    }
label_02c489:
    FUN_14005B104(&local_res20);
    return 0;
}

/* ========== FUN_14002E3D4 @0x14002e3d4 ==========
 * PELOGON 初始化扩展：解析 -dummy/-dummyx、FirstUsb、钩子选项并安装 shell 组件。
 * TODO(verify): 反编译 in_stack 残留按 0 处理；FUN_14001708c 的 0x140120784
 *               是 .rdata 字符串地址/格式串参数，保持原样。
 */
uint64_t FUN_14002E3D4(int64_t *ctx, WCHAR *cmd)
{
    uint16_t uVar1;
    uint16_t uVar8;
    uint8_t bVar10;
    uint8_t bVar4 = 0;
    uint8_t bVar3 = 0;
    uint8_t bVar5 = 0;
    char cVar6;
    char cVar7;
    uint32_t uVar13 = 0;
    WCHAR *local_res10;
    int16_t local_res20[2];
    WCHAR *local_188 = NULL;
    WCHAR local_178[160];
    bool bVar2 = false;
    uint64_t uVar9;
    uint32_t local_res18[2] = {0, 0};

    (void)uVar13;
    local_178[0] = L'\0';
    local_res10 = cmd;
    FUN_14002286c();
    PECMD_SkipLeadingControls(&local_res10);
    cVar6 = FUN_1400660AC("-dummy", (int64_t *)&local_res10, 6);
    cVar7 = FUN_1400660AC("-dummyx", (int64_t *)&local_res10, 7);
    bVar10 = (uint8_t)(cVar6 != '\0');
    if (cVar7 != '\0') {
        bVar10 = 3;
    }
    FUN_140063620(&local_188);
    FUN_1400675b8((int64_t *)&local_res10, (int64_t *)&local_188, 0x2c);
    if (*local_res10 == 0x2c) {
        local_res10 = local_res10 + 1;
        FUN_1400679b0(&local_res10, &g_tooltipTimeout, 0x2c);
    }
    if (g_tooltipTimeout < 1000) {
        g_tooltipTimeout = 5000;
    }
    if (*local_res10 == 0x2c) {
        local_res10 = local_res10 + 1;
        PECMD_SkipLeadingControls(&local_res10);
        if ((uint16_t)((*local_res10 & 0xffdf) - 0x41) < 0x1a) {
            g_flagA24E = (uint8_t)(*local_res10 & 0xffdf);
        }
    }
    local_res20[0] = (int16_t)g_flagA24E;
    local_res20[1] = 0;
    ((void (*)(HKEY, LPCWSTR, LPCWSTR, DWORD, void *, DWORD))g_pSHSetValueW)(
        HKEY_LOCAL_MACHINE, WSTR("SOFTWARE\\PELOGON"), WSTR("FirstUsb"), 1,
        local_res20, (DWORD)(((uint64_t)uVar13 << 32) | 2));
    for (; (uVar1 = *local_188, uVar1 != 0 && (uVar8 = (uint16_t)(uVar1 | 0x20), uVar1 != 0x2c));
         local_188 = local_188 + 1) {
        if (uVar8 == 0x63) {
            bVar2 = true;
        } else if (uVar8 == 0x69) {
            bVar4 = 1;
        } else if (uVar8 == 0x6b) {
            bVar3 = 1;
        } else if (uVar8 == 0x75) {
            bVar5 = 1;
        }
    }
    local_res10 = local_188;
    if (bVar10 < 2) {
        if (bVar2) {
            PECMD_EnumCDRomDrives(ctx);
        }
        LoadEnvi(g_szEmpty, g_szEmpty);
    }
    FUN_14002cc30(ctx, local_178, 0, 0x24, NULL);
    if (bVar10 == 0) {
        FUN_14001708c(local_178, 0x140120784,
                      WSTR(" -timeout:#9000 -incmd -nfb =PECMD CALL $SHELL32.DLL,DllInstall,#1,I"),
                      (void *)(uintptr_t)0x24);
        local_res18[0] = 0;
        FUN_14000e26c((void *)ctx, (void *)local_178, (void *)ctx,
                      (void *)local_res18, 0, NULL, NULL, NULL);
        FUN_14001708c(local_178, 0x140120784,
                      WSTR(" -timeout:#9000 -incmd -nfb =PECMD CALL $BROWSEUI.DLL,DllInstall,#1,I"),
                      (void *)(uintptr_t)0x24);
        local_res18[0] = 0;
        FUN_14000e26c((void *)ctx, (void *)local_178, (void *)ctx,
                      (void *)local_res18, 0, NULL, NULL, NULL);
        FUN_14001708c(local_178, 0x140120784,
                      WSTR(" -timeout:#9000 -incmd -nfb =PECMD CALL $SHDOCVW.DLL,DllInstall,#1,I"),
                      (void *)(uintptr_t)0x24);
        local_res18[0] = 0;
        FUN_14000e26c((void *)ctx, (void *)local_178, (void *)ctx,
                      (void *)local_res18, 0, NULL, NULL, NULL);
    }
    if (bVar10 < 2) {
        PECMD_WritePELogonDword(0, WSTR("HOOKKBD"), (uint32_t)bVar3);
        PECMD_WritePELogonDword(0, WSTR("TOPICON"), (uint32_t)bVar4);
        PECMD_WritePELogonDword(0, WSTR("AUTOUSB"), (uint32_t)bVar5);
    }
    PECMD_RunFbwfHookScript();
    uVar9 = PECMD_GetPackedSystemVersion();
    if ((uint32_t)(uVar9 >> 0x10) < 0x60000) {
        PECMD_StartOnlyApp(WSTR("-init"));
    }
    FUN_140077358();
    FUN_14005B104(&local_188);
    return 0;
}

/* ========== FUN_14002EE44 @0x14002ee44 ==========
 * PELOGON 主窗口过程：处理任务栏重启、热键注册、定时器与托盘消息。
 */
int64_t FUN_14002EE44(LARGE_INTEGER script, HWND hwnd, uint32_t msg,
                                 void *wParam, int64_t lParam)
{
    int iVar1;
    LRESULT LVar2;
    LARGE_INTEGER LVar3;
    DWORD DVar4;
    uintptr_t nIDEvent;
    UINT uElapse;
    void *ptVar5 = wParam;
    uint32_t local_res18[2];
    uint32_t local_38;
    int local_34[3];
    LARGE_INTEGER local_28;
    WCHAR *local_20 = NULL;

    if ((g_WM_TaskbarRestart != 0) && (g_topiconFlag != 0) &&
        (g_WM_TaskbarRestart == msg)) {
        ptVar5 = NULL;
        FUN_14005B900(&g_hwndTray, (uint64_t)(uintptr_t)hwnd, WSTR("PECMD2012"),
                      (HICON)0, 0x456);
    }
    iVar1 = (int)(intptr_t)wParam;
    if (0x219 < msg) {
        if (msg == 0x312) {
            local_28.QuadPart = 0;
            DVar4 = 0x80070057;
            PECMD_AllocWStringBuffer(&local_20, 0x303a);
            LVar3.QuadPart = (int64_t)(uintptr_t)(local_20 + 0x32);
            local_38 = 1;
            local_res18[0] = 0x3000;
            local_34[0] = 0;
            local_34[1] = 100000;
            local_28.QuadPart = LVar3.QuadPart;
            FUN_14001708c(local_20, 0x140120458,
                          (LPCWSTR)(uintptr_t)((uint64_t)(uintptr_t)wParam & 0xffffffff),
                          ptVar5);
            *(WCHAR *)(uintptr_t)LVar3.QuadPart = L'\0';
            iVar1 = ((LONG (*)(HKEY, LPCWSTR, LPCWSTR, uint32_t *, void *, uint32_t *))g_pSHGetValueW)(
                HKEY_LOCAL_MACHINE, WSTR("SOFTWARE\\PELOGON\\RAMDATA\\HOTKEY"), local_20,
                &local_38, (void *)(uintptr_t)LVar3.QuadPart, local_res18);
            if ((iVar1 == 0) && (*(WCHAR *)(uintptr_t)local_28.QuadPart != L'\0')) {
                if ((uint16_t)(*(WCHAR *)(uintptr_t)local_28.QuadPart - 0x30U) < 10) {
                    FUN_1400679DC((uint64_t *)&local_28, local_34, 0x2c);
                    FUN_1400679DC((uint64_t *)&local_28, local_34 + 1, 0x2c);
                }
                if (*(WCHAR *)(uintptr_t)local_28.QuadPart != L'\0') {
                    LVar3.QuadPart = (int64_t)FUN_14004c0bc((uint64_t)script.QuadPart,
                                                            (uint64_t)local_28.QuadPart,
                                                            NULL, NULL, NULL);
                    DVar4 = LVar3.LowPart;
                }
            }
            FUN_14005B104(&local_20);
            return (int64_t)(int32_t)DVar4;
        }
        if (msg == 0x446) {
            FUN_140022A70((uint64_t)script.QuadPart, 0);
        } else if (msg != 0x44a) {
            if (msg == 1099) {
                if (g_hwndTray != 0) {
                    return 0;
                }
                FUN_14005B900(&g_hwndTray, (uint64_t)(uintptr_t)hwnd, WSTR("PECMD2012"),
                              (HICON)0, 0x456);
                return 0;
            }
            if (msg == 0x456) {
                if (g_hwndTray == 0) {
                    return 0;
                }
                if ((lParam != 0x203) && (lParam != 0x202) &&
                    (lParam != 0x201) && (lParam != 0x206) &&
                    (lParam != 0x205) && (lParam != 0x204)) {
                    return 0;
                }
                FUN_14002E790(hwnd);
                return 0;
            }
            goto label_02effd;
        }
        FUN_140022e94();
        return 0;
    }
    if (msg == 0x219) {
        if (iVar1 == 0x8000) {
            FUN_140077358();
            return 0;
        }
        if (g_flagA24B < 1) {
            return 0;
        }
        nIDEvent = 10;
        KillTimer(hwnd, 10);
        uElapse = (UINT)g_tooltipTimeout;
    } else {
        if (msg == 1) {
            if (g_topiconFlag == 0) {
                return 0;
            }
            SendMessageW(g_hwndCF78, 1099, 0, 0);
            return 0;
        }
        if (msg == 2) {
            return 0;
        }
        if (msg == 0x10) {
            return 0;
        }
        if (msg != 0x1a) {
            if (msg == 0x111) {
                return 0;
            }
            if (msg == 0x113) {
                if (iVar1 == 10) {
                    KillTimer(hwnd, 10);
                    if (g_flagA24B < 1) {
                        return 0;
                    }
                    FUN_140077358();
                }
                if (iVar1 != 0x1e) {
                    return 0;
                }
                KillTimer(hwnd, 0x1e);
                PECMD_ApplyDesktopWallpaper();
                KillTimer(hwnd, 0x1e);
                return 0;
            }
label_02effd:
            LVar2 = DefWindowProcW(hwnd, msg, (WPARAM)wParam, lParam);
            return LVar2;
        }
        nIDEvent = 0x1e;
        KillTimer(hwnd, 0x1e);
        uElapse = 5;
    }
    SetTimer(hwnd, nIDEvent, uElapse, NULL);
    return 0;
}

/* ========== FUN_1400389C4 @0x1400389c4 ==========
 * 解析 LOGO 命令参数，设置图标/LogoFile 并创建或通知 LOGO 窗口。
 */
uint64_t FUN_1400389C4(uint64_t ctx, LPCWSTR text)
{
    HWND pHVar1;
    HICON pHVar2;
    bool bVar3;
    uint32_t uVar4;
    uint32_t uVar6;
    int iVar5;
    HWND hWnd;
    uint8_t bVar7;
    WCHAR *local_res10;
    uint8_t local_res18[8];
    char local_res20[8];
    char local_268 = '*';
    char local_267[7];
    WCHAR *local_260 = NULL;
    uint32_t local_258[2];
    WCHAR *local_250;
    WCHAR *local_248 = NULL;
    WCHAR local_238[264];

    (void)ctx;
    local_res20[0] = '\0';
    local_res18[0] = 1;
    local_267[0] = 1;
    local_res10 = (WCHAR *)text;
    FUN_140025CE0((int64_t *)&local_res10, local_res20, local_res18, local_267,
                  &local_268, NULL);
    g_u8CF85 = local_res18[0] & 0xf0;
    bVar7 = local_res18[0] & 1;
    local_res18[0] = bVar7;
    FUN_1400702B0(&local_248, local_res10);
    local_238[0] = L'\0';
    local_250 = NULL;
    local_res10 = local_248;
    EnterCriticalSection(&g_csInit);
    g_bitFlags = g_bitFlags ^ (((local_res20[0] != '\0') * 2 ^ g_bitFlags) & 2);
    local_250 = StrChrW(local_res10, L',');
    if (local_250 == NULL) {
        local_250 = NULL;
    } else {
        *local_250 = L'\0';
        local_250 = local_250 + 1;
        local_258[0] = 0;
        bVar3 = PECMD_ParseNumber((int64_t *)&local_250, (int *)local_258);
        if (bVar3 > 0) {
            g_dwTipBkColor = local_258[0];
        }
    }
    if (g_dwTipBkColor == 0x80000000) {
        g_dwTipBkColor = PECMD_GetWinlogonBackground();
    }
    uVar4 = (uint32_t)(uint8_t)local_268;
    uVar6 = (uint32_t)(uint16_t)*local_res10;
    if ((uVar4 == uVar6) ||
        (bVar3 = FUN_140101E70(local_res10), bVar3 == 0)) {
        *local_res10 = L'\0';
        bVar3 = (bVar7 == 0);
    } else {
        bVar3 = (bVar7 == 0);
        if (!bVar3) {
            bVar7 = 2;
            local_res18[0] = 2;
            goto label_038b2f;
        }
    }
label_038b2f:
    g_u8CF84 = (uint8_t)!bVar3;
    g_bitFlags = g_bitFlags ^ ((local_267[0] << 2 ^ g_bitFlags) & 0xc);
    if (*local_res10 != L'\0') {
        StrCpyNW(local_238, local_res10, 0x104);
        pHVar2 = g_hIconCF88;
        g_logoActiveFlag = 1;
        g_hIconCF88 = PECMD_LoadIcon(local_238, NULL);
        if ((pHVar2 != (HICON)0) && (g_pGdipDisposeImage != NULL)) {
            ((void (*)(HICON))g_pGdipDisposeImage)(pHVar2);
        }
    }
    FUN_140063620(&local_260);
    FUN_1400e3cd4(local_238, &local_260, NULL);
    iVar5 = lstrlenW(local_260);
    if (uVar6 != uVar4) {
        FUN_14005c5a0(HKEY_LOCAL_MACHINE, WSTR("SOFTWARE\\PELOGON"), WSTR("LogoFile"), 1,
                      (BYTE *)local_260, iVar5 * 2 + 2);
    }
    pHVar1 = g_hPelogonWnd;
    hWnd = PECMD_GetPELogonWindow();
    if (hWnd != (HWND)0) {
        if ((g_hPelogonWnd != (HWND)0) && (hWnd != g_hPelogonWnd)) {
            SendMessageW(hWnd, 0x111, 0x66, 0);
            hWnd = (HWND)0;
        }
        if ((hWnd != (HWND)0) && (pHVar1 == (HWND)0)) {
            goto label_038c65;
        }
    }
    hWnd = pHVar1;
    if (uVar6 != uVar4) {
        FUN_1400703E4(&g_pwszD220, local_260);
    }
label_038c65:
    if (hWnd == (HWND)0) {
        ((LPWSTR)text)[0] = L'\0';
        FUN_14005B104(&local_260);
        LeaveCriticalSection(&g_csInit);
        DialogBoxIndirectParamW(g_hInstance, (const void *)g_bE6b8, (HWND)0,
                                (void *)PECMD_ScriptWndProc, 0);
        EnterCriticalSection(&g_csInit);
        g_hPelogonWnd = (HWND)0;
        LeaveCriticalSection(&g_csInit);
    } else {
        if ((g_hPelogonWnd == (HWND)0) && (g_hIconCF88 != (HICON)0)) {
            ((void (*)(void))g_pGdipDisposeImage)();
            g_hIconCF88 = (HICON)0;
        }
        LeaveCriticalSection(&g_csInit);
        SendMessageW(hWnd, 0x450, 0, 0);
        ((LPWSTR)text)[0] = L'\0';
        g_u8CF83 = 0;
        FUN_14005B104(&local_260);
    }
    FUN_14005B104(&local_248);
    return 1;
}

/* ========== FUN_14003AAD0 @0x14003aad0 ==========
 * 处理一个安装项：复制工作串、建立互斥锁名、执行 7z 解压/设备更新，
 * 最后递减任务计数。TODO(verify): ctx[4] 在反编译中同时作长度/状态，
 * 此处按“初始长度”与“-1 状态”解释。
 */
uint64_t FUN_14003AAD0(uint64_t *task)
{
    int *piVar1;
    int64_t *plVar5;
    uint16_t uVar3;
    WCHAR WVar4;
    WCHAR *pwVar16;
    WCHAR *pWVar6;
    WCHAR *pWVar7;
    uint32_t uVar8;
    bool bVar9;
    int iVar10 = 0;
    int iVar11;
    uint32_t uVar12;
    WCHAR *pWVar13;
    WCHAR *pWVar14;
    WCHAR *pWVar15;
    int64_t lVar17 = -1;
    WCHAR *local_res10 = NULL;
    uint64_t local_res18 = 0;
    WCHAR *local_res20 = NULL;
    WCHAR *local_88 = NULL;
    HANDLE local_80 = (HANDLE)0;
    uint64_t local_78;
    WCHAR *local_60;
    uint64_t local_70 = 0;
    uint64_t *local_58[3] = {0, 0, 0};
    int local_68;

    plVar5 = (int64_t *)*task;
    local_78 = task[4];
    pWVar7 = (WCHAR *)task[5];
    uVar3 = *(uint16_t *)((uint8_t *)plVar5 + 0x14c);
    task[4] = (uint64_t)-1;
    uVar8 = (uint32_t)uVar3;
    piVar1 = (int *)((uint8_t *)plVar5 + 0x1a4);
    local_58[0] = task;
    if ((HANDLE)task[3] != (HANDLE)0) {
        SetEvent((HANDLE)task[3]);
    }
    pwVar16 = (WCHAR *)plVar5[0x33];
    local_60 = pWVar7 + local_78 + 0x27;
    pWVar6 = pWVar7 + local_78;
    local_68 = (int)((uint16_t)pWVar6[1] + 1) * 2;
    memcpy((uint8_t *)pWVar6, (uint8_t *)local_60, local_68);
    if ((((int)plVar5[0x34] < 0) ||
         (((pwVar16 != NULL &&
            (bVar9 = PECMD_MatchDeviceClass((int64_t)plVar5, pwVar16, (uint64_t)(uintptr_t)pWVar7),
             !bVar9)) ||
           (pWVar13 = (WCHAR *)FUN_14002B2EC(plVar5, (uint64_t)(uintptr_t)pWVar7 + 1, NULL),
            (int)plVar5[0x34] < 0)) ||
          ((pWVar13 == NULL || (WVar4 = *pWVar13, WVar4 == L'\0'))))) ||
        (pWVar13[1] == L'\0')) {
        goto label_03afca;
    }
    iVar10 = lstrlenW(pWVar13 + 1);
    PECMD_AllocWStringBuffer(&local_res10, (int64_t)iVar10 + 0x105);
    pWVar15 = pWVar13 + 1;
    pWVar14 = (WCHAR *)(uintptr_t)(uint16_t)WVar4;
    wsprintfW(local_res10, WSTR("Local\\pecmd2012.lock.%c%s"), (WCHAR)WVar4, pWVar15);
    local_res10[0x104] = L'\0';
    PECMD_CreateMutexToSlot(&local_res18, local_res10);
    if (-1 < (int)plVar5[0x34]) {
        if ((*pWVar13 != L'\0') && (*(char *)((uint8_t *)plVar5 + 0x157) == '\0')) {
            iVar10 = 0;
            if ((char)plVar5[0x32] == '@') {
                iVar11 = lstrlenW((WCHAR *)plVar5[0x1f]);
                memcpy((uint8_t *)pWVar6, (uint8_t *)WSTR("!!!PECMD_SUB_OK"), 0x20);
                PECMD_AllocWStringBuffer(&local_res20, (int64_t)local_78 + 0x113);
                local_78 = (uint64_t)(uintptr_t)(pWVar7 + iVar11);
                wsprintfW(local_res20, WSTR("Local\\pecmd2012.lock.%s"), pWVar7);
                local_res20[0x104] = L'\0';
                PECMD_CreateMutexToSlot(&local_70, local_res20);
                bVar9 = FUN_140101E70(pWVar7);
                if (bVar9 == 0) {
                    WVar4 = pWVar6[-1];
                    pWVar6[-1] = L'\0';
                    if ((int)plVar5[0x34] < 0) {
                        PECMD_ReleaseMutex(&local_70);
                        FUN_14005B104(&local_res20);
                        goto label_03afb0;
                    }
                    PECMD_AllocWStringBuffer(&local_88, 0x1000);
                    wsprintfW(local_88, WSTR("x  %s %s ..."));
                    FUN_140025f10((int64_t)(plVar5 + 1), local_88, 0,
                                  (void *)(uintptr_t)0x11, NULL, NULL);
                    FUN_14005B104(&local_88);
                    PECMD_RunProcessCommand((int64_t)plVar5, WSTR("7z x -y -aos "),
                                  (uint64_t)(plVar5[0x2e] + 1),
                                  (uint64_t)(plVar5[0x1f] + 1), local_78);
                    pWVar6[-1] = WVar4;
                    pWVar15 = (WCHAR *)(uintptr_t)7;
                    local_80 = (HANDLE)0;
                    PECMD_OpenFileHandle(&local_80, pWVar7, 0xc0000000, 7, NULL, 2, 0x80,
                                   (HANDLE)0);
                    if ((local_80 != (HANDLE)0) && (local_80 != (HANDLE)-1)) {
                        CloseHandle(local_80);
                    }
                    local_80 = (HANDLE)0;
                }
                PECMD_ReleaseMutex(&local_70);
                FUN_14005B104(&local_res20);
                pWVar14 = (WCHAR *)(uintptr_t)local_68;
                memcpy((uint8_t *)pWVar6, (uint8_t *)local_60, local_68);
            }
            if ((char)plVar5[0x2b] == '\0') {
                if (((*(char *)((uint8_t *)plVar5 + 0x154) == 3) ||
                     (iVar11 = 0, *(char *)((uint8_t *)plVar5 + 0x156) != 0)) &&
                    (iVar11 = 0, *(char *)((uint8_t *)plVar5 + 0x191) == 0)) {
                    goto label_03aee2;
                }
                while (true) {
                    if ((int)plVar5[0x34] < 0) {
                        goto label_03afb0;
                    }
                    pWVar15 = (WCHAR *)(uintptr_t)uVar8;
                    pWVar14 = pWVar7;
                    uVar12 = FUN_14002B7F8((int64_t)plVar5, pWVar13, pWVar7, uVar8, -1, NULL);
                    iVar10 = iVar11 + 1;
                    lVar17 = (int64_t)(int32_t)uVar12;
                    if ((1 < iVar10) || (lVar17 == 0)) {
                        break;
                    }
label_03aee2:
                    if ((int)plVar5[0x34] < 0) {
                        goto label_03afb0;
                    }
                    if (*(char *)((uint8_t *)plVar5 + 0x156) == '\0') {
                        if (*(char *)((uint8_t *)plVar5 + 0x154) == 3) {
                            pWVar15 = (WCHAR *)(uintptr_t)-1;
                            pWVar14 = pWVar13;
                            uVar12 = FUN_14002C634((int64_t)plVar5, pWVar7, pWVar13, -1);
                            lVar17 = (int64_t)(uint32_t)uVar12;
                        }
                    } else {
                        pWVar15 = (WCHAR *)(uintptr_t)-1;
                        pWVar14 = pWVar13;
                        uVar12 = PECMD_DevconUpdate((int64_t)plVar5, pWVar7, pWVar13, -1);
                        lVar17 = (int64_t)(uint32_t)uVar12;
                    }
                    if ((1 < iVar10 + 1) || (iVar11 = iVar10 + 1, lVar17 == 0)) {
                        break;
                    }
                }
            } else {
                if ((int)plVar5[0x34] < 0) {
                    goto label_03afb0;
                }
                pWVar14 = (WCHAR *)(uintptr_t)((uint64_t)uVar3 << 0x10);
                FUN_14002B9EC((int64_t)plVar5, pWVar7, uVar8 << 0x10);
            }
        }
        EnterCriticalSection(&g_csInit);
        if (lVar17 == 0) {
            if (pWVar13 != NULL) {
                *pWVar13 = L'\0';
            }
            plVar5[0x35] = 0;
            if (0 < (int)plVar5[0x34]) {
                int *pCount = (int *)&plVar5[0x34];
                *pCount = *pCount - 1;
                if (*pCount == 0) {
                    *(int32_t *)&plVar5[0x34] = -1;
                    if ((*(uint8_t *)&plVar5[0x36] & 0x10) != 0) {
                        FUN_140018d8c((uint64_t)(uintptr_t)g_Script, WSTR("END 1111\r\n"),
                                      (uint64_t)(uintptr_t)pWVar14,
                                      (uint64_t)(uintptr_t)pWVar15);
                    }
                    PECMD_TerminateJobObject((int64_t)plVar5);
                }
            }
        }
        LeaveCriticalSection(&g_csInit);
    }
label_03afb0:
    PECMD_ReleaseMutex(&local_res18);
    FUN_14005B104(&local_res10);
label_03afca:
    if (piVar1 != NULL) {
        EnterCriticalSection(&g_csInit);
        *piVar1 = *piVar1 - 1;
        LeaveCriticalSection(&g_csInit);
    }
    FUN_14005B104((WCHAR **)local_58);
    return 0;
}

/* ========== FUN_14003B010 @0x14003b010 ==========
 * 逐行读取安装脚本，为每个 <INFFILE> 项创建任务并启动线程。
 * TODO(verify): 栈上的任务槽数组按 6 槽展开，线程参数布局与反编译一致。
 */
uint64_t FUN_14003B010(uint64_t *tasks)
{
    uint8_t bVar1;
    int iVar2;
    WCHAR *lpString;
    int64_t lVar3;
    WCHAR *pWVar4;
    WCHAR *pWVar5;
    HANDLE pvVar6;
    uint32_t uVar7 = 0;
    DWORD DVar8;
    uint8_t *lpFirst;
    uint8_t *pbVar10;
    int64_t *plVar11;
    uint8_t *pbVar12;
    int64_t *plVar13;
    int16_t sVar14;
    uint16_t uVar15;
    int64_t lVar16;
    uint8_t *local_res8;
    WCHAR *local_res10 = NULL;
    WCHAR *local_res18;
    WCHAR *local_res20 = NULL;
    int64_t local_108 = 0;
    int64_t local_100;
    WCHAR *local_f8;
    HANDLE local_f0[3];
    int64_t *local_d8[18] = {0};
    int64_t local_c8[18] = {0};
    uint64_t uVar9 = 0;

    local_res18 = (WCHAR *)tasks[5];
    plVar11 = (int64_t *)*tasks;
    tasks[5] = 0;
    pbVar12 = (uint8_t *)tasks[1];
    lVar16 = (int64_t)tasks[4];
    tasks[1] = 0;
    tasks[4] = 0;
    local_res20 = NULL;
    local_108 = 0;
    local_res8 = pbVar12;
    local_100 = lVar16;
    local_f8 = local_res18;
    if (((char)plVar11[0x32] == 'A') && ((void *)(uintptr_t)plVar11[0x1f] != NULL)) {
        iVar2 = lstrlenW((WCHAR *)plVar11[0x1f]);
        local_100 = (int64_t)iVar2 - 1;
        PECMD_AllocString(&local_res20, (int64_t)iVar2 + 0x270);
        memcpy((uint8_t *)local_res20, (uint8_t *)plVar11[0x1f],
                      (int)(local_100 + 2) * 2);
        local_res18 = local_res20;
        local_108 = lVar16;
    }
    local_d8[1] = NULL;
    local_c8[0] = 0;
    local_c8[1] = 0;
    local_c8[2] = 1;
    local_c8[3] = 0;
    lVar16 = 3;
    local_d8[0] = plVar11;
    pWVar5 = local_res18;

    do {
        if (pbVar12 == NULL) {
            int idx;
            for (idx = 0; idx < 3; idx++) {
                if ((HANDLE)local_c8[idx * 6] != (HANDLE)0) {
                    WaitForSingleObject((HANDLE)local_c8[idx * 6], INFINITE);
                    if (local_c8[idx * 6 + 2] == 0) {
                        tasks[4] = 0;
                    }
                    local_c8[idx * 6] = 0;
                }
                FUN_14005B104((WCHAR **)&local_d8[idx * 6 + 1]);
                FUN_14005B104((WCHAR **)&local_c8[idx * 6 + 3]);
            }
            FUN_14005B104(&local_res20);
            FUN_14005B104((WCHAR **)&local_f8);
            return 0;
        }
        sVar14 = 0x20;
        uVar15 = 9;
        bVar1 = *pbVar12;
        while ((bVar1 != 0) && (local_res8 = pbVar12,
                                (bVar1 < 9 || (0xd < bVar1)) && (bVar1 != 0x20))) {
            pbVar12 = pbVar12 + 1;
            local_res8 = pbVar12;
            bVar1 = *pbVar12;
        }
        PECMD_SkipWsByte((int64_t *)&local_res8);
        bVar1 = *local_res8;
        while ((bVar1 != 0) &&
               (((uint8_t)(int8_t)bVar1 < uVar15 || (0xd < (uint8_t)(int8_t)bVar1))) &&
               (sVar14 != (char)bVar1)) {
            local_res8 = local_res8 + 1;
            bVar1 = *local_res8;
        }
        PECMD_SkipWsByte((int64_t *)&local_res8);
        lpFirst = local_res8;
        pbVar10 = local_res8;
        if (*local_res8 == 0x22) {
            lpFirst = local_res8 + 1;
            pbVar10 = lpFirst;
        }
        for (; *lpFirst != 0; lpFirst = lpFirst + 1) {
            if (*lpFirst == 0x22) {
                *lpFirst = 0;
                goto label_03b225;
            }
        }
        for (; *lpFirst != 0; lpFirst = lpFirst + 1) {
            if (*lpFirst == 10) {
                *lpFirst = 0;
                lpFirst = lpFirst + 1;
                break;
            }
label_03b225:
            ;
        }
        pbVar12 = (uint8_t *)StrStrA((LPCSTR)lpFirst, "<INFFILE>");
        local_res8 = pbVar12;
        if (pbVar12 != NULL) {
            local_res8 = pbVar12 + 1;
            *pbVar12 = 0;
            if (local_res8 != NULL) {
                *local_res8 = 0;
                local_res8 = pbVar12 + 2;
            }
        }
        pbVar12 = local_res8;
        FUN_1400702F0((int64_t *)&local_res10, (LPCSTR)pbVar10, (uint64_t)-1);
        iVar2 = lstrlenW(local_res10);
        if (0x20c < iVar2) {
            FUN_14005B104(&local_res10);
            continue;
        }
        memcpy((uint8_t *)(pWVar5 + local_100 + 1), (uint8_t *)local_res10,
                      (iVar2 + 1) * 2);
        lpString = (WCHAR *)FUN_14002B2EC(plVar11, (uint64_t)(uintptr_t)pWVar5, (LPCSTR)lpFirst);
        uVar7 = 0;
        if (*(char *)((uint8_t *)plVar11 + 0x157) == '\0') {
            if (lpString != NULL) {
                local_f0[0] = (HANDLE)0;
                local_f0[1] = (HANDLE)0;
                local_f0[2] = (HANDLE)0;
                plVar13 = local_c8;
                uVar9 = 0;
                do {
                    DVar8 = (DWORD)uVar9;
                    pvVar6 = (HANDLE)*plVar13;
                    local_f0[uVar7] = pvVar6;
                    if (pvVar6 == (HANDLE)0) {
                        break;
                    }
                    uVar7 = uVar7 + 1;
                    DVar8 = DVar8 + 1;
                    uVar9 = (uint64_t)DVar8;
                    plVar13 = plVar13 + 6;
                } while ((int64_t)uVar7 < 3);
                if (2 < (int)DVar8) {
                    while (true) {
                        DVar8 = WaitForMultipleObjects(3, local_f0, 0, 0);
                        if ((-1 < (int)DVar8) && ((int)DVar8 < 3)) {
                            break;
                        }
                        FUN_1400195F0((uint64_t)(uintptr_t)(plVar11 + 1), 1, 0, NULL);
                    }
                    local_c8[(int)DVar8 * 6] = 0;
                    if (local_c8[(int)DVar8 * 6 + 2] == 0) {
                        tasks[4] = 0;
                    }
                }
                lVar3 = (int64_t)(int32_t)DVar8;
                FUN_1400703E4((WCHAR **)&local_c8[lVar3 * 6 + 3], local_res18);
                iVar2 = lstrlenW(lpString);
                PECMD_AllocString(&local_d8[lVar3 * 6 + 1], iVar2 + 4);
                memcpy((uint8_t *)local_d8[lVar3 * 6 + 1],
                              (uint8_t *)(lpString - 3), (iVar2 + 4) * 2);
                pWVar5 = NULL;
                local_c8[lVar3 * 6 + 2] = 1;
                if (local_108 != 0) {
                    pWVar4 = StrRChrW(local_res10, NULL, L'\\');
                    if (pWVar4 != NULL) {
                        *pWVar4 = L'\0';
                        pWVar5 = local_res10;
                    }
                    PECMD_RunProcessCommand((int64_t)plVar11, WSTR("7z x -y -aos "),
                                  (uint64_t)plVar11[0x2e], (uint64_t)plVar11[0x1f],
                                  (uint64_t)(uintptr_t)pWVar5);
                    if (pWVar4 != NULL) {
                        *pWVar4 = L'\\';
                    }
                }
                pvVar6 = CreateThread(NULL, 0x10000, (void *)FUN_14003A810,
                                      (LPVOID)(local_d8 + lVar3 * 6), 0x10004,
                                      &g_dwC96C);
                local_c8[lVar3 * 6] = (int64_t)(intptr_t)pvVar6;
                pWVar5 = local_res18;
                if (pvVar6 != (HANDLE)0) {
                    ResumeThread(pvVar6);
                    pWVar5 = local_res18;
                }
                goto label_03b48d;
            }
        } else {
label_03b48d:
            if (lpString != NULL) {
                *lpString = L'\0';
            }
        }
        FUN_14005B104(&local_res10);
    } while (true);
}

/* ========== FUN_14003C9E8 @0x14003c9e8 ==========
 * 解析并发送按键/鼠标输入命令（-gui/-m 前缀）。
 */
uint64_t FUN_14003C9E8(LPCWSTR cmd)
{
    WCHAR wVar1;
    WCHAR WVar2;
    char cVar3;
    char cVar4;
    uint32_t uVar5 = 0;
    uint64_t uVar6;
    uint32_t dwFlags = 0;
    WCHAR *local_res8;
    WCHAR *local_res10 = NULL;
    WCHAR *ptVar7;
    int local_44[5];
    int local_48;
    int local_res20[2];
    int local_res18[2];

    local_res8 = (WCHAR *)cmd;
    PECMD_SkipLeadingControls(&local_res8);
    cVar3 = FUN_1400660AC("-gui", (int64_t *)&local_res8, 4);
    if (cVar3 == '\0') {
        cVar3 = FUN_1400660AC("-m", (int64_t *)&local_res8, 2);
        if (cVar3 == '\0') {
            cVar3 = '*';
            if (*local_res8 == L'-') {
                cVar3 = '-';
            }
            while ((*local_res8 == (WCHAR)(uint8_t)cVar3) &&
                   (local_res8[1] == (WCHAR)(uint8_t)cVar3)) {
                local_res8 = local_res8 + 2;
                cVar4 = FUN_1400660AC("ext", (int64_t *)&local_res8, 3);
                if (cVar4 == '\0') {
                    cVar4 = FUN_1400660AC("s", (int64_t *)&local_res8, 1);
                    if (cVar4 == '\0') {
                        for (; (wVar1 = *local_res8, wVar1 != L'\0' &&
                                (((uint16_t)wVar1 < 9 || (0xd < (uint16_t)wVar1)) &&
                                 (wVar1 != L' ')));
                             local_res8 = local_res8 + 1) {
                        }
                        PECMD_SkipLeadingControls(&local_res8);
                    } else {
                        dwFlags = dwFlags | 4;
                    }
                } else {
                    dwFlags = dwFlags | 1;
                }
            }
            CharUpperW(local_res8);
            WVar2 = *local_res8;
            while (local_res10 = local_res8, WVar2 != L'\0') {
                do {
                    ptVar7 = local_res10;
                    local_res10 = local_res10 + 1;
                    WVar2 = *local_res10;
                    if ((WVar2 == L'\0') || (WVar2 == L',') || (WVar2 == L';') ||
                        ((8 < (uint16_t)WVar2 && ((uint16_t)WVar2 < 0xe)))) {
                        break;
                    }
                } while (WVar2 != L' ');
                if (*local_res10 != L'\0') {
                    *local_res10 = L'\0';
                    local_res10 = ptVar7 + 1;
                }
                uVar5 = PECMD_ParseVkKeyName(local_res8, '\x01');
                if (0 < (int)uVar5) {
                    if (*ptVar7 != L'^') {
                        keybd_event((BYTE)uVar5, '\0', dwFlags, 0);
                    }
                    if (*ptVar7 != L'_') {
                        keybd_event((BYTE)uVar5, '\0', dwFlags | 2, 0);
                    }
                }
                PECMD_SkipLeadingControls(&local_res10);
                local_res8 = local_res10;
                WVar2 = *local_res10;
            }
            uVar6 = (uint64_t)(uVar5 == 0);
        } else {
            local_44[0] = 0;
            local_48 = 0;
            local_res20[0] = 0;
            local_res18[0] = 0;
            local_res10 = NULL;
            PECMD_ParseIntThenSkip((int64_t *)&local_res8, (int *)local_44);
            PECMD_ParseIntThenSkip((int64_t *)&local_res8, (int *)&local_48);
            PECMD_ParseIntThenSkip((int64_t *)&local_res8, (int *)local_res20);
            PECMD_ParseIntThenSkip((int64_t *)&local_res8, (int *)local_res18);
            PECMD_ParseIntThenSkip((int64_t *)&local_res8, (int *)&local_res10);
            mouse_event(local_44[0], local_48, local_res20[0], local_res18[0],
                        (int64_t)(int32_t)(uintptr_t)local_res10);
            uVar6 = 0;
        }
    } else {
        FUN_1400702B0(&local_res10, WSTR("#30:INDATA "));
        FUN_14006375C(&local_res10, local_res8);
        uVar6 = (uint64_t)PECMD_RunCommand((void *)g_Script, local_res10);
        FUN_14005B104(&local_res10);
    }
    return uVar6;
}

/* ========== FUN_14003D608 @0x14003d608 ==========
 * 执行系统关机/重启流程，支持立即/延时/远程关机选项。
 * TODO(verify): wsprintfA/wsprintfW 反编译缺参，按可见格式串保留。
 */
int64_t FUN_14003D608(int mode, uint32_t flags, LPCWSTR remote)
{
    bool bVar1 = true;
    bool bVar2;
    bool bVar9;
    bool bVar10;
    uint8_t bVar3;
    int iVar4;
    uint64_t uVar5;
    uint32_t uVar6;
    uint32_t uVar7;
    int64_t lVar8;
    uint32_t bRebootAfterShutdown;
    LPSTR local_res20;
    WCHAR *local_c8[2] = {NULL, NULL};
    STARTUPINFOW local_b8;

    bVar10 = (flags & 0x72) == 0;
    bVar9 = (flags & 0x70) == 0;
    g_runFlag = g_runFlag | 0x80;
    bVar3 = (uint8_t)((flags >> 5) & 2);
    bRebootAfterShutdown = (uint32_t)(mode != 0);
    if (bRebootAfterShutdown != 0) {
        bVar3 = 1;
    }
    uVar7 = (flags >> 0xe) & 1;
    uVar5 = PECMD_GetPackedSystemVersion();
    iVar4 = PECMD_ReadPelogonFlag(WSTR("ASNT5SHUTDOWN"));
    uVar6 = 1;
    if (iVar4 != 0) {
        uVar6 = ~flags & 1;
    }
    if (((uVar6 == 0) || (bVar9)) || ((uint32_t)(uVar5 >> 0x10) < 0x60002)) {
        bVar1 = false;
    }
    if (uVar7 != 0) {
        bVar2 = false;
        bVar10 = false;
    } else if (bVar10) {
        bVar2 = false;
        bVar10 = false;
    } else {
        bVar2 = true;
        bVar10 = true;
    }
    FUN_1400633A8((void **)&local_res20, 300);
    wsprintfA(local_res20,
              "\nBoot(0x%X, 0x%X, \"%S\") Main=0x%X %d op=[%d %d:0x%X %d %d:%d]",
              (uint64_t)(mode != 0), flags);
    PECMD_AppendDebugLog(local_res20);
    FUN_14005B104((WCHAR **)&local_res20);
    local_b8.lpReserved = NULL;
    local_b8.cb = 0x68;
    memset(&local_b8.lpDesktop, 0, 0x58);
    GetStartupInfoW(&local_b8);
    iVar4 = lstrlenW(remote);
    PECMD_AllocWStringBuffer(&local_c8[0], (int64_t)iVar4 + 0xf0);
    wsprintfW(local_c8[0], WSTR("\"%s\" Down=0x%X Reboot=0x%X sa.wShowWindow=0x%X"),
              remote, flags, bRebootAfterShutdown, local_b8.wShowWindow);
    PECMD_RunShutdownScript(local_c8[0], flags);
    FUN_14001C2CC(WSTR("SeShutdownPrivilege"), 2, 0);
    FUN_14001C2CC(WSTR("SeRemoteShutdownPrivilege"), 2, 0);
    lVar8 = 0;
    if (bVar1) {
        InitiateSystemShutdownW(NULL, g_szEmpty, 0, 1, bRebootAfterShutdown);
        FUN_1400195F0((uint64_t)(uintptr_t)g_Script, 10000, 0, NULL);
        PECMD_AppendDebugLog("---next ");
    }
    if (bVar2) {
        FUN_140027EAC((uint64_t)(uintptr_t)g_Script, NULL, 0x16, 1, 0, 2, 10, 1, 0);
    }
    if (bVar10) {
        FUN_140027EAC((uint64_t)(uintptr_t)g_Script, NULL, 0x10, 0, 0, 2, 10, 1, 0);
        FUN_140027EAC((uint64_t)(uintptr_t)g_Script, NULL, 2, 0, 0, 2, 10, 1, 0);
    }
    if (!bVar9) {
        FUN_14001a56c((int)bVar3);
        lVar8 = 0;
    }
    FUN_14005B104(&local_c8[0]);
    return lVar8;
}

/* ========== FUN_14003E220 @0x14003e220 ==========
 * “确认/密码”对话框过程：初始化居中与文本，处理按钮与 -force 命令。
 */
uint64_t FUN_14003E220(HWND hwnd, int msg, int16_t wParam)
{
    uint64_t *puVar1;
    int iVar2;
    int iVar3;
    HWND pHVar4;
    WCHAR *pWVar5;
    LPCWSTR pWVar6;
    WCHAR *lpString1;
    union {
        RECT r;
        uint64_t p;
    } local_918;
    WCHAR local_908[104];
    WCHAR local_838[1024];

    memset(&local_918, 0, sizeof(local_918));
    puVar1 = (uint64_t *)g_pAppData;
    if (msg == 0x110) {
        iVar2 = GetSystemMetrics(0x3d);
        iVar3 = GetSystemMetrics(0x3e);
        if (g_hFontE2B0 == 0) {
            FUN_1400E648C((uint64_t *)&g_hFontE2B0, 0x3ea);
        }
        puVar1[2] = (uint64_t)(uintptr_t)g_hFontE2B0;
        GetWindowRect(hwnd, &local_918.r);
        SetWindowPos(hwnd, (HWND)-2,
                     ((local_918.r.left - local_918.r.right) + iVar2) / 2,
                     ((local_918.r.top - local_918.r.bottom) + iVar3) / 2,
                     0, 0, 0x41);
        pWVar6 = FUN_1400169BC(0x3f4, NULL);
        if (pWVar6 != NULL) {
            pHVar4 = GetDlgItem(hwnd, 1);
            SetWindowTextW(pHVar4, pWVar6);
        }
        pWVar6 = FUN_1400169BC(0x3f5, NULL);
        if (pWVar6 != NULL) {
            pHVar4 = GetDlgItem(hwnd, 2);
            SetWindowTextW(pHVar4, pWVar6);
        }
        pWVar6 = FUN_1400169BC(0x3fb, NULL);
        if (pWVar6 != NULL) {
            pHVar4 = GetDlgItem(hwnd, 0xb);
            SetWindowTextW(pHVar4, pWVar6);
        }
        pWVar6 = FUN_1400169BC(0x2788, NULL);
        if (pWVar6 != NULL) {
            pHVar4 = GetDlgItem(hwnd, 0xc);
            SetWindowTextW(pHVar4, pWVar6);
        }
        pWVar6 = FUN_1400169BC(0x3fd, NULL);
        if (pWVar6 != NULL) {
            pHVar4 = GetDlgItem(hwnd, 0xd);
            SetWindowTextW(pHVar4, pWVar6);
        }
        PECMD_TrimWorkingSet('\0');
        return 1;
    }
    if (msg == 0x111) {
        if (wParam == 2) {
            pHVar4 = GetDlgItem(hwnd, 0x757d);
            SetWindowTextW(pHVar4, g_szEmpty);
        } else {
            if (wParam == 0xd) {
                FUN_1400702B0((WCHAR **)&local_918.p, WSTR("-force "));
                FUN_14003DB00((WCHAR *)(uintptr_t)local_918.p);
            } else {
                if (wParam != 1) {
                    return 0;
                }
                local_838[0] = L'\0';
                local_918.p = 0;
                pHVar4 = GetDlgItem(hwnd, 0x757d);
                GetWindowTextW(pHVar4, local_838, 0x400);
                FUN_14006B1E8(local_838, &local_918.p, '\0');
                memset((uint64_t *)local_838, 0, 0x802);
                pWVar5 = StrChrW(local_838, L',');
                if (pWVar5 == NULL) {
                    local_908[0] = L'\0';
                    LoadStringW(g_hInstance, 0x2729, local_908, 100);
                    for (lpString1 = local_908;
                         (*lpString1 == L'*' ||
                          (((8 < (uint16_t)*lpString1 && ((uint16_t)*lpString1 < 0xe)) ||
                            (*lpString1 == L' '))));
                         lpString1 = lpString1 + 1) {
                    }
                    iVar2 = lstrcmpW((LPCWSTR)*puVar1,
                                     (LPCWSTR)((uint8_t *)(uintptr_t)local_918.p + 2));
                    if ((iVar2 == 0) ||
                        ((*lpString1 != L'\0' &&
                          (iVar2 = lstrcmpW(lpString1,
                                            (LPCWSTR)((uint8_t *)(uintptr_t)local_918.p + 2)),
                           iVar2 == 0)))) {
                        EndDialog(hwnd, 1);
                    } else if (*(int *)(puVar1 + 1) == 1) {
                        pHVar4 = GetDlgItem(hwnd, 0x757d);
                        SetWindowTextW(pHVar4, g_szEmpty);
                        pHVar4 = GetDlgItem(hwnd, 1);
                        EnableWindow(pHVar4, 0);
                        pHVar4 = GetDlgItem(hwnd, 2);
                        ShowWindow(pHVar4, 0);
                        pHVar4 = GetDlgItem(hwnd, 1);
                        ShowWindow(pHVar4, 0);
                        pHVar4 = GetDlgItem(hwnd, 0xd);
                        ShowWindow(pHVar4, 5);
                    } else {
                        *(int *)(puVar1 + 1) = *(int *)(puVar1 + 1) - 1;
                    }
                }
            }
        }
        FUN_14005B104((WCHAR **)&local_918.p);
    }
    return 0;
}
