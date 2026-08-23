/* ====================================================================
 * 还原: /tmp/restore_groups/partA.c   (group 1)
 * 仅包含本批 3 个业务函数定义 + 顶部 extern 声明。
 * 未定义任何 helper 体 (由 link_stubs.c / 相应 core 文件提供)。
 * ==================================================================== */

#include <stdlib.h>
#include <string.h>

#include "win32_stub.h"
#include "pecmd_defs.h"

/* ---- 本批补充类型 ---- */
typedef WIN32_FIND_DATAW *LPWIN32_FIND_DATAW;
typedef BYTE *LPBYTE;
typedef OVERLAPPED *LPOVERLAPPED;
typedef LONG LSTATUS;

typedef struct _SYSTEM_INFO {
    union {
        struct { DWORD dwOemId; };
        struct { WORD wProcessorArchitecture; WORD wReserved; };
    };
    DWORD  dwPageSize;
    void  *lpMinimumApplicationAddress;
    void  *lpMaximumApplicationAddress;
    uint64_t dwActiveProcessorMask;
    DWORD  dwNumberOfProcessors;
    DWORD  dwProcessorType;
    DWORD  dwAllocationGranularity;
    WORD   wProcessorLevel;
    WORD   wProcessorRevision;
} _SYSTEM_INFO;

/* ---- Win32 API 补充声明 (win32_stub.h 未含) ---- */
extern void GetSystemInfo(_SYSTEM_INFO *);
extern LONG RegOpenKeyExA(HKEY, LPCSTR, DWORD, DWORD, HKEY *);
extern char *lstrcpynA(char *, const char *, int);

/* ---- FUN_ helper 声明 ---- */
extern void  PECMD_FindFirstFileW(HANDLE *ph, LPCWSTR path, WIN32_FIND_DATAW *fd); /* @0x140101db8 查找入口 */
extern LPCVOID PECMD_MapFileView(HANDLE h, uint64_t size, uint64_t flags, uint64_t offset); /* @0x1400e3f80 映射 */
extern LARGE_INTEGER PECMD_SetFilePointer(HANDLE h, LARGE_INTEGER pos, DWORD method); /* @0x14005c674 定位 */
extern void *PECMD_GrowByteBuffer(void **ps, int64_t len);                 /* @0x140063424 分配槽数组 */
extern int  PECMD_AnsiStrNCompare(char *buf, int64_t a, int64_t b);        /* @0x14005b184 检索匹配 */
extern void PECMD_AppendFormattedI64(int64_t *list, int64_t pos);             /* @0x14006cc70 记录命中位置 */
extern void PECMD_FreeStrBuf(void *ps);                             /* @0x14005b104 释放字符串槽 */
extern void PECMD_TlsLogWrite(uint64_t ctx, LPCWSTR fmt, uint64_t a, uint64_t b); /* @0x140018d8c 日志 */
extern int64_t * PECMD_StrBldCopyAnsi(int64_t *out, const char *src, uint64_t len);   /* @0x1400702f0 取串槽 */
extern uint64_t *PECMD_AssignAnsiString(uint64_t *out, LPCSTR s);           /* @0x14007026c 串构造 */
extern void PECMD_AppendAnsiStr(void **ps, LPCSTR s);                    /* @0x14006345c 缀接字符串 */
extern int  PECMD_PadTrailingSpaces(char *s, int len);                       /* @0x14005cc04 校验/规范化 IP */
extern int  PECMD_CountSeparators(char *s);                                /* @0x14005cbd8 计数 helper */
extern uint64_t PECMD_SetDHCPSettings(HKEY *h, LPCSTR p1, BYTE *p5);      /* @0x140071d00 删除注册项 */

/* ---- DAT_ 全局声明 ---- */
extern uint64_t g_u64D7D8;      /* 页掩码缓存 */
extern uint8_t  g_u8CCB1;      /* MAIN_DBG 日志标志 */
extern int      (*g_pinet_addr)(char *);  /* atoi-ish inet 解析函数指针 */

/* =========================================================================
 * PECMD_GetDirectorySize @0x14006b254 — 递归统计目录占用的字节总和
 * ========================================================================= */
