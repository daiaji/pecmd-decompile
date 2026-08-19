/* ====================================================================
 * core_b7c.c — B7 中批 (1400c0000-1400dffff) 简化实现
 *
 * 这些函数体量较大且依赖复杂（文件哈希/控件创建/列表项添加），
 * 本轮先补可编译、可链接的简化实现，保留原始 @0x 地址和 TODO(verify)。
 *
 * 来源: PECMD原始.EXE (x64, ImageBase=0x140000000)
 *   FUN_1400C0AD8       @0x1400c0ad8  (简化桩)
 *   FUN_1400C3820   @0x1400c3820  (简化桩)
 *   FUN_1400C3CF8   @0x1400c3cf8  (简化桩)
 *   FUN_1400C5300    @0x1400c5300  (简化桩)
 *   FUN_1400C56FC  @0x1400c56fc  (简化桩)
 * ==================================================================== */
#include <stdint.h>

#include "pecmd_defs.h"

/* ========== FUN_1400C0AD8 @0x1400c0ad8 ==========
 * [简化桩] 读取文件并计算哈希。返回 NULL。
 * TODO(verify): 需完整还原文件映射/哈希算法。
 */
void *FUN_1400C0AD8(int64_t *a1, LPCWSTR a2, int64_t *a3)
{
    (void)a1;
    (void)a2;
    (void)a3;
    return NULL;
}

/* ========== FUN_1400C3820 @0x1400c3820 ==========
 * [简化桩] 创建 updown 控件对象。直接返回 a1。
 * TODO(verify): 需完整还原控件创建/样式计算。
 */
