/* ====================================================================
 * core_b3n.c — B3-B6 小函数批 10（主代理并行批次，3 个）
 *
 * 本批新实现函数全部使用人类可读 PECMD_ 名称，原始地址保留在 @0x 注释。
 *
 * 来源: PECMD原始.EXE (x64, ImageBase=0x140000000)
 *   统计换行数       FUN_140060B5C @0x140060b5c
 *   查询磁盘几何     FUN_140065EFC @0x140065efc
 *   查询注册表串     FUN_1400690C0 @0x1400690c0
 *
 * 约定:
 *   - 新实现函数使用 PECMD_ 可读名；未实现依赖仍 extern FUN_ + TODO(verify)
 *   - 全局数据符号先用 extern 声明; 完整链接时统一在 core_globals.c 定义
 * ==================================================================== */
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>

#include "pecmd_defs.h"

/* ---- 未实现依赖 (extern + TODO(verify)) ---- */
extern uint64_t *PECMD_GrowByteBuffer(uint64_t *out, int64_t size);

/* ========== FUN_140060B5C @0x140060b5c ==========
 * 统计指定范围内的换行/模式出现次数。
 */
int FUN_140060B5C(uint64_t *text, int start, int mode, int end)
{
    uint64_t *puVar1 = (uint64_t *)((uint8_t *)text + (int64_t)start * 2);
    int count = 1;
    int stride = mode >> 8;
    uint64_t *puVar3 = (uint64_t *)((uint8_t *)puVar1 - 2);
    char step = 1;
    uint64_t *puVar5 = text;
    if ((char)mode < 1) {
        puVar3 = (uint64_t *)((uint8_t *)text + (int64_t)end * 2 - 2);
        step = -1;
        text = puVar3;
        puVar5 = puVar1;
    }
    if (stride < 3) {
        for (; (text <= puVar3 && (puVar5 <= text));
             text = (uint64_t *)((uint8_t *)text + (int64_t)step * 2)) {
            if ((int16_t)*text == 10) {
                count++;
            }
        }
    }
    else {
        step = (char)((uint32_t)mode >> 8) * step;
        puVar3 = (uint64_t *)((uint8_t *)puVar3 + (int64_t)stride * -2 + 2);
        if ((char)mode < 1) {
            puVar3 = (uint64_t *)((uint8_t *)puVar5 +
                                  (((int64_t)((end / stride) * stride) - stride) - start) * 2);
            text = puVar3;
        }
        if (stride == 3) {
            for (; (text <= puVar3 && (puVar5 <= text));
                 text = (uint64_t *)((uint8_t *)text + (int64_t)step * 2)) {
                if ((*text & 0xffffffffffffULL) == 0x4100300020ULL) {
                    count++;
                }
            }
        }
        else {
            for (; (text <= puVar3 && (puVar5 <= text));
                 text = (uint64_t *)((uint8_t *)text + (int64_t)step * 2)) {
                if (((int16_t)text[1] == 0x41) && (*text == 0x30003000300020ULL)) {
                    count++;
                }
            }
        }
    }
    return count;
}

/* ========== FUN_140065EFC @0x140065efc ==========
 * 获取磁盘几何信息。
 */
uint32_t FUN_140065EFC(LPCWSTR path, HANDLE hFile)
{
    HANDLE hObject = 0;
    HANDLE pvVar1 = 0;
    DWORD local_res10[2] = {0, 0};
    uint32_t local_48 = 0;
    uint32_t local_44 = 0;
    uint32_t local_40 = 0;
    (void)pvVar1;
    (void)local_44;
    (void)local_40;
    uint32_t local_38 = 0;
    uint32_t local_34[6];
    uint32_t local_1c = 0;

    if (hFile == (HANDLE)0) {
        hObject = CreateFileW(path, 0x80000000, 3, NULL, 3, 0, (HANDLE)0);
        if (hObject == INVALID_HANDLE_VALUE) {
            hObject = 0;
        }
        hFile = hObject;
        if ((hObject == 0) && (hObject = CreateFileW(path, 0, 3, NULL, 3, 0, (HANDLE)0),
                               hFile = hObject, hObject == INVALID_HANDLE_VALUE)) {
            hObject = 0;
            hFile = 0;
        }
    }
    if ((hFile == INVALID_HANDLE_VALUE) || (hFile == (HANDLE)0)) {
        if ((hObject != 0) && (hObject != INVALID_HANDLE_VALUE)) {
            CloseHandle(hObject);
        }
        local_1c = 0xffffffff;
    }
    else {
        local_48 = 0;
        local_44 = 0;
        local_40 = 0;
        local_res10[0] = 0;
        local_38 = 0;
        memset(local_34, 0, 0x24);
        local_34[0] = 0x28;
        DeviceIoControl(hFile, 0x2d1400, &local_48, 0xc, &local_38, 0x28, local_res10, NULL);
        if ((hObject != 0) && (hObject != INVALID_HANDLE_VALUE)) {
            CloseHandle(hObject);
        }
    }
    return local_1c;
}

/* ========== FUN_1400690C0 @0x1400690c0 ==========
 * 读取注册表字符串值。
 */
int FUN_1400690C0(HKEY root, LPCWSTR subkey, LPCWSTR name, int64_t *out, DWORD *size, LONG *status)
{
    LONG LVar2;
    int64_t lVar1;
    int64_t lVar3;
    int result = 0;
    DWORD local_28[2] = {0xffffffff, 0};
    HKEY local_20 = (HKEY)0;

    LVar2 = RegOpenKeyExW(root, subkey, 0, 0x20019, &local_20);
    if (LVar2 == 0) {
        *size = 0xffa;
        PECMD_GrowByteBuffer((uint64_t *)out, 0x1000);
        *(uint8_t *)*out = 0;
        LVar2 = RegQueryValueExW(local_20, name, NULL, local_28, (BYTE *)*out, size);
        if (LVar2 == 0xea) {
            *size = *size + 10;
            PECMD_GrowByteBuffer((uint64_t *)out, (int64_t)(int)(*size + 6));
            LVar2 = RegQueryValueExW(local_20, name, NULL, local_28, (BYTE *)*out, size);
        }
        RegCloseKey(local_20);
        result = -1;
        if (LVar2 == 0) {
            if ((int)*size < 0) {
                *size = 0;
            }
            *(uint8_t *)((int)*size + *out) = 0;
            lVar3 = (int64_t)(int)(((int)*size + 1U) >> 1);
            lVar1 = *out;
            *(uint16_t *)(lVar1 + 2 + lVar3 * 2) = 0;
            *(uint16_t *)(lVar1 + lVar3 * 2) = 0;
            PECMD_GrowByteBuffer((uint64_t *)out, (int64_t)(int)(*size + 5));
            return (int)local_28[0];
        }
    }
    *size = 0;
    if (status != NULL) {
        *status = LVar2;
    }
    return result - 1;
}
