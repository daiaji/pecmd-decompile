/* ====================================================================
 * core_globals.c — PECMD 全局变量定义
 *
 * 来源: PECMD原始.EXE (.data 段 0x14013A000-0x140140000)
 *   Ghidra 命名 -> 语义名, 地址注释防重复匹配
 * ==================================================================== */
#include "pecmd_defs.h"
extern WCHAR **FUN_14005B154(WCHAR **pp); /* @0x14005b154 */

/* 堆/实例 */
HANDLE g_hHeap;             /* DAT_14013d328 进程堆 (mainW 初始化) */
HINSTANCE g_hInstance;      /* DAT_14013ca68 模块句柄 */
HINSTANCE g_hInst;          /* DAT_14013cf70 mainW 保存的实例 */

/* 消息/命令缓冲 */
WCHAR *g_szMsgBuf;          /* DAT_14013cab8 EXEC 消息缓冲 (26000 WCHAR) */
WCHAR *g_pNextCmd;          /* DAT_14013cac8 下一命令行 */

/* 脚本结构 (mainW 初始化后传给脚本执行器) */
uint8_t g_Script[0x200];    /* DAT_14013d130 脚本结构 (大小 TODO(verify)) */

/* 环境表 */
uint8_t g_Env[0x100];       /* DAT_14013d1a0 运行环境表 (大小 TODO(verify)) */
const uint8_t g_DefEnv[0x100]; /* DAT_14011d308 .rdata 默认环境表 (TODO(verify)) */

/* 状态标志 */
uint32_t g_runFlag;         /* DAT_140147000 运行状态 */
uint8_t g_flag169;          /* DAT_14013c969 (低字节) */
uint8_t g_state138;         /* DAT_14013d138 全局状态 */
uint32_t g_afterMain;       /* DAT_14013ccc8 收尾钩子标志 */
uint8_t g_flagA24F;         /* DAT_14013a24f 运行标志 */
uint32_t g_state190;        /* DAT_14013d190 */
uint32_t g_state198;        /* DAT_14013d198 */

/* 退出码/转换回调 */
uint32_t g_exitCode;        /* DAT_14013d180 指向的退出码值 */
uint32_t *g_pExitCode = &g_exitCode;   /* DAT_14013d180 (指针) */
int (*g_pConvFunc)(uint32_t, DWORD, LPCSTR, int, LPWSTR, int);  /* DAT_14013c970 转换回调 */
uint32_t g_SysCodePage;     /* DAT_140147004 系统代码页 */
uint32_t g_ShowWindow;      /* DAT_14013a234 wShowWindow */
int64_t g_QPC;              /* DAT_14013cb88 QPC 计数 */
uint64_t g_StartTime;       /* DAT_14013d038 启动 FILETIME */
uint32_t g_Tid;             /* DAT_14013ccb4 主线程 ID */
uint32_t g_LastTick;        /* DAT_14013c9fc 上次 GetTickCount */
uint32_t g_WM_TaskbarRestart; /* DAT_14013d3e8 WM_TaskbarRestart 消息值 */

/* 动态导入/模块句柄 */
int (*g_pNtQueryInfo)(HANDLE, int, void *, uint32_t, void *);  /* DAT_14013ca60 */
uint8_t g_flag16a;          /* DAT_14013c96a */
uint8_t g_flag16b;          /* DAT_14013c96b (DEBUGMDG) */
uint32_t g_WM_TaskbarButtonCreated;  /* DAT_14013c9f8 */
HMODULE g_hKernel32;        /* DAT_14013cfd0 */
HMODULE g_hUser32;          /* DAT_14013cfc8 */
HMODULE g_hGdi32;           /* DAT_14013cfd8 */
uint32_t g_bX64;            /* DAT_14013a038 */
void (*g_pRegDeleteKeyExW)(void);  /* DAT_14013d408 */
int (*g_pSHDeleteKeyW)(HKEY, LPCWSTR);  /* DAT_14013d400 */
int (*g_pSHGetValueW)(HKEY, LPCWSTR, LPCWSTR, DWORD *, void *, DWORD *); /* DAT_14013cd48 */
void (*g_pSHGetValueA)(void);      /* DAT_14013cd50 */
void (*g_pStrToIntExW)(void);      /* DAT_14013cd58 */
int (*g_pSHSetValueW)(HKEY, LPCWSTR, LPCWSTR, DWORD, const void *, DWORD); /* DAT_14013d3f0 */
int (*g_pSHDeleteValueW)(HKEY, LPCWSTR, LPCWSTR);  /* DAT_14013d3f8 */
void (*g_pZwOpenSection)(void);    /* DAT_14013cb10 */
HMODULE g_hNtdll;           /* DAT_14013ccf8 */
void (*g_pRtlInitUnicodeString)(void); /* DAT_14013cb48 */
void (*g_pNtOpenFile)(void);       /* DAT_14013cd18 */
void (*g_pNtCreateFile)(void);     /* DAT_14013cd20 */
void (*g_pNtReadFile)(void);       /* DAT_14013cd28 */
void (*g_pNtClose)(void);          /* DAT_14013cd40 */
void (*g_pNtCreateDirectoryObject)(void);  /* DAT_14013cd30 */
void (*g_pNtCreateSymbolicLinkObject)(void); /* DAT_14013cd38 */
void *g_csInit;             /* DAT_14013e190 关键段 */
int64_t g_QPFreq;           /* DAT_14013cb80 QPC 频率 */

/* 系统状态 */
WCHAR *g_pLocale;           /* DAT_14013ca70 代码页字符串 */
void *g_pSharedMap;         /* DAT_14013ca30 共享映射 */
uint8_t g_flag13f;          /* DAT_14013d13f */
void *g_pVtblA;             /* DAT_14013d670 = g_szEmpty */
void *g_pVtblB;             /* DAT_14013d5e0 = g_szEmpty */
int32_t g_val668;           /* DAT_14013d668 = 0xffffff9c */
int32_t g_val5d8;           /* DAT_14013d5d8 = 0xffffff9c */
uint16_t g_privFlags;       /* DAT_14013e20c 权限标志 */

/* .rdata 常量 */
const WCHAR *PTR_u_2024_05_12_14013a000 = WSTR("2024.05.12"); /* PTR_u_2024_05_12_14013a000 */

/* 随机数状态 (B8a) */
int64_t g_randState;    /* DAT_14013a358 随机状态 */
int64_t g_lcgState;     /* DAT_14013a360 LCG 状态 (FUN_14005DFF4) */
int64_t g_randPrev;     /* DAT_14013e1c0 上次随机值 */

/* 执行环境 (B8a) */
HHOOK g_hHook;              /* DAT_14013d068 键盘钩子 */
void *g_pHookData;          /* DAT_14013d078 钩子数据 */
HANDLE g_hWaitEvent;        /* DAT_14013cd10 等待事件 */
void *g_pComState;          /* DAT_14013d3c0 COM 状态块 */
int (*g_pOleInit)(int);     /* DAT_14013d800 OleInitialize */
void (*g_pOleUninit)(void); /* DAT_14013d808 OleUninitialize */
int32_t g_taskCount;        /* DAT_14013d7e8 活动任务计数 */

