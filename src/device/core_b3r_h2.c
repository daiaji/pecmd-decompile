/*
 * core_b3r_h2.c — Restored batch28 business functions (group h2)
 *
 *   PECMD_VolumeDeviceCommand @0x140095178  卷/设备信息命令 (Volume Label / DosDevice / FS info)
 *   PECMD_ParseExtPathArg @0x140096880  -ext 扩展名辅助
 *   PECMD_LinkFile @0x1400969d4  硬链接/符号链接/文件操作 (-j)
 *   PECMD_ScreenAreaCommand @0x140097c70  屏幕/窗口区域命令 (display/capture/taskbar)
 *   PECMD_WlanManage @0x140099f18  WLAN 管理命令 (-wlan)
 *   PECMD_TempPathCommand @0x14009bbbc  临时目录/Delete/Setting 命令
 *   PECMD_ControlTimer @0x14009c548  定时器控制 (-del)
 *
 * 约定:
 *   - memset(...) 是 memset 别名, 直接写 memset。
 *   - 宽字符串字面量用 WSTR("..") (赋值给 LPCWSTR/WCHAR*); 单宽字符比较用 L'x'。
 *   - CONCAT44/CONCAT22/CONCAT71 拼接以位运算再现 (Ghidra register glue)。
 *   - 带 `._4_4_` / `._1_7_` 的高低位拼接按 64 位整数就地还原 (位运算)。
 *   - 辅助函数 / DAT 全局仅 extern; 不在本文件定义。
 *   - 底层 wlanapi/VolumeInfo/Path 函数指针槽 (DAT_14013d7xx 等) 为惰性加载槽。
 *
 * 待核实项见于各函数内 TODO(verify) 注释与文件末尾汇总。
 */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>

#include "win32_stub.h"
#include "pecmd_defs.h"

/* ---- Ghidra primitive aliases ---- */
typedef uint32_t uint;
typedef uint16_t ushort;
typedef uint8_t byte;
typedef uint8_t undefined1;
typedef uint16_t undefined2;
typedef uint32_t undefined4;
typedef uint64_t undefined8;
typedef void undefined;
typedef int64_t longlong;
typedef uint64_t ulonglong;

/* ---- CONCAT splice macros (Ghidra register glue) ---- */
#define CONCAT44(hi, lo)   ((((ulonglong)(uint32_t)(hi)) << 32) | (uint32_t)(lo))
#define CONCAT71(hi7, lo1) ((((ulonglong)(hi7)) << 8) | (uint8_t)(lo1))

/* ---- stub 缺失补充类型 ---- */
typedef DWORD *LPDWORD;
typedef LONG *LSTATUS;
typedef void *TIMERPROC;
typedef uint64_t pthreadmbcinfo;
typedef struct _APPBARDATA {
    DWORD cbSize;
    HWND hWnd;
    UINT uCallbackMessage;
    UINT uEdge;
    RECT rc;
    int lParam;
} APPBARDATA;
#define ABM_GETTASKBARPOS 5

/* ---- 本文件引用的全局 DATA ---- */
extern DWORD g_imgBufLen;        /* 位图数据大小           */
extern WCHAR g_szEmpty[];        /* "" (空串)              */
extern const WCHAR g_wsz28038[]; /* 关键字串 .rdata         */
extern const WCHAR g_wsz27fc8[]; /* 关键字串 .rdata         */
extern const WCHAR g_wsz27fd0[]; /* 关键字串 .rdata         */

/* ---- 惰性加载函数指针槽 (DAT_14013xxx) ---- */
extern DWORD (*g_pGetVolumeInformationByHandleW)(HANDLE, LPWSTR, DWORD,
                                                 LPDWORD); /* GetVolumeInformationByHandleW */
extern DWORD (*g_pGetFinalPathNameByHandleW)(HANDLE, LPWSTR, DWORD,
                                             DWORD);            /* GetFinalPathNameByHandleW    */
extern DWORD (*g_pWlanOpenHandle)(DWORD, void *, DWORD *);      /* WlanOpenHandle               */
extern DWORD (*g_pWlanEnumInterfaces)(HANDLE, void *, void **); /* WlanEnumInterfaces           */
extern DWORD (*g_pWlanSetProfile)(HANDLE, LPCWSTR, LPCWSTR, LPCWSTR); /* WlanSetProfile */
extern DWORD (*g_pWlanCloseHandle)(HANDLE, void *); /* WlanCloseHandle              */
extern DWORD (*g_pWlanRegisterNotification)(void);  /* WlanRegisterNotification     */
extern DWORD (*g_pWlanScan)(HANDLE, LPCWSTR);       /* WlanScan                     */
extern DWORD (*g_pWlanGetAvailableNetworkList)(HANDLE, LPCWSTR, LPCWSTR,
                                               void *);          /* WlanGetAvailableNetworkList  */
extern DWORD (*g_pWlanDisconnect)(HANDLE, LPCWSTR, LPCWSTR);     /* WlanDisconnect               */
extern void (*g_pWlanFreeMemory)(void);                          /* WlanFreeMemory               */
extern DWORD (*g_pWlanConnect)(HANDLE, LPCWSTR, void *, void *); /* WlanConnect                  */

/* ---- 本文件引用的辅助函数 (仅 extern) ---- */
extern void PECMD_FreeStrBuf(void *ps);                  /* @0x14005b104 释放串槽     */
extern void PECMD_FreeContainer(void *ps);               /* @0x14005b134 释放串资源   */
extern void PECMD_ZeroLenBuf(void *p);                   /* @0x14005b0b8 对象初始化   */
/* S11: 本地声明与定义冲突已删除, 统一采用 xproto.h 原型 (原: extern WCHAR *PECMD_SkipLeadingControlChars(WCHAR **pp); /* @0x14005b154 跳过空白 * /) */
extern undefined8 PECMD_MatchPrefixN(ushort *a, undefined8 *b, int c); /* @0x14005b1a8 */
extern undefined8 PECMD_OnDeleteCommand(void *a, LPCWSTR b, HWND c); /* @0x14005b2c0              */
extern LPCWSTR PECMD_LoadLocalizedString(HINSTANCE, UINT, LPWSTR,
                                         int); /* @0x14005b6ac 资源字符串   */
extern int PECMD_AsciiPrefixICmp(const char *a, const WCHAR *w, int n); /* @0x14005c788 前缀比较 */
extern int32_t PECMD_AsciiWideICmp(const char *a, const WCHAR *w); /* @0x14005c7c4 后缀比较     */
extern void PECMD_GetApiProcCached(const char *func, const char *dll, void **out,
                                   uintptr_t *hmod); /* @0x14005c828 延迟加载     */
extern LARGE_INTEGER PECMD_SetFilePointer(HANDLE h, LARGE_INTEGER dist,
                                          DWORD method); /* @0x14005c674 */
extern DWORD PECMD_RegSetValueWithOpen(HKEY root, LPCWSTR sub, LPCWSTR name, DWORD type, BYTE *data,
                                       DWORD size); /* @0x14005c5a0 RegSetValue  */
extern void PECMD_OpenFileHandle(void *pH, LPCWSTR path, DWORD access, DWORD share, void *sa,
                                 DWORD disp, DWORD flags, HANDLE tmpl); /* CreateFile 包装 */
extern void PECMD_RunCommandLine(void *script, WCHAR **str, int mode);  /* @0x140003a20  */
extern WCHAR *PECMD_EnumerateVolume(longlong *a, longlong b, ulonglong c,
                                    LPCWSTR d);                                   /* @0x140006aa0 */
/* S11: 本地声明与定义冲突已删除, 统一采用 xproto.h 原型 (原: extern void PECMD_CreateTempMutexDir(void *a, void *b, undefined8 *c, LPCWSTR d); /* @0x140008110 * / extern vo) */











extern undefined8 PECMD_MountVolumeToDir(LPCWSTR a, LPCWSTR b); /* @0x1400604b0             */
extern undefined8 PECMD_CreateSymbolicLink(undefined8 a, undefined8 b,
                                           undefined4 c);            /* @0x140060600    */
extern short *PECMD_LastPathSeparator(short *s);                     /* @0x14006045c             */
extern undefined8 FUN_140063060(undefined8 a);                       /* @0x140063060             */
extern undefined8 *PECMD_AllocSmallObject(undefined8 *a);            /* @0x140063344 小对象分配  */
/* S11: 本地声明与定义冲突已删除, 统一采用 xproto.h 原型 (原: extern void PECMD_AllocStrSlot(void *out); /* @0x140063620 串容器初始化 * /) */
extern void PECMD_AllocWStringBuffer(WCHAR **ps, int64_t count);     /* @0x140063694 串分配      */
extern WCHAR *PECMD_AllocString(WCHAR **ps, longlong count);         /* @0x140063720 串扩容      */
/* S11: 本地声明与定义冲突已删除, 统一采用 xproto.h 原型 (原: extern WCHAR *PECMD_AppendWideStr(WCHAR **ps, LPCWSTR src); /* @0x14006375c 串追加 * /) */
extern WCHAR *PECMD_StrCopyW(WCHAR **ps, LPCWSTR src, longlong len); /* @0x140063888       */
extern char PECMD_MatchTokenAdvance(const char *tok, WCHAR **pp, int n); /* @0x1400660ac 词比较 */
extern void PECMD_AppendLongDecimal(void *script, int64_t value,
                                    LPCWSTR key); /* @0x1400669c4 SetVarD   */
extern void PECMD_FormatSetVar(longlong *a, undefined8 b, LPCWSTR c, LPCWSTR d); /* @0x140066930 */
extern void PECMD_VarSetUInt(longlong *a, undefined8 b, LPCWSTR c); /* @0x140066978         */
extern void PECMD_SplitTokenTrimWs(longlong *src, longlong *dst,
                                   short delim);                   /* @0x1400675b8 切分 */
extern void PECMD_ParseShortStore(undefined8 *a, int *b, short c); /* @0x1400679b0             */
extern void PECMD_RegReadWrap(void *a, LPCWSTR b, LPCWSTR c, longlong *d, void *e,
                              void *f);                       /* @0x140069218 RegQuery   */
extern undefined8 PECMD_StripDevicePrefix(longlong a);        /* @0x14006acb8             */
extern void PECMD_HexStringToBytes(byte *a, byte *b);         /* @0x14006ad9c             */
extern undefined8 PECMD_Base64Decode(byte *a, undefined8 *b); /* @0x14006ae00             */
extern int64_t *PECMD_StrBldCopyAnsi(int64_t *out, const char *src,
                                     uint64_t len);             /* @0x1400702f0 取串槽      */
extern void PECMD_StrDupAssign(WCHAR **ps, const WCHAR *src);   /* @0x1400702b0 串赋值      */
extern WCHAR *PECMD_AssignString(WCHAR **ps, const WCHAR *src); /* @0x14007034c             */
extern void *PECMD_StrBldCopyWide(void *a, const WCHAR *b);     /* @0x1400703e4             */
extern int PECMD_ParseUIntValue(LPCWSTR *pp, int *out);         /* @0x140074838 数值解析    */
/* S11: 本地声明与定义冲突已删除, 统一采用 xproto.h 原型 (原: extern void PECMD_SetVariableWithPrefix(longlong *a, LPCWSTR b, LPCWSTR c); /* @0x14007d0ac * /) */
extern undefined8 PECMD_ListDrives(undefined8 *a, WCHAR b, int c, int d, uint e,
                                   LPCWSTR f); /* @0x1400787b4             */
extern undefined8 *PECMD_SplitTokenAssignVar(undefined8 *a, longlong *b, short c,
                                             byte d);                   /* @0x14007f6e4 */
extern undefined8 PECMD_EvalExprSkipOneChar(longlong *a, ulonglong *b); /* @0x1400a9a84 */
extern undefined8 PECMD_HasRasDialApi(void);                    /* @0x1400e4d84             */
extern undefined8 PECMD_LoadRasApi(undefined8 a);               /* @0x1400e4d94             */
extern void PECMD_DestroyEventSlots(undefined8 *a);             /* @0x1400e4e94             */
extern ulonglong PECMD_FindRasConnection(LPCWSTR a);            /* @0x1400e7664             */
extern longlong PECMD_EnumRasConnections(longlong *a);          /* @0x1400e7758             */
extern longlong PECMD_EnumPhonebookEntries(longlong *a, int b); /* @0x1400e7840             */
extern undefined8 PECMD_RasDialEntry(longlong a, LPCWSTR b, LPCWSTR c, LPCWSTR d, LPCWSTR e,
                                     LPCWSTR f);                  /* @0x1400ebd30             */
extern void PECMD_SetVariable(longlong *a, LPCWSTR b, LPCWSTR c); /* @0x1400629b8 SetVar      */
extern undefined8 PECMD_ShowBrowseFolder(longlong *a, ushort *b, longlong c,
                                         longlong *d);     /* @0x140087690 */
/* S11: 本地声明与定义冲突已删除, 统一采用 xproto.h 原型 (原: extern void PECMD_NotifyMainWindowRefresh(void *a, int b); /* @0x14009bb28 * / extern void PECMD_ExpandCommandL) */








extern int SHAppBarMessage(DWORD, APPBARDATA *);

/* ================================================================
 * @0x140096880  (longlong PECMD_ParseExtPathArg(longlong*, short*))
 * -ext 扩展名辅助: 取得路径基名, 可剥去扩展名。
 * ================================================================ */
longlong PECMD_ParseExtPathArg(longlong *param_1, short *param_2)
{
    char cVar1;
    longlong lVar2;
    LPCWSTR lpStart;
    short *psVar3;
    LPWSTR pWVar4;
    int iVar5;
    int iVar6;
    WCHAR *local_res10;
    WCHAR *local_res20;
    WCHAR *local_28[2];

    local_res10 = (WCHAR *)param_2;
    PECMD_SkipLeadingControlChars(&local_res10);
    PECMD_AllocWStringBuffer((WCHAR **)&local_res20, 5);
    PECMD_AllocWStringBuffer(local_28, 0x14);
    cVar1 = PECMD_MatchTokenAdvance("-ext", &local_res10, 4);
    PECMD_SplitTokenTrimWs((longlong *)&local_res10, (longlong *)&local_res20, 0x3d);
    PECMD_RunCommandLine(param_1, &local_res20, 0);
    if (*local_res10 == 0x3d) {
        local_res10 = local_res10 + 1;
        PECMD_SplitTokenTrimWs((longlong *)&local_res10, (longlong *)local_28, 0);
    }
    iVar6 = 0;
    if (*local_res20 == L'\0') {
        PECMD_FreeStrBuf((WCHAR **)&local_28);
        PECMD_FreeStrBuf((WCHAR **)&local_res20);
        lVar2 = -0x7ff8ffa9;
    }
    else {
        lpStart = PECMD_UnquoteString(local_28[0]);
        iVar5 = -0x7ff8ffa9;
        if (*lpStart != L'\0') {
            psVar3 = PECMD_LastPathSeparator((short *)lpStart);
            if (psVar3 != (short *)0x0) {
                lpStart = (LPCWSTR)(psVar3 + 1);
            }
            iVar5 = iVar6;
            if ((cVar1 == '\0') &&
                (pWVar4 = StrRChrW(lpStart, (LPCWSTR)0x0, L'.'), pWVar4 != (LPWSTR)0x0)) {
                *pWVar4 = L'\0';
            }
        }
        PECMD_SetVariable(param_1, local_res20, lpStart);
        PECMD_FreeStrBuf((WCHAR **)&local_28);
        PECMD_FreeStrBuf((WCHAR **)&local_res20);
        lVar2 = (longlong)iVar5;
    }
    return lVar2;
}

/* ================================================================
 * @0x14009c548  (uint64_t PECMD_ControlTimer(uint64_t*, uint64_t, ushort*,
 *                                        LPCWSTR, uint64_t, uint64_t*))
 * -del 定时器控制: 解析间隔/标志, 设置/清除定时器并回显。
 * ================================================================ */
undefined8 PECMD_ControlTimer(undefined8 *param_1, undefined8 param_2, ushort *param_3,
                              LPCWSTR param_4, undefined8 param_5, undefined8 *param_6)
{
    (void)param_2;
    (void)param_5;
    HWND hWnd;
    void *lpCriticalSection;
    bool bVar1;
    bool bVar2;
    undefined8 uVar3;
    UINT uElapse;
    int iVar4;
    LPCWSTR pwVar5;
    HWND pHVar6;
    UINT UVar7;
    int local_res8[2];
    UINT local_28[2];
    ushort *local_20;
    uint8_t *pb;

    pb = (uint8_t *)param_1;
    hWnd = (HWND)param_1[0xf];
    pHVar6 = *(HWND *)(*(longlong *)(param_1[10] + 0x40) + 0x20);
    uVar3 = PECMD_AsciiPrefixICmp("-del", param_3, 4);
    pwVar5 = WSTR("*del");
    if ((char)uVar3 == '\0') {
        pwVar5 = (LPCWSTR)param_4;
    }
    bVar1 = PECMD_OnDeleteCommand(param_1, pwVar5, pHVar6) != 0;
    UVar7 = 0;
    if (bVar1) {
        lpCriticalSection = (void *)*param_6;
        if (lpCriticalSection != (void *)0x0) {
            *param_6 = 0;
            LeaveCriticalSection(lpCriticalSection);
        }
        if (*pwVar5 != L'\0') {
            return 0x80070057;
        }
        local_28[0] = 0xffffff00;
        local_res8[0] = 0;
        local_20 = param_3;
        bVar1 = PECMD_ParseUIntValue((LPCWSTR *)&local_20, (int *)local_28) > 0;
        uElapse = local_28[0];
        iVar4 = 0;
        if (*local_20 != 0) {
            local_20 = local_20 + 1;
            bVar2 = PECMD_ParseUIntValue((LPCWSTR *)&local_20, local_res8) > 0;
            uElapse = local_28[0];
            iVar4 = local_res8[0];
            if ((bVar2) && (!bVar1)) {
                uElapse = *(UINT *)(pb + 0x74);
            }
        }
        if (((int)uElapse < 1) || (iVar4 < 0)) {
            if (*(int *)(pb + 0x80) != 0) {
                KillTimer(hWnd, (longlong) * (int *)(pb + 0x70));
            }
            *(undefined4 *)(pb + 0x80) = 0;
            *(undefined4 *)(pb + 0x84) = 0;
        }
        else {
            if (*(int *)(pb + 0x80) != 0) {
                KillTimer(hWnd, (longlong) * (int *)(pb + 0x70));
            }
            *(int *)(pb + 0x84) = iVar4;
            *(UINT *)(pb + 0x74) = uElapse;
            SetTimer(hWnd, (longlong) * (int *)(pb + 0x70), uElapse, (TIMERPROC)0x0);
            *(undefined4 *)(pb + 0x80) = 1;
            UVar7 = uElapse;
        }
        PECMD_AppendLongDecimal((longlong *)param_1[10], (undefined8)(ulonglong)UVar7,
                                (LPCWSTR)param_1[2]);
    }
    return 0;
}

/* ================================================================
 * @0x1400969d4  (longlong PECMD_LinkFile(longlong*, LPCWSTR))
 * -j 硬链接 / 符号链接 / 目录删除 / 文件操作。
 * ================================================================ */
