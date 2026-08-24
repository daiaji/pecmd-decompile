/* ====================================================================
 * partC.c — restored business functions from Ghidra decompiled export.
 *   Contains: PECMD_QueryDevice, PECMD_ReadModifyWrite, PECMD_ShowShutdownUI,
 *             PECMD_SaveBitmap, PECMD_LoadIconFrames
 * ==================================================================== */
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "pecmd_defs.h"

/* ---------- extern helpers / globals / Win32 APIs (not in headers) ---------- */
/* helpers */
extern void PECMD_AllocStringSlot2(void **ps, int64_t len); /* @0x1400633a8 alloc */
extern void PECMD_FreeStrBuf(void *ps);                     /* @0x14005b104 free */
extern void PECMD_ZeroLenBuf(void *p);                      /* @0x14005b0b8 object init */
extern void *PECMD_GrowByteBuffer(void **ps, int64_t len);  /* @0x140063424 realloc/alloc */
extern LARGE_INTEGER PECMD_SetFilePointer(HANDLE h, LARGE_INTEGER dist,
                                          DWORD method); /* @0x14005c674 */
extern void PECMD_TlsLogWrite(uint64_t ctx, const WCHAR *fmt, uint64_t a,
                              uint64_t b); /* @0x140018d8c debug */
extern void PECMD_AsciiTrimToWide(void **pp, const void *src,
                                  uint64_t len); /* @0x14005fe34 append bytes as wide */
extern uint16_t *PECMD_DataToWideString(int64_t data, int start, int end,
                                        uint16_t *out);                    /* @0x14005fd7c */
extern HICON PECMD_LoadIcon(LPCWSTR p, uint64_t *a2);                      /* @0x14001f1d4 */
extern uint64_t PECMD_LoadImageStream(uint8_t *p, size_t n, uint64_t *p3); /* @0x140061e98 */
extern int64_t PECMD_VectorSlotPtr(int64_t a, int64_t *b, int64_t *c,
                                   uint32_t d); /* @0x140063b00 */
extern int64_t PECMD_EnableTokenPrivilege(LPCWSTR priv, DWORD attr,
                                          uint32_t flag); /* @0x14001c2cc EnablePrivilege */
extern uint64_t PECMD_TaskShowMessage(uint64_t *param_1); /* @0x1400612cc thread proc */
extern uint64_t PECMD_FindTargetWindow(LPCWSTR text, char matchCase, char matchWord, HWND parent,
                                       uint32_t flags1, uint32_t flags2, uint64_t value1,
                                       uint64_t value2); /* @0x1400e3d60 */
extern DWORD PECMD_EncodeImageToStream(int64_t param_1, LPCWSTR param_2, LPCWSTR param_3,
                                       int param_4); /* @0x14007d340 */

/* Win32 API not present in headers */
extern int StrCmpIW(LPCWSTR, LPCWSTR);
extern HDC CreateDCW(LPCWSTR, LPCWSTR, LPCWSTR, const void *);
extern HPALETTE SelectPalette(HDC, HPALETTE, BOOL);
extern UINT RealizePalette(HDC);

/* SIZE_T is not declared in win32_stub.h */
typedef size_t SIZE_T;

/* globals (DAT_*) */
extern uint8_t g_u8CCB1;                 /* MAIN_DBG 日志标志 */
extern DWORD g_imgBufLen;                /* 位图数据大小 */
extern void *g_pGdipLoadImageFromStream; /* GdipLoadImageFromStream */

/* GDI+ function-pointer slots */
extern void *g_pGdipLoadImageFromFile;            /* GdipLoadImageFromFile */
extern void *g_pGdipImageGetFrameDimensionsCount; /* GdipImageGetFrameDimensionsCount */
extern void *g_pGdipImageGetFrameDimensionsList;  /* GdipImageGetFrameDimensionsList */
extern void *g_pGdipImageGetFrameCount;           /* GdipImageGetFrameCount */
extern void *g_pGdipGetPropertyItemSize;          /* GdipGetPropertyItemSize */
extern void *g_pGdipGetPropertyItem;              /* GdipGetPropertyItem */

/* 已由 pecmd_defs.h 提供: g_privFlags(g_privFlags), g_pSharedMap(DAT_14013ca30),
 * g_csInit(g_csInit), g_hInst(g_hInst) */

/* ====================================================================
 * PECMD_QueryDevice @0x1400773f8
 *   DeviceIoControl 磁盘查询 -> 生成格式化字符串到 param_3
 * signature: ulonglong __fastcall PECMD_QueryDevice(HANDLE param_1, undefined8 param_2,
 *             undefined2 * param_3, longlong param_4)
 * ==================================================================== */