/* ---- WS2_32 延迟加载函数指针槽 (FUN_14005cfd0 经 FUN_14005c828 填充) ---- */
int   (*g_pWSAStartup)(int, void *);          /* DAT_14013d4e8 WSAStartup */
uintptr_t (*g_psock)(int, int, int);          /* DAT_14013d4f0 socket */
int   (*g_pshutdown)(uintptr_t, int);         /* DAT_14013d4f8 shutdown */
int   (*g_pclosesocket)(uintptr_t);           /* DAT_14013d500 closesocket */
int   (*g_pconnect)(uintptr_t, void *, int);  /* DAT_14013d508 connect */
int   (*g_psend)(uintptr_t, const void *, int); /* DAT_14013d510 send */
int   (*g_pselect)(int, void *, void *, void *, void *); /* DAT_14013d518 select */
int   (*g_precvfrom)(uintptr_t, void *, int, int, void *, void *); /* DAT_14013d520 recvfrom */
int   (*g_psendto)(uintptr_t, const void *, int, int, void *, int); /* DAT_14013d528 sendto */
int   (*g_pbind)(uintptr_t, const void *, int); /* DAT_14013d530 bind */
int   (*g_plisten)(uintptr_t, int);           /* DAT_14013d538 listen */
uintptr_t (*g_paccept)(uintptr_t, void *, void *); /* DAT_14013d540 accept */
int   (*g_pgetpeername)(uintptr_t, void *, void *); /* DAT_14013d548 getpeername */
int   (*g_pgetsockname)(uintptr_t, void *, void *); /* DAT_14013d550 getsockname */
int   (*g_pioctlsocket)(uintptr_t, int, uint32_t *); /* DAT_14013d558 ioctlsocket */
int   (*g_psetsockopt)(uintptr_t, int, int, const void *, int); /* DAT_14013d560 setsockopt */
uint32_t (*g_pntohl)(uint32_t);               /* DAT_14013d568 ntohl */
uint32_t (*g_phtonl)(uint32_t);               /* DAT_14013d570 htonl */
uint16_t (*g_phtons)(uint16_t);               /* DAT_14013d578 htons */
int    (*g_pinet_addr)(char *);               /* DAT_14013d580 inet_addr */
void  *(*g_pgethostbyname)(const char *);     /* DAT_14013d588 gethostbyname */
LPCSTR (*g_pinet_ntoa)(uint32_t);             /* DAT_14013d590 inet_ntoa */
int    (*g_pWSAGetLastError)(void);           /* DAT_14013d598 WSAGetLastError */
int    (*g_pWSAFDIsSet)(void *, void *);      /* DAT_14013d5a0 __WSAFDIsSet */
void *g_pMapBlk;            /* DAT_14013cf50 映射块 */
CRITICAL_SECTION g_csCom;   /* DAT_14013e168 COM 关键段 */
CRITICAL_SECTION g_csHook;  /* DAT_14013e2d0 钩子关键段 */

/* B8h 批 (core_b8h.c) */
uint32_t (*g_pfnVirtualAllocEx)(void) = NULL; /* DAT_14013e258 VirtualAllocEx */
uint32_t *g_pCrcTable = NULL;   /* DAT_14013e260 CRC32 查表指针 */
uint32_t (*g_pfnRasDial)(void);    /* DAT_14013e268 RasDialW (RAS API) */
HANDLE g_hSyncEvtA;             /* DAT_14013e1f8 同步事件 A */
HANDLE g_hSyncEvtB;             /* DAT_14013e200 同步事件 B */
uint32_t g_syncEvtVal;          /* DAT_14013e208 同步事件记录值 */
uint32_t (*g_pfnRasEnumEntries)(void *, void *, void *, uint32_t *, uint32_t *) = NULL; /* DAT_14013e270 RasEnumEntriesW */
uint32_t (*g_pfnRasGetConnectStatus)(uint64_t, uint32_t *) = NULL; /* DAT_14013e278 RasGetConnectStatusW */
uint32_t (*g_pfnRasGetErrorString)(void);           /* DAT_14013e280 RasGetErrorStringW (B8i 定义) */
uint32_t (*g_pfnRasHangUp)(uint64_t) = NULL;        /* DAT_14013e288 RasHangUpW */
uint32_t (*g_pfnRasEnumConnections)(void *, uint32_t *, uint32_t *) = NULL; /* DAT_14013e290 RasEnumConnectionsW */
void *g_syncState;            /* DAT_14013e298 同步状态 */
HWND g_hActiveDevWnd;           /* DAT_14013e400 当前设备窗口 */
HCURSOR g_hArrowCursor;         /* DAT_14013e4d8 箭头光标 (IDC_ARROW) */
uint8_t g_objMode;              /* DAT_14013a84d 对象布局/指针模式 (TODO(verify)) */
uint32_t (*g_pfnGetThreadCtx)(HANDLE, int64_t); /* DAT_14013e248 GetThreadContext */
uint32_t (*g_pfnSetThreadCtx)(HANDLE, int64_t); /* DAT_14013e250 SetThreadContext */
uint64_t g_remoteTebBase;       /* DAT_14013e2f9 远程 TEB 基址 (TODO(verify)) */
uint64_t g_ptrA84C;             /* DAT_14013a84c 布局指针 (TODO(verify)) */
uint64_t g_peCtxSize;           /* DAT_14013a850 上下文大小 */
uint64_t g_peCtxOffB;           /* DAT_14013e2f8 上下文偏移 B */
uint8_t g_peByteA84E;           /* DAT_14013a84e 布局字节 E */
uint16_t g_peWordA852;          /* DAT_14013a852 布局字 852 */
uint8_t g_peByteA84F;           /* DAT_14013a84f 布局字节 F */
uint16_t g_peWordA853;          /* DAT_14013a853 布局字 853 */
HGDIOBJ g_hStockWhiteBrush;     /* DAT_14013a858 库存白色画刷 */
float g_barScaleDiv;            /* DAT_14012c0d0 条比例除数 (TODO(verify)) */
double g_scrollLimit;           /* DAT_140129ee8 滚动上限常量 (TODO(verify)) */
void (*g_pFnCleanup)(void) = NULL;  /* DAT_14013d398 组合对象清理回调 */
void *(*g_pImageListCreate)(int, int, unsigned, int, int) = NULL;  /* DAT_14013d390 ImageList_Create */
int (*g_pImageListReplaceIcon)(void *, int, void *) = NULL;        /* DAT_14013d3a0 ImageList_ReplaceIcon */
int (*g_pImageListAdd)(void *, void *) = NULL;                     /* DAT_14013d3a8 ImageList_Add */
int64_t g_cacheTableA[3] = {0, 0, 0};   /* DAT_14013dc80 cache table A {data, cap, cnt} */
int64_t g_cacheTableB[3] = {0, 0, 0};   /* DAT_14013dce8 cache table B {data, cap, cnt} */
/* VirtDisk/vhdmount 动态导入槽 (FUN_14005F60C 延迟装载) */
int (*g_pCreateVirtualDisk)(void *, uint64_t, uint32_t, int, uint32_t, int, uint32_t *, int, HANDLE *) = NULL; /* DAT_14013d730 */
void *g_pAttachVirtualDisk = NULL;    /* DAT_14013d740 AttachVirtualDisk (只装载) */
void *g_pDetachVirtualDisk = NULL;    /* DAT_14013d748 DetachVirtualDisk */
void *g_pGetVirtDiskPhysPath = NULL;  /* DAT_14013d750 GetVirtualDiskPhysicalPath (只装载) */
void *g_pGetVirtDiskInfo = NULL;      /* DAT_14013d758 GetVirtualDiskInformation (只装载) */
void *g_pMountVHD = NULL;             /* DAT_14013d760 MountVHD */
void *g_pUnmountVHD = NULL;           /* DAT_14013d768 UnmountVHD */
/* ImDisk.cpl 动态导入槽 (延迟装载; void* 槽, 调用处强转) */
void *g_pImDiskGetVersion = NULL;        /* DAT_14013d0e8 ImDiskGetVersion */
void *g_pImDiskFindFreeLetter = NULL;    /* DAT_14013d088 ImDiskFindFreeDriveLetter */
void *g_pImDiskForceRemove = NULL;       /* DAT_14013d090 ImDiskForceRemoveDevice */
void *g_pImDiskGetOffsetByExt = NULL;    /* DAT_14013d0a8 ImDiskGetOffsetByFileExt */
void *g_pImDiskGetPartInfo = NULL;       /* DAT_14013d0b0 ImDiskGetPartitionInformation */
void *g_pImDiskOpenDevice = NULL;        /* DAT_14013d0b8 ImDiskOpenDeviceByName */
void *g_pImDiskRemoveMountPoint = NULL;  /* DAT_14013d0d0 ImDiskRemoveMountPoint */
void *g_pImDiskStartService = NULL;      /* DAT_14013d0d8 ImDiskStartService */
void *g_pImDiskCreateMountPoint = NULL;  /* DAT_14013d0e0 ImDiskCreateMountPoint */

