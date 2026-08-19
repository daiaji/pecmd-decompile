/* ====================================================================
 * core_b3r_g5.c — 还原 4 个业务函数 (WIM 镜像 / 文本清洗 / 存储依赖)
 *
 *   PECMD_CleanScriptText @0x1400752a0  清洗文本(去注释/空白/CRLF)
 *   PECMD_ApplyWimImage @0x140075628  WIM 镜像写入/应用 (Wimgapi 延迟槽)
 *   PECMD_CaptureWimImage @0x140075b04  WIM 镜像捕获 (Wimgapi 延迟槽)
 *   PECMD_GetStorageDependency @0x140077c1c  获取存储依赖信息 (VirtDisk)
 *
 * 来源: PECMD原始.EXE (x64, ImageBase=0x140000000) Ghidra 反编译
 * ==================================================================== */

#include <stdbool.h>

#include "win32_stub.h"
#include "pecmd_defs.h"

/* ------------------------------------------------------------------
 * 本文件引用的 Win32 API (win32_stub.h 未声明者在此补齐)
 * ------------------------------------------------------------------ */
extern DWORD GetTempPathW(DWORD nBufferLength, LPWSTR lpBuffer);

/* ------------------------------------------------------------------
 * 本文件引用的辅助函数 (均为 extern, 正文不在此定义)
 * ------------------------------------------------------------------ */
extern void   *FUN_14001d744(void *dst, const void *src, int len);      /* @0x14001d744 重叠搬移(memmove) */
extern void    PECMD_AllocWStringBuffer(WCHAR **ps, int64_t count);                /* @0x140063694 分配 */
extern void    FUN_14005b104(void *ps);                                 /* @0x14005b104 释放字符串槽 */
extern void    FUN_140063620(WCHAR **out);                              /* @0x140063620 初始化串缓冲 */
extern WCHAR  *PECMD_AllocString(WCHAR **ps, int64_t count);                /* @0x140063720 分配串 */
extern void    FUN_1400633a8(void **ps, int64_t len);                   /* @0x1400633a8 分配 */
extern void    FUN_14005b0b8(void *p);                                  /* @0x14005b0b8 缓冲区构造 */
extern void    FUN_14005c828(const char *func, const char *dll,
                             void **out, uintptr_t *hmod);              /* @0x14005c828 GetProcAddress 包装 */
extern uint64_t FUN_14006a7f4(int64_t *param_1, uint64_t *param_2);     /* @0x14006a7f4 尺寸换算 */
extern void    PECMD_OpenFileHandle(HANDLE *out, LPCWSTR path, DWORD access,
                             DWORD share, LPSECURITY_ATTRIBUTES sa,
                             DWORD disp, DWORD flags, HANDLE tmpl);     /* @0x140003864 CreateFileW 包装 */
extern WCHAR  *PECMD_EnumerateVolume(int64_t *param_1, int64_t param_2,
                             uint64_t param_3, LPCWSTR param_4);        /* @0x140006aa0 卷对象/挂载点解析 */

/* ------------------------------------------------------------------
 * 本文件引用的全局
 * ------------------------------------------------------------------ */
extern uint64_t g_u642d1c8;             /* 16 字节 GUID 前 8 字节 (.rdata) */
extern uint64_t _UNK_14012d1d0;             /* 16 字节 GUID 后 8 字节 (.rdata) */

/* ------------------------------------------------------------------
 * 本文件引用的函数指针槽 (Wimgapi / VirtDisk 延迟加载)
 *  WIM 镜像 API (wtc/wimgapi):
 *   g_pWIMCreateFile = 打开/创建 WIM (WIMCreateFile)
 *   g_pWIMSetTemporaryPath = 设置临时路径 (WIMSetTemporaryPath)
 *   g_pWimLoadImage = 装载映像 (WIMLoadImage)
 *   g_pWIMHandleOp480 = 应用映像 (WIMApplyImage)
 *   g_pWIMCloseHandleSlot = 关闭句柄 (WIMCloseHandle)
 * ------------------------------------------------------------------ */
typedef uint64_t (*FN_14013d468)(uint64_t wim, uint32_t mode, int f3,
                                 int f4, int f5, uint32_t *out);