longlong PECMD_LinkFile(longlong *param_1, LPCWSTR param_2)
{
    WCHAR WVar1;
    char cVar2;
    int iVar3;
    int iVar4;
    DWORD DVar5;
    DWORD DVar6;
    longlong *plVar7;
    undefined8 *puVar8;
    LPCWSTR pWVar9;
    WCHAR *pWVar10;
    undefined8 uVar11;
    DWORD DVar12;
    uint uVar13;
    DWORD DVar14;
    LPCWSTR lpString;
    DWORD DVar15;
    WCHAR *local_res10;
    uint64_t local_res20;
    WCHAR *local_98;
    WCHAR *local_90;
    WCHAR *local_88;
    longlong local_80;
    longlong *local_78;
    undefined8 local_70;
    LPWSTR local_68;
    uint local_60[10];

    local_res10 = (WCHAR *)param_2;
    PECMD_AllocStrSlot(&local_80);
    local_70 = 0;
    local_78 = param_1;
    (void)local_70;
    (void)local_78;
    PECMD_SkipLeadingControlChars(&local_res10);
    DVar6 = 0;
    PECMD_AllocWStringBuffer((WCHAR **)&local_90, 5);
    PECMD_AllocWStringBuffer((WCHAR **)&local_98, 0x14);
    DVar5 = 0;
    local_res20 = (uint64_t)((uint64_t)(uint32_t)(local_res20 >> 32) << 32);
    cVar2 = PECMD_MatchTokenAdvance("-j", &local_res10, 2);
    DVar12 = 1;
    plVar7 = (longlong *)PECMD_SplitTokenAssignVar((undefined8 *)&local_80,
                                                   (longlong *)&local_res10, 0x2c, 1);
    PECMD_SplitTokenTrimWs((longlong *)plVar7, (longlong *)&local_90, 0);
    if (*local_res10 == L',') {
        local_res10 = local_res10 + 1;
        plVar7 = (longlong *)PECMD_SplitTokenAssignVar((undefined8 *)&local_80,
                                                       (longlong *)&local_res10, 0x2c, 1);
        PECMD_SplitTokenTrimWs((longlong *)plVar7, (longlong *)&local_98, 0);
    }
    DVar14 = DVar5;
    DVar15 = DVar5;
    if (*local_res10 == L',') {
        DVar15 = (DWORD)(local_res10[1] == L'?');
        pWVar9 = (LPCWSTR)(local_res10 + 1);
        if (DVar15 != 0) {
            local_res20 = CONCAT44(local_res20 >> 32, 0x10);
            pWVar9 = (LPCWSTR)(local_res10 + 2);
        }
        local_res10 = (WCHAR *)pWVar9;
        puVar8 =
            PECMD_SplitTokenAssignVar((undefined8 *)&local_80, (longlong *)&local_res10, 0x2c, 1);
        PECMD_ParseShortStore(puVar8, (int *)&local_res20, 0x2c);
        DVar14 = (uint)(uint32_t)local_res20;
    }
    if ((*local_90 == 0) || (((DVar14 == 0 && (cVar2 == '\0')) && (*local_98 == L'\0')))) {
    LAB_140096b4b:
        PECMD_FreeStrBuf((WCHAR **)&local_98);
        PECMD_FreeStrBuf((WCHAR **)&local_90);
        PECMD_FreeStrBuf(&local_80);
        return -0x7ff8ffa9;
    }
    iVar3 = lstrlenW(local_98);
    PECMD_AllocString(&local_98, (longlong)(iVar3 + 0x214));
    local_68 = (LPWSTR)0x0;
    pWVar9 = PECMD_UnquoteString(local_98);
    pWVar10 = PECMD_UnquoteString((WCHAR *)local_90);
    if (*pWVar10 == L'\0')
        goto LAB_140096b4b;
    if (((DVar15 == 0) && (cVar2 == '\0')) && (DVar14 == 0)) {
        iVar3 = CreateHardLinkW(pWVar10, pWVar9, (LPSECURITY_ATTRIBUTES)0x0);
    }
    else {
        if ((DVar14 == 2) || (cVar2 != '\0')) {
            if (*pWVar9 == L'\0') {
                iVar3 = RemoveDirectoryW(pWVar10);
            }
            else {
                PECMD_ExpandDrivePath(pWVar9, 0x208, local_98 + (iVar3 + 4) + 4, &local_68);
                memcpy(local_98 + (iVar3 + 4), WSTR("\\??\\"), 8);
                uVar11 = PECMD_MountVolumeToDir(pWVar10, local_98 + (iVar3 + 4));
                iVar3 = (int)uVar11;
            }
            if (iVar3 == 0) {
                DVar6 = GetLastError();
            }
            PECMD_FreeStrBuf((WCHAR **)&local_98);
            PECMD_FreeStrBuf((WCHAR **)&local_90);
            PECMD_FreeStrBuf(&local_80);
            return (longlong)(int)DVar6;
        }
        if (DVar15 != 0) {
            memset((ulonglong *)local_60, 0, 0x24);
            GetFileAttributesExW(pWVar10, 2, local_60);
            local_88 = (WCHAR *)0x0;
            lpString = (LPCWSTR)g_szEmpty;
            if ((local_60[0] >> 10 & 1) != 0) {
                local_res10 = (WCHAR *)0x0;
                PECMD_OpenFileHandle(&local_res10, pWVar10, 0xc0000000, 7,
                                     (LPSECURITY_ATTRIBUTES)0x0, 3, 0x80, (HANDLE)0x0);
                pWVar10 = (WCHAR *)local_res10;
                if (local_res10 != (WCHAR *)0x0) {
                    PECMD_GetApiProcCached("GetFinalPathNameByHandleW", (LPCSTR)0x0,
                                           (void **)&g_pGetFinalPathNameByHandleW, &g_hKernel32);
                    if (g_pGetFinalPathNameByHandleW !=
                        (DWORD (*)(HANDLE, LPWSTR, DWORD, DWORD))0) {
                        PECMD_AllocString(&local_88, 0xa000);
                        lpString = (LPCWSTR)local_88;
                        *local_88 = L'\0';
                        DVar12 =
                            (*g_pGetFinalPathNameByHandleW)((HANDLE)pWVar10, local_88, 0xa000, 8);
                        if (DVar12 == 0) {
                            DVar12 = (*g_pGetFinalPathNameByHandleW)((HANDLE)pWVar10,
                                                                     (LPWSTR)lpString, 0xa000, 10);
                            iVar3 = lstrlenW(lpString);
                            if ((7 < iVar3) &&
                                (iVar4 = StrCmpNW(lpString, WSTR("\\Device\\"), 8), iVar4 == 0)) {
                                local_res20 = (uint64_t)(uintptr_t)(lpString + 8);
                                PECMD_SkipWCharUntil((WCHAR **)&local_res20, 0x5c);
                                WVar1 = *(WCHAR *)(uintptr_t)local_res20;
                                if (WVar1 != L'\0') {
                                    *(WCHAR *)(uintptr_t)local_res20 = L'\0';
                                }
                                PECMD_AllocWStringBuffer((WCHAR **)&local_res10, 100);
                                *((LPWSTR)local_res10) = L'\0';
                                PECMD_EnumerateVolume((longlong *)&local_res10, -5, 0x48fffb,
                                                      lpString);
                                if (WVar1 != L'\0') {
                                    *(WCHAR *)(uintptr_t)local_res20 = WVar1;
                                }
                                iVar4 = lstrlenW(local_res10);
                                if (iVar4 != 0) {
                                    memmove((uint8_t *)(lpString + iVar4),
                                            (uint8_t *)(uintptr_t)local_res20,
                                            ((iVar3 - (int)(((uintptr_t)local_res20 -
                                                             (uintptr_t)lpString) >>
                                                            1)) +
                                             1) *
                                                2);
                                    memcpy((uint8_t *)lpString, (uint8_t *)local_res10, iVar4 * 2);
                                }
                                PECMD_FreeStrBuf((WCHAR **)&local_res10);
                            }
                        }
                        if (((int)DVar12 < 1) && (DVar12 = GetLastError(), DVar12 == 0)) {
                            DVar12 = 1;
                        }
                    }
                    if ((uintptr_t)pWVar10 != 0xffffffffffffffff) {
                        CloseHandle((HANDLE)pWVar10);
                    }
                }
            }
            PECMD_SetVariable(param_1, pWVar9, lpString);
            PECMD_FreeStrBuf((WCHAR **)&local_88);
            DVar5 = DVar12;
            goto LAB_140096ecc;
        }
        iVar3 = lstrlenW(pWVar10);
        uVar13 = (uint)((DVar14 & 0x10) != 0);
        if (pWVar10[(longlong)iVar3 + -1] == L'\\') {
            pWVar10[(longlong)iVar3 + -1] = L'\0';
            uVar13 = DVar12;
        }
        if ((uVar13 == 0) && (DVar14 == 3)) {
            uVar11 = PECMD_IsDirectory(pWVar9);
            uVar13 = (uint)((int)uVar11 != 0);
        }
        uVar11 = PECMD_CreateSymbolicLink((undefined8)(uintptr_t)pWVar10,
                                          (undefined8)(uintptr_t)pWVar9, (undefined4)uVar13);
        iVar3 = (int)uVar11;
    }
    if (iVar3 == 0) {
        DVar5 = GetLastError();
    }
LAB_140096ecc:
    PECMD_FreeStrBuf((WCHAR **)&local_98);
    PECMD_FreeStrBuf((WCHAR **)&local_90);
    PECMD_FreeStrBuf(&local_80);
    return (longlong)(int)DVar5;
}

/* ================================================================
 * @0x14009bbbc  (LARGE_INTEGER* PECMD_TempPathCommand(longlong*, LARGE_INTEGER,
 *                                             longlong))
 * 临时目录 / *del / *tmpdir / *tmpfile / Delete / Setting 命令。
 * ================================================================ */
LARGE_INTEGER *PECMD_TempPathCommand(longlong *param_1, LARGE_INTEGER param_2, longlong param_3)
{
    ushort uVar1;
    bool bVar2;
    char cVar3;
    char cVar4;
    int iVar5;
    undefined8 uVar6;
    LARGE_INTEGER LVar7;
    LPWSTR pWVar8;
    LPCWSTR pWVar9;
    LPCWSTR pWVar10;
    LPCWSTR pWVar11;
    LARGE_INTEGER *pLVar12;
    LPCWSTR *ppWVar13;
    LARGE_INTEGER *pLVar14;
    WCHAR WVar15;
    LARGE_INTEGER LVar16;
    LARGE_INTEGER *pLVar17;
    char cVar18;
    LARGE_INTEGER local_res10;
    longlong local_res18;
    undefined8 local_res20;
    char local_ef8;
    DWORD local_ef4;
    LPCWSTR local_ef0;
    WCHAR *local_ee8;
    WCHAR *local_ee0;
    ushort *local_ed8;
    WCHAR *local_ed0;
    LPCWSTR local_ec8;
    WCHAR *local_ec0;
    WCHAR *local_eb8;
    WCHAR *local_eb0;
    WCHAR local_ea8[264];
    WCHAR local_c98[264];
    WCHAR local_a88[264];
    WCHAR local_878[264];
    WCHAR local_668[264];
    WCHAR local_458[264];
    WCHAR local_248[264];

    pLVar17 = (LARGE_INTEGER *)0x0;
    cVar3 = '\0';
    local_res20 = 0;
    cVar18 = '\0';
    local_ef8 = '\0';
    local_res10 = param_2;
    local_res18 = param_3;
    local_eb8 = (WCHAR *)0x0;
    PECMD_AllocStrSlot(&local_ee8);
    PECMD_StrBldCopyAnsi((longlong *)&local_eb8, ".tmp", 0xffffffffffffffff);
    PECMD_SkipLeadingControlChars((WCHAR **)&local_res10.QuadPart);
    bVar2 = false;
    LVar7 = local_res10;
    while (*(ushort *)LVar7.QuadPart == 0x2a) {
        local_res10.QuadPart = LVar7.QuadPart + 2;
        cVar4 = PECMD_MatchTokenAdvance("del", (WCHAR **)&local_res10.QuadPart, 3);
        if (cVar4 == '\0') {
            LVar7 = local_res10;
            uVar6 = PECMD_AsciiPrefixICmp("tmpl:", (ushort *)local_res10.QuadPart, 5);
            if ((char)uVar6 == '\0') {
                cVar4 = PECMD_MatchTokenAdvance("tmpdir", (WCHAR **)&local_res10.QuadPart, 6);
                if (cVar4 == '\0') {
                    cVar4 = PECMD_MatchTokenAdvance("tmpfile", (WCHAR **)&local_res10.QuadPart, 7);
                    if (cVar4 == '\0') {
                        uVar1 = *(ushort *)local_res10.QuadPart;
                        while (
                            (uVar1 != 0 && (((uVar1 < 9 || (0xd < uVar1)) && (uVar1 != 0x20))))) {
                            local_res10.QuadPart = local_res10.QuadPart + 2;
                            uVar1 = *(ushort *)local_res10.QuadPart;
                        }
                        PECMD_SkipLeadingControlChars((WCHAR **)&local_res10.QuadPart);
                        LVar7 = local_res10;
                    }
                    else {
                        cVar18 = '\x02';
                        LVar7 = local_res10;
                    }
                }
                else {
                    cVar18 = '\x01';
                    LVar7 = local_res10;
                }
            }
            else {
                local_ef0 = (LPCWSTR)(LVar7.QuadPart + 10);
                local_ee0 = (WCHAR *)0x0;
                LVar7 = PECMD_LI((intptr_t)PECMD_NextToken((longlong *)&local_ef0,
                                                                 (longlong *)&local_ee0, 5));
                local_res10 = LVar7;
                if (0 < (longlong)(intptr_t)local_ee0) {
                    PECMD_StrCopyW(&local_ee8, local_ef0, (longlong)(intptr_t)local_ee0);
                    pWVar8 = StrChrW((LPCWSTR)local_ee8, L'*');
                    if (pWVar8 != (LPWSTR)0x0) {
                        *pWVar8 = L'\0';
                        PECMD_StrBldCopyWide(&local_eb8, pWVar8 + 1);
                    }
                }
            }
        }
        else {
            bVar2 = true;
            LVar7 = local_res10;
        }
    }
    while (uVar1 = *(ushort *)LVar7.QuadPart, uVar1 != 0) {
        if (uVar1 == 0x40) {
            local_res20 = (undefined8)(uint8_t)*(char *)LVar7.QuadPart;
        }
        else {
            if (uVar1 != 0x24)
                break;
            local_ef8 = *(char *)LVar7.QuadPart;
        }
        local_res10.QuadPart = LVar7.QuadPart + 2;
        PECMD_SkipLeadingControlChars((WCHAR **)&local_res10.QuadPart);
        LVar7 = local_res10;
    }
    if (cVar18 == '\0') {
        cVar3 = PECMD_MatchTokenAdvance("Delete", (WCHAR **)&local_res10.QuadPart, 6);
        cVar3 = cVar3 != '\0';
        cVar4 = PECMD_MatchTokenAdvance("Setting", (WCHAR **)&local_res10.QuadPart, 7);
        if (cVar4 != '\0') {
            cVar3 = -1;
        }
    }
    PECMD_SkipLeadingControlChars((WCHAR **)&local_res10.QuadPart);
    local_ee0 = (WCHAR *)0x0;
    local_ed0 = (WCHAR *)0x0;
    local_ef0 = (LPCWSTR)0x0;
    PECMD_AllocStrSlot(&local_ee0);
    PECMD_AllocStrSlot(&local_ed0);
    PECMD_AllocStrSlot(&local_ef0);
    if (cVar18 == '\0') {
        if (*(ushort *)local_res10.QuadPart != 0x2c) {
            PECMD_SplitTokenTrimWs(&local_res10.QuadPart, (longlong *)&local_ee0, 0x2c);
            goto LAB_14009bea3;
        }
    LAB_14009bea8:
        local_res10.QuadPart = local_res10.QuadPart + 2;
    }
    else {
    LAB_14009bea3:
        if (*(ushort *)local_res10.QuadPart == 0x2c)
            goto LAB_14009bea8;
    }
    PECMD_SplitTokenTrimWs(&local_res10.QuadPart, (longlong *)&local_ed0, 0x2c);
    if (*(ushort *)local_res10.QuadPart == 0x2c) {
        local_res10.QuadPart = local_res10.QuadPart + 2;
    }
    PECMD_SplitTokenTrimWs(&local_res10.QuadPart, (longlong *)&local_ef0, 0x2c);
    if (cVar18 != '\0') {
        local_ec0 = (WCHAR *)0x0;
        PECMD_AllocStrSlot(&local_ec0);
        pWVar9 = local_ef0;
        if ((cVar18 == '\x01') || (pLVar12 = pLVar17, *local_ef0 != L'\0')) {
            pLVar12 = (LARGE_INTEGER *)&local_ec0;
        }
        pLVar14 = (LARGE_INTEGER *)&local_ee8;
        if (cVar18 != '\x02') {
            pLVar14 = pLVar17;
        }
        PECMD_CreateTempMutexDir(&pLVar12->QuadPart, &pLVar14->QuadPart, (undefined8 *)0x0,
                                 WSTR("mktmp"), (uint64_t)(uintptr_t)WSTR(".tmp"));
        /* R23 定案: 原版 0x14009bf48 第5参=[RSP+0x20]=RAX 运行时值(param_1 非空走目录模式,
         * param_5 不读; 传 ".tmp" 占位)。 */
        if (cVar18 == '\x01') {
            PECMD_SetVariable(param_1, local_ed0, (LPCWSTR)local_ec0);
            WVar15 = *(WCHAR *)local_ec0;
        }
        else {
            PECMD_SetVariable(param_1, local_ed0, (LPCWSTR)local_ee8);
            PECMD_SetVariable(param_1, pWVar9, (LPCWSTR)local_ec0);
            WVar15 = *(WCHAR *)local_ee8;
        }
        if ((bVar2) && (*(short *)local_ec0 != 0)) {
            PECMD_ScheduleSelfDelete(local_ec0, 0);
        }
        PECMD_NotifyMainWindowRefresh(param_1, 0);
        PECMD_FreeStrBuf(&local_ec0);
        PECMD_FreeStrBuf((WCHAR **)&local_ef0);
        PECMD_FreeStrBuf((WCHAR **)&local_ed0);
        PECMD_FreeStrBuf(&local_ee0);
        PECMD_FreeStrBuf(&local_eb8);
        PECMD_FreeStrBuf(&local_ee8);
        return (LARGE_INTEGER *)(ulonglong)(WVar15 == L'\0');
    }
    local_ef4 = 0;
    local_ec0 = (WCHAR *)0x0;
    local_eb0 = (WCHAR *)0x0;
    PECMD_AllocStrSlot(&local_ec0);
    PECMD_AllocStrSlot(&local_eb0);
    PECMD_RegReadWrap((void *)0xffffffff80000001, WSTR("Environment"), WSTR("TEMP"),
                      (longlong *)&local_ec0, &local_ef4, (LSTATUS *)0x0);
    if (((*(ushort *)local_ec0 == 0) &&
         (PECMD_RegReadWrap((void *)0xffffffff80000001, WSTR("Environment"), WSTR("TMP"),
                            (longlong *)&local_ec0, &local_ef4, (LSTATUS *)0x0),
          *(ushort *)local_ec0 == 0)) &&
        (PECMD_RegReadWrap((void *)0xffffffff80000002,
                           WSTR("SYSTEM\\CurrentControlSet\\Control\\Session Manager\\Environment"),
                           WSTR("TEMP"), (longlong *)&local_ec0, &local_ef4, (LSTATUS *)0x0),
         *(ushort *)local_ec0 == 0)) {
        PECMD_RegReadWrap((void *)0xffffffff80000002,
                          WSTR("SYSTEM\\CurrentControlSet\\Control\\Session Manager\\Environment"),
                          WSTR("TMP"), (longlong *)&local_ec0, &local_ef4, (LSTATUS *)0x0);
    }
    LVar7 = PECMD_LI((intptr_t)local_ec0);
    if (*local_ed0 != L'\0') {
        PECMD_SetVariable(param_1, local_ed0, (LPCWSTR)local_ec0);
    }
    if (cVar3 == '\0') {
        pLVar12 = (LARGE_INTEGER *)0xffffffff80070057;
        goto LAB_14009c4e0;
    }
    if (*(short *)LVar7.QuadPart == 0) {
        pWVar9 = PECMD_LoadLocalizedString(g_hInstance, 0x2716, local_878, 0x104);
        pWVar10 = PECMD_LoadLocalizedString(g_hInstance, 0x2717, local_458, 0x104);
        MessageBoxW((HWND)0x0, pWVar9, pWVar10, 0x10);
        pLVar12 = (LARGE_INTEGER *)0x1;
        if (cVar3 == '\x01')
            goto LAB_14009c4e0;
    }
    PECMD_ExpandCommandLine(param_1, (WCHAR *)LVar7.QuadPart, &local_eb0, 0, 0);
    LVar16 = PECMD_LI((intptr_t)local_eb0);
    local_res10 = PECMD_LI((intptr_t)local_eb0);
    local_eb0 = (WCHAR *)LVar7.QuadPart;
    pWVar9 = PECMD_LoadLocalizedString(g_hInstance, 0x2718, local_ea8, 0x104);
    pLVar12 = pLVar17;
    if (cVar3 == '\x01') {
        if ((char)local_res20 == '\0') {
            pWVar10 = PECMD_LoadLocalizedString(g_hInstance, 0x2719, local_c98, 0x104);
            pWVar11 = PECMD_LoadLocalizedString(g_hInstance, 0x271a, local_a88, 0x104);
            PECMD_StrDupAssign((WCHAR **)&local_res20, pWVar9);
            PECMD_AppendWideStr((WCHAR **)&local_res20, (LPCWSTR)LVar16.QuadPart);
            PECMD_AppendWideStr((WCHAR **)&local_res20, pWVar10);
            iVar5 = MessageBoxW((HWND)0x0, (LPCWSTR)local_res20, pWVar11, 4);
            ppWVar13 = (LPCWSTR *)&local_res20;
            if (iVar5 != 6)
                goto LAB_14009c4d8;
            PECMD_FreeStrBuf((WCHAR **)&ppWVar13);
        }
        PECMD_AppendWideStr((WCHAR **)&local_res10.QuadPart, WSTR("\\*.*"));
        PECMD_IndataCopyDirs(param_1, &local_res10, 1);
    }
    else {
        pWVar10 = PECMD_LoadLocalizedString(g_hInstance, 0x271b, local_668, 0x104);
        pWVar11 = PECMD_LoadLocalizedString(g_hInstance, 0x271c, local_248, 0x104);
        local_ec8 = (LPCWSTR)0x0;
        PECMD_StrDupAssign((WCHAR **)&local_ec8, pWVar9);
        PECMD_AppendWideStr((WCHAR **)&local_ec8, (LPCWSTR)LVar16.QuadPart);
        PECMD_AppendWideStr((WCHAR **)&local_ec8, pWVar10);
        local_ec0 = (WCHAR *)0x0;
        local_ed8 = (ushort *)0x0;
        PECMD_AllocStrSlot(&local_ec0);
        PECMD_AllocStrSlot(&local_ed8);
        LVar7 = PECMD_LI((intptr_t)local_ee0);
        if ((*(short *)local_ee0 == 0) || ((char)local_res20 == '\0')) {
            iVar5 = MessageBoxW((HWND)0x0, local_ec8, pWVar11, 4);
            if (iVar5 == 6) {
                PECMD_AssignString((WCHAR **)&local_ed8, WSTR("_,*"));
                if (*(short *)LVar7.QuadPart != 0) {
                    LVar16 = LVar7;
                }
                PECMD_AppendWideStr((WCHAR **)&local_ed8, (LPCWSTR)LVar16.QuadPart);
                PECMD_AppendWideStr((WCHAR **)&local_ed8, WSTR(",,,0x10"));
                PECMD_ShowBrowseFolder(param_1, (ushort *)local_ed8, local_res18,
                                       (longlong *)&local_ec0);
                LVar7 = PECMD_LI((intptr_t)local_ec0);
                if (*(short *)local_ec0 != 0)
                    goto LAB_14009c3db;
            }
        }
        else {
        LAB_14009c3db:
            iVar5 = lstrlenW((LPCWSTR)LVar7.QuadPart);
            local_ef4 = iVar5 * 2;
            PECMD_RegSetValueWithOpen((HKEY)0xffffffff80000001, WSTR("Environment"), WSTR("TMP"), 1,
                                      (BYTE *)LVar7.QuadPart, local_ef4);
            PECMD_RegSetValueWithOpen((HKEY)0xffffffff80000001, WSTR("Environment"), WSTR("TEMP"),
                                      1, (BYTE *)LVar7.QuadPart, local_ef4);
            if (local_ef8 != '\0') {
                PECMD_RegSetValueWithOpen(
                    (HKEY)0xffffffff80000002,
                    WSTR("SYSTEM\\CurrentControlSet\\Control\\Session Manager\\Environment"),
                    WSTR("TEMP"), 1, (BYTE *)LVar7.QuadPart, local_ef4);
                PECMD_RegSetValueWithOpen(
                    (HKEY)0xffffffff80000002,
                    WSTR("SYSTEM\\CurrentControlSet\\Control\\Session Manager\\Environment"),
                    WSTR("TMP"), 1, (BYTE *)LVar7.QuadPart, local_ef4);
            }
            PECMD_SetVariable(param_1, WSTR("TMP"), (LPCWSTR)LVar7.QuadPart);
            PECMD_SetVariable(param_1, WSTR("TEMP"), (LPCWSTR)LVar7.QuadPart);
            PECMD_SetVariable(param_1, local_ed0, (LPCWSTR)LVar7.QuadPart);
        }
        PECMD_FreeStrBuf((WCHAR **)&local_ed8);
        PECMD_FreeStrBuf((WCHAR **)&local_ec0);
        ppWVar13 = &local_ec8;
    LAB_14009c4d8:
        PECMD_FreeStrBuf((WCHAR **)&ppWVar13);
    }
LAB_14009c4e0:
    PECMD_FreeStrBuf(&local_eb0);
    PECMD_FreeStrBuf(&local_ec0);
    PECMD_FreeStrBuf((WCHAR **)&local_ef0);
    PECMD_FreeStrBuf((WCHAR **)&local_ed0);
    PECMD_FreeStrBuf(&local_ee0);
    PECMD_FreeStrBuf(&local_eb8);
    PECMD_FreeStrBuf(&local_ee8);
    return pLVar12;
}

