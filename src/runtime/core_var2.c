/* ====================================================================
 * core_var2.c — PECMD 变量表实现 (节点/查找/创建/更新)
 *
 * 来源: PECMD原始.EXE (x64)
 *   PECMD_StrCopyW   @0x140063888   (宽串复制到分配缓冲)
 *   PECMD_StrCopyBytes   @0x1400638f8   (字节复制, WCHAR 对齐)
 *   PECMD_StrBldCopyWideN   @0x1400702d4   (节点名字写入)
 *   PECMD_VarWriteNodeValue  @0x140070330   (节点值写入)
 *   PECMD_VarWriteValueCap    @0x140066224   (值+容量写入)
 *   PECMD_NewVarNode    @0x1400188ec   (创建节点)
 *   PECMD_VarLookup      @0x140018978   (变量查找: & 前缀/:: 作用域/父链)
 *   FUN_14001E5B0 @0x14001e5b0  (添加/更新到默认表)
 *   PECMD_VarTruncateUpdate   @0x14005b708   (截断更新值)
 *   FUN_14001E6BC     @0x14001e6bc   (变量写入核心: 查找/创建/更新分发)
 *   FUN_14005D534    @0x14005d534   (环境变量直写)
 *
 * VarNode 结构 (0x20 字节):
 *   +0x00 name   +0x08 value  +0x10 rsv  +0x18 cap(字节,高2位标志 0xc0=固定)
 * Script 结构: +0x00 变量数组 +0x08 数量 +0x38 父 +0x50 模板
 * ==================================================================== */
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#include "pecmd_defs.h"
extern void *PECMD_HeapRealloc(void *ptr, size_t size); /* @0x140063118 */

/* ---- 待重构函数原型 (后续批次) ---- */
void PECMD_VarWrite_impl(void); /* placeholder, 本文件实现 */

/* 全局 */

/* ========== 宽串复制 @0x140063888 ========== */
/* *ps = 分配 len+1 WCHAR 并复制 src; len<0 自动长度 */
WCHAR *PECMD_StrCopyW(WCHAR **ps, LPCWSTR src, int64_t len)
{
    if (len < 0) {
        len = (int64_t)lstrlenW(src);
    }
    PECMD_AllocString(ps, len + 1);
    memcpy(*ps, src, (size_t)len * 2);
    (*ps)[len] = L'\0';
    return *ps;
}

/* ========== 字节复制 (WCHAR 对齐) @0x1400638f8 ========== */
/* *ps = 分配并复制 len 字节, 对齐到 WCHAR 边界写 0 */
WCHAR *PECMD_StrCopyBytes(WCHAR **ps, const void *src, int64_t len)
{
    size_t words;

    if (len < 0) {
        len = 0;
    }
    words = ((size_t)len + 1) >> 1;
    PECMD_AllocString(ps, (int64_t)words + 1);
    if (words * 2 != (size_t)len) {
        ((uint8_t *)*ps)[len] = 0; /* 奇数长度补 0 */
    }
    memcpy(*ps, src, (size_t)len);
    ((uint8_t *)*ps)[words * 2] = 0;
    return *ps;
}

/* ========== 节点名字写入 @0x1400702d4 ========== */
void PECMD_StrBldCopyWideN(WCHAR **pname, LPCWSTR src, int64_t len)
{
    *pname = NULL;
    PECMD_StrCopyW(pname, src, len);
}

/* ========== 节点值写入 @0x140070330 ========== */
void PECMD_VarWriteNodeValue(WCHAR **pval, const void *src, int64_t len)
{
    *pval = NULL;
    PECMD_StrCopyBytes(pval, src, len);
}

/* ========== 值+容量写入 @0x140066224 ========== */
/* *pval = 复制 len 字节 (WCHAR 对齐), *pcap = len */
void PECMD_VarWriteValueCap(WCHAR **pval, uint64_t *pcap, const void *src, int64_t len)
{
    size_t words = ((size_t)len + 1) >> 1;

    PECMD_AllocString(pval, (int64_t)words + 1);
    (*pval)[words] = L'\0';
    (*pval)[len / 2] = L'\0';
    memcpy(*pval, src, (size_t)len);
    *pcap = (uint64_t)len;
}

