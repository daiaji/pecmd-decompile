/* ====================================================================
 * core_string.c — PECMD 字符串/内存工具族
 *
 * 来源: PECMD原始.EXE (x64, ImageBase=0x140000000)
 *   PECMD_HeapRealloc  @0x140063118
 *   PECMD_AllocStrSlot     @0x140063720  (调用 PECMD_HeapRealloc)
 *   FUN_14006375C       @0x14006375c  (追加)
 *   PECMD_StrDupAlloc       @0x1400700c4
 *   FUN_1400702B0    @0x1400702b0  (直接覆盖指针, 不释放旧值)
 *   PECMD_FreeStrBuf      @0x14005b104
 *   FUN_14005B154    @0x14005b154
 *   FUN_14005C788     @0x14005c788  (ASCII 前缀词比较, 忽略大小写)
 *   PECMD_TokPrefixICmp    @0x14005c72c  (同语义变体)
 *   FUN_1400630D0    @0x1400630d0
 *
 * 内存块布局 (HeapAlloc, 全局堆 g_hHeap = g_hHeap):
 *   [-8] size_t 容量(字节)  [-4] 魔数 0xaa55  [+0] 数据起始
 *
 * PECMD_MemMoveForward/14001d744 = MSVC 内联 memcpy/memmove, 库替换
 * ==================================================================== */
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h> /* TEMP PROBE(既有 OOM 探针): memfail.log */

#include "pecmd_defs.h" /* win32_stub.h: HeapAlloc/HeapFree/MessageBoxW 等 */

/* 全局堆 (mainW 初始化, g_hHeap) */
extern HANDLE g_hHeap;

int FUN_1400630D0(int mode);          /* @0x1400630d0 (S11 返回型归正 dc:60834) */
void PECMD_ExitProcessCall(int code); /* @0x14005b21c (TODO: 待重构确认) */

/* ========== 内存分配器 @0x140063118 ========== */
/* 分配/重分配带 8 字节头 (size+magic) 的内存块, 返回数据指针
 * 失败弹窗: 选 Retry(4)/Ignore(5) 重试, 否则退出+int3 */
void *PECMD_HeapRealloc(void *ptr, size_t size)
{
    /* @0x140063118 — 已还原真体 (B4/P8: 原"UNIMPLEMENTED"标记过期移除) */
    if (ptr == NULL) {
        for (;;) {
            uint8_t *hdr = (uint8_t *)HeapAlloc(g_hHeap, 0, size + 8);
            if (hdr) {
                /* 原文 @140063118: 先魔数(+4) 后 qword 尺寸(+0), 尺寸覆盖魔数(死存储);
                 * 颠倒则头 qword 高32位=0xaa55 → 按 qword 读尺寸处巨量 memset/拷贝 (T1c) */
                *(uint32_t *)(hdr + 4) = 0xaa55; /* [-4] 魔数 (被下行覆盖) */
                *(size_t *)hdr = size;           /* [-8] 容量 */
                return hdr + 8;
            }
            { /* TEMP PROBE */
                FILE *pf_ = fopen("C:\\pectest\\memfail.log", "a");
                if (pf_) {
                    fprintf(pf_, "OOM str.c site=1 size=0x%llx ptr=NULL\n",
                            (unsigned long long)size);
                    fclose(pf_);
                }
            }
            FUN_1400630D0(2);
        }
    }
    {
        size_t old = *(size_t *)((uint8_t *)ptr - 8);
        uint8_t *nh = (uint8_t *)HeapReAlloc(g_hHeap, 0, (uint8_t *)ptr - 8, size + 8);
        if (nh == NULL) {
            /* ReAlloc 失败: 新分配 + 拷贝 min(old,size) + 释放旧块 */
            for (;;) {
                nh = (uint8_t *)HeapAlloc(g_hHeap, 0, size + 8);
                if (nh)
                    break;
                { /* TEMP PROBE */
                    FILE *pf_ = fopen("C:\\pectest\\memfail.log", "a");
                    if (pf_) {
                        fprintf(pf_, "OOM str.c site=2 size=0x%llx ptr=%p\n",
                                (unsigned long long)size, (void *)ptr);
                        fclose(pf_);
                    }
                }
                FUN_1400630D0(2);
            }
            *(uint32_t *)(nh + 4) = 0xaa55;
            *(size_t *)nh = size;
            size_t copy_len = (size < old) ? size : old;
            memcpy(nh + 8, ptr, copy_len); /* 反编译冗余空检查已删除 (恒真) */
            HeapFree(g_hHeap, 0, (uint8_t *)ptr - 8);
            return nh + 8;
        }
        *(uint32_t *)(nh + 4) = 0xaa55;
        *(size_t *)nh = size;
        return nh + 8;
    }
}

/* ========== 字符串分配 @0x140063720 ========== */
/* 确保 *ps 容量 >= count 个 WCHAR (含结尾 0), 返回 *ps */
WCHAR *PECMD_AllocString(WCHAR **ps, int64_t count)
{
    WCHAR *p = (WCHAR *)PECMD_HeapRealloc(*ps, count * 2 + 2);
    *ps = p;
    if (p != NULL) {
        p[count] = 0;
    }
    return *ps;
}

