/* ====================================================================
 * core_b3m.c — B3-B6 小函数批 11 (140040000-1400bffff)
 *
 * 本批新实现函数全部使用人类可读 PECMD_ 名称，原始地址保留在 @0x 注释。
 *
 * 来源: PECMD原始.EXE (x64, ImageBase=0x140000000)
 *   清理任务对象     PECMD_CleanupTaskEnvObject @0x14004f788
 *   按参数设置变量   PECMD_AssignTypedVariable @0x1400544bc
 *   移除索引项       FUN_14005BE68 @0x14005be68
 *   LZ 解压          FUN_14005C260 @0x14005c260
 *   设置本地时间     PECMD_SetSystemTimeFromUnix @0x14005d390
 *   获取系统版本     PECMD_GetPackedSystemVersion @0x14005ea5c
 *   加载虚拟磁盘 API PECMD_LoadVhdApi @0x14005f60c
 *   消息/关机任务    FUN_1400612CC @0x1400612cc
 *   解析颜色列表     FUN_1400677B0 @0x1400677b0
 *   复制路径段       FUN_14006E030 @0x14006e030
 *   调用 COM 对象    PECMD_DrawImageFileToDC @0x14006e588
 *   窗口区域命中测试 FUN_14006F73C @0x14006f73c
 *   弹出/载入介质    FUN_14006F908 @0x14006f908
 *   解析冒号列表     PECMD_SplitItemSpecFields @0x14006fd1c
 *   写引导扇区       PECMD_WriteMbrPartitionEntry @0x140077fb8
 *   变更设备状态     FUN_140078C68 @0x140078c68
 *   解析短整型列表   FUN_140079CF8 @0x140079cf8
 *   查找 # 资源标记  FUN_140079F50 @0x140079f50
 *   导入环境变量     FUN_14007BF78 @0x14007bf78
 *   显示窗口并设状态 FUN_14007E01C @0x14007e01c
 *   处理控件消息     FUN_140080B0C @0x140080b0c
 *   展开变量字符串   PECMD_ExpandVarsLocked @0x14008569c
 *   设置文件名扩展   PECMD_SetFileNameExtension @0x140096748
 *   查找/创建命名锁  FUN_1400A41FC @0x1400a41fc
 *   添加控件 A       FUN_1400AA53C @0x1400aa53c
 *   添加控件 B       PECMD_AppendCheckboxChild @0x1400b1dec
 *   添加控件 C       PECMD_CreateListItem @0x1400b2b64
 *   添加控件 D       PECMD_CreateLabelItem @0x1400b9204
 *   添加控件 E       PECMD_CreateGroupItem @0x1400b9340
 *   添加控件 F       PECMD_CreateImageItem @0x1400bd620
 *
 * 约定:
 *   - 新实现函数使用 PECMD_ 可读名；未实现依赖仍 extern FUN_ + TODO(verify)
 *   - 全局数据符号先用 extern 声明; 完整链接时统一在 core_globals.c 定义
 * ==================================================================== */
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <time.h>

#include "pecmd_defs.h"
extern WCHAR **FUN_14005B154(WCHAR **pp); /* @0x14005b154 */
extern void FUN_1400629B8(void *script, LPCWSTR key, LPCWSTR value); /* @0x1400629b8 */

/* ---- 未实现依赖 (extern + TODO(verify)) ---- */
extern uint32_t FUN_1400E3288(uint32_t mode, uint32_t flags); /* @0x1400e3288 */
extern void PECMD_ReleaseModuleAndVars(int64_t *a1);
extern void FUN_14004EAA8(void *script, int mode);
extern int64_t PECMD_VarLookup(void *script, LPCWSTR name, void *scope, int64_t len,
                             void *p5);
extern int64_t * PECMD_StrBldCopyAnsi(int64_t *out, const char *src, uint64_t len);
extern int64_t PECMD_ParseHex_4a34(uint16_t *a1);
extern LPCWSTR PECMD_ExtractTableSegment(int64_t a1, int64_t *a2, uint16_t *a3,
                             int64_t *a4, char a5);
extern int32_t PECMD_LoadOle32Apis(void);
extern int64_t *PECMD_LoadImageFromFile(LPCWSTR a1);
extern void PECMD_GetApiProcCached(LPCSTR a1, LPCSTR a2, int64_t *a3,
                          int64_t *a4);
extern void PECMD_AllocStrSlot(void *ps);
extern int64_t *PECMD_WideToAnsiStr(int64_t *ps, LPCWSTR src, int64_t len, uint64_t cap);
extern void FUN_1400633A8(WCHAR **pp, int64_t len);
extern LARGE_INTEGER PECMD_SetFilePointer(HANDLE a1, LARGE_INTEGER a2,
                                   DWORD a3);
extern void PECMD_SplitTokenTrimWs(int64_t *src, int64_t *dst, int16_t delim);
extern void PECMD_CopyTokenTrimmed(int64_t *a1, int64_t *a2, int16_t a3,
                          int16_t a4);
extern void FUN_140003A20(void *script, WCHAR **out, int mode);
extern void PECMD_StrBldCopyWideN(WCHAR **pname, LPCWSTR src, int64_t len);
extern void FUN_14001E5B0(void *script, LPCWSTR name, LPCWSTR value, int64_t a,
                          int64_t b);
extern void PECMD_VarSetUInt(int64_t *a1, uint64_t a2, LPCWSTR a3);
extern bool PECMD_ParseUIntValue(int64_t *a1, int *a2);
extern void FUN_14007D0AC(int64_t *a1, LPCWSTR a2, LPCWSTR a3);
extern int PECMD_EnumClassDevices(uint32_t state, LPCWSTR id, uint32_t flag, int64_t *p4,
                         uint16_t *p5);
extern DWORD FUN_1400195F0(uint64_t a1, int64_t a2, int a3,
                           uint64_t *a4);
extern int PECMD_EnumNetAdapters(int a1, int a2);
extern void thunk_PECMD_GetNetworkConnectionName(LPCSTR a1, int64_t *a2, LPCSTR a3);
extern void FUN_1400F429C(WCHAR **pp, WCHAR ch);
extern WCHAR *FUN_14001BE14(WCHAR *s);
extern uint16_t *PECMD_LastPathSeparator(uint16_t *a1);
extern void PECMD_ResolveVariable(int64_t *a1, LPCWSTR text, int64_t *out,
                          int flag);
extern int64_t PECMD_ResolveIndexedVariable(int64_t a1, uint16_t *a2,
                             int64_t *a3);
extern void FUN_14007BDA8(void *script, LPCWSTR text, WCHAR **out, int c,
                          int d);
extern void FUN_14007A224(void *script, LPCWSTR text, WCHAR **out, int c,
                          int d);
extern uint64_t FUN_140061FFC(uint32_t a1, uint32_t a2,
                              LPCWSTR a3);
extern void *PECMD_SendCtrlMessage_0834(WPARAM a1, uint64_t a2);
extern void FUN_14005D9A8(WPARAM a1, int a2);
extern int64_t *PECMD_CreateEditControl(void *obj, int64_t a2, int a3, int64_t *a4,
                              int a5, int a6, int a7, int a8,
                              int64_t *a9, int64_t *a10, uint32_t a11,
                              WCHAR *a12);
extern int64_t *PECMD_ConstructCheckbox(void *obj, int64_t a2, uint64_t a3,
                              int64_t *a4, int64_t *a5, uint32_t a6,
                              uint32_t a7, int a8);
extern int64_t *PECMD_ConstructControlObject(void *obj, int64_t a2, int a3, int64_t *a4,
                              int a5, int a6, int a7, int a8,
                              int64_t *a9, LPCWSTR a10, LPCWSTR a11,
                              uint32_t a12, char a13, int a14);
extern int64_t *PECMD_CreateControlObject(void *obj, int64_t a2, int a3, int64_t *a4,
                              int a5, int a6, int a7, int a8,
                              int64_t *a9, int64_t *a10, int64_t *a11,
                              LPCWSTR a12, uint32_t a13, int a14);
extern int64_t *PECMD_AddControlObject(void *obj, int64_t a2, int a3, int64_t *a4,
                              int a5, int a6, int a7, int a8,
                              int64_t *a9, uint32_t a10, int *a11,
                              LPCWSTR a12);
extern int64_t *PECMD_CreateControlBody(void *obj, int64_t a2, int a3, int64_t *a4,
                              int a5, int a6, int a7, int a8,
                              int64_t *a9, int64_t *a10, uint32_t *a11,
                              uint32_t a12, uint32_t a13, uint32_t a14);

/* ---- 已实现公共工具 (其他 core_*.c / core_globals.c) ---- */
extern void PECMD_StripTrailingSlash(int64_t *a1);
extern void PECMD_OpenFileHandle(HANDLE *out, LPCWSTR path, DWORD access, DWORD share,
                           LPSECURITY_ATTRIBUTES sa, DWORD disp, DWORD flags,
                           HANDLE tmpl);
extern int FUN_14005F96C(HANDLE a1, int a2);
extern int64_t FUN_1400639F0(int64_t **a1, int64_t *a2,
                                        int64_t *a3, void *a4,
                                        uint32_t a5, int64_t a6);
