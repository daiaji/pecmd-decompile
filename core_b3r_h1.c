/*
 * core_b3r_h1.c — Restored batch28 business functions (group h1)
 *
 *   PECMD_MsgCommand @0x140083274  MSG 参数解析 + 写控件 (对话框) 分发
 *   PECMD_ParseMsgDispatch @0x140083f74  MSG 参数解析 + 控件分发 (变体 1)
 *   PECMD_MsgDispatch @0x14008430c  MSG 参数解析 + 控件分发 (变体 2)
 *   PECMD_GenerateTextContent @0x1400858c4  文本/文件内容生成循环 (含 -format/row/col 处理)
 *   PECMD_ProcessEncodedFile @0x140086944  加解密/文件流处理主入口
 *   PECMD_GetDiskFreeSpace @0x1400944c4  磁盘/卷信息查询命令 (临界区保护)
 *   PECMD_NormalizePath @0x140094c04  路径规范化 (full/short/volume) 命令
 *
 * 说明:
 *   - FUN_140102a90(...) 是 memset 别名, 直接写 memset。
 *   - 所有 helper 仅 extern 声明 (由其他 core_*.c / link_stubs.c 提供), 不在本文件定义。
 *   - WPARAM/LPARAM 见 win32_stub.h; WSTR() 宏用于宽字符串字面量。
 */

#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#include "win32_stub.h"
#include "pecmd_defs.h"

/* ---- 本文件引用的全局 (link_stubs.c / core_globals.c 定义) ---- */
extern WCHAR g_szEmpty[];   /* 空串 (.rdata) */
extern uint32_t         g_randSeedAccum;     /* 随机种子累加 */
extern uint8_t          g_u8CCB1;     /* MAIN_DBG 日志标志 */
extern HANDLE           g_hStdIn;     /* 标准输入句柄 */
extern uint8_t          g_b24d00[];   /* 文件读缓冲区数据 (.rdata) */
extern int32_t g_aiDiskType[];   /* 磁盘类型缓存 */
extern uint32_t         g_u322570;     /* 卷信息格式串第一字 (int32) */
extern uint16_t         g_u162574;     /* 卷信息格式串第二字 (int16) */
extern uint16_t         g_u16127d30[];   /* "--xxx" 宽串常量 */
extern void (*PTR_FUN_140126540)(void);    /* 解密流 vtable 槽 */
extern void (*PTR_FUN_140126560)(void);    /* 加密流 vtable 槽 */

/* ---- 宽字符串 // CRT (不在 win32_stub 中的 API 在此补充) ---- */
extern int64_t _time64(int64_t *t);                                        /* time(NULL) */
extern BOOL    GetDiskFreeSpaceExW(LPCWSTR, ULARGE_INTEGER *, ULARGE_INTEGER *,
                                   ULARGE_INTEGER *);
extern DWORD   GetLongPathNameW(LPCWSTR, LPWSTR, DWORD);

/* ---- 字符串容器辅助 ---- */
extern void      FUN_140063620(void *out);                       /* @0x140063620 串容器初始化 */
extern WCHAR    *PECMD_AllocWStringBuffer(WCHAR **ps, int64_t count);       /* @0x140063694 分配串槽 */
extern WCHAR    *PECMD_AllocString(WCHAR **ps, int64_t count);       /* @0x140063720 PECMD_StrAlloc */
extern WCHAR    *PECMD_StrCopyW(void *ps, LPCWSTR src, int64_t len); /* @0x140063888 定长拷贝 */
extern void      FUN_14005b104(void *ps);                         /* @0x14005b104 释放串槽 */
extern WCHAR    *FUN_14005b154(WCHAR **pp);                       /* @0x14005b154 跳过空白 */
extern void      PECMD_StrDupAssign(WCHAR **ps, const WCHAR *src);     /* @0x1400702b0 赋值串 */
extern WCHAR    *PECMD_AssignString(WCHAR **ps, const WCHAR *src);     /* @0x14007034c 追加串 */
extern WCHAR    *FUN_1400703e4(int64_t *out, const WCHAR *src);   /* @0x1400703e4 拷贝串 */
extern WCHAR    *PECMD_SkipWCharUntil(WCHAR **pp, uint16_t ch);          /* 定位分隔符 (thunk) */
extern void      FUN_14001b3a0(int64_t *, int64_t *);             /* 默认参数获取 */
extern void      PECMD_ParseNumSkipWs(int64_t *, uint64_t *);            /* 数字写串 */

/* ---- 字符串比较/解析 ---- */
extern int64_t   FUN_14005c72c(const char *a, const WCHAR *w, int n); /* icmp 前缀 */
extern int64_t   PECMD_AsciiPrefixICmp(const char *s, const WCHAR *w, int n); /* 前缀比较 */
extern int32_t   PECMD_AsciiWideICmp(const char *a, const WCHAR *w);        /* 后缀比较 */
extern char      FUN_1400660ac(const char *tok, WCHAR **pp, int n);   /* token 消费 */
extern int64_t   PECMD_MatchPrefixN(uint16_t *a, WCHAR **pp, int n);       /* 前缀匹配 */
extern LPCWSTR   PECMD_StripTrailingSpaces(LPCWSTR s);                            /* 尾部剥离 */
extern WCHAR    *FUN_14001be14(WCHAR *s);                             /* 串标签/前缀查找 */

/* ---- 脚本/变量 ---- */
extern void      PECMD_RunCommandLine(void *script, WCHAR **str, int mode);  /* 变量展开 */
extern void      PECMD_SetVariable(int64_t *script, LPCWSTR key, LPCWSTR value); /* SetVar */
extern int64_t   PECMD_VarLookup(void *script, LPCWSTR name, void *scope,
                               int64_t len, void *p5);                /* 变量查找 */
extern void      FUN_14007bf44(void *ctx, WCHAR *src, WCHAR **out,
                               int mode, int flag);                   /* 变量替换 read */
extern int64_t   PECMD_ExpandCommandLine(void *ctx, WCHAR *src, WCHAR **out,
                               int mode, uint8_t flag);               /* 变量写入 */

/* ---- 参数拆分/数值 ---- */
extern WCHAR    *FUN_1400547bc(void *ctx, int64_t *pp, int64_t *out,
                               int c1, int c2);                       /* 拆分到串 */
extern void      FUN_1400545f8(void *script, WCHAR **p1, WCHAR **p2,
                               uint64_t c, int64_t d);                /* 路由解析 */
extern void      PECMD_SplitTokenTrimWs(int64_t *src, int64_t *dst, short c);  /* 按字符切分 */
extern void      PECMD_ExtractTokenByDelim(int64_t *, int64_t *, int);            /* 数值转串 */
extern void      PECMD_CopyUpToChar(int64_t *, int64_t *, uint32_t);       /* 字节转串 */
extern void      PECMD_ParseHashNumbers(int64_t *, int64_t);                   /* 数值写 */
extern int64_t  *PECMD_SplitTokenAssignVar(WCHAR **out, WCHAR **pp, uint32_t sep, int flag);
extern void      PECMD_ParseLtwhParams(int64_t *, uint32_t *, int *, int *, uint32_t *);
extern uint64_t  PECMD_ParseSignedNumber(short *);                              /* 解析数字 */
extern int64_t   FUN_1400745c8(WCHAR **pp, int64_t *out);            /* 解析整数 */
extern uint64_t  FUN_1400a9a84(WCHAR **pp, uint64_t *out);            /* 解析长度 */
extern uint16_t *PECMD_NextToken(int64_t *a, int64_t *b, uint32_t c);   /* 路径扩展 */
extern void      PECMD_ExpandBackslashNewline(const WCHAR *, char);                  /* 清空串 */
extern void      PECMD_ReadFileToWide(WCHAR *, int64_t *);                   /* 变量到串 */

/* ---- 写控件分发 (对话框) ---- */
extern void      PECMD_DispatchCreateControl(uint64_t, int64_t, WCHAR **, int, int, int, int,
                               WCHAR **, WCHAR **, uint32_t, int *, LPCWSTR, uint64_t);
extern void      FUN_140063ed4(WPARAM, int64_t, int64_t *, int, int, int, int,
                               int64_t *, int64_t *, uint32_t);
extern void      PECMD_InsertControlObject(WPARAM, int64_t, int64_t *, int, int, int, int,
                               int64_t *, int64_t *, uint32_t);

/* ---- 随机/时间 ---- */
extern int64_t   PECMD_NextRandomSeed(void);                       /* @0x14005e04c 随机数 */
extern void      FUN_140018d8c(void *ctx, LPCWSTR fmt, uint64_t a, uint64_t b); /* 日志 */

/* ---- 文件/流 ---- */
extern void      PECMD_OpenFileHandle(HANDLE *out, LPCWSTR path, DWORD access, DWORD share,
                               LPSECURITY_ATTRIBUTES sa, DWORD disp, DWORD flags,
                               HANDLE tmpl);                /* CreateFileW 封装 */
extern uint32_t  PECMD_GetDiskGeometry(LPCWSTR p, HANDLE h);        /* 文件系统类型 */
extern WCHAR    *PECMD_DriveTypeName(int param_1, LPWSTR param_2, int param_3); /* 卷信息串 */
extern int       PECMD_QueryDiskGeometry(HANDLE param_1, uint64_t *param_2, int param_3,
                               int param_4);                /* 磁盘 IO ctl */
extern uint64_t  PECMD_ReadFileToWideString(HANDLE param_1, int64_t *param_2, uint32_t param_3); /* 读文本 */
extern uint64_t  PECMD_CleanScriptText(short *param_1, int param_2, unsigned int param_3);  /* 清洗文本 */
extern LPWSTR    FUN_14005eefc(LPWSTR param_1, WCHAR *param_2);   /* 串拷贝 */
extern void      PECMD_ExpandDrivePath(LPCWSTR path, DWORD a, LPWSTR buf, LPWSTR *out);
extern int64_t   PECMD_DescribePartitionInfo(int64_t *param_1, LPCWSTR param_2, uint64_t param_3,
                               uint64_t param_4, LARGE_INTEGER *param_5,
                               uint32_t *param_6, char param_7);
extern int64_t   PECMD_StripDevicePrefix(int64_t param_1);            /* 变量值读取 */
extern void      PECMD_QuoteTokenString(int64_t *param_1, int64_t *param_2, int param_3);
extern void      FUN_1400e6d74(LPCWSTR param_1, uint64_t param_2); /* %I64u 格式化 */

/* ---- 流/加密流栈 ---- */
extern int64_t  *PECMD_InitStreamObject(int64_t *stream, int64_t handle);   /* 初始化流 */
extern void      PECMD_StreamClose(int64_t *stream);                   /* 关闭流 */
extern bool      PECMD_StreamReadByte(int64_t *stream, uint8_t *buf,
                               int64_t n, DWORD *read);            /* 流读 */
extern int64_t   PECMD_BufferedWriteByte(int64_t *stream, uint8_t *buf);     /* 流写 */
extern uint32_t *PECMD_ZeroStruct_bc98(uint32_t *param_1);                /* 摘要初始化 */
extern void      PECMD_DigestUpdate(uint32_t *param_1, uint64_t param_2); /* 摘要更新 */
extern void      PECMD_LzDecompress(int64_t param_1, uint64_t param_2);   /* 摘要收尾 */

/* ---- 内存搬移 (memcpy/memmove 类) ---- */

/* ---- 文本渲染 (PECMD_GenerateTextContent) ---- */
extern bool      PECMD_ParseHexOrDec(WCHAR **pp, uint64_t *size);   /* 解析十六进制大小 */
extern uint64_t  PECMD_ParseFirstWordToken(int64_t *a, int64_t *b, uint64_t *c); /* 取单词 */
extern LPCWSTR   PECMD_RegexReplace(int64_t *param_1, LPCWSTR param_2, WCHAR *param_3,
                               WCHAR *param_4, WCHAR *param_5, int64_t *param_6);
extern WCHAR    *PECMD_ResolveVariable(int64_t *a1, LPCWSTR a2, uint64_t *a3, uint32_t a4);
extern void      PECMD_ListDrives(int64_t *, WCHAR, int, int, uint32_t, LPCWSTR);

/* ==================================================================
 * @0x140083274  (uint64_t PECMD_MsgCommand(longlong*, WCHAR*, WPARAM, uint64_t))
 * MSG 参数解析: 解析 "-center/-right/-vcenter/-rich/-3D/-u2d" 修饰符,
 * 读取若干字段并分发给写控件函数 PECMD_DispatchCreateControl。
 * ================================================================== */
