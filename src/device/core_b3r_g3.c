/*
 * core_b3r_g3.c — 还原 batch28 的 4 个业务函数
 *
 *   PECMD_ReadFileToWideString @0x1400685f4  读取文本文件并识别 BOM/转换编码 (UTF-16LE/BE, UTF-8)
 *   PECMD_GetPathSizeToVar @0x14006b414  计算路径/目录大小并写回变量
 *   PECMD_DispatchListboxCommand @0x14006b9f0  列表控件 (ListBox) 命令分派 (Enable/颜色等)
 *   PECMD_DispatchListWriteCommand @0x14006fef0  列表/输出命令分派 (Write 类)
 *
 * 未定义任何 helper 体 (由 link_stubs.c / 相应 core 文件提供);
 * 只实现本文件列出的目标函数。
 */

#include <string.h>
#include <stdbool.h>

#include "win32_stub.h"
#include "pecmd_defs.h"

/* R24f-d: C4013 隐式 int 防护 — 指针返回型 extern 补齐 */
extern short *PECMD_UnquoteString(short *); /* @0x1400xxxx */
/* R24e(051 SIZE AV 定案): S11 曾删本地声明"统一 xproto"但本文件不包含 xproto →
 * C4013 隐式 int → SplitNextToken 返回截断 (0x17096823d0c→0xffffffff96823d0c) → AV。
 * 按 core_b3i.c:216 定义签名补齐。 */
extern int16_t *PECMD_SplitNextToken(int64_t *script, int64_t *pp, int64_t *out, int16_t sep1,
                                     int16_t sep2); /* @0x1400547bc core_b3i.c */

/* ---- 本文件使用的 BOM 常量 (对应 .rdata 中的 DAT_140124128/12c/130) ---- */
static const uint8_t DAT_140124128[] = {0xFF, 0xFE};       /* UTF-16LE BOM */
static const uint8_t DAT_14012412c[] = {0xFE, 0xFF};       /* UTF-16BE BOM */
static const uint8_t DAT_140124130[] = {0xEF, 0xBB, 0xBF}; /* UTF-8 BOM */

/* ---- 本文件引用但未声明的辅助函数 (均为 extern, 不在本文件定义) ---- */

/* 文件大小 @0x1400e693c GetFileSize */
extern uint32_t PECMD_GetFileSize(HANDLE hFile);
/* 分配字符串槽 @0x140063720 PECMD_StrAlloc */
extern WCHAR *PECMD_AllocString(WCHAR **ps, int64_t count);
/* 跳过前导空白 @0x14005b154 */
extern int64_t *PECMD_SkipLeadingControlChars(int64_t *pp);
/* 释放字符串槽 @0x14005b104 */
extern void PECMD_FreeStrBuf(void *ps);
/* 匹配/比较字节串 @0x14005b184 */
extern int PECMD_AnsiStrNCompare(char *buf, int64_t a, int64_t b);
/* memmove 类 @0x14001d744 */
/* 转换/规范化小写 @0x140060a74 */
extern void PECMD_SwapBytePairs(uint8_t *param_1, int param_2);
/* 编码转换函数指针 (DAT_14013c970 == g_pConvFunc, MultiByteToWideChar 类) */
extern int (*DAT_14013c970)(uint32_t, uint32_t, char *, int, char *, int);

/* ---- PECMD_GetPathSizeToVar 的 helper ---- */
extern uint8_t PECMD_MatchTokPrefixAdv(LPCWSTR param_1, int64_t *param_2,
                                       int param_3); /* "-link" 等前缀比较 */
/* S11: 本地声明与定义冲突已删除, 统一采用 xproto.h 原型 (原: extern void PECMD_AllocStrSlot(WCHAR **out); /* @0x140063620 串容器初始化 * / extern WCHAR *PECMD_SplitNextToken(int6) */







extern int64_t PECMD_GetDirectorySize(LPCWSTR param_1); /* @0x14006b254 递归统计目录大小 */
extern void PECMD_FormatU64Dec(WCHAR *dst, uint64_t v); /* %I64u 格式化 */
extern void PECMD_SetVariable(int64_t *script, LPCWSTR key,
                              LPCWSTR value); /* @0x1400629b8 SetVar */

