/* ==========================================================================
 * core_b3r_g7.c — Restored PECMD business functions (batch28 group 7)
 *
 * Restored from decompiled Ghidra pseudocode in refactored/batch28.txt
 * Functions kept with their Ghidra names FUN_<addr> and `@0x<addr>` header.
 * Uses the project convention: FUN_140102a90 -> memset, WSTR() for wide lits,
 * FUN_<helper> declared extern (no bodies here — only listed targets defined).
 * ========================================================================== */

#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdbool.h>

#include "win32_stub.h"
#include "pecmd_defs.h"

/* ---- Ghidra primitive aliases ---- */
typedef unsigned int        uint;
typedef unsigned short      ushort;
typedef uint8_t             byte;
typedef uint64_t            undefined8;
typedef uint32_t            undefined4;
typedef uint16_t            undefined2;
typedef uint8_t             undefined1;
typedef int64_t             longlong;
typedef uint64_t            ulonglong;

/* ---- Ghidra CONCAT62 splice normalization ----
 * CONCAT62(hi6, lo2) packs a 6-byte value into bits 16..63 and a 2-byte value
 * into bits 0..15.  Every CONCAT62 hull we meet here only replaces the low
 * WCHAR (drive letter) of an 8-byte path-prefix global, so the macro reduces
 * to that simple bit splice. */
#ifndef CONCAT62
#define CONCAT62(hi6, lo2) \
    (((((uint64_t)(hi6)) & 0xFFFFFFFFFFFFULL) << 16) | ((uint64_t)(lo2) & 0xFFFF))
#endif

/* ---- globals (8-byte data) referenced by PECMD_DriveLetterSet ----
 * These four .rdata/.data terms are UTF-16LE path-prefix fragments that are
 * assembled (with a drive letter spliced in via CONCAT62) into drive-root
 * strings such as "C:\\..." for GetDriveTypeW / FUN_140065efc.  Content is
 * provided elsewhere; here they are read as raw 8-byte/4-byte data. */
extern uint64_t g_u6426e70;
extern uint64_t g_u646e78;
extern uint32_t g_u32126e80;
extern uint64_t g_u64ee90;
extern uint32_t g_u3211ee98;


/* ---- Helper function externs (bodies are NOT defined here) ---- */
extern ulonglong FUN_1400688e0(LPCWSTR param_1);              /* @0x1400688e0 */
extern LPVOID   FUN_1400179f8(LPCWSTR, LPVOID, DWORD *);      /* @0x1400179f8 读文件 */
extern void    *FUN_14007026c(void *out, LPCSTR s);           /* @0x14007026c 串构造 */
extern undefined8 *PECMD_AllocSmallObject(undefined8 *arr);            /* @0x140063344 小对象分配 */
extern undefined8 PECMD_EncodeBuffer(longlong *in, longlong *out,
                                undefined1 cp);               /* @0x140068984 */
extern HANDLE   PECMD_LoadImageFromMemory(undefined8 *param_1, longlong *param_2); /* @0x14006eaac */
extern void     FUN_14005b104(void *ps);                      /* @0x14005b104 释放字符串槽 */
extern WCHAR   *FUN_14005b154(WCHAR **ps);                    /* @0x14005b154 跳过空白 */
extern WCHAR   *FUN_14006375c(WCHAR **ps, LPCWSTR src);       /* @0x14006375c 串追加 */
extern void     FUN_140063620(WCHAR **out);                   /* @0x140063620 初始化串缓冲 */
extern WCHAR   *PECMD_AllocString(WCHAR **ps, int64_t count);     /* @0x140063720 串分配(计数) */
extern WCHAR   *FUN_1400637dc(WCHAR **ps, LPCWSTR src,
                              int64_t a, int64_t b);          /* @0x1400637dc 串复制分配 */
