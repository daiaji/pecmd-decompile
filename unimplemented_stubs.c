/* unimplemented_stubs.c - B0/P3: FUN_/thunk_/DAT_/PTR_/gap-stub definitions. FOR LINK VERIFICATION ONLY. */
#include "stubs_common.h"
#include <string.h>
#include <stdio.h>

/* ==================== S8 新增 extern（真体所在文件见注释；仅本文件引用） ====================
 * 同址别名依据 tools/rename_map.json 权威映射：
 *   FUN_14007bda8 ≡ PECMD_ExpandVarsRecursive   真体 src/lang/core_execline.c
 *   FUN_14007a224 ≡ PECMD_ExpandCommandLine     真体 src/lang/core_execline.c
 *   FUN_14006e030 ≡ PECMD_CopyPathToken         真体 src/commands/core_b3m.c
 *   FUN_1400702d4 ≡ PECMD_StrBldCopyWideN       真体 src/runtime/core_var2.c */
extern int64_t FUN_14007BDA8(void *script, WCHAR *line, WCHAR **out, int mode, uint8_t opt);
extern int64_t FUN_14007A224(void *script, WCHAR *line, WCHAR **out, int mode, uint8_t opt);
extern void    PECMD_FreeStrBuf(WCHAR **ps);            /* @0x14005b104 core_string.c */
extern void    PECMD_ZeroLenBuf(void *p);               /* @0x14005b0b8 core_thread.c */
extern int64_t FUN_14006E030(int64_t script, int64_t *pp, int64_t *out, int64_t limit); /* @0x14006e030 core_b3m.c */
extern void    PECMD_StrBldCopyWideN(WCHAR **pname, LPCWSTR src, int64_t len); /* @0x1400702d4 core_var2.c */
/* ---- S8 第二批（运行时取证新增的同址别名真体） ---- */
extern void     FUN_1400702B0(WCHAR **ps, LPCWSTR src);                      /* @0x1400702b0 core_string.c */
extern int32_t  FUN_14001B5AC(LPCWSTR buf, uint32_t key, int64_t n);         /* @0x14001b5ac core_var3.c */
extern uint16_t *FUN_14002FD88(int64_t *ctx, WCHAR *text, uint32_t *flags, int64_t *out); /* @0x14002fd88 core_b2e.c */
extern void    *FUN_14003CD0C(int64_t *script, LPCRITICAL_SECTION name);     /* @0x14003cd0c core_b2f.c */
extern int64_t  FUN_14003ED4C(int64_t *script, LPCWSTR cmd);                 /* @0x14003ed4c core_b2f.c */
extern void     FUN_14009BB28(void *script, int flag);                       /* @0x14009bb28 restored_bodies.c */

/* ---- R14 R1 转发批次（仲裁报告 s13_r1_arbitration.md 处方） ---- */
extern WCHAR    *FUN_14000531C(WCHAR *p);                                    /* @0x14000531c 真体 core_proc.c:78 (返回指针版跳空白) */
extern void      FUN_1400E6860(uint64_t *obj, int result);                   /* @0x1400e6860 真体 core_b8f.c:226 */
extern void      FUN_1400633A8(void **ps, int64_t len);                      /* @0x1400633a8 真体 core_thread.c:41 */
extern void      PECMD_ReleaseWindowHooks(int64_t param_1);                  /* ≡dc:73211 core_b3_remaining.c:13399 */
extern void      PECMD_ReleaseObjectListTail(int64_t param_1, int param_2);  /* ≡dc:73213 core_b3_remaining.c:13354 */
extern uint64_t  PECMD_IsAlnumLowerDigit(uint16_t ch);                       /* @0x14005bc28 乙体独占导出 core_b3a.c */

/* ---- S8 TEMP PROBE（诊断行执行器链路用，过门后随 [S7] 系列一并移除） ---- */
void P8_Probe(const char *tag, long long v1, long long v2)
{
    FILE *pf_ = fopen("C:\\pectest\\memfail.log", "a");
    if (pf_) {
        fprintf(pf_, "[S8] %s a=%llx b=%llx\n", tag,
                (unsigned long long)(uintptr_t)v1, (unsigned long long)(uintptr_t)v2);
        fclose(pf_);
    }
}


uint64_t FUN_140103020(const uint16_t *s) { (void)s; return 0; }
void *FUN_1400170b0(void **p) { (void)p; return 0; }
void *FUN_140070044(const char *s) { (void)s; return 0; }
int FUN_1400678f0(void *a, long long *b, short c) { (void)a;(void)b;(void)c; return 0; }
void FUN_1400284d4(long long *a, const void *b) { (void)a;(void)b; }
void FUN_140062950(void *a) { (void)a; }
void FUN_14001b888(uint64_t a) { (void)a; }    /* CoUninitialize 槽 (PECMD_GetApiProcCached("CoUninitialize",...) 装载) */
uint64_t DAT_14013e168[8] = {0}; /* CRITICAL_SECTION COM */
/* T1d2: DAT_14013e190 独立定义已删除 — 与 core_globals.c 已初始化的 g_csInit 是同一原版关键段(双符号分裂曾致 restored_bodies 约50处锁零置桩 → EnterCriticalSection AV)。经 stubs_common.h 引用处直接改用 &g_csInit。 */
/* PTR_u 标识符字符短串指针表 (.rdata, NULL 结尾): pe_data_extract 14013a2d0 */
uint64_t PTR_u_a__a_zA_Z0_9___14013a2d0[] = {
    0x1401233c0, 0x1401233e0, 0x1401233f8, 0x140123410, 0x140123428,
    0x140123448, 0x140123460, 0x140123490, 0x1401234b0, 0
};   /* arity 修正 0->3 (PECMD_ToSysCopyFiles 直移) */
/* S8: @0x140003a20 双名归一 —— rename_map.json: FUN_140003a20 ≡ PECMD_RunCommandLine。
 * 原 no-op 桩使全部命令 TU 的行级变量展开死路; 现转发本文件下方 FUN_140003a20 直移真体。 */
uint64_t PECMD_RunCommandLine(void *script, WCHAR **str, int mode)
    { return (uint64_t)FUN_140003a20((longlong *)script,(undefined8 *)str,(byte)mode); }   /* def @6907 → 真体见本文件 FUN_140003a20 */
     /* 新增桩 @0x1400c10c0 */
const uint8_t DAT_14011dcb0[16] = {0x2a,0,0,0,'p','p','i','d',':',0,0,0,'h','p','i','d'}; /* 新增桩 */
const uint16_t DAT_14011d108[] = {'.','t','m','p',0};   /* argv 表末端字节偏移 (初 0) */
void FUN_1400166b4(void) { /* 初始化 argv 表 (缺失 helper, no-op 桩) */ }
/* S14 批次3: 同址别名归一 —— rename_map.json: FUN_1400186bc ≡ PECMD_ScriptInit
 * (dc:13335 size=161, 签名 void(undefined8*,longlong); 真体 src/runtime/core_exec5.c FUN_1400186BC)。
 * 原零参空桩 arity 与全部调用点(2参)不符且恒返 0 → 改为转发真体。 */
extern void FUN_1400186BC(void *s, int64_t parent);
void PECMD_ScriptInit(void *s, int64_t parent)
{
    FUN_1400186BC(s, parent);
}
uint64_t PECMD_WaitHandlesOrMessages(uint64_t param_1, int64_t param_2, int param_3, uint64_t *param_4) { (void)param_1;(void)param_2;(void)param_3;(void)param_4; return 0; }

/* S8: 同址别名归一 —— rename_map.json: FUN_14001b5ac ≡ PECMD_XorEncode（真体 core_var3.c）。 */
uint64_t PECMD_XorEncode(const uint16_t *a, uint32_t b, uint64_t c)
    { return (uint64_t)(uint32_t)FUN_14001B5AC((LPCWSTR)(uintptr_t)a,b,(int64_t)c); }
uint64_t PECMD_InstallKeyboardHook(void) { return 0; }
uint64_t PECMD_ExpandDrivePathAlloc(void) { return 0; }
uint64_t PECMD_AddVarDefault(void *script, LPCWSTR name, LPCWSTR val, int len, int64_t flag) { (void)script;(void)name;(void)val;(void)len;(void)flag; return 0; }   /* arity 修正 0->5 (PECMD_ExecCmdDispatch 恢复体) */
int64_t PECMD_FindVarValue(int64_t *a, LPCWSTR b, int64_t *c, int d) { (void)a;(void)b;(void)c;(void)d; return 0; }   /* arity 修正 0->4 (PECMD_ExecCmdDispatch 恢复体) */
/* S11(dc FUN_14001e6bc(longlong*,LPCWSTR,LPCWSTR,longlong)->void) */
void PECMD_SetVarCore(int64_t *script, LPCWSTR key, LPCWSTR val, int64_t n)
    { (void)script;(void)key;(void)val;(void)n; }
/* PECMD_FixKnownDlls32 — KnownDlls32 环境修复: 首个调用时读取系统模式标志, 在 64 位
   系统中注册 \\KnownDlls32 路径(经 ntdll 函数指针槽), 仅执行一次. */
uint FUN_14000e0bc(void) { return 0; }           /* 操作系统位宽探测 (no-op) */
uint64_t FUN_14006042c(void) { return 0; }
void PECMD_SendHotkeyKeyMessage(uint32_t a, int b, int c) { (void)a;(void)b;(void)c; }   /* 签名修正: uint32_t,int,int */
uint64_t PECMD_EnsureCallbackWindow(void) { return 0; }
/* ---- PECMD_ProcessScriptBlock 移植新增最小桩 (原未定义符号, 按调用点签名) ---- */
uint64_t PECMD_IsPecmdScriptFile(uint64_t a) { (void)a; return 0; }
uint64_t PECMD_UserCmdHandler(uint64_t a) { (void)a; return 0; }









uint64_t PECMD_MstrStringOps(uint64_t a, uint64_t b) { (void)a;(void)b; return 0; }

uint64_t PECMD_ArrayPrepend(void) { return 0; }
void PECMD_SetObjectEnable(int64_t a, unsigned int b) { (void)a; (void)b; }
void FUN_140053e78(void) { }
void PECMD_DestroyTrayIcon(char *param_1) { (void)param_1; }
uint64_t PECMD_UpdateTrayIcon(void) { return 0; }
DWORD PECMD_RegDeleteValue(HKEY root, LPCWSTR subkey, LPCWSTR name) { (void)root;(void)subkey;(void)name; return 0; }