uint64_t PECMD_QueryDevice(HANDLE param_1, uint64_t param_2, uint16_t *param_3, int64_t param_4)
{
    (void)param_2;
    uint8_t *buf;         /* local_48[0] */
    uint8_t *lpOutBuffer; /* buf + 0x400 */
    uint16_t *run;        /* local_res18 */
    DWORD local_54[2];
    int iVar9;
    uint32_t uVar10;
    uint16_t *puVar11;
    uint16_t *puVar12;
    uint16_t uVar4;
    uint32_t uVar8;
    uint32_t uVar13;
    int bVar5 = 0; /* bool bVar5 */
    int bVar6 = 0; /* bool bVar6 */
    int bVar7 = 0; /* bool bVar7 */
    uint8_t bVar3;
    uint8_t local_res20 = 0; /* char */
    char cVar14;
    int16_t *psVar2;
    uint8_t *puVar1;

    local_54[0] = 0;
    PECMD_AllocStringSlot2((void **)&buf, 0x1410);
    lpOutBuffer = buf + 0x400;

    memset(buf, 0, 0xc);
    *(uint32_t *)(buf + 0) = 0;
    *(uint32_t *)(buf + 4) = 0;
    *(uint8_t *)(buf + 8) = 0;
    memset(lpOutBuffer, 0, 0x28);
    uVar8 = DeviceIoControl(param_1, 0x2d1400, buf, 0xc, lpOutBuffer, 0x828, &local_54[0], 0);
    iVar9 = *(int32_t *)(buf + 0x41c);
    if ((int)uVar8 < 1) {
        *(uint32_t *)(buf + 0x418) = 0;
        *(uint32_t *)(buf + 0x410) = 0;
    }
    if (g_u8CCB1 != 0) {
        PECMD_TlsLogWrite((uint64_t)(uintptr_t)g_Script, WSTR("DBGG %d %d %d: %d %d %d\r\n"),
                          0x36a5, (uint64_t)param_4);
    }

    *param_3 = 0x22;
    run = param_3 + 1;
    if (*(uint32_t *)(buf + 0x410) != 0) {
        PECMD_AsciiTrimToWide(
            (void **)&run, (char *)(lpOutBuffer + *(uint32_t *)(buf + 0x410)),
            (uint64_t)(uint32_t)(*(int32_t *)(buf + 0x404) - (int)*(uint32_t *)(buf + 0x410)));
    }
    *run = 0x22;
    puVar11 = run + 3;
    run[1] = 0x20;
    run[2] = 0x22;
    uVar10 = *(uint32_t *)(buf + 0x418);
    run = puVar11;
    if (uVar10 != 0) {
        PECMD_AsciiTrimToWide((void **)&run, (char *)(lpOutBuffer + uVar10),
                              (uint64_t)(uint32_t)(*(int32_t *)(buf + 0x404) - uVar10));
    }
    puVar11 = run;
    *run = 0x22;
    run[1] = 0;

    if (param_4 == 0) {
        if (iVar9 == 0x11)
            goto L140077696;
        if (iVar9 == 7)
            goto L1400777ea;
        cVar14 = local_res20;
        if (iVar9 == 5)
            goto L1400777ea;
    L1400779bd:
        do {
            bVar6 = 1;
            memset(lpOutBuffer, 0, 0x400);
            memset(buf, 0, 0x29);
            *(uint32_t *)(buf + 0) = 0x200;
            *(uint8_t *)(buf + 5) = 1;
            *(uint8_t *)(buf + 6) = 1;
            *(uint8_t *)(buf + 9) = 0xa0;
            *(uint8_t *)(buf + 10) = 0xec;
            uVar8 =
                DeviceIoControl(param_1, 0x7c088, buf, 0x21, lpOutBuffer, 0x400, &local_54[0], 0);
            uVar13 = uVar8;
            if (g_u8CCB1 != 0) {
                PECMD_TlsLogWrite((uint64_t)(uintptr_t)g_Script, WSTR("DBGA %d: %d 0x%X 0x%X\r\n"),
                                  0x36df, (uint64_t)uVar13);
            }
            if ((uVar8 != 0) && (*(uint16_t *)(buf + 0x424) > 0x1fff) &&
                (*(uint16_t *)(buf + 0x446) > 0x1fff)) {
                *param_3 = 0x22;
                puVar11 = PECMD_DataToWideString((int64_t)(buf + 0x410), 0x1b, 0x2e, param_3 + 1);
                *puVar11 = 0x22;
                puVar11[1] = 0x20;
                puVar11[2] = 0x22;
                puVar12 = PECMD_DataToWideString((int64_t)(buf + 0x410), 10, 0x13, puVar11 + 3);
                uVar4 = puVar11[3];
                *puVar12 = 0x22;
                puVar12[1] = 0;
                if (uVar4 != 0)
                    break;
            }
            if (cVar14 != 0)
                goto L1400777db;
        L140077696:
            do {
                local_res20 = 0x01;
                memset(buf, 0, 0x1030);
                *(uint32_t *)(buf + 0) = 0x31;
                *(uint32_t *)(buf + 4) = 0;
                *(uint32_t *)(buf + 0x14) = 0;
                *(uint32_t *)(buf + 8) = 2;
                *(uint32_t *)(buf + 0xc) = 1;
                *(uint32_t *)(buf + 0x10) = 1;
                *(uint32_t *)(buf + 0x18) = 0x28;
                *(uint32_t *)(buf + 0x1c) = 0x1000;
                uVar8 =
                    DeviceIoControl(param_1, 0x2d1400, buf, 0x1030, buf, 0x1030, &local_54[0], 0);
                uVar13 = uVar8;
                psVar2 = (int16_t *)(buf + 8 + (uint32_t)*(uint64_t *)(buf + 0x18));
                if (((uVar8 < 1) || (*(int32_t *)(buf + 0) != 0x30)) ||
                    (*(int32_t *)(buf + 4) != 0x30 || ((uint32_t)*(uint64_t *)(buf + 0x18) < 0x28 ||
                                                       (*(uint32_t *)(buf + 0x1c) < 0x1000))) ||
                    (*psVar2 == 0) || ((bVar5 = 1), *(int32_t *)(psVar2 + 0x102) == 0)) {
                    bVar5 = 0;
                }
                if (g_u8CCB1 != 0) {
                    GetLastError();
                    PECMD_TlsLogWrite((uint64_t)(uintptr_t)g_Script,
                                      WSTR("DBGN %d: %d %d %d  %d %d %d %d %d err=%d\r\n"), 0x370b,
                                      (uint64_t)uVar8);
                }
                if (bVar5) {
                    run = param_3 + 1;
                    *param_3 = 0x22;
                    PECMD_AsciiTrimToWide((void **)&run, (char *)(psVar2 + 0xc), 0x28);
                    *run = 0x22;
                    run[1] = 0x20;
                    run[2] = 0x22;
                    run = run + 3;
                    PECMD_AsciiTrimToWide((void **)&run, (char *)(psVar2 + 2), 0x14);
                    *run = 0x22;
                    run[1] = 0;
                    goto L140077bf4;
                }
                cVar14 = local_res20;
                if (!bVar6)
                    goto L1400779bd;
            L1400777db:
                if (!bVar7) {
                L1400777ea:
                    puVar1 = buf + 0x800;
                    bVar7 = 1;
                    memset(puVar1, 0, 0x38);
                    memset(buf, 0, 0x38);
                    memset(lpOutBuffer, 0, 0x100);
                    *(uint32_t *)(buf + 0xc) = 0x100;
                    *(uint16_t *)(buf + 0) = 0x38;
                    *(uint8_t *)(buf + 6) = 6;
                    *(uint8_t *)(buf + 8) = 1;
                    *(uint32_t *)(buf + 0x10) = 2;
                    *(uint64_t *)(buf + 0x18) = (uint64_t)lpOutBuffer;
                    *(uint8_t *)(buf + 0x24) = 0x12;
                    *(uint8_t *)(buf + 0x28) = 200;
                    uVar8 =
                        DeviceIoControl(param_1, 0x4d014, buf, 0x38, buf, 0x100, &local_54[0], 0);
                    bVar3 = *(uint8_t *)(buf + 2);
                    /* CONCAT44 noise dropped: run is re-set below */

                    memset(puVar1, 0, 4);
                    *(uint64_t *)(buf + 0x804) = 0;
                    memset(buf, 0, 0x38);
                    *(uint8_t *)(buf + 6) = 6;
                    *(uint8_t *)(buf + 8) = 1;
                    *(uint32_t *)(buf + 0xc) = 0x100;
                    *(uint32_t *)(buf + 0x10) = 2;
                    *(uint64_t *)(buf + 0x18) = (uint64_t)puVar1;
                    *(uint16_t *)(buf + 0) = 0x38;
                    *(uint8_t *)(buf + 0x25) |= 1;
                    local_54[1] = 0;
                    *(uint8_t *)(buf + 0x24) = 0x12;
                    *(uint8_t *)(buf + 0x26) = 0x80;
                    *(uint8_t *)(buf + 0x28) = 200;
                    uVar10 =
                        DeviceIoControl(param_1, 0x4d014, buf, 0x38, buf, 0x100, &local_54[1], 0);
                    uVar13 = uVar10;
                    if (uVar8 == 0) {
                        *(uint8_t *)(buf + 0x410) = 0;
                    }
                    if (uVar10 == 0) {
                        *(uint8_t *)(buf + 0x804) = 0;
                    }
                    if ((uVar8 < 1) || (uVar10 < 1) || (*(char *)(buf + 0x803) == '\0') ||
                        ((bVar3 != 0) || ((bVar5 = 1), *(char *)(buf + 2) != '\0'))) {
                        bVar5 = 0;
                    }
                    if (g_u8CCB1 != 0) {
                        PECMD_TlsLogWrite((uint64_t)(uintptr_t)g_Script,
                                          WSTR("DBGS %d: %d %d %d %d %d %d\r\n"), 0x3742,
                                          (uint64_t)uVar8);
                    }
                    cVar14 = local_res20;
                    if (bVar5) {
                        run = param_3 + 1;
                        *param_3 = 0x22;
                        if (uVar8 != 0) {
                            PECMD_AsciiTrimToWide((void **)&run, (char *)(buf + 0x410), 0x10);
                        }
                        *run = 0x22;
                        puVar11 = run + 3;
                        run[1] = 0x20;
                        run[2] = 0x22;
                        *puVar11 = 0;
                        run = puVar11;
                        if ((uVar10 != 0) && (*(char *)(buf + 0x803) != '\0')) {
                            PECMD_AsciiTrimToWide((void **)&run, (char *)(buf + 0x804),
                                                  (uint64_t)*(uint8_t *)(buf + 0x803));
                        }
                        *run = 0x22;
                        uVar13 = 1;
                        run[1] = 0;
                        goto L140077bf4;
                    }
                }
            } while (cVar14 == '\0');
        } while (!bVar6);
    L140077bf4:
        PECMD_FreeStrBuf((void *)&buf);
    }
    else {
        int iVarN;
        iVarN = wsprintfW(run + 1, WSTR(" %d %d %d %d %d \""), (int)*(uint32_t *)lpOutBuffer,
                          (uint64_t)(uint8_t)*(uint64_t *)(buf + 0x408));
        run = puVar11 + iVarN + 1;
        uVar10 = *(uint32_t *)(buf + 0x40c);
        if (uVar10 != 0) {
            PECMD_AsciiTrimToWide((void **)&run, (char *)(lpOutBuffer + uVar10),
                                  (uint64_t)(uint32_t)(*(int32_t *)(buf + 0x404) - uVar10));
        }
        *run = 0x22;
        puVar11 = run + 3;
        run[1] = 0x20;
        run[2] = 0x22;
        uVar10 = *(uint32_t *)(buf + 0x414);
        run = puVar11;
        if (uVar10 != 0) {
            PECMD_AsciiTrimToWide((void **)&run, (char *)(lpOutBuffer + uVar10),
                                  (uint64_t)(uint32_t)(*(int32_t *)(buf + 0x404) - uVar10));
        }
        *run = 0x22;
        run[1] = 0;
        PECMD_FreeStrBuf((void *)&buf);
        uVar13 = uVar8;
    }
    return uVar13;
}

