/* ====================================================================
 * core_b8i.c — B8 中批 1 (1400e0000-1400fffff)
 *
 * 本批新实现函数全部使用人类可读 PECMD_ 名称，原始地址保留在 @0x 注释。
 *
 * 来源: PECMD原始.EXE (x64, ImageBase=0x140000000)
 *   加载 RAS API           FUN_1400E4D94         @0x1400e4d94
 *   追加菜单命令项          FUN_1400E5120  @0x1400e5120
 *   构建菜单树              FUN_1400E5248      @0x1400e5248
 *   UTF-8 转 UTF-16         FUN_1400E6DE8        @0x1400e6de8
 *   查找 RAS 连接           FUN_1400E7664  @0x1400e7664
 *   构建 RAS 条目列表       FUN_1400E7758  @0x1400e7758
 *   分发控件消息            FUN_1400EC500 @0x1400ec500
 *   设置文本/画刷           FUN_1400F20C0     @0x1400f20c0
 *   跟踪 Tooltip 定时器     FUN_1400F2CF0  @0x1400f2cf0
 *   初始化对象(虚表 C550)   FUN_1400F5C74    @0x1400f5c74
 *   应用字体到控件          FUN_1400FB934 @0x1400fb934
 *   设置绘制画刷            FUN_1400FBBB0     @0x1400fbbb0
 *   创建富文本控件          FUN_1400FBCB0 @0x1400fbcb0
 *   启动 Tooltip 定时器     FUN_1400FD764  @0x1400fd764
 *   解析格式标志            FUN_1400FFA38   @0x1400ffa38
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
extern void FUN_14005C828(LPCSTR func, LPCSTR dll, void **out, HMODULE *hmod); /* @0x14005c828 */
extern void FUN_1400669C4(void *script, int64_t value, LPCWSTR key); /* @0x1400669c4 */
extern void FUN_1400633A8(void **ps, int64_t len);               /* @0x1400633a8 */
extern int64_t FUN_1400702F0(int64_t *out, const char *src, uint64_t len); /* @0x1400702f0 */
extern uint64_t PECMD_HangUpRasConnection(uint64_t *state);       /* @0x1400e75cc */
extern void PECMD_SetHotTrackWindow(HWND hwnd);              /* @0x1400f1448 */
extern void *FUN_1400E57C0(void *obj);                   /* @0x1400e57c0 */
extern int64_t *FUN_140063B64(int64_t *arr);                  /* @0x140063b64 */
extern void FUN_1400FD538(HWND hwnd, int mode); /* @0x1400fd538 */
extern void PECMD_InitControlFlags(int64_t obj, uint32_t *buf, int a3,
                                   LPCWSTR text, int a5, int a6,
                                   int a7, uint32_t flags); /* @0x140054380 */

/* ---- 未实现依赖 (extern + TODO(verify)) ---- */
extern void FUN_1400E4F14(HMENU menu, UINT flags, uintptr_t id, uint8_t *item);
extern int FUN_1400F2384(int64_t obj, LPCWSTR cmd, int64_t *out,
                         int64_t script, int flag, uint16_t mode);
extern uint16_t FUN_1400F172C(int64_t *map, int msg, uint64_t wParam, uint64_t *lParam,
                              int64_t hwnd, uint8_t mode, uint64_t *out);
extern void PECMD_AllocStrSlot(WCHAR **ps);
extern int64_t *PECMD_ReplaceStringSlot(int64_t *dst, uint64_t *src);
extern int64_t PECMD_ContainerAppend(int64_t *container);
extern uint64_t PECMD_FindTreeItemByPath(int64_t obj, WCHAR *p, uint64_t *out); /* @0x1400ff414 */
extern int wsprintfW(LPWSTR buf, LPCWSTR fmt, ...);

/* ---- 本批引用的虚表/数据符号 (.rdata) ---- */
extern uint8_t PTR_FUN_14012c550[];
extern WCHAR g_szEmpty[];   /* .rdata 公共表/虚表基址 */

