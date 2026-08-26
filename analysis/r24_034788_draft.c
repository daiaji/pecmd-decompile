/* ====================================================================
 * r24_034788_draft.c — C1 工单落码草稿
 *
 *   FUN_140034788 @ 0x140034788 (原版 PECMD.exe, base 0x140000000)
 *   body 0x140034788–0x14003585F, 恰 0x10D8 = 4312 字节
 *   身份: IFEX / FIND 共享执行体
 *     包装器  FUN_140035b08(IFEX, verb_mode=1)  dc:45105–45115
 *             FUN_140035b24(FIND, verb_mode=0)  dc:44403–44414
 *             (重构库 core_b2a.c: PECMD_IfexCmdHandler / PECMD_FindCmdHandler)
 *
 * 权威资料:
 *   - reference/decompiled.c:31379 为 [DECOMPILE FAILED] 存根 (无语义)
 *   - analysis/s15_fun140034788_restore.md (34KB 汇编级还原稿)
 *   - 本稿以 Ghidra MCP 对原版 PECMD.exe 的 420 条指令完整反汇编为准逐块核验;
 *     s15 个别转述与实际指令有出入处, 以反汇编为准并在此记录:
 *       · A/B 阶段扫描范围: s15 称"扫全文", 反汇编实为【前导特殊字符段】+
 *         【首个空白断开的单词】(0x140034823/0x14003491D), 本稿按反汇编实现。
 *       · Q2 已由反汇编闭合: 无 #id 时 GetMenu(hwnd) 结果流入 SetVarFmt64。
 *       · Q5 常量: 失败路径回归 0x80070057 (R12D 于环内多次重写),
 *         s15 猜测的 0x23 残留未见稳定路径, 采用 0x80070057。
 *
 * 还原签名 (与 core_b2a.c 包装器调用形态对齐; s15 §1.2/§5):
 *   int64_t PECMD_IfexFindExecutor(int64_t *script, ULARGE_INTEGER cmdline,
 *                                  uint8_t verb_mode, int64_t *out_var,
 *                                  uint64_t locale)
 *   verb_mode: 1 = IFEX, 0 = FIND ; locale: 第 5 参 (pthreadmbcinfo 透传)
 *
 * 依赖符号 (extern) 均需主代理在合入时与项目头文件逐项核对/收敛:
 *   已实现: PECMD_EvalLoopCondition(core_b2f.c:2660) / PECMD_SkipLeadingControls /
 *     PECMD_AllocStrSlot / PECMD_AllocString / PECMD_AppendWideStr /
 *     PECMD_FreeStrBuf / PECMD_ProcessScriptBlock(restored_bodies.c 直移) /
 *     PECMD_TokenizeExpression(core_b3_remaining.c:24759) /
 *     PECMD_ParseCommandBlock(core_b2d.c:451) / PECMD_AsciiPrefixICmp /
 *     PECMD_MatchTokenAdvance / PECMD_NextToken / PECMD_RemoveDuplicateChar /
 *     PECMD_CollapseRepeatedChars / PECMD_AdvanceTokenPointer /
 *     PECMD_ParseUIntValue / PECMD_ParseHexOrDecBool / PECMD_ParseShortStore /
 *     PECMD_AppendFmtValue / FUN_1400629B8(SetVar)
 *   ⚠ 待主代理核对: PECMD_ExpandVarDispatch(签名 p2 实为 WCHAR*? 见 0x14003508F
 *     传入整串文本)、FUN_14002d708、PECMD_FindTargetWindow、WinAPI 组。
 *
 * 本文件为分析产物 (草稿), 不参与构建; 合并路径见 analysis/r24_034788_draft_notes.md。
 * ==================================================================== */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* ---- 最小类型 (合入时改用 pecmd_defs.h / Windows 头) ---- */
typedef unsigned short WCHAR;
typedef const WCHAR *LPCWSTR;
typedef WCHAR *LPWSTR;
typedef unsigned long DWORD;
typedef int BOOL;
typedef unsigned int UINT;
typedef void *HWND;
typedef void *HMENU;

/* MSVC 惯用 LARGE_INTEGER/ULARGE_INTEGER (项目内 pecmd_defs.h 等价) */
typedef union _CL_LARGE_INTEGER {
    struct {
        DWORD LowPart;
        long HighPart;
    } s;
    int64_t QuadPart;
} CL_LARGE_INTEGER;
typedef union _CL_ULARGE_INTEGER {
    struct {
        DWORD LowPart;
        DWORD HighPart;
    } s;
    uint64_t QuadPart;
} CL_ULARGE_INTEGER;

/* 允许宿主头把宏名让位给 Windows 真实类型 (MSVC 编译时见 windows.h) */
#ifndef LARGE_INTEGER
#define LARGE_INTEGER CL_LARGE_INTEGER
#endif
#ifndef ULARGE_INTEGER
#define ULARGE_INTEGER CL_ULARGE_INTEGER
#endif

/* ====================================================================
 * 依赖 extern 区 —— 语义以 s15 §3.7/§4.b + 反汇编调用点为准
 * ==================================================================== */

/* @0x1400170b0 = PECMD_SkipLeadingControls: 跳过 0x09–0x0D/0x20, 推进槽内指针 */
extern uint64_t *PECMD_SkipLeadingControls(uint64_t *pp);

/* @0x140032dc4 = PECMD_EvalLoopCondition(ctx, path, mode, extra):
 *   条件求值器。本函数内 mode = verb_mode|bit16(*c)|bit17(*ab)|bit18(*var);
 *   返回 >0 即有符号真 (SETG, 0x14003576D)。 */
extern ULARGE_INTEGER PECMD_EvalLoopCondition(int64_t *ctx, LPCWSTR path, int mode,
                                              LPCWSTR extra);

/* @0x14005b104 = PECMD_FreeStrBuf(WCHAR **ps): HeapFree(ps[0]-8) 并清槽 */
extern void PECMD_FreeStrBuf(WCHAR **ps);

/* @0x140063620 = PECMD_AllocStrSlot(WCHAR **out): 新建空 StrBuf, 槽置文本指针 */
extern WCHAR *PECMD_AllocStrSlot(WCHAR **out);

