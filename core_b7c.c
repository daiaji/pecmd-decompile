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

/* 前向声明: 本文件 TABL 段私有 thunk_FUN_1400f429c (定义于文件后部) */
static void b7c_skip_to(WCHAR **pp, WCHAR c);

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


/* ---- HASH 命令 (PECMD_HashCmdCompute @0x1400c0ad8) 新增依赖 ---- */
extern uint64_t FUN_14005c7c4(const char *a, const uint16_t *b);   /* @0x14005c7c4 ANSI 词前缀匹配 */
extern LARGE_INTEGER PECMD_SetFilePointer(HANDLE h, LARGE_INTEGER dist, DWORD method); /* @0x14005c674 定位取长 */
extern void *PECMD_MapFileView(HANDLE h, int64_t size, uint32_t prot, int64_t offset); /* @0x1400e3f80 文件映射视图 */
extern uint32_t PECMD_CryptoHashCompute(BYTE *data, DWORD len, uint32_t alg, uint64_t extra); /* @0x1400e4864 CryptAPI 哈希 */
extern uint32_t PECMD_Crc32HexOfBytes(const uint8_t *data, int64_t len, char *out); /* @0x1400e4cc0 CRC32 */
extern short  *PECMD_UnquoteString(short *s);                      /* @0x14001be14 剥成对引号 */
extern void    FUN_14006355c(void *out, const WCHAR *src, int c, uint64_t d); /* @0x14006355c wide→ANSI 槽 */
extern void    FUN_1400BEF64(LPCWSTR cmd);                         /* @0x1400bef64 蜂鸣并执行文本 */
extern HANDLE  PECMD_OpenFileHandle(HANDLE *out, LPCWSTR path, DWORD access, DWORD share,
                                    LPSECURITY_ATTRIBUTES sa, DWORD disp, DWORD flags,
                                    HANDLE tmpl);                  /* @0x140003864 CreateFileW 包装 */

/* ========== PECMD_HashCmdCompute @0x1400c0ad8 ==========
 * 'HASH' 命令引擎 (decompiled.c @119159 size=1318 忠实直移,
 * 分块/收尾路径经 objdump -d 于原始 EXE 核对寄存器流向)。
 *
 * 行格式: [$|*|&]路径,%输出变量%[,算法]   算法缺省 MD5(CALG 0x8003)。
 *   '$' 数据源为串本体(转 ANSI); '*'/'&' 为具名内存块
 *   (FUN_140018978, 持全局锁 DAT_14013e190 直至收尾);
 *   其余按文件处理: 剥引号 → GENERIC_READ 打开 → SetFilePointer(FILE_END)
 *   取长度 → PECMD_MapFileView 映射。
 * 输出: " <摘要大写十六进制>" 写入宽串槽; 有变量名则 SetVariable/
 *   param_3 直写(跳过首空格), 否则 FUN_1400bef64 执行原文。
 * 返回: [rsp+0x40] 槽 — 仅 >4GB 分块哈希时为末次调用返回值, 其余为 0。
 * TODO(verify): list/CRC32 大文件分支不进入分块循环 (与原体一致)。
 */
