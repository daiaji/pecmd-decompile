/* ====================================================================
 * core_b8m.c — B8 中批 4 (1400e0000-1400fffff), 前半
 *
 * 本批新实现函数全部使用人类可读 PECMD_ 名称，原始地址保留在 @0x 注释。
 *
 * 来源: PECMD原始.EXE (x64, ImageBase=0x140000000)
 *   通用 GDI 虚表消息分发 FUN_1400E5B0C @0x1400e5b0c
 *
 * 约定:
 *   - 新实现函数使用 PECMD_ 可读名；未实现依赖仍 extern FUN_ + TODO(verify)
 *   - 全局数据符号先用 extern 声明; 定义统一放在 core_globals.c
 * ==================================================================== */
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "pecmd_defs.h"
extern WCHAR **FUN_14005B154(WCHAR **pp); /* @0x14005b154 */

/* ---- CryptoAPI 局部声明 (win32_stub 未提供) ---- */
typedef uintptr_t HCRYPTPROV;
typedef uintptr_t HCRYPTHASH;
typedef uintptr_t HCRYPTKEY;
typedef uint32_t ALG_ID;
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

/* ---- 已实现公共工具 (pecmd_defs.h / core_*.c) ---- */
extern void PECMD_ValidateCtlPaint(HWND hwnd);               /* @0x1400e5ae4 */
extern int64_t PECMD_CtlForwardSetBlock(int64_t *obj, uint64_t *args); /* @0x1400e5aac */
extern void FUN_1400EF91C(int64_t obj, uint32_t param2, uint64_t param3); /* @0x1400ef91c */
extern void PECMD_FrameRegion(uint64_t *obj, HDC hdc, HRGN rgn,
                          uint32_t flags, int thickness); /* @0x14005d600 */

/* ---- 字符串/脚本工具 (extern) ---- */
extern void PECMD_AppendFmtValue(void *script, uint64_t value, LPCWSTR key, LPCWSTR fmt); /* @0x1400668ec */

/* ---- 未实现依赖 (extern + TODO(verify)) ---- */
extern DWORD PECMD_RegSetValueWithOpen(HKEY root, LPCWSTR sub, LPCWSTR name, DWORD type,
                           BYTE *data, DWORD size);             /* @0x14005c5a0 */
extern void PECMD_TlsLogWrite(uint64_t ctx, LPCWSTR fmt, uint64_t a, uint64_t b);
extern void FUN_140025f10(int64_t ctx, LPCWSTR msg, uint32_t code, void *p4,
                          void *p5, int64_t *p6);
extern void PECMD_AllocStrSlot(void *ps);
extern void PECMD_SplitTokenTrimWs(int64_t *src, int64_t *dst, int16_t delim);
extern void FUN_14007BF44(int64_t *ctx, WCHAR *name, void *out, int mode,
                          uint8_t flag);
extern void PECMD_FormatSetVar(int64_t *script, uint64_t value, LPCWSTR key,
                          LPCWSTR fmt);                  /* @0x140066930 */
extern int64_t PECMD_ProcessScriptBlock(uint64_t script, uint64_t cmd, void *p3, void *p4,
                             void *p5);
extern int32_t g_msgLockCount;                                /* 消息锁计数 */
extern int PECMD_SelectTabPageIndex(int64_t obj, int index);
extern void PECMD_ControlOnMouseMove(int64_t *obj, int param2, uint64_t param3);
extern void PECMD_OnTabSelChange(int64_t obj);
extern HWND FUN_1400E5788(HWND hwnd);             /* @0x1400e5788 */
extern void PECMD_LayoutTabPageArea(int64_t obj, char track); /* @0x1400ec0f0 */
extern int64_t FUN_140063B00(int64_t idx, int64_t *arr, int64_t *cap,
                               uint32_t esize);               /* @0x140063b00 */
extern void FUN_1400E5730(HWND hwnd, int64_t *out);
extern LRESULT FUN_1400E5890(int64_t obj);             /* @0x1400e5890 */
extern void PECMD_DrawScaledBarFill(int64_t obj, HDC hdc, RECT *rc, COLORREF color,
                                 int edge);                   /* @0x1400f0df4 */
extern void PECMD_DrawVertCenteredText(HDC hdc, LPCWSTR text, int length, RECT *rect,
                                   uint32_t flags);           /* @0x1400ef08c */
extern void PECMD_DrawIconInRect(int64_t obj, HDC hdc, RECT *rect, int centerX); /* @0x1400eff58 */
extern void PECMD_SelectObjectSlot_b028(uint64_t *slot, HDC hdc, HGDIOBJ obj);
extern void PECMD_RestoreAndDeleteObject(uint64_t *slot);
extern float g_alphaThreshold;                                /* DAT_14012f6bc */
extern int (*g_pGdipGetImageWidth)();
extern int (*g_pGdipGetImageHeight)();
extern int (*g_pGdipCreateFromHDC)();
extern int (*g_pGdipDrawImageRectI)(void *, void *, int, int, int64_t, int);
extern int (*g_pGdipDeleteGraphics)();
extern int64_t PECMD_AlignUpSize(int64_t value, uint32_t align); /* @0x1400e4064 */
extern int32_t FUN_14005C7C4(const char *a, const WCHAR *w); /* @0x14005c7c4 */
extern int64_t FUN_14007A224(void *script, WCHAR *text, WCHAR **out, int c, int d);
extern void PECMD_ParseNumSkipChar_de4c(int64_t *pp, double *out);  /* @0x14007de4c */
extern uint32_t PECMD_ParseIntOrColor(uint64_t *pp, uint64_t *out); /* @0x140066850 */
extern void PECMD_ParseNumSkipChar_0224(int64_t *pp, int *out);    /* @0x140070224 */
extern void FUN_1400F4040(int64_t obj, float value);   /* @0x1400f4040 */
extern void FUN_1400F4064(int64_t obj, int height, int mode); /* @0x1400f4064 */
extern int PECMD_DpiConvert(double value);                     /* @0x1400628b4 */
extern HFONT FUN_1400B1F34(int *lf, double *size, LPCWSTR name); /* @0x1400b1f34 */
extern HFONT FUN_1400B89DC(HANDLE obj, double *size, LPCWSTR name); /* @0x1400b89dc */
extern uint64_t PECMD_FindTreeItemByPath(int64_t obj, WCHAR *path, uint64_t *out); /* @0x1400ff414 */
extern void PECMD_CaptureParentBackground(int64_t obj);        /* @0x1400fd5e8 */
extern void PECMD_SelectObjectSlot_b054(int64_t ctx, HDC hdc, HGDIOBJ obj); /* @0x14005b054 */
extern void FUN_1400F429C(WCHAR **pp, WCHAR ch);      /* @0x1400f429c */
extern void PECMD_StrBldCopyWideN(WCHAR **pname, LPCWSTR src, int64_t len); /* @0x1400702d4 */
extern void PECMD_ParseNumSkipChar_01f8(int64_t *pp, int *out);      /* @0x1400701f8 */
extern void PECMD_HeapFreeWithHeader(void *ps);
extern bool PECMD_ParseUIntValue(WCHAR **pp, int *out);               /* @0x140067d20 */
extern void PECMD_ListSubItemHitTest(int64_t obj, int *out_index, int *out_flag); /* @0x1400f3308 */
extern void PECMD_TableSetHoverIdx(int64_t obj, int current); /* @0x1400f51d8 */
extern uint64_t PECMD_ListGetItemTextData(int64_t obj, int *rect, int msgParam); /* @0x1400f2f58 */
extern COLORREF PECMD_FillRectColor(HDC hdc, RECT *rc, COLORREF color); /* @0x1400e68e0 */
extern void PECMD_FormatOutput(int64_t obj);                     /* @0x14009c6dc */
extern void PECMD_SetWindowTheme(uint64_t hwnd);           /* @0x140066054 */
extern BOOL GetScrollRange(HWND hWnd, int nBar, int *lpMinPos, int *lpMaxPos);
extern BOOL SetScrollRange(HWND hWnd, int nBar, int nMinPos, int nMaxPos, BOOL bRedraw);
extern uint8_t g_u8CCB1;                                  /* MAIN_DBG 日志标志 */
extern void FUN_1400633A8(void **ps, int64_t len);             /* @0x1400633a8 */
extern void PECMD_SafeVFormatW(WCHAR *dest, size_t cchDest, LPCWSTR fmt, uint64_t arg); /* @0x1400e6960 */
extern WCHAR **FUN_14007034C(WCHAR **ps, LPCWSTR src);     /* @0x14007034c */
extern void *FUN_14007DE70(LPCWSTR *a, LPCWSTR *out, LPCWSTR src); /* @0x14007de70 */
extern int64_t *PECMD_ReplaceStringSlot(int64_t *ps, uint64_t *src); /* @0x140070398 */
extern double g_dbl20b28;
extern double g_dbl2b380;
extern double g_dbl2b378;
extern double g_dbl2b370;
extern double g_dbl2b368;
extern void PECMD_EnumDropFiles(void *hDrop, int64_t *a, int64_t *b);
extern void FUN_14007D0AC(int64_t **ctx, LPCWSTR key, LPCWSTR value); /* @0x14007d0ac */
extern void PECMD_AdjustEditScrollRect(int64_t obj);
extern BOOL SystemParametersInfoW(UINT uiAction, UINT uiParam, void *pvParam, UINT fWinIni);
extern void *PECMD_HeapRealloc(void *ptr, size_t size);      /* @0x140063118 */
extern uint8_t DefWindowProcW_exref;
extern uint64_t FUN_1400F3554(int64_t obj, LPARAM param2); /* @0x1400f3554 */
extern void *PECMD_GrowByteBuffer(void **ps, int64_t len);      /* @0x140063424 */
extern int64_t PECMD_CalcPeImageSize(uint64_t file, uint32_t fileSize,
                                     int64_t peHeader, int64_t sectionTable); /* @0x1400e4078 */
extern BOOL VirtualProtectEx(HANDLE hProcess, LPVOID lpAddress, size_t dwSize,
                              DWORD flNewProtect, DWORD *lpflOldProtect);
extern uint32_t (*g_pfnVirtualAllocEx)(void); /* DAT_14013e258 VirtualAllocEx */
extern uint32_t PECMD_CreateProcReadImageBase(LPWSTR cmd, int64_t ctxOff, int64_t *outPtr,
                                              int64_t *outSize, BOOL inherit, uint32_t flags,
                                              LPVOID env, LPCWSTR cwd, STARTUPINFOW *si,
                                              PROCESS_INFORMATION *pi, LPCWSTR param11); /* @0x1400e4324 */
extern BOOL PECMD_HasVirtualAllocEx(void);     /* @0x1400e411c */
extern bool PECMD_ZwUnmapViewOfSection(HANDLE process, void *baseAddress); /* @0x1400e4228 */
extern BOOL FUN_1400E412C(int64_t obj);   /* @0x1400e412c */
extern void PECMD_PeApplyRelocations(int64_t obj, void *src, void *dst); /* @0x1400e4160 */
extern uint32_t (*g_pfnGetThreadCtx)(HANDLE, int64_t);      /* DAT_14013e248 */
extern uint32_t (*g_pfnSetThreadCtx)(HANDLE, int64_t);      /* DAT_14013e250 */
extern uint64_t g_remoteTebBase;                            /* DAT_14013e2f9 */
extern uint64_t g_ptrA84C;                                  /* DAT_14013a84c */
extern uint8_t g_objMode;                                   /* DAT_14013a84d */
extern uint64_t g_peCtxSize;                                /* DAT_14013a850 */
extern uint64_t g_peCtxOffB;                                /* DAT_14013e2f8 */
extern uint8_t g_peByteA84E;                                /* DAT_14013a84e */
extern uint16_t g_peWordA852;                               /* DAT_14013a852 */
extern uint8_t g_peByteA84F;                                /* DAT_14013a84f */
extern uint16_t g_peWordA853;                               /* DAT_14013a853 */
extern void FUN_1400E7664(LPCWSTR entry);          /* @0x1400e7664 */
extern int64_t FUN_1400E7840(int64_t *out, int flag); /* @0x1400e7840 */
extern void PECMD_RasDialStatusCallback(uint64_t unused, int mode, int value); /* @0x1400e4edc */
extern HANDLE g_hSyncEvtA;               /* DAT_14013e1f8 */
extern HANDLE g_hSyncEvtB;               /* DAT_14013e200 */
extern uint32_t g_syncEvtVal;            /* DAT_14013e208 */
extern uint64_t g_rasState;              /* DAT_14013e2a0 RAS 等待状态 */
extern uint64_t *g_pRasEntryBuf;         /* DAT_14013e300 RAS 条目缓冲指针 */
extern void *g_syncState;                /* DAT_14013e298 同步状态指针 */
extern uint32_t (*g_pfnRasDial)(void);   /* DAT_14013e268 RasDialW */
extern uint32_t (*g_pfnRasGetErrorString)(void); /* DAT_14013e280 RasGetErrorStringW */
extern WCHAR g_szEmpty[];            /* g_szEmpty 空串 */
extern uint8_t g_b12ae98[];
extern uint8_t g_b12acc0[];

/* ---- 全局 (core_globals.c) ---- */
extern HWND g_hActiveDevWnd;        /* DAT_14013e400 当前设备窗口 */

/* ========== FUN_1400E5B0C @0x1400e5b0c ==========
 * 通用 GDI/控件对象虚表消息分发：把消息按编号转发到对象虚表不同槽位；
 * 未命中槽位时回退到虚表 +0x08 的默认处理。
 * 注：原反编译参数类型为 HDC，实为对象指针；Ghidra 指针元素按 4 字节
 * 计，故 N 换算为字节偏移 N*4。
 */