extern void FUN_14005DAF8(int64_t a1, int *a2, int *a3,
                             int *a4, int *a5);

/* ---- 未覆盖原生 API / CRT 声明 ---- */
extern BOOL GetVersionExW(OSVERSIONINFOW *);
extern void RtlGetNtVersionNumbers(DWORD *, DWORD *, DWORD *);
extern void SetLocalTime(SYSTEMTIME *);
extern struct tm *_localtime64(const int64_t *);

/* ---- 本批引用的全局数据 ---- */
extern CRITICAL_SECTION g_csThreadTbl;
extern int32_t g_qMode;
extern uint8_t g_lowVerFlag;
extern void *g_pRtlGetNtVersionNumbers;           /* RtlGetNtVersionNumbers 函数指针 */
extern int64_t DAT_14013d738;         /* VirtDisk 可用性标志 (int/vn-ptr 冲突, 暂留桩) */
extern int64_t *g_pi64e118;
extern int64_t g_i64E120;
extern uint8_t g_u8D5D0;
extern WCHAR g_szEmpty[];             /* g_szEmpty 空串 */

/* ========== PECMD_CleanupTaskEnvObject @0x14004f788 ==========
 * 清理任务/线程上下文对象：关闭句柄并释放各槽位。
 */
void PECMD_CleanupTaskEnvObject(int64_t task)
{
    HANDLE hObject;

    if (*(int *)(task + 0x230) > 0) {
        EnterCriticalSection((LPCRITICAL_SECTION)&g_csThreadTbl);
        FUN_1400E3288(*(uint32_t *)(task + 0x230), 0);
        *(uint32_t *)(task + 0x230) = 0;
        LeaveCriticalSection((LPCRITICAL_SECTION)&g_csThreadTbl);
    }
    PECMD_StripTrailingSlash((int64_t *)(task + 0x228));
    PECMD_FreeStrBuf((WCHAR **)(task + 0x228));
    PECMD_StripTrailingSlash((int64_t *)(task + 0x220));
    PECMD_FreeStrBuf((WCHAR **)(task + 0x220));
    PECMD_ReleaseModuleAndVars((int64_t *)(task + 0x200));
    PECMD_FreeStrBuf((WCHAR **)(task + 0x1e0));
    PECMD_FreeStrBuf((WCHAR **)(task + 0x1d0));
    PECMD_FreeStrBuf((WCHAR **)(task + 0x1c8));
    PECMD_FreeStrBuf((WCHAR **)(task + 0x1c0));
    hObject = *(HANDLE *)(task + 0x1b8);
    if ((hObject != (HANDLE)0) && (hObject != INVALID_HANDLE_VALUE)) {
        CloseHandle(hObject);
    }
    *(int64_t *)(task + 0x1b8) = 0;
    PECMD_FreeStrBuf((WCHAR **)(task + 0x188));
    PECMD_FreeStrBuf((WCHAR **)(task + 0x180));
    PECMD_FreeStrBuf((WCHAR **)(task + 0x178));
    PECMD_FreeStrBuf((WCHAR **)(task + 0x120));
    PECMD_FreeStrBuf((WCHAR **)(task + 0x108));
    FUN_14004EAA8((void *)(task + 8), 0);
    PECMD_FreeStrBuf((WCHAR **)(task + 0x78));
}

/* ========== PECMD_AssignTypedVariable @0x1400544bc ==========
 * 按参数写入脚本变量；@ 前缀表示直接复制缓冲区到已有变量。
 */
void PECMD_AssignTypedVariable(int64_t *script, LPCWSTR name, LPCWSTR key,
                                uint8_t *data, uint32_t len)
{
    bool bAt;
    LPCSTR pCVar2;
    LPCSTR pCVar5;
    int64_t *plVar3;
    int64_t lVar4;
    uint64_t uVar6;
    int64_t local_res10;

    bAt = *name == L'@';
    if (bAt) {
        name = name + 1;
    }
    if (*name != L'\0') {
        EnterCriticalSection((LPCRITICAL_SECTION)&g_csInit);
        if (bAt) {
            lVar4 = PECMD_VarLookup(script, name, NULL, -1, NULL);
            if (lVar4 != 0) {
                uVar6 = (uint64_t)len;
                if ((*(uint64_t *)(lVar4 + 0x18) & 0x3fffffffffffffffULL) < uVar6) {
                    PECMD_AllocString((void *)(lVar4 + 8), (uVar6 + 1) >> 1);
                    *(uint64_t *)(lVar4 + 0x18) = uVar6;
                }
                memcpy(*(void **)(lVar4 + 8), data, len);
            }
        } else {
            pCVar2 = (LPCSTR)((LPCSTR (*)(uint32_t))g_pinet_ntoa)(*(uint32_t *)(data + 4));
            pCVar5 = (LPCSTR)&g_DefEnv;
            if (pCVar2 != NULL) {
                pCVar5 = pCVar2;
            }
            plVar3 = PECMD_StrBldCopyAnsi(&local_res10, pCVar5, (uint64_t)-1);
            FUN_1400629B8(script, name, (LPCWSTR)*plVar3);
            PECMD_FreeStrBuf((WCHAR **)&local_res10);
        }
        LeaveCriticalSection((LPCRITICAL_SECTION)&g_csInit);
    }
    if (bAt) {
        uVar6 = (uint64_t)len;
    } else {
        uVar6 = (uint64_t)((uint16_t (*)(uint16_t))g_phtons)(*(uint16_t *)(data + 2));
    }
    PECMD_AppendLongDecimal(script, uVar6, key);
}

/* ========== FUN_14005BE68 @0x14005be68 ==========
 * 从三数组索引结构中移除指定项并重连前后链接。
 */
void FUN_14005BE68(int64_t table, int index)
{
    int iVar1;
    int *piVar2;
    int64_t lVar3;
    int iVar4;
    int64_t lVar5;

    lVar5 = (int64_t)index;
    if (*(int *)(table + 0x9430 + lVar5 * 4) != 0x1000) {
        iVar4 = *(int *)(table + 0x502c + lVar5 * 4);
        if (iVar4 == 0x1000) {
            iVar4 = *(int *)(table + 0x1028 + lVar5 * 4);
        } else {
            iVar1 = *(int *)(table + 0x1028 + lVar5 * 4);
            lVar3 = (int64_t)iVar1;
            if (iVar1 != 0x1000) {
                piVar2 = (int *)(table + 0x502c + lVar3 * 4);
                iVar4 = iVar1;
                if (*piVar2 != 0x1000) {
                    do {
                        iVar4 = *piVar2;
                        lVar3 = (int64_t)iVar4;
                        piVar2 = (int *)(table + 0x502c + lVar3 * 4);
                    } while (*piVar2 != 0x1000);
                    *(int *)(table + 0x502c + (int64_t)*(int *)(table + 0x9430 + lVar3 * 4) * 4) =
                        *(int *)(table + 0x1028 + lVar3 * 4);
                    *(int *)(table + 0x9430 + (int64_t)*(int *)(table + 0x1028 + lVar3 * 4) * 4) =
                        *(int *)(table + 0x9430 + lVar3 * 4);
                    *(int *)(table + 0x1028 + lVar3 * 4) =
                        *(int *)(table + 0x1028 + lVar5 * 4);
                    *(int *)(table + 0x9430 + (int64_t)*(int *)(table + 0x1028 + lVar5 * 4) * 4) =
                        iVar4;
                }
                *(int *)(table + 0x502c + lVar3 * 4) =
                    *(int *)(table + 0x502c + lVar5 * 4);
                *(int *)(table + 0x9430 + (int64_t)*(int *)(table + 0x502c + lVar5 * 4) * 4) =
                    iVar4;
            }
        }
        *(int *)(table + 0x9430 + (int64_t)iVar4 * 4) =
            *(int *)(table + 0x9430 + lVar5 * 4);
        lVar3 = (int64_t)*(int *)(table + 0x9430 + lVar5 * 4);
        if (*(int *)(table + 0x502c + lVar3 * 4) == index) {
            *(int *)(table + 0x502c + lVar3 * 4) = iVar4;
        } else {
            *(int *)(table + 0x1028 + lVar3 * 4) = iVar4;
        }
        *(int *)(table + 0x9430 + lVar5 * 4) = 0x1000;
    }
}

/* ========== FUN_14005C260 @0x14005c260 ==========
 * 基于 4096 字节环形缓冲的 LZ 解压，从输入流读取并写输出。
 */