uint64_t *FUN_1400C3820(uint64_t *a1, int64_t a2, uint32_t a3,
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

/* ========== FUN_1400C3CF8 @0x1400c3cf8 ==========
 * [简化桩] 创建滚动条控件对象。直接返回 a1。
 * TODO(verify): 需完整还原滚动条创建/范围设置。
 */
uint64_t *FUN_1400C3CF8(uint64_t *a1, int64_t a2, uint32_t a3,
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

/* ========== FUN_1400C5300 @0x1400c5300 ==========
 * [简化桩] 向列表控件添加项。返回 0。
 * TODO(verify): 需完整还原参数解析/控件消息。
 */
uint64_t FUN_1400C5300(int64_t *a1, uint16_t *a2, WPARAM a3,
                               uint64_t a4)
{
    (void)a1;
    (void)a2;
    (void)a3;
    (void)a4;
    return 0;
}

/* ========== FUN_1400C56FC @0x1400c56fc ==========
 * [简化桩] 向列表控件添加项（带 -color 前缀解析）。返回 0。
 * TODO(verify): 需完整还原参数解析/颜色处理。
 */
uint64_t FUN_1400C56FC(int64_t *a1, uint16_t *a2, WPARAM a3,
                                 uint64_t a4)
{
    (void)a1;
    (void)a2;
    (void)a3;
    (void)a4;
    return 0;
}

/* ========== FUN_1400C5BA8 @0x1400c5ba8 ==========
 * [简化桩] 解析文件属性命令。返回 0。
 * TODO(verify): 需完整还原属性/时间解析。
 */
int64_t FUN_1400C5BA8(int64_t *a1, int64_t *a2, LPCWSTR a3,
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

/* ========== FUN_1400D0468 @0x1400d0468 ==========
 * [简化桩] 文件时间转 POINT。返回 {0,0}。
 * TODO(verify): 需完整还原时间转换。
 */
POINT FUN_1400D0468(int64_t *a1, FILETIME a2, uint64_t a3)
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

/* ========== FUN_1400DF464 @0x1400df464 ==========
 * [简化桩] 下载 URL 到文件。返回 0。
 * TODO(verify): 需完整还原下载/缓存逻辑。
 */
DWORD FUN_1400DF464(LPCWSTR a1, uint32_t a2, LPCWSTR a3,
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

/* ========== FUN_1400C13F8 @0x1400c13f8 ==========
 * [简化桩] 注册表复制。返回 a1。
 * TODO(verify): 需完整还原递归复制。
 */
HKEY FUN_1400C13F8(HKEY a1, HKEY a2, char a3)
{
    (void)a2;
    (void)a3;
    return a1;
}

/* ========== FUN_1400C493C @0x1400c493c ==========
 * [简化桩] 添加树控件项。返回 0。
 * TODO(verify): 需完整还原树节点添加。
 */
uint64_t FUN_1400C493C(int64_t *a1, int64_t *a2, LPCWSTR a3,
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

/* ========== FUN_1400C6324 @0x1400c6324 ==========
 * [简化桩] 执行脚本行。返回 NULL。
 * TODO(verify): 需完整还原脚本执行。
 */
void *FUN_1400C6324(WCHAR *a1, void *a2, uint64_t a3,
                             WCHAR *a4, char a5)
{
    (void)a1;
    (void)a2;
    (void)a3;
    (void)a4;
    (void)a5;
    return NULL;
}

/* ========== FUN_1400C9B9C @0x1400c9b9c ==========
 * [简化桩] 创建列表控件。返回 a1。
 * TODO(verify): 需完整还原列表创建。
 */
uint64_t *FUN_1400C9B9C(uint64_t *a1, uint64_t a2, uint64_t a3,
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

/* ========== FUN_1400CAF78 @0x1400caf78 ==========
 * [简化桩] 列表控件命令。返回 NULL。
 * TODO(verify): 需完整还原命令处理。
 */
uint16_t *FUN_1400CAF78(int64_t *a1, LPCWSTR a2, WPARAM a3)
{
    (void)a1;
    (void)a2;
    (void)a3;
    return NULL;
}

/* ========== FUN_1400CB820 @0x1400cb820 ==========
 * [简化桩] 列表命中测试。返回 {0,0}。
 * TODO(verify): 需完整还原命中测试。
 */
POINT FUN_1400CB820(int64_t *a1, POINT a2, uint64_t a3, char a4)
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

/* ========== FUN_1400CD3A8 @0x1400cd3a8 ==========
 * [简化桩] 解析变量命令。返回 0。
 * TODO(verify): 需完整还原变量命令解析。
 */
int64_t FUN_1400CD3A8(uint64_t a1, WCHAR *a2)
{
    (void)a1;
    (void)a2;
    return 0;
}

/* ========== FUN_1400D0C6C @0x1400d0c6c ==========
 * [简化桩] 执行命令 A。返回 {0}。
 * TODO(verify): 需完整还原执行逻辑。
 */
LARGE_INTEGER FUN_1400D0C6C(int64_t *a1, LARGE_INTEGER a2)
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

/* ========== FUN_1400D5B48 @0x1400d5b48 ==========
 * [简化桩] 判断点是否在控件内。返回 0。
 * TODO(verify): 需完整还原命中判断。
 */
char FUN_1400D5B48(int64_t a1, HDC a2)
{
    (void)a1;
    (void)a2;
    return 0;
}

/* ========== FUN_1400D7038 @0x1400d7038 ==========
 * [简化桩] 解析控件规格。返回 0。
 * TODO(verify): 需完整还原规格解析。
 */
uint64_t FUN_1400D7038(int64_t *a1, WCHAR *a2)
{
    (void)a1;
    (void)a2;
    return 0;
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

/* ========== FUN_1400DC9FC @0x1400dc9fc ==========
 * [简化桩] 执行命令 C。返回 {0}。
 * TODO(verify): 需完整还原执行逻辑。
 */
LARGE_INTEGER FUN_1400DC9FC(LARGE_INTEGER a1, LARGE_INTEGER a2,
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

/* ========== FUN_1400DFB14 @0x1400dfb14 ==========
 * [简化桩] 文件时间命令。返回 {0}。
 * TODO(verify): 需完整还原时间命令。
 */
FILETIME FUN_1400DFB14(int64_t *a1, FILETIME a2, WPARAM a3)
{
    FILETIME r;
    (void)a1;
    (void)a2;
    (void)a3;
    r.dwLowDateTime = 0;
    r.dwHighDateTime = 0;
    return r;
}
