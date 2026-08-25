/* ====================================================================
 * core_b8h.c — B8 小函数批 7 (1400e0000-1400fffff)
 *
 * 本批新实现函数全部使用人类可读 PECMD_ 名称，原始地址保留在 @0x 注释。
 *
 * 来源: PECMD原始.EXE (x64, ImageBase=0x140000000)
 *   进程注入 API 就绪    PECMD_HasVirtualAllocEx @0x1400e411c
 *   对象双标志查询         PECMD_PeCtxHasImageBase      @0x1400e412c
 *   指针表偏移搬运         PECMD_PeApplyRelocations  @0x1400e4160
 *   构建程序 exe 路径      PECMD_GetModulePathAlt      @0x1400e429c
 *   初始化 CRC32 表        PECMD_InitCrc32Table      @0x1400e4c38
 *   计算 CRC32             PECMD_Crc32HexOfBytes         @0x1400e4cc0
 *   RAS API 就绪查询      PECMD_HasRasDialApi     @0x1400e4d84
 *   初始化事件对           FUN_1400E4E6C     @0x1400e4e6c
 *   销毁事件对             FUN_1400E4E94  @0x1400e4e94
 *   触发同步事件           PECMD_RasDialStatusCallback   @0x1400e4edc
 *   取控件字体             FUN_1400E5890    @0x1400e5890
 *   加载控件字体           FUN_1400E66D4   @0x1400e66d4
 *   配置对话框过程         FUN_1400E6790  @0x1400e6790
 *   跳到指定分隔符 A       PECMD_SkipEncByteToEol   @0x1400e706c
 *   跳过前导 CR/LF A       PECMD_SkipCrLfEncByte     @0x1400e7098
 *   跳到指定分隔符 W       PECMD_SkipEncWCharToEol   @0x1400e70c0
 *   跳过前导 CR/LF W       PECMD_SkipCrLfEncWchar     @0x1400e70f4
 *   等待 RAS 拨号连接      PECMD_HangUpRasConnection @0x1400e75cc
 *   缓存窗口 ID            FUN_1400E8644    @0x1400e8644
 *   下发控件命令           PECMD_CtlSendChildCmd  @0x1400e86b4
 *   销毁窗口对象(带GDI)    FUN_1400E9138 @0x1400e9138
 *   绘制百分比条           PECMD_DrawScaledBarFill    @0x1400f0df4
 *   设置对象画刷           PECMD_SetCtlBgBrush   @0x1400f0eb0
 *   激活设备窗口           PECMD_SetHotTrackWindow @0x1400f1448
 *   初始化简单窗口对象     FUN_1400F1BE4 @0x1400f1be4
 *   计算滚动偏移           PECMD_ScaleMetricByFactor  @0x1400f2a7c
 *   宽字符游标扫描         FUN_1400F429C @0x1400f429c
 *   初始化列表视窗对象     FUN_1400FB588 @0x1400fb588
 *   初始化编辑对象核心     FUN_1400FCF44   @0x1400fcf44
 *   销毁组合对象           PECMD_DtorTreeSubObj @0x1400fecfc
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
/* ---- 已实现公共工具 (pecmd_defs.h / core_*.c) ---- */
extern void PECMD_AppendFmtValue(void *script, uint64_t value, LPCWSTR key,
                                 LPCWSTR fmt);                                 /* @0x1400668ec */
extern LPCWSTR FUN_14005B6AC(HINSTANCE inst, UINT id, LPWSTR buf, int buflen); /* @0x14005b6ac */
extern HFONT FUN_1400B89DC(HANDLE obj, double *size, LPCWSTR name);            /* @0x1400b89dc */
extern void PECMD_GetDpiCached(HWND hwnd);                                     /* @0x140062950 */
extern COLORREF PECMD_FillRectColor(HDC hdc, RECT *rc, COLORREF color);        /* @0x1400e68e0 */
extern void *FUN_1400E57C0(void *obj);                                         /* @0x1400e57c0 */
extern void FUN_1400E8940(void *obj);                                          /* @0x1400e8940 */
/* S11: 本地声明与定义冲突已删除, 统一采用 xproto.h 原型 (原: extern int32_t FUN_1400630D0(int mode); /* @0x1400630d0 分配失败提示 * / /* ---- 未实现依赖 (extern + TODO(verify)) ---- *) */


/* S11: 本地声明与定义冲突已删除, 统一采用 xproto.h 原型 (原: extern HWND PECMD_GetOrCreateHiddenWnd(void *script, int64_t flag);) */


extern int wsprintfA(LPSTR buf, LPCSTR fmt, ...);
extern BOOL EndDialog(HWND hwnd, intptr_t result);

