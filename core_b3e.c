/* ====================================================================
 * core_b3e.c — B3-B6 小函数批 5 (140040000-1400bffff)
 *
 * 来源: PECMD原始.EXE (x64, ImageBase=0x140000000)
 *   命令分发          PECMD_DispatchCommand @0x140045c44
 *   解析+跳分隔       PECMD_ParseSkipSeparator @0x140054840
 *   对象释放          PECMD_ReleaseObject_b270 @0x14005b270
 *   定时器消息        PECMD_TimerMessage @0x14005bb6c
 *   尾部补空格        PECMD_PadTrailingSpaces @0x14005cc04
 *   大小写前缀匹配    PECMD_PrefixMatchNoCase @0x14005ceec
 *   工作集清理        PECMD_TrimWorkingSet @0x14005d4e4
 *   资源释放          PECMD_ReleaseResources @0x14005d644
 *   前台窗口恢复      PECMD_RestoreForegroundWindow @0x14005e204
 *   回调清理          PECMD_CleanupCallback @0x14005e4ac
 *   读取字节          PECMD_ReadByte_f264 @0x14005f264
 *   错误处理封装      PECMD_ErrorHandlerWrap @0x1400601b8
 *   键表标记          PECMD_MarkKeyTable @0x140060244
 *   路径尾分隔        PECMD_LastPathSeparator @0x14006045c
 *   毫秒时间          PECMD_GetTimeMs @0x140060fd0
 *   纳秒时间          PECMD_GetTimeNs @0x140061020
 *   释放模块          PECMD_FreeModules @0x140061bf4
 *   对象释放          PECMD_ReleaseObject_20a0 @0x1400620a0
 *   定时器+转发       PECMD_TimerAndForward @0x1400620f4
 *   DPI 换算          PECMD_DpiConvert @0x1400628b4
 *   数组释放          PECMD_FreeArray_2e84 @0x140062e84
 *   消息发送          PECMD_SendCtrlMessage_3d00 @0x140063d00
 *   十六进制解析      PECMD_ParseHex_4a34 @0x140064a34
 *   格式化设变量      PECMD_FormatSetVar @0x140066930
 *   控件消息          PECMD_ControlMessage @0x140068ca8
 *   数组追加          PECMD_ArrayAppend @0x14006e8a4
 *   串替换            PECMD_ReplaceStringSlot @0x140070398
 *   命令行处理        PECMD_ProcessCommandLine @0x1400727c4
 *   带符号数字解析    PECMD_ParseSignedNumber @0x140074c10
 *   数组释放          PECMD_FreeArray_ddf8 @0x14007ddf8
 *
 * 约定:
 *   - 未实现内部函数 extern 挂起 + TODO(verify), 不臆造语义
 *   - 全局数据符号先用 extern 声明; 完整链接时统一在 core_globals.c 定义
 * ==================================================================== */
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>

#include "pecmd_defs.h"
extern void FUN_1400629B8(void *script, LPCWSTR key, LPCWSTR value); /* @0x1400629b8 */

/* ---- 未实现依赖 (extern + TODO(verify)) ---- */
extern uint64_t PECMD_ParseDateTimeSpec(int64_t *script, uint64_t cmd, uint32_t flags,
                              uint64_t extra);
extern int64_t PECMD_ExecSubCommand(int64_t *script, WCHAR *cmd, int64_t *out,
                             LPCWSTR extra, uint32_t flags);
extern int64_t PECMD_ExecuteCommand(int64_t *script, LPCWSTR cmd, uint64_t ctx,
                             LPCWSTR extra, uint32_t flags, int64_t a, int b);
extern void FUN_14006764C(int64_t *pp, int64_t *out, int16_t sep1, int16_t sep2);
extern void PECMD_ReleaseGdiImage(int64_t obj);
extern void FUN_1400FD1A8(void *obj);
extern void FUN_14005B7E8(char *s);
extern void FUN_1400FD86C(int64_t *obj, uint32_t msg); /* @0x1400fd86c */
extern uint64_t FUN_140063060(uint32_t *buf);
extern bool PECMD_CheckNetAddress(LPCSTR cmd, uint32_t flags, char *state, char *out,
                          uint64_t ctx, int mode);
