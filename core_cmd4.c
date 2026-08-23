/* ========== PECMD_CodeConvertCommand @0x140088300 ========== */
/* CODE 命令处理器 (编码转换 / 加密变换)
 *
 * 语义 (TODO(verify) 按调用点还原):
 *   模式A (普通):  CODE [*源编码][*nobom][*transonly][*jm] 源编码,数据,目标编码,输出
 *      源编码可带 * (数据为字面量/变量/脚本行) 或 ** (数据=变量值) 或 *** (数据=脚本行前 N 字节);
 *      目标编码带 * 时输出=变量名 (无 * 时输出=文件名/标准输出), ** 时按行追加;
 *      源/目标编码: ANSI/GBK(0x3a8)/BIG5(0x3b6)/UTF8(0xfde9)/UTF7(0xfde8)/
 *      UNICODE(0x4b0)/UNI/UNICODEB(0x4b1)/UNIB/BOM 或数字代码页;
 *      读文件→检测 BOM→转码→写 stdout(0)/文件/变量.
 *   模式B (*jm / *transonly):  CODE *jm 源变量|表达式
 *      按表达式对源变量值做字节/字/双字算术-逻辑变换 (累加器 acc 统计),
 *      操作符: + ^ > . Σ ** * & | ? ω - / o # 等; 结果可写回变量或 '?' 输出位置.
 *
 * 参数还原: script = 脚本对象 (void*), cmd = 命令参数串 (WCHAR*);
 *   返回 int64 状态 (0=成功, 1=参数/读文件错误, 0xffffffff80070057=非法编码).
 *
 * 依赖: 见文件头 extern 清单. 未实现函数保持 extern + TODO(verify).
 */
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>

#include "pecmd_defs.h"

/* ---- 已实现 (core_string.c / core_var2.c / core_var.c) ---- */
extern void   PECMD_GrowByteBuffer(void **ps, int64_t len);         /* @0x140063424 */
extern void   FUN_1400629B8(void *script, LPCWSTR key, LPCWSTR value);  /* @0x1400629b8 */
extern void PECMD_AppendFmtValue(void *script, uint64_t value, LPCWSTR key, LPCWSTR fmt); /* @0x1400668ec */
extern int32_t FUN_14005C7C4(const char *a, const WCHAR *w);   /* @0x14005c7c4 1=相等 */
extern char    FUN_1400660AC(const char *word, WCHAR **pp, int n); /* @0x1400660ac 非0=匹配并推进 */
extern WCHAR **FUN_14005B154(WCHAR **pp);                      /* @0x14005b154 */
extern int64_t FUN_140064A88(const WCHAR *p);                       /* @0x140064a88 */
extern void   *PECMD_VarLookup(void *script, LPCWSTR name, void *scope,
                             int64_t len, void **out);           /* @0x140018978 */
extern void    FUN_14007BF44(void *script, WCHAR *line, WCHAR **out,
                               int mode, uint8_t opt);           /* @0x14007bf44 */
extern void    PECMD_OpenFileHandle(HANDLE *out, LPCWSTR path, DWORD access, DWORD share,
                              LPSECURITY_ATTRIBUTES sa, DWORD disp, DWORD flags,
                              HANDLE tmpl);                      /* @0x140003864 */
extern WCHAR  *FUN_14001BE14(WCHAR *s);                       /* @0x14001be14 */
extern void   *PECMD_AllocSmallObject(void **ps);                          /* @0x140063344 小对象 */
extern void    FUN_140060A74(uint8_t *buf, int len);           /* @0x140060a74 */
extern int32_t FUN_14005B184(const char *a, const char *b, int n);/* @0x14005b184 0=相等 */
extern void PECMD_ZeroLenBuf(void *p);               /* @0x14005b0b8 */

/* ---- 未实现 (extern + TODO(verify)) ---- */
extern int64_t *PECMD_SplitTokenAssignVar(WCHAR **out, WCHAR **pp, uint32_t sep, int flag);
    /* @0x14007f6e4 按分隔符切分串, token 存 out, *pp 停在分隔符 */
extern void PECMD_SplitTokenTrimWs(int64_t *src, WCHAR **dst, int mode);
    /* @0x1400675b8 串拷入容器 */
extern void PECMD_ExtractTokenByDelim(int64_t *src, WCHAR **dst, int mode);
    /* @0x1400676e4 串拷入容器 (变体) */
extern void FUN_140003A20(void *script, WCHAR **out, int mode);
    /* @0x140003a20 取脚本当前命令串 */
extern void PECMD_SkipUntilDelim(WCHAR **pp, WCHAR ch1, WCHAR ch2);
    /* @0x14005b374 查找两字符之一, *pp 停在字符或串尾 */
extern void FUN_1400F429C(WCHAR **pp, WCHAR ch);
    /* @0x1400f429c thunk: 查找字符, *pp 停在字符或串尾 */
extern bool PECMD_ParseNumSkipWs(WCHAR **pp, int64_t *out);
    /* @0x140067cf4 数字解析, 非0=成功 */
extern int64_t FUN_1400a9a84(WCHAR **pp, int64_t *out);
    /* @0x1400a9a84 数字解析, >0=成功 */
extern int64_t PECMD_EvalParenStripped(WCHAR **pp, int64_t *out);
    /* @0x1400745c8 数字解析 (core_scriptdep.c 同签名) */
extern int64_t PECMD_SetFilePointer(HANDLE h, int64_t offset, DWORD method);
    /* @0x14005c674 SetFilePointer 包装, 返回新位置 */