/* ---- PECMD_DispatchListboxCommand / PECMD_DispatchListWriteCommand 的 helper ---- */
extern int PECMD_OnDeleteCommand(uint64_t *param_1, LPCWSTR param_2,
                                 HWND param_3); /* 列表接受检查 */
extern int PECMD_DispatchControlCommand(WPARAM a, LPCWSTR b, WPARAM c, HWND d, LPCWSTR e,
                                        uint64_t f, int64_t *g, HWND h, int64_t i);
extern void PECMD_CopyTokenTrimmed(int64_t *pp, int64_t *out, int16_t c1,
                                   int16_t c2);                          /* 按分隔符切分 */
extern void PECMD_VarSetUInt(void *script, uint64_t value, LPCWSTR key); /* @0x140066978 SetVarD */
extern uint64_t PECMD_IsVisibleKeyword(uint16_t *param_1);               /* 前缀字符检查 */

/* ==========================================================================
 * @0x1400685f4  读取文件(字节)并统一编码为宽字符串写回 param_2
 */
uint64_t PECMD_ReadFileToWideString(HANDLE param_1, int64_t *param_2, uint32_t param_3)
{
    char *lpBuffer;
    DWORD DVar1;
    BOOL BVar2;
    int iVar3;
    uint32_t uVar4;
    uint32_t uVar5;
    uint64_t uVar6;
    uint64_t uVar7;
    int iVar8;
    bool bVar9;
    DWORD local_res18[2];
    int64_t local_res20;

    uVar4 = param_3 & 1;
    DVar1 = PECMD_GetFileSize(param_1);
    if (0 < (int)DVar1) {
        PECMD_AllocString((WCHAR **)param_2, (int64_t)(int)(DVar1 + 0xc));
        lpBuffer = (char *)*param_2;
        local_res18[0] = 0xffffffffu;
        BVar2 = ReadFile(param_1, lpBuffer, DVar1, local_res18, (void *)0);
        DVar1 = local_res18[0];
        uVar6 = (uint64_t)(int)local_res18[0];
        if ((0 < (int)local_res18[0]) && (BVar2 != 0)) {
            memset(lpBuffer + uVar6, 0, 10); /* FUN_140102a90 */
            iVar8 = DVar1 + 10;
            while (1) {
                iVar3 = PECMD_AnsiStrNCompare(lpBuffer, (int64_t)(uintptr_t)DAT_14012412c, 2);
                if (iVar3 == 0) {
                    PECMD_SwapBytePairs((uint8_t *)lpBuffer, (int)uVar6);
                }
                iVar3 = PECMD_AnsiStrNCompare(lpBuffer, (int64_t)(uintptr_t)DAT_140124128, 2);
                bVar9 = iVar3 == 0;
                if ((*lpBuffer != '\0') && (lpBuffer[1] == '\0')) {
                    memmove(lpBuffer + 2, (void *)lpBuffer, iVar8);
                    uVar6 = (uint64_t)((int)uVar6 + 2);
                    iVar8 = iVar8 + 2;
                    bVar9 = true;
                    lpBuffer[0] = (char)-1;
                    lpBuffer[1] = (char)-2;
                }
                iVar3 = (int)uVar6;
                if (bVar9)
                    break;
                if ((*lpBuffer != '\0') || (lpBuffer[1] == '\0')) {
                    local_res20 = *param_2;
                    *param_2 = 0;
                    PECMD_AllocString((WCHAR **)param_2, (int64_t)iVar3);
                    uVar7 = 3;
                    *(uint16_t *)(uintptr_t)*param_2 = *(uint16_t *)DAT_140124128;
                    iVar8 = PECMD_AnsiStrNCompare(lpBuffer, (int64_t)(uintptr_t)DAT_140124130, 3);
                    if ((iVar8 != 0) && (uVar5 = 0, uVar7 = 0, uVar4 == 0)) {
                        goto LAB_1400687a7;
                    }
                    do {
                        iVar8 =
                            (*DAT_14013c970)(0xfde9, 8, lpBuffer + (int)uVar7, iVar3 - (int)uVar7,
                                             (char *)((uintptr_t)*param_2 + 2), iVar3 + 1);
                        if ((uVar4 == 0) || (0 < iVar8)) {
                            PECMD_FreeStrBuf(&local_res20);
                            if (iVar8 < 1) {
                                return 0;
                            }
                            return (uint64_t)(iVar8 + 1);
                        }
                        uVar5 = uVar4 | 2;
                    LAB_1400687a7:
                        iVar8 = (*DAT_14013c970)(0, 8, lpBuffer, (uint32_t)uVar6,
                                                 (char *)((uintptr_t)*param_2 + 2), iVar3 + 1);
                    } while ((iVar8 < 1) && (uVar4 = 0, uVar7 = 0, uVar5 == 0));
                    PECMD_FreeStrBuf(&local_res20);
                    if (iVar8 < 1) {
                        return 0;
                    }
                    return (uint64_t)(iVar8 + 1);
                }
                memmove(lpBuffer + 2, (void *)lpBuffer, iVar8);
                uVar6 = (uint64_t)(iVar3 + 2);
                lpBuffer[0] = (char)-2;
                lpBuffer[1] = (char)-1;
                iVar8 = iVar8 + 2;
            }
            return (uint64_t)(int64_t)iVar3 >> 1;
        }
    }
    return 0;
}

