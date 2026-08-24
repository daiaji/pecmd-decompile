/* ====================================================================
 * core_b2f.c — B2 批次大函数 6 (140020000-14003ffff)
 *
 * 本批新实现函数全部使用人类可读 PECMD_ 名称，原始地址保留在 @0x 注释。
 *
 * 来源: PECMD原始.EXE (x64, ImageBase=0x140000000)
 *   服务命令          PECMD_ServiceControl @0x140020018
 *   创建快捷方式      PECMD_LinkCreateShortcut @0x140021a4c
 *   帮助对话框过程    PECMD_HelpDlgProc @0x140028a00
 *   枚举进程          FUN_14002D708 @0x14002d708
 *   安装字体          PECMD_FontCommand @0x14002f454
 *   IF 条件求值       PECMD_EvalLoopCondition @0x140032dc4
 *   虚拟盘菜单构建    PECMD_BuildImDiskMenu @0x140034788
 *   虚拟盘命令        PECMD_ParseImdiskMountArgs @0x1400369d0
 *   LOGO 窗口过程     PECMD_LogoDlgProc @0x140037ba8
 *   显示模式命令      PECMD_DispCommand @0x140038d30
 *   设备目录扫描      FUN_14003B540 @0x14003b540
 *   文件操作命令      FUN_14003C06C @0x14003c06c
 *   结束进程/线程     FUN_14003CD0C @0x14003cd0c
 *   等待/执行命令     FUN_14003ED4C @0x14003ed4c
 *
 * 约定:
 *   - 新实现函数使用 PECMD_ 可读名；未实现依赖仍 extern FUN_ + TODO(verify)
 *   - 全局数据符号先用 extern 声明; 完整链接时统一在 core_globals.c 定义
 *   - 反编译中的 Ghidra 扩展宏 (SUB168/SEXT816/CONCATxx) 用等价整数运算替代
 *   - 个别反编译缺参/寄存器残留处按调用语义还原并标 TODO(verify)
 * ==================================================================== */

/* ====================================================================
 * b2f_part1.c — B2F 批次重构第 1 部分
 *
 * 本文件包含:
 *   PECMD_ServiceControl   @0x140020018   (PECMD_ServiceControl)
 *   PECMD_LinkCreateShortcut   @0x140021a4c   (PECMD_LinkCreateShortcut)
 *   PECMD_HelpDlgProc   @0x140028a00   (PECMD_HelpDlgProc)
 *
 * 约定:
 *   - 新实现函数使用 PECMD_ 可读名；原始地址保留在 @0x 注释。
 *   - 未实现内部依赖保留 extern FUN_<addr> 并加 TODO(verify)。
 *   - Ghidra 扩展宏 (CONCATxx/SUB168 等) 已替换为等价整数/位运算。
 * ==================================================================== */
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>

#include "pecmd_defs.h"

/* Portable (a*b)/d with 128-bit intermediate for MSVC C (no __int128).
 * Bit-by-bit long division; safe because r < d <= 2^63 keeps r<<1 in range.
 * Replaces the GCC (__uint128_t) product form. */
static uint64_t PECMD_MulDivU64(uint64_t a, uint64_t b, uint64_t d)
{
    uint64_t ua = a, ub = b;
    uint64_t al = ua & 0xffffffffULL, ah = ua >> 32;
    uint64_t bl = ub & 0xffffffffULL, bh = ub >> 32;
    uint64_t ll = al * bl;
    uint64_t lm = al * bh;
    uint64_t mh = ah * bl;
    uint64_t hh = ah * bh;
    uint64_t mid = (ll >> 32) + (lm & 0xffffffffULL) + (mh & 0xffffffffULL);
    uint64_t hi = hh + (lm >> 32) + (mh >> 32) + (mid >> 32);
    uint64_t lo = (ll & 0xffffffffULL) | (mid << 32);
    uint64_t q = 0, r = 0;
    int i;
    for (i = 127; i >= 0; i--) {
        uint64_t bit = (i >= 64) ? ((hi >> (i - 64)) & 1) : ((lo >> i) & 1);
        r = (r << 1) | bit;
        if (r >= d) {
            r -= d;
            if (i >= 64) q |= 1ULL << (i - 64); else q |= 1ULL << i;
        }
    }
    return q;
}

/* ---- 宽字符 CRT 辅助 (Linux wchar_t=4B, 这里按 PECMD 的 WCHAR=2B 声明) ---- */
extern WCHAR *wcscat(WCHAR *, const WCHAR *);
extern int wcscmp(const WCHAR *, const WCHAR *);
extern WCHAR *wcstok(WCHAR *, const WCHAR *);
extern unsigned long wcstoul(const WCHAR *, WCHAR **, int);

/* ---- 本文件需要的本地补充类型/API (合并时避免与其它批重复定义) ---- */
#ifndef B2F_PART1_LOCAL
#define B2F_PART1_LOCAL

typedef uintptr_t HDROP;

extern BOOL ChangeServiceConfigW(SC_HANDLE, DWORD, DWORD, DWORD, LPCWSTR, LPCWSTR, DWORD *, LPCWSTR,
                                 LPCWSTR, LPCWSTR, LPCWSTR);
extern BOOL ChangeServiceConfig2W(SC_HANDLE, DWORD, const void *);
extern SC_HANDLE CreateServiceW(SC_HANDLE, LPCWSTR, LPCWSTR, DWORD, DWORD, DWORD, DWORD, LPCWSTR,
                                LPCWSTR, DWORD *, LPCWSTR, LPCWSTR, LPCWSTR);
extern BOOL QueryServiceStatusEx(SC_HANDLE, int, void *, DWORD, DWORD *);
extern BOOL StartServiceW(SC_HANDLE, DWORD, const LPCWSTR *);
extern DWORD SearchPathW(LPCWSTR, LPCWSTR, LPCWSTR, DWORD, LPWSTR, LPWSTR *);
extern UINT GetDlgItemTextW(HWND, int, LPWSTR, int);
extern BOOL EndDialog(HWND, intptr_t);

#endif /* B2F_PART1_LOCAL */

/* ---- 本批内部互相调用 ---- */
uint64_t PECMD_ServiceControl(int64_t *script, LPCWSTR name);
void FUN_1400629B8(void *script, LPCWSTR key, LPCWSTR value); /* @0x1400629b8 */

/* ---- 未实现依赖 (extern) ---- */
extern void PECMD_GetApiProcCached(LPCSTR a2, LPCSTR dll, void *slot, void *err);
extern void PECMD_SkipLeadingControls(void *pp);
extern WCHAR *FUN_14001BE14(const WCHAR *s);
extern char FUN_1400660AC(char *a1, int64_t *a2, int a3);
extern void PECMD_ParseSkipSeparator(int64_t *a1, int64_t *a2, int16_t a3, int16_t a4);
extern WCHAR *PECMD_ParseIntSkipSepChar(uint64_t *a1, int *a2, int16_t a3);
extern void PECMD_SplitTokenTrimWs(int64_t *src, int64_t *dst, int16_t delim);
extern void PECMD_ExtractTokenByDelim(int64_t *src, WCHAR **dst, int mode);
extern void FUN_14007BF44(int64_t *ctx, WCHAR *a2, void *out, int mode, uint8_t flag);
extern uint64_t PECMD_ParseHotkeyCode(int64_t *a1, uint32_t *a2, int64_t *a3, char a4);
extern uint64_t PECMD_IsDirectory(LPCWSTR s);
extern uint64_t PECMD_CheckFileAttr(LPCWSTR s);
extern uint64_t PECMD_IsPeExecutable(LPCWSTR s);
extern WCHAR *FUN_14001C270(LPCWSTR src, void *out);
extern bool PECMD_ParseNumber(int64_t *a1, int *a2);
extern int32_t PECMD_LoadOle32Apis(void);
extern void FUN_14007034C(void *ps, LPCWSTR src);
extern void PECMD_QueryServiceStatus(SC_HANDLE, int);
extern void PECMD_VarSetUInt(void *s, uint64_t v, LPCWSTR k);
extern void PECMD_CreateDirectoryTree(LPCWSTR path);
extern short PECMD_NextNonDelimChar(WCHAR **pp);
extern void PECMD_GrowByteBuffer(void *pp, int64_t size);
extern void PECMD_AllocStrSlot(void *ps);
extern uint64_t PECMD_ComparePrefixNoCaseLen(LPCWSTR a, LPCWSTR b);
extern uint64_t *PECMD_NewFormattedI64Str(uint64_t *a1, int a2, int64_t a3);
extern LPCWSTR FUN_1400169BC(int id, void **pp);
extern LPCWSTR FUN_14005B6AC(HINSTANCE, UINT, LPWSTR, int);
extern int64_t *PECMD_StrBldCopyAnsi(int64_t *out, const char *src, uint64_t len);
extern int PECMD_LoadScriptSkipBom(LPCWSTR, int64_t *);
extern HMODULE FUN_14001EA18(HMODULE h, LPCWSTR a2, LPCWSTR a3, int64_t *pp, uint32_t *a5);
extern void FUN_1400E7D58(int64_t *, int);
extern int FUN_14005B184(char *a1, int64_t a2, int64_t a3);
extern uint64_t PECMD_CreateAuxWindow(HWND);
extern void PECMD_ReleaseGdiObject(int64_t *, uint64_t, HGDIOBJ);
extern void PECMD_RestoreWindowProc(int64_t);
extern HWND PECMD_ReplaceWithEditBox(HWND, DWORD, uint64_t);
extern void PECMD_HeapFreeWithHeader(void *ps);
extern void PECMD_TrimWorkingSet(char);

/* ---- 本批引用的全局数据 (完整链接时在 core_globals.c 统一定义) ---- */
extern void *g_pOpenSCManagerW;
extern void *g_pOpenServiceW;
extern void *g_pQueryServiceStatusEx;
extern void *g_pControlService;
extern void *g_pStartServiceW;
extern void *g_pCloseServiceHandle;
extern int (*g_pCoCreateInstance)(void *, void *, uint32_t, void *, void **);
extern uint8_t g_b12d1b8[];
extern uint8_t g_b12d158[];
extern uint8_t g_b12d1a8[];
extern uint64_t g_u64FontA98;
extern uint32_t g_u32FontAa0;
extern uint8_t g_b20aa8[];

/* 函数指针 vtable 调用辅助: obj 为 COM 对象指针, off 为 vtable 字节偏移 */
#define VT_CALL(obj, off, type) ((type)((void **)*(void ***)(obj))[(off) / sizeof(void *)])

/* ========== PECMD_ServiceControl @0x140020018 ==========
 * 服务管理命令: 创建/删除/修改/启动/停止/查询服务。
 * TODO(verify): 原函数返回 unaff_RBP (未初始化寄存器), 此处按 0 处理；
 *               QueryServiceStatusEx 第 5 个参数按 Win32 API 补 NULL；
 *               wsprintfW 的 %lX 参数在反编译中缺失，仅保留可见两个实参。
 */
uint64_t PECMD_ServiceControl(int64_t *script, LPCWSTR name)
{
    uint16_t uVar3;
    int16_t sVar2;
    int iVar6;
    DWORD DVar7 = 0;
    BOOL BVar8;
    int iVar9;
    LPCWSTR pWVar10;
    uint64_t *puVar11;
    uint64_t uVar12 = 0;
    uint64_t uVar13 = 1;
    DWORD DVar14 = (DWORD)-1;
    SC_HANDLE hService = (SC_HANDLE)0;
    DWORD DVar15 = 0;
    DWORD DVar16 = 0;
    WCHAR WVar17;
    LPCWSTR local_res10 = name;
    uint64_t local_res18 = 0;
    DWORD local_res20[2] = {0, 0};
    DWORD local_268 = (DWORD)-1;
    DWORD local_264 = 0;
    uint64_t local_260 = 0;
    uint64_t local_258 = L' ';
    uint64_t local_248 = L'\t';
    uint64_t local_250 = 0;
    uint64_t local_240 = 0;
    uint64_t local_238 = 0;
    LPCWSTR local_230 = NULL;
    SC_HANDLE local_228 = (SC_HANDLE)0;
    uint32_t local_220 = 0;
    uint64_t local_218 = 0;
    uint64_t local_210 = 0;
    uint32_t local_208[9] = {0};
    uint64_t local_1e0 = 0;
    uint8_t local_1d8[32] = {0};
    WCHAR local_1b8[204];
    DWORD *lpdwTagId;
    WCHAR WVar1;
    char cVar5;
    uint32_t uVarTmp;

    (void)uVar3;
    (void)local_220;
    if (g_pOpenSCManagerW == NULL) {
        PECMD_GetApiProcCached("OpenSCManagerW", "Advapi32.DLL",
                               (int64_t *)(void **)&g_pOpenSCManagerW, (int64_t *)&local_res18);
        PECMD_GetApiProcCached("OpenServiceW", "Advapi32.DLL", (int64_t *)(void **)&g_pOpenServiceW,
                               (int64_t *)&local_res18);
        PECMD_GetApiProcCached("QueryServiceStatusEx", "Advapi32.DLL",
                               (int64_t *)(void **)&g_pQueryServiceStatusEx,
                               (int64_t *)&local_res18);
        PECMD_GetApiProcCached("ControlService", "Advapi32.DLL",
                               (int64_t *)(void **)&g_pControlService, (int64_t *)&local_res18);
        PECMD_GetApiProcCached("StartServiceW", "Advapi32.DLL",
                               (int64_t *)(void **)&g_pStartServiceW, (int64_t *)&local_res18);
        PECMD_GetApiProcCached("CloseServiceHandle", "Advapi32.DLL",
                               (int64_t *)(void **)&g_pCloseServiceHandle, (int64_t *)&local_res18);
        if (g_pOpenSCManagerW == NULL) {
            return 0;
        }
    }

    SetLastError(0);
    local_230 = NULL;
    memset((uint64_t *)local_208, 0, 0x24);
    local_220 = 0;
    local_240 = 0;
    local_250 = 0;
    local_228 =
        (SC_HANDLE)((SC_HANDLE (*)(LPCWSTR, LPCWSTR, DWORD))g_pOpenSCManagerW)(NULL, NULL, 0xf003f);
    DVar7 = 0;

    if (local_228 != (SC_HANDLE)0) {
        DVar14 = (DWORD)-1;
        iVar6 = 0;
        DVar15 = 0;
        local_res18 &= 0xffffffff00000000ULL;
        local_res20[0] = 0;
        local_264 = 0;
        local_268 = (DWORD)-1;
        local_260 = 0;
        local_248 = L'\t';
        local_258 = L' ';

        if (*local_res10 == L'-') {
            do {
                cVar5 = FUN_1400660AC("-create", (int64_t *)&local_res10, 7);
                DVar7 = 0;
                if (cVar5 == '\0') {
                    cVar5 = FUN_1400660AC("-delete", (int64_t *)&local_res10, 7);
                    DVar7 = 0;
                    if (cVar5 == '\0') {
                        cVar5 = FUN_1400660AC("-wait", (int64_t *)&local_res10, 5);
                        DVar7 = 0;
                        if (cVar5 == '\0') {
                            cVar5 = FUN_1400660AC("-auto", (int64_t *)&local_res10, 5);
                            DVar7 = 0;
                            if (cVar5 == '\0') {
                                cVar5 = FUN_1400660AC("-boot", (int64_t *)&local_res10, 5);
                                DVar7 = 0;
                                if (cVar5 == '\0') {
                                    cVar5 = FUN_1400660AC("-demand", (int64_t *)&local_res10, 7);
                                    DVar7 = 0;
                                    if (cVar5 == '\0') {
                                        cVar5 =
                                            FUN_1400660AC("-disabled", (int64_t *)&local_res10, 9);
                                        DVar7 = 0;
                                        if (cVar5 == '\0') {
                                            cVar5 = FUN_1400660AC("-delayed-auto",
                                                                  (int64_t *)&local_res10, 0xd);
                                            DVar7 = 0;
                                            if (cVar5 == '\0') {
                                                cVar5 = FUN_1400660AC("-start",
                                                                      (int64_t *)&local_res10, 6);
                                                DVar7 = 0;
                                                if (cVar5 == '\0') {
                                                    cVar5 = FUN_1400660AC(
                                                        "-stop", (int64_t *)&local_res10, 5);
                                                    DVar7 = 0;
                                                    if (cVar5 == '\0') {
                                                        cVar5 = FUN_1400660AC(
                                                            "-stop-", (int64_t *)&local_res10, 6);
                                                        DVar7 = 0;
                                                        if (cVar5 == '\0') {
                                                            WVar1 = *local_res10;
                                                            while ((WVar1 != L'\0' &&
                                                                    ((((uint16_t)WVar1 < 9 ||
                                                                       (0xd < (uint16_t)WVar1)) &&
                                                                      (WVar1 != L' '))))) {
                                                                local_res10 = local_res10 + 1;
                                                                WVar1 = *local_res10;
                                                            }
                                                            DVar7 = 0;
                                                            PECMD_SkipLeadingControls(
                                                                (WCHAR **)&local_res10);
                                                        }
                                                        else {
                                                            iVar6 = -1;
                                                        }
                                                    }
                                                    else {
                                                        iVar6 = 1;
                                                    }
                                                }
                                                else {
                                                    iVar6 = 2;
                                                }
                                            }
                                            else {
                                                DVar14 = 2;
                                                local_264 = 1;
                                            }
                                        }
                                        else {
                                            DVar14 = 4;
                                        }
                                    }
                                    else {
                                        DVar14 = 3;
                                    }
                                }
                                else {
                                    DVar14 = 0;
                                }
                            }
                            else {
                                DVar14 = 2;
                            }
                        }
                        else {
                            local_240 = 1;
                        }
                        DVar15 = (DWORD)local_res18;
                    }
                    else {
                        DVar15 = (DWORD)local_res18;
                        local_res20[0] = 1;
                    }
                }
                else {
                    DVar15 = 1;
                    local_res18 = (local_res18 & 0xffffffff00000000ULL) | 1ULL;
                }
            } while (*local_res10 == L'-');
            uVar12 = local_238;
            local_268 = DVar14;
            local_260 = (uint64_t)(int64_t)iVar6;
        }

        iVar6 = (int)local_260;
        DVar14 = local_268;
        DVar16 = local_res20[0];

        if (*local_res10 == L'?') {
            local_230 = local_res10 + 1;
            local_res10 = local_230;
            while (true) {
                WVar1 = *local_res10;
                WVar17 = (WCHAR)DVar7;
                if (WVar1 == WVar17) {
                    break;
                }
                if ((((uint16_t)local_248 <= (uint16_t)WVar1) && ((uint16_t)WVar1 < 0xe)) ||
                    ((uint16_t)local_258 == WVar1)) {
                    if (*local_res10 != WVar17) {
                        *((WCHAR *)local_res10) = WVar17;
                        local_res10 = local_res10 + 1;
                    }
                    break;
                }
                local_res10 = local_res10 + 1;
            }
            PECMD_SkipLeadingControls((WCHAR **)&local_res10);
        }
        else if (*local_res10 == L'!') {
            local_res10 = local_res10 + 1;
            iVar6 = 1;
            local_260 = 1;
        }

        if (DVar15 == DVar7) {
            if (DVar16 == DVar7) {
                /* 修改/查询/启动/停止已有服务 */
                PECMD_SkipLeadingControls((WCHAR **)&local_res10);
                local_res10 = FUN_14001BE14((WCHAR *)local_res10);
                local_250 = (uint64_t)(local_230 != NULL);
                uVarTmp = 0;
                if ((int32_t)DVar14 >= 0) {
                    uVarTmp = 2;
                }
                uVarTmp = uVarTmp | 0x34;
                if (local_250 != 0) {
                    uVarTmp = 4;
                }
                hService = (SC_HANDLE)((SC_HANDLE (*)(SC_HANDLE, LPCWSTR, DWORD))g_pOpenServiceW)(
                    local_228, local_res10, uVarTmp);
                if (hService == (SC_HANDLE)0) {
                    goto label_0207ef;
                }

                if (local_230 == NULL) {
                    if ((int32_t)local_268 < 0) {
                        if (iVar6 == 1) {
                            goto label_020b13;
                        }
                    label_020ac6:
                        iVar9 = ((BOOL (*)(SC_HANDLE, DWORD, const LPCWSTR *))g_pStartServiceW)(
                            hService, 0, NULL);
                    }
                    else {
                        local_res18 = (local_res18 & 0xffffffff00000000ULL) | (uint64_t)local_264;
                        BVar8 = ChangeServiceConfigW(hService, (DWORD)-1, local_268, (DWORD)-1,
                                                     NULL, NULL, NULL, NULL, NULL, NULL, NULL);
                        if (BVar8 == 0) {
                            DVar7 = GetLastError();
                            uVar12 = (uint64_t)DVar7;
                        }
                        BVar8 = ChangeServiceConfig2W(hService, 3, &local_res18);
                        if ((local_264 != 0) && (BVar8 == 0) && ((int32_t)uVar12 == 0)) {
                            DVar7 = GetLastError();
                            uVar12 = (uint64_t)DVar7;
                        }
                        if (iVar6 != 1) {
                            if (iVar6 == 2) {
                                goto label_020ac6;
                            }
                            goto label_020ae4;
                        }
                    label_020b13:
                        iVar9 = ((BOOL (*)(SC_HANDLE, DWORD, void *))g_pControlService)(hService, 1,
                                                                                        local_1d8);
                    }
                    if ((iVar9 == 0) && ((int32_t)uVar12 == 0)) {
                        DVar7 = GetLastError();
                        uVar12 = (uint64_t)DVar7;
                    }
                }
                else {
                    iVar6 =
                        ((BOOL (*)(SC_HANDLE, int, void *, DWORD, DWORD *))g_pQueryServiceStatusEx)(
                            hService, 0, local_208, 0x24, NULL);
                    DVar7 = GetLastError();
                    uVar12 = (uint64_t)DVar7;
                    if (iVar6 == 0) {
                        iVar6 = (int)local_260;
                        if (DVar7 != 0) {
                            uVar13 = (uint64_t)DVar7;
                        }
                    }
                    else {
                        uVar13 = 0;
                        wsprintfW(local_1b8,
                                  WSTR("0 0x%lX 0x%lX 0x%lX 0x%lX 0x%lX 0x%lX 0x%lX 0x%lX 0x%lX"),
                                  local_208[0], local_208[1]);
                        iVar6 = (int)local_260;
                    }
                }

            label_020ae4:
                if ((local_240 != 0) && (local_250 == 0)) {
                    PECMD_QueryServiceStatus(hService, iVar6);
                }
                goto label_0207f7;
            }

            /* 删除服务 */
            if ((int32_t)DVar7 <= iVar6) {
                FUN_1400702B0((WCHAR **)&local_res18, WSTR("-wait !"));
                FUN_14006375C((WCHAR **)&local_res18, local_res10);
                PECMD_ServiceControl(script, (LPCWSTR)(uintptr_t)local_res18);
                PECMD_FreeStrBuf((WCHAR **)&local_res18);
            }
            PECMD_AllocStrSlot(&local_1e0);
            SetLastError(0);
            uVar12 = 0;
            if (iVar6 >= 0) {
                uVar12 = 0x20;
            }
            hService = (SC_HANDLE)((SC_HANDLE (*)(SC_HANDLE, LPCWSTR, DWORD))g_pOpenServiceW)(
                local_228, local_res10, uVar12 | 0x10000);
            if (hService != (SC_HANDLE)0) {
                DeleteService(hService);
            }
            puVar11 = &local_1e0;
        }
        else {
            /* 创建服务 */
            PECMD_AllocStrSlot(&local_238);
            PECMD_AllocStrSlot(&local_248);
            PECMD_AllocStrSlot(&local_218);
            PECMD_AllocStrSlot(&local_210);
            PECMD_AllocStrSlot(&local_258);
            PECMD_AllocStrSlot(&local_260);
            PECMD_AllocStrSlot(&local_240);
            local_res18 &= 0xffffffff00000000ULL;
            local_268 = 0x10;
            local_264 = 3;
            local_res20[0] = 1;
            PECMD_ParseSkipSeparator((int64_t *)&local_res10, (int64_t *)&local_238, 0x2c, 0);
            PECMD_ParseSkipSeparator((int64_t *)&local_res10, (int64_t *)&local_218, 0x2c, 0);
            PECMD_ParseIntSkipSepChar((uint64_t *)&local_res10, (int *)&local_268, 0x2c);
            PECMD_ParseIntSkipSepChar((uint64_t *)&local_res10, (int *)&local_264, 0x2c);
            PECMD_ParseIntSkipSepChar((uint64_t *)&local_res10, (int *)local_res20, 0x2c);
            PECMD_ParseSkipSeparator((int64_t *)&local_res10, (int64_t *)&local_258, 0x2c, 0);
            PECMD_ParseSkipSeparator((int64_t *)&local_res10, (int64_t *)&local_260, 0x2c, 0);
            PECMD_ParseSkipSeparator((int64_t *)&local_res10, (int64_t *)&local_240, 0x2c, 0);
            PECMD_ParseSkipSeparator((int64_t *)&local_res10, (int64_t *)&local_248, 0x2c, 0);
            PECMD_ParseSkipSeparator((int64_t *)&local_res10, (int64_t *)&local_210, 0x2c, 0);
            iVar6 = (int)(intptr_t)PECMD_ParseIntSkipSepChar((uint64_t *)&local_res10,
                                                             (int *)&local_res18, 0x2c);
            lpdwTagId = (DWORD *)&local_res18;
            if (iVar6 == 0) {
                lpdwTagId = NULL;
            }
            iVar6 = lstrlenW((LPCWSTR)(uintptr_t)local_258);
            PECMD_AllocString(&local_258, iVar6 + 2);
            ((WCHAR *)(uintptr_t)local_258)[iVar6 + 1] = L'\0';
            ((WCHAR *)(uintptr_t)local_258)[iVar6] = L'\0';
            sVar2 = *(int16_t *)(uintptr_t)local_260;
            if (*(int16_t *)(uintptr_t)local_248 == 0) {
                FUN_14007034C((int64_t *)&local_248, (LPCWSTR)(uintptr_t)local_238);
            }
            pWVar10 = (LPCWSTR)(uintptr_t)local_258;
            local_res10 = pWVar10;
            if (*pWVar10 != L'\0') {
                do {
                    if (*pWVar10 == L'/') {
                        *((LPWSTR)pWVar10) = L'\0';
                    }
                    local_res10 = pWVar10 + 1;
                    pWVar10 = local_res10;
                } while (*local_res10 != L'\0');
                pWVar10 = (LPCWSTR)(uintptr_t)local_258;
            }
            hService = CreateServiceW(
                local_228, (LPCWSTR)(uintptr_t)local_238, (LPCWSTR)(uintptr_t)local_248, 0xf01ff,
                local_268, local_264, local_res20[0], (LPCWSTR)(uintptr_t)local_218,
                (LPCWSTR)(uintptr_t)local_210, lpdwTagId, pWVar10,
                (LPCWSTR)(sVar2 != 0 ? (uintptr_t)(int16_t *)(uintptr_t)local_260 : 0),
                (LPCWSTR)(uintptr_t)local_240);
            PECMD_FreeStrBuf((WCHAR **)&local_240);
            PECMD_FreeStrBuf((WCHAR **)&local_260);
            PECMD_FreeStrBuf((WCHAR **)&local_258);
            PECMD_FreeStrBuf((WCHAR **)&local_210);
            PECMD_FreeStrBuf((WCHAR **)&local_218);
            PECMD_FreeStrBuf((WCHAR **)&local_248);
            puVar11 = &local_238;
        }
        PECMD_FreeStrBuf((WCHAR **)puVar11);
    }

label_0207ef:
    DVar7 = GetLastError();
    uVar12 = (uint64_t)DVar7;

label_0207f7:
    if (hService != (SC_HANDLE)0) {
        ((BOOL (*)(SC_HANDLE))g_pCloseServiceHandle)(hService);
    }
    if (local_228 != (SC_HANDLE)0) {
        ((BOOL (*)(SC_HANDLE))g_pCloseServiceHandle)(local_228);
    }
    pWVar10 = local_230;
    if (local_230 != NULL) {
        if (local_250 == 0) {
            wsprintfW(local_1b8, WSTR("%d"), uVar12);
        }
        else {
            uVar12 = uVar13;
            if ((int32_t)uVar13 != 0) {
                wsprintfW(local_1b8, WSTR("%d"), uVar12);
            }
        }
        FUN_1400629B8((void *)script, pWVar10, local_1b8);
    }
    return 0;
}

/* ========== PECMD_LinkCreateShortcut @0x140021a4c ==========
 * 创建/查询快捷方式 (IShellLinkW)。
 * TODO(verify): COM vtable 偏移/参数按反编译保留，部分反编译缺参处未补；
 *               StrStrW 在 win32_stub.h 中返回 BOOL，实际为 LPWSTR。
 */
int64_t PECMD_LinkCreateShortcut(int64_t *script, LPCWSTR text)
{
    WCHAR WVar1;
    WCHAR WVar17;
    bool bVar2 = false;
    bool bVar4 = false;
    LPCWSTR pWVar3;
    char cVar5;
    char cVar16;
    int16_t sVar6;
    int iVar7;
    int iVar8;
    DWORD DVar9;
    int iVar10;
    LPWSTR pWVar11;
    WCHAR *lpFileName;
    LPCWSTR pWVar12;
    uint64_t uVar13;
    uint64_t uVar14;
    LPWSTR pWVar15;
    int64_t lVar18 = 0;
    uint32_t uVar19 = 0;
    uint32_t uVar21 = 0;
    LPCWSTR local_res10 = text;
    uint64_t local_res18 = 0;
    uint64_t local_res20 = 0;
    LPCWSTR local_2d8;
    WCHAR *local_2d0 = NULL;
    int64_t *local_2c8 = NULL;
    char local_2c0 = 0;
    uint64_t local_2b8 = 0;
    WCHAR *local_2b0 = NULL;
    int local_2a8[2] = {0, 0};
    WCHAR *local_2a0 = NULL;
    WCHAR *local_298 = NULL;
    WCHAR *local_290 = NULL;
    WCHAR *local_288 = NULL;
    WCHAR *local_280 = NULL;
    WCHAR *local_278 = NULL;
    WCHAR *local_270 = NULL;
    WCHAR *local_268 = NULL;
    WCHAR *local_260 = NULL;
    WCHAR *local_258 = NULL;
    WCHAR *local_250 = NULL;
    WCHAR local_248[264];

    (void)iVar7;
    (void)uVar13;
    (void)uVar14;

    local_res10 = text;
    sVar6 = PECMD_NextNonDelimChar((WCHAR **)&local_res10);
    local_res10 = local_res10 + 1;
    PECMD_SkipLeadingControls((WCHAR **)&local_res10);
    PECMD_AllocStrSlot(&local_2d0);
    PECMD_AllocStrSlot(&local_280);
    PECMD_AllocStrSlot(&local_260);
    PECMD_AllocStrSlot(&local_298);
    PECMD_AllocStrSlot(&local_270);
    PECMD_AllocStrSlot(&local_278);
    PECMD_AllocStrSlot(&local_2b0);
    PECMD_AllocStrSlot(&local_268);
    PECMD_AllocStrSlot(&local_2a0);
    PECMD_AllocStrSlot(&local_290);

    local_2d8 = local_res10;
    pWVar11 = StrChrW(local_res10, L',');
    if (pWVar11 == NULL) {
        PECMD_FreeStrBuf(&local_290);
        PECMD_FreeStrBuf(&local_2a0);
        PECMD_FreeStrBuf(&local_268);
        PECMD_FreeStrBuf(&local_2b0);
        PECMD_FreeStrBuf(&local_278);
        PECMD_FreeStrBuf(&local_270);
        PECMD_FreeStrBuf(&local_298);
        PECMD_FreeStrBuf(&local_260);
        PECMD_FreeStrBuf(&local_280);
        PECMD_FreeStrBuf(&local_2d0);
        return 3;
    }

    local_2a8[0] = 0;
    local_2b8 &= 0xffffffff00000000ULL;

    if (*local_2d8 != L'\0') {
        PECMD_SplitTokenTrimWs((int64_t *)&local_2d8, (int64_t *)&local_2d0, sVar6);
        FUN_14007BF44(script, local_2d0, (void *)&local_280, 0, 1);
    }
    local_res10 = local_280;
    cVar5 = FUN_1400660AC("?", (int64_t *)&local_res10, 1);
    cVar16 = '*';
    local_res18 = (local_res18 & 0xffffffffffffff00ULL) | (uint8_t)cVar5;
    if ((*local_res10 == L'-') && (cVar16 = '*', local_res10[1] == L'-')) {
        cVar16 = '-';
    }
    WVar17 = (WCHAR)cVar16;
    bVar2 = false;
    local_res20 = local_res20 & 0xffffffffffffff00ULL;
    uVar19 = 0;
    bVar4 = false;
    if (WVar17 == *local_res10) {
        do {
            bVar2 = bVar4;
            if (WVar17 != local_res10[1]) {
                break;
            }
            local_res10 = local_res10 + 2;
            cVar5 = FUN_1400660AC("force", (int64_t *)&local_res10, 5);
            if (cVar5 == '\0') {
                cVar5 = FUN_1400660AC("f", (int64_t *)&local_res10, 1);
                if (cVar5 == '\0') {
                    while ((WVar1 = *local_res10,
                            WVar1 != L'\0' && ((((uint16_t)WVar1 < 9 || (0xd < (uint16_t)WVar1)) &&
                                                (WVar1 != L' '))))) {
                        local_res10 = local_res10 + 1;
                    }
                    PECMD_SkipLeadingControls((WCHAR **)&local_res10);
                }
                else {
                    local_res20 = (local_res20 & 0xffffffffffffff00ULL) | 1ULL;
                }
            }
            else {
                bVar2 = true;
            }
            bVar4 = bVar2;
        } while (WVar17 == *local_res10);
        uVar19 = (uint32_t)local_2b8;
    }

    if (*local_2d8 != L'\0') {
        local_2d8 = local_2d8 + 1;
        PECMD_SplitTokenTrimWs((int64_t *)&local_2d8, (int64_t *)&local_2d0, sVar6);
        FUN_14007BF44(script, local_2d0, (void *)&local_260, 0, 1);
    }
    lpFileName = FUN_14001BE14(local_260);

    if (*local_2d8 != L'\0') {
        local_2d8 = local_2d8 + 1;
        PECMD_ExtractTokenByDelim((int64_t *)&local_2d8, &local_2d0, sVar6);
        FUN_14007BF44(script, local_2d0, (void *)&local_298, 0, 1);
        if (*local_2d8 != L'\0') {
            local_2d8 = local_2d8 + 1;
            PECMD_SplitTokenTrimWs((int64_t *)&local_2d8, (int64_t *)&local_2d0, sVar6);
            FUN_14007BF44(script, local_2d0, (void *)&local_270, 0, 1);
        }
    }
    pWVar12 = FUN_14001BE14(local_270);
    local_2c0 = *pWVar12 != L'\0';
    pWVar11 = StrChrW(pWVar12, L'#');

    if (pWVar11 == NULL) {
        pWVar11 = NULL;
        pWVar15 = NULL;
        if ((char)local_res18 != '\0') {
            goto label_021e1f;
        }
        if (*local_2d8 != L'\0') {
            local_2d8 = local_2d8 + 1;
            PECMD_SplitTokenTrimWs((int64_t *)&local_2d8, (int64_t *)&local_2d0, sVar6);
            FUN_14007BF44(script, local_2d0, (void *)&local_278, 0, 1);
        }
        pWVar15 = local_278;
        if ((uint16_t)(*local_278 + 0xffd0) < 10) {
            goto label_021e1f;
        }
        FUN_14007034C((int64_t *)&local_2b0, local_278);
    }
    else {
        *pWVar11 = L'\0';
        pWVar15 = pWVar11 + 1;
    label_021e1f:
        pWVar11 = pWVar15;
        if (*local_2d8 != L'\0') {
            local_2d8 = local_2d8 + 1;
            PECMD_SplitTokenTrimWs((int64_t *)&local_2d8, (int64_t *)&local_2d0, sVar6);
            FUN_14007BF44(script, local_2d0, (void *)&local_2b0, 0, 1);
        }
    }

    local_258 = NULL;
    if (pWVar11 != NULL) {
        local_258 = pWVar11;
        PECMD_ParseNumber((int64_t *)&local_258, local_2a8);
    }

    if (*local_2d8 != L'\0') {
        local_2d8 = local_2d8 + 1;
        PECMD_SplitTokenTrimWs((int64_t *)&local_2d8, (int64_t *)&local_2d0, sVar6);
        FUN_14007BF44(script, local_2d0, (void *)&local_268, 0, 1);
    }
    local_250 = FUN_14001BE14(local_268);

    if (*local_2d8 != L'\0') {
        local_2d8 = local_2d8 + 1;
        PECMD_SplitTokenTrimWs((int64_t *)&local_2d8, (int64_t *)&local_2d0, sVar6);
        FUN_14007BF44(script, local_2d0, (void *)&local_2a0, 0, 1);
        if (*local_2d8 != L'\0') {
            local_2d8 = local_2d8 + 1;
            PECMD_SplitTokenTrimWs((int64_t *)&local_2d8, (int64_t *)&local_2d0, sVar6);
            FUN_14007BF44(script, local_2d0, (void *)&local_290, 0, 1);
        }
    }

    if (*local_2a0 != L'\0') {
        local_288 = local_2a0;
        local_2b8 &= 0xffffffff00000000ULL;
        uVar13 = PECMD_ParseHotkeyCode((int64_t *)&local_288, (uint32_t *)&local_2b8, script, '\0');
        if ((int)(uint32_t)uVar13 > 0) {
            uVar19 = 0;
            if ((local_2b8 & 1) != 0) {
                uVar19 = 4;
            }
            if ((local_2b8 & 2) != 0) {
                uVar19 = uVar19 | 2;
            }
            if ((local_2b8 & 4) != 0) {
                uVar19 = uVar19 | 1;
            }
            if ((local_2b8 & 8) != 0) {
                uVar19 = uVar19 | 8;
            }
            uVar19 = (uVar19 << 8) | (uint32_t)uVar13;
        }
    }

    if ((char)local_res18 == '\0') {
        if (*local_res10 == WVar17) {
            local_res10 = local_res10 + 1;
            local_res10 = FUN_14001BE14((WCHAR *)local_res10);
        }
        else {
            DVar9 = SearchPathW(NULL, lpFileName, NULL, 0x104, local_248, NULL);
            if (DVar9 == 0) {
                if ((!bVar2) && ((uVar14 = PECMD_IsDirectory(lpFileName), (int32_t)uVar14 == 0))) {
                    lVar18 = 3;
                    goto label_02261f;
                }
            }
            else {
                lpFileName = local_248;
            }
        }
    }

    local_2c8 = NULL;
    EnterCriticalSection((LPCRITICAL_SECTION)&g_csCom);
    PECMD_LoadOle32Apis();
    iVar7 = -3;
    if ((g_pOleInit != NULL) &&
        ((iVar7 = ((int (*)(int))g_pOleInit)(0), iVar7 != 0 && (iVar7 == 1)))) {
        ((void (*)(void))g_pOleUninit)();
    }
    iVar8 = ((int (*)(void *, int, int, void *, int64_t **))g_pCoCreateInstance)(
        (void *)g_b12d1b8, 0, 1, (void *)g_b12d158, &local_2c8);
    uVar21 = (uint32_t)((uint64_t)(uintptr_t)&local_2c8 >> 32);

    if ((iVar8 == 0) && (local_2c8 != NULL)) {
        local_2b8 = 0;
        iVar8 = VT_CALL(local_2c8, 0, int (*)(int64_t *, void *, uint64_t *))(
            local_2c8, (void *)g_b12d1a8, &local_2b8);
        if (iVar8 == 0) {
            iVar10 = -1;
            if (*local_res10 == L'_') {
                local_res10 = local_res10 + 1;
                iVar10 = 7;
            }
            if (*local_res10 == L'^') {
                local_res10 = local_res10 + 1;
                iVar10 = 3;
            }
            if (*local_res10 == L'!') {
                local_res10 = local_res10 + 1;
                iVar10 = 0;
            }
            if (((char)local_res18 == '\0') && (iVar10 >= 0)) {
                iVar8 = VT_CALL(local_2c8, 0x78, int (*)(int64_t *, int))(local_2c8, iVar10);
            }
            local_res10 = FUN_14001BE14((WCHAR *)local_res10);
            FUN_1400702B0(&local_288, local_res10);
            if ((char)local_res20 == '\0') {
                FUN_14006375C(&local_288, WSTR(".LNK"));
            }
            pWVar3 = local_288;

            if ((char)local_res18 == '\0') {
                VT_CALL(local_2c8, 0xa0, int (*)(int64_t *, LPCWSTR))(local_2c8, lpFileName);
                if (*local_298 != L'\0') {
                    VT_CALL(local_2c8, 0x58, void (*)(void))();
                }
                if (local_2c0 != '\0') {
                    local_res18 = 0;
                    uVar13 = PECMD_CheckFileAttr(pWVar12);
                    if ((0 < (int64_t)uVar13) &&
                        ((local_2a8[0] != 0 ||
                          (uVar14 = PECMD_IsPeExecutable(pWVar12), (int32_t)uVar14 == 0)))) {
                        pWVar12 = FUN_14001C270(pWVar12, (WCHAR **)&local_res18);
                    }
                    VT_CALL(local_2c8, 0x88, int (*)(int64_t *, LPCWSTR, int))(local_2c8, pWVar12,
                                                                               local_2a8[0]);
                    PECMD_FreeStrBuf((WCHAR **)&local_res18);
                }
                if (*local_2b0 != L'\0') {
                    VT_CALL(local_2c8, 0x38, void (*)(void))();
                }
                pWVar12 = local_250;
                if (*local_250 == L'\0') {
                    pWVar15 = StrRChrW(lpFileName, NULL, L'\\');
                    pWVar11 = lpFileName;
                    if (pWVar15 != NULL) {
                        *pWVar15 = L'\0';
                        pWVar11 = pWVar15 + 1;
                    }
                    pWVar12 = lpFileName;
                    if (pWVar11 != lpFileName) {
                        goto label_022598;
                    }
                }
                else {
                label_022598:
                    VT_CALL(local_2c8, 0x48, int (*)(int64_t *, LPCWSTR))(local_2c8, pWVar12);
                }
                if ((int32_t)uVar19 > 0) {
                    VT_CALL(local_2c8, 0x68, int (*)(int64_t *, uint16_t))(local_2c8,
                                                                           uVar19 & 0xffff);
                }
                PECMD_CreateDirectoryTree(local_res10);
                iVar8 = VT_CALL((int64_t *)(uintptr_t)local_2b8, 0x30,
                                int (*)(int64_t *, LPCWSTR, int))((int64_t *)(uintptr_t)local_2b8,
                                                                  pWVar3, 0);
            }
            else {
                VT_CALL((int64_t *)(uintptr_t)local_2b8, 0x28, int (*)(int64_t *, LPCWSTR, int))(
                    (int64_t *)(uintptr_t)local_2b8, local_288, 0);
                PECMD_AllocWStringBuffer((WCHAR **)&local_res20, 0x4ed);
                if (*lpFileName != L'\0') {
                    VT_CALL(local_2c8, 0x18, int (*)(int64_t *, LPCWSTR, int, int, uint64_t))(
                        local_2c8, (LPCWSTR)(uintptr_t)local_res20, 0x4ec, 0,
                        ((uint64_t)uVar21 << 32) | 2);
                    FUN_1400629B8((void *)script, lpFileName, (LPCWSTR)(uintptr_t)local_res20);
                }
                if (*local_298 != L'\0') {
                    *(WCHAR *)(uintptr_t)local_res20 = L'\0';
                    VT_CALL(local_2c8, 0x50, int (*)(int64_t *, LPCWSTR, int))(
                        local_2c8, (LPCWSTR)(uintptr_t)local_res20, 0x4ec);
                    FUN_1400629B8((void *)script, local_298, (LPCWSTR)(uintptr_t)local_res20);
                }
                if (*local_2b0 != L'\0') {
                    *(WCHAR *)(uintptr_t)local_res20 = L'\0';
                    VT_CALL(local_2c8, 0x30, int (*)(int64_t *, LPCWSTR, int))(
                        local_2c8, (LPCWSTR)(uintptr_t)local_res20, 0x4ec);
                    FUN_1400629B8((void *)script, local_2b0, (LPCWSTR)(uintptr_t)local_res20);
                }
                if (*pWVar12 != L'\0') {
                    *(WCHAR *)(uintptr_t)local_res20 = L'\0';
                    local_res18 &= 0xffffffff00000000ULL;
                    VT_CALL(local_2c8, 0x80, int (*)(int64_t *, LPCWSTR, int, uint64_t *))(
                        local_2c8, (LPCWSTR)(uintptr_t)local_res20, 0x4ba, &local_res18);
                    iVar10 = lstrlenW((LPCWSTR)(uintptr_t)local_res20);
                    local_2d8 = (LPCWSTR)((uintptr_t)local_res20 + (int64_t)iVar10 * 2);
                    if (pWVar11 != NULL) {
                        local_2d8 = local_2d8 + 1;
                    }
                    wsprintfW((LPWSTR)local_2d8, WSTR("#%d"), local_2a8[0]);
                    if (pWVar11 != NULL) {
                        local_2d8 = local_2d8 + 1;
                    }
                    FUN_1400629B8((void *)script, pWVar12, (LPCWSTR)(uintptr_t)local_res20);
                    if ((pWVar11 != NULL) && (*pWVar11 != L'\0')) {
                        FUN_1400629B8((void *)script, pWVar11, local_2d8);
                    }
                }
                pWVar12 = local_250;
                if (*local_250 != L'\0') {
                    *(WCHAR *)(uintptr_t)local_res20 = L'\0';
                    VT_CALL(local_2c8, 0x40, int (*)(int64_t *, LPCWSTR, int))(
                        local_2c8, (LPCWSTR)(uintptr_t)local_res20, 0x4ec);
                    FUN_1400629B8((void *)script, pWVar12, (LPCWSTR)(uintptr_t)local_res20);
                }
                if (*local_2a0 != L'\0') {
                    local_res18 &= 0xffffffffffff0000ULL;
                    VT_CALL(local_2c8, 0x60, int (*)(int64_t *, uint64_t *))(local_2c8,
                                                                             &local_res18);
                    PECMD_VarSetUInt((void *)script, local_res18 & 0xffff, local_2a0);
                }
                if (*local_290 != L'\0') {
                    local_res18 &= 0xffffffff00000000ULL;
                    VT_CALL(local_2c8, 0x70, int (*)(int64_t *, uint64_t *))(local_2c8,
                                                                             &local_res18);
                    PECMD_VarSetUInt((void *)script, local_res18 & 0xffffffff, local_290);
                }
                PECMD_FreeStrBuf((WCHAR **)&local_res20);
            }
            VT_CALL((int64_t *)(uintptr_t)local_2b8, 0x10,
                    int (*)(int64_t *))((int64_t *)(uintptr_t)local_2b8);
            PECMD_FreeStrBuf(&local_288);
        }
        VT_CALL(local_2c8, 0x10, int (*)(int64_t *))(local_2c8);
    }

    lVar18 = (int64_t)iVar8;
    if ((g_pOleUninit != NULL) && (iVar7 == 0)) {
        ((void (*)(void))g_pOleUninit)();
    }
    LeaveCriticalSection((LPCRITICAL_SECTION)&g_csCom);

label_02261f:
    PECMD_FreeStrBuf(&local_290);
    PECMD_FreeStrBuf(&local_2a0);
    PECMD_FreeStrBuf(&local_268);
    PECMD_FreeStrBuf(&local_2b0);
    PECMD_FreeStrBuf(&local_278);
    PECMD_FreeStrBuf(&local_270);
    PECMD_FreeStrBuf(&local_298);
    PECMD_FreeStrBuf(&local_260);
    PECMD_FreeStrBuf(&local_280);
    PECMD_FreeStrBuf(&local_2d0);
    return lVar18;
}

/* ========== PECMD_HelpDlgProc @0x140028a00 ==========
 * 帮助对话框窗口过程。
 * TODO(verify): HWND 类型被 Ghidra 复用作 WCHAR* 缓冲/整型，偏移按原反编译保留；
 *               StrStrW 在 stub 中返回 BOOL，实际为 LPWSTR。
 */
uint64_t PECMD_HelpDlgProc(int *app, HWND hwnd, uint32_t msg, HDC hdc, HWND wnd)
{
    LPWSTR pWVar1;
    WCHAR WVar2;
    LPWSTR pWVar3;
    bool bVar4 = false;
    bool bVar31 = false;
    uint32_t uVar5;
    int iVar6 = 0;
    uint32_t uVar7;
    int extraout_EAX;
    int iVar8;
    HICON lParam;
    HBRUSH pHVar9;
    LPWSTR pWVar10;
    int64_t *plVar11;
    WCHAR *psVar12;
    HWND pHVar13;
    uint64_t uVar14;
    HWND hWnd;
    LPWSTR pWVar15;
    uint64_t uVar16 = 0;
    LRESULT LVar17;
    HWND pHVar18;
    LRESULT LVar19;
    LONG_PTR LVar20;
    uint64_t *puVar21;
    HWND *ppHVar22;
    int16_t sVar23;
    uint64_t uVar24;
    LPWSTR pWVar25;
    int iVar26;
    DWORD DVar27;
    HINSTANCE pHVar28;
    int iVar29 = 0;
    int iVar30;
    uint32_t local_res18[2] = {0, 0};
    UINT UVar32;
    uint32_t local_258 = 0;
    LPWSTR local_250 = NULL;
    RECT local_248;
    HWND local_238 = (HWND)0;
    RECT local_230;
    uint32_t local_220[2] = {0, 0};
    RECT local_218;
    HWND local_208 = (HWND)0;
    HWND local_200 = (HWND)0;
    RECT local_1f8;
    RECT local_1e8;
    HWND local_1d8 = (HWND)0;
    RECT local_1d0;
    HWND local_1c0 = (HWND)0;
    HWND local_1b8 = (HWND)0;
    HWND local_1b0 = (HWND)0;
    POINT local_1a8;
    HWND local_1a0 = (HWND)0;
    RECT local_190;
    int64_t local_180 = 0;
    int64_t local_178 = 0;
    int64_t local_170 = 0;
    int64_t local_168 = 0;
    int64_t local_160 = 0;
    int64_t local_158 = 0;
    int64_t local_150 = 0;
    uint64_t local_148 = 0;
    uint32_t local_140 = 0;
    uint32_t local_13c[32] = {0};

    (void)pHVar28;
    (void)uVar14;
    (void)uVar16;
    (void)local_140;

    uVar7 = (uint32_t)((uint64_t)hdc >> 16);
    if (msg < 0x111) {
        if (msg == 0x110) {
            lParam = LoadIconW(g_hInstance, (LPCWSTR)1);
            SendMessageW(hwnd, 0x80, 1, (LPARAM)lParam);
            SendMessageW(hwnd, 0x80, 0, (LPARAM)lParam);
            pHVar18 = (HWND)0;
            if (*(int64_t *)(app + 0x36) == 0) {
                pHVar9 = CreateSolidBrush((COLORREF)(uint32_t)app[0x34]);
                *(HBRUSH *)(app + 0x36) = pHVar9;
            }
            pWVar10 = (LPWSTR)FUN_1400169BC(0x3fe, NULL);
            if (pWVar10 != NULL) {
                SetDlgItemTextW(hwnd, 0x7579, pWVar10);
            }
            pWVar10 = (LPWSTR)FUN_1400169BC(0x3ff, NULL);
            if (pWVar10 != NULL) {
                SetDlgItemTextW(hwnd, 0x7573, pWVar10);
            }
            pWVar10 = (LPWSTR)FUN_1400169BC(0x400, NULL);
            if (pWVar10 != NULL) {
                SetDlgItemTextW(hwnd, 0x757b, pWVar10);
            }

            PECMD_AllocWStringBuffer((WCHAR **)&local_238, 0x516);
            *(WCHAR *)(uintptr_t)local_238 = L'\0';
            pHVar13 = (HWND)(uintptr_t)((WCHAR *)(uintptr_t)local_238 + 0x82);
            *(WCHAR *)(uintptr_t)pHVar13 = L'\0';
            *(WCHAR *)((uintptr_t)local_238 + (0x208 + 1) * 2) = L'\0';
            GetDlgItemTextW(hwnd, 0x136, (LPWSTR)(uintptr_t)local_238, 0x104);
            pWVar10 = (LPWSTR)FUN_14005B6AC(
                g_hInstance, 0x271e, (LPWSTR)(uintptr_t)((WCHAR *)(uintptr_t)local_238 + 0x208 + 1),
                0x104);
            _snwprintf((WCHAR *)(uintptr_t)pHVar13, 0x30c, (WCHAR *)(uintptr_t)local_238,
                       WSTR("201201.88.05.94 X64"), pWVar10);
            *(WCHAR *)((uintptr_t)local_238 + 0x208 * 2) = L'\0';
            SetDlgItemTextW(hwnd, 0x136, (LPCWSTR)(uintptr_t)pHVar13);
            GetDlgItemTextW(hwnd, 0x140, (LPWSTR)(uintptr_t)local_238, 0x104);
            _snwprintf((WCHAR *)(uintptr_t)pHVar13, 0x208, (WCHAR *)(uintptr_t)local_238,
                       PTR_u_2024_05_12_14013a000);
            *(WCHAR *)((uintptr_t)local_238 + 0x186 * 2) = L'\0';
            SetDlgItemTextW(hwnd, 0x140, (LPCWSTR)(uintptr_t)pHVar13);

            PECMD_AllocWStringBuffer((WCHAR **)&local_250, 0x105);
            *local_250 = L'\0';
            GetModuleFileNameW(g_hInstance, local_250, 0x104);
            local_200 = (HWND)(uintptr_t)-1;
            plVar11 = PECMD_StrBldCopyAnsi(&local_160, ".", (uint64_t)-1);
            FUN_14006375C((WCHAR **)&local_250, (LPCWSTR)(uintptr_t)*plVar11);
            PECMD_FreeStrBuf((WCHAR **)&local_160);
            iVar8 = lstrlenW(local_250);
            FUN_14006375C((WCHAR **)&local_250, g_pLocale);
            plVar11 = PECMD_StrBldCopyAnsi(&local_180, ".help", (uint64_t)-1);
            FUN_14006375C((WCHAR **)&local_250, (LPCWSTR)(uintptr_t)*plVar11);
            PECMD_FreeStrBuf((WCHAR **)&local_180);
            iVar26 = PECMD_LoadScriptSkipBom(local_250, (int64_t *)(app + 0x1a));
            if (iVar26 < 1) {
                local_250[iVar8] = L'\0';
                FUN_14006375C((WCHAR **)&local_250, g_pLocale);
                plVar11 = PECMD_StrBldCopyAnsi(&local_158, ".lang", (uint64_t)-1);
                FUN_14006375C((WCHAR **)&local_250, (LPCWSTR)(uintptr_t)*plVar11);
                PECMD_FreeStrBuf((WCHAR **)&local_158);
                iVar26 = PECMD_LoadScriptSkipBom(local_250, (int64_t *)(app + 0x1a));
                if (iVar26 < 1) {
                    local_250[iVar8] = L'\0';
                    plVar11 = PECMD_StrBldCopyAnsi(&local_170, "help", (uint64_t)-1);
                    FUN_14006375C((WCHAR **)&local_250, (LPCWSTR)(uintptr_t)*plVar11);
                    PECMD_FreeStrBuf((WCHAR **)&local_170);
                    iVar26 = PECMD_LoadScriptSkipBom(local_250, (int64_t *)(app + 0x1a));
                    if (iVar26 < 1) {
                        local_250[iVar8] = L'\0';
                        plVar11 = PECMD_StrBldCopyAnsi(&local_150, "lang", (uint64_t)-1);
                        FUN_14006375C((WCHAR **)&local_250, (LPCWSTR)(uintptr_t)*plVar11);
                        PECMD_FreeStrBuf((WCHAR **)&local_150);
                        iVar26 = PECMD_LoadScriptSkipBom(local_250, (int64_t *)(app + 0x1a));
                    }
                }
            }

            pHVar28 = g_hInst;
            if (iVar26 < 1) {
                do {
                    if ((char)app[0x44] == '\0') {
                        if (*(int64_t *)(app + 0x1c) < 1000) {
                            FUN_14001EA18(pHVar28, (LPCWSTR)(uintptr_t)g_SysCodePage, WSTR("HELP"),
                                          (int64_t *)(app + 0x1a), NULL);
                        }
                        if (((char)app[0x44] == '\0') && (*(int64_t *)(app + 0x1c) < 1000)) {
                            FUN_14001EA18(pHVar28, (LPCWSTR)(uintptr_t)g_SysCodePage, WSTR("LANG"),
                                          (int64_t *)(app + 0x1a), NULL);
                        }
                    }
                    if (*(int64_t *)(app + 0x1c) < 100) {
                        FUN_14001EA18(pHVar28, *(LPCWSTR *)(app + 0x42), (LPCWSTR)(uintptr_t)10,
                                      (int64_t *)(app + 0x1a), NULL);
                    }
                } while ((pHVar28 != g_hInstance) &&
                         (pHVar28 = g_hInstance, *(int64_t *)(app + 0x1c) < 100));
                FUN_1400E7D58((int64_t *)(app + 0x1a), 1);
            }

            psVar12 = *(WCHAR **)(app + 0x1a);
            while ((sVar23 = *psVar12, sVar23 == 0x23)) {
                while (((sVar23 != 0 && (sVar23 != 0xd)) && (sVar23 != 10))) {
                    psVar12 = psVar12 + 1;
                    sVar23 = *psVar12;
                }
                for (; (*psVar12 == 10 || (*psVar12 == 0xd)); psVar12 = psVar12 + 1) {
                }
            }
            *(WCHAR **)(app + 0x22) = psVar12;
            *(WCHAR **)(app + 0x20) = psVar12;
            while (true) {
                if (*psVar12 == 0) {
                    goto label_029032;
                }
                if ((**(WCHAR **)(app + 0x20) == 10) &&
                    (iVar8 = FUN_14005B184((char *)*(WCHAR **)(app + 0x20),
                                           (int64_t)(uintptr_t)g_b20aa8, 0x1a),
                     iVar8 == 0)) {
                    break;
                }
                *(int64_t *)(app + 0x20) = *(int64_t *)(app + 0x20) + 2;
                psVar12 = *(WCHAR **)(app + 0x20);
            }
            app[0x24] = (int)((*(int64_t *)(app + 0x20) - *(int64_t *)(app + 0x22)) >> 1) + 1;
            for (psVar12 = (WCHAR *)(*(int64_t *)(app + 0x20) + 0x1a);
                 (*(WCHAR **)(app + 0x20) = psVar12, *psVar12 == 10 || (*psVar12 == 0xd));
                 psVar12 = psVar12 + 1) {
            }
        label_029032:
            pWVar10 = (LPWSTR) * (LPCWSTR *)(app + 0x20);
            *(uint8_t *)(app + 0x25) = (*pWVar10 == L'\u2554'); /* ╔ */
            if (app[0x24] == 0) {
                app[0x24] = (int)((intptr_t)pWVar10 - *(intptr_t *)(app + 0x22)) >> 1;
            }
            pHVar13 = GetDlgItem(hwnd, (uint32_t)*(uint16_t *)((uint8_t *)app + 0x112));
            *(HWND *)(app + 0x28) = pHVar13;
            if (*(int64_t *)(app + 0x42) != 0x12d) {
                ShowWindow(pHVar13, 0);
                *(uint16_t *)((uint8_t *)app + 0x112) = 0x132;
                pHVar13 = GetDlgItem(hwnd, 0x132);
                *(HWND *)(app + 0x28) = pHVar13;
                ShowWindow(pHVar13, 5);
            }
            SetDlgItemTextW(hwnd, (uint32_t)*(uint16_t *)((uint8_t *)app + 0x112), pWVar10);
            local_1a8.x = 0;
            local_1a8.y = 0;
            ClientToScreen(hwnd, &local_1a8);
            GetWindowRect(hwnd, &local_230);
            uVar14 = PECMD_CreateAuxWindow(hwnd);
            *(int64_t *)(app + 0x40) = (int64_t)uVar14;
            pHVar13 = GetDlgItem(hwnd, 0x7577);
            *(HWND *)(app + 0x26) = pHVar13;
            pHVar13 = GetDlgItem(hwnd, 0x7578);
            *(HWND *)(app + 0x2a) = pHVar13;
            SendMessageW(pHVar13, 0xc5, 0x7fffffffffffffff, 0);
            pHVar13 = GetDlgItem(hwnd, 0x7579);
            local_1d8 = pHVar13;
            local_1c0 = GetDlgItem(hwnd, 0x757e);
            local_1b8 = GetDlgItem(hwnd, 0x757a);
            hWnd = GetDlgItem(hwnd, 0x7573);
            DragAcceptFiles(*(HWND *)(app + 0x28), 1);
            DragAcceptFiles(*(HWND *)(app + 0x2a), 1);
            GetWindowRect(*(HWND *)(app + 0x28), &local_1d0);
            GetWindowRect(*(HWND *)(app + 0x26), &local_248);
            GetWindowRect(*(HWND *)(app + 0x2a), &local_1f8);
            GetWindowRect(pHVar13, &local_218);
            GetWindowRect(local_1c0, &local_190);
            GetWindowRect(local_1b8, &local_1e8);
            app[0x15] = local_1e8.top - local_218.top;
            app[0x14] = local_1e8.left - local_218.left;
            GetWindowRect(hWnd, &local_1e8);
            app[0x16] = local_1e8.left - local_218.left;
            app[0x17] = local_1e8.top - local_218.top;
            pHVar13 = GetDlgItem(hwnd, 0x757b);
            GetWindowRect(pHVar13, &local_1e8);
            app[0x19] = local_1e8.top - local_218.top;
            app[0x18] = local_1e8.left - local_218.left;
            app[0xf] = local_1a8.y - local_230.top;
            app[0xe] = local_1a8.x - local_230.left;
            iVar26 = GetSystemMetrics(0x3d);
            iVar6 = GetSystemMetrics(0x3e);
            uVar7 = local_res18[0] >> 8;
            local_res18[0] = local_res18[0] & 0xffffff00u;
            iVar30 = local_230.bottom - iVar6;
            iVar8 = local_1f8.bottom - iVar6;
            if (local_1f8.bottom - iVar6 < iVar30) {
                iVar8 = iVar30;
            }
            if (iVar8 > 0) {
                local_1d0.bottom = local_1d0.bottom - iVar8;
                local_248.bottom = local_248.bottom - iVar8;
                local_230.bottom = local_230.bottom - iVar30;
                local_1f8.top = local_1f8.top - iVar8;
                if (iVar6 - app[0xe] < local_1f8.bottom) {
                    local_1f8.bottom = iVar6 - app[0xe];
                }
                local_218.top = local_218.top - iVar8;
                local_218.bottom = local_218.bottom - iVar8;
                local_res18[0] = (local_res18[0] & 0xffffff00u) | 1u;
            }
            iVar6 = local_230.right - iVar26;
            iVar8 = local_248.right - iVar26;
            if (local_248.right - iVar26 < iVar6) {
                iVar8 = iVar6;
            }
            if (iVar8 > 0) {
                local_res18[0] = local_res18[0] | 2;
                iVar8 = iVar8 + 1;
                local_1f8.right = local_1f8.right - iVar8;
                local_248.left = local_248.left - iVar8;
                iVar26 = iVar26 + app[0xe] * -2;
                local_230.right = local_230.right - iVar6;
                local_1d0.right = local_1d0.right - iVar8;
                if (iVar26 < local_248.right) {
                    local_248.right = iVar26;
                }
                local_218.left = local_218.left - iVar8;
                if (iVar26 < local_218.right) {
                    local_218.right = iVar26;
                }
            }
            if ((char)local_res18[0] != '\0') {
                SetWindowPos(hwnd, (HWND)0, 0, 0, local_230.right - local_230.left,
                             local_230.bottom - local_230.top, 6);
                SetWindowPos(*(HWND *)(app + 0x28), (HWND)0, 0, 0, local_1d0.right - local_1d0.left,
                             local_1d0.bottom - local_1d0.top, 6);
                SetWindowPos(*(HWND *)(app + 0x26), (HWND)0, 0, 0, local_248.right - local_248.left,
                             local_248.bottom - local_248.top, 6);
                SetWindowPos(*(HWND *)(app + 0x2a), (HWND)0,
                             (local_1f8.left - app[0xe]) - local_230.left,
                             (local_1f8.top - app[0xf]) - local_230.top,
                             local_1f8.right - local_1f8.left, local_1f8.bottom - local_1f8.top, 4);
                SetWindowPos(local_1d8, (HWND)0, (local_218.left - app[0xe]) - local_230.left,
                             (local_218.top - app[0xf]) - local_230.top,
                             local_218.right - local_218.left, local_218.bottom - local_218.top, 4);
                SetWindowPos(local_1b8, (HWND)0, (local_1e8.left - app[0xe]) - local_230.left,
                             (local_1e8.top - app[0xf]) - local_230.top,
                             local_1e8.right - local_1e8.left, local_1e8.bottom - local_1e8.top, 4);
                SetWindowPos(local_1c0, (HWND)0, (local_190.left - app[0xe]) - local_230.left,
                             (local_190.top - app[0xf]) - local_230.top,
                             local_190.right - local_1e8.left, local_190.bottom - local_1e8.top, 4);
            }
            local_1c0 =
                (local_1c0 & 0xffffffff00000000ULL) | (uint32_t)(local_230.bottom - local_230.top);
            local_220[0] = local_230.right - local_230.left;
            app[0xc] = local_1f8.bottom - local_1f8.top;
            app[10] = (local_1f8.left - app[0xe]) - local_230.left;
            app[0xd] = ((local_1f8.left - local_1f8.right) - local_230.left) + local_230.right;
            app[0xb] = (local_230.bottom - local_1f8.top) + app[0xf];
            app[9] = local_220[0];
            app[8] = (local_1d0.top - local_230.top) - app[0xf];
            app[1] = ((local_1d0.top - local_1d0.bottom) - local_230.top) + local_230.bottom;
            app[2] = local_230.right - local_1d0.right;
            app[3] = local_1d0.right - local_1d0.left;
            *app = ((local_1d0.left - local_1d0.right) - local_230.left) + local_230.right;

            pHVar13 = GetDlgItem(hwnd, (uint32_t)*(uint16_t *)((uint8_t *)app + 0x112));
            PostMessageW(pHVar13, 0xb1, 0xffffffffffffffff, -1);
            PECMD_AllocStrSlot(&local_208);
            pWVar10 = (LPWSTR) * (LPCWSTR *)(app + 0x22);
            local_1d8 = (HWND)(uintptr_t)(pWVar10 + app[0x24]);
            *(uint16_t *)(app + 0x32) = 0;
            while ((LPCWSTR)(uintptr_t)local_1d8 > pWVar10) {
                for (; ((WVar2 = *pWVar10, WVar2 != L'\0' && (WVar2 != L'\r')) && (WVar2 != L'\n'));
                     pWVar10 = pWVar10 + 1) {
                }
                for (; (*pWVar10 == L'\n' || (*pWVar10 == L'\r')); pWVar10 = pWVar10 + 1) {
                }
                *(uint16_t *)(app + 0x32) = (uint16_t)(*(uint16_t *)(app + 0x32) + 1);
            }
            uVar24 = (uint64_t)((uint16_t)*(uint16_t *)(app + 0x32) * 8 + 0x10);
            PECMD_GrowByteBuffer((uint64_t *)(app + 0x30), (int64_t)uVar24);
            memset(*(uint64_t **)(app + 0x30), 0, uVar24);
            local_1b0 = *(HWND *)(app + 0x30);
            iVar8 = lstrlenW(*(LPCWSTR *)(app + 0x2e));
            pWVar10 = (LPWSTR) * (LPCWSTR *)(app + 0x22);
            local_1a0 = (HWND)(intptr_t)iVar8;
            local_res18[0] = 0;
            local_258 = 0;
            pHVar13 = pHVar18;
            if ((LPCWSTR)(uintptr_t)local_1d8 > pWVar10) {
                do {
                    if ((int)(int16_t)*(uint16_t *)(app + 0x32) <= (int)(intptr_t)pHVar18) {
                        break;
                    }
                    pWVar25 = (LPWSTR)pWVar10;
                    if ((intptr_t)local_1a0 > 0) {
                        uVar24 = PECMD_ComparePrefixNoCaseLen(pWVar10, *(LPCWSTR *)(app + 0x2e));
                        if ((int)local_res18[0] < (int)(uint32_t)uVar24) {
                            local_200 = pHVar13;
                            local_res18[0] = (uint32_t)uVar24;
                        }
                    }
                    for (; ((WVar2 = *pWVar25,
                           WVar2 != L'\0' && (((uint16_t)WVar2 < 9 || (0xd < (uint16_t)WVar2)))) &&
                            (WVar2 != L' '));
                         pWVar25 = pWVar25 + 1) {
                    }
                    PECMD_StrCopyW((WCHAR **)&local_208, pWVar10,
                                   ((intptr_t)pWVar25 - (intptr_t)pWVar10) >> 1);
                    pWVar10 = pWVar25;
                    if (*pWVar25 == L' ') {
                        pWVar25 = pWVar25 + 1;
                        pWVar10 = pWVar25;
                    }
                    for (; ((WVar2 = *pWVar25, WVar2 != L'\0' && (WVar2 != L'\r')) &&
                            (WVar2 != L'\n'));
                         pWVar25 = pWVar25 + 1) {
                    }
                    if (*pWVar25 == L'\0') {
                        goto label_0298c9;
                    }
                    *pWVar25 = L'\0';
                    do {
                        pWVar25 = pWVar25 + 1;
                    label_0298c9:;
                    } while ((*pWVar25 == L'\n') || (*pWVar25 == L'\r'));
                    *(LPCWSTR *)(uintptr_t)local_1b0 = pWVar10;
                    local_1b0 = (HWND)((uintptr_t)local_1b0 + 8);
                    for (; *pWVar10 != L'\0'; pWVar10 = pWVar10 + 1) {
                        if (*pWVar10 == L'*') {
                            pWVar1 = pWVar10 + 1;
                            if (*pWVar1 == L'*') {
                                *pWVar10 = L'\r';
                                *pWVar1 = L'\n';
                                pWVar10 = pWVar1;
                            }
                            else {
                                *pWVar10 = L'\n';
                            }
                        }
                    }
                    SendMessageW(*(HWND *)(app + 0x26), 0x180, 0, (LPARAM)(uintptr_t)local_208);
                    local_258 = local_258 + 1;
                    pHVar18 = (HWND)(uintptr_t)local_258;
                    pWVar10 = pWVar25;
                    pHVar13 = (HWND)(uintptr_t)((intptr_t)pHVar13 + 1);
                } while ((LPCWSTR)(uintptr_t)local_1d8 > pWVar25);
            }
            app[6] = local_230.right - local_248.left;
            app[4] = local_248.right - local_248.left;
            app[0x10] = (local_220[0] - local_218.left) + app[0xe] + local_230.left;
            app[0x11] = (app[0xf] - local_218.top) + local_230.bottom;
            SetWindowPos(*(HWND *)(app + 0x26), (HWND)0,
                         local_220[0] - (local_230.right - local_248.left), app[8],
                         local_248.right - local_248.left, ((int)local_1c0 - app[1]) + 3, 4);
            if ((int)local_200 < 0) {
                FUN_1400702B0((WCHAR **)&local_200, *(LPCWSTR *)(app + 0x2e));
                pHVar18 = (HWND)(uintptr_t)local_200;
                if (*(uint16_t *)(uintptr_t)pHVar18 == 0x7e) {
                    pHVar18 = (HWND)(uintptr_t)((WCHAR *)(uintptr_t)pHVar18 + 1);
                }
                if (*(uint16_t *)(uintptr_t)pHVar18 == 0x2a) {
                    *(uint16_t *)(uintptr_t)pHVar18 = 10;
                }
                SendMessageW(hwnd, 0x452, (WPARAM)pHVar18, 1);
                PECMD_FreeStrBuf((WCHAR **)&local_200);
            }
            else {
                SendMessageW(*(HWND *)(app + 0x26), 0x186, (WPARAM)(intptr_t)(int)local_200, 0);
                app[0x13] = 0;
                SendMessageW(hwnd, 0x111, 0x27577, *(LPARAM *)(app + 0x26));
            }
            PECMD_TrimWorkingSet('\x01');
            LVar20 = SetWindowLongPtrW(*(HWND *)(app + 0x28), -4, 0x140028560);
            *(LONG_PTR *)(app + 0x38) = LVar20;
            LVar20 = SetWindowLongPtrW(*(HWND *)(app + 0x2a), -4, 0x140028708);
            *(LONG_PTR *)(app + 0x3a) = LVar20;
            GetLastError();
            if ((int32_t)app[0x12] == (int32_t)0x80000000) {
                app[0x12] = app[0xc];
            }
            puVar21 = PECMD_NewFormattedI64Str((uint64_t *)&local_178, 1, (int64_t)app[0x12]);
            SetWindowTextW(local_1b8, (LPCWSTR)(uintptr_t)*puVar21);
            PECMD_FreeStrBuf((WCHAR **)&local_178);
            PostMessageW(hwnd, 0x44e, 0, 0);
            PECMD_FreeStrBuf((WCHAR **)&local_208);
            PECMD_FreeStrBuf((WCHAR **)&local_250);
            PECMD_FreeStrBuf((WCHAR **)&local_238);
            return 1;
        }

        if (msg != 5) {
            if (msg == 0x10) {
                PECMD_ReleaseGdiObject(*(int64_t **)(app + 0x3c), *(uint64_t *)(app + 0x3e),
                                       *(HGDIOBJ *)(app + 0x40));
                app[0x3e] = 0;
                app[0x3f] = 0;
                app[0x3c] = 0;
                app[0x3d] = 0;
                app[0x40] = 0;
                app[0x41] = 0;
                DragAcceptFiles(*(HWND *)(app + 0x28), 0);
                DragAcceptFiles(*(HWND *)(app + 0x2a), 0);
                PECMD_RestoreWindowProc((int64_t)(intptr_t)app);
                return 0;
            }
            if (msg == 0x2e) {
                uVar24 = (uint64_t)hdc >> 16;
                sVar23 = (int16_t)((uint64_t)hdc >> 16);
                if ((int16_t)hdc == 0x28) {
                    if ((int)(uVar7 & 0xffff) < (int16_t)app[0x32] + -1) {
                        uVar24 = (uint64_t)(uint16_t)(sVar23 + 1);
                    }
                }
                else if (((int16_t)hdc == 0x26) && (sVar23 != 0)) {
                    uVar24 = (uint64_t)(uint16_t)(sVar23 - 1);
                }
                return uVar24 & 0xffff;
            }
            if (msg == 0x30) {
                return 0;
            }
            if (msg != 0x100) {
                return 0;
            }
            if (hdc != (HDC)0x72) {
                goto label_028a83;
            }
        label_02a301:
            PECMD_AllocWStringBuffer((WCHAR **)&local_238, 0x1fffff);
            pHVar18 = (HWND)(uintptr_t)local_238;
            *(WCHAR *)(uintptr_t)pHVar18 = L'\0';
            GetWindowTextW(*(HWND *)(app + 0x2a), (LPWSTR)(uintptr_t)local_238, 0x1ffff9);
            *(WCHAR *)((uintptr_t)local_238 + 0xffffd * 2) = L'\0';
            uVar7 = (uint32_t)lstrlenW((LPCWSTR)(uintptr_t)pHVar18);
            local_res18[0] = 0;
            local_220[0] = 0;
            SendMessageW(*(HWND *)(app + 0x2a), 0xb0, (WPARAM)local_res18, (LPARAM)local_220);
            if (((local_res18[0] < local_220[0]) && (local_res18[0] < uVar7)) &&
                (local_220[0] <= uVar7)) {
                *(WCHAR *)((uintptr_t)pHVar18 + (uintptr_t)local_220[0] * 2) = L'\0';
                pHVar18 = (HWND)(uintptr_t)((WCHAR *)(uintptr_t)pHVar18 + local_res18[0]);
            }
            pHVar13 = pHVar18;
            if ((*(uint16_t *)(uintptr_t)pHVar18 == 0x2f) &&
                (pHVar13 = (HWND)(uintptr_t)((WCHAR *)(uintptr_t)pHVar18 + 1),
                 *(uint16_t *)(uintptr_t)pHVar13 == 0x2f)) {
                pHVar13 = (HWND)(uintptr_t)((WCHAR *)(uintptr_t)pHVar18 + 1);
            }
            sVar23 = *(uint16_t *)(uintptr_t)pHVar13;
            pHVar18 = pHVar13;
            while (((sVar23 != 0 && (sVar23 != 0xd)) && (sVar23 != 10))) {
                pHVar18 = (HWND)(uintptr_t)((WCHAR *)(uintptr_t)pHVar18 + 1);
                sVar23 = *(uint16_t *)(uintptr_t)pHVar18;
            }
            *(WCHAR *)(uintptr_t)pHVar18 = L'\0';
            if (*(uint16_t *)(uintptr_t)pHVar13 != 0) {
                if (*(uint16_t *)(uintptr_t)pHVar13 == 0x2a) {
                    *(WCHAR *)(uintptr_t)pHVar13 = 10;
                }
                iVar8 = lstrlenW((LPCWSTR)(uintptr_t)pHVar13);
                *(WCHAR *)((uintptr_t)pHVar13 + (uintptr_t)iVar8 * 2) = 0x2e;
                *(WCHAR *)((uintptr_t)pHVar13 + (uintptr_t)iVar8 * 2 + 2) = 0;
                SendMessageW(hwnd, 0x452, (WPARAM)pHVar13, 1);
            }
            ppHVar22 = (HWND *)&local_238;
        label_02a42a:
            PECMD_FreeStrBuf((WCHAR **)ppHVar22);
            return 0;
        }
    }
    else {
        if (msg == 0x111) {
        label_028a83:
            uVar5 = (uint32_t)hdc & 0xffff;
            if (uVar5 == 0x7579) {
                hwnd = *(HWND *)(app + 0x2a);
                UVar32 = 0x44d;
                goto label_02a4dc;
            }
            if ((uVar5 != 0x757a) && (uVar5 != 1)) {
                if (uVar5 == 0x7573) {
                    pHVar18 = GetDlgItem(hwnd, 0x7573);
                    LVar19 = SendMessageW(pHVar18, 0xf0, 0, 0);
                    UVar32 = 3;
                    iVar8 = 0;
                    pHVar18 = (HWND)(uintptr_t)((intptr_t)((int)LVar19 != 0) - 2);
                    iVar26 = 0;
                    goto label_02a0e6;
                }
                if (uVar5 == 0x757e) {
                    pHVar18 = GetDlgItem(hwnd, 0x757e);
                    LVar19 = SendMessageW(pHVar18, 0xf0, 0, 0);
                    uVar7 = (uint32_t)GetWindowLongW(*(HWND *)(app + 0x2a), -0x10);
                    if ((int)LVar19 == 0) {
                        DVar27 = uVar7 | 0x100080;
                    }
                    else {
                        DVar27 = uVar7 & 0xffefff7f;
                    }
                    DragAcceptFiles(*(HWND *)(app + 0x2a), 0);
                    pHVar18 = PECMD_ReplaceWithEditBox(
                        *(HWND *)(app + 0x2a), DVar27,
                        (uint64_t)(((-(uint32_t)(*(int64_t *)(app + 0x2c) != 0) & 0xffffffc0u) |
                                    0x7960u)));
                    *(HWND *)(app + 0x2a) = pHVar18;
                    *(HWND *)(app + 0x2c) = pHVar18;
                    LVar20 = SetWindowLongPtrW(pHVar18, -4, 0x140028708);
                    *(LONG_PTR *)(app + 0x3a) = LVar20;
                    DragAcceptFiles(*(HWND *)(app + 0x2a), 1);
                    return 0;
                }
                if (uVar5 == 2) {
                    PECMD_HeapFreeWithHeader((WCHAR **)(app + 0x1a));
                    if (*(HGDIOBJ *)(app + 0x36) != (HGDIOBJ)0) {
                        DeleteObject(*(HGDIOBJ *)(app + 0x36));
                    }
                    app[0x36] = 0;
                    app[0x37] = 0;
                    PECMD_RestoreWindowProc((int64_t)(intptr_t)app);
                    EndDialog(hwnd, 2);
                    return 0;
                }
                if (uVar5 == 0x7577) {
                    if (1 < (uVar7 & 0xffff) - 1) {
                        return 0;
                    }
                    local_148 = g_u64FontA98;
                    local_140 = g_u32FontAa0;
                    memset((uint64_t *)local_13c, 0, 0xf8);
                    if ((char)app[0x25] != '\0') {
                        local_148 = (local_148 & 0xffffffff00000000ULL) | (0x2551u << 16) |
                                    (uint16_t)local_148;
                    }
                    LVar19 = SendMessageW(wnd, 0x188, 0, 0);
                    extraout_EAX = (int)LVar19;
                    if ((int16_t)app[0x32] < extraout_EAX) {
                        return 0;
                    }
                    if (extraout_EAX < 0) {
                        return 0;
                    }
                    psVar12 = *(WCHAR **)(*(int64_t *)(app + 0x30) + (int64_t)extraout_EAX * 8);
                    if (*psVar12 == 0) {
                        psVar12 = (WCHAR *)&local_148;
                        SendMessageW(wnd, 0x189, (WPARAM)extraout_EAX,
                                     (LPARAM)(uintptr_t)((uint8_t *)&local_148 + 4));
                    }
                    app[0x13] = 0;
                    SendMessageW(hwnd, 0x452, (WPARAM)psVar12, 0);
                    return 0;
                }
                if (uVar5 != 0x757b) {
                    return 0;
                }
                goto label_02a301;
            }

            local_248.left = 0;
            local_248.top = 0;
            local_248.right = 0;
            local_248.bottom = 0;
            GetWindowRect(hwnd, &local_248);
            iVar8 = app[0x12];
            if (iVar8 < 0) {
                iVar8 = iVar8 + 200;
            label_02a480:
                app[0x12] = iVar8;
            }
            else {
                if ((iVar8 > 0) && (iVar8 < 100)) {
                    iVar8 = iVar8 - 100;
                    goto label_02a480;
                }
                app[0x12] = iVar8 + 100;
                if ((((local_248.bottom - app[0xf]) - app[8]) - local_248.top) + 100 <=
                    iVar8 + 100) {
                    app[0x12] = 0;
                }
            }
            pHVar18 = GetDlgItem(hwnd, 0x757a);
            puVar21 = PECMD_NewFormattedI64Str((uint64_t *)&local_168, 1, (int64_t)app[0x12]);
            SetWindowTextW(pHVar18, (LPCWSTR)(uintptr_t)*puVar21);
            PECMD_FreeStrBuf((WCHAR **)&local_168);
            UVar32 = 0x44e;
        label_02a4dc:
            PostMessageW(hwnd, UVar32, 0, 0);
            return 0;
        }

        if (msg == 0x133) {
            if (wnd != *(HWND *)(app + 0x28)) {
                return 0;
            }
            if (app[0x33] >= 0) {
                SetTextColor(hdc, (COLORREF)(uint32_t)app[0x33]);
            }
            SetBkMode(hdc, 1);
            return *(uint64_t *)(app + 0x36);
        }

        if (msg != 0x44e) {
            if (msg != 0x452) {
                return 0;
            }
            if (hdc == (HDC)0) {
                return 0;
            }
            if (*(uint16_t *)(uintptr_t)hdc == 0) {
                return 0;
            }
            FUN_1400702B0((WCHAR **)&local_208, (LPCWSTR)(uintptr_t)hdc);
            local_res18[0] = local_res18[0] & 0xffffff00u;
            iVar8 = lstrlenW((LPCWSTR)(uintptr_t)local_208);
            local_1c0 = (local_1c0 & 0xffffffff00000000ULL) | (uint32_t)iVar8;
            if ((iVar8 >= 0) &&
                (*(WCHAR *)((uintptr_t)local_208 + (uintptr_t)iVar8 * 2 - 2) == 0x2e)) {
                *(WCHAR *)((uintptr_t)local_208 + (uintptr_t)iVar8 * 2 - 2) = 0;
                local_res18[0] = (local_res18[0] & 0xffffff00u) | 1u;
            }
            pWVar10 = (LPWSTR)(*(LPCWSTR *)(app + 0x20) + app[0x13]);
            if (*pWVar10 == L'\0') {
                app[0x13] = 0;
                pWVar10 = (LPWSTR) * (LPCWSTR *)(app + 0x20);
            }
            bVar31 = app[0x13] == 0;
            iVar8 = lstrlenW((LPCWSTR)(uintptr_t)local_208);
            do {
                pWVar15 = (LPWSTR)(uintptr_t)StrStrW(pWVar10, (LPCWSTR)(uintptr_t)local_208);
                iVar26 = iVar8;
                if (pWVar15 == NULL) {
                    iVar26 = 1;
                    if (*pWVar10 != L'\0') {
                        iVar6 = (int)local_1c0;
                        uVar24 = 1;
                        do {
                            uVar16 = PECMD_ComparePrefixNoCaseLen(pWVar10,
                                                                  (LPCWSTR)(uintptr_t)local_208);
                            iVar26 = (int)uVar16;
                            if (((int)uVar24 <= iVar26) &&
                                (uVar24 = uVar16 & 0xffffffffu, pWVar15 = (LPWSTR)pWVar10,
                                 iVar6 <= iVar26)) {
                                break;
                            }
                            iVar26 = (int)uVar24;
                            pWVar10 = pWVar10 + 1;
                        } while (*pWVar10 != L'\0');
                    }
                    pWVar10 = (LPWSTR) * (LPCWSTR *)(app + 0x20);
                    if (pWVar15 != NULL) {
                        goto label_029c5c;
                    }
                }
                else {
                label_029c5c:
                    app[0x13] = (int)(((intptr_t)pWVar15 - *(intptr_t *)(app + 0x20)) >> 1) + 1;
                    if ((pWVar15 != NULL) && ((bVar31 || (iVar8 <= iVar26)))) {
                        break;
                    }
                }
                app[0x13] = 0;
                bVar4 = !bVar31;
                bVar31 = true;
            } while (bVar4);

            if (pWVar15 != NULL) {
                pWVar3 = *(LPWSTR *)(app + 0x20);
                iVar8 = (int)(((intptr_t)pWVar15 - (intptr_t)pWVar3) >> 1);
                if (*pWVar15 == L'\n') {
                    iVar8 = iVar8 + 1;
                    iVar26 = iVar26 - 1;
                }
                if ((char)local_res18[0] == '\0') {
                    for (; pWVar3 < pWVar15; pWVar15 = pWVar15 - 1) {
                        if ((*pWVar15 == L'\n') &&
                            ((((WVar2 = pWVar15[1], WVar2 == L'\u2523' || (WVar2 == L'\u2560')) ||
                               (WVar2 == L'\u250f')) ||
                              (WVar2 == L'\u2554')))) {
                            pWVar15 = pWVar15 + 1;
                            break;
                        }
                    }
                }
                LVar19 = SendMessageW(
                    *(HWND *)(app + 0x28), 0xc9,
                    (WPARAM)(intptr_t)(((intptr_t)pWVar15 - (intptr_t)pWVar3) >> 1), 0);
                pHVar18 = GetDlgItem(hwnd, (uint32_t)*(uint16_t *)((uint8_t *)app + 0x112));
                LVar17 = SendMessageW(pHVar18, 0xce, 0, 0);
                iVar6 = (int)LVar17;
                if ((int)LVar17 < 0) {
                    iVar6 = 0;
                }
                PostMessageW(pHVar18, 0xb6, 0, (LPARAM)(intptr_t)((int)LVar19 - iVar6));
                PostMessageW(pHVar18, 0x44c, 0, 0);
                if (wnd != (HWND)0) {
                    SendMessageW(pHVar18, 0xb1, (WPARAM)(intptr_t)iVar8,
                                 (LPARAM)(intptr_t)(iVar8 + iVar26));
                }
            }
            ppHVar22 = (HWND *)&local_208;
            goto label_02a42a;
        }
    }

    /* WM_44E: 重新布局 */
    local_248.left = 0;
    local_248.top = 0;
    local_248.right = 0;
    local_248.bottom = 0;
    GetWindowRect(hwnd, &local_248);
    iVar26 = local_248.right - local_248.left;
    iVar8 = local_248.bottom - local_248.top;
    iVar6 = *app;
    local_200 = GetDlgItem(hwnd, (uint32_t)*(uint16_t *)((uint8_t *)app + 0x112));
    local_1d8 = GetDlgItem(hwnd, 0x7579);
    local_1b0 = GetDlgItem(hwnd, 0x757a);
    local_208 = GetDlgItem(hwnd, 0x757e);
    local_1b8 = GetDlgItem(hwnd, 0x7573);
    local_1a0 = GetDlgItem(hwnd, 0x7577);
    local_238 = GetDlgItem(hwnd, 0x757b);
    iVar30 = app[0x12];
    local_res18[0] = iVar26 - app[0xd];
    iVar29 = iVar30 - app[0xc];
    if (iVar30 < 1) {
        iVar30 = 0;
        iVar29 = -app[0xc];
    }
    SetWindowPos(local_1a0, (HWND)0, iVar26 - app[6], app[8], app[4], (iVar8 - app[1]) + 3, 4);
    SetWindowPos(local_200, (HWND)0, 0, app[8], iVar26 - iVar6, (iVar8 - iVar29) - app[1], 6);
    iVar29 = (iVar8 - app[0xb]) - iVar29;
    if (iVar29 < app[8]) {
        iVar6 = app[8] - iVar29;
        iVar29 = iVar29 + iVar6;
        iVar30 = iVar30 - iVar6;
    }
    SetWindowPos(*(HWND *)(app + 0x2a), (HWND)0, app[10], iVar29, local_res18[0], iVar30, 4);
    iVar26 = iVar26 - app[0x10];
    iVar8 = iVar8 - app[0x11];
    SetWindowPos(local_1d8, (HWND)0, iVar26, iVar8, 0, 0, 5);
    SetWindowPos(local_1b0, (HWND)0, iVar26 + app[0x14], iVar8 + app[0x15], 0, 0, 5);
    SetWindowPos(local_208, (HWND)0, iVar26 + app[0x16], iVar8 + app[0x15], 0, 0, 5);
    SetWindowPos(local_1b8, (HWND)0, iVar26 + app[0x16], iVar8 + app[0x17], 0, 0, 5);
    iVar8 = iVar8 + app[0x19];
    iVar26 = iVar26 + app[0x18];
    UVar32 = 5;
    pHVar18 = (HWND)0;
    hwnd = local_238;
label_02a0e6:
    SetWindowPos(hwnd, pHVar18, iVar26, iVar8, 0, 0, UVar32);
    return 0;
}

/* ====================================================================
 * b2f_part2.c — B2F 批次三个函数 (standalone syntax-check target)
 *
 * 来源: PECMD原始.EXE (x64, ImageBase=0x140000000)
 *   FUN_14002D708       @0x14002d708
 *   PECMD_FontCommand         @0x14002f454
 *   PECMD_EvalLoopCondition @0x140032dc4
 *
 * 约定:
 *   - 新实现函数使用 PECMD_ 可读名；原始地址保留在 @0x 注释。
 *   - 未实现外部依赖保留 extern FUN_<addr> 声明并加 TODO(verify)。
 *   - Ghidra 扩展宏已用等价整数/位运算替换。
 *   - pthreadmbcinfo/pthreadlocinfo 按 WCHAR* 语义映射。
 * ==================================================================== */
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <math.h>

#include "pecmd_defs.h"

#ifndef B2F_PART2_LOCAL
#define B2F_PART2_LOCAL

/* ---- win32_stub.h 缺失的补充类型/API ---- */
typedef struct tagPROCESSENTRY32W {
    DWORD dwSize;
    DWORD cntUsage;
    DWORD th32ProcessID;
    uintptr_t th32DefaultHeapID;
    DWORD th32ModuleID;
    DWORD cntThreads;
    DWORD th32ParentProcessID;
    LONG pcPriClassBase;
    DWORD dwFlags;
    WCHAR szExeFile[MAX_PATH];
} PROCESSENTRY32W;

typedef struct _PROCESS_MEMORY_COUNTERS {
    DWORD cb;
    DWORD PageFaultCount;
    uint64_t PeakWorkingSetSize;
    uint64_t WorkingSetSize;
    uint64_t QuotaPeakPagedPoolUsage;
    uint64_t QuotaPagedPoolUsage;
    uint64_t QuotaPeakNonPagedPoolUsage;
    uint64_t QuotaNonPagedPoolUsage;
    uint64_t PagefileUsage;
    uint64_t PeakPagefileUsage;
} PROCESS_MEMORY_COUNTERS;

extern HANDLE CreateToolhelp32Snapshot(DWORD, DWORD);
extern BOOL Process32FirstW(HANDLE, PROCESSENTRY32W *);
extern BOOL Process32NextW(HANDLE, PROCESSENTRY32W *);
extern BOOL GetProcessTimes(HANDLE, FILETIME *, FILETIME *, FILETIME *, FILETIME *);
extern HANDLE AddFontMemResourceEx(LPVOID, DWORD, LPVOID, DWORD *);
extern BOOL RemoveFontMemResourceEx(HANDLE);


#endif /* B2F_PART2_LOCAL */

/* ---- 未实现外部依赖 (extern) ---- */
extern LPCWSTR PECMD_FindLastPathSep(LPCWSTR);
extern void *FUN_1400F429C(WCHAR **pp, WCHAR ch);
extern int64_t FUN_14001C2CC(LPCWSTR priv, DWORD attr, uint32_t flag);
extern void PECMD_FormatI64Dec(LPWSTR dst, int64_t value);
extern void PECMD_GetProcessorCount(void);
extern void PECMD_StrAppendFormat(int64_t *ps, double a2, LPCWSTR fmt);
extern int64_t PECMD_WideStrLen(const void *p);
extern int PECMD_CmpStrN(LPCWSTR s1, LPCWSTR s2, int n, uint32_t case_sens);
extern int PECMD_IsProcessUser(LPCWSTR s, DWORD pid, uint32_t case_sens);
extern void PECMD_TerminateProcessById(DWORD pid, UINT hwnd, void *msg);
extern void PECMD_GetProcessModuleFile(DWORD pid, WCHAR *path);
extern void PECMD_GetProcessUserName(DWORD pid, WCHAR *buf);
extern void PECMD_ReadProcessCommandLine(DWORD pid, void *buf, uint64_t size);
extern void PECMD_ParseNumAdvance(int64_t *pp, uint64_t *out);
extern void PECMD_CrtShim(WCHAR *dst, uint64_t id, LPCWSTR src, WCHAR *extra);
extern int PECMD_InstallFonts(WCHAR *s, int hwnd);
extern void PECMD_AllocSmallObject(void *pp);
extern LPCWSTR FUN_140079F50(WCHAR **pp, int mode);
extern uint64_t PECMD_LoadFileToSlot(LPCWSTR path, int64_t *pp);
extern LPCWSTR PECMD_ParseItemImageSpec(int64_t *pp, int64_t *out, const WCHAR *kind);
extern HICON PECMD_LoadIcon(LPCWSTR p, uint64_t *a2);
extern void FUN_1400633A8(void *pp, int64_t len);
extern void PECMD_ParseNumSkipWs(WCHAR **pp, void *out);
extern void PECMD_AppendFmtValue(void *script, uint64_t value, LPCWSTR key, LPCWSTR fmt);
extern void FUN_1400629B8(void *a1, LPCWSTR key, LPCWSTR a2);
extern uint64_t FUN_140024C48(void *pp, int64_t *a2, int mode);
extern void FUN_14006F884(LPCWSTR s, ULARGE_INTEGER *out);
extern WCHAR *FUN_14001E69C(int64_t *a1, LPCWSTR s, int64_t *a3, int a4);
extern int64_t FUN_14007A224(int64_t *a1, WCHAR *s, ULARGE_INTEGER *out, int a4, uint8_t a5);
extern WCHAR *PECMD_ResolveScriptVariable(int64_t *a1, LPCWSTR s, ULARGE_INTEGER *out);
extern LPCWSTR PECMD_StripTrailingSpaces(LPCWSTR s);
extern void PECMD_SkipUntilDelim(void *pp, WCHAR a, WCHAR b);
extern uint64_t PECMD_IsRemovableDrive(uint32_t c);
extern uint32_t PECMD_FindFileOrDir(LPCWSTR s, int mode);
extern void PECMD_ParseParenthesizedExpression(int64_t *pp, double *out);
extern uint64_t PECMD_EvalParenStripped(void *pp, void *out);
extern uint64_t PECMD_ParseAndSkipSpace_7f90(int64_t *pp, double *out);
extern void FUN_14006A7F4(int64_t *pp, ULARGE_INTEGER *out);
extern uint64_t PECMD_QueryPhysicalMemory(int mode);
extern uint64_t PECMD_WriteNumberToScriptVar(LPCWSTR s, ULARGE_INTEGER v, int64_t *a1);
extern uint32_t PECMD_ParseVkKeyName(LPCWSTR s, char c);
extern ULARGE_INTEGER PECMD_GetDiskSpaceInfo(WCHAR *s, int a2, int a3);
extern uint64_t PECMD_CalcExpression(int64_t *a1, WCHAR *s, void *a3);

/* ---- 本批引用的全局数据 ---- */
extern WCHAR g_szEmpty[]; /* g_szEmpty */
extern uint8_t g_u8CA49;  /* 版本/标志字节 */
extern double g_dbl1178;
extern double g_dbl1668;
extern double g_dbl21630;
extern void *g_pNtQuerySystemInformation;     /* NtQuerySystemInformation */
extern void *g_pGetProcessMemoryInfo;         /* GetProcessMemoryInfo */
extern void *g_pGdipNewPrivateFontCollection; /* 字体枚举函数指针 */
extern void *g_pGdipPrivateAddMemoryFont;
extern void *g_pGdipGetFontCollectionFamilyList;
extern void *g_pGdipGetFamilyName;
extern void *g_pGdipDeletePrivateFontCollection;

/* FUN_14002D708 is defined below and used by PECMD_EvalLoopCondition. */
DWORD FUN_14002D708(LPCWSTR filter, uint32_t mode, int64_t *out, DWORD pid, DWORD parentPid);

/* ========== FUN_14002D708 @0x14002d708 ==========
 * 原 FUN_14002D708。枚举进程/输出进程信息。
 * TODO(verify): Ghidra 对 Process32FirstW/NextW 漏了第二个参数，
 *   这里按真实 Win32 API 补上 PROCESSENTRY32W*。
 */
DWORD FUN_14002D708(LPCWSTR filter, uint32_t mode, int64_t *out, DWORD pid, DWORD parentPid)
{
    WCHAR *pWVar19 = NULL;
    LPCWSTR local_438;
    WCHAR *local_3c0 = NULL;
    WCHAR *local_400;
    WCHAR *local_3f8;
    WCHAR *local_430;
    WCHAR *local_3b0;
    WCHAR *local_398;
    WCHAR *local_3e0;
    WCHAR *local_3a0;
    int64_t local_440 = 0;
    uint8_t local_2c8[8];
    HANDLE hProcess = 0;
    LPCWSTR lpString1 = NULL;
    WCHAR *pWVar1 = NULL;
    uint64_t uVar11 = 0;
    PROCESSENTRY32W pe32;
    PROCESS_MEMORY_COUNTERS pmc;
    FILETIME ftCreate[2];
    FILETIME ftExit;
    FILETIME ftKernel;
    FILETIME ftUser;

    (void)pWVar19;

    if (((mode == 0) && (out == NULL)) && (pid == 0) && (parentPid == 0)) {
        filter = PECMD_FindLastPathSep(filter);
    }
    local_438 = filter;
    FUN_1400F429C((WCHAR **)&local_438, L'|');
    {
        WCHAR WVar18 = (WCHAR)(uintptr_t)pWVar19;
        if (*local_438 != WVar18) {
            *(WCHAR *)local_438 = WVar18;
            local_438 = local_438 + 1;
            if (*local_438 != WVar18)
                goto LAB_14002d791;
        }
        local_438 = NULL;
    }
LAB_14002d791: {
    UINT local_410 = (UINT)(uintptr_t)pWVar19;
    uint32_t uVar8 = mode & 0x10000;
    uint32_t uVar13 = mode & 0x40;
    int iVar3 = (mode & 0x80) != 0; /* 原 CONCAT71 宏简化 */
    uint32_t local_41c = mode & 0x80000;
    uint32_t local_420 = uVar13;
    DWORD local_40c = GetCurrentProcessId();
    uint64_t local_3c8 = 0x400;
    uint64_t local_428 = 0;
    DWORD *local_3d8 = NULL;
    DWORD *local_418 = NULL;

    if (iVar3 != 0) {
        local_410 = (UINT)(uintptr_t)out;
        out = NULL;
    }
    if ((uVar13 != 0) && (iVar3 != 0)) {
        PECMD_GrowByteBuffer((void **)&local_418, 0x1000);
        local_3d8 = local_418;
        out = NULL;
    }
    uint8_t local_res10 = uVar13 != 0;
    if (((uVar13 != 0) && (iVar3 != 0)) && (parentPid != 0)) {
        *local_3d8 = parentPid;
        local_428 = 1;
        local_res10 = 2;
    }
    local_418 = local_3d8;

    if (g_pNtQuerySystemInformation == NULL) {
        HMODULE hModule = GetModuleHandleA("ntdll");
        g_pNtQuerySystemInformation = GetProcAddress(hModule, "NtQuerySystemInformation");
    }
    FUN_14001C2CC(WSTR("SeDebugPrivilege"), 2, 4);
    DWORD DVar4 = 0;
    PECMD_AllocWStringBuffer((WCHAR **)&local_3c0, 0x48f3);
    local_400 = local_3c0;
    local_3f8 = local_3c0 + 0x50;
    local_430 = local_3c0 + 0x82;
    local_3b0 = local_3c0 + 0x7dd;
    *local_3b0 = L'\t';
    local_398 = local_3c0 + 0x7de;
    *local_398 = L'\0';
    local_3e0 = local_3c0 + 0x8e3;
    *local_3e0 = L'\0';
    local_3c0[0x5d3] = L'\0';
    SHGetSpecialFolderPathW((HWND)0, local_3c0 + 0x5d3, 0x24, 0);
    {
        int iVar9 = lstrlenW(local_430 + 0x551);
        local_3a0 = local_430 + (int64_t)iVar9 + 0x551;
    }
    if (out != NULL) {
        uint8_t local_374[28];
        uint64_t local_190[42];

        memset(local_374, 0, 0x1c);
        memset(local_2c8 + 4, 0, 0x134);
        memset(local_190, 0, 0x138);
        memset(local_2c8, 0, 8);
        ((void (*)(int, void *, unsigned long))g_pNtQuerySystemInformation)(3, &local_374, 0x20);
        ((void (*)(int, void *, unsigned long))g_pNtQuerySystemInformation)(2, local_2c8, 0x134);
        /* TODO(verify): 原反编译只显示 2 个实参 */
        PECMD_FormatI64Dec(local_400, *(uint64_t *)local_2c8);
        PECMD_FormatI64Dec(local_3f8, *(uint64_t *)(local_374 + 4));
    }
    DWORD DVar15 = 0;
    if (((mode & 1) == 0) && (*filter == L'\0')) {
        FUN_14007034C(out, local_400);
        FUN_14006375C((WCHAR **)out, WSTR("\t"));
        FUN_14006375C((WCHAR **)out, local_3f8);
        if (g_u8CA49 == '\0') {
            PECMD_GetProcessorCount();
        }
        {
            WCHAR local_348[32];
            wsprintfW(local_348, WSTR("\t%d\t"), (unsigned long long)(uint8_t)g_u8CA49);
            FUN_14006375C((WCHAR **)out, local_348);
            PECMD_FormatI64Dec(local_348, (uint64_t)g_QPFreq);
            FUN_14006375C((WCHAR **)out, local_348);
            {
                double dVar2 = g_dbl1178 / (double)g_QPFreq;
                PECMD_AllocStrSlot(&local_440);
                PECMD_StrAppendFormat((int64_t *)&local_440, dVar2, WSTR("\t%lf"));
                FUN_14006375C((WCHAR **)out, (LPCWSTR)(uintptr_t)local_440);
                PECMD_FreeStrBuf((WCHAR **)&local_440);
            }
        }
    }
    local_440 = PECMD_WideStrLen((void *)filter);
    {
        HANDLE local_408 = CreateToolhelp32Snapshot(2, parentPid);
        if (local_408 == (HANDLE)0xffffffffffffffffULL) {
            local_408 = (HANDLE)0;
        }
        if (local_408 != (HANDLE)0xffffffffffffffffULL) {
            int64_t local_3e8 = (int64_t)(int)(mode & 0x20);
            int64_t local_3f0 = (int64_t)(int)(mode & 1);
            int64_t local_3a8 = (int64_t)(int)(mode & 0x100);
            int64_t local_3b8 = (int64_t)(int)(mode & 2);
            int64_t local_3d0 = (int64_t)(int)(mode & 0x10);
            uint64_t uVar20 = local_428;
            DWORD *pDVar21 = local_3d8;
            do {
                int64_t lVar16 = local_3f0;
                pe32.dwSize = 0x238;
                local_428 = uVar20;
                int iVar9 = Process32FirstW(local_408, &pe32);
                HANDLE pvVar6 = local_408;
                uVar11 = uVar20;
                int64_t lVar17 = local_3e8;
                DWORD DVar5 = DVar15;
                DWORD DVar14 = DVar4;
                while (iVar9 != 0) {
                    uVar11 = 0;
                    if (local_res10 == 2) {
                        uint64_t uVar12 = 0;
                        if (uVar20 != 0) {
                            do {
                                if (pDVar21[uVar12] == pe32.th32ProcessID)
                                    goto LAB_14002e2de;
                                uVar12 = uVar12 + 1;
                            } while (uVar12 < uVar20);
                        }
                        if (uVar20 != 0) {
                            do {
                                if (pDVar21[uVar11] == pe32.th32ParentProcessID)
                                    goto LAB_14002dfb4;
                                uVar11 = uVar11 + 1;
                            } while (uVar11 < uVar20);
                        }
                    }
                    else if (((pid == 0) || (pid == pe32.th32ParentProcessID)) &&
                             ((parentPid == 0) || (parentPid == pe32.th32ProcessID))) {
                        if (lVar17 == 0) {
                            if (local_3a8 != 0) {
                                BOOL BVar10 = PathMatchSpecW(pe32.szExeFile, filter);
                                if (BVar10 != 0)
                                    goto LAB_14002dfb4;
                                goto LAB_14002dcd5;
                            }
                            iVar9 = (int)local_440;
                            if (local_3b8 != 0) {
                                if (lVar16 == 0) {
                                    if ((int)local_440 == 0) {
                                        if (pe32.szExeFile[0] == L'\0')
                                            goto LAB_14002dcd5;
                                    }
                                    else {
                                        if (uVar8 == 0) {
                                            iVar9 = lstrcmpiW(pe32.szExeFile, filter);
                                        }
                                        else {
                                            iVar9 = lstrcmpW(pe32.szExeFile, filter);
                                        }
                                        if (iVar9 == 0)
                                            goto LAB_14002dfb4;
                                    }
                                }
                                else if (((int)local_440 == 0) ||
                                         PECMD_CmpStrN(pe32.szExeFile, filter, (int)local_440,
                                                       uVar8)) {
                                    goto LAB_14002dfb4;
                                }
                                goto LAB_14002e2de;
                            }
                            if ((lVar16 == 0) ||
                                (((int)local_440 != 0 &&
                                  !PECMD_CmpStrN(pe32.szExeFile, filter, (int)local_440, uVar8)))) {
                                if ((iVar9 != 0) &&
                                    PECMD_CmpStrN(pe32.szExeFile, filter, iVar9, uVar8) &&
                                    (local_3f0 == 0)) {
                                    if ((pe32.szExeFile[(int)local_440] == L'\0') ||
                                        (pe32.szExeFile[(int)local_440] == L'.'))
                                        goto LAB_14002dfb4;
                                }
                                goto LAB_14002e2de;
                            }
                        LAB_14002dfb4:
                            if ((((out == NULL) && (local_3d0 != 0)) &&
                                 (local_40c == pe32.th32ProcessID)) ||
                                ((local_438 != NULL) &&
                                 (PECMD_IsProcessUser(local_438, pe32.th32ProcessID, uVar8) ==
                                  0))) {
                                goto LAB_14002e2de;
                            }
                            if (out == NULL) {
                                uVar11 = local_428;
                                DVar4 = pe32.th32ProcessID;
                                DVar15 = pe32.th32ProcessID;
                                if (iVar3 != 0 || local_420 != 0) {
                                    if (pDVar21 != NULL) {
                                        if (local_3c8 <= uVar20) {
                                            local_3c8 = local_3c8 + 0x400;
                                            PECMD_GrowByteBuffer((void **)&local_418,
                                                                 local_3c8 * 4);
                                        }
                                        local_418[uVar20] = pe32.th32ProcessID;
                                        uVar20 = uVar20 + 1;
                                        pDVar21 = local_418;
                                    }
                                    if (iVar3 != 0) {
                                        PECMD_TerminateProcessById(pe32.th32ProcessID, local_410,
                                                                   NULL);
                                    }
                                    DVar14 = DVar5 + 1;
                                    goto LAB_14002e2de;
                                }
                                break;
                            }
                            DVar14 = DVar5 + 1;
                            lVar17 = local_3e8;
                        }
                        else {
                        LAB_14002dcd5:
                            if ((local_438 != NULL) &&
                                (PECMD_IsProcessUser(local_438, pe32.th32ProcessID, uVar8) == 0)) {
                                goto LAB_14002e2de;
                            }
                        }
                        lVar16 = 0;
                        if ((out != NULL) || (lVar17 != 0)) {
                            if ((out != NULL) && (hProcess = OpenProcess(
                                                      0x400, 0, pe32.th32ProcessID)) != (HANDLE)0) {
                                PECMD_GetApiProcCached("GetProcessMemoryInfo", "PSAPI.DLL",
                                                       (int64_t *)(void **)&g_pGetProcessMemoryInfo,
                                                       (int64_t *)0);
                                pmc.cb = 0x48;
                                pmc.PageFaultCount = 0;
                                memset((uint8_t *)&pmc + 8, 0, 0x40);
                                if (g_pGetProcessMemoryInfo != NULL) {
                                    ((void (*)(HANDLE, void *, DWORD))g_pGetProcessMemoryInfo)(
                                        hProcess, &pmc, 0x48);
                                }
                                BOOL BVar10 = GetProcessTimes(hProcess, &ftCreate[0], &ftExit,
                                                              &ftKernel, &ftUser);
                                if (BVar10 != 0) {
                                    lVar16 = (int64_t)((uint64_t)ftKernel.dwLowDateTime |
                                                       ((uint64_t)ftKernel.dwHighDateTime << 32)) +
                                             (int64_t)((uint64_t)ftUser.dwLowDateTime |
                                                       ((uint64_t)ftUser.dwHighDateTime << 32));
                                }
                                CloseHandle(hProcess);
                            }
                            lpString1 = local_430 + 0x656;
                            PECMD_GetProcessModuleFile(pe32.th32ProcessID, local_430 + 0x656);
                            if ((((local_430[0x656] == L'\\') && (local_430[0x657] == L'?')) &&
                                 (local_430[0x658] == L'?')) &&
                                (((local_430[0x659] == L'\\' && (local_430[0x65a] != L'\0')) &&
                                  (local_430[0x65b] == L':')))) {
                                lpString1 = local_430 + 0x65a;
                            }
                            else {
                                iVar9 = StrCmpNIW(WSTR("\\SystemRoot\\"), local_430 + 0x656, 0xc);
                                if (iVar9 == 0) {
                                    lpString1 = local_430 + 0x551;
                                    lstrcpyW(local_3a0, local_430 + 0x661);
                                }
                            }
                            if (local_3e8 == 0) {
                            LAB_14002e1cd:
                                if (local_41c != 0) {
                                    PECMD_GetProcessUserName(pe32.th32ProcessID, local_398);
                                }
                                PECMD_ReadProcessCommandLine(pe32.th32ProcessID, local_3e0, 0x8000);
                                local_3e0[0x4000] = L'\0';
                                PECMD_FormatI64Dec(local_430, (uint64_t)lVar16);
                                pWVar1 = local_430 + 0x46;
                                wsprintfW(pWVar1, WSTR("%lu\t%lu\t%lu\t%s\t%s%s\t"));
                                /* TODO(verify): 原反编译缺少 vararg 实参 */
                                if (*(uint16_t *)(uintptr_t)*out != 0) {
                                    FUN_14006375C((WCHAR **)out, WSTR("\r\n"));
                                }
                                FUN_14006375C((WCHAR **)out, pWVar1);
                                FUN_14006375C((WCHAR **)out, lpString1);
                                FUN_14006375C((WCHAR **)out, WSTR("\t"));
                                FUN_14006375C((WCHAR **)out, local_3e0);
                            }
                            else {
                                int cmp = 0;
                                if (uVar8 == 0) {
                                    iVar9 = lstrcmpiW(lpString1, filter);
                                    cmp = (iVar9 == 0);
                                }
                                else {
                                    iVar9 = lstrcmpW(lpString1, filter);
                                    cmp = (iVar9 == 0);
                                }
                                if (cmp) {
                                    if (out != NULL) {
                                        DVar14 = DVar14 + 1;
                                        goto LAB_14002e1cd;
                                    }
                                    uVar11 = local_428;
                                    DVar4 = pe32.th32ProcessID;
                                    DVar15 = pe32.th32ProcessID;
                                    if ((local_3d0 == 0) || (local_40c != pe32.th32ProcessID))
                                        break;
                                }
                            }
                        }
                    }
                LAB_14002e2de:
                    iVar9 = Process32NextW(pvVar6, &pe32);
                    uVar11 = local_428;
                    lVar16 = local_3f0;
                    lVar17 = local_3e8;
                    DVar5 = DVar14;
                }
                local_res10 = 2;
            } while ((local_420 != 0) && (uVar11 < uVar20));
        }
        if ((local_408 != (HANDLE)0) && (local_408 != (HANDLE)0xffffffffffffffffULL)) {
            CloseHandle(local_408);
        }
    }
    PECMD_FreeStrBuf((WCHAR **)&local_3c0);
    PECMD_FreeStrBuf((WCHAR **)&local_418);
    return DVar15;
}
}

/* ========== PECMD_FontCommand @0x14002f454 ==========
 * 原 PECMD_FontCommand。安装/枚举字体。
 * pthreadmbcinfo/pthreadlocinfo 均按 WCHAR* 游标映射。
 * TODO(verify): 若干 Ghidra 类型误标已按 WCHAR 指针语义重写。
 */
HMODULE PECMD_FontCommand(int64_t *fontList, WCHAR *spec, uint64_t unused, WCHAR *name)
{
    (void)unused;

    HMODULE pHVar22 = 0;
    int iVar21 = 0;
    int iVar8 = 0;
    WCHAR *local_60 = NULL;
    WCHAR *local_78 = NULL;
    WCHAR *local_b8 = NULL;
    WCHAR *local_res10 = spec;
    WCHAR *local_b0 = NULL;
    WCHAR *local_a0 = NULL;
    uint64_t local_58 = 0;
    uint64_t uVar20 = 2;
    uint64_t local_res18 = 0;
    DWORD DVar7 = 2;
    HANDLE local_98 = 0;
    LPVOID local_90 = NULL;
    uint64_t local_88 = 0;
    uint64_t local_80 = 0;
    LPCWSTR local_68 = NULL;
    void *local_70 = NULL;
    LPCWSTR local_50[2] = {NULL, NULL};
    DWORD local_res20[2] = {0, 0};
    (void)local_80;
    WCHAR *pWVar12 = NULL;
    WCHAR *pWVar13 = NULL;
    LPWSTR pWVar9 = NULL;

    PECMD_AllocStrSlot(&local_b0);
    if (*(int16_t *)local_res10 == 0x2d) {
        HMODULE pHVar15 = 0;
        HMODULE pHVar19 = 0;
        do {
            char cVar3 = (char)FUN_1400660AC("-", (int64_t *)&local_res10, -1);
            if (cVar3 == 0) {
                cVar3 = (char)FUN_1400660AC("-p", (int64_t *)&local_res10, -1);
                if (cVar3 == 0) {
                    int uVar10 = FUN_14005C788("p:", (const WCHAR *)(local_res10 + 1), 2);
                    if (uVar10 != 0) {
                        pHVar15 = (HMODULE)3;
                        local_b8 = local_res10 + 2;
                        uint16_t uVar2 = *(uint16_t *)local_b8;
                        local_res10 = local_b8;
                        while (uVar2 != 0) {
                            if (((8 < uVar2) && (uVar2 < 0xe)) || (uVar2 == 0x20)) {
                                if (*(int16_t *)local_res10 != 0) {
                                    *(uint16_t *)local_res10 = 0;
                                    local_res10 = local_res10 + 1;
                                }
                                break;
                            }
                            local_res10 = local_res10 + 1;
                            uVar2 = *(uint16_t *)local_res10;
                        }
                        {
                            int16_t sVar18 = 0x3a;
                            local_60 = local_b8;
                            FUN_1400F429C(&local_b8, L':');
                            if (*(int16_t *)local_b8 != 0) {
                                *(uint16_t *)local_b8 = 0;
                                local_b8 = local_b8 + 1;
                                if ((*(int16_t *)local_b8 != 0) &&
                                    (local_78 = local_b8, FUN_1400F429C(&local_b8, (WCHAR)sVar18),
                                     *(int16_t *)local_b8 != 0)) {
                                    *(uint16_t *)local_b8 = 0;
                                    local_b8 = local_b8 + 1;
                                }
                            }
                        }
                        PECMD_ParseNumAdvance((int64_t *)&local_b8, &local_58);
                        if (((*(uint16_t *)local_res10 < 9) || (0xd < *(uint16_t *)local_res10)) &&
                            (*(int16_t *)local_res10 != 0x20)) {
                            goto LAB_14002f66a;
                        }
                    }
                    {
                        uint16_t uVar2;
                        for (; (uVar2 = *(uint16_t *)local_res10,
                               uVar2 != 0 && (((uVar2 < 9 || (0xd < uVar2)) && (uVar2 != 0x20))));
                             local_res10 = local_res10 + 1) {
                        }
                    }
                    PECMD_SkipLeadingControls(&local_res10);
                }
                else {
                    pHVar15 = (HMODULE)1;
                }
            }
            else {
                pHVar19 = (HMODULE)1;
            }
        LAB_14002f66a:
            iVar21 = (int)pHVar19;
            iVar8 = (int)pHVar15;
        } while (*(int16_t *)local_res10 == 0x2d);
    }

    {
        int iVar5 = lstrlenW(local_res10);
        PECMD_AllocWStringBuffer((WCHAR **)&local_a0, (int64_t)(iVar5 + 0x3a9));
        local_98 = 0;
        WCHAR *lpBuffer = local_a0 + (iVar5 + 0x84);

        if ((iVar8 == 0) && (*(int16_t *)local_res10 == 0x5c)) {
            local_res10 = local_res10 + 1;
            {
                LPWSTR pWVar9 = StrRChrW(local_res10, NULL, L',');
                WCHAR WVar23 = L'\0';
                if (pWVar9 != NULL) {
                    LPWSTR lpsz = pWVar9 + 1;
                    *pWVar9 = L'\0';
                    CharUpperW(lpsz);
                    if (*lpsz != L'\0') {
                        WVar23 = *lpsz;
                    }
                }
                local_res10 = FUN_14001BE14(local_res10);
                GetLogicalDriveStringsW(0x324, lpBuffer);
                WCHAR WVar1 = *lpBuffer;
                DVar7 = 2;
                while (WVar1 != L'\0') {
                    if ((uint16_t)WVar23 <= (uint16_t)WVar1) {
                        PECMD_CrtShim(local_a0, 0x1401214f8, lpBuffer, local_res10);
                        uint64_t uVar10 = PECMD_IsDirectory(local_a0);
                        if ((int)uVar10 != 0) {
                            wcscat(local_a0, WSTR("\\*.*"));
                            int bVar4 = PECMD_InstallFonts(local_a0, iVar21);
                            uVar20 = (uint64_t)(uint32_t)bVar4;
                        }
                    }
                    DVar7 = (DWORD)uVar20;
                    {
                        int64_t lVar11 = PECMD_WideStrLen(lpBuffer);
                        lpBuffer = lpBuffer + (int)lVar11 + 1;
                    }
                    WVar1 = *lpBuffer;
                }
            }
            goto LAB_14002fc1a;
        }

        local_res10 = FUN_14001BE14(local_res10);
        if (iVar8 == 0) {
            uint64_t uVar10 = PECMD_IsDirectory(local_res10);
            if ((int)uVar10 != 0) {
                PECMD_StrBldCopyWide((WCHAR **)&local_res18, local_res10);
                FUN_14006375C((WCHAR **)&local_res18, WSTR("\\*.*"));
                local_res10 = (WCHAR *)local_res18;
            }
            PECMD_CrtShim(local_a0, 0x140120784, local_res10, name);
            {
                pWVar9 = StrRChrW(local_res10, NULL, L'\\');
                if (pWVar9 != NULL) {
                    *pWVar9 = L'\0';
                }
            }
            uVar10 = PECMD_IsDirectory(local_res10);
            if ((int)uVar10 != 0) {
                int bVar4 = PECMD_InstallFonts(local_a0, iVar21);
                DVar7 = (DWORD)(uint32_t)bVar4;
            }
            PECMD_FreeStrBuf((WCHAR **)&local_res18);
        }
        else {
            if (iVar21 == 0) {
                PECMD_AllocSmallObject((void **)&local_90);
                local_88 = 0;
                local_80 = 0;
                local_68 = NULL;
                {
                    pWVar12 = (WCHAR *)FUN_140079F50(&local_res10, 1);
                    WCHAR *lpLibFileName = local_res10;
                    uint64_t uVar16;
                    if ((pWVar12 == NULL) || (*pWVar12 != L'#')) {
                        local_50[0] = pWVar12;
                        uVar16 = PECMD_LoadFileToSlot(local_res10, (int64_t *)&local_90);
                    }
                    else {
                        pWVar13 = NULL;
                        HMODULE pHVar14;
                        HMODULE pHVar15;
                        HMODULE pHVar19 = 0;
                        WCHAR WVar23;
                        local_50[0] = pWVar12 + 1;
                        pWVar13 = (WCHAR *)PECMD_ParseItemImageSpec(
                            (int64_t *)local_50, (int64_t *)&local_68, WSTR("FONT"));
                        pHVar15 = g_hInst;
                        WVar23 = *pWVar12;
                        *pWVar12 = L'\0';
                        pHVar14 = g_hInst;
                        if ((*(int16_t *)lpLibFileName != 0x2e ||
                             *(int16_t *)((uintptr_t)lpLibFileName + 2) != 0) &&
                            (*(int16_t *)lpLibFileName != 0)) {
                            pHVar14 = LoadLibraryExW((LPCWSTR)lpLibFileName, (HANDLE)0, 2);
                            pHVar19 = pHVar14;
                        }
                        *pWVar12 = WVar23;
                        pHVar15 = 0;
                        if (pHVar14 != 0) {
                            pHVar15 = FUN_14001EA18(pHVar14, local_68, pWVar13,
                                                    (int64_t *)&local_90, NULL);
                        }
                        if (pHVar19 != 0) {
                            FreeLibrary(pHVar19);
                        }
                        uVar16 = local_88;
                        if (pHVar15 == 0) {
                            PECMD_FreeStrBuf((WCHAR **)&local_90);
                            pHVar22 = (HMODULE)1;
                            goto LAB_14002f943;
                        }
                    }
                    if ((int64_t)uVar16 < 0xb) {
                        PECMD_FreeStrBuf((WCHAR **)&local_90);
                        pHVar22 = (HMODULE)0xffffffff80070057ULL;
                        goto LAB_14002f943;
                    }
                    local_res20[0] = 0;
                    local_98 = AddFontMemResourceEx(local_90, (DWORD)uVar16, NULL, local_res20);
                    if (local_98 != 0) {
                        if (local_78 == NULL) {
                        LAB_14002f98f:
                            PECMD_FreeStrBuf((WCHAR **)&local_90);
                            goto LAB_14002fbdf;
                        }
                        {
                            HICON pHVar17 = PECMD_LoadIcon((LPCWSTR)(uintptr_t)-1, NULL);
                            if (pHVar17 != 0) {
                                int64_t local_a8 = 0;
                                ((void (*)(int64_t *))g_pGdipNewPrivateFontCollection)(&local_a8);
                                if (local_a8 != 0) {
                                    ((void (*)(int64_t, void *, DWORD))g_pGdipPrivateAddMemoryFont)(
                                        local_a8, local_90, (DWORD)uVar16);
                                    FUN_1400633A8((WCHAR **)&local_70, 800);
                                    local_res18 = 0;
                                    ((void (*)(int64_t, int, void *,
                                               uint64_t *))g_pGdipGetFontCollectionFamilyList)(
                                        local_a8, 100, local_70, &local_res18);
                                    if (0 < (int)local_res18) {
                                        PECMD_AllocString(&local_b0,
                                                          ((int)local_res18 * 9 + 9) * 4);
                                    }
                                    pWVar12 = local_b0;
                                    iVar8 = (int)local_res18;
                                    if (0 < (int)local_res18) {
                                        do {
                                            if (0 < iVar8) {
                                                if (0 < (intptr_t)pHVar22) {
                                                    *pWVar12 = L'\r';
                                                    pWVar12[1] = L'\n';
                                                    pWVar12 = pWVar12 + 2;
                                                }
                                                *pWVar12 = L'\"';
                                                pWVar13 = pWVar12 + 1;
                                                ((void (*)(uintptr_t, WCHAR *,
                                                           uint16_t))g_pGdipGetFamilyName)(
                                                    *(uint64_t *)((uint8_t *)local_70 +
                                                                  (intptr_t)pHVar22 * 8),
                                                    pWVar13, (uint16_t)local_58);
                                                iVar8 = lstrlenW(pWVar13);
                                                pWVar9 = StrChrW(pWVar13, L' ');
                                                if (pWVar9 == NULL) {
                                                    memmove(pWVar12, pWVar13, iVar8 * 2);
                                                    uVar16 = 0xfffffffffffffffeULL;
                                                }
                                                else {
                                                    pWVar13[iVar8] = L'\"';
                                                    uVar16 = uVar20;
                                                }
                                                pWVar12 = (WCHAR *)((uintptr_t)(pWVar13 + iVar8) +
                                                                    uVar16);
                                                iVar8 = (int)local_res18;
                                            }
                                            pHVar22 = (HMODULE)((intptr_t)pHVar22 + 1);
                                        } while ((intptr_t)pHVar22 < (intptr_t)iVar8);
                                    }
                                    *pWVar12 = L'\0';
                                    ((void (*)(int64_t *))g_pGdipDeletePrivateFontCollection)(
                                        &local_a8);
                                    PECMD_FreeStrBuf((WCHAR **)&local_70);
                                    DVar7 = 0;
                                    goto LAB_14002f98f;
                                }
                            }
                        }
                    }
                    PECMD_FreeStrBuf((WCHAR **)&local_90);
                }
            }
            else {
                PECMD_ParseNumSkipWs(&local_res10, &local_98);
                if ((local_98 != 0) && (RemoveFontMemResourceEx(local_98) != 0)) {
                LAB_14002f943:
                    PECMD_FreeStrBuf((WCHAR **)&local_a0);
                    PECMD_FreeStrBuf((WCHAR **)&local_b0);
                    return pHVar22;
                }
            }
            DVar7 = GetLastError();
            if (DVar7 == 0) {
                DVar7 = 1;
            }
        }
    LAB_14002fbdf:
        if (local_60 != NULL) {
            PECMD_AppendFmtValue(fontList, local_98, local_60, WSTR("0x%I64X"));
        }
        if (local_78 != NULL) {
            FUN_1400629B8(fontList, local_78, local_b0);
        }
    LAB_14002fc1a:
        PECMD_FreeStrBuf((WCHAR **)&local_a0);
        PECMD_FreeStrBuf((WCHAR **)&local_b0);
        return (HMODULE)(intptr_t)(int)DVar7;
    }
}

/* ========== PECMD_EvalLoopCondition @0x140032dc4 ==========
 * 原 PECMD_EvalLoopCondition，大型 IF 条件求值器。
 * 保留原分支/跳转结构；Ghidra 的 CONCATxx/位域宏已用整数运算替换，
 * 个别不确定处保留 TODO(verify)。
 */
ULARGE_INTEGER PECMD_EvalLoopCondition(int64_t *script, ULARGE_INTEGER value, uint32_t flags,
                                       LPCWSTR text)
{
    uint8_t bVar1 = 0, bVar16 = 0, bVar20 = 0, bVar21 = 0, bVar33 = 0, bVar36 = 0;
    WCHAR WVar2 = 0, WVar29 = 0;
    uint32_t uVar3 = 0, uVar4 = 0, uVar5 = 0, uVar22 = 0, uVar26 = 0, uVar37 = 0;
    uint32_t uVar39 = 0;
    int iVar6 = 0, iVar7 = 0, iVar23 = 0;
    DWORD DVar8 = 0, DVar9 = 0, DVar10 = 0, DVar38 = 0;
    WCHAR *pWVar11 = NULL, *pWVar32 = NULL, *pWVar41 = NULL;
    uint64_t uVar12 = 0, uVar14 = 0, uVar25 = 0, uVar34 = 0;
    int64_t lVar13 = 0, lVar40 = 0;
    uint16_t uVar17 = 0, uVar28 = 0;
    double dVar45 = 0.0, local_120 = 0.0;
    bool bVar42 = false, bVar43 = false, bVar44 = false;
    char cVar31 = 0;
    LPCWSTR pWVar18 = NULL;
    ULARGE_INTEGER local_res10 = {{0, 0}};
    ULARGE_INTEGER UVar15 = {{0, 0}};
    ULARGE_INTEGER UVar19 = {{0, 0}};
    ULARGE_INTEGER UVar27 = {{0, 0}};
    ULARGE_INTEGER UVar30 = {{0, 0}};
    ULARGE_INTEGER UVar35 = {{0, 0}};
    ULARGE_INTEGER local_1a0 = {{0, 0}};
    ULARGE_INTEGER local_190 = {{0, 0}};
    ULARGE_INTEGER local_188 = {{0, 0}};
    ULARGE_INTEGER local_178 = {{0, 0}};
    ULARGE_INTEGER local_170 = {{0, 0}};
    ULARGE_INTEGER local_160 = {{0, 0}};
    ULARGE_INTEGER local_150 = {{0, 0}};
    ULARGE_INTEGER local_140 = {{0, 0}};
    ULARGE_INTEGER local_138 = {{0, 0}};
    ULARGE_INTEGER local_130 = {{0, 0}};
    ULARGE_INTEGER local_b0 = {{0, 0}};
    ULARGE_INTEGER local_a8 = {{0, 0}};
    ULARGE_INTEGER local_118 = {{0, 0}};
    LPCWSTR local_res20 = NULL;
    uint32_t local_res18 = 0;
    uint32_t local_1a4 = 0, local_180 = 0, local_168 = 0, local_158 = 0;
    uint32_t local_128 = 0, local_104 = 0, local_100 = 0, local_148 = 0;
    uint32_t local_108 = 0x3d, local_fc = 0x26, local_f8 = 0x7c, local_e0 = 0x40;
    uint32_t local_e8 = 0;
    uint8_t local_1a7 = 0, local_198 = 0;
    uint16_t local_ee = 0, local_ec = 0;
    WCHAR local_f0 = 0, local_ea = 0, local_98 = 0, local_96 = 0;
    WCHAR *local_110 = NULL, *local_d8 = NULL, *local_d0 = NULL;
    WCHAR *local_c8 = NULL, *local_c0 = NULL, *local_a0 = NULL, *local_b8 = NULL;
    uint16_t *puVar24 = NULL;

    (void)local_ee;
    (void)local_ec;
    (void)local_e8;
    (void)local_ea;
    (void)local_96;

    local_res10 = value;
    local_res20 = text;
    EnterCriticalSection((LPCRITICAL_SECTION)&g_csInit);
    UVar30.QuadPart = 0;
    local_128 = flags & 0x40000;
    uVar26 = flags & 0x10000;
    iVar7 = 0;
    uVar3 = flags & 0xff;
    local_158 = 0;
    local_188.QuadPart = (uint64_t)(flags & 0x20000); /* 原 CONCAT44 简化 */
    uVar39 = (uint32_t)(uVar3 == 1);
    uVar22 = (uint32_t)(uVar3 == 0);
    local_198 = 0;
    if ((uVar22 != 0) || (uVar39 != 0)) {
        local_198 = 1;
    }
    pWVar41 = (WCHAR *)(local_res10.QuadPart - 2);
    local_res18 = uVar26;
    local_148 = uVar39;
    local_110 = pWVar41;
    local_b8 = pWVar41;
    bVar1 = (uint8_t)FUN_1400660AC("!", (int64_t *)&local_res10.QuadPart, 1);
    local_180 = 0x100002;
    local_100 = (uint32_t)bVar1;
    uVar5 = 0x100001;
    local_1a4 = 0x100003;
    local_168 = 0x100001;
    local_f8 = 0x7c;
    uVar4 = 0x24;
    if (uVar22 == 0) {
        if (uVar39 != 0) {
            local_168 = 0x7c;
            local_1a4 = 0x24;
            uVar4 = 0x7c;
            goto LAB_140032f00;
        }
        if ((uVar3 == 2) != 0) {
            local_158 = 0x7c;
            goto LAB_140032ef6;
        }
    }
    else {
    LAB_140032ef6:
        local_1a4 = 0x7c;
        local_168 = 0x24;
    LAB_140032f00:
        local_180 = 0x23;
        uVar5 = uVar4;
    }
    local_1a7 = 0;
    if (*(uint16_t *)local_res10.QuadPart == 0x2a) {
        local_1a7 = (uint8_t)uVar5;
        local_128 = 0x2a;
        local_158 = uVar5;
    }
    else {
        if ((*(uint16_t *)local_res10.QuadPart != 0x24) &&
            (*(uint16_t *)local_res10.QuadPart != 0x23) &&
            (*(uint16_t *)local_res10.QuadPart != 0x7c)) {
            goto LAB_140032f6a;
        }
        local_158 = (uint32_t)*(uint16_t *)local_res10.QuadPart;
        local_1a7 = (uint8_t)*(uint16_t *)local_res10.QuadPart;
    }
    local_res10.QuadPart = local_res10.QuadPart + 2;
LAB_140032f6a:
    UVar15 = local_res10;
    bVar43 = *(int16_t *)local_res10.QuadPart == 0x5b;
    uVar37 = flags & 0x20000;
    local_170 = local_res10;
    if (bVar43) {
        iVar6 = lstrlenW((LPCWSTR)local_res10.QuadPart);
        for (puVar24 = (uint16_t *)(UVar15.QuadPart + (int64_t)(iVar6 + -1) * 2);
             ((8 < *puVar24 && (*puVar24 < 0xe)) || (*puVar24 == 0x20)); puVar24 = puVar24 - 1) {
            *puVar24 = 0;
        }
        if (*puVar24 == 0x5d) {
            *puVar24 = 0;
        }
        local_170.QuadPart = UVar15.QuadPart + 2;
        WVar2 = *(WCHAR *)local_170.QuadPart;
        if (WVar2 != L'\0') {
            do {
                if ((((uint16_t)WVar2 < 9) || (0xd < (uint16_t)WVar2)) && (WVar2 != L' ')) {
                    if ((WVar2 != L'[') && (WVar2 != L'('))
                        break;
                    *pWVar41 = WVar2;
                    pWVar41 = pWVar41 + 1;
                }
                local_170.QuadPart = local_170.QuadPart + 2;
                WVar2 = *(WCHAR *)local_170.QuadPart;
            } while (WVar2 != L'\0');
            uVar26 = local_res18;
            uVar37 = local_188.LowPart;
            local_110 = pWVar41;
        }
    }
    if ((*(char *)((char *)script + 0xda) != '\0') || (*(char *)((char *)script + 0xd) != '\0')) {
        iVar7 = 1;
    }
    local_d8 = (WCHAR *)(intptr_t)iVar7;
    local_108 = 0x3d;
    local_d0 = (WCHAR *)(intptr_t)(int)(uint32_t)(uVar3 == 2);
    local_fc = 0x26;
    local_a0 = (WCHAR *)(intptr_t)(int)uVar22;
    local_e0 = 0x40;
    local_c8 = (WCHAR *)(intptr_t)(int)uVar26;
    local_c0 = (WCHAR *)(intptr_t)(int)uVar37;
    UVar15 = local_170;

LAB_1400330ae:
    uVar26 = local_128;
    local_res10 = UVar15;
    bVar1 = (uint8_t)FUN_1400660AC("!", (int64_t *)&local_res10.QuadPart, 1);
    uVar37 = local_158;
    pWVar41 = (WCHAR *)(intptr_t)(int)local_158;
    local_150 = local_res10;
    local_104 = (uint32_t)bVar1;
    bVar16 = (uint8_t)UVar30.LowPart;
    local_res18 = (local_res18 & 0xffffff00u) | bVar16;
    local_188.LowPart = local_158;
    WVar2 = (WCHAR)UVar30.LowPart;
    DVar8 = UVar30.LowPart;
    bVar1 = bVar16;
    UVar15.QuadPart = 0;
    local_190.QuadPart = 0;
    local_178.QuadPart = 0;
    local_160.QuadPart = 0;
    local_140.QuadPart = 0;
    local_138.QuadPart = 0;
    local_130.QuadPart = 0;

    if (bVar43 == (bool)bVar16) {
        UVar19 = local_res10;
        UVar27 = local_res10;
        uVar3 = uVar37;
        if ((local_148 == DVar8) || (*(WCHAR *)local_res10.QuadPart != L'\"')) {
        LAB_140033715:
            WVar29 = *(WCHAR *)UVar19.QuadPart;
            while (WVar29 != WVar2) {
                if ((((WVar29 == L'>') || (WVar29 == L'<')) || (WVar29 == L'=')) ||
                    (WVar29 == L'!')) {
                    bVar1 = 1;
                    local_res18 = (local_res18 & 0xffffff00u) | 1;
                    PECMD_StrBldCopyWide((WCHAR **)&local_130.QuadPart, (LPCWSTR)UVar19.QuadPart);
                    local_160 = local_130;
                    *(WCHAR *)UVar19.QuadPart = WVar2;
                    UVar27 = local_res10;
                    UVar15 = local_130;
                    break;
                }
                UVar19.QuadPart = UVar19.QuadPart + 2;
                WVar29 = *(WCHAR *)UVar19.QuadPart;
            }
        }
        else {
            UVar19.QuadPart = local_res10.QuadPart + 2;
            WVar29 = *(WCHAR *)UVar19.QuadPart;
            if (WVar29 != WVar2) {
                do {
                    UVar19.QuadPart = UVar19.QuadPart + 2;
                    if (WVar29 == L'\"')
                        break;
                    WVar29 = *(WCHAR *)UVar19.QuadPart;
                } while (WVar29 != WVar2);
                goto LAB_140033715;
            }
        }
    }
    else {
        local_170 = local_res10;
        WVar29 = *(WCHAR *)local_res10.QuadPart;
        local_1a0 = local_res10;
        uVar34 = UVar30.QuadPart & 0xff;
        uVar12 = UVar30.QuadPart & 0xff;
        DVar9 = local_148;
        if ((WVar29 == L'\"') || (WVar29 == L'\'')) {
            if (9 < (uint16_t)(*(WCHAR *)(local_res10.QuadPart + 2) + 0xfff0)) {
                DVar9 = DVar8;
            }
            local_170.QuadPart = FUN_140024C48((int64_t *)&local_1a0.QuadPart, NULL, 0xc1);
            uVar34 = 1;
            uVar12 = 1;
            local_150 = local_170;
        }
        else if ((WVar29 != WVar2) &&
                 (local_150.QuadPart = local_res10.QuadPart + 2, 9 < (uint16_t)(WVar29 + 0xfff0))) {
            DVar9 = DVar8;
        }
        UVar27 = local_res10;
        UVar19 = local_1a0;
        PECMD_SkipLeadingControls((WCHAR **)&local_150.QuadPart);
        DVar38 = local_148;
        bVar33 = (uint8_t)uVar34;
        pWVar32 = (WCHAR *)(intptr_t)(int)DVar9;
        WVar29 = *(WCHAR *)local_150.QuadPart;
        while (1) {
            UVar35 = local_150;
            if (WVar29 == WVar2)
                break;
            bVar33 = (uint8_t)uVar34;
            cVar31 = (char)uVar12;
            if ((((uint16_t)WVar29 < 9) || (0xd < (uint16_t)WVar29)) && (WVar29 != L' ')) {
                if (((WVar29 == L'>') || (WVar29 == L'<')) ||
                    ((WVar29 == (WCHAR)local_108 || (WVar29 == L'!')))) {
                    bVar1 = 1;
                }
                else {
                    if (((((pWVar32 == (WCHAR *)(uintptr_t)UVar30.QuadPart) || (DVar38 == DVar8)) ||
                          (((WCHAR)local_fc != WVar29 &&
                            (((WCHAR)local_f8 != WVar29 && ((WCHAR)local_e0 != WVar29)))))) ||
                         ((((uint16_t)*(WCHAR *)(local_150.QuadPart + 2) < 9 ||
                            (0xd < (uint16_t)*(WCHAR *)(local_150.QuadPart + 2))) &&
                           (*(WCHAR *)(local_150.QuadPart + 2) != L' ')))) ||
                        ((((WVar29 = *(WCHAR *)(local_150.QuadPart - 2),
                            (uint16_t)WVar29 < 9 || (0xd < (uint16_t)WVar29)) &&
                           (WVar29 != L' ')) ||
                          ((((uint16_t)WVar29 < 9 || (0xd < (uint16_t)WVar29)) &&
                            (WVar29 != L' ')))))) {
                        if (cVar31 <= (char)bVar16)
                            goto LAB_1400332df;
                        break;
                    }
                    if (*(WCHAR *)(local_150.QuadPart + 2) == *(WCHAR *)local_150.QuadPart) {
                        *(WCHAR *)local_150.QuadPart = WVar2;
                        bVar1 = 5;
                        UVar27 = local_res10;
                        UVar35.QuadPart = local_150.QuadPart + 2;
                    }
                    else {
                        bVar1 = 3;
                    }
                }
                UVar19.QuadPart = UVar35.QuadPart + 2;
                local_res18 = (local_res18 & 0xffffff00u) | (uint8_t)bVar1;
                local_1a0 = UVar19;
                if ((bVar1 == 1) &&
                    (((*(WCHAR *)UVar19.QuadPart == L'>') || (*(WCHAR *)UVar19.QuadPart == L'<')) ||
                     ((*(WCHAR *)UVar19.QuadPart == L'=') ||
                      (*(WCHAR *)UVar19.QuadPart == L'!')))) {
                    UVar19.QuadPart = UVar35.QuadPart + 4;
                    local_1a0 = UVar19;
                }
                goto LAB_140033300;
            }
            if ('\x01' < cVar31)
                break;
            uVar12 = (uint64_t)(uint8_t)(cVar31 + 1);
            PECMD_SkipLeadingControls((WCHAR **)&local_150.QuadPart);
            local_150.QuadPart = local_150.QuadPart - 2;
        LAB_1400332df:
            bVar33 = (uint8_t)uVar34;
            local_150.QuadPart = local_150.QuadPart + 2;
            WVar29 = *(WCHAR *)local_150.QuadPart;
        }
        if (bVar33 == bVar16) {
        LAB_140033300:
            if (uVar37 == DVar8) {
                if ((local_1a4 != (uint16_t)*(WCHAR *)UVar27.QuadPart) &&
                    (local_180 != (uint16_t)*(WCHAR *)UVar27.QuadPart)) {
                    goto LAB_140033464;
                }
                pWVar41 = (WCHAR *)(uintptr_t)(uint16_t)*(WCHAR *)UVar27.QuadPart;
            LAB_1400333d1:
                if (pWVar41 == (WCHAR *)(uintptr_t)UVar30.QuadPart)
                    goto LAB_140033464;
                local_150 = UVar19;
                PECMD_SkipLeadingControls((WCHAR **)&local_150.QuadPart);
                local_170 = local_150;
                if ((*(WCHAR *)local_150.QuadPart == L'@') &&
                    ((uint16_t)(*(WCHAR *)(local_150.QuadPart + 2) + 0xfff0) < 10)) {
                    local_170.QuadPart = local_150.QuadPart + 2;
                }
                WVar29 = *(WCHAR *)local_170.QuadPart;
                UVar19.QuadPart = 0;
                while (WVar29 != WVar2) {
                    if (WVar29 == L'%') {
                        if ((2 < (int64_t)(UVar19.QuadPart + 1)) &&
                            (local_d8 == (WCHAR *)(uintptr_t)UVar30.QuadPart)) {
                            goto LAB_140033464;
                        }
                        while (1) {
                            local_170.QuadPart = local_170.QuadPart + 2;
                            WVar29 = *(WCHAR *)local_170.QuadPart;
                            if (WVar29 == L'%')
                                break;
                            if ((WVar29 == WVar2) ||
                                (((8 < (uint16_t)WVar29 && ((uint16_t)WVar29 < 0xe)) ||
                                  (WVar29 == L' ')))) {
                                goto LAB_140033486;
                            }
                        }
                        UVar19.QuadPart = UVar19.QuadPart + 2;
                    }
                    else if (((((8 < (uint16_t)WVar29) && ((uint16_t)WVar29 < 0xe)) ||
                               (WVar29 == L' ')) ||
                              ((WVar29 == L')') || (WVar29 == L']'))) ||
                             ((WVar29 == L'&') || ((WVar29 == L'|') || (WVar29 == L'@')))) {
                        break;
                    }
                    local_170.QuadPart = local_170.QuadPart + 2;
                    WVar29 = *(WCHAR *)local_170.QuadPart;
                }
            }
            else {
                if ((local_1a4 == uVar37) || (DVar9 = DVar8, local_180 == uVar37)) {
                    DVar9 = 1;
                }
                if ((*(WCHAR *)UVar27.QuadPart == L'\'') ||
                    (uVar3 = 1, *(WCHAR *)UVar27.QuadPart == L'*')) {
                    uVar3 = DVar8;
                }
                if ((DVar9 & uVar3) != 0)
                    goto LAB_1400333d1;
            LAB_140033464:
                local_170.QuadPart = FUN_140024C48((int64_t *)&local_1a0.QuadPart, NULL, 0xc1);
                UVar27 = local_res10;
            }
        LAB_140033486:
            if (bVar1 == bVar16)
                goto LAB_1400336c4;
            PECMD_StrCopyW((WCHAR **)&local_130.QuadPart, (LPCWSTR)UVar35.QuadPart,
                           (int64_t)(local_170.QuadPart - UVar35.QuadPart) >> 1);
            *(WCHAR *)UVar35.QuadPart = WVar2;
            local_160 = local_130;
            UVar27 = local_res10;
            UVar15 = local_130;
        }
        else {
        LAB_1400336c4:
            if (*(WCHAR *)local_170.QuadPart != WVar2) {
                *(WCHAR *)local_170.QuadPart = WVar2;
                local_170.QuadPart = local_170.QuadPart + 2;
                UVar27 = local_res10;
            }
        }
        uVar37 = local_188.LowPart;
        uVar3 = local_158;
    }

    if (((*(WCHAR *)UVar27.QuadPart == L'*') && (bVar43 != (bool)bVar16)) &&
        ((local_168 != uVar3 || (uVar26 != DVar8)))) {
        UVar27.QuadPart = UVar27.QuadPart + 2;
        local_188.LowPart = local_168;
        uVar26 = 0x2a;
        uVar37 = local_168;
        local_res10.QuadPart = UVar27.QuadPart;
    }

    if (local_d8 == (WCHAR *)(uintptr_t)UVar30.QuadPart) {
        if (uVar26 != DVar8) {
        LAB_14003378b:
            if ((*(uint8_t *)((char *)script + 0xd) == bVar16) &&
                (*(int16_t *)UVar27.QuadPart != 0x26)) {
                FUN_14006F884((LPCWSTR)UVar27.QuadPart, &local_190);
            }
            else {
                pWVar41 = FUN_14001E69C(script, (LPCWSTR)UVar27.QuadPart, NULL, -1);
                if (pWVar41 == (WCHAR *)(uintptr_t)UVar30.QuadPart) {
                    pWVar18 = g_szEmpty;
                }
                else {
                    pWVar18 = *(LPCWSTR *)pWVar41;
                }
                PECMD_StrBldCopyWide((WCHAR **)&local_190.QuadPart, pWVar18);
            }
            goto LAB_140033812;
        }
        FUN_14007BF44(script, (WCHAR *)UVar27.QuadPart, &local_190, 0, 1);
        UVar19 = local_190;
        local_a8 = local_190;
        local_190.QuadPart = 0;
        FUN_14007A224(script, (WCHAR *)UVar19.QuadPart, &local_190, 0, bVar16);
        if (UVar15.QuadPart != UVar30.QuadPart) {
            FUN_14007BF44(script, (WCHAR *)UVar15.QuadPart, &local_138, 0, 1);
            UVar15 = local_138;
            local_b0 = local_138;
            local_138.QuadPart = 0;
            FUN_14007A224(script, (WCHAR *)UVar15.QuadPart, &local_138, 0, bVar16);
            iVar7 = lstrlenW((LPCWSTR)local_190.QuadPart);
            FUN_14006375C((WCHAR **)&local_190.QuadPart, (LPCWSTR)local_138.QuadPart);
            UVar15.QuadPart = local_190.QuadPart + (int64_t)iVar7 * 2;
            local_160 = UVar15;
            PECMD_FreeStrBuf((WCHAR **)&local_b0.QuadPart);
        }
        PECMD_FreeStrBuf((WCHAR **)&local_a8.QuadPart);
    }
    else {
        if (uVar26 != DVar8)
            goto LAB_14003378b;
        FUN_14007BF44(script, (WCHAR *)UVar27.QuadPart, &local_190, 0, 1);
    LAB_140033812:
        if (UVar15.QuadPart != UVar30.QuadPart) {
            iVar7 = lstrlenW((LPCWSTR)local_190.QuadPart);
            UVar15 = local_160;
            lVar13 = (int64_t)iVar7;
            if (uVar26 == DVar8) {
                FUN_14007BF44(script, (WCHAR *)local_160.QuadPart, &local_138, 0, 1);
                FUN_14006375C((WCHAR **)&local_190.QuadPart, (LPCWSTR)local_138.QuadPart);
            }
            else {
                for (UVar19 = local_160; (((*(int16_t *)UVar19.QuadPart == 0x3e) ||
                                           (*(int16_t *)UVar19.QuadPart == 0x3c)) ||
                                          (*(int16_t *)UVar19.QuadPart == 0x3d)) ||
                                         (*(int16_t *)UVar19.QuadPart == 0x21);
                     UVar19.QuadPart = UVar19.QuadPart + 2) {
                }
                bVar1 = (uint8_t)local_res18;
                pWVar32 = (WCHAR *)((UVar19.QuadPart - local_160.QuadPart) >> 1);
                pWVar41 = g_szEmpty;
                local_1a0.QuadPart = 0;
                if (((*(uint8_t *)((char *)script + 0xd) != bVar16) ||
                     (*(int16_t *)UVar19.QuadPart == 0x26)) &&
                    (pWVar11 =
                         PECMD_ResolveScriptVariable(script, (LPCWSTR)UVar19.QuadPart, &local_1a0),
                     pWVar11 != (WCHAR *)(uintptr_t)UVar30.QuadPart)) {
                    pWVar41 = pWVar11;
                }
                iVar7 = lstrlenW(pWVar41);
                PECMD_AllocString(&local_190.QuadPart,
                                  (int64_t)(uintptr_t)pWVar32 + lVar13 + iVar7);
                if (pWVar32 != (WCHAR *)(uintptr_t)UVar30.QuadPart) {
                    memcpy((void *)(local_190.QuadPart + lVar13 * 2), (const void *)UVar15.QuadPart,
                           (int)(uintptr_t)pWVar32 * 2);
                }
                memcpy((void *)(local_190.QuadPart + ((uintptr_t)pWVar32 + lVar13) * 2),
                       (const void *)pWVar41, (iVar7 + 1) * 2);
                PECMD_FreeStrBuf((WCHAR **)&local_1a0.QuadPart);
                uVar37 = local_188.LowPart;
            }
            UVar15.QuadPart = local_190.QuadPart + lVar13 * 2;
            local_160 = UVar15;
        }
    }

    local_res10 = local_190;
    PECMD_StripTrailingSpaces((LPCWSTR)local_190.QuadPart);
    iVar7 = lstrlenW((LPCWSTR)local_res10.QuadPart);
    local_1a0 = local_res10;
    lVar13 = local_res10.QuadPart + (int64_t)iVar7 * 2;
    if (UVar15.QuadPart == UVar30.QuadPart) {
        iVar7 = lstrlenW((LPCWSTR)local_res10.QuadPart);
        UVar15.QuadPart = local_res10.QuadPart + (int64_t)iVar7 * 2;
        local_160 = UVar15;
    }
    pWVar41 = local_d0;
    UVar19 = local_res10;
    if (UVar15.QuadPart != UVar30.QuadPart) {
        for (; (((uint16_t)*(WCHAR *)UVar19.QuadPart < 9 ||
                 (0xd < (uint16_t)*(WCHAR *)UVar19.QuadPart)) &&
                ((*(WCHAR *)UVar19.QuadPart != L' ') && (UVar19.QuadPart < UVar15.QuadPart)));
             UVar19.QuadPart = UVar19.QuadPart + 2) {
        }
    }
    lVar40 = (int64_t)((UVar19.QuadPart - local_res10.QuadPart) >> 1);
    UVar19.QuadPart = (UVar19.QuadPart & 0xffULL) | (UVar30.QuadPart & 0xffffffffffffff00ULL);
    UVar19 = UVar30;
    UVar27 = local_res10;
    if ((((bVar1 == bVar16) && (local_d0 == (WCHAR *)(uintptr_t)UVar30.QuadPart)) &&
         (*(WCHAR *)local_res10.QuadPart != L':')) &&
        (((local_180 != uVar37 && (local_1a4 != uVar37)) ||
          ((((uint16_t)*(WCHAR *)local_res10.QuadPart < 0x30 ||
             (0x39 < (uint16_t)*(WCHAR *)local_res10.QuadPart)) &&
            ((*(WCHAR *)local_res10.QuadPart != L'(') &&
             (*(WCHAR *)local_res10.QuadPart != L'-'))))))) {
        if (local_148 == DVar8) {
        LAB_140033fb7:
            local_1a0 = local_res10;
            PECMD_SkipUntilDelim((WCHAR **)&local_1a0.QuadPart, 0x2a, 0x3f);
            DVar8 = FUN_14002D708(
                (LPCWSTR)local_res10.QuadPart,
                (uint32_t)(-(uint32_t)(*(WCHAR *)local_1a0.QuadPart != L'\0') & 0x101), NULL, 0,
                DVar8);
            UVar19.LowPart = (UVar19.LowPart & 0xffffff00u) | (uint8_t)(DVar8 != 0);
        }
        else {
        LAB_1400343b2:
            local_res10.QuadPart =
                (uint64_t)(uintptr_t)FUN_14001BE14((WCHAR *)(uintptr_t)UVar27.QuadPart);
            DVar9 = lstrlenW((LPCWSTR)local_res10.QuadPart);
            DVar38 = 0xfffffff8;
            uVar28 = *(WCHAR *)local_res10.QuadPart & 0xffdf;
            if (((uint16_t)(uVar28 - 0x41) < 0x1a) &&
                (*(WCHAR *)(local_res10.QuadPart + 2) == L':')) {
                DVar10 = 0xfffffff8;
                if (*(WCHAR *)(local_res10.QuadPart + 4) == WVar2) {
                LAB_140034486:
                    DVar38 = DVar10;
                    DVar10 = GetLogicalDrives();
                    UVar19.LowPart = (DVar10 >> (((char)uVar28 + 0xbfU) & 0x1f)) & 1;
                    UVar19.HighPart = 0;
                    if ((UVar19.LowPart == 0) || (*(WCHAR *)(local_res10.QuadPart + 4) == WVar2)) {
                        goto LAB_14003450f;
                    }
                }
                else if (uVar28 < 0x43) {
                    uVar12 = PECMD_IsRemovableDrive(uVar28);
                    DVar38 = (uint32_t)(uVar12 == DVar8);
                    DVar10 = DVar38;
                    if (DVar38 != DVar8)
                        goto LAB_140034486;
                }
            }
            if (((int)DVar8 < (int)DVar9) &&
                ((((WCHAR *)(local_res10.QuadPart - 2))[(int)DVar9] == L'\\') ||
                 (((WCHAR *)(local_res10.QuadPart - 2))[(int)DVar9] == L'/'))) {
                bVar16 = 1;
                lVar13 = (int64_t)(int)(DVar9 - 1);
                if ((int64_t)UVar30.QuadPart <= lVar13) {
                    do {
                        if ((*(WCHAR *)(local_res10.QuadPart + lVar13 * 2) != L'\\') &&
                            (*(WCHAR *)(local_res10.QuadPart + lVar13 * 2) != L'/'))
                            break;
                        *(WCHAR *)(local_res10.QuadPart + lVar13 * 2) = WVar2;
                        lVar13 = lVar13 - 1;
                    } while (-1 < lVar13);
                }
                DVar9 = 1;
            }
            UVar19 = UVar30;
            {
                bool condA = (local_c0 == (WCHAR *)(uintptr_t)UVar30.QuadPart) ||
                             (*(WCHAR *)local_res10.QuadPart == WVar2) ||
                             (*(WCHAR *)(local_res10.QuadPart + 2) != L':');
                bool condB = ((int)DVar38 <= (int)DVar8 &&
                              ((int)DVar8 <= (int)DVar38 ||
                               (uVar12 = PECMD_IsRemovableDrive(*(WCHAR *)local_res10.QuadPart),
                                (DWORD)uVar12 != DVar8)));
                if ((DVar9 != DVar8) && (condA || condB)) {
                    UVar30.LowPart = PECMD_FindFileOrDir((LPCWSTR)local_res10.QuadPart,
                                                         (int)(char)bVar16 | 0x10);
                    UVar30.HighPart = 0;
                    UVar19.HighPart = 0;
                    UVar19.LowPart = 0; /* extraout_EAX 原反编译为函数返回低 32 位，TODO(verify) */
                }
            }
        }
        goto LAB_14003450f;
    }

    if ((*(WCHAR *)local_res10.QuadPart == L':') && (bVar1 == bVar16)) {
        uVar37 = 0xffffffff;
    }
    local_188 = UVar15;
    if ((((bVar1 == 1) &&
          (local_188.QuadPart = UVar15.QuadPart + 2, *(WCHAR *)local_188.QuadPart != L'=')) &&
         ((UVar35 = UVar15, *(WCHAR *)UVar15.QuadPart != L'<') ||
          (*(WCHAR *)local_188.QuadPart != L'>'))) ||
        (UVar35 = local_188, bVar1 != bVar16)) {
        local_188.QuadPart = UVar35.QuadPart + 2;
    }
    UVar35.QuadPart = 0;
    uVar26 = local_180;
    PECMD_SkipLeadingControls((WCHAR **)&local_188.QuadPart);
    uVar3 = UVar35.LowPart;
    UVar35 = local_188;
    bVar1 = bVar16;
    if ((local_198 != bVar16) && (*(WCHAR *)local_188.QuadPart == L'?')) {
        UVar35.QuadPart = local_188.QuadPart + 2;
        local_188 = UVar35;
        PECMD_SkipLeadingControls((WCHAR **)&local_res20);
        bVar1 = 1;
    }
    if (uVar37 != local_168) {
        WVar29 = *(WCHAR *)UVar27.QuadPart;
        uVar22 = (uint32_t)(uint16_t)WVar29;
        if (((local_168 == (uint16_t)WVar29) || (uVar26 == (uint16_t)WVar29)) ||
            (local_1a4 == uVar22)) {
            UVar27.QuadPart = UVar27.QuadPart + 2;
            uVar37 = uVar22;
            uVar3 = uVar22;
            local_res10 = UVar27;
        }
    }
    if ((((pWVar41 != (WCHAR *)(uintptr_t)UVar30.QuadPart) && (local_1a7 == bVar16)) &&
         ((uVar37 != local_1a4 &&
           ((PECMD_SkipLeadingControls((WCHAR **)&local_res10.QuadPart), UVar27 = local_res10,
             (uint16_t)*(WCHAR *)local_res10.QuadPart < 0x30 ||
                 (0x39 < (uint16_t)*(WCHAR *)local_res10.QuadPart)))))) &&
        (*(WCHAR *)local_res10.QuadPart != L'.')) {
        uVar37 = DVar8;
    }
    if (((local_a0 == (WCHAR *)(uintptr_t)UVar30.QuadPart) && (uVar3 == DVar8)) &&
        (*(WCHAR *)UVar27.QuadPart == L'\'')) {
        uVar37 = local_168;
    }

    if (local_168 != uVar37) {
        if (local_1a4 == uVar37) {
            local_1a0.QuadPart = 0;
            local_150.QuadPart = 0;
            PECMD_ParseParenthesizedExpression((int64_t *)&local_res10.QuadPart,
                                               (double *)&local_1a0);
            PECMD_ParseParenthesizedExpression((int64_t *)&local_188.QuadPart,
                                               (double *)&local_150);
            local_140 = local_1a0;
            local_178 = local_150;
            if (((int8_t)(uint8_t)local_res18 < 2) &&
                (((double)local_1a0.QuadPart != (double)(int64_t)local_140.QuadPart) ||
                 ((double)local_150.QuadPart != (double)(int64_t)local_178.QuadPart))) {
                if ((double)local_1a0.QuadPart <= (double)local_150.QuadPart) {
                    if ((double)local_1a0.QuadPart != (double)local_150.QuadPart) {
                        DVar8 = 0xffffffff;
                    }
                }
                else {
                    DVar8 = 1;
                }
                goto LAB_14003425e;
            }
            goto LAB_14003426e;
        }
        if (uVar26 == uVar37) {
            PECMD_EvalParenStripped((int64_t *)&local_res10.QuadPart, &local_140);
            PECMD_EvalParenStripped((int64_t *)&local_188.QuadPart, &local_178);
            goto LAB_14003426e;
        }
        iVar7 = (int)lVar40;
        if (iVar7 < 3) {
            if (pWVar41 == (WCHAR *)(uintptr_t)UVar30.QuadPart) {
                PECMD_FreeStrBuf((WCHAR **)&local_130.QuadPart);
                PECMD_FreeStrBuf((WCHAR **)&local_138.QuadPart);
                PECMD_FreeStrBuf((WCHAR **)&local_190.QuadPart);
                LeaveCriticalSection((LPCRITICAL_SECTION)&g_csInit);
                UVar15.QuadPart = 0xffffffff80070057ULL;
                return UVar15;
            }
            goto LAB_1400343b2;
        }
        PECMD_SkipLeadingControls((WCHAR **)&local_188.QuadPart);
        bVar33 = bVar16;
        if (*(WCHAR *)local_188.QuadPart == L'#') {
            local_188.QuadPart = local_188.QuadPart + 2;
            bVar33 = 1;
        }
        bVar21 = (uint8_t)local_148;
        local_120 = 0.0;
        bVar36 = 1;
        if (*(WCHAR *)local_188.QuadPart == L'*') {
            local_188.QuadPart = local_188.QuadPart + 2;
            bVar36 = bVar16;
        }
        local_118 = local_188;
        uVar12 = PECMD_ParseAndSkipSpace_7f90((int64_t *)&local_118.QuadPart, &local_120);
        local_118 = local_188;
        FUN_14006A7F4((int64_t *)&local_118.QuadPart, &local_178);
        if (*(WCHAR *)local_118.QuadPart == L'.') {
            local_178.QuadPart = (ULONGLONG)local_120;
        }
        if (((uint32_t)(iVar7 - 3) < 3) &&
            (DVar9 = StrCmpNIW((LPCWSTR)local_1a0.QuadPart, WSTR("MEM"), 3), DVar9 == DVar8)) {
            bVar20 = bVar21;
            if (iVar7 == 3) {
            LAB_140033e74:
                bVar21 = bVar20;
                bVar20 = 1;
            LAB_140033e77:
                local_140.QuadPart = PECMD_QueryPhysicalMemory((int)(char)bVar21);
                UVar27 = local_140;
                if (bVar20 != bVar16) {
                    UVar27.QuadPart = (local_140.QuadPart + 0xfffff) >> 0x14;
                }
                if (bVar1 == bVar16) {
                    if ((int)DVar8 < (int)uVar12) {
                        UVar15 = local_160;
                        if (bVar20 != bVar16) {
                            dVar45 = (double)(int64_t)UVar27.QuadPart;
                            if ((int64_t)UVar27.QuadPart < 0) {
                                dVar45 = dVar45 + g_dbl1668;
                            }
                            if (dVar45 <= local_120) {
                                bVar42 = isnan(dVar45) || isnan(local_120);
                                bVar44 = dVar45 == local_120;
                            LAB_140033f28:
                                if (bVar42 || (UVar27.QuadPart = 0, !bVar44)) {
                                    UVar27.QuadPart = 0xffffffffffffffffULL;
                                }
                                UVar15 = local_160;
                                DVar8 = UVar27.LowPart;
                                goto LAB_14003425e;
                            }
                        LAB_140033ed9:
                            UVar15 = local_160;
                            DVar8 = 1;
                            goto LAB_14003425e;
                        }
                        goto LAB_14003426e;
                    }
                    goto LAB_140034640;
                }
            }
            else {
                uVar17 = *(WCHAR *)(local_1a0.QuadPart + 8) & 0xffdf;
                uVar28 = *(WCHAR *)(local_1a0.QuadPart + 6) & 0xffdf;
                bVar20 = bVar16;
                if (iVar7 == 4) {
                    if (uVar28 != (uint16_t)((int16_t)lVar40 + 0x3dU)) {
                        if (uVar28 == 0x42)
                            goto LAB_140033e77;
                        if (uVar28 != 0x55)
                            goto LAB_140033f6b;
                        bVar20 = 1;
                    }
                    goto LAB_140033e74;
                }
                if (iVar7 != 5)
                    goto LAB_140033f47;
                if (uVar28 == 0x42) {
                    if (uVar17 == 0x55) {
                        bVar21 = 1;
                    }
                    else {
                        bVar21 = bVar16;
                        if (uVar17 != 0x41)
                            goto LAB_140033f6b;
                    }
                    goto LAB_140033e77;
                }
            LAB_140033f6b:
                UVar15 = local_160;
                if (*(WCHAR *)local_res10.QuadPart == L':') {
                    if (*(WCHAR *)(local_res10.QuadPart + 2) == L'e') {
                        local_res10.QuadPart = local_res10.QuadPart + 4;
                        UVar27 = local_res10;
                        goto LAB_1400343b2;
                    }
                    if (*(WCHAR *)(local_res10.QuadPart + 2) == L'p') {
                        local_res10.QuadPart = local_res10.QuadPart + 4;
                        goto LAB_140033fb7;
                    }
                    if (*(WCHAR *)(local_res10.QuadPart + 2) == L'k') {
                        local_res10.QuadPart = local_res10.QuadPart + 4;
                        goto LAB_14003401f;
                    }
                }
                local_f0 = *(WCHAR *)local_1a0.QuadPart;
                local_ee = 0x3a;
                local_e8 = 0;
                local_ec = 0x5c;
                UVar27.QuadPart = (uint64_t)(uintptr_t)&local_f0;
                WVar29 = WVar2;
                if ((local_f0 == L'\\') &&
                    (UVar27 = local_1a0, local_160.QuadPart != UVar30.QuadPart) &&
                    (*(WCHAR *)local_160.QuadPart != WVar2)) {
                    WVar29 = *(WCHAR *)local_160.QuadPart;
                    *(WCHAR *)local_160.QuadPart = WVar2;
                }
                local_ea = WVar2;
                UVar27 = PECMD_GetDiskSpaceInfo((WCHAR *)(uintptr_t)UVar27.QuadPart,
                                                (int)(char)(uint8_t)local_148, 0);
                if (WVar29 != WVar2) {
                    *(WCHAR *)UVar15.QuadPart = WVar29;
                }
                if (bVar1 == bVar16) {
                    if ((int)uVar12 <= (int)DVar8) {
                        UVar30.QuadPart = 0xffffffffffffffffULL;
                        goto LAB_140034679;
                    }
                    if (bVar36 == bVar16) {
                        DVar8 = -(uint32_t)(UVar27.QuadPart != local_178.QuadPart);
                        if (local_178.QuadPart < UVar27.QuadPart) {
                            DVar8 = 1;
                        }
                        UVar15 = local_160;
                        if ((int64_t)local_178.QuadPart < (int64_t)UVar30.QuadPart) {
                            DVar8 = 1;
                        }
                        goto LAB_14003425e;
                    }
                    dVar45 = (double)(UVar27.QuadPart >> 0x14);
                    if (*(WCHAR *)local_118.QuadPart == L'.') {
                        dVar45 = dVar45 + (double)(UVar27.LowPart & 0xfffff) * g_dbl21630;
                    }
                    if (dVar45 <= local_120) {
                        bVar42 = isnan(dVar45) || isnan(local_120);
                        bVar44 = dVar45 == local_120;
                        goto LAB_140033f28;
                    }
                    goto LAB_140033ed9;
                }
            }
            uVar14 = PECMD_WriteNumberToScriptVar(local_res20, UVar27, script);
            UVar30.QuadPart = (uint64_t)(int32_t)uVar14;
            goto LAB_140034679;
        }
    LAB_140033f47:
        if ((iVar7 != 3) ||
            (DVar9 = StrCmpNIW((LPCWSTR)local_1a0.QuadPart, WSTR("KEY"), 3), DVar9 != DVar8)) {
            goto LAB_140033f6b;
        }
    LAB_14003401f:
        if (((uint16_t)(*(int16_t *)local_188.QuadPart - 0x30) < 10) &&
            (((uint16_t)(*(uint16_t *)(local_188.QuadPart + 2) - 0x30) < 10) ||
             ((*(uint16_t *)(local_188.QuadPart + 2) | 0x20) == 0x78))) {
            bVar33 = 2;
        }
        if (bVar33 == bVar16) {
            uVar26 = PECMD_ParseVkKeyName((LPCWSTR)local_188.QuadPart, '\0');
            local_178.QuadPart = (uint64_t)(int32_t)uVar26;
        }
        if ((int64_t)UVar30.QuadPart < (int64_t)local_178.QuadPart) {
            if (local_178.QuadPart - 0x61 < 0x1a) {
                local_178.QuadPart = local_178.QuadPart - 0x20;
            }
            uVar28 = GetAsyncKeyState(local_178.LowPart);
            uVar26 = (uint32_t)(-(uint32_t)((uVar28 & 0x8000) != 0) & local_178.LowPart);
            local_140.QuadPart = (uint64_t)(int32_t)uVar26;
            local_98 = (WCHAR)uVar26;
            local_96 = WVar2;
            if ((((int)uVar26 < 0x41) || (0x5a < (int)uVar26)) && (9 < uVar26 - 0x30)) {
                wsprintfW(&local_98, WSTR("0x%lX"));
            }
            if (bVar1 != bVar16) {
                FUN_1400629B8(script, local_res20, &local_98);
                goto LAB_140034679;
            }
            FUN_1400629B8(script, WSTR("PressKey"), &local_98);
            UVar15 = local_160;
            goto LAB_14003426e;
        }
    LAB_140034640:
        UVar30.QuadPart = 0xffffffff80070057ULL;
    LAB_140034679:
        PECMD_FreeStrBuf((WCHAR **)&local_130.QuadPart);
        PECMD_FreeStrBuf((WCHAR **)&local_138.QuadPart);
        PECMD_FreeStrBuf((WCHAR **)&local_190.QuadPart);
        LeaveCriticalSection((LPCRITICAL_SECTION)&g_csInit);
        return UVar30;
    }

    local_1a0 = UVar27;
    PECMD_SkipLeadingControls((WCHAR **)&local_1a0.QuadPart);
    uVar28 = 9;
    pWVar41 = (WCHAR *)(UVar15.QuadPart - 2);
    for (; (local_1a0.QuadPart <= (uint64_t)(uintptr_t)pWVar41 &&
            (((UVar15 = local_160, 8 < (uint16_t)*pWVar41 && ((uint16_t)*pWVar41 < 0xe)) ||
              (*pWVar41 == L' '))));
         pWVar41 = pWVar41 - 1) {
    }
    iVar7 = (int)(((uintptr_t)pWVar41 + 2 - local_1a0.QuadPart) >> 1);
    UVar27 = local_1a0;
    local_1a0 = UVar35;
    PECMD_SkipLeadingControls((WCHAR **)&local_1a0.QuadPart);
    for (puVar24 = (uint16_t *)(lVar13 + -2);
         ((uint64_t)(uintptr_t)local_1a0.QuadPart <= (uint64_t)(uintptr_t)puVar24 &&
          (((UVar15 = local_160, uVar28 <= *puVar24 && (*puVar24 < 0xe)) || (*puVar24 == 0x20))));
         puVar24 = puVar24 - 1) {
    }
    iVar23 = (int)(((uintptr_t)puVar24 + 2 - local_1a0.QuadPart) >> 1);
    iVar6 = iVar7;
    if (iVar23 < iVar7) {
        iVar6 = iVar23;
    }
    if ((int)DVar8 < iVar6) {
        if (local_c8 == (WCHAR *)(uintptr_t)UVar30.QuadPart) {
            DVar9 = StrCmpNIW((LPCWSTR)UVar27.QuadPart, (LPCWSTR)local_1a0.QuadPart, iVar6);
        }
        else {
            DVar9 = StrCmpNW((LPCWSTR)UVar27.QuadPart, (LPCWSTR)local_1a0.QuadPart, iVar6);
        }
        bVar44 = DVar9 == DVar8;
        DVar8 = DVar9;
        if (bVar44)
            goto LAB_140033c4c;
    }
    else {
    LAB_140033c4c:
        DVar8 = iVar7 - iVar23;
    }
LAB_14003425e:
    local_140.QuadPart = (uint64_t)(int32_t)DVar8;
    local_178 = UVar30;
LAB_14003426e:
    UVar27.QuadPart = 0;
    if (*(WCHAR *)UVar15.QuadPart == WVar2) {
    LAB_14003427e:
        UVar19.LowPart =
            (UVar19.LowPart & 0xffffff00u) | (uint8_t)(local_140.QuadPart != UVar27.QuadPart);
        goto LAB_14003450f;
    }
    if (*(WCHAR *)UVar15.QuadPart == L'=') {
        UVar19.LowPart =
            (UVar19.LowPart & 0xffffff00u) | (uint8_t)(local_140.QuadPart == local_178.QuadPart);
        goto LAB_14003450f;
    }
    if ((*(WCHAR *)UVar15.QuadPart == L'>') && (*(WCHAR *)(UVar15.QuadPart + 2) == L'=')) {
    LAB_1400342af:
        UVar19.LowPart = (UVar19.LowPart & 0xffffff00u) |
                         (uint8_t)((int64_t)local_178.QuadPart <= (int64_t)local_140.QuadPart);
    LAB_14003450f:
        uVar26 = UVar19.LowPart;
    }
    else {
        UVar27 = local_178;
        if (*(WCHAR *)UVar15.QuadPart == L'<') {
            if (*(WCHAR *)(UVar15.QuadPart + 2) != L'=') {
                if (*(WCHAR *)(UVar15.QuadPart + 2) == L'>')
                    goto LAB_14003427e;
                goto LAB_1400342e6;
            }
        LAB_1400342cc:
            UVar19.LowPart = (UVar19.LowPart & 0xffffff00u) |
                             (uint8_t)((int64_t)local_140.QuadPart <= (int64_t)local_178.QuadPart);
            goto LAB_14003450f;
        }
    LAB_1400342e6:
        if (*(WCHAR *)UVar15.QuadPart == L'!') {
            if (*(WCHAR *)(UVar15.QuadPart + 2) == L'=')
                goto LAB_14003427e;
            if (*(WCHAR *)(UVar15.QuadPart + 2) == L'>')
                goto LAB_1400342cc;
            if (*(WCHAR *)(UVar15.QuadPart + 2) == L'<')
                goto LAB_1400342af;
        }
        if (*(WCHAR *)UVar15.QuadPart == L'>') {
            UVar19.LowPart = (UVar19.LowPart & 0xffffff00u) |
                             (uint8_t)((int64_t)local_178.QuadPart < (int64_t)local_140.QuadPart);
            goto LAB_14003450f;
        }
        if (*(WCHAR *)UVar15.QuadPart == L'<') {
            UVar19.LowPart = (UVar19.LowPart & 0xffffff00u) |
                             (uint8_t)((int64_t)local_140.QuadPart < (int64_t)local_178.QuadPart);
            goto LAB_14003450f;
        }
        if (*(WCHAR *)UVar15.QuadPart == L'&') {
            if ((local_140.QuadPart != UVar30.QuadPart) &&
                (local_178.QuadPart != UVar30.QuadPart)) {
                UVar19.QuadPart = 1;
            }
            goto LAB_14003450f;
        }
        if (*(WCHAR *)UVar15.QuadPart == L'|') {
            if ((local_140.QuadPart != UVar30.QuadPart) ||
                (local_178.QuadPart != UVar30.QuadPart)) {
                UVar19.QuadPart = 1;
            }
            goto LAB_14003450f;
        }
        if (*(WCHAR *)UVar15.QuadPart != L'@')
            goto LAB_140034640;
        uVar25 = (uint64_t)(uint8_t)(local_178.QuadPart != UVar30.QuadPart);
        uVar14 = (uint64_t)(uint8_t)(local_140.QuadPart != UVar30.QuadPart);
        uVar26 = (uint32_t)uVar25 ^ (uint32_t)uVar14;
    }
    uVar3 = UVar30.LowPart;
    UVar27.QuadPart = (UVar27.QuadPart & 0xffULL) | (UVar30.QuadPart & 0xffffffffffffff00ULL);
    if (local_104 == uVar3) {
        bVar44 = uVar26 != uVar3;
    }
    else {
        bVar44 = uVar26 == uVar3;
    }
    UVar19.LowPart = (UVar19.LowPart & 0xffffff00u) | (uint8_t)bVar44;
    *local_110 = (WCHAR)((uint16_t)UVar19.LowPart + L'0');
    pWVar41 = local_110 + 1;
    UVar15 = UVar19;
    local_110 = pWVar41;
    if (bVar43 == (bool)(char)UVar30.LowPart)
        goto LAB_1400346eb;
    WVar2 = *(WCHAR *)local_170.QuadPart;
    WVar29 = (WCHAR)UVar30.LowPart;
    if (WVar2 != WVar29) {
        do {
            UVar15 = local_170;
            if ((((((WVar2 == L'[') || (WVar2 == L']')) || (WVar2 == L'(')) ||
                  ((WVar2 == L')') || (WVar2 == L'&'))) ||
                 ((WVar2 == L'|' && (((8 < (uint16_t)*(WCHAR *)(local_170.QuadPart + 2) &&
                                       ((uint16_t)*(WCHAR *)(local_170.QuadPart + 2) < 0xe)) ||
                                      (*(WCHAR *)(local_170.QuadPart + 2) == L' ')))))) ||
                (WVar2 == L'@')) {
                *pWVar41 = WVar2;
                pWVar41 = pWVar41 + 1;
                local_110 = pWVar41;
            }
            else if ((((uint16_t)WVar2 < 9) || (0xd < (uint16_t)WVar2)) && (WVar2 != L' ')) {
                goto code_r0x00014003460d;
            }
            local_170.QuadPart = local_170.QuadPart + 2;
            WVar2 = *(WCHAR *)local_170.QuadPart;
            if (WVar2 == WVar29)
                break;
        } while (1);
    }
LAB_1400346b2:
    local_b8[-3] = L'#';
    local_b8[-2] = L'&';
    *(uint16_t *)(local_b8 - 1) = 0x3d;
    *pWVar41 = WVar29;
    UVar15.QuadPart = PECMD_CalcExpression(script, local_b8 - 1, (void *)&local_98);
LAB_1400346eb:
    if (local_100 != uVar3) {
        bVar43 = UVar15.QuadPart == UVar30.QuadPart;
        UVar15.QuadPart = (UVar15.QuadPart & 0xffULL) | (UVar27.QuadPart & 0xffffffffffffff00ULL);
        UVar15.LowPart = (UVar15.LowPart & 0xffffff00u) | (uint8_t)bVar43;
    }
    PECMD_FreeStrBuf((WCHAR **)&local_130.QuadPart);
    PECMD_FreeStrBuf((WCHAR **)&local_138.QuadPart);
    PECMD_FreeStrBuf((WCHAR **)&local_190.QuadPart);
    LeaveCriticalSection((LPCRITICAL_SECTION)&g_csInit);
    return UVar15;

code_r0x00014003460d:
    if (*(WCHAR *)local_170.QuadPart == WVar29)
        goto LAB_1400346b2;
    PECMD_FreeStrBuf((WCHAR **)&local_130.QuadPart);
    PECMD_FreeStrBuf((WCHAR **)&local_138.QuadPart);
    PECMD_FreeStrBuf((WCHAR **)&local_190.QuadPart);
    goto LAB_1400330ae;
}

/* ====================================================================
 * b2f_part3.c — B2F 批次 Part 3 (0x140034788, 0x1400369d0, 0x140037ba8)
 *
 * 来源: PECMD原始.EXE (x64, ImageBase=0x140000000)
 *   菜单构建            PECMD_BuildImDiskMenu @0x140034788 (stub)
 *   RAM 盘命令行        PECMD_ParseImdiskMountArgs      @0x1400369d0
 *   LOGO 窗口过程       PECMD_LogoDlgProc         @0x140037ba8
 *
 * 约定:
 *   - 新实现函数使用 PECMD_ 可读名；未实现依赖仍 extern FUN_ + TODO(verify)
 *   - 反编译中的 Ghidra 扩展宏用等价整数/位运算代替
 *   - 本文件独立 gcc -std=c99 -Wall -Wextra ... 编译，零警告
 * ==================================================================== */
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>

#include "pecmd_defs.h"

/* ---- 局部补充类型/API 声明 ---- */
#ifndef B2F_PART3_LOCAL
#define B2F_PART3_LOCAL

typedef struct _MEMORYSTATUS {
    DWORD dwLength;
    DWORD dwMemoryLoad;
    uint64_t dwTotalPhys;
    uint64_t dwAvailPhys;
    uint64_t dwTotalPageFile;
    uint64_t dwAvailPageFile;
    uint64_t dwTotalVirtual;
    uint64_t dwAvailVirtual;
    uint64_t dwAvailExtendedVirtual;
} MEMORYSTATUS;

/* win32_stub.h 未声明的 CRT/Win32 接口 */


extern void GlobalMemoryStatus(MEMORYSTATUS *);


/* ---- 未实现外部依赖 (extern) ---- */
extern int PECMD_EnumImDiskDrives(uint64_t a1, int64_t *a2, int64_t a3);
extern int64_t *PECMD_ReplaceStringSlot(int64_t *a1, uint64_t *a2);
extern int FUN_14010443c(uint32_t a1);
extern void FUN_140035B40(uint32_t a1, uint32_t a2, int a3);
extern uint32_t PECMD_MountImDiskRamDisk(uint32_t *a1, uint64_t *a2, uint64_t *a3, uint32_t a4,
                                         LPCWSTR a5, int a6, LPCWSTR a7, LPCWSTR a8, uint32_t a9);
extern uint64_t PECMD_DismountRamDiskDrive(uint32_t a1, LPCWSTR a2, int a3, uint32_t a4, int a5,
                                           uint32_t a6);
extern uint32_t PECMD_QueryImDiskVolumeInfo(uint32_t a1, LPCWSTR a2, int64_t *a3, uint64_t text);
extern char PECMD_DevLockUnlock(uint64_t a1, LPCWSTR a2, uint32_t a3, uint32_t text);
extern char PECMD_DevAlignCheck(uint64_t a1, LPCWSTR a2, uint64_t a3);
extern DWORD PECMD_FormatImDiskDrive(uint32_t a1, WCHAR *a2, LPCWSTR a3); /* @0x1400279d8 */
extern uint64_t PECMD_ApplyTextWindowLayout(uint64_t a1, LPCWSTR a2);     /* @0x14002a508 */
extern void FUN_14005C61C(HKEY root, LPCWSTR sub, LPCWSTR a2);
extern DWORD PECMD_RegSetValueWithOpen(HKEY root, LPCWSTR sub, LPCWSTR a2, DWORD type, BYTE *data,
                                       DWORD size);
extern void PECMD_IncTaskCount(void);
extern void PECMD_DecrementInitRef(uint8_t a1);
extern uint64_t PECMD_DrawTooltipText(uint64_t a1, HDC a2, HWND a3);
extern void PECMD_SetThreeSlots(uint64_t *a1, uint64_t a2, uint64_t a3, uint64_t text);
extern void PECMD_RestoreAndDeleteObject(uint64_t *a1);

/* ---- 本批引用的全局数据 ---- */
extern WCHAR g_szEmpty[]; /* 空串/通用缓冲 (g_szEmpty) */
extern float g_flae4;
extern float g_flae8;
extern float g_flaf0;
extern float g_flaf4;
extern int g_intA238;
extern int g_intA23C;
extern int g_intA240;
extern int g_intA244;
extern int16_t g_transState;
extern uint32_t g_bitFlags;
extern uint8_t g_logoActiveFlag;
extern int (*g_pGdipDisposeImage)();
extern void *g_pGdipLoadImageFromFile;
extern int (*g_pGdipGetImageWidth)();
extern int (*g_pGdipGetImageHeight)();
extern void *g_pGdipDrawImageRectRectI;
extern int (*g_pGdipCreateFromHDC)();
extern int (*g_pGdipDeleteGraphics)();
extern HWND g_hPelogonWnd;
extern HWND g_wndCF68;
extern uint8_t g_u8CF83;
extern uint8_t g_u8CF84;
extern uint8_t g_u8CF85;
extern uint8_t g_u8CF87;
extern HICON g_hIconCF88;
extern int g_tipOffX;
extern int g_tipOffY;
extern HWND g_hTooltipParent;
extern WCHAR *g_pwszD220;
extern WCHAR *g_pwszD258;

extern COLORREF g_dwTipBkColor;
extern uint8_t g_u8D6F7;

#endif /* B2F_PART3_LOCAL */


/* ========== PECMD_BuildImDiskMenu @0x140034788 ==========
 * Original PECMD_BuildImDiskMenu was marked [DECOMPILE FAILED], no body.
 * Given signature:
 *   HMENU __fastcall PECMD_BuildImDiskMenu(longlong *a1, ULARGE_INTEGER pos,
 *                                  byte mode, longlong *out, undefined8 flags)
 *
 * SKIP(blocker) @0x140034788 size=4312: this function is very large
 * (~1000 instructions, 23 internal calls, 9 imports) with complex virtual-disk
 * menu building (GetMenu/GetMenuItemCount/GetSubMenu, --sub/--user/--visible/
 * --invisible/--class:/--pid/--menu/--wid/--forpid:/--fortid:, and *var* form).
 * A faithful byte-level reconstruction could not be produced & verified here,
 * so the minimal stub is kept to keep the full link free of undefined refs.
 */
HMENU PECMD_BuildImDiskMenu(int64_t *a1, ULARGE_INTEGER pos, uint8_t mode, int64_t *out,
                            uint64_t flags)
{
    (void)a1;
    (void)pos;
    (void)mode;
    (void)out;
    (void)flags;
    return (HMENU)0;
}


/* ========== PECMD_ParseImdiskMountArgs @0x1400369d0 ==========
 * RAM 盘命令行入口：解析 -a/-e/-l/-s/-v 等选项，创建/删除/查询 RAM 盘。
 * TODO(verify): 多处 Ghidra 残留寄存器值（CONCAT44/CONCAT71/extraout）按 0/低字节近似。
 */
uint64_t PECMD_ParseImdiskMountArgs(int argc, int64_t *argv, int64_t *out)
{
    uint16_t uVar1;
    uint16_t *puVar2;
    uint8_t bVar4;
    LPCWSTR pWVar5;
    char cVar6;
    WCHAR WVar7 = L'\0';
    int iVar8;
    uint32_t uVar9;
    uint32_t uVar10 = 0;
    int iVar11 = 0;
    uint64_t uVar12 = 0;
    uint32_t uVar13;
    int64_t *plVar15;
    LPCWSTR pwVar16;
    WCHAR *_Str1 = NULL;
    uint32_t uVar17 = 0xffffffff;
    int iVar18;
    uint64_t uVar19 = 0;
    uint32_t uVar20 = 0;
    bool bVar21 = false;
    uint64_t local_res20 = 0;
    int local_1f8 = 4000;
    uint64_t local_1f0 = 0;
    uint32_t local_1e8 = 0;
    uint64_t local_1e0 = 0;
    uint32_t local_1d8[2] = {0xffffffff, 0};
    uint64_t local_1d0[3] = {0, 0, 0};
    uint32_t local_1c4 = 0;
    uint32_t local_1c0 = 0;
    uint32_t local_1bc = 0;
    int local_1b8 = 0;
    int local_1b4 = 0;
    int local_1b0 = 0;
    uint16_t *local_1a8 = NULL;
    uint32_t local_1a0 = 0;
    LPCWSTR local_198 = NULL;
    WCHAR *local_190 = NULL;
    int16_t local_188;
    uint32_t local_184 = 0;
    union {
        MEMORYSTATUS ms;
        uint64_t
            raw[64]; /* TODO(verify): 反编译以 dwAvailPageFile/dwTotalVirtual 偏移兼作 64 位表 */
    } local_178;
    int64_t local_138[32] = {0}; /* TODO(verify): 原始栈上槽表大小按 32 项展开 */

    if (g_pImDiskCreateMountPoint == 0) {
        return 0xffffffffULL;
    }

    local_188 = *(int16_t *)(uintptr_t)*out;

    if (argc == 2) {
        iVar8 = wcscmp((WCHAR *)(uintptr_t)argv[1], WSTR("--version"));
        if (iVar8 == 0) {
            local_198 = NULL;
            local_res20 = 0;
            if (g_pImDiskGetVersion != NULL) {
                ((void (*)(LPCWSTR *, uint64_t *))g_pImDiskGetVersion)(&local_198, &local_res20);
            }
            PECMD_AllocStrSlot(&local_190);
            PECMD_EnumImDiskDrives(1, (int64_t *)&local_190, 0x3f);
            wsprintfW((LPWSTR)(uintptr_t)*out, WSTR("*%X.%02X  %X.%X %s"),
                      (int)(int16_t)((uint64_t)local_198 >> 8), (uint64_t)local_198 & 0xff);
            PECMD_FreeStrBuf(&local_190);
            return 0;
        }
        goto parse_loop;
    }
    else {
        uVar13 = 0;
        if (argc <= 1) {
            goto after_parse;
        }
    }

parse_loop:
    while (argc > 1) {
        uVar20 = uVar10;
        plVar15 = argv + 1;
        iVar18 = argc - 1;
        iVar8 = wcscmp((WCHAR *)(uintptr_t)*plVar15, WSTR("--id"));
        if (iVar8 == 0) {
            local_1f0 = local_1f0 | 2;
            argc = iVar18;
        }
        else {
            iVar8 = wcscmp((WCHAR *)(uintptr_t)*plVar15, WSTR("--nbrd"));
            if (iVar8 == 0) {
                local_1f0 = local_1f0 | 1;
                argc = iVar18;
            }
            else {
                iVar8 = wcscmp((WCHAR *)(uintptr_t)*plVar15, WSTR("--remove"));
                if (iVar8 == 0) {
                    local_1f0 = local_1f0 | 4;
                    argc = iVar18;
                }
                else {
                    iVar8 = wcscmp((WCHAR *)(uintptr_t)*plVar15, WSTR("-BD"));
                    if (iVar8 == 0) {
                        iVar11 = 5;
                        argc = iVar18;
                    }
                    else {
                        iVar8 = wcscmp((WCHAR *)(uintptr_t)*plVar15, WSTR("--hide"));
                        if (iVar8 == 0) {
                            local_1f0 = local_1f0 | 0x10;
                            argc = iVar18;
                        }
                        else {
                            iVar8 = lstrlenW((LPCWSTR)(uintptr_t)*plVar15);
                            bVar21 = false;
                            if (iVar8 == 2) {
                                bVar21 = *(int16_t *)(uintptr_t)*plVar15 == 0x2d;
                            }
                            if (!bVar21) {
                                return 0;
                            }
                            uVar1 = *(uint16_t *)(uintptr_t)(*plVar15 + 2);
                            if (0x6d < uVar1) {
                                if (uVar1 < 0x75) {
                                    if (uVar1 == 0x74) {
                                        if (((uVar20 & 0xf00) != 0 || iVar11 != 1) || iVar18 < 2) {
                                            return 0;
                                        }
                                        plVar15 = argv + 2;
                                        iVar8 = wcscmp((WCHAR *)(uintptr_t)*plVar15, WSTR("file"));
                                        if (iVar8 == 0) {
                                            uVar20 = uVar20 | 0x100;
                                        }
                                        else {
                                            iVar8 =
                                                wcscmp((WCHAR *)(uintptr_t)*plVar15, WSTR("vm"));
                                            if (iVar8 == 0) {
                                                uVar20 = uVar20 | 0x200;
                                            }
                                            else {
                                                iVar8 = wcscmp((WCHAR *)(uintptr_t)*plVar15,
                                                               WSTR("proxy"));
                                                if (iVar8 != 0) {
                                                    return 0;
                                                }
                                                uVar20 = uVar20 | 0x300;
                                            }
                                        }
                                    }
                                    else {
                                        if (uVar1 == 0x6e) {
                                            local_1a0 = 1;
                                            argc = iVar18;
                                            goto LAB_1400376f3;
                                        }
                                        if (uVar1 == 0x6f) {
                                            if ((iVar11 != 4 && iVar11 != 1) || iVar18 < 2) {
                                                return 0;
                                            }
                                            pwVar16 =
                                                wcstok((WCHAR *)(uintptr_t)argv[2], WSTR(","));
                                            while (uVar17 = local_1d8[0], pwVar16 != NULL) {
                                                iVar8 = wcscmp(pwVar16, WSTR("ro"));
                                                if (iVar8 == 0) {
                                                    if ((local_1e8 & 1) != 0) {
                                                        return 0;
                                                    }
                                                    local_1e8 = local_1e8 | 1;
                                                    uVar20 = uVar20 | 1;
                                                }
                                                else {
                                                    iVar8 = wcscmp(pwVar16, WSTR("rw"));
                                                    if (iVar8 == 0) {
                                                        if ((local_1e8 & 1) != 0) {
                                                            return 0;
                                                        }
                                                        local_1e8 = local_1e8 | 1;
                                                        uVar20 = uVar20 & 0xfffffffe;
                                                    }
                                                    else {
                                                        iVar8 = wcscmp(pwVar16, WSTR("rem"));
                                                        if (iVar8 == 0) {
                                                            if ((local_1e8 & 2) != 0) {
                                                                return 0;
                                                            }
                                                            local_1e8 = local_1e8 | 2;
                                                            uVar20 = uVar20 | 2;
                                                        }
                                                        else {
                                                            iVar8 = wcscmp(pwVar16, WSTR("fix"));
                                                            if (iVar8 == 0) {
                                                                if ((local_1e8 & 2) != 0) {
                                                                    return 0;
                                                                }
                                                                local_1e8 = local_1e8 | 2;
                                                                uVar20 = uVar20 & 0xfffffffd;
                                                            }
                                                            else {
                                                                iVar8 =
                                                                    wcscmp(pwVar16, WSTR("saved"));
                                                                if (iVar8 == 0) {
                                                                    if (iVar11 != 4) {
                                                                        return 0;
                                                                    }
                                                                    local_1e8 = local_1e8 | 0x10000;
                                                                    local_1f0 = local_1f0 | 8;
                                                                }
                                                                else {
                                                                    if (iVar11 != 1) {
                                                                        return 0;
                                                                    }
                                                                    iVar8 =
                                                                        wcscmp(pwVar16, WSTR("ip"));
                                                                    if (iVar8 == 0) {
                                                                        if ((uVar20 & 0xf000) !=
                                                                                0 ||
                                                                            (uVar20 & 0xf00) !=
                                                                                0x300) {
                                                                            return 0;
                                                                        }
                                                                        local_1b8 = 1;
                                                                        uVar20 = uVar20 | 0x2000;
                                                                    }
                                                                    else {
                                                                        iVar8 = wcscmp(
                                                                            pwVar16, WSTR("comm"));
                                                                        if (iVar8 == 0) {
                                                                            if ((uVar20 & 0xf000) !=
                                                                                    0 ||
                                                                                (uVar20 & 0xf00) !=
                                                                                    0x300) {
                                                                                return 0;
                                                                            }
                                                                            local_1b8 = 1;
                                                                            uVar20 =
                                                                                uVar20 | 0x1000;
                                                                        }
                                                                        else {
                                                                            iVar8 =
                                                                                wcscmp(pwVar16,
                                                                                       WSTR("shm"));
                                                                            if (iVar8 == 0) {
                                                                                if ((uVar20 &
                                                                                     0xf000) != 0 ||
                                                                                    (uVar20 &
                                                                                     0xf00) !=
                                                                                        0x300) {
                                                                                    return 0;
                                                                                }
                                                                                uVar20 =
                                                                                    uVar20 | 0x3000;
                                                                            }
                                                                            else {
                                                                                iVar8 = wcscmp(
                                                                                    pwVar16,
                                                                                    WSTR("awe"));
                                                                                if (iVar8 == 0) {
                                                                                    if (((uVar20 &
                                                                                          0xf00) !=
                                                                                             0x100 &&
                                                                                         (uVar20 &
                                                                                          0xf00) !=
                                                                                             0) ||
                                                                                        (uVar20 &
                                                                                         0xf000) !=
                                                                                            0) {
                                                                                        return 0;
                                                                                    }
                                                                                    uVar20 =
                                                                                        uVar20 |
                                                                                        0x1100;
                                                                                }
                                                                                else {
                                                                                    iVar8 = wcscmp(
                                                                                        pwVar16,
                                                                                        WSTR(
                                                                                            "par"));
                                                                                    if (iVar8 ==
                                                                                        0) {
                                                                                        if (((uVar20 &
                                                                                              0xf00) !=
                                                                                                 0x100 &&
                                                                                             (uVar20 &
                                                                                              0xf00) !=
                                                                                                 0) ||
                                                                                            (uVar20 &
                                                                                             0xf000) !=
                                                                                                0) {
                                                                                            return 0;
                                                                                        }
                                                                                        uVar20 =
                                                                                            uVar20 |
                                                                                            0x2100;
                                                                                    }
                                                                                    else {
                                                                                        iVar8 = wcscmp(
                                                                                            pwVar16,
                                                                                            WSTR(
                                                                                                "sh"
                                                                                                "ar"
                                                                                                "e"
                                                                                                "d"));
                                                                                        if (iVar8 ==
                                                                                            0) {
                                                                                            uVar20 =
                                                                                                uVar20 |
                                                                                                0x20000;
                                                                                        }
                                                                                        else {
                                                                                            if ((uVar20 &
                                                                                                 0xf0) !=
                                                                                                0) {
                                                                                                return 0;
                                                                                            }
                                                                                            iVar8 = wcscmp(
                                                                                                pwVar16,
                                                                                                WSTR(
                                                                                                    "hd"));
                                                                                            if (iVar8 ==
                                                                                                0) {
                                                                                                uVar20 =
                                                                                                    uVar20 |
                                                                                                    0x10;
                                                                                            }
                                                                                            else {
                                                                                                iVar8 = wcscmp(
                                                                                                    pwVar16,
                                                                                                    WSTR(
                                                                                                        "fd"));
                                                                                                if (iVar8 ==
                                                                                                    0) {
                                                                                                    uVar20 =
                                                                                                        uVar20 |
                                                                                                        0x20;
                                                                                                }
                                                                                                else {
                                                                                                    iVar8 = wcscmp(
                                                                                                        pwVar16,
                                                                                                        WSTR(
                                                                                                            "cd"));
                                                                                                    if (iVar8 !=
                                                                                                        0) {
                                                                                                        return 0;
                                                                                                    }
                                                                                                    uVar20 =
                                                                                                        uVar20 |
                                                                                                        0x30;
                                                                                                }
                                                                                            }
                                                                                        }
                                                                                    }
                                                                                }
                                                                            }
                                                                        }
                                                                    }
                                                                }
                                                            }
                                                        }
                                                    }
                                                }
                                            }
                                            pwVar16 = wcstok(NULL, WSTR(","));
                                        }
                                    }
                                }
                                else if (uVar1 == 0x70) {
                                    if ((iVar11 == 1) || (bVar21 = true, iVar11 == 4)) {
                                        bVar21 = false;
                                    }
                                    if ((local_198 != NULL || iVar18 < 2) || bVar21) {
                                        return 0;
                                    }
                                    local_198 = (LPCWSTR)(uintptr_t)argv[2];
                                }
                                else {
                                    if (uVar1 == 0x71) {
                                        local_1f8 = 1;
                                        argc = iVar18;
                                        goto LAB_1400376f3;
                                    }
                                    if (uVar1 != 0x73) {
                                        return 0;
                                    }
                                    if (((iVar11 != 4 && iVar11 != 1) || local_1d0[0] != 0) ||
                                        iVar18 < 2) {
                                        return 0;
                                    }
                                    local_1a8 = (uint16_t *)(uintptr_t)argv[2];
                                    PECMD_ParseNumSkipWs(&local_1a8, (int64_t *)local_1d0);
                                    uVar1 = *local_1a8;
                                    if (uVar1 < 0x55) {
                                        if (uVar1 == 0x54) {
                                            local_1d0[0] = local_1d0[0] << 10;
                                        LAB_140037078:
                                            local_1d0[0] = local_1d0[0] << 10;
                                        LAB_14003707e:
                                            local_1d0[0] = local_1d0[0] << 10;
                                        LAB_140037084:
                                            local_1d0[0] = local_1d0[0] << 10;
                                        }
                                        else if (uVar1 != 0) {
                                            if (uVar1 != 0x25) {
                                                if (uVar1 == 0x47) {
                                                    goto LAB_140037078;
                                                }
                                                if (uVar1 != 0x4b) {
                                                    if (uVar1 != 0x4d) {
                                                        return 10;
                                                    }
                                                    goto LAB_14003707e;
                                                }
                                                goto LAB_140037084;
                                            }
                                            if (99 < (int64_t)local_1d0[0] ||
                                                (int64_t)local_1d0[0] < 1) {
                                                return 0;
                                            }
                                            GlobalMemoryStatus(&local_178.ms);
                                            {
                                                /* (__uint128_t)a*b/100: portable 128-bit
                                                 * mul-div (MSVC C lacks __int128) */
                                                local_1d0[0] = PECMD_MulDivU64(
                                                    local_178.ms.dwAvailPhys,
                                                    (uint64_t)local_1d0[0], 100);
                                            }
                                        }
                                    }
                                    else {
                                        if (uVar1 == 0x62) {
                                            local_1d0[0] = local_1d0[0] << 9;
                                            goto LAB_1400370f5;
                                        }
                                        if (uVar1 == 0x67) {
                                        LAB_1400370ba:
                                            local_1d0[0] = local_1d0[0] * 1000;
                                        LAB_1400370cb:
                                            local_1d0[0] = local_1d0[0] * 1000;
                                        }
                                        else if (uVar1 != 0x6b) {
                                            if (uVar1 != 0x6d) {
                                                if (uVar1 != 0x74) {
                                                    return 10;
                                                }
                                                local_1d0[0] = local_1d0[0] * 1000;
                                                goto LAB_1400370ba;
                                            }
                                            goto LAB_1400370cb;
                                        }
                                        local_1d0[0] = local_1d0[0] * 1000;
                                    }
                                LAB_1400370f5:
                                    if ((int64_t)local_1d0[0] < 0) {
                                        GlobalMemoryStatus(&local_178.ms);
                                        local_1d0[0] = local_1d0[0] + local_178.ms.dwAvailPhys;
                                        if ((int64_t)local_1d0[0] < 0) {
                                            return 0xb;
                                        }
                                    }
                                }
                            }
                            else if (uVar1 == 0x75) {
                                if ((((_Str1 != NULL && iVar11 != 4) && iVar11 != 1) ||
                                     uVar17 != 0xffffffff) ||
                                    iVar18 < 2) {
                                    return 0;
                                }
                                iVar8 = FUN_14010443c(*(uint32_t *)(uintptr_t)argv[2]);
                                if (iVar8 == 0) {
                                    return 0;
                                }
                                local_1d8[0] =
                                    (uint32_t)wcstoul((WCHAR *)(uintptr_t)argv[2], NULL, 0);
                                uVar17 = local_1d8[0];
                            }
                            else if (uVar1 == 0x76) {
                                if (((char)local_res20 != '\0' || iVar11 != 1) || iVar18 < 2) {
                                    return 0;
                                }
                                puVar2 = (uint16_t *)(uintptr_t)argv[2];
                                if (0x38 < *puVar2 || *puVar2 < 0x31) {
                                    return 0;
                                }
                                if (puVar2[1] != 0) {
                                    return 0;
                                }
                                local_res20 = (local_res20 & 0xffffffffffffff00ULL) |
                                              (uint8_t)(*puVar2 - 0x30);
                            }
                            else if (uVar1 == 0x78) {
                                if ((local_1c0 != 0 || iVar11 != 1) || iVar18 < 2) {
                                    return 0;
                                }
                                iVar8 = FUN_14010443c(*(uint32_t *)(uintptr_t)argv[2]);
                                if (iVar8 == 0) {
                                    return 0;
                                }
                                local_1c0 = (uint32_t)wcstoul((WCHAR *)(uintptr_t)argv[2], NULL, 0);
                            }
                            else {
                                if (uVar1 != 0x79) {
                                    return 0;
                                }
                                if ((local_1c4 != 0 || iVar11 != 1) || iVar18 < 2) {
                                    return 0;
                                }
                                iVar8 = FUN_14010443c(*(uint32_t *)(uintptr_t)argv[2]);
                                if (iVar8 == 0) {
                                    return 0;
                                }
                                local_1c4 = (uint32_t)wcstoul((WCHAR *)(uintptr_t)argv[2], NULL, 0);
                            }
                            goto LAB_1400376e7;
                        }
                        if (uVar1 == 0x6d) {
                            if ((((uVar17 != 0xffffffff && iVar11 != 4) && iVar11 != 1) ||
                                 _Str1 != NULL) ||
                                iVar18 < 2) {
                                return 0;
                            }
                            _Str1 = (WCHAR *)(uintptr_t)argv[2];
                            goto LAB_1400376e7;
                        }
                        if (0x62 < uVar1) {
                            if (uVar1 != 100) {
                                if (uVar1 == 0x65) {
                                    if (iVar11 != 0) {
                                        return 0;
                                    }
                                    iVar11 = 4;
                                    argc = iVar18;
                                }
                                else {
                                    if (uVar1 == 0x66) {
                                        goto LAB_140036e53;
                                    }
                                    if (uVar1 != 0x6c) {
                                        return 0;
                                    }
                                    if (iVar11 != 0) {
                                        return 0;
                                    }
                                    iVar11 = 3;
                                    argc = iVar18;
                                }
                                goto LAB_1400376f3;
                            }
                        LAB_140036ec1:
                            if (iVar11 != 0) {
                                return 0;
                            }
                            if (uVar1 == 0x44) {
                                local_1b0 = 1;
                            }
                            iVar11 = 2;
                            argc = iVar18;
                            if (uVar1 == 0x52) {
                                local_1b0 = 1;
                                local_184 = 1;
                            }
                            goto LAB_1400376f3;
                        }
                        if (uVar1 == 0x62) {
                            if (((local_1b4 != 0 || local_1e0 != 0) || iVar11 != 1) || iVar18 < 2) {
                                return 0;
                            }
                            iVar8 = wcscmp((WCHAR *)(uintptr_t)argv[2], WSTR("auto"));
                            if (iVar8 == 0) {
                                local_1b4 = 1;
                            }
                            else {
                                local_1a8 = (uint16_t *)(uintptr_t)argv[2];
                                PECMD_ParseNumSkipWs(&local_1a8, (int64_t *)&local_1e0);
                                uVar1 = *local_1a8;
                                if (0x62 < uVar1) {
                                    return 10;
                                }
                                if (uVar1 == 0x62) {
                                    local_1e0 = local_1e0 << 9;
                                }
                                else if (uVar1 != 0) {
                                    if (uVar1 == 0x47) {
                                    LAB_140036e04:
                                        local_1e0 = local_1e0 << 10;
                                    LAB_140036e08:
                                        local_1e0 = local_1e0 << 10;
                                    }
                                    else if (uVar1 != 0x4b) {
                                        if (uVar1 != 0x4d) {
                                            if (uVar1 != 0x54) {
                                                return 10;
                                            }
                                            local_1e0 = local_1e0 << 10;
                                            goto LAB_140036e04;
                                        }
                                        goto LAB_140036e08;
                                    }
                                    local_1e0 = local_1e0 << 10;
                                }
                            }
                        }
                        else {
                            if (uVar1 == 0x44) {
                                goto LAB_140036ec1;
                            }
                            if (uVar1 == 0x46) {
                            LAB_140036e53:
                                if ((local_190 != NULL || iVar11 != 1) || iVar18 < 2) {
                                    return 0;
                                }
                                if (uVar1 == 0x46) {
                                    local_1b8 = 1;
                                }
                                local_190 = FUN_14001BE14((WCHAR *)(uintptr_t)argv[2]);
                            }
                            else {
                                if (uVar1 == 0x52) {
                                    goto LAB_140036ec1;
                                }
                                if (uVar1 != 0x53) {
                                    if (uVar1 != 0x61) {
                                        return 0;
                                    }
                                    if (iVar11 != 0) {
                                        return 0;
                                    }
                                    iVar11 = 1;
                                    argc = iVar18;
                                    goto LAB_1400376f3;
                                }
                                if ((local_1bc != 0 || iVar11 != 1) || iVar18 < 2) {
                                    return 0;
                                }
                                iVar8 = FUN_14010443c(*(uint32_t *)(uintptr_t)argv[2]);
                                if (iVar8 == 0) {
                                    return 0;
                                }
                                local_1bc = (uint32_t)wcstoul((WCHAR *)(uintptr_t)argv[2], NULL, 0);
                            }
                        }
                    LAB_1400376e7:
                        plVar15 = argv + 2;
                        argc = argc - 2;
                    }
                }
            }
        }
    LAB_1400376f3:
        argv = plVar15;
        uVar10 = uVar20;
    }

    uVar13 = local_1e8;
    if (iVar11 == 5) {
        if ((local_1f0 & 1) != 0) {
            local_1f8 = 1;
        }
        if (_Str1 == NULL) {
            return 0;
        }
        if (*_Str1 == L'\0') {
            return 0;
        }
        FUN_140035B40((uint32_t)(uint16_t)*_Str1, ~(uint32_t)(local_1f0 >> 2) & 1, local_1f8);
        return 0;
    }

after_parse:
    pWVar5 = local_190;
    uVar10 = local_1d8[0];
    WVar7 = L'\0';
    if (iVar11 == 1) {
        if (local_1b4 != 0) {
            if (local_190 == NULL) {
                return 0;
            }
            ((void (*)(LPCWSTR, uint64_t *))g_pImDiskGetOffsetByExt)(local_190, &local_1e0);
        }
        FUN_1400629B8(&g_Script, WSTR("CurRamDisk"), (LPCWSTR)g_szEmpty);
        if ((char)local_res20 == '\0') {
            if (local_1b4 != 0) {
                iVar11 = ((int (*)(LPCWSTR, uint32_t, uint64_t *, int64_t *))g_pImDiskGetPartInfo)(
                    pWVar5, local_1bc, &local_1e0, local_138);
                if (iVar11 != 0) {
                    plVar15 = local_138;
                    do {
                        bVar4 = 1;
                        if (((char)plVar15[3] == '\x05') || ((char)plVar15[3] == '\x0f')) {
                            bVar4 = 0;
                        }
                        if ((bVar4 != 0) && (plVar15[1] != 0 && *plVar15 != 0)) {
                            local_1e0 = local_1e0 + *plVar15;
                            local_1d0[0] = (uint64_t)plVar15[1];
                            break;
                        }
                        plVar15 = plVar15 + 4;
                    } while (plVar15 < local_138 + 32);
                }
            }
        }
        else {
            uVar12 = local_res20 & 0xff;
            iVar11 = ((int (*)(LPCWSTR, uint32_t, uint64_t *, int64_t *))g_pImDiskGetPartInfo)(
                pWVar5, local_1bc, &local_1e0, local_138);
            if ((iVar11 == 0) ||
                (local_178.raw[4 + uVar12 * 4] == 0 || local_178.raw[5 + uVar12 * 4] == 0)) {
                return 0xc;
            }
            local_1e0 = local_1e0 + local_178.raw[4 + uVar12 * 4];
            local_1d0[0] = local_178.raw[5 + uVar12 * 4];
        }
        EnterCriticalSection(&g_csInit);
        WVar7 = L'\0';
        if ((_Str1 != NULL) && (iVar11 = wcscmp(_Str1, WSTR("#:")), iVar11 == 0)) {
            WVar7 = ((WCHAR (*)(void))g_pImDiskFindFreeLetter)();
            *_Str1 = WVar7;
        }
        uVar10 = PECMD_MountImDiskRamDisk(local_1d8, local_1d0, &local_1e0, uVar20, pWVar5,
                                          local_1b8, _Str1, local_198, (uint32_t)local_1f0);
        uVar19 = (uint64_t)uVar10;
        LeaveCriticalSection(&g_csInit);
        uVar10 = local_1d8[0];
    }
    else if (iVar11 == 2) {
        if (((uint32_t)(local_1d8[0] == 0xffffffff) & ((_Str1 == NULL) | local_184)) == 0) {
            uVar12 = PECMD_DismountRamDiskDrive(local_1d8[0], _Str1, local_1b0, local_184,
                                                local_1f8, (uint32_t)local_1f0);
            return uVar12;
        }
    }
    else {
        if (iVar11 == 3) {
            FUN_14007034C(out, WSTR("*"));
            if (_Str1 != NULL || local_1d8[0] != 0xffffffff) {
                uVar9 = PECMD_QueryImDiskVolumeInfo(local_1d8[0], _Str1, out, local_1f0);
                return (uint64_t)uVar9;
            }
            iVar11 = PECMD_EnumImDiskDrives((uint64_t)local_1a0, out, local_188);
            return (uint64_t)iVar11;
        }
        if ((iVar11 == 4) && (_Str1 != NULL || local_1d8[0] != 0xffffffff)) {
            if ((uVar13 != 0) || (_Str1 != NULL)) {
                cVar6 = PECMD_DevLockUnlock((uint64_t)local_1d8[0], _Str1, uVar13, uVar20);
                uVar12 = (uint64_t)(uint8_t)cVar6;
            }
            if (0 < (int64_t)local_1d0[0]) {
                cVar6 = PECMD_DevAlignCheck((uint64_t)uVar10, _Str1, local_1d0[0]);
                uVar12 = (uint64_t)(uint8_t)cVar6;
            }
            uVar19 = uVar12 & 0xffffffffULL;
            if ((int32_t)uVar12 != 0) {
                return uVar19;
            }
            if (local_198 == NULL) {
                return uVar19;
            }
            PECMD_FormatImDiskDrive(uVar10, (WCHAR *)_Str1, local_198);
            goto LAB_140037a4d;
        }
    }
    return 0;

LAB_140037a4d:
    if (_Str1 != NULL) {
        iVar11 = lstrlenW(_Str1);
        if (iVar11 == 2) {
            bVar21 = _Str1[1] == L':';
        }
        else {
            iVar11 = lstrlenW(_Str1);
            if (iVar11 == 3) {
                iVar11 = wcscmp(_Str1 + 1, WSTR(":\\"));
                bVar21 = iVar11 == 0;
            }
            else {
                bVar21 = false;
            }
        }
        if ((bVar21) &&
            (FUN_1400629B8(&g_Script, WSTR("CurRamDisk"), _Str1), (local_1f0 & 1) == 0)) {
            FUN_140035B40((uint32_t)(uint16_t)*_Str1, 1, local_1f8);
        }
    }
    if ((WVar7 != L'\0') && (local_1a0 == 0)) {
        PECMD_StrBldCopyAnsi((int64_t *)&local_1a8, "*X:", (uint64_t)-1);
        PECMD_ReplaceStringSlot(out, (uint64_t *)&local_1a8);
        PECMD_FreeStrBuf((WCHAR **)&local_1a8);
        *(WCHAR *)(uintptr_t)(*out + 2) = WVar7;
        return uVar19;
    }
    PECMD_AllocString((WCHAR **)out, 0x42);
    *(uint16_t *)(uintptr_t)*out = 0x2a;
    if ((WVar7 == L'\0') || (pwVar16 = WSTR("%d %c:"), (int32_t)uVar10 < 0)) {
        pwVar16 = WSTR("%d");
    }
    wsprintfW((LPWSTR)(uintptr_t)(*out + 2), pwVar16, (uint64_t)uVar10, (uint64_t)(uint16_t)WVar7);
    return uVar19;
}

/* ========== PECMD_LogoDlgProc @0x140037ba8 ==========
 * PELOGON LOGO 窗口过程：处理初始化、绘制、淡入淡出、Logo 文件装载和销毁。
 * TODO(verify): 反编译 in_stack 高 32 位按 0 处理；RECT _8_8_ 保存/恢复无外部效应，已简化。
 */
HGDIOBJ PECMD_LogoDlgProc(uint64_t script, HWND hwnd, uint32_t msg, HDC hdcIn, uint8_t *p5)
{
    (void)hdcIn;
    uint32_t uVar1;
    float fVar2;
    WCHAR *psVar3;
    int iVar5;
    int iVar6;
    int iVar7;
    int iVar8;
    BOOL BVar9;
    DWORD DVar10;
    DWORD DVar11;
    HWND pHVar12;
    HDC hdc;
    HBRUSH hbr;
    HGDIOBJ pvVar13;
    uint64_t uVar14;
    BYTE bAlpha;
    int cx;
    uint64_t uVar15;
    uint64_t uVar16;
    uint64_t uVar17;
    HDC hDC;
    bool bVar18;
    float fVar19;
    float fVar20;
    HWND local_res10 = hwnd;
    DWORD local_res18[2] = {0, 0};
    uint32_t uVar21 = 0; /* TODO(verify): in_stack_fffffffffffffe98 >> 32 */
    uint32_t uVar22 = 0; /* TODO(verify): in_stack_fffffffffffffea0 >> 32 */
    RECT local_118;
    uint32_t local_108;
    uint32_t local_104;
    WCHAR *local_100 = NULL;
    WCHAR *local_f8 = NULL;
    int64_t local_f0;
    DWORD local_e8[4] = {0, 0, 0, 0};
    uint64_t local_d8[3] = {0, 0, 0};
    PAINTSTRUCT local_b8;

    if (msg == 0x21) {
        if ((g_u8CF85 & 0x50) != 0) {
            return (HGDIOBJ)0x4;
        }
    }
    else if (msg == 0x453) {
        PECMD_AllocWStringBuffer((WCHAR **)&local_100, 0x19000);
        *local_100 = L'\0';
        local_100[0x18ffe] = L'\0';
        local_100[0x18fff] = L'\0';
        local_e8[0] = 0;
        local_res18[0] = 0x31ffe;
        DVar10 = FUN_14005C4E0((HKEY)0xffffffff80000002, WSTR("SOFTWARE\\PELOGON"),
                               WSTR("Text.Paramd"), local_e8, (BYTE *)local_100, local_res18);
        if (DVar10 != 0) {
            *local_100 = L'\0';
        }
        uVar14 = PECMD_ApplyTextWindowLayout(script, local_100);
        SetWindowLongPtrW(local_res10, 0, (LONG_PTR)(uVar14 | 0x1010));
        PECMD_FreeStrBuf(&local_100);
        return (HGDIOBJ)(uVar14 | 0x1000);
    }

    pHVar12 = GetDesktopWindow();
    hdc = (HDC)0;
    iVar7 = 0;
    iVar6 = 0;
    DVar10 = 0;
    if (((g_WM_TaskbarRestart != 0) && (g_WM_TaskbarRestart == msg) &&
         (g_hTooltipParent != (HWND)0)) &&
        (pHVar12 != (HWND)0)) {
        g_hTooltipParent = pHVar12;
        SetParent(local_res10, pHVar12);
    }
    if (((msg != 0x110) && (msg != 0x44f)) &&
        ((pHVar12 != (HWND)0 && (g_hTooltipParent != (HWND)0 && (g_hTooltipParent != pHVar12))))) {
        g_hTooltipParent = pHVar12;
        SetParent(local_res10, pHVar12);
    }

    if (msg > 0x110) {
        if (msg == 0x111) {
            if (hdc != (HDC)0x66) {
                if (hdc != (HDC)0x67) {
                    if (hdc != (HDC)0x2) {
                        return (HGDIOBJ)0x0;
                    }
                    if ((g_bitFlags & 0xc) != 0) {
                        return (HGDIOBJ)0x0;
                    }
                }
                EnterCriticalSection(&g_csInit);
                g_taskCount = g_taskCount + 1;
                LeaveCriticalSection(&g_csInit);
                if ((hdc == (HDC)0x67) && (p5 != NULL)) {
                    *p5 = 0;
                }
                if ((g_flagA249 < 0) && ((g_bitFlags & 2) == 0)) {
                    DVar10 = GetTickCount();
                    fVar2 = g_flaf4;
                    fVar20 = 0.0f;
                    fVar19 = 0.0f;
                    do {
                        iVar6 = (int)(int64_t)((fVar20 * g_flaf0) / fVar2) + DVar10;
                        DVar11 = GetTickCount();
                        if (0 < (int)(iVar6 - DVar11)) {
                            SetLayeredWindowAttributes(
                                local_res10, 0,
                                (BYTE)(0xff - (char)(int64_t)((double)fVar19 + (double)g_flae8)),
                                2);
                        }
                        fVar19 = fVar19 + g_flae4;
                        fVar20 = fVar20 + g_flae4;
                        DVar11 = GetTickCount();
                        if (0 < (int)(iVar6 - DVar11)) {
                            SleepEx(0xb, 1);
                        }
                    } while (fVar19 <= fVar2);
                }
                EnterCriticalSection(&g_csInit);
                g_taskCount = g_taskCount - 1;
                LeaveCriticalSection(&g_csInit);
                if (g_hWaitEvent != (HANDLE)0) {
                    SetEvent(g_hWaitEvent);
                }
            }
            EnterCriticalSection(&g_csInit);
            g_bitFlags = g_bitFlags & 0xfd;
            FUN_14005C61C((HKEY)0xffffffff80000002, WSTR("SOFTWARE\\PELOGON"), WSTR("LogohWnd"));
            g_hTooltipParent = (HWND)0;
            g_flagA249 = (int8_t)0xff;
            PECMD_HeapFreeWithHeader((int64_t *)&g_pwszD220);
            if (g_hIconCF88 != 0) {
                ((void (*)(void))g_pGdipDisposeImage)();
            }
            g_hIconCF88 = (HICON)0;
            bVar18 = g_wndCF68 == g_hPelogonWnd;
            g_hPelogonWnd = (HWND)0;
            g_flagCF86 = 0;
            if (bVar18) {
                g_wndCF68 = (HWND)hdc;
            }
            LeaveCriticalSection(&g_csInit);
            PECMD_DecrementInitRef(g_u8D6F7);
            BVar9 = EndDialog(local_res10, 8);
            return (HGDIOBJ)(int64_t)BVar9;
        }
        if (msg == 0x214) {
            return (HGDIOBJ)0x0;
        }
        if (msg == 0x44f) {
            g_u8CF83 = 0;
            return (HGDIOBJ)0x0;
        }
        if (msg != 0x450) {
            return (HGDIOBJ)0x0;
        }
    LAB_14003858a:
        EnterCriticalSection(&g_csInit);
        if (msg == 0x450) {
            PECMD_AllocWStringBuffer((WCHAR **)&local_f8, 0x20a);
            *local_f8 = L'\0';
            local_e8[2] = 0;
            local_e8[1] = 0x412;
            DVar10 = FUN_14005C4E0((HKEY)0xffffffff80000002, WSTR("SOFTWARE\\PELOGON"),
                                   WSTR("LogoFile"), local_e8 + 2, (BYTE *)local_f8, local_e8 + 1);
            local_f8[0x208] = L'\0';
            if ((DVar10 == 0) && (*local_f8 != L'\0')) {
                PECMD_StrBldCopyWide(&g_pwszD220, local_f8);
            }
            PECMD_FreeStrBuf(&local_f8);
        }
        psVar3 = g_pwszD220;
        if ((g_pwszD220 != NULL) && (*g_pwszD220 != 0)) {
            if (g_hIconCF88 != 0) {
                ((void (*)(void))g_pGdipDisposeImage)();
                g_hIconCF88 = (HICON)0;
            }
            if (g_pGdipLoadImageFromFile != NULL) {
                ((void (*)(WCHAR *, HICON *))g_pGdipLoadImageFromFile)(psVar3, &g_hIconCF88);
            }
        }
        LeaveCriticalSection(&g_csInit);
        g_tipOffX = GetSystemMetrics(0);
        g_tipOffY = GetSystemMetrics(1);
        if (g_flagCF86 == '\0') {
            SetWindowPos(local_res10, (HWND)0, 0, 0, g_tipOffX, g_tipOffY, 0x10);
        }
        g_u8CF87 = 1;
        InvalidateRect(local_res10, &g_rectCF90, 1);
        g_u8CF83 = 0;
        return (HGDIOBJ)0x0;
    }

    if (msg == 0x110) {
        PECMD_IncTaskCount();
        uVar14 = GetWindowLongPtrW(local_res10, -0x14);
        SetWindowLongPtrW(local_res10, -0x14, uVar14 | 0x8000000);
        EnterCriticalSection(&g_csInit);
        FUN_14005C61C((HKEY)0xffffffff80000002, WSTR("SOFTWARE\\PELOGON"), WSTR("LogohWnd"));
        PECMD_RegSetValueWithOpen((HKEY)0xffffffff80000002, WSTR("SOFTWARE\\PELOGON"),
                                  WSTR("LogohWnd"), 0xb, (BYTE *)&local_res10, 8);
        LeaveCriticalSection(&g_csInit);
        if ((g_bitFlags & 0xc) == 8) {
            EnableWindow(local_res10, 0);
        }
        g_tipOffX = GetSystemMetrics(0);
        g_tipOffY = GetSystemMetrics(1);
        local_118.top = g_intA23C;
        local_118.left = g_intA238;
        local_118.right = g_intA240;
        local_118.bottom = g_intA244;
        if (local_118.right < 1) {
            local_118.right = ((local_118.right * 2 >> 1) - g_intA238) + g_tipOffX;
        }
        if (local_118.bottom < 1) {
            local_118.bottom = ((local_118.bottom * 2 >> 1) - g_intA23C) + g_tipOffY;
        }
        if (g_flagCF86 != '\0') {
            iVar5 = local_118.bottom;
            iVar8 = local_118.right;
            SetWindowPos(local_res10, (HWND)0, g_intA238, g_intA23C, iVar8, iVar5, 0x14);
        }
        g_hTooltipParent = (HWND)0xffffffffffffeffb;
        if (pHVar12 != (HWND)0) {
            g_hTooltipParent = pHVar12;
            SetParent(local_res10, pHVar12);
        }
        bAlpha = 0xff;
        uVar1 = 2;
        if (-1 < g_flagA249) {
            uVar1 = 3;
        }
        if (-1 < g_transState) {
            bAlpha = 0xff - (char)g_transState;
        }
        SetLayeredWindowAttributes(local_res10, g_dwTipBkColor, bAlpha, uVar1 | 2);
        iVar5 = g_tipOffX;
        iVar8 = g_tipOffY;
        if ((g_u8CF85 & 0x40) != 0) {
            iVar7 = GetSystemMetrics(0);
            iVar8 = GetSystemMetrics(1);
            GetSystemMetrics(0x3d);
            iVar5 = 300;
            iVar7 = iVar7 + -300;
            iVar6 = GetSystemMetrics(0x3e);
            iVar6 = iVar6 + -0x50;
            iVar8 = iVar8 - iVar6;
        }
        MoveWindow(local_res10, iVar7, iVar6, iVar5, iVar8, 0);
        pHVar12 = (HWND)(-(uint64_t)(g_u8CF84 != 0));
        if ((g_u8CF85 & 0x10) != 0) {
            pHVar12 = (HWND)0x1;
        }
        BVar9 = SetWindowPos(local_res10, pHVar12, iVar7, iVar6, iVar5, iVar8, 0x11);
        if (BVar9 == 0) {
            SetWindowPos(local_res10, (HWND)0, iVar7, iVar6, iVar5, iVar8, 0x11);
        }
        EnterCriticalSection(&g_csInit);
        g_hPelogonWnd = local_res10;
        g_wndCF68 = local_res10;
        g_u8CF83 = 2;
        LeaveCriticalSection(&g_csInit);
        PostMessageW(local_res10, 0x44f, 0, 0);
        return (HGDIOBJ)0x0;
    }

    if (msg == 5) {
        if (hdc == (HDC)0x1) {
            ShowWindow(local_res10, 1);
        }
        return (HGDIOBJ)0x0;
    }

    if (msg == 0xf) {
        if ((g_hTooltipParent != (HWND)0) && (g_hTooltipParent != pHVar12)) {
            g_hTooltipParent = pHVar12;
            SetParent(local_res10, pHVar12);
        }
        BeginPaint(local_res10, &local_b8);
        EndPaint(local_res10, &local_b8);
        return (HGDIOBJ)0x0;
    }

    if (msg != 0x14) {
        if (msg == 0x21) {
            return (HGDIOBJ)((-(uint64_t)((g_u8CF85 & 0x50) != 0) & 3) + 1);
        }
        if (msg != 0x7e) {
            return (HGDIOBJ)0x0;
        }
        goto LAB_14003858a;
    }

    local_f0 = 0;
    local_res18[0] = 0;
    PECMD_SetThreeSlots(local_d8, 0, 0, 0);
    iVar8 = g_tipOffX;
    cx = iVar6;
    iVar5 = iVar6;
    if (g_flagCF86 == '\0') {
        g_tipOffX = GetSystemMetrics(0);
        iVar6 = g_tipOffY;
        iVar5 = GetSystemMetrics(1);
        DVar11 = DVar10;
        if ((iVar8 != g_tipOffX) || (iVar6 != iVar5)) {
            DVar11 = 1;
        }
        cx = g_tipOffX;
        iVar6 = iVar7;
        g_tipOffY = iVar5;
        if ((g_u8CF85 & 0x40) != 0) {
            iVar6 = GetSystemMetrics(0);
            iVar5 = GetSystemMetrics(1);
            GetSystemMetrics(0x3d);
            DVar10 = iVar6 - 300;
            local_res18[0] = DVar10;
            iVar6 = GetSystemMetrics(0x3e);
            iVar5 = iVar5 - (iVar6 + -0x50);
            cx = 300;
            iVar6 = iVar6 + -0x50;
        }
        if (DVar11 != 0) {
            SetWindowPos(local_res10, (HWND)0, DVar10, iVar6, cx, iVar5, 0x16);
        }
        hdc = CreateCompatibleDC(hdc);
        local_d8[0] = (uint64_t)CreateCompatibleBitmap(hdc, cx, iVar5);
        local_d8[2] = (uint64_t)hdc;
        local_d8[1] = (uint64_t)SelectObject(hdc, (HGDIOBJ)local_d8[0]);
        if (g_hIconCF88 != 0) {
            local_108 = 0;
            local_104 = 0;
            ((void (*)(HICON, uint32_t *))g_pGdipGetImageWidth)(g_hIconCF88, &local_108);
            ((void (*)(HICON))g_pGdipGetImageHeight)(g_hIconCF88);
            uVar17 = (uint64_t)local_108;
            uVar16 = (uint64_t)g_tipOffX;
            uVar14 = (uint64_t)local_104;
            uVar15 = (uint64_t)g_tipOffY;
            local_100 = (WCHAR *)(uintptr_t)((int64_t)iVar6 * uVar14 / uVar15);
            local_f8 = (WCHAR *)(uintptr_t)((int64_t)cx * uVar17 / uVar16);
            iVar7 = ((int (*)(HDC, int64_t *))g_pGdipCreateFromHDC)(hdc, &local_f0);
            if (iVar7 == 0) {
                ((void (*)(int64_t, HICON, int, int, uint64_t, uint64_t, int, int, int, int, int,
                           int, int, int))g_pGdipDrawImageRectRectI)(
                    local_f0, g_hIconCF88, 0, 0, ((uint64_t)uVar21 << 32) | (uint32_t)cx,
                    ((uint64_t)uVar22 << 32) | (uint32_t)iVar5,
                    (int)((int64_t)(int32_t)DVar10 * uVar17 / uVar16), (int)(intptr_t)local_100,
                    (int)(intptr_t)local_f8, (int)((int64_t)iVar5 * uVar14 / uVar15), 2, 0, 0, 0);
                if ((g_pwszD258 != NULL) && (*g_pwszD258 != 0)) {
                    PECMD_DrawTooltipText(script, hdc, local_res10);
                }
                BitBlt(hdc, 0, 0, cx, iVar5, hdc, 0, 0, 0xcc0020);
                if (local_f0 != 0) {
                    ((void (*)(void))g_pGdipDeleteGraphics)();
                }
                goto LAB_1400381d8;
            }
        }
        hDC = hdc;
        if (g_logoActiveFlag != '\0') {
            goto LAB_140038086;
        }
    LAB_14003808f:
        if ((g_flagCCCE == '\0') || (g_u8CF87 != '\0')) {
            g_u8CF87 = '\0';
            g_pCurRect = &g_rectCF90;
        }
        else if (g_flagCF86 == '\0') {
            local_118.left = g_intA238;
            local_118.top = g_intA23C;
            local_118.right = g_intA240;
            local_118.bottom = g_intA244;
            if (local_118.right < 1) {
                local_118.right = ((local_118.right * 2 >> 1) - g_intA238) + g_tipOffX;
            }
            g_pCurRect = &local_118;
            if (local_118.bottom < 1) {
                local_118.bottom = ((local_118.bottom * 2 >> 1) - g_intA23C) + g_tipOffY;
            }
        }
        else {
            g_pCurRect = &local_118;
            GetClientRect(local_res10, &local_118);
        }
        local_res18[0] = g_pCurRect->left;
        iVar6 = g_pCurRect->top;
        cx = g_pCurRect->right - local_res18[0];
        iVar5 = g_pCurRect->bottom - iVar6;
        hbr = CreateSolidBrush(g_dwTipBkColor);
        FillRect(hDC, g_pCurRect, hbr);
        DeleteObject(hbr);
    }
    else {
    LAB_140038086:
        hDC = hdc;
        if (g_logoActiveFlag == '\0') {
            goto LAB_14003808f;
        }
    }

    PECMD_DrawTooltipText(script, hDC, local_res10);
    if (hdc != hDC) {
        BitBlt(hdc, local_res18[0], iVar6, cx, iVar5, hDC, local_res18[0], iVar6, 0xcc0020);
    }

LAB_1400381d8:
    pvVar13 = GetStockObject(5);
    PECMD_RestoreAndDeleteObject(local_d8);
    if (hdc == (HDC)0) {
        return pvVar13;
    }
    DeleteDC(hdc);
    return pvVar13;
}

/* ====================================================================
 * b2f_part4.c — B2F 子任务: 5 个 PECMD 大函数 (140038d30-14003ed4c)
 *
 * 生成自 /tmp/opencode/b2f.txt (Ghidra 反编译), 重构为可独立编译的 C99。
 * 仅用于 -fsyntax-only 验证; 最终合并进 core_b2f.c。
 *
 * 函数:
 *   uint64_t      PECMD_DispCommand      @0x140038d30
 *   uint64_t      FUN_14003B540      @0x14003b540
 *   LARGE_INTEGER FUN_14003C06C    @0x14003c06c
 *   LPCRITICAL_SECTION FUN_14003CD0C @0x14003cd0c
 *   int64_t       FUN_14003ED4C      @0x14003ed4c
 * ==================================================================== */
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>

#include "pecmd_defs.h"
extern WCHAR **FUN_14005B154(WCHAR **pp); /* @0x14005b154 */

#ifndef B2F_PART4_LOCAL
#define B2F_PART4_LOCAL

/* ---- Ghidra 位段/字段辅助宏 ---- */
#define B2F_LO8(x)  (*(uint8_t *)&(x))
#define B2F_LO16(x) (*(uint16_t *)&(x))
#define B2F_HI16(x) (*(((uint16_t *)&(x)) + 1))
#define B2F_LO32(x) (*(uint32_t *)&(x))
#define B2F_HI32(x) (*(((uint32_t *)&(x)) + 1))
#define B2F_HI24(x) ((uint32_t)((x) & 0xffffff00u))
#define B2F_LO64(x) (*(uint64_t *)&(x))
#define B2F_HI64(x) (*(((uint64_t *)&(x)) + 1))

/* ---- 反编译补充类型 ---- */
typedef struct {
    WCHAR dmDeviceName[32];
    WORD dmSpecVersion;
    WORD dmDriverVersion;
    WORD dmSize;
    WORD dmDriverExtra;
    DWORD dmFields;
    union {
        struct {
            POINT dmPosition;
            DWORD dmDisplayOrientation;
            DWORD dmDisplayFixedOutput;
        } field1;
    } field6_0x4c;
    WORD dmColor;
    WORD dmDuplex;
    WORD dmYResolution;
    WORD dmTTOption;
    WORD dmCollate;
    WCHAR dmFormName[32];
    WORD dmLogPixels;
    DWORD dmBitsPerPel;
    DWORD dmPelsWidth;
    DWORD dmPelsHeight;
    DWORD dmDisplayFlags;
    DWORD dmDisplayFrequency;
    WCHAR dmICMMethod[4];
    DWORD dmICMIntent;
    DWORD dmMediaType;
    DWORD dmDitherType;
    DWORD dmReserved1;
    DWORD dmReserved2;
    ULONGLONG dmPanningWidth;
    ULONGLONG dmPanningHeight;
} DEVMODEW;
typedef DEVMODEW *LPDEVMODEW;

typedef struct {
    DWORD cb;
    WCHAR DeviceName[32];
    WCHAR DeviceString[128];
    DWORD StateFlags;
    WCHAR DeviceID[128];
    WCHAR DeviceKey[128];
} _DISPLAY_DEVICEW;

typedef struct {
    HWND hwnd;
    UINT wFunc;
    LPCWSTR pFrom;
    LPCWSTR pTo;
    WORD fFlags;
    BOOL fAnyOperationsAborted;
    void *hNameMappings;
    LPCWSTR lpszProgressTitle;
} _SHFILEOPSTRUCTW;

/* ---- win32_stub.h 缺失的 API ---- */
extern BOOL EnumDisplayDevicesW(LPCWSTR, DWORD, _DISPLAY_DEVICEW *, DWORD);
extern LPSTR StrStrA(LPCSTR, LPCSTR);
extern BOOL MoveFileExW(LPCWSTR, LPCWSTR, DWORD);
extern int SHFileOperationW(_SHFILEOPSTRUCTW *);
extern HANDLE OpenThread(DWORD, BOOL, DWORD);

/* ---- 本文件引用的全局数据 (完整链接时与 core_globals.c 对齐) ---- */
/* 空串 */
extern WCHAR *PTR_DAT_14013a250;
extern double g_dbl21c80;
extern double g_dbl21bf0;
extern uint16_t g_u161bec;
extern uint16_t g_u161be4;
extern double g_dbl2598;
extern double g_dbl22590;
extern double g_dbl22588;
extern double g_dbl2578;
extern double g_dbl22580;
extern double g_dbl20b28;
extern int (*g_pSetDisplayConfig)(UINT, UINT, UINT); /* SetDisplayConfig 函数指针 */
extern int (*g_pSetDeviceGammaRamp)(HDC, void *);    /* SetDeviceGammaRamp 函数指针 */
extern int (*g_pGetDeviceGammaRamp)(HDC, void *);    /* GetDeviceGammaRamp 函数指针 */
                                                     /* g_Script */
extern DWORD g_dwC96C;
extern WCHAR u__26_INDATA_140121fe0[0x26];
extern WCHAR _UNK_140121ff6;

/* ---- 未实现外部依赖 (保留 FUN_ 原名) ---- */
extern int64_t PECMD_OpenFileHandle();
extern int64_t PECMD_CreateMutexSlot();
extern int64_t PECMD_ReleaseMutex();
extern int64_t PECMD_TlsLogWrite();
extern int64_t FUN_1400195F0();
extern int64_t PECMD_SetCurrentDirIfChanged();
extern int64_t PECMD_RegisterCallbackWnd();
extern int64_t PECMD_GenRandomSeed16();
extern int64_t FUN_14001B5AC();
extern int64_t PECMD_GetParentProcessIdLevel();
extern int64_t PECMD_MsgWaitForObjects();
extern int64_t PECMD_WaitCountPumpMessages();
extern void *PECMD_MatchKeywordToken();
extern int64_t PECMD_AppendParamToken();
extern int64_t PECMD_DeleteDirectoryTree();
extern int64_t PECMD_EnumDisplayModes();
extern int64_t PECMD_RunPecmdMain();
extern int64_t PECMD_IsSysStartuped();
extern int64_t PECMD_RunSysInit();
extern int64_t PECMD_WaitKeyPressHooked();
extern int64_t FUN_140025f10();
extern uint64_t PECMD_StartWorkerThread(void *script, void **pref, uint32_t a3, uint64_t a4,
                                        uint64_t a5, uint32_t a6, uint64_t a7, int64_t a8, int a9);
extern int64_t PECMD_StartOnlyApp();
extern int64_t PECMD_RunCommand();
extern uint64_t PECMD_DeviSubPackageWorkerProc(uint64_t *task);
extern uint64_t PECMD_DeviExtractSchedulerProc(uint64_t *tasks);
extern LARGE_INTEGER PECMD_ProcessScriptBlock();
extern int64_t PECMD_SplitNextToken();
extern void PECMD_ZeroLenBuf(void *p);
extern int64_t FUN_14005B1A8();
extern void *PECMD_LastPathSeparator();
extern void *FUN_140063B00();
extern int64_t *PECMD_InitPtrTable(int64_t *arr);
extern int64_t PECMD_MatchPrefixAdvance();
extern int64_t PECMD_MatchAssignToken();
extern int64_t PECMD_ParseAndSkipSpace_7b54();
extern int64_t PECMD_EvalExpressionTree();
extern int64_t PECMD_ParseIntRound();
extern int64_t PECMD_CopyStrToSlot();
extern int64_t PECMD_ParseUIntValue();
extern int64_t PECMD_FreeArray_ddf8();
extern int64_t FUN_14009BB28();
extern int64_t PECMD_RunScriptText(void *pScript, LPCWSTR pText, LPCWSTR pName, LPCWSTR pCurFile,
                                   uint32_t flags, LPCWSTR pFile, void *pPersist);
extern LARGE_INTEGER FUN_1400D2E90();
extern void *PECMD_MapFileView();
extern int64_t FUN_1400E693C();
extern int64_t PECMD_FindFirstFileW();

#endif /* B2F_PART4_LOCAL */

/* ==================================================================== */
/* ========== PECMD_DispCommand @0x140038d30 ========== */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */
/* WARNING: Restarted to delay deadcode elimination for space: register */

uint64_t PECMD_DispCommand(int64_t *script, WCHAR *cmd)

{
    uint16_t *puVar1;
    WCHAR WVar2;
    WCHAR WVar3;
    uint8_t auVar4[16];
    LPCWSTR pWVar5;
    double dVar6;
    LPCWSTR pWVar7;
    bool bVar8;
    char cVar9;
    bool bVar10;
    uint16_t uVar11;
    uint16_t uVar12;
    uint32_t uVar13;
    int iVar14;
    int iVar15;
    BOOL BVar16;
    LONG LVar17;
    uint32_t uVar18;
    uint64_t uVar19;
    LPWSTR pWVar20;
    int64_t *plVar21;
    HDC hDC;
    uint64_t uVar22;
    LPCWSTR *ppWVar23;
    uint16_t *puVar24;
    LPCWSTR pwVar25;
    uint64_t uVar26;
    uint64_t uVar27;
    int64_t lVar28;
    WCHAR *pWVar29;
    char cVar30;
    uint32_t uVar31;
    DWORD DVar32;
    uint16_t *puVar33;
    int iVar34;
    int64_t lVar35;
    char cVar36;
    DEVMODEW *pDVar37;
    char cVar38;
    uint32_t uVar39;
    bool bVar40;
    LPWSTR local_res10;
    uint64_t local_res18;
    uint32_t local_res20[2];
    LPWSTR local_a58;
    uint32_t local_a50;
    char local_a4c;
    int local_a48[2];
    WCHAR *local_a40;
    uint32_t local_a38;
    uint64_t local_a30;
    uint32_t local_a28;
    uint32_t local_a24;
    uint64_t local_a20;
    DWORD local_a18;
    int local_a14;
    uint64_t local_a10;
    uint64_t local_a08;
    uint32_t local_a00;
    int local_9fc;
    int64_t local_9f8;
    int64_t local_9f0;
    WCHAR *local_9e8;
    LPCWSTR local_9e0;
    int64_t local_9d8;
    BOOL local_9d0;
    LPCWSTR local_9c8;
    uint16_t *local_9c0;
    uint32_t local_9b8;
    LPCWSTR local_9b0;
    LPCWSTR local_9a8;
    LPCWSTR local_9a0;
    double local_998;
    WCHAR *local_990;
    DEVMODEW local_988;
    DEVMODEW local_8a8;
    DEVMODEW local_7c8;
    uint8_t local_6e8[84];
    uint32_t local_694;
    uint32_t local_640;
    uint32_t local_63c;
    uint32_t local_638;
    uint32_t local_630;
    WCHAR local_608[32];
    DEVMODEW local_5c8;
    WCHAR local_4e8[160];
    _DISPLAY_DEVICEW local_3a8;

    cVar36 = '*';
    cVar9 = '\0';
    if (*cmd == L'-') {
        cVar36 = '-';
    }
    local_a4c = '\0';
    cVar38 = -1;
    B2F_LO32(local_res18) = (uint32_t)local_res18 & 0xffffff00;
    cVar30 = '\0';
    local_res20[0] = local_res20[0] & 0xffffff00;
    local_a00 = ((local_a00) & 0xffff0000u) | (uint32_t)(1);
    local_a28 = 0;
    local_a24 = 0;
    local_res10 = cmd;
    local_a40 = cmd;
    if (cVar36 == (char)*cmd) {
        bVar8 = PECMD_MatchPrefixAdvance("guis", (int64_t *)&local_a40, 4);
        if (bVar8) {
            FUN_1400702B0(&local_res18, WSTR("#35:INDATA*DispSet "));
            FUN_14006375C(&local_res18, local_a40);
            uVar19 = PECMD_RunCommand(script, (WCHAR *)(uint64_t)(local_res18));
        LAB_140038e01:
            ppWVar23 = (LPCWSTR *)&local_res18;
            goto LAB_14003a7ec;
        }
        if (cVar36 == (char)*local_a40) {
            bVar8 = PECMD_MatchPrefixAdvance("confirm", (int64_t *)&local_a40, 7);
            if (bVar8) {
                local_res10 = local_a40;
                CharUpperW(local_a40);
                uVar13 = (uint32_t)(uint16_t)*local_res10;
                uVar19 = (uint64_t)(uint32_t)(int)cVar36;
                if ((uint32_t)(int)cVar36 == uVar13) {
                    do {
                        while ((uVar11 = (uint16_t)uVar13,
                                uVar11 != 0 &&
                                    (((uVar11 < 9 || (0xd < uVar11)) && (uVar11 != 0x20))))) {
                            local_res10 = local_res10 + 1;
                            uVar13 = (uint32_t)(uint16_t)*local_res10;
                        }
                        PECMD_SkipLeadingControls(&local_res10);
                        uVar13 = (uint32_t)(uint16_t)*local_res10;
                    } while ((uint32_t)uVar19 == uVar13);
                }
                pWVar20 = StrChrW(local_res10, L'O');
                uVar27 = 0;
                pWVar5 = (LPCWSTR)0x0;
                if (pWVar20 != (LPWSTR)0x0) {
                    uVar27 = 0x10;
                }
                PECMD_AllocWStringBuffer((WCHAR **)&local_res18, 0x28);
                wsprintfW((LPWSTR)(uint64_t)(local_res18), WSTR(" -- 0x%X"), uVar27);
                FUN_14006375C(&local_res18, WSTR(" -confirm- "));
                FUN_14006375C(&local_res18, local_a40);
                FUN_1400629B8(script, WSTR("&&YesNo"), g_szEmpty);
                iVar14 = lstrlenW((LPCWSTR)PTR_DAT_14013a250);
                PECMD_AllocSmallObject(&local_9b0);
                local_9a8 = pWVar5;
                local_9a0 = pWVar5;
                (void)local_9a0;
                PECMD_AllocString(&local_9b0, iVar14 + 8);
                memcpy((uint8_t *)local_9b0, PTR_DAT_14013a250, (iVar14 + 1) * 2);
                for (local_a40 = (WCHAR *)local_9b0; *local_a40 != L'\0';
                     local_a40 = local_a40 + 1) {
                    if ((char)*local_a40 == '\\') {
                        if ((char)local_a40[1] == 'n') {
                            *local_a40 = L'\r';
                            local_a40 = local_a40 + 1;
                            *local_a40 = L'\n';
                        }
                        else if ((char)local_a40[1] == '\\') {
                            *local_a40 = L' ';
                            local_a40 = local_a40 + 1;
                        }
                    }
                }
                iVar15 = lstrlenW(local_a40);
                local_9a8 = (LPCWSTR)((int64_t)iVar15 * 2);
                local_9a0 = local_9a8;
                uVar11 = PECMD_GenRandomSeed16();
                FUN_14001B5AC(local_9b0, (uint32_t)uVar11, (int64_t)(iVar14 + 2));
                pWVar7 = local_9b0;
                local_9b0 = pWVar5;
                plVar21 = PECMD_StrBldCopyAnsi((int64_t *)local_res20, "**mem", 0xffffffffffffffff);
                uVar19 = PECMD_RunScriptText(script, pWVar7, (LPCWSTR)(uint64_t)(local_res18),
                                             (LPCWSTR)*plVar21, (uint32_t)uVar11 << 0x10 | 0x40,
                                             (LPCWSTR)0x0, (int64_t *)0x0);
                PECMD_FreeStrBuf((WCHAR **)&local_res20);
                PECMD_FreeStrBuf((WCHAR **)&local_9b0);
                goto LAB_140038e01;
            }
            if (cVar36 == (char)*local_a40) {
                PECMD_MatchPrefixAdvance("nwb", (int64_t *)&local_a40, 3);
            }
        }
    }
    PECMD_GetApiProcCached("SetDisplayConfig", "User32.DLWSTR(",
                           (int64_t *)(void **)&g_pSetDisplayConfig, &g_hUser32);
    local_8a8.dmDeviceName[0] = L'\0';
    memset((uint64_t *)(local_8a8.dmDeviceName + 1), 0, 0x3e);
    memset((uint64_t *)&local_8a8.dmSpecVersion, 0, 0x9c);
    local_8a8.dmSize = 0xdc;
    local_8a8.dmPelsWidth = 0x280;
    local_8a8.dmPelsHeight = 0x1e0;
    local_8a8.dmBitsPerPel = 4;
    local_8a8.dmDisplayFrequency = 1;
    local_a38 = 0;
    PECMD_AllocStrSlot(&local_9c8);
    PECMD_AllocStrSlot(&local_9e0);
    dVar6 = g_dbl21c80;
    local_a30 = 0;
    WVar2 = *local_res10;
    local_a20 = 0;
    local_a10 = 0;
    local_a08 = 0;
joined_r0x00014003919a:
    if (WVar2 == L'\0')
        goto LAB_140039657;
    if (((WVar2 == L'-') && (local_res10[1] == L'-')) &&
        (((WVar3 = local_res10[2],
           WVar3 == L'\0' || ((8 < (uint16_t)WVar3 && ((uint16_t)WVar3 < 0xe)))) ||
          (WVar3 == L' '))))
        goto LAB_140039630;
    if (cVar36 != WVar2)
        goto LAB_140039645;
    local_res10 = local_res10 + 1;
    cVar9 = FUN_1400660AC(")delay", (int64_t *)&local_res10, 6);
    if (cVar9 == '\0') {
        cVar9 = FUN_1400660AC("reset", (int64_t *)&local_res10, 6);
        if (cVar9 != '\0') {
            cVar30 = '\x01';
            local_res20[0] = ((local_res20[0]) & 0xffffff00u) | (uint32_t)(1);
            cVar9 = (char)local_res18;
            goto LAB_1400395b8;
        }
        cVar9 = FUN_1400660AC("confirm", (int64_t *)&local_res10, 7);
        if (cVar9 == '\0') {
            cVar9 = FUN_1400660AC("confirm-", (int64_t *)&local_res10, 8);
            if (cVar9 == '\0') {
                cVar9 = FUN_1400660AC("nsort", (int64_t *)&local_res10, 5);
                if (cVar9 != '\0') {
                    local_a00 = 0;
                LAB_1400395a9:
                    cVar9 = (char)local_res18;
                    goto LAB_1400395b0;
                }
                cVar9 = FUN_1400660AC("rsort", (int64_t *)&local_res10, 5);
                if (cVar9 != '\0') {
                    local_a00 = ((local_a00) & 0xffff0000u) | (uint32_t)(0xffff);
                    goto LAB_1400395a9;
                }
                cVar9 = FUN_1400660AC("size", (int64_t *)&local_res10, 4);
                if (cVar9 != '\0') {
                    local_a28 = 0x10000;
                    goto LAB_1400395a9;
                }
                cVar9 = FUN_1400660AC("ori", (int64_t *)&local_res10, 3);
                if (cVar9 != '\0') {
                    local_a24 = 0x20000;
                    goto LAB_1400395a9;
                }
                uVar19 = PECMD_MatchAssignToken("bright:", (int64_t *)&local_res10, 6);
                if ((char)uVar19 == '\0') {
                    uVar19 = PECMD_MatchAssignToken("bright?:", (int64_t *)&local_res10, 7);
                    if ((char)uVar19 == '\0') {
                        for (;
                             (WVar2 = *local_res10,
                             WVar2 != L'\0' && ((((uint16_t)WVar2 < 9 || (0xd < (uint16_t)WVar2)) &&
                                                 (WVar2 != L' '))));
                             local_res10 = local_res10 + 1) {
                        }
                        PECMD_SkipLeadingControls(&local_res10);
                        goto LAB_1400395a9;
                    }
                    cVar9 = '\x01';
                }
                else {
                    cVar9 = -1;
                }
                local_998 = dVar6;
                if (cVar9 < '\x01') {
                    PECMD_EvalExpressionTree((int64_t *)&local_res10, &local_998);
                }
                else {
                    PECMD_SkipLeadingControls(&local_res10);
                    local_a40 = local_res10;
                    WVar2 = *local_res10;
                    while (WVar2 != L'\0') {
                        if (((8 < (uint16_t)WVar2) && ((uint16_t)WVar2 < 0xe)) || (WVar2 == L' ')) {
                            if (*local_res10 != L'\0') {
                                *local_res10 = L'\0';
                                local_res10 = local_res10 + 1;
                                PECMD_SkipLeadingControls(&local_res10);
                            }
                            break;
                        }
                        local_res10 = local_res10 + 1;
                        WVar2 = *local_res10;
                    }
                }
                uVar19 = 0x80004001;
                PECMD_GetApiProcCached("SetDeviceGammaRamp", "GDI32",
                                       (int64_t *)(void **)&g_pSetDeviceGammaRamp, &g_hGdi32);
                PECMD_GetApiProcCached("GetDeviceGammaRamp", "GDI32",
                                       (int64_t *)(void **)&g_pGetDeviceGammaRamp, &g_hGdi32);
                hDC = GetDC((HWND)0x0);
                FUN_1400633A8(&local_9c0, 0x600);
                puVar33 = local_9c0 + 0x100;
                if (cVar9 < '\x01') {
                    if ((cVar9 < '\x01') && (g_pSetDeviceGammaRamp != (void *)0x0)) {
                        iVar14 = 0;
                        lVar28 = 0x400 - (int64_t)puVar33;
                        puVar24 = local_9c0;
                        do {
                            lVar35 = (int64_t)((double)iVar14 * local_998 + g_dbl21bf0);
                            uVar12 = (uint16_t)lVar35;
                            if (0xffff < (uint32_t)lVar35) {
                                uVar12 = 0xffff;
                            }
                            iVar14 = iVar14 + 1;
                            *(uint16_t *)((int64_t)local_9c0 + lVar28 + (int64_t)puVar33) = uVar12;
                            *puVar33 = uVar12;
                            *puVar24 = uVar12;
                            puVar24 = puVar24 + 1;
                            puVar33 = puVar33 + 1;
                        } while (iVar14 < 0x100);
                        iVar14 = (*g_pSetDeviceGammaRamp)(hDC, local_9c0);
                        if (iVar14 == 1) {
                            uVar19 = 0;
                        }
                        else {
                            DVar32 = GetLastError();
                            uVar19 = (uint64_t)DVar32;
                            if (DVar32 == 0) {
                                uVar19 = 1;
                            }
                        }
                    }
                    if (*local_res10 == L'\0')
                        goto LAB_1400396a0;
                    PECMD_FreeStrBuf((WCHAR **)&local_9c0);
                    if (hDC != (HDC)0x0) {
                        ReleaseDC((HWND)0x0, hDC);
                    }
                    goto LAB_1400395a9;
                }
                lVar28 = 0;
                local_608[0] = L'\0';
                if (g_pGetDeviceGammaRamp == (void *)0x0)
                    goto LAB_140039748;
                iVar14 = (*g_pGetDeviceGammaRamp)(hDC, local_9c0);
                lVar35 = lVar28;
                if (iVar14 == 1) {
                    uVar19 = 0;
                    goto LAB_140039701;
                }
                uVar13 = GetLastError();
                if (uVar13 == 0) {
                    uVar13 = 1;
                }
                uVar19 = (uint64_t)uVar13;
                if (uVar13 == 0) {
                LAB_140039701:
                    do {
                        puVar1 = local_9c0 + lVar28;
                        lVar28 = lVar28 + 1;
                        lVar35 = lVar35 + (uint64_t)*puVar1;
                    } while (lVar28 < 0x300);
                    uVar22 = lVar35 + 1;
                    B2F_HI64(auVar4) = 0;
                    B2F_LO64(auVar4) = uVar22;
                    /* (uint64_t)(__uint128_t M * x) == low 64 bits == plain mul */
                    lVar28 = 0x56ac0156ac0156adULL * (uint64_t)B2F_LO64(auVar4);
                    wsprintfW(local_608, WSTR("%d"), (lVar28 + ((uVar22 - lVar28) >> 1)) >> 0x10);
                }
            LAB_140039748:
                FUN_1400629B8(script, local_a40, local_608);
            LAB_1400396a0:
                PECMD_FreeStrBuf((WCHAR **)&local_9c0);
                if (hDC != (HDC)0x0) {
                    ReleaseDC((HWND)0x0, hDC);
                }
                goto LAB_14003a7d7;
            }
            cVar9 = (char)local_res18;
            if (cVar38 < '\0') {
                cVar38 = '\0';
            }
        }
        else {
            cVar9 = (char)local_res18;
            if (cVar38 < '\0') {
                cVar38 = '\x01';
            }
        }
    }
    else {
        B2F_LO32(local_res18) = ((local_res18) & 0xffffff00u) | (uint32_t)(1);
        cVar9 = '\x01';
    }
LAB_1400395b0:
    cVar30 = (char)local_res20[0];
LAB_1400395b8:
    WVar2 = *local_res10;
    goto joined_r0x00014003919a;
LAB_140039630:
    for (;
         (WVar2 = *local_res10,
         WVar2 != L'\0' && ((((uint16_t)WVar2 < 9 || (0xd < (uint16_t)WVar2)) && (WVar2 != L' '))));
         local_res10 = local_res10 + 1) {
    }
    PECMD_SkipLeadingControls(&local_res10);
LAB_140039645:
    cVar9 = (char)local_res18;
    cVar30 = (char)local_res20[0];
LAB_140039657:
    uVar19 = 1;
    local_9b8 = -(uint32_t)(cVar9 != '\0') & 0x10000000;
    if (cVar30 != '\0') {
        local_9b8 = 0x40000000;
    }
    uVar27 = FUN_14005B1A8((uint16_t *)&g_u161bec, &local_res10, 1);
    if ((int)uVar27 == 0) {
        uVar27 = FUN_14005B1A8((uint16_t *)&g_u161be4, &local_res10, 2);
        cVar9 = '\0';
        if ((int)uVar27 != 0) {
            cVar9 = '\x02';
        }
    }
    else {
        cVar9 = '\x01';
    }
    local_a18 = 0xffffffff;
    local_3a8.cb = 0x348;
    memset((uint64_t *)local_3a8.DeviceName, 0, 0x344);
    local_990 = local_res10;
    local_9e8 = (WCHAR *)0x0;
    if (*local_res10 == L'=') {
        local_res10 = local_res10 + 1;
        local_990 = local_res10;
        PECMD_ParseNumber((int64_t *)&local_990, (int *)&local_a18);
        WVar2 = *local_res10;
        while (((WVar2 != L'\0' && (((uint16_t)WVar2 < 9 || (0xd < (uint16_t)WVar2)))) &&
                (WVar2 != L' '))) {
            local_res10 = local_res10 + 1;
            WVar2 = *local_res10;
        }
        PECMD_SkipLeadingControls(&local_res10);
        if ((int)(intptr_t)local_a18 < 0) {
            uVar19 = 0x80070057;
        }
        else {
            BVar16 = EnumDisplayDevicesW((LPCWSTR)0x0, local_a18, &local_3a8, 0);
            if (BVar16 != 0) {
                local_9e8 = local_3a8.DeviceName;
                goto LAB_14003989b;
            }
        }
    }
    else {
    LAB_14003989b:
        pWVar29 = local_9e8;
        if (cVar9 != '\0') {
            PECMD_SplitNextToken(script, (int64_t *)&local_res10, (int64_t *)&local_9c8, 0x2c, 0);
        }
        CharUpperW(local_res10);
        local_a48[0] = -1;
        local_a50 = 0xffffffff;
        uVar13 = 0;
        bVar8 = false;
        iVar15 = 0;
        local_a14 = 0;
        iVar14 = 100;
        FUN_1400633A8(&local_9d8, 0x328);
        pWVar20 = local_res10;
        local_a58 = (LPWSTR)0x0;
        local_9f8 = 0;
        local_a58 = (LPWSTR)PECMD_MatchKeywordToken((uint16_t *)local_res10, 0x53);
        if (local_a58 != (LPWSTR)0x0) {
            local_a48[0] = -1;
            local_a58 = local_a58 + 1;
            PECMD_ParseNumber((int64_t *)&local_a58, local_a48);
            if (local_a48[0] != -1) {
                uVar13 = 0x400;
            }
            if (pWVar20 < local_a58) {
                pWVar20 = local_a58;
            }
        }
        local_a58 = (LPWSTR)PECMD_MatchKeywordToken((uint16_t *)local_res10, 0x50);
        iVar34 = 0x84;
        if (((local_a58 != (LPWSTR)0x0) && (pWVar29 != (WCHAR *)0x0)) &&
            ((local_a48[0] == 0x84 && (uVar13 = uVar13 | 0x100, pWVar20 < local_a58)))) {
            pWVar20 = local_a58;
        }
        local_a58 = (LPWSTR)PECMD_MatchKeywordToken((uint16_t *)local_res10, 0x40);
        lVar28 = 0;
        if ((local_a58 != (LPWSTR)0x0) && ((local_a48[0] == iVar34 || (local_a48[0] < 1)))) {
            uVar13 = uVar13 | 0x200;
            do {
                local_a58 = local_a58 + 1;
                local_res20[0] = 0x80000000;
                local_9fc = -0x80000000;
                bVar10 = PECMD_ParseNumber((int64_t *)&local_a58, (int *)local_res20);
                if ((int)(uint64_t)(uint8_t)bVar10 < 1)
                    break;
                local_a58 = local_a58 + 1;
                PECMD_ParseNumber((int64_t *)&local_a58, &local_9fc);
                if (iVar15 <= iVar14) {
                    PECMD_GrowByteBuffer(&local_9d8, (int64_t)(iVar14 + 0x65) << 3);
                    local_9f8 = local_9d8;
                    iVar14 = iVar14 + 100;
                }
                local_a14 = iVar15 + 1;
                uVar13 = uVar13 | 0x100;
                *(uint32_t *)(local_9f8 + lVar28) = local_res20[0];
                lVar28 = lVar28 + 8;
                *(int *)(local_9f8 + -4 + lVar28) = local_9fc;
                iVar15 = local_a14;
            } while (*local_a58 == L':');
            if (pWVar20 < local_a58) {
                pWVar20 = local_a58;
            }
        }
        pWVar29 = local_9e8;
        lVar28 = 0;
        if ((*pWVar20 == L'\0') && (uVar13 == 0)) {
            bVar8 = true;
            uVar13 = 0x40;
        }
        local_9d0 = 0;
        local_res10 = pWVar20;
        if (cVar9 != '\x02') {
            local_9d0 = EnumDisplaySettingsW(local_9e8, 0xffffffff, &local_8a8);
        }
        memcpy(local_6e8, &local_8a8, 0xdc);
        uVar18 = local_a28;
        local_9f0 = 0;
        uVar19 = 0;
        if (cVar9 == '\x01') {
            pDVar37 = &local_8a8;
            goto LAB_140039bdb;
        }
        if ((bVar8) || (cVar9 != '\0')) {
            uVar19 = PECMD_EnumDisplayModes(
                &local_9f0, 1000, ((local_a00 & 0xffffu) | local_a24 | local_a28), pWVar29);
            uVar19 = uVar19 & 0xffffffff;
            lVar28 = local_9f0;
            for (local_a38 = 0; (int)local_a38 < (int)uVar19; local_a38 = local_a38 + 1) {
                uVar22 = local_a20 * local_a30;
                pDVar37 = (DEVMODEW *)((int64_t)(int)local_a38 * 0xdc + lVar28);
                uVar26 = (int64_t)(int)pDVar37->dmPelsWidth * (int64_t)(int)pDVar37->dmPelsHeight &
                         0xffffffff;
                if (cVar9 == '\x02') {
                    if (*local_9e0 != L'\0') {
                        FUN_14006375C((int64_t *)&local_9e0, WSTR("\r\n"));
                    }
                LAB_140039bdb:
                    if (local_a24 == 0) {
                        pwVar25 = WSTR("%d\t%d\t%d\t%d");
                        if (uVar18 != 0) {
                            pwVar25 = WSTR("%d\t%d");
                        }
                    LAB_140039c58:
                        wsprintfW(local_4e8, pwVar25, (uint64_t)pDVar37->dmPelsWidth,
                                  pDVar37->dmPelsHeight);
                    }
                    else {
                        pwVar25 = WSTR("%d\t%d\t%d\t%d\t%d");
                        if (local_a28 == 0) {
                            uVar18 = 0;
                            goto LAB_140039c58;
                        }
                        wsprintfW(local_4e8, WSTR("%d\t%d\t%d"), (uint64_t)pDVar37->dmPelsWidth,
                                  pDVar37->dmPelsHeight);
                        uVar18 = local_a28;
                    }
                    FUN_14006375C((int64_t *)&local_9e0, local_4e8);
                    if (cVar9 == '\x01')
                        break;
                }
                else if (uVar22 < uVar26 || uVar22 - uVar26 == 0) {
                    if (uVar22 * local_a08 * local_a10 < (uint64_t)pDVar37->dmBitsPerPel *
                                                             (uint64_t)pDVar37->dmDisplayFrequency *
                                                             uVar26) {
                        local_a30 = (uint64_t)pDVar37->dmPelsWidth;
                        local_a20 = (uint64_t)pDVar37->dmPelsHeight;
                        local_a10 = (uint64_t)pDVar37->dmDisplayFrequency;
                        local_a08 = (uint64_t)pDVar37->dmBitsPerPel;
                    }
                }
            }
            uVar19 = local_a20;
            uVar22 = local_a30;
            if ((cVar9 != '\0') || (local_a30 != 0)) {
                local_638 = (uint32_t)local_a20;
                local_640 = (uint32_t)local_a08;
                local_630 = (uint32_t)local_a10;
                local_63c = (uint32_t)local_a30;
                if (cVar9 == '\0') {
                    PECMD_TlsLogWrite(script, WSTR("autodisp: w=%d h=%d r=%d f=%d\r\n"),
                                      local_a30 & 0xffffffff, local_a20 & 0xffffffff);
                    goto LAB_140039e25;
                }
                FUN_1400629B8(script, local_9c8, local_9e0);
                goto LAB_14003a7b7;
            }
            PECMD_FreeStrBuf(&local_9f0);
            PECMD_FreeStrBuf(&local_9d8);
            uVar19 = 1;
        }
        else {
            local_a10 = (uint64_t)local_630;
            uVar19 = (uint64_t)local_638;
            uVar22 = (uint64_t)local_63c;
            local_a08 = (uint64_t)local_640;
            local_a30 = uVar22;
            local_a20 = uVar19;
        LAB_140039e25:
            if ((local_a48[0] != -1) && (g_pSetDisplayConfig != (void *)0x0)) {
                (*g_pSetDisplayConfig)(0, 0, 0);
            }
            local_a58 = StrChrW(local_res10, L'W');
            if (local_a58 != (LPWSTR)0x0) {
                local_a50 = 0xffffffff;
                local_a58 = local_a58 + 1;
                PECMD_ParseIntRound((int64_t *)&local_a58, (int *)&local_a50);
                if (0 < (int)local_a50) {
                    uVar13 = uVar13 | 1;
                    local_63c = local_a50;
                    uVar22 = (uint64_t)local_a50;
                    local_a30 = (uint64_t)local_a50;
                }
            }
            local_a58 = StrChrW(local_res10, L'H');
            uVar18 = (uint32_t)uVar19;
            if (local_a58 != (LPWSTR)0x0) {
                local_a50 = 0xffffffff;
                local_a58 = local_a58 + 1;
                PECMD_ParseIntRound((int64_t *)&local_a58, (int *)&local_a50);
                if (0 < (int)local_a50) {
                    uVar13 = uVar13 | 2;
                    local_638 = local_a50;
                    local_a20 = (uint64_t)local_a50;
                    uVar18 = local_a50;
                }
            }
            local_a58 = StrChrW(local_res10, L'B');
            if (local_a58 != (LPWSTR)0x0) {
                local_a50 = 0xffffffff;
                local_a58 = local_a58 + 1;
                PECMD_ParseIntRound((int64_t *)&local_a58, (int *)&local_a50);
                if (0 < (int)local_a50) {
                    uVar13 = uVar13 | 4;
                    local_640 = local_a50;
                    local_a08 = (uint64_t)local_a50;
                }
            }
            local_a58 = StrChrW(local_res10, L'F');
            if (local_a58 != (LPWSTR)0x0) {
                local_a50 = 0xffffffff;
                local_a58 = local_a58 + 1;
                PECMD_ParseIntRound((int64_t *)&local_a58, (int *)&local_a50);
                if (1 < (int)local_a50) {
                    local_a10 = (uint64_t)local_a50;
                    uVar13 = uVar13 | 8;
                    local_630 = local_a50;
                }
            }
            local_a58 = StrChrW(local_res10, L'T');
            if ((local_a58 != (LPWSTR)0x0) && (uVar13 != 0)) {
                local_a58 = local_a58 + 1;
                B2F_LO32(local_res18) = 0;
                StrToIntExW(local_a58, 0, (int *)&local_res18);
                FUN_1400195F0(script, (int64_t)(int)(uint32_t)local_res18, 0, (uint64_t *)0x0);
            }
            local_a58 = StrChrW(local_res10, L'O');
            if (local_a58 == (LPWSTR)0x0) {
                local_a58 = (LPWSTR)0x0;
            }
            else {
                local_a50 = 0xffffffff;
                local_a58 = local_a58 + 1;
                PECMD_ParseIntRound((int64_t *)&local_a58, (int *)&local_a50);
                if (-1 < (int)local_a50) {
                    uVar13 = uVar13 | 0x10;
                    local_694 = local_a50;
                }
            }
            uVar39 = local_694;
            DVar32 = local_8a8.dmPelsHeight;
            uVar19 = 0;
            bVar8 = false;
            if (uVar13 == 0) {
            LAB_14003a7b7:
                PECMD_FreeStrBuf(&local_9f0);
                PECMD_FreeStrBuf(&local_9d8);
                uVar19 = 0;
            }
            else {
                local_a24 = 0;
                B2F_LO32(local_res18) = 0;
                local_a38 = 0;
                pWVar29 = local_9e8;
                cVar9 = local_a4c;
                if ((char)uVar13 != '\0') {
                    bVar10 = bVar8;
                    if (((uint32_t)uVar22 == local_8a8.dmPelsWidth) &&
                        (uVar18 == local_8a8.dmPelsHeight)) {
                        bVar10 = true;
                    }
                    bVar40 = (DWORD)local_a08 != local_8a8.dmBitsPerPel;
                    if ((((uVar13 & 8) == 0) || ((uint32_t)local_a10 == 0)) ||
                        ((uint32_t)local_a10 == local_8a8.dmDisplayFrequency)) {
                        bVar8 = true;
                    }
                    memcpy(&local_988, local_6e8, 0xdc);
                    pWVar29 = local_9e8;
                    if ((uVar13 & 8) == 0) {
                    LAB_14003a144:
                        uVar18 = 0;
                    }
                    else {
                        uVar18 = 0x400000;
                        if ((uint32_t)local_a10 < 2)
                            goto LAB_14003a144;
                    }
                    local_988.dmFields = uVar18 | (uVar13 & 0x10) * 8 | (uVar13 & 4) << 0x10 |
                                         (uVar13 & 2) * 0x80000 | (uVar13 & 1) * 0x80000;
                    if (local_988.dmFields == 0) {
                        local_988.dmFields = 0x1c0000;
                    }
                    uVar31 = (uint32_t)local_a20;
                    uVar18 = (uint32_t)local_a30;
                    if ((uVar13 & 0x10) != 0) {
                        if ((((uVar39 & 1) !=
                              (local_8a8.field6_0x4c.field1.dmDisplayOrientation & 1)) &&
                             ((uVar13 & 1) == 0)) &&
                            ((uVar13 & 2) == 0)) {
                            local_988.dmFields = local_988.dmFields | 0x180000;
                            local_988.dmPelsWidth = DVar32;
                            local_988.dmPelsHeight = local_8a8.dmPelsWidth;
                        }
                        if ((((uVar39 & 1) != 0) && ((uVar13 & 1) != 0)) && ((uVar13 & 2) != 0)) {
                            local_988.dmPelsWidth = uVar31;
                            local_988.dmPelsHeight = uVar18;
                        }
                    }
                    uVar39 = local_9b8 | 1;
                    LVar17 = ChangeDisplaySettingsExW(local_9e8, &local_988, (HWND)0x0, uVar39,
                                                      (LPVOID)0x0);
                    if (LVar17 == 0) {
                        uVar19 = (uint64_t)(uint32_t)local_res18;
                    LAB_14003a352:
                        uVar22 = local_a30;
                        iVar15 = local_a14;
                        cVar9 = '\x01';
                    }
                    else {
                        if ((!bVar10) || (bVar40)) {
                            memcpy(&local_988, &local_8a8, 0xdc);
                            local_988.dmBitsPerPel = (DWORD)local_a08;
                            local_988.dmFields = 0x1c0000;
                            local_988.dmPelsWidth = uVar18;
                            local_988.dmPelsHeight = uVar31;
                            local_a38 = ChangeDisplaySettingsExW(pWVar29, &local_988, (HWND)0x0,
                                                                 uVar39, (LPVOID)0x0);
                            if (local_a38 != 0)
                                goto LAB_14003a2b4;
                            memcpy(&local_8a8, &local_988, 0xdc);
                        }
                        else {
                        LAB_14003a2b4:
                            if (!bVar10) {
                                memcpy(&local_988, &local_8a8, 0xdc);
                                local_988.dmPelsHeight = (DWORD)local_a20;
                                local_988.dmFields = 0x180000;
                                local_988.dmPelsWidth = uVar18;
                                local_a38 = ChangeDisplaySettingsExW(pWVar29, &local_988, (HWND)0x0,
                                                                     uVar39, (LPVOID)0x0);
                                if (local_a38 != 0)
                                    goto LAB_14003a370;
                                memcpy(&local_8a8, &local_988, 0xdc);
                                if (!bVar40)
                                    goto LAB_14003a3f0;
                                if (bVar8)
                                    goto LAB_14003a370;
                            LAB_14003a349:
                                uVar19 = (uint64_t)(uint32_t)local_res18;
                                goto LAB_14003a352;
                            }
                        LAB_14003a370:
                            if (bVar40) {
                                memcpy(&local_988, &local_8a8, 0xdc);
                                local_988.dmBitsPerPel = (DWORD)local_a08;
                                local_988.dmFields = 0x40000;
                                local_a24 = ChangeDisplaySettingsExW(pWVar29, &local_988, (HWND)0x0,
                                                                     uVar39, (LPVOID)0x0);
                                if ((local_a24 == 0) &&
                                    (memcpy(&local_8a8, &local_988, 0xdc), !bVar8))
                                    goto LAB_14003a349;
                            }
                        }
                    LAB_14003a3f0:
                        if ((bVar8) || (uVar18 = (uint32_t)local_a10, uVar18 < 2)) {
                            uVar19 = (uint64_t)(uint32_t)local_res18;
                        }
                        else {
                            memcpy(&local_988, &local_8a8, 0xdc);
                            local_988.dmFields = 0x400000;
                            local_988.dmDisplayFrequency = uVar18;
                            uVar18 = ChangeDisplaySettingsExW(pWVar29, &local_988, (HWND)0x0,
                                                              uVar39, (LPVOID)0x0);
                            uVar19 = (uint64_t)uVar18;
                            if (uVar18 == 0)
                                goto LAB_14003a352;
                        }
                        local_8a8.dmFields =
                            ((-(uint32_t)(1 < local_8a8.dmDisplayFrequency) & 0x400000u) |
                             0x1c0000u);
                        uVar22 = local_a30;
                        iVar15 = local_a14;
                        cVar9 = local_a4c;
                        if (local_9d0 != 0) {
                            ChangeDisplaySettingsExW(pWVar29, &local_8a8, (HWND)0x0, uVar39,
                                                     (LPVOID)0x0);
                            uVar22 = local_a30;
                            iVar15 = local_a14;
                            cVar9 = local_a4c;
                        }
                    }
                }
                if (local_a24 != 0) {
                    uVar19 = (uint64_t)local_a24;
                }
                if (local_a38 != 0) {
                    uVar19 = (uint64_t)local_a38;
                }
                DVar32 = 0;
                B2F_LO32(local_res18) = (uint32_t)uVar19;
                local_7c8.dmDeviceName[0] = L'\0';
                memset((uint64_t *)(local_7c8.dmDeviceName + 1), 0, 0x3e);
                memset((uint64_t *)&local_7c8.dmSpecVersion, 0, 0x9c);
                local_5c8.dmDeviceName[0] = L'\0';
                memset((uint64_t *)(local_5c8.dmDeviceName + 1), 0, 0x3e);
                memset((uint64_t *)&local_5c8.dmSpecVersion, 0, 0x9c);
                lVar28 = local_9f8;
                if (((uVar13 & 0xf00) != 0) && ((local_a48[0] == 0x84 || (0 < iVar15)))) {
                    local_7c8.field6_0x4c.field1.dmPosition.y = 0;
                    local_7c8.dmSize = 0xdc;
                    local_7c8.field6_0x4c.field1.dmPosition.x = 0;
                    if ((0 < iVar15) &&
                        (((int)(intptr_t)local_a18 < iVar15 && (-1 < (int)(intptr_t)local_a18)))) {
                        local_7c8.field6_0x4c.field1.dmPosition.x =
                            *(LONG *)(local_9f8 + (int64_t)(int)(intptr_t)local_a18 * 8);
                        local_7c8.field6_0x4c.field1.dmPosition.y =
                            *(LONG *)(local_9f8 + 4 + (int64_t)(int)(intptr_t)local_a18 * 8);
                    }
                    local_7c8.dmFields = 0x20;
                    if ((uVar13 >> 8 & 1) != 0) {
                        ChangeDisplaySettingsExW(pWVar29, &local_7c8, (HWND)0x0, 0x11, (LPVOID)0x0);
                    }
                    pWVar29 = local_9e8;
                    if (local_a48[0] == 0x84) {
                        local_7c8.field6_0x4c.field1.dmPosition.x = (LONG)uVar22;
                    }
                    iVar14 = 6;
                    if (0 < iVar15) {
                        iVar14 = iVar15;
                    }
                    if ((local_a48[0] == 0x84) || (0 < iVar15)) {
                        lVar35 = 0;
                        if (0 < (int64_t)iVar14) {
                            do {
                                BVar16 = EnumDisplayDevicesW((LPCWSTR)0x0, DVar32, &local_3a8, 0);
                                if (BVar16 != 0) {
                                    if ((int64_t)iVar15 < 1) {
                                        if (DVar32 == local_a18)
                                            goto LAB_14003a704;
                                    }
                                    else {
                                        if (iVar15 <= lVar35)
                                            break;
                                        local_7c8.field6_0x4c.field1.dmPosition.x =
                                            *(LONG *)(lVar28 + lVar35 * 8);
                                        local_7c8.field6_0x4c.field1.dmPosition.y =
                                            *(LONG *)(lVar28 + 4 + lVar35 * 8);
                                    }
                                    ChangeDisplaySettingsExW(local_3a8.DeviceName, &local_7c8,
                                                             (HWND)0x0, 9, (LPVOID)0x0);
                                    if (local_a48[0] == 0x84) {
                                        local_5c8.dmPelsWidth = 0;
                                        EnumDisplaySettingsW(pWVar29, 0xffffffff, &local_5c8);
                                        local_7c8.field6_0x4c.field1.dmPosition.x =
                                            local_5c8.dmPelsWidth;
                                    }
                                }
                            LAB_14003a704:
                                lVar35 = lVar35 + 1;
                                DVar32 = DVar32 + 1;
                            } while (lVar35 < iVar14);
                        }
                        uVar19 = (uint64_t)(uint32_t)local_res18;
                    }
                    ChangeDisplaySettingsExW((LPCWSTR)0x0, (DEVMODEW *)0x0, (HWND)0x0, 0,
                                             (LPVOID)0x0);
                    cVar9 = '\x02';
                }
                if ('\0' < cVar9) {
                    PECMD_StartWorkerThread(script, (void **)0x0, 0x1a, 0, 0, 0x22, 5000, 1, 9000);
                }
                PECMD_FreeStrBuf(&local_9f0);
                PECMD_FreeStrBuf(&local_9d8);
            }
        }
    }
LAB_14003a7d7:
    PECMD_FreeStrBuf((WCHAR **)&local_9e0);
    ppWVar23 = &local_9c8;
LAB_14003a7ec:
    PECMD_FreeStrBuf((WCHAR **)&ppWVar23);
    return uVar19 & 0xffffffff;
}

/* ========== FUN_14003B540 @0x14003b540 ========== */

uint64_t FUN_14003B540(LPCWSTR path, uint64_t size, uint64_t offset, uint8_t mode, LPCWSTR name)

{
    WCHAR *pwVar1;
    uint32_t uVar2;
    uint64_t *lpParameter;
    LPWSTR pWVar3;
    uint8_t bVar4;
    int iVar5;
    DWORD DVar6;
    BOOL BVar7;
    int64_t lVar8;
    LPSTR pCVar10;
    LPWSTR pWVar11;
    HANDLE pvVar12;
    WCHAR *lpBuffer;
    LPCWSTR pWVar13;
    WCHAR *pwVar14;
    uint64_t uVar15;
    WCHAR *pwVar16;
    int local_res20;
    char local_318;
    HANDLE local_310;
    WCHAR *local_308;
    WCHAR *local_300;
    LPWSTR local_2f8;
    uint64_t *local_2f0;
    LPWSTR local_2e8;
    WCHAR *local_2e0;
    LPCWSTR local_2d8;
    WCHAR *local_2d0;
    HANDLE local_2c8;
    HANDLE local_2c0;
    uint64_t local_2b8;
    LPWSTR local_2b0;
    WCHAR *local_2a8;
    uint64_t local_2a0;
    WCHAR *local_298;
    WCHAR *local_290;
    uint64_t local_288;
    WIN32_FIND_DATAW local_278;
    uint64_t uVar9;

    local_318 = '\x01';
    local_2d0 = (WCHAR *)PECMD_WideStrLen(path);
    uVar15 = 0;
    iVar5 = (int)(intptr_t)local_2d0;
    local_2c0 = (HANDLE)(int64_t)(iVar5 + 0x20d);
    if (((mode & 0xf0) == 0) && (4 < iVar5)) {
        iVar5 = lstrcmpiW(path + (int64_t)iVar5 + -4, WSTR(".INF"));
        local_2f0 = (uint64_t *)(((uint64_t)B2F_HI32(local_2f0) << 32) | 1ULL);
        if (iVar5 != 0)
            goto LAB_14003b5c1;
    }
    else {
    LAB_14003b5c1:
        local_2f0 = (uint64_t *)((uint64_t)local_2f0 & 0xffffffff00000000);
    }
    pvVar12 = local_2c0;
    bVar4 = *(uint8_t *)(size + 0x1b0) & 2;
    PECMD_AllocWStringBuffer((WCHAR **)&local_290, (int64_t)((int)local_2c0 + 0x325));
    lpBuffer = (WCHAR *)(local_290 + (int64_t)pvVar12);
    local_2d8 = local_290;
    if (bVar4 != 0) {
        PECMD_AllocWStringBuffer((WCHAR **)&local_2f8, 0x1000);
        wsprintfW(local_2f8, WSTR("DeviDir....Begin:%s"));
        FUN_140025f10(size + 8, local_2f8, 0, (WCHAR *)0x1100, (WCHAR *)0x0, (int64_t *)0x0);
        PECMD_FreeStrBuf((WCHAR **)&local_2f8);
    }
    if (*path == L'\\') {
        GetLogicalDriveStringsW(0x324, (LPWSTR)lpBuffer);
        for (; (uint16_t)*lpBuffer != 0;
             lpBuffer = (WCHAR *)((uint8_t *)lpBuffer + ((int)lVar8 + 1) * 2)) {
            PECMD_CrtShim(local_290, 0x140121598, lpBuffer, (WCHAR *)(path + 1));
            FUN_14003B540(local_290, size, offset, mode, (LPCWSTR)0x0);
            lVar8 = PECMD_WideStrLen(lpBuffer);
        }
    }
    else {
        iVar5 = (int)(intptr_t)local_2d0;
        local_2b8 = (uint64_t)(iVar5 + 1);
        lVar8 = local_2b8 * 2;
        memcpy((uint8_t *)local_290, (uint8_t *)path, (int)lVar8);
        local_2a0 = (uint64_t)iVar5;
        local_2e0 = local_290 + local_2a0;
        local_2e0[0] = L'\0';
        local_2e0[1] = L'\0';
        local_2e0[2] = L'\0';
        local_2e0[3] = L'\0';
        if ((int)(intptr_t)local_2f0 == 0) {
            if ((mode & 0xf) == 0) {
                memcpy((uint8_t *)local_2e0, (uint8_t *)WSTR("\\INF\\*.INF"), 0x16);
                uVar9 = (int64_t)(iVar5 + 5);
            }
            else {
                local_290[local_2a0 + 2] = L'\0';
                local_290[local_2a0 + 1] = L'\\';
                *local_2e0 = L'\\';
                if (name == (LPCWSTR)0x0) {
                    memcpy((uint8_t *)(lVar8 + (int64_t)local_290),
                           (uint8_t *)WSTR("!!!!!!!![allinf-file]"), 0x2c);
                    name = local_290;
                }
                local_2c8 = (HANDLE)0x0;
                PECMD_OpenFileHandle(&local_2c8, name, 0x80000000, 7, (LPSECURITY_ATTRIBUTES)0x0, 3,
                                     0, (HANDLE)0x0);
                pvVar12 = local_2c8;
                if (local_2c8 != (HANDLE)0x0) {
                    PECMD_AllocWStringBuffer((WCHAR **)&local_300, 0x2800);
                    _snwprintf(local_300, 0x27ff, WSTR("Found Index<%s>"), name);
                    FUN_140025f10(size + 8, local_300, 0, (WCHAR *)(void *)0x11, (WCHAR *)0x0,
                                  (int64_t *)0x0);
                    PECMD_FreeStrBuf((WCHAR **)&local_300);
                    uVar2 = 0;
                    DVar6 = FUN_1400E693C(pvVar12);
                    uVar9 = (uint64_t)(uint32_t)DVar6;
                    local_2b8 = uVar9;
                    local_2a8 = (WCHAR *)PECMD_MapFileView(pvVar12, uVar9, 8, 0);
                    if (local_2a8 != (WCHAR *)0x0) {
                        pwVar16 = (WCHAR *)(uVar9 + (int64_t)local_2a8);
                        local_288 =
                            (((int64_t)(uVar9 + 3 +
                                        (uint64_t)((uint32_t)((int64_t)(uVar9 + 3) >> 0x3f) & 3)) >>
                              2) +
                             0xffffU) &
                            0xffffffffffff0000ULL;
                        pwVar14 = local_2a8;
                        if ((*(uint32_t *)local_2a8 & 0xffffff) == 0xbfbbef) {
                            pwVar14 = (WCHAR *)((int64_t)local_2a8 + 3);
                        }
                    LAB_14003b8f2:
                        while (((uint8_t)*pwVar14 == 0x23 && (pwVar14 < pwVar16))) {
                            do {
                                if (((uint8_t)*pwVar14 == 10) || ((uint8_t)*pwVar14 == 0xd))
                                    goto LAB_14003b8ed;
                                pwVar14 = (WCHAR *)((int64_t)pwVar14 + 1);
                            } while (pwVar14 < pwVar16);
                        }
                        local_298 = (WCHAR *)0x0;
                        FUN_1400633A8(&local_2b0, 0x30);
                        PECMD_ZeroLenBuf((uint64_t *)local_2b0);
                        uVar9 = local_2b8;
                        local_2e0 = (WCHAR *)((int64_t)pwVar16 + -1);
                        bVar4 = *(uint8_t *)local_2e0;
                        local_2e8 = local_2b0;
                        local_300 = (WCHAR *)0x0;
                        if (((bVar4 < 9) || (0xd < bVar4)) &&
                            ((bVar4 != 0x20 &&
                              (pwVar16 = local_2e0, *(uint8_t *)local_2e0 != 0x23)))) {
                            for (; pwVar14 < pwVar16; pwVar16 = (WCHAR *)((int64_t)pwVar16 + -1)) {
                                if (((uint8_t)*pwVar16 == 10) || ((uint8_t)*pwVar16 == 0xd)) {
                                    pwVar1 = (WCHAR *)((int64_t)pwVar16 + 1);
                                    if ((*(uint8_t *)pwVar1 == 0x23) &&
                                        (*(uint8_t *)pwVar16 = 0, local_300 = pwVar1,
                                         local_2e0 = pwVar16, pwVar1 != (WCHAR *)0x0))
                                        goto LAB_14003b9c8;
                                    break;
                                }
                            }
                            PECMD_GrowByteBuffer(&local_298, local_2b8 + 0x10);
                            local_2e0 = (WCHAR *)(uVar9 + (int64_t)local_298);
                            local_2e0[0] = L'\0';
                            local_2e0[1] = L'\0';
                            local_2e0[2] = L'\0';
                            local_2e0[3] = L'\0';
                            pwVar14 = local_298;
                        }
                        else {
                            *(uint8_t *)local_2e0 = 0;
                        }
                    LAB_14003b9c8:
                        pWVar13 = local_2d8;
                        local_res20 = 0;
                        local_2f8 = (LPWSTR)0x0;
                        do {
                            pwVar1 = local_2e0;
                            pWVar3 = local_2e8;
                            pWVar11 = local_2f8;
                            pwVar16 = local_300;
                            if (pwVar14 < local_2e0) {
                                local_308 = (WCHAR *)(local_288 + (int64_t)pwVar14);
                                pwVar16 = pwVar14;
                                if ((local_308 < local_2e0) &&
                                    (pCVar10 = StrStrA((LPCSTR)local_308, "\n<INFFILE>"),
                                     local_308 = pwVar1, pCVar10 != (LPSTR)0x0)) {
                                    *pCVar10 = '\0';
                                    local_308 = (WCHAR *)(pCVar10 + 1);
                                }
                            }
                            else {
                                if ((local_300 == (WCHAR *)0x0) || ((uint8_t)*local_300 == 0x23)) {
                                    if (0 < (int64_t)local_2f8) {
                                        do {
                                            WaitForSingleObject(
                                                *(HANDLE *)(*(int64_t *)(pWVar3 + uVar15 * 4) +
                                                            0x10),
                                                0xffffffff);
                                            if (*(int64_t *)(*(int64_t *)(pWVar3 + uVar15 * 4) +
                                                             0x20) != 0) {
                                                uVar2 = *(
                                                    uint32_t *)(*(int64_t *)(pWVar3 + uVar15 * 4) +
                                                                0x20);
                                            }
                                            uVar15 = uVar15 + 1;
                                        } while ((int64_t)uVar15 < (int64_t)pWVar11);
                                    }
                                    pvVar12 = local_2c8;
                                    for (lVar8 = (int64_t)(local_res20 + -1); -1 < lVar8;
                                         lVar8 = lVar8 + -1) {
                                        PECMD_HeapFreeWithHeader(
                                            (int64_t *)(*(int64_t *)(pWVar3 + lVar8 * 4) + 0x28));
                                        local_2a0 = *(uint64_t *)(pWVar3 + lVar8 * 4);
                                        pWVar11 = pWVar3 + lVar8 * 4;
                                        pWVar11[0] = L'\0';
                                        pWVar11[1] = L'\0';
                                        pWVar11[2] = L'\0';
                                        pWVar11[3] = L'\0';
                                        PECMD_FreeStrBuf((WCHAR **)&local_2a0);
                                    }
                                    if (local_318 != '\0') {
                                        uVar2 = (int)local_318;
                                    }
                                    *(uint64_t *)(size + 0x1a8) = (uint64_t)uVar2;
                                    PECMD_FreeStrBuf((WCHAR **)&local_2b0);
                                    PECMD_FreeStrBuf((WCHAR **)&local_298);
                                    UnmapViewOfFile(local_2a8);
                                    if (pvVar12 != (HANDLE)0xffffffffffffffff) {
                                        CloseHandle(pvVar12);
                                    }
                                    uVar15 = (uint64_t)uVar2;
                                    goto LAB_14003c048;
                                }
                                local_300 = (WCHAR *)0x0;
                                local_308 = pwVar1;
                            }
                            FUN_1400633A8(&local_2f0, 0x30);
                            PECMD_ZeroLenBuf(local_2f0);
                            lpParameter = local_2f0;
                            *local_2f0 = size;
                            PECMD_AllocString((WCHAR **)(lpParameter + 5), (int64_t)local_2c0 + 99);
                            memcpy((uint8_t *)lpParameter[5], (uint8_t *)pWVar13,
                                   ((int)(intptr_t)local_2d0 + 0x17) * 2);
                            lpParameter[4] = local_2a0;
                            lpParameter[1] = (uint64_t)pwVar16;
                            pvVar12 = CreateThread((LPSECURITY_ATTRIBUTES)0x0, 0x20000,
                                                   PECMD_DeviExtractSchedulerProc, lpParameter,
                                                   0x10004, (DWORD *)&g_dwC96C);
                            lpParameter[2] = (uint64_t)pvVar12;
                            if (pvVar12 == (HANDLE)0x0) {
                                PECMD_HeapFreeWithHeader((int64_t *)(lpParameter + 5));
                            }
                            else {
                                local_res20 = local_res20 + 1;
                                *(uint64_t **)(local_2e8 + (int64_t)local_2f8 * 4) = local_2f0;
                                local_2f8 = (LPWSTR)((int64_t)local_2f8 + 1);
                                local_2f0 = (uint64_t *)0x0;
                                local_318 = '\0';
                                ResumeThread(pvVar12);
                            }
                            pwVar14 = local_308;
                            PECMD_FreeStrBuf((WCHAR **)&local_2f0);
                        } while (true);
                    }
                    if (pvVar12 != (HANDLE)0xffffffffffffffff) {
                        CloseHandle(pvVar12);
                    }
                    uVar15 = 1;
                    goto LAB_14003c048;
                }
                memcpy((uint8_t *)local_2e0, (uint8_t *)WSTR("\\*.INF"), 0x16);
                local_2c8 = (HANDLE)0x0;
                uVar9 = local_2b8;
            }
        }
        else {
            pWVar11 = StrRChrW(path, (LPCWSTR)0x0, L'\\');
            uVar9 = uVar15;
            if (pWVar11 != (LPWSTR)0x0) {
                uVar9 = ((int64_t)pWVar11 + (2 - (int64_t)path)) >> 1;
            }
        }
        local_278.dwFileAttributes = 0;
        memset((uint64_t *)&local_278.ftCreationTime, 0, 0x24c);
        pWVar13 = local_2d8;
        local_310 = (HANDLE)0x0;
        PECMD_FindFirstFileW(&local_310, local_2d8, &local_278);
        local_2a8 = (WCHAR *)(int64_t)(int)uVar9;
        if (local_310 != (HANDLE)0x0) {
            do {
                if (*(int *)(size + 0x1a0) < 0)
                    break;
                iVar5 = lstrlenW(local_278.cFileName);
                FUN_1400633A8(&local_308, ((int64_t)iVar5 + uVar9) * 2 + 0xf6);
                PECMD_ZeroLenBuf((uint64_t *)local_308);
                *(uint64_t *)local_308 = size;
                local_2d0 = local_308;
                *(WCHAR **)(local_308 + 0x14) = local_308 + 0x18;
                pwVar14 = local_308;
                memcpy((uint8_t *)(local_308 + 0x18), (uint8_t *)pWVar13, ((int)uVar9 + 0x17) * 2);
                *(WCHAR **)(pwVar14 + 0x10) = local_2a8;
                *(short *)(*(uint64_t *)(pwVar14 + 0x14) + 2 + (int64_t)local_2a8 * 2) =
                    (short)iVar5;
                memcpy((uint8_t *)(*(uint64_t *)(pwVar14 + 0x14) + 0x4e + uVar9 * 2),
                       (uint8_t *)local_278.cFileName, (iVar5 + 1) * 2);
                local_318 = '\0';
                local_2c0 = CreateThread((LPSECURITY_ATTRIBUTES)0x0, 0x20000,
                                         PECMD_DeviSubPackageWorkerProc, pwVar14, 0x10004,
                                         (DWORD *)&g_dwC96C);
                if (local_2c0 != (HANDLE)0x0) {
                    local_308 = (WCHAR *)0x0;
                    EnterCriticalSection((LPCRITICAL_SECTION)&g_csInit);
                    *(int *)(*(uint64_t *)local_2d0 + 0x1a4) =
                        *(int *)(*(uint64_t *)local_2d0 + 0x1a4) + 1;
                    LeaveCriticalSection((LPCRITICAL_SECTION)&g_csInit);
                    pvVar12 = CreateEventW((LPSECURITY_ATTRIBUTES)0x0, 1, 0, (LPCWSTR)0x0);
                    *(HANDLE *)(local_2d0 + 0xc) = pvVar12;
                    ResumeThread(local_2c0);
                    CloseHandle(local_2c0);
                    WaitForSingleObject(pvVar12, 0xffffffff);
                    CloseHandle(pvVar12);
                }
                PECMD_FreeStrBuf((WCHAR **)&local_308);
                BVar7 = FindNextFileW(local_310, &local_278);
            } while (BVar7 != 0);
            pWVar13 = local_2d8;
            if ((local_310 != (HANDLE)0x0) && (local_310 != (HANDLE)0xffffffffffffffff)) {
                FindClose(local_310);
                pWVar13 = local_2d8;
            }
        }
        local_310 = (HANDLE)0x0;
        if ((mode & 0xf) == 1) {
            memcpy((uint8_t *)local_2e0, (uint8_t *)WSTR("\\*.*"), 0xc);
            local_278.cFileName[2] = L'\0';
            PECMD_FindFirstFileW(&local_310, pWVar13, &local_278);
            if (local_310 != (HANDLE)0x0) {
                do {
                    if (*(int *)(size + 0x1a0) < 0)
                        break;
                    if (((local_278.dwFileAttributes & 0x10) != 0) &&
                        ((local_278.cFileName[0] != L'.' ||
                          ((local_278.cFileName[1] != L'\0' &&
                            ((local_278.cFileName[1] != L'.' ||
                              (local_278.cFileName[2] != L'\0')))))))) {
                        iVar5 = lstrlenW(local_278.cFileName);
                        memcpy((uint8_t *)(pWVar13 + local_2b8), (uint8_t *)local_278.cFileName,
                               (iVar5 + 1) * 2);
                        FUN_14003B540(pWVar13, size, offset, mode, (LPCWSTR)0x0);
                        local_318 = '\0';
                        local_278.cFileName[2] = L'\0';
                    }
                    BVar7 = FindNextFileW(local_310, &local_278);
                } while (BVar7 != 0);
                if (bVar4 != 0) {
                    PECMD_AllocWStringBuffer((WCHAR **)&local_2e8, 0x1000);
                    wsprintfW(local_2e8, WSTR("DeviDir....End:%s"));
                    FUN_140025f10(size + 8, local_2e8, 0, (WCHAR *)0x1100, (WCHAR *)0x0,
                                  (int64_t *)0x0);
                    PECMD_FreeStrBuf((WCHAR **)&local_2e8);
                }
                if ((local_310 != (HANDLE)0x0) && (local_310 != (HANDLE)0xffffffffffffffff)) {
                    FindClose(local_310);
                }
                local_310 = (HANDLE)0x0;
                uVar15 = (uint64_t)(-(uint32_t)(local_318 != '\0') & (int)local_318);
                goto LAB_14003c048;
            }
        }
        local_310 = (HANDLE)0x0;
    }
LAB_14003c048:
    PECMD_FreeStrBuf((WCHAR **)&local_290);
    return uVar15;
LAB_14003b8ed:
    for (; (pwVar14 < pwVar16 && (((uint8_t)*pwVar14 == 10 || ((uint8_t)*pwVar14 == 0xd))));
         pwVar14 = (WCHAR *)((int64_t)pwVar14 + 1)) {
    }
    goto LAB_14003b8f2;
}

/* ========== FUN_14003C06C @0x14003c06c ========== */

LARGE_INTEGER FUN_14003C06C(int64_t *script, LARGE_INTEGER cmd, uint32_t flags)

{
    uint16_t uVar1;
    bool bVar2;
    bool bVar3;
    bool bVar4;
    bool bVar5;
    bool bVar6;
    char cVar7;
    uint8_t uVar8;
    int iVar9;
    uint32_t uVar10;
    DWORD DVar11;
    BOOL BVar12;
    int64_t lVar13;
    LARGE_INTEGER LVar14;
    short *psVar15;
    LARGE_INTEGER *pLVar16;
    LARGE_INTEGER LVar17;
    int iVar18;
    LARGE_INTEGER LVar19;
    uint16_t uVar20;
    LARGE_INTEGER LVar21;
    LARGE_INTEGER LVar22;
    WCHAR WVar24;
    uint8_t bVar25;
    LARGE_INTEGER local_res10;
    LARGE_INTEGER local_res20;
    char local_c5;
    LARGE_INTEGER local_c0;
    uint32_t local_b8;
    _SHFILEOPSTRUCTW local_b0;
    WCHAR *local_78;
    LARGE_INTEGER local_70;
    int64_t local_68;
    uint64_t local_60;
    LARGE_INTEGER local_58;
    int64_t local_50;
    uint64_t local_48;
    LARGE_INTEGER LVar23;

    LVar23.QuadPart = 0;
    iVar18 = 0;
    LVar22.QuadPart = 0;
    uVar20 = 4;
    bVar25 = 0;
    local_res20.QuadPart = local_res20.QuadPart & 0xffffffffffffff00;
    local_c5 = '\0';
    bVar2 = false;
    bVar5 = false;
    bVar6 = false;
    bVar4 = false;
    local_res10 = cmd;
    while (uVar10 = flags, *(uint16_t *)(uintptr_t)local_res10.QuadPart == 0x2d) {
        if (*(uint16_t *)(local_res10.QuadPart + 2) == 0x2d) {
            local_res10.QuadPart = local_res10.QuadPart + 2;
        }
        cVar7 = FUN_1400660AC("-", &local_res10.QuadPart, 1);
        if (cVar7 != '\0')
            break;
        cVar7 = FUN_1400660AC("-simpleprogress", &local_res10.QuadPart, 0xf);
        flags = uVar10;
        if (cVar7 == '\0') {
            cVar7 = FUN_1400660AC("-progress", &local_res10.QuadPart, 9);
            if (cVar7 == '\0') {
                cVar7 = FUN_1400660AC("-file", &local_res10.QuadPart, 5);
                if (cVar7 == '\0') {
                    cVar7 = FUN_1400660AC("-m", &local_res10.QuadPart, 2);
                    if (cVar7 == '\0') {
                        cVar7 = FUN_1400660AC("-forceq", &local_res10.QuadPart, 7);
                        flags = 3;
                        if ((cVar7 == '\0') &&
                            (cVar7 = FUN_1400660AC("-force", &local_res10.QuadPart, 6), flags = 1,
                             cVar7 == '\0')) {
                            cVar7 = FUN_1400660AC("-su", &local_res10.QuadPart, 3);
                            if (cVar7 == '\0') {
                                cVar7 = FUN_1400660AC("-q", &local_res10.QuadPart, 2);
                                if (cVar7 == '\0') {
                                    cVar7 = FUN_1400660AC("-delme", &local_res10.QuadPart, 6);
                                    if (cVar7 == '\0') {
                                        cVar7 = FUN_1400660AC("-rd", &local_res10.QuadPart, 3);
                                        if (cVar7 == '\0') {
                                            cVar7 =
                                                FUN_1400660AC("-delay", &local_res10.QuadPart, 6);
                                            if (cVar7 == '\0') {
                                                uVar1 =
                                                    *(uint16_t *)(uintptr_t)local_res10.QuadPart;
                                                while (
                                                    (uVar1 != 0 && (((uVar1 < 9 || (0xd < uVar1)) &&
                                                                     (uVar1 != 0x20))))) {
                                                    local_res10.QuadPart = local_res10.QuadPart + 2;
                                                    uVar1 = *(uint16_t *)(uintptr_t)
                                                                 local_res10.QuadPart;
                                                }
                                                PECMD_SkipLeadingControls(
                                                    (WCHAR **)&local_res10.QuadPart);
                                                flags = uVar10;
                                            }
                                            else {
                                                bVar4 = true;
                                                flags = uVar10;
                                            }
                                        }
                                        else {
                                            bVar25 = 2;
                                            bVar5 = true;
                                            flags = uVar10;
                                        }
                                    }
                                    else {
                                        bVar2 = true;
                                        flags = uVar10;
                                    }
                                }
                                else {
                                    local_c5 = '\x10';
                                    flags = uVar10;
                                }
                            }
                            else {
                                bVar6 = true;
                                bVar25 = bVar25 | 0x10;
                                flags = uVar10;
                            }
                        }
                    }
                    else {
                        B2F_LO8(local_res20.LowPart) = 1;
                    }
                }
                else {
                    bVar25 = 1;
                }
            }
            else {
                uVar20 = 0;
            }
        }
        else {
            uVar20 = 0x100;
        }
    }
    PECMD_SkipLeadingControls((WCHAR **)&local_res10.QuadPart);
    if (bVar2) {
        FUN_1400702B0((WCHAR **)&local_res20.QuadPart, WSTR("#23:INDATA "));
        FUN_14006375C((WCHAR **)&local_res20.QuadPart, (LPCWSTR)(uintptr_t)local_res10.QuadPart);
        lVar13 = PECMD_RunCommand((void *)g_Script, (WCHAR *)local_res20.QuadPart);
        PECMD_FreeStrBuf((WCHAR **)&local_res20.QuadPart);
        return PECMD_LI((int64_t)lVar13);
    }
    if (*(int16_t *)(uintptr_t)local_res10.QuadPart == 0) {
        return PECMD_LI((int64_t)-0x7ff8ffa9);
    }
    iVar9 = lstrlenW((LPCWSTR)(uintptr_t)local_res10.QuadPart);
    PECMD_AllocWStringBuffer((WCHAR **)&local_78, (int64_t)((iVar9 + 1) * 2 + 4));
    lVar13 = (int64_t)(iVar9 + 1) * 2;
    memcpy((uint8_t *)local_78, (uint8_t *)(uintptr_t)local_res10.QuadPart, (int)lVar13);
    local_res10.QuadPart = (int64_t)(uintptr_t)local_78;
    *(uint16_t *)(lVar13 + (int64_t)local_78) = 0;
    local_b0.hwnd = (HWND)0x0;
    memset((uint64_t *)&local_b0.wFunc, 0, 0x30);
    PECMD_SkipLeadingControls((WCHAR **)&local_res10.QuadPart);
    LVar17 = local_res10;
    local_b0.fFlags = uVar20 | 0x610;
    local_b0.wFunc = 3;
    local_b0.pFrom = (LPCWSTR)(uintptr_t)local_res10.QuadPart;
    local_b8 = 0;
    bVar2 = false;
    PECMD_AllocStrSlot(&local_70);
    local_68 = 0;
    local_60 = 0;
    (void)local_60;
    PECMD_AllocStrSlot(&local_58);
    local_50 = 0;
    local_48 = 0;
    (void)local_48;
    pLVar16 = &local_70;
    PECMD_AllocString(&local_70, 10);
    PECMD_AllocString(&local_58, 10);
    WVar24 = *(WCHAR *)(uintptr_t)LVar17.QuadPart;
    LVar21 = LVar17;
    bVar3 = false;
    if (WVar24 != L'\0') {
        LVar19.QuadPart = LVar23.QuadPart;
        uVar8 = B2F_LO8(local_res20.LowPart);
        do {
            if ((uVar8 == '\0') || (WVar24 != L';')) {
                if (WVar24 == L'>') {
                    if (*(WCHAR *)(LVar17.QuadPart + -2) == L'=') {
                        local_b0.wFunc = 2;
                    }
                    else if (*(WCHAR *)(LVar17.QuadPart + -2) == L'-') {
                        local_b0.wFunc = 1;
                    }
                    else if (*(WCHAR *)(LVar17.QuadPart + 2) == L'>') {
                        LVar17.QuadPart = LVar17.QuadPart + 2;
                        local_b0.wFunc = 4;
                    }
                    LVar14.QuadPart = LVar17.QuadPart + -4;
                    if ((uint64_t)LVar21.QuadPart <= (uint64_t)LVar14.QuadPart) {
                        do {
                            if ((((uint16_t)*(WCHAR *)(uintptr_t)LVar14.QuadPart < 9) ||
                                 (0xd < (uint16_t)*(WCHAR *)(uintptr_t)LVar14.QuadPart)) &&
                                (*(WCHAR *)(uintptr_t)LVar14.QuadPart != L' '))
                                break;
                            *(WCHAR *)(uintptr_t)LVar14.QuadPart = L'\0';
                            LVar14.QuadPart = LVar14.QuadPart + -2;
                        } while ((uint64_t)LVar21.QuadPart <= (uint64_t)LVar14.QuadPart);
                        LVar19.HighPart = 0;
                        LVar19.LowPart = local_b8;
                    }
                    *(WCHAR *)(LVar17.QuadPart + -2) = L'\0';
                    *(WCHAR *)(uintptr_t)LVar17.QuadPart = L'\0';
                    local_c0.QuadPart = LVar17.QuadPart + 2;
                    PECMD_SkipLeadingControls((WCHAR **)&local_c0.QuadPart);
                    LVar17 = local_c0;
                    local_b0.pTo = (LPCWSTR)(uintptr_t)local_c0.QuadPart;
                    if (*(WCHAR *)(uintptr_t)LVar21.QuadPart != L'\0') {
                        PECMD_AppendParamToken(&pLVar16->QuadPart,
                                               (LPCWSTR)(uintptr_t)LVar21.QuadPart, '\0');
                    }
                    bVar2 = true;
                    pLVar16 = &local_58;
                    LVar21 = LVar17;
                    uVar8 = B2F_LO8(local_res20.LowPart);
                }
            }
            else {
                *(WCHAR *)(uintptr_t)LVar17.QuadPart = L'\0';
                local_c0.QuadPart = LVar17.QuadPart + 2;
                if (*(WCHAR *)(uintptr_t)LVar21.QuadPart != L'\0') {
                    if ((bVar2) || (cVar7 = '\x01', bVar25 == 0)) {
                        cVar7 = '\0';
                    }
                    PECMD_AppendParamToken(&pLVar16->QuadPart, (LPCWSTR)(uintptr_t)LVar21.QuadPart,
                                           cVar7);
                }
                PECMD_SkipLeadingControls((WCHAR **)&local_c0.QuadPart);
                LVar17 = local_c0;
                LVar21 = local_c0;
                uVar8 = B2F_LO8(local_res20.LowPart);
                if (bVar2) {
                    LVar19.LowPart = LVar19.LowPart + 1;
                    LVar19.HighPart = 0;
                    local_b8 = LVar19.LowPart;
                }
            }
            iVar18 = (int)LVar19.QuadPart;
            LVar17.QuadPart = LVar17.QuadPart + 2;
            WVar24 = *(WCHAR *)(uintptr_t)LVar17.QuadPart;
            bVar3 = bVar2;
        } while (WVar24 != L'\0');
    }
    if ((*(WCHAR *)(uintptr_t)LVar21.QuadPart != L'\0') &&
        (PECMD_AppendParamToken(&pLVar16->QuadPart, (LPCWSTR)(uintptr_t)LVar21.QuadPart, '\0'),
         bVar3)) {
        iVar18 = iVar18 + 1;
    }
    *(WCHAR *)(local_70.QuadPart + local_68 * 2) = L'\0';
    *(short *)(local_58.QuadPart + local_50 * 2) = 0;
    local_b0.pFrom = (LPCWSTR)local_70.QuadPart;
    if (0 < iVar18) {
        local_b0.pTo = (LPCWSTR)local_58.QuadPart;
    }
    SetLastError(0);
    if ((uVar10 != 0) && (local_b0.wFunc == 3)) {
        if (((uVar10 & 2) == 0) || (iVar18 = SHFileOperationW(&local_b0),
                                    LVar22.QuadPart = LVar23.QuadPart, iVar18 != 0)) {
            LVar22.QuadPart =
                PECMD_DeleteDirectoryTree((LPCWSTR)local_70.QuadPart, (uint32_t)(uint8_t)local_c5);
        }
        goto LAB_14003c9a2;
    }
    if (bVar4) {
        if ((local_b0.pTo == (LPCWSTR)0x0) ||
            (LVar22.QuadPart = (int64_t)(uintptr_t)local_b0.pTo, *local_b0.pTo == 0)) {
            LVar22.QuadPart = LVar23.QuadPart;
        }
        iVar18 = MoveFileExW(local_b0.pFrom, (LPCWSTR)(uintptr_t)LVar22.QuadPart, 6);
    }
    else {
        if (bVar25 == 0) {
        LAB_14003c906:
            if (local_b0.wFunc != 4) {
                if (1 < iVar18) {
                    local_b0.fFlags = local_b0.fFlags | 1;
                }
                iVar18 = SHFileOperationW(&local_b0);
                LVar22.QuadPart = iVar18;
                goto LAB_14003c9a2;
            }
            FUN_1400702B0((WCHAR **)&local_c0.QuadPart, local_b0.pFrom);
            psVar15 = PECMD_LastPathSeparator((short *)(uintptr_t)local_c0.QuadPart);
            LVar22 = local_c0;
            if (psVar15 != (short *)0x0) {
                LVar22.QuadPart = (int64_t)(uintptr_t)(psVar15 + 1);
            }
            *(WCHAR *)(uintptr_t)LVar22.QuadPart = L'\0';
            psVar15 = PECMD_LastPathSeparator(local_b0.pTo);
            LVar22.QuadPart = (int64_t)(uintptr_t)local_b0.pTo;
            if (psVar15 != (short *)0x0) {
                LVar22.QuadPart = (int64_t)(uintptr_t)(psVar15 + 1);
            }
            FUN_14006375C((WCHAR **)&local_c0.QuadPart, (LPCWSTR)(uintptr_t)LVar22.QuadPart);
            BVar12 = MoveFileW(local_b0.pFrom, (LPCWSTR)(uintptr_t)local_c0.QuadPart);
            if (BVar12 != 1) {
                uVar10 = GetLastError();
                if (uVar10 == 0) {
                    uVar10 = 1;
                }
                LVar23.HighPart = 0;
                LVar23.LowPart = uVar10;
            }
            pLVar16 = &local_c0;
            LVar22 = LVar23;
        LAB_14003c8ca:
            PECMD_FreeStrBuf(&pLVar16->QuadPart);
            goto LAB_14003c9a2;
        }
        if (bVar5) {
            iVar18 = RemoveDirectoryW(local_b0.pFrom);
        }
        else if (local_b0.wFunc == 3) {
            if (bVar6) {
                local_res20.QuadPart = 0;
                lVar13 = PECMD_OpenFileHandle((HANDLE *)&local_res20.QuadPart, local_b0.pFrom,
                                              0x80000000, 7, (LPSECURITY_ATTRIBUTES)0x0, 3,
                                              0x6000080, (HANDLE)0x0);
                DVar11 = GetLastError();
                if (lVar13 == 0) {
                    if (DVar11 == 2) {
                        if (((WCHAR *)local_res20.QuadPart != (WCHAR *)0x0) &&
                            ((WCHAR *)local_res20.QuadPart != (WCHAR *)0xffffffffffffffff)) {
                            CloseHandle((HANDLE)(uintptr_t)local_res20.QuadPart);
                        }
                        LVar22.QuadPart = 2;
                        goto LAB_14003c9a2;
                    }
                }
                else {
                    if (((WCHAR *)local_res20.QuadPart != (WCHAR *)0x0) &&
                        ((WCHAR *)local_res20.QuadPart != (WCHAR *)0xffffffffffffffff)) {
                        CloseHandle((HANDLE)(uintptr_t)local_res20.QuadPart);
                    }
                    local_res20.QuadPart = 0;
                    lVar13 = PECMD_OpenFileHandle((HANDLE *)&local_res20.QuadPart, local_b0.pFrom,
                                                  0x80000000, 7, (LPSECURITY_ATTRIBUTES)0x0, 3,
                                                  0x6000080, (HANDLE)0x0);
                    DVar11 = GetLastError();
                    if ((lVar13 == 0) && (DVar11 == 2)) {
                        if (((WCHAR *)local_res20.QuadPart != (WCHAR *)0x0) &&
                            ((WCHAR *)local_res20.QuadPart != (WCHAR *)0xffffffffffffffff)) {
                            CloseHandle((HANDLE)(uintptr_t)local_res20.QuadPart);
                        }
                        goto LAB_14003c9a2;
                    }
                }
                if (((WCHAR *)local_res20.QuadPart != (WCHAR *)0x0) &&
                    ((WCHAR *)local_res20.QuadPart != (WCHAR *)0xffffffffffffffff)) {
                    CloseHandle((HANDLE)(uintptr_t)local_res20.QuadPart);
                }
            }
            iVar18 = DeleteFileW(local_b0.pFrom);
        }
        else if (local_b0.wFunc == 2) {
            if (bVar6) {
                FUN_1400702B0((WCHAR **)&local_res20.QuadPart, WSTR(" -dd -bs=8M \""));
                FUN_14006375C((WCHAR **)&local_res20.QuadPart, local_b0.pTo);
                FUN_14006375C((WCHAR **)&local_res20.QuadPart, WSTR("\",0,\""));
                FUN_14006375C((WCHAR **)&local_res20.QuadPart, local_b0.pFrom);
                FUN_14006375C((WCHAR **)&local_res20.QuadPart, WSTR("\""));
                LVar22 = FUN_1400D2E90(script, local_res20);
                pLVar16 = &local_res20;
                goto LAB_14003c8ca;
            }
            iVar18 = CopyFileW(local_b0.pFrom, local_b0.pTo, 0);
        }
        else {
            if (local_b0.wFunc != 1)
                goto LAB_14003c906;
            iVar18 = MoveFileW(local_b0.pFrom, local_b0.pTo);
        }
    }
    LVar22.QuadPart = LVar23.QuadPart;
    if (iVar18 != 1) {
        uVar10 = GetLastError();
        if (uVar10 == 0) {
            uVar10 = 1;
        }
        LVar22.HighPart = 0;
        LVar22.LowPart = uVar10;
    }
LAB_14003c9a2:
    PECMD_FreeStrBuf(&local_58.QuadPart);
    PECMD_FreeStrBuf(&local_70.QuadPart);
    PECMD_FreeStrBuf((WCHAR **)&local_78);
    return LVar22;
}

/* ========== FUN_14003CD0C @0x14003cd0c ========== */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

LPCRITICAL_SECTION FUN_14003CD0C(int64_t *script, LPCRITICAL_SECTION name)

{
    WCHAR WVar1;
    bool bVar2;
    LPCRITICAL_SECTION p_Var3;
    char cVar4;
    int iVar5;
    DWORD DVar6;
    uint64_t uVar7;
    uint64_t uVar8;
    HANDLE hThread;
    LPCWSTR pWVar9;
    LPWSTR pWVar10;
    LPCRITICAL_SECTION p_Var11;
    LPCRITICAL_SECTION lpCriticalSection;
    LPCRITICAL_SECTION p_Var12;
    uint32_t uVar13;
    UINT UVar14;
    WCHAR WVar15;
    LPCRITICAL_SECTION p_Var16;
    uint32_t uVar17;
    DWORD DVar18;
    uint8_t bVar19;
    bool bVar20;
    LPCRITICAL_SECTION local_res10;
    uint8_t local_res18;
    UINT local_res20;
    LPCRITICAL_SECTION local_b8;
    int local_b0;
    uint32_t local_ac;
    UINT local_a8[2];
    LPCRITICAL_SECTION local_a0;
    LPCRITICAL_SECTION local_98;
    LPCRITICAL_SECTION local_90;
    DWORD local_88;
    LPCRITICAL_SECTION local_80;
    LPCRITICAL_SECTION local_78;
    LPCRITICAL_SECTION local_70;
    uint8_t local_68[40];

    p_Var12 = (LPCRITICAL_SECTION)0x0;
    local_res20 = 0;
    local_78 = (LPCRITICAL_SECTION)0x0;
    local_b8 = (LPCRITICAL_SECTION)0x0;
    local_80 = (LPCRITICAL_SECTION)0x0;
    local_a0 = (LPCRITICAL_SECTION)0x1;
    local_70 = (LPCRITICAL_SECTION)0x0;
    local_res10 = name;
    PECMD_AllocStrSlot(&local_98);
    local_88 = GetCurrentProcessId();
    FUN_14007BF44(script, (WCHAR *)local_res10, &local_98, 0, 1);
    local_res10 = local_98;
    bVar2 = false;
    bVar20 = false;
    uVar17 = 0;
    local_ac = (uint32_t)B2F_HI16(local_ac) << 0x10;
    WVar15 = L'*';
    if (*(WCHAR *)&local_98->DebugInfo == L'-') {
        WVar15 = L'-';
    }
    if (WVar15 == *(WCHAR *)&local_98->DebugInfo) {
        do {
            local_res10 = (LPCRITICAL_SECTION)((int64_t)&local_98->DebugInfo + 2);
            if (local_res10 == (LPCRITICAL_SECTION)0x0)
                break;
            cVar4 = FUN_1400660AC("force", (int64_t *)&local_res10, 5);
            if (cVar4 == '\0') {
                uVar7 = FUN_14005C788("svr2", (uint16_t *)local_res10, 4);
                if ((char)uVar7 != '\0') {
                    EnterCriticalSection((LPCRITICAL_SECTION)&g_csInit);
                    FUN_14001C2CC(WSTR("SeShutdownPrivilege"), 2, 8);
                    bVar20 = (g_privFlags & 4) == 0;
                    if (bVar20) {
                        g_privFlags = g_privFlags | 4;
                        FUN_14001C2CC(WSTR("SeDebugPrivilege"), 2, 0);
                    }
                    AbortSystemShutdownW((LPWSTR)0x0);
                    if (bVar20) {
                        g_privFlags = g_privFlags & 0xfffb;
                        FUN_14001C2CC(WSTR("SeDebugPrivilege"), 4, 0);
                    }
                    LeaveCriticalSection((LPCRITICAL_SECTION)&g_csInit);
                    PECMD_FreeStrBuf((WCHAR **)&local_98);
                    return (LPCRITICAL_SECTION)0x0;
                }
                iVar5 = StrCmpNIW(WSTR("exitcode="), (LPCWSTR)local_res10, 9);
                if (iVar5 == 0) {
                    local_a8[0] = 0;
                    local_res10 = (LPCRITICAL_SECTION)((int64_t)&local_res10->OwningThread + 4);
                    WVar1 = *(WCHAR *)&local_res10->DebugInfo;
                    PECMD_ParseUIntValue((int64_t *)&local_res10, (int *)local_a8);
                    if ((uint16_t)(WVar1 + L'￐') < 10) {
                        local_res20 = local_a8[0];
                    }
                }
                else {
                    cVar4 = FUN_1400660AC("explorer", (int64_t *)&local_res10, 8);
                    if (cVar4 == '\0') {
                        cVar4 = FUN_1400660AC("gui", (int64_t *)&local_res10, 3);
                        if (cVar4 == '\0') {
                            cVar4 = FUN_1400660AC("tree", (int64_t *)&local_res10, 4);
                            if (cVar4 == '\0') {
                                cVar4 = FUN_1400660AC("1", (int64_t *)&local_res10, 1);
                                if (cVar4 == '\0') {
                                    local_res10 =
                                        (LPCRITICAL_SECTION)((int64_t)&local_res10[-1].SpinCount +
                                                             6);
                                    break;
                                }
                                local_ac = 1;
                            }
                            else {
                                uVar17 = 0xc0;
                            }
                        }
                        else {
                            bVar20 = true;
                        }
                    }
                    else {
                        local_res20 = 1;
                    }
                }
            }
            else {
                bVar2 = true;
            }
            local_98 = local_res10;
        } while (WVar15 == *(WCHAR *)&local_res10->DebugInfo);
        if (bVar20) {
            B2F_LO16(local_68) = u__26_INDATA_140121fe0[0];
            *(uint16_t *)&local_68[2] = u__26_INDATA_140121fe0[1];
            *(uint16_t *)&local_68[4] = u__26_INDATA_140121fe0[2];
            *(uint16_t *)&local_68[6] = u__26_INDATA_140121fe0[3];
            *(uint16_t *)&local_68[8] = u__26_INDATA_140121fe0[4];
            *(uint16_t *)&local_68[16] = u__26_INDATA_140121fe0[5];
            *(uint16_t *)&local_68[18] = u__26_INDATA_140121fe0[6];
            *(uint16_t *)&local_68[20] = u__26_INDATA_140121fe0[7];
            *(uint16_t *)&local_68[22] = u__26_INDATA_140121fe0[8];
            *(uint16_t *)&local_68[24] = u__26_INDATA_140121fe0[9];
            *(uint16_t *)&local_68[32] = u__26_INDATA_140121fe0[10];
            *(uint16_t *)&local_68[34] = _UNK_140121ff6;
            PECMD_RunCommand(script, (WCHAR *)local_68);
            goto LAB_14003d454;
        }
    }
    EnterCriticalSection((LPCRITICAL_SECTION)&g_csInit);
    local_b0 = 0x8000;
    bVar20 = false;
    if ((bVar2) && ((g_privFlags & 4) == 0)) {
        g_privFlags = g_privFlags | 4;
        bVar20 = true;
        FUN_14001C2CC(WSTR("SeDebugPrivilege"), 2, 0);
        lpCriticalSection = (LPCRITICAL_SECTION)&g_csInit;
    }
    else {
        LeaveCriticalSection((LPCRITICAL_SECTION)&g_csInit);
        lpCriticalSection = p_Var12;
    }
    p_Var11 = p_Var12;
    if (*(short *)&local_res10->DebugInfo == 0x2a) {
        do {
            local_res10 = (LPCRITICAL_SECTION)((int64_t)&local_res10->DebugInfo + 2);
            uVar13 = (int)(intptr_t)p_Var11 + 1;
            p_Var11 = (LPCRITICAL_SECTION)(uint64_t)uVar13;
            p_Var16 = p_Var12;
        } while (*(WCHAR *)&local_res10->DebugInfo == L'*');
        for (; iVar5 = (int)(intptr_t)p_Var16, *(WCHAR *)&local_res10->DebugInfo == L'&';
             local_res10 = (LPCRITICAL_SECTION)((int64_t)&local_res10->DebugInfo + 2)) {
            p_Var16 = (LPCRITICAL_SECTION)(uint64_t)(iVar5 + 1);
        }
        uVar8 = PECMD_EvalParenStripped((int64_t *)&local_res10, (uint64_t *)&local_b8);
        if ((0 < (int)uVar8) && (local_b8 != (LPCRITICAL_SECTION)0x0)) {
            if (uVar13 == 1) {
                if (iVar5 == 0)
                    goto LAB_14003d1cb;
                iVar5 = TerminateProcess((HANDLE)(uintptr_t)local_b8, local_res20);
                goto LAB_14003d128;
            }
            if (uVar13 != 2) {
            LAB_14003d1cb:
                local_80 = local_b8;
                bVar19 = 0;
                UVar14 = local_res20;
                if ((uint16_t)uVar17 == 0) {
                    local_res10 = (LPCRITICAL_SECTION)0x0;
                    p_Var11 = local_b8;
                    p_Var16 = p_Var12;
                    goto LAB_14003d388;
                }
                p_Var16 = (LPCRITICAL_SECTION)g_szEmpty;
                uVar17 = (uint32_t)(uint16_t)((uint16_t)uVar17 | 1);
                local_res10 = (LPCRITICAL_SECTION)g_szEmpty;
                do {
                    p_Var3 = local_80;
                    DVar18 = (DWORD)(uintptr_t)local_80;
                    DVar6 = FUN_14002D708(
                        (LPCWSTR)p_Var16, uVar17 | 0x10 | (int)(char)bVar19,
                        (int64_t *)(uint64_t)(-(uint32_t)((short)uVar17 != 0) & UVar14), 0, DVar18);
                    local_b8 = (LPCRITICAL_SECTION)(uint64_t)(uint32_t)DVar6;
                    p_Var11 = local_b8;
                    p_Var16 = local_res10;
                    if (p_Var3 != (LPCRITICAL_SECTION)0x0) {
                        PECMD_TerminateProcessById(DVar18, UVar14, &local_a0);
                        p_Var11 = local_b8;
                        p_Var16 = local_res10;
                    }
                    do {
                        if ((short)uVar17 != 0) {
                            local_a0 =
                                (LPCRITICAL_SECTION)(uint64_t)(p_Var11 == (LPCRITICAL_SECTION)0x0);
                            if ((p_Var16 != (LPCRITICAL_SECTION)0x0) &&
                                (p_Var11 != (LPCRITICAL_SECTION)0x0))
                                goto LAB_14003d3a6;
                        LAB_14003d40f:
                            if (local_78 != (LPCRITICAL_SECTION)0x0) {
                                *(WCHAR *)&local_78->DebugInfo = L'*';
                            }
                            if (bVar20) {
                                g_privFlags = g_privFlags & 0xfffb;
                                FUN_14001C2CC(WSTR("SeDebugPrivilege"), 4, 0);
                            }
                            p_Var12 = local_a0;
                            if (lpCriticalSection != (LPCRITICAL_SECTION)0x0) {
                                LeaveCriticalSection(lpCriticalSection);
                                p_Var12 = local_a0;
                            }
                            goto LAB_14003d454;
                        }
                        if ((LPCRITICAL_SECTION)(int64_t)(int)local_88 != p_Var11) {
                        LAB_14003d388:
                            if (p_Var11 != (LPCRITICAL_SECTION)0x0) {
                                PECMD_TerminateProcessById((DWORD)(uintptr_t)p_Var11, UVar14,
                                                           &local_a0);
                                p_Var11 = local_b8;
                                p_Var16 = local_res10;
                            }
                        }
                    LAB_14003d3a6:
                        if ((((short)local_ac != 0) || (p_Var16 == (LPCRITICAL_SECTION)0x0)) ||
                            (p_Var11 == (LPCRITICAL_SECTION)0x0))
                            goto LAB_14003d40f;
                        if (local_70 == p_Var11) {
                            FUN_1400195F0(script, 1, 0, (uint64_t *)0x0);
                            p_Var11 = local_b8;
                            p_Var16 = local_res10;
                        }
                        local_70 = p_Var11;
                        if ((short)uVar17 != 0) {
                            local_70 = p_Var12;
                        }
                        if (((intptr_t)p_Var11 < 1) || (local_b0 = local_b0 + -1, local_b0 < 1))
                            goto LAB_14003d40f;
                    LAB_14003d2f8:; /* empty stmt: MSVC requires a statement after a label */
                    } while (p_Var16 == (LPCRITICAL_SECTION)0x0);
                } while (true);
            }
            if (iVar5 == 0) {
                hThread = OpenThread(1, 0, (DWORD)(uintptr_t)local_b8);
                if ((hThread != (HANDLE)0x0) && (hThread != (HANDLE)0xffffffffffffffff)) {
                    iVar5 = TerminateThread(hThread, local_res20);
                    CloseHandle(hThread);
                    goto LAB_14003d128;
                }
            LAB_14003d12f:
                DVar6 = GetLastError();
                if (DVar6 == 0) {
                    DVar6 = 1;
                }
                p_Var12 = (LPCRITICAL_SECTION)(uint64_t)DVar6;
            }
            else {
                iVar5 = TerminateThread((HANDLE)(uintptr_t)local_b8, local_res20);
            LAB_14003d128:
                if (iVar5 != 1)
                    goto LAB_14003d12f;
            }
            if (bVar20) {
                g_privFlags = g_privFlags & 0xfffb;
                FUN_14001C2CC(WSTR("SeDebugPrivilege"), 4, 0);
            }
            if (lpCriticalSection != (LPCRITICAL_SECTION)0x0) {
                LeaveCriticalSection(lpCriticalSection);
            }
            goto LAB_14003d454;
        }
    }
    else {
        pWVar9 = PECMD_StripTrailingSpaces((LPCWSTR)local_res10);
        local_res10 = (LPCRITICAL_SECTION)FUN_14001BE14(pWVar9);
        if (*(WCHAR *)&local_res10->DebugInfo != L'\0') {
            pWVar10 = StrChrW((LPCWSTR)local_res10, L'\\');
            local_res18 = -(pWVar10 != (LPWSTR)0x0) & 0x20;
            p_Var11 = local_b8;
            p_Var16 = local_res10;
            UVar14 = local_res20;
            bVar19 = local_res18;
            if (local_res10 == (LPCRITICAL_SECTION)0x0)
                goto LAB_14003d2f8;
            local_90 = local_res10;
            PECMD_SkipUntilDelim(&local_90, 0x2a, 0x3f);
            p_Var11 = local_b8;
            p_Var16 = local_res10;
            UVar14 = local_res20;
            bVar19 = local_res18;
            if (((local_res10 < local_90) && (*(WCHAR *)&local_90->DebugInfo == L'*')) &&
                (*(LPCWSTR)((int64_t)&local_90->DebugInfo + 2) == L'\0')) {
                *(WCHAR *)&local_90->DebugInfo = L'\0';
                local_78 = local_90;
                uVar17 = 0x81;
                goto LAB_14003d2f8;
            }
            if (*(WCHAR *)&local_90->DebugInfo == L'\0')
                goto LAB_14003d2f8;
            uVar17 = 0x181;
            goto LAB_14003d2f8;
        }
    }
    if (bVar20) {
        g_privFlags = g_privFlags & 0xfffb;
        FUN_14001C2CC(WSTR("SeDebugPrivilege"), 4, 0);
    }
    if (lpCriticalSection != (LPCRITICAL_SECTION)0x0) {
        LeaveCriticalSection(lpCriticalSection);
    }
    p_Var12 = (LPCRITICAL_SECTION)0xffffffff80070057;
LAB_14003d454:
    PECMD_FreeStrBuf((WCHAR **)&local_98);
    return p_Var12;
}

/* ========== FUN_14003ED4C @0x14003ed4c ========== */

/* WARNING: Function: __chkstk replaced with injection: alloca_probe */
/* WARNING: Type propagation algorithm not settling */
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

int64_t FUN_14003ED4C(int64_t *script, LPCWSTR cmd)

{
    LPCWSTR pWVar1;
    WCHAR WVar2;
    LARGE_INTEGER *pLVar3;
    bool bVar4;
    bool bVar5;
    uint8_t bVar6;
    uint8_t bVar7;
    char cVar8;
    uint16_t uVar9;
    DWORD DVar10;
    int iVar11;
    int iVar12;
    uint32_t uVar13;
    DWORD DVar14;
    BOOL BVar15;
    uint64_t uVar16;
    HWND hWnd;
    LRESULT LVar17;
    HANDLE pvVar18;
    int64_t *plVar19;
    uint64_t *puVar20;
    uint64_t uVar21;
    LARGE_INTEGER LVar22;
    uint32_t uVar23;
    uint32_t *puVar24;
    uint8_t bVar25;
    int64_t *plVar26;
    uint8_t bVar27;
    uint8_t bVar28;
    int64_t lVar29;
    DWORD DVar30;
    uint8_t *pbVar31;
    WCHAR *pWVar32;
    WCHAR WVar33;
    WCHAR WVar34;
    WCHAR WVar35;
    uint8_t bVar36;
    LPCWSTR pwVar37;
    int64_t *plVar38;
    int64_t *plVar39;
    HANDLE *ppvVar40;
    uint8_t bVar41;
    int64_t *plVar42;
    double dVar43;
    LPCWSTR local_res10;
    char local_res18;
    char local_res20;
    uint32_t local_4298[2];
    uint64_t local_4290;
    HANDLE local_4288;
    int local_4280;
    int local_427c;
    LPWSTR local_4278;
    LPWSTR local_4270;
    uint32_t local_4268;
    DWORD local_4264;
    DWORD local_4260[2];
    LPCWSTR local_4258;
    LPCWSTR local_4250;
    int64_t local_4248;
    int64_t local_4240;
    int64_t local_4238;
    int64_t local_4230;
    LPCWSTR local_4228;
    int64_t local_4220;
    int local_4218;
    HANDLE local_4210;
    LPCWSTR local_4208;
    LPCWSTR local_4200;
    LARGE_INTEGER local_41f8;
    double local_41f0;
    LPCWSTR local_41e8;
    double local_41e0;
    HANDLE *local_41d8;
    uint64_t local_41d0;
    WCHAR local_41c8;
    uint16_t local_41c6;
    MSG local_4188;
    uint8_t local_4158[256];
    WCHAR local_4058[1279];
    uint16_t local_365a;

    local_res10 = cmd;
    PECMD_SkipLeadingControls(&local_res10);
    plVar26 = (int64_t *)0x0;
    iVar11 = 1;
    local_4298[0] = 0xfffffff6;
    bVar25 = 0;
    local_res18 = '\0';
    local_res20 = '\0';
    bVar4 = false;
    DVar10 = 0;
    local_4290 = 1;
    local_427c = 0;
    local_4264 = GetCurrentProcessId();
    local_4280 = 1000;
    bVar28 = (uint8_t)*local_res10;
    bVar36 = 0x2a;
    if (*local_res10 == L'-') {
        bVar36 = 0x2d;
    }
    local_4218 = 4;
    plVar42 = plVar26;
    bVar27 = bVar25;
    DVar14 = DVar10;
    bVar41 = bVar25;
    bVar6 = 0;
    bVar7 = 0;
    if (bVar36 == bVar28) {
        do {
            bVar41 = bVar7;
            bVar27 = bVar6;
            DVar14 = (DWORD)(uintptr_t)plVar42;
            pWVar1 = local_res10 + 1;
            if (((((((uint16_t)*pWVar1 < 0x41) || (*pWVar1 == L'*')) ||
                   ((0x2f < (uint16_t)*pWVar1 && ((uint16_t)*pWVar1 < 0x3a)))) ||
                  (*pWVar1 == L'(')) ||
                 (((bVar28 == 0x2d && ((uint8_t)((uint8_t)*pWVar1 | 0x20) == 99)) &&
                   ((uint8_t)((uint8_t)local_res10[2] | 0x20) == 0x6d)))) ||
                ((bVar28 == 0x2a &&
                  (((*pWVar1 == L'\0' || ((8 < (uint16_t)*pWVar1 && ((uint16_t)*pWVar1 < 0xe)))) ||
                    (*pWVar1 == L' '))))))
                break;
            local_res10 = pWVar1;
            cVar8 = FUN_1400660AC("pad", (int64_t *)&local_res10, 3);
            if (cVar8 == '\0') {
                cVar8 = FUN_1400660AC("InitSys*", (int64_t *)&local_res10, 8);
                if (cVar8 == '\0') {
                    cVar8 = FUN_1400660AC("sys", (int64_t *)&local_res10, 3);
                    if (cVar8 == '\0') {
                        cVar8 = FUN_1400660AC("sys0", (int64_t *)&local_res10, 4);
                        if (cVar8 == '\0') {
                            cVar8 = FUN_1400660AC("syscmd", (int64_t *)&local_res10, 6);
                            if (cVar8 == '\0') {
                                cVar8 = FUN_1400660AC("sys0cmd", (int64_t *)&local_res10, 7);
                                if (cVar8 == '\0') {
                                    cVar8 = FUN_1400660AC("cont", (int64_t *)&local_res10, 4);
                                    if (cVar8 == '\0') {
                                        uVar16 = FUN_14005C788("scan:", (uint16_t *)local_res10, 5);
                                        if ((char)uVar16 == '\0') {
                                            cVar8 = FUN_1400660AC("scanall",
                                                                  (int64_t *)&local_res10, 7);
                                            if (cVar8 == '\0') {
                                                cVar8 = FUN_1400660AC("ncd",
                                                                      (int64_t *)&local_res10, 3);
                                                if (cVar8 == '\0') {
                                                    cVar8 = FUN_1400660AC(
                                                        "freemem", (int64_t *)&local_res10, 7);
                                                    if (cVar8 == '\0') {
                                                        cVar8 = FUN_1400660AC(
                                                            "thread", (int64_t *)&local_res10, 6);
                                                        if (cVar8 == '\0') {
                                                            uVar16 = FUN_14005C788(
                                                                "cur:", (uint16_t *)local_res10, 4);
                                                            if ((char)uVar16 != '\0') {
                                                                local_res10 = local_res10 + 4;
                                                                puVar24 = &local_4264;
                                                                goto LAB_14003efd6;
                                                            }
                                                            WVar35 = *local_res10;
                                                            while (
                                                                ((WVar35 != L'\0' &&
                                                                  (((uint16_t)WVar35 < 9 ||
                                                                    (0xd < (uint16_t)WVar35)))) &&
                                                                 (WVar35 != L' '))) {
                                                                local_res10 = local_res10 + 1;
                                                                WVar35 = *local_res10;
                                                            }
                                                            PECMD_SkipLeadingControls(&local_res10);
                                                        }
                                                        else {
                                                            bVar4 = true;
                                                        }
                                                    }
                                                    else {
                                                        PECMD_TrimWorkingSet('\x01');
                                                        if (*local_res10 == L'\0') {
                                                            return 0;
                                                        }
                                                    }
                                                }
                                                else {
                                                    local_427c = 0x40;
                                                }
                                            }
                                            else {
                                                local_4298[0] = 0x1000;
                                            }
                                        }
                                        else {
                                            local_res10 = local_res10 + 5;
                                            puVar24 = local_4298;
                                        LAB_14003efd6:
                                            PECMD_ParseUIntValue((int64_t *)&local_res10,
                                                                 (int *)puVar24);
                                        }
                                    }
                                    else {
                                        plVar42 = (int64_t *)0x10000;
                                    }
                                }
                                else {
                                    bVar27 = 0x50;
                                }
                            }
                            else {
                                bVar27 = 0x10;
                            }
                        }
                        else {
                            bVar27 = 0x41;
                        }
                    }
                    else {
                        bVar27 = 1;
                    }
                }
                else {
                    local_res20 = '\x01';
                }
            }
            else {
                bVar41 = 1;
            }
            DVar14 = (DWORD)(uintptr_t)plVar42;
            bVar28 = (uint8_t)*local_res10;
            bVar6 = bVar27;
            bVar7 = bVar41;
        } while (bVar36 == bVar28);
        if (bVar4) {
            DVar10 = GetCurrentThreadId();
            uVar16 = PECMD_WaitCountPumpMessages(&g_taskCount, (uint32_t)(DVar10 != g_Tid));
            return (int64_t)(int)uVar16;
        }
    }
    WVar34 = L'-';
    WVar33 = L'(';
    WVar35 = WVar34;
    if ((((uint8_t)*local_res10 == 0x2d) && ((uint8_t)local_res10[1] == 0x2d)) &&
        ((WVar2 = local_res10[2],
          WVar2 == L'\0' || (((8 < (uint16_t)WVar2 && ((uint16_t)WVar2 < 0xe)) ||
                              (WVar35 = L'-', WVar2 == L' ')))))) {
        for (; (WVar35 = *local_res10,
               WVar35 != L'\0' &&
                    ((((uint16_t)WVar35 < 9 || (0xd < (uint16_t)WVar35)) && (WVar35 != L' '))));
             local_res10 = local_res10 + 1) {
        }
        WVar35 = WVar34;
        WVar33 = L'(';
        PECMD_SkipLeadingControls(&local_res10);
    }
    if (bVar27 != 0) {
        EnterCriticalSection((LPCRITICAL_SECTION)&g_csInit);
        PECMD_CreateMutexSlot(&local_41d0, "Global\\PECMD:main:lock");
        hWnd = (HWND)PECMD_RegisterCallbackWnd(bVar27 & 0x40);
        if (hWnd == (HWND)0x0) {
            hWnd = (HWND)PECMD_RunPecmdMain(script, bVar27 & 0x40);
        }
        if (hWnd != (HWND)0x0) {
            iVar11 = lstrlenW(local_res10);
            PECMD_AllocWStringBuffer((WCHAR **)&local_4270, (int64_t)iVar11 + 0x75);
            *local_4270 = L'\0';
            if ((bVar27 & 0x10) == 0) {
                wsprintfW(local_4270, WSTR("THREAD&+ WAIT -cur:%lu "), (uint64_t)local_4264);
            }
            iVar12 = lstrlenW(local_4270);
            lstrcpynW(local_4270 + iVar12, local_res10, iVar11 + 0x11);
            iVar11 = lstrlenW(local_4270);
            (*g_pSHSetValueW)(0xffffffff80000002, WSTR("SOFTWARE\\PELOGON\\RAMDATA\\HOTKEY"),
                              WSTR("HOTKEY9000000"), 1, local_4270, iVar11 * 2);
            LVar17 = SendMessageW(hWnd, 0x312, 9000000, 0);
            iVar11 = (int)LVar17;
            (*g_pSHDeleteValueW)(0xffffffff80000002, WSTR("SOFTWARE\\PELOGON\\RAMDATA\\HOTKEY"),
                                 WSTR("HOTKEY9000000"));
            PECMD_FreeStrBuf((WCHAR **)&local_4270);
        }
        PECMD_ReleaseMutex(&local_41d0);
        LeaveCriticalSection((LPCRITICAL_SECTION)&g_csInit);
        return (int64_t)iVar11;
    }
    if (-1 < (int)local_4298[0]) {
        PECMD_AllocStrSlot(&local_4228);
        pwVar37 = WSTR("PressKey");
        if (*local_res10 != L'\0') {
            PECMD_SplitTokenTrimWs((int64_t *)&local_res10, (int64_t *)&local_4228, 0x2c);
            pwVar37 = local_4228;
        }
        local_4158[0] = 0;
        memset((uint64_t *)((int64_t)local_4158 + 1), 0, 0xff);
        GetKeyboardState(local_4158);
        lVar29 = 0x100;
        local_4058[0] = L'\0';
        if (local_4298[0] < 0x100) {
            wsprintfW(local_4058, WSTR("0x%02X "), (uint64_t)local_4158[(int)local_4298[0]]);
        }
        else {
            pbVar31 = local_4158;
            pWVar32 = local_4058;
            do {
                wsprintfW(pWVar32, WSTR("0x%02X "), (uint64_t)*pbVar31);
                pWVar32 = pWVar32 + 5;
                pbVar31 = pbVar31 + 1;
                lVar29 = lVar29 + -1;
            } while (lVar29 != 0);
        }
        local_365a = 0;
        (void)local_365a;
        FUN_1400629B8(script, pwVar37, local_4058);
        PECMD_FreeStrBuf((WCHAR **)&local_4228);
        goto LAB_140040046;
    }
    uVar13 = 0x80000000;
    DVar30 = 0xfffffc18;
    if (WVar35 == *local_res10) {
        uVar9 = local_res10[1] | 0x20;
        if ((uVar9 != 99) && (uVar9 != 100)) {
            local_res18 = '\x01';
            local_res10 = local_res10 + 1;
            PECMD_SkipLeadingControls(&local_res10);
        }
    }
    bVar28 = bVar25;
    if (*local_res10 == L'=') {
        WVar35 = *local_res10;
        local_res10 = local_res10 + 1;
        bVar28 = (uint8_t)WVar35;
    }
    if ((uint16_t)(*local_res10 + L'￐') < 10) {
        local_4268 = 0x80000000;
        local_41e0 = g_dbl2598;
        local_4200 = local_res10;
        PECMD_ParseUIntValue((int64_t *)&local_4200, (int *)&local_4268);
        uVar23 = local_4268;
        for (; (local_res10 < local_4200 &&
                ((((uint16_t)*local_res10 < 9 || (0xd < (uint16_t)*local_res10)) &&
                  (*local_res10 != L' '))));
             local_res10 = local_res10 + 1) {
        }
        if (((int)local_4268 < 0) || (uVar13 = local_4268, *local_res10 != L'.')) {
            WVar35 = *local_res10;
            while (((WVar35 != L'\0' && (((uint16_t)WVar35 < 9 || (0xd < (uint16_t)WVar35)))) &&
                    (WVar35 != L' '))) {
                local_res10 = local_res10 + 1;
                WVar35 = *local_res10;
            }
            PECMD_SkipLeadingControls(&local_res10);
        }
        else {
            PECMD_ParseAndSkipSpace_7b54((int64_t *)&local_res10, &local_41e0);
            uVar13 = uVar23;
            if (g_dbl20b28 <= local_41e0) {
                DVar30 = (DWORD)((local_41e0 + g_dbl22590) * g_dbl22588);
            }
        }
    }
    else if (WVar33 == *local_res10) {
        local_41f0 = g_dbl2598;
        uVar21 = PECMD_EvalExpressionTree((int64_t *)&local_res10, &local_41f0);
        if ((0 < (int)uVar21) && (0.0 <= local_41f0)) {
            dVar43 = local_41f0 + g_dbl22590;
            uVar13 = (uint32_t)dVar43;
            dVar43 = dVar43 + (0.0 - (double)(int64_t)dVar43);
            if ((g_dbl22580 < dVar43) && (dVar43 < g_dbl2578)) {
                DVar30 = (DWORD)(dVar43 * g_dbl22588);
            }
        }
    }
    if ((*local_res10 != L'*') && ((uVar13 != 0x80000000 || (-1 < (int)DVar30)))) {
        local_41c6 = 0;
        (void)local_41c6;
        local_41c8 = L'\0';
        PECMD_AllocStrSlot(&local_4258);
        pwVar37 = WSTR("PressKey");
        if (*local_res10 == L',') {
            local_res10 = local_res10 + 1;
            PECMD_SplitTokenTrimWs((int64_t *)&local_res10, (int64_t *)&local_4258, 0x2c);
            if (*local_4258 != L'\0') {
                pwVar37 = local_4258;
            }
        }
        if (bVar28 == 0) {
            if (local_res18 == '\0') {
                if (uVar13 != 0)
                    goto LAB_14003f819;
                if ((int)DVar30 < 1)
                    goto LAB_14003f729;
            LAB_14003f84e:
                DVar14 = DVar10;
                if (0 < (int)DVar30) {
                    DVar14 = DVar30;
                }
                if (0 < (int)uVar13) {
                    DVar10 = uVar13;
                }
                DVar10 = FUN_1400195F0(script, (int64_t)(int)DVar10, DVar14, (uint64_t *)0x0);
                local_4290 = (uint64_t)(uint32_t)DVar10;
            }
            else {
            LAB_14003f729:
                if ((uVar13 != 0) || (uVar23 = 2, local_res18 != '\0')) {
                    uVar23 = 1;
                }
                uVar13 = PECMD_WaitKeyPressHooked(uVar13, uVar23 | DVar14);
                local_4290 = (uint64_t)uVar13;
                if ((bVar41 == 0) && (uVar13 - 0x60 < 10)) {
                    local_4290 = (uint64_t)(uVar13 - 0x30);
                }
                uVar21 = local_4290;
                iVar11 = (int)local_4290;
                pWVar32 = &local_41c8;
                if (iVar11 < 1) {
                    pWVar32 = (WCHAR *)WSTR("-1");
                }
                else {
                    wsprintfW(&local_41c8, WSTR("0x%X"), local_4290);
                }
                FUN_1400629B8(script, WSTR("&&PressKey.Hex"), pWVar32);
                if (0 < iVar11) {
                    if (((iVar11 < 0x41) || (0x5a < iVar11)) &&
                        (9 < (uint16_t)((short)uVar21 - 0x30U))) {
                        wsprintfW(&local_41c8, WSTR("0x%X"), uVar21);
                    }
                    else {
                        local_41c6 = 0;
                        (void)local_41c6;
                        local_41c8 = (WCHAR)(char)uVar21;
                    }
                }
                FUN_1400629B8(script, pwVar37, &local_41c8);
            }
        }
        else {
        LAB_14003f819:
            if (-1 < (int)uVar13) {
                if (bVar28 == 0)
                    goto LAB_14003f84e;
                local_4290 = 0;
                Sleep(uVar13);
                if (*local_res10 == L'\0') {
                    PECMD_FreeStrBuf((WCHAR **)&local_4258);
                    return 0;
                }
            }
        }
        PECMD_SkipLeadingControls(&local_res10);
        uVar13 = 0x80000000;
        local_res18 = '\0';
        PECMD_FreeStrBuf((WCHAR **)&local_4258);
    }
    if (*local_res10 == L'\0') {
        DVar10 = (DWORD)local_4290;
        goto LAB_140040046;
    }
    local_4288 = (HANDLE)0x0;
    iVar11 = -1;
    bVar28 = 0;
    bVar4 = false;
    bVar5 = false;
    if (*local_res10 == L'*') {
        local_4288 = (HANDLE)(int64_t)(int)local_4264;
        iVar11 = 1;
        while (local_res10 = local_res10 + 1, *local_res10 == L'*') {
            iVar11 = iVar11 + 1;
        }
    }
    bVar36 = bVar25;
    if (0 < iVar11) {
        if (*local_res10 == L'-') {
            local_res10 = local_res10 + 1;
            local_4288 = (HANDLE)0x0;
        }
        else {
            if (*local_res10 == L'=') {
                bVar28 = 1;
                bVar4 = false;
            LAB_14003f965:
                local_res10 = local_res10 + 1;
                local_4288 = (HANDLE)0x0;
            }
            else if (*local_res10 == L'$') {
                bVar4 = true;
                bVar28 = 0;
                goto LAB_14003f965;
            }
            if ((uint16_t)(*local_res10 + L'￐') < 10) {
                PECMD_EvalParenStripped((int64_t *)&local_res10, (uint64_t *)&local_4288);
                iVar11 = iVar11 + -1;
            }
            bVar36 = bVar28;
            bVar5 = bVar4;
            if ((0 < iVar11) && (!bVar4)) {
                DVar10 = PECMD_GetParentProcessIdLevel((DWORD)local_4288, iVar11);
                local_4288 = (HANDLE)(uint64_t)DVar10;
            }
        }
    }
    local_4260[0] = 0xffffffff;
    pvVar18 = local_4288;
    PECMD_SkipLeadingControls(&local_res10);
    if (*local_res10 == L'=') {
        bVar25 = (uint8_t)*local_res10;
        local_res10 = local_res10 + 1;
    }
    if ((uint16_t)(*local_res10 + L'￐') < 10) {
        PECMD_ParseUIntValue((int64_t *)&local_res10, (int *)local_4260);
        pvVar18 = local_4288;
    }
    ppvVar40 = &local_4210;
    if (bVar5) {
        ppvVar40 = &local_4288;
    }
    local_4210 = (HANDLE)0x0;
    local_4290 = 0;
    plVar42 = plVar26;
    local_41d8 = ppvVar40;
    if (bVar5) {
    LAB_14003fa3d:
        DVar10 = (DWORD)(uintptr_t)plVar42;
        if ((pvVar18 != (HANDLE)0x0) && (!bVar5))
            goto LAB_14003fa47;
    }
    else {
        DVar10 = 0;
        if (pvVar18 == (HANDLE)0x0) {
            plVar42 = (int64_t *)0x80070057;
            local_4290 = 0x80070057;
            goto LAB_14003fa3d;
        }
    LAB_14003fa47:
        if (bVar36 == 0) {
            pvVar18 = OpenProcess(0x100000, 0, (DWORD)pvVar18);
        }
        else {
            pvVar18 = OpenThread(0x100000, 0, (DWORD)pvVar18);
        }
        *ppvVar40 = pvVar18;
        if (pvVar18 == (HANDLE)0x0) {
            DVar10 = GetLastError();
            local_4290 = (uint64_t)DVar10;
        }
    }
    PECMD_AllocWStringBuffer((WCHAR **)&local_4278, 0x209);
    *local_4278 = L'\0';
    GetCurrentDirectoryW(0x208, local_4278);
    PECMD_CopyStrToSlot(&local_4250, &local_4278);
    StrChrW(local_4250, L'\\');
    *local_4278 = L'\0';
    GetEnvironmentVariableW(WSTR("SystemRoot"), local_4278, 0x208);
    lstrlenW(local_4250);
    PECMD_SkipLeadingControls(&local_res10);
    PECMD_InitPtrTable(&local_4240);
    local_41f8.QuadPart = 0;
    WVar35 = *local_res10;
joined_r0x00014003fb14:
    if ((WVar35 == L'\0') || (g_flagA24F < '\x01'))
        goto LAB_14003fd2f;
    cVar8 = FUN_1400660AC("-delms:", (int64_t *)&local_res10, 7);
    iVar11 = local_4218;
    if (cVar8 == '\0') {
        cVar8 = FUN_1400660AC("-del", (int64_t *)&local_res10, local_4218);
        if (cVar8 == '\0') {
            cVar8 = FUN_1400660AC("-cmd", (int64_t *)&local_res10, iVar11);
            if (cVar8 != '\0') {
                local_41f8.QuadPart = (int64_t)local_res10;
            LAB_14003fd2f:
                iVar11 = local_427c;
                if (local_res18 != '\0') {
                    uVar13 = -uVar13;
                }
                if ((local_427c == 0) && (*ppvVar40 != (HANDLE)0x0)) {
                    PECMD_SetCurrentDirIfChanged(local_4278);
                }
                DVar14 = GetTickCount();
                iVar12 = uVar13 + DVar14;
                if ((*ppvVar40 != (HANDLE)0x0) && ('\0' < g_flagA24F)) {
                    do {
                        if (uVar13 == 0)
                            break;
                        DVar10 = 1000;
                        if (0 < (int)uVar13) {
                            DVar10 = GetTickCount();
                            uVar13 = iVar12 - DVar10;
                            if ((int)uVar13 < 0)
                                break;
                            DVar10 = 1000;
                            if ((int)uVar13 < 1000) {
                                DVar10 = uVar13;
                            }
                        }
                        iVar11 = PECMD_MsgWaitForObjects(script, 1, ppvVar40, 0, DVar10, 0x4ff);
                        if (iVar11 == 0)
                            break;
                        iVar11 = 100;
                        while (((('\0' < g_flagA24F &&
                                  (BVar15 = PeekMessageW(&local_4188, (HWND)0x0, 0, 0, 1),
                                   BVar15 != 0)) &&
                                 (iVar11 = iVar11 + -1, iVar11 != 0)) &&
                                (local_4188.message != 0x12))) {
                            TranslateMessage(&local_4188);
                            DispatchMessageW(&local_4188);
                        }
                    } while ('\0' < g_flagA24F);
                    DVar10 = (DWORD)local_4290;
                    iVar11 = local_427c;
                }
                if (-1 < (int)local_4260[0]) {
                    if (bVar25 == 0) {
                        FUN_1400195F0(script, (int64_t)(int)local_4260[0], 0, (uint64_t *)0x0);
                    }
                    else {
                        Sleep(local_4260[0]);
                    }
                }
                if ((0 < local_4230) && ('\0' < g_flagA24F)) {
                    if (iVar11 == 0) {
                        PECMD_SetCurrentDirIfChanged(local_4278);
                    }
                    if (0 < local_4230) {
                        do {
                            if ((g_flagA24F < '\x01') || ((int)local_4230 < 1))
                                break;
                            lVar29 = (int64_t)(int)local_4230;
                            plVar38 = (int64_t *)0x1;
                            plVar42 = plVar26;
                            plVar19 = plVar26;
                            do {
                                iVar11 = (int)(intptr_t)plVar19;
                                if (g_flagA24F < '\x01')
                                    break;
                                pLVar3 = *(LARGE_INTEGER **)(local_4240 + (int64_t)plVar42 * 8);
                                plVar39 = plVar19;
                                if (pLVar3 != (LARGE_INTEGER *)0x0) {
                                    FUN_14003C06C(script, *pLVar3, 0);
                                    uVar21 = PECMD_CheckFileAttr(
                                        (LPCWSTR) *
                                        *(uint64_t **)(local_4240 + (int64_t)plVar42 * 8));
                                    plVar39 = plVar38;
                                    if ((int64_t)uVar21 < 1) {
                                        plVar39 = *(int64_t **)(local_4240 + (int64_t)plVar42 * 8);
                                        if (plVar39 != (int64_t *)0x0) {
                                            PECMD_FreeStrBuf(plVar39);
                                            free(plVar39);
                                        }
                                        *(uint64_t *)(local_4240 + (int64_t)plVar42 * 8) = 0;
                                        plVar39 = plVar19;
                                    }
                                }
                                iVar11 = (int)(intptr_t)plVar39;
                                plVar42 = (int64_t *)((int64_t)plVar42 + 1);
                                plVar38 = (int64_t *)(uint64_t)((int)(intptr_t)plVar38 + 1);
                                plVar19 = plVar39;
                            } while ((int64_t)plVar42 < lVar29);
                            if (iVar11 < 1)
                                break;
                            FUN_1400195F0(script, (int64_t)local_4280, 0, (uint64_t *)0x0);
                            local_4230 = (int64_t)iVar11;
                        } while (0 < (int64_t)iVar11);
                        DVar10 = (DWORD)local_4290;
                        ppvVar40 = local_41d8;
                        iVar11 = local_427c;
                    }
                }
                if ((iVar11 == 0) && (*ppvVar40 != (HANDLE)0x0)) {
                    PECMD_SetCurrentDirIfChanged(local_4250);
                }
                if ((local_41f8.QuadPart != 0) &&
                    (*(int16_t *)(uintptr_t)local_41f8.QuadPart != 0)) {
                    LVar22 =
                        PECMD_ProcessScriptBlock((uint64_t)(uintptr_t)script, local_41f8.QuadPart,
                                                 (int64_t *)0x0, (int64_t *)0x0, (WCHAR *)0x0);
                    DVar10 = LVar22.LowPart;
                }
                PECMD_FreeArray_ddf8(&local_4240);
                PECMD_FreeStrBuf(&local_4240);
                PECMD_FreeStrBuf((WCHAR **)&local_4250);
                PECMD_FreeStrBuf((WCHAR **)&local_4278);
                if ((local_4210 != (HANDLE)0x0) && (local_4210 != (HANDLE)0xffffffffffffffff)) {
                    CloseHandle(local_4210);
                }
                local_4210 = (HANDLE)0x0;
            LAB_140040046:
                if (local_res20 != '\0') {
                    iVar11 = PECMD_IsSysStartuped();
                    if (iVar11 != 0) {
                        PECMD_RunSysInit(script, WSTR("exec"));
                    }
                    PECMD_StartOnlyApp((LPCWSTR)0x0);
                }
                FUN_14009BB28(script, 0);
                return (int64_t)(int)DVar10;
            }
            WVar35 = *local_res10;
            while ((WVar35 != L'\0' &&
                    ((((uint16_t)WVar35 < 9 || (0xd < (uint16_t)WVar35)) && (WVar35 != L' '))))) {
                local_res10 = local_res10 + 1;
                WVar35 = *local_res10;
            }
            PECMD_SkipLeadingControls(&local_res10);
        }
        else {
            local_4220 = 0;
            local_41e8 = local_res10;
            local_res10 = (LPCWSTR)FUN_140024C48((int64_t *)&local_41e8, &local_4220, 1);
            PECMD_AllocStrSlot(&local_4248);
            PECMD_AllocStrSlot(&local_4208);
            PECMD_StrCopyW((int64_t *)&local_4208, local_41e8, local_4220);
            FUN_14001C270(local_4208, &local_4248);
            PECMD_FreeStrBuf((WCHAR **)&local_4208);
            if (0 < local_4220) {
                plVar19 = malloc(8);
                plVar42 = plVar26;
                if (plVar19 != (int64_t *)0x0) {
                    *plVar19 = local_4248;
                    local_4248 = 0;
                    plVar42 = plVar19;
                }
                puVar20 = (uint64_t *)FUN_140063B00(local_4230, &local_4240, &local_4238, 8);
                *puVar20 = (uint64_t)(uintptr_t)plVar42;
                local_4230 = local_4230 + 1;
            }
            PECMD_SkipLeadingControls(&local_res10);
            PECMD_FreeStrBuf(&local_4248);
        }
    }
    else {
        PECMD_ParseUIntValue((int64_t *)&local_res10, &local_4280);
        if (local_4280 < 0) {
            local_4280 = 0;
        }
    }
    WVar35 = *local_res10;
    goto joined_r0x00014003fb14;
}