/* ================================================================
 * @0x140097c70  (ulonglong PECMD_ScreenAreaCommand(longlong*, LPCWSTR))
 * 屏幕/窗口区域命令: -cap / -max / -win / -taskbar / -desk- / -save /
 * -display: / -gui / -capgui, 输出数值或截图。
 * ================================================================ */
ulonglong PECMD_ScreenAreaCommand(longlong *param_1, LPCWSTR param_2)
{
    WCHAR WVar1;
    WCHAR WVar2;
    char cVar3;
    char cVar4;
    uint uVar5;
    uint uVar6;
    int iVar7;
    BOOL BVar8;
    DWORD DVar9;
    uint uVar10;
    uint uVar11;
    undefined8 uVar12;
    ulonglong uVar13;
    ulonglong uVar14;
    HDC hdc;
    ulonglong uVar15;
    ulonglong uVar16;
    char cVar17;
    char cVar18;
    RECT *hWnd;
    ulonglong uVar19;
    RECT *ptVar20;
    ulonglong uVar21;
    longlong lVar22;
    LPCWSTR pWVar23;
    int iVar24;
    char cVar25;
    ulonglong uVar26;
    byte bVar27;
    WCHAR *local_res10[2];
    uint64_t local_res20;
    WCHAR *local_c8;
    byte local_c0;
    char local_bf;
    uint local_bc;
    uint local_b8;
    RECT local_b0;
    uint local_a0;
    uint64_t local_98;
    WCHAR *local_90;
    LPCWSTR local_88;
    uint local_80;
    uint local_78;
    APPBARDATA local_70;

    local_res10[0] = (WCHAR *)param_2;
    PECMD_SkipLeadingControlChars(local_res10);
    PECMD_AllocWStringBuffer((WCHAR **)&local_90, 5);
    PECMD_AllocWStringBuffer((WCHAR **)&local_c8, 0x14);
    cVar18 = '\0';
    local_88 = WSTR("DISPLAY");
    local_bf = '\0';
    local_c0 = 0;
    local_res20 = 0;
    cVar17 = '\0';
    cVar25 = cVar18;
    cVar4 = cVar18;
    if (*local_res10[0] == L'-') {
    LAB_140097cfe:
        cVar3 = PECMD_MatchTokenAdvance("-cap", local_res10, 4);
        cVar4 = '\x01';
        if (cVar3 == '\0') {
            cVar3 = PECMD_MatchTokenAdvance("-max", local_res10, 4);
            if (cVar3 == '\0') {
                cVar3 = PECMD_MatchTokenAdvance("-win", local_res10, 4);
                if (cVar3 == '\0') {
                    cVar3 = PECMD_MatchTokenAdvance("-taskbar", local_res10, 8);
                    if (cVar3 == '\0') {
                        cVar3 = PECMD_MatchTokenAdvance("-desk-", local_res10, 5);
                        if (cVar3 != '\0')
                            goto LAB_140097f1d;
                        cVar4 = PECMD_MatchTokenAdvance("-desk-", local_res10, 6);
                        if (cVar4 == '\0') {
                            cVar3 = PECMD_MatchTokenAdvance("-save", local_res10, 5);
                            cVar4 = cVar17;
                            if (cVar3 == '\0') {
                                pWVar23 = local_res10[0];
                                uVar12 =
                                    PECMD_AsciiPrefixICmp("-display:", (ushort *)local_res10[0], 9);
                                if ((char)uVar12 == '\0') {
                                    cVar4 = PECMD_MatchTokenAdvance("-gui", local_res10, 4);
                                    if ((cVar4 != '\0') ||
                                        (cVar4 = PECMD_MatchTokenAdvance("-capgui", local_res10, 7),
                                         cVar4 != '\0')) {
                                        PECMD_StrDupAssign((WCHAR **)&local_res20,
                                                           WSTR("#31:INDATA "));
                                        PECMD_AppendWideStr((WCHAR **)&local_res20, local_res10[0]);
                                        uVar16 = PECMD_RunCommand(param_1, local_res20);
                                        PECMD_FreeStrBuf((WCHAR **)&local_res20);
                                        goto LAB_1400987b7;
                                    }
                                    WVar1 = *local_res10[0];
                                    while ((WVar1 != L'\0' &&
                                            ((((ushort)WVar1 < 9 || (0xd < (ushort)WVar1)) &&
                                              (WVar1 != L' '))))) {
                                        local_res10[0] = local_res10[0] + 1;
                                        WVar1 = *local_res10[0];
                                    }
                                }
                                else {
                                    local_88 = pWVar23 + 9;
                                    WVar1 = *local_88;
                                    local_res10[0] = (WCHAR *)local_88;
                                    while (WVar1 != L'\0') {
                                        if (((8 < (ushort)WVar1) && ((ushort)WVar1 < 0xe)) ||
                                            (WVar1 == L' ')) {
                                            if (*local_res10[0] != L'\0') {
                                                *local_res10[0] = L'\0';
                                                local_res10[0] = local_res10[0] + 1;
                                            }
                                            break;
                                        }
                                        local_res10[0] = local_res10[0] + 1;
                                        WVar1 = *local_res10[0];
                                    }
                                }
                                PECMD_SkipLeadingControlChars(local_res10);
                                cVar4 = cVar17;
                            }
                        }
                        else {
                            cVar4 = -1;
                        }
                        goto LAB_140097f1d;
                    }
                    cVar4 = '\x02';
                }
                cVar18 = cVar4;
                local_res20 = (local_res20 & 0xffffffffffffff00ULL) | (uint8_t)(uint8_t)cVar18;
                cVar4 = cVar17;
            }
            else {
                local_c0 = 1;
                cVar4 = cVar17;
            }
        }
        else {
            local_bf = '\x01';
            cVar25 = cVar4;
            cVar4 = cVar17;
        }
    LAB_140097f1d:
        cVar17 = cVar4;
        cVar4 = cVar17;
        if (*local_res10[0] != L'-')
            goto LAB_140097f33;
        goto LAB_140097cfe;
    }
LAB_140097f33:
    PECMD_TokenizeQuotedField(param_1, (longlong *)local_res10, (longlong *)&local_90, L',', 8);
    uVar26 = 0;
    local_b8 = 0;
    memset(&local_b0, 0, sizeof(RECT));
    local_98 = 0;
    uVar5 = GetSystemMetrics(0);
    local_bc = uVar5;
    uVar6 = GetSystemMetrics(1);
    pWVar23 = local_res10[0];
    local_80 = uVar6;
    PECMD_ParseSkipSeparator((longlong *)local_res10, (longlong *)&local_c8, 0x2c, 0);
    local_78 = (uint)local_c0;
    if (cVar18 != '\0') {
        local_78 = 0;
    }
    if (((cVar4 < '\x01') && (*local_res10[0] == L'\0')) &&
        ((cVar18 != '\x01' && (cVar25 == '\0')))) {
        uVar10 = (uint)(uintptr_t)local_res10[0];
    }
    else {
        hdc = CreateDCW(local_88, (LPCWSTR)0x0, (LPCWSTR)0x0, (const void *)0x0);
        local_a0 = GetDeviceCaps(hdc, 0x76);
        uVar10 = GetDeviceCaps(hdc, 0x75);
        iVar7 = GetDeviceCaps(hdc, 0x58);
        local_b0.left = iVar7;
        iVar7 = GetDeviceCaps(hdc, 0x5a);
        local_98 = (uint64_t)(uint32_t)iVar7;
        if (hdc != (HDC)0x0) {
            DeleteDC(hdc);
        }
        if (local_bf != '\0') {
            WVar1 = *local_90;
            local_b0.left = 0;
            local_b0.top = 0;
            ptVar20 = (RECT *)0x0;
            local_b0.right = local_a0;
            local_res20 = 0;
            local_98 = 0;
            WVar2 = *pWVar23;
            local_res10[0] = (WCHAR *)pWVar23;
            if (WVar2 == L'#') {
                local_res10[0] = (WCHAR *)(pWVar23 + 1);
            }
            local_b0.bottom = uVar10;
            iVar7 = (int)PECMD_EvalExprSkipOneChar((longlong *)local_res10, (ulonglong *)&local_98);
            bVar27 = (byte)(iVar7 != 0);
            hWnd = ptVar20;
            if (bVar27) {
                local_b0.left = (int)(uint32_t)local_98;
                hWnd = (RECT *)(uintptr_t)local_98;
            }
            iVar7 =
                (int)PECMD_EvalExprSkipOneChar((longlong *)local_res10, (ulonglong *)&local_res20);
            if (iVar7 != 0) {
                bVar27 = (byte)(bVar27 | 2);
                local_b0.top = (uint)(uint32_t)local_res20;
            }
            iVar7 =
                (int)PECMD_EvalExprSkipOneChar((longlong *)local_res10, (ulonglong *)&local_res20);
            if (iVar7 != 0) {
                bVar27 = (byte)(bVar27 | 4);
                local_b0.right = (uint)(uint32_t)local_res20;
            }
            iVar7 =
                (int)PECMD_EvalExprSkipOneChar((longlong *)local_res10, (ulonglong *)&local_res20);
            if (iVar7 != 0) {
                bVar27 = (byte)(bVar27 | 8);
                local_b0.bottom = (uint)(uint32_t)local_res20;
            }
            PECMD_NextToken((longlong *)local_res10, (longlong *)0x0, 0x1d);
            pWVar23 = (LPCWSTR)0x0;
            if ((*local_res10[0] != L'\0') && (pWVar23 = local_res10[0], (bVar27 & 0xc) != 0xc)) {
                local_b0.bottom = 0;
                local_b0.right = 0;
            }
            if (WVar2 != L'#') {
                if ((bVar27 != 0) || (g_imgBufLen < 1)) {
                    ptVar20 = &local_b0;
                }
            }
            else {
                ptVar20 = &local_b0;
                BVar8 = GetWindowRect((HWND)(uintptr_t)hWnd, &local_b0);
                if (BVar8 == 0) {
                    DVar9 = GetLastError();
                    uVar16 = (ulonglong)DVar9;
                    if (DVar9 == 0) {
                        uVar16 = 1;
                    }
                    PECMD_FreeStrBuf((WCHAR **)&local_c8);
                    PECMD_FreeStrBuf((WCHAR **)&local_90);
                    return uVar16;
                }
            }
            DVar9 = (DWORD)PECMD_SaveImageToFile(
                ptVar20, (LPCWSTR)(-(uint64_t)(WVar1 != L'\0') & (uint64_t)(uintptr_t)local_90),
                pWVar23, local_88);
            uVar16 = (ulonglong)(int)DVar9;
            goto LAB_1400987b7;
        }
    }
    if ((char)local_78 != '\0') {
        uVar5 = GetSystemMetrics(0x3d);
        uVar6 = GetSystemMetrics(0x3e);
    }
    memset(&local_70, 0, sizeof(local_70));
    local_70.hWnd = (HWND)0x0;
    local_70.cbSize = 0x30;
    uVar13 =
        ((longlong)(int)local_a0 * 100 + (longlong)((int)local_bc / 2)) / (longlong)(int)local_bc;
    local_88 = (LPCWSTR)(uintptr_t)((int)local_80 / 2);
    uVar14 = (longlong)((longlong)(uintptr_t)local_88 + (longlong)(int)uVar10 * 0x32) /
             (longlong)(int)local_80;
    if ((local_a0 == local_bc) && (uVar10 == local_80)) {
        uVar13 = ((longlong)local_b0.left * 100 + 0x30) / 0x60;
        uVar14 = ((longlong)(int)(uint32_t)local_98 * 100 + 0x30) / 0x60;
    }
    else {
        local_b0.left = (LONG)((longlong)(int)local_a0 * 0x60 + (longlong)((int)local_bc / 2)) /
                        (longlong)(int)local_bc;
        local_98 = (uint64_t)(((longlong)(uintptr_t)local_88 + (longlong)(int)uVar10 * 0x30) /
                              (longlong)(int)local_80);
    }
    if ('\0' < cVar4) {
        uVar6 = uVar10;
        uVar5 = local_a0;
    }
    uVar21 = (ulonglong)uVar5;
    uVar19 = (ulonglong)uVar6;
    cVar25 = (char)local_res20;
    iVar7 = (int)uVar14;
    iVar24 = (int)uVar13;
    if ((char)local_res20 == '\0') {
    LAB_1400985e6:
        uVar15 = (ulonglong)local_b8;
    }
    else {
        SHAppBarMessage(ABM_GETTASKBARPOS, &local_70);
        if (cVar25 == '\x02') {
            uVar15 = (ulonglong)(uint)local_70.rc.top;
            uVar19 = (ulonglong)(uint)(local_70.rc.bottom - local_70.rc.top);
            uVar21 = (ulonglong)(uint)(local_70.rc.right - local_70.rc.left);
            uVar26 = (ulonglong)(uint)local_70.rc.left;
        }
        else if (local_70.uEdge == 3) {
            uVar19 = (ulonglong)local_70.rc.top;
            uVar15 = (ulonglong)local_b8;
            uVar26 = 0;
            if (cVar4 < '\x01') {
                uVar19 = (longlong)((longlong)(iVar7 / 2) + uVar19 * 100) / (longlong)iVar7;
            }
        }
        else if (local_70.uEdge == 1) {
            uVar15 = (ulonglong)(uint)local_70.rc.bottom;
            if (cVar4 < '\x01') {
                uVar15 = ((longlong)(iVar24 / 2) + (longlong)local_70.rc.bottom * 100) /
                         (longlong)iVar24;
            }
            uVar19 = (ulonglong)(uVar6 - local_70.rc.bottom);
            if (cVar4 < '\x01') {
                uVar19 =
                    (ulonglong)(local_80 -
                                (int)(((longlong)(iVar7 / 2) + (longlong)local_70.rc.bottom * 100) /
                                      (longlong)iVar7));
            }
        }
        else if (local_70.uEdge == 0) {
            uVar26 = (ulonglong)(uint)local_70.rc.right;
            if (cVar4 < '\x01') {
                uVar26 =
                    ((longlong)(iVar24 / 2) + (longlong)local_70.rc.right * 100) / (longlong)iVar24;
            }
            uVar15 = (ulonglong)local_b8;
            uVar21 = (ulonglong)(uVar5 - local_70.rc.right);
            if (cVar4 < '\x01') {
                uVar21 =
                    (ulonglong)(local_bc -
                                (int)(((longlong)(iVar7 / 2) + (longlong)local_70.rc.right * 100) /
                                      (longlong)iVar7));
            }
        }
        else {
            if (local_70.uEdge != 2) {
                uVar10 = GetSystemMetrics(0x3d);
                uVar11 = GetSystemMetrics(0x3e);
                if ('\0' < cVar4) {
                    lVar22 = (longlong)(int)uVar10 * (longlong)iVar24 + (longlong)(iVar24 / 2);
                    uVar10 = (uint)(int)(lVar22 >> 6);
                    lVar22 = (longlong)(int)uVar11 * (longlong)iVar7 + (longlong)(iVar7 / 2);
                    uVar11 = (uint)(int)(lVar22 >> 6);
                }
                if ((int)uVar10 < (int)uVar5) {
                    uVar21 = (ulonglong)uVar10;
                }
                if ((int)uVar11 < (int)uVar6) {
                    uVar19 = (ulonglong)uVar11;
                }
                goto LAB_1400985e6;
            }
            uVar21 = (ulonglong)local_70.rc.left;
            uVar15 = (ulonglong)local_b8;
            if (cVar4 < '\x01') {
                uVar21 = (longlong)((longlong)(iVar24 / 2) + uVar21 * 100) / (longlong)iVar24;
            }
        }
    }
    uVar16 = 0;
    if (*local_90 != L'\0') {
        PECMD_AppendLongDecimal(param_1, uVar21 & 0xffffffff, local_90);
    }
    if (*local_c8 != L'\0') {
        PECMD_AppendLongDecimal(param_1, uVar19 & 0xffffffff, local_c8);
    }
    *local_c8 = L'\0';
    PECMD_ParseSkipSeparator((longlong *)local_res10, (longlong *)&local_c8, 0x2c, 0);
    if (*local_c8 != L'\0') {
        PECMD_AppendLongDecimal(param_1, uVar26 & 0xffffffff, local_c8);
    }
    *local_c8 = L'\0';
    PECMD_ParseSkipSeparator((longlong *)local_res10, (longlong *)&local_c8, 0x2c, 0);
    if (*local_c8 != L'\0') {
        PECMD_AppendLongDecimal(param_1, uVar15 & 0xffffffff, local_c8);
    }
    *local_c8 = L'\0';
    PECMD_ParseSkipSeparator((longlong *)local_res10, (longlong *)&local_c8, 0x2c, 0);
    if (*local_c8 != L'\0') {
        PECMD_AppendLongDecimal(param_1, (ulonglong)local_70.uEdge, local_c8);
    }
    *local_c8 = L'\0';
    PECMD_ParseSkipSeparator((longlong *)local_res10, (longlong *)&local_c8, 0x2c, 0);
    if (*local_c8 != L'\0') {
        PECMD_AppendLongDecimal(param_1, (ulonglong)local_b0.left, local_c8);
    }
    *local_c8 = L'\0';
    PECMD_ParseSkipSeparator((longlong *)local_res10, (longlong *)&local_c8, 0x2c, 0);
    if (*local_c8 != L'\0') {
        PECMD_AppendLongDecimal(param_1, (ulonglong)(uint32_t)local_98, local_c8);
    }
    *local_c8 = L'\0';
    PECMD_ParseSkipSeparator((longlong *)local_res10, (longlong *)&local_c8, 0x2c, 0);
    if (*local_c8 != L'\0') {
        PECMD_AppendLongDecimal(param_1, uVar13 & 0xffffffff, local_c8);
    }
    *local_c8 = L'\0';
    PECMD_ParseSkipSeparator((longlong *)local_res10, (longlong *)&local_c8, 0x2c, 0);
    if (*local_c8 != L'\0') {
        PECMD_AppendLongDecimal(param_1, uVar14 & 0xffffffff, local_c8);
    }
LAB_1400987b7:
    PECMD_FreeStrBuf((WCHAR **)&local_c8);
    PECMD_FreeStrBuf((WCHAR **)&local_90);
    return uVar16;
}
/* ================================================================
 * @0x140095178  (longlong PECMD_VolumeDeviceCommand(longlong*, LPCWSTR))
 * 卷 / 设备信息命令: setvol / idle / idlec / rsort / sort / devs /
 * link? / w, 以及盘符文件系统 (FAT/NTFS/...)、DosDevice、卷序列号等。
 * ================================================================ */