/* ==========================================================================
 * @0x14006b414  计算路径/目录大小并写回变量 (支持 -link 链接/符号重解析)
 */
uint64_t PECMD_GetPathSizeToVar(int64_t *param_1, LARGE_INTEGER param_2)
{
    HANDLE hObject;
    LARGE_INTEGER LVar1;
    char cVar2;
    int iVar3;
    LARGE_INTEGER LVar4;
    LPCWSTR lpString;
    LARGE_INTEGER local_res10[2];
    LPCWSTR local_res20;
    HANDLE local_368;
    LARGE_INTEGER local_360;
    HANDLE local_358[2];
    WCHAR local_348[104];
    WIN32_FIND_DATAW local_278;

    local_res10[0] = param_2;
    PECMD_SkipLeadingControlChars((int64_t *)&local_res10[0].QuadPart);
    LVar4.QuadPart = 0;
    cVar2 = (char)PECMD_MatchTokPrefixAdv(WSTR("-link"), (int64_t *)&local_res10[0].QuadPart, 5);
    PECMD_AllocStrSlot((WCHAR **)&local_res20);
    lpString = WSTR(".");
    if ((*(int16_t *)(uintptr_t)local_res10[0].QuadPart == 0x3d) ||
        (local_res10[0].QuadPart = (int64_t)(uintptr_t)PECMD_SplitNextToken(
             (int64_t *)param_1, (int64_t *)&local_res10[0].QuadPart, (int64_t *)&local_res20, 0x3d,
             0),
         *(int16_t *)(uintptr_t)local_res10[0].QuadPart == 0x3d)) {
        local_res10[0].QuadPart = local_res10[0].QuadPart + 2;
        lpString = PECMD_UnquoteString((WCHAR *)(uintptr_t)local_res10[0].QuadPart);
    }
    if (*local_res20 == L'\0')
        goto LAB_14006b58f;
    iVar3 = lstrlenW(lpString);
    local_368 = (HANDLE)0;
    local_278.dwFileAttributes = 0;
    memset((uint64_t *)&local_278.ftCreationTime, 0, 0x24c);
    if ((iVar3 < 1) || ((LVar1 = local_res10[0],
                         (lpString[iVar3 - 1] != L'\\') && (lpString[iVar3 - 1] != L'/')))) {
        PECMD_FindFirstFileW((HANDLE *)&local_368, lpString, &local_278);
        if (local_368 != (HANDLE)0) {
            LVar1.QuadPart = LVar4.QuadPart;
            if ((local_278.dwFileAttributes & 0x10) != 0)
                goto LAB_14006b539;
            if (((local_278.dwFileAttributes >> 10 & 1) == 0) || (cVar2 == '\0')) {
                LVar4.HighPart = (LONG)local_278.nFileSizeHigh;
                LVar4.LowPart = local_278.nFileSizeLow;
            }
            else {
                local_358[0] = (HANDLE)0;
                PECMD_OpenFileHandle(local_358, lpString, 0, 7, (LPSECURITY_ATTRIBUTES)0, 3, 0x80,
                                     (HANDLE)0);
                hObject = local_358[0];
                if (local_358[0] == (HANDLE)0)
                    goto LAB_14006b539;
                local_360.QuadPart = 0;
                GetFileSizeEx(local_358[0], &local_360);
                LVar4 = local_360;
                if (hObject != (HANDLE)(uintptr_t)-1) {
                    CloseHandle(hObject);
                }
            }
        }
    }
    else {
    LAB_14006b539:
        LVar4 = LVar1;
        if (iVar3 < 0x20e) {
            LVar4.QuadPart = PECMD_GetDirectorySize(lpString);
        }
    }
    local_348[0] = L'\0';
    PECMD_FormatU64Dec(local_348, (uint64_t)LVar4.QuadPart);
    PECMD_SetVariable(param_1, local_res20, local_348);
    if ((local_368 != (HANDLE)0) && (local_368 != (HANDLE)(uintptr_t)-1)) {
        FindClose(local_368);
    }
    local_368 = (HANDLE)0;
LAB_14006b58f:
    PECMD_FreeStrBuf((WCHAR **)&local_res20);
    return 0;
}

