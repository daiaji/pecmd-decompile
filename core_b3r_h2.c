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
 *   - FUN_140102a90(...) 是 memset 别名, 直接写 memset。
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
typedef uint32_t        uint;
typedef uint16_t        ushort;
typedef uint8_t         byte;
typedef uint8_t         undefined1;
typedef uint16_t        undefined2;
typedef uint32_t        undefined4;
typedef uint64_t        undefined8;
typedef void            undefined;
typedef int64_t         longlong;
typedef uint64_t        ulonglong;

/* ---- CONCAT splice macros (Ghidra register glue) ---- */
#define CONCAT44(hi, lo) ((((ulonglong)(uint32_t)(hi)) << 32) | (uint32_t)(lo))
#define CONCAT71(hi7, lo1) ((((ulonglong)(hi7)) << 8) | (uint8_t)(lo1))

/* ---- stub 缺失补充类型 ---- */
typedef DWORD *LPDWORD;
typedef LONG  *LSTATUS;
typedef void   *TIMERPROC;
typedef uint64_t pthreadmbcinfo;
typedef struct _APPBARDATA {
    DWORD cbSize;
    HWND  hWnd;
    UINT  uCallbackMessage;
    UINT  uEdge;
    RECT  rc;
    int   lParam;
} APPBARDATA;
#define ABM_GETTASKBARPOS 5

/* ---- 本文件引用的全局 DATA ---- */
extern DWORD g_imgBufLen;          /* 位图数据大小           */
extern WCHAR g_szEmpty[];       /* "" (空串)              */
extern const WCHAR g_wsz28038[];       /* 关键字串 .rdata         */
extern const WCHAR g_wsz27fc8[];       /* 关键字串 .rdata         */
extern const WCHAR g_wsz27fd0[];       /* 关键字串 .rdata         */

/* ---- 惰性加载函数指针槽 (DAT_14013xxx) ---- */
extern DWORD (*g_pGetVolumeInformationByHandleW)(HANDLE, LPWSTR, DWORD, LPDWORD);   /* GetVolumeInformationByHandleW */
extern DWORD (*g_pGetFinalPathNameByHandleW)(HANDLE, LPWSTR, DWORD, DWORD);     /* GetFinalPathNameByHandleW    */
extern DWORD (*g_pWlanOpenHandle)(DWORD, void *, DWORD *);           /* WlanOpenHandle               */
extern DWORD (*g_pWlanEnumInterfaces)(HANDLE, void *, void **);          /* WlanEnumInterfaces           */
extern DWORD (*g_pWlanSetProfile)(HANDLE, LPCWSTR, LPCWSTR, LPCWSTR);/* WlanSetProfile               */
extern DWORD (*g_pWlanCloseHandle)(HANDLE, void *);                   /* WlanCloseHandle              */
extern DWORD (*g_pWlanRegisterNotification)(void);                             /* WlanRegisterNotification     */
extern DWORD (*g_pWlanScan)(HANDLE, LPCWSTR);                  /* WlanScan                     */
extern DWORD (*g_pWlanGetAvailableNetworkList)(HANDLE, LPCWSTR, LPCWSTR, void *); /* WlanGetAvailableNetworkList  */
extern DWORD (*g_pWlanDisconnect)(HANDLE, LPCWSTR, LPCWSTR);         /* WlanDisconnect               */
extern void   (*g_pWlanFreeMemory)(void);                            /* WlanFreeMemory               */
extern DWORD (*g_pWlanConnect)(HANDLE, LPCWSTR, void *, void *);  /* WlanConnect                  */

/* ---- 本文件引用的辅助函数 (仅 extern) ---- */
extern void      FUN_14005b104(void *ps);                           /* @0x14005b104 释放串槽     */
extern void      PECMD_FreeContainer(void *ps);                           /* @0x14005b134 释放串资源   */
extern void      FUN_14005b0b8(void *ps);                           /* @0x14005b0b8 对象初始化   */
extern WCHAR    *FUN_14005b154(WCHAR **pp);                         /* @0x14005b154 跳过空白     */
extern undefined8 FUN_14005b1a8(ushort *a, undefined8 *b, int c);   /* @0x14005b1a8              */
extern undefined8 FUN_14005b2c0(void *a, LPCWSTR b, HWND c);        /* @0x14005b2c0              */
extern LPCWSTR   FUN_14005b6ac(HINSTANCE, UINT, LPWSTR, int);       /* @0x14005b6ac 资源字符串   */
extern int       FUN_14005c788(const char *a, const WCHAR *w, int n); /* @0x14005c788 前缀比较   */
extern int32_t   FUN_14005c7c4(const char *a, const WCHAR *w);      /* @0x14005c7c4 后缀比较     */
extern void      FUN_14005c828(const char *func, const char *dll, void **out,
                               uintptr_t *hmod);                    /* @0x14005c828 延迟加载     */
extern LARGE_INTEGER PECMD_SetFilePointer(HANDLE h, LARGE_INTEGER dist, DWORD method); /* @0x14005c674 */
extern DWORD     FUN_14005c5a0(HKEY root, LPCWSTR sub, LPCWSTR name, DWORD type,
                               BYTE *data, DWORD size);             /* @0x14005c5a0 RegSetValue  */
extern void      PECMD_OpenFileHandle(void *pH, LPCWSTR path, DWORD access, DWORD share,
                               void *sa, DWORD disp, DWORD flags, HANDLE tmpl); /* CreateFile 包装 */
extern void      FUN_140003a20(void *script, WCHAR **str, int mode); /* @0x140003a20             */
extern WCHAR    *PECMD_EnumerateVolume(longlong *a, longlong b, ulonglong c, LPCWSTR d); /* @0x140006aa0 */
extern void      PECMD_CreateTempMutexDir(void *a, void *b, undefined8 *c, LPCWSTR d); /* @0x140008110   */
extern void      PECMD_ScheduleSelfDelete(void *a, int b);                     /* @0x14000481c             */
extern WCHAR    *FUN_14001be14(WCHAR *s);                           /* @0x14001be14 前缀/标签查询 */
extern longlong  PECMD_EnumNtSymbolicLink(LPWSTR a, longlong *b, longlong *c, longlong *d); /* @0x14001d8c8 */
extern WCHAR    *FUN_140024c48(longlong *a, longlong *b, uint32_t c); /* @0x140024c48           */
extern longlong  PECMD_RunCommand(longlong *a, pthreadmbcinfo b);      /* @0x140031454 执行命令    */
extern void      FUN_14003c06c(void *a, void *b, uint c);           /* @0x14003c06c             */
extern WCHAR    *FUN_1400545f8(longlong *a, longlong *b, longlong *c,
                               WCHAR d, ushort e);                  /* @0x1400545f8 切分        */
extern void      PECMD_ParseSkipSeparator(longlong *a, longlong *b, int c, int d); /* @0x140054840       */
extern DWORD     FUN_14006459c(LPCWSTR path, uint32_t a, LPWSTR buf, LPWSTR *out);
extern undefined8 PECMD_MountVolumeToDir(LPCWSTR a, LPCWSTR b);              /* @0x1400604b0             */
extern undefined8 PECMD_CreateSymbolicLink(undefined8 a, undefined8 b, undefined4 c); /* @0x140060600    */
extern short    *PECMD_LastPathSeparator(short *s);                           /* @0x14006045c             */
extern undefined8 FUN_140063060(undefined8 a);                      /* @0x140063060             */
extern undefined8 *PECMD_AllocSmallObject(undefined8 *a);                    /* @0x140063344 小对象分配  */
extern void      FUN_140063620(void *out);                          /* @0x140063620 串容器初始化 */
extern void      PECMD_AllocWStringBuffer(WCHAR **ps, longlong count);         /* @0x140063694 串分配      */
extern WCHAR    *PECMD_AllocString(WCHAR **ps, longlong count);         /* @0x140063720 串扩容      */
extern WCHAR    *FUN_14006375c(WCHAR **ps, LPCWSTR src);            /* @0x14006375c 串追加      */
extern WCHAR    *PECMD_StrCopyW(WCHAR **ps, LPCWSTR src, longlong len); /* @0x140063888       */
extern char      FUN_1400660ac(const char *tok, WCHAR **pp, int n); /* @0x1400660ac 词比较      */
extern void      FUN_1400669c4(longlong *a, undefined8 b, LPCWSTR c); /* @0x1400669c4 SetVarD   */
extern void      PECMD_FormatSetVar(longlong *a, undefined8 b, LPCWSTR c, LPCWSTR d); /* @0x140066930 */
extern void      PECMD_VarSetUInt(longlong *a, undefined8 b, LPCWSTR c); /* @0x140066978         */
extern void      PECMD_SplitTokenTrimWs(longlong *src, longlong *dst, short delim); /* @0x1400675b8 切分 */
extern void      FUN_1400679b0(undefined8 *a, int *b, short c);     /* @0x1400679b0             */
extern void      PECMD_RegReadWrap(void *a, LPCWSTR b, LPCWSTR c, longlong *d,
                               void *e, void *f);                   /* @0x140069218 RegQuery   */
extern undefined8 PECMD_StripDevicePrefix(longlong a);                       /* @0x14006acb8             */
extern void      PECMD_HexStringToBytes(byte *a, byte *b);                   /* @0x14006ad9c             */
extern undefined8 PECMD_Base64Decode(byte *a, undefined8 *b);            /* @0x14006ae00             */
extern longlong *FUN_1400702f0(longlong *a, LPCSTR b, ulonglong c); /* @0x1400702f0 取串槽      */
extern void      FUN_1400702b0(WCHAR **ps, const WCHAR *src);       /* @0x1400702b0 串赋值      */
extern WCHAR    *FUN_14007034c(WCHAR **ps, const WCHAR *src);       /* @0x14007034c             */
extern WCHAR    *FUN_1400703e4(WCHAR **out, const WCHAR *src);      /* @0x1400703e4             */
extern int       PECMD_ParseUIntValue(LPCWSTR *pp, int *out);              /* @0x140074838 数值解析    */
extern void      FUN_14007d0ac(longlong *a, LPCWSTR b, LPCWSTR c);  /* @0x14007d0ac             */
extern undefined8 PECMD_ListDrives(undefined8 *a, WCHAR b, int c, int d,
                                uint e, LPCWSTR f);                 /* @0x1400787b4             */
