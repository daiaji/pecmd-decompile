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
typedef uint8_t byte;
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

/* ---- REGI 命令引擎 (FUN_1400c13f8) 依赖 ---- */
extern void PECMD_EnableBackupPrivileges(void);                          /* @0x140060698 (core_b3i.c) */
extern void PECMD_InitRamdataRegistry(int n);                            /* @0x14001b888 (core_execscript.c) */
extern WCHAR *PECMD_AllocString(WCHAR **ps, int64_t count);              /* @0x140063720 (core_string.c) */
extern void PECMD_AllocWStringBuffer(WCHAR **ps, int64_t count);         /* @0x140063694 (link_stubs.c) */
extern WCHAR *PECMD_AppendWideStr(WCHAR **ps, LPCWSTR src);              /* @0x14006375c (core_string.c) */
extern WCHAR *PECMD_StrDupA(WCHAR **ps, LPCSTR src, int64_t len, int64_t cp); /* @0x1400637dc (core_exec5.c) */
extern void FUN_1400629B8(void *script, LPCWSTR key, LPCWSTR value);     /* @0x1400629b8 SetVariable (core_b2f.c) */
extern int64_t *PECMD_AssignString(int64_t *ps, LPCWSTR src);            /* @0x14007034c (core_strbld.c) */
extern int64_t PECMD_ParseHex_4a34(uint16_t *s);                         /* @0x140064a34 (core_b3e.c) */
extern uint64_t PECMD_EvalParenStripped(WCHAR **pp, uint64_t *out);      /* @0x1400745c8 (link_stubs.c) */
extern bool PECMD_ParseUIntValue(WCHAR **pp, int *out);                  /* @0x140074838 (core_remaining_helpers.c) */
extern bool PECMD_ParseNumSkipWs(WCHAR **pp, uint64_t *out);             /* @0x140067cf4 (core_remaining_helpers.c) */
extern uint64_t PECMD_ParseHexByteList(LPCWSTR str, int64_t *outBuf, int mode); /* @0x1400c12fc (core_b7a.c) */
extern void PECMD_ExpandVarDispatch(void *script, WCHAR *line, WCHAR **out,
                                     int mode, uint8_t opt);             /* @0x14007bf44 (core_execline.c) */
extern DWORD PECMD_RegSetValueWithOpen(HKEY root, LPCWSTR sub, LPCWSTR name,
                                       DWORD type, BYTE *data, DWORD size); /* @0x14005c5a0 */
extern DWORD PECMD_RegDeleteValue(HKEY root, LPCWSTR subkey, LPCWSTR name); /* @0x14005c61c */
extern DWORD PECMD_DeleteRegistryKeyRecursive(HKEY root, LPCWSTR sub);   /* @0x1400717f0 (core_b3_remaining.c) */
extern DWORD PECMD_RegScrubKey(HKEY root, LPCWSTR sub, unsigned int flags); /* @0x140071ae8 (core_b3r_g4.c) */
extern LONG PECMD_RegCreateKeyRetryWow64(HKEY root, LPCWSTR sub, int vol); /* @0x14005f750 (core_b3j.c) */
extern int FUN_1400690C0(HKEY root, LPCWSTR subkey, LPCWSTR name,
                         int64_t *out, DWORD *size, LONG *status);       /* @0x1400690c0 (core_b3n.c) */
extern WCHAR *PECMD_SprintfRetEnd(WCHAR *dst, uint64_t v, LPCWSTR fmt);  /* @0x1400e6d38 (core_strbld.c) */
extern int64_t FUN_140079C80(int64_t *ctx, int64_t *pp, int64_t *out); /* @0x140079c80 连接tokens (core_b3h.c) */

/* 数据槽: DAT_14011c638 = g_szEmpty (.rdata 空串, pecmd_defs.h) */

/* 本文件 static 展开包装前向声明 (REGI 先于其定义使用) */
static longlong b7c_003a20(longlong *script, WCHAR **slot, uint8_t flag);


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
 * 'REGI' 注册表命令引擎 (decompiled.c @119655 size=8911 忠实直移)。
 *
 * 行格式: [**|--引导]<opts…> Root\SubKey[\ValueName] (=|,|!) Data
 *   引导符 '*'/'-' 成对出现才进入逐词选项解析; 选项词:
 *   0[:<n>] 访问位 / t:<n> 期望类型槽 / 16 无符号十进制输出 / r10 /
 *   byte 字节列表 / a 全枚举 / av / ak / name(值名取自数据侧) /
 *   v± / k / qk(SHSetValueW) / init(备份特权) / su。
 *   类型前缀: $SZ ~EXPAND_SZ *MULTI_SZ #DWORD +QWORD @BINARY ^LINK
 *             uMUI_SZ bBIG_ENDIAN nNONE; '.' 枚举输出 '?' 仅类型名。
 *   根键名: HKLM/HKCU/HKCR/HKU/HKCC/全名/数值句柄(FUN_140067cf4)。
 * 分隔: '=' 写值 / '!' 删除 / ',' 查询写变量(缺省 RegDat)。
 * 两遍扫描: 先原始行; 未命中分隔符则整行展开变量后重扫一遍;
 * 再命中 "---initram" 则 PECMD_InitRamdataRegistry(3)。
 * 返回: 强转 HKEY 的状态码 (0/1/LSTATUS/0x80070057); 绝不返回对象指针。
 */