/* R14(S18 普查 LINKAGE-RISK 收口): 三个别名此前均落本文件返0桩 ——
 * PECMD_AsciiPrefixICmp(rb 115 处)/小写 FUN_14005c788(rb 17 处)/
 * PECMD_AsciiWideICmp(≡FUN_14005C7C4) 全部恒"不匹配", 动词前缀/全串比较面失效。
 * 统一转发 core_string.c / core_exec5.c 已还原真体。 */
extern int FUN_14005C788(const char *s, const WCHAR *w, int n);   /* 真体 core_string.c (dc:54955) */
extern int32_t FUN_14005C7C4(const char *a, const WCHAR *w);      /* 真体 core_exec5.c (dc:54983) */
int PECMD_AsciiPrefixICmp(const char *a, const uint16_t *w, int n) { return FUN_14005C788(a, w, n); }
uint64_t PECMD_AsciiWideICmp(const char *a, const uint16_t *b) { return (uint64_t)(int32_t)FUN_14005C7C4(a, b); }


int64_t PECMD_EnableTokenPrivilege(LPCWSTR a, DWORD b, uint32_t c) { (void)a;(void)b;(void)c; return 0; }

void PECMD_DialogBeepNotify(int64_t a, int b) { (void)a;(void)b; }
uint64_t PECMD_NextRandomSeed(void) { return 0; }
/* S11(R1 双体分裂修正): 本名与真体 FUN_1400629B8(core_var.c) 同对应原版
 * @0x1400629b8(pecmd_defs.h:93 PECMD_SetVar 别名注记)。原 no-op 空桩致
 * b3r 家族 87 处调用静默丢变量写回(LSTR 结果恒空实锤) → 转发真体。 */
extern void FUN_1400629B8(void *script, LPCWSTR key, LPCWSTR value);
void PECMD_SetVariable(void *a, const WCHAR *b, const WCHAR *c)
{
    FUN_1400629B8(a, b, c);
}


uint64_t PECMD_ExpandDrivePath(const uint16_t *a, uint64_t b, uint16_t *c, longlong *d) { (void)a;(void)b;(void)c;(void)d; return 0; }
long long FUN_140064a34(uint16_t *s) { (void)s; return 0; }
uint64_t PECMD_EncodeStringId(LPCWSTR name, uint64_t *out, char mode) { (void)name;(void)out;(void)mode; return 0; }
/* S8: 同址别名归一 —— FUN_1400702b0 ≡ PECMD_StrDupAssign（真体 core_string.c）。 */
void PECMD_StrDupAssign(uint16_t **ps, const uint16_t *src)
    { FUN_1400702B0((WCHAR **)ps,(LPCWSTR)(uintptr_t)src); }
void PECMD_HeapFreeWithHeader(long long *a) { (void)a; }   /* @0x14005b0d4 串槽释放 (leaf stub), 原体见 decompiled.c:53617 */
uint64_t PECMD_ParsePrefixColon(void) { return 0; }
/* S8: 同址别名归一 —— rename_map.json: FUN_14007a224 ≡ PECMD_ExpandCommandLine,
 * FUN_14007bda8 ≡ PECMD_ExpandVarsRecursive（真体均在 core_execline.c）。
 * 原 no-op 桩使 ProcessScriptBlock→PECMD_ExpandVarDispatch 展开支路整体空转
 * （%VAR% 不展开, 且输出槽不写→调用方读未初始化）。现转发真体。 */
long long PECMD_ExpandCommandLine(long long *a, WCHAR *b, void *c, int d, uint8_t e)
    { P8_Probe("ExpandCL", (longlong)(uintptr_t)b, (longlong)d);
      return (long long)FUN_14007A224((void *)a,(WCHAR *)b,(WCHAR **)c,d,e); }
long long PECMD_ExpandVarsRecursive(long long *a, WCHAR *b, void *c, int d, uint8_t e)
    { P8_Probe("ExpandVR", (longlong)(uintptr_t)b, (longlong)d);
      return (long long)FUN_14007BDA8((void *)a,(WCHAR *)b,(WCHAR **)c,d,e); }
/* S11(dc 签名归正): 原为 uint64_t f(void) 零参占位 —— 与全部调用点(2-3 参)
 * ABI 不符。按 decompiled.c 原文签名改为占位真体(函数体仍待移植, 见 T4 分诊):
 *   FUN_14007d0ac(longlong*,LPCWSTR,LPCWSTR)->void  (变量前缀赋值)
 *   FUN_14007df90(longlong,int)->void               (CHECK 变量写) */
void PECMD_SetVariableWithPrefix(int64_t *ctx, LPCWSTR key, LPCWSTR value)
    { (void)ctx; (void)key; (void)value; }
void PECMD_SetCheckVariable(int64_t param_1, int param_2)
    { (void)param_1; (void)param_2; }
/* S8: 同址别名归一 —— FUN_14009bb28 ≡ PECMD_NotifyMainWindowRefresh（真体 restored_bodies.c）。 */
uint64_t PECMD_NotifyMainWindowRefresh(uint64_t a, int b) { FUN_14009BB28((void *)(uintptr_t)a,b); return 0; }   /* arity 修正 0->2 (core 调用方 + PECMD_ProcessScriptBlock 移入) */
int64_t PECMD_QueryFontInfo(int64_t a, int *b, const void *c) { (void)a;(void)b;(void)c; return 0; }
int PECMD_ParseHexOrDec() { return 1; }  /* 目标句柄克隆 (leaf stub) */
  /* 资源名定位 (leaf stub) */
  /* 设备/文件类型探测 (leaf stub) */
static longlong PECMD_GetPartitionLayoutEntry(uintptr_t h, int mode, ulonglong *out) { (void)h;(void)mode;(void)out; return 0; }

void PECMD_GetWindowTextAlloc(HWND param_1, uint64_t *param_2) { (void)param_1;(void)param_2; }
uint64_t PECMD_GetOwnerWindow(uint64_t param_1) { (void)param_1; return 0; }
uint64_t PECMD_GetFileSize(void) { return 0; }
uint64_t PECMD_ResDecode(int64_t *ps, uint32_t flag) { (void)ps;(void)flag; return 0; }
uint64_t PECMD_RichEditWordBreakProc(void) { return 0; }
uint64_t PECMD_EvalExpr(void) { return 0; }
uint64_t PECMD_ExecCmd(void) { return 0; }
uint64_t PECMD_MemCmp(void) { return 0; }
uint64_t PECMD_ResPrefA(void) { return 0; }
uint64_t PECMD_ResPrefB(void) { return 0; }
uint64_t PECMD_ResPrefC(void) { return 0; }
uint64_t PECMD_ResPrefD(void) { return 0; }
uint64_t PECMD_ResReenc(void) { return 0; }
uint64_t PECMD_SwapBytes(void) { return 0; }
uint64_t PECMD_XorDecode(void) { return 0; }
int FUN_140068984() { }

void FUN_140023640(uint64_t a, uint64_t b, int c) { (void)a;(void)b;(void)c; }
uint64_t thunk_FUN_140072814(void *a, longlong *b, const char *c) { (void)a;(void)b;(void)c; return 0; }
/* 本批(B1)新增定义 */
void PECMD_RestartSelf(uint16_t *param_1) { (void)param_1; }int FUN_14005B184(const char *a, const char *b, int n) { (void)a; (void)b; (void)n; return 0; }
void thunk_PECMD_GetNetworkConnectionName(const char *a, void *b, const char *c) { (void)a; (void)b; (void)c; }
uint16_t *PECMD_StrAlloc(uint16_t **ps, size_t count) { (void)ps; (void)count; return 0; }
void FUN_140060A74(uint8_t *buf, int len) { (void)buf; (void)len; }
/* 原版 @0x1400050c8: CreateFiber(FUN_1400050a0)->SwitchToFiber, fiber 体经
 * FUN_140004fd4 最终调用 PECMD_RunStartupScript(hinst,0,cmdline) (@0x14004eb34,
 * 真体 core_script2.c). 单行命令行无并发 yield, 顺序调用语义等价. */
int PECMD_RunBootScriptInFiber(uint16_t *cmdline)
{
    extern int64_t PECMD_RunStartupScript(HINSTANCE hinst, uint64_t flag,
                                          const WCHAR *cmd);
    extern HINSTANCE g_hInst; /* DAT_14013cf70 (原版实参来源 DAT_14013cad8 链) */
    PECMD_RunStartupScript(g_hInst, 0, cmdline);
    return 0;
}
int32_t PECMD_DecodeEncTextToUtf16(uint32_t spec, const uint8_t *src, int srclen,
                      uint16_t *dst, int dstcap, uint32_t key)
{ (void)spec; (void)src; (void)srclen; (void)dst; (void)dstcap; (void)key; return 0; }

/* S14 批次3: 同址别名归一 —— rename_map.json: FUN_140017cdc ≡ PECMD_ScriptCopy
 * (dc:12764 size=597, 签名 ptr(undefined8*,undefined8*) 返回 dst; 真体
 * src/runtime/core_exec5.c FUN_140017CDC)。原零参空桩 arity 不符且恒返 0 → 转发真体。 */
extern void *FUN_140017CDC(void *dst, void *src);
void *PECMD_ScriptCopy(void *dst, void *src)
{
    return FUN_140017CDC(dst, src);
}
/* S8: 同址别名归一 —— FUN_14002fd88 ≡ PECMD_ParseCommandPath（真体 core_b2e.c）。 */
uint64_t PECMD_ParseCommandPath(uint64_t a, uint64_t b, uint32_t *c, int64_t *d)
    { return (uint64_t)(uintptr_t)FUN_14002FD88((int64_t *)(uintptr_t)a,(WCHAR *)(uintptr_t)b,c,d); }   /* arity 修正 0->4 (core 调用方 + PECMD_ProcessScriptBlock 移入) */
uint64_t FUN_140061E98(void) { return 0; }
uint64_t FUN_1400688E0(void) { return 0; }
uint64_t FUN_14006A81C(void) { return 0; }
/* S8: 同址别名归一 —— rename_map.json: FUN_14006e030 ≡ PECMD_CopyPathToken（真体 core_b3m.c）。
 * 运行时取证: 本桩原返回 0 → DispatchExpressionBlock→ProcessScriptBlock 拿到
 * NULL 命令指针, SkipLeadingControls 解引用爆 C0000005(dump 0x12dddb 实锤)。 */