/* ====================================================================
 * PECMD_ReadModifyWrite @0x14007a070
 *   read-modify-write file loop
 * signature: longlong __fastcall PECMD_ReadModifyWrite(HANDLE param_1, undefined1 * param_2,
 *             longlong param_3, int param_4, uint param_5)
 * ==================================================================== */
int64_t PECMD_ReadModifyWrite(HANDLE param_1, uint8_t *param_2, int64_t param_3, int param_4,
                              uint32_t param_5)
{
    int bVar1;
    uint32_t uVar2;
    uint8_t *lpBuffer;
    LARGE_INTEGER LVar3;
    uint8_t *lpBuffer_00;
    int iVar4;
    int64_t lVar5;
    uint32_t uVar6;
    uint32_t nNumberOfBytesToWrite;
    int iVar7;
    DWORD local_res18[2];
    uint32_t local_res20;
    uint8_t *local_50;
    int64_t local_40;

    local_res20 = param_5 & 0xffff;
    uVar2 = local_res20 - 1;
    lVar5 = (int64_t)param_4;
    PECMD_AllocStringSlot2((void **)&local_50, 0xa0000);
    local_res18[0] = 0;
    lpBuffer = (uint8_t *)((uintptr_t)(local_50 + 0x20000) & (uintptr_t)~0x1ffffULL);
    local_40 = param_3;
    if (param_5 == 0) {
        lVar5 = 0;
        iVar7 = 0;
    }
    else {
        iVar7 = (int)((param_3 + lVar5) % (int64_t)(uint64_t)(uint16_t)param_5);
    }
    for (; 0 < param_3; param_3 = param_3 - (int64_t)uVar6) {
        iVar4 = (int)lVar5;
        if ((iVar4 == 0) && (iVar7 < param_3)) {
            bVar1 = 0;
        }
        else {
            bVar1 = 1;
        }
        uVar6 = (uint32_t)param_3;
        if (0x10000000 < param_3) {
            uVar6 = 0x10000000;
        }
        if (bVar1) {
            uVar6 = local_res20 - (uint32_t)iVar4;
            if (param_3 < (int64_t)(uint64_t)(local_res20 - (uint32_t)iVar4)) {
                uVar6 = (uint32_t)param_3;
            }
            {
                LARGE_INTEGER zero = {0};
                LVar3 = PECMD_SetFilePointer(param_1, zero, 1);
            }
            ReadFile(param_1, lpBuffer, local_res20, local_res18, 0);
            {
                uint8_t *local_48 = lpBuffer;
                memcpy(lpBuffer + iVar4, param_2, uVar6);
                PECMD_SetFilePointer(param_1, LVar3, 0);
                lpBuffer_00 = local_48;
                nNumberOfBytesToWrite = local_res20;
            }
        }
        else {
            lpBuffer_00 = param_2;
            nNumberOfBytesToWrite = uVar6;
            if (param_5 != 0) {
                uVar6 = uVar6 & ~uVar2;
                nNumberOfBytesToWrite = uVar6;
            }
        }
        local_res18[0] = 0;
        WriteFile(param_1, lpBuffer_00, nNumberOfBytesToWrite, local_res18, 0);
        lVar5 = 0;
        param_2 = param_2 + uVar6;
    }
    PECMD_FreeStrBuf((void *)&local_50);
    return local_40;
}