extern void FUN_14005CC4C(void);
extern uint64_t FUN_1400745c8(int64_t *pp, uint64_t *out);
extern void *FUN_140070154(LPCWSTR src);
extern bool FUN_14005F1B0(uint64_t *stream, uint8_t *out, uint64_t unused,
                          DWORD *readCount);
extern HANDLE PECMD_OpenLockVolume(int mode, uint64_t a, int64_t b,
                            uint32_t c, WCHAR *d);

/* ---- 本批引用的全局数据 ---- */
extern HWND g_hwndD310;
extern uint32_t g_msgWndState[2];
extern uint8_t g_flagE1B8;
extern uint8_t g_fgWndLock;
extern HWND g_ownerWnd;
extern uint32_t g_guidPtrState;
extern HMODULE g_hOleaut32;
extern void *g_pSysFreeString;
extern uint32_t g_dpi;
extern double g_dpiBase;
extern double g_fontRound;
extern double g_fontMinus0;
extern uint8_t PTR_FUN_140124e50[];
extern uint8_t PTR_FUN_140126060[];
extern uint8_t PTR__purecall_140126050[];

/* ========== PECMD_DispatchCommand @0x140045c44 ==========
 * 按首字符分发命令。
 */
void PECMD_DispatchCommand(int64_t *script, LPCWSTR cmd, int64_t *out)
{
    if (*cmd == L'$') {
        PECMD_ParseDateTimeSpec(script, (uint64_t)(uintptr_t)(cmd + 1), 0, 0);
    } else if (*cmd == L'@') {
        PECMD_ExecSubCommand(script, (WCHAR *)(cmd + 1), out, NULL, 0);
    } else {
        PECMD_ExecuteCommand(script, cmd, (uint64_t)(uintptr_t)out, NULL, 0, 0, 0);
    }
}

/* ========== PECMD_ParseSkipSeparator @0x140054840 ==========
 * 解析后若当前字符为分隔符则前进。
 */
void PECMD_ParseSkipSeparator(int64_t *pp, int64_t *out, int16_t sep1, int16_t sep2)
{
    FUN_14006764C(pp, out, sep1, sep2);
    int16_t *psVar1 = (int16_t *)*pp;
    if ((*psVar1 != 0) && ((*psVar1 == sep1 || (*psVar1 == sep2)))) {
        *pp = (int64_t)(psVar1 + 1);
    }
}

/* ========== PECMD_ReleaseObject_b270 @0x14005b270 ==========
 * 对象释放。
 */
uint64_t *PECMD_ReleaseObject_b270(uint64_t *obj, uint32_t flags)
{
    *obj = (uint64_t)(uintptr_t)PTR_FUN_140124e50;
    PECMD_ReleaseGdiImage((int64_t)obj);
    FUN_14005B104((WCHAR **)(obj + 0x1e));
    FUN_1400FD1A8(obj);
    if ((flags & 1) != 0) {
        free(obj);
    }
    return obj;
}

/* ========== PECMD_TimerMessage @0x14005bb6c ==========
 * 定时器消息处理。
 */
void PECMD_TimerMessage(HWND hwnd, uint64_t wParam, int timerId)
{
    (void)hwnd;
    (void)wParam;
    if (timerId == 0xabb) {
        KillTimer(hwnd, 0xabb);
        SendMessageW(g_hwndD310, 0x411, 0, 0);
        if ((g_msgWndState[0] & 3) != 0) {
            g_msgWndState[1]++;
        }
    }
}

/* ========== PECMD_PadTrailingSpaces @0x14005cc04 ==========
 * 尾部连续空终止转空格并计数。
 */
