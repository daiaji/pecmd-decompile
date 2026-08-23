/*
 * core_exec4.c — 脚本执行核心（B8b）
 *
 *   FUN_140006A4C   @0x140006a4c   设备路径判断
 *   FUN_14001D810 @0x14001d810   打开设备路径（NtCreateFile 包装）
 *   FUN_14005FBD4     @0x14005fbd4   设备大小（IOCTL）
 *   PECMD_GetDeviceSize    @0x140061070   设备大小（卷信息）
 *   FUN_14006A740 @0x14006a740   数字解析（T/G/M/K/S 后缀）
 *   FUN_14006A7F4   @0x14006a7f4   数字解析包装（跳过空白）
 *   PECMD_NormalizeVolumeDevPath  @0x14006ccd4   设备路径修正（盘符→\\\\.\X:）
 *   FUN_14006CE38 @0x14006ce38   设备路径修正（同上，返回类型）
 *   PECMD_RefCountRelease   @0x140028270   引用计数释放
 *   FUN_1400A4020   @0x1400a4020   引用计数字符串设置
 *   FUN_14007DE70   @0x14007de70   拼接两串（新分配）
 *   PECMD_AbsPathFromCurDir     @0x140024a54   相对路径→绝对路径
 *   PECMD_EncodeBuffer   @0x140068984   读文件到缓冲（编码识别）
 *   FUN_1400E7D58   @0x1400e7d58   资源解码（XOR 分隔符/编码）
 */
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>

#include "pecmd_defs.h"

extern int32_t FUN_1400630D0(int mode);                 /* @0x1400630d0 */
extern void *PECMD_AllocAnsiString(const char *src);            /* @0x140070044 */
extern int64_t *FUN_14005B154(WCHAR **ps);           /* @0x14005b154 */
extern int64_t PECMD_ParseUIntValue(LPCWSTR *ps, uint64_t *out); /* @0x140067d20 */
extern uint32_t FUN_14006A7F4(LPCWSTR *ps, uint64_t *out);  /* @0x14006a7f4 */
extern uint8_t *FUN_14001E69C(void *script, LPCWSTR name, void *scope, int64_t len); /* @0x14001e69c */
extern uint64_t FUN_14005F33C(const uint8_t *data, int len); /* @0x14005f33c */

/* ========== FUN_140006A4C @0x140006a4c ==========
 * 判断是否 \Device\ 或 \ArcName\ 前缀的设备路径。
 */
int32_t FUN_140006A4C(LPCWSTR path)
{
    if (StrCmpNIW(WSTR("\\Device\\"), path, 8) != 0 &&
        StrCmpNIW(WSTR("\\ArcName\\"), path, 9) != 0) {
        return 0;
    }
    return 1;
}

/* ========== FUN_14001D810 @0x14001d810 ==========
 * 打开设备路径（NtCreateFile 包装）。返回句柄或 0。
 * TODO(verify): 原实现经 g_pNtCreateFile/g_pNtClose 动态加载；
 * 本重构直接 CreateFileW。
 */
int64_t FUN_14001D810(LPCWSTR path, uint64_t access, int64_t share)
{
    HANDLE h = CreateFileW(path, (DWORD)access, (DWORD)share, NULL,
                           OPEN_EXISTING, 0, (HANDLE)0);
    if (h == (HANDLE)-1) h = 0;
    return (int64_t)h;
}

/* ========== FUN_14005FBD4 @0x14005fbd4 ==========
 * 设备大小（IOCTL_DISK_GEOMETRY + IOCTL_DISK_GET_DRIVE_GEOMETRY）。
 */