extern void     FUN_140063694(WCHAR **ps, int64_t count);     /* @0x140063694 串分配(count) */
extern void     FUN_140017b8c(void);                          /* @0x140017b8c 初始化 */
extern bool     FUN_1400c11c0(WCHAR **pp, int *out);          /* @0x1400c11c0 */
extern bool     FUN_1400c1194(WCHAR **pp, uint64_t *size);    /* @0x1400c1194 */
extern uint32_t FUN_140065efc(LPCWSTR p, HANDLE h);           /* @0x140065efc 取文件系统类型 */
extern BOOL     FUN_14006f908(char param_1, int param_2);     /* @0x14006f908 */
extern void     FUN_14007c7ec(WCHAR param_1, int param_2);    /* @0x14007c7ec */
extern short   *FUN_1400547bc(int64_t *ctx, WCHAR **pp, WCHAR **out,
                              short c, short f);              /* @0x1400547bc 拆串 */
extern DWORD    FUN_14005c5a0(HKEY root, LPCWSTR sub, LPCWSTR name, DWORD type,
                              BYTE *data, DWORD size);        /* @0x14005c5a0 注册表查询 */
extern int      FUN_14005c788(const char *a, const WCHAR *w, int n); /* @0x14005c788 串前缀比较 */
extern int64_t *FUN_1400702f0(int64_t *out, LPCSTR s, uint64_t len); /* @0x1400702f0 取串槽 */
extern uint64_t FUN_14000e26c(uint64_t script, uint64_t cmd, uint64_t s3,
                              uint64_t s4, uint32_t flag, void *p6,
                              uint64_t s7, void *p8);         /* @0x14000e26c 脚本执行 */

/* ================================================================
 * @0x14007c730
 * 打开文件并返回句柄；失败返回 (HANDLE)0。把“是否可读”写入 *(bool*)param_2。
 */
HANDLE FUN_14007c730(LPCWSTR param_1, uint64_t param_2)
{
    ulonglong uVar1;
    HANDLE pvVar2;
    bool bVar3;
    DWORD local_res10[6];
    int64_t local_38[3];
    LPVOID local_20;
    ulonglong local_18;

    uVar1 = FUN_1400688e0(param_1);
    bVar3 = (char)uVar1 != '\0';
    *(bool *)param_2 = bVar3;
    if (bVar3) {
        local_res10[0] = 0;
        local_20 = FUN_1400179f8(param_1, (LPVOID)0, local_res10);
        if (local_20 != (LPVOID)0) {
            FUN_14007026c(&local_20, (LPCSTR)0);
            PECMD_AllocSmallObject((undefined8 *)local_38);
            local_18 = (ulonglong)local_res10[0];
            local_38[1] = 0;
            local_38[2] = 0;
            PECMD_EncodeBuffer((longlong *)&local_20, local_38, 0);
            pvVar2 = PECMD_LoadImageFromMemory((undefined8 *)local_38, (longlong *)0);
            FUN_14005b104(local_38);
            FUN_14005b104((void *)&local_20);
            return pvVar2;
        }
    }
    return (HANDLE)0;
}

/* ================================================================
 * @0x14007c88c
 * 驱动器/CD 字母设置：解析 C- / U- 开关与显式盘符，枚举逻辑盘并调用
 * FUN_14006f908 / FUN_14007c7ec。盘符路径前缀用 CONCAT62 拼接（已规范化）。
 */
