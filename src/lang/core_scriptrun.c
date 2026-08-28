/* ====================================================================
 * core_scriptrun.c — ScriptRunA 命令行脚本调度（主体段）
 *
 *   PECMD_RunCommand @0x140031454   命令行处理器主流程
 *
 * 段1（前缀指令解析）在 core_srparse.c（PECMD_SrParsePrefix），
 * 本文件实现主展开路径（LAB_140031887 起，decompiled.c 29635-30332）：
 *   *map: 前缀 / 命令 token → 裸路径脚本执行 / 资源脚本执行 /
 *   变量执行，最后 sysinit 收尾。
 *
 * S10（本轮）: 补齐原文 dc:29640-30062 的命令 token 分类与分支——
 *   1) 无 '#' 修饰的裸路径 → 执行脚本文件（原文 dc:30060-30066 走
 *      FUN_140031068=PECMD_ExecuteScriptBlock；见 srx_ExecuteScriptFile
 *      头部注释关于工程内 PECMD_LoadScriptFileSegment 缺合并段的登记）；
 *   2) 词首单个反斜杠 → 逐盘符探测执行 (dc:30019-30057)；
 *   3) '#' 定位 → 资源路径；mem 前缀 → 变量路径（既有代码归位原分支次序）。
 *   分支条件均以 decompiled.c 字节级核对（1400319c1/9cf/9d9 反斜杠标志
 *   经 ASM 验证为词首 2 字符，与 pthreadmbcinfo 字段渲染无关）。
 * ==================================================================== */
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>

#include "pecmd_defs.h"
#include "win32_stub.h"
extern WCHAR **FUN_14005B154(WCHAR **pp); /* @0x14005b154 */

/* 段1：前缀解析（core_srparse.c） */
extern int64_t PECMD_SrParsePrefix(void *script, WCHAR **pp, int *flags, int *flags2,
                                   uint8_t *p_m_flag, uint8_t *p_mem_flag, WCHAR **psysinit_name,
                                   bool *p_sysinit, WCHAR **p_outbuf,
                                   int *p_qkmode); /* @0x140031454 段1 */

/* core_execline.c 展开器 */
extern void FUN_14007BF44(void *script, WCHAR *line, WCHAR **out, int mode,
                          uint8_t opt); /* @0x14007bf44 */

/* 核心辅助 */
extern void *PECMD_GrowByteBuffer(void **ps, int64_t len);          /* @0x140063424 */
extern void *PECMD_HeapRealloc(void *ptr, size_t size);             /* @0x140063118 */
extern void PECMD_SwapBytePairs(uint8_t *p, int n); /* @0x140060a74 restored_bodies.c:12197 (WORD 内两字节互换) */
extern uint16_t PECMD_GenRandomSeed16(void);                        /* @0x14001b510 */
extern int64_t FUN_14001B5AC(void *buf, uint32_t key, int64_t len); /* @0x14001b5ac */
extern uint8_t *PECMD_VarLookup(void *script, LPCWSTR name, void *scope, int64_t len,
                                void **out);                           /* @0x140018978 */
extern int64_t PECMD_StrChrOffset(const WCHAR *s, WCHAR c);            /* @0x14001b4f8 */
extern WCHAR *FUN_140024C48(WCHAR **pp, size_t *plen, uint32_t flags); /* @0x140024c48 */
extern uint8_t *FUN_14001EA18(HMODULE mod, LPCWSTR id, LPCWSTR type, void **out,
                              uint32_t *flags_out);                    /* @0x14001ea18 */
extern void FUN_140017CDC(void *dst, void *src);                       /* @0x140017cdc */
extern void FUN_1400186BC(void *s, void *parent);                      /* @0x1400186bc */
extern void PECMD_InitObfuscatedKeywords(void *script, uint64_t seed); /* @0x14006159c */
extern void FUN_14004EAA8(void *script, int flag);                     /* @0x14004eaa8 */
extern void FUN_14007034C(void **ps, LPCWSTR src);                     /* @0x14007034c */
extern int32_t g_sysinitState;                                         /* DAT_14013d058 */
extern WCHAR *g_sysinitName;                                           /* DAT_14013d060 */

/* 未实现（TODO(verify) 挂起） */
extern uint32_t PECMD_InvokeSubRoutine(void *buf, void *script, uint32_t flags); /* @0x140030dcc */
extern uint32_t FUN_1400E7D58(int64_t *ps, uint32_t flags);                      /* @0x1400e7d58 */
extern void *PECMD_PrependEnviHeader(uint32_t key, void **buf, LPCWSTR extra, uint32_t f,
                                     int off); /* @0x140024f20 */
extern WCHAR *PECMD_PrependCallSubLine(uint32_t key, void **buf, LPCWSTR name,
                                       int64_t a4);                         /* @0x140030f1c */
