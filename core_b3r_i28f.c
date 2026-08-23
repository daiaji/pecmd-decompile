/* Restored batch28 B3 business functions (part i28f)
 *
 * Contains:
 *   PECMD_ParseControlDef @0x1400bdf6c  对话框/窗口控件构造解析执行
 *   PECMD_ImageCommand @0x1400be64c  图标/图像窗口参数解析执行 (size/center/right/num*)
 *   PECMD_AttachControlImage @0x1400bf068  把图像/图标挂到控件上 (bmp 更新)
 */
#include <stdbool.h>

#include "win32_stub.h"
#include "pecmd_defs.h"

/* ---- Ghidra primitive aliases (project convention) ---- */
typedef unsigned int        uint;
typedef unsigned short      ushort;
typedef uint8_t             byte;
typedef uint64_t            undefined8;
typedef int64_t             longlong;
typedef uint64_t            ulonglong;

/* ---- Globals ---- */
extern WCHAR g_szEmpty[];   /* empty string (g_szEmpty) */
extern int (*g_pGdipGetImageWidth)();     /* GdipGetImageWidth  (function ptr) */
extern int (*g_pGdipGetImageHeight)();     /* GdipGetImageHeight (function ptr) */

/* ---- string / var helpers ---- */
extern void      PECMD_AllocStrSlot(void *out);                  /* init/release slot */
extern WCHAR    *PECMD_SkipLeadingControlChars(WCHAR **pp);                 /* skip spaces */
extern void      PECMD_StrDupAssign(void *ps, const WCHAR *src); /* assign string slot */
extern void PECMD_FreeStrBuf(void *ps);                   /* free string slot */
extern void      PECMD_SplitTokenTrimWs(void *src, void *dst, int16_t delim); /* split list */
extern void      PECMD_RunCommandLine(void *script, void *str, int mode);   /* expand */
extern int64_t  *PECMD_SplitTokenAssignVar(WCHAR **out, WCHAR **pp, uint32_t sep, int flag);
extern void      PECMD_ParseLtwhParams(int64_t *a, uint32_t *b, int *c, int *d, uint32_t *e);
extern void      PECMD_CopyUpToChar(int64_t *pp, int64_t *out, uint32_t sep);
extern void      PECMD_ParseShortStore(void *pp, int *out, WCHAR sep);
extern WCHAR    *PECMD_SkipWCharUntil(WCHAR **pp, uint16_t ch);    /* delimiter scan */
extern WCHAR    *PECMD_AppendWideStr(WCHAR **ps, const WCHAR *src); /* string append */
extern int64_t  *PECMD_WideToAnsiStr(int64_t *ps, LPCWSTR src, int64_t len, uint64_t cap);
extern int64_t   FUN_14005c72c(const char *a, const WCHAR *w, int n);
extern int64_t   PECMD_AsciiPrefixICmp(const char *a, const WCHAR *w, int n);
extern char      PECMD_MatchTokenAdvance(const char *tok, void *pp, int n);
extern bool      PECMD_ParseHexOrDec(WCHAR **pp, uint64_t *size); /* parse numeric/expr */
extern void      PECMD_ParseHexOrDecBool(LPWSTR *pp, int *out);        /* parse integer */
extern uint64_t  PECMD_EvalParenStripped(int64_t *pp, uint64_t *out);  /* parse integer */
extern uint64_t  PECMD_DispatchControlConstruct(WPARAM, uint64_t, int64_t, LPCWSTR, int, int, int, int,
                               uint64_t *, int64_t *, LPCWSTR, uint64_t, double,
                               uint64_t, uint64_t);          /* 构造派发 */

/* ---- object / window helpers ---- */
extern void      PECMD_ResetScriptChain(int64_t *script, int64_t *a2); /* 默认参数/上下文 */
extern void      FUN_1400bd620(WPARAM, LPCWSTR, void *, int, int, int, int, void *,
                               void *, int *, uint32_t, uint32_t); /* 构造派发(size 版) */
extern void      PECMD_ClipboardCommand(WCHAR *);                       /* 直接挂图像 */
extern void     *PECMD_EncodeStringId(LPCWSTR, void *, char);         /* 装载图像 */
extern void     *PECMD_DecodeBase64ToWideStr(byte *, ulonglong *);           /* 装载图像(串) */
extern void      PECMD_PushStringToken(HMODULE, LPCWSTR, LPWSTR, LONG_PTR); /* 资源名回调 */
extern char      PECMD_LoadImageListApi(void);                          /* 图/列表初始化检查 */

