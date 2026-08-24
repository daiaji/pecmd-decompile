#ifndef PECMD_GLOBALS_H
#define PECMD_GLOBALS_H

/* PECMD global variable extern declarations (DAT_->g_ migration).
 * Split from pecmd_defs.h in B0/P1: everything that used to live
 * outside the include guard now has a proper guard of its own. */
#include "pecmd_defs.h"


extern int64_t g_randState;                                   /* DAT_14013a358 */
extern int64_t g_lcgState;                                    /* DAT_14013a360 */
extern int64_t g_randPrev;                                    /* DAT_14013e1c0 */
extern HHOOK g_hHook;                                         /* DAT_14013d068 */
extern void *g_pHookData;                                     /* DAT_14013d078 */
extern HANDLE g_hWaitEvent;                                   /* DAT_14013cd10 */
extern void *g_pComState;                                     /* DAT_14013d3c0 */
extern int (*g_pOleInit)(int);                                /* DAT_14013d800 OleInitialize */
extern void (*g_pOleUninit)(void);                            /* DAT_14013d808 */
extern int32_t g_taskCount;                                   /* DAT_14013d7e8 */
extern int (*g_pWSAStartup)(int, void *);                     /* DAT_14013d4e8 WSAStartup */
extern uintptr_t (*g_psock)(int, int, int);                   /* DAT_14013d4f0 socket */
extern int (*g_pshutdown)(uintptr_t, int);                    /* DAT_14013d4f8 shutdown */
extern int (*g_pclosesocket)(uintptr_t);                      /* DAT_14013d500 closesocket */
extern int (*g_pconnect)(uintptr_t, void *, int);             /* DAT_14013d508 connect */
extern int (*g_psend)(uintptr_t, const void *, int);          /* DAT_14013d510 send */
extern int (*g_pselect)(int, void *, void *, void *, void *); /* DAT_14013d518 select */
extern int (*g_precvfrom)(uintptr_t, void *, int, int, void *, void *); /* DAT_14013d520 recvfrom */
extern int (*g_psendto)(uintptr_t, const void *, int, int, void *, int); /* DAT_14013d528 sendto */
extern int (*g_pbind)(uintptr_t, const void *, int);                     /* DAT_14013d530 bind */
extern int (*g_plisten)(uintptr_t, int);                                 /* DAT_14013d538 listen */
extern uintptr_t (*g_paccept)(uintptr_t, void *, void *);                /* DAT_14013d540 accept */
extern int (*g_pgetpeername)(uintptr_t, void *, void *);             /* DAT_14013d548 getpeername */
extern int (*g_pgetsockname)(uintptr_t, void *, void *);             /* DAT_14013d550 getsockname */
extern int (*g_pioctlsocket)(uintptr_t, int, uint32_t *);            /* DAT_14013d558 ioctlsocket */
extern int (*g_psetsockopt)(uintptr_t, int, int, const void *, int); /* DAT_14013d560 setsockopt */
extern uint32_t (*g_pntohl)(uint32_t);                               /* DAT_14013d568 ntohl */
extern uint32_t (*g_phtonl)(uint32_t);                               /* DAT_14013d570 htonl */
extern uint16_t (*g_phtons)(uint16_t);                               /* DAT_14013d578 htons */
extern int (*g_pinet_addr)(char *);                                  /* DAT_14013d580 inet_addr */
extern void *(*g_pgethostbyname)(const char *); /* DAT_14013d588 gethostbyname */
extern LPCSTR (*g_pinet_ntoa)(uint32_t);        /* DAT_14013d590 inet_ntoa */
extern int (*g_pWSAGetLastError)(void);         /* DAT_14013d598 WSAGetLastError */
extern int (*g_pWSAFDIsSet)(void *, void *);    /* DAT_14013d5a0 __WSAFDIsSet */
extern void *g_pMapBlk;                         /* DAT_14013cf50 */
extern CRITICAL_SECTION g_csCom;                /* DAT_14013e168 */
extern CRITICAL_SECTION g_csHook;               /* DAT_14013e2d0 */