extern WCHAR *PECMD_UnquoteTokenInPlace(WCHAR *p);                          /* @0x14001d5f4 */
extern void PECMD_ParseIntSkipSepChar(WCHAR **pp, int64_t *out, WCHAR sep); /* @0x1400679dc */
extern bool PECMD_ParseUIntValue(WCHAR **pp, int64_t *out);                 /* @0x140074838 */
extern int64_t PECMD_RunScriptText(void *pScript, LPCWSTR pText, LPCWSTR pName, LPCWSTR pCurFile,
                                   uint32_t flags, LPCWSTR pFile,
                                   void *pPersist); /* @0x1400b638c core_execmain.c */
extern DWORD PECMD_ExecuteScriptBlock(void *script, LPCWSTR a2, LPCWSTR a3, uint32_t flags,
                                      LPCWSTR a5, LPCWSTR a6); /* @0x140031068 */
extern void FUN_14006F884(LPCWSTR name, WCHAR **out);          /* @0x14006f884 */
extern void PECMD_CheckFirstStartupFlag(void *script);         /* @0x1400251ac */
extern void PECMD_RunSysInit(void *script, LPCWSTR name);      /* @0x140025180 */
extern void FUN_14009BB28(void *script, int flag);             /* @0x14009bb28 */
extern HANDLE PECMD_OpenFileHandle(HANDLE *out, LPCWSTR path, DWORD access, DWORD share,
                                   LPSECURITY_ATTRIBUTES sa, DWORD disp, DWORD flags,
                                   HANDLE tmpl);   /* @0x140003864 core_exec2.c */
extern bool FUN_140101E70(LPCWSTR path);           /* @0x140101e70 文件存在 core_exec2.c */

/* core_string.c / core_var.c / core_token.c 符号 (未入公共头, 显式声明) */
extern void PECMD_AllocWStringBuffer(WCHAR **ps, int64_t count); /* @0x140063694 core_var.c */
extern void PECMD_FreeStrBuf(WCHAR **ps);                         /* @0x14005b104 core_string.c */
extern WCHAR *FUN_14006375C(WCHAR **ps, LPCWSTR src);             /* @0x14006375c core_string.c */
/* S11: 本地声明与定义冲突已删除, 统一采用 xproto.h 原型 (原: extern int64_t FUN_14005C788(const char *s, WCHAR *p, int n); /* @0x14005c788 词比较 * /) */
extern void FUN_1400702B0(WCHAR **ps, LPCWSTR src);               /* @0x1400702b0 core_string.c (stubs_common.h 同声明) */

/* ========== srx_ExecuteScriptFile — LOAD 脚本文件装载执行 ==========
 * 语义 = 原版裸路径分支 (dc:30060-30066) → FUN_140031068 (ExecuteScriptBlock)
 * → PECMD_LoadScriptFileSegment(FUN_1400307c8) 读文件 → 编码流 →
 * PECMD_RunScriptText(key 分隔) 的执行链。
 *
 * S10 登记（SKIP 级，需后续修）:
 *   工程内 PECMD_LoadScriptFileSegment (core_script.c:180) 的移植体
 *   **缺"文件内容→编码流"合并段** —— 只把 "path\n" 追加到 out 并丢弃读入
 *   内容（decompiled.c FUN_1400307c8 原文会把 XOR 后的内容合并进 ctrl 并
 *   递归 PECMD_ParseScriptSegments）。因此直接调 PECMD_ExecuteScriptBlock
 *   会因 StrChrOffset(流,key)==0 静默空转。本函数按原文语义实现等价装载:
 *   读文件(UTF-16LE, 跳 BOM) → 明文流 → key=0 RunScriptText。
 *   key=0 明文流与 S7 启动路径实测一致（[DEB] end88=0 sep48=0, PSB 收明文行）；
 *   DispatchExpressionBlock 行解码头(script+0x48)为 0 时 XOR 恒等，
 *   \r\n 即行分隔（InitObfuscatedKeywords(seed=0): 0x8a=0xd,0x90=0xa）。
 *   TODO(verify): 原版以 GenRandomSeed16 内存 XOR 混淆编码流；本实现取
 *   key=0 明文流，运行时解码面行为等价（字节级混淆差异已登记）。
 * 所有权: pText 交 PECMD_RunScriptText 后为 takeover（内层释放），不再释放。
 */