int64_t PECMD_GetDirectorySize(LPCWSTR param_1)
{
    int iVar1;
    int iVar2;
    BOOL BVar3;
    HANDLE local_res18;
    int64_t lVar4;
    int64_t lVar5;
    LPWIN32_FIND_DATAW lpFindFileData;

    lVar5 = 0;
    lpFindFileData = (LPWIN32_FIND_DATAW)malloc(0xa98);
    if (lpFindFileData == NULL)
        return 0;
    iVar1 = lstrlenW(param_1);
    if ((0 < iVar1) &&
        (param_1[iVar1 - 1] == L'\\' || param_1[iVar1 - 1] == L'/')) {
        iVar1 = iVar1 - 1;
    }
    if (iVar1 + 4 < 0x212) {
        if (0 < iVar1) {
            memcpy((void *)(lpFindFileData + 1), param_1, iVar1 * 2);
        }
        memcpy((uint8_t *)lpFindFileData[1].cFileName + (iVar1 * 2 - 0x2c),
                      WSTR("\\*.*"), 10);
        lpFindFileData[2].cFileName[0xd3] = L'\0';
        local_res18 = (HANDLE)0;
        PECMD_FindFirstFileW(&local_res18, (LPCWSTR)(lpFindFileData + 1), lpFindFileData);
        if (local_res18 != (HANDLE)0) {
            do {
                if ((lpFindFileData->dwFileAttributes & 0x10) == 0) {
                    lVar5 = lVar5 +
                        (((int64_t)lpFindFileData->nFileSizeHigh << 32) |
                         lpFindFileData->nFileSizeLow);
                }
                else {
                    iVar2 = lstrcmpiW(lpFindFileData->cFileName, WSTR("."));
                    if (((iVar2 != 0) &&
                         (iVar2 = lstrcmpiW(lpFindFileData->cFileName, WSTR("..")),
                          iVar2 != 0)) &&
                        (iVar2 = lstrlenW(lpFindFileData->cFileName),
                         (int64_t)iVar2 + (int64_t)(iVar1 + 4) < 0x211)) {
                        wsprintfW((LPWSTR)(lpFindFileData[2].cFileName + 0xd4),
                                  WSTR("%s\\%s"), param_1,
                                  lpFindFileData->cFileName);
                        lpFindFileData[4].cFileName[0x95] = L'\0';
                        lVar4 = PECMD_GetDirectorySize(lpFindFileData[2].cFileName + 0xd4);
                        lVar5 = lVar5 + lVar4;
                    }
                }
                BVar3 = FindNextFileW(local_res18, lpFindFileData);
            } while (BVar3 != 0);
            if ((local_res18 != (HANDLE)0) &&
                (local_res18 != (HANDLE)(uintptr_t)-1)) {
                FindClose(local_res18);
            }
            local_res18 = (HANDLE)0;
            free(lpFindFileData);
            return lVar5;
        }
        local_res18 = (HANDLE)0;
    }
    if (lpFindFileData != (LPWIN32_FIND_DATAW)0) {
        free(lpFindFileData);
    }
    return 0;
}

/* =========================================================================
 * PECMD_FindPatternInFile @0x14006cf88 — 大文件映射/检索 (给定长度/方向在文件中搜索模式)
 *   unaff_R13 返回值无意义 -> 全部以 0 返回。
 *   param_12 的 CONCAT71 / _1_7_ 位域噪音简化为: neg = (param_12<0) 方向标志,
 *   uVar23 = 绝对值; 之后 param_12 作为 ReadFile 字节计数复用 (与反编译一致)。
 * ========================================================================= */