uint64_t PECMD_MsgCommand(int64_t *param_1, WCHAR *param_2, WPARAM param_3, uint64_t param_4)
{
    WCHAR WVar1;
    bool bVar2;
    int64_t lVar3;
    int64_t *plVar5;
    uint64_t uVar4;
    uint64_t uVar11;
    uint32_t uVar8;
    uint32_t uVar10;
    uint16_t uVar9;
    int iVar7;
    uint32_t *puVar6;
    WCHAR WVar13;
    WCHAR *pWVar14;
    int local_res8[2];
    WCHAR *local_res10;
    WPARAM local_res18;
    uint64_t local_res20;
    LPCWSTR local_c8;
    LPCWSTR local_c0;
    int local_b8[2];
    int local_b0;
    int local_ac;
    uint32_t local_a8;
    uint32_t local_a4;
    int64_t local_a0;
    LPCWSTR local_98;
    WCHAR *local_90;
    uint64_t local_88;
    LPCWSTR local_80;
    int64_t local_78;
    WCHAR *local_70;
    int64_t local_68;
    int64_t *local_60;
    int64_t local_58;

    local_res10 = param_2;
    local_res18 = param_3;
    local_res20 = param_4;
    FUN_140063620(&local_68);
    local_58 = 0;
    local_60 = param_1;
    if (param_3 == 0) {
        FUN_14001b3a0(param_1, (int64_t *)0x0);
        param_3 = (WPARAM)param_1[8];
        local_res18 = param_3;
        if (param_3 == 0) {
            local_res18 = 0;
            uVar4 = (uint64_t)(int64_t)(int32_t)0xffffffff80070057;
            goto LAB_140083a22;
        }
    }
    uVar11 = 0;
    WVar13 = L'-';
    bVar2 = false;
    while (true) {
        WVar1 = *local_res10;
        uVar8 = (uint32_t)uVar11;
        if ((WVar1 == L'\0') || (((8 < (uint16_t)WVar1 && ((uint16_t)WVar1 < 0xe)) ||
                                  (WVar1 == L' ')))) {
            break;
        }
        if (WVar1 == L'-') {
            uVar11 = (uint64_t)(uVar8 | 0x8000000);
        }
        else if (WVar1 == L'|') {
            uVar11 = (uint64_t)(uVar8 | 0x10000000);
        }
        else if (WVar1 == L'+') {
            uVar11 = (uint64_t)(uVar8 | 0x4000000);
        }
        else if (WVar1 == L'.') {
            bVar2 = true;
        }
        local_res10 = local_res10 + 1;
    }
    uVar8 = uVar8 ^ 0x18000000;
    FUN_14005b154((WCHAR **)&local_res10);
    iVar7 = 0x2c;
    local_70 = local_res10;
    uVar11 = uVar11 & 0xffffffff;
    uVar10 = (uint32_t)uVar11;
    PECMD_SkipWCharUntil(&local_70, 0x2c);
    WVar1 = *local_70;
    *local_70 = (WCHAR)uVar11;
    local_90 = local_70;
    local_res8[0] = (uint16_t)WVar1;
    local_88 = uVar11;
    if (WVar13 == *local_res10) {
        do {
            pWVar14 = local_res10;
            lVar3 = FUN_14005c72c("-center", (const WCHAR *)local_res10, 7);
            if ((char)lVar3 == '\0') {
                lVar3 = FUN_14005c72c("-right", (const WCHAR *)pWVar14, 6);
                if ((char)lVar3 == '\0') {
                    lVar3 = FUN_14005c72c("-vcenter:", (const WCHAR *)pWVar14, 8);
                    if ((char)lVar3 == '\0') {
                        uVar4 = (uint64_t)PECMD_AsciiPrefixICmp("-vcenter:", (const WCHAR *)pWVar14,
                                                        iVar7 + -0x23);
                        if ((char)uVar4 == '\0') {
                            lVar3 = FUN_14005c72c("-rich", (const WCHAR *)pWVar14, 5);
                            if (((char)lVar3 == '\0') &&
                                (lVar3 = FUN_14005c72c("-rich", (const WCHAR *)pWVar14, 5),
                                 (char)lVar3 == '\0')) {
                                lVar3 = FUN_14005c72c("-3D", (const WCHAR *)pWVar14, 3);
                                if ((char)lVar3 == '\0') {
                                    lVar3 = FUN_14005c72c("-u2d", (const WCHAR *)pWVar14, 4);
                                    if ((char)lVar3 != '\0') {
                                        uVar8 = uVar8 | 0x1000000;
                                    }
                                }
                                else {
                                    uVar8 = uVar8 | 0x200000;
                                }
                            }
                            else {
                                uVar8 = uVar8 | 0x800000;
                            }
                        }
                        else {
                            local_res10 = pWVar14 + 9;
                            uVar8 = uVar8 | 0x2000000;
                            FUN_140063620(&local_c8);
                            FUN_14007bf44(param_1, local_res10, (WCHAR **)&local_c8, 0, 1);
                            local_98 = local_c8;
                            PECMD_ParseNumSkipWs((int64_t *)&local_98, &local_88);
                            local_88 = (uint64_t)(char)local_88;
                            FUN_14005b104((void *)&local_c8);
                            pWVar14 = local_res10;
                        }
                    }
                    else {
                        uVar8 = uVar8 | 0x2000000;
                    }
                }
                else {
                    uVar11 = 0x20000000;
                }
            }
            else {
                uVar11 = 0x40000000;
            }
            uVar10 = (uint32_t)uVar11;
            WVar1 = *pWVar14;
            while (((WVar1 != L'\0' &&
                     (((uint16_t)WVar1 < (uint16_t)(WCHAR)(iVar7 + -0x23) ||
                       (0xd < (uint16_t)WVar1)))) &&
                    (WVar1 != L' '))) {
                pWVar14 = pWVar14 + 1;
                local_res10 = pWVar14;
                WVar1 = *pWVar14;
            }
            FUN_14005b154((WCHAR **)&local_res10);
        } while (WVar13 == *local_res10);
        param_3 = local_res18;
        WVar1 = (WCHAR)local_res8[0];
    }
    *local_90 = WVar1;
    uVar8 = uVar8 | uVar10;
    FUN_140063620(&local_a0);
    FUN_140063620(&local_c0);
    FUN_140063620(&local_98);
    FUN_140063620(&local_c8);
    PECMD_StrDupAssign((WCHAR **)&local_80, g_szEmpty);
    uVar4 = 0;
    local_78 = (int64_t)local_res20;
    local_b0 = -0x80000000;
    local_ac = -0x80000000;
    local_a8 = 0x80000000;
    local_a4 = 0x80000000;
    local_90 = (WCHAR *)0;
    local_b8[0] = 0;
    local_res8[0] = 0;
    local_res18 = 0;
    uVar11 = 0;
    WVar13 = *local_res10;
    if (WVar13 == L'*') {
        local_res10 = local_res10 + 1;
        FUN_14005b154((WCHAR **)&local_res10);
    }
    FUN_1400547bc(param_1, (int64_t *)&local_res10, (int64_t *)&local_a0, 0x2c, 0);
    if (*local_res10 == L'\0') {
        FUN_14005b104((void *)&local_80);
        FUN_14005b104((void *)&local_c8);
        FUN_14005b104((void *)&local_98);
        FUN_14005b104((void *)&local_c0);
        FUN_14005b104((void *)&local_a0);
        uVar4 = 1;
    }
    else {
        plVar5 = PECMD_SplitTokenAssignVar((WCHAR **)&local_68, (WCHAR **)&local_res10, 0x2c, 1);
        PECMD_ParseLtwhParams(plVar5, (uint32_t *)&local_90, local_b8, local_res8, (uint32_t *)&local_res18);
        if (*local_res10 == L',') {
            local_res10 = local_res10 + 1;
            plVar5 = PECMD_SplitTokenAssignVar((WCHAR **)&local_68, (WCHAR **)&local_res10, 0x2c, 1);
            PECMD_ExtractTokenByDelim(plVar5, (int64_t *)&local_c0, 0);
        }
        if (*local_res10 != L'\0') {
            local_res10 = local_res10 + 1;
            FUN_1400545f8(param_1, (WCHAR **)&local_res10, (WCHAR **)&local_80, L',', 0);
            if (*local_res10 != L'\0') {
                puVar6 = (uint32_t *)PECMD_SplitTokenAssignVar((WCHAR **)&local_68, (WCHAR **)&local_res10,
                                                   0x2c, 1);
                uVar11 = PECMD_ParseSignedNumber((short *)(uintptr_t)*puVar6);
            }
        }
        uVar9 = (uint16_t)uVar11;
        if (*local_res10 == L',') {
            local_res10 = local_res10 + 1;
            plVar5 = PECMD_SplitTokenAssignVar((WCHAR **)&local_68, (WCHAR **)&local_res10, 0x2c, 1);
            PECMD_ParseHashNumbers(plVar5, (int64_t)local_b0);
            if (*local_res10 == L',') {
                local_res10 = local_res10 + 1;
                plVar5 = PECMD_SplitTokenAssignVar((WCHAR **)&local_68, (WCHAR **)&local_res10, 0x2c, 1);
                PECMD_SplitTokenTrimWs(plVar5, (int64_t *)&local_c8, 0x2c);
            }
        }
        local_a4 = 0x80000000;
        local_a8 = 0x80000000;
        if (!bVar2) {
            PECMD_ExpandBackslashNewline(local_c0, '\0');
            uVar8 = uVar8 | 0x1000000;
        }
        if ((*local_c0 == L'\0') && (*local_80 != L'\0')) {
            PECMD_ReadFileToWide((WCHAR *)local_80, (int64_t *)&local_c0);
        }
        *((WCHAR *)local_80) = L'\0';
        if ((0 < (short)uVar9) && ((uVar11 & 7) != 0)) {
            uVar9 = uVar9 | 8;
        }
        if ((-1 < local_b0) && (local_ac < 0)) {
            local_ac = g_helpVerFlag;
        }
        if (WVar13 != L'*') {
            param_1 = *(int64_t **)((uintptr_t)param_3 + 0x290);
        }
        PECMD_DispatchCreateControl(param_3, (int64_t)param_1, (WCHAR **)&local_a0, (int)(uintptr_t)local_90,
                      local_b8[0], local_res8[0], (int)(uintptr_t)local_res18,
                      (WCHAR **)&local_c0, (WCHAR **)&local_80, (((uVar9 & 0xfffe) | 4)) | uVar8,
                      &local_b0, local_c8, local_88);
        FUN_14005b104((void *)&local_80);
        FUN_14005b104((void *)&local_c8);
        FUN_14005b104((void *)&local_98);
        FUN_14005b104((void *)&local_c0);
        FUN_14005b104((void *)&local_a0);
    }
    (void)local_58; (void)local_60; (void)local_78; (void)local_a4; (void)local_a8;
LAB_140083a22:
    FUN_14005b104((void *)&local_68);
    return uVar4;
}

/* ==================================================================
 * @0x140083f74  (uint64_t PECMD_ParseMsgDispatch(longlong*, ushort*, WPARAM, uint64_t))
 * MSG 参数解析: 同族写控件分发, 通过 FUN_140063ed4。
 * ================================================================== */
