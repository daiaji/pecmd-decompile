/* ====================================================================
 * core_b3k.c — B3-B6 小函数批 9（主代理并行批次，10 个）
 *
 * 本批新实现函数全部使用人类可读 PECMD_ 名称，原始地址保留在 @0x 注释。
 *
 * 来源: PECMD原始.EXE (x64, ImageBase=0x140000000)
 *   初始化对话框对象 FUN_140053DC8 @0x140053dc8
 *   处理删除命令     FUN_14005B2C0 @0x14005b2c0
 *   安全复制句柄     FUN_14005B494 @0x14005b494
 *   抖动光标         FUN_14005E9AC @0x14005e9ac
 *   展开 %d          FUN_14005EEFC @0x14005eefc
 *   获取盘类型名     FUN_14005FF30 @0x14005ff30
 *   读取流字节       FUN_14005F1B0 @0x14005f1b0
 *   查找 token       FUN_140062EC8 @0x140062ec8
 *   移除数组对象     PECMD_RemoveObjectByKey @0x140061780
 *   加载图像列表 API PECMD_LoadImageListApi @0x140064278
 *
 * 约定:
 *   - 新实现函数使用 PECMD_ 可读名；未实现依赖仍 extern FUN_ + TODO(verify)
 *   - 全局数据符号先用 extern 声明; 完整链接时统一在 core_globals.c 定义
 * ==================================================================== */
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>

#include "pecmd_defs.h"
extern WCHAR **FUN_14005B154(WCHAR **pp); /* @0x14005b154 */

/* ---- 未实现依赖 (extern + TODO(verify)) ---- */
extern int64_t *PECMD_WideToAnsiStr(int64_t *ps, LPCWSTR src, int64_t len,
                              uint64_t cap);
extern DWORD FUN_1400195F0(uint64_t ctx, int64_t timeout, int mode,
                           uint64_t *result);
extern void FUN_14005C828(LPCSTR name, LPCSTR dll, int64_t *out,
                          int64_t *err);
extern int64_t PECMD_VarLookup(void *script, LPCWSTR name, void *scope, int64_t len,
                             void *p5);
extern void FUN_14001E5B0(void *script, LPCWSTR name, LPCWSTR value, int64_t a,
                          int64_t b);
extern HWND PECMD_FindTrayWindow(void);
extern void FUN_1400F172C(void *a, int b, int c, uint64_t *d, void *e, int f,
                          void *g);
extern int64_t PECMD_MatchPatternSwap(LPCWSTR a, LPCWSTR b);

/* ---- 本批引用的全局数据 ---- */
extern uint8_t PTR_FUN_1401234f0[];
extern uint32_t g_msgWndState[2];
extern uint8_t g_fgWndLock;
extern HWND g_ownerWnd;
extern HHOOK g_hookD710;
extern void *DAT_14013d660;
extern uint8_t g_flagD321;

/* ========== FUN_140053DC8 @0x140053dc8 ==========
 * 初始化对话框对象。
 */
uint64_t *FUN_140053DC8(uint64_t *obj, uint32_t id, uint64_t data,
                                 LPCWSTR name1, LPCWSTR name2, LPCWSTR name3,
                                 uint32_t flags1, uint32_t flags2, uint32_t flags3,
                                 uint32_t flags4)
{
    *(uint32_t *)(obj + 1) = id;
    *obj = (uint64_t)(uintptr_t)PTR_FUN_1401234f0;
    FUN_1400702B0((WCHAR **)(obj + 2), name1);
    obj[3] = 0;
    PECMD_WideToAnsiStr((int64_t *)(obj + 3), name1, -1, 0xffffffffffffffffULL);
    FUN_1400702B0((WCHAR **)(obj + 4), name3);
    FUN_1400702B0((WCHAR **)(obj + 5), name2);
    obj[6] = 0;
    obj[7] = 0;
    obj[10] = data;
    *(uint32_t *)(obj + 8) = flags1;
    *(uint32_t *)((uint8_t *)obj + 0x44) = flags2;
    *(uint32_t *)(obj + 9) = flags3;
    *(uint32_t *)((uint8_t *)obj + 0x4c) = flags4;
    obj[0xb] = data;
    obj[0xc] = 0;
    *(uint32_t *)(obj + 0xd) = 0;
    return obj;
}

/* ========== FUN_14005B2C0 @0x14005b2c0 ==========
 * 处理 "*del" 命令。
 */