int PECMD_PadTrailingSpaces(char *s, int len)
{
    char *pcVar2 = s + len - 1;
    int count = 0;
    pcVar2[2] = '\0';
    pcVar2[1] = '\0';
    *pcVar2 = '\0';
    for (; s < pcVar2; pcVar2--) {
        if (*pcVar2 != '\0') goto LAB_14005cc31;
    }
    if (*pcVar2 != '\0') {
LAB_14005cc31:
        pcVar2++;
    }
    for (; s < pcVar2; s++) {
        if (*s == '\0') {
            *s = ' ';
            count++;
        }
    }
    return count;
}

/* ========== PECMD_PrefixMatchNoCase @0x14005ceec ==========
 * 大小写不敏感前缀匹配 (宽 vs ANSI)。
 */
bool PECMD_PrefixMatchNoCase(uint16_t *w, char *a)
{
    uint16_t uVar2 = 0;
    uint16_t uVar3 = 0;
    do {
        char cVar1 = *a;
        if (cVar1 == '\0') {
            return *w == 0;
        }
        uVar3 = *w;
        uVar2 = (uint16_t)(uint8_t)cVar1;
        a++;
        w++;
        if ((uint16_t)(uVar3 - 0x41) < 0x1a) {
            uVar3 |= 0x20;
        }
        if ((uint16_t)((int16_t)cVar1 - 0x41U) < 0x1a) {
            uVar2 = (uint16_t)((int16_t)cVar1 | 0x20);
        }
    } while (uVar3 == uVar2);
    return false;
}

/* ========== PECMD_TrimWorkingSet @0x14005d4e4 ==========
 * 清理进程工作集 (带一次性标志)。
 */
void PECMD_TrimWorkingSet(char force)
{
    if ((g_flagE1B8 == 0) || (force != '\0')) {
        g_flagE1B8 = 1;
        SetProcessWorkingSetSize(GetCurrentProcess(), (size_t)-1, (size_t)-1);
        SetProcessWorkingSetSize(GetCurrentProcess(), (size_t)-1, (size_t)-1);
    }
}

/* ========== PECMD_ReleaseResources @0x14005d644 ==========
 * 释放字符串/图标/模块。
 */
void PECMD_ReleaseResources(int64_t obj)
{
    FUN_14005B7E8((char *)(obj + 0x690));
    if (*(HICON *)(obj + 0xa78) != 0) {
        DestroyIcon(*(HICON *)(obj + 0xa78));
        *(uint64_t *)(obj + 0xa78) = 0;
    }
    if (*(HMODULE *)(obj + 0xa70) != 0) {
        FreeLibrary(*(HMODULE *)(obj + 0xa70));
        *(uint64_t *)(obj + 0xa70) = 0;
    }
}

/* ========== PECMD_RestoreForegroundWindow @0x14005e204 ==========
 * 恢复前台窗口。
 */
void PECMD_RestoreForegroundWindow(void)
{
    if (g_fgWndLock == 0) {
        if (IsWindow(g_ownerWnd)) {
            HWND hWnd = GetForegroundWindow();
            if (!IsWindow(hWnd) || (hWnd != g_ownerWnd)) {
                SetForegroundWindow(g_ownerWnd);
            }
        }
    }
}

/* ========== PECMD_CleanupCallback @0x14005e4ac ==========
 * 回调清理。
 */
void PECMD_CleanupCallback(int64_t *obj)
{
    if ((char)obj[0x24] == 1) {
        EnterCriticalSection(&g_csInit);
        if (obj[0x52] != 0) {
            *(uint64_t *)(obj[0x52] + 0x40) = 0;
        }
        LeaveCriticalSection(&g_csInit);
        void (**fn)(void *, int) = (void (**)(void *, int))(*obj + 0x18);
        (**fn)(obj, 1);
    }
}

/* ========== PECMD_ReadByte_f264 @0x14005f264 ==========
 * 读取一个字节 (带 GetLastError 调用)。
 */