/* ====================================================================
 * PECMD_ShowShutdownUI @0x14007c0a0
 *   spawn thread + find window + shutdown UI
 * signature: ulonglong * __fastcall PECMD_ShowShutdownUI(int param_1, LPCWSTR param_2,
 *             LPCWSTR param_3, undefined4 param_4, int param_5, byte param_6)
 * ==================================================================== */
uint64_t *PECMD_ShowShutdownUI(int param_1, LPCWSTR param_2, LPCWSTR param_3, uint32_t param_4,
                               int param_5, uint8_t param_6)
{
    int64_t lVar1;
    int iVar2;
    int iVar3;
    DWORD DVar4;
    DWORD DVar5;
    HANDLE pvVar6;
    HWND hWnd;
    uint64_t *puVar7;
    uint64_t *lpParameter;
    uint64_t *local_60;
    uint64_t *local_48;
    LPCWSTR lpWindowName;
    LPCWSTR lpWindowName_00;
    LPCWSTR pwVar8;
    LPCWSTR local_58;
    int bVar9;
    uint32_t local_64;
    HANDLE local_50;

    iVar2 = lstrlenW(param_2);
    lVar1 = (int64_t)iVar2 * 2 + 2;
    iVar3 = lstrlenW(param_3);
    PECMD_AllocStringSlot2((void **)&local_60, (int64_t)iVar3 * 2 + 0x42 + lVar1);
    lpParameter = local_60;
    local_48 = local_60;
    PECMD_ZeroLenBuf(local_60);
    local_50 = CreateThread(0, 0x20000, (void *)PECMD_TaskShowMessage, lpParameter, 0x10004,
                            (DWORD *)&g_lastThreadId);
    if (local_50 == (HANDLE)0) {
        DVar4 = GetLastError();
        puVar7 = (uint64_t *)(uint64_t)DVar4;
        if (DVar4 == 0) {
            puVar7 = (uint64_t *)1;
        }
    }
    else {
        *(uint32_t *)(lpParameter + 4) = param_4;
        lpParameter[2] = (uint64_t)param_2;
        *(int *)((char *)lpParameter + 0x24) = param_5;
        lpParameter[3] = (uint64_t)param_3;
        *lpParameter = 2;
        *(int *)(lpParameter + 1) = param_1;
        *(uint8_t *)(lpParameter + 5) = param_6 & 0xf;
        lpParameter[3] = (uint64_t)((char *)lpParameter + (int64_t)iVar2 * 2 + 0x42);
        lpParameter[2] = (uint64_t)(lpParameter + 8);
        *(uint32_t *)((char *)lpParameter + 0x34) = 1;
        memcpy((uint8_t *)(lpParameter + 8), (uint8_t *)param_2, (int)lVar1);
        memcpy((uint8_t *)lpParameter[3], (uint8_t *)param_3, ((int64_t)iVar3 * 2) + 2);
        pvVar6 = CreateEventW(0, 1, 0, 0);
        lpParameter[7] = (uint64_t)pvVar6;
        ResumeThread(local_50);
        WaitForSingleObject(pvVar6, 0xffffffff);
        CloseHandle(pvVar6);
        GetTickCount();

        pwVar8 = WSTR("#32770");
        hWnd = (HWND)0;
        lpWindowName = (LPCWSTR)0;
        local_58 = WSTR("#32770");
        lpWindowName_00 = param_3;
        if (param_1 == 2) {
            pwVar8 = (LPCWSTR)0;
            lpWindowName_00 = WSTR("\x63d0\x793a\x4fe1\x606f\x3000\x3000\x3000\x3000\x3000\x3000"
                                   "\x3000\x3000\x3000\x3000");
            lpWindowName = WSTR("\x7cfb\x7edf\x5173\x673a");
            local_58 = (LPCWSTR)0;
        }

        local_64 = 0;
        bVar9 = (g_privFlags & 4) == 0;
        if (bVar9) {
            g_privFlags = (uint16_t)(g_privFlags | 4);
            PECMD_EnableTokenPrivilege(WSTR("SeDebugPrivilege"), 2, 0);
        }
        iVar2 = 10;
        do {
            iVar2 = iVar2 - 1;
            if ((iVar2 < 0) || (hWnd = FindWindowW(pwVar8, lpWindowName_00), hWnd != (HWND)0) ||
                (lpWindowName != (LPCWSTR)0 &&
                 (hWnd = FindWindowW(pwVar8, lpWindowName), hWnd != (HWND)0))) {
                break;
            }
            Sleep(10);
        } while (hWnd == (HWND)0);
        iVar2 = 10;
        if (hWnd == (HWND)0) {
            do {
                iVar2 = iVar2 - 1;
                if (iVar2 < 0)
                    break;
                hWnd = FindWindowW((LPCWSTR)0, lpWindowName_00);
                if ((hWnd != (HWND)0) ||
                    (lpWindowName != (LPCWSTR)0 &&
                     (hWnd = FindWindowW((LPCWSTR)0, lpWindowName), hWnd != (HWND)0))) {
                    goto L14007c326;
                }
                Sleep(1);
            } while (hWnd == (HWND)0);
            if (hWnd == (HWND)0) {
                hWnd = (HWND)PECMD_FindTargetWindow(lpWindowName_00, 0, 0, (HWND)0, 0, 0, 0, 0);
            }
        }
    L14007c326:
        pwVar8 = local_58;
        iVar3 = 0x5a;
        iVar2 = 0x5a;
        if (hWnd == (HWND)0) {
            while (1) {
                iVar2 = iVar2 - 1;
                if (!(-1 < iVar2))
                    break;
                lpParameter = local_48;
                hWnd = FindWindowW(pwVar8, lpWindowName_00);
                lpParameter = local_48;
                if (hWnd != (HWND)0)
                    break;
                if ((lpWindowName != (LPCWSTR)0 && (hWnd = FindWindowW(pwVar8, lpWindowName),
                                                    lpParameter = local_48, hWnd != (HWND)0)) ||
                    (Sleep(10), lpParameter = local_48, hWnd != (HWND)0)) {
                    break;
                }
            }
            while (1) {
                if (!(hWnd == (HWND)0))
                    break;
                iVar3 = iVar3 - 1;
                if (!(-1 < iVar3))
                    break;
                hWnd = FindWindowW((LPCWSTR)0, lpWindowName_00);
                if (hWnd != (HWND)0)
                    break;
                if (!(lpWindowName == (LPCWSTR)0 ||
                      (hWnd = FindWindowW((LPCWSTR)0, lpWindowName), hWnd == (HWND)0))) {
                    break;
                }
                Sleep(1);
            }
        }
        puVar7 = (uint64_t *)0;
        if ((param_1 == 2) && (hWnd != (HWND)0)) {
            SetWindowTextW(hWnd, param_3);
        }
        DVar4 = GetTickCount();
        if (((param_1 == 1) && (hWnd != (HWND)0)) && (g_pSharedMap != (void *)0)) {
            *(HWND *)g_pSharedMap = hWnd;
        }
        if (((param_6 & 0xc0) != 0x80) && ((param_1 != 1) || (hWnd != (HWND)0))) {
            if (param_1 == 2) {
                if (param_5 < 1) {
                    if (bVar9) {
                        g_privFlags = (uint16_t)(g_privFlags & 0xfffb);
                        PECMD_EnableTokenPrivilege(WSTR("SeDebugPrivilege"), 4, 0);
                    }
                    goto L14007c5b0;
                }
                do {
                    if (g_flagA24F < 1)
                        break;
                    DVar5 = GetTickCount();
                } while ((int)(DVar5 - DVar4) < param_5);
                AbortSystemShutdownW((LPWSTR)0);
                lpParameter = local_60;
            }
            else {
                while (g_flagA24F > 0) {
                    if ((0 < param_5) &&
                        (DVar5 = GetTickCount(), param_5 <= (int)(DVar5 - DVar4))) {
                        local_64 = param_6 & 0xf;
                        goto L14007c4e0;
                    }
                    hWnd = FindWindowW(local_58, param_3);
                    if (hWnd == (HWND)0)
                        break;
                    Sleep(10);
                }
                local_64 = *(uint32_t *)((char *)lpParameter + 0x34);
            L14007c4e0:
                lpParameter = local_60;
                if (g_pSharedMap != (void *)0) {
                    *(HWND *)g_pSharedMap = (HWND)0;
                }
            }
        }
        EnterCriticalSection(g_csInit);
        *lpParameter = *lpParameter - 1;
        *(uint32_t *)(lpParameter + 6) = 0;
        local_60 = lpParameter;
        if (0 < (int64_t)*lpParameter) {
            local_60 = puVar7;
        }
        LeaveCriticalSection(g_csInit);
        if (hWnd != (HWND)0) {
            PostMessageW(hWnd, 0x10, 0, 0);
            SendMessageW(hWnd, 0x111, 2, 0);
            SendMessageW(hWnd, 0x111, 7, 0);
            DestroyWindow(hWnd);
        }
        pvVar6 = local_50;
        TerminateThread(local_50, 0);
        CloseHandle(pvVar6);
        puVar7 = (uint64_t *)(uint64_t)local_64;
        if (bVar9) {
            g_privFlags = (uint16_t)(g_privFlags & 0xfffb);
            PECMD_EnableTokenPrivilege(WSTR("SeDebugPrivilege"), 4, 0);
        }
    }
L14007c5b0:
    PECMD_FreeStrBuf((void *)&local_60);
    return puVar7;
}

