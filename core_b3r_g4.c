/*
 * core_b3r_g4.c — 还原 3 个业务函数 (batch28)
 *
 *   PECMD_RegScrubKey @0x140071ae8  registry subkey/value scrub & delete
 *   PECMD_SntpResolveServer @0x140072e94  SNTP/NTP time-server name resolution + setvar
 *   PECMD_ReadTipDummyConfig @0x140074f14  PELOGON TIPSDUMMY config read from registry/INI
 *
 * NTP 函数中有多处把按 WCHAR 构建的缓冲当作 ASCII 字节串处理
 * (gethostbyname / IP 地址字节) —— 按约定归一化为 byte 指针并标注 TODO(verify)。
 */

#include "win32_stub.h"
#include "pecmd_defs.h"
#include <string.h>
#include <stdbool.h>

/* =====================================================================
 * 本文件引用的全局数据 (extern, 定义在其他翻译单元)
 * ===================================================================== */
extern WCHAR g_szEmpty[];    /* .rdata 空串 */
extern char * g_timeServer;      /* time-server 名 / 解析结果 (char*) */
extern int32_t g_qMode;      /* -q/-qo 模式 */
extern int64_t DAT_14013d5c0;      /* 临时缓冲指针槽 */
extern int32_t g_i32D6F4;      /* config dirty flag */
extern void *g_pConfigStr;      /* config string pointer */

/* 动态导入的函数指针槽 */

/* =====================================================================
 * 未被 win32_stub.h / pecmd_defs.h 声明的辅助函数 (extern, 不在此定义)
 * ===================================================================== */
extern LPWSTR  lstrcatW(LPWSTR, LPCWSTR);

extern void    PECMD_InitWinsock(void *p);                 /* 懒加载 WS2_32 指针 */
extern int32_t PECMD_QueryState_cfc0(void);                    /* 初始化成功? */
extern void    PECMD_InitWinsockOnce(void *p);                 /* 附加初始化 */
extern void    PECMD_WideToAnsiStr(int64_t *ps, LPCWSTR src, int64_t len,
                             uint64_t cap);            /* 复制/解析串 */
extern void    PECMD_SplitTokenTrimWs(int64_t *src, int64_t *dst, int16_t delim); /* 切分 */
extern void    FUN_1400633a8(void **ps, int64_t len);  /* 分配缓冲 */
extern void    PECMD_AllocStrSlot(void *ps);                /* 初始化串容器 */
extern void    PECMD_NtpSyncLoop(uint32_t *addr);          /* IP 地址 → 串 */
extern void    PECMD_SkipUntilDelim(WCHAR **pp, WCHAR ch1, WCHAR ch2); /* 行切分 */
extern HANDLE  PECMD_OpenFileHandle(HANDLE *out, LPCWSTR path, DWORD access,
                             DWORD share, LPSECURITY_ATTRIBUTES sa,
                             DWORD disp, DWORD flags, HANDLE tmpl); /* CreateFile 包装 */
extern void    PECMD_SetConfigString(WCHAR *param_1, int param_2); /* config string store */

