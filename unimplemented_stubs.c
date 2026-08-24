/* unimplemented_stubs.c - B0/P3: FUN_/thunk_/DAT_/PTR_/gap-stub definitions. FOR LINK VERIFICATION ONLY. */
#include "stubs_common.h"


uint64_t FUN_140103020(const uint16_t *s) { (void)s; return 0; }
void *FUN_1400170b0(void **p) { (void)p; return 0; }
void *FUN_140070044(const char *s) { (void)s; return 0; }
int FUN_1400678f0(void *a, long long *b, short c) { (void)a;(void)b;(void)c; return 0; }
void FUN_1400284d4(long long *a, const void *b) { (void)a;(void)b; }
void FUN_140062950(void *a) { (void)a; }
void FUN_14001b888(uint64_t a) { (void)a; }    /* CoUninitialize 槽 (PECMD_GetApiProcCached("CoUninitialize",...) 装载) */
uint64_t DAT_14013e168[8] = {0}; /* CRITICAL_SECTION COM */
uint64_t DAT_14013e190[8] = {0};  /* g_runFlag 0x140147000: 00 01 00 00 */
/* PTR_u 标识符字符短串指针表 (.rdata, NULL 结尾): pe_data_extract 14013a2d0 */
uint64_t PTR_u_a__a_zA_Z0_9___14013a2d0[] = {
    0x1401233c0, 0x1401233e0, 0x1401233f8, 0x140123410, 0x140123428,
    0x140123448, 0x140123460, 0x140123490, 0x1401234b0, 0
};   /* arity 修正 0->3 (PECMD_ToSysCopyFiles 直移) */
uint64_t PECMD_RunCommandLine(void *script, WCHAR **str, int mode) { (void)script;(void)str;(void)mode; return 0; }                                           /* def @6907 */
     /* 新增桩 @0x1400c10c0 */
const uint8_t DAT_14011dcb0[16] = {0x2a,0,0,0,'p','p','i','d',':',0,0,0,'h','p','i','d'}; /* 新增桩 */
const uint16_t DAT_14011d108[] = {'.','t','m','p',0};   /* argv 表末端字节偏移 (初 0) */
void FUN_1400166b4(void) { /* 初始化 argv 表 (缺失 helper, no-op 桩) */ }
uint64_t PECMD_ScriptInit(void) { return 0; }
uint64_t PECMD_WaitHandlesOrMessages(uint64_t param_1, int64_t param_2, int param_3, uint64_t *param_4) { (void)param_1;(void)param_2;(void)param_3;(void)param_4; return 0; }

uint64_t PECMD_XorEncode(const uint16_t *a, uint32_t b, uint64_t c) { (void)a;(void)b;(void)c; return 0; }
uint64_t PECMD_InstallKeyboardHook(void) { return 0; }
uint64_t PECMD_ExpandDrivePathAlloc(void) { return 0; }
uint64_t PECMD_AddVarDefault(void *script, LPCWSTR name, LPCWSTR val, int len, int64_t flag) { (void)script;(void)name;(void)val;(void)len;(void)flag; return 0; }   /* arity 修正 0->5 (PECMD_ExecCmdDispatch 恢复体) */
int64_t PECMD_FindVarValue(int64_t *a, LPCWSTR b, int64_t *c, int d) { (void)a;(void)b;(void)c;(void)d; return 0; }   /* arity 修正 0->4 (PECMD_ExecCmdDispatch 恢复体) */
uint64_t PECMD_SetVarCore(void) { return 0; }
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

int PECMD_AsciiPrefixICmp(const char *a, const uint16_t *w, int n) { (void)a;(void)w;(void)n; return 0; }
uint64_t PECMD_AsciiWideICmp(const char *a, const uint16_t *b) { (void)a;(void)b; return 0; }


int64_t PECMD_EnableTokenPrivilege(LPCWSTR a, DWORD b, uint32_t c) { (void)a;(void)b;(void)c; return 0; }