uint64_t PECMD_CopyPathToken(longlong a, longlong *b, longlong *c, longlong d)
{
    uint64_t r = (uint64_t)FUN_14006E030((int64_t)a,(int64_t *)(uintptr_t)b,(int64_t *)(uintptr_t)c,(int64_t)d);
    P8_Probe("CopyPathToken", (longlong)r, d);
    return r;
}

/* FUN_ helper 无操作桩 */
uint64_t FUN_140001188(void) { return 0; }
/* S11(dc 无此函数, 调用点 b1_remaining:5291 传1参取返回) 占位签名归正 */
uint64_t FUN_14000C764(LPCWSTR p) { (void)p; return 0; }
uint64_t PECMD_ResizeBuffer(void) { return 0; }
uint64_t PECMD_GetWindowObjectRef(void) { return 0; }
uint64_t PECMD_EncodeDet(long long a, uint64_t b) { (void)a;(void)b; return 1; }
/* S11(dc 签名归正): FUN_1400677b0(longlong*,longlong)->short, 原零参占位与
 * 调用点(2 参) ABI 不符; 函数体仍待移植(T4 分诊)。 */
short PECMD_ParseHashNumbers(int64_t *pp, int64_t val) { (void)pp; (void)val; return 0; }
uint64_t PECMD_GetComboItemText(void) { return 0; }
uint64_t PECMD_SaveSelectionToVar(void) { return 0; }
uint64_t PECMD_SkipWCharUntil(void *pp, uint16_t ch) { (void)pp;(void)ch; return 0; }
/* S11(dc FUN_1400549bc(longlong,uint,longlong)->void) */
void PECMD_InitDragDrop(int64_t a, uint32_t b, int64_t c)
    { (void)a;(void)b;(void)c; }
/* S11(dc FUN_14006643c(GUID*,LPCSTR)->undefined4) */
int PECMD_IsSetupClass(const void *pguid, const char *name)
    { (void)pguid;(void)name; return 0; }
/* S11(dc FUN_14007e01c(longlong,uint,LPWSTR)->void) */
void PECMD_SetControlState(int64_t a, uint32_t b, LPWSTR c)
    { (void)a;(void)b;(void)c; }
/* S11(dc FUN_1400e54d4(char*,HWND,longlong*,longlong,longlong)->void) */
void PECMD_ShowContextMenu(char *a, HWND h, int64_t *c, int64_t d, int64_t e)
    { (void)a;(void)h;(void)c;(void)d;(void)e; }
LRESULT PECMD_GetControlFont(int64_t param_1) { (void)param_1; return 0; }
int PECMD_UpdateWindowStyleBits(int64_t a, unsigned int b, uint64_t c) { (void)a;(void)b;(void)c; return 0; }
uint64_t FUN_1400ec698(int64_t a, uint64_t b) { (void)a;(void)b; return 0; }

void PECMD_LayoutChildWindows(int64_t a, unsigned char b) { (void)a;(void)b; }
uint64_t PECMD_InitWindowObjectF(void) { return 0; }



/* S11(dc FUN_1400f1378(longlong*,uint,int*,HWND,uint)->bool) */
bool PECMD_CreateDateTimePickCtl(int64_t *a, uint32_t b, int *c, HWND h, uint32_t e)
    { (void)a;(void)b;(void)c;(void)h;(void)e; return false; }
/* S11(dc 未含此函数(自建助手), 按调用点2参归正占位) */
void PECMD_InitControlObjC(void *a, void *b) { (void)a;(void)b; }

void PECMD_CreateStaticControlFromRect(int64_t *a, const void *b, unsigned int c, uint64_t *d, int64_t e, unsigned int f) { (void)a;(void)b;(void)c;(void)d;(void)e;(void)f; }
uint64_t *PECMD_InitStaticCtl(uint64_t *a, uint64_t b) { (void)a;(void)b; return a; }
/* S11(dc FUN_1400fc060(longlong*,DWORD,int*,HWND,uint)->bool) */
bool PECMD_CreateProgressCtl(int64_t *a, uint32_t b, int *c, HWND h, uint32_t e)
    { (void)a;(void)b;(void)c;(void)h;(void)e; return false; }
/* S11(dc FUN_140064b78(char*)->void) */
void thunk_FUN_140064b78(char *a) { (void)a; }
uint64_t PECMD_ProcessWindowObjMessage(void) { return 0; }

/* ---- B3 业务还原 (core_b3_remaining.c) 引用的未定义 helper 桩 ---- */
uint16_t PECMD_DispatchMessageMap(int64_t *param_1, int param_2, uint64_t param_3, uint64_t *param_4,
                       int64_t param_5, uint8_t param_6, uint64_t *param_7)
{ (void)param_1;(void)param_2;(void)param_3;(void)param_4;(void)param_5;(void)param_6;(void)param_7; return 0; }
uint64_t PECMD_MessageHookProc(int param_1, uint64_t param_2, uint64_t param_3)
{ (void)param_1;(void)param_2;(void)param_3; return 0; }
void PECMD_CloseThreadSlot(int64_t *param_1) { (void)param_1; }
void PECMD_DestroyStaticControl(uint64_t *param_1) { (void)param_1; }
void PECMD_ReleaseSlotObject(int64_t *param_1) { (void)param_1; }
int64_t PECMD_ControlWindowProc(uint64_t param_1, uint64_t param_2, uint64_t param_3, int64_t *param_4)
{ (void)param_1;(void)param_2;(void)param_3;(void)param_4; return 0; }

/* --- r36 follow-up stubs --- */
void PECMD_CleanupParser(int64_t p){ (void)p; }
void *PECMD_FreeResourceObject(void *a, unsigned int b){ (void)a;(void)b; return a; }

/* ---- 新增辅助桩 (core_b3 12 函数依赖; 签名与 decompiled 一致, 基类型等价) ---- */
/* S11(R11 门A 终局根因): 原 no-op 桩导致 STARTUPINFO.lpDesktop/lpTitle 等区域
 * 不被清零, 陈旧栈垃圾(lpTitle=0xe)直入内核 → CreateProcessInternalW
 * wcslen(0xe) AV(si.dump 探针实锤)。dc 原文 = CRT memset 内联
 * (@0x140102a90 零填充, attic/outA.txt 物证)。 */
void FUN_140102a90(uint64_t *dst, uint64_t v, uint64_t n)
{
    memset((void *)dst, (int)v, (size_t)n);
}
uint16_t *PECMD_DriveTypeName(int i, uint16_t *out, int max){ (void)i;(void)max; return out; }
uint64_t PECMD_ParseControlMessage(int64_t *a, uintptr_t b, uintptr_t c, const uint16_t *d, uintptr_t e, int64_t f){ (void)a;(void)b;(void)c;(void)d;(void)e;(void)f; return 0; }
uint64_t PECMD_ControlEnableCommand(int64_t *a, uintptr_t b, uintptr_t c, const uint16_t *d, uintptr_t e, int64_t f){ (void)a;(void)b;(void)c;(void)d;(void)e;(void)f; return 0; }
uint32_t PECMD_GetDiskGeometry(const uint16_t *p, uintptr_t h){ (void)p;(void)h; return 0; }
int64_t PECMD_OpenFileExisting(uint64_t a, uint64_t b, uint64_t c){ (void)a;(void)b;(void)c; return 0; }
void *PECMD_InitSimpleWindow(void *a, uint64_t *b, uint64_t *c){ (void)a;(void)b;(void)c; return a; }
void *PECMD_InitListViewObject(void *a, uint64_t *b, uint64_t *c){ (void)a;(void)b;(void)c; return a; }
void PECMD_AppendFormattedI64(int64_t *a, int64_t b){ (void)a;(void)b; }
void *PECMD_InstallInfDevice(void *p){ return p; }
void FUN_140021144(const WCHAR *p) { (void)p; }        /* 设备驱动安装后端 (no-op) */
void FUN_140019da8(long long p1, const WCHAR *p2, int p3) { (void)p1;(void)p2;(void)p3; }  /* 字体/安装回调 (no-op) */
void FUN_140020fd4(long long p1, char c, const WCHAR *p2, const WCHAR *p3) { (void)p1;(void)c;(void)p2;(void)p3; }

void PECMD_AllocExpandPath(void *a, int64_t *b){ (void)a;(void)b; }
void *PECMD_CheckInfDriver(int64_t *a, uint64_t b, char *c){ (void)a;(void)b;(void)c; return 0; }   /* arity 修正 0->2 (PECMD_ExecCmdDispatch 恢复体) */
void PECMD_ScanDeviDirInfFiles(void *a, uint64_t b, uint64_t c, int d, void *e){ (void)a;(void)b;(void)c;(void)d;(void)e; }
uint64_t PECMD_UpdateDriverSigning(int a, int b){ (void)a;(void)b; return 0; }
/* S8: 同址别名归一 —— FUN_14003cd0c/FUN_14003ed4c ≡ CmdKill/DispatchSystemCommandLine
 * （真体 core_b2f.c）。原 0 元桩按调用点实参补齐 arity 后转发。 */
int PECMD_CmdKill(int64_t *script, LPCWSTR name)
    { return (int)(intptr_t)FUN_14003CD0C(script,name); }
int PECMD_DispatchSystemCommandLine(int64_t *script, LPCWSTR cmd)
    { return (int)FUN_14003ED4C(script,cmd); }   /* arity 修正 0->2 (PECMD_ExecCmdDispatch 恢复体) */

/* ---- B3 还原批次 (core_b3r_*.c) 依赖补充 ---- */
uint64_t PECMD_ManualMapPeImage(int *a, int16_t *b, uint64_t c){ (void)a;(void)b; return c; }
/* ---- P0 补缺失符号桩 ---- */
/* ---- 批1-01ed5c 依赖桩 ---- */
/* S11: 旧零参体已删, 真签名版见上 (dc FUN_14001ebdc(LPCWSTR,longlong*)) */
uint32_t FUN_14001ebdc(LPCWSTR a, int64_t *b) { (void)a; (void)b; return 0; }
uint64_t FUN_140063344(void *p){ (void)p; return 0; }   /* 磁盘类型/分区对照表(未实现) */
uint64_t PECMD_OomPrompt(int a) { (void)a; return 0; }

uint64_t PECMD_LoadLogPixelsDpi(void){ return 0; }
uint64_t PECMD_GetDiskGeometrySize(void){ return 0; }
uint64_t PECMD_TaskShowMessage(uint64_t *p){ (void)p; return 0; }
uint64_t PECMD_FindPartitionInfo(void){ return 0; }
uint64_t PECMD_EncodeImageToStream(void){ return 0; }   /* 窗口枚举回调 (leaf stub, 定义见文件尾) */
uint64_t PECMD_UpdateWindowExStyle(void){ return 0; }
void PECMD_StrAlloc2(uint16_t **ps, int64_t count){ (void)ps;(void)count; }