uint64_t PECMD_DriveLetterSet(uint64_t param_1, LPCWSTR param_2)
{
    char cVar1;
    DWORD DVar2;
    UINT UVar3;
    int iVar4;
    int iVar5;
    char cVar6;
    char cVar7;
    uint uVar8;
    WCHAR WVar9;
    char *pcVar10;
    WCHAR WVar11;
    WCHAR WVar12;
    WCHAR WVar13;
    LPCWSTR local_res10;
    int local_res18[2];
    int local_res20[2];
    LPWSTR local_b8;
    char local_b0[32];
    uint64_t local_90;      /* WCHAR local_90 + uStack_8e (8 字节路径前缀) */
    uint64_t local_88;
    uint64_t local_80;
    uint32_t local_78;
    uint64_t local_70;
    uint64_t local_68;
    uint32_t local_60;
    uint64_t local_58;
    uint64_t local_50;      /* short local_50 + uStack_4e (8 字节路径前缀) */
    uint32_t local_48;

    local_res10 = param_2;
    FUN_140017b8c();
    FUN_14005b154((WCHAR **)&local_res10);
    local_b8 = StrChrW(local_res10, L',');
    WVar9 = L'\0';
    cVar7 = '\0';
    if (local_b8 != (LPWSTR)0) {
        *local_b8 = L'\0';
        local_b8 = local_b8 + 1;
        FUN_14005b154((WCHAR **)&local_b8);
        if ((*local_b8 & 0xffdfU) == 0x52) {
            cVar7 = '\x02';
        } else {
            local_res20[0] = 0;
            FUN_1400c11c0(&local_b8, local_res20);
            cVar7 = '\0';
            if (local_res20[0] != 0) {
                cVar7 = '\x01';
            }
        }
    }
    WVar12 = L'\0';
    local_res18[0] = -1;
    if ((((*local_res10 != L'C') ||
          (WVar11 = L'\x01', WVar13 = WVar12, local_res10[1] != L'-')) &&
         ((WVar11 = WVar9, *local_res10 != L'U' ||
           (WVar13 = L'\x01', local_res10[1] != L'-')))) &&
        (WVar13 = WVar12, *local_res10 != L'\0')) {
        if (local_res10[1] == L':') {
            WVar9 = *local_res10;
        }
        if ((0x2f < (ushort)*local_res10) && ((ushort)*local_res10 < 0x3a)) {
            FUN_1400c11c0((WCHAR **)&local_res10, local_res18);
        }
    }
    local_70 = g_u6426e70;
    local_68 = g_u646e78;
    local_60 = g_u32126e80;
    local_80 = g_u64ee90;
    local_78 = g_u3211ee98;
    if (local_res18[0] < 0) {
        if (WVar9 != L'\0') {
            /* TODO(verify): CONCAT62 仅替换低位 WCHAR(盘符) */
            local_80 = (g_u64ee90 & 0xFFFFFFFFFFFF0000ULL) | (WVar9 & 0xFFFF);
            local_68 = (g_u646e78 & 0xFFFFFFFFFFFF0000ULL)
                       | ((uint64_t)(uint8_t)WVar9 & 0xFFFF);
            UVar3 = GetDriveTypeW((LPCWSTR)&local_80);
            iVar5 = (int)FUN_140065efc((LPCWSTR)&local_70, (HANDLE)0);
            iVar4 = local_res18[0];
            if (UVar3 == 5) {
                if ((cVar7 != '\x02') || (iVar5 != 7)) {
                    FUN_14006f908((char)WVar9, (uint)(cVar7 != '\x01'));
                    return 0;
                }
                iVar4 = -1;
            }
            FUN_14007c7ec(WVar9, iVar4);
            return 0;
        }
    } else {
        FUN_14007c7ec(WVar9, local_res18[0]);
    }
    if (WVar11 == L'\0') {
        if (WVar13 == L'\0') {
            return 0;
        }
        cVar7 = '\0';
    }
    DVar2 = GetLogicalDrives();
    memset(local_b0, 0, sizeof(local_b0));
    uVar8 = 0;
    pcVar10 = local_b0;
    do {
        cVar6 = (char)uVar8;
        if ((DVar2 >> (uVar8 & 0x1f) & 1) != 0) {
            local_58 = g_u6426e70;
            local_48 = g_u32126e80;
            local_88 = g_u3211ee98;
            /* TODO(verify): CONCAT62 仅替换低位 WCHAR(盘符) */
            local_50 = (g_u646e78 & 0xFFFFFFFFFFFF0000ULL)
                       | (uint64_t)(uint8_t)(cVar6 + 0x41);
            local_90 = (g_u64ee90 & 0xFFFFFFFFFFFF0000ULL)
                       | (uint64_t)(uint8_t)(cVar6 + 0x41);
            UVar3 = GetDriveTypeW((LPCWSTR)&local_90);
            if (('\x01' < cVar6) || (UVar3 != 2)) {
                if (WVar13 == L'\0') {
                LAB_14007cacb:
                    if (UVar3 != 5) goto LAB_14007cad0;
                } else {
                    if (UVar3 == 5) {
                        if (cVar7 != '\x02') goto LAB_14007cb34;
                        goto LAB_14007cacb;
                    }
                LAB_14007cad0:
                    if ((UVar3 != 2) && (UVar3 != 3)) goto LAB_14007cb34;
                }
                if ((WVar11 == L'\0') || (UVar3 == 5)) {
                    iVar4 = (int)FUN_140065efc((LPCWSTR)&local_58, (HANDLE)0);
                    if (WVar13 != L'\0') {
                        if ((iVar4 != 7) || ((UVar3 == 5 && (cVar7 != '\x02'))))
                            goto LAB_14007cb34;
                        *pcVar10 = 'E';
                    }
                    if ((WVar11 != L'\0') && (UVar3 == 5)) {
                        if ((cVar7 != '\x02') || (cVar1 = 'E', iVar4 != 7)) {
                            cVar1 = 'C';
                        }
                        *pcVar10 = cVar1;
                    }
                }
            }
        }
    LAB_14007cb34:
        uVar8 = (uint)(byte)((uint8_t)cVar6 + 1U);
        pcVar10 = pcVar10 + 1;
        if ('\x19' < (char)(cVar6 + 1U)) {
            cVar6 = '\0';
            pcVar10 = local_b0;
            do {
                if (*pcVar10 == 'C') {
                    /* TODO(verify): CONCAT62 仅替换低位 WCHAR(盘符) */
                    local_68 = (local_68 & 0xFFFFFFFFFFFF0000ULL)
                               | (uint8_t)(cVar6 + 0x41);
                    FUN_14006f908((char)(cVar6 + 0x41), (uint)(cVar7 != '\x01'));
                } else if (*pcVar10 == 'E') {
                    FUN_14007c7ec((WCHAR)((uint16_t)(uint8_t)cVar6 + L'A'), -1);
                }
                cVar6 = (char)(cVar6 + '\x01');
                pcVar10 = pcVar10 + 1;
            } while (cVar6 < '\x1a');
            return 0;
        }
    } while (true);
}