int64_t FUN_1400E5B0C(int64_t obj, uint64_t msg, int64_t wParam, int64_t *lParam)
{
    uint8_t *b = (uint8_t *)(uintptr_t)obj;
    void **vt = *(void ***)(uintptr_t)obj;
    RECT rc;
    HBRUSH brush;
    int64_t *round;
    HGDIOBJ oldObj;
    HRGN rgn, tmpRgn;
    HWND hwnd;
    HDC hdc;
    COLORREF color;
    int result;
    uint64_t vtSelf;

    if (msg < 0x203) {
        if (msg == 0x202) {
            rc.left = (int16_t)(uint16_t)(uint64_t)lParam;
            rc.top = (int16_t)(uint16_t)((uint64_t)lParam >> 16);
            ((int64_t (*)(int64_t, uint32_t, uint64_t))vt[0x58 / 8])(
                obj, (uint32_t)wParam,
                (uint64_t)(uint32_t)((uint32_t)(uint16_t)rc.top << 16 |
                                     (uint32_t)(uint16_t)rc.left));
            return 0;
        }
        if (msg < 0x2d) {
            if (msg == 0x2c) {
                ((int64_t (*)(int64_t, int64_t, int64_t *))vt[0xc0 / 8])(
                    obj, wParam, lParam);
                return 0;
            }
            if (msg == 5) {
                ((int64_t (*)(int64_t, uint32_t, uint32_t, uint32_t))vt[0x78 / 8])(
                    obj, (uint32_t)wParam,
                    (uint32_t)(uint64_t)lParam & 0xffff,
                    (uint32_t)((uint64_t)lParam >> 16) & 0xffff);
                return 0;
            }
            if (msg == 0xf) {
                if ((*(uint8_t *)(b + 0x78) & 2) != 0) {
                    PECMD_ValidateCtlPaint(*(HWND *)(b + OBJ_HWND));
                    return 0;
                }
                ((int64_t (*)(int64_t, int64_t, int64_t *))vt[0x28 / 8])(
                    obj, wParam, lParam);
                return 0;
            }
            if (msg == 0x10) {
                ((void (*)(void))vt[0x38 / 8])();
                return 0;
            }
            if (msg == 0x14) {
                round = *(int64_t **)(b + 0xc0);
                rc.left = 0;
                rc.top = 0;
                rc.right = 0;
                rc.bottom = 0;
                if (round != NULL) {
                    GetWindowRect(*(HWND *)(b + OBJ_HWND), &rc);
                    rc.right -= rc.left;
                    rc.bottom -= rc.top;
                    rc.left = 0;
                    rc.top = 0;
                    brush = *(HBRUSH *)((uint8_t *)round + 0x10);
                    if (brush != 0)
                        FillRect((HDC)wParam, &rc, brush);
                }
                result = (int)((int64_t (*)(int64_t, int64_t))vt[0x40 / 8])(obj, wParam);
                round = *(int64_t **)(b + 0xc0);
                if (round != NULL) {
                    oldObj = (HGDIOBJ)round[1];
                    if (oldObj == 0 ||
                        rc.right != *(int *)((uint8_t *)round + 0x20) ||
                        rc.bottom != *(int *)((uint8_t *)round + 0x24)) {
                        *(int *)((uint8_t *)round + 0x20) = rc.right;
                        *(int *)((uint8_t *)round + 0x24) = rc.bottom;
                        rgn = CreateRoundRectRgn(rc.left, rc.top, rc.right, rc.bottom,
                                                 *(int *)((uint8_t *)round + 0x28),
                                                 *(int *)((uint8_t *)round + 0x2c));
                        round[1] = (int64_t)(uintptr_t)rgn;
                        if ((HGDIOBJ)round[3] != 0)
                            DeleteObject((HGDIOBJ)round[3]);
                        tmpRgn = CreateRoundRectRgn(rc.left, rc.top, rc.right, rc.bottom,
                                                    *(int *)((uint8_t *)round + 0x28),
                                                    *(int *)((uint8_t *)round + 0x2c));
                        round[3] = (int64_t)(uintptr_t)tmpRgn;
                        SetWindowRgn(*(HWND *)(b + OBJ_HWND), (HRGN)round[1], 1);
                        if (oldObj != 0)
                            DeleteObject(oldObj);
                    }
                    if (*(uint32_t *)((uint8_t *)round + 0x30) != 0) {
                        PECMD_FrameRegion((uint64_t *)round, (HDC)wParam, (HRGN)round[3],
                                      *(uint32_t *)((uint8_t *)round + 0x30),
                                      *(int *)((uint8_t *)round + 0x34));
                    }
                }
                if (*(HWND *)(b + 0xc8) != 0)
                    PostMessageW(*(HWND *)(b + 0xc8), 0x453, 0, 0);
                if (*(int64_t *)(b + 0xc0) != 0)
                    PostMessageW(*(HWND *)(b + OBJ_HWND), 0x453, 0, (LPARAM)obj);
                return result;
            }
            if (msg == 0x20) {
                if (*(HCURSOR *)(b + 0x30) != 0) {
                    SetCursor(*(HCURSOR *)(b + 0x30));
                    return 1;
                }
                return ((int64_t (*)(int64_t, int64_t, uint32_t, uint32_t))vt[0x80 / 8])(
                    obj, wParam,
                    (uint32_t)(uint64_t)lParam & 0xffff,
                    (uint32_t)((uint64_t)lParam >> 16) & 0xffff);
            }
            if (msg == 0x2b) {
                ((int64_t (*)(int64_t, int64_t, int64_t *))vt[0xd8 / 8])(
                    obj, wParam, lParam);
                return 0;
            }
        } else {
            if (msg == 0x83) {
                ((int64_t (*)(int64_t, uint32_t, int64_t *))vt[0xc8 / 8])(
                    obj, (uint32_t)wParam, lParam);
                return 0;
            }
            if (msg == 0x84) {
                rc.left = (int16_t)(uint16_t)(uint64_t)lParam;
                rc.top = (int16_t)(uint16_t)((uint64_t)lParam >> 16);
                result = (int)((int64_t (*)(int64_t, uint64_t))vt[0x70 / 8])(
                    obj, (uint64_t)(uint32_t)((uint32_t)(uint16_t)rc.top << 16 |
                                              (uint32_t)(uint16_t)rc.left));
                if (result != (int)0x8000000000000000LL)
                    return result;
                msg = 0x84;
                goto default_dispatch;
            }
            if (msg == 0x85) {
                ((void (*)(void))vt[0xa8 / 8])();
                ((int64_t (*)(uint64_t, uint64_t, int64_t, int64_t *))vt[8])(
                    *(uint64_t *)(b + OBJ_HWND), 0x85, wParam, lParam);
                return 0;
            }
            if (msg == 0x113) {
                ((int64_t (*)(int64_t, int64_t))vt[0x88 / 8])(obj, wParam);
                return 0;
            }
            if (msg == 0x200) {
                rc.left = (int16_t)(uint16_t)(uint64_t)lParam;
                rc.top = (int16_t)(uint16_t)((uint64_t)lParam >> 16);
                ((int64_t (*)(int64_t, uint32_t, uint64_t))vt[0x48 / 8])(
                    obj, (uint32_t)wParam,
                    (uint64_t)(uint32_t)((uint32_t)(uint16_t)rc.top << 16 |
                                         (uint32_t)(uint16_t)rc.left));
                return 0;
            }
            if (msg == 0x201) {
                rc.left = (int16_t)(uint16_t)(uint64_t)lParam;
                rc.top = (int16_t)(uint16_t)((uint64_t)lParam >> 16);
                ((int64_t (*)(int64_t, uint32_t, uint64_t))vt[0x50 / 8])(
                    obj, (uint32_t)wParam,
                    (uint64_t)(uint32_t)((uint32_t)(uint16_t)rc.top << 16 |
                                         (uint32_t)(uint16_t)rc.left));
                return 0;
            }
        }
    } else if (msg < 0x454) {
        if (msg == 0x453) {
            round = *(int64_t **)(b + 0xc0);
            if (round != NULL && *round != 0 && (int)round[6] != 0) {
                rgn = (HRGN)round[3];
                tmpRgn = (HRGN)0;
                if (rgn == 0) {
                    rc.left = 0;
                    rc.top = 0;
                    rc.right = 0;
                    rc.bottom = 0;
                    GetWindowRect(*(HWND *)(b + OBJ_HWND), &rc);
                    rc.right -= rc.left;
                    rc.bottom -= rc.top;
                    rc.left = 0;
                    rc.top = 0;
                    rgn = CreateRoundRectRgn(0, 0, rc.right, rc.bottom,
                                             (int)round[5],
                                             *(int *)((uint8_t *)round + 0x2c));
                    tmpRgn = rgn;
                }
                hwnd = *(HWND *)(b + OBJ_HWND);
                hdc = GetDC(hwnd);
                PECMD_FrameRegion((uint64_t *)round, hdc, rgn,
                              *(uint32_t *)((uint8_t *)round + 0x30),
                              *(int *)((uint8_t *)round + 0x34));
                if (tmpRgn != 0)
                    DeleteObject(tmpRgn);
                if (hdc != 0)
                    ReleaseDC(hwnd, hdc);
            }
            return 0;
        }
        if (msg == 0x204) {
            rc.left = (int16_t)(uint16_t)(uint64_t)lParam;
            rc.top = (int16_t)(uint16_t)((uint64_t)lParam >> 16);
            ((int64_t (*)(int64_t, uint32_t, uint64_t))vt[0x60 / 8])(
                obj, (uint32_t)wParam,
                (uint64_t)(uint32_t)((uint32_t)(uint16_t)rc.top << 16 |
                                     (uint32_t)(uint16_t)rc.left));
            return 0;
        }
        if (msg == 0x205) {
            rc.left = (int16_t)(uint16_t)(uint64_t)lParam;
            rc.top = (int16_t)(uint16_t)((uint64_t)lParam >> 16);
            ((int64_t (*)(int64_t, uint32_t, uint64_t))vt[0x68 / 8])(
                obj, (uint32_t)wParam,
                (uint64_t)(uint32_t)((uint32_t)(uint16_t)rc.top << 16 |
                                     (uint32_t)(uint16_t)rc.left));
            return 0;
        }
        if (msg == 0x448) {
            return ((int64_t (*)(int64_t, int64_t))vt[0xe0 / 8])(obj, wParam);
        }
        if (msg == 0x449) {
            hwnd = *(HWND *)(b + OBJ_HWND);
            hdc = GetDC(hwnd);
            rc.left = 0;
            rc.top = 0;
            rc.right = 0;
            rc.bottom = 0;
            GetClientRect(hwnd, &rc);
            color = GetPixel(hdc, (rc.right + rc.left) / 2, (rc.bottom + rc.top) / 2);
            if (hdc != 0)
                ReleaseDC(hwnd, hdc);
            return (int64_t)(int)color;
        }
        if (msg == 0x44c) {
            g_hActiveDevWnd = 0;
            SendMessageW(*(HWND *)(b + OBJ_HWND), 0x2a3, 0, 0);
            return 0;
        }
        if (msg == 0x452) {
            return ((int64_t (*)(int64_t, int64_t, int64_t *))vt[0x110 / 8])(
                obj, wParam, lParam);
        }
    } else {
        if (msg == 0x459) {
            if (wParam != obj)
                return 1;
            return PECMD_CtlForwardSetBlock((int64_t *)(uintptr_t)obj,
                                        (uint64_t *)(uintptr_t)lParam);
        }
        if (msg == 0x45b) {
            int64_t target = obj;
            if (wParam != obj && wParam != (int64_t)(uintptr_t)g_Script)
                return 0;
            if (*(int8_t *)(b + 0x61) == 2 && *(int64_t *)(b + 0x110) == 0) {
                FUN_1400EF91C(obj, 0x10000000, 0);
            }
            if (*(int8_t *)(b + 0x61) != 2 || *(int64_t *)(b + 0x110) == 0)
                target = obj;
            else
                target = *(int64_t *)(b + 0x110);
            if ((int64_t)-0x42c1d81 < lParam[0])
                *(int *)((uint8_t *)(uintptr_t)target + 0x50) = (int)(float)lParam[0];
            if (lParam[1] != (int64_t)INT32_MIN)
                *(int *)((uint8_t *)(uintptr_t)target + 0x54) = (int)lParam[1];
            if (lParam[2] != (int64_t)INT32_MIN)
                *(int *)((uint8_t *)(uintptr_t)target + 0x58) = (int)lParam[2];
            if (lParam[3] != (int64_t)INT32_MIN)
                *(int *)((uint8_t *)(uintptr_t)target + 0x5c) = (int)lParam[3];
            if ((LPCWSTR)lParam[4] != NULL)
                SetWindowTextW(*(HWND *)((uint8_t *)(uintptr_t)target + OBJ_HWND),
                               (LPCWSTR)lParam[4]);
            if (0 < lParam[5])
                *(uint8_t *)((uint8_t *)(uintptr_t)target + 0x60) = (uint8_t)lParam[5];
            InvalidateRect(*(HWND *)((uint8_t *)(uintptr_t)target + OBJ_HWND), NULL, 1);
            return 0;
        }
        if (msg == 0xbc19) {
            result = (int)((int64_t (*)(int64_t, int64_t, uint32_t))vt[0x98 / 8])(
                obj, wParam, (uint32_t)(uint64_t)lParam & 0xffffffff);
            if (result != 0)
                return result;
            msg = 0xbc19;
            goto default_dispatch;
        }
        if (msg == 0xbc2b) {
            ((int64_t (*)(int64_t, int64_t *))vt[0xd0 / 8])(obj, lParam);
            if (*(HWND *)(b + 0xc8) != 0)
                PostMessageW(*(HWND *)(b + 0xc8), 0x453, 0, (LPARAM)obj);
            if (*(int64_t *)(b + 0xc0) == 0)
                return 0;
            PostMessageW(*(HWND *)(b + OBJ_HWND), 0x453, 0, (LPARAM)obj);
            return 0;
        }
        if (msg == 0xbc2c) {
            ((int64_t (*)(int64_t, int64_t *))vt[0xb8 / 8])(obj, lParam);
            return 0;
        }
    }
default_dispatch:
    vtSelf = *(uint64_t *)(b + OBJ_HWND);
    return ((int64_t (*)(uint64_t, uint64_t, int64_t, int64_t *))vt[1])(
        vtSelf, msg, wParam, lParam);
}

/* ========== FUN_1400F172C @0x1400f172c ==========
 * 消息映射表分发：按 hwnd/msg/mode 匹配 0x28 字节表项，执行表项中
 * 的命令串（支持 ":" 变量前缀、"," 附加变量、"-" 抑制错误、"<" 临界区）。
 * TODO(verify): PECMD_ProcessScriptBlock 返回值（原反编译为 LARGE_INTEGER）。
 */
uint16_t FUN_1400F172C(int64_t *map, int msg, uint64_t wParam, uint64_t *lParam,
                              int64_t hwnd, uint8_t mode, uint64_t *out)
{
    uint16_t ret = 0;
    int idx = -1;
    int64_t entIdx = -1;
    int64_t base = map[1];
    int count = (int)map[2];
    int64_t script;
    int64_t execResult = 0;

    for (;;) {
        WCHAR *cmdCopy = NULL;
        WCHAR *varName = NULL;
        WCHAR *varVal = NULL;
        WCHAR *varKey = NULL;
        WCHAR *p;
        uint8_t *e;
        uint16_t flags;
        uint16_t extra;
        int64_t LVar9;
        uint8_t bVar10 = 0;
        int cVar11 = 0;

        if (count < 1) {
            PECMD_FreeStrBuf(&cmdCopy);
            return ret;
        }
        script = map[0];
        for (;;) {
            idx++;
            entIdx++;
            if (count <= idx) {
                PECMD_FreeStrBuf(&cmdCopy);
                return ret;
            }
            e = (uint8_t *)(base + entIdx * 0x28);
            flags = *(uint16_t *)(e + 0x18);
            if (hwnd != *(int64_t *)(e + 0x00) ||
                mode != (uint8_t)(flags & 0x93) ||
                *(int *)(e + 0x08) != msg) {
                continue;
            }
            extra = flags & 0x60;
            if (extra == 0)
                break;
            if (msg == 0x111 || (flags & 0x100) != 0) {
                if (((flags & 0x20) == 0 ||
                     *(uint16_t *)(e + 0x20) == (uint16_t)wParam) &&
                    ((flags & 0x40) == 0 ||
                     *(uint32_t *)(e + 0x1c) == (uint32_t)(wParam >> 16))) {
                    break;
                }
            } else if ((msg != 0x4e && (flags & 0x200) == 0) ||
                       (((flags & 0x20) == 0 ||
                         *(uint16_t *)(e + 0x20) == (uint16_t)lParam[1]) &&
                        ((flags & 0x40) == 0 ||
                         *(int *)(e + 0x1c) == (int)lParam[2]))) {
                break;
            }
        }

        if (*(WCHAR **)(e + 0x10) == NULL || **(WCHAR **)(e + 0x10) == L'\0') {
            PECMD_FreeStrBuf(&cmdCopy);
            return ret;
        }
        ret = flags;
        PECMD_StrBldCopyWide(&cmdCopy, *(WCHAR **)(e + 0x10));
        p = cmdCopy;
        PECMD_AllocStrSlot(&varName);
        PECMD_AllocStrSlot(&varVal);
        PECMD_AllocStrSlot(&varKey);

        if (*p == L':') {
            p++;
            PECMD_SplitTokenTrimWs((int64_t *)&p, (int64_t *)&varName, 0x2c);
            FUN_14007BF44((int64_t *)script, varName, &varVal, 0, 1);
            if (msg == 0x111 || ((flags >> 8) & 1) != 0) {
                PECMD_FormatSetVar((int64_t *)script, wParam & 0xffff,
                              WSTR("&&__wParam.wID"), WSTR("0x%X"));
                PECMD_AppendFmtValue((void *)script, wParam >> 16,
                                WSTR("&&__wParam.wNotifyCode"), WSTR("0x%I64X"));
                extra |= 0x1000;
            } else if (msg == 0x4e || ((flags >> 9) & 1) != 0) {
                PECMD_AppendFmtValue((void *)script, lParam[1],
                                WSTR("&&__NMHDR.idFrom"), WSTR("0x%I64X"));
                PECMD_FormatSetVar((int64_t *)script, (uint32_t)lParam[2],
                              WSTR("&&__NMHDR.code"), WSTR("0x%X"));
                PECMD_AppendFmtValue((void *)script, *lParam,
                                WSTR("&&__NMHDR.hwndFrom"), WSTR("0x%I64X"));
                extra |= 0x1000;
            }
            if (varVal != NULL && *varVal != L'\0') {
                PECMD_AppendFmtValue((void *)script, wParam, varVal, WSTR("0x%I64X"));
            }
            if (*p == L',') {
                p++;
                PECMD_SplitTokenTrimWs((int64_t *)&p, (int64_t *)&varKey, 0x2c);
                FUN_14007BF44((int64_t *)script, varKey, &varVal, 0, 1);
                if (varVal != NULL && *varVal != L'\0') {
                    PECMD_AppendFmtValue((void *)script, (uint64_t)(uintptr_t)lParam,
                                    varVal, WSTR("0x%I64X"));
                }
                if (*p == L',')
                    p++;
            }
        }

        FUN_14005B154(&p);
        LVar9 = script;
        if (*p == L'-') {
            p++;
            FUN_14005B154(&p);
            bVar10 = *(uint8_t *)(*(int64_t *)(LVar9 + 0x40) + 0x122);
            while ((bVar10 & 0xf0) == 0x10) {
                LVar9 = *(int64_t *)(LVar9 + 0x38);
                bVar10 = *(uint8_t *)(*(int64_t *)(LVar9 + 0x40) + 0x122);
            }
        }
        if (*p != L'\0') {
            if (*p == L'<') {
                p++;
                EnterCriticalSection(&g_csInit);
                g_msgLockCount++;
                bVar10 = *(uint8_t *)(LVar9 + 0x11);
                cVar11 = 1;
                *(uint8_t *)(LVar9 + 0x11) = (uint8_t)(bVar10 | 2);
                bVar10 = (uint8_t)((bVar10 >> 1) & 1);
                LeaveCriticalSection(&g_csInit);
            } else {
                cVar11 = -1;
            }
            execResult = PECMD_ProcessScriptBlock((uint64_t)LVar9, (uint64_t)(uintptr_t)p,
                                       *(void **)(LVar9 + 0x40), NULL, NULL);
            if (cVar11 >= 0) {
                EnterCriticalSection(&g_csInit);
                if (cVar11 != 0)
                    g_msgLockCount--;
                if (bVar10 == 0)
                    *(uint8_t *)(LVar9 + 0x11) &= 0xfd;
                LeaveCriticalSection(&g_csInit);
            }
        }
        if (out != NULL)
            *out = (uint64_t)execResult;

        PECMD_FreeStrBuf(&varKey);
        PECMD_FreeStrBuf(&varVal);
        PECMD_FreeStrBuf(&varName);
        PECMD_FreeStrBuf(&cmdCopy);
        if (extra == 0 || extra == 0x1060)
            return ret;
    }
}

/* ========== PECMD_TabHostMsgDispatch @0x1400eca00 ==========
 * 控件消息分发变体：0x462/0x463/0x465 走专用临界区路径，
 * 其余走消息映射 + FUN_1400E5B0C 兜底。
 */
uint64_t PECMD_TabHostMsgDispatch(int64_t obj, uint32_t msg, int64_t wParam,
                                       uint64_t *lParam)
{
    uint8_t *b = (uint8_t *)(uintptr_t)obj;
    uint64_t result = 0;
    uint64_t out = 0;
    uint16_t r;
    int count;
    HWND top;
    uint64_t packed;
    HWND child;
    uint64_t *slotBase;
    int i, slotIdx;

    if (msg == 0x462) {
        EnterCriticalSection(&g_csInit);
        PECMD_SelectTabPageIndex(obj, (int)wParam);
        LeaveCriticalSection(&g_csInit);
        return 1;
    }
    if (msg == 0x463) {
        EnterCriticalSection(&g_csInit);
        if ((int)(intptr_t)lParam < (int)*(int32_t *)(b + 0x118) + 3000) {
            uint8_t info[0x20] = {0};
            HWND target;
            uint64_t *data;
            if (lParam != NULL) {
                target = *(HWND *)(b + OBJ_HWND);
                data = lParam;
            } else {
                target = GetParent(*(HWND *)(b + OBJ_HWND));
                data = *(uint64_t **)(b + OBJ_HWND);
            }
            *(uint32_t *)(info + 0) = (uint32_t)(lParam == NULL);
            *(HDC *)(info + 0x18) = (HDC)wParam;
            SendMessageW(*(HWND *)(*(int64_t *)(b + OBJ_SUBWND) + OBJ_HWND), 0x439, 0, (LPARAM)info);
            (void)target;
            (void)data;
        }
        LeaveCriticalSection(&g_csInit);
        return 1;
    }
    if (msg != 0x465) {
        bool fallback = false;
        if (msg == 0x201) {
            packed = (uint64_t)(uint32_t)((uint32_t)(int16_t)((uint64_t)lParam >> 16) << 16 |
                                          (uint32_t)(int16_t)(uint64_t)lParam);
            ((int64_t (*)(int64_t, uint32_t, uint64_t))(*(void ***)(uintptr_t)obj)[0x50 / 8])(
                obj, (uint32_t)wParam, packed);
        } else if (msg == 0xf) {
            ((int64_t (*)(int64_t, int64_t, uint64_t *))(*(void ***)(uintptr_t)obj)[0x28 / 8])(
                obj, wParam, lParam);
        } else if (msg == 0x459) {
            if (wParam == obj) {
                ((int64_t (*)(int64_t, uint64_t, uint64_t, uint64_t, uint64_t, int, uint64_t))
                    (*(void ***)(uintptr_t)obj)[0xf0 / 8])(
                        obj, lParam[0], lParam[1], lParam[2], lParam[3], 0, lParam[6]);
            }
        } else {
            fallback = true;
        }
        count = *(int *)(*(int64_t *)(b + 0xe8) + 0x10);
        if (count < 1)
            top = 0;
        else
            top = FUN_1400E5788(*(HWND *)(b + OBJ_HWND));
        if (top != 0)
            SendMessageW(top, 0x450, 4, 0x5aa555aa);
        if (msg != 0x233 && 0 < count) {
            out = 0;
            r = FUN_1400F172C(*(int64_t **)(b + 0xe8), (int)msg, (uint64_t)wParam,
                                     lParam, *(int64_t *)(b + OBJ_HWND), 1, &out);
            result = out;
            if ((r & 4) != 0) {
                if (top == 0)
                    return out;
                PostMessageW(top, 0x451, 0x204, -0x55aaa55b);
                return result;
            }
        }
        if (fallback) {
            result = (uint64_t)FUN_1400E5B0C(obj, msg, wParam, (int64_t *)lParam);
            if (msg == 0x200) {
                packed = (uint64_t)((uint32_t)((uint64_t)lParam >> 16) |
                                    (uint32_t)(uint64_t)lParam) & 0xffff0000ffffULL;
                PECMD_ControlOnMouseMove((int64_t *)(uintptr_t)obj, (int)wParam, packed);
            } else if (msg == 0x100) {
                PECMD_OnTabSelChange(obj);
            } else if (msg == 0x47) {
                PECMD_LayoutTabPageArea(obj, (char)((*(uint32_t *)(b + OBJ_LINK) >> 7) & 1));
            }
        }
        if (0 < count)
            FUN_1400F172C(*(int64_t **)(b + 0xe8), (int)msg, (uint64_t)wParam,
                                 lParam, *(int64_t *)(b + OBJ_HWND), 2, NULL);
        if (top != 0)
            PostMessageW(top, 0x451, 0x204, -0x55aaa55b);
        return result;
    }

    EnterCriticalSection(&g_csInit);
    slotBase = *(uint64_t **)(b + 0x108);
    i = *(int *)(b + 0x10); /* 结构第 4 槽 -> byte +0x10 */
    slotIdx = *(int *)(b + 0x00); /* 结构首槽 -> byte +0x00 */
    FUN_140063B00(slotIdx, (int64_t *)(b + 0x108), (int64_t *)(b + 0x110), 8);
    {
        uint8_t info[0x20] = {0};
        uint64_t *data = *(uint64_t **)((uint8_t *)wParam + 8);
        int idx = *(int *)((uint8_t *)wParam + 0x10);
        int slot = *(int *)((uint8_t *)wParam + 0x00);
        int used = *(int *)(b + 0x120);
        uint64_t *base = *(uint64_t **)(b + 0x108);
        (void)slotBase;
        if (i == 0) {
            *(uint32_t *)(info + 0) = 0;
            *(uint32_t *)(info + 4) = 1;
            *(HWND *)(info + 0x10) = *(HWND *)(b + OBJ_HWND);
            *(uint64_t **)(info + 0x18) = data;
            SendMessageW(*(HWND *)(b + OBJ_HWND), 0x133d, (WPARAM)slot, (LPARAM)info);
            child = *(HWND *)(**(int64_t **)(base + (int64_t)used * 8) + OBJ_HWND);
            if (child != 0) {
                ShowWindow(child, 0);
                ShowWindow(child, 5);
            }
        }
        (void)idx;
        (void)slot;
    }
    LeaveCriticalSection(&g_csInit);
    return 1;
}

/* ========== PECMD_RasDialEntry @0x1400ebd30 ==========
 * RAS 拨号并等待同步事件：创建事件对、填充拨号参数、调用 RasDialW，
 * 超时/失败时取错误串到 g_pRasEntryBuf。
 * TODO(verify): RasDialW 参数布局与 0xa78 句柄槽。
 */