longlong PECMD_VolumeDeviceCommand(longlong *param_1, LPCWSTR param_2)
{
    LPWSTR pWVar1;
    char cVar2;
    char cVar3;
    BOOL BVar4;
    DWORD DVar5;
    int iVar6;
    int iVar7;
    undefined8 uVar8;
    uint *puVar9;
    WCHAR *pWVar10;
    longlong lVar11;
    WCHAR *pWVar12;
    WCHAR WVar13;
    LPCWSTR *ppWVar14;
    WCHAR *pWVar15;
    longlong lVar16;
    HANDLE pvVar17;
    LPCWSTR pWVar18;
    LPCWSTR pWVar19;
    char cVar20;
    LPCWSTR pWVar21;
    uint uVar22;
    uint uVar23;
    int iVar24;
    WCHAR *local_res10;
    uint64_t local_res20;
    uint local_1128;
    DWORD local_1124;
    WCHAR *local_1120;
    DWORD local_1118[2];
    uint64_t local_1110;
    uint64_t local_1108;
    WCHAR *local_1100;
    WCHAR *local_10f8;
    WCHAR local_10f0[5];
    WCHAR local_10e6;
    WCHAR local_10e4;
    uint64_t local_10e0;
    WCHAR *local_10d8;
    uint64_t local_10d0;
    WCHAR *local_10c8;
    uint local_10c0[2];
    HANDLE local_10b8;
    WCHAR *local_10b0;
    WCHAR *local_10a8;
    WCHAR *local_10a0;
    WCHAR *local_1098;
    WCHAR *local_1090;
    WCHAR local_1088[28];
    WCHAR local_1050[10];
    WCHAR local_103c[8];
    uint8_t auStack_102c[4076];

    local_res10 = (WCHAR *)param_2;
    PECMD_SkipLeadingControlChars(&local_res10);
    PECMD_AllocWStringBuffer((WCHAR **)&local_1120, 5);
    PECMD_AllocWStringBuffer((WCHAR **)&local_10d8, 0x14);
    PECMD_AllocStrSlot(&local_10f8);
    pWVar21 = (LPCWSTR)0x0;
    local_10c0[0] = 2;
    local_10c8 = (WCHAR *)0x0;
    uVar23 = 0;
    local_1128 = 0;
    iVar6 = 0;
    local_res20 = 0;
    local_1118[0] = 0;
    local_1110 = 0;
    local_1124 = 0;
    local_10e0 = 0;
    uVar22 = 0;
    cVar2 = '*';
    if (*local_res10 == L'-') {
        cVar2 = (char)*local_res10;
    }
    cVar20 = '\x01';
    local_1108 = 0x200;
    pWVar18 = pWVar21;
    if ((uint)(int)cVar2 == (uint)(ushort)*local_res10) {
        do {
            local_res10 = local_res10 + 1;
            uVar8 = PECMD_MatchPrefixN((ushort *)&g_wsz28038, (undefined8 *)&local_res10, 3);
            if ((int)uVar8 == 0) {
                uVar8 = PECMD_MatchPrefixN((ushort *)WSTR("setvol"), (undefined8 *)&local_res10, 6);
                if ((int)uVar8 == 0) {
                    uVar8 =
                        PECMD_MatchPrefixN((ushort *)WSTR("idle"), (undefined8 *)&local_res10, 4);
                    if ((int)uVar8 == 0) {
                        uVar8 = PECMD_MatchPrefixN((ushort *)WSTR("idlec"),
                                                   (undefined8 *)&local_res10, 5);
                        if ((int)uVar8 == 0) {
                            pWVar19 = local_res10;
                            uVar8 = PECMD_AsciiPrefixICmp("idlec:", (ushort *)local_res10, 6);
                            if ((char)uVar8 == '\0') {
                                uVar8 = PECMD_AsciiPrefixICmp("idle:", (ushort *)pWVar19, 5);
                                if ((char)uVar8 == '\0') {
                                    uVar8 = PECMD_MatchPrefixN((ushort *)WSTR("rsort"),
                                                               (undefined8 *)&local_res10, 5);
                                    if ((int)uVar8 == 0) {
                                        uVar8 = PECMD_MatchPrefixN((ushort *)WSTR("sort"),
                                                                   (undefined8 *)&local_res10, 4);
                                        if ((int)uVar8 == 0) {
                                            uVar8 =
                                                PECMD_MatchPrefixN((ushort *)&g_wsz27fd0,
                                                                   (undefined8 *)&local_res10, 2);
                                            if ((int)uVar8 == 0) {
                                                uVar8 = PECMD_MatchPrefixN(
                                                    (ushort *)&g_wsz27fc8,
                                                    (undefined8 *)&local_res10, 3);
                                                if ((int)uVar8 == 0) {
                                                    uVar8 = PECMD_MatchPrefixN(
                                                        (ushort *)WSTR("devs"),
                                                        (undefined8 *)&local_res10, 4);
                                                    if ((int)uVar8 == 0) {
                                                        cVar3 = PECMD_MatchTokenAdvance(
                                                            "link?", &local_res10, 5);
                                                        if (cVar3 == '\0') {
                                                            cVar3 = PECMD_MatchTokenAdvance(
                                                                "w", &local_res10, 1);
                                                            if (cVar3 == '\0') {
                                                                WVar13 = *local_res10;
                                                                while ((WVar13 != L'\0' &&
                                                                        (((uVar23 = local_1128,
                                                                           (ushort)WVar13 < 9 ||
                                                                               (0xd <
                                                                                (ushort)WVar13)) &&
                                                                          (WVar13 != L' '))))) {
                                                                    local_res10 = local_res10 + 1;
                                                                    WVar13 = *local_res10;
                                                                }
                                                                PECMD_SkipLeadingControlChars(
                                                                    &local_res10);
                                                            }
                                                            else {
                                                                uVar22 = 0x200;
                                                            }
                                                        }
                                                        else {
                                                            local_10e0 = (local_10e0 &
                                                                          0xffffffff00000000ULL) |
                                                                         1;
                                                        }
                                                    }
                                                    else {
                                                        local_1124 = 2;
                                                    }
                                                }
                                                else {
                                                    local_1124 = 1;
                                                }
                                            }
                                            else {
                                                local_1110 =
                                                    (local_1110 & 0xffffffff00000000ULL) | 0x100;
                                            }
                                        }
                                        else {
                                            local_1118[0] = 1;
                                        }
                                    }
                                    else {
                                        local_1118[0] = 0xff;
                                    }
                                    goto LAB_140095579;
                                }
                                iVar6 = 1;
                            }
                            else {
                                iVar6 = 2;
                                pWVar19 = pWVar19 + 1;
                            }
                            pWVar18 = pWVar19 + 5;
                            WVar13 = *pWVar18;
                            local_res10 = (WCHAR *)pWVar18;
                            while (WVar13 != L'\0') {
                                if (((8 < (ushort)WVar13) && ((ushort)WVar13 < 0xe)) ||
                                    (WVar13 == L' ')) {
                                    if (*local_res10 != L'\0') {
                                        *((LPWSTR)local_res10) = L'\0';
                                        local_res10 = local_res10 + 1;
                                    }
                                    break;
                                }
                                local_res10 = local_res10 + 1;
                                WVar13 = *local_res10;
                            }
                            PECMD_SkipLeadingControlChars(&local_res10);
                            if (*pWVar18 == L'\0') {
                                pWVar18 = pWVar21;
                            }
                        }
                        else {
                            iVar6 = 2;
                        }
                    }
                    else {
                        iVar6 = 1;
                    }
                }
                else {
                    local_1128 = local_10c0[0];
                    uVar23 = local_10c0[0];
                }
            }
            else {
                local_1128 = 1;
                uVar23 = 1;
            }
        LAB_140095579:
            local_res20 = (local_res20 & 0xffffffff00000000ULL) | (uint32_t)iVar6;
            local_10c8 = (WCHAR *)pWVar18;
            if ((uVar22 != 0) && (iVar6 != 0)) {
                uVar22 = uVar22 & -(uint)(pWVar18 != (LPCWSTR)0x0);
            }
        } while ((uint)(int)cVar2 == (uint)(ushort)*local_res10);
        local_res20 = (local_res20 & 0xffffffff00000000ULL) | (uint32_t)iVar6;
        local_10c8 = (WCHAR *)pWVar18;
        if ((uVar22 != 0) && (iVar6 != 0)) {
            uVar22 = uVar22 & -(uint)(pWVar18 != (LPCWSTR)0x0);
        }
    }
    PECMD_SplitTokenTrimWs((longlong *)&local_res10, (longlong *)&local_1120, 0x3d);
    PECMD_RunCommandLine(param_1, &local_1120, 0);
    pWVar18 = local_res10;
    if (*local_res10 == L'=') {
        pWVar18 = local_res10 + 1;
        local_res10 = (WCHAR *)pWVar18;
        PECMD_SplitTokenTrimWs((longlong *)&local_res10, (longlong *)&local_10d8, 0);
    }
    if (*local_1120 == L'\0') {
        PECMD_FreeStrBuf((WCHAR **)&local_10f8);
        PECMD_FreeStrBuf((WCHAR **)&local_10d8);
        PECMD_FreeStrBuf((WCHAR **)&local_1120);
        return -0x7ff8ffa9;
    }
    puVar9 = (uint *)FUN_140063060((undefined8)(uintptr_t)local_10c0);
    if (*puVar9 == uVar23) {
        PECMD_AllocString(&local_1120, 2);
        local_1120[1] = L':';
        BVar4 = SetVolumeLabelW(local_1120, pWVar18);
        if (BVar4 == 0) {
            DVar5 = GetLastError();
            pWVar21 = (LPCWSTR)(ulonglong)DVar5;
        }
    }
    else {
        if (*local_10d8 == 0x3f) {
            PECMD_AllocWStringBuffer((WCHAR **)&local_res20, 0x400010);
            pWVar12 = (WCHAR *)(uintptr_t)local_res20;
            pWVar10 = pWVar12 + 0x200008;
            pWVar10[0] = L'\0';
            pWVar10[1] = L'\0';
            pWVar10[2] = L'\0';
            pWVar10[3] = L'\0';
            pWVar12[0] = L'\0';
            pWVar12[1] = L'\0';
            pWVar12[2] = L'\0';
            pWVar12[3] = L'\0';
            DVar5 = (DWORD)QueryDosDeviceW((LPCWSTR)0x0, pWVar10, 0x1ffffe);
            pvVar17 = (HANDLE)(ulonglong)DVar5;
            pWVar1 = pWVar10 + (longlong)pvVar17;
            pWVar15 = pWVar12;
            if (pvVar17 != (HANDLE)0x0) {
                while (pWVar10 < pWVar1) {
                    for (; *pWVar10 != L'\0'; pWVar10 = pWVar10 + 1) {
                        *pWVar12 = *pWVar10;
                        pWVar12 = pWVar12 + 1;
                    }
                    pWVar10 = pWVar10 + 1;
                    pWVar15 = pWVar12;
                    if (*pWVar10 == L'\0')
                        break;
                    *pWVar12 = L'\r';
                    pWVar12[1] = L'\n';
                    pWVar12 = pWVar12 + 2;
                }
            }
            *pWVar12 = L'\0';
            local_10b8 = pvVar17;
            PECMD_SetVariable(param_1, local_1120, pWVar15);
            if (pvVar17 == (HANDLE)0x0) {
                DVar5 = GetLastError();
                pWVar21 = (LPCWSTR)(ulonglong)DVar5;
            }
            ppWVar14 = (LPCWSTR *)&local_res20;
        }
        else {
            pWVar10 = PECMD_UnquoteString(local_10d8);
            if ((uint32_t)local_10e0 == 0) {
                pWVar12 = (WCHAR *)PECMD_StripDevicePrefix((longlong)(uintptr_t)pWVar10);
                PECMD_AllocWStringBuffer((WCHAR **)&local_1100, 0x517);
                pWVar1 = local_1100 + 0x209;
                if (local_1124 == 0) {
                    if ((local_1128 & 1) == 0) {
                        local_res10 = pWVar12;
                        if (*pWVar12 == L'\0') {
                            cVar2 = '|';
                        }
                        else {
                            uVar8 = PECMD_AsciiWideICmp("*:", (ushort *)pWVar12);
                            cVar20 = '\x01';
                            if ((char)uVar8 == '\0') {
                                uVar8 = PECMD_AsciiWideICmp("*", (ushort *)pWVar12);
                                if ((char)uVar8 == '\0') {
                                    if (pWVar12[1] != L':') {
                                        local_1100[0x30e] = L'\0';
                                        GetCurrentDirectoryW(0x208, local_1100 + 0x30e);
                                        pWVar12 = local_res10;
                                        if ((((*local_res10 == L'\\') &&
                                              (local_res10[1] == L'\\')) &&
                                             (local_res10[2] == L'?')) &&
                                            (local_res10[3] == L'\\')) {
                                            local_res10 = local_res10 + 4;
                                            WVar13 = *local_res10;
                                            if (WVar13 != L'\0') {
                                                do {
                                                    if (WVar13 == L'\\')
                                                        break;
                                                    local_res10 = local_res10 + 1;
                                                    WVar13 = *local_res10;
                                                } while (WVar13 != L'\0');
                                                if (*local_res10 != L'\0') {
                                                    *((LPWSTR)local_res10) = L'\0';
                                                }
                                            }
                                            goto LAB_1400966dc;
                                        }
                                        PECMD_AllocString(&local_10f8, 5);
                                        *local_10f8 = local_1100[0x30e];
                                        local_10f8[1] = L':';
                                        pWVar12 = local_10f8;
                                    }
                                    pWVar12[2] = L'\0';
                                    goto LAB_1400966dc;
                                }
                                cVar20 = '\0';
                            }
                            cVar2 = ' ';
                        }
                        pWVar12 = (WCHAR *)PECMD_ListDrives(
                            (undefined8 *)&local_10f8, (WCHAR)cVar2, (int)cVar20, (int)local_res20,
                            uVar22 | (uint)(uintptr_t)local_1110 | local_1118[0], local_10c8);
                    LAB_1400966dc:
                        PECMD_SetVariable(param_1, local_1120, pWVar12);
                        PECMD_FreeStrBuf((WCHAR **)&local_1100);
                        PECMD_FreeStrBuf((WCHAR **)&local_10f8);
                        PECMD_FreeStrBuf((WCHAR **)&local_10d8);
                        PECMD_FreeStrBuf((WCHAR **)&local_1120);
                        return 0;
                    }
                    local_1100[0] = L'\0';
                    local_1100[1] = L'\0';
                    local_1100[2] = L'\0';
                    local_1100[3] = L'\0';
                    pWVar1[0] = L'\0';
                    pWVar1[1] = L'\0';
                    pWVar1[2] = L'\0';
                    pWVar1[3] = L'\0';
                    WVar13 = *pWVar12;
                    if ((WVar13 != L'\0') && ((pWVar12[1] == L'\0' ||
                                               ((pWVar12[1] == L':' && (pWVar12[2] == L'\0')))))) {
                        pWVar12 = local_10f0;
                        local_10f0[1] = 0x3a;
                        local_10f0[2] = 0x5c;
                        local_10f0[3] = 0;
                        local_10f0[0] = WVar13;
                    }
                    local_10d0 = 0;
                    local_1124 = 0;
                    local_1118[0] = 0;
                    local_10e0 = (local_10e0 & 0xffffffff00000000ULL) | 1;
                    iVar6 = StrCmpNIW(pWVar12, WSTR("\\\\?\\HarddiskVolume"), 0xc);
                    if (iVar6 != 0) {
                        BVar4 =
                            GetVolumeInformationW(pWVar12, local_1100, 0x208, (LPDWORD)&local_10d0,
                                                  &local_1124, local_1118, pWVar1, 0x104);
                        if (BVar4 != 0) {
                            local_10e0 = (uint64_t)(uint32_t)local_10e0;
                            local_10e0 = ((uint64_t)0 << 32) | 0;
                            goto LAB_140095d85;
                        }
                        local_10e0 = (local_10e0 & 0xffffffff00000000ULL) | GetLastError();
                        if ((uint32_t)local_10e0 == 0)
                            goto LAB_140095d85;
                    }
                    pWVar1[0] = L'\0';
                    pWVar1[1] = L'\0';
                    pWVar1[2] = L'\0';
                    pWVar1[3] = L'\0';
                    local_1118[0] = 0;
                    local_1124 = 0;
                    local_10d0 = 0;
                    PECMD_GetApiProcCached("GetVolumeInformationByHandleW", "Kernel32.DLL",
                                           (void **)&g_pGetVolumeInformationByHandleW,
                                           &g_hKernel32);
                    if (g_pGetVolumeInformationByHandleW !=
                        (DWORD (*)(HANDLE, LPWSTR, DWORD, LPDWORD))0) {
                        local_res20 = 0;
                        PECMD_OpenFileHandle(&local_res20, pWVar10, 0x80000000, 7,
                                             (LPSECURITY_ATTRIBUTES)0x0, 3, 0, (HANDLE)0x0);
                        pWVar21 = (LPCWSTR)(uintptr_t)local_res20;
                        iVar6 = (int)(*g_pGetVolumeInformationByHandleW)(
                            (HANDLE)(uintptr_t)local_res20, local_1100, 0x208,
                            (LPDWORD)&local_10d0);
                        if (iVar6 == 0) {
                            DVar5 = GetLastError();
                            local_10e0 = (local_10e0 & 0xffffffff00000000ULL) | DVar5;
                        }
                        else {
                            local_10e0 = local_10e0 & 0xffffffff00000000ULL;
                        }
                        if ((pWVar21 != (LPCWSTR)0x0) &&
                            ((uintptr_t)pWVar21 != 0xffffffffffffffff)) {
                            CloseHandle((HANDLE)(uintptr_t)pWVar21);
                        }
                    }
                LAB_140095d85:
                    local_res10 = local_1120;
                    PECMD_AllocStrSlot(&local_10c8);
                    PECMD_AllocStrSlot(&local_10a0);
                    PECMD_AllocStrSlot(&local_1098);
                    PECMD_AllocStrSlot(&local_10a8);
                    PECMD_AllocStrSlot(&local_10b0);
                    PECMD_AllocStrSlot(&local_1090);
                    PECMD_SplitTokenTrimWs((longlong *)&local_res10, (longlong *)&local_10c8, 0x2c);
                    if (*local_res10 != L'\0') {
                        local_res10 = local_res10 + 1;
                        PECMD_SplitTokenTrimWs((longlong *)&local_res10, (longlong *)&local_10a8,
                                               0x2c);
                        if (*local_res10 != L'\0') {
                            local_res10 = local_res10 + 1;
                            PECMD_SplitTokenTrimWs((longlong *)&local_res10,
                                                   (longlong *)&local_10a0, 0x2c);
                            if (*local_res10 != L'\0') {
                                local_res10 = local_res10 + 1;
                                PECMD_SplitTokenTrimWs((longlong *)&local_res10,
                                                       (longlong *)&local_1098, 0x2c);
                                if (*local_res10 != L'\0') {
                                    local_res10 = local_res10 + 1;
                                    PECMD_SplitTokenTrimWs((longlong *)&local_res10,
                                                           (longlong *)&local_10b0, 0x2c);
                                    if (*local_res10 != L'\0') {
                                        local_res10 = local_res10 + 1;
                                        PECMD_SplitTokenTrimWs((longlong *)&local_res10,
                                                               (longlong *)&local_1090, 0x2c);
                                    }
                                }
                            }
                        }
                    }
                    if (*local_1090 != L'\0') {
                        iVar24 = -1;
                        local_1128 = 1;
                        local_10e0 = local_10e0 & 0xffffffff00000000ULL;
                        local_1110 = (uint64_t)(uintptr_t)WSTR("%02X%02X-%02X%02X");
                        local_1088[0] = L'\0';
                        local_103c[0] = L'\0';
                        local_103c[1] = L'\0';
                        local_103c[2] = L'\0';
                        local_103c[3] = L'\0';
                        iVar6 = lstrcmpW(WSTR("FAT"), (LPCWSTR)local_1100 + 0x209);
                        if ((iVar6 == 0) ||
                            (iVar6 = lstrcmpW(WSTR("FAT16"), (LPCWSTR)local_1100 + 0x209),
                             iVar6 == 0)) {
                            iVar24 = 0x27;
                        }
                        else {
                            iVar6 = lstrcmpW(WSTR("FAT32"), (LPCWSTR)local_1100 + 0x209);
                            if (iVar6 == 0) {
                                iVar24 = 0x43;
                            }
                            else {
                                iVar6 = lstrcmpiW(WSTR("ExFAT"), (LPCWSTR)local_1100 + 0x209);
                                if (iVar6 == 0) {
                                    iVar24 = 100;
                                }
                                else {
                                    iVar6 = lstrcmpW(WSTR("NTFS"), (LPCWSTR)local_1100 + 0x209);
                                    if (iVar6 == 0) {
                                        iVar24 = 0x48;
                                        local_1128 = 2;
                                        local_1110 = (uint64_t)(uintptr_t)WSTR(
                                            "%02X%02X%02X%02X%02X%02X%02X%02X");
                                    }
                                    else {
                                        iVar6 =
                                            StrCmpNIW(WSTR("EXT2"), (LPCWSTR)local_1100 + 0x209, 3);
                                        if ((iVar6 == 0) &&
                                            (local_1100[0x20c] == L'9' ||
                                             (local_1100[0x20c] + (WCHAR)0xff10) < 9)) {
                                            iVar24 = 0x468;
                                            local_1128 = 3;
                                            local_1110 = (uint64_t)(uintptr_t)WSTR(
                                                "%02x%02x%02x%02x-%02x%02x-%02x%02x-%02x%02x-%02x%"
                                                "02x%02x%02x%02x%02x");
                                        }
                                        else {
                                            iVar6 =
                                                lstrcmpW(WSTR("CDFS"), (LPCWSTR)local_1100 + 0x209);
                                            if ((iVar6 == 0) ||
                                                ((iVar6 = lstrcmpW(WSTR("UDF"),
                                                                   (LPCWSTR)local_1100 + 0x209),
                                                  iVar6 == 0 ||
                                                      (iVar6 = StrCmpNIW(
                                                           WSTR("ISO9600"),
                                                           (LPCWSTR)local_1100 + 0x209, 3),
                                                       iVar6 == 0)))) {
                                                local_1108 = 0x800;
                                                iVar24 = 0x833e;
                                                local_1110 = (uint64_t)(uintptr_t)WSTR(
                                                    "%02x%02x-%02x-%02x-%02x-%02x-%02x-%02x");
                                                local_1128 = 4;
                                            }
                                        }
                                    }
                                }
                            }
                        }
                        iVar6 = lstrlenW(pWVar12);
                        iVar7 = iVar6 + -1;
                        if (0 < iVar7) {
                            if (pWVar12[iVar7] == L'\\') {
                                pWVar12[iVar7] = L'\0';
                                iVar7 = iVar6 + -2;
                            }
                            if ((0 < iVar7) && (pWVar12[1] == L':')) {
                                local_10f0[4] = *pWVar12;
                                pWVar12 = local_10f0;
                                memcpy(local_10f0, WSTR("\\\\.\\"), 8);
                                local_10e6 = 0x3a;
                                local_10e4 = 0;
                                (void)local_10e6;
                                (void)local_10e4;
                            }
                        }
                        local_10b8 = (HANDLE)0x0;
                        PECMD_OpenFileHandle(&local_10b8, pWVar12, 0x80000000, 7,
                                             (LPSECURITY_ATTRIBUTES)0x0, 3, 0x80, (HANDLE)0x0);
                        pvVar17 = local_10b8;
                        DVar5 = (DWORD)(uintptr_t)local_1108;
                        if ((int)(uintptr_t)local_1108 <= iVar24) {
                            PECMD_SetFilePointer(
                                local_10b8,
                                PECMD_LI((longlong)(int)((iVar24 /
                                                                (int)(uintptr_t)local_1108) *
                                                               (uintptr_t)local_1108)),
                                0);
                        }
                        ReadFile(pvVar17, local_103c, DVar5, (LPDWORD)&local_10e0, (void *)0x0);
                        GetLastError();
                        if ((uint32_t)local_10e0 != 0) {
                            lVar11 = (longlong)(iVar24 % (int)DVar5);
                            pWVar12 = (WCHAR *)((longlong)(uintptr_t)local_103c + lVar11);
                            local_1108 = (uint64_t)(uintptr_t)pWVar12;
                            if (local_1128 == 4) {
                                local_1108 = (uint64_t)(uintptr_t)local_1050;
                                auStack_102c[lVar11] = 0;
                                (void)auStack_102c[lVar11];
                                memset(local_1050, 0, 8);
                                PECMD_HexStringToBytes((byte *)pWVar12, (byte *)local_1050);
                                pWVar12 = local_1050;
                            }
                            if (local_1128 < 2) {
                                wsprintfW(local_1088, (LPCWSTR)(uintptr_t)local_1110,
                                          (ulonglong) *
                                              (byte *)((longlong)(uintptr_t)local_1108 + 3),
                                          (ulonglong)(byte)pWVar12[1]);
                            }
                            else if (local_1128 == 2) {
                                wsprintfW(local_1088, (LPCWSTR)(uintptr_t)local_1110,
                                          (ulonglong) *
                                              (byte *)((longlong)(uintptr_t)local_1108 + 7),
                                          (ulonglong)(byte)pWVar12[3]);
                            }
                            else if (local_1128 == 3) {
                                wsprintfW(local_1088, (LPCWSTR)(uintptr_t)local_1110,
                                          (ulonglong)(byte) * (WCHAR *)(uintptr_t)local_1108,
                                          (ulonglong) * (byte *)((longlong)(uintptr_t)pWVar12 + 1));
                            }
                            else if (local_1128 == 4) {
                                wsprintfW(local_1088, (LPCWSTR)(uintptr_t)local_1110,
                                          (ulonglong)(byte) * (WCHAR *)(uintptr_t)local_1108,
                                          (ulonglong) * (byte *)((longlong)(uintptr_t)pWVar12 + 1));
                            }
                        }
                        PECMD_SetVariable(param_1, local_1090, local_1088);
                        if ((pvVar17 != (HANDLE)0x0) &&
                            ((uintptr_t)pvVar17 != 0xffffffffffffffff)) {
                            CloseHandle(pvVar17);
                        }
                    }
                    PECMD_SetVariable(param_1, local_10c8, local_1100);
                    PECMD_FormatSetVar(param_1, (ulonglong)(uint32_t)local_10d0, local_10a0,
                                       WSTR("%08lX"));
                    PECMD_VarSetUInt(param_1, (ulonglong)local_1124, local_1098);
                    PECMD_FormatSetVar(param_1, (ulonglong)local_1118[0], local_10b0,
                                       WSTR("0x%lX"));
                    PECMD_SetVariable(param_1, local_10a8, (LPCWSTR)local_1100 + 0x209);
                    PECMD_FreeStrBuf((WCHAR **)&local_1090);
                    PECMD_FreeStrBuf((WCHAR **)&local_10b0);
                    PECMD_FreeStrBuf((WCHAR **)&local_10a8);
                    PECMD_FreeStrBuf((WCHAR **)&local_1098);
                    PECMD_FreeStrBuf((WCHAR **)&local_10a0);
                    PECMD_FreeStrBuf((WCHAR **)&local_10c8);
                    PECMD_FreeStrBuf((WCHAR **)&local_1100);
                    PECMD_FreeStrBuf((WCHAR **)&local_10f8);
                    PECMD_FreeStrBuf((WCHAR **)&local_10d8);
                    PECMD_FreeStrBuf((WCHAR **)&local_1120);
                    return (longlong)(int)(uint32_t)local_res20;
                }
                *local_1100 = L'\0';
                DVar5 = (DWORD)QueryDosDeviceW(pWVar12, local_1100, 0x517);
                if ((1 < (int)local_1124) && (*local_1100 != L'\0')) {
                    local_res10 = local_1100;
                    pWVar18 = pWVar21;
                    do {
                        do {
                            pWVar19 = pWVar18;
                            WVar13 = *local_res10;
                            local_res10 = local_res10 + 1;
                            pWVar18 = pWVar19;
                        } while (WVar13 != L'\0');
                        pWVar18 = (LPCWSTR)((longlong)pWVar19 + 1);
                    } while (*local_res10 != L'\0');
                    if (pWVar19 != (LPCWSTR)0x0 && 0 < (longlong)pWVar18) {
                        lVar16 = ((longlong)local_res10 - (longlong)local_1100) >> 1;
                        lVar11 = lVar16 + (longlong)pWVar18;
                        if (0x517 < lVar11) {
                            PECMD_AllocString(&local_1100, lVar11 + 2);
                        }
                        local_res10 = local_1100 + lVar16 + -2;
                        pWVar12 = (WCHAR *)local_res10 + (longlong)pWVar18;
                        *pWVar12 = L'\0';
                        while (local_res10 < pWVar12) {
                            do {
                                pWVar15 = pWVar12;
                                WVar13 = *local_res10;
                                *pWVar15 = WVar13;
                                pWVar12 = pWVar15 + -1;
                                local_res10 = local_res10 + -1;
                                if (WVar13 == L'\0')
                                    break;
                            } while (local_res10 < pWVar12);
                            *pWVar12 = L'\r';
                            *pWVar15 = L'\n';
                            pWVar12 = pWVar15 + -2;
                        }
                    }
                }
                PECMD_SetVariable(param_1, local_1120, local_1100);
                if (DVar5 == 0) {
                    DVar5 = GetLastError();
                    pWVar21 = (LPCWSTR)(ulonglong)DVar5;
                }
                ppWVar14 = (LPCWSTR *)&local_1100;
            }
            else {
                if ((*pWVar10 != L'\0') &&
                    ((pWVar10[1] == L'\0' || ((pWVar10[1] == L':' && (pWVar10[2] == L'\0')))))) {
                    memcpy(local_10f0, WSTR("\\??\\"), 8);
                    local_10f0[4] = *pWVar10;
                    local_10e6 = 0x3a;
                    local_10e4 = 0;
                    (void)local_10e6;
                    (void)local_10e4;
                    pWVar10 = local_10f0;
                }
                local_res10 = local_1120;
                PECMD_AllocStrSlot(&local_1110);
                PECMD_AllocStrSlot(&local_1100);
                PECMD_AllocStrSlot(&local_res20);
                PECMD_ParseSkipSeparator((longlong *)&local_res10, (longlong *)&local_1110, 0x2c,
                                         0);
                PECMD_ParseSkipSeparator((longlong *)&local_res10, (longlong *)&local_1100, 0x2c,
                                         0);
                PECMD_ParseSkipSeparator((longlong *)&local_res10, (longlong *)&local_res20, 0x2c,
                                         0);
                local_10e0 = 0;
                local_1108 = 0;
                local_10d0 = 0;
                lVar11 = PECMD_EnumNtSymbolicLink(
                    pWVar10,
                    (longlong *)(-(ulonglong)(*((WCHAR *)(uintptr_t)local_1110) != L'\0') &
                                 (ulonglong)&local_10e0),
                    (longlong *)(-(ulonglong)(*((WCHAR *)(uintptr_t)local_1100) != L'\0') &
                                 (ulonglong)&local_1108),
                    (longlong *)(-(ulonglong)(*((WCHAR *)(uintptr_t)local_res20) != L'\0') &
                                 (ulonglong)&local_10d0));
                if (*((WCHAR *)(uintptr_t)local_1110) != L'\0') {
                    PECMD_SetVariable(param_1, (LPCWSTR)(uintptr_t)local_1110,
                                      (LPCWSTR)(uintptr_t)local_10e0);
                }
                if (*((WCHAR *)(uintptr_t)local_1100) != L'\0') {
                    PECMD_SetVariable(param_1, (LPCWSTR)(uintptr_t)local_1100,
                                      (LPCWSTR)(uintptr_t)local_1108);
                }
                if (*((WCHAR *)(uintptr_t)local_res20) != L'\0') {
                    PECMD_SetVariable(param_1, (LPCWSTR)(uintptr_t)local_res20,
                                      (LPCWSTR)(uintptr_t)local_10d0);
                }
                if (lVar11 != 0) {
                    DVar5 = GetLastError();
                    pWVar21 = (LPCWSTR)(ulonglong)DVar5;
                }
                PECMD_FreeStrBuf((WCHAR **)&local_10d0);
                PECMD_FreeStrBuf((WCHAR **)&local_1108);
                PECMD_FreeStrBuf((WCHAR **)&local_10e0);
                PECMD_FreeStrBuf((WCHAR **)&local_res20);
                PECMD_FreeStrBuf((WCHAR **)&local_1100);
                ppWVar14 = (LPCWSTR *)&local_1110;
            }
        }
        PECMD_FreeStrBuf((WCHAR **)&ppWVar14);
    }
    PECMD_FreeStrBuf((WCHAR **)&local_10f8);
    PECMD_FreeStrBuf((WCHAR **)&local_10d8);
    PECMD_FreeStrBuf((WCHAR **)&local_1120);
    return (longlong)(int)(uintptr_t)pWVar21;
}