/* ---- 全局数据 (core_globals.c 定义) ---- */
extern uint32_t (*g_pfnRasDial)(void);                    /* DAT_14013e268 RasDialW */
extern uint32_t (*g_pfnRasEnumEntries)(void *, void *, void *, uint32_t *, uint32_t *); /* DAT_14013e270 */
extern uint32_t (*g_pfnRasGetConnectStatus)(uint64_t, uint32_t *); /* DAT_14013e278 */
extern uint32_t (*g_pfnRasGetErrorString)(void);          /* DAT_14013e280 RasGetErrorStringW */
extern uint32_t (*g_pfnRasHangUp)(uint64_t);              /* DAT_14013e288 RasHangUpW */
extern uint32_t (*g_pfnRasEnumConnections)(void *, uint32_t *, uint32_t *); /* DAT_14013e290 */
extern uint32_t g_dpi;                                    /* g_dpi */
extern HCURSOR g_hArrowCursor;                            /* DAT_14013e4d8 */
extern HGDIOBJ g_hStockWhiteBrush;                        /* DAT_14013a858 */
extern HWND g_hActiveDevWnd;                                     /* DAT_14013e400 */
extern uint64_t g_rasState;                               /* DAT_14013e2a0 RAS 等待状态 (B8i 新增) */
extern uint64_t *g_pRasEntryBuf;                          /* DAT_14013e300 条目缓冲指针 (B8i 新增) */

/* ---- 其他文件已实现名称不一致的全局 (本地声明) ---- */
extern uint8_t g_tooltipThreshold;  /* DAT_14013a861 托盘/Tooltip 阈值 */
extern uint8_t g_tooltipCount0;     /* DAT_14013a860 计数器 0 */

/* ========== FUN_1400E4D94 @0x1400e4d94 ==========
 * 延迟加载 RASAPI32.DLL 的 6 个函数指针 (表 E268/E270/E278/E280/E288/E290)。
 */
uint64_t FUN_1400E4D94(uint64_t this_)
{
    int64_t hmod = 0;

    if (g_pfnRasDial == NULL) {
        FUN_14005C828("RasDialW", "RASAPI32.DLL", (void **)&g_pfnRasDial, (HMODULE *)&hmod);
        FUN_14005C828("RasEnumEntriesW", "RASAPI32.DLL", (void **)&g_pfnRasEnumEntries,
                          (HMODULE *)&hmod);
        FUN_14005C828("RasGetConnectStatusW", "RASAPI32.DLL",
                          (void **)&g_pfnRasGetConnectStatus, (HMODULE *)&hmod);
        FUN_14005C828("RasGetErrorStringW", "RASAPI32.DLL", (void **)&g_pfnRasGetErrorString,
                          (HMODULE *)&hmod);
        FUN_14005C828("RasHangUpW", "RASAPI32.DLL", (void **)&g_pfnRasHangUp, (HMODULE *)&hmod);
        FUN_14005C828("RasEnumConnectionsW", "RASAPI32.DLL",
                          (void **)&g_pfnRasEnumConnections, (HMODULE *)&hmod);
    }
    return this_;
}

/* ========== FUN_1400E5120 @0x1400e5120 ==========
 * 向菜单追加一条"命令"项: 由标志字节拼出 MF_ 风格标志, 记 id,
 * 并把 id 写入 "&%s.id" 变量; 若命令文本非空再注册 "_COMMAND#%d:" 命令。
 * TODO(verify): wsprintfW 缺实参 (原始反编译) — 补 "",0 占位。
 */