typedef int      (*FN_14013d470)(uint64_t wim, WCHAR *path);
typedef uint64_t (*FN_14013d478)(uint64_t wim, unsigned int index);
typedef int      (*FN_14013d480)(uint64_t h, uint64_t data, uint32_t mode);
typedef void     (*FN_14013d488)(uint64_t h);
typedef int      (*FN_14013d730)(void *guid, uint64_t a2, uint32_t a3,
                                 int a4, uint32_t a5, int a6,
                                 uint32_t *a7, int a8, HANDLE *a9);
typedef int      (*FN_14013d3b0)(HANDLE h, int a2, uint32_t a3,
                                 void *a4, uint64_t *a5);

extern uint64_t (*g_pWIMCreateFile)(uint64_t wim, uint32_t mode, int f3, int f4, int f5, uint32_t *out);
extern int (*g_pWIMSetTemporaryPath)(uint64_t wim, WCHAR *path);
extern uint64_t (*g_pWimLoadImage)(uint64_t wim, unsigned int index);
extern int (*g_pWIMHandleOp480)(uint64_t h, uint64_t data, uint32_t mode);
extern void (*g_pWIMCloseHandleSlot)();
extern int (*g_pGetStorageDependencyInformation)(HANDLE h, int a2, uint32_t a3, void *a4, uint64_t *a5);

/* ================================================================
 * @0x1400752a0  清洗文本 (去注释/空白/CRLF)
 * signature: ulonglong __fastcall PECMD_CleanScriptText(short *param_1,
 *               int param_2, uint param_3)
 */
uint64_t PECMD_CleanScriptText(short *param_1, int param_2, unsigned int param_3)
{
    bool   bVar1;
    bool   bVar2;
    short  sVar3;
    short *psVar4;
    short *psVar5;
    short *psVar6;
    short *psVar7;

    psVar7 = param_1 + param_2;
    *psVar7 = 0;
    psVar4 = param_1;
    if (*param_1 == (short)-0x101) {          /* 0xFEFF BOM */
        psVar4 = param_1 + 1;
    }
    bVar1 = true;
    if (((param_3 & 8) != 0) && (psVar5 = psVar4, psVar4 < psVar7)) {
        do {
            psVar6 = psVar5;
            if ((*psVar5 == 10) || (*psVar5 == 0xd)) {
                bVar1 = true;
            } else {
                bVar2 = false;
                if ((((bVar1) || (*psVar5 == 9)) || (*psVar5 == 0xb)) ||
                    ((*psVar5 == 0xc || (bVar1 = bVar2, *psVar5 == 0x20)))) {
                    for (; (((*psVar6 == 9 ||
                              ((*psVar6 == 0xb || (*psVar6 == 0xc)))) ||
                             (*psVar6 == 0x20))); psVar6 = psVar6 + 1) {
                    }
                    sVar3 = *psVar6;
                    if ((sVar3 == 0x60) || ((bVar1 = bVar2,
                                             sVar3 == 0x2f && (psVar6[1] == 0x2f)))) {
                        while ((sVar3 != 0 && (sVar3 != 0xd && (sVar3 != 10)))) {
                            psVar6 = psVar6 + 1;
                            sVar3 = *psVar6;
                        }
                        /* 折叠整行注释: 搬移尾部 */
                        FUN_14001d744((void *)psVar5, (const void *)psVar6,
                                      ((char *)psVar7 - (char *)psVar6) + 2);
                        psVar7 -= (psVar6 - psVar5);
                        psVar6 = psVar5;
                        bVar1 = bVar2;
                    }
                }
            }
            psVar5 = psVar6 + 1;
        } while (psVar5 < psVar7);
    }

    if (((param_3 & 1) != 0) && (psVar5 = psVar4, psVar6 = psVar4,
                                 psVar4 < psVar7)) {
        do {
            for (; (*psVar5 == 9 ||
                    (((*psVar5 == 0xb || (*psVar5 == 0xc)) ||
                      (*psVar5 == 0x20)))); psVar5 = psVar5 + 1) {
            }
            sVar3 = *psVar5;
            if (((((sVar3 == 0x60) || (sVar3 == 0x7e)) ||
                  ((sVar3 == 0x3b || ((sVar3 == 0x2f || (sVar3 == 0x23)))))) ||
                 (sVar3 == 0x27)) ||
                ((sVar3 == 0x3d || (bVar1 = false, sVar3 == 0x3a)))) {
                bVar1 = true;
            }
            if (((bVar1) && (sVar3 == 0x23)) &&
                ((psVar5[1] == 0x21 ||
                  ((unsigned short)(psVar5[1] | 0x20U) == 99)))) {
                bVar1 = false;
            }
            while (((sVar3 != 0 && (sVar3 != 0xd)) && (sVar3 != 10))) {
                psVar5 = psVar5 + 1;
                sVar3 = *psVar5;
            }
            if (bVar1) {
                FUN_14001d744((void *)psVar6, (const void *)psVar5,
                              ((char *)psVar7 - (char *)psVar5) + 2);
                psVar7 -= (psVar5 - psVar6);
                psVar5 = psVar6;
            }
            if (*psVar5 == 0xd) {
                psVar5 = psVar5 + 1;
            }
            if (*psVar5 == 10) {
                psVar5 = psVar5 + 1;
            }
            psVar6 = psVar5;
        } while (psVar5 < psVar7);
    }

    for (; ((sVar3 = *psVar4, sVar3 != 0 && (sVar3 != 0xd)) && (sVar3 != 10));
         psVar4 = psVar4 + 1) {
    }

    if ((param_3 & 4) == 0) {
        if ((param_3 & 2) != 0) {
            while (psVar4 < psVar7) {
                psVar5 = psVar4 + 1;
                if ((*psVar4 == 0xd) && (*psVar4 = 10, *psVar5 == 10)) {
                    FUN_14001d744((void *)psVar4, (const void *)psVar5,
                                  ((char *)psVar7 - (char *)psVar5) + 2);
                    psVar7 = psVar7 - 1;
                }
                for (; ((sVar3 = *psVar5, psVar4 = psVar5,
                         sVar3 != 0 && (sVar3 != 0xd)) && (sVar3 != 10));
                     psVar5 = psVar5 + 1) {
                }
            }
        }
    } else {
        while (psVar4 < psVar7) {
            *psVar4 = 10;
            psVar5 = psVar4 + 1;
            psVar4 = psVar5;
            if (psVar5 != (short *)0x0) {
                for (; (*psVar4 == 10 || (*psVar4 == 0xd)); psVar4 = psVar4 + 1) {
                }
                if (psVar5 != psVar4) {
                    FUN_14001d744((void *)psVar5, (const void *)psVar4,
                                  ((char *)psVar7 - (char *)psVar4) + 2);
                    psVar7 -= (psVar4 - psVar5);
                    psVar4 = psVar5;
                }
            }
            for (; ((sVar3 = *psVar4, sVar3 != 0 && (sVar3 != 0xd)) && (sVar3 != 10));
                 psVar4 = psVar4 + 1) {
            }
        }
    }

    /* (psVar7 - param_1) 为 short 个数, 对应 Ghidra 的 (byte数目 >> 1) */
    return (uint64_t)(uint32_t)(psVar7 - param_1);
}

