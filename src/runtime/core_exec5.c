/*
 * core_exec5.c — 脚本对象操作（B8b）
 *
 *   FUN_140017CDC  @0x140017cdc   脚本结构复制（0xe0 字节）
 *   FUN_1400186BC  @0x1400186bc   脚本结构初始化（父脚本引用）
 *   FUN_14005C7C4   @0x14005c7c4   字符串忽略大小写相等
 *   PECMD_StrBldCopyAnsi    @0x1400702f0   多字节字符串复制（带长度）
 *   FUN_140073CCC    @0x140073ccc   参数表构建（脚本+0xe 起）
 *   FUN_1400C11C0    @0x1400c11c0   字符串转数字（hex/dec）
 */
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>

#include "pecmd_defs.h"
extern void FUN_1400629B8(void *script, LPCWSTR key, LPCWSTR value); /* @0x1400629b8 */

extern void *PECMD_GrowByteBuffer(void **ps, int64_t len); /* @0x140063424 */
/* S11: 本地声明与定义冲突已删除, 统一采用 xproto.h 原型 (原: extern int64_t *FUN_14005B154(WCHAR **ps); /* @0x14005b154 * / extern void *PECMD_StrBldCopyWide(void *ps, cons) */




/* 原版 DAT_1401206f4: L"[]" 字面量 ([] 空参数的 argv[0]) */
static const WCHAR s_EmptyBracket[] = L"[]";

/* ========== FUN_140017CDC @0x140017cdc ==========
 * 脚本结构复制（0xe0 字节字段逐项复制，bit0 强制清除）。
 */
void *FUN_140017CDC(void *dst, void *src)
{
    uint8_t *d = dst;
    uint8_t *s = src;
    uint8_t b;

    *(int64_t *)d = *(int64_t *)s;
    *(uint32_t *)(d + 8) = *(uint32_t *)(s + 8);
    d[0xc] = s[0xc];
    d[0xd] = s[0xd];
    d[0xe] = s[0xe];
    d[0xf] = s[0xf];
    b = d[0x10];
    d[0x10] = (uint8_t)(((b ^ s[0x10]) & 1) ^ b);
    b = d[0x10];
    d[0x10] = (uint8_t)(((b ^ s[0x10]) & 2) ^ b);
    b = d[0x10];
    d[0x10] = (uint8_t)(((b ^ s[0x10]) & 4) ^ b);
    b = d[0x10];
    d[0x10] = (uint8_t)(((b ^ s[0x10]) & 8) ^ b);
    b = d[0x10];
    d[0x10] = (uint8_t)(((b ^ s[0x10]) & 0x10) ^ b);
    b = d[0x10];
    d[0x10] = (uint8_t)(((b ^ s[0x10]) & 0x20) ^ b);
    b = d[0x10];
    d[0x10] = (uint8_t)(((b ^ s[0x10]) & 0x40) ^ b);
    d[0x10] = (uint8_t)(((d[0x10] ^ s[0x10]) & 0x7f) ^ s[0x10]);
    b = d[0x11];
    d[0x11] = (uint8_t)(((b ^ s[0x11]) & 1) ^ b);
    b = d[0x11];
    d[0x11] = (uint8_t)(((s[0x11] ^ b) & 2) ^ b);
    d[0x12] = s[0x12];
    *(int64_t *)(d + 0x18) = *(int64_t *)(s + 0x18);
    *(uint32_t *)(d + 0x20) = *(uint32_t *)(s + 0x20);
    *(uint32_t *)(d + 0x24) = *(uint32_t *)(s + 0x24);
    *(int64_t *)(d + 0x28) = *(int64_t *)(s + 0x28);
    *(int64_t *)(d + 0x30) = *(int64_t *)(s + 0x30);
    *(int64_t *)(d + 0x38) = *(int64_t *)(s + 0x38);
    *(int64_t *)(d + 0x40) = *(int64_t *)(s + 0x40);
    *(uint16_t *)(d + 0x48) = *(uint16_t *)(s + 0x48);
    d[0x4a] = s[0x4a];
    *(int64_t *)(d + 0x50) = *(int64_t *)(s + 0x50);
    *(int64_t *)(d + 0x58) = *(int64_t *)(s + 0x58);
    *(uint32_t *)(d + 0x60) = *(uint32_t *)(s + 0x60);
    *(int64_t *)(d + 0x68) = *(int64_t *)(s + 0x68);
    /* +0xe0 处字符串引用复制 TODO(verify): 原 PECMD_CopyStrToSlot 深拷贝 */
    FUN_1400702B0((WCHAR **)(d + 0x70), *(LPCWSTR *)(s + 0x70));
    *(int64_t *)(d + 0x78) = *(int64_t *)(s + 0x78);
    *(int64_t *)(d + 0x80) = *(int64_t *)(s + 0x80);
    *(uint16_t *)(d + 0x88) = *(uint16_t *)(s + 0x88);
    *(uint16_t *)(d + 0x8a) = *(uint16_t *)(s + 0x8a);
    *(uint32_t *)(d + 0x8c) = *(uint32_t *)(s + 0x8c);
    *(uint16_t *)(d + 0x90) = *(uint16_t *)(s + 0x90);
    *(uint16_t *)(d + 0x92) = *(uint16_t *)(s + 0x92);
    *(uint16_t *)(d + 0x94) = *(uint16_t *)(s + 0x94);
    *(uint16_t *)(d + 0x96) = *(uint16_t *)(s + 0x96);
    *(uint16_t *)(d + 0x98) = *(uint16_t *)(s + 0x98);
    *(int64_t *)(d + 0x9a) = *(int64_t *)(s + 0x9a);
    *(int64_t *)(d + 0xa2) = *(int64_t *)(s + 0xa2);
    *(int64_t *)(d + 0xaa) = *(int64_t *)(s + 0xaa);
    *(int64_t *)(d + 0xb2) = *(int64_t *)(s + 0xb2);
    *(uint32_t *)(d + 0xba) = *(uint32_t *)(s + 0xba);
    *(int64_t *)(d + 0xc0) = *(int64_t *)(s + 0xc0);
    *(uint16_t *)(d + 0xc8) = *(uint16_t *)(s + 0xc8);
    d[0xca] = s[0xca];
    *(int64_t *)(d + 0xd0) = *(int64_t *)(s + 0xd0);
    d[0xd8] = s[0xd8];
    d[0xd9] = s[0xd9];
    d[0xda] = s[0xda];
    *(int64_t *)(d + 0xe0) = *(int64_t *)(s + 0xe0);
    *(uint32_t *)(d + 0xe8) = *(uint32_t *)(s + 0xe8);
    return dst;
}