uint64_t PECMD_RasDialEntry(int64_t conn, LPCWSTR entryName, LPCWSTR user,
                              LPCWSTR pwd, LPCWSTR domain, LPCWSTR phone)
{
    WCHAR *phonebook = NULL;
    LPCWSTR src;
    WCHAR errBuf[0x100];
    DWORD wait;
    uint64_t result = 0;

    if (g_rasState != 0)
        FUN_1400E7664(NULL);
    if (g_hSyncEvtA != 0)
        ResetEvent(g_hSyncEvtA);
    if (g_hSyncEvtB != 0)
        ResetEvent(g_hSyncEvtB);
    if (g_hSyncEvtA == 0) {
        g_hSyncEvtA = CreateEventW(NULL, 0, 0, NULL);
        if (g_hSyncEvtA == 0)
            return 0;
    }
    if (g_hSyncEvtB == 0) {
        g_hSyncEvtB = CreateEventW(NULL, 0, 0, NULL);
        if (g_hSyncEvtB == 0)
            return 0;
    }

    *(uint32_t *)(conn + 8) = 0x83c;
    src = entryName;
    if (src == NULL) {
        src = (LPCWSTR)FUN_1400E7840((int64_t *)&phonebook, 0);
        if (src == NULL)
            goto done;
    }
    lstrcpyW((LPWSTR)(conn + 0xc), src);
    if (user == NULL)
        user = g_szEmpty;
    lstrcpyW((LPWSTR)(conn + 0x20e), user);
    if (pwd == NULL)
        pwd = g_szEmpty;
    lstrcpyW((LPWSTR)(conn + 0x412), pwd);
    if (domain == NULL)
        domain = g_szEmpty;
    lstrcpyW((LPWSTR)(conn + 0x614), domain);
    if (phone == NULL)
        phone = g_szEmpty;
    lstrcpyW((LPWSTR)(conn + 0x816), phone);

    {
        typedef uint32_t (*RasDialFn)(void *, LPCWSTR, void *, uint32_t, void *, void **);
        RasDialFn rasDial = (RasDialFn)(void *)g_pfnRasDial;
        uint32_t rc = rasDial(NULL, NULL, (void *)(conn + 8), 0,
                              (void *)(uintptr_t)&PECMD_RasDialStatusCallback,
                              (void **)(conn + 0xa78));
        if (rc == 0) {
            HANDLE evts[2];
            g_rasState = *(uint64_t *)(conn + 0xa78);
            g_syncState = (void *)(uintptr_t)conn;
            evts[0] = g_hSyncEvtA;
            evts[1] = g_hSyncEvtB;
            wait = WaitForMultipleObjects(2, evts, 0, 50000);
            if (wait == 0) {
                result = 1;
                goto done;
            }
            if (wait == 1 || wait == 0x102) {
                typedef uint32_t (*RasGetErrorStringFn)(uint32_t, WCHAR *, uint32_t);
                RasGetErrorStringFn rasGetErr = (RasGetErrorStringFn)(void *)g_pfnRasGetErrorString;
                rasGetErr(g_syncEvtVal, errBuf, 0x100);
                PECMD_AllocString(&g_pRasEntryBuf, 100);
                wsprintfW((LPWSTR)g_pRasEntryBuf, WSTR("Error: %d - "), 0);
                FUN_14006375C((WCHAR **)&g_pRasEntryBuf, errBuf);
                FUN_14006375C((WCHAR **)&g_pRasEntryBuf, WSTR("\n"));
            }
            Sleep(10);
        }
    }
done:
    PECMD_FreeStrBuf(&phonebook);
    return result;
}

/* ========== FUN_1400E3288 @0x1400e3288 ==========
 * 更新/校验驱动签名策略：读取 Driver Signing Policy，必要时用
 * CryptoAPI 计算 PnP 种子哈希并写回 PrivateHash。
 * TODO(verify): 分支与返回码细节。
 */
uint32_t FUN_1400E3288(uint32_t mode, uint32_t flags)
{
    uint32_t result = 0;
    uint32_t step = 1;
    uint32_t policy = 0;
    uint32_t autoDisverify = 0;
    uint32_t regType = 1;
    uint32_t regVal[2] = {0, 0};
    BYTE seed[8] = {0};
    BYTE hash[24] = {0};
    HCRYPTPROV prov = 0;
    HCRYPTHASH hashObj = 0;
    uint32_t uVar1 = 0xffffffff;
    uint32_t uVar10;
    int iVar2;
    BOOL ok;
    DWORD err;
    uint32_t uVar5 = 0;
    LPCWSTR msg;
    uint32_t local_res8 = mode;

    if ((flags & 0x10) != 0)
        goto skip_crypto;

    regType = 4;
    ((LONG (*)(HKEY, LPCWSTR, LPCWSTR, uint32_t *, void *, uint32_t *))
        (void *)g_pSHGetValueW)(
            (HKEY)0xffffffff80000002, WSTR("SOFTWARE\\PELOGON"),
            WSTR("DEVIAutoDisverify"), &regType, &autoDisverify, &regVal[0]);
    regType = 4;
    iVar2 = ((LONG (*)(HKEY, LPCWSTR, LPCWSTR, uint32_t *, void *, uint32_t *))
        (void *)g_pSHGetValueW)(
            (HKEY)0xffffffff80000002, WSTR("SOFTWARE\\Microsoft\\Driver Signing"),
            WSTR("Policy"), &regType, &policy, &regVal[0]);
    result = policy;
    if (iVar2 == 0) {
        PECMD_TlsLogWrite((uint64_t)(uintptr_t)g_Script,
                      WSTR("DEVI驱动签名【%s】(%d.%d)\r\n"),
                      (uint64_t)(uintptr_t)&g_b12ae98,
                      (uint64_t)(flags & 0xffffffef));
        if (policy == local_res8 && (flags & 0xffffffef) == 0 &&
            regVal[0] == 1) {
            return result;
        }
        uVar10 = regType;
        if (autoDisverify != 4 && (uVar1 = result, autoDisverify != 3))
            goto LAB_33dc;
    } else {
LAB_33dc:
        result = uVar1;
        autoDisverify = 3;
        uVar10 = step;
    }
    uVar1 = autoDisverify;
    regType = 4;
    ((LONG (*)(HKEY, LPCWSTR, LPCWSTR, uint32_t *, void *, uint32_t *))
        (void *)g_pSHGetValueW)(
            (HKEY)0xffffffff80000002, WSTR("System\\WPA\\PnP"), WSTR("seed"),
            &regType, seed, &regVal[0]);

    ok = CryptAcquireContextW(&prov, NULL, NULL, 1, 0);
    if (!ok) {
        err = GetLastError();
        FUN_140025f10((int64_t)(uintptr_t)g_Script, WSTR("Err_1"), err,
                      NULL, NULL, NULL);
        if (local_res8 == 0) {
            GetLastError();
            iVar2 = 0;
            goto LAB_365f;
        }
    } else {
        ok = CryptCreateHash(prov, 0x8003, 0, 0, &hashObj);
        step = 2;
        GetLastError();
        if (!ok) {
            uVar5 = GetLastError();
            msg = WSTR("Err_2");
            iVar2 = 0;
        } else {
            BYTE data[4] = {0};
            data[1] = (BYTE)local_res8;
            ok = CryptHashData(hashObj, data, 4, 0);
            step = 3;
            GetLastError();
            if (!ok) {
                uVar5 = GetLastError();
                msg = WSTR("Err_3");
                iVar2 = 0;
            } else {
                step = 4;
                ok = CryptHashData(hashObj, seed, 4, 0);
                GetLastError();
                if (!ok) {
                    uVar5 = GetLastError();
                    msg = WSTR("Err_4");
                    iVar2 = 0;
                } else {
                    regType = 0x10;
                    iVar2 = CryptGetHashParam(hashObj, 2, hash, (DWORD *)&regType, 0);
                    step = 5;
                    GetLastError();
                    if (iVar2 != 0) {
                        PECMD_RegSetValueWithOpen((HKEY)0xffffffff80000002,
                                      WSTR("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Setup"),
                                      WSTR("PrivateHash"), 3, hash, 0x10);
                        goto LAB_365f;
                    }
                    uVar5 = GetLastError();
                    msg = WSTR("Err_5");
                }
            }
        }
        FUN_140025f10((int64_t)(uintptr_t)g_Script, msg, uVar5, NULL, NULL, NULL);
        if (local_res8 == 0)
            goto LAB_365f;
    }
    result |= 0x1000;
    if (prov != 0)
        CryptReleaseContext(prov, 0);
    prov = 0;
    if (hashObj != 0)
        CryptDestroyHash(hashObj);
    goto done;

LAB_365f:
    if (prov != 0)
        CryptReleaseContext(prov, 0);
    prov = 0;
    if (hashObj != 0)
        CryptDestroyHash(hashObj);
    PECMD_RegSetValueWithOpen((HKEY)0xffffffff80000001, WSTR("Software\\Microsoft\\Driver Signing"),
                  WSTR("Policy"), uVar1, (BYTE *)regVal, uVar10);
    PECMD_RegSetValueWithOpen((HKEY)0xffffffff80000002, WSTR("Software\\Microsoft\\Non-Driver Signing"),
                  WSTR("Policy"), uVar1, (BYTE *)regVal, uVar10);
    {
        DWORD dw = PECMD_RegSetValueWithOpen((HKEY)0xffffffff80000002,
                                 WSTR("Software\\Microsoft\\Driver Signing"),
                                 WSTR("Policy"), uVar1, (BYTE *)regVal, uVar10);
        if (iVar2 != 0) {
            GetLastError();
            step = 7;
        }
        if (dw == 0) {
            if (iVar2 == 0) {
                if (regVal[0] == 0) {
                    msg = WSTR("成功？警告_");
                    goto log_result;
                }
            } else if (regVal[0] == 0) {
                PECMD_TlsLogWrite((uint64_t)(uintptr_t)g_Script, WSTR("DEVI驱动签名【%s】\r\n"),
                              (uint64_t)(uintptr_t)&g_b12acc0, (uint64_t)uVar1);
            }
        } else {
            if (iVar2 != 0) {
                GetLastError();
                step = 7;
            }
            if (regVal[0] == 0) {
                msg = WSTR("Err_");
log_result:
                PECMD_TlsLogWrite((uint64_t)(uintptr_t)g_Script,
                              WSTR("DEVI驱动签名【%s%d:0x%lx】\r\n"),
                              (uint64_t)(uintptr_t)msg, (uint64_t)step);
            }
        }
        if (regVal[0] == 1)
            return result;
        goto done;
    }

skip_crypto:
done:
    if (local_res8 == 0) {
        regVal[0] = 1;
        PECMD_RegSetValueWithOpen((HKEY)0xffffffff80000002, WSTR("SOFTWARE\\PELOGON"),
                      WSTR("DEVIAutoDisverify"), 4, (BYTE *)regVal, 4);
    }
    return result;
}

/* ========== PECMD_DrawItemContent @0x1400f00f4 ==========
 * 绘制对象内容：背景/百分比条/图标/文本，按对象状态选色。
 * TODO(verify): '@' 图像分支的隐藏栈参数语义。
 */
void PECMD_DrawItemContent(int64_t obj, HDC hdc, int64_t target, int64_t overrideObj)
{
    RECT rc;
    LPCWSTR text = NULL;
    int64_t styleObj = target;
    uint32_t align = 1;
    uint32_t style;
    COLORREF bgColor = 0;
    HGDIOBJ oldFont;
    LONG x, y;
    uint64_t imgW = 1, imgH = 1;
    uint64_t drawW, drawH;
    HDC hdcSrc;
    uint8_t objBuf[0x20] = {0};
    int drawY;
    uint32_t uVar15 = 0;
    int64_t src;

    rc.left = rc.top = rc.right = rc.bottom = 0;
    GetClientRect(*(HWND *)(target + OBJ_HWND), &rc);
    PECMD_AllocStrSlot(&text);
    if (overrideObj != 0)
        styleObj = overrideObj;
    FUN_1400E5730(*(HWND *)(styleObj + OBJ_HWND), (int64_t *)&text);
    SetBkMode(hdc, 1);

    if (overrideObj == 0) {
        src = *(int64_t *)(obj + 0xe0);
        if (src != 0 && *(int16_t *)(src + 0xa2) >= 0 &&
            *(int64_t *)(obj + OBJ_LINK) == 0 && *text != L'\0' &&
            *(int *)(obj + 0x94 + (int64_t)*(int16_t *)(src + 0xa2) * 8) >= 0) {
            HBRUSH brush = *(HBRUSH *)(src + 0x80 + (int64_t)*(int16_t *)(src + 0xa2) * 8);
            if (brush != 0)
                FillRect(hdc, &rc, brush);
        }
    }

    oldFont = (HGDIOBJ)FUN_1400E5890(target);
    oldFont = SelectObject(hdc, oldFont);
    style = (uint32_t)GetWindowLongW(*(HWND *)(target + OBJ_HWND), -0x10);
    if ((style & 1) == 0) {
        align = 0;
        if ((style & 2) != 0)
            align = 2;
    }
    if (g_alphaThreshold <= *(float *)(obj + 0x50)) {
        bgColor = 0;
        if (*(int *)(obj + 0x54) != 0) {
            HWND parent = GetParent(*(HWND *)(obj + OBJ_HWND));
            bgColor = (COLORREF)SendMessageW(parent, 0x449, 0, 0);
        }
        PECMD_DrawScaledBarFill(target, hdc, &rc, bgColor, 0);
        if ((*(uint8_t *)(obj + 0x60) & 0x10) != 0)
            align = (uint32_t)(int8_t)*(uint8_t *)(obj + 0x60) & 3;
    }

    x = rc.left;
    y = rc.top;
    if (*(int8_t *)(obj + 0xdc) == 1 && *(int64_t *)(obj + OBJ_LINK) != 0) {
        PECMD_DrawIconInRect(obj, hdc, &rc, style & 0x800);
    }
    drawW = (uint64_t)(rc.right - rc.left);
    drawH = (uint64_t)(rc.bottom - rc.top);
    if (*(int8_t *)(obj + 0xdc) == 0 && *(int64_t *)(obj + OBJ_LINK) != 0) {
        memset(objBuf, 0, sizeof(objBuf));
        GetObjectW((HGDIOBJ)*(uintptr_t *)(obj + OBJ_LINK), 0x20, objBuf);
        imgW = (uint64_t)*(int32_t *)(objBuf + 4);
        imgH = (uint64_t)*(int32_t *)(objBuf + 0x1c);
    }
    if (*(int8_t *)(obj + 0xdc) == '@' && *(int64_t *)(obj + OBJ_LINK) != 0) {
        ((void (*)(int64_t, uint64_t *))g_pGdipGetImageWidth)(*(int64_t *)(obj + OBJ_LINK), &imgW);
        ((void (*)(int64_t))g_pGdipGetImageHeight)(*(int64_t *)(obj + OBJ_LINK));
    }
    drawH = (imgW * drawH) / imgH;
    if (drawW < drawH) {
        drawH = (imgH * drawW) / imgW;
        drawW = drawW;
    } else {
        drawW = drawH;
    }
    if ((style & 0x800) != 0)
        x += (LONG)((drawW - drawH) / 2);
    drawY = (int)y;

    if (*(int8_t *)(obj + 0xdc) == 0 && *(int64_t *)(obj + OBJ_LINK) != 0) {
        hdcSrc = CreateCompatibleDC(hdc);
        PECMD_SelectObjectSlot_b028((uint64_t *)objBuf, hdcSrc, (HGDIOBJ)*(uintptr_t *)(obj + OBJ_LINK));
        BitBlt(hdc, x, drawY, (int)drawW, (int)drawH, hdcSrc, 0, 0, 0xcc0020);
        PECMD_RestoreAndDeleteObject((uint64_t *)objBuf);
        if (hdcSrc != 0)
            DeleteDC(hdcSrc);
    }
    if (*(int8_t *)(obj + 0xdc) == '@' && *(int64_t *)(obj + OBJ_LINK) != 0) {
        int64_t tmp = 0;
        if (((int (*)(HDC, int64_t *))g_pGdipCreateFromHDC)(hdc, &tmp) == 0 && tmp != 0) {
            ((void (*)(int64_t, int64_t, LONG, int, int, uint64_t))g_pGdipDrawImageRectI)(
                tmp, *(int64_t *)(obj + OBJ_LINK), x, drawY, (int)drawW,
                ((uint64_t)(uint32_t)uVar15 << 32) | (uint32_t)(int32_t)drawH);
            ((void (*)(int64_t))g_pGdipDeleteGraphics)(tmp);
        }
    }

    if (-1 < (int)*(COLORREF *)(obj + 0xd8))
        SetTextColor(hdc, *(COLORREF *)(obj + 0xd8));
    if (*(int64_t *)(obj + OBJ_LINK) == 0 || *text != L'\0') {
        src = *(int64_t *)(obj + 0xe0);
        if (src != 0 && *(int16_t *)(src + 0xa2) >= 0) {
            int16_t idx = *(int16_t *)(src + 0xa2);
            uint32_t color = *(uint32_t *)(src + 0x90 + (int64_t)idx * 8);
            if (!IsWindowEnabled(*(HWND *)(src + OBJ_HWND))) {
                DWORD sys = GetSysColor(0x16);
                color = ((int)((color >> 8 & 0xff) + (sys >> 8 & 0xff)) >> 1) << 8 |
                        ((int)((sys >> 16 & 0xffU) + (color >> 16 & 0xffU)) >> 1) << 16 |
                        (int)((color & 0xff) + (sys & 0xff)) >> 1;
            }
            if (-1 < *(int *)(src + 0x90 + (int64_t)idx * 8))
                SetTextColor(hdc, color);
        }
        PECMD_DrawVertCenteredText(hdc, text, -1, &rc, align);
    }
    SelectObject(hdc, oldFont);
    PECMD_FreeStrBuf((WCHAR **)&text);
}

/* ========== PECMD_Wow64MapPeImage @0x1400e7124 ==========
 * 把 PE 文件映射到内存：校验 DOS/PE 头，按 32/64 位布局初始化
 * 上下文相关全局，分配映像内存并展开节区。
 * TODO(verify): 32 位 ImageBase 偏移/节区映射细节。
 */