/* ---- 本批引用的虚表/数据符号 (.rdata) ---- */
extern uint8_t PTR_FUN_14012b1c0[];
extern uint8_t PTR_FUN_14012b430[];
extern uint8_t PTR_FUN_14012c2e0[];
extern uint8_t PTR_FUN_14012c7b0[];
extern uint8_t PTR_FUN_14012ccc0[];
extern uint8_t PTR_FUN_14012cf00[];

/* ---- 全局数据 (core_globals.c 定义) ---- */
extern uint32_t (*g_pfnVirtualAllocEx)(void); /* DAT_14013e258 VirtualAllocEx */
extern uint32_t *g_pCrcTable;                 /* DAT_14013e260 */
extern HANDLE g_hSyncEvtA;                    /* DAT_14013e1f8 */
extern HANDLE g_hSyncEvtB;                    /* DAT_14013e200 */
extern uint32_t g_syncEvtVal;                 /* DAT_14013e208 */
extern uint32_t (*g_pfnRasDial)(void);        /* DAT_14013e268 RasDialW */
extern uint32_t (*g_pfnRasGetConnectStatus)(uint64_t,
                                            uint32_t *); /* DAT_14013e278 RasGetConnectStatusW */
extern uint32_t (*g_pfnRasHangUp)(uint64_t);             /* DAT_14013e288 RasHangUpW */
extern void *g_syncState;                                /* DAT_14013e298 同步状态 */
extern HWND g_hActiveDevWnd;                             /* DAT_14013e400 当前设备窗口 */
extern HCURSOR g_hArrowCursor;                           /* DAT_14013e4d8 箭头光标句柄 */
extern uint8_t g_objMode;                                /* DAT_14013a84d 对象布局/指针模式 */
extern HGDIOBJ g_hStockWhiteBrush;                       /* DAT_14013a858 库存白色画刷 */
extern float g_barScaleDiv;                              /* DAT_14012c0d0 条比例除数 */
extern double g_scrollLimit;                             /* DAT_140129ee8 滚动上限常量 */

/* ---- 其他文件已使用的语义全局 ---- */
extern uint32_t g_dpi;       /* g_dpi */
extern HANDLE g_pFontBase;   /* DAT_14013e2a8 字体基对象 */
extern double g_fontSizeDef; /* DAT_1401293c0 = -0x80000000.0 */
extern double g_fontMinus0;  /* DAT_140125238 = -0.0 */
extern double g_dbl25230;    /* 0.5 四舍五入常量 */

/* ========== PECMD_HasVirtualAllocEx @0x1400e411c ==========
 * 查询进程注入 API (VirtualAllocEx) 是否已加载。
 */
bool PECMD_HasVirtualAllocEx(void)
{
    return g_pfnVirtualAllocEx != NULL;
}

/* ========== PECMD_PeCtxHasImageBase @0x1400e412c ==========
 * 检查对象两个相邻字段是否同时非零 (对象布局随 g_objMode 切换)。
 * TODO(verify): 具体语义 (可能是某容器"待处理数据"标志)。
 */
bool PECMD_PeCtxHasImageBase(int64_t obj)
{
    int32_t v;

    if (g_objMode == 0x04) {
        if (*(int32_t *)((uint8_t *)obj + 0xa0) == 0)
            return false;
        v = *(int32_t *)((uint8_t *)obj + 0xa4);
    }
    else {
        if (*(int32_t *)((uint8_t *)obj + 0xb0) == 0)
            return false;
        v = *(int32_t *)((uint8_t *)obj + 0xb4);
    }
    return v != 0;
}

/* ========== PECMD_PeApplyRelocations @0x1400e4160 ==========
 * 对"段表/重定位表"的一批记录做偏移搬运:
 * 每条记录为 { 头偏移, 项数 }, 其中的项带类型高 4 位 (0x3000=32位, 其他=64位)
 * 与低 12 位偏移, 将 delta 与对象尺寸之差加到每个指针。
 * TODO(verify): 记录布局与算法细节。
 */