/* @0x140063720 = PECMD_AllocString(WCHAR **ps, int64_t len) */
extern WCHAR *PECMD_AllocString(WCHAR **ps, int64_t len);

/* @0x14006375c = PECMD_AppendWideStr(void *acc_slot, LPCWSTR src) */
extern int64_t *PECMD_AppendWideStr(void *acc_slot, LPCWSTR src);

/* @0x14007bf44 = PECMD_ExpandVarDispatch(script, text, out_slot, 0, 1):
 *   执行/展开命令文本并捕获输出到 out_slot (s15: 执行命令并捕获输出)。
 *   ⚠ 项目头声明 p2 为 WCHAR*; 0x14003508F 传入光标文本 — 主代理核对。 */
extern void PECMD_ExpandVarDispatch(void *p1, WCHAR *p2, void *p3, int p4, uint8_t p5);

/* @0x14004c0bc = PECMD_ProcessScriptBlock(p1,p2,p3,p4,p5): 递归大解释器 */
extern LARGE_INTEGER PECMD_ProcessScriptBlock(LARGE_INTEGER p1, LARGE_INTEGER p2,
                                              int64_t *p3, int64_t *p4, void *p5);

/* @0x1400a53e4 = PECMD_TokenizeExpression(a,b,c,d,e): 脚本定界符动作执行器 */
extern int64_t PECMD_TokenizeExpression(LARGE_INTEGER a, int64_t b, int64_t *c,
                                        uint32_t d, WCHAR *e);

/* @0x140025474 = PECMD_ParseCommandBlock(script, WCHAR **pp, flags, threadInfo) */
extern uint64_t PECMD_ParseCommandBlock(int64_t script, WCHAR **pp, uint32_t flags,
                                        void *threadInfo);

/* @0x14005c788 = PECMD_AsciiPrefixICmp: 小写不敏感前缀比较, 命中返非 0 (dc:54955) */
extern int PECMD_AsciiPrefixICmp(const char *a, const WCHAR *w, int n);

/* @0x1400660ac = PECMD_MatchTokenAdvance(tok, &cursor, n): 命中则推进 cursor+trim */
extern char PECMD_MatchTokenAdvance(const char *tok, uint64_t *pp, int n);

/* @0x140024c48 = PECMD_NextToken(&cursor_slot, &len_slot, mode): 引号串解析 */
extern WCHAR *PECMD_NextToken(int64_t *a, int64_t *b, uint32_t mode);

/* @0x140025274 = PECMD_RemoveDuplicateChar(s, allowChar):
 *   返回首个未双写转义 allowChar 的位置指针 */
extern WCHAR *PECMD_RemoveDuplicateChar(LPCWSTR s, WCHAR allowChar);

/* @0x140025404 = PECMD_CollapseRepeatedChars(s, ch): 折叠连续双写 ch */
extern int64_t PECMD_CollapseRepeatedChars(LPCWSTR s, WCHAR ch);

/* @0x140062290 = PECMD_AdvanceTokenPointer(ctx, out_var, mode, p): 名提取/绑定 */
extern void PECMD_AdvanceTokenPointer(int64_t ctx, int64_t *a4, int mode, int64_t *p);

/* @0x140074838 = PECMD_ParseUIntValue(WCHAR **pp, int *out): 十进制解析, 返回状态 */
extern int PECMD_ParseUIntValue(WCHAR **pp, int *out);

/* @0x1400c11c0 = PECMD_ParseHexOrDecBool(int64_t **pp, int *out) */
extern int PECMD_ParseHexOrDecBool(int64_t **pp, int *out);

/* @0x1400679b0 = PECMD_ParseShortStore(WCHAR **pp, int *out, WCHAR sep):
 *   ⚠ 项目声明 void; 反汇编 0x140034D7F 判其返回值 — 主代理核对真实签名 */
extern void PECMD_ParseShortStore(WCHAR **pp, int *out, WCHAR sep);

/* @0x1400668ec = PECMD_AppendFmtValue(script, value, key, fmt) */
extern void PECMD_AppendFmtValue(void *script, uint64_t value, LPCWSTR key, LPCWSTR fmt);

/* @0x1400629b8 = SetVar(script, key, value) */
extern void FUN_1400629b8(void *script, const WCHAR *key, const WCHAR *value);

/* @0x14002d708 = 跨文件/进程/窗口通用匹配器 (--pid 路径) */
extern uint64_t FUN_14002d708(int64_t *script, uint32_t flags, int64_t *outbuf,
                              int pid_hash, uint32_t pid_plain);

/* @0x1400e3d60 = PECMD_FindTargetWindow (--wid 无# 枚举搜索路径) */
extern uint64_t PECMD_FindTargetWindow(LPCWSTR p1, char p2, char p3, HWND p4,
                                       uint32_t p5, uint32_t p6, uint64_t p7,
                                       uint64_t p8);

/* ---- WinAPI (二级跳转表槽位破译见 s15 §3.7) ---- */
extern LPCWSTR StrChrW(LPCWSTR str, WCHAR ch);
extern HMENU GetMenu(HWND hWnd);
extern int GetMenuItemCount(HMENU hMenu);
extern HMENU GetSubMenu(HMENU hMenu, int nPos);
extern UINT GetMenuItemID(HMENU hMenu, int nPos);
extern int GetMenuStringW(HMENU hMenu, UINT uIDItem, LPWSTR lpString, int nMaxCount,
                          UINT uFlag);
extern int wsprintfW(LPWSTR out, LPCWSTR fmt, ...);
extern int lstrlenW(LPCWSTR str);
extern HWND GetDlgItem(HWND hDlg, int nIDDlgItem);