void PECMD_DialogBeepNotify(int64_t a, int b) { (void)a;(void)b; }
uint64_t PECMD_NextRandomSeed(void) { return 0; }
void PECMD_SetVariable(void *a, const WCHAR *b, const WCHAR *c) { (void)a;(void)b;(void)c; }


uint64_t PECMD_ExpandDrivePath(const uint16_t *a, uint64_t b, uint16_t *c, longlong *d) { (void)a;(void)b;(void)c;(void)d; return 0; }
long long FUN_140064a34(uint16_t *s) { (void)s; return 0; }
uint64_t PECMD_EncodeStringId(LPCWSTR name, uint64_t *out, char mode) { (void)name;(void)out;(void)mode; return 0; }
void PECMD_StrDupAssign(uint16_t **ps, const uint16_t *src) { (void)ps;(void)src; }
void PECMD_HeapFreeWithHeader(long long *a) { (void)a; }   /* @0x14005b0d4 串槽释放 (leaf stub), 原体见 decompiled.c:53617 */
uint64_t PECMD_ParsePrefixColon(void) { return 0; }
long long PECMD_ExpandCommandLine(long long *a, WCHAR *b, void *c, int d, uint8_t e) { (void)a;(void)b;(void)c;(void)d;(void)e; return 0; }
long long PECMD_ExpandVarsRecursive(long long *a, WCHAR *b, void *c, int d, uint8_t e) { (void)a;(void)b;(void)c;(void)d;(void)e; return 0; }
uint64_t PECMD_SetVariableWithPrefix(void) { return 0; }
uint64_t PECMD_SetCheckVariable(void) { return 0; }
uint64_t PECMD_NotifyMainWindowRefresh(uint64_t a, int b) { (void)a;(void)b; return 0; }   /* arity 修正 0->2 (core 调用方 + PECMD_ProcessScriptBlock 移入) */
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

uint64_t PECMD_ScriptCopy(void) { return 0; }
uint64_t PECMD_ParseCommandPath(uint64_t a, uint64_t b, uint32_t *c, int64_t *d) { (void)a;(void)b;(void)c;(void)d; return 0; }   /* arity 修正 0->4 (core 调用方 + PECMD_ProcessScriptBlock 移入) */
uint64_t FUN_140061E98(void) { return 0; }
uint64_t FUN_1400688E0(void) { return 0; }
uint64_t FUN_14006A81C(void) { return 0; }
uint64_t PECMD_CopyPathToken(longlong a, longlong *b, longlong *c, longlong d) { (void)a;(void)b;(void)c;(void)d; return 0; }

/* FUN_ helper 无操作桩 */
uint64_t FUN_140001188(void) { return 0; }
uint64_t FUN_14000C764(void) { return 0; }
uint64_t PECMD_ResizeBuffer(void) { return 0; }
uint64_t PECMD_GetWindowObjectRef(void) { return 0; }
uint64_t PECMD_EncodeDet(long long a, uint64_t b) { (void)a;(void)b; return 1; }
uint64_t PECMD_ParseHashNumbers(void) { return 0; }
uint64_t PECMD_GetComboItemText(void) { return 0; }
uint64_t PECMD_SaveSelectionToVar(void) { return 0; }
uint64_t PECMD_SkipWCharUntil(void *pp, uint16_t ch) { (void)pp;(void)ch; return 0; }
uint64_t PECMD_InitDragDrop(void) { return 0; }
uint64_t PECMD_IsSetupClass(void) { return 0; }
uint64_t PECMD_SetControlState(void) { return 0; }
uint64_t PECMD_ShowContextMenu(void) { return 0; }
LRESULT PECMD_GetControlFont(int64_t param_1) { (void)param_1; return 0; }
int PECMD_UpdateWindowStyleBits(int64_t a, unsigned int b, uint64_t c) { (void)a;(void)b;(void)c; return 0; }
uint64_t FUN_1400ec698(int64_t a, uint64_t b) { (void)a;(void)b; return 0; }

void PECMD_LayoutChildWindows(int64_t a, unsigned char b) { (void)a;(void)b; }
uint64_t PECMD_InitWindowObjectF(void) { return 0; }