void FUN_1400E5120(uint8_t *item, uint16_t *pId, HMENU menu,
                             int64_t script, int64_t cmdCtx, int64_t *varTable)
{
    uint32_t flags;
    uint16_t id;
    uint8_t b;
    LPWSTR tmp = NULL;

    b = *item;
    flags = 0;
    if ((b & 1) != 0)
        flags = 3;
    if ((b & 4) != 0)
        flags |= 8;
    if ((b & 2) != 0)
        flags |= 0x800;
    if ((b & 0x20) != 0)
        flags |= 0x20;

    id = *pId;
    *(uint16_t *)(item + 2) = id;
    FUN_1400E4F14(menu, flags, id, item);
    *(HMENU *)(item + 0x20) = menu;
    (*pId)++;

    PECMD_AllocWStringBuffer(&tmp, 100);
    wsprintfW(tmp, WSTR("&%s.id"), WSTR(""));            /* TODO(verify) 缺实参 */
    if (varTable != NULL)
        FUN_1400669C4(varTable, *(uint16_t *)(item + 2), tmp);

    if ((script != 0) && (**(int16_t **)(item + 0x10) != 0)) {
        wsprintfW(tmp, WSTR("_COMMAND#%d:"), 0);         /* TODO(verify) 缺实参 */
        FUN_14006375C(&tmp, *(LPCWSTR *)(item + 0x10));
        FUN_1400F2384(cmdCtx, tmp, varTable, script, 0, 1);
    }
    FUN_14005B104(&tmp);
}

/* ========== FUN_1400E5248 @0x1400e5248 ==========
 * 递归构建菜单树: 0x80 类型为子菜单 (AppendMenuW MF_POPUP), 否则为命令项。
 */
void FUN_1400E5248(int64_t node, uint16_t *pId, HMENU menu,
                         int64_t script, int64_t cmdCtx, int64_t *varTable)
{
    uint64_t i = 0;
    uint64_t seq = 0;
    int n;
    uint8_t *child;
    HMENU sub;

    if (menu == 0)
        menu = CreatePopupMenu();

    n = *(int *)((uint8_t *)node + 4);
    if (0 < n) {
        do {
            child = *(uint8_t **)(*(int64_t *)((uint8_t *)node + 8) + i * 8);
            if (*child == 0x80) {
                int64_t subnode = *(int64_t *)(child + 0x10);
                sub = CreatePopupMenu();
                FUN_1400E5248(subnode, pId, sub, script, cmdCtx, varTable);
                AppendMenuW(menu, 0x10, (uintptr_t)sub, *(LPCWSTR *)(child + 8));
                (*pId)++;
            }
            else {
                FUN_1400E5120(child, pId, menu, script, cmdCtx, varTable);
                *(int *)(child + 0x28) = (int)seq;
            }
            i++;
            seq++;
        } while ((int64_t)i < (int64_t)n);
    }
}

/* ========== FUN_1400E6DE8 @0x1400e6de8 ==========
 * 把 [src, src+len) 的 UTF-8 字节解码为 UTF-16 写入 out, 返回写入的字符数。
 * 按首字节 0x80/0xc0/0xe0/0xf0/0xf8/0xfc 判定序列长度; 非法续字节直接跳过。
 * TODO(verify): 0xfc/0xf8 超长序列的边界处理。
 */
int64_t FUN_1400E6DE8(uint8_t *src, int64_t len, uint16_t *out)
{
    uint8_t *end;
    uint8_t lead, b1, b2;
    uint32_t n;
    int64_t cnt = 0;

    end = src + len;
loop:
    for (;;) {
        while (end <= src)
            return cnt;
        *out = 0;
        lead = *src;
        if (lead < 0xfc) {
            if (lead < 0xf8) {
                if (lead < 0xf0) {
                    if (lead < 0xe0) {
                        n = (uint32_t)(lead < 0x80);
                    }
                    else
                        n = 3;
                }
                else
                    n = 4;
            }
            else
                n = 5;
        }
        else
            n = 6;
        if (n != 0)
            break;
        n = 1;
        src += (int)n;
    }

    if (n == 1) {
        *out = (uint16_t)lead;
        src += 1;
        goto written;
    }
    if (n == 2)
        goto two;
    if (n != 3) {
        /* 4/5/6 字节超长序列: 直接跳过继续 */
        src += (int)n;
        goto loop;
    }

    /* 3 字节序列: 0xE0.. 后跟两个 10xxxxxx 续字节 */
    b1 = src[1];
    b2 = src[2];
    src += 3;
    if (((b1 & 0xc0) == 0x80) && ((b2 & 0xc0) == 0x80)) {
        *(uint8_t *)((uint8_t *)out + 1) = (uint8_t)(((int8_t)b1 >> 2 & 0x0fU) + (uint8_t)(lead * 0x10));
        *(uint8_t *)out = (uint8_t)((b2 & 0x3f) + (uint8_t)(b1 * 0x40));
        goto written;
    }
    goto loop;

two:
    /* 2 字节序列: 0xC0.. 后跟一个 10xxxxxx 续字节 */
    b1 = src[1];
    src += 2;
    if ((b1 & 0xe0) == 0x80) {
        *(uint8_t *)((uint8_t *)out + 1) = (uint8_t)((lead >> 2) & 7);
        *(uint8_t *)out = (uint8_t)((b1 & 0x3f) + (uint8_t)(lead * 0x40));
    written:
        out++;
        cnt++;
    }
    goto loop;
}