uint64_t PECMD_ParseMsgDispatch(int64_t *param_1, uint16_t *param_2, WPARAM param_3, uint64_t param_4)
{
    uint16_t uVar1;
    int64_t *plVar2;
    uint64_t uVar4;
    uint16_t *puVar5;
    uint64_t uVar6;
    uint16_t *puVar7;
    uint64_t uVar8;
    uint32_t uVar9;
    int local_res8[2];
    uint16_t *local_res10;
    int local_res18[2];
    int local_88;
    int local_84;
    int64_t local_80;
    int64_t local_78;
    int64_t local_70;
    int64_t local_68;
    int64_t local_60;
    uint16_t *local_58;
    int64_t local_50;
    int64_t *local_48;
    int64_t local_40;

    local_res10 = param_2;
    FUN_140063620(&local_50);
    uVar6 = 0;
    local_40 = 0;
    local_48 = param_1;
    FUN_14005b154((WCHAR **)&local_res10);
    if (param_3 == 0) {
        FUN_14001b3a0(param_1, (int64_t *)0x0);
        param_3 = (WPARAM)param_1[8];
        if (param_3 == 0) {
            uVar6 = (uint64_t)(int64_t)(int32_t)0xffffffff80070057;
            goto LAB_1400842e0;
        }
    }
    puVar5 = local_res10;
    local_58 = local_res10;
    PECMD_SkipWCharUntil(&local_58, 0x2c);
    uVar8 = 0x2d;
    uVar4 = (uint64_t)*local_58;
    *local_58 = 0;
    puVar7 = local_58;
    while ((uVar1 = *puVar5, (uint16_t)uVar8 == uVar1)) {
        while ((uVar1 != 0 && (((local_res10 = puVar5, uVar1 < 9 || (0xd < uVar1)) &&
                                (uVar1 != 0x20))))) {
            puVar5 = puVar5 + 1;
            local_res10 = puVar5;
            uVar1 = *puVar5;
        }
        FUN_14005b154((WCHAR **)&local_res10);
        puVar5 = local_res10;
    }
    *puVar7 = (uint16_t)uVar4;
    FUN_140063620(&local_80);
    FUN_140063620(&local_78);
    FUN_140063620(&local_70);
    PECMD_StrDupAssign((WCHAR **)&local_68, g_szEmpty);
    uVar9 = 0;
    local_88 = 0;
    local_84 = 0;
    local_res18[0] = 0;
    local_res8[0] = 0;
    uVar1 = *puVar5;
    local_60 = (int64_t)param_4;
    if (uVar1 == 0x2a) {
        local_res10 = puVar5 + 1;
        FUN_14005b154((WCHAR **)&local_res10);
    }
    PECMD_SplitTokenTrimWs((int64_t *)&local_res10, &local_80, 0x2c);
    PECMD_RunCommandLine(param_1, (WCHAR **)&local_80, 1);
    if (*local_res10 == 0x2c) {
        local_res10 = local_res10 + 1;
        plVar2 = PECMD_SplitTokenAssignVar((WCHAR **)&local_50, (WCHAR **)&local_res10, 0x2c, 1);
        PECMD_ParseLtwhParams(plVar2, (uint32_t *)&local_88, &local_84, local_res18, (uint32_t *)local_res8);
        if (*local_res10 == 0x2c) {
            local_res10 = local_res10 + 1;
            plVar2 = PECMD_SplitTokenAssignVar((WCHAR **)&local_50, (WCHAR **)&local_res10, 0x2c, 1);
            PECMD_SplitTokenTrimWs(plVar2, &local_78, 0);
            if (*local_res10 == 0x2c) {
                local_res10 = local_res10 + 1;
                PECMD_CopyUpToChar((int64_t *)&local_res10, &local_68, 0x2c);
                if (*local_res10 == 0x2c) {
                    local_res10 = local_res10 + 1;
                    puVar7 = (uint16_t *)PECMD_SplitTokenAssignVar((WCHAR **)&local_50, (WCHAR **)&local_res10,
                                                      0x2c, 1);
                    uVar4 = PECMD_ParseSignedNumber((short *)(uintptr_t)*puVar7);
                    uVar9 = (uint32_t)uVar4;
                }
            }
        }
        if (uVar1 != 0x2a) {
            param_1 = *(int64_t **)((uintptr_t)param_3 + 0x290);
        }
        FUN_140063ed4(param_3, (int64_t)param_1, &local_80, local_88, local_84, local_res18[0],
                      local_res8[0], &local_78, &local_68, uVar9);
        FUN_14005b104((void *)&local_68);
        FUN_14005b104((void *)&local_70);
        FUN_14005b104((void *)&local_78);
        FUN_14005b104((void *)&local_80);
    }
    else {
        FUN_14005b104((void *)&local_68);
        FUN_14005b104((void *)&local_70);
        FUN_14005b104((void *)&local_78);
        FUN_14005b104((void *)&local_80);
        uVar6 = 1;
    }
    (void)local_40; (void)local_48; (void)local_60;
LAB_1400842e0:
    FUN_14005b104((void *)&local_50);
    return uVar6;
}

/* ==================================================================
 * @0x14008430c  (uint64_t PECMD_MsgDispatch(longlong*, ushort*, WPARAM, uint64_t))
 * MSG 参数解析: 同族写控件分发, 通过 PECMD_InsertControlObject。
 * ================================================================== */
uint64_t PECMD_MsgDispatch(int64_t *param_1, uint16_t *param_2, WPARAM param_3, uint64_t param_4)
{
    uint16_t uVar1;
    int64_t *plVar2;
    uint64_t uVar4;
    uint16_t *puVar5;
    uint64_t uVar6;
    uint16_t *puVar7;
    uint64_t uVar8;
    uint32_t uVar9;
    int local_res8[2];
    uint16_t *local_res10;
    int local_res18[2];
    int local_88;
    int local_84;
    int64_t local_80;
    int64_t local_78;
    int64_t local_70;
    int64_t local_68;
    int64_t local_60;
    uint16_t *local_58;
    int64_t local_50;
    int64_t *local_48;
    int64_t local_40;

    local_res10 = param_2;
    FUN_140063620(&local_50);
    uVar6 = 0;
    local_40 = 0;
    local_48 = param_1;
    FUN_14005b154((WCHAR **)&local_res10);
    if (param_3 == 0) {
        FUN_14001b3a0(param_1, (int64_t *)0x0);
        param_3 = (WPARAM)param_1[8];
        if (param_3 == 0) {
            uVar6 = (uint64_t)(int64_t)(int32_t)0xffffffff80070057;
            goto LAB_140084678;
        }
    }
    puVar5 = local_res10;
    local_58 = local_res10;
    PECMD_SkipWCharUntil(&local_58, 0x2c);
    uVar8 = 0x2d;
    uVar4 = (uint64_t)*local_58;
    *local_58 = 0;
    puVar7 = local_58;
    while ((uVar1 = *puVar5, (uint16_t)uVar8 == uVar1)) {
        while ((uVar1 != 0 && (((local_res10 = puVar5, uVar1 < 9 || (0xd < uVar1)) &&
                                (uVar1 != 0x20))))) {
            puVar5 = puVar5 + 1;
            local_res10 = puVar5;
            uVar1 = *puVar5;
        }
        FUN_14005b154((WCHAR **)&local_res10);
        puVar5 = local_res10;
    }
    *puVar7 = (uint16_t)uVar4;
    FUN_140063620(&local_80);
    FUN_140063620(&local_78);
    FUN_140063620(&local_70);
    PECMD_StrDupAssign((WCHAR **)&local_68, g_szEmpty);
    uVar9 = 0;
    local_88 = 0;
    local_84 = 0;
    local_res18[0] = 0;
    local_res8[0] = 0;
    uVar1 = *puVar5;
    local_60 = (int64_t)param_4;
    if (uVar1 == 0x2a) {
        local_res10 = puVar5 + 1;
        FUN_14005b154((WCHAR **)&local_res10);
    }
    PECMD_SplitTokenTrimWs((int64_t *)&local_res10, &local_80, 0x2c);
    PECMD_RunCommandLine(param_1, (WCHAR **)&local_80, 1);
    if (*local_res10 == 0x2c) {
        local_res10 = local_res10 + 1;
        plVar2 = PECMD_SplitTokenAssignVar((WCHAR **)&local_50, (WCHAR **)&local_res10, 0x2c, 1);
        PECMD_ParseLtwhParams(plVar2, (uint32_t *)&local_88, &local_84, local_res18, (uint32_t *)local_res8);
        if (*local_res10 == 0x2c) {
            local_res10 = local_res10 + 1;
            plVar2 = PECMD_SplitTokenAssignVar((WCHAR **)&local_50, (WCHAR **)&local_res10, 0x2c, 1);
            PECMD_SplitTokenTrimWs(plVar2, &local_78, 0);
            if (*local_res10 == 0x2c) {
                local_res10 = local_res10 + 1;
                PECMD_CopyUpToChar((int64_t *)&local_res10, &local_68, 0x2c);
                if (*local_res10 == 0x2c) {
                    local_res10 = local_res10 + 1;
                    puVar7 = (uint16_t *)PECMD_SplitTokenAssignVar((WCHAR **)&local_50, (WCHAR **)&local_res10,
                                                      0x2c, 1);
                    uVar4 = PECMD_ParseSignedNumber((short *)(uintptr_t)*puVar7);
                    uVar9 = (uint32_t)uVar4;
                }
            }
        }
        if (uVar1 != 0x2a) {
            param_1 = *(int64_t **)((uintptr_t)param_3 + 0x290);
        }
        PECMD_InsertControlObject(param_3, (int64_t)param_1, &local_80, local_88, local_84, local_res18[0],
                      local_res8[0], &local_78, &local_68, uVar9);
        FUN_14005b104((void *)&local_68);
        FUN_14005b104((void *)&local_70);
        FUN_14005b104((void *)&local_78);
        FUN_14005b104((void *)&local_80);
    }
    else {
        FUN_14005b104((void *)&local_68);
        FUN_14005b104((void *)&local_70);
        FUN_14005b104((void *)&local_78);
        FUN_14005b104((void *)&local_80);
        uVar6 = 1;
    }
    (void)local_40; (void)local_48; (void)local_60;
LAB_140084678:
    FUN_14005b104((void *)&local_50);
    return uVar6;
}

/* ==================================================================
 * @0x1400858c4  (uint64_t PECMD_GenerateTextContent(longlong*, longlong))
 * 文本/文件内容生成循环: 解析格式化选项, 生成多行/多块输出。
 * ================================================================== */
