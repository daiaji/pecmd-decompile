/* ====================================================================
 * core_scriptdep.c — 脚本执行依赖工具族
 *
 * 来源: PECMD原始.EXE (x64, ImageBase=0x140000000)
 *   随机种子         FUN_14001B510      @0x14001b510  (实现见 core_var3.c)
 *   XOR 加/解密      FUN_14001B5AC        @0x14001b5ac  (实现见 core_var3.c)
 *   去引号截断       FUN_14001D5F4       @0x14001d5f4
 *   脚本编码行插入   FUN_140024F20 @0x140024f20
 *   sysinit 执行     PECMD_RunSysInit   @0x140025180
 *   sysinit 检查     FUN_1400251AC  @0x1400251ac
 *   脚本子执行       PECMD_InvokeSubRoutine @0x140030dcc
 *   脚本 @CALL 插入  FUN_140030F1C @0x140030f1c
 *   脚本编码块执行   PECMD_ExecuteScriptBlock     @0x140031068
 *   token 推进       FUN_1400679DC      @0x1400679dc
 *   环境变量查询     FUN_14006F884        @0x14006f884
 *   带头串赋值       FUN_14007034C    @0x14007034c
 *   数字解析         PECMD_ParseUIntValue      @0x140074838
 *   主窗口刷新通知   FUN_14009BB28    @0x14009bb28
 *
 * 约定:
 *   - 字符串常量一律 WSTR() 宏 (WCHAR=uint16_t)
 *   - 脚本结构字段用 *(int64_t*)((uint8_t*)p+OFF) 形式访问
 *   - 未实现内部函数 extern 挂起 + TODO(verify), 不臆造
 * ==================================================================== */
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdbool.h>

#include "pecmd_defs.h"
extern WCHAR **FUN_14005B154(WCHAR **pp); /* @0x14005b154 */
extern void FUN_1400629B8(void *script, LPCWSTR key, LPCWSTR value); /* @0x1400629b8 */

/* ---- 全局变量 (DAT_14013xxxx, 本文件定义) ---- */
int32_t g_sysinitState = 0;      /* DAT_14013d058 sysinit 状态标志 */
WCHAR *g_sysinitName = NULL;     /* DAT_14013d060 sysinit 名称 */
int8_t g_flagA248 = 0;           /* g_charTableF 运行标志 TODO(verify) */

/* ---- 已有实现引用 ---- */
extern uint16_t FUN_14001B510(void);        /* @0x14001b510 core_var3.c */
extern int32_t FUN_14001B5AC(LPCWSTR buf, uint32_t key, int64_t n);  /* @0x14001b5ac core_var3.c */
extern uint64_t FUN_14001B4F8(const WCHAR *buf, WCHAR ch);         /* @0x14001b4f8 core_var3.c */
extern void *FUN_140017CDC(void *dst, void *src);                /* @0x140017cdc core_exec5.c */
extern void FUN_1400186BC(void *s, int64_t parent);              /* @0x1400186bc core_exec5.c */
extern void FUN_14006159C(void *script, uint64_t seed);           /* @0x14006159c core_exec2.c */
extern WCHAR *FUN_140024C48(WCHAR **pp, size_t *plen, uint32_t flags); /* @0x140024c48 core_token.c */

/* ---- 未实现依赖 (extern + TODO(verify), 不编造) ---- */
extern void PECMD_AllocStrSlot(void *ps);              /* @0x140063620 分配引用串容器 */
extern void *PECMD_ReallocBuffer(void *ptr, int64_t len); /* @0x140063224 重分配(带 OOM 处理) */
extern void *PECMD_AllocMagicString(LPCWSTR src);            /* @0x140070154 带 -8 头的串复制 */
extern void FUN_14001b660(void *script);            /* @0x14001b660 sysinit 前置 */
extern void PECMD_AutoMountStartup(void *script, LPCWSTR name);   /* @0x1400250f0 sysinit 执行 */
extern void PECMD_ParseShortStore(WCHAR **pp, int *out, WCHAR sep); /* @0x1400679b0 解析到分隔符 */
extern int64_t PECMD_EvalParenStripped(WCHAR **pp, int64_t *val);    /* @0x1400745c8 数字解析 */
extern void PECMD_CloseRestartByName(void *script, LPCWSTR path, void *win, int mode); /* @0x140082520 */
extern void PECMD_ExpandPathAlloc2(LPCWSTR src, WCHAR **out, int64_t *pos); /* @0x1400e3cd4 路径分隔符定位 */
extern void FUN_14004EAA8(void *script, int mode);   /* @0x14004eaa8 脚本结构清理 */
extern uint32_t PECMD_ParseScriptSegments(void *script, int a, int b, WCHAR **pc, void *sub, uint32_t flags); /* @0x140030420 */
extern uint32_t PECMD_LoadScriptFileSegment(WCHAR **buf, int a, LPCWSTR b, WCHAR **c, void *sub, uint32_t flags); /* @0x1400307c8 */
extern int64_t FUN_1400B638C(void *script, void *buf, void *a3, void *a4, uint64_t flags,
                             void *a6, void *a7);   /* @0x1400b638c 脚本执行主入口 */

