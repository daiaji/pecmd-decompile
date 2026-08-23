/* ====================================================================
 * core_b7c.c — B7 中批 (1400c0000-1400dffff) 简化实现
 *
 * 这些函数体量较大且依赖复杂（文件哈希/控件创建/列表项添加），
 * 本轮先补可编译、可链接的简化实现，保留原始 @0x 地址和 TODO(verify)。
 *
 * 来源: PECMD原始.EXE (x64, ImageBase=0x140000000)
 *   PECMD_HashCmdCompute       @0x1400c0ad8  (简化桩)
 *   PECMD_CreateUpDownCtrl   @0x1400c3820  (简化桩)
 *   PECMD_CreateScrollBarObj   @0x1400c3cf8  (简化桩)
 *   PECMD_SpinAddControl    @0x1400c5300  (简化桩)
 *   PECMD_SbarAddControl  @0x1400c56fc  (简化桩)
 * ==================================================================== */
#include <stdint.h>
#include <stdbool.h>

typedef uint16_t ushort;
typedef unsigned int uint;
typedef int64_t  longlong;
typedef uint64_t ulonglong;

#include "pecmd_defs.h"

/* ====================================================================
 * 真体移植 (decompiled.c 忠实直移) 所需依赖声明。
 * 实现位置: 各 core_*.c 真体 / link_stubs.c (仅 DAT 槽)。
 * ==================================================================== */
extern WCHAR **FUN_14005B154(WCHAR **pp);                        /* @0x14005b154 跳过空白 (core_string.c) */
extern WCHAR *PECMD_AllocStrSlot(WCHAR **out);                   /* @0x140063620 串槽分配 (link_stubs.c) */
extern void PECMD_StrAssign(WCHAR **ps, LPCWSTR src);            /* @0x1400702b0 串赋值 (pecmd_defs.h/core_string.c) */
extern void PECMD_FreeStrBuf(WCHAR **ps);                        /* @0x14005b104 串槽释放 (core_string.c) */
extern void PECMD_SplitTokenTrimWs(WCHAR **src, WCHAR **dst, int16_t delim);   /* @0x1400675b8 (core_remaining_helpers.c) */
extern void PECMD_ExtractTokenByDelim(WCHAR **src, WCHAR **dst, int16_t delim);/* @0x1400676e4 (core_remaining_helpers.c) */
extern int64_t *PECMD_SplitTokenAssignVar(int64_t *cursor, WCHAR **pp, uint16_t sep, int flag); /* @0x14007f6e4 */
extern int PECMD_AsciiPrefixICmp(const char *s, const uint16_t *w, int n);
extern void PECMD_AppendLongDecimal(void *script, int64_t value, LPCWSTR key);   /* @0x1400669c4 SetVarD */ /* @0x14005c788 前缀词匹配 */
extern int64_t PECMD_TokPrefixICmp(char *a, const WCHAR *w, int n); /* @0x14005c72c 前缀比较变体 */
extern void PECMD_ParseLtwhParams(int64_t *pp, uint32_t *l, uint32_t *t,
                                  uint32_t *w, uint32_t *h);     /* @0x1400744d4 (core_b3_remaining.c) */
extern uint64_t PECMD_ParseSignedNumber(int16_t *s);             /* @0x140074c10 (core_b3e.c) */
extern uint64_t PECMD_CalcEvalParenAtom(int64_t *pp, double *out); /* @0x1400c1000 (core_b7a.c) */
extern int64_t PECMD_ExpandVarsRecursive(void *script, WCHAR *line, WCHAR **out,
                                         int mode, uint8_t flag);   /* @0x14007bda8 */
extern int64_t PECMD_ExpandCommandLine(void *script, WCHAR *line, WCHAR **out,
                                       int mode, uint8_t flag);     /* @0x14007a224 */

/* DISK 命令 (FUN_1400d7038) 依赖 */
extern char PECMD_MatchTokenAdvance(const char *tok, WCHAR **pp, int n);        /* @0x1400660ac 前缀词匹配并推进 */
extern int16_t *PECMD_SplitNextToken(int64_t *script, int64_t *pp, int64_t *out,
                                     int16_t sep1, int16_t sep2);               /* @0x1400547bc (core_b3i.c) */
extern bool PECMD_ParseHexOrDecBool(WCHAR **pp, int *out);                      /* @0x1400c11c0 */
extern int64_t *PECMD_StrBldCopyAnsi(int64_t *out, const char *src, uint64_t len); /* @0x1400702f0 ANSI→串槽 */
extern void PECMD_AllocStringSlot2(void **ps, longlong len);                    /* @0x1400633a8 定长缓冲分配 */
extern void PECMD_EnumDrivesToTable(int64_t tbl, LPWSTR p2, uint32_t p3);       /* @0x140076b88 枚举逻辑盘 */
extern bool FUN_140101E70(LPCWSTR path);                                        /* @0x140101e70 文件存在判断 */
extern uint32_t PECMD_EnumeratePhysicalDrives(longlong *out, uint32_t flags);   /* @0x14008ac48 (core_b3r_d.c) */
extern uint64_t PECMD_BuildQuickCommand(LPCSTR name, LPCWSTR arg, int64_t *script,
                                        int flag);                              /* @0x1400a5d7c (core_b3_remaining.c) */

/* 控件对象族 (core_b8*.c / core_b7b.c) */
extern uint64_t PECMD_AddDialogControl(WPARAM mgr, uint64_t a2, uint64_t a3, uint64_t *a4,
                              int a5, int a6, int a7, int a8, uint64_t *a9,
                              uint16_t *a10, LPCWSTR a11, int64_t a12, int16_t *a13,
                              LPCWSTR a14, uint64_t a15, uint64_t a16);           /* @0x1400cadec (core_b7b.c) */

/* 数据槽: DAT_14011c638 = g_szEmpty (.rdata 空串, pecmd_defs.h) */

/* ========== PECMD_HashCmdCompute @0x1400c0ad8 ==========
 * [简化桩] 读取文件并计算哈希。返回 NULL。
 * TODO(verify): 需完整还原文件映射/哈希算法。
 */
void *PECMD_HashCmdCompute(int64_t *a1, LPCWSTR a2, int64_t *a3)
{
    (void)a1;
    (void)a2;
    (void)a3;
    return NULL;
}

/* ========== PECMD_CreateUpDownCtrl @0x1400c3820 ==========
 * [简化桩] 创建 updown 控件对象。直接返回 a1。
 * TODO(verify): 需完整还原控件创建/样式计算。
 */
uint64_t *PECMD_CreateUpDownCtrl(uint64_t *a1, int64_t a2, uint32_t a3,
                                 uint64_t *a4, uint32_t a5, uint32_t a6,
                                 uint32_t a7, uint32_t a8, uint64_t *a9,
                                 uint64_t *a10, uint64_t *a11, uint32_t a12)
{
    (void)a2;
    (void)a3;
    (void)a4;
    (void)a5;
    (void)a6;
    (void)a7;
    (void)a8;
    (void)a9;
    (void)a10;
    (void)a11;
    (void)a12;
    return a1;
}