uint64_t PECMD_GenerateTextContent(int64_t *param_1, int64_t param_2)
{
    uint64_t uVar1;
    WCHAR *pWVar2;
    LPWSTR pWVar3;
    char cVar4;
    WCHAR WVar5;
    uint32_t uVar6;
    int iVar7;
    int iVar8;
    WCHAR **plVar9;
    uint64_t uVar10;
    uint64_t uVar11;
    short *psVar12;
    LPWSTR pWVar13;
    WCHAR *pWVar14;
    short sVar15;
    LPWSTR pWVar16;
    int64_t lVar17;
    uint64_t uVar18;
    LPWSTR pWVar19;
    WCHAR *pWVar20;
    char cVar21;
    WCHAR *pWVar22;
    WCHAR *pWVar23;
    LPWSTR pWVar24;
    LPWSTR pWVar25;
    LPWSTR pWVar27;
    uint32_t uVar26;
    bool bVar28;
    bool bVar29;
    int64_t local_res10;
    WCHAR *local_1a8;
    WCHAR *local_1a0;
    int64_t local_198;
    WCHAR *local_190;
    WCHAR *local_188;
    WCHAR *local_180;
    WCHAR *local_178;
    uint32_t local_170;
    short *local_168;
    uint64_t local_160;
    LPWSTR local_158;
    WCHAR *local_150;
    WCHAR *local_148;
    LPWSTR local_140;
    WCHAR *local_138;
    WCHAR *local_130;
    LPWSTR local_128;
    LPCWSTR local_120;
    uint32_t local_118;
    uint64_t local_110;
    int64_t local_108;
    LPWSTR local_100;
    int64_t local_f8;
    short *local_f0;
    LPWSTR local_e8;
    WCHAR *local_e0;
    LPWSTR local_d8;
    LPCWSTR local_d0;
    LPCWSTR local_c8;
    LPWSTR local_c0;
    LPCWSTR local_b8;
    uint64_t local_a8;
    uint64_t local_a0;
    uint64_t local_80;
    WCHAR *local_78;
    uint64_t local_70;
    WCHAR *local_68;
    uint64_t local_60;
    uint64_t local_98[3];
    uint64_t local_58;

    pWVar24 = (LPWSTR)0x0;
    local_a8 = (uint64_t)(int64_t)-0x80000000;
    local_a0 = 0;
    local_res10 = param_2;
    memset(local_98, 0, 0x58);
    local_100 = (LPWSTR)0x0;
    local_128 = (LPWSTR)0x0;
    uVar6 = 1;
    local_120 = (LPCWSTR)0x0;
    local_e8 = (LPWSTR)0x0;
    local_158 = (LPWSTR)0x0;
    local_198 = -1;
    local_110 = 1;
    local_160 = 0;
    local_178 = (WCHAR *)0x0;
    local_170 = 0;
    local_f8 = 0;
    local_108 = 0;
    plVar9 = (WCHAR **)FUN_14005b154((WCHAR **)&local_res10);
    local_1a8 = (WCHAR *)*plVar9;
    WVar5 = L'*';
    cVar21 = '*';
    if (*local_1a8 == L'-') {
        cVar21 = '-';
    }
    local_138 = (WCHAR *)local_1a8;
    PECMD_SkipWCharUntil(&local_138, 0x3d);
    if (*local_138 == L'\0') goto LAB_1400859c1;
    *local_138 = L'\0';
    local_140 = local_138 + 1;
    do {
        pWVar20 = local_138;
        local_138 = pWVar20 + -1;
        pWVar14 = local_1a8;
        lVar17 = -1;
        uVar26 = 0;
        if (local_138 <= local_1a8) goto LAB_140085eeb;
    } while ((((uint16_t)*local_138 < 9) || (0xd < (uint16_t)*local_138)) &&
             (*local_138 != L' '));
    *local_138 = L'\0';
    pWVar14 = pWVar20;
    uVar26 = 0;
    if (local_1a8 < pWVar20) {
        pWVar13 = pWVar24;
        do {
            uVar26 = (uint32_t)(uintptr_t)pWVar13;
            if ((int)cVar21 != (int)(uint32_t)(uint16_t)*local_1a8) break;
            local_1a8 = local_1a8 + 1;
            cVar4 = FUN_1400660ac("ni", (WCHAR **)&local_1a8, 2);
            if (cVar4 == '\0') {
                cVar4 = FUN_1400660ac("ex", (WCHAR **)&local_1a8, 2);
                if (cVar4 == '\0') {
                    cVar4 = FUN_1400660ac("l", (WCHAR **)&local_1a8, 1);
                    if (cVar4 == '\0') {
                        cVar4 = FUN_1400660ac("f", (WCHAR **)&local_1a8, 1);
                        if (cVar4 == '\0') {
                            cVar4 = FUN_1400660ac("t", (WCHAR **)&local_1a8, 1);
                            if (cVar4 == '\0') {
                                cVar4 = FUN_1400660ac("ts1", (WCHAR **)&local_1a8, 3);
                                if (cVar4 == '\0') {
                                    cVar4 = FUN_1400660ac("ts", (WCHAR **)&local_1a8, 2);
                                    if (cVar4 == '\0') {
                                        if ((WCHAR)(*local_1a8 | 0x20U) == 0x78) {
                                            local_1a8 = local_1a8 + 1;
                                            if (9 < (uint16_t)(*local_1a8 + 0x1f10)) {
                                                pWVar23 = local_1a8;
                                                if (((*local_1a8 == L'\0') ||
                                                     ((8 < (uint16_t)*local_1a8 &&
                                                       ((uint16_t)*local_1a8 < 0xe)))) ||
                                                    (*local_1a8 == L' ')) {
                                                    local_160 = 2;
                                                }
                                                goto LAB_140085e5d;
                                            }
                                            PECMD_ParseHexOrDec((WCHAR **)&local_1a8, &local_160);
                                        }
                                        else {
                                            pWVar23 = local_1a8;
                                            uVar10 = (uint64_t)PECMD_AsciiPrefixICmp("[",
                                                                             (const WCHAR *)local_1a8,
                                                                             1);
                                            if ((char)uVar10 == '\0') {
                                                cVar4 = FUN_1400660ac("many", (WCHAR **)&local_1a8,
                                                                     4);
                                                if (cVar4 == '\0') {
                                                    if ((WCHAR)(*local_1a8 | 0x20U) == 0x68) {
                                                        pWVar23 = local_1a8 + 1;
                                                        pWVar19 = pWVar24;
                                                        if (*pWVar23 == L'~') {
                                                            pWVar23 = local_1a8 + 2;
                                                            pWVar19 = (LPWSTR)0x8000000000000000ULL;
                                                        }
                                                        if (*pWVar23 == L'?') {
                                                            local_1a8 = pWVar23 + 1;
                                                            local_78 = (WCHAR *)PECMD_ParseFirstWordToken(param_1,
                                                                        (int64_t *)&local_1a8,
                                                                        (uint64_t *)&local_f8);
                                                            iVar7 = lstrlenW(local_78);
                                                            local_70 = (uint64_t)iVar7 |
                                                                       (uint64_t)(uintptr_t)pWVar19;
                                                            goto LAB_140085eac;
                                                        }
                                                    }
                                                    else {
                                                        pWVar23 = local_1a8;
                                                        if ((WCHAR)(*local_1a8 | 0x20U) == 0x65) {
                                                            pWVar23 = local_1a8 + 1;
                                                            pWVar19 = pWVar24;
                                                            if (*pWVar23 == L'~') {
                                                                pWVar23 = local_1a8 + 2;
                                                                pWVar19 = (LPWSTR)0x8000000000000000ULL;
                                                            }
                                                            if (*pWVar23 == L'?') {
                                                                local_1a8 = pWVar23 + 1;
                                                                local_68 = (WCHAR *)PECMD_ParseFirstWordToken(param_1,
                                                                            (int64_t *)&local_1a8,
                                                                            (uint64_t *)&local_108);
                                                                iVar7 = lstrlenW(local_68);
                                                                local_60 = (uint64_t)iVar7 |
                                                                           (uint64_t)(uintptr_t)pWVar19;
                                                                goto LAB_140085eac;
                                                            }
                                                        }
                                                    }
LAB_140085e5d:
                                                    local_1a8 = pWVar23 + -1;
                                                    WVar5 = *local_1a8;
                                                    if (WVar5 != L'\0') {
                                                        do {
                                                            if (((8 < (uint16_t)WVar5) &&
                                                                 ((uint16_t)WVar5 < 0xe)) ||
                                                                (WVar5 == L' ')) {
                                                                break;
                                                            }
                                                            local_1a8 = local_1a8 + 1;
                                                            WVar5 = *local_1a8;
                                                        } while (WVar5 != L'\0');
                                                        pWVar13 = (LPWSTR)(uintptr_t)local_170;
                                                    }
                                                    FUN_14005b154((WCHAR **)&local_1a8);
                                                }
                                                else {
                                                    uVar6 = uVar6 | 4;
                                                }
                                            }
                                            else {
                                                pWVar22 = pWVar23 + 1;
                                                WVar5 = *pWVar22;
                                                local_1a8 = pWVar22;
                                                if (WVar5 != L'\0') {
                                                    do {
                                                        if (((8 < (uint16_t)WVar5) &&
                                                             ((uint16_t)WVar5 < 0xe)) ||
                                                            (WVar5 == L' ')) {
                                                            break;
                                                        }
                                                        local_1a8 = local_1a8 + 1;
                                                        WVar5 = *local_1a8;
                                                    } while (WVar5 != L'\0');
                                                    pWVar13 = (LPWSTR)(uintptr_t)local_170;
                                                }
                                                FUN_14005b154((WCHAR **)&local_1a8);
                                                local_1a8[-1] = L'\0';
                                                PECMD_ExpandCommandLine(param_1, pWVar22, (WCHAR **)&local_178,
                                                              0, 1);
                                                local_1a0 = local_178;
                                                FUN_1400a9a84((WCHAR **)&local_1a0,
                                                              (uint64_t *)&local_128);
                                                FUN_1400a9a84((WCHAR **)&local_1a0,
                                                              (uint64_t *)&local_120);
                                                pWVar19 = local_128;
                                                local_100 = local_128;
                                                local_158 = local_128;
                                                FUN_1400745c8((WCHAR **)&local_1a0,
                                                              (int64_t *)&local_158);
                                                FUN_1400745c8((WCHAR **)&local_1a0, (int64_t *)&local_110);
                                                if ((int64_t)local_110 < 1) {
                                                    local_110 = 1;
                                                }
                                                local_198 = (int64_t)(uintptr_t)local_120 +
                                                           (1 - (int64_t)(uintptr_t)pWVar19);
                                            }
                                        }
                                    }
                                    else {
                                        local_98[0] = 0x73;
                                    }
                                }
                                else {
                                    local_98[0] = 0x31;
                                }
                            }
                            else {
                                local_98[0] = 0x74;
                            }
                        }
                        else {
                            uVar6 = uVar6 | 0x20;
                        }
                    }
                    else {
                        uVar6 = uVar6 | 0x10;
                    }
                }
                else {
                    local_170 = uVar26 + 1;
                    pWVar13 = (LPWSTR)(uintptr_t)local_170;
                }
            }
            else {
                uVar6 = uVar6 & 0xfffffffe;
            }
LAB_140085eac:
                uVar26 = (uint32_t)(uintptr_t)pWVar13;
            } while (local_1a8 < pWVar20);
            local_e8 = local_158;
            WVar5 = L'*';
            lVar17 = local_198;
        }
LAB_140085eeb:
        pWVar13 = local_140;
        if (WVar5 == *pWVar14) {
            pWVar14 = pWVar14 + 1;
            uVar6 = uVar6 | 4;
        }
        FUN_1400703e4((int64_t *)&local_178, pWVar14);
        pWVar20 = local_178;
        local_1a8 = (WCHAR *)pWVar13;
        if (lVar17 < 1) {
            PECMD_RunCommandLine(param_1, (WCHAR **)&local_178, 0);
        }
        else {
            local_178 = (WCHAR *)0x0;
            local_140 = pWVar20;
            PECMD_ExpandCommandLine(param_1, pWVar20, (WCHAR **)&local_178, 0, 1);
            FUN_14005b104((void *)&local_140);
        }
        local_e0 = local_178;
        local_80 = (uint64_t)(int)uVar6;
        local_58 = local_160;
        plVar9 = (WCHAR **)FUN_14005b154((WCHAR **)&local_1a8);
        pWVar20 = (WCHAR *)*plVar9;
        sVar15 = 0x2c;
        local_1a8 = (WCHAR *)pWVar20;
        local_190 = pWVar20;
        local_188 = pWVar20;
        local_180 = pWVar20;
        PECMD_SkipWCharUntil(&local_180, 0x2c);
        if (*local_180 != L'\0') {
            *local_180 = L'\0';
            local_180 = local_180 + 1;
        }
        local_190 = local_180;
        PECMD_SkipWCharUntil(&local_190, sVar15);
        if (*local_190 != L'\0') {
            *local_190 = L'\0';
            local_190 = local_190 + 1;
        }
        local_188 = local_190;
        PECMD_SkipWCharUntil(&local_188, sVar15);
        if (*local_188 != L'\0') {
            *local_188 = L'\0';
            local_188 = local_188 + 1;
        }
        FUN_140063620(&local_f0);
        FUN_140063620(&local_148);
        FUN_140063620(&local_130);
        FUN_140063620(&local_150);
        FUN_14007bf44(param_1, pWVar20, (WCHAR **)&local_f0, 0, 1);
        if (*local_f0 == 0x3f) {
            local_f0 = local_f0 + 1;
            local_98[0] = 0x3f;
        }
        bVar28 = *local_f0 == 0x2d;
        local_168 = local_f0;
        if (bVar28) {
            local_168 = local_f0 + 1;
        }
        uVar11 = FUN_1400745c8((WCHAR **)&local_168, (int64_t *)&local_a8);
        if (((int)uVar11 != 0) && ((int64_t)local_a8 < 0)) {
            bVar28 = !bVar28;
            local_a8 = (uint64_t)(-((int64_t)local_a8));
        }
        if (*local_168 == 0x3a) {
            local_168 = local_168 + 1;
        }
        bVar29 = *local_168 == 0x2d;
        if (bVar29) {
            local_168 = local_168 + 1;
        }
        uVar11 = FUN_1400745c8((WCHAR **)&local_168, (int64_t *)&local_a0);
        if (((int)uVar11 != 0) && ((int64_t)local_a0 < 0)) {
            bVar29 = !bVar29;
            local_a0 = (uint64_t)(-((int64_t)local_a0));
        }
        if ((bVar28) || (bVar29)) {
            local_a0 = (uint64_t)(-((int64_t)local_a0) - 1);
        }
        bVar28 = false;
        bVar29 = false;
        psVar12 = local_168;
        if (*local_168 == 0x3a) {
            psVar12 = local_168 + 1;
        }
        if (*psVar12 == 0) {
LAB_1400861bf:
            PECMD_ExpandCommandLine(param_1, local_180, (WCHAR **)&local_148, 0, 1);
            local_180 = local_148;
        }
        else {
            bVar28 = false;
            if (((psVar12[1] != 0) && (bVar28 = psVar12[1] == 0x2a, psVar12[2] != 0)) &&
                (psVar12[2] == 0x2a)) {
                bVar29 = true;
            }
            if (*psVar12 != 0x2a) goto LAB_1400861bf;
        }
        if (!bVar28) {
            PECMD_ExpandCommandLine(param_1, local_190, (WCHAR **)&local_130, 0, 1);
            local_190 = local_130;
        }
        if (!bVar29) {
            PECMD_ExpandCommandLine(param_1, local_188, (WCHAR **)&local_150, 0, 1);
            local_188 = local_150;
        }
        iVar7 = lstrlenW(local_180);
        PECMD_AllocWStringBuffer(&local_128, (int64_t)(iVar7 + 0x11));
        pWVar13 = local_128;
        memcpy((void *)local_128, (const void *)local_180, (iVar7 + 1) * 2);
        memset((uint64_t *)(pWVar13 + (iVar7 + 1)), 0, 0x20);
        local_180 = pWVar13;
        iVar7 = lstrlenW(local_190);
        PECMD_AllocWStringBuffer((WCHAR **)&local_120, (int64_t)(iVar7 + 0x11));
        pWVar23 = (WCHAR *)local_120;
        memcpy((void *)local_120, (const void *)local_190, (iVar7 + 1) * 2);
        memset((uint64_t *)(pWVar23 + (iVar7 + 1)), 0, 0x20);
        local_1a8 = (WCHAR *)pWVar23;
        local_190 = (WCHAR *)pWVar23;
        pWVar14 = pWVar23;
        if (local_98[0] != 0x73) {
            WVar5 = *pWVar23;
            while (WVar5 != L'\0') {
                if (*local_1a8 == L'\\') {
                    WVar5 = local_1a8[1];
                    if (WVar5 == L'r') {
                        *pWVar14 = L'\r';
                    }
                    else if (WVar5 == L'n') {
                        *pWVar14 = L'\n';
                    }
                    else {
                        if (WVar5 != L't') {
                            *pWVar14 = L'\\';
                            pWVar14 = pWVar14 + 1;
                            local_1a8 = local_1a8 + 1;
                            *pWVar14 = *local_1a8;
                            goto LAB_14008638a;
                        }
                        *pWVar14 = L'\t';
                    }
                    local_1a8 = local_1a8 + 2;
                }
                else {
                    *pWVar14 = *local_1a8;
LAB_14008638a:
                    local_1a8 = local_1a8 + 1;
                }
                pWVar14 = pWVar14 + 1;
                uVar26 = local_170;
                WVar5 = *local_1a8;
            }
        }
        if ((0 < (int)uVar26) || ((int64_t)local_160 < 1)) {
            for (; pWVar14 < local_1a8; pWVar14 = pWVar14 + 1) {
                *pWVar14 = L'\0';
            }
            local_d0 = (LPCWSTR)0x0;
            local_140 = (LPWSTR)0x0;
            local_158 = (LPWSTR)0x0;
            local_d8 = (LPWSTR)0x0;
            local_c0 = (LPWSTR)0x0;
            local_c8 = (LPCWSTR)0x0;
            local_b8 = (LPCWSTR)0x0;
            local_118 = 0;
            local_168 = (short *)0;
            pWVar13 = StrChrW(pWVar23, L';');
            pWVar20 = local_e0;
            uVar6 = -(uint32_t)(pWVar13 != (LPWSTR)0x0) & 2;
            local_80 = local_80 | (uint64_t)(int)uVar6;
            if (local_a8 == (uint64_t)(int64_t)-0x80000000) {
                local_a8 = (uint64_t)(uVar6 == 0);
            }
            pWVar16 = pWVar24;
            pWVar19 = pWVar24;
            pWVar25 = pWVar24;
            pWVar27 = pWVar24;
            pWVar13 = pWVar24;
            if (local_198 >= 1) {
                iVar7 = lstrlenW(local_e0);
                iVar8 = lstrlenW(local_188);
                lVar17 = (int64_t)iVar8 + 1;
                iVar8 = lstrlenW(local_180);
                uVar6 = (uint32_t)iVar8 + 1;
                pWVar25 = (LPWSTR)(uintptr_t)uVar6;
                local_118 = uVar6;
                iVar8 = lstrlenW(local_190);
                uVar26 = (uint32_t)iVar8 + 1;
                pWVar27 = (LPWSTR)(uintptr_t)uVar26;
                local_168 = (short *)(uintptr_t)uVar26;
                PECMD_AllocString(&local_158, (int64_t)iVar7 + 0xc81 + lVar17 * 2 +
                                          (int64_t)(int)(uVar26 + uVar6));
                pWVar16 = local_158;
                local_c8 = local_180;
                local_d0 = pWVar20;
                local_b8 = local_190;
                pWVar13 = local_158 + (int)(uVar26 + uVar6);
                local_140 = pWVar13;
                memcpy((void *)pWVar13, (const void *)local_188, (int)lVar17 * 2);
                pWVar13 = pWVar13 + lVar17;
                pWVar19 = pWVar13 + (int64_t)iVar7 + 0x641;
                local_d8 = pWVar13;
                local_c0 = pWVar19;
                do {
                    local_198 = local_198 + -1;
                    local_180 = pWVar16;
                    memcpy((void *)pWVar16, (const void *)local_c8, (int)(uintptr_t)pWVar25 * 2);
                    local_190 = pWVar16 + (int)(uintptr_t)pWVar25;
                    memcpy((void *)local_190, (const void *)local_b8, (int)(uintptr_t)pWVar27 * 2);
                    local_e0 = pWVar13;
                    FUN_14005eefc(pWVar13, (WCHAR *)local_d0);
                    pWVar3 = local_e8;
                    local_188 = pWVar19;
                    FUN_14005eefc(pWVar19, local_140);
                    local_100 = (LPWSTR)((uintptr_t)local_100 + local_110);
                    local_e8 = (LPWSTR)((uintptr_t)pWVar3 + local_110);
                    uVar6 = local_170;
                    if ((2 < (int)local_170) && (*local_190 != L'\0')) {
                        local_1a0 = (WCHAR *)0x0;
                        local_190 = PECMD_ResolveVariable(param_1, local_190, (uint64_t *)&local_1a0, 0);
                        pWVar20 = local_130;
                        local_130 = local_1a0;
                        local_1a0 = pWVar20;
                        FUN_14005b104((void *)&local_1a0);
                    }
                    if ((1 < (int)uVar6) && (*local_180 != L'\0')) {
                        local_1a0 = (WCHAR *)0x0;
                        local_180 = PECMD_ResolveVariable(param_1, local_180, (uint64_t *)&local_1a0, 0);
                        pWVar20 = local_148;
                        local_148 = local_1a0;
                        local_1a0 = pWVar20;
                        FUN_14005b104((void *)&local_1a0);
                    }
                    if ((uVar6 != 0) && (*local_188 != L'\0')) {
                        local_1a0 = (WCHAR *)0x0;
                        if ((int64_t)local_160 < 1) {
                            local_188 = PECMD_ResolveVariable(param_1, local_188, (uint64_t *)&local_1a0, 0);
                            pWVar20 = local_150;
                            local_150 = local_1a0;
                            local_1a0 = pWVar20;
                        }
                        else {
                            lVar17 = PECMD_VarLookup(param_1, local_188, (void *)0x0, -1, (void *)0x0);
                            if (lVar17 == 0) {
                                *local_188 = L'\0';
                            }
                            else {
                                uVar11 = *(uint64_t *)(lVar17 + 0x18) & 0x3fffffffffffffff;
                                if (1 < (int64_t)local_160) {
                                    uVar11 = *(uint64_t *)(lVar17 + 0x18) & 0x3ffffffffffffffe;
                                }
                                uVar18 = *(uint64_t *)(lVar17 + 8);
                                uVar1 = uVar18 + uVar11;
                                PECMD_AllocString(&local_1a0, (int64_t)uVar11 / (int64_t)local_160 +
                                                           0x11 + uVar11 * 2);
                                pWVar2 = local_1a0;
                                *local_1a0 = L'\0';
                                pWVar25 = pWVar24;
                                pWVar20 = local_1a0;
                                pWVar22 = local_150;
                                pWVar19 = local_c0;
                                for (; local_c0 = pWVar19, uVar18 < uVar1; uVar18 = uVar18 + 1) {
                                    if ((uintptr_t)pWVar25 % (uintptr_t)local_160 == 0) {
                                        *pWVar20 = L' ';
                                        pWVar20 = pWVar20 + 1;
                                    }
                                    local_150 = pWVar22;
                                    wsprintfW(pWVar20, WSTR("%02X"));
                                    pWVar20 = pWVar20 + 2;
                                    pWVar25 = (LPWSTR)((uintptr_t)pWVar25 + 1);
                                    pWVar16 = local_158;
                                    pWVar13 = local_d8;
                                    pWVar22 = local_150;
                                    pWVar19 = local_c0;
                                }
                                pWVar25 = (LPWSTR)(uintptr_t)local_118;
                                pWVar27 = (LPWSTR)(uintptr_t)local_168;
                                *pWVar20 = L'\0';
                                local_188 = pWVar2;
                                local_150 = pWVar2;
                                local_1a0 = pWVar22;
                            }
                        }
                        FUN_14005b104((void *)&local_1a0);
                    }
                    pWVar14 = (WCHAR *)PECMD_RegexReplace((int64_t *)&local_a8, local_180, local_190, local_188,
                                            local_e0, param_1);
                } while (0 < local_198);
            }
            FUN_14005b104((void *)&local_158);
            FUN_14005b104((void *)&local_120);
            FUN_14005b104((void *)&local_128);
            FUN_14005b104((void *)&local_150);
            FUN_14005b104((void *)&local_130);
            FUN_14005b104((void *)&local_148);
            FUN_14005b104((void *)&local_f0);
            FUN_14005b104((void *)&local_108);
            FUN_14005b104((void *)&local_f8);
            FUN_14005b104((void *)&local_178);
            return (uint64_t)(pWVar14 == (LPCWSTR)0x0);
        }
        FUN_14005b104((void *)&local_120);
        FUN_14005b104((void *)&local_128);
        FUN_14005b104((void *)&local_150);
        FUN_14005b104((void *)&local_130);
        FUN_14005b104((void *)&local_148);
        FUN_14005b104((void *)&local_f0);
    (void)local_58; (void)local_60; (void)local_70;
LAB_1400859c1:
        FUN_14005b104((void *)&local_108);
        FUN_14005b104((void *)&local_f8);
        FUN_14005b104((void *)&local_178);
        return (uint64_t)(int64_t)(int32_t)0xffffffff80070057;
}