uint64_t PECMD_PeApplyRelocations(int64_t obj, int64_t base, int64_t delta)
{
    uint64_t objSize;
    uint32_t head;
    uint32_t *rec; /* 当前记录头 (两项: 头偏移 + 项数) */
    uint32_t *p;   /* 记录内的项指针 */
    uint64_t n;
    uint32_t id;
    int64_t *target;

    if (g_objMode == 0x04)
        objSize = *(uint32_t *)((uint8_t *)obj + 0x34);
    else
        objSize = *(uint64_t *)((uint8_t *)obj + 0x30);

    if (g_objMode == 0x04)
        head = *(uint32_t *)((uint8_t *)obj + 0xa0);
    else
        head = *(uint32_t *)((uint8_t *)obj + 0xb0);

    rec = (uint32_t *)((uint64_t)head + base);
    if (rec[1] + rec[0] == 0)
        return 1;

    for (;;) {
        p = rec;
        if ((rec[1] - (int32_t)g_objMode) >> 1 != 0) {
            n = (uint64_t)((rec[1] - (int32_t)g_objMode) >> 1);
            do {
                id = *p & 0xf000;
                if (id != 0) {
                    target =
                        (int64_t *)((uint64_t)rec[0] + (uint64_t)((uint16_t)*p & 0xfff) + base);
                    if (id == 0x3000)
                        *(int32_t *)target += (int32_t)(delta - objSize);
                    else
                        *target += (delta - objSize);
                }
                p = (uint32_t *)((uint8_t *)p + 2);
                n--;
            } while (n != 0);
        }
        rec = p;
        if (rec[1] + rec[0] == 0)
            break;
    }
    return 1;
}

/* ========== PECMD_GetModulePathAlt @0x1400e429c ==========
 * 构造当前可执行文件的完整路径 (含扩展名探测):
 *   - 先 GetModuleFileNameW 取 exe 路径
 *   - 64 位系统中若同名 "xxx2.exe" 存在则改用它
 * TODO(verify): 缓冲区头部 16 字节用途。
 */
LPCWSTR PECMD_GetModulePathAlt(uint64_t *obj)
{
    LPWSTR lpFilename;
    int i;

    PECMD_AllocString((WCHAR **)obj, 0x218);
    lpFilename = (LPWSTR)((uint64_t *)*obj + 2); /* 跳过头部 16 字节 */
    memset((uint64_t *)*obj, 0, 0x430);
    lpFilename[0] = L'\0';
    GetModuleFileNameW((HMODULE)0, lpFilename, 0x208);
    if (g_bX64 == 1) {
        i = lstrlenW(lpFilename);
        lpFilename[i] = L'2';
        if (!FUN_140101E70(lpFilename))
            lpFilename[i] = L'\0';
    }
    return lpFilename;
}

/* ========== PECMD_InitCrc32Table @0x1400e4c38 ==========
 * 初始化 CRC32 查表 (多项式 0xEDB88320, 256 项)。
 * 分配失败时通过 FUN_1400630D0 提示并重试。标准 CRC32, 行为与 zlib 表一致。
 */
void PECMD_InitCrc32Table(void)
{
    uint64_t *blk;
    uint32_t *tbl;
    uint32_t v;
    int bit;

    do {
        blk = HeapAlloc(g_hHeap, 0, 0x408);
        if (blk != NULL)
            break;
        if (FUN_1400630D0(2) != 4)
            break;
    } while (1);

    tbl = (uint32_t *)((uint8_t *)blk + 8); /* 头部 0x400 容量 + 0xaa55 魔数 */
    *(uint32_t *)((uint8_t *)blk + 4) = 0xaa55;
    blk[0] = 0x400;
    g_pCrcTable = tbl;

    for (v = 0; v < 0x100; v++) {
        uint32_t c = v;
        for (bit = 0; bit < 8; bit++) {
            if (c & 1)
                c = (c >> 1) ^ 0xedb88320;
            else
                c >>= 1;
        }
        tbl[v] = c;
    }
}

/* ========== PECMD_Crc32HexOfBytes @0x1400e4cc0 ==========
 * 计算数据块的 CRC32:
 *   - 输出缓冲按 8 字节对齐时, 可复用 arena 头部 (hdr) 做前置种子/结果结构。
 *   - 低 bit1 (out & 2) 为 0 时补码并格式化 %08X; 否则写回 arena 结构。
 * TODO(verify): arena 头部解析与 out&2 语义。
 */
uint32_t PECMD_Crc32HexOfBytes(const uint8_t *data, int64_t len, char *out)
{
    uint32_t crc;
    int64_t *hdr;
    char *base;

    if (g_pCrcTable == NULL)
        PECMD_InitCrc32Table();

    crc = 0xffffffff;
    hdr = NULL;
    base = out;
    if (((uintptr_t)out & 7) != 0) {
        base = out - (int8_t)((uintptr_t)out & 7);
        hdr = (int64_t *)(base - 0x18);
        crc = 0xffffffff;
        if (*hdr != 0)
            crc = *(uint32_t *)(base - 0x10);
    }

    while (len-- != 0) {
        crc = g_pCrcTable[(uint8_t)(crc ^ *data++)] ^ (crc >> 8);
    }

    if (((uintptr_t)out & 2) == 0) {
        crc = ~crc;
        if (base != NULL)
            wsprintfA(base, "%08X", crc); /* 反编译缺实参, 按值补齐 (TODO(verify)) */
    }
    else {
        hdr[0] = 1;
        hdr[1] = (int64_t)crc;
    }
    return crc;
}