/* ---- R25-j 新增 extern (dc 直移依赖; 均引用全工程已有真体/桩, 定义处见行尾注) ---- */
extern LPWSTR PECMD_GuidToString(LPWSTR out, uint *guid, int braces);  /* @0x14005fc90 def core_b3_remaining.c:11176 (param_3!=0 加花括号) */
extern uint64_t *PECMD_GrowCapacityAligned(uint64_t *container, int64_t needed,
                                           int64_t align);             /* @0x1400e6cf8 def core_b8d.c:150 (容量按 align 对齐扩展) */
extern int PECMD_AnsiStrNCompare(char *a, longlong b, longlong n);     /* @0x14005b184 def restored_bodies.c:7212 (字节比较, 0=相等) */
extern uint8_t *PECMD_MemMoveForward(uint8_t *a, uint8_t *b, int n);   /* @0x14001d78c def restored_bodies.c:11567 (前向 memmove) */
extern void PECMD_TlsLogWrite(uint64_t ctx, const uint16_t *fmt, uint64_t a,
                              uint64_t b);                             /* @0x140018d8c def restored_bodies.c:4311 (调试日志) */
extern DWORD FUN_1400195F0(void *script, int64_t timeout, int maxmsg,
                           void *param4);                             /* @0x1400195f0 def core_thread.c:250 (消息等待) */
extern uint8_t *FUN_14001E69C(void *script, LPCWSTR name, void *scope,
                              int64_t len);                           /* @0x14001e69c (变量节点查找, 全工程已有调用) */
extern WCHAR *FUN_1400637DC(WCHAR **ps, LPCSTR src, int64_t srclen,
                            int64_t codepage);                        /* @0x1400637dc ANSI→宽 (4 参形态, 同 core_b3l.c:54 与 core_main.c:45;
                                                                         链接落在 core_exec5.c 3 参定义 = 既有全工程行为, 见报告) */
extern int64_t *PECMD_InitStrContainer(int64_t *ps, LPCWSTR src, int64_t len,
                                       uint64_t cap);                 /* @0x140070294 def core_b3a.c:357 (清槽后 WideToAnsiStr) */
extern uint8_t DAT_1401210f8[8];                                       /* dc L"\r\n" .rdata 槽 (0x1401210f8; 现为 unimplemented_stubs.c:460 零桩, 内容差记报告) */

/* ---- WLAN API 实参形态 typedef (仅本函数调用点就地换型) ----
 * dc 对 DAT_14013d7xx 槽的调用丢弃了若干尾部参数 (Ghidra 槽签名过窄);
 * 原版 EXE 反汇编证得实参数 (VA 见各调用点注), pecmd_globals.h 的槽声明
 * 本 TU 已经包含且类型不可改, 故按 C 标准对槽值做函数指针换型后调用。 */
typedef DWORD (*PECMD_FN_WlanOpenHandle)(DWORD, void *, DWORD *, HANDLE *);        /* 实调 4 参 @0x14009a686 */
typedef DWORD (*PECMD_FN_WlanSetProfile)(HANDLE, void *, DWORD, LPCWSTR, LPCWSTR,
                                         DWORD, void *, DWORD *);                  /* 实调 8 参 @0x14009b535 */
typedef DWORD (*PECMD_FN_WlanScan)(HANDLE, void *, void *, void *, void *);        /* 实调 5 参 @0x14009aaca */
typedef DWORD (*PECMD_FN_WlanGetAvailNetList)(HANDLE, void *, DWORD, void *,
                                              void **);                            /* 实调 5 参 @0x14009ab0c */
typedef void (*PECMD_FN_WlanFreeMemory)(void *);                                   /* 实调 1 参 @0x14009b6bc / 0x14009b6cf */

/* ---- R25-j: FUN_1400e4e6c @0x1400e4e6c size=37 (dc 直移, 工程未还原件, 补最小静态体) ----
 * RAS 拨号事件槽初始化: 挂函数指针表, 清两个同步事件句柄全局, 清槽 0x14f。 */
extern uint8_t PTR_FUN_14012b1c0[];                                    /* dc 函数指针表 (0x14012b1c0) */
extern HANDLE g_hSyncEvtA;                                             /* DAT_14013e1f8 (core_globals.c:167) */
extern HANDLE g_hSyncEvtB;                                             /* DAT_14013e200 (core_globals.c:168) */
static undefined8 *PECMD_InitEventSlots(undefined8 *param_1)
{
    *param_1 = (undefined8)(uintptr_t)&PTR_FUN_14012b1c0;              /* dc:FUN_1400e4e6c 首语句 */
    g_hSyncEvtA = (HANDLE)0;                                           /* dc: DAT_14013e1f8 = 0 */
    g_hSyncEvtB = (HANDLE)0;                                           /* dc: DAT_14013e200 = 0 */
    param_1[0x14f] = 0;                                                /* dc: param_1[0x14f] = 0 */
    return param_1;
}

/* ================================================================
 * @0x140099f18  (ulonglong PECMD_WlanManage(longlong*, LPCWSTR))
 * R25-j dc 直移体 (dc:96610-97468 FUN_140099f18 size=7183), 整体替换
 * 前语义级重写残体 (彼体缺 WLAN 后 2/3 分支)。
 * ADSL 动词处理器: "-wlan" 前缀路由 RAS(拨号/挂断/枚举) 与 WLAN
 * (接口枚举/扫描/连接/配置文件/可用网络列表) 双路径。
 * 直移纪律: 变量名沿用 dc (Ghidra 名); 逐段附 dc 行号; Ghidra 伪影
 * (变参丢弃/unaff 残留/CONCAT 拼接) 处理决策见 analysis/r25j_adsl_wlan_port.md。
 * 结构布局 (按 dc 访存偏移还原, 不引 wlanapi.h):
 *   WLAN_INTERFACE_INFO_LIST    { u32 NumberOfItems; u32 Index; INFO[]; }
 *   WLAN_INTERFACE_INFO (0x214) { GUID(0x10); WCHAR Description[256](@0x10);
 *                                 u32 State(@0x210); }   表基+8 起每项 0x214
 *   WLAN_AVAILABLE_NETWORK_LIST { u32 NumberOfItems; u32 Index; NET[]; }
 *   WLAN_AVAILABLE_NETWORK(0x274){ WCHAR ProfileName[256](@0); u32 SSIDLen(@0x200);
 *                                 u8 SSID[32](@0x204); u32 BssType(@0x224);
 *                                 u32 NumBssid(@0x228); BOOL Connectable(@0x22C);
 *                                 u32 Reason(@0x230); u32 NumPhyTypes(@0x234);
 *                                 u32 PhyTypes[8](@0x238); BOOL MorePhyTypes(@0x258);
 *                                 u32 SignalQuality(@0x25C); BOOL SecEnabled(@0x260);
 *                                 u32 Auth(@0x264); u32 Cipher(@0x268); u32 Flags(@0x26C); }
 * ================================================================ */