/* 本文件内前置声明 (PECMD_ExecuteScriptBlock 先于定义调用) */
WCHAR **FUN_14007034C(WCHAR **ps, LPCWSTR src);  /* @0x14007034c 本文件 */

/* SHGetValueW/SHSetValueW 真实签名 (动态导入按 void(*)(void) 存储, 调用处强转) */
typedef LONG (*PECMD_SHGetValueW)(HKEY, LPCWSTR, LPCWSTR, DWORD *, void *, DWORD *);
typedef LONG (*PECMD_SHSetValueW)(HKEY, LPCWSTR, LPCWSTR, DWORD, const void *, DWORD);

/* ========== FUN_14001B510 @0x14001b510 ==========
 * 生成 16 位随机种子: 要求偶数、低 8 位非 0、bit15=1、汉明权重在 5..11。
 * 已实现于 core_var3.c (同地址), 本文件仅引用。
 */
extern uint16_t FUN_14001B510(void);

/* ========== FUN_14001B5AC @0x14001b5ac ==========
 * 用 key 对 buf 前 n 个字异或 (n<1 用 strlen+1); key<0 时先取随机种子。
 * 已实现于 core_var3.c (同地址), 本文件仅引用。
 */
extern int32_t FUN_14001B5AC(LPCWSTR buf, uint32_t key, int64_t n);

/* ========== FUN_14001D5F4 @0x14001d5f4 ==========
 * 去掉字符串前导双引号, 在下一个双引号处截断 (置 NUL)。
 * 无闭合引号则原样返回; 返回去引号后的串起点。
 */
WCHAR *FUN_14001D5F4(WCHAR *p)
{
    WCHAR c;
    WCHAR *q;

    while (*p == L'"') {
        p++;
    }
    c = *p;
    q = p;
    for (;;) {
        if (c == 0) {
            return p;
        }
        if (*q == L'"') {
            break;
        }
        q++;
        c = *q;
    }
    *q = 0;
    return p;
}

/* ========== FUN_140024F20 @0x140024f20 ==========
 * 把一行 (LOGS 指令 + @ENVI 指令) XOR 编码后插入脚本编码流。
 *   key    : XOR 密钥 (编码流分隔字符)
 *   pbuf   : 编码流缓冲指针变量 (自动重分配增长)
 *   line   : LOGS 指令的附加文本 (空串则只插 @ENVI 行)
 *   flags  : bit4(0x10)=@ENVI^ ForceLocal 行, bit5(0x20)=@ENVI^ EnviMode 行
 *   off    : 插入位置 (字符偏移, 编码流起点)
 * 返回新行起点 (字节地址/2 的字符偏移语义由调用方换算)。
 */
