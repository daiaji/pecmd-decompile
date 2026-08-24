/* ====================================================================
 * pecmd_defs.h — PECMD 重构公共定义
 *
 * PECMD原始.EXE 全局布局 (x64, ImageBase=0x140000000):
 *   .data  0x14013A000-0x140140000  全局变量 (DAT_14013xxxx)
 *   .flag  0x140147000 (DAT_140147000)
 *
 * 已知全局变量 (Ghidra 命名 -> 语义):
 *   DAT_14013d328 -> g_hHeap        进程堆 (mainW 中 GetProcessHeap)
 *   DAT_14013ca68 -> g_hInstance    模块句柄
 *   DAT_14013cf70 -> g_hInst        mainW 保存的实例句柄
 *   DAT_14013cab8 -> g_szMsgBuf     消息/EXEC 缓冲区 (26000 WCHAR)
 *   DAT_14013d130 -> g_Script       脚本结构 (mainW 初始化)
 *   DAT_14013d1a0 -> g_Env          环境表
 *   DAT_14013cac8 -> g_pNextCmd     下一命令行
 *   DAT_14013a24f -> g_flag         运行标志
 *   DAT_14013c96a / 14013c969 -> 全局标志
 *   DAT_14013ccc8 -> 收尾标志
 *   DAT_14013d138 -> 全局状态
 *   DAT_140147000 -> 运行状态
 *   DAT_14011d308 -> 默认环境表 (.rdata)
 * ==================================================================== */
#ifndef PECMD_DEFS_H
#define PECMD_DEFS_H

#include <stdint.h>
#include <stddef.h>

#include "win32_stub.h"
#include "pecmd_offsets.h"

/* ---- 全局变量 (在 core_globals.c 中定义) ---- */
extern HANDLE g_hHeap;        /* DAT_14013d328 */
extern HINSTANCE g_hInstance; /* DAT_14013ca68 */
extern HINSTANCE g_hInst;     /* DAT_14013cf70 */
extern WCHAR *g_szMsgBuf;     /* DAT_14013cab8 */
extern WCHAR *g_pNextCmd;     /* DAT_14013cac8 */
extern uint32_t g_runFlag;    /* DAT_140147000 */
extern uint32_t *g_pExitCode; /* DAT_14013d180 退出码指针 */
extern uint32_t g_exitCode;   /* DAT_14013d180 指向的退出码值 */
extern int (*g_pConvFunc)(uint32_t, DWORD, LPCSTR, int, LPWSTR, int);    /* DAT_14013c970 */
extern uint32_t g_SysCodePage;                                           /* DAT_140147004 */
extern uint32_t g_ShowWindow;                                            /* DAT_14013a234 */
extern int64_t g_QPC;                                                    /* DAT_14013cb88 */
extern uint64_t g_StartTime;                                             /* DAT_14013d038 */
extern uint32_t g_Tid;                                                   /* DAT_14013ccb4 */
extern uint32_t g_LastTick;                                              /* DAT_14013c9fc */
extern uint32_t g_WM_TaskbarRestart;                                     /* DAT_14013d3e8 */
extern const WCHAR *PTR_u_2024_05_12_14013a000;                          /* .rdata 版本日期串 */
extern int (*g_pNtQueryInfo)(HANDLE, int, void *, uint32_t, void *);     /* DAT_14013ca60 */
extern uint8_t g_flag16a;                                                /* DAT_14013c96a */
extern uint8_t g_flag16b;                                                /* DAT_14013c96b */
extern uint32_t g_WM_TaskbarButtonCreated;                               /* DAT_14013c9f8 */
extern HMODULE g_hKernel32;                                              /* DAT_14013cfd0 */
extern HMODULE g_hUser32;                                                /* DAT_14013cfc8 */
extern HMODULE g_hGdi32;                                                 /* DAT_14013cfd8 */
extern uint32_t g_bX64;                                                  /* DAT_14013a038 */
extern int64_t (*g_pRegDeleteKeyExW)(HKEY, LPCWSTR, uint32_t, uint32_t); /* DAT_14013d408 */
extern int (*g_pSHDeleteKeyW)(HKEY, LPCWSTR);                            /* DAT_14013d400 */
extern int (*g_pSHGetValueW)(HKEY, LPCWSTR, LPCWSTR, DWORD *, void *, DWORD *); /* DAT_14013cd48 */
extern void (*g_pSHGetValueA)(void);                                            /* DAT_14013cd50 */
extern void (*g_pStrToIntExW)(void);                                            /* DAT_14013cd58 */
extern int (*g_pSHSetValueW)(HKEY, LPCWSTR, LPCWSTR, DWORD, const void *,
                             DWORD);                     /* DAT_14013d3f0 */