/* --- r41 follow-up stubs (main-agent closure) --- */
/* PECMD_CreateButtonSubCtl — 创建 BUTTON 控件窗口并按其样式/标志初始化 (func: 按钮/超链接样式).
   依赖: FUN_14006b8fc (控件尺寸计算), PECMD_InitButtonState (按钮状态初始化, 内绕坐标/标志). */
long long FUN_14006b8fc(long long *param_1) { (void)param_1; return 1; }
int PECMD_CreateTreeViewControl(int64_t *a, unsigned long b, int *c, void *d, uint32_t e){ (void)a;(void)b;(void)c;(void)d;(void)e; return 1; }
uint64_t PECMD_ParseAngleNumbers(uint64_t *a, int64_t *b, int64_t *c, int64_t *d, int64_t *e){ (void)a;(void)b;(void)c;(void)d;(void)e; return 0; }
/* PECMD_HandleServiceCommandLine — 命令行后处理: 识别 /InstallService 等开关并把 argv 元素改写
   为服务名(前缀 ~+类型字母), 随后启动服务安装流程并 Exit. */
void FUN_140005344(void) { /* no-op */ }
uint16_t *FUN_14000531c(uint16_t *s) { return (uint16_t *)FUN_14000531C((WCHAR *)s); } /* R14 转发: 真体 core_proc.c:78 (三体等价分裂, 仲裁#2); v0 no-op 吞 rb:11643/11651 跳空白 */
void FUN_140017048(const WCHAR *s) { (void)s; }
/* --- batch28 restored-function (core_b3r_g*.c) link stubs --- */
/* data globals */
uint8_t  DAT_14013d5c0[8] = {0};
uint8_t  DAT_1401210f8[8] = {0};

/* helper FUN_ stubs referenced by restored bodies */
uint64_t PECMD_CheckDriveType(void){ return 0; }
uint64_t PECMD_OpenCloseDrive(void){ return 0; }
uint64_t PECMD_ShowWindowMode(void){ return 0; }
uint64_t PECMD_SendPingPacket(void){ return 0; }
uint64_t PECMD_InitWinsockOnce(void){ return 0; }
uint64_t PECMD_LzDecompress(void) { return 0; }
uint64_t PECMD_BufferedWriteByte(void) { return 0; }
uint64_t PECMD_StreamReadByte(void) { return 0; }
uint64_t PECMD_AllocConsoleBuffers(void) { return 0; }
uint64_t PECMD_CalcCalendarMonthRows(void) { return 0; }
uint64_t PECMD_StreamClose(void) { return 0; }
uint64_t PECMD_ConvertStringEncoding(void) { return 0; }
uint64_t PECMD_DeviceCheckReady(LPCWSTR p) { (void)p; return 0; }
uint64_t PECMD_JoinTokensAndResolve(void) { return 0; }
uint64_t PECMD_ParseCommaNumbers(void) { return 0; }


uint64_t PECMD_LoadRasApi(void) { return 0; }
uint64_t PECMD_FindRasConnection(void) { return 0; }
uint64_t PECMD_EnumRasConnections(void) { return 0; }
uint64_t PECMD_EnumPhonebookEntries(void) { return 0; }
uint64_t PECMD_ExecSpecialCommand(void) { return 0; }
uint64_t PECMD_FreeCacheBlock(void) { return 0; }
uint64_t PECMD_ShakeTrayCursor(void) { return 0; }
uint64_t FUN_14007033c(void) { return 0; }

uint64_t PECMD_EnumNetworkDevices(void) { return 0; }
uint64_t PECMD_ImportEnvironment(void) { return 0; }
uint64_t PECMD_SendTitleMessage(void) { return 0; }

uint64_t PECMD_ClipboardCommand(void) { return 0; }

/* ============================================================
   ---- 新增叶依赖桩 (FUN_14004e488 行解析器恢复所需) ----
   仅供链接满足符号, 无真实逻辑.
   ============================================================ */
int      FUN_14006156c(const uint16_t *a, const uint16_t *b) { (void)a;(void)b; return 0; }
uint64_t FUN_1400a53e4(int64_t a, void *b, void *c, int d, const uint16_t *e) { (void)a;(void)b;(void)c;(void)d;(void)e; return 0; }
/* R14(SUB 族 dump 16664 定案): FUN_14001b23c v0 桩不写回游标出参 param_4 →
 * 调用方 FUN_14006e030(rb core_b3m.c) 的 *pp 停在预清 0 → NULL 解引用 AV。
 * 按 dc:15729-15819 直移真体 + 两 helper(注释截断/RTrim)。 */
extern WCHAR *PECMD_AllocString(WCHAR **ps, int64_t count);      /* 真体 core_init.c @0x140063720 */
extern uint64_t *PECMD_SkipLeadingControls(uint64_t *pp);        /* 真体 core_b1_remaining.c @0x1400170b0 */
extern int64_t PECMD_WideStrLen(const uint16_t *s);              /* 真体 core_b9_remaining.c @0x140103020 */
extern uint8_t g_charTableF;                                     /* DAT_14013a248 pecmd_globals.h */

/* dc:13726-13741 RTrim: 去尾部空格/tab */
static LPCWSTR local_RTrim(LPCWSTR s)
{
    int n = lstrlenW(s);
    LPCWSTR end = s + n - 1;
    while (s <= end && (*end == L' ' || *end == L'\t')) {
        *(WCHAR *)end = L'\0';
        end--;
    }
    return s;
}

/* dc:15729-15771 注释/特殊前缀截断 */
static uint64_t local_StripInlineComments(uint16_t *buf, int len)
{
    uint64_t r;
    uint16_t *p;
    WCHAR *cursor = (WCHAR *)buf;
    PECMD_SkipLeadingControls((uint64_t *)&cursor);
    p = (uint16_t *)cursor;
    if (*cursor == 0x60 || *cursor == 0x7e || *cursor == 0x3b || *cursor == 0x2f ||
        *cursor == 0x23 || *cursor == 0x27 || *cursor == 0x3d || *cursor == 0x3a) {
        *buf = 0;
        return 1;
    }
    if (len < 1) {
        len = (int)PECMD_WideStrLen(buf);
    }
    {
        uint16_t *endp = buf + len;
        for (; p < endp; p++) {
            if (((8 < *p && *p < 0xe) || *p == 0x20) &&
                (p[1] == 0x60 || (p[1] == 0x2f && p[2] == 0x2f))) {
                *p = 0;
                return 1;
            }
        }
    }
    return 0;
}

/* dc:15778-15819 提取表段 token(XOR 解码+可选截断), 写回 *param_4 游标 */
const uint16_t *FUN_14001b23c(int64_t a, void *b, const uint16_t *c, void *d, char e)
{
    short ch;
    LPCWSTR ret;
    int n;
    uint16_t *src = (uint16_t *)c;
    int64_t *cur = (int64_t *)d;
    uint16_t *dst;
    if (*(int64_t *)d == 0) {
        *(int64_t *)d = (int64_t)(intptr_t)c;
        if (*(uint16_t *)(intptr_t)c != *(uint16_t *)(a + 0x88)) {
            for (;;) {
                ch = *(short *)*(int64_t *)d;
                if (ch == *(short *)(a + 0x90)) break;
                if (ch == *(short *)(a + 0x8a)) {
                    *(int64_t *)d += 2;
                }
                if (*(short *)*(int64_t *)d == *(short *)(a + 0x88)) break;
            }
        }
    }
    for (; src <= (uint16_t *)*(int64_t *)d &&
           (*(uint16_t *)(a + 0x92) == *src || *(uint16_t *)(a + 0x94) == *src);
         src++) {
    }
    n = (int)((*(int64_t *)d - (int64_t)(intptr_t)src) >> 1);
    PECMD_AllocString((WCHAR **)b, (int64_t)n + 6);
    dst = *(uint16_t **)b;
    for (; src < (uint16_t *)*(int64_t *)d; src++) {
        *dst = (uint16_t)(*(uint16_t *)(a + 0x48) ^ *src);
        dst++;
    }
    *dst = 0;
    dst[1] = 0;
    dst[2] = 0;
    ret = (LPCWSTR)*(void **)b;
    if ((0 < g_charTableF) && (*(char *)(a + 0xe) == '\0')) {
        local_StripInlineComments((uint16_t *)ret, n);
    }
    if (e == '\0') {
        local_RTrim(ret);
    }
    return ret;
}

/* ============================================================
   ---- 新增叶依赖桩 (FUN_140051610 表达式解析恢复所需) ----
   ============================================================ */
int      FUN_1400510e8(int *a, uint16_t **b, char *c) { (void)a;(void)b;(void)c; return 0; }
int      FUN_1400512f4(int *a, uint16_t **b, char *c) { (void)a;(void)b;(void)c; return 0; }


/* ============================================================
   ---- 新增叶依赖桩/数据 (FUN_140057940 字体控制消息恢复所需) ----
   ============================================================ */
uint64_t FUN_140067b54(uint16_t *a, double *b) { (void)a;(void)b; return 0; }
void     FUN_140007bcc(uint16_t *a, int *b) { (void)a;(void)b; }
int FUN_140074838() { return 1; }
void    FUN_14005e36c(int64_t a, uint32_t *b) { (void)a;(void)b; }

/* ============================================================
   ---- 新增叶依赖桩/数据 (FUN_140064694/14006587c/140065c04 数字解析族恢复所需) ----
   ============================================================ */
int      FUN_14005bc28(uint16_t a) { return (int)PECMD_IsAlnumLowerDigit(a); } /* R14 转发: 乙体独占名导出 core_b3a.c(dc:54316 无A-Z语义); v0 恒0 吞 b3_remaining 六处调用 */
int64_t  FUN_14005bbb4(int64_t *a) { (void)a; return 0; }   /* 默认色 CLR_NONE */
uint8_t DAT_14013a838[16] = {0x6d,0xbd,0xed,0x6a,0xb5,0x3f,0x8a,0x41,0x83,0xa6,0x7f,0x45,0x22,0x9d,0xc8,0x72};   /* GDI+ 格式 GUID(自.raw) */