/* ========== FUN_1400186BC @0x1400186bc ==========
 * 脚本结构初始化：父脚本引用计数+1，其余字段清零。
 */
void FUN_1400186BC(void *s, int64_t parent)
{
    uint8_t *d = s;
    int64_t templ;
    uint8_t b = 0;

    if (parent != 0) {
        *(int *)(d + 0x20) = *(int *)((char *)parent + 0x20) + 1;
    }
    *(int64_t *)(d + 0x58) = parent;
    *(int64_t *)(d + 0x40) = 0;
    *(uint32_t *)(d + 0x60) = 0;
    *(int64_t *)(d + 0x68) = 0;
    d[0xf] = 0;
    *(int64_t *)(d + 0x58) = 0; /* 修正: 0x58 处父引用 (见下) */
    d[0xe] = 0;
    *(uint32_t *)(d + 8) = 0;
    *(int64_t *)d = 0;
    *(int64_t *)(d + 0xe0) = 0;
    *(uint32_t *)(d + 0xe8) = 0;
    *(uint16_t *)(d + 0xc8) = 0;
    d[0xca] = 0;
    *(int64_t *)(d + 0xd0) = 0;
    d[0xd8] = 0;
    d[0x12] = 0;
    if (parent != 0) {
        templ = *(int64_t *)((char *)parent + 0x50);
    }
    else {
        /* S11(dc:13328/汇编@1400186cc 实证): 原版在条件前 LEA RAX,[RCX+0x58]
         * 预载自锚默认值, parent==0 时跳过父读取、无条件写 this+0x50=RAX
         * ⇒ 根对象自锚(+0x50→&this+0x58)。旧移植体写 0 ⇒ g_Script+0x50
         * 恒 NULL → PSB 尾解引用 AV(门A 残余崩点)。 */
        templ = (int64_t)(intptr_t)((char *)d + 0x58);
    }
    *(int64_t *)(d + 0x50) = templ;
    d[0xd9] = 0;
    *(int64_t *)(d + 0xc0) = 0;
    if (parent != 0) {
        b = *(uint8_t *)((char *)parent + 0x11) >> 1 & 1;
    }
    d[0x11] = (uint8_t)(d[0x11] & 0xfd);
    d[0x11] = (uint8_t)(d[0x11] | b * 2);
}

