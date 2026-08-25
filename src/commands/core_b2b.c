/* ====================================================================
 * core_b2b.c — B2 批次小函数 (140020000-14003ffff) 第二批
 *
 * 来源: PECMD原始.EXE (x64, ImageBase=0x140000000)
 *   大小解析         PECMD_ParseSize @0x140020b6c
 *   SystemStartOptions PECMD_CheckSystemStartOptions @0x140020cf8
 *   参数记录写入     PECMD_WriteParamRecord @0x140020fd4
 *   参数表追加       PECMD_AppendParamToken @0x1400216c4
 *   目录树删除       PECMD_DeleteDirectoryTree @0x1400217bc
 *   VK/键名解析      PECMD_ParseVkKeyName @0x1400226e4
 *   钩子处理         PECMD_KeyboardHookProc @0x1400259a4
 *   一次性初始化体   PECMD_OneTimeInitBody @0x140027dbc
 *   带引号查找       PECMD_FindQuotedToken @0x1400280a8
 *   去重/尾查        PECMD_RemoveDuplicateChar @0x140025274
 *   尾部截断         PECMD_TrimTrailingSeparator @0x140025334
 *   菜单递归         PECMD_ScanMenuRecursive @0x140023080
 *   排序             PECMD_BubbleSort @0x140023268
 *   OnlyApp 启动     PECMD_StartOnlyApp @0x14002ae88
 *   进程执行         PECMD_RunProcessCommand @0x14002afa4
 *   devcon 更新      PECMD_DevconUpdate @0x14002c4bc
 *   驱动安装提示     PECMD_ShowDriverInstallMsg @0x14002c7dc
 *   参数跳过         PECMD_SkipSwitches @0x14002f1ec
 *   字体安装         PECMD_InstallFonts @0x14002f2dc
 *   Ramdriv 执行     PECMD_RunRamdriv @0x14002fc48
 *   关机脚本         PECMD_RunShutdownScript @0x14003d478
 *   系统应答         PECMD_WriteSysAck @0x14003e694
 *
 * 约定:
 *   - 未实现内部函数 extern 挂起 + TODO(verify), 不臆造语义
 *   - 全局数据符号先用 extern 声明; 完整链接时统一在 core_globals.c 定义
 *   - 反编译中的 Ghidra 扩展宏 (SUB168/SEXT816/CONCATxx) 用等价整数运算替代
 * ==================================================================== */
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>

#include "pecmd_defs.h"

/* ---- 未实现依赖 (extern + TODO(verify)) ---- */
extern int64_t PECMD_GetAvailPhysMemoryMB(void);
extern int64_t PECMD_GetPhysicalMemoryMb(void);
extern int FUN_1400690C0(HKEY root, LPCWSTR sub, LPCWSTR name, int64_t *out, DWORD *type,
                         LONG *status);
extern LPCWSTR PECMD_StripTrailingSpaces(LPCWSTR s);
/* S11: 本地声明与定义冲突已删除, 统一采用 xproto.h 原型 (原: /* S11: 本地声明与定义冲突, 已删除, 统一采用 xproto.h 原型 (原: extern BOOL FUN_140101E70(LPCWSTR s); * / extern uint64_t PECMD_Is) */

extern void PECMD_FindFirstFileW(HANDLE *ph, LPCWSTR path, WIN32_FIND_DATAW *fd);
extern LARGE_INTEGER PECMD_ExecDashCommand(LPCWSTR path);
extern uint32_t PECMD_ParseVirtualKeyCode(LPCWSTR s, int len);
extern void PECMD_ParseShortStore(WCHAR **pp, int *out, WCHAR sep);
extern LPCWSTR FUN_1400169BC(int id, void **pp);
extern int64_t FUN_140063B00(int64_t idx, int64_t *arr, int64_t *end, uint32_t esize);
extern void FUN_1400633A8(WCHAR **pp, int64_t len);
extern uint64_t FUN_14001D628(void);
extern uint64_t PECMD_MapPhysicalMemoryNT5(void);
extern uint32_t PECMD_ReadPhysMemSlot(uint64_t a);
extern BOOL PECMD_WritePhysicalMem(uint64_t a, uint32_t b);
extern void PECMD_FreeNtdll(void);
extern LPCWSTR FUN_140062EC8(LPCWSTR s, LPCWSTR sep, int mode, uint32_t flags);
extern void PECMD_FixKnownDlls32(void);
extern void PECMD_MounMountWimImage(void *script, WCHAR *cmd, uintptr_t flag);
extern void FUN_140025f10(int64_t ctx, LPCWSTR msg, uint32_t code, void *p4, void *p5, int64_t *p6);
extern void PECMD_AppendKeyIfMissing(int64_t ctx, LPCWSTR path, int mode);
extern uint64_t PECMD_StartWorkerThread(void *script, void **pref, uint32_t a3, uint64_t a4,
                                        uint64_t a5, uint32_t a6, uint64_t a7, int64_t a8, int a9);
extern void PECMD_CrtShim(WCHAR *out, size_t size, void *ctx, void *name);
extern uint64_t PECMD_RamdMountImDisk(LPCWSTR cmd, int64_t *ctx);
extern void FUN_14007BF44(int64_t *ctx, WCHAR *name, void *out, int mode, uint8_t flag);
extern uint64_t PECMD_SetRamdrivDiskSize(int size, LPCWSTR name);
extern void PECMD_QueryDiskSpace(LPCWSTR name);
/* S11: 本地声明与定义冲突已删除, 统一采用 xproto.h 原型 (原: /* S11: 本地声明与定义冲突, 已删除, 统一采用 xproto.h 原型 (原: extern void PECMD_AllocStrSlot(void *ps); * / /* S11: 本地声明与定义冲突, 已) */
/* S11: 本地声明与定义冲突已删除, 统一采用 xproto.h 原型 (原: extern void PECMD_ExecIndataCommand(LPCWSTR name, LPCWSTR value);) */


extern uint16_t *FUN_140024C48(int64_t *pp, int64_t *len, uint32_t flags);
extern WCHAR *FUN_14001C270(LPCWSTR src, WCHAR **out);
extern int64_t PECMD_IsVkPrefix(WCHAR *s);
extern void FUN_1400F429C(WCHAR **pp, WCHAR ch);
extern void PECMD_InitRamdataRegistry(uint32_t mode);
extern int64_t PECMD_ExecCmdDispatch(void *script, void *cmd, void *s3, void *s4, uint32_t flag,
                                     void *p6, void *s7, void *p8);

/* ---- 本批引用的全局数据 ---- */
extern uint8_t g_sysStartFlag;       /* SystemStartOptions 缓存 */
extern uint8_t g_answerFlag;         /* 应答标志 */
extern uint32_t g_sysVerClass;       /* 系统版本类别 */
extern HANDLE g_u64cd08;             /* 句柄 */
extern WCHAR g_szEmpty[];            /* g_szEmpty 空串 */
extern WCHAR g_wsz20c64[];           /* .rdata 字符串 */
extern WCHAR g_wsz21014[];           /* .rdata 字符串 */
extern WCHAR DAT_1401214d8[];        /* .rdata 字体路径模板 */
extern void *g_pInstallHinfSectionW; /* MyInstallHinfSection 函数指针 */