/* ========== 字符串追加 @0x14006375c ========== */
/* *ps 保留原有内容并追加 src (含结尾 0), 返回 *ps */
WCHAR *FUN_14006375C(WCHAR **ps, LPCWSTR src)
{
    size_t old = 0;
    if (src == NULL) {
        return *ps;
    }
    if (*ps != NULL) {
        old = (size_t)lstrlenW(*ps);
    }
    size_t slen = (size_t)lstrlenW(src);
    WCHAR *p = (WCHAR *)PECMD_HeapRealloc(*ps, (old + slen + 1) * 2);
    *ps = p;
    if (p != NULL) {
        memcpy((uint8_t *)p + old * 2, src, slen * 2 + 2);
    }
    return *ps;
}

/* ========== 字符串复制 @0x1400700c4 ========== */
/* 分配新块并复制 src, 返回新指针 */
WCHAR *PECMD_StrDupAlloc(LPCWSTR src)
{
    size_t len = (size_t)lstrlenW(src);
    uint8_t *hdr;
    for (;;) {
        hdr = (uint8_t *)HeapAlloc(g_hHeap, 0, len * 2 + 12);
        if (hdr)
            break;
        { /* TEMP PROBE */
            FILE *pf_ = fopen("C:\\pectest\\memfail.log", "a");
            if (pf_) {
                fprintf(pf_, "OOM str.c site=3 strdup len=0x%llx\n",
                        (unsigned long long)len);
                fclose(pf_);
            }
        }
        FUN_1400630D0(2);
    }
    *(uint32_t *)(hdr + 4) = 0xaa55;
    *(size_t *)hdr = len * 2 + 4;
    memcpy(hdr + 8, src, len * 2 + 2);
    return (WCHAR *)(hdr + 8);
}

/* ========== 字符串赋值 @0x1400702b0 ========== */
/* 直接覆盖 *ps (不释放旧值, 调用方负责), src 可空 */
void FUN_1400702B0(WCHAR **ps, LPCWSTR src)
{
    *ps = NULL;
    if (src != NULL) {
        *ps = PECMD_StrDupAlloc(src);
    }
}

/* ========== 字符串释放 @0x14005b104 ========== */
void PECMD_FreeStrBuf(WCHAR **ps)
{
    if (*ps != NULL) {
        HeapFree(g_hHeap, 0, (uint8_t *)*ps - 8);
    }
    *ps = NULL;
}

/* ========== 跳过空白 @0x14005b154 ========== */
/* 空白: 0x9-0xD 或 0x20 (空格), 修改 *pp 并返回 */
WCHAR **FUN_14005B154(WCHAR **pp)
{
    if (*pp != NULL) {
        while ((**pp > 8 && **pp < 0xe) || **pp == 0x20) {
            (*pp)++;
        }
    }
    return pp;
}

/* ========== ASCII 前缀词比较 @0x14005c788 / @0x14005c72c ========== */
/* 比较 s(ASCII) 与 w(宽字符) 前 n 字符 (忽略大小写, 仅 ASCII),
 * 全部匹配且 w[n] 是空白/结束 → 返回 1, 否则 0.
 * 调用方只检查低字节 (char), 高字节为寄存器残留 */
int FUN_14005C788(const char *s, const WCHAR *w, int n)
{
    /* R14(batch-A #016): dc:54955-54976 纯 n 字符前缀比较, 全配无条件返 1
     * (dc:54963-65)。v0 尾部混入的分隔符校验实为姊妹函数 FUN_14005c72c 语义
     * (dc:54922-54947), 该语义真体已存在 = PECMD_TokPrefixICmp
     * (restored_bodies.c:7307 ≡ dc:54922), 需要它的调用方应走那个入口。 */
    while (n-- > 0) {
        char c = *s;
        WCHAR u = *w;
        if (!((c > 0x40 || u == (WCHAR)(short)c) && ((WCHAR)(short)c | 0x20) == (u | 0x20))) {
            return 0;
        }
        s++;
        w++;
    }
    return 1;
}

/* ========== 内存不足提示 @0x1400630d0 ========== */
/* mode: 0=内存错误, 其他=内存不足!; 返回 4(Retry)/5(Ignore) 继续, 否则退出.
 * S11(dc:60834 调用方 `iVar2 = FUN_1400630d0(2); while(iVar2==4)` 实证返回 int,
 * 旧移植体误写 void → 全部 OOM 重试环编译失真). */
int FUN_1400630D0(int mode)
{
    /* TEMP PROBE (P2 分诊): 记录 OOM 弹窗触发, 定位后移除 */
    {
        FILE *pf = fopen("C:\\pectest\\memfail.log", "a");
        if (pf) {
            fprintf(pf, "OOM dialog mode=%d heap=%p GetProcessHeap=%p\n", mode,
                    (void *)g_hHeap, (void *)GetProcessHeap());
            fclose(pf);
        }
    }
    LPCWSTR msg = (mode == 0) ? WSTR("内存错误") : WSTR("内存不足!");
    int r = MessageBoxW((HWND)0, msg, WSTR("异常退出"), 5);
    if (r != 4 && r != 5) {
        PECMD_ExitProcessCall(8);
        /* int3 (swi(3)) */
        __debugbreak();
    }
    return r;
}
