/* ====================================================================
 * core_b8_failed.c — B8 decompile-failed 函数还原
 *
 * 这两个函数在 decompiled.c 中只有签名 + [DECOMPILE FAILED]。本文件从
 * 原始二进制 x64 反汇编逐指令重建 C 函数体。
 *
 * 来源: PECMD原始.EXE (x64, ImageBase=0x140000000)
 *   PECMD_CryptoHashCompute @0x1400e4864   DWORD (BYTE*, DWORD, ALG_ID, uint64)
 *   PECMD_HandleControlMessage @0x1400fdad8   void  (void*, int, int64_t, uint64)
 *
 * 反汇编窗口见 ghidra_out/asm/1400e4864.asm 与 1400fdad8.asm；
 * 完整反汇编由 objdump -d -M intel 于原始 PECMD.exe 上自取。
 * ==================================================================== */
#include <stdint.h>
#include <string.h>

#include "pecmd_defs.h"

typedef uint32_t ALG_ID;
typedef uintptr_t HCRYPTPROV;
typedef uintptr_t HCRYPTHASH;
typedef uintptr_t HCRYPTKEY;

/* ---- CryptoAPI (win32_stub 未提供，link_stubs.c 提供桩) ---- */
extern BOOL CryptAcquireContextW(HCRYPTPROV *phProv, LPCWSTR szContainer,
                                 LPCWSTR szProvider, uint32_t dwProvType,
                                 uint32_t dwFlags);
extern BOOL CryptReleaseContext(HCRYPTPROV hProv, uint32_t dwFlags);
extern BOOL CryptCreateHash(HCRYPTPROV hProv, ALG_ID Algid, HCRYPTKEY hKey,
                            uint32_t dwFlags, HCRYPTHASH *phHash);
extern BOOL CryptHashData(HCRYPTHASH hHash, const BYTE *pbData, DWORD dwDataLen,
                          DWORD dwFlags);
extern BOOL CryptGetHashParam(HCRYPTHASH hHash, DWORD dwParam, BYTE *pbData,
                              DWORD *pdwDataLen, DWORD dwFlags);
extern BOOL CryptDestroyHash(HCRYPTHASH hHash);
extern int wsprintfA(LPSTR buf, LPCSTR fmt, ...);

/* ---- CryptoAPI 未在 link_stubs 提供的两个：本文件内提供最小桩（同项目 API 惯例）。
 *       真实语义已由反汇编注释保留。 ---- */
BOOL CryptEnumProviderTypesW(DWORD dwIndex, DWORD *pdwReserved,
                             DWORD dwFlags, DWORD *pdwProvType,
                             DWORD cbProvType, WCHAR *pszProvTypeName)
{
    (void)dwIndex; (void)pdwReserved; (void)dwFlags;
    (void)pdwProvType; (void)cbProvType; (void)pszProvTypeName;
    return FALSE;
}
BOOL CryptGetProvParam(HCRYPTPROV hProv, DWORD dwParam, BYTE *pbData,
                       DWORD *pdwDataLen, DWORD dwFlags)
{
    (void)hProv; (void)dwParam; (void)pbData; (void)pdwDataLen; (void)dwFlags;
    return FALSE;
}

/* ---- 内部函数 (其它 core_*.c 已实现/桩) ---- */
extern int64_t *FUN_1400634D4(int64_t *ps, LPCSTR src, int64_t len);   /* @0x1400634d4 ANSI 串写入容器 */
extern void     FUN_1400633A8(void **ps, int64_t len);                 /* @0x1400633a8 分配 */
extern int64_t *FUN_14006345C(int64_t *ps, LPCSTR src);                /* @0x14006345c ANSI 串追加 */
extern void PECMD_FreeStrBuf(void *ps);                             /* @0x14005b104 释放 */