/* ---- image / var helpers ---- */
extern HICON     PECMD_LoadIcon(LPCWSTR, uint64_t *);           /* 加载图标 */
extern LPCWSTR   PECMD_UnquoteString(LPCWSTR);                       /* 串标签/前缀查询 */
extern void      PECMD_SetVariable(int64_t *script, LPCWSTR key, LPCWSTR value); /* SetVar */
extern int64_t   PECMD_RunCommand(int64_t *script, LPCWSTR cmd);  /* 执行脚本行 */

/* ================================================================
 * @0x1400bdf6c  对话框/窗口控件构造解析执行
 * signature: undefined8 __fastcall PECMD_ParseControlDef(longlong * param_1,
 *   LPCWSTR param_2, WPARAM param_3)
 *
 * 解析 "<提示串>,x,y,w,h,dx,dy,.." 及 "-font:"/"-color:" 前缀选项,
 * 归一化寄存器拼接 (CONCAT/局部寄存器), 调用 PECMD_DispatchControlConstruct 完成构造。
 */
uint64_t PECMD_ParseControlDef(int64_t *param_1, LPCWSTR param_2, WPARAM param_3)
{
    WCHAR   WVar1;
    WCHAR   WVar2;
    WCHAR   WVar14;
    WCHAR  *pWVar13;
    LPCWSTR pWVar3;
    LPCWSTR pWVar4;
    WCHAR  *pWVar5;
    WCHAR  *pWVar16;
    char    cVar6;
    int     iVar7;
    int64_t *plVar8;
    uint64_t uVar9;
    uint64_t uVar10;
    uint64_t uVar11;
    uint32_t uVar15;
    uint64_t uVar12;
    uint8_t  local_res22;
    uint16_t local_res20;
    WPARAM   local_res18;

    WCHAR  *local_res10;
    WCHAR  *local_b8;
    uint64_t local_b0;
    int     local_a8[2];
    WCHAR   local_a0[4];
    int     local_98;
    int     local_94;
    int     local_90[2];
    void   *local_88;
    void   *local_80;
    void   *local_78;
    WCHAR  *local_70;
    void   *local_68;
    WCHAR  *local_60;
    int64_t *local_50;
    uint64_t local_48;
    int64_t  local_58;

    /* skip leading blanks / control chars */
    for (; (WVar1 = *param_2, WVar1 != L'\0' &&
           ((((ushort)WVar1 < 9 || (0xd < (ushort)WVar1)) && (WVar1 != L' '))));
         param_2 = param_2 + 1) {
    }
    local_res10 = (WCHAR *)param_2;
    local_res18 = param_3;
    PECMD_SkipLeadingControlChars(&local_res10);
    cVar6 = PECMD_LoadImageListApi();
    if (cVar6 < '\0') {
        return 0xffffffff80004001ULL;
    }
    PECMD_AllocStrSlot(&local_58);
    local_48 = 0;
    (void)local_48;
    local_70 = (WCHAR *)g_szEmpty;
    local_50 = param_1;
    (void)local_50;
    PECMD_StrDupAssign(local_a0, g_szEmpty);
    PECMD_AllocStrSlot(&local_80);
    PECMD_AllocStrSlot(&local_78);
    PECMD_AllocStrSlot(&local_68);
    PECMD_AllocStrSlot(&local_88);
    uVar12 = 1;
    plVar8 = PECMD_SplitTokenAssignVar((WCHAR **)&local_58, &local_res10, 0x2c, 1);
    PECMD_SplitTokenTrimWs(plVar8, &local_88, 0);
    pWVar3 = local_res10;
    local_res10 = (WCHAR *)local_88;
    local_res22 = 9;
    (void)local_res22;
    local_a8[0] = 0;
    local_98 = 0;
    local_94 = 0;
    local_90[0] = 0;
    local_b0 = 0;
    local_60 = (WCHAR *)g_szEmpty;
    local_res20 = 0;
    WVar1 = *(WCHAR *)local_88;
    if (WVar1 == L'*') {
        local_res10 = (WCHAR *)local_88 + 1;
        PECMD_SkipLeadingControlChars(&local_res10);
    }
    pWVar4 = local_res10;
    iVar7 = lstrlenW(local_res10);
    /* trailing-blank trim */
    local_b8 = (WCHAR *)pWVar4 + iVar7;
    while (local_b8 > local_res10) {
        WCHAR *cand = local_b8 - 1;
        if (((8 < (ushort)*cand) && ((ushort)*cand < 0xe)) || (*cand == L' ')) {
            local_b8 = cand;
        } else {
            break;
        }
    }
    PECMD_SkipLeadingControlChars(&local_b8);
    pWVar5 = local_b8;
    WVar2 = *local_b8;
    *local_b8 = L'\0';
    uVar10 = 0x10000027;
    WVar14 = *local_res10;
    pWVar13 = local_res10;

    for (;;) {
        if ((WVar14 != L'-') || (param_3 = local_res18, pWVar5 <= pWVar13)) {
            /* ---- main path ---- */
            *pWVar5 = WVar2;
            WVar2 = *pWVar13;
            local_res10 = pWVar13;
            if (WVar2 == L'*') {
                local_res10 = pWVar13 + 1;
                PECMD_SkipLeadingControlChars(&local_res10);
            }
            PECMD_SplitTokenTrimWs(&local_res10, local_a0, 0x2c);
            PECMD_RunCommandLine(param_1, local_a0, 1);
            local_res10 = (WCHAR *)pWVar3;
            if (*pWVar3 == L',') {
                local_res10 = (WCHAR *)pWVar3 + 1;
                plVar8 = PECMD_SplitTokenAssignVar((WCHAR **)&local_58, &local_res10, 0x2c, 1);
                PECMD_ParseLtwhParams(plVar8, (uint32_t *)local_a8, &local_98, &local_94,
                              (uint32_t *)local_90);
                if (*local_res10 == L',') {
                    local_res10 = local_res10 + 1;
                    plVar8 = PECMD_SplitTokenAssignVar((WCHAR **)&local_58, &local_res10, 0x2c, 1);
                    PECMD_SplitTokenTrimWs(plVar8, &local_80, 0);
                    uVar11 = local_b0;
                    if (*local_res10 == L',') {
                        local_res10 = local_res10 + 1;
                        plVar8 = PECMD_SplitTokenAssignVar((WCHAR **)&local_58, &local_res10, 0x2c, 1);
                        PECMD_SplitTokenTrimWs(plVar8, &local_78, 0);
                        uVar11 = local_b0;
                        if (*local_res10 == L',') {
                            local_res10 = local_res10 + 1;
                            PECMD_SkipLeadingControlChars(&local_res10);
                            uVar11 = local_b0;
                            if (*local_res10 == L'-') {
                                local_res10 = local_res10 + 1;
                                uVar11 = 0x8000000;
                            }
                            WVar14 = *local_res10;
                            if (WVar14 == L'+') {
                                local_res10 = local_res10 + 1;
                            }
                            if (((0x2f < (ushort)*local_res10) && ((ushort)*local_res10 < 0x3a)) ||
                                (*local_res10 == L'(')) {
                                local_b0 = 0x10000027;
                                if (WVar14 != L'+') {
                                    uVar10 = 0;
                                }
                                PECMD_ParseHexOrDec(&local_res10, &local_b0);
                                uVar10 = uVar10 | local_b0;
                            }
                        }
                    }
                    uVar12 = 0;
                    if ((WVar2 != L'*') && (WVar1 != L'*')) {
                        param_1 = *(int64_t **)((char *)(uintptr_t)param_3 + 0x290);
                    }
                    PECMD_DispatchControlConstruct(param_3, (uint64_t)param_1, (int64_t)param_3,
                                  (LPCWSTR)local_a0, local_a8[0], local_98, local_94,
                                  local_90[0], (uint64_t *)&local_80, (int64_t *)&local_78,
                                  local_70, (uint64_t)local_60,
                                  (double)(uVar10 | uVar11), (uint64_t)&local_res20, 0);
                }
            }
            PECMD_FreeStrBuf(&local_88);
            PECMD_FreeStrBuf(&local_68);
            PECMD_FreeStrBuf(&local_78);
            PECMD_FreeStrBuf(&local_80);
            PECMD_FreeStrBuf((void *)local_a0);
            PECMD_FreeStrBuf(&local_58);
            return uVar12;
        }

        /* ---- "-font:" / "-color:" option path ---- */
        local_res10 = pWVar13;
        uVar9 = (uint64_t)PECMD_AsciiPrefixICmp("-font:", (const WCHAR *)pWVar13, 6);
        if ((char)uVar9 == '\0') {
            uVar9 = (uint64_t)PECMD_AsciiPrefixICmp("-color:", (const WCHAR *)pWVar13, 7);
            if ((char)uVar9 != '\0') {
                /* -color: */
                pWVar13 = pWVar13 + 7;
                local_60 = pWVar13;
            }
        LAB_1400be241:
            WVar14 = L'\0';
            pWVar16 = pWVar13;
            if (*pWVar13 != L'"') goto LAB_1400be26d;
            local_res10 = pWVar13 + 1;
            PECMD_SkipWCharUntil(&local_res10, 0x22);
            pWVar13 = local_res10;
        } else {
            /* -font: */
            pWVar16 = pWVar13 + 6;
            local_70 = pWVar16;
            if (*pWVar16 == L'"') {
                local_70 = pWVar13 + 7;
                pWVar13 = pWVar16;
                goto LAB_1400be241;
            }
        LAB_1400be26d:
            local_b8 = pWVar16;
            uVar15 = 0;
            uVar9 = 0x2d;
            do {
                WVar14 = *local_b8;
                pWVar13 = local_b8;
                while ((WVar14 != (WCHAR)uVar15 &&
                       ((((ushort)WVar14 < 9 || (0xd < (ushort)WVar14)) && (WVar14 != L' '))))) {
                    pWVar13 = pWVar13 + 1;
                    WVar14 = *pWVar13;
                }
                local_res10 = pWVar13;
                local_b8 = pWVar13;
                PECMD_SkipLeadingControlChars(&local_b8);
                WVar14 = (WCHAR)uVar15;
            } while ((*local_b8 != WVar14) && ((WCHAR)uVar9 != *local_b8));
        }
        if (*pWVar13 != WVar14) {
            *pWVar13 = WVar14;
            local_res10 = local_res10 + 1;
        }
        PECMD_SkipLeadingControlChars(&local_res10);
        WVar14 = *local_res10;
        param_3 = local_res18;
        pWVar13 = local_res10;
    }
}