/* ====================================================================
 * C1 返回值模型 —— U-1 活体取证闭环 (analysis/r24_u1_exit2_writer.md, 工单0)
 *
 * 已否决"IFEX/FIND 恒返 2"。真实语义 = 条件分派 + 分支行回执行:
 *   · 条件真 → 直接执行真分支行 (裸文本, 无前缀) → 返回值 = 分支命令自身返回值
 *     (ENVI/EXEC→0, WRITE/FILE→2) → 031/061 golden=0 的真相 (真分支 ENVI→0)。
 *   · 条件假 → 以 act2 ("ELSE <分支>" 行文本) 嵌套重入 PECMD_ProcessScriptBlock
 *     → 该行执行返 2 (原版 PSB 尾 LAB_14004c525 写槽) → 处理器回传 2 →
 *     外层 PSB 再尾写一次 → 013/014/016/017/035/036 golden=2 的来源。
 *   · 本草稿【不做任何返回值强制】: result = 嵌套 PSB 的返回原样回传
 *     (真分支 ENVI→0 / 假分支 "ELSE 行"→2 均由 PSB 侧产生, 与 U-1 §5 等价实现一致)。
 *   · 验收锚: 去桩后 013/014/016/017/035/036 须 exit=2, 031/061 须保持 0。
 *   · PSB 侧对 "ELSE <cmd>" 行返回 2 的直移保真 (LAB_14004c525 尾写已直移,
 *     restored_bodies.c:7177) 属 PSB 核对项, 见 notes §4 T9。
 *
 * FIND $X=Y 为等值比较 (非子串, U-1 §4.1): 016 "hello world"!=="world"→假→ELSE→2;
 * 031 "pecmd"=="pecmd"→真→ENVI→0。比较语义在 PECMD_EvalLoopCondition(032dc4) 内部
 * (StrCmpNIW/StrCmpNW 等值), 本函数不实现比较; 草稿无子串逻辑。
 * ==================================================================== */

/* 局部小工具: 跳过非空白 run 至 空白(0x09–0x0D/0x20) 或 NUL (0x140034BDB 等) */
static void cl_skip_num_run(WCHAR **pp)
{
    WCHAR *p = *pp;
    while (*p != 0) {
        WCHAR c = *p;
        if ((c >= (WCHAR)9 && c <= (WCHAR)0xd) || c == L' ')
            break;
        p += 1;
    }
    *pp = p;
}

static int cl_is_space(WCHAR c)
{
    return (c >= (WCHAR)9 && c <= (WCHAR)0xd) || c == L' ';
}

/* C 无法对联合体做整型直接强转 (MSVC C2440), 统一经成员构造 */
static LARGE_INTEGER cl_mk_li(uintptr_t v)
{
    LARGE_INTEGER li;
    li.QuadPart = (int64_t)v;
    return li;
}

/* ====================================================================
 * 主体 — 六阶段 (s15 §2)
 * ==================================================================== */