void FUN_14005C260(int64_t lz, uint64_t unused)
{
    uint8_t uVar1;
    uint32_t uVar2;
    int iVar3;
    uint32_t uVar4;
    uint32_t uVar5;
    uint32_t uVar6;
    int64_t lVar7;
    uint64_t uVar8;
    int64_t *r_obj;
    uint32_t (*read_byte)(void);
    int64_t *w_obj;
    void (*write_byte)(void *, uint8_t, int);

    (void)unused;
    uVar6 = 0xfee;
    memset((void *)(lz + 0xc), 0x20, 0xfee);
    r_obj = *(int64_t **)(lz + 0xd438);
    read_byte = (uint32_t (*)(void))(*(int64_t *)r_obj + 8);
    w_obj = *(int64_t **)(lz + 0xd440);
    write_byte = (void (*)(void *, uint8_t, int))(*(int64_t *)w_obj);
    uVar2 = 0;
    for (;;) {
        for (;;) {
            uVar4 = uVar2 >> 9;
            uVar2 >>= 1;
            if ((uVar4 & 1) == 0) {
                uVar2 = read_byte();
                if (uVar2 == 0xffffffff) {
                    return;
                }
                uVar2 |= 0xff00;
            }
            if ((uVar2 & 1) == 0) {
                break;
            }
            iVar3 = (int)read_byte();
            if (iVar3 == -1) {
                return;
            }
            write_byte(w_obj, (uint8_t)iVar3, 1);
            *(char *)((int64_t)(int)uVar6 + 0xc + lz) = (char)iVar3;
            uVar6 = (uVar6 + 1) & 0xfff;
        }
        uVar4 = read_byte();
        if (uVar4 == 0xffffffff) {
            return;
        }
        uVar5 = read_byte();
        if (uVar5 == 0xffffffff) {
            break;
        }
        uVar4 = uVar4 | (uVar5 & 0xf0) << 4;
        uVar8 = (uint64_t)((uVar5 & 0xf) + 3);
        do {
            uVar1 = *(uint8_t *)((uint64_t)(uVar4 & 0xfff) + 0xc + lz);
            write_byte(w_obj, uVar1, 1);
            lVar7 = (int64_t)(int)uVar6;
            uVar6 = (uVar6 + 1) & 0xfff;
            uVar4++;
            uVar8--;
            *(uint8_t *)(lVar7 + 0xc + lz) = uVar1;
        } while (uVar8 != 0);
    }
}

/* ========== PECMD_SetSystemTimeFromUnix @0x14005d390 ==========
 * 由 Unix 时间戳(含毫秒)设置本地系统时间，并按需记录日志。
 */
void PECMD_SetSystemTimeFromUnix(int *time)
{
    struct tm *ptVar1;
    int64_t local_res8;
    SYSTEMTIME local_18;

    local_res8 = (int64_t)*time;
    ptVar1 = _localtime64(&local_res8);
    if (ptVar1 != NULL) {
        local_18.wYear = (WORD)(ptVar1->tm_year + 0x76c);
        local_18.wDayOfWeek = 0;
        local_18.wMonth = (WORD)(ptVar1->tm_mon + 1);
        local_18.wDay = (WORD)ptVar1->tm_mday;
        local_18.wHour = (WORD)ptVar1->tm_hour;
        local_18.wMinute = (WORD)ptVar1->tm_min;
        local_18.wSecond = (WORD)ptVar1->tm_sec;
        local_18.wMilliseconds = (WORD)(time[1] / 1000);
        if (g_qMode < 2) {
            SetLocalTime(&local_18);
        }
        (void)GetLastError();
        if (g_qMode != 0) {
            if (g_qOutBuf != NULL) {
                wsprintfW(g_qOutBuf, WSTR("%04d-%02d-%02d:%02d:%02d:%02d:%05d\n"),
                          (int)local_18.wYear, (int)local_18.wMonth, (int)local_18.wDay,
                          (int)local_18.wHour, (int)local_18.wMinute, (int)local_18.wSecond,
                          (int)local_18.wMilliseconds);
            }
        }
    }
}

/* ========== PECMD_GetPackedSystemVersion @0x14005ea5c ==========
 * 返回打包的系统版本号 (major<<32 | minor<<16 | build)。
 */
uint64_t PECMD_GetPackedSystemVersion(void)
{
    OSVERSIONINFOW local_128;
    uint32_t local_res8[2];
    uint32_t local_res10[2];
    uint32_t local_res18[2];
    int64_t local_res20;

    if (g_osVersion == 0) {
        memset(&local_128, 0, sizeof(local_128));
        local_128.dwOSVersionInfoSize = sizeof(local_128);
        GetVersionExW(&local_128);
        g_lowVerFlag = (uint8_t)(local_128.dwMajorVersion < 0x50001);
        g_osVersion = ((uint64_t)local_128.dwMajorVersion << 32) |
                        ((uint64_t)local_128.dwMinorVersion << 16) |
                        local_128.dwBuildNumber;
        if ((uint64_t)0x60001ffff < g_osVersion) {
            local_res8[0] = 0;
            local_res10[0] = 0;
            local_res18[0] = 0;
            local_res20 = 0;
            PECMD_GetApiProcCached("RtlGetNtVersionNumbers", "NTDLL.DLL",
                          (int64_t *)(void **)&g_pRtlGetNtVersionNumbers, &local_res20);
            if (g_pRtlGetNtVersionNumbers != NULL) {
                ((void (*)(uint32_t *, uint32_t *, uint32_t *))g_pRtlGetNtVersionNumbers)
                    (local_res8, local_res10, local_res18);
            }
            g_osVersion = ((uint64_t)local_res8[0] << 32) |
                            ((uint64_t)local_res10[0] << 16) |
                            local_res18[0];
        }
    }
    return g_osVersion;
}

/* ========== PECMD_LoadVhdApi @0x14005f60c ==========
 * 延迟加载虚拟磁盘/VHD 相关 API，返回是否可用。
 */
uint64_t PECMD_LoadVhdApi(void)
{
    int64_t lVar1;
    uint64_t uVar2;
    int64_t local_res8;
    int64_t local_res10;

    uVar2 = 0;
    if (DAT_14013d738 == 0) {
        local_res8 = 0;
        PECMD_GetApiProcCached("CreateVirtualDisk", "VirtDisk.DLL", (int64_t *)(void **)&g_pCreateVirtualDisk, &local_res8);
        PECMD_GetApiProcCached("AttachVirtualDisk", "VirtDisk.DLL", (int64_t *)(void **)&g_pAttachVirtualDisk, &local_res8);
        PECMD_GetApiProcCached("DetachVirtualDisk", "VirtDisk.DLL", (int64_t *)(void **)&g_pDetachVirtualDisk, &local_res8);
        PECMD_GetApiProcCached("GetVirtualDiskPhysicalPath", "VirtDisk.DLL", (int64_t *)(void **)&g_pGetVirtDiskPhysPath, &local_res8);
        PECMD_GetApiProcCached("GetVirtualDiskInformation", "VirtDisk.DLL", (int64_t *)(void **)&g_pGetVirtDiskInfo, &local_res8);
        local_res10 = 0;
        PECMD_GetApiProcCached("OpenVirtualDisk", "VirtDisk.DLL", &local_res10, &local_res8);
        local_res8 = 0;
        PECMD_GetApiProcCached("UnmountVHD", "vhdmount.dll", (int64_t *)(void **)&g_pUnmountVHD, &local_res8);
        lVar1 = local_res10;
        if (local_res10 == 0) {
            PECMD_GetApiProcCached("MountVHD", "vhdmount.dll", (int64_t *)(void **)&g_pMountVHD, &local_res8);
        }
        DAT_14013d738 = lVar1;
        if ((lVar1 != 0) || (g_pMountVHD != 0)) {
            uVar2 = 1;
        }
    } else {
        uVar2 = 1;
    }
    return uVar2;
}

/* ========== FUN_1400612CC @0x1400612cc ==========
 * 显示消息框/系统关机提示，等待完成信号并递减任务计数。
 */
uint64_t FUN_1400612CC(int64_t *task)
{
    int iVar1;
    LPCWSTR lpText;
    LPCWSTR lpCaption;
    int64_t lVar2;
    BOOL BVar3;
    uint32_t uVar4;
    uint64_t uVar5;
    int iVar6;
    int64_t *local_res8;

    if (*task < 1) {
        uVar5 = 0x80070057;
    } else {
        lVar2 = task[1];
        lpText = (LPCWSTR)task[2];
        lpCaption = (LPCWSTR)task[3];
        uVar4 = *(uint32_t *)(task + 4);
        *(uint32_t *)((uint8_t *)task + 0x2c) = 0;
        local_res8 = task;
        if ((HANDLE)task[7] != (HANDLE)0) {
            SetEvent((HANDLE)task[7]);
        }
        SetLastError(0);
        if ((int)lVar2 == 2) {
            FUN_14001C2CC(WSTR("SeShutdownPrivilege"), 2, 0);
            BVar3 = InitiateSystemShutdownW(NULL, lpText, 0x12cc02f6, 0, 1);
            *(BOOL *)((uint8_t *)task + 0x34) = BVar3;
        } else {
            uVar4 = MessageBoxW((HWND)0, lpText, lpCaption, uVar4 | 0x200000);
            *(uint32_t *)((uint8_t *)task + 0x34) = uVar4 & 0xffff;
        }
        (void)GetLastError();
        iVar1 = (int)task[6];
        iVar6 = 1000;
        while ((iVar1 != 0) && (iVar6--, -1 < iVar6)) {
            Sleep(10);
            iVar1 = (int)task[6];
        }
        EnterCriticalSection((LPCRITICAL_SECTION)&g_csInit);
        *task = *task - 1;
        if (0 < *task) {
            local_res8 = NULL;
        }
        LeaveCriticalSection((LPCRITICAL_SECTION)&g_csInit);
        PECMD_FreeStrBuf((WCHAR **)&local_res8);
        uVar5 = 0;
    }
    return uVar5;
}

