/*
 * core_b3r_h4.c — Restored batch28/gh4 business functions (group h4)
 *
 *   PECMD_VarSearchReplace @0x1400a1938  通用变量搜索/替换 (通配/列表复制)
 *   FUN_1400a2390 @0x1400a2390  增强变量解析 (前缀/-delims/-trim 等)
 *   PECMD_FormatTimeString @0x1400a3310  日期/时间变量格式化成串
 *   PECMD_LoadTasksWait @0x1400a4504  并发任务装载与等待 (--try/--try+星/--exist)
 *   PECMD_ParseCommaParams @0x1400a5e6c  解析 ",,,,," 分隔的参数
 *   PECMD_CreateMenuItem @0x1400a600c  菜单项/工具条构造
 *   PECMD_DirWildcardExpand @0x1400a662c  目录通配展开 (dir /B)
 *
 * 说明:
 *   - FUN_140102a90(...) 是 memset 别名, 直接写 memset。
 *   - thunk_FUN_* 跳转桩 -> 直接调用真实 FUN_*。
 *   - CONCATxx 片断合并写成普通整数/位运算。
 *   - Ghidra 切片字段 (local_x._2_8_, _10_4_) 已按 SYSTEMTIME 字段还原。
 *   - 大量辅助函数/全局仅 extern; 未在本项目出现的符号见文件尾注释。
 */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>

#include "win32_stub.h"
#include "pecmd_defs.h"

/* ---- Ghidra primitive aliases ---- */
typedef unsigned int        uint;
typedef unsigned short      ushort;
typedef uint8_t             byte;
typedef uint8_t             undefined1;
typedef uint16_t            undefined2;
typedef uint32_t            undefined4;
typedef uint64_t            undefined8;
typedef void                undefined;
typedef int64_t             longlong;
typedef uint64_t            ulonglong;
typedef undefined8 *        pcode_t;

/* 用于回复 _code* 函数指针槽 */
typedef void *              code_ptr;
#define code  void

/* TIME_ZONE_INFORMATION —— win32_stub.h 未提供, 此处定义 (编译用) */
typedef struct tagTIME_ZONE_INFORMATION {
    LONG   Bias;
    WCHAR  StandardName[32];
    SYSTEMTIME StandardDate;
    LONG   StandardBias;
    WCHAR  DaylightName[32];
    SYSTEMTIME DaylightDate;
    LONG   DaylightBias;
} TIME_ZONE_INFORMATION;
typedef TIME_ZONE_INFORMATION _TIME_ZONE_INFORMATION;

/* 缺失的 Windows API 声明 (win32_stub.h 未含) */
extern BOOL GetTimeZoneInformation(TIME_ZONE_INFORMATION *);

/* ---- 全局数据 (link_stubs.c / core_globals.c 定义) ---- */
extern WCHAR g_szEmpty[];          /* 空串 (.rdata)              */
extern const WCHAR g_wsz28f44[];      /* 选项串常量 (.rdata)         */
extern const WCHAR g_wsz28eb8[];      /* 选项串常量 (.rdata)         */
extern const WCHAR g_wsz28bf4[];      /* 选项串常量 (.rdata)         */

/* 惰性加载的函数指针槽 */
extern int  (*g_pUiCallback)(int, int);  /* 全局槽 (调用 (*)(0,..))      */
extern int  (*g_pOle32Slot828)(int,int,int,int,int,int,int,int,int); /* 全局槽 */

/* ---- 本文件引用的辅助函数 (定义于其它文件, 仅 extern) ---- */
extern void *operator_new(size_t size);
extern void  FUN_14005b104(void *ps);
extern WCHAR *PECMD_SkipLeadingControlChars(void *pp);
extern char  PECMD_MatchTokenAdvance(const char *tok, void *pp, int n);
extern int64_t PECMD_AsciiPrefixICmp(const char *a, const void *w, int n);
extern int64_t FUN_14005c72c(const char *a, const void *w, int n);
extern int32_t PECMD_AsciiWideICmp(const char *a, const void *w);
extern void  PECMD_StrDupAssign(void *ps, const WCHAR *src);
extern void  PECMD_RunCommandLine(void *script, void *str, int mode);
extern void  PECMD_AllocStrSlot(void *out);
extern WCHAR *PECMD_AppendWideStr(void *ps, const WCHAR *src);
extern WCHAR *PECMD_StrCopyW(void *ps, LPCWSTR src, int64_t len);
extern void  PECMD_SetVariable(int64_t *script, LPCWSTR key, LPCWSTR value);
extern void  FUN_1400669c4(int64_t *a, uint64_t b, LPCWSTR c);
extern void  PECMD_SplitTokenTrimWs(void *src, void *dst, int16_t delim);
extern void  PECMD_AppendFormattedI64(int64_t *list, int64_t pos);
extern uint64_t PECMD_CountNewlines(uint64_t *a, int b, void *c, int d);
extern WCHAR *PECMD_SkipWCharUntil(void *pp, uint16_t ch);
extern void  PECMD_ExpandVarDispatch(void *, LPCWSTR, void *, int, int);
extern uint64_t PECMD_EvalParenStripped(void *pp, uint64_t *out);
extern bool  PECMD_ParseUIntValue(void *pp, int *out);
extern void  PECMD_ParseShortStore(void *pp, int *out, WCHAR sep);
extern int64_t *PECMD_SplitTokenAssignVar(void *out, void *pp, uint32_t sep, int flag);
extern void  PECMD_ExtractTokenByDelim(void *src, void *dst, int mode);
extern int64_t PECMD_VectorSlotPtr(int64_t a, int64_t *b, int64_t *c, uint32_t d);
extern void  PECMD_VectorAppendGen(int64_t *a, int64_t *b, int64_t *c, void *d, int e, int f);
extern void  FUN_140061c44(void);
extern int64_t PECMD_DelayLoadOleaut32(void);
extern void  PECMD_UpdateTrayIcon(void *pc, uint64_t hwnd, LPCWSTR s, void *hicon, uint32_t flag);
extern void  PECMD_ScaleWindowPos(int64_t param_1, uint32_t param_2, int *param_3);
extern void  PECMD_ResetScriptChain(void *script, void *a2);
extern WCHAR *PECMD_NextToken(void *param_1, void *param_2, uint32_t param_3);
extern void  PECMD_ParseNumSkipChar_01f8(void *pp, int *out);
extern void  FUN_140067d20(void *pp, int *out);
extern int64_t *PECMD_CopyStrToSlot(void *a1, void *a2);
extern int64_t *PECMD_ReplaceStringSlot(void *ps, void *src);
extern WCHAR *PECMD_AssignString(void *ps, const WCHAR *src);
extern LPCWSTR PECMD_ExtractTokenByIndex(uint64_t a1, int *a2, LPCWSTR a3,
                             int64_t *a4, uint32_t a5, uint16_t *a6);
extern LPCWSTR PECMD_StripTrailingSpaces(LPCWSTR s);
extern uint64_t PECMD_IsNumericString(const WCHAR *p);
extern void  PECMD_QuoteTokenString(WCHAR **pp, WCHAR **out, int len);
extern void  FUN_140063b64(void *out);
extern void  FUN_1400633a8(void **ps, int64_t len);
extern void *PECMD_GrowByteBuffer(void *ps, int64_t len);
extern void  PECMD_TrimWs(WCHAR **pp, uint64_t flags, uint16_t ch);
extern void  PECMD_TrimOuterQuotes(WCHAR **pp, uint8_t flags, uint16_t ch);
extern uint16_t **PECMD_SkipRepeatedDelimiter(uint16_t **pp, uint16_t ch);
extern char  FUN_140062fc4(LPCWSTR param_1, void *param_2, int param_3);
extern void  PECMD_ForwardCall_6838(int64_t obj, int64_t key);
extern void  PECMD_ReleaseObjectSlots(int64_t param_1, int64_t param_2);
extern char  PECMD_MatchAndPad(const void *key, WCHAR **pp, int len);
extern uint32_t PECMD_ArgTokenize(int64_t *param_1, LPCWSTR param_2, int param_3);
extern void  PECMD_JoinTokensAndResolve(void *ctx, void *pp, void *out);
extern int   PECMD_ParseCommaNumbers(WCHAR **pp, uint16_t *year, uint8_t flag);
extern int   PECMD_CalcDayOfYear(uint16_t *a1);
extern int   PECMD_CalcCalendarMonthRows(uint16_t *a, int b);
extern void  PECMD_GetTime100ns(int64_t *out);
extern int64_t PECMD_GetTimeMs(void);
extern int64_t PECMD_GetTimeNs(void);
extern void  FUN_1400e6d68(LPCWSTR param_1, uint64_t param_2);
extern void  FUN_1400e6d74(LPCWSTR param_1, uint64_t param_2);
extern FILETIME FUN_14000e26c(void *script, void *cmd, void *s3, void *s4,
                              uint32_t flag, void *p6, void *s7, void *p8);
extern void  PECMD_ScriptCopy(uint64_t *obj, uint64_t *p);
extern void  PECMD_ScriptInit(uint64_t *obj, int64_t p);
extern uint64_t PECMD_ParseAndExecuteLine(int64_t param_1, LPCWSTR param_2, uint64_t param_3, uint64_t param_4);
extern void  PECMD_ClearTaskTable(int64_t script, int mode);
extern LPCWSTR PECMD_ParseCommandPath(int64_t *script, WCHAR *key, uint32_t *mode, int64_t *out);
extern void  PECMD_NotifyMainWindowRefresh(int64_t *param_1, int flag);
extern void  FUN_1400e5248(int64_t a, uint16_t *b, HMENU c, int64_t d, int64_t e, int64_t *f);
extern void  PECMD_MenuRegisterCommand(void *a, uint16_t *b, HMENU c, int64_t d, int64_t e, int64_t *f);
extern void  PECMD_MenuItemWithIcon(HMENU menu, UINT flags, uintptr_t item, void *data);
extern void *PECMD_CreateNamedLock(LPCWSTR s, char c, uint32_t *mode);
extern void  PECMD_ArrayAppend(int64_t script, int64_t obj);
extern bool  PECMD_ParseHexOrDecBool(void *pp, int *out);
extern void  PECMD_ParseStringToken(WCHAR **pp, int64_t *param_1, void **out);
extern void  FUN_14001bbac(void *script, int a, HANDLE *b, DWORD c, DWORD ms, DWORD d);
extern undefined8 PECMD_MatchPrefixN(ushort *a, undefined8 *b, int c);
extern void  PECMD_FreeInitObjectList(int64_t p1);