WCHAR *FUN_140024F20(uint32_t key, WCHAR **pbuf, LPCWSTR line,
                           uint32_t flags, int off)
{
    WCHAR *tmp = NULL;            /* 待编码行容器 */
    size_t linelen;
    size_t seglen;
    void *nb;
    WCHAR *ins;

    if (*line == 0 && flags == 0) {
        /* 无内容直接返回插入点 (反编译: *pbuf + off*2 字节地址) */
        return *(WCHAR **)pbuf + off;
    }
    PECMD_AllocStrSlot(&tmp);
    if (*line != 0) {
        FUN_14006375C(&tmp, WSTR("LOGS "));
        FUN_14006375C(&tmp, line);
        FUN_14006375C(pbuf, WSTR("\n"));
    }
    if ((flags & 0x10) != 0) {
        /* 原 FUN_1400702F0 复制常量串到临时容器再拼接, 等价直接拼接 */
        FUN_14006375C(&tmp, WSTR("@ENVI^ ForceLocal=1\n"));
    }
    if ((flags & 0x20) != 0) {
        FUN_14006375C(&tmp, WSTR("@ENVI^ EnviMode=1\n"));
    }
    linelen = (size_t)lstrlenW(tmp);
    /* 从 off 起找下一个 key 分隔字符, +1 含其自身 */
    seglen = (size_t)FUN_14001B4F8(*(const WCHAR **)pbuf + off, (WCHAR)key) + 1;
    FUN_14001B5AC(tmp, key, 0);
    nb = PECMD_ReallocBuffer(*(void **)pbuf, (int64_t)(seglen + linelen + (size_t)off) * 2 + 0x12);
    ins = (WCHAR *)((uint8_t *)nb + (size_t)off * 2);
    *(void **)pbuf = nb;
    /* 旧段 (原 off 起 seglen 字符) 后移 linelen 字符 (PECMD_MemMoveSafe memmove) */
    memmove((uint8_t *)nb + (linelen + (size_t)off) * 2, ins, seglen * 2);
    /* 新行拷到插入点 (PECMD_MemMoveForward; 反编译按字符计) */
    memcpy(ins, tmp, linelen * 2);
    /* 新行尾写 key 分隔 (PECMD_FillChar6 = 写 ushort) */
    *(uint16_t *)((uint8_t *)nb + (linelen + seglen + (size_t)off) * 2) = (uint16_t)key;
    FUN_14005B104(&tmp);
    return ins;
}

/* ========== PECMD_RunSysInit @0x140025180 ==========
 * 执行 sysinit 前置处理 + 执行 sysinit 命名脚本。
 */
void PECMD_RunSysInit(void *script, LPCWSTR name)
{
    FUN_14001b660(script);        /* TODO(verify) @0x14001b660 */
    PECMD_AutoMountStartup(script, name);  /* TODO(verify) @0x1400250f0 */
}

/* ========== FUN_1400251AC @0x1400251ac ==========
 * sysinit 一次性检查: 读注册表 "SysStartuped.load" 标记,
 * 未标记 (读失败或值为 0) 则写入标记并执行 sysinit。
 */
void FUN_1400251AC(void *script)
{
    LPCWSTR name = g_sysinitName;       /* DAT_14013d060 */
    DWORD type = 0;                     /* 反编译 local_18 (类型输出) */
    DWORD data = 0;                     /* 反编译 local_res10 (数据缓冲) */
    DWORD size = 4;                     /* 反编译 local_res20 */
    DWORD newval = 0xffffffff;          /* 反编译 local_res18 (初值 -1) */
    LONG rc;

    g_sysinitState |= 7;                /* DAT_14013d058 */
    g_sysinitName = NULL;               /* DAT_14013d060 */
    rc = ((PECMD_SHGetValueW)g_pSHGetValueW)(HKEY_LOCAL_MACHINE,
            WSTR("SOFTWARE\\PELOGON\\RAMDATA"), WSTR("SysStartuped.load"),
            &type, &data, &size);
    if (rc != 0 || data == 0) {
        newval = 1;
        ((PECMD_SHSetValueW)g_pSHSetValueW)(HKEY_LOCAL_MACHINE,
            WSTR("SOFTWARE\\PELOGON\\RAMDATA"), WSTR("SysStartuped.load"),
            REG_DWORD, &newval, sizeof(DWORD));
        PECMD_RunSysInit(script, name);
    }
}

/* ========== PECMD_InvokeSubRoutine @0x140030dcc ==========
 * 按模板执行子脚本: 复制模板结构, 注入 &&CurDir + XOR 分隔符表,
 * 交给 PECMD_ParseScriptSegments 执行。
 *   script : 宿主脚本表
 *   tmpl   : 子脚本模板结构
 *   flags  : 高 16 位 = XOR 密钥, 低 16 位执行标志
 * 返回 (执行结果 & 0xffff) | flags。
 */