/* ========== FUN_1400E7664 @0x1400e7664 ==========
 * 按名称在已建立/建立的 RAS 连接中查找 (支持通配 '*' 首个连接):
 *   - 名称为空: 若已加载 RAS 则等待连接, 返回等待结果
 *   - 否则枚举连接匹配名称, 命中后等待其就绪
 * TODO(verify): 条目结构 (stride 0x56c) 与等待语义。
 */
uint64_t FUN_1400E7664(LPCWSTR name)
{
    uint32_t *blk;       /* local_res18 缓冲块 */
    uint32_t count;
    uint32_t cb;
    uint64_t result = 0;
    int i;

    if (name == NULL || *name == L'\0') {
        if (g_rasState == 0) {
            result = 0;
        }
        else {
            PECMD_AllocString(&g_pRasEntryBuf, 100);
            result = PECMD_HangUpRasConnection(&g_rasState);
        }
    }
    else {
        FUN_1400633A8((void **)&blk, 0x21e30);
        blk[0] = 0x56c;
        cb = 0x21e30;
        count = 0;
        g_pfnRasEnumConnections(blk, &cb, &count);

        i = 0;
        if (0 < (int)count) {
            do {
                if (*name == L'*') {
                    result = PECMD_HangUpRasConnection((uint64_t *)((uint8_t *)blk +
                                                                  (int64_t)i * 0x15b + 1));
                    result &= 0xffffffffU;
                }
                else {
                    if (lstrcmpiW(name, (LPCWSTR)(blk + i * 0x15b + 3)) == 0) {
                        result = PECMD_HangUpRasConnection((uint64_t *)((uint8_t *)blk +
                                                                      (int64_t)i * 0x15b + 1));
                        result &= 0xffffffffU;
                    }
                }
                i++;
            } while ((int)(i) < (int)count);
        }
        FUN_14005B104((WCHAR **)&blk);
    }
    return result;
}

/* ========== FUN_1400E7758 @0x1400e7758 ==========
 * 枚举当前 RAS 连接, 生成 "连接名\r\n连接名..." 的宽字符串 (首项无前缀)。
 * 每次追加前扩容, 返回最终字符串指针。
 */
int64_t FUN_1400E7758(int64_t *out)
{
    uint32_t *blk;      /* undefined4* 缓冲块 */
    uint32_t count;
    uint32_t cb;
    uint16_t *dst;
    int64_t pos = 0;
    int i, len;

    FUN_1400633A8((void **)&blk, 0x21e30);
    *blk = 0x56c;
    count = 0;
    cb = 0x21e30;
    g_pfnRasEnumConnections(blk, &cb, &count);

    for (i = 0; 0 < (int)count && i < (int)count; i++) {
        len = lstrlenW((LPCWSTR)(blk + i * 0x15b + 3));
        PECMD_AllocString(out, len + 3 + pos);
        dst = (uint16_t *)(*out + pos * 2);
        if (i != 0) {
            dst[0] = 0xd;
            dst[1] = 10;
            pos += 2;
            dst += 2;
        }
        memcpy((uint8_t *)dst,
                      (uint8_t *)(blk + i * 0x15b + 3),
                      (len + 1) * 2);
        pos += len;
    }
    int64_t r = *out;
    FUN_14005B104((WCHAR **)&blk);
    return r;
}

/* ========== FUN_1400EC500 @0x1400ec500 ==========
 * 控件消息分发: 转发 WM_0x407, 0x200 走虚表 0x48 槽, 0x233 查映射表;
 * 命中 (返回值 bit2) 返回映射结果, 否则走通用 0x80 分发。
 */