/* ================================================================
 * @0x1400be64c  图标/图像窗口参数解析执行
 * signature: longlong __fastcall PECMD_ImageCommand(LPCWSTR param_1,
 *   LPCWSTR param_2, WPARAM param_3)
 *
 * 解析 "-gui"/-size/-center/-right/-left/-bupdate/.../-num: 选项及
 * 逗号分隔字段; "-size" 分支获取图标尺寸, 否则通过 FUN_1400bd620
 * 构造控件或枚举资源。寄存器拼接 (CONCAT/低32位清零) 已归一化。
 */
int64_t PECMD_ImageCommand(LPCWSTR param_1, LPCWSTR param_2, WPARAM param_3)
{
    WCHAR  WVar1;
    int    iVar12;
    int64_t lVar18;
    char   cVar3;
    byte   bVar4;
    uint   uVar5;
    uint   uVar6;
    uint   uVar16;
    uint   uVar17;
    LPCWSTR pWVar2;
    LPCWSTR pWVar13;
    LPCWSTR pWVar14;
    WCHAR  *pWVar15;
    LPCWSTR pWVar19;
    HICON  pHVar9;
    HMODULE hModule;

    int64_t  *plVar8;
    void     *puVar10;
    LPCWSTR   local_160;
    LPCWSTR   local_158;
    LPCWSTR   local_150;
    WCHAR    *local_res10;
    int       local_res18[2];
    uint64_t  local_res20;
    uint64_t  local_res8;      /* 寄存器: 高位存脚本指针 / 低位存尺寸结果 */
    uint64_t  local_138;       /* 低 32 位为 -size 匹配标志, 亦可作串容器 */
    uint32_t  local_130;
    int       local_148;
    int       local_144[3];
    int64_t   local_128;       /* 串容器 */
    LPCWSTR   local_120;
    uint64_t  local_118;
    WCHAR    *local_110;
    LPCWSTR   local_168;       /* 串容器 */
    uint64_t  local_108;
    uint64_t  local_f8;
    uint64_t  local_f0;
    uint64_t  local_100;
    WCHAR     local_e8[84];
    void     *pptVar11;

    local_res8 = (uint64_t)(uintptr_t)param_1;
    local_res10 = (WCHAR *)param_2;
    PECMD_AllocStrSlot(&local_128);
    pWVar13 = (LPCWSTR)0;
    uVar5 = 0;
    local_118 = 0;
    (void)local_118;
    local_120 = param_1;
    (void)local_120;
    PECMD_SkipLeadingControlChars(&local_res10);
    if (param_3 == 0) {
        PECMD_ResetScriptChain((int64_t *)param_1, 0);
        param_3 = *(WPARAM *)((char *)(uintptr_t)param_1 + 0x20);
        if (param_3 == 0) {
            lVar18 = (int64_t)0xffffffff80070057ULL;
            goto LAB_1400beace;
        }
    }
    iVar12 = 0x2c;
    local_110 = local_res10;
    local_res20 = 0;
    PECMD_SkipWCharUntil(&local_110, 0x2c);
    pWVar15 = local_110;
    WVar1 = *local_110;
    local_160 = local_110;
    *local_110 = L'\0';
    /* 保存逗号字符 (寄存器低16) */
    /* TODO(verify): CONCAT22 源自寄存器拼接, 仅低16位(逗号字符)被后续读取 */
    local_res18[0] = (int)((uint32_t)(local_res18[0] & 0xffff0000u) | WVar1);
    cVar3 = PECMD_MatchTokenAdvance("-gui", &local_res10, iVar12 + -0x28);
    if (cVar3 == '\0') {
        /* ---- 非 -gui: 主解析路径 ---- */
        bVar4 = PECMD_MatchTokenAdvance("-size", &local_res10, 5);
        local_138 = (uint32_t)bVar4;          /* 低32 -size 标志 */
        WVar1 = *local_res10;
        pWVar14 = pWVar13;
        pWVar19 = pWVar13;
        pWVar2 = pWVar13;
        uVar16 = uVar5;
        uVar17 = uVar5;
        uVar6 = uVar5;
        {
            WCHAR *pWVar20 = local_res10;
            while ((local_res10 = pWVar20, WVar1 == L'-')) {
                lVar18 = FUN_14005c72c("-center", (const WCHAR *)pWVar20, 7);
                if ((char)lVar18 == '\0') {          /* -center 未命中 */
                    lVar18 = FUN_14005c72c("-right", (const WCHAR *)pWVar20, 6);
                    if ((char)lVar18 == '\0') {      /* -right 未命中 */
                        lVar18 = FUN_14005c72c("-left", (const WCHAR *)pWVar20, 5);
                        if ((char)lVar18 == '\0') {  /* -left 未命中 */
                            lVar18 = FUN_14005c72c("-bupdate", (const WCHAR *)pWVar20, 8);
                            uVar16 = (uint)(uintptr_t)pWVar19;
                            if ((char)lVar18 == '\0') { /* -bupdate 未命中 */
                                lVar18 = FUN_14005c72c("-real", (const WCHAR *)pWVar20, 5);
                                if ((char)lVar18 == '\0') { /* -real 未命中 */
                                    lVar18 = FUN_14005c72c("-smooth", (const WCHAR *)pWVar20, 7);
                                    if ((char)lVar18 == '\0') { /* -smooth 未命中 */
                                        lVar18 = FUN_14005c72c("-tab", (const WCHAR *)pWVar20, 4);
                                        if ((char)lVar18 == '\0') { /* -tab 未命中 */
                                            lVar18 = FUN_14005c72c("-numicong", (const WCHAR *)pWVar20, 9);
                                            uVar16 = (uint)(uintptr_t)pWVar14;
                                            if ((char)lVar18 == '\0') { /* -numicong 未命中 */
                                                lVar18 = FUN_14005c72c("-numicon", (const WCHAR *)pWVar20, 8);
                                                if ((char)lVar18 == '\0') { /* -numicon 未命中 */
                                                    lVar18 = FUN_14005c72c("-numbmp", (const WCHAR *)pWVar20, 7);
                                                    if ((char)lVar18 == '\0') { /* -numbmp 未命中 */
                                                        if ((char)PECMD_AsciiPrefixICmp("-num:", (const WCHAR *)pWVar20, 5) != '\0') {
                                                            local_res10 = pWVar20 + 5;
                                                            PECMD_EvalParenStripped((int64_t *)&local_res10, &local_res20);
                                                            pWVar14 = (LPCWSTR)(uintptr_t)(uVar16 | 0x100000);
                                                            pWVar20 = local_res10;
                                                        }
                                                    } else { /* -numbmp */
                                                        pWVar14 = (LPCWSTR)(uintptr_t)(uVar16 | 0x100002);
                                                    }
                                                } else { /* -numicon */
                                                    pWVar14 = (LPCWSTR)(uintptr_t)(uVar16 | 0x100001);
                                                }
                                            } else { /* -numicong */
                                                pWVar14 = (LPCWSTR)(uintptr_t)(uVar16 | 0x100011);
                                            }
                                        } else { /* -tab */
                                            pWVar19 = (LPCWSTR)(uintptr_t)(uVar16 | 0x800);
                                        }
                                    } else { /* -smooth */
                                        pWVar19 = (LPCWSTR)(uintptr_t)(uVar16 | 0x400);
                                    }
                                } else { /* -real */
                                    pWVar19 = (LPCWSTR)(uintptr_t)(uVar16 | 0x200);
                                }
                            } else { /* -bupdate */
                                pWVar19 = (LPCWSTR)(uintptr_t)(uVar16 | 0x100);
                            }
                        } else { /* -left */
                            pWVar13 = (LPCWSTR)0x80000000;
                        }
                    } else { /* -right */
                        pWVar13 = (LPCWSTR)0x20000000;
                    }
                } else { /* -center */
                    pWVar13 = (LPCWSTR)0x40000000;
                }
                /* 跳过本项到空白/结尾 */
                for (;;) {
                    uVar6 = (uint)(uintptr_t)pWVar14;
                    uVar17 = (uint)(uintptr_t)pWVar19;
                    uVar16 = (uint)(uintptr_t)pWVar13;
                    WVar1 = *pWVar20;
                    if ((WVar1 == L'\0') ||
                        (((8 < (ushort)WVar1 && ((ushort)WVar1 < 0xe)) || (WVar1 == L' '))))
                        break;
                    local_res10 = pWVar20 + 1;
                    pWVar20 = local_res10;
                }
                PECMD_SkipLeadingControlChars(&local_res10);
                param_1 = (LPCWSTR)local_res8;
                pWVar2 = (LPCWSTR)(uintptr_t)local_res20;
                pWVar20 = local_res10;
                pWVar15 = (WCHAR *)local_160;
                WVar1 = *local_res10;
            }
        }
        local_130 = uVar17 | uVar16;
        *pWVar15 = (WCHAR)local_res18[0];
        PECMD_StrDupAssign(&local_168, g_szEmpty);
        PECMD_StrDupAssign(&local_158, g_szEmpty);
        PECMD_StrDupAssign(&local_160, g_szEmpty);
        PECMD_StrDupAssign(&local_150, g_szEmpty);
        local_144[1] = (int)0x80000000;
        local_144[2] = (int)0x80000000;
        local_148 = 0;
        local_144[0] = 0;
        local_res20 = local_res20 & 0xffffffff00000000ULL;
        local_res18[0] = 0;
        local_res8 = local_res8 & 0xffffffff00000000ULL;
        lVar18 = 1;
        WVar1 = *local_res10;
        if (WVar1 == L'*') {
            local_res10 = local_res10 + 1;
            PECMD_SkipLeadingControlChars(&local_res10);
        }
        PECMD_SplitTokenTrimWs(&local_res10, &local_168, 0x2c);
        PECMD_RunCommandLine((int64_t *)param_1, &local_168, 1);
        if (*local_res10 == L',') {
            local_res10 = local_res10 + 1;
            plVar8 = PECMD_SplitTokenAssignVar((WCHAR **)&local_128, &local_res10, 0x2c, 1);
            PECMD_ParseLtwhParams(plVar8, (uint32_t *)&local_148, local_144, (int *)&local_res20,
                          (uint32_t *)local_res18);
            if (*local_res10 == L',') {
                local_res10 = local_res10 + 1;
                plVar8 = PECMD_SplitTokenAssignVar((WCHAR **)&local_128, &local_res10, 0x2c, 1);
                PECMD_SplitTokenTrimWs(plVar8, &local_158, 0);
            }
            pWVar13 = PECMD_UnquoteString(local_158);
            if ((uint32_t)local_138 != 0) {     /* -size 分支 */
                pHVar9 = PECMD_LoadIcon(local_158, (uint64_t *)0);
                local_res18[0] = 0;
                local_res8 = local_res8 & 0xffffffff00000000ULL;
                if (pHVar9 == (HICON)0) {
                    uVar5 = GetLastError();
                    if (uVar5 == 0) {
                        uVar5 = 1;
                    }
                } else {
                    ((void (*)(HICON, uint32_t *))g_pGdipGetImageWidth)(pHVar9, (uint32_t *)local_res18);
                    ((void (*)(HICON, void *))g_pGdipGetImageHeight)(pHVar9, &local_res8);
                }
                /* TODO(verify): Ghidra 丢弃了 wsprintfW 实参, 宽度/高度来自图像尺寸 */
                wsprintfW(local_e8, WSTR("%d %d"), local_res18[0], (int)(uint32_t)local_res8);
                PECMD_SetVariable((int64_t *)param_1, (LPCWSTR)local_168, local_e8);
                PECMD_FreeStrBuf(&local_150);
                PECMD_FreeStrBuf(&local_160);
                PECMD_FreeStrBuf(&local_158);
                PECMD_FreeStrBuf(&local_168);
                PECMD_FreeStrBuf(&local_128);
                return (int64_t)(int32_t)uVar5;
            }
            if (*local_res10 == L',') {
                local_res10 = local_res10 + 1;
                PECMD_CopyUpToChar((int64_t *)&local_res10, (int64_t *)&local_160, 0x2c);
                if (*local_res10 == L',') {
                    local_res10 = local_res10 + 1;
                    plVar8 = PECMD_SplitTokenAssignVar((WCHAR **)&local_128, &local_res10, 0x2c, 1);
                    PECMD_SplitTokenTrimWs(plVar8, &local_150, 0);
                    if (*local_res10 == L',') {
                        local_res10 = local_res10 + 1;
                        puVar10 = PECMD_SplitTokenAssignVar((WCHAR **)&local_128, &local_res10, 0x2c, 1);
                        PECMD_ParseShortStore(puVar10, (int *)&local_res8, 0x2c);
                    }
                }
            }
            local_res10 = (WCHAR *)local_150;
            PECMD_SkipLeadingControlChars(&local_res10);
            if (*local_res10 != L'#') {
                PECMD_ParseHexOrDecBool(&local_res10, local_144 + 1);
            }
            PECMD_SkipLeadingControlChars(&local_res10);
            if (*local_res10 == L'#') {
                local_res10 = local_res10 + 1;
                PECMD_ParseHexOrDecBool(&local_res10, local_144 + 2);
            }
            if (uVar6 == 0) {
                PECMD_StrDupAssign(&local_138, pWVar13);
                if (WVar1 != L'*') {
                    param_1 = *(LPCWSTR *)((char *)(uintptr_t)param_3 + 0x290);
                }
                FUN_1400bd620(param_3, param_1, &local_168, local_148, local_144[0],
                              (int)(uint32_t)local_res20, local_res18[0], &local_138,
                              &local_160, local_144 + 1, (uint32_t)local_res8, local_130);
                PECMD_FreeStrBuf(&local_138);
            } else {
                uVar6 = uVar6 & 0xff;
                local_108 = 0;
                local_f8 = 0;
                local_f0 = 0;
                (void)local_f0;
                local_100 = 0x7fffffff;
                (void)local_100;
                if (uVar6 == 1) {
                    pWVar15 = (WCHAR *)0x3;
                } else if (uVar6 == 2) {
                    pWVar15 = (WCHAR *)0x2;
                } else {
                    pWVar15 = (WCHAR *)0xe;
                    if (uVar6 == 0) {
                        pWVar15 = (WCHAR *)pWVar2;
                    }
                }
                hModule = LoadLibraryExW(pWVar13, (HANDLE)0, 2);
                if (hModule != (HMODULE)0) {
                    EnumResourceNamesW(hModule, pWVar15, (void *)PECMD_PushStringToken,
                                       (LONG_PTR)&local_108);
                    FreeLibrary(hModule);
                }
                /* TODO(verify): Ghidra 丢弃了 wsprintfW 实参, 计数来自资源枚举回调 */
                wsprintfW(local_e8, WSTR("%d %d"), (int)(uint32_t)local_108,
                          (int)(uint32_t)local_f8);
                PECMD_SetVariable((int64_t *)param_1, (LPCWSTR)local_168, local_e8);
            }
            PECMD_FreeStrBuf(&local_150);
            PECMD_FreeStrBuf(&local_160);
            PECMD_FreeStrBuf(&local_158);
            PECMD_FreeStrBuf(&local_168);
            PECMD_FreeStrBuf(&local_128);
            return 0;
        }
        PECMD_FreeStrBuf(&local_150);
        PECMD_FreeStrBuf(&local_160);
        PECMD_FreeStrBuf(&local_158);
        pptVar11 = &local_168;
    } else {
        /* ---- -gui 分支 ---- */
        PECMD_StrDupAssign(&local_res8, WSTR("#32:INDATA "));
        PECMD_AppendWideStr((WCHAR **)&local_res8, local_res10);
        lVar18 = PECMD_RunCommand((int64_t *)param_1, (LPCWSTR)local_res8);
        pptVar11 = &local_res8;
    }
    PECMD_FreeStrBuf(pptVar11);
LAB_1400beace:
    PECMD_FreeStrBuf(&local_128);
    return lVar18;
}