uint32_t PECMD_InvokeSubRoutine(void *script, void *tmpl, uint32_t flags)
{
    WCHAR *nls = NULL;                  /* local_res10 "\n" */
    WCHAR *cwd = NULL;                  /* local_res8 当前目录 */
    WCHAR *v98 = NULL;                  /* local_98 容器 */
    uint8_t sub[0xf0];                  /* local_108 脚本副本 (反编译 128B TODO(verify) 放大) */
    uint16_t key = (uint16_t)(flags >> 16);
    uint64_t pos;
    uint32_t r;

    FUN_1400702B0(&nls, WSTR("\n"));
    pos = FUN_14001B4F8(*(const WCHAR **)script, (WCHAR)key);
    PECMD_AllocWStringBuffer(&cwd, 0x20a);
    GetCurrentDirectoryW(0x208, cwd);
    FUN_140017CDC(sub, tmpl);
    FUN_1400186BC(sub, (int64_t)(intptr_t)tmpl);
    /* 反编译 local_e8 = *(undefined4*)(tmpl+4) 未见消费, 省略 */
    FUN_1400629B8(sub, WSTR("&&CurDir"), cwd);
    /* 反编译 local_c0 = key 未见消费, 省略 */
    FUN_14006159C(sub, (uint64_t)(flags >> 16 & 0xffff));
    r = PECMD_ParseScriptSegments(script, 0, (int)pos, &nls, sub, flags);   /* TODO(verify) @0x140030420 */
    EnterCriticalSection(&g_csInit);
    v98 = NULL;                         /* 反编译 local_98[1]=0 (死写) TODO(verify) */
    LeaveCriticalSection(&g_csInit);
    FUN_14004EAA8(sub, 0);              /* TODO(verify) @0x14004eaa8 脚本结构清理 */
    FUN_14005B104(&v98);
    FUN_14005B104(&cwd);
    FUN_14005B104(&nls);
    return (r & 0xffff) | flags;
}

/* ========== FUN_140030F1C @0x140030f1c ==========
 * 在编码流 key 分隔位置 + 0x10 字符头处插入 "@CALL ** name token" 行。
 *   key : XOR 密钥
 *   buf : 编码流缓冲指针变量 (重分配增长)
 *   name: @CALL 名字 (空则不插入, 返回原缓冲)
 *   a4  : 命令行剩余 (NextToken 取第一个 token 拼到行尾)
 * 返回新行起点。
 */
WCHAR *FUN_140030F1C(uint32_t key, void **buf, LPCWSTR name, int64_t a4)
{
    WCHAR *line = NULL;
    WCHAR *cur = (WCHAR *)(intptr_t)a4;
    WCHAR *tok;
    WCHAR *ins;
    uint64_t pos;
    size_t clen;
    void *nb;

    if (name == NULL || *name == 0) {
        return *(WCHAR **)buf;
    }
    pos = FUN_14001B4F8(*(const WCHAR **)buf, (WCHAR)key);
    tok = FUN_140024C48(&cur, NULL, 1);
    FUN_1400702B0(&line, WSTR("@CALL ** "));
    FUN_14006375C(&line, name);
    FUN_14006375C(&line, tok);
    /* 反编译 local_28/local_30 长度累计仅写入未见消费, 省略 */
    clen = (size_t)lstrlenW(line);
    FUN_14001B5AC(line, key, 0);
    /* iVar4 = pos + 0x10: 跳过 key 段 + 16 字符头 */
    nb = PECMD_ReallocBuffer(*(void **)buf, (int64_t)(clen + 0x11 + pos + 0x10) * 2 + 2);
    *(void **)buf = nb;
    ins = (WCHAR *)((uint8_t *)nb + (pos + 0x10) * 2);
    memcpy(ins, line, (clen + 1) * 2);
    /* 行尾写 key 分隔 (PECMD_FillChar6 = 写 ushort) */
    *(uint16_t *)((uint8_t *)ins + (clen + 1) * 2) = (uint16_t)key;
    FUN_14005B104(&line);
    return ins;
}

/* ========== PECMD_ExecuteScriptBlock @0x140031068 ==========
 * 把命令脚本写入编码流并执行 (EXEC 块)。
 *   script : 宿主脚本表
 *   cmd    : 脚本文件路径 (参与 &&CurDir 提取; flags&8 时执行后删除)
 *   a3     : 执行参数命令行 (传给 @CALL 插入行)
 *   flags  : bit3(8)=执行后删源文件, bit8(0x100)/bit9(0x200) 透传,
 *            bit5(0x20)=ForceLocal 行, bit6(0x40)=EnviMode 行 (见 FUN_140024F20)
 *   a5     : @CALL 名字
 *   a6     : LOGS 附加行
 * 返回 0 成功; 1 编码流为空; 其他 GetLastError。
 */