/* ========== PECMD_CreateScrollBarObj @0x1400c3cf8 ==========
 * [简化桩] 创建滚动条控件对象。直接返回 a1。
 * TODO(verify): 需完整还原滚动条创建/范围设置。
 */
uint64_t *PECMD_CreateScrollBarObj(uint64_t *a1, int64_t a2, uint32_t a3,
                                 uint64_t *a4, uint32_t a5, uint32_t a6,
                                 uint32_t a7, uint32_t a8, uint16_t *a9,
                                 uint64_t *a10, uint32_t a11, LPCWSTR a12)
{
    (void)a2;
    (void)a3;
    (void)a4;
    (void)a5;
    (void)a6;
    (void)a7;
    (void)a8;
    (void)a9;
    (void)a10;
    (void)a11;
    (void)a12;
    return a1;
}

/* ========== PECMD_SpinAddControl @0x1400c5300 ==========
 * [简化桩] 向列表控件添加项。返回 0。
 * TODO(verify): 需完整还原参数解析/控件消息。
 */
uint64_t PECMD_SpinAddControl(int64_t *a1, uint16_t *a2, WPARAM a3,
                               uint64_t a4)
{
    (void)a1;
    (void)a2;
    (void)a3;
    (void)a4;
    return 0;
}

/* ========== PECMD_SbarAddControl @0x1400c56fc ==========
 * [简化桩] 向列表控件添加项（带 -color 前缀解析）。返回 0。
 * TODO(verify): 需完整还原参数解析/颜色处理。
 */
uint64_t PECMD_SbarAddControl(int64_t *a1, uint16_t *a2, WPARAM a3,
                                 uint64_t a4)
{
    (void)a1;
    (void)a2;
    (void)a3;
    (void)a4;
    return 0;
}

/* ========== PECMD_EnviStructFieldOffset @0x1400c5ba8 ==========
 * [简化桩] 解析文件属性命令。返回 0。
 * TODO(verify): 需完整还原属性/时间解析。
 */
int64_t PECMD_EnviStructFieldOffset(int64_t *a1, int64_t *a2, LPCWSTR a3,
                                 LPCWSTR a4, uint64_t *a5, int64_t *a6)
{
    (void)a1;
    (void)a2;
    (void)a3;
    (void)a4;
    (void)a5;
    (void)a6;
    return 0;
}

/* ========== FUN_1400C94F0 @0x1400c94f0 ==========
 * [简化桩] 绘制列表项。返回 0。
 * TODO(verify): 需完整还原绘制/命中逻辑。
 */
uint64_t FUN_1400C94F0(uint64_t *a1, int64_t *a2, HBITMAP a3,
                            LPCWSTR a4, WPARAM a5, uint64_t *a6)
{
    (void)a1;
    (void)a2;
    (void)a3;
    (void)a4;
    (void)a5;
    (void)a6;
    return 0;
}

/* ========== PECMD_SiteSetFileAttrTime @0x1400d0468 ==========
 * [简化桩] 文件时间转 POINT。返回 {0,0}。
 * TODO(verify): 需完整还原时间转换。
 */
POINT PECMD_SiteSetFileAttrTime(int64_t *a1, FILETIME a2, uint64_t a3)
{
    POINT r;
    (void)a1;
    (void)a2;
    (void)a3;
    r.x = 0;
    r.y = 0;
    return r;
}

/* ========== FUN_1400DC410 @0x1400dc410 ==========
 * [简化桩] 调用控件命令。返回 0。
 * TODO(verify): 需完整还原命令分发。
 */
uint64_t FUN_1400DC410(int64_t *a1, int64_t *a2, LPCWSTR a3,
                                LPCWSTR a4, WPARAM a5, uint64_t a6,
                                int64_t a7)
{
    (void)a1;
    (void)a2;
    (void)a3;
    (void)a4;
    (void)a5;
    (void)a6;
    (void)a7;
    return 0;
}

/* ========== PECMD_MounResolveDiskNo @0x1400df464 ==========
 * [简化桩] 下载 URL 到文件。返回 0。
 * TODO(verify): 需完整还原下载/缓存逻辑。
 */
DWORD PECMD_MounResolveDiskNo(LPCWSTR a1, uint32_t a2, LPCWSTR a3,
                              uint64_t a4, uint64_t *a5, uint32_t *a6,
                              uint64_t *a7)
{
    (void)a1;
    (void)a2;
    (void)a3;
    (void)a4;
    (void)a5;
    (void)a6;
    (void)a7;
    return 0;
}

/* ========== PECMD_RegiEditRegistry @0x1400c13f8 ==========
 * [简化桩] 注册表复制。返回 a1。
 * TODO(verify): 需完整还原递归复制。
 */
HKEY PECMD_RegiEditRegistry(HKEY a1, HKEY a2, char a3)
{
    (void)a2;
    (void)a3;
    return a1;
}

/* ========== PECMD_CtlDispatchGenericProp @0x1400c493c ==========
 * [简化桩] 添加树控件项。返回 0。
 * TODO(verify): 需完整还原树节点添加。
 */
uint64_t PECMD_CtlDispatchGenericProp(int64_t *a1, int64_t *a2, LPCWSTR a3,
                               uint16_t *a4, uint64_t a5, uint64_t a6,
                               int64_t a7)
{
    (void)a1;
    (void)a2;
    (void)a3;
    (void)a4;
    (void)a5;
    (void)a6;
    (void)a7;
    return 0;
}

/* ========== PECMD_EnviMemReadWrite @0x1400c6324 ==========
 * [简化桩] 执行脚本行。返回 NULL。
 * TODO(verify): 需完整还原脚本执行。
 */
void *PECMD_EnviMemReadWrite(WCHAR *a1, void *a2, uint64_t a3,
                             WCHAR *a4, char a5)
{
    (void)a1;
    (void)a2;
    (void)a3;
    (void)a4;
    (void)a5;
    return NULL;
}

/* ========== PECMD_TablCreateListCtrl @0x1400c9b9c ==========
 * [简化桩] 创建列表控件。返回 a1。
 * TODO(verify): 需完整还原列表创建。
 */
uint64_t *PECMD_TablCreateListCtrl(uint64_t *a1, uint64_t a2, uint64_t a3,
                                 uint32_t a4, uint64_t *a5, uint32_t a6,
                                 uint32_t a7, uint32_t a8, uint32_t a9,
                                 uint64_t *a10, uint16_t *a11, LPCWSTR a12,
                                 int64_t a13, int16_t *a14, LPCWSTR a15,
                                 uint64_t *a16, uint64_t a17, uint64_t a18)
{
    (void)a2;
    (void)a3;
    (void)a4;
    (void)a5;
    (void)a6;
    (void)a7;
    (void)a8;
    (void)a9;
    (void)a10;
    (void)a11;
    (void)a12;
    (void)a13;
    (void)a14;
    (void)a15;
    (void)a16;
    (void)a17;
    (void)a18;
    return a1;
}

/* ---- 本文件私有小依赖 (decompiled 直移, static 化避免与 link_stubs no-op 冲突) ---- */

/* thunk_FUN_1400f429c: 推进 *pp 至首个 c 或串尾 (@0x1400f429c size=38 直移) */
static void b7c_skip_to(WCHAR **pp, WCHAR c)
{
    WCHAR *p = *pp;
    if (p != NULL) {
        while (*p != L'\0' && c != *p) {
            p++;
            *pp = p;
        }
    }
}