/* memset 别名 FUN_140102a90 */
#define FUN_140102a90(dst, val, n)   memset((dst), (val), (n))


/* ================================================================
 * @0x1400a1938  (LPCWSTR * PECMD_VarSearchReplace(longlong * param_1, LPCWSTR param_2))
 * 通用变量「搜索/替换」: 支持 * 前缀、-case 大小写、~ 取反、逗号列表、
 * 通配(第一个 token)与任意次出现(计数) 复制到变量槽。
 * ================================================================ */
LPCWSTR *PECMD_VarSearchReplace(longlong *param_1, LPCWSTR param_2)
{
  WCHAR WVar1;
  bool bVar2;
  ulonglong uVar3;
  ulonglong uVar4;
  char cVar5;
  char cVar6;
  int iVar7;
  int iVar8;
  int iVar9;
  int iVar10;
  uint uVar11;
  longlong lVar12;
  WCHAR *pWVar13;
  LPCWSTR *ppWVar14;
  WCHAR *pWVar15;
  LPCWSTR *ppWVar16;
  WCHAR *pWVar17;
  LPCWSTR *ppWVar18;
  WCHAR *pWVar19;
  WCHAR WVar20;
  char cVar21;
  bool bVar22;
  WCHAR *local_res10;
  WCHAR *local_d8;
  int local_d0;
  WCHAR *local_c8;
  int local_c0;
  WCHAR *local_b8;
  int local_b0 [2];
  WCHAR *local_a8;
  WCHAR *local_a0;
  WCHAR *local_98;
  WCHAR *local_90;
  ulonglong local_88;
  WCHAR *local_80;
  WCHAR *local_78;
  ulonglong local_70;
  WCHAR *local_68;
  uint64_t local_60;
  uint64_t local_58;
  WCHAR *local_50;
  uint64_t local_48;
  uint64_t local_40;

  ppWVar16 = (LPCWSTR *)0x0;
  cVar21 = '\0';
  local_res10 = (WCHAR *)param_2;
  if (*param_2 == L'*') {
    cVar21 = (char)*param_2;
    local_res10 = (WCHAR *)(param_2 + 1);
  }
  PECMD_SkipLeadingControlChars(&local_res10);
  cVar5 = PECMD_MatchTokenAdvance("*", &local_res10, 1);
  cVar6 = '\0';
  if (cVar5 != '\0') {
    cVar6 = PECMD_MatchTokenAdvance("*", &local_res10, 1);
  }
  pWVar17 = g_szEmpty;
  local_a0 = g_szEmpty;
  PECMD_StrDupAssign(&local_a8, g_szEmpty);
  pWVar13 = local_res10;
  lVar12 = FUN_14005c72c("-case", (ushort *)local_res10, 5);
  bVar22 = (char)lVar12 != '\0';
  if (bVar22) {
    local_res10 = pWVar13 + 5;
    PECMD_SkipLeadingControlChars(&local_res10);
    pWVar13 = local_res10;
  }
  if (*pWVar13 == L'\0') {
    FUN_14005b104(&local_a8);
    return (LPCWSTR *)0x0;
  }
  PECMD_SplitTokenTrimWs(&local_res10, &local_a8, 0x3d);
  PECMD_RunCommandLine(param_1, &local_a8, 1);
  local_98 = local_a8;
  local_78 = local_a8;
  PECMD_SkipWCharUntil(&local_78, 0x2c);
  if (*local_78 != L'\0') {
    *local_78 = L'\0';
    pWVar17 = local_78 + 1;
    local_a0 = pWVar17;
    local_78 = pWVar17;
  }
  PECMD_AllocStrSlot(&local_50);
  local_48 = 0;
  local_40 = 0;
  PECMD_AllocStrSlot(&local_68);
  local_60 = 0;
  local_58 = 0;
  (void)local_48;
  (void)local_40;
  (void)local_60;
  (void)local_58;
  ppWVar18 = ppWVar16;
  if (*local_a8 == L'*') {
    local_a8 = local_a8 + 1;
    ppWVar18 = (LPCWSTR *)&local_50;
    local_98 = local_a8;
  }
  ppWVar14 = ppWVar16;
  if (*pWVar17 == L'*') {
    pWVar17 = pWVar17 + 1;
    ppWVar14 = (LPCWSTR *)&local_68;
    local_a0 = pWVar17;
  }
  local_c0 = 1;
  local_d0 = 1;
  if (*local_a8 == L'~') {
    local_a8 = local_a8 + 1;
    local_c0 = -1;
    local_98 = local_a8;
  }
  local_88 = -(ulonglong)(*local_a8 != L'\0') & (ulonglong)ppWVar18;
  if (*pWVar17 == L'~') {
    pWVar17 = pWVar17 + 1;
    local_d0 = -1;
    local_a0 = pWVar17;
  }
  local_70 = -(ulonglong)(*pWVar17 != L'\0') & (ulonglong)ppWVar14;
  if ((local_88 != 0) || (bVar2 = true, local_70 != 0)) {
    bVar2 = false;
  }
  if ((*local_a8 == L'\0') && (*pWVar17 == L'\0')) goto LAB_1400a234b;
  if (*local_res10 != L'=') {
LAB_1400a1bd9:
    ppWVar16 = (LPCWSTR *)0xffffffff80070057;
    goto LAB_1400a234b;
  }
  local_d8 = local_res10 + 1;
  local_res10 = local_d8;
  do {
    pWVar13 = local_res10;
    local_res10 = pWVar13 + 1;
    if (*local_res10 == L'\0') {
      if (*local_res10 != L',') goto LAB_1400a1bd9;
      break;
    }
  } while (*local_res10 != L',');
  *local_res10 = L'\0';
  local_res10 = pWVar13 + 2;
  local_80 = (WCHAR *)0;
  PECMD_AllocStrSlot(&local_90);
  ppWVar18 = (LPCWSTR *)0x0;
  WVar20 = L' ';
  if (cVar21 == '\0') {
    local_c8 = local_d8;
    pWVar19 = local_d8;
    PECMD_SkipLeadingControlChars(&local_c8);
    if ((*local_c8 != L'\0') &&
       ((((ushort)(WVar20 + 0xffe9) <= (ushort)*pWVar19 && ((ushort)*pWVar19 < 0xe)) ||
         (WVar20 == *pWVar19)))) {
      local_d8 = local_c8;
      pWVar19 = local_c8;
    }
    pWVar13 = local_res10 + -2;
    if (pWVar19 < pWVar13) {
      do {
        if ((((ushort)*pWVar13 < (ushort)(WVar20 + 0xffe9)) || (0xd < (ushort)*pWVar13)) &&
           (WVar20 != *pWVar13)) break;
        *pWVar13 = L'\0';
        pWVar13 = pWVar13 + -1;
      } while (local_d8 < pWVar13);
    }
  }
  PECMD_AllocStrSlot(&local_b8);
  PECMD_ExpandVarDispatch(param_1, local_d8, &local_b8, 0, 1);
  local_d8 = local_b8;
  if (cVar6 != '\0') {
    local_c8 = local_b8;
    local_b8 = (WCHAR *)0x0;
    PECMD_JoinTokensAndResolve(param_1, &local_d8, (int64_t *)&local_b8);
    local_d8 = local_b8;
    FUN_14005b104(&local_c8);
  }
  WVar20 = *local_d8;
  uVar11 = 0;
  if (WVar20 == L'\0') {
    iVar7 = (int)(uintptr_t)local_res10;
    pWVar13 = local_res10;
    ppWVar18 = ppWVar16;
    pWVar19 = local_res10;
  }
  else {
    WVar1 = local_d8[1];
    PECMD_ExpandVarDispatch(param_1, local_res10, &local_90, 0, 1);
    local_res10 = local_90;
    local_b0[0] = 0;
    PECMD_ParseShortStore(&local_res10, local_b0, 0x2c);
    if (*local_res10 != L',') {
      FUN_14005b104(&local_b8);
      FUN_14005b104(&local_90);
      FUN_14005b104(&local_80);
      goto LAB_1400a1bd9;
    }
    local_res10 = local_res10 + 1;
    pWVar13 = local_res10;
    if (cVar5 != '\0') {
      PECMD_JoinTokensAndResolve(param_1, &local_res10, (int64_t *)&local_80);
      pWVar13 = local_80;
    }
    local_res10 = pWVar13;
    iVar7 = lstrlenW(pWVar13);
    uVar4 = local_70;
    uVar3 = local_88;
    pWVar19 = (WCHAR *)((longlong)iVar7 + 1);
    if (bVar22) {
LAB_1400a1e3b:
      cVar21 = '\0';
    }
    else {
      if ((ushort)(WVar20 + 0xffbf) < 0x1a) {
        WVar20 = WVar20 | 0x20;
      }
      cVar21 = '\x01';
      if (0x7a < (ushort)WVar20) goto LAB_1400a1e3b;
    }
    pWVar15 = (WCHAR *)(longlong)local_c0;
    local_c8 = pWVar15;
    if (WVar1 == L'\0') {
      local_res10 = pWVar13 + (longlong)iVar7 + -1;
      ppWVar18 = ppWVar16;
      iVar9 = local_b0[0];
      if (local_b0[0] < 1) {
        WVar1 = *pWVar13;
        local_res10 = pWVar13;
        while (WVar1 != L'\0') {
          WVar1 = *local_res10;
          if ((WVar1 == WVar20) ||
             (((!bVar22 && ((ushort)WVar1 < 0x5b)) && ((WCHAR)(WVar1 | 0x20U) == WVar20)))) {
            ppWVar18 = (LPCWSTR *)(((longlong)local_res10 - (longlong)(pWVar13 + -1)) >> 1);
            if (uVar3 != 0) {
              ppWVar14 = ppWVar18;
              if (((longlong)pWVar15 < 1) && (0 < (longlong)ppWVar18)) {
                ppWVar14 = (LPCWSTR *)((longlong)pWVar19 - (longlong)ppWVar18);
              }
              PECMD_AppendFormattedI64((longlong *)&local_50, (longlong)ppWVar14);
            }
            if (uVar4 != 0) {
              iVar9 = (int)PECMD_CountNewlines((ulonglong *)pWVar13, (int)(uintptr_t)ppWVar18, (void *)(longlong)local_d0, iVar7);
              PECMD_AppendFormattedI64((longlong *)&local_68, iVar9);
            }
            pWVar15 = local_c8;
            if (bVar2) break;
          }
          local_res10 = local_res10 + 1;
          WVar1 = *local_res10;
        }
      }
      else {
        for (; pWVar13 <= local_res10; local_res10 = local_res10 + -1) {
          if (((*local_res10 == WVar20) ||
              ((cVar21 != '\0' && ((WCHAR)(*local_res10 | 0x20U) == WVar20)))) &&
             (iVar9 = iVar9 + -1, iVar9 < 1)) {
            ppWVar18 = (LPCWSTR *)(((longlong)local_res10 - (longlong)(pWVar13 + -1)) >> 1);
            if (local_88 != 0) {
              ppWVar14 = ppWVar18;
              if (((longlong)pWVar15 < 1) && (0 < (longlong)ppWVar18)) {
                ppWVar14 = (LPCWSTR *)((longlong)pWVar19 - (longlong)ppWVar18);
              }
              PECMD_AppendFormattedI64((longlong *)&local_50, (longlong)ppWVar14);
            }
            if (local_70 != 0) {
              iVar8 = (int)PECMD_CountNewlines((ulonglong *)pWVar13, (int)(uintptr_t)ppWVar18, (void *)(longlong)local_d0, iVar7);
              PECMD_AppendFormattedI64((longlong *)&local_68, iVar8);
            }
            pWVar15 = local_c8;
            if (bVar2) break;
          }
        }
      }
    }
    else {
      iVar9 = lstrlenW(local_d8);
      pWVar15 = local_c8;
      if (local_b0[0] < 1) {
        local_res10 = pWVar13;
        if (*pWVar13 != L'\0') {
          ppWVar18 = ppWVar16;
          do {
            if (bVar22) {
              iVar8 = StrCmpNW(local_res10, local_d8, iVar9);
            }
            else {
              iVar8 = StrCmpNIW(local_res10, local_d8, iVar9);
            }
            if (iVar8 == 0) {
              ppWVar18 = (LPCWSTR *)(((longlong)local_res10 - (longlong)(pWVar13 + -1)) >> 1);
              if (local_88 != 0) {
                ppWVar14 = ppWVar18;
                if (((longlong)pWVar15 < 1) && (0 < (longlong)ppWVar18)) {
                  ppWVar14 = (LPCWSTR *)((longlong)pWVar19 - (longlong)ppWVar18);
                }
                PECMD_AppendFormattedI64((longlong *)&local_50, (longlong)ppWVar14);
              }
              if (local_70 != 0) {
                iVar8 = (int)PECMD_CountNewlines((ulonglong *)pWVar13, (int)(uintptr_t)ppWVar18, (void *)(longlong)local_d0, iVar7);
                PECMD_AppendFormattedI64((longlong *)&local_68, iVar8);
              }
              if (bVar2) break;
            }
            local_res10 = local_res10 + 1;
          } while (*local_res10 != L'\0');
        }
      }
      else {
        ppWVar18 = ppWVar16;
        iVar8 = local_b0[0];
        for (local_res10 = pWVar13 + (longlong)iVar7 + -1 + (1 - (longlong)iVar9);
            pWVar13 <= local_res10; local_res10 = local_res10 + -1) {
          if (bVar22) {
            iVar10 = StrCmpNW(local_res10, local_d8, iVar9);
          }
          else {
            iVar10 = StrCmpNIW(local_res10, local_d8, iVar9);
          }
          if ((iVar10 == 0) && (iVar8 = iVar8 + -1, iVar8 < 1)) {
            ppWVar18 = (LPCWSTR *)(((longlong)local_res10 - (longlong)(pWVar13 + -1)) >> 1);
            if (local_88 != 0) {
              ppWVar14 = ppWVar18;
              if (((longlong)pWVar15 < 1) && (0 < (longlong)ppWVar18)) {
                ppWVar14 = (LPCWSTR *)((longlong)pWVar19 - (longlong)ppWVar18);
              }
              PECMD_AppendFormattedI64((longlong *)&local_50, (longlong)ppWVar14);
            }
            if (local_70 != 0) {
              iVar10 = (int)PECMD_CountNewlines((ulonglong *)pWVar13, (int)(uintptr_t)ppWVar18, (void *)(longlong)local_d0, iVar7);
              PECMD_AppendFormattedI64((longlong *)&local_68, iVar10);
            }
            if (bVar2) break;
          }
        }
      }
    }
  }
  pWVar17 = local_a0;
  if (*local_68 == L'\0') {
    if (*local_a0 != L'\0') {
      if (0 < (longlong)ppWVar18) {
        uVar11 = PECMD_CountNewlines((ulonglong *)pWVar13, (int)(uintptr_t)ppWVar18, (void *)(longlong)local_d0, iVar7);
      }
      FUN_1400669c4(param_1, uVar11, pWVar17);
    }
  }
  else {
    PECMD_SetVariable(param_1, local_a0, local_68);
  }
  if (*local_50 == L'\0') {
    if ((local_c0 < 1) && (0 < (longlong)ppWVar18)) {
      ppWVar18 = (LPCWSTR *)((longlong)pWVar19 - (longlong)ppWVar18);
    }
    FUN_1400669c4(param_1, (ulonglong)ppWVar18 & 0xffffffff, local_98);
  }
  else {
    PECMD_SetVariable(param_1, local_98, local_50);
  }
  FUN_14005b104(&local_b8);
  FUN_14005b104(&local_90);
  FUN_14005b104(&local_80);
LAB_1400a234b:
  FUN_14005b104(&local_68);
  FUN_14005b104(&local_50);
  FUN_14005b104(&local_a8);
  return ppWVar16;
}