int PECMD_Wow64MapPeImage(void *filePtr, uint32_t fileSize, int64_t *outPeHeader,
                     int64_t *outSectionTable, int64_t *outBase, size_t *outSize)
{
    uint8_t *file = (uint8_t *)filePtr;
    uint8_t *pe;
    int16_t machine;
    int64_t ctxSize;
    size_t imageSize;
    uint16_t numSections;
    uint32_t minRaw;
    void *base;
    int64_t peMapped;
    uint32_t sizeOfHeaders;
    uint32_t sizeOfImage;
    int64_t cursor;
    uint8_t *dst;

    if (fileSize <= 0x3f || *(uint16_t *)file != 0x5a4d)
        return 0;
    if ((int)*(int32_t *)(file + 0x3c) + 0x108 > (int)fileSize)
        return 0;
    pe = file + *(int32_t *)(file + 0x3c);
    if (*(uint32_t *)pe != 0x4550)
        return 0;
    if ((*(uint16_t *)(pe + 0x16) & 0x2000) != 0 ||
        (*(uint8_t *)(pe + 0x16) & 2) == 0)
        return 0;
    machine = *(int16_t *)(pe + 0x14);
    if (machine != 0xe0 && machine != 0xf0)
        return 0;

    if (machine == 0xe0) {
        g_objMode = 4;
        g_peCtxSize = 0xf8;
        g_remoteTebBase = g_peByteA84E;
        g_ptrA84C = 0;
        g_peCtxOffB = g_peWordA852;
        g_pfnGetThreadCtx = (uint32_t (*)(HANDLE, int64_t))GetProcAddress(
            GetModuleHandleW(WSTR("kernel32.dll")), "Wow64GetThreadContext");
        g_pfnSetThreadCtx = (uint32_t (*)(HANDLE, int64_t))GetProcAddress(
            GetModuleHandleW(WSTR("kernel32.dll")), "Wow64SetThreadContext");
    } else {
        g_objMode = 8;
        g_peCtxSize = 0x108;
        g_remoteTebBase = g_peByteA84F;
        g_ptrA84C = 0x30;
        g_peCtxOffB = g_peWordA853;
    }

    ctxSize = (int64_t)g_peCtxSize;
    imageSize = (size_t)PECMD_CalcPeImageSize((uint64_t)(uintptr_t)file, fileSize,
                                              (int64_t)(uintptr_t)pe,
                                              ctxSize + (int64_t)(uintptr_t)pe);
    *outSize = imageSize;
    if (imageSize == 0)
        return 0;

    numSections = *(uint16_t *)(pe + 6);
    minRaw = *(uint32_t *)(pe + 0x54);
    if (numSections != 0) {
        uint32_t *sec = (uint32_t *)(pe + ctxSize + 0x14);
        uint16_t n = numSections;
        do {
            if (*sec != 0 && *sec < minRaw)
                minRaw = *sec;
            sec += 10;
            n--;
        } while (n != 0);
    }

    if (g_objMode == 4)
        base = (void *)(uintptr_t)*(uint32_t *)(pe + 0x34);
    else
        base = *(void **)(pe + 0x30);
    base = VirtualAlloc(base, imageSize, 0x1000, 0x40);
    *outBase = (int64_t)(uintptr_t)base;
    if (base == NULL) {
        base = VirtualAlloc(NULL, *outSize, 0x1000, 0x40);
        *outBase = (int64_t)(uintptr_t)base;
        if (base == NULL)
            return 0;
    }

    memmove((void *)(uintptr_t)*outBase, file, minRaw);
    peMapped = (int64_t)(uintptr_t)*outBase + *(int32_t *)((uint8_t *)(uintptr_t)*outBase + 0x3c);
    *outPeHeader = peMapped;
    *outSectionTable = ctxSize + peMapped;

    if (g_objMode == 4)
        sizeOfHeaders = *(uint32_t *)(peMapped + 0x38);
    else
        sizeOfHeaders = *(uint32_t *)(peMapped + 0x38);
    if (g_objMode == 4)
        sizeOfImage = *(uint32_t *)(peMapped + 0x54);
    else
        sizeOfImage = *(uint32_t *)(peMapped + 0x54);
    cursor = PECMD_AlignUpSize((int64_t)sizeOfImage, sizeOfHeaders);
    dst = (uint8_t *)(cursor + *outBase);

    numSections = *(uint16_t *)(peMapped + 6);
    if (numSections != 0) {
        int64_t off = 0;
        uint16_t n = numSections;
        do {
            uint32_t va = *(uint32_t *)(off + 0xc + *outSectionTable);
            if (va != 0)
                dst = (uint8_t *)((uint64_t)va + *outBase);
            int rawSize = *(int *)(off + 0x10 + *outSectionTable);
            uint32_t secSize;
            if (rawSize == 0) {
                secSize = *(uint32_t *)(off + 8 + *outSectionTable);
            } else {
                memmove(dst, file + *(uint32_t *)(off + 0x14 + *outSectionTable),
                        (size_t)rawSize);
                secSize = *(uint32_t *)(off + 0x10 + *outSectionTable);
                if (secSize <= *(uint32_t *)(off + 8 + *outSectionTable))
                    secSize = *(uint32_t *)(off + 8 + *outSectionTable);
            }
            cursor = PECMD_AlignUpSize((int64_t)secSize, sizeOfHeaders);
            off += 0x28;
            dst += cursor;
            n--;
        } while (n != 0);
    }
    return 1;
}

/* ========== PECMD_RunPeInjectStart @0x1400e4480 ==========
 * 远程注入变体：先由 PECMD_CreateProcReadImageBase 建进程并读上下文，
 * 再根据情况写入原地址或 VirtualAllocEx 新地址后 SetThreadContext。
 * TODO(verify): 上下文布局偏移与失败路径。
 */
int PECMD_RunPeInjectStart(LPWSTR cmd, int64_t ctxBase, int64_t param3,
                                  LPCVOID data, uint32_t dataSize, BOOL inherit,
                                  uint32_t flags, LPVOID env, LPCWSTR cwd,
                                  STARTUPINFOW *si, PROCESS_INFORMATION *outPi,
                                  LPCWSTR param12)
{
    uint8_t ctx[0x4d0];
    PROCESS_INFORMATION piLocal;
    PROCESS_INFORMATION *pi;
    uint64_t *puVar1;
    uint32_t uVar3;
    uint64_t ctxOffA;
    uint64_t ctxOffB;
    LPCWSTR local_res8;
    int64_t local_res10[2] = {0, 0};
    LPCVOID local_res20 = data;
    DWORD oldProt[2] = {0, 0};
    int iVar4;
    LPCWSTR pWVar6;
    int64_t lVar8;
    LPVOID remoteBuf = NULL;
    size_t written = 0;
    BOOL wrote;
    uint64_t ctxVal;

    memset(ctx, 0, sizeof(ctx));
    piLocal.hProcess = (HANDLE)0;
    piLocal.hThread = (HANDLE)0;
    piLocal.dwProcessId = 0;
    piLocal.dwThreadId = 0;
    pi = outPi != NULL ? outPi : &piLocal;

    puVar1 = (uint64_t *)(ctxBase + 0x30);
    if (g_objMode == 4) {
        uVar3 = *(uint32_t *)(ctxBase + 0x34);
        puVar1 = (uint64_t *)(ctxBase + 0x34);
    } else {
        uVar3 = (uint32_t)*puVar1;
    }
    ctxOffA = g_remoteTebBase;
    ctxOffB = g_peCtxOffB;
    local_res8 = (LPCWSTR)(uintptr_t)uVar3;
    lVar8 = (int64_t)(uintptr_t)*(uint32_t *)(ctxBase + 0x28) + (int64_t)(uintptr_t)local_res8;
    if (g_objMode == 4)
        *(int32_t *)(ctx + ctxOffB) = (int32_t)lVar8;
    else
        *(int64_t *)(ctx + ctxOffB) = lVar8;

    pWVar6 = param12;
    if (*param12 == L'>')
        pWVar6++;

    iVar4 = (int)PECMD_CreateProcReadImageBase(cmd, (int64_t)(uintptr_t)ctx,
                                               (int64_t *)&local_res8, local_res10,
                                               inherit, flags, env, cwd, si, pi, pWVar6);
    if (g_objMode == 4)
        pWVar6 = (LPCWSTR)(uintptr_t)*(uint32_t *)puVar1;
    else
        pWVar6 = (LPCWSTR)*puVar1;
    uVar3 = *(uint32_t *)(ctxBase + 0x28);

    if (pi->hProcess == (HANDLE)0xffffffffffffffffULL)
        pi->hProcess = (HANDLE)0;
    if (pi->hThread == (HANDLE)0xffffffffffffffffULL)
        pi->hThread = (HANDLE)0;
    if (iVar4 < 1)
        return iVar4;

    if (pWVar6 == local_res8 && dataSize <= (uint32_t)local_res10[0]) {
        remoteBuf = (LPVOID)(uintptr_t)local_res8;
        VirtualProtectEx(pi->hProcess, remoteBuf, (size_t)local_res10[0], 0x40, oldProt);
    } else if (PECMD_HasVirtualAllocEx() && PECMD_ZwUnmapViewOfSection(pi->hProcess, pWVar6)) {
        remoteBuf = (LPVOID)(uintptr_t)((uint32_t (*)(HANDLE, LPCVOID, size_t, DWORD, DWORD))
            (void *)g_pfnVirtualAllocEx)(pi->hProcess, pWVar6, dataSize, 0x3000, 0x40);
        if (remoteBuf == NULL && FUN_1400E412C(ctxBase)) {
            remoteBuf = (LPVOID)(uintptr_t)((uint32_t (*)(HANDLE, LPCVOID, size_t, DWORD, DWORD))
                (void *)g_pfnVirtualAllocEx)(pi->hProcess, NULL, dataSize, 0x3000, 0x40);
            if (remoteBuf != NULL)
                PECMD_PeApplyRelocations(ctxBase, (void *)local_res20, remoteBuf);
        }
    }

    if (remoteBuf == NULL) {
        iVar4 = -2;
        goto fail;
    }

    /* 写回远程上下文与数据 */
    written = 0;
    if (g_objMode == 4)
        ctxVal = *(uint32_t *)(ctx + ctxOffA);
    else
        ctxVal = *(uint64_t *)(ctx + ctxOffA);
    WriteProcessMemory(pi->hProcess, (LPVOID)(g_objMode * 2 + ctxVal),
                       &remoteBuf, g_objMode, &written);
    if (g_objMode == 4)
        *(uint32_t *)puVar1 = (uint32_t)(uintptr_t)remoteBuf;
    else
        *puVar1 = (uint64_t)(uintptr_t)remoteBuf;

    written = 0;
    wrote = WriteProcessMemory(pi->hProcess, remoteBuf, local_res20, dataSize, &written);
    if (!wrote) {
        iVar4 = -1;
        goto fail;
    }

    if (remoteBuf != (LPVOID)(uintptr_t)local_res8)
        pWVar6 = (LPCWSTR)(uintptr_t)remoteBuf;
    lVar8 = (int64_t)(uintptr_t)pWVar6 + (int64_t)(uintptr_t)uVar3;
    if (g_objMode == 4)
        *(int32_t *)(ctx + ctxOffB) = (int32_t)lVar8;
    else
        *(int64_t *)(ctx + ctxOffB) = lVar8;
    g_pfnSetThreadCtx(pi->hThread, (int64_t)(uintptr_t)ctx);
    if ((flags & 4) == 0)
        ResumeThread(pi->hThread);
    if (outPi != NULL)
        return iVar4;
    CloseHandle(pi->hThread);
    CloseHandle(pi->hProcess);
    return iVar4;

fail:
    TerminateProcess(pi->hProcess, 0);
    CloseHandle(pi->hThread);
    pi->hThread = (HANDLE)0;
    CloseHandle(pi->hProcess);
    pi->hProcess = (HANDLE)0;
    (void)param3;
    return iVar4;
}

/* ========== FUN_1400F4E5C @0x1400f4e5c ==========
 * 向列表/表头对象插入一列：维护三个平行数组（列序/宽度/文本偏移）。
 * TODO(verify): 0x380/0x388/0x368/0x370 数组语义。
 */
WPARAM FUN_1400F4E5C(int64_t obj, LPARAM param2)
{
    uint64_t metric = FUN_1400F3554(obj, param2);
    WPARAM wParam = (WPARAM)(int)metric;
    int32_t info[3] = {0x1000, 0xf000, 0};
    int iVar1 = (int)metric;
    int64_t lVar7, lVar10, lVar6;
    WPARAM WVar3;
    uint64_t uVar2;
    int16_t *ps;

    SendMessageW(*(HWND *)(obj + OBJ_HWND), 0x102b, wParam, (LPARAM)info);
    if (*(int16_t *)(obj + 0x3b0) == 0)
        return wParam;

    {
        bool bVar11 = *(int *)(param2 + 4) == 0x7fffffff;
        if (bVar11)
            lVar10 = *(int64_t *)(*(int64_t *)(obj + 0x380) - 8 + wParam * 8) + 1;
        else
            lVar10 = *(int64_t *)(*(int64_t *)(obj + 0x380) + wParam * 8);

        PECMD_GrowByteBuffer((void **)(obj + 0x398), *(int64_t *)(obj + 0x3a0) * 8 + 0x18);
        lVar7 = *(int64_t *)(obj + 0x398);
        if (!bVar11) {
            lVar6 = lVar10;
            if (lVar10 < *(int64_t *)(obj + 0x3a0)) {
                do {
                    *(int64_t *)(lVar7 + lVar6 * 8) += (int64_t)(uint64_t)*(uint16_t *)(obj + 0x3b0);
                    lVar6++;
                } while (lVar6 < *(int64_t *)(obj + 0x3a0));
            }
            lVar6 = lVar7 + lVar10 * 8;
            memmove((void *)(lVar6 + 8), (void *)lVar6,
                    (size_t)(((int)*(int64_t *)(obj + 0x3a0) - (int)lVar10) * 8 + 0x10));
        }
        *(WPARAM *)(lVar7 + lVar10 * 8) = wParam;
        *(int64_t *)(obj + 0x3a0) += 1;

        PECMD_GrowByteBuffer((void **)(obj + 0x380),
                          ((uint64_t)*(uint16_t *)(obj + 0x3b0) + *(int64_t *)(obj + 0x388)) * 8 + 0x10);
        lVar7 = *(int64_t *)(obj + 0x380);
        if (!bVar11) {
            WVar3 = wParam;
            if ((int64_t)wParam < *(int64_t *)(obj + 0x388)) {
                do {
                    *(int64_t *)(lVar7 + WVar3 * 8) += 1;
                    WVar3++;
                } while ((int64_t)WVar3 < *(int64_t *)(obj + 0x388));
            }
            memmove((void *)(lVar7 + (*(uint16_t *)(obj + 0x3b0) + wParam) * 8),
                    (void *)(lVar7 + wParam * 8),
                    (size_t)(((int)*(int64_t *)(obj + 0x388) - iVar1) * 8 + 0x10));
        }
        lVar6 = (int64_t)(uint64_t)*(uint16_t *)(obj + 0x3b0) - 1;
        if (lVar6 >= 0) {
            int64_t *pl = (int64_t *)(lVar7 + (wParam + lVar6) * 8);
            do {
                lVar6--;
                *pl = lVar10;
                pl--;
            } while (lVar6 >= 0);
        }
        *(int64_t *)(obj + 0x388) += (int64_t)(uint64_t)*(uint16_t *)(obj + 0x3b0);

        PECMD_AllocString((WCHAR **)(obj + 0x368),
                      ((uint64_t)*(uint16_t *)(obj + 0x3b0) + *(int64_t *)(obj + 0x370)) * 2 + 4);
        lVar10 = *(int64_t *)(obj + 0x368);
        if (!bVar11) {
            memmove((void *)(lVar10 + (*(uint16_t *)(obj + 0x3b0) + wParam) * 2),
                    (void *)(lVar10 + wParam * 2),
                    (size_t)(((int)*(int64_t *)(obj + 0x370) - iVar1) * 2 + 4));
        }
        lVar7 = 1;
        *(uint16_t *)(lVar10 + wParam * 2) = *(uint16_t *)(obj + 0x3b0);
        if (1 < *(uint16_t *)(obj + 0x3b0)) {
            ps = (int16_t *)(lVar10 + 2 + wParam * 2);
            do {
                int16_t s = (int16_t)lVar7;
                lVar7++;
                *ps = -s;
                ps++;
            } while (lVar7 < (int64_t)(uint64_t)*(uint16_t *)(obj + 0x3b0));
        }
        uVar2 = (uint64_t)*(uint16_t *)(obj + 0x3b0);
        *(int64_t *)(obj + 0x370) += (int64_t)uVar2;
        while ((uVar2 = uVar2 - 1) > 0) {
            uint64_t uVar4 = FUN_1400F3554(obj, param2);
            wParam = (WPARAM)(int)uVar4;
            info[0] = 0x1000;
            info[1] = 0xf000;
            SendMessageW(*(HWND *)(obj + OBJ_HWND), 0x102b, wParam, (LPARAM)info);
        }
    }
    return wParam;
}

/* ========== FUN_1400E89FC @0x1400e89fc ==========
 * 窗口对象消息处理：覆盖退出/销毁/初始化/命令/注册/子窗口数组等，
 * 未命中分支转 FUN_1400E5B0C。
 * TODO(verify): 0x44d/0x466 数组槽位与标志位语义。
 */
uint64_t FUN_1400E89FC(int64_t obj, uint64_t msg, int64_t wParam,
                                     int64_t *lParam)
{
    uint8_t *b = (uint8_t *)(uintptr_t)obj;
    void **vt = *(void ***)(uintptr_t)obj;
    uint64_t result = 0;
    int count;
    int i;
    int64_t *arr;
    HWND hwnd;
    WPARAM wp;
    int flags;
    int32_t msgId = (int32_t)msg;

    if (msgId == 2) {
        if ((*(uint8_t *)(b + 0x120) & 0x10) == 0)
            return 0;
        PostQuitMessage(0);
        return 1;
    }
    if (msgId == 0x82) {
        SetWindowLongPtrW(*(HWND *)(b + OBJ_HWND), -4,
                          (LONG_PTR)(uintptr_t)&DefWindowProcW_exref);
        if (*(int64_t *)(b + 0xd8) == 0)
            return 1;
        if ((*(uint8_t *)(b + 0x120) & 1) != 0) {
            EnterCriticalSection(&g_csInit);
            wp = *(WPARAM *)(b + 0xd8);
            flags = *(uint8_t *)(b + 0x120);
            *(uint64_t *)(b + 0xd8) = 0;
            hwnd = *(HWND *)(wp + OBJ_HWND);
            if ((flags & 9) == 9)
                *(int64_t *)(wp + 0x1c8) -= 1;
            PostMessageW(hwnd, 0x447, wp, (LPARAM)obj);
            PostMessageW(hwnd, 0x43d, 0, 0);
            LeaveCriticalSection(&g_csInit);
            return 1;
        }
        return 1;
    }
    if (msgId == 0x110) {
        SendMessageW(*(HWND *)(b + OBJ_HWND), 0x400, 0, 0);
        return (uint64_t)((int64_t (*)(int64_t))vt[0x20 / 8])(obj);
    }
    if (msgId == 0x111) {
        uint32_t id = (uint32_t)wParam & 0xffff;
        if (id == 2) {
            ((void (*)(void))vt[0x120 / 8])();
            return 0;
        }
        if (id != 1)
            return 0;
        ((void (*)(void))vt[0x118 / 8])();
        return 0;
    }
    if (msgId == 0x400)
        return 0x64c;

    if (msgId == 0x44d) {
        if (wParam == 1) {
            int freeSlot = -1;
            count = *(int32_t *)(b + 0xe8);
            arr = *(int64_t **)(b + 0xe0);
            if (count > 0) {
                for (i = 0; i < count; i++) {
                    if (arr[i] == (int64_t)lParam)
                        return 0;
                    if (freeSlot < 0 && arr[i] == 0)
                        freeSlot = i;
                }
                if (freeSlot >= 0) {
                    arr[freeSlot] = (int64_t)lParam;
                    return 0;
                }
            }
            arr = (int64_t *)PECMD_HeapRealloc(*(void **)(b + 0xe0),
                                               (size_t)(count + 1) * 8);
            *(int64_t **)(b + 0xe0) = arr;
            if (arr == NULL) {
                *(int32_t *)(b + 0xe8) = 0;
                return 0;
            }
            arr[*(int32_t *)(b + 0xe8)] = (int64_t)lParam;
            *(int32_t *)(b + 0xe8) += 1;
            return 0;
        }
        if (wParam == 0) {
            count = *(int32_t *)(b + 0xe8);
            arr = *(int64_t **)(b + 0xe0);
            if (count > 0) {
                for (i = 0; i < count; i++) {
                    if (arr[i] == (int64_t)lParam)
                        arr[i] = 0;
                }
            }
            for (i = count - 1; i >= 0; i--) {
                if (*(int64_t **)(*(int64_t *)(b + 0xe0) + i * 8) != NULL)
                    return 0;
                *(int32_t *)(b + 0xe8) = i;
            }
            return 0;
        }
        return 0;
    }

    if (msgId == 0x45c) {
        uint8_t mode = *(uint8_t *)(b + 0xd4);
        if (lParam == (int64_t *)2) {
            *(uint8_t *)(b + 0xd4) = mode | (uint8_t)wParam;
            return mode;
        }
        if (lParam == (int64_t *)6) {
            if (mode == 2)
                wParam = (int64_t)(uint8_t)((uint8_t)wParam | 2);
        } else if (lParam == (int64_t *)8) {
            if ((mode & 1) != 0)
                PostMessageW(*(HWND *)(b + OBJ_HWND), 0x111, (WPARAM)wParam, 0x100);
            return mode;
        } else if (lParam != NULL) {
            return mode;
        }
        *(uint8_t *)(b + 0xd4) = (uint8_t)wParam;
        return mode;
    }

    if (msgId == 0x466) {
        if (wParam == 1) {
            count = *(int32_t *)(b + 0x138);
            arr = *(int64_t **)(b + 0x130);
            for (i = 0; i < count; i++) {
                if (arr[i * 3] == (int64_t)lParam) {
                    arr[i * 3] = 0;
                    *(int64_t *)(*(int64_t *)(b + 0x130) + 8 + i * 0x18) = 0;
                    return 0;
                }
            }
            return 0;
        }
        if (wParam == 2) {
            POINT pt;
            RECT rc;
            count = *(int32_t *)(b + 0x138);
            arr = *(int64_t **)(b + 0x130);
            for (i = 0; i < count; i++) {
                if (arr[i * 3] == (int64_t)lParam) {
                    pt.x = 0; pt.y = 0;
                    ClientToScreen(*(HWND *)(b + OBJ_HWND), &pt);
                    if (pt.y < -0xc7f)
                        return 0;
                    rc.left = rc.top = rc.right = rc.bottom = 0;
                    GetWindowRect((HWND)arr[i * 3], &rc);
                    *(LONG *)(arr[i * 3] + 0x10) = rc.left - pt.x;
                    *(LONG *)(arr[i * 3] + 0x14) = rc.top - pt.y;
                    return 0;
                }
            }
            return 0;
        }
        if (wParam == 3) {
            int64_t target = *lParam;
            int64_t packed = lParam[1];
            int64_t x = lParam[2];
            int64_t y = lParam[3];
            count = *(int32_t *)(b + 0x138);
            arr = *(int64_t **)(b + 0x130);
            for (i = 0; i < count; i++) {
                if ((HWND)arr[i * 3] == (HWND)target) {
                    POINT pt = {0, 0};
                    ClientToScreen(*(HWND *)(b + OBJ_HWND), &pt);
                    if ((packed & 1) != 0)
                        *(int *)(arr[i * 3] + 0x10) = (int)x;
                    if ((packed & 2) != 0)
                        *(int *)(arr[i * 3] + 0x14) = (int)y;
                    SendMessageW((HWND)target, 0x466, 4, (LPARAM)(arr + i * 3));
                    return 0;
                }
            }
            return 0;
        }
        if (wParam == 4) {
            int64_t child = *lParam;
            int yOffset = *(int *)((uint8_t *)lParam + 0x14);
            *(uint8_t *)(b + 0x158) = 1;
            if (yOffset < 0x7d1 && yOffset >= 0) {
                SetWindowPos(*(HWND *)(b + OBJ_HWND), 0,
                             *(int *)(child + 0x10) + (int)lParam[2],
                             yOffset + *(int *)(child + 0x14), 0, 0, 5);
            }
            *(uint8_t *)(b + 0x158) = 0;
            return 0;
        }
        if (wParam == 5) {
            count = *(int32_t *)(b + 0x138);
            arr = *(int64_t **)(b + 0x130);
            if (count <= 0)
                return 0;
            for (i = 0; i < count; i++) {
                if ((int64_t *)arr[i * 3] == lParam) {
                    int64_t lVar9 = (int64_t)i;
                    *(int64_t *)(*(int64_t *)(b + 0x130) + lVar9 * 0x18) = *lParam;
                    *(int64_t *)(*(int64_t *)(b + 0x130) + 8 + lVar9 * 0x18) = lParam[1];
                    *(int64_t *)(*(int64_t *)(b + 0x130) + 0x10 + lVar9 * 0x18) = lParam[2];
                    return 0;
                }
            }
            i = -1;
            for (count = *(int32_t *)(b + 0x138); count > 0; count--) {
                int64_t *entry = (int64_t *)(*(int64_t *)(b + 0x130) + (count - 1) * 0x18);
                if (*entry == 0) {
                    i = count - 1;
                    break;
                }
            }
            if (i < 0) {
                arr = (int64_t *)PECMD_HeapRealloc(*(void **)(b + 0x130),
                                                   (size_t)(*(int32_t *)(b + 0x138) + 1) * 0x18);
                *(int64_t **)(b + 0x130) = arr;
                i = *(int32_t *)(b + 0x138);
            }
            {
                int64_t base = *(int64_t *)(b + 0x130);
                *(int64_t *)(base + i * 0x18) = *lParam;
                *(int64_t *)(base + 8 + i * 0x18) = lParam[1];
                *(int64_t *)(base + 0x10 + i * 0x18) = lParam[2];
                if (i >= *(int32_t *)(b + 0x138))
                    *(int32_t *)(b + 0x138) = i + 1;
            }
            return 0;
        }
        return 0;
    }

    result = (uint64_t)FUN_1400E5B0C(obj, msg, wParam, lParam);
    if (msgId == 0x47) {
        if (*(HWND *)(b + 0x150) != 0) {
            if (*(uint8_t *)(b + 0x158) == 0)
                SendMessageW(*(HWND *)(b + 0x150), 0x466, 3, *(LPARAM *)(b + OBJ_HWND));
        }
    }
    return result;
}

