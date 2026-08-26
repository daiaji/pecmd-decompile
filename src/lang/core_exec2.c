/*
 * core_exec2.c — 命令解析/资源加载/执行工具（B8a）
 *
 *   FUN_14005B154      @0x14005b154   跳过空白
 *   FUN_14005B1A8  @0x14005b1a8   前缀比较（后随空白/结束）
 *   PECMD_TokPrefixICmp @0x14005c72c   前缀比较（字母忽略大小写）(已声明 StrICmpN)
 *   FUN_14001EA18     @0x14001ea18   加载资源（FindResource/LoadResource + 解码）
 *   FUN_14005F33C   @0x14005f33c   编码检测（UTF8/UTF16/UNICODE 魔数）
 *   PECMD_SemicolonRunToNewline   @0x14005e51c   空白/注释处理（";;" → 换行）
 *   FUN_140024C48  @0x140024c48   Token 解析（引号/空白）
 *   FUN_140073CCC    @0x140073ccc   参数表构建
 *   FUN_14001BE14  @0x14001be14   去首尾引号
 *   PECMD_CopyCommandLine     @0x140027970   命令行复制（+0x11 预留）
 *   FUN_14006459C    @0x14006459c   全路径
 *   FUN_14001C270     @0x14001c270   路径修正
 *   PECMD_OpenFileHandle    @0x140003864   文件打开（包装 CreateFileW）
 *   FUN_140101E70   @0x140101e70   文件存在
 *   FUN_14007026C    @0x14007026c   多字节字符串分配
 *   PECMD_AllocAnsiString     @0x140070044   ANSI 字符串复制
 *   PECMD_InitObfuscatedKeywords   @0x14006159c   脚本分隔符初始化（XOR 编码表）
 *   PECMD_MaskScriptEndFileTail     @0x14002487c   行分割（_ENDFILE 处理）
 *   PECMD_ExitProcessCall    @0x14005b21c   退出进程
 *   PECMD_ReallocBuffer     @0x140063224   带 0xaa55 头分配
 */
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>

#include "pecmd_defs.h"

extern WCHAR **FUN_14005B154(WCHAR **pp);            /* @0x14005b154 见 core_string.c */
extern char *PECMD_AllocAnsiString(const char *src); /* @0x140070044 */
/* S11: 本地声明与定义冲突已删除, 统一采用 xproto.h 原型 (原: extern int32_t FUN_1400630D0(int mode); /* @0x1400630d0 * /) */
extern void PECMD_ExitProcessCall(UINT code);        /* @0x14005b21c */
extern int32_t FUN_14001B5AC(LPCWSTR buf, uint32_t key, int64_t n); /* @0x14001b5ac */
extern int64_t PECMD_TokPrefixICmp(const char *a, const WCHAR *w, int n); /* @0x14005c72c (S18 Patch#3/#4) */
/* TEMP PROBE 最小 CRT 原型 (禁 stdio.h) */
extern void *__cdecl fopen(const char *_Filename, const char *_Mode);
extern int __cdecl fprintf(void *_Stream, const char *_Format, ...);
extern int __cdecl fclose(void *_Stream);
extern int64_t PECMD_TokPrefixICmp(const char *a, const WCHAR *w, int n); /* @0x14005c72c (S18 Patch#3/#4) */

/* FUN_14005B154 实现见 core_string.c (@0x14005b154)。 */

/* ========== FUN_14005B1A8 @0x14005b1a8 ==========
 * 比较 w 前 n 个字符与 s 是否相等，且 w 其后为空白/结束。
 * 匹配成功时推进 *pw 越过前缀并返回 1。
 */
int32_t FUN_14005B1A8(const WCHAR *s, const WCHAR **pw, int n)
{
    const WCHAR *w = *pw;
    while (n-- > 0) {
        WCHAR c = *s++;
        WCHAR d = *w++;
        if (c != d)
            return 0;
    }
    if (*w != L'\0' && !((*w > 8 && *w < 0xe) || *w == 0x20)) {
        return 0;
    }
    FUN_14005B154((WCHAR **)pw);
    *pw = w;
    return 1;
}