DWORD PECMD_ExecuteScriptBlock(void *script, LPCWSTR cmd, LPCWSTR a3, uint32_t flags,
                      LPCWSTR a5, LPCWSTR a6)
{
    WCHAR *l198 = NULL;              /* local_198 编码流容器 */
    WCHAR *l178 = NULL;              /* local_178 命令副本 */
    WCHAR *l1a0 = NULL;              /* local_1a0 "\n" */
    WCHAR *l1a8 = NULL;              /* local_1a8 目录名 */
    WCHAR *v8 = NULL;                /* local_e8 容器 */
    uint8_t sub[0x100];              /* local_158 脚本副本 (反编译 64B TODO(verify) 放大) */
    uint16_t key;
    int64_t pos;
    int bVar1, bVar13;
    uint32_t l180;
    uint32_t r;
    DWORD ret;
    WCHAR *p;
    WCHAR *p2;
    int64_t v48;
    void *pv7;
    /* bVar13==9 时 +0x88/+0x90 区域交换缓冲 (初值反编译未显示 TODO(verify)) */
    uint8_t save90[0x2e];
    uint32_t v88 = 0;
    uint8_t swap90[0x2e] = {0};

    PECMD_AllocStrSlot(&l198);
    FUN_1400702B0(&l178, cmd);
    l180 = flags & 0xfffffff7;
    bVar1 = (flags & 0x200) ? -1 : 0;
    bVar13 = bVar1 & 9;
    FUN_1400702B0(&l1a0, WSTR("\n"));
    key = FUN_14001B510();
    FUN_140017CDC(sub, script);
    FUN_1400186BC(sub, (int64_t)(intptr_t)script);
    FUN_1400186BC(sub, (int64_t)(intptr_t)script);   /* 反编译二次调用(冗余) */
    FUN_1400702B0(&l1a8, cmd);
    {
        int64_t pos2 = 0;
        PECMD_ExpandPathAlloc2(cmd, &l1a8, &pos2);   /* TODO(verify) @0x1400e3cd4 取目录分隔位置 */
        if (pos2 != 0) {
            *(uint16_t *)(pos2 - 2) = 0;    /* 截断到最后一个分隔符前 */
        }
    }
    FUN_1400629B8(sub, WSTR("&&CurDir"), l1a8);
    FUN_14006159C(sub, (uint64_t)key);
    *l198 ^= key;                       /* 编码流首字符 = key */
    r = PECMD_LoadScriptFileSegment(&l198, 0, l178, &l1a0, sub, (uint32_t)key << 16); /* TODO(verify) */
    if (r == 0xffffffffu) {
        ret = GetLastError();
    } else {
        FUN_14007034C(&l1a0, NULL);
        p = l198;                       /* pWVar11 初值 */
        pos = (int64_t)FUN_14001B4F8(l198, (WCHAR)key);
        ret = 0;
        if (pos > 0) {
            /* key=0 无变换, 忠实还原反编译调用 */
            FUN_14001B5AC(p, 0, pos + 2);
            if ((flags & 8) != 0) {
                DeleteFileW(cmd);
            }
            if (p == NULL) {
                ret = 1;
            } else {
                if ((int8_t)g_flagA248 < 0) {
                    g_flagA248 = 1;
                }
                p2 = FUN_140030F1C(key, (void **)&l198, a5,
                                         (int64_t)(intptr_t)a3);
                /* +0x48 处保存 8 字节 (执行前无条件读取) TODO(verify) */
                v48 = ((int64_t *)script)[9];
                if (bVar13 == 9) {
                    int64_t t = ((int64_t *)script)[0x11];     /* +0x88 */
                    *(uint32_t *)((uint8_t *)script + 0x88) = v88;
                    v88 = (uint32_t)t;
                    memcpy(save90, (uint8_t *)script + 0x90, 0x2e);
                    memcpy((uint8_t *)script + 0x90, swap90, 0x2e);
                    memcpy(swap90, save90, 0x2e);
                    *(uint16_t *)((uint8_t *)script + 0x48) = key;
                }
                {
                    WCHAR *p3 = FUN_140024F20(key, &l198, a6, l180,
                                (int)((size_t)((char *)p2 - (char *)l198) >> 1));
                    /* bVar13 != 0 与 bVar1&1 等价, 合并为 bVar13 ? script : NULL */
                    pv7 = bVar13 ? (void *)script : NULL;
                    FUN_1400B638C(script, l198, (void *)a3, l178,
                                  (uint64_t)((r & 2) | ((uint32_t)key << 16) | l180),
                                  p3, pv7);
                }
                if (bVar13 == 9) {
                    int64_t t = ((int64_t *)script)[0x11];
                    *(uint32_t *)((uint8_t *)script + 0x88) = v88;
                    v88 = (uint32_t)t;
                    memcpy(save90, (uint8_t *)script + 0x90, 0x2e);
                    memcpy((uint8_t *)script + 0x90, swap90, 0x2e);
                    memcpy(swap90, save90, 0x2e);
                    *(uint16_t *)((uint8_t *)script + 0x48) = (uint16_t)v48;
                }
                ret = 0;
            }
        }
    }
    FUN_14005B104(&l1a8);
    FUN_14004EAA8(sub, 0);          /* TODO(verify) 脚本结构清理 */
    FUN_14005B104(&v8);
    FUN_14005B104(&l1a0);
    FUN_14005B104(&l178);
    FUN_14005B104(&l198);
    return ret;
}