extern undefined8 *PECMD_SplitTokenAssignVar(undefined8 *a, longlong *b, short c, byte d); /* @0x14007f6e4 */
extern undefined8 FUN_1400a9a84(longlong *a, ulonglong *b);         /* @0x1400a9a84             */
extern undefined8 FUN_1400e4d84(void);                              /* @0x1400e4d84             */
extern undefined8 FUN_1400e4d94(undefined8 a);                      /* @0x1400e4d94             */
extern void      FUN_1400e4e94(undefined8 *a);                      /* @0x1400e4e94             */
extern ulonglong FUN_1400e7664(LPCWSTR a);                          /* @0x1400e7664             */
extern longlong  FUN_1400e7758(longlong *a);                        /* @0x1400e7758             */
extern longlong  FUN_1400e7840(longlong *a, int b);                 /* @0x1400e7840             */
extern undefined8 FUN_1400ebd30(longlong a, LPCWSTR b, LPCWSTR c,
                                LPCWSTR d, LPCWSTR e, LPCWSTR f);   /* @0x1400ebd30             */
extern void      FUN_1400629b8(longlong *a, LPCWSTR b, LPCWSTR c);  /* @0x1400629b8 SetVar      */
extern undefined8 PECMD_ShowBrowseFolder(longlong *a, ushort *b, longlong c, longlong *d); /* @0x140087690 */
extern void      FUN_14009bb28(void *a, int b);                     /* @0x14009bb28             */
extern void      FUN_14007a224(void *a, WCHAR *b, WCHAR **c, int d, int e); /* @0x14007a224  */
extern WCHAR    *FUN_1400f429c(WCHAR **pp, uint16_t ch);             /* @0x1400f429c 查找字符   */

extern DWORD PECMD_SaveImageToFile(RECT *param_1, LPCWSTR param_2, LPCWSTR param_3,
                           LPCWSTR param_4);                          /* @0x14007d8d4 保存图像 */