int64_t PECMD_IfexFindExecutor(int64_t *script, ULARGE_INTEGER cmdline_ui,
                               uint8_t verb_mode, int64_t *out_var, uint64_t locale)
{
    /* ---- 局部状态 (槽地址 = 反汇编 [RSP+0xXX]) ---- */
    WCHAR *cur;         /* [0x138] 文本光标 */
    WCHAR *act1;        /* [0x50]  首分隔符后动作部 */
    WCHAR *act2;        /* [0x60]  '!'/特殊字符切分第二段 */
    WCHAR *slot_a8;     /* [0xa8]  括号路径暂存 */
    WCHAR *rbp_brace;   /* 括号路径闭合定界指针 (RBP) */
    WCHAR *slot_d0;     /* [0xd0]  动作执行器参数 */
    WCHAR *slot_80;     /* [0x80]  解析/别名槽 */
    WCHAR *slot_a0;     /* [0xa0]  --class 文本槽 */
    WCHAR *buf_a;       /* [0x70]  模式路径命令输出捕获 */
    WCHAR *buf_b;       /* [0x88]  act1 输出捕获 */
    WCHAR *buf_78;      /* [0x78]  累积/返回缓冲 */
    WCHAR *buf_c;       /* [0xc0]  '%var%' 展开缓冲 */

    WCHAR delim;        /* BL 生效分隔符 */
    WCHAR delim_word;   /* [0x98] 分隔符字 (L5 用, lead^delim) */
    WCHAR spec_char;    /* [0x46] 特殊字符, 默认 '!' */
    uint8_t saw_pipe;   /* [0x47] 前导 '|' 标志 */

    int32_t slot_68;    /* [0x68] bit16(*c)|bit18(*var) */
    int32_t ab_bit;     /* bit17(*ab) — 原 [0x78] DWORD 槽 (与 buf_78 同一栈址, 分阶段互斥) */
    int32_t slot_40;    /* [0x40] --user=0x80000 / 菜单 hwnd 解析槽 */
    int32_t slot_44;    /* [0x44] --visible=6 / --invisible=4 */
    int32_t slot_45;    /* [0x45] --sub=1 */
    int32_t menuid;     /* [0x48] --menu id (哨兵 0x80000000) */
    int32_t slot_58;    /* [0x58] '@' 标记字节 (byte 语义) */
    int32_t pid_plain;  /* [0x6c] --pid 普通值 / --forpid: */
    int32_t pid_hash;   /* [0xb0] --pid '#' 值 */
    int32_t fortid;     /* [0xb4] --fortid: */
    int32_t sz_b8;      /* [0xb8] --wid 数值暂存 */
    int64_t wid_plain;  /* [0x90] --wid 普通值 */
    int64_t wid_hash;   /* [0xd8] --wid '#' 值 */

    uint8_t r14b;       /* '*'('.' 修饰累积 */
    uint8_t r15b;       /* '@' 修饰标记 */
    int32_t mode;       /* R13D: 0=表达,1=--pid,2=--menu,16=--wid */
    int32_t err;        /* ESI/R12D 错误累积 (0x80070057) */

    uint8_t negform;    /* 最终 BL: 0=无,1='!!',2='!' */
    uint8_t r13b;       /* slot_a8 闭合定界判定 */
    int truthy;
    LPCWSTR cond_text;
    int64_t result;
    uint8_t one = 1;
    int r;

    /* ======== 入口初始化 (0x14003478B–0x1400347C7) ======== */
    cur = (WCHAR *)(uintptr_t)cmdline_ui.QuadPart;
    result = 0;
    spec_char = L'!';
    saw_pipe = 0;
    delim = L',';
    slot_68 = 0;
    ab_bit = 0;
    slot_40 = 0;
    slot_44 = 0;
    slot_45 = 0;
    menuid = (int32_t)0x80000000;
    slot_58 = 0;
    pid_plain = 0;
    pid_hash = 0;
    fortid = 0;
    sz_b8 = 0;
    wid_plain = 0;
    wid_hash = 0;
    r14b = 0;
    r15b = 0;
    mode = 0;
    err = 0;
    act1 = NULL;
    act2 = NULL;
    slot_a8 = NULL;
    rbp_brace = NULL;
    slot_d0 = NULL;
    slot_80 = NULL;
    slot_a0 = NULL;
    buf_a = NULL;
    buf_b = NULL;
    buf_78 = NULL;
    buf_c = NULL;
    negform = 0;
    r13b = 0;
    truthy = 0;
    cond_text = NULL;
    delim_word = L',';

    /* ============ A. 前导段预扫描 (0x1400347C9–0x140034835) ============
     * 反汇编核验: 只处理【前导特殊字符段】, 首普通字符即移交 B (修正 s15 §3.1
     * "扫全文" 描述)。C1 案首字符为 ' ' 等普通字符 → 本段等效直出。 */
    {
        WCHAR *p = (WCHAR *)(uintptr_t)cmdline_ui.QuadPart;
        while (*p != 0) {
            WCHAR ch = *p;
            if (ch == L'|') {
                saw_pipe = (uint8_t)ch;
            } else if (ch == L'^' || ch == L'#' || ch == L'~' || ch == L'+' ||
                       ch == L'-') {
                spec_char = ch;
            } else if (ch == L';' || ch == L':') {
                delim = ch;
            } else {
                break;
            }
            p += 1;
        }
        cur = p;
    }

    /* ============ B. *c/*ab/*var 标记 (0x140034837–0x140034942) ============
     * 反汇编核验: 外层环仅扫描【首个空白断开的 run】; 每个 '*' 字冠测三连标记;
     * 遇 0x09–0x0D/0x20/NUL 即止 (修正 s15 "扫全文" 描述)。光标终值停在断点。 */
    {
        WCHAR *run = cur;
        while (*run != 0 && !cl_is_space(*run)) {
            if (*run == L'*') {
                if (PECMD_AsciiPrefixICmp("*c", run, 2) != 0) {
                    slot_68 |= 0x10000; /* BTS ESI,0x10 */
                } else if (PECMD_AsciiPrefixICmp("*ab", run, 3) != 0) {
                    ab_bit = 0x20000;   /* BTS EBP,0x11 → [0x78] */
                } else if (PECMD_AsciiPrefixICmp("*var", run, 4) != 0) {
                    slot_68 |= 0x40000; /* BTS ESI,0x12 */
                }
            }
            run += 1;
        }
        cur = run; /* 0x14003491D 断点 (空白/NUL) */
    }
    /* 位标志经 ab_bit 传递; 原栈槽 [0x78] 与 buf_78 (E' 累积缓冲) 同一地址, 分阶段互斥使用 */

    /* ============ C. 空白修剪 + 空判 (0x140034942–0x14003495C) ============ */
    PECMD_SkipLeadingControls((uint64_t *)&cur);
    if (*cur == 0)
        return 0; /* 0x14003495C → epilogue */

    /* ============ D. 首分隔符切分 (0x140034963–0x1400349E1) ============ */
    {
        WCHAR *pos = (WCHAR *)StrChrW(cur, delim);
        act1 = pos;
        if (pos != NULL) {
            *pos = 0;               /* 就地切断 (0x1400349D0) */
            act1 = pos + 1;         /* 0x1400349D8: 余部 [0x50] */
        }
    }
    delim_word = delim;             /* [0x98] */

    /* ============ E. "--" 选项环 (0x1400349E1–0x14003506A) ============ */
    if (*cur == L'-') {
        if (cur[2] == L'-') {
            /* 0x140034A0E: 前导 "--" → %var% 展开整串, 光标改指新缓冲 */
            PECMD_ExpandVarDispatch(script, cur, &buf_c, 0, one);
            if (buf_c != NULL)
                cur = buf_c;
        }
        if (*cur == L'-') {         /* 0x140034A3E */
            for (;;) {
                WCHAR *np;

                if (cur[2] != L'-') /* 0x140034A61 */
                    break;
                err = (int32_t)0x80070057; /* 0x140034A4F */

                /* T1 --sub (0x140034A67): 匹配并推进 */
                if (PECMD_MatchTokenAdvance("--sub", (uint64_t *)&cur, 5) != 0) {
                    slot_45 = 1;    /* [0x45]=1 */
                }
                /* T2 --user (0x140034A98) */
                else if (PECMD_MatchTokenAdvance("--user", (uint64_t *)&cur, 6) != 0) {
                    slot_40 = 0x80000;
                }
                /* T3 --visible (0x140034ACA) */
                else if (PECMD_MatchTokenAdvance("--visible", (uint64_t *)&cur, 9) != 0) {
                    slot_44 = 6;
                }
                /* T4 --invisible (0x140034AF0) */
                else if (PECMD_MatchTokenAdvance("--invisible", (uint64_t *)&cur, 11) != 0) {
                    slot_44 = 4;
                }
                /* T5 --class: (0x140034B19): 不推进比较; 跳 8 字符后引号串解析 */
                else if (PECMD_AsciiPrefixICmp("--class:", cur, 8) != 0) {
                    LPCWSTR arg_s = cur + 8;
                    slot_80 = NULL;
                    slot_a0 = (WCHAR *)arg_s;
                    cur = PECMD_NextToken((int64_t *)&slot_a0, (int64_t *)&slot_80,
                                          0x1cd); /* [0x138]=Return */
                    /* TODO(verify): [0xa0] 经 NextToken 后指向 token 尾;
                     * 0x14003539E 仍以 [0xa0] 作文本 — 与反汇编一致, 语义待活体验证 */
                }
                /* T6 --pid (0x140034B84) */
                else if (PECMD_AsciiPrefixICmp("--pid", cur, 5) != 0) {
                    WCHAR *np_cvt;
                    int has_hash = 0;
                    int val = -1;
                    mode = 1;                       /* R13D=1 (0x140034BA7) */
                    np = cur + 5;
                    cur = np;
                    cl_skip_num_run(&cur);          /* 0x140034BDB 扫至空白 */
                    err = (int32_t)0x80070057;      /* 0x140034BF3 */
                    PECMD_SkipLeadingControls((uint64_t *)&cur);
                    /* np 保持 = 数字起始 (R8, 0x140034BBB), 不随 cur 前进
                     * — 反汇编修饰符测试与解析均用 R8 (0x140034C0B/0x140034C82) */
                    /* 修饰符 (0x140034C0B–0x140034C5E) */
                    if (*np == L'*') { r14b |= 1; np += 1; }
                    if (*np == L'@') { r15b = (uint8_t)L'@'; np += 1; }
                    if (*np == L'.') { r14b |= 2; np += 1; }
                    if (*np == L'#') { has_hash = 1; np += 1; }
                    np_cvt = np;
                    r = PECMD_ParseHexOrDecBool((int64_t **)(void *)&np_cvt, &val); /* 0x140034C82 */
                    (void)r;
                    if (val <= 0)
                        err = (int32_t)0x80070057;  /* CMOVLE ESI,R12D */
                    if (has_hash)
                        pid_hash = val;             /* [0xb0] */
                    else
                        pid_plain = val;            /* [0x6c] */
                }
                /* T7 --menu (0x140034CB9) */
                else if (PECMD_AsciiPrefixICmp("--menu", cur, 6) != 0) {
                    np = cur + 6;
                    cur = np;
                    cl_skip_num_run(&cur);          /* 0x140034D1C */
                    err = (int32_t)0x80070057;      /* 0x140034D30 */
                    PECMD_SkipLeadingControls((uint64_t *)&cur);
                    /* np 保持 = 数字起始 (R8), '#': 检查在原始 R8 上 (0x140034D4C) */
                    mode = 2;                       /* R13D=2 (0x140034D48 段) */
                    if (*np == L'#') {              /* 0x140034D4C 强制 '#' */
                        np += 1;
                        menuid = 0;
                        PECMD_ParseShortStore(&np, &menuid, (WCHAR)0x2c); /* 0x140034D76 */
                        /* 0x140034D7F: EAX(返回值)<=0 → 兜底; 项目声明 void,
                         * 以 menuid 值代偿 + TODO(verify) 主代理核对真实返回 */
                        if (menuid <= 0) {
                            r15b = 1;               /* 0x140034D91 */
                            menuid = 1;
                        } else {
                            menuid = (int32_t)(int16_t)menuid; /* MOVSX word */
                        }
                    } else {
                        r15b = 1;                   /* 兜底: [0x48]=1 */
                        menuid = 1;
                    }
                }
                /* T8 --wid (0x140034D9D) */
                else if (PECMD_AsciiPrefixICmp("--wid", cur, 5) != 0) {
                    int has_hash = 0;
                    int val = 0;
                    mode = 16;                      /* R13D=0x10 (0x140034E32) */
                    np = cur + 5;
                    cur = np;
                    cl_skip_num_run(&cur);          /* 0x140034E00 */
                    err = (int32_t)0x80070057;      /* 0x140034E14 */
                    PECMD_SkipLeadingControls((uint64_t *)&cur);
                    /* np 保持 = 数字起始 (R8), 修饰循环在原始 R8 上 (0x140034E48) */
                    for (;;) {                      /* 0x140034E48 修饰循环 */
                        WCHAR c = *np;
                        if (c == L'*') { r14b = (uint8_t)c; np += 1; }
                        else if (c == L'@') { r15b = (uint8_t)c; np += 1; }
                        else if (c == L'#') { has_hash = 1; np += 1; }
                        else
                            break;
                        if (*np == 0)
                            break;
                    }
                    slot_58 = r15b;                 /* 0x140034E7B */
                    err = (int32_t)0x80070057;      /* 0x140034E80 */
                    sz_b8 = 0;
                    r = PECMD_ParseUIntValue(&np, &sz_b8); /* 0x140034E96 */
                    val = sz_b8;
                    if (r <= 0 || val <= 0) {
                        err = (int32_t)0x80070057;  /* 0x140034EB4 */
                    } else if (has_hash) {
                        wid_hash = val;             /* [0xd8] */
                    } else {
                        wid_plain = val;            /* [0x90] */
                    }
                }
                /* T9 --forpid: (0x140034ED6): 跳 9 字符 → [0x6c] */
                else if (PECMD_AsciiPrefixICmp("--forpid:", cur, 9) != 0) {
                    np = cur + 9;
                    cur = np;
                    cl_skip_num_run(&cur);          /* 0x140034F2F */
                    err = (int32_t)0x80070057;      /* 0x140034F43 */
                    PECMD_SkipLeadingControls((uint64_t *)&cur);
                    slot_80 = cur;
                    PECMD_ParseUIntValue(&slot_80, &pid_plain); /* 0x140034F63 */
                }
                /* T10 --fortid: (0x140034F6D): 跳 9 字符 → [0xb4] */
                else if (PECMD_AsciiPrefixICmp("--fortid:", cur, 9) != 0) {
                    np = cur + 9;
                    cur = np;
                    cl_skip_num_run(&cur);          /* 0x140034FC6 */
                    err = (int32_t)0x80070057;      /* 0x140034FDA */
                    PECMD_SkipLeadingControls((uint64_t *)&cur);
                    slot_80 = cur;
                    PECMD_ParseUIntValue(&slot_80, &fortid); /* 0x140034FF5 */
                }
                else {
                    /* 兜底: 未知 --xxx 跳过该 token (0x140034FFA) */
                    cl_skip_num_run(&cur);          /* 0x140035013 */
                    err = (int32_t)0x80070057;      /* 0x140035037 */
                    PECMD_SkipLeadingControls((uint64_t *)&cur);
                }

                /* 底部回边 (0x14003504E/0x14003505B): cur[0]=='-' 则继续 */
                if (*cur != L'-')
                    break;
            }
        }
    }

    /* 环出口 (0x14003506A): [0x58]=r15b */
    slot_58 = r15b;

    /* ============ E'. 模式分派 (mode!=0: --pid/--menu/--wid) ============ */
    if (mode != 0) {
        int64_t r_mode = 0;

        PECMD_AllocStrSlot(&buf_a);                     /* NewStrBuf A [0x70] */
        PECMD_ExpandVarDispatch(script, cur, &buf_a, 0, one); /* 0x1400350A7 */

        if (err != 0 || (*cur == 0 && mode != 3)) {
            /* 错误/空命令 → SetVar(script, bufA, L"") + 返 err (0x1400350CB–F6) */
            FUN_1400629b8(script, buf_a != NULL ? buf_a : L"", L""); /* L""@0x14011C638 */
            PECMD_FreeStrBuf(&buf_a);
            PECMD_FreeStrBuf(&buf_c);
            if (err != 0)
                return (int64_t)(int32_t)err;
            return (int64_t)(int32_t)0x80070057;
        }

        /* 公共段 (0x140035114–0x14003515C): buf B = act1 展开; 累积缓冲 [0x78] */
        if (mode == 1 && act1 != NULL)
            PECMD_SkipLeadingControls((uint64_t *)&act1); /* 0x14003510F */
        PECMD_AllocStrSlot(&buf_b);
        if (act1 != NULL)
            PECMD_ExpandVarDispatch(script, act1, &buf_b, 0, one); /* 0x14003513E */
        slot_80 = buf_b;                                /* [0x80]=bufB (0x140035154) */
        PECMD_AllocStrSlot(&buf_78);                    /* NewStrBuf [0x78] */

        if (mode == 1) {
            /* ---- --pid: FUN_14002d708 (0x14003517E–0x1400351AD) ---- */
            uint32_t flags2;
            int64_t *outbuf = NULL;
            if (r15b != 0)                              /* NEG/SBB → &[0x78] */
                outbuf = (int64_t *)&buf_78;
            flags2 = (uint32_t)r14b | (uint32_t)slot_40 | (uint32_t)slot_68;
            /* 0x1400351A8: (script, flags2, outbuf, pid_hash, pid_plain) */
            r_mode = (int64_t)FUN_14002d708(script, flags2, outbuf, pid_hash,
                                            (uint32_t)pid_plain);
        } else if (mode == 2) {
            /* ---- --menu: 菜单枚举转储 (0x1400351B2–0x140035336) ---- */
            int hwnd;
            WCHAR *np = buf_b;
            int pos, cnt;
            HMENU hmenu;
            PECMD_ParseUIntValue(&np, &slot_40);        /* 0x1400351CD → hwnd */
            hwnd = slot_40;
            if (menuid == (int32_t)0x80000000) {
                /* 无 #id: GetMenu(hwnd) 校验 (0x1400351E4); 结果流向 SetVarFmt64 */
                r_mode = (int64_t)(uintptr_t)GetMenu((HWND)(uintptr_t)hwnd);
            } else if (menuid <= 0) {
                if (hwnd > 0) {                         /* 0x140035201 */
                    r_mode = (int64_t)GetMenuItemCount((HMENU)(uintptr_t)hwnd);
                }
            } else if (hwnd > 0) {
                /* 枚举环 L4 (0x14003521B–0x140035316) */
                hmenu = (HMENU)(uintptr_t)hwnd;
                cnt = GetMenuItemCount(hmenu);          /* 0x140035233 */
                pos = menuid - 1;
                r_mode = 0xd;                           /* ESI=0xd (0x140035239) */
                PECMD_AllocStrSlot(&slot_a0);           /* @ 行缓冲 (0x14003521B) */
                for (;;) {
                    HMENU sub;
                    UINT id;
                    WCHAR *row;
                    int wlen;
                    if (r15b != 0 && pos >= cnt)        /* @ 模式界检 */
                        break;
                    sub = GetSubMenu(hmenu, pos);       /* 0x140035255 */
                    /* 行缓冲: @ → [0xa0] 专有; 非@ → [0x78] 累积缓冲 (CMOVZ, 0x140035278) */
                    if (r15b != 0) {
                        PECMD_AllocString(&slot_a0, 0x1065); /* 0x14003527F */
                        row = slot_a0;
                        row[0] = 0xd;                   /* \r\n 行前缀 (0x140035294) */
                        row[1] = 0xa;
                        row += 2;
                    } else {
                        PECMD_AllocString(&buf_78, 0x1065); /* 0x14003527F/278 */
                        row = buf_78;
                    }
                    id = GetMenuItemID(hmenu, pos);     /* 0x1400352A7 */
                    wsprintfW(row, L"%ld\t%lu\t%ld\t", pos + 1,
                              (unsigned long)(uintptr_t)sub,
                              (unsigned long)id);       /* fmt@0x140121680 */
                    wlen = lstrlenW(row);
                    GetMenuStringW(hmenu, (UINT)pos, row + wlen, 0x1000, 0x400);
                    /* 0x1400352FA: [0x58]==0 → 单行即止 */
                    if (r15b != 0) {
                        PECMD_AppendWideStr(&buf_78, slot_a0); /* 0x140035309 */
                        pos += 1;
                    } else {
                        break;
                    }
                }
                PECMD_FreeStrBuf(&slot_a0);             /* 0x140035326 */
                r15b = 1;                               /* 0x140035323 */
            }
        } else if (mode == 16) {
            /* ---- --wid (0x14003533B–0x1400353C3) ---- */
            if (wid_hash != 0) {
                int child_id = 0;
                WCHAR *np = buf_b;
                PECMD_ParseUIntValue(&np, &slot_40);    /* 0x140035359 */
                child_id = slot_40;
                r_mode = (int64_t)(uintptr_t)GetDlgItem(
                    (HWND)(uintptr_t)wid_hash, child_id); /* 0x140035365 */
            } else {
                /* EnumSearch: PECMD_FindTargetWindow 八参 (0x1400353C3) */
                char p2 = (char)(slot_44 | slot_45);
                char p3 = (char)(r14b != 0);
                HWND p4 = (HWND)(uintptr_t)wid_plain;
                uint32_t p5 = (uint32_t)pid_plain;      /* [0x6c] forpid */
                uint32_t p6 = (uint32_t)fortid;         /* [0xb4] fortid */
                uint64_t p7 = (r15b != 0) ? (uint64_t)(uintptr_t)&buf_78 : 0;
                uint64_t p8 = (uint64_t)(uintptr_t)slot_a0; /* [0xa0] class */
                r_mode = (int64_t)PECMD_FindTargetWindow(buf_b, p2, p3, p4, p5, p6,
                                                         p7, p8);
            }
        }

        /* 结果写回 + 释放 (0x1400353CB–0x140035421); 函数返 0 (0x140035426) */
        if (r15b != 0) {
            FUN_1400629b8(script, buf_a != NULL ? buf_a : L"",
                          buf_78 != NULL ? buf_78 : L""); /* 0x1400353DD */
        } else {
            PECMD_AppendFmtValue(script, (uint64_t)r_mode, buf_a != NULL ? buf_a : L"",
                                 L"%I64u");             /* fmt@0x14011C538 */
        }
        PECMD_FreeStrBuf(&buf_78);
        PECMD_FreeStrBuf(&buf_b);
        PECMD_FreeStrBuf(&buf_a);
        PECMD_FreeStrBuf(&buf_c);
        return 0;
    }

    /* ============ F. 表达式路径 (0x14003542B–0x14003583C) — 主语义 ============ */
    if (act1 == NULL) {
        goto cl_f_done; /* 0x140035430: act1==0 → 收尾 */
    }
    {
        WCHAR *r11 = act1;            /* 未修剪 act1 */
        WCHAR *r9;
        WCHAR *hit;
        WCHAR *p_trim;

        act2 = NULL;
        slot_a8 = NULL;
        negform = 0;

        /* 0x140035454: 修剪副本 (注释: 反汇编在 [0x90] 做修剪副本) */
        p_trim = act1;
        PECMD_SkipLeadingControls((uint64_t *)&p_trim);
        r9 = p_trim;

        /* 切分识别: '!!' → negform=1; '!' → negform=2; 否则 spec_char 切第二段 */
        if (r11[0] == L'!' && r11[1] == L'!') {
            act1 = r11 + 2;                        /* 0x140035468 */
            PECMD_SkipLeadingControls((uint64_t *)&act1);
            negform = 1;                          /* BL=1 (0x14003547B) */
            r9 = NULL;
        } else if (r9 != NULL && r9[0] == L'!') {
            act2 = r9;                            /* [0x60]=trimmed (0x14003548F) */
            negform = 2;                          /* BL=2 */
            r9 = act2;
        } else {
            /* 常规: 首未转义 spec_char 切第二段 (0x140035498–0x1400354C1) */
            hit = PECMD_RemoveDuplicateChar(r11, spec_char); /* 0x140025274 */
            act2 = hit;
            r9 = hit;
            if (hit != NULL) {
                PECMD_CollapseRepeatedChars(hit + 1, spec_char); /* 0x140025404 */
                r9 = act2;
            }
        }

        /* 0x1400354C6: 修剪 act1 本体 */
        PECMD_SkipLeadingControls((uint64_t *)&act1);

        /* ---- %var% 绑定 / 括号配对区 (0x1400354D0–0x14003573C) ----
         * C1 常规案 (无 {..} 引导) 量化结果: r13b=0, bl=0, rbp=0, [0xa8]=0。
         * 括号路径按反汇编结构直译, 数据驱动; 详细对齐见 notes §4.6。 */
        {
            LPCWSTR ov;
            WCHAR r10w = 0, r11w = 0, ax = 0;
            WCHAR lead;
            WCHAR r13w;
            WCHAR *r8 = NULL;
            int brace_path = 0;

            ov = (out_var != NULL) ? (LPCWSTR)(uintptr_t)*out_var : NULL;
            rbp_brace = NULL;

            if (ov != NULL) {
                /* R10W = script+{0x48} ^ *ov (0x140035517) */
                r10w = (WCHAR)(*(WCHAR *)((uint8_t *)script + 0x48) ^ *ov);
            } else {
                r10w = 0;
            }

            if (r9 != NULL) {
                /* 0x140035526: 就地截断; act2 指向切点后 (跳过分隔字符) */
                *r9 = 0;
                if (act2 != NULL)
                    act2 += 1;
                PECMD_SkipLeadingControls((uint64_t *)&act2);
                r9 = act2;
                ax = (r9 != NULL) ? *r9 : 0;
            }

            lead = *(WCHAR *)((uint8_t *)script + 0x48);
            r13w = (WCHAR)(lead ^ delim_word);  /* 0x140035567 */
            r11w = (act1 != NULL) ? *act1 : 0;
            (void)r13w; (void)r11w;

            /* 主判据 (0x140035570–0x14003569F):
             *   '{'==r10w || ('{'==r11w && r9==NULL) → 括号反向扫描;
             *   '{'==ax && r11w==0 && negform==0 → act2 括号绑定;
             *   否则常规路径。 */
            if (r10w == L'{' || (r11w == L'{' && r9 == NULL) ||
                (ax == L'{' && r11w == 0 && negform == 0)) {
                /* 括号路径 (0x14003559D–0x14003568C):
                 * 反汇编直译 — 反向以 [0x8a]/[0x90] 定界, 正向以 [0x88] 与
                 * r13w/'!'-**调和 [0x92]/[0x94]/[0x8a]/[0x90] 至闭合 [0x96]。
                 * 因 C1 常规案不进此支, 精读留 TODO(verify); 结构与调用保持。 */
                WCHAR *lo = *(WCHAR **)((uint8_t *)script + 0x80);
                r8 = (WCHAR *)ov;                       /* R8=*out_var 指针 */
                brace_path = 1;
                while (r8 != NULL && (uintptr_t)r8 > (uintptr_t)lo) {
                    uint16_t c = *(uint16_t *)r8;
                    if (c == *(uint16_t *)((uint8_t *)script + 0x8a) ||
                        c == *(uint16_t *)((uint8_t *)script + 0x90))
                        break;
                    r8 -= 1;
                }
                if (negform == 2)
                    slot_a8 = r8;                       /* 0x140035679 */
                else
                    rbp_brace = r8;                     /* 0x140035683 */
                r13b = 1;
                /* 0x140035686/0x1400356E2: AdvanceTokenPointer(script, out_var,
                 *   mode=(negform==2)?1:0x10000, &slot_a8) */
                PECMD_AdvanceTokenPointer((int64_t)(uintptr_t)script, out_var,
                                          (negform == 2) ? 1 : 0x10000,
                                          (int64_t *)(uintptr_t)&slot_a8);
            } else {
                /* 常规: 0x140035718 r13d=1 → 0x14003571E bl=0 →
                 * 0x140035721…73C: slot_a8 有效闭合判定 → r13b */
                r13b = 1;
                negform = 0;                            /* 0x14003571E: BL=0 */
                if (slot_a8 != NULL) {
                    if (*(WCHAR *)((uint8_t *)script + 0x96) == *slot_a8)
                        r13b = 1;
                    else
                        r13b = 0;
                } else {
                    r13b = 0;
                }
            }
            (void)brace_path;
        }

        /* ---- 条件求值 (0x14003573F–0x140035774) ---- */
        cond_text = cur;                                /* [0x138] 修剪后条件 */
        {
            uint32_t flags = (uint32_t)verb_mode | (uint32_t)ab_bit | (uint32_t)slot_68;
            ULARGE_INTEGER cr;
            /* 0x140035760: (script, cond_text, flags, act1) */
            cr = PECMD_EvalLoopCondition((int64_t *)script, cond_text, (int)flags,
                                         act1);
            /* 0x14003576D: SETG — 有符号 >0 */
            truthy = ((int64_t)cr.QuadPart > 0);
        }
        PECMD_SkipLeadingControls((uint64_t *)&act1);   /* 0x140035774 */

        /* 分派 (0x140035779–0x140035839) —— 返回值无强制:
         * 真分支 → PSB(act1) 原样回传 (ENVI→0 等);
         * 假分支 → PSB(act2 "ELSE <分支>") 原样回传 (该行返 2, U-1 §5)。 */
        if (saw_pipe) {
            /* 管道模式 (Q3): 真→act1, 假→act2, 交 PECMD_ParseCommandBlock */
            if (truthy) {
                result = (int64_t)PECMD_ParseCommandBlock((int64_t)(uintptr_t)script,
                                                          &act1, 0,
                                                          (void *)(uintptr_t)locale);
            } else if (act2 != NULL) {
                result = (int64_t)PECMD_ParseCommandBlock((int64_t)(uintptr_t)script,
                                                          &act2, 0,
                                                          (void *)(uintptr_t)locale);
            } else {
                goto cl_f_done;                         /* 0x14003581F */
            }
        } else if (truthy) {
            if (negform != 0) {
                /* 0x14003578E–0x1400357B9: 脚本动作执行器 (FUN_1400a53e4) */
                slot_d0 = (WCHAR *)((uintptr_t)rbp_brace + 2); /* RBP+2 */
                /* TODO(verify): 常规 '!' 形态 rbp_brace==0 时反汇编为 LEA [RBP+2]
                 * = 地址 2, 原版依赖括号路径先置 RBP; 草稿加防御避免裸解引用,
                 * 语义以活体为准。 */
                if ((uintptr_t)slot_d0 < (uintptr_t)0x10000)
                    slot_d0 = act1;
                result = PECMD_TokenizeExpression(
                    cl_mk_li((uintptr_t)script),
                    *(int64_t *)((uint8_t *)script + 0x40),
                    (int64_t *)&slot_d0, 0, L"");
            } else {
                /* 0x1400357C0–0x1400357E5: 递归大解释器执行真分支动作 */
                if (act1 == NULL || *act1 == 0)
                    goto cl_f_done;                     /* 0x1400357C5/7CD */
                result = (int64_t)PECMD_ProcessScriptBlock(
                    cl_mk_li((uintptr_t)script),
                    cl_mk_li((uintptr_t)act1), NULL, NULL,
                    (void *)(uintptr_t)locale).QuadPart;
            }
        } else {
            if (act2 == NULL && slot_a8 == NULL)
                goto cl_f_done;                         /* 0x140035801 */
            if (r13b != 0) {
                /* 0x140035808: a8+2 → 脚本动作执行器 */
                slot_d0 = (WCHAR *)((uintptr_t)slot_a8 + 2);
                result = PECMD_TokenizeExpression(
                    cl_mk_li((uintptr_t)script),
                    *(int64_t *)((uint8_t *)script + 0x40),
                    (int64_t *)&slot_d0, 0, L"");
            } else {
                /* 反汇编 0x1400357EC 先加载 RDX=[0x60](act2 = "ELSE <分支>" 行文本),
                 * r13b==0 落入 0x1400357C5 共享调用点 → PECMD_ProcessScriptBlock(act2)。
                 * U-1 动态实测绘得嵌套行 = "ELSE ENVI R=missing" (013/016), 该行执行返 2
                 * (PSB 尾部 LAB_14004c525 尾写 2, 内外两次), 处理器(本函数)回传子执行结果。
                 * 注: "ELSE " 前缀在语料中由脚本文本携带 (act2 经 '!' 切分后保留),
                 * 假分支分派段 (0x1400357EC–0x1400357E5) 无反汇编证据表明现场拼接 StrBuf;
                 * 语料外写法 `!CMD`(无字面 ELSE) 行为 → TODO(verify live)。 */
                if (act2 == NULL || *act2 == 0)
                    goto cl_f_done;                     /* 0x1400357C5/C7CD (RDX=act2) */
                result = (int64_t)PECMD_ProcessScriptBlock(
                    cl_mk_li((uintptr_t)script),
                    cl_mk_li((uintptr_t)act2), NULL, NULL,
                    (void *)(uintptr_t)locale).QuadPart;
            }
        }
    }

cl_f_done:
    /* 0x14003583C: FreeStrBuf(&[0xc0]); RAX=RDI — 无条件强制; 返回值 = 分派路径
     * 原样 (U-1 模型: 真分支 ENVI→0 / 假分支 ELSE 行→2 由 PSB 产生并经本函数回传) */
    PECMD_FreeStrBuf(&buf_c);
    return result;
}