/* ==================================================================
 * ==================================================================
 * @0x140086944  (uint64_t PECMD_ProcessEncodedFile(longlong*, WCHAR*))
 * 加解密/文件流主入口: 解析开关 (-f/-m/-udm/-o/-u/-bin/-src/-utf8),
 * 打开输入输出流, 处理后写回变量。
 * ================================================================== */
uint64_t PECMD_ProcessEncodedFile(int64_t *param_1, WCHAR *param_2)
{
    uint32_t uVar16;
    uint32_t uVar17;
    uint32_t uVar19;
    uint8_t bVar24;
    bool bVar29;
    int iVar22;
    DWORD DVar7;
    int64_t lVar11;
    int64_t lVar21;
    int64_t lVar27;
    int64_t _Var10;
    WCHAR WVar4;
    uint16_t uVar15;
    char cVar28;
    char cVar2;
    LPCWSTR pWVar9;
    LPCWSTR lpString2;
    HANDLE pvVar12;
    uint64_t uVar13;
    uint64_t uVar23;
    HANDLE *ppvVar14;
    uint8_t *pbVar20;
    uint8_t *pbVar18;
    WCHAR *pWVar26;
    short *psVar1;
    WCHAR *local_res10;
    uint8_t local_res20[8];
    uint8_t abStack[28358];
    DWORD local_d5a8;
    char local_d5a4;
    uint8_t local_d5a2;
    char local_d5a3;
    uint32_t local_d5a0;
    LPCWSTR local_d598;
    LPCWSTR local_d590;
    LPCWSTR local_d588;
    int64_t local_d580;
    LPCWSTR local_d578;
    int64_t *local_d570;
    HANDLE local_d568;
    int64_t local_d560[2];
    int64_t local_d550;
    uint32_t local_d540;
    int64_t *local_d538;
    HANDLE local_d530[4];
    uint8_t local_d510;
    uint32_t local_d508;
    int local_d504;
    int64_t *local_d500;
    HANDLE local_d4f8[5];
    uint32_t local_d4d0;
    int local_d4cc;
    WCHAR *local_d4c8;
    int64_t *local_d4c0;
    HANDLE local_d4b8[4];
    uint8_t local_d498;
    uint32_t local_d490;
    uint32_t local_d488[1022];
    uint8_t local_c48e[35];
    char local_c46b;
    int64_t *local_50;
    int64_t *local_48;
    uint64_t uStack_40;

    (void)abStack; (void)uStack_40; (void)uVar17;
    (void)local_48; (void)local_50; (void)local_d490; (void)local_d498;
    (void)local_d508; (void)local_d510; (void)local_d540; (void)local_d550;
    local_res10 = param_2;
    FUN_14005b154((WCHAR **)&local_res10);
    iVar22 = 0;
    if (*local_res10 == L'\0') {
        return 0;
    }
    PECMD_StrDupAssign((WCHAR **)&local_d598, g_szEmpty);
    PECMD_StrDupAssign((WCHAR **)&local_d590, g_szEmpty);
    local_d5a4 = '\0';
    local_res20[0] = 0;
    local_d5a2 = '\0';
    local_d5a8 = 0xff000000;
    uVar23 = 1;
    lVar27 = 4;
    WVar4 = *local_res10;
    cVar28 = ' ';
    while (local_d4c8 = local_res10, WVar4 != L'\0') {
        cVar2 = FUN_1400660ac("-f", (WCHAR **)&local_res10, 2);
        if (cVar2 == '\0') {
            cVar2 = FUN_1400660ac("-m", (WCHAR **)&local_res10, 2);
            if (cVar2 == '\0') {
                cVar2 = FUN_1400660ac("-udm", (WCHAR **)&local_res10, 4);
                if (cVar2 == '\0') {
                    char cVar3 = FUN_1400660ac("-o", (WCHAR **)&local_res10, 2);
                    cVar2 = '\x01';
                    if (cVar3 == '\0') {
                        cVar2 = FUN_1400660ac("-u", (WCHAR **)&local_res10, 2);
                        if (cVar2 == '\0') {
                            pWVar26 = local_res10;
                            uVar23 = PECMD_AsciiPrefixICmp("-u:", (const WCHAR *)local_res10, 3);
                            if ((char)uVar23 == '\0') {
                                cVar2 = FUN_1400660ac("-bin", (WCHAR **)&local_res10, 4);
                                if (cVar2 == '\0') {
                                    pWVar26 = local_res10;
                                    uVar23 = PECMD_AsciiPrefixICmp("-src", (const WCHAR *)local_res10, 4);
                                    if ((char)uVar23 == '\0') {
                                        cVar2 = FUN_1400660ac("-utf8", (WCHAR **)&local_res10, 5);
                                        if (cVar2 == '\0') {
                                            cVar2 = FUN_1400660ac("--", (WCHAR **)&local_res10, 2);
                                            local_d4c8 = local_res10;
                                            if (((cVar2 == '\0') &&
                                                (lVar11 = FUN_14005c72c("-", (const WCHAR *)local_res10, 1),
                                                 (char)lVar11 == '\0')) &&
                                                (WVar4 = *local_d4c8, WVar4 == L'-')) {
                                                do {
                                                    if (((8 < (uint16_t)WVar4) &&
                                                         ((uint16_t)WVar4 < 0xe)) ||
                                                        (WVar4 == L' ')) {
                                                        break;
                                                    }
                                                    local_d4c8 = local_d4c8 + 1;
                                                    WVar4 = *local_d4c8;
                                                    local_res10 = local_d4c8;
                                                } while (WVar4 != L'\0');
                                                goto LAB_140086c8f;
                                            }
                                            break;
                                        }
                                        local_d5a2 = '\x01';
                                        cVar2 = cVar28;
                                    }
                                    else {
                                        local_res10 = pWVar26 + 4;
                                        if ((((uint16_t)*local_res10 < 9) ||
                                             (0xd < (uint16_t)*local_res10)) &&
                                            (*local_res10 != L' ')) {
                                            cVar2 = cVar28;
                                            if (*local_res10 == L':') {
                                                local_res10 = pWVar26 + 5;
                                                local_d5a8 = 0;
                                                PECMD_ParseUIntValue((LPCWSTR *)&local_res10, (int *)&local_d5a8);
                                            }
                                        }
                                        else {
                                            local_d5a8 = 5;
                                            cVar2 = cVar28;
                                        }
                                    }
                                }
                                else {
                                    local_res20[0] = 1;
                                    local_res10 = local_res10 + 2;
LAB_140086c8f:
                                    FUN_14005b154((WCHAR **)&local_res10);
                                    cVar2 = cVar28;
                                }
                            }
                            else {
                                local_res10 = pWVar26 + 3;
                                cVar28 = '\x10';
                                FUN_14005b154((WCHAR **)&local_res10);
                                local_d4c8 = local_res10;
                                if (*local_res10 == L'\0') break;
                                *local_res10 = L'\0';
                                do {
                                    local_res10 = local_res10 + 1;
                                    WVar4 = *local_res10;
                                    local_d4c8 = local_res10;
                                    if (WVar4 == L'\0') goto LAB_140086cae;
                                    cVar2 = cVar28;
                                } while ((((uint16_t)WVar4 < 9) || (0xd < (uint16_t)WVar4)) &&
                                         (WVar4 != L' '));
                            }
                        }
                        else {
                            cVar2 = '\x10';
                        }
                    }
                }
                else {
                    cVar2 = '@';
                }
            }
            else {
                cVar2 = '\x02';
            }
        }
        else {
            local_d5a4 = '\x01';
            cVar2 = cVar28;
        }
        cVar28 = cVar2;
        WVar4 = *local_res10;
    }
LAB_140086cae:
    if (*local_d4c8 == L'\"') {
        FUN_1400545f8(param_1, (WCHAR **)&local_res10, (WCHAR **)&local_d598, L',', 0);
        FUN_1400545f8(param_1, (WCHAR **)&local_res10, (WCHAR **)&local_d590, L',', 0);
        pWVar9 = local_d590;
        lpString2 = local_d598;
    }
    else {
        PECMD_SplitTokenTrimWs((int64_t *)&local_res10, (int64_t *)&local_d598, 0x2c);
        if (*local_res10 == L',') {
            local_res10 = local_res10 + 1;
            PECMD_SplitTokenTrimWs((int64_t *)&local_res10, (int64_t *)&local_d590, 0x2c);
            pWVar9 = FUN_14001be14((WCHAR *)local_d590);
            lpString2 = local_d598;
        }
        else {
            local_d580 = 0;
            local_d588 = (LPCWSTR)PECMD_NextToken((int64_t *)&local_d4c8, &local_d580, 1);
            PECMD_StrCopyW((WCHAR **)&local_d598, local_d4c8, local_d580);
            PECMD_NextToken((int64_t *)&local_d588, &local_d580, 0);
            PECMD_StrCopyW((WCHAR **)&local_d590, (LPCWSTR)local_d588, local_d580);
            pWVar9 = local_d590;
            lpString2 = local_d598;
        }
    }
    local_d578 = pWVar9;
    if ((*pWVar9 == L'\0') || (*lpString2 == L'\0')) {
        FUN_14005b104((void *)&local_d590);
        FUN_14005b104((void *)&local_d598);
        return (uint64_t)(int64_t)(int32_t)0xffffffff80070057;
    }
    iVar22 = lstrcmpW(WSTR("-"), lpString2);
    local_d5a3 = iVar22 == 0;
    do {
        uVar16 = 0;
        uVar17 = local_d5a0;
        if (cVar28 != '\x02') {
            do {
                do {
                    local_d5a0 = uVar17;
                    _Var10 = _time64((int64_t *)0x0);
                    DVar7 = GetTickCount();
                    lVar11 = PECMD_NextRandomSeed();
                    uVar16 = (uint32_t)(g_randSeedAccum * 0x10 + (int)_Var10 + DVar7 + (int)lVar11);
                    g_randSeedAccum = g_randSeedAccum ^ uVar16;
                    uVar17 = uVar16;
                } while (((uint8_t)uVar16 & 3) != 3);
            } while ((((uVar16 & 0xf000) == 0) || ((uVar16 & 0xf00) == 0)) ||
                     (((uVar16 & 0xf00000) == 0 ||
                       ((((uVar16 & 0xf0000) == 0 ||
                          (((uint8_t *)&local_d5a0)[3] = (uint8_t)(uVar16 >> 24),
                           (uVar16 & 0xf000000) == 0)) ||
                         ((((uint8_t *)&local_d5a0)[3] & 0x88) != 0x80))))));
        }
        if (cVar28 == '\x10') {
            local_d500 = (int64_t *)&PTR_FUN_140126560;
            local_d5a0 = 0;
            PECMD_InitStreamObject((int64_t *)local_d4f8, 0);
            local_d4d0 = 0;
            local_d4c0 = (int64_t *)&PTR_FUN_140126560;
            PECMD_InitStreamObject((int64_t *)local_d4b8, 0);
            local_d490 = 0;
            PECMD_OpenFileHandle(local_d4f8, lpString2, 1, 7, (LPSECURITY_ATTRIBUTES)0x0, 3, 0x80,
                          (HANDLE)0x0);
            lVar11 = 0;
            if (local_d4f8[0] != (HANDLE)0x0) {
                lVar21 = 8;
                do {
                    local_d4cc = local_d4cc + 1;
                    PECMD_StreamReadByte((int64_t *)local_d4f8, local_res20, 1, &local_d5a8);
                    lVar21 = lVar21 + -1;
                } while (lVar21 != 0);
                do {
                    local_d4cc = local_d4cc + 1;
                    local_res20[0] = 0;
                    local_d5a8 = 0;
                    bVar24 = PECMD_StreamReadByte((int64_t *)local_d4f8, local_res20, 1, &local_d5a8);
                    if ((!bVar24) || (local_d5a8 == 0)) {
                        bVar24 = 0xff;
                    }
                    ((uint8_t *)&local_d5a0)[lVar11] = bVar24;
                    lVar11 = lVar11 + 1;
                } while (lVar11 < 4);
                if ((local_d5a0 == 0) &&
                    (PECMD_OpenFileHandle(local_d4b8, pWVar9, 0x40000001, 7, (LPSECURITY_ATTRIBUTES)0x0,
                                   2, 0x80, (HANDLE)0x0),
                     local_d4b8[0] != (HANDLE)0x0)) {
                    local_d498 = 1;
                    PECMD_ZeroStruct_bc98(local_d488);
                    local_48 = (int64_t *)&local_d4c0;
                    local_50 = (int64_t *)&local_d500;
                    if (local_d4d0 != 0) {
                        local_d4d0 = local_d4d0 ^ 0x5aa59669;
                    }
                    do {
                        local_d4cc = local_d4cc + 1;
                        pbVar20 = local_res20;
                        PECMD_StreamReadByte((int64_t *)local_d4f8, pbVar20, 1, &local_d5a8);
                        lVar27 = lVar27 + -1;
                    } while (lVar27 != 0);
                    PECMD_LzDecompress((int64_t)local_d488, (uint64_t)pbVar20);
                    FlushFileBuffers(local_d4b8[0]);
                    local_d4c0 = (int64_t *)&PTR_FUN_140126560;
                    PECMD_StreamClose((int64_t *)local_d4b8);
                    local_d500 = (int64_t *)&PTR_FUN_140126560;
                    ppvVar14 = local_d4f8;
                    goto LAB_140087639;
                }
            }
            local_d4c0 = (int64_t *)&PTR_FUN_140126560;
            PECMD_StreamClose((int64_t *)local_d4b8);
            local_d500 = (int64_t *)&PTR_FUN_140126560;
            ppvVar14 = local_d4f8;
            goto LAB_14008755e;
        }
        uVar19 = uVar16 & 0xfffffff;
        uVar17 = 0;
        if (uVar16 != 0) {
            uVar17 = uVar16 ^ 0x5aa59669;
        }
        uVar15 = 0;
        if (cVar28 == ' ') {
            uVar15 = 0x14;
        }
        if (cVar28 == '@') {
            uVar15 = 0x18;
        }
        uVar15 = (uint16_t)uVar17 ^ uVar15;
        local_d5a0 = ((uint32_t)(uint16_t)(uVar17 >> 0x10) << 16) | (uint16_t)uVar15;
    } while ((uVar15 != 0) && (uVar15 == 0));
    local_d570 = (int64_t *)&PTR_FUN_140126540;
    PECMD_InitStreamObject((int64_t *)&local_d568, 0);
    local_d540 = 0;
    local_d538 = (int64_t *)&PTR_FUN_140126540;
    PECMD_InitStreamObject((int64_t *)local_d530, 0);
    bVar29 = *lpString2 != L'&';
    lVar27 = 7;
    local_d508 = uVar19;
    if (bVar29) {
        if (local_d5a3 == '\0') {
            PECMD_OpenFileHandle(&local_d568, lpString2, 1, 7, (LPSECURITY_ATTRIBUTES)0x0, 3, 0x80,
                          (HANDLE)0x0);
            if (g_u8CCB1 != '\0') {
                pvVar12 = GetStdHandle(0xfffffff6);
                FUN_140018d8c(&g_Script, WSTR("infile=0x%p 0x%p bstin=%d\r\n"),
                              (uint64_t)local_d568, (uint64_t)pvVar12);
            }
            iVar22 = 0;
            if (local_d568 == (HANDLE)0x0) goto LAB_140087144;
        }
        else {
            local_d568 = g_hStdIn;
        }
LAB_14008702a:
        PECMD_OpenFileHandle(local_d530, local_d578, 0x40000001, 7, (LPSECURITY_ATTRIBUTES)0x0, 2, 0x80,
                      (HANDLE)0x0);
        if (local_d530[0] == (HANDLE)0x0) goto LAB_140087144;
        local_d510 = 1;
        PECMD_ZeroStruct_bc98(local_d488);
        local_48 = (int64_t *)&local_d538;
        local_50 = (int64_t *)&local_d570;
        if (cVar28 == ' ') {
            bVar24 = 0x61;
        }
        else {
            bVar24 = 0x53;
            if (cVar28 == '@') {
                bVar24 = 0x4d;
            }
        }
        FUN_140063620(&local_d588);
        uVar16 = local_d5a8;
        if ((((int)local_d5a8 < 0) && (bVar24 == 0x53)) || (local_res20[0] != 0)) {
LAB_1400871d8:
            pbVar20 = g_b24d00;
            do {
                local_res20[0] = *pbVar20;
                local_d504 = local_d504 + 1;
                PECMD_BufferedWriteByte((int64_t *)local_d530, local_res20);
                pbVar20 = pbVar20 + 1;
                lVar27 = lVar27 + -1;
            } while (lVar27 != 0);
            local_d504 = local_d504 + 1;
            local_res20[0] = bVar24;
            PECMD_BufferedWriteByte((int64_t *)local_d530, local_res20);
            lVar11 = 4;
            pbVar20 = (uint8_t *)&local_d5a0;
            local_d504 = 8;
            lVar27 = lVar11;
            do {
                local_res20[0] = *pbVar20;
                local_d504 = local_d504 + 1;
                PECMD_BufferedWriteByte((int64_t *)local_d530, local_res20);
                pbVar20 = pbVar20 + 1;
                lVar27 = lVar27 + -1;
            } while (lVar27 != 0);
            local_d504 = 0xc;
            local_c48e[0] = 0;
            local_c48e[1] = 0;
            local_c48e[2] = 0;
            local_c48e[3] = 0;
            {
                short sVar25 = 0;
                do {
                    local_res20[0] = 0;
                    local_d5a8 = 0;
                    bVar24 = PECMD_StreamReadByte((int64_t *)&local_d568, local_res20, 1, &local_d5a8);
                    GetLastError();
                    if ((!bVar24) || (local_d5a8 == 0)) {
                        uVar16 = 0xffffffff;
                    }
                    else {
                        uVar16 = (uint32_t)local_res20[0];
                    }
                    if (uVar16 == 0xffffffff) break;
                    lVar27 = (int64_t)sVar25;
                    sVar25 = sVar25 + 1;
                    local_c48e[lVar27] = (uint8_t)uVar16;
                    local_c46b = local_c46b + '\x01';
                } while (sVar25 < 4);
            }
            cVar28 = local_d5a4;
            pbVar20 = local_c48e;
            do {
                if (cVar28 == '\0') {
                    local_res20[0] = 0;
                }
                else {
                    local_res20[0] = *pbVar20;
                }
                local_d504 = local_d504 + 1;
                pbVar18 = local_res20;
                PECMD_BufferedWriteByte((int64_t *)local_d530, pbVar18);
                pbVar20 = pbVar20 + 1;
                lVar11 = lVar11 + -1;
            } while (lVar11 != 0);
            local_d504 = 0x10;
            SetLastError(0);
            PECMD_DigestUpdate(local_d488, (uint64_t)pbVar18);
            FlushFileBuffers(local_d530[0]);
            if (local_d5a3 != '\0') {
                local_d568 = (HANDLE)0x0;
            }
            FUN_14005b104((void *)&local_d588);
            local_d538 = (int64_t *)&PTR_FUN_140126540;
            PECMD_StreamClose((int64_t *)local_d530);
            local_d570 = (int64_t *)&PTR_FUN_140126540;
            ppvVar14 = &local_d568;
LAB_140087639:
            PECMD_StreamClose((int64_t *)ppvVar14);
            FUN_14005b104((void *)&local_d590);
            FUN_14005b104((void *)&local_d598);
            return 0;
        }
        if (bVar29) {
            uVar13 = PECMD_ReadFileToWideString(local_d568, (int64_t *)local_d560, (uint32_t)local_d5a2);
            iVar22 = (int)uVar13;
            if ((local_d568 != (HANDLE)0x0) &&
                (local_d568 != (HANDLE)0xffffffffffffffff)) {
                CloseHandle(local_d568);
            }
            local_d568 = (HANDLE)0x0;
        }
        if (0 < iVar22) {
            if ((((bVar29) && (1 < iVar22)) && ((uint16_t)((short *)local_d560[0])[1] < 0x80)) &&
                ((cVar28 == ' ' || (cVar28 == '@')))) {
                memmove((void *)local_d560[0], (void *)(local_d560[0] + 1), iVar22 * 2);
                iVar22 = iVar22 + -1;
            }
            if ((int)uVar16 < 0) {
                uVar16 = 5;
            }
            if (bVar29) {
                uVar13 = PECMD_CleanScriptText((short *)local_d560[0], iVar22, uVar16);
                iVar22 = (int)uVar13;
            }
            local_d550 = (int64_t)iVar22 * 2;
            goto LAB_1400871d8;
        }
        FUN_14005b104((void *)&local_d588);
    }
    else {
        lVar11 = PECMD_VarLookup(param_1, lpString2, (void *)0x0, -1, (void *)0x0);
        if (lVar11 != 0) {
            iVar22 = (int)(*(uint64_t *)(lVar11 + 0x18) >> 1);
            PECMD_AllocString((WCHAR **)local_d560, (int64_t)(iVar22 + 0xc));
            psVar1 = (short *)local_d560[0];
            memcpy((void *)local_d560[0], *(const void **)(lVar11 + 8), iVar22 * 2);
            memset((uint64_t *)((int64_t)(iVar22 * 2) + (int64_t)psVar1), 0, 10);
            local_d550 = (int64_t)iVar22 * 2;
            goto LAB_14008702a;
        }
    }
    uVar23 = (uint64_t)(int64_t)(int32_t)0xffffffff80070057;
LAB_140087144:
    local_d538 = (int64_t *)&PTR_FUN_140126540;
    PECMD_StreamClose((int64_t *)local_d530);
    local_d570 = (int64_t *)&PTR_FUN_140126540;
    ppvVar14 = &local_d568;
LAB_14008755e:
    PECMD_StreamClose((int64_t *)ppvVar14);
    FUN_14005b104((void *)&local_d590);
    FUN_14005b104((void *)&local_d598);
    return uVar23;
}
/* ==================================================================
 * @0x1400944c4  (LPCWSTR PECMD_GetDiskFreeSpace(longlong*, WCHAR*))
 * 磁盘/卷信息查询命令: 解析 -userfree/-userall/-free/-raw/-x/-clear 等,
 * 获取磁盘空闲/容量/卷信息或格式化输出, 受全局临界区保护。
 * ================================================================== */