uint64_t PECMD_DispatchControlMsgEc500(int64_t *obj, int64_t pmsg)
{
    typedef void (*ObjMsgFn)(int64_t *, int, uint64_t);
    uint64_t local_res8;
    uint16_t r;

    if (obj[0x1e] != 0)
        SendMessageW(*(HWND *)(obj[0x1e] + OBJ_HWND), 0x407, 0, pmsg);

    if (*(int *)(pmsg + 8) == 0x200)
        ((ObjMsgFn)((void **)*obj)[9])(obj, *(int *)(pmsg + 0x10), local_res8);

    if (*(int *)(pmsg + 8) == 0x233) {
        if (*(int64_t *)(pmsg + 0x18) == 0) {
            g_randState += 2;
            *(uint64_t *)(pmsg + 0x18) = (uint64_t)g_randState;
        }
        local_res8 = 0;
        r = FUN_1400F172C((int64_t *)obj[0x1d], 0x233, *(uint64_t *)(pmsg + 0x10),
                          *(uint64_t **)(pmsg + 0x18), obj[4], 1, &local_res8);
        if ((r & 4) != 0)
            return local_res8 & 0xffffffffU;
    }
    FUN_1400F172C((int64_t *)obj[0x1d], *(int *)(pmsg + 8), *(uint64_t *)(pmsg + 0x10),
                  *(uint64_t **)(pmsg + 0x18), obj[4], 0x80, NULL);
    return 0;
}

/* ========== FUN_1400F20C0 @0x1400f20c0 ==========
 * 按当前主题槽 (对象 +0x94/+0x90 的槽位数组) 设置 DC 的文本/背景色,
 * 必要时重建画刷; 返回画刷句柄。
 */
uint64_t FUN_1400F20C0(int64_t obj, HDC hdc)
{
    uint32_t flags;
    uint32_t w;
    int64_t slot;
    COLORREF c;

    flags = *(uint8_t *)((uint8_t *)obj + OBJ_LINK) & 2;
    w = (uint32_t)GetWindowLongW(*(HWND *)((uint8_t *)obj + OBJ_HWND), -0x10);
    if (flags || ((w >> 0xb & 1) == 0)) {
        if (*(int8_t *)((uint8_t *)obj + 0xa2) >= 0) {
            slot = *(int8_t *)((uint8_t *)obj + 0xa2);
            if (*(int32_t *)((uint8_t *)obj + 0x94 + slot * 8) < 0) {
                c = GetBkColor(hdc);
                *(COLORREF *)((uint8_t *)obj + 0x94 + slot * 8) = c;
                if (*(HGDIOBJ *)((uint8_t *)obj + 0x80) != 0)
                    DeleteObject(*(HGDIOBJ *)((uint8_t *)obj + 0x80));
                *(HBRUSH *)((uint8_t *)obj + 0x80) = CreateSolidBrush(c);
            }
            c = *(COLORREF *)((uint8_t *)obj + 0x90 + slot * 8);
            if ((int32_t)c >= 0)
                SetTextColor(hdc, c);
            c = *(COLORREF *)((uint8_t *)obj + 0x94 + slot * 8);
            if ((int32_t)c >= 0)
                SetBkColor(hdc, c);
            return *(uint64_t *)((uint8_t *)obj + 0x80 + slot * 8);
        }
        if ((*(uint8_t *)((uint8_t *)obj + 0xac) & 1) != 0 &&
            *(int8_t *)((uint8_t *)obj + 0xa0) != 0)
            SetCursor(g_hArrowCursor);
    }
    return 0;
}

/* ========== FUN_1400F2CF0 @0x1400f2cf0 ==========
 * Timer=0x2711 的 Tooltip 跟踪: 鼠标移出窗口则停止并发 0x2a3,
 * 移入则累计计数, 达阈值发 0x2a1。阈值由 DAT_14013a860/861 控制。
 */