extern int (*g_pSHDeleteValueW)(HKEY, LPCWSTR, LPCWSTR); /* DAT_14013d3f8 */
extern void (*g_pZwOpenSection)(void);                   /* DAT_14013cb10 */
extern HMODULE g_hNtdll;                                 /* DAT_14013ccf8 */
extern void (*g_pRtlInitUnicodeString)(void);            /* DAT_14013cb48 */
extern void (*g_pNtOpenFile)(void);                      /* DAT_14013cd18 */
extern void (*g_pNtCreateFile)(void);                    /* DAT_14013cd20 */
extern void (*g_pNtReadFile)(void);                      /* DAT_14013cd28 */
extern void (*g_pNtClose)(void);                         /* DAT_14013cd40 */
extern void (*g_pNtCreateDirectoryObject)(void);         /* DAT_14013cd30 */
extern void (*g_pNtCreateSymbolicLinkObject)(void);      /* DAT_14013cd38 */
extern CRITICAL_SECTION g_csInit;                        /* DAT_14013e190 */
extern int64_t g_QPFreq;                                 /* DAT_14013cb80 */

/* ---- 公共工具 (core_string.c) ---- */
void *PECMD_HeapRealloc(void *ptr, size_t size);          /* @0x140063118 */
WCHAR *PECMD_StrAlloc(WCHAR **ps, size_t count);          /* @0x140063720 */
WCHAR *PECMD_StrCat(WCHAR **ps, LPCWSTR src);             /* @0x14006375c */
WCHAR *PECMD_StrDup(LPCWSTR src);                         /* @0x1400700c4 */
void PECMD_StrAssign(WCHAR **ps, LPCWSTR src);            /* @0x1400702b0 */
void PECMD_StrFree(WCHAR **ps);                           /* @0x14005b104 */
WCHAR **PECMD_SkipSpace(WCHAR **pp);                      /* @0x14005b154 */
int PECMD_StrICmpN(const char *s, const WCHAR *w, int n); /* @0x14005c788/72c */
void PECMD_OOMPrompt(int mode);                           /* @0x1400630d0 */
void PECMD_ExitProcess(int code);                         /* @0x14005b21c */

/* ---- 公共工具 (core_var2.c / core_sys.c) ---- */
WCHAR *PECMD_VarStrCpyW(WCHAR **ps, LPCWSTR src, int64_t len);           /* @0x140063888 */
void PECMD_StrAlloc2(WCHAR **ps, int64_t count);                         /* @0x140063694 */
void PECMD_SetVar(void *script, LPCWSTR key, LPCWSTR value);             /* @0x1400629b8 */
void PECMD_SetVarD(void *script, int64_t value, LPCWSTR key);            /* @0x1400669c4 */
WCHAR *PECMD_StrCpyW2(WCHAR **ps, LPCWSTR src);                          /* @0x1400703e4 */
int64_t PECMD_WtoI(const WCHAR *p);                                      /* @0x140064a88 */
void PECMD_SpecialDirVar(LPCWSTR var, LPCWSTR env, const WCHAR *suffix); /* @0x14001bf20 */
void PECMD_RegValueToVar(LPCWSTR var, int type, LPCWSTR value, LPWSTR tmp,
                         const WCHAR *suffix); /* @0x14001bea8 */
DWORD PECMD_RegQuery(HKEY root, LPCWSTR subkey, LPCWSTR name, DWORD *type, BYTE *data,
                     DWORD *size); /* @0x14005c4e0 */
DWORD PECMD_RegOpen(HKEY root, LPCWSTR subkey, HKEY *out, REGSAM access,
                    uint32_t opt);                                      /* @0x14005c394 */
int64_t PECMD_EnablePrivilege(LPCWSTR priv, DWORD attr, uint32_t flag); /* @0x14001c2cc */
void PECMD_Empty(void);                                                 /* @0x140053e78 */

