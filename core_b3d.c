/* ====================================================================
 * core_b3d.c — B3-B6 小函数批 4 (140040000-1400bffff)
 *
 * 来源: PECMD原始.EXE (x64, ImageBase=0x140000000)
 *   调试输出          PECMD_DebugOutput @0x140045868
 *   选对象清理        PECMD_RestoreAndDeleteObject @0x14005b07c
 *   对象初始化        PECMD_InitObject @0x14005b3a0
 *   结构清零          PECMD_ZeroStruct_b848 @0x14005b848
 *   销毁窗口          PECMD_DestroyWindowLocked @0x14005ba6c
 *   边框绘制          PECMD_FrameRegion @0x14005d600
 *   数值换算          PECMD_ConvertValue @0x14005d6bc
 *   ICON 判定         PECMD_IsIconResource @0x14005d83c
 *   BITMAP 判定       PECMD_IsBitmapResource @0x14005d87c
 *   随机数取回        PECMD_GetRandom @0x14005e0a0
 *   标志复制          PECMD_CopyFlags @0x14005e36c
 *   读取字节          PECMD_ReadByte_f2d8 @0x14005f2d8
 *   状态查询          PECMD_QueryState_f414 @0x14005f414
 *   随机数种子        PECMD_FixRandomSeed @0x140060180
 *   标志清理          PECMD_ClearFlagAndError @0x14006020c
 *   字符表填充        PECMD_FillCharTable @0x1400603e8
 *   消息发送          PECMD_SendCtrlMessage_0834 @0x140060834
 *   设置字体          PECMD_SetFontMessage_090c @0x14006090c
 *   设置字体          PECMD_SetFontMessage_0948 @0x140060948
 *   标识符判定        PECMD_IsIdentifierChar @0x140060b24
 *   十六进制解析      PECMD_ParseHex_49f4 @0x1400649f4
 *   串匹配前置        PECMD_MatchPrefixAdvance @0x140066110
 *   串匹配+填充       PECMD_MatchAndPad @0x140066148
 *   串匹配推进        PECMD_MatchAndAdvance @0x1400661e4
 *   窗口动作          PECMD_WindowAction @0x1400668a8
 *   数字串打包        PECMD_PackDecimalDigits @0x140069cdc
 *   一次性初始化      PECMD_OneTimeInit @0x14007053c
 *   数字解析          PECMD_ParseNumber @0x14007486c
 *   消息+回调         PECMD_SendMessageAndCallback @0x14007df54
 *   格式化输出        PECMD_FormatOutput @0x14009c6dc
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
extern WCHAR **FUN_14005B154(WCHAR **pp); /* @0x14005b154 */

/* ---- 未实现依赖 (extern + TODO(verify)) ---- */
extern void PECMD_TlsLogWrite(uint64_t ctx, LPCWSTR fmt, uint64_t a, uint64_t b);
extern void PECMD_ShellLaunchThread(LARGE_INTEGER script, uint64_t a, uint64_t b,
                          uint64_t c);
extern void *FUN_1400E57C0(void *obj);
extern bool PECMD_PrefixMatchNoCase(uint16_t *s, char *key);
extern int64_t FUN_14005E04C(void);
extern bool FUN_14005F1B0(uint64_t *stream, uint8_t *out, uint64_t unused,
                          DWORD *readCount);
extern HANDLE PECMD_OpenLockVolume(int mode, uint64_t a, int64_t b,
                            uint32_t c, WCHAR *d);
extern char FUN_1400660AC(char *key, int64_t *pp, int len);
extern int64_t PECMD_TokPrefixICmp(char *a, uint16_t *w, int n);
extern int64_t PECMD_InitComState(void);
extern uint64_t PECMD_EvalParenExpression(int64_t *pp, uint64_t *out);
extern void FUN_14007D0AC(int64_t *ctx, LPCWSTR key, LPCWSTR value);
extern void FUN_14007DF90(int64_t ctx, int mode);
extern void PECMD_FillSpaces(int64_t *cursor, int count);

/* ---- 本批引用的全局数据 ---- */
extern uint8_t g_u8CCB1;
extern HWND g_hwndD310;
extern uint8_t g_msgWndMode;
extern HWND g_hPelogonWnd;
extern uint8_t g_flagD6F5;
extern int64_t g_pComWrite;
extern uint8_t PTR_FUN_1401250d0[];

