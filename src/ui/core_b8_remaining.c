/* ====================================================================
 * core_b8_remaining.c — B8 大函数还原
 *
 * 来源: PECMD原始.EXE (x64, ImageBase=0x140000000)
 *   PECMD_RamdMountImDisk       @0x1400e9724  (忠实移植, decompiled.c @143323)
 *   PECMD_SockCreateIpcObj      @0x1400e1228  (简化桩)
 *   PECMD_ListMoveItemBlock     @0x1400f36ec  (简化桩)
 *   FUN_1400F6DB0               @0x1400f6db0  (简化桩)
 *   PECMD_ListHostMsgDispatch   @0x1400f94a0  (简化桩)
 * ==================================================================== */
#include <stdint.h>
#include <string.h>

#include "pecmd_defs.h"
/* ========== PECMD_SockCreateIpcObj @0x1400e1228 ==========
 * [简化桩] 原函数为 8K 大函数，按签名返回 0。
 * TODO(verify): 需完整还原脚本执行逻辑。
 */
LARGE_INTEGER PECMD_SockCreateIpcObj(LARGE_INTEGER script, WCHAR *cmd, WPARAM wParam)
{
    LARGE_INTEGER r;
    (void)script;
    (void)cmd;
    (void)wParam;
    r.QuadPart = 0;
    return r;
}

/* ====================================================================
 * PECMD_RamdMountImDisk @0x1400e9724 忠实移植所需依赖
 * (签名与各定义处一致; 定义分布 core_string.c / core_var*.c /
 *  core_b1_remaining.c / core_b2f.c / core_b3*.c / link_stubs.c 等)
 * ==================================================================== */
extern void PECMD_StrDupAssign(uint16_t **ps, const uint16_t *src);              /* @0x1400702b0 */
extern long long *PECMD_SkipLeadingControlChars(long long *ps);                  /* @0x14005b154 */
extern long long PECMD_TokPrefixICmp(char *a, uint16_t *w, int n);               /* @0x14005c72c */
extern void PECMD_LoadUnloadImdisk(uint64_t p1, int p2);                         /* @0x14001c45c */
extern int PECMD_IsImDiskMountPointAvailable(void);                              /* @0x14001c44c */
extern char PECMD_MatchTokenAdvance(char *tok, void *pp, int n);                 /* @0x1400660ac */
extern void PECMD_AllocWStringBuffer(WCHAR **ps, int64_t count);                 /* @0x140063694 */
extern void PECMD_AllocStrSlot(WCHAR **ps);                                      /* @0x140063620 */
extern WCHAR *PECMD_StrCopyW(WCHAR **ps, LPCWSTR src, int64_t len);              /* @0x140063888 */
extern WCHAR *PECMD_AllocString(WCHAR **ps, int64_t count);                      /* @0x140063720 */
extern WCHAR *PECMD_StrDupA(WCHAR **ps, LPCWSTR src, int64_t a, int64_t b);      /* @0x1400637dc */
extern long long *PECMD_AppendWideStr(void *ps, LPCWSTR src);                    /* @0x14006375c */
extern void PECMD_SetVariable(void *script, const WCHAR *key, const WCHAR *val); /* @0x1400629b8 */
extern uint64_t PECMD_ParseImdiskMountArgs(int argc, int64_t *argv,
                                           int64_t *out); /* @0x1400369d0 */
extern void PECMD_FreeStrBuf(void *ps);                   /* @0x14005b104 */
extern uint64_t PECMD_WaitHandlesOrMessages(uint64_t script, int64_t ms, int f3,
                                            uint64_t *p4);                      /* @0x1400195f0 */
extern int64_t PECMD_RunCommand(void *script, WCHAR *cmdline);                  /* @0x140031454 */
extern void PECMD_SplitTokenTrimWs(WCHAR **src, WCHAR **dst, int16_t delim);    /* @0x1400675b8 */
extern int PECMD_ParseSignedNumberStr(uint64_t *pp, int64_t *out, int16_t sep); /* @0x1400678f0 */
extern int64_t *PECMD_AssignString(int64_t *ps, const uint16_t *src);           /* @0x14007034c */
extern uint64_t PECMD_IsDirectory(LPCWSTR path);                                /* @0x140101d34 */
extern void PECMD_FormatI64Dec(LPWSTR dst, int64_t value);                      /* @0x1400e6d68 */
extern uint64_t PECMD_EvalParenExpression(int64_t *pp, uint64_t *out);          /* @0x14007474c */
extern uint64_t PECMD_QueryPhysicalMemory(int mode);                            /* @0x14001e870 */
extern void PECMD_FormatU64Dec(WCHAR *dst, uint64_t v);                         /* @0x1400e6d74 */
extern uint64_t PECMD_AsciiWideICmp(const char *a, const uint16_t *w);          /* @0x14005c7c4 */
extern uint64_t PECMD_SkipWCharUntil(void *pp, uint16_t ch);                    /* @0x1400f429c */
extern uint64_t PECMD_ParseSizeAndSkipWs(int64_t *pp, uint64_t *out);           /* @0x14006a7f4 */
extern int PECMD_ParseUIntValue(WCHAR **pp, int *out);                          /* @0x140074838 */
extern uint64_t PECMD_NextRandomSeed(void);                                     /* @0x14005e04c */
extern uint8_t *PECMD_MemMoveForward(uint8_t *dst, uint8_t *src, int n);        /* @0x14001d78c */
extern LARGE_INTEGER PECMD_ProcessScriptBlock(LARGE_INTEGER p1, LARGE_INTEGER p2, long long *p3,
                                              long long *p4, char *p5);         /* @0x14004c0bc */
extern uint64_t PECMD_MatchWordAndPad(int64_t word, int64_t *pp, uint32_t len); /* @0x14005c6b8 */
extern char PECMD_MatchAndPad(int64_t key, int64_t *pp, int len);               /* @0x140066148 */
extern int PECMD_AsciiPrefixICmp(const char *s, const uint16_t *w, int n);      /* @0x14005c788 */
extern uint32_t PECMD_ArgTokenize(long long *state, LPCWSTR cmd, int mode);     /* @0x140073ccc */
extern uint64_t PECMD_ExecCmdDispatch(uint64_t a, uint64_t b, uint64_t c, uint64_t d, uint64_t e,
                                      uint64_t f, uint64_t g, uint64_t h); /* @0x14000e26c */
extern int64_t PECMD_CreateWindow(int64_t *script, WCHAR *name, int64_t a3, LPCWSTR a4,
                                  int a5);                    /* @0x140081fa0 */
extern void PECMD_StrBldCopyWide(void *ps, const WCHAR *src); /* @0x1400703e4 */
extern void PECMD_TlsLogWrite(uint64_t ctx, const uint16_t *fmt, uint64_t a,
                              uint64_t b);     /* @0x140018d8c */
extern uint64_t FUN_140063060(uint64_t value); /* 恒等函数 (core_b3_remaining.c) */

/* ---- 本文件新增全局槽 (此前全库无定义) ----
 * DAT_14013e240 @0x14013e240: RAMD 安装互斥计数; 纯运行时槽 → 初值 0。
 * DAT_14013a854 @0x14013a854: ImDisk 版本状态; .data 初值 0xFFFFFFC0(-64),
 *   pe_registry.json 真值 "c0 ff ff ff ..." 与体内 0xffffffc0 写回一致。 */
long long DAT_14013e240 = 0;
int DAT_14013a854 = -64;