/* ========== FUN_1400677B0 @0x1400677b0 ==========
 * 解析由 # 分隔的最多 4 个颜色/数值，@ 前缀取系统色。
 */
int16_t FUN_1400677B0(int64_t *pp, int64_t out)
{
    uint16_t uVar1;
    DWORD DVar2;
    uint16_t *puVar3;
    int64_t lVar4;
    int64_t *plVar5;
    uint16_t *puVar6;
    int64_t lVar7;
    int16_t sVar8;
    int cVar9;
    bool bAt;
    uint16_t *local_res8;

    plVar5 = pp;
    FUN_14005B154((WCHAR **)pp);
    puVar6 = (uint16_t *)*plVar5;
    uVar1 = *puVar6;
    while ((uVar1 != 0) && (*(uint16_t *)*pp != 0x2c)) {
        puVar3 = (uint16_t *)((uint16_t *)*pp + 1);
        *pp = (int64_t)puVar3;
        uVar1 = *puVar3;
    }
    sVar8 = 0;
    for (;;) {
        cVar9 = 1;
        bAt = *puVar6 == 0x40;
        if (bAt) {
            puVar6 = puVar6 + 1;
        }
        if (*puVar6 == 0x2d) {
            cVar9 = -1;
            puVar6 = puVar6 + 1;
        }
        if (((0x2f < *puVar6) && (*puVar6 < 0x3a)) ||
            ((0x60 < (*puVar6 | 0x20)) && ((*puVar6 | 0x20) < 0x67))) {
            if ((*puVar6 == 0x30) && ((puVar6[1] | 0x20) == 0x78)) {
                puVar6 = puVar6 + 2;
            }
            lVar4 = PECMD_ParseHex_4a34(puVar6);
            lVar7 = (int64_t)sVar8;
            *(int *)(out + lVar7 * 4) = (int)lVar4;
            if (cVar9 < 0) {
                *(int *)(out + lVar7 * 4) = -(int)lVar4;
            }
            if (bAt) {
                DVar2 = GetSysColor(*(int *)(out + lVar7 * 4));
                *(DWORD *)(out + lVar7 * 4) = DVar2;
            }
        }
        for (; (puVar6 < (uint16_t *)*pp) && (*puVar6 != 0x23); puVar6 = puVar6 + 1) {
        }
        sVar8 = (int16_t)(sVar8 + 1);
        if (*puVar6 != 0x23) {
            break;
        }
        local_res8 = puVar6 + 1;
        FUN_14005B154((WCHAR **)&local_res8);
        puVar6 = local_res8;
        if (3 < sVar8) {
            return sVar8;
        }
    }
    return sVar8;
}

/* ========== FUN_14006E030 @0x14006e030 ==========
 * 复制路径中的一段 token 到目标字符串，并跳过结束分隔符。
 */
int64_t FUN_14006E030(int64_t script, int64_t *pp, int64_t *out,
                            int64_t limit)
{
    uint16_t uVar1;
    uint16_t uVar2;
    int16_t sVar3;
    int64_t lVar4;
    LPCWSTR pWVar5;
    LPCWSTR pWVar6;
    uint16_t *puVar7;
    int64_t local_res8;

    uVar1 = *(uint16_t *)(script + 0x48);
    lVar4 = *out;
    if (*(uint16_t *)*pp != *(uint16_t *)(script + 0x88)) {
        do {
            puVar7 = (uint16_t *)*pp;
            if (*(uint16_t *)(script + 0x8a) == *puVar7) {
                puVar7 = puVar7 + 1;
            }
            if (*(uint16_t *)(script + 0x90) != *puVar7) {
                goto LAB_14006e097;
            }
            do {
                puVar7 = puVar7 + 1;
LAB_14006e097:
                uVar2 = *puVar7;
            } while ((*(uint16_t *)(script + 0x92) == uVar2) ||
                     (*(uint16_t *)(script + 0x94) == uVar2));
            if ((uVar1 ^ 0x5c) != uVar2) {
                break;
            }
            local_res8 = 0;
            *pp = 0;
            pWVar5 = PECMD_ExtractTableSegment(script, &local_res8, puVar7, pp, '\0');
            pWVar6 = pWVar5 + 1;
            if ((*(WCHAR *)(script + 0x92) == *pWVar6) ||
                (*(WCHAR *)(script + 0x94) == *pWVar6)) {
                pWVar6 = pWVar5 + 2;
            }
            FUN_14006375C((WCHAR **)out, pWVar6);
            PECMD_FreeStrBuf((WCHAR **)&local_res8);
        } while (*(uint16_t *)*pp != *(uint16_t *)(script + 0x88));
    }
    for (;;) {
        sVar3 = *(int16_t *)*pp;
        if (((*(uint16_t *)(script + 0x92) != (uint16_t)sVar3) &&
             (*(uint16_t *)(script + 0x94) != (uint16_t)sVar3) &&
             (*(uint16_t *)(script + 0x8a) != (uint16_t)sVar3) &&
             (*(uint16_t *)(script + 0x90) != (uint16_t)sVar3)) ||
            (*(uint16_t *)*pp == 0)) {
            break;
        }
        *pp = (int64_t)((uint16_t *)*pp + 1);
    }
    return *out + (int64_t)(int)((limit - lVar4) >> 1) * 2;
}

/* ========== PECMD_DrawImageFileToDC @0x14006e588 ==========
 * 在 COM 临界区中获取对象并调用其绘制/显示虚函数。
 */
bool PECMD_DrawImageFileToDC(int64_t ctx, LPCWSTR name, int width, int height,
                           uint32_t flags1, uint32_t flags2)
{
    int iVar1;
    int64_t *plVar2;
    int local_18;
    int local_14[3];

    EnterCriticalSection((LPCRITICAL_SECTION)&g_csCom);
    PECMD_LoadOle32Apis();
    iVar1 = -3;
    if (g_pOleInit != NULL) {
        iVar1 = ((int (*)(int))g_pOleInit)(0);
        if ((iVar1 != 0) && (iVar1 == 1)) {
            ((void (*)(void))g_pOleUninit)();
        }
    }
    plVar2 = PECMD_LoadImageFromFile(name);
    if (plVar2 != NULL) {
        local_18 = 0;
        local_14[0] = 0;
        ((void (*)(int64_t *, int *))(*(int64_t *)plVar2 + 0x30))(plVar2, &local_18);
        ((void (*)(int64_t *, int *))(*(int64_t *)plVar2 + 0x38))(plVar2, local_14);
        if (width < 0) {
            width = local_18;
        }
        if (height < 0) {
            height = local_14[0];
        }
        if (ctx != 0) {
            ((void (*)(int64_t *, int64_t, uint32_t, uint32_t, int, int, int, int, int, int, int))
                 (*(int64_t *)plVar2 + 0x40))
                (plVar2, ctx, flags1, flags2, width, height, 0,
                 local_14[0], local_18, -local_14[0], 0);
        }
        ((void (*)(int64_t *))(*(int64_t *)plVar2 + 0x10))(plVar2);
    }
    if ((g_pOleUninit != NULL) && (iVar1 == 0)) {
        ((void (*)(void))g_pOleUninit)();
    }
    LeaveCriticalSection((LPCRITICAL_SECTION)&g_csCom);
    return plVar2 != NULL;
}

/* ========== FUN_14006F73C @0x14006f73c ==========
 * 按扩展区域/热键状态判断命中位置。
 */
uint64_t FUN_14006F73C(int64_t obj, POINT pt)
{
    int iVar1;
    uint16_t uVar2;
    BOOL BVar3;
    uint64_t uVar4;
    int iVar5;
    RECT local_28;
    RECT local_18;

    if (*(int *)(obj + 0x180) != 0) {
        local_28.left = 0;
        local_28.top = 0;
        local_28.right = 0;
        local_28.bottom = 0;
        GetWindowRect(*(HWND *)(obj + OBJ_HWND), &local_28);
        iVar1 = *(int *)(obj + 0x180);
        local_18.right = local_28.right;
        iVar5 = -iVar1;
        if (-1 < iVar1) {
            iVar5 = iVar1;
        }
        local_18.left = local_28.left;
        if ((*(uint16_t *)(obj + 0x186) != 0) &&
            (local_18.left = (int)*(uint16_t *)(obj + 0x186) + local_28.left,
             local_28.right < local_18.left)) {
            local_18.left = local_28.right;
        }
        local_18.top = local_28.top;
        if ((*(uint16_t *)(obj + 0x188) != 0) &&
            (local_18.top = (int)*(uint16_t *)(obj + 0x188) + local_28.top,
             local_28.bottom < local_18.top)) {
            local_18.top = local_28.bottom;
        }
        if ((*(uint16_t *)(obj + 0x184) != 0) &&
            (local_18.right = (int)*(uint16_t *)(obj + 0x184) + local_18.left,
             local_28.right < local_18.right)) {
            local_18.right = local_28.right;
        }
        local_18.bottom = local_18.top + iVar5;
        if (local_28.bottom < local_18.top + iVar5) {
            local_18.bottom = local_28.bottom;
        }
        BVar3 = PtInRect(&local_18, pt);
        if (BVar3 != 0) {
            if (*(int *)(obj + 0x180) < 1) {
                return 0xffffffffffffffffULL;
            }
            return 2;
        }
    }
    if ((*(char *)(obj + 0x298) == '\0') ||
        ((*(char *)(obj + 0x298) < 2) &&
         (uVar2 = (uint16_t)GetAsyncKeyState(0x12), (uVar2 & 0x8000) == 0))) {
        uVar4 = 0x8000000000000000ULL;
    } else {
        uVar4 = 2;
    }
    return uVar4;
}