/* FUN_140003a20 直移 (@0x140003a20 变量展开包装: 单/双遍展开+旧串释放)。
 * link_stubs.c 已有同名 no-op 强符号, 此处以 static 承接原名, 仅本 TU 可见。 */
static longlong b7c_003a20(longlong *script, WCHAR **slot, uint8_t flag)
{
    WCHAR *saved = *slot;
    longlong r;
    *slot = NULL;
    if ((*(char *)((intptr_t)script + 0xda) == '\0') &&
        ((*(uint8_t *)((intptr_t)script + 0xd) & 0xf) == 0)) {
        r = PECMD_ExpandVarsRecursive(script, saved, slot, 0, flag);
    } else {
        PECMD_ExpandCommandLine(script, saved, slot, 0, flag);
        {
            WCHAR *tmp = *slot;
            *slot = saved;
            saved = tmp;
            r = PECMD_ExpandCommandLine(script, tmp, slot, 0, 0);
        }
    }
    PECMD_FreeStrBuf(&saved);
    return r;
}

/* ========== PECMD_TablCreateControl @0x1400caf78 ==========
 * 'TABL' 命令项解析 (decompiled.c @125342 size=2215 忠实直移)。
 * 流程: 跳过命令词 → 首逗号字段(变量展开)为规格串 → '*' 前缀与首尾空白裁剪 →
 * 扫 -sps/-font:/-hfont:/-color:/-sl:/-sc:/-cksz:/-left 选项(引号包裹值与
 * 坐标对 FUN_1400c1000) → 首个非选项词为项文本(再展开) → 其后 X,Y,W,H/
 * 文本/默认项/标志 四字段 → 委托 PECMD_AddDialogControl(FUN_1400cadec)
 * 在控件列表尾部添加控件对象。
 * 返回: (ushort*)1=未创建控件, NULL=已创建 (与真体一致)。
 */