/* @0x1400e4864 size=980 — 作用(asm→C重建)：CryptAPI 哈希/算法信息辅助。
 * 按 alg 分两路：
 *  - alg==0xffffcfc7：枚举本机 Cryptographic Provider 类型（CryptEnumProviderTypesW），
 *    再对默认哈希容器枚举全部算法（CryptGetProvParam[PP_ENUMALGS]），把形如
 *        "%8.8xh  %4d  %4s  %4d  %s\r\n"   (签名/加密/哈希/交换/未知 + 算法名)
 *    的行追加到 extra 对齐缓冲；失败返回 1，成功返回 0。
 *  - 其它 alg：哈希摘要工具。CryptAcquireContextW(CALG 默认) → CryptCreateHash(alg)
 *    → CryptHashData(data,len) → 取 HP_HASHVAL 逐字节 "%02X" 拼十六进制串写到
 *    extra 对齐缓冲；(extra&2) 时立即返回 0。句柄槽在本地或 extra 对齐前
 *    0x18/0x10 处调用方槽。
 */
DWORD PECMD_CryptoHashCompute(BYTE *data, DWORD len, ALG_ID alg, uint64_t extra)
{
    HCRYPTPROV hProv = 0;          /* [rsp+0x68] 本地句柄槽 */
    HCRYPTHASH hHash = 0;          /* [rsp+0x70] 本地句柄槽 */
    HCRYPTPROV *provSlot = &hProv;
    HCRYPTHASH *hashSlot = &hHash;
    char *out;                     /* r12 = extra & ~7（输出 ANSI/hex 缓冲） */
    char *pC = NULL;               /* [rsp+0x50] 0x1c00 大缓冲 */
    char *pD = NULL;               /* [rsp+0x58] pC+0x1800 */
    BYTE *pE = NULL;               /* [rsp+0x60] 哈希值缓冲 */
    DWORD dwA = 0;                 /* [rsp+0x40] */
    DWORD dwB = 0x400;             /* [rsp+0x44] */
    DWORD dwC = 0;                 /* [rsp+0x48] */
    DWORD subLen = 0;              /* [rsp+0xd8] */
    int32_t r13;                   /* (int8)(extra&7) / 哈希类型值 */
    int32_t ebp = 0;
    int32_t edi = 1;
    int32_t esi = 1;
    BYTE *r15 = data;
    const char *label;
    DWORD count;

    out = (char *)(extra & ~(uint64_t)7u);
    r13 = (int32_t)(int8_t)((uint8_t)extra & 7u);

    if (r13 != 0) {
        provSlot = (HCRYPTPROV *)(out - 0x18);
        hashSlot = (HCRYPTHASH *)(out - 0x10);
        if (*(uint64_t *)hashSlot != 0) {
            goto LAB_b0e;          /* 调用方已备好 prov/hash 句柄 */
        }
    }

    if ((DWORD)alg == 0xffffcfc7u) {
        /* ===== 路径 A：枚举算法 / 哈希类型打印 ===== */
        FUN_1400634D4((int64_t *)out, "\x20" /* @0x14012b12c ""空格"" */, -1);
        FUN_1400633A8((void **)&pC, 0x1c00);

        pD = pC + 0x1800;
        r15 = (BYTE *)(pC + 0x1400);       /* r15 = pC+0x1400 */
        ebp = (int32_t)(uintptr_t)(pC + 0x1000); /* rbp */
        dwB = 0x400;

        {
            DWORD dwIndex = 0;
            for (;;) {
                if (CryptEnumProviderTypesW(dwIndex, NULL, 0, &dwA, 0,
                                            (WCHAR *)&subLen) == FALSE) {
                    break;
                }
                CryptEnumProviderTypesW(dwIndex, NULL, 0, &dwA,
                                        (DWORD)(uintptr_t)ebp,
                                        (WCHAR *)&subLen);
                dwIndex++;
            }
        }

        if (CryptAcquireContextW((HCRYPTPROV *)&pE, NULL, NULL, 1, 0) == FALSE) {
            goto LAB_aa6;
        }

        do {
            dwC = 0;
            if (CryptGetProvParam(*(HCRYPTPROV *)&pE, 1 /* PP_ENUMALGS */,
                                  r15, &dwB, (DWORD)r13) == FALSE) {
                esi = 0;
            } else {
                DWORD v0 = *(uint32_t *)(r15 + 0x0);   /* r13 哈希类型值 */
                DWORD v1 = *(uint32_t *)(r15 + 0x4);   /* r14 */
                DWORD v2 = *(uint32_t *)(r15 + 0x8);   /* ebp */
                count = (v2 < 0x63) ? v2 : 0x63;
                memcpy(pD, r15 + 0xc, count);   /* @0x1401077e0 (strncpy 内联, SKIP) */
                pD[count] = 0;
                r13 = (int32_t)v0;

                switch (v0 & 0xe000) {
                    case 0x2000: label = "\xC7\xA9\xC3\xFB"; break; /* 签名 @0x14012b104 */
                    case 0x6000: label = "\xBC\xD3\xC3\xDC"; break; /* 加密 @0x14012b10c */
                    case 0x8000: label = "\xB9\xFE\xCF\xA3"; break; /* 哈希 @0x14012b114 */
                    case 0xa000: label = "\xBD\xBB\xBB\xBB"; break; /* 交换 @0x14012b11c */
                    default:     label = "\xCE\xB4\xD6\xAA"; break; /* 未知 @0x14012b124 */
                }

                wsprintfA(pC, "%8.8xh  %4d  %4s  %4d  %s\r\n",
                          (int32_t)v0, (int32_t)v1, label, (int32_t)v2,
                          pD);                     /* fmt @0x14012b0e8 */
                FUN_14006345C((int64_t *)out, pC); /* 追加该行 */

                r13 = (int32_t)dwC;
                dwB = 0x400;
            }
        } while (esi != 0);

        if (*(HCRYPTPROV *)&pE != (HCRYPTPROV)0) {
            CryptReleaseContext(*(HCRYPTPROV *)&pE, 0);
            edi = 0;
        }
    LAB_aa6:
        PECMD_FreeStrBuf((WCHAR **)&pC);
        return (DWORD)edi;
    }

    /* ===== 路径 B：哈希摘要 → 十六进制串 ===== */
    if (CryptAcquireContextW(provSlot, NULL,
                             WSTR("Microsoft Base Cryptographic Provider v1.0") /* @0x14012b090 */,
                             1, 0xf0000000) == FALSE) {
        return GetLastError();
    }
    if (CryptCreateHash(*provSlot, alg, 0, 0, hashSlot) == FALSE) {
        ebp = (int32_t)GetLastError();
        goto LAB_bf7;
    }

LAB_b0e:
    if (CryptHashData(*hashSlot, data, len, 0) == FALSE) {
        ebp = (int32_t)GetLastError();
        goto LAB_bf7;
    }
    if ((extra & 2u) != 0) {
        return 0;
    }

    dwA = 0;
    subLen = 4;
    CryptGetHashParam(*hashSlot, 4 /* HP_HASHSIZE */, (BYTE *)&dwA, &subLen, 0);
    FUN_1400633A8((void **)&pE, (int64_t)dwA);
    subLen = dwA;
    CryptGetHashParam(*hashSlot, 2 /* HP_HASHVAL */, pE, &subLen, 0);

    {
        char *p = out;
        BYTE *q = pE;
        DWORD i = 0;
        p[0] = 0;
        while (i < subLen) {
            wsprintfA(p, "%02X", (unsigned int)q[0]);   /* fmt @0x140127458 */
            i++;
            p += 2;
            q += 1;
        }
    }
    PECMD_FreeStrBuf((WCHAR **)&pE);

LAB_bf7:
    if (*hashSlot != (HCRYPTHASH)0) {
        CryptDestroyHash(*hashSlot);
        *hashSlot = (HCRYPTHASH)0;
    }
    if (*provSlot != (HCRYPTPROV)0) {
        CryptReleaseContext(*provSlot, 0);
        *provSlot = (HCRYPTPROV)0;
    }
    return (DWORD)ebp;
}