/* 阶段4a: DAT_->g_ 迁入 core_globals (core_b1_remaining.c 等) */
extern int g_cmdTable3Count;                                                  /* DAT_14013a058 */
extern void *g_cmdTable3;                                                     /* DAT_14013a060 */
extern int g_cmdTable2Count;                                                  /* DAT_14013a078 */
extern void *g_cmdTable2;                                                     /* DAT_14013a080 */
extern int g_cmdTable4Count;                                                  /* DAT_14013a098 */
extern void *g_cmdTable4;                                                     /* DAT_14013a0a0 */
extern int g_cmdTable5Len;                                                    /* DAT_14013a0c8 */
extern void *g_cmdTable5;                                                     /* DAT_14013a0c0 */
extern int g_cmdTable1Count;                                                  /* DAT_14013ca98 */
extern void *g_cmdTable1;                                                     /* DAT_14013caa0 */
extern HANDLE g_hStdIn;                                                       /* DAT_14013a330 */
extern HANDLE g_hStdErr;                                                      /* DAT_14013a338 */
extern HANDLE g_hStdOut;                                                      /* DAT_14013a340 */
extern char g_minintFlag;                                                     /* DAT_14013ca48 */
extern uint32_t g_exitCodeCache;                                              /* DAT_14013caf0 */
extern uint8_t g_wallpaperMode;                                               /* DAT_14013cad0 */
extern void *g_pMemQuery;                                                     /* DAT_14013a048 */
extern void *g_pProcMemRead;                                                  /* DAT_14013ca58 */
extern void *g_pShutdownFlag;                                                 /* DAT_14013ca08 */
extern void *g_pPendingObj;                                                   /* DAT_14013cac0 */
extern void *g_pGetFileVersionInfoExW;                                        /* DAT_14013ca40 */
extern void *g_pGetFileVersionInfoSizeExW;                                    /* DAT_14013ca38 */
extern int64_t *g_pVolHandles;                                                /* DAT_14013caa8 */
extern void *g_pSysAllocString;                                               /* DAT_14013c978 */
extern void *g_pSysFreeString;                                                /* DAT_14013c980 */
extern HMODULE g_hOleaut32;                                                   /* DAT_14013c9b0 */
extern void *g_pImDiskCtrl;                                                   /* DAT_14013d0a0 */
extern void *g_pImDiskMask;                                                   /* DAT_14013d098 */
extern void *g_pDevOpen;                                                      /* DAT_14013d0c0 */
extern void *g_pDevOpen2;                                                     /* DAT_14013d0c8 */
extern void *g_pNtOpenSymLink;                                                /* DAT_14013d0f8 */
extern void *g_pNtQuerySymLink;                                               /* DAT_14013d100 */
extern int (*g_pCoCreateInstance)(void *, void *, uint32_t, void *, void **); /* DAT_14013d830 */
extern HMODULE g_hGdiPlus;                                                    /* DAT_14013ccc0 */
extern void *g_gdiplusToken;                                                  /* DAT_14013cfc0 */

/* 阶段4b: DAT_->g_ 迁入 core_globals (批2) */
extern uint8_t g_lowVerFlag;     /* DAT_14013a349 */
extern uint32_t g_sysColor8;     /* DAT_14013a350 */
extern int g_tooltipTimeout;     /* DAT_14013a258 */
extern uint8_t g_runMode;        /* DAT_14013caf9 */
extern uint8_t g_flagCB08;       /* DAT_14013cb08 */
extern uint32_t g_bitFlags;      /* DAT_14013cb58 */
extern uint32_t g_sysVerClass;   /* DAT_14013cb98 */
extern uint8_t g_foregroundFlag; /* DAT_14013ccb2 */
extern uint8_t g_flagCCB3;       /* DAT_14013ccb3 */
extern uint8_t g_tipAlign;       /* DAT_14013cf82 */
extern int g_tipOffX;            /* DAT_14013cf98 */
extern int g_tipOffY;            /* DAT_14013cf9c */
extern uint8_t g_sysStartFlag;   /* DAT_14013d030 */
extern uint8_t g_answerFlag;     /* DAT_14013d031 */
extern int32_t g_msgLockCount;   /* DAT_14013d080 */
extern int32_t g_qMode;          /* DAT_14013d5a8 */
extern uint8_t g_flagD6F5;       /* DAT_14013d6f5 */
extern int32_t g_flagD5AC;       /* DAT_14013d5ac */
extern uint8_t g_flagD3EF;       /* DAT_14013d3ef */
extern uint8_t g_flagD321;       /* DAT_14013d321 */
extern uint8_t g_iconRefCount;   /* DAT_14013d322 */
extern uint8_t g_msgWndMode;     /* DAT_14013d323 */
extern uint8_t g_fgWndLock;      /* DAT_14013d324 */
extern int g_dblClickFlag;       /* DAT_14013d4e0 */
extern char g_scrollOff;         /* DAT_14013a320 */
extern HMODULE g_hRichEdit;      /* DAT_14013d2f8 */
extern int32_t g_flagD6F0;       /* DAT_14013d6f0 */
extern uint32_t g_guidPtrState;  /* DAT_14013d840 */
extern uint32_t g_flagD7FC;      /* DAT_14013d7fc */
extern uint8_t g_flagA298;       /* DAT_14013a298 */
extern int8_t g_flagA24B;        /* DAT_14013a24b */
extern uint8_t g_flagA24E;       /* DAT_14013a24e */
extern HWND g_ownerWnd;          /* DAT_14013d3d8 */
extern HWND g_wndCF68;           /* DAT_14013cf68 */
extern HICON g_hIconCF88;        /* DAT_14013cf88 */
extern HFONT g_hFontFast;        /* DAT_14013cfa8 */