/* ========== PECMD_HasRasDialApi @0x1400e4d84 ==========
 * 查询 RASAPI32 是否已加载 (即 RasDialW 函数指针非空)。
 */
bool PECMD_HasRasDialApi(void)
{
    return g_pfnRasDial != NULL;
}

/* ========== FUN_1400E4E6C @0x1400e4e6c ==========
 * 初始化"事件对"对象: 挂虚表, 清空两个事件句柄与槽位。
 */
uint64_t *FUN_1400E4E6C(uint64_t *obj)
{
    obj[0] = (uint64_t)&PTR_FUN_14012b1c0;
    g_hSyncEvtA = 0;
    g_hSyncEvtB = 0;
    obj[0x14f] = 0;
    return obj;
}

/* ========== FUN_1400E4E94 @0x1400e4e94 ==========
 * 销毁"事件对"对象: 关闭两个事件句柄。
 */
void FUN_1400E4E94(uint64_t *obj)
{
    obj[0] = (uint64_t)&PTR_FUN_14012b1c0;
    if (g_hSyncEvtA != 0) {
        CloseHandle(g_hSyncEvtA);
        g_hSyncEvtA = 0;
    }
    if (g_hSyncEvtB != 0) {
        CloseHandle(g_hSyncEvtB);
        g_hSyncEvtB = 0;
    }
}

/* ========== PECMD_RasDialStatusCallback @0x1400e4edc ==========
 * 按模式触发"事件对"之一:
 *   - value != 0          -> SetEvent(B 事件), 记录 value
 *   - mode 为 0x2000/2001 -> SetEvent(A 事件), 沿用已记录值
 * TODO(verify): mode 常量的业务含义。
 */
void PECMD_RasDialStatusCallback(uint64_t unused, int mode, int value)
{
    HANDLE evt = g_hSyncEvtB;
    int val = value;

    (void)unused;
    if (value != 0 ||
        (evt = g_hSyncEvtA, val = (int)g_syncEvtVal, mode == 0x2000 || mode == 0x2001)) {
        g_syncEvtVal = (uint32_t)val;
        SetEvent(evt);
    }
}

/* ========== FUN_1400E5890 @0x1400e5890 ==========
 * 取控件字体: 优先 WM_GETFONT, 失败回退字体基对象。
 */
LRESULT FUN_1400E5890(int64_t obj)
{
    LRESULT r;

    r = SendMessageW(*(HWND *)((uint8_t *)obj + OBJ_HWND), 0x31, 0, 0); /* WM_GETFONT */
    if (r == (LRESULT)-1 || r == 0)
        r = (LRESULT)g_pFontBase;
    return r;
}

/* ========== FUN_1400E66D4 @0x1400e66d4 ==========
 * 把控件字体指针 (id 指代的字体规格) 装载到 *pfont。
 * 字符串优先取预置资源, 否则从语言表取, '|' 后为真正字面。
 */
void FUN_1400E66D4(int64_t *pfont, UINT id, HANDLE base)
{
    LPWSTR tmp = NULL;
    LPCWSTR face;
    LPWSTR pipe;
    HFONT f;
    double sz;

    if (*pfont == -1)
        return;

    PECMD_AllocWStringBuffer((WCHAR **)&tmp, 0x516);
    tmp[0] = L'\0';
    face = FUN_1400169BC(id, NULL);
    if (face == NULL) {
        FUN_14005B6AC(g_hInstance, id, tmp, 0x514);
        pipe = StrChrW(tmp, L'|');
        face = tmp;
        if (pipe != NULL)
            face = pipe + 1;
    }
    sz = g_fontSizeDef;
    f = FUN_1400B89DC(base, &sz, face);
    *pfont = (int64_t)(uintptr_t)f;
    PECMD_FreeStrBuf(&tmp);
}

/* ========== FUN_1400E6790 @0x1400e6790 ==========
 * 配置对话框的消息过程:
 *   msg=0x30 -> 初始化 DPI 与基础字体; msg=0x110 -> EndDialog(2)。
 * TODO(verify): 0x30/0x110 的消息含义。
 */