int64_t PECMD_FindPatternInFile(int64_t *param_1, HANDLE param_2, uint64_t param_3,
                      int64_t param_4, int param_5, uint64_t param_6,
                      LARGE_INTEGER param_7, int64_t param_8,
                      int param_9, int param_10, LPCVOID param_11,
                      uint64_t param_12)
{
    int iVar1;
    BOOL BVar2;
    int iVar3;
    int iVar4;
    LPCVOID lpBaseAddress;
    LPCVOID lpBaseAddress_00;
    LARGE_INTEGER LVar6;
    LARGE_INTEGER LVar8;
    LARGE_INTEGER LVar15;
    uint64_t uVar7;
    uint64_t uVar9;
    uint64_t uVar11;
    uint64_t uVar23;
    uint64_t uVar27;
    uint64_t uVar28;
    uint64_t uVar21;
    int64_t lVar5;
    int64_t lVar10;
    int64_t lVar13;
    int64_t lVar14;
    int64_t lVar16;
    int64_t lVar22;
    int64_t lVar24;
    int64_t lVar25;
    int64_t lVar26;
    int64_t local_d8;
    int64_t local_b8;
    uint8_t *lpBuffer;
    uint8_t *pcVar12;
    uint8_t *pcVar18;
    uint8_t *puVar19;
    uint8_t *puVar20;
    uint32_t uVar17;
    int neg;
    _SYSTEM_INFO local_78;

    if (g_u64D7D8 == 0) {
        memset(&local_78, 0, 0x30);
        GetSystemInfo(&local_78);
        if (0x1ff < local_78.dwPageSize) {
            g_u64D7D8 = local_78.dwPageSize - 1;
        }
    }
    uVar23 = param_12;
    uVar17 = 0xffff;
    if (g_u64D7D8 != 0) {
        uVar17 = (uint32_t)g_u64D7D8;
    }
    neg = ((int64_t)param_12 < 0);
    if (neg) {
        uVar23 = (uint64_t)(-(int64_t)param_12);
    }
    iVar1 = 0x200;
    if (param_9 != 0) {
        iVar1 = param_9;
    }
    uVar21 = param_3;
    if (param_10 == 0) {
        if (param_8 != 0) {
            uVar21 = (uint64_t)(param_7.QuadPart + param_8);
        }
        if (1 < iVar1) {
            iVar3 = (int)(param_7.QuadPart % (int64_t)iVar1);
            lVar10 = (int64_t)iVar3;
            goto LAB_14006d089;
        }
        goto LAB_14006d0a7;
    }
    else {
        if (param_8 != 0) {
            uVar21 = (uint64_t)(param_7.QuadPart + param_8);
        }
        if (iVar1 < 2) {
            goto LAB_14006d0a7;
        }
        lVar10 = param_7.QuadPart % (int64_t)iVar1;
        iVar3 = param_10;
        goto LAB_14006d089;
    }

LAB_14006d0a7:
    local_d8 = 0;
    LVar15 = param_7;
    goto LAB_14006d_common;

LAB_14006d089:
    lVar10 = (param_7.QuadPart - lVar10) + (int64_t)iVar3;
    local_d8 = lVar10 % (int64_t)iVar1;
    LVar15.QuadPart = lVar10 - (int)local_d8;

LAB_14006d_common:
    lVar10 = (int64_t)param_5;
    if ((int64_t)param_3 < (int64_t)uVar21) {
        uVar21 = param_3;
    }
    lVar5 = (int64_t)(int)local_d8;
    uVar11 = (uint64_t)(((int64_t)uVar17 + (int64_t)uVar21) & (int64_t)(int)~uVar17);
    lVar25 = lVar5 + LVar15.QuadPart;
    uVar27 = (uint64_t)((int64_t)(int)~uVar17 & LVar15.QuadPart);
    if ((int64_t)param_3 < (int64_t)uVar11) {
        uVar11 = param_3;
    }
    lVar24 = (uVar21 - lVar10) - lVar25;

    lpBaseAddress = param_11;
    lpBaseAddress_00 = (LPCVOID)0;
    if (param_11 == (LPCVOID)0) {
        lpBaseAddress = (LPCVOID)PECMD_MapFileView(param_2, uVar11 - uVar27, 2, uVar27);
        lpBaseAddress_00 = lpBaseAddress;
        if (lpBaseAddress == (LPCVOID)0) {
            lVar25 = (int64_t)(param_5 + (int)local_d8);
            LVar6 = PECMD_SetFilePointer(param_2, LVar15, 0);
            if (LVar15.QuadPart != LVar6.QuadPart) {
                return 0;
            }
            lVar14 = uVar21 - LVar15.QuadPart;
            lVar24 = lVar25 * 4;
            if (lVar14 < lVar25 * 4) {
                lVar24 = lVar14;
            }
            uVar11 = (uint64_t)((int64_t)(lVar24 + 0x1ffU) & 0xfffffffffffffe00LL);
            if ((int64_t)uVar11 < 0x400000) {
                uVar11 = 0x400000;
            }
            param_7.QuadPart = 0;
            do {
                lVar13 = (int64_t)(intptr_t)PECMD_GrowByteBuffer((void **)&param_7,
                                                          uVar11 + 0x100800 + lVar25 * 4);
                lpBuffer = (uint8_t *)0;
                if ((lVar13 != 0) &&
                    (lpBuffer = (uint8_t *)((uintptr_t)(lVar13 + 0x20000U) & 0xfffffffffffe0000ULL),
                     lpBuffer != (uint8_t *)0)) break;
                if ((int64_t)uVar11 < lVar24) {
                    goto LAB_14006d4ef;
                }
                uVar11 = (uint64_t)(((int64_t)uVar11 / 2 + 0x1ffU) & 0xfffffffffffffe00LL);
            } while (lpBuffer == (uint8_t *)0);
            if ((int64_t)((lVar25 + 0x1ffU) & 0xfffffffffffffe00LL) <= (int64_t)uVar11) {
                uVar27 = 0;
                SetLastError(0);
                param_11 = (LPCVOID)0x3e8;
                iVar3 = iVar1;
                if (neg) {
                    iVar3 = -iVar1;
                }
                if (!neg) {
                    /* -------- 前向搜索 -------- */
                    while (LVar15.QuadPart <= (int64_t)(uVar21 - lVar25)) {
                        uVar28 = uVar11 - uVar27;
                        uVar7 = ((uVar21 - LVar15.QuadPart) + 0x1ff) & 0xfffffffffffffe00ULL;
                        if ((int64_t)uVar28 < (int64_t)uVar7) {
                            uVar7 = uVar28;
                        }
                        if ((int64_t)uVar7 < 1) break;
                        param_12 = 0;
                        BVar2 = ReadFile(param_2, lpBuffer + uVar27, (DWORD)uVar7,
                                         (DWORD *)&param_12, (LPOVERLAPPED)0);
                        if ((BVar2 == 0) || ((int64_t)param_12 < 1)) {
                            param_11 = (LPCVOID)((int64_t)param_11 - 1);
                            if ((int64_t)param_11 < 1) break;
                            param_12 = 0;
                        }
                        if ((int64_t)uVar28 < (int64_t)param_12) {
                            param_12 = uVar28;
                        }
                        uVar27 = param_12 + uVar27;
                        param_12 = uVar27;
                        if (lVar25 <= (int64_t)uVar27) {
                            lVar14 = (int64_t)iVar1;
                            pcVar12 = lpBuffer + (int)local_d8;
                            lVar24 = ((((lVar14 - lVar10) - (int64_t)(int)local_d8) +
                                       uVar27) / lVar14);
                            lVar14 = lVar14 * lVar24;
                            while ((lVar24 = lVar24 - 1, -1 < lVar24)) {
                                pcVar18 = pcVar12;
                                iVar3 = PECMD_AnsiStrNCompare((char *)pcVar12, param_4, lVar10);
                                if (iVar3 == 0) {
                                    if (param_1 == (int64_t *)0) goto LAB_14006d4dd;
                                    PECMD_AppendFormattedI64(param_1, lVar5 + LVar15.QuadPart);
                                    pcVar18 = pcVar12;
                                    if ((0 < (int64_t)uVar23) &&
                                        (uVar23 = uVar23 - 1, uVar23 == 0)) goto LAB_14006d4c3;
                                }
                                LVar15.QuadPart = LVar15.QuadPart + iVar1;
                                pcVar12 = pcVar18 + iVar1;
                            }
                            lVar24 = (int64_t)(int)local_d8;
                            local_d8 = 0;
                            lVar14 = lVar14 + lVar24;
                            uVar27 = uVar27 - lVar14;
                            if (0 < (int64_t)uVar27) {
                                memmove(lpBuffer, (void *)(lpBuffer + lVar14),
                                              (int)uVar27);
                            }
                        }
                    }
                }
                else {
                    /* -------- 后向搜索 -------- */
                    lVar24 = uVar11 + 0x400 + lVar25;
                    local_d8 = 0;
                    while (LVar15.QuadPart <= (int64_t)(uVar21 - lVar25)) {
                        uVar28 = uVar11 - uVar27;
                        uVar7 = (uint64_t)(lVar14 + -local_d8);
                        if ((int64_t)uVar28 < (int64_t)uVar7) {
                            uVar7 = uVar28;
                        }
                        if ((int64_t)uVar7 < 1) break;
                        LVar6.QuadPart = ((uVar21 - uVar7) - local_d8 + 0x1ff) & 0xfffffffffffffe00ULL;
                        uVar9 = ((-local_d8 - LVar6.QuadPart) + 0x1ff + uVar21) & 0xfffffffffffffe00ULL;
                        if ((int64_t)uVar28 < (int64_t)uVar9) {
                            uVar9 = uVar28;
                        }
                        param_12 = 0;
                        LVar8 = PECMD_SetFilePointer(param_2, LVar6, 0);
                        if (LVar6.QuadPart != LVar8.QuadPart) {
                            PECMD_FreeStrBuf((WCHAR **)&param_7.QuadPart);
                            return 0;
                        }
                        local_b8 = (int64_t)(DWORD)uVar9;
                        BVar2 = ReadFile(param_2, lpBuffer, (DWORD)local_b8,
                                         (DWORD *)&param_12, (LPOVERLAPPED)0);
                        if ((BVar2 == 0) || ((int64_t)param_12 < 1)) {
                            param_11 = (LPCVOID)((int64_t)param_11 - 1);
                            if ((int64_t)param_11 < 1) break;
                            param_12 = 0;
                        }
                        if ((int64_t)uVar7 < (int64_t)param_12) {
                            param_12 = uVar7;
                        }
                        if (0 < (int64_t)uVar27) {
                            memmove(lpBuffer + param_12,
                                          (void *)(lpBuffer + lVar24), (int)uVar27);
                        }
                        local_d8 = local_d8 + param_12;
                        uVar27 = param_12 + uVar27;
                        param_12 = uVar27;
                        if (lVar25 <= (int64_t)uVar27) {
                            lVar16 = (int64_t)iVar1;
                            local_b8 = (((lVar16 - lVar10) - lVar5) + uVar27) / lVar16;
                            lVar13 = lVar16 * local_b8;
                            lVar26 = (lVar13 - lVar16) + LVar6.QuadPart;
                            local_b8 = local_b8 - 1;
                            if (-1 < local_b8) {
                                puVar19 = lpBuffer + ((lVar5 - lVar16) + lVar13) - lVar26;
                                puVar20 = puVar19;
                                lVar16 = lVar5;
                                lVar22 = lVar10;
                                do {
                                    iVar4 = PECMD_AnsiStrNCompare((char *)(puVar20 + lVar26),
                                                          param_4, lVar22);
                                    if (iVar4 == 0) {
                                        if (param_1 == (int64_t *)0) goto LAB_14006d4dd;
                                        PECMD_AppendFormattedI64(param_1, lVar16 + lVar26);
                                        puVar20 = puVar19;
                                        lVar16 = lVar5;
                                        lVar22 = lVar10;
                                        if ((0 < (int64_t)uVar23) &&
                                            (uVar23 = uVar23 - 1, uVar23 == 0)) goto LAB_14006d4c3;
                                    }
                                    lVar26 = lVar26 + iVar3;
                                    local_b8 = local_b8 - 1;
                                } while (-1 < local_b8);
                            }
                            uVar27 = uVar27 - lVar13;
                            LVar15.QuadPart = LVar15.QuadPart + lVar13;
                            if (0 < (int64_t)uVar27) {
                                memmove(lpBuffer + lVar24, (void *)lpBuffer,
                                              (int)uVar27);
                            }
                        }
                    }
                }
            }
        LAB_14006d4ef:
            PECMD_FreeStrBuf((WCHAR **)&param_7.QuadPart);
            goto LAB_14006d500;
        }
    }

    /* -------- 主扫描 (已映射 / 外部缓冲) -------- */
    lVar14 = 0;
    pcVar12 = (uint8_t *)((intptr_t)lpBaseAddress + (lVar25 - (int64_t)uVar27));
    lVar5 = lVar24;
    if (1 < iVar1) {
        lVar5 = (lVar24 / (int64_t)iVar1) * (int64_t)iVar1;
    }
    if (!neg) {
        lVar13 = (int64_t)iVar1;
        lVar5 = lVar13 + lVar5;
    }
    else {
        pcVar12 = pcVar12 + lVar5;
        lVar13 = (int64_t)-iVar1;
        lVar14 = lVar5;
        lVar5 = lVar13;
    }
    lpBaseAddress = lpBaseAddress_00;
    if ((lVar14 <= lVar24) && (lVar24 = lVar10, -1 < lVar14)) {
        for (; lVar14 != lVar5; lVar14 = lVar14 + lVar13) {
            iVar1 = PECMD_AnsiStrNCompare((char *)pcVar12, param_4, lVar24);
            if (iVar1 == 0) {
                if (param_1 == (int64_t *)0) goto LAB_14006d208;
                PECMD_AppendFormattedI64(param_1, lVar14 + lVar25);
                lVar24 = lVar10;
                if ((0 < (int64_t)uVar23) && (uVar23 = uVar23 - 1, uVar23 == 0))
                    goto LAB_14006d1f0;
            }
            pcVar12 = pcVar12 + lVar13;
        }
    }

LAB_14006d500:
    if (lpBaseAddress == (LPCVOID)0) {
        return 0;
    }
    UnmapViewOfFile(lpBaseAddress);
    return 0;

LAB_14006d4c3:
    PECMD_FreeStrBuf((WCHAR **)&param_7.QuadPart);
LAB_14006d1f0:
    if (lpBaseAddress == (LPCVOID)0) {
        return 0;
    }
    UnmapViewOfFile(lpBaseAddress);
    return 0;

LAB_14006d4dd:
    PECMD_FreeStrBuf((WCHAR **)&param_7.QuadPart);
LAB_14006d208:
    if (lpBaseAddress_00 != (LPCVOID)0) {
        UnmapViewOfFile(lpBaseAddress_00);
    }
    return 0;
}