/* ========== FUN_14006F908 @0x14006f908 ==========
 * 打开盘符设备并发送弹出/载入介质 IOCTL。
 */
BOOL FUN_14006F908(char drive, int eject)
{
    BOOL BVar1;
    HANDLE hObject;
    DWORD local_res8[2];
    HANDLE local_res18;
    WCHAR local_28[4];
    uint16_t local_20;
    uint16_t local_1e;
    uint16_t local_1c;
    uint16_t local_1a;
    uint16_t local_18;
    uint16_t local_16;

    memcpy(local_28, WSTR("\\\\.\\"), 8);
    local_20 = (uint16_t)(uint8_t)drive;
    local_1e = 0x3a;
    local_res18 = (HANDLE)0;
    local_res8[0] = 0;
    local_1c = 0;
    local_1a = 0;
    local_18 = 0;
    local_16 = 0;
    (void)local_20;
    (void)local_1e;
    (void)local_1c;
    (void)local_1a;
    (void)local_18;
    (void)local_16;
    PECMD_OpenFileHandle(&local_res18, local_28, 0xc0000000, 3, NULL, 3, 0x80, (HANDLE)0);
    if (local_res18 == (HANDLE)0) {
        PECMD_OpenFileHandle(&local_res18, local_28, 0x80000000, 3, NULL, 3, 0x80, (HANDLE)0);
        hObject = local_res18;
        if (local_res18 != (HANDLE)0) {
            goto LAB_14006f9e1;
        }
    } else {
LAB_14006f9e1:
        hObject = local_res18;
        BVar1 = DeviceIoControl(local_res18,
                                (-(uint32_t)(eject != 0) & 0xfffffffcU) + 0x2d480cU,
                                NULL, 0, NULL, 0, local_res8, NULL);
        if (BVar1 != 0) {
            goto LAB_14006fa1d;
        }
    }
    BVar1 = 0;
LAB_14006fa1d:
    if ((hObject != (HANDLE)0) && (hObject != INVALID_HANDLE_VALUE)) {
        CloseHandle(hObject);
    }
    return BVar1;
}

/* ========== PECMD_SplitItemSpecFields @0x14006fd1c ==========
 * 把输入串按 ; 和 : 拆成最多 4 个片段。
 */
void PECMD_SplitItemSpecFields(int64_t *pp, int64_t *out1, int64_t *out2,
                          int64_t *out3, int64_t *out4, int64_t *tmp)
{
    int64_t lVar1;
    uint16_t *psVar2;
    int64_t *plVar3;

    plVar3 = pp;
    FUN_14005B154((WCHAR **)pp);
    *(uint16_t *)*tmp = 0;
    PECMD_SplitTokenTrimWs(plVar3, tmp, 0x3b);
    if (*(uint16_t *)*pp == 0x3b) {
        *pp = (int64_t)((uint16_t *)*pp + 1);
    }
    FUN_14005B154((WCHAR **)pp);
    lVar1 = *tmp;
    *out1 = lVar1;
    *out2 = lVar1;
    *out3 = lVar1;
    psVar2 = (uint16_t *)*out2;
    while ((*psVar2 != 0) && (*(uint16_t *)*out2 != 0x3a)) {
        psVar2 = (uint16_t *)*out2 + 1;
        *out2 = (int64_t)psVar2;
    }
    if (*(uint16_t *)*out2 != 0) {
        *(uint16_t *)*out2 = 0;
        *out2 = *out2 + 2;
    }
    psVar2 = (uint16_t *)*out2;
    while ((*out3 = (int64_t)psVar2, *psVar2 != 0) &&
           (*(uint16_t *)*out3 != 0x3a)) {
        psVar2 = (uint16_t *)*out3 + 1;
    }
    if (*(uint16_t *)*out3 != 0) {
        *(uint16_t *)*out3 = 0;
        *out3 = *out3 + 2;
    }
    psVar2 = (uint16_t *)*out3;
    while ((*out4 = (int64_t)psVar2, *psVar2 != 0) &&
           (*(uint16_t *)*out4 != 0x3a)) {
        psVar2 = (uint16_t *)*out4 + 1;
    }
    if (*(uint16_t *)*out4 != 0) {
        *(uint16_t *)*out4 = 0;
        *out4 = *out4 + 2;
    }
}

/* ========== PECMD_WriteMbrPartitionEntry @0x140077fb8 ==========
 * 读回一个扇区，按需写入引导标记/签名后再写回。
 */
uint64_t PECMD_WriteMbrPartitionEntry(HANDLE hDevice, int64_t sector, uint8_t *data,
                               int mode)
{
    DWORD nNumberOfBytesToRead;
    uint16_t uVar1;
    uint32_t uVar2;
    BOOL BVar3;
    int iVar4;
    LARGE_INTEGER LVar5;
    char *pcVar6;
    uint64_t uVar7;
    LARGE_INTEGER LVar8;
    int64_t local_res10[2];
    DWORD local_res20[2];

    local_res20[0] = 0;
    LVar8.QuadPart = sector << 9;
    uVar2 = (uint32_t)FUN_14005F96C(hDevice, 0x200);
    uVar7 = 0x200;
    if (0x1ff < (int)uVar2) {
        uVar7 = (uint64_t)uVar2;
    }
    nNumberOfBytesToRead = (DWORD)uVar7;
    FUN_1400633A8((WCHAR **)local_res10, (int64_t)(nNumberOfBytesToRead * 2 + 0x200));
    pcVar6 = (char *)((uint64_t)(local_res10[0] + 0x200) & 0xfffffffffffffe00ULL);
    char *lpBuffer = pcVar6 + uVar7;
    LVar5 = PECMD_SetFilePointer(hDevice, LVar8, 0);
    if (LVar5.QuadPart == LVar8.QuadPart) {
        BVar3 = ReadFile(hDevice, lpBuffer, nNumberOfBytesToRead, local_res20, NULL);
        if (BVar3 != 0) {
            memcpy(pcVar6, lpBuffer, nNumberOfBytesToRead);
            if (mode == 1) {
                memcpy(lpBuffer + 0x1ce, data, 0x10);
            }
            uVar1 = 0;
            if (mode == 1) {
                uVar1 = 0xaa55;
            }
            *(uint16_t *)(lpBuffer + 0x1fe) = uVar1;
            LVar5 = PECMD_SetFilePointer(hDevice, LVar8, 0);
            if (LVar5.QuadPart == LVar8.QuadPart) {
                iVar4 = memcmp(pcVar6, lpBuffer, (size_t)uVar7);
                if (iVar4 != 0) {
                    WriteFile(hDevice, lpBuffer, nNumberOfBytesToRead, local_res20, NULL);
                }
            }
        }
    }
    PECMD_FreeStrBuf((WCHAR **)local_res10);
    return 1;
}

/* ========== FUN_140078C68 @0x140078c68 ==========
 * 根据设备实例 ID 启用/禁用/重启设备。
 */
int FUN_140078C68(LPCSTR deviceId, int action, uint32_t flags)
{
    LPCWSTR pWVar1;
    int iVar2;
    WCHAR *local_28[2];
    WCHAR *local_res20;

    memset(local_28, 0, sizeof(local_28));
    local_res20 = NULL;
    iVar2 = 0;
    if (-1 < action) {
        iVar2 = action;
    }
    if ((flags & 2) == 0) {
        if (*deviceId != '\0') {
            thunk_PECMD_GetNetworkConnectionName(deviceId, (int64_t *)local_28, "PnpInstanceID");
        }
        PECMD_WideToAnsiStr((int64_t *)&local_res20, (LPCWSTR)local_28[0], -1, (uint64_t)-1);
        pWVar1 = local_res20;
        if ((char)*local_res20 != '\0') {
            iVar2 = PECMD_EnumClassDevices((uint32_t)(iVar2 != 0), local_res20, flags & 1,
                                  NULL, NULL);
            if (action < 0) {
                FUN_1400195F0((uint64_t)(uintptr_t)g_Script, 0x32, 0, NULL);
                PECMD_EnumClassDevices(1, pWVar1, flags & 1, NULL, NULL);
            }
            goto LAB_140078d61;
        }
    }
    iVar2 = PECMD_EnumNetAdapters(0xffffffff, iVar2);
    if (action < 0) {
        PECMD_EnumNetAdapters(0xffffffff, 1);
    }
LAB_140078d61:
    PECMD_FreeStrBuf((WCHAR **)&local_res20);
    PECMD_FreeStrBuf((WCHAR **)local_28);
    return iVar2;
}