/* ================================================================
 * @0x14007cc1c
 * 回收站(Recycle Bin)卷查询：对指定卷或全部逻辑盘查询 BitBucket 卷注册表
 * MaxCapacity/NukeOnDelete。返回 0 成功，负值失败。
 */
int64_t PECMD_QueryRecycleBinVolume(int64_t *param_1, short *param_2)
{
    LPWSTR lpBuffer;
    WCHAR WVar1;
    bool bVar3;
    int iVar4;
    UINT UVar5;
    BOOL BVar6;
    uint uVar7;
    DWORD DVar8;
    DWORD DVar9;
    int iVar10;
    undefined8 uVar11;
    int64_t lVar12;
    LPWSTR lpRootPathName;
    LPWSTR lpString;
    short *local_res10;
    int local_res18[2];
    uint local_res20[2];
    LPWSTR local_58;
    LPWSTR local_50;
    ulonglong local_48;
    LPWSTR local_40;
    LPWSTR pWVar2;

    uVar7 = 0;
    local_res10 = param_2;
    FUN_14005b154((WCHAR **)&local_res10);
    FUN_140063620((WCHAR **)&local_58);
    local_48 = 0x8000000000000000ULL;
    FUN_1400547bc(param_1, (WCHAR **)&local_res10, (WCHAR **)&local_58, 0x2c, 0);
    if ((*local_res10 == 0) ||
        (bVar3 = FUN_1400c1194((WCHAR **)&local_res10, &local_48), !bVar3)) {
        FUN_14005b104((void *)&local_58);
        lVar12 = -0x7ff8ffa9;
    } else {
        bVar3 = *local_58 != L'*';
        iVar4 = lstrlenW(local_58);
        PECMD_AllocString((WCHAR **)&local_58, (int64_t)iVar4 + 2);
        local_50 = (LPWSTR)0;
        FUN_140063694(&local_40, 0x433);
        lpBuffer = local_40 + 0x107;
        memset(local_40, 0, 0x42f * 2);
        lpRootPathName = local_58;
        if (!bVar3) {
            GetLogicalDriveStringsW(0x324, lpBuffer);
            lpRootPathName = lpBuffer;
        }
        if ((*local_58 != L'\0') && (local_58[1] == L':')) {
            local_58[2] = L'\\';
            local_58[3] = L'\0';
        }
        FUN_1400637dc((WCHAR **)&local_50,
                      WSTR("Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\BitBucket\\Volume\\"),
                      0xffffffffffffffffLL, 0xffffffffffffffffLL);
        iVar4 = lstrlenW(local_50);
        PECMD_AllocString((WCHAR **)&local_50, (int64_t)(iVar4 + 0x20a));
        pWVar2 = local_50;
        local_res18[0] = (int)local_48;
        local_res20[0] = (uint)(local_res18[0] == 0);
        do {
            if (bVar3) {
            LAB_14007cdc6:
                BVar6 = GetVolumeNameForVolumeMountPointW(lpRootPathName, local_40, 0x208);
                if (BVar6 == 0) {
                    uVar7 = GetLastError();
                    if (uVar7 == 0) {
                        uVar7 = 1;
                    }
                } else {
                    uVar11 = FUN_14005c788("\\\\?\\Volume", (ushort *)local_40, 10);
                    lpString = local_40;
                    if ((char)uVar11 != '\0') {
                        lpString = local_40 + 10;
                    }
                    iVar10 = lstrlenW(lpString);
                    if (lpString[(int64_t)iVar10 + -1] == L'\\') {
                        lpString[(int64_t)iVar10 + -1] = L'\0';
                    }
                    lstrcpynW(pWVar2 + iVar4, lpString, 0x208);
                    DVar8 = FUN_14005c5a0((HKEY)0xffffffff80000001, pWVar2,
                                          WSTR("MaxCapacity"), 4, (BYTE *)local_res18, 4);
                    DVar9 = FUN_14005c5a0((HKEY)0xffffffff80000001, pWVar2,
                                          WSTR("NukeOnDelete"), 4, (BYTE *)local_res20, 4);
                    uVar7 = -(uint)(DVar9 != 0) & -(uint)(DVar8 != 0) & uVar7;
                }
            } else {
                WVar1 = lpRootPathName[2];
                lpRootPathName[2] = L'\0';
                UVar5 = GetDriveTypeW(lpRootPathName);
                lpRootPathName[2] = WVar1;
                if (UVar5 == 3) goto LAB_14007cdc6;
            }
            if (bVar3) break;
            iVar10 = lstrlenW(lpRootPathName);
            lpRootPathName = lpRootPathName + (int64_t)iVar10 + 1;
        } while (*lpRootPathName != L'\0');
        FUN_14005b104((void *)&local_40);
        FUN_14005b104((void *)&local_50);
        FUN_14005b104((void *)&local_58);
        lVar12 = (int64_t)(int)uVar7;
    }
    return lVar12;
}