/* ========== FUN_14005F33C @0x14005f33c ==========
 * 自定义容器编码检测（R14 batch-A #031 按 dc:57228-57264 重写）：
 *   len>12 时校验 data[4..6]=="CMP"（魔数表 @0x140124d00 = 00 00 00 00 'C''M''P' 'x'…,
 *   pe_registry.json:94 字节级实证），再按 data[7] 分派：
 *   'a'→0x14 / 'A'→8 / 'x'→4；code!=0 或 =='S' 时 *(int*)(data+8)!=0 加 bit1，
 *   返回 code|1。不匹配返 0。v0 误写为 BOM 嗅探(FF FE/FE FF/EF BB BF)，结构错位。
 */
uint64_t FUN_14005F33C(const uint8_t *data, int len)
{
    if (len > 12) {
        static const uint8_t magic[8] = {0x00, 0x00, 0x00, 0x00, 'C', 'M', 'P', 'x'};
        int i;
        for (i = 4; i < 7; i++) {
            if (data[i] != magic[i]) {
                return 0;
            }
        }
        {
            uint8_t code = 0;
            char c = (char)data[7];
            if (c == 'a') {
                code = 0x14;
            }
            if (c == 'A') {
                code = 8;
            }
            if (c == 'x') {
                code = 4;
            }
            if (code != 0 || c == 'S') {
                if (*(const int *)(const void *)(data + 8) != 0) {
                    code = (uint8_t)(code | 2);
                }
                return (uint64_t)((int)(char)code | 1);
            }
        }
    }
    return 0;
}

/* ========== FUN_14007026C @0x14007026c ==========
 * 分配多字节字符串容器（*out = ANSI 串复制）。
 */
void *FUN_14007026C(void **out, const char *src)
{
    *out = NULL;
    if (src != NULL) {
        *out = PECMD_AllocAnsiString(src);
    }
    return out;
}

/* ========== PECMD_AllocAnsiString @0x140070044 ==========
 * ANSI 字符串复制到 0xaa55 头分配块。
 */
char *PECMD_AllocAnsiString(const char *src)
{
    int n = lstrlenA(src);
    long l = (long)n + 1;
    int64_t *p;
    int r;
    do {
        p = (int64_t *)HeapAlloc(g_hHeap, 0, (size_t)n + 9);
        if (p != NULL)
            break;
        r = FUN_1400630D0(2);
    } while (r == 4);
    *(uint32_t *)((char *)p + 4) = 0xaa55;
    *p = l;
    memcpy(p + 1, src, (size_t)l);
    return (char *)(p + 1);
}

/* FUN_1400630D0 实现见 core_string.c (@0x1400630d0)。 */

/* ========== PECMD_ExitProcessCall @0x14005b21c ========== */
void PECMD_ExitProcessCall(UINT code)
{
    ExitProcess(code);
}

/* ========== PECMD_ReallocBuffer @0x140063224 ==========
 * 分配 size+8 头（0xaa55 魔数 + 容量），复制旧块内容（按小者）。
 */
void *PECMD_ReallocBuffer(void *old, int64_t size)
{
    int64_t *p;
    int r;
    do {
        p = (int64_t *)HeapAlloc(g_hHeap, 0, (size_t)size + 8);
        if (p != NULL)
            break;
        r = FUN_1400630D0(2);
    } while (r == 4);
    *(uint32_t *)((char *)p + 4) = 0xaa55;
    *p = size;
    if (old != NULL) {
        uint64_t osize = (uint64_t)((int64_t *)old)[-1];
        int copy = (int)osize;
        if (size < (int64_t)osize)
            copy = (int)size;
        memcpy(p + 1, old, (size_t)copy);
        memset(old, 0, (size_t)osize);
        HeapFree(g_hHeap, 0, (char *)old - 8);
    }
    return p + 1;
}

/* ========== PECMD_OpenFileHandle @0x140003864 ==========
 * 打开文件到 *out（失败置 NULL）。INVALID_HANDLE_VALUE → NULL。
 */