uint64_t FUN_1400E6790(HWND hwnd, int msg, HANDLE base)
{
    if (msg == 0x30) {
        if (g_dpi == 0)
            PECMD_GetDpiCached(hwnd);
        if (g_pFontBase == 0)
            FUN_1400E66D4((int64_t *)&g_pFontBase, 0x3e9, base);
    }
    else if (msg == 0x110) {
        EndDialog(hwnd, 2);
    }
    return 0;
}

/* ========== PECMD_SkipEncByteToEol @0x1400e706c ==========
 * ANSI 指针前进至命中 param/d 或 CR(0x0d^d)/LF(0x0a^d) 处。
 */
void PECMD_SkipEncByteToEol(uint8_t **pp, uint8_t d)
{
    uint8_t *p = *pp;

    while (d != *p && (d ^ 0x0d) != *p && (d ^ 0x0a) != *p) {
        p++;
        *pp = p;
    }
}

/* ========== PECMD_SkipCrLfEncByte @0x1400e7098 ==========
 * ANSI: 跳过紧邻的 CR/LF (各至多一个)。
 */
void PECMD_SkipCrLfEncByte(uint8_t **pp, uint8_t d)
{
    if ((d ^ 0x0d) == *(*pp))
        (*pp)++;
    if ((d ^ 0x0a) == *(*pp))
        (*pp)++;
}

/* ========== PECMD_SkipEncWCharToEol @0x1400e70c0 ==========
 * 宽字符指针前进至命中 param/d 或 CR(0x0d^d)/LF(0x0a^d) 处。
 */
void PECMD_SkipEncWCharToEol(uint16_t **pp, uint16_t d)
{
    uint16_t *p = *pp;

    while (d != *p && (d ^ 0x0d) != *p && (d ^ 0x0a) != *p) {
        p++;
        *pp = p;
    }
}

/* ========== PECMD_SkipCrLfEncWchar @0x1400e70f4 ==========
 * 宽字符: 跳过紧邻的 CR/LF (各至多一个)。
 */
void PECMD_SkipCrLfEncWchar(uint16_t **pp, uint16_t d)
{
    if ((d ^ 0x0d) == *(*pp))
        (*pp)++;
    if ((d ^ 0x0a) == *(*pp))
        (*pp)++;
}

/* ========== PECMD_HangUpRasConnection @0x1400e75cc ==========
 * 等待 RAS 拨号连接: 先调用 RasHangUpW 快速试探, 失败则调用
 * RasGetConnectStatusW 在 3 秒内轮询 (每 50ms), 返回 6 视为完成。
 * 返回 1 表示成功, 0 表示超时/中断。
 * TODO(verify): RAS 返回码与 0x234 状态块的具体语义。
 */
uint64_t PECMD_HangUpRasConnection(uint64_t *state)
{
    uint32_t pollBlk[144];
    uint32_t t0, t1;
    uint64_t obj;
    int r;

    r = g_pfnRasHangUp(*state);
    if (r == 0) {
        pollBlk[0] = 0x234;
        t0 = GetTickCount();
        obj = *state;
        *state = 0;
        for (;;) {
            r = g_pfnRasGetConnectStatus(obj, pollBlk);
            if (r == 6)
                break;
            t1 = GetTickCount();
            if (t0 + 3000 <= t1)
                break;
            Sleep(50);
        }
        *state = 0;
        obj = 0;
    }
    else {
        *state = 0;
        g_syncState = 0;
        obj = 1;
    }
    return obj;
}

/* ========== FUN_1400E8644 @0x1400e8644 ==========
 * 取脚本对象缓存的窗口 ID; 为空则创建脚本窗口并写入集变量 "&&__WinID"。
 * 全程持有临界区 g_csInit。
 */
int64_t FUN_1400E8644(int64_t *obj)
{
    HWND hwnd;
    int64_t v;

    EnterCriticalSection(&g_csInit);
    v = obj[8];
    if (v == 0) {
        hwnd = PECMD_GetOrCreateHiddenWnd(obj, 0);
        obj[8] = (int64_t)(uintptr_t)hwnd;
        if (hwnd != 0)
            PECMD_AppendFmtValue(obj, *(uint64_t *)(hwnd + 8), WSTR("&&__WinID"), WSTR("0x%I64X"));
        v = obj[8];
    }
    LeaveCriticalSection(&g_csInit);
    return v;
}

/* ========== PECMD_CtlSendChildCmd @0x1400e86b4 ==========
 * 向 (id-0x1000) 索引的控件转发消息 WM_0xBC2B, 并调用对象虚表消息入口。
 * TODO(verify): 0xbc2b/0x2c 与控件表布局。
 */