/* ========== FUN_14005C7C4 @0x14005c7c4 ==========
 * 比较 ansi 与 unicode 串忽略大小写是否相等（全串）。
 */
int32_t FUN_14005C7C4(const char *a, const WCHAR *w)
{
    for (;;) {
        if (*a == '\0' && *w == 0)
            return 1;
        {
            char c = *a;
            uint32_t v = (uint32_t)(uint8_t)c;
            WCHAR u = *w;
            a++;
            w++;
            if (!((0x40 < (uint16_t)(int16_t)c) || (u == (int16_t)c)) ||
                !(((uint16_t)(int16_t)c | 0x20) == (u | 0x20))) {
                /* R14(batch-A #017): 失配条件 dc:54999-5001 = !(A&&B)=!A||!B;
                 * v0 写成 !A&&!B → 字母位恒使 A 真 → 任何失配都被跳过("AB"=="AC")。 */
                return (int32_t)(v & 0xffffff00);
            }
        }
    }
}

/* ========== FUN_1400637DC @0x1400637dc ==========
 * 多字节字符串复制（len 上限，负值用 strlen）。
 */
int64_t *FUN_1400637DC(int64_t *out, const char *src, int64_t len)
{
    size_t n;
    char *p;
    if (len < 0)
        len = lstrlenA(src);
    n = (size_t)len;
    p = (char *)calloc(1, n + 1);
    if (p && src)
        memcpy(p, src, n);
    p[n] = 0;
    *out = (int64_t)p;
    return out;
}

/* ========== PECMD_StrBldCopyAnsi @0x1400702f0 ==========
 * 多字节字符串复制（*out = strndup 风格，len 上限）。
 */
int64_t *PECMD_StrBldCopyAnsi(int64_t *out, const char *src, uint64_t len)
{
    *out = 0;
    FUN_1400637DC(out, src, (int64_t)len);
    return out;
}

/* ========== FUN_140073CCC @0x140073ccc ==========
 * 参数表构建（原文直移, v6.2 重写）。
 * script 状态字段: +0x60 argc(uint) / +0x68 argv 表指针 / +0x70 命令文本串槽 / +0x4a flag。
 * 文本缓冲布局: [命令文本第一份][前部副本(分词 NUL 不破坏 &&__arg 引用)][十进制argc][argv表]。
 * 旧版失真: 缺 StrBldCopyWide 赋源/前部复制/表尾辅助指针, 重建逻辑野指针 → AV(T1d)。 */