/* 阶段4c: DAT_->g_ 迁入 core_globals (批3) */
extern int g_readBufSize;              /* DAT_14013a210 */
extern int g_ramdrivFlag;              /* DAT_14013a220 */
extern int16_t g_transState;           /* DAT_14013a24c */
extern int8_t g_popmenuFlag;           /* DAT_14013a348 */
extern uint32_t g_msgRetCode;          /* DAT_14013c91c */
extern uint8_t g_flagCCC9;             /* DAT_14013ccc9 */
extern uint8_t g_topiconFlag;          /* DAT_14013cccb */
extern int16_t g_hotkeyIdx;            /* DAT_14013cccc */
extern uint8_t g_logoActiveFlag;       /* DAT_14013cccf */
extern uint32_t g_hookBusyFlag;        /* DAT_14013d070 */
extern uint32_t g_hookReentry;         /* DAT_14013d074 */
extern HMODULE g_hImdiskCpl;           /* DAT_14013d268 */
extern uint8_t g_sysStartuped;         /* DAT_14013d271 */
extern uint8_t g_initOnceFlag;         /* DAT_14013d272 */
extern uint32_t g_threadTableInitFlag; /* DAT_14013d290 */
extern HHOOK g_hHook2;                 /* DAT_14013d3c8 */
extern uint8_t g_flagD6F6;             /* DAT_14013d6f6 */
extern uint8_t g_flagE1B8;             /* DAT_14013e1b8 */
extern uint32_t g_randSeedAccum;       /* DAT_14013e1c8 */
extern uint8_t g_flagE1F4;             /* DAT_14013e1f4 */
extern HFONT g_hFontE2B0;              /* DAT_14013e2b0 */
extern uint64_t g_u64B960;             /* DAT_14013b960 */
extern uint64_t g_u64BF50;             /* DAT_14013bf50 */
extern uint64_t g_u64BF54;             /* DAT_14013bf54 */
extern uint64_t g_u64BF58;             /* DAT_14013bf58 */
extern uint64_t g_u64E598;             /* DAT_14013e598 */
extern uint64_t g_u64E600;             /* DAT_14013e600 */
extern uint64_t g_u64EC88;             /* DAT_14013ec88 */
extern uint64_t g_u64EEE8;             /* DAT_14013eee8 */
extern uint64_t g_u64EEF0;             /* DAT_14013eef0 */
extern uint64_t g_u64F520;             /* DAT_14013f520 */

/* 阶段4d: DAT_->g_ 迁入 core_globals (批4) */
extern uint32_t g_u32A22C; /* DAT_14013a22c */
extern int g_intA238;      /* DAT_14013a238 */
extern int g_intA23C;      /* DAT_14013a23c */
extern int g_intA240;      /* DAT_14013a240 */
extern int g_intA244;      /* DAT_14013a244 */
extern uint8_t g_u8A24A;   /* DAT_14013a24a */
extern DWORD g_dwC934;     /* DAT_14013c934 */
extern DWORD g_dwC96C;     /* DAT_14013c96c */
extern HWND g_hwndC9C8;    /* DAT_14013c9c8 */
extern uint64_t g_u64C9E8; /* DAT_14013c9e8 */
extern uint64_t g_u64C9F0; /* DAT_14013c9f0 */
extern uint64_t g_u64CA20; /* DAT_14013ca20 */
extern uint8_t g_u8CA49;   /* DAT_14013ca49 */
extern uint64_t g_u64CA50; /* DAT_14013ca50 */
extern uint64_t g_u64CB60; /* DAT_14013cb60 */
extern uint8_t g_u8CCB1;   /* DAT_14013ccb1 */
extern int64_t g_i64CCB8;  /* DAT_14013ccb8 */
extern uint64_t g_u64CD00; /* DAT_14013cd00 */
extern HWND g_hwndCF78;    /* DAT_14013cf78 */
extern uint8_t g_u8CF83;   /* DAT_14013cf83 */
extern uint8_t g_u8CF84;   /* DAT_14013cf84 */
extern uint8_t g_u8CF85;   /* DAT_14013cf85 */
extern uint8_t g_u8CF87;   /* DAT_14013cf87 */
extern int64_t g_i64D170;  /* DAT_14013d170 */
extern char g_charD294;    /* DAT_14013d294 */
extern HWND g_hwndD310;    /* DAT_14013d310 */
extern int64_t g_i64D428;  /* DAT_14013d428 */
extern int64_t g_i64D438;  /* DAT_14013d438 */
extern uint8_t g_u8D5D0;   /* DAT_14013d5d0 */
extern int32_t g_i32D6F4;  /* DAT_14013d6f4 */
extern uint8_t g_u8D6F7;   /* DAT_14013d6f7 */
extern HHOOK g_hookD710;   /* DAT_14013d710 */
extern uint64_t g_u64D7D8; /* DAT_14013d7d8 */
extern int32_t g_i32D7DC;  /* DAT_14013d7dc */
extern int32_t g_i32D7EC;  /* DAT_14013d7ec */
extern int64_t g_i64E120;  /* DAT_14013e120 */

/* ---- 阶段4e (批5): DAT_→g_ 迁入 core_globals ---- */
extern uint64_t g_threadTableCap;        /* DAT_14013d280 线程表容量 */
extern uint64_t g_threadTableCnt;        /* DAT_14013d288 线程表计数 */
extern uint64_t g_loadThreadId;          /* DAT_14013d3e0 当前加载线程 id */
extern uint64_t g_osVersion;             /* DAT_14013d708 OS 版本 (双 32 位打包) */
extern int64_t g_intE128;                /* DAT_14013e128 */
extern LPCWSTR g_svcCmdStr;              /* DAT_14013ca18 service 命令串 */
extern LPWSTR g_qOutBuf;                 /* DAT_14013d5b8 -q 输出缓冲 */
extern RECT *g_pCurRect;                 /* DAT_14013cfa0 当前 RECT 指针 */
extern RECT g_rectCF90;                  /* DAT_14013cf90 RECT 值 */
extern void *g_pAppData;                 /* DAT_14013cfb0 应用/窗口过程数据基址 */
extern void *g_winHandle;                /* DAT_14013d718 关联窗口/句柄 */
extern void *g_winData;                  /* DAT_14013d720 关联数据 */
extern void *g_pOnWndProc2;              /* DAT_14013c9d8 OnWndProc2 脚本 */
extern void *g_pOnWndProc3;              /* DAT_14013c9e0 OnWndProc3 脚本 */
extern void *g_pFiber;                   /* DAT_14013ca00 目标 fiber 句柄 */
extern HMODULE g_hOle32;                 /* DAT_14013cfe0 OLE32 模块句柄 */
extern void *g_pImageBuf;                /* DAT_14013e0e0 位图数据指针 */
extern uint32_t g_helpWord;              /* DAT_14013a228 帮助模式字 */
extern uint32_t g_helpHi32;              /* DAT_14013a230 帮助资源高 32 位 */
extern uint8_t g_charTableF;             /* DAT_14013a248 字符表开关 */
extern int8_t g_flagA249;                /* DAT_14013a249 .data初值0xFF, 有符号语义 */
extern int32_t g_helpVerFlag;            /* DAT_14013a34c 帮助版本标志 */
extern uint8_t g_flagCF86;               /* DAT_14013cf86 提示串显示/前导'*'计数 */
extern uint8_t g_flagCCCE;               /* DAT_14013ccce */
extern volatile uint32_t g_lastThreadId; /* DAT_14013d7f8 最近线程 id 槽 */