/* ========== 创建节点 @0x1400188ec ========== */
/* node: +0 name, +1 value, +2 rsv, +3 cap */
void *PECMD_NewVarNode(void *node, LPCWSTR name, LPCWSTR value, int namelen, int64_t caplen)
{
    PECMD_StrBldCopyWideN((WCHAR **)node, name, namelen);
    if (caplen < 0) {
        caplen = (int64_t)lstrlenW(value) * 2;
    }
    PECMD_VarWriteNodeValue((WCHAR **)((uint8_t *)node + 8), value, caplen);
    *(uint64_t *)((uint8_t *)node + 0x10) = 0;
    *(uint64_t *)((uint8_t *)node + 0x18) = 0;
    return node;
}

/* ========== 查找变量 @0x140018978 ========== */
/* 从 script 或 scope 开始沿父链查找 name (剥离 & 前缀, :: 前缀转默认表).
 * namelen<0 精确匹配 (lstrcmpiW), >=0 前缀匹配 N 字符且下一字符为 0.
 * found 非空输出命中所在表; 返回节点指针或 NULL */
void *PECMD_VarLookup(void *script, LPCWSTR name, void *scope, int namelen, void **found)
{
    void *tbl;  /* 当前表 */
    void *stop; /* 停止表 */
    WCHAR c;
    int amp;
    int i; /* & 前缀数 */
    int guard;

    c = *name;
    amp = 0;
    while (c == L'&') {
        name++;
        amp++;
        c = *name;
    }
    if (c == L'\0') {
        return NULL;
    }
    if (c == L':' && name[1] == L':') {
        name += 2;
        scope = &g_Script; /* :: 强制默认表 */
    }
    if (namelen < 0) {
        namelen = lstrlenW(name);
    }
    tbl = script;
    if (scope != NULL) {
        tbl = scope;
    }
    stop = scope;
    if (amp > 1) {
        scope = script; /* 多 & 前缀: 回到脚本根表 */
    }

    if (namelen < 0) {
        /* ---- 精确匹配 ---- */
        guard = 10000000;
        while (tbl != NULL && (guard = guard - 1, 0 < guard)) {
            if (*(int *)((uint8_t *)tbl + 8) > 0) {
                for (i = 0; i < *(int *)((uint8_t *)tbl + 8); i++) {
                    void *node = *(void **)((char *)*(void **)tbl + (size_t)i * 8);
                    if (lstrcmpiW(*(LPCWSTR *)node, name) == 0) {
                        if (found != NULL) {
                            *found = tbl;
                        }
                        return node;
                    }
                }
            }
            if (stop != NULL) {
                return NULL;
            }
            if (tbl == &g_Script) {
                return NULL;
            }
            tbl = *(void **)((uint8_t *)tbl + 0x38); /* 父链 */
            if (tbl == NULL) {
                return NULL;
            }
        }
    }
    else {
        /* ---- 前缀匹配 ---- */
        while (tbl != NULL && (guard = 10000000, 0 < guard)) {
            guard--;
            if (*(int *)((uint8_t *)tbl + 8) > 0) {
                for (i = 0; i < *(int *)((uint8_t *)tbl + 8); i++) {
                    void *node = *(void **)((char *)*(void **)tbl + (size_t)i * 8);
                    LPCWSTR n = *(LPCWSTR *)node;
                    if (StrCmpNIW(n, name, namelen) == 0 && n[namelen] == L'\0') {
                        if (found != NULL) {
                            *found = tbl;
                        }
                        return node;
                    }
                }
            }
            if (stop != NULL) {
                return NULL;
            }
            if (tbl == &g_Script) {
                return NULL;
            }
            tbl = *(void **)((uint8_t *)tbl + 0x38);
        }
    }
    return NULL;
}

/* ========== 截断更新 @0x14005b708 ========== */
/* 节点固定容量模式 (cap 高 2 位 0xc0): 复制 min(容量, len), 尾部清零 */
void PECMD_VarTruncateUpdate(void *node, const void *src, uint64_t len)
{
    uint64_t cap = *(uint64_t *)((uint8_t *)node + 0x18) & 0x3fffffffffffffffULL;

    if ((int64_t)cap < (int64_t)len) {
        len = cap;
    }
    if ((int64_t)len > 0) {
        memcpy(*(void **)((uint8_t *)node + 8), src, (size_t)len);
    }
    cap -= len;
    if (cap > 2) {
        cap = 2;
    }
    if ((int64_t)cap > 0) {
        memset((uint8_t *)(*(void **)((uint8_t *)node + 8)) + len, 0, (size_t)cap);
    }
}

