/* ====================================================================
 * core_scriptrun.c — ScriptRunA 命令行脚本调度（主体段）
 *
 *   PECMD_RunCommand @0x140031454   命令行处理器主流程
 *
 * 段1（前缀指令解析）在 core_srparse.c（PECMD_SrParsePrefix），
 * 本文件实现主展开路径（LAB_140031887 起）：
 *   *map: 前缀 / 引号 / 命令 token → 资源脚本执行 或 变量执行，
 *   最后 sysinit 收尾。深度依赖（脚本执行主入口 PECMD_RunScriptText、
 *   PECMD_InvokeSubRoutine / FUN_1400E7D58 ResDecode 等）extern 挂起。
 * ==================================================================== */
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>

#include "pecmd_defs.h"
#include "win32_stub.h"
extern WCHAR **FUN_14005B154(WCHAR **pp); /* @0x14005b154 */

/* 段1：前缀解析（core_srparse.c） */
extern int64_t PECMD_SrParsePrefix(void *script, WCHAR **pp, int *flags, int *flags2,
                                   uint8_t *p_m_flag, uint8_t *p_mem_flag, WCHAR **psysinit_name,
                                   bool *p_sysinit, WCHAR **p_outbuf,
                                   int *p_qkmode); /* @0x140031454 段1 */

/* core_execline.c 展开器 */
extern void FUN_14007BF44(void *script, WCHAR *line, WCHAR **out, int mode,
                          uint8_t opt); /* @0x14007bf44 */

/* 核心辅助 */
extern void *PECMD_GrowByteBuffer(void **ps, int64_t len);          /* @0x140063424 */
extern void *PECMD_HeapRealloc(void *ptr, size_t size);             /* @0x140063118 */
extern uint16_t PECMD_GenRandomSeed16(void);                        /* @0x14001b510 */
extern int64_t FUN_14001B5AC(void *buf, uint32_t key, int64_t len); /* @0x14001b5ac */
extern uint8_t *PECMD_VarLookup(void *script, LPCWSTR name, void *scope, int64_t len,
                                void **out);                           /* @0x140018978 */
extern int64_t PECMD_StrChrOffset(const WCHAR *s, WCHAR c);            /* @0x14001b4f8 */
extern WCHAR *FUN_140024C48(WCHAR **pp, size_t *plen, uint32_t flags); /* @0x140024c48 */
extern uint8_t *FUN_14001EA18(HMODULE mod, LPCWSTR id, LPCWSTR type, void **out,
                              uint32_t *flags_out);                    /* @0x14001ea18 */
extern void FUN_140017CDC(void *dst, void *src);                       /* @0x140017cdc */
extern void FUN_1400186BC(void *s, void *parent);                      /* @0x1400186bc */
extern void PECMD_InitObfuscatedKeywords(void *script, uint64_t seed); /* @0x14006159c */
extern void FUN_14004EAA8(void *script, int flag);                     /* @0x14004eaa8 */
extern int32_t g_sysinitState;                                         /* DAT_14013d058 */
extern WCHAR *g_sysinitName;                                           /* DAT_14013d060 */

/* 未实现（TODO(verify) 挂起） */
extern uint32_t PECMD_InvokeSubRoutine(void *buf, void *script, uint32_t flags); /* @0x140030dcc */
extern uint32_t FUN_1400E7D58(int64_t *ps, uint32_t flags);                      /* @0x1400e7d58 */
extern void *PECMD_PrependEnviHeader(uint32_t key, void **buf, LPCWSTR extra, uint32_t f,
                                     int off); /* @0x140024f20 */
extern WCHAR *PECMD_PrependCallSubLine(uint32_t key, void **buf, LPCWSTR name,
                                       int64_t a4);                         /* @0x140030f1c */
extern WCHAR *PECMD_UnquoteTokenInPlace(WCHAR *p);                          /* @0x14001d5f4 */
extern void PECMD_ParseIntSkipSepChar(WCHAR **pp, int64_t *out, WCHAR sep); /* @0x1400679dc */
extern bool PECMD_ParseUIntValue(WCHAR **pp, int64_t *out);                 /* @0x140074838 */
extern int64_t PECMD_RunScriptText(void *pScript, LPCWSTR pText, LPCWSTR pName, LPCWSTR pCurFile,
                                   uint32_t flags, LPCWSTR pFile,
                                   void *pPersist); /* @0x1400b638c core_execmain.c */