/* ========== PECMD_DebugOutput @0x140045868 ==========
 * 调试输出 + 转发。
 */
void PECMD_DebugOutput(uint64_t a, uint64_t b, uint64_t c,
                   uint64_t d)
{
    (void)b;
    if (g_u8CCB1 != 0) {
        c = 0x261d;
        PECMD_TlsLogWrite((uint64_t)(uintptr_t)g_Script, WSTR("MAIN_DBG:%d\r\n"),
                      0x261d, d);
    }
    PECMD_ShellLaunchThread((LARGE_INTEGER)(int64_t)(uintptr_t)g_Script, a, c, d);
}

/* ========== PECMD_RestoreAndDeleteObject @0x14005b07c ==========
 * 还原旧对象并删除新对象。
 */
void PECMD_RestoreAndDeleteObject(uint64_t *slot)
{
    if ((HGDIOBJ)slot[1] != 0) {
        SelectObject((HDC)slot[2], (HGDIOBJ)slot[1]);
        slot[1] = 0;
    }
    if ((HGDIOBJ)*slot != 0) {
        DeleteObject((HGDIOBJ)*slot);
        *slot = 0;
    }
}

/* ========== PECMD_InitObject @0x14005b3a0 ==========
 * 对象初始化并挂接虚表。
 */
uint64_t *PECMD_InitObject(uint64_t *obj, uint64_t vtable)
{
    FUN_1400E57C0(obj);
    obj[0x1a] = 0;
    obj[0x1b] = 0;
    obj[0x1c] = vtable;
    *obj = (uint64_t)(uintptr_t)PTR_FUN_1401250d0;
    return obj;
}

/* ========== PECMD_ZeroStruct_b848 @0x14005b848 ==========
 * 结构清零并设置容量。
 */
uint8_t *PECMD_ZeroStruct_b848(uint8_t *obj)
{
    *(uint32_t *)(obj + 4) = 0;
    *(uint64_t *)(obj + 8) = 0;
    *obj = 0;
    memset(obj + 0x10, 0, 0x3d0);
    *(uint32_t *)(obj + 0x10) = 0x3d0;
    return obj;
}

/* ========== PECMD_DestroyWindowLocked @0x14005ba6c ==========
 * 临界区中销毁窗口。
 */
void PECMD_DestroyWindowLocked(void)
{
    EnterCriticalSection(&g_csInit);
    if (g_hwndD310 != (HWND)0) {
        DestroyWindow(g_hwndD310);
        g_hwndD310 = (HWND)0;
        g_msgWndMode = 0;
    }
    LeaveCriticalSection(&g_csInit);
}

/* ========== PECMD_FrameRegion @0x14005d600 ==========
 * 边框绘制。
 */
void PECMD_FrameRegion(uint64_t *obj, HDC hdc, HRGN rgn, uint32_t flags,
                   int thickness)
{
    if (thickness == 0) {
        thickness = *(int *)((uint8_t *)obj + 0x34);
    }
    if (rgn == (HRGN)0) {
        rgn = (HRGN)obj[3];
    }
    FrameRgn(hdc, rgn, (HBRUSH)*obj,
             -(uint32_t)(*(int *)((uint8_t *)obj + 48) != 0) & flags,
             thickness);
}

/* ========== PECMD_ConvertValue @0x14005d6bc ==========
 * 数值换算。
 */
int PECMD_ConvertValue(int value)
{
    int iVar1 = 1000;
    if ((value < 0x3e9) && (iVar1 = value, value < -1000)) {
        iVar1 = -1000;
    }
    int iVar2 = -2;
    if (-1 < iVar1) {
        iVar2 = 2;
    }
    return (iVar2 + iVar1 * 0xff) / 100;
}

/* ========== PECMD_IsIconResource @0x14005d83c ==========
 * ICON/#3 判定。
 */
uint32_t PECMD_IsIconResource(uint16_t *s)
{
    if (!PECMD_PrefixMatchNoCase(s, "ICON")) {
        if (!PECMD_PrefixMatchNoCase(s, "#3")) {
            return 0;
        }
    }
    return 1;
}