void PECMD_CtlSendChildCmd(int64_t *obj, uint64_t wParam, LPARAM lParam)
{
    typedef uint64_t (*ObjMsgFn)(int64_t self, int msg, uint64_t w, int64_t l);
    int64_t child;
    int idx;

    idx = (int)(wParam & 0xffff) - 0x1000;
    if ((int8_t)g_flagA24F > 0) {
        if (idx >= 0 && idx < *(int32_t *)obj[0x35]) {
            child = *(int64_t *)(*(int64_t *)obj[0x34] + (int64_t)idx * 8);
            if (child != 0) {
                child = *(int64_t *)(child + 0x38);
                if (child != 0)
                    SendMessageW(*(HWND *)(child + OBJ_HWND), 0xbc2b, 0, lParam);
            }
        }
        ((ObjMsgFn)((void **)*obj)[1])(obj[4], 0x2c, wParam, lParam);
    }
}

/* ========== FUN_1400E9138 @0x1400e9138 ==========
 * 销毁"窗口对象"(含 GDI 画刷、子缓冲区、字符串容器)。
 * 恢复原窗口过程 0x140001188 后 DestroyWindow, 再回收 GDI 与基类。
 */
void FUN_1400E9138(uint64_t *obj)
{
    HWND hwnd;

    hwnd = (HWND)obj[4];
    obj[0] = (uint64_t)&PTR_FUN_14012b430;
    if (hwnd != 0) {
        obj[4] = 0;
        if (IsWindow(hwnd)) {
            *(uint32_t *)((uint8_t *)obj + OBJ_FLAGS) = 0; /* obj+0x1d */
            if (obj[0x1c] != 0) {
                HeapFree(g_hHeap, 0, (void *)(obj[0x1c] - 8)); /* 带头部分配块 */
                obj[0x1c] = 0;
            }
            SetWindowLongPtrW(hwnd, -4, 0x140001188); /* 恢复默认 WndProc */
            DestroyWindow(hwnd);
        }
    }
    if ((HGDIOBJ)obj[0x1e] != 0) {
        DeleteObject((HGDIOBJ)obj[0x1e]);
        obj[0x1e] = 0;
    }
    PECMD_FreeStrBuf((WCHAR **)(obj + 0x25));
    FUN_1400E8940(obj);
}

/* ========== PECMD_DrawScaledBarFill @0x1400f0df4 ==========
 * 绘制两段式百分比条:
 *   - 底色取对象 0x54 (负值则用 GetBkColor/前景 0x58)
 *   - 按 obj+0x50 比例在左边叠画一段前景色
 *   - edge 非 0 时加 DrawEdge 边框
 */
void PECMD_DrawScaledBarFill(int64_t obj, HDC hdc, RECT *rc, COLORREF color, int edge)
{
    COLORREF c;
    int w;

    if (*(int32_t *)((uint8_t *)obj + 0x54) < 0) {
        c = color;
        if ((int32_t)c < 0)
            c = GetBkColor(hdc);
    }
    else {
        c = *(COLORREF *)((uint8_t *)obj + 0x54);
    }
    PECMD_FillRectColor(hdc, rc, c);

    w = rc->right - rc->left;
    rc->right =
        rc->left - (int)(((float)(int64_t)w * *(float *)((uint8_t *)obj + 0x50)) / g_barScaleDiv);
    PECMD_FillRectColor(hdc, rc, *(COLORREF *)((uint8_t *)obj + 0x58));
    rc->right = w + rc->left;

    if (edge != 0)
        DrawEdge(hdc, rc, 9, 0x200f); /* EDGE_ETCHED */
}

/* ========== PECMD_SetCtlBgBrush @0x1400f0eb0 ==========
 * 设置对象画刷/前景:
 *   - flags==0        -> 仅记前景色
 *   - 否则创建实心画刷 (0xffffffff 用库存白色画刷), 并写样式标志位
 *   - 最后使控件区失效重绘
 * TODO(verify): flags 位段 (0x7fff / bit16) 含义。
 */