/* ========== PECMD_ParseSize @0x140020b6c ==========
 * 解析 FBWF/内存大小串: Fxxx/Pxxx/Lxxx/Hxxx 混合, 返回最终 MB 值。
 * TODO(verify): P 分支的 Ghidra 魔数除法按 /100 近似。
 */
uint32_t PECMD_ParseSize(LPWSTR s, uint32_t minMB, uint64_t unk, uint32_t defaultMB)
{
    int64_t lVar1;
    LPWSTR pWVar2;
    int64_t lVar3;
    uint32_t uVar4;
    uint32_t uVar5;
    uint32_t uVar6;
    uint32_t local_28[4];

    (void)unk;
    uVar4 = 0;
    CharUpperW(s);
    lVar1 = PECMD_GetAvailPhysMemoryMB();
    pWVar2 = StrChrW(s, L'F');
    if (pWVar2 != NULL) {
        local_28[0] = 0;
        StrToIntExW(pWVar2 + 1, 0, (int *)local_28);
        lVar3 = PECMD_GetPhysicalMemoryMb();
        if (((int64_t)(uint64_t)local_28[0] < lVar3) &&
            ((lVar3 - (int64_t)(uint64_t)local_28[0]) < 0)) {
            uVar4 = (uint32_t)((int64_t)lVar3 - (int64_t)local_28[0]);
        }
    }
    pWVar2 = StrChrW(s, L'P');
    if (pWVar2 != NULL) {
        local_28[0] = 0;
        StrToIntExW(pWVar2 + 1, 0, (int *)local_28);
        uVar4 = local_28[0];
        if (local_28[0] == 0) {
            uVar4 = 1;
        }
        lVar1 = (uint64_t)uVar4 * (uint64_t)lVar1;
        /* Ghidra: 乘 0x-5c28f5c28f5c28f5 后 >>6 是 /100 的优化形式 */
        uVar4 = (uint32_t)((uint64_t)uVar4 * (uint64_t)lVar1 / 100);
    }
    pWVar2 = StrChrW(s, L'L');
    uVar5 = 0;
    if (pWVar2 != NULL) {
        local_28[0] = 0;
        StrToIntExW(pWVar2 + 1, 0, (int *)local_28);
        uVar5 = local_28[0];
        if ((local_28[0] != 0) && (local_28[0] < minMB)) {
            uVar5 = minMB;
        }
    }
    pWVar2 = StrChrW(s, L'H');
    local_28[0] = 0;
    if (pWVar2 != NULL) {
        local_28[0] = 0;
        StrToIntExW(pWVar2 + 1, 0, (int *)local_28);
        if ((local_28[0] != 0) && (local_28[0] < minMB)) {
            local_28[0] = minMB;
        }
    }
    uVar6 = uVar5;
    if (local_28[0] < uVar5) {
        uVar6 = local_28[0];
        local_28[0] = uVar5;
    }
    if ((local_28[0] != 0) && ((local_28[0] < uVar4 || (uVar4 == 0)))) {
        uVar4 = local_28[0];
    }
    if ((uVar6 != 0) && (uVar4 < uVar6)) {
        uVar4 = uVar6;
    }
    if (uVar4 == 0) {
        uVar4 = defaultMB;
    }
    return uVar4;
}

/* ========== PECMD_CheckSystemStartOptions @0x140020cf8 ==========
 * 检查 SystemStartOptions 是否含 PECMDHB=/WIMHB=, 结果缓存到 g_sysStartFlag。
 */
uint32_t PECMD_CheckSystemStartOptions(void)
{
    WCHAR WVar1;
    LPCWSTR lpStr1;
    uint32_t uVar2;
    int iVar3;
    DWORD local_res8[2];
    LPCWSTR local_res10 = NULL;
    LPCWSTR local_res18;

    if (g_sysStartFlag == 0) {
        PECMD_AllocStrSlot(&local_res10);
        local_res8[0] = 0xffffffff;
        *((WCHAR *)local_res10) = L'\0';
        iVar3 =
            FUN_1400690C0((HKEY)0xffffffff80000002, WSTR("SYSTEM\\CurrentControlSet\\Control"),
                          WSTR("SystemStartOptions"), (int64_t *)&local_res10, local_res8, NULL);
        if ((iVar3 < 0) || ((int)local_res8[0] < 0)) {
            PECMD_FreeStrBuf((WCHAR **)&local_res10);
            uVar2 = 0xffffffff;
        }
        else {
            local_res18 = local_res10;
            while (lpStr1 = local_res18, *local_res18 != L'\0') {
                iVar3 = StrCmpNW(local_res18, WSTR("PECMDHB="), 8);
                if ((iVar3 == 0) || (iVar3 = StrCmpNW(lpStr1, WSTR("WIMHB="), 6), iVar3 == 0)) {
                    g_sysStartFlag = 1;
                    PECMD_FreeStrBuf((WCHAR **)&local_res10);
                    return 1;
                }
                WVar1 = *lpStr1;
                while ((WVar1 != L'\0' &&
                        (((local_res18 = lpStr1, (uint16_t)WVar1 < 9 || (0xd < (uint16_t)WVar1)) &&
                          (WVar1 != L' '))))) {
                    local_res18 = lpStr1 + 1;
                    lpStr1 = local_res18;
                    WVar1 = *local_res18;
                }
                PECMD_SkipLeadingControls((WCHAR **)&local_res18);
            }
            g_sysStartFlag = 0x10;
            PECMD_FreeStrBuf((WCHAR **)&local_res10);
            uVar2 = 0;
        }
    }
    else {
        uVar2 = (uint32_t)g_sysStartFlag & 1;
    }
    return uVar2;
}

/* ========== PECMD_WriteParamRecord @0x140020fd4 ==========
 * 向 s+0x160 的串缓冲写入一条结构化参数记录。
 * TODO(verify): wsprintfW(L"%d") 原反编译缺少实参, 这里按写入空串处理。
 */