/* ========== PECMD_IsBitmapResource @0x14005d87c ==========
 * BITMAP/#2 判定。
 */
uint32_t PECMD_IsBitmapResource(uint16_t *s)
{
    if (!PECMD_PrefixMatchNoCase(s, "BITMAP")) {
        if (!PECMD_PrefixMatchNoCase(s, "#2")) {
            return 0;
        }
    }
    return 1;
}

/* ========== PECMD_GetRandom @0x14005e0a0 ==========
 * 临界区中取随机数并清符号位。
 */
uint64_t PECMD_GetRandom(void)
{
    uint64_t uVar1;
    EnterCriticalSection(&g_csInit);
    uVar1 = (uint64_t)FUN_14005E04C();
    LeaveCriticalSection(&g_csInit);
    return uVar1 & 0x7fffffffffffffffULL;
}

/* ========== PECMD_CopyFlags @0x14005e36c ==========
 * 按掩码复制 4 个字段。
 */
void PECMD_CopyFlags(int64_t dst, uint32_t *src)
{
    uint32_t uVar1 = src[5];
    if ((uVar1 & 1) != 0) {
        *(uint32_t *)(dst + 0x13c) = src[0];
    }
    if ((uVar1 & 2) != 0) {
        *(uint32_t *)(dst + 0x140) = src[1];
    }
    if ((uVar1 & 4) != 0) {
        *(uint32_t *)(dst + 0x144) = src[2];
    }
    if ((uVar1 & 8) != 0) {
        *(uint32_t *)(dst + 0x148) = src[3];
    }
}

/* ========== PECMD_ReadByte_f2d8 @0x14005f2d8 ==========
 * 读取一个字节, 失败返回 -1。
 */
uint64_t PECMD_ReadByte_f2d8(int64_t stream)
{
    uint8_t local_res8[8] = {0};
    DWORD local_res10[6] = {0};
    *(int *)(stream + 0x34) = *(int *)(stream + 0x34) + 1;
    bool bVar1 = FUN_14005F1B0((void *)(stream + 8), local_res8, 1, local_res10);
    if (!bVar1 || local_res10[0] == 0) {
        return 0xffffffff;
    }
    return (uint64_t)local_res8[0];
}

/* ========== PECMD_QueryState_f414 @0x14005f414 ==========
 * 状态查询: 特殊条件下取全局值。
 */
uint64_t PECMD_QueryState_f414(int64_t obj)
{
    uint64_t uVar1;
    if ((obj == 0) || (*(char *)(obj + 0x121) != '\0')) {
        EnterCriticalSection(&g_csInit);
        uVar1 = g_hPelogonWnd;
        LeaveCriticalSection(&g_csInit);
    } else {
        uVar1 = *(uint64_t *)(obj + 0x20);
    }
    return uVar1;
}

/* ========== PECMD_FixRandomSeed @0x140060180 ==========
 * 随机数种子修正。
 */
void PECMD_FixRandomSeed(int64_t obj)
{
    int iVar1 = *(int *)(obj + 0x1b8);
    if (((iVar1 == 0) || (iVar1 == -1)) || (iVar1 == 1)) {
        int64_t lVar2 = FUN_14005E04C();
        *(uint32_t *)(obj + 0x1b8) = ((uint32_t)lVar2 & 0xffffff4d) | 0x4d;
    }
}

/* ========== PECMD_ClearFlagAndError @0x14006020c ==========
 * 标志清理并调用错误处理。
 */
void PECMD_ClearFlagAndError(uint8_t *flags)
{
    if ((*flags & 0xf) != 0) {
        PECMD_OpenLockVolume(-1, 0xffffffffffffffffULL, -1, 0, NULL);
        *flags = 0;
        g_flagD6F5 = 0;
    }
}

/* ========== PECMD_FillCharTable @0x1400603e8 ==========
 * 填充字符表 [start, 0x5b)。
 */
void PECMD_FillCharTable(uint16_t start, uint64_t *table)
{
    memset(table, 0, 0x1b);
    for (; start < 0x5b; start++) {
        *(char *)table = (char)start;
        table = (uint64_t *)((uint8_t *)table + 1);
    }
}