/* ---- 阶段4h (批7): 简单标量 DAT_→g_ ---- */
extern uint64_t g_u64FontA98; /* DAT_140120a98 (core_b2f) */
extern uint32_t g_u32FontAa0; /* DAT_140120aa0 (core_b2f) */
extern double g_dbl1178;      /* DAT_140121178 (core_b2f) */
extern double g_dbl1668;      /* DAT_140121668 (core_b2f) */
extern float g_flae4;         /* DAT_140121ae4 (core_b2f) */
extern float g_flae8;         /* DAT_140121ae8 (core_b2f) */
extern double g_dbl2578;      /* DAT_140122578 (core_b2f) */
extern double g_dbl2598;      /* DAT_140122598 (core_b2f) */
extern double g_dbl2b368;     /* DAT_14012b368 */
extern double g_dbl2b370;     /* DAT_14012b370 */
extern double g_dbl2b378;     /* DAT_14012b378 */
extern double g_dbl2b380;     /* DAT_14012b380 */
extern uint64_t g_u6497e8;    /* DAT_1401297e8 */
extern uint64_t g_u6497f0;    /* DAT_1401297f0 */
extern uint64_t g_u6497f8;    /* DAT_1401297f8 */
extern uint32_t g_u329800;    /* DAT_140129800 */
extern uint16_t g_u169804;    /* DAT_140129804 */
extern uint8_t g_u897806;     /* DAT_140129806 */
extern uint64_t g_u6453d8;    /* DAT_1401253d8 */
extern uint32_t g_u3253e0;    /* DAT_1401253e0 */
extern uint64_t g_u6459e4;    /* DAT_1401259e4 */
extern uint64_t g_u6459e8;    /* DAT_1401259e8 */
extern uint32_t g_u326778;    /* DAT_140126778 */
extern uint16_t g_u162677c;   /* DAT_14012677c */
extern uint64_t g_u64ee90;    /* DAT_14011ee90 */
extern uint64_t g_u646e78;    /* DAT_140126e78 */
extern uint64_t g_u6435a0;    /* DAT_1401235a0 */
extern double g_dbl94e8;      /* DAT_1401294e8 */
extern float g_flaf0;         /* DAT_140121af0 */
extern float g_flaf4;         /* DAT_140121af4 */
extern uint32_t g_u322570;    /* DAT_140122570 */
extern uint16_t g_u162574;    /* DAT_140122574 */

/* ---- 阶段4i (批8): 简单标量/结构 DAT_→g_ (第1组) ---- */
extern double g_dbl20b28;              /* DAT_140120b28 (亦 _DAT_140120b28) */
extern uint16_t g_u161be4;             /* DAT_140121be4 */
extern uint16_t g_u161bec;             /* DAT_140121bec */
extern double g_dbl25230;              /* DAT_140125230 */
extern WCHAR *g_pwszD220;              /* DAT_14013d220 */
extern CRITICAL_SECTION g_csThreadTbl; /* DAT_14013d228 */
extern WCHAR *g_pwszD250;              /* DAT_14013d250 */
extern WCHAR *g_pwszD258;              /* DAT_14013d258 */
extern CRITICAL_SECTION g_csDisk;      /* DAT_14013e0e8 */
extern HHOOK g_hMsgHook;               /* DAT_14013cf58 (原 link_stubs g_hMsgHook) */
extern uint64_t g_u64e770;             /* DAT_14011e770 */
extern void *g_pSvStatus;              /* DAT_14013c910 */
extern int64_t *g_pi64e118;            /* DAT_14013e118 */
extern uint64_t g_u64c9d0[4];          /* DAT_14013c9d0 */
extern uint64_t g_u64cd08;             /* DAT_14013cd08 物理内存映射句柄 */
extern uint8_t g_svcStatus[128];       /* DAT_14013c918 */
extern uint8_t g_bD500[0x40];          /* DAT_14011d500 */
extern uint8_t g_bE6b8[0x100];         /* DAT_14011e6b8 */
extern uint8_t g_bE870[16];            /* DAT_14011e870 */
extern uint8_t g_bE880[16];            /* DAT_14011e880 */
extern uint8_t g_bE8a0[16];            /* DAT_14011e8a0 */
extern uint8_t g_b20aa8[0x20];         /* DAT_140120aa8 */
extern uint8_t g_b21728[0x100];        /* DAT_140121728 */
extern uint8_t g_b24d00[0x100];        /* DAT_140124d00 */
extern uint8_t g_b24d10[0x80];         /* DAT_140124d10 */
extern uint8_t g_b24d20[0x80];         /* DAT_140124d20 */
extern uint8_t g_b24e10[0x80];         /* DAT_140124e10 */
extern uint8_t g_b297d0[0x80];         /* DAT_1401297d0 */
extern uint8_t g_b12acc0[0x100];       /* DAT_14012acc0 */
extern uint8_t g_b12ae98[0x100];       /* DAT_14012ae98 */
extern uint8_t g_b12d158[0x80];        /* DAT_14012d158 */
extern uint8_t g_b12d1a8[0x80];        /* DAT_14012d1a8 */
extern uint8_t g_b12d1b8[0x80];        /* DAT_14012d1b8 */
extern WCHAR g_wsz20c64[0x40];         /* DAT_140120c64 */
extern WCHAR g_wsz21014[0x40];         /* DAT_140121014 */
extern DWORD g_d278[0x80];             /* DAT_14013d278 */
extern uint8_t g_b13caf8[0x40];        /* DAT_14013caf8 */
extern uint16_t g_u16127d30[0x40];     /* DAT_140127d30 */
extern int32_t g_i1255d0[0x20];        /* DAT_1401255d0 */
extern uint8_t g_b12d1d8[16];          /* DAT_14012d1d8 */

