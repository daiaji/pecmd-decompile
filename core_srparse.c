/* ====================================================================
 * core_srparse.c — ScriptRunA 前缀指令解析（PECMD_RunCommand 段1）@0x140031454
 *
 * 来源: PECMD原始.EXE PECMD_RunCommand @0x140031454
 *   巨型函数 ScriptRunA 拆分任务第一段: 反编译 batch_b8c.txt 第 112-296 行
 *   （序言 *qk / *sysinit 检测 + LAB_1400316b5 前缀指令循环）。
 *
 * 语义约定:
 *   - FUN_1400660AC 返回非 0 = 匹配（已推进 pp 并跳空白）; 0 = 不匹配
 *   - FUN_14005C788    返回 1 = 相等（完整词）; 0 = 不等
 *   - 反编译中 Ghidra 指针偏移（pthreadmbcinfo 字段）按 WCHAR 游标理解;
 *     241/260 行的"回退 N 字符"无法精确复刻, 用"回退到词起点"等价实现
 *
 * 签名说明（相对任务基线扩展 4 个输出参数, 段2 主展开路径消费）:
 *   p_m_flag   : local_res18 低字节 (m 前缀标志; 反编译 569 行消费)
 *   p_mem_flag : local_res20 低字节 (mem 前缀标志)
 *   p_outbuf   : local_258  (logs: 执行输出缓冲; 入口 FUN_140063620 初始化)
 *   p_qkmode   : cVar6 (*qk 前缀匹配结果, 供段2 sysinit_end 逻辑)
 *
 * 返回码约定:
 *   0  = 前缀解析正常结束, *pp 就绪, 调用方继续主展开路径 (LAB_140031887)
 *   sysinit  已处理: *psysinit_name 非空 && *p_sysinit=true, flags|=0x100,
 *                   已调 FUN_1400629B8(script,"&&__UDM.sysinit","1")
 *   2sysinit 已处理: g_sysinitState=1, 窗口已隐藏, 同上 SetVar
 *   （反编译中 lVar41 初始 1 且仅主展开路径 384/389 行修改, 属段2, 此处恒返 0）
 *
 * TODO(段2, 反编译 170-173 行 sysinit_end, 在主展开路径开头执行):
 *   if (*psysinit_name != NULL && *p_qkmode == 0 && g_sysinitState == 0) {
 *       g_sysinitState = 3;
 *       g_sysinitName = *psysinit_name;    (DAT_14013d060, extern WCHAR*)
 *   }
 * TODO(段2): 序言 116-120 行 local_180/local_188/local_198/local_178/
 *   local_1a0/local_230 初值（g_hInst 等）由主展开路径自行初始化。
 * ==================================================================== */
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

#include "pecmd_defs.h"
extern void FUN_1400629B8(void *script, LPCWSTR key, LPCWSTR value); /* @0x1400629b8 */

/* ---- extern 声明 ---- */
extern int FUN_1400660AC(const char *s, WCHAR **pp, int n);   /* @0x1400660ac core_thread.c */
extern WCHAR *FUN_140024C48(WCHAR **pp, size_t *plen, uint32_t flags); /* @0x140024c48 core_token.c */
extern int64_t FUN_14007A224(void *script, WCHAR *line, WCHAR **out, int mode, uint8_t opt); /* @0x14007a224 core_execline.c */
extern DWORD FUN_1400195F0(void *script, int64_t timeout, int maxmsg, void *param4); /* @0x1400195f0 core_thread.c */
extern void FUN_140063620(WCHAR **ps);   /* @0x140063620 分配引用串容器 */
extern int32_t g_sysinitState;           /* DAT_14013d058 sysinit 状态 */

/* 2sysinit 窗口隐藏链（未实现, 仅声明 + TODO(verify)） */
extern void PECMD_ResetScriptChain(void *script, void *arg2);   /* @0x14001b3a0 */
extern uint64_t PECMD_GetObjField20(void *script);           /* @0x14005dec4 取窗口句柄 */
extern void PECMD_BringWindowToFront(HWND hwnd);                  /* @0x14005ded4 */