ulonglong PECMD_WlanManage(longlong *param_1, LPCWSTR param_2)
{
    WCHAR WVar1;
    bool bVar2;
    bool bVar3;
    bool bVar4;
    bool bVar5;
    uint64_t lpString;                       /* dc:96620 (ulonglong* 槽值形态) */
    LPWSTR pWVar6;
    char cVar7;
    bool bVar8;
    int iVar9;
    int iVar10;
    DWORD DVar11;
    undefined8 uVar12;
    longlong lVar13;
    wchar_t *pwVar14;
    LPWSTR *ppWVar15;
    WCHAR wVar16;
    uint uVar17;
    LPCWSTR *ppWVar18;
    LPCWSTR pWVar19;
    LPWSTR pWVar20;
    LPWSTR pWVar21;                          /* dc:96629 LPCWSTR, 但 dc:97328/97329 以其为 wsprintfW 目标, 按可变点声明 */
    char cVar22;
    LPWSTR pWVar23;
    short *psVar24;
    uint uVar25;
    undefined2 *puVar26;
    wchar_t *pwVar27;
    longlong lVar28;
    int iVar29;
    LPCWSTR pWVar30;
    wchar_t *pwVar31;
    ulonglong uVar32;
    ulonglong uVar33;
    uint uVarRow;                            /* R25-j: EXE r15d 行分隔计数 (dc:97081 变参丢弃, 恢复自 0x14009a94e) */
    LPWSTR pWVar34;
    LPCWSTR pWVar35;
    uint64_t local_res10;                    /* dc:96651 解析游标(低32兼解析出参槽, dc:96964 显式半清零) */
    uint64_t local_res20;                    /* dc:96652 多角色: list 模式值 / ANSI 串槽 */
    char local_c57;                          /* dc:96653 排序模式 '\x01'/'\x02'/'\x10' */
    char local_c53;                          /* dc:96654 '?' 前缀标记 */
    wchar_t *local_c50;                      /* dc:96655 */
    uint local_c48[2];                       /* dc:96656 */
    WCHAR *local_c40;                        /* dc:96657 SSID 宽文本槽 */
    uint64_t local_c38;                      /* dc:96658 query/queryall 标志字节 / ANSI token1 槽 */
    WCHAR *local_c30;                        /* dc:96659 token3 槽 */
    undefined1 local_c28;                    /* dc:96660 RAS API 加载出参 */
    char local_c27;                          /* dc:96661 "-wlan" 命中 */
    int *local_c20;                          /* dc:96662 WLAN_INTERFACE_INFO_LIST* */
    uint64_t local_c18;                      /* dc:96663 宽字符名槽 */
    WCHAR *local_c10;                        /* dc:96664 变量名槽 */
    uint64_t local_c08;                      /* dc:96665 ANSI token2(密码) 槽 */
    LPWSTR local_c00;                        /* dc:96666 XML 头缓冲 */
    wchar_t *local_bf8;                      /* dc:96667 token1 槽 */
    LPWSTR local_bf0;                        /* dc:96668 "%s.PN" 变量名槽 */
    uint64_t local_be8;                      /* dc:96669 ssid 名长(低32) / ANSI token2 槽 */
    WCHAR *local_be0;                        /* dc:96670 */
    wchar_t *local_bd8;                      /* dc:96671 token2 槽 */
    uint32_t local_bd0;                      /* dc:96672 连接模式 */
    wchar_t *local_bc8;                      /* dc:96673 认证串 / RAS ANSI 槽 */
    wchar_t *local_bc0;                      /* dc:96674 输出行缓冲 */
    longlong local_bb8;                      /* dc:96675 已用字符数 */
    undefined8 local_bb0;                    /* dc:96676 (仅 dc:96710 初始化, 全文无读点) */
    LPWSTR local_ba8;                        /* dc:96677 sharedKey 块缓冲 / RAS ANSI 槽 */
    longlong local_ba0;                      /* dc:96678 hClientHandle */
    LPCWSTR local_b98;                       /* dc:96679 加密串 */
    LPWSTR local_b90;                        /* dc:96680 当前接口项 */
    WCHAR *local_b88;                        /* dc:96681 WLAN_AVAILABLE_NETWORK_LIST* */
    uint64_t local_b80;                      /* dc:96682 (dc 拆 local_b80+uStack_b7c 的 64 位槽) */
    uint32_t local_b78[4];                   /* dc:96684 协商版本出参 */
    LPWSTR local_b68;                        /* dc:96685 hex SSID 串 */
    ulonglong local_b60[5];                  /* dc:96686-96689 WLAN_CONNECTION_PARAMETERS 0x28 字节
                                                (EXE: [rsp+0x188]; dc 的 b60[2]+b50+b40+b3c 连续拼合) */
    uint8_t *local_b50;                      /* dc:96687 = 参数块 +0x10 pDot11Ssid */
    uint32_t local_b40;                      /* dc:96688 = 参数块 +0x20 BssType */
    uint32_t local_b3c;                      /* dc:96689 = 参数块 +0x24 dwFlags */
    uint8_t local_b38[48];                   /* dc:96690 DOT11_SSID {u32 len; u8 ssid[32];} */
    WCHAR local_b08[39];                     /* dc:96691-96692 GUID 串缓冲 (dc local_b08 + local_b06[9] 相邻合并;
                                                EXE 清零跨度 0x4c+首字符 = 39 字符, 恰容 "{GUID}" 39 字符含 NUL) */
    undefined8 local_ab8[336];               /* dc:96693 RAS 事件槽 */

    /* ---- 入口与词头 (dc:96695-96725) ---- */
    local_res10 = (uint64_t)(uintptr_t)param_2;                                       /* dc:96695 */
    PECMD_SkipLeadingControlChars((longlong *)&local_res10);                          /* dc:96696 */
    local_c27 = PECMD_MatchTokenAdvance("-wlan", (WCHAR **)&local_res10, 5);          /* dc:96697 */
    uVar33 = 0;                                                                       /* dc:96698 */
    pWVar34 = (LPWSTR)0x0;                                                            /* dc:96699 */
    iVar10 = 0;                                                                       /* dc:96700 */
    /* dc:96701 为 Ghidra 残留 (读未初始化高半再移位); 全文只用低 32 位, 以 0 定值 */
    local_be8 = 0;
    local_ba8 = (LPWSTR)0x0;                                                          /* dc:96702 */
    PECMD_AllocStrSlot(&local_bf8);                                                   /* dc:96703 */
    PECMD_AllocStrSlot(&local_bd8);                                                   /* dc:96704 */
    PECMD_AllocStrSlot(&local_bf0);                                                   /* dc:96705 */
    PECMD_AllocStrSlot(&local_c10);                                                   /* dc:96706 */
    PECMD_AllocStrSlot(&local_c30);                                                   /* dc:96707 */
    PECMD_AllocStrSlot(&local_bc0);                                                   /* dc:96708 */
    local_bb8 = 0;                                                                    /* dc:96709 */
    local_bb0 = 0;                                                                    /* dc:96710 */
    (void)local_bb0;
    uVar25 = 1;                                                                       /* dc:96711 */
    local_c48[0] = 1;                                                                 /* dc:96712 */
    bVar4 = false;                                                                    /* dc:96713 */
    /* dc:96714 Ghidra 残留 (低字节清零), 等价 0 定值 */
    local_c38 = 0;
    bVar3 = false;                                                                    /* dc:96715 */
    cVar22 = '\0';                                                                    /* dc:96716 */
    /* dc:96717 Ghidra 残留 (低字节清零), 等价 0 定值 */
    local_res20 = 0;
    bVar8 = false;                                                                    /* dc:96718 */
    bVar5 = false;                                                                    /* dc:96719 */
    bVar2 = false;                                                                    /* dc:96720 */
    local_c57 = '\x02';                                                               /* dc:96721 */
    pWVar30 = (LPCWSTR)(uintptr_t)local_res10;                                        /* dc:96722 */
    uVar12 = (undefined8)(int64_t)(int)PECMD_AsciiPrefixICmp("-ssid:", (ushort *)(uintptr_t)local_res10, 6);  /* dc:96723 */
    pWVar20 = local_bf0;                                                              /* dc:96724 */
    pWVar21 = local_c10;                                                              /* dc:96725 */
    if ((char)uVar12 != '\0') {                                                       /* dc:96726 */
        pWVar30 = pWVar30 + 6;                                                        /* dc:96727 */
        /* dc:96728-96737: 取 -ssid: 名至空白/控制符止, 就地 NUL 截断
         * (local_res10 为字节地址槽, dc 元素 +1 = 字节 +2) */
        for (local_res10 = (uint64_t)(uintptr_t)pWVar30;
             WVar1 = *(WCHAR *)(uintptr_t)local_res10, WVar1 != L'\0';
             local_res10 = local_res10 + 2) {
            if (((8 < (ushort)WVar1) && ((ushort)WVar1 < 0xe)) || (WVar1 == L' ')) {
                if (*(WCHAR *)(uintptr_t)local_res10 != L'\0') {
                    *(WCHAR *)(uintptr_t)local_res10 = L'\0';
                    local_res10 = local_res10 + 2;
                }
                break;
            }
        }
        PECMD_SkipLeadingControlChars((longlong *)&local_res10);                      /* dc:96738 */
        iVar9 = lstrlenW(pWVar30);                                                    /* dc:96739 */
        local_be8 = (local_be8 & 0xffffffff00000000ULL) | (uint64_t)(uint32_t)iVar9;  /* dc:96740 */
        PECMD_AllocString(&local_bf0, (longlong)(iVar9 + 0x68));                      /* dc:96741 */
        pWVar20 = local_bf0;                                                          /* dc:96742 */
        wsprintfW(local_bf0, WSTR("%s.PN"), pWVar30);                                 /* dc:96743 */
        PECMD_AllocString(&local_c10, (longlong)((int)local_be8 + 100));              /* dc:96744 */
        pWVar21 = local_c10;                                                          /* dc:96745 */
        wsprintfW(local_c10, WSTR("%s"), pWVar30);                                    /* dc:96746 */
    }
    cVar7 = PECMD_MatchTokenAdvance("-start", (WCHAR **)&local_res10, 6);             /* dc:96748 */
    local_bd0 = 0;                                                                    /* dc:96749 */
    if (cVar7 != '\0') {                                                              /* dc:96750 */
        local_bd0 = 0xe;                                                              /* dc:96751 */
    }
    PECMD_SplitTokenTrimWs((longlong *)&local_res10, (longlong *)&local_bf8, 0x2c);   /* dc:96753 */
    if (*(WCHAR *)(uintptr_t)local_res10 == L',') {                                   /* dc:96754 */
        local_res10 = local_res10 + 2;                                                /* dc:96755 (dc 元素 +1) */
        PECMD_SplitTokenTrimWs((longlong *)&local_res10, (longlong *)&local_bd8, 0x2c);  /* dc:96756 */
    }
    if (*(WCHAR *)(uintptr_t)local_res10 == L',') {                                   /* dc:96758 */
        local_res10 = local_res10 + 2;                                                /* dc:96759 (dc 元素 +1) */
        PECMD_SplitTokenTrimWs((longlong *)&local_res10, (longlong *)&local_bc0, 0x2c);  /* dc:96760 */
        /* ---- 动词解析: 问号 / 星号 / 双星号 / 减号 / 尖号 前缀与 stop/start/list/query/scan 词 (dc:96761-96858) ---- */
        local_c53 = '\0';                                                             /* dc:96761 */
        cVar22 = '\x02';                                                              /* dc:96762 */
        pwVar31 = local_bc0;                                                          /* dc:96763 */
        if (*local_bc0 == L'?') {                                                     /* dc:96764 */
            local_c53 = (char)*local_bc0;                                             /* dc:96765 */
            pwVar31 = local_bc0 + 1;                                                  /* dc:96766 */
        }
        cVar7 = '\x02';                                                               /* dc:96768 */
        if (*pwVar31 == L'*') {                                                       /* dc:96769 */
            if (pwVar31[1] == L'*') {                                                 /* dc:96770 */
                pwVar31 = pwVar31 + 2;                                                /* dc:96771 */
                cVar22 = '\x10';                                                      /* dc:96772 */
            }
            cVar7 = cVar22;                                                           /* dc:96773 */
            if (*pwVar31 == L'*') {                                                   /* dc:96775 */
                pwVar31 = pwVar31 + 1;                                                /* dc:96776 */
                cVar7 = '\0';                                                         /* dc:96777 */
            }
        }
        if (*pwVar31 == L'-') {                                                       /* dc:96780 */
            pwVar31 = pwVar31 + 1;                                                    /* dc:96781 */
            cVar7 = '\x01';                                                           /* dc:96782 */
        }
        if (*pwVar31 == L'^') {                                                       /* dc:96784 */
            pwVar31 = pwVar31 + 1;                                                    /* dc:96785 */
            cVar7 = '\x02';                                                           /* dc:96786 */
        }
        iVar9 = lstrcmpiW(WSTR("stop"), pwVar31);                                     /* dc:96788 */
        if (iVar9 == 0) {                                                             /* dc:96789 */
            bVar4 = true;                                                             /* dc:96790 */
        }
        else {
            iVar9 = lstrcmpiW(WSTR("start+"), pwVar31);                               /* dc:96793 */
            if (iVar9 == 0) {                                                         /* dc:96794 */
                local_bd0 = (local_bd0 & 0xffffff00U) | 6;                            /* dc:96795 */
            }
            else {
                iVar9 = StrCmpNIW(WSTR("start+"), pwVar31, 5);                        /* dc:96798 */
                if (iVar9 != 0) {                                                     /* dc:96799 */
                    iVar9 = lstrcmpiW(WSTR("list+"), pwVar31);                        /* dc:96800 */
                    if (iVar9 == 0) {                                                 /* dc:96801 */
                        local_res20 = (local_res20 & 0xffffffffffffff00ULL) | 3;      /* dc:96802 */
                    }
                    else {
                        iVar9 = lstrcmpiW(WSTR("list"), pwVar31);                     /* dc:96805 */
                        if (iVar9 != 0) {                                             /* dc:96806 */
                            iVar9 = lstrcmpiW(WSTR("queryall"), pwVar31);             /* dc:96807 */
                            if (iVar9 == 0) {                                         /* dc:96808 */
                                bVar3 = true;                                         /* dc:96809 */
                                local_c38 = (local_c38 & 0xffffffffffffff00ULL) | 8;  /* dc:96810 */
                            }
                            else {
                                iVar9 = lstrcmpiW(WSTR("query"), pwVar31);            /* dc:96813 */
                                if (iVar9 == 0) {                                     /* dc:96814 */
                                    local_c38 = (local_c38 & 0xffffffffffffff00ULL) | 1;  /* dc:96815 */
                                }
                                else {
                                    iVar9 = lstrcmpiW(WSTR("scan"), pwVar31);         /* dc:96818 */
                                    if (iVar9 == 0) {                                 /* dc:96819 */
                                        bVar2 = true;                                 /* dc:96820 */
                                    }
                                    else {
                                        iVar9 = lstrcmpiW(WSTR("liston"), pwVar31);   /* dc:96823 */
                                        if (iVar9 == 0) {                             /* dc:96824 */
                                            bVar8 = true;                             /* dc:96825 */
                                            bVar5 = true;                             /* dc:96826 */
                                            cVar22 = '\0';                            /* dc:96827 */
                                            goto LAB_14009a3f5;                       /* dc:96828 */
                                        }
                                        /* dc:96830: 其余词 → 解析重试次数入 local_c48 */
                                        PECMD_ParseShortStore((undefined8 *)&local_res10,
                                                              (int *)local_c48, 0x2c);
                                        uVar25 = local_c48[0];                        /* dc:96831 */
                                        if ((int)local_c48[0] < 1) {                  /* dc:96832 */
                                            uVar25 = 0;                               /* dc:96833 */
                                        }
                                    }
                                }
                            }
                            goto LAB_14009a3ea;                                       /* dc:96838 */
                        }
                        local_res20 = (local_res20 & 0xffffffffffffff00ULL) | 1;      /* dc:96840 */
                    }
                    cVar22 = (char)local_res20;                                       /* dc:96842 */
                    uVar17 = (uint)(uint8_t)local_c38;                                /* dc:96843 */
                    bVar8 = false;                                                    /* dc:96844 */
                    if (local_c53 != '\0') {                                          /* dc:96845 */
                        uVar17 = 1;                                                   /* dc:96846 */
                    }
                    local_c38 = (local_c38 & 0xffffffff00000000ULL) | (uint64_t)uVar17;  /* dc:96848 */
                    local_c57 = cVar7;                                                /* dc:96849 */
                    goto LAB_14009a3f5;                                               /* dc:96850 */
                }
                local_bd0 = (local_bd0 & 0xffffff00U) | 2;                            /* dc:96852 */
            }
        }
    LAB_14009a3ea:
        cVar22 = '\0';                                                                /* dc:96856 */
        bVar8 = false;                                                                /* dc:96857 */
    }
LAB_14009a3f5:
    if (*(WCHAR *)(uintptr_t)local_res10 == L',') {                                   /* dc:96860 */
        local_res10 = local_res10 + 2;                                                /* dc:96861 (dc 元素 +1) */
        PECMD_SplitTokenTrimWs((longlong *)&local_res10, (longlong *)&local_c30, 0x2c);  /* dc:96862 */
    }
    pWVar30 = local_c30;                                                              /* dc:96864 */
    if ((cVar22 != '\0') || (bVar8)) {                                                /* dc:96865 */
        PECMD_SetVariableWithPrefix(param_1, local_c30, (LPCWSTR)g_szEmpty);          /* dc:96866 (值 = &DAT_14011c638 空串) */
    }
    pwVar31 = local_bf8;                                                              /* dc:96868 */
    /* ---- RAS 半区 (未命中 -wlan): 枚举/拨号/挂断 (dc:96869-96962) ---- */
    if (local_c27 == '\0') {                                                          /* dc:96869 */
        PECMD_LoadRasApi((uint64_t)(uintptr_t)&local_c28);                            /* dc:96870 */
        uVar12 = PECMD_HasRasDialApi();                                               /* dc:96871 (CONCAT71 拼装 = RAX 低 32) */
        bVar8 = (char)uVar12 != '\0';
        uVar32 = 1;                                                                   /* dc:96872 */
        if ((int)uVar12 != 0) {                                                       /* dc:96873 */
            if (bVar5) {                                                              /* dc:96874 */
                PECMD_AllocStrSlot((WCHAR **)&local_res10);                           /* dc:96875 */
                PECMD_EnumRasConnections((longlong *)&local_res10);                   /* dc:96876 */
                PECMD_SetVariableWithPrefix(param_1, pWVar30, (LPCWSTR)(uintptr_t)local_res10);  /* dc:96877 */
                ppWVar18 = (LPCWSTR *)&local_res10;                                   /* dc:96878 */
            }
            else {
                if (cVar22 == '\0') {                                                 /* dc:96881 */
                    if (bVar4) {                                                      /* dc:96882 */
                        uVar33 = PECMD_FindRasConnection(pWVar30);                    /* dc:96883 */
                        uVar32 = (ulonglong)(int)uVar33;                              /* dc:96884 */
                    }
                    else {
                        /* dc:96887-96894: token1/token2(与 c30) ANSI 化, b64 出参槽备 */
                        local_c40 = (WCHAR *)0x0;
                        PECMD_WideToAnsiStr((longlong *)&local_c40, local_bf8, -1, 0xffffffffffffffff);  /* dc:96888 */
                        local_c00 = (LPWSTR)0x0;
                        PECMD_WideToAnsiStr((longlong *)&local_c00, local_bd8, -1, 0xffffffffffffffff);  /* dc:96890 */
                        local_c18 = 0;
                        PECMD_WideToAnsiStr((longlong *)&local_c18, pWVar30, -1, 0xffffffffffffffff);    /* dc:96892 */
                        PECMD_AllocSmallObject((undefined8 *)&local_res20);           /* dc:96893 */
                        PECMD_AllocSmallObject((undefined8 *)&local_res10);           /* dc:96894 */
                        lVar13 = PECMD_Base64Decode((byte *)(uintptr_t)local_c40, (undefined8 *)&local_res20);  /* dc:96895 */
                        if ((lVar13 != 0) &&
                            (lVar13 = PECMD_Base64Decode((byte *)(uintptr_t)local_c00,
                                                         (undefined8 *)&local_res10),  /* dc:96897 */
                             lVar13 != 0)) {
                            PECMD_AllocStrSlot((WCHAR **)&local_be0);                 /* dc:96898 */
                            PECMD_AllocStrSlot((WCHAR **)&local_b88);                 /* dc:96899 */
                            PECMD_StrBldCopyAnsi((longlong *)&local_ba8,
                                                 (LPCSTR)(uintptr_t)local_res20, 0xffffffffffffffff);  /* dc:96900 */
                            PECMD_StrBldCopyAnsi((longlong *)&local_bc8,
                                                 (LPCSTR)(uintptr_t)local_res10, 0xffffffffffffffff);  /* dc:96901 */
                            WVar1 = *pWVar30;                                         /* dc:96902 */
                            do {                                                      /* dc:96903 拨号重试环 */
                                uVar25 = uVar25 - 1;                                  /* dc:96904 */
                                if ((int)uVar25 < 0) {                                /* dc:96905 */
                                LAB_14009ba25:
                                    PECMD_ZeroLenBuf((void *)(uintptr_t)local_res20); /* dc:96907 */
                                    PECMD_ZeroLenBuf((void *)(uintptr_t)local_res10); /* dc:96908 */
                                    PECMD_FreeStrBuf((WCHAR **)&local_bc8);           /* dc:96909 */
                                    PECMD_FreeStrBuf((WCHAR **)&local_ba8);           /* dc:96910 */
                                    PECMD_FreeStrBuf((WCHAR **)&local_b88);           /* dc:96911 */
                                    PECMD_FreeStrBuf((WCHAR **)&local_be0);           /* dc:96912 */
                                    PECMD_FreeStrBuf((WCHAR **)&local_res10);         /* dc:96913 */
                                    PECMD_FreeStrBuf((WCHAR **)&local_res20);         /* dc:96914 */
                                    PECMD_FreeStrBuf((WCHAR **)&local_c18);           /* dc:96915 */
                                    PECMD_FreeStrBuf((WCHAR **)&local_c00);           /* dc:96916 */
                                    PECMD_FreeStrBuf((WCHAR **)&local_c40);           /* dc:96917 */
                                    PECMD_FreeStrBuf((WCHAR **)&local_bc0);           /* dc:96918 */
                                    PECMD_FreeStrBuf((WCHAR **)&local_c30);           /* dc:96919 */
                                    PECMD_FreeContainer((longlong *)&local_c10);      /* dc:96920 */
                                    PECMD_FreeContainer((longlong *)&local_bf0);      /* dc:96921 */
                                    PECMD_FreeContainer((longlong *)&local_bd8);      /* dc:96922 */
                                    PECMD_FreeContainer((longlong *)&local_bf8);      /* dc:96923 */
                                    return (ulonglong)(iVar10 == 0);                  /* dc:96924 */
                                }
                                PECMD_InitEventSlots(local_ab8);                      /* dc:96926 (FUN_1400e4e6c) */
                                uVar12 = PECMD_RasDialEntry(
                                    (longlong)(uintptr_t)local_ab8,                   /* dc:96927 */
                                    (LPCWSTR)(uintptr_t)(-(ulonglong)(WVar1 != L'\0') &
                                                         (ulonglong)(uintptr_t)pWVar30),  /* dc:96928 */
                                    (LPCWSTR)(uintptr_t)local_be0, (LPCWSTR)(uintptr_t)local_ba8,
                                    (LPCWSTR)(uintptr_t)local_bc8, (LPCWSTR)(uintptr_t)local_b88);      /* dc:96929 */
                                iVar10 = (int)uVar12;                                 /* dc:96930 */
                                if (iVar10 != 0) {                                    /* dc:96931 */
                                    PECMD_DestroyEventSlots(local_ab8);               /* dc:96932 */
                                    goto LAB_14009ba25;                               /* dc:96933 */
                                }
                                PECMD_DestroyEventSlots(local_ab8);                   /* dc:96935 */
                            } while (true);                                          /* dc:96936 */
                        }
                        PECMD_FreeStrBuf((WCHAR **)&local_res10);                     /* dc:96938 */
                        PECMD_FreeStrBuf((WCHAR **)&local_res20);                     /* dc:96939 */
                        PECMD_FreeStrBuf((WCHAR **)&local_c18);                       /* dc:96940 */
                        PECMD_FreeStrBuf((WCHAR **)&local_c00);                       /* dc:96941 */
                        PECMD_FreeStrBuf((WCHAR **)&local_c40);                       /* dc:96942 */
                        uVar32 = 0xffffffff80070057;                                  /* dc:96943 */
                    }
                    goto LAB_14009b90c;                                               /* dc:96945 */
                }
                PECMD_AllocStrSlot((WCHAR **)&local_res20);                           /* dc:96947 */
                PECMD_EnumPhonebookEntries((longlong *)&local_res20, 1);              /* dc:96948 */
                PECMD_SetVariableWithPrefix(param_1, pWVar30, (LPCWSTR)(uintptr_t)local_res20);  /* dc:96949 */
                ppWVar18 = (LPCWSTR *)&local_res20;                                   /* dc:96950 */
            }
            PECMD_FreeStrBuf((void *)ppWVar18);                                       /* dc:96952 */
            uVar32 = uVar33;                                                          /* dc:96953 */
        }
    LAB_14009b90c:
        PECMD_FreeStrBuf((WCHAR **)&local_bc0);                                       /* dc:96956 */
        PECMD_FreeStrBuf((WCHAR **)&local_c30);                                       /* dc:96957 */
        PECMD_FreeContainer((longlong *)&local_c10);                                  /* dc:96958 */
        PECMD_FreeContainer((longlong *)&local_bf0);                                  /* dc:96959 */
        PECMD_FreeContainer((longlong *)&local_bd8);                                  /* dc:96960 */
        PECMD_FreeContainer((longlong *)&local_bf8);                                  /* dc:96961 */
        return uVar32;                                                                /* dc:96962 */
    }
    /* ---- WLAN 半区入口: 解析接口号 (dc:96964-96988) ---- */
    local_res10 = local_res10 & 0xffffffff00000000ULL;                                /* dc:96964 (低半清零, 槽兼作解析出参) */
    local_c50 = local_bf8;                                                            /* dc:96965 */
    if (((cVar22 != '\0') || (bVar2)) || ((char)local_c38 != '\0')) {                 /* dc:96966 */
        local_c48[0] = 0;                                                             /* dc:96967 */
    }
    else {
        local_c48[0] = 1;                                                             /* dc:96970 */
        local_c50 = pWVar30;                                                          /* dc:96971 */
    }
    PECMD_ParseUIntValue((LPCWSTR *)&local_c50, (int *)&local_res10);                 /* dc:96973 */
    uVar32 = local_res10 & 0xffffffffULL;                                             /* dc:96974 */
    if (bVar3) {                                                                      /* dc:96975 */
        uVar32 = 0;                                                                   /* dc:96976 */
    }
    if ((int)uVar32 < 0) {                                                            /* dc:96978 */
        uVar33 = 0xffffffff80070057;                                                  /* dc:96979 */
    LAB_14009a630:
        PECMD_FreeStrBuf((WCHAR **)&local_bc0);                                       /* dc:96981 */
        PECMD_FreeStrBuf((WCHAR **)&local_c30);                                       /* dc:96982 */
        PECMD_FreeContainer((longlong *)&local_c10);                                  /* dc:96983 */
        PECMD_FreeContainer((longlong *)&local_bf0);                                  /* dc:96984 */
        PECMD_FreeContainer((longlong *)&local_bd8);                                  /* dc:96985 */
        PECMD_FreeContainer((longlong *)&local_bf8);                                  /* dc:96986 */
        return uVar33;                                                                /* dc:96987 */
    }
    /* ---- wlanapi 惰性加载 10 槽 (dc:96989-97006) ---- */
    if (g_pWlanOpenHandle == (DWORD (*)(DWORD, void *, DWORD *))0) {                  /* dc:96989 */
        PECMD_GetApiProcCached("WlanEnumInterfaces", "wlanapi.DLL",
                               (void **)&g_pWlanEnumInterfaces, (uintptr_t *)0x0);    /* dc:96990 */
        PECMD_GetApiProcCached("WlanSetProfile", "wlanapi.DLL",
                               (void **)&g_pWlanSetProfile, (uintptr_t *)0x0);        /* dc:96991 */
        PECMD_GetApiProcCached("WlanCloseHandle", "wlanapi.DLL",
                               (void **)&g_pWlanCloseHandle, (uintptr_t *)0x0);       /* dc:96992 */
        PECMD_GetApiProcCached("WlanRegisterNotification", "wlanapi.DLL",
                               (void **)&g_pWlanRegisterNotification, (uintptr_t *)0x0);  /* dc:96993 */
        PECMD_GetApiProcCached("WlanScan", "wlanapi.DLL",
                               (void **)&g_pWlanScan, (uintptr_t *)0x0);              /* dc:96995 */
        PECMD_GetApiProcCached("WlanGetAvailableNetworkList", "wlanapi.DLL",
                               (void **)&g_pWlanGetAvailableNetworkList, (uintptr_t *)0x0);  /* dc:96996 */
        PECMD_GetApiProcCached("WlanDisconnect", "wlanapi.DLL",
                               (void **)&g_pWlanDisconnect, (uintptr_t *)0x0);        /* dc:96998 */
        PECMD_GetApiProcCached("WlanFreeMemory", "wlanapi.DLL",
                               (void **)&g_pWlanFreeMemory, (uintptr_t *)0x0);        /* dc:96999 */
        PECMD_GetApiProcCached("WlanConnect", "wlanapi.DLL",
                               (void **)&g_pWlanConnect, (uintptr_t *)0x0);           /* dc:97000 */
        PECMD_GetApiProcCached("WlanOpenHandle", "wlanapi.DLL",
                               (void **)&g_pWlanOpenHandle, (uintptr_t *)0x0);        /* dc:97001 */
        if (g_pWlanOpenHandle == (DWORD (*)(DWORD, void *, DWORD *))0) {              /* dc:97002 */
            uVar33 = 0xffffffff80004001;                                              /* dc:97003 */
            goto LAB_14009a630;                                                       /* dc:97004 */
        }
    }
    local_ba0 = 0;                                                                    /* dc:97007 */
    local_b78[0] = 0;                                                                 /* dc:97008 */
    local_c20 = (int *)0x0;                                                           /* dc:97009 */
    local_c00 = (LPWSTR)0x0;                                                          /* dc:97010 */
    local_b88 = (WCHAR *)0x0;                                                         /* dc:97011 */
    /* dc:97012: dc 丢第 4 参; EXE @0x14009a686 实调 (2, NULL, &协商版本, &hClient) */
    iVar10 = (int)((PECMD_FN_WlanOpenHandle)g_pWlanOpenHandle)(
        2, (void *)0, local_b78, (HANDLE *)&local_ba0);
    if (iVar10 != 0) {                                                                /* dc:97013 */
        DVar11 = GetLastError();                                                      /* dc:97014 */
        uVar33 = (ulonglong)DVar11;                                                   /* dc:97015 */
        if (DVar11 == 0) {                                                            /* dc:97016 */
            uVar33 = 1;                                                               /* dc:97017 */
        }
        PECMD_FreeStrBuf((WCHAR **)&local_c00);                                       /* dc:97019 */
        PECMD_FreeStrBuf((WCHAR **)&local_bc0);                                       /* dc:97020 */
        PECMD_FreeStrBuf((WCHAR **)&local_c30);                                       /* dc:97021 */
        PECMD_FreeContainer((longlong *)&local_c10);                                  /* dc:97022 */
        PECMD_FreeContainer((longlong *)&local_bf0);                                  /* dc:97023 */
        PECMD_FreeContainer((longlong *)&local_bd8);                                  /* dc:97024 */
        PECMD_FreeContainer((longlong *)&local_bf8);                                  /* dc:97025 */
        return uVar33;                                                                /* dc:97026 */
    }
    local_b80 = (uint64_t)(uint32_t)(*g_pWlanEnumInterfaces)(
        (HANDLE)local_ba0, (void *)0, (void **)&local_c20);                           /* dc:97028 */
    if ((uint)local_b80 != 0) {                                                       /* dc:97029 */
        DVar11 = GetLastError();                                                      /* dc:97030 */
        pWVar34 = (LPWSTR)(ulonglong)DVar11;                                          /* dc:97031 */
        if (DVar11 == 0) {                                                            /* dc:97032 */
            pWVar34 = (LPWSTR)0x1;                                                    /* dc:97033 */
        }
        goto LAB_14009b69a;                                                           /* dc:97035 */
    }
    if ((local_c20 == (int *)0x0) || (*local_c20 == 0)) {                             /* dc:97037 */
        if (((char)local_c38 == '\0') || (cVar22 != '\0')) {                          /* dc:97038 */
            pWVar34 = (LPWSTR)0x1;                                                    /* dc:97039 */
        }
        goto LAB_14009b69a;                                                           /* dc:97041 */
    }
    PECMD_AllocString(&local_bc0, 0x1000);                                            /* dc:97043 */
    if (*local_c20 <= (int)uVar32) {                                                  /* dc:97044 */
    LAB_14009a7ac:
        pWVar34 = (LPWSTR)0xffffffff80070057;                                         /* dc:97046 */
        goto LAB_14009b69a;                                                           /* dc:97047 */
    }
    local_b08[0] = L'\0';                                                             /* dc:97049 */
    memset(&local_b08[1], 0, 0x4c);                                                   /* dc:97050 (EXE 对 local_b08+local_b06 连续缓冲清 0x4c) */
    if ((bVar4) && (*local_c30 == L'\0')) {                                           /* dc:97051 */
        uVar32 = (ulonglong)(uint32_t)local_c20[1];                                   /* dc:97052 (list.Index) */
    LAB_14009a8e9:
        if (((int)uVar32 < 0) || (*local_c20 <= (int)uVar32)) {                       /* dc:97054 */
            goto LAB_14009a7ac;
        }
    }
    else if (((local_c48[0] != 0) || (bVar4)) && (*local_c30 != L'\0')) {             /* dc:97056 */
        local_res10 = (local_res10 & 0xffffffff00000000ULL) | 0xffffffffULL;          /* dc:97057 (CONCAT44 低半 = -1) */
        local_c50 = local_c30;                                                        /* dc:97058 */
        uVar12 = (undefined8)PECMD_ParseUIntValue((LPCWSTR *)&local_c50, (int *)&local_res10);  /* dc:97059 (CONCAT71 拼装 = int 返回) */
        bVar8 = (char)uVar12;
        if (((int)uVar12 < 1) || (*local_c50 != L'\0')) {                             /* dc:97060 (解析失败或有余文) */
            uVar32 = 0;                                                               /* dc:97061 */
            if (0 < *local_c20) {                                                     /* dc:97061b */
                do {                                                                  /* dc:97062 按名/GUID 串搜接口 */
                    local_bc8 = (wchar_t *)(void *)(local_c20 + (longlong)(int)uVar32 * 0x85 + 2);  /* dc:97063 */
                    iVar10 = lstrcmpiW(local_bc8 + 8, local_c30);                     /* dc:97064 (Description, 元素+8=字节+0x10) */
                    if (iVar10 == 0) goto LAB_14009a8e9;                              /* dc:97065 */
                    PECMD_GuidToString(local_b08, (uint *)local_bc8, 1);              /* dc:97066 (GUID→"{...}" 串) */
                    iVar10 = lstrcmpiW(local_b08, local_c30);                         /* dc:97067 */
                    if (iVar10 == 0) goto LAB_14009a8e9;                              /* dc:97068 */
                    uVar25 = (int)uVar32 + 1;                                         /* dc:97069 */
                    uVar32 = (ulonglong)uVar25;                                       /* dc:97070 */
                } while ((int)uVar25 < *local_c20);                                   /* dc:97071 */
            }
            uVar32 = local_res10 & 0xffffffffULL;                                     /* dc:97073 (未命中回退解析值) */
            goto LAB_14009a8e9;                                                       /* dc:97074 */
        }
        /* R25-j: dc:97060 良性解析路径无显式刷新; EXE @0x14009a8e2 证得
         * uVar32 取 local_res10 低半 (esi = [rsp+0xcf8]), 补此行对齐二进制 */
        uVar32 = local_res10 & 0xffffffffULL;
    }
    local_b90 = (LPWSTR)(void *)(local_c20 + (longlong)(int)uVar32 * 0x85 + 2);       /* dc:97076 (选中接口项) */
    local_c48[0] = (uint)(*(int *)(void *)(local_b90 + 0x108) == 1);                  /* dc:97077 (State==1) */
    if ((char)local_c38 != '\0') {                                                    /* dc:97078 (query/queryall) */
        uVarRow = 0;                                                                  /* EXE @0x14009a94e (r15d = 已打印行数) */
        while (true) {                                                                /* dc:97079 接口列表环 */
            PECMD_GuidToString(local_b08, (uint *)local_b90, 1);                      /* dc:97080 */
            /* dc:97081: 变参被 Ghidra 丢弃; EXE @0x14009a9b7 实参:
             *   %s = (行计数>0 ? DAT_1401210f8("\r\n") : DAT_14011c638(""))
             *   %d = 当前接口下标 (r9d = esi = uVar32)
             *   %s = &local_b08 (GUID 串)      %u = *(u32*)(local_b90+0x108) 状态
             *   %s = local_b90+8 (Description) */
            iVar10 = wsprintfW(local_bc0 + local_bb8, WSTR("%s%d\t%s\t%u\t%s"),
                               (0 < (int)uVarRow) ? (LPCWSTR)DAT_1401210f8 : (LPCWSTR)g_szEmpty,
                               (int)uVar32, local_b08,
                               *(uint32_t *)(void *)(local_b90 + 0x108), local_b90 + 8);
            pWVar34 = local_ba8;                                                      /* dc:97082 */
            local_bb8 = local_bb8 + iVar10;                                           /* dc:97083 */
            if (!bVar3) break;                                                        /* dc:97084 */
            uVar25 = (int)uVar32 + 1;                                                 /* dc:97085 */
            uVar32 = (ulonglong)uVar25;                                               /* dc:97086 */
            uVarRow = uVarRow + 1;                                                    /* EXE @0x14009a9e3 (随下标递增) */
            if (*local_c20 <= (int)uVar25) break;                                     /* dc:97087 */
            PECMD_GrowCapacityAligned((uint64_t *)&local_bc0, local_bb8 + 0x800, 0x1000);  /* dc:97088 */
            local_b90 = (LPWSTR)(void *)(local_c20 + (longlong)(int)uVar25 * 0x85 + 2);      /* dc:97089 */
            local_c48[0] = (uint)(*(int *)(void *)(local_b90 + 0x108) == 1);          /* dc:97090 */
        }
        pWVar20 = local_bf0;                                                          /* dc:97092 */
        pWVar21 = local_c10;                                                          /* dc:97093 */
        pwVar31 = local_bf8;                                                          /* dc:97094 */
        if ((char)local_res20 == '\0') {                                              /* dc:97095 */
            PECMD_SetVariableWithPrefix(param_1, local_c30, (LPCWSTR)local_bc0);      /* dc:97096 */
            goto LAB_14009b69a;                                                       /* dc:97097 */
        }
    }
    cVar22 = (char)local_res20;                                                       /* dc:97100 */
    if (bVar4) {                                                                      /* dc:97101 */
        (*g_pWlanDisconnect)((HANDLE)local_ba0, (LPCWSTR)local_b90, (LPCWSTR)0);      /* dc:97102 */
        goto LAB_14009b69a;                                                           /* dc:97103 */
    }
    if (bVar2) {                                                                      /* dc:97105 */
        /* dc:97106: dc 丢 3 参; EXE @0x14009aaca 实调 (handle, ifGuid, 0, 0, 0) */
        iVar10 = (int)((PECMD_FN_WlanScan)g_pWlanScan)(
            (HANDLE)local_ba0, (void *)local_b90, (void *)0, (void *)0, (void *)0);
        if (iVar10 == 0) {                                                            /* dc:97107 */
            FUN_1400195F0((void *)param_1, 4000, 0, (void *)0);                       /* dc:97108 */
            goto LAB_14009b69a;                                                       /* dc:97109 */
        }
    }
    else {                                                                            /* dc:97112 */
        if (cVar22 == '\0') {                                                         /* dc:97113 (连接/配置路径) */
            /* dc:97114-97123: token1/token2/c30 ANSI 化 */
            local_c38 = 0;                                                            /* dc:97114 */
            PECMD_WideToAnsiStr((longlong *)&local_c38, local_bf8, -1, 0xffffffffffffffff);  /* dc:97115 */
            local_be8 = 0;                                                            /* dc:97116 */
            PECMD_WideToAnsiStr((longlong *)&local_be8, local_bd8, -1, 0xffffffffffffffff);  /* dc:97117 */
            local_res20 = 0;                                                          /* dc:97118 */
            PECMD_WideToAnsiStr((longlong *)&local_res20, pwVar31, -1, 0xffffffffffffffff);  /* dc:97119 (pwVar31 = token1) */
            local_c08 = 0;                                                            /* dc:97120 */
            PECMD_WideToAnsiStr((longlong *)&local_c08, local_bd8, -1, 0xffffffffffffffff);  /* dc:97121 */
            local_c40 = (WCHAR *)0x0;                                                 /* dc:97122 */
            PECMD_WideToAnsiStr((longlong *)&local_c40, local_c30, -1, 0xffffffffffffffff);  /* dc:97123 */
            if ((*(char *)(uintptr_t)local_c38 == 0x23) &&                            /* dc:97124 (token1 '#' → b64) */
                (lVar13 = PECMD_Base64Decode((byte *)(uintptr_t)local_c38, (undefined8 *)&local_res20), lVar13 == 0)) {
            LAB_14009b0f8:
                pWVar34 = (LPWSTR)0xffffffff80070057;                                 /* dc:97126 */
            }
            else {
                if ((*(char *)(uintptr_t)local_be8 == 0x23) &&                        /* dc:97129 (token2 '#' → b64) */
                    (lVar13 = PECMD_Base64Decode((byte *)(uintptr_t)local_be8, (undefined8 *)&local_c08), lVar13 == 0)) {
                    PECMD_ZeroLenBuf((void *)(uintptr_t)local_res20);                 /* dc:97130 */
                    goto LAB_14009b0f8;                                               /* dc:97131 */
                }
                PECMD_StrBldCopyAnsi((longlong *)&local_c18, (LPCSTR)(uintptr_t)local_res20, 0xffffffffffffffff);  /* dc:97133 (ANSI→宽名) */
                PECMD_ZeroLenBuf((void *)(uintptr_t)local_res20);                     /* dc:97134 */
                pWVar21 = (LPWSTR)(uintptr_t)local_c18;                               /* dc:97135 */
                PECMD_InitStrContainer((int64_t *)&local_be0, (LPCWSTR)(uintptr_t)local_c18, -1, 0xfde9);  /* dc:97136 (宽→ANSI/UTF-8) */
                pWVar30 = local_be0;                                                  /* dc:97137 (此后 pWVar30 为 ANSI 形态) */
                lpString = local_c08;                                                 /* dc:97138 */
                if (((char)*(char *)(uintptr_t)local_c08 == '\0') &&                  /* dc:97139 (密码空) */
                    (pWVar20 = local_b90, uVar25 = (uint)local_b80, (char)local_bd0 == '\x02')) {  /* dc:97140 */
            LAB_14009b5a0:
                if (((char)local_bd0 != '\0') || (local_c48[0] != 0)) {               /* dc:97142 */
                    *(uint32_t *)local_b38 = 0;                                       /* dc:97143 (uSSIDLength) */
                    memset(local_b38 + 4, 0, 0x20);                                   /* dc:97144 */
                    *(uint32_t *)local_b38 = (uint32_t)lstrlenA((LPCSTR)pWVar30);      /* dc:97145 */
                    PECMD_MemMoveForward(local_b38 + 4, (uint8_t *)(uintptr_t)pWVar30,
                                         (int)*(uint32_t *)local_b38 + 1);            /* dc:97146 */
                    memset(local_b60, 0, 0x28);                                       /* dc:97147 (WLAN_CONNECTION_PARAMETERS) */
                    local_b50 = local_b38;                                            /* dc:97148 */
                    local_b60[2] = (ulonglong)(uintptr_t)local_b50;                   /* dc:97148 落位 (+0x10 pDot11Ssid; EXE @0x14009b623) */
                    local_b60[0] &= 0xffffffff00000000ULL;                            /* dc:97149 (mode 低半 = 0, 已清零) */
                    local_b60[1] = (ulonglong)(uintptr_t)local_c18;                   /* EXE @0x14009b633: +0x08 strProfile = local_c18 (dc 漏记) */
                    local_b40 = 3;                                                    /* dc:97150 */
                    local_b3c = 1;                                                    /* dc:97151 */
                    local_b60[4] = (ulonglong)local_b40 | ((ulonglong)local_b3c << 32);  /* dc:97150-97151 落位 (+0x20 BssType / +0x24 dwFlags) */
                    uVar25 = (uint)(*g_pWlanConnect)((HANDLE)local_ba0, (LPCWSTR)pWVar20,
                                                     local_b60, (void *)0);           /* dc:97152 */
                    memset(local_b38, 0, 0x24);                                       /* dc:97153 */
                }
                if (uVar25 != 0) {                                                    /* dc:97155 */
                    DVar11 = GetLastError();                                          /* dc:97156 */
                    pWVar34 = (LPWSTR)(ulonglong)DVar11;                              /* dc:97157 */
                    if (DVar11 == 0) {                                                /* dc:97158 */
                        pWVar34 = (LPWSTR)0x1;                                        /* dc:97159 */
                    }
                }
                }
                else {                                                                /* dc:97164 (XML 配置文件路径) */
                PECMD_StrBldCopyAnsi((longlong *)&local_b80, (LPCSTR)(uintptr_t)local_c08, 0xffffffffffffffff);  /* dc:97164 (密码 ANSI→宽) */
                PECMD_ZeroLenBuf((void *)(uintptr_t)lpString);                        /* dc:97165 */
                psVar24 = (short *)(uintptr_t)local_b80;                              /* dc:97166 */
                local_bc8 = (wchar_t *)L"open";                                       /* dc:97167 */
                if (*psVar24 != 0) {                                                  /* dc:97168 */
                    local_bc8 = (wchar_t *)L"WPA2PSK";                                /* dc:97169 */
                }
                local_b98 = L"none";                                                  /* dc:97171 */
                if (*psVar24 != 0) {                                                  /* dc:97172 */
                    local_b98 = L"AES";                                               /* dc:97173 */
                }
                pwVar27 = local_bc0;                                                  /* dc:97175 */
                if (*local_bc0 == L'>') {                                             /* dc:97176 */
                    pwVar27 = local_bc0 + 1;                                          /* dc:97177 */
                }
                wVar16 = *pwVar27;                                                    /* dc:97179 */
                pwVar14 = pwVar27;                                                    /* dc:97180 */
                if (wVar16 != L'\0') {                                                /* dc:97181 */
                    do {                                                              /* dc:97182 */
                        if (((8 < (ushort)wVar16) && ((ushort)wVar16 < 0xe)) || (wVar16 == L' ')) {  /* dc:97183 */
                            local_c50 = pwVar14;                                      /* dc:97184 */
                            if (*pwVar14 != L'\0') {                                  /* dc:97185 */
                                *pwVar14 = L'\0';                                     /* dc:97186 */
                                local_c50 = pwVar14 + 1;                              /* dc:97187 */
                            }
                            break;                                                    /* dc:97188 */
                        }
                        pwVar14 = pwVar14 + 1;                                        /* dc:97191 */
                        wVar16 = *pwVar14;                                            /* dc:97192 */
                        local_c50 = pwVar14;                                          /* dc:97193 */
                    } while (wVar16 != L'\0');                                        /* dc:97194 */
                    local_bc8 = pwVar27;                                              /* dc:97195 */
                    PECMD_SkipLeadingControlChars((longlong *)&local_c50);            /* dc:97196 */
                    iVar10 = lstrcmpW(L"open", pwVar27);                              /* dc:97197 */
                    if (iVar10 == 0) {                                                /* dc:97198 */
                        local_b98 = L"none";                                          /* dc:97199 */
                        if (*local_c50 != L'\0') {                                    /* dc:97200 */
                            local_b98 = local_c50;                                    /* dc:97201 */
                        }
                    }
                    else {
                        local_b98 = local_c50;                                        /* dc:97205 */
                    }
                }
                iVar10 = lstrlenW(pWVar21);                                           /* dc:97208 */
                iVar9 = lstrlenA((LPCSTR)(uintptr_t)lpString);                        /* dc:97209 */
                lVar13 = ((longlong)iVar9 + (longlong)iVar10) * 8 + 200;              /* dc:97210 */
                PECMD_AllocString(&local_c00, lVar13 * 3 + 0x4000);                   /* dc:97211 */
                PECMD_AllocStrSlot((WCHAR **)&local_c50);                             /* dc:97212 */
                pWVar6 = local_c00;                                                   /* dc:97213 */
                pWVar20 = local_c00 + 0x4000;                                         /* dc:97214 */
                local_ba8 = pWVar20 + lVar13;                                         /* dc:97215 */
                *pWVar20 = L'\0';                                                     /* dc:97216 */
                *local_ba8 = L'\0';                                                   /* dc:97217 */
                pWVar23 = local_ba8 + lVar13;                                         /* dc:97218 */
                *pWVar23 = L'\0';                                                     /* dc:97219 */
                local_b68 = pWVar23;                                                  /* dc:97220 */
                if (*local_b98 != L'\0') {                                            /* dc:97221 */
                    wsprintfW(pWVar20, WSTR("\t\t\t\t<encryption>%s</encryption>\r\n"), local_b98);  /* dc:97222 */
                }
                iVar10 = lstrlenA((LPCSTR)pWVar30);                                   /* dc:97224 */
                local_b98 = pWVar23;                                                  /* dc:97225 */
                if (0 < (longlong)iVar10) {                                           /* dc:97226 */
                    do {                                                              /* dc:97227 */
                        wsprintfW(pWVar23, WSTR("%02X"),
                                  (ulonglong)*(byte *)(uVar33 + (longlong)(intptr_t)pWVar30));  /* dc:97228 */
                        uVar33 = uVar33 + 1;                                          /* dc:97229 */
                        pWVar23 = pWVar23 + 2;                                        /* dc:97230 */
                    } while ((longlong)uVar33 < (longlong)iVar10);                    /* dc:97231 */
                    psVar24 = (short *)(uintptr_t)local_b80;                          /* dc:97232 (CONCAT44(uStack_b7c,local_b80) = 同槽重组) */
                    pWVar21 = (LPWSTR)(uintptr_t)local_c18;                           /* dc:97233 */
                    pwVar31 = local_bf8;                                              /* dc:97234 */
                }
                if (*psVar24 != 0) {                                                  /* dc:97236 */
                    wsprintfW(local_ba8,
                              WSTR("\t\t\t<sharedKey>\r\n\t\t\t\t<keyType>passPhrase</keyType>\r\n\t\t\t\t<protected>false</protected>\r\n\t\t\t\t<keyMaterial>%s</keyMaterial>\r\n\t\t\t</sharedKey>\r\n"),
                              psVar24);                                               /* dc:97237-97239 */
                }
                FUN_1400637DC((WCHAR **)&local_c50,
                              "<?xml version=\"1.0\"?>\r\n<WLANProfile xmlns=\"http://www.microsoft.com/networking/WLAN/profile/v1\">\r\n\t<name>%s</name>\r\n\t<SSIDConfig>\r\n\t\t<SSID>\r\n\t\t\t<hex>%s</hex>\r\n\t\t\t<name>%s</name>\r\n\t\t</SSID>\r\n\t\t<nonBroadcast>true</nonBroadcast>\r\n\t</SSIDConfig>\r\n\t<connectionType>ESS</connectionType>\r\n\t<connectionMode>auto</connectionMode>\r\n\t<autoSwitch>false</autoSwitch>\r\n\t<MSM>\r\n\t\t<security>\r\n\t\t\t<authEncryption>\r\n\t\t\t\t<authentication>%s</authentication>\r\n%s\t\t\t\t<useOneX>false</useOneX>\r\n\t\t\t</authEncryption>\r\n%s\t\t</security>\r\n\t</MSM>\r\n</WLANProfile>\r\n",
                              -1, -1);                                                /* dc:97241-97243 (dc 字面 0xffffffffffffffff) */
                if (*pwVar31 == L'&') {                                               /* dc:97244 */
                    ppWVar15 = (LPWSTR *)FUN_14001E69C((void *)param_1, (LPCWSTR)pwVar31,
                                                       (void *)0, -1);                /* dc:97245 */
                }
                else {
                    /* dc:97248: 6 个 %s 实参由 EXE @0x14009b4a5 恢复:
                     *   1,3 = pWVar21 (名, 模板 <name> 出现两次)   2 = local_b68 (hex)
                     *   4 = local_bc8 (authentication)   5 = pWVar20 (encryption 行)
                     *   6 = local_ba8 (sharedKey 块) */
                    wsprintfW(pWVar6, (LPCWSTR)local_c50, pWVar21, local_b68, pWVar21,
                              (LPCWSTR)local_bc8, pWVar20, local_ba8);
                    ppWVar15 = &local_c00;                                            /* dc:97249 */
                }
                pWVar20 = local_b90;                                                  /* dc:97251 */
                if (ppWVar15 != (LPWSTR *)0x0) {                                      /* dc:97252 */
                    local_res10 = local_res10 & 0xffffffff00000000ULL;                /* dc:97253 */
                    /* dc:97254: dc 丢 4 参; EXE @0x14009b535 实调
                     * (handle, ifGuid, 0, xml, security=NULL, fOverwrite=1, NULL, &reason) */
                    uVar25 = (uint)((PECMD_FN_WlanSetProfile)g_pWlanSetProfile)(
                        (HANDLE)local_ba0, (void *)local_b90, (DWORD)0, *ppWVar15,
                        (LPCWSTR)0, (DWORD)1, (void *)0, (DWORD *)&local_res10);
                    PECMD_ZeroLenBuf((void *)pWVar6);                                 /* dc:97255 */
                    if (g_u8CCB1 != 0) {                                              /* dc:97256 (DAT_14013ccb1) */
                        PECMD_TlsLogWrite((uint64_t)(uintptr_t)g_Script,
                                          WSTR("dwResult=%d reson=%d\r\n"),
                                          (ulonglong)uVar25,
                                          (ulonglong)local_res10 & 0xffffffffULL);    /* dc:97257-97258 */
                    }
                    PECMD_FreeStrBuf((WCHAR **)&local_c50);                           /* dc:97260 */
                    PECMD_FreeContainer((longlong *)&local_b80);                      /* dc:97261 */
                    goto LAB_14009b5a0;                                               /* dc:97262 */
                }
                PECMD_FreeStrBuf((WCHAR **)&local_c50);                               /* dc:97264 */
                PECMD_FreeContainer((longlong *)&local_b80);                          /* dc:97265 */
                pWVar34 = (LPWSTR)0xffffffff80070057;                                 /* dc:97266 */
                }
                PECMD_FreeContainer((longlong *)&local_be0);                          /* dc:97268 */
                PECMD_FreeContainer((longlong *)&local_c18);                          /* dc:97269 */
            }                                                                         /* dc:97270 */
            PECMD_FreeStrBuf((WCHAR **)&local_c40);                                   /* dc:97271 */
            PECMD_FreeContainer((longlong *)&local_c08);                              /* dc:97272 */
            PECMD_FreeContainer((longlong *)&local_res20);                            /* dc:97273 */
            PECMD_FreeContainer((longlong *)&local_be8);                              /* dc:97274 */
            PECMD_FreeContainer((longlong *)&local_c38);                              /* dc:97275 */
            goto LAB_14009b69a;                                                       /* dc:97276 */
        }
        /* dc:97278: dc 丢第 5 参; EXE @0x14009ab0c 实调 (handle, ifGuid, 0, 0, &local_b88) */
        iVar10 = (int)((PECMD_FN_WlanGetAvailNetList)g_pWlanGetAvailableNetworkList)(
            (HANDLE)local_ba0, (void *)local_b90, (DWORD)0, (void *)0, (void **)&local_b88);
        if (iVar10 == 0) {                                                            /* dc:97279 */
            PECMD_AllocStrSlot((WCHAR **)&local_c40);                                 /* dc:97280 */
            pWVar30 = local_b88;                                                      /* dc:97281 */
            if (local_b88 != (WCHAR *)0x0) {                                          /* dc:97282 */
                iVar10 = *(int *)local_b88;                                           /* dc:97283 (NumberOfItems) */
                pwVar31 = (wchar_t *)(longlong)iVar10;                                /* dc:97284 (计数借指针槽) */
                local_res20 = (local_res20 & 0xffffffff00000000ULL) | (uint64_t)(uint32_t)iVar10;  /* dc:97285 */
                if (iVar10 != 0) {                                                    /* dc:97286 */
                    local_c50 = (wchar_t *)g_szEmpty;                                 /* dc:97287 (&DAT_14011c638) */
                    uVar33 = 0;                                                       /* dc:97288 */
                    if (((int)cVar22 & 2U) != 0) {                                    /* dc:97289 */
                        local_c50 = (wchar_t *)L"\t";                                 /* dc:97290 */
                    }
                    local_c08 = ((local_c08 & 0xffffffff00000000ULL) | (uint64_t)(uint32_t)(int)cVar22) & 0xffffffff00000002ULL;  /* dc:97292 */
                    local_res10 = local_res10 & 0xffffffff00000000ULL;                /* dc:97293 */
                    local_be0 = pWVar21;                                              /* dc:97294 */
                    local_bc8 = pwVar31;                                              /* dc:97295 */
                    local_b98 = pWVar20;                                              /* dc:97296 */
                    if (0 < (longlong)(intptr_t)pwVar31) {                            /* dc:97297 */
                        pWVar35 = local_b88 + 0x130;                                  /* dc:97298 (= 表基+0x260 = 首 entry+0x258) */
                        local_c18 = (uint64_t)(intptr_t)pwVar31;                      /* dc:97299 (剩余计数) */
                        do {                                                          /* dc:97300 网络列表主环 */
                            iVar10 = (int)uVar33;                                     /* dc:97301 */
                            pwVar27 = pwVar31;                                        /* dc:97302 */
                            if (local_c57 == '\x01') {                                /* dc:97303 ('-' 模式: 与前项全等去重) */
                                uVar33 = 0;                                           /* dc:97304 */
                                if (0 < iVar10) {                                     /* dc:97305 */
                                    pWVar19 = pWVar30 + ((longlong)iVar10 * 0x9d + 2) * 2;  /* dc:97306 */
                                    do {                                              /* dc:97307 */
                                        iVar9 = PECMD_AnsiStrNCompare((char *)pWVar19,
                                            (longlong)(intptr_t)(pWVar30 + ((longlong)(int)uVar33 * 0x9d + 2) * 2),
                                            0x274);                                   /* dc:97308-97310 (整项比较) */
                                        if (iVar9 == 0) goto LAB_14009ac4c;           /* dc:97311 */
                                        uVar25 = (int)uVar33 + 1;                     /* dc:97312 */
                                        uVar33 = (ulonglong)uVar25;                   /* dc:97313 */
                                    } while ((int)uVar25 < iVar10);                   /* dc:97314 */
                                }
                            LAB_14009ad9f:
                                PECMD_GrowCapacityAligned((uint64_t *)&local_bc0, local_bb8 + 0x800, 0x1000);  /* dc:97317 */
                                pWVar34 = pWVar30 + ((longlong)iVar10 * 0x9d + 2) * 2;/* dc:97318 */
                                local_b90 = (LPWSTR)pWVar34;                          /* dc:97319 */
                                FUN_1400637DC((WCHAR **)&local_c40,
                                              (LPCSTR)(uintptr_t)(pWVar34 + 0x102),
                                              (longlong)*(int *)(pWVar35 + -0x2c), 0xfde9);  /* dc:97320-97321 (SSID 字节→宽, UTF-8) */
                                lVar13 = (longlong)(int)local_be8;                    /* dc:97322 */
                                if ((int)local_be8 == 0) {                            /* dc:97323 */
                                    local_be0 = local_c40;                            /* dc:97324 */
                                    local_b98 = pWVar34;                              /* dc:97325 */
                                }
                                else {
                                    wsprintfW(pWVar21 + lVar13, WSTR("%d"), iVar10 + 1);      /* dc:97328 */
                                    wsprintfW(pWVar20 + lVar13, WSTR("%d.PN"), iVar10 + 1);  /* dc:97329 */
                                    PECMD_SetVariableWithPrefix(param_1, pWVar21, (LPCWSTR)local_c40);  /* dc:97330 */
                                    PECMD_SetVariableWithPrefix(param_1, pWVar20, (LPCWSTR)local_b90);  /* dc:97331 */
                                    iVar10 = (int)local_res10;                        /* dc:97332 */
                                }
                                if (iVar10 < 1) {                                     /* dc:97334 */
                                    puVar26 = (undefined2 *)g_szEmpty;                /* dc:97335 */
                                    if ((char)local_c38 != '\0') goto LAB_14009aea7;  /* dc:97336 */
                                }
                                else {
                                LAB_14009aea7:
                                    puVar26 = (undefined2 *)DAT_1401210f8;            /* dc:97340 */
                                }
                                pWVar34 = (LPCWSTR)g_szEmpty;                         /* dc:97342 */
                                if ((int)local_c08 != 0) {                            /* dc:97343 */
                                    pWVar34 = local_b98;                              /* dc:97344 */
                                }
                                /* dc:97346-97348: 16 实参中 3-16 由 EXE @0x14009af57 恢复。
                                 * %s×5: puVar26(行前缀), pWVar34(ProfileName 或空),
                                 *       local_c50, local_be0, L"\t"(=0x14011fa84 常量);
                                 * %u×11 顺次: SignalQuality(+0x25C), Flags(+0x26C),
                                 *   BssType(+0x224), NumBssid(+0x228), Connectable(+0x22C),
                                 *   Reason(+0x230), SecEnabled(+0x260), Auth(+0x264),
                                 *   Cipher(+0x268), MorePhyTypes(+0x258), NumPhyTypes(+0x234) */
                                iVar9 = wsprintfW(local_bc0 + local_bb8,
                                                  WSTR("%s%s%s%s%s%u\t%u\t%u\t%u\t%u\t%u\t%u\t%u\t%u\t%u\t%u"),
                                                  (LPCWSTR)puVar26, pWVar34, (LPCWSTR)local_c50,
                                                  (LPCWSTR)local_be0, WSTR("\t"),
                                                  *(uint32_t *)((longlong)(intptr_t)pWVar35 + 4),
                                                  *(uint32_t *)((longlong)(intptr_t)pWVar35 + 0x14),
                                                  *(uint32_t *)((longlong)(intptr_t)pWVar35 - 0x34),
                                                  *(uint32_t *)((longlong)(intptr_t)pWVar35 - 0x30),
                                                  *(uint32_t *)((longlong)(intptr_t)pWVar35 - 0x2c),
                                                  *(uint32_t *)((longlong)(intptr_t)pWVar35 - 0x28),
                                                  *(uint32_t *)((longlong)(intptr_t)pWVar35 + 8),
                                                  *(uint32_t *)((longlong)(intptr_t)pWVar35 + 0xc),
                                                  *(uint32_t *)((longlong)(intptr_t)pWVar35 + 0x10),
                                                  *(uint32_t *)((longlong)(intptr_t)pWVar35),
                                                  *(uint32_t *)((longlong)(intptr_t)pWVar35 - 0x24));
                                local_bb8 = local_bb8 + iVar9;                        /* dc:97349 */
                                iVar9 = *(int *)(pWVar35 + -0x12);                    /* dc:97350 (NumPhyTypes) */
                                if (8 < iVar9) {                                      /* dc:97351 */
                                    iVar9 = 8;                                        /* dc:97352 */
                                }
                                lVar13 = (longlong)iVar9;                             /* dc:97354 */
                                if (0 < lVar13) {                                     /* dc:97355 */
                                    pWVar19 = pWVar35 + -0x10;                        /* dc:97356 (PhyTypes[0]) */
                                    do {
                                        iVar9 = wsprintfW(local_bc0 + local_bb8, WSTR("\t%u"),
                                                          *(uint32_t *)pWVar19);      /* dc:97358 */
                                        pWVar19 = pWVar19 + 2;                        /* dc:97359 */
                                        local_bb8 = local_bb8 + iVar9;                /* dc:97360 */
                                        lVar13 = lVar13 + -1;                         /* dc:97361 */
                                        pWVar21 = local_c10;                          /* dc:97362 */
                                    } while (lVar13 != 0);                            /* dc:97363 */
                                }
                            }
                            else {
                            LAB_14009ac4c:
                                iVar9 = (int)uVar33;                                  /* dc:97368 */
                                iVar29 = (int)(intptr_t)pwVar27;                      /* dc:97369 */
                                if (local_c57 == '\x02') {                            /* dc:97370 (向后扫: 同 SSID 且 Auth 更优) */
                                    uVar25 = iVar29 - 1;                              /* dc:97371 */
                                    if (-1 < (int)uVar25) {                           /* dc:97372 */
                                        lVar28 = ((longlong)(intptr_t)pwVar31 + -1) * 0x274;  /* dc:97373 */
                                        lVar13 = (longlong)iVar9 * 0x274;             /* dc:97374 */
                                        pWVar21 = pWVar30 + ((longlong)(int)uVar25 * 0x9d + 0x82) * 2;  /* dc:97375 */
                                        do {                                          /* dc:97376 */
                                            uVar32 = (ulonglong)uVar25;               /* dc:97377 */
                                            iVar9 = PECMD_AnsiStrNCompare(
                                                (char *)(pWVar30 + ((longlong)(int)uVar33 * 0x9d + 0x82) * 2),
                                                (longlong)(intptr_t)pWVar21, 0x24);   /* dc:97378-97380 (ProfileName 片段比较) */
                                            if (((iVar9 == 0) &&
                                                ((*(short *)((longlong)(intptr_t)pWVar30 + lVar28 + 8) != 0 ||
                                                  (*(short *)((longlong)(intptr_t)pWVar30 + lVar13 + 8) == 0)))) &&
                                               (*(uint32_t *)((longlong)(intptr_t)pWVar30 + lVar13 + 0x264) <=
                                                *(uint32_t *)((longlong)(intptr_t)pWVar30 + lVar28 + 0x264))) {  /* dc:97381-97385 */
                                                uVar33 = uVar32 & 0xffffffffULL;      /* dc:97386 */
                                                lVar13 = lVar28;                      /* dc:97387 */
                                            }
                                            uVar25 = (int)uVar32 - 1;                 /* dc:97389 */
                                            pWVar21 = pWVar21 + -0x13a;               /* dc:97390 */
                                            lVar28 = lVar28 + -0x274;                 /* dc:97391 */
                                        } while (-1 < (int)uVar25);                   /* dc:97392 */
                                        iVar29 = (int)local_res20;                    /* dc:97393 */
                                        pwVar31 = local_bc8;                          /* dc:97394 */
                                        pWVar21 = local_c10;                          /* dc:97395 */
                                        goto LAB_14009ad07;                           /* dc:97396 */
                                    }
                                }
                                else {
                                LAB_14009ad07:
                                    iVar9 = (int)uVar33;                              /* dc:97401 */
                                    if (local_c57 == '\x10') {                        /* dc:97402 ('**' 模式: 同 SSID 且对方有 Auth 我无) */
                                        uVar25 = iVar29 - 1;                          /* dc:97403 */
                                        if (-1 < (int)uVar25) {                       /* dc:97404 */
                                            lVar13 = (longlong)iVar9 * 0x274;         /* dc:97405 */
                                            lVar28 = ((longlong)(intptr_t)pwVar31 + -1) * 0x274;  /* dc:97406 */
                                            pWVar19 = pWVar30 + ((longlong)(int)uVar25 * 0x9d + 0x82) * 2;  /* dc:97407 */
                                            do {                                      /* dc:97408 */
                                                uVar32 = (ulonglong)uVar25;           /* dc:97409 */
                                                iVar9 = PECMD_AnsiStrNCompare(
                                                    (char *)(pWVar30 + ((longlong)(int)uVar33 * 0x9d + 0x82) * 2),
                                                    (longlong)(intptr_t)pWVar19, 0x24);  /* dc:97410-97412 */
                                                if (((iVar9 == 0) && (*(short *)((longlong)(intptr_t)pWVar30 + lVar28 + 8) != 0)) &&
                                                    (*(short *)((longlong)(intptr_t)pWVar30 + lVar13 + 8) == 0)) {  /* dc:97413-97414 */
                                                    uVar33 = uVar32 & 0xffffffffULL;  /* dc:97415 */
                                                    lVar13 = lVar28;                  /* dc:97416 */
                                                }
                                                iVar9 = (int)uVar33;                  /* dc:97418 */
                                                uVar25 = (int)uVar32 - 1;             /* dc:97419 */
                                                pWVar19 = pWVar19 + -0x13a;           /* dc:97420 */
                                                lVar28 = lVar28 + -0x274;             /* dc:97421 */
                                                pWVar20 = local_bf0;                  /* dc:97422 */
                                            } while (-1 < (int)uVar25);               /* dc:97423 */
                                        }
                                    }
                                }
                                if (iVar9 == iVar10) goto LAB_14009ad9f;              /* dc:97427 */
                            }
                            pwVar31 = (wchar_t *)(longlong)(int)local_res20;          /* dc:97429 */
                            uVar33 = (ulonglong)(iVar10 + 1U);                       /* dc:97430 */
                            pWVar35 = pWVar35 + 0x13a;                               /* dc:97431 */
                            local_c18 = (uint64_t)((longlong)local_c18 - 1);         /* dc:97432 */
                            local_res10 = (local_res10 & 0xffffffff00000000ULL) | (uint64_t)(uint32_t)(iVar10 + 1U);  /* dc:97433 */
                        } while (local_c18 != 0);                                    /* dc:97434 */
                        local_c18 = 0;                                               /* dc:97435 */
                        pWVar34 = local_ba8;                                         /* dc:97436 */
                    }                                                                /* dc:97437 */
                }                                                                    /* dc:97438 */
            }                                                                        /* dc:97439 */
            PECMD_SetVariableWithPrefix(param_1, local_c30, (LPCWSTR)local_bc0);      /* dc:97440 */
            PECMD_FreeStrBuf((WCHAR **)&local_c40);                                  /* dc:97441 */
            goto LAB_14009b69a;                                                      /* dc:97442 */
        }
    }                                                                                /* dc:97444 */
    DVar11 = GetLastError();                                                         /* dc:97445 */
    pWVar34 = (LPWSTR)(ulonglong)DVar11;                                             /* dc:97446 */
    if (DVar11 == 0) {                                                               /* dc:97447 */
        pWVar34 = (LPWSTR)0x1;                                                       /* dc:97448 */
    }
LAB_14009b69a:
    if (local_ba0 != 0) {                                                            /* dc:97451 */
        (*g_pWlanCloseHandle)((HANDLE)local_ba0, (void *)0);                         /* dc:97452 */
    }
    if (local_b88 != (WCHAR *)0x0) {                                                 /* dc:97454 */
        /* dc:97455: dc 丢参; EXE @0x14009b6bc rcx = local_b88 */
        ((PECMD_FN_WlanFreeMemory)g_pWlanFreeMemory)((void *)local_b88);
    }
    if (local_c20 != (int *)0x0) {                                                   /* dc:97457 */
        /* dc:97458: dc 丢参; EXE @0x14009b6cf rcx = local_c20 */
        ((PECMD_FN_WlanFreeMemory)g_pWlanFreeMemory)((void *)local_c20);
    }
    PECMD_FreeStrBuf((WCHAR **)&local_c00);                                          /* dc:97460 */
    PECMD_FreeStrBuf((WCHAR **)&local_bc0);                                          /* dc:97461 */
    PECMD_FreeStrBuf((WCHAR **)&local_c30);                                          /* dc:97462 */
    PECMD_FreeContainer((longlong *)&local_c10);                                     /* dc:97463 */
    PECMD_FreeContainer((longlong *)&local_bf0);                                     /* dc:97464 */
    PECMD_FreeContainer((longlong *)&local_bd8);                                     /* dc:97465 */
    PECMD_FreeContainer((longlong *)&local_bf8);                                     /* dc:97466 */
    return (ulonglong)(int64_t)(int)(intptr_t)pWVar34;                               /* dc:97467 */
}