/* ====================================================================
 * PECMD_SaveBitmap @0x14007d450
 *   save bitmap to file/clipboard
 * signature: DWORD __fastcall PECMD_SaveBitmap(HBITMAP param_1, LPCWSTR param_2,
 *             int param_3, LPCWSTR param_4)
 * ==================================================================== */
DWORD PECMD_SaveBitmap(HBITMAP param_1, LPCWSTR param_2, int param_3, LPCWSTR param_4)
{
    LPCWSTR lpString1;
    uint8_t *puVar1;
    int iVar2;
    DWORD DVar3;
    int iVar4;
    uint32_t uVar5;
    LPWSTR pWVar6;
    HDC hdc;
    HGLOBAL hMem;
    BITMAPINFO *lpbmi;
    HPALETTE hPal;
    HANDLE hFile;
    wchar_t *lpStart;
    LPCWSTR lpFileName;
    HPALETTE pHVar7;
    HPALETTE local_a0;
    DWORD local_a8[2];
    BITMAPFILEHEADER bfh;
    BITMAPINFO header;
    BITMAP bm;

    if (*param_2 == L':') {
        lpStart = (wchar_t *)(param_2 + 1);
        pWVar6 = StrChrW((LPCWSTR)lpStart, L':');
        lpFileName = (LPCWSTR)lpStart;
        if (pWVar6 != (LPWSTR)0) {
            *lpStart = L'\0';
            param_2 = param_2 + 2;
        L14007d57c:
            lpFileName = param_2;
            if (lpStart != (wchar_t *)0)
                goto L14007d581;
        }
    }
    else {
        StrRChrW(param_2, (LPCWSTR)0, L'\\');
        pWVar6 = StrRChrW(param_2, (LPCWSTR)0, L'.');
        lpString1 = pWVar6 + 1;
        lpFileName = param_2;
        if (pWVar6 != (LPWSTR)0) {
            iVar2 = lstrcmpiW(lpString1, WSTR("png"));
            if (iVar2 == 0) {
                lpStart = (wchar_t *)WSTR("image/png");
            }
            else {
                iVar2 = lstrcmpiW(lpString1, WSTR("jpg"));
                if ((iVar2 == 0) || (iVar2 = lstrcmpiW(lpString1, WSTR("jpeg")), iVar2 == 0)) {
                    lpStart = (wchar_t *)WSTR("image/jpeg");
                    goto L14007d57c;
                }
                iVar2 = lstrcmpiW(lpString1, WSTR("gif"));
                if (iVar2 == 0) {
                    lpStart = (wchar_t *)WSTR("image/gif");
                }
                else {
                    iVar2 = lstrcmpiW(lpString1, WSTR("tiff"));
                    if (iVar2 == 0) {
                        lpStart = (wchar_t *)WSTR("image/tiff");
                    }
                    else {
                        iVar2 = lstrcmpiW(lpString1, WSTR("bmp"));
                        if (iVar2 != 0)
                            goto L14007d5ab;
                        lpStart = (wchar_t *)WSTR("image/bmp");
                    }
                }
            }
        L14007d581:
            iVar2 = StrCmpIW((LPCWSTR)lpStart, WSTR("bmp"));
            lpFileName = param_2;
            if (iVar2 != 0) {
                DVar3 =
                    PECMD_EncodeImageToStream((int64_t)param_1, param_2, (LPCWSTR)lpStart, param_3);
                return DVar3;
            }
        }
    }
L14007d5ab:
    hPal = (HPALETTE)0;
    if (param_3 != 0) {
        return 1;
    }
    hdc = CreateDCW(param_4, (LPCWSTR)0, (LPCWSTR)0, (void *)0);
    iVar2 = GetDeviceCaps(hdc, 0xe);
    iVar4 = GetDeviceCaps(hdc, 0xc);
    iVar2 = iVar2 * iVar4;
    DeleteDC(hdc);
    if (iVar2 < 2) {
        iVar4 = 1;
    }
    else {
        iVar4 = 4;
        if ((4 < iVar2) && (iVar4 = 8, 8 < iVar2)) {
            iVar4 = 0x18;
            pHVar7 = hPal;
            if (0x18 < iVar2) {
                iVar4 = 0x20;
            }
            goto L14007d646;
        }
    }
    pHVar7 = (HPALETTE)(uint64_t)(uint32_t)((1 << (int8_t)iVar4) << 2);
L14007d646:
    GetObjectW(param_1, 0x20, &bm);
    memset(&header, 0, sizeof(header));
    header.bmiHeader.biSize = 0x28;
    header.bmiHeader.biWidth = bm.bmWidth;
    header.bmiHeader.biHeight = bm.bmHeight;
    header.bmiHeader.biPlanes = 1;
    header.bmiHeader.biBitCount = (uint16_t)iVar4;
    iVar2 = iVar4 * (int)bm.bmWidth + 0x1f;
    uVar5 =
        (uint32_t)(((iVar2 + ((iVar2 >> 31) & 0x1fU)) >> 5) * (int)bm.bmHeight * 4 + (int)pHVar7);
    hMem = GlobalAlloc(0x42, (uint64_t)uVar5 + 0x28);
    lpbmi = (BITMAPINFO *)GlobalLock(hMem);
    memcpy(lpbmi, &header, 0x28);
    local_a0 = GetStockObject(0xf);
    if (local_a0 != (HPALETTE)0) {
        hdc = GetDC((HWND)0);
        hPal = SelectPalette(hdc, local_a0, 0);
        RealizePalette(hdc);
    }
    GetDIBits(hdc, param_1, 0, (uint32_t)bm.bmHeight,
              (void *)((char *)lpbmi->bmiColors + (int)pHVar7), lpbmi, 0);
    if (hPal != (HPALETTE)0) {
        SelectPalette(hdc, hPal, 1);
        RealizePalette(hdc);
        ReleaseDC((HWND)0, hdc);
    }
    bfh.bfType = 0x4d42;
    bfh.bfSize = uVar5 + 0x36;
    bfh.bfReserved1 = 0;
    bfh.bfReserved2 = 0;
    bfh.bfOffBits = (int)pHVar7 + 0x36;
    if (lpFileName == (LPCWSTR)0) {
        g_imgBufLen = (int64_t)(int)(uVar5 + 0x36);
        PECMD_GrowByteBuffer((void **)&g_pImageBuf, g_imgBufLen + 8);
        puVar1 = g_pImageBuf;
        memcpy(g_pImageBuf, &bfh, 0xe);
        memcpy(puVar1 + 0xe, lpbmi, uVar5 + 0x28);
    }
    else {
        hFile = CreateFileW(lpFileName, 0x40000000, 0, 0, 2, 0x8000080, (HANDLE)0);
        if (hFile == (HANDLE)-1) {
            DVar3 = GetLastError();
            if (DVar3 != 0) {
                return DVar3;
            }
            return 1;
        }
        WriteFile(hFile, &bfh, 0xe, local_a8, 0);
        WriteFile(hFile, lpbmi, uVar5 + 0x28, local_a8, 0);
        CloseHandle(hFile);
    }
    GlobalUnlock(hMem);
    GlobalFree(hMem);
    return 0;
}