/* B8i 批 (core_b8i.c) */
uint64_t g_rasState;            /* DAT_14013e2a0 RAS 等待状态 */
uint64_t *g_pRasEntryBuf = NULL;/* DAT_14013e300 RAS 条目缓冲指针 */
uint8_t g_richEditMode;         /* DAT_14013d300 富文本类选择 (2=RichEdit20W) */
uint8_t g_tooltipThreshold;     /* DAT_14013a861 Tooltip 触发阈值 (TODO(verify)) */
uint8_t g_tooltipCount0;        /* DAT_14013a860 Tooltip 计数 0 (TODO(verify)) */

/* DAT_14013a058 @0x14013a058 */
int g_cmdTable3Count;

/* DAT_14013a060 @0x14013a060 */
void * g_cmdTable3;

/* DAT_14013a078 @0x14013a078 */
int g_cmdTable2Count;

/* DAT_14013a080 @0x14013a080 */
void * g_cmdTable2;

/* DAT_14013a098 @0x14013a098 */
int g_cmdTable4Count;

/* DAT_14013a0a0 @0x14013a0a0 */
void * g_cmdTable4;

/* DAT_14013a0c8 @0x14013a0c8 */
int g_cmdTable5Len;

/* DAT_14013a0c0 @0x14013a0c0 */
void * g_cmdTable5;

/* DAT_14013ca98 @0x14013ca98 */
int g_cmdTable1Count;

/* DAT_14013caa0 @0x14013caa0 */
void * g_cmdTable1;

/* DAT_14013a330 @0x14013a330 */
HANDLE g_hStdIn;

/* DAT_14013a338 @0x14013a338 */
HANDLE g_hStdErr;

/* DAT_14013a340 @0x14013a340 */
HANDLE g_hStdOut;

/* DAT_14013ca48 @0x14013ca48 */
char g_minintFlag;

/* DAT_14013caf0 @0x14013caf0 */
uint32_t g_exitCodeCache;

/* DAT_14013cad0 @0x14013cad0 */
uint8_t g_wallpaperMode;

/* DAT_14013a048 @0x14013a048 */
void * g_pMemQuery;

/* DAT_14013ca58 @0x14013ca58 */
void * g_pProcMemRead;

/* DAT_14013ca08 @0x14013ca08 */
void * g_pShutdownFlag;

/* DAT_14013cac0 @0x14013cac0 */
void * g_pPendingObj;

/* DAT_14013ca40 @0x14013ca40 */
void * g_pGetFileVersionInfoExW;

/* DAT_14013ca38 @0x14013ca38 */
void * g_pGetFileVersionInfoSizeExW;

/* DAT_14013caa8 @0x14013caa8 */
int64_t * g_pVolHandles;

/* DAT_14013c978 @0x14013c978 */
void * g_pSysAllocString;

/* DAT_14013c980 @0x14013c980 */
void * g_pSysFreeString;

/* DAT_14013c9b0 @0x14013c9b0 */
HMODULE g_hOleaut32;

/* DAT_14013d0a0 @0x14013d0a0 */
void * g_pImDiskCtrl;

/* DAT_14013d098 @0x14013d098 */
void * g_pImDiskMask;

/* DAT_14013d0c0 @0x14013d0c0 */
void * g_pDevOpen;

/* DAT_14013d0c8 @0x14013d0c8 */
void * g_pDevOpen2;

/* DAT_14013d0f8 @0x14013d0f8 */
void * g_pNtOpenSymLink;

/* DAT_14013d100 @0x14013d100 */
void * g_pNtQuerySymLink;

/* DAT_14013d830 @0x14013d830 */
int (*g_pCoCreateInstance)(void *, void *, uint32_t, void *, void **);

/* DAT_14013ccc0 @0x14013ccc0 */
HMODULE g_hGdiPlus;

/* DAT_14013cfc0 @0x14013cfc0 */
void * g_gdiplusToken;

/* 阶段4b: DAT_->g_ 迁入 core_globals (批2) */

/* DAT_14013a349 @0x14013a349 */
uint8_t g_lowVerFlag;

/* DAT_14013a350 @0x14013a350 */
uint32_t g_sysColor8;

/* DAT_14013a258 @0x14013a258 */
int g_tooltipTimeout;

/* DAT_14013caf9 @0x14013caf9 */
uint8_t g_runMode;

/* DAT_14013cb08 @0x14013cb08 */
uint8_t g_flagCB08;

/* DAT_14013cb58 @0x14013cb58 */
uint32_t g_bitFlags;

/* DAT_14013cb98 @0x14013cb98 */
uint32_t g_sysVerClass;

/* DAT_14013ccb2 @0x14013ccb2 */
uint8_t g_foregroundFlag;

/* DAT_14013ccb3 @0x14013ccb3 */
uint8_t g_flagCCB3;

/* DAT_14013cf82 @0x14013cf82 */
uint8_t g_tipAlign;

/* DAT_14013cf98 @0x14013cf98 */
int g_tipOffX;

/* DAT_14013cf9c @0x14013cf9c */
int g_tipOffY;

/* DAT_14013d030 @0x14013d030 */
uint8_t g_sysStartFlag;

/* DAT_14013d031 @0x14013d031 */
uint8_t g_answerFlag;