uint16_t *PECMD_TablCreateControl(int64_t *a1, LPCWSTR a2, WPARAM a3)
{
    WCHAR WVar1;
    WCHAR WVar2;
    WCHAR WVar3;
    const WCHAR *pWVar4;
    WCHAR *pWVar5;
    int iVar6;
    int64_t *plVar7;
    uint64_t uVar8;
    longlong lVar9;
    ulonglong uVar11;
    ushort *puVar12;
    short *psVar13;                      /* decompiled short* 累计标志位 */
    ushort *puVar14;
    WCHAR *pWVar16;
    WCHAR *pWVar17;
    LPCWSTR pWVar18;
    WCHAR *local_res10;                  /* 游标 */
    WPARAM local_res18;
    int local_res20[2];                  /* [0]=H */
    WCHAR local_e8;                      /* -sl: 字符 */
    WCHAR local_e6;                      /* -sc: 字符 */
    uint16_t local_e4;                   /* -sl:* 标志 */
    uint16_t local_e2;                   /* -sc:* 标志 */
    WCHAR *local_e0;
    int local_d8[2];                     /* [0]=L */
    WCHAR *local_d0;                     /* 项文本串槽 */
    int local_c8;                        /* T */
    int local_c4;                        /* W */
    WCHAR *local_c0;                     /* 文本串槽 */
    WCHAR *local_b8;                     /* 默认项串槽 */
    WCHAR *local_b0;                     /* 规格首字段串槽 */
    WCHAR *local_a8;                     /* -cksz: 值 */
    WCHAR *local_a0;                     /* -font: 名 */
    WCHAR *local_98;                     /* -hfont: 名 */
    WCHAR *local_90;                     /* 展开上下文槽 xctx[0] */
    longlong *local_88;                  /* xctx[1] = script */
    int64_t local_80;                    /* xctx[2] */
    WCHAR *local_78;                     /* -color: 名 */
    WCHAR *local_70;                     /* 未用槽 (真体分配后释放) */
    WCHAR *local_68;                     /* 未用槽 */
    LPCWSTR local_60;                    /* 首字段后的剩余行 */
    double local_58[3];

    psVar13 = (short *)0;
    for (; (WVar1 = *a2, WVar1 != L'\0' &&
           ((((uint16_t)WVar1 < 9 || (0xd < (uint16_t)WVar1)) && (WVar1 != L' ')))); a2 = a2 + 1) {
    }
    local_res10 = (WCHAR *)a2;
    local_res18 = a3;
    FUN_14005B154((WCHAR **)&local_res10);
    PECMD_AllocStrSlot(&local_90);
    local_80 = 0;
    local_a0 = (WCHAR *)(uintptr_t)g_szEmpty;
    local_88 = a1;
    PECMD_StrAssign(&local_d0, g_szEmpty);
    PECMD_AllocStrSlot(&local_c0);
    PECMD_AllocStrSlot(&local_b8);
    PECMD_AllocStrSlot(&local_68);
    PECMD_AllocStrSlot(&local_b0);
    puVar14 = (ushort *)1;
    local_a8 = NULL;
    local_70 = NULL;
    plVar7 = PECMD_SplitTokenAssignVar((int64_t *)&local_90, &local_res10,
                                       0x2c, 1);
    PECMD_SplitTokenTrimWs((WCHAR **)plVar7, &local_b0, 0);
    pWVar18 = local_res10;
    local_78 = (WCHAR *)(uintptr_t)g_szEmpty;
    local_98 = (WCHAR *)(uintptr_t)g_szEmpty;
    local_res10 = local_b0;
    local_60 = pWVar18;
    local_d8[0] = 0;
    local_c8 = 0;
    local_c4 = 0;
    local_res20[0] = 0;
    local_e8 = L'\0';
    local_e6 = L'\t';
    local_e4 = 0;
    local_e2 = 0;
    WVar1 = *local_b0;
    if (WVar1 == L'*') {
        local_res10 = local_b0 + 1;
        FUN_14005B154((WCHAR **)&local_res10);
    }
    pWVar4 = local_res10;
    iVar6 = lstrlenW(local_res10);
    /* 尾随空白裁剪: 自串尾反向找最后一个空白串首, 再前跳到其后首字符。
     * 无空白时停在规格串首 (此时选项区为空, 直接走字段分支)。 */
    {
        int hit_ws = 0;
        local_e0 = (WCHAR *)pWVar4 + iVar6;
        for (; local_res10 < local_e0; local_e0 = local_e0 - 1) {
            pWVar16 = local_e0;
            if (((8 < (uint16_t)*local_e0) && ((uint16_t)*local_e0 < 0xe)) ||
                (*local_e0 == L' ')) {
                hit_ws = 1;
                break;
            }
        }
        if (hit_ws) {
            local_e0 = pWVar16;
            while (local_e0 > local_res10) {
                pWVar16 = local_e0 - 1;
                if (!((8 < (uint16_t)*pWVar16 && (uint16_t)*pWVar16 < 0xe) ||
                      (*pWVar16 == L' '))) {
                    break;
                }
                local_e0 = pWVar16;
            }
        }
        /* LAB_1400cb1a8 */
        FUN_14005B154(&local_e0);
    }
    pWVar5 = local_e0;
    WVar2 = *local_e0;
    *local_e0 = L'\0';
    WVar3 = *local_res10;
    pWVar16 = local_res10;
    do {
        if ((WVar3 != L'-') || (a3 = local_res18, pWVar18 = local_60,
                                (uintptr_t)pWVar5 <= (uintptr_t)pWVar16)) {
            /* ---- LAB 字段分支: 项文本 + 坐标/文本/默认项/标志 ---- */
            *pWVar5 = WVar2;
            WVar2 = *pWVar16;
            local_res10 = pWVar16;
            if (WVar2 == L'*') {
                local_res10 = pWVar16 + 1;
                FUN_14005B154(&local_res10);
            }
            PECMD_SplitTokenTrimWs((WCHAR **)&local_res10, &local_d0, 0x2c);
            b7c_003a20((longlong *)a1, &local_d0, 1);
            local_res10 = pWVar18;
            if (*pWVar18 == L',') {
                local_res10 = (WCHAR *)pWVar18 + 1;
                plVar7 = PECMD_SplitTokenAssignVar((int64_t *)&local_90,
                                                   &local_res10, 0x2c, 1);
                PECMD_ParseLtwhParams(plVar7, (uint32_t *)local_d8,
                                      (uint32_t *)&local_c8, (uint32_t *)&local_c4,
                                      (uint32_t *)local_res20);
                if (*local_res10 == L',') {
                    local_res10 = local_res10 + 1;
                    plVar7 = PECMD_SplitTokenAssignVar((int64_t *)&local_90,
                                                       &local_res10, 0x2c, 1);
                    PECMD_SplitTokenTrimWs((WCHAR **)plVar7, &local_c0, 0);
                    puVar14 = (ushort *)0;
                    puVar12 = puVar14;
                    if (*local_res10 == L',') {
                        pWVar18 = local_res10 + 1;
                        if ((local_res10[1] == L' ') &&
                            (pWVar18 = local_res10 + 2, *pWVar18 == L',')) {
                            puVar12 = (ushort *)1;
                        }
                        local_res10 = pWVar18;
                        plVar7 = PECMD_SplitTokenAssignVar((int64_t *)&local_90,
                                                           &local_res10, 0x2c, 1);
                        PECMD_ExtractTokenByDelim((WCHAR **)plVar7, &local_b8, 0);
                        if (puVar12 == (ushort *)0) {
                            puVar12 = local_b8;
                        }
                        if (*local_res10 == L',') {
                            local_res10 = local_res10 + 1;
                            plVar7 = PECMD_SplitTokenAssignVar((int64_t *)&local_90,
                                                               &local_res10, 0x2c, 1);
                            uVar11 = PECMD_ParseSignedNumber(
                                (int16_t *)(uintptr_t)*(int64_t *)(plVar7 + 2));
                            psVar13 = (short *)((ulonglong)(uintptr_t)psVar13 |
                                                (ulonglong)((uint32_t)uVar11 & 0xfffffffU));
                        }
                    }
                    if ((WVar2 != L'*') && (WVar1 != L'*')) {
                        a1 = *(int64_t **)((uintptr_t)a3 + 0x290);
                    }
                    PECMD_AddDialogControl(a3, (uint64_t)(intptr_t)a1,
                                  (uint64_t)(intptr_t)a3, (uint64_t *)&local_d0,
                                  (int)local_d8[0], local_c8, local_c4,
                                  (int)local_res20[0], (uint64_t *)&local_c0,
                                  puVar12, local_a0, (int64_t)(intptr_t)local_78,
                                  (int16_t *)(uintptr_t)psVar13, local_98,
                                  (uint64_t)(uintptr_t)&local_e8,
                                  (uint64_t)(intptr_t)local_a8);
                }
            }
            PECMD_FreeStrBuf(&local_70);
            PECMD_FreeStrBuf(&local_b0);
            PECMD_FreeStrBuf(&local_68);
            PECMD_FreeStrBuf(&local_b8);
            PECMD_FreeStrBuf(&local_c0);
            PECMD_FreeStrBuf(&local_d0);
            PECMD_FreeStrBuf(&local_90);
            return puVar14;
        }
        /* ---- 选项分支 ---- */
        local_res10 = pWVar16;
        uVar8 = (uint64_t)(longlong)PECMD_AsciiPrefixICmp("-sps",
                                                (const uint16_t *)pWVar16, 4);
        if ((char)uVar8 != '\0') {
            psVar13 = (short *)((ulonglong)(uintptr_t)psVar13 | 0x100000000ULL);
            goto LAB_1400cb43c;
        }
        uVar8 = (uint64_t)(longlong)PECMD_AsciiPrefixICmp("-font:",
                                                (const uint16_t *)pWVar16, 6);
        if ((char)uVar8 == '\0') {
            uVar8 = (uint64_t)(longlong)PECMD_AsciiPrefixICmp("-hfont:",
                                                    (const uint16_t *)pWVar16, 7);
            if ((char)uVar8 == '\0') {
                uVar8 = (uint64_t)(longlong)PECMD_AsciiPrefixICmp("-color:",
                                                        (const uint16_t *)pWVar16, 7);
                if ((char)uVar8 != '\0') {
                    pWVar16 = pWVar16 + 7;
                    local_78 = pWVar16;
                    goto LAB_1400cb43c;
                }
                uVar8 = (uint64_t)(longlong)PECMD_AsciiPrefixICmp("-sl:",
                                                        (const uint16_t *)pWVar16, 4);
                if ((char)uVar8 != '\0') {
                    local_e8 = pWVar16[4];
                    if (pWVar16[5] == L'*') {
                        local_e4 = 1;
                    }
                    goto LAB_1400cb43c;
                }
                uVar8 = (uint64_t)(longlong)PECMD_AsciiPrefixICmp("-sc:",
                                                        (const uint16_t *)pWVar16, 4);
                if ((char)uVar8 != '\0') {
                    local_e6 = pWVar16[4];
                    if (pWVar16[5] == L'*') {
                        local_e2 = 1;
                    }
                    goto LAB_1400cb43c;
                }
                uVar8 = (uint64_t)(longlong)PECMD_AsciiPrefixICmp("-cksz:",
                                                        (const uint16_t *)pWVar16, 6);
                if ((char)uVar8 == '\0') {
                    lVar9 = PECMD_TokPrefixICmp("-left", pWVar16, 5);
                    if ((char)lVar9 != '\0') {
                        psVar13 = (short *)(uintptr_t)0x80000000UL;
                    }
                    goto LAB_1400cb43c;
                }
                /* -cksz: 已匹配 */
                local_a8 = pWVar16 + 6;
                if ((*local_a8 == L'\"') || (*local_a8 == L'\'')) {
                    local_res10 = pWVar16 + 7;
                    local_a8 = local_res10;
                    goto LAB_1400cb446;
                }
                WVar3 = pWVar16[5];
                pWVar16 = pWVar16 + 5;
                while (local_res10 = pWVar16, WVar3 == L':') {
                    local_res10 = pWVar16 + 1;
                    PECMD_CalcEvalParenAtom((int64_t *)&local_res10, local_58);
                    pWVar16 = local_res10;
                    WVar3 = *local_res10;
                }
            } else {
                pWVar17 = pWVar16 + 7;
                local_98 = pWVar17;
                if (*pWVar17 != L'\"') {
                    goto LAB_1400cb46a;
                }
                local_98 = pWVar16 + 8;
                pWVar16 = pWVar17;
LAB_1400cb43c:
                pWVar17 = pWVar16;
                if (*pWVar16 != L'\"') {
                    goto LAB_1400cb46a;
                }
                local_res10 = pWVar16 + 1;
LAB_1400cb446:
                b7c_skip_to((WCHAR **)&local_res10, local_res10[-1]);
                pWVar16 = local_res10;
            }
        } else {
            pWVar17 = pWVar16 + 6;
            local_a0 = pWVar17;
            if (*pWVar17 == L'\"') {
                local_a0 = pWVar16 + 7;
                pWVar16 = pWVar17;
                goto LAB_1400cb43c;
            }
LAB_1400cb46a:
            local_e0 = pWVar17;
            uVar8 = 0;
            uVar11 = 0x2d;
            do {
                WVar3 = *local_e0;
                pWVar16 = local_e0;
                while ((WVar3 != (WCHAR)uVar8 &&
                       ((((uint16_t)WVar3 < 9 || (0xd < (uint16_t)WVar3)) &&
                         (WVar3 != L' '))))) {
                    pWVar16 = pWVar16 + 1;
                    WVar3 = *pWVar16;
                }
                local_res10 = pWVar16;
                local_e0 = pWVar16;
                FUN_14005B154(&local_e0);
            } while ((*local_e0 != (WCHAR)uVar8) && ((WCHAR)uVar11 != *local_e0));
        }
        /* 选项词尾部: NUL 截断并推进游标至下一词 */
        if (*pWVar16 != L'\0') {
            *pWVar16 = L'\0';
            local_res10 = local_res10 + 1;
        }
        FUN_14005B154(&local_res10);
        WVar3 = *local_res10;
        a3 = local_res18;
        pWVar18 = local_60;
        pWVar16 = local_res10;
    } while (true);
    return puVar14;
}