/* ================================================================
 * @0x1400a5e6c  (int PECMD_ParseCommaParams(longlong * param_1, short * param_2,
 *                 longlong * param_3, longlong * param_4, longlong * param_5,
 *                 longlong * param_6))
 * 解析 ",,,,," 分隔的参数串: 依次取最多 4 个字段 (第 4 个字段按整数解析,
 * 若为 "-" 视为 -1)。返回第 4 字段数值 (缺省 0)。
 * ================================================================ */
int PECMD_ParseCommaParams(longlong *param_1, int16_t *param_2, longlong *param_3,
                 longlong *param_4, longlong *param_5, longlong *param_6)
{
  short sVar1;
  longlong *plVar2;
  undefined8 *puVar3;
  int iVar4;
  int local_res8 [2];
  int16_t *local_res10;
  int16_t *local_48;
  longlong local_40;
  longlong *local_38;
  undefined8 local_30;

  local_res8[0] = 0;
  local_res10 = param_2;
  PECMD_SkipLeadingControlChars(&local_res10);
  PECMD_AllocStrSlot(&local_40);
  local_30 = 0;
  local_38 = param_1;
  (void)local_30;
  (void)local_38;
  PECMD_SplitTokenTrimWs(&local_res10, param_3, 0x2c);
  PECMD_RunCommandLine(param_1, param_3, 1);
  iVar4 = 0;
  if (*local_res10 == 0x2c) {
    local_res10 = local_res10 + 1;
    plVar2 = PECMD_SplitTokenAssignVar(&local_40, (longlong *)&local_res10, 0x2c, 1);
    PECMD_ExtractTokenByDelim(plVar2, param_4, 0);
    iVar4 = 0;
    if (*local_res10 == 0x2c) {
      local_res10 = local_res10 + 1;
      plVar2 = PECMD_SplitTokenAssignVar(&local_40, (longlong *)&local_res10, 0x2c, 1);
      PECMD_ExtractTokenByDelim(plVar2, param_5, 0);
      if (*local_res10 == 0x2c) {
        local_res10 = local_res10 + 1;
        puVar3 = (undefined8 *)PECMD_SplitTokenAssignVar(&local_40, (longlong *)&local_res10, 0x2c, 1);
        local_48 = (int16_t *)*puVar3;
        sVar1 = *local_48;
        PECMD_ParseShortStore(&local_48, local_res8, 0x2c);
        iVar4 = local_res8[0];
        if (((short)local_res8[0] == 0) && (sVar1 == 0x2d)) {
          iVar4 = -1;
        }
        if (*local_res10 == 0x2c) {
          local_res10 = local_res10 + 1;
          plVar2 = PECMD_SplitTokenAssignVar(&local_40, (longlong *)&local_res10, 0x2c, 1);
          PECMD_ExtractTokenByDelim(plVar2, param_6, 0);
        }
      }
    }
  }
  FUN_14005b104(&local_40);
  return iVar4;
}

/* ================================================================
 * @0x1400a662c  (_FILETIME PECMD_DirWildcardExpand(longlong * param_1, _FILETIME param_2))
 * 目录通配展开: 解析以 '%' 为前缀的模式串 (可选 "--wd:" 工作目录),
 * 组装 "*cmd.exe /c dir /B ..." 命令并执行, 返回执行结果 (FILETIME 槽复用)。
 * ================================================================ */