/* ================================================================
 * @0x140075628  WIM 镜像写入/应用
 * signature: int __fastcall PECMD_ApplyWimImage(undefined8 param_1,
 *               undefined8 param_2, undefined4 param_3, WCHAR * param_4)
 */
int PECMD_ApplyWimImage(uint64_t param_1, uint64_t param_2, unsigned int param_3,
                  WCHAR *param_4)
{
    DWORD     DVar1;
    int       iVar2;
    int       iVar3;
    uint64_t  lVar4;
    uint64_t  lVar5;
    uint32_t  uVar6;
    DWORD     DVar7;
    uint32_t  local_res20[2];
    int64_t   local_248[2];
    WCHAR     local_238[264];

    DVar1 = 1;
    local_238[0] = L'\0';
    PECMD_AllocWStringBuffer((WCHAR **)local_248, 0x100000);
    DVar7 = DVar1;
    if (param_4 == (WCHAR *)0x0) {
        DVar1 = GetTempPathW(0x104, local_238);
        param_4 = local_238;
        iVar3 = 0;
        DVar7 = 0;
        if (DVar1 == 0) {
            goto LAB_14007581a;
        }
    }
    uVar6 = 0xa0000000;
    if (DVar7 != 0) {
        uVar6 = 0xe0000000;
    }
    local_res20[0] = 0;
    lVar4 = (*g_pWIMCreateFile)(param_2, uVar6, 3, 0, 0, local_res20);
    if (lVar4 == 0) {
        do {
            if (DVar7 != 1) {
                break;
            }
            DVar7 = 2;
            uVar6 = uVar6 & 0xbfffffff;
            lVar4 = (*g_pWIMCreateFile)(param_2, uVar6, 3, 0, 0, local_res20);
        } while (lVar4 == 0);
        if (lVar4 == 0) {
            GetLastError();
            DVar1 = 0;
        }
    }
    iVar3 = 0;
    if (DVar1 != 0) {
        iVar2 = (*g_pWIMSetTemporaryPath)(lVar4, param_4);
        if (iVar2 == 0) {
            GetLastError();
            iVar2 = 1;
        }
        lVar5 = (*g_pWimLoadImage)(lVar4, param_3);
        if (lVar5 == 0) {
            GetLastError();
            iVar2 = 0;
        }
        iVar3 = 0;
        if (iVar2 != 0) {
            uVar6 = 0x400;
            if (DVar7 == 0) {
                uVar6 = 0x600;
            }
            iVar3 = (*g_pWIMHandleOp480)(lVar5, param_1, uVar6);
            DVar1 = GetLastError();
            if (iVar3 == 0) {
                do {
                    if ((uVar6 >> 10 == 0) || (DVar1 != 0x57)) {
                        break;
                    }
                    uVar6 = uVar6 & 0xfffffbff;
                    iVar3 = (*g_pWIMHandleOp480)(lVar5, param_1, uVar6);
                    DVar1 = GetLastError();
                } while (iVar3 == 0);
                if ((iVar3 == 0) &&
                    ((DVar7 != 2 ||
                      (iVar3 = (*g_pWIMHandleOp480)(lVar5, param_1,
                                                uVar6 | 0x200), iVar3 == 0)))) {
                    GetLastError();
                }
            }
        }
        if (lVar5 != 0) {
            (*g_pWIMCloseHandleSlot)(lVar5);
        }
    }
    if (lVar4 != 0) {
        (*g_pWIMCloseHandleSlot)(lVar4);
    }
LAB_14007581a:
    FUN_14005b104((void *)local_248);
    return iVar3;
}