/* ========== 前缀指令解析（ScriptRunA 段1）@0x140031454 ==========
 * 解析命令行前缀指令并设置标志位。pp 为命令行游标（解析后推进）。
 * 输出位: bit3(8)=del, bit2(4)=g_DefEnv 词(分隔符检测),
 *   bit4(0x10)=local, bit5(0x20)=EnviMode, bit6(0x40)=ncd,
 *   bit8(0x100)=sysinit 前缀已处理, bit9/bit2(0x204)=this / * 前缀。
 * 详细签名/返回码约定见文件头注释。 */
int64_t PECMD_SrParsePrefix(void *script, WCHAR **pp, int *flags, int *flags2,
                            uint8_t *p_m_flag, uint8_t *p_mem_flag,
                            WCHAR **psysinit_name, bool *p_sysinit,
                            WCHAR **p_outbuf, int *p_qkmode)
{
    WCHAR lead;             /* WVar23: 期望的前缀首字符 ('*' 或 '-') */
    int saved_flags2;       /* local_294: 反编译 LAB_140031908 (282-284) 保存的 flags2 快照 */
    WCHAR *word_start;      /* 当前词起点 (241/260 行回退用) */

    /* ---- 序言 (112-148 行) ---- */
    FUN_14005B154(pp);                            /* 115 PECMD_SkipLeadingControls */
    FUN_140063620(p_outbuf);                        /* 121 local_258 */
    *p_sysinit = false;                             /* 122 bVar3 */
    *psysinit_name = NULL;                          /* 112 pwVar36 */
    *flags = 0;                                     /* 134-135 pwVar43 */
    *flags2 = 0;                                    /* 134-135 pwVar37 */
    *p_m_flag = 0;                                  /* 125 local_res18 */
    *p_mem_flag = 0;                                /* 124 local_res20 */
    saved_flags2 = 0;                               /* 126 local_294 */

    /* 123: *qk 前缀检测 (匹配时推进 pp; cVar6 供段2 sysinit_end) */
    *p_qkmode = FUN_1400660AC("*qk", pp, 3);

    /* 128-131: 前缀首字符 */
    lead = L'*';
    if (**pp == L'-') {
        lead = L'-';
    }

    /* 132-148: *sysinit 检测 (命中则记录 sysinit 名并设置内部变量) */
    if (FUN_1400660AC("*sysinit", pp, 8)) {
        lead = L'*';
        if (**pp == L'-') {                         /* 138-141 */
            lead = L'-';
            *flags = 0;
            *flags2 = 0;
        }
        /* LAB_14003168e (143-145) */
        *psysinit_name = (WCHAR *)WSTR("sysinit");
        *p_sysinit = true;
        /* LAB_14003169a (146-148): 设置 &&__UDM.sysinit + 标志 */
        FUN_1400629B8(script, WSTR("&&__UDM.sysinit"), WSTR("1"));
        *flags |= 0x100;
    }

    /* ---- LAB_1400316b5 (149-296): 前缀指令循环 ---- */
    for (;;) {
        word_start = *pp;                           /* 154 检测前词起点 */
        if (lead != **pp) break;                    /* 154 → 主展开路径 */

        (*pp)++;                                    /* 211 跳过首字符 */

        if (FUN_1400660AC("del", pp, 3)) {      /* 212-216 */
            *flags |= 8;
            continue;
        }

        if (FUN_1400660AC((const char *)g_DefEnv, pp, 0)) {  /* 217 */
            /* g_DefEnv 词: n=0 时 StrICmpN 仅做分隔符检测
             * (当前字符为空白/结束即匹配, pp 跳空白) */
            *flags |= 4;                            /* 295 */
            continue;                               /* 296 do-while */
        }

        /* 218-294: 内层指令匹配链 */
        if (FUN_1400660AC("*", pp, 1) ||        /* 219 */
            FUN_1400660AC("this", pp, 4)) {     /* 222 */
            *flags |= 0x204;                        /* 223 */
            *flags2 |= 0x200;                       /* 224 */
            saved_flags2 = *flags2;                 /* LAB_140031908 (284) */
            continue;
        }
        if (FUN_1400660AC("2sysinit", pp, 8)) { /* 227 */
            g_sysinitState = 1;                     /* 229 */
            PECMD_ResetScriptChain(script, NULL);            /* 230 TODO(verify) */
            {
                HWND hwnd = (HWND)PECMD_GetObjField20(script);    /* 231 TODO(verify) */
                PECMD_BringWindowToFront(hwnd);                /* 232 TODO(verify) */
                FUN_1400195F0(script, 5, 0, 0);    /* 233 */
                ShowWindow(hwnd, 0);                /* 234 */
            }
            *flags2 = saved_flags2;                 /* 235 */
            /* LAB_14003169a: 设置 &&__UDM.sysinit + 标志 */
            FUN_1400629B8(script, WSTR("&&__UDM.sysinit"), WSTR("1"));
            *flags |= 0x100;
            continue;
        }
        if (FUN_1400660AC("sysinit", pp, 7)) {  /* 238-239 */
            *psysinit_name = (WCHAR *)WSTR("sysinit");
            *p_sysinit = true;
            FUN_1400629B8(script, WSTR("&&__UDM.sysinit"), WSTR("1"));
            *flags |= 0x100;
            continue;
        }
        if (lead != **pp) {                         /* 240 */
            *pp = word_start;                       /* 241 回退到词起点 */
            break;                                  /* 242 → 主展开路径 */
        }
        (*pp)++;                                    /* 244 */

        /* 245-293: mem / m / local / EnviMode / ncd / logs: 链 */
        if (FUN_1400660AC("mem", pp, 3)) {      /* 245 匹配 */
            *p_mem_flag |= 1;                       /* 290 */
        } else if (FUN_1400660AC("m", pp, 1)) { /* 247 匹配 */
            *p_m_flag |= 1;                         /* 286 */
        } else if (FUN_1400660AC("local", pp, 5)) {     /* 249 匹配 */
            *flags |= 0x10;                         /* 279-280 */
            *flags2 |= 0x10;
            saved_flags2 = *flags2;                 /* LAB_140031908 */
        } else if (FUN_1400660AC("EnviMode", pp, 8)) {  /* 251 匹配 */
            *flags |= 0x20;                         /* 275-276 */
            *flags2 |= 0x20;
            saved_flags2 = *flags2;                 /* LAB_140031908 */
        } else if (FUN_1400660AC("ncd", pp, 3)) {       /* 253 匹配 */
            *flags |= 0x40;                         /* 255 */
            continue;                               /* 256 */
        } else if (FUN_14005C788("logs:", *pp, 5)) {       /* 258 是 logs: */
            /* 263-273: 提取并执行 logs: 参数行 */
            *pp += 5;                               /* 263 跳过 "logs:" */
            {
                size_t toklen = 0;
                WCHAR *tok = *pp;                   /* 265 local_158 */
                *pp = FUN_140024C48(&tok, &toklen, 1);    /* 266 */
                if (toklen > 0) {                   /* 267 */
                    WCHAR *copy = NULL;
                    PECMD_StrCopyW(&copy, tok, (int64_t)toklen); /* 268 */
                    FUN_14007A224(script, copy, p_outbuf, 0, 1); /* 269 */
                    FUN_14005B104(&copy);           /* 270 */
                }
                FUN_14005B154(pp);                /* 272 PECMD_SkipLeadingControls */
            }
            continue;                               /* 273 */
        } else {
            /* 259-261: 非 ncd 非 logs: → 回退并跳出 */
            *pp = word_start;
            break;
        }
        continue;                                   /* 292-293 → 主循环 */
    }

    return 0;
}