/* SetupDi 设备枚举 (FUN_140072924) 缺失 helper 桩 */
uint64_t PECMD_FindTokenInList(const uint16_t *src, const uint16_t *devid, int len, uint flags)
{ (void)src;(void)devid;(void)len;(void)flags; return 0; }
uint64_t FUN_1400662a4(int64_t *p1, void *p2, int64_t *p3, const uint16_t *p4,
                       const uint16_t *p5, uint p6)
{ (void)p1;(void)p2;(void)p3;(void)p4;(void)p5;(void)p6; return 0; }
void *FUN_140063224(uint64_t *a, uint64_t b) { (void)a;(void)b; return 0; }
uint64_t thunk_FUN_1400f429c(void *a, short b) { (void)a;(void)b; return 0; }
/* DAT_140125230 -> g_dbl25230 = 0.5 (core_globals.c 定义), DAT_140125238 -> g_fontMinus0 = -0.0 */
/* 计算器栈写入辅助: 将值 param_3 写入栈元素 param_2 并递减栈指针 (param_1+4) */
void FUN_14005bc48(int64_t param_1, double *param_2, double param_3)
{ (void)param_1;(void)param_2;(void)param_3; }
void FUN_14005bc5c(int64_t param_1, double *param_2, double param_3)
{ (void)param_1;(void)param_2;(void)param_3; }
/* 取计算器栈顶当前值 (取模运算种子) */
uint64_t FUN_14005e0a0(void) { return 0; }                /* 默认命令串缓存区(静态, 初 0) */

/* 缺失 helper 桩 (无调用方校验, 仅满足符号) */
uint64_t FUN_14005ea5c(void) { return 0; }  /* 注册表读取值 */
int      FUN_140003864(void *a, const uint16_t *b, uint32_t c, uint32_t d, void *e, uint32_t f, uint32_t g, void *h) { (void)a;(void)b;(void)c;(void)d;(void)e;(void)f;(void)g;(void)h; return 0; }
void    *FUN_14005b374(void *a, int16_t b, int16_t c) { (void)a;(void)b;(void)c; return a; } /* 行切分 */
void     PECMD_AppendQuotedString(int64_t *a, void *b, int c) { (void)a;(void)b;(void)c; }              /* 串填充 */
uint64_t FUN_1400048c4(int64_t *a) { (void)a; return 0; }        /* 命名互斥/事件 */
void     FUN_1400177b8(void *a) { (void)a; }                                                 /* 释放互斥/事件 */
uint32_t FUN_14000bfcc(uint64_t a, uint64_t b, const uint16_t *c) { (void)a;(void)b;(void)c; return 0; }
/* FUN_14005b540 — 去除串内前导/尾随空白 (PECMD_PinStartMenuTask 使用) */
void     FUN_14005b540(uint16_t *a, int b) { (void)a;(void)b; }

/* ---- P4 wave-4 helper 定义 (与头部前置声明对应) ---- */
void     FUN_14000500c(void) {}
void     FUN_140005738(int a, const char *b) { (void)a;(void)b; }
uint64_t FUN_140008c5c(uint64_t a, uint64_t b, void *c, void *d) { (void)a;(void)b;(void)c;(void)d; return 0; }
uint64_t FUN_14000befc(void *p) { (void)p; return 0; }
uint64_t FUN_14005370c(uint32_t *a, void *b, uint64_t c) { (void)a;(void)b;(void)c; return 0; }
int      FUN_14005ceec(const uint16_t *a, const char *b) { (void)a;(void)b; return 1; }
void     FUN_14005d694(void) {}
void     FUN_14006355c(void *a, const WCHAR *b, int c, uint64_t d) { (void)a;(void)b;(void)c;(void)d; }
/* S8 直移: FUN_140063888 @0x140063888 size=109 (decompiled.c:60952-60968)
 * ≡ PECMD_StrCopyW。len<0→lstrlen; AllocString(len+1); 拷贝 len*2 字节; 终止 0。
 * 原为 no-op 桩 ⇒ PECMD_StrBldCopyWide/StrBldCopyWideN 全部空转,
 * ProcessScriptBlock 行展开结果槽保持 NULL → SkipLeadingControls 读 NULL 爆
 * C0000005(dumpbt 0x12dddb 实锤)。FUN_14001d78c=内联 memcpy。 */
void FUN_140063888(void *a, const WCHAR *b, int64_t c)
{
  P8_Probe("SCWin", (longlong)(uintptr_t)a, (longlong)(uintptr_t)b);
  longlong *slot = (longlong *)(uintptr_t)a;
  if (c < 0) {
    c = lstrlenW(b);
  }
  FUN_140063720(slot, c + 1);
  if (*slot != 0) {
    memcpy((void *)(uintptr_t)*slot, (const void *)(uintptr_t)b, (size_t)(c * 2));
    *(undefined2 *)(uintptr_t)(c * 2 + *slot) = 0;
  }
  P8_Probe("StrCopyW", (longlong)(uintptr_t)*slot, (longlong)c);
}
uint64_t FUN_14009d4b8(uint64_t a, uint64_t b, const WCHAR *c, int16_t d, const WCHAR *e, int f) { (void)a;(void)b;(void)c;(void)d;(void)e;(void)f; return 0; }

/* ---- P4 wave-4 补桩: 声明存在但缺定义的内部 helper ---- */
uint      FUN_14001b608(uint64_t a) { (void)a; return 0; }
long long FUN_140031454(long long *a, pthreadmbcinfo b) { (void)a;(void)b; return 0; }
int       FUN_140067cf4(long long *a, uint64_t *b) { (void)a;(void)b; return 0; }
uint64_t FUN_14004fb44(longlong *a, ulonglong b) { (void)a;(void)b; return 1; }
void FUN_14005d9a8(int64_t a, int b) { (void)a;(void)b; }
uint64_t FUN_1400630d0(int a) { (void)a; return 0; }

/* ---- P4 wave-4 helper 补定义 (2) ---- */
uint64_t FUN_14005c7c4(const char *a, const uint16_t *b) { (void)a;(void)b; return 0; }
void FUN_1400702b0(uint16_t **ps, const uint16_t *src) { (void)ps;(void)src; } /* @0x14005b6ac 资源串 */
/* ==== S8 直移: FUN_140003a20 @0x140003a20 size=139 (decompiled.c:1236-1264) ====
 * 变量展开包装（PECMD_RunCommandLine 同体）: 备份 *param_2 旧串→置空输出槽→
 * 按脚本标志单遍(14007bda8=ExpandVarsRecursive)或双遍(14007a224=ExpandCommandLine,
 * 第二遍 opt=0)展开→释放旧串。依赖真体: core_execline.c / PECMD_FreeStrBuf。 */
longlong FUN_140003a20(longlong *param_1,undefined8 *param_2,byte param_3)

{
  WCHAR *pWVar1;
  longlong lVar2;
  WCHAR *local_res8;

  local_res8 = (WCHAR *)(uintptr_t)*param_2;
  *param_2 = 0;
  if ((*(char *)((longlong)(uintptr_t)param_1 + 0xda) == '\0') &&
     ((*(byte *)((longlong)(uintptr_t)param_1 + 0xd) & 0xf) == 0)) {
    lVar2 = (longlong)FUN_14007BDA8((void *)(uintptr_t)param_1,local_res8,(WCHAR **)param_2,0,param_3);
  }
  else {
    (void)FUN_14007A224((void *)(uintptr_t)param_1,local_res8,(WCHAR **)param_2,0,param_3);
    pWVar1 = (WCHAR *)(uintptr_t)*param_2;
    *param_2 = (undefined8)(uintptr_t)local_res8;
    local_res8 = pWVar1;
    lVar2 = (longlong)FUN_14007A224((void *)(uintptr_t)param_1,pWVar1,(WCHAR **)param_2,0,0);
  }
  PECMD_FreeStrBuf((WCHAR **)&local_res8);
  P8_Probe("RunCmdLine", (longlong)lVar2, (longlong)param_3);
  return lVar2;
}
undefined8 FUN_1400748a0(HWND param_1, undefined8 *param_2) { (void)param_1;(void)param_2; return 1; }
void       FUN_1400f172c(longlong *a, UINT b, ulonglong c, ulonglong *d, longlong e, int f, undefined8 *g) { (void)a;(void)b;(void)c;(void)d;(void)e;(void)f;(void)g; }   /* 0x14013e290 原始: 48 b8 05 00 86 b8 05 00 (RTL_UTF8... 文本转储回调表) */
/* S8: 同址别名归一 —— rename_map.json: FUN_140063720 ≡ PECMD_AllocString
 * （真体 core_string.c:96, 经 PECMD_HeapRealloc@0x140063118）。 */
void  FUN_140063720(longlong **_ps, longlong len)
    { PECMD_AllocString((WCHAR **)(uintptr_t)_ps, len); }

/* ---- wave-current support: 018220/018c6c/01b3a0/01b888 deps ---- */
undefined8 FUN_14005c788(char *a, ushort *b, int c) { return (undefined8)FUN_14005C788(a, b, c); } /* R14 转发(原 leaf stub 返0): ≡大写 FUN_14005C788 真体 core_string.c */
void  FUN_140018148(HMODULE a, LPCSTR b) { (void)a;(void)b; }                              /* 重定向 thunk: GetProcAddress (leaf stub) */
void  FUN_140018178(HMODULE a, LPWSTR b, DWORD c) { (void)a;(void)b;(void)c; }                          /* 重定向 thunk: GetModuleHandleW (leaf stub) */
void  FUN_1400185c8(char a, longlong *b) { (void)a;(void)b; }                             /* 输出缓冲刷新 (leaf stub) */
void  FUN_140017908(void) { }                                                             /* 提权检查 (leaf stub) */
DWORD FUN_1400717f0(HKEY a, LPCWSTR b) { (void)a;(void)b; return 0; }         /* 0x14013c934 TLS 槽索引: 00 00 00 00 */
longlong DAT_14013cb18[8] = {0};


/* ---- wave-current support: 022e94 ---- */
unsigned int FUN_140022e24(void) { return 0; }


/* ---- wave-current support: 0612cc ---- */
void FUN_14001c2cc(const unsigned short *a, int b, int c) { (void)a;(void)b;(void)c; }