/* ================================================================
 * @0x14007d1c4
 * 解析以 "$" 为前缀的 EXEC 描述串并交给脚本执行器运行。
 */
uint64_t FUN_14007d1c4(LPCWSTR param_1)
{
    uint64_t _Var1;
    LPCWSTR pWVar2;
    short sVar3;
    LPCWSTR local_res8;
    uint64_t local_res10;   /* Ghidra 的 _FILETIME 8 字节串槽 */

    local_res8 = param_1;
    FUN_14005b154((WCHAR **)&local_res8);
    pWVar2 = local_res8;
    sVar3 = 0;
    if (*local_res8 == L'$') {
        pWVar2 = local_res8 + 1;
        sVar3 = 5;
    }
    FUN_1400702f0((int64_t *)&local_res10, "--exe:#2|INDATA !=pecmd ", 0xffffffffffffffffLL);
    if (sVar3 == 5) {
        *(uint16_t *)((char *)&local_res10 + 0x20) = 0x20;
    }
    FUN_14006375c((WCHAR **)&local_res10, pWVar2);
    local_res8 = (LPCWSTR)((uint64_t)(uintptr_t)local_res8 & 0xffffffffffff0000ULL);
    /* TODO(verify): 4/5/6/7/8 参为 Ghidra 成对残留；按原样传入 */
    _Var1 = FUN_14000e26c((uint64_t)(uintptr_t)g_Script,
                          local_res10,
                          (uint64_t)(uintptr_t)g_Script,
                          (uint64_t)(uintptr_t)&local_res8, 0, (void *)0,
                          0, (void *)0);
    FUN_14005b104((void *)&local_res10);
    return _Var1 & 0xffffffff;
}