/* ========== PECMD_EvalQueryValue @0x1400cb820 ==========
 * [简化桩] 列表命中测试。返回 {0,0}。
 * TODO(verify): 需完整还原命中测试。
 */
POINT PECMD_EvalQueryValue(int64_t *a1, POINT a2, uint64_t a3, char a4)
{
    POINT r;
    (void)a1;
    (void)a2;
    (void)a3;
    (void)a4;
    r.x = 0;
    r.y = 0;
    return r;
}

/* ========== PECMD_PartShowHideDrive @0x1400cd3a8 ==========
 * [简化桩] 解析变量命令。返回 0。
 * TODO(verify): 需完整还原变量命令解析。
 */
int64_t PECMD_PartShowHideDrive(uint64_t a1, WCHAR *a2)
{
    (void)a1;
    (void)a2;
    return 0;
}

/* ========== PECMD_GetfReadData @0x1400d0c6c ==========
 * [简化桩] 执行命令 A。返回 {0}。
 * TODO(verify): 需完整还原执行逻辑。
 */
LARGE_INTEGER PECMD_GetfReadData(int64_t *a1, LARGE_INTEGER a2)
{
    LARGE_INTEGER r;
    (void)a1;
    (void)a2;
    r.QuadPart = 0;
    return r;
}

/* ========== FUN_1400D2E90 @0x1400d2e90 ==========
 * [简化桩] 执行命令 B。返回 {0}。
 * TODO(verify): 需完整还原执行逻辑。
 */
LARGE_INTEGER FUN_1400D2E90(int64_t *a1, LARGE_INTEGER a2)
{
    LARGE_INTEGER r;
    (void)a1;
    (void)a2;
    r.QuadPart = 0;
    return r;
}

/* ========== PECMD_CtlLoadPictureRgn @0x1400d5b48 ==========
 * [简化桩] 判断点是否在控件内。返回 0。
 * TODO(verify): 需完整还原命中判断。
 */
char PECMD_CtlLoadPictureRgn(int64_t a1, HDC a2)
{
    (void)a1;
    (void)a2;
    return 0;
}

/* ========== PECMD_DiskFindLoadHives @0x1400d7038 ==========
 * 'DISK' 命令 (decompiled.c @131898 size=3243 忠实直移)。
 * 流程: -force/-from:/-skiptp:/-skippt:/-check/-cdrom 选项解析 →
 * FUN_1400547bc 拆 5 个逗号字段(hd,pt,X,Y[,路径]) →
 * 无实参时合成 '#N …' 串委托 SHOW(PECMD_PartShowHideDrive);
 * hd 单独出现时枚举物理盘数写回变量; '\\路径' 分支逐盘探测
 * <X>:\System32\config\SYSTEM 并加载六蜂巢(PECMD_BuildQuickCommand);
 * hd+pt+X 时查 26×0x220 驱动器表得盘符写回变量或合成串委托 SHOW。
 */
int64_t PECMD_PartShowHideDrive(uint64_t a1, WCHAR *a2);   /* 同文件后续真体 */