void *PECMD_HashCmdCompute(int64_t *a1, LPCWSTR a2, int64_t *a3)
{
    WCHAR *cur = (WCHAR *)(uintptr_t)a2;
    char   cVar12;                        /* 输出格式旗标: 0 hex /1 CRC32 /-1 list */
    char   cVar14;                        /* '*'/'&' 内存块模式 */
    char   flag_dollar;                   /* '$' 串模式 ([rsp+0x198]) */
    WCHAR *pWVar5;
    WCHAR *tail_alg;                      /* 首个 StrRChrW 尾字段 = 算法名 */
    WCHAR *varname;                       /* 第二个尾字段 = 输出变量名 */
    uint32_t AVar17;                      /* ALG_ID */
    char  *hash_out;                      /* [rsp+0x58]: 摘要输出指针 (list 时=&list_alloc) */
    char  *res_src;                       /* [rsp+0x48]: 送 StrBldCopyAnsi 的 ANSI 串 */
    char  *list_alloc;                    /* local_128: list 模式分配的行缓冲槽 */
    uint64_t ansi_slot;                   /* local_118 ($ 模式 ANSI 转换槽) */
    BYTE  *data;                          /* r12: 数据基址 */
    longlong size;                        /* rbp: 数据长度 */
    void  *mapbase;                       /* r14: 待 Unmap 的映射基址 (0=无) */
    CRITICAL_SECTION *kept_cs;            /* rdi: 跨收尾持有的锁 */
    uint32_t retval;                      /* [rsp+0x40] 返回槽 */
    /* hexbuf 区: 前 0x18 字节供分块流式句柄槽 (out-0x18/out-0x10),
     * hexbuf 自 hb+0x17 起, hexbuf+1 恰 8 字节对齐。 */
    static uint64_t hb_align[(0x18 + 176) / 8];
    char *hexbuf = (char *)hb_align + 0x17;

    cur = (WCHAR *)a2;
    retval = 0;
    data = NULL;
    mapbase = NULL;
    kept_cs = NULL;
    list_alloc = NULL;
    ansi_slot = 0;
    size = 0;

    FUN_14005B154(&cur);
    flag_dollar = (char)(*cur == L'$');
    if ((*cur == L'*') || (*cur == L'&')) {
        cVar14 = '\x01';
    }
    else {
        cVar14 = '\0';
    }
    pWVar5 = cur;
    if ((*cur == L'$') || (*cur == L'*')) {
        pWVar5 = cur + 1;
    }
    tail_alg = (WCHAR *)StrRChrW(pWVar5, NULL, L',');
    if (tail_alg != NULL) {
        *(WCHAR *)tail_alg = L'\0';
        tail_alg = tail_alg + 1;
    }
    varname = (WCHAR *)StrRChrW(pWVar5, NULL, L',');
    if (varname != NULL) {
        *(WCHAR *)varname = L'\0';
        varname = varname + 1;
    }
    if (varname == NULL) {
        varname = tail_alg;               /* 仅一个尾字段: 视作变量名 */
        tail_alg = NULL;
    }

    hash_out = hexbuf + 1;
    res_src = hexbuf;
    hexbuf[0] = ' ';
    hexbuf[1] = '\0';
    cVar12 = '\0';
    AVar17 = 0x8003u;                     /* CALG_MD5 缺省 */
    if (tail_alg != NULL) {
        if (FUN_14005c7c4("SHA1", (const ushort *)tail_alg)) {
            AVar17 = 0x8004u;
        }
        else if (FUN_14005c7c4("SHA256", (const ushort *)tail_alg)) {
            AVar17 = 0x800cu;
        }
        else if (FUN_14005c7c4("SHA384", (const ushort *)tail_alg)) {
            AVar17 = 0x800du;
        }
        else if (FUN_14005c7c4("SHA512", (const ushort *)tail_alg)) {
            AVar17 = 0x800eu;
        }
        else if (FUN_14005c7c4("CRC32", (const ushort *)tail_alg)) {
            cVar12 = '\x01';
        }
        else if (FUN_14005c7c4("list", (const ushort *)tail_alg)) {
            hash_out = (char *)&list_alloc;
            AVar17 = 0xffffcfc7u;
            cVar12 = '\xff';
        }
        /* CRC32/list 命中才回写旗标字节 (local_res10 低字节, 此处以独立变量承接) */
    }

    EnterCriticalSection((CRITICAL_SECTION *)g_csInit);       /* DAT_14013e190 */
    kept_cs = (CRITICAL_SECTION *)g_csInit;
    if (cVar14 == '\0') {
        LeaveCriticalSection((CRITICAL_SECTION *)g_csInit);
        kept_cs = NULL;
    }

    if (flag_dollar == '\0') {
        if (cVar14 == '\0') {
            /* ---------- 文件模式 ---------- */
            HANDLE hFile = (HANDLE)0;
            WCHAR *path = (WCHAR *)PECMD_UnquoteString((short *)pWVar5);
            PECMD_OpenFileHandle(&hFile, path, 0x80000000u /*GENERIC_READ*/,
                                 3 /*FILE_SHARE_READ|WRITE*/, (LPSECURITY_ATTRIBUTES)0,
                                 3 /*OPEN_EXISTING*/, 0, (HANDLE)0);
            if (hFile == (HANDLE)0) {
                res_src = hexbuf;         /* 失败: 结果 = " " */
                goto LAB_1400c0f05_out;
            }
            {
                LARGE_INTEGER zero;
                LARGE_INTEGER szv;
                zero.QuadPart = 0;
                szv = PECMD_SetFilePointer(hFile, zero, 2 /*FILE_END*/);
                size = szv.QuadPart;
            }
            data = (BYTE *)PECMD_MapFileView(hFile, size, 2, 0);
            mapbase = data;
            if (data == NULL) {
                if (hFile != INVALID_HANDLE_VALUE) {
                    CloseHandle(hFile);
                }
                mapbase = NULL;           /* r14 = rax = 0 */
                res_src = hexbuf;
                goto LAB_1400c0f05_out;
            }
            if ((cVar12 > '\0') || ((ulonglong)size <= 0xffffffffULL)) {
                /* 单次哈希路径 (≤4GB 或 CRC32): 句柄即关, 收尾统一算 */
                if (hFile != INVALID_HANDLE_VALUE) {
                    CloseHandle(hFile);
                }
                mapbase = data;
                goto LAB_1400c0eb3_tail;
            }
            /* ---------- >4GB SHA 分块流式循环 ---------- */
            {
                uint64_t remaining = (ulonglong)size;
                BYTE *cursor = data;
                ((void **)hb_align)[0] = 0;          /* out-0x18 prov 槽清零 */
                ((void **)hb_align)[1] = 0;          /* out-0x10 hash 槽清零 */
                do {
                    longlong chunk = 0xffffffffLL;
                    char cf = '\x03';
                    uint32_t rv;
                    if (remaining <= (ulonglong)0xffffffffULL) {
                        chunk = (longlong)remaining;
                        cf = '\x01';      /* 末块: extra 低位 1 → 终结 */
                    }
                    rv = PECMD_CryptoHashCompute(cursor, (DWORD)chunk, AVar17,
                            (uint64_t)(intptr_t)hash_out + (uint64_t)(byte)cf);
                    if ((byte)cVar12 >= 0x80) {      /* cmovl: list 模式切换结果串 */
                        res_src = list_alloc;
                    }
                    retval = rv;          /* [rsp+0x40] */
                    cursor = cursor + chunk;
                    remaining -= (ulonglong)chunk;
                } while (remaining != 0);
            }
            if (hFile != INVALID_HANDLE_VALUE) {
                CloseHandle(hFile);
            }
            mapbase = data;               /* r14 ← [rsp+0x198] */
            data = NULL;                  /* r12 清零: 跳过单次哈希 */
            goto LAB_1400c0eb3_tail;
        }
        else {
            /* ---------- 具名内存块模式 (* / &) ---------- */
            int64_t blk = FUN_140018978(a1, (const uint16_t *)pWVar5,
                                        (int64_t *)0, ~(longlong)0, (void *)0);
            if (blk == 0) {
                res_src = hexbuf;
                goto LAB_1400c0f05_out;
            }
            size = *(int64_t *)(intptr_t)(blk + 0x18) & 0x3fffffffffffffffLL;
            data = *(BYTE **)(intptr_t)(blk + 8);
            goto LAB_1400c0eb3_tail;
        }
    }
    else {
        /* ---------- '$' 串模式: wide→ANSI ---------- */
        FUN_14006355c(&ansi_slot, pWVar5, ~0, ~(uint64_t)0);
        data = (BYTE *)(uintptr_t)ansi_slot;
        size = (longlong)lstrlenA((LPCSTR)(uintptr_t)ansi_slot);
        goto LAB_1400c0eb3_tail;
    }

LAB_1400c0eb3_tail:
    /* ---------- 单次摘要计算公共尾 ---------- */
    if (data != NULL) {
        if ((signed char)cVar12 > 0) {
            /* CRC32: 全长 64 位直送 */
            PECMD_Crc32HexOfBytes((const uint8_t *)data, size, hash_out);
        }
        else {
            PECMD_CryptoHashCompute(data, (DWORD)size, AVar17,
                                    (uint64_t)(intptr_t)hash_out);
            if ((byte)cVar12 >= 0x80) {
                res_src = list_alloc;     /* list 模式切换结果串 */
            }
        }
    }

LAB_1400c0f05_out:
    /* ---------- 结果落槽 / 变量 / 执行 ---------- */
    {
        int64_t wslot = 0;                /* local_res10 复用的宽串槽 */
        PECMD_StrBldCopyAnsi(&wslot, res_src, ~(uint64_t)0);
        if (a3 != (int64_t *)0) {
            PECMD_AssignString(a3, (LPCWSTR)(uintptr_t)(wslot + 2));  /* 跳过首空格 */
        }
        else if ((varname != NULL) && (*varname != L'\0')) {
            FUN_1400629B8((void *)a1, (LPCWSTR)varname,
                          (LPCWSTR)(uintptr_t)(wslot + 2));
        }
        else {
            FUN_1400BEF64((LPCWSTR)(uintptr_t)wslot);
        }
        PECMD_FreeStrBuf((WCHAR **)&wslot);
    }
    if (kept_cs != NULL) {
        LeaveCriticalSection(kept_cs);
    }
    if (mapbase != NULL) {
        UnmapViewOfFile(mapbase);
    }
    PECMD_FreeStrBuf((WCHAR **)&ansi_slot);
    PECMD_FreeStrBuf((WCHAR **)&list_alloc);
    return (void *)(uintptr_t)retval;
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
    (void)a3;
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


/* ====================================================================
 * ENVI 扩展内存读写引擎 (PECMD_EnviMemReadWrite @0x1400c6324)
 * decompiled.c @122536 size=12745 忠实移植 (1848 行 → 结构化转写)。
 *
 * 行格式: ENVI [~][&...]名称[=值][,参数2]  + param_4 开关族
 *   字符开关: '*'(位2|4) '~~'(位2) '~'(位1) '~.'(位8) '~*'(位0x10)
 *             '<'前插 '>'后插 '.'/'$'/'%'跳过
 *   词开关:  -std(读宽,元素1) -env(ANSI→宽,2) -raw(写块,4) -def(块长,8)
 *            -tom(内存比较,0x100) -cmp(指针直取,0x20)
 *            -make(0x80)/-tow(0x81)/-copy(0x83) 具名块写入
 *            -addr/-ex 地址表达式  -swap(l1a0|=0x40) -zero(|=0x80)
 *            -ret/-ex计数+2  -get 清填充  -x N 展开重复
 *            -[起:止:步 多行项  -mkfixdummy/-mkdummy 父链深度
 *            -*类型[:域] / ?类型[:域] 结构域取址 (char/wchar/short/ptr/
 *            intptr/intpe/float/double/ldouble/int64/longlong/long/intsys/int)
 * 返回: HRESULT 型槽 (0 成功 / 0x80070057 参数错)。
 * TODO(verify): decompiled.c 对 dash 开关族的 else 配对与 objdump 反汇编
 *   存在错位嫌疑 (二进制中 -std/-env 写 local_1a0 位型等); 本移植按
 *   decompiled 文本配对保守还原, 待后续以反汇编逐项复核。
 * ==================================================================== */

/* ---- ENVI 引擎新增依赖 (实现位置) ---- */
extern int      FUN_14005C788(const char *s, const WCHAR *w, int n);  /* @0x14005c788 ASCII 前缀词匹配真体 (core_string.c) */
extern LPCWSTR  PECMD_StripTrailingSpaces(LPCWSTR s);                 /* @0x140018b70 去尾空格 (core_b1_remaining.c) */
extern uint64_t PECMD_EvalExprSkipOneChar(WCHAR **pp, uint64_t *out); /* @0x1400a9a84 取值并跳一字符 (core_remaining_helpers.c) */
extern void    *PECMD_CheckVarMultiLevelRef(int64_t *tbl, LPCWSTR name); /* @0x1400681ec 多级变量引用 (core_b3_remaining.c) */
extern int64_t  PECMD_DispatchByObjectName(uint64_t *mgr, LPCWSTR name, int64_t len,
                                           uint16_t *a4, uint64_t a5, uint64_t a6);  /* @0x14005eb80 (core_b3_remaining.c) */
extern uint64_t *PECMD_ExpandVarsLocked(int64_t *script, int64_t *out, uint8_t flags); /* @0x14008569c 锁内展开 (core_b3m.c) */
extern LPCWSTR  PECMD_ApplyVarWriteModifiers(int64_t *pslot, int64_t *plen,
                                             uint16_t *mods, int64_t *plen2); /* @0x140084a5c 写修饰符 (core_b3_remaining.c) */
extern uint64_t PECMD_ParseNumOrVar(int64_t *pp, uint64_t *out, int64_t *script); /* @0x1400746b0 数字/变量解析 (core_b3j.c) */
extern void     PECMD_VarTruncateUpdate(void *node, const void *src, uint64_t len); /* @0x14005b708 (core_var2.c) */
extern void     PECMD_VarWriteValueCap(WCHAR **pval, uint64_t *pcap, const void *src, int64_t len); /* @0x140066224 (core_var2.c) */
extern uint8_t *PECMD_MemMoveSafe(void *dst, longlong src, longlong n);  /* @0x14001d744 memmove (link_stubs.c) */
extern uint64_t PECMD_GrowByteBuffer(void *pslot, longlong len);      /* @0x140063424 缓冲增长 (link_stubs.c) */
extern void    *PECMD_VarLookup(void *script, LPCWSTR name, void *scope, int namelen, void **found); /* @0x140018978 变量查找真体 (core_var2.c) */
extern uint64_t PECMD_AddVarDefault(void *script, LPCWSTR name, LPCWSTR val, int len, int64_t cap); /* @0x14001e5b0 (link_stubs.c) */
extern uint16_t *PECMD_ExpandPercentD(uint16_t *dst, WCHAR *src);     /* @0x14005eefc 展开 %% 与 %d (link_stubs.c) */
extern int      PECMD_AnsiStrNCompare(char *a, longlong b, longlong c); /* @0x14005b184 ANSI 串比较 (link_stubs.c) */
extern WCHAR   *PECMD_FormatU64RetEnd(WCHAR *dst, uint64_t v, LPCWSTR fmt);    /* @0x1400e6db4 (core_strbld.c) */
extern WCHAR   *PECMD_FormatDblRetEnd(WCHAR *dst, float v, LPCWSTR fmt);       /* @0x1400e6d80 (core_strbld.c) */
extern bool     FUN_1400C1194(int64_t *pp, uint64_t *out);            /* @0x1400c1194 表达式→整数 */
extern uint64_t FUN_140067b54(uint16_t *pcursor, double *out);        /* @0x140067b54 数词解析 */
extern longlong FUN_140063b00(longlong idx, longlong *pdata, longlong *pcapa, uint32_t elemsz); /* 向量槽 */
extern longlong *FUN_140063b64(longlong *v);                          /* 向量初始化 */
extern void     FUN_14001d78c(unsigned char *dst, const unsigned char *src, longlong n); /* memcpy 库替换 */
extern void     FUN_1400703e4(void *slot, LPCWSTR src);               /* @0x1400703e4 串槽拷贝 */
extern int      lstrcmpA(LPCSTR, LPCSTR);                             /* Win32 (未在 stub 头声明) */
extern int      StrCmpNA(LPCSTR, LPCSTR, int);                        /* Win32 (未在 stub 头声明) */
extern uint32_t DAT_14013a038;                                       /* g_bX64 (link_stubs.c 定义) */
extern uint8_t  DAT_14011d308[];                                     /* @0x14011d308 缺省串区 (link_stubs.c 定义) */

/* $ 移位取值 (@0x1400857e4 带 RAX 直传返回值变体; core_b3f.c 版省略了返回值,
 * 本函数依赖其成功标志 → 按原体在本文件内忠实补齐, 底层调用真体 PECMD_EvalExprSkipOneChar) */
static int64_t FUN_1400857E4(int64_t *pp, uint64_t *out, uint8_t *shift)
{
    int iVar1;
    if (*(short *)(intptr_t)*pp == 0x24) {
        shift[0] = 0;
        shift[1] = 0;
        while (*(short *)(intptr_t)*pp == 0x24) {
            *(int64_t *)shift = *(int64_t *)shift + 1;
            *pp = (int64_t)((WCHAR *)(intptr_t)*pp + 1);
        }
    }
    iVar1 = (int)PECMD_EvalExprSkipOneChar((WCHAR **)pp, out);
    if ((0 < iVar1) && (0 < *(int64_t *)shift)) {
        *out = *out << (*shift & 0x3f);
    }
    return (int64_t)iVar1;
}
                                        /* g_bX64 (link_stubs.c 定义) */
extern int (*DAT_14013c970)(uint32_t, uint32_t, char *, int);         /* 转换回调槽 (link_stubs.c 定义, 调用点按 6 参原型强转) */

/* ---------- -*-/? 共用: 类型名后置修饰 s/0/@ 处理 (decompiled 直移) ---------- */

/* ========== PECMD_EnviMemReadWrite @0x1400c6324 ========== */
void *PECMD_EnviMemReadWrite(WCHAR *a1, void *a2, uint64_t a3,
                             WCHAR *a4, char a5)
{
    (void)a3;
    /* ---- 寄存器/槽位镜像 (名称对应 decompiled local_*) ---- */
    uint64_t *r1_lenptr;              /* ppppppppppppWVar1: 记录长度域指针 */
    bool bVar2;                       /* -tom 负计数旗标 */
    uint8_t bVar3;
    ushort uVar4;
    int iVar5;
    int iVar6 = 1;                    /* 返回值跨项寄存器 (循环间保持) */
    int64_t *plVar8;
    int64_t *plVar9;
    ulonglong uVar10;
    longlong lVar11;
    longlong lVar15;
    WCHAR *pWVar12;                   /* 名称查表作用域基址 */
    LPCWSTR pWVar13;
    ulonglong *puVar14;
    unsigned char *puVar16;
    int64_t v17;                      /* ppppppppppppWVar17 多用槽 */
    int64_t v18;                      /* ppppppppppppWVar18 多用槽 */
    LPCWSTR lpStr2;
    LPCWSTR pwVar19;
    int iVar20 = 1;
    WCHAR *pWVar21;
    WCHAR *pWVar22;
    WCHAR WVar23;
    WCHAR *pWVar24;
    WCHAR *pWVar25;                   /* 开关扫描游标 */
    int64_t v26;                      /* ppppppppppppWVar26 */
    ulonglong *puVar27;
    int64_t r28 = 0;                  /* ppppppppppppWVar28 返回值槽/向量下标 */
    int64_t v29 = 0;                  /* ppppppppppppWVar29 长度槽 */
    WCHAR *pWVar30;                   /* 零标记/作用域镜像 */
    int64_t v31 = 0;                  /* ppppppppppppWVar31 */
    int64_t v32 = 0;                  /* ppppppppppppWVar32 元素尺寸/模式 */
    WCHAR WVar33;
    bool bVar34 = false;              /* 直取缓冲旗标 */
    int64_t v35 = 0;                  /* ppppppppppppWVar35 值位型槽 */
    int64_t local_res10 = 0;          /* 低字节: 未给 '=' 旗标; 整槽兼解析出参/串槽 */
    uint8_t local_327 = 0;            /* ~ 与 * 修饰位: 1|2|4|8|0x10 */
    WCHAR *local_320 = 0;             /* 当前值指针 */
    char local_318 = 0;               /* '<'=1 / '>'=2 前后插 */
    char local_317;                   /* 深度计数 (param_5 + '&') */
    WCHAR *local_310 = 0;             /* 作用域保存 */
    WCHAR *local_308 = 0;             /* 名称起点/行游标 */
    WCHAR *local_300 = 0;             /* 分配槽 */
    int64_t local_2f8 = 0;            /* 写尺寸选择 */
    WCHAR *local_2f0 = 0;             /* 第二 token 槽 */
    WCHAR *local_2e8 = 0;             /* 表达式串槽 */
    WCHAR *local_2e0 = 0;             /* 环境表 A (名称查表) */
    int64_t local_2d8 = 0;            /* 解析出参/长度 */
    WCHAR *local_2d0 = 0;             /* 展开出参槽 */
    WCHAR *local_2c8 = 0;             /* 多行拼接缓冲 */
    char local_2c0 = 0;               /* -addr/-ex 旗标 */
    WCHAR *local_2b8 = 0;             /* 游标兼记录指针 */
    int64_t local_2b8_num = 0;        /* 数值暂存 (与 local_2b8 同槽复用) */
    int64_t local_1a8 = 0;            /* 暂存槽 (decompiled local_1a8) */
    int64_t local_2b0 = 0;            /* 数值暂存 */
    LPCWSTR local_2a8 = WSTR("%I64d");     /* 格式串 */
    int64_t local_2a0 = 0;            /* ?路径尺寸/0x2a 标记 */
    int8_t local_298 = 0;             /* 深度>1 旗标 */
    int64_t local_290 = 0;            /* 读模式选择 */
    WCHAR *local_288 = 0;             /* 主游标 */
    int64_t local_280 = 0;            /* 数值暂存 */
    int local_278 = -1;               /* -get 填充值 (<0 非填充) */
    int64_t local_270 = 0;            /* 输出宽度类别 */
    int64_t local_268 = 0;            /* 偏移暂存 */
    longlong local_260 = 0;           /* 位偏累计 */
    WCHAR *local_258 = 0;             /* 环境表 B (数据源) */
    int64_t local_250 = 0;
    int64_t local_248 = 0;
    WCHAR *local_240 = 0;             /* 第一 token 槽 */
    int64_t local_238 = 0;            /* 字节长度出参 */
    uint64_t local_230 = 0;           /* double 位型 */
    int64_t local_228 = 0;            /* 出参槽 A (整/址双用) */
    int64_t local_220 = 0;            /* 止索引/尺寸 */
    int64_t local_218 = 0;            /* -mkdummy 深度 */
    ulonglong local_210 = 0;          /* 填充值位型 */
    int64_t local_208 = 0;            /* 向量条数 */
    int64_t local_200 = 0;            /* 向量数据指针 */
    longlong local_1f8 = 0;           /* 向量容量 */
    longlong local_1f0 = 0;           /* 向量条数 */
    int64_t local_1e8 = 0;            /* 拼接源 A */
    longlong local_1e0 = -1;          /* 多行项数 */
    int64_t local_1d8 = 1;            /* 步长 */
    longlong local_1d0 = 0;           /* float 尺寸标记 */
    longlong local_1c8 = 0;           /* ldouble 标记 */
    longlong local_1c0 = 0;           /* double 标记 */
    ulonglong local_1b8 = 0;          /* -x 展开重复数 */
    int64_t local_1b0 = 0;            /* 起始索引游标 */
    int64_t local_1a0 = 1;            /* -std/-env 位型写入槽 (0x40/0x80) */
    LPCWSTR local_198 = 0;            /* ~名~ 内部默认名 */
    ulonglong local_190 = 0;          /* 第二填充值 */
    WCHAR *local_188 = 0;             /* 拼接游标 */
    uint64_t *local_180 = 0;          /* 出参槽 B 指针 */
    WCHAR *local_178 = 0;
    ulonglong local_168[5];           /* 0x28 清零区 */
    WCHAR *local_140 = 0;             /* 拼接半区 B */
    WCHAR *local_138[2];
    WCHAR local_128[32];              /* wsprintf 缓冲 (原栈距 0x40 字节) */
    WCHAR local_e8[44];               /* 结果格式化缓冲 */
    union { uint64_t u64; uint8_t b[16]; } local_170_u;   /* 值位型缓冲 (原 &local_170 栈区) */
    WCHAR **param_2 = (WCHAR **)a2;
    WCHAR *const param_1 = a1;
    WCHAR *const param_4 = a4;

    local_198 = (LPCWSTR)0;
    local_318 = '\0';
    local_2c0 = '\0';
    local_1d0 = 0;
    local_1c0 = 0;
    local_1c8 = 0;
    local_1b8 = 0;
    local_2f8 = 0;
    local_2a0 = 0;
    local_270 = 0;
    local_1b0 = 0;
    local_228 = 0;
    local_220 = 0;
    local_2b0 = 0;
    local_280 = 0;
    local_1a0 = 1;
    local_2a8 = WSTR("%I64d");
    local_1e0 = -1;
    local_1d8 = 1;
    local_278 = -1;
    local_288 = *param_2;
    FUN_140063b64(&local_200);                       /* 向量 {data,capa=1,count=0} */
    FUN_14005B154(&local_288);                       /* 跳过前导空白/控制符 */
    local_317 = a5;
    if (*(ushort *)local_288 == 0x26) {              /* '&' 前缀: 逐个加深父链层级 */
        do {
            local_288 = local_288 + 1;
            local_317 = local_317 + '\x01';
        } while (*(ushort *)local_288 == 0x26);
    }
    PECMD_AllocStrSlot(&local_240);                  /* token1 槽 */
    pWVar13 = (LPCWSTR)local_288;                    /* 名称起点 */
    local_308 = local_288;
    WVar23 = *local_288;
    v17 = (int64_t)(intptr_t)local_288;
    while ((WVar23 != L'\0') && (WVar23 != L'=')) {
        v17 = (int64_t)(intptr_t)((WCHAR *)(intptr_t)v17 + 1);
        WVar23 = *(WCHAR *)(intptr_t)v17;
    }
    local_res10 &= ~(int64_t)0xff;                   /* 低字节清零 (保留高位语义) */
    local_320 = (WCHAR *)g_szEmpty;                  /* 缺省空值 */
    if (*(WCHAR *)(intptr_t)v17 == L'=') {
        local_320 = (WCHAR *)(intptr_t)v17 + 1;      /* 值 = '=' 之后 */
        local_288 = local_320;
    }
    else {
        local_res10 |= 1;                            /* 未给 '=' 旗标 */
        local_288 = (WCHAR *)(intptr_t)v17;
    }
    *(WCHAR *)(intptr_t)v17 = L'\0';                 /* 截断名称 */
    PECMD_StripTrailingSpaces(pWVar13);
    if (*(ushort *)local_308 == 0x7e) {              /* ~名称~默认名 形式 */
        v17 = (int64_t)(intptr_t)(local_308 + 1);
        for (local_308 = (WCHAR *)(intptr_t)v17;
             (*(ushort *)local_308 != 0) && (*(ushort *)local_308 != 0x7e);
             local_308 = local_308 + 1) {
        }
        if (*(ushort *)local_308 != 0x7e) {
LAB_1400c654d:
            PECMD_FreeStrBuf(&local_240);
            PECMD_FreeStrBuf((WCHAR **)&local_200);
            return (void *)(uintptr_t)0xffffffff80070057ULL;
        }
        *(ushort *)local_308 = 0;
        local_308 = local_308 + 1;
        local_198 = (*(WCHAR *)(intptr_t)v17 != 0) ? (LPCWSTR)(WCHAR *)(intptr_t)v17 : (LPCWSTR)0;
    }
    v18 = 0;
    v35 = 0;
    local_260 = 0;
    local_208 = 0;
    local_230 = 0;
    v17 = v18;                          /* -[ 起始索引继承 */
    v32 = v18;                          /* 模式槽继承 */
    pWVar12 = (WCHAR *)0;
    if (a4 != (WCHAR *)0) {
        pWVar22 = (WCHAR *)0;
        pWVar12 = a4;
        pWVar25 = a4;
        pWVar30 = pWVar22;
LAB_1400c65e2:
        /* 循环终止: NUL / 控制符 / 空格 */
        if ((*pWVar25 == (WCHAR)(uintptr_t)pWVar30) ||
            ((((ushort)*pWVar25 > 8) && ((ushort)*pWVar25 < 0xe)) || (*pWVar25 == L' '))) {
            goto LAB_1400c671a;
        }
        if (((*pWVar25 != L'.') && (*pWVar25 != L'$')) && (*pWVar25 != L'%')) {
            if (*pWVar25 == L'*') {
                local_327 = (uint8_t)(local_327 | 4);
                goto LAB_1400c670e;
            }
            if (*pWVar25 == L'~') {
                if (pWVar25[1] == L'~') {
                    local_327 = (uint8_t)(local_327 | 2);
                    pWVar25 = pWVar25 + 1;
                    goto LAB_1400c670e;
                }
                if (pWVar25[1] == L'.') {
                    local_327 = (uint8_t)(local_327 | 8);
                    pWVar25 = pWVar25 + 1;
                    goto LAB_1400c670e;
                }
                if (pWVar25[1] == L'*') {
                    local_327 = (uint8_t)(local_327 | 0x10);
                    pWVar25 = pWVar25 + 1;
                    goto LAB_1400c670e;
                }
                local_327 = (uint8_t)(local_327 | 1);
                goto LAB_1400c670e;
            }
            if (*pWVar25 == L'<') {
                local_318 = '\x01';
                goto LAB_1400c670e;
            }
            if (*pWVar25 == L'>') {
                local_318 = '\x02';
                goto LAB_1400c670e;
            }
            /* ======== 词开关族 (decompiled 嵌套链平铺为优先级阶梯) ======== */
            if (FUN_14005C788("-std", pWVar25, 4)) {
                local_290 = 1;
                v32 = r28;
                pWVar25 = pWVar25 + 3;
                goto LAB_1400c670e;
            }
            if (FUN_14005C788("-env", pWVar25, 4)) {
                local_290 = 2;
                v32 = 2;
                pWVar25 = pWVar25 + 3;
                goto LAB_1400c670e;
            }
            if (FUN_14005C788("-raw", pWVar25, 4)) {
                local_290 = 4;
                v32 = 4;
                pWVar25 = pWVar25 + 4;
                goto LAB_1400c670e;
            }
            if (FUN_14005C788("-def", pWVar25, 4)) {
                local_290 = 8;
                v32 = 8;
                pWVar25 = pWVar25 + 4;
                goto LAB_1400c670e;
            }
            if (FUN_14005C788("-tom", pWVar25, 4)) {
                v32 = 0x100;
                pWVar25 = pWVar25 + 4;
                local_290 = v32;
                goto LAB_1400c670e;
            }
            if (FUN_14005C788("-tow", pWVar25, 4)) {
                pWVar25 = pWVar25 + 7;
                v32 = 0x81;
                local_290 = v32;
                goto LAB_1400c670e;
            }
            if (FUN_14005C788("-copy", pWVar25, 5)) {
                pWVar25 = pWVar25 + 10;
                v32 = 0x83;
                local_290 = v32;
                goto LAB_1400c670e;
            }
            if (FUN_14005C788("-cmp", pWVar25, 4)) {
                local_290 = 0x20;
                v32 = 0x20;
                pWVar25 = pWVar25 + 4;
                goto LAB_1400c670e;
            }
            if (FUN_14005C788("-addr", pWVar25, 5)) {
                local_2c0 = '\x01';
                pWVar25 = pWVar25 + 2;
                goto LAB_1400c670e;
            }
            if (FUN_14005C788("-mkfixdummy", pWVar25, 0xb)) {   /* 父链深度 A → v18 */
                local_2b0 = 1;
                local_310 = pWVar25 + 4;
                if (*local_310 == L':') {
                    local_310 = pWVar25 + 5;
                }
                v18 = r28;
                if (((((ushort)*local_310 > 0x2f) && ((ushort)*local_310 < 0x3a)) ||
                     (*local_310 == L'('))) {
                    PECMD_EvalParenStripped(&local_310, (uint64_t *)&local_2b0);
                    v18 = local_2b0;
                }
                pWVar25 = local_310 + -1;
                goto LAB_1400c670e;
            }
            if (FUN_14005C788("-mkdummy", pWVar25, 8)) {        /* 父链深度 B → local_218 */
                local_310 = pWVar25 + 4;
                local_218 = 1;
                local_2b0 = 1;
                if (*local_310 == L':') {
                    local_310 = pWVar25 + 5;
                }
                if (((((ushort)*local_310 > 0x2f) && ((ushort)*local_310 < 0x3a)) ||
                     (*local_310 == L'('))) {
                    PECMD_EvalParenStripped(&local_310, (uint64_t *)&local_2b0);
                    local_218 = local_2b0;
                }
                pWVar25 = local_310 + -1;
                goto LAB_1400c670e;
            }
            if (FUN_14005C788("-make", pWVar25, 5)) {
                v32 = 0x80;
                pWVar25 = pWVar25 + 4;
                local_290 = v32;
                goto LAB_1400c670e;
            }
            if (FUN_14005C788("-swap", pWVar25, 5)) {
                local_1a0 = 0x40;
                pWVar25 = pWVar25 + 3;
                goto LAB_1400c670e;
            }
            if (FUN_14005C788("-zero", pWVar25, 5)) {
                local_1a0 = 0x80;
                pWVar25 = pWVar25 + 3;
                goto LAB_1400c670e;
            }
            if (FUN_14005C788("-ret", pWVar25, 4)) {
                local_317 = local_317 + '\x02';
                pWVar25 = pWVar25 + 3;
                goto LAB_1400c670e;
            }
            if (FUN_14005C788("-get", pWVar25, 4)) {
                local_278 = (int)(uintptr_t)pWVar30;            /* (=0) 清填充模式 */
                pWVar25 = pWVar25 + 4;
                goto LAB_1400c670e;
            }
            if (FUN_14005C788("-ex", pWVar25, 3)) {
                local_317 = local_317 + '\x02';
                pWVar25 = pWVar25 + 3;
                goto LAB_1400c670e;
            }
            if (FUN_14005C788("-x", pWVar25, 2)) {              /* 展开重复次数 */
                local_310 = pWVar25 + 2;
                local_1b8 = 1;
                PECMD_EvalParenStripped(&local_310, (uint64_t *)&local_1b8);
                pWVar25 = local_310 + -1;
                goto LAB_1400c670e;
            }
            if (FUN_14005C788("-[", pWVar25, 2)) {              /* 起:止:步 多行区间 */
                local_310 = pWVar25 + 2;
                PECMD_EvalExprSkipOneChar((WCHAR **)&local_310, (uint64_t *)&local_228);
                PECMD_EvalExprSkipOneChar((WCHAR **)&local_310, (uint64_t *)&local_220);
                v26 = local_228;
                local_1b0 = local_228;
                local_280 = local_228;
                PECMD_EvalParenStripped(&local_310, (uint64_t *)&local_280);
                PECMD_EvalParenStripped(&local_310, (uint64_t *)&local_1d8);
                if (local_1d8 < 1) {
                    local_1d8 = r28;
                }
                local_1e0 = local_220 + (1 - v26);
                pWVar25 = local_310;
                if (*local_310 != L']') {
                    pWVar25 = local_310 + -1;
                }
                goto LAB_1400c670e;
            }
            if ((*pWVar25 == L'-') && FUN_14005C788("*", pWVar25 + 1, 1)) {
                /* -------- -*类型[:结构域] -------- */
                pWVar24 = pWVar25 + 1;
                local_2f8 = 0x2a;                               /* '*' 标记 */
                pWVar13 = (LPCWSTR)(pWVar25 + 2);
                WVar33 = (WCHAR)(uintptr_t)pWVar30;
                WVar23 = *pWVar13;
                pWVar25 = (WCHAR *)(uintptr_t)pWVar13;
                while (WVar23 != WVar33) {
                    if (((((ushort)WVar23 > 8) && ((ushort)WVar23 < 0xe)) || (WVar23 == L' ')) ||
                        (WVar23 == L':')) {
                        if (*pWVar25 != WVar33) {
                            *pWVar25 = WVar33;                  /* 截断于分隔符 */
                        }
                        break;
                    }
                    pWVar25 = pWVar25 + 1;
                    WVar23 = *pWVar25;
                }
                local_138[0] = (WCHAR *)(uintptr_t)pWVar30;
                local_260 = PECMD_EnviStructFieldOffset((int64_t *)(intptr_t)&local_200, (int64_t *)a1,
                                                        pWVar13, (LPCWSTR)local_308,
                                                        (uint64_t *)(void *)local_138,
                                                        (int64_t *)(intptr_t)&local_208);
                pWVar12 = ((WCHAR *)0);                      /* 终结开关循环 */
                if (local_260 < 0) goto LAB_1400c654d;
                pWVar24 = (WCHAR *)WSTR("ptr");
                if (*local_138[0] != L'*') {
                    pWVar24 = local_138[0];
                }
                if (0 < local_208) {
                    local_308[local_208] = L'\0';               /* 截断字段名 */
                }
                /* 类型名分派 (decompiled 嵌套链直移) */
                if (FUN_14005C788("char", pWVar24, 4)) {
                    local_2f8 = 1;
                    pWVar25 = pWVar24 + 3;
                    goto LAB_1400c670e;
                }
                if (FUN_14005C788("wchar", pWVar24, 5) || FUN_14005C788("short", pWVar24, 5)) {
                    local_2f8 = 2;
                    pWVar25 = pWVar24 + 4;
                    goto LAB_1400c670e;
                }
                if (!FUN_14005C788("ptr", pWVar24, 3)) {
                    if (FUN_14005C788("intptr", pWVar24, 6)) {
                        local_2f8 = 8;
                        pWVar25 = pWVar24 + 2;
                        goto LAB_1400c670e;
                    }
                    if (FUN_14005C788("intpe", pWVar24, 5)) {
                        local_2f8 = 4;
                        pWVar25 = pWVar24 + 4;
                        goto LAB_1400c670e;
                    }
                    if (FUN_14005C788("float", pWVar24, 5)) {
                        local_1d0 = 4;
                        local_2f8 = 4;
                        pWVar25 = pWVar24 + 4;
                        goto LAB_1400c670e;
                    }
                    if (FUN_14005C788("double", pWVar24, 6)) {
                        local_1c0 = 8;
                        local_2f8 = 8;
                        pWVar25 = pWVar24 + 5;
                        goto LAB_1400c670e;
                    }
                    if (FUN_14005C788("ldouble", pWVar24, 7)) {
                        local_1c8 = 8;
                        local_2f8 = 8;
                        pWVar25 = pWVar24 + 6;
                        goto LAB_1400c670e;
                    }
                    if (FUN_14005C788("int64", pWVar24, 5)) {
                        local_2f8 = 8;
                        pWVar25 = pWVar24 + 4;
                        goto LAB_1400c670e;
                    }
                    if (FUN_14005C788("longlong", pWVar24, 8)) {
                        local_2f8 = 8;
                        pWVar25 = pWVar24 + 7;
                        goto LAB_1400c670e;
                    }
                    if (FUN_14005C788("long", pWVar24, 4)) {
                        local_2f8 = 4;
                        pWVar25 = pWVar24 + 3;
                        goto LAB_1400c670e;
                    }
                    if (FUN_14005C788("intsys", pWVar24, 6)) {
LAB_1400c754d:
                        local_2f8 = 4;
                        if (DAT_14013a038 == '\x03') {
                            local_2f8 = 8;
                        }
                        pWVar25 = pWVar24 + 5;
                        if (pWVar24[6] == L'0') {
                            local_2a8 = (DAT_14013a038 != '\x03') ? WSTR("0x%08I64X") : WSTR("0x%016I64X");
                            pWVar25 = pWVar24 + 6;
                        }
                        goto LAB_1400c670e;
                    }
                    if (FUN_14005C788("int", pWVar24, 3)) {
                        uVar4 = (ushort)(pWVar24[3] & 0xffdf);
                        if ((uVar4 == 0x53) || (uVar4 < 0x41) || (0x5a < uVar4)) {
                            /* int 且后继非字母(S 除外) */
                            local_2f8 = 4;
                            pWVar25 = pWVar24 + 2;
                            goto LAB_1400c670e;
                        }
                    }
                    pWVar25 = pWVar24;
                    goto LAB_1400c670e;
                }
                /* ptr 命中 */
                local_2f8 = 8;
                pWVar25 = pWVar24 + 5;
                goto LAB_1400c670e;
            }
            if (*pWVar25 == L'?') {
                /* -------- ?类型[:结构域] (decompiled 直移, 含 s/0/@ 后缀) -------- */
                pWVar24 = pWVar25 + 1;
                pWVar21 = pWVar22;
                if (FUN_14005C788("*", pWVar24, 1)) {
                    local_2a0 = 0x2a;
                    pWVar13 = (LPCWSTR)(pWVar25 + 2);
                    WVar33 = (WCHAR)(uintptr_t)pWVar30;
                    WVar23 = *pWVar13;
                    pWVar25 = (WCHAR *)(uintptr_t)pWVar13;
                    while (true) {
                        pWVar21 = pWVar22;
                        if (WVar23 == WVar33) break;
                        if (((((ushort)WVar23 > 8) && ((ushort)WVar23 < 0xe)) || (WVar23 == L' '))) {
                            if (*pWVar25 != WVar33) {
                                *pWVar25 = WVar33;
                            }
                            break;
                        }
                        pWVar21 = pWVar25;                  /* 第三析支副作用 */
                        if (WVar23 == L':') {
                            if (*pWVar25 != WVar33) {
                                *pWVar25 = WVar33;
                            }
                            break;
                        }
                        pWVar25 = pWVar25 + 1;
                        WVar23 = *pWVar25;
                    }
                    local_178 = (WCHAR *)(uintptr_t)pWVar30;
                    local_260 = PECMD_EnviStructFieldOffset((int64_t *)(intptr_t)&local_200, (int64_t *)a1,
                                                            pWVar13, (LPCWSTR)local_308,
                                                            (uint64_t *)&local_178,
                                                            (int64_t *)(intptr_t)&local_208);
                    pWVar12 = ((WCHAR *)0);              /* 终结开关循环 */
                    if (local_260 < 0) goto LAB_1400c883d;
                    pWVar24 = (WCHAR *)WSTR("ptr");
                    if (*local_178 != L'*') {
                        pWVar24 = local_178;
                    }
                    if (0 < local_208) {
                        local_308[local_208] = L'\0';
                    }
                }
                pWVar22 = pWVar21;
                if (FUN_14005C788("char", pWVar24, 4)) {
                    pWVar25 = pWVar24 + 3;
                    if (pWVar21 != (WCHAR *)(uintptr_t)pWVar30) {
                        pWVar25 = pWVar21;
                    }
                    local_2f8 = 1;
                    local_2a0 = 1;
                    if (pWVar25[1] == L's') {
                        v17 = r28;
                    }
                    local_2a8 = WSTR("0x%I64X");
                    local_270 = v17;
                    if (pWVar25[1] == L'0') {
                        local_2a8 = WSTR("0x%02I64X");
                        pWVar25 = pWVar25 + 1;
                        v17 = local_270;
                    }
                }
                else if (FUN_14005C788("wchar", pWVar24, 5)) {
                    pWVar25 = pWVar24 + 4;
                    if (pWVar21 != (WCHAR *)(uintptr_t)pWVar30) {
                        pWVar25 = pWVar21;
                    }
                    local_2a0 = 2;
                    local_2f8 = 2;
                    if (pWVar25[1] == L's') {
                        v17 = 2;
                    }
                    local_2a8 = WSTR("0x%I64X");
                    local_270 = v17;
                    if (pWVar25[1] == L'0') {
                        local_2a8 = WSTR("0x%04I64X");
                        pWVar25 = pWVar25 + 1;
                        v17 = local_270;
                    }
                }
                else if (FUN_14005C788("short", pWVar24, 5)) {
                    local_2a0 = 100;
                    pWVar25 = pWVar24 + 2;
                    if (pWVar21 != (WCHAR *)(uintptr_t)pWVar30) {
                        pWVar25 = pWVar21;
                    }
                    local_2f8 = 8;
                    local_2a8 = WSTR("0x%I64X");
                    if (pWVar25[1] == L'0') {
                        pWVar25 = pWVar25 + 1;
                    }
                }
                else if (!FUN_14005C788("ptr", pWVar24, 3)) {
                    if (!FUN_14005C788("intptr", pWVar24, 6)) {
                        if (!FUN_14005C788("float", pWVar24, 5)) {
                            if (!FUN_14005C788("double", pWVar24, 6)) {
                                if (!FUN_14005C788("ldouble", pWVar24, 7)) {
                                    if (!FUN_14005C788("int64", pWVar24, 5)) {
                                        if (!FUN_14005C788("longlong", pWVar24, 8)) {
                                            if (!FUN_14005C788("long", pWVar24, 4)) {
                                                if (!FUN_14005C788("intpe", pWVar24, 5)) {
                                                    if (FUN_14005C788("intsys", pWVar24, 6)) {
                                                        local_2a0 = 4;
                                                        if (pWVar21 != (WCHAR *)(uintptr_t)pWVar30) {
                                                            pWVar24 = pWVar21 + -5;
                                                        }
                                                        goto LAB_1400c754d;
                                                    }
                                                    if (!FUN_14005C788("int", pWVar24, 3)) {
                                                        goto LAB_1400c670e;
                                                    }
                                                    pWVar25 = pWVar24 + 2;
                                                }
                                                else {
                                                    pWVar25 = pWVar24 + 4;
                                                }
                                                if (pWVar21 != (WCHAR *)(uintptr_t)pWVar30) {
                                                    pWVar25 = pWVar21;
                                                }
                                                if (pWVar25[1] == L's') {
                                                    v17 = 4;
                                                }
                                                WVar23 = pWVar25[1];
                                            }
                                            else {
                                                pWVar25 = pWVar24 + 3;
                                                local_2a8 = WSTR("0x%I64X");
                                                if (pWVar21 != (WCHAR *)(uintptr_t)pWVar30) {
                                                    pWVar25 = pWVar21 + -1;
                                                }
                                                if (pWVar25[1] == L's') {
                                                    v17 = 4;
                                                }
                                                WVar23 = pWVar25[1];
                                            }
                                            local_2a0 = 4;
                                            local_2f8 = 4;
                                            local_270 = v17;
                                            if (WVar23 == L'0') {
                                                pWVar25 = pWVar25 + 1;
                                                local_2a8 = WSTR("0x%08I64X");
                                            }
                                        }
                                        else {
                                            /* int64 命中 */
                                            pWVar25 = pWVar24 + 7;
                                        }
                                    }
                                    else {
                                        /* ldouble 命中 */
                                        pWVar25 = pWVar24 + 4;
                                    }
                                    local_2f8 = 8;
                                    if (pWVar21 != (WCHAR *)(uintptr_t)pWVar30) {
                                        pWVar25 = pWVar21;
                                    }
                                    local_2a0 = 8;
                                    if (pWVar25[1] == L's') {
                                        v17 = 8;
                                    }
                                    local_270 = v17;
                                    if (pWVar25[1] == L'0') {
                                        local_2a8 = WSTR("0x%016I64X");
                                        pWVar25 = pWVar25 + 1;
                                        v17 = local_270;
                                    }
                                }
                                else {
                                    /* double 命中 */
                                    pWVar25 = pWVar24 + 6;
                                    local_1c8 = 8;
                                }
                            }
                            else {
                                /* float 命中 */
                                pWVar25 = pWVar24 + 5;
                                local_1c0 = 8;
                            }
                            local_2f8 = 8;
                            local_2a0 = 8;
                        }
                        else {
                            /* intptr 命中 */
                            local_1d0 = 4;
                            local_2f8 = 4;
                            local_2a0 = 4;
                            pWVar25 = pWVar24 + 4;
                        }
                        goto LAB_1400c670e;
                    }
                    /* ptr 命中 */
                    pWVar25 = pWVar24 + 5;
                    if (pWVar21 != (WCHAR *)(uintptr_t)pWVar30) {
                        pWVar25 = pWVar21;
                    }
                    local_2a0 = 8;
                    local_270 = v17;
                    if (pWVar25[1] == L's') {
                        v17 = 8;
                        local_270 = v17;
                    }
                }
                else {
                    /* short/wchar 合并分支 (decompiled: short 落此, 尺寸 100) */
                    local_2a0 = 100;
                    pWVar25 = pWVar24 + 2;
                    if (pWVar21 != (WCHAR *)(uintptr_t)pWVar30) {
                        pWVar25 = pWVar21;
                    }
                    local_2f8 = 8;
                    local_2a8 = WSTR("0x%I64X");
                    if (pWVar25[1] == L'0') {
                        pWVar25 = pWVar25 + 1;
                    }
                }
if (pWVar25[1] == L'@') {
                    local_2a8 = local_2a8 + 2;
                    pWVar25 = pWVar25 + 1;
                }
                goto LAB_1400c670e;
            }
            if (*pWVar25 == L'-') {
                /* 裸 '-': 清填充模式 */
                local_278 = (int)(uintptr_t)pWVar30;
                pWVar25 = pWVar25 + 4;
                goto LAB_1400c670e;
            }
        }
LAB_1400c670e:
        pWVar25 = pWVar25 + 1;
        if (pWVar12 == pWVar30) goto LAB_1400c671a;
        goto LAB_1400c65e2;
    }
LAB_1400c671a:
    local_2b0 = local_280;                       /* -[ 起始索引暂存传递 */
    v17 = local_218;                             /* 父链深度 (-mkdummy) */
    goto LAB_1400c6734;
    /* ======== 主流程: 重切 token 并落盘 ======== */
LAB_1400c6734:
    PECMD_AllocStrSlot(&local_2f0);
    PECMD_SplitNextToken((int64_t *)param_1, (int64_t *)&local_308, (int64_t *)&local_240, 0x2c, 0x3b);
    PECMD_SplitNextToken((int64_t *)param_1, (int64_t *)&local_308, (int64_t *)&local_2f0, 0x2c, 0x3b);
    local_308 = local_240;
    if (((v32 == 8) || (*(WCHAR *)local_240 != L'\0')) || (*local_2f0 != L'\0')) {
        local_298 = ('\x01' < local_317) ? (int8_t)1 : (int8_t)0;
        pWVar25 = (local_298 != 0) ? param_1 : (WCHAR *)0;
        local_2e0 = param_1;
        if (0 < v17) {                           /* 名称查表作用域: 沿父链上溯 v17 层 */
            while (v17 > 0) {
                v17--;
                local_2e0 = *(WCHAR **)(intptr_t)((char *)local_2e0 + 0x1c);
            }
            if (local_298 != 0) {
                pWVar25 = local_2e0;
            }
        }
        local_258 = param_1;                     /* 数据源作用域: 上溯 v18 层 */
        if (0 < v18) {
            while (v18 > 0) {
                v18--;
                local_258 = *(WCHAR **)(intptr_t)((char *)local_258 + 0x1c);
            }
        }
        if ((uint8_t)local_res10 != 0) {         /* 未给 '=': 清插入模式与读模式 */
            local_318 = '\0';
            local_290 = 0;
        }
        local_310 = pWVar25;
        EnterCriticalSection((CRITICAL_SECTION *)g_csInit);
        local_2c8 = (WCHAR *)0;
        local_1e8 = 0;
        local_250 = 0;
        local_188 = (WCHAR *)0;
        local_140 = (WCHAR *)0;
        if (local_1e0 < 1) goto LAB_1400c77a6;
        iVar5 = lstrlenW((LPCWSTR)local_308);
        iVar6 = lstrlenW((LPCWSTR)local_320);
        PECMD_AllocString(&local_2c8, (longlong)iVar6 + 0x641 + (longlong)iVar5 + 0x641);
        local_1e8 = (int64_t)(intptr_t)local_308;
        local_250 = (int64_t)(intptr_t)local_320;
        local_140 = (WCHAR *)((char *)local_2c8 + ((longlong)iVar5 + 0x641) * 2);
        local_188 = local_2c8;
LAB_1400c8787:
        v17 = (int64_t)(intptr_t)local_140;
        v18 = local_1b0;
        local_1e0 = local_1e0 - 1;
        local_308 = local_188;
        PECMD_ExpandPercentD((uint16_t *)local_188, (WCHAR *)(intptr_t)local_1e8);       /* 展开 %% 与 %d 行项 */
        v26 = local_2b0;
        local_320 = (WCHAR *)(intptr_t)v17;
        PECMD_ExpandPercentD((uint16_t *)(intptr_t)v17, (WCHAR *)(intptr_t)local_250);
        local_1b0 = v18 + local_1d8;
        local_2b0 = v26 + local_1d8;
LAB_1400c77a6:
        v18 = 0;
        local_2d8 = 0;
        bVar34 = false;
        local_170_u.u64 = 0;
        memset(local_168, 0, 0x28);                  /* 原 FUN_140102a90(local_168,0,0x28) */
        local_2d0 = (WCHAR *)0;
        PECMD_AllocStrSlot(&local_2e8);
        pWVar30 = local_258;
        v17 = (int64_t)(intptr_t)local_320;
        local_210 = (ulonglong)(longlong)local_278;  /* 填充值符号扩展 */
        v32 = 0;
        local_228 = (int64_t)(intptr_t)&local_300;
        local_180 = (uint64_t *)&local_248;
        local_300 = (WCHAR *)0;
        local_190 = 0;
        local_1a8 = 0;
        local_2b8 = (WCHAR *)0;
        local_220 = (int64_t)0xffffffffff000000;
        local_268 = (int64_t)0xffffffffff000000;
        if (local_278 < 0) {
            if (local_2c0 != '\0') {                 /* -addr/-ex: 值为地址表达式 */
                local_320 = (WCHAR *)PECMD_CheckVarMultiLevelRef((int64_t *)local_258, (LPCWSTR)local_320);
                v18 = 2;
LAB_1400c7962:
                pWVar12 = local_2e0;
                pWVar30 = local_310;
                goto LAB_1400c84b1;
            }
            if ((char)(int8_t)local_290 < 0) {       /* -make/-tow/-copy: 具名内存块写入 */
                local_238 = 0;
                WVar23 = L'&';
                local_2d8 = 0;
                v29 = -1;
                local_280 = 0;
                local_268 = 0;
                if (*(ushort *)local_320 == 0x26) {  /* '&名:偏移@基址:长度' */
                    v32 = 1;
                    local_2b8 = local_320;
                    do {
                        if (((WVar23 == L';') || (WVar23 == L',')) || ((WVar23 == L':' || (WVar23 == L'@')))) break;
                        local_2b8 = local_2b8 + 1;
                        WVar23 = *local_2b8;
                    } while (WVar23 != L'\0');
                    WVar23 = *local_2b8;
                    if (WVar23 != L'\0') {
                        *local_2b8 = L'\0';
                        local_2b8 = local_2b8 + 1;
                    }
                    if ((WVar23 == L'@') &&
                        (WVar23 = *local_2b8, local_268 = (int64_t)(intptr_t)local_2b8, WVar23 != L'\0')) {
                        do {
                            if ((WVar23 == L';') || ((WVar23 == L',' || (WVar23 == L':')))) break;
                            local_2b8 = local_2b8 + 1;
                            WVar23 = *local_2b8;
                        } while (WVar23 != L'\0');
                        if (*local_2b8 != L'\0') {
                            *local_2b8 = L'\0';
                            local_2b8 = local_2b8 + 1;
                        }
                    }
                    v31 = local_268;
                    v26 = (int64_t)(intptr_t)local_2b8;
                    lVar15 = (longlong)(intptr_t)PECMD_VarLookup(local_258, (LPCWSTR)local_320, (void *)0, -1, NULL);
                    if (lVar15 != 0) {
                        local_320 = *(WCHAR **)(lVar15 + 8);
                        v29 = (int64_t)(*(ulonglong *)(lVar15 + 0x18) & 0x3fffffffffffffffULL);
                        local_2d8 = v29;
                        goto LAB_1400c7c61;
                    }
                    goto LAB_1400c87f4;
                }
                local_248 = (int64_t)(intptr_t)local_320;
                local_2b8 = local_320;
                if (*(short *)local_320 != 0) {
                    WVar23 = *local_320;
                    do {
                        if ((((WVar23 == L';') || (WVar23 == L',')) || (WVar23 == L':')) || (WVar23 == L'@')) break;
                        local_2b8 = local_2b8 + 1;
                        WVar23 = *local_2b8;
                    } while (WVar23 != L'\0');
                }
                WVar23 = *local_2b8;
                if (WVar23 != L'\0') {
                    *local_2b8 = L'\0';
                    local_2b8 = local_2b8 + 1;
                }
                if ((WVar23 == L'@') &&
                    (WVar23 = *local_2b8, local_268 = (int64_t)(intptr_t)local_2b8, WVar23 != L'\0')) {
                    do {
                        if ((WVar23 == L';') || ((WVar23 == L',' || (WVar23 == L':')))) break;
                        local_2b8 = local_2b8 + 1;
                        WVar23 = *local_2b8;
                    } while (WVar23 != L'\0');
                    if (*local_2b8 != L'\0') {
                        *local_2b8 = L'\0';
                        local_2b8 = local_2b8 + 1;
                    }
                }
                v31 = local_268;
                v26 = (int64_t)(intptr_t)local_2b8;
                iVar5 = StrCmpNIW(WSTR("(shm)"), (LPCWSTR)local_320, 5);
                if (iVar5 == 0) {                    /* (shm) 共享内存对象 */
                    if ((((*(LPCWSTR)((char *)(intptr_t)v17 + 10) == L'\0') ||
                         (lVar15 = (longlong)(intptr_t)PECMD_DispatchByObjectName((uint64_t *)param_1,
                                                                   (LPCWSTR)((char *)(intptr_t)v17 + 10), -1,
                                                                   (uint16_t *)g_szEmpty, (uint64_t)(uintptr_t)g_szEmpty,
                                                                   (uint64_t)(uintptr_t)g_szEmpty), lVar15 == 0)) ||
                        (*(int *)(lVar15 + 8) != 0x14)) ||
                       ((*(int *)(lVar15 + 0x80) != -2 || (*(longlong *)(lVar15 + 0x88) < 1)))) {
                        goto LAB_1400c87f4;
                    }
                    local_320 = *(WCHAR **)(lVar15 + 0x78);
                    v29 = *(longlong *)(lVar15 + 0x88) + -2;
                    pWVar30 = local_258;
                    local_2d8 = v29;
                }
                else {
                    FUN_1400857E4((int64_t *)&local_248, (uint64_t *)&local_238, (uint8_t *)&local_280);
                    local_320 = (WCHAR *)(intptr_t)local_238;
                    pWVar30 = local_258;
                }
LAB_1400c7c61:
                if (((v31 != 0) &&
                    (iVar5 = FUN_1400857E4((int64_t *)&local_268, (uint64_t *)&local_238,
                                                    (uint8_t *)&local_280), 0 < iVar5)) &&
                   (0 < (longlong)local_238)) {
                    local_320 = (WCHAR *)((char *)local_320 + local_238);
                    v29 = (int64_t)((char *)(intptr_t)v29 - local_238);
                    local_2d8 = v29;
                }
                WVar23 = *(WCHAR *)(intptr_t)v26;
                if (WVar23 == L'~') {                /* '~': 长度取整串 */
                    v26 += 2;
                    local_2b8 = (WCHAR *)(intptr_t)v26;
                }
                WVar33 = *(WCHAR *)(intptr_t)v26;
                if (WVar33 == L'*') {                /* '*': 零终止长度 */
                    local_2b8 = (WCHAR *)(intptr_t)(v26 + 2);
                    local_280 = 0;
                }
                iVar5 = FUN_1400857E4((int64_t *)&local_2b8, (uint64_t *)&local_2d8, (uint8_t *)&local_280);
                v18 = 0;
                if (local_320 == (WCHAR *)0) {
                    local_320 = (WCHAR *)DAT_14011d308;
                    v32 = v18;
                }
                else {
                    v18 = local_2d8;
                    if (iVar5 < 1) {
                        if ((char)v32 == '\0') {
                            if (WVar33 != L'*') {
                                if (WVar23 != L'~') {
                                    iVar5 = lstrlenW((LPCWSTR)local_320);
                                    v18 = (int64_t)iVar5 * 2;
                                }
                            }
                            else {
                                iVar5 = lstrlenA((LPCSTR)local_320);
                                v18 = (int64_t)iVar5;
                            }
                        }
                    }
                    else if (((char)v32 != '\0') && ((longlong)v29 < (longlong)local_2d8)) {
                        v18 = v29;                   /* 越界截断到块尾 */
                    }
                    v29 = local_290;                 /* 模式复入长度槽 */
                    v17 = (int64_t)(intptr_t)local_320;
                    if ((longlong)v18 < 0) {
LAB_1400c891e:
                        PECMD_FreeStrBuf(&local_300);
                        PECMD_FreeStrBuf(&local_2e8);
                        PECMD_FreeStrBuf(&local_2d0);
                        PECMD_FreeStrBuf(&local_2c8);
                        goto LAB_1400c8828;
                    }
                    v18 &= 0x3fffffffffffffffLL;
                    v32 = local_1a8;
                    if ((((uint8_t)local_290 & 0x81)) == 0x81) {
                        /* -tow/-copy: 把块指针直接挂入变量槽 */
                        plVar9 = (int64_t *)(intptr_t)PECMD_VarLookup(pWVar30, (LPCWSTR)local_308, (void *)0, -1, NULL);
                        r28 = 0;
                        if (plVar9 == (int64_t *)0) {
                            plVar9 = (int64_t *)(intptr_t)PECMD_AddVarDefault(local_2e0, (LPCWSTR)local_308,
                                                               (LPCWSTR)g_szEmpty, -1, 0);
                        }
                        else if ((*(uint8_t *)((char *)plVar9 + 0x1f) & 0xc0) != 0) {
                            plVar9[1] = 0;
                        }
                        uVar10 = 0x8000000000000000ULL;
                        if (v29 == 0x83) {
                            uVar10 = 0xc000000000000000ULL;
                        }
                        plVar9[3] = (longlong)(uVar10 | (ulonglong)v18);
                        local_res10 = plVar9[1];     /* 旧缓冲 */
                        plVar9[1] = (longlong)(intptr_t)v17;
                        PECMD_FreeStrBuf((WCHAR **)&local_res10);
                        PECMD_FreeStrBuf(&local_300);
                        PECMD_FreeStrBuf(&local_2e8);
                        PECMD_FreeStrBuf(&local_2d0);
                        PECMD_FreeStrBuf(&local_2c8);
                        goto LAB_1400c949b;
                    }
                }
                pWVar12 = local_2e0;
                pWVar30 = local_310;
                goto LAB_1400c84b1;
            }
            if (local_290 != 0) {                    /* 读模式分派 */
                WVar33 = L'\0';
                WVar23 = *local_320;
                local_2b8 = local_320;               /* 兼作扫描游标 */
                {
                    WCHAR *scan = local_320;
                    while (WVar23 != L'\0') {
                        if ((((WVar23 == L',') || (WVar23 == L';')) || (WVar23 == L':')) || (WVar23 == L'|')) {
                            WVar33 = *scan;
                            *scan = L'\0';
                            scan = scan + 1;
                            break;
                        }
                        scan = scan + 1;
                        WVar23 = *scan;
                    }
                    local_2d8 = (int64_t)(intptr_t)scan;
                }
                PECMD_SplitNextToken((int64_t *)param_1, (int64_t *)&local_320, (int64_t *)&local_2e8, 0x2c, 0);
                lVar15 = (longlong)(intptr_t)PECMD_VarLookup(local_258, (LPCWSTR)local_2e8, (void *)0, -1, NULL);
                pWVar30 = local_310;
                if (lVar15 == 0) goto LAB_1400c891e;
                v17 = lVar15 + 8;                    /* 记录数据域指针 */
                r1_lenptr = (uint64_t *)(lVar15 + 0x18);
                local_320 = *(WCHAR **)(lVar15 + 8);
                v29 = (int64_t)*(ulonglong *)(lVar15 + 0x18);
                v18 = (int64_t)(v29 & 0x3fffffffffffffffLL);
                if (local_290 == 8) {                /* -def: 输出块地址 */
                    PECMD_AppendLongDecimal((int64_t *)param_1, (int64_t)(v29 & 0xffffffff), local_2f0);
                    if (*local_308 != L'\0') {
                        wsprintfW(local_128, WSTR("0x%p"), (void *)local_320);   /* TODO(verify): 原文变参缺省 */
                        local_320 = local_128;
                        iVar5 = lstrlenW((LPCWSTR)local_128);
                        v18 = (int64_t)iVar5 * 2;
                        goto LAB_1400c7962;
                    }
                    /* 原: ~-(ulonglong)(cond) & 0xffffffff80070057 → 有第二参数则 0 否则参数错 */
                    r28 = (*local_2f0 != L'\0') ? 0 : (int64_t)0xffffffff80070057ULL;
                    PECMD_FreeStrBuf(&local_300);
                    PECMD_FreeStrBuf(&local_2e8);
                    PECMD_FreeStrBuf(&local_2d0);
                    PECMD_FreeStrBuf(&local_2c8);
                    goto LAB_1400c949b;
                }
                if (local_290 != 4) {
                    if (local_290 == 0x100) {        /* -tom: 内存比较 */
                        local_2b8 = (WCHAR *)(intptr_t)PECMD_VarLookup(local_2e0, (LPCWSTR)local_308,
                                                               (void *)(intptr_t)local_310, -1, NULL);
                        local_248 = (int64_t)(*(ulonglong *)((char *)(intptr_t)local_2b8 + 0x18) &
                                              0x3fffffffffffffffULL);
                        local_238 = 0;
                        local_268 = -8;              /* 0xfffffffffffffff8: 全长比较标记 */
                        local_280 = 0;
                        WVar23 = L'\0';
                        PECMD_EvalExprSkipOneChar((WCHAR **)&local_2d8, (uint64_t *)&local_238);
                        PECMD_EvalExprSkipOneChar((WCHAR **)&local_2d8, (uint64_t *)&local_268);
                        bVar2 = (longlong)local_268 < 0;
                        v17 = local_268;
                        if (bVar2) {
                            v17 = v18;
                        }
                        PECMD_EvalExprSkipOneChar((WCHAR **)&local_2d8, (uint64_t *)&local_280);
                        if ((((WCHAR)(*(WCHAR *)(intptr_t)local_2d8 | 0x20U) == 0x73) ||
                            ((WVar33 = WVar23, (WCHAR)(*(WCHAR *)(intptr_t)local_2d8 | 0x20U) == 0x69)))) {
                            WVar23 = *(WCHAR *)(intptr_t)local_2d8;
                            WVar33 = WVar23 ^ 0x20;
                            if (WVar23 == L'\0') {
LAB_1400c807a:
                                bVar2 = false;
                            }
                        }
                        else {
                            goto LAB_1400c807a;
                        }
                        if ((((longlong)local_280 < 0) || ((longlong)local_238 < 0)) ||
                           (((longlong)v17 < 1 ||
                            (((longlong)v18 < (longlong)local_238 ||
                             ((longlong)local_248 < (longlong)local_280)))))) {
                            goto LAB_1400c891e;
                        }
                        if (!bVar2) {
                            v32 = local_248 - local_280;
                            if ((longlong)v18 - (longlong)local_238 <
                                (longlong)local_248 - (longlong)local_280) {
                                v32 = v18 - local_238;
                            }
                            if ((longlong)v32 < (longlong)v17) {
                                v17 = v32;
                            }
                            if ((WVar33 & 0x20U) != 0) {
                                v17 = (int64_t)((ulonglong)v17 >> 1);   /* 宽串: 字节长→字符长 */
                            }
                        }
                        iVar6 = 0;
                        if (0 < (longlong)v17) {
                            pWVar13 = (LPCWSTR)((char *)(intptr_t)local_280 +
                                                (longlong)(intptr_t)*(WCHAR **)((char *)(intptr_t)local_2b8 + 8));
                            lpStr2 = (LPCWSTR)((char *)(intptr_t)local_238 + (longlong)(intptr_t)local_320);
                            iVar5 = (int)v17;
                            if (WVar23 == L's') {
                                iVar6 = bVar2 ? lstrcmpA((LPCSTR)pWVar13, (LPCSTR)lpStr2)
                                              : StrCmpNA((LPCSTR)pWVar13, (LPCSTR)lpStr2, iVar5);
                            }
                            else if (WVar23 == L'S') {
                                iVar6 = bVar2 ? lstrcmpW(pWVar13, lpStr2)
                                              : StrCmpNW(pWVar13, lpStr2, iVar5);
                            }
                            else if (WVar23 == L'i') {
                                iVar6 = bVar2 ? lstrcmpiA((LPCSTR)pWVar13, (LPCSTR)lpStr2)
                                              : StrCmpNIA((LPCSTR)pWVar13, (LPCSTR)lpStr2, iVar5);
                            }
                            else if (WVar23 == L'I') {
                                iVar6 = bVar2 ? lstrcmpiW(pWVar13, lpStr2)
                                              : StrCmpNIW(pWVar13, lpStr2, iVar5);
                            }
                            else {
                                iVar6 = PECMD_AnsiStrNCompare((char *)(intptr_t)pWVar13, (longlong)(intptr_t)lpStr2,
                                                              (longlong)v17);
                            }
                            /* TODO(verify): 原体比较返回值经寄存器直接进格式化, 此处显式捕获 */
                        }
                        wsprintfW(local_128, WSTR("%d"), iVar6);
                        local_320 = local_128;
                        iVar5 = lstrlenW((LPCWSTR)local_128);
                        v18 = (int64_t)iVar5 * 2;
                        v32 = local_1a8;
                        goto LAB_1400c7962;
                    }
                    if (local_290 == 1) {            /* -std: 宽→ANSI 半宽存储 */
                        v17 = (int64_t)((((ulonglong)v29 >> 1) & 0x1fffffffffffffffULL) * 5ULL);
                        PECMD_GrowByteBuffer(&local_300, v17 + 1);
                        v26 = (int64_t)(intptr_t)local_300;
                        v18 = 0;
                        iVar5 = WideCharToMultiByte(0, 0, (LPCWSTR)local_320,
                                                    (int)((ulonglong)v29 >> 1),
                                                    (LPSTR)local_300, (int)v17, (LPCSTR)0, (BOOL *)0);
                        v29 = (int64_t)iVar5;
                        if ((-1 < (longlong)v29) &&
                            ((v18 = v29), (longlong)v17 < (longlong)v29)) {
                            v18 = v17;
                        }
                        *(unsigned char *)((char *)(intptr_t)v18 + (longlong)(intptr_t)v26) = 0;
                        iVar5 = (int)(((longlong)v18 + 1) / 2);
                        PECMD_AllocString(&local_300, (longlong)iVar5 + 1);
                        *(WCHAR *)((char *)local_300 + (longlong)iVar5 * 2) = L'\0';
LAB_1400c827c:
                        bVar34 = true;
                        pWVar12 = local_2e0;
                        pWVar30 = local_310;
                        local_320 = local_300;
                        goto LAB_1400c84b1;
                    }
                    if (local_290 == 0x2) {          /* -env: ANSI→宽 */
                        PECMD_AllocString(&local_300, (longlong)v18 + 1);
                        iVar5 = ((int (*)(uint32_t, uint32_t, char *, int, WCHAR *, int))
                                 (uintptr_t)DAT_14013c970)(0, 0, (char *)local_320,
                                                           (int)(v29 & 0xffffffff),
                                                           (WCHAR *)local_300, (int)(v29 & 0xffffffff));
                        if (iVar5 < 0) {
                            iVar5 = 0;
                        }
                        PECMD_AllocString(&local_300, (longlong)iVar5 + 1);
                        *(WCHAR *)((char *)local_300 + (longlong)iVar5 * 2) = L'\0';
                        v18 = (int64_t)iVar5 * 2;
                        goto LAB_1400c827c;
                    }
                    if (local_290 == 0x20) {         /* -cmp: 直取记录缓冲指针 */
                        local_320 = *(WCHAR **)(intptr_t)v17;
                        bVar34 = true;
                        v18 = (int64_t)(*r1_lenptr & 0x3fffffffffffffffULL);
                        local_228 = (int64_t)(intptr_t)r1_lenptr;
                        local_180 = r1_lenptr;
                        goto LAB_1400c7910;
                    }
                    /* else: -raw 写具名块 */
                    plVar9 = (int64_t *)(intptr_t)PECMD_VarLookup(local_2e0, (LPCWSTR)local_308,
                                                                  (void *)(intptr_t)local_310, -1, NULL);
                    if (WVar33 == L'\0') {
                        if ((plVar9 != (int64_t *)0) &&
                            ((v17 = (int64_t)(intptr_t)local_320,
                              v29 = v18,
                              (*(uint8_t *)((char *)plVar9 + 0x1f) & 0xc0) != 0))) {
                            goto LAB_1400c89bb;
                        }
                        bVar34 = false;
                        pWVar12 = local_2e0;
                        goto LAB_1400c84b1;
                    }
                    uVar10 = (ulonglong)plVar9[3];
                    local_res10 = 0;
                    local_1e8 = 0;
                    local_250 = v18;
                    PECMD_EvalExprSkipOneChar((WCHAR **)&local_2d8, (uint64_t *)&local_res10);
                    PECMD_EvalExprSkipOneChar((WCHAR **)&local_2d8, (uint64_t *)&local_250);
                    PECMD_EvalExprSkipOneChar((WCHAR **)&local_2d8, (uint64_t *)&local_1e8);
                    r28 = 0;
                    if (((((longlong)local_res10 < 0) || ((longlong)local_250 < 1)) ||
                        ((longlong)v18 < (longlong)local_res10)) ||
                       ((longlong)(uVar10 & 0x3fffffffffffffffULL) < (longlong)local_1e8)) {
                        goto LAB_1400c87f4;
                    }
                    v26 = local_1e8;
                    if ((longlong)local_1e8 < 0) {
                        v26 = r28;
                    }
                    v32 = (int64_t)((uVar10 & 0x3fffffffffffffffULL) - (ulonglong)v26);
                    v17 = local_250;
                    if ((longlong)v18 - (longlong)local_res10 < (longlong)local_250) {
                        v17 = v18 - local_res10;
                    }
                    if ((longlong)v32 < (longlong)v17) {
                        v17 = v32;
                    }
                    if (0 < (longlong)v17) {
                        PECMD_MemMoveSafe((unsigned char *)((char *)(intptr_t)v26 + plVar9[1]),
                                          (longlong)local_res10 + (longlong)(intptr_t)local_320,
                                          (int)v17);
                    }
                    PECMD_FreeStrBuf(&local_300);
                    PECMD_FreeStrBuf(&local_2e8);
                    PECMD_FreeStrBuf(&local_2d0);
                    PECMD_FreeStrBuf(&local_2c8);
                    LeaveCriticalSection((CRITICAL_SECTION *)g_csInit);
                    PECMD_FreeStrBuf(&local_2f0);
                    goto LAB_1400c94b0;
                }
                /* -raw 且无修饰符时落入下方通用展开/写路径 */
            }
            if ((local_327 != 0) && (*(short *)local_320 != 0)) {   /* ~ 与 * 修饰展开 */
                bVar3 = (uint8_t)((uint8_t)(uint64_t)local_1a0 | local_327);
                plVar9 = (int64_t *)PECMD_AssignString((int64_t *)&local_2e8, (LPCWSTR)local_320);
                local_320 = (WCHAR *)PECMD_ExpandVarsLocked((int64_t *)local_258, plVar9, bVar3);
            }
            pWVar30 = local_258;
            iVar5 = (int)local_1b8;
            while (true) {
                iVar5 = iVar5 + -1;
                if (iVar5 < 0) break;
                PECMD_ExpandCommandLine((int64_t *)pWVar30, local_320, &local_2d0, 0, 0);
                local_320 = local_2d0;               /* -x: 反复展开 */
            }
            v17 = v32;
            if (*local_320 != L'\0') {
                PECMD_ApplyVarWriteModifiers((int64_t *)&local_320, (int64_t *)&local_300,
                                             (uint16_t *)param_4, (int64_t *)&local_2d8);
                v17 = (int64_t)(intptr_t)local_300;
                v18 = local_2d8;
            }
            v29 = local_2f8;
            bVar34 = local_320 == (WCHAR *)(intptr_t)v17;
            if (0 < local_2a0) {
                if (local_2a0 == 0x2a) goto LAB_1400c891e;   /* 结构域解析失败 */
                v18 = local_2f8;
                if (0 < local_1f0) {
                    v18 = 8;
                }
                local_288 = local_320;
                PECMD_AllocStrSlot((WCHAR **)&local_res10);
                PECMD_SplitNextToken((int64_t *)param_1, (int64_t *)&local_288,
                                     (int64_t *)&local_res10, 0x3a, 0x2c);
                if (*(WCHAR *)(intptr_t)local_res10 == L'\0') {
                    PECMD_FreeStrBuf((WCHAR **)&local_res10);
                    PECMD_FreeStrBuf(&local_300);
                    PECMD_FreeStrBuf(&local_2e8);
                    PECMD_FreeStrBuf(&local_2d0);
                    PECMD_FreeStrBuf(&local_2c8);
                    LeaveCriticalSection((CRITICAL_SECTION *)g_csInit);
                    PECMD_FreeStrBuf(&local_2f0);
                    r28 = 0;
                    goto LAB_1400c94b0;
                }
                v17 = v32;
                if (*(WCHAR *)local_288 == L'~') {
                    local_288 = local_288 + 1;
                    v17 = v18;
                }
                local_2b0 = 0;
                PECMD_ParseNumOrVar((int64_t *)&local_288, (uint64_t *)&local_2b0, (int64_t *)param_1);
                pWVar25 = local_2e0;
                v26 = local_2b0;
                if ((char)v17 != '\0') {
                    v26 = (int64_t)((ulonglong)local_2b0 * (ulonglong)(longlong)(char)v17);
                }
                local_e8[0] = L'\0';
                local_260 = local_260 + v26;
                lVar15 = local_260 + (longlong)v18;
                if (0 < local_208) {
                    local_308[local_208] = L'\0';
                }
                if ((((*(ushort *)local_308 < 0x30) || (0x39 < *(ushort *)local_308)) &&
                    (*(short *)local_308 != 0x28))) {
                    lVar11 = (longlong)(intptr_t)PECMD_VarLookup(local_2e0, (LPCWSTR)local_308,
                                                                 (void *)(intptr_t)local_310, -1, NULL);
                    v18 = v32;
                    if (lVar11 != 0) {
                        v18 = *(longlong *)(lVar11 + 8);
                        v32 = (int64_t)(*(ulonglong *)(lVar11 + 0x18) & 0x3fffffffffffffffULL);
                    }
                }
                else {
                    local_248 = (int64_t)(intptr_t)local_308;
                    local_250 = 0;
                    FUN_1400C1194((int64_t *)&local_248, (uint64_t *)&local_250);
                    v32 = 0x7fffffffffffffffLL - local_250;
                    v18 = local_250;
                    pWVar25 = local_2e0;
                }
                if (lVar15 <= (longlong)v32) {
                    plVar9 = (int64_t *)((char *)(intptr_t)v18 + local_260);
                    iVar5 = 0;
                    iVar20 = 0;
                    if (0 < local_1f0) {
                        do {
                            plVar8 = (int64_t *)(intptr_t)FUN_140063b00(r28, &local_200, &local_1f8, 8);
                            r28 = r28 + 1;
                            plVar9 = (int64_t *)(*plVar8 + *plVar9);
                        } while ((longlong)r28 <= local_1f0);
                    }
                    FUN_14001d78c(local_170_u.b, (unsigned char *)plVar9, (int)v29);
                    if (local_1d0 == 0) {
                        if (local_1c0 == 0) {
                            if (local_1c8 == 0) {
                                if (local_2a0 == 100) {
                                    pwVar19 = WSTR("0x%p");
LAB_1400c9415:
                                    wsprintfW(local_e8, pwVar19, (void *)(uintptr_t)local_170_u.u64); /* TODO(verify): 变参缺省补全 */
                                }
                                else if (((local_270 == 1) || (local_270 == 2)) ||
                                        (local_270 == 4)) {
                                    wsprintfW(local_e8, WSTR("%d"), (int)(uint32_t)local_170_u.u64);
                                }
                                else {
                                    if (local_270 == 8) {
                                        pwVar19 = WSTR("%I64d");
                                        goto LAB_1400c9415;
                                    }
                                    PECMD_SprintfRetEnd(local_e8, local_170_u.u64, local_2a8);
                                }
                            }
                            else {
                                PECMD_FormatU64RetEnd(local_e8, local_170_u.u64, WSTR("%Lf"));
                                iVar20 = iVar5;
                            }
                        }
                        else {
                            PECMD_FormatU64RetEnd(local_e8, local_170_u.u64, WSTR("%lf"));
                            iVar20 = iVar5;
                        }
                    }
                    else {
                        { union { uint32_t u; float f; } fcv; fcv.u = (uint32_t)local_170_u.u64;
                          PECMD_FormatDblRetEnd(local_e8, fcv.f, WSTR("%f")); }
                        iVar20 = iVar5;
                    }
                }
                FUN_1400629B8(pWVar25, (LPCWSTR)(intptr_t)local_res10, local_e8);
                r28 = (int64_t)iVar20;
                PECMD_FreeStrBuf((WCHAR **)&local_res10);
                PECMD_FreeStrBuf(&local_300);
                PECMD_FreeStrBuf(&local_2e8);
                PECMD_FreeStrBuf(&local_2d0);
                PECMD_FreeStrBuf(&local_2c8);
LAB_1400c949b:
                LeaveCriticalSection((CRITICAL_SECTION *)g_csInit);
                PECMD_FreeStrBuf(&local_2f0);
                goto LAB_1400c94b0;
            }
            if (0 < (longlong)local_2f8) {           /* 定尺寸写入变量 */
                v17 = 0;
                if (local_2f8 == 0x2a) goto LAB_1400c891e;
                local_res10 = (int64_t)(intptr_t)local_320;   /* 槽复用: 名字游标 */
                bVar34 = true;
                if (*local_320 == L'&') {
                    bVar34 = false;
                    v18 = (int64_t)(intptr_t)local_320;
                    b7c_skip_to((WCHAR **)&local_res10, L':');
                    WVar23 = *(WCHAR *)(intptr_t)local_res10;
                    if (WVar23 != L'\0') {
                        *(WCHAR *)(intptr_t)local_res10 = L'\0';
                    }
                    lVar15 = (longlong)(intptr_t)PECMD_VarLookup(param_1, (LPCWSTR)(intptr_t)v18,
                                                                 (void *)0, -1, NULL);
                    if (WVar23 != L'\0') {
                        *(WCHAR *)(intptr_t)local_res10 = WVar23;
                    }
                    v17 = 0;
                    if (lVar15 != 0) {
                        v17 = *(longlong *)(lVar15 + 8);
                        v35 = (int64_t)(longlong)(double)(longlong)v17;   /* 经 double 位型截断(原体如此) */
                        local_230 = (uint64_t)v35;
                    }
                }
                v18 = local_2f8;
                if (local_1d0 == 0) {
                    if ((local_1c0 == 0) && (local_1c8 == 0)) {
                        v35 = v17;
                        if (bVar34) {
                            PECMD_ParseNumOrVar((int64_t *)&local_res10, &local_170_u.u64,
                                                (int64_t *)param_1);
                            v35 = (int64_t)local_170_u.u64;
                        }
                    }
                    else if (bVar34) {
                        FUN_140067b54((uint16_t *)(intptr_t)local_res10, (double *)&local_230);
                        v35 = (int64_t)local_230;
                    }
                }
                else {
                    if (bVar34) {
                        FUN_140067b54((uint16_t *)(intptr_t)local_res10, (double *)&local_230);
                        v35 = (int64_t)local_230;
                    }
                    { /* float 截断: 低 32 位 = (float)(double)v35 位型 */
                        union { uint32_t u; float f; } fc2; fc2.f = (float)(double)(longlong)v35;
                        local_170_u.u64 = (local_170_u.u64 & 0xffffffff00000000ULL) | fc2.u;
                    }
                    v35 = (int64_t)local_170_u.u64;
                }
                local_170_u.u64 = (uint64_t)v35;
                if (*(WCHAR *)(intptr_t)local_res10 != L'\0') {
                    v17 = 0;
                    v32 = (longlong)(intptr_t)local_res10 + 2;
                    if (*(WCHAR *)((char *)(intptr_t)local_res10 + 2) == L'~') {
                        v17 = v18;
                        v32 = (longlong)(intptr_t)local_res10 + 4;
                    }
                    local_res10 = v32;
                    local_2b0 = 0;
                    PECMD_ParseNumOrVar((int64_t *)&local_res10, (uint64_t *)&local_2b0,
                                        (int64_t *)param_1);
                    v26 = local_2b0;
                    if ((char)v17 != '\0') {
                        v26 = (int64_t)((ulonglong)local_2b0 * (ulonglong)(longlong)(char)v17);
                    }
                    local_260 = local_260 + v26;
                }
                pWVar25 = local_2e0;
                plVar9 = (int64_t *)(intptr_t)PECMD_VarLookup(local_2e0, (LPCWSTR)local_308,
                                                              (void *)(intptr_t)local_310, -1, NULL);
                v17 = (int64_t)(intptr_t)&local_170_u.u64;
                v29 = local_2f8;
                if ((plVar9 != (int64_t *)0) ||
                    ((plVar9 = (int64_t *)(intptr_t)PECMD_AddVarDefault(pWVar25, (LPCWSTR)local_308,
                                                     (LPCWSTR)g_szEmpty, -1, 0),
                      v29 = local_2f8, plVar9 != (int64_t *)0))) {
LAB_1400c89bb:
                    local_2f8 = v29;
                    if (0 < local_1f0) {
                        v18 = 8;
                    }
                    lVar15 = local_260 + (longlong)v18;
                    uVar10 = (ulonglong)plVar9[3] & 0x3fffffffffffffffULL;
                    if (((*(uint8_t *)((char *)plVar9 + 0x1f) & 0xc0) != 0) &&
                        ((longlong)uVar10 < lVar15)) {
                        goto LAB_1400c891e;
                    }
                    if (0 < (longlong)(lVar15 - (longlong)uVar10)) {
                        lVar11 = (lVar15 + 1) / 2 + 2;
                        PECMD_AllocString((WCHAR **)(plVar9 + 1), lVar11);
                        memset((void *)(uVar10 + (ulonglong)plVar9[1]), 0, (size_t)(lVar11 * 2 - (longlong)uVar10));
                        plVar9[3] = lVar15;
                    }
                    plVar9 = (int64_t *)(local_260 + plVar9[1]);
                    if (0 < local_1f0) {
                        do {
                            plVar8 = (int64_t *)(intptr_t)FUN_140063b00(r28, &local_200, &local_1f8, 8);
                            r28 = r28 + 1;
                            plVar9 = (int64_t *)(*plVar8 + *plVar9);
                        } while ((longlong)r28 <= local_1f0);
                    }
                    FUN_14001d78c((unsigned char *)plVar9, (unsigned char *)(intptr_t)v17, (int)local_2f8);
LAB_1400c8ab1:
                    PECMD_FreeStrBuf(&local_300);
                    PECMD_FreeStrBuf(&local_2e8);
                    PECMD_FreeStrBuf(&local_2d0);
                    PECMD_FreeStrBuf(&local_2c8);
                    LeaveCriticalSection((CRITICAL_SECTION *)g_csInit);
                    goto LAB_1400c67b0;
                }
                PECMD_FreeStrBuf(&local_300);
                PECMD_FreeStrBuf(&local_2e8);
                PECMD_FreeStrBuf(&local_2d0);
                PECMD_FreeStrBuf(&local_2c8);
                goto LAB_1400c949b;
            }
            if (local_318 == '\0') {                 /* 无 </>: 常规置值 */
                pWVar12 = local_2e0;
                pWVar30 = local_310;
                if (((uint8_t)local_res10 != 0) && (local_298 == 0)) {   /* 无 '=' 且单层: 清变量 */
                    lVar15 = (longlong)(intptr_t)PECMD_VarLookup(local_2e0, (LPCWSTR)local_308,
                                                                 (void *)(intptr_t)pWVar25, -1, NULL);
                    if (lVar15 != 0) {
                        if ((*(uint8_t *)((char *)lVar15 + 0x1f) & 0xc0) == 0) {
                            PECMD_AssignString((int64_t *)(lVar15 + 8), (LPCWSTR)g_szEmpty);
                            *(longlong *)(lVar15 + 0x18) = 0;
                        }
                        else {
                            **(WCHAR **)(lVar15 + 8) = L'\0';
                        }
                    }
                    goto LAB_1400c8d54;
                }
                goto LAB_1400c84b1;
            }
            if ((longlong)v18 < 1) goto LAB_1400c8ab1;   /* 插入长度为 0 */
            EnterCriticalSection((CRITICAL_SECTION *)g_csInit);
            pWVar12 = local_2e0;
            lVar15 = (longlong)(intptr_t)PECMD_VarLookup(local_2e0, (LPCWSTR)local_308,
                                                         (void *)(intptr_t)pWVar25, -1, NULL);
            if (lVar15 != 0) {
                uVar10 = *(ulonglong *)(lVar15 + 0x18) & 0x3fffffffffffffffULL;
                { WCHAR *oldbuf = *(WCHAR **)(lVar15 + 8);
                  local_178 = oldbuf; }
                bVar34 = -1 < *(longlong *)(lVar15 + 0x18);
                if (!bVar34) {
                    *(longlong *)(lVar15 + 8) = 0;
                }
                local_248 = (int64_t)(uVar10 + (ulonglong)v18);
                lVar11 = (longlong)local_248 / 2;
                PECMD_AllocString((WCHAR **)(lVar15 + 8), lVar11 + 2);
                *(uint16_t *)(*(longlong *)(lVar15 + 8) + 2 + lVar11 * 2) = 0;
                *(uint16_t *)(*(longlong *)(lVar15 + 8) + lVar11 * 2) = 0;
                puVar16 = *(unsigned char **)(lVar15 + 8);
                iVar20 = (int)uVar10;
                if (local_318 == '\x01') {           /* '<': 前插 */
                    if (!bVar34) {
                        FUN_14001d78c(puVar16, (const unsigned char *)(intptr_t)local_178, iVar20);
                    }
                    puVar16 = puVar16 + uVar10;
LAB_1400c8ce3:
                    FUN_14001d78c(puVar16, (const unsigned char *)(intptr_t)local_320, v18);
                }
                else if (local_318 == '\x02') {      /* '>': 后插 */
                    if (bVar34) {
                        PECMD_MemMoveSafe(puVar16 + v18, (longlong)(intptr_t)puVar16, iVar20);
                    }
                    else {
                        FUN_14001d78c(puVar16 + v18, (const unsigned char *)(intptr_t)local_178, iVar20);
                    }
                    goto LAB_1400c8ce3;
                }
                *(longlong *)(lVar15 + 0x18) = local_248;
                LeaveCriticalSection((CRITICAL_SECTION *)g_csInit);
LAB_1400c8d54:
                PECMD_FreeStrBuf(&local_300);
                PECMD_FreeStrBuf(&local_2e8);
                PECMD_FreeStrBuf(&local_2d0);
                PECMD_FreeStrBuf(&local_2c8);
                LeaveCriticalSection((CRITICAL_SECTION *)g_csInit);
                PECMD_FreeStrBuf(&local_2f0);
                PECMD_FreeStrBuf(&local_240);
                PECMD_FreeStrBuf((WCHAR **)&local_200);
                return (void *)(uintptr_t)0;
            }
            LeaveCriticalSection((CRITICAL_SECTION *)g_csInit);
            pWVar30 = local_310;
        }
        else {                                       /* -get 填充模式解析 */
            FUN_1400857E4((int64_t *)&local_320, (uint64_t *)&local_210, (uint8_t *)&local_268);
            bVar3 = (uint8_t)local_268;
            v17 = (int64_t)((ulonglong)local_268 & 0xff);
            local_2d8 = 0;
            local_220 = v17;
            if (*(short *)local_320 != 0) {
                FUN_1400857E4((int64_t *)&local_320, (uint64_t *)&local_190, (uint8_t *)&local_2d8);
                if (*(short *)local_320 == 0) {
LAB_1400c7926:
                    if (local_2d8 == 0) goto LAB_1400c790d;
                }
                else {
                    local_2d8 = 0;
                    iVar5 = FUN_1400857E4((int64_t *)&local_320, (uint64_t *)&local_2b8_num,
                                                   (uint8_t *)&local_2d8);
                    v32 = local_2b8_num;
                    if (iVar5 < 1) goto LAB_1400c7926;
                    if (local_2d8 == 0) {
                        if (v17 == 0) goto LAB_1400c790d;
                        v32 = (int64_t)(local_2b8_num << (bVar3 & 0x3f));
                        goto LAB_1400c7926;
                    }
                }
                if (v17 == 0) {
                    local_220 = local_2d8;
                }
            }
LAB_1400c790d:
            bVar34 = false;
LAB_1400c7910:
            pWVar12 = local_2e0;
            pWVar30 = local_310;
        }
LAB_1400c84b1:
        iVar5 = 0;
        if (((local_218 == 0) || (local_317 != '\0')) ||
            (( (*(uint8_t *)((char *)(intptr_t)pWVar12 + 0xd) & 0xf) != 0) ||
             bVar34 || (-1 < local_278) ||
            ((uint8_t)local_res10 != 0))) {
            plVar8 = (int64_t *)(intptr_t)PECMD_VarLookup(pWVar12, (LPCWSTR)local_308,
                                                          (void *)(intptr_t)pWVar30, -1, NULL);
            plVar9 = plVar8;
            if (plVar8 == (int64_t *)0) {
                iVar6 = iVar20;
                if (local_278 < 0) {
                    v29 = (int64_t)(intptr_t)local_320;
                    v17 = v18;
                    if (bVar34) {
                        v29 = (int64_t)(intptr_t)g_szEmpty;
                        v17 = 0;
                    }
                    plVar9 = (int64_t *)(intptr_t)PECMD_AddVarDefault(local_2e0, (LPCWSTR)local_308,
                                                       (LPCWSTR)(intptr_t)v29, -1, v17);
                    if (local_198 != (LPCWSTR)0) {   /* ~名~: 追加别名 */
                        if (plVar9 == (int64_t *)0) goto LAB_1400c873e;
                        FUN_1400703e4(plVar9 + 2, local_198);
                    }
                    if (plVar9 != (int64_t *)0) goto LAB_1400c858e;
                }
            }
            else {
LAB_1400c858e:
                v26 = local_228;
                if (local_278 < 0) {
                    iVar6 = iVar5;
                    if ((uint8_t)local_res10 == 0) {
                        if (plVar8 != (int64_t *)0) {
                            bVar3 = *(uint8_t *)((char *)plVar8 + 0x1f) & 0xc0;
                            if (bVar3 == 0xc0) {
                                if (bVar34) {
                                    local_320 = *(WCHAR **)(intptr_t)local_228;
                                }
                                PECMD_VarTruncateUpdate(plVar9, (const void *)(intptr_t)local_320,
                                                        (ulonglong)v18);
                                goto LAB_1400c873e;
                            }
                            if (bVar3 != 0) {
                                plVar8[1] = 0;
                            }
                        }
                        if (bVar34) {                /* -cmp: 缓冲所有权交换 */
                            *local_180 = (uint64_t)plVar9[3];
                            v31 = *(int64_t *)(intptr_t)v26;
                            *(int64_t *)(intptr_t)v26 = plVar9[1];
                            plVar9[1] = v31;
                            plVar9[3] = v18;
                        }
                        else if (plVar8 != (int64_t *)0) {
                            PECMD_VarWriteValueCap((WCHAR **)(plVar9 + 1), (uint64_t *)(plVar9 + 3),
                                                   (const void *)(intptr_t)local_320, (longlong)v18);
                        }
                    }
                }
                else {                               /* -get: 区间填充 */
                    v17 = (int64_t)(((plVar9[3] & 0x3fffffffffffffffULL)) - (ulonglong)local_190);
                    v18 = (int64_t)(1 << ((int)local_220 & 0x1f));
                    if (((ulonglong)v17 < (ulonglong)v32) || (v32 == 0)) {
                        v32 = v17;
                    }
                    puVar27 = (ulonglong *)((char *)(intptr_t)local_190 + plVar9[1]);
                    puVar14 = (ulonglong *)((char *)puVar27 - v18 + v32);
                    iVar6 = iVar5;
                    if ((local_190 < (plVar9[3] & 0x3fffffffffffffffULL)) && (v32 != 0)) {
                        if (v18 < 2) {
LAB_1400c868f:
                            memset(puVar27, (int)(local_210 & 0xffffffff), (size_t)v32);
                        }
                        else if (v18 <= v32) {
                            if (v18 == 2) {
                                if (puVar27 <= puVar14) {
                                    lVar15 = ((ulonglong)((char *)puVar14 - (char *)puVar27) >> 1) + 1;
                                    for (; lVar15 != 0; lVar15 = lVar15 + -1) {
                                        *(uint16_t *)puVar27 = (uint16_t)local_210;
                                        puVar27 = (ulonglong *)((char *)puVar27 + 2);
                                    }
                                }
                            }
                            else if (v18 == 4) {
                                if (puVar27 <= puVar14) {
                                    lVar15 = ((ulonglong)((char *)puVar14 - (char *)puVar27) >> 2) + 1;
                                    for (; lVar15 != 0; lVar15 = lVar15 + -1) {
                                        *(uint32_t *)puVar27 = (uint32_t)local_210;
                                        puVar27 = (ulonglong *)((char *)puVar27 + 4);
                                    }
                                }
                            }
                            else {
                                if (v18 != 8) goto LAB_1400c868f;
                                if (puVar27 <= puVar14) {
                                    lVar15 = ((ulonglong)((char *)puVar14 - (char *)puVar27) >> 3) + 1;
                                    for (; lVar15 != 0; lVar15 = lVar15 + -1) {
                                        *puVar27 = local_210;
                                        puVar27 = puVar27 + 1;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        else {
            SetEnvironmentVariableW((LPCWSTR)local_308, (LPCWSTR)local_320);
            iVar6 = iVar5;
        }
LAB_1400c873e:
        if (0 < local_1e0) goto b7c_next_item;
        r28 = (int64_t)iVar6;
        PECMD_FreeStrBuf(&local_300);
        PECMD_FreeStrBuf(&local_2e8);
        PECMD_FreeStrBuf(&local_2d0);
        PECMD_FreeStrBuf(&local_2c8);
        LeaveCriticalSection((CRITICAL_SECTION *)g_csInit);
        PECMD_FreeStrBuf(&local_2f0);
        PECMD_FreeStrBuf(&local_240);
        PECMD_FreeStrBuf((WCHAR **)&local_200);
    }
    else {
LAB_1400c67b0:
        r28 = 0;
        PECMD_FreeStrBuf(&local_2f0);
LAB_1400c94b0:
        PECMD_FreeStrBuf(&local_240);
        PECMD_FreeStrBuf((WCHAR **)&local_200);
    }
    return (void *)(uintptr_t)r28;
LAB_1400c8828:
    LeaveCriticalSection((CRITICAL_SECTION *)g_csInit);
    PECMD_FreeStrBuf(&local_2f0);
LAB_1400c883d:
    r28 = (int64_t)0xffffffff80070057ULL;
    goto LAB_1400c94b0;
LAB_1400c87f4:
    PECMD_FreeStrBuf(&local_300);
    PECMD_FreeStrBuf(&local_2e8);
    PECMD_FreeStrBuf(&local_2d0);
    PECMD_FreeStrBuf(&local_2c8);
    goto LAB_1400c8828;
b7c_next_item:                                       /* 原 code_r0x0001400c8753: 多行下一项 */
    PECMD_FreeStrBuf(&local_300);
    PECMD_FreeStrBuf(&local_2e8);
    PECMD_FreeStrBuf(&local_2d0);
    pWVar25 = local_310;
    goto LAB_1400c8787;
}

/* ---- TABL 列表控件族新增依赖声明 (实现位置见各函数注记) ---- */
#include <stdlib.h>
#include <string.h>
extern void *operator_new(size_t size);                                   /* @link_stubs.c 全局 new 包装 */
extern uint64_t FUN_1400630d0(int kind);                                  /* @0x1400630d0 内存告警重试 (link_stubs.c) */
extern void FUN_140063888(void *dst, const WCHAR *src, int64_t nbytes);   /* @0x140063888 定长串赋值 (link_stubs.c) */
extern short FUN_1400677b0(longlong *pcursor, longlong out4);             /* @0x1400677b0 四连整数解析 (link_stubs.c 真体) */
extern bool FUN_1400C1194(int64_t *pp, uint64_t *out);                    /* @0x1400c1194 表达式→整数 (core_exec5.c) */
extern void *FUN_1400bf358(void *ctx, void *expr, double *out);           /* @0x1400bf358 括号表达式求值 (link_stubs.c leaf 桩) */
extern uint64_t FUN_140067b54(uint16_t *pcursor, double *out);            /* @0x140067b54 数词解析 (link_stubs.c leaf 桩) */
extern longlong FUN_14006b8fc(longlong *self);                            /* @0x14006b8fc 控件尺寸计算 (link_stubs.c 桩) */
extern void FUN_14007d0ac(longlong *script, LPCWSTR key, LPCWSTR val);    /* @0x14007d0ac SetVariable (link_stubs.c 真体) */
extern void PECMD_ParseNumSkipChar_0224(int64_t *pp, int *out);           /* @0x140070224 数字+跳字 (core_b3b.c) */
extern void FUN_140053e78(void);                                          /* @0x140053e78 (link_stubs.c) */

/* 本批引用的全局数据槽 */
extern WCHAR g_szEmpty[];                 /* DAT_14011c638 .rdata 空串 */
extern uint8_t g_Script[0x200];           /* DAT_14013d130 表达式求值上下文 */
extern HANDLE g_hHeap;                    /* DAT_14013d328 进程堆 */
extern uint64_t g_u6435a0;                /* DAT_1401235a0 空 double 槽种子 */
extern double g_scrollLimit;              /* DAT_140129ee8 = 32768.0 */
extern double g_fontSizeDef;              /* DAT_1401293c0 默认字号位型 */
extern double g_dbl20b28;                 /* _DAT_140120b28 = 0.0 */
extern double g_dbl25230;                 /* DAT_140125230 = 0.5 四舍五入 */
extern double g_fontMinus0;               /* DAT_140125238 = -0.0 符号翻转 */
extern double g_fontRound;                /* DAT_140126070 ≈0.5 舍入补偿 */
extern double g_dpiBase;                  /* _DAT_140126078 = 72.0 基准字高 */
extern uint32_t g_dpi;                    /* DAT_14013d868 当前 DPI */
extern uint64_t g_pFontBase;              /* DAT_14013e2a8 默认字体句柄槽 */
extern uint8_t DAT_14011d308[];           /* 0x14011d308 缺省串 (link_stubs.c) */

/* win32_stub.h 已含 ShowWindow/HeapReAlloc 等声明; FUN_14001d78c 见文件后部 REGI 段 extern。 */

/* ---- .rdata 虚表地址占位 ----
 * 对象头仅存储虚表地址; 本 TU 内唯一被分派的虚方法是 +0x108 (应用字体),
 * 以具名数组承接保证可安全调用; 其余槽保持 0 (未被本批代码触及)。TODO(verify) */
typedef void (*b7c_vt_fn_t)(void *self, HFONT font, int flag);
static void b7c_listctrl_applyfont(void *self, HFONT font, int flag);
static const void *b7c_vt_PTR_1401234f0 = NULL;          /* 控件基类虚表 @0x1401234f0 */
static const void *b7c_vt_PTR_14012c550[0x120 / 8];      /* 表头子对象虚表 @0x14012c550 (任务所指「虚表 14012c550 关联」) */
static const void *b7c_vt_PTR_14012c670[0x120 / 8] = {   /* 列表控件对象虚表 @0x14012c670 */
    [0x108 / 8] = (const void *)&b7c_listctrl_applyfont
};
static const void *b7c_vt_PTR_140126888[0x120 / 8];      /* TABL 宿主对象虚表 @0x140126888 */

/* ========== FUN_140053dc8 @0x140053dc8 (控件宿主基类构造, 直移) ========== */
void *FUN_140053dc8(uint64_t *self, uint32_t cmd, uint64_t id,
                    LPCWSTR name, LPCWSTR clsA, LPCWSTR clsW,
                    uint32_t x, uint32_t y, uint32_t w, uint32_t h)
{
    *(uint32_t *)(self + 1) = cmd;                        /* [0x08] = 0x10 */
    *(const void **)self = (const void *)&b7c_vt_PTR_1401234f0;
    PECMD_StrAssign((WCHAR **)(self + 2), name);          /* [0x10] 名称串槽 */
    self[3] = 0;
    FUN_14006355c(self + 3, name, -1, ~(uint64_t)0);      /* [0x18] ANSI 名称槽 */
    PECMD_StrAssign((WCHAR **)(self + 4), clsW);          /* [0x20] 类名 W */
    PECMD_StrAssign((WCHAR **)(self + 5), clsA);          /* [0x28] 类名 A */
    self[6] = 0;
    self[7] = 0;
    self[10] = id;                                        /* [0x50] 命令 id */
    *(uint32_t *)(self + 8) = x;                          /* [0x40] X */
    *(uint32_t *)((char *)self + 0x44) = y;               /* Y */
    *(uint32_t *)(self + 9) = w;                          /* [0x48] W */
    *(uint32_t *)((char *)self + 0x4c) = h;               /* H */
    self[0xb] = id;                                       /* [0x58] */
    self[0xc] = 0;
    *(uint32_t *)(self + 0xd) = 0;                        /* [0x68] 子项计数 */
    return self;
}

/* ========== FUN_140053cec @0x140053cec (写 <名>.Visible/.Visable 环境变量, 直移) ========== */
void FUN_140053cec(longlong host, int visible)
{
    LPCWSTR name;
    LPWSTR buf;
    int len;
    LPCWSTR val;

    name = *(LPCWSTR *)((char *)host + 0x80);
    if (*name != L'\0') {
        val = (LPCWSTR)(const uint16_t *)L"0";
        if (visible != 0) {
            val = (LPCWSTR)(const uint16_t *)L"1";
        }
        len = lstrlenW(name);
        PECMD_AllocWStringBuffer(&buf, (int64_t)len + 0xb);
        lstrcpyW(buf, name);
        lstrcpyW(buf + len, (LPCWSTR)(const uint16_t *)L".Visible");
        if (**(short **)((char *)host + 0x80) != 0) {
            FUN_14007d0ac(*(longlong **)((char *)host + 0x50), buf, val);
        }
        buf[len + 4] = L'a';                              /* 二进制原样: 第二次改写为 .Visable */
        if (**(short **)((char *)host + 0x80) != 0) {
            FUN_14007d0ac(*(longlong **)((char *)host + 0x50), buf, val);
        }
        PECMD_FreeStrBuf((WCHAR **)&buf);
    }
}

/* ========== FUN_140063118 @0x140063118 (向量缓冲分配/扩容, 直移) ========== */
longlong *FUN_140063118(unsigned char *old, longlong nbytes)
{
    longlong *hdr;
    longlong oldcap;
    int rv;

    if (old == NULL) {
        for (;;) {
            hdr = (longlong *)HeapAlloc(g_hHeap, 0, nbytes + 8);
            if (hdr != NULL) break;
            rv = (int)FUN_1400630d0(2);
            if (rv != 4) break;                           /* 原体 while(iVar==4) 重试 */
        }
        *(uint32_t *)((char *)hdr + 4) = 0xaa55;
        hdr[0] = nbytes;
        return hdr + 1;
    }
    oldcap = *(longlong *)(old - 8);
    hdr = (longlong *)HeapReAlloc(g_hHeap, 0, old - 8, nbytes + 8);
    if (hdr == NULL) {
        for (;;) {
            hdr = (longlong *)HeapAlloc(g_hHeap, 0, nbytes + 8);
            if (hdr != NULL) break;
            rv = (int)FUN_1400630d0(2);
            if (rv != 4) break;
        }
        *(uint32_t *)((char *)hdr + 4) = 0xaa55;
        hdr[0] = nbytes;
        if (nbytes < oldcap) {
            oldcap = nbytes;
        }
        hdr = hdr + 1;
        FUN_14001d78c((unsigned char *)hdr, old, oldcap);
        HeapFree(g_hHeap, 0, old - 8);
    }
    else {
        *(uint32_t *)((char *)hdr + 4) = 0xaa55;
        hdr[0] = nbytes;
        hdr = hdr + 1;
    }
    return hdr;
}

/* ========== FUN_140063978 @0x140063978 (向量按增量扩容, 直移) ========== */
void FUN_140063978(longlong *pdata, longlong *pcapa, uint32_t elemsz, longlong grow)
{
    int newcap;

    newcap = (int)grow + (int)*pcapa;
    *pdata = (longlong)(uintptr_t)FUN_140063118((unsigned char *)(uintptr_t)*pdata, (ulonglong)newcap * elemsz);
    memset((unsigned char *)*pdata + *pcapa * elemsz, 0, (size_t)(elemsz * grow));
    *pcapa = newcap;
}

/* ========== FUN_140063b00 @0x140063b00 (向量第 idx 元素地址, 自动扩容, 直移) ========== */
longlong FUN_140063b00(longlong idx, longlong *pdata, longlong *pcapa, uint32_t elemsz)
{
    if (idx < 0) {
        idx = 0;
    }
    if ((ulonglong)(*pcapa - 1) <= (ulonglong)idx) {
        FUN_140063978(pdata, pcapa, elemsz, idx - *pcapa + 2);
    }
    return (ulonglong)elemsz * idx + *pdata;
}

/* ========== FUN_1400639f0 @0x1400639f0 (向量 push_back, 直移) ========== */
longlong FUN_1400639f0(longlong *pdata, longlong *pcapa, longlong *pcount,
                       unsigned char *elem, uint32_t elemsz, longlong grow)
{
    longlong oldcnt;
    longlong base;

    if ((ulonglong)(*pcapa - 1) <= (ulonglong)*pcount) {
        oldcnt = *pcapa + grow;
        *pcapa = oldcnt;
        *pdata = (longlong)FUN_140063118((unsigned char *)*pdata, elemsz * oldcnt);
    }
    FUN_14001d78c((unsigned char *)(*pcount * elemsz + *pdata), elem, elemsz);
    oldcnt = *pcount;
    base = *pdata;
    *pcount = oldcnt + 1;
    return base + oldcnt;
}

/* ========== FUN_140063b64 @0x140063b64 (向量槽初始化 {data,cap=1,count=0}, 直移) ========== */
longlong *FUN_140063b64(longlong *v)
{
    longlong cap;
    longlong i;
    unsigned char *p;
    int rv;

    v[2] = 0;
    v[0] = 0;
    v[1] = 1;
    for (;;) {
        p = (unsigned char *)HeapAlloc(g_hHeap, 0, 0x10);
        if (p != NULL) break;
        rv = (int)FUN_1400630d0(2);
        if (rv != 4) break;
    }
    *(uint32_t *)(p + 4) = 0xaa55;
    *(longlong *)p = 8;
    cap = v[1];
    v[0] = (longlong)(uintptr_t)(p + 8);
    for (i = 0; i < cap; i++) {
        *(uint64_t *)(v[0] + i * 8) = 0;
    }
    return v;
}

/* ========== FUN_1400e57c0 @0x1400e57c0 (控件对象基类构造, 直移) ========== */
void *FUN_1400e57c0(uint64_t *o)
{
    *(const void **)o = (const void *)&b7c_vt_PTR_1401234f0;
    o[4] = 0;                                   /* HWND @byte 0x20 */
    o[7] = 0;
    o[0x18] = 0;
    *(uint8_t *)((char *)o + 0xf * 8) &= (uint8_t)~3u;
    *(uint8_t *)((char *)o + 0x14 * 8) = 0xff;  /* byte 0xa0 */
    *(uint8_t *)((char *)o + 0x17 * 8) = 0xff;  /* byte 0xb8 */
    *(uint16_t *)((char *)o + 0xa2) = 0xffff;
    o[0x19] = 0;
    *(uint32_t *)(o + 8) = 0x80000000u;         /* byte 0x40 */
    o[6] = 0;
    o[5] = 0;
    *(uint8_t *)((char *)o + 0x61) = 0;
    o[0x11] = 0;
    o[0x10] = 0;
    *(uint32_t *)(o + 0x15) = 0x80000000u;      /* byte 0xa8 */
    *(uint8_t *)((char *)o + 0xac) = 0;
    *(uint8_t *)((char *)o + 0xa4) = 0;
    *(uint32_t *)((char *)o + 0x9c) = 0x80000000u;
    *(uint32_t *)(o + 0x13) = 0x80000000u;      /* byte 0x98 */
    *(uint32_t *)((char *)o + 0x94) = 0x80000000u;
    *(uint32_t *)(o + 0x12) = 0x80000000u;      /* byte 0x90 */
    *(uint8_t *)((char *)o + 0xa5) = 0;
    *(uint8_t *)((char *)o + 0xad) = 0;
    *(uint8_t *)((char *)o + 0xa1) = 0;
    *(uint32_t *)((char *)o + 0x44) = 0x80000000u;
    o[9] = 0;
    *(uint32_t *)((char *)o + 0x54) = 0x80000000u;
    *(uint32_t *)(o + 0xb) = 0xdefbacu;         /* byte 0x58 缺省底色 */
    *(uint32_t *)((char *)o + 0x5c) = 0xff0000u;
    *(uint32_t *)(o + 10) = 0xbf800000u;        /* byte 0x50 位型 */
    *(uint8_t *)((char *)o + 0xc * 8) = 0x4c;   /* byte 0x60 */
    o[0xe] = 0;
    o[0xd] = 0;
    return o;
}

/* ========== FUN_1400f5c74 @0x1400f5c74 (列表表头子对象构造, 直移;
 *   安装虚表 PTR_FUN_14012c550 — 即任务所指「虚表 14012c550 关联」) ========== */
void *FUN_1400f5c74(uint64_t *o)
{
    int64_t tmp;

    FUN_1400e57c0(o);
    *(const void **)o = (const void *)&b7c_vt_PTR_14012c550;
    FUN_140063b64((longlong *)(o + 0x1a));
    FUN_140063b64((longlong *)(o + 0x1d));
    PECMD_AllocStrSlot((WCHAR **)(o + 0x20));
    *(uint32_t *)((char *)o + 0x124) = 0;
    *(uint32_t *)(o + 0x23) = 0xabu;            /* byte 0x118 */
    *(uint32_t *)((char *)o + 0x11c) = 199;
    *(uint32_t *)(o + 0x24) = 0xebu;            /* byte 0x120 */
    *(uint8_t *)((char *)o + 0x26 * 8) = 1;     /* byte 0x130 */
    PECMD_StrBldCopyAnsi(&tmp, (const char *)DAT_14011d308, ~(uint64_t)0);
    PECMD_AssignString((int64_t *)(o + 0x20), *(LPCWSTR *)&tmp);  /* = FUN_140070398 同体 */
    PECMD_FreeStrBuf((WCHAR **)&tmp);
    *(uint32_t *)((char *)o + 0x134) = 0;
    o[0x22] = 0;
    o[0x21] = 0;
    o[0x27] = 0;
    o[0x25] = 0x3ff0000000000000ULL;            /* double 1.0 */
    return o;
}

/* ========== FUN_1400f9134 @0x1400f9134 (列表控件对象构造, operator_new(0x400) 体, 直移) ========== */
uint64_t *FUN_1400f9134(uint64_t *mem, uint64_t owner, uint64_t host)
{
    FUN_1400e57c0(mem);
    *(const void **)mem = (const void *)&b7c_vt_PTR_14012c670;
    mem[0x1a] = host;
    FUN_1400f5c74(mem + 0x1b);
    *(uint32_t *)((char *)mem + 0x43 * 8) = 0;  /* byte 0x218 */
    FUN_140063b64((longlong *)(mem + 0x4c));
    FUN_140063b64((longlong *)(mem + 0x4f));
    FUN_140063b64((longlong *)(mem + 0x52));
    FUN_140063b64((longlong *)(mem + 0x55));
    FUN_140063b64((longlong *)(mem + 0x58));
    FUN_140063b64((longlong *)(mem + 0x5b));
    FUN_140063b64((longlong *)(mem + 0x5e));
    FUN_140063b64((longlong *)(mem + 0x61));
    FUN_140063b64((longlong *)(mem + 100));     /* 元素 100 = byte 0x320 */
    FUN_140063b64((longlong *)(mem + 0x67));
    FUN_140063b64((longlong *)(mem + 0x6a));
    mem[0x6d] = 0;
    mem[0x6e] = 0;
    mem[0x6f] = 0;
    mem[0x70] = 0;
    mem[0x71] = 0;
    mem[0x72] = 0;
    mem[0x73] = 0;
    mem[0x74] = 0;
    mem[0x75] = 0;
    mem[0x7b] = 0x4028000000000000ULL;          /* double 12.0 行高系数槽 */
    *(uint32_t *)((char *)mem + 0x3b4) = 0;
    *(uint32_t *)((char *)mem + 0x3bc) = 0xffffffffu;
    *(uint32_t *)((char *)mem + 0x77 * 8) = 0xffffffffu;  /* byte 0x3b8 */
    *(uint32_t *)((char *)mem + 0x3d4) = 0xffffffe0u;
    *(uint32_t *)((char *)mem + 0x7a * 8) = 0xffffffe0u;  /* byte 0x3d0 */
    *(uint32_t *)((char *)mem + 0x3cc) = 0xffffffe0u;
    mem[0x7c] = 0;
    *(uint16_t *)((char *)mem + 0x7f * 8) = 0;  /* byte 0x3f8 标志字 */
    *(uint8_t *)((char *)mem + 0x14 * 8) = 0;   /* byte 0xa0 */
    *(uint8_t *)((char *)mem + 0x4a * 8) = 0;   /* byte 0x250 */
    *(uint32_t *)((char *)mem + 0x21c) = 0;
    mem[0x46] = 0;
    mem[0x45] = 0;
    mem[0x44] = 0;
    *(uint16_t *)((char *)mem + 0x76 * 8) = 0;  /* byte 0x3b0 */
    *(uint8_t *)((char *)mem + 0x4b * 8) = 0;   /* byte 0x258 */
    mem[0x49] = 0;
    mem[0x48] = 0;
    *(uint16_t *)((char *)mem + 0x23a) = 0;
    *(uint16_t *)((char *)mem + 0x47 * 8) = 0;  /* byte 0x238 */
    *(uint32_t *)((char *)mem + 0x78 * 8) = 0xfffff000u;  /* byte 0x3c0 */
    mem[0x7d] = owner;                          /* 宿主 TABL 对象 */
    *(uint32_t *)((char *)mem + 0x79 * 8) = 0x80000000u;  /* byte 0x3c8 文本色 */
    *(uint32_t *)((char *)mem + 0x3c4) = 0xffffffu;       /* 文本底色 */
    mem[0x42] = (uint64_t)(uintptr_t)mem;       /* 自引用 */
    mem[0x7e] = 0xfffffffffffffff0ULL;
    *(uint32_t *)((char *)mem + 0x254) = 0x80000000u;
    return mem;
}

/* ========== FUN_140064a88 @0x140064a88 (十进数字串取值, 直移) ========== */
longlong PECMD_ParseDecDigits_b7c(uint16_t *p)
{
    longlong v = 0;
    for (; (uint16_t)(*p - '0') < 10; p++) {
        v = (longlong)(int32_t)(*p - '0') + v * 10;
    }
    return v;
}

/* ========== FUN_140067b78 @0x140067b78 (整词解析 hex/oct/bin/dec, 直移) ========== */
uint64_t FUN_140067b78(longlong *pcursor, uint64_t *out)
{
    uint16_t *p;
    char neg;
    uint16_t ch;

    FUN_14005B154((WCHAR **)pcursor);
    p = (uint16_t *)(uintptr_t)*pcursor;
    neg = '\0';
    if (*p == '-') {
        p++;
        neg = -1;
    }
    if (*p == '0') {
        ch = (uint16_t)(p[1] | 0x20);
        if (ch == 'x') {
            *out = 0;
            p += 2;
            *out = PECMD_ParseHex_4a34(p);
            *pcursor = (longlong)(uintptr_t)p;
            for (;;) {
                ch = *(uint16_t *)(uintptr_t)*pcursor;
                if (((ch < '0') || ('9' < ch)) && (5 < (uint16_t)((ch | 0x20) - 'a'))) break;
                *pcursor += 2;
            }
            goto neg_done;
        }
        if (ch == 'o') {
            *out = 0;
            p += 2;
            *pcursor = (longlong)(uintptr_t)p;
            while ((*p > '/') && (*(uint16_t *)(uintptr_t)*pcursor < '8')) {
                *out = (longlong)(int32_t)(*(uint16_t *)(uintptr_t)*pcursor - '0') | (*out << 3);
                *pcursor += 2;
                p = (uint16_t *)(uintptr_t)*pcursor;
            }
            goto neg_done;
        }
        if (ch == 'b') {
            *out = 0;
            p += 2;
            *pcursor = (longlong)(uintptr_t)p;
            while ((*p > '/') && (*(uint16_t *)(uintptr_t)*pcursor < '2')) {
                *out = (longlong)(int32_t)(*(uint16_t *)(uintptr_t)*pcursor - '0') | (*out * 2);
                *pcursor += 2;
                p = (uint16_t *)(uintptr_t)*pcursor;
            }
            goto neg_done;
        }
    }
    if (9 < (uint16_t)(*p - '0')) {
        return 0;
    }
    *out = (uint64_t)PECMD_ParseDecDigits_b7c(p);
    *pcursor = (longlong)(uintptr_t)p;
    ch = *p;
    while ((ch > '/') && (*(uint16_t *)(uintptr_t)*pcursor < ':')) {
        p = (uint16_t *)(uintptr_t)*pcursor + 1;
        *pcursor = (longlong)(uintptr_t)p;
        ch = *p;
    }
neg_done:
    if (neg != '\0') {
        *out = -*out;
    }
    return 1;
}

/* ========== FUN_14006a740 @0x14006a740 (整词 + T/G/M/K/S 单位尾, 直移) ========== */
uint64_t FUN_14006a740(longlong *pcursor, uint64_t *out)
{
    uint16_t ch;

    FUN_14005B154((WCHAR **)pcursor);
    if (FUN_140067b78(pcursor, out) < 1) {
        return 0;
    }
    ch = *(uint16_t *)(uintptr_t)*pcursor & 0xffdf;
    if (ch == 'T') {
        *out = *out << 40;
    }
    else if (ch == 'G') {
        *out = *out << 30;
    }
    else if (ch == 'M') {
        *out = *out << 20;
    }
    else if (ch == 'K') {
        *out = *out << 10;
    }
    else {
        if (ch != 'S') {
            return 1;                     /* 非单位: 原体 -2 再 +2 抵消, 字符不消费 */
        }
        *out = *out << 9;
        *pcursor += 2;
        return 1;
    }
    *pcursor += 2;
    return 1;
}

/* ========== FUN_14006a7f4 @0x14006a7f4 (整数解析包装, 成功后跳空白, 直移) ========== */
uint64_t FUN_14006a7f4(longlong *pcursor, uint64_t *out)
{
    uint64_t rv = FUN_14006a740(pcursor, out);
    if ((int)rv != 0) {
        FUN_14005B154((WCHAR **)pcursor);
    }
    return rv & 0xffffffffULL;
}

/* ========== FUN_1400c11f4 @0x1400c11f4 (整数解析: 括号表达式优先, 直移) ========== */
uint64_t FUN_1400c11f4(longlong *pcursor, uint64_t *out)
{
    uint16_t *p;
    uint16_t *close;
    WCHAR *start;
    int depth;
    double val;

    FUN_14005B154((WCHAR **)pcursor);
    p = (uint16_t *)(uintptr_t)*pcursor;
    close = NULL;
    if (*p == '(') {
        start = (WCHAR *)(p + 1);
        depth = 1;
        p++;
        for (;;) {
            if (*p == 0) {
                *pcursor = (longlong)(uintptr_t)p;
                break;
            }
            if (*p == '(') {
                depth++;
            }
            else if (*p == ')') {
                depth--;
                if (depth <= 0) {
                    close = p;
                    *pcursor = (longlong)(uintptr_t)(close + 1);
                    *close = 0;
                    break;
                }
            }
            p++;
        }
        val = (double)(longlong)*out;
        FUN_1400bf358((void *)g_Script, start, &val);
        if (close != NULL) {
            *close = ')';
        }
        FUN_14005B154((WCHAR **)pcursor);
        if (0.0 <= val) {
            if (0.0 < val) {
                val += g_dbl25230;
            }
        }
        else {
            val -= g_dbl25230;
        }
        *out = (uint64_t)(int64_t)val;
        return 1;
    }
    return FUN_14006a7f4(pcursor, out);
}

/* ========== FUN_140077e48 @0x140077e48 (浮点词 + T/G/M/K/S 乘数尾, 直移) ========== */
uint64_t FUN_140077e48(longlong *pcursor, double *out)
{
    uint16_t *q;
    uint16_t ch;
    double scaled;
    uint64_t rv;

    FUN_14005B154((WCHAR **)pcursor);
    q = (uint16_t *)(uintptr_t)*pcursor;
    rv = FUN_140067b54((uint16_t *)pcursor, out);
    if (0 < (int)rv) {
        if (*q == '-') {
            q++;
        }
        ch = *q;
        if ((ch == '0') && ((q[1] | 0x20) == 'x')) {
            for (q += 2; (((ch = *q) > '/') && (ch < ':')) ||
                         ((((ch | 0x20)) > '`') && ((ch | 0x20) < 'g')); q += 1) {
            }
        }
        else {
            while ((ch > '/') && (ch < ':')) {
                q++;
                ch = *q;
            }
            if (*q == '.') {
                do {
                    q++;
                    if (*q < '0') break;
                } while (*q < ':');
            }
        }
        ch = *q & 0xffdf;
        if (ch == 'T') {
            scaled = *out * 1099511627776.0;       /* _DAT_1401270a8 */
        }
        else if (ch == 'G') {
            scaled = *out * 1073741824.0;          /* _DAT_1401270a0 */
        }
        else if (ch == 'M') {
            scaled = *out * 1048576.0;             /* _DAT_140127098 */
        }
        else if (ch == 'K') {
            scaled = *out * 1024.0;                /* _DAT_140127090 */
        }
        else {
            if (ch != 'S') {
                return rv & 0xffffffffULL;
            }
            scaled = *out * 512.0;                 /* _DAT_140127088 */
        }
        *out = scaled;
        *pcursor = (longlong)(uintptr_t)(q + 1);
        rv &= 0xffffffffULL;
    }
    return rv;
}

/* ========== FUN_140077f90 @0x140077f90 (浮点词包装, 成功后跳空白, 直移) ========== */
uint64_t FUN_140077f90(longlong *pcursor, double *out)
{
    uint64_t rv = FUN_140077e48(pcursor, out);
    if ((int)rv != 0) {
        FUN_14005B154((WCHAR **)pcursor);
    }
    return rv & 0xffffffffULL;
}

/* ========== FUN_140082448 @0x140082448 (浮点解析: 括号表达式优先, 直移) ========== */
uint64_t FUN_140082448(longlong *pcursor, double *out)
{
    uint16_t *p;
    uint16_t *close;
    WCHAR *start;
    int depth;
    double val;

    FUN_14005B154((WCHAR **)pcursor);
    p = (uint16_t *)(uintptr_t)*pcursor;
    close = NULL;
    if (*p == '(') {
        start = (WCHAR *)(p + 1);
        depth = 1;
        p++;
        for (;;) {
            if (*p == 0) {
                *pcursor = (longlong)(uintptr_t)p;
                break;
            }
            if (*p == '(') {
                depth++;
            }
            else if (*p == ')') {
                depth--;
                if (depth <= 0) {
                    close = p;
                    *pcursor = (longlong)(uintptr_t)(close + 1);
                    *close = 0;
                    break;
                }
            }
            p++;
        }
        val = *out;
        FUN_1400bf358((void *)g_Script, start, &val);
        *out = val;
        if (close != NULL) {
            *close = ')';
        }
        FUN_14005B154((WCHAR **)pcursor);
        return 1;
    }
    return FUN_140077f90(pcursor, out);
}

/* ========== FUN_140067d54 @0x140067d54 (浮点解析: 括号表达式优先, 直移) ========== */
uint64_t FUN_140067d54(longlong *pcursor, double *out)
{
    uint16_t *p;
    uint16_t *close;
    WCHAR *start;
    int depth;
    double val;

    FUN_14005B154((WCHAR **)pcursor);
    p = (uint16_t *)(uintptr_t)*pcursor;
    close = NULL;
    if (*p == '(') {
        start = (WCHAR *)(p + 1);
        depth = 1;
        p++;
        for (;;) {
            if (*p == 0) {
                *pcursor = (longlong)(uintptr_t)p;
                break;
            }
            if (*p == '(') {
                depth++;
            }
            else if (*p == ')') {
                depth--;
                if (depth <= 0) {
                    close = p;
                    *pcursor = (longlong)(uintptr_t)(close + 1);
                    *close = 0;
                    break;
                }
            }
            p++;
        }
        val = 0.0;
        FUN_1400bf358((void *)g_Script, start, &val);
        *out = val;
        if (close != NULL) {
            *close = ')';
        }
        FUN_14005B154((WCHAR **)pcursor);
        return 1;
    }
    return FUN_140067b54((uint16_t *)pcursor, out) & 0xffffffffULL;
}

/* ========== FUN_14007de4c @0x14007de4c (浮点解析并跳过尾字符, 直移) ========== */
void FUN_14007de4c(longlong *pcursor, double *out)
{
    FUN_140067d54(pcursor, out);
    if (*(uint16_t *)(uintptr_t)*pcursor != 0) {
        *pcursor += 2;
    }
}

/* ========== FUN_140067e2c @0x140067e2c (带初值浮点解析→int, 四舍五入, 直移) ========== */
uint64_t FUN_140067e2c(longlong *pcursor, int *out)
{
    uint64_t rv;
    double val;

    val = (double)*out;
    rv = FUN_140067d54(pcursor, &val);
    if ((int)rv < 1) {
        return 0;
    }
    if (val < g_dbl20b28) {
        val -= g_dbl25230;
    }
    else {
        val += g_dbl25230;
    }
    *out = (int)val;
    return 1;
}

/* ========== FUN_140066850 @0x140066850 (颜色词: '@'前缀走 GetSysColor, 直移) ========== */
int FUN_140066850(longlong *pcursor, uint64_t *out)
{
    int is_sys;
    int ok;

    is_sys = (*(uint16_t *)(uintptr_t)*pcursor == '@');
    if (is_sys != 0) {
        *pcursor += 2;
    }
    ok = FUN_1400C1194((int64_t *)pcursor, out) ? 1 : 0;
    if (is_sys != 0) {
        *out = (uint64_t)GetSysColor((int)*out);
    }
    return ok;
}

/* ========== FUN_1400628b4 @0x1400628b4 (DPI 换算取整, 直移) ========== */
int FUN_1400628b4(double v)
{
    double s;

    if ((int)g_dpi < 1) {
        return (int)v;
    }
    s = ((double)(int)g_dpi * v) / g_dpiBase;
    if (s < 0.0) {
        s = g_fontMinus0 - s;
    }
    return (int)(s + g_fontRound);
}

/* ========== FUN_1400e5890 @0x1400e5890 (WM_GETFONT 取字体, 失败回缺省, 直移) ========== */
LRESULT FUN_1400e5890(longlong host)
{
    LRESULT f = SendMessageW(*(HWND *)((char *)host + 0x20), 0x31, 0, 0);
    if ((f == -1) || (f == 0)) {
        f = (LRESULT)g_pFontBase;
    }
    return f;
}

/* ========== FUN_1400703e4 @0x1400703e4 (串槽整体赋值, 直移) ========== */
void FUN_1400703e4(void *slot, LPCWSTR src)
{
    FUN_140063888(slot, src, -1);
}

/* ========== FUN_14005b0b8 @0x14005b0b8 (定长缓冲清零 [容量在 [-8]), 直移) ========== */
void FUN_14005b0b8(unsigned char *p)
{
    if (p != NULL) {
        memset(p, 0, *(longlong *)(p - 8));
    }
}

/* ========== FUN_1400a8664 @0x1400a8664 ("面名**BIUS#宽,高…" → LOGFONT, 忠实直移) ========== */
void FUN_1400a8664(longlong lf, LPWSTR spec, uint32_t *pflags)
{
    WCHAR *tail;
    WCHAR c;
    LPCWSTR unq;
    int iv;
    double dv;

    if ((spec != NULL) && (*spec != L'\0')) {
        c = *spec;
        tail = spec;
        while (c != L'\0') {
            if ((c == L'*') && (tail[1] == L'*')) {
                *tail = L'\0';
                tail += 2;
                break;
            }
            tail++;
            c = *tail;
        }
        unq = (uint16_t *)PECMD_UnquoteString((short *)spec);
        if (unq != spec) {
            lstrcpyW(spec, unq);
        }
        if (*tail != L'\0') {
            b7c_skip_to((WCHAR **)&tail, L'#');
            if (*tail != L'\0') {
                *tail = L'\0';
                tail++;
            }
            /* B/b/I/i/U/u/S/s 开关族 (StrChrW 全串检索) */
            if (StrChrW(tail, L'B') != NULL) *(uint32_t *)((char *)lf + 0x10) = 700;
            if (StrChrW(tail, L'b') != NULL) *(uint32_t *)((char *)lf + 0x10) = 400;
            if (StrChrW(tail, L'I') != NULL) *(uint8_t *)((char *)lf + 0x14) = 1;
            if (StrChrW(tail, L'i') != NULL) *(uint8_t *)((char *)lf + 0x14) = 0;
            if (StrChrW(tail, L'U') != NULL) *(uint8_t *)((char *)lf + 0x15) = 1;
            if (StrChrW(tail, L'u') != NULL) *(uint8_t *)((char *)lf + 0x15) = 0;
            if (StrChrW(tail, L'S') != NULL) *(uint8_t *)((char *)lf + 0x16) = 1;
            if (StrChrW(tail, L's') != NULL) *(uint8_t *)((char *)lf + 0x16) = 0;
            iv = 0;
            PECMD_ParseNumSkipChar_0224((int64_t *)&tail, &iv);
            if (iv != 0) {
                *(uint32_t *)((char *)lf + 0x10) = (uint32_t)iv;
                if (pflags != NULL) {
                    *pflags |= 0x80000000u;
                }
            }
            dv = 0.0;
            FUN_14007de4c((longlong *)&tail, &dv);
            if (dv != 0.0) {
                double s = dv;
                if ((int)g_dpi != 0) {
                    if (dv < 0.0) {
                        s = g_fontMinus0 - dv;
                    }
                    s = ((double)(int)g_dpi * s) / g_dpiBase;
                }
                *(int32_t *)((char *)lf + 4) = -(int)(s + g_fontRound);
            }
            iv = -1;
            PECMD_ParseNumSkipChar_0224((int64_t *)&tail, &iv);
            if (-1 < iv) {
                *(char *)((char *)lf + 0x17) = (char)iv;
                if (pflags != NULL) {
                    *pflags |= 0x8000000u;
                }
            }
            PECMD_ParseNumSkipChar_0224((int64_t *)&tail, &iv);
            if (-1 < iv) *(char *)((char *)lf + 0x1a) = (char)iv;
            PECMD_ParseNumSkipChar_0224((int64_t *)&tail, &iv);
            if (-1 < iv) *(int32_t *)((char *)lf + 0xc) = iv;
            PECMD_ParseNumSkipChar_0224((int64_t *)&tail, &iv);
            if (-1 < iv) *(int32_t *)((char *)lf + 8) = iv;
            PECMD_ParseNumSkipChar_0224((int64_t *)&tail, &iv);
            if (-1 < iv) *(char *)((char *)lf + 0x18) = (char)iv;
            PECMD_ParseNumSkipChar_0224((int64_t *)&tail, &iv);
            if (-1 < iv) *(char *)((char *)lf + 0x19) = (char)iv;
            PECMD_ParseNumSkipChar_0224((int64_t *)&tail, &iv);
            if (-1 < iv) *(char *)((char *)lf + 0x1b) = (char)iv;
        }
    }
}

/* ========== FUN_1400b1f34 @0x1400b1f34 (LOGFONT + "[~|^]高[:面名]" → HFONT, 忠实直移) ========== */
HFONT FUN_1400b1f34(int *lf, double *ph, LPCWSTR face)
{
    double rnd;
    double h;
    int dpi;
    int ih;
    int iw;
    int bold;
    int imin;
    int prec;
    int flag9;
    LPCWSTR psz;
    WCHAR *slot;
    WCHAR *cur;
    HFONT hf;
    uint64_t rv;

    dpi = (int)g_dpi;
    rnd = g_fontRound;
    h = *ph;
    if (h < 0.0) {
        h -= rnd;
    }
    else {
        h += rnd;
    }
    ih = *lf;
    iw = (int)h;
    flag9 = 0;
    if (ih < 0) {
        ih = -ih;
    }
    PECMD_StrAssign(&slot, face);
    h = g_fontMinus0;                            /* dVar11 复用为负零 */
    bold = 0;
    imin = (int)0x80000000;
    cur = slot;
    if ((iw < 1) || (*lf == (int)0x80000000)) {
        if (iw == (int)0x80000000) {
            double seed = 0.0;
            rv = FUN_140082448((longlong *)&cur, &seed);
            if ((int)rv != 0) {
                double t = seed;
                if (seed < 0.0) {
                    t = g_fontMinus0 - seed;
                }
                *ph = t;
                iw = (int)(t + rnd);
            }
            flag9 = 1;
            goto LAB_2047;
        }
        if (iw == -1) {
            iw = (int)0x80000000;
        }
        goto LAB_211a;
    }
    else {
        double t;
        flag9 = 0;
        (void)t;
LAB_2047:
        bold = 0;
        imin = (int)0x80000000;
        prec = dpi;
        if (*cur == L'~') {
            cur++;
            if (dpi != 0) {
                prec = 0x60;
            }
            goto LAB_209a;
        }
        else if (*cur == L'^') {
            cur++;
            flag9 = 4;
            if (dpi == 0) {
                goto LAB_209a;
            }
            prec = 0x48;
        }
        if ((flag9 != 0) || (*cur == L':')) {
LAB_209a:
            imin = (int)0x80000000;
            bold = 0;
            FUN_14005B154((WCHAR **)&cur);
            slot = cur;
            if (*cur != (WCHAR)bold) {
                slot = cur + 1;
            }
        }
        psz = slot;
        t = 0.0;
        (void)t;
        if (iw != imin) {
            if (dpi != bold) {
                if (*ph <= 0.0 && *ph != 0.0) {
                    *ph = h - *ph;
                }
                h = ((double)prec * *ph) / g_dpiBase;
                *ph = h;
                ih = (int)(h + rnd);
                *lf = -ih;
                if (lf[1] != bold) {
                    lf[1] = -ih / 2;
                }
                goto LAB_2151;
            }
            goto LAB_211a;
        }
        goto LAB_2152;
    }
LAB_211a:
    psz = slot;
    if (iw != imin) {
        *lf = iw;
        ih = iw;
    }
LAB_2152:
    if ((ih != imin) && (lf[1] != bold)) {
        int t2 = -ih;
        if (bold <= ih) {
            t2 = ih;
        }
        lf[1] = -(t2 / 2);
    }
    if ((ih < bold) && (ih != imin)) {
        ih = -ih;
    }
LAB_2151:
    FUN_1400a8664((longlong)lf, (LPWSTR)psz, (uint32_t *)NULL);
    if ((psz == NULL) || (*psz == L'\0')) {
        psz = (LPCWSTR)(lf + 7);
    }
    hf = CreateFontW(-ih, lf[1], lf[2], lf[3], lf[4],
                     (DWORD)*(uint8_t *)((char *)lf + 20),
                     (DWORD)*(uint8_t *)((char *)lf + 21),
                     (DWORD)*(uint8_t *)((char *)lf + 22),
                     (DWORD)*(uint8_t *)((char *)lf + 23),
                     (DWORD)*(uint8_t *)((char *)lf + 24),
                     (DWORD)*(uint8_t *)((char *)lf + 25),
                     (DWORD)*(uint8_t *)((char *)lf + 26),
                     (DWORD)*(uint8_t *)((char *)lf + 27),
                     psz);
    PECMD_FreeStrBuf((WCHAR **)&slot);
    return hf;
}

/* ========== FUN_1400b89dc @0x1400b89dc (克隆现有字体再调参创建, 直移) ========== */
HFONT FUN_1400b89dc(HANDLE font, double *ph, LPCWSTR face)
{
    unsigned char *buf;
    HFONT hf;

    PECMD_AllocStringSlot2((void **)&buf, 0x5c);
    FUN_14005b0b8(buf);
    *(uint32_t *)(buf + 16) = 400;
    GetObjectW(font, 0x5c, buf);
    *(uint32_t *)(buf + 4) = 0;
    *(uint8_t *)(buf + 0x17) = 1;
    hf = FUN_1400b1f34((int *)buf, ph, face);
    PECMD_FreeStrBuf((WCHAR **)&buf);
    return hf;
}

/* ========== FUN_1400f2b44 @0x1400f2b44 (存表头高度并刷新, 直移) ========== */
bool FUN_1400f2b44(longlong *sub, longlong val)
{
    sub[4] = val;
    FUN_14006b8fc(sub);
    return sub[4] != 0;
}

/* ========== FUN_1400f345c @0x1400f345c (创建 SysListView32 子窗口, 直移) ========== */
bool FUN_1400f345c(longlong *self, uint32_t style, int *rect, HWND parent, uint32_t id)
{
    int l = rect[0];
    int t = rect[1];
    int r = rect[2];
    int b = rect[3];
    LONG inst;
    HWND wnd;

    self[0x42] = (longlong)(uintptr_t)self;
    inst = GetWindowLongW(parent, -6);            /* GWL_HINSTANCE */
    wnd = CreateWindowExW(0, (LPCWSTR)(const uint16_t *)L"SysListView32", (LPCWSTR)0,
                          style | 0x400u, l, t, r - l, b - t, parent,
                          (HMENU)(uintptr_t)id, (HINSTANCE)(uintptr_t)(uintptr_t)inst, NULL);
    self[4] = (longlong)(uintptr_t)wnd;
    FUN_14006b8fc(self);
    UpdateWindow((HWND)(uintptr_t)self[4]);
    return self[4] != 0;
}

/* ========== FUN_1400f353c @0x1400f353c (LVM_SETEXTENDEDLISTVIEWSTYLE, 直移) ========== */
void FUN_1400f353c(longlong self, uint32_t style)
{
    SendMessageW(*(HWND *)((char *)self + 0x20), 0x1036, 0, style);
}

/* ========== FUN_1400f3554 @0x1400f3554 (LVM_INSERTITEMW + LVM_SETITEMSTATE, 直移) ========== */
uint64_t FUN_1400f3554(longlong self, LPARAM lvitem)
{
    uint64_t idx = SendMessageW(*(HWND *)((char *)self + 0x20), 0x104d, 0, lvitem);
    uint8_t st[0x14];
    memset(st, 0, sizeof(st));
    *(uint32_t *)(st + 0xc) = 0x1000u;            /* state = LVIS_STATEIMAGEMASK(1) */
    *(uint32_t *)(st + 0x10) = 0xf000u;           /* stateMask */
    SendMessageW(*(HWND *)((char *)self + 0x20), 0x102b, (longlong)(int)idx, (LPARAM)st);
    return idx & 0xffffffffULL;
}

/* ========== FUN_1400f3674 @0x1400f3674 (向窗口发 0x47 尺寸同步消息, 直移) ========== */
void FUN_1400f3674(HWND wnd)
{
    RECT rc;
    unsigned char blob[0x28];
    int w;
    int h;

    rc.left = rc.top = rc.right = rc.bottom = 0;
    GetWindowRect(wnd, &rc);
    memset(blob, 0, sizeof(blob));
    w = rc.right - rc.left;
    h = rc.bottom - rc.top;
    *(HWND *)(blob + 0) = wnd;                    /* local_38 */
    *(int *)(blob + 0x18) = w;                    /* local_20 */
    *(int *)(blob + 0x1c) = h;                    /* local_1c */
    *(uint32_t *)(blob + 0x20) = 0x216;           /* local_18 */
    SendMessageW(wnd, 0x47, 0, (LPARAM)blob);
}

/* ========== FUN_1400f400c @0x1400f400c (记录文本色 RGBA, 直移) ========== */
void FUN_1400f400c(longlong self, int r, int g, int b, uint32_t a)
{
    if (-1 < r) *(int32_t *)((char *)self + 0x1f0) = r;
    if (-1 < g) *(int32_t *)((char *)self + 500) = g;
    if (-1 < b) *(int32_t *)((char *)self + 0x1f8) = b;
    *(uint32_t *)((char *)self + 0x1fc) = a;
}

/* ========== FUN_1400f4040 @0x1400f4040 (行高系数, 直移) ========== */
void FUN_1400f4040(longlong self, float factor)
{
    if (factor < 0.0f) {
        factor = (float)(int)factor;
    }
    *(double *)((char *)self + 0x200) = (double)factor;
}

/* ========== FUN_1400f4064 @0x1400f4064 (选中项 + 可选失效刷新, 直移) ========== */
void FUN_1400f4064(longlong self, int item, int mode)
{
    if (-2 < item) {
        *(int32_t *)((char *)self + 0x218) = item;
    }
    if (mode != 1) {
        HWND wnd = *(HWND *)((char *)self + 0x20);
        BOOL erase;
        if (mode == 2) {
            erase = 0;
        }
        else {
            FUN_1400f3674(wnd);
            if (-2 < item) {
                return;
            }
            wnd = *(HWND *)((char *)self + 0x20);
            erase = 1;
        }
        InvalidateRect(wnd, NULL, erase);
    }
}

/* ========== FUN_1400f40bc @0x1400f40bc (记录文本底色索引, 直移) ========== */
uint64_t FUN_1400f40bc(longlong self, uint32_t color)
{
    *(uint32_t *)((char *)self + 0x3b4) = color;
    return 1;
}

/* ========== FUN_1400f5608 @0x1400f5608 (列宽表中查列, 直移) ========== */
longlong FUN_1400f5608(longlong self, int col, int *pwidth)
{
    int n = *(int32_t *)((char *)self + 0x288);
    int i;
    for (i = 0; i < n; i++) {
        int *node = *(int **)(uintptr_t)FUN_140063b00(i, (longlong *)((char *)self + 0x278),
                                                       (longlong *)((char *)self + 0x280), 8);
        if ((node != NULL) && (*node == col)) {
            *pwidth = node[2];
            return i;
        }
    }
    return -1;
}

/* ========== FUN_1400f59c0 @0x1400f59c0 (子项宽表中查列, 直移) ========== */
longlong FUN_1400f59c0(longlong self, int col, int *pwidth)
{
    int n = *(int32_t *)((char *)self + 0x2d0);
    int i;
    for (i = 0; i < n; i++) {
        int *node = *(int **)(uintptr_t)FUN_140063b00(i, (longlong *)((char *)self + 0x2c0),
                                                       (longlong *)((char *)self + 0x2c8), 8);
        if ((node != NULL) && (*node == col)) {
            *pwidth = node[2];
            return i;
        }
    }
    return -1;
}

/* ========== FUN_1400f578c / FUN_1400f58d4 @0x1400f578c/@0x1400f58d4 (列/子项宽表追加, 直移) ========== */
static void FUN_1400f578c(longlong self, uint32_t col, uint32_t width)
{
    void *node = operator_new(0x18);
    *(uint32_t *)node = col;
    *(uint64_t *)((char *)node + 8) = width & 0xffffffffULL;
    FUN_1400639f0((longlong *)((char *)self + 0x278), (longlong *)((char *)self + 0x280),
                  (longlong *)((char *)self + 0x288), (unsigned char *)&node, 8, 1);
}

static void FUN_1400f58d4(longlong self, uint32_t col, uint32_t width)
{
    void *node = operator_new(0x18);
    *(uint32_t *)node = col;
    *(uint64_t *)((char *)node + 8) = width & 0xffffffffULL;
    FUN_1400639f0((longlong *)((char *)self + 0x2c0), (longlong *)((char *)self + 0x2c8),
                  (longlong *)((char *)self + 0x2d0), (unsigned char *)&node, 8, 1);
}

/* ========== FUN_1400f2b84 @0x1400f2b84 (宽度节点更新, 直移) ========== */
uint64_t FUN_1400f2b84(longlong *vec, longlong found, int width, uint32_t col, int extra)
{
    void **slot;
    void *node;

    if (found >= 0) {
        slot = (void **)(uintptr_t)FUN_140063b00(found, vec, vec + 1, 8);
        node = *slot;
        if (node != NULL) {
            if (width < 0) {
                *slot = 0;
                free(node);
                return 0;
            }
            *(longlong *)((char *)node + 8) = width;
            *(uint32_t *)node = col;
            if (-2 < extra) {
                *(int32_t *)((char *)node + 16) = extra;
            }
            node = *slot;
        }
        return (uint64_t)(uintptr_t)node & 0xffffffffffffff00ULL;
    }
    hit_out:
    return (uint64_t)(uintptr_t)0;
}

/* ========== FUN_1400f8f00 @0x1400f8f00 (设列宽/子项宽: 记录+广播, 直移) ========== */
void FUN_1400f8f00(longlong self, int col, uint32_t wmain, uint32_t wsub)
{
    char hit;
    longlong found;
    int scratch;

    if (-2 < (int)wsub) {
        scratch = 0;
        found = FUN_1400f5608(self, col, &scratch);
        if (found < 0) {
            hit = (char)(-1 < (int)wsub);
        }
        else {
            hit = (char)(FUN_1400f2b84((longlong *)((char *)self + 0x278),
                                       found, (int)wsub, (uint32_t)col, -3) & 0xff);
        }
        if (hit != '\0') {
            FUN_1400f578c(self, (uint32_t)col, wsub);
        }
    }
    if (-2 < (int)wmain) {
        scratch = 0;
        found = FUN_1400f59c0(self, col, &scratch);
        if (found < 0) {
            hit = (char)(-1 < (int)wmain);
        }
        else {
            hit = (char)(FUN_1400f2b84((longlong *)((char *)self + 0x2c0),
                                       found, (int)wmain, (uint32_t)col, -3) & 0xff);
        }
        if (hit != '\0') {
            FUN_1400f58d4(self, (uint32_t)col, wmain);
        }
    }
}

/* ========== PECMD_ListAddItemEntry @0x1400f53c8 (插列: LVM_INSERTCOLUMNW + 记录列标题, 忠实直移) ==========
 * 本函数为本地址唯一保留定义; core_b8l.c 重复副本(iSubItem 误写 +0x18)已按符号审计线索2删除。
 */
void PECMD_ListAddItemEntry(longlong self, int idx, LPCWSTR text, uint32_t fmt, int cx,
                            int subitem, int maskextra)
{
    WPARAM wp = (WPARAM)idx;
    void **arr;
    void *old;
    void *node;
    int len;
    WCHAR one[2];
    unsigned char lvc[0x30];
    uint32_t mask = 5;

    arr = (void **)(uintptr_t)FUN_140063b00(wp, (longlong *)((char *)self + 0x1a8),
                                            (longlong *)((char *)self + 0x1b0), 8);
    old = *arr;
    if (fmt == 0) {
        fmt = 0x30;
    }
    node = operator_new(8);
    if (node == NULL) {
        node = NULL;
    }
    else {
        PECMD_StrAssign((WCHAR **)node, text);
    }
    *arr = node;
    if (old != NULL) {
        PECMD_FreeStrBuf((WCHAR **)old);
        free(old);
    }
    len = lstrlenW(*(LPCWSTR *)((char *)self + 0x1d8));
    one[0] = (WCHAR)fmt;
    one[1] = 0;
    if (idx < len) {
        *(WCHAR *)(*(longlong *)((char *)self + 0x1d8) + (longlong)len * 2) = one[0];
    }
    else {
        PECMD_AppendWideStr((WCHAR **)((char *)self + 0x1d8), one);
    }
    *(longlong *)(uintptr_t)FUN_140063b00(wp, (longlong *)((char *)self + 0x1c0),
                                          (longlong *)((char *)self + 0x1c8), 8) = maskextra;
    memset(lvc, 0, 0x30);
    if (fmt >> 8) {
        fmt >>= 8;
    }
    *(uint32_t *)(lvc + 4) = fmt & 0xf;
    if (cx != -1) {
        mask |= 2;
        *(int32_t *)(lvc + 8) = cx;
    }
    if (subitem != -1) {
        mask |= 8;
        *(int32_t *)(lvc + 0x1c) = subitem;
    }
    *(uint32_t *)(lvc + 0) = mask;
    *(LPCWSTR *)(lvc + 0x10) = text;
    SendMessageW(*(HWND *)((char *)self + 0x20), 0x1061 /*LVM_INSERTCOLUMNW*/, wp, (LPARAM)lvc);
}

/* ========== FUN_1400f5558 @0x1400f5558 (由列描述块插列, 直移) ========== */
void FUN_1400f5558(longlong self, int idx, longlong coldsc, uint64_t fmtword, int align)
{
    PECMD_ListAddItemEntry(self, idx, *(LPCWSTR *)(coldsc + 0x10), *(uint32_t *)(coldsc + 4),
                           *(int32_t *)(coldsc + 8), -1, align);
    (void)fmtword;
}

/* ========== FUN_1400f2c44 @0x1400f2c44 (行数超出可视高度时刷新滚动条, 直移) ========== */
void FUN_1400f2c44(longlong self)
{
    LRESULT n;
    uint8_t st[0x18];
    RECT cli;
    RECT hdr;

    if ((*(uint8_t *)((char *)self + 0x3f8) & 0xc) == 4) {
        n = SendMessageW(*(HWND *)((char *)self + 0x20), 0x1004 /*GETITEMCOUNT*/, 0, 0);
        if (0 < (int)n) {
            SendMessageW(*(HWND *)((char *)self + 0x20), 0x1022, 0, (LPARAM)st);
            GetClientRect(*(HWND *)((char *)self + 0x20), &cli);
            GetWindowRect(*(HWND *)((char *)self + 0xf8), &hdr);
            if (cli.bottom < (hdr.bottom - hdr.top) + *(int32_t *)(st + 8)) {
                ShowScrollBar(*(HWND *)((char *)self + 0x20), 1, 0);
                ShowScrollBar(*(HWND *)(char *)(self + 0x20 / 8), 1, 1);
            }
        }
    }
}

/* ========== FUN_1400c99b4 @0x1400c99b4 (列规格 "w[/s][align]标题" 解析, 忠实直移) ========== */
int FUN_1400c99b4(longlong coldsc, longlong *pcursor, longlong *outtitle,
                  int *pwidth, char *pfmt, uint32_t *palign, short delim)
{
    char hadfmt;
    int w;
    uint32_t *fmtdw;
    uint32_t *align0;
    uint32_t scratch;
    uint64_t rv;
    LPCWSTR start;

    fmtdw = (uint32_t *)(void *)pfmt;
    hadfmt = *pfmt;
    *pfmt = '\0';
    *pwidth = (int)0x80000000;
    *(uint32_t *)((char *)coldsc + 4) = 0;
    w = (int)0x80000000;
    *fmtdw = 0x32;
    if (*(uint16_t *)(uintptr_t)*pcursor == '#') {
        *pcursor += 2;
        PECMD_ParseHexOrDecBool((WCHAR **)pcursor, &w);
        if (*(uint16_t *)(uintptr_t)*pcursor == '#') {
            *pcursor += 2;
            PECMD_ParseHexOrDecBool((WCHAR **)pcursor, pwidth);
        }
        if (*(uint16_t *)(uintptr_t)*pcursor == ':') {
            *pcursor += 2;
        }
    }
    for (;;) {
        uint16_t ch = *(uint16_t *)(uintptr_t)*pcursor;
        if (ch == '=') {
            *fmtdw = (*fmtdw << 8) | 0x32;
            *(uint32_t *)((char *)coldsc + 8) = *fmtdw;
        }
        else if (ch == '+') {
            *fmtdw = (*fmtdw << 8) | 0x31;
            *(uint32_t *)((char *)coldsc + 8) = *fmtdw;
        }
        else if (ch != '*') {
            if (*(int32_t *)((char *)coldsc + 4) == 0) {
                *(uint32_t *)((char *)coldsc + 4) = 0x30;
            }
            PECMD_ParseUIntValue((WCHAR **)pcursor, (int *)fmtdw);
            *palign = 0xffffffffu;
            align0 = palign;
            while (*(uint16_t *)(uintptr_t)*pcursor == '/') {
                *pcursor += 2;
                scratch = 0;
                rv = FUN_1400c11f4(pcursor, (uint64_t *)&scratch);
                if (0 < (int)rv) {
                    *align0 = (*align0 << 0x10) | (uint32_t)(uint16_t)scratch;
                }
            }
            if ((short)*align0 < 0) {
                *(uint16_t *)align0 = 2;
            }
            if (*(uint16_t *)(uintptr_t)*pcursor == ':') {
                *pcursor += 2;
            }
            *(int32_t *)((char *)coldsc + 8) = (int32_t)*fmtdw;
            if (hadfmt == '\0') {
                start = (LPCWSTR)(uintptr_t)*pcursor;
                b7c_skip_to((WCHAR **)pcursor, (WCHAR)delim);
                FUN_140063888(outtitle, start, (*pcursor - (longlong)(uintptr_t)start) >> 1);
                if (*(uint16_t *)(uintptr_t)*pcursor != 0) {
                    *pcursor += 2;
                }
            }
            else {
                FUN_1400703e4(outtitle, (LPCWSTR)(uintptr_t)*pcursor);
            }
            *(longlong *)((char *)coldsc + 0x10) = *outtitle;
            return w;
        }
        else {
            *(uint32_t *)((char *)coldsc + 4) =
                (*(uint32_t *)((char *)coldsc + 4) << 8) | 0x30;
            *pfmt = '\x10';
        }
        *pcursor += 2;
    }
}

/* 虚表 0x108 槽方法占位: 应用字体到列表控件 (语义保守 no-op)。TODO(verify) */
static void b7c_listctrl_applyfont(void *self, HFONT font, int flag)
{
    (void)self; (void)font; (void)flag;
}

/* ========== PECMD_TablCreateListCtrl @0x1400c9b9c ==========
 * 'TABL' 列表控件创建 (decompiled.c @124641 size=4603 忠实直移)。
 * 流程: 宿主基类构造(FUN_140053dc8) → 挂虚表 PTR_FUN_140126888 → 加锁 DAT_14013e190 →
 *   标志位集展开窗口样式 → operator_new(0x400)+FUN_1400f9134 构造列表对象 →
 *   "X,Y,W" 坐标串(FUN_14007de4c×3, '-'/''*' 前缀取负) → 分隔符指针参数 →
 *   FUN_1400f345c 创建 SysListView32 → 表头子对象(FUN_1400f2b44,
 *   其构造经 FUN_1400f5c74 安装虚表 14012c550) → "=名称[:]" 列名段复制 →
 *   字体链解析(主/表头双字体 FUN_1400b1f34/FUN_1400b89dc) → 底色/文本色/
 *   扩展样式(LVM 0x1001/0x1037/0x1036/0x1026) → '#'/GetSysColor 底色回退 →
 *   列规格循环(FUN_1400c99b4 + FUN_1400f8f00 设宽 + FUN_1400f5558 插列) →
 *   行文本循环: \r\n 或自定义分隔符分行、列分隔符分字段, 首列 LVM_INSERTITEMW
 *   (FUN_1400f3554)、后续列 LVM_SETITEMTEXTW(0x1074), 超出标题列数自动补空列 →
 *   收尾: LVM_SCROLL(0x100f)、可见性环境变量(FUN_140053cec)、滚动条修正(FUN_1400f2c44)。
 * 返回: param_1。
 * TODO(verify): 叶子数字解析器 FUN_140067b54/FUN_1400bf358 目前经 link_stubs 桩
 *   (返回失败/NULL), 含括号或小数的坐标/字体字段运行期退化为缺省值;
 *   虚表 0x108 方法为保守 no-op; operator_new 为 link_stubs NULL 桩。
 */
uint64_t *PECMD_TablCreateListCtrl(uint64_t *a1, uint64_t a2, uint64_t a3,
                                 uint32_t a4, uint64_t *a5, uint32_t a6,
                                 uint32_t a7, uint32_t a8, uint32_t a9,
                                 uint64_t *a10, uint16_t *a11, LPCWSTR a12,
                                 int64_t a13, int16_t *a14, LPCWSTR a15,
                                 uint64_t *a16, uint64_t a17, uint64_t a18)
{
    /* ---- 形参语义还原 ---- */
    ulonglong F = (ulonglong)a14;                 /* param_14: 标志位集 (指针位型复用) */
    short *pd = (short *)(uintptr_t)a17;          /* param_17: [0]=行分隔符 [1..3]=列分隔符等 */
    short *spec = (short *)(uintptr_t)a18;        /* param_18: "X,Y,W[,…]" 坐标规格串 */

    /* ---- 工作变量 (沿用 decompiled 命名) ---- */
    int iVar9;
    longlong *plVar21;
    WCHAR WVar20;
    WCHAR WVar36;
    short local_248;
    short col_delim;
    longlong *plVar13;
    uint32_t uVar22;
    uint32_t uVar24;
    uint32_t uVar8;
    uint32_t uVar25;
    uint32_t uVar27;
    uint32_t uVar4flag;
    int iVar26;
    ulonglong uVar32;
    longlong local_228;
    uint64_t uStack_220;
    uint32_t local_1f8;
    ulonglong local_200;
    double dVar5;
    uint64_t *puVar12;
    bool bVar3;
    bool bVar38;
    char cVar37;
    WCHAR *pWVar30;
    LPCWSTR pWVar28;
    LPCWSTR local_258;
    longlong lVar31;
    HFONT pHVar15;
    HFONT pHVar16;
    HDC hdc;
    HWND hWnd;
    COLORREF CVar10;
    LRESULT LVar14;
    uint32_t uVar39;
    uint32_t uVar40;
    uint32_t uVar41;
    uint32_t uVar42;
    DWORD DVar11;
    int param_18_int;
    double param_16;
    double local_1d0;
    uint64_t local_208;
    uint64_t local_218;
    uint64_t local_1d8;
    uint64_t local_210;
    uint64_t seed238;
    longlong cursor12;
    longlong cursor258;
    int local_128;
    int local_124;
    int local_c8;
    int local_c4[35];
    uint8_t lvi_ins[0x54];                        /* LVITEM #1 (插入): 基址 = 原 &local_188 */
    uint8_t lvi_set[0x20];                        /* LVITEM #2 (更新): 基址 = 原 &local_128 */
    uint16_t *local_240;
    uint16_t *local_170;
    uint16_t *puVar17;
    uint16_t *puVar18;
    uint16_t *puVar33;
    uint16_t *local_250;
    uint16_t uVar1;
    uint16_t uVar2;
    uint64_t uVar19;
    longlong lVar31b;
    longlong *plVar34;
    longlong *plVar35;
    longlong *plVar29;
    int local_1c0;
    int local_1ec_i;
    int local_1e8_i;
    DWORD local_1e4;
    uint32_t local_1f0;
    int colcnt;
    int hdr_colbase;
    void *titleslot;
    char hdr_done;
    int64_t colname_slot;
    struct {
        uint32_t f00;                             /* local_1c8 初值 7 */
        uint32_t mask;                            /* local_1c4 = 0x30 */
        int32_t fmt;                              /* local_1c0 位型 */
        uint32_t pad;
        LPCWSTR title;                            /* +0x10 local_1b8 */
    } colctx;
    int rect4[4];
    int32_t trio[4];

    plVar21 = NULL;
    WVar20 = L'\0';
    WVar36 = L'\0';
    local_248 = 0;
    col_delim = 9;
    uVar27 = 0;
    param_18_int = -1;

    FUN_140053dc8(a1, 0x10, a2, (LPCWSTR)*a5, (LPCWSTR)g_szEmpty,
                  (LPCWSTR)g_szEmpty, a6, a7, a8, a9);
    *a1 = (uint64_t)(uintptr_t)&b7c_vt_PTR_140126888;
    EnterCriticalSection((CRITICAL_SECTION *)g_csInit);       /* DAT_14013e190 */

    iVar9 = *(int32_t *)((char *)a3 + 0xd0);
    if (pd != NULL) {
        local_248 = *pd;
        col_delim = pd[1];
    }
    local_228 = (longlong)a1[8];
    uVar22 = (uint32_t)F;
    uStack_220 = ((uint64_t)(uint32_t)(
                      *(int32_t *)((char *)a1 + 0x44) + *(int32_t *)((char *)a1 + 0x4c))
                      << 32)
               | (uint32_t)((int32_t)a1[8] + *(int32_t *)((char *)a1 + 0x48));
    local_1f8 = uVar22 & 0x800;
    local_200 = F >> 32;
    *(uint32_t *)((char *)a1 + 0x74) = uVar22 & 0xfffff7ffu;
    uVar4flag = (uint32_t)(F & 0x10);             /* uVar4 */
    uVar24 = 0x10000000u;
    if (((F >> 15) & 1) != 0) {
        uVar24 = 0x18000000u;
    }
    if ((F & 0x40) != 0) {
        uVar24 |= 0x800000u;
    }
    uVar8 = 0;
    if ((F & 0xffffffff80000000ULL) != 0) {
        uVar8 = 0x800;
    }
    if (((F >> 19) & 1) == 0) {
        uVar24 |= 8;
    }
    uVar25 = uVar24 | 1;
    if (((F >> 13) & 1) != 0) {
        uVar25 = uVar24 | 0x4001;
    }
    if (((F >> 12) & 1) != 0) {
        uVar25 |= 0x200;
    }
    if ((F & 0x2090000ULL) != 0) {
        uVar25 |= 4;
    }
    if (uVar8 != 0) {
        uVar25 |= uVar8;
    }

    puVar12 = (uint64_t *)operator_new(0x400);
    plVar13 = plVar21;
    if (puVar12 != NULL) {
        plVar13 = (longlong *)FUN_1400f9134(puVar12, (uint64_t)(uintptr_t)(a1 + 0xb),
                                (uint64_t)(uintptr_t)a1);
    }
    dVar5 = g_fontMinus0;                         /* DAT_140125238 = -0.0 */
    a1[7] = (uint64_t)(uintptr_t)plVar13;
    *a16 = (uint64_t)(uintptr_t)a1;
    bVar38 = false;
    if (spec != NULL) {
        if ((*spec == 0x2a) || (*spec == 0x2d)) {
            spec++;
            bVar3 = true;
        }
        else {
            bVar3 = false;
        }
        cursor12 = (longlong)(uintptr_t)spec;
        FUN_14007de4c(&cursor12, (double *)&plVar13[0x44]);
        spec = (short *)(uintptr_t)cursor12;
        if (bVar3) {
            plVar13[0x44] = (uint64_t)(dVar5 - (double)plVar13[0x44]);
        }
        if ((*spec == 0x2a) || (*spec == 0x2d)) {
            spec++;
            bVar3 = true;
        }
        else {
            bVar3 = false;
        }
        cursor12 = (longlong)(uintptr_t)spec;
        FUN_14007de4c(&cursor12, (double *)&plVar13[0x45]);
        spec = (short *)(uintptr_t)cursor12;
        if (bVar3) {
            plVar13[0x45] = (uint64_t)(dVar5 - (double)plVar13[0x45]);
        }
        if ((*spec == 0x2a) || (*spec == 0x2d)) {
            spec++;
            bVar38 = true;
        }
        cursor12 = (longlong)(uintptr_t)spec;
        FUN_14007de4c(&cursor12, (double *)&plVar13[0x46]);
        spec = (short *)(uintptr_t)cursor12;
        if (bVar38) {
            plVar13[0x46] = (uint64_t)(dVar5 - (double)plVar13[0x46]);
        }
    }

    if (pd != NULL) {
        if (pd[2] != 0) {
            *(uint16_t *)((char *)plVar13 + 0x23a) = (uint16_t)*pd;
        }
        if (pd[3] != 0) {
            *(uint16_t *)((char *)plVar13 + 0x47 * 8) = (uint16_t)pd[1];
        }
    }

    rect4[0] = (int32_t)local_228;                /* X = (int)a1[8] */
    rect4[1] = (int32_t)((uint64_t)local_228 >> 32);
    rect4[2] = (int32_t)uStack_220;
    rect4[3] = (int32_t)(uStack_220 >> 32);
    FUN_1400f345c((longlong *)plVar13,
                  uVar25 | (uint32_t)(~((ulonglong)iVar9 << 2) & 0x10000ULL) | 0x40000000u,
                  rect4, *(HWND *)((char *)a3 + 0x20), a4);
    LVar14 = SendMessageW((HWND)(uintptr_t)plVar13[4], 0x101f, 0, 0);
    FUN_1400f2b44((longlong *)(plVar13 + 0x1b), LVar14);
    if ((uVar22 & 0x4800) == 0x4000) {
        EnableWindow((HWND)(uintptr_t)plVar13[0x1f], 0);
        *(uint16_t *)((char *)plVar13 + 0x7f * 8) |= 0x100;
    }
    if (((F >> 14) & 1) != 0) {
        *(uint16_t *)((char *)plVar13 + 0x7f * 8) |= 1;
    }

    /* ---- 列名段 "=名称[:]" ---- */
    PECMD_AllocStrSlot((WCHAR **)&colname_slot);  /* FUN_140063620(&local_228) */
    cursor12 = (longlong)(uintptr_t)a12;          /* param_12 游标初值 */
    pWVar28 = a12;
    if (*a12 == L'=') {
        pWVar30 = (WCHAR *)a12 + 1;
        cursor12 = (longlong)(uintptr_t)pWVar30;
        b7c_skip_to((WCHAR **)&cursor12, L':');   /* thunk_FUN_1400f429c */
        pWVar28 = (LPCWSTR)(uintptr_t)cursor12;
        FUN_140063888(&colname_slot, pWVar30,
                      (cursor12 - (longlong)(uintptr_t)pWVar30) >> 1);
        if (*pWVar28 != L'\0') {
            pWVar28++;
            cursor12 = (longlong)(uintptr_t)pWVar28;
        }
    }

    /* ---- 字体链解析 ---- */
    pHVar15 = (HFONT)(uintptr_t)FUN_1400e5890((longlong)a3);
    puVar12 = &g_u6435a0;
    WVar36 = *pWVar28;
    uVar41 = (uint32_t)g_u6435a0;
    uVar42 = (uint32_t)(g_u6435a0 >> 32);
    seed238 = g_u6435a0;                          /* local_238[0]: 主字体高度位型 */
    if ((WVar36 != L'\0') && (pWVar28[1] == L':')) {
        WVar36 = WVar20;                          /* WVar20 此时为 0: 清「有面名」旗标 */
    }
    if (*pWVar28 == L'*') {
        *(WCHAR *)pWVar28 = L'\0';
    }
    FUN_140067d54(&cursor12, (double *)&seed238);
    pWVar28 = (LPCWSTR)(uintptr_t)cursor12;
    lVar31 = 2;
    if (*pWVar28 == L':') {
        pWVar28++;
        cursor12 = (longlong)(uintptr_t)pWVar28;
    }
    local_258 = pWVar28;
    cursor258 = (longlong)(uintptr_t)local_258;
    b7c_skip_to((WCHAR **)&cursor258, L':');
    local_258 = (LPCWSTR)(uintptr_t)cursor258;
    if (*local_258 != L'\0') {
        *(WCHAR *)local_258 = L'\0';
        cursor258 += lVar31;
        local_258 = (LPCWSTR)(uintptr_t)cursor258;
    }
    local_208 = (uint64_t)(uintptr_t)puVar12;
    local_218 = (uint64_t)(uintptr_t)puVar12;
    cursor12 = (cursor12 & 0xffffffff00000000ULL) | 0x80000000ULL;
    param_16 = 0.0;
    local_1d0 = 0.0;
    local_1d8 = (uint64_t)(uintptr_t)puVar12;
    local_210 = (uint64_t)(uintptr_t)puVar12;
    if ((*local_258 == L'-') || (*local_258 == L'*')) {
        WVar20 = *local_258;
        cursor258 += lVar31;
        local_258 = (LPCWSTR)(uintptr_t)cursor258;
    }
    FUN_140067d54(&cursor258, &param_16);
    if (WVar20 == L'-') {
        puVar12 = (uint64_t *)(uint64_t)(dVar5 - param_16);
    }
    else if (WVar20 == L'*') {
        puVar12 = (uint64_t *)(uint64_t)(param_16 + g_scrollLimit);
    }
    else {
        puVar12 = (uint64_t *)(uintptr_t)param_16;
    }
    if (*local_258 != L'\0') {
        cursor258 += lVar31;
        local_258 = (LPCWSTR)(uintptr_t)cursor258;
        cursor12 = 0xffffffff80000000ULL |
                   (cursor12 & 0xffffffff00000000ULL);
        iVar9 = FUN_140066850(&cursor258, (uint64_t *)&cursor12);
        uVar39 = 0x80000000u;
        if (iVar9 != 0) {
            uVar39 = (uint32_t)cursor12;
        }
        cursor12 = (cursor12 & 0xffffffff00000000ULL) | uVar39;
        if (*local_258 != L'\0') {
            cursor258 += lVar31;
            local_258 = (LPCWSTR)(uintptr_t)cursor258;
            FUN_140067d54(&cursor258, &local_1d0);
            if (*local_258 != L'\0') {
                cursor258 += lVar31;
                local_258 = (LPCWSTR)(uintptr_t)cursor258;
                FUN_140067d54(&cursor258, (double *)&local_208);
                if (*local_258 != L'\0') {
                    cursor258 += lVar31;
                    local_258 = (LPCWSTR)(uintptr_t)cursor258;
                    FUN_140067d54(&cursor258, (double *)&local_218);
                    if (*local_258 != L'\0') {
                        cursor258 += lVar31;
                        local_258 = (LPCWSTR)(uintptr_t)cursor258;
                        FUN_140067d54(&cursor258, (double *)&local_1d8);
                        if (*local_258 != L'\0') {
                            cursor258 += lVar31;
                            local_258 = (LPCWSTR)(uintptr_t)cursor258;
                            FUN_140067d54(&cursor258, (double *)&local_210);
                            if (*local_258 != L'\0') {
                                cursor258 += lVar31;
                                local_258 = (LPCWSTR)(uintptr_t)cursor258;
                                FUN_140067e2c(&cursor258, &param_18_int);
                            }
                            uVar41 = (uint32_t)local_210;
                            uVar42 = (uint32_t)(local_210 >> 32);
                        }
                    }
                }
            }
        }
    }

    /* ---- 底色采样 + 主字体重建 ---- */
    hWnd = *(HWND *)((char *)a3 + 0x20);
    hdc = GetDC(hWnd);
    CVar10 = GetBkColor(hdc);
    SendMessageW((HWND)(uintptr_t)plVar13[4], 0x1001 /*SETBKCOLOR*/, 0, CVar10);
    if (hdc != NULL) {
        ReleaseDC(hWnd, hdc);
    }
    plVar13[0x48] = (uint64_t)(uintptr_t)pHVar15;
    if ((0.0 < *(double *)&seed238) || (*pWVar28 != L'\0') || (WVar36 != L'\0')) {
        param_16 = *(double *)&seed238;
        local_128 = 0;
        memset(&local_124, 0, 0x58);
        GetObjectW(pHVar15, 0x5c, &local_128);
        if (0.0 < *(double *)&local_1d8) {
            local_128 = FUN_1400628b4(*(double *)&local_1d8);
        }
        {
            double dseed = *(double *)(((uint64_t)uVar42 << 32) | uVar41);
            if ((0.0 <= dseed) && (dseed != 0.0)) {
                local_124 = FUN_1400628b4(dseed);
            }
        }
        pHVar16 = FUN_1400b1f34(&local_128, &param_16, pWVar28);
        plVar13[0xd] = (uint64_t)(uintptr_t)pHVar16;
        if (pHVar16 != NULL) {
            pHVar15 = pHVar16;
        }
        plVar13[0x48] = (uint64_t)(uintptr_t)pHVar15;
        pHVar16 = FUN_1400b89dc(pHVar15, (double *)&seed238, pWVar28);
        plVar13[0x49] = (uint64_t)(uintptr_t)pHVar16;
        plVar13[0x48] = (uint64_t)(uintptr_t)pHVar16;
        uVar39 = (uint32_t)seed238;
        uVar40 = (uint32_t)(seed238 >> 32);
    }
    else {
        uVar39 = (uint32_t)seed238;
        uVar40 = (uint32_t)(seed238 >> 32);
    }
    {
        double d7b = *(double *)(((uint64_t)uVar40 << 32) | uVar39);
        if ((0.0 <= d7b) && (d7b != 0.0)) {
            plVar13[0x7b] = ((uint64_t)uVar40 << 32) | uVar39;
        }
        dVar5 = *(double *)(((uint64_t)uVar42 << 32) | uVar41);
        if ((0.0 <= dVar5) && (dVar5 != 0.0)) {
            plVar13[0x7c] = ((uint64_t)uVar42 << 32) | uVar41;
        }
    }

    DragAcceptFiles((HWND)(uintptr_t)plVar13[4], 1);
    if (*(double *)&puVar12 != 0.0) {
        FUN_1400f4040((longlong)plVar13, (float)*(double *)&puVar12);
    }
    if (plVar13[0xd] != 0) {
        ((b7c_vt_fn_t)(uintptr_t)((const void **)*(const void **)plVar13)[0x108 / 8])
            (plVar13, pHVar15, 1);                /* 虚方法 0x108: 应用字体 */
    }

    /* ---- 文本色 RGBA ---- */
    {
        uint32_t color = (uint32_t)cursor12;
        uint32_t rr = 0xffffffffu;
        uint32_t gg = 0xffffffffu;
        uint32_t bb = 0xffffffffu;
        if (-1 < (int)color) {
            bb = (color >> 16) & 0xff;
            gg = (color >> 8) & 0xff;
            rr = color & 0xff;
        }
        FUN_1400f400c((longlong)plVar13, (int)rr, (int)gg, (int)bb,
                      (uint32_t)*(double *)&local_1d0);
    }

    /* ---- 表头字体 ---- */
    {
        double seed = (double)g_fontSizeDef;      /* DAT_1401293c0 */
        local_c8 = 0;
        memset(local_c4, 0, 0x58);
        GetObjectW(pHVar15, 0x5c, &local_c8);
        if (0.0 < *(double *)&local_208) {
            local_c8 = FUN_1400628b4(*(double *)&local_208);
        }
        if (0.0 < *(double *)&local_218) {
            local_c4[0] = FUN_1400628b4(*(double *)&local_218);
        }
        plVar13[0x3c] = (uint64_t)(uintptr_t)pHVar15;
        if ((0.0 < *(double *)&local_208) || (0.0 < *(double *)&local_218) ||
            (*a15 != L'\0')) {
            pHVar15 = FUN_1400b1f34(&local_c8, &seed, a15);
            plVar13[0x3d] = (uint64_t)(uintptr_t)pHVar15;
            plVar13[0x3c] = (uint64_t)(uintptr_t)pHVar15;
        }
    }

    /* ---- 扩展样式合成 ---- */
    if ((F & 0x80) != 0) {
        *(uint16_t *)((char *)plVar13 + 0x7f * 8) |= 4;
    }
    if (((ulonglong)(F & 0xfffffffffffff7ffULL) >> 8 & 1) != 0) {
        *(uint16_t *)((char *)plVar13 + 0x7f * 8) |= 8;
    }
    bVar38 = ((F & 0x3800200ULL) == 0);
    uVar24 = (uint32_t)(bVar38 ? 1 : 0);
    if (((ulonglong)(F & 0xfffffffffffff7ffULL) >> 10 & 1) != 0) {
        uVar24 = (uint32_t)(bVar38 ? 1 : 0) | 4;
    }
    if (((F >> 14) & 1) != 0) {
        *(uint16_t *)((char *)plVar13 + 0x7f * 8) |= 1;
    }
    if (local_1f8 != 0) {
        uVar24 |= 0x10;
    }
    if (((F >> 17) & 1) != 0) {
        uVar24 |= 0x8000;
    }
    LVar14 = SendMessageW((HWND)(uintptr_t)plVar13[4], 0x1037 /*GETEXSTYLE*/, 0, 0);
    uVar24 |= (uint32_t)LVar14;
    if ((F & 0x600000ULL) != 0) {
        uVar24 |= 4;
    }
    FUN_1400f353c((longlong)plVar13, uVar24);
    if (((F >> 16) & 1) != 0) {
        /* psVar23 ← F & ~0x80000_0x800 (原体二次掩码) */
    }
    if (((ulonglong)(F & 0xfffffffffff7f7ffULL) >> 18 & 1) != 0)
        *(uint16_t *)((char *)plVar13 + 0x7f * 8) |= 2;
    if (((ulonglong)(F & 0xfffffffffff7f7ffULL) >> 19 & 1) != 0)
        *(uint16_t *)((char *)plVar13 + 0x7f * 8) |= 0x20;
    if (((ulonglong)(F & 0xfffffffffff7f7ffULL) >> 20 & 1) != 0)
        *(uint16_t *)((char *)plVar13 + 0x7f * 8) |= 0x40;
    if (((ulonglong)(F & 0xfffffffffff7f7ffULL) >> 21 & 1) != 0)
        *(uint16_t *)((char *)plVar13 + 0x7f * 8) |= 0x400;
    if (((ulonglong)(F & 0xfffffffffff7f7ffULL) >> 22 & 1) != 0)
        *(uint16_t *)((char *)plVar13 + 0x7f * 8) |= 0xc00;
    if (((ulonglong)(F & 0xfffffffffff7f7ffULL) >> 23 & 1) != 0)
        *(uint16_t *)((char *)plVar13 + 0x7f * 8) |= 0x1800;
    if (((ulonglong)(F & 0xfffffffffff7f7ffULL) >> 24 & 1) != 0)
        *(uint16_t *)((char *)plVar13 + 0x7f * 8) |= 0x2800;
    uVar32 = local_200 & 1;
    local_200 &= 0xffffffffffffff01ULL;
    if (uVar32 != 0) {
        *(uint16_t *)((char *)plVar13 + 0x7f * 8) |= 0x200;
    }

    /* ---- 底色四连 "-1,bk,tbk,bk2" + '#sys' 回退 ---- */
    {
        longlong ccur = (longlong)(uintptr_t)(LPCWSTR)(uintptr_t)a13;
        local_1f0 = 0xffffffffu;
        local_1ec_i = -1;
        local_1e8_i = -1;
        local_1e4 = 0x80000000u;
        trio[0] = -1; trio[1] = -1; trio[2] = -1; trio[3] = (int32_t)0x80000000;
        FUN_1400677b0(&ccur, (longlong)(uintptr_t)trio);
        local_1f0 = (uint32_t)trio[0];
        local_1ec_i = trio[1];
        local_1e8_i = trio[2];
        DVar11 = local_1e4;
        if (trio[3] != (int32_t)0x80000000) {
            local_1e4 = (uint32_t)trio[3];
            DVar11 = local_1e4;
        }
        else {
            DVar11 = local_1e4;
        }
        if (-1 < (int)local_1f0) {
            SendMessageW((HWND)(uintptr_t)plVar13[4], 0x1001, 0, local_1f0);
        }
        if (-1 < local_1ec_i) {
            *(uint32_t *)((char *)plVar13 + 0x3c4) = (uint32_t)local_1ec_i;
            SendMessageW((HWND)(uintptr_t)plVar13[4], 0x1026 /*SETTEXTBKCOLOR*/, 0,
                         (uint64_t)(uint32_t)local_1ec_i);
        }
        if (-1 < local_1e8_i) {
            *(int32_t *)((char *)plVar13 + 0x78 * 8) = local_1e8_i;
            FUN_1400f40bc((longlong)plVar13, (uint32_t)local_1e8_i);
        }
        iVar9 = 0xd;
        DVar11 = local_1e4;
        if (local_1e4 == 0x80000000u) {
            if ((F & 0x50000ULL) == 0) {          /* psVar23 掩码后低 32 位同值 */
                WVar20 = L'#';
                b7c_skip_to((WCHAR **)&ccur, L'#');
                if (WVar20 != *(WCHAR *)(uintptr_t)ccur) goto LAB_1400ca7fa;
                ccur += 2;
                b7c_skip_to((WCHAR **)&ccur, L'#');
                if (WVar20 != *(WCHAR *)(uintptr_t)ccur) goto LAB_1400ca7fa;
                ccur += 2;
                b7c_skip_to((WCHAR **)&ccur, L'#');
                if ((WVar20 != *(WCHAR *)(uintptr_t)ccur) ||
                    ((WCHAR)(WVar20 + L'\a') != ((WCHAR *)(uintptr_t)ccur)[1])) {
                    goto LAB_1400ca7fa;
                }
            }
            DVar11 = GetSysColor(iVar9);
            if (DVar11 == 0x80000000u) {
                goto LAB_1400ca7fa;
            }
        }
        *(uint32_t *)((char *)plVar13 + 0x79 * 8) = DVar11;
LAB_1400ca7fa:
        if (((int32_t)plVar13[0x79] >= 0) &&
            (((F & 0xfffffffffff7f7ffULL) >> 16 & 1) != 0)) {
            *(uint16_t *)((char *)plVar13 + 0x7f * 8) |= 0x10;
        }
        if ((((int32_t)plVar13[0x79] >= 0) &&
             ((*(uint8_t *)((char *)plVar13 + 0x7f * 8) & 0x10) != 0)) ||
            ((*(uint8_t *)((char *)plVar13 + 0x7f * 8) & 0x20) != 0)) {
            *(uint16_t *)((char *)plVar13 + 0x7f * 8) |= 0x80;
        }
    }

    /* ---- 列标题解析循环 ---- */
    memset(&colctx, 0, sizeof(colctx));           /* FUN_140102a90(&local_1c4,0,0x34) */
    colctx.f00 = 7;                               /* local_1c8 */
    titleslot = NULL;
    local_250 = (uint16_t *)(uintptr_t)*a10;
    hdr_colbase = *(int32_t *)((char *)a3 + 0x17c);
    local_1c0 = 0;
    uVar1 = *local_250;
    uVar2 = *local_250;
    cVar37 = (char)(uVar1 == 0);
    hdr_done = cVar37;
    plVar34 = plVar21;
    plVar35 = plVar21;
    plVar29 = plVar21;
    while (uVar2 != 0) {
        uint32_t col_w;
        uint32_t col_align;
        int32_t col_wsub;
        char fmtbyte;
        col_wsub = (int)0x80000000;
        col_align = 2;
        titleslot = NULL;
        fmtbyte = 0;
        colctx.mask = 0x30;
        uVar24 = (uint32_t)FUN_1400c99b4((longlong)&colctx, (longlong *)&local_250,
                                         (longlong *)&titleslot, &col_wsub, &fmtbyte,
                                         &col_align, col_delim);
        col_w = (uint32_t)col_wsub;
        iVar26 = (int)(uintptr_t)plVar29;
        if (((int)uVar24 >= 0) || (col_wsub >= 0)) {
            FUN_1400f8f00((longlong)plVar13, iVar26, col_w, (uint32_t)col_wsub);
        }
        plVar34 = (longlong *)((longlong)plVar34 + local_1c0);
        if (0 < hdr_colbase) {
            local_1c0 = (int)((ulonglong)((longlong)hdr_colbase * (longlong)plVar34) / 0x60) -
                        (int)(uintptr_t)plVar35;
        }
        FUN_1400f5558((longlong)plVar13, iVar26, (longlong)&colctx, 0, col_align);
        plVar35 = (longlong *)((longlong)plVar35 + local_1c0);
        colctx.f00 = 7;
        PECMD_FreeStrBuf((WCHAR **)&titleslot);
        uVar27 = (uint32_t)iVar26 + 1;
        plVar29 = (longlong *)(uintptr_t)(uint64_t)uVar27;
        uVar2 = *local_250;
    }
    *(uint32_t *)((char *)a1 + 0x70) = uVar27;    /* (a1+0xe) 列数 */
    if (0 < param_18_int) {
        FUN_1400f4064((longlong)plVar13, param_18_int, 0);
    }

    /* ---- 行文本装入 (LVITEM 双结构: 插入块 lvi_ins / 更新块 lvi_set) ---- */
    memset(lvi_ins, 0, 0x54);
    *(uint32_t *)(lvi_ins + 0) = 1;               /* local_188 = LVIF_TEXT */
    *(int32_t *)(lvi_ins + 4) = (int32_t)0x7fffffff;  /* local_184 = iItem */
    *(int32_t *)(lvi_ins + 8) = 0;                /* local_180 = iSubItem */
    local_240 = a11;
    local_170 = (uint16_t *)g_szEmpty;
    *(WCHAR **)(lvi_ins + 0x18) = (WCHAR *)local_170;  /* local_170 = pszText */
    if (a11 == (uint16_t *)1) {
        local_240 = (uint16_t *)DAT_14011d308;
        FUN_1400f3554((longlong)plVar13, (LPARAM)lvi_ins);
        SendMessageW((HWND)(uintptr_t)plVar13[4], 0x1008, 0, 0);
    }
    uVar32 = local_200;
    uVar1 = *local_240;
    puVar33 = local_240;
    for (;;) {
        if (uVar1 == 0) {
            /* ---- 收尾 ---- */
            FUN_140053e78();
            SendMessageW((HWND)(uintptr_t)plVar13[4], 0x100f, 1, 0x320032);
            bVar38 = (uVar4flag == 0);
            if (!bVar38) {
                ShowWindow((HWND)(uintptr_t)plVar13[4], 0);
            }
            FUN_140053cec((longlong)a1, (int)(bVar38 ? 1 : 0));
            FUN_1400f2c44((longlong)plVar13);
            PECMD_FreeStrBuf((WCHAR **)&colname_slot);
            LeaveCriticalSection((CRITICAL_SECTION *)g_csInit);
            return a1;
        }
        local_250 = puVar33;
        if (local_248 == 0) {
            uVar1 = *puVar33;
            puVar17 = puVar33;
            while ((uVar1 != 0) && (uVar1 != 0xd) && (uVar1 != 0xa)) {
                puVar17++;
                local_240 = puVar17;
                uVar1 = *puVar17;
            }
            puVar18 = puVar17;
            if (*puVar17 == 0xd) {
                puVar18 = puVar17 + 1;
                local_240 = puVar18;
            }
            uVar1 = *puVar18;
        }
        else {
            b7c_skip_to((WCHAR **)&local_240, (WCHAR)local_248);
            uVar1 = *local_240;
            puVar17 = local_240;
            puVar18 = local_240;
        }
        if (uVar1 != 0) {
            puVar18 = puVar18 + 1;
            local_240 = puVar18;
        }
        if (*puVar17 != 0) {
            *puVar17 = 0;
            puVar18 = local_240;
            puVar33 = local_250;
        }
        uVar1 = *puVar33;
        plVar34 = plVar21;
        plVar35 = plVar21;
        puVar17 = puVar33;
        while ((puVar33 = puVar18, uVar1 != 0)) {
            lVar31b = 2;
            iVar9 = (int)(uintptr_t)plVar34;
            if ((hdr_done == '\0') &&
                (puVar33 = local_240,
                 *(uint32_t *)((char *)a1 + 0x70) <= (uint32_t)iVar9)) {
                break;
            }
            puVar33 = puVar17;
            if ((char)uVar32 == '\0') {
                b7c_skip_to((WCHAR **)&local_250, (WCHAR)col_delim);
                puVar33 = local_250;
            LAB_1400cabb9:
                if (*puVar33 != 0) {
                    *puVar33 = 0;
                    puVar33 = (uint16_t *)((longlong)local_250 + lVar31b);
                    local_250 = puVar33;
                }
            }
            else {
                while (uVar1 != 0) {
                    if (((8 < uVar1) && (uVar1 < 0xe)) || (uVar1 == 0x20)) {
                        goto LAB_1400cabb9;
                    }
                    local_250 = puVar33 + 1;
                    puVar33 = local_250;
                    uVar1 = *local_250;
                }
            }
            if (((char)uVar32 != '\0') && (puVar33 != NULL)) {
                while ((*puVar33 == 9) || (*puVar33 == 0xb) || (*puVar33 == 0xc) ||
                       (*puVar33 == 0x20)) {
                    puVar33 = (uint16_t *)((longlong)puVar33 + lVar31b);
                    local_250 = puVar33;
                }
            }
            colcnt = *(int32_t *)((char *)a1 + 0x70);
            *(int32_t *)(lvi_ins + 8) = iVar9;    /* LVITEM.iSubItem */
            if (colcnt <= iVar9) {
                *(int32_t *)((char *)a1 + 0x70) = colcnt + 1;
                FUN_1400f5558((longlong)plVar13, iVar9, (longlong)&colctx, 0,
                              (int)lVar31b);
                puVar33 = local_250;
            }
            local_170 = puVar17;
            *(WCHAR **)(lvi_ins + 0x18) = (WCHAR *)puVar17;
            if (iVar9 == 0) {
                uVar19 = FUN_1400f3554((longlong)plVar13, (LPARAM)lvi_ins);
                plVar35 = (longlong *)(uintptr_t)(uVar19 & 0xffffffffULL);
                puVar33 = local_250;
            }
            else if (*puVar17 != 0) {
                memset(lvi_set, 0, sizeof(lvi_set));
                *(int32_t *)(lvi_set + 8) = iVar9;     /* local_120 = iSubItem */
                *(WCHAR **)(lvi_set + 0x18) = (WCHAR *)puVar17;  /* local_110 = pszText */
                SendMessageW((HWND)(uintptr_t)plVar13[4], 0x1074 /*SETITEMTEXTW*/,
                             (WPARAM)(int)(uintptr_t)plVar35, (LPARAM)lvi_set);
                puVar33 = local_250;
            }
            plVar34 = (longlong *)(uintptr_t)(uint64_t)(uint32_t)(iVar9 + 1);
            puVar18 = local_240;
            puVar17 = puVar33;
            uVar1 = *puVar33;
        }
        uVar1 = *puVar33;
    }
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
    (void)local_80; (void)local_88;   /* xctx 槽位镜像 */
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
    (void)local_e6; (void)local_e4; (void)local_e2;   /* -sl/-sc 解析旗标 */
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
            local_res10 = (WCHAR *)pWVar18;
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
                        local_res10 = (WCHAR *)pWVar18;
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
    uint8_t uVar3;                       /* SetDriveMount 返回旗标 (原体 uVar3, CONCAT71 低字节) */
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
            pcVar23 = (char *)&local_858[0];
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
                                local_8f8 = (uint)*(int16_t *)((uintptr_t)ent + 24 +
                                                              (uintptr_t)local_8b8);
                                if (0 < iVar9) {
                                    puVar20 = (ulonglong *)&pair_area[2];
                                    do {
                                        if ((local_904 == *(short *)((byte *)puVar20 - 2)) &&
                                            (local_8f8 ==
                                             (int)(short)*(int16_t *)puVar20)) break;
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
                                                else {
                                                    /* 原体 @127889 unshow 直除路径 (else-of-E):
                                                     * SetDriveMount 以 (91c<<8|98c) 模式对当前盘/分区做卷映射删除 */
                                                    uVar3 = PECMD_SetDriveMount(
                                                        (int64_t)(intptr_t)local_8e8, local_978,
                                                        local_8f8, local_944,
                                                        ((uint)local_91c << 8) | local_98c,
                                                        local_950, (uint16_t *)local_958,
                                                        &local_908);
                                                    puVar20 = local_8b8;
                                                    uVar8 = local_8f8;
                                                    if ((short)local_878 == 0) {
                                                        if (local_98c != 0) {
                                                            goto LAB_1400d0018;
                                                        }
                                                        if ((local_987 == '\0') ||
                                                            ((((uVar12 & 0x10) == 0 &&
                                                               (uVar12 != 0x84)) &&
                                                              ((uVar12 != 0xef &&
                                                                ((local_986 == '\0' ||
                                                                  ((*(ulonglong *)
                                                                    ((uintptr_t)ent + 0x40 +
                                                                     (uintptr_t)local_8b8) &
                                                                    0xc000000000000000ULL) ==
                                                                   0)))))))) {
                                                            /* 原体 CONCAT71(extraout_var,uVar3):
                                                             * SetDriveMount 返回 uint8_t 成功旗标 */
                                                            if ((uVar3 != 0) &&
                                                                (((short)local_920 == 0 ||
                                                                  (local_979 != '\0')))) {
LAB_1400d0018:;
                                                                /* 移除模式命中或已完成: 关句柄+五连释放+
                                                                 * LeaveCriticalSection 后按 rbx_ret 返回 */
                                                                if (pWVar27 !=
                                                                    (WCHAR *)0xffffffffffffffff) {
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
                                                            wsprintfW(local_958,
                                                                      (LPCWSTR)L"\\Device\\Harddisk%d\\Partition%d",
                                                                      (int)(short)local_978,
                                                                      (int)(short)local_8f8);
                                                            if (-1 < (char)local_990) {
                                                                PECMD_FindVolumeByDeviceId(
                                                                    (uint32_t *)(uintptr_t)
                                                                        ((uintptr_t)ent + 0x30 +
                                                                         (uintptr_t)local_8b8),
                                                                    (int64_t *)&local_958,
                                                                    (LPWSTR)1);
                                                            }
                                                            uVar12 = mount_root[0];
                                                            if (letter_str[0] == L'\0') {
                                                                uVar12 =
                                                                    (uint)PECMD_PickFreeDriveLetter(
                                                                        &local_8f0, 0xc,
                                                                        (char)(local_97b + 0xbf),
                                                                        local_898);
                                                                letter_str[0] = (WCHAR)uVar12;
                                                                mount_root[0] = letter_str[0];
                                                            }
                                                            if ((local_820 != 0) ||
                                                                ((ushort)(letter_str[0] +
                                                                          0xffbf) < 0x1a)) {
                                                                if ((short)uVar12 != 0) {
                                                                    if (local_91c != 0) {
                                                                        /* 原体第 3 参为盘符 (栈变参截断) TODO(verify) */
                                                                        PECMD_TlsLogWrite(
                                                                            (uint64_t)(uintptr_t)g_Script,
                                                                            (LPCWSTR)L"unshow pt: %d:%d %c\r\n",
                                                                            (uint64_t)local_978,
                                                                            (uint64_t)uVar8);
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
                                                                }
                                                                local_938 = local_908;
                                                                uVar8 = local_8a8;
                                                                if (-1 < (int)local_984) {
                                                                    goto LAB_1400d0018;
                                                                }
                                                                goto LAB_1400ceefb;
                                                            }
                                                            if (pWVar27 !=
                                                                (WCHAR *)0xffffffffffffffff) {
                                                                CloseHandle((HANDLE)pWVar27);
                                                            }
                                                            PECMD_FreeStrBuf((WCHAR **)&local_918);
                                                            PECMD_FreeStrBuf((WCHAR **)&local_8c0);
                                                            PECMD_FreeStrBuf(&local_8d0);
                                                            PECMD_FreeStrBuf(&local_958);
                                                            PECMD_FreeStrBuf(&local_900);
                                                            /* 原体 LAB_1400cfff4: LeaveCriticalSection + 返回 */
                                                            goto LAB_1400cfff4;
                                                        }
                                                    }
                                                    local_938 = local_908;
                                                    uVar8 = local_8a8;
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

/* ---- FUN_1400d5b48 (控件图片装载/透明区域) 新增依赖 ---- */
extern int64_t (*DAT_14013ce08)();        /* GdipGetImageWidth  (link_stubs.c) */
extern int64_t (*DAT_14013ce10)();        /* GdipGetImageHeight (link_stubs.c) */
extern int64_t (*DAT_14013ce28)();        /* GdipCreateFromHDC  (link_stubs.c) */
extern int64_t (*DAT_14013ce30)();        /* GdipDeleteGraphics (link_stubs.c) */
extern void (*DAT_14013cd90)();               /* GdipDisposeImage   (link_stubs.c) */
extern void *g_pGdipDrawImageRectRectI;       /* DAT_14013ce20 (core_globals.c) */
extern void (*g_pGdipCreateHBITMAPFromBitmap)(); /* DAT_14013cdf0 (pecmd_defs.h) */
extern void (*g_pGdipCreateBitmapFromScan0)();   /* DAT_14013ce90 */
extern void (*g_pGdipGetImageGraphicsContext)(); /* DAT_14013cea8 */
extern CRITICAL_SECTION g_csCom;              /* DAT_14013e168 */
extern int (*g_pOleInit)(int);                /* DAT_14013d800 */
extern void (*g_pOleUninit)(void);            /* DAT_14013d808 */
extern HINSTANCE g_hInst;                     /* DAT_14013cf70 */
extern int FUN_14005ceec(const uint16_t *a, const char *b);  /* @0x14005ceec 词前缀匹配 (link_stubs.c) */
extern HBITMAP LoadBitmapW(HMODULE, LPCWSTR); /* win32_stub 缺原型 (定义在 link_stubs.c) */

/* OLE32 惰性导入槽 (DAT_14013d810 族, 本 TU 随需补齐; 均未被其他文件引用) */
static longlong b7c_ole_coinitex, b7c_ole_coinisec, b7c_ole_coinit, b7c_ole_couninit;
static longlong b7c_ole_crstream, b7c_ole_cocreate, b7c_ole_coload, b7c_ole_cofree;
static longlong b7c_ole_cofreeex, b7c_ole_stringguid;
static longlong b7c_ole_ole32mod;             /* DAT_14013cfe0 OLE32 模块槽 */

/* win32_stub 缺口补齐: CreateRectRgn/CombineRgn 无既有定义, no-op 占位。
 * TODO(verify): 区域句柄恒 NULL, SetWindowRgn 运行期不生效。 */
static HRGN b7c_CreateRectRgn(int a, int b, int c, int d)
{
    (void)a; (void)b; (void)c; (void)d;
    return (HRGN)0;
}

static int b7c_CombineRgn(HRGN d, HRGN s1, HRGN s2, int mode)
{
    (void)d; (void)s1; (void)s2; (void)mode;
    return 0;
}

/* ========== FUN_1400649f4 @0x1400649f4 (ANSI 十六进制串取值, 直移) ========== */
longlong FUN_1400649f4(unsigned char *p)
{
    longlong v = 0;
    uint ch;
    while (((('/' < (char)*p) && ((char)*p < ':')) ||
            (('`' < (char)(*p | 0x20)) && ((char)(*p | 0x20) < 'g')))) {
        ch = (uint)(char)*p;
        p++;
        if ((int)ch < 0x3a) {
            v = v * 0x10 + (longlong)(int32_t)(ch - '0');
        }
        else {
            v = v * 0x10 + (longlong)(int32_t)((ch | 0x20) - 0x57);
        }
    }
    return v;
}

/* ========== FUN_14005b028 @0x14005b028 (DC 选入对象并记录旧对象, 直移) ========== */
void FUN_14005b028(uint64_t *slot, HDC hdc, HGDIOBJ obj)
{
    slot[2] = (uint64_t)(uintptr_t)hdc;
    slot[0] = (uint64_t)(uintptr_t)obj;
    slot[1] = (uint64_t)(uintptr_t)SelectObject(hdc, obj);
}

/* ========== FUN_14005b07c @0x14005b07c (恢复 DC 并删除新对象, 直移) ========== */
void FUN_14005b07c(uint64_t *slot)
{
    if (slot[1] != 0) {
        SelectObject((HDC)(uintptr_t)slot[2], (HGDIOBJ)(uintptr_t)slot[1]);
        slot[1] = 0;
    }
    if (slot[0] != 0) {
        DeleteObject((HGDIOBJ)(uintptr_t)slot[0]);
        slot[0] = 0;
    }
}

/* ========== FUN_14005c828 @0x14005c828 (GetProcAddress 包装, 直移) ========== */
void FUN_14005c828(LPCSTR proc, LPCSTR lib, longlong *pslot, longlong *phmod)
{
    HMODULE hModule;
    longlong scratch = 0;
    longlong *pl;

    if (*pslot != 0) {
        return;
    }
    pl = &scratch;
    if (phmod != NULL) {
        pl = phmod;
    }
    hModule = (HMODULE)(uintptr_t)*pl;
    if (hModule == NULL) {
        hModule = LoadLibraryA(lib);
        *pl = (longlong)(uintptr_t)hModule;
        if (hModule == NULL) {
            return;
        }
    }
    *pslot = (longlong)(uintptr_t)GetProcAddress(hModule, proc);
}

/* ========== FUN_140061c44 @0x140061c44 (OLE32 导入表惰性加载, 直移) ========== */
longlong FUN_140061c44(void)
{
    if (b7c_ole_stringguid == 0) {                /* DAT_14013d840 判空 */
        FUN_14005c828("CoInitializeEx", "OLE32", &b7c_ole_coinitex, &b7c_ole_ole32mod);
        FUN_14005c828("CoInitializeSecurity", "OLE32", &b7c_ole_coinisec, &b7c_ole_ole32mod);
        FUN_14005c828("CoInitialize", "OLE32", &b7c_ole_coinit, &b7c_ole_ole32mod);
        FUN_14005c828("CoUninitialize", "OLE32", &b7c_ole_couninit, &b7c_ole_ole32mod);
        FUN_14005c828("CreateStreamOnHGlobal", "OLE32", &b7c_ole_crstream, &b7c_ole_ole32mod);
        FUN_14005c828("OleInitialize", "OLE32", (longlong *)&g_pOleInit, &b7c_ole_ole32mod);
        FUN_14005c828("OleUninitialize", "OLE32", (longlong *)&g_pOleUninit, &b7c_ole_ole32mod);
        FUN_14005c828("CoCreateInstance", "OLE32", &b7c_ole_cocreate, &b7c_ole_ole32mod);
        FUN_14005c828("CoLoadLibrary", "OLE32", &b7c_ole_coload, &b7c_ole_ole32mod);
        FUN_14005c828("CoFreeLibrary", "OLE32", &b7c_ole_cofree, &b7c_ole_ole32mod);
        FUN_14005c828("CoFreeUnusedLibrariesEx", "OLE32", &b7c_ole_cofreeex, &b7c_ole_ole32mod);
        FUN_14005c828("StringFromGUID2", "OLE32", &b7c_ole_stringguid, &b7c_ole_ole32mod);
    }
    return b7c_ole_stringguid;
}

/* ========== FUN_140079f50 @0x140079f50 ("路径"#资源名 拆分, 忠实直移) ========== */
LPCWSTR FUN_140079f50(WCHAR **pcursor, uint mode)
{
    WCHAR *base;
    WCHAR quote;
    WCHAR *q;
    LPWSTR hash;
    LPCWSTR ret;
    WCHAR *scan;
    WCHAR *tail;

    base = *pcursor;
    quote = L'"';
    if ((mode & 2) == 0) {
        if (*base != L'"') {
            q = (WCHAR *)StrRChrW(base, NULL, L'\\');
            if (q == NULL) {
                q = base;
            }
            hash = StrRChrW(q, NULL, L'#');
            if (hash == NULL) {
                return NULL;
            }
            if (*hash != L'#') {
                return NULL;
            }
            /* +0xff50: '#' 后须为数字或(带 mode&1)'.'数字 */
            if (9 < (uint16_t)(hash[1] + 0xff50)) {
                if ((mode & 1) == 0) {
                    return NULL;
                }
                if (hash[1] != L'.') {
                    return NULL;
                }
                if (9 < (uint16_t)(hash[2] + 0xff50)) {
                    return NULL;
                }
                return hash;
            }
            return hash;
        }
        base++;
    }
    scan = base;
    b7c_skip_to(&scan, L'"');                     /* thunk_FUN_1400f429c */
    q = scan;
    if (*scan != L'\0') {
        scan++;
    }
    if (quote == *scan) {
        scan++;
    }
    if (*scan != L'#') {
        return NULL;
    }
    if (*q != L'\0') {
        *q = L'\0';
    }
    *pcursor = base;
    ret = scan;
    tail = scan;
    b7c_skip_to(&tail, quote);
    if (*tail != L'\0') {
        *tail = L'\0';
        return ret;
    }
    return ret;
}

/* ========== FUN_14005d83c / 5d7e8 / 5d87c @0x14005d83c 族 (资源类型词判定, 直移) ========== */
static int b7c_restype_icon(const uint16_t *s)
{
    if (FUN_14005ceec(s, "ICON") != 0) return 1;
    if (FUN_14005ceec(s, "#3") != 0) return 1;
    return 0;
}

static int b7c_restype_icongroup(const uint16_t *s)
{
    if (FUN_14005ceec(s, "icon Group") != 0) return 1;
    if (FUN_14005ceec(s, "iconGroup") != 0) return 1;
    if (FUN_14005ceec(s, "#14") != 0) return 1;
    return 0;
}

static int b7c_restype_bitmap(const uint16_t *s)
{
    if (FUN_14005ceec(s, "BITMAP") != 0) return 1;
    if (FUN_14005ceec(s, "#2") != 0) return 1;
    return 0;
}

/* ========== FUN_1400d0b2c @0x1400d0b2c ("#id|类型" 资源描述解析, 忠实直移) ========== */
uint16_t *FUN_1400d0b2c(longlong *pcursor, longlong *pname, uint16_t *deflt)
{
    short ch;
    bool ok;
    uint16_t u;
    short *p;
    ushort *res;
    uint16_t *start;
    uint64_t num;
    WCHAR *ws;

    *pname = *pcursor - 2;
    *(uint16_t *)(*pcursor - 2) = 0x23;
    if (9 < (uint16_t)((ushort)*(short *)(*pcursor) - '0')) {
        *pname = *pcursor;
    }
    p = (short *)(*pcursor);
    while ((*p != 0) && (*p != 0x7c) && (*p != 0x3a)) {
        p++;
        *pcursor = (longlong)(uintptr_t)p;
    }
    ch = *(short *)(*pcursor);
    *(short *)(*pcursor) = 0;
    if (ch != 0) {
        *pcursor += 2;
        ws = (WCHAR *)(*pcursor);
        start = ws;
        u = *start;
        res = start;
        while ((u != 0) && ((u < 9) || (0xd < u)) && (u != 0x20)) {
            res++;
            u = *res;
        }
        *res = 0;
        if (*ws == 0x7c) {
            return ws + 1;
        }
        if (*(short *)(*pcursor) == 0x23) {
            num = 0;
            *pcursor += 2;
            ok = FUN_1400C1194((int64_t *)pcursor, &num);
            if (ok ? 1 : 0) {
                return (uint16_t *)(uintptr_t)num;
            }
        }
    }
    if ((uintptr_t)deflt > 0xffff) {
        if (b7c_restype_icon(deflt)) {
            return (uint16_t *)(uintptr_t)3;
        }
        if (b7c_restype_icongroup(deflt)) {
            return (uint16_t *)(uintptr_t)0xe;
        }
        if (b7c_restype_bitmap(deflt)) {
            return (uint16_t *)(uintptr_t)2;
        }
    }
    return deflt;
}

/* ---- 未移植叶子 (保守失败桩, 与 link_stubs no-op 同风格)。TODO(verify):
 *   FUN_14007f764 frm< 框架样式 / FUN_14007c730 自定义图像解码 /
 *   FUN_14001f1d4 图标提取 / FUN_14001ea18 资源装载 / FUN_14006eaac 流→位图。
 *   返回失败后主流程按原体走 LoadImageW/LoadBitmapW 回退路径。 ---------- */
static longlong *FUN_14007f764_stub(longlong *slot, short *spec, HWND wnd, uint64_t flag)
{
    (void)slot; (void)spec; (void)wnd; (void)flag;
    return (longlong *)0;
}

static HANDLE FUN_14007c730_stub(LPCWSTR path, unsigned char *flagout)
{
    (void)path;
    if (flagout != NULL) {
        *flagout = 0;
    }
    return (HANDLE)0;
}

static HICON FUN_14001f1d4_stub(LPCWSTR path, uint64_t *extra)
{
    (void)path; (void)extra;
    return (HICON)0;
}

static unsigned char *FUN_14001ea18_stub(HMODULE mod, LPCWSTR name, LPCWSTR type,
                                         longlong *out, uint32_t *flags)
{
    (void)mod; (void)name; (void)type; (void)out; (void)flags;
    return NULL;
}

static HANDLE FUN_14006eaac_stub(void *info, HICON *iconout)
{
    (void)info;
    if (iconout != NULL) {
        *iconout = (HICON)0;
    }
    return (HANDLE)0;
}

/* ========== PECMD_CtlLoadPictureRgn @0x1400d5b48 ==========
 * 控件图片装载 + 色键透明区域生成 (decompiled.c @131228 size=5185 忠实直移;
 * 原名 FUN_1400d5b48, 保持本文件可读名以兼容 core_b7a/b7b 调用方)。
 * 流程: 取当前位图(obj+600 元素)/标志字节(obj+0x250) → 空名清区清图失效分支 →
 *   GetWindowRect 尺寸 → "frm<" 框架样式(FUN_14007f764) → "%索引%|%hex%"
 *   序号/色键参数(hex 经 FUN_1400649f4, 至多 8 位) → '*' 星标模式
 *   (local_198=0x42|~|4, 输出直写 obj 位图槽) → "<w,h,x,y,sx,sy>" 几何覆盖
 *   (PECMD_ParseNumSkipChar_0224×6) → 载入分派: '#id' LoadBitmapW /
 *   文件 FUN_14007c730→图标回退链(g_pGdipCreateHBITMAPFromBitmap→OLE CS 内
 *   WIC 对象 FUN_14006e3a4)→LoadImageW / "路径#资源" DLL 链(LoadLibraryExW+
 *   FUN_1400d0b2c+FUN_14001ea18+FUN_14006eaac) → 星标+图标时 GDI+ ARGB
 *   缩放支路(GdipCreateBitmapFromScan0 0x26200a) → 否则常规支路:
 *   GetObject/GetDIBits(32bpp) 逐列色键扫描(XOR 合并竖条区域) → 目标尺寸
 *   重采样(StretchBlt 或 GDI+ DrawImageRectRectI) → SetWindowRgn 应用并
 *   释放旧区(obj+0xf0) → 写回标志字节 → 失效重绘。
 * 返回: 0 (原体 unaff_BL 未定值路径)。
 * TODO(verify): ①原体第 3/4 栈参(size/name)本签名不可见, 恒取 0 与 obj+0x260
 *   缺省路径; ②五个未移植叶子见桩注记; ③「非星标且有旧图且 size<1」分支按
 *   local_120==&local_a0 等价判定; ④色键像素地址按 (src_x+srow)*2 复刻,
 *   Ghidra 的 &pHVar13->unused 偏移未能核验。 */
char PECMD_CtlLoadPictureRgn(int64_t a1, HDC a2)
{
    longlong obj = (longlong)a1;
    HDC hdcSrc = a2;
    uint64_t *bmpslot = (uint64_t *)(obj + 600 * 8);           /* param_1+600 元素 */
    uint64_t old_bmp = *bmpslot;                               /* local_130 初值 */
    unsigned char flagbyte = *(uint8_t *)((char *)obj + 0x250);
    int stack_size = 0;                                        /* in_stack_00000030 */
    LPCWSTR name_arg = NULL;                                   /* in_stack_00000038 */
    char unaff_BL = 0;
    WCHAR *local_190;
    bool bVar4;
    bool bVar5;
    unsigned char local_198;
    RECT rc;
    ulonglong local_100;
    ulonglong local_f8;
    uint64_t *local_120;
    uint64_t local_a0;
    HBITMAP local_180;
    HICON local_138;
    HICON local_108;
    longlong *local_d0;
    ulonglong local_118;
    int local_168;
    uint32_t local_160v;
    uint64_t local_158;
    uint64_t local_148;
    uint64_t local_128v;
    uint64_t local_140;
    longlong lVar25;
    longlong lVar16;
    longlong lVar24;
    uint32_t iVar27;
    uint32_t iVar6;
    uint32_t iVar7;
    uint32_t iVar8;
    int flag170;
    WCHAR WVar22;
    WCHAR WVar2;
    char cVar26;
    LPCWSTR pWVar18;
    LPCWSTR local_150;
    uint16_t *res_type;
    HRGN local_f0;
    int local_e8;
    uint64_t uVar19;
    uint64_t uVar20;
    uint32_t y1;
    uint32_t y2;
    longlong key_color;
    BITMAP bm;
    longlong dst_w;
    longlong dst_h;
    longlong scaled_w;
    longlong scaled_h;
    struct { uint64_t data; uint64_t cap; uint64_t count; } resinfo;

    bVar4 = false;
    bVar5 = false;
    local_198 = 2;
    local_e8 = stack_size;
    local_f0 = (HRGN)0;
    iVar27 = 1;
    local_190 = (WCHAR *)name_arg;
    if (name_arg == NULL) {
        local_190 = (WCHAR *)*(LPCWSTR *)((char *)obj + 0x260);
    }
    else if (*name_arg == L'\0') {
        if ((flagbyte & 0x40) == 0) {
            SetWindowRgn(*(HWND *)((char *)obj + 0x20), (HRGN)0, 1);
            if (*(uint64_t *)((char *)obj + 0xf0) != 0) {
                DeleteObject((HGDIOBJ)(uintptr_t)*(uint64_t *)((char *)obj + 0xf0));
            }
            *(uint64_t *)((char *)obj + 0xf0) = 0;
        }
        if (*bmpslot != 0) {
            if ((*(uint8_t *)((char *)obj + 0x250) & 0x20) == 0) {
                DeleteObject((HGDIOBJ)(uintptr_t)*bmpslot);
            }
            else {
                DAT_14013cd90();
            }
            *bmpslot = 0;
        }
        *(uint8_t *)((char *)obj + 0x250) = 0;
        InvalidateRect(*(HWND *)((char *)obj + 0x20), NULL, 1);
        return unaff_BL;
    }

    if ((HBITMAP)(uintptr_t)old_bmp == 0) {
        *(uint8_t *)((char *)obj + 0x250) = 0;
    }
    rc.left = rc.top = rc.right = rc.bottom = 0;
    GetWindowRect(*(HWND *)((char *)obj + 0x20), &rc);
    local_100 = (ulonglong)(uint32_t)(rc.right - rc.left);
    local_a0 = 0;
    local_f8 = (ulonglong)(uint32_t)(rc.bottom - rc.top);
    local_120 = &local_a0;
    local_180 = (HBITMAP)0;
    local_138 = (HICON)0;
    local_108 = (HICON)0;
    local_d0 = NULL;
    local_118 = 0;
    pWVar18 = local_190;

    /* ---- "frm<" 框架样式前缀 ---- */
    if (FUN_14005c788((char *)"frm<", (uint16_t *)local_190, 4) != 0) {
        pWVar18 += 4;
        local_190 = (WCHAR *)pWVar18;
        b7c_skip_to((WCHAR **)&local_190, L'>');
        WVar22 = *local_190;
        if (WVar22 != L'\0') {
            *local_190 = L'\0';
        }
        FUN_14007f764_stub((longlong *)(obj + 0xc0 * 8), (short *)pWVar18,
                           *(HWND *)((char *)obj + 0x20), 1);
        pWVar18 = local_190;
        if (WVar22 != L'\0') {
            *local_190 = WVar22;
        }
    }

    /* ---- "%索引%" / "%hex%" 参数 ---- */
    WVar22 = L'(';
    if (*pWVar18 == L'%') {
        local_190 = (WCHAR *)(pWVar18 + 1);
        FUN_14005B154((WCHAR **)&local_190);
        WVar2 = *local_190;
        local_118 = 0;
        if (WVar22 == WVar2) {
            int iv = 0;
            PECMD_ParseUIntValue((WCHAR **)&local_190, &iv);
            local_118 = (uint32_t)iv;
        }
        else {
            if ((((uint16_t)WVar2 > 0x2f) && ((uint16_t)WVar2 < 0x3a)) ||
                (((uint16_t)(WVar2 | 0x20) > 0x60) && ((uint16_t)(WVar2 | 0x20) < 0x67))) {
                {
                    int64_t ansi = 0;
                    FUN_14006355c(&ansi, local_190, -1, ~(uint64_t)0);
                    local_118 = (uint64_t)FUN_1400649f4((unsigned char *)(uintptr_t)ansi);
                    PECMD_FreeStrBuf((WCHAR **)&ansi);
                }
                cVar26 = '\0';
                do {
                    WVar22 = *local_190;
                    if ((((uint16_t)WVar22 < 0x30) || (0x39 < (uint16_t)WVar22)) &&
                        (((uint16_t)(WVar22 | 0x20) < 0x61) ||
                         (0x66 < (uint16_t)(WVar22 | 0x20)))) {
                        break;
                    }
                    local_190++;
                    cVar26++;
                } while (cVar26 < '\b');
            }
            FUN_14005B154((WCHAR **)&local_190);
        }
        if (*local_190 == L'%') {
            local_190++;
        }
        FUN_14005B154((WCHAR **)&local_190);
        pWVar18 = local_190;
    }

    /* ---- '*' 星标模式 ---- */
    if (*pWVar18 == L'*') {
        local_190 = (WCHAR *)(pWVar18 + 1);
        WVar22 = *local_190;
        if (WVar22 != L'*') {
            local_198 = 2;
        }
        else {
            local_190 = (WCHAR *)(pWVar18 + 2);
            local_198 = 0x42;
        }
        bVar5 = (WVar22 == L'*');
        if (*local_190 == L'~') {
            local_190++;
            local_198 |= 0x10;
        }
        local_198 |= 4;
        local_120 = bmpslot;
    }
    FUN_14005B154((WCHAR **)&local_190);

    /* ---- "<w,h,x,y,sx,sy>" 几何覆盖 ---- */
    local_128v = 0;
    local_148 = 0;
    local_168 = 0;
    local_140 = 0;
    local_160v = 0;
    local_158 = 0;
    key_color = 0;
    if (*local_190 == L'<') {
        int v1 = (int32_t)local_100;
        int v2 = (int32_t)local_f8;
        int v3 = 0;
        int v4 = 0;
        int v5 = 0;
        int v6 = 0;
        LPCWSTR closep;
        WCHAR savech;
        local_190++;
        closep = local_190;
        b7c_skip_to((WCHAR **)&local_190, L'>');
        pWVar18 = local_190;
        WVar22 = *local_190;
        if (*local_190 != L'\0') {
            *local_190 = L'\0';
            local_190++;
        }
        savech = WVar22;
        PECMD_ParseNumSkipChar_0224((int64_t *)&closep, &v1);
        PECMD_ParseNumSkipChar_0224((int64_t *)&closep, &v2);
        PECMD_ParseNumSkipChar_0224((int64_t *)&closep, &v3);
        PECMD_ParseNumSkipChar_0224((int64_t *)&closep, &v4);
        local_168 = v1;
        v5 = v1;
        PECMD_ParseNumSkipChar_0224((int64_t *)&closep, &v5);
        PECMD_ParseNumSkipChar_0224((int64_t *)&closep, &v6);
        if (savech != L'\0') {
            *(WCHAR *)pWVar18 = L'>';
        }
        local_160v = (uint32_t)v3;
        local_140 = (uint32_t)v4;
        local_158 = (uint32_t)v5;
        key_color = v6;
    }

    /* ---- 载入分派 ---- */
    if ((local_120 != &local_a0) || ((HBITMAP)(uintptr_t)old_bmp == 0) || (stack_size >= 1)) {
        WCHAR *cur = (WCHAR *)local_190;
        LPCWSTR resref = FUN_140079f50(&cur, 1);
        local_190 = cur;
        if (*local_190 == L'#') {
            int resid = 0;
            LPCWSTR pk = local_190;
            PECMD_ParseUIntValue((WCHAR **)&pk, &resid);
            if (0 < resid) {
                local_180 = LoadBitmapW(g_hInst, (LPCWSTR)(uintptr_t)(int64_t)resid);
            }
            goto LAB_1400d6484;
        }
        else if (resref == NULL) {
            unsigned char fflag = 0;
            local_180 = (HBITMAP)(uintptr_t)FUN_14007c730_stub(local_190, &fflag);
            if (((HBITMAP)(uintptr_t)local_180 == 0) && (fflag != 0)) {
                return unaff_BL;
            }
            if (!bVar5) {
                goto LAB_1400d6395;
            }
            while ((HBITMAP)(uintptr_t)local_180 == 0) {
                if (bVar4) {
LAB_1400d63dd:
                    EnterCriticalSection(&g_csCom);
                    FUN_140061c44();
                    iVar27 = (uint32_t)-3;
                    if (g_pOleInit != NULL) {
                        iVar27 = (uint32_t)g_pOleInit(0);
                        if (iVar27 != 0) {
                            if (iVar27 == 1) {
                                g_pOleUninit();
                            }
                        }
                    }
                    local_d0 = (longlong *)(uintptr_t)FUN_14006e3a4(local_190);
                    if (local_d0 != NULL) {
                        (*(void (**)(void *))((uintptr_t)*local_d0 + 0x18))(local_d0);
                        if (local_180 == 0) {
                            (*(void (**)(void *))((uintptr_t)*local_d0 + 0x10))(local_d0);
                            local_d0 = NULL;
                        }
                    }
                    if ((g_pOleUninit != NULL) && (iVar27 == 0)) {
                        g_pOleUninit();
                    }
                    LeaveCriticalSection(&g_csCom);
                    goto LAB_1400d6484;
                }
                {
                    HICON ic = FUN_14001f1d4_stub(local_190, NULL);
                    local_138 = ic;
                    if (ic != 0) {
                        if (bVar5) {
                            goto LAB_1400d6553;
                        }
                        g_pGdipCreateHBITMAPFromBitmap(ic, &local_180, 0);
                        if (local_180 != 0) {
                            break;
                        }
                        DAT_14013cd90();
                        local_138 = 0;
                    }
                }
                if ((!bVar5) || ((bVar4 = true), local_138 != 0)) {
                    if (local_180 == 0) {
                        goto LAB_1400d63dd;
                    }
                    break;
                }
LAB_1400d6395:
                if (local_180 != 0) {
                    break;
                }
                local_180 = LoadImageW((HINSTANCE)0, local_190, 0, 0, 0, 0x10);
            }
        }
        else {
            longlong nm = 0;
            HMODULE hmod;
            {
                longlong cur2 = (longlong)(uintptr_t)resref;
                res_type = FUN_1400d0b2c(&cur2, &nm, (uint16_t *)(uintptr_t)2);
                local_150 = (LPCWSTR)(uintptr_t)nm;
            }
            pWVar18 = local_150;
            if (local_150 != 0) {
                hmod = LoadLibraryExW(local_190, (HANDLE)0, 2);
                *(uint64_t *)(obj + 0x1e0) = (uint64_t)(uintptr_t)hmod;
            }
            if (!bVar5) {
                goto LAB_1400d6258;
            }
            while (*(uint64_t *)(obj + 0x1e0) != 0) {
                if (local_180 != 0) {
                    goto LAB_1400d64eb;
                }
                if (bVar4) {
                    goto LAB_1400d62b5;
                }
                memset(&resinfo, 0, sizeof(resinfo));
                FUN_14001ea18_stub((HMODULE)(uintptr_t)*(uint64_t *)(obj + 0x1e0),
                                   (LPCWSTR)pWVar18, (LPCWSTR)res_type,
                                   (longlong *)&resinfo, NULL);
                local_180 = (HBITMAP)(uintptr_t)FUN_14006eaac_stub(&resinfo, &local_108);
                local_138 = local_108;
                if (bVar5 && local_108 != 0) {
                    goto LAB_1400d6553;
                }
                if ((!bVar5) || (local_108 != 0)) {
                    break;
                }
                bVar4 = true;
LAB_1400d6258:
                if (*(uint64_t *)(obj + 0x1e0) != 0) {
                    local_180 = LoadBitmapW(
                        (HMODULE)(uintptr_t)*(uint64_t *)(obj + 0x1e0),
                        (LPCWSTR)pWVar18);
                }
            }
            if (local_180 == 0) {
LAB_1400d62b5:
                if (*(uint64_t *)(obj + 0x1e0) == 0) {
                    goto LAB_1400d6495;
                }
                FreeLibrary((HMODULE)(uintptr_t)*(uint64_t *)(obj + 0x1e0));
                *(uint64_t *)(obj + 0x1e0) = 0;
                goto LAB_1400d6484;
            }
        }
LAB_1400d6484:
        if ((HBITMAP)(uintptr_t)local_180 == 0) {
            goto LAB_1400d6495;
        }
LAB_1400d64eb:
        *local_120 = local_180;
        if (name_arg != NULL) {
            PECMD_AssignString((int64_t *)((char *)obj + 0x260), name_arg);
        }
        if (((0 < stack_size) || (name_arg != NULL)) &&
            ((HBITMAP)(uintptr_t)old_bmp != 0)) {
            if ((flagbyte & 0x20) == 0) {
                DeleteObject((HGDIOBJ)(uintptr_t)old_bmp);
            }
            else {
                DAT_14013cd90();
            }
        }
LAB_1400d6553:
        if (stack_size < 1) {
            stack_size = *(int32_t *)((char *)obj + 0x17c);
        }
    }

    iVar6 = (uint32_t)stack_size;                 /* local_184 */
    local_108 = (HICON)(uintptr_t)0x60;
    if ((stack_size < 1) || (stack_size == 0x60)) {
        iVar7 = 0;
    }
    else {
        iVar7 = iVar27;
    }
    iVar8 = 0x60;
    if (iVar7 != 0) {
        iVar8 = (uint32_t)stack_size;
    }
    flag170 = (int)iVar7;
    local_150 = (LPCWSTR)(uintptr_t)iVar8;
    if (bVar5 && (local_138 != 0) && (local_120 == bmpslot)) {
        /* ---- GDI+ ARGB 缩放支路 (星标 + 图标) ---- */
        HICON scaled = local_138;
        if ((0 < local_168) || (0 < (int)iVar6)) {
            longlong w_ic = 0;
            longlong h_ic = 0;
            HBITMAP argbbmp = 0;
            void *g2 = NULL;
            DAT_14013ce08(local_138, &w_ic);
            DAT_14013ce10(local_138, &h_ic);
            lVar25 = (longlong)local_168;
            lVar16 = lVar25 * (longlong)iVar8;
            lVar16 = lVar16 / 6 + (lVar16 >> 63);
            uVar19 = (lVar16 >> 4) - (lVar16 >> 63);
            iVar27 = (uint32_t)(int)uVar19;
            lVar16 = (longlong)(int32_t)iVar6 * (longlong)(int32_t)(intptr_t)local_150;
            lVar16 = lVar16 / 6 + (lVar16 >> 63);
            uVar20 = (lVar16 >> 4) - (lVar16 >> 63);
            g_pGdipCreateBitmapFromScan0(uVar19 & 0xffffffffULL,
                                         uVar20 & 0xffffffffULL,
                                         (uint32_t)iVar27 << 2, 0x26200a, NULL, &argbbmp);
            g_pGdipGetImageGraphicsContext(argbbmp, &g2);
            g_pGdipSetInterpolationMode(g2, 7);
            if (0 < (int32_t)local_158) {
                if (0 < (int32_t)key_color) {
                    ((int (*)(void *, void *, int, int, int, int, int, int, int, int,
                              int, int, int, int))g_pGdipDrawImageRectRectI)(
                        g2, (void *)(uintptr_t)local_138, 0, 0, (int)iVar27, (int)uVar20,
                        (int)(((uint32_t)local_160v * w_ic) / (int32_t)local_158),
                        (int)(((int32_t)local_140 * h_ic) / (int32_t)key_color),
                        (int)((lVar25 * w_ic) / (int32_t)local_158),
                        (int)((iVar6 * h_ic) / (int32_t)key_color), 2, 0, 0, 0);
                }
            }
            DAT_14013ce30();
            DAT_14013cd90();
            scaled = (HICON)argbbmp;
        }
        if (local_180 != 0) {
            DeleteObject((HGDIOBJ)(uintptr_t)local_180);
            local_180 = 0;
        }
        *bmpslot = (uint64_t)(uintptr_t)scaled;
        local_198 |= 0x20;
    }
    else {
        /* ---- 常规支路: 位图扫描色键生成区域 ---- */
        uint8_t bmi[0x30];
        HDC hdcMem;
        HGDIOBJ old_sel;
        uint64_t selrec[3];
        int dib_w;
        int dib_h;
        int got_dib;
        unsigned char *pixbuf;
        LPCWSTR lpvBits;
        longlong row_last;
        ulonglong stride;
        ulonglong ratio_y;
        uint32_t keyrgb;
        longlong bmwidth;
        longlong scalew;
        longlong x;
        memset(bmi, 0, sizeof(bmi));
        GetObjectW((HGDIOBJ)(uintptr_t)*local_120, 0x20, &bm);
        local_148 = (uint64_t)(uint32_t)bm.bmHeight;
        local_128v = (uint64_t)(uint32_t)bm.bmWidth;
        flagbyte = (unsigned char)!bVar5;
        dst_w = (longlong)(((int32_t)local_100 -
                            *(int32_t *)((char *)obj + 0x24c)) +
                           *(int32_t *)((char *)obj + 0x238) * -2 -
                           *(int32_t *)((char *)obj + 0x248));
        dst_h = (longlong)(((int32_t)local_f8 -
                            *(int32_t *)((char *)obj + 0x244)) -
                           *(int32_t *)((char *)obj + 0x238) -
                           (*(int32_t *)((char *)obj + 0x240) +
                            *(int32_t *)((char *)obj + 0x23c)));
        hdcMem = CreateCompatibleDC(hdcSrc);
        old_bmp = *bmpslot;
        old_sel = SelectObject(hdcMem, (HGDIOBJ)(uintptr_t)old_bmp);
        scaled_h = (longlong)(int32_t)(intptr_t)local_150;
        scaled_w = (longlong)local_168;
        lVar16 = 0x60;
        if (0 < local_168) {
            scaled_w = (longlong)local_168 * (longlong)(int32_t)(intptr_t)local_150;
            local_108 = (HICON)(uintptr_t)(local_128v * 0x60);
        }
        if (0 < (int)local_148) {
            lVar16 = (longlong)local_148 * 0x60;
            scaled_h = (longlong)(int)local_148 * (longlong)(int32_t)(intptr_t)local_150;
        }
        lVar25 = (longlong)local_148;
        *(uint32_t *)(bmi + 0) = 0x28;            /* biSize */
        got_dib = GetDIBits(hdcSrc, (HBITMAP)(uintptr_t)old_bmp, 0, (uint32_t)lVar25,
                            NULL, (BITMAPINFO *)bmi, 0);
        if (got_dib == 0x57) {
            got_dib = 0;
        }
        dib_w = *(int32_t *)(bmi + 4);            /* biWidth */
        dib_h = *(int32_t *)(bmi + 8);            /* biHeight */
        *(uint16_t *)(bmi + 0x12) = 0x20;         /* planes/bpp 字 (local_52) */
        {
            int64_t buf = 0;
            PECMD_AllocStringSlot2((void *)&buf,
                                   (longlong)(bm.bmHeight + 1) * dib_w * 4);
            pixbuf = (unsigned char *)(uintptr_t)buf;
            lpvBits = (LPCWSTR)(uintptr_t)buf;
        }
        FUN_14005b0b8(pixbuf);
        row_last = (longlong)dib_h - 1;
        iVar7 = 0;
        if (got_dib != 0) {
            iVar7 = GetDIBits(hdcSrc, (HBITMAP)(uintptr_t)old_bmp, 0,
                              (uint32_t)local_148, (void *)lpvBits,
                              (BITMAPINFO *)bmi, 0);
        }
        if (!bVar5) {
            local_f0 = b7c_CreateRectRgn(0, 0, (int)local_100, (int)local_f8);
        }
        if ((flagbyte != 0) && (iVar7 != 0) && (*(uint16_t *)(bmi + 0x12) == 0x20)) {
            stride = ((ulonglong)(uint32_t)dib_w * 4) >> 2;
            ratio_y = ((ulonglong)(lVar16 << 0x20)) / (ulonglong)scaled_h;
            keyrgb = ((local_118 >> 0x10) & 0xff) | ((local_118 & 0xff) << 0x10) |
                     (local_118 & 0xff00);
            bmwidth = (longlong)local_108;
            scalew = scaled_w;
            x = (longlong)(int32_t)local_140;
            while (x < dst_w) {
                longlong src_x = (x * bmwidth) / scalew;
                ulonglong y = 0;                   /* local_b0 恒 0 起点 */
                if ((longlong)y < dst_h) {
                    do {
                        /* 第一段: 逐行测试, 非键色前进 (y1 跟随当前行),
                         * 键色命中或出界时停 —— 对应原体逗号条件式 */
                        y2 = (uint32_t)y;
                        y1 = 0xffffffffu;
                        for (;;) {
                            longlong srow;
                            int cont;
                            if ((longlong)y >= dst_h) {
                                break;
                            }
                            srow = (row_last -
                                    (longlong)((y * ratio_y + 0x80000000ULL) >> 0x20)) *
                                   (longlong)stride;
                            if (srow < 0) {
                                cont = 1;
                            }
                            else {
                                y1 = y2;
                                cont = (keyrgb !=
                                        (*(uint32_t *)(lpvBits + (src_x + srow) * 2) &
                                         0xffffff));
                            }
                            if (!cont) {
                                break;
                            }
                            y++;
                            y2 = (uint32_t)y;
                        }
                        /* 第二段: 连续键色像素延伸 y2 */
                        while ((longlong)(int32_t)y2 < dst_h) {
                            longlong srow2 =
                                (row_last -
                                 (longlong)(((ulonglong)(int32_t)y2 * ratio_y +
                                             0x80000000ULL) >> 0x20)) * (longlong)stride;
                            if ((srow2 < 0) ||
                                (keyrgb !=
                                 (*(uint32_t *)(lpvBits + (src_x + srow2) * 2) &
                                  0xffffff))) {
                                break;
                            }
                            y2++;
                        }
                        if ((int32_t)y1 >= 0) {
                            HRGN rtmp = b7c_CreateRectRgn((int)x, (int)y1, (int)x + 1,
                                                          (int)y2);
                            b7c_CombineRgn(local_f0, local_f0, rtmp, 3);
                            DeleteObject((HGDIOBJ)rtmp);
                        }
                        y2 = y2 + 1;
                        y = (ulonglong)y2;
                    } while ((longlong)y < dst_h);
                }
                x++;
            }
        }
        /* ---- 目标尺寸重采样 ---- */
        lVar16 = (longlong)local_128v;
        lVar25 = (longlong)local_148;
        if ((flag170 != 0) || (local_168 != 0) || ((int)local_140 != 0)) {
            int cw = local_168;
            int chh = (int)local_140;
            longlong tgt = (longlong)(int32_t)(intptr_t)local_150;
            if (cw < 1) {
                local_128v = 1;
                cw = (int)iVar27;
            }
            if (chh < 1) {
                local_148 = 1;
                chh = (int)iVar27;
            }
            {
                longlong newh = ((longlong)chh * tgt * (longlong)local_148) /
                                (local_128v * 0x60);
                HDC hdcDest = CreateCompatibleDC(hdcSrc);
                int neww = (int)((cw * tgt * lVar16) / (local_128v * 0x60));
                HBITMAP nbmp = CreateCompatibleBitmap(hdcSrc, neww, (int)newh);
                HGDIOBJ prev;
                memset(selrec, 0, sizeof(selrec));
                FUN_14005b028(selrec, hdcDest, (HGDIOBJ)nbmp);
                prev = (HGDIOBJ)(uintptr_t)*local_120;
                *local_120 = (uint64_t)(uintptr_t)nbmp;
                selrec[0] = (uint64_t)(uintptr_t)prev;
                local_150 = 0;
                {
                    int sh = (int)lVar25;
                    int sw = (int)lVar16;
                    void *lg = NULL;
                    uint32_t gr;
                    if ((local_138 == 0) ||
                        ((gr = (uint32_t)DAT_14013ce28(hdcDest, &lg), gr != 0))) {
                        StretchBlt(hdcDest, 0, 0, neww, (int)newh, hdcMem, 0, 0, sw, sh,
                                   0xcc0020);
                    }
                    else {
                        ((int (*)(void *, void *, int, int, int, int, int, int, int, int,
                                  int, int, int, int))g_pGdipDrawImageRectRectI)(
                            lg, (void *)(uintptr_t)local_138, 0, 0, neww, (int)newh,
                            0, 0, sw, sh, 2, 0, 0, 0);
                    }
                    if (lg != NULL) {
                        DAT_14013ce30();
                    }
                }
                FUN_14005b07c(selrec);
                if (hdcDest != 0) {
                    DeleteDC(hdcDest);
                }
            }
        }
        if (old_sel != 0) {
            SelectObject(hdcMem, old_sel);
        }
        PECMD_FreeStrBuf((WCHAR **)(void *)&pixbuf);
        if (hdcMem != 0) {
            DeleteDC(hdcMem);
        }
        lVar24 = 0;
        (void)lVar24;
    }

    /* ---- 区域应用/释放 + 收尾 (两支路汇合点) ---- */
    {
        HRGN newrgn = local_f0;
        HRGN delrgn;
        if (!bVar5) {
            delrgn = *(HRGN *)((char *)obj + 0xf0);
            SetWindowRgn(*(HWND *)((char *)obj + 0x20), local_f0, 1);
            *(HRGN *)((char *)obj + 0xf0) = newrgn;
        }
        else {
            delrgn = local_f0;
        }
        if (delrgn != 0) {
            DeleteObject((HGDIOBJ)delrgn);
        }
    }
    *(uint8_t *)((char *)obj + 0x250) = local_198;
    if ((0 < local_e8) || (name_arg != NULL)) {
        InvalidateRect(*(HWND *)((char *)obj + 0x20), NULL, 1);
    }
LAB_1400d6495:
    if (local_a0 != 0) {
        DeleteObject((HGDIOBJ)(uintptr_t)local_a0);
    }
    if (local_d0 != NULL) {
        (*(void (**)(void *))((uintptr_t)*local_d0 + 0x10))(local_d0);
    }
    if (local_138 != 0) {
        DAT_14013cd90();
    }
    return unaff_BL;
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