int64_t FUN_14005FBD4(HANDLE h, int64_t *geo)
{
    DWORD ret = 0;
    int64_t size = 0;
    BOOL ok;

    ok = DeviceIoControl(h, 0x7405c, NULL, 0, &size, 8, &ret, NULL);
    memset(geo, 0, 0x18);
    DeviceIoControl(h, 0x70000, NULL, 0, geo, 0x18, &ret, NULL);
    if (!ok) {
        /* 几何扇区数 × 每扇区字节 */
        size = (uint64_t)*(uint32_t *)((char *)geo + 0x14) *
               (uint64_t)*(uint32_t *)((char *)geo + 0x18) *
               (uint64_t)*(uint32_t *)((char *)geo + 0x24) *
               (uint64_t)*(uint32_t *)(geo + 0);
    }
    return size;
}

/* ========== PECMD_GetDeviceSize @0x140061070 ==========
 * 卷/设备大小（mtype & 0xf == 1 时 IOCTL，==2 时 DISK_GEOMETRY_EX）。
 */
int64_t PECMD_GetDeviceSize(HANDLE h, uint8_t mtype)
{
    int64_t size = -1;
    if ((mtype & 0xf) == 1) {
        int64_t geo[3] = {0};
        size = FUN_14005FBD4(h, geo);
    } else if ((mtype & 0xf) == 2) {
        int64_t geoex[0x12] = {0};
        DWORD ret = 0;
        if (DeviceIoControl(h, 0x70048, NULL, 0, geoex, 0x90, &ret, NULL)) {
            return geoex[0x11];
        }
    }
    return size;
}

/* ========== FUN_14006A740 @0x14006a740 ==========
 * 数字解析：解析 *s 为首数字（调用 FUN_14006A7F4），处理 T/G/M/K/S 后缀。
 * 返回 1 成功；*out = 数值。
 */
int32_t FUN_14006A740(LPCWSTR *ps, uint64_t *out)
{
    int r;
    uint16_t ch;
    FUN_14005B154((WCHAR **)ps);
    r = FUN_14006A7F4(ps, out);
    if (r < 1) return 0;
    ch = **ps & 0xffdf;
    if (ch == L'T') *out <<= 0x28;
    else if (ch == L'G') *out <<= 0x1e;
    else if (ch == L'M') *out <<= 0x14;
    else if (ch == L'K') *out <<= 10;
    else if (ch == L'S') *out <<= 9;
    else {
        (*ps)--;
        *ps = (LPCWSTR)((WCHAR *)*ps - 1);
        return 1;
    }
    (*ps)++;
    return 1;
}

/* ========== FUN_14006A7F4 @0x14006a7f4 ==========
 * 数字解析包装：解析成功后跳过空白。
 */
uint32_t FUN_14006A7F4(LPCWSTR *ps, uint64_t *out)
{
    int64_t r = PECMD_ParseUIntValue(ps, out);
    if ((int)r != 0) {
        FUN_14005B154((WCHAR **)ps);
    }
    return (uint32_t)r & 0xffffffff;
}

/* ========== PECMD_NormalizeVolumeDevPath @0x14006ccd4 ==========
 * 设备路径修正：带扩展名的普通路径不变（返回 0）；
 * 盘符 "C:" → "\\\\.\C:"（返回 2）；
 * PhysicalDrive 返回 1。
 */