/* DAT_14013d080 @0x14013d080 */
int32_t g_msgLockCount;

/* DAT_14013d5a8 @0x14013d5a8 */
int32_t g_qMode;

/* DAT_14013d6f5 @0x14013d6f5 */
uint8_t g_flagD6F5;

/* DAT_14013d5ac @0x14013d5ac */
int32_t g_flagD5AC;

/* DAT_14013d3ef @0x14013d3ef */
uint8_t g_flagD3EF;

/* DAT_14013d321 @0x14013d321 */
uint8_t g_flagD321;

/* DAT_14013d322 @0x14013d322 */
uint8_t g_iconRefCount;

/* DAT_14013d323 @0x14013d323 */
uint8_t g_msgWndMode;

/* DAT_14013d324 @0x14013d324 */
uint8_t g_fgWndLock;

/* DAT_14013d4e0 @0x14013d4e0 */
int g_dblClickFlag;

/* DAT_14013a320 @0x14013a320 */
char g_scrollOff;

/* DAT_14013d2f8 @0x14013d2f8 */
HMODULE g_hRichEdit;

/* DAT_14013d6f0 @0x14013d6f0 */
int32_t g_flagD6F0;

/* DAT_14013d840 @0x14013d840 */
uint32_t g_guidPtrState;

/* DAT_14013d7fc @0x14013d7fc */
uint32_t g_flagD7FC;

/* DAT_14013a298 @0x14013a298 */
uint8_t g_flagA298;

/* DAT_14013a24b @0x14013a24b */
int8_t g_flagA24B;

/* DAT_14013a24e @0x14013a24e */
uint8_t g_flagA24E;

/* DAT_14013d3d8 @0x14013d3d8 */
HWND g_ownerWnd;

/* DAT_14013cf68 @0x14013cf68 */
HWND g_wndCF68;

/* DAT_14013cf88 @0x14013cf88 */
HICON g_hIconCF88;

/* DAT_14013cfa8 @0x14013cfa8 */
HFONT g_hFontFast;

/* 阶段4c: DAT_->g_ 迁入 core_globals (批3) */

/* DAT_14013a210 @0x14013a210 */
int g_readBufSize;

/* DAT_14013a220 @0x14013a220 */
int g_ramdrivFlag;

/* DAT_14013a24c @0x14013a24c */
int16_t g_transState;

/* DAT_14013a348 @0x14013a348 */
uint8_t g_popmenuFlag;

/* DAT_14013c91c @0x14013c91c */
uint32_t g_msgRetCode;

/* DAT_14013ccc9 @0x14013ccc9 */
uint8_t g_flagCCC9;

/* DAT_14013cccb @0x14013cccb */
uint8_t g_topiconFlag;

/* DAT_14013cccc @0x14013cccc */
int16_t g_hotkeyIdx;

/* DAT_14013cccf @0x14013cccf */
uint8_t g_logoActiveFlag;

/* DAT_14013d070 @0x14013d070 */
uint32_t g_hookBusyFlag;

/* DAT_14013d074 @0x14013d074 */
uint32_t g_hookReentry;

/* DAT_14013d268 @0x14013d268 */
HMODULE g_hImdiskCpl;

/* DAT_14013d271 @0x14013d271 */
uint8_t g_sysStartuped;

/* DAT_14013d272 @0x14013d272 */
uint8_t g_initOnceFlag;

/* DAT_14013d290 @0x14013d290 */
uint32_t g_threadTableInitFlag;

/* DAT_14013d3c8 @0x14013d3c8 */
HHOOK g_hHook2;

/* DAT_14013d6f6 @0x14013d6f6 */
uint8_t g_flagD6F6;

/* DAT_14013e1b8 @0x14013e1b8 */
uint8_t g_flagE1B8;

/* DAT_14013e1c8 @0x14013e1c8 */
uint32_t g_randSeedAccum;

/* DAT_14013e1f4 @0x14013e1f4 */
uint8_t g_flagE1F4;

/* DAT_14013e2b0 @0x14013e2b0 */
HFONT g_hFontE2B0;

/* DAT_14013b960 @0x14013b960 */
uint64_t g_u64B960;

/* DAT_14013bf50 @0x14013bf50 */
uint64_t g_u64BF50;

/* DAT_14013bf54 @0x14013bf54 */
uint64_t g_u64BF54;

/* DAT_14013bf58 @0x14013bf58 */
uint64_t g_u64BF58;

/* DAT_14013e598 @0x14013e598 */
uint64_t g_u64E598;

/* DAT_14013e600 @0x14013e600 */
uint64_t g_u64E600;

/* DAT_14013ec88 @0x14013ec88 */
uint64_t g_u64EC88;

/* DAT_14013eee8 @0x14013eee8 */
uint64_t g_u64EEE8;

/* DAT_14013eef0 @0x14013eef0 */
uint64_t g_u64EEF0;

/* DAT_14013f520 @0x14013f520 */
uint64_t g_u64F520;

/* 阶段4d: DAT_->g_ 迁入 core_globals (批4) */

/* DAT_14013a22c @0x14013a22c */
uint32_t g_u32A22C;

/* DAT_14013a238 @0x14013a238 */
int g_intA238;

/* DAT_14013a23c @0x14013a23c */
int g_intA23C;

/* DAT_14013a240 @0x14013a240 */
int g_intA240;

/* DAT_14013a244 @0x14013a244 */
int g_intA244;

/* DAT_14013a24a @0x14013a24a */
uint8_t g_u8A24A;

/* DAT_14013c934 @0x14013c934 */
DWORD g_dwC934;

/* DAT_14013c96c @0x14013c96c */
DWORD g_dwC96C;

/* DAT_14013c9c8 @0x14013c9c8 */
HWND g_hwndC9C8;

/* DAT_14013c9e8 @0x14013c9e8 */
uint64_t g_u64C9E8;

/* DAT_14013c9f0 @0x14013c9f0 */
uint64_t g_u64C9F0;

/* DAT_14013ca20 @0x14013ca20 */
uint64_t g_u64CA20;

/* DAT_14013ca49 @0x14013ca49 */
uint8_t g_u8CA49;

/* DAT_14013ca50 @0x14013ca50 */
uint64_t g_u64CA50;

/* DAT_14013cb60 @0x14013cb60 */
uint64_t g_u64CB60;

/* DAT_14013ccb1 @0x14013ccb1 */
uint8_t g_u8CCB1;

/* DAT_14013ccb8 @0x14013ccb8 */
int64_t g_i64CCB8;

/* DAT_14013cd00 @0x14013cd00 */
uint64_t g_u64CD00;

/* DAT_14013cf78 @0x14013cf78 */
HWND g_hwndCF78;

/* DAT_14013cf83 @0x14013cf83 */
uint8_t g_u8CF83;

/* DAT_14013cf84 @0x14013cf84 */
uint8_t g_u8CF84;

/* DAT_14013cf85 @0x14013cf85 */
uint8_t g_u8CF85;

/* DAT_14013cf87 @0x14013cf87 */
uint8_t g_u8CF87;

/* DAT_14013d170 @0x14013d170 */
int64_t g_i64D170;

/* DAT_14013d294 @0x14013d294 */
char g_charD294;