/* ========== 添加/更新到默认表 @0x14001e5b0 ========== */
void *FUN_14001E5B0(void *script, LPCWSTR name, LPCWSTR value, int namelen, int64_t caplen)
{
    void *node;
    void *newtbl;

    while (*name != L'\0') {
        if (caplen < 0) {
            caplen = (int64_t)lstrlenW(value) * 2;
        }
        if (*name != L':' || name[1] != L':') {
            break;
        }
        name += 2;
        namelen -= 2;
        script = &g_Script;
    }
    if (*name == L'\0') {
        return NULL;
    }
    node = calloc(1, 0x20); /* operator_new(0x20) 库替换 */
    if (node != NULL) {
        node = PECMD_NewVarNode(node, name, value, namelen, -1);
    }
    PECMD_VarWriteValueCap((WCHAR **)((uint8_t *)node + 8), (uint64_t *)((uint8_t *)node + 0x18),
                           value, caplen);
    *(int *)((uint8_t *)script + 8) = *(int *)((uint8_t *)script + 8) + 1;
    newtbl = PECMD_HeapRealloc(*(void **)script, (size_t)*(int *)((uint8_t *)script + 8) * 32);
    *(void **)script = newtbl;
    ((void **)newtbl)[*(int *)((uint8_t *)script + 8) - 1] = node;
    return node;
}

/* ========== 变量写入核心 @0x14001e6bc ========== */
void FUN_14001E6BC(void *script, LPCWSTR key, LPCWSTR value, int64_t caplen)
{
    void *node;
    void *tbl;
    LPCWSTR p;
    LPCWSTR actual;
    int amp;
    int namelen;
    uint64_t len;
    bool force_default;
    void *newarr;

    amp = 0;
    p = key;
    while (*p == L'&') {
        p++;
        amp++;
    }
    if (*p == L'\0') {
        return;
    }
    tbl = NULL;
    force_default = false;
    actual = key;
    if (*p == L':' && p[1] == L':') {
        p += 2;
        force_default = true;
        tbl = &g_Script;
        actual = p;
        if (caplen >= -0xf) {
            caplen -= 2;
        }
        else {
            caplen = -1;
            len = (uint64_t)(-caplen - 0x10); /* 保留: 反编译特殊值, TODO(verify) */
        }
    }
    namelen = (int)caplen;
    node = PECMD_VarLookup(script, actual, (amp > 1) ? script : ((tbl != NULL) ? tbl : NULL),
                           namelen, NULL);

    if (caplen < 0) {
        len = (uint64_t)lstrlenW(value) * 2;
    }
    if (node == NULL) {
        if (force_default) {
            FUN_14001E5B0(&g_Script, actual, value, namelen, (int64_t)len);
        }
        else {
            node = calloc(1, 0x20); /* operator_new(0x20) 库替换 */
            if (node != NULL) {
                node = PECMD_NewVarNode(node, p, value, namelen, (int64_t)len);
            }
            *(uint64_t *)((uint8_t *)node + 0x18) = len;
            newarr = PECMD_HeapRealloc(*(void **)script,
                                       (size_t)(*(int *)((uint8_t *)script + 8) + 1) * 32);
            *(void **)script = newarr;
            ((void **)newarr)[*(int *)((uint8_t *)script + 8)] = node;
            *(int *)((uint8_t *)script + 8) = *(int *)((uint8_t *)script + 8) + 1;
        }
    }
    else {
        uint8_t flag = *(uint8_t *)((uint8_t *)node + 0x1f) & 0xc0;
        if (flag == 0xc0) {
            PECMD_VarTruncateUpdate(node, value, len);
        }
        else {
            if (flag != 0) {
                *(uint64_t *)((uint8_t *)node + 8) = 0;
            }
            PECMD_AllocString((WCHAR **)((uint8_t *)node + 8), len + 2);
            memmove(*(void **)((uint8_t *)node + 8), value, (size_t)len);
            *(uint16_t *)((uint8_t *)*(void **)((uint8_t *)node + 8) + len) = 0;
            *(uint64_t *)((uint8_t *)node + 0x18) = len;
        }
    }
}

/* ========== 环境变量直写 @0x14005d534 ========== */
BOOL FUN_14005D534(LPCWSTR key, LPCWSTR value)
{
    if (*key == L'\0') {
        return FALSE;
    }
    return SetEnvironmentVariableW(key, value);
}