/* ====================================================================
 * PECMD_LoadIconFrames @0x14007db70
 *   icon/resource loading
 * signature: uint __fastcall PECMD_LoadIconFrames(longlong * param_1,
 *             undefined8 * param_2, int * param_3)
 * ==================================================================== */
uint32_t PECMD_LoadIconFrames(int64_t *param_1, uint64_t *param_2, int *param_3)
{
    HMODULE hModule;
    DWORD DVar1;
    HICON pHVar2;
    HRSRC hResInfo;
    uint8_t *puVar3;
    int64_t lVar4;
    uint32_t *puVar5;
    uint32_t *puVar6;
    uint32_t uVar7;
    SIZE_T SVar8;
    int64_t *plVar9;
    uint64_t uVar10;
    uint64_t uVar13;
    uint64_t uVar14;
    uint64_t uVar15;
    uint32_t local_res20[2];
    uint32_t local_48[2];
    uint64_t local_38[2];
    uint32_t iVar11;
    uint64_t uVar12;
    uint64_t local_40;

    uVar12 = 0;
    iVar11 = 0;
    if ((g_pGdipLoadImageFromStream == 0) &&
        (pHVar2 = PECMD_LoadIcon((LPCWSTR)0, (uint64_t *)0), pHVar2 == (HICON)0)) {
        return 0xffffffff;
    }
    puVar5 = (uint32_t *)(param_1 + 3);
    *(uint32_t *)((char *)param_1 + 0x1c) = 0;
    *puVar5 = 0;
    if (param_3 != (int *)0) {
        iVar11 = (uint32_t)*param_3;
    }
    *(uint32_t *)(param_1 + 5) = 0;
    if (param_2 == (uint64_t *)0) {
        param_2 = (uint64_t *)(uintptr_t)0x64;
    }
    *(uint32_t *)((char *)param_1 + 0x2c) = 0;
    *(uint32_t *)(param_1 + 6) = 0;
    *(uint32_t *)((char *)param_1 + 0x34) = 0;
    hModule = g_hInst;
    if ((int64_t)param_2 < 0x10000) {
        hResInfo = FindResourceW(g_hInst, (LPCWSTR)((uint64_t)param_2 & 0xffff), WSTR("IMAGE"));
        if (hResInfo == (HRSRC)0) {
            return 0xffffffff;
        }
        DVar1 = SizeofResource(hModule, hResInfo);
        SVar8 = (SIZE_T)DVar1;
        if (SVar8 < 8) {
            return 0xffffffff;
        }
        puVar3 = (uint8_t *)LoadResource(hModule, hResInfo);
    }
    else {
        if (iVar11 == 0) {
            ((void (*)(LPCWSTR, uint64_t *))g_pGdipLoadImageFromFile)((LPCWSTR)param_2,
                                                                      (uint64_t *)(param_1 + 7));
            goto L14007dc6e;
        }
        puVar3 = (uint8_t *)*param_2;
        SVar8 = (SIZE_T)iVar11;
    }
    if (puVar3 == (uint8_t *)0) {
        return 0xffffffff;
    }
    lVar4 = PECMD_LoadImageStream(puVar3, SVar8, (uint64_t *)(param_1 + 8));
    param_1[7] = lVar4;
L14007dc6e:
    plVar9 = param_1 + 7;
    if (*plVar9 != 0) {
        local_48[0] = 0;
        ((void (*)(uint64_t, uint32_t *))g_pGdipImageGetFrameDimensionsCount)((uint64_t)*plVar9,
                                                                              local_48);
        PECMD_AllocStringSlot2((void **)&local_40, (uint64_t)local_48[0] << 4);
        ((void (*)(uint64_t, uint64_t, uint32_t))g_pGdipImageGetFrameDimensionsList)(
            (uint64_t)*plVar9, local_40, local_48[0]);
        ((void (*)(uint64_t, uint64_t, uint32_t *))g_pGdipImageGetFrameCount)((uint64_t)*plVar9,
                                                                              local_40, puVar5);
        if (*puVar5 < 2) {
            uVar7 = 0xffffffff;
        }
        else {
            local_res20[0] = 0;
            ((void (*)(uint64_t, uint32_t, uint32_t *))g_pGdipGetPropertyItemSize)(
                (uint64_t)*plVar9, 0x5100, local_res20);
            PECMD_AllocStringSlot2((void **)local_38, (int64_t)(int)local_res20[0]);
            ((void (*)(uint64_t, uint32_t, uint32_t, uint64_t))g_pGdipGetPropertyItem)(
                (uint64_t)*plVar9, 0x5100, local_res20[0], local_38[0]);
            uVar15 = ((uint64_t)local_res20[0] - 0x18) >> 2;
            PECMD_VectorSlotPtr((int64_t)(int)*puVar5, param_1, param_1 + 1, 4);
            uVar10 = uVar12;
            if ((int)uVar15 != 0) {
                uVar10 = uVar15 & 0xffffffff;
                uVar15 = uVar15 & 0xffffffff;
                uVar13 = uVar12;
                uVar14 = uVar12;
                do {
                    uVar7 = (uint32_t)(*(int *)(uVar14 + *(uint64_t *)(local_38[0] + 0x10)) * 10);
                    uVar12 = (uint64_t)uVar7;
                    puVar5 = (uint32_t *)PECMD_VectorSlotPtr(uVar13, param_1, param_1 + 1, 4);
                    uVar13 = uVar13 + 1;
                    uVar14 = uVar14 + 4;
                    uVar15 = uVar15 - 1;
                    *puVar5 = uVar7;
                } while (uVar15 != 0);
            }
            while (1) {
                if (*(uint32_t *)(param_1 + 3) <= (uint32_t)uVar10)
                    break;
                puVar6 = (uint32_t *)PECMD_VectorSlotPtr(uVar10, param_1, param_1 + 1, 4);
                uVar10 = (uint64_t)((uint32_t)uVar10 + 1);
                *puVar6 = (int)uVar12;
            }
            uVar7 = *(uint32_t *)(param_1 + 3);
            PECMD_FreeStrBuf((void *)local_38);
        }
        PECMD_FreeStrBuf((void *)&local_40);
        return uVar7;
    }
    return 0xfffffffe;
}