/* DAT_14013d310 @0x14013d310 */
HWND g_hwndD310;

/* DAT_14013d428 @0x14013d428 */
int64_t g_i64D428;

/* DAT_14013d438 @0x14013d438 */
int64_t g_i64D438;

/* DAT_14013d5d0 @0x14013d5d0 */
uint8_t g_u8D5D0;

/* DAT_14013d6f4 @0x14013d6f4 */
int32_t g_i32D6F4;

/* DAT_14013d6f7 @0x14013d6f7 */
uint8_t g_u8D6F7;

/* DAT_14013d710 @0x14013d710 */
HHOOK g_hookD710;

/* DAT_14013d7d8 @0x14013d7d8 */
uint64_t g_u64D7D8;

/* DAT_14013d7dc @0x14013d7dc */
int32_t g_i32D7DC;

/* DAT_14013d7ec @0x14013d7ec */
int32_t g_i32D7EC;

/* DAT_14013e120 @0x14013e120 */
int64_t g_i64E120;

/* ---- 阶段4e (批5): DAT_→g_ 迁入 ---- */

/* g_threadTableCap @0x14013d280 */
uint64_t g_threadTableCap;

/* g_threadTableCnt @0x14013d288 */
uint64_t g_threadTableCnt;

/* g_loadThreadId @0x14013d3e0 */
uint64_t g_loadThreadId;

/* g_osVersion @0x14013d708 */
uint64_t g_osVersion;

/* g_intE128 @0x14013e128 */
int64_t g_intE128;

/* g_svcCmdStr @0x14013ca18 */
LPCWSTR g_svcCmdStr;

/* g_qOutBuf @0x14013d5b8 */
LPWSTR g_qOutBuf;

/* g_pCurRect @0x14013cfa0 */
RECT *g_pCurRect;

/* g_rectCF90 @0x14013cf90 */
RECT g_rectCF90;

/* g_pAppData @0x14013cfb0 */
void *g_pAppData;

/* g_winHandle @0x14013d718 */
void *g_winHandle;

/* g_winData @0x14013d720 */
void *g_winData;

/* g_pOnWndProc2 @0x14013c9d8 */
void *g_pOnWndProc2;

/* g_pOnWndProc3 @0x14013c9e0 */
void *g_pOnWndProc3;

/* g_pFiber @0x14013ca00 */
void *g_pFiber;

/* g_hOle32 @0x14013cfe0 */
HMODULE g_hOle32;

/* g_pImageBuf @0x14013e0e0 */
void *g_pImageBuf;

/* g_helpWord @0x14013a228 */
uint32_t g_helpWord;

/* g_helpHi32 @0x14013a230 */
uint32_t g_helpHi32;

/* g_charTableF @0x14013a248 */
uint8_t g_charTableF;

/* g_flagA249 @0x14013a249 */
uint8_t g_flagA249;

/* g_helpVerFlag @0x14013a34c */
int32_t g_helpVerFlag;

/* g_flagCF86 @0x14013cf86 */
uint8_t g_flagCF86;

/* g_flagCCCE @0x14013ccce */
uint8_t g_flagCCCE;

/* g_lastThreadId @0x14013d7f8 */
volatile uint32_t g_lastThreadId;

/* ===== 阶段4h 批7: 简单标量 DAT_ → g_ ===== */

/* g_u64FontA98 @0x140120a98 (core_b2f 字体相关) */
uint64_t g_u64FontA98;
/* g_u32FontAa0 @0x140120aa0 (core_b2f) */
uint32_t g_u32FontAa0;
/* g_dbl1178 @0x140121178 (core_b2f 计时) */
double g_dbl1178;
/* g_dbl1668 @0x140121668 (core_b2f) */
double g_dbl1668;
/* g_flae4 @0x140121ae4 (core_b2f 颜色分量) */
float g_flae4;
/* g_flae8 @0x140121ae8 (core_b2f 颜色分量) */
float g_flae8;
/* g_dbl2578 @0x140122578 (core_b2f) */
double g_dbl2578;
/* g_dbl21630 @0x140121630 (core_b2f double) — 阶段4l 迁入 */
double g_dbl21630;
/* g_dbl21bf0 @0x140121bf0 (core_b2f double) — 阶段4l 迁入 */
double g_dbl21bf0;
/* g_dbl21c80 @0x140121c80 (core_b2f double) — 阶段4l 迁入 */
double g_dbl21c80;
/* g_dbl22580 @0x140122580 (core_b2f double) — 阶段4l 迁入 */
double g_dbl22580;
/* g_dbl22588 @0x140122588 (core_b2f double) — 阶段4l 迁入 */
double g_dbl22588;
/* g_dbl22590 @0x140122590 (core_b2f double) — 阶段4l 迁入 */
double g_dbl22590;
/* g_dbl2598 @0x140122598 (core_b2f) */
double g_dbl2598;
/* g_dbl2b368 @0x14012b368 (core_b8m) */
double g_dbl2b368;
/* g_dbl2b370 @0x14012b370 (core_b8m) */
double g_dbl2b370;
/* g_dbl2b378 @0x14012b378 (core_b8m) */
double g_dbl2b378;
/* g_dbl2b380 @0x14012b380 (core_b8m) */
double g_dbl2b380;
/* g_u6497e8 @0x1401297e8 (core_b3l) */
uint64_t g_u6497e8;
/* g_u6497f0 @0x1401297f0 (core_b3l) */
uint64_t g_u6497f0;
/* g_u6497f8 @0x1401297f8 (core_b3l) */
uint64_t g_u6497f8;
/* g_u329800 @0x140129800 (core_b3l) */
uint32_t g_u329800;
/* g_u169804 @0x140129804 (core_b3l) */
uint16_t g_u169804;
/* g_u897806 @0x140129806 (core_b3l) */
uint8_t g_u897806;
/* g_u6453d8 @0x1401253d8 (core_b3g) */
uint64_t g_u6453d8;
/* g_u3253e0 @0x1401253e0 (core_b3g) */
uint32_t g_u3253e0;
/* g_u6459e4 @0x1401259e4 (core_b3_remaining) */
uint64_t g_u6459e4;
/* g_u6459e8 @0x1401259e8 (core_b3_remaining) */
uint64_t g_u6459e8;
/* g_u326778 @0x140126778 (core_b3_remaining) */
uint32_t g_u326778;
/* g_u162677c @0x14012677c (core_b3_remaining) */
uint16_t g_u162677c;
/* g_u64ee90 @0x14011ee90 (core_b3r_g7) */
uint64_t g_u64ee90;
/* g_u646e78 @0x140126e78 (core_b3r_g7) */
uint64_t g_u646e78;
/* g_u6435a0 @0x1401235a0 (core_b3_remaining 字体特征数据) */
uint64_t g_u6435a0;
/* g_dbl94e8 @0x1401294e8 (core_b3_remaining 字体大小常量) */
double g_dbl94e8;
/* g_flaf0 @0x140121af0 (core_b2f) */
float g_flaf0;
/* g_flaf4 @0x140121af4 (core_b2f) */
float g_flaf4;
/* g_u322570 @0x140122570 (core_b3r_h1) */
uint32_t g_u322570;
/* g_u162574 @0x140122574 (core_b3r_h1) */
uint16_t g_u162574;

