/* ====================================================================
 * core_b8_remaining.c — B8 最后 5 个大函数简化实现
 *
 * 这些函数体量很大（2K-9K），本轮先补可编译、可链接的简化实现，
 * 保留原始 @0x 地址和 TODO(verify)，后续可读化深化阶段再逐行还原。
 *
 * 来源: PECMD原始.EXE (x64, ImageBase=0x140000000)
 *   FUN_1400E1228      @0x1400e1228  (简化桩)
 *   FUN_1400E9724       @0x1400e9724  (简化桩)
 *   FUN_1400F36EC  @0x1400f36ec  (简化桩)
 *   FUN_1400F6DB0     @0x1400f6db0  (简化桩)
 *   FUN_1400F94A0    @0x1400f94a0  (简化桩)
 * ==================================================================== */
#include <stdint.h>
#include <string.h>

#include "pecmd_defs.h"
/* ========== FUN_1400E1228 @0x1400e1228 ==========
 * [简化桩] 原函数为 8K 大函数，按签名返回 0。
 * TODO(verify): 需完整还原脚本执行逻辑。
 */
LARGE_INTEGER FUN_1400E1228(LARGE_INTEGER script, WCHAR *cmd, WPARAM wParam)
{
    LARGE_INTEGER r;
    (void)script;
    (void)cmd;
    (void)wParam;
    r.QuadPart = 0;
    return r;
}

/* ========== FUN_1400E9724 @0x1400e9724 ==========
 * [简化桩] 原函数为 9K 大函数，按签名返回 0。
 * TODO(verify): 需完整还原 Ramdriv 解析逻辑。
 */
uint64_t FUN_1400E9724(LPCWSTR cmd, int64_t *out)
{
    (void)cmd;
    (void)out;
    return 0;
}

/* ========== FUN_1400F36EC @0x1400f36ec ==========
 * [简化桩] 原函数为列表列布局，先返回。
 * TODO(verify): 需完整还原列宽/滚动映射。
 */
void FUN_1400F36EC(int64_t obj, int index, int height)
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

/* ========== FUN_1400F94A0 @0x1400f94a0 ==========
 * [简化桩] 原函数为列表视图命中测试，返回 {0,0}。
 * TODO(verify): 需完整还原命中测试逻辑。
 */
POINT FUN_1400F94A0(HDC obj, uint32_t msg, POINT *pt, POINT coord)
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