FILETIME PECMD_DirWildcardExpand(longlong *param_1, FILETIME param_2)
{
  uint64_t _Var1;
  undefined8 uVar2;
  FILETIME _Var3;
  uint64_t _Var4;
  ushort uVar5;
  uint64_t local_res10[2];   /* 复用: 既保存字符串指针也当作 WCHAR** 槽 */
  LPCWSTR local_res20;
  WCHAR *p;

  (void)p;
  local_res10[0] = 0;
  memcpy(&local_res10[0], &param_2, 8);   /* 8 字节按位复用为指针/值 */
  /* Ghidra: local_res10[0] 最初即 param_2 (8 字节槽) */
  _Var1 = local_res10[0];
  uVar5 = 0x25;
  _Var3.dwLowDateTime = 0;
  _Var3.dwHighDateTime = 0;
  if (*((int16_t *)(uintptr_t)local_res10[0]) == 0x25) {
    do {
      if (((8 < uVar5) && (uVar5 < 0xe)) || (uVar5 == 0x20)) break;
      local_res10[0] = local_res10[0] + 2;
      uVar5 = *((ushort *)(uintptr_t)local_res10[0]);
    } while (uVar5 != 0);
    PECMD_SkipLeadingControlChars((longlong *)local_res10);
  }
  _Var4 = local_res10[0];
  uVar2 = PECMD_AsciiPrefixICmp("--wd:", (ushort *)(uintptr_t)local_res10[0], 5);
  if ((char)uVar2 != '\0') {
    local_res10[0] = _Var4 + 10;
    if (*((ushort *)(uintptr_t)local_res10[0]) == 0x2a) {
      local_res10[0] = _Var4 + 0xc;
    }
    uVar5 = *((ushort *)(uintptr_t)local_res10[0]);
    if (uVar5 == 0x22) {
      local_res10[0] = local_res10[0] + 2;
      uVar5 = *((ushort *)(uintptr_t)local_res10[0]);
      if (uVar5 != 0) {
        do {
          if (uVar5 == 0x22) break;
          local_res10[0] = local_res10[0] + 2;
          uVar5 = *((ushort *)(uintptr_t)local_res10[0]);
        } while (uVar5 != 0);
        for (; ((uVar5 = *((ushort *)(uintptr_t)local_res10[0]), uVar5 != 0 &&
                ((uVar5 < 9 || (0xd < uVar5)))) && (uVar5 != 0x20));
            local_res10[0] = local_res10[0] + 2) {
        }
      }
    }
    else {
      while (((uVar5 != 0 && ((uVar5 < 9 || (0xd < uVar5)))) && (uVar5 != 0x20))) {
        local_res10[0] = local_res10[0] + 2;
        uVar5 = *((ushort *)(uintptr_t)local_res10[0]);
      }
    }
    PECMD_SkipLeadingControlChars((longlong *)local_res10);
    _Var4 = local_res10[0];
  }
  PECMD_AllocStrSlot(&local_res20);
  uVar5 = *((ushort *)(uintptr_t)_Var4);
  if (uVar5 == 0) {
    _Var3.dwLowDateTime = 0x80070057;
    _Var3.dwHighDateTime = 0xffffffff;
  }
  else {
    do {
      if (((8 < uVar5) && (uVar5 < 0xe)) || (uVar5 == 0x20)) break;
      _Var4 = _Var4 + 2;
      uVar5 = *((ushort *)(uintptr_t)_Var4);
      local_res10[0] = _Var4;
    } while (uVar5 != 0);
    PECMD_StrCopyW(&local_res20, (LPCWSTR)(uintptr_t)_Var1,
                  ((longlong)_Var4 - (longlong)_Var1) >> 1);
    PECMD_SkipLeadingControlChars((longlong *)local_res10);
    _Var1 = local_res10[0];
    if (*((int16_t *)(uintptr_t)local_res10[0]) != 0) {
      PECMD_StrDupAssign(local_res10, WSTR("*- "));
      PECMD_AppendWideStr(local_res10, local_res20);
      PECMD_AppendWideStr(local_res10, WSTR("=*cmd.exe /c dir /B "));
      PECMD_AppendWideStr(local_res10, (LPCWSTR)(uintptr_t)_Var1);
      _Var3 = FUN_14000e26c(param_1, (void *)0, param_1, (void *)(uintptr_t)local_res10[0],
                            0, (void *)0, (void *)0, (void *)0);
      FUN_14005b104(local_res10);
    }
  }
  FUN_14005b104(&local_res20);
  return _Var3;
}

/* ================================================================
 * @0x1400a600c  (byte * PECMD_CreateMenuItem(longlong * param_1, short * param_2))
 * 菜单/工具条构造: 解析 (bar/sub/clear)+参数, 加入命令栈, 可选建立子菜单/
 * 工具条并 SetMenu。返回状态 (错误码或成功指针的低 32 位)。
 * ================================================================ */