/* ---- wave-current support: 079cf8/078c68 ---- */
int FUN_140072924(uint param_1, LPCWSTR param_2, uint param_3, longlong *param_4, ushort *param_5) { (void)param_1;(void)param_2;(void)param_3;(void)param_4;(void)param_5; return 0; }  /* SetupDi 枚举 (leaf stub) */
int FUN_1400664dc(undefined8 param_1, int param_2) { (void)param_1;(void)param_2; return 0; }  /* 设备重扫描 (leaf stub) */
DWORD FUN_1400195f0(undefined8 param_1, longlong param_2, int param_3, undefined8 *param_4) { (void)param_1;(void)param_2;(void)param_3;(void)param_4; return 0; }
void FUN_14001e6bc(longlong *param_1, LPCWSTR param_2, LPCWSTR param_3, longlong param_4) { (void)param_1;(void)param_2;(void)param_3;(void)param_4; }
undefined1 *FUN_14005b848(undefined1 *a) { (void)a; return a; }    /* 0x14013cf70 HINSTANCE 槽 */
uint8_t DAT_14011d308[8] = {0};


/* ---- wave-current support: 0ebd30 ---- */
void  FUN_1400e7664(LPCWSTR a) { (void)a; }  /* 0x14013e298 参数槽 */
uint8_t  DAT_14013e300[512] = {0}; /* 0x14013e300 错误消息缓冲 */

/* ---- wave-current support: 073ccc ---- */
void FUN_1400629b8(void *a, const WCHAR *b, const WCHAR *c) { (void)a;(void)b;(void)c; }  /* 变量赋值 (leaf stub) */
uint8_t DAT_1401206f4[8] = {0};

/* ---- wave-INDIR: d9818 依赖最小桩定义体 (TODO(verify) 后续波次还原真体) ---- */
void FUN_14005b900(char *slot, void *hwnd, LPCWSTR tip, HICON icon, uint msg)
    { (void)slot;(void)hwnd;(void)tip;(void)icon;(void)msg; }
uint32_t FUN_14007e15c(byte *slot, void *hwnd, LPCWSTR a3, HICON a4, LPCWSTR a5, uint a6, int a7)
    { (void)slot;(void)hwnd;(void)a3;(void)a4;(void)a5;(void)a6;(void)a7; return 0; }
void FUN_1400e6860(WPARAM a1, int a2) { FUN_1400E6860((uint64_t *)(uintptr_t)a1, a2); } /* R14 转发: 真体 core_b8f.c:226(≡rb:10034 EndDialogDeferred); v0 no-op 吞 rb:16423 */
void FUN_14006703c(longlong obj) { PECMD_ReleaseWindowHooks(obj); } /* R14 转发: 真体 core_b3_remaining.c:13399(≡dc:73211); v0 no-op 致 073c58 清理流量走劣化副本丢钩子释放(仲裁#3 高优先) */
/* ==== S8 直移: FUN_1400b1724 @0x1400b1724 size=1733 (decompiled.c:110300-110509) ====
 * 行循环执行器（PECMD_DispatchExpressionBlock 同体, rename_map.json 权威）。
 * 依赖重绑真体（绕开同址 no-op 桩）: FUN_14006156c→PECMD_MatchPattern(core_b3c.c),
 * FUN_1400a53e4→PECMD_TokenizeExpression(core_b3_remaining.c), FUN_14001b23c→
 * PECMD_ExtractTableSegment(core_b1_remaining.c), FUN_14006e030→FUN_14006E030
 * (core_b3m.c), FUN_1400702d4→PECMD_StrBldCopyWideN(core_var2.c),
 * FUN_14005b0b8→PECMD_ZeroLenBuf, FUN_14005b104→PECMD_FreeStrBuf,
 * FUN_14006159c→PECMD_InitObfuscatedKeywords(core_exec2.c), FUN_1400251ac→
 * PECMD_CheckFirstStartupFlag(core_scriptdep.c), FUN_14004c0bc→
 * PECMD_ProcessScriptBlock(restored_bodies.c 真体)。
 * DAT_14013d080→g_msgLockCount, DAT_14013d058→g_sysinitState。 */
ulonglong FUN_1400b1724(LARGE_INTEGER param_1,ulonglong param_2)