static int64_t srx_ExecuteScriptFile(void *script, LPCWSTR cmd, LPCWSTR a3, uint32_t flags,
                                     LPCWSTR extra, LPCWSTR logs)
{
    /* FIX(R20C): dc 裸文件执行入口 FUN_140031068 在装载前种子化关键字
     * (dc:29410 FUN_14006159c(clone,key)；形态同 dc:110331)。本替代路径原先
     * key=0 明文直跑且从不写 script+0x88/8A/90 分隔符——下游 ExtractTableSegment
     * 扫描器无 NUL 兜底，踩到未初始化堆即静默自旋(021/037/038 等五案挂死根因)。
     * seed 取 script+0x48 低字 | 0x10000(高位仅门控花括号表, 与 rb:7869 同形)。 */
    PECMD_InitObfuscatedKeywords(script,
        (uint64_t)(*(uint16_t *)((char *)script + 0x48)) | 0x10000ULL);
    HANDLE h = NULL;
    LARGE_INTEGER fsz;
    uint8_t *buf = NULL;
    DWORD rd = 0;
    int64_t wchars;
    WCHAR *text = NULL;
    int64_t ret;
    int bomSkip = 0;
    WCHAR *convText = NULL; /* S11(方案B): MBCS→宽转换产物 */

    (void)extra;
    (void)logs;
    fsz.QuadPart = 0;
    PECMD_OpenFileHandle(&h, cmd, 0x80000000, 7, NULL, 3, 0, (HANDLE)0); /* dc:0307c8 同款 */
    if (h == NULL || h == INVALID_HANDLE_VALUE) {
        ret = (int64_t)GetLastError(); /* 原版: LoadScriptFileSegment -1 → GetLastError */
        goto done;
    }
    if (GetFileSizeEx(h, &fsz) == 0 || fsz.QuadPart < 1) {
        ret = (int64_t)GetLastError();
        goto done;
    }
    if (fsz.QuadPart > 0x10000000) {
        fsz.QuadPart = 0x10000000; /* dc:0307c8 同款上限 */
    }
    buf = (uint8_t *)PECMD_GrowByteBuffer((void **)&buf, fsz.QuadPart + 2);
    if (buf == NULL) {
        ret = (int64_t)GetLastError();
        goto done;
    }
    if (!ReadFile(h, buf, (DWORD)fsz.QuadPart, &rd, NULL)) {
        ret = (int64_t)GetLastError();
        goto done;
    }
    CloseHandle(h);
    h = NULL;
    /* ==== S11 编码嗅探 + ANSI→宽转换 (方案B, 见 build/msvc/s11_encoding_sniff_patch.md) ====
     * 语义源: FUN_1400685f4 PECMD_ReadFileToWideString (dc:64780-64866);
     * BOM 判定 dc:100148-100165; CP 字面量 dc:64837(CP_UTF8,8)/64848(CP_ACP,0)。
     * 注: 原版 LOAD 锚点(dc:29161-29190)明文路径无 ANSI→宽, 属工程前移(§1.4 登记差异)。 */
    {
        int      isWide = 0;  /* 缓冲可按 UTF-16LE 直取 */
        int      mbOff  = 0;  /* MBCS 数据起始偏移 (UTF-8 BOM=3, dc:64834 uVar7) */
        uint8_t *cpsrc  = buf;

        if (rd >= 2 && buf[0] == 0xFE && buf[1] == 0xFF) {
            /* dc:64811-64815 UTF-16BE BOM -> 整块换序转 LE 视角 */
            PECMD_SwapBytePairs(buf, (int)rd);
            isWide = 1; bomSkip = 1;
        } else if (rd >= 2 && buf[0] == 0xFF && buf[1] == 0xFE) {
            isWide = 1; bomSkip = 1;           /* dc:64816-64827 LE BOM */
        } else if (rd >= 3 && buf[0] == 0xEF && buf[1] == 0xBB && buf[2] == 0xBF) {
            mbOff = 3;                         /* dc:100162-100165 UTF-8 BOM */
        }

        if (isWide) {
            wchars = (int64_t)((rd - (DWORD)(bomSkip * 2)) >> 1);
        } else {
            /* MBCS(UTF-8/ANSI)->宽: 先试 CP_UTF8|MB_ERR_INVALID_CHARS (dc:64837),
             * 失败回退 CP_ACP 字面量 0 (dc:64848)。g_SysCodePage 取舍见补丁稿 §4.3。 */
            unsigned int cp  = 65001u; /* 0xfde9 CP_UTF8 */
            int  off = mbOff;
            int  n;
            n = MultiByteToWideChar(cp, 8 /*MB_ERR_INVALID_CHARS*/, (LPCSTR)(buf + off),
                                    (int)(rd - (DWORD)off), NULL, 0);
            if (n <= 0) {
                /* dc:64848 原文回退从偏移0全长转(含 BOM 三字节怪癖); 本稿保留
                 * 跳过 BOM (一处有意偏离 §5.1, 防 BOM 变首行乱码) */
                cp = 0u; /* CP_ACP */
                n  = MultiByteToWideChar(cp, 8, (LPCSTR)(buf + off),
                                         (int)(rd - (DWORD)off), NULL, 0);
            }
            if (n <= 0) {
                ret = (int64_t)GetLastError();
                goto done;
            }
            PECMD_AllocWStringBuffer(&convText, (int64_t)n + 1 + 0x10);
            if (convText == NULL ||
                MultiByteToWideChar(cp, 8, (LPCSTR)(buf + off),
                                    (int)(rd - (DWORD)off), convText, n) <= 0) {
                ret = (int64_t)GetLastError();
                goto done;
            }
            convText[n] = L'\0';
            cpsrc  = (uint8_t *)convText;
            wchars = (int64_t)n;
        }

        {
            int64_t nn = wchars + 1 + 0x10;
            /* S10 幽灵行松弛保留: PSB 行游标越过终止符时落在零填充冗余内 */
            PECMD_AllocWStringBuffer(&text, nn); /* 带计数头的副本 (Adopt 兼容) */
            if (text == NULL) {
                ret = (int64_t)GetLastError();
                goto done;
            }
            memcpy(text, cpsrc + (size_t)(isWide ? bomSkip * 2 : 0),
                   (size_t)wchars * 2);
            text[wchars] = 0;
            {
                int64_t z;
                for (z = wchars + 1; z < nn; z++) text[z] = 0;
            }
        }
        if (convText != NULL) {
            PECMD_FreeStrBuf(&convText);      /* 已复制进 text, 立即回收 */
            convText = NULL;
        }
    }
    if ((flags & 8) != 0) { /* dc:29425-bit3: 执行后删源文件 */
        DeleteFileW(cmd);
    }
    /* 原 FUN_140031068 尾部: RunScriptText(script, 流, pName=a3, pCurFile=cmd, flags) */
    ret = PECMD_RunScriptText(script, text, a3, cmd, flags & 0xfffffff7u, NULL, NULL);
    text = NULL; /* takeover: 内层已持有并释放 (T1e 契约) */
done:
    if (h != NULL && h != INVALID_HANDLE_VALUE) {
        CloseHandle(h);
    }
    if (buf != NULL) {
        /* T4 缺陷丙·v3 归正(2026-08-25 Round14): buf 经 GrowByteBuffer→
         * PECMD_HeapRealloc(=FUN_140063118) 分配 —— 带 8 字节头{size,magic},
         * 返回 hdr+8 (dc:60560-60566); 原文对该家族的释放即 ptr-8 回拨
         * (dc:60586/dc:60773)。v2 改 HeapFree(g_hHeap,0,buf) 实为释放块内
         * 指针 → RtlFreeHeap 判堆损坏 c0000374(dump pecmd_msvc.exe.15364
         * 栈帧 srx+0x671=call HeapFree 返回址 +0x11de71 反汇编实锤, 全仓
         * 31 处 HeapFree 唯一缺 -8 处)。恢复原文 -8 契约: */
        PECMD_FreeStrBuf((WCHAR **)&buf);
        buf = NULL;
    }
    if (convText != NULL) { /* MBCS 分支中途 goto done 的兜底回收 */
        PECMD_FreeStrBuf(&convText);
    }
    return ret;
}