/* ========== FUN_140079CF8 @0x140079cf8 ==========
 * 解析逗号分隔的数字到 8 个 short，支持引号包裹。
 */
uint64_t FUN_140079CF8(int64_t *pp, uint16_t *out, char mode)
{
    uint16_t uVar1;
    int64_t lVar2;
    uint64_t uVar3;
    uint16_t *puVar4;
    int iVar5;
    uint64_t uVar6;
    uint16_t vals[8];

    memset(vals, 0, sizeof(vals));
    puVar4 = (uint16_t *)*pp;
    uVar1 = *puVar4;
    if ((uVar1 == 0x22) || (uVar1 == 0x27)) {
        puVar4 = puVar4 + 1;
        *pp = (int64_t)puVar4;
        FUN_1400F429C((WCHAR **)pp, uVar1);
    }
    uVar3 = 0;
    uVar6 = uVar3;
    do {
        iVar5 = (int)uVar6;
        if (9 < (uint16_t)(*puVar4 - 0x30)) {
            break;
        }
        lVar2 = FUN_140064A88(puVar4);
        vals[uVar3] = (uint16_t)lVar2;
        for (; (uint16_t)(*puVar4 - 0x30) < 10; puVar4 = puVar4 + 1) {
        }
        if (*puVar4 != 0) {
            puVar4 = puVar4 + 1;
        }
        uVar3 = uVar3 + 1;
        uVar6 = (uint64_t)(iVar5 + 1);
    } while ((int64_t)uVar3 < 8);
    out[0] = vals[0];
    out[1] = vals[1];
    out[3] = vals[2];
    if (mode == '\0') {
        out[2] = vals[3];
        out[4] = vals[4];
        out[5] = vals[5];
        out[6] = vals[6];
        out[7] = vals[7];
    } else {
        out[4] = vals[3];
        out[5] = vals[4];
        out[6] = vals[5];
        out[7] = vals[6];
        out[2] = vals[7];
    }
    return uVar6;
}

/* ========== FUN_140079F50 @0x140079f50 ==========
 * 在路径/字符串中定位 # 资源标记，并处理引号与 #. 形式。
 */
LPCWSTR FUN_140079F50(int64_t *pp, uint32_t flags)
{
    WCHAR *pWVar1;
    LPCWSTR pWVar2;
    LPWSTR pWVar3;
    WCHAR WVar4;
    WCHAR *local_res8;
    WCHAR *local_res18[2];

    pWVar2 = (LPCWSTR)*pp;
    WVar4 = L'\"';
    if ((flags & 2) == 0) {
        if (*pWVar2 != L'\"') {
            pWVar2 = StrRChrW(pWVar2, NULL, L'\\');
            if (pWVar2 == NULL) {
                pWVar2 = (LPCWSTR)*pp;
            }
            pWVar3 = StrRChrW(pWVar2, NULL, L'#');
            if (pWVar3 == NULL) {
                return NULL;
            }
            if (*pWVar3 != L'#') {
                return NULL;
            }
            if (9 < (uint16_t)(pWVar3[1] - L'0')) {
                if ((flags & 1) == 0) {
                    return NULL;
                }
                if (pWVar3[1] != L'.') {
                    return NULL;
                }
                if (9 < (uint16_t)(pWVar3[2] - L'0')) {
                    return NULL;
                }
                return pWVar3;
            }
            return pWVar3;
        }
        pWVar2 = pWVar2 + 1;
    }
    local_res8 = (WCHAR *)pWVar2;
    FUN_1400F429C((WCHAR **)&local_res8, 0x22);
    pWVar1 = local_res8;
    if (*local_res8 != L'\0') {
        local_res8 = local_res8 + 1;
    }
    if (WVar4 == *local_res8) {
        local_res8 = local_res8 + 1;
    }
    if (*local_res8 != L'#') {
        return NULL;
    }
    if (*pWVar1 != L'\0') {
        *pWVar1 = L'\0';
    }
    *pp = (int64_t)pWVar2;
    pWVar2 = local_res8;
    local_res18[0] = (WCHAR *)local_res8;
    FUN_1400F429C(local_res18, WVar4);
    if (*local_res18[0] != L'\0') {
        *local_res18[0] = L'\0';
        return local_res8;
    }
    return pWVar2;
}

/* ========== FUN_14007BF78 @0x14007bf78 ==========
 * 把进程环境块导入脚本变量；mode=0 时仅补缺失项。
 */
void FUN_14007BF78(int64_t *script, int64_t *table, int mode)
{
    WCHAR WVar1;
    WCHAR *pWVar2;
    int64_t lVar3;
    LPCWSTR pWVar4;
    LPCWSTR pWVar5;
    LPCWSTR pWVar6;
    int64_t *plVar7;
    int64_t lVar8;
    WCHAR *local_res20;

    pWVar2 = GetEnvironmentStringsW();
    if (pWVar2 != NULL) {
        plVar7 = script;
        if (table != NULL) {
            plVar7 = table;
        }
        WVar1 = *pWVar2;
        pWVar6 = pWVar2;
        while ((pWVar5 = pWVar6, WVar1 != L'\0')) {
            do {
                pWVar4 = pWVar5;
                pWVar5 = pWVar4 + 1;
                if (*pWVar5 == L'\0') {
                    break;
                }
            } while (*pWVar5 != L'=');
            if (mode == 0) {
                lVar8 = (int64_t)(pWVar5 - pWVar6) >> 1;
                local_res20 = NULL;
                PECMD_StrBldCopyWideN((WCHAR **)&local_res20, pWVar6, lVar8);
                lVar3 = PECMD_VarLookup(plVar7, local_res20, NULL, -1, NULL);
                if (lVar3 == 0) {
                    FUN_14001E5B0(script, pWVar6, pWVar4 + 2, (int)lVar8, -1);
                }
                PECMD_FreeStrBuf((WCHAR **)&local_res20);
            } else {
                FUN_14001E5B0(script, pWVar6, pWVar4 + 2,
                              (int)((pWVar5 - pWVar6) >> 1), -1);
            }
            for (; *pWVar5 != L'\0'; pWVar5 = pWVar5 + 1) {
            }
            pWVar6 = pWVar5 + 1;
            WVar1 = *pWVar6;
        }
        FreeEnvironmentStringsW(pWVar2);
    }
}

/* ========== FUN_14007E01C @0x14007e01c ==========
 * 按状态显示窗口，支持 #编号: 形式并更新标题。
 */
void FUN_14007E01C(int64_t obj, uint32_t state, LPWSTR title)
{
    int64_t lVar1;
    HWND hWnd;
    bool bVar2;
    int iVar3;
    LPWSTR pWVar4;
    int nCmdShow;
    LPWSTR local_res8;
    int local_res10[2];
    WCHAR local_58[24];

    if (state != 0xf8000009) {
        *(uint32_t *)(obj + 0x70) = state;
    }
    lVar1 = *(int64_t *)(obj + 0x38);
    nCmdShow = 0;
    hWnd = *(HWND *)(lVar1 + OBJ_HWND);
    if (title != NULL) {
        if (*title == L'#') {
            pWVar4 = StrChrW(title, L':');
            if (pWVar4 != NULL) {
                local_res8 = title + 1;
                *pWVar4 = L'\0';
                local_res10[0] = -0x80000000;
                title = pWVar4 + 1;
                bVar2 = PECMD_ParseUIntValue((int64_t *)&local_res8, local_res10);
                if (bVar2) {
                    iVar3 = local_res10[0];
                    if (local_res10[0] < 0) {
                        iVar3 = -0x80000000;
                    }
                    *(int *)(lVar1 + 0xe0) = iVar3;
                }
            }
        }
        PECMD_StrBldCopyWide((WCHAR **)(lVar1 + 0xd8), title);
    }
    if (-1 < *(int *)(obj + 0x70)) {
        nCmdShow = 5;
    }
    ShowWindow(hWnd, nCmdShow);
    if (state < 0x65) {
        SendMessageW(hWnd, 0x402, (WPARAM)(int)*(int *)(obj + 0x70), 0);
    }
    if (title != NULL) {
        InvalidateRect(hWnd, NULL, 1);
    }
    wsprintfW(local_58, WSTR("%ld"), *(int *)(obj + 0x70));
    if (**(LPCWSTR *)(obj + 0x10) != L'\0') {
        FUN_14007D0AC(*(int64_t **)(obj + 0x50), *(LPCWSTR *)(obj + 0x10), local_58);
    }
}

/* ========== FUN_140080B0C @0x140080b0c ==========
 * 解析控件消息命令并转发给目标窗口。
 */