LPCWSTR PECMD_GetDiskFreeSpace(int64_t *param_1, WCHAR *param_2)
{
    WCHAR WVar1;
    bool bVar2;
    bool bVar3;
    char cVar4;
    int iVar5;
    BOOL BVar6;
    UINT UVar7;
    WCHAR *pWVar8;
    uint64_t uVar9;
    LPCWSTR pWVar10;
    char cVar11;
    LPCWSTR pWVar12;
    int iVar13;
    WCHAR *pWVar14;
    LPWSTR local_res10;
    LPWSTR local_130;
    LPWSTR local_128;
    LPWSTR local_120;
    LPWSTR local_118;
    ULARGE_INTEGER local_110[3];
    WCHAR local_f8[32];
    uint32_t local_b8;
    uint16_t local_b4;
    uint64_t local_b2[15];

    local_res10 = param_2;
    EnterCriticalSection((LPCRITICAL_SECTION)&g_csInit);
    FUN_14005b154((WCHAR **)&local_res10);
    PECMD_AllocWStringBuffer((WCHAR **)&local_128, 5);
    PECMD_AllocWStringBuffer((WCHAR **)&local_130, 0x14);
    FUN_140063620(&local_120);
    pWVar10 = (LPCWSTR)0x0;
    bVar3 = false;
    cVar11 = -1;
    bVar2 = false;
    pWVar12 = (LPCWSTR)0x1;
    WVar1 = *local_res10;
    while (WVar1 == L'-') {
        cVar4 = FUN_1400660ac("-userfree", (WCHAR **)&local_res10, 9);
        if (cVar4 == '\0') {
            cVar4 = FUN_1400660ac("-userall", (WCHAR **)&local_res10, 8);
            if (cVar4 == '\0') {
                cVar4 = FUN_1400660ac("-free", (WCHAR **)&local_res10, 5);
                if (cVar4 == '\0') {
                    cVar4 = FUN_1400660ac("-raw", (WCHAR **)&local_res10, 4);
                    if (cVar4 == '\0') {
                        cVar4 = FUN_1400660ac("-x", (WCHAR **)&local_res10, 2);
                        if (cVar4 == '\0') {
                            cVar4 = FUN_1400660ac("-clear", (WCHAR **)&local_res10, 6);
                            if (cVar4 != '\0') {
                                memset((void *)&g_aiDiskType, 0, 8);
                                pWVar12 = (LPCWSTR)0x0;
                                goto LAB_140094bba;
                            }
                            WVar1 = *local_res10;
                            while ((WVar1 != L'\0' &&
                                    ((((uint16_t)WVar1 < 9 || (0xd < (uint16_t)WVar1)) &&
                                      (WVar1 != L' '))))) {
                                local_res10 = local_res10 + 1;
                                WVar1 = *local_res10;
                            }
                            FUN_14005b154((WCHAR **)&local_res10);
                        }
                        else {
                            bVar2 = true;
                        }
                    }
                    else {
                        bVar3 = true;
                    }
                }
                else {
                    cVar11 = '\x02';
                }
            }
            else {
                cVar11 = '\x01';
            }
        }
        else {
            cVar11 = '\0';
        }
        WVar1 = *local_res10;
    }
    PECMD_SplitTokenTrimWs((int64_t *)&local_res10, (int64_t *)&local_130, 0x3d);
    PECMD_RunCommandLine(param_1, (WCHAR **)&local_130, 0);
    if (*local_res10 == L'=') {
        local_res10 = local_res10 + 1;
        PECMD_SplitTokenTrimWs((int64_t *)&local_res10, (int64_t *)&local_128, 0x2c);
    }
    local_res10 = local_128;
    local_128 = (LPWSTR)0x0;
    pWVar8 = FUN_14001be14((WCHAR *)local_res10);
    PECMD_AssignString((WCHAR **)&local_128, pWVar8);
    FUN_14005b104((void *)&local_res10);
    for (pWVar8 = local_130; *pWVar8 != L'\0'; pWVar8 = pWVar8 + 1) {
        if (*pWVar8 == L',') {
            *pWVar8 = L'\0';
            local_res10 = pWVar8 + 1;
            PECMD_SplitTokenTrimWs((int64_t *)&local_res10, (int64_t *)&local_120, 0x2c);
            break;
        }
    }
    PECMD_RunCommandLine(param_1, (WCHAR **)&local_120, 0);
    PECMD_StripTrailingSpaces(local_130);
    if (((*local_130 == L'\0') && (*local_120 == L'\0')) || (*local_128 == L'\0')) goto LAB_140094bba;
    pWVar8 = local_128;
    uVar9 = (uint64_t)PECMD_AsciiWideICmp("*", (const WCHAR *)local_128);
    pWVar12 = pWVar10;
    if ((char)uVar9 != '\0') {
        PECMD_AllocString((WCHAR **)&local_120, 0xca);
        *local_120 = L'\0';
        PECMD_ListDrives((int64_t *)&local_128, L'|', 1, 0, 0, (LPCWSTR)0x0);
        PECMD_SetVariable(param_1, local_130, local_128);
        goto LAB_140094bba;
    }
    iVar5 = lstrlenW(pWVar8);
    PECMD_AllocString((WCHAR **)&local_128, (int64_t)(iVar5 + 0x19));
    pWVar8 = local_128;
    if (cVar11 < '\0') {
        if ((0x60 < (uint16_t)*local_128) && ((uint16_t)*local_128 < 0x7b)) {
            *local_128 = *local_128 & 0xffdf;
        }
        PECMD_StrDupAssign((WCHAR **)&local_118, WSTR("\\\\.\\C:***"));
        local_118[4] = *pWVar8;
        local_118[7] = L'\0';
        iVar5 = -1;
        UVar7 = 0xffffffff;
        local_118[6] = L'\0';
        iVar13 = 0x800;
        uVar9 = PECMD_AsciiPrefixICmp("\\\\.\\", (const WCHAR *)pWVar8, 4);
        pWVar14 = pWVar8;
        if (((char)uVar9 == '\0') &&
            (uVar9 = PECMD_AsciiPrefixICmp("\\Device\\", (const WCHAR *)pWVar8, 8), (char)uVar9 == '\0')) {
            pWVar8[1] = L':';
            pWVar8[2] = L'\\';
            pWVar8[3] = L'\0';
            if (bVar2) {
                pWVar14 = local_118;
                if (((0x40 < (uint16_t)*pWVar8) && ((uint16_t)*pWVar8 < 0x43)) &&
                    (UVar7 = GetDriveTypeW(pWVar8), UVar7 == 2)) {
                    iVar13 = (uint16_t)*pWVar8 - 0x41;
                }
                goto LAB_1400949c9;
            }
            if (*local_130 != L'\0') {
                UVar7 = GetDriveTypeW(pWVar8);
            }
            if ((*local_120 != L'\0') || ((!bVar3 && (((UVar7 == 2 || (UVar7 == 3)) ||
                                                       (UVar7 == 5)))))) {
                iVar5 = PECMD_GetDiskGeometry(local_118, (HANDLE)0x0);
            }
            iVar13 = -1;
            if (!bVar3) {
                iVar13 = iVar5;
            }
            pWVar10 = PECMD_DriveTypeName(UVar7, local_f8, iVar13);
        }
        else {
LAB_1400949c9:
            local_res10 = (LPWSTR)0x0;
            if (((1 < iVar13) || (*(int *)((intptr_t)&g_aiDiskType + (int64_t)iVar13 * 4) == 0)) &&
                (*local_130 != L'\0')) {
                PECMD_OpenFileHandle((HANDLE *)&local_res10, pWVar14, 0x80000000, 3,
                              (LPSECURITY_ATTRIBUTES)0x0, 3, 0x20000000, (HANDLE)0x0);
                pWVar10 = local_res10;
                PECMD_QueryDiskGeometry((HANDLE)local_res10, (uint64_t *)0x0, 1, iVar13);
            }
            if (*local_120 != L'\0') {
                if (pWVar10 == (LPCWSTR)0x0) {
                    PECMD_OpenFileHandle((HANDLE *)&local_res10, pWVar14, 0x80000000, 3,
                                  (LPSECURITY_ATTRIBUTES)0x0, 3, 0x20000000, (HANDLE)0x0);
                    pWVar10 = local_res10;
                }
                PECMD_GetDiskGeometry((LPCWSTR)0x0, (HANDLE)pWVar10);
            }
            if ((pWVar10 != (LPCWSTR)0x0) && (pWVar10 != (LPCWSTR)0xffffffffffffffff)) {
                CloseHandle((HANDLE)pWVar10);
            }
            pWVar10 = local_f8;
            wsprintfW(local_f8, WSTR("%d"));
        }
        if (*local_130 != L'\0') {
            PECMD_SetVariable(param_1, local_130, pWVar10);
        }
        if (*local_120 != L'\0') {
            wsprintfW(local_f8, WSTR("%d"));
            PECMD_SetVariable(param_1, local_120, local_f8);
        }
        if (*pWVar10 == L'\0') {
            FUN_14005b104((void *)&local_118);
            pWVar12 = (LPCWSTR)0x1;
        }
        else {
            FUN_14005b104((void *)&local_118);
        }
        goto LAB_140094bba;
    }
    if (iVar5 == 1) {
        if (*local_128 != L'.') {
            local_128[1] = L':';
            local_128[2] = L'\\';
            local_128[3] = L'\0';
            goto LAB_140094864;
        }
    }
    else {
LAB_140094864:
        if ((iVar5 == 2) && (local_128[1] == L':')) {
            local_128[2] = L'\\';
            local_128[3] = L'\0';
        }
    }
    local_b8 = g_u322570;
    local_b4 = g_u162574;
    (void)local_b4;
    memset(local_b2, 0, 0x76);
    local_110[0].QuadPart = 0;
    local_110[1].QuadPart = 0;
    local_110[2].QuadPart = 0;
    BVar6 = GetDiskFreeSpaceExW(pWVar8, local_110, local_110 + 1, local_110 + 2);
    if (BVar6 != 0) {
        FUN_1400e6d74((LPCWSTR)&local_b8, local_110[(int)cVar11].QuadPart);
    }
    PECMD_SetVariable(param_1, local_130, (LPCWSTR)&local_b8);
    pWVar12 = (LPCWSTR)(uintptr_t)(BVar6 == 0);
LAB_140094bba:
    FUN_14005b104((void *)&local_120);
    FUN_14005b104((void *)&local_130);
    FUN_14005b104((void *)&local_128);
    LeaveCriticalSection((LPCRITICAL_SECTION)&g_csInit);
    return pWVar12;
}