extern int PECMD_WcharToByteDigits(void *out, LPCWSTR src);
    /* @0x140069314 WCHAR 串→字节缓冲, 返回字节数 */
extern void FUN_1400692d8(WCHAR **ps, LPCWSTR src, int len);
    /* @0x1400692d8 追加 len 字节到串 */
extern void PECMD_BytesToHexStr(WCHAR **ps, LPCSTR src, int len);
    /* @0x14006923c 追加 len 字节 (字节→WCHAR) 到串 */
extern void PECMD_VarWriteLine(void *script, LPCWSTR key, LPCWSTR data, int64_t len,
                          int64_t *pkey, char mode);
    /* @0x140075148 按行写变量 */
extern void PECMD_MapStringByLocale(LPCSTR src, WCHAR **out, uint32_t isBIG5);
    /* @0x1400693c0 GBK/BIG5 字节重映射 */

/* ---- .rdata BOM 常量 ---- */
static const uint8_t BOM_LE[2]   = { 0xFF, 0xFE };   /* DAT_140124128 UTF-16LE BOM */
static const uint8_t BOM_BE[2]   = { 0xFE, 0xFF };   /* DAT_14012412c UTF-16BE BOM */
static const uint8_t BOM_UTF8[3] = { 0xEF, 0xBB, 0xBF };  /* DAT_140124130 UTF-8 BOM */

/* 分隔符/空白判断 (反编译 (8<c<0xe)||c==0x20) */
static inline int CodeWs(WCHAR c) { return ((8 < (uint16_t)c) && ((uint16_t)c < 0xe)) || (c == 0x20); }