uint64_t PECMD_CreateDateTimePickCtl(void) { return 0; }
uint64_t PECMD_InitControlObjC(void) { return 0; }

void PECMD_CreateStaticControlFromRect(int64_t *a, const void *b, unsigned int c, uint64_t *d, int64_t e, unsigned int f) { (void)a;(void)b;(void)c;(void)d;(void)e;(void)f; }
uint64_t *PECMD_InitStaticCtl(uint64_t *a, uint64_t b) { (void)a;(void)b; return a; }
uint64_t PECMD_CreateProgressCtl(void) { return 0; }
uint64_t thunk_FUN_140064b78(void) { return 0; }
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
void FUN_140102a90(uint64_t *dst, uint64_t v, uint64_t n){ (void)dst;(void)v;(void)n; }	/* CRT 标准库内联 */
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
int PECMD_CmdKill() { }
int PECMD_DispatchSystemCommandLine() { }   /* arity 修正 0->2 (PECMD_ExecCmdDispatch 恢复体) */

/* ---- B3 还原批次 (core_b3r_*.c) 依赖补充 ---- */
uint64_t PECMD_ManualMapPeImage(int *a, int16_t *b, uint64_t c){ (void)a;(void)b; return c; }
/* ---- P0 补缺失符号桩 ---- */
/* ---- 批1-01ed5c 依赖桩 ---- */
uint64_t FUN_14001ebdc(void){ return 0; }
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
uint16_t *FUN_14000531c(uint16_t *s) { (void)s; return s; }
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
const uint16_t *FUN_14001b23c(int64_t a, void *b, const uint16_t *c, void *d, char e) { (void)a;(void)b;(void)d;(void)e; return c; }

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
int      FUN_14005bc28(uint16_t a) { (void)a; return 0; }      /* 字符分类: 返回0表示字符合法 */
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
void     FUN_140063888(void *a, const WCHAR *b, int64_t c) { (void)a;(void)b;(void)c; }
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
longlong FUN_140003a20(longlong *param_1, undefined8 *param_2, byte param_3) { (void)param_1;(void)param_2;(void)param_3; return 0; }
undefined8 FUN_1400748a0(HWND param_1, undefined8 *param_2) { (void)param_1;(void)param_2; return 1; }
void       FUN_1400f172c(longlong *a, UINT b, ulonglong c, ulonglong *d, longlong e, int f, undefined8 *g) { (void)a;(void)b;(void)c;(void)d;(void)e;(void)f;(void)g; }   /* 0x14013e290 原始: 48 b8 05 00 86 b8 05 00 (RTL_UTF8... 文本转储回调表) */
void  FUN_140063720(longlong **_ps, longlong len) { (void)_ps;(void)len; }

/* ---- wave-current support: 018220/018c6c/01b3a0/01b888 deps ---- */
undefined8 FUN_14005c788(char *a, ushort *b, int c) { (void)a;(void)b;(void)c; return 0; }   /* 模块名前缀判定 (leaf stub) */
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
void FUN_1400e6860(WPARAM a1, int a2) { (void)a1;(void)a2; }
void FUN_14006703c(longlong obj) { (void)obj; }
ulonglong FUN_1400b1724(LARGE_INTEGER li, ulonglong a2) { (void)a2; return li.QuadPart; }
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
int FUN_1400633a8() { }
void FUN_1400f527c(longlong self, HDC dc, void *attr, LPRECT rc)                      /* 焦点框覆盖绘制 */
    { (void)self;(void)dc;(void)attr;(void)rc; }
undefined4 FUN_1400ede48(HDC dc, HICON ic, undefined8 a3, int a4, ulonglong a5, ulonglong a6,
                         undefined4 a7, undefined4 a8, ulonglong a9, ulonglong a10, COLORREF a11)
    { (void)dc;(void)ic;(void)a3;(void)a4;(void)a5;(void)a6;(void)a7;(void)a8;(void)a9;(void)a10;(void)a11; return 0; }

void FUN_140066eac(longlong a1, int a2) { (void)a1;(void)a2; }
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