/* @0x1400fdad8 size=1025 — 作用(asm→C重建)：带悬浮菜单/无线状态提示的
 * WndProc 风格消息处理器。param_1=obj, param_2=msg, param_3=wParam,
 * param_4=lParam。处理 0x44f(光标提示 GetCursorPos/PtInRect/InvalidateRect/
 * KillTimer/SetTimer/PostMessageW)、0x44e(虚表+0xe0 方法转发)、0x462(校验/
 * 状态位更新)、0x45a(SetControlText)、默认(字段追加 FUN_1400F172C)。
 * 返回值为 rax（调用方按 void）。
 */
extern uint16_t FUN_1400F172C(int64_t *map, int msg, uint64_t wParam,
                              uint64_t *lParam, int64_t hwnd, uint32_t mode,
                              int64_t *out);                 /* @0x1400f172c */
extern void PECMD_ParseNumTryWriteback(void **pp, int *out);              /* @0x140067d20 (link_stubs) */
extern void PECMD_InvalidateParentRect(HWND child, int margin);           /* @0x1400fd538 */
extern void PECMD_RestoreForegroundWindow(void);             /* @0x14005e204 */
extern void PECMD_SetControlText(HWND hwnd, int64_t p2, LPCWSTR p3, int p4); /* @0x1400a895c */
extern HWND    FUN_1400E5788(HWND hwnd);                     /* @0x1400e5788 取顶层父窗口 */
extern int64_t FUN_1400E5B0C(int64_t a1, uint64_t a2, int64_t a3,
                             int64_t *a4);                   /* @0x1400e5b0c */