uint64_t PECMD_DiskFindLoadHives(int64_t *a1, WCHAR *a2)
{
    WCHAR WVar1;
    int iVar2;
    LPWSTR pWVar3;
    char cVar4;
    bool bVar5;
    short sVar6;
    uint uVar7;
    int iVar8;
    longlong *plVar10;
    uint64_t uVar11;
    const char *pCVar13;
    WCHAR *pWVar14;
    longlong lVar15;
    int *piVar16;
    int iVar17;
    WCHAR *pWVar18;
    LPWSTR lpString;
    WCHAR *local_res10;                  /* 游标 */
    int local_res20[2];                  /* X 解析槽 (默认 INT_MIN) */
    /* 串槽组 */
    WCHAR *local_98;                     /* 字段1: hd / 变量名 / 路径表 */
    WCHAR *local_90;                     /* 字段3: X */
    WCHAR *local_88;                     /* 字段5: Y (第4槽 local_70 的别名读) */
    WCHAR *local_80;                     /* 字段2: pt */
    WCHAR *local_78;
    WCHAR *local_70;                     /* 字段4: Y */
    WCHAR *local_60;
    WCHAR *local_50;
    WCHAR *local_48;
    WCHAR *syn;                          /* decompiled local_res18 复用槽: 合成命令缓冲 */
    int pt_num;                          /* decompiled local_res18 低 32 位复用: pt 解析值 */
    short drv_idx;                       /* decompiled local_res18._0_2_ 复用: 蜂巢探测盘序号 */
    int local_58;                        /* Y 解析值 */
    int local_54;                        /* 蜂巢命中标志 */
    int local_68len;                     /* 探测路径长度 */
    WCHAR letterbuf[3];                  /* decompiled local_68/local_64: "X:" 串 */
    int letter_found;                    /* iVar17 盘符查找结果 */
    WCHAR *local_40;

    local_res10 = a2;
    PECMD_AllocStrSlot(&local_98);
    PECMD_AllocStrSlot(&local_80);
    PECMD_AllocStrSlot(&local_90);
    PECMD_AllocStrSlot(&local_70);
    PECMD_AllocStrSlot(&local_88);
    pt_num = (int)0x80000000;            /* _0_2_=0,_2_2_=0x8000 */
    local_res20[0] = (int)0x80000000;
    local_58 = (int)0x80000000;
    syn = NULL;
    drv_idx = 0;
    FUN_14005B154(&local_res10);
    uVar11 = 0;
    iVar8 = 0;
    pWVar14 = local_res10 + -1;
    WVar1 = *local_res10;
    local_40 = pWVar14;
    while (WVar1 == L'-') {
        cVar4 = PECMD_MatchTokenAdvance("-force", &local_res10, 6);
        if (cVar4 == '\0') {
            pWVar18 = local_res10;
            uVar7 = (uint)PECMD_AsciiPrefixICmp("-from:", (const uint16_t *)local_res10, 6);
            if ((char)uVar7 == '\0') {
                uVar7 = (uint)PECMD_AsciiPrefixICmp("-skiptp:", (const uint16_t *)pWVar18, 8);
                if (((char)uVar7 == '\0') &&
                    (uVar7 = (uint)PECMD_AsciiPrefixICmp("-skippt:", (const uint16_t *)pWVar18, 8),
                     (char)uVar7 == '\0')) {
                    cVar4 = PECMD_MatchTokenAdvance("-check", &local_res10, 6);
                    if ((cVar4 != '\0') ||
                        (cVar4 = PECMD_MatchTokenAdvance("-cdrom", &local_res10, 6), cVar4 != '\0')) {
                        goto LAB_1400d71cb;
                    }
                    break;
                }
                local_res10 = pWVar18 + 8;
            } else {
                local_res10 = pWVar18 + 6;
            }
            for (; (WVar1 = *local_res10, WVar1 != L'\0' &&
                   ((((uint16_t)WVar1 < 9 || (0xd < (uint16_t)WVar1)) && (WVar1 != L' '))));
                 local_res10 = local_res10 + 1) {
            }
            FUN_14005B154(&local_res10);
        }
LAB_1400d71cb:
        WVar1 = *local_res10;
    }
    local_res10[-1] = L'\0';
    PECMD_SplitNextToken((int64_t *)a1, (int64_t *)&local_res10, (int64_t *)&local_98, 0x2c, 0);
    PECMD_SplitNextToken((int64_t *)a1, (int64_t *)&local_res10, (int64_t *)&local_80, 0x2c, 0);
    PECMD_SplitNextToken((int64_t *)a1, (int64_t *)&local_res10, (int64_t *)&local_90, 0x2c, 0);
    PECMD_SplitNextToken((int64_t *)a1, (int64_t *)&local_res10, (int64_t *)&local_70, 0x2c, 0);
    PECMD_SplitNextToken((int64_t *)a1, (int64_t *)&local_res10, (int64_t *)&local_88, 0x2c, 0);
    local_78 = local_70;
    PECMD_ParseHexOrDecBool((WCHAR **)&local_78, &local_58);
    if (((*local_98 == L'\0') && (*local_80 == L'\0')) && (*local_90 == L'\0')) {
LAB_1400d754b:
        iVar8 = local_58;
        if ((local_58 == 1) || (local_58 == 4)) {
            pCVar13 = "#4";
            if (local_58 == 1) {
                pCVar13 = "#1";
            }
            /* 合成 "#N"+首字段+" * F:0,,0,Y,rest" 委托 SHOW */
            PECMD_StrBldCopyAnsi((int64_t *)(intptr_t)&syn, pCVar13, ~(uint64_t)0);
            PECMD_StrCat(&syn, pWVar14);
            PECMD_StrCat(&syn, (const uint16_t *)L" * F:0");
            PECMD_StrCat(&syn, (const uint16_t *)L",,0,");
            PECMD_StrCat(&syn, local_88);
            PECMD_StrCat(&syn, (const uint16_t *)L",");
            PECMD_StrCat(&syn, local_res10);
            pWVar14 = syn;
        } else {
            if ((local_58 != 2) && (local_58 != 0x16)) {
                if (((local_58 != 3) ||
                     (*local_88 = (*local_88 & (WCHAR)0xffdf), (uint16_t)*local_88 < 0x41)) ||
                    (0x5a < (uint16_t)*local_88)) {
                    uVar11 = 0xffffffff80070057ULL;
                    goto LAB_1400d77ed;
                }
                /* 合成 "#3"+首字段+" * U:0,,0,Y,rest" */
                PECMD_StrBldCopyAnsi((int64_t *)(intptr_t)&syn, "#3", ~(uint64_t)0);
                PECMD_StrCat(&syn, pWVar14);
                PECMD_StrCat(&syn, (const uint16_t *)L" * U:0");
                PECMD_StrCat(&syn, (const uint16_t *)L",,0,");
                PECMD_StrCat(&syn, local_88);
                PECMD_StrCat(&syn, (const uint16_t *)L",");
                PECMD_StrCat(&syn, local_res10);
                pWVar14 = syn;
            } else {
                /* 合成 "#2"[+改写#22]+首字段+" * F:0,,0,Y,rest" */
                PECMD_StrBldCopyAnsi((int64_t *)(intptr_t)&syn, "#2", ~(uint64_t)0);
                PECMD_StrCat(&syn, pWVar14);
                PECMD_StrCat(&syn, (const uint16_t *)L" * F:0");
                PECMD_StrCat(&syn, (const uint16_t *)L",,0,");
                PECMD_StrCat(&syn, local_88);
                PECMD_StrCat(&syn, (const uint16_t *)L",");
                PECMD_StrCat(&syn, local_res10);
                pWVar14 = syn;
                if (iVar8 == 0x16) {
                    pWVar14[2] = L'2';
                }
            }
        }
        uVar11 = (uint64_t)(longlong)PECMD_PartShowHideDrive(
            (uint64_t)(intptr_t)a1, pWVar14);
    } else {
        local_78 = local_80;
        PECMD_ParseHexOrDecBool((WCHAR **)&local_78, &pt_num);
        local_78 = local_90;
        PECMD_ParseHexOrDecBool((WCHAR **)&local_78, local_res20);
        iVar2 = local_res20[0];
        sVar6 = (short)((uint32_t)pt_num >> 16);
        WVar1 = *local_98;
        if (((WVar1 == L'\0') || (*local_80 != L'\0')) || (*local_90 != L'\0')) {
            if (WVar1 != L'\0') {
                if (*local_80 == L'\0') {
                    if (*local_90 != L'\0') {
                        /* hd,X 有值: 统计可用逻辑盘数 (X>0 计全部, 否则仅整盘项) */
                        PECMD_AllocStringSlot2((void **)&syn, 0x3960);
                        piVar16 = (int *)(uintptr_t)syn;
                        PECMD_EnumDrivesToTable((int64_t)(intptr_t)piVar16, (LPWSTR)0, 2);
                        lVar15 = 0x1a;
                        do {
                            if ((0 < *piVar16) &&
                                ((0 < local_res20[0] || (piVar16[1] == 0)))) {
                                uVar11 = (ulonglong)((int)uVar11 + 1);
                            }
                            piVar16 = piVar16 + 0x88;
                            lVar15 = lVar15 + -1;
                        } while (lVar15 != 0);
                        PECMD_AppendLongDecimal(a1, (int64_t)uVar11, local_98);
                        PECMD_FreeStrBuf(&syn);
                        PECMD_FreeStrBuf(&local_88);
                        PECMD_FreeStrBuf(&local_70);
                        PECMD_FreeStrBuf(&local_90);
                        PECMD_FreeStrBuf(&local_80);
                        PECMD_FreeStrBuf(&local_98);
                        return 0;
                    }
                    if (*local_80 == L'\0') {
                        goto LAB_1400d7c8f;
                    }
                }
                if (*local_90 == L'\0') {
                    /* hd,pt 有值: 统计分区号匹配的逻辑盘数写回变量 */
                    iVar8 = pt_num;
                    if (-1 < sVar6) {
                        PECMD_AllocStringSlot2((void **)&syn, 0x3960);
                        piVar16 = (int *)(uintptr_t)syn;
                        PECMD_EnumDrivesToTable((int64_t)(intptr_t)piVar16, (LPWSTR)0, 2);
                        lVar15 = 0x1a;
                        do {
                            if ((0 < *piVar16) && (piVar16[1] == iVar8)) {
                                uVar11 = (ulonglong)((int)uVar11 + 1);
                            }
                            piVar16 = piVar16 + 0x88;
                            lVar15 = lVar15 + -1;
                        } while (lVar15 != 0);
                        PECMD_FreeStrBuf(&syn);
                    }
                    PECMD_AppendLongDecimal(a1, (int64_t)uVar11, local_98);
                    PECMD_FreeStrBuf(&local_88);
                    PECMD_FreeStrBuf(&local_70);
                    PECMD_FreeStrBuf(&local_90);
                    PECMD_FreeStrBuf(&local_80);
                    PECMD_FreeStrBuf(&local_98);
                    if (-1 < sVar6) {
                        return 0;
                    }
                    return 0xffffffff80070057ULL;
                }
            }
            if ((*local_80 == L'\0') || (*local_90 == L'\0')) {
LAB_1400d7c8f:
                PECMD_FreeStrBuf(&local_88);
                PECMD_FreeStrBuf(&local_70);
                PECMD_FreeStrBuf(&local_90);
                PECMD_FreeStrBuf(&local_80);
                PECMD_FreeStrBuf(&local_98);
                return 0xffffffff80070057ULL;
            }
            if (*local_98 == L'\0') {
                /* pt,X 有值无变量名: 合成 " *- pt:X" 委托 SHOW */
                if ((-1 < sVar6) && (-1 < local_res20[0])) {
                    PECMD_StrBldCopyAnsi((int64_t *)(intptr_t)&syn, " *- ", ~(uint64_t)0);
                    PECMD_StrCat(&syn, local_80);
                    PECMD_StrCat(&syn, (const uint16_t *)L":");
LAB_1400d7b1e:
                    PECMD_StrCat(&syn, local_90);
                    uVar11 = (uint64_t)(longlong)PECMD_PartShowHideDrive(
                        (uint64_t)(intptr_t)a1, syn);
                    PECMD_FreeStrBuf(&syn);
                    uVar11_ret:
                    PECMD_FreeStrBuf(&local_88);
                    PECMD_FreeStrBuf(&local_70);
                    PECMD_FreeStrBuf(&local_90);
                    PECMD_FreeStrBuf(&local_80);
                    PECMD_FreeStrBuf(&local_98);
                    return uVar11;
                }
            } else {
                if (local_98[1] != L':') {
                    /* hd 为变量名: 按 (disk=X?, part=pt?) 查驱动器表得盘符写回 */
                    iVar8 = pt_num;
                    letterbuf[0] = L'\0';
                    letterbuf[1] = L':';
                    letterbuf[2] = L'\0';
                    letter_found = (int)0x80070057;
                    if ((-1 < sVar6) && (0 < local_res20[0])) {
                        PECMD_AllocStringSlot2((void **)&syn, 0x3960);
                        lVar15 = (longlong)(intptr_t)syn;
                        PECMD_EnumDrivesToTable(lVar15, (LPWSTR)0, 2);
                        sVar6 = 0;
                        do {
                            if ((*(int *)((longlong)sVar6 * 0x220 + 4 + lVar15) == iVar8) &&
                                (*(int *)((longlong)sVar6 * 0x220 + lVar15) == iVar2)) {
                                letter_found = 0;
                                letterbuf[0] = *(WCHAR *)((longlong)sVar6 * 0x220 + 0x10 + lVar15);
                                break;
                            }
                            sVar6 = sVar6 + 1;
                            letter_found = 1;
                        } while (sVar6 < 0x1a);
                        PECMD_FreeStrBuf(&syn);
                    }
                    PECMD_SetVar((void *)a1, local_98, (LPCWSTR)letterbuf);
                    PECMD_FreeStrBuf(&local_88);
                    PECMD_FreeStrBuf(&local_70);
                    PECMD_FreeStrBuf(&local_90);
                    PECMD_FreeStrBuf(&local_80);
                    PECMD_FreeStrBuf(&local_98);
                    return (ulonglong)(longlong)letter_found;
                }
                if ((-1 < sVar6) && (-1 < local_res20[0])) {
                    /* hd 为盘符名: 合成 "* * "/" * " + pt:X,Y 委托 SHOW */
                    pCVar13 = "* * ";
                    if ((local_58 != 0) && (pCVar13 = " * ", *local_70 == L'\0')) {
                        pCVar13 = "* * ";
                    }
                    PECMD_StrBldCopyAnsi((int64_t *)(intptr_t)&syn, pCVar13, ~(uint64_t)0);
                    PECMD_StrCat(&syn, local_80);
                    PECMD_StrCat(&syn, (const uint16_t *)L":");
                    PECMD_StrCat(&syn, local_90);
                    PECMD_StrCat(&syn, (const uint16_t *)L",");
                    local_90 = local_98;
                    goto LAB_1400d7b1e;
                }
            }
            uVar11 = 0xffffffff80070057ULL;
            goto uVar11_ret;
        }
        if (WVar1 == L'\\') {
            /* '\\路径' 分支: 逐盘探测 <X>:<path>\System32\config\SYSTEM 并加载蜂巢 */
            local_54 = 1;
            PECMD_AllocStringSlot2((void **)&local_50, 0x3960);
            PECMD_EnumDrivesToTable((int64_t)(intptr_t)local_50, (LPWSTR)0, 2);
            local_78 = local_98;
            local_res20[0] = 0;
            if (*local_98 != L'\0') {
                do {
                    lpString = local_78;
                    b7c_skip_to(&local_78, L'|');
                    if (*local_78 != L'\0') {
                        *local_78 = L'\0';
                        local_78 = local_78 + 1;
                    }
                    if (*lpString == L'\\') {
                        lpString = lpString + 1;
                    }
                    iVar8 = local_res20[0];
                    if (*lpString != L'\0') {
                        iVar8 = lstrlenW(lpString);
                        if (lpString[(longlong)iVar8 + -1] == L'\\') {
                            lpString[(longlong)iVar8 + -1] = L'\0';
                        }
                        iVar8 = local_res20[0];
                        if (*lpString != L'\0') {
                            drv_idx = 0;
                            do {
                                lVar15 = drv_idx;
                                if (0 < *(int *)(lVar15 * 0x220 + (longlong)(intptr_t)local_50)) {
                                    PECMD_StrBldCopyAnsi((int64_t *)(intptr_t)&local_60,
                                                         "C:\\", ~(uint64_t)0);
                                    *local_60 =
                                        *(WCHAR *)(lVar15 * 0x220 + 0x10 +
                                                   (longlong)(intptr_t)local_50);
                                    PECMD_StrCat(&local_60, lpString);
                                    plVar10 = PECMD_StrBldCopyAnsi(
                                        (int64_t *)(intptr_t)&local_48,
                                        "\\System32\\config\\SYSTEM", ~(uint64_t)0);
                                    PECMD_StrCat(&local_60, (LPCWSTR)*plVar10);
                                    PECMD_FreeStrBuf(&local_48);
                                    pWVar3 = local_60;
                                    local_68len = lstrlenW(local_60);
                                    bVar5 = FUN_140101E70(pWVar3);
                                    iVar8 = local_res20[0];
                                    if (bVar5) {
                                        pWVar3[(longlong)local_68len + -6] = L'\0';
                                        uVar11 = PECMD_BuildQuickCommand(
                                            "SYSTEM", pWVar3, a1, 0);
                                        iVar8 = (int)uVar11;
                                        if ((int)uVar11 == 0) {
                                            local_54 = 0;
                                            uVar11 = PECMD_BuildQuickCommand(
                                                ".DEFAULT", pWVar3, a1, 1);
                                            iVar8 = local_res20[0];
                                            if ((int)uVar11 != 0) {
                                                iVar8 = (int)uVar11;
                                            }
                                            uVar11 = PECMD_BuildQuickCommand(
                                                "SOFTWARE", pWVar3, a1, 0);
                                            if ((int)uVar11 != 0) {
                                                iVar8 = (int)uVar11;
                                            }
                                            uVar11 = PECMD_BuildQuickCommand(
                                                "SAM", pWVar3, a1, 0);
                                            if ((int)uVar11 != 0) {
                                                iVar8 = (int)uVar11;
                                            }
                                            uVar11 = PECMD_BuildQuickCommand(
                                                "SECURITY", pWVar3, a1, 0);
                                            if ((int)uVar11 != 0) {
                                                iVar8 = (int)uVar11;
                                            }
                                            local_res20[0] = iVar8;
                                            PECMD_BuildQuickCommand(
                                                "USERDIFF", pWVar3, a1, 0);
                                            PECMD_FreeStrBuf(&local_60);
                                            break;
                                        }
                                    }
                                    local_res20[0] = iVar8;
                                    PECMD_FreeStrBuf(&local_60);
                                }
                                drv_idx = (short)(drv_idx + 1);
                                iVar8 = local_res20[0];
                            } while (drv_idx < 0x1a);
                        }
                    }
                } while (*local_78 != L'\0');
                if (local_54 == 0) {
                    goto LAB_1400d7827;
                }
            }
            if (*local_70 == L'\0') {
LAB_1400d7827:
                PECMD_FreeStrBuf(&local_50);
                PECMD_FreeStrBuf(&local_88);
                PECMD_FreeStrBuf(&local_70);
                PECMD_FreeStrBuf(&local_90);
                PECMD_FreeStrBuf(&local_80);
                PECMD_FreeStrBuf(&local_98);
                return (ulonglong)(longlong)iVar8;
            }
            PECMD_FreeStrBuf(&local_50);
            pWVar14 = local_40;
            goto LAB_1400d754b;
        }
        /* 仅 hd: 枚举物理盘数写回变量 */
        PECMD_AllocStrSlot(&syn);
        uVar7 = PECMD_EnumeratePhysicalDrives((longlong *)(intptr_t)&syn, 0xffffffffU);
        PECMD_AppendLongDecimal(a1, (int64_t)uVar7, local_98);
        PECMD_FreeStrBuf(&syn);
    }
LAB_1400d77ed:
    PECMD_FreeStrBuf(&local_88);
    PECMD_FreeStrBuf(&local_70);
    PECMD_FreeStrBuf(&local_90);
    PECMD_FreeStrBuf(&local_80);
    PECMD_FreeStrBuf(&local_98);
    return uVar11;
}