/* ========== PECMD_RescaleDcPixelsWeighted @0x1400ecf90 ==========
 * 双线性插值缩放图像：从源 DC 采样加权平均后写入目标 DC。
 * TODO(verify): 权重边界公式。
 */
void PECMD_RescaleDcPixelsWeighted(HDC hdcDst, int xDst, int yDst, int64_t dstW,
                              int64_t dstH, HDC hdcSrc, int xSrc, int ySrc,
                              int64_t srcW, int64_t srcH)
{
    int64_t srcYAcc = 0;
    int64_t srcXAcc = 0;
    int64_t dstY = 0;

    if (dstH <= 0)
        return;
    {
        int64_t lVar8 = dstH - 1;
        do {
            int64_t dstX = 0;
            lVar8 += srcH;
            if (dstW > 0) {
                int64_t lVar2 = srcYAcc / dstH;
                do {
                    uint64_t bSum = 0, gSum = 0, rSum = 0, wSum = 0;
                    int64_t srcX = srcXAcc / dstW;
                    int64_t srcXEnd = (dstW - 1 + srcW + srcXAcc) / dstW;
                    if (lVar2 < lVar8 / dstH) {
                        int64_t weightY = lVar2 * dstH - srcYAcc;
                        int64_t srcY = lVar2;
                        do {
                            if (srcX < srcXEnd) {
                                int64_t weightX = srcX * dstW - srcXAcc;
                                int64_t srcXX = srcX;
                                do {
                                    int64_t wx;
                                    int64_t wy;
                                    int64_t w;
                                    COLORREF c;
                                    if (weightX < 0)
                                        wx = srcW + weightX;
                                    else if (srcW < dstW + weightX)
                                        wx = (srcW * 2 - weightX) - dstW;
                                    else
                                        wx = srcW;
                                    if (weightY < 0)
                                        wy = srcH + weightY;
                                    else if (srcH < weightY + dstH)
                                        wy = (srcH * 2 - weightY) - dstH;
                                    else
                                        wy = srcH;
                                    w = wx * wy;
                                    c = GetPixel(hdcSrc, xSrc + (int)srcXX, ySrc + (int)srcY);
                                    bSum += (uint64_t)(uint8_t)c * (uint64_t)w;
                                    gSum += (uint64_t)(uint8_t)(c >> 8) * (uint64_t)w;
                                    rSum += (uint64_t)(uint8_t)(c >> 16) * (uint64_t)w;
                                    wSum += (uint64_t)w;
                                    srcXX++;
                                    weightX += dstW;
                                } while (srcXX < srcXEnd);
                            }
                            srcY++;
                            weightY += dstH;
                        } while (srcY < lVar8 / dstH);
                    }
                    SetPixel(hdcDst, xDst + (int)dstX, yDst + (int)dstY,
                             (COLORREF)(((uint8_t)(gSum / wSum) << 8) |
                                        ((uint8_t)(rSum / wSum) << 16) |
                                        (uint8_t)(bSum / wSum)));
                    dstX++;
                    srcXAcc += srcW;
                } while (dstX < dstW);
            }
            dstY++;
            srcYAcc += srcH;
        } while (dstY < dstH);
    }
}

/* ========== FUN_1400F1C8C @0x1400f1c8c ==========
 * 控件消息分发包装：处理 0x233 拖放/0x44b 等专用路径后走映射 + GDI 兜底。
 * TODO(verify): 原反编译 unaff_R15 返回分支按 0 处理。
 */
uint64_t FUN_1400F1C8C(int64_t obj, uint64_t msg, int64_t wParam,
                                       uint64_t *lParam)
{
    uint8_t *b = (uint8_t *)(uintptr_t)obj;
    void **vt = *(void ***)(uintptr_t)obj;
    uint64_t result = 0;
    uint64_t out = 0;
    uint16_t r;
    int count;
    HWND top = 0;
    bool fallback = false;
    WCHAR *drop = NULL;

    if (msg == 0x45e)
        return 0;
    if ((msg == 0x100 || msg == 0x101) && wParam == 0x1b)
        return 0;
    if (msg > 0x1ff) {
        if (msg < 0x204) {
            FUN_1400E5B0C(obj, msg, wParam, (int64_t *)lParam);
            goto after;
        }
        if (msg == 0x233) {
            if ((*(uint8_t *)(b + 0xac) & 0x30) != 0) {
                PECMD_AllocStrSlot(&drop);
                if ((*(uint8_t *)(b + 0xac) & 0x10) == 0) {
                    PECMD_EnumDropFiles((void *)(uintptr_t)wParam, (int64_t *)&drop, (int64_t *)&top);
                } else {
                    PECMD_EnumDropFiles((void *)(uintptr_t)wParam, (int64_t *)&top, (int64_t *)&drop);
                }
                if (drop != NULL && *drop != L'\0') {
                    LPCWSTR key = *(LPCWSTR *)(*(int64_t *)(b + 0xe0) + 0x10);
                    if (*key != L'\0')
                        FUN_14007D0AC(*(int64_t ***)(*(int64_t *)(b + 0xe0) + 0x50),
                                             key, drop);
                    SetWindowTextW(*(HWND *)(b + OBJ_HWND), drop);
                }
                PECMD_FreeStrBuf(&drop);
            }
            goto after;
        }
        if (msg == 1099) {
            (*(void (**)(void))(*(void ***)(*(int64_t *)(b + 0xe0)))[3])();
            goto after;
        }
        if (msg == 0xbc19) {
            return (uint64_t)((int64_t (*)(int64_t, int64_t, uint32_t))vt[0x98 / 8])(
                obj, wParam, (uint32_t)(uint64_t)lParam & 0xffffffff);
        }
    }
    fallback = true;
after:
    count = *(int *)(*(int64_t *)(b + 0xd8) + 0x10);
    if (count > 0)
        top = FUN_1400E5788(*(HWND *)(b + OBJ_HWND));
    if (top != 0)
        SendMessageW(top, 0x450, 4, 0x5aa555aa);
    if (count > 0) {
        out = 0;
        r = FUN_1400F172C(*(int64_t **)(b + 0xd8), (int)msg, wParam, lParam,
                                 *(int64_t *)(b + OBJ_HWND), 1, &out);
        if ((r & 4) != 0) {
            if (top == 0)
                return 0;
            PostMessageW(top, 0x451, 0x204, -0x55aaa55b);
            return 0;
        }
    }
    if (fallback)
        FUN_1400E5B0C(obj, msg & 0xffffffff, wParam, (int64_t *)lParam);
    if (msg == 0x47)
        PECMD_AdjustEditScrollRect(*(int64_t *)(b + 0xe0));
    if (count > 0)
        FUN_1400F172C(*(int64_t **)(b + 0xd8), (int)msg, wParam, lParam,
                             *(int64_t *)(b + OBJ_HWND), 2, NULL);
    if (top != 0)
        PostMessageW(top, 0x451, 0x204, -0x55aaa55b);
    return result;
}

/* ========== FUN_1400FB654 @0x1400fb654 ==========
 * 控件消息分发包装（滚轮/拖放/0x459 专用路径 + 映射 + GDI 兜底）。
 * TODO(verify): 原反编译复杂指针返回类型按 uint64_t 处理。
 */
uint64_t FUN_1400FB654(int64_t obj, uint64_t msg, int64_t wParam,
                                       uint64_t *lParam)
{
    uint8_t *b = (uint8_t *)(uintptr_t)obj;
    uint64_t result = 0;
    uint64_t out = 0;
    uint16_t r;
    int count;
    HWND top = 0;
    bool fallback = false;
    WCHAR *drop = NULL;

    if (msg == 0x45e)
        return 0x20000;
    if (msg == 0x203) {
        return (uint64_t)FUN_1400E5B0C(obj, 0x203, wParam, (int64_t *)lParam);
    }
    if (msg == 0x20a) {
        int16_t s = (int16_t)((uint64_t)wParam >> 16);
        int delta = (int)s;
        int scroll = 0;
        if (s < 0)
            delta = -delta;
        SystemParametersInfoW(0x68, 0, &scroll, 0);
        delta = (scroll * delta) / 0x78 - 1;
        if (delta >= 0) {
            do {
                SendMessageW(*(HWND *)(b + OBJ_HWND), 0x115, (WPARAM)(s < 0), 0);
                delta--;
            } while (delta >= 0);
        }
        goto after;
    }
    if (msg == 0x233) {
        if ((*(uint8_t *)(b + 0xac) & 0x30) != 0) {
            PECMD_AllocStrSlot(&drop);
            if ((*(uint8_t *)(b + 0xac) & 0x10) == 0) {
                PECMD_EnumDropFiles((void *)(uintptr_t)wParam, (int64_t *)&drop, (int64_t *)&top);
            } else {
                PECMD_EnumDropFiles((void *)(uintptr_t)wParam, (int64_t *)&top, (int64_t *)&drop);
            }
            if (drop != NULL && *drop != L'\0') {
                LPCWSTR key = *(LPCWSTR *)(*(int64_t *)(b + 0xe0) + 0x10);
                if (*key != L'\0')
                    FUN_14007D0AC(*(int64_t ***)(*(int64_t *)(b + 0xe0) + 0x50),
                                         key, drop);
                SetWindowTextW(*(HWND *)(b + OBJ_HWND), drop);
            }
            PECMD_FreeStrBuf(&drop);
        }
        goto after;
    }
    if (msg == 0x459) {
        if (wParam == obj) {
            result = (uint64_t)((int64_t (*)(int64_t, uint64_t, uint64_t, uint64_t, uint64_t, int))
                (*(void ***)(lParam[6]))[1])(
                    (int64_t)lParam[6], lParam[0], lParam[1], lParam[2], lParam[3], 0);
            return result;
        }
    } else {
        fallback = true;
    }
after:
    count = *(int *)(*(int64_t *)(b + 0xd8) + 0x10);
    if (count > 0)
        top = FUN_1400E5788(*(HWND *)(b + OBJ_HWND));
    if (top != 0)
        SendMessageW(top, 0x450, 4, 0x5aa555aa);
    if (count > 0) {
        out = 0;
        r = FUN_1400F172C(*(int64_t **)(b + 0xd8), (int)msg, (uint64_t)wParam, lParam,
                                 *(int64_t *)(b + OBJ_HWND), 1, &out);
        result = out;
        if ((r & 4) != 0) {
            if (top == 0)
                return out;
            PostMessageW(top, 0x451, 0x204, -0x55aaa55b);
            return result;
        }
    }
    if (fallback)
        result = (uint64_t)FUN_1400E5B0C(obj, msg, wParam, (int64_t *)lParam);
    if (msg == 0x47)
        PECMD_AdjustEditScrollRect(*(int64_t *)(b + 0xe0));
    if (count > 0)
        FUN_1400F172C(*(int64_t **)(b + 0xd8), (int)msg, (uint64_t)wParam, lParam,
                             *(int64_t *)(b + OBJ_HWND), 2, NULL);
    if (top != 0)
        PostMessageW(top, 0x451, 0x204, -0x55aaa55b);
    return result;
}

/* ========== PECMD_FormatDoubleToStr @0x1400e69ac ==========
 * 按模式/精度格式化 double，并做尾部零修剪、指数补零/去零。
 * TODO(verify): 与原始 wsprintfW 行为细节。
 */
WCHAR **PECMD_FormatDoubleToStr(WCHAR **out, double value, LPCWSTR fmt, uint32_t prec,
                                 char trim, char mode)
{
    WCHAR *tmp = NULL;
    WCHAR *suffix = NULL;
    WCHAR *expBuf = NULL;
    WCHAR *expPart = NULL;
    WCHAR *pStr;
    WCHAR *dot;
    WCHAR *last;
    WCHAR wc;
    const WCHAR *format;
    uint32_t p = prec;
    LPCWSTR old;
    LPCWSTR result = NULL;
    void *joined;
    int i, len;

    (void)fmt;
    PECMD_AllocWStringBuffer(out, 100);
    PECMD_AllocStrSlot(&tmp);
    PECMD_AllocStrSlot(&suffix);
    PECMD_AllocStrSlot(&expBuf);

    if ((int)prec < 0)
        p = 0xf;
    if (mode == 'f') {
        format = WSTR("%.*lf");
    } else if (mode == 'e') {
        format = WSTR("%.*lE");
    } else if (mode == 'g') {
        format = WSTR("%.*lG");
    } else if (value == g_dbl20b28 || g_dbl2b380 <= value ||
               value <= g_dbl2b378 ||
               (value < g_dbl2b370 && g_dbl2b368 < value)) {
        format = WSTR("%.*lf");
    } else {
        format = WSTR("%.*lE");
    }
    PECMD_SafeVFormatW(*out, 99, format, p);

    expPart = StrChrW(*out, L'E');
    if (expPart != NULL) {
        FUN_14007034C(&expBuf, expPart);
        *expPart = L'\0';
        expPart = expBuf;
    }

    pStr = *out;
    dot = StrChrW(pStr, L'.');
    if (dot != NULL) {
        uint32_t lenu = (uint32_t)lstrlenW(dot);
        if ((int)prec >= 0) {
            uint32_t u = prec;
            if (prec > 0)
                u = prec + 1;
            if (u < lenu) {
                dot[u] = L'\0';
                lenu = u;
            }
        }
        last = dot + lenu - 1;
        if (trim == 0) {
            while (dot < last && *last == L'0') {
                *last = L'\0';
                last--;
            }
        }
        wc = *last;
        if (wc == L'.') {
            *last = L'\0';
            last--;
        }
        if ((int)prec < 0 && wc != L'.') {
            if (0x11 < (int)((last + 2 - pStr) >> 1)) {
                i = (int)((dot - pStr) >> 1);
                if (i < 0x11)
                    pStr[0x11] = L'\0';
                else
                    pStr[i] = L'\0';
            }
            len = lstrlenW(pStr);
            pStr = pStr + len - 1;
            if (trim == 0) {
                while (dot < pStr && *pStr == L'0') {
                    *pStr = L'\0';
                    pStr--;
                }
            }
            if (*pStr == L'.') {
                *pStr = L'\0';
                pStr--;
            }
        }
        if (*pStr == L'-' && pStr[1] == L'0' && pStr[2] == L'\0') {
            *pStr = L'0';
            pStr[1] = L'\0';
        }
    }

    if (expPart != NULL) {
        WCHAR *q = expPart + 1;
        WCHAR c;
        if (*q == L'-')
            q++;
        c = (WCHAR)(q[1] != L'\0');
        *q = c;
        while (c != L'\0') {
            c = q[1];
            *q = c;
            q++;
        }
        old = *out;
        *out = NULL;
        joined = FUN_14007DE70(&old, &result, expPart);
        PECMD_ReplaceStringSlot((int64_t *)out, (uint64_t *)joined);
        PECMD_FreeStrBuf((WCHAR **)&result);
        PECMD_FreeStrBuf((WCHAR **)&old);
    }

    PECMD_FreeStrBuf(&expBuf);
    PECMD_FreeStrBuf(&suffix);
    PECMD_FreeStrBuf(&tmp);
    return out;
}

/* ========== PECMD_DownscaleDibBlockAvg @0x1400eda04 ==========
 * DIB 平均缩放：把源位图按块平均写入目标 DIB。
 * TODO(verify): 缓冲布局与 bpp 处理。
 */