void PECMD_HandleControlMessage(void *param_1, int param_2, int64_t param_3, uint64_t param_4)
{
    uint8_t *obj = (uint8_t *)param_1;
    HWND hwnd = *(HWND *)(obj + 0x20);
    int32_t msg = param_2;
    int64_t wParam = param_3;      /* rbp */
    uint64_t lParam = param_4;     /* r14 */
    POINT pt;
    RECT rc;
    int32_t ebp = 0;
    int32_t ebx = 0;
    int32_t esi = 0;
    int32_t r13d = param_2;
    DWORD sub888 = 0;              /* [rsp+0x98] */
    DWORD sub40 = 0;               /* [rsp+0x40] */
    int64_t r12 = 0;
    uint16_t rv;

    memset(&pt, 0, sizeof(pt));
    memset(&rc, 0, sizeof(rc));

    if (msg == 0x44f) {
        GetCursorPos(&pt);
        memset(&rc, 0, sizeof(rc));
        GetWindowRect(hwnd, &rc);

        ebx = 1;
        if (lParam == 0) {
            ebp = (int32_t)(GetAsyncKeyState(ebx) & 0x8000);
        } else {
            ebp = (int32_t)(uint8_t)lParam;
        }

        if (PtInRect(&rc, pt) != 0) {
            if ((int8_t)obj[0xad] > 0) {
                obj[0xad] = 0;
                InvalidateRect(hwnd, NULL, 1);
            } else {
                int8_t nv = (int8_t)(obj[0xad] - 1u);
                obj[0xad] = (uint8_t)nv;
                if (!(nv >= (int8_t)0x9c)) {
                    obj[0xad] = 0;
                    KillTimer(hwnd, 0x2713);
                }
            }
            /* eax = 0 */
        } else if (ebp != 0) {
            if ((int8_t)obj[0xad] > 0) {
                return;
            }
            obj[0xad] = 1;
            InvalidateRect(hwnd, NULL, 1);
            SetTimer(hwnd, 0x2713, 10, NULL);
            ebp = 0x201;
            PECMD_RestoreForegroundWindow();
            FUN_1400F172C(*(int64_t **)(obj + 0xd8), ebp, 0, NULL,
                          (int64_t)hwnd, 0x80, NULL);
            return;
        } else {
            if ((int8_t)obj[0xad] <= 0) {
                return;
            }
            obj[0xad] = 0;
            InvalidateRect(hwnd, NULL, 1);
            KillTimer(hwnd, 0x2713);
            if ((obj[0xd1] & 0x40) == 0) {
                (void)GetDlgCtrlID(hwnd);
                ebx = (int32_t)(uint16_t)GetDlgCtrlID(hwnd);
                ebx |= 0x2020000;
                (void)GetParent(hwnd);
                PostMessageW((HWND)(intptr_t)GetParent(hwnd), 0x441,
                             (WPARAM)ebx, (LPARAM)0x202);
                ebp = 0x202;
                PECMD_RestoreForegroundWindow();
                FUN_1400F172C(*(int64_t **)(obj + 0xd8), ebp, 0, NULL,
                              (int64_t)hwnd, 0x80, NULL);
            }
            return;
        }
    }

    if (msg == 0x44e) {
        /* 虚表方法 +0xe0 转发 */
        {
            int64_t (*fn)(void *, int64_t) =
                (int64_t (*)(void *, int64_t))(uintptr_t)(
                    *(uint64_t *)((uint8_t *)param_1 + 0xe0));
            (void)fn(param_1, wParam);
        }
        return;
    }

    if (msg == 0x462) {
        if (lParam != (uint64_t)(uintptr_t)param_1) {
            return;
        }
        if ((uint16_t)wParam == 0x3f) {
            ebx = 1;
            *(uint16_t *)wParam = (uint16_t)(~((uint8_t)obj[0xd1] >> 6) & 1u);
            return;
        }
        {
            uint16_t *p = (uint16_t *)wParam;
            sub888 = 0;
            if (*(uint16_t *)wParam == 0x23) {
                p = (uint16_t *)(wParam + 2);
            }
            PECMD_ParseNumTryWriteback((void **)&p, (int *)&sub888);
            if ((int32_t)sub888 > 0) {
                obj[0xd1] &= 0xbf;
            } else {
                KillTimer(hwnd, 0x2713);
                obj[0xd1] |= 0x40;
                obj[0xad] = 0;
            }
            ebx = 1;
            if ((int32_t)sub888 > 0) {
                PECMD_InvalidateParentRect(hwnd, ebx);
            } else {
                InvalidateRect(hwnd, NULL, 1);
            }
            return;
        }
    }

    if (msg == 0x45a) {
        esi = (((uint8_t)obj[0xa4] & 0x10) == 0 &&
               ((uint8_t)obj[0xa4] & 0x3) != 0) ? 1 : 0;
        PECMD_SetControlText(*(HWND *)(obj + 0x20), wParam,
                             (LPCWSTR)lParam, esi);
        ebx = 1;
        return;
    }

    /* ===== 默认分支：字段追加 ===== */
    {
        int64_t *map = *(int64_t **)(obj + 0xd8);
        int32_t count = *(int32_t *)((uint8_t *)map + 0x10);
        if (count > 0) {
            r12 = (int64_t)FUN_1400E5788(hwnd);
        } else {
            r12 = 0;
        }
        if (r12 != 0) {
            SendMessageW((HWND)r12, 0x450, (WPARAM)4, (LPARAM)0x5aa555aa);
        }

        if (count > 0) {
            sub40 = 0;
            rv = FUN_1400F172C(map, r13d, (uint64_t)wParam, (uint64_t *)&lParam,
                               (int64_t)hwnd, 1, (int64_t *)&sub40);
            if (rv & 0x4) {
                ebx = (int32_t)sub40;
            } else {
                ebx = (int32_t)FUN_1400E5B0C((int64_t)param_1, (uint64_t)r13d,
                                             wParam, (int64_t *)&lParam);
                if (count > 0) {
                    FUN_1400F172C(map, r13d, (uint64_t)wParam,
                                  (uint64_t *)&lParam, (int64_t)hwnd, 2, NULL);
                }
            }
        } else {
            ebx = (int32_t)FUN_1400E5B0C((int64_t)param_1, (uint64_t)r13d,
                                         wParam, (int64_t *)&lParam);
        }

        if (r12 != 0) {
            PostMessageW((HWND)r12, 0x451, (WPARAM)0x204,
                         (LPARAM)0xffffffffaa555aa5ULL);
        }
        (void)ebx;
        return;
    }
}