/* ========== PECMD_SendCtrlMessage_0834 @0x140060834 ==========
 * 发送 0x465 消息并附带 5 个参数槽，返回 SendMessageW 结果。
 */
void *PECMD_SendCtrlMessage_0834(WPARAM wParam, uint64_t lParam)
{
    (void)lParam;
    uint64_t local_38 = 1;
    return (void *)SendMessageW(*(HWND *)((uint8_t *)wParam + OBJ_HWND), 0x465,
                                wParam, (LPARAM)&local_38);
}

/* ========== PECMD_SetFontMessage_090c @0x14006090c ==========
 * 设置字体/颜色消息。
 */
LRESULT PECMD_SetFontMessage_090c(int64_t obj, WPARAM wParam, int set)
{
    LRESULT LVar1;
    if ((wParam == 0) || (wParam == (WPARAM)-1)) {
        LVar1 = (LRESULT)(int)wParam;
    } else {
        *(WPARAM *)(obj + OBJ_WPARAM) = wParam;
        LVar1 = SendMessageW(*(HWND *)(obj + OBJ_HWND), 0x30, wParam,
                             (uint64_t)(set != 0));
    }
    return LVar1;
}

/* ========== PECMD_SetFontMessage_0948 @0x140060948 ==========
 * 设置字体/颜色消息 (指定窗口)。
 */
LRESULT PECMD_SetFontMessage_0948(int64_t obj, HWND hwnd, WPARAM wParam, int set)
{
    LRESULT LVar1;
    if ((wParam == 0) || (wParam == (WPARAM)-1)) {
        LVar1 = (LRESULT)(int)wParam;
    } else {
        if (hwnd == *(HWND *)(obj + OBJ_HWND)) {
            *(WPARAM *)(obj + OBJ_WPARAM) = wParam;
        }
        LVar1 = SendMessageW(hwnd, 0x30, wParam, (uint64_t)(set != 0));
    }
    return LVar1;
}

/* ========== PECMD_IsIdentifierChar @0x140060b24 ==========
 * 标识符字符判定。
 */
uint64_t PECMD_IsIdentifierChar(uint16_t ch)
{
    if (((((ch != 0x5f) && (ch != 0x24)) &&
          ((ch < 0x61 || (0x7a < ch)))) &&
         (ch < 0x80)) &&
        ((ch < 0x30 || (0x39 < ch)))) {
        return 0;
    }
    return 1;
}

/* ========== PECMD_ParseHex_49f4 @0x1400649f4 ==========
 * 十六进制字符串解析。
 */
int64_t PECMD_ParseHex_49f4(uint8_t *s)
{
    int64_t lVar3 = 0;
    while ((('/' < (char)*s && ((char)*s < ':')) ||
            (('`' < (char)(*s | 0x20) && ((char)(*s | 0x20) < 'g'))))) {
        uint32_t uVar1 = (uint32_t)(char)*s;
        s++;
        int iVar2;
        if ((int)uVar1 < 0x3a) {
            iVar2 = (int)uVar1 - 0x30;
        } else {
            iVar2 = (int)(uVar1 | 0x20) - 0x57;
        }
        lVar3 = lVar3 * 0x10 + iVar2;
    }
    return lVar3;
}

/* ========== PECMD_MatchPrefixAdvance @0x140066110 ==========
 * 串匹配前置: 成功则推进指针。
 */
bool PECMD_MatchPrefixAdvance(char *key, int64_t *pp, int len)
{
    int64_t local_res10[3];
    local_res10[0] = *pp + 2;
    char cVar1 = FUN_1400660AC(key, local_res10, len);
    if (cVar1 != '\0') {
        *pp = local_res10[0];
    }
    return cVar1 != '\0';
}

/* ========== PECMD_MatchAndPad @0x140066148 ==========
 * 串匹配+填充+跳空白。
 */
char PECMD_MatchAndPad(int64_t key, int64_t *pp, int len)
{
    int64_t lVar2 = PECMD_TokPrefixICmp((char *)(key + 1),
                                  (uint16_t *)(*pp + 2), len - 1);
    char cVar1 = (char)lVar2;
    if (cVar1 != '\0') {
        PECMD_FillSpaces(pp, len);
        FUN_14005B154((WCHAR **)pp);
    }
    return cVar1;
}

