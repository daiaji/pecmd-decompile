/* ====================================================================
 * core_token.c — PECMD token 分割 / EXEC 命令处理
 *
 * 来源: PECMD原始.EXE (x64)
 *   FUN_140024C48 @0x140024c48   (命令行 token 分割, 引号/转义)
 *   FUN_1400084D0   @0x1400084d0   (EXEC 资源消息处理: Service 安装)
 *
 * NextToken flags:
 *   0x001 跳过闭合引号
 *   0x004 结束后跳过空白
 *   0x008 在 token 结束处写 0 (截断)
 *   0x010 引号闭合不扩展扫描
 *   0x020 不推进 *pp (跳过引号)
 *   0x040 支持单引号
 *   0x080 引号闭合不做特殊处理
 *   0x100 空 token 补写 0
 *   0x200 引号交替转义
 *   0x400 引号后粘连继续扫描
 * ==================================================================== */
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <string.h>

#include "pecmd_defs.h"
extern WCHAR **FUN_14005B154(WCHAR **pp); /* @0x14005b154 */

/* ---- 待重构函数原型 (后续批次) ---- */
void PECMD_CheckDebugMsg(void);            /* @0x140005344 */
void FUN_14007A224(void *s, LPCWSTR a, WCHAR **b, int c, int d); /* @0x14007a224 */
WCHAR *FUN_1400703E4(WCHAR **ps, LPCWSTR src);  /* @0x1400703e4 */
WCHAR *FUN_14000531C(WCHAR *p);      /* @0x14000531c 跳空白(返回指针) */
int FUN_1400660AC(const char *s, WCHAR **pp, int n);  /* @0x1400660ac 前缀词比较 */
void FUN_140008B2C(WCHAR *p);        /* @0x140008b2c RemoveService */
void PECMD_ServiceEntry(WCHAR *p);        /* @0x140017048 执行构造的命令 */

/* 全局 */

/* ========== token 分割 @0x140024c48 ========== */
/* 从 *pp 当前位置取一个 token; *pp 推进到 token 结束;
 * plen 非空则输出 token 长度 (WCHAR 数); 返回 token 起始指针 */
WCHAR *FUN_140024C48(WCHAR **pp, size_t *plen, uint32_t flags)
{
    WCHAR *cur;         /* 扫描位置 */
    WCHAR *start;       /* token 起点 */
    WCHAR q;            /* 引号字符 */
    WCHAR c;
    bool flip;          /* 0x200 转义交替标志 */
    size_t dummy_len;

    if (plen == NULL) {
        plen = &dummy_len;
    }
    *plen = 0;
    FUN_14005B154(pp);
    start = *pp;
    cur = start;
    q = *cur;

    if (q == L'"' || ((flags & 0x40) != 0 && q == L'\'')) {
        /* ---- 引号模式 ---- */
        flip = true;
        do {
            cur++;
            c = *cur;
            while (c != 0 && c != q) {   /* 扫到匹配引号或结束 */
                cur++;
                c = *cur;
            }
        } while (
            /* 0x400: 引号后紧跟非空白 -> 继续下一引号对 */
            (((flags & 0x400) != 0 && *cur != 0) &&
             (c = cur[1], c != 0) &&
             ((c < 9 || c > 0xd) && c != 0x20))
            ||
            /* 0x200: 引号交替转义, 奇数层或紧跟非空白 -> 继续 */
            (((flags & 0x200) != 0 && *cur != 0) &&
             (c = cur[1], c != 0 &&
              (flip = (bool)(flip ^ 1), flip ||
               ((c < 9 || c > 0xd) && c != 0x20))))
        );

        /* 闭合引号后紧跟非空白 -> 扩展到空白 */
        if (*cur == q && (flags & 0x80) == 0 &&
            ((flags & 0x10) == 0 &&
             (cur[1] != 0 && (cur[1] < 9 || cur[1] > 0xd)) &&
             cur[1] != 0x20)) {
            cur++;
            while (*cur != 0 && ((*cur < 9 || *cur > 0xd) && *cur != 0x20)) {
                cur++;
            }
        }
        if ((flags & 0x20) == 0) {
            *pp = *pp + 1;      /* 推进跳过引号 */
        }
        *plen = (size_t)(cur - *pp);   /* 引号内长度 */
        if ((flags & 0x001) != 0 && q == *cur) {
            cur++;
        }
    } else {
        /* ---- 非引号模式: 扫到空白/结束 ---- */
        while (*cur != 0 && ((*cur < 9 || *cur > 0xd) && *cur != 0x20)) {
            cur++;
        }
        *plen = (size_t)(cur - start);
    }

    /* 0x008: 截断 token (在结束处写 0) */
    if ((flags & 0x008) != 0) {
        WCHAR *end = *pp + *plen;
        if (*end != 0) {
            *end = 0;
            if (end == cur) {
                cur++;
            }
        }
    }
    /* 0x100: token 未推进时补写 0 */
    if ((flags & 0x100) != 0 && cur == start && *cur != 0) {
        *cur = 0;
        cur++;
    }
    /* 0x004: 结束后跳空白 */
    if ((flags & 0x004) != 0) {
        FUN_14005B154(&cur);
    }
    return cur;
}