void FUN_1400F2CF0(int64_t obj, int64_t msg)
{
    POINT pt[3];
    RECT rc;
    UINT outMsg;

    if (msg == 0x2711 && *(int8_t *)((uint8_t *)obj + 0xa0) > 0) {
        GetCursorPos(pt);
        rc.left = 0;
        rc.top = 0;
        rc.right = 0;
        rc.bottom = 0;
        GetWindowRect(*(HWND *)((uint8_t *)obj + OBJ_HWND), &rc);
        if (PtInRect(&rc, pt[0]) == 0) {
            KillTimer(*(HWND *)((uint8_t *)obj + OBJ_HWND), 0x2711);
            *(uint8_t *)((uint8_t *)obj + 0xa0) = 0;
            g_hActiveDevWnd = 0;
            outMsg = 0x2a3;
        }
        else {
            if (g_tooltipThreshold == *(int8_t *)((uint8_t *)obj + 0xa0))
                PECMD_SetHotTrackWindow(*(HWND *)((uint8_t *)obj + OBJ_HWND));
            if (*(int8_t *)((uint8_t *)obj + 0xa0) <= g_tooltipCount0)
                *(int8_t *)((uint8_t *)obj + 0xa0) =
                    *(int8_t *)((uint8_t *)obj + 0xa0) + 1;
            if (g_tooltipCount0 != *(int8_t *)((uint8_t *)obj + 0xa0))
                return;
            outMsg = 0x2a1;
        }
        PostMessageW(*(HWND *)((uint8_t *)obj + OBJ_HWND), outMsg, 0, 0);
    }
}

/* ========== FUN_1400F5C74 @0x1400f5c74 ==========
 * 初始化"对象" (虚表 PTR_FUN_14012c550): 两个数组 + 引用串容器 +
 * 若干默认字段 (0xab/0xeb/199/1.0)。
 * TODO(verify): 各字段含义。
 */
uint64_t *FUN_1400F5C74(uint64_t *obj)
{
    uint64_t local_res8;

    FUN_1400E57C0(obj);
    obj[0] = (uint64_t)&PTR_FUN_14012c550;
    FUN_140063B64((int64_t *)(obj + 0x1a));        /* 0xd0 */
    FUN_140063B64((int64_t *)(obj + 0x1d));        /* 0xe8 */
    PECMD_AllocStrSlot((WCHAR **)(obj + 0x20));     /* 0x100 引用串容器 */
    *(uint32_t *)((uint8_t *)obj + 0x124) = 0;
    *(uint32_t *)((uint8_t *)obj + 0x118) = 0xab;
    *(uint32_t *)((uint8_t *)obj + 0x11c) = 199;
    *(uint32_t *)((uint8_t *)obj + 0x120) = 0xeb;
    *(uint8_t *)((uint8_t *)obj + 0x130) = 1;
    FUN_1400702F0((int64_t *)&local_res8, (const char *)g_DefEnv, 0xffffffffffffffff);
    PECMD_ReplaceStringSlot((int64_t *)(obj + 0x20), &local_res8);
    FUN_14005B104((WCHAR **)&local_res8);
    *(uint32_t *)((uint8_t *)obj + 0x134) = 0;
    obj[0x22] = 0;
    obj[0x21] = 0;
    obj[0x27] = 0;
    obj[0x25] = 0x3ff0000000000000;                  /* 1.0 */
    return obj;
}

/* ========== FUN_1400FB934 @0x1400fb934 ==========
 * 把字体应用到控件 (WM_0x30 设置字体), 并按 DPI 重算行高后
 * 通过 WM_0x444 让控件重新应用格式。
 */