{
  ushort uVar1;
  ushort uVar2;
  int iVar3;
  WCHAR *pWVar4;
  uint64_t uVar5;
  LPCWSTR pWVar6;
  LARGE_INTEGER lpStr1;
  WCHAR WVar7;
  WCHAR WVar8;
  longlong lVar9;
  int iVar10;
  WCHAR *pWVar11;
  WCHAR *pWVar12;
  LPCWSTR *ppWVar13;
  WCHAR *local_res10;
  ulonglong *local_res18;
  WCHAR *local_res20;
  LPCWSTR local_58;
  LPCWSTR local_50 [2];

  uVar1 = *(ushort *)(param_1.QuadPart + 0x48);
  iVar10 = 0;
  pWVar11 = (WCHAR *)(uintptr_t)(param_2 & 0xfffffffffffffffeULL);
  local_res18 = (ulonglong *)0x0;
  if ((param_2 & 1) == 0) {
    PECMD_InitObfuscatedKeywords((void *)(uintptr_t)param_1.QuadPart,
        (uint64_t)(*(ushort *)(param_1.QuadPart + 0x48) | 0x10000));
    *(undefined2 *)(uintptr_t)(param_1.QuadPart + 200) = 0;
    for (; (((WVar8 = *pWVar11, *(WCHAR *)(param_1.QuadPart + 0x92) == WVar8 ||
             (*(WCHAR *)(param_1.QuadPart + 0x94) == WVar8)) ||
            (*(WCHAR *)(param_1.QuadPart + 0x8a) == WVar8)) ||
           (*(WCHAR *)(param_1.QuadPart + 0x90) == WVar8)); pWVar11 = pWVar11 + 1) {
    }
  }
  local_58 = (LPCWSTR)0x0;
  if (((*(byte *)(param_1.QuadPart + 0x10) & 0x80) == 0) ||
     (ppWVar13 = &local_58, g_msgLockCount == 0)) {
    ppWVar13 = local_50;
  }
  local_res10 = pWVar11;
  if (*pWVar11 != *(WCHAR *)(param_1.QuadPart + 0x88)) {
LAB_1400b17e7:
    do {
      if ((*(byte *)(param_1.QuadPart + 0x11) & 1) != 0) {
        PECMD_ZeroLenBuf(local_res18);
      }
      if ((*(byte *)(param_1.QuadPart + 200) & 0x42) != 0) break;
      WVar8 = *(WCHAR *)(param_1.QuadPart + 0x88);
      local_res10 = pWVar11;
      pWVar4 = (WCHAR *)(uintptr_t)FUN_140063060((uint64_t)(uintptr_t)pWVar11);
      if (*pWVar4 == WVar8) break;
      if (iVar10 < 1) {
        if (*(WCHAR *)(param_1.QuadPart + 0x96) == *pWVar11) {
          pWVar4 = pWVar11 + 1;
          for (pWVar11 = pWVar4;
              ((WVar7 = *pWVar11, *(WCHAR *)(param_1.QuadPart + 0x92) == WVar7 ||
               (*(WCHAR *)(param_1.QuadPart + 0x94) == WVar7)) ||
              ((*(WCHAR *)(param_1.QuadPart + 0x8a) == WVar7 ||
               (*(WCHAR *)(param_1.QuadPart + 0x90) == WVar7)))); pWVar11 = pWVar11 + 1) {
          }
          local_res10 = pWVar11;
          if (((*pWVar11 != WVar8) &&
              (uVar5 = PECMD_MatchPattern((int16_t *)(uintptr_t)pWVar11,param_1.QuadPart + 0xaa), (int)uVar5 != 0)) &&
             (uVar5 = PECMD_MatchPattern((int16_t *)(uintptr_t)pWVar11,param_1.QuadPart + 0x9a), (int)uVar5 != 0)) {
            local_res10 = pWVar4;
            PECMD_TokenizeExpression(param_1,*(int64_t *)(param_1.QuadPart + 0x40),
                          (int64_t *)(intptr_t)&local_res10,0,(WCHAR *)(uintptr_t)&DAT_14011c638);
            for (; ((WVar8 = *local_res10, *(WCHAR *)(param_1.QuadPart + 0x92) == WVar8 ||
                    (*(WCHAR *)(param_1.QuadPart + 0x94) == WVar8)) ||
                   ((*(WCHAR *)(param_1.QuadPart + 0x8a) == WVar8 ||
                    (pWVar11 = local_res10, *(WCHAR *)(param_1.QuadPart + 0x90) == WVar8))));
                local_res10 = local_res10 + 1) {
            }
          }
          goto LAB_1400b17e7;
        }
        if ((*pWVar11 == *(WCHAR *)(param_1.QuadPart + 0xba)) &&
           (pWVar11[1] == *(WCHAR *)(param_1.QuadPart + 0xbc))) {
          for (local_res10 = pWVar11 + 2;
              (((WVar7 = *local_res10, *(WCHAR *)(param_1.QuadPart + 0x92) == WVar7 ||
                (*(WCHAR *)(param_1.QuadPart + 0x94) == WVar7)) ||
               (*(WCHAR *)(param_1.QuadPart + 0x8a) == WVar7)) ||
              (*(WCHAR *)(param_1.QuadPart + 0x90) == WVar7)); local_res10 = local_res10 + 1) {
          }
          WVar7 = *local_res10;
          if (WVar7 != WVar8) {
            do {
              if (((WVar7 == *(WCHAR *)(param_1.QuadPart + 0x8a)) ||
                  (WVar7 == *(WCHAR *)(param_1.QuadPart + 0x90))) ||
                 (*(WCHAR *)(param_1.QuadPart + 0x96) == WVar7)) break;
              local_res10 = local_res10 + 1;
              WVar7 = *local_res10;
            } while (WVar7 != *(WCHAR *)(param_1.QuadPart + 0x88));
          }
          for (pWVar4 = local_res10;
              ((*(WCHAR *)(param_1.QuadPart + 0x92) == *pWVar4 ||
               (*(WCHAR *)(param_1.QuadPart + 0x94) == *pWVar4)) ||
              ((*(WCHAR *)(param_1.QuadPart + 0x8a) == *pWVar4 ||
               (*(WCHAR *)(param_1.QuadPart + 0x90) == *pWVar4)))); pWVar4 = pWVar4 + 1) {
          }
          if ((uVar1 ^ 0x7b) == *pWVar4) {
            PECMD_StrBldCopyWideN((WCHAR **)&local_res20,pWVar11,
                          (longlong)local_res10 - (longlong)pWVar11 >> 1);
            for (lVar9 = (longlong)((int)((longlong)local_res10 - (longlong)pWVar11 >> 1) + -1);
                -1 < lVar9; lVar9 = lVar9 + -1) {
              local_res20[lVar9] = local_res20[lVar9] ^ *(ushort *)(param_1.QuadPart + 0x48);
            }
            for (pWVar11 = pWVar4 + 1;
                (((WVar8 = *pWVar11, *(WCHAR *)(param_1.QuadPart + 0x92) == WVar8 ||
                  (*(WCHAR *)(param_1.QuadPart + 0x94) == WVar8)) ||
                 (*(WCHAR *)(param_1.QuadPart + 0x8a) == WVar8)) ||
                (*(WCHAR *)(param_1.QuadPart + 0x90) == WVar8)); pWVar11 = pWVar11 + 1) {
            }
            local_res10 = pWVar11;
            if (((*pWVar11 != *(WCHAR *)(param_1.QuadPart + 0x88)) &&
                (pWVar12 = local_res20, uVar5 = PECMD_MatchPattern((int16_t *)(uintptr_t)pWVar11,param_1.QuadPart + 0xaa),
                (int)uVar5 != 0)) &&
               (uVar5 = PECMD_MatchPattern((int16_t *)(uintptr_t)pWVar11,param_1.QuadPart + 0x9a), (int)uVar5 != 0)) {
              local_res10 = pWVar4 + 1;
              PECMD_TokenizeExpression(param_1,*(int64_t *)(param_1.QuadPart + 0x40),
                            (int64_t *)(intptr_t)&local_res10,0,pWVar12);
              pWVar11 = local_res10;
              PECMD_FreeStrBuf((WCHAR **)&local_res20);
              goto LAB_1400b17e7;
            }
            PECMD_FreeStrBuf((WCHAR **)&local_res20);
          }
          WVar8 = *local_res10;
          if (((WVar8 == *(WCHAR *)(param_1.QuadPart + 0x92)) ||
              (WVar8 == *(WCHAR *)(param_1.QuadPart + 0x94))) ||
             ((WVar8 == *(WCHAR *)(param_1.QuadPart + 0x8a) ||
              (WVar8 == *(WCHAR *)(param_1.QuadPart + 0x90))))) {
            while (((*(WCHAR *)(param_1.QuadPart + 0x88) != WVar8 &&
                    (*(WCHAR *)(param_1.QuadPart + 0x8a) != WVar8)) &&
                   (*(WCHAR *)(param_1.QuadPart + 0x90) != WVar8))) {
              local_res10 = local_res10 + 1;
              WVar8 = *local_res10;
            }
          }
          for (; ((WVar8 = *local_res10, *(WCHAR *)(param_1.QuadPart + 0x92) == WVar8 ||
                  (*(WCHAR *)(param_1.QuadPart + 0x94) == WVar8)) ||
                 ((*(WCHAR *)(param_1.QuadPart + 0x8a) == WVar8 ||
                  (pWVar11 = local_res10, *(WCHAR *)(param_1.QuadPart + 0x90) == WVar8))));
              local_res10 = local_res10 + 1) {
          }
          goto LAB_1400b17e7;
        }
      }
      local_res10 = (WCHAR *)0x0;
      local_50[0] = pWVar11;
      pWVar6 = PECMD_ExtractTableSegment(param_1.QuadPart,(uint64_t *)&local_res18,
                             (uint16_t *)(uintptr_t)pWVar11,
                             (int64_t *)(intptr_t)&local_res10,'\0');
      if (((*pWVar6 != L'\0') && (*pWVar6 != L'/')) && ((*pWVar6 != L';' && (*pWVar6 != L'`')))) {
        lpStr1.QuadPart =
             (longlong)FUN_14006E030(param_1.QuadPart,(int64_t *)(intptr_t)&local_res10,
                           (int64_t *)(intptr_t)&local_res18,
                           (int64_t)(intptr_t)pWVar6);
        iVar3 = StrCmpNIW((LPCWSTR)(uintptr_t)lpStr1.QuadPart,L"_SUB",4);
        if ((iVar3 == 0) &&
           ((((uVar2 = *(ushort *)(lpStr1.QuadPart + 8), 8 < uVar2 && (uVar2 < 0xe)) ||
             (uVar2 == 0x20)) || (uVar2 == 0)))) {
          iVar10 = iVar10 + 1;
        }
        else {
          iVar3 = StrCmpNIW((LPCWSTR)(uintptr_t)lpStr1.QuadPart,L"_END",4);
          if ((iVar3 == 0) &&
             (((uVar2 = *(ushort *)(lpStr1.QuadPart + 8), 8 < uVar2 && (uVar2 < 0xe)) ||
              ((uVar2 == 0x20 || (uVar2 == 0)))))) {
            if (0 < iVar10) {
              iVar10 = iVar10 + -1;
            }
          }
          else if (iVar10 < 1) {
            if ((((g_sysinitState == '\x03') &&
                 ((iVar3 = StrCmpNIW(L"REGI",(LPCWSTR)(uintptr_t)lpStr1.QuadPart,4), iVar3 != 0 ||
                  (((uVar2 = *(ushort *)(lpStr1.QuadPart + 8), uVar2 < 9 || (0xd < uVar2)) &&
                   ((uVar2 != 0x20 && (uVar2 != 0)))))))) &&
                ((iVar3 = StrCmpNIW(L"ENVI",(LPCWSTR)(uintptr_t)lpStr1.QuadPart,4), iVar3 != 0 ||
                 ((((uVar2 = *(ushort *)(lpStr1.QuadPart + 8), uVar2 < 9 || (0xd < uVar2)) &&
                   (uVar2 != 0x20)) && (uVar2 != 0)))))) &&
               ((iVar3 = StrCmpNIW(L"SET",(LPCWSTR)(uintptr_t)lpStr1.QuadPart,3), iVar3 != 0 ||
                (((uVar2 = *(ushort *)(lpStr1.QuadPart + 6), uVar2 < 9 || (0xd < uVar2)) &&
                 ((uVar2 != 0x20 && (uVar2 != 0)))))))) {
              PECMD_CheckFirstStartupFlag(param_1.QuadPart);
            }
            PECMD_ProcessScriptBlock(param_1,lpStr1,(longlong *)0x0,(longlong *)&local_res10,
                          (pthreadmbcinfo)*ppWVar13);
          }
        }
      }
      for (; (*local_res10 == *(WCHAR *)(param_1.QuadPart + 0x8a) ||
             (*local_res10 == *(WCHAR *)(param_1.QuadPart + 0x90))); local_res10 = local_res10 + 1)
      {
      }
      pWVar11 = local_res10;
    } while (*local_res10 != *(WCHAR *)(param_1.QuadPart + 0x88));
  }
  if ((param_2 & 1) == 0) {
    *(undefined2 *)(uintptr_t)(param_1.QuadPart + 200) = 0;
  }
  PECMD_FreeStrBuf((WCHAR **)&local_res18);
  return 0;
}
void FUN_140066d18(longlong a1, longlong a2) { (void)a1;(void)a2; }
ulonglong FUN_14006b7f0(longlong obj, longlong *a2) { (void)obj;(void)a2; return 0; }
void FUN_14007df90(longlong a1, int a2) { (void)a1;(void)a2; }
void FUN_1400e54d4(char *slot, HWND h, longlong *a3, longlong a4, longlong a5)
    { (void)slot;(void)h;(void)a3;(void)a4;(void)a5; }
void FUN_14002e790(HWND h) { (void)h; }
char FUN_1400d5b48(longlong obj, HDC dc) { (void)obj;(void)dc; return 0; }
ulonglong FUN_1400e89fc(HDC obj, ulonglong a2, HDC a3, longlong *a4)
    { (void)obj;(void)a2;(void)a3;(void)a4; return 0; }

/* ---- wave-INDIR: f6db0 依赖最小桩定义体 (TODO(verify) 后续波次还原真体) ---- */
void FUN_1400633a8(void **ps, int64_t len) { FUN_1400633A8(ps, len); } /* R14 转发+签名归正(int()->void(*,int64)): 真体 core_thread.c:41(dc:60677 带0xaa55头); v0 零参空桩吞 rb:17608 */
void FUN_1400f527c(longlong self, HDC dc, void *attr, LPRECT rc)                      /* 焦点框覆盖绘制 */
    { (void)self;(void)dc;(void)attr;(void)rc; }
undefined4 FUN_1400ede48(HDC dc, HICON ic, undefined8 a3, int a4, ulonglong a5, ulonglong a6,
                         undefined4 a7, undefined4 a8, ulonglong a9, ulonglong a10, COLORREF a11)
    { (void)dc;(void)ic;(void)a3;(void)a4;(void)a5;(void)a6;(void)a7;(void)a8;(void)a9;(void)a10;(void)a11; return 0; }

void FUN_140066eac(longlong a1, int a2) { PECMD_ReleaseObjectListTail(a1, a2); } /* R14 转发: 真体 core_b3_remaining.c:13354(≡dc:73213); v0 no-op 丢对象列表尾释放 */
ulonglong FUN_140082520(longlong *a1, WCHAR *a2, longlong a3, int a4) { (void)a1;(void)a2;(void)a3;(void)a4; return 0; }
/* ==== B0/P3: scalar data definitions (single copy lives here) ==== */

unsigned long DAT_14013c934;
longlong DAT_14013cb18[8];
uint64_t PTR_FUN_1401234f0;
void *DAT_14013cb10;
void *DAT_14013ccf8;
void *DAT_14013cd18;
void *DAT_14013cd20;
void *DAT_14013cd28;
void *DAT_14013cfb0;
void *DAT_14013cf50;
void *DAT_1401293c0;
void *DAT_14013d868;
uint64_t DAT_14013e2a8;
void *DAT_14013ca68;
void *DAT_14013e1f8;
void *DAT_14013e200;
uint32_t DAT_14013a038 = 3;
void    *DAT_14013c910 = 0;
uint32_t DAT_14013c918 = 0;
uint32_t DAT_14013c91c = 0;
uint32_t DAT_14013c920 = 0;
uint32_t DAT_14013c924 = 0;
uint32_t DAT_14013c928 = 0;
uint32_t DAT_14013c92c = 0;
uint32_t DAT_14013c930 = 0;
uint8_t  DAT_14013c96a = 0;
uint8_t  DAT_14013d260 = 0xb4;
uint32_t DAT_14013c9f8 = 0;
void    *DAT_14013cfc8 = 0;
void    *DAT_14013cfd0 = 0;
void    *DAT_14013cfd8 = 0;
uint8_t  DAT_14013d300 = 0x10;
/* T1b: DAT_14013d328 独立定义已删除 — 与 core_globals.c 的 g_hHeap 是同一原版全局
 * (pecmd_defs.h 注释 + 原版 Ghidra xref 实锤: 仅 mainW@0x9ccc 赋值 +
 * attach 阶段 FUN_1400051b4@0x51fc 空守卫 GetProcessHeap)。
 * stub 侧经 stubs_common.h 的 #define 别名访问 g_hHeap; 双定义曾致 restored_bodies
 * 分配器拿到永远为 0 的句柄 (ntdll 分配路径 AV)。 */