int64_t PECMD_CodeConvertCommand(void *script, WCHAR *args)
{
    /* ---- 通用容器/游标 ---- */
    WCHAR *bufScr = NULL;    /* local_70  SplitTok 输出容器 */
    WCHAR *sTok = NULL;      /* local_120 第一 token (源编码规格) */
    WCHAR *sArg2 = NULL;     /* local_100 第二 token (文件名/数据/变量) */
    WCHAR *sArg3 = NULL;     /* local_a0  第三 token (目标编码规格) */
    WCHAR *sArg4 = NULL;     /* local_140 第四 token (输出文件/变量名) */
    WCHAR *pTok;             /* local_180 游标 */
    WCHAR *pSplit = NULL;    /* local_c0  保存指针 */
    WCHAR *pName = NULL;     /* local_res20 首 token 末空白后的文件名起点 */
    uint8_t *data = NULL;    /* local_160 数据缓冲 */
    WCHAR *sStr = NULL;      /* local_res20 输出串容器 */
    WCHAR *sStr2 = NULL;     /* local_118 辅助串/字节缓冲 */
    WCHAR *sStr3 = NULL;     /* local_110 转换缓冲 */
    HANDLE hOut = 0;         /* local_98  输出句柄 */
    int64_t err = 0;         /* ppppppppppppWVar25 返回值 */
    int64_t *plVar9;
    DWORD written = 0;

    /* ---- 编码/标志 ---- */
    uint8_t flNobom = 0;     /* bVar42  *nobom */
    uint8_t flTrans = 0;     /* local_158 低字节 *transonly */
    uint8_t flJm = 0;        /* cVar26  *jm */
    uint8_t srcStar = 0;     /* local_163 源编码 '*' 前缀 */
    char    dstStar = 0;     /* local_178 目标编码 '*' 前缀 */
    char    srcBom = 0;      /* local_164 "BOM" 源编码 */
    int32_t srcType = 0;     /* iVar30 源编码类型 */
    int32_t srcCp = 0;       /* local_150 源代码页 */
    int32_t dstCp = 0;       /* local_144 目标代码页 */
    int32_t dstType = 0;     /* local_128[0] 目标编码类型 */
    int32_t dstFlag2 = 0;    /* local_138 */
    int32_t dstBomLen = 0;   /* local_174 BOM 长度 */
    int32_t specEmpty = 0;   /* iVar6 源编码为空 */
    int32_t specEmpty2 = 0;  /* local_168 */
    uint8_t dstNobom = 0;    /* local_res10 低字节 目标编码 '*' */
    int32_t skipCount = 0;   /* local_res20 低字节 源 ** / *** 跳过字节 */
    const uint8_t *bomBytes = NULL;   /* local_a8 输出 BOM 指针 */
    int32_t dataMode = 0;    /* cVar2 数据来源: 0=文件 1=变量 2=字面量 */
    int64_t datalen = 0;     /* ppppppppppppWVar18 数据长度 */
    int32_t nSmall = 0;      /* local_168 */
    uint8_t pFlags = 0;      /* local_res10 低字节 */
    (void)pFlags;

    PECMD_AllocWStringBuffer((WCHAR **)&bufScr, 0);
    err = 0;
    FUN_14005B154(&args);
    pTok = args;                 /* FUN_14005B154 */
    PECMD_AllocWStringBuffer((WCHAR **)&sTok, 0);
    PECMD_AllocWStringBuffer((WCHAR **)&sArg2, 0);
    PECMD_AllocWStringBuffer((WCHAR **)&sArg3, 0);
    PECMD_AllocWStringBuffer((WCHAR **)&sArg4, 0);
    pTok = args;
    plVar9 = PECMD_SplitTokenAssignVar(&bufScr, &pTok, 0x2c, 1);   /* 切首 token */
    PECMD_SplitTokenTrimWs(plVar9, &sTok, 0);

    /* ---- 首 token 末空白处截断: 前面是 *标志, 后面是文件名 ---- */
    {
        WCHAR *p = sTok;
        uint16_t u = (uint16_t)*p;
        WCHAR *lastws = NULL;
        while (u != 0) {
            if (CodeWs(u)) lastws = p;
            p++;
            u = (uint16_t)*p;
        }
        pName = lastws;
        if (lastws != NULL) {
            *lastws = 0;
            pName = lastws + 1;
        }
    }

    /* ---- 解析 *nobom / *transonly / *jm ---- */
    if (pName != NULL) {
        uint16_t u;
        WCHAR *p = sTok;
        for (;;) {
            u = (uint16_t)*p;
            if (!(u == 0x2a && p[1] != 0x2d)) break;
            if (!FUN_1400660AC("*nobom", &p, 6)) {        /* *nobom 未匹配 */
                if (!FUN_1400660AC("*transonly", &p, 10)) {
                    if (!FUN_1400660AC("*jm", &p, 3)) {   /* *jm 未匹配 */
                        u = (uint16_t)*p;
                        while (u != 0 && !CodeWs(u)) {
                            p++;
                            u = (uint16_t)*p;
                        }
                        FUN_14005B154(&p);
                    } else {
                        flJm = 1;
                    }
                } else {
                    flTrans = 1;                              /* local_158 低字节=1 */
                }
            } else {
                flNobom = 1;
            }
            pFlags = 0;
            u = (uint16_t)*p;
        }
        flJm = (uint8_t)(flJm ? 1 : 0);
    }

    /* ================= 模式A: 编码转换 ================= */
    if (flJm == 0) {
        /* ---- 解析 4 个 token: 源编码,文件名,目标编码,输出 ---- */
        pSplit = pTok + 1;
        if (*pTok == 0x2c) {
            pTok = pSplit;
            plVar9 = PECMD_SplitTokenAssignVar(&bufScr, &pTok, 0x2c, 1);
            PECMD_ExtractTokenByDelim(plVar9, &sArg2, 0);
            if (*pTok == 0x2c) {
                pTok = pTok + 1;
                plVar9 = PECMD_SplitTokenAssignVar(&bufScr, &pTok, 0x2c, 1);
                PECMD_SplitTokenTrimWs(plVar9, &sArg3, 0);
            }
        }
        if (*pTok == 0x2c) {
            pTok = pTok + 1;
            plVar9 = PECMD_SplitTokenAssignVar(&bufScr, &pTok, 0x2c, 1);
            PECMD_SplitTokenTrimWs(plVar9, &sArg4, 0);
        }
        if ((*sTok == 0) || (*sArg2 == 0) || (*sArg3 == 0) || (*sArg4 == 0)) {
            PECMD_FreeStrBuf(&sArg4);
            PECMD_FreeStrBuf(&sArg3);
            PECMD_FreeStrBuf(&sArg2);
            PECMD_FreeStrBuf(&sTok);
            PECMD_FreeStrBuf(&bufScr);
            return 1;
        }

        /* ---- 源编码规格解析 (local_120) ---- */
        {
            WCHAR *p = sTok;
            if (*p == 0x2a) {                     /* * / ** / *** 前缀 */
                if (p[1] == 0x2a) {
                    skipCount = 2;
                    srcStar = (uint8_t)*p;
                    p += 2;
                    if (*p == 0x2a) {
                        skipCount = 3;
                        p += 1;
                    }
                } else {
                    srcStar = (uint8_t)*p;
                    p += 1;
                }
            }
            if (*p == 0x2d) p += 1;               /* '-' 前缀 */
            if (srcStar != 0) {                   /* 有编码名 */
                if (!FUN_14005C7C4("BOM", p)) {          /* BOM: 源编码 "BOM" → 自动检测 */
                    srcBom = 1;
                    specEmpty = 0;
                    srcType = 0;
                } else if (!FUN_14005C7C4("ANSI", p)) {  /* ANSI: 默认 */
                } else if (!FUN_14005C7C4("GBK", p)) {
                    srcCp = 0x3a8;
                } else if (!FUN_14005C7C4("BIG5", p)) {
                    srcCp = 0x3b6;
                } else if (!FUN_14005C7C4("UTF8", p)) {
                    srcCp = 0xfde9;
                    specEmpty = 0;
                    srcType = 0;
                } else if (!FUN_14005C7C4("UTF7", p)) {
                    srcCp = 0xfde8;
                } else if (!FUN_14005C7C4("UNICODE", p)) {
                    srcType = 0x4b0;
                } else if (!FUN_14005C7C4("UNI", p)) {
                    srcType = 0x4b0;
                } else if (!FUN_14005C7C4("UNICODEB", p)) {
                    srcType = 0x4b1;
                } else if (!FUN_14005C7C4("UNIB", p)) {
                    srcType = 0x4b1;
                } else if ((uint16_t)(*p - 0x30) < 10) {    /* 数字代码页 */
                    srcCp = (int32_t)FUN_140064A88(p);
                    specEmpty = 0;
                } else if (*p == 0) {                       /* 空规格 */
                    specEmpty = 1;
                    specEmpty2 = 1;
                    srcType = 1;
                } else {
                    goto L_invalid;                          /* 未知编码 → E_INVALIDARG */
                }
            }
        }

        /* ---- 目标编码规格解析 (local_a0) ---- */
        {
            WCHAR *p = sArg3;
            if (*p == 0x2a) {                     /* * / ** 前缀 */
                if (p[1] == 0x2a) {
                    dstStar = (char)*p;
                    p += 2;
                } else {
                    dstStar = (char)*p;
                    p += 1;
                }
                pFlags = (uint8_t)*sArg3;         /* local_res10 低字节 = '*' */
                specEmpty = 0;
            }
            if (*p == 0x2d) p += 1;               /* '-' 前缀 */
            if (dstStar != 0) {                   /* 有编码名 */
                if (!FUN_14005C7C4("ANSI", p)) {
                    dstCp = 0;
                } else if (!FUN_14005C7C4("GBK", p)) {
                    dstCp = 0x3a8;
                } else if (!FUN_14005C7C4("BIG5", p)) {
                    dstCp = 0x3b6;
                } else if (!FUN_14005C7C4("UTF8", p)) {
                    bomBytes = BOM_UTF8;
                    dstBomLen = 3;
                    dstCp = 0xfde9;
                } else if (!FUN_14005C7C4("UTF7", p)) {
                    dstCp = 65000;
                } else if (!FUN_14005C7C4("UNICODE", p)) {
                    dstType = 0x4b0;
                    bomBytes = BOM_LE;
                    dstBomLen = 2;
                } else if (!FUN_14005C7C4("UNI", p)) {
                    dstType = 0x4b0;
                    bomBytes = BOM_LE;
                    dstBomLen = 2;
                } else if (!FUN_14005C7C4("UNICODEB", p)) {
                    dstType = 0x4b1;
                    bomBytes = BOM_BE;
                    dstBomLen = 2;
                } else if (!FUN_14005C7C4("UNIB", p)) {
                    dstType = 0x4b1;
                    bomBytes = BOM_BE;
                    dstBomLen = 2;
                } else if ((uint16_t)(*p - 0x30) < 10) {    /* 数字代码页 */
                    dstCp = (int32_t)FUN_140064A88(p);
                } else if (*p == 0) {                       /* 空规格 */
                    dstType = 1;
                    dstFlag2 = 1;
                } else {
                    goto L_invalid;
                }
            }
        }
        if (flNobom) bomBytes = NULL;             /* *nobom 不写 BOM */

        /* ============ 源数据获取 ============ */
        if (srcStar == 0) {
            /* 读文件 (源编码无 * 前缀, sArg2 为文件名) */
            HANDLE hFile = 0;
            uint8_t *filebuf;
            int64_t fsize;
            WCHAR *fn;
            int32_t skip;
            int32_t bytesRead;

            pSplit = pTok;                        /* local_c0 */
            pTok = pSplit;
            plVar9 = PECMD_SplitTokenAssignVar(&bufScr, &pTok, 0x2c, 1);
            PECMD_SplitTokenTrimWs(plVar9, &sArg2, 0);
            fn = FUN_14001BE14(sArg2);
            PECMD_OpenFileHandle(&hFile, fn, 0x80000000, 7, NULL, 3, 0x80, 0);
            if (hFile != 0) {
                (void)PECMD_SetFilePointer(hFile, 0, 2);        /* 移到末尾 */
                fsize = PECMD_SetFilePointer(hFile, 0, 1);      /* 得大小 */
                (void)PECMD_SetFilePointer(hFile, 0, 0);        /* 移到开头 */
                filebuf = NULL;
                PECMD_GrowByteBuffer((void **)&filebuf, (int64_t)((int32_t)fsize + 10));
                filebuf[0] = 0;
                filebuf[1] = 0;
                filebuf[2] = 0;
                {
                    DWORD got = 0xffffffff;
                    ReadFile(hFile, filebuf, (DWORD)fsize, &got, 0);
                    bytesRead = (int32_t)got;
                }
                data = filebuf;
                if (bytesRead >= 0) {
                    data[bytesRead] = 0;
                    data[bytesRead + 1] = 0;
                    data[bytesRead + 2] = 0;
                    /* ---- BOM 检测 (源编码) ---- */
                    skip = 0;
                    if (srcBom == 0 && srcType != 0x4b0) {
                        if (srcType == 0x4b1) {
                            if (FUN_14005B184((const char *)data, (const char *)BOM_BE, 2) == 0) {
                                skip = 2;
                                srcType = 0x4b1;
                            } else {
                                if (srcBom == 0 && srcCp == 0xfde9) {
                                    if (FUN_14005B184((const char *)data, (const char *)BOM_UTF8, 3) == 0) {
                                        skip = 3;
                                        srcCp = 0xfde9;
                                    }
                                }
                            }
                        } else if (srcCp == 0xfde9) {
                            if (FUN_14005B184((const char *)data, (const char *)BOM_UTF8, 3) == 0) {
                                skip = 3;
                                srcCp = 0xfde9;
                            }
                        }
                    } else {
                        if (FUN_14005B184((const char *)data, (const char *)BOM_LE, 2) == 0) {
                            skip = 2;
                            srcType = 0x4b0;
                        } else {
                            if (srcType == 0x4b1) {
                                if (FUN_14005B184((const char *)data, (const char *)BOM_BE, 2) == 0) {
                                    skip = 2;
                                    srcType = 0x4b1;
                                } else {
                                    if (srcBom == 0 && srcCp == 0xfde9 &&
                                        FUN_14005B184((const char *)data, (const char *)BOM_UTF8, 3) == 0) {
                                        skip = 3;
                                        srcCp = 0xfde9;
                                    }
                                }
                            } else if (srcBom == 0 && srcCp == 0xfde9 &&
                                       FUN_14005B184((const char *)data, (const char *)BOM_UTF8, 3) == 0) {
                                skip = 3;
                                srcCp = 0xfde9;
                            }
                        }
                    }
                    if (skip != 0) {
                        memmove(data, data + skip, (size_t)((int32_t)fsize - skip) + 3);
                        skipCount = skip;
                    }
                    datalen = (int64_t)(bytesRead - skip);
                    if (datalen < 0) datalen = 0;
                }
                CloseHandle(hFile);
                dataMode = 0;
            } else {
                goto L_err1;
            }
        } else if (skipCount == 0) {
            /* 源编码 * : sArg2 为字面量数据 */
            if (specEmpty == 0) {
                datalen = PECMD_WcharToByteDigits(&data, sArg2);   /* WCHAR→字节 */
            } else {
                PECMD_StrBldCopyWide((WCHAR **)&data, sArg2);
                datalen = (int64_t)lstrlenW(sArg2) * 2;
                PECMD_GrowByteBuffer((void **)&data, datalen + 1);
            }
            dataMode = 2;
            goto L_data_ok;
        } else {
            /* 源编码 ** / *** : sArg2 为变量名 / 数字 */
            WCHAR *src = sArg2;
            if (9 < (uint16_t)(*src - 0x30)) {       /* 非数字 → 变量名 */
            L_var_path:
                {
                    void *node = PECMD_VarLookup(script, src, NULL, -1, NULL);
                    if (node == NULL) {
                        PECMD_GrowByteBuffer((void **)&data, 8);
                        dataMode = 0;
                        datalen = 0;
                    } else {
                        datalen = (int64_t)(*(uint64_t *)((uint8_t *)node + 0x18) &
                                            0x3fffffffffffffff);
                        if (skipCount == 3) {
                            datalen = PECMD_WcharToByteDigits(&data, *(LPCWSTR *)((uint8_t *)node + 8));
                            dataMode = 2;
                            goto L_data_ok;
                        }
                        PECMD_GrowByteBuffer((void **)&data, datalen + 1);
                        memset(data + datalen, 0, 8);
                        memcpy(data, *(void **)((uint8_t *)node + 8), (size_t)datalen);
                        dataMode = 1;
                    }
                }
            L_data_ok:
                pSplit = sArg2;
                dataMode = 2;
                goto L_output;
            } else {
                int64_t n;
                WCHAR *q = src;
                if (!PECMD_ParseNumSkipWs(&q, &n) || *q != 0) goto L_var_path;
                datalen = n;
                if (skipCount == 3) {
                    PECMD_StrBldCopyWide((WCHAR **)&data, sArg2);
                    datalen = (int64_t)lstrlenW(sArg2) * 2;
                    PECMD_GrowByteBuffer((void **)&data, datalen + 1);
                    dataMode = 2;
                    goto L_data_ok;
                }
                datalen = -1;
                goto L_var_path2;
            }
        }

        /* 读文件后继续 (LAB_14008a38e) */
        pSplit = (WCHAR *)data;
        goto L_output;

    L_var_path2:
        pSplit = (WCHAR *)data;
        /* 长度修正: 按编码选 lstrlenA/W */
        if (srcType == 0) {
            datalen = (int64_t)lstrlenA((LPCSTR)data);
        } else {
            datalen = (int64_t)lstrlenW((LPCWSTR)data) * 2;
        }
        goto L_output;

    L_output:   /* LAB_14008a38e / LAB_14008a1a8 入口 */
        {
            int32_t srcCp2 = srcCp;
            if (datalen < 0) goto L_err1;
            /* ---- 2 次 BOM 检测 (字面量/变量数据) ---- */
            if (srcBom != 0 && dataMode != 0) {
                if (FUN_14005B184((const char *)data, (const char *)BOM_LE, 2) == 0) {
                    srcType = 0x4b0;
                } else if (FUN_14005B184((const char *)data, (const char *)BOM_BE, 2) == 0) {
                    srcType = 0x4b1;
                } else {
                    srcCp2 = (int32_t)(uint32_t)srcCp2;
                    if (FUN_14005B184((const char *)data, (const char *)BOM_UTF8, 3) == 0) {
                        srcCp = 0xfde9;
                    }
                }
            }
            srcCp = srcCp2;
            pSplit = (WCHAR *)data;
            PECMD_AllocWStringBuffer((WCHAR **)&sStr, 0);
            if (dstCp == srcCp && dstType == srcType) {
                /* ---- 无转换: 原样输出 ---- */
                if (dstStar == 0) {
                    if (dstNobom == 0) {
                        WriteFile((HANDLE)0, data, (DWORD)datalen, &written, 0);
                    } else {
                        LPCWSTR v;
                        if (dstFlag2 == 0) {
                            FUN_1400692d8(&sStr, (LPCWSTR)data, (int)datalen);
                            v = sStr;
                        } else {
                            v = sArg2;
                            if (specEmpty2 == 0) v = (LPCWSTR)data;
                        }
                        FUN_1400629B8(script, sArg4, v);
                    }
                } else {
                    PECMD_VarWriteLine(script, sArg4, (LPCWSTR)data, datalen & 0xffffffff,
                                  (int64_t *)&sArg4, '\0');
                }
                PECMD_FreeStrBuf(&sStr);
                PECMD_FreeStrBuf((WCHAR **)&data);
                goto L_done;
            }
            /* ---- 需转换 ---- */
            if (srcType == 0x4b1) {
                srcType = 0x4b0;
                FUN_140060A74(data, (int)datalen);
                datalen &= ~(int64_t)1;
            }
            hOut = 0;
            if (dstNobom == 0) {
                WCHAR *fn = FUN_14001BE14(sArg4);
                PECMD_OpenFileHandle(&hOut, fn, 0x40000000, 7, NULL, 4, 0x80, 0);
                if (hOut == 0) {
                    PECMD_FreeStrBuf(&sStr);
                    goto L_err1;
                }
            }
            sStr2 = NULL;
            /* ---- GBK↔BIG5 直接重映射 ---- */
            if ((dstCp == 0x3a8 && srcCp == 0x3b6) || (srcCp == 0x3a8 && dstCp == 0x3b6)) {
                uint8_t *old;
                dataMode = 0;
                if (flTrans) dataMode = 3;
                PECMD_AllocSmallObject((void **)&sStr3);
                PECMD_MapStringByLocale((LPCSTR)data, &sStr3, (uint32_t)(dstCp == 0x3b6));
                old = data;
                data = (uint8_t *)sStr3;
                sStr3 = (WCHAR *)old;
                datalen = (int64_t)lstrlenA((LPCSTR)data);
                if (dataMode == 3 && dstNobom == 0) {
                    if (datalen > 0) {
                        WriteFile(hOut, data, (DWORD)datalen, &written, 0);
                    }
                    PECMD_FreeStrBuf(&sStr3);
                    goto L_ab5e;
                }
                PECMD_FreeStrBuf(&sStr3);
            }
            /* ---- 字节源 → WCHAR (MB→WC) ---- */
            nSmall = (int32_t)(datalen / 2);
            {
                LPCWSTR wv = (LPCWSTR)pSplit;
                if (srcType == 0) {
                    int r;
                    PECMD_AllocStrSlot(&sStr);
                    wv = sStr;
                    r = (*g_pConvFunc)((uint32_t)srcCp, 0, (LPCSTR)data, (int)datalen,
                                       sStr, (int)datalen + 10);
                    if (r < 0) r = 0;
                    datalen = (int64_t)r * 2;
                    sStr[datalen / 2] = 0;
                    data = (uint8_t *)sStr;
                }
                /* 目标 UTF-16BE → 字节交换 */
                if (dstType == 0x4b1) {
                    FUN_140060A74(data, (int)datalen);
                }
                /* 目标为字节编码 → WC→MB */
                if (dstType < 1) {
                    if (dstFlag2 == 0) {
                        int32_t wc = (int32_t)(datalen / 2);
                        int32_t cap = wc * 6;
                        PECMD_GrowByteBuffer((void **)&sStr2, cap + 0xb);
                        {
                            uint8_t *mb = (uint8_t *)sStr2;
                            int mblen = WideCharToMultiByte((UINT)dstCp, 0, (LPCWSTR)data, wc,
                                                            (LPSTR)mb, cap + 10, NULL, NULL);
                            datalen = mblen;
                            if (mblen < 0) datalen = 0;
                            mb[datalen] = 0;
                            if (dstNobom == 0) {
                                if (datalen < 1) goto L_ab5e;
                                data = mb;
                                if (bomBytes != NULL) {
                                    WriteFile(hOut, bomBytes, (DWORD)dstBomLen, &written, 0);
                                }
                                WriteFile(hOut, data, (DWORD)datalen, &written, 0);
                                goto L_ab5e;
                            }
                            if (dstStar != 0) {
                                PECMD_VarWriteLine(script, sArg4, (LPCWSTR)mb,
                                              (int64_t)lstrlenA((LPCSTR)mb),
                                              (int64_t *)&sArg4, '\0');
                                goto L_ab5e;
                            }
                            PECMD_BytesToHexStr(&sStr, (LPCSTR)mb, (int)datalen);
                            FUN_1400629B8(script, sArg4, sStr);
                            goto L_ab5e;
                        }
                    }
                } else {
                    /* 目标 UNICODE */
                    if (dstStar != 0) {
                        PECMD_VarWriteLine(script, sArg4, (LPCWSTR)data, datalen & 0xffffffff,
                                      (int64_t *)&sArg4, '\0');
                        PECMD_FreeStrBuf(&sStr2);
                        PECMD_FreeStrBuf(&sStr);
                        PECMD_FreeStrBuf((WCHAR **)&data);
                        if (hOut != 0 && hOut != (HANDLE)-1) CloseHandle(hOut);
                        goto L_done;
                    }
                    if (dstNobom == 0) {
                        if (datalen < 1) goto L_ab5e;
                        if (bomBytes != NULL) {
                            WriteFile(hOut, bomBytes, (DWORD)dstBomLen, &written, 0);
                        }
                        WriteFile(hOut, data, (DWORD)datalen, &written, 0);
                        goto L_ab5e;
                    }
                    if (dstFlag2 == 0) {
                        WCHAR *sv = NULL;
                        PECMD_AllocWStringBuffer((WCHAR **)&sv, 0);
                        FUN_1400692d8(&sv, (LPCWSTR)data, nSmall);
                        FUN_1400629B8(script, sArg4, sv);
                        PECMD_FreeStrBuf(&sv);
                        goto L_ab5e;
                    }
                }
                FUN_1400629B8(script, sArg4, (LPCWSTR)(wv ? wv : (LPCWSTR)data));
            }
        L_ab5e:
            if (hOut != 0) FlushFileBuffers(hOut);
            PECMD_FreeStrBuf(&sStr2);
            PECMD_FreeStrBuf(&sStr);
            PECMD_FreeStrBuf((WCHAR **)&data);
            if (hOut != 0 && hOut != (HANDLE)-1) CloseHandle(hOut);
            PECMD_FreeStrBuf(&sArg4);
            PECMD_FreeStrBuf(&sArg3);
            PECMD_FreeStrBuf(&sArg2);
            PECMD_FreeStrBuf(&sTok);
            PECMD_FreeStrBuf(&bufScr);
            return 0;
        }
    }
    /* ================= 模式B: *jm 加密变换 ================= */
    else {
        /* ---- 局部变量 (模式B专用, 简化自 Ghidra 反编译) ---- */
        WCHAR *sLine = NULL;       /* 脚本当前行: "源变量|表达式" */
        WCHAR *pSep = NULL;        /* 分隔符位置 */
        WCHAR *pAfter = NULL;      /* 表达式起点 / 第二变量名 */
        WCHAR *sExp = NULL;        /* ExecLine2 展开后的表达式 */
        WCHAR *pCur = NULL;        /* 表达式游标 */
        WCHAR *pOut = NULL;        /* 输出变量名 (local_90) */
        int64_t n1 = -1;           /* 源偏移数字 */
        int64_t acc = 0;           /* 累加器 local_170 */
        uint32_t opChar = 0;       /* local_174 */
        uint32_t flags = 0;        /* local_res10 低 32 位 */
        uint32_t opMode = 6;       /* local_148 低字节 */
        int64_t nLen = 1;          /* local_d0 元素大小 */
        int64_t nSize2 = 1;        /* local_b8 @步长 */
        int64_t nChunk = 0;        /* local_e0 /块大小 */
        int64_t nCount2 = -1;      /* local_118 最大处理长度 */
        int64_t srcOff = 0;        /* local_c8 */
        int64_t srcLen = 0;        /* local_e8 */
        int64_t vNode2 = 0;        /* local_d8 第二变量节点 */
        uint8_t *srcData = NULL;   /* 源数据指针 */
        uint8_t *keyData = NULL;   /* 第二变量数据指针 */
        int64_t keyLen = 0;
        int elem = 1;
        int count = 0;
        int i;
        bool bNeg = false;
        void *node;
        uint64_t a, b, r;
        int64_t sa, sb;

        FUN_140003A20(script, &sLine, 0);
        n1 = -1;
        pSep = sLine;
        PECMD_SkipUntilDelim(&pSep, 0x7c, 0x2a);      /* 找 '|' 或 '*' */
        pAfter = pSep;
        if (*pSep != 0) {
            *pSep = 0;
            pAfter = pSep + 1;
        }
        if ((uint16_t)(*pAfter - 0x30) < 10) {
            FUN_1400a9a84(&pAfter, &n1);        /* 数字前缀 = 源偏移 */
            pAfter = NULL;
        } else {
            pAfter = (*pAfter != 0) ? pAfter : NULL;
        }
        FUN_14007BF44(script, pAfter, &sExp, 0, 1);

        pCur = sExp;
        acc = 0;
        pOut = NULL;
        if (*pCur == 0x2c) pCur++;
        FUN_14005B154(&pCur);
        opChar = (uint32_t)*pCur;
        if (opChar != 0) {
            /* ---- 运算符前缀 (Σ/ω/普通) ---- */
            if (opChar == 0x2b && pCur[1] == 0x2b) {   /* "++" -> Σ */
                pCur++;
                opChar = 0x3a3;
            } else if (opChar == 0x2a && pCur[1] == 0x2a) {  /* "**" -> ω */
                pCur++;
                opChar = 0x3a0;
            }
            if (opChar == 0x3a0) acc = 1;
            else if (opChar == 0x3f) acc = -1;
            else acc = 0;
            pCur++;
            FUN_14005B154(&pCur);
            pAfter = pCur;

            /* ---- 可选输出变量名 (仅 Σ/ω/?) ---- */
            if (opChar == 0x3a3 || opChar == 0x3a0) {
                if (*pCur != 0x2c) {
                    pOut = pCur;
                    FUN_1400F429C(&pCur, 0x2c);
                    if (*pCur != 0) {
                        *pCur = 0;
                        pCur++;
                    }
                }
            } else if (*pCur == 0x2c) {
                pCur++;
            }

            /* ---- '*' 标志 ---- */
            if (*pCur == 0x2a) {
                pCur++;
                flags |= 1;
            }

            /* ---- '&' 第二变量 / 数字源偏移 ---- */
            if (*pCur == 0x26) {
                pAfter = pCur + 1;
                FUN_1400F429C(&pCur, 0x2c);
                if (*pCur != 0) {
                    *pCur = 0;
                    pCur++;
                }
                vNode2 = (int64_t)PECMD_VarLookup(script, pAfter, NULL, -1, NULL);
                if (vNode2 != 0) {
                    keyData = *(uint8_t **)((uint8_t *)vNode2 + 8);
                    keyLen = (*(int64_t *)((uint8_t *)vNode2 + 0x18)) & 0x3fffffffffffffff;
                }
            } else {
                if (FUN_1400a9a84(&pCur, &srcOff) > 0) {
                    /* 数字源偏移已解析 */
                }
            }

            /* ---- 负偏移 ---- */
            bNeg = (*pCur == 0x2d);
            if (bNeg) pCur++;

            /* ---- 元素大小 / @步长 / /块大小 / 数量 ---- */
            if (PECMD_EvalParenStripped(&pCur, &nLen) > 0 && nLen > 0) {
                if (*pCur == 0x40) {
                    pCur++;
                    PECMD_EvalParenStripped(&pCur, &nSize2);
                }
                if (*pCur == 0x2f) {
                    pCur++;
                    PECMD_EvalParenStripped(&pCur, &nChunk);
                }
                if (*pCur != 0) pCur++;
                FUN_1400a9a84(&pCur, &srcOff);
                FUN_1400a9a84(&pCur, &nCount2);
            }

            /* ---- 取源变量数据 ---- */
            node = PECMD_VarLookup(script, sLine, NULL, -1, NULL);
            if (node != NULL) {
                srcData = *(uint8_t **)((uint8_t *)node + 8);
                srcLen = (*(int64_t *)((uint8_t *)node + 0x18)) & 0x3fffffffffffffff;
                if (srcData != NULL && srcLen > 0) {
                    elem = (int)(nLen > 8 ? 8 : (nLen > 4 ? 4 : (nLen > 2 ? 2 : 1)));
                    count = (int)(srcLen / elem);
                    if (nCount2 >= 0 && nCount2 < count) count = (int)nCount2;
                    if (srcOff < 0) srcOff = 0;
                    if (srcOff > srcLen) srcOff = srcLen;

                    /* ---- 运算符 -> opMode (与反编译 local_148 低字节对应) ---- */
                    switch (opChar) {
                    case 0x2b: case 0x23: opMode = 0; break;   /* + # */
                    case 0x5e: opMode = 1; break;              /* ^ */
                    case 0x3e: opMode = 2; break;              /* > 循环右移 */
                    case 0x2e: opMode = 3; break;              /* . 赋值 */
                    case 0x3a3: opMode = 4; break;             /* Σ 累加 */
                    case 0x2a: opMode = 5; break;              /* * 累乘 */
                    case 0x26: opMode = 6; break;              /* & */
                    case 0x7c: opMode = 7; break;              /* | */
                    case 0x3f: opMode = 8; break;              /* ? 测试 */
                    case 0x3a0: opMode = 9; break;             /* ω 计数 */
                    case 0x2d: opMode = 0xd; break;            /* - */
                    case 0x2f: opMode = 0xe; break;            /* / */
                    default: opMode = 6; break;
                    }

                    /* ---- 逐元素变换 (简化: 未实现反编译中的块置换/交错循环,
                     *      仅保留算术/逻辑内核, TODO(verify) 待汇编细查) ---- */
                    for (i = 0; i < count; i++) {
                        uint8_t *dst = srcData + srcOff + (int64_t)i * elem;
                        uint8_t *key = NULL;
                        if (keyData != NULL && keyLen > 0) {
                            key = keyData + (int64_t)(i % (int)(keyLen / elem)) * elem;
                        }
                        a = 0; b = 0;
                        memcpy(&a, dst, (size_t)elem);
                        if (key != NULL) memcpy(&b, key, (size_t)elem);
                        else b = (uint64_t)(uint8_t)flags;  /* 无第二变量时用 flags 低字节作立即数 TODO(verify) */
                        r = a;
                        switch (opMode) {
                        case 0:  r = a + b; break;
                        case 1:  r = a ^ b; break;
                        case 2:  r = (a >> 1) | (a << (elem*8 - 1)); break; /* 循环右移1位 TODO(verify) */
                        case 3:  r = b; break;
                        case 4:  acc += (int64_t)(int32_t)(uint32_t)b; break;
                        case 5:  acc *= (int64_t)(int32_t)(uint32_t)b; break;
                        case 6:  r = a & b; break;
                        case 7:  r = a | b; break;
                        case 8:  if ((a & b) != 0) { acc = (int64_t)(i + (srcOff / elem)); goto L_jm_found; } break;
                        case 9:  if ((a & b) != 0) acc++; break;
                        case 0xa: acc *= (int64_t)(int32_t)(uint32_t)b; break;
                        case 0xb: r = a * b; break;
                        case 0xc: r = a * b; break;
                        case 0xd: r = a - b; break;
                        case 0xe: if (b != 0) r = a / b; break;
                        case 0xf: sa = (int64_t)a; sb = (int64_t)b; if (sb != 0) r = (uint64_t)(sa / sb); break;
                        default: break;
                        }
                        memcpy(dst, &r, (size_t)elem);
                    }
                L_jm_found:
                    /* 若指定输出变量, 把累加器/命中位置写回 */
                    if (pOut != NULL) {
                        PECMD_AppendFmtValue(script, (uint64_t)acc, pOut, WSTR("%I64d"));
                    }
                }
            }
        }

        /* ---- 模式B清理 ---- */
        PECMD_FreeStrBuf(&sExp);
        PECMD_FreeStrBuf(&sLine);
        err = 0;
        goto L_done;
    }

L_invalid:
    err = (int64_t)0xffffffff80070057;
    goto L_done;

L_err1:
    err = 1;

L_done:
    PECMD_FreeStrBuf(&sArg4);
    PECMD_FreeStrBuf(&sArg3);
    PECMD_FreeStrBuf(&sArg2);
    PECMD_FreeStrBuf(&sTok);
    PECMD_FreeStrBuf(&bufScr);
    return err;
}