void PECMD_WriteParamRecord(int64_t ctx, char type, LPCWSTR a, LPCWSTR b)
{
    int iVar1;
    int iVar2;
    int iVar3;
    int iVar4;
    uint16_t *psVar5;
    uint16_t sVar7;
    WCHAR local_78[40];

    if (*(int64_t *)(ctx + 0x160) != 0) {
        EnterCriticalSection(&g_csInit);
        wsprintfW(local_78, WSTR("%d"), 0);
        iVar1 = lstrlenW(*(LPCWSTR *)(*(int64_t *)(ctx + 0x160)));
        iVar2 = lstrlenW(a);
        iVar3 = lstrlenW(b);
        iVar4 = lstrlenW(local_78);
        PECMD_AllocString((void *)*(int64_t *)(ctx + 0x160),
                          (int64_t)iVar1 + iVar2 + iVar3 + iVar4 + 8);
        sVar7 = 9;
        psVar5 = (uint16_t *)(*(int64_t *)(*(int64_t *)(ctx + 0x160)) + (int64_t)iVar1 * 2);
        *psVar5 = (uint16_t)type;
        psVar5[1] = 9;
        psVar5 += 2;
        memcpy(psVar5, a, iVar2 * 2);
        psVar5 += iVar2;
        *psVar5 = sVar7;
        psVar5[1] = sVar7 + 0x19;
        psVar5 += 2;
        memcpy(psVar5, b, iVar3 * 2);
        psVar5 += iVar3;
        *psVar5 = sVar7 + 0x19;
        psVar5[1] = sVar7;
        psVar5 += 2;
        memcpy(psVar5, local_78, iVar4 * 2);
        psVar5 += iVar4;
        *psVar5 = sVar7 + 4;
        psVar5[1] = sVar7 + 1;
        psVar5[2] = 0;
        LeaveCriticalSection(&g_csInit);
    }
}

/* ========== PECMD_AppendParamToken @0x1400216c4 ==========
 * 向 ctx 描述的串表追加一个 token (去引号, 可选过滤)。
 */
void PECMD_AppendParamToken(int64_t *list, LPCWSTR token, char filter)
{
    int64_t lVar1;
    BOOL bVar2;
    int iVar3;
    LPCWSTR lpString;
    LPCWSTR local_res10;
    LPCWSTR local_res20;

    local_res10 = token;
    PECMD_SkipLeadingControls((WCHAR **)&local_res10);
    lpString = local_res10;
    PECMD_StripTrailingSpaces(local_res10);
    if (*lpString == L'\"') {
        lpString++;
    }
    if (*lpString == L'\0') {
        return;
    }
    iVar3 = lstrlenW(lpString);
    if (lpString[iVar3 - 1] == L'\"') {
        ((WCHAR *)lpString)[iVar3 - 1] = L'\0';
    }
    if (*lpString != L'\0') {
        PECMD_AllocStrSlot(&local_res20);
        FUN_14001C270(lpString, (WCHAR **)&local_res20);
        iVar3 = lstrlenW(local_res20);
        iVar3++;
        if ((filter == '\0') || (bVar2 = FUN_140101E70(local_res20), bVar2 != 0)) {
            lVar1 = list[1] + 2 + iVar3;
            list[2] = lVar1;
            PECMD_AllocString(list, lVar1);
            memcpy((uint8_t *)(*list + list[1] * 2), local_res20, iVar3 * 2);
            list[1] += iVar3;
        }
        PECMD_FreeStrBuf((WCHAR **)&local_res20);
    }
}

/* ========== PECMD_DeleteDirectoryTree @0x1400217bc ==========
 * 递归删除目录/文件树; 返回错误码。
 */
int64_t PECMD_DeleteDirectoryTree(LPCWSTR path, uint32_t flags)
{
    WCHAR WVar1;
    int iVar2;
    int iVar3;
    int iVar4;
    BOOL BVar5;
    DWORD DVar6;
    uint64_t uVar10;
    int64_t lVar9;
    LPCWSTR local_res18 = NULL;
    HANDLE local_res20 = 0;
    WIN32_FIND_DATAW local_278;
    uint32_t uVar13 = flags & 0xfffffffe;
    uint64_t uVar12 = 0;

    DVar6 = 0;
    WVar1 = *path;
    while (WVar1 != L'\0') {
        iVar2 = lstrlenW(path);
        iVar3 = lstrlenW(path);
        if (path[iVar3 - 1] == L'\\') {
            ((WCHAR *)path)[iVar3 - 1] = L'\0';
            iVar3 = 2;
        }
        else {
            uVar10 = PECMD_IsDirectory(path);
            iVar3 = 0;
            if ((int)uVar10 != 0) {
                iVar3 = 1;
            }
        }
        FUN_1400702B0((WCHAR **)&local_res18, path);
        if (iVar3 != 0) {
            FUN_14006375C((WCHAR **)&local_res18, WSTR("\\*.*"));
        }
        local_res20 = 0;
        PECMD_FindFirstFileW(&local_res20, local_res18, &local_278);
        {
            WCHAR *pWVar8 = StrRChrW(local_res18, NULL, L'\\');
            if (pWVar8 != NULL) {
                *pWVar8 = L'\0';
            }
        }
        iVar4 = lstrlenW(local_res18);
        ((WCHAR *)local_res18)[iVar4] = L'\\';
        ((WCHAR *)local_res18)[iVar4 + 1] = L'\0';
        if (local_res20 == 0) {
            DVar6 = GetLastError();
            uVar12 = DVar6;
            if (DVar6 == 0) {
                uVar12 = 1;
                DVar6 = 1;
            }
        }
        else {
            do {
                ((WCHAR *)local_res18)[iVar4 + 1] = L'\0';
                FUN_14006375C((WCHAR **)&local_res18, local_278.cFileName);
                if ((local_278.dwFileAttributes & 0x10) == 0) {
                    if (uVar13 == 0) {
                        uVar10 = PECMD_ExecDashCommand(local_res18).QuadPart;
                        if ((DWORD)uVar10 != 0) {
                            uVar12 = uVar10 & 0xffffffff;
                            DVar6 = (DWORD)uVar10;
                        }
                    }
                    else {
                        BVar5 = DeleteFileW(local_res18);
                        if (BVar5 == 0) {
                            DVar6 = GetLastError();
                            uVar12 = DVar6;
                            if (DVar6 == 0) {
                                uVar12 = 1;
                                DVar6 = 1;
                            }
                        }
                    }
                }
                else if ((local_278.cFileName[0] != L'.') ||
                         ((local_278.cFileName[1] != L'\0' &&
                           ((local_278.cFileName[1] != L'.' ||
                             (local_278.cFileName[2] != L'\0')))))) {
                    lVar9 = PECMD_DeleteDirectoryTree(local_res18, uVar13);
                    DVar6 = DVar6 & -(uint32_t)((int)lVar9 != 0);
                    uVar12 = DVar6;
                }
                BVar5 = FindNextFileW(local_res20, &local_278);
            } while (BVar5 != 0);
        }
        if ((local_res20 != 0) && (local_res20 != INVALID_HANDLE_VALUE)) {
            FindClose(local_res20);
        }
        if (iVar3 != 0) {
            if (uVar13 == 0) {
                uVar10 = PECMD_ExecDashCommand(path).QuadPart;
                DVar6 = (DWORD)uVar12;
                if ((DWORD)uVar10 != 0) {
                    uVar12 = uVar10 & 0xffffffff;
                    DVar6 = (DWORD)uVar10;
                }
            }
            else {
                BVar5 = RemoveDirectoryW(path);
                if (BVar5 == 0) {
                    DVar6 = GetLastError();
                    if (DVar6 == 0) {
                        DVar6 = 1;
                    }
                    uVar12 = DVar6;
                }
            }
        }
        PECMD_FreeStrBuf((WCHAR **)&local_res18);
        WVar1 = path[iVar2 + (int)(flags & 1)];
        path += iVar2 + (int)(flags & 1);
    }
    return (int64_t)(int)DVar6;
}