/* ================================================================
 * @0x1400bf068  把图像/图标挂到控件上 (bmp 更新)
 * signature: undefined8 __fastcall PECMD_AttachControlImage(longlong * param_1,
 *   LPCWSTR param_2)
 *
 * 解析 "[-u ]路径[,资源]" 并装载图像写入变量/控件。
 */
uint64_t PECMD_AttachControlImage(int64_t *param_1, LPCWSTR param_2)
{
    WCHAR   WVar1;
    bool    bVar2;
    LPCWSTR pWVar3;
    int     iVar4;
    int64_t *plVar5;
    WCHAR  *puVar6;
    uint64_t uVar7;
    char    cVar8;
    WCHAR  *local_res8;
    WCHAR  *local_res10;
    void   *local_res18;
    void   *local_res20;
    int64_t local_58;
    int64_t *local_50;
    uint64_t local_48;

    PECMD_AllocStrSlot(&local_58);
    uVar7 = 0;
    local_48 = 0;
    (void)local_48;
    bVar2 = false;
    cVar8 = '\0';
    local_res8 = (WCHAR *)param_2;
    if (*param_2 == L'*') {
        cVar8 = (char)*param_2;
        local_res8 = (WCHAR *)param_2 + 1;
    }
    for (; (WVar1 = *local_res8, WVar1 != L'\0' &&
           ((((ushort)WVar1 < 9 || (0xd < (ushort)WVar1)) && (WVar1 != L' '))));
         local_res8 = local_res8 + 1) {
    }
    local_50 = param_1;
    (void)local_50;
    PECMD_SkipLeadingControlChars(&local_res8);
    pWVar3 = local_res8;
    iVar4 = StrCmpNIW(local_res8, WSTR("-u"), 2);
    if (iVar4 == 0) {
        local_res8 = (WCHAR *)pWVar3 + 2;
        bVar2 = true;
        WVar1 = *local_res8;
        while ((WVar1 != L'\0' && (((ushort)WVar1 < 9 || (0xd < (ushort)WVar1)))) &&
               (WVar1 != L' ')) {
            local_res8 = local_res8 + 1;
            WVar1 = *local_res8;
        }
        PECMD_SkipLeadingControlChars(&local_res8);
    }
    PECMD_AllocStrSlot(&local_res20);
    PECMD_AllocStrSlot(&local_res18);
    local_res10 = 0;
    plVar5 = PECMD_SplitTokenAssignVar((WCHAR **)&local_58, &local_res8, 0x2c, 1);
    PECMD_SplitTokenTrimWs(plVar5, &local_res20, 0);
    if (*local_res8 == L',') {
        local_res8 = local_res8 + 1;
        PECMD_SkipLeadingControlChars(&local_res8);
        if (*local_res8 != L'\0') {
            plVar5 = PECMD_SplitTokenAssignVar((WCHAR **)&local_58, &local_res8, 0x2c, 1);
            PECMD_SplitTokenTrimWs(plVar5, &local_res18, 0);
        }
        PECMD_RunCommandLine(param_1, &local_res18, 0);
    }
    if (bVar2) {
        if (cVar8 == '\0') {
            PECMD_FreeStrBuf(&local_res10);
            PECMD_FreeStrBuf(&local_res18);
            PECMD_FreeStrBuf(&local_res20);
            PECMD_FreeStrBuf(&local_58);
            return 0xffffffff80070057ULL;
        }
        local_res8 = 0;
        PECMD_WideToAnsiStr((int64_t *)&local_res8, (LPCWSTR)local_res20, -1, 0xffffffffffffffffULL);
        puVar6 = (WCHAR *)PECMD_DecodeBase64ToWideStr((byte *)local_res8, (ulonglong *)&local_res10);
        PECMD_FreeStrBuf(&local_res8);
    } else {
        puVar6 = (WCHAR *)PECMD_EncodeStringId((LPCWSTR)local_res20, &local_res10, cVar8);
    }
    if (puVar6 == (WCHAR *)0) {
        uVar7 = 0xffffffff80070057ULL;
    } else {
        *local_res10 = L' ';
        if (*(WCHAR *)local_res18 == L'\0') {
            PECMD_ClipboardCommand(local_res10);
        } else {
            PECMD_SetVariable(param_1, (LPCWSTR)local_res18, local_res10 + 1);
        }
    }
    PECMD_FreeStrBuf(&local_res10);
    PECMD_FreeStrBuf(&local_res18);
    PECMD_FreeStrBuf(&local_res20);
    PECMD_FreeStrBuf(&local_58);
    return uVar7;
}