/* 外部扩展 API (stub 未声明) */
extern HDC CreateDCW(LPCWSTR, LPCWSTR, LPCWSTR, const void *);
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
    short   *psVar3;
    LPWSTR  pWVar4;
    int     iVar5;
    int     iVar6;
    WCHAR   *local_res10;
    WCHAR   *local_res20;
    WCHAR   *local_28[2];

    local_res10 = (WCHAR *)param_2;
    FUN_14005b154(&local_res10);
    PECMD_AllocWStringBuffer(&local_res20, 5);
    PECMD_AllocWStringBuffer(local_28, 0x14);
    cVar1 = FUN_1400660ac("-ext", &local_res10, 4);
    PECMD_SplitTokenTrimWs((longlong *)&local_res10, (longlong *)&local_res20, 0x3d);
    FUN_140003a20(param_1, &local_res20, 0);
    if (*local_res10 == 0x3d) {
        local_res10 = local_res10 + 1;
        PECMD_SplitTokenTrimWs((longlong *)&local_res10, (longlong *)local_28, 0);
    }
    iVar6 = 0;
    if (*local_res20 == L'\0') {
        FUN_14005b104((longlong *)local_28);
        FUN_14005b104((longlong *)&local_res20);
        lVar2 = -0x7ff8ffa9;
    }
    else {
        lpStart = FUN_14001be14(local_28[0]);
        iVar5 = -0x7ff8ffa9;
        if (*lpStart != L'\0') {
            psVar3 = PECMD_LastPathSeparator((short *)lpStart);
            if (psVar3 != (short *)0x0) {
                lpStart = (LPCWSTR)(psVar3 + 1);
            }
            iVar5 = iVar6;
            if ((cVar1 == '\0') &&
                (pWVar4 = StrRChrW(lpStart, (LPCWSTR)0x0, L'.'),
                 pWVar4 != (LPWSTR)0x0)) {
                *pWVar4 = L'\0';
            }
        }
        FUN_1400629b8(param_1, local_res20, lpStart);
        FUN_14005b104((longlong *)local_28);
        FUN_14005b104((longlong *)&local_res20);
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
    HWND  hWnd;
    void *lpCriticalSection;
    bool  bVar1;
    bool  bVar2;
    undefined8 uVar3;
    UINT  uElapse;
    int   iVar4;
    LPCWSTR pwVar5;
    HWND  pHVar6;
    UINT  UVar7;
    int   local_res8[2];
    UINT  local_28[2];
    ushort *local_20;
    uint8_t *pb;

    pb = (uint8_t *)param_1;
    hWnd = (HWND)param_1[0xf];
    pHVar6 = *(HWND *)(*(longlong *)(param_1[10] + 0x40) + 0x20);
    uVar3 = FUN_14005c788("-del", param_3, 4);
    pwVar5 = WSTR("*del");
    if ((char)uVar3 == '\0') {
        pwVar5 = (LPCWSTR)param_4;
    }
    bVar1 = FUN_14005b2c0(param_1, pwVar5, pHVar6) != 0;
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
                KillTimer(hWnd, (longlong)*(int *)(pb + 0x70));
            }
            *(undefined4 *)(pb + 0x80) = 0;
            *(undefined4 *)(pb + 0x84) = 0;
        }
        else {
            if (*(int *)(pb + 0x80) != 0) {
                KillTimer(hWnd, (longlong)*(int *)(pb + 0x70));
            }
            *(int *)(pb + 0x84) = iVar4;
            *(UINT *)(pb + 0x74) = uElapse;
            SetTimer(hWnd, (longlong)*(int *)(pb + 0x70), uElapse, (TIMERPROC)0x0);
            *(undefined4 *)(pb + 0x80) = 1;
            UVar7 = uElapse;
        }
        FUN_1400669c4((longlong *)param_1[10], (undefined8)(ulonglong)UVar7,
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
    WCHAR     WVar1;
    char      cVar2;
    int       iVar3;
    int       iVar4;
    DWORD     DVar5;
    DWORD     DVar6;
    longlong *plVar7;
    undefined8 *puVar8;
    LPCWSTR   pWVar9;
    WCHAR    *pWVar10;
    undefined8 uVar11;
    DWORD     DVar12;
    uint      uVar13;
    DWORD     DVar14;
    LPCWSTR   lpString;
    DWORD     DVar15;
    WCHAR    *local_res10;
    uint64_t  local_res20;
    WCHAR    *local_98;
    WCHAR    *local_90;
    WCHAR    *local_88;
    longlong  local_80;
    longlong *local_78;
    undefined8 local_70;
    LPWSTR    local_68;
    uint      local_60[10];

    local_res10 = (WCHAR *)param_2;
    FUN_140063620(&local_80);
    local_70 = 0;
    local_78 = param_1;
    (void)local_70;
    (void)local_78;
    FUN_14005b154(&local_res10);
    DVar6 = 0;
    PECMD_AllocWStringBuffer(&local_90, 5);
    PECMD_AllocWStringBuffer(&local_98, 0x14);
    DVar5 = 0;
    local_res20 = (uint64_t)((uint64_t)(uint32_t)(local_res20 >> 32) << 32);
    cVar2 = FUN_1400660ac("-j", &local_res10, 2);
    DVar12 = 1;
    plVar7 = (longlong *)PECMD_SplitTokenAssignVar((undefined8 *)&local_80, (longlong *)&local_res10, 0x2c, 1);
    PECMD_SplitTokenTrimWs((longlong *)plVar7, (longlong *)&local_90, 0);
    if (*local_res10 == L',') {
        local_res10 = local_res10 + 1;
        plVar7 = (longlong *)PECMD_SplitTokenAssignVar((undefined8 *)&local_80, (longlong *)&local_res10, 0x2c, 1);
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
        puVar8 = PECMD_SplitTokenAssignVar((undefined8 *)&local_80, (longlong *)&local_res10, 0x2c, 1);
        FUN_1400679b0(puVar8, (int *)&local_res20, 0x2c);
        DVar14 = (uint)(uint32_t)local_res20;
    }
    if ((*local_90 == 0) || (((DVar14 == 0 && (cVar2 == '\0')) && (*local_98 == L'\0')))) {
LAB_140096b4b:
        FUN_14005b104((longlong *)&local_98);
        FUN_14005b104((longlong *)&local_90);
        FUN_14005b104(&local_80);
        return -0x7ff8ffa9;
    }
    iVar3 = lstrlenW(local_98);
    PECMD_AllocString(&local_98, (longlong)(iVar3 + 0x214));
    local_68 = (LPWSTR)0x0;
    pWVar9 = FUN_14001be14(local_98);
    pWVar10 = FUN_14001be14((WCHAR *)local_90);
    if (*pWVar10 == L'\0') goto LAB_140096b4b;
    if (((DVar15 == 0) && (cVar2 == '\0')) && (DVar14 == 0)) {
        iVar3 = CreateHardLinkW(pWVar10, pWVar9, (LPSECURITY_ATTRIBUTES)0x0);
    }
    else {
        if ((DVar14 == 2) || (cVar2 != '\0')) {
            if (*pWVar9 == L'\0') {
                iVar3 = RemoveDirectoryW(pWVar10);
            }
            else {
                FUN_14006459c(pWVar9, 0x208, local_98 + (iVar3 + 4) + 4, &local_68);
                memcpy(local_98 + (iVar3 + 4), WSTR("\\??\\"), 8);
                uVar11 = PECMD_MountVolumeToDir(pWVar10, local_98 + (iVar3 + 4));
                iVar3 = (int)uVar11;
            }
            if (iVar3 == 0) {
                DVar6 = GetLastError();
            }
            FUN_14005b104((longlong *)&local_98);
            FUN_14005b104((longlong *)&local_90);
            FUN_14005b104(&local_80);
            return (longlong)(int)DVar6;
        }
        if (DVar15 != 0) {
            FUN_140102a90((ulonglong *)local_60, 0, 0x24);
            GetFileAttributesExW(pWVar10, 2, local_60);
            local_88 = (WCHAR *)0x0;
            lpString = (LPCWSTR)g_szEmpty;
            if ((local_60[0] >> 10 & 1) != 0) {
                local_res10 = (WCHAR *)0x0;
                PECMD_OpenFileHandle(&local_res10, pWVar10, 0xc0000000, 7,
                              (LPSECURITY_ATTRIBUTES)0x0, 3, 0x80, (HANDLE)0x0);
                pWVar10 = (WCHAR *)local_res10;
                if (local_res10 != (WCHAR *)0x0) {
                    FUN_14005c828("GetFinalPathNameByHandleW", (LPCSTR)0x0,
                                  (void **)&g_pGetFinalPathNameByHandleW, &g_hKernel32);
                    if (g_pGetFinalPathNameByHandleW != (DWORD (*)(HANDLE, LPWSTR, DWORD, DWORD))0) {
                        PECMD_AllocString(&local_88, 0xa000);
                        lpString = (LPCWSTR)local_88;
                        *local_88 = L'\0';
                        DVar12 = (*g_pGetFinalPathNameByHandleW)((HANDLE)pWVar10, local_88, 0xa000, 8);
                        if (DVar12 == 0) {
                            DVar12 = (*g_pGetFinalPathNameByHandleW)((HANDLE)pWVar10, (LPWSTR)lpString, 0xa000, 10);
                            iVar3 = lstrlenW(lpString);
                            if ((7 < iVar3) &&
                                (iVar4 = StrCmpNW(lpString, WSTR("\\Device\\"), 8), iVar4 == 0)) {
                                local_res20 = (uint64_t)(uintptr_t)(lpString + 8);
                                FUN_1400f429c((WCHAR **)&local_res20, 0x5c);
                                WVar1 = *(WCHAR *)(uintptr_t)local_res20;
                                if (WVar1 != L'\0') {
                                    *(WCHAR *)(uintptr_t)local_res20 = L'\0';
                                }
                                PECMD_AllocWStringBuffer(&local_res10, 100);
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
                                              (uintptr_t)lpString) >> 1)) + 1) * 2);
                                    memcpy((uint8_t *)lpString,
                                           (uint8_t *)local_res10, iVar4 * 2);
                                }
                                FUN_14005b104((longlong *)&local_res10);
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
            FUN_1400629b8(param_1, pWVar9, lpString);
            FUN_14005b104((longlong *)&local_88);
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
    FUN_14005b104((longlong *)&local_98);
    FUN_14005b104((longlong *)&local_90);
    FUN_14005b104(&local_80);
    return (longlong)(int)DVar5;
}

/* ================================================================
 * @0x14009bbbc  (LARGE_INTEGER* PECMD_TempPathCommand(longlong*, LARGE_INTEGER,
 *                                             longlong))
 * 临时目录 / *del / *tmpdir / *tmpfile / Delete / Setting 命令。
 * ================================================================ */
LARGE_INTEGER * PECMD_TempPathCommand(longlong *param_1, LARGE_INTEGER param_2, longlong param_3)
{
    ushort       uVar1;
    bool         bVar2;
    char         cVar3;
    char         cVar4;
    int          iVar5;
    undefined8   uVar6;
    LARGE_INTEGER LVar7;
    LPWSTR       pWVar8;
    LPCWSTR      pWVar9;
    LPCWSTR      pWVar10;
    LPCWSTR      pWVar11;
    LARGE_INTEGER *pLVar12;
    LPCWSTR      *ppWVar13;
    LARGE_INTEGER *pLVar14;
    WCHAR        WVar15;
    LARGE_INTEGER LVar16;
    LARGE_INTEGER *pLVar17;
    char         cVar18;
    LARGE_INTEGER local_res10;
    longlong     local_res18;
    undefined8   local_res20;
    char         local_ef8;
    DWORD        local_ef4;
    LPCWSTR      local_ef0;
    WCHAR       *local_ee8;
    WCHAR       *local_ee0;
    ushort      *local_ed8;
    WCHAR       *local_ed0;
    LPCWSTR      local_ec8;
    WCHAR       *local_ec0;
    WCHAR       *local_eb8;
    WCHAR       *local_eb0;
    WCHAR        local_ea8[264];
    WCHAR        local_c98[264];
    WCHAR        local_a88[264];
    WCHAR        local_878[264];
    WCHAR        local_668[264];
    WCHAR        local_458[264];
    WCHAR        local_248[264];

    pLVar17 = (LARGE_INTEGER *)0x0;
    cVar3 = '\0';
    local_res20 = 0;
    cVar18 = '\0';
    local_ef8 = '\0';
    local_res10 = param_2;
    local_res18 = param_3;
    local_eb8 = (WCHAR *)0x0;
    FUN_140063620(&local_ee8);
    FUN_1400702f0((longlong *)&local_eb8, ".tmp", 0xffffffffffffffff);
    FUN_14005b154((WCHAR **)&local_res10.QuadPart);
    bVar2 = false;
    LVar7 = local_res10;
    while (*(ushort *)LVar7.QuadPart == 0x2a) {
        local_res10.QuadPart = LVar7.QuadPart + 2;
        cVar4 = FUN_1400660ac("del", (WCHAR **)&local_res10.QuadPart, 3);
        if (cVar4 == '\0') {
            LVar7 = local_res10;
            uVar6 = FUN_14005c788("tmpl:", (ushort *)local_res10.QuadPart, 5);
            if ((char)uVar6 == '\0') {
                cVar4 = FUN_1400660ac("tmpdir", (WCHAR **)&local_res10.QuadPart, 6);
                if (cVar4 == '\0') {
                    cVar4 = FUN_1400660ac("tmpfile", (WCHAR **)&local_res10.QuadPart, 7);
                    if (cVar4 == '\0') {
                        uVar1 = *(ushort *)local_res10.QuadPart;
                        while ((uVar1 != 0 &&
                                (((uVar1 < 9 || (0xd < uVar1)) && (uVar1 != 0x20))))) {
                            local_res10.QuadPart = local_res10.QuadPart + 2;
                            uVar1 = *(ushort *)local_res10.QuadPart;
                        }
                        FUN_14005b154((WCHAR **)&local_res10.QuadPart);
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
                LVar7 = (LARGE_INTEGER)(intptr_t)FUN_140024c48((longlong *)&local_ef0,
                                              (longlong *)&local_ee0, 5);
                local_res10 = LVar7;
                if (0 < (longlong)(intptr_t)local_ee0) {
                    PECMD_StrCopyW(&local_ee8, local_ef0, (longlong)(intptr_t)local_ee0);
                    pWVar8 = StrChrW((LPCWSTR)local_ee8, L'*');
                    if (pWVar8 != (LPWSTR)0x0) {
                        *pWVar8 = L'\0';
                        FUN_1400703e4(&local_eb8, pWVar8 + 1);
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
            if (uVar1 != 0x24) break;
            local_ef8 = *(char *)LVar7.QuadPart;
        }
        local_res10.QuadPart = LVar7.QuadPart + 2;
        FUN_14005b154((WCHAR **)&local_res10.QuadPart);
        LVar7 = local_res10;
    }
    if (cVar18 == '\0') {
        cVar3 = FUN_1400660ac("Delete", (WCHAR **)&local_res10.QuadPart, 6);
        cVar3 = cVar3 != '\0';
        cVar4 = FUN_1400660ac("Setting", (WCHAR **)&local_res10.QuadPart, 7);
        if (cVar4 != '\0') {
            cVar3 = -1;
        }
    }
    FUN_14005b154((WCHAR **)&local_res10.QuadPart);
    local_ee0 = (WCHAR *)0x0;
    local_ed0 = (WCHAR *)0x0;
    local_ef0 = (LPCWSTR)0x0;
    FUN_140063620(&local_ee0);
    FUN_140063620(&local_ed0);
    FUN_140063620(&local_ef0);
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
        if (*(ushort *)local_res10.QuadPart == 0x2c) goto LAB_14009bea8;
    }
    PECMD_SplitTokenTrimWs(&local_res10.QuadPart, (longlong *)&local_ed0, 0x2c);
    if (*(ushort *)local_res10.QuadPart == 0x2c) {
        local_res10.QuadPart = local_res10.QuadPart + 2;
    }
    PECMD_SplitTokenTrimWs(&local_res10.QuadPart, (longlong *)&local_ef0, 0x2c);
    if (cVar18 != '\0') {
        local_ec0 = (WCHAR *)0x0;
        FUN_140063620(&local_ec0);
        pWVar9 = local_ef0;
        if ((cVar18 == '\x01') || (pLVar12 = pLVar17, *local_ef0 != L'\0')) {
            pLVar12 = (LARGE_INTEGER *)&local_ec0;
        }
        pLVar14 = (LARGE_INTEGER *)&local_ee8;
        if (cVar18 != '\x02') {
            pLVar14 = pLVar17;
        }
        PECMD_CreateTempMutexDir(&pLVar12->QuadPart, &pLVar14->QuadPart, (undefined8 *)0x0, WSTR("mktmp"));
        if (cVar18 == '\x01') {
            FUN_1400629b8(param_1, local_ed0, (LPCWSTR)local_ec0);
            WVar15 = *(WCHAR *)local_ec0;
        }
        else {
            FUN_1400629b8(param_1, local_ed0, (LPCWSTR)local_ee8);
            FUN_1400629b8(param_1, pWVar9, (LPCWSTR)local_ec0);
            WVar15 = *(WCHAR *)local_ee8;
        }
        if ((bVar2) && (*(short *)local_ec0 != 0)) {
            PECMD_ScheduleSelfDelete(local_ec0, 0);
        }
        FUN_14009bb28(param_1, 0);
        FUN_14005b104(&local_ec0);
        FUN_14005b104((longlong *)&local_ef0);
        FUN_14005b104((longlong *)&local_ed0);
        FUN_14005b104(&local_ee0);
        FUN_14005b104(&local_eb8);
        FUN_14005b104(&local_ee8);
        return (LARGE_INTEGER *)(ulonglong)(WVar15 == L'\0');
    }
    local_ef4 = 0;
    local_ec0 = (WCHAR *)0x0;
    local_eb0 = (WCHAR *)0x0;
    FUN_140063620(&local_ec0);
    FUN_140063620(&local_eb0);
    PECMD_RegReadWrap((void *)0xffffffff80000001, WSTR("Environment"), WSTR("TEMP"), (longlong *)&local_ec0,
                  &local_ef4, (LSTATUS *)0x0);
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
    LVar7 = (LARGE_INTEGER)(intptr_t)local_ec0;
    if (*local_ed0 != L'\0') {
        FUN_1400629b8(param_1, local_ed0, (LPCWSTR)local_ec0);
    }
    if (cVar3 == '\0') {
        pLVar12 = (LARGE_INTEGER *)0xffffffff80070057;
        goto LAB_14009c4e0;
    }
    if (*(short *)LVar7.QuadPart == 0) {
        pWVar9 = FUN_14005b6ac(g_hInstance, 0x2716, local_878, 0x104);
        pWVar10 = FUN_14005b6ac(g_hInstance, 0x2717, local_458, 0x104);
        MessageBoxW((HWND)0x0, pWVar9, pWVar10, 0x10);
        pLVar12 = (LARGE_INTEGER *)0x1;
        if (cVar3 == '\x01') goto LAB_14009c4e0;
    }
    FUN_14007a224(param_1, (WCHAR *)LVar7.QuadPart, &local_eb0, 0, 0);
    LVar16 = (LARGE_INTEGER)(intptr_t)local_eb0;
    local_res10 = (LARGE_INTEGER)(intptr_t)local_eb0;
    local_eb0 = (WCHAR *)LVar7.QuadPart;
    pWVar9 = FUN_14005b6ac(g_hInstance, 0x2718, local_ea8, 0x104);
    pLVar12 = pLVar17;
    if (cVar3 == '\x01') {
        if ((char)local_res20 == '\0') {
            pWVar10 = FUN_14005b6ac(g_hInstance, 0x2719, local_c98, 0x104);
            pWVar11 = FUN_14005b6ac(g_hInstance, 0x271a, local_a88, 0x104);
            FUN_1400702b0((WCHAR **)&local_res20, pWVar9);
            FUN_14006375c((WCHAR **)&local_res20, (LPCWSTR)LVar16.QuadPart);
            FUN_14006375c((WCHAR **)&local_res20, pWVar10);
            iVar5 = MessageBoxW((HWND)0x0, (LPCWSTR)local_res20, pWVar11, 4);
            ppWVar13 = (LPCWSTR *)&local_res20;
            if (iVar5 != 6) goto LAB_14009c4d8;
            FUN_14005b104((longlong *)ppWVar13);
        }
        FUN_14006375c((WCHAR **)&local_res10.QuadPart, WSTR("\\*.*"));
        FUN_14003c06c(param_1, &local_res10, 1);
    }
    else {
        pWVar10 = FUN_14005b6ac(g_hInstance, 0x271b, local_668, 0x104);
        pWVar11 = FUN_14005b6ac(g_hInstance, 0x271c, local_248, 0x104);
        local_ec8 = (LPCWSTR)0x0;
        FUN_1400702b0((WCHAR **)&local_ec8, pWVar9);
        FUN_14006375c((WCHAR **)&local_ec8, (LPCWSTR)LVar16.QuadPart);
        FUN_14006375c((WCHAR **)&local_ec8, pWVar10);
        local_ec0 = (WCHAR *)0x0;
        local_ed8 = (ushort *)0x0;
        FUN_140063620(&local_ec0);
        FUN_140063620(&local_ed8);
        LVar7 = (LARGE_INTEGER)(intptr_t)local_ee0;
        if ((*(short *)local_ee0 == 0) || ((char)local_res20 == '\0')) {
            iVar5 = MessageBoxW((HWND)0x0, local_ec8, pWVar11, 4);
            if (iVar5 == 6) {
                FUN_14007034c((WCHAR **)&local_ed8, WSTR("_,*"));
                if (*(short *)LVar7.QuadPart != 0) {
                    LVar16 = LVar7;
                }
                FUN_14006375c((WCHAR **)&local_ed8, (LPCWSTR)LVar16.QuadPart);
                FUN_14006375c((WCHAR **)&local_ed8, WSTR(",,,0x10"));
                PECMD_ShowBrowseFolder(param_1, (ushort *)local_ed8, local_res18, (longlong *)&local_ec0);
                LVar7 = (LARGE_INTEGER)(intptr_t)local_ec0;
                if (*(short *)local_ec0 != 0) goto LAB_14009c3db;
            }
        }
        else {
LAB_14009c3db:
            iVar5 = lstrlenW((LPCWSTR)LVar7.QuadPart);
            local_ef4 = iVar5 * 2;
            FUN_14005c5a0((HKEY)0xffffffff80000001, WSTR("Environment"), WSTR("TMP"), 1,
                          (BYTE *)LVar7.QuadPart, local_ef4);
            FUN_14005c5a0((HKEY)0xffffffff80000001, WSTR("Environment"), WSTR("TEMP"), 1,
                          (BYTE *)LVar7.QuadPart, local_ef4);
            if (local_ef8 != '\0') {
                FUN_14005c5a0((HKEY)0xffffffff80000002,
                              WSTR("SYSTEM\\CurrentControlSet\\Control\\Session Manager\\Environment"),
                              WSTR("TEMP"), 1, (BYTE *)LVar7.QuadPart, local_ef4);
                FUN_14005c5a0((HKEY)0xffffffff80000002,
                              WSTR("SYSTEM\\CurrentControlSet\\Control\\Session Manager\\Environment"),
                              WSTR("TMP"), 1, (BYTE *)LVar7.QuadPart, local_ef4);
            }
            FUN_1400629b8(param_1, WSTR("TMP"), (LPCWSTR)LVar7.QuadPart);
            FUN_1400629b8(param_1, WSTR("TEMP"), (LPCWSTR)LVar7.QuadPart);
            FUN_1400629b8(param_1, local_ed0, (LPCWSTR)LVar7.QuadPart);
        }
        FUN_14005b104((longlong *)&local_ed8);
        FUN_14005b104((longlong *)&local_ec0);
        ppWVar13 = &local_ec8;
LAB_14009c4d8:
        FUN_14005b104((longlong *)ppWVar13);
    }
LAB_14009c4e0:
    FUN_14005b104(&local_eb0);
    FUN_14005b104(&local_ec0);
    FUN_14005b104((longlong *)&local_ef0);
    FUN_14005b104((longlong *)&local_ed0);
    FUN_14005b104(&local_ee0);
    FUN_14005b104(&local_eb8);
    FUN_14005b104(&local_ee8);
    return pLVar12;
}

/* ================================================================
 * @0x140097c70  (ulonglong PECMD_ScreenAreaCommand(longlong*, LPCWSTR))
 * 屏幕/窗口区域命令: -cap / -max / -win / -taskbar / -desk- / -save /
 * -display: / -gui / -capgui, 输出数值或截图。
 * ================================================================ */
ulonglong PECMD_ScreenAreaCommand(longlong *param_1, LPCWSTR param_2)
{
    WCHAR     WVar1;
    WCHAR     WVar2;
    char      cVar3;
    char      cVar4;
    uint      uVar5;
    uint      uVar6;
    int       iVar7;
    BOOL      BVar8;
    DWORD     DVar9;
    uint      uVar10;
    uint      uVar11;
    undefined8 uVar12;
    ulonglong uVar13;
    ulonglong uVar14;
    HDC       hdc;
    ulonglong uVar15;
    ulonglong uVar16;
    char      cVar17;
    char      cVar18;
    RECT     *hWnd;
    ulonglong uVar19;
    RECT     *ptVar20;
    ulonglong uVar21;
    longlong  lVar22;
    LPCWSTR   pWVar23;
    int       iVar24;
    char      cVar25;
    ulonglong uVar26;
    byte      bVar27;
    WCHAR    *local_res10[2];
    uint64_t  local_res20;
    WCHAR    *local_c8;
    byte      local_c0;
    char      local_bf;
    uint      local_bc;
    uint      local_b8;
    RECT      local_b0;
    uint      local_a0;
    uint64_t  local_98;
    WCHAR    *local_90;
    LPCWSTR   local_88;
    uint      local_80;
    uint      local_78;
    APPBARDATA local_70;

    local_res10[0] = (WCHAR *)param_2;
    FUN_14005b154(local_res10);
    PECMD_AllocWStringBuffer(&local_90, 5);
    PECMD_AllocWStringBuffer(&local_c8, 0x14);
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
        cVar3 = FUN_1400660ac("-cap", local_res10, 4);
        cVar4 = '\x01';
        if (cVar3 == '\0') {
            cVar3 = FUN_1400660ac("-max", local_res10, 4);
            if (cVar3 == '\0') {
                cVar3 = FUN_1400660ac("-win", local_res10, 4);
                if (cVar3 == '\0') {
                    cVar3 = FUN_1400660ac("-taskbar", local_res10, 8);
                    if (cVar3 == '\0') {
                        cVar3 = FUN_1400660ac("-desk-", local_res10, 5);
                        if (cVar3 != '\0') goto LAB_140097f1d;
                        cVar4 = FUN_1400660ac("-desk-", local_res10, 6);
                        if (cVar4 == '\0') {
                            cVar3 = FUN_1400660ac("-save", local_res10, 5);
                            cVar4 = cVar17;
                            if (cVar3 == '\0') {
                                pWVar23 = local_res10[0];
                                uVar12 = FUN_14005c788("-display:", (ushort *)local_res10[0], 9);
                                if ((char)uVar12 == '\0') {
                                    cVar4 = FUN_1400660ac("-gui", local_res10, 4);
                                    if ((cVar4 != '\0') ||
                                        (cVar4 = FUN_1400660ac("-capgui", local_res10, 7),
                                         cVar4 != '\0')) {
                                        FUN_1400702b0((WCHAR **)&local_res20, WSTR("#31:INDATA "));
                                        FUN_14006375c((WCHAR **)&local_res20, local_res10[0]);
                                        uVar16 = PECMD_RunCommand(param_1, local_res20);
                                        FUN_14005b104((longlong *)&local_res20);
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
                                FUN_14005b154(local_res10);
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
        if (*local_res10[0] != L'-') goto LAB_140097f33;
        goto LAB_140097cfe;
    }
LAB_140097f33:
    FUN_1400545f8(param_1, (longlong *)local_res10, (longlong *)&local_90, L',', 8);
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
            iVar7 = (int)FUN_1400a9a84((longlong *)local_res10, (ulonglong *)&local_98);
            bVar27 = (byte)(iVar7 != 0);
            hWnd = ptVar20;
            if (bVar27) {
                local_b0.left = (int)(uint32_t)local_98;
                hWnd = (RECT *)(uintptr_t)local_98;
            }
            iVar7 = (int)FUN_1400a9a84((longlong *)local_res10, (ulonglong *)&local_res20);
            if (iVar7 != 0) {
                bVar27 = (byte)(bVar27 | 2);
                local_b0.top = (uint)(uint32_t)local_res20;
            }
            iVar7 = (int)FUN_1400a9a84((longlong *)local_res10, (ulonglong *)&local_res20);
            if (iVar7 != 0) {
                bVar27 = (byte)(bVar27 | 4);
                local_b0.right = (uint)(uint32_t)local_res20;
            }
            iVar7 = (int)FUN_1400a9a84((longlong *)local_res10, (ulonglong *)&local_res20);
            if (iVar7 != 0) {
                bVar27 = (byte)(bVar27 | 8);
                local_b0.bottom = (uint)(uint32_t)local_res20;
            }
            FUN_140024c48((longlong *)local_res10, (longlong *)0x0, 0x1d);
            pWVar23 = (LPCWSTR)0x0;
            if ((*local_res10[0] != L'\0') &&
                (pWVar23 = local_res10[0], (bVar27 & 0xc) != 0xc)) {
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
                    FUN_14005b104((longlong *)&local_c8);
                    FUN_14005b104((longlong *)&local_90);
                    return uVar16;
                }
            }
            DVar9 = (DWORD)PECMD_SaveImageToFile(ptVar20,
                    (LPCWSTR)(-(uint64_t)(WVar1 != L'\0') & (uint64_t)(uintptr_t)local_90),
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
    uVar13 = ((longlong)(int)local_a0 * 100 + (longlong)((int)local_bc / 2)) / (longlong)(int)local_bc;
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
                uVar19 = (ulonglong)
                         (local_80 -
                          (int)(((longlong)(iVar7 / 2) +
                                 (longlong)local_70.rc.bottom * 100) / (longlong)iVar7));
            }
        }
        else if (local_70.uEdge == 0) {
            uVar26 = (ulonglong)(uint)local_70.rc.right;
            if (cVar4 < '\x01') {
                uVar26 = ((longlong)(iVar24 / 2) + (longlong)local_70.rc.right * 100) /
                         (longlong)iVar24;
            }
            uVar15 = (ulonglong)local_b8;
            uVar21 = (ulonglong)(uVar5 - local_70.rc.right);
            if (cVar4 < '\x01') {
                uVar21 = (ulonglong)
                         (local_bc -
                          (int)(((longlong)(iVar7 / 2) +
                                 (longlong)local_70.rc.right * 100) / (longlong)iVar7));
            }
        }
        else {
            if (local_70.uEdge != 2) {
                uVar10 = GetSystemMetrics(0x3d);
                uVar11 = GetSystemMetrics(0x3e);
                if ('\0' < cVar4) {
                    lVar22 = (longlong)(int)uVar10 * (longlong)iVar24 +
                             (longlong)(iVar24 / 2);
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
        FUN_1400669c4(param_1, uVar21 & 0xffffffff, local_90);
    }
    if (*local_c8 != L'\0') {
        FUN_1400669c4(param_1, uVar19 & 0xffffffff, local_c8);
    }
    *local_c8 = L'\0';
    PECMD_ParseSkipSeparator((longlong *)local_res10, (longlong *)&local_c8, 0x2c, 0);
    if (*local_c8 != L'\0') {
        FUN_1400669c4(param_1, uVar26 & 0xffffffff, local_c8);
    }
    *local_c8 = L'\0';
    PECMD_ParseSkipSeparator((longlong *)local_res10, (longlong *)&local_c8, 0x2c, 0);
    if (*local_c8 != L'\0') {
        FUN_1400669c4(param_1, uVar15 & 0xffffffff, local_c8);
    }
    *local_c8 = L'\0';
    PECMD_ParseSkipSeparator((longlong *)local_res10, (longlong *)&local_c8, 0x2c, 0);
    if (*local_c8 != L'\0') {
        FUN_1400669c4(param_1, (ulonglong)local_70.uEdge, local_c8);
    }
    *local_c8 = L'\0';
    PECMD_ParseSkipSeparator((longlong *)local_res10, (longlong *)&local_c8, 0x2c, 0);
    if (*local_c8 != L'\0') {
        FUN_1400669c4(param_1, (ulonglong)local_b0.left, local_c8);
    }
    *local_c8 = L'\0';
    PECMD_ParseSkipSeparator((longlong *)local_res10, (longlong *)&local_c8, 0x2c, 0);
    if (*local_c8 != L'\0') {
        FUN_1400669c4(param_1, (ulonglong)(uint32_t)local_98, local_c8);
    }
    *local_c8 = L'\0';
    PECMD_ParseSkipSeparator((longlong *)local_res10, (longlong *)&local_c8, 0x2c, 0);
    if (*local_c8 != L'\0') {
        FUN_1400669c4(param_1, uVar13 & 0xffffffff, local_c8);
    }
    *local_c8 = L'\0';
    PECMD_ParseSkipSeparator((longlong *)local_res10, (longlong *)&local_c8, 0x2c, 0);
    if (*local_c8 != L'\0') {
        FUN_1400669c4(param_1, uVar14 & 0xffffffff, local_c8);
    }
LAB_1400987b7:
    FUN_14005b104((longlong *)&local_c8);
    FUN_14005b104((longlong *)&local_90);
    return uVar16;
}
/* ================================================================
 * @0x140095178  (longlong PECMD_VolumeDeviceCommand(longlong*, LPCWSTR))
 * 卷 / 设备信息命令: setvol / idle / idlec / rsort / sort / devs /
 * link? / w, 以及盘符文件系统 (FAT/NTFS/...)、DosDevice、卷序列号等。
 * ================================================================ */
longlong PECMD_VolumeDeviceCommand(longlong *param_1, LPCWSTR param_2)
{
    LPWSTR    pWVar1;
    char      cVar2;
    char      cVar3;
    BOOL      BVar4;
    DWORD     DVar5;
    int       iVar6;
    int       iVar7;
    undefined8 uVar8;
    uint     *puVar9;
    WCHAR    *pWVar10;
    longlong  lVar11;
    WCHAR    *pWVar12;
    WCHAR     WVar13;
    LPCWSTR  *ppWVar14;
    WCHAR    *pWVar15;
    longlong  lVar16;
    HANDLE    pvVar17;
    LPCWSTR   pWVar18;
    LPCWSTR   pWVar19;
    char      cVar20;
    LPCWSTR   pWVar21;
    uint      uVar22;
    uint      uVar23;
    int       iVar24;
    WCHAR    *local_res10;
    uint64_t  local_res20;
    uint      local_1128;
    DWORD     local_1124;
    WCHAR    *local_1120;
    DWORD     local_1118[2];
    uint64_t  local_1110;
    uint64_t  local_1108;
    WCHAR    *local_1100;
    WCHAR    *local_10f8;
    WCHAR     local_10f0[5];
    WCHAR     local_10e6;
    WCHAR     local_10e4;
    uint64_t  local_10e0;
    WCHAR    *local_10d8;
    uint64_t  local_10d0;
    WCHAR    *local_10c8;
    uint      local_10c0[2];
    HANDLE    local_10b8;
    WCHAR    *local_10b0;
    WCHAR    *local_10a8;
    WCHAR    *local_10a0;
    WCHAR    *local_1098;
    WCHAR    *local_1090;
    WCHAR     local_1088[28];
    WCHAR     local_1050[10];
    WCHAR     local_103c[8];
    uint8_t   auStack_102c[4076];

    local_res10 = (WCHAR *)param_2;
    FUN_14005b154(&local_res10);
    PECMD_AllocWStringBuffer(&local_1120, 5);
    PECMD_AllocWStringBuffer(&local_10d8, 0x14);
    FUN_140063620(&local_10f8);
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
            uVar8 = FUN_14005b1a8((ushort *)&g_wsz28038, (undefined8 *)&local_res10, 3);
            if ((int)uVar8 == 0) {
                uVar8 = FUN_14005b1a8((ushort *)WSTR("setvol"), (undefined8 *)&local_res10, 6);
                if ((int)uVar8 == 0) {
                    uVar8 = FUN_14005b1a8((ushort *)WSTR("idle"), (undefined8 *)&local_res10, 4);
                    if ((int)uVar8 == 0) {
                        uVar8 = FUN_14005b1a8((ushort *)WSTR("idlec"), (undefined8 *)&local_res10, 5);
                        if ((int)uVar8 == 0) {
                            pWVar19 = local_res10;
                            uVar8 = FUN_14005c788("idlec:", (ushort *)local_res10, 6);
                            if ((char)uVar8 == '\0') {
                                uVar8 = FUN_14005c788("idle:", (ushort *)pWVar19, 5);
                                if ((char)uVar8 == '\0') {
                                    uVar8 = FUN_14005b1a8((ushort *)WSTR("rsort"),
                                                          (undefined8 *)&local_res10, 5);
                                    if ((int)uVar8 == 0) {
                                        uVar8 = FUN_14005b1a8((ushort *)WSTR("sort"),
                                                              (undefined8 *)&local_res10, 4);
                                        if ((int)uVar8 == 0) {
                                            uVar8 = FUN_14005b1a8((ushort *)&g_wsz27fd0,
                                                                  (undefined8 *)&local_res10, 2);
                                            if ((int)uVar8 == 0) {
                                                uVar8 = FUN_14005b1a8((ushort *)&g_wsz27fc8,
                                                                      (undefined8 *)&local_res10, 3);
                                                if ((int)uVar8 == 0) {
                                                    uVar8 = FUN_14005b1a8((ushort *)WSTR("devs"),
                                                                          (undefined8 *)&local_res10, 4);
                                                    if ((int)uVar8 == 0) {
                                                        cVar3 = FUN_1400660ac("link?", &local_res10, 5);
                                                        if (cVar3 == '\0') {
                                                            cVar3 = FUN_1400660ac("w", &local_res10, 1);
                                                            if (cVar3 == '\0') {
                                                                WVar13 = *local_res10;
                                                                while ((WVar13 != L'\0' &&
                                                                       (((uVar23 = local_1128,
                                                                         (ushort)WVar13 < 9 ||
                                                                         (0xd < (ushort)WVar13)) &&
                                                                         (WVar13 != L' '))))) {
                                                                    local_res10 = local_res10 + 1;
                                                                    WVar13 = *local_res10;
                                                                }
                                                                FUN_14005b154(&local_res10);
                                                            }
                                                            else {
                                                                uVar22 = 0x200;
                                                            }
                                                        }
                                                        else {
                                                            local_10e0 = (local_10e0 & 0xffffffff00000000ULL) | 1;
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
                                                local_1110 = (local_1110 & 0xffffffff00000000ULL) | 0x100;
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
                            FUN_14005b154(&local_res10);
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
    FUN_140003a20(param_1, &local_1120, 0);
    pWVar18 = local_res10;
    if (*local_res10 == L'=') {
        pWVar18 = local_res10 + 1;
        local_res10 = (WCHAR *)pWVar18;
        PECMD_SplitTokenTrimWs((longlong *)&local_res10, (longlong *)&local_10d8, 0);
    }
    if (*local_1120 == L'\0') {
        FUN_14005b104((longlong *)&local_10f8);
        FUN_14005b104((longlong *)&local_10d8);
        FUN_14005b104((longlong *)&local_1120);
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
                    if (*pWVar10 == L'\0') break;
                    *pWVar12 = L'\r';
                    pWVar12[1] = L'\n';
                    pWVar12 = pWVar12 + 2;
                }
            }
            *pWVar12 = L'\0';
            local_10b8 = pvVar17;
            FUN_1400629b8(param_1, local_1120, pWVar15);
            if (pvVar17 == (HANDLE)0x0) {
                DVar5 = GetLastError();
                pWVar21 = (LPCWSTR)(ulonglong)DVar5;
            }
            ppWVar14 = (LPCWSTR *)&local_res20;
        }
        else {
            pWVar10 = FUN_14001be14(local_10d8);
            if ((uint32_t)local_10e0 == 0) {
                pWVar12 = (WCHAR *)PECMD_StripDevicePrefix((longlong)(uintptr_t)pWVar10);
                PECMD_AllocWStringBuffer(&local_1100, 0x517);
                pWVar1 = local_1100 + 0x209;
                if (local_1124 == 0) {
                    if ((local_1128 & 1) == 0) {
                        local_res10 = pWVar12;
                        if (*pWVar12 == L'\0') {
                            cVar2 = '|';
                        }
                        else {
                            uVar8 = FUN_14005c7c4("*:", (ushort *)pWVar12);
                            cVar20 = '\x01';
                            if ((char)uVar8 == '\0') {
                                uVar8 = FUN_14005c7c4("*", (ushort *)pWVar12);
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
                                                    if (WVar13 == L'\\') break;
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
                        pWVar12 = (WCHAR *)PECMD_ListDrives((undefined8 *)&local_10f8, (WCHAR)cVar2,
                                                         (int)cVar20,
                                                         (int)local_res20,
                                                         uVar22 | (uint)(uintptr_t)local_1110 |
                                                         local_1118[0], local_10c8);
LAB_1400966dc:
                        FUN_1400629b8(param_1, local_1120, pWVar12);
                        FUN_14005b104((longlong *)&local_1100);
                        FUN_14005b104((longlong *)&local_10f8);
                        FUN_14005b104((longlong *)&local_10d8);
                        FUN_14005b104((longlong *)&local_1120);
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
                    if ((WVar13 != L'\0') &&
                        ((pWVar12[1] == L'\0' || ((pWVar12[1] == L':' && (pWVar12[2] == L'\0')))))) {
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
                        BVar4 = GetVolumeInformationW(pWVar12, local_1100, 0x208,
                                                      (LPDWORD)&local_10d0, &local_1124,
                                                      local_1118, pWVar1, 0x104);
                        if (BVar4 != 0) {
                            local_10e0 = (uint64_t)(uint32_t)local_10e0;
                            local_10e0 = ((uint64_t)0 << 32) | 0;
                            goto LAB_140095d85;
                        }
                        local_10e0 = (local_10e0 & 0xffffffff00000000ULL) | GetLastError();
                        if ((uint32_t)local_10e0 == 0) goto LAB_140095d85;
                    }
                    pWVar1[0] = L'\0';
                    pWVar1[1] = L'\0';
                    pWVar1[2] = L'\0';
                    pWVar1[3] = L'\0';
                    local_1118[0] = 0;
                    local_1124 = 0;
                    local_10d0 = 0;
                    FUN_14005c828("GetVolumeInformationByHandleW", "Kernel32.DLL",
                                  (void **)&g_pGetVolumeInformationByHandleW, &g_hKernel32);
                    if (g_pGetVolumeInformationByHandleW != (DWORD (*)(HANDLE, LPWSTR, DWORD, LPDWORD))0) {
                        local_res20 = 0;
                        PECMD_OpenFileHandle(&local_res20, pWVar10, 0x80000000, 7,
                                      (LPSECURITY_ATTRIBUTES)0x0, 3, 0, (HANDLE)0x0);
                        pWVar21 = (LPCWSTR)(uintptr_t)local_res20;
                        iVar6 = (int)(*g_pGetVolumeInformationByHandleW)((HANDLE)(uintptr_t)local_res20,
                                                      local_1100, 0x208, (LPDWORD)&local_10d0);
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
                    FUN_140063620(&local_10c8);
                    FUN_140063620(&local_10a0);
                    FUN_140063620(&local_1098);
                    FUN_140063620(&local_10a8);
                    FUN_140063620(&local_10b0);
                    FUN_140063620(&local_1090);
                    PECMD_SplitTokenTrimWs((longlong *)&local_res10, (longlong *)&local_10c8, 0x2c);
                    if (*local_res10 != L'\0') {
                        local_res10 = local_res10 + 1;
                        PECMD_SplitTokenTrimWs((longlong *)&local_res10, (longlong *)&local_10a8, 0x2c);
                        if (*local_res10 != L'\0') {
                            local_res10 = local_res10 + 1;
                            PECMD_SplitTokenTrimWs((longlong *)&local_res10, (longlong *)&local_10a0, 0x2c);
                            if (*local_res10 != L'\0') {
                                local_res10 = local_res10 + 1;
                                PECMD_SplitTokenTrimWs((longlong *)&local_res10, (longlong *)&local_1098, 0x2c);
                                if (*local_res10 != L'\0') {
                                    local_res10 = local_res10 + 1;
                                    PECMD_SplitTokenTrimWs((longlong *)&local_res10, (longlong *)&local_10b0, 0x2c);
                                    if (*local_res10 != L'\0') {
                                        local_res10 = local_res10 + 1;
                                        PECMD_SplitTokenTrimWs((longlong *)&local_res10, (longlong *)&local_1090, 0x2c);
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
                                        local_1110 = (uint64_t)(uintptr_t)WSTR("%02X%02X%02X%02X%02X%02X%02X%02X");
                                    }
                                    else {
                                        iVar6 = StrCmpNIW(WSTR("EXT2"), (LPCWSTR)local_1100 + 0x209, 3);
                                        if ((iVar6 == 0) &&
                                            (local_1100[0x20c] == L'9' ||
                                             (local_1100[0x20c] + (WCHAR)0xff10) < 9)) {
                                            iVar24 = 0x468;
                                            local_1128 = 3;
                                            local_1110 = (uint64_t)(uintptr_t)WSTR(
                                                "%02x%02x%02x%02x-%02x%02x-%02x%02x-%02x%02x-%02x%02x%02x%02x%02x%02x");
                                        }
                                        else {
                                            iVar6 = lstrcmpW(WSTR("CDFS"), (LPCWSTR)local_1100 + 0x209);
                                            if ((iVar6 == 0) ||
                                                ((iVar6 = lstrcmpW(WSTR("UDF"), (LPCWSTR)local_1100 + 0x209),
                                                  iVar6 == 0 ||
                                                  (iVar6 = StrCmpNIW(WSTR("ISO9600"),
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
                            PECMD_SetFilePointer(local_10b8,
                                          (LARGE_INTEGER)(longlong)(int)
                                             ((iVar24 / (int)(uintptr_t)local_1108) *
                                              (uintptr_t)local_1108),
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
                                          (ulonglong)*(byte *)((longlong)(uintptr_t)local_1108 + 3),
                                          (ulonglong)(byte)pWVar12[1]);
                            }
                            else if (local_1128 == 2) {
                                wsprintfW(local_1088, (LPCWSTR)(uintptr_t)local_1110,
                                          (ulonglong)*(byte *)((longlong)(uintptr_t)local_1108 + 7),
                                          (ulonglong)(byte)pWVar12[3]);
                            }
                            else if (local_1128 == 3) {
                                wsprintfW(local_1088, (LPCWSTR)(uintptr_t)local_1110,
                                          (ulonglong)(byte)*(WCHAR *)(uintptr_t)local_1108,
                                          (ulonglong)*(byte *)((longlong)(uintptr_t)pWVar12 + 1));
                            }
                            else if (local_1128 == 4) {
                                wsprintfW(local_1088, (LPCWSTR)(uintptr_t)local_1110,
                                          (ulonglong)(byte)*(WCHAR *)(uintptr_t)local_1108,
                                          (ulonglong)*(byte *)((longlong)(uintptr_t)pWVar12 + 1));
                            }
                        }
                        FUN_1400629b8(param_1, local_1090, local_1088);
                        if ((pvVar17 != (HANDLE)0x0) &&
                            ((uintptr_t)pvVar17 != 0xffffffffffffffff)) {
                            CloseHandle(pvVar17);
                        }
                    }
                    FUN_1400629b8(param_1, local_10c8, local_1100);
                    PECMD_FormatSetVar(param_1, (ulonglong)(uint32_t)local_10d0, local_10a0,
                                  WSTR("%08lX"));
                    PECMD_VarSetUInt(param_1, (ulonglong)local_1124, local_1098);
                    PECMD_FormatSetVar(param_1, (ulonglong)local_1118[0], local_10b0, WSTR("0x%lX"));
                    FUN_1400629b8(param_1, local_10a8, (LPCWSTR)local_1100 + 0x209);
                    FUN_14005b104((longlong *)&local_1090);
                    FUN_14005b104((longlong *)&local_10b0);
                    FUN_14005b104((longlong *)&local_10a8);
                    FUN_14005b104((longlong *)&local_1098);
                    FUN_14005b104((longlong *)&local_10a0);
                    FUN_14005b104((longlong *)&local_10c8);
                    FUN_14005b104((longlong *)&local_1100);
                    FUN_14005b104((longlong *)&local_10f8);
                    FUN_14005b104((longlong *)&local_10d8);
                    FUN_14005b104((longlong *)&local_1120);
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
                                if (WVar13 == L'\0') break;
                            } while (local_res10 < pWVar12);
                            *pWVar12 = L'\r';
                            *pWVar15 = L'\n';
                            pWVar12 = pWVar15 + -2;
                        }
                    }
                }
                FUN_1400629b8(param_1, local_1120, local_1100);
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
                FUN_140063620(&local_1110);
                FUN_140063620(&local_1100);
                FUN_140063620(&local_res20);
                PECMD_ParseSkipSeparator((longlong *)&local_res10, (longlong *)&local_1110, 0x2c, 0);
                PECMD_ParseSkipSeparator((longlong *)&local_res10, (longlong *)&local_1100, 0x2c, 0);
                PECMD_ParseSkipSeparator((longlong *)&local_res10, (longlong *)&local_res20, 0x2c, 0);
                local_10e0 = 0;
                local_1108 = 0;
                local_10d0 = 0;
                lVar11 = PECMD_EnumNtSymbolicLink(pWVar10,
                              (longlong *)(-(ulonglong)(*((WCHAR *)(uintptr_t)local_1110) != L'\0') & (ulonglong)&local_10e0),
                              (longlong *)(-(ulonglong)(*((WCHAR *)(uintptr_t)local_1100) != L'\0') & (ulonglong)&local_1108),
                              (longlong *)(-(ulonglong)(*((WCHAR *)(uintptr_t)local_res20) != L'\0') & (ulonglong)&local_10d0));
                if (*((WCHAR *)(uintptr_t)local_1110) != L'\0') {
                    FUN_1400629b8(param_1, (LPCWSTR)(uintptr_t)local_1110, (LPCWSTR)(uintptr_t)local_10e0);
                }
                if (*((WCHAR *)(uintptr_t)local_1100) != L'\0') {
                    FUN_1400629b8(param_1, (LPCWSTR)(uintptr_t)local_1100, (LPCWSTR)(uintptr_t)local_1108);
                }
                if (*((WCHAR *)(uintptr_t)local_res20) != L'\0') {
                    FUN_1400629b8(param_1, (LPCWSTR)(uintptr_t)local_res20, (LPCWSTR)(uintptr_t)local_10d0);
                }
                if (lVar11 != 0) {
                    DVar5 = GetLastError();
                    pWVar21 = (LPCWSTR)(ulonglong)DVar5;
                }
                FUN_14005b104((longlong *)&local_10d0);
                FUN_14005b104((longlong *)&local_1108);
                FUN_14005b104((longlong *)&local_10e0);
                FUN_14005b104((longlong *)&local_res20);
                FUN_14005b104((longlong *)&local_1100);
                ppWVar14 = (LPCWSTR *)&local_1110;
            }
        }
        FUN_14005b104((longlong *)ppWVar14);
    }
    FUN_14005b104((longlong *)&local_10f8);
    FUN_14005b104((longlong *)&local_10d8);
    FUN_14005b104((longlong *)&local_1120);
    return (longlong)(int)(uintptr_t)pWVar21;
}

/* ================================================================
 * @0x140099f18  (ulonglong PECMD_WlanManage(longlong*, LPCWSTR))
 * WLAN 管理命令 (-wlan): 枚举/扫描/连接/断开/设置配置文件/查询。
 * 依赖惰性加载 wlanapi 函数指针槽 (g_pWlanOpenHandle..7d0)。
 * ================================================================ */
ulonglong PECMD_WlanManage(longlong *param_1, LPCWSTR param_2)
{
    WCHAR      WVar1;
    bool       bVar2;
    bool       bVar3;
    bool       bVar4;
    bool       bVar5;
    ulonglong *lpString;
    LPWSTR     pWVar6;
    char       cVar7;
    bool       bVar8;
    int        iVar9;
    int        iVar10;
    DWORD      DVar11;
    undefined8 uVar12;
    longlong   lVar13;
    WCHAR   *pwVar14;
    LPWSTR    *ppWVar15;
    WCHAR    wVar16;
    uint       uVar17;
    LPCWSTR   *ppWVar18;
    LPCWSTR    pWVar19;
    LPWSTR     pWVar20;
    LPCWSTR    pWVar21;
    char       cVar22;
    LPWSTR     pWVar23;
    short     *psVar24;
    uint       uVar25;
    undefined2 *puVar26;
    WCHAR   *pwVar27;
    longlong   lVar28;
    int        iVar29;
    LPCWSTR    pWVar30;
    WCHAR   *pwVar31;
    ulonglong  uVar32;
    ulonglong  uVar33;
    LPWSTR     pWVar34;
    LPCWSTR    pWVar35;
    LPCWSTR    local_res10;
    uint64_t   local_res20;
    char       local_c57;
    char       local_c53;
    WCHAR   *local_c50;
    uint       local_c48[2];
    LPCWSTR    local_c40;
    uint64_t   local_c38;
    LPCWSTR    local_c30;
    uint8_t    local_c28;
    char       local_c27;
    int       *local_c20;
    LPCWSTR    local_c18;
    LPCWSTR    local_c10;
    uint64_t   local_c08;
    LPWSTR     local_c00;
    WCHAR   *local_bf8;
    LPWSTR     local_bf0;
    uint64_t   local_be8;
    LPCWSTR    local_be0;
    LPCWSTR    local_bd8;
    uint32_t   local_bd0;
    WCHAR   *local_bc8;
    WCHAR   *local_bc0;
    longlong   local_bb8;
    uint64_t   local_bb0;
    LPWSTR     local_ba8;
    longlong   local_ba0;
    LPWSTR     local_b98;
    LPWSTR     local_b90;
    LPCWSTR    local_b88;
    uint       local_b80;
    uint32_t   uStack_b7c;
    uint32_t   local_b78[4];
    LPWSTR     local_b68;
    ulonglong  local_b60[2];
    uint8_t   *local_b50;
    uint32_t   local_b40;
    uint32_t   local_b3c;
    uint8_t    local_b38[48];
    WCHAR      local_b08;
    ulonglong  local_b06[9];
    undefined8 local_ab8[336];

    (void)bVar2; (void)bVar3; (void)lpString; (void)pWVar6; (void)DVar11;
    (void)pwVar14; (void)ppWVar15; (void)wVar16; (void)pWVar19;
    (void)pWVar20; (void)pWVar21; (void)pWVar23; (void)psVar24;
    (void)puVar26; (void)pwVar27; (void)lVar28; (void)iVar29;
    (void)pWVar34; (void)pWVar35; (void)local_c57; (void)local_c50;
    (void)local_c20; (void)local_c08; (void)local_bb8; (void)local_bb0;
    (void)local_ba0; (void)local_b98; (void)local_b90; (void)local_b80;
    (void)uStack_b7c; (void)local_b78; (void)local_b68; (void)local_b60;
    (void)local_b50; (void)local_b40; (void)local_b3c; (void)local_b38;
    (void)local_b08; (void)local_b06;

    local_res10 = param_2;
    FUN_14005b154((WCHAR **)&local_res10);
    local_c27 = FUN_1400660ac("-wlan", (WCHAR **)&local_res10, 5);
    uVar33 = 0;
    pWVar34 = (LPWSTR)0x0;
    iVar10 = 0;
    local_be8 = 0;
    local_ba8 = (LPWSTR)0x0;
    FUN_140063620(&local_bf8);
    FUN_140063620(&local_bd8);
    FUN_140063620(&local_bf0);
    FUN_140063620(&local_c10);
    FUN_140063620(&local_c30);
    FUN_140063620(&local_bc0);
    local_bb8 = 0;
    local_bb0 = 0;
    uVar25 = 1;
    local_c48[0] = 1;
    bVar4 = false;
    local_c38 = 0;
    bVar3 = false;
    cVar22 = '\0';
    local_res20 = 0;
    bVar8 = false;
    bVar5 = false;
    bVar2 = false;
    local_c57 = '\x02';
    pWVar30 = local_res10;
    uVar12 = FUN_14005c788("-ssid:", (ushort *)local_res10, 6);
    pWVar20 = local_bf0;
    pWVar21 = local_c10;
    if ((char)uVar12 != '\0') {
        pWVar30 = pWVar30 + 6;
        for (local_res10 = pWVar30; WVar1 = *local_res10, WVar1 != L'\0';
             local_res10 = local_res10 + 1) {
            if (((8 < (ushort)WVar1) && ((ushort)WVar1 < 0xe)) || (WVar1 == L' ')) {
                if (*local_res10 != L'\0') {
                    *((LPWSTR)local_res10) = L'\0';
                    local_res10 = local_res10 + 1;
                }
                break;
            }
        }
        FUN_14005b154((WCHAR **)&local_res10);
        iVar9 = lstrlenW(pWVar30);
        local_be8 = (local_be8 & 0xffffffff00000000ULL) | (uint32_t)iVar9;
        PECMD_AllocString(&local_bf0, (longlong)(iVar9 + 0x68));
        pWVar20 = local_bf0;
        wsprintfW(local_bf0, WSTR("%s.PN"), pWVar30);
        PECMD_AllocString((WCHAR **)&local_c10, (longlong)((int)(uint32_t)local_be8 + 100));
        pWVar21 = local_c10;
        wsprintfW((LPWSTR)local_c10, WSTR("%s"), pWVar30);
    }
    cVar7 = FUN_1400660ac("-start", (WCHAR **)&local_res10, 6);
    local_bd0 = 0;
    if (cVar7 != '\0') {
        local_bd0 = 0xe;
    }
    PECMD_SplitTokenTrimWs((longlong *)&local_res10, (longlong *)&local_bf8, 0x2c);
    if (*local_res10 == L',') {
        local_res10 = local_res10 + 1;
        PECMD_SplitTokenTrimWs((longlong *)&local_res10, (longlong *)&local_bd8, 0x2c);
    }
    if (*local_res10 == L',') {
        local_res10 = local_res10 + 1;
        PECMD_SplitTokenTrimWs((longlong *)&local_res10, (longlong *)&local_bc0, 0x2c);
        local_c53 = '\0';
        cVar22 = '\x02';
        pwVar31 = local_bc0;
        if (*local_bc0 == L'?') {
            local_c53 = (char)*local_bc0;
            pwVar31 = local_bc0 + 1;
        }
        cVar7 = '\x02';
        if (*pwVar31 == L'*') {
            if (pwVar31[1] == L'*') {
                pwVar31 = pwVar31 + 2;
                cVar22 = '\x10';
            }
            cVar7 = cVar22;
            if (*pwVar31 == L'*') {
                pwVar31 = pwVar31 + 1;
                cVar7 = '\0';
            }
        }
        if (*pwVar31 == L'-') {
            pwVar31 = pwVar31 + 1;
            cVar7 = '\x01';
        }
        if (*pwVar31 == L'^') {
            pwVar31 = pwVar31 + 1;
            cVar7 = '\x02';
        }
        iVar9 = lstrcmpiW(WSTR("stop"), pwVar31);
        if (iVar9 == 0) {
            bVar4 = true;
        }
        else {
            iVar9 = lstrcmpiW(WSTR("start+"), pwVar31);
            if (iVar9 == 0) {
                local_bd0 = (local_bd0 & 0xffffff00) | 6;
            }
            else {
                iVar9 = StrCmpNIW(WSTR("start+"), pwVar31, 5);
                if (iVar9 != 0) {
                    iVar9 = lstrcmpiW(WSTR("list+"), pwVar31);
                    if (iVar9 == 0) {
                        local_res20 = (local_res20 & 0xffffffffffffff00ULL) | 3;
                    }
                    else {
                        iVar9 = lstrcmpiW(WSTR("list"), pwVar31);
                        if (iVar9 != 0) {
                            iVar9 = lstrcmpiW(WSTR("queryall"), pwVar31);
                            if (iVar9 == 0) {
                                bVar3 = true;
                                local_c38 = (local_c38 & 0xffffffffffffff00ULL) | 8;
                            }
                            else {
                                iVar9 = lstrcmpiW(WSTR("query"), pwVar31);
                                if (iVar9 == 0) {
                                    local_c38 = (local_c38 & 0xffffffffffffff00ULL) | 1;
                                }
                                else {
                                    iVar9 = lstrcmpiW(WSTR("scan"), pwVar31);
                                    if (iVar9 == 0) {
                                        bVar2 = true;
                                    }
                                    else {
                                        iVar9 = lstrcmpiW(WSTR("liston"), pwVar31);
                                        if (iVar9 == 0) {
                                            bVar8 = true;
                                            bVar5 = true;
                                            cVar22 = '\0';
                                            goto LAB_14009a3f5;
                                        }
                                        FUN_1400679b0((undefined8 *)&local_res10, (int *)local_c48, 0x2c);
                                        uVar25 = local_c48[0];
                                        if ((int)local_c48[0] < 1) {
                                            uVar25 = 0;
                                        }
                                    }
                                }
                            }
                            goto LAB_14009a3ea;
                        }
                        local_res20 = (local_res20 & 0xffffffffffffff00ULL) | 1;
                    }
                    cVar22 = (char)local_res20;
                    uVar17 = (uint)(uint8_t)local_c38;
                    bVar8 = false;
                    if (local_c53 != '\0') {
                        uVar17 = 1;
                    }
                    local_c38 = (local_c38 & 0xffffffff00000000ULL) | (uint32_t)uVar17;
                    local_c57 = cVar7;
                    goto LAB_14009a3f5;
                }
                local_bd0 = (local_bd0 & 0xffffff00) | 2;
            }
        }
LAB_14009a3ea:
        cVar22 = '\0';
        bVar8 = false;
    }
LAB_14009a3f5:
    if (*local_res10 == L',') {
        local_res10 = local_res10 + 1;
        PECMD_SplitTokenTrimWs((longlong *)&local_res10, (longlong *)&local_c30, 0x2c);
    }
    pWVar30 = local_c30;
    if ((cVar22 != '\0') || (bVar8)) {
        FUN_14007d0ac(param_1, local_c30, (LPCWSTR)g_szEmpty);
    }
    pwVar31 = local_bf8;
    if (local_c27 == '\0') {
        FUN_1400e4d94((uint64_t)(uintptr_t)&local_c28);
        bVar8 = FUN_1400e4d84() != 0;
        uVar32 = 1;
        if (bVar8) {
            if (bVar5) {
                FUN_140063620(&local_res10);
                FUN_1400e7758((longlong *)&local_res10);
                FUN_14007d0ac(param_1, pWVar30, local_res10);
                ppWVar18 = &local_res10;
            }
            else {
                if (cVar22 == '\0') {
                    if (bVar4) {
                        uVar33 = FUN_1400e7664(pWVar30);
                        uVar32 = (ulonglong)(int)uVar33;
                    }
                    else {
                        local_c40 = (LPCWSTR)0x0;
                        PECMD_WideToAnsiStr((longlong *)&local_c40, local_bf8, -1, 0xffffffffffffffff);
                        local_c00 = (LPWSTR)0x0;
                        PECMD_WideToAnsiStr((longlong *)&local_c00, local_bd8, -1, 0xffffffffffffffff);
                        local_c18 = (LPCWSTR)0x0;
                        PECMD_WideToAnsiStr((longlong *)&local_c18, pWVar30, -1, 0xffffffffffffffff);
                        PECMD_AllocSmallObject(&local_res20);
                        PECMD_AllocSmallObject((undefined8 *)&local_res10);
                        lVar13 = PECMD_Base64Decode((byte *)(uintptr_t)local_c40, &local_res20);
                        if ((lVar13 != 0) &&
                            (lVar13 = PECMD_Base64Decode((byte *)(uintptr_t)local_c00, (undefined8 *)&local_res10),
                             lVar13 != 0)) {
                            FUN_140063620(&local_be0);
                            FUN_140063620(&local_b88);
                            FUN_1400702f0((longlong *)&local_ba8, (LPCSTR)(uintptr_t)local_res20,
                                          0xffffffffffffffff);
                            FUN_1400702f0((longlong *)&local_bc8, (LPCSTR)(uintptr_t)local_res10,
                                          0xffffffffffffffff);
                            WVar1 = *pWVar30;
                            do {
                                uVar25 = uVar25 - 1;
                                if ((int)uVar25 < 0) {
LAB_14009ba25:
                                    FUN_14005b0b8((ulonglong *)local_res20);
                                    FUN_14005b0b8((ulonglong *)local_res10);
                                    FUN_14005b104(&local_bc8);
                                    FUN_14005b104(&local_ba8);
                                    FUN_14005b104(&local_b88);
                                    FUN_14005b104(&local_be0);
                                    FUN_14005b104(&local_res10);
                                    FUN_14005b104(&local_res20);
                                    FUN_14005b104(&local_c18);
                                    FUN_14005b104(&local_c00);
                                    FUN_14005b104(&local_c40);
                                    FUN_14005b104(&local_bc0);
                                    FUN_14005b104(&local_c30);
                                    PECMD_FreeContainer(&local_c10);
                                    PECMD_FreeContainer(&local_bf0);
                                    PECMD_FreeContainer(&local_bd8);
                                    PECMD_FreeContainer(&local_bf8);
                                    return (uint64_t)(iVar10 == 0);
                                }
                                uVar12 = FUN_1400ebd30((longlong)(uintptr_t)local_ab8,
                                              (LPCWSTR)(uintptr_t)(-(ulonglong)(WVar1 != L'\0') &
                                              (ulonglong)(uintptr_t)pWVar30),
                                              local_be0, local_ba8, local_bc8, local_b88);
                                iVar10 = (int)uVar12;
                                if (iVar10 != 0) {
                                    FUN_1400e4e94(local_ab8);
                                    goto LAB_14009ba25;
                                }
                                FUN_1400e4e94(local_ab8);
                            } while (true);
                        }
                        FUN_14005b104((longlong *)&local_res10);
                        FUN_14005b104((longlong *)&local_res20);
                        FUN_14005b104((longlong *)&local_c18);
                        FUN_14005b104((longlong *)&local_c00);
                        FUN_14005b104((longlong *)&local_c40);
                        uVar32 = 0xffffffff80070057;
                    }
                    goto LAB_14009b90c;
                }
                FUN_140063620(&local_res20);
                FUN_1400e7840((longlong *)&local_res20, 1);
                FUN_14007d0ac(param_1, pWVar30, (LPCWSTR)(uintptr_t)local_res20);
                ppWVar18 = (LPCWSTR *)&local_res20;
            }
            FUN_14005b104((longlong *)ppWVar18);
            uVar32 = uVar33;
        }
LAB_14009b90c:
        FUN_14005b104((longlong *)&local_bc0);
        FUN_14005b104((longlong *)&local_c30);
        PECMD_FreeContainer((longlong *)&local_c10);
        PECMD_FreeContainer((longlong *)&local_bf0);
        PECMD_FreeContainer((longlong *)&local_bd8);
        PECMD_FreeContainer((longlong *)&local_bf8);
        return uVar32;
    }
}