void PECMD_SetCtlBgBrush(int64_t obj, COLORREF color, uint64_t flags)
{
    HGDIOBJ old;
    HBRUSH br;

    old = *(HGDIOBJ *)((uint8_t *)obj + OBJ_BRUSH);
    br = 0;
    if (flags == 0) {
        *(COLORREF *)((uint8_t *)obj + OBJ_COLOR) = color;
    }
    else {
        if ((int32_t)color >= 0)
            br = CreateSolidBrush(color);
        *(HBRUSH *)((uint8_t *)obj + OBJ_BRUSH) = br;
        if (g_hStockWhiteBrush == (HGDIOBJ)-1)
            g_hStockWhiteBrush = GetStockObject(5); /* WHITE_BRUSH */
        if (color == 0xffffffff)
            *(HGDIOBJ *)((uint8_t *)obj + OBJ_BRUSH) = g_hStockWhiteBrush;
        if (old != 0 && g_hStockWhiteBrush != old)
            DeleteObject(old);
        if ((flags & 0x7fff) != 0)
            *(int8_t *)((uint8_t *)obj + 0xa4) = (int8_t)flags;
        if (((flags >> 0x10) & 0x7fff) != 0) {
            *(uint8_t *)((uint8_t *)obj + 0x78) &= 0xfc;
            *(uint8_t *)((uint8_t *)obj + 0x78) |= (uint8_t)(flags >> 0x10) & 3;
        }
    }
    InvalidateRect(*(HWND *)((uint8_t *)obj + OBJ_HWND), NULL, 1);
}

/* ========== PECMD_SetHotTrackWindow @0x1400f1448 ==========
 * 切换到新设备窗口: 旧窗口收 WM_0x44C, 新窗口挂 WM_0x1000 刷新消息。
 */
void PECMD_SetHotTrackWindow(HWND hwnd)
{
    if (g_hActiveDevWnd != 0)
        SendMessageW(g_hActiveDevWnd, 0x44c, 0, 0);
    g_hActiveDevWnd = hwnd;
    PostMessageW(hwnd, 0x1000, 0, 0);
}

/* ========== FUN_1400F1BE4 @0x1400f1be4 ==========
 * 初始化"简单窗口对象" (虚表 PTR_FUN_14012c2e0)。
 * 大量初始字段: 光标、滚动、透明、样式字节等。
 * TODO(verify): 各字段含义。
 */
uint64_t *FUN_1400F1BE4(uint64_t *obj, uint64_t data1, uint64_t data2)
{
    FUN_1400E57C0(obj);
    *(uint8_t *)((uint8_t *)obj + OBJ_LINK) &= 0xfc; /* obj+0x1a */
    obj[0x1b] = data1;
    *(uint16_t *)((uint8_t *)obj + 0xa2) = 0xffff;
    obj[0x1c] = data2;
    obj[0] = (uint64_t)&PTR_FUN_14012c2e0;
    *(uint8_t *)((uint8_t *)obj + 0xac) = 0;
    *(uint8_t *)((uint8_t *)obj + 0xa0) = 0; /* obj+0x14 */
    *(uint32_t *)((uint8_t *)obj + 0x94) = 0x80000000;
    *(uint32_t *)((uint8_t *)obj + 0x90) = 0x80000000; /* obj+0x12 */
    obj[0x11] = 0;
    obj[0x10] = 0;
    *(uint8_t *)((uint8_t *)obj + 0x61) = 3;
    if (g_hArrowCursor == 0)
        g_hArrowCursor = LoadCursorW((HINSTANCE)0, (LPCWSTR)0x7f89); /* IDC_ARROW */
    return obj;
}

/* ========== PECMD_ScaleMetricByFactor @0x1400f2a7c ==========
 * 计算滚动目标值: 基点 + (比例 * 量) 的圆整值。
 * 量化到 scroll[1]+0x1c 与 *scroll+4 同步写入。
 * TODO(verify): 滚动参数含义。
 */
uint64_t PECMD_ScaleMetricByFactor(uint64_t *obj, uint64_t wParam, uint64_t *scroll)
{
    typedef uint64_t (*ObjMsgFn)(int64_t self, int msg, uint64_t w, int64_t l);
    int64_t base;
    double v;
    int scale;
    uint64_t r;

    (void)wParam;
    r = ((ObjMsgFn)((void **)*obj)[1])(obj[4], 0x1205, 0, (int64_t)(intptr_t)scroll);
    base = *scroll;
    v = (double)obj[0x25];
    if (v < g_scrollLimit) {
        if (v < 0.0) {
            v = g_fontMinus0 - v;
            goto calc;
        }
        scale = *(int32_t *)(scroll[1] + 0x1c);
    }
    else {
        v = v - g_scrollLimit;
        scale = *(int32_t *)(obj[0x27] + 0x218);
    }
calc:
    v = (double)scale * v + g_dbl25230; /* 四舍五入 +0.5 */
    *(int32_t *)(scroll[1] + 0x1c) = (int32_t)v;
    *(int32_t *)(base + 4) = (int32_t)v;
    return r;
}

/* ========== FUN_1400F429C @0x1400f429c ==========
 * 宽字符游标前进至命中 ch 或 L'\0' 终止符处。
 */