byte *PECMD_CreateMenuItem(longlong *param_1, int16_t *param_2)
{
  char cVar1;
  char cVar2;
  byte bVar3;
  uint uVar4;
  HMENU hMenu;
  LARGE_INTEGER LVar5;
  undefined1 *puVar6;
  LARGE_INTEGER LVar7;
  HMENU pHVar8;
  longlong lVar9;
  LARGE_INTEGER LVar10;
  LARGE_INTEGER LVar11;
  LPCWSTR pWVar12;
  char *pcVar13;
  int16_t *local_res10;
  uint local_res18;
  uint32_t uStackX_1c;
  LPCWSTR local_res20;
  LPCWSTR local_78;
  LPCWSTR local_70;
  longlong local_68;
  HMENU local_60;
  longlong local_58;
  LPCWSTR local_50;

  local_res10 = param_2;
  PECMD_AllocStrSlot(&local_68);
  PECMD_AllocStrSlot((void *)&local_res20);
  PECMD_AllocStrSlot((void *)&local_70);
  PECMD_AllocStrSlot((void *)&local_78);
  PECMD_SkipLeadingControlChars(&local_res10);
  cVar1 = PECMD_MatchTokenAdvance("-bar", &local_res10, -1);
  cVar2 = PECMD_MatchTokenAdvance("-sub", &local_res10, -1);
  bVar3 = (byte)PECMD_MatchTokenAdvance("-clear", &local_res10, -1);
  local_res18 = (uint)bVar3;
  uVar4 = PECMD_ParseCommaParams(param_1, local_res10, &local_68, (longlong *)&local_res20,
                        (longlong *)&local_70, (longlong *)&local_78);
  lVar9 = param_1[8];
  LVar11.QuadPart = 0;
  if (lVar9 == 0) {
    PECMD_ResetScriptChain(param_1, (longlong *)0x0);
    lVar9 = param_1[8];
    if (lVar9 == 0) {
      LVar11.QuadPart = -0x7ff8ffa9;
      goto LAB_1400a65eb;
    }
  }
  hMenu = GetMenu(*(HWND *)(lVar9 + 0x20));
  local_60 = hMenu;
  EnterCriticalSection((LPCRITICAL_SECTION)&g_csInit);
  pcVar13 = (char *)param_1[3];
  if (pcVar13 == (char *)0x0) {
    pcVar13 = (char *)(param_1[8] + 0x2b0);
  }
  LVar5 = (LARGE_INTEGER)(intptr_t)operator_new(0x48);
  LVar10.QuadPart = LVar11.QuadPart;
  if ((void *)(intptr_t)LVar5.QuadPart != (void *)0x0) {
    PECMD_AllocStrSlot((void *)(uintptr_t)(LVar5.QuadPart + 8));
    PECMD_AllocStrSlot((void *)(uintptr_t)(LVar5.QuadPart + 0x10));
    PECMD_AllocStrSlot((void *)(uintptr_t)(LVar5.QuadPart + 0x18));
    PECMD_AllocStrSlot((void *)(uintptr_t)(LVar5.QuadPart + 0x30));
    *(uint64_t *)(uintptr_t)(LVar5.QuadPart + 0x38) = 0;
    *(uint64_t *)(uintptr_t)(LVar5.QuadPart + 0x40) = 0;
    LVar10 = LVar5;
  }
  PECMD_ReplaceStringSlot((void *)(uintptr_t)(LVar10.QuadPart + 0x18), &local_68);
  *(uint32_t *)(uintptr_t)(LVar10.QuadPart + 0x28) = *(uint32_t *)(pcVar13 + 4);
  PECMD_AssignString((void *)(uintptr_t)(LVar10.QuadPart + 0x10), local_70);
  PECMD_AssignString((void *)(uintptr_t)(LVar10.QuadPart + 8), local_res20);
  PECMD_AssignString((void *)(uintptr_t)(LVar10.QuadPart + 0x30), local_78);
  if ((int)uVar4 < 0) {
    uVar4 = -uVar4 | 1;
  }
  *(byte *)(uintptr_t)LVar10.QuadPart = (byte)uVar4;
  if ((cVar2 == '\0' && cVar1 == '\0') || (local_res18 == 0)) {
    if (**(short **)(uintptr_t)(LVar10.QuadPart + 0x18) == 0x2d) {
      *(byte *)(uintptr_t)LVar10.QuadPart = (byte)uVar4 | 2;
      **(short **)(uintptr_t)(LVar10.QuadPart + 0x18) = 0;
    }
    if (**(short **)(uintptr_t)(LVar10.QuadPart + 0x18) == 0x7c) {
      *(byte *)(uintptr_t)LVar10.QuadPart = *(byte *)(uintptr_t)LVar10.QuadPart | 1;
      **(short **)(uintptr_t)(LVar10.QuadPart + 0x18) = 0;
    }
    PECMD_GrowByteBuffer((void *)(pcVar13 + 8), (longlong)(*(int *)(pcVar13 + 4) + 1) * 0x48);
    if (**(LPCWSTR *)(uintptr_t)(LVar10.QuadPart + 0x18) != L'\0') {
      PECMD_SetVariable(param_1, *(LPCWSTR *)(uintptr_t)(LVar10.QuadPart + 0x18), local_res20);
      PECMD_CopyStrToSlot((void *)&local_res18, (void *)(uintptr_t)(LVar10.QuadPart + 0x18));
      PECMD_AppendWideStr(&local_res18, WSTR(".Enable"));
      if (param_1[3] == 0) {
        pWVar12 = WSTR("1");
        if ((*(byte *)(uintptr_t)LVar10.QuadPart & 1) != 0) {
          pWVar12 = WSTR("0");
        }
        PECMD_SetVariableWithPrefix(param_1,
                      (LPCWSTR)(uintptr_t)(((uint64_t)(uint32_t)uStackX_1c << 32) | (uint32_t)local_res18),
                      pWVar12);
      }
      FUN_14005b104(&local_res18);
    }
    ((LARGE_INTEGER *)(uintptr_t)(*(longlong *)(pcVar13 + 8) +
                     (longlong)*(int *)(pcVar13 + 4) * 8))->QuadPart = (int64_t)LVar10.QuadPart;
    *(int *)(pcVar13 + 4) = *(int *)(pcVar13 + 4) + 1;
    if ((cVar1 != '\0') || (cVar2 != '\0')) {
      if (hMenu == (HMENU)0x0) {
        hMenu = CreateMenu();
        local_60 = hMenu;
      }
      if (cVar2 != '\0') {
        local_58 = param_1[5];
        local_res18 = *(uint *)((longlong)param_1 + 0x24);
        local_50 = PECMD_ParseCommandPath(param_1, *(WCHAR **)(uintptr_t)(LVar10.QuadPart + 0x18),
                                 &local_res18, &local_58);
        if (local_50 == (LPCWSTR)0x0) {
          LeaveCriticalSection((LPCRITICAL_SECTION)&g_csInit);
          LVar11.QuadPart = 1;
          goto LAB_1400a65eb;
        }
        puVar6 = (undefined1 *)operator_new(0x10);
        *(uint32_t *)(puVar6 + 4) = 0;
        *(uint64_t *)(puVar6 + 8) = 0;
        *puVar6 = 0;
        *(undefined1 *)(uintptr_t)LVar10.QuadPart = 0x80;
        if (*(longlong *)(uintptr_t)(LVar10.QuadPart + 0x10) != 0) {
          HeapFree(g_hHeap, 0, (LPVOID)(uintptr_t)(*(longlong *)(uintptr_t)(LVar10.QuadPart + 0x10) + -8));
          *(uint64_t *)(uintptr_t)(LVar10.QuadPart + 0x10) = 0;
        }
        *(undefined1 **)(uintptr_t)(LVar10.QuadPart + 0x10) = puVar6;
        LVar7 = (LARGE_INTEGER)(intptr_t)operator_new(0xf0);
        LVar5.QuadPart = LVar11.QuadPart;
        if ((undefined8 *)(uintptr_t)LVar7.QuadPart != (undefined8 *)0x0) {
          PECMD_ScriptCopy((uint64_t *)(uintptr_t)LVar7.QuadPart, (uint64_t *)param_1);
          PECMD_ScriptInit((uint64_t *)(uintptr_t)LVar7.QuadPart, (longlong)param_1);
          *(uint64_t *)(uintptr_t)(LVar7.QuadPart + 0x40) = 0;
          LVar5 = LVar7;
        }
        *(undefined1 **)(uintptr_t)(LVar5.QuadPart + 0x18) = puVar6;
        *(uint *)(uintptr_t)(LVar5.QuadPart + 0x24) = local_res18 + 1;
        *(longlong *)(uintptr_t)(LVar5.QuadPart + 0x28) = param_1[5];
        PECMD_ArgTokenize((longlong *)(uintptr_t)LVar5.QuadPart, g_szEmpty, 0);
        lVar9 = param_1[8];
        *(longlong *)(uintptr_t)(LVar5.QuadPart + 0x40) = lVar9;
        EnterCriticalSection((LPCRITICAL_SECTION)&g_csInit);
        LeaveCriticalSection((LPCRITICAL_SECTION)&g_csInit);
        PECMD_ParseAndExecuteLine(LVar5.QuadPart, local_50,
                      *(undefined8 *)(uintptr_t)(LVar5.QuadPart + 0x40), 1);
        EnterCriticalSection((LPCRITICAL_SECTION)&g_csInit);
        if (lVar9 != 0) {
          PECMD_ForwardCall_6838(lVar9, LVar5.QuadPart);
          PECMD_ReleaseObjectSlots(lVar9, LVar5.QuadPart);
        }
        LeaveCriticalSection((LPCRITICAL_SECTION)&g_csInit);
        PECMD_ClearTaskTable(LVar5.QuadPart, 0);
        FUN_14005b104((void *)(uintptr_t)(LVar5.QuadPart + 0x70));
        free((void *)(uintptr_t)LVar5.QuadPart);
        if (param_1[3] != 0) goto LAB_1400a65de;
        pHVar8 = CreatePopupMenu();
        lVar9 = param_1[8];
        FUN_1400e5248((longlong)(intptr_t)puVar6, (ushort *)(lVar9 + 0x218), pHVar8,
                      *(longlong *)(lVar9 + 0x20), *(longlong *)(lVar9 + 0x200),
                      *(longlong **)(lVar9 + 0x290));
        hMenu = local_60;
        *(short *)(param_1[8] + 0x218) = *(short *)(param_1[8] + 0x218) + 1;
        PECMD_MenuItemWithIcon(local_60, 0x10, (uintptr_t)pHVar8, (void *)(uintptr_t)LVar10.QuadPart);
      }
    }
    if ((param_1[3] == 0) && (param_1[8] != 0)) {
      if (*(int *)(pcVar13 + 4) == 1) {
        if (cVar1 != '\0') goto LAB_1400a657f;
        if (cVar2 == '\0') {
          PECMD_UpdateTrayIcon(pcVar13, *(uint64_t *)(uintptr_t)(param_1[8] + 0x20),
                        (LPCWSTR)0x0, (void *)0x0, 0x456);
          goto LAB_1400a6576;
        }
      }
      else {
LAB_1400a6576:
        if ((cVar1 == '\0') && (cVar2 == '\0')) goto LAB_1400a65de;
LAB_1400a657f:
        if (cVar2 == '\0') {
          lVar9 = param_1[8];
          PECMD_MenuRegisterCommand((void *)(uintptr_t)LVar10.QuadPart, (ushort *)(lVar9 + 0x218), hMenu,
                        *(longlong *)(lVar9 + 0x20), *(longlong *)(lVar9 + 0x200),
                        *(longlong **)(lVar9 + 0x290));
        }
      }
      if (*(int *)(pcVar13 + 4) == 1) {
        SetMenu(*(HWND *)(param_1[8] + 0x20), hMenu);
        PECMD_ScaleWindowPos(param_1[8], 3, (int *)0x0);
      }
    }
  }
  else if (param_1[3] != 0) {
    if (param_1[8] != 0) {
      SetMenu(*(HWND *)(param_1[8] + 0x20), (HMENU)0x0);
      PECMD_ScaleWindowPos(param_1[8], 3, (int *)0x0);
    }
    pcVar13 = (char *)param_1[3];
    if (pcVar13 != (char *)0x0) {
      PECMD_FreeInitObjectList((longlong)(intptr_t)pcVar13);
      free(pcVar13);
    }
    param_1[3] = 0;
  }
LAB_1400a65de:
  LeaveCriticalSection((LPCRITICAL_SECTION)&g_csInit);
LAB_1400a65eb:
  FUN_14005b104((void *)&local_78);
  FUN_14005b104((void *)&local_70);
  FUN_14005b104((void *)&local_res20);
  FUN_14005b104(&local_68);
  return (byte *)(uintptr_t)(uint32_t)LVar11.LowPart;
}

/* ================================================================
 * @0x1400a3310  (longlong PECMD_FormatTimeString(longlong * param_1, WCHAR * param_2))
 * 日期/时间变量格式化: 解析 (utc/gmt/local/sys/uptime/bsys/space/us) 前缀,
 * 依据 %Y %w %h %min %s %ms 等 token 输出去年/周/时/分/秒/毫秒 到 CurDate。
 * ================================================================ */