/* ========== PECMD_MatchAndAdvance @0x1400661e4 ==========
 * 串匹配成功则推进 len 个字符。
 */
bool PECMD_MatchAndAdvance(char *key, int64_t *pp, int len)
{
    uint16_t *puVar1 = (uint16_t *)*pp;
    int64_t lVar3 = len;
    uint64_t uVar2 = FUN_14005C788(key, puVar1, len);
    bool bVar4 = (char)uVar2 != '\0';
    if (bVar4) {
        *pp = (int64_t)(puVar1 + lVar3);
    }
    return bVar4;
}

/* ========== PECMD_WindowAction @0x1400668a8 ==========
 * 窗口动作: 特殊标志直接 ShowWindow, 否则虚表调用。
 */
void PECMD_WindowAction(int64_t *obj, int action, uint32_t flags, int extra)
{
    if ((action == 1) && ((*(uint8_t *)((uint8_t *)obj + 0x1a * 8) & 0x10) != 0)) {
        ShowWindow((HWND)obj[4], 1);
    } else {
        void (**fn)(void *, int, int, int64_t) =
            (void (**)(void *, int, int, int64_t))(*obj + 8);
        (**fn)((void *)obj[4], 5, action,
               (int64_t)(int)(extra << 0x10 | flags));
    }
}

/* ========== PECMD_PackDecimalDigits @0x140069cdc ==========
 * 十进制数字串按字节打包。
 */
uint64_t PECMD_PackDecimalDigits(char *s, int count)
{
    uint64_t uVar2 = 0;
    uint64_t uVar4 = 0;
    uint32_t uVar3 = 0;
    if (0 < count) {
        do {
            if (9 < (uint8_t)(*s - 0x30U)) {
                return uVar2;
            }
            char cVar1 = *s;
            uVar3 = (uint32_t)uVar4 + 1;
            uVar4 = uVar3;
            s++;
            uVar2 = (uVar2 << 8) | (uint64_t)(uint8_t)(cVar1 - 0x30);
        } while ((int)uVar3 < count);
    }
    return uVar2;
}

/* ========== PECMD_OneTimeInit @0x14007053c ==========
 * 一次性初始化状态机。
 */
uint64_t PECMD_OneTimeInit(void)
{
    if (g_pComWrite == 1) {
        return 0xffffffff;
    }
    int64_t lVar2 = g_pComWrite;
    if (g_pComWrite == 0) {
        g_pComWrite = 1;
        lVar2 = PECMD_InitComState();
        if (lVar2 == 0) {
            return 0xffffffff;
        }
    }
    g_pComWrite = lVar2;
    return 0;
}

/* ========== PECMD_ParseNumber @0x14007486c ==========
 * 数字解析: 成功则回写。
 */
bool PECMD_ParseNumber(int64_t *pp, int *out)
{
    uint64_t local_res10[3];
    local_res10[0] = (uint64_t)*out;
    uint64_t uVar2 = PECMD_EvalParenExpression(pp, local_res10);
    bool bVar1 = 0 < (int)uVar2;
    if (bVar1) {
        *out = (int)local_res10[0];
    }
    return bVar1;
}

/* ========== PECMD_SendMessageAndCallback @0x14007df54 ==========
 * 发送 0xf0 消息后回调。
 */
void PECMD_SendMessageAndCallback(int64_t obj)
{
    if (**(int16_t **)(obj + 0x10) != 0) {
        LRESULT LVar1 = SendMessageW(*(HWND *)(*(int64_t *)(obj + 0x38) + OBJ_HWND),
                                     0xf0, 0, 0);
        FUN_14007DF90(obj, (int)LVar1);
    }
}

/* ========== PECMD_FormatOutput @0x14009c6dc ==========
 * 格式化输出到控件。
 * TODO(verify): wsprintfW(L"%d") 缺实参, 按 0 补齐。
 */
void PECMD_FormatOutput(int64_t obj)
{
    WCHAR local_88[64];
    wsprintfW(local_88, WSTR("%d"), 0);
    if (**((LPCWSTR *)(obj + 0x10)) != L'\0') {
        FUN_14007D0AC(*(int64_t **)(obj + 0x50),
                      *(LPCWSTR *)(obj + 0x10), local_88);
    }
}