/* ==========================================================================
 * @0x14006b9f0  列表控件命令分派 (Enable / 颜色等)
 * 说明: CONCAT71(extraout_var,bVar6) 为 Ghidra 残留, 归一化为 bVar6 判断。
 */
uint64_t PECMD_DispatchListboxCommand(uint64_t *param_1, int64_t *param_2, int64_t **param_3,
                                      LPCWSTR param_4, WPARAM param_5, uint64_t *param_6)
{
    WCHAR WVar1;
    int16_t sVar2;
    uint16_t uVar3;
    int64_t lVar4;
    HWND hWnd;
    HWND pHVar5;
    void *lpCriticalSection;
    bool bVar6;
    int iVar7;
    uint64_t uVar8;
    UINT Msg;
    uint64_t *wParam;
    int64_t **lParam;
    HWND local_res8;
    int64_t local_68;
    int64_t *local_60;
    int64_t **local_58;
    LPCWSTR local_50;
    WPARAM local_48;
    HWND local_40;
    uint64_t local_38;
    uint64_t *local_30;

    /* TODO(verify): param_1 + 0x13 保持反编译原始指针运算 (uint64_t* 下标) */
    lVar4 = param_1[7];
    hWnd = *(HWND *)(uintptr_t)(lVar4 + 0x20);
    pHVar5 = *(HWND *)(uintptr_t)(*(int64_t *)(uintptr_t)(param_1[10] + 0x40) + 0x20);
    local_res8 = pHVar5;
    bVar6 = (PECMD_OnDeleteCommand(param_1, param_4, pHVar5) != 0);
    if (bVar6 == 0) { /* CONCAT71(extraout_var,bVar6) == 0 */
        return 0;
    }
    lpCriticalSection = (void *)*param_6;
    if (lpCriticalSection != (void *)0) {
        *param_6 = 0;
        LeaveCriticalSection(lpCriticalSection);
    }
    WVar1 = *param_4;
    if ((*(char *)(param_1 + 0x13) != '\0') &&
        (iVar7 = lstrcmpiW(param_4, WSTR("Enable")), iVar7 == 0)) {
        sVar2 = *(int16_t *)(uintptr_t)param_3;
        SendMessageW(hWnd, 0x462, (WPARAM)param_3, (LPARAM)param_1[7]);
        if (sVar2 != 0x3f) {
            return 0;
        }
        uVar3 = *(uint16_t *)(uintptr_t)param_3;
        PECMD_AllocStrSlot((WCHAR **)&local_res8);
        local_68 = (int64_t)param_3 + 2;
        PECMD_CopyTokenTrimmed(&local_68, (int64_t *)&local_res8, 0x3b, 0x3a);
        PECMD_VarSetUInt(param_2, (uint64_t)uVar3, (LPCWSTR)local_res8);
        PECMD_FreeStrBuf((WCHAR **)&local_res8);
        return 0;
    }
    uVar8 = (uint64_t)PECMD_DispatchControlCommand((WPARAM)param_1[7], param_4, param_5, hWnd,
                                                   (LPCWSTR)param_3, param_1[10], param_2, pHVar5,
                                                   (int64_t)param_1);
    if ((int)uVar8 != 0) {
        return 0;
    }
    if (*(char *)(param_1 + 0x13) == '\0') {
        if (WVar1 == L'\0') {
            if (*(int64_t *)(uintptr_t)(lVar4 + 0x110) == 0) {
                Msg = 0x45a;
                wParam = param_1;
                lParam = param_3;
                goto LAB_14006bbcd;
            }
        }
        else {
        LAB_14006bb78:
            iVar7 = lstrcmpW(WSTR("color"), param_4);
            if (iVar7 != 0) {
                return 0;
            }
        }
    }
    else if (WVar1 != L'\0')
        goto LAB_14006bb78;
    wParam = (uint64_t *)param_1[7];
    local_40 = local_res8;
    local_48 = param_5;
    local_38 = 0;
    lParam = &local_60;
    Msg = 0x459;
    local_60 = param_2;
    local_58 = param_3;
    local_50 = param_4;
    local_30 = param_1;
    (void)local_30;
    (void)local_38;
    (void)local_40;
    (void)local_48;
    (void)local_50;
    (void)local_58;
LAB_14006bbcd:
    SendMessageW(hWnd, Msg, (WPARAM)wParam, (LPARAM)lParam);
    return 0;
}