/* ================================================================
 * @0x140075b04  WIM 镜像捕获
 * signature: int __fastcall PECMD_CaptureWimImage(undefined8 param_1,
 *               longlong param_2, undefined4 param_3, undefined4 param_4,
 *               uint param_5)
 */
int PECMD_CaptureWimImage(uint64_t param_1, int64_t param_2, unsigned int param_3,
                  unsigned int param_4, unsigned int param_5)
{
    FN_14013d730 pcVar1;
    int       iVar2;
    int64_t   local_res10;
    uint32_t  local_b8;              /* 结构第 1 个 DWORD (来自 param_4) */
    uint32_t  local_b4;
    uint16_t  local_b0;
    uint16_t  local_ae;
    uint8_t   local_ac, local_ab, local_aa, local_a9;
    uint8_t   local_a8, local_a7, local_a6, local_a5;
    HANDLE    local_a0;
    uint64_t  local_98[2];
    uint32_t  local_88[2];
    uint64_t  local_80;              /* GUID 前 8 字节 */
    uint64_t  uStack_78;             /* GUID 后 8 字节 */
    int64_t   local_70;
    uint32_t  local_68;
    uint32_t  local_64;
    int64_t   local_60;
    uint64_t  local_58;

    pcVar1 = g_pCreateVirtualDisk;
    if (g_pCreateVirtualDisk == (FN_14013d730)0x0) {
        iVar2 = (int)-0x7fffbfff;
    } else {
        local_98[0] = 0;
        local_res10 = param_2;
        FUN_14006a7f4(&local_res10, local_98);
        local_80 = g_u642d1c8;
        uStack_78 = _UNK_14012d1d0;
        local_a0 = (HANDLE)0xffffffffffffffff;
        local_68 = 0;
        local_58 = 0;
        local_64 = param_5;
        /* 组装 16 字节 GUID (little-endian): 见下方 local_b0..local_a5 */
        local_b0 = 0xa0f9;
        local_ae = 0x47e9;
        local_b4 = 0xec984aec;
        local_ac = 0x90;
        local_ab = 0x1f;
        local_aa = 0x71;
        local_a9 = 0x41;
        local_a8 = 0x5a;
        local_a7 = 0x66;
        local_a6 = 0x34;
        local_a5 = 0x5b;
        local_88[0] = 1;
        local_70 = ((int64_t)local_98[0] / (int64_t)(uint64_t)param_5) *
                   (uint64_t)param_5;
        local_60 = 0;
        if (local_98[0] == 0) {
            local_60 = local_res10;
        }
        local_b8 = param_4;
        /* silence unused-but-set (decompiled dead stack slots) */
        (void)local_58; (void)local_60; (void)local_64; (void)local_68;
        (void)local_70; (void)local_80; (void)uStack_78;
        (void)local_b4; (void)local_b0; (void)local_ae;
        (void)local_ac; (void)local_ab; (void)local_aa; (void)local_a9;
        (void)local_a8; (void)local_a7; (void)local_a6; (void)local_a5;
        iVar2 = (*pcVar1)(&local_b8, param_1, 0x100000, 0, param_3, 0,
                          local_88, 0, &local_a0);
        if ((iVar2 == 0) && (local_a0 != (HANDLE)0xffffffffffffffff)) {
            CloseHandle(local_a0);
        }
    }
    return iVar2;
}

