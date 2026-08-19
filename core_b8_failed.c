/* ====================================================================
 * core_b8_failed.c — B8 decompile-failed 占位桩
 *
 * 这两个函数在 decompiled.c 中只有签名 + [DECOMPILE FAILED]，没有
 * 可重构的 C 伪代码体。此处先补正确签名的占位实现，保证后续完整链接
 * 不出现 undefined；语义留待二进制反汇编专项处理。
 *
 * 来源: PECMD原始.EXE (x64, ImageBase=0x140000000)
 *   FUN_1400e4864 @0x1400e4864  (decompile-failed 桩)
 *   FUN_1400fdad8 @0x1400fdad8  (decompile-failed 桩)
 * ==================================================================== */
#include <stdint.h>

#include "pecmd_defs.h"

typedef uint32_t ALG_ID;

/* ========== FUN_1400e4864 @0x1400e4864 ==========
 * [DECOMPILE FAILED] 占位桩。TODO(verify): 需从二进制反汇编补真实实现。
 */
DWORD FUN_1400e4864(BYTE *data, DWORD len, ALG_ID alg, uint64_t extra)
{
    /* UNIMPLEMENTED @0xFUN_1400e4864 — decompile-failed, body 未还原 */
(void)data;
    (void)len;
    (void)alg;
    (void)extra;
    return 0;
}

/* ========== FUN_1400fdad8 @0x1400fdad8 ==========
 * [DECOMPILE FAILED] 占位桩。TODO(verify): 需从二进制反汇编补真实实现。
 * 反编译代码虽无法展开，但调用方以其 WndProc 消息形参加入（obj,msg,wParam,lParam）。
 */
void FUN_1400fdad8(void *param_1, int param_2, int64_t param_3, uint64_t param_4)
{
    /* UNIMPLEMENTED @0xFUN_1400fdad8 — decompile-failed, body 未还原 */
(void)param_1; (void)param_2; (void)param_3; (void)param_4;
}