/* ===================================================================== */
/* @0x140071ae8                                                        */
/* ===================================================================== */
/* signature: DWORD PECMD_RegScrubKey(HKEY param_1, LPCWSTR param_2, uint param_3) */
DWORD PECMD_RegScrubKey(HKEY param_1, LPCWSTR param_2, unsigned int param_3)
{
    DWORD DVar1;
    DWORD DVar2;
    DWORD DVar3;
    DWORD local_res20[2];
    DWORD local_38;
    DWORD local_34;
    DWORD local_30;
    DWORD local_2c;
    HKEY  local_28;
    LPWSTR local_20;

    local_28 = (HKEY)0;
    DVar2 = PECMD_RegOpen(param_1, param_2, &local_28, 0x2001b, 4);
    if (DVar2 == 0) {
        local_30 = 0;
        local_res20[0] = 0;
        local_34 = 0;
        local_38 = 0;
        if ((param_3 & 1) != 0) {
            RegDeleteValueW(local_28, (LPCWSTR)g_szEmpty);
        }
        DVar2 = RegQueryInfoKeyW(local_28, (LPWSTR)0, (DWORD *)0, (DWORD *)0,
                                 &local_30, local_res20, (DWORD *)0, &local_34,
                                 &local_38, (DWORD *)0, (DWORD *)0, (FILETIME *)0);
        if (local_res20[0] < local_38) {
            local_res20[0] = local_38;
        }
        PECMD_StrAlloc2(&local_20, (int64_t)(local_res20[0] * 4 + 8));
        if (DVar2 == 0) {
            DVar3 = DVar2;
            DVar1 = local_34;
            if ((param_3 & 4) != 0) {
                for (;;) {
                    DVar2 = DVar3;
                    DVar1 = DVar1 - 1;
                    if (!(-1 < (int)DVar1)) break;
                    local_2c = local_res20[0] * 4 + 1;
                    *local_20 = L'\0';
                    DVar3 = RegEnumValueW(local_28, DVar1, local_20, &local_2c,
                                          (DWORD *)0, (DWORD *)0, (BYTE *)0,
                                          (DWORD *)0);
                    if ((DVar3 == 0) && (DVar3 = DVar2, *local_20 != L'\0')) {
                        DVar2 = RegDeleteValueW(local_28, local_20);
                        if (DVar2 != 0) {
                            DVar3 = DVar2;
                        }
                    }
                }
            }
            DVar3 = DVar2;
            DVar1 = local_30;
            if ((param_3 & 2) != 0) {
                for (;;) {
                    DVar2 = DVar3;
                    DVar1 = DVar1 - 1;
                    if (!(-1 < (int)DVar1)) break;
                    local_2c = local_res20[0] * 4 + 1;
                    *local_20 = L'\0';
                    DVar3 = RegEnumKeyExW(local_28, DVar1, local_20, &local_2c,
                                          (DWORD *)0, (LPWSTR)0, (DWORD *)0,
                                          (FILETIME *)0);
                    if ((DVar3 == 0) && (DVar3 = DVar2, *local_20 != L'\0')) {
                        DVar2 = ((LONG (*)(HKEY, LPCWSTR))(uintptr_t)g_pSHDeleteKeyW)(
                            local_28, local_20);
                        if (DVar2 != 0) {
                            DVar3 = DVar2;
                        }
                    }
                }
            }
        }
        RegCloseKey(local_28);
        PECMD_StrFree(&local_20);
    }
    return DVar2;
}

