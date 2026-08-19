/*
 * core_script.c — 脚本执行器（B8b 核心）
 *
 *   PECMD_ParseScriptSegments   @0x140030420   脚本执行器（段切分/import/递归）
 *   FUN_1400307C8   @0x1400307c8   import 文件处理（读文件/编码/合并）
 *
 * 依赖（已实现）：
 *   PECMD_AllocSmallObject/StrAlloc/StrFree/StrCat/StrAssign/XorStr/FindChar/ZeroMem
 *   FUN_14005C788/StrICmpEq/SkipWs/NumParse2/DevPathFix/DevPathIs
 *   PECMD_OpenFileHandle/OpenDevPath/DevSize/DevSize2/EncodeDet/PathAbs/LineSep
 *   FUN_14007DE70/ByteRealloc/AllocAA
 * 待实现（extern 声明）：
 *   FUN_1400E7D58  @0x1400e7d58   资源解码（B9）
 *   FUN_140003A20   @0x140003a20   脚本行执行（B8c）
 */
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>

#include "pecmd_defs.h"

extern void *PECMD_GrowByteBuffer(void **ps, int64_t len);   /* @0x140063424 */
extern int32_t FUN_14001B5AC(LPCWSTR buf, uint32_t key, int64_t n);  /* @0x14001b5ac */
extern uint64_t FUN_14001B4F8(const WCHAR *buf, WCHAR ch); /* @0x14001b4f8 */
extern void FUN_14005B0B8(void *p, size_t len);           /* @0x14005b0b8 */
extern int64_t *FUN_14005B154(WCHAR **ps);                 /* @0x14005b154 */
extern void *PECMD_ReallocBuffer(void *old, int64_t size);      /* @0x140063224 */
extern void FUN_140024A54(LPCWSTR in, WCHAR **out, void *script); /* @0x140024a54 */
extern uint64_t FUN_14005F33C(const uint8_t *data, int len);    /* @0x14005f33c */
extern void FUN_14002487C(void *script, WCHAR *buf, bool stopMain); /* @0x14002487c */
extern char FUN_14006CCD4(LPCWSTR *ps);                /* @0x14006ccd4 */
extern int32_t FUN_140006A4C(LPCWSTR path);             /* @0x140006a4c */
extern void PECMD_OpenFileHandle(HANDLE *out, LPCWSTR path, DWORD access, DWORD share,
                           LPSECURITY_ATTRIBUTES sa, DWORD disp, DWORD flags, HANDLE tmpl); /* @0x140003864 */
extern int64_t FUN_14001D810(LPCWSTR path, uint64_t access, int64_t share); /* @0x14001d810 */
extern int64_t FUN_14005FBD4(HANDLE h, int64_t *geo);     /* @0x14005fbd4 */
extern int64_t PECMD_GetDeviceSize(HANDLE h, uint8_t mtype);   /* @0x140061070 */
extern uint32_t FUN_14006A7F4(LPCWSTR *ps, uint64_t *out); /* @0x14006a7f4 */
extern int64_t PECMD_EncodeBuffer(int64_t *in, int64_t *out, uint8_t cp); /* @0x140068984 */

/* B9 待实现 */
extern uint32_t FUN_1400E7D58(int64_t *ps, uint32_t flags); /* @0x1400e7d58 */
/* B8c 待实现 */
extern void FUN_14007BF44(void *script, WCHAR *line, WCHAR **out, int mode, uint8_t opt); /* @0x14007bf44 */

/* 前向声明（本文件内相互调用） */
extern uint32_t FUN_1400307C8(int64_t *ctrl, int start, LPCWSTR script, int64_t *out,
                                 void *script2, uint32_t flags); /* @0x1400307c8 */

/* ========== PECMD_ParseScriptSegments @0x140030420 ==========
 * 脚本执行器：
 *   ctrl   : 缓冲控制块 [0]=文本指针 [1][2]=长度
 *   start  : 起始偏移（WCHAR）
 *   len    : 长度（WCHAR）
 *   out    : 输出行缓冲 [0]=行文本
 *   script : 脚本结构（+0x44 主分隔符, +0x45/+0x48 副, +0x49/+0x4a 附加）
 *   flags  : 编码标志（高 16 位=编码, 低=状态）
 * 处理段切分、import 前缀、递归合并。
 */