uint64_t FUN_140080B0C(int64_t *script, HWND hwnd, HWND target,
                                     LPCWSTR spec, WPARAM wParam, uint64_t lParam)
{
    LPCWSTR pWVar1;
    int iVar2;
    uint32_t uVar3;
    int iVar4;
    WCHAR *local_res20;
    int64_t local_48;
    struct {
        HWND hwnd;
        int64_t id;
        uint64_t flags;
        uint64_t p6;
        uint64_t zero;
    } msg;

    iVar4 = 0;
    local_48 = 0xf8000009;
    iVar2 = 0;
    if (*spec == L'?') {
        spec = spec + 1;
        iVar2 = 0x3f;
    }
    local_res20 = (WCHAR *)spec;
    if (*spec == L'*') {
        local_res20 = (WCHAR *)spec + 1;
        iVar4 = 0x2a;
    }
    pWVar1 = local_res20;
    if (iVar2 == 0) {
        PECMD_ParseUIntValue((int64_t *)&local_res20, (int *)&local_48);
        iVar2 = (int)local_48;
        msg.id = (int64_t)(int)local_48;
        if ((int)local_48 != -0x7fffff7) {
            msg.flags = (uint64_t)(iVar4 != 0);
            msg.zero = 0;
            msg.p6 = lParam;
            msg.hwnd = target;
            SendMessageW(hwnd, 0x463, wParam, (LPARAM)&msg);
            if (iVar2 == 0) {
                return 1;
            }
            return 3;
        }
    } else {
        PECMD_AllocStrSlot(&local_res20);
        local_48 = (int64_t)(intptr_t)pWVar1;
        PECMD_CopyTokenTrimmed((int64_t *)&local_48, (int64_t *)&local_res20, 0x3b, 0x3a);
        uVar3 = IsWindowVisible(target);
        PECMD_VarSetUInt(script, (uint64_t)uVar3, local_res20);
        PECMD_FreeStrBuf((WCHAR **)&local_res20);
    }
    return 0;
}

/* ========== PECMD_ExpandVarsLocked @0x14008569c ==========
 * 在脚本临界区中按标志展开/替换变量字符串。
 */
uint64_t PECMD_ExpandVarsLocked(int64_t *script, int64_t *out, uint8_t flags)
{
    uint8_t bVar1;
    uint8_t bVar5;
    WCHAR *pWVar3;
    int64_t lVar4;
    WCHAR *local_res8;
    uint64_t uVar2;

    EnterCriticalSection((LPCRITICAL_SECTION)&g_csInit);
    bVar1 = *(uint8_t *)((uint8_t *)script + 0xd);
    bVar5 = flags & 0x40;
    if ((flags & 0xf) != 0) {
        if ((flags & 4) == 0) {
            while (*(uint16_t *)*out != 0) {
                if ((flags & 8) == 0) {
LAB_14008576e:
                    if ((flags & 1) != 0) {
                        local_res8 = (WCHAR *)*out;
                        *out = 0;
                        PECMD_ResolveVariable(script, local_res8, out,
                                      (int)(char)(-((bVar1 & 1) != 0) & bVar5));
                        goto LAB_14008578e;
                    }
                } else {
                    local_res8 = NULL;
                    lVar4 = PECMD_ResolveIndexedVariable((int64_t)script, (uint16_t *)*out,
                                          (int64_t *)&local_res8);
                    if (((flags & 1) != 0) && (lVar4 == 0)) {
                        PECMD_FreeStrBuf((WCHAR **)&local_res8);
                        goto LAB_14008576e;
                    }
                    pWVar3 = (WCHAR *)*out;
                    *out = (int64_t)local_res8;
                    local_res8 = pWVar3;
LAB_14008578e:
                    PECMD_FreeStrBuf((WCHAR **)&local_res8);
                }
                if ((flags & 2) == 0) {
                    break;
                }
                flags = flags & 0xfd;
            }
        } else {
            local_res8 = (WCHAR *)*out;
            *out = 0;
            if ((*(char *)((uint8_t *)script + 0xda) == '\0') &&
                ((*(uint8_t *)((uint8_t *)script + 0xd) & 0xf) == 0)) {
                FUN_14007BDA8(script, local_res8, (WCHAR **)out, -1, 0);
            } else {
                FUN_14007A224(script, local_res8, (WCHAR **)out, -1, 0);
            }
            PECMD_FreeStrBuf((WCHAR **)&local_res8);
        }
    }
    uVar2 = *out;
    LeaveCriticalSection((LPCRITICAL_SECTION)&g_csInit);
    return uVar2;
}

/* ========== PECMD_SetFileNameExtension @0x140096748 ==========
 * 设置文件扩展名相关变量，返回 0 成功或 ATL 错误码。
 */
int64_t PECMD_SetFileNameExtension(int64_t *script, uint16_t *spec)
{
    LPCWSTR pWVar1;
    uint16_t *psVar2;
    LPWSTR pWVar3;
    int64_t iVar4;
    uint16_t *local_res10;
    WCHAR *local_res20;
    WCHAR *local_18;
    WCHAR *local_10;

    local_res10 = spec;
    local_res20 = NULL;
    local_18 = NULL;
    local_10 = NULL;
    FUN_14005B154((WCHAR **)&local_res10);
    PECMD_AllocWStringBuffer((WCHAR **)&local_res20, 5);
    PECMD_AllocWStringBuffer((WCHAR **)&local_18, 0x14);
    PECMD_AllocStrSlot(&local_10);
    PECMD_SplitTokenTrimWs((int64_t *)&local_res10, (int64_t *)&local_res20, 0x3d);
    FUN_140003A20(script, &local_res20, 0);
    if (*local_res10 == 0x3d) {
        local_res10 = local_res10 + 1;
        PECMD_SplitTokenTrimWs((int64_t *)&local_res10, (int64_t *)&local_18, 0);
    }
    pWVar1 = g_szEmpty;
    iVar4 = -0x7ff8ffa9;
    if ((*local_res20 != L'\0') && (*local_18 != 0)) {
        pWVar1 = FUN_14001BE14(local_18);
        psVar2 = PECMD_LastPathSeparator((uint16_t *)pWVar1);
        if (psVar2 != NULL) {
            pWVar1 = psVar2 + 1;
        }
        pWVar3 = StrRChrW(pWVar1, NULL, L'.');
        if (pWVar3 == NULL) {
            pWVar1 = g_szEmpty;
        } else {
            pWVar1 = pWVar3 + 1;
        }
        iVar4 = 0;
    }
    FUN_1400629B8(script, local_res20, pWVar1);
    PECMD_FreeStrBuf((WCHAR **)&local_10);
    PECMD_FreeStrBuf((WCHAR **)&local_18);
    PECMD_FreeStrBuf((WCHAR **)&local_res20);
    return iVar4;
}

/* ========== FUN_1400A41FC @0x1400a41fc ==========
 * 在全局命名对象表中查找或创建锁对象，并递增引用计数。
 */
uint64_t *FUN_1400A41FC(LPCWSTR name)
{
    int *piVar1;
    int64_t lVar2;
    int64_t lVar3;
    int64_t iVar4;
    uint64_t *puVar5;
    uint64_t uVar6;
    int64_t lVar7;
    int64_t lVar9;
    uint64_t *local_res10;

    EnterCriticalSection((LPCRITICAL_SECTION)&g_csInit);
    lVar3 = (int64_t)g_pi64e118;
    lVar9 = (int64_t)g_intE128;
    iVar4 = 0;
    lVar7 = 0;
    if (0 < lVar9) {
        do {
            lVar2 = *(int64_t *)(lVar3 + lVar7 * 8);
            if (lVar2 != 0) {
                if (lstrcmpW(name, *(LPCWSTR *)(lVar2 + 8)) == 0) {
                    piVar1 = (int *)(*(int64_t *)(lVar3 + iVar4 * 8) + 0x10);
                    *piVar1 = *piVar1 + 1;
                    puVar5 = *(uint64_t **)(lVar3 + iVar4 * 8);
                    goto LAB_1400a42f4;
                }
            }
            lVar7 = lVar7 + 1;
            iVar4 = iVar4 + 1;
        } while (lVar7 < lVar9);
    }
    puVar5 = (uint64_t *)calloc(1, 0x18);
    if (puVar5 == NULL) {
        puVar5 = NULL;
    } else {
        *puVar5 = 0;
        FUN_1400702B0((WCHAR **)(puVar5 + 1), name);
        *(uint32_t *)(puVar5 + 2) = 1;
        uVar6 = FUN_140061FFC(0, 0, NULL);
        *puVar5 = uVar6;
    }
    iVar4 = 0;
    lVar7 = 0;
    if (0 < lVar9) {
        do {
            if (*(int64_t *)(lVar3 + lVar7 * 8) == 0) {
                *(uint64_t **)(lVar3 + iVar4 * 8) = puVar5;
                goto LAB_1400a42f4;
            }
            lVar7 = lVar7 + 1;
            iVar4 = iVar4 + 1;
        } while (lVar7 < lVar9);
    }
    local_res10 = puVar5;
    FUN_1400639F0(&g_pi64e118, (int64_t *)&g_i64E120,
                             (int64_t *)&g_intE128, &local_res10, 8, 1);
LAB_1400a42f4:
    LeaveCriticalSection((LPCRITICAL_SECTION)&g_csInit);
    return puVar5;
}