uint64_t PECMD_ReadByte_f264(int64_t stream)
{
    uint8_t local_res8[8] = {0};
    DWORD local_res10[6] = {0};
    bool bVar1 = FUN_14005F1B0((void *)(stream + 8), local_res8, 1, local_res10);
    GetLastError();
    if (!bVar1 || local_res10[0] == 0) {
        return 0xffffffff;
    }
    return (uint64_t)local_res8[0];
}

/* ========== PECMD_ErrorHandlerWrap @0x1400601b8 ==========
 * 错误处理封装。
 */
void PECMD_ErrorHandlerWrap(uint8_t *flags, int mode, uint64_t a, int64_t b,
                   uint32_t c, WCHAR *d)
{
    if (((*flags & 0x11) == 0) || (d != NULL)) {
        PECMD_OpenLockVolume(mode, a, b, c, d);
        *flags = (uint8_t)(*flags | ((d != NULL) + 1U));
    }
}

/* ========== PECMD_MarkKeyTable @0x140060244 ==========
 * 键表标记。
 */
void PECMD_MarkKeyTable(uint16_t key, int64_t table)
{
    int16_t sVar3 = (int16_t)(key - 0x41);
    if (sVar3 < 0x1a) {
        uint32_t *puVar1 = (uint32_t *)((int64_t)sVar3 * 0x220 + 0x10 + table);
        uint64_t uVar2 = (uint64_t)(uint16_t)(0x1a - sVar3);
        do {
            if ((uint32_t)key == *puVar1) {
                *puVar1 |= 0x9000;
            }
            puVar1 += 0x88;
            uVar2--;
        } while (uVar2 != 0);
    }
}

/* ========== PECMD_LastPathSeparator @0x14006045c ==========
 * 找路径最后一个分隔符。
 */
int16_t *PECMD_LastPathSeparator(int16_t *path)
{
    int16_t *psVar3 = NULL;
    if (*path != 0) {
        if ((*path != 0x3a) && (path[1] == 0x3a)) {
            path += 2;
            psVar3 = path - 1;
        }
        for (; *path != 0; path++) {
            if ((*path == 0x5c) || (*path == 0x2f)) {
                psVar3 = path;
            }
        }
    }
    return psVar3;
}

/* ========== PECMD_GetTimeMs @0x140060fd0 ==========
 * 毫秒时间。
 */
int64_t PECMD_GetTimeMs(void)
{
    int64_t counter;
    QueryPerformanceCounter(&counter);
    uint64_t q = (uint64_t)counter;
    int64_t freq = g_QPFreq;
    return (int64_t)(((q - (q / (uint64_t)freq) * (uint64_t)freq) * 1000) /
                     (uint64_t)freq + (q / (uint64_t)freq) * 1000);
}

/* ========== PECMD_GetTimeNs @0x140061020 ==========
 * 纳秒时间。
 */
int64_t PECMD_GetTimeNs(void)
{
    int64_t counter;
    QueryPerformanceCounter(&counter);
    uint64_t q = (uint64_t)counter;
    int64_t freq = g_QPFreq;
    return (int64_t)(((q - (q / (uint64_t)freq) * (uint64_t)freq) * 1000000000ULL) /
                     (uint64_t)freq + (q / (uint64_t)freq) * 1000000000ULL);
}

/* ========== PECMD_FreeModules @0x140061bf4 ==========
 * 释放两个模块并清状态。
 */
void PECMD_FreeModules(void)
{
    if (g_hOle32 != (HMODULE)0) {
        FreeLibrary(g_hOle32);
    }
    g_hOle32 = (HMODULE)0;
    g_guidPtrState = 0;
    if (g_hOleaut32 != (HMODULE)0) {
        FreeLibrary(g_hOleaut32);
    }
    g_hOleaut32 = (HMODULE)0;
    g_pSysFreeString = 0;
}