/* ---- 阶段4j (批9): DAT_→g_ 迁入 extern ---- */
extern uint8_t g_b11e7b0[0x20];      /* DAT_14011e7b0 (core_b3_remaining 常量字节) */
extern uint8_t g_b120950[0x20];      /* DAT_140120950 (core_b3_remaining 常量串「存在」) */
extern uint8_t g_b1215e8[0x20];      /* DAT_1401215e8 (core_b3_remaining 常量串「不存在」) */
extern uint8_t g_b122c70[8];         /* DAT_140122c70 (core_b3_remaining 8字节常量) */
extern uint8_t g_b122cb0[0x400];     /* DAT_140122cb0 (core_b3_remaining PSP_DRVINFO 缓冲) */
extern uint8_t g_b123768[0x20];      /* DAT_140123768 (core_b3_remaining 常量 0x1a 字节) */
extern uint8_t g_b123790[0x60];      /* DAT_140123790 (core_b3_remaining 引导扇区数据 0x4e) */
extern uint8_t g_b124148[0x100];     /* DAT_140124148 (core_b3_remaining 表) */
extern uint8_t g_b124df8[0x20];      /* DAT_140124df8 (core_b3_remaining 异或表 0x14) */
extern uint8_t g_b126d30[0x20];      /* DAT_140126d30 (core_b3_remaining 常量 0xc) */
extern uint8_t g_b127748[0x40];      /* DAT_140127748 (core_b3r_d 字节表) */
extern double g_dbl26630;            /* DAT_140126630 */
extern double g_dbl26638;            /* DAT_140126638 */
extern double g_dbl26640;            /* DAT_140126640 (阶段4l) */
extern double g_dbl26650;            /* DAT_140126650 (阶段4l) */
extern double g_dbl26658;            /* DAT_140126658 (阶段4l) */
extern double g_dbl26660;            /* DAT_140126660 (阶段4l) */
extern double g_dbl27088;            /* DAT_140127088 (阶段4l) */
extern double g_dbl27090;            /* DAT_140127090 (阶段4l) */
extern double g_dbl27098;            /* DAT_140127098 (阶段4l) */
extern double g_dbl270a0;            /* DAT_1401270a0 (阶段4l) */
extern double g_dbl270a8;            /* DAT_1401270a8 (阶段4l) */
extern double g_dbl21630;            /* DAT_140121630 (阶段4l, core_b2f) */
extern double g_dbl21bf0;            /* DAT_140121bf0 (阶段4l, core_b2f) */
extern double g_dbl21c80;            /* DAT_140121c80 (阶段4l, core_b2f) */
extern double g_dbl22580;            /* DAT_140122580 (阶段4l, core_b2f) */
extern double g_dbl22588;            /* DAT_140122588 (阶段4l, core_b2f) */
extern double g_dbl22590;            /* DAT_140122590 (阶段4l, core_b2f) */
extern double g_dbl26648;            /* DAT_140126648 */
extern uint8_t g_u86960;             /* DAT_140126960 null 终结符 */
extern uint32_t g_u32265b0;          /* DAT_1401265b0 (阶段4l, core_b3_remaining) */
extern uint64_t g_u6426770;          /* DAT_140126770 (阶段4l, core_b3_remaining) */
extern uint64_t g_u6426e70;          /* DAT_140126e70 (阶段4l, core_b3r_g7) */
extern uint64_t g_u642d1c8;          /* DAT_14012d1c8 (阶段4l, core_b3r_g5) */
extern uint32_t g_u32126e80;         /* DAT_140126e80 */
extern double g_dbl27188;            /* DAT_140127188 clamp 下界 */
extern double g_dbl27190;            /* DAT_140127190 clamp 上界 */
extern uint32_t g_u32127750;         /* DAT_140127750 */
extern uint16_t g_u1627754;          /* DAT_140127754 */
extern uint32_t g_u32127758;         /* DAT_140127758 */
extern uint32_t g_u32127760;         /* DAT_140127760 */
extern uint32_t g_u32127768;         /* DAT_140127768 */
extern uint32_t g_u3212776c;         /* DAT_14012776c */
extern const WCHAR g_wsz27fc8[0x40]; /* DAT_140127fc8 */
extern const WCHAR g_wsz27fd0[0x40]; /* DAT_140127fd0 */
extern const WCHAR g_wsz28038[0x40]; /* DAT_140128038 */
extern const WCHAR g_wsz28bf4[0x40]; /* DAT_140128bf4 */
extern const WCHAR g_wsz28eb8[0x40]; /* DAT_140128eb8 */
extern const WCHAR g_wsz28f44[0x40]; /* DAT_140128f44 */
extern GUID g_guid2d168;             /* DAT_14012d168 壁纸 CLSID */
extern GUID g_guid2d178;             /* DAT_14012d178 壁纸 IID */
extern uint32_t g_u3211ee98;         /* DAT_14011ee98 */