/* ========== FUN_1400AA53C @0x1400aa53c ==========
 * 添加控件 A（0x78 构造体）。
 */
void FUN_1400AA53C(WPARAM mgr, int64_t v2, int64_t *p3, int x,
                       int y, int w, int h, int64_t *p8,
                       int64_t *p9, uint32_t flags, WCHAR *text)
{
    int64_t lVar1;
    int64_t *puVar2;
    int64_t *puVar3;
    int64_t *puVar4;
    int local_res20[2];

    local_res20[0] = x;
    puVar2 = (int64_t *)PECMD_SendCtrlMessage_0834(mgr, *p3);
    puVar4 = NULL;
    if (puVar2 != NULL) {
        lVar1 = **(int64_t **)(mgr + 0x1a0);
        FUN_14005DAF8((int64_t)mgr, local_res20, &y, &w, &h);
        puVar3 = (int64_t *)calloc(1, 0x78);
        if (puVar3 != NULL) {
            puVar4 = PECMD_CreateEditControl(puVar3, v2,
                                   (int)((((int64_t)puVar2 - lVar1) >> 3) + 0x1000),
                                   p3, local_res20[0], y, w, h,
                                   p8, p9, flags, text);
        }
        *puVar2 = (int64_t)puVar4;
        FUN_14005D9A8(mgr, 0);
    }
}

/* ========== PECMD_AppendCheckboxChild @0x1400b1dec ==========
 * 添加控件 B（0x88 构造体，含全局数组槽位）。
 */
void PECMD_AppendCheckboxChild(int64_t mgr, int64_t v2, int64_t *p3,
                       uint32_t flags1, uint32_t flags2, int mode)
{
    uint32_t uVar1;
    int iVar2;
    int64_t lVar3;
    int64_t *plVar4;
    int64_t *puVar5;
    int64_t *puVar6;
    int local_res8[2];
    int64_t **ppArray;
    int32_t *pCount;

    puVar6 = NULL;
    local_res8[0] = 0;
    EnterCriticalSection((LPCRITICAL_SECTION)&g_csInit);
    ppArray = *(int64_t ***)(mgr + 0x1b8);
    pCount = *(int32_t **)(mgr + 0x1c0);
    plVar4 = (int64_t *)(uintptr_t)PECMD_HeapRealloc((void *)(uintptr_t)*ppArray,
                                          (size_t)((*pCount + 1) << 3));
    *ppArray = plVar4;
    uVar1 = (uint32_t)*pCount;
    *(int64_t **)((uint8_t *)plVar4 + (uint64_t)uVar1 * 8) =
        (int64_t *)&g_u8D5D0;
    iVar2 = *pCount;
    lVar3 = (int64_t)plVar4;
    *pCount = iVar2 + 1;
    LeaveCriticalSection((LPCRITICAL_SECTION)&g_csInit);
    puVar5 = (int64_t *)calloc(1, 0x88);
    if (puVar5 != NULL) {
        FUN_1400702B0((WCHAR **)local_res8, g_szEmpty);
        puVar6 = PECMD_ConstructCheckbox(puVar5, v2, (uint64_t)(uVar1 + 0x1000),
                               (int64_t *)local_res8, p3, flags1, flags2,
                               mode);
    }
    *(int64_t **)(lVar3 + (int64_t)iVar2 * 8) = puVar6;
    if (puVar5 != NULL) {
        PECMD_FreeStrBuf((WCHAR **)local_res8);
    }
}

/* ========== PECMD_CreateListItem @0x1400b2b64 ==========
 * 添加控件 C（0x98 构造体，带双文本参数）。
 */
void PECMD_CreateListItem(WPARAM mgr, int64_t v2, int64_t *p3, int x,
                       int y, int w, int h, int64_t *p8,
                       LPCWSTR text1, LPCWSTR text2, uint32_t flags,
                       char mode, int extra)
{
    int64_t lVar1;
    int64_t *puVar2;
    int64_t *puVar3;
    int64_t *puVar4;
    int local_res20[2];

    local_res20[0] = x;
    puVar2 = (int64_t *)PECMD_SendCtrlMessage_0834(mgr, *p3);
    puVar4 = NULL;
    if (puVar2 != NULL) {
        lVar1 = **(int64_t **)(mgr + 0x1a0);
        FUN_14005DAF8((int64_t)mgr, local_res20, &y, &w, &h);
        puVar3 = (int64_t *)calloc(1, 0x98);
        if (puVar3 != NULL) {
            puVar4 = PECMD_ConstructControlObject(puVar3, v2,
                                   (int)((((int64_t)puVar2 - lVar1) >> 3) + 0x1000),
                                   p3, local_res20[0], y, w, h,
                                   p8, text1, text2, flags, mode,
                                   extra);
        }
        *puVar2 = (int64_t)puVar4;
        FUN_14005D9A8(mgr, 0);
    }
}

/* ========== PECMD_CreateLabelItem @0x1400b9204 ==========
 * 添加控件 D（0x98 构造体，三指针文本参数）。
 */
void PECMD_CreateLabelItem(WPARAM mgr, int64_t v2, int64_t *p3, int x,
                       int y, int w, int h, int64_t *p8,
                       int64_t *p9, int64_t *p10, LPCWSTR text,
                       uint32_t flags)
{
    int64_t lVar1;
    int64_t *puVar2;
    int64_t *puVar3;
    int64_t *puVar4;
    int local_res20[2];

    local_res20[0] = x;
    puVar2 = (int64_t *)PECMD_SendCtrlMessage_0834(mgr, *p3);
    puVar4 = NULL;
    if (puVar2 != NULL) {
        lVar1 = **(int64_t **)(mgr + 0x1a0);
        FUN_14005DAF8((int64_t)mgr, local_res20, &y, &w, &h);
        puVar3 = (int64_t *)calloc(1, 0x98);
        if (puVar3 != NULL) {
            puVar4 = PECMD_CreateControlObject(puVar3, v2,
                                   (int)((((int64_t)puVar2 - lVar1) >> 3) + 0x1000),
                                   p3, local_res20[0], y, w, h,
                                   p8, p9, p10, text, flags,
                                   g_helpVerFlag);
        }
        *puVar2 = (int64_t)puVar4;
    }
}

/* ========== PECMD_CreateGroupItem @0x1400b9340 ==========
 * 添加控件 E（0x78 构造体，带整数指针参数）。
 */
void PECMD_CreateGroupItem(WPARAM mgr, int64_t v2, int64_t *p3, int x,
                       int y, int w, int h, int64_t *p8,
                       uint32_t flags, int *p10, LPCWSTR text)
{
    int64_t lVar1;
    int64_t *puVar2;
    int64_t *puVar3;
    int64_t *puVar4;
    int local_res20[2];

    local_res20[0] = x;
    puVar2 = (int64_t *)PECMD_SendCtrlMessage_0834(mgr, *p3);
    puVar4 = NULL;
    if (puVar2 != NULL) {
        lVar1 = **(int64_t **)(mgr + 0x1a0);
        FUN_14005DAF8((int64_t)mgr, local_res20, &y, &w, &h);
        puVar3 = (int64_t *)calloc(1, 0x78);
        if (puVar3 != NULL) {
            puVar4 = PECMD_AddControlObject(puVar3, v2,
                                   (int)((((int64_t)puVar2 - lVar1) >> 3) + 0x1000),
                                   p3, local_res20[0], y, w, h,
                                   p8, flags, p10, text);
        }
        *puVar2 = (int64_t)puVar4;
    }
}

/* ========== PECMD_CreateImageItem @0x1400bd620 ==========
 * 添加控件 F（0xb0 构造体，带额外 DPI 参数）。
 */
void PECMD_CreateImageItem(WPARAM mgr, uint64_t v2, int64_t *p3, int x,
                       int y, int w, int h, int64_t *p8,
                       int64_t *p9, uint32_t *p10, uint32_t flags1,
                       uint32_t flags2)
{
    int64_t lVar1;
    int64_t *puVar2;
    int64_t *puVar3;
    int64_t *puVar4;
    int local_res20[2];

    local_res20[0] = x;
    puVar2 = (int64_t *)PECMD_SendCtrlMessage_0834(mgr, *p3);
    puVar4 = NULL;
    if (puVar2 != NULL) {
        lVar1 = **(int64_t **)(mgr + 0x1a0);
        FUN_14005DAF8((int64_t)mgr, local_res20, &y, &w, &h);
        puVar3 = (int64_t *)calloc(1, 0xb0);
        if (puVar3 != NULL) {
            puVar4 = PECMD_CreateControlBody(puVar3, v2,
                                   (int)((((int64_t)puVar2 - lVar1) >> 3) + 0x1000),
                                   p3, local_res20[0], y, w, h,
                                   p8, p9, p10, flags1, flags2,
                                   *(uint32_t *)((uint8_t *)mgr + 0x17c));
        }
        *puVar2 = (int64_t)puVar4;
        FUN_14005D9A8(mgr, 0);
    }
}