/* ========== PECMD_ParseVkKeyName @0x1400226e4 ==========
 * 键名/虚拟键解析: 返回 VK 码或字符。
 */
uint32_t PECMD_ParseVkKeyName(LPCWSTR s, char allowChar)
{
    WCHAR WVar1;
    uint32_t uVar2;
    uint64_t uVar3;
    LPCWSTR pWVar4;
    uint16_t uVar5;
    uint64_t uVar6;
    LPCWSTR local_res8;
    uint32_t local_res18[4];

    local_res18[0] = 0xffffffff;
    WVar1 = *s;
    uVar6 = (uint64_t)(uint16_t)WVar1;
    if (WVar1 == L'#') {
        local_res8 = s + 1;
    }
    else {
        local_res8 = s;
        if (((WVar1 != L'0') || ((WCHAR)(s[1] | 0x20U) != 0x78)) &&
            ((9 < (uint16_t)(WVar1 + 0xff10) || (9 < (uint16_t)(s[1] + 0xff10))))) {
            uVar3 = PECMD_IsVkPrefix((WCHAR *)s);
            pWVar4 = s;
            if ((int16_t)uVar6 != 0) {
                do {
                    uVar5 = (uint16_t)uVar6;
                    if ((uVar5 == 0x2c) || (((8 < uVar5 && (uVar5 < 0xe)) || (uVar5 == 0x20))))
                        break;
                    pWVar4++;
                    uVar6 = (uint64_t)(uint16_t)*pWVar4;
                } while (*pWVar4 != L'\0');
                if ((s < pWVar4) && ((pWVar4[-1] == L'_' || (pWVar4[-1] == L'^')))) {
                    pWVar4--;
                }
            }
            uVar2 = PECMD_ParseVirtualKeyCode(s, (int)((int64_t)pWVar4 - (int64_t)s) / 2);
            if (0 < (int)uVar2) {
                return uVar2;
            }
            if ((int)uVar3 != 0) {
                return uVar2;
            }
            PECMD_SkipLeadingControls((WCHAR **)&local_res8);
            if (*local_res8 == L'\0') {
                return local_res18[0];
            }
            if (((allowChar == '\0') && (WVar1 = local_res8[1], WVar1 != L',')) &&
                ((((uint16_t)WVar1 < 9 || (0xd < (uint16_t)WVar1)) &&
                  ((WVar1 != L' ' && (WVar1 != L'\0')))))) {
                return local_res18[0];
            }
            if ((uint16_t)*local_res8 < 0x61) {
                return (uint32_t)(uint16_t)*local_res8;
            }
            if (0x7a < (uint16_t)*local_res8) {
                return (uint32_t)(uint16_t)*local_res8;
            }
            return (uint32_t)(uint16_t)*local_res8 - 0x20;
        }
    }
    PECMD_ParseShortStore((WCHAR **)&local_res8, (int *)local_res18, 0x2c);
    return local_res18[0];
}

/* ========== PECMD_RemoveDuplicateChar @0x140025274 ==========
 * 删除连续重复的 allowChar; 返回第一个可替换位置/尾位置。
 */
WCHAR *PECMD_RemoveDuplicateChar(LPCWSTR s, WCHAR allowChar)
{
    int iVar1;
    WCHAR *pWVar2;
    WCHAR *pWVar3;
    LPCWSTR pWVar4;

    iVar1 = lstrlenW(s);
    pWVar4 = s + iVar1;
    pWVar2 = (WCHAR *)s;
    if (*s != L'\0') {
        do {
            if (((allowChar == *pWVar2) && (pWVar3 = pWVar2 + 1, *pWVar3 != L'=')) &&
                (*pWVar3 != L'\'')) {
                if (allowChar != *pWVar3) {
                    return pWVar2;
                }
                memmove(pWVar2, pWVar3, (int)((int64_t)pWVar4 - (int64_t)pWVar2) / 2 * 2);
                pWVar4--;
                for (; *pWVar3 == allowChar; pWVar3++) {
                    pWVar2 = pWVar3;
                }
            }
            pWVar2++;
        } while (*pWVar2 != L'\0');
        if (*s != L'\0') {
            iVar1 = lstrlenW(s);
            if (s[iVar1 - 1] != allowChar) {
                return NULL;
            }
            return (WCHAR *)s + iVar1 - 1;
        }
    }
    return NULL;
}

/* ========== PECMD_TrimTrailingSeparator @0x140025334 ==========
 * 从尾部删除参数分隔符; 返回处理后的串首。
 */
LPCWSTR PECMD_TrimTrailingSeparator(int64_t *list, LPCWSTR s, WCHAR sep)
{
    LPCWSTR pWVar1;
    int iVar2;
    LPCWSTR pWVar3;
    LPCWSTR pWVar4;
    LPCWSTR pWVar5;

    iVar2 = lstrlenW(s);
    pWVar5 = s + iVar2;
    pWVar4 = pWVar5;
    do {
        do {
            pWVar1 = pWVar4;
            pWVar4 = pWVar1 - 1;
            pWVar3 = s;
            if (pWVar4 < s) {
                PECMD_StrBldCopyWide(list, pWVar3);
                if (pWVar3 == s) {
                    *(WCHAR *)s = L'\0';
                }
                return s;
            }
        } while (((sep != *pWVar4) || (*pWVar1 == L'=')) || (*pWVar1 == L'\''));
        if ((pWVar4 <= s) || (sep != pWVar1[-2])) {
            *(WCHAR *)pWVar4 = L'\0';
            pWVar3 = pWVar1;
            PECMD_StrBldCopyWide(list, pWVar3);
            if (pWVar3 == s) {
                *(WCHAR *)s = L'\0';
            }
            return s;
        }
        memmove((void *)pWVar4, (void *)pWVar1, (int)((int64_t)pWVar5 - (int64_t)pWVar4) / 2 * 2);
        pWVar5--;
        do {
            pWVar1 = pWVar4 - 1;
            if (*pWVar1 != sep)
                break;
            pWVar4 = pWVar1;
        } while (s < pWVar1);
    } while (1);
}

/* ========== PECMD_KeyboardHookProc @0x1400259a4 ==========
 * 键盘钩子: 过滤自定义热键, 记录最后按键。
 */