char PECMD_NormalizeVolumeDevPath(LPCWSTR *ps)
{
    LPCWSTR p = *ps;
    WCHAR *dot;
    int r;
    char c = 0;

    dot = StrRChrW(p, NULL, L'\\');
    if (dot != NULL && (dot = StrChrW(dot, L'.'), dot != NULL)) {
        return 0;
    }
    r = StrCmpNIW(*ps, WSTR("\\\\.\\PhysicalDrive"), 0x11);
    c = (r == 0);
    if (!c) {
        r = FUN_140006A4C(*ps);
        if (r == 0) goto check_dos;
        c = 0x21;
    }
    if (c == 1) return 1;
check_dos:
    r = StrCmpNIW(*ps, WSTR("\\\\?\\"), 4);
    if (r == 0 && *(WCHAR *)(*ps + 8) != 0 &&
        *(WCHAR *)(*ps + 10) == L':' && *(WCHAR *)(*ps + 12) == 0) {
        WCHAR *slash = StrRChrW(p + 5, NULL, L'\\');
        if (slash != NULL) return c;
        *(WCHAR *)(*ps + 4) = L'.';
    } else {
        r = StrCmpNIW(*ps, WSTR("\\\\.\\"), 4);
        if (r != 0 || *(WCHAR *)(*ps + 8) == 0) {
            WCHAR first = **(WCHAR **)ps;
            if (first == 0) return c;
            if (((WCHAR *)*ps)[1] != L':') return c;
            if (((WCHAR *)*ps)[2] != 0) return c;
            PECMD_AllocString(ps, 0x20);
            memcpy((void *)*ps, WSTR("\\\\.\\"), 14);  /* "\\\\.\\" + 盘符占位 */
            *(WCHAR *)(*ps + 8) = first;
            return 2;
        }
    }
    return 2;
}

/* ========== FUN_14006CE38 @0x14006ce38 ==========
 * 同上（返回 undefined1）。
 */
uint8_t FUN_14006CE38(LPCWSTR *ps)
{
    return (uint8_t)PECMD_NormalizeVolumeDevPath(ps);
}

/* ========== PECMD_RefCountRelease @0x140028270 ==========
 * 引用计数释放：计数-1，归零时释放。
 */
void PECMD_RefCountRelease(WCHAR **ps)
{
    WCHAR *s = *ps;
    if (s != NULL) {
        EnterCriticalSection(&g_csInit);
        *(int *)(s + 8) = *(int *)(s + 8) - 1;
        if (*(int *)(s + 8) < 1 && s != NULL) {
            PECMD_FreeStrBuf(&s);
            free(s);
        }
        *ps = NULL;
        LeaveCriticalSection(&g_csInit);
    }
}

/* ========== FUN_1400A4020 @0x1400a4020 ==========
 * 引用计数字符串设置：释放旧值，赋新值（计数=1）。
 */
void FUN_1400A4020(WCHAR **ps, LPCWSTR src)
{
    WCHAR *s;
    EnterCriticalSection(&g_csInit);
    if (*ps != NULL) {
        *(int *)(*ps + 8) = *(int *)(*ps + 8) - 1;
        s = *ps;
        if (*(int *)(s + 8) < 1 && s != NULL) {
            PECMD_FreeStrBuf(&s);
            free(s);
        }
        *ps = NULL;
    }
    s = (WCHAR *)calloc(1, 0x10);
    if (s == NULL) {
        s = NULL;
    } else {
        FUN_1400702B0(&s, NULL);
        *(uint32_t *)(s + 8) = 0;
    }
    *ps = s;
    if (*s != 0) {
        HeapFree(g_hHeap, 0, (void *)(uintptr_t)(*s - 8));
    }
    *(uint32_t *)(*ps + 8) = 1;
    *(WCHAR **)s = (WCHAR *)src;
    LeaveCriticalSection(&g_csInit);
}

/* ========== FUN_14007DE70 @0x14007de70 ==========
 * 拼接 *a + src 到 *out（新分配，带 0xaa55 头）。
 */
void *FUN_14007DE70(LPCWSTR *a, LPCWSTR *out, LPCWSTR src)
{
    int n1 = 0, n2, r;
    int64_t total;
    int64_t *p;

    if (src == NULL) {
        FUN_1400702B0((WCHAR **)out, (LPCWSTR)*a);
    } else {
        if ((LPCWSTR)*a != NULL) n1 = lstrlenW((LPCWSTR)*a);
        n2 = lstrlenW(src);
        total = (int64_t)((n2 + 1 + n1) * 2);
        do {
            p = (int64_t *)HeapAlloc(g_hHeap, 0, (size_t)total + 8);
            if (p != NULL) break;
            r = FUN_1400630D0(2);
        } while (r == 4);
        *(uint32_t *)((char *)p + 4) = 0xaa55;
        *p = total;
        p++;
        memcpy(p, *a, (size_t)n1 * 2);
        memcpy((char *)p + (size_t)n1 * 2, src, (size_t)n2 * 2 + 2);
        *out = (LPCWSTR)p;
    }
    return out;
}