/* ---- .rdata 宽字面量真值 (tools/pe_data_extract.py 提取) ---- */
static const uint16_t ramd_lit_dash_l[] = {'-', 'l', 0};   /* @0x14012b960 = L"-l"  */
static const uint16_t ramd_lit_dash_m[] = {'-', 'm', 0};   /* @0x14012b618 = L"-m"  */
static const uint16_t ramd_lit_starstar[] = {'*', '*', 0}; /* @0x14012b9d8 = L"**"  */
static const uint16_t ramd_wlit_dash_b[] = {'-', 'b', 0};  /* @0x14012b868 = L"-b"  */
static const uint16_t ramd_wlit_dash_s[] = {'-', 's', 0};  /* @0x14012b860 = L"-s"  */

#define RAMD_FREE_ALL()                                                                            \
    do {                                                                                           \
        PECMD_FreeStrBuf(&local_378);                                                              \
        PECMD_FreeStrBuf(&local_350);                                                              \
        PECMD_FreeStrBuf(&local_3c8);                                                              \
        PECMD_FreeStrBuf(&local_370);                                                              \
        PECMD_FreeStrBuf(&local_3d0);                                                              \
        PECMD_FreeStrBuf(&local_348);                                                              \
        PECMD_FreeStrBuf(&local_380);                                                              \
        PECMD_FreeStrBuf(&local_3e0);                                                              \
        PECMD_FreeStrBuf(&local_3b0);                                                              \
        PECMD_FreeStrBuf(&local_390);                                                              \
        PECMD_FreeStrBuf(&local_3a0);                                                              \
        PECMD_FreeStrBuf(&local_3f0);                                                              \
        PECMD_FreeStrBuf(&local_2f0);                                                              \
        PECMD_FreeStrBuf(&local_3c0);                                                              \
        PECMD_FreeStrBuf(&local_368);                                                              \
    } while (0)

/* ========== PECMD_RamdMountImDisk @0x1400e9724 ==========
 * RAMD 命令巨函数忠实移植 (decompiled.c @143323, size=9737)。
 * 解析 RAMD 参数并构造 imdisk 命令行完成 RAM 盘挂载:
 *   - 设备为 '*': 子选项(-mem/-m/-ex/-map/-exe/-nexe) + '*'@规格(尺寸/偏移/盘符...)
 *   - 显式设备名: ',' 分隔字段; 走 imdisk 直挂或 proxy/devio 共享内存路径
 *   - "--free": 自旋等待其它 RAMD 安装完成后卸载 imdisk.cpl
 * 转换规范见 tools/AGENT_HANDOFF.md §3。
 */