bool FUN_14005B2C0(uint64_t *obj, LPCWSTR cmd, HWND hwnd)
{
    int iVar1 = lstrcmpiW(cmd, WSTR("*del"));
    if (iVar1 == 0) {
        uint64_t local_58 = 4;
        uint64_t local_50 = 0;
        void *local_40 = &DAT_14013d660;
        uint64_t local_38 = 0;
        uint64_t *local_48 = obj;
        (void)local_50; (void)local_40; (void)local_38; (void)local_48;
        uint8_t local_30[0x28];
        memset(local_30, 0, sizeof(local_30));
        *(uint8_t *)obj[3] = 0;
        *(uint16_t *)obj[2] = 0;
        SendMessageW(hwnd, 0x465, *(WPARAM *)(obj[10] + 0x40), (LPARAM)&local_58);
        (*(void (**)(void *, int))(uintptr_t)*obj)(obj, 1);
    }
    return iVar1 != 0;
}

/* ========== FUN_14005B494 @0x14005b494 ==========
 * 安全复制句柄。
 */
HANDLE FUN_14005B494(HANDLE handle, uint64_t *out)
{
    DWORD local_res18[2] = {0, 0};
    BOOL BVar1 = GetHandleInformation(handle, local_res18);
    if ((BVar1 == 0) && (GetLastError() == 6)) {
        handle = (HANDLE)0;
    } else if (out != NULL) {
        HANDLE local_res20 = 0;
        HANDLE pvVar3 = GetCurrentProcess();
        BVar1 = DuplicateHandle(pvVar3, handle, pvVar3, &local_res20, 0, 0, 2);
        if (BVar1 != 0) {
            pvVar3 = (HANDLE)*out;
            if ((pvVar3 != 0) && (pvVar3 != INVALID_HANDLE_VALUE)) {
                CloseHandle(pvVar3);
            }
            pvVar3 = local_res20;
            if (local_res20 == INVALID_HANDLE_VALUE) {
                pvVar3 = (HANDLE)0;
            }
            *out = (uint64_t)pvVar3;
            handle = local_res20;
        }
    }
    return handle;
}

/* ========== FUN_14005E9AC @0x14005e9ac ==========
 * 在托盘区域抖动光标。
 */
void FUN_14005E9AC(void)
{
    POINT local_res8;
    RECT local_18;
    memset(&local_18, 0, sizeof(local_18));
    local_res8.x = 0;
    local_res8.y = 0;
    HWND hWnd = (HWND)PECMD_FindTrayWindow();
    GetWindowRect(hWnd, &local_18);
    GetCursorPos(&local_res8);
    int X, Y;
    for (X = local_18.left + 1; X < local_18.right - 1; X += 4) {
        for (Y = local_18.top + 1; Y < local_18.bottom - 1; Y += 4) {
            SetCursorPos(X, Y);
            Sleep(0);
        }
    }
    SetCursorPos(local_res8.x, local_res8.y);
}

/* ========== FUN_14005EEFC @0x14005eefc ==========
 * 展开 %d。
 * TODO(verify): wsprintfW(L"%d") 缺实参，按 0 补齐。
 */
LPWSTR FUN_14005EEFC(LPWSTR dst, WCHAR *src)
{
    WCHAR WVar1 = *src;
    do {
        if (WVar1 == L'\0') {
            *dst = L'\0';
            return dst;
        }
        if (WVar1 == L'%') {
            if (src[1] == L'%') {
                *dst = L'%';
                dst++;
            } else {
                if (src[1] != L'd') goto LAB_14005ef73;
                int iVar2 = wsprintfW(dst, WSTR("%d"), 0);
                dst += iVar2;
            }
            src += 2;
        } else {
LAB_14005ef73:
            *dst = WVar1;
            dst++;
            src++;
        }
        WVar1 = *src;
    } while (1);
}

/* ========== FUN_14005FF30 @0x14005ff30 ==========
 * 获取盘类型名称。
 */
LPCWSTR FUN_14005FF30(int type, LPWSTR buf, int detail)
{
    LPCWSTR pwVar1;
    LPCWSTR pwVar2;
    if (type == 0) {
        pwVar1 = WSTR("DRIVE_UNKNOWN");
    } else if (type == 1) {
        pwVar1 = WSTR("DRIVE_NO_ROOT_DIR");
    } else {
        if (type == 2) {
            pwVar2 = WSTR("DRIVE_USBFLASH");
            pwVar1 = WSTR("DRIVE_REMOVABLE");
        } else if (type == 3) {
            pwVar2 = WSTR("DRIVE_USBDISK");
            pwVar1 = WSTR("DRIVE_FIXED");
        } else {
            if (type == 4) {
                return WSTR("DRIVE_REMOTE");
            }
            if (type != 5) {
                if (type != 6) {
                    wsprintfW(buf, WSTR("%d"), 0);
                    return buf;
                }
                return WSTR("DRIVE_RAMDISK");
            }
            pwVar2 = WSTR("DRIVE_CDROMUSB");
            pwVar1 = WSTR("DRIVE_CDROM");
        }
        if (detail == 7) {
            pwVar1 = pwVar2;
        }
    }
    return pwVar1;
}