LRESULT PECMD_KeyboardHookProc(int nCode, WPARAM wParam, char *kb)
{
    char cVar1;
    uint16_t uVar2;
    LRESULT LVar3;
    uint16_t *puVar4;
    uint16_t uVar5;
    BOOL bVar6;

    g_randState = (int32_t)g_randState + 0xb;
    uVar5 = 0;
    bVar6 = 0;
    if (nCode < 0) {
        LVar3 = CallNextHookEx(g_hHook, nCode, wParam, (LPARAM)kb);
    }
    else {
        LVar3 = 1;
        if ((g_pHookData != NULL) && (nCode == 0)) {
            EnterCriticalSection(&g_csHook);
            puVar4 = (uint16_t *)g_pHookData;
            bVar6 = 0;
            if ((g_pHookData != NULL) && (*puVar4 != 0)) {
                cVar1 = *kb;
                if ((kb[8] & 0x20U) != 0) {
                    uVar5 = 0x100;
                }
                uVar2 = (uint16_t)GetKeyState(0x11);
                if ((uVar2 & 0x8000) != 0) {
                    uVar5 |= 0x200;
                }
                uVar2 = (uint16_t)GetKeyState(0x10);
                if ((uVar2 & 0x8000) != 0) {
                    uVar5 |= 0x400;
                }
                for (uVar2 = *puVar4; puVar4++, bVar6 = 0, 0 < (int16_t)uVar2; uVar2--) {
                    if ((cVar1 == (char)*puVar4) &&
                        ((*puVar4 & 0xff00) == (*puVar4 & 0xff00 & uVar5))) {
                        bVar6 = 1;
                        break;
                    }
                }
            }
            LeaveCriticalSection(&g_csHook);
        }
        if (wParam == 0x100) {
            *(uint32_t *)kb = 0; /* g_hookBusyFlag = *(undefined4*)kb; 桩 */
        }
        if (!bVar6) {
            LVar3 = CallNextHookEx(g_hHook, nCode, wParam, (LPARAM)kb);
        }
    }
    return LVar3;
}

/* ========== PECMD_OneTimeInitBody @0x140027dbc ==========
 * 一次性初始化体: 解析/修正系统内部句柄并建立互指。
 */
uint64_t PECMD_OneTimeInitBody(void)
{
    int iVar1;
    uint32_t uVar2;
    uint64_t uVar3;
    uint64_t uVar4;
    uint32_t local_res8 = 0;

    uVar3 = FUN_14001D628();
    if (((int)uVar3 == 0) || (uVar4 = PECMD_MapPhysicalMemoryNT5(), uVar4 == 0)) {
        uVar3 = 0;
    }
    else {
        iVar1 = PECMD_ReadPhysMemSlot(0xffdff124);
        iVar1 = PECMD_ReadPhysMemSlot((uint64_t)(iVar1 + 0x44));
        uVar2 = local_res8;
        if (g_sysVerClass == 0) {
            local_res8 = PECMD_ReadPhysMemSlot((uint64_t)(iVar1 + 0xa0));
            uVar2 = PECMD_ReadPhysMemSlot((uint64_t)(iVar1 + 0xa4));
        }
        if (g_sysVerClass == 1) {
            local_res8 = PECMD_ReadPhysMemSlot((uint64_t)(iVar1 + 0x88));
            uVar2 = PECMD_ReadPhysMemSlot((uint64_t)(iVar1 + 0x8c));
        }
        PECMD_WritePhysicalMem((uint64_t)(local_res8 + 4), uVar2);
        PECMD_WritePhysicalMem((uint64_t)uVar2, local_res8);
        CloseHandle(g_u64cd08);
        PECMD_FreeNtdll();
        uVar3 = 1;
    }
    return uVar3;
}

/* ========== PECMD_FindQuotedToken @0x1400280a8 ==========
 * 带引号感知的 token 查找: 跳过 '...'/"..." 后再查。
 */
LPCWSTR PECMD_FindQuotedToken(LPCWSTR s, LPCWSTR sep, int mode, uint32_t flags)
{
    WCHAR WVar1;
    LPCWSTR pWVar2;
    WCHAR WVar3;
    LPCWSTR local_res8;

    while (1) {
        local_res8 = NULL;
        WVar1 = *s;
        WVar3 = L'\0';
        if ((WVar1 == L'\"') || (WVar1 == L'\'')) {
            s++;
            local_res8 = s;
            FUN_1400F429C((WCHAR **)&local_res8, WVar1);
            WVar3 = *local_res8;
            if (WVar3 != L'\0') {
                *(WCHAR *)local_res8 = L'\0';
            }
        }
        pWVar2 = FUN_140062EC8(s, sep, mode, flags);
        if (WVar3 != L'\0') {
            *(WCHAR *)local_res8 = WVar3;
            local_res8++;
        }
        if (pWVar2 != NULL)
            break;
        if ((local_res8 == NULL) || (s = local_res8, *local_res8 == L'\0')) {
            return NULL;
        }
    }
    return pWVar2;
}

/* ========== PECMD_ScanMenuRecursive @0x140023080 ==========
 * 递归扫描菜单, 收集项到动态数组; 返回第一个满足位置的项索引。
 */
UINT PECMD_ScanMenuRecursive(HMENU menu, int64_t *arr, int *count, LPCWSTR buf, int minId)
{
    int iVar1;
    UINT uPosition;
    HMENU pHVar2;
    WCHAR **puVar3;
    WCHAR **puVar4;
    LPCWSTR pWVar5;
    LPWSTR pWVar6;
    WCHAR WVar7;
    LPCWSTR lpStart;
    UINT uIDItem;
    UINT UVar8;
    int local_res20;

    UVar8 = 0xffffffff;
    local_res20 = 0;
    uIDItem = 0;
    do {
        if (2 < local_res20) {
            return UVar8;
        }
        pHVar2 = GetSubMenu(menu, uIDItem);
        if (pHVar2 == 0) {
            *(WCHAR *)buf = L'\0';
            uPosition = GetMenuItemID(menu, uIDItem);
            if (((int)UVar8 < 0) && (minId <= (int)uPosition)) {
                UVar8 = uIDItem;
            }
            GetMenuStringW(menu, uIDItem, (LPWSTR)buf, 0x1fe, 0x400);
            puVar3 = (WCHAR **)calloc(1, sizeof(WCHAR *));
            puVar4 = NULL;
            if (puVar3 != NULL) {
                FUN_1400702B0(puVar3, buf);
                puVar4 = puVar3;
            }
            iVar1 = *count;
            *count = iVar1 + 1;
            puVar3 = (WCHAR **)FUN_140063B00((int64_t)iVar1, arr, arr + 1, 8);
            *puVar3 = (WCHAR *)puVar4;
            if (*buf == L'\0') {
                local_res20++;
            }
            else {
                local_res20 = 0;
                if (puVar4 != NULL && *puVar4 != NULL) {
                    *(uint16_t *)*puVar4 = (uint16_t)uPosition;
                }
                pWVar5 = FUN_1400169BC((int)uPosition, NULL);
                lpStart = buf;
                if (pWVar5 != NULL) {
                    lpStart = pWVar5;
                }
                pWVar6 = StrChrW(lpStart, L',');
                if ((pWVar6 != NULL) || (pWVar5 != NULL)) {
                    WVar7 = L'\0';
                    if (pWVar6 != NULL) {
                        WVar7 = *pWVar6;
                        *pWVar6 = L'\0';
                    }
                    ModifyMenuW(menu, uPosition, 0, (uintptr_t)(int)uPosition, lpStart);
                    if ((WVar7 != L'\0') && (pWVar5 != NULL)) {
                        *pWVar6 = WVar7;
                    }
                }
            }
        }
        else {
            PECMD_ScanMenuRecursive(pHVar2, arr, count, buf, -1);
        }
        uIDItem++;
    } while ((int)uIDItem < 500);
    return UVar8;
}