longlong PECMD_FormatTimeString(longlong *param_1, WCHAR *param_2)
{
  bool bVar1;
  bool bVar2;
  bool bVar3;
  char cVar4;
  char cVar5;
  WCHAR WVar6;
  int iVar7;
  undefined8 uVar8;
  ulonglong uVar9;
  LARGE_INTEGER LVar10;
  LARGE_INTEGER *lpFileTime;
  LPCWSTR pwVar11;
  longlong lVar12;
  LPCWSTR pWVar13;
  WCHAR *lpString;
  uint uVar14;
  WCHAR *pWVar15;
  int iVar16;
  char cVar17;
  WCHAR *local_res10[2];
  WCHAR *local_res20;
  SYSTEMTIME local_1918;
  LARGE_INTEGER local_1908;
  LARGE_INTEGER local_1900;
  LPCWSTR local_18f8;
  FILETIME local_18f0;
  LARGE_INTEGER local_18e8[2];
  _TIME_ZONE_INFORMATION local_18d8;
  WCHAR local_1828[3000];
  WCHAR local_b8[60];

  local_res10[0] = param_2;
  PECMD_SkipLeadingControlChars((longlong *)local_res10);
  PECMD_StrDupAssign(&local_18f8, WSTR("CurDate"));
  PECMD_SkipLeadingControlChars((longlong *)local_res10);
  iVar16 = 0;
  memset(&local_1918, 0, sizeof(local_1918));
  local_1900.LowPart = 0;
  local_1828[0] = L'\0';
  local_1908.QuadPart = 0;
  local_1900.HighPart = 0;
  lpString = local_1828;
  cVar5 = '\0';
  bVar3 = false;
  bVar2 = false;
  bVar1 = false;
  cVar4 = '*';
  if (*local_res10[0] == L'-') {
    cVar4 = (char)*local_res10[0];
  }
  uVar14 = (uint)cVar4;
  if (uVar14 == (ushort)*local_res10[0]) {
    do {
      cVar4 = PECMD_MatchAndPad(g_wsz28eb8, local_res10, 2);
      if (cVar4 == '\0') {
        cVar4 = PECMD_MatchAndPad(g_wsz28bf4, local_res10, 2);
        if (cVar4 == '\0') break;
        QueryPerformanceCounter((int64_t *)&g_QPC);
        GetSystemTimeAsFileTime((FILETIME *)&g_StartTime);
      }
      else {
        bVar1 = true;
      }
    } while (uVar14 == (ushort)*local_res10[0]);
    if (bVar1) {
      PECMD_GetTime100ns(&local_1908.QuadPart);
      goto LAB_1400a344b;
    }
  }
  GetSystemTimeAsFileTime((FILETIME *)&local_1908);
LAB_1400a344b:
  WVar6 = *local_res10[0];
  cVar4 = cVar5;
  pWVar13 = local_res10[0];
  while (local_res10[0] = (WCHAR *)pWVar13, uVar14 == (ushort)WVar6) {
    uVar8 = PECMD_AsciiPrefixICmp("utc:", (ushort *)(pWVar13 + 1), 4);
    if ((char)uVar8 == '\0') {
      uVar8 = PECMD_AsciiPrefixICmp("gmt:", (ushort *)(pWVar13 + 1), 4);
      if ((char)uVar8 != '\0') {
        local_res10[0] = (WCHAR *)(pWVar13 + 5);
        uVar9 = PECMD_EvalParenStripped((longlong *)local_res10, (uint64_t *)&local_1908.QuadPart);
        if ((int)uVar9 < 1) goto LAB_1400a34a9;
        local_1908.QuadPart = (local_1908.QuadPart + 0x2b6109100) * 10000000;
        goto LAB_1400a36ff;
      }
      uVar8 = PECMD_AsciiPrefixICmp("local0:", (ushort *)(pWVar13 + 1), 7);
      if ((char)uVar8 == '\0') {
        uVar8 = PECMD_AsciiPrefixICmp("local:", (ushort *)(pWVar13 + 1), 6);
        if ((char)uVar8 != '\0') {
          cVar17 = '\0';
          goto LAB_1400a3551;
        }
        uVar8 = PECMD_AsciiPrefixICmp("sys0:", (ushort *)(pWVar13 + 1), 5);
        if ((char)uVar8 == '\0') {
          uVar8 = PECMD_AsciiPrefixICmp("sys:", (ushort *)(pWVar13 + 1), 4);
          if ((char)uVar8 != '\0') {
            cVar17 = '\0';
            goto LAB_1400a35e0;
          }
          lVar12 = FUN_14005c72c("bsys", (ushort *)(pWVar13 + 1), 4);
          if ((char)lVar12 == '\0') {
            lVar12 = FUN_14005c72c("space0", (ushort *)(pWVar13 + 1), 6);
            if ((char)lVar12 == '\0') {
              lVar12 = FUN_14005c72c("space", (ushort *)(pWVar13 + 1), 5);
              if ((char)lVar12 == '\0') {
                lVar12 = FUN_14005c72c("us", (ushort *)(pWVar13 + 1), 2);
                bVar2 = bVar3;
                if ((char)lVar12 != '\0') {
                  bVar3 = true;
                  bVar2 = true;
                }
              }
              else {
                cVar4 = '\x01';
              }
            }
            else {
              cVar4 = '\x02';
            }
          }
          else {
            cVar5 = '\x01';
          }
        }
        else {
          cVar17 = '\x01';
LAB_1400a35e0:
          local_res10[0] = (WCHAR *)(pWVar13 + (cVar17 + 5));
          uVar9 = PECMD_ParseCommaNumbers(local_res10, &local_1918.wYear, cVar17);
          if ((int)uVar9 < 1) goto LAB_1400a34a9;
          SystemTimeToFileTime(&local_1918, (FILETIME *)&local_1908);
          pWVar13 = local_res10[0];
        }
      }
      else {
        cVar17 = '\x01';
LAB_1400a3551:
        local_res10[0] = (WCHAR *)(pWVar13 + (cVar17 + 7));
        uVar9 = PECMD_ParseCommaNumbers(local_res10, &local_1918.wYear, cVar17);
        if ((int)uVar9 < 1) goto LAB_1400a34a9;
        SystemTimeToFileTime(&local_1918, (FILETIME *)&local_1900);
        LocalFileTimeToFileTime((FILETIME *)&local_1900, (FILETIME *)&local_1908);
        pWVar13 = local_res10[0];
      }
      WVar6 = *pWVar13;
      while ((WVar6 != L'\0' && ((((ushort)WVar6 < 9 || (0xd < (ushort)WVar6)) && (WVar6 != L' '))))
            ) {
        pWVar13 = pWVar13 + 1;
        local_res10[0] = (WCHAR *)pWVar13;
        WVar6 = *pWVar13;
      }
      PECMD_SkipLeadingControlChars((longlong *)local_res10);
    }
    else {
      local_res10[0] = (WCHAR *)(pWVar13 + 5);
      uVar9 = PECMD_EvalParenStripped((longlong *)local_res10, (uint64_t *)&local_1908.QuadPart);
      if ((int)uVar9 < 1) {
LAB_1400a34a9:
        iVar16 = -0x7ff8ffa9;
        goto LAB_1400a3ec4;
      }
    }
LAB_1400a36ff:
    pWVar13 = local_res10[0];
    WVar6 = *local_res10[0];
  }
  WVar6 = *pWVar13;
  if (WVar6 != L'\0') {
    do {
      if (((8 < (ushort)WVar6) && ((ushort)WVar6 < 0xe)) || (WVar6 == L' ')) break;
      local_res10[0] = local_res10[0] + 1;
      WVar6 = *local_res10[0];
    } while (WVar6 != L'\0');
    PECMD_StrCopyW(&local_18f8, pWVar13,
                  ((longlong)local_res10[0] - (longlong)pWVar13) >> 1);
    PECMD_RunCommandLine(param_1, &local_18f8, 0);
  }
  PECMD_SkipLeadingControlChars((longlong *)local_res10);
  pWVar15 = local_res10[0];
  local_res20 = local_res10[0];
  FileTimeToLocalFileTime((FILETIME *)&local_1908, (FILETIME *)&local_1900);
  lpFileTime = &local_1908;
  if (cVar5 == '\0') {
    lpFileTime = &local_1900;
  }
  FileTimeToSystemTime((FILETIME *)lpFileTime, &local_1918);
  if (*pWVar15 != L'\0') goto LAB_1400a3924;
  do {
    local_18d8.Bias = local_18d8.Bias & 0xffff0000;
    if (bVar2) {
      wsprintfW((LPWSTR)&local_18d8, WSTR(".%04lu"),
                (ulonglong)(local_1900.LowPart +
                (int)(local_1900.QuadPart / 10000) * -10000));
    }
    if (cVar4 == '\0') {
      pwVar11 = WSTR("%d-%d-%d|%d|%d:%d:%d.%u%s");
LAB_1400a385b:
      wsprintfW(local_1828, pwVar11, (ulonglong)local_1918.wYear, local_1918.wMonth);
    }
    else {
      if (cVar4 == '\x01') {
        pwVar11 = WSTR("%d %d %d %d %d %d %d %u%s");
        goto LAB_1400a385b;
      }
      wsprintfW(local_1828, WSTR("%04u %02u %02u %02u %02u %02u %03u%s %1u "),
                (ulonglong)local_1918.wYear, local_1918.wMonth);
    }
LAB_1400a3924:
    while ((lpString < local_b8 && (*pWVar15 != L'\0'))) {
      if (lpString != local_1828) {
        *lpString = L' ';
        lpString = lpString + 1;
      }
      cVar5 = PECMD_MatchTokenAdvance("gmt", &local_res20, -1);
      if (cVar5 == '\0') {
        cVar5 = PECMD_MatchTokenAdvance("zone", &local_res20, -1);
        if (cVar5 == '\0') {
          cVar5 = PECMD_MatchTokenAdvance("utc", &local_res20, -1);
          LVar10 = local_1908;
          if (cVar5 == '\0') {
            cVar5 = PECMD_MatchTokenAdvance("uptime", &local_res20, -1);
            if (cVar5 == '\0') {
              cVar5 = PECMD_MatchTokenAdvance("uptimens", &local_res20, -1);
              if (cVar5 == '\0') {
                cVar5 = PECMD_MatchTokenAdvance("Y", &local_res20, -1);
                if ((cVar5 == '\0') &&
                   (cVar5 = FUN_140062fc4(WSTR("年"), &local_res20, -1), cVar5 == '\0')) {
                  cVar5 = PECMD_MatchTokenAdvance("Mon", &local_res20, -1);
                  if ((cVar5 != '\0') ||
                     (cVar5 = FUN_140062fc4(WSTR("月"), &local_res20, -1), cVar5 != '\0')) {
                    uVar9 = local_1918.wMonth;
                    goto LAB_1400a3e8c;
                  }
                  cVar5 = PECMD_MatchTokenAdvance("d", &local_res20, -1);
                  if ((cVar5 != '\0') ||
                     (cVar5 = FUN_140062fc4(WSTR("日"), &local_res20, -1), cVar5 != '\0')) {
                    uVar9 = local_1918.wDay;
                    goto LAB_1400a3e8c;
                  }
                  cVar5 = PECMD_MatchTokenAdvance("w", &local_res20, -1);
                  if ((cVar5 != '\0') ||
                     (cVar5 = FUN_140062fc4(WSTR("星期"), &local_res20, -1), cVar5 != '\0'))
                  {
                    uVar9 = local_1918.wDayOfWeek;
                    goto LAB_1400a3e8c;
                  }
                  cVar5 = PECMD_MatchTokenAdvance("h", &local_res20, -1);
                  if ((cVar5 != '\0') ||
                     (cVar5 = FUN_140062fc4(WSTR("时"), &local_res20, -1), cVar5 != '\0')) {
                    uVar9 = local_1918.wHour;
                    goto LAB_1400a3e8c;
                  }
                  cVar5 = PECMD_MatchTokenAdvance("min", &local_res20, -1);
                  if ((cVar5 != '\0') ||
                     (cVar5 = FUN_140062fc4(WSTR("分"), &local_res20, -1), cVar5 != '\0')) {
                    uVar9 = local_1918.wMinute;
                    goto LAB_1400a3e8c;
                  }
                  cVar5 = PECMD_MatchTokenAdvance("s", &local_res20, -1);
                  if ((cVar5 != '\0') ||
                     (cVar5 = FUN_140062fc4(WSTR("秒"), &local_res20, -1), cVar5 != '\0')) {
                    uVar9 = local_1918.wSecond;
                    goto LAB_1400a3e8c;
                  }
                  cVar5 = PECMD_MatchTokenAdvance("ms", &local_res20, -1);
                  if ((cVar5 != '\0') ||
                     (cVar5 = FUN_140062fc4(WSTR("毫秒"), &local_res20, -1), cVar5 != '\0'))
                  {
                    uVar9 = local_1918.wMilliseconds;
                    goto LAB_1400a3e8c;
                  }
                  cVar5 = PECMD_MatchTokenAdvance("ws", &local_res20, -1);
                  if ((cVar5 == '\0') &&
                     (cVar5 = FUN_140062fc4(WSTR("周"), &local_res20, -1), cVar5 == '\0')) {
                    cVar5 = PECMD_MatchTokenAdvance("ws1", &local_res20, -1);
                    if ((cVar5 != '\0') ||
                       (cVar5 = FUN_140062fc4(WSTR("周1"), &local_res20, -1), cVar5 != '\0'))
                    {
                      iVar7 = 1;
                      goto LAB_1400a3e3e;
                    }
                    cVar5 = PECMD_MatchTokenAdvance("ds", &local_res20, -1);
                    if ((cVar5 == '\0') &&
                       (cVar5 = FUN_140062fc4(WSTR("天"), &local_res20, -1), cVar5 == '\0')) {
                      cVar5 = PECMD_MatchTokenAdvance("Freq", &local_res20, -1);
                      lVar12 = g_QPFreq;
                      if (cVar5 != '\0') goto LAB_1400a3d8f;
                      cVar5 = PECMD_MatchTokenAdvance("Counter", &local_res20, -1);
                      if (cVar5 == '\0') {
                        WVar6 = *local_res20;
                        while ((WVar6 != L'\0' &&
                               ((((ushort)WVar6 < 9 || (0xd < (ushort)WVar6)) && (WVar6 != L' ')))
                               ))
                        {
                          local_res20 = local_res20 + 1;
                          WVar6 = *local_res20;
                        }
                        PECMD_SkipLeadingControlChars((longlong *)&local_res20);
                        goto LAB_1400a3e98;
                      }
                      QueryPerformanceCounter(&local_18e8[0].QuadPart);
                      LVar10 = local_18e8[0];
                      goto LAB_1400a3dce;
                    }
                    uVar14 = PECMD_CalcDayOfYear(&local_1918.wYear);
                  }
                  else {
                    iVar7 = 0;
LAB_1400a3e3e:
                    uVar14 = PECMD_CalcCalendarMonthRows(&local_1918.wYear, iVar7);
                  }
                  uVar9 = (ulonglong)uVar14;
                  pWVar13 = WSTR("%ld");
                }
                else {
                  uVar9 = (ulonglong)local_1918.wYear;
LAB_1400a3e8c:
                  pWVar13 = WSTR("%d");
                }
                wsprintfW(lpString, pWVar13, uVar9);
                goto LAB_1400a3e98;
              }
              LVar10.QuadPart = PECMD_GetTimeNs();
            }
            else {
              LVar10.QuadPart = PECMD_GetTimeMs();
            }
          }
LAB_1400a3dce:
          FUN_1400e6d74(lpString, (uint64_t)LVar10.QuadPart);
        }
        else {
          local_18d8.Bias = 0;
          memset(local_18d8.StandardName, 0, 0xa8);
          GetTimeZoneInformation(&local_18d8);
          iVar7 = (int)((ulonglong)((longlong)local_18d8.Bias * (longlong)0x77777777) >> 0x20) -
                  local_18d8.Bias;
          lVar12 = (longlong)((iVar7 >> 5) - (iVar7 >> 0x1f));
LAB_1400a3d8f:
          FUN_1400e6d68(lpString, (uint64_t)lVar12);
        }
      }
      else {
        lVar12 = (longlong)((__int128)(int64_t)-0x29406b2a1a85bd43LL * (__int128)local_1908.QuadPart) +
                 local_1908.QuadPart;
        FUN_1400e6d68(lpString, (uint64_t)(((lVar12 >> 0x17) + -0x2b6109100) - (lVar12 >> 0x3f)));
        local_18f0.dwLowDateTime = 0;
        local_18f0.dwHighDateTime = 0;
        GetSystemTimeAsFileTime(&local_18f0);
      }
LAB_1400a3e98:
      iVar7 = lstrlenW(lpString);
      lpString = lpString + iVar7;
      pWVar15 = local_res20;
    }
    if (local_1828[0] != L'\0') {
LAB_1400a3ec4:
      PECMD_SetVariable(param_1, local_18f8, local_1828);
      FUN_14005b104((longlong *)&local_18f8);
      return (longlong)iVar16;
    }
  } while(true);
}