/* ========== FUN_14005F1B0 @0x14005f1b0 ==========
 * 从流读取一个字节。
 */
bool FUN_14005F1B0(uint64_t *stream, uint8_t *out, uint64_t unused,
                          DWORD *readCount)
{
    (void)unused;
    DWORD local_res8[2] = {0, 0};
    DWORD *pDVar6 = local_res8;
    if (readCount != NULL) {
        pDVar6 = readCount;
    }
    int *piVar1 = (int *)((uint8_t *)stream + 0x24);
    if ((int64_t)stream[3] <= (int64_t)stream[2]) {
        do {
            stream[2] = 0;
            local_res8[0] = 0;
            BOOL BVar4 = ReadFile((HANDLE)*stream, (LPVOID)stream[1], 0x1000,
                                  local_res8, NULL);
            stream[3] = local_res8[0];
            if (((BVar4 == 0) && (GetLastError() == 0x6d)) || (local_res8[0] != 0)) break;
            *piVar1 = *piVar1 - 1;
        } while (-1 < *piVar1);
    }
    int64_t lVar2 = stream[2];
    int64_t lVar3 = stream[3];
    if (lVar3 <= lVar2) {
        *pDVar6 = 0;
    } else {
        stream[2] = lVar2 + 1;
        *out = *(uint8_t *)(lVar2 + stream[1]);
        *pDVar6 = 1;
    }
    return lVar3 > lVar2;
}

/* ========== FUN_140062EC8 @0x140062ec8 ==========
 * 查找 token。
 */
LPCWSTR FUN_140062EC8(LPCWSTR text, LPCWSTR tokens, int len, uint32_t flags)
{
    int iVar1;
    while (1) {
        if ((flags & 0xff00) == 0) {
            iVar1 = StrCmpNIW(text, tokens, len);
            if (iVar1 == 0) {
                if (tokens[len] == L'\0') {
                    return tokens;
                }
                if (tokens[len] == L'&') {
                    return tokens;
                }
            }
            iVar1 = 0;
        } else {
            iVar1 = (int)PECMD_MatchPatternSwap(tokens, text);
        }
        if (iVar1 != 0) {
            return tokens;
        }
        if ((char)flags != '\0') break;
        iVar1 = lstrlenW(tokens);
        tokens += (iVar1 + 1);
        if (*tokens == L'\0') {
            return NULL;
        }
    }
    return NULL;
}

/* ========== PECMD_RemoveObjectByKey @0x140061780 ==========
 * 移除数组中匹配对象。
 */
void PECMD_RemoveObjectByKey(int64_t *arr, int *count, int64_t key)
{
    EnterCriticalSection(&g_csInit);
    int64_t lVar2 = (int64_t)(*count - 1);
    if (-1 < *count - 1) {
        do {
            if ((*(int64_t *)(*arr + lVar2 * 8) != 0) &&
                (*(uint64_t **)(*arr + lVar2 * 8))[10] == (uint64_t)key) {
                uint64_t *puVar1 = *(uint64_t **)(*arr + lVar2 * 8);
                if (puVar1 != NULL) {
                    (*(void (**)(void *, int))(uintptr_t)*puVar1)(puVar1, 1);
                }
                *(uint64_t *)(*arr + lVar2 * 8) = 0;
            }
            lVar2--;
        } while (-1 < lVar2);
    }
    lVar2 = (int64_t)(*count - 1);
    if (-1 < *count - 1) {
        do {
            if (*(int64_t *)(*arr + lVar2 * 8) != 0) break;
            *count = *count - 1;
            lVar2--;
        } while (-1 < lVar2);
    }
    LeaveCriticalSection(&g_csInit);
}

/* ========== PECMD_LoadImageListApi @0x140064278 ==========
 * 加载 ImageList API。
 */
void PECMD_LoadImageListApi(void)
{
    int64_t local_res8[4] = {0, 0, 0, 0};
    if (g_flagD321 == 0) {
        FUN_14005C828("ImageList_Create", "Comctl32.DLL", (int64_t *)(void **)&g_pImageListCreate, local_res8);
        FUN_14005C828("ImageList_Destroy", "Comctl32.DLL", (int64_t *)(void **)&g_pFnCleanup, local_res8);
        FUN_14005C828("ImageList_ReplaceIcon", "Comctl32.DLL", (int64_t *)(void **)&g_pImageListReplaceIcon, local_res8);
        FUN_14005C828("ImageList_Add", "Comctl32.DLL", (int64_t *)(void **)&g_pImageListAdd, local_res8);
        g_flagD321 = (uint8_t)((-(g_pImageListAdd != 0) & 2U) - 1);
    }
}