/* ========== PECMD_BubbleSort @0x140023268 ==========
 * 冒泡排序: 元素宽 count, 比较函数 buf。
 */
void PECMD_BubbleSort(uint8_t *base, int64_t count, int64_t width, void *cmp)
{
    int iVar1;
    int64_t lVar2;
    int64_t lVar3;
    uint8_t *puVar4;
    int64_t lVar5;
    uint8_t *local_res8 = NULL;
    int64_t local_res10;

    local_res10 = count;
    FUN_1400633A8((WCHAR **)&local_res8, width);
    if (0 < count) {
        lVar2 = 1;
        lVar5 = count;
        do {
            if (lVar2 < count) {
                lVar3 = count - lVar2;
                puVar4 = base;
                do {
                    puVar4 += width;
                    iVar1 = ((int (*)(const void *, const void *))cmp)(base, puVar4);
                    if (0 < iVar1) {
                        iVar1 = (int)width;
                        memcpy(local_res8, base, iVar1);
                        memcpy(base, puVar4, iVar1);
                        memcpy(puVar4, local_res8, iVar1);
                    }
                    lVar3--;
                    count = local_res10;
                } while (lVar3 != 0);
            }
            lVar2++;
            base += width;
            lVar5--;
        } while (lVar5 != 0);
    }
    PECMD_FreeStrBuf((WCHAR **)&local_res8);
}

/* ========== PECMD_StartOnlyApp @0x14002ae88 ==========
 * OnlyApp 启动: 读 OnlyApp 标志, 按需执行 "-udm OnlyApp"。
 */
void PECMD_StartOnlyApp(LPCWSTR cmdline)
{
    LONG iVar1;
    DWORD local_res8[2];
    DWORD local_res18[2];
    WCHAR local_78[56];
    WCHAR *local_res20 = NULL;

    local_78[0] = 0;
    local_res18[0] = 0;
    local_res8[0] = 100;
    iVar1 = ((LONG (*)(HKEY, LPCWSTR, LPCWSTR, DWORD *, void *, DWORD *))g_pSHGetValueW)(
        (HKEY)0xffffffff80000002, WSTR("SOFTWARE\\PELOGON\\RAMDATA"), WSTR("OnlyApp"), local_res18,
        local_78, local_res8);
    if (iVar1 != 0) {
        local_78[0] = 0;
    }
    if (local_78[0] < 0x31) {
        PECMD_FixKnownDlls32();
        if (PECMD_CheckSystemStartOptions() == 0) {
            PECMD_InitRamdataRegistry(0);
            ((LONG (*)(HKEY, LPCWSTR, LPCWSTR, DWORD, const void *, DWORD))g_pSHSetValueW)(
                (HKEY)0xffffffff80000002, WSTR("SOFTWARE\\PELOGON\\RAMDATA"), WSTR("OnlyApp"), 1,
                g_wsz20c64, 2);
        }
        else {
            FUN_1400702B0(&local_res20, WSTR("-udm OnlyApp "));
            if (cmdline != NULL) {
                FUN_14006375C(&local_res20, cmdline);
            }
            PECMD_MounMountWimImage(g_Script, local_res20, 0);
            PECMD_FreeStrBuf(&local_res20);
        }
    }
}

/* ========== PECMD_RunProcessCommand @0x14002afa4 ==========
 * 构造命令行并创建进程; 等待结束并回写结果。
 * TODO(verify): 命令行偏移/空格细节按反编译保留。
 */
uint32_t PECMD_RunProcessCommand(int64_t ctx, LPCWSTR cmd, uint64_t arg1, uint64_t arg2,
                                 uint64_t flags)
{
    BOOL bVar1;
    DWORD DVar2;
    int iVar4;
    int iVar5;
    int iVar6;
    uint8_t *puVar7;
    uint16_t *psVar8;
    uint16_t *puVar9;
    LPCWSTR lpString;
    LPCWSTR lpString_00;
    int64_t lVar10;
    int64_t lVar11;
    uint16_t sVar3;
    uint16_t sVar12;
    DWORD local_res18[2];
    int local_res20;
    uint8_t *local_d0 = NULL;
    PROCESS_INFORMATION local_c8;
    STARTUPINFOW local_a8;

    lpString_00 = (LPCWSTR)(flags & 0xfffffffffffffffeULL);
    lpString = (LPCWSTR)(arg2 & 0xfffffffffffffffeULL);
    if (((arg2 & 1) == 0) || (bVar1 = 1, *(int64_t *)(ctx + 0x1b8) == 0)) {
        bVar1 = 0;
    }
    local_res20 = lstrlenW(cmd);
    iVar4 = lstrlenW((LPCWSTR)(arg1 & 0xfffffffffffffffeULL));
    if (lpString == NULL) {
        iVar5 = 0;
    }
    else {
        iVar5 = lstrlenW(lpString);
    }
    if (lpString_00 == NULL) {
        iVar6 = 0;
    }
    else {
        iVar6 = lstrlenW(lpString_00);
    }
    PECMD_AllocWStringBuffer((WCHAR **)&local_d0,
                             (int64_t)(iVar5 + 0x416 + iVar6 * 2 + iVar4 + local_res20));
    puVar7 = local_d0;
    memcpy(local_d0, WSTR("    *="), 0xc);
    lVar10 = (int64_t)local_res20 * 2;
    puVar7 += 0xc;
    memcpy(puVar7, cmd, (int)lVar10);
    sVar3 = 0x20;
    psVar8 = (uint16_t *)(puVar7 + lVar10);
    lVar10 = 2;
    if (lpString == NULL) {
        sVar12 = 0x22;
    }
    else {
        *psVar8 = 0x20;
        psVar8[1] = 0x2d;
        psVar8[2] = 0x6f;
        psVar8[3] = 0x22;
        lVar11 = (int64_t)iVar5 * 2;
        psVar8 += 4;
        memcpy(psVar8, lpString, (int)lVar11);
        sVar12 = (uint16_t)lVar10 + 0x20;
        *(uint16_t *)((uint8_t *)psVar8 + lVar11) = sVar12;
        psVar8 = (uint16_t *)((uint8_t *)psVar8 + lVar11 + lVar10);
        sVar3 = (uint16_t)lVar10 + 0x1e;
    }
    *psVar8 = sVar3;
    *(uint16_t *)((uint8_t *)psVar8 + lVar10) = sVar12;
    lVar11 = (int64_t)iVar4 * 2;
    puVar7 = (uint8_t *)psVar8 + lVar10 + lVar10;
    memcpy(puVar7, (const void *)(arg1 & 0xfffffffffffffffeULL), (int)lVar11);
    *(uint16_t *)(puVar7 + lVar11) = sVar12;
    puVar9 = (uint16_t *)(puVar7 + lVar11 + lVar10);
    if (iVar6 != 0) {
        *puVar9 = 0x20;
        lVar11 = (int64_t)iVar6 * 2;
        *(uint16_t *)((uint8_t *)puVar9 + lVar10) = sVar12;
        puVar7 = (uint8_t *)puVar9 + lVar10 + lVar10;
        memcpy(puVar7, lpString_00, (int)lVar11);
        *(uint16_t *)(puVar7 + lVar11) = sVar12;
        puVar9 = (uint16_t *)(puVar7 + lVar11 + lVar10);
    }
    *puVar9 = 0;
    local_res18[0] = 1;
    memset(&local_c8, 0, sizeof(local_c8));
    memset(&local_a8, 0, sizeof(local_a8));
    local_a8.cb = 0x68;
    local_a8.wShowWindow = 0;
    local_a8.dwFlags = 0x81;
    CreateProcessW(NULL, (LPWSTR)(local_d0 + 0xc), NULL, NULL, 0,
                   (((uint32_t)(arg1 << 7)) & 0x80) | 0x1000004, NULL, NULL, &local_a8, &local_c8);
    if (bVar1) {
        if (local_c8.hProcess != 0) {
            AssignProcessToJobObject(*(HANDLE *)(ctx + 0x1b8), local_c8.hProcess);
            goto LAB_14002b229;
        }
        local_res18[0] = GetLastError();
        if (local_c8.hProcess == 0)
            goto LAB_14002b28d;
    }
    else {
    LAB_14002b229:
        if (local_c8.hProcess == 0) {
            local_res18[0] = GetLastError();
            goto LAB_14002b28d;
        }
    }
    ResumeThread(local_c8.hThread);
    WaitForSingleObject(local_c8.hProcess, 0xffffffff);
    GetExitCodeProcess(local_c8.hProcess, local_res18);
    CloseHandle(local_c8.hThread);
    CloseHandle(local_c8.hProcess);
LAB_14002b28d:
    if ((flags & 1) == 0) {
        FUN_140025f10(ctx + 8, (LPCWSTR)(local_d0 + 0x12), local_res18[0], (void *)0x1100, NULL,
                      NULL);
    }
    DVar2 = local_res18[0];
    PECMD_FreeStrBuf((WCHAR **)&local_d0);
    return DVar2;
}