extern DWORD PECMD_ExecuteScriptBlock(void *script, LPCWSTR a2, LPCWSTR a3, uint32_t flags,
                                      LPCWSTR a5, LPCWSTR a6); /* @0x140031068 */
extern void FUN_14006F884(LPCWSTR name, WCHAR **out);          /* @0x14006f884 */
extern void *FUN_14007034C(void **ps, LPCWSTR src);            /* @0x14007034c */
extern void PECMD_CheckFirstStartupFlag(void *script);         /* @0x1400251ac */
extern void PECMD_RunSysInit(void *script, LPCWSTR name);      /* @0x140025180 */
extern void FUN_14009BB28(void *script, int flag);             /* @0x14009bb28 */

/* ========== PECMD_RunCommand @0x140031454 ==========
 * 命令行脚本调度主流程。cmdline 被就地解析（截断）。
 */
int64_t PECMD_RunCommand(void *script, WCHAR *cmdline)
{
    WCHAR *pp = cmdline;
    int flags = 0, flags2 = 0;
    uint8_t m_flag = 0, mem_flag = 0;
    WCHAR *sysinit_name = NULL;
    bool b_sysinit = false;
    WCHAR *outbuf = NULL;
    int qkmode = 0;
    int64_t DVar13 = 0;

    /* ---- 段1：前缀指令解析 ---- */
    PECMD_SrParsePrefix(script, &pp, &flags, &flags2, &m_flag, &mem_flag, &sysinit_name, &b_sysinit,
                        &outbuf, &qkmode);

    /* ---- 主展开路径（LAB_140031887）---- */
    {
        WCHAR *p = pp;
        WCHAR *pt2;
        int64_t lVar32 = 0;
        int64_t lVar12 = 0;
        uint32_t uVar31 = 0, uVar24 = 0, uVar34 = 0, uVar35 = 0;
        uint16_t uVar42 = 0;
        int64_t local_234 = 0;
        int64_t local_26c = 0x7fffffff;
        WCHAR *local_250 = NULL, *local_278 = NULL, *local_260 = NULL;
        WCHAR *local_1d0 = NULL, *local_1e8 = NULL, *local_1f8 = NULL;
        WCHAR *local_160 = NULL, *local_240 = NULL, *local_210 = NULL;
        WCHAR *local_1b8 = NULL, *local_150 = NULL, *local_228 = NULL;
        WCHAR *local_1f0 = NULL;
        HMODULE hmod = 0;
        WCHAR *local_1a0 = NULL;
        int64_t *local_138 = NULL;
        WCHAR *mapStr = NULL;
        uint64_t mapOff = 0;

        /* 156-169：引号组合/实例检测 */
        pt2 = p;
        if (*p == L'*')
            pt2 = p + 1;
        if (*pt2 == L'"' && pt2[1] == L'.' && (uint16_t)pt2[2] == 0x22 && pt2[3] == L'#') {
            /* ".# 组合 → g_hInstance 模式（TODO(verify): 原语义为版本串检测） */
            pt2 = pt2 + 3;
            lVar32 = (int64_t)(pt2 - p) >> 1;
        }

        /* 170-173：sysinit_end 检查 */
        if (sysinit_name != NULL && qkmode == 0 && g_sysinitState == 0) {
            g_sysinitState = 3;
            g_sysinitName = sysinit_name;
        }

        /* 174-198：*map: 前缀 + 命令 token */
        p = pt2;
        if (FUN_14005C788("*map:", p, 5) != 1 && *p == L'*') {
            p++;
        }
        pt2 = p;
        {
            size_t tokLen = 0;
            FUN_140024C48(&p, &tokLen, 0x21);
            lVar32 = (int64_t)tokLen;
        }
        {
            /* 引号/反斜杠标志（反编译 local_298） */
            int q1 = 0;
            if (*p == L'"')
                p++;
            if ((uint16_t)*p == 0x5c) {
                q1 = 1;
                if ((uint16_t)p[1] != 0x5c)
                    q1 = 0;
            }
            if (*p == L'*') {
                p++;
            }
            (void)q1;
        }
        PECMD_AllocWStringBuffer(
            (WCHAR **)&local_250,
            0); /* 反编译 PECMD_AllocStrSlot(&local_278)? 实为 local_250 构造 */
        /* 199-210：命令名拷贝（token 长度 lVar32） */
        if (lVar32 >= 0) {
            PECMD_AllocWStringBuffer((WCHAR **)&local_250, lVar32);
            if (local_250 != NULL && lVar32 > 0) {
                memcpy(local_250, pt2, (size_t)lVar32 * 2);
                local_250[lVar32] = 0;
            }
        }
        /* 引号模式判断（反编译 203-209） */
        if (*local_250 == L'"') {
            /* 引号 token：找结束引号 */
            WCHAR *q = local_250;
            while (*q != L'\0' && *q != L'"')
                q++;
            if (*q == L'"')
                *q = L'\0';
        }

        /* ---- 命令执行主路径（反编译 319-390）---- */
        {
            WCHAR *out2 = NULL;
            FUN_14007BF44(script, local_250, &out2, 0, 1);
            /* 320-343：结果处理（引号包裹/剥离） */
            {
                uint16_t u9 = (out2 != NULL) ? (uint16_t)*out2 : 0;
                WCHAR *pt = out2;
                if (u9 != 0x22) {
                    if (u9 == 0) {
                        /* 空结果 */
                    }
                    else {
                        /* 剥离空白直到引号或结束 */
                        while (u9 != 0) {
                            if ((u9 > 8 && u9 < 0xe) || u9 == 0x20)
                                break;
                            pt++;
                            u9 = (uint16_t)*pt;
                        }
                    }
                }
                /* 若结果以引号开头则保留 */
            }
            PECMD_FreeStrBuf(&out2);
        }

        /* ---- *map: 映射处理（反编译 344-390）---- */
        {
            size_t tokLen2 = 0;
            WCHAR *tok2 = FUN_140024C48(&p, &tokLen2, 0x20);
            WCHAR *mapOut = NULL;
            FUN_14007BF44(script, tok2, &mapOut, 0, 1);
            FUN_14006375C(&local_278, mapOut);
            PECMD_FreeStrBuf(&mapOut);
            if (FUN_14005C788("*map:", local_278, 5) == 1) {
                mapStr = local_278;
            }
        }
        if (mapStr != NULL) {
            /* 映射文件读取（反编译 358-390）：
             * 文件名由 mapStr+5 起；PECMD_ParseUIntValue 取设备大小（未实现）
             */
            if (PECMD_ParseUIntValue(&mapStr, (int64_t *)&mapOff) > 0) {
                void *mv = MapViewOfFile((HANDLE)(intptr_t)mapOff, 6, 0, 0, 8);
                if (mv != NULL) {
                    int64_t sz = *(int64_t *)mv;
                    UnmapViewOfFile(mv);
                    if (sz > 0) {
                        void *mv2 =
                            MapViewOfFile((HANDLE)(intptr_t)mapOff, 6, 0, 0, (size_t)(sz + 2));
                        if (mv2 != NULL) {
                            local_1a0 = (WCHAR *)mv2;
                            /* TODO(verify): 反编译 379-390 行映射数据拷贝与解码 */
                        }
                    }
                }
            }
        }

        /* ---- 资源脚本执行路径（反编译 391-752）---- */
        {
            size_t tokLen3 = 0;
            WCHAR *resTok = FUN_140024C48(&p, &tokLen3, 0x20);
            WCHAR *resName = NULL;
            uint32_t resFlags = 0;

            PECMD_AllocWStringBuffer((WCHAR **)&local_260, 0);
            FUN_1400702B0(&resName, resTok);
            /* 引号剥离 */
            if (*resName == L'"')
                resName++;
            if (resName[0] != L'\0' && resName[1] == L'"')
                resName[1] = 0;

            hmod = LoadLibraryExW(resName, 0, 2);
            if (hmod != 0) {
                uVar42 = PECMD_GenRandomSeed16();
                PECMD_AllocWStringBuffer((WCHAR **)&local_1e8, 0);
                {
                    WCHAR *rname = NULL;
                    FUN_1400702B0(&rname, resTok);
                    /* 资源名 = token 中 '#' 后的部分（TODO(verify)） */
                    FUN_14001EA18(hmod, rname, WSTR("PECMD"), (void **)&local_1e8, &resFlags);
                    PECMD_FreeStrBuf(&rname);
                }
                if (local_1e8 != NULL && *local_1e8 != L'\0') {
                    /* XOR 解码 + 脚本对象构造 */
                    int64_t len = (int64_t)lstrlenW(local_1e8);
                    uint64_t k = (uint64_t)(uint16_t)uVar42;
                    uint64_t kf = (k << 16) | (k & 0xff);
                    FUN_14001B5AC(local_1e8, (uint32_t)((uint16_t)uVar42 ^ (uint16_t)uVar42), len);
                    FUN_140017CDC(local_138, script);
                    FUN_1400186BC(local_138, script);
                    PECMD_InitObfuscatedKeywords(local_138, (uint64_t)uVar42);
                    if (PECMD_InvokeSubRoutine(&local_1e8, local_138, kf) == 0) {
                        /* TODO(verify): 反编译 707-721 的详细 flags 组合 */
                        DVar13 = PECMD_RunScriptText(script, local_1e8, local_278, local_1f8,
                                                     ((uint64_t)uVar42 << 16) | kf | 0x40,
                                                     local_240, NULL);
                    }
                    PECMD_FreeStrBuf(&local_1e8);
                }
                FreeLibrary(hmod);
            }
            PECMD_FreeStrBuf(&local_260);
        }

        /* ---- 变量执行路径（反编译 753-852）---- */
        {
            WCHAR *varName = NULL;
            size_t tokLen4 = 0;
            WCHAR *vTok = FUN_140024C48(&p, &tokLen4, 0x20);
            PECMD_AllocWStringBuffer((WCHAR **)&local_1b8, 0);
            FUN_1400702B0(&varName, vTok);
            if (*varName == L'&' || *(char *)((char *)script + 0xd) != '\0') {
                /* 变量表查找 */
                EnterCriticalSection(&g_csInit);
                {
                    uint8_t *node = PECMD_VarLookup(script, varName, NULL, -1, NULL);
                    if (node != NULL) {
                        int64_t vlen = *(int64_t *)((char *)node + 0x18);
                        PECMD_AllocWStringBuffer((WCHAR **)&local_210, vlen);
                        memcpy(local_210, *(void **)((char *)node + 8), (size_t)vlen);
                        local_210[vlen] = 0;
                    }
                }
                LeaveCriticalSection(&g_csInit);
            }
            else {
                FUN_14006F884(varName, &local_210);
            }
            if (local_210 != NULL) {
                FUN_1400E7D58((int64_t *)&local_210, 1);
                {
                    uint16_t seed = PECMD_GenRandomSeed16();
                    FUN_14001B5AC(local_210, 0, 0);
                    PECMD_InvokeSubRoutine(&local_210, script, 0);
                    {
                        int64_t pos = PECMD_StrChrOffset(local_210, 0);
                        FUN_14001B5AC(local_210, (uint32_t)seed, pos + 2);
                    }
                    FUN_1400702B0(&local_150, WSTR("**mem"));
                    DVar13 = PECMD_RunScriptText(script, local_210, local_278, local_150,
                                                 ((uint64_t)seed << 16) | 0x40, NULL, NULL);
                    PECMD_FreeStrBuf(&local_150);
                }
                PECMD_FreeStrBuf(&local_210);
            }
            PECMD_FreeStrBuf(&local_1b8);
        }

        /* ---- 收尾（反编译 853-871）---- */
        if (g_sysinitState == 3) {
            PECMD_CheckFirstStartupFlag(script);
        }
        if (b_sysinit) {
            g_sysinitState |= 1;
            PECMD_RunSysInit(script, WSTR("sysinit_end"));
        }
        FUN_14009BB28(script, 0);
        PECMD_FreeStrBuf(&local_1f8);
        PECMD_FreeStrBuf(&local_240);
        PECMD_FreeStrBuf(&local_260);
        PECMD_FreeStrBuf(&local_278);
        PECMD_FreeStrBuf(&outbuf);
        (void)flags;
        (void)flags2;
        (void)m_flag;
        (void)mem_flag;
        (void)uVar24;
        (void)uVar31;
        (void)uVar34;
        (void)uVar35;
        (void)local_1a0;
        (void)local_1d0;
        (void)local_160;
        (void)local_228;
        (void)local_1f0;
        (void)local_234;
        (void)local_26c;
        (void)lVar12;
        return (int)DVar13;
    }
}