/* =========================================================================
 * PECMD_SetIpConfig @0x140071e90 — 注册表 TCP/IP 配置 读/写 (RegSetIP 相关)
 * ========================================================================= */
uint64_t PECMD_SetIpConfig(LPCSTR param_1, LPCSTR param_2, LPBYTE param_3,
                       LPBYTE param_4, BYTE *param_5, LPBYTE param_6,
                       short *param_7)
{
    int64_t *plVar8;
    int64_t lVar1;
    LSTATUS LVar2;
    int iVar3;
    int iVar4;
    int iVar6;
    int iVar7;
    DWORD DVar5;
    DWORD cbData;
    uint64_t uVar9;
    uint64_t uVar11;
    BYTE *pBVar10;
    BYTE *pBVar12;
    LPBYTE lpData;
    BYTE *lpData_00;
    LPCSTR lpString2;
    char CVar13;
    HKEY local_4d0;
    DWORD local_4d8[2];
    DWORD local_res10[2];
    uint32_t local_4c8[2];
    DWORD local_4c0[2];
    DWORD local_4a0[2];
    LPCSTR local_4b8;
    int local_4b0[2];
    LPBYTE local_4a8;
    int64_t local_498[2];
    uint8_t local_488[256];   /* 网关串 */
    uint8_t local_487[256];   /* NameServer 临时 */
    uint8_t local_3b8[256];   /* SubnetMask 串 */
    uint8_t *local_3b7 = local_3b8 + 1;
    uint8_t local_2d8[256];   /* IPAddress 串 */
    uint8_t *local_2d7 = local_2d8 + 1;
    uint8_t local_1f8[256];   /* 旧 SubnetMask */
    uint8_t *local_1f7 = local_1f8 + 1;
    uint8_t local_118[256];   /* 旧 IPAddress */
    uint8_t *local_117 = local_118 + 1;

    uVar9 = 0;
    CVar13 = '\0';
    local_4a8 = param_4;
    if (g_u8CCB1 != '\0') {
        plVar8 = PECMD_StrBldCopyAnsi((int64_t *)&local_4b8, param_1, 0xffffffffffffffffULL);
        lVar1 = *plVar8;
        PECMD_StrBldCopyAnsi(local_498, (LPCSTR)param_5, 0xffffffffffffffffULL);
        PECMD_StrBldCopyAnsi((int64_t *)local_4c0, (LPCSTR)local_4a8, 0xffffffffffffffffULL);
        PECMD_StrBldCopyAnsi((int64_t *)local_4a0, (LPCSTR)param_3, 0xffffffffffffffffULL);
        plVar8 = PECMD_StrBldCopyAnsi((int64_t *)local_4b0, param_2, 0xffffffffffffffffULL);
        PECMD_TlsLogWrite((uint64_t)(uintptr_t)g_Script,
                      WSTR("\r\nRegSetIP(%s,%s,%s,%s,%s)\r\n"),
                      (uint64_t)lVar1, (uint64_t)*plVar8);
        PECMD_FreeStrBuf((WCHAR **)&local_4b8);
        PECMD_FreeStrBuf((WCHAR **)local_4b0);
        PECMD_FreeStrBuf((WCHAR **)local_4a0);
        PECMD_FreeStrBuf((WCHAR **)local_4c0);
        PECMD_FreeStrBuf((WCHAR **)local_498);
    }
    lpData = local_4a8;
    if ((*param_2 == '*') || (*param_2 == '?')) {
        CVar13 = *param_2;
    }
    PECMD_AssignAnsiString((uint64_t *)&local_4b8,
                  "SYSTEM\\CurrentControlSet\\Services\\Tcpip\\Parameters\\Interfaces\\");
    PECMD_AppendAnsiStr((void **)&local_4b8, param_1);
    LVar2 = RegOpenKeyExA((HKEY)0xffffffff80000002, local_4b8, 0,
                          (~-(uint32_t)(CVar13 != '\0') & 0x20006u) | 1,
                          &local_4d0);
    DVar5 = 0;
    if (LVar2 == 0) {
        if (*param_2 == '-') {
            uVar9 = PECMD_SetDHCPSettings(&local_4d0, param_1, param_5);
            uVar9 = uVar9 & 0xffffffff;
        }
        else if (CVar13 == '\0') {
            /* ---------------- 写配置 ---------------- */
            local_2d8[0] = '\0';
            memset(local_2d7, 0, 0xdb);
            local_3b8[0] = '\0';
            memset(local_3b7, 0, 0xdb);
            local_118[0] = '\0';
            memset(local_117, 0, 0xdb);
            local_1f8[0] = '\0';
            memset(local_1f7, 0, 0xdb);
            local_4d8[0] = 0;
            local_res10[0] = 200;
            RegQueryValueExA(local_4d0, "IPAddress", (DWORD *)0, local_4d8,
                             (BYTE *)&local_118, local_res10);
            iVar3 = PECMD_PadTrailingSpaces((char *)&local_118, local_res10[0]);
            local_4a0[0] = local_res10[0];
            local_4d8[0] = 0;
            local_res10[0] = 200;
            RegQueryValueExA(local_4d0, "SubnetMask", (DWORD *)0, local_4d8,
                             (BYTE *)&local_1f8, local_res10);
            iVar4 = PECMD_PadTrailingSpaces((char *)&local_1f8, local_res10[0]);
            local_488[0] = '\0';
            iVar3 = iVar3 * 0x10000 + iVar4 * 0x10000;
            local_4c0[0] = local_res10[0];
            memset(local_487, 0, 199);
            local_4c8[0] = 0;
            local_res10[0] = 4;
            RegQueryValueExA(local_4d0, "EnableDHCP", (DWORD *)0, local_4d8,
                             (LPBYTE)local_4c8, local_res10);
            if (0 < (int)local_4c8[0]) {
                iVar3 = iVar3 + 0x10000;
            }
            lpString2 = (LPCSTR)&g_DefEnv;
            if (*param_2 != '-') {
                lpString2 = param_2;
            }
            lstrcpynA((char *)&local_2d8, lpString2, 0xc6);
            pBVar12 = (BYTE *)&g_DefEnv;
            if (*param_3 != '-') {
                pBVar12 = param_3;
            }
            lstrcpynA((char *)&local_3b8, (LPCSTR)pBVar12, 0xc6);
            pBVar12 = (BYTE *)&g_DefEnv;
            if (*lpData != '-') {
                pBVar12 = lpData;
            }
            if (*pBVar12 != '0') {
                lstrcpynA((char *)&local_488, (LPCSTR)pBVar12, 0xc6);
            }
            DVar5 = lstrlenA((LPCSTR)&local_2d8);
            cbData = lstrlenA((LPCSTR)&local_3b8);
            iVar4 = lstrlenA((LPCSTR)&local_488);
            (*g_pinet_addr)((char *)param_2);
            ((uint8_t *)local_487)[iVar4] = 0;
            local_3b7[(int)cbData] = 0;
            local_2d7[(int)DVar5] = 0;
            if (DVar5 != 0) {
                DVar5 = DVar5 + 2;
            }
            if (cbData != 0) {
                cbData = cbData + 2;
            }
            if (iVar4 != 0) {
                iVar4 = iVar4 + 1;
            }
            iVar6 = PECMD_CountSeparators((char *)&local_3b8);
            iVar7 = PECMD_CountSeparators((char *)&local_2d8);
            local_4b0[0] = iVar7 + iVar3 + iVar6;
            PECMD_CountSeparators((char *)&local_488);
            local_4c8[0] = (uint32_t)(param_5 == (BYTE *)0);
            lpData_00 = param_5;
            if (param_5 != (BYTE *)0) {
                pBVar10 = (BYTE *)&g_DefEnv;
                if (*param_5 != '-') {
                    pBVar10 = param_5;
                }
                lpData_00 = (BYTE *)&g_DefEnv;
                if (*param_5 != '0') {
                    lpData_00 = pBVar10;
                }
            }
            if (*param_2 != '\0') {
                RegSetValueExA(local_4d0, "IPAddress", 0, 7, (const BYTE *)local_2d8, DVar5);
            }
            if (*param_3 != '\0') {
                RegSetValueExA(local_4d0, "SubnetMask", 0, 7, (const BYTE *)local_3b8, cbData);
            }
            if (*local_4a8 != '\0') {
                RegSetValueExA(local_4d0, "DefaultGateway", 0, 7,
                               (const BYTE *)local_488, iVar4 + 1);
            }
            if (*param_5 != '\0') {
                DVar5 = lstrlenA((LPCSTR)lpData_00);
                RegSetValueExA(local_4d0, "NameServer", 0, 1, lpData_00, DVar5);
            }
            RegSetValueExA(local_4d0, "EnableDHCP", 0, 4, (BYTE *)local_4c8, 4);
            RegCloseKey(local_4d0);
            DVar5 = local_4c0[0];
            if ((*param_3 == '\0') && (local_1f8[0] != '\0')) {
                memcpy((void *)param_3, &local_1f8, local_4c0[0]);
                uVar11 = uVar9;
                if (0 < (int)(DVar5 - 2)) {
                    do {
                        if (param_3[uVar11] == '\0') {
                            param_3[uVar11] = ';';
                        }
                        uVar11 = uVar11 + 1;
                    } while ((int64_t)uVar11 < (int64_t)(int)(DVar5 - 2));
                }
            }
            DVar5 = local_4a0[0];
            if ((*param_2 == '\0') && (local_118[0] != '\0')) {
                memcpy((void *)param_2, &local_118, local_4a0[0]);
                if (0 < (int)(DVar5 - 2)) {
                    do {
                        if (((char *)param_2)[uVar9] == '\0') {
                            ((char *)param_2)[uVar9] = ';';
                        }
                        uVar9 = uVar9 + 1;
                    } while ((int64_t)uVar9 < (int64_t)(int)(DVar5 - 2));
                }
            }
            PECMD_FreeStrBuf((WCHAR **)&local_4b8);
            return (uint64_t)(local_4b0[0] + 1);
        }
        else {
            /* ---------------- 读配置 ---------------- */
            local_4c0[0] = 0;
            local_4d8[0] = 0;
            local_res10[0] = 800;
            LVar2 = RegQueryValueExA(local_4d0, "EnableDHCP", (DWORD *)0, local_4d8,
                                     (LPBYTE)local_4c0, local_res10);
            if (LVar2 != 0) {
                local_4c0[0] = DVar5;
            }
            if ((CVar13 != '?') || (local_4c0[0] == 0)) {
                DVar5 = 4;
            }
            local_res10[0] = 800;
            local_4d8[0] = 0;
            RegQueryValueExA(local_4d0, "DhcpIPAddress" + DVar5, (DWORD *)0, local_4d8,
                             (LPBYTE)(param_2 + 1), local_res10);
            PECMD_PadTrailingSpaces((char *)(param_2 + 1), local_res10[0]);
            local_4d8[0] = 0;
            local_res10[0] = 800;
            RegQueryValueExA(local_4d0, "DhcpSubnetMask" + DVar5, (DWORD *)0, local_4d8,
                             param_3, local_res10);
            PECMD_PadTrailingSpaces((char *)param_3, local_res10[0]);
            local_4d8[0] = 0;
            local_res10[0] = 800;
            RegQueryValueExA(local_4d0, "DhcpDefaultGateway" + DVar5, (DWORD *)0, local_4d8,
                             lpData, local_res10);
            PECMD_PadTrailingSpaces((char *)lpData, local_res10[0]);
            local_4d8[0] = 0;
            local_res10[0] = 800;
            RegQueryValueExA(local_4d0, "DhcpNameServer" + DVar5, (DWORD *)0, local_4d8,
                             param_5, local_res10);
            local_4d8[0] = 0;
            local_res10[0] = 800;
            RegQueryValueExA(local_4d0, "DhcpServer", (DWORD *)0, local_4d8,
                             param_6, local_res10);
            if (param_7 != (short *)0) {
                *param_7 = (short)(char)((local_4c0[0] != 0) + '0');
            }
            uVar9 = 1;
        }
    }
    PECMD_FreeStrBuf((WCHAR **)&local_4b8);
    return uVar9;
}