/* ---- 阶段4i (批8): 简单标量/结构 DAT_→g_ (第1组) ---- */
/* g_dbl20b28 @0x140120b28 (core_b2d/b2f/b3f/b3_remaining/b8m 默认字体大小) 亦为 g_dbl20b28 */
double g_dbl20b28;
/* g_u161be4 @0x140121be4 (core_b2f) */
uint16_t g_u161be4;
/* g_u161bec @0x140121bec (core_b2f) */
uint16_t g_u161bec;
/* g_dbl25230 @0x140125230 (core_b3f/b3_remaining/b7a/b8h 四舍五入 0.5) */
double g_dbl25230;
/* g_pwszD220 @0x14013d220 (core_b2d/b2f 状态提示串) */
WCHAR *g_pwszD220;
/* g_csThreadTbl @0x14013d228 (core_b2a/b3m/b3_remaining 线程表临界区) */
CRITICAL_SECTION g_csThreadTbl;
/* g_pwszD250 @0x14013d250 (core_b2d/b2e shell 命令串) */
WCHAR *g_pwszD250;
/* g_pwszD258 @0x14013d258 (core_b1_remaining/b2d/b2f 前台窗口状态串) */
WCHAR *g_pwszD258;
/* g_csDisk @0x14013e0e8 (core_b3_remaining/b3r_i28b 全局临界区) */
CRITICAL_SECTION g_csDisk;

/* g_hMsgHook @0x14013cf58 (core_execscript/b1_remaining 消息钩子句柄) 原 link_stubs 遗留 g_ */
HHOOK g_hMsgHook;


/* ---- 阶段4i (批8): 标量/小数组 DAT_→g_ (第2组) ---- */
/* g_u64e770 @0x14011e770 (core_b1_remaining SetupDiGetClassDevsW 类 GUID) */
uint64_t g_u64e770;
/* g_pSvStatus @0x14013c910 (core_b1_remaining service 状态句柄) */
void *g_pSvStatus;
/* g_pi64e118 @0x14013e118 (core_b3i/b3m 缓存对象表, 动态数组指针) */
int64_t *g_pi64e118;
/* g_u64c9d0 @0x14013c9d0 (core_b1_remaining uint64[4]) */
uint64_t g_u64c9d0[4];
/* g_u64cd08 @0x14013cd08 (物理内存映射句柄; core_b1_remaining uint64 / core_b2b HANDLE) */
uint64_t g_u64cd08;
/* g_svcStatus @0x14013c918 (core_b1_remaining SERVICE_STATUS 缓冲) */
uint8_t g_svcStatus[128];

/* ---- 阶段4i (批8): .rdata 字节/字符串数组 DAT_→g_ (第3组) ---- */
/* g_bD500 @0x14011d500 (core_b3f 主题名) */
uint8_t g_bD500[0x40];
/* g_bE6b8 @0x14011e6b8 (core_b2d 对话框模板) */
uint8_t g_bE6b8[0x100];
/* g_bE870 @0x14011e870 (core_b3b/b3r_d/b3_remaining GUID 字节) */
uint8_t g_bE870[16];
/* g_bE880 @0x14011e880 (core_b3_remaining 存储设备类 GUID) */
uint8_t g_bE880[16];
/* g_bE8a0 @0x14011e8a0 (core_b3_remaining 设备实例 GUID) */
uint8_t g_bE8a0[16];
/* g_b20aa8 @0x140120aa8 (core_b2f) */
uint8_t g_b20aa8[0x20];
/* g_b21728 @0x140121728 (core_b2c 热键表 6字节/项) */
uint8_t g_b21728[0x100];
/* g_b24d00 @0x140124d00 (core_b3r_h1 文件读缓冲区) */
uint8_t g_b24d00[0x100];
/* g_b24d10 @0x140124d10 (core_b3l) */
uint8_t g_b24d10[0x80];
/* g_b24d20 @0x140124d20 (core_b3l) */
uint8_t g_b24d20[0x80];
/* g_b24e10 @0x140124e10 (core_b3j) */
uint8_t g_b24e10[0x80];
/* g_b297d0 @0x1401297d0 (core_b3l) */
uint8_t g_b297d0[0x80];
/* g_b12acc0 @0x14012acc0 (core_b8m) */
uint8_t g_b12acc0[0x100];
/* g_b12ae98 @0x14012ae98 (core_b8m) */
uint8_t g_b12ae98[0x100];
/* g_b12d158 @0x14012d158 (core_b2f) */
uint8_t g_b12d158[0x80];
/* g_b12d1a8 @0x14012d1a8 (core_b2f) */
uint8_t g_b12d1a8[0x80];
/* g_b12d1b8 @0x14012d1b8 (core_b2f) */
uint8_t g_b12d1b8[0x80];
/* g_wsz20c64 @0x140120c64 (core_b2b .rdata 字符串) */
WCHAR g_wsz20c64[0x40];
/* g_wsz21014 @0x140121014 (core_b2b .rdata 字符串) */
WCHAR g_wsz21014[0x40];

/* ---- 阶段4i (批8): 更多数组 DAT_→g_ (第4组) ---- */
/* g_d278 @0x14013d278 (core_b2c 线程 ID 表) */
DWORD g_d278[0x80];
/* g_b13caf8 @0x14013caf8 (core_b1_remaining 收尾/EXEC 标志位表) */
uint8_t g_b13caf8[0x40];
/* g_u16127d30 @0x140127d30 (core_b3r_h1 "--xxx" 宽串常量) */
uint16_t g_u16127d30[0x40];
/* g_i1255d0 @0x1401255d0 (core_b3h int32 表, show 值映射) */
int32_t g_i1255d0[0x20];
/* g_b12d1d8 @0x14012d1d8 (core_b3_remaining IID/GUID 16字节) */
uint8_t g_b12d1d8[16];

/* ---- 阶段4j (批9): 简单标量/数组 DAT_→g_ ---- */
/* 注: DAT_140147010 因类型冲突留存 DAT_+stub (见 REVIEW §41);
 * DAT_140125238(->g_fontMinus0)/140126070(->g_fontRound)/140126078(->g_dpiBase)
 * 命名冲突已在本批(阶段4l, 批11)解析为 g_fontMinus0/g_fontRound/g_dpiBase(均 double)。 */