extern void *g_pGdipLoadImageFromFile;            /* DAT_14013cda0 */
extern void *g_pRtlDosPathNameToNtPathName_U;     /* DAT_14013cd60 */
extern void *g_pRtlCreateUnicodeString;           /* DAT_14013cd68 */
extern void *g_pRtlFreeUnicodeString;             /* DAT_14013cd70 */
extern void *g_pGetProcessMemoryInfo;             /* DAT_14013cd78 */
extern void *g_pGdiplusStartup;                   /* DAT_14013cd80 */
extern void *g_pGdiplusShutdown;                  /* DAT_14013cd88 */
extern void *g_pGdipImageGetFrameDimensionsList;  /* DAT_14013cda8 */
extern void *g_pGdipImageSelectActiveFrame;       /* DAT_14013cdb0 */
extern void *g_pGdipImageGetFrameDimensionsCount; /* DAT_14013cdc8 */
extern void *g_pGdipImageGetFrameCount;           /* DAT_14013cdd0 */
extern void *g_pGdipGetPropertyItemSize;          /* DAT_14013cdd8 */
extern void *g_pGdipGetPropertyItem;              /* DAT_14013cde0 */
extern void *g_pGdipSaveImageToFile;              /* DAT_14013cdf8 */
extern void *g_pGdipDrawImageI;                   /* DAT_14013ce00 */
extern void *g_pGdipDrawString;                   /* DAT_14013ce18 */
extern void *g_pGdipDrawImageRectRectI;           /* DAT_14013ce20 */
extern void *g_pGdipSetPixelOffsetMode;           /* DAT_14013ce40 */
extern void *g_pGdipSetSmoothingMode;             /* DAT_14013ce48 */
extern void *g_pGdipLoadImageFromStream;          /* DAT_14013ce50 */
extern void *g_pGdipGetImageType;                 /* DAT_14013ce58 */
extern void *g_pGdipSetTextRenderingHint;         /* DAT_14013ce68 */
extern void *g_pGdipGetImageDimension;            /* DAT_14013ce78 */
extern void *g_pGdipBitmapLockBits;               /* DAT_14013ce80 */
extern void *g_pGdipBitmapUnlockBits;             /* DAT_14013ce88 */
extern void *g_pGdipCreateBitmapFromResource;     /* DAT_14013ce98 */
extern void *g_pGdipScaleWorldTransform;          /* DAT_14013ceb0 */
extern void *g_pGdipCreateTexture;                /* DAT_14013ceb8 */
extern void *g_pGdipDeleteBrush;                  /* DAT_14013cec0 */
extern void *g_pGdipFillRectangle;                /* DAT_14013cec8 */
extern void *g_pSetupDiSetClassInstallParamsW;    /* DAT_14013cf18 */
extern void *g_pInstallHinfSectionW;              /* DAT_14013cf38 */
extern void *g_pInstallHinfSection;               /* DAT_14013cf40 */
extern uint64_t g_u64d188;                        /* DAT_14012d188 COM CLSID 槽 */
extern uint64_t g_u64d198;                        /* DAT_14012d198 COM IID 槽 */
extern CRITICAL_SECTION g_csE138;                 /* DAT_14013e138 */
extern double g_fontSizeDef;                      /* DAT_1401293c0 默认字体大小 */
extern void *g_pZwUnmapViewOfSection;             /* DAT_14013cb50 */
extern void *g_pZwMapViewOfSection;               /* DAT_14013cca8 */
extern void *g_pOpenSCManagerW;                   /* DAT_14013d000 */
extern void *g_pOpenServiceW;                     /* DAT_14013d008 */
extern void *g_pQueryServiceStatusEx;             /* DAT_14013d010 */
extern void *g_pControlService;                   /* DAT_14013d018 */
extern void *g_pStartServiceW;                    /* DAT_14013d020 */
extern void *g_pCloseServiceHandle;               /* DAT_14013d028 */
extern void *g_pNtQuerySystemInformation;         /* DAT_14013d040 */
extern void *g_pSetSuspendState;                  /* DAT_14013d048 */
extern void *g_pHookCb3d0;                        /* DAT_14013d3d0 */
extern void *g_pSetWindowTheme;                   /* DAT_14013d418 */
extern void *g_pWIMMountImage;                    /* DAT_14013d440 */
extern void *g_pWIMUnmountImage;                  /* DAT_14013d448 */
extern void *g_pWIMMountImage2;                   /* DAT_14013d450 */
extern void *g_pRtlGetNtVersionNumbers;           /* DAT_14013d700 */
extern void *g_pCreateSymbolicLinkW;              /* DAT_14013d778 */
extern void *g_pFreeLibrary;                      /* DAT_14013d850 */