/* ==================================================================
 * @0x140094c04  (uint64_t PECMD_NormalizePath(longlong*, LPCWSTR))
 * 路径规范化命令: 解析 --fullfile/--full/--short/--short+/--long/--long+
 * 与 --volume/--volumepart/--part, 获取完整/短路径或卷挂载点。
 * ================================================================== */
uint64_t PECMD_NormalizePath(int64_t *param_1, LPCWSTR param_2)
{
    WCHAR WVar1;
    bool bVar2;
    bool bVar3;
    bool bVar4;
    bool bVar5;
    bool bVar6;
    bool bVar7;
    LPWSTR lpString;
    char cVar8;
    int iVar9;
    BOOL BVar10;
    uint64_t uVar11;
    LPWSTR pWVar12;
    LPWSTR pWVar13;
    LPWSTR lpszLongPath;
    char cVar14;
    LPWSTR local_res10[2];
    LPWSTR local_res20;
    WCHAR local_68[4];
    LPWSTR local_60;
    LPWSTR local_58;
    LPWSTR local_50;
    int64_t local_48[2];

    local_res10[0] = (LPWSTR)param_2;
    FUN_14005b154((WCHAR **)local_res10);
    PECMD_AllocWStringBuffer((WCHAR **)&local_50, 5);
    PECMD_AllocWStringBuffer((WCHAR **)local_48, 0x14);
    cVar14 = '\x01';
    bVar3 = false;
    bVar2 = false;
    bVar6 = false;
    bVar4 = false;
    bVar7 = false;
    bVar5 = false;
    while (*local_res10[0] == L'-') {
        cVar8 = FUN_1400660ac("--fullfile", (WCHAR **)local_res10, -1);
        if (cVar8 == '\0') {
            cVar8 = FUN_1400660ac("--full", (WCHAR **)local_res10, -1);
            if (cVar8 == '\0') {
                cVar8 = FUN_1400660ac("--short", (WCHAR **)local_res10, -1);
                if (cVar8 == '\0') {
                    cVar8 = FUN_1400660ac("--short+", (WCHAR **)local_res10, -1);
                    if (cVar8 == '\0') {
                        cVar8 = FUN_1400660ac("--long", (WCHAR **)local_res10, -1);
                        if (cVar8 == '\0') {
                            cVar8 = FUN_1400660ac("--long+", (WCHAR **)local_res10, -1);
                            if (cVar8 == '\0') {
                                uVar11 = PECMD_MatchPrefixN((uint16_t *)WSTR("--volume"),
                                                       (WCHAR **)local_res10, 8);
                                if ((int)uVar11 == 0) {
                                    uVar11 = PECMD_MatchPrefixN((uint16_t *)WSTR("--volumepart"),
                                                           (WCHAR **)local_res10, 0xc);
                                    if ((int)uVar11 == 0) {
                                        uVar11 = PECMD_MatchPrefixN((uint16_t *)WSTR("--part"),
                                                               (WCHAR **)local_res10, 6);
                                        if ((int)uVar11 == 0) {
                                            uVar11 = PECMD_MatchPrefixN(g_u16127d30,
                                                                   (WCHAR **)local_res10, 3);
                                            if ((int)uVar11 == 0) {
                                                for (; (WVar1 = *local_res10[0], WVar1 != L'\0' &&
                                                       ((((uint16_t)WVar1 < 9 ||
                                                          (0xd < (uint16_t)WVar1)) &&
                                                         (WVar1 != L' '))));
                                                     local_res10[0] = local_res10[0] + 1) {
                                                }
                                                FUN_14005b154((WCHAR **)local_res10);
                                            }
                                            else {
                                                bVar5 = true;
                                            }
                                        }
                                        else {
                                            bVar4 = true;
                                            bVar7 = false;
                                        }
                                    }
                                    else {
                                        bVar4 = true;
                                        bVar7 = true;
                                    }
                                }
                                else {
                                    bVar4 = false;
                                    bVar7 = true;
                                }
                            }
                            else {
                                bVar3 = true;
                                bVar2 = true;
                                bVar6 = false;
                            }
                        }
                        else {
                            bVar3 = false;
                            bVar2 = true;
                            bVar6 = false;
                        }
                    }
                    else {
                        bVar3 = true;
                        bVar2 = false;
                        bVar6 = true;
                    }
                }
                else {
                    bVar3 = false;
                    bVar2 = false;
                    bVar6 = true;
                }
            }
            else {
                cVar14 = '\x01';
            }
        }
        else {
            cVar14 = '\x02';
        }
    }
    local_res10[0] = FUN_1400547bc(param_1, (int64_t *)local_res10, (int64_t *)&local_50, 0x3d, 0);
    if (*local_res10[0] == L'=') {
        local_res10[0] = local_res10[0] + 1;
        FUN_1400545f8(param_1, (WCHAR **)local_res10, (WCHAR **)local_48, L'\0', 0);
    }
    pWVar12 = (LPWSTR)PECMD_StripDevicePrefix(local_48[0]);
    if ((*local_50 == L'\0') || (*pWVar12 == L'\0')) {
        FUN_14005b104((void *)local_48);
        FUN_14005b104((void *)&local_50);
        return (uint64_t)(int64_t)(int32_t)0xffffffff80070057;
    }
    iVar9 = lstrlenW(pWVar12);
    PECMD_StrDupAssign((WCHAR **)&local_58, pWVar12);
    PECMD_AllocString((WCHAR **)&local_58, (int64_t)(iVar9 * 8 + 0x105));
    lpszLongPath = local_58;
    local_res20 = (LPWSTR)0x0;
    PECMD_ExpandDrivePath(pWVar12, iVar9 * 8 + 0x104, local_58, &local_res20);
    if (local_res20 != (LPWSTR)0x0) {
        local_res20 = local_res20 + -1;
    }
    if (bVar6) {
        PECMD_AllocWStringBuffer((WCHAR **)&local_60, 0x2000);
        pWVar12 = local_60;
        *local_60 = L'\0';
        GetShortPathNameW(lpszLongPath, (LPWSTR)local_60, 0x1fff);
        while (true) {
            if ((bVar3) && (*pWVar12 == L'\0')) goto LAB_140094f98;
            if (!bVar2) break;
            GetLongPathNameW(lpszLongPath, (LPWSTR)pWVar12, 0x1fff);
            bVar2 = false;
        }
        local_58 = (LPWSTR)pWVar12;
        pWVar12[0x1ffe] = L'\0';
        local_60 = lpszLongPath;
        lpszLongPath = (LPWSTR)pWVar12;
        if (cVar14 == '\x01') {
            local_res20 = StrRChrW(pWVar12, (LPCWSTR)0x0, L'\\');
        }
LAB_140094f98:
        FUN_14005b104((void *)&local_60);
    }
    if ((cVar14 == '\x01') && (local_res20 != (LPWSTR)0x0)) {
        *local_res20 = L'\0';
    }
    pWVar12 = lpszLongPath;
    if (((!bVar7) || (*lpszLongPath == L'\0')) || (lpszLongPath[1] != L':')) goto LAB_1400950ce;
    PECMD_AllocWStringBuffer((WCHAR **)&local_60, 0x1000);
    lpString = local_60;
    *local_60 = L'\0';
    local_68[0] = *lpszLongPath;
    local_68[1] = 0x3a;
    local_68[3] = 0;
    local_68[2] = 0x5c;
    *lpszLongPath = L'\0';
    BVar10 = GetVolumeNameForVolumeMountPointW(local_68, (LPWSTR)local_60, 0x1000);
    if ((BVar10 == 0) || (pWVar13 = lpString, *lpString == L'\0')) {
        if (bVar4) {
            *lpString = local_68[0];
            lpString[1] = L'\0';
            PECMD_DescribePartitionInfo((int64_t *)&local_60, (LPCWSTR)0xffffffffffffffff, 0xffffffffffffffff,
                          0xac, (LARGE_INTEGER *)0x0, (uint32_t *)0x0, '\0');
            pWVar13 = local_60;
            if ((-1 < *(int64_t *)local_60) && (0 < *(int64_t *)(local_60 + 4))) {
                wsprintfW((LPWSTR)lpString, WSTR("\\\\?\\Harddisk%uPartition%u\\"));
                goto LAB_14009509b;
            }
        }
    }
    else {
LAB_14009509b:
        iVar9 = lstrlenW(lpString);
        lstrcpyW((LPWSTR)(lpString + iVar9), lpszLongPath + 3);
        pWVar12 = pWVar13;
        local_60 = lpszLongPath;
        local_58 = (LPWSTR)pWVar13;
    }
    FUN_14005b104((void *)&local_60);
LAB_1400950ce:
    local_60 = (LPWSTR)0x0;
    local_res10[0] = pWVar12;
    if ((bVar5) && (*pWVar12 != L'\"')) {
        PECMD_QuoteTokenString((int64_t *)local_res10, (int64_t *)&local_60, -1);
    }
    PECMD_SetVariable(param_1, local_50, local_res10[0]);
    FUN_14005b104((void *)&local_60);
    FUN_14005b104((void *)&local_58);
    FUN_14005b104((void *)local_48);
    FUN_14005b104((void *)&local_50);
    return 0;
}