HANDLE PECMD_OpenFileHandle(HANDLE *out, LPCWSTR path, DWORD access, DWORD share,
                            LPSECURITY_ATTRIBUTES sa, DWORD disp, DWORD flags, HANDLE tmpl)
{
    HANDLE h = *out;
    if (h != 0 && h != (HANDLE)-1)
        CloseHandle(h);
    *out = 0;
    h = CreateFileW(path, access, share, sa, disp, flags, tmpl);
    if (h == (HANDLE)-1)
        h = 0;
    *out = h;
    { /* TEMP PROBE R15(文件打开序列取证) */
        void *pf_ = fopen("C:\\pectest\\memfail.log", "a");
        if (pf_) {
            fprintf(pf_, "[OFH] h=%p disp=%u fl=%08x path=%ls\n", (void *)h,
                    (unsigned)disp, (unsigned)flags, path ? path : L"(null)");
            fclose(pf_);
        }
    }
    return h;
}

/* ========== FUN_140101E70 @0x140101e70 ========== */
bool FUN_140101E70(LPCWSTR path)
{
    HANDLE h = 0;
    PECMD_OpenFileHandle(&h, path, 0, 7, (LPSECURITY_ATTRIBUTES)0, 3, 0, (HANDLE)0);
    bool b = h != 0;
    if (h != 0 && h != (HANDLE)-1)
        CloseHandle(h);
    return b;
}

/* ========== FUN_14006459C @0x14006459c ==========
 * 单字符盘符（如 "C"）展开为 "C:\"。返回路径长度。
 */
DWORD FUN_14006459C(LPCWSTR src, uint32_t buflen, LPWSTR buf, LPWSTR *last)
{
    WCHAR cwd[0x110]; /* R14(batch-A #019): dc:61522 缓冲 264 元素; v0 0x104=256 在长 CWD 时栈越界 */
    if (!((uint16_t)src[0] < 0x7b && src[1] == L':' && src[2] == L'\0')) {
        return GetFullPathNameW(src, buflen, buf, last);
    }
    cwd[0] = L'\0';
    if (last)
        *last = NULL;
    GetCurrentDirectoryW(0x104, cwd);
    if (*src == cwd[0]) {
        DWORD n = lstrlenW(cwd);
        uint32_t u = n + 1;
        if (u > 4) {
            if (buflen < u)
                return u;
            memcpy(buf, cwd, (u) * 2);
            return n;
        }
    }
    if (buflen < 4)
        return 4;
    buf[0] = *src;
    buf[1] = L':';
    buf[2] = L'\\';
    buf[3] = L'\0';
    return 3;
}

/* ========== FUN_14001C270 @0x14001c270 ==========
 * 路径修正：分配 len*2+0x105 缓冲，调用 FullPath 展开。
 */
int64_t FUN_14001C270(LPCWSTR path, WCHAR **out)
{
    int n = lstrlenW(path);
    LPWSTR tmp = NULL;
    PECMD_AllocString(out, n * 2 + 0x105);
    FUN_14006459C(path, (uint32_t)(n * 2 + 0x104), (LPWSTR)*out, &tmp);
    return (int64_t)*out;
}

/* ========== FUN_14001BE14 @0x14001be14 ==========
 * 去掉成对的引号（首尾都是 " 时）。返回处理后起点。
 */
WCHAR *FUN_14001BE14(WCHAR *s)
{
    int64_t n = lstrlenW(s);
    WCHAR *tail = s + (n - 1);
    while (s[0] == 0x22 && *tail == 0x22) {
        s++;
        *tail = 0;
        tail--;
        if (tail < s)
            break;
    }
    return s;
}

/* ========== PECMD_CopyCommandLine @0x140027970 ==========
 * 复制命令行到 *out（容量 len+0x11，末尾清 0x22 字节），返回长度。
 */
int32_t PECMD_CopyCommandLine(LPCWSTR src, WCHAR **out)
{
    int n = lstrlenW(src);
    PECMD_AllocString(out, n + 0x11);
    memset((void *)((int64_t)*out + (int64_t)n * 2), 0, 0x22);
    memcpy(*out, src, (size_t)(n + 1) * 2);
    return n;
}