/* ========== PECMD_DevconUpdate @0x14002c4bc ==========
 * 执行 "!=devcon update ..." 并显示安装结果。
 */
uint32_t PECMD_DevconUpdate(int64_t ctx, LPCWSTR inf, LPCWSTR hwid, int mode)
{
    DWORD DVar1;
    int iVar2;
    FILETIME local_38;
    WCHAR *local_30 = NULL;
    WCHAR *local_28[2];

    PECMD_AllocStrSlot(&local_30);
    FUN_14006375C(&local_30, WSTR("!=devcon update  \""));
    FUN_14006375C(&local_30, inf);
    FUN_14006375C(&local_30, WSTR("\" \""));
    FUN_14006375C(&local_30, hwid);
    FUN_14006375C(&local_30, WSTR("\""));
    iVar2 = lstrlenW(local_30);
    PECMD_AllocString(&local_30, iVar2 + 5);
    local_38.dwLowDateTime = 0;
    local_38.dwHighDateTime = 0;
    PECMD_ExecCmdDispatch(g_Script, local_30, g_Script, 0, 0, &local_38, 0, 0);
    if ((*(int64_t *)(ctx + 0x110) != 0) && (local_38.dwLowDateTime == 0)) {
        PECMD_AppendKeyIfMissing(ctx, hwid, mode);
    }
    PECMD_AllocWStringBuffer(local_28, 0x2800);
    _snwprintf(local_28[0], 0x27ff, WSTR("Devcon安装驱动【%s】[%s]"), hwid, inf);
    FUN_140025f10(ctx + 8, local_28[0], local_38.dwLowDateTime, (void *)0x1100, NULL, NULL);
    PECMD_FreeStrBuf(local_28);
    DVar1 = local_38.dwLowDateTime;
    PECMD_FreeStrBuf(&local_30);
    return DVar1;
}

/* ========== PECMD_ShowDriverInstallMsg @0x14002c7dc ==========
 * MyInstallHinfSection 驱动安装提示。
 */
uint64_t PECMD_ShowDriverInstallMsg(uint64_t a, uint64_t b, uint64_t c, uint64_t flags)
{
    (void)a;
    (void)b;
    WCHAR *local_18[2];
    LPCWSTR puVar1;

    SetLastError(0);
    ((void (*)(void))g_pInstallHinfSectionW)();
    PECMD_AllocWStringBuffer(local_18, 0x2800);
    puVar1 = g_szEmpty;
    if ((flags & 0xffff0000) != 0) {
        puVar1 = g_wsz21014;
    }
    _snwprintf(local_18[0], 0x27ff, WSTR("MyInstallHinfSection安装驱动%s【%s】\r\n"), puVar1,
               (LPCWSTR)c, 0);
    FUN_140025f10(0x14013d130, local_18[0], 0, (void *)0x11, NULL, NULL);
    PECMD_FreeStrBuf(local_18);
    return 0;
}

/* ========== PECMD_SkipSwitches @0x14002f1ec ==========
 * 跳过连续以 '-' 开头的开关参数。
 */
void PECMD_SkipSwitches(uint16_t *p)
{
    uint16_t uVar1;
    uint16_t *local_res8;
    int64_t local_res10;
    uint16_t *local_res18[2];

    local_res8 = p;
    PECMD_SkipLeadingControls((WCHAR **)&local_res8);
    if (*local_res8 == 0x2d) {
        uVar1 = 0x2d;
        do {
            while ((((uVar1 != 0 && (uVar1 != 0x22)) && ((uVar1 < 9 || (0xd < uVar1)))) &&
                    (uVar1 != 0x20))) {
                local_res8++;
                uVar1 = *local_res8;
            }
            local_res10 = 0;
            local_res18[0] = local_res8;
            if (*local_res8 == 0x22) {
                FUN_140024C48((int64_t *)local_res18, &local_res10, 5);
            }
            PECMD_SkipLeadingControls((WCHAR **)&local_res8);
            uVar1 = *local_res8;
        } while (uVar1 == 0x2d);
    }
}

/* ========== PECMD_InstallFonts @0x14002f2dc ==========
 * 安装/移除目录下所有字体; 返回是否有失败。
 */