/* ========== PECMD_ReleaseObject_20a0 @0x1400620a0 ==========
 * 对象释放 (关闭句柄, 挂 purecall)。
 */
uint64_t *PECMD_ReleaseObject_20a0(uint64_t *obj, uint32_t flags)
{
    *obj = (uint64_t)(uintptr_t)PTR_FUN_140126060;
    if ((HANDLE)obj[1] != 0) {
        CloseHandle((HANDLE)obj[1]);
    }
    obj[1] = 0;
    *obj = (uint64_t)(uintptr_t)PTR__purecall_140126050;
    if ((flags & 1) != 0) {
        free(obj);
    }
    return obj;
}

/* ========== PECMD_TimerAndForward @0x1400620f4 ==========
 * 定时器消息 + 转发。
 */
void PECMD_TimerAndForward(int64_t *obj, int64_t timerId)
{
    if (timerId == 0x2712) {
        KillTimer((HWND)obj[4], 0x2712);
        if ((*(uint32_t *)((uint8_t *)obj + 0x1a * 8) >> 0x11 & 1) != 0) {
            PECMD_RestoreForegroundWindow();
        }
    }
    FUN_1400FD86C(obj, (uint32_t)timerId);
}

/* ========== PECMD_DpiConvert @0x1400628b4 ==========
 * DPI 换算。
 */
int PECMD_DpiConvert(double value)
{
    if (g_dpi < 1) {
        return (int)value;
    }
    double dVar1 = ((double)g_dpi * value) / g_dpiBase;
    if (dVar1 < 0.0) {
        dVar1 = g_fontMinus0 - dVar1;
    }
    return (int)(dVar1 + g_fontRound);
}

/* ========== PECMD_FreeArray_2e84 @0x140062e84 ==========
 * 释放对象数组。
 */
void PECMD_FreeArray_2e84(int64_t *arr)
{
    int64_t lVar3 = arr[2];
    int64_t lVar1 = *arr;
    arr[2] = 0;
    while (0 < lVar3) {
        lVar3--;
        int64_t *plVar2 = *(int64_t **)(lVar1 + lVar3 * 8);
        if (plVar2 != NULL) {
            void (**fn)(void *, int) = (void (**)(void *, int))(*plVar2 + 0x18);
            (**fn)(plVar2, 1);
            *(uint64_t *)(lVar1 + lVar3 * 8) = 0;
        }
    }
}

/* ========== PECMD_SendCtrlMessage_3d00 @0x140063d00 ==========
 * 两条消息发送。
 */
void PECMD_SendCtrlMessage_3d00(int64_t obj, LPARAM lParam1, LPARAM lParam2)
{
    SendMessageW(*(HWND *)(obj + OBJ_HWND), 0x407, 0, lParam1);
    SendMessageW(*(HWND *)(obj + OBJ_HWND), 0x408, 1, lParam2);
}

/* ========== PECMD_ParseHex_4a34 @0x140064a34 ==========
 * 十六进制解析 (宽字符)。
 */
int64_t PECMD_ParseHex_4a34(uint16_t *s)
{
    int64_t lVar3 = 0;
    while (((0x2f < *s && (*s < 0x3a)) ||
            ((0x60 < (*s | 0x20) && ((*s | 0x20) < 0x67))))) {
        uint16_t uVar1 = *s++;
        int iVar2;
        if (uVar1 < 0x3a) {
            iVar2 = uVar1 - 0x30;
        } else {
            iVar2 = (uVar1 | 0x20) - 0x57;
        }
        lVar3 = lVar3 * 0x10 + iVar2;
    }
    return lVar3;
}

/* ========== PECMD_FormatSetVar @0x140066930 ==========
 * 格式化后设置变量。
 * TODO(verify): wsprintfW 缺实参, 按 0 补齐。
 */