uint32_t PECMD_ParseScriptSegments(int64_t *ctrl, int start, int len, int64_t *out,
                          LPCWSTR script, uint32_t flags)
{
    WCHAR *base = (WCHAR *)ctrl[0];
    WCHAR *p = base + start;
    WCHAR *end = p + len;
    WCHAR *seg;
    uint32_t code = flags >> 0x10;
    WCHAR *tmp = NULL;
    uint32_t ret = flags;

    /* 跳过前导分隔符 */
    while (*p == script[0x49] || *p == script[0x4a] || *p == script[0x45] || *p == script[0x48]) p++;

    PECMD_AllocWStringBuffer(&tmp, 0x804);
    if (p < end) {
        do {
            WCHAR *t = p;
            WCHAR *seg2;
            int n;
            uint32_t f2 = flags;
            if (*p == script[0x44]) break;
            /* 找段尾 */
            while (*t != script[0x45] && *t != script[0x48] && *t != script[0x44]) t++;
            n = (int)(t - p);
            if (n > 0x800) n = 0x800;
            tmp[n] = L'\0';
            memcpy(tmp, p, (size_t)n * 2);
            FUN_14001B5AC(tmp, code & 0xffff, n);
            seg = tmp;
            {
                uint64_t r = FUN_14005C788("import", seg, 6);
                WCHAR c8;
                if (r != 0) {
                    /* import 前缀检查：数字 8..0xd 或空格 */
                    c8 = seg[6];
                    if ((c8 > 8 && c8 < 0xe) || c8 == L' ') {
                        /* import 标记改写 */
                        p[1] = (WCHAR)(code ^ 0x2f);
                        *p = (WCHAR)(code ^ 0x2f);
                        /* 找第一个非空白参数 */
                        {
                            WCHAR *q = seg + 6;
                            WCHAR *fn = NULL;
                            WCHAR *fnbuf = NULL;
                            uint64_t found;
                            while (q < tmp + n) {
                                if (!((*q < 9 || *q > 0xd) == 0 || *q == L' ')) {
                                    break;
                                }
                                q++;
                            }
                            if (q < tmp + n) {
                                /* 提取文件名（到空白或文件分隔符） */
                                WCHAR *qe = q;
                                while (qe < tmp + n && *qe != L' ' && *qe != L';' && *qe != L'#' &&
                                       !(*qe > 8 && *qe < 0xe)) qe++;
                                FUN_140063620(&fnbuf);
                                memcpy(fnbuf, q, (size_t)(qe - q) * 2);
                                fnbuf[qe - q] = 0;
                                FUN_140024A54(fnbuf, &fn, (void *)script);
                                /* 检查是否已 import */
                                found = (uint64_t)(intptr_t)StrStrW((LPCWSTR)out[0], fn);
                                FUN_14005B104(&fn);
                                FUN_14005B104(&fnbuf);
                                if (found == 0) {
                                    /* 跳过分隔符后递归处理 import 文件 */
                                    WCHAR *r2 = qe;
                                    while (*r2 == script[0x45] || *r2 == script[0x48]) r2++;
                                    {
                                        int imp_off = (int)(r2 - base);
                                        uint32_t u = FUN_1400307C8(ctrl, imp_off, script, out,
                                                                      (void *)script, flags);
                                        if (u != 0xffffffff) {
                                            ret = f2 | u;
                                        }
                                        /* 更新游标：定位到新合并文本的主分隔符 */
                                        {
                                            uint64_t nl = FUN_14001B4F8((WCHAR *)(base + imp_off), (WCHAR)code);
                                            end = (WCHAR *)(base + ((int)nl + imp_off) * 2);
                                            p = end;
                                        }
                                    }
                                }
                            }
                        }
                        break;
                    }
                }
            }
            /* 正常段：跳过 */
            (void)seg2;
            /* 跳到下一个分隔符 */
            p = t;
            while (*p == script[0x49] || *p == script[0x4a] || *p == script[0x45] || *p == script[0x48]) p++;
            code = ret >> 0x10;
        } while (p < end);
    }
    {
        /* 尾部：定位主分隔符 */
        uint64_t u8 = FUN_14001B4F8(end, script[0x44]);
        int64_t l17 = (int64_t)(((int64_t)(int)((u8 * 2 - ctrl[0]) + (int64_t)end) >> 1) * 2);
        ctrl[1] = l17;
        ctrl[2] = l17;
    }
    FUN_14005B0B8(tmp, 0);
    FUN_14005B104(&tmp);
    return ret;
}

/* ========== FUN_1400307C8 @0x1400307c8 ==========
 * import 文件处理：
 *   格式 "file[|offset[|len]]"；打开/读文件，编码识别，
 *   XOR 解码，合并到 ctrl 缓冲，递归 PECMD_ParseScriptSegments。
 */
