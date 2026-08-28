/* ====================================================================
 * core_var.c — PECMD 内置变量设置族
 *
 * 来源: PECMD原始.EXE (x64)
 *   FUN_1400629B8     @0x1400629b8   (变量设置核心: & 前缀/脚本标志分支)
 *   PECMD_SetVarAndEnvSync  @0x140062a2c   (SetVar + SetEnvironmentVariableW(key+2))
 *   PECMD_AllocWStringBuffer  @0x140063694   (缓冲分配变体, size=count*2+2, 分配+10)
 *   PECMD_AppendFmtValue  @0x1400668ec   (格式化设置: PECMD_SprintfRetEnd + SetVar)
 *   PECMD_VarSetUInt    @0x140066978   (wsprintfW "%lu" + SetVar)
 *   PECMD_AppendLongDecimal    @0x1400669c4   (wsprintfW "%ld" + SetVar)
 *
 * 反编译缺陷: PECMD_VarSetUInt/1400669c4 的 wsprintfW 参数被 Ghidra 丢弃
 * (寄存器残留), 按调用语义还原为 value (REVIEW.md §2).
 * ==================================================================== */
#include <stdint.h>
#include <stddef.h>

#include "pecmd_defs.h"
/* ---- 待重构函数原型 (后续批次) ---- */
void FUN_14001E6BC(void *s, LPCWSTR k, LPCWSTR v, int64_t a);        /* @0x14001e6bc 变量写入 */
/* T1: 上行原为 `int a`, 与 core_var2.c 定义 (int64_t) 不一致 — MSVC 按int传参把 r9 高半
 * 清零, 被调方读到 caplen=0x00000000FFFFFFFF (>0), 跳过 len 重算, 消费未初始化 len → OOM 弹窗 */
void FUN_14005D534(void *s, LPCWSTR k, LPCWSTR v);                   /* @0x14005d534 无锁写入 */
WCHAR *PECMD_SprintfRetEnd(WCHAR *buf, uint64_t value, LPCWSTR fmt); /* @0x1400e6d38 格式化 */

/* 全局 */

/* ========== 变量设置核心 @0x1400629b8 ========== */
void FUN_1400629B8(void *script, LPCWSTR key, LPCWSTR value)
{
    if (*key == L'\0') {
        return;
    }
    if (*key == L'&' || (*(uint8_t *)((uint8_t *)script + 0xd) & 0xf) != 0) {
        EnterCriticalSection(&g_csInit);
        FUN_14001E6BC(script, key, value, -1);
        LeaveCriticalSection(&g_csInit);
    }
    else {
        FUN_14005D534(script, key, value);
    }
}

/* ========== 变量设置 + 环境变量 @0x140062a2c ========== */
void PECMD_SetVarAndEnvSync(void *script, LPCWSTR key, LPCWSTR value)
{
    FUN_1400629B8(script, key, value);
    /* 反编译 jumptable 警告: 实际为 SetEnvironmentVariableW(key+2, value) */
    SetEnvironmentVariableW(key + 2, value);
}

/* ========== 缓冲分配变体 @0x140063694 ========== */
/* 分配 count*2+10 字节 (头 size 字段 = count*2+2), 数据起始处写 0 */
void PECMD_AllocWStringBuffer(WCHAR **ps, int64_t count)
{
    uint8_t *hdr;

    *ps = NULL;
    if (count >= 0) {
        for (;;) {
            hdr = (uint8_t *)HeapAlloc(g_hHeap, 0, (size_t)count * 2 + 10);
            if (hdr)
                break;
            FUN_1400630D0(2);
        }
        /* 原文 @60866-60867: 先魔数 dword(+4) 后 qword 尺寸(+0), 尺寸存储覆盖魔数
         * ⇒ 头 qword 仅含尺寸; 颠倒则高32位=0xaa55 → ZeroLenBuf 巨量 memset AV (T1c) */
        *(uint32_t *)(hdr + 4) = 0xaa55;
        *(size_t *)hdr = (size_t)count * 2 + 2;
        *ps = (WCHAR *)(hdr + 8);
        if (*ps != NULL) {
            **ps = L'\0';
        }
    }
}

/* ========== 格式化设置 @0x1400668ec ========== */
void PECMD_AppendFmtValue(void *script, uint64_t value, LPCWSTR key, LPCWSTR fmt)
{
    WCHAR buf[64];

    PECMD_SprintfRetEnd(buf, value, fmt);
    FUN_1400629B8(script, key, buf);
}

/* ========== 数值变量 (无符号) @0x140066978 ========== */
void PECMD_VarSetUInt(void *script, uint64_t value, LPCWSTR key)
{
    WCHAR buf[56];

    wsprintfW(buf, WSTR("%lu"), (uint32_t)value); /* Ghidra 丢参, 已还原 (REVIEW §2) */
    FUN_1400629B8(script, key, buf);
}

/* ========== 数值变量 (有符号) @0x1400669c4 ========== */
void PECMD_AppendLongDecimal(void *script, int64_t value, LPCWSTR key)
{
    WCHAR buf[56];

    wsprintfW(buf, WSTR("%ld"), (int32_t)value); /* Ghidra 丢参, 已还原 (REVIEW §2) */
    FUN_1400629B8(script, key, buf);
}