void FUN_1400F429C(WCHAR **pp, WCHAR ch)
{
    WCHAR *p;

    p = *pp;
    if (p != NULL) {
        while (*p != L'\0' && ch != *p) {
            p++;
            *pp = p;
        }
    }
}

/* ========== FUN_1400FB588 @0x1400fb588 ==========
 * 初始化"列表视窗对象"核心 (虚表 PTR_FUN_14012c7b0)。
 * TODO(verify): 各字段含义, 0x74 数据块用途。
 */
uint64_t *FUN_1400FB588(uint64_t *obj, uint64_t data1, uint64_t data2)
{
    FUN_1400E57C0(obj);
    obj[0x11] = 0;
    obj[0x10] = 0;
    obj[0x1c] = data2;
    obj[0] = (uint64_t)&PTR_FUN_14012c7b0;
    *(uint8_t *)((uint8_t *)obj + OBJ_LINK) = 0; /* obj+0x1a */
    obj[0x1b] = data1;
    *(uint8_t *)((uint8_t *)obj + 0xa0) = 0; /* obj+0x14 */
    *(uint8_t *)((uint8_t *)obj + 0xac) = 0;
    *(uint8_t *)((uint8_t *)obj + 0xb8) = 0;          /* obj+0x17 */
    memset(obj + 0x1d, 0, 0x74);                      /* 清 0x74 字节表块 */
    *(uint32_t *)((uint8_t *)obj + OBJ_FLAGS) = 0x74; /* obj+0x1d 容量 */
    *(uint32_t *)((uint8_t *)obj + 0x9c) = 0x80000000;
    *(uint32_t *)((uint8_t *)obj + 0x98) = 0x80000000; /* obj+0x13 */
    *(uint32_t *)((uint8_t *)obj + 0x94) = 0x80000000;
    *(uint32_t *)((uint8_t *)obj + 0x90) = 0x80000000; /* obj+0x12 */
    if (g_hArrowCursor == 0)
        g_hArrowCursor = LoadCursorW((HINSTANCE)0, (LPCWSTR)0x7f89);
    return obj;
}

/* ========== FUN_1400FCF44 @0x1400fcf44 ==========
 * 初始化"编辑对象"核心 (虚表 PTR_FUN_14012ccc0)。
 * TODO(verify): 各字段含义。
 */
uint64_t *FUN_1400FCF44(uint64_t *obj, uint64_t data)
{
    FUN_1400E57C0(obj);
    obj[0x1b] = data;
    *(uint8_t *)((uint8_t *)obj + 0x61) = 1;
    *(uint8_t *)((uint8_t *)obj + 0xd2) = 0;
    obj[0x11] = 0;
    obj[0x10] = 0;
    obj[0] = (uint64_t)&PTR_FUN_14012ccc0;
    *(uint32_t *)((uint8_t *)obj + 0xa8) = 0x80000000; /* obj+0x15 */
    *(uint8_t *)((uint8_t *)obj + 0xa0) = 0;           /* obj+0x14 */
    *(uint16_t *)((uint8_t *)obj + 0xa2) = 0xffff;
    *(uint8_t *)((uint8_t *)obj + 0xac) = 0;
    *(uint8_t *)((uint8_t *)obj + 0xa4) = 0;
    *(uint32_t *)((uint8_t *)obj + 0x9c) = 0x80000000;
    *(uint32_t *)((uint8_t *)obj + 0x98) = 0x80000000; /* obj+0x13 */
    *(uint32_t *)((uint8_t *)obj + 0x94) = 0x80000000;
    *(uint32_t *)((uint8_t *)obj + 0x90) = 0x80000000; /* obj+0x12 */
    *(uint8_t *)((uint8_t *)obj + 0xa5) = 0;
    *(uint8_t *)((uint8_t *)obj + OBJ_LINK) = 0; /* obj+0x1a */
    *(uint8_t *)((uint8_t *)obj + 0xad) = 0;
    *(uint8_t *)((uint8_t *)obj + 0xd1) = 0;
    obj[0x1c] = 0;
    obj[0x1d] = 0;
    if (g_hArrowCursor == 0)
        g_hArrowCursor = LoadCursorW((HINSTANCE)0, (LPCWSTR)0x7f89);
    return obj;
}

/* ========== PECMD_DtorTreeSubObj @0x1400fecfc ==========
 * 销毁"组合对象": 先执行清理回调, 再销毁基类。
 */
void PECMD_DtorTreeSubObj(uint64_t *obj)
{
    obj[0] = (uint64_t)&PTR_FUN_14012cf00;
    if (obj[0x1b] != 0) {
        g_pFnCleanup();
        obj[0x1b] = 0;
    }
    FUN_1400E8940(obj);
}