/* ========== FUN_1400D7CE4 @0x1400d7ce4 ==========
 * [简化桩] 控件消息扩展。返回 0。
 * TODO(verify): 需完整还原消息处理。
 */
uint64_t FUN_1400D7CE4(uint64_t *a1, int64_t *a2, WCHAR *a3,
                            LPCWSTR a4, WPARAM a5, uint64_t *a6)
{
    (void)a1;
    (void)a2;
    (void)a3;
    (void)a4;
    (void)a5;
    (void)a6;
    return 0;
}

/* ========== FUN_1400D85D0 @0x1400d85d0 ==========
 * [简化桩] 控件消息主处理。返回 0。
 * TODO(verify): 需完整还原消息主循环。
 */
uint64_t FUN_1400D85D0(int64_t *a1)
{
    (void)a1;
    return 0;
}

/* ========== FUN_1400D9818 @0x1400d9818 ==========
 * [简化桩] 控件窗口过程。返回 NULL。
 * TODO(verify): 需完整还原窗口过程。
 */
HWND FUN_1400D9818(HDC a1, uint32_t a2, HDC a3, POINT a4)
{
    (void)a1;
    (void)a2;
    (void)a3;
    (void)a4;
    return (HWND)0;
}

/* ========== FUN_1400DB648 @0x1400db648 ==========
 * [简化桩] 绘制控件文本。返回 0。
 * TODO(verify): 需完整还原绘制。
 */