int64_t FUN_1400FB934(int64_t obj, HANDLE font, int flag)
{
    LRESULT res;
    int lh;
    int64_t local_68;
    uint64_t local_64;
    uint32_t local_58;
    WCHAR local_4c[34];
    int r;

    if (font == 0 || font == (HANDLE)0xffffffffffffffffU) {
        r = (int)font;
    }
    else {
        SendMessageW(*(HWND *)((uint8_t *)obj + OBJ_HWND), 0x30, (WPARAM)font,
                     (uint64_t)(flag != 0));
        lh = 0;
        memset(&local_64, 0, 0x58);
        local_58 = 400;
        (void)local_58;
        GetObjectW(font, 0x5c, &local_68);
        if (lh < 0)
            lh = -lh;
        if (g_dpi > 0)
            lh = (int)((g_dpi / 2 + (uint32_t)lh * 0x48) / g_dpi);
        PECMD_InitControlFlags((int64_t)((uint8_t *)obj + OBJ_FLAGS), (uint32_t *)&lh, lh,
                               local_4c, -1, -1, 0, 0);
        res = SendMessageW(*(HWND *)((uint8_t *)obj + OBJ_HWND), 0x444, 0,
                           (LPARAM)((uint8_t *)obj + OBJ_FLAGS));
        r = (int)res;
    }
    return r;
}

/* ========== FUN_1400FBBB0 @0x1400fbbb0 ==========
 * 绘制前设置画刷 (与 FUN_1400F20C0 同族, 按 short 槽索引):
 * 槽色无效则取 GetBkColor 并重建画刷, 再设置文本/背景色, 返回画刷。
 */
uint64_t FUN_1400FBBB0(int64_t obj, HDC hdc)
{
    HGDIOBJ old;
    COLORREF c;
    HBRUSH br;
    uint64_t result;
    int slot;

    slot = *(int16_t *)((uint8_t *)obj + 0xa2);
    if (slot < 0) {
        if ((*(uint8_t *)((uint8_t *)obj + 0xac) & 1) != 0 &&
            *(int8_t *)((uint8_t *)obj + 0xa0) != 0)
            SetCursor(g_hArrowCursor);
        result = 0;
    }
    else {
        if (*(int32_t *)((uint8_t *)obj + 0x94 + (int64_t)slot * 8) < 0) {
            c = GetBkColor(hdc);
            *(COLORREF *)((uint8_t *)obj + 0x94 + (int64_t)slot * 8) = c;
            old = *(HGDIOBJ *)((uint8_t *)obj + 0x80);
            br = CreateSolidBrush(c);
            *(HBRUSH *)((uint8_t *)obj + 0x80) = br;
            if (old != 0)
                DeleteObject(old);
        }
        c = *(COLORREF *)((uint8_t *)obj + 0x90 + (int64_t)slot * 8);
        if ((int32_t)c >= 0)
            SetTextColor(hdc, c);
        SetBkColor(hdc, *(COLORREF *)((uint8_t *)obj + 0x94 + (int64_t)slot * 8));
        result = *(uint64_t *)((uint8_t *)obj + 0x80 + (int64_t)slot * 8);
    }
    return result;
}

/* ========== FUN_1400FBCB0 @0x1400fbcb0 ==========
 * 创建 RichEdit 控件窗口 (RICHEDIT / RichEdit20W), 存入 obj[4],
 * 追加容器并注册 0x1400f21a8 回调 (当 EDITSTREAM 标志开启)。
 */
bool FUN_1400FBCB0(int64_t *obj, uint32_t style, int *rc,
                              HWND parent, uint32_t id)
{
    int iVar1, iVar2, iVar3, iVar4;
    LONG lparent;
    LPCWSTR cls;
    HWND h;

    iVar4 = rc[0];
    iVar3 = rc[1];
    iVar2 = rc[2];
    iVar1 = rc[3];
    lparent = GetWindowLongW(parent, -6);
    cls = WSTR("RICHEDIT");
    if (g_richEditMode == 2)
        cls = WSTR("RichEdit20W");
    h = CreateWindowExW(0, cls, NULL, style & 0xffffbfff, rc[0], rc[1],
                        iVar2 - iVar4, iVar1 - iVar3, parent,
                        (HMENU)(uintptr_t)id, (HINSTANCE)(intptr_t)lparent, NULL);
    obj[4] = (int64_t)(uintptr_t)h;
    PECMD_ContainerAppend(obj);
    if ((style >> 0xe & 1) != 0)
        SendMessageW((HWND)obj[4], 0xd0, 0, 0x1400f21a8);
    return obj[4] != 0;
}

/* ========== FUN_1400FD764 @0x1400fd764 ==========
 * 启动 Tooltip 定时器 (0x2711, 50ms), 设置内部状态, 然后
 * 经虚表入口把消息 0x200 下发; 需要时切换设备窗口/设光标。
 */