uint32_t DAT_140147000 = 0x100;
void *DAT_14013d3b8 = 0;
int64_t DAT_14013a24f = 0;
int64_t DAT_14013a24c = 0;
longlong DAT_14013e118 = 0;
longlong DAT_14013e120 = 0;
longlong DAT_14013e128 = 0;
uint8_t DAT_14013c968 = 0;
uint64_t DAT_14013ca90 = 0;
short DAT_14013ca4c = 0;
const uint8_t DAT_14011dd24[] = "catch";
uint8_t DAT_14013cb09 = 0;
uint64_t DAT_14013c960 = 0;
LPCWSTR DAT_14013ca78;
int64_t DAT_14013ca80;
uint8_t DAT_14013d270;
int    DAT_14013ccca = 0;
longlong DAT_14013cfe0 = 0;
longlong DAT_14013d810 = 0;
longlong DAT_14013d818 = 0;
longlong DAT_14013d828 = 0;
longlong DAT_14013d830 = 0;
longlong DAT_14013d838 = 0;
longlong DAT_14013d840 = 0;
longlong DAT_14013d848 = 0;
longlong DAT_14013d850 = 0;
longlong DAT_14013d858 = 0;
uint64_t PTR_s_No_error_14013b800;
uint64_t PTR_DAT_14013a8f0;
uint64_t PTR_FUN_14011eb78;
uint64_t PTR_FUN_14011eb88;
uint64_t PTR_FUN_14011c410;
uint64_t PTR_DAT_14013bfe0;
uint64_t DAT_14013d660;
uint64_t DAT_14013d738;
uint64_t DAT_140147001;
uint64_t DAT_140147002;
uint64_t DAT_140147003;
uint64_t DAT_140147010;
uint64_t PTR_DAT_14013a250;
uint64_t PTR_FUN_140123540;
uint64_t PTR_FUN_140123560;
uint64_t PTR_FUN_140123580;
uint64_t PTR_FUN_1401235a8;
uint64_t PTR_FUN_140124e50;
uint64_t PTR_FUN_1401250d0;
uint64_t PTR_FUN_140126060;
uint64_t PTR_FUN_140126690;
uint64_t PTR_FUN_1401266c0;
uint64_t PTR_FUN_1401266e8;
uint64_t PTR_FUN_140126708;
uint64_t PTR_FUN_140126740;
uint64_t PTR_FUN_140126888;
uint64_t PTR_FUN_1401268a8;
uint64_t PTR_FUN_140128ed0;
uint64_t PTR_FUN_140129040;
uint64_t PTR_FUN_140129060;
uint64_t PTR_FUN_140129080;
uint64_t PTR_FUN_1401290a8;
uint64_t PTR_FUN_1401294f0;
uint64_t PTR_FUN_1401296e8;
uint64_t PTR_FUN_14012b1c0;
uint64_t PTR_FUN_14012b240;
uint64_t PTR_FUN_14012b430;
uint64_t PTR_FUN_14012bad0;
uint64_t PTR_FUN_14012bbf0;
uint64_t PTR_FUN_14012bd70;
uint64_t PTR_FUN_14012be90;
uint64_t PTR_FUN_14012bfb0;
uint64_t PTR_FUN_14012c0e0;
uint64_t PTR_FUN_14012c2e0;
uint64_t PTR_FUN_14012c410;
uint64_t PTR_FUN_14012c550;
uint64_t PTR_FUN_14012c670;
uint64_t PTR_FUN_14012c7b0;
uint64_t PTR_FUN_14012c900;
uint64_t PTR_FUN_14012ca50;
uint64_t PTR_FUN_14012cb90;
uint64_t PTR_FUN_14012ccc0;
uint64_t PTR_FUN_14012cde0;
uint64_t PTR_FUN_14012cf00;
uint64_t PTR__purecall_140126050;
uint64_t PTR_u_CallBackhWnd_14011e668_2_14013a288;
uint64_t PTR_u_Shell_14011ddb8_2_14013a290;
uint8_t  GetCommandLineW_exref[8];
uint64_t PTR_FUN_140126080;
uint64_t PTR_FUN_1401272a8;
uint64_t g_alphaThreshold;
uint64_t g_bInitWin;
double g_dpiBase;
uint64_t g_flagC96A;
uint64_t g_flagD032;
uint64_t g_flagD6f7;
double g_fontMinus0;
double g_fontRound;
uint64_t g_hCallBackWnd;
uint64_t g_hMainMutex;
uint64_t g_initFlags;
uint64_t g_langLen;
uint64_t g_logEnter;
uint64_t g_logFlag;
uint64_t g_msgboxActive;
uint64_t g_msgboxFlag;
uint64_t g_pLangBuf;
uint64_t g_pMainArgStr;
uint64_t g_paintScale2;
uint64_t g_pfnIsAppThemed;
uint64_t g_savedVtblSlot;
uint64_t g_subWndProc;
uint64_t g_threadId;
uint64_t PTR_FUN_140120a50[128];
uint64_t PTR_FUN_140120a60[128];
uint64_t PTR_FUN_140120a70[128];
uint64_t PTR_FUN_14013a020[4];
uint64_t PTR_s_VK_LBUTTON_140124140[512];
uint64_t PTR_WndProc1_14013a008[4];
void *DAT_14013a858;
uint64_t _UNK_1401265b8;
uint64_t PTR_FUN_140125be0;
uint64_t PTR_FUN_1401261e0;
uint64_t Ordinal_418_exref;
uint64_t PTR_DAT_14013a040;
int32_t  _DAT_14013d8a4;
uint8_t  PTR_FUN_140125d00[8];
uint8_t  DAT_14013d8a0[256];
uint8_t PTR_FUN_140126b20[8];
uint8_t s_ABCDEFGHIJKLMNOPQRSTUVWXYZabcdef_140124db0[128];
uint8_t PTR_FUN_1401268c8[8];
uint8_t DAT_140127738[8];
uint8_t DAT_140127740[8];
uint8_t DAT_00000011;
char *PTR_s___disverify_14013a2c8;
char *PTR_s__AutoDisverify_14013a2c0;
uint16_t u__SystemRoot___140123190[64];
uint64_t PTR_FUN_14011cbe8;
uint8_t DAT_140124100[64];
uint64_t DAT_14013a360;
int64_t   DAT_14013d870;
uint8_t PTR_FUN_140124f70[8];
uint8_t PTR_FUN_140125e20[8];
char s_0_0_0_0_140126958[16];
uint32_t DAT_14013c9fc;
uint64_t _UNK_14012d1d0 = 0;
uint64_t PTR_FUN_140126540;
uint64_t PTR_FUN_140126560;
uint64_t g_pFontBase;
uint64_t u_____C__140126e88 = 0x005C003F003F005CULL;
uint64_t ram0x000140126e90 = 0x00000000003A0043ULL;
void    *DAT_14013d388;
double   DAT_1401237e0;
double   DAT_1401237e8;
double   DAT_1401237f0;
double   DAT_1401237f8;
double   DAT_140123800;
double   DAT_140124110;
double   DAT_140124118 = 3.1415926535897932384626433832795;
double   DAT_1401263a0 = 10.0;
double   DAT_1401261a0 = 1.0;
double   DAT_140126398;
double   DAT_140126390;
double   DAT_140121668;
uint32_t DAT_14013a34c = 0x80000000;
uintptr_t g_hFontE2B8;
double DAT_140124120 = 1.5707963267948966192313216916398;
double DAT_1401268f0 = 180.0;
uint32_t DAT_14013a848;
uint32_t DAT_14013e1f0;
int64_t  DAT_14013e1e8;
int64_t  DAT_14013e1e0;
int64_t  DAT_14013e1d8;
int64_t  DAT_14013e1d0;
uint8_t  DAT_14011c638[64];
int DAT_14013d7fc = 0x22a6e;
code *DAT_14013e238;
uint8_t  DAT_14013cb41 = 0;
uint8_t  DAT_14013d261 = 0;
LPCWSTR DAT_14013ca10 = 0;
HWND  DAT_14013cf78 = 0;
ushort DAT_14013cf80 = 0;
longlong DAT_14013d4d8 = 0;
void *DAT_14013cf70 = 0;
void *DAT_14013e1f8 = 0;
void *DAT_14013e200 = 0;
void *DAT_14013e208 = 0;
uint64_t DAT_14013e268 = 0x000005b708001323fcULL;
uint64_t DAT_14013e280 = 0x000005b7e8001358ULL;
longlong DAT_14013e2a0 = 0;
longlong DAT_14013e298 = 0;
uint32_t DAT_14013e20c = 0;
void *DAT_14013d3e0;
uint8_t DAT_14013e1f5;
void *DAT_14013e458;

/* ==== B0/P3: function-pointer data definitions ==== */
void (*DAT_14013cd90)() = 0;
int    (*DAT_14013d800)(int) = 0;
void   (*DAT_14013d808)(void) = 0;
int    (*DAT_14013d820)(int) = 0;
void (*DAT_14013cb48)() = 0;
int  (*DAT_14013cd30)() = 0;
int  (*DAT_14013cd38)() = 0;
void (*DAT_14013cd40)() = 0;
void (*DAT_14013d380)(void *h, long long len) = 0;
int64_t (*DAT_14013d408)(HKEY, LPCWSTR, uint32_t, uint32_t) = 0;
int64_t (*DAT_14013ce08)(void) = 0;
int64_t (*DAT_14013ce10)(void) = 0;
int64_t (*DAT_14013ce28)(void) = 0;
int64_t (*DAT_14013ce30)(void) = 0;
void (*DAT_14013e290)(void *, void *, int *) = 0;
int (*DAT_14013cd48)(longlong, const unsigned short *, void *, void *, void *, void *) = 0;

/* B0/P3: tentative fn-ptr defs */
int (*DAT_14013c970)(uint32_t, uint32_t, char *, int) = 0;
void (*DAT_14013ce78)(HICON, float *, float *) = 0;


long long DAT_14013d130; /* B0/P3 fixer: definition for DAT_14013d130 */

/* B0/P3 fixer: definition for PTR_FUN_14011fb08 */
uint64_t PTR_FUN_14011fb08[128];
