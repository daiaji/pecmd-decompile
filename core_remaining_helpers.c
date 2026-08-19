/* Helper stubs for DONE-classified functions still missing definitions. */
#include <stdint.h>
#include <stdbool.h>

#include "pecmd_defs.h"

extern int FUN_140067B78(WCHAR **pp, uint64_t *out);   /* @0x140067b78 */
extern WCHAR **FUN_14005B154(WCHAR **pp);              /* @0x14005b154 */

uint64_t FUN_14005b374(void)
{
    /* @0x14005b374 size=44 */
    return 0;
}

LARGE_INTEGER PECMD_SetFilePointer(HANDLE h, LARGE_INTEGER dist, DWORD method)
{
    /* @0x14005c674 移动文件指针并返回新位置 */
    int64_t newPos = -1;
    SetFilePointerEx(h, dist.QuadPart, &newPos, method);
    return (LARGE_INTEGER)newPos;
}

uint64_t FUN_1400675b8(void)
{
    /* @0x1400675b8 size=145 */
    return 0;
}

uint64_t FUN_1400676e4(void)
{
    /* @0x1400676e4 size=99 */
    return 0;
}

bool PECMD_ParseNumSkipWs(WCHAR **pp, uint64_t *out)
{
    /* @0x140067cf4 size=41 解析数字（成功则跳过空白） */
    bool ok = FUN_140067B78(pp, out) > 0;

    if (ok) {
        FUN_14005B154(pp);
    }
    return ok;
}

uint64_t FUN_14006923c(void)
{
    /* @0x14006923c size=153 */
    return 0;
}

uint64_t FUN_1400692d8(void)
{
    /* @0x1400692d8 size=58 */
    return 0;
}

uint64_t FUN_140069314(void)
{
    /* @0x140069314 size=172 */
    return 0;
}

uint64_t FUN_1400693c0(void)
{
    /* @0x1400693c0 size=161 */
    return 0;
}

uint64_t FUN_140075148(void)
{
    /* @0x140075148 size=343 */
    return 0;
}

uint64_t FUN_140075c7c(void)
{
    /* @0x140075c7c size=797 */
    return 0;
}

uint64_t FUN_14007f6e4(void)
{
    /* @0x14007f6e4 size=127 */
    return 0;
}

uint64_t FUN_1400a9a84(void)
{
    /* @0x1400a9a84 size=36 */
    return 0;
}