uint32_t FUN_140073CCC(void *script, LPCWSTR cmdline, int saveArg)
{
    uint8_t *s = script;
    int64_t *slot = (int64_t *)(s + 0x70); /* param_1+0xe: 命令文本槽 */
    char flag = *(char *)(s + 0x4a);
    WCHAR **tmp = NULL;  /* local_68 临时 token 指针数组 (带8字节头串槽) */
    WCHAR *cur;          /* local_70 */
    WCHAR *save;         /* puVar12: 未消费参数起点(第一份文本内) */
    WCHAR *tok;          /* local_60 */
    int64_t baseOld, newBase;
    int64_t quarter, qcnt, cap50, dupOff, table;
    int64_t saveOff = 0; /* uVar16/iVar15 */
    uint32_t argc = 0;   /* uVar9/uVar7 */
    uint32_t capPlan;    /* local_res20 */
    WCHAR c0;
    int len, i;

    PECMD_AllocWStringBuffer((WCHAR **)&tmp, 4);
    PECMD_StrBldCopyWide((int64_t **)slot, cmdline);      /* 槽@0x70 = cmdline 副本 */
    len = lstrlenW(cmdline);
    quarter = (int64_t)((len + 8) / 4) * 4;               /* local_48 字节 */
    qcnt = ((len + 8) / 4) * 8;                           /* iVar6 终值 */
    cap50 = qcnt + 0x20;                                  /* local_50 wchars */
    PECMD_AllocString((WCHAR **)slot, cap50);             /* 扩容命令缓冲 */
    dupOff = quarter * 2;                                 /* local_58 字节偏移 */
    PECMD_MemMoveForward((uint8_t *)(uintptr_t)*slot + dupOff,
                         (uint8_t *)(uintptr_t)*slot, (int)dupOff);

    cur = (WCHAR *)(uintptr_t)*slot;
    FUN_14005B154(&cur);
    save = cur;
    tok = cur;
    if (cur[0] == L'[' && cur[1] == L']') {
        PECMD_AllocString((WCHAR **)&tmp, 0xc);
        cur += 2;
        *(int64_t *)tmp = (int64_t)(intptr_t)s_EmptyBracket;
        tok = cur;
        FUN_14005B154(&cur);
        argc = 1;
        saveOff = ((intptr_t)cur - (intptr_t)(uintptr_t)*slot) >> 1;
    }
    c0 = *cur;
    if (c0 != 0) {
        capPlan = argc * 4 + 0xc;
        cur = tok;
        do {
            WCHAR *p = cur + 1;   /* 首字符已在 c0, 从下一个开始扫 */
            WCHAR c = *p;
            if (c0 == L'"') {
                while (c != 0 && (c != L'"' || (p[1] != 0 &&
                       !(((p[1] > 8 && p[1] < 0xe) || p[1] == 0x20)))))
                    { p++; c = *p; }
                if (*p == L'"')
                    p++;
            }
            else {
                while (c != 0 && !((c > 8 && c < 0xe) || c == 0x20))
                    { p++; c = *p; }
            }
            if (argc == 0)
                save = p;
            if (*p != 0) {
                *p = 0;
                p++;
            }
            tok = cur;
            PECMD_AllocString((WCHAR **)&tmp, (int64_t)capPlan);
            capPlan += 4;
            tmp[argc] = tok;
            argc++;
            FUN_14005B154(&p);
            if (argc == 1)
                saveOff = ((intptr_t)p - (intptr_t)(uintptr_t)*slot) >> 1;
            cur = p;
            c0 = *cur;
        } while (c0 != 0);
    }
    if (saveArg != 0 || flag != '\0') {
        FUN_1400629B8(script, WSTR("&&__arg"), (LPCWSTR)((intptr_t)save + dupOff));
    }
    baseOld = *slot;
    *(uint32_t *)(s + 0x60) = argc;
    PECMD_AllocString((WCHAR **)slot, qcnt + 0x7a + (int64_t)argc * 4);
    wsprintfW((LPWSTR)((WCHAR *)(uintptr_t)*slot + qcnt), WSTR("%d"), (int)argc);
    newBase = *slot;
    table = newBase + cap50 * 2;
    *(int64_t *)(s + 0x68) = table;
    for (i = 0; i < (int64_t)argc; i++) {
        *(int64_t *)(table + i * 8) =
            (int64_t)(tmp[i]) + (((newBase - baseOld) >> 1) * 2);
    }
    *(int64_t *)(table + (int64_t)argc * 8) =
        newBase + (quarter + saveOff) * 2;
    *(int64_t *)(table + ((int64_t)argc + 1) * 8) = dupOff + newBase;
    *(int64_t *)(table + ((int64_t)argc + 2) * 8) = newBase + qcnt * 2;
    PECMD_FreeStrBuf((WCHAR **)&tmp);
    return argc;
}

/* ========== FUN_1400C1194 @0x1400c1194 ==========
 * 字符串转数字（支持 0x/十进制）。成功返回 true 并推进 *ps。
 */
bool FUN_1400C1194(LPCWSTR *ps, uint64_t *out)
{
    LPCWSTR p = *ps;
    uint64_t v = 0;
    bool neg = false;
    bool any = false;
    if (*p == L'-') {
        neg = true;
        p++;
    }
    if (p[0] == L'0' && (p[1] == L'x' || p[1] == L'X')) {
        p += 2;
        while ((*p >= L'0' && *p <= L'9') || (*p >= L'a' && *p <= L'f') ||
               (*p >= L'A' && *p <= L'F')) {
            WCHAR c = *p++;
            uint64_t d = (c <= L'9') ? (uint64_t)(c - L'0') : (uint64_t)((c | 0x20) - L'a' + 10);
            v = v * 16 + d;
            any = true;
        }
    }
    else {
        while (*p >= L'0' && *p <= L'9') {
            v = v * 10 + (uint64_t)(*p - L'0');
            p++;
            any = true;
        }
    }
    if (!any)
        return false;
    *out = neg ? (uint64_t)(-(int64_t)v) : v;
    *ps = p;
    return true;
}

/* ========== FUN_1400C11C0 @0x1400c11c0 ==========
 * 字符串转数字（支持 0x 前缀）。成功返回 true 并回填 *out。
 */
bool FUN_1400C11C0(LPCWSTR *ps, int *out)
{
    uint64_t v = 0;
    bool r = FUN_1400C1194(ps, &v);
    if (r) {
        *out = (int)v;
    }
    return r;
}