uint32_t FUN_1400307C8(int64_t *ctrl, int start, LPCWSTR script, int64_t *out,
                          void *script2, uint32_t flags)
{
    bool isDev = *(WCHAR *)(*out + 2) != 0;   /* 设备路径标志 TODO(verify) */
    WCHAR *s = StrChrW(script, L'|');
    WCHAR *path = NULL;
    int64_t off = 0;
    int64_t limit = -1;
    WCHAR *p2;
    HANDLE h = 0;
    int64_t fsize = 0;
    uint8_t *buf = NULL;
    int64_t bsize = 0;
    uint32_t ret;
    uint64_t enc;

    /* 解析 "file[|offset[|len]]" */
    FUN_1400702B0(&path, script);
    if (s != NULL) {
        *s = L'\0';
        s++;
        {
            uint64_t v = 0;
            if (FUN_14006A7F4((LPCWSTR *)&s, &v)) off = (int64_t)(int32_t)v;
        }
        if (*s == L'|') {
            s++;
            {
                uint64_t v = 0;
                if (FUN_14006A7F4((LPCWSTR *)&s, &v)) limit = (int64_t)(int32_t)v;
            }
        }
    }

    p2 = path;
    isDev = FUN_14006CCD4((LPCWSTR *)&p2) != 0;
    if (isDev && FUN_140006A4C(p2)) {
        h = (HANDLE)FUN_14001D810(p2, 0x80000000, 7);
    } else {
        PECMD_OpenFileHandle(&h, p2, 0x80000000, 7, NULL, 3, 0, (HANDLE)0);
    }
    if (h == 0) goto done;

    if (isDev) {
        fsize = PECMD_GetDeviceSize(h, (uint8_t)isDev);
    } else {
        int64_t sz = 0;
        GetFileSizeEx(h, (void *)&sz);
        fsize = sz;
    }
    if (fsize < 1 || fsize < off) {
        CloseHandle(h);
        goto done;
    }
    if (off > 0) fsize -= off;
    if (fsize > 0x10000000) fsize = 0x10000000;
    if (limit > 0 && limit < fsize) fsize = limit;

    buf = (uint8_t *)PECMD_GrowByteBuffer((void **)&buf, fsize + 0x10);
    if (buf == NULL) {
        CloseHandle(h);
        goto done;
    }
    bsize = (int64_t)(int32_t)fsize;
    if (off > 0) {
        int64_t pos = 0;
        SetFilePointerEx(h, off, &pos, 0);
    }
    {
        DWORD rd = 0;
        ReadFile(h, buf, (DWORD)fsize, &rd, NULL);
        bsize = rd;
    }
    CloseHandle(h);
    memset(buf + bsize, 0, 0x10);
    /* 合并到行缓冲 */
    FUN_14006375C((WCHAR **)out, path);
    FUN_14006375C((WCHAR **)out, WSTR("\n"));
    enc = FUN_14005F33C(buf, (int)bsize);
    {
        uint32_t u17 = (flags >> 0x10) & 0xffff;
        uint32_t u13 = (flags >> 0x10) & 0xff;
        u13 = (u13 << 8) | u13;
        if ((uint8_t)enc == 0) {
            if (*(uint16_t *)buf == 0xfeff || *(uint16_t *)buf == 0xfffe) {
                u13 = u17;
            }
            {
                WCHAR *w = (WCHAR *)buf;
                int64_t n = (bsize + 1) >> 1;
                while (n-- > 0) {
                    *w = *w ^ (uint16_t)u13;
                    w++;
                }
            }
            ret = (u13 << 0x10) | (flags & 0xffff);
        } else {
            /* 编码文本：经 PECMD_EncodeBuffer 解码 */
            int64_t in[3] = { (int64_t)buf, bsize, bsize };
            int64_t out2[3] = { 0, 0, 0 };
            PECMD_EncodeBuffer(in, out2, (uint8_t)u17);
            ret = (u13 << 0x10) | (flags & 0xffff) | (int)(uint8_t)enc;
            flags = (int)(uint8_t)enc | flags;
            FUN_14005B104((WCHAR **)&out2[0]);
        }
    }
    FUN_14005B104((WCHAR **)&buf);
    return ret;
done:
    if (buf) FUN_14005B104((WCHAR **)&buf);
    FUN_14005B104(&path);
    return 0xffffffff;
}