extern WCHAR g_szEmpty[]; /* DAT_14011c638 .rdata 空串 */

/* ---- 阶段5a-1 DAT_→g_ ---- */
extern HWND g_hPelogonWnd;    /* DAT_14013cf60 */
extern HWND g_hTooltipParent; /* DAT_14013cfb8 */
extern char *g_timeServer;    /* DAT_14013d5c8 */
extern uint32_t g_dpi;        /* DAT_14013d868 */

/* ---- 阶段5a-1 DAT_→g_ 声明 ---- */
extern int64_t g_pCacheBlock;
extern int64_t g_pComWrite;
extern int (*g_pGdipDisposeImage)();
extern int (*g_pGdipGetImageWidth)();
extern int (*g_pGdipGetImageHeight)();
extern int (*g_pGdipDeleteGraphics)();
extern int (*g_pGdipCreateFromHDC)();
extern uint32_t g_msgWndState[2];
extern DWORD g_imgBufLen;                                  /* DAT_14013d870 */
extern COLORREF g_dwTipsTextColor;                         /* DAT_14014700c */
extern COLORREF g_dwTipBkColor;                            /* DAT_140147008 */
extern HWND g_hwndTray;                                    /* DAT_14013dd00 */
extern GUID g_guidDevInstance;                             /* DAT_14011e890 */
extern GUID g_clsidCoCreate;                               /* DAT_14012d1e8 */
extern GUID g_iidCoCreate;                                 /* DAT_14012d1f8 */
extern DWORD (*g_pWlanOpenHandle)(DWORD, void *, DWORD *); /* DAT_14013d788 */
extern DWORD (*g_pWlanGetAvailableNetworkList)(HANDLE, LPCWSTR, LPCWSTR,
                                               void *);               /* DAT_14013d7b8 */
extern DWORD (*g_pWlanCloseHandle)(HANDLE, void *);                   /* DAT_14013d7a0 */
extern DWORD (*g_pWlanConnect)(HANDLE, LPCWSTR, void *, void *);      /* DAT_14013d7d0 */
extern DWORD (*g_pWlanScan)(HANDLE, LPCWSTR);                         /* DAT_14013d7b0 */
extern DWORD (*g_pWlanRegisterNotification)(void);                    /* DAT_14013d7a8 */
extern DWORD (*g_pWlanEnumInterfaces)(HANDLE, void *, void **);       /* DAT_14013d790 */
extern DWORD (*g_pWlanDisconnect)(HANDLE, LPCWSTR, LPCWSTR);          /* DAT_14013d7c0 */
extern DWORD (*g_pWlanSetProfile)(HANDLE, LPCWSTR, LPCWSTR, LPCWSTR); /* DAT_14013d798 */
extern void (*g_pWlanFreeMemory)(void);                               /* DAT_14013d7c8 */
extern BOOL (*g_pWTSQueryUserToken)(DWORD, HANDLE *);                 /* DAT_14013c988 */
extern int (*g_pWTSSendMessageW)(HANDLE, DWORD, LPCWSTR, DWORD, LPCWSTR, DWORD, DWORD, DWORD,
                                 DWORD *, BOOL);                 /* DAT_14013c990 */
extern int (*g_pWTSGetActiveConsoleSessionId)(void);             /* DAT_14013c998 */
extern BOOL (*g_pCreateEnvironmentBlock)(void **, HANDLE, BOOL); /* DAT_14013c9a0 */
extern void (*g_pDestroyEnvironmentBlock)(void);                 /* DAT_14013c9a8 */
extern int (*g_pSetDisplayConfig)(UINT, UINT, UINT);             /* DAT_14013cfe8 */
extern int (*g_pSetDeviceGammaRamp)(HDC, void *);                /* DAT_14013cff0 */
extern int (*g_pGetDeviceGammaRamp)(HDC, void *);                /* DAT_14013cff8 */
extern DWORD (*g_pGetVolumeInformationByHandleW)(HANDLE, LPWSTR, DWORD,
                                                 LPDWORD);                     /* DAT_14013d358 */