BOOL PECMD_InstallFonts(void *dir, int remove)
{
    int iVar1;
    int iVar4;
    HANDLE local_res18 = 0;
    WCHAR *local_res20 = NULL;
    WIN32_FIND_DATAW local_268;
    WCHAR *pWVar3;

    PECMD_AllocWStringBuffer((WCHAR **)&local_res20, 0x208);
    iVar4 = 1;
    local_res18 = 0;
    PECMD_FindFirstFileW(&local_res18, (LPCWSTR)dir, &local_268);
    pWVar3 = StrRChrW((LPCWSTR)dir, NULL, L'\\');
    if (pWVar3 != NULL) {
        *pWVar3 = L'\0';
    }
    if (local_res18 != 0) {
        do {
            if ((local_268.dwFileAttributes & 0x10) == 0) {
                PECMD_CrtShim(local_res20, 0x208, (void *)dir, (void *)local_268.cFileName);
                if (remove == 0) {
                    iVar1 = AddFontResourceW(local_res20);
                }
                else {
                    iVar1 = RemoveFontResourceW(local_res20);
                }
                iVar4 += iVar1;
            }
        } while (FindNextFileW(local_res18, &local_268) != 0);
        if ((local_res18 != 0) && (local_res18 != INVALID_HANDLE_VALUE)) {
            FindClose(local_res18);
        }
        local_res18 = 0;
        PECMD_StartWorkerThread((void *)(uintptr_t)g_Script, NULL, 0x1d, 0, 0, 0xffffffff, 5000, 1,
                                0);
    }
    if ((local_res18 != 0) && (local_res18 != INVALID_HANDLE_VALUE)) {
        FindClose(local_res18);
    }
    local_res18 = 0;
    PECMD_FreeStrBuf(&local_res20);
    return iVar4 == 0;
}

/* ========== PECMD_RunRamdriv @0x14002fc48 ==========
 * 执行 ImDisk/Ramdriv 内存盘命令。
 */
uint64_t PECMD_RunRamdriv(int64_t *ctx, LPCWSTR cmd)
{
    int iVar1;
    uint32_t uVar2;
    uint64_t uVar3;
    WCHAR WVar4;
    LPCWSTR local_res10;
    LPCWSTR local_res20;
    WCHAR *local_res18 = NULL;
    WCHAR *local_28[2] = {NULL, NULL};

    if (g_flagA24F < 1) {
        uVar3 = 0;
    }
    else {
        local_res10 = cmd;
        iVar1 = StrCmpNIW(cmd, WSTR("ImDisk"), 6);
        if (iVar1 == 0) {
            uVar3 = PECMD_RamdMountImDisk(cmd, ctx);
            uVar3 = (uint64_t)(int)uVar3;
        }
        else {
            PECMD_AllocWStringBuffer((WCHAR **)&local_res18, 0x50);
            FUN_1400702B0(local_28, WSTR("%ramdrv%"));
            FUN_14007BF44(ctx, local_28[0], &local_res18, 0, 1);
            iVar1 = StrCmpNIW(cmd, WSTR("Ramdriv"), 7);
            if (iVar1 == 0) {
                local_res10 = cmd + 7;
                PECMD_SkipLeadingControls((WCHAR **)&local_res10);
                WVar4 = L',';
                cmd = local_res10;
                if (*local_res10 == L',') {
                    cmd = local_res10 + 1;
                }
                local_res20 = cmd;
                FUN_1400F429C((WCHAR **)&local_res20, 0x2c);
                if (*local_res20 == WVar4) {
                    *(WCHAR *)local_res20 = L'\0';
                }
            }
            uVar2 = PECMD_ParseSize((LPWSTR)cmd, 1, 0x200, 0x20);
            PECMD_SetRamdrivDiskSize((int)(uVar2 << 0x14), local_res18);
            PECMD_QueryDiskSpace(local_res18);
            uVar3 = 1;
            PECMD_FreeStrBuf(local_28);
            PECMD_FreeStrBuf(&local_res18);
        }
    }
    return uVar3;
}

/* ========== PECMD_RunShutdownScript @0x14003d478 ==========
 * 关机/注销脚本: 按位标志选择动作并执行 OnShutdown.wcs。
 */
void PECMD_RunShutdownScript(LPCWSTR args, uint32_t flags)
{
    int iVar1;
    LPCWSTR pwVar2;
    LPCWSTR pwVar3;
    WCHAR *local_res8 = NULL;
    WCHAR *local_res18 = NULL;

    PECMD_AllocWStringBuffer((WCHAR **)&local_res8, 0x20a);
    local_res8[0] = L' ';
    local_res8[1] = L'\0';
    GetEnvironmentVariableW(WSTR("SystemRoot"), local_res8 + 2, 0x208);
    FUN_14006375C(&local_res8, WSTR("\\System32\\OnShutdown.wcs "));
    pwVar2 = WSTR("shutdown");
    if ((flags & 0x20) == 0) {
        pwVar2 = WSTR("unknown");
    }
    pwVar3 = WSTR("reboot");
    if ((flags & 0x10) == 0) {
        pwVar3 = pwVar2;
    }
    pwVar2 = WSTR("logout");
    if ((flags & 2) == 0) {
        pwVar2 = pwVar3;
    }
    pwVar3 = WSTR("suspend");
    if ((flags & 4) == 0) {
        pwVar3 = pwVar2;
    }
    pwVar2 = WSTR("hiber");
    if ((flags & 8) == 0) {
        pwVar2 = pwVar3;
    }
    pwVar3 = WSTR("poweroff");
    if ((flags & 0x40) == 0) {
        pwVar3 = pwVar2;
    }
    pwVar2 = WSTR("lock");
    if ((flags & 0x80) == 0) {
        pwVar2 = pwVar3;
    }
    iVar1 = lstrlenW(local_res8);
    FUN_14006375C(&local_res8, pwVar2);
    FUN_14006375C(&local_res8, WSTR(" "));
    if (args != NULL) {
        FUN_14006375C(&local_res8, args);
    }
    PECMD_RunCommand(g_Script, local_res8);
    FUN_1400702B0(&local_res18, WSTR(" #25|INDATA "));
    FUN_14006375C(&local_res18, local_res8 + iVar1 - 6);
    if ((flags & 0x70) != 0) {
        PECMD_RunCommand(g_Script, local_res18);
    }
    PECMD_FreeStrBuf(&local_res18);
    PECMD_FreeStrBuf(&local_res8);
}

/* ========== PECMD_WriteSysAck @0x14003e694 ==========
 * 写 SysShel.ack 应答; 必要时执行 AfterRun。
 */
void PECMD_WriteSysAck(uint32_t ack, int mode)
{
    uint8_t bVar1;
    uint32_t local_res8[2];
    WCHAR local_a8[2];
    uint64_t local_a4[19];

    local_res8[0] = ack & 1;
    PECMD_InitRamdataRegistry(0);
    ((LONG (*)(HKEY, LPCWSTR, LPCWSTR, DWORD, const void *, DWORD))g_pSHSetValueW)(
        (HKEY)0xffffffff80000002, WSTR("SOFTWARE\\PELOGON\\RAMDATA"), WSTR("SysShel.ack"), 4,
        local_res8, 4);
    if (local_res8[0] != 0) {
        bVar1 = (uint8_t)(-(mode != 0) & 2);
        if (g_answerFlag == 1) {
            bVar1 |= 1;
        }
        local_a8[0] = (WCHAR)(bVar1 + L'0');
        local_a8[1] = 0;
        memset(local_a4, 0, sizeof(local_a4));
        wsprintfW(local_a8, WSTR("%d %lu"), (int)bVar1, (unsigned long)local_res8[0]);
        PECMD_ExecIndataCommand(WSTR("AfterRun"), local_a8);
    }
}
