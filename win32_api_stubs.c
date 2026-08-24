/* win32_api_stubs.c - B0/P3: Win32-API-shaped no-op stubs. FOR LINK VERIFICATION ONLY. */
#include "stubs_common.h"

                        /* 传 PECMD_ExpandCommandLine 输出槽 (后部定义) */

/* --- (原型置于此, 匹配后部定义; 消除与先前隐式 int() 的 conflicting types) --- */
/* CreateFileMappingW: 签名升级 0->6 参 (FUN_1400e3e38 文件映射辅助直移需要; 体保持桩, 同 win32_stub.h) */
uint64_t DefWindowProcW_exref(void) { return 0; }
uint64_t ExitWindowsEx_exref(void) { return 0; }
/* FormatMessageW 库内原缺 → 新增最小桩 (返回 0 → 调用方走 "\r\n" 兜底路径) */
/* advapi32 安全描述符 API 桩 (R1 批新增: PECMD_InitNullDaclSD 真体依赖, 库内原无) */
uint64_t GetThreadContext_exref(void) { return 0; }
uint64_t LoadEnvi(const uint16_t *a, const uint16_t *b) { (void)a;(void)b; return 0; }   /* arity 修正 0->2 (PECMD_ScriptMainEntry 恢复体) */
/* arity 修正 0->5 (PECMD_ExecCmdDispatch 恢复体 PECMD_MsgWaitForObjects 内联展开) */
uint64_t SetThreadContext_exref(void) { return 0; }
uint64_t WinExec_exref(void) { return 0; }

/* P0-2: _snwprintf/_vsnwprintf — modern UCRT exports no import symbol for them
 * nor for vswprintf (all header-inline over the core). Forward to the exported
 * core __stdio_common_vswprintf (mode=0). NOTE: legacy-crt truncation semantics
 * differ slightly; registered in known_limits. Do NOT include <stdio.h>. */
extern unsigned long long __stdcall __stdio_common_vswprintf(
    unsigned long long mode, uint16_t *buf, size_t cnt,
    const uint16_t *fmt, void *locale, va_list ap);

int _vsnwprintf(uint16_t *buf, size_t cnt, const uint16_t *fmt, va_list ap)
{
    return (int)__stdio_common_vswprintf(0, buf, cnt, fmt, 0, ap);
}

int _snwprintf(uint16_t *buf, size_t cnt, const uint16_t *fmt, ...)
{
    va_list ap;
    int r;
    va_start(ap, fmt);
    r = (int)__stdio_common_vswprintf(0, buf, cnt, fmt, 0, ap);
    va_end(ap);
    return r;
}
uint64_t _UNK_140121ff6(void) { return 0; }
void *operator_new(uint64_t n) { (void)n; return 0; }
uint64_t ram0x000140120a48(void) { return 0; }
uint64_t u__26_INDATA_140121fe0(void) { return 0; }
uint64_t u_____D__140120a40(void) { return 0; }

/* ---- 补充桩 (B1/B3 重构新增引用) ---- */

/* ---- 追加桩: batch25 还原(140017b8c/00d2a0/0192d0/004ac4 及 B3 13 函数)引用的未定义符号 ---- */
/* Win32 / shlwapi API 桩 */

/* --- r34 follow-up: Win32 API stubs (no real impl) --- */
/* --- r39 follow-up stubs (main-agent closure) --- */

/* ---- Tier 依赖预补 ---- */

/* Win32 API 补充 */


/* --- batch r42: B1/B3/B9 remaining restore link stubs --- */
/* Win32 / SetupAPI */

/* --- batch28 restored-function deps --- */

/* RegLoadKeyW / RegUnLoadKeyW — Advapi32 直接导入, 原 refactor 未引用故缺桩 (house no-op 模式) */

/* ============================================================
   ---- 新增叶依赖桩 (FUN_14005ccb0 SetupDi 设备变更恢复所需) ----
   ============================================================ */
/* SetupDiClassNameFromGuidA: setupapi.lib 真 API, MSVC 链接走系统实现 (P0-2 裁剪) */

/* 图标资源 (FUN_140073934) 缺失 WinAPI 桩 */
/* ---- 075c7c/00cedc 依赖 ---- */
/* LocalFree — 安全描述符/ACL 释放 (PECMD_SetAclPermission 使用), 声明于 win32_stub.h, 原无定义 */
/* WinAPI 桩: 资源串读取 / CRT atexit */



/* ---- wave-current support: 008834 ---- */
/* 以下桩在本文件原为 (void) 签名, 按带参调用点就地修正 (仅本文件内可见调用): */