uint64_t PECMD_DownscaleDibBlockAvg(HDC hdc, HBITMAP srcBmp, uint64_t param3, UINT param4,
                               HBITMAP dstBmp, int64_t dstW, int64_t dstH)
{
    BITMAPINFO bmiSrc;
    BITMAPINFO bmiDst;
    UINT cLines = (UINT)dstH;
    uint8_t *buf = NULL;
    uint8_t *srcBits;
    uint8_t *dstBits;
    int64_t srcStride, dstStride, srcBpp, dstBpp;
    int64_t stepX, stepArea;
    int64_t x, y;

    (void)param3;
    memset(&bmiSrc, 0, sizeof(bmiSrc));
    bmiSrc.bmiHeader.biSize = 0x28;
    memset(&bmiDst, 0, sizeof(bmiDst));
    bmiDst.bmiHeader.biSize = 0x28;

    GetDIBits(hdc, srcBmp, 0, param4, NULL, &bmiSrc, 0);
    GetDIBits(hdc, dstBmp, 0, cLines, NULL, &bmiDst, 0);
    if (g_u8CCB1 != 0) {
        PECMD_TlsLogWrite((uint64_t)(uintptr_t)g_Script,
                      WSTR("DBGSCALEb\r\n"), 0, 0);
    }
    bmiSrc.bmiHeader.biCompression = 0;
    bmiDst.bmiHeader.biCompression = 0;
    srcBpp = bmiSrc.bmiHeader.biBitCount >> 3;
    dstBpp = bmiDst.bmiHeader.biBitCount >> 3;
    srcStride = (int64_t)((bmiSrc.bmiHeader.biWidth * bmiSrc.bmiHeader.biBitCount + 0x1f) >> 5) * 4;
    dstStride = (int64_t)(((uint64_t)bmiDst.bmiHeader.biBitCount * (uint64_t)dstW + 0x1f) >> 5) << 2;
    {
        int64_t dstBytes = dstStride * cLines;
        int64_t total = dstBytes + srcStride * bmiSrc.bmiHeader.biHeight + srcStride;
        FUN_1400633A8((void **)&buf, total);
        srcBits = buf + srcStride * bmiSrc.bmiHeader.biHeight + srcStride;
        dstBits = buf + srcStride * bmiSrc.bmiHeader.biHeight + srcStride;
        memset(dstBits, 0, (size_t)dstBytes);
        GetDIBits(hdc, srcBmp, 0, (UINT)bmiSrc.bmiHeader.biHeight, buf, &bmiSrc, 0);
    }

    stepX = bmiSrc.bmiHeader.biWidth / dstW;
    stepArea = (bmiSrc.bmiHeader.biHeight / dstH) * stepX;
    for (y = 0; y < dstH; y++) {
        uint8_t *dstRow = dstBits + y * dstStride;
        for (x = 0; x < dstW; x++) {
            int64_t sy0 = (bmiSrc.bmiHeader.biHeight * y) / dstH;
            int64_t sy1 = (bmiSrc.bmiHeader.biHeight * (y + 1)) / dstH;
            int64_t sx0 = (bmiSrc.bmiHeader.biWidth * x) / dstW;
            int64_t sx1 = (bmiSrc.bmiHeader.biWidth * (x + 1)) / dstW;
            int64_t bSum = 0, gSum = 0, rSum = 0;
            int64_t sy, sx;
            for (sy = sy0; sy < sy1; sy++) {
                uint8_t *srcRow = srcBits + sy * srcStride;
                for (sx = sx0; sx < sx1; sx++) {
                    uint8_t *p = srcRow + sx * srcBpp + 2;
                    bSum += p[-2];
                    gSum += p[-1];
                    rSum += p[0];
                }
            }
            {
                uint8_t *d = dstRow + x * dstBpp + 2;
                d[-2] = (uint8_t)(bSum / stepArea);
                d[-1] = (uint8_t)(gSum / stepArea);
                d[0] = (uint8_t)(rSum / stepArea);
            }
        }
    }

    bmiDst.bmiHeader.biWidth = (LONG)dstW;
    bmiDst.bmiHeader.biHeight = (LONG)cLines;
    SetDIBits(hdc, dstBmp, 0, cLines, dstBits, &bmiDst, 0);
    PECMD_FreeStrBuf((WCHAR **)&buf);
    return 0;
}

/* ========== FUN_1400FC4A4 @0x1400fc4a4 ==========
 * 控件消息分发包装：0x462 查询/设置对象状态，0x44a 执行脚本，
 * 其余走映射 + GDI 兜底。
 * TODO(verify): 0x462 参数数组语义。
 */
uint64_t FUN_1400FC4A4(int64_t obj, uint32_t msg, int64_t wParam,
                                       uint64_t *lParam)
{
    uint8_t *b = (uint8_t *)(uintptr_t)obj;
    void **vt = *(void ***)(uintptr_t)obj;
    uint64_t out = 0;
    uint16_t r;
    int count;
    HWND top = 0;
    uint64_t result = 0;
    uint64_t v0, v1;

    if (msg == 0x462 && wParam == obj) {
        if (lParam[0] == 0x3f) {
            result = (uint64_t)((uint64_t (*)(void))vt[0x120 / 8])();
            lParam[1] = result;
            lParam[2] = 0;
            lParam[3] = 0;
            lParam[4] = 0;
            ((void (*)(int64_t, uint64_t *, uint64_t *))vt[0x118 / 8])(obj, &lParam[2], &lParam[3]);
            lParam[4] = SendMessageW(*(HWND *)(b + OBJ_HWND), 0x416, 0, 0);
        } else {
            uint64_t local_28 = 0, local_20 = 0;
            if ((lParam[0] & 6) != 0) {
                ((void (*)(int64_t, uint64_t *, uint64_t *))vt[0x118 / 8])(obj, &local_28, &local_20);
                if ((lParam[0] & 2) == 0)
                    lParam[2] = local_28;
                if ((lParam[0] & 4) == 0)
                    lParam[3] = local_20;
                if ((lParam[0] & 1) == 0) {
                    lParam[1] = (uint64_t)((uint64_t (*)(int64_t))vt[0x120 / 8])(obj);
                    lParam[0] |= 1;
                }
                ((void (*)(int64_t, uint64_t))vt[0x130 / 8])(obj, lParam[2]);
            }
            if ((lParam[0] & 1) != 0) {
                ((void (*)(int64_t, uint64_t))vt[0x138 / 8])(obj, lParam[1]);
                if (*(int8_t *)(b + 0x61) == 0x15)
                    goto exec_script;
            }
            if ((lParam[0] & 8) != 0)
                ((void (*)(int64_t, uint64_t))vt[0x140 / 8])(obj, lParam[4]);
        }
        return 0;
    }
    if (msg == 0x44a && *(int64_t *)(b + 0xd8) != 0) {
exec_script:
        if (*(int64_t *)(b + 0xd8) != 0) {
            ((void (*)(int64_t))vt[0x120 / 8])(obj);
            PECMD_FormatOutput(*(int64_t *)(b + 0xd8));
            v0 = *(uint64_t *)(*(int64_t *)(b + 0xd8) + 0x50);
            v1 = *(uint64_t *)(*(int64_t *)(b + 0xd8) + 0x28);
            PECMD_ProcessScriptBlock(v0, v1, NULL, NULL, NULL);
        }
        return 1;
    }

    count = *(int *)(*(int64_t *)(b + OBJ_LINK) + 0x10);
    if (count < 1)
        top = 0;
    else
        top = FUN_1400E5788(*(HWND *)(b + OBJ_HWND));
    if (top != 0)
        SendMessageW(top, 0x450, 4, 0x5aa555aa);
    if (0 < count) {
        out = 0;
        r = FUN_1400F172C(*(int64_t **)(b + OBJ_LINK), (int)msg, (uint64_t)wParam,
                                 lParam, *(int64_t *)(b + OBJ_HWND), 1, &out);
        result = out;
        if ((r & 4) != 0) {
            if (top == 0)
                return out;
            PostMessageW(top, 0x451, 0x204, -0x55aaa55b);
            return result;
        }
    }
    result = (uint64_t)FUN_1400E5B0C(obj, msg, wParam, (int64_t *)lParam);
    if (0 < count)
        FUN_1400F172C(*(int64_t **)(b + OBJ_LINK), (int)msg, (uint64_t)wParam,
                             lParam, *(int64_t *)(b + OBJ_HWND), 2, NULL);
    if (top != 0)
        PostMessageW(top, 0x451, 0x204, -0x55aaa55b);
    return result;
}

/* ========== PECMD_DrawWrappedText @0x1400fe168 ==========
 * 自定义 DrawText 换行/垂直居中实现：可先测量行高数组再绘制。
 * TODO(verify): 行高/居中边界与原始行为。
 */
int PECMD_DrawWrappedText(HDC hdc, LPCWSTR text, int64_t maxChars, RECT *rect, uint32_t flags)
{
    uint32_t fmt = flags & 0xffffffda;
    int64_t width = rect->right - rect->left;
    int drawn = 0;
    int64_t remaining = maxChars;
    int64_t lineHeight = 0;
    int64_t *lineLens = NULL;
    int64_t *lineIter = NULL;
    LPCWSTR p = text;
    int64_t n;

    if (remaining < 0)
        remaining = lstrlenW(text);
    n = remaining;

    if ((flags & 4) != 0) {
        int64_t *arr;
        int64_t totalH = 0;
        int64_t baseTop = rect->top;
        int64_t baseHeight = rect->bottom - rect->top;
        PECMD_GrowByteBuffer((void **)&lineLens, n << 3);
        arr = lineLens;
        p = text;
        n = remaining;
        while (n > 0 && rect->top < rect->bottom) {
            int64_t lineLen = 1;
            int64_t k = 1;
            RECT rc2 = *rect;
            if (n > 0) {
                for (;;) {
                    rc2 = *rect;
                    DrawTextW(hdc, p, (int)k, &rc2, fmt | 0x400);
                    if (width < (int64_t)(rc2.right - rc2.left) ||
                        (lineLen = k, (int64_t)(rc2.right - rc2.left) == width)) {
                        break;
                    }
                    k++;
                    if (k > n)
                        break;
                }
            }
            if (lineHeight == 0)
                lineHeight = (int64_t)(rc2.bottom - rc2.top);
            *arr++ = lineLen;
            totalH += lineHeight;
            n -= lineLen;
            p += lineLen;
        }
        rect->top = (int)(baseTop + (baseHeight - totalH) / 2);
        rect->bottom = (int)(baseTop + baseHeight - (baseHeight - totalH) / 2);
        lineIter = lineLens;
        p = text;
        n = remaining;
    }

    for (;;) {
        int64_t lineLen;
        RECT rc;
        width = rect->right - rect->left;
        if (n < 1 || rect->bottom <= rect->top) {
            PECMD_FreeStrBuf((WCHAR **)&lineLens);
            return drawn;
        }
        if (lineIter == NULL) {
            lineLen = 1;
            {
                int64_t k = 1;
                rc = *rect;
                if (n > 0) {
                    for (;;) {
                        rc = *rect;
                        DrawTextW(hdc, p, (int)k, &rc, fmt | 0x400);
                        if (width < (int64_t)(rc.right - rc.left) ||
                            (lineLen = k, (int64_t)(rc.right - rc.left) == width)) {
                            break;
                        }
                        k++;
                        if (k > n)
                            break;
                    }
                }
                if (lineHeight == 0)
                    lineHeight = (int64_t)(rc.bottom - rc.top);
            }
        } else {
            lineLen = *lineIter++;
        }
        if (n == lineLen)
            fmt = flags;
        drawn = DrawTextW(hdc, p, (int)lineLen, rect, fmt);
        rect->top += (int)lineHeight;
        n -= lineLen;
        p += lineLen;
    }
}

/* ========== PECMD_ResampleDibBoxFilter @0x1400ed278 ==========
 * DIB 双线性缩放：标准双线性插值写入目标 DIB。
 * TODO(verify): 与原整数定点权重实现的边界差异。
 */
uint64_t PECMD_ResampleDibBoxFilter(HDC hdc, HBITMAP srcBmp, uint64_t param3, UINT param4,
                                HBITMAP dstBmp, uint64_t dstW, uint64_t dstH)
{
    BITMAPINFO bmiSrc;
    BITMAPINFO bmiDst;
    UINT cLines = (UINT)dstH;
    uint8_t *buf = NULL;
    uint8_t *srcBits;
    uint8_t *dstBits;
    int64_t srcStride, dstStride, srcBpp, dstBpp;
    int64_t srcW, srcH;
    uint64_t x, y;

    (void)param3;
    memset(&bmiSrc, 0, sizeof(bmiSrc));
    bmiSrc.bmiHeader.biSize = 0x28;
    memset(&bmiDst, 0, sizeof(bmiDst));
    bmiDst.bmiHeader.biSize = 0x28;

    GetDIBits(hdc, srcBmp, 0, param4, NULL, &bmiSrc, 0);
    GetDIBits(hdc, dstBmp, 0, cLines, NULL, &bmiDst, 0);
    bmiSrc.bmiHeader.biCompression = 0;
    bmiDst.bmiHeader.biCompression = 0;
    srcW = bmiSrc.bmiHeader.biWidth;
    srcH = bmiSrc.bmiHeader.biHeight;
    srcBpp = bmiSrc.bmiHeader.biBitCount >> 3;
    dstBpp = bmiDst.bmiHeader.biBitCount >> 3;
    srcStride = (int64_t)((bmiSrc.bmiHeader.biWidth * bmiSrc.bmiHeader.biBitCount + 0x1f) >> 5) * 4;
    dstStride = (int64_t)(((uint64_t)bmiDst.bmiHeader.biBitCount * dstW + 0x1f) >> 5) << 2;

    {
        int64_t srcBytes = srcStride * srcH;
        int64_t dstBytes = dstStride * cLines;
        FUN_1400633A8((void **)&buf, srcBytes + dstBytes + 0x1000);
        srcBits = buf + 0x10;
        dstBits = buf + 0x10 + srcBytes;
        memset(dstBits, 0, (size_t)dstBytes);
        GetDIBits(hdc, srcBmp, 0, (UINT)srcH, srcBits, &bmiSrc, 0);
    }

    for (y = 0; y < dstH; y++) {
        uint8_t *dstRow = dstBits + y * dstStride;
        double sy = ((double)y + 0.5) * (double)srcH / (double)dstH - 0.5;
        int y0 = (int)sy;
        double fy = sy - (double)y0;
        if (y0 < 0) { y0 = 0; fy = 0.0; }
        if (y0 >= (int)srcH - 1) { y0 = (int)srcH - 2; fy = 1.0; }
        if ((int)srcH < 2) y0 = 0;
        int y1 = y0 + 1;
        for (x = 0; x < dstW; x++) {
            uint8_t *d = dstRow + x * dstBpp + 2;
            double sx = ((double)x + 0.5) * (double)srcW / (double)dstW - 0.5;
            int x0 = (int)sx;
            double fx = sx - (double)x0;
            if (x0 < 0) { x0 = 0; fx = 0.0; }
            if (x0 >= (int)srcW - 1) { x0 = (int)srcW - 2; fx = 1.0; }
            if ((int)srcW < 2) x0 = 0;
            int x1 = x0 + 1;
            uint8_t *p00 = srcBits + (int64_t)y0 * srcStride + (int64_t)x0 * srcBpp + 2;
            uint8_t *p01 = srcBits + (int64_t)y0 * srcStride + (int64_t)x1 * srcBpp + 2;
            uint8_t *p10 = srcBits + (int64_t)y1 * srcStride + (int64_t)x0 * srcBpp + 2;
            uint8_t *p11 = srcBits + (int64_t)y1 * srcStride + (int64_t)x1 * srcBpp + 2;
            int i;
            for (i = 0; i < 3; i++) {
                double v = (p00[i] * (1.0 - fx) + p01[i] * fx) * (1.0 - fy) +
                           (p10[i] * (1.0 - fx) + p11[i] * fx) * fy;
                int vi = (int)(v + 0.5);
                if (vi < 0) vi = 0;
                if (vi > 255) vi = 255;
                d[i] = (uint8_t)vi;
            }
        }
    }

    bmiDst.bmiHeader.biWidth = (LONG)dstW;
    bmiDst.bmiHeader.biHeight = (LONG)cLines;
    SetDIBits(hdc, dstBmp, 0, cLines, dstBits, &bmiDst, 0);
    PECMD_FreeStrBuf((WCHAR **)&buf);
    return 0;
}

/* ========== PECMD_ScrollHostMsgDispatch @0x1400fc8ec ==========
 * 滚动控件消息处理：滚轮/滚动信息查询与设置，未命中走映射 + GDI 兜底。
 * TODO(verify): 0x462 参数数组与滚动范围细节。
 */
POINT PECMD_ScrollHostMsgDispatch(int64_t obj, uint32_t msg, int64_t wParam,
                                  uint64_t *lParam)
{
    uint8_t *b = (uint8_t *)(uintptr_t)obj;
    POINT result = {0, 0};
    HWND child = *(HWND *)(b + 0xe8);
    HWND top = 0;
    int count;
    uint16_t r;
    uint64_t out = 0;
    POINT mapOut = {0, 0};
    POINT defRet;

    if (msg == 0x45e) {
        result.x = 0x20000;
        return result;
    }
    if (msg == 0xbc19) {
        return *(POINT *)(b + OBJ_LINK);
    }
    if (child != 0 && lParam != (uint64_t *)(uintptr_t)child &&
        (msg == 0xbd14 || msg == 0xbd15)) {
        SendMessageW(child, msg - 0xbc00, (WPARAM)wParam, *(LPARAM *)(b + OBJ_HWND));
        PostMessageW(*(HWND *)(b + OBJ_HWND), 0x45f, 0, 0);
        goto after;
    }

    if (msg == 0x20a || msg == 0x20e) {
        POINT pt;
        RECT rc;
        SCROLLINFO si;
        int delta;
        uint32_t newPos;
        GetCursorPos(&pt);
        GetWindowRect(*(HWND *)(b + OBJ_HWND), &rc);
        if (PtInRect(&rc, pt) != 0) {
            delta = (int16_t)((uint64_t)wParam >> 16);
            {
                int64_t rv = ((int64_t (*)(int64_t, uint32_t, int64_t, uint64_t *))
                    (*(void ***)(uintptr_t)obj)[1])(obj, msg, wParam, lParam);
                defRet.x = (LONG)(uint32_t)rv;
                defRet.y = (LONG)(uint32_t)((uint64_t)rv >> 32);
            }
            if (delta == 0)
                return defRet;
            memset(&si, 0, sizeof(si));
            si.cbSize = 0x1c;
            si.fMask = 0x17;
            if (GetScrollInfo(*(HWND *)(b + OBJ_HWND), 2, &si) != 0) {
                newPos = (uint32_t)(si.nPos - delta);
                if ((int64_t)(int32_t)newPos < 0)
                    newPos = 0;
                if ((int64_t)(si.nMax - si.nPage) < (int64_t)(int32_t)newPos)
                    newPos = (uint32_t)(si.nMax - si.nPage);
                SetScrollPos(*(HWND *)(b + OBJ_HWND), 2, (int)newPos, 1);
                PostMessageW(*(HWND *)(b + OBJ_HWND), 0x45f, 0, 0);
                if (child != 0) {
                    if (lParam != (uint64_t *)(uintptr_t)child) {
                        SendMessageW(child, 0x115 - (msg != 0x20a), 5, *(LPARAM *)(b + OBJ_HWND));
                        return defRet;
                    }
                }
                return defRet;
            }
            return defRet;
        }
        goto dispatch;
    } else if (msg == 0x45f) {
        if (*(int32_t *)(b + OBJ_COLOR) < 0)
            goto dispatch;
        *(uint64_t *)(b + OBJ_SUBWND) |= 1;
        PostMessageW(*(HWND *)(b + OBJ_HWND), 0x463, 0, 0);
    } else if (msg == 0x463) {
        if ((*(uint64_t *)(b + OBJ_SUBWND) & 1) == 0)
            goto dispatch;
        if (*(int32_t *)(b + OBJ_COLOR) >= 0)
            InvalidateRect(*(HWND *)(b + OBJ_HWND), NULL, 1);
        *(uint64_t *)(b + OBJ_SUBWND) &= ~1ULL;
    } else if (msg == 0x462) {
        if (wParam != obj) {
            PECMD_SetWindowTheme(*(uint64_t *)(b + OBJ_HWND));
            goto dispatch;
        }
        if (lParam[0] == 0x3f) {
            int minV, maxV;
            SCROLLINFO si;
            lParam[4] = 0;
            lParam[6] = 0;
            lParam[8] = 0;
            lParam[2] = (uint64_t)GetScrollPos(*(HWND *)(b + OBJ_HWND), 2);
            GetScrollRange(*(HWND *)(b + OBJ_HWND), 2, (int *)&lParam[4], (int *)&lParam[6]);
            memset(&si, 0, sizeof(si));
            si.cbSize = 0x1c;
            si.fMask = 2;
            if (GetScrollInfo(*(HWND *)(b + OBJ_HWND), 2, &si) != 0)
                lParam[8] = si.nPage;
            else
                lParam[8] = 0xfffff000;
            (void)minV; (void)maxV;
            return result;
        }
        if ((lParam[0] & 6) != 0) {
            int lo = 0, hi = 0;
            GetScrollRange(*(HWND *)(b + OBJ_HWND), 2, &lo, &hi);
            if ((lParam[0] & 2) == 0)
                lParam[4] = (uint64_t)(uint32_t)lo;
            if ((lParam[0] & 4) == 0) {
                lParam[6] = (uint64_t)(uint32_t)lo;
                lParam[7] = (uint64_t)(uint32_t)hi;
            }
            if ((lParam[0] & 1) == 0) {
                lParam[2] = (uint64_t)GetScrollPos(*(HWND *)(b + OBJ_HWND), 2);
                lParam[0] |= 1;
            }
            SetScrollRange(*(HWND *)(b + OBJ_HWND), 2, (int)lParam[4], (int)lParam[6], 1);
        }
        if ((lParam[0] & 1) == 0) {
            if ((lParam[0] & 8) == 0)
                return result;
            {
                SCROLLINFO si;
                memset(&si, 0, sizeof(si));
                si.cbSize = 0x1c;
                si.fMask = 2;
                si.nPage = (int)lParam[8];
                SetScrollInfo(*(HWND *)(b + OBJ_HWND), 2, &si, 1);
            }
            return result;
        }
        SetScrollPos(*(HWND *)(b + OBJ_HWND), 2, (int)lParam[2], 1);
    } else if (msg == 0x44a && *(int64_t *)(b + 0xe0) != 0) {
        goto after;
    } else {
        goto dispatch;
    }
after:
    if (*(int64_t *)(b + 0xe0) != 0) {
        GetScrollPos(*(HWND *)(b + OBJ_HWND), 2);
        PECMD_FormatOutput(*(int64_t *)(b + 0xe0));
        PECMD_ProcessScriptBlock(*(uint64_t *)(*(int64_t *)(b + 0xe0) + 0x50),
                      *(uint64_t *)(*(int64_t *)(b + 0xe0) + 0x28), NULL, NULL, NULL);
    }
    result.x = 1;
    return result;

dispatch:
    count = *(int *)(*(int64_t *)(b + 0xd8) + 0x10);
    if (count > 0)
        top = FUN_1400E5788(*(HWND *)(b + OBJ_HWND));
    if (top != 0)
        SendMessageW(top, 0x450, 4, 0x5aa555aa);
    if (count > 0) {
        out = 0;
        r = FUN_1400F172C(*(int64_t **)(b + 0xd8), (int)msg, (uint64_t)wParam,
                                 lParam, *(int64_t *)(b + OBJ_HWND), 1, &out);
        mapOut.x = (LONG)(uint32_t)out;
        mapOut.y = (LONG)(uint32_t)(out >> 32);
        if ((r & 4) != 0) {
            if (top != 0)
                PostMessageW(top, 0x451, 0x204, -0x55aaa55b);
            return mapOut;
        }
    }
    {
        {
            int64_t rv = FUN_1400E5B0C(obj, msg, wParam, (int64_t *)lParam);
            POINT pv;
            if (count > 0)
                FUN_1400F172C(*(int64_t **)(b + 0xd8), (int)msg, (uint64_t)wParam,
                                     lParam, *(int64_t *)(b + OBJ_HWND), 2, NULL);
            if (top != 0)
                PostMessageW(top, 0x451, 0x204, -0x55aaa55b);
            pv.x = (LONG)(uint32_t)rv;
            pv.y = (LONG)(uint32_t)((uint64_t)rv >> 32);
            return pv;
        }
    }
}