uint64_t PECMD_RamdMountImDisk(LPCWSTR cmd, int64_t *out)
{
    /* ---- Ghidra 局部变量 (保留原名; bool→int 承载) ---- */
    WCHAR WVar1;
    WCHAR WVar2;
    WCHAR WVar3;
    uint64_t uVar22;
    uint64_t uVar35;
    uint16_t uVar11;
    char cVar10;
    int bVar9;
    int bVar31;
    int bVar39;
    int iVar12;
    int iVar13;
    int iVar14;
    DWORD DVar15;
    DWORD DVar16;
    uint32_t uVar17;
    long long lVar18;
    long long lVar19;
    long long lVar20;
    long long lVar21;
    HANDLE pvVar23;
    LPWSTR pWVar24;
    LPWSTR pWVar26;
    LPWSTR pWVar29;
    LPWSTR pWVar32;
    LPWSTR pWVar33;
    LPWSTR pWVar34;
    LPWSTR pWVar8;
    LPCWSTR pwVar27;
    LPCWSTR pwVar37;
    uint64_t *puVar28;
    uint64_t uVar4;
    uint64_t uVar5;
    uint64_t uVar6;
    uint64_t uVar7;
    uint64_t DVar25;
    uint32_t errCode; /* _Var30: _FILETIME 载体, 仅 dwLowDateTime 有意义 */
    uint32_t res38;   /* _Var38 */

    /* ---- 字符串游标 ---- */
    WCHAR *local_260; /* param_1 副本缓冲槽 */
    WCHAR *local_3e8;
    WCHAR *local_3f8;
    WCHAR *local_300;
    WCHAR *local_3b8;
    WCHAR *local_218;
    WCHAR *local_398;
    WCHAR *local_328;
    WCHAR *local_388;
    WCHAR *local_308;
    WCHAR *local_360;
    WCHAR *local_2f8;
    WCHAR *local_3a8;

    /* ---- 动态字符串槽 (FUN_140063620 族分配) ---- */
    WCHAR *local_368;
    WCHAR *local_3c0;
    WCHAR *local_2f0;
    WCHAR *local_3f0;
    WCHAR *local_3a0;
    WCHAR *local_390;
    WCHAR *local_3b0;
    WCHAR *local_3e0;
    WCHAR *local_380;
    WCHAR *local_348;
    WCHAR *local_3d0;
    WCHAR *local_370;
    WCHAR *local_3c8;
    WCHAR *local_378;
    WCHAR *local_350;
    WCHAR *local_338; /* (pthreadmbcinfo 实为串槽) */
    WCHAR *local_268;
    WCHAR *local_258;
    WCHAR *local_298;
    WCHAR *local_2e0; /* (LARGE_INTEGER 实为串槽) */
    WCHAR *local_2a8; /* (_FILETIME 实为串槽) */
    WCHAR *local_2e8;
    WCHAR *local_2d0;
    WCHAR *local_288;
    WCHAR *local_210;
    WCHAR *local_220;
    WCHAR *lpString;
    const WCHAR *local_2b8; /* &DAT_14011c638 / &DAT_14012b9d8 */
    WCHAR *local_230;       /* "-m" 挂载点参数位置 */
    long long local_208;

    /* ---- 数值/标志局部 ---- */
    uint64_t local_290;
    uint64_t local_270;
    uint64_t local_238;
    uint64_t local_2c0;
    uint64_t local_2b0;
    uint64_t local_278;
    uint64_t local_2a0;
    uint64_t local_240;
    uint64_t local_250;
    uint64_t local_318;
    int64_t local_228;
    int local_320;
    int local_2d8;
    int local_3d8;
    int local_2d4;
    uint32_t local_330;
    uint32_t local_340 = 0; /* 首入为不定值(Ghidra), 保守置 0 */
    uint32_t savedErr;      /* local_358 */
    uint64_t local_280;
    uint64_t local_248;
    WCHAR local_res20;
    uint8_t local_res18[8]; /* FUN_14001c45c 不透明槽 */

    /* ---- ArgTokenize 状态块 (基址=&f00 ≡ &local_188; +0x08..+0xF8 为清零区) ---- */
    struct {
        long long f00;    /* local_188 (+0x00) */
        uint64_t mid[11]; /* local_180[11] (+0x08, FUN_140102a90 清零起点) */
        int count;        /* local_128 (+0x60) */
        uint32_t _pad60;
        long long *argv; /* local_120 (+0x68) */
        long long f70;   /* local_118 (+0x70) */
        uint8_t _pad78[0x88];
        WCHAR tail[36]; /* local_88 (+0x100) */
    } tk;

    WCHAR local_1c8[32];
    WCHAR numbuf[3]; /* local_2c8/local_2c6/local_2c4 */
    WCHAR local_310[4];

    (void)local_220;
    (void)uVar35;

    tk.f00 = 0;
    memset(tk.mid, 0, 0xf0); /* FUN_140102a90 即 memset 内联 */

LAB_1400e9774:
    bVar39 = 0;
    local_340 = local_340 & 0xffffff00u;
    PECMD_StrDupAssign(&local_260, cmd);
    local_3e8 = local_260;
    local_3f8 = local_260 + 6;
    for (local_3b8 = local_3f8;; local_3b8 = local_3b8 + 1) {
        WVar1 = *local_3b8;
        if (!(WVar1 != L'\0' &&
              ((((uint16_t)WVar1 < 9 || (0xd < (uint16_t)WVar1)) && (WVar1 != L' ')))))
            break;
    }
    local_300 = local_3b8;
    PECMD_SkipLeadingControlChars((long long *)&local_300);
    WVar1 = *local_3f8;
    FUN_140063060((uint64_t)(uintptr_t)local_res18); /* 恒等函数, 无副作用 */
    lVar18 = PECMD_TokPrefixICmp("--free", local_300, 6);
    if ((char)lVar18 != '\0')
        goto LAB_1400ebcdd;
    EnterCriticalSection(g_csInit);
    DAT_14013e240 = DAT_14013e240 + 1;
    LeaveCriticalSection(g_csInit);
    PECMD_LoadUnloadImdisk((uint64_t)(uintptr_t)local_res18, 0);
    bVar9 = PECMD_IsImDiskMountPointAvailable();
    local_320 = 0;
    local_2d8 = 0;
    local_330 = (uint32_t)(bVar9 == 0); /* CONCAT71(extraout_var,bVar9)==0 */
    if ((int)DAT_14013a854 < 0) {
        local_248 = 0;
        DAT_14013a854 = 0;
        local_280 = 0;
        if (g_pImDiskGetVersion != NULL) {
            ((void (*)(uint64_t *, uint64_t *))g_pImDiskGetVersion)(&local_280, &local_248);
            if (local_248 != 0) {
                if (local_280 < 0x202) {
                    if (local_280 < 0x184) {
                        if (local_280 < 0x160) {
                            if (local_280 != 0) {
                                DAT_14013a854 = 1;
                            }
                        }
                        else {
                            DAT_14013a854 = 2;
                        }
                    }
                    else {
                        DAT_14013a854 = 8;
                    }
                }
                else {
                    DAT_14013a854 = 0x38;
                }
            }
        }
        if (g_u8CCB1 != '\0') {
            PECMD_TlsLogWrite((uint64_t)(uintptr_t)g_Script,
                              (const uint16_t *)L"DBG: IMD: 0x%X 0x%X 0x%X, 0x%p\r\n",
                              (uint64_t)(local_280 & 0xffffffff),
                              (uint64_t)(local_248 & 0xffffffff));
        }
    }
    lVar18 = PECMD_TokPrefixICmp("-BD", local_300, 3);
    local_3d8 = 0;
    if ((char)lVar18 != '\0') {
        local_3d8 = -0x100000;
    }
    cVar10 = PECMD_MatchTokenAdvance("--ins", &local_300, 5);
    if (cVar10 == '\0') {
        cVar10 = PECMD_MatchTokenAdvance("--fins", &local_300, 6);
        bVar39 = 0;
        if (cVar10 != '\0') {
            bVar39 = 6;
        }
    }
    else {
        bVar39 = 5;
    }
    PECMD_AllocWStringBuffer(&local_368, 1000);
    PECMD_AllocStrSlot(&local_3c0);
    PECMD_AllocStrSlot(&local_2f0);
    PECMD_AllocStrSlot(&local_3f0);
    local_3a0 = (WCHAR *)0;
    PECMD_AllocStrSlot(&local_390);
    PECMD_AllocStrSlot(&local_3b0);
    PECMD_AllocStrSlot(&local_3e0);
    PECMD_AllocStrSlot(&local_380);
    PECMD_AllocStrSlot(&local_348);
    PECMD_AllocStrSlot(&local_3d0);
    PECMD_AllocStrSlot(&local_370);
    PECMD_AllocStrSlot(&local_3c8);
    local_2c0 = 0xffffffffffffffffULL;
    local_2b0 = 0xffffffffffffffffULL;
    res38 = 0; /* _Var38 = {0,0} */
    local_350 = (WCHAR *)0;
    local_318 = 0;
    local_310[0] = L'\0';
    local_2b8 = (const WCHAR *)g_szEmpty; /* &DAT_14011c638 */
    local_230 = (WCHAR *)g_szEmpty;
    local_228 = 0;
    savedErr = 0;                              /* local_358 = {0,0} */
    PECMD_AllocWStringBuffer(&local_378, 200); /* FUN_140063694(&local_378,200) */
    WVar2 = *local_3b8;
    local_218 = local_3b8;
    local_3a8 = local_3b8;
    pWVar33 = local_300;
    local_res20 = WVar2;
    if (WVar1 == L'*') {
        pWVar29 = local_3f8 + 1;
        *local_3b8 = L'\0';
        WVar3 = *pWVar29;
        local_3f8 = pWVar29;
        while ((WVar3 != L'\0' && (WVar3 != L'*'))) {
            local_3f8 = local_3f8 + 1;
            WVar3 = *local_3f8;
        }
        PECMD_StrCopyW(&local_3c0, pWVar29, (int64_t)(local_3f8 - pWVar29));
        if (*local_3c0 != L'\0') {
            PECMD_AllocString(&local_368, 1000);
        }
        local_3a8 = local_3b8;
        if (WVar2 != L'\0') {
            local_3a8 = local_3b8 + 1;
        }
        PECMD_SkipLeadingControlChars((long long *)&local_3a8);
        /* 原 Ghidra while 级联 ≡ 五选一平铺循环: 反复消费 -mem/-m/-ex/-map/-exe/-nexe */
        for (;;) {
            while (
                ((cVar10 = PECMD_MatchAndPad((int64_t)(uintptr_t)"-mem", (int64_t *)&local_3a8, 4)),
                 cVar10 != '\0') ||
                ((cVar10 = PECMD_MatchAndPad((int64_t)(uintptr_t)"-m", (int64_t *)&local_3a8, 2)),
                 cVar10 != '\0')) {
                local_2b8 = (const WCHAR *)g_szEmpty;
            }
            cVar10 = PECMD_MatchAndPad((int64_t)(uintptr_t)"-ex", (int64_t *)&local_3a8, 3);
            if (cVar10 != '\0') {
                local_2b8 = (const WCHAR *)ramd_lit_starstar; /* &DAT_14012b9d8 */
                continue;
            }
            cVar10 = PECMD_MatchAndPad((int64_t)(uintptr_t)"-map", (int64_t *)&local_3a8, 4);
            if (cVar10 != '\0') {
                bVar39 = 1;
                continue;
            }
            cVar10 = PECMD_MatchAndPad((int64_t)(uintptr_t)"-exe", (int64_t *)&local_3a8, 4);
            if (cVar10 != '\0') {
                local_3d8 = 1;
                continue;
            }
            cVar10 = PECMD_MatchAndPad((int64_t)(uintptr_t)"-nexe", (int64_t *)&local_3a8, 5);
            if (cVar10 != '\0') {
                local_3d8 = -0x100000;
                continue;
            }
            break;
        }
        pWVar33 = local_3a8;
    }
    PECMD_SetVariable(out, (const uint16_t *)L"ERRORLEVEL", (const uint16_t *)L"inline");
    uVar35 = 0xffffffffffffffffULL;
    if (*local_3c0 == L'\0') {
        PECMD_StrDupA(&local_3a0, (LPCWSTR) " =*Imdisk ", -1, -1);
    }
    else {
        PECMD_StrDupA(&local_3a0, (LPCWSTR) "*- ", -1, -1);
        PECMD_AppendWideStr(&local_3a0, local_3c0);
        PECMD_AppendWideStr(&local_3a0, (const uint16_t *)L"=*Imdisk ");
    }
    lVar18 = PECMD_TokPrefixICmp("--version", pWVar33, 9);
    lVar19 = PECMD_TokPrefixICmp("-l", pWVar33, 2);
    lVar20 = PECMD_TokPrefixICmp("-D", pWVar33, 2);
    lVar21 = PECMD_TokPrefixICmp("-BD", pWVar33, 3);
    uVar17 = local_330;
    bVar9 = 0;
    if ((((((char)lVar18 == '\0') && ((char)lVar19 == '\0')) && ((char)lVar20 == '\0')) &&
         ((char)lVar21 == '\0'))) {
        bVar9 = 1;
    }
    if (((((local_330 != 0) || ((int)DAT_14013a854 < 1)) || ((int)DAT_14013a854 < 2)) &&
         ((bVar9 || (bVar39 != 0)))) ||
        (5 < bVar39)) {
        pWVar29 = (LPWSTR)L"0 ";
        if (local_330 == 0) {
            const WCHAR *probeArgv[2];
            PECMD_AllocStrSlot(&local_268);
            probeArgv[0] = (const WCHAR *)ramd_lit_dash_l; /* &DAT_14012b960 = L"-l" */
            probeArgv[1] = (const WCHAR *)ramd_lit_dash_l;
            PECMD_ParseImdiskMountArgs(2, (int64_t *)probeArgv, (int64_t *)&local_268);
            if ((*local_268 != 0) && (pWVar29 = (LPWSTR)L"1 ", local_268[1] == 0)) {
                pWVar29 = (LPWSTR)L"0 ";
            }
            PECMD_FreeStrBuf(&local_268);
        }
        EnterCriticalSection(g_csInit);
        DAT_14013e240 = DAT_14013e240 + -1;
        LeaveCriticalSection(g_csInit);
        EnterCriticalSection(g_csInit);
        while (0 < DAT_14013e240) {
            LeaveCriticalSection(g_csInit);
            uVar35 = 0;
            PECMD_WaitHandlesOrMessages((uint64_t)(uintptr_t)out, 1, 0, (uint64_t *)0);
            EnterCriticalSection(g_csInit);
            res38 = savedErr; /* _Var38 = local_358 */
        }
        PECMD_LoadUnloadImdisk((uint64_t)(uintptr_t)local_res18, -1);
        LeaveCriticalSection(g_csInit);
        numbuf[0] = (uint16_t)((char)bVar39 + L'0');
        numbuf[1] = (uint16_t)0x20;
        numbuf[2] = L'\0';
        PECMD_LoadUnloadImdisk((uint64_t)(uintptr_t)local_res18, -1);
        PECMD_StrDupAssign(&local_338, (const uint16_t *)L"#28:INDATA ");
        pwVar37 = (LPCWSTR)L"0 ";
        if (uVar17 != 0) {
            pwVar37 = (LPCWSTR)L"1 ";
        }
        PECMD_AppendWideStr(&local_338, pwVar37);
        PECMD_AppendWideStr(&local_338, pWVar29);
        PECMD_AppendWideStr(&local_338, numbuf);
        PECMD_AppendWideStr(&local_338, local_3b8);
        if (g_u8CCB1 != '\0') {
            PECMD_TlsLogWrite((uint64_t)(uintptr_t)g_Script,
                              (const uint16_t *)L"Install IMD[%s]\r\n",
                              (uint64_t)(uintptr_t)local_338, uVar35);
        }
        PECMD_RunCommand(out, local_338);
        EnterCriticalSection(g_csInit);
        DAT_14013e240 = DAT_14013e240 + 1;
        DAT_14013a854 = -64; /* 0xffffffc0 */
        LeaveCriticalSection(g_csInit);
        PECMD_LoadUnloadImdisk((uint64_t)(uintptr_t)local_res18, 0);
        bVar9 = PECMD_IsImDiskMountPointAvailable();
        uVar17 = (uint32_t)(bVar9 == 0);
        local_330 = uVar17;
        EnterCriticalSection(g_csInit);
        if (uVar17 == 0) {
            DAT_14013a854 = 0x38;
        }
        LeaveCriticalSection(g_csInit);
        PECMD_FreeStrBuf(&local_338);
    }
    if (WVar1 != L'*') {
        bVar9 = 0;
        iVar12 = local_3d8;
        cVar10 = PECMD_MatchTokenAdvance("-exe", &local_3b8, 4);
        if (cVar10 != '\0') {
            iVar12 = 1;
        }
        cVar10 = PECMD_MatchTokenAdvance("-nexe", &local_3b8, 5);
        if (cVar10 != '\0') {
            iVar12 = -0x100000;
        }
        local_3d8 = iVar12;
        cVar10 = PECMD_MatchTokenAdvance("-nexe1", &local_3b8, 6);
        if (cVar10 != '\0') {
            local_3d8 = -0x100000;
            local_2d8 = 1;
        }
        PECMD_SplitTokenTrimWs(&local_3e8, &local_390, 0x2c);
        *local_390 = L'\0';
        if (*local_3e8 != L'\0') {
            local_3e8 = local_3e8 + 1;
            PECMD_SplitTokenTrimWs(&local_3e8, &local_390, 0x2c);
            if (*local_3e8 != L'\0') {
                local_3e8 = local_3e8 + 1;
                PECMD_SplitTokenTrimWs(&local_3e8, &local_3b0, 0x2c);
                if (*local_3e8 != L'\0') {
                    local_3e8 = local_3e8 + 1;
                    PECMD_SplitTokenTrimWs(&local_3e8, &local_3e0, 0x2c);
                    if (*local_3e8 != L'\0') {
                        local_3e8 = local_3e8 + 1;
                        PECMD_SplitTokenTrimWs(&local_3e8, &local_380, 0x2c);
                        if (*local_3e8 != L'\0') {
                            local_3e8 = local_3e8 + 1;
                            PECMD_SplitTokenTrimWs(&local_3e8, &local_348, 0x2c);
                            if (*local_3e8 != L'\0') {
                                local_3e8 = local_3e8 + 1;
                                PECMD_ParseSignedNumberStr((uint64_t *)&local_3e8, &local_228,
                                                           0x2c);
                            }
                        }
                    }
                }
            }
        }
        PECMD_AllocString(&local_3e0, 6);
        local_3e0[1] = L':';
        local_3e0[2] = L'\0';
        if (*local_3e0 == L'\0') {
        LAB_1400eaff7:
            PECMD_AssignString((int64_t *)&local_3e0, (const uint16_t *)L"#:");
            if (*local_3e0 != L'\0')
                goto LAB_1400eb01d;
            RAMD_FREE_ALL();
            EnterCriticalSection(g_csInit);
        }
        else {
            uVar35 = PECMD_IsDirectory(local_3e0);
            if ((int)uVar35 != 0) {
                const WCHAR *qArgv[4];
                qArgv[0] = (const WCHAR *)L"imdisk";
                qArgv[1] = (const WCHAR *)ramd_lit_dash_l; /* &DAT_14012b960 = L"-l" */
                qArgv[2] = (const WCHAR *)ramd_lit_dash_m; /* &DAT_14012b618 = L"-m" */
                qArgv[3] = local_3e0;
                PECMD_AllocStrSlot(&local_258);
                uVar22 = PECMD_ParseImdiskMountArgs(4, (int64_t *)qArgv, (int64_t *)&local_258);
                bVar9 = ((int)uVar22 == 0);
                PECMD_FreeStrBuf(&local_258);
            }
            if (*local_3e0 == L'\0')
                goto LAB_1400eaff7;
        LAB_1400eb01d:
            PECMD_AllocWStringBuffer(&local_2e8, 0x3c);
            local_290 = 0xffffffffffffffffULL;
            local_270 = 0xffffffffffffffffULL;
            local_238 = 0xffffffffffffffffULL;
            local_360 = local_390;
            uVar22 = 0xffffffffffffffffULL;
            if (*local_390 != 0) {
                do {
                    PECMD_SkipLeadingControlChars((long long *)&local_360);
                    uVar11 = (uint16_t)(*local_360 | 0x20);
                    if (uVar11 == 0x70) { /* 'p': 物理内存百分比 */
                        puVar28 = &local_290;
                    }
                    else if (uVar11 == 0x6c) { /* 'l': MB 值 */
                        puVar28 = &local_270;
                    }
                    else {
                        if (uVar11 != 0x68)
                            break; /* 'h': 上限 MB */
                        puVar28 = &local_238;
                    }
                    local_360 = local_360 + 1;
                    PECMD_EvalParenExpression((int64_t *)&local_360, puVar28);
                } while (*local_360 != 0);
                if (0 < (long long)local_290) {
                    /* SUB168(SEXT816(C)*SEXT816(x),8) = 128 位有符号乘法高 64 位(mulhi);
                     * 整体系编译器魔法除法序列。TODO(verify): 等价除数语义待核对 */
                    __int128 prod;
                    DVar25 = PECMD_QueryPhysicalMemory(1);
                    prod = (__int128)(long long)(DVar25 * local_290) *
                           (__int128)(long long)(uint64_t)0xA3D70A3D70A3D70BULL;
                    lVar18 = (long long)(DVar25 * local_290) + (long long)(uint64_t)(prod >> 64);
                    uVar22 = (uint64_t)(((lVar18 >> 6) - (lVar18 >> 63)) >> 20);
                }
                if ((long long)uVar22 < (long long)local_270) {
                    uVar22 = local_270;
                }
                if (0 < (long long)local_238) {
                    if ((long long)local_238 < (long long)uVar22) {
                        uVar22 = local_238;
                    }
                    if ((long long)uVar22 < 1) {
                        uVar22 = local_238;
                    }
                }
                if ((-1 < (long long)uVar22) && (*local_3e0 != L'\0')) {
                    if ((long long)uVar22 < 1) {
                        uVar22 = 1;
                    }
                    PECMD_FormatU64Dec(local_2e8, uVar22);
                    PECMD_AppendWideStr(&local_2e8, (const uint16_t *)L"M ");
                    pwVar37 = (LPCWSTR)L"imdisk  -a ";
                    if (bVar9) {
                        pwVar37 = (LPCWSTR)L"imdisk  -e ";
                    }
                    PECMD_AssignString((int64_t *)&local_3f0, pwVar37);
                    if ((local_228 & 1) != 0) {
                        PECMD_AppendWideStr(&local_3f0, (const uint16_t *)L" -o awe ");
                    }
                    if (*local_2e8 != L'\0') {
                        PECMD_AppendWideStr(&local_3f0, (const uint16_t *)L"  -s ");
                        PECMD_AppendWideStr(&local_3f0, local_2e8);
                    }
                    if (*local_3e0 != L'\0') {
                        PECMD_AppendWideStr(&local_3f0, (const uint16_t *)L"  -m ");
                        PECMD_AppendWideStr(&local_3f0, local_3e0);
                    }
                    if ((*local_3b0 != L'\0') || (*local_380 != L'\0')) {
                        PECMD_AppendWideStr(&local_3f0, (const uint16_t *)L"  -p \"");
                        if (*local_3b0 != L'\0') {
                            PECMD_AppendWideStr(&local_3f0, (const uint16_t *)L" /FS:");
                            PECMD_AppendWideStr(&local_3f0, local_3b0);
                        }
                        if (*local_380 != L'\0') {
                            PECMD_AppendWideStr(&local_3f0, (const uint16_t *)L" /V:");
                            PECMD_AppendWideStr(&local_3f0, local_380);
                        }
                        uVar35 = PECMD_AsciiWideICmp("NTFS", local_3b0);
                        if ((((char)uVar35 != '\0') && (*local_348 != 0)) && (*local_348 != 0x30)) {
                            PECMD_AppendWideStr(&local_3f0, (const uint16_t *)L" /C");
                        }
                        PECMD_AppendWideStr(&local_3f0, (const uint16_t *)L"  /Q /Y /X \"");
                    }
                    local_3e8 = local_3f0;
                    PECMD_FreeStrBuf(&local_2e8);
                    goto LAB_1400eb33b;
                }
            }
            PECMD_FreeStrBuf(&local_2e8);
            RAMD_FREE_ALL();
            EnterCriticalSection(g_csInit);
        }
        DAT_14013e240 = DAT_14013e240 + -1;
        errCode = 0x80070057u; /* dwHighDateTime = 0 */
        goto LAB_1400ebd01;
    }
    bVar9 = 0;
    WVar1 = *local_3a8;
    local_2f8 = local_3a8;
    while (WVar1 != L'\0') {
        cVar10 = PECMD_MatchTokenAdvance("-m", &local_2f8, 2);
        if (cVar10 != '\0') {
            local_230 = local_2f8;
            break;
        }
        WVar1 = *local_2f8;
        while (((WVar1 != L'\0' && (((uint16_t)WVar1 < 9 || (0xd < (uint16_t)WVar1)))) &&
                (WVar1 != L' '))) {
            local_2f8 = local_2f8 + 1;
            WVar1 = *local_2f8;
        }
        PECMD_SkipLeadingControlChars((long long *)&local_2f8);
        WVar1 = *local_2f8;
    }
    if (*local_3f8 == L'*') {
        local_278 = 0;
        local_2a0 = 0;
        bVar39 = (local_3f8[1] == L'@');
        pWVar33 = local_3f8 + 1;
        if (bVar39) {
            pWVar33 = local_3f8 + 2;
        }
        local_3f8 = pWVar33;
        PECMD_ParseSizeAndSkipWs((int64_t *)&local_3f8, &local_2c0);
        if (*local_3f8 != L'\0') {
            local_3f8 = local_3f8 + 1;
        }
        PECMD_ParseSizeAndSkipWs((int64_t *)&local_3f8, &local_2b0);
        if (*local_3f8 != L'\0') {
            local_3f8 = local_3f8 + 1;
        }
        PECMD_ParseSizeAndSkipWs((int64_t *)&local_3f8, &local_318);
        if (*local_3f8 != L'\0') {
            local_3f8 = local_3f8 + 1;
        }
        uVar22 = local_318 & 1;
        uVar4 = local_318 & 1;
        uVar5 = local_318 & 1;
        uVar6 = local_318 & 1;
        uVar7 = local_318 & 1;
        pWVar33 = local_3f8;
        PECMD_SkipWCharUntil(&local_3f8, 0x2a); /* thunk_FUN_1400f429c */
        PECMD_StrCopyW(&local_3c8, pWVar33, (int64_t)(local_3f8 - pWVar33));
        if (*local_3f8 != L'\0') {
            local_3f8 = local_3f8 + 1;
        }
        pWVar33 = local_3f8;
        PECMD_SkipWCharUntil(&local_3f8, 0x2a);
        PECMD_StrCopyW(&local_3d0, pWVar33, (int64_t)(local_3f8 - pWVar33));
        if (*local_3f8 != L'\0') {
            local_3f8 = local_3f8 + 1;
        }
        PECMD_ParseSizeAndSkipWs((int64_t *)&local_3f8, &local_278);
        if (*local_3f8 != L'\0') {
            local_3f8 = local_3f8 + 1;
        }
        pWVar33 = local_3f8;
        PECMD_SkipWCharUntil(&local_3f8, 0x2a);
        PECMD_StrCopyW(&local_370, pWVar33, (int64_t)(local_3f8 - pWVar33));
        PECMD_ParseSizeAndSkipWs((int64_t *)&local_3f8, &local_2a0);
        if (*local_3f8 != L'\0') {
            local_3f8 = local_3f8 + 1;
        }
        local_240 = local_2c0;
        local_250 = local_2b0;
        if (bVar39) {
            local_250 = local_2b0 + local_2c0;
            local_240 = 0;
        }
        if ((0 < (long long)local_278) && ((long long)local_2a0 < 1)) {
            local_2a0 = 1;
        }
        PECMD_AllocWStringBuffer(&local_2d0, 0x318);
        pWVar8 = local_2d0;
        pWVar29 = local_2d0 + 0x35;
        local_288 = local_2d0;
        local_210 = local_2d0 + 0x6a;
        pWVar26 = local_2d0 + 0x9f;
        *pWVar26 = L'\0';
        *local_210 = L'\0';
        lpString = local_2d0 + 0xd4;
        local_220 = pWVar29; /* 仅赋值, 后续未读 */
        local_2c0 =
            (uint64_t)((long long)(local_2c0 + (((long long)local_2c0 >> 63) & 0x1ffU))) >> 9;
        PECMD_FormatI64Dec(pWVar8, local_2c0);
        local_2b0 =
            (uint64_t)((long long)(local_2b0 + (((long long)local_2b0 >> 63) & 0x1ffU))) >> 9;
        PECMD_FormatI64Dec(pWVar29, local_2b0);
        *pWVar26 = L'\0';
        if (0 < (long long)local_278) {
            PECMD_FormatI64Dec(pWVar26, local_278);
        }
        local_398 = local_3b8 + (int)(local_res20 != L'\0');
        PECMD_SkipLeadingControlChars((long long *)&local_398);
        local_2d4 = 0;
        uVar35 = (uint64_t)(uint32_t)PECMD_AsciiPrefixICmp("--w:", local_398, 4);
        if ((char)uVar35 != '\0') {
            local_398 = local_398 + 4;
            PECMD_ParseUIntValue(&local_398, &local_2d4);
        }
        PECMD_MatchTokenAdvance("-a", &local_398, -1);
        if ((0 < (int)DAT_14013a854) && (local_3d8 == 0)) {
            local_3d8 = -0x100000;
        }
        bVar31 = 0;
        bVar39 = 0;
        pwVar37 = (LPCWSTR)g_szEmpty; /* &DAT_14011c638 */
        WVar1 = *local_398;
        local_328 = local_398;
        while (WVar1 != L'\0') {
            cVar10 = PECMD_MatchTokenAdvance("-o", &local_328, 2);
            if (cVar10 == '\0') {
                if (bVar9) {
                    cVar10 = PECMD_MatchTokenAdvance("ro", &local_328, 2);
                    if (cVar10 == '\0') {
                        cVar10 = PECMD_MatchTokenAdvance("par", &local_328, 3);
                        if (cVar10 == '\0')
                            goto LAB_1400ea531;
                        bVar39 = 1;
                    }
                    else {
                        bVar31 = 1;
                    }
                }
                else {
                LAB_1400ea531:
                    WVar1 = *local_328;
                    while ((WVar1 != L'\0' && ((((uint16_t)WVar1 < 9 || (0xd < (uint16_t)WVar1))) &&
                                               (WVar1 != L' ')))) {
                        local_328 = local_328 + 1;
                        WVar1 = *local_328;
                    }
                    PECMD_SkipLeadingControlChars((long long *)&local_328);
                    bVar9 = 0;
                }
            }
            else {
                bVar9 = 1;
            }
            WVar1 = *local_328;
        }
        if ((DAT_14013a854 & 0x10) != 0) {
            pwVar37 = (LPCWSTR)L"-o par";
            if (bVar39) {
                pwVar37 = (LPCWSTR)g_szEmpty;
            }
        }
        bVar39 = ((DAT_14013a854 & 0x20) == 0);
        if ((((local_318 >> 9) & 1) == 0) && ((DAT_14013a854 & 0x20) == 0)) {
            if ((uVar22 != 0) && (((local_318 >> 8) & 1) != 0)) {
                bVar39 = 1;
            }
        }
        else {
            bVar39 = 0;
        }
        if (((int)DAT_14013a854 < 8) || ((uVar4 != 0 && (bVar39)))) {
            /* —— proxy/devio 共享内存路径 —— */
            if (uVar6 != 0) {
                pvVar23 = CreateFileW(local_3c8, 0xc0000000u, 7, (LPSECURITY_ATTRIBUTES)0, 3,
                                      0xa0000080u, (HANDLE)0);
                if (pvVar23 == INVALID_HANDLE_VALUE) {
                    uVar17 = GetLastError();
                    if (uVar17 == 0) {
                        uVar17 = 1;
                    }
                    errCode = uVar17; /* dwHighDateTime = 0 */
                    PECMD_FreeStrBuf(&local_2d0);
                    goto LAB_1400eb8bc;
                }
                CloseHandle(pvVar23);
            }
            if (0 < (long long)local_2a0) {
                PECMD_FormatI64Dec(local_210, local_2a0);
            }
            pWVar24 = StrRChrW(local_3c8, (LPCWSTR)0, (WCHAR)L'\\');
            pWVar32 = local_3c8;
            if (pWVar24 != (LPWSTR)0) {
                pWVar32 = pWVar24 + 1;
            }
            if (*local_3d0 == L'\0') {
                PECMD_AllocString(&local_3d0, 0x800);
                pWVar24 = local_3d0;
                PECMD_NextRandomSeed(); /* 返回值弃用 */
                /* TODO(verify): Ghidra 仅见寄存器变参, 栈上实参不可见
                 * (%s#%s#%s$%u 四规格 vs 两可见实参) */
                wsprintfW(pWVar24, (const uint16_t *)L"%s#%s#%s$%u", pWVar32, local_288);
            }
            iVar12 = lstrlenW(local_3d0);
            if (0x400 < iVar12) {
                iVar12 = 0x400;
                local_3d0[0x400] = L'\0';
            }
            PECMD_StrBldCopyWide(&local_378, (const WCHAR *)L"Global\\"); /* FUN_1400703e4 */
            PECMD_AppendWideStr(&local_378, local_3d0);
            PECMD_AppendWideStr(&local_378, (const uint16_t *)L"_devio_Ack");
            *lpString = L'\0';
            local_1c8[0] = L'\0';
            if (((local_318 & 0x30) != 0) || ((uVar7 != 0 && ((local_318 & 0x40) == 0)))) {
                PECMD_MemMoveForward((uint8_t *)lpString, (uint8_t *)(const uintptr_t)L"\"-kill:",
                                     0x10);
                GetModuleFileNameW((HMODULE)0, pWVar8 + 0x213, 0x104);
                GetShortPathNameW(pWVar8 + 0x213, pWVar8 + 0xdb, 0x104);
                iVar13 = lstrlenW(lpString);
                WVar1 = *local_230;
                pWVar33 = local_230;
                if (WVar1 == L'\"') {
                    pWVar34 = local_230 + 1;
                    for (; (*pWVar33 != L'\0' && (*pWVar33 != L'\"')); pWVar33 = pWVar33 + 1) {
                    }
                }
                else {
                    for (;;) {
                        pWVar34 = local_230;
                        if (!(WVar1 != L'\0' &&
                              ((((uint16_t)WVar1 < 9 || (0xd < (uint16_t)WVar1))) &&
                               (WVar1 != L' '))))
                            break;
                        WVar1 = pWVar33[1];
                        pWVar33 = pWVar33 + 1;
                    }
                }
                WVar1 = *pWVar33;
                *pWVar33 = L'\0';
                pwVar27 = (LPCWSTR)L" RAMD Imdisk*&V -D -m %s\"";
                if ((local_318 & 0x20) != 0) {
                    pwVar27 = (LPCWSTR)L" LOAD #25 poweroff";
                }
                wsprintfW(lpString + iVar13, pwVar27, pWVar34);
                *pWVar33 = WVar1;
            }
            if (((long long)local_318 >> 1 & 3U) != 0) {
                wsprintfW(local_1c8, (const uint16_t *)L"-nrc:%d",
                          (uint32_t)(((long long)local_318 >> 1) & 3));
            }
            pWVar26 = local_288;
            PECMD_FormatI64Dec(
                local_288, (long long)((((long long)local_240 >> 63) & 0x1ffU) + local_240) >> 9);
            pWVar29 = pWVar26 + 0x35;
            PECMD_FormatI64Dec(
                pWVar29, (long long)((((long long)local_250 >> 63) & 0x1ffU) + local_250) >> 9);
            PECMD_SetVariable(out, (const uint16_t *)L"&&__ImdMapPid", (const uint16_t *)L"0");
            iVar13 = lstrlenW(local_370);
            iVar14 = lstrlenW(local_3c8);
            PECMD_AllocWStringBuffer(&local_2e0, (long long)iVar12 + 0x940 + (long long)iVar13 +
                                                     (long long)iVar14);
            PECMD_StrDupAssign(&local_298, (const uint16_t *)L"Global\\"); /* FUN_1400702b0 */
            PECMD_AppendWideStr(&local_298, local_3d0);
            pvVar23 = OpenFileMappingW(2, 0, local_298);
            if (pvVar23 != (HANDLE)0) {
                CloseHandle(pvVar23);
                PECMD_FreeStrBuf(&local_298);
                PECMD_FreeStrBuf(&local_2e0);
                PECMD_FreeStrBuf(&local_2d0);
                RAMD_FREE_ALL();
                EnterCriticalSection(g_csInit);
                DAT_14013e240 = DAT_14013e240 + -1;
                errCode = 0xb7u;
                goto LAB_1400ebd01;
            }
            PECMD_FreeStrBuf(&local_298);
            /* TODO(verify): EXEC 串 %s 规格多于可见寄存器实参 (栈参不可见) */
            wsprintfW(
                (LPWSTR)local_2e0,
                (const uint16_t *)L"EXEC --pid:&__ImdMapPid  --exe:%s#10|INDATA --nfb "
                                  L"*%%&MyName0%% %s -w:60000 %s %s %s shm:%s  %s %s %s %s %s",
                local_2b8, lpString);
            PECMD_AllocString(&local_3f0, (long long)(iVar12 + 0x800));
            *pWVar26 = L'\0';
            *pWVar29 = L'\0';
            /* TODO(verify): 同上, proxy imdisk 命令行规格/可见实参不齐 */
            wsprintfW(local_3f0, (const uint16_t *)L"imdisk -a -t proxy %s %s %s -o shm -f %s ",
                      (const WCHAR *)g_szEmpty, pwVar37);
            PECMD_AppendWideStr(&local_3f0, local_398);
            local_3e8 = local_3f0;
            pvVar23 = (HANDLE)CreateEventW((LPSECURITY_ATTRIBUTES)0, 0, 0, local_378);
            (void)PECMD_ProcessScriptBlock(
                (LARGE_INTEGER){.QuadPart = (long long)(uintptr_t)out},
                (LARGE_INTEGER){.QuadPart = (long long)(uintptr_t)local_2e0}, (long long *)0,
                (long long *)0, (char *)0);
            DVar15 = WaitForSingleObject(pvVar23, 60000);
            CloseHandle(pvVar23);
            if (DVar15 != 0) {
                errCode = 1;
                PECMD_FreeStrBuf(&local_2e0);
                PECMD_FreeStrBuf(&local_2d0);
                goto LAB_1400eb8bc;
            }
            for (iVar12 = 0x31;;) { /* Sleep(0) 共 0x32 次 */
                Sleep(0);
                iVar12 = iVar12 + -1;
                if (iVar12 < 0)
                    break;
            }
            if (0 < local_2d4) {
                PECMD_WaitHandlesOrMessages((uint64_t)(uintptr_t)out, (long long)local_2d4, 0,
                                            (uint64_t *)0);
            }
            PECMD_FreeStrBuf(&local_2e0);
        }
        else {
            /* —— imdisk 直挂路径 —— */
            pwVar37 = (LPCWSTR)g_szEmpty;
            if (uVar5 == 0) {
                if (!bVar31) {
                    pwVar37 = (LPCWSTR)L"-o ro";
                }
            }
            else {
                local_340 = local_340 & 0xff;
                if ((DAT_14013a854 & 0x20) == 0) {
                    local_340 = 1;
                }
            }
            iVar12 = lstrlenW(local_3c8);
            iVar13 = lstrlenW(local_3b8);
            PECMD_AllocString(&local_3f0, (long long)iVar13 + 0x800 + (long long)iVar12);
            /* TODO(verify): 规格含 -b/-s/-f 数值, 可见实参仅 pwVar37 */
            wsprintfW(local_3f0, (const uint16_t *)L"imdisk -a  %s %s %s  -b %sb -s %sb -f %s ",
                      pwVar37);
            WVar1 = *local_398;
            local_388 = local_398;
            while (WVar1 != L'\0') {
                uVar22 = PECMD_MatchWordAndPad((int64_t)(uintptr_t)ramd_wlit_dash_b,
                                               (int64_t *)&local_388, 2);
                if (((char)uVar22 == '\0') &&
                    (uVar22 = PECMD_MatchWordAndPad((int64_t)(uintptr_t)ramd_wlit_dash_s,
                                                    (int64_t *)&local_388, 2),
                     (char)uVar22 == '\0')) {
                    WVar1 = *local_388;
                    while (
                        ((WVar1 != L'\0' && (((uint16_t)WVar1 < 9 || (0xd < (uint16_t)WVar1)))) &&
                         (WVar1 != L' '))) {
                        local_388 = local_388 + 1;
                        WVar1 = *local_388;
                    }
                }
                else {
                    WVar1 = *local_388;
                    while ((WVar1 != L'\0' && ((((uint16_t)WVar1 < 9 || (0xd < (uint16_t)WVar1))) &&
                                               (WVar1 != L' ')))) {
                        *local_388 = L' ';
                        local_388 = local_388 + 1;
                        WVar1 = *local_388;
                    }
                }
                PECMD_SkipLeadingControlChars((long long *)&local_388);
                WVar1 = *local_388;
            }
            PECMD_AppendWideStr(&local_3f0, local_398);
            local_3e8 = local_3f0;
        }
        bVar39 = 1;
        local_3b8 = local_398;
        *local_398 = L'\0';
        PECMD_FreeStrBuf(&local_2d0);
        res38 = savedErr; /* _Var38 = local_358 */
    }
    *local_218 = local_res20;
LAB_1400eb33b:
    errCode = 0;
    PECMD_SkipLeadingControlChars((long long *)&local_3b8);
    lVar18 = PECMD_TokPrefixICmp("-gui", local_3b8, 4);
    if ((char)lVar18 == '\0') {
        DVar15 = GetTickCount();
        WVar1 = *local_3e8;
        pWVar29 = local_3e8;
        while (((WVar1 != L'\0' && (((uint16_t)WVar1 < 9 || (0xd < (uint16_t)WVar1)))) &&
                (WVar1 != L' '))) {
            pWVar29 = pWVar29 + 1;
            WVar1 = *pWVar29;
        }
        PECMD_StrBldCopyWide(&local_350, pWVar29); /* FUN_1400703e4 */
        pWVar29 = local_350;
        WVar1 = *local_350;
        local_308 = local_350;
        iVar12 = local_3d8;
        uVar17 = local_330;
        iVar13 = local_2d8;
        while (WVar1 != L'\0') { /* 原体逗号自赋值为寄存器搬运, 此处语义为空 */
            cVar10 = PECMD_MatchAndPad((int64_t)(uintptr_t)"--nbrd", (int64_t *)&local_308, 6);
            if (((cVar10 == '\0') && (cVar10 = PECMD_MatchAndPad((int64_t)(uintptr_t)"--hide",
                                                                 (int64_t *)&local_308, 6),
                                      cVar10 == '\0')) &&
                (cVar10 = PECMD_MatchAndPad((int64_t)(uintptr_t)"-q", (int64_t *)&local_308, 2),
                 cVar10 == '\0')) {
                WVar1 = *local_308;
                while (((WVar1 != L'\0' && (((uint16_t)WVar1 < 9 || (0xd < (uint16_t)WVar1)))) &&
                        (WVar1 != L' '))) {
                    local_308 = local_308 + 1;
                    WVar1 = *local_308;
                }
                PECMD_SkipLeadingControlChars((long long *)&local_308);
            }
            WVar1 = *local_308;
        }
        do {
            if ((uVar17 == 0) && (iVar12 < 1)) {
                PECMD_ArgTokenize((long long *)&tk, local_3e8, 0);
                if (tk.count < 2) {
                    errCode = 0x80070057u;
                    goto LAB_1400eb8bc;
                }
                tk.argv[0] = (long long)(uintptr_t)(const uint16_t *)L"imdisk";
                uVar22 =
                    PECMD_ParseImdiskMountArgs(tk.count, (int64_t *)tk.argv, (int64_t *)&local_368);
                res38 = (uint32_t)uVar22; /* _FILETIME 低 32 位 */
                local_320 = 0;
                savedErr = res38;
                if (((('\0' < (char)local_340) && ((int)uVar22 == 3)) && (iVar13 == 0)))
                    goto code_r0x0001400eb551;
            }
            else if (iVar13 == 0) {
                PECMD_StrDupAssign(&local_2a8, local_3a0); /* FUN_1400702b0 */
                PECMD_AppendWideStr(&local_2a8, pWVar29);
                local_320 = 1;
                if (*local_3c0 == L'\0') {
                    local_310[0] = L'\0';
                    uVar22 = PECMD_ExecCmdDispatch(
                        (uint64_t)(uintptr_t)out, (uint64_t)(uintptr_t)local_2a8,
                        (uint64_t)(uintptr_t)out, (uint64_t)(uintptr_t)local_310, 0, 0, 0, 0);
                    uVar22 &= 0xffffffffULL;
                }
                else {
                    uVar22 =
                        PECMD_ExecCmdDispatch((uint64_t)(uintptr_t)out, 0, (uint64_t)(uintptr_t)out,
                                              (uint64_t)(uintptr_t)local_2a8, 0, 0, 0, 0);
                }
                res38 = (uint32_t)uVar22;
                savedErr = res38;
                PECMD_FreeStrBuf(&local_2a8);
            }
            errCode = savedErr;
            if ((!bVar39) || (errCode != 3))
                goto LAB_1400eb8bc;
            DVar16 = GetTickCount();
            errCode = savedErr;
            if (4999 < DVar16 - (DVar15 - 200))
                goto LAB_1400eb8bc;
            PECMD_WaitHandlesOrMessages((uint64_t)(uintptr_t)out, 200, 0, (uint64_t *)0);
        } while (1);
    }
    (void)PECMD_CreateWindow((int64_t *)out, (WCHAR *)(const uintptr_t)L"imdisk.cpl,", 0,
                             (LPCWSTR)g_szEmpty, -100);
    RAMD_FREE_ALL();
    EnterCriticalSection(g_csInit);
    DAT_14013e240 = DAT_14013e240 + -1;
    goto LAB_1400ebd01;
code_r0x0001400eb551:
    RAMD_FREE_ALL();
    EnterCriticalSection(g_csInit);
    DAT_14013e240 = DAT_14013e240 + -1;
    LeaveCriticalSection(g_csInit);
    PECMD_FreeStrBuf(&local_260);
    goto LAB_1400e9774;
LAB_1400eb8bc:
    if (tk.f70 != 0) { /* local_118: argv 缓冲回收 */
        local_208 = tk.f70;
        tk.f70 = 0;
        PECMD_FreeStrBuf(&local_208);
    }
    if ((*local_3c0 != L'\0') && (local_320 == 0)) { /* ERRORLEVEL 变量回写 */
        tk.tail[0] = L'\0';                          /* local_88[0] */
        pWVar29 = tk.tail;
        if (*local_368 == L'*') {
            pWVar29 = local_368 + 1;
        }
        PECMD_SetVariable(out, local_3c0, pWVar29);
    }
    RAMD_FREE_ALL();
    EnterCriticalSection(g_csInit);
    DAT_14013e240 = DAT_14013e240 + -1;
    goto LAB_1400ebd01;
LAB_1400ebcdd:
    for (;;) {
        EnterCriticalSection(g_csInit);
        if (!(0 < DAT_14013e240))
            break;
        LeaveCriticalSection(g_csInit);
        PECMD_WaitHandlesOrMessages((uint64_t)(uintptr_t)out, 1, 0, (uint64_t *)0);
    }
    PECMD_LoadUnloadImdisk((uint64_t)(uintptr_t)local_res18, -1);
    errCode = 0;
LAB_1400ebd01:
    LeaveCriticalSection(g_csInit);
    PECMD_FreeStrBuf(&local_260);
    return (uint64_t)errCode & 0xffffffff;
}
#undef RAMD_FREE_ALL

/* ========== PECMD_ListMoveItemBlock @0x1400f36ec ==========
 * [简化桩] 原函数为列表列布局，先返回。
 * TODO(verify): 需完整还原列宽/滚动映射。
 */
void PECMD_ListMoveItemBlock(int64_t obj, int index, int height)
{
    (void)obj;
    (void)index;
    (void)height;
}

/* ========== FUN_1400F6DB0 @0x1400f6db0 ==========
 * [简化桩] 原函数为列表视图背景绘制，先返回。
 * TODO(verify): 需完整还原绘制逻辑。
 */
void FUN_1400F6DB0(int64_t obj, int64_t paintInfo)
{
    (void)obj;
    (void)paintInfo;
}

/* ========== PECMD_ListHostMsgDispatch @0x1400f94a0 ==========
 * [简化桩] 原函数为列表视图命中测试，返回 {0,0}。
 * TODO(verify): 需完整还原命中测试逻辑。
 */
POINT PECMD_ListHostMsgDispatch(HDC obj, uint32_t msg, POINT *pt, POINT coord)
{
    POINT r;
    (void)obj;
    (void)msg;
    (void)pt;
    (void)coord;
    r.x = 0;
    r.y = 0;
    return r;
}