/* g_u3211ee98 @0x14011ee98 (core_b3r_g7 uint32) */
uint32_t g_u3211ee98;
/* g_dbl26630 @0x140126630 (core_b3_remaining double) */
double g_dbl26630;
/* g_dbl26638 @0x140126638 (core_b3_remaining double) */
double g_dbl26638;
/* g_dbl26640 @0x140126640 (core_b3_remaining double) — 阶段4l 迁入 */
double g_dbl26640;
/* g_dbl26650 @0x140126650 (core_b3_remaining double) — 阶段4l 迁入 */
double g_dbl26650;
/* g_dbl26658 @0x140126658 (core_b3_remaining double) — 阶段4l 迁入 */
double g_dbl26658;
/* g_dbl26660 @0x140126660 (core_b3_remaining double) — 阶段4l 迁入 */
double g_dbl26660;
/* g_dbl27088 @0x140127088 (core_b3_remaining double) — 阶段4l 迁入 */
double g_dbl27088;
/* g_dbl27090 @0x140127090 (core_b3_remaining double) — 阶段4l 迁入 */
double g_dbl27090;
/* g_dbl27098 @0x140127098 (core_b3_remaining double) — 阶段4l 迁入 */
double g_dbl27098;
/* g_dbl270a0 @0x1401270a0 (core_b3_remaining double) — 阶段4l 迁入 */
double g_dbl270a0;
/* g_dbl270a8 @0x1401270a8 (core_b3_remaining double) — 阶段4l 迁入 */
double g_dbl270a8;
/* g_dbl26648 @0x140126648 (core_b3_remaining double) */
double g_dbl26648;
/* g_u86960 @0x140126960 (core_b3_remaining uint8 终结符) */
uint8_t g_u86960;
/* g_u32265b0 @0x1401265b0 (core_b3_remaining uint32) — 阶段4l 迁入 */
uint32_t g_u32265b0;
/* g_u6426770 @0x140126770 (core_b3_remaining uint64) — 阶段4l 迁入 */
uint64_t g_u6426770;
/* g_u6426e70 @0x140126e70 (core_b3r_g7 uint64) — 阶段4l 迁入 */
uint64_t g_u6426e70;
/* g_u642d1c8 @0x14012d1c8 (core_b3r_g5 uint64, GUID 前 8 字节) — 阶段4l 迁入 */
uint64_t g_u642d1c8;
/* g_u32126e80 @0x140126e80 (core_b3r_g7 uint32) */
uint32_t g_u32126e80;
/* g_dbl27188 @0x140127188 (core_b3r_g6 double clamp 下界) */
double g_dbl27188;
/* g_dbl27190 @0x140127190 (core_b3r_g6 double clamp 上界) */
double g_dbl27190;
/* g_u32127750 @0x140127750 (core_b3r_d uint32) */
uint32_t g_u32127750;
/* g_u1627754 @0x140127754 (core_b3r_d uint16) */
uint16_t g_u1627754;
/* g_u32127758 @0x140127758 (core_b3_remaining GUID 组合数据) */
uint32_t g_u32127758;
/* g_u32127760 @0x140127760 (core_b3_remaining GUID 组合数据) */
uint32_t g_u32127760;
/* g_u32127768 @0x140127768 (core_b3_remaining GUID 组合数据) */
uint32_t g_u32127768;
/* g_u3212776c @0x14012776c (core_b3_remaining GUID 组合数据) */
uint32_t g_u3212776c;

/* .rdata WCHAR 关键字/选项字符串 */
/* g_wsz27fc8 @0x140127fc8 (core_b3r_h2 关键字串) */
const WCHAR g_wsz27fc8[0x40];
/* g_wsz27fd0 @0x140127fd0 (core_b3r_h2 关键字串) */
const WCHAR g_wsz27fd0[0x40];
/* g_wsz28038 @0x140128038 (core_b3r_h2 关键字串) */
const WCHAR g_wsz28038[0x40];
/* g_wsz28bf4 @0x140128bf4 (core_b3r_h4 选项串) */
const WCHAR g_wsz28bf4[0x40];
/* g_wsz28eb8 @0x140128eb8 (core_b3r_h4 选项串) */
const WCHAR g_wsz28eb8[0x40];
/* g_wsz28f44 @0x140128f44 (core_b3r_h4 选项串) */
const WCHAR g_wsz28f44[0x40];

/* 壁纸 CLSID/IID */
/* g_guid2d168 @0x14012d168 (core_b1_remaining 壁纸 CLSID) */
GUID g_guid2d168;
/* g_guid2d178 @0x14012d178 (core_b1_remaining 壁纸 IID) */
GUID g_guid2d178;

/* .rdata 字节/字符串数组 */
/* g_b11e7b0 @0x14011e7b0 (core_b3_remaining 常量字节) */
uint8_t g_b11e7b0[0x20];
/* g_b120950 @0x140120950 (core_b3_remaining 常量串「存在」) */
uint8_t g_b120950[0x20];
/* g_b1215e8 @0x1401215e8 (core_b3_remaining 常量串「不存在」) */
uint8_t g_b1215e8[0x20];
/* g_b122c70 @0x140122c70 (core_b3_remaining 8字节常量) */
uint8_t g_b122c70[8];
/* g_b122cb0 @0x140122cb0 (core_b3_remaining PSP_DRVINFO 缓冲) */
uint8_t g_b122cb0[0x400];
/* g_b123768 @0x140123768 (core_b3_remaining 常量 0x1a 字节) */
uint8_t g_b123768[0x20];
/* g_b123790 @0x140123790 (core_b3_remaining 引导扇区数据 0x4e) */
uint8_t g_b123790[0x60];
/* g_b124148 @0x140124148 (core_b3_remaining 表) */
uint8_t g_b124148[0x100];
/* g_b124df8 @0x140124df8 (core_b3_remaining 异或表 0x14) */
uint8_t g_b124df8[0x20];
/* g_b126d30 @0x140126d30 (core_b3_remaining 常量 0xc) */
uint8_t g_b126d30[0x20];
/* g_b127748 @0x140127748 (core_b3r_d 字节表) */
uint8_t g_b127748[0x40];

/* ---- 阶段4j (批9): GDI+/Rtl 延迟加载函数指针导入槽 void* → g_ ---- */
/* g_pGdipLoadImageFromFile @0x14013cda0 */
void *g_pGdipLoadImageFromFile;
/* g_pRtlDosPathNameToNtPathName_U @0x14013cd60 */
void *g_pRtlDosPathNameToNtPathName_U;
/* g_pRtlCreateUnicodeString @0x14013cd68 */
void *g_pRtlCreateUnicodeString;
/* g_pRtlFreeUnicodeString @0x14013cd70 */
void *g_pRtlFreeUnicodeString;
/* g_pGetProcessMemoryInfo @0x14013cd78 */
void *g_pGetProcessMemoryInfo;
/* g_pGdiplusStartup @0x14013cd80 */
void *g_pGdiplusStartup;
/* g_pGdiplusShutdown @0x14013cd88 */
void *g_pGdiplusShutdown;
/* g_pGdipImageGetFrameDimensionsList @0x14013cda8 */
void *g_pGdipImageGetFrameDimensionsList;
/* g_pGdipImageSelectActiveFrame @0x14013cdb0 */
void *g_pGdipImageSelectActiveFrame;
/* g_pGdipImageGetFrameDimensionsCount @0x14013cdc8 */
void *g_pGdipImageGetFrameDimensionsCount;
/* g_pGdipImageGetFrameCount @0x14013cdd0 */
void *g_pGdipImageGetFrameCount;
/* g_pGdipGetPropertyItemSize @0x14013cdd8 */
void *g_pGdipGetPropertyItemSize;
/* g_pGdipGetPropertyItem @0x14013cde0 */
void *g_pGdipGetPropertyItem;
/* g_pGdipSaveImageToFile @0x14013cdf8 */
void *g_pGdipSaveImageToFile;
/* g_pGdipDrawImageI @0x14013ce00 */
void *g_pGdipDrawImageI;
/* g_pGdipDrawString @0x14013ce18 */
void *g_pGdipDrawString;
/* g_pGdipDrawImageRectRectI @0x14013ce20 */
void *g_pGdipDrawImageRectRectI;
/* g_pGdipSetPixelOffsetMode @0x14013ce40 */
void *g_pGdipSetPixelOffsetMode;
/* g_pGdipSetSmoothingMode @0x14013ce48 */
void *g_pGdipSetSmoothingMode;
/* g_pGdipLoadImageFromStream @0x14013ce50 */
void *g_pGdipLoadImageFromStream;