/* ===================================================================== */
/* @0x140072e94  SNTP/NTP time-server name resolution                   */
/* ===================================================================== */
/* signature: undefined8 PECMD_SntpResolveServer(longlong *param_1, LPCWSTR param_2) */
uint64_t PECMD_SntpResolveServer(int64_t *param_1, LPCWSTR param_2)
{
    WCHAR WVar1;
    int   iVar4;
    int64_t lVar5;
    LPCWSTR pWVar6;
    WCHAR *pWVar7;
    uint64_t uVar8;
    char *pWVar9;
    LPCWSTR local_res10;
    WCHAR *local_res18;
    WCHAR *local_res20;
    WCHAR *local_58;
    void  *local_50;
    int64_t local_48[2];

    local_res10 = param_2;
    PECMD_InitWinsock(&local_res18);
    if (PECMD_QueryState_cfc0() == 0) {
        return 0x80004005;
    }
    PECMD_InitWinsockOnce(&local_res18);
    PECMD_SkipSpace((WCHAR **)&local_res10);
    g_timeServer = "time.windows.com";
    FUN_1400633a8((void **)local_48, 0x5dc);
    DAT_14013d5c0 = local_48[0];
    uVar8 = 1;
    PECMD_AllocStrSlot(&local_58);
    PECMD_AllocStrSlot(&local_res18);
    pWVar6 = local_res10;
    local_res20 = (WCHAR *)local_res10;
    PECMD_SplitTokenTrimWs((int64_t *)&local_res20, (int64_t *)&local_res18, 0x2c);
    *((WCHAR *)local_res18) = L'\0';
    pWVar7 = local_res18;
    if (*local_res20 == L',') {
        *local_res20 = L'\0';
        local_res20 = local_res20 + 1;
        PECMD_SplitTokenTrimWs((int64_t *)&local_res20, (int64_t *)&local_res18, 0x2c);
        pWVar7 = local_res18;
    }
    iVar4 = StrCmpNIW(pWVar6, WSTR("-q"), 2);
    if (iVar4 == 0) {
        local_res10 = pWVar6 + 2;
        g_qMode = 1;
        if (*local_res10 == L'o') {
            g_qMode = 2;
        }
        WVar1 = *local_res10;
        while (WVar1 != L'\0' &&
               ((((unsigned short)WVar1 < 9) || (0xd < (unsigned short)WVar1)) &&
                (WVar1 != L' '))) {
            local_res10 = local_res10 + 1;
            WVar1 = *local_res10;
        }
        PECMD_SkipSpace((WCHAR **)&local_res10);
        PECMD_StrAlloc(&local_58, 100);
        *((WCHAR *)local_58) = L'\0';
        g_qOutBuf = (WCHAR *)local_58;
    }
    FUN_1400633a8(&local_50, 0x14a);
    PECMD_SkipSpace((WCHAR **)&local_res10);
    local_res20 = (WCHAR *)0;
    PECMD_WideToAnsiStr((int64_t *)&local_res20, local_res10, -1, 0xffffffffffffffffULL);

    /* 归一化: 下面把复制出的串当作 ASCII 字节串扫描 (源码用 byte 指针) --- TODO(verify) */
    {
        const unsigned char *bp = (const unsigned char *)local_res20;
        unsigned char b = *bp;
        while (b != 0 && (b < 9 || 0xd < b) && b != 0x20) {
            bp++;
            b = *bp;
        }
        iVar4 = (int)(intptr_t)(bp - (const unsigned char *)local_res20);
    }
    if (iVar4 == 0) {
        iVar4 = -1;
        pWVar9 = (char *)g_timeServer;
    LAB_1400730df:
        if ((unsigned char)((unsigned char)*(unsigned char *)pWVar9 - 0x30) < 10) {
            g_timeServer = (char *)local_50;
            memcpy((void *)local_50, (const void *)pWVar9, (size_t)iVar4);
            ((char *)g_timeServer)[iVar4] = 0;
            pWVar6 = (LPCWSTR)g_timeServer;
        }
        else {
            if ((0 < iVar4) && ((unsigned char *)pWVar9)[iVar4] != 0) {
                ((unsigned char *)pWVar9)[iVar4] = 0;
            }
            lVar5 = (int64_t)(intptr_t)((void *(*)(const char *))(uintptr_t)g_pgethostbyname)(
                (const char *)pWVar9);
            if ((lVar5 == 0) || **(uint64_t **)(lVar5 + 0x18) == 0) {
                goto LAB_140073155;
            }
            pWVar6 = (LPCWSTR)0x1;
            g_timeServer = (char *)**(uint64_t **)(lVar5 + 0x18);
        }
    }
    else {
        pWVar6 = (LPCWSTR)g_timeServer;
        pWVar9 = (char *)local_res20;
        if (iVar4 - 1U < 0x148) {
            goto LAB_1400730df;
        }
    }
    PECMD_NtpSyncLoop((uint32_t *)(uintptr_t)pWVar6);
    uVar8 = 0;
LAB_140073155:
    if ((g_qMode != 0) && (*pWVar7 != L'\0')) {
        PECMD_SetVar(param_1, pWVar7, g_qOutBuf);
    }
    DAT_14013d5c0 = 0;
    g_timeServer = (char *)0;
    PECMD_StrFree((WCHAR **)&local_res20);
    PECMD_StrFree((WCHAR **)&local_50);
    PECMD_StrFree(&local_res18);
    PECMD_StrFree(&local_58);
    PECMD_StrFree((WCHAR **)local_48);
    return uVar8;
}