/* ========== FUN_1400679DC @0x1400679dc ==========
 * 解析 token (分隔符 sep) 后推进游标一位。
 *   pp  : 解析游标 (推进)
 *   out : 解析结果输出
 */
void FUN_1400679DC(WCHAR **pp, int *out, WCHAR sep)
{
    PECMD_ParseShortStore(pp, out, sep);    /* TODO(verify) @0x1400679b0 解析到分隔符 */
    if (**pp != 0) {
        (*pp)++;
    }
}

/* ========== FUN_14006F884 @0x14006f884 ==========
 * 查询环境变量到动态串 out (自动扩容到实际长度)。
 */
void FUN_14006F884(LPCWSTR name, WCHAR **out)
{
    DWORD n;

    PECMD_AllocString(out, 0x104);
    **out = 0;
    n = GetEnvironmentVariableW(name, *out, 0x104);
    if (n > 0 && **out == 0) {
        /* 0x104 缓冲不足, 按返回值扩容重查 */
        PECMD_AllocString(out, n + 2);
        GetEnvironmentVariableW(name, *out, n + 1);
    }
}

/* ========== FUN_14007034C @0x14007034c ==========
 * 带头串赋值: 用 PECMD_AllocMagicString 复制 src 覆盖 *ps, 并释放旧串 (-8 头)。
 *   src 为 NULL 时仅释放旧值。
 * 与 FUN_1400702B0 (不释放旧值) 的区别: 本函数带 -8 长度头分配 + 释放旧值。
 */
WCHAR **FUN_14007034C(WCHAR **ps, LPCWSTR src)
{
    WCHAR *old = *ps;

    *ps = NULL;
    if (src != NULL) {
        *ps = (WCHAR *)PECMD_AllocMagicString(src);  /* TODO(verify) @0x140070154 带头串复制 */
    }
    if (old != NULL) {
        HeapFree(g_hHeap, 0, (uint8_t *)old - 8);
    }
    return ps;
}

/* ========== PECMD_ParseUIntValue @0x140074838 ==========
 * 数字解析包装: 成功 (解析到有效数字) 时回写 *out 并返回 true。
 */
bool PECMD_ParseUIntValue(WCHAR **ps, int *out)
{
    int64_t val = *out;
    int64_t n = PECMD_EvalParenStripped(ps, &val);    /* TODO(verify) @0x1400745c8 数字解析 */

    if (n > 0) {
        *out = (int)val;
        return true;
    }
    return false;
}

/* ========== FUN_14009BB28 @0x14009bb28 ==========
 * 通知主窗口刷新 (PostMessage 0x43d)。
 *   script   : 宿主脚本表
 *   withPath : 非 0 时先向主窗口对象追加路径并调 PECMD_CloseRestartByName
 * 受 g_csInit 临界区保护。
 */
void FUN_14009BB28(void *script, int withPath)
{
    WCHAR bs[2];
    int64_t win;

    EnterCriticalSection(&g_csInit);
    win = *(int64_t *)((uint8_t *)script + 0x40);   /* +0x40 主窗口对象 TODO(verify) */
    if (win != 0 && *(int8_t *)(win + 0x121) != (int8_t)0x81) {
        if (withPath != 0 && *(int64_t *)(win + 0x1c8) < 1 &&
            *(int32_t *)(win + 0x1d0) != 0) {
            bs[0] = L'\\';
            bs[1] = 0;
            PECMD_CloseRestartByName(script, bs, (void *)(intptr_t)win, 1);   /* TODO(verify) @0x140082520 */
        }
        PostMessageW(*(HWND *)(win + 0x20), 0x43d, 0, 0);  /* 0x43d = PECMD 内部刷新消息 */
    }
    LeaveCriticalSection(&g_csInit);
}