/* 全局 (core_globals.c) */
extern WCHAR *g_pLocale;              /* DAT_14013ca70 */
extern void *g_pSharedMap;            /* DAT_14013ca30 */
extern uint8_t g_flag13f;             /* DAT_14013d13f */
extern void *g_pVtblA;                /* DAT_14013d670 */
extern void *g_pVtblB;                /* DAT_14013d5e0 */
extern int32_t g_val668;              /* DAT_14013d668 */
extern int32_t g_val5d8;              /* DAT_14013d5d8 */
extern uint16_t g_privFlags;          /* DAT_14013e20c */
extern int8_t g_flagA24F;             /* DAT_14013a24f 运行标志 */
extern uint8_t g_Script[0x200];       /* DAT_14013d130 脚本结构 */
extern const uint8_t g_DefEnv[0x100]; /* DAT_14011d308 默认环境表 */
extern uint8_t g_richEditMode;        /* DAT_14013d300 富文本类选择 */
extern void (*g_pFnCleanup)(void);    /* DAT_14013d398 清理回调 */
extern void *(*g_pImageListCreate)(int, int, unsigned, int,
                                   int); /* DAT_14013d390 ImageList_Create */
extern int (*g_pImageListReplaceIcon)(void *, int,
                                      void *); /* DAT_14013d3a0 ImageList_ReplaceIcon */
extern int (*g_pImageListAdd)(void *, void *); /* DAT_14013d3a8 ImageList_Add */
extern int64_t g_cacheTableA[3];               /* DAT_14013dc80 cache table A {data, cap, cnt} */
extern int64_t g_cacheTableB[3];               /* DAT_14013dce8 cache table B {data, cap, cnt} */
/* VirtDisk/vhdmount 动态导入槽 (延迟装载函数指针) */
extern int (*g_pCreateVirtualDisk)(void *, uint64_t, uint32_t, int, uint32_t, int, uint32_t *, int,
                                   HANDLE *); /* DAT_14013d730 */
extern void *g_pAttachVirtualDisk;            /* DAT_14013d740 AttachVirtualDisk (只装载) */
extern void *g_pDetachVirtualDisk;            /* DAT_14013d748 DetachVirtualDisk */
extern void *g_pGetVirtDiskPhysPath; /* DAT_14013d750 GetVirtualDiskPhysicalPath (只装载) */
extern void *g_pGetVirtDiskInfo;     /* DAT_14013d758 GetVirtualDiskInformation (只装载) */
extern void *g_pMountVHD;            /* DAT_14013d760 MountVHD */
extern void *g_pUnmountVHD;          /* DAT_14013d768 UnmountVHD */
/* ImDisk.cpl 动态导入槽 (延迟装载; void* 槽, 调用处强转) */
extern void *g_pImDiskGetVersion;       /* DAT_14013d0e8 ImDiskGetVersion */
extern void *g_pImDiskFindFreeLetter;   /* DAT_14013d088 ImDiskFindFreeDriveLetter */
extern void *g_pImDiskForceRemove;      /* DAT_14013d090 ImDiskForceRemoveDevice */
extern void *g_pImDiskGetOffsetByExt;   /* DAT_14013d0a8 ImDiskGetOffsetByFileExt */
extern void *g_pImDiskGetPartInfo;      /* DAT_14013d0b0 ImDiskGetPartitionInformation */
extern void *g_pImDiskOpenDevice;       /* DAT_14013d0b8 ImDiskOpenDeviceByName */
extern void *g_pImDiskRemoveMountPoint; /* DAT_14013d0d0 ImDiskRemoveMountPoint */
extern void *g_pImDiskStartService;     /* DAT_14013d0d8 ImDiskStartService */
extern void *g_pImDiskCreateMountPoint; /* DAT_14013d0e0 ImDiskCreateMountPoint */

/* ---- 阶段6 全局结构 typedef ---- */
typedef struct tagOSVERSIONINFOW {
    DWORD dwOSVersionInfoSize;
    DWORD dwMajorVersion;
    DWORD dwMinorVersion;
    DWORD dwBuildNumber;
    DWORD dwPlatformId;
    WCHAR szCSDVersion[128];
} OSVERSIONINFOW;
extern OSVERSIONINFOW g_osVersionInfo; /* DAT_14013cb90 */
extern int32_t g_aiDiskType[2];        /* DAT_14013d770 */


/* B0/P1: global extern declarations split into pecmd_globals.h (kept
 * inside this guard so the whole header stays self-contained). */
#include "pecmd_globals.h"

#endif /* PECMD_DEFS_H */