/* ========== PECMD_DrawRadioButtonFace @0x1400ef14c ==========
 * 绘制单选/复选按钮图形：先复制背景，再画圆/框、焦点框和文本。
 * TODO(verify): 图形样式位与 0x110 内嵌对象分支。
 */
void PECMD_DrawRadioButtonFace(int64_t obj, int64_t paintInfo)
{
    uint8_t *b = (uint8_t *)(uintptr_t)obj;
    LPCWSTR text = NULL;
    RECT rc, draw;
    COLORREF color;
    HDC hdc;
    HDC memDC;
    HBITMAP bmp;
    HBRUSH brush;
    HPEN pen;
    HGDIOBJ oldObj, stock;
    uint32_t flags = *(uint32_t *)(paintInfo + 0x10);
    uint64_t style;
    uint32_t glyphColor;
    int16_t idx;
    uint64_t slot[3];

    PECMD_AllocStrSlot(&text);
    FUN_1400E5730(*(HWND *)(b + OBJ_HWND), (int64_t *)&text);
    idx = *(int16_t *)(b + 0xa2);
    color = *(COLORREF *)(b + OBJ_COLOR);
    if (idx >= 0)
        color = *(COLORREF *)(b + 0x94 + (int64_t)idx * 8);
    if ((int)*(COLORREF *)(b + OBJ_COLOR) < 0) {
        if (idx >= 0)
            color = *(COLORREF *)(b + 0x94 + (int64_t)idx * 8);
        if ((int)color < 0)
            color = GetSysColor(0xf);
    }

    rc.left = *(int *)(paintInfo + 0x28);
    rc.top = *(int *)(paintInfo + 0x2c);
    rc.right = *(int *)(paintInfo + 0x30);
    rc.bottom = *(int *)(paintInfo + 0x34);
    hdc = *(HDC *)(paintInfo + 0x20);
    memDC = CreateCompatibleDC(hdc);
    bmp = CreateCompatibleBitmap(hdc, rc.right - rc.left, rc.bottom - rc.top);
    PECMD_SelectObjectSlot_b028(slot, memDC, (HGDIOBJ)bmp);
    BitBlt(memDC, 0, 0, rc.right - rc.left, rc.bottom - rc.top, hdc, 0, 0, 0xcc0020);

    style = SendMessageW(*(HWND *)(b + OBJ_HWND), 0xf2, 0, 0);
    glyphColor = *(uint32_t *)(b + 0xa8);
    draw = rc;
    if ((int)glyphColor < 0 && *(int16_t *)(b + 0xa2) >= 0)
        glyphColor = *(uint32_t *)(b + 0x90 + (int64_t)*(int16_t *)(b + 0xa2) * 8);
    brush = CreateSolidBrush(color);
    PECMD_SelectObjectSlot_b028(slot, memDC, (HGDIOBJ)brush);
    if ((style & 4) == 0) {
        rc.bottom -= 2;
        rc.right -= 2;
    } else {
        rc.top += 2;
        rc.left += 2;
    }

    if ((*(uint8_t *)(b + 0xe8) & 6) == 0) {
        if (color != 0xffffffff)
            Ellipse(memDC, draw.left, draw.top, draw.right, draw.bottom);
        stock = GetStockObject(5);
        SelectObject(memDC, stock);
        pen = CreatePen(0, 0, GetSysColor(0x14));
        PECMD_SelectObjectSlot_b028(slot, memDC, (HGDIOBJ)pen);
        Ellipse(memDC, rc.left, rc.top, rc.right, rc.bottom);
        rc.top += 2;
        rc.left += 2;
        rc.bottom -= 2;
        rc.right -= 2;
        pen = CreatePen(0, 0, GetSysColor(0x15));
        PECMD_SelectObjectSlot_b028(slot, memDC, (HGDIOBJ)pen);
        Ellipse(memDC, rc.left, rc.top, rc.right, rc.bottom);
        PECMD_RestoreAndDeleteObject(slot);
        PECMD_RestoreAndDeleteObject(slot);
    } else {
        if ((*(uint8_t *)(b + 0xe8) & 1) != 0) {
            pen = CreatePen(0, 0, color);
            PECMD_SelectObjectSlot_b028(slot, memDC, (HGDIOBJ)pen);
            Ellipse(memDC, draw.left, draw.top, draw.right, draw.bottom);
            PECMD_RestoreAndDeleteObject(slot);
        } else if (color != 0xffffffff) {
            PECMD_FillRectColor(memDC, &draw, color);
        }
    }
    PECMD_RestoreAndDeleteObject(slot);

    oldObj = (HGDIOBJ)0;
    if (*(int64_t *)(b + 0x110) == 0) {
        SetBkMode(memDC, 1);
        if (-1 < (int)glyphColor) {
            if (!IsWindowEnabled(*(HWND *)(b + OBJ_HWND))) {
                DWORD sys = GetSysColor(0x16);
                glyphColor = ((int)((glyphColor >> 8 & 0xff) + (sys >> 8 & 0xff)) >> 1) << 8 |
                             ((int)((sys >> 16 & 0xffU) + (glyphColor >> 16 & 0xffU)) >> 1) << 16 |
                             (int)((glyphColor & 0xff) + (sys & 0xff)) >> 1;
            }
            SetTextColor(memDC, glyphColor);
        }
        {
            HGDIOBJ font = (HGDIOBJ)*(uintptr_t *)(b + 0x70);
            if (font == 0)
                font = (HGDIOBJ)FUN_1400E5890(obj);
            if (font != 0)
                oldObj = SelectObject(memDC, font);
        }
        PECMD_DrawVertCenteredText(memDC, text, -1, &rc, 1);
        if (oldObj != 0)
            SelectObject(memDC, oldObj);
        if (*(int64_t *)(b + 0x110) == 0)
            goto done;
    }
    PECMD_DrawItemContent(*(int64_t *)(b + 0x110), memDC, obj, *(int64_t *)(b + 0x110));
done:
    if ((*(uint8_t *)(b + 0xe8) & 4) != 0 && (flags & 0x10) != 0)
        DrawFocusRect(memDC, &draw);
    BitBlt(hdc, 0, 0, draw.right - draw.left, draw.bottom - draw.top, memDC, 0, 0, 0xcc0020);
    DeleteDC(memDC);
    PECMD_RestoreAndDeleteObject(slot);
    PECMD_FreeStrBuf((WCHAR **)&text);
}

/* ========== PECMD_ListMouseDownGroupSelect @0x1400f69b8 ==========
 * 列表鼠标按下处理：命中测试、滚动命中、选区更新与失效。
 * TODO(verify): local_b4 原反编译未初始化，按 0 处理。
 */
void PECMD_ListMouseDownGroupSelect(int64_t *obj, uint32_t msg, uint64_t lParam)
{
    uint8_t *b = (uint8_t *)obj;
    int64_t lVar14 = 0, lVar11 = 0, lVar9 = 0;
    uint32_t uVar2;
    bool bVar4, bVar5;
    int iVar13 = -3;
    int local_b4 = 0;
    POINT pt = { -3, -3 };
    int local_c8[2] = { 0xfffffffd, 0 };
    LONG LVar6;
    int iVar10;
    WPARAM wParam;
    LRESULT lr;
    RECT rc;
    WPARAM WVar12;
    uint8_t buf2[0x18] = {0};

    if (obj[0x6e] > 0)
        lVar14 = obj[0x6d];
    if (obj[0x74] > 0)
        lVar11 = obj[0x73];
    uVar2 = *(uint32_t *)(obj[0x1a] + 0x74);
    bVar4 = ((uVar2 >> 0x19) & 1) != 0 && lVar14 != 0;
    bVar5 = ((uVar2 >> 0x19) & 1) != 0 && ((uVar2 >> 0x10) & 1) != 0;

    PECMD_ListSubItemHitTest((int64_t)obj, &pt.x, local_c8);
    LVar6 = pt.x;
    if (obj[0x71] > 0)
        lVar9 = obj[0x70];
    wParam = (WPARAM)pt.x;
    iVar10 = pt.x;

    if (lVar11 != 0) {
        if (pt.x >= 0) {
            iVar10 = (int)*(int64_t *)(lVar11 + *(int64_t *)(lVar9 + wParam * 8) * 8);
            goto after_hit;
        }
        if ((*(uint16_t *)(b + 0x3f8) & 0x100) != 0)
            return;
        if (pt.x < 0)
            goto after_hit2;
    } else {
        if (pt.x < 0)
            goto after_hit2;
    }
    lr = SendMessageW((HWND)obj[4], 0x102c, (WPARAM)iVar10, 4);
    if (lr != 0)
        return;

after_hit:
    if (pt.x < 0)
        goto after_hit2;
after_hit2:
    if (((*(uint32_t *)(obj[0x1a] + 0x74) >> 10) & 1) != 0 && LVar6 >= 0) {
        uint8_t buf[0x18] = {0};
        GetCursorPos(&pt);
        ScreenToClient((HWND)obj[4], &pt);
        *(uint32_t *)(buf + 0) = (uint32_t)((uint16_t)pt.y << 16 | (uint16_t)pt.x);
        *(uint32_t *)(buf + 4) = 4;
        lr = SendMessageW((HWND)obj[4], 0x1039, 0, (LPARAM)buf);
        if (lr >= 0) {
            rc.left = rc.top = rc.right = rc.bottom = 0;
            lr = SendMessageW((HWND)obj[4], 0x1038, (WPARAM)local_b4, (LPARAM)&rc);
            if (lr != 0) {
                PECMD_ListGetItemTextData((int64_t)obj, &rc.left, local_b4);
                if (pt.x <= rc.right - 2 && rc.left + 2 <= pt.x)
                    iVar13 = local_b4;
            }
        }
    }

    WVar12 = wParam;
    if (bVar4 && (int64_t)wParam >= 0 && *(int16_t *)(lVar14 + wParam * 2) < 0)
        WVar12 = wParam + (WPARAM)(int64_t)*(int16_t *)(lVar14 + wParam * 2);

    if (!bVar5) {
        int hi = (int)(lParam >> 32);
        uint32_t lo = (uint32_t)lParam;
        (*(void (**)(uint64_t, uint32_t, uint32_t, uint64_t))(*obj + 8))(
            obj[4], 0x201, msg, (uint64_t)(int)((hi << 16) | lo));
    }

    if (bVar4) {
        int16_t s;
        LONG top;
        if ((int64_t)WVar12 >= 0) {
            s = *(int16_t *)(lVar14 + WVar12 * 2);
            rc.left = 0;
            SendMessageW((HWND)obj[4], 0x100e, (WPARAM)(int64_t)WVar12, (LPARAM)&rc);
            top = rc.top;
            rc.left = 0;
            SendMessageW((HWND)obj[4], 0x100e,
                         (WPARAM)(int64_t)(s - 1 + (int)WVar12), (LPARAM)&rc);
            rc.top = top;
            InvalidateRect((HWND)obj[4], &rc, 1);
        }
        {
            WPARAM old = obj[0x7e];
            if ((int64_t)old >= 0 && old != WVar12) {
                s = *(int16_t *)(lVar14 + old * 2);
                rc.left = 0;
                SendMessageW((HWND)obj[4], 0x100e, old, (LPARAM)&rc);
                top = rc.top;
                rc.left = 0;
                SendMessageW((HWND)obj[4], 0x100e,
                             (WPARAM)(int64_t)(s - 1 + (int)old), (LPARAM)&rc);
                rc.top = top;
                InvalidateRect((HWND)obj[4], &rc, 1);
            }
        }
    }

    if (iVar13 >= 0) {
        lr = SendMessageW((HWND)obj[4], 0x102c, (WPARAM)iVar13, 0xf000);
        *(uint32_t *)(buf2 + 0x10) = (2 - (uint32_t)((uint32_t)lr >> 0xc != 1)) * 0x1000;
        SendMessageW((HWND)obj[4], 0x102b, (WPARAM)iVar13, (LPARAM)buf2);
    }

    if (LVar6 >= 0) {
        obj[0x7e] = WVar12;
        if ((*(uint8_t *)(b + 0x3f8) & 0x10) != 0) {
            if ((int8_t)*(uint8_t *)(b + 0x3f8) < 0) {
                *(uint32_t *)(buf2 + 0x10) = 3;
                *(uint32_t *)(buf2 + 0x14) = 0;
                SendMessageW((HWND)obj[4], 0x102b, wParam, (LPARAM)buf2);
            }
            if (obj[0x71] > 0)
                lVar9 = obj[0x70];
            if (lVar9 != 0)
                WVar12 = *(WPARAM *)(lVar9 + WVar12 * 8);
            PECMD_TableSetHoverIdx((int64_t)obj, (int)WVar12);
        }
    }
}

/* ========== PECMD_DrawIconScaled @0x1400ede48 ==========
 * 图标缩放绘制（简化版）：先缩放到临时 DC，再拷回目标 DC。
 * TODO(verify): 原实现含 GDI+ 路径与隐藏栈参数，此处按回退路径近似。
 */
uint32_t PECMD_DrawIconScaled(HDC hdc, HICON icon, uint64_t param3, int param4,
                              uint64_t srcW, uint64_t srcH, uint32_t param7,
                              uint32_t param8, uint64_t dstW, uint64_t dstH,
                              COLORREF color)
{
    HDC mem;
    HBITMAP bmp;
    HGDIOBJ old;
    int w, h;
    int srcX = (int)param3;
    int srcY = param4;

    (void)param7;
    (void)param8;
    w = (int)(dstW & 0x7fffffffffffffffULL);
    h = (int)(dstH & 0x7fffffffffffffffULL);
    if (w < (int)srcW)
        w = (int)srcW;
    if (h < (int)srcH)
        h = (int)srcH;

    mem = CreateCompatibleDC(hdc);
    bmp = CreateCompatibleBitmap(hdc, w, h);
    old = SelectObject(mem, bmp);
    if ((int64_t)dstH < 0) {
        RECT rc = {0, 0, w, h};
        if ((int)color < 0) {
            RECT rc2 = {0, 0, w, h};
            ExtTextOutW(mem, 0, 0, 2, &rc2, NULL, 0, NULL);
        } else {
            HBRUSH br = CreateSolidBrush(color);
            FillRect(mem, &rc, br);
            DeleteObject(br);
        }
    } else {
        StretchBlt(mem, 0, 0, w, h, hdc, srcX, srcY, (int)srcW, (int)srcH, 0xcc0020);
    }
    if ((int64_t)dstW < 0)
        DrawIconEx(mem, 0, 0, icon, w, h, 0, (HBRUSH)0, 3);
    else
        DrawIconEx(mem, 0, 0, icon, w, h, 0, (HBRUSH)0, 3);
    BitBlt(hdc, srcX, srcY, (int)srcW, (int)srcH, mem, 0, 0, 0xcc0020);
    SelectObject(mem, old);
    DeleteObject(bmp);
    DeleteDC(mem);
    return 0;
}

/* ========== FUN_1400F2384 @0x1400f2384 ==========
 * 解析 "COMMAND/NOTIFY/数字[:参数]" 消息映射串并注册/删除表项。
 * TODO(verify): 与原始解析的边界分支差异。
 */
int FUN_1400F2384(int64_t obj, LPCWSTR text, int64_t *script,
                             int64_t param4, int msg, uint16_t flags)
{
    uint8_t *b = (uint8_t *)(uintptr_t)obj;
    WCHAR *p = (WCHAR *)text;
    WCHAR *name = NULL;
    WCHAR *expanded = NULL;
    WCHAR *q;
    int extra = 0;
    int extraVal = 0;
    int64_t extra64 = 0;
    int count;
    int i;
    int found = -1;
    uint16_t uFlags = flags;

    if (msg < 1) {
        WCHAR *scan = p;
        while (*scan != L'\0' && *scan != L':') {
            if (*scan == L'%')
                break;
            scan++;
        }
        if (*scan == L'%') {
            if ((scan[1] == L'&' && scan[2] == L':') ||
                (*(char *)((uint8_t *)script + 0xd) != 0 && scan[1] == L':')) {
                WCHAR *q2 = scan + 2;
                while (*q2 != L'\0') {
                    if (*q2 == L'%')
                        break;
                    if (*q2 == L'=' || (*q2 > 8 && *q2 < 0xe) || *q2 == L' ')
                        break;
                    q2++;
                }
                if (*q2 == L'%')
                    p = q2 + 1;
            }
        }
        FUN_1400F429C(&p, L':');
        PECMD_StrBldCopyWideN(&name, text, (int64_t)(p - text) >> 1);
        PECMD_AllocStrSlot(&expanded);
        q = name;
        FUN_14007BF44(script, q, &expanded, 0, 1);
        q = expanded;
        FUN_14005B154(&q);
        if (*p != L'\0') {
            p++;
            FUN_14005B154(&p);
        }
        while (*q != L'\0' && *q != L'#' && !(9 < (WCHAR)(*q + 0xff10))) {
            if (*q == L'_')
                uFlags = 2;
            else if (*q == L'$')
                uFlags = 5;
            else if (*q == L'*')
                extra = 0x10;
            else if (*q == L'+')
                uFlags = 0x80;
            else
                break;
            q++;
        }
        uFlags = (uint16_t)(uFlags | (uint16_t)extra);
        if (*q == L'#') {
            q++;
            extra = 0x5000;
        }
        if (FUN_14005C788("COMMAND", q, 7) != 0) {
            if (FUN_14005C788("NOTIFY", q, 6) != 0) {
                if (!PECMD_ParseUIntValue(&q, &msg))
                    msg = 0;
                msg += extra;
            } else {
                msg = 0x4e;
                uFlags |= 0x200;
                q += 6;
            }
        } else {
            msg = 0x111;
            uFlags |= 0x100;
            q += 7;
        }
        if (*q == L'=') {
            q++;
            if (*q == L'#') {
                extra = 0x5000;
                q++;
            }
            PECMD_ParseUIntValue(&q, &msg);
        }
        if (*q == L':') {
            extra64 = 0;
        } else if (*q == L'#') {
            q++;
            PECMD_ParseNumSkipChar_01f8((int64_t *)&q, &extraVal);
            if (extraVal > 0)
                extra64 = extraVal;
        }
        if (msg <= 0) {
            PECMD_FreeStrBuf(&expanded);
            PECMD_FreeStrBuf(&name);
            return -0x7ff8ffa9;
        }
        PECMD_FreeStrBuf(&expanded);
        PECMD_FreeStrBuf(&name);
    }

    count = *(int32_t *)(b + 0x10);
    {
        int64_t base = *(int64_t *)(b + 8);
        for (i = 0; i < count; i++) {
            uint8_t *e = (uint8_t *)(base + (int64_t)i * 0x28);
            if (*(int64_t *)(e + 0) == param4 &&
                *(int *)(e + 8) == msg &&
                *(uint16_t *)(e + 0x18) == uFlags &&
                *(int64_t *)(e + 0x20) == extra64 &&
                *(int *)(e + 0x1c) == extraVal) {
                found = i;
                break;
            }
        }
    }

    if (*p == L'*') {
        if (found >= 0) {
            int64_t base = *(int64_t *)(b + 8);
            uint8_t *e = (uint8_t *)(base + (int64_t)found * 0x28);
            PECMD_HeapFreeWithHeader((void *)(e + 0x10));
            if (found < count - 1) {
                memmove(e, e + 0x28, (size_t)(count - found - 1) * 0x28);
            }
            *(int32_t *)(b + 0x10) = count - 1;
        }
        return 0;
    }

    if (found < 0) {
        int64_t *basePtr = (int64_t *)(b + 8);
        int64_t base = *basePtr;
        if (count == 0 || base == 0) {
            base = (int64_t)(uintptr_t)PECMD_GrowByteBuffer((void **)basePtr,
                                                         (int64_t)(count + 1) * 0x28);
            *basePtr = base;
        } else {
            base = (int64_t)(uintptr_t)PECMD_GrowByteBuffer((void **)basePtr,
                                                         (int64_t)(count + 1) * 0x28);
            *basePtr = base;
        }
        found = count;
        *(int32_t *)(b + 0x10) = count + 1;
    }

    {
        uint8_t *e = (uint8_t *)(*(int64_t *)(b + 8) + (int64_t)found * 0x28);
        *(int64_t *)(e + 0) = param4;
        *(int *)(e + 8) = msg;
        *(uint16_t *)(e + 0x18) = uFlags;
        *(int *)(e + 0x1c) = extraVal;
        *(int64_t *)(e + 0x20) = extra64;
        PECMD_StrBldCopyWide((WCHAR **)(e + 0x10), p);
    }
    return 0;
}