void PECMD_FormatSetVar(int64_t *script, uint64_t unused, LPCWSTR key, LPCWSTR fmt)
{
    (void)unused;
    WCHAR local_78[56];
    wsprintfW(local_78, fmt, 0);
    FUN_1400629B8(script, key, local_78);
}

/* ========== PECMD_ControlMessage @0x140068ca8 ==========
 * 控件消息。
 */
uint64_t PECMD_ControlMessage(HWND hwnd, uint32_t msg, uint64_t unused, LPARAM lParam)
{
    (void)unused;
    uint32_t local_res10[6] = {0};
    local_res10[0] = msg;
    int *piVar1 = (int *)FUN_140063060(local_res10);
    if (*piVar1 == 1) {
        SendMessageW(hwnd, 0x467, 1, lParam);
    }
    return 0;
}

/* ========== PECMD_ArrayAppend @0x14006e8a4 ==========
 * 数组追加。
 */
void PECMD_ArrayAppend(int64_t arr, int64_t value)
{
    int64_t *plVar1 = (int64_t *)(uintptr_t)PECMD_HeapRealloc(
        *(void **)(arr + 0xe0),
        (size_t)((int64_t)(*(int *)(arr + 0xe8) + 1) << 3));
    *(int64_t **)(arr + 0xe0) = plVar1;
    plVar1[*(int *)(arr + 0xe8)] = value;
    *(int *)(arr + 0xe8) = *(int *)(arr + 0xe8) + 1;
}

/* ========== PECMD_ReplaceStringSlot @0x140070398 ==========
 * 替换串并释放旧容器。
 */
int64_t *PECMD_ReplaceStringSlot(int64_t *ps, uint64_t *src)
{
    int64_t lVar1 = *ps;
    *ps = 0;
    if ((LPCWSTR)*src != NULL) {
        int64_t *plVar2 = (int64_t *)FUN_140070154((LPCWSTR)*src);
        *ps = (int64_t)plVar2;
    }
    if (lVar1 != 0) {
        HeapFree(g_hHeap, 0, (void *)(lVar1 - 8));
    }
    return ps;
}

/* ========== PECMD_ProcessCommandLine @0x1400727c4 ==========
 * 命令行处理。
 */
uint64_t PECMD_ProcessCommandLine(LPCSTR cmd, uint32_t flags, char *state, char *out,
                       uint64_t unused, uint64_t ctx, int mode)
{
    (void)unused;
    if (*state == '*') {
        return 1;
    }
    bool bVar1 = PECMD_CheckNetAddress(cmd, flags, state, out, ctx, mode);
    uint64_t uVar2 = bVar1 ? 1 : 0;
    if (*state == '-') {
        FUN_14005CC4C();
    }
    return uVar2;
}

/* ========== PECMD_ParseSignedNumber @0x140074c10 ==========
 * 带符号数字解析。
 */
uint64_t PECMD_ParseSignedNumber(int16_t *s)
{
    uint64_t uVar1 = 0;
    uint64_t local_res10[3] = {0, 0, 0};
    int16_t *local_res8 = s;
    if (*s == 0x2d) {
        uVar1 = 0x8000;
        do {
            s++;
            local_res8 = s;
        } while (*s == 0x2d);
    }
    FUN_1400745c8((int64_t *)&local_res8, local_res10);
    return (local_res10[0] & 0xffffffffffff7fffULL) | uVar1;
}

/* ========== PECMD_FreeArray_ddf8 @0x14007ddf8 ==========
 * 释放串数组。
 */
void PECMD_FreeArray_ddf8(int64_t *arr)
{
    int64_t lVar2 = arr[2];
    int64_t lVar1 = *arr;
    arr[2] = 0;
    while (0 < lVar2) {
        lVar2--;
        int64_t *_Memory = *(int64_t **)(lVar1 + lVar2 * 8);
        if (_Memory != NULL) {
            FUN_14005B104((WCHAR **)_Memory);
            free(_Memory);
            *(uint64_t *)(lVar1 + lVar2 * 8) = 0;
        }
    }
}