/* ===================================================================== */
/* @0x140074f14  read PELOGON TIPSDUMMY config (registry / .USRCFG.INI)  */
/* ===================================================================== */
/* signature: undefined8 PECMD_ReadTipDummyConfig(void) */
uint64_t PECMD_ReadTipDummyConfig(void)
{
    WCHAR wVar1;
    WCHAR *lpStr1;
    HANDLE hObject;
    DWORD DVar2;
    int iVar3;
    WCHAR *pwVar4;
    BOOL bVar5;
    DWORD local_res8[2];
    DWORD local_res10[2];
    DWORD local_res18[2];
    WCHAR *local_res20;
    HANDLE local_38;
    WCHAR *local_30;

    if (g_i32D6F4 == 0) {
        PECMD_StrAlloc2(&local_30, 0x1122);
        local_30[0] = L'\0';
        local_30[1] = L'\0';
        local_30[2] = L'\0';
        local_30[3] = L'\0';
        *local_30 = L'\0';
        local_res18[0] = 1;
        local_res8[0] = 0x1ffe;
        local_res20 = local_30;
        DVar2 = PECMD_RegQuery(HKEY_LOCAL_MACHINE, WSTR("SOFTWARE\\PELOGON"),
                               WSTR("TIPSDUMMY"), local_res18, (BYTE *)local_30,
                               local_res8);
        bVar5 = (DVar2 == 0);
        local_res8[0] = (DWORD)(-(uint32_t)bVar5) & local_res8[0];
        local_30[local_res8[0] >> 1] = L'\0';
        pwVar4 = local_30;
        if (*local_30 == L'\0') {
            local_30[0x1000] = L'\0';
            GetModuleFileNameW((HMODULE)0, local_30 + 0x1000, 0x104);
            lstrcatW(local_30 + 0x1000, WSTR(".USRCFG.INI"));
            local_38 = (HANDLE)0;
            PECMD_OpenFileHandle(&local_38, local_30 + 0x1000, 0x80000000u, 7,
                          (LPSECURITY_ATTRIBUTES)0, 3, 0x80, (HANDLE)0);
            hObject = local_38;
            local_res10[0] = 0;
            *local_30 = L'\0';
            ReadFile(local_38, local_30, 0x1ffe, local_res10, (void *)0);
            local_res10[0] = local_res10[0] >> 1;
            if (0xffe < local_res10[0]) {
                local_res10[0] = 0xffe;
            }
            local_30[(int)local_res10[0]] = L'\0';
            if (*local_30 == L'\xfeff') {
                local_30 = local_30 + 1;
                local_res20 = local_30;
            }
            wVar1 = *local_30;
            while (wVar1 != L'\0') {
                PECMD_SkipSpace((WCHAR **)&local_res20);
                lpStr1 = local_res20;
                iVar3 = 10;
                PECMD_SkipUntilDelim(&local_res20, 10, 0xd);
                iVar3 = StrCmpNIW(lpStr1, WSTR("TIPSDUMMY:"), iVar3);
                if (iVar3 == 0) {
                    if (*local_res20 != L'\0') {
                        *local_res20 = L'\0';
                    }
                    pwVar4 = lpStr1 + 10;
                    bVar5 = true;
                    break;
                }
                wVar1 = *local_res20;
            }
            if ((hObject != (HANDLE)0) &&
                (hObject != (HANDLE)(uintptr_t)-1)) {
                CloseHandle(hObject);
            }
        }
        PECMD_SetConfigString(pwVar4, (int)(uint32_t)bVar5);
        PECMD_StrFree(&local_30);
    }
    return (uint64_t)(uintptr_t)g_pConfigStr;
}