/* ========== PECMD_AbsPathFromCurDir @0x140024a54 ==========
 * 相对路径→绝对路径：解析行首/行尾空白和引号，非绝对路径
 * 前置 &CurDir\。
 */
void PECMD_AbsPathFromCurDir(LPCWSTR in, WCHAR **out, void *script)
{
    LPCWSTR p = in;
    WCHAR *start, *end;
    WCHAR *tmp = NULL;
    uint8_t *curdir;

    if (*p == L'\0') return;
    /* 定位行尾 */
    end = (WCHAR *)p;
    while (*end != L'\r' && *end != L'\n' && *end != L'\0') end++;
    /* 去前导空白 */
    while (p < end && (((WCHAR)*p < 9 || *p > 0xd) == 0 || *p == L' ')) p++;
    if (p >= end) return;
    start = (WCHAR *)p;
    /* 找注释起点（// 、;、#、`） */
    {
        WCHAR *q = start;
        while (q < end) {
            if (((*q > 8 && *q < 0xe) || *q == L' ') &&
                ((q[1] == L'/' && q[2] == L'/') || q[1] == L'`' ||
                 q[1] == L';' || q[1] == L'#')) {
                end = q;
                break;
            }
            q++;
        }
    }
    /* 去尾部空白 */
    while (end > start && ((*(WCHAR *)(end - 1) < 9 || *(WCHAR *)(end - 1) > 0xd) == 0 ||
                           *(WCHAR *)(end - 1) == L' ')) end--;
    /* 去引号 */
    if (*start == L'"') start++;
    if (end > start && *(end - 1) == L'"') end--;

    if (start < end) {
        PECMD_AllocString(&tmp, (int64_t)(end - start) + 1);
        memcpy(tmp, start, (size_t)(end - start) * 2);
        tmp[end - start] = 0;
        /* TODO(verify): 原实现调用 FUN_140003A20 解析变量后写 *out；
         * 此处简化为直接赋值（需注册脚本上下文） */
        FUN_1400702B0(out, tmp);
        /* 非绝对路径 → 前置 &CurDir */
        if (tmp[0] != L'\\' && !(tmp[0] != 0 && tmp[1] == L':')) {
            curdir = FUN_14001E69C(script, WSTR("&CurDir"), NULL, -1);
            if (curdir != NULL) {
                WCHAR *old = *out;
                *out = NULL;
                FUN_1400702B0(out, (LPCWSTR)*(int64_t *)curdir);
                FUN_14006375C(out, WSTR("\\"));
                FUN_14006375C(out, old);
                PECMD_FreeStrBuf(&old);
            }
        }
        PECMD_FreeStrBuf(&tmp);
    }
}

/* ========== PECMD_EncodeBuffer @0x140068984 ==========
 * 读文件到缓冲（编码识别 + 解码）。文件数据来自 *in（含长度 in[1]）。
 * 输出 *out 缓冲（0x10 头 + 数据 + 0x10 尾）。
 * TODO(verify): 原实现依赖 FUN_1400E7D58 解码；本重构简化。
 */
int64_t PECMD_EncodeBuffer(int64_t *in, int64_t *out, uint8_t cp)
{
    uint64_t enc = FUN_14005F33C((const uint8_t *)*in, (int)in[1]);
    uint8_t *data;
    int64_t len;

    if ((uint8_t)enc == 0) return 1;
    data = (uint8_t *)PECMD_AllocAnsiString((const char *)*in);
    len = in[1];
    out[0] = (int64_t)data;
    out[1] = len;
    out[2] = len;
    return 0;
}