void FUN_1400FD764(int64_t *obj, uint32_t wParam, uint64_t lParam)
{
    typedef uint64_t (*ObjMsgFn)(int64_t, int, uint32_t, int64_t);
    uint32_t lo;
    uint32_t hi;

    if (*(uint8_t *)((uint8_t *)obj + 0xa0) == 0) {
        *(uint8_t *)((uint8_t *)obj + 0xa0) = 1;         /* obj+0x14 */
        SetTimer((HWND)obj[4], 0x2711, 50, NULL);
        if (*(int16_t *)((uint8_t *)obj + 0xa2) >= 0) {
            *(uint16_t *)((uint8_t *)obj + 0xa2) = 1;
            if (*(int8_t *)((uint8_t *)obj + 0xac) > 0)
                *(uint8_t *)((uint8_t *)obj + 0xa5) = *(uint8_t *)((uint8_t *)obj + 0xa4) & 2;
            if (*(int8_t *)((uint8_t *)obj + 0xa1) != 0 ||
                *(int8_t *)((uint8_t *)obj + 0xac) > 0)
                InvalidateRect((HWND)obj[4], NULL, 1);
        }
        if ((int8_t)g_tooltipThreshold < 1)
            PECMD_SetHotTrackWindow((HWND)obj[4]);
        if ((uint64_t)obj[0x11] == (uint64_t)g_hStockWhiteBrush)
            FUN_1400FD538((HWND)obj[4], 1);
    }
    hi = (uint32_t)(lParam >> 0x20);
    lo = (uint32_t)lParam;
    ((ObjMsgFn)((void **)*obj)[1])(obj[4], 0x200, wParam,
                                   (int64_t)(int)(hi << 0x10 | lo));
    if ((*(uint8_t *)((uint8_t *)obj + 0xac) & 7) != 0)
        SetCursor(g_hArrowCursor);
}

/* ========== FUN_1400FFA38 @0x1400ffa38 ==========
 * 解析格式标志串 (前缀 + tailPtr 尾串):
 *   '@' -> bit4, '*' -> bit1, ':' 停止; 数字标记 bit5 并记录尾部@
 * TODO(verify): 标志位精确含义。
 */
void FUN_1400FFA38(int64_t obj, int64_t *cursor, uint8_t *flags,
                            int64_t *tailPtr)
{
    int16_t *p;
    uint16_t *tail;
    int16_t orig;

    orig = *(int16_t *)*tailPtr;
    tail = (uint16_t *)g_szEmpty;
    if (orig != 0) {
        *(int16_t *)*tailPtr = 0;
        *tailPtr += 2;
    }
    while (*(int16_t *)*cursor != 0) {
        p = (int16_t *)*cursor;
        if (*p == 0x40 && 9 < (uint16_t)(p[1] - 0x30U)) {
            *flags |= 0x10;
        }
        else if (*p == 0x2a) {
            *flags |= 2;
        }
        else if (*p != 0x3a) {
            if (((uint16_t)(*p - 0x30U) < 10) ||
                (*p == 0x40 && ((uint16_t)(p[1] - 0x30U) < 10))) {
                *flags |= 0x20;
                tail = (uint16_t *)(intptr_t)*cursor;
            }
            break;
        }
        *cursor += 2;
    }
    p = (int16_t *)*cursor;
    if (*p != 0) {
        while (*(p = p + 1) != 0) {
            if (*p == 0x40) {
                *p = 0;
                *flags |= 0x10;
            }
            else if (*p == 0x2a) {
                *p = 0;
                *flags |= 2;
            }
        }
    }
    while (*(int16_t *)*tailPtr != 0) {
        if (*(int16_t *)*tailPtr == 0x40)
            *flags |= 0x10;
        else {
            if (*(int16_t *)*tailPtr != 0x2a)
                break;
            *flags |= 3;
        }
        *tailPtr += 2;
    }
    if (orig != 0)
        *cursor = *tailPtr;
    PECMD_FindTreeItemByPath(obj, tail, NULL);
}