uint64_t FUN_1400DB648(HWND a1, uint16_t *a2, uint64_t a3,
                               int64_t *a4, HWND a5, COLORREF a6,
                               int64_t a7)
{
    (void)a1;
    (void)a2;
    (void)a3;
    (void)a4;
    (void)a5;
    (void)a6;
    (void)a7;
    return 0;
}

/* ========== PECMD_EvalAtCommand @0x1400dc9fc ==========
 * [简化桩] 执行命令 C。返回 {0}。
 * TODO(verify): 需完整还原执行逻辑。
 */
LARGE_INTEGER PECMD_EvalAtCommand(LARGE_INTEGER a1, LARGE_INTEGER a2,
                             uint16_t *a3, int64_t a4)
{
    LARGE_INTEGER r;
    (void)a1;
    (void)a2;
    (void)a3;
    (void)a4;
    r.QuadPart = 0;
    return r;
}

/* ========== PECMD_MounMountWimImage @0x1400dfb14 ==========
 * [简化桩] 文件时间命令。返回 {0}。
 * TODO(verify): 需完整还原时间命令。
 */
FILETIME PECMD_MounMountWimImage(int64_t *a1, FILETIME a2, WPARAM a3)
{
    FILETIME r;
    (void)a1;
    (void)a2;
    (void)a3;
    r.dwLowDateTime = 0;
    r.dwHighDateTime = 0;
    return r;
}