/* ================================================================
 * @0x140077c1c  获取存储依赖信息 (PhysicalDrive/CDROM)
 * signature: int __fastcall PECMD_GetStorageDependency(undefined8 param_1,
 *               undefined8 * param_2, WCHAR * param_3, int param_4)
 */
int PECMD_GetStorageDependency(uint64_t param_1, uint64_t *param_2, WCHAR *param_3,
                  int param_4)
{
    LPCWSTR lpString;
    HANDLE  hObject;
    int     iVar1;
    int     iVar2;
    int     iVar3;
    WCHAR  *pWVar4;
    LPCWSTR pwVar5;
    LPCWSTR lpString_00;
    HANDLE  local_108;
    uint64_t *local_100;
    uint64_t local_f8[2];
    WCHAR   local_e8[104];

    EnterCriticalSection((void *)&g_csInit);
    FUN_14005c828("GetStorageDependencyInformation", "VirtDisk.DLL",
                  (void **)&g_pGetStorageDependencyInformation, (uintptr_t *)0x0);
    iVar1 = 0;
    if (g_pGetStorageDependencyInformation == (FN_14013d3b0)0x0) {
        LeaveCriticalSection((void *)&g_csInit);
        iVar1 = 0;
    } else {
        local_108 = (HANDLE)0x0;
        if ((uintptr_t)param_3 < 0x10000) {
            pwVar5 = WSTR("\\\\.\\PhysicalDrive%d");
            if (param_4 == 8) {
                pwVar5 = WSTR("\\\\.\\CDROM%d");
            }
            wsprintfW(local_e8, pwVar5, (int)(intptr_t)param_3); /* TODO(verify) vararg */
            param_3 = local_e8;
        }
        PECMD_OpenFileHandle(&local_108, param_3, 0x80000000, 3,
                      (LPSECURITY_ATTRIBUTES)0x0, 3, 0x22000000, (HANDLE)0x0);
        hObject = local_108;
        if (local_108 != (HANDLE)0x0) {
            FUN_1400633a8((void **)&local_100, 0x7ff0);
            FUN_14005b0b8((void *)local_100);
            *(uint32_t *)local_100 = 2;
            local_f8[0] = 0;
            iVar1 = (*g_pGetStorageDependencyInformation)(hObject, 3, 0x7ff0, local_100, local_f8);
            lpString_00 = (LPCWSTR)local_100[6];
            lpString = (LPCWSTR)local_100[8];
            if (((iVar1 == 0) && (lpString_00 != (LPCWSTR)0x0)) &&
                (lpString != (LPCWSTR)0x0)) {
                FUN_140063620((WCHAR **)&local_108);
                pWVar4 = PECMD_EnumerateVolume((int64_t *)&local_108, -5,
                                       0xfffffffff008fffb, lpString_00);
                if (((pWVar4 != (WCHAR *)0x0) && (*pWVar4 != L'\0')) &&
                    (pWVar4[1] == L':')) {
                    pWVar4[2] = L'\0';
                    lpString_00 = pWVar4;
                }
                iVar2 = lstrlenW(lpString_00);
                iVar3 = lstrlenW(lpString);
                PECMD_AllocString((WCHAR **)param_2, (int64_t)(iVar3 + iVar2 + 8));
                wsprintfW((LPWSTR)*param_2, WSTR("%s%s"),
                          lpString_00, lpString);   /* TODO(verify) varargs */
                FUN_14005b104((void *)&local_108);
            }
            FUN_14005b104((void *)&local_100);
            if (hObject != (HANDLE)0xffffffffffffffff) {
                CloseHandle(hObject);
            }
        }
        LeaveCriticalSection((void *)&g_csInit);
    }
    return iVar1;
}