/* ---- 阶段4k (批10): DAT_→g_ 迁入 core_globals ---- */
/* g_pGdipGetImageType @0x14013ce58 (core_b1_remaining GDI+ 槽) */
void *g_pGdipGetImageType;
/* g_pGdipSetTextRenderingHint @0x14013ce68 (core_b1_remaining GDI+ 槽) */
void *g_pGdipSetTextRenderingHint;
/* g_pGdipGetImageDimension @0x14013ce78 (core_b1_remaining GDI+ 槽) */
void *g_pGdipGetImageDimension;
/* g_pGdipBitmapLockBits @0x14013ce80 (core_b1_remaining GDI+ 槽) */
void *g_pGdipBitmapLockBits;
/* g_pGdipBitmapUnlockBits @0x14013ce88 (core_b1_remaining GDI+ 槽) */
void *g_pGdipBitmapUnlockBits;
/* g_pGdipCreateBitmapFromResource @0x14013ce98 (core_b1_remaining GDI+ 槽) */
void *g_pGdipCreateBitmapFromResource;
/* g_pGdipScaleWorldTransform @0x14013ceb0 (core_b1_remaining GDI+ 槽) */
void *g_pGdipScaleWorldTransform;
/* g_pGdipCreateTexture @0x14013ceb8 (core_b1_remaining GDI+ 槽) */
void *g_pGdipCreateTexture;
/* g_pGdipDeleteBrush @0x14013cec0 (core_b1_remaining GDI+ 槽) */
void *g_pGdipDeleteBrush;
/* g_pGdipFillRectangle @0x14013cec8 (core_b1_remaining GDI+ 槽) */
void *g_pGdipFillRectangle;
/* g_pSetupDiSetClassInstallParamsW @0x14013cf18 (core_b1_remaining SetupAPI 槽) */
void *g_pSetupDiSetClassInstallParamsW;
/* g_pInstallHinfSectionW @0x14013cf38 (core_b1_remaining/core_b2b SetupAPI 槽) */
void *g_pInstallHinfSectionW;
/* g_pInstallHinfSection @0x14013cf40 (core_b1_remaining SetupAPI 槽) */
void *g_pInstallHinfSection;
/* g_u64d188 @0x14012d188 (core_b1_remaining COM CLSID 槽) */
uint64_t g_u64d188;
/* g_u64d198 @0x14012d198 (core_b1_remaining COM IID 槽) */
uint64_t g_u64d198;
/* g_csE138 @0x14013e138 (core_b3r_g1 全局临界区) */
CRITICAL_SECTION g_csE138;
/* g_fontSizeDef @0x1401293c0 (core_exec/core_b8h/core_b3_remaining 默认字体大小 = -0x80000000.0) */
double g_fontSizeDef;
/* g_pZwUnmapViewOfSection @0x14013cb50 (core_b1_remaining NTDLL 槽) */
void *g_pZwUnmapViewOfSection;
/* g_pZwMapViewOfSection @0x14013cca8 (core_b1_remaining NTDLL 槽) */
void *g_pZwMapViewOfSection;
/* g_pOpenSCManagerW @0x14013d000 (core_b2f Advapi32 槽) */
void *g_pOpenSCManagerW;
/* g_pOpenServiceW @0x14013d008 (core_b2f Advapi32 槽) */
void *g_pOpenServiceW;
/* g_pQueryServiceStatusEx @0x14013d010 (core_b1_remaining/core_b2f Advapi32 槽) */
void *g_pQueryServiceStatusEx;
/* g_pControlService @0x14013d018 (core_b2f Advapi32 槽) */
void *g_pControlService;
/* g_pStartServiceW @0x14013d020 (core_b2f Advapi32 槽) */
void *g_pStartServiceW;
/* g_pCloseServiceHandle @0x14013d028 (core_b2f Advapi32 槽) */
void *g_pCloseServiceHandle;
/* g_pNtQuerySystemInformation @0x14013d040 (core_b2f NTDLL 槽) */
void *g_pNtQuerySystemInformation;
/* g_pSetSuspendState @0x14013d048 (core_b2e 槽) */
void *g_pSetSuspendState;
/* g_pHookCb3d0 @0x14013d3d0 (core_b3h/core_b3_remaining 钩子回调对象槽) */
void *g_pHookCb3d0;
/* g_pSetWindowTheme @0x14013d418 (core_b3f UxTheme 槽) */
void *g_pSetWindowTheme;
/* g_pWIMMountImage @0x14013d440 (core_b3_remaining WIMGAPI 槽) */
void *g_pWIMMountImage;
/* g_pWIMUnmountImage @0x14013d448 (core_b3_remaining WIMGAPI 槽) */
void *g_pWIMUnmountImage;
/* g_pWIMMountImage2 @0x14013d450 (core_b3_remaining WIMGAPI 槽, 又名 WIMUnmountImage) */
void *g_pWIMMountImage2;
/* g_pRtlGetNtVersionNumbers @0x14013d700 (core_b3m 槽) */
void *g_pRtlGetNtVersionNumbers;
/* g_pCreateSymbolicLinkW @0x14013d778 (core_b3g 槽) */
void *g_pCreateSymbolicLinkW;
/* g_pFreeLibrary @0x14013d850 (core_b1_remaining 槽) */
void *g_pFreeLibrary;

/* ---------------- 空串 ---------------- */
WCHAR g_szEmpty[2] = {0};   /* g_szEmpty .rdata 空串 L"" */
HWND g_hPelogonWnd;   /* g_hPelogonWnd */
HWND g_hTooltipParent;   /* g_hTooltipParent */
char * g_timeServer;   /* g_timeServer */
uint32_t g_dpi;   /* g_dpi */

/* ---- 阶段5a-1 DAT_→g_ 定义 ---- */
int64_t g_pCacheBlock;
int64_t g_pComWrite;
int (*g_pGdipDisposeImage)();
int (*g_pGdipGetImageWidth)();
int (*g_pGdipGetImageHeight)();
int (*g_pGdipDeleteGraphics)();
int (*g_pGdipCreateFromHDC)();
uint32_t g_msgWndState[2];
/* ---- 阶段5a-2 DAT_→g_ (干净子集) ---- */
DWORD g_imgBufLen;   /* DAT_14013d870 位图数据大小 */
COLORREF g_dwTipsTextColor;   /* DAT_14014700c 提示文本颜色 */
COLORREF g_dwTipBkColor;   /* DAT_140147008 提示背景色 */
HWND g_hwndTray;   /* DAT_14013dd00 托盘窗口句柄 */