/* ========== PECMD_InitObfuscatedKeywords @0x14006159c ==========
 * 脚本分隔符初始化：按 seed 生成 XOR 分隔符表。
 *   +0x88: 主分隔符; +0x92/+0x94: 副; +0x8a/+0x90: 行分隔
 *   hi 非 0 时额外生成 {} 等配对符表。
 */
void PECMD_InitObfuscatedKeywords(void *script, uint64_t seed)
{
    uint16_t s = (uint16_t)seed;
    *(uint16_t *)((char *)script + 0x88) = s;
    *(uint16_t *)((char *)script + 0x92) = s ^ 0x20;
    *(uint16_t *)((char *)script + 0x94) = s ^ 9;
    *(uint16_t *)((char *)script + 0x8a) = s ^ 0xd;
    *(uint16_t *)((char *)script + 0x90) = s ^ 10;
    if ((seed & 0xffff0000) != 0) {
        uint16_t b = *(uint16_t *)((char *)script + 0x48);
        *(uint16_t *)((char *)script + 0x96) = b ^ 0x7b;
        *(uint16_t *)((char *)script + 0x98) = b ^ 0x7d;
        *(uint16_t *)((char *)script + 0x9c) = b ^ 0x53;
        *(uint16_t *)((char *)script + 0x9a) = b ^ 0x5f;
        *(uint16_t *)((char *)script + 0xa2) = b ^ 0x5f;
        *(uint16_t *)((char *)script + 0xaa) = b ^ 0x5f;
        *(uint16_t *)((char *)script + 0x9e) = b ^ 0x55;
        *(uint16_t *)((char *)script + 0xb2) = b ^ 0x5f;
        *(uint16_t *)((char *)script + 0xa0) = b ^ 0x42;
        *(uint16_t *)((char *)script + 0xa4) = b ^ 0x73;
        *(uint16_t *)((char *)script + 0xa6) = b ^ 0x75;
        *(uint16_t *)((char *)script + 0xa8) = b ^ 0x62;
        *(uint16_t *)((char *)script + 0xac) = b ^ 0x45;
        *(uint16_t *)((char *)script + 0xae) = b ^ 0x4e;
        *(uint16_t *)((char *)script + 0xb0) = b ^ 0x44;
        *(uint16_t *)((char *)script + 0xb4) = b ^ 0x65;
        *(uint16_t *)((char *)script + 0xb6) = b ^ 0x6e;
        *(uint16_t *)((char *)script + 0xb8) = b ^ 100;
        *(uint16_t *)((char *)script + 0xbc) = b ^ 0x5d;
        *(uint16_t *)((char *)script + 0xba) = b ^ 0x5b;
    }
}

/* ========== PECMD_MaskScriptEndFileTail @0x14002487c ==========
 * 行分割：以脚本分隔符切分（遇 _ENDFILE 终止），空格填充处理。
 * stopMain 非 0 且行首 "FIND $1 = %&&__MAIN__%," 时终止。
 */