/* ========== PECMD_ControlPaint @0x1400fe610 ==========
 * 控件高级绘制：背景/百分比条/阴影/文本/图标/边框。
 * TODO(verify): 与原始绘制标志的边界差异。
 */
void PECMD_ControlPaint(int64_t obj, HDC hdcIn)
{
    uint8_t *b = (uint8_t *)(uintptr_t)obj;
    HDC hdc = hdcIn;
    HDC memDC = (HDC)0;
    HBITMAP bmp = (HBITMAP)0;
    HGDIOBJ oldBmp = (HGDIOBJ)0;
    HGDIOBJ oldFont = (HGDIOBJ)0;
    LPCWSTR text = NULL;
    RECT rc, rc2;
    LONG x, y;
    int cx, cy;
    uint32_t style;
    bool useMem = true;
    bool shadow = false;
    uint32_t drawFlags;
    COLORREF color;
    uint64_t uVar10 = 0xffffffffffffffffULL;
    uint8_t mode = 0;
    uint8_t a4 = *(uint8_t *)(b + 0xa4);
    uint8_t d1 = *(uint8_t *)(b + 0xd1);
    HICON icon;

    style = (uint32_t)GetWindowLongW(*(HWND *)(b + OBJ_HWND), -0x10);
    if (((style >> 0x1b) & 1) != 0 || (d1 & 0x40) != 0)
        shadow = true;
    GetClientRect(*(HWND *)(b + OBJ_HWND), &rc);
    x = rc.left;
    y = rc.top;
    cx = rc.right - rc.left;
    cy = rc.bottom - rc.top;

    if ((a4 & 0x10) == 0) {
        if ((a4 & 3) != 0 && (*(int64_t *)(b + 0xe8) == 0 || *(int8_t *)(b + 0xa5) == 0))
            PECMD_CaptureParentBackground(obj);
        if (((a4 & 3) != 0 && *(int8_t *)(b + 0xa5) != 0) && *(int64_t *)(b + 0xe8) != 0)
            useMem = true;
        else
            useMem = false;
    }

    if (useMem || (a4 & 0x10) != 0) {
        RECT full = {0, 0, cx, cy};
        rc = full;
        memDC = CreateCompatibleDC(hdcIn);
        bmp = CreateCompatibleBitmap(hdcIn, cx, cy);
        oldBmp = SelectObject(memDC, bmp);
        hdc = memDC;
    }

    if ((a4 & 0x10) != 0) {
        if (*(int16_t *)(b + 0xa2) < 0)
            color = 0x80000000;
        else
            color = *(COLORREF *)(b + 0x94 + (int64_t)*(int16_t *)(b + 0xa2) * 8);
        if ((int)color < 0)
            color = *(COLORREF *)(b + OBJ_COLOR);
        if ((int)color < -1)
            color = GetSysColor(0xf);
        if (-1 < (int)color)
            PECMD_FillRectColor(hdc, &rc, color);
    }

    if (useMem) {
        HDC srcDC = CreateCompatibleDC(hdcIn);
        uint64_t slot[3] = {0};
        PECMD_SelectObjectSlot_b054((int64_t)slot, srcDC, (HGDIOBJ)*(uintptr_t *)(b + 0xe8));
        slot[0] = 0;
        BitBlt(hdc, 0, 0, cx, cy, srcDC, 0, 0, 0xcc0020);
        PECMD_RestoreAndDeleteObject(slot);
        if (srcDC != 0)
            DeleteDC(srcDC);
    }

    *(uint8_t *)(b + 0xa5) = 0;
    if ((d1 & 1) == 0)
        drawFlags = (d1 & 2) != 0 ? 0x42 : 0x40;
    else
        drawFlags = 0x41;
    if ((int8_t)d1 < 0)
        drawFlags |= 0x2010;

    if (*(int16_t *)(b + 0xa2) >= 0)
        uVar10 = (uint64_t)*(int32_t *)(b + 0x90 + (int64_t)*(int16_t *)(b + 0xa2) * 8);
    if (shadow)
        uVar10 = GetSysColor(0x10);
    if (*(int32_t *)(b + 0xa8) >= 0) {
        uVar10 = *(uint32_t *)(b + 0xa8);
        shadow = false;
    }

    if (g_alphaThreshold <= *(float *)(b + 0x50)) {
        PECMD_DrawScaledBarFill(obj, hdc, &rc, *(COLORREF *)(b + 0x54),
                             (int)(int8_t)*(uint8_t *)(b + 0x60) & 8);
        shadow = false;
        drawFlags = 0;
        if (*(int32_t *)(b + 0x5c) >= 0)
            uVar10 = *(uint32_t *)(b + 0x5c);
        mode = *(uint8_t *)(b + 0x60);
        if ((mode & 0x10) != 0)
            drawFlags = (uint32_t)(int8_t)mode & 3;
        mode &= 0x20;
    } else {
        if (*(int32_t *)(b + OBJ_COLOR) >= 0)
            PECMD_FillRectColor(hdc, &rc, *(COLORREF *)(b + OBJ_COLOR));
    }

    PECMD_AllocStrSlot(&text);
    FUN_1400E5730(*(HWND *)(b + OBJ_HWND), (int64_t *)&text);
    if (mode == 0 && ((style >> 9) & 1) != 0) {
        if (StrChrW(text, L'\n') == NULL)
            drawFlags |= 0x24;
        else
            mode = 2;
    }

    {
        HGDIOBJ font = (HGDIOBJ)FUN_1400E5890(obj);
        if (font != 0)
            oldFont = SelectObject(hdc, font);
    }
    SetBkMode(hdc, 1);
    if (mode != 0) {
        rc2 = rc;
        DrawTextW(hdc, text, -1, &rc2, drawFlags | 0x400);
        OffsetRect(&rc, 0, (((rc2.top - rc2.bottom) - rc.top) + rc.bottom) / 2);
    }
    if (shadow) {
        SetTextColor(hdc, 0xf8f8f8);
        OffsetRect(&rc, 1, 1);
        if ((d1 & 0x20) == 0)
            DrawTextW(hdc, text, -1, &rc, drawFlags | 0x40);
        else
            PECMD_DrawWrappedText(hdc, text, -1, &rc, drawFlags | 0x40);
        OffsetRect(&rc, -1, -1);
    }
    if ((int64_t)uVar10 >= 0)
        SetTextColor(hdc, (COLORREF)uVar10);
    if ((d1 & 0x20) == 0)
        DrawTextW(hdc, text, -1, &rc, drawFlags | 0x40);
    else
        PECMD_DrawWrappedText(hdc, text, -1, &rc, drawFlags | 0x40);

    icon = *(HICON *)(b + 0xe0);
    if (icon != 0) {
        int left = rc.left + 2;
        int width = (rc.right - rc.left) - 4;
        int top = rc.top + 2;
        int height = (rc.bottom - rc.top) - 4;
        if (height < width)
            width = height;
        if (width < height)
            height = width;
        if ((d1 & 8) != 0) {
            left = rc.left + width / 2;
            top = height / 2 + rc.top;
        }
        DrawIconEx(hdc, left, top, icon, width, height, 0, (HBRUSH)0, 3);
    }

    if (*(int8_t *)(b + OBJ_LINK) != 0) {
        UINT edge = *(int8_t *)(b + 0xad) > 0 ? 10 : 5;
        DrawEdge(hdc, &rc, edge, 0xf);
    }
    if (oldFont != 0)
        SelectObject(hdc, oldFont);

    if (useMem || (a4 & 0x10) != 0) {
        BitBlt(hdcIn, x, y, cx, cy, hdc, 0, 0, 0xcc0020);
        SelectObject(hdc, oldBmp);
        DeleteObject(bmp);
    }
    PECMD_FreeStrBuf((WCHAR **)&text);
    if (memDC != 0)
        DeleteDC(memDC);
}

/* ========== PECMD_TreeDumpItemsText @0x1400ffb64 ==========
 * 树路径格式化到字符串缓冲（简化版）：递归输出 HTREEITEM 地址。
 * TODO(verify): 与原始 flags/分隔符/文本槽语义差异。
 */
uint64_t PECMD_TreeDumpItemsText(int64_t obj, uint16_t *path, int64_t *out,
                                    LPCWSTR prefix, LPCWSTR suffix, uint64_t flags)
{
    uint64_t hItem;
    uint64_t result = 0;
    int64_t off;
    int written;
    int preLen = 0, sufLen = 0;
    HWND hwnd = *(HWND *)(obj + OBJ_HWND);
    bool first = true;

    if (((uintptr_t)path & 1) == 0)
        hItem = PECMD_FindTreeItemByPath(obj, path, NULL);
    else
        hItem = (uint64_t)(uintptr_t)path & 0xfffffffffffffffeULL;
    if (hItem == 0)
        return 0xffffffff80070057ULL;

    if (prefix != NULL)
        preLen = lstrlenW(prefix);
    if (suffix != NULL)
        sufLen = lstrlenW(suffix);
    (void)preLen;
    (void)sufLen;
    off = out[1];
    PECMD_AllocString(out, off + 0x100086);

    while (hItem != 0) {
        if ((flags & 0x200) != 0 && !first) {
            if (off != 0) {
                out[1] = off + 1;
                *(uint16_t *)(*out + off * 2) = 0xb;
                off = out[1];
            }
        }
        {
            WCHAR *p = (WCHAR *)(*out + off * 2);
            if ((flags & 0x20) == 0) {
                written = wsprintfW(p, WSTR("%I64d"), (int64_t)hItem);
            } else {
                *p = L'@';
                written = wsprintfW(p + 1, WSTR("%I64d"), (int64_t)hItem) + 1;
            }
            off += written;
            out[1] = off;
        }
        first = false;
        if ((flags & 0x2000) != 0)
            break;
        if ((flags & 0x100) != 0) {
            uint64_t next = SendMessageW(hwnd, 0x110a, 4, hItem);
            if (next == 0)
                break;
            hItem = next;
        } else {
            uint64_t child = SendMessageW(hwnd, 0x110a, 1, hItem);
            if (child == 0)
                break;
            hItem = child;
        }
    }

    if ((flags & 0x400) != 0 && off != 0) {
        out[1] = off + 1;
        *(uint16_t *)(*out + off * 2) = 0xc;
        off = out[1];
    }
    *(uint16_t *)(*out + off * 2) = 0;
    out[1] = off;
    return result;
}

/* ========== PECMD_ParseCtlLayoutFontSpec @0x1400f61dc ==========
 * 解析字体样式串（"^N*size:margins..."）并更新对象字号/高度。
 * TODO(verify): 与原始完整解析的边界差异。
 */
uint64_t PECMD_ParseCtlLayoutFontSpec(int64_t obj, uint64_t param2, WCHAR *spec)
{
    uint8_t *b = (uint8_t *)(uintptr_t)obj;
    WCHAR *p = spec;
    double size = 0.0;
    double margin1 = 0.0;
    int height = 0;
    int mode = 0;
    uint32_t caret = 0;
    int lf[0x17] = {0};
    double sizeArg = -1.0;
    HFONT font;

    (void)param2;
    while (*p == L'^') {
        p++;
        caret++;
    }
    if (*p == L'*')
        p++;
    PECMD_ParseNumSkipChar_de4c((int64_t *)&p, &size);
    if (size > 0.0)
        sizeArg = size;
    if (*p == L':')
        p++;
    PECMD_ParseNumSkipChar_de4c((int64_t *)&p, &margin1);

    if (sizeArg > 0.0) {
        FUN_1400F4040(obj, (float)sizeArg);
        height = PECMD_DpiConvert(sizeArg);
    }
    if (caret != 0 || sizeArg != 0.0 || margin1 != 0.0) {
        FUN_1400F4064(obj, height, (int)caret);
    }

    font = FUN_1400B1F34(lf, &sizeArg, NULL);
    if (font != (HFONT)0) {
        if (*(HGDIOBJ *)(b + OBJ_CUSTOMMODE) != 0)
            DeleteObject(*(HGDIOBJ *)(b + OBJ_CUSTOMMODE));
        *(HFONT *)(b + OBJ_CUSTOMMODE) = font;
    }
    mode = 0;
    (void)mode;
    return 0;
}

/* ========== PECMD_DrawCheckboxControl @0x1400ee3d0 ==========
 * 绘制复选框/单选框外观（简化版）：画框 + 文本。
 * TODO(verify): 原实现含复杂图形/图标分支，此处近似。
 */
void PECMD_DrawCheckboxControl(int64_t obj, int64_t paintInfo)
{
    uint8_t *b = (uint8_t *)(uintptr_t)obj;
    LPCWSTR text = NULL;
    HDC hdc = *(HDC *)(paintInfo + 0x20);
    RECT rc;
    RECT box;
    uint32_t style;
    bool isCheck = *(int8_t *)(b + 0xd8) == 3;
    uint32_t frameFlags = isCheck ? 4 : 4;

    rc.left = *(int *)(paintInfo + 0x28);
    rc.top = *(int *)(paintInfo + 0x2c);
    rc.right = *(int *)(paintInfo + 0x30);
    rc.bottom = *(int *)(paintInfo + 0x34);
    PECMD_AllocStrSlot(&text);
    FUN_1400E5730(*(HWND *)(b + OBJ_HWND), (int64_t *)&text);
    style = (uint32_t)GetWindowLongW(*(HWND *)(b + OBJ_HWND), -0x10);

    box = rc;
    box.right = box.left + (box.bottom - box.top);
    if (box.right - box.left > 0) {
        DrawFrameControl(hdc, &box, 4, frameFlags);
    }
    rc.left = box.right + 6;
    if ((style & 0x200) != 0) {
        SetTextColor(hdc, GetSysColor(0x10));
    }
    DrawTextW(hdc, text, -1, &rc, 0x24);
    PECMD_FreeStrBuf((WCHAR **)&text);
}

/* ========== PECMD_PaintGradientHeader @0x1400f42fc ==========
 * 表头渐变绘制（简化版）：用对象前景色填充客户区。
 * TODO(verify): 原实现含逐行渐变/文本/图标，此处近似。
 */
void PECMD_PaintGradientHeader(int64_t obj)
{
    uint8_t *b = (uint8_t *)(uintptr_t)obj;
    HWND hwnd = *(HWND *)(b + OBJ_HWND);
    PAINTSTRUCT ps;
    HDC hdc;
    RECT rc;
    HBRUSH br;

    hdc = BeginPaint(hwnd, &ps);
    GetClientRect(hwnd, &rc);
    br = CreateSolidBrush((COLORREF)((uint32_t)*(uint8_t *)(b + 0x11c) << 8 |
                                     (uint32_t)*(uint8_t *)(b + 0x120) << 16 |
                                     (uint32_t)*(uint8_t *)(b + 0x118)));
    FillRect(hdc, &rc, br);
    DeleteObject(br);
    EndPaint(hwnd, &ps);
}

/* ========== FUN_1400E3804 @0x1400e3804 ==========
 * 从注册表加载环境变量（HKCU/HKLM），PATH 特殊合并。
 * TODO(verify): 原始多根路径表与 Favorites 特殊处理未完整还原。
 */
LONG FUN_1400E3804(LPCWSTR path, short *flag)
{
    WCHAR *allPath = NULL;
    WCHAR *expanded = NULL;
    HKEY hKey = (HKEY)0xffffffff80000001;
    LPCWSTR filter = (LPCWSTR)(-(uint64_t)(*flag != 0) & (uint64_t)(uintptr_t)flag);
    WCHAR first = *path;
    uint32_t stage;

    PECMD_AllocStrSlot(&allPath);
    PECMD_AllocStrSlot(&expanded);
    if (first != L'\0') {
        if (FUN_14005C788("HKCU\\", path, 5) != 0) {
            if (FUN_14005C788("HKEY_CURRENT_USER\\", path, 0x13) != 0) {
                if (FUN_14005C788("HKLM\\", path, 5) != 0) {
                    if (FUN_14005C788("HKEY_LOCAL_MACHINE\\", path, 0x13) != 0)
                        goto start;
                    path += 0x13;
                } else {
                    path += 5;
                }
                hKey = (HKEY)0xffffffff80000002;
            } else {
                path += 0x12;
            }
        } else {
            path += 5;
        }
    }
start:
    for (stage = 0; stage < 3; stage++) {
        HKEY key = (HKEY)0;
        DWORD valueCount = 0, nameLen = 0, dataLen = 0;
        LONG r;
        WCHAR *nameBuf = NULL;
        WCHAR *dataBuf = NULL;
        DWORD i;
        RegOpenKeyExW(hKey, path, 0, 0xf003f, &key);
        if (key == (HKEY)0)
            continue;
        r = RegQueryInfoKeyW(key, NULL, NULL, NULL, NULL, NULL, NULL,
                            &valueCount, &nameLen, &dataLen, NULL, NULL);
        if (r != 0) {
            if (key != (HKEY)0)
                RegCloseKey(key);
            PECMD_FreeStrBuf(&expanded);
            PECMD_FreeStrBuf(&allPath);
            return r;
        }
        PECMD_AllocWStringBuffer((WCHAR **)&nameBuf, (int64_t)(nameLen * 4 + 8));
        PECMD_AllocWStringBuffer((WCHAR **)&dataBuf, (int64_t)(dataLen * 4 + 8));
        for (i = 0; i < valueCount; i++) {
            DWORD nlen = nameLen << 2;
            DWORD dlen = dataLen << 2;
            *nameBuf = L'\0';
            r = RegEnumValueW(key, i, nameBuf, &nlen, NULL, NULL, (BYTE *)dataBuf, &dlen);
            if (r == 0 && *nameBuf != L'\0' &&
                (filter == NULL || lstrcmpiW(filter, nameBuf) == 0)) {
                dataBuf[dlen >> 1] = L'\0';
                if (FUN_14005C7C4("PATH", nameBuf)) {
                    if (*dataBuf != L'\0') {
                        if (*allPath != L'\0')
                            FUN_14006375C(&allPath, WSTR(";"));
                        FUN_14006375C(&allPath, dataBuf);
                    }
                } else {
                    FUN_14007A224((void *)(uintptr_t)g_Script, dataBuf, &expanded, -1, 0);
                    SetEnvironmentVariableW(nameBuf, expanded);
                }
            }
        }
        PECMD_FreeStrBuf(&dataBuf);
        PECMD_FreeStrBuf(&nameBuf);
        if (key != (HKEY)0)
            RegCloseKey(key);

        if (stage == 0) {
            hKey = (HKEY)0xffffffff80000002;
            path = WSTR("SYSTEM\\CurrentControlSet\\Control\\Session Manager\\Environment");
        } else if (stage == 1) {
            path = WSTR("Environment");
        }
    }

    if (filter != NULL && FUN_14005C7C4("PATH", filter)) {
        FUN_14007A224((void *)(uintptr_t)g_Script, allPath, &expanded, -1, 0);
        SetEnvironmentVariableW(WSTR("PATH"), expanded);
    }
    PECMD_FreeStrBuf(&expanded);
    PECMD_FreeStrBuf(&allPath);
    return 0;
}