/* ========== PECMD_RunCommand @0x140031454 ==========
 * 命令行脚本调度主流程。cmdline 被就地解析（截断）。
 */
int64_t PECMD_RunCommand(void *script, WCHAR *cmdline)
{
    WCHAR *pp = cmdline;
    int flags = 0, flags2 = 0;
    uint8_t m_flag = 0, mem_flag = 0;
    WCHAR *sysinit_name = NULL;
    bool b_sysinit = false;
    WCHAR *outbuf = NULL;
    int qkmode = 0;
    int64_t DVar13 = 0;

    /* ---- 段1：前缀指令解析 ---- */
    PECMD_SrParsePrefix(script, &pp, &flags, &flags2, &m_flag, &mem_flag, &sysinit_name, &b_sysinit,
                        &outbuf, &qkmode);

    /* ---- 主展开路径（LAB_140031887，dc:29635-30332）---- */
    {
        WCHAR *p = pp;
        WCHAR *pt2;
        WCHAR *tokenEnd;              /* pWVar16 (dc:29666) token 结束指针 */
        size_t tokLen = 0;
        int64_t lVar32 = 0;
        int64_t lVar12 = 0;
        uint32_t uVar31 = 0, uVar24 = 0, uVar34 = 0, uVar35 = 0;
        uint16_t uVar42 = 0;
        int64_t local_234 = 0;
        int64_t local_26c = 0x7fffffff;
        WCHAR *local_250 = NULL, *local_278 = NULL, *local_260 = NULL;
        WCHAR *local_1d0 = NULL, *local_1e8 = NULL, *local_1f8 = NULL;
        WCHAR *local_160 = NULL, *local_240 = NULL, *local_210 = NULL;
        WCHAR *local_1b8 = NULL, *local_150 = NULL, *local_228 = NULL;
        WCHAR *local_1f0 = NULL;
        HMODULE hmod = 0;
        WCHAR *local_1a0 = NULL;
        WCHAR *mapStr = NULL;
        uint64_t mapOff = 0;
        int q1 = 0;                   /* local_298 词首反斜杠标志 (ASM 核实) */
        int starFlag = 0;             /* local_280 '*' 前缀标志 */
        WCHAR driveChar = 0;          /* local_270 ',' 后参数首字符(大写) */
        WCHAR firstCh = 0;            /* iVar11 命令词首字符 */
        WCHAR *line = NULL;           /* local_278 展开后完整命令行 */
        WCHAR *wcls = NULL;           /* local_240 分类用的词副本 */
        WCHAR *ptHash = NULL;         /* 分类结果: '#' 定位或 NULL(裸路径) */
        WCHAR *unq = NULL;            /* 去引号词 (裸路径/变量路径用) */
        uint8_t sub[0x300];           /* 资源分支的脚本克隆缓冲 (dc:local_138) */
        WCHAR emptyW = L'\0';         /* 空串兜底 (空命令行) */
        int branches = 0;             /* 分支已执行标志: 1=map 2=变量 4=裸路径 8=资源 16=盘符 */

        /* 156-169：引号组合/实例检测（".# 组合 → g_hInstance 模式） */
        pt2 = p;
        if (*p == L'*')
            pt2 = p + 1;
        if (*pt2 == L'"' && pt2[1] == L'.' && (uint16_t)pt2[2] == 0x22 && pt2[3] == L'#') {
            /* ".# 组合 → g_hInstance 模式（TODO(verify): 原语义为版本串检测） */
            pt2 = pt2 + 3;
            lVar32 = (int64_t)(pt2 - p) >> 1;
        }

        /* 170-173：sysinit_end 检查 */
        if (sysinit_name != NULL && qkmode == 0 && g_sysinitState == 0) {
            g_sysinitState = 3;
            g_sysinitName = sysinit_name;
        }

        /* 174-198：*map: 前缀 + 命令 token */
        p = pt2;
        if (FUN_14005C788("*map:", p, 5) != 1 && *p == L'*') {
            p++;   /* dc:29659-29662 star 前缀 (非 *map:) */
            starFlag = 1;
        }
        pt2 = p;
        {
            tokenEnd = FUN_140024C48(&p, &tokLen, 0x21); /* dc:29666 */
            lVar32 = (int64_t)tokLen;
        }
        {
            /* 引号/反斜杠标志（dc:29667-29678；ASM 1400319c1/9cf/9d9 核实:
             * 反斜杠判词首 2 字符（引号后 +1），非 pthreadmbcinfo 字段偏移） */
            WCHAR *chk = p;
            if (*p == L'"')
                chk = p + 1;
            if (*chk == L'\\' && chk[1] != L'\\')
                q1 = 1;
            if (*p == L'*') {
                p++;        /* dc:29675-29677 词首 '*' 跳过 */
                starFlag = 1;
            }
        }
        firstCh = *p; /* dc:29655 iVar11 (词首字符) */
        /* 199-210：命令名拷贝（token 长度 lVar32） */
        if (lVar32 >= 0) {
            PECMD_AllocWStringBuffer(&local_250, lVar32);
            if (local_250 != NULL && lVar32 > 0) {
                memcpy(local_250, pt2, (size_t)lVar32 * 2);
                local_250[lVar32] = 0;
            }
        }
        /* dc:29777-29797 token 末端 ','/引号分流（local_270 盘符过滤字符） */
        {
            WCHAR *wscan = local_250;
            if (*local_250 == L'"') {
                wscan = local_250 + 1;
                while (*wscan != L'\0' && *wscan != L'"')
                    wscan++;
                if (*wscan == L'"')
                    wscan++; /* dc:29783-29785 越过闭合引号 */
            }
            else {
                while (*wscan != L'\0' && *wscan != L',')
                    wscan++;
            }
            if (*wscan == L',') {
                *wscan = L'\0';
                driveChar = wscan[1] & 0xffdf; /* dc:29795-29797 小写转大写 */
            }
        }

        /* ---- 展开命令词 + 余串合并（dc:29799-29826）---- */
        FUN_14007BF44(script, local_250, &line, 0, 1); /* dc:29799 展开首 token */
        /* 320-343 等价: 结果首字符处理（引号包裹/剥离, dc:29800-29822 简版） */
        {
            WCHAR *e = line;
            if (e != NULL && *e != L'"' && *e != L'\0') {
                /* 原文 LAB_140031aec: 展开结果内首个空白处重建引号串。
                 * 对 LOAD 裸路径/资源/变量路径无语义影响, 保留既有简版(无重建)。 */
                while (*e != L'\0' && !((*e > 8 && *e < 0xe) || *e == L' ')) {
                    e++;
                }
            }
        }
        { /* dc:29824-29826 余串 token 化 + 展开 + 并入 line */
            size_t restLen = 0;
            WCHAR *restExp = NULL;
            FUN_140024C48(&p, &restLen, 0x20);
            FUN_14007BF44(script, tokenEnd, &restExp, 0, 1);
            if (restExp != NULL) {
                FUN_14006375C(&line, restExp);
            }
            PECMD_FreeStrBuf(&restExp);
        }

        /* ---- *map: 前缀检测（dc:29830-29870）---- */
        if (line != NULL && FUN_14005C788("*map:", line, 5) == 1) {
            mapStr = line;
        }
        if (mapStr != NULL) {
            /* 映射文件读取（dc:344-390 简版, 既有无改动） */
            WCHAR *ms = mapStr + 5;
            if (PECMD_ParseUIntValue(&ms, (int64_t *)&mapOff) > 0) {
                void *mv = MapViewOfFile((HANDLE)(intptr_t)mapOff, 6, 0, 0, 8);
                if (mv != NULL) {
                    int64_t sz = *(int64_t *)mv;
                    UnmapViewOfFile(mv);
                    if (sz > 0) {
                        void *mv2 =
                            MapViewOfFile((HANDLE)(intptr_t)mapOff, 6, 0, 0, (size_t)(sz + 2));
                        if (mv2 != NULL) {
                            local_1a0 = (WCHAR *)mv2;
                            /* TODO(verify): 反编译 379-390 行映射数据拷贝与解码 */
                        }
                    }
                }
            }
            branches |= 1;
            goto srx_tail;
        }

        /* ---- token 分类（dc:29871-30002，本 S10 补齐）----
         * 在展开行副本上做 引号/星号/'#' 定位:
         *   裸路径: 无引号修饰且无 '#' → ptHash=NULL
         *   资源:   词首/'#'(-含星号后末反斜杠处数字资源) → ptHash 指向 '#'
         * 注: 原文转录起点含词前 1 字符并以 +1 定位(local_240), 对本分类无语义。
         */
        if (line != NULL) {
            FUN_1400702B0(&wcls, line);
        }
        if (wcls != NULL) {
            WCHAR *cp = wcls;
            if (*cp == L'"') {
                /* dc:29939-29956 引号词: 找闭合引号并截断 */
                WCHAR *qq = cp + 1;
                while (*qq != L'\0' && *qq != L'"')
                    qq++;
                if (*qq == L'"') {
                    *qq = L'\0';
                    qq++;
                    cp = qq; /* u24 = star|0x10 != 0 → 越过引号 */
                }
                /* else: 未闭合(u24=star=0) → cp 保持词起点, 交由去引号处理 */
                if (*cp == L'#') {
                    ptHash = cp;
                }
            }
            else if (starFlag != 0) {
                /* dc:29961-29989 '*' 前缀词: 末反斜杠后的 '#' 定位 (数字资源) */
                WCHAR *bsl = StrRChrW(cp, NULL, L'\\');
                WCHAR *hh;
                if (bsl == NULL)
                    bsl = cp;
                hh = StrRChrW(bsl, NULL, L'#');
                if (hh != NULL) {
                    WCHAR *dig = hh;
                    WCHAR *resAt = NULL;
                    while (*dig == L'#')
                        dig++;
                    if ((uint16_t)(*dig - 0x30) < 10)
                        resAt = hh; /* '#' 后为数字 → 资源 id */
                    while ((uint16_t)(*dig - 0x30) < 10)
                        dig++;
                    if (*dig == L':')
                        resAt = hh; /* 数字后 ':' → 资源 id */
                    if (resAt != NULL && *resAt == L'#')
                        ptHash = resAt;
                }
            }
            else {
                /* dc:29958-29960 无前缀: 词首 '#' 即资源 */
                if (*cp == L'#')
                    ptHash = cp;
            }
        }

        /* ---- 分支分派（dc:30016-30332; mem 优先于 map, map 优先于裸/资源）---- */
        if (mem_flag != 0) {
            /* ---- 变量执行路径（dc:30273-30332, 既有代码归位）---- */
            WCHAR *vp = wcls;
            int64_t resSlot[2];
            uint16_t seed;
            if (vp != NULL)
                vp = PECMD_UnquoteTokenInPlace(vp); /* dc:30274 去引号 */
            unq = vp;
            {
                WCHAR *ve = vp;
                while (ve != NULL && *ve != L'\0' && !((*ve > 8 && *ve < 0xe) || *ve == L' ')) {
                    ve++;
                }
                if (ve != NULL && *ve != L'\0')
                    *ve = L'\0';
            }
            PECMD_AllocWStringBuffer(&local_1b8, 0);
            if (vp != NULL) {
                FUN_1400702B0(&local_1b8, vp); /* dc:30284 变量名副本 */
            }
            if (local_1b8 != NULL &&
                (*local_1b8 == L'&' || *(char *)((char *)script + 0xd) != '\0')) {
                /* 变量表查找 */
                EnterCriticalSection(&g_csInit);
                {
                    uint8_t *node = PECMD_VarLookup(script, local_1b8, NULL, -1, NULL);
                    if (node != NULL) {
                        int64_t vlen = *(int64_t *)((char *)node + 0x18);
                        PECMD_AllocWStringBuffer(&local_210, vlen);
                        memcpy(local_210, *(void **)((char *)node + 8), (size_t)vlen);
                        local_210[vlen] = 0;
                    }
                }
                LeaveCriticalSection(&g_csInit);
            }
            else {
                FUN_14006F884(local_1b8, &local_210);
            }
            if (local_210 != NULL) {
                /* 原文 dc:30309-30316: 调 ResDecode 前把字节长度写在槽变量相邻
                 * (local_208 = lstrlenW*2), ResDecode 经 param_1[1] 读取。
                 * 曾漏写导致长度=栈上垃圾 → memmove 巨长度 AV (windbg 活体+dump 实锤)。 */
                resSlot[0] = (int64_t)(intptr_t)local_210;
                resSlot[1] = (int64_t)lstrlenW(local_210) * 2;
                FUN_1400E7D58(resSlot, 1);
                local_210 = (WCHAR *)(intptr_t)resSlot[0]; /* 取回可能被重分配的指针 */
                {
                    seed = PECMD_GenRandomSeed16();
                    FUN_14001B5AC(local_210, 0, 0);
                    PECMD_InvokeSubRoutine(&local_210, script, 0);
                    {
                        int64_t pos = PECMD_StrChrOffset(local_210, 0);
                        FUN_14001B5AC(local_210, (uint32_t)seed, pos + 2);
                    }
                    FUN_1400702B0(&local_150, WSTR("**mem"));
                    DVar13 = PECMD_RunScriptText(script, local_210, line, local_150,
                                                 ((uint64_t)seed << 16) | (uint32_t)flags | 0x40,
                                                 NULL, NULL);
                    local_210 = NULL; /* 所有权已移交内层(其退出时释放), 防外层重复释放 (T1e) */
                    PECMD_FreeStrBuf(&local_150);
                }
                PECMD_FreeStrBuf(&local_210);
            }
            PECMD_FreeStrBuf(&local_1b8);
            branches |= 2;
            goto srx_tail;
        }

        if (q1 != 0) {
            /* ---- 裸路径逐盘符探测（dc:30019-30057）----
             * 词首单个反斜杠: 对每个存在的盘符执行 drive+词。
             * 原文经 PECMD_CrtShim(FUN_14001708c, SKIP(CRT) 空桩, fmt "%s%s"
             * @0x140121598) 拼接; 此处以直接拼接达成同义结果。
             * cmd = drive + 词全文; a3 = drive + 词去首字符(去引号已先行)。 */
            WCHAR drvList[512];
            DWORD nDrv = 0;
            WCHAR *drv;
            WCHAR *cand = NULL;
            WCHAR *argLine = NULL;
            int64_t drvIdx = 0;
            WCHAR *wsrc;
            if (wcls != NULL) {
                unq = PECMD_UnquoteTokenInPlace(wcls); /* dc:30063 去引号 (词首 '\') */
                wsrc = unq;
            }
            else {
                wsrc = NULL;
            }
            nDrv = GetLogicalDriveStringsW(512, drvList);
            drv = drvList;
            while (wsrc != NULL && nDrv >= 2 && *drv != L'\0') {
                if (driveChar <= *drv && drvIdx < 0x5dc - lVar32) { /* dc:30040 过滤+上限 */
                    cand = NULL;
                    argLine = NULL;
                    FUN_14006375C(&cand, drv);
                    FUN_14006375C(&cand, wsrc);
                    if (FUN_140101E70(cand)) { /* dc:30044 文件存在 */
                        FUN_14006375C(&argLine, drv);
                        FUN_14006375C(&argLine, wsrc + 1); /* dc:30046 词去首字符 */
                        DVar13 = (int64_t)srx_ExecuteScriptFile(script, cand, argLine,
                                                                (uint32_t)(flags | flags2),
                                                                NULL, outbuf);
                        argLine = NULL;
                        if (m_flag == 0) /* dc:30049 m 前缀置位才继续遍历 */ {
                            PECMD_FreeStrBuf(&cand);
                            break;
                        }
                    }
                    PECMD_FreeStrBuf(&cand);
                    PECMD_FreeStrBuf(&argLine);
                }
                drvIdx += (int64_t)lstrlenW(drv) + 1;
                drv += lstrlenW(drv) + 1;
            }
            branches |= 16;
            goto srx_tail;
        }

        if (ptHash == NULL) {
            /* ---- 裸路径 → 执行脚本文件（dc:30060-30066, S10 补齐）---- */
            if (wcls != NULL) {
                unq = PECMD_UnquoteTokenInPlace(wcls); /* dc:30063 */
            }
            if (unq == NULL) {
                unq = &emptyW; /* 空命令行兜底: 打开失败路径, 不崩溃 */
            }
            DVar13 = srx_ExecuteScriptFile(script, unq, line, (uint32_t)(flags | flags2), NULL,
                                           outbuf);
            branches |= 4;
            goto srx_tail;
        }

        /* ---- 资源脚本执行路径（dc:30068-30233, 既有代码按 '#' 分流适配）---- */
        {
            /* 资源引用形如 [Dll|路径]#[资源名|#id]: 在 '#' 处截断得 DLL 名 */
            uint32_t resFlags = 0;
            memset(sub, 0, sizeof(sub));
            *ptHash = L'\0';                         /* dc:30123-30124 (执行后恢复) */
            hmod = NULL;
            if (*wcls != L'\0') {
                hmod = LoadLibraryExW(wcls, (HANDLE)0, 2); /* dc:30126-30129 DLL 名=词首部 */
                if (hmod == INVALID_HANDLE_VALUE)
                    hmod = NULL;
            }
            /* else TODO(verify): 原文以 g_hInstance(DAT_14013cf70) 装载当前 EXE
             * 内 SCRIPT 资源; 工程未还原该全局, 此处置 NULL 跳资源块。 */
            *ptHash = L'#';                        /* dc:30131 恢复 */
            {
                WCHAR *rname = NULL;
                uint16_t uSeed = 0;
                uint64_t kf = 0;
                int64_t len = 0;
                FUN_1400702B0(&rname, ptHash + 1); /* 资源名 = '#' 后 */
                if (hmod != 0) {
                    uSeed = PECMD_GenRandomSeed16();
                    PECMD_AllocWStringBuffer(&local_1e8, 0);
                    FUN_14001EA18(hmod, rname, WSTR("PECMD"), (void **)&local_1e8, &resFlags);
                    if (local_1e8 != NULL && *local_1e8 != L'\0') {
                        /* XOR 解码 + 脚本对象构造 */
                        len = (int64_t)lstrlenW(local_1e8);
                        kf = (uint64_t)(uint16_t)uSeed;
                        kf = (kf << 16) | (kf & 0xff);
                        FUN_14001B5AC(local_1e8, (uint32_t)((uint16_t)uSeed ^ (uint16_t)uSeed),
                                     len);
                        FUN_140017CDC(sub, script);
                        FUN_1400186BC(sub, script);
                        PECMD_InitObfuscatedKeywords(sub, (uint64_t)uSeed);
                        if (PECMD_InvokeSubRoutine(&local_1e8, sub, kf) == 0) {
                            /* TODO(verify): 反编译 707-721 的详细 flags 组合 */
                            DVar13 = PECMD_RunScriptText(script, local_1e8, line, local_1f8,
                                                         ((uint64_t)uSeed << 16) | kf | 0x40,
                                                         local_240, NULL);
                            local_1e8 = NULL; /* 所有权已移交内层(其退出时释放) */
                        }
                        PECMD_FreeStrBuf(&local_1e8);
                    }
                    FreeLibrary(hmod);
                }
                PECMD_FreeStrBuf(&rname);
            }
            branches |= 8;
            goto srx_tail;
        }

    srx_tail:
        /* ---- 收尾（dc:30235-30349 等价物）---- */
        (void)branches;
        if (g_sysinitState == 3) {
            PECMD_CheckFirstStartupFlag(script);
        }
        if (b_sysinit) {
            g_sysinitState |= 1;
            PECMD_RunSysInit(script, WSTR("sysinit_end"));
        }
        FUN_14009BB28(script, 0);
        PECMD_FreeStrBuf(&local_1f8);
        PECMD_FreeStrBuf(&local_240);
        PECMD_FreeStrBuf(&local_260);
        PECMD_FreeStrBuf(&local_278);
        PECMD_FreeStrBuf(&local_250);
        PECMD_FreeStrBuf(&outbuf);
        PECMD_FreeStrBuf(&wcls);
        PECMD_FreeStrBuf(&line);
        (void)flags;
        (void)flags2;
        (void)m_flag;
        (void)uVar24;
        (void)uVar31;
        (void)uVar34;
        (void)uVar35;
        (void)uVar42;
        (void)local_1a0;
        (void)local_1d0;
        (void)local_160;
        (void)local_228;
        (void)local_1f0;
        (void)local_234;
        (void)local_26c;
        (void)lVar12;
        (void)firstCh;
        (void)driveChar;
        return (int)DVar13;
    }
}