/* ========== EXEC 消息处理 @0x1400084d0 ========== */
/* mainW EXEC 分支调用: 解析消息文本, 处理 /InstallService /Service /RemoveService,
 * 构造 "~S/服务 参数" 或 "~I/Service ..." 命令文本交给 FUN_1400050C8 (PECMD_ServiceEntry) */
int FUN_1400084D0(WCHAR *msg)
{
    WCHAR first;        /* 首字符 (引号检测) */
    WCHAR *p;           /* 当前 */
    WCHAR *pCopy;       /* 参数副本 */
    WCHAR *pParsed;     /* 解析结果 */
    WCHAR *pOut;        /* 解析输出缓冲 */
    char marker;
    int i;

    first = *msg;
    p = msg;
    if ((*p < 9 || *p > 0xd) && *p != L' ') {
        p++;            /* 跳过命令名第一个字符后 (反编译: p+4/+5) */
    }
    PECMD_CheckDebugMsg();
    g_LastTick = GetTickCount();
    FUN_1400702B0(&pCopy, p);
    FUN_140063620(&pOut);
    FUN_14007A224(&g_Script, pCopy, &pOut, 0, 1);   /* 解析命令文本 */
    FUN_1400703E4(&pParsed, pOut);
    p = pParsed;
    FUN_14005B104(&pOut);
    FUN_14005B104(&pCopy);

    p = FUN_14000531C(p);       /* 跳空白 */

    if (StrCmpNIW(WSTR("/InstallService"), p, 0xf) == 0 &&
        ((p[0xf] > 8 && p[0xf] < 0xe) || p[0xf] == L' ' || p[0xf] == L'\0')) {
        marker = 'I';
        memcpy(p, WSTR("/Service       "), 0x1e);   /* 15 WCHAR, 覆盖为 /Service */
        p = FUN_14000531C(p + 0xf);
    } else {
        marker = (char)FUN_1400660AC("/Service", &p, 8);
        if (marker == 0) {
            marker = (char)FUN_1400660AC("/RemoveService", &p, 0xe);
            if (marker != 0) {
                FUN_140008B2C(p);
                ExitProcess(0);
            }
            FUN_14005B104(&pParsed);
            return 0;
        }
        marker = 'S';
    }

    /* 原命令以引号开头: 去掉尾部引号 (反编译: 从尾部倒数扫空白, 遇 '"' 截断) */
    if (first == L'"') {
        WCHAR *q;
        i = lstrlenW(p);
        for (q = p + i - 1; p < q; q--) {
            if ((*q < 9 || *q > 0xd) && *q != L' ') {
                if (p < q && *q == L'"') {
                    *q = L'\0';
                }
                break;
            }
        }
    }
    /* 构造 "~X" + 命令文本 */
    p[-1] = marker;
    p -= 2;
    *p = L'~';
    PECMD_ServiceEntry(p);
    ExitProcess(0);
    return 0;   /* 不可达 */
}