void PECMD_MaskScriptEndFileTail(void *script, WCHAR *buf, bool stopMain)
{
    WCHAR sep = *(WCHAR *)((char *)script + 0x88);
    WCHAR *p = buf;
    int64_t cap = 0xfff;
    WCHAR *tmp = NULL;

    PECMD_AllocWStringBuffer((WCHAR **)&tmp, 0x1000);
    if (*buf == sep)
        return;
    do {
        /* 跳过前导分隔符 */
        while (*p == *(WCHAR *)((char *)script + 0x92) || *p == *(WCHAR *)((char *)script + 0x94) ||
               *p == *(WCHAR *)((char *)script + 0x8a) || *p == *(WCHAR *)((char *)script + 0x90))
            p++;
        WCHAR *start = p;
        WCHAR c = *p;
        while (c != sep && c != *(WCHAR *)((char *)script + 0x8a) &&
               c != *(WCHAR *)((char *)script + 0x90)) {
            p++;
            c = *p;
        }
        int64_t n = p - start;
        if (cap < n) {
            int64_t newCap = cap;
            do {
                newCap = newCap * 2 + 1;
            } while (newCap < n);
            PECMD_AllocString(&tmp, newCap + 1);
            cap = newCap;
        }
        memcpy(tmp, start, (size_t)n * 2);
        tmp[n] = 0;
        WCHAR xor = *(WCHAR *)((char *)script + 0x48);
        if (xor != 0) {
            for (int64_t i = 0; i < n; i++)
                tmp[i] = tmp[i] ^ xor;
        }
        if (PECMD_TokPrefixICmp("_ENDFILE", tmp, 8) != 0) /* dc:22012 原体 c72c(S18 Patch#3) */
            break;
        /* 原文 @22019-22030 极性: 仅 "_ENDFILE-IMPORT" 匹配行被空格掩码
         * (!stopMain 时); 普通行不掩码。曾写反导致交给 RunScriptText 的
         * 脚本文本被整体抹空、动词永不执行 (REVIEW §133)。 */
        if (PECMD_TokPrefixICmp("_ENDFILE-IMPORT", start, 0xf) != 0) { /* dc:22019 原体 c72c(S18 Patch#4; 与 :350 成对替换否则 IMPORT 分支不可达) */
            if (stopMain)
                break;
            for (; start < p; start++)
                *start = *(WCHAR *)((char *)script + 0x48) ^ 0x20;
        }
        else if (stopMain && FUN_14005C788("FIND $1 = %&&__MAIN__%,", start, 0x17) != 0) {
            break;
        }
        sep = *(WCHAR *)((char *)script + 0x88);
    } while (*p != sep);
    /* 尾部填充 */
    while (*p != sep) {
        *p = sep;
        p++;
    }
    PECMD_FreeStrBuf((WCHAR **)&tmp);
}

/* ========== FUN_14001EA18 @0x14001ea18 ==========
 * 加载资源：FindResource/SizeofResource/LoadResource；
 * 无 0x40 标志时解码文本，否则原样复制 + XOR 编码。
 */
uint8_t *FUN_14001EA18(HMODULE mod, LPCWSTR id, LPCWSTR type, void **out, uint32_t *flags)
{
    uint32_t f = 0x20;
    uint16_t key = 0;
    uint8_t *data = NULL;
    HRSRC res;
    uint32_t sz = 0;
    int64_t *buf;
    uint64_t dec;

    if (flags)
        f = *flags;
    if (flags)
        key = *(uint16_t *)((char *)flags + 2);
    res = FindResourceW(mod, id, type);
    if (res != 0)
        sz = SizeofResource(mod, res);
    if (res != 0)
        data = (uint8_t *)LoadResource(mod, res);
    if ((f & 1) != 0) {
        if (data == 0)
            goto done;
        data = (uint8_t *)LockResource((HGLOBAL)(uintptr_t)data);
    }
    if (data == 0 || sz == 0)
        goto done;
    if ((f & 0x40) == 0) {
        /* 解码文本 */
        dec = FUN_14005F33C(data, (int)sz);
        if (dec == 0)
            goto done;
        if ((f & 0x20) != 0 && (dec & 2) != 0)
            return NULL;
        /* TODO(verify): 原实现经 PECMD_ReadFile 解码填充 *out */
        {
            uint8_t *t = (uint8_t *)PECMD_AllocAnsiString((const char *)data);
            *out = t;
        }
        *flags = *flags | ((int)(uint8_t)dec & 2U);
        return (uint8_t *)*out;
    }
    /* 原样复制 + XOR */
    buf = (int64_t *)PECMD_ReallocBuffer(NULL, (int64_t)sz + 0x411);
    *(int64_t *)((char *)buf + 0x10) = sz;
    *(int64_t *)((char *)buf + 8) = sz;
    memcpy(buf, data, sz);
    if (key != 0) {
        FUN_14001B5AC((WCHAR *)buf, key, (int64_t)((sz + 1) >> 1));
    }
    *out = buf;
    if (key != 0)
        FUN_14001B5AC((WCHAR *)buf, key, (int64_t)((sz + 1) >> 1));
done:
    return (uint8_t *)*out;
}