/* ==========================================================================
 * @0x14006fef0  列表/输出命令分派 (Write 类)
 * 说明: CONCAT71(extraout_var,bVar3) 为 Ghidra 残留, 归一化为 bVar3 判断。
 */
uint64_t PECMD_DispatchListWriteCommand(uint64_t *param_1, int64_t *param_2, LPCWSTR param_3,
                                        LPCWSTR param_4, WPARAM param_5, uint64_t *param_6)
{
    HWND pHVar1;
    HWND pHVar2;
    void *lpCriticalSection;
    bool bVar3;
    int iVar4;
    uint64_t uVar5;
    LPCWSTR lpString1;

    if (param_4 == (LPCWSTR)0) {
        return 0;
    }
    /* TODO(verify): param_1 + 0x10 保持反编译原始指针运算 (uint64_t* 下标) */
    pHVar1 = *(HWND *)(uintptr_t)(param_1[7] + 0x20);
    pHVar2 = *(HWND *)(uintptr_t)(*(int64_t *)(uintptr_t)(param_1[10] + 0x40) + 0x20);
    if ((*(char *)(param_1 + 0x10) == '\0') &&
        (bVar3 = (PECMD_OnDeleteCommand(param_1, param_4, pHVar2) != 0), bVar3 == 0)) {
        return 0;
    }
    lpCriticalSection = (void *)*param_6;
    if (lpCriticalSection != (void *)0) {
        *param_6 = 0;
        LeaveCriticalSection(lpCriticalSection);
    }
    if (*(char *)(param_1 + 0x10) != '\0') {
        lpString1 = param_4;
        uVar5 = PECMD_IsVisibleKeyword((uint16_t *)param_4);
        if ((char)uVar5 != '\0') {
            return 0;
        }
        iVar4 = lstrcmpiW(lpString1, WSTR("Pos"));
        if (iVar4 == 0) {
            return 0;
        }
    }
    PECMD_DispatchControlCommand((WPARAM)param_1[7], param_4, param_5, pHVar1, param_3, param_1[10],
                                 param_2, pHVar2, (int64_t)param_1);
    return 0;
}