/* ================================================================
 * @0x1400a4504  (ulonglong PECMD_LoadTasksWait(longlong * param_1, LPCWSTR param_2,
 *                 longlong param_3))
 * 并发任务装载: 解析 --try/--try星/--exist 选项与逗号分隔的命令清单,
 * 分批创建 (thread/ole/com 任选其一), 等待完成并返回退出码。
 * ================================================================ */
ulonglong PECMD_LoadTasksWait(longlong *param_1, LPCWSTR param_2, longlong param_3)
{
  WCHAR WVar1;
  LPCWSTR lpString1;
  char cVar2;
  int iVar3;
  DWORD DVar4;
  BOOL BVar5;
  LPCWSTR pWVar6;
  uint64_t *puVar7;
  undefined8 uVar8;
  longlong *plVar9;
  ulonglong uVar10;
  ulonglong uVar11;
  WCHAR *pWVar12;
  uint uVar13;
  int iVar14;
  ulonglong uVar15;
  int iVar16;
  longlong lVar17;
  LPCWSTR pWVar18;
  uint uVar19;
  ulonglong uVar20;
  WCHAR *local_res10;
  longlong local_res18;
  int local_res20 [2];
  WCHAR *local_108;
  uint64_t local_100;
  uint local_f8 [2];
  WCHAR *local_f0;
  int local_e8;
  int local_e4;
  WCHAR *local_e0;
  int local_d8 [2];
  longlong local_d0;
  longlong local_c8;
  longlong local_c0;
  WCHAR *local_b8;
  WCHAR *local_b0;
  longlong local_a8;
  WCHAR *local_a0;
  longlong local_98;
  longlong local_90;
  longlong local_88;
  WCHAR *local_80;
  WCHAR *local_78;
  uint64_t *local_70;
  MSG local_68;

  local_res18 = param_3;
  PECMD_AllocStrSlot(&local_e0);
  FUN_140063b64(&local_d0);
  PECMD_StrDupAssign(&local_b8, param_2);
  PECMD_RunCommandLine(param_1, &local_b8, 0);
  local_res10 = local_b8;
  PECMD_SkipLeadingControlChars(&local_res10);
  uVar11 = 0;
  local_100 = 0;
  uVar10 = uVar11;
  uVar20 = uVar11;
  while (((*local_res10 == L'-' && (local_res10[1] == L'-')) &&
         (uVar8 = PECMD_MatchPrefixN((ushort *)&g_wsz28f44, (undefined8 *)&local_res10, 2),
          (int)uVar8 == 0))) {
    cVar2 = FUN_140062fc4(WSTR("--try"), &local_res10, 5);
    if (cVar2 == '\0') {
      cVar2 = FUN_140062fc4(WSTR("--try*"), &local_res10, 6);
      if (cVar2 == '\0') {
        cVar2 = FUN_140062fc4(WSTR("--exist"), &local_res10, 7);
        if (cVar2 == '\0') {
          WVar1 = *local_res10;
          while (((WVar1 != L'\0' && (((ushort)WVar1 < 9 || (0xd < (ushort)WVar1)))) &&
                 (WVar1 != L' '))) {
            local_res10 = local_res10 + 1;
            WVar1 = *local_res10;
          }
          PECMD_SkipLeadingControlChars(&local_res10);
        }
        else {
          uVar10 = 1;
          local_100 = 1;
        }
      }
      else {
        uVar20 = 2;
      }
    }
    else {
      uVar20 = 1;
    }
  }
  pWVar6 = local_res10;
  local_f0 = local_res10;
  PECMD_SkipWCharUntil(&local_f0, 0x2c);
  if (*local_f0 != L'\0') {
    *local_f0 = L'\0';
    local_f0 = local_f0 + 1;
    PECMD_SkipLeadingControlChars(&local_f0);
  }
  PECMD_AssignString(&local_e0, local_f0);
  PECMD_StrDupAssign(&local_b0, pWVar6);
  local_a0 = local_b0;
  PECMD_SkipLeadingControlChars(&local_a0);
  pWVar6 = local_a0;
  if (*local_a0 != L'\0') {
    FUN_140063b64(&local_98);
    lVar17 = local_c0;
    if (*pWVar6 != L'\0') {
      iVar16 = (int)uVar10;
      do {
        local_a8 = 0;
        local_80 = (WCHAR *)pWVar6;
        pWVar6 = (LPCWSTR)PECMD_NextToken(&local_80, &local_a8, 0xcd);
        WVar1 = *local_80;
        pWVar12 = local_80;
        while (WVar1 != L'\0') {
          WVar1 = *pWVar12;
          if (((ushort)WVar1 < 0x61) || (0x7a < (ushort)WVar1)) {
            if (WVar1 == L'\\') {
              *pWVar12 = L'/';
            }
          }
          else {
            *pWVar12 = WVar1 + 0xffc0;
          }
          pWVar12 = pWVar12 + 1;
          lVar17 = local_c0;
          WVar1 = *pWVar12;
        }
        local_108 = local_80;
        local_res20[0] = 0;
        if (*local_80 == L'-') {
          local_108 = local_80 + 1;
          PECMD_ParseHexOrDecBool(&local_108, local_res20);
          if (*local_108 == L':') {
            local_108 = local_108 + 1;
          }
          if (local_res20[0] < 0) {
            local_res20[0] = 0;
          }
        }
        lpString1 = local_108;
        if (*local_108 != L'\0') {
          iVar3 = lstrlenW(local_108);
          local_a8 = (longlong)iVar3;
          uVar10 = uVar11;
          uVar15 = uVar11;
          if (0 < lVar17) {
            do {
              puVar7 = (uint64_t *)PECMD_VectorSlotPtr((int64_t)uVar10, &local_98, &local_90, 8);
              iVar3 = lstrcmpW(lpString1, (LPCWSTR)(uintptr_t)*puVar7);
              if (iVar3 == 0) {
                local_108 = (WCHAR *)0;
                break;
              }
              uVar13 = (int)uVar15 + 1;
              uVar10 = (longlong)(int)uVar13;
              uVar15 = (ulonglong)uVar13;
            } while ((int)uVar13 < lVar17);
          }
          if (local_108 != (WCHAR *)0) {
            if ((iVar16 != 0) && (*local_108 != L'#')) {
              local_108 = local_108 + -1;
              *local_108 = L'#';
            }
            pWVar18 = local_108;
            uVar8 = PECMD_AsciiWideICmp(".thread", (ushort *)local_108);
            if ((char)uVar8 == '\0') {
              uVar8 = PECMD_AsciiWideICmp(".ole", (ushort *)pWVar18);
              if ((char)uVar8 == '\0') {
                uVar8 = PECMD_AsciiWideICmp(".com", (ushort *)pWVar18);
                if (((char)uVar8 == '\0') &&
                   (uVar8 = PECMD_AsciiPrefixICmp(".com*", (ushort *)pWVar18, 5), (char)uVar8 == '\0')) {
                  local_f8[0] = 0xffffffff;
                  puVar7 = PECMD_CreateNamedLock(pWVar18, '\0',
                                         (uint32_t *)(-(ulonglong)((int)local_100 != 0) &
                                                     (ulonglong)local_f8));
                  if (iVar16 != 0) {
                    FUN_1400669c4(param_1, (ulonglong)local_f8[0], local_e0);
                  }
                  if (puVar7 != (uint64_t *)0x0) {
                    *(int *)((longlong)puVar7 + 0x14) = local_res20[0];
                    local_78 = (WCHAR *)lpString1;
                    PECMD_VectorAppendGen(&local_98, &local_90, &local_88, &local_78, 8, 1);
                    local_70 = puVar7;
                    PECMD_VectorAppendGen(&local_d0, &local_c8, &local_c0, &local_70, 8, 1);
                    PECMD_ArrayAppend((longlong)param_1, (longlong)puVar7);
                    lVar17 = local_c0;
                    if (iVar16 != 0) {
                      uVar11 = (ulonglong)(int)local_f8[0];
                      goto LAB_1400a4b62;
                    }
                  }
                }
                else {
                  local_108 = (WCHAR *)(pWVar18 + 4);
                  local_e8 = 0;
                  local_e4 = 3;
                  iVar3 = -1000;
                  iVar14 = 3;
                  if (*local_108 != L'\0') {
                    local_108 = (WCHAR *)(pWVar18 + 5);
                    FUN_140067d20(&local_108, &local_e8);
                    if (*local_108 != L'\0') {
                      local_108 = local_108 + 1;
                      local_d8[0] = 0;
                      PECMD_ParseNumSkipChar_01f8(&local_108, local_d8);
                      PECMD_ParseNumSkipChar_01f8(&local_108, &local_e4);
                      iVar3 = local_d8[0];
                      iVar14 = local_e4;
                    }
                  }
                  PECMD_DelayLoadOleaut32();
                  FUN_140061c44();
                  if ((((*(byte *)((longlong)param_1 + 0x12) & 4) == 0) &&
                      (*(byte *)((longlong)param_1 + 0x12) =
                          *(byte *)((longlong)param_1 + 0x12) | 4,
                       g_pUiCallback != (code *)0x0)) && ((*g_pUiCallback)(0, local_e8), -1 < iVar3)
                     ) {
                    (*g_pOle32Slot828)(0, 0xffffffff, 0, 0, iVar3, iVar14, 0, 0, 0);
                  }
                }
              }
              else {
                PECMD_DelayLoadOleaut32();
                FUN_140061c44();
                if ((*(byte *)((longlong)param_1 + 0x12) & 2) == 0) {
                  EnterCriticalSection((LPCRITICAL_SECTION)&g_csCom);
                  *(byte *)((longlong)param_1 + 0x12) = *(byte *)((longlong)param_1 + 0x12) | 2;
                  if (g_pOleInit != (code *)0x0) {
                    (*g_pOleInit)(0);
                  }
                }
              }
            }
            else {
              PECMD_DelayLoadOleaut32();
              FUN_140061c44();
              if ((*(byte *)((longlong)param_1 + 0x12) & 1) == 0) {
                *(byte *)((longlong)param_1 + 0x12) = *(byte *)((longlong)param_1 + 0x12) | 1;
                EnterCriticalSection((LPCRITICAL_SECTION)&g_csInit);
              }
            }
          }
        }
        param_3 = local_res18;
      } while (*pWVar6 != L'\0');
    }
    if (0 < lVar17) {
      FUN_1400633a8((void **)&local_100, lVar17 * 8 + 8);
      uVar13 = (uint)lVar17;
      if (0 < lVar17) {
        puVar7 = (uint64_t *)(uintptr_t)local_100;
        uVar10 = uVar11;
        uVar15 = uVar11;
        do {
          plVar9 = (longlong *)PECMD_VectorSlotPtr((int64_t)uVar10, &local_d0, &local_c8, 8);
          uVar19 = (int)uVar15 + 1;
          uVar15 = (ulonglong)uVar19;
          *puVar7 = *(uint64_t *)(*plVar9 + 8);
          uVar10 = (ulonglong)(int)uVar19;
          puVar7 = puVar7 + 1;
          lVar17 = local_c0;
        } while ((longlong)uVar10 < local_c0);
      }
      uVar10 = uVar11;
      if ((int)uVar20 != 2) {
        uVar10 = 1;
        if ((int)uVar20 == 0) {
LAB_1400a4cee:
          if ('\0' < g_flagA24F) {
            if (param_3 == 0) {
              uVar19 = WaitForMultipleObjects(uVar13, (const HANDLE *)(uintptr_t)local_100,
                                              (uint)(uVar13 != 1), 100);
            }
            else {
              FUN_14001bbac(param_1, (int)uVar13,
                            (HANDLE *)(uintptr_t)local_100,
                            (uint)(uVar13 != 1), 1000, 0x4ff);
              uVar19 = 0;
            }
            lVar17 = local_c0;
            if (uVar19 < uVar13) goto LAB_1400a4d06;
            if (uVar19 == 0xffffffff) {
              Sleep(1);
            }
            iVar16 = 1999;
            do {
              if ((g_flagA24F < '\x01') ||
                 (BVar5 = PeekMessageW(&local_68, (HWND)0, 0, 0, 1), BVar5 == 0)) break;
              TranslateMessage(&local_68);
              DispatchMessageW(&local_68);
              iVar16 = iVar16 + -1;
            } while (-1 < iVar16);
            goto LAB_1400a4cee;
          }
        }
        else {
          DVar4 = WaitForMultipleObjects(uVar13, (const HANDLE *)(uintptr_t)local_100, 1, 0);
          if (DVar4 < uVar13) {
LAB_1400a4d06:
            uVar10 = 0;
            uVar20 = uVar11;
            if (0 < lVar17) {
              do {
                plVar9 = (longlong *)PECMD_VectorSlotPtr(uVar11, &local_d0, &local_c8, 8);
                uVar13 = (int)uVar20 + 1;
                uVar11 = (ulonglong)(int)uVar13;
                *(undefined1 *)(*plVar9 + 0x10) = 1;
                uVar20 = (ulonglong)uVar13;
              } while ((longlong)uVar11 < local_c0);
            }
          }
        }
      }
      if (*local_e0 != L'\0') {
        FUN_1400669c4(param_1, uVar10, local_e0);
      }
      PECMD_NotifyMainWindowRefresh(param_1, 0);
      FUN_14005b104(&local_100);
      FUN_14005b104(&local_98);
      FUN_14005b104(&local_b0);
      FUN_14005b104(&local_b8);
      FUN_14005b104(&local_d0);
      FUN_14005b104(&local_e0);
      return (longlong)(int)uVar10;
    }
LAB_1400a4b62:
    FUN_14005b104(&local_98);
  }
  FUN_14005b104(&local_b0);
  FUN_14005b104(&local_b8);
  FUN_14005b104(&local_d0);
  FUN_14005b104(&local_e0);
  return uVar11;
}