extern int (*g_pWIMCloseHandle)();                                             /* DAT_14013d490 */
extern int (*g_pWIMGetMountedImageHandle)();                                   /* DAT_14013d498 */
extern int (*g_pWIMGetMountedImageNum)();                                      /* DAT_14013d4a0 */
extern int (*g_pWIMCommitImageHandle)();                                       /* DAT_14013d4a8 */
extern int (*g_pWIMCommitImageNum)();                                          /* DAT_14013d4b0 */
extern int (*g_pWIMUnmountImageHandle)();                                      /* DAT_14013d4b8 */
extern int (*g_pWIMUnmountImageNum)();                                         /* DAT_14013d4c0 */
extern void (*g_pCoTaskMemFree)(LPVOID);                                       /* DAT_14013d728 */
extern DWORD (*g_pGetFinalPathNameByHandleW)(HANDLE, LPWSTR, DWORD, DWORD);    /* DAT_14013d780 */
extern int (*g_pUiCallback)(int, int);                                         /* DAT_14013d810 */
extern int (*g_pOle32Slot828)(int, int, int, int, int, int, int, int, int);    /* DAT_14013d828 */
extern int (*g_pCreateStreamOnHGlobal)(void *hGlobal, int fDel, void **ppstm); /* DAT_14013d838 */
extern int (*g_pComLoad)(void *punk, int a, int b, void *riid, void **ppv);    /* DAT_14013d860 */
extern uint64_t g_u64_124e40;                                                  /* DAT_140124e40 */
extern uint64_t g_u64_124e48;                                                  /* DAT_140124e48 */
extern uint64_t g_u64_12d058;                                                  /* DAT_14012d058 */
extern uint64_t g_u64_12d060;                                                  /* DAT_14012d060 */
extern void *g_pGdipNewPrivateFontCollection;                                  /* DAT_14013ced0 */
extern void *g_pGdipDeletePrivateFontCollection;                               /* DAT_14013ced8 */
extern void *g_pGdipPrivateAddMemoryFont;                                      /* DAT_14013cee0 */
extern void *g_pGdipGetFontCollectionFamilyList;                               /* DAT_14013cee8 */
extern void *g_pGdipGetFamilyName;                                             /* DAT_14013cef0 */
extern int (*g_pWIMGetMountedImages)(void *buf, uint32_t *flags);              /* DAT_14013d458 */
extern int (*g_pWIMGetMountedImageCount)(void *buf, uint32_t *flags);          /* DAT_14013d460 */
extern BOOL (*g_pSetupDiCallClassInstaller)(DWORD, void *, void *);            /* DAT_14013cf20 */
extern void *(*g_pSetupDiGetClassDevsW)(const void *);                         /* DAT_14013cf00 */
extern BOOL (*g_pSetupDiDestroyDeviceInfoList)(void *);                        /* DAT_14013cf08 */
extern int (*g_pSetupDiEnumDeviceInfo)(void *, DWORD);                         /* DAT_14013cf28 */
extern void (*g_pGdipCreateBitmapFromHBITMAP)();                               /* DAT_14013cde8 */
extern void (*g_pGdipCreateHBITMAPFromBitmap)();                               /* DAT_14013cdf0 */
extern void (*g_pGdipCloneBitmapAreaI)();                                      /* DAT_14013cea0 */
extern void (*g_pGdipCreateBitmapFromScan0)();                                 /* DAT_14013ce90 */
extern void (*g_pGdipGetImageGraphicsContext)();                               /* DAT_14013cea8 */
extern void (*g_pGdipCreateHICONFromBitmap)();                                 /* DAT_14013ce60 */
extern void (*g_pGdipCreateBitmapFromHICON)();                                 /* DAT_14013ce70 */
extern int (*g_pGetAdaptersInfo)(int64_t, uint32_t *);                         /* DAT_14013d4d8 */
extern void (*g_pGdipGetImageEncodersSize)(uint32_t *, uint32_t *);            /* DAT_14013cdb8 */
extern void (*g_pGdipGetImageEncoders)(uint32_t, uint32_t, int64_t);           /* DAT_14013cdc0 */
extern int64_t (*g_pSetupIterateCabinetW)(void *, int);                        /* DAT_14013cf30 */
extern void (*g_pCmGetDevNodeStatus)(void *, void *, DWORD);                   /* DAT_14013cf48 */
extern int (*g_pGdipDrawImageRectI)(void *, void *, int, int, int64_t, int);   /* DAT_14013cd98 */
extern int (*g_pGdipSetInterpolationMode)(void *, int);                        /* DAT_14013ce38 */
extern int (*g_pSetupDiGetINFClassW)(LPCWSTR, GUID *, LPWSTR, DWORD, DWORD);   /* DAT_14013cef8 */
extern BOOL (*g_pSetupDiDestroyDeviceInfoListRev)(HDEVINFO);                   /* DAT_14013cf10 */
extern void (*g_pMciSendStringW)(LPCWSTR, int);                                /* DAT_14013d050 */
extern int (*g_pDhcpNotifyConfigChange)(uint64_t, int64_t, int, uint32_t, int, uint32_t,
                                        uint8_t); /* DAT_14013d4d0 */
extern void *g_pConfigStr;                        /* DAT_14013e110 */
extern uint64_t (*g_pWIMCreateFile)(uint64_t wim, uint32_t mode, int f3, int f4, int f5,
                                    uint32_t *out);                        /* DAT_14013d468 */
extern int (*g_pWIMSetTemporaryPath)(uint64_t wim, WCHAR *path);           /* DAT_14013d470 */
extern uint64_t (*g_pWimLoadImage)(uint64_t wim, unsigned int index);      /* DAT_14013d478 */
extern int (*g_pWIMHandleOp480)(uint64_t h, uint64_t data, uint32_t mode); /* DAT_14013d480 */
extern void (*g_pWIMCloseHandleSlot)();                                    /* DAT_14013d488 */
extern int (*g_pGetStorageDependencyInformation)(HANDLE h, int a2, uint32_t a3, void *a4,
                                                 uint64_t *a5); /* DAT_14013d3b0 */
extern int (*g_pGetSaveFileNameW)(void);                        /* DAT_14013d430 */

#endif /* PECMD_GLOBALS_H */