HKEY PECMD_RegiEditRegistry(HKEY a1, HKEY a2, char a3)
{
    WCHAR WVar10;
    char cVar6;
    bool bVar4;
    bool bVar7;
    bool bVar8;
    ushort uVar9;
    int iVar11;
    LONG LVar12;
    DWORD DVar13;
    DWORD DVar24;
    DWORD DVar35;
    char *pcVar15;
    WCHAR *pWVar16;
    longlong *plVar17;
    ulonglong uVar20;
    ulonglong uVar31;
    byte bVar21;
    uint uVar22;
    byte bVar26;
    byte bVar28;

    /* --- 寄存器复用槽 (Ghidra 混型, int64 承接、使用点强转) --- */
    int64_t script;        /* local_res8: 脚本对象; 写路径复用作字节缓冲槽 */
    WCHAR  *cursor;        /* local_res10 命令行游标 */
    char   delim_keep;     /* local_res10 低字节保存的分隔符 ('!' 判定) */
    int64_t s_70;          /* SplitTokenAssignVar 槽 */
    int64_t s_78;          /* local_78: 原始行→子键路径; 枚举分支复用为缓冲 */
    int64_t s_f0;          /* 根键名展开槽 */
    int64_t s_c0;          /* 空闲槽 (真体分配后释放) */
    int64_t s_108;         /* 输出变量名槽 */
    int64_t s_c8;          /* '=' 数据展开结果槽 */
    int64_t s_e0;          /* '.' 模式第二输出变量名槽 */
    int64_t s_b8;          /* 值名展开槽 */
    int64_t s_a8;          /* -name 选项提取的值名 */
    int64_t s_50;          /* 值表达式副本槽 */
    int64_t s_110;         /* 游标 → 输出缓冲 */
    int64_t s_f8;          /* 值串游标 → 类型名缓冲 */
    int64_t s_120;         /* 多角色: 数值/默认数据/注册表原始数据缓冲 */
    int64_t s_98;          /* 重试展开后的整行 */
    DWORD   flg_88[2];     /* local_88[0]: '?' 标记 / 枚举值计数 */
    DWORD   cnt_b0;        /* local_b0[0]: '0' 访问位标志 */

    /* --- 选项/流程状态 --- */
    byte   prefix;         /* local_res20[0] 类型前缀字符 */
    ulonglong want_type;   /* local_90 期望类型 (-0x1000=未指定) */
    uint   aflags;         /* local_128 a/av/ak 位组合 */
    char   opt16;          /* local_127 "16" */
    char   optr10;         /* local_117 "r10" */
    char   optname;        /* local_d0 "name" */
    char   optqk;          /* local_e8 "qk" */
    char   optsu;          /* local_d8 "su" */
    char   v_cnt;          /* local_125 "v"/"v-" 计数 */
    char   k_cnt;          /* local_124 "k" 计数 */
    char   mark2;          /* local_118 第二标记 ('$'/重复前缀) */
    char   pass_flag;      /* local_res18 = a3: 非零禁止二次展开 */
    char   pass_cnt;       /* local_e7/cVar34 两遍计数 */

    HKEY   retv;           /* pHVar27 返回值 */
    HKEY   root_key;       /* pHVar36 根键游标→根键句柄 */
    int64_t root_slot;     /* local_100: 根名游标→根键→输出目标选择 */
    WCHAR  *valcur;        /* pHVar29 值数据游标 */
    WCHAR  *subp;          /* pHVar25 子键游标 */
    WCHAR  *valname;       /* pWVar16/local_80 值名 */
    WCHAR  *a0;            /* local_a0 数据区游标 */
    DWORD  status_q;       /* local_50 低半: 查询状态返回值 */
    BYTE  *datap;          /* pHVar30 写数据指针 */
    ulonglong wtype;       /* uVar31 写类型 */
    int64_t *outsel;       /* pHVar25(查询): 类型名目标槽 &s_f8/&s_110 */
    DWORD  data_size;      /* local_d4 读值大小 */
    LONG   rd_stat;        /* local_res10(查询) 读状态 */
    int    out_off;        /* local_58 输出偏移 wchar 数 */

    /* ---- MULTI_SZ/'~' 拷贝循环局部 (LAB_1400c324c/joined_r0x…) ---- */
    WCHAR  *cp_src;        /* 源游标 */
    WCHAR  *cp_end;        /* 源结束 */
    WCHAR  *cp_dst;        /* 目的游标 */
    int     cp_sep;        /* 分隔计数 */
    int     cp_len;        /* 源长 */

    script = (int64_t)(intptr_t)a1;              /* local_res8 */
    cursor = (WCHAR *)(uintptr_t)a2;             /* local_res10 */
    delim_keep = '\0';
    prefix = 0;
    s_78 = (int64_t)(uintptr_t)a2;               /* local_78 = param_2 */
    PECMD_AllocStrSlot((WCHAR **)&s_70);
    retv = (HKEY)0;                              /* pHVar27 */
    bVar8 = false;
    pass_cnt = -1;
    bVar26 = 0;
    v_cnt = '\0';
    k_cnt = '\0';
    optr10 = '\0';
    s_98 = 0;
    valcur = NULL;
    subp = NULL;
    valname = NULL;
    a0 = NULL;
    status_q = 0;
    /* local_68=param_1 与 local_60=0 为死赋值, 略。 */

    FUN_14005B154(&cursor);
    cVar6 = PECMD_MatchTokenAdvance("---initram", &cursor, 10);
    if (cVar6 == '\0') {
      do {
        prefix = 0;                              /* local_res20[0] */
        want_type = 0xfffffff000000000ULL;       /* local_90 */
        cVar6 = '*';
        if (*cursor == L'-') {
            cVar6 = '-';
        }
        mark2 = '\0';                            /* local_118 */
        opt16 = '\0';                            /* local_127 */
        aflags = 0;                              /* local_128 */
        optname = '\0';                          /* local_d0 */
        bVar4 = false;
        optqk = '\0';                            /* local_e8 */
        optsu = '\0';                            /* local_d8 */
        DVar13 = 0;
        cnt_b0 = 0;                              /* local_b0[0] */
        {
            /* ---- 选项词循环: 引导符需成对 ---- */
            uint cur_flags = 0;                  /* uVar22/pHVar25 访问位累积 */
            WCHAR *optp = cursor;                /* pHVar36/pHVar30 选项词首 */

            WVar10 = *cursor;
            while ((((WVar10 == (WCHAR)(short)cVar6 &&
                      (cursor[1] == (WCHAR)(short)cVar6)) &&
                     ((uint16_t)((uint16_t)cursor[1] & 0xffdf) != 0x48)) &&
                    ((((uint16_t)cursor[1] < 9) || (0xd < (uint16_t)cursor[1])) &&
                     (cursor[1] != L' ')))) {
                cursor = optp + 1;
                if (PECMD_AsciiPrefixICmp("0", optp, 1)) {
                    /* "0" 选项: 0:<n> 显式访问位; 否则 |=7。
                     * ':' 判定固定于 '0' 后首字符, 词尾扫描独立推进 */
                    pWVar16 = optp + 1;
                    {
                        WCHAR *wscan = optp;
                        while (((*wscan != L'\0') &&
                                ((((uint16_t)*wscan < 9) ||
                                  (0xd < (uint16_t)*wscan)))) &&
                               (*wscan != L' ')) {
                            wscan = wscan + 1;
                        }
                        cursor = wscan;
                    }
                    FUN_14005B154(&cursor);
                    if (*pWVar16 == L':') {
                        WCHAR *np = pWVar16 + 1;
                        int tvv = 0;
                        if (PECMD_ParseHexOrDecBool(&np, &tvv)) {
                            cur_flags = cur_flags | (uint)tvv;
                        }
                    } else {
                        cur_flags |= 7;
                    }
                }
                else if (PECMD_AsciiPrefixICmp("t:", optp, 2)) {
                    /* t:<n> — decompiled 以 +1 wchar 落于 ':' 后由解析器
                     * 处置, 无法核验处保守按字面直移。TODO(verify) */
                    {
                        int tvv = 0;
                        WCHAR *np = optp + 1;
                        if (PECMD_ParseUIntValue(&np, &tvv)) {
                            want_type = (ulonglong)(uint)tvv;
                        }
                    }
                    pWVar16 = optp;
                    while (((*pWVar16 != L'\0') &&
                            ((((uint16_t)*pWVar16 < 9) ||
                              (0xd < (uint16_t)*pWVar16)))) &&
                           (*pWVar16 != L' ')) {
                        pWVar16 = pWVar16 + 1;
                    }
                    cursor = pWVar16;
                    FUN_14005B154(&cursor);
                }
                else if (PECMD_MatchTokenAdvance("16", &cursor, 2)) {
                    opt16 = '\x01';
                }
                else if (PECMD_MatchTokenAdvance("r10", &cursor, 3)) {
                    optr10 = '\x01';
                }
                else if (PECMD_MatchTokenAdvance("byte", &cursor, 4)) {
                    bVar4 = true;
                }
                else if (PECMD_MatchTokenAdvance("a", &cursor, 1)) {
                    aflags = 3;
                }
                else if (PECMD_MatchTokenAdvance("av", &cursor, 2)) {
                    aflags |= 2;
                }
                else if (PECMD_MatchTokenAdvance("ak", &cursor, 2)) {
                    aflags |= 1;
                }
                else if (PECMD_MatchTokenAdvance("name", &cursor, 4)) {
                    optname = '\x01';
                }
                else if (PECMD_MatchTokenAdvance("v", &cursor, 1)) {
                    v_cnt = (char)(v_cnt + '\x01');
                }
                else if (PECMD_MatchTokenAdvance("v-", &cursor, 2)) {
                    v_cnt = (char)(v_cnt - '\x01');
                }
                else if (PECMD_MatchTokenAdvance("k", &cursor, 1)) {
                    k_cnt = (char)(k_cnt + '\x01');
                }
                else if (PECMD_MatchTokenAdvance("qk", &cursor, 2)) {
                    optqk = '\x01';
                }
                else if (PECMD_MatchTokenAdvance("init", &cursor, 4)) {
                    PECMD_EnableBackupPrivileges();
                }
                else if (PECMD_MatchTokenAdvance("su", &cursor, 2)) {
                    optsu = '\x01';
                }
                else {
                    /* 缺省: 掠过当前词并跳过空白 */
                    while ((*cursor != L'\0') &&
                           ((((9 <= (uint16_t)*cursor) &&
                              ((uint16_t)*cursor <= 0xd)) ||
                             (*cursor == L' ')) == 0)) {
                        cursor = cursor + 1;
                    }
                    FUN_14005B154(&cursor);
                }
                WVar10 = *cursor;
                optp = cursor;
            }
            cnt_b0 = cur_flags;
        }
        PECMD_AllocStrSlot((WCHAR **)&s_f0);
        PECMD_AllocStrSlot((WCHAR **)&s_c0);
        PECMD_AllocStrSlot((WCHAR **)&s_108);
        PECMD_AllocStrSlot((WCHAR **)&s_c8);
        PECMD_AllocStrSlot((WCHAR **)&s_e0);
        /* ---- 类型前缀字符 ---- */
        uVar9 = (ushort)(*cursor | 0x20);
        if ((((uVar9 == 0x6e) || (uVar9 == 0x75)) ||
             ((uVar9 == 0x62 ||
              (((((*cursor == L'^') || (*cursor == L'+')) ||
                 (*cursor == L'#')) ||
                ((*cursor == L'@') || (*cursor == L'$'))))))) ||
            ((((*cursor == L'*') || ((*cursor == L'~') || (*cursor == L'.'))) ||
              (*cursor == L'?')))) {
            prefix = (byte)*cursor;
            cursor = cursor + 1;
        }
        bVar28 = prefix;
        if ((((prefix != 0) && ((uint)prefix == (uint)(ushort)*cursor)) ||
             (*cursor == L'$'))) {
            mark2 = (char)*cursor;
            cursor = cursor + 1;
        }
        if ((byte)(bVar28 + 0xbf) < 0x1a) {      /* 大写前缀归一为小写 */
            prefix = (byte)(bVar28 | 0x20);
        }
        FUN_14005B154(&cursor);
        root_slot = (int64_t)(intptr_t)cursor;   /* local_100 = 键路径起点 */
        subp = cursor;
        pWVar16 = (WCHAR *)(uintptr_t)StrStrW(cursor, WSTR("\\\\"));
        if (pWVar16 != NULL) {
            subp = pWVar16 + 2;                  /* 越过 UNC '\\' 前缀 */
        }
        /* ---- 分隔符扫描: ',' / '=' / '!' ---- */
        {
            WCHAR *sp = subp;
            WCHAR  dc = L'=';
            a0 = sp;                             /* local_a0 */
            if (*subp != L'\0') {
                for (;;) {
                    a0 = sp;
                    if (L',' == *sp) {
                        WCHAR *q = (WCHAR *)StrChrW(sp + 1, L'=');
                        if (q == NULL) {
                            WCHAR *ex = (WCHAR *)StrChrW(sp + 1, L'!');
                            q = sp;
                            if (ex != NULL) {
                                WCHAR *after = ex + 1;
                                FUN_14005B154(&after);
                                if (*after == L'\0') {
                                    q = ex;      /* 尾部 '!' 视作分隔符 */
                                }
                            }
                        }
                        a0 = q;
                        sp = q;
                        break;
                    }
                    if (dc == *sp) break;        /* '=' 直接命中 */
                    if (*sp == L'!') {
                        WCHAR *after = sp + 1;
                        FUN_14005B154(&after);
                        if (*after == L'\0') break;
                        sp = after - 1;
                    }
                    sp = sp + 1;
                    a0 = sp;
                    if (*sp == L'\0') break;
                }
            }
            cVar6 = (char)*a0;
            if (((*a0 == L',') || (dc == *a0)) || (*a0 == L'!')) {
                *(WCHAR *)(uintptr_t)a0 = L'\0';
                a0 = a0 + 1;                     /* Data 区起点 */
                delim_keep = cVar6;              /* CONCAT71 直移 */
                if (cVar6 == '\0') goto LAB_1400c1c02;
            } else {
                goto LAB_1400c1c02;              /* 未命中任何分隔符 */
            }
        }
        /* ---- 键路径拆分: Root[\SubKey[\ValueName]] ---- */
        s_b8 = 0;
        if ((pWVar16 == NULL) &&
            ((pWVar16 = StrRChrW(subp, NULL, L'\\')) == NULL)) {
            /* 原文无 '\\': 首逗号字段展开后再查 '\\' */
            plVar17 = PECMD_SplitTokenAssignVar(&s_70, (WCHAR **)&root_slot,
                                                0x2c, 1);
            PECMD_SplitTokenTrimWs((WCHAR **)plVar17, (WCHAR **)&s_f0, 0);
            root_slot = s_f0;
            pWVar16 = StrChrW((LPCWSTR)(uintptr_t)s_f0, L'\\');
            if (pWVar16 == NULL) {
                goto LAB_1400c1dd0;
            }
            subp = pWVar16 + 1;
            *pWVar16 = L'\0';
            s_78 = (int64_t)(intptr_t)subp;
            pWVar16 = (WCHAR *)(uintptr_t)StrStrW(subp, WSTR("\\\\"));
            if (aflags == 0) {
                if ((pWVar16 == NULL) &&
                    ((pWVar16 = StrRChrW(subp, NULL, L'\\')) == NULL)) {
                    goto LAB_1400c1dd0;
                }
            }
            else if (pWVar16 == NULL) {
                iVar11 = lstrlenW(subp);
                pWVar16 = subp + iVar11;         /* a 模式: 值名取子键尾空段 */
            }
            if (*pWVar16 != L'\0') {
                *pWVar16 = L'\0';
                pWVar16 = pWVar16 + 1;
            }
            valname = pWVar16;
            goto LAB_1400c1f01;
        }
        {
            WCHAR *lastbs = pWVar16;             /* 最后一个 '\\' 处分界 */
            *lastbs = L'\0';
            PECMD_ExpandVarDispatch((void *)(uintptr_t)script,
                                    subp, (WCHAR **)&s_f0, 0, 1);
            root_slot = s_f0;
            {
                WCHAR *firstbs = StrChrW((LPCWSTR)(uintptr_t)s_f0, L'\\');
                subp = NULL;
                if (firstbs != NULL) {
                    subp = firstbs + 1;
                    *firstbs = L'\0';
                }
                s_78 = (int64_t)(intptr_t)subp;
                PECMD_ExpandVarDispatch((void *)(uintptr_t)script,
                                        lastbs + 1, (WCHAR **)&s_b8, 0, 1);
            }
        }
        valname = (WCHAR *)(uintptr_t)s_b8;
LAB_1400c1f01:
        if (*valname == L'\\') {
            valname = valname + 1;
        }
        a0 = valname;                            /* local_a0 ← local_80 */
        s_a8 = 0;
        if (optname != '\0') {
            FUN_140079C80((int64_t *)(intptr_t)&script,
                          (int64_t *)&a0, (int64_t *)&s_a8);
            a0 = (WCHAR *)(uintptr_t)s_a8;
        }
        /* ---- 按分隔符处理 Data 区 ---- */
        if (cVar6 == ',') {
            plVar17 = PECMD_SplitTokenAssignVar(&s_70, &a0, 0x2c, 1);
            PECMD_SplitTokenTrimWs((WCHAR **)plVar17, (WCHAR **)&s_108, 0);
            b7c_003a20(&script, (WCHAR **)&s_108, 0);
            if ((prefix == 0x2e) && (*a0 == L',')) {
                a0 = a0 + 1;
                plVar17 = PECMD_SplitTokenAssignVar(&s_70, &a0, 0x2c, 1);
                PECMD_SplitTokenTrimWs((WCHAR **)plVar17, (WCHAR **)&s_e0, 0);
                b7c_003a20(&script, (WCHAR **)&s_e0, 0);
            }
            if (*(WCHAR *)(uintptr_t)s_108 == L'\0') {
                PECMD_AssignString(&s_108, WSTR("RegDat"));
            }
        }
        else if (cVar6 != '!') {
            if (*a0 == L'\0') {
                if (prefix != 0x24) {
                    if ((prefix != 0x2a) && (prefix != 0x7e)) {
                        if (k_cnt != '\0') {
                            valcur = a0;
                            s_f8 = (int64_t)(intptr_t)a0;
                        }
                        goto LAB_1400c21b1;
                    }
                    goto LAB_1400c208f;
                }
            }
            else {
LAB_1400c208f:
                if (((prefix != 0x24) && (prefix != 0x2a)) && (prefix != 0x7e)) {
                    FUN_14005B154(&a0);
                }
            }
            PECMD_StrAssign((WCHAR **)&s_50, (LPCWSTR)(uintptr_t)a0);
            valcur = (WCHAR *)(uintptr_t)s_50;
            root_slot = (int64_t)(intptr_t)valcur;   /* local_110 游标副本 */
            FUN_14005B154((WCHAR **)&root_slot);
            if (pass_flag == '\0') {
                PECMD_ExpandCommandLine((void *)(uintptr_t)script,
                                        (WCHAR *)(uintptr_t)s_50,
                                        (WCHAR **)&s_c8, 0, 1);
            }
            else {
                PECMD_AssignString(&s_c8, (LPCWSTR)(uintptr_t)s_50);
            }
            s_f8 = s_c8;
            s_120 = s_c8;
            FUN_14005B154((WCHAR **)&s_120);
            valcur = (WCHAR *)(uintptr_t)s_c8;
            if (prefix == 0) {
                iVar11 = lstrcmpW(WSTR("\"\""), (LPCWSTR)(uintptr_t)root_slot);
                if (iVar11 == 0) {
                    valcur = (WCHAR *)(uintptr_t)g_szEmpty;
                    s_f8 = (int64_t)(intptr_t)g_szEmpty;
                }
                else if ((*(WCHAR *)(uintptr_t)s_120 == L'#') ||
                         (*(WCHAR *)(uintptr_t)s_120 == L'@')) {
                    s_f8 = s_120;
                    valcur = (WCHAR *)(uintptr_t)s_120;
                }
            }
            else if ((prefix == 0x23) || (prefix == 0x40)) {
                valcur = (*(WCHAR *)(uintptr_t)s_120 != L'\0')
                    ? (WCHAR *)(uintptr_t)s_120 : (WCHAR *)NULL;
                s_f8 = (int64_t)(intptr_t)valcur;
            }
            PECMD_FreeStrBuf((WCHAR **)&s_50);
        }
LAB_1400c21b1:
        s_120 = 0;
        /* ---- 根键名解析 (pHVar36 游标→句柄) ---- */
        {
            const WCHAR *rn = (const WCHAR *)(uintptr_t)s_f0;
            (void)rn;
        }
        iVar11 = lstrcmpiW((LPCWSTR)(uintptr_t)s_f0, WSTR("HKLM"));
        if ((iVar11 == 0) ||
            (iVar11 = lstrcmpiW((LPCWSTR)(uintptr_t)s_f0,
                                WSTR("HKEY_LOCAL_MACHINE")),
             iVar11 == 0)) {
            root_key = (HKEY)(uintptr_t)0xffffffff80000002ULL;
        }
        else {
            iVar11 = lstrcmpiW((LPCWSTR)(uintptr_t)s_f0, WSTR("HKCU"));
            if ((iVar11 == 0) ||
                (iVar11 = lstrcmpiW((LPCWSTR)(uintptr_t)s_f0,
                                    WSTR("HKEY_CURRENT_USER")),
                 iVar11 == 0)) {
                root_key = (HKEY)(uintptr_t)0xffffffff80000001ULL;
            }
            else {
                iVar11 = lstrcmpiW((LPCWSTR)(uintptr_t)s_f0, WSTR("HKCR"));
                if ((iVar11 == 0) ||
                    (iVar11 = lstrcmpiW((LPCWSTR)(uintptr_t)s_f0,
                                        WSTR("HKEY_CLASSES_ROOT")),
                     iVar11 == 0)) {
                    root_key = (HKEY)(uintptr_t)0xffffffff80000000ULL;
                }
                else {
                    iVar11 = lstrcmpiW((LPCWSTR)(uintptr_t)s_f0, WSTR("HKU"));
                    if ((iVar11 == 0) ||
                        (iVar11 = lstrcmpiW((LPCWSTR)(uintptr_t)s_f0,
                                            WSTR("HKEY_USERS")),
                         iVar11 == 0)) {
                        root_key = (HKEY)(uintptr_t)0xffffffff80000003ULL;
                    }
                    else {
                        iVar11 = lstrcmpiW((LPCWSTR)(uintptr_t)s_f0, WSTR("HKCC"));
                        if ((iVar11 == 0) ||
                            (iVar11 = lstrcmpiW((LPCWSTR)(uintptr_t)s_f0,
                                                WSTR("HKEY_CURRENT_CONFIG")),
                             iVar11 == 0)) {
                            root_key = (HKEY)(uintptr_t)0xffffffff80000005ULL;
                        }
                        else {
                            uint64_t numv = 0;
                            WCHAR *rp = (WCHAR *)(uintptr_t)root_slot;
                            bVar7 = PECMD_ParseNumSkipWs(&rp, &numv);
                            root_slot = (int64_t)(intptr_t)rp;
                            if ((!bVar7) || (numv == 0)) {
                                PECMD_FreeStrBuf((WCHAR **)&s_a8);
                                goto LAB_1400c1dd0;
                            }
                            root_key = (HKEY)(uintptr_t)numv;
                        }
                    }
                }
            }
        }
        root_slot = (int64_t)(intptr_t)root_key;
        if (*(WCHAR *)(uintptr_t)s_108 == L'\0') {
            if (valcur == NULL) {
                /* ---- 删除 / 仅建键路径 ---- */
                if ((delim_keep == '!') && (*valname == L'\0')) {
                    if (cnt_b0 == 0) {
                        DVar13 = PECMD_DeleteRegistryKeyRecursive(
                            root_key, (LPCWSTR)(intptr_t)s_78);
                    }
                    else {
                        DVar13 = PECMD_RegScrubKey(
                            root_key, (LPCWSTR)(intptr_t)s_78, cnt_b0);
                    }
                }
                else if (cnt_b0 == 0) {
                    DVar13 = PECMD_RegDeleteValue(
                        root_key, (LPCWSTR)(intptr_t)s_78,
                        (LPCWSTR)(uintptr_t)valname);
                }
                else {
                    HKEY hk = (HKEY)0;
                    DVar13 = (DWORD)RegCreateKeyExW(
                        root_key, (LPCWSTR)(intptr_t)s_78, 0, (LPWSTR)0, 0,
                        0x2001b, (LPSECURITY_ATTRIBUTES)0, &hk, (LPDWORD)0);
                    if (hk != (HKEY)0) {
                        RegCloseKey(hk);
                    }
                }
                if (DVar13 == 2) {
                    DVar13 = 0;                  /* ERROR_FILE_NOT_FOUND 视为成功 */
                }
                goto LAB_1400c3517;
            }
            /* ================= 写值路径 ================= */
            script = 0;                          /* local_res8 复用为字节缓冲槽 */
            DVar24 = 8;
            s_120 = 0;
            datap = (BYTE *)(intptr_t)&s_120;    /* 缺省数据 = 数值槽本体 */
            wtype = 0;
            if (('\0' < v_cnt) || (k_cnt != '\0')) {
                LVar12 = PECMD_RegCreateKeyRetryWow64(
                    root_key, (LPCWSTR)(intptr_t)s_78, (int)v_cnt);
                if (('\x01' < v_cnt) || (k_cnt != '\0')) {
                    PECMD_FreeStrBuf((WCHAR **)&script);
                    PECMD_FreeStrBuf((WCHAR **)&s_a8);
                    PECMD_FreeStrBuf((WCHAR **)&s_b8);
                    PECMD_FreeStrBuf((WCHAR **)&s_e0);
                    PECMD_FreeStrBuf((WCHAR **)&s_c8);
                    PECMD_FreeStrBuf((WCHAR **)&s_108);
                    PECMD_FreeStrBuf((WCHAR **)&s_c0);
                    PECMD_FreeStrBuf((WCHAR **)&s_f0);
                    PECMD_FreeStrBuf((WCHAR **)&s_98);
                    PECMD_FreeStrBuf((WCHAR **)&s_70);
                    return (HKEY)(longlong)LVar12;
                }
            }
            if (prefix == 0x2b) {
LAB_1400c2ef2:
                /* '+': REG_QWORD */
                wtype = 0xb;
                want_type = 0xb;
                if (bVar4) {
                    PECMD_ParseHexByteList((LPCWSTR)(uintptr_t)valcur,
                                           &script, (int)opt16);
                    datap = (BYTE *)(uintptr_t)script;
                    goto LAB_1400c34b3;
                }
                DVar24 = 8;
                if (opt16 != '\0') {
                    if (((((uint16_t)*valcur < 0x30) ||
                          (0x39 < (uint16_t)*valcur)) &&
                         (((uVar9 = (ushort)(*valcur | 0x20)), (uVar9 < 0x61) ||
                           (0x66 < uVar9)))))
                        goto LAB_1400c30bf;
                    if ((*(short *)(uintptr_t)valcur == 0x30) &&
                        ((valcur[1] | 0x20) == 0x78)) {
                        valcur = valcur + 1;
                    }
                    s_120 = (int64_t)PECMD_ParseHex_4a34((uint16_t *)valcur);
                    goto LAB_1400c34b3;
                }
                {
                    uint64_t ev = 0;
                    uVar20 = PECMD_EvalParenStripped((WCHAR **)&s_f8, &ev);
                    s_120 = (int64_t)(uintptr_t)ev;
                }
                if (0 < (int)uVar20) goto LAB_1400c34b3;
                goto LAB_1400c33b0;
            }
            if (prefix != 0) {
                if (prefix == 0x23) {
LAB_1400c2f3b:
                    /* '#': REG_DWORD */
                    if (!bVar4) {
                        if (opt16 == '\0') {
                            int iv = 0;
                            bVar7 = PECMD_ParseUIntValue((WCHAR **)&s_f8, &iv);
                            if (bVar7) {
                                s_120 = (int64_t)(uint)iv;
                                DVar24 = 4;
                                wtype = 4;
                                goto LAB_1400c34b3;
                            }
                            goto LAB_1400c33b0;
                        }
                        if (((0x2f < (uint16_t)*valcur) &&
                             ((uint16_t)*valcur < 0x3a)) ||
                            (((uVar9 = (ushort)(*valcur | 0x20)),
                              (0x60 < uVar9) && (uVar9 < 0x67)))) {
                            if ((*(short *)(uintptr_t)valcur == 0x30) &&
                                ((valcur[1] | 0x20) == 0x78)) {
                                valcur = valcur + 1;
                            }
                            s_120 = (int64_t)(int)(uint)
                                PECMD_ParseHex_4a34((uint16_t *)valcur);
                            DVar24 = 4;
                            wtype = 4;
                            goto LAB_1400c34b3;
                        }
                        goto LAB_1400c30bf;
                    }
                    PECMD_ParseHexByteList((LPCWSTR)(uintptr_t)valcur,
                                           &script, (int)opt16);
                    datap = (BYTE *)(uintptr_t)script;
                    DVar24 = 4;
                    wtype = 4;
                    goto LAB_1400c34b3;
                }
                if (prefix != 0x62) {
                    if (prefix != 0x2a) {
                        if (prefix == 0x7e) goto LAB_1400c2fc5;
                        if (prefix == 0x40) goto LAB_1400c303e;
                        if (prefix != 0x6e) goto LAB_1400c3175;
                        wtype = 0;               /* 'n': REG_NONE */
                        want_type = 0;
                        goto LAB_1400c2f9a;
                    }
LAB_1400c2f81:
                    /* '*': REG_MULTI_SZ */
                    want_type = 7;
                    wtype = 7;
                    if (bVar4) goto LAB_1400c2f9a;
                    cp_len = lstrlenW(valcur);
                    PECMD_AllocString((WCHAR **)&script, (int64_t)(cp_len + 0x10));
                    cp_end = valcur + cp_len;
                    s_f8 = script;
                    cp_sep = 0;
                    cp_dst = (WCHAR *)(uintptr_t)script;
                    if (valcur < cp_end) goto LAB_1400c324c;
                    goto b7c_finish_pad;
                }
                /* 'b': REG_DWORD_BIG_ENDIAN */
                wtype = 5;
                if (bVar4) {
                    PECMD_ParseHexByteList((LPCWSTR)(uintptr_t)valcur,
                                           &script, (int)opt16);
                    datap = (BYTE *)(uintptr_t)script;
                }
                else {
                    if (opt16 == '\0') {
                        int iv = 0;
                        bVar7 = PECMD_ParseUIntValue((WCHAR **)&s_f8, &iv);
                        if (!bVar7) goto LAB_1400c33b0;
                        s_120 = (int64_t)(uint)iv;
                    }
                    else {
                        if (((((uint16_t)*valcur < 0x30) ||
                              (0x39 < (uint16_t)*valcur)) &&
                             (((uVar9 = (ushort)(*valcur | 0x20)),
                               (uVar9 < 0x61) || (0x66 < uVar9)))))
                            goto LAB_1400c30bf;
                        if ((*(short *)(uintptr_t)valcur == 0x30) &&
                            ((valcur[1] | 0x20) == 0x78)) {
                            valcur = valcur + 1;
                        }
                        s_120 = (int64_t)(int)(uint)
                            PECMD_ParseHex_4a34((uint16_t *)valcur);
                    }
                    {   /* 低 32 位字节序翻转 (CONCAT 序列直移) */
                        uint32_t w32 = (uint32_t)(uint)s_120;
                        w32 = ((w32 & 0xffu) << 24) | ((w32 & 0xff00u) << 8) |
                              ((w32 >> 8) & 0xff00u) | ((w32 >> 24) & 0xffu);
                        s_120 = (int64_t)(uint)w32;
                    }
                }
                DVar24 = 4;
                goto LAB_1400c34b3;
            }
            /* 前缀缺省: 值首字符内嵌类型形式 */
            if (*(short *)(uintptr_t)valcur == 0x2b) {
                valcur = valcur + 1;
                s_f8 = (int64_t)(intptr_t)valcur;
                goto LAB_1400c2ef2;
            }
            if (*(short *)(uintptr_t)valcur == 0x23) {
                valcur = valcur + 1;
                s_f8 = (int64_t)(intptr_t)valcur;
                goto LAB_1400c2f3b;
            }
            if (*(short *)(uintptr_t)valcur == 0x2a) {
                valcur = valcur + 1;
                goto LAB_1400c2f81;
            }
            if (*(short *)(uintptr_t)valcur == 0x7e) {
                valcur = valcur + 1;
                goto LAB_1400c2fc5;
            }
            if (*(short *)(uintptr_t)valcur == 0x40) {
                valcur = valcur + 1;
LAB_1400c303e:
                wtype = 3;                       /* '@': REG_BINARY */
                want_type = 3;
            }
            else {
LAB_1400c3175:
                wtype = want_type;
                if ((longlong)want_type < 0) {
                    if (prefix == 0x5e) {
                        wtype = 6;               /* '^': REG_LINK */
                        want_type = 6;
                    }
                    else if (prefix == 0x75) {
                        wtype = 0x15;            /* 'u': REG_MUI_SZ */
                        want_type = 0x15;
                    }
                    else {
                        if ((prefix != 0) && (prefix != 0x24))
                            goto LAB_1400c30bf;
                        wtype = 1;               /* 缺省 REG_SZ */
                        want_type = 1;
                    }
                    if (!bVar4) {
                        iVar11 = lstrlenW(valcur);
                        DVar24 = (DWORD)(iVar11 * 2);
                        datap = (BYTE *)(uintptr_t)valcur;
                        goto LAB_1400c34b3;
                    }
                }
            }
LAB_1400c2f9a:
            /* byte 模式公共入口: 字节列表解析 */
            uVar20 = PECMD_ParseHexByteList((LPCWSTR)(uintptr_t)valcur,
                                            &script, (int)opt16);
            DVar24 = (DWORD)uVar20;
            datap = (BYTE *)(uintptr_t)script;
            goto LAB_1400c34b3;

LAB_1400c2fc5:
            /* '~': REG_EXPAND_SZ — "\\0" 转义拷贝 */
            if (bVar4) goto LAB_1400c2f9a;
            iVar11 = lstrlenW(valcur);
            PECMD_AllocString((WCHAR **)&script, (int64_t)(iVar11 + 2));
            cp_dst = (WCHAR *)(uintptr_t)script;
            {
                WCHAR *sp2 = valcur;
                while (*sp2 != L'\0') {
                    if ((*sp2 == L'\\') && (sp2[1] == L'0')) {
                        *cp_dst = L'\0';
                        sp2 = sp2 + 1;
                    }
                    else {
                        *cp_dst = *sp2;
                        sp2 = sp2 + 1;
                    }
                    cp_dst = cp_dst + 1;
                }
                *cp_dst = L'\0';
            }
            DVar24 = (DWORD)((intptr_t)cp_dst - (intptr_t)script) + 2;
            wtype = 2;
            want_type = 2;
            goto LAB_1400c34b3;
        }
        /* ================= 查询路径 (输出变量名非空) ================= */
        if ((prefix == 0x2e) || (prefix == 0x3f)) {
            bVar8 = true;
            bVar4 = true;
            if (prefix != 0x3f) goto LAB_1400c2336;
            DVar24 = 1;                          /* '?': 仅类型名 */
        }
        else {
LAB_1400c2336:
            DVar24 = DVar13;
            if ((bVar8) && (*(WCHAR *)(uintptr_t)s_108 == L'\0')) {
                DVar24 = 1;
            }
        }
        flg_88[0] = DVar24;
        SetLastError(0);
        status_q = 0;
        PECMD_AllocWStringBuffer((WCHAR **)&s_110, 100);
        PECMD_AllocStrSlot((WCHAR **)&s_f8);
        outsel = (int64_t *)(intptr_t)&s_f8;     /* '.' 模式目标 = 第二槽 */
        if (prefix != 0x2e) {
            outsel = (int64_t *)(intptr_t)&s_110;
        }
        root_slot = (int64_t)(intptr_t)outsel;
        PECMD_AllocStringSlot2((void **)&s_120, 0x1000);
        data_size = 0xfff;
        out_off = 0;
        if (aflags != 0) {
            /* ---- 枚举分支 (a/av/ak): 列示值名与子键名 ---- */
            DWORD enum_len = 0;
            DWORD sub_cnt = 0;
            DWORD val_cnt = 0;
            DWORD dmx1 = 0;
            DWORD dmx2 = 0;
            HKEY ekey = (HKEY)0;
            WCHAR *enamebuf;
            WCHAR *elabel;
            WCHAR *enamep;
            DWORD eidx;
            RegOpenKeyExW(root_key, (LPCWSTR)(intptr_t)s_78, 0, 9, &ekey);
            RegQueryInfoKeyW(ekey, (LPWSTR)0, (LPDWORD)0, (LPDWORD)0,
                             &sub_cnt, &dmx1, (LPDWORD)0, (LPDWORD)0,
                             (LPDWORD)0, (LPDWORD)0, (LPDWORD)0, (FILETIME *)0);
            LVar12 = RegQueryInfoKeyW(ekey, (LPWSTR)0, (LPDWORD)0, (LPDWORD)0,
                             &sub_cnt, &dmx1, (LPDWORD)0, &val_cnt, &dmx2,
                             (LPDWORD)0, (LPDWORD)0, (FILETIME *)0);
            uVar22 = dmx1;
            if (dmx1 < dmx2) {
                uVar22 = dmx2;
            }
            iVar11 = (int)(uVar22 * 4 + 0x20);
            PECMD_AllocWStringBuffer((WCHAR **)&s_78,
                                     (int64_t)(((longlong)iVar11 >> 1) + 8));
            enamebuf = (WCHAR *)(uintptr_t)s_78;
            enamebuf[0] = L'\r';
            enamebuf[1] = L'\n';
            elabel = enamebuf + 2;               /* 'v '/'k ' 标签 */
            elabel[0] = L'v';
            elabel[1] = L' ';
            enamebuf[4] = L'\0';
            /* TODO(verify): 双枚举时名字写点位于标签后, 单枚举覆盖标签 */
            enamep = (aflags == 3) ? (elabel + 2) : elabel;
            if (LVar12 == 0) {
                if (((aflags & 1) != 0) && (0 < (int)val_cnt)) {
                    eidx = 0;
                    do {
                        enum_len = (DWORD)iVar11;
                        *enamep = L'\0';
                        LVar12 = RegEnumValueW(ekey, eidx, enamep, &enum_len,
                                               (LPDWORD)0, (LPDWORD)0,
                                               (BYTE *)0, (LPDWORD)0);
                        if (LVar12 == 0) {
                            PECMD_AppendWideStr((WCHAR **)&s_110,
                                                (LPCWSTR)(uintptr_t)elabel);
                        }
                        eidx = eidx + 1;
                    } while ((int)eidx < (int)val_cnt);
                }
                elabel[0] = L'k';
                if (((aflags & 2) != 0) && (0 < (int)sub_cnt)) {
                    eidx = 0;
                    do {
                        enum_len = (DWORD)iVar11;
                        *enamep = L'\0';
                        LVar12 = RegEnumKeyExW(ekey, eidx, enamep, &enum_len,
                                               (LPDWORD)0, (LPWSTR)0,
                                               (LPDWORD)0, (FILETIME *)0);
                        if (LVar12 == 0) {
                            PECMD_AppendWideStr((WCHAR **)&s_110,
                                                (LPCWSTR)(uintptr_t)elabel);
                        }
                        eidx = eidx + 1;
                    } while ((int)eidx < (int)sub_cnt);
                }
            }
            if (ekey != (HKEY)0) {
                RegCloseKey(ekey);
            }
            PECMD_FreeStrBuf((WCHAR **)&s_78);
            goto LAB_1400c2d88;
        }
        /* ---- 读值 FUN_1400690C0 ---- */
        {
            LONG rd_status = 0;
            uVar22 = (uint)FUN_1400690C0(root_key, (LPCWSTR)(intptr_t)s_78,
                                         (LPCWSTR)(uintptr_t)valname,
                                         &s_120, &data_size, &rd_status);
            rd_stat = rd_status;
        }
        if (rd_stat != 0) {
            if ((*valname == L'\0') && (rd_stat == 2) &&
                ((int)uVar22 == 0xfffffffe)) {
                if (!bVar8) goto LAB_1400c2d88;
                pcVar15 = "NI";
            }
            else {
                status_q = rd_stat;
                if (!bVar8) {
                    if (optsu == '\0') {
                        PECMD_FreeStrBuf((WCHAR **)&s_120);
                        PECMD_FreeStrBuf((WCHAR **)&s_f8);
                        PECMD_FreeStrBuf((WCHAR **)&s_110);
                        PECMD_FreeStrBuf((WCHAR **)&s_a8);
                        PECMD_FreeStrBuf((WCHAR **)&s_b8);
                        PECMD_FreeStrBuf((WCHAR **)&s_e0);
                        PECMD_FreeStrBuf((WCHAR **)&s_c8);
                        PECMD_FreeStrBuf((WCHAR **)&s_108);
                        PECMD_FreeStrBuf((WCHAR **)&s_c0);
                        PECMD_FreeStrBuf((WCHAR **)&s_f0);
                        PECMD_FreeStrBuf((WCHAR **)&s_98);
                        PECMD_FreeStrBuf((WCHAR **)&s_70);
                        return (HKEY)(longlong)(int)status_q;
                    }
                    goto LAB_1400c2d88;
                }
                pcVar15 = "ERROR";
            }
            PECMD_StrDupA((WCHAR **)outsel, pcVar15, ~(longlong)0, ~(longlong)0);
            goto LAB_1400c2d88;
        }
        /* 期望类型核对: DVar35=1 ⇔ 显式 t: 且不匹配 */
        if (((longlong)want_type < 0) ||
            ((longlong)want_type == (longlong)(int)uVar22)) {
            DVar35 = 0;
        }
        else {
            DVar35 = 1;
        }
        pcVar15 = "REG_SZ";
        bVar28 = 0xff;
        bVar21 = 0;
        if (uVar22 == 1) {
            bVar21 = 0x24;
            bVar28 = bVar26;
LAB_1400c27f6:
            if ((bVar8) &&
                (PECMD_StrDupA((WCHAR **)outsel, pcVar15,
                               ~(longlong)0, ~(longlong)0),
                DVar24 != 0)) goto LAB_1400c2d88;
            if ((DVar35 != 0) ||
                ((((longlong)want_type < 0) && (bVar21 != prefix)) &&
                 ((prefix != 0x2e) && (bVar28 != prefix)))) {
LAB_1400c2847:
                PECMD_FreeStrBuf((WCHAR **)&s_120);
                PECMD_FreeStrBuf((WCHAR **)&s_f8);
                PECMD_FreeStrBuf((WCHAR **)&s_110);
                PECMD_FreeStrBuf((WCHAR **)&s_a8);
                PECMD_FreeStrBuf((WCHAR **)&s_b8);
                PECMD_FreeStrBuf((WCHAR **)&s_e0);
                PECMD_FreeStrBuf((WCHAR **)&s_c8);
                PECMD_FreeStrBuf((WCHAR **)&s_108);
                PECMD_FreeStrBuf((WCHAR **)&s_c0);
                PECMD_FreeStrBuf((WCHAR **)&s_f0);
                PECMD_FreeStrBuf((WCHAR **)&s_98);
                PECMD_FreeStrBuf((WCHAR **)&s_70);
                return (HKEY)(uintptr_t)0xffffffff80070057ULL;
            }
        }
        else if (uVar22 == 6) {
            pcVar15 = "REG_LINK";
            bVar21 = 0x5e;
            goto LAB_1400c27f6;
        }
        else if (uVar22 == 0x15) {
            pcVar15 = "REG_MUI_SZ";
            bVar21 = 0x75;
            goto LAB_1400c27f6;
        }
        else if (uVar22 == 0xb) {
            if ((bVar8) &&
                (PECMD_StrDupA((WCHAR **)outsel, "REG_QWORD",
                               ~(longlong)0, ~(longlong)0), DVar24 != 0))
                goto LAB_1400c2d88;
            if ((DVar35 != 0) ||
                ((((longlong)want_type < 0) && (prefix != 0x2b)) &&
                 (prefix != 0x2e)))
                goto LAB_1400c2847;
            {
                LPCWSTR fmt = WSTR("0x%I64x");
                if (optr10 != '\0') {
                    fmt = WSTR("%I64u");
                }
                PECMD_SprintfRetEnd((WCHAR *)(uintptr_t)s_110,
                                    *(uint64_t *)(uintptr_t)s_120, fmt);
            }
            goto LAB_1400c2d88;
        }
        else if (uVar22 == 4) {
            if ((bVar8) &&
                (PECMD_StrDupA((WCHAR **)outsel, "REG_DWORD",
                               ~(longlong)0, ~(longlong)0), DVar24 != 0))
                goto LAB_1400c2d88;
            if ((DVar35 != 0) ||
                ((((longlong)want_type < 0) && (prefix != 0x23)) &&
                 (prefix != 0x2e)))
                goto LAB_1400c2847;
            {
                LPCWSTR fmt = WSTR("0x%lx");     /* TODO(verify): vararg 第 3 参还原 */
                if (optr10 != '\0') {
                    fmt = WSTR("%lu");
                }
                wsprintfW((LPWSTR)(uintptr_t)s_110, fmt,
                          *(DWORD *)(uintptr_t)s_120);
            }
            goto LAB_1400c2d88;
        }
        else if (uVar22 == 5) {
            if ((bVar8) &&
                (PECMD_StrDupA((WCHAR **)outsel, "REG_DWORD_BIG_ENDIAN",
                               ~(longlong)0, ~(longlong)0), DVar24 != 0))
                goto LAB_1400c2d88;
            if ((DVar35 != 0) ||
                ((((longlong)want_type < 0) && (prefix != 0x62)) &&
                 (prefix != 0x2e)))
                goto LAB_1400c2847;
            {   /* 4 字节翻转后按 DWORD 输出 */
                BYTE *bp = (BYTE *)(uintptr_t)s_120;
                BYTE tb;
                tb = bp[0]; bp[0] = bp[3]; bp[3] = tb;
                tb = bp[1]; bp[1] = bp[2]; bp[2] = tb;
            }
            {
                LPCWSTR fmt = WSTR("0x%lx");
                if (optr10 != '\0') {
                    fmt = WSTR("%lu");
                }
                wsprintfW((LPWSTR)(uintptr_t)s_110, fmt,
                          *(DWORD *)(uintptr_t)s_120);
            }
            goto LAB_1400c2d88;
        }
        else if (uVar22 == 2) {
            /* REG_EXPAND_SZ */
            if ((bVar8) &&
                (PECMD_StrDupA((WCHAR **)outsel, "REG_EXPAND_SZ",
                               ~(longlong)0, ~(longlong)0), DVar24 != 0))
                goto LAB_1400c2d88;
            if ((DVar35 != 0) ||
                ((((longlong)want_type < 0) && (prefix != 0x7e)) &&
                 (prefix != 0x2e)))
                goto LAB_1400c2847;
            if (mark2 != '\0') {
                PECMD_ExpandVarDispatch((void *)(uintptr_t)script,
                                        (WCHAR *)(uintptr_t)s_120,
                                        (WCHAR **)&s_110, 0, 1);
                goto LAB_1400c2d88;
            }
        }
        else if (uVar22 == 7) {
            /* REG_MULTI_SZ: NUL → "\\0"(或 CRLF) 还原拷贝 */
            if ((bVar8) &&
                (PECMD_StrDupA((WCHAR **)outsel, "REG_MULTI_SZ",
                               ~(longlong)0, ~(longlong)0), DVar24 != 0))
                goto LAB_1400c2d88;
            if ((DVar35 != 0) ||
                ((((longlong)want_type < 0) && (prefix != 0x2a)) &&
                 (prefix != 0x2e)))
                goto LAB_1400c2847;
            uVar31 = (ulonglong)(int)data_size;
            PECMD_AllocString((WCHAR **)&s_110,
                              (int64_t)((int)(uVar31 >> 1) * 4 + 6));
            cp_end = (WCHAR *)(uintptr_t)(s_120 +
                      (int64_t)((uint)(uVar31 >> 1)) * 2);
            cp_src = (WCHAR *)(uintptr_t)s_120;
            cp_dst = (WCHAR *)(uintptr_t)s_110;
            goto joined_r0x0001400c2b93;
        }
        else {
            if (uVar22 == 3) {
                pcVar15 = "REG_BINARY";
            }
            else if (uVar22 == 0) {
                pcVar15 = "REG_NONE";
            }
            else if (uVar22 == 8) {
                pcVar15 = "REG_RESOURCE_LIST";
            }
            else if (uVar22 == 9) {
                pcVar15 = "REG_FULL_RESOURCE_DESCRIPTOR";
            }
            else if (uVar22 == 10) {
                pcVar15 = "REG_RESOURCE_REQUIREMENTS_LIST";
            }
            else {
                pcVar15 = NULL;
            }
            if (pcVar15 != NULL) {
                if ((bVar8) &&
                    (PECMD_StrDupA((WCHAR **)outsel, pcVar15,
                                   ~(longlong)0, ~(longlong)0),
                    flg_88[0] != 0)) goto LAB_1400c2d88;
                bVar26 = 0x40;
            }
            else {
                if (bVar8) {
                    /* 未知类型: 以 "0x%lx" 输出类型号。TODO(verify): 变参还原 */
                    PECMD_AllocString((WCHAR **)outsel, 0x32);
                    wsprintfW(*(LPWSTR *)(intptr_t)outsel, WSTR("0x%lx"),
                              (uint)uVar22);
                    if (flg_88[0] != 0) goto LAB_1400c2d88;
                    bVar26 = 0;
                }
            }
            if (((((longlong)want_type < 0) && (bVar26 != prefix)) &&
                 (prefix != 0x2e)) || (DVar35 != 0))
                goto LAB_1400c2847;
            /* 二进制十六进制转储 */
            PECMD_AllocString((WCHAR **)&s_110, (int64_t)(data_size * 5 + 7));
            *(WCHAR *)(uintptr_t)s_110 = L'\0';
            ((WCHAR *)(uintptr_t)s_110)[1] = L'\0';
            out_off = 1;
            if (0 < (int)data_size) {
                LPCWSTR fmt = WSTR(" 0x%02X");
                WCHAR *op = (WCHAR *)(uintptr_t)s_110;
                int bi;
                if (optr10 != '\0') {
                    fmt = WSTR(" %u");
                }
                for (bi = 0; bi < (int)data_size; bi++) {
                    /* TODO(verify): 步进按 wsprintfW 返回宽度还原 */
                    op += wsprintfW(op, fmt,
                                    (uint)((BYTE *)(uintptr_t)s_120)[bi]);
                }
            }
            goto LAB_1400c2d88;
        }
        /* SZ/EXPAND_SZ 公共尾: 结果缓冲切换 */
        {
            int64_t tmpswap = s_110;
            s_110 = s_120;
            s_120 = tmpswap;
        }
        goto LAB_1400c2d88;

joined_r0x0001400c2b93:
        /* 查询侧 MULTI_SZ 还原循环 */
        if (cp_end <= cp_src) {
            *cp_dst = L'\0';
            goto LAB_1400c2d88;
        }
        if ((mark2 == '*') && (*cp_src == L'\\')) {
            WCHAR *nxt = cp_src + 1;
            if ((*nxt == L'0') || (*nxt == L'\\') || (*nxt == L'\0')) {
                cp_src = nxt;
                *cp_dst = L'\\';
                cp_dst[1] = *nxt;
                cp_dst = cp_dst + 1;
                goto joined_cont;
            }
        }
        {
            WVar10 = *cp_src;
            if (WVar10 == L'\0') {
                cp_src = cp_src + 1;
                if (mark2 != '$') {
                    *cp_dst = L'\\';
                    cp_dst[1] = L'0';
                    cp_dst = cp_dst + 1;
                    goto joined_cont;
                }
                if (cp_src < cp_end) {
                    *cp_dst = L'\r';
                    cp_dst[1] = L'\n';
                    cp_dst = cp_dst + 1;
                    goto joined_cont;
                }
                *cp_dst = L'\0';
                goto LAB_1400c2d88;
            }
            *cp_dst = WVar10;
            cp_src = cp_src + 1;
        }
joined_cont:
        cp_dst = cp_dst + 1;
        goto joined_r0x0001400c2b93;

/* ---------------- 写入循环与收尾 ---------------- */
LAB_1400c324c:
        /* '*'/'$'/无标记 写入拷贝循环体 */
        *cp_dst = L'\0';
        if (mark2 == '*') {
            if ((*cp_src == L'\\') && (cp_src[1] == L'\\')) {
                *cp_dst = L'\\';                 // 双反斜杠折叠为单
                cp_src = cp_src + 1;
            }
            else if ((*cp_src == L'\\') && (cp_src[1] == L'0')) {
                *cp_dst = L'\0';
                cp_src = cp_src + 1;
                goto b7c_c32c5;
            }
            else {
                *cp_dst = *cp_src;
            }
            cp_src = cp_src + 1;
        }
        else {
            if (mark2 != '$') {
                if ((*cp_src == L'\\') && (cp_src[1] == L'0')) {
                    *cp_dst = L'\0';
                    cp_src = cp_src + 1;
                    goto b7c_c32c5;
                }
                *cp_dst = *cp_src;
                cp_src = cp_src + 1;
            }
            else {
                WVar10 = *cp_src;
                if ((WVar10 == L'\r') && (cp_src[1] == L'\n')) {
                    cp_src = cp_src + 1;
                }
                else if (WVar10 != L'\n') {
                    if ((*cp_src == L'\\') && (cp_src[1] == L'0')) {
                        *cp_dst = L'\0';
                        cp_src = cp_src + 1;
                        goto b7c_c32c5;
                    }
                    *cp_dst = *cp_src;
                    cp_src = cp_src + 1;
                    goto b7c_copy_adv;
                }
                cp_src = cp_src + 1;
                *cp_dst = L'\0';
                goto b7c_c32c5;
            }
        }
b7c_c32c5:
        cp_sep = cp_sep + 1;
b7c_copy_adv:
        cp_dst = cp_dst + 1;
        if (cp_end <= cp_src) goto b7c_finish_pad;
        goto LAB_1400c324c;

b7c_finish_pad:                                  /* LAB_1400c32f3 */
        if (cp_sep < 1) {
            cp_sep = cp_sep + 1;
            *cp_dst = L'\0';
            cp_dst = cp_dst + 1;
        }
        {
            WCHAR *pend = cp_dst;
            if ((0 < cp_len) && (cp_sep < 2)) {
                int padn = 2 - cp_sep;
                pend = cp_dst + padn;
                while (padn-- > 0) {
                    *cp_dst = L'\0';
                    cp_dst = cp_dst + 1;
                }
            }
            *pend = L'\0';
            DVar24 = (DWORD)((intptr_t)pend - (intptr_t)script);
        }
        wtype = want_type;                       /* uVar31 = local_90 直移 */
LAB_1400c34b3:
        for (;;) {
            if (optqk == '\0') {
                PECMD_RegSetValueWithOpen(root_key,
                                          (LPCWSTR)(intptr_t)s_78,
                                          (LPCWSTR)(uintptr_t)valname,
                                          (DWORD)wtype, datap, DVar24);
                DVar13 = 0;                      /* 直移: 返回被丢弃 */
            }
            else {
                /* SHSetValueW 同参重构 (Ghidra 丢失变参)。TODO(verify) */
                DVar13 = (DWORD)(*g_pSHSetValueW)(
                    root_key, (LPCWSTR)(intptr_t)s_78,
                    (LPCWSTR)(uintptr_t)valname,
                    (DWORD)wtype, (const void *)datap, DVar24);
            }
            if ((DVar13 != 0x3fd) || (v_cnt != '\0')) break;
            v_cnt = '\x01';
            PECMD_RegCreateKeyRetryWow64(root_key, (LPCWSTR)(intptr_t)s_78, 1);
        }
        PECMD_FreeStrBuf((WCHAR **)&script);
LAB_1400c3517:
        PECMD_FreeStrBuf((WCHAR **)&s_a8);
        PECMD_FreeStrBuf((WCHAR **)&s_b8);
        PECMD_FreeStrBuf((WCHAR **)&s_e0);
        PECMD_FreeStrBuf((WCHAR **)&s_c8);
        PECMD_FreeStrBuf((WCHAR **)&s_108);
        PECMD_FreeStrBuf((WCHAR **)&s_c0);
        PECMD_FreeStrBuf((WCHAR **)&s_f0);
        PECMD_FreeStrBuf((WCHAR **)&s_98);
        PECMD_FreeStrBuf((WCHAR **)&s_70);
        return (HKEY)(longlong)(int)DVar13;

LAB_1400c1dd0:
        PECMD_FreeStrBuf((WCHAR **)&s_b8);
        goto LAB_1400c1d02;

LAB_1400c30bf:
        retv = (HKEY)(uintptr_t)0xffffffff80070057ULL;
LAB_1400c33b5:
        PECMD_FreeStrBuf((WCHAR **)&script);
        PECMD_FreeStrBuf((WCHAR **)&s_a8);
        PECMD_FreeStrBuf((WCHAR **)&s_b8);
        PECMD_FreeStrBuf((WCHAR **)&s_e0);
        PECMD_FreeStrBuf((WCHAR **)&s_c8);
        PECMD_FreeStrBuf((WCHAR **)&s_108);
        PECMD_FreeStrBuf((WCHAR **)&s_c0);
        PECMD_FreeStrBuf((WCHAR **)&s_f0);
        goto LAB_1400c1cd1;
LAB_1400c33b0:
        retv = (HKEY)1;
        goto LAB_1400c33b5;

LAB_1400c2d88:
        /* ---- 查询收尾: 写回环境变量 ---- */
        if (*(WCHAR *)(uintptr_t)s_108 != L'\0') {
            FUN_1400629B8((void *)(uintptr_t)script,
                          (LPCWSTR)(uintptr_t)s_108,
                          (LPCWSTR)(intptr_t)(s_110 + (int64_t)out_off * 2));
        }
        if (*(WCHAR *)(uintptr_t)s_e0 != L'\0') {
            FUN_1400629B8((void *)(uintptr_t)script,
                          (LPCWSTR)(uintptr_t)s_e0,
                          (LPCWSTR)*(int64_t *)outsel);
        }
        PECMD_FreeStrBuf((WCHAR **)&s_120);
        PECMD_FreeStrBuf((WCHAR **)&s_f8);
        PECMD_FreeStrBuf((WCHAR **)&s_110);
        PECMD_FreeStrBuf((WCHAR **)&s_a8);
        PECMD_FreeStrBuf((WCHAR **)&s_b8);
        PECMD_FreeStrBuf((WCHAR **)&s_e0);
        PECMD_FreeStrBuf((WCHAR **)&s_c8);
        PECMD_FreeStrBuf((WCHAR **)&s_108);
        PECMD_FreeStrBuf((WCHAR **)&s_c0);
        PECMD_FreeStrBuf((WCHAR **)&s_f0);
        PECMD_FreeStrBuf((WCHAR **)&s_98);
        PECMD_FreeStrBuf((WCHAR **)&s_70);
        return (HKEY)(longlong)(int)status_q;

LAB_1400c1c02:
        pass_cnt = (char)(pass_cnt + '\x01');
        cVar6 = pass_cnt;
        if ('\0' < cVar6) {
LAB_1400c1d02:
            PECMD_FreeStrBuf((WCHAR **)&s_e0);
            PECMD_FreeStrBuf((WCHAR **)&s_c8);
            PECMD_FreeStrBuf((WCHAR **)&s_108);
            PECMD_FreeStrBuf((WCHAR **)&s_c0);
            PECMD_FreeStrBuf((WCHAR **)&s_f0);
            retv = (HKEY)(uintptr_t)0xffffffff80070057ULL;
            goto LAB_1400c1cd1;
        }
        PECMD_ExpandVarDispatch((void *)(uintptr_t)script,
                                (WCHAR *)(uintptr_t)s_78,
                                (WCHAR **)&s_98, 0, 1);
        cursor = (WCHAR *)(uintptr_t)s_98;
        PECMD_FreeStrBuf((WCHAR **)&s_e0);
        PECMD_FreeStrBuf((WCHAR **)&s_c8);
        PECMD_FreeStrBuf((WCHAR **)&s_108);
        PECMD_FreeStrBuf((WCHAR **)&s_c0);
        PECMD_FreeStrBuf((WCHAR **)&s_f0);
        FUN_14005B154(&cursor);
        cVar6 = PECMD_MatchTokenAdvance("---initram", &cursor, 10);
      } while (cVar6 == '\0');
    }
    PECMD_InitRamdataRegistry(3);
LAB_1400c1cd1:
    PECMD_FreeStrBuf((WCHAR **)&s_98);
    PECMD_FreeStrBuf((WCHAR **)&s_70);
    return retv;
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

/* ---- SHOW 命令引擎 (PECMD_PartShowHideDrive @0x1400cd3a8) 新增依赖 ---- */
extern void     PECMD_FillCharTable(uint16_t start, uint64_t *table);       /* @0x1400603e8 候选盘符表 C..Z (core_b3d.c) */
extern uint64_t PECMD_GetSpecialDirFirstChar(void);                          /* @0x14006042c 系统目录首字符 (core_b3c.c) */
extern void     PECMD_ParseShortStore(void *pp, int *out, short sep);        /* @0x1400679b0 按分隔符解析整数 (link_stubs.c) */
extern void     FUN_1400F429C(WCHAR **pp, WCHAR ch);                         /* @0x1400f429c 游标前进至 ch (core_b8h.c) */
extern void     PECMD_ExpandDriveList(uint8_t *out, uint16_t *start, uint16_t *end); /* @0x14006aa9c 盘符区间展开 (core_b3l.c) */
extern uint32_t PECMD_ParseU64SkipSep(int64_t *pp, uint64_t *out);           /* @0x1400c453c 解析数+跳分隔符 (core_b7a.c) */
extern uint64_t PECMD_EvalParenthesizedExpr(int64_t *pp, uint64_t *out);     /* @0x1400c10c0 数值/#(表达式) (core_b7a.c 近似体) */
extern int64_t  PECMD_ParseVolumeGuid(int64_t *pp, uint32_t *out, int flag); /* @0x1400780fc 卷 GUID 解析 (core_b3_remaining.c) */
extern void     FUN_14001d78c(unsigned char *dst, const unsigned char *src, longlong n); /* @0x14001d78c memcpy 库替换 (link_stubs.c) */
extern void     FUN_140102a90(void *dst, uint64_t v, uint64_t n);            /* @0x140102a90 CRT memset (link_stubs.c) */
extern int      FUN_14005B184(char *a, int64_t b, int64_t n);                /* @0x14005b184 ANSI 定长比较 */
extern HANDLE   PECMD_OpenFileHandle(HANDLE *out, LPCWSTR path, DWORD access, DWORD share,
                                     LPSECURITY_ATTRIBUTES sa, DWORD disp, DWORD flags,
                                     HANDLE tmpl);                           /* @0x140003864 CreateFileW 包装 (core_exec2.c) */
extern uint64_t *PECMD_UpdatePartitionLayout(HANDLE h, uint64_t *info, uint32_t *count,
                                             uint8_t *outType, uint32_t flags); /* @0x14006abb8 (core_b3l.c) */
extern int      PECMD_QueryDeviceIoInfo(HANDLE h, int sz);                   /* @0x14005f96c 扇区大小 (core_b3r_d.c extern) */
extern uint64_t PECMD_ReadDiskSectorScan(uint64_t *a1, uint64_t *buf, uint32_t a3,
                                         DWORD sect, uint64_t a5, uint64_t *a6,
                                         LARGE_INTEGER a7);                  /* @0x140069868 (core_b3_remaining.c) */
extern int      PECMD_FindPartitionByGeometryV2(char *a1, int64_t a2, int a3, int a4, int a5); /* @0x140069704 */
extern uint32_t PECMD_GetDiskGeometry(LPCWSTR p, HANDLE h);                  /* @0x140065efc 文件系统类型 */
extern uint8_t  PECMD_SetDriveMount(int64_t tbl, uint32_t disk, uint32_t part, uint32_t flags,
                                    uint32_t mode, int unshow, uint16_t *path,
                                    uint32_t *err);                          /* @0x14005f9f0 卷映射删除/定义 */
extern BOOL     PECMD_DosDeviceMount(LPCWSTR dev, LPCWSTR letter, WCHAR *mount,
                                     uint32_t flags, char unshow);           /* @0x140075f9c (core_b3_remaining.c) */
extern int      PECMD_FindVolumeByDeviceId(uint32_t *devid, int64_t *out, LPWSTR flag); /* @0x14008b820 */
extern int      PECMD_PickFreeDriveLetter(uint32_t *bitmap, int16_t start, char mode,
                                          char *exclude);                    /* @0x14005f868 (core_b3j.c) */
extern void     PECMD_TlsLogWrite(uint64_t ctx, LPCWSTR fmt, uint64_t a, uint64_t b); /* @0x140018d8c 日志 (link_stubs.c, 固定4参截断) */
extern void     PECMD_SendHotkeyKeyMessage(uint32_t w, int a, int b);        /* @0x140035b40 (core_b3_remaining.c extern) */
extern void     PECMD_MarkKeyTable(uint16_t key, int64_t table);             /* @0x140060244 表内标记盘符 (core_b3e.c) */
extern void     PECMD_RemoveFirstMatchChar(uint16_t ch, char *s);            /* @0x140060290 候选表移除 (core_b3f.c) */
extern uint16_t PECMD_NextTokenChar(uint16_t a1, int64_t a2, int64_t a3, int16_t a4,
                                    int64_t *a5);                            /* @0x1400602f0 选下一可用盘符 */
extern void     PECMD_DeleteDriveMountPoint(uint16_t ch);                    /* @0x14005f7ec 卷更新通知 */
extern int      PECMD_QueryDiskGeometry(HANDLE h, uint64_t *buf, int a, int b); /* @0x140069a20 */
extern int64_t  PECMD_FindPartitionStartSector(HANDLE h, int *a, int64_t *b);/* @0x140078514 */

/* SHOW 命令尾部 GPT 类型过滤条目用的 .rdata GUID 真值
 * (tools/pe_data_extract.py ../PECMD.exe 提取):
 *   DAT_14012a258 PARTITION_BASIC_DATA_GUID   {deb94ba4-06d1-4d40-a16a-bfd50179d6ac}
 *   DAT_14012a268 PARTITION_MSFT_RESERVED_GUID{e3c9e316-0b5c-4db8-817d-f92df00215ae}
 *   DAT_14012a278 PARTITION_SYSTEM_GUID(EFI)  {c12a7328-f81f-11d2-ba4b-00a0c93ec93b} */
static const byte b7c_GuidBasicData[16] = {
    0xa4,0xbb,0x94,0xde,0xd1,0x06,0x40,0x4d,0xa1,0x6a,0xbf,0xd5,0x01,0x79,0xd6,0xac };
static const byte b7c_GuidMsftReserved[16] = {
    0x16,0xe3,0xc9,0xe3,0x5c,0x0b,0xb8,0x4d,0x81,0x7d,0xf9,0x2d,0xf0,0x02,0x15,0xae };
static const byte b7c_GuidEspSystem[16] = {
    0x28,0x73,0x2a,0xc1,0x1f,0xf8,0xd2,0x11,0xba,0x4b,0x00,0xa0,0xc9,0x3e,0xc9,0x3b };

/* ========== PECMD_PartShowHideDrive @0x1400cd3a8 ==========
 * 'SHOW' 命令引擎 (decompiled.c @126546 size=12480 忠实直移)。
 * 子结构:
 *   SEG1  入口/前缀(#1..#4)解析与串槽初始化 (@126700)
 *   SEG2  选项循环 =1/-cdrom/-BD/-force/-from:/-SKIP=/-skiptp:/-skippt:/-check/-exist (@126776)
 *   SEG3  主字段扫描 '*'/'&'/'-' 与 F/U/R/:/# 字段拆解 (@126910)
 *   SEG4  分区区间换算 + 候选盘符表准备 (@127077)
 *   SEG5  模式旗标收敛 / 快照缓冲 / GPT 过滤条目注入 (@127172)
 *   SEG6  物理盘枚举主循环: 打开 \\.\PhysicalDrive%d → 读分区表 →
 *         内层重试循环(26×0x220 驱动器表匹配 → Show/Unshow 应用:
 *         DefineDosDeviceW/DeleteVolumeMountPointW/PECMD_SetDriveMount) (@127232)
 *   SEG7  预算耗尽阶段升级 (#1..#5 态迁移重扫) (@128038)
 *   SEG8  终局直接 DefineDosDeviceW 应用与清理退出 (@128098)
 * 返回值: 原 binary 为路径相关的 unaff_RBX(Ghidra 未定), 保守取 0 /
 *         直接 DDD 路径取 0|GetLastError() — TODO(verify)。
 */
int64_t PECMD_PartShowHideDrive(uint64_t a1, WCHAR *a2)
{
    LPWSTR pWVar1;
    char cVar2;
    char cVar4;
    WCHAR WVar5;
    WCHAR WVar6;
    int iVar7;
    uint uVar8;
    int iVar9;
    int iVar10;
    DWORD DVar11;
    uint uVar12;
    BOOL BVar13;
    longlong lVar17;
    LPWSTR pWVar19;
    HANDLE hFindFile;
    ulonglong *puVar20;
    short sVar21;
    ushort uVar22;
    char *pcVar23;
    LPWSTR pWVar25;
    ulonglong *puVar26;
    WCHAR *pWVar27;
    byte bVar28;
    uint uVar29;
    ulonglong uVar30;
    ushort uVar31;
    WCHAR *pWVar32;
    byte *pbVar33;
    ulonglong uVar34;
    WCHAR WVar35;
    longlong lVar36;
    short sVar38;
    uint uVar39;
    short sVar40;
    bool bVar41;
    WCHAR *cur;                          /* decompiled local_res10[0] 游标 */
    byte res20[8];                       /* decompiled local_res20 设备类型槽 */
    WIN32_FIND_DATAW local_288;
    char local_998;                      /* '#2' 前缀旗标 */
    char local_988;
    byte local_990;
    uint64_t uVar15;
    uint64_t uVar16;
    WCHAR *local_958;
    char local_940;
    LPWSTR local_918;
    byte *local_8c0;
    ulonglong *local_8b8;
    int64_t local_890_a;
    int64_t local_890_b;
    byte local_997;                      /* '#' 变体旗标 */
    char local_996;                      /* '#4'/'#1' 前缀旗标 */
    byte local_995;                      /* 默认盘符 ('U' when #3) */
    char local_986;                      /* 当前分区 GPT 旗标 */
    char local_987;                      /* 首字符 '*' 旗标 */
    byte local_985;                      /* -BD / '**' 位 0x10 */
    byte local_98f;                      /* -skiptp 条目计数 */
    uint local_98c;                      /* '-' 移除模式 */
    char local_974;                      /* 允许自动选符旗标 */
    char local_97c;                      /* 允许占用检查旗标 */
    byte local_97b;                      /* 目标盘符字节 (默认 'C') */
    byte local_97a;                      /* GPT|0x10 复合旗标 */
    char local_979;                      /* '=1' 旗标 */
    uint local_978;                      /* 当前探测盘号 */
    uint local_968;                      /* 盘号 (-1 未定) */
    int64_t local_890;                   /* 数值解析槽 (-1 初值) */
    uint local_888;                      /* -cdrom 旗标 */
    uint local_884;                      /* -check/-exist 位标志 */
    int local_87c;                       /* 几何匹配序号 */
    uint local_878;                      /* 'X' 字段盘符/星号槽 */
    WCHAR local_874;                     /* 系统目录首字符 */
    WCHAR *local_870;                    /* scratch 游标 (大缓冲尾) */
    ushort local_880;                    /* 快照表命中的字母 */
    int local_860;                       /* 已分配计数 */
    byte local_858[40];                  /* 候选盘符表 B (&local_858 区, 27B 有效) */
    uint64_t cand_tbl[40];               /* &local_818 候选盘符表 A (27B 有效) */
    uint local_81c;                      /* EnumDrivesToTable 模式 (1/9) */
    char local_92e;                      /* 快照已建立旗标 */
    char local_94c;                      /* 精确分区命中旗标 */
    char local_93f;                      /* 目标存在旗标 */
    int local_93c;                       /* 目标盘符 int (默认 'C') */
    uint local_938;                      /* 状态/错误码 */
    ushort local_930;                    /* -skippt 对计数 */
    uint local_920;                      /* 当前应用字母 (uint) */
    DWORD local_908;                     /* 上次错误 */
    short local_904;                     /* 当前盘号 (short) */
    WCHAR *local_900;                    /* 5 wchar 槽: 盘符根 "X:\" */
    uint local_8f8;                      /* 当前分区号 */
    uint local_8f4;                      /* 位标志收敛槽 */
    uint local_8f0;                      /* GetLogicalDrives 掩码 */
    char local_8ec;                      /* '#22' 前缀旗标 */
    byte local_8eb;                      /* uVar12&0x10 */
    LPWSTR local_8e8;                    /* 驱动器表基址指针 */
    byte local_8e0;                      /* uVar12&0x40 */
    uint local_8dc;                      /* 阶段升级计数 */
    uint local_8d8;                      /* CDROM 探测旗标 */
    WCHAR *local_8d0;                    /* 第一字段串槽 */
    int local_8b0;                       /* 重试预算 (1000) */
    uint local_8ac;                      /* -SKIP= 解析值 */
    uint local_8a8;                      /* 分区属性副本 */
    WCHAR *local_8a0;                    /* Y 字段游标/条目基址复用 */
    char *local_898;                     /* 候选表游标 */
    uint64_t local_838[2];               /* FindFirstFileW "X:\NUL" 图样 */
    uint local_820;                      /* (> 'Z') 旗标 */
    longlong local_7f8;                  /* 起始扇区查询 B */
    longlong local_7e8;                  /* (int)local_948 复用槽 */
    longlong local_7e0;                  /* 起始扇区查询 A */
    char *local_7f0;                     /* 候选游标暂存 */
    byte pair_area[240];                 /* local_7d8 头 + local_7d6 对区共享:
                                            pair i = area[4i..4i+4) (disk,part) */
    byte local_6e8[1120];                /* -skiptp 列表: 步 0x11 [valid][GUID16] */
    int local_950;                       /* 任一 # 前缀旗标 */
    int local_91c;                       /* 同 local_950 (后半段名) */
    int local_90c;                       /* F/U/R 模式 (-1=F,1=U/R) */
    char local_964;                      /* U/R 旗标 */
    char local_96c;                      /* '#3' 前缀旗标 */
    uint local_944;                      /* DefineDosDevice 标志位 */
    uint local_948;                      /* 目标分区换算结果 */
    uint local_970;                      /* 主分区目标 */
    uint local_984;                      /* 分区目标换算 */
    uint local_934;                      /* 分区计数 */
    uint local_980;                      /* 盘号循环计数 */
    uint local_910;                      /* 计数暂存 */
    uint local_994;                      /* 内层跳步计数 */
    ulonglong pv18;                      /* decompiled puVar18 字节旗标扩展 */
    HANDLE hDev;                         /* pWVar27 设备句柄复用 */
    LPWSTR tb;                           /* 驱动器表基址 (local_918+0x20C010 字节) */
    LPWSTR sc;                           /* 大缓冲尾部 scratch (+0x20F970 字节) */
    byte *info;                          /* 分区信息数组基址 (local_8b8) */
    byte *ent;                           /* 分区信息条目 (i*0x90) */
    longlong rbx_ret;                    /* unaff_RBX 保守承载 */
    uint64_t uVar14;                     /* '#4' 匹配原始返回 */
    WCHAR letter_str[3];                 /* &local_960 区 "X:" */
    WCHAR mount_root[4];                 /* &local_928 区 "X:\" */
    WCHAR final_letter[3];               /* &local_868 区 "X:" */
    LARGE_INTEGER liZero;
    WCHAR local_8c8;                     /* 表内命中的旧盘符 */

    (void)a1;
    liZero.QuadPart = 0;
    rbx_ret = 0;
    info = NULL;

    /* ================= SEG1: 入口与前缀解析 (@126700) ================= */
    cur = a2;
    EnterCriticalSection(&g_csDisk);
    local_987 = (*cur == L'*');
    uVar12 = 0;
    local_996 = '\0';
    if ((char)local_987) {
        cur = cur + 1;
    }
    pWVar27 = cur;
    uVar14 = (uint64_t)(uint32_t)PECMD_AsciiPrefixICmp("#4", (const uint16_t *)cur, 2);
    cVar4 = '\x01';
    if (((char)uVar14 != '\0') ||
        (uVar15 = (uint64_t)(uint32_t)PECMD_AsciiPrefixICmp("#1", (const uint16_t *)pWVar27, 2),
         (char)uVar15 != '\0')) {
        local_996 = '\x01';
    }
    uVar15 = (uint64_t)(uint32_t)PECMD_AsciiPrefixICmp("#2", (const uint16_t *)pWVar27, 2);
    local_998 = (char)uVar15;
    uVar15 = (uint64_t)(uint32_t)PECMD_AsciiPrefixICmp("#22", (const uint16_t *)pWVar27, 3);
    local_8ec = (char)uVar15;
    uVar16 = (uint64_t)(uint32_t)PECMD_AsciiPrefixICmp("#3", (const uint16_t *)pWVar27, 2);
    cVar2 = (char)uVar16;
    local_950 = 0;
    if ((((local_996 != '\0') || (local_998 != '\0')) || ((char)local_8ec != '\0')) ||
        (cVar2 != '\0')) {
        local_950 = 1;
    }
    while (true) {
        WVar6 = *pWVar27;
        if (((WVar6 == L'\0') || ((8 < (ushort)WVar6 && ((ushort)WVar6 < 0xe)))) ||
            (WVar6 == L' ')) {
            break;
        }
        pWVar27 = pWVar27 + 1;
        cur = pWVar27;
    }
    local_96c = cVar2;
    local_91c = local_950;
    FUN_14005B154(&cur);
    PECMD_AllocWStringBuffer(&local_900, 5);
    PECMD_AllocWStringBuffer(&local_958, 0x14);
    PECMD_AllocStrSlot(&local_8d0);
    local_968 = 0xffffffffU;
    local_890 = -1;
    bVar28 = 0;
    res20[0] = 0;
    local_997 = 0;
    local_964 = '\0';
    local_940 = '\0';
    local_930 = 0;
    local_98f = 0;
    FUN_140102a90(cand_tbl, 0, sizeof(cand_tbl));   /* local_818 区清零 */
    FUN_140102a90(local_858, 0, sizeof(local_858)); /* local_858 区清零 */
    local_8f4 = 0;
    local_995 = (byte)((uint)(-(cVar2 != '\0')) & 0x55);
    local_8dc = 0;
    local_944 = 0;
    local_98c = 0;
    local_898 = (char *)cand_tbl;
    local_8b0 = 1000;
    local_888 = 0;
    local_979 = '\0';
    local_984 = 0;
    local_938 = 0;
    local_93c = 0x43;
    local_97b = 0x43;
    PECMD_FillCharTable(0x43, cand_tbl);
    uVar34 = 0;
    local_884 = 0;                       /* 原体读未初始化高位后掩码; 置 0 TODO(verify) */
    bVar41 = false;
    local_985 = 0;
    uVar22 = 0;

    /* ================= SEG2: 选项循环 (@126776) ================= */
LAB_1400cd654:
    do {
        while (true) {
            while (true) {
                while (true) {
                    while (true) {
                        while (true) {
                            WVar6 = (WCHAR)uVar34;
                            if ((*cur != L'-') && (*cur != L'=')) goto LAB_1400cdb32;
                            cVar2 = PECMD_MatchTokenAdvance("=1", &cur, 2);
                            uVar34 = 0;
                            if (cVar2 == '\0') break;
                            local_979 = '\x01';
                        }
                        cVar2 = PECMD_MatchTokenAdvance("-cdrom", &cur, 6);
                        uVar34 = 0;
                        if (cVar2 == '\0') break;
                        local_888 = 1;
                    }
                    cVar2 = PECMD_MatchTokenAdvance("-BD", &cur, 3);
                    uVar34 = 0;
                    if (cVar2 == '\0') break;
                    local_985 = 0x10;
                }
                cVar2 = PECMD_MatchTokenAdvance("-force", &cur, 6);
                uVar34 = 0;
                if (cVar2 == '\0') break;
                bVar41 = true;
            }
            pWVar27 = cur;
            uVar15 = (uint64_t)(uint32_t)PECMD_AsciiPrefixICmp("-from:", (const uint16_t *)cur, 6);
            uVar34 = 0;
            if ((char)uVar15 == '\0') break;
            pWVar32 = pWVar27 + 6;
            uVar31 = *pWVar32 & 0xffdf;
            WVar6 = *pWVar27;
            while ((WVar6 != L'\0' &&
                    ((((ushort)WVar6 < 9 || (0xd < (ushort)WVar6)) && (WVar6 != L' '))))) {
                pWVar27 = pWVar27 + 1;
                cur = pWVar27;
                WVar6 = *pWVar27;
            }
            FUN_14005B154(&cur);
            if ((ushort)(uVar31 - 0x41) < 0x1a) {
                local_93c = (int)(((uint)local_93c & 0xffff0000U) | uVar31);
                PECMD_ExpandDriveList((uint8_t *)cand_tbl, pWVar32, cur);
LAB_1400cd843:
                uVar34 = 0;
            }
        }
        uVar15 = (uint64_t)(uint32_t)PECMD_AsciiPrefixICmp("-SKIP=", (const uint16_t *)pWVar27, 6);
        if ((char)uVar15 != '\0') {
            cur = pWVar27 + 6;
            FUN_14005B154(&cur);
            local_8ac = 0x42;
            PECMD_ParseUIntValue(&cur, (int *)&local_8ac);
            local_940 = (char)local_8ac;
            FUN_14005B154(&cur);
            goto LAB_1400cd843;
        }
        uVar15 = (uint64_t)(uint32_t)PECMD_AsciiPrefixICmp("-skiptp:", (const uint16_t *)pWVar27, 8);
        uVar34 = 0;
        if ((char)uVar15 != '\0') {
            cur = pWVar27 + 8;
            FUN_14005B154(&cur);
            if ((short)uVar22 < 0x3c) {
                do {
                    pWVar27 = cur;
                    uVar30 = (ulonglong)bVar28;
                    local_838[0] = 0;
                    local_838[1] = uVar34;
                    lVar17 = PECMD_ParseVolumeGuid((int64_t *)&cur, (uint32_t *)local_838, 1);
                    local_6e8[uVar30 * 0x11] = (byte)(-1 < lVar17);
                    if (-1 >= lVar17) {
                        cur = pWVar27;
                        uVar15 = PECMD_EvalParenthesizedExpr((int64_t *)&cur,
                                                             (uint64_t *)local_838); /* TODO(verify): 近似体 */
                        uVar34 = 0;
                        bVar28 = local_98f;
                        if ((int)uVar15 < 1) break;
                    }
                    FUN_14001d78c(local_6e8 + uVar30 * 0x11 + 1,
                                  (const unsigned char *)local_838, 0x10);
                    uVar34 = 0;
                    bVar28 = local_98f + 1;
                    local_98f = bVar28;
                    if ((*cur == L'\0') ||
                        (((8 < (ushort)*cur && ((ushort)*cur < 0xe)) || (*cur == L' ')))) break;
                    cur = cur + 1;
                } while (true);
            }
            for (; ((WVar6 = *cur, WVar6 != (WCHAR)uVar34 &&
                     (((ushort)WVar6 < 9 || (0xd < (ushort)WVar6)))) && (WVar6 != L' '));
                 cur = cur + 1) {
            }
            FUN_14005B154(&cur);
            uVar22 = local_930;
            goto LAB_1400cd654;
        }
        uVar15 = (uint64_t)(uint32_t)PECMD_AsciiPrefixICmp("-skippt:", (const uint16_t *)pWVar27, 8);
        if ((char)uVar15 != '\0') {
            cur = pWVar27 + 8;
            local_890_a = -1;                /* local_8a0 复用数值槽 */
            local_890_b = -1;                /* local_8e8 复用数值槽 */
            uVar31 = local_930;
            while ((((short)uVar22 < 0x3c &&
                     (iVar7 = (int)PECMD_ParseU64SkipSep((int64_t *)&cur,
                                                         (uint64_t *)&local_890_a),
                      uVar31 = uVar22, 0 < iVar7)) &&
                    (iVar7 = (int)PECMD_ParseU64SkipSep((int64_t *)&cur,
                                                        (uint64_t *)&local_890_b),
                     0 < iVar7))) {
                uVar31 = uVar22 + 1;
                /* 对 i = (disk,part) 各占 u16, 区基 &pair_area[0](=原 local_7d8) */
                *(uint16_t *)&pair_area[(longlong)(short)uVar22 * 4] =
                    (uint16_t)(ulonglong)local_890_a;
                *(uint16_t *)&pair_area[(longlong)(short)uVar22 * 4 + 2] =
                    (uint16_t)(ulonglong)local_890_b;
                uVar22 = uVar31;
            }
            local_930 = uVar31;
            uVar34 = 0;
            WVar6 = *cur;
            while ((WVar6 != L'\0' &&
                    ((((ushort)WVar6 < 9 || (0xd < (ushort)WVar6)) && (WVar6 != L' '))))) {
                cur = cur + 1;
                WVar6 = *cur;
            }
            FUN_14005B154(&cur);
            goto LAB_1400cd654;
        }
        cVar2 = PECMD_MatchTokenAdvance("-check", &cur, 6);
        uVar34 = 0;
        if (cVar2 != '\0') {
            local_884 = ((uint)local_884 & 0xffffff00U) | 1;
            goto LAB_1400cd654;
        }
        cVar2 = PECMD_MatchTokenAdvance("-exist", &cur, 6);
        uVar34 = 0;
        WVar6 = L'\0';
    } while (cVar2 != '\0');

    /* ================= SEG3: 主字段扫描与 F/U/R/:/# 拆解 (@126910) ================= */
LAB_1400cdb32:
    WVar5 = *cur;
    uVar8 = uVar12;
    while (WVar5 != WVar6) {
        if (WVar5 == L'*') {
            if (cur[1] == L'*') {
                cur = cur + 1;
                local_950 = 1;
                local_985 = 0x10;
            } else {
                local_950 = 1;
            }
        } else if (WVar5 == L'&') {
            uVar8 = 8;
        } else {
            local_944 = uVar8;
            if ((WVar5 != L'-') || ((ushort)(cur[1] + 0xffd0) < 10)) break;
            local_98c = 1;
        }
        cur = cur + 1;
        local_944 = uVar8;
        WVar5 = *cur;
    }
    FUN_14005B154(&cur);
    uVar15 = (uint64_t)PECMD_GetSpecialDirFirstChar();
    uVar8 = 0;
    local_874 = (WCHAR)uVar15;
    uVar39 = uVar12;
    if (*cur == L'\0') {
        local_984 = 0xffffffffU;
        local_938 = 0xffffffffU;
        local_968 = 0xffffffffU;
        local_890 = -1;
        uVar39 = 0xffffffffU;
    }
    uVar29 = local_968;
    local_878 = (uint)local_878 & 0xffff0000U;   /* 原体: 高半保留低半清零 TODO(verify) */
    local_90c = 0;
    pWVar27 = local_900;
    if (*cur != L'\0') {
        PECMD_SplitTokenTrimWs(&cur, &local_8d0, 0x2c);
        if (*cur == L',') {
            pWVar27 = cur + 1;
            cur = pWVar27;
            PECMD_SplitTokenTrimWs(&cur, &local_900, 0x2c);
            if ((ushort)((*pWVar27 & 0xffdfU) - 0x41U) < 0x1a) {
                local_995 = (byte)(*pWVar27 & 0xffdfU);
                local_97b = local_995;
                PECMD_ExpandDriveList(local_858, pWVar27, cur);
            }
            if (*cur != L',') goto LAB_1400cddb5;
            cur = cur + 1;
            PECMD_ParseShortStore(&cur, (int *)&local_8dc, 0x2c);
            uVar15 = 0x2c;
            if (*cur != L',') goto LAB_1400cddb5;
            cur = cur + 1;
            local_8a0 = cur;
            {
                WCHAR *tmp = local_8a0;
                FUN_14005B154(&tmp);
                local_8a0 = tmp;
            }
            FUN_1400F429C(&cur, (WCHAR)uVar15);
            if (((int)uVar39 < 1) && ((WCHAR)((int)uVar15 + -2) == *local_8a0)) {
                local_878 = (uint)((int)uVar15 + -2);
            }
            if ((ushort)((*local_8a0 & 0xffdfU) - 0x41U) < 0x1a) {
                local_995 = (byte)(*local_8a0 & 0xffdfU);
                local_97b = local_995;
                PECMD_ExpandDriveList(local_858, local_8a0, cur);
                goto LAB_1400cddb5;
            }
            if ((*local_8a0 == L'?') && ((local_996 != '\0' || (local_998 != '\0')))) {
                local_995 = (byte)*local_8a0;
            }
        } else {
LAB_1400cddb5:
            uVar15 = 0x2c;
        }
        uVar8 = 0;
        WVar6 = (WCHAR)uVar15;
        if (WVar6 == *cur) {
            if ((ushort)(cur[1] + 0xffd0) < 10) {
                cur = cur + 1;
                PECMD_ParseShortStore(&cur, (int *)&local_8f4, (short)WVar6);
                WVar6 = L',';
                uVar12 = local_8f4;
                if (*cur != L'\0') {
                    cur = cur + 1;
                }
            }
            uVar8 = 0;
            if (WVar6 == *cur) {
                cur = cur + 1;
                uVar8 = 0;
                local_8a0 = cur;
                {
                    WCHAR *tmp = local_8a0;
                    FUN_14005B154(&tmp);
                    local_8a0 = tmp;
                }
                FUN_1400F429C(&cur, WVar6);
                if ((ushort)((*local_8a0 & 0xffdfU) - 0x41U) < 0x1a) {
                    local_93c = (int)(char)(*local_8a0 & 0xffdfU);
                    PECMD_ExpandDriveList((uint8_t *)cand_tbl, local_8a0, cur);
                    uVar8 = 0;
                }
            }
        }
        cur = local_8d0;
        uVar22 = *local_8d0 & 0xffdf;
        if (((ushort)(*local_8d0 + 0xffd0) < 10) ||
            ((*local_8d0 == L'-') && ((ushort)(local_8d0[1] + 0xffd0) < 10))) {
            local_870 = local_8d0;
            PECMD_ParseHexOrDecBool((WCHAR **)&local_870, (int *)&local_890);
            uVar8 = 0;
            WVar6 = *cur;
            while (((WVar6 != L'\0' && (WVar6 != L':')) && (WVar6 != L'#'))) {
                cur = cur + 1;
                WVar6 = *cur;
            }
            FUN_14005B154(&cur);
            local_968 = (uint)local_890;
            uVar29 = (uint)local_890;
            bVar28 = (byte)uVar8;
LAB_1400cdf7a:
            pWVar27 = local_900;
            uVar22 = 9;
            WVar6 = L'#';
            if ((*cur == (WCHAR)uVar8) && (*local_900 != (WCHAR)uVar8)) {
                local_98c = 1;
            }
            FUN_14005B154(&cur);
            if (*cur != L':') {
                if (WVar6 != *cur) goto LAB_1400ce10d;
                local_997 = 1;
            }
            if ((bVar28 != (byte)uVar8) || ((int)uVar29 < (int)uVar8)) {
                local_997 = 0;
            }
            cur = cur + 1;
            FUN_14005B154(&cur);
            if ((*cur == L'-') && (uVar22 < (ushort)(cur[1] + 0xffd0))) {
                local_98c = 1;
            } else {
                PECMD_ParseShortStore(&cur, (int *)&local_938, 0x2c);
                uVar8 = 0;
                local_984 = local_938;
                uVar39 = local_938;
            }
            goto LAB_1400ce10d;
        }
        if (uVar22 == 0x46) {
            local_90c = -1;
LAB_1400ce02d:
            cur = local_8d0 + 1;
            res20[0] = 0xc;
            bVar28 = 0xc;
            goto LAB_1400cdf7a;
        }
        if ((uVar22 == 0x55) || (uVar22 == 0x52)) {
            local_90c = 1;
            local_964 = '\x01';
            goto LAB_1400ce02d;
        }
        bVar28 = res20[0];
        if (((*local_8d0 == L':') || (*local_8d0 == L'#')) ||
            ((local_98c != uVar8 || (*local_8d0 == (WCHAR)uVar8)))) goto LAB_1400cdf7a;
        goto LAB_1400ce40d;
    }

    /* 空命令体(*cur==0)时原体越过下方全部逻辑直达清理退出;
     * 此处等价复刻为再次门控。 */
    if (*cur != L'\0') {

    /* ================= SEG4: 分区区间换算 + 字母表准备 (@127077) ================= */
LAB_1400ce10d:
    local_970 = uVar39;
    local_948 = 0xfffffff8U;
    uVar39 = local_970;
    if (local_997 != 0) {
        if ((int)uVar8 < (int)local_970) {
            if ((int)local_970 < 5) {
                local_984 = local_970 - 1;
            } else {
                local_984 = local_970 * 4 - 0x10;
            }
            if ((int)local_984 < (int)uVar8) {
                uVar39 = local_984;
                if (3 < (int)local_984) goto LAB_1400ce15b;
            } else if ((int)local_984 < 4) {
                local_984 = local_984 + 1;
                uVar39 = local_984;
            } else {
LAB_1400ce15b:
                local_984 =
                    (uint)((((int)local_984 >> 0x1f & 3U) + local_984) >> 2) + 4;
                uVar39 = local_984;
            }
        }
LAB_1400ce173:
        iVar7 = lstrlenW(pWVar27);
        PECMD_AllocString(&local_900, (int64_t)iVar7 + 5);
        bVar28 = local_98f;
        if ((0x60 < (ushort)*local_900) && ((ushort)*local_900 < 0x7b)) {
            *local_900 = (WCHAR)(*local_900 + 0xffe2);
        }
        uVar34 = 0;
        local_94c = '\0';
        local_820 = (uint)(0x5a < (ushort)*local_900);
        if ((((res20[0] == 0) && (local_98c == 0)) &&
             ((ushort)*local_900 < 0x100)) &&
            (((ushort)*local_900 < 0x41 || (0x5a < (ushort)*local_900)))) {
            *local_900 = L'\0';
            if ((-1 < (int)local_968) && ((local_948 = 0xfffffff8U, 0 < (int)uVar39))) {
                local_948 = uVar39;
            }
            uVar34 = (ulonglong)local_968;
            if (((local_997 != 0) && (-1 < (int)local_968)) && (-1 < (int)uVar39)) {
                local_94c = '\x01';
                local_948 = uVar39;
            }
        }
    } else {
        goto LAB_1400ce173;
    }
    if ((local_91c == 0) || ((local_988 = '\x01', (uVar12 & 1) == 0))) {
        local_988 = '\0';
    }
    if (local_91c != 0) {
        local_884 = (uint)local_884 & 0xff;
        if ((uVar12 & 2) != 0) {
            local_884 = 1;
        }
        uVar34 = 0;
    }
    if ((((char)uVar14 == '\0') && ((local_91c == 0 || ((uVar12 & 4) == 0))))) {
        cVar4 = '\0';
    }
    pv18 = ((ulonglong)uVar34 >> 8 << 8) | (byte)cVar4;
    local_8eb = (byte)uVar12 & 0x10;
    local_8e0 = (byte)uVar12 & 0x40;
    if ((uVar12 & 0x20) != 0) {
        local_888 = 1;
    }
    if (cVar4 != '\0') {
        /* 注入 GPT 类型过滤条目至 (disk,part) 对区 (DAT_14012a258/268/278) */
        *(uint16_t *)&pair_area[(uint)local_98f * 4] = 0;
        FUN_140102a90(&pair_area[(uint)bVar28 * 4], 0, 0x10);
        *(uint16_t *)&pair_area[(uint)bVar28 * 4] = 0xee;
        *(uint16_t *)&pair_area[(uint)(byte)(bVar28 + 1) * 4] = 0;
        FUN_140102a90(&pair_area[(uint)(byte)(bVar28 + 1) * 4], 0, 0x10);
        *(uint16_t *)&pair_area[(uint)(byte)(bVar28 + 1) * 4] = 0xef;
        *(uint16_t *)&pair_area[(uint)(byte)(bVar28 + 2) * 4] = 1;
        FUN_14001d78c(&pair_area[(uint)(byte)(bVar28 + 2) * 4],
                      b7c_GuidEspSystem, 0x10);      /* DAT_14012a278 */
        *(uint16_t *)&pair_area[(uint)(byte)(bVar28 + 3) * 4] = 1;
        FUN_14001d78c(&pair_area[(uint)(byte)(bVar28 + 3) * 4],
                      b7c_GuidMsftReserved, 0x10);   /* DAT_14012a268 */
        *(uint16_t *)&pair_area[(uint)(byte)(bVar28 + 4) * 4] = 1;
        FUN_14001d78c(&pair_area[(uint)(byte)(bVar28 + 4) * 4],
                      b7c_GuidBasicData, 0x10);      /* DAT_14012a258 */
        local_98f = bVar28 + 5;
    }
    iVar7 = local_91c;
    if (((int)local_968 < 0) ||
        ((pv18 = (ulonglong)pv18 | 0x100, (int)uVar39 < 1))) {
        pv18 = pv18 & 0xffffffffffffff00ULL;
    }
    local_938 = (uint)((byte)pv18 != 0);
    local_920 = (uint)(ushort)*local_900 |
                (uint)(((pv18 >> 0x10) & 0xffff) << 0x10);
    local_908 = local_938;
    PECMD_AllocString(&local_958, 100);
    local_8f0 = GetLogicalDrives();

    /* ================= SEG5: 模式旗标收敛 / 快照缓冲 (@127172) ================= */
    uVar8 = local_968;
    uVar12 = local_970;
    if ((local_96c == '\0') || (local_995 != 0)) {
        if ((0 < local_90c) || (local_96c != '\0')) {
            local_93c = (int)(char)local_995;
            pcVar23 = &local_858[0];
            if (local_858[0] == '\0') {
                pcVar23 = local_898;
            }
            local_898 = pcVar23;
            if (local_96c != '\0') {
                res20[0] = 0xc;
                local_90c = 1;
            }
        }
        if ((iVar7 == 0) && (0 < local_90c)) {
            res20[0] = 0xb;
            local_90c = 0;
        }
        local_8dc = 1;
        if ((local_98c != 0) || ((0 < (int)local_970 && (bVar41)))) {
            local_974 = '\0';
LAB_1400ce501:
            local_97c = '\0';
        } else {
            local_974 = '\x01';
            if ((int)local_968 < 0) goto LAB_1400ce501;
            local_97c = '\x01';
            if (0 < (int)uVar39) {
                local_93f = '\x01';
                goto LAB_1400ce50e;
            }
        }
        local_93f = '\0';
LAB_1400ce50e:
        local_8ac = (uint)(local_90c < 1);
        PECMD_AllocStringSlot2((void **)&local_8c0, 0x3960);
        local_81c = 1;
        if (0 < (int)local_888) {
            local_81c = 9;
        }
        local_92e = '\0';
        if (((local_996 != '\0') || (uVar29 = local_81c, local_998 != '\0'))) {
            uVar29 = 0;
            if (local_995 == 0x3f) {
                FUN_14001d78c(local_858, (void *)cand_tbl, 0x1b);
                uVar29 = (uint)(uintptr_t)&local_858[0];
            }
        }
        local_8f4 = uVar29 & 0xffffff00U;
        if (local_91c < 1) {
            local_8f4 = (uint)((int)uVar12 < 1);
        }
    } else {
        /* '#3' 但默认盘符为 0 的角落: 原体直接落到清理退出 */
        goto LAB_1400ce40d;
    }
    /* 注: 原体中 LAB_1400ce5a6 起的主循环仍位于两级门控之内 */

    /* ================= SEG6: 物理盘枚举主循环 (@127227) ================= */
LAB_1400ce5a6:
    if (((local_997 == 0) && (local_979 != '\0')) &&
        ((-1 < (int)uVar8 && (0 < (int)uVar12)))) {
        local_948 = uVar39;
    }
    uVar12 = local_948;
    PECMD_AllocStringSlot2((void **)&local_918, 0x20fa38);
    pWVar1 = local_918;
    pWVar19 = pWVar1 + 0x106008;                 /* 驱动器表基址 (+0x20C010 字节) */
    pWVar27 = pWVar1 + 0x107cb8;                 /* 尾部 scratch (+0x20F970 字节) */
    local_8e8 = pWVar19;
    local_870 = pWVar27;
    if ((((local_97c == '\0') && ((char)local_987 == '\0')) &&
         ((res20[0] == 0 && (((int)uVar12 < 0 && (local_997 == 0)))))) &&
        ((*local_900 != L'\0' || ((local_970 != 0 && (local_970 != 0xffffffff)))))) {
        uVar8 = local_968;
        if (local_98c == 0) goto LAB_1400d00c1;
        if (((int)local_968 < 0) || ((int)local_970 < 1)) goto LAB_1400d00bd;
    }
    PECMD_EnumDrivesToTable((int64_t)(intptr_t)pWVar19, pWVar1, local_81c);
    if ((local_988 == '\0') || (local_92e == '\0')) {
        if (local_8e0 != 0) {
            lVar17 = 0x1a;
            pWVar25 = pWVar1 + 0x106010;         /* 表内首条目 */
            do {
                uVar12 = *(uint *)pWVar25;
                if (0 < (int)uVar12) {
                    pcVar23 = (char *)cand_tbl;  /* 候选表 A 扫描 */
                    cVar4 = *(char *)cand_tbl;
                    while (cVar4 != '\0') {
                        if ((int)cVar4 == (int)uVar12) goto LAB_1400ce723;
                        pcVar23 = pcVar23 + 1;
                        cVar4 = *pcVar23;
                    }
                    pcVar23 = (char *)local_858; /* 候选表 B 扫描 */
                    cVar4 = (char)local_858[0];
                    while (cVar4 != '\0') {
                        if ((int)cVar4 == (int)uVar12) goto LAB_1400ce723;
                        pcVar23 = pcVar23 + 1;
                        cVar4 = *pcVar23;
                    }
                    *(uint *)pWVar25 = uVar12 | 0xe000;
                }
LAB_1400ce723:
                pWVar25 = pWVar25 + 0x110;
                lVar17 = lVar17 + -1;
            } while (lVar17 != 0);
        }
        FUN_14001d78c((unsigned char *)local_8c0, (const unsigned char *)pWVar19, 0x3960);
        local_92e = '\x01';
    }
    letter_str[0] = (WCHAR)local_920;
    letter_str[1] = 0x3a;
    letter_str[2] = 0;
    mount_root[0] = (WCHAR)local_920;
    mount_root[1] = 0x3a;
    mount_root[2] = 0x5c;
    mount_root[3] = 0;
    local_860 = 0;
    local_8d8 = (uint)(res20[0] == 5);
    uVar12 = 0;
    if ((int)local_968 < 0) {
        uVar34 = (ulonglong)(uintptr_t)pWVar27 & 0xffffffffffff0000ULL;
        goto LAB_1400ce7cb;
    }
    uVar34 = 30000;
    local_980 = 30000;
    uVar8 = local_968;
    do {
        pWVar27 = local_870;
        bVar28 = res20[0];
        local_904 = (short)uVar8;
        local_978 = (uint)local_904;
        wsprintfW(local_870, (LPCWSTR)(local_8d8 != 0 ? L"\\\\.\\CDROM%d"
                                                        : L"\\\\.\\PhysicalDrive%d"),
                  (uint)(ushort)(local_904));
        hDev = (HANDLE)0;
        PECMD_OpenFileHandle(&hDev, (LPCWSTR)pWVar27, 0x80000000U, 3,
                             (LPSECURITY_ATTRIBUTES)0, 3, 0x20000000U, (HANDLE)0);
        pWVar27 = (WCHAR *)(uintptr_t)hDev;
        if (hDev == (HANDLE)0) {
            uVar22 = (short)uVar12 + 1;
            uVar12 = (uint)uVar22;
            uVar8 = local_968;
            uVar39 = local_984;
            if (0x1f < (short)uVar22) goto LAB_1400cfd2d;
        } else {
            local_910 = 0;
            local_7e0 = -1;
            local_7f8 = -1;
            if (local_974 != '\0') {
                local_7e0 = PECMD_FindPartitionStartSector(hDev, &local_87c, &local_7f8);
            }
            if (bVar28 == 0) {
LAB_1400ce990:
                local_934 = 0;
                local_990 = 0xff;
                puVar20 = PECMD_UpdatePartitionLayout(hDev, (uint64_t *)&local_918,
                                                      &local_934, &local_990, 0);
                if (puVar20 == (ulonglong *)0) goto joined_r0x0001400ce985;
                local_8b8 = puVar20 + 6;
                bVar41 = (*(int *)puVar20 == 1);
                local_986 = bVar41;
                DVar11 = (DWORD)PECMD_QueryDeviceIoInfo(hDev, 0x200);
                uVar34 = ((ulonglong *)puVar20)[5];
                local_97a = (byte)((((uint)((char)local_990 < '\0') - 1U) & 0x10) |
                                   (uint)bVar41);
                uVar12 = local_934;
                puVar20 = local_8b8;
                if ((char)local_990 < '\0') {
                    if (local_997 == 0) {
LAB_1400ceb42:
                        if ((int)local_970 <= (int)local_934) goto LAB_1400ceb57;
                    } else {
                        if (local_986 == '\0') {
                            if (0 < (int)local_970) {
                                if ((int)local_970 < 5) {
                                    local_984 = local_970 - 1;
                                } else {
                                    local_984 = local_970 * 4 - 0x10;
                                }
                            }
                            goto LAB_1400ceb42;
                        }
                        PECMD_ReadDiskSectorScan((uint64_t *)&local_890,
                                                 (uint64_t *)(void *)((byte *)pWVar1 +
                                                                      0x100008 * 2),
                                                 (uint)uVar34, DVar11, 0,
                                                 (uint64_t *)0, liZero);
                        puVar20 = local_8b8;
                        uVar12 = local_934;
                        if (((int)local_970 <= (int)(uint)uVar34) &&
                            ((local_984 = (uint)PECMD_FindPartitionByGeometryV2(
                                  (char *)((byte *)pWVar1 + (longlong)(int)DVar11 * 2 +
                                           (longlong)(int)(local_970 - 1) * 0x40 * 2 +
                                           0x100008 * 2),
                                  (int64_t)(intptr_t)local_8b8, local_934,
                                  (int)DVar11, 1)),
                             -1 < (int)local_984)) {
                            if (local_94c != '\0') {
                                local_948 = local_984;
                            }
                            goto LAB_1400ceb57;
                        }
                    }
                } else {
                    local_984 = local_970 - 1;
                }
                /* 原体: ceb57 链位于 outType>=0 的 else 侧; GPT 路径经 goto 跳入 */
                if (!((char)local_990 < '\0')) {
LAB_1400ceb57:;
                    iVar7 = (int)PECMD_GetDiskGeometry((LPCWSTR)0, (HANDLE)hDev);
                    if ((((((iVar7 == 7) || (local_97a != 0)) || (uVar12 != 4)) ||
                         ((local_940 != (char)puVar20[4] ||
                           ((char)puVar20[0x16] != '\0')))) ||
                        ((char)puVar20[0x28] != '\0')) ||
                       ((char)puVar20[0x3a] != '\0')) {
                        local_87c = 0;
                        local_8b0 = local_8b0 + -1;
                        local_994 = 0;
                        if (0 < local_8b0) {
                            local_7e8 = (longlong)(int)local_948;
                            do {
                                uVar8 = local_994;
                                if ((int)uVar12 <= (int)local_994) break;
                                local_890 = (int64_t)local_994;
                                LeaveCriticalSection(&g_csDisk);
                                EnterCriticalSection(&g_csDisk);
                                if (local_8d8 != 0) {
                                    local_994 = uVar12 + 100000;
                                }
                                if (((local_997 != 0) || (-1 < (char)local_990)) &&
                                    (-1 < (int)local_984)) {
                                    local_994 = uVar12 + 100000;
                                    if ((char)local_990 < '\0') {
                                        if ((local_997 != 0) &&
                                            ((local_890 = (int64_t)local_984,
                                              uVar8 = local_984,
                                              (int)uVar12 <= (int)local_984))) break;
                                    } else {
                                        local_890 &= (int64_t)0xffffffff00000000ULL;
                                        if ((int)uVar12 < 1) break;
                                        puVar20 = local_8b8 + 3;
                                        uVar8 = 0;
                                        while ((int)(short)*puVar20 != local_970) {
                                            uVar8 = uVar8 + 1;
                                            puVar20 = puVar20 + 0x12;
                                            local_890 = (int64_t)uVar8;
                                            if ((int)uVar12 <= (int)uVar8)
                                                goto joined_r0x0001400cea8e;
                                        }
                                        if ((int)uVar8 < 0) break;
                                    }
                                }
                                iVar7 = 0;
                                if (local_986 == '\0') {
                                    uVar12 = (uint)info[(longlong)(int)uVar8 * 0x12 * 8 + 32];
                                    /* decompiled: (uint)(byte)local_8b8[i*0x12+4] */
                                } else {
                                    uVar12 = 0xff07;
                                }
                                iVar9 = (int)(short)local_930;
                                ent = (byte *)(uintptr_t)((longlong)(int)uVar8 * 0x90);
                                local_8f8 = (uint)*(ushort *)((uintptr_t)ent + 24 +
                                                              (uintptr_t)local_8b8);
                                if (0 < iVar9) {
                                    puVar20 = (ulonglong *)&pair_area[2];
                                    do {
                                        if ((local_904 == *(short *)((byte *)puVar20 - 2)) &&
                                            (local_8f8 ==
                                             (int)(short)*(ushort *)puVar20)) break;
                                        iVar7 = iVar7 + 1;
                                        puVar20 = (ulonglong *)((byte *)puVar20 + 4);
                                    } while (iVar7 < iVar9);
                                }
                                uVar8 = uVar12;
                                if (iVar9 <= iVar7) {
                                    local_8a8 = uVar12;
                                    if (local_98f != 0) {
                                        pcVar23 = (char *)((uintptr_t)ent + 0x20 +
                                                           (uintptr_t)local_8b8);
                                        iVar7 = 0;
                                        uVar34 = (ulonglong)(byte)(
                                            ((uint)(-(local_97a != 0) & 0xfU)) + 1);
                                        uVar39 = (uint)local_98f;
                                        pbVar33 = local_6e8;
                                        do {
                                            if ((local_97a == *pbVar33) &&
                                                ((iVar9 = FUN_14005B184(
                                                     pcVar23,
                                                     (int64_t)(intptr_t)(local_6e8 +
                                                                         (longlong)iVar7 *
                                                                             0x11 + 1),
                                                     (int64_t)(uVar34 & 0xff)),
                                                  iVar9 == 0))) break;
                                            iVar7 = iVar7 + 1;
                                            pbVar33 = pbVar33 + 0x11;
                                        } while (iVar7 < (int)uVar39);
                                        uVar8 = local_8a8;
                                        if (iVar7 < (int)uVar39) goto LAB_1400ceefb;
                                    }
                                    pWVar32 = (WCHAR *)ent;
                                    uVar39 = local_8f8;
                                    uVar8 = local_8a8;
                                    if (((local_987 != '\0') && (-1 < (int)local_984)) ||
                                        ((local_93f != '\0') && (local_997 == 0) &&
                                         (0 < (int)local_984))) {
                                        if (local_8f8 != local_984) goto LAB_1400ceefb;
                                        if ((local_987 != '\0') &&
                                            (((((uVar12 & 0x10) != 0 || (uVar12 == 0x84)) ||
                                               (uVar12 == 0xef)) ||
                                              ((local_97a == 1 &&
                                                ((*(ulonglong *)
                                                  ((uintptr_t)ent + 0x40 +
                                                   (uintptr_t)local_8b8) &
                                                  0xc000000000000000ULL) != 0)))))) {
                                            /* '*' 命中当前目标分区 → 立即退出 (unaff_RBX) */
                                            if (pWVar27 != (WCHAR *)0xffffffffffffffff) {
                                                CloseHandle((HANDLE)pWVar27);
                                            }
                                            PECMD_FreeStrBuf((WCHAR **)&local_918);
                                            PECMD_FreeStrBuf((WCHAR **)&local_8c0);
                                            PECMD_FreeStrBuf(&local_8d0);
                                            PECMD_FreeStrBuf(&local_958);
                                            PECMD_FreeStrBuf(&local_900);
                                            LeaveCriticalSection(&g_csDisk);
                                            return rbx_ret;
                                        }
                                        local_994 = local_934 + 100000;
                                    }
                                    if (((local_974 == '\0') ||
                                        ((lVar17 = *(longlong *)
                                          ((uintptr_t)ent + 8 +
                                           (uintptr_t)local_8b8),
                                          lVar17 < 1)) ||
                                        ((local_7e0 != lVar17 &&
                                          (local_7f8 != lVar17))))) {
                                            /* L2: CDROM 或 非常驻类型分区才进入应用链 */
                                            if ((local_8d8 != 0) ||
                                               ((((uVar12 != 0 && (0 < (int)local_8f8)) &&
                                                  (uVar12 != 5)) && (uVar12 != 0xf)))) {
                                            if (((int)local_984 < 1) &&
                                                (*(longlong *)
                                                 ((uintptr_t)ent + 16 +
                                                  (uintptr_t)local_8b8) < 0x201)) {
                                                if ((uVar12 & 0x10) != 0) goto LAB_1400ceec7;
                                            }
                                            else {
                                                local_860 = local_860 + 1;
                                                if ((local_997 != 0) ||
                                                   ((local_7e8 < 1 ||
                                                     (local_948 == local_8f8)))) {
                                                    if (-1 < local_7e8) {
                                                        local_994 = local_934 + 1000;
                                                    }
                                                    if ((local_997 == 0) &&
                                                       ((((int)local_984 < 0 ||
                                                          (local_984 != local_8f8)) ||
                                                         ((int)local_8f8 < 1)))) {
                                                        if ((short)local_878 == 0) {
                                                            if ((((local_98c == 0) &&
                                                                  (local_7e8 < 0)) &&
                                                                 (0 < (int)local_984)) &&
                                                                ((int)local_984 <= local_860)) {
                                                                uVar8 = (uint)local_904;
                                                                local_984 = local_8f8;
                                                                local_968 = uVar8;
                                                                if (pWVar27 !=
                                                                    (WCHAR *)0xffffffffffffffff) {
                                                                    CloseHandle((HANDLE)pWVar27);
                                                                }
                                                                goto LAB_1400cfd2d;
                                                            }
                                                            sVar38 = -1;
                                                            if ((local_984 == 0) ||
                                                                (local_987 != '\0')) {
                                                                if (local_986 == '\0') {
                                                                    if ((((local_8a8 & 0x10)
                                                                          != 0) ||
                                                                         (local_8a8 == 0x84)) ||
                                                                        (local_8a8 == 0xef)) {
LAB_1400cf02b:
                                                                        /* 驱动器表现存预扫: 命中则 sVar40=1 直入应用 */
                                                                        if (local_8eb != 0) {
                                                                            sVar21 = 0;
                                                                            do {
                                                                                lVar17 =
                                                                                    (longlong)sVar21;
                                                                                if ((*(uint *)
                                                                                     ((byte *)local_8e8 +
                                                                                      lVar17 * 0x220 +
                                                                                      4) ==
                                                                                     local_978) &&
                                                                                    (*(uint *)
                                                                                     ((byte *)local_8e8 +
                                                                                      lVar17 * 0x220) ==
                                                                                     local_8f8) &&
                                                                                    (
                                                                                        sVar40 = 1,
                                                                                        0 <
                                                                                        *(int *)
                                                                                        ((byte *)local_8e8 +
                                                                                         lVar17 * 0x220 +
                                                                                         16)))
                                                                                    goto LAB_1400cf0a2;
                                                                                sVar21 = sVar21 + 1;
                                                                            } while (sVar21 < 0x1a);
                                                                        }
                                                                        goto LAB_1400cf07d;
                                                                    }
                                                                }
                                                                else if ((*(ulonglong *)
                                                                          ((uintptr_t)ent +
                                                                           0x40 +
                                                                           (uintptr_t)local_8b8) &
                                                                          0xc000000000000000ULL) !=
                                                                         0) {
                                                                    goto LAB_1400cf02b;
                                                                }
                                                                sVar38 = 2;
                                                                sVar40 = 2;
                                                                if (local_984 != 0)
                                                                    goto LAB_1400cf07d;
                                                            }
                                                            else {
LAB_1400cf07d:
                                                                sVar40 = sVar38;
                                                                if (((local_97c == '\0') ||
                                                                     (local_984 != local_8f8)) &&
                                                                    (((-1 < (int)local_984 &&
                                                                      ((local_98c == 0 ||
                                                                        (local_984 != local_8f8)))) &&
                                                                     (local_7e8 < 0))))
                                                                    goto LAB_1400ceefb;
                                                            }
LAB_1400cf0a2:
                                                            /* 驱动器表 26 槽 Unshow 扫描 */
                                                            bVar41 = false;
                                                            sVar38 = 0;
                                                            local_8c8 = L'\0';
                                                            pWVar19 = local_8e8;
                                                            do {
                                                                lVar36 = (longlong)sVar38;
                                                                lVar17 = lVar36 * 0x220;
                                                                if (*(int *)
                                                                     ((byte *)pWVar19 +
                                                                      lVar36 * 0x220 + 8) == 5) {
LAB_1400cf0ea:
                                                                    if (local_8d8 != 0) {
LAB_1400cf0f3:
                                                                        if ((*(uint *)
                                                                             ((byte *)pWVar19 +
                                                                              lVar36 * 0x220 +
                                                                              4) == local_978) &&
                                                                            (*(uint *)
                                                                             ((byte *)pWVar19 +
                                                                              lVar36 * 0x220) ==
                                                                             local_8f8) &&
                                                                            (0 < *(int *)
                                                                             ((byte *)pWVar19 +
                                                                              lVar36 * 0x220 +
                                                                              16))) {
                                                                            bVar41 = true;
                                                                            local_8c8 =
                                                                                (WCHAR)(byte)
                                                                                    *((byte *)pWVar19 +
                                                                                      lVar36 * 0x220 +
                                                                                      16);
                                                                            if (local_98c == 0)
                                                                                goto LAB_1400cf24d;
                                                                            PECMD_MarkKeyTable(
                                                                                local_8c8,
                                                                                (int64_t)(intptr_t)pWVar19);
                                                                            letter_str[0] =
                                                                                *(WCHAR *)
                                                                                    ((byte *)local_8e8 +
                                                                                     lVar17 + 0x10);
                                                                            local_994 =
                                                                                local_934 + 1000;
                                                                            mount_root[0] =
                                                                                letter_str[0];
                                                                            if (local_91c != 0) {
                                                                                /* 原体第 3 参为盘符 (栈变参截断) TODO(verify) */
                                                                                PECMD_TlsLogWrite(
                                                                                    (uint64_t)(uintptr_t)g_Script,
                                                                                    (LPCWSTR)L"Unshow pt: %d:%d %c\r\n",
                                                                                    (uint64_t)local_978,
                                                                                    (uint64_t)local_8f8);
                                                                            }
                                                                            if ((local_950 != 0) &&
                                                                                ((BVar13 =
                                                                                     DeleteVolumeMountPointW(
                                                                                         (LPCWSTR)mount_root),
                                                                                  BVar13 == 0))) {
                                                                                GetLastError();
                                                                            }
                                                                            BVar13 = DefineDosDeviceW(
                                                                                local_944 | 2,
                                                                                (LPCWSTR)letter_str,
                                                                                (LPCWSTR)(
                                                                                    -(uint64_t)(
                                                                                        *local_958 != L'\0') &
                                                                                    (uint64_t)(intptr_t)local_958));
                                                                            if (BVar13 == 0) {
                                                                                GetLastError();
                                                                            }
                                                                            pWVar19 = local_8e8;
                                                                            if (local_950 != 0) {
                                                                                PECMD_DeleteDriveMountPoint(
                                                                                    letter_str[0]);
                                                                                pWVar19 = local_8e8;
                                                                            }
                                                                        }
                                                                    }
                                                                } else {
                                                                    if (local_8d8 == 0)
                                                                        goto LAB_1400cf0f3;
                                                                    if (*(int *)
                                                                         ((byte *)pWVar19 +
                                                                          lVar36 * 0x220 + 8) == 5)
                                                                        goto LAB_1400cf0ea;
                                                                }
                                                                sVar38 = sVar38 + 1;
                                                            } while (sVar38 < 0x1a);
                                                            uVar8 = local_8a8;
                                                            if (local_98c == 0) {
LAB_1400cf24d:
                                                                WVar6 = local_8c8;
                                                                if ((sVar40 < 1) &&
                                                                    (local_987 != '\0')) {
                                                                    uVar8 = local_8a8;
                                                                    if (local_986 == '\0') {
                                                                        if ((((local_8a8 & 0x10) ==
                                                                              0) &&
                                                                             (local_8a8 != 0x84)) &&
                                                                            (local_8a8 != 0xef))
                                                                            goto LAB_1400cf2af;
                                                                    } else {
                                                                        if ((*(ulonglong *)
                                                                             ((uintptr_t)ent + 0x40 +
                                                                              (uintptr_t)local_8b8) &
                                                                             0xc000000000000000ULL)
                                                                            == 0)
                                                                            goto LAB_1400cf2af;
                                                                    }
                                                                } else {
LAB_1400cf2af:
                                                                    /* Show 应用: 选符/挂载/NUL 探测/表回写 */
                                                                    sVar38 = 0;
                                                                    local_7f0 = local_898;
                                                                    local_880 = 0;
                                                                    local_8a8 &= 0xffff0000U;
                                                                    if (local_91c == 0) {
                                                                        WVar35 = (WCHAR)local_920;
                                                                        WVar5 = (WCHAR)local_93c;
LAB_1400cf6f3:
                                                                        uVar12 = local_8f8;
                                                                        if ((!bVar41) ||
                                                                            ((uVar8 = local_8a8,
                                                                              WVar35 != L'\0' &&
                                                                                  (local_979 ==
                                                                                   '\0')))) {
                                                                            if ((char)local_990 < '\0') {
                                                                                if (local_8d8 == 0) {
                                                                                    wsprintfW(local_958,
                                                                                        (LPCWSTR)L"\\Device\\Harddisk%d\\Partition%d",
                                                                                        (int)(short)local_978,
                                                                                        (int)(short)local_8f8);
                                                                                } else {
                                                                                    wsprintfW(local_958,
                                                                                        (LPCWSTR)L"\\\\.\\CDROM%d",
                                                                                        (int)(short)local_978);
                                                                                    uVar12 = local_8f8;
                                                                                }
                                                                            } else {
                                                                                PECMD_FindVolumeByDeviceId(
                                                                                    (uint32_t *)(uintptr_t)
                                                                                        ((uintptr_t)ent + 0x30 +
                                                                                         (uintptr_t)local_8b8),
                                                                                    (int64_t *)&local_958,
                                                                                    (LPWSTR)1);
                                                                                uVar12 = local_8f8;
                                                                            }
                                                                            cVar4 = (char)local_8f4;
                                                                            if (((short)local_920 == 0) ||
                                                                                (cVar4 != '\0')) {
                                                                                iVar7 = PECMD_PickFreeDriveLetter(
                                                                                    &local_8f0, 0xc,
                                                                                    (char)(local_97b + 0xbf),
                                                                                    local_898);
                                                                                WVar6 = (WCHAR)iVar7;
                                                                                mount_root[0] = WVar6;
                                                                                letter_str[0] = WVar6;
                                                                            } else {
                                                                                WVar6 = mount_root[0];
                                                                            }
                                                                            if (0x19 <
                                                                                (ushort)(letter_str[0] +
                                                                                         0xffbf))
                                                                                goto LAB_1400cff4b;
                                                                            if (WVar6 != L'\0') {
                                                                                if ((WCHAR)local_920 ==
                                                                                    WVar6) {
                                                                                    if (local_91c == 0) {
                                                                                        cVar4 = '\x01';
                                                                                    }
                                                                                    local_8f4 =
                                                                                        ((uint)local_8f4 &
                                                                                         0xffffff00U) |
                                                                                        (byte)cVar4;
                                                                                }
                                                                                BVar13 = PECMD_DosDeviceMount(
                                                                                    (LPCWSTR)local_958,
                                                                                    (LPCWSTR)letter_str,
                                                                                    (WCHAR *)mount_root,
                                                                                    local_944,
                                                                                    (char)local_950);
                                                                                if (BVar13 == 0) {
                                                                                    local_908 = GetLastError();
                                                                                } else {
                                                                                    local_908 = 0;
                                                                                }
                                                                                local_938 = local_908;
                                                                                if ((local_985 & 0x10) != 0) {
                                                                                    PECMD_SendHotkeyKeyMessage(
                                                                                        (uint32_t)mount_root[0],
                                                                                        2, 0x32);
                                                                                }
                                                                                bVar41 = true;
                                                                                if (((char)local_884 != '\0') &&
                                                                                    (
                                                                                        bVar41 = true,
                                                                                        local_880 == 0)) {
                                                                                    /* FindFirstFileW("X:\NUL") 校验卷真实可访问 */
                                                                                    local_838[0] =
                                                                                        (uint64_t)(uint32_t)
                                                                                            mount_root[0] |
                                                                                        0x003a0000ULL |
                                                                                        0x005c00000000ULL |
                                                                                        0x004e000000000000ULL;
                                                                                    local_838[1] = 0x004c0055ULL;
                                                                                    hFindFile = FindFirstFileW(
                                                                                        (LPCWSTR)local_838,
                                                                                        &local_288);
                                                                                    if (hFindFile ==
                                                                                        (HANDLE)
                                                                                            0xffffffffffffffff) {
                                                                                        local_93c =
                                                                                            ((int)local_93c &
                                                                                             0xffff0000) |
                                                                                            (int)WVar5;
                                                                                        local_920 = 0;
                                                                                        bVar41 = false;
                                                                                        local_898 = local_7f0;
                                                                                        BVar13 =
                                                                                            DeleteVolumeMountPointW(
                                                                                                (LPCWSTR)mount_root);
                                                                                        if (BVar13 == 0) {
                                                                                            GetLastError();
                                                                                        }
                                                                                        BVar13 =
                                                                                            DefineDosDeviceW(
                                                                                                local_944 | 2,
                                                                                                (LPCWSTR)letter_str,
                                                                                                (LPCWSTR)0);
                                                                                        if (BVar13 == 0) {
                                                                                            GetLastError();
                                                                                        }
                                                                                        if (local_950 != 0) {
                                                                                            PECMD_DeleteDriveMountPoint(
                                                                                                letter_str[0]);
                                                                                        }
                                                                                    } else {
                                                                                        FindClose(hFindFile);
                                                                                        local_8f0 = local_8f0 |
                                                                                                    1 <<
                                                                                                        (((char)mount_root[0] +
                                                                                                          0xbfU) &
                                                                                                         0x1f);
                                                                                        bVar41 = true;
                                                                                    }
                                                                                }
                                                                                if (local_91c != 0) {
                                                                                    /* 原体另有盘符与状态两个栈变参 TODO(verify) */
                                                                                    PECMD_TlsLogWrite(
                                                                                        (uint64_t)(uintptr_t)g_Script,
                                                                                        (LPCWSTR)L"Show pt: %d:%d %c %d\r\n",
                                                                                        (uint64_t)local_978,
                                                                                        (uint64_t)uVar12);
                                                                                }
                                                                                if ((bVar41) &&
                                                                                    (((local_996 != '\0' ||
                                                                                       (local_998 != '\0')) &&
                                                                                      (local_8ac != 0)))) {
                                                                                    PECMD_RemoveFirstMatchChar(
                                                                                        mount_root[0],
                                                                                        (char *)local_858);
                                                                                }
                                                                            }
                                                                            uVar8 = local_8a8;
                                                                            if ((short)local_8a8 != 0) {
                                                                                lVar17 = (longlong)(short)
                                                                                    ((short)local_8a8 + -0x41);
                                                                                uVar39 = 0xfffffff1U;
                                                                                if ((short)local_920 != 0) {
                                                                                    uVar39 = local_978;
                                                                                }
                                                                                *(uint *)((byte *)local_8e8 +
                                                                                          lVar17 * 0x220 +
                                                                                          4) = uVar39;
                                                                                uVar39 = 0xfffffff1U;
                                                                                if ((short)local_920 != 0) {
                                                                                    uVar39 = uVar12;
                                                                                }
                                                                                *(uint *)((byte *)local_8e8 +
                                                                                          lVar17 * 0x220) =
                                                                                    uVar39;
                                                                                *(uint *)((byte *)local_8e8 +
                                                                                          lVar17 * 0x220 +
                                                                                          16) =
                                                                                    local_920 & 0xffff;
                                                                            }
                                                                        }
                                                                    } else {
                                                                        /* '#N' 模式: 对照快照表回收旧字母后重入 Show 应用 */
                                                                        if ((local_988 != '\0') ||
                                                                            ((char)local_884 != '\0')) {
                                                                            do {
                                                                                lVar17 = (longlong)sVar38 *
                                                                                         0x220;
                                                                                if (*(int *)
                                                                                     ((byte *)local_8c0 +
                                                                                      lVar17 + 8) == 5) {
LAB_1400cf32d:
                                                                                    if (local_8d8 != 0) {
LAB_1400cf332:
                                                                                        if ((*(uint *)
                                                                                             ((byte *)local_8c0 +
                                                                                              lVar17 + 4) ==
                                                                                             local_978) &&
                                                                                            (*(uint *)
                                                                                             ((byte *)local_8c0 +
                                                                                              lVar17) ==
                                                                                             local_8f8)) {
                                                                                            local_880 =
                                                                                                *(ushort *)
                                                                                                    ((byte *)local_8c0 +
                                                                                                     (longlong)sVar38 *
                                                                                                         0x220 +
                                                                                                     0x10);
                                                                                            break;
                                                                                        }
                                                                                    }
                                                                                } else {
                                                                                    if (local_8d8 == 0)
                                                                                        goto LAB_1400cf332;
                                                                                    if (*(int *)
                                                                                         ((byte *)local_8c0 +
                                                                                          lVar17 + 8) == 5)
                                                                                        goto LAB_1400cf32d;
                                                                                }
                                                                                sVar38 = sVar38 + 1;
                                                                            } while (sVar38 < 0x1a);
                                                                        }
                                                                        iVar7 = local_87c + 1;
                                                                        cVar4 = (char)local_93c;
                                                                        uVar8 = local_8a8;
                                                                        if (local_8ec == '\0') {
                                                                            if (local_998 == '\x01') {
                                                                                local_994 = local_934 + 1000;
                                                                            }
                                                                        } else if (((local_998 != '\0') &&
                                                                                    (4 < (int)(uint)local_890))
                                                                                   &&
                                                                                   (
                                                                                       local_87c =
                                                                                           local_87c + 2,
                                                                                       iVar7 = local_87c,
                                                                                       local_998 == '\x01'))
                                                                            goto LAB_1400ceefb;
                                                                        local_87c = iVar7;
                                                                        if (((((local_998 != '\x02') ||
                                                                               (local_8c8 == L'\0')) ||
                                                                              (local_87c != 1)) &&
                                                                             ((local_988 == '\0' ||
                                                                              ((local_880 != 0 &&
                                                                                ((local_880 & 0x2000) ==
                                                                                 0)))))) &&
                                                                            (local_874 != local_8c8)) {
                                                                            iVar7 = local_93c;
                                                                            if (local_874 ==
                                                                                (WCHAR)local_93c) {
                                                                                if (*local_898 == '\0')
                                                                                    goto LAB_1400ceefb;
                                                                                local_898 = local_898 + 1;
                                                                                iVar7 = (int)*local_898;
                                                                            }
                                                                            WVar5 = PECMD_NextTokenChar(
                                                                                (ushort)iVar7,
                                                                                (int64_t)(intptr_t)local_8e8,
                                                                                (int64_t)(intptr_t)pair_area,
                                                                                local_930,
                                                                                (int64_t *)&local_898);
                                                                            pWVar19 = local_8e8;
                                                                            uVar30 = (ulonglong)WVar5;
                                                                            mount_root[0] = WVar5;
                                                                            /* 原体高位取自指针位, 仅低位有语义 TODO(verify) */
                                                                            local_93c = (int)(ushort)WVar5;
                                                                            local_920 = (uint)uVar30;
                                                                            local_7f0 = local_898;
                                                                            letter_str[0] = WVar5;
                                                                            uVar8 = local_8a8;
                                                                            if ((ushort)WVar5 < 0x5b) {
                                                                                if (*local_898 == '\0') {
                                                                                    local_93c = 0x5b;
                                                                                } else {
                                                                                    local_898 = local_898 + 1;
                                                                                    local_93c = (int)*local_898;
                                                                                }
                                                                                if (WVar6 != WVar5) {
                                                                                    if ((local_8f0 &
                                                                                         1 << (((char)cVar4 +
                                                                                                0xbfU) &
                                                                                               0x1f)) != 0) {
                                                                                        PECMD_MarkKeyTable(
                                                                                            WVar5,
                                                                                            (int64_t)(intptr_t)local_8e8);
                                                                                        letter_str[0] =
                                                                                            (WCHAR)uVar30;
                                                                                        mount_root[0] =
                                                                                            (WCHAR)uVar30;
                                                                                        PECMD_TlsLogWrite(
                                                                                            (uint64_t)(uintptr_t)g_Script,
                                                                                            (LPCWSTR)L"unShow-int1 %c\r\n",
                                                                                            (uint64_t)(ushort)uVar30,
                                                                                            0);
                                                                                        BVar13 =
                                                                                            DeleteVolumeMountPointW(
                                                                                                (LPCWSTR)mount_root);
                                                                                        if (BVar13 == 0) {
                                                                                            GetLastError();
                                                                                        }
                                                                                        BVar13 = DefineDosDeviceW(
                                                                                            local_944 | 2,
                                                                                            (LPCWSTR)letter_str,
                                                                                            (LPCWSTR)0);
                                                                                        if (BVar13 == 0) {
                                                                                            GetLastError();
                                                                                        }
                                                                                        if (local_950 != 0) {
                                                                                            PECMD_DeleteDriveMountPoint(
                                                                                                letter_str[0]);
                                                                                        }
                                                                                        uVar30 =
                                                                                            (ulonglong)local_920;
                                                                                        local_8a8 =
                                                                                            ((uint)local_8a8 &
                                                                                             0xffff0000U) |
                                                                                            (uint)(ushort)local_920;
                                                                                    }
                                                                                    WVar35 = (WCHAR)uVar30;
                                                                                    if (WVar6 != L'\0') {
                                                                                        PECMD_MarkKeyTable(
                                                                                            WVar6,
                                                                                            (int64_t)(intptr_t)pWVar19);
                                                                                        mount_root[0] = WVar6;
                                                                                        letter_str[0] = WVar6;
                                                                                        PECMD_TlsLogWrite(
                                                                                            (uint64_t)(uintptr_t)g_Script,
                                                                                            (LPCWSTR)L"unshow-int2 %c\r\n",
                                                                                            (uint64_t)(ushort)WVar6,
                                                                                            0);
                                                                                        BVar13 =
                                                                                            DeleteVolumeMountPointW(
                                                                                                (LPCWSTR)mount_root);
                                                                                        if (BVar13 == 0) {
                                                                                            GetLastError();
                                                                                        }
                                                                                        BVar13 = DefineDosDeviceW(
                                                                                            local_944 | 2,
                                                                                            (LPCWSTR)letter_str,
                                                                                            (LPCWSTR)0);
                                                                                        if (BVar13 == 0) {
                                                                                            GetLastError();
                                                                                        }
                                                                                        if (local_950 != 0) {
                                                                                            PECMD_DeleteDriveMountPoint(
                                                                                                letter_str[0]);
                                                                                        }
                                                                                        letter_str[0] =
                                                                                            (WCHAR)local_920;
                                                                                        mount_root[0] =
                                                                                            (WCHAR)local_920;
                                                                                        local_8f0 =
                                                                                            local_8f0 &
                                                                                            ~(1 <<
                                                                                              (((char)WVar6 +
                                                                                                0xbfU) &
                                                                                               0x1f));
                                                                                        WVar35 = letter_str[0];
                                                                                    }
                                                                                    bVar41 = false;
                                                                                    goto LAB_1400cf6f3;
                                                                                }
                                                                                if (local_8ac != 0) {
                                                                                    PECMD_RemoveFirstMatchChar(
                                                                                        WVar5,
                                                                                        (char *)local_858);
                                                                                    uVar8 = local_8a8;
                                                                                }
                                                                            }
                                                                        }
                                                                    }
                                                                }
                                                            }
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                    }
                                    /* 原体 L1 else-if: 起始扇区匹配时直接以 985|1 模式卸除 */
                                    if (!(((local_974 == '\0') ||
                                        ((lVar17 = *(longlong *)
                                          ((uintptr_t)ent + 8 +
                                           (uintptr_t)local_8b8),
                                          lVar17 < 1)) ||
                                        ((local_7e0 != lVar17 &&
                                          (local_7f8 != lVar17)))))) {
                                        if (((short)local_878 != 0) && (0 < (int)local_8f8)) {
LAB_1400ceec7:;
                                            PECMD_SetDriveMount(
                                                (int64_t)(intptr_t)local_8e8, local_978,
                                                local_8f8, 0, local_985 | 1, 1,
                                                (uint16_t *)0, (uint32_t *)0);
                                            uVar8 = local_8a8;
                                        }
                                    }
                                    }   /* L2 补收拢 */
LAB_1400ceefb:;
                                    local_8a8 = uVar8;
                                    local_994 = local_994 + 1;
                                    local_8b0 = local_8b0 + -1;
                                    uVar12 = local_934;
                                } while (0 < local_8b0);
                            }
                        }
                }
                } else {
                        iVar9 = PECMD_QueryDiskGeometry((HANDLE)pWVar27, (uint64_t *)pWVar1, 1, 0x800);
                        iVar7 = local_90c;
                        if (((bVar28 == 5) || (iVar9 != 0xb)) || (bVar28 == 0xb)) {
                            if ((local_90c == 0) && (local_964 < '\x01')) {
LAB_1400ce95a:
                                if (iVar9 == 0xc) {
                                    iVar9 = 0xb;
                                }
                            } else {
                                iVar10 = (int)PECMD_GetDiskGeometry((LPCWSTR)0, (HANDLE)pWVar27);
                                if ((((0 < iVar7) || ('\0' < local_964)) && (iVar10 != 7)) ||
                                    ((iVar7 < 0 && (iVar10 == 7)))) goto joined_r0x0001400ce985;
                                if (iVar7 == 0) goto LAB_1400ce95a;
                            }
                            if ((res20[0] == 5) || (iVar9 == (char)res20[0])) goto LAB_1400ce990;
                        }
                        goto joined_r0x0001400ce985;
                    }
joined_r0x0001400cea8e:
                    uVar12 = local_910;
                    if (pWVar27 != (WCHAR *)0xffffffffffffffff) {
                        CloseHandle((HANDLE)pWVar27);
                        uVar12 = local_910;
                    }
                    goto LAB_1400ce81c_common;
joined_r0x0001400ce985:
                    uVar12 = 0;
                    if (pWVar27 != (WCHAR *)0xffffffffffffffff) {
                        CloseHandle((HANDLE)pWVar27);
                        uVar12 = 0;
                    }
LAB_1400ce81c_common:;
                }
                uVar34 = (ulonglong)local_980;
                uVar34 = (uVar34 & 0xffffffffffff0000ULL) |
                         (ulonglong)(ushort)((short)uVar34 + 1);
LAB_1400ce7cb:
                local_980 = (uint)uVar34;
                local_8b0 = local_8b0 + -1;
                uVar8 = local_968;
                uVar39 = local_984;
                if ((local_8b0 < 1) || (15999 < (short)(ushort)uVar34)) goto LAB_1400cfd2d;
                uVar8 = (uint)(ushort)uVar34;
            } while (true);
        }
    /* (原体 gate2/gate1 于主循环后收拢; 本移植 SEG7 起为 goto-only 区,
     *  置于 gate1 内层以保持符号作用域一致, 语义不变) */

    /* ================= SEG7: 预算耗尽阶段升级 (@128038) ================= */
LAB_1400cfd2d:
    local_8dc = local_8dc + 1;
    if (local_998 == '\x01') {
        local_998 = '\x02';
    } else if (local_96c == '\x01') {
        local_96c = '\x02';
        res20[0] = 0xb;
    } else {
        if (((local_996 == '\0') && (local_998 == '\0')) || (local_995 == 0)) goto LAB_1400d0083;
        if ((local_996 == '\x01') || (local_998 == '\x02')) {
            cVar4 = (char)((-(local_888 != 0) & 0xfeU) + 5);
            if (local_996 != '\0') {
                local_996 = cVar4;
            }
            if (local_998 != '\0') {
                local_998 = cVar4;
            }
            if (local_995 != 0x3f) {
                local_898 = (char *)local_858;
                local_93c = (int)(char)local_858[0];
            }
            res20[0] = (byte)((-(local_888 != 0) & 0xf9U) + 0xc);
            local_8ac = 0;
            if (local_888 == 0) {
                local_90c = 1;
            }
            PECMD_FreeStrBuf((WCHAR **)&local_918);
            uVar12 = local_970;
            goto LAB_1400ce5a6;
        }
        if ((local_996 == '\x03') || (local_998 == '\x03')) {
            if (local_996 != '\0') {
                local_996 = local_996 + '\x01';
            }
            if (local_998 != '\0') {
                local_998 = local_998 + '\x01';
            }
            local_90c = 1;
        } else {
            if ((local_996 == '\x04') || (local_998 == '\x04')) {
                res20[0] = 0xc;
            } else {
                if ((local_996 != '\x05') && (local_998 != '\x05')) goto LAB_1400d0083;
                res20[0] = 0xb;
            }
            if (local_996 != '\0') {
                local_996 = local_996 + '\x01';
            }
            if (local_998 != '\0') {
                local_998 = local_998 + '\x01';
            }
        }
    }
    PECMD_FreeStrBuf((WCHAR **)&local_918);
    uVar12 = local_970;
    goto LAB_1400ce5a6;

    /* ================= SEG8: 终局直接 DDD 应用与清理退出 (@128098) ================= */
LAB_1400d0083:
    if (((int)uVar39 < 1) ||
        ((((-1 < (int)local_948 || (local_98c != 0)) || (local_997 != 0)) ||
          (local_97c != '\0')))) {
        PECMD_FreeStrBuf((WCHAR **)&local_918);
        PECMD_FreeStrBuf((WCHAR **)&local_8c0);
        PECMD_FreeStrBuf(&local_8d0);
        PECMD_FreeStrBuf(&local_958);
        PECMD_FreeStrBuf(&local_900);
        goto LAB_1400d043f;
    }
LAB_1400d00bd:
    if (local_98c == 0) {
LAB_1400d00c1:
        if (((int)uVar8 < 0) || ((int)uVar39 < 1)) {
            PECMD_FreeStrBuf((WCHAR **)&local_918);
            PECMD_FreeStrBuf((WCHAR **)&local_8c0);
            PECMD_FreeStrBuf(&local_8d0);
            PECMD_FreeStrBuf(&local_958);
            PECMD_FreeStrBuf(&local_900);
            goto LAB_1400ce434;
        }
    }
    pWVar27 = local_870;
    if ((-1 < (int)uVar8) && (0 < (int)uVar39)) {
        wsprintfW(local_958, (LPCWSTR)L"\\Device\\Harddisk%d\\Partition%d",
                  (int)(short)uVar8, (int)(short)uVar39);
        wsprintfW(pWVar27, (LPCWSTR)L"\\\\.\\PhysicalDrive%d", (int)(short)uVar8);
        local_870 = (WCHAR *)0;
        PECMD_OpenFileHandle((HANDLE *)&local_870, (LPCWSTR)pWVar27, 0x80000000U, 3,
                             (LPSECURITY_ATTRIBUTES)0, 3, 0x20000000U, (HANDLE)0);
        pWVar27 = local_870;
        local_8dc = 0;
        res20[0] = 0xff;
        puVar20 = PECMD_UpdatePartitionLayout((HANDLE)pWVar27, (uint64_t *)&local_918,
                                              &local_8dc, &res20[0], 0);
        iVar7 = 0;
        if (-1 < (char)res20[0]) {
            if (0 < local_8dc) {
                puVar26 = puVar20 + 9;
                do {
                    if ((int)(short)*puVar26 == (int)uVar39) {
                        PECMD_FindVolumeByDeviceId(
                            (uint32_t *)(void *)((byte *)puVar20 + (longlong)iVar7 * 0x90 + 0x30),
                            (int64_t *)&local_958, (LPWSTR)1);
                        if (-1 < iVar7) goto LAB_1400d024b;
                        break;
                    }
                    iVar7 = iVar7 + 1;
                    puVar26 = puVar26 + 0x12;
                } while (iVar7 < (int)local_8dc);
            }
            if (pWVar27 != (WCHAR *)0) {
LAB_1400cff4b:
                if (pWVar27 != (WCHAR *)0xffffffffffffffff) {
                    CloseHandle((HANDLE)pWVar27);
                }
            }
            PECMD_FreeStrBuf((WCHAR **)&local_918);
            PECMD_FreeStrBuf((WCHAR **)&local_8c0);
            PECMD_FreeStrBuf(&local_8d0);
            PECMD_FreeStrBuf(&local_958);
            PECMD_FreeStrBuf(&local_900);
LAB_1400cfff4:
            LeaveCriticalSection(&g_csDisk);
            return rbx_ret;
        }
LAB_1400d024b:
        if ((pWVar27 != (WCHAR *)0) && (pWVar27 != (WCHAR *)0xffffffffffffffff)) {
            CloseHandle((HANDLE)pWVar27);
        }
    }
    iVar7 = local_950;
    local_900[1] = L':';
    if (*local_900 == L',') {
        WVar6 = L'，';
LAB_1400d02ac:
        *local_900 = WVar6;
    } else {
        if (*local_900 == L';') {
            WVar6 = L'；';
            goto LAB_1400d02ac;
        }
        if (*local_900 == L':') {
            WVar6 = L'：';
            goto LAB_1400d02ac;
        }
        if (*local_900 == L'=') {
            WVar6 = L'＝';
            goto LAB_1400d02ac;
        }
    }
    local_900[2] = L'\\';
    local_900[3] = L'\0';
    final_letter[0] = *local_900;
    final_letter[1] = 0x3a;
    final_letter[2] = 0;
    if ((*local_958 == L'\0') || (local_98c != 0)) {
        if ((local_950 != 0) &&
            ((BVar13 = DeleteVolumeMountPointW((LPCWSTR)local_900), BVar13 == 0))) {
            GetLastError();
        }
        BVar13 = DefineDosDeviceW(local_944 | 2, (LPCWSTR)final_letter,
                                  (LPCWSTR)(-(uint64_t)(*local_958 != L'\0') &
                                            (uint64_t)(intptr_t)local_958));
        if (BVar13 == 0) {
            rbx_ret = (longlong)GetLastError();   /* 原体 rax=rbx 承载错误码 */
        } else {
            rbx_ret = 0;
        }
        if (iVar7 != 0) {
            PECMD_DeleteDriveMountPoint(final_letter[0]);
        }
    } else {
        BVar13 = PECMD_DosDeviceMount((LPCWSTR)local_958, (LPCWSTR)final_letter,
                                      (WCHAR *)local_900, local_944, (char)local_950);
        if (BVar13 == 0) {
            GetLastError();
        }
        rbx_ret = (longlong)local_8dc;   /* movsxd rbx,[rsp+0xa0] TODO(verify) */
    }
    PECMD_FreeStrBuf((WCHAR **)&local_918);
    PECMD_FreeStrBuf((WCHAR **)&local_8c0);
    PECMD_FreeStrBuf(&local_8d0);
    PECMD_FreeStrBuf(&local_958);
    PECMD_FreeStrBuf(&local_900);
LAB_1400d043f:
    LeaveCriticalSection(&g_csDisk);
    return rbx_ret;

    /* ================= 清理退出路径 ================= */
LAB_1400ce40d:
    PECMD_FreeStrBuf(&local_8d0);
    PECMD_FreeStrBuf(&local_958);
    PECMD_FreeStrBuf(&local_900);
LAB_1400ce434:
    LeaveCriticalSection(&g_csDisk);
    return rbx_ret;
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
