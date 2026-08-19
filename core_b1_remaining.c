/* B1 剩余业务函数还原 (core_b1_remaining.c)
 * 本批已还原: PECMD_DelayLoadOleaut32, FUN_14001a6fc, FUN_14001a9fc, PECMD_EnumImDiskDrives,
 *             PECMD_DevLockUnlock, PECMD_ExecLoadCommand, PECMD_HandleDropFile, FUN_14001f164.
 * 其余函数仍为 NO-OP stub, 待后续批次还原。 */
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "pecmd_defs.h"

extern uint64_t PTR_FUN_14011eb78;   /* 对象虚表 */
extern uint64_t g_u64CA20;       /* 文件映射视图 */
extern void FUN_1400703E4(WCHAR **ps, LPCWSTR src);        /* @0x1400703e4 */
extern WCHAR *PECMD_AllocString(WCHAR **ps, int64_t count);    /* @0x140063720 */
extern bool PECMD_ParseUIntValue(WCHAR **pp, int *out);           /* @0x140074838 */
extern uint64_t PECMD_IsDirectory(LPCWSTR path);               /* @0x140101d34 */
extern int64_t PECMD_WideStrLen(const uint16_t *s);       /* @0x140103020 */
extern void FUN_1400633A8(void **ps, int64_t len);         /* @0x1400633a8 */
extern void FUN_1400E56E4(uint64_t *slot);                 /* @0x1400e56e4 */
extern bool PECMD_ParseNumSkipWs(WCHAR **pp, uint64_t *out);      /* @0x140067cf4 */
extern LARGE_INTEGER PECMD_SetFilePointer(HANDLE h, LARGE_INTEGER dist, DWORD method); /* @0x14005c674 */
extern BOOL GlobalMemoryStatusEx(void *buffer);            /* Win32 API */
extern void FUN_14005B104(WCHAR **ps);                     /* @0x14005b104 */
extern void FUN_1400E9138(uint64_t *obj);                  /* @0x1400e9138 */
extern uint64_t PTR_FUN_14011eb88;                         /* 对象虚表 */
extern uint64_t PTR_FUN_14011c410;                         /* 对象虚表 */
extern UINT WinExec(LPCSTR lpCmdLine, UINT uCmdShow);      /* Win32 API */
extern void FUN_14005C828(LPCSTR func, LPCSTR dll, void **out, HMODULE *hmod); /* @0x14005c828 */
extern int FUN_14005C788(const char *a, const WCHAR *w, int n); /* @0x14005c788 */
extern DWORD FUN_14005c5a0(HKEY root, LPCWSTR sub, LPCWSTR name, DWORD type,
                           BYTE *data, DWORD size);       /* @0x14005c5a0 */
extern DWORD FUN_14005C4E0(HKEY root, LPCWSTR subkey, LPCWSTR name, DWORD *type,
                           BYTE *data, DWORD *size);      /* @0x14005c4e0 */
extern DWORD FUN_14005c61c(HKEY root, LPCWSTR subkey, LPCWSTR name); /* @0x14005c61c 删注册表值 */
extern void FUN_1400E6D74(WCHAR *dst, uint64_t v);        /* @0x1400e6d74 */
extern void FUN_1400629B8(void *script, LPCWSTR key, LPCWSTR value); /* @0x1400629b8 */
extern WCHAR *FUN_1400702D4(WCHAR **out, LPCWSTR src, int64_t len); /* @0x1400702d4 */
extern LARGE_INTEGER FUN_14003C06C(int64_t *script, LARGE_INTEGER cmd, uint32_t flags); /* @0x14003c06c */
extern WCHAR *FUN_14006375C(WCHAR **ps, LPCWSTR src);     /* @0x14006375c */
extern LARGE_INTEGER FUN_14004c0bc(uint64_t script, LARGE_INTEGER cmd,
                                   void *p3, void *p4, void *p5); /* @0x14004c0bc */
extern void *g_pQueryServiceStatusEx;                                 /* QueryServiceStatusEx 槽 */
extern HANDLE RegisterDeviceNotificationW(HANDLE hRecipient, void *notificationFilter,
                                          DWORD flags);    /* Win32 API */
extern BOOL DeviceIoControl(HANDLE hDevice, DWORD dwIoControlCode, LPVOID lpInBuffer,
                            DWORD nInBufferSize, LPVOID lpOutBuffer, DWORD nOutBufferSize,
                            DWORD *lpBytesReturned, void *lpOverlapped); /* Win32 API */
extern uint64_t DAT_14013d050;                             /* mciSendStringW 延迟加载槽 */
extern uint64_t g_u64CA50;                             /* 释放回调 */
extern uint64_t g_u64C9E8;                             /* GetModuleFileNameExW 槽 */
extern uint64_t g_u64C9F0;                             /* EnumProcessModules 槽 */
extern HWND g_hwndCF78;                                 /* PELogon 回调窗口 */
extern HWND g_hwndTray;                             /* 托盘数据块 */
extern uint64_t g_u64CD00;                             /* 物理内存映射基址 */
extern uint64_t g_u64cd08;                             /* 物理内存映射句柄 */
extern void *PTR_u_CallBackhWnd_14011e668_2_14013a288;    /* 回调窗口名数据 */
extern void FUN_14005B900(char *data, uint64_t id, LPCWSTR tip,
                          HICON icon, uint32_t flags);    /* @0x14005b900 */
extern HWND g_hPelogonWnd;                                 /* PELogon 回调窗口句柄 */

/* ---- 本批(28)还原所需: SetupAPI CM_Locate/CM_Reenumerate (Win32, link stub) ---- */
extern int CM_Locate_DevNodeW(uint32_t *pdnDevInst, void *pDeviceID, uint32_t ulFlags); /* SetupAPI */
extern int CM_Reenumerate_DevNode(uint32_t dnDevInst, uint32_t ulFlags);                /* SetupAPI */

/* ---- 本批还原所需: 额外 helper extern ---- */
extern int lstrcmpA(LPCSTR, LPCSTR);                            /* Win32 ANSI 比较(未在 stub 头声明) */
extern void perror(const char *);                               /* CRT 错误输出(未在 stub 头声明) */
extern int wnsprintfW(LPWSTR, int, LPCWSTR, ...);               /* shlwapi 格式化(未在 stub 头声明) */
extern uint64_t PECMD_AllocSmallObject(uint64_t *arr);                   /* @0x140063344 小对象分配 */
extern uint64_t FUN_140082448(int64_t *pp, double *out);        /* @0x140082448 (定义于 core_b3_remaining.c) */
extern void *PECMD_GrowByteBuffer(void **ps, int64_t len);             /* @0x140063424 分配清零槽数组 */
extern void FUN_14005B0D4(void *ps);                            /* @0x14005b0d4 释放 */
extern void FUN_140063694(WCHAR **ps, int64_t count);           /* @0x140063694 */
extern void FUN_1400702B0(WCHAR **ps, LPCWSTR src);             /* @0x1400702b0 */
extern int64_t *PECMD_ReplaceStringSlot(int64_t *ps, uint64_t *src);      /* @0x140070398 */
extern int64_t PECMD_EncodeBuffer(int64_t *in, int64_t *out, uint8_t cp); /* @0x140068984 */
extern uint32_t PECMD_IsBitmapResource(uint16_t *s);                     /* @0x14005d87c core_b3d.c */
extern uint32_t PECMD_IsIconResource(uint16_t *s);                     /* @0x14005d83c core_b3d.c */
extern uint32_t FUN_14005D7E8(uint16_t *s);                     /* @0x14005d7e8 core_b3f.c */
extern HMODULE FUN_14001EA18(HMODULE h, LPCWSTR a2, LPCWSTR a3,
                             int64_t *pp, uint32_t *a5);       /* @0x14001ea18 */
extern WCHAR *FUN_14001BE14(WCHAR *s);                          /* @0x14001be14 去引号 */
extern WCHAR *PECMD_StrCopyW(WCHAR **ps, LPCWSTR src, int64_t len); /* @0x140063888 */
extern void FUN_14007BF44(int64_t *ctx, WCHAR *name, void *out,
                          int mode, uint8_t flag);             /* @0x14007bf44 */
extern void FUN_140101db8(HANDLE *ph, LPCWSTR path, WIN32_FIND_DATAW *fd); /* @0x140101db8 */
extern HANDLE PECMD_OpenFileHandle(HANDLE *out, LPCWSTR path, DWORD access, DWORD share,
                           LPSECURITY_ATTRIBUTES sa, DWORD disp, DWORD flags,
                           HANDLE tmpl);                     /* @0x140003864 CreateFileW 包装(exec2 定义, 返回句柄) */
extern uint32_t FUN_1400e693c(HANDLE hFile);                    /* @0x1400e693c GetFileSize */
extern uint64_t FUN_14005f33c(int64_t buf, int len);            /* @0x14005f33c 编码检测 */
extern uint32_t FUN_1400e7d58(int64_t *ps, uint32_t flag);      /* @0x1400e7d58 卸载/释放缓冲 */
extern LPWSTR FUN_14005fc90(LPWSTR dst, uint32_t *guid, int mode); /* @0x14005fc90 GUID->字符串 */
extern void FUN_140018d8c(uint64_t ctx, LPCWSTR fmt, uint64_t a, uint64_t b); /* @0x140018d8c 日志 */
extern int (*DAT_14013cef8)(LPCWSTR, GUID *, LPWSTR, DWORD, DWORD); /* SetupDiGetINFClassW 槽 */
extern BOOL LookupAccountSidW(LPCWSTR sys, void *sid, LPWSTR name, DWORD *cchName,
                              LPWSTR dom, DWORD *cchDom, int *peUse); /* Win32 API */
extern uint64_t FUN_1400688E0(LPCWSTR path);                    /* @0x1400688e0 core_b3_remaining.c */
extern HICON FUN_140061E98(void *buf, uint64_t size, void *p3); /* @0x140061e98 待还原(见core_b3_remaining.c) */
extern HICON FUN_140073934(HMODULE h, LPCWSTR name, int a, int b,
                           int c, uint32_t *flags);            /* @0x140073934 待还原(见core_b3_remaining.c) */

/* ---- 本批(B1 剩余 8 个)还原所需: 额外 helper extern ---- */
extern HKEY FUN_1400C13F8(HKEY a1, HKEY a2, char a3);                 /* @0x1400c13f8 脚本注册表写 */
extern bool FUN_140101E70(LPCWSTR path);                              /* @0x140101e70 路径存在判断 */
extern int64_t *FUN_1400702F0(int64_t *out, const char *src, uint64_t len); /* @0x1400702f0 串复制(core_exec5.c) */
extern uint64_t FUN_14000e26c(uint64_t script, uint64_t cmd, uint64_t s3,
                              uint64_t s4, uint32_t flag, void *p6,
                              uint64_t s7, void *p8);                 /* @0x14000e26c 脚本执行 */
extern void FUN_140006554(void *hDrop, int64_t *a, int64_t *b);       /* @0x140006554 取拖放路径 */
extern void FUN_140103a20(void);                                      /* @0x140103a20 */
extern int wcscmp(const WCHAR *a, const WCHAR *b);                    /* 宽串比较 */
extern WCHAR g_szEmpty[];                                             /* g_szEmpty 空串 */
extern void *g_pImDiskCtrl;     /* ImDisk 控制函数指针槽 (延迟加载) */
extern void *g_pImDiskMask;     /* 返回 ImDisk 位掩码的函数指针槽 */
extern void *g_pDevOpen;     /* CreateFile 类函数指针槽 (设备打开) */
extern void *g_pDevOpen2;     /* CreateFile 类函数指针槽 (设备打开2) */
extern void *g_pSysAllocString;     /* SysAllocString 槽 */
extern void *g_pSysFreeString;     /* SysFreeString 槽 */
extern HMODULE g_hOleaut32;   /* Oleaut32 模块槽 */
extern void *Ordinal_418_exref; /* OleLoadPicture 槽 (exref) */

/* ---- PECMD_SetDesktopWallpaper / PECMD_ParseEnvSwitches 还原所需: 额外 helper extern ---- */
extern WCHAR *FUN_14005B154(WCHAR **ps);                       /* @0x14005b154 跳过空白 */
extern WCHAR *FUN_140063620(WCHAR **out);                      /* @0x140063620 初始化串缓冲 */
extern int64_t FUN_14007a224(int64_t *ctx, WCHAR *src, WCHAR **out,
                             int mode, uint8_t flag);          /* @0x14007a224 */
extern short FUN_1400677b0(WCHAR **pp, int64_t val);           /* @0x1400677b0 */
extern uint32_t PECMD_TokenizeList(int64_t *ctx, short *src, int mode); /* @0x14007403c */
extern void PECMD_SetClipboardUnicode(LPCWSTR s);                          /* @0x140060718 */
extern WCHAR *FUN_140078E90(WCHAR **ps);                       /* @0x140078e90 */
extern char FUN_1400660ac(const char *tok, WCHAR **pp, int n); /* @0x1400660ac */
extern void FUN_1400668ec(int64_t *ctx, uint64_t fn, LPCWSTR a,
                          LPCWSTR fmt);                        /* @0x1400668ec */
extern void FUN_1400a9a84(WCHAR **pp, uint64_t *out);          /* @0x1400a9a84 */
extern short *FUN_1400547bc(int64_t *ctx, WCHAR **pp, WCHAR **out,
                            short c, short f);                 /* @0x1400547bc */
extern uint64_t FUN_140001188(void);                           /* @0x140001188 */
extern bool FUN_1400c1194(WCHAR **pp, uint64_t *size);         /* @0x1400c1194 */
extern bool FUN_1400c11c0(WCHAR **pp, int *out);               /* @0x1400c11c0 */
extern uint64_t FUN_1400745c8(WCHAR **pp, uint64_t *out);      /* @0x1400745c8 */
extern WCHAR *FUN_1400f429c(WCHAR **pp, uint16_t ch);          /* @0x1400f429c */
extern void FUN_140061C44(void);                               /* @0x140061c44 */
extern void FUN_140025f10(int64_t ctx, LPCWSTR fmt, uint32_t a,
                          void *p4, void *p5, int64_t *p6);    /* @0x140025f10 */
extern BOOL SystemParametersInfoW(UINT uiAction, UINT uiParam, void *pvParam,
                                  UINT fWinIni);               /* Win32 API */
extern LONG LoadEnvi(LPCWSTR a, short *b);                     /* @0x1400e3804 */

/* ---- 本批还原所需: 额外全局 extern ---- */

/* ---- 本批(14个)还原所需: helper / 全局 / Win32 API extern ---- */
extern int64_t FUN_14001c2cc(LPCWSTR priv, DWORD attr, uint32_t flag); /* @0x14001c2cc EnablePrivilege */
extern WCHAR *FUN_1400637dc(WCHAR **ps, LPCWSTR src, int64_t a, int64_t b); /* @0x1400637dc */
extern void FUN_14003ed4c(void *script, LPCWSTR cmd);                     /* @0x14003ed4c */
extern void FUN_14003cd0c(void *script, void *p2);                        /* @0x14003cd0c */
extern uint64_t *PECMD_ServiceControl(void *script, LPCWSTR cmd);                /* @0x140020018 */
extern void FUN_140102a90(void *buf, int val, uint64_t count);            /* @0x140102a90 buffer zero-fill 辅助 */
extern void FUN_140067b78(int64_t *ps, uint64_t *out);                    /* @0x140067b78 数字解析 */
extern uint64_t PECMD_GetParentProcessId(DWORD pid);                                 /* @0x140006988 */
extern uint64_t PECMD_RunCommand(void *p1, void *p2);                        /* @0x140031454 */
extern uint64_t PECMD_ParseDateTimeSpec(void *p1, void *p2, int p3, void *p4);      /* @0x1400408d0 */
extern void *FUN_14007de70(void *a, void *b, LPCWSTR c);                  /* @0x14007de70 */
extern void FUN_14007bf44(int64_t *param_1, WCHAR *param_2, void *param_3, int param_4,
                          uint8_t param_5);                                /* @0x14007bf44 命令串解析 */
/* 全局: 标准句柄 / 文件版本 / 进程创建 / 命令表 */
extern HANDLE g_hStdIn;           /* GetStdHandle(STD_INPUT_HANDLE) */
extern HANDLE g_hStdOut;           /* GetStdHandle(STD_OUTPUT_HANDLE) */
extern HANDLE g_hStdErr;           /* GetStdHandle(STD_ERROR_HANDLE) */
extern uint32_t g_exitCodeCache;         /* 退出码值缓存 */
extern void *g_pGetFileVersionInfoExW;            /* GetFileVersionInfoExW 槽 */
extern void *g_pGetFileVersionInfoSizeExW;            /* GetFileVersionInfoSizeExW 槽 */
extern void *g_pMemQuery;            /* 内存查询函数槽 */
extern void *g_pProcMemRead;            /* 进程内存读取槽 */
extern void *g_pShutdownFlag;            /* 系统关闭标志 */
extern void *g_pPendingObj;            /* 待处理对象 */
extern int g_cmdTable1Count;              /* 命令表1计数 */
extern void *g_cmdTable1;            /* 命令表1 */
extern int g_cmdTable2Count;              /* 命令表2计数 */
extern void *g_cmdTable2;            /* 命令表2 */
extern int g_cmdTable3Count;              /* 命令表3计数 */
extern void *g_cmdTable3;            /* 命令表3 */
extern int g_cmdTable4Count;              /* 命令表4计数 */
extern void *g_cmdTable4;            /* 命令表4 */
extern int g_cmdTable5Len;              /* 命令表5长度(字节) */
extern void *g_cmdTable5;            /* 命令表5 */
extern uint64_t PTR_FUN_14011cbe8;     /* 对象虚表 (PECMD_DispatchBuiltin) */
/* Win32 API (未在 win32_stub.h 声明) */
extern UINT SetErrorMode(UINT uMode);
extern DWORD TlsAlloc(void);
extern DWORD GetFileVersionInfoSizeW(LPCWSTR lptstrFilename, DWORD *lpdwHandle);
extern BOOL GetFileVersionInfoW(LPCWSTR lptstrFilename, DWORD dwHandle, DWORD dwLen, LPVOID lpData);
extern BOOL VerQueryValueW(LPCVOID pBlock, LPCWSTR lpSubBlock, void **lplpBuffer, UINT *puLen);
extern BOOL SetupDiClassGuidsFromNameW(LPCWSTR ClassName, GUID *ClassGuidList,
                                       DWORD ClassGuidListSize, DWORD *RequiredSize);
/* 本文件局部定义, 但定义晚于调用处, 需前置声明 */
extern void FUN_140005148(void);                     /* @0x140005148 */
extern bool FUN_140018534(uint64_t *param_1);        /* @0x140018534 */

/* ---- 本批(B1 剩余 9 个)还原所需: 额外 helper / 全局 extern (核对自 decompiled.c) ---- */
extern DWORD g_dwC96C;                                    /* 线程 ID 输出 */
extern int (*DAT_14013c990)(HANDLE, DWORD, LPCWSTR, DWORD, LPCWSTR, DWORD, DWORD,
                            DWORD, DWORD *, BOOL);             /* WTSSendMessageW 槽 */
extern HWND FUN_1400e3d60(LPCWSTR text, DWORD a, DWORD b, HWND c, DWORD d,
                          DWORD e, DWORD f, DWORD g);          /* @0x1400e3d60 消息框 */
extern uint64_t FUN_14002d708(WCHAR *s, int a, int64_t *p, int b, DWORD d); /* @0x14002d708 */
extern HANDLE FUN_14005b228(int64_t (*cb)(void *), LPVOID arg, uint64_t stack,
                            uint64_t flags, DWORD *tid, LPSECURITY_ATTRIBUTES sa); /* @0x14005b228 线程创建 */

extern int (*g_pCoCreateInstance)(void *, void *, uint32_t, void *, void **); /* CoCreateInstance 槽 */
extern GUID g_guid2d168;           /* 壁纸 CLSID */
extern GUID g_guid2d178;           /* 壁纸 IID */
extern uint8_t g_wallpaperMode;            /* 壁纸模式标志 */
extern uint32_t g_u32A22C;
extern uint8_t g_flagCCC9;
extern uint8_t g_popmenuFlag;
extern uint8_t g_flagCB08;
extern uint8_t g_flagCCB3;
extern uint64_t g_u64c9d0[4];
extern void *PTR_WndProc1_14013a008[4];
extern void *PTR_FUN_14013a020[4];
extern uint8_t g_u8A24A;
extern uint8_t DAT_140147001[];
extern int64_t *g_pVolHandles;       /* 卷句柄槽数组 */
extern void *g_pNtOpenSymLink;          /* NtOpenSymbolicLinkObject 槽 */
extern void *g_pNtQuerySymLink;          /* NtQuerySymbolicLinkObject 槽 */
extern void *DAT_14013cf00;          /* SetupDiGetClassDevsW 槽 */
extern void *DAT_14013cf08;          /* SetupDiGetDeviceRegistryPropertyW 槽 */
extern void *DAT_14013cf10;          /* SetupDiDestroyDeviceInfoList 槽 */
extern void *g_pSetupDiSetClassInstallParamsW;          /* SetupDiSetClassInstallParamsW 槽 */
extern void *DAT_14013cf20;          /* SetupDiCallClassInstaller 槽 */
extern void *DAT_14013cf28;          /* SetupDiEnumDeviceInfo 槽 */
extern void *DAT_14013cf30;          /* SetupIterateCabinetW 槽 */
extern void *g_pInstallHinfSectionW;          /* InstallHinfSectionW 槽 */
extern void *g_pInstallHinfSection;          /* InstallHinfSection 槽 */
extern void *DAT_14013cf48;          /* CM_Get_DevNode_Status 槽 */
extern HFONT g_hFontFast;          /* 快速字体缓冲区(前台窗口文本) */
extern WCHAR *g_pwszD258;         /* 前台窗口文本/状态提示串 */
extern uint8_t g_tipAlign;        /* 提示串对齐标志 */
extern int g_tipOffX;            /* 提示区偏移X */
extern int g_tipOffY;            /* 提示区偏移Y */
extern int g_intA238;            /* 提示区 left */
extern int g_intA23C;            /* 提示区 top */
extern int g_intA240;            /* 提示区 right */
extern int g_intA244;            /* 提示区 bottom */
extern COLORREF g_dwTipsTextColor;       /* 提示文本颜色 */
extern HMODULE g_hGdiPlus;        /* GdiPlus.dll 句柄 */
extern void *g_gdiplusToken;          /* Gdiplus token */

/* GDI+ 延迟加载函数指针槽 (均为 GetProcAddress 所得) */
extern HBITMAP LoadBitmapW(HINSTANCE, LPCWSTR);              /* Win32(未在 stub 头声明) */
extern void *g_pGdiplusStartup;          /* GdiplusStartup */
extern void *g_pGdiplusShutdown;          /* GdiplusShutdown */
extern void *g_pGdipLoadImageFromFile;          /* GdipLoadImageFromFile */
extern int (*g_pGdipDisposeImage)();          /* GdipDisposeImage */
extern int (*g_pGdipCreateFromHDC)();          /* GdipCreateFromHDC */
extern void *DAT_14013cd98;          /* GdipDrawImageRectI */
extern void *DAT_14013cde8;          /* GdipCreateBitmapFromHBITMAP */
extern void *DAT_14013cdf0;          /* GdipCreateHBITMAPFromBitmap */
extern void *g_pGdipSaveImageToFile;          /* GdipSaveImageToFile */
extern void *DAT_14013cdb8;          /* GdipGetImageEncodersSize */
extern void *DAT_14013cdc0;          /* GdipGetImageEncoders */
extern void *g_pGdipDrawImageI;          /* GdipDrawImageI */
extern int (*g_pGdipGetImageWidth)();          /* GdipGetImageWidth */
extern int (*g_pGdipGetImageHeight)();          /* GdipGetImageHeight */
extern void *g_pGdipDrawImageRectRectI;          /* GdipDrawImageRectRectI */
extern int (*g_pGdipDeleteGraphics)();          /* GdipDeleteGraphics */
extern void *DAT_14013ce38;          /* GdipSetInterpolationMode */
extern void *g_pGdipSetPixelOffsetMode;          /* GdipSetPixelOffsetMode */
extern void *g_pGdipSetSmoothingMode;          /* GdipSetSmoothingMode */
extern void *g_pGdipLoadImageFromStream;          /* GdipLoadImageFromStream */
extern void *g_pGdipGetImageType;          /* GdipGetImageType */
extern void *DAT_14013ce70;          /* GdipCreateBitmapFromHICON */
extern void *g_pGdipGetImageDimension;          /* GdipGetImageDimension */
extern void *DAT_14013ce60;          /* GdipCreateHICONFromBitmap */
extern void *g_pGdipSetTextRenderingHint;          /* GdipSetTextRenderingHint */
extern void *g_pGdipImageGetFrameDimensionsList;          /* GdipImageGetFrameDimensionsList */
extern void *g_pGdipImageSelectActiveFrame;          /* GdipImageSelectActiveFrame */
extern void *g_pGdipImageGetFrameDimensionsCount;          /* GdipImageGetFrameDimensionsCount */
extern void *g_pGdipImageGetFrameCount;          /* GdipImageGetFrameCount */
extern void *g_pGdipGetPropertyItemSize;          /* GdipGetPropertyItemSize */
extern void *g_pGdipGetPropertyItem;          /* GdipGetPropertyItem */
extern void *g_pGdipBitmapLockBits;          /* GdipBitmapLockBits */
extern void *g_pGdipBitmapUnlockBits;          /* GdipBitmapUnlockBits */
extern void *DAT_14013ce90;          /* GdipCreateBitmapFromScan0 */
extern void *g_pGdipCreateBitmapFromResource;          /* GdipCreateBitmapFromResource */
extern void *DAT_14013cea0;          /* GdipCloneBitmapAreaI */
extern void *DAT_14013cea8;          /* GdipGetImageGraphicsContext */
extern void *g_pGdipScaleWorldTransform;          /* GdipScaleWorldTransform */
extern void *g_pGdipCreateTexture;          /* GdipCreateTexture */
extern void *g_pGdipDeleteBrush;          /* GdipDeleteBrush */
extern void *g_pGdipFillRectangle;          /* GdipFillRectangle */
extern void *g_pGdipDrawString;          /* GdipDrawString */
extern void *DAT_14013ced0;          /* GdipNewPrivateFontCollection */
extern void *DAT_14013ced8;          /* GdipDeletePrivateFontCollection */
extern void *DAT_14013cee0;          /* GdipPrivateAddMemoryFont */
extern void *DAT_14013cee8;          /* GdipGetFontCollectionFamilyList */
extern void *DAT_14013cef0;          /* GdipGetFamilyName */

/* ---- B1 剩余 15 个业务函数还原所需: 额外 helper extern ---- */
extern void FUN_1400e6860(WPARAM param_1, int param_2);          /* @0x1400e6860 */
extern uint64_t FUN_1400e89fc(HDC param_1, uint64_t param_2, HDC param_3,
                              int64_t *param_4);                 /* @0x1400e89fc */
extern uint64_t FUN_140004e34(int param_1, int64_t param_2);     /* @0x140004e34 */
extern uint64_t FUN_140004fd4(LPCWSTR param_1);      /* @0x140004fd4 (void in decompile; caller uses eax residue) */
extern int64_t PECMD_ExecuteCommand(int64_t *param_1, LPCWSTR param_2, int64_t param_3,
                             LPCWSTR param_4, uint32_t param_5, int64_t param_6,
                             int param_7);                       /* @0x1400a4dcc */
extern void FUN_1400056bc(void);                                 /* @0x1400056bc */
extern void FUN_140008d9c(uint16_t *param_1);                    /* @0x140008d9c */
extern DWORD FUN_14000a584(LPCWSTR param_1, int param_2);        /* @0x14000a584 */
extern void FUN_1400185c8(char param_1, int64_t *param_2);       /* @0x1400185c8 */
extern void *g_pFreeLibrary;                                      /* FreeLibrary 槽 */
extern void *PTR_DAT_14013a040;                                  /* 主窗口指针槽 */
extern void *g_pSvStatus;                                      /* service 状态句柄 */
extern HHOOK g_hMsgHook;                                      /* hook 句柄(KEYHOOK) */
/* 数据/标量全局 */
extern uint32_t g_msgRetCode;
extern int64_t  g_i64CCB8;
extern uint8_t  g_svcStatus[128];  /* SERVICE_STATUS 缓冲 */

extern void SwitchToFiber(void *lpFiber);                        /* Win32 API */
/* win32_stub.h 中 FindFirstVolumeW/FindNextVolumeW 原型参数个数错误(多一个实参),
   这里用正确签名的函数指针间接调用以修正(见 PECMD_OpenLockVolume)。 */
typedef HANDLE (*FFVW_t)(LPWSTR, DWORD);
typedef BOOL   (*FNFVW_t)(HANDLE, LPWSTR, DWORD);

/* ---- B1 剩余 18 函数还原(B2)所需: 额外 helper/全局 extern ---- */
typedef struct tagOSVERSIONINFOW {
    DWORD   dwOSVersionInfoSize;
    DWORD   dwMajorVersion;
    DWORD   dwMinorVersion;
    DWORD   dwBuildNumber;
    DWORD   dwPlatformId;
    WCHAR   szCSDVersion[128];
} OSVERSIONINFOW;

typedef struct tagPROCESSENTRY32W {
    DWORD   dwSize;
    DWORD   cntUsage;
    DWORD   th32ProcessID;
    uintptr_t th32DefaultHeapID;
    DWORD   th32ModuleID;
    DWORD   cntThreads;
    DWORD   th32ParentProcessID;
    LONG    pcPriClassBase;
    DWORD   dwFlags;
    WCHAR   szExeFile[260];
} PROCESSENTRY32W;

/* 令牌/权限结构 (win32_stub.h 未声明, 用于 PECMD_CreateProcessAsUser) */
typedef struct tagLUID {
    DWORD LowPart;
    LONG  HighPart;
} LUID, _LUID;
typedef struct tagLUID_AND_ATTRIBUTES {
    LUID  Luid;
    DWORD Attributes;
} LUID_AND_ATTRIBUTES;
typedef struct tagTOKEN_PRIVILEGES {
    DWORD PrivilegeCount;
    LUID_AND_ATTRIBUTES Privileges[1];
} TOKEN_PRIVILEGES, _TOKEN_PRIVILEGES;

/* Win32 API extern (win32_stub.h 未声明) */
extern DWORD GetVersion(void);
extern BOOL  GetVersionExW(OSVERSIONINFOW *);
extern BOOL  GetProcessAffinityMask(HANDLE, DWORD_PTR *, DWORD_PTR *);
extern BOOL  TlsSetValue(DWORD, void *);
extern HANDLE CreateToolhelp32Snapshot(DWORD, DWORD);
extern BOOL  Process32FirstW(HANDLE, PROCESSENTRY32W *);
extern BOOL  Process32NextW(HANDLE, PROCESSENTRY32W *);

/* helper extern */
extern int  FUN_1400630D0(int mode);                          /* @0x1400630d0 分配失败提示 */
extern uint8_t *FUN_14001d78c(uint8_t *dst, const uint8_t *src, int len); /* @0x14001d78c memcpy */
extern uint64_t *FUN_1400E9048(uint64_t *obj, uint32_t hwnd, uint64_t data); /* @0x1400e9048 窗口对象 G 初始化 */
extern void FUN_1400171a4(HANDLE param_1);                        /* @0x1400171a4 (SKIP; 需 link_stubs 桩) */
extern uint64_t PECMD_GetPackedSystemVersion(void);                              /* @0x14005ea5c 系统版本 */

/* ---- B1 剩余 10 个业务函数还原所需: 额外 helper/全局 extern ---- */
extern int64_t PECMD_BuildDosDevicePath(uint16_t param_1, int64_t *param_2);         /* @0x140060000 盘符->卷名 */
extern int     PECMD_FindVolumeByDeviceId(uint32_t *param_1, int64_t *param_2, LPWSTR param_3); /* @0x14008b820 */
extern int64_t PECMD_EnumeratePartitions(int64_t param_1, int64_t *param_2, uint32_t *param_3); /* @0x14008b340 */
extern int64_t PECMD_ParseVolumeGuid(int64_t *param_1, uint32_t *param_2, int param_3);     /* @0x1400780fc 卷 GUID 解析 */
extern int64_t *FUN_14007034c(int64_t *param_1, LPCWSTR param_2);        /* @0x14007034c */
extern void FUN_1400e6d68(LPCWSTR param_1, uint64_t param_2);             /* @0x1400e6d68 */
extern DWORD PECMD_EnumDevices(LPCWSTR param_1, LPWSTR param_2, uint32_t param_3, GUID *param_4); /* @0x140076554 */
extern LPWSTR FUN_140077190(LPWSTR param_1, int64_t param_2, int64_t param_3, int *param_4,
                            LPWSTR param_5, uint32_t param_6, LPCWSTR param_7); /* @0x140077190 */
extern int     FUN_1400690c0(HKEY param_1, LPCWSTR param_2, LPCWSTR param_3, int64_t *param_4,
                             DWORD *param_5, void *param_6);               /* @0x1400690c0 */
extern uint32_t FUN_14008ba90(uint32_t *param_1, int64_t param_2, uint16_t param_3, LPCWSTR param_4); /* @0x14008ba90 */
extern uint32_t FUN_140006a4c(LPCWSTR param_1);                            /* @0x140006a4c */
extern int64_t FUN_14001d810(uint64_t param_1, uint64_t param_2, uint64_t param_3); /* @0x14001d810 */
extern int     FUN_14005b184(char *param_1, int64_t param_2, int64_t param_3);       /* @0x14005b184 */
extern uint64_t FUN_1400e3cd4(LPCWSTR param_1, uint64_t *param_2, int64_t *param_3); /* @0x1400e3cd4 */
extern uint16_t *FUN_140024c48(int64_t *param_1, int64_t *param_2, uint32_t param_3); /* @0x140024c48 */
extern void FUN_14000397c(void);                                              /* @0x14000397c */
extern void FUN_14005e7dc(uint64_t *param_1);                                 /* @0x14005e7dc */
extern HANDLE FUN_1400060b8(HANDLE param_1);                                  /* @0x1400060b8 */
extern uint8_t *FUN_14001d744(uint8_t *dst, uint8_t *src, int len);       /* @0x14001d744 重叠搬移 (本文件定义) */
extern int64_t PECMD_EnumNtSymbolicLink(LPWSTR param_1, int64_t *param_2, int64_t *param_3,
                             int64_t *param_4);                            /* @0x14001d8c8 (本文件定义) */
extern uint64_t FUN_1400048c4(int64_t *param_1);                              /* @0x1400048c4 取串长度 */
extern void *operator_new(uint64_t n);                                        /* C++ new (link_stubs.c) */
extern uint64_t FUN_14005e04c(void);                                          /* @0x14005e04c */
extern uint64_t FUN_1400195f0(uint64_t param_1, int64_t param_2, int param_3,
                              uint64_t *param_4);                             /* @0x1400195f0 日志/重试提示 */
extern DWORD SearchPathW(LPCWSTR lpPath, LPCWSTR lpFileName, LPCWSTR lpExtension,
                         DWORD nBufferLength, LPWSTR lpBuffer, LPWSTR *lpFilePart); /* Win32 API */
extern BOOL DuplicateTokenEx(HANDLE, DWORD, LPSECURITY_ATTRIBUTES, int, int, HANDLE *); /* Win32 API */
extern BOOL SetTokenInformation(HANDLE, int, void *, DWORD);                    /* Win32 API */
extern BOOL CreateProcessWithLogonW(LPCWSTR, LPCWSTR, LPCWSTR, DWORD, LPCWSTR, LPWSTR,
                                    DWORD, LPVOID, LPCWSTR, LPSTARTUPINFOW, LPPROCESS_INFORMATION); /* Win32 API */
extern GUID DAT_14011e890;                                          /* @0x14011e890 (GUID 数据) */
/* ImDisk.cpl 动态装载的函数指针槽 (PECMD_LoadUnloadImdisk) */
extern HMODULE g_hImdiskCpl;                                       /* Imdisk.cpl 模块句柄 */
/* 会话/进程创建用延迟加载函数指针槽 (PECMD_CreateProcessAsUser) */
extern int (*DAT_14013c998)(void);                                  /* WTSGetActiveConsoleSessionId 槽 */
extern BOOL (*DAT_14013c988)(DWORD, HANDLE *);                      /* WTSQueryUserToken 槽 */
extern BOOL (*DAT_14013c9a0)(void **, HANDLE, BOOL);                /* CreateEnvironmentBlock 槽 */
extern void (*DAT_14013c9a8)(void);                                 /* DestroyEnvironmentBlock 槽 */

/* 全局(槽/数据) extern */
extern uint8_t  g_u8CA49;      /* CPU 计数 */
extern int      g_ramdrivFlag;      /* Ramdriv 标志 */
extern int      g_readBufSize;      /* 读取缓冲大小 */
extern DWORD    g_dwC934;      /* Tls 索引 */
extern uint32_t DAT_14013cb90[4];   /* OSVERSIONINFOW 于 14013cb90 */
extern uint8_t  GetCommandLineW_exref[]; /* GetCommandLineW 导入引用 */

/* ---- 本批(B1 剩余 10 个)还原所需: 额外 helper / 全局 extern ---- */
extern void     PECMD_InitEnvironmentVars(HINSTANCE hInst, int show);      /* @0x140027690 core_init.c */
extern void     FUN_1400084d0(short *param_1);                 /* @0x1400084d0 */
extern int64_t  FUN_14005c72c(char *a, const WCHAR *w, int n); /* @0x14005c72c 前缀比较 */
extern int64_t  FUN_140045c90(int64_t *param_1, void *param_2);/* @0x140045c90 */
extern uint64_t FUN_14002ca30(void);                           /* @0x14002ca30 */
extern uint64_t FUN_14005b7dc(void);                           /* @0x14005b7dc */
extern void     FUN_14005B21C(UINT code);                      /* @0x14005b21c 退出进程 */
extern void     FUN_14004eaa8(int64_t script, int mode);       /* @0x14004eaa8 */
extern void     PECMD_NormalizeQuoteChars(int16_t *param_1, int param_2);  /* @0x14005b540 引号/括号归一 */
extern LPCWSTR  FUN_14005182c(int64_t *param_1, LPCWSTR param_2, WCHAR *param_3,
                              WCHAR *param_4, WCHAR *param_5, int64_t *param_6); /* @0x14005182c */
extern void     PECMD_CreateHelperThread(uint8_t *param_1);               /* @0x140004f38 (本文件定义) */
extern void     FUN_1400051b4(void);                           /* @0x1400051b4 (本文件定义) */
extern int64_t  PECMD_DelayLoadOleaut32(void);                           /* @0x1400038fc (本文件定义) */
extern void     PECMD_RegisterDeviceNotify(HANDLE hwnd);                    /* @0x14001a918 (本文件定义) */
extern void     FUN_140017b8c(void);                           /* @0x140017b8c (本文件定义) */

extern uint8_t  g_Env[];             /* DAT_14013d1a0 环境表 (core_globals.c) */
extern uint8_t  g_state138;          /* DAT_14013d138 全局状态 */
extern uint32_t g_state190;          /* DAT_14013d190 */
extern uint32_t g_state198;          /* DAT_14013d198 */
extern uint8_t  g_flag169;           /* DAT_14013c969 */
extern uint32_t g_afterMain;         /* DAT_14013ccc8 收尾标志 */
extern uint8_t  g_b13caf8[];     /* 收尾/EXEC 标志位表 */
extern uint8_t  g_runMode;       /* 运行方式标志 */
extern uint64_t g_u64CB60;       /* SetupDi API 参数缓冲槽 */
extern void    *g_pZwMapViewOfSection;       /* ZwMapViewOfSection 槽 */
extern void    *g_pZwUnmapViewOfSection;       /* ZwUnmapViewOfSection 槽 */
extern uint64_t g_u64e770;       /* SetupDiGetClassDevsW 类 GUID (rdata) */

/* 委托 COM 接口 vtable 调用辅助 (obj 为 int64_t* 指向 vtable, off 为方法字节偏移) */
#define PECMD_VT_CALL(obj, off, fntype, ...) \
    ((fntype)(uintptr_t)*(int64_t *)((char *)(uintptr_t)*(obj) + (off)))(__VA_ARGS__)


uint64_t *PECMD_CreateWindowObject(uint64_t *param_1, uint64_t param_2, uint32_t param_3)
{
    /* @0x140001000 size=380 */
    HICON pHVar1;

    FUN_1400E9048(param_1, param_3, param_2);
    *param_1 = (uint64_t)(uintptr_t)PTR_FUN_14011c410;
    FUN_140063620((WCHAR **)(param_1 + 0x5f));
    *(uint32_t *)((uint8_t *)param_1 + 0x32c) = 0;
    *(uint32_t *)(param_1 + 0x66) = 0;
    *(uint32_t *)((uint8_t *)param_1 + 0x334) = 0;
    *(uint32_t *)(param_1 + 0x67) = 0;
    *(uint8_t *)(param_1 + 0x79) = 0;
    *(uint32_t *)(param_1 + 0x2c) = 0;
    *(uint16_t *)((uint8_t *)param_1 + 0x164) = 0;
    *(uint32_t *)((uint8_t *)param_1 + 0x2f4) = 0;
    param_1[0x60] = 0;
    *(uint8_t *)(param_1 + 0x61) = 0;
    param_1[0x62] = 0xb;
    *(uint32_t *)(param_1 + 99) = 10;
    *(uint32_t *)((uint8_t *)param_1 + 0x31c) = 0;
    *(uint32_t *)(param_1 + 100) = 0;
    param_1[0x1b] = 0;
    *(uint32_t *)((uint8_t *)param_1 + 0x324) = 0;
    *(uint32_t *)(param_1 + 0x65) = 0;
    *(uint32_t *)((uint8_t *)param_1 + 0x394) = 0;
    *(uint32_t *)(param_1 + 0x72) = 0;
    *(uint32_t *)((uint8_t *)param_1 + 0x34c) = 0;
    *(uint32_t *)(param_1 + 0x69) = 0;
    *(uint32_t *)((uint8_t *)param_1 + 0x344) = 0;
    *(uint32_t *)(param_1 + 0x68) = 0;
    *(uint32_t *)((uint8_t *)param_1 + 0x33c) = 0;
    *(uint32_t *)((uint8_t *)param_1 + 0x354) = 0;
    *(uint32_t *)(param_1 + 0x6a) = 0;
    memset(param_1 + 0x6b, 0, 0x24);
    *(uint16_t *)((uint8_t *)param_1 + 0x37c) = 0;
    *(uint32_t *)((uint8_t *)param_1 + 900) = 0;
    *(uint32_t *)(param_1 + 0x70) = 0;
    *(uint32_t *)((uint8_t *)param_1 + 0x38c) = 0;
    *(uint32_t *)(param_1 + 0x71) = 0;
    *(uint32_t *)((uint8_t *)param_1 + 0x39c) = 0;
    *(uint32_t *)(param_1 + 0x73) = 0;
    *(uint32_t *)(param_1 + 0x74) = 0;
    param_1[0x75] = 0;
    pHVar1 = LoadIconW(g_hInst, (LPCWSTR)(uintptr_t)0x1);
    param_1[0x76] = 0;
    param_1[0x77] = 0;
    param_1[0x75] = (uint64_t)(uintptr_t)pHVar1;
    param_1[0x78] = 0;
    param_1[0x7a] = 0;
    param_1[0x7b] = 0;
    param_1[0x7c] = 0;
    *(uint8_t *)(param_1 + 0x7d) = 0;
    return param_1;
}

void PECMD_ScriptDtor(uint64_t *obj)
{
    /* @0x14000118c size=70 释放模块句柄、串容器并调用基类析构 */
    *obj = (uint64_t)(uintptr_t)PTR_FUN_14011c410;
    if ((HMODULE)obj[0x78] != (HMODULE)0) {
        FreeLibrary((HMODULE)obj[0x78]);
    }
    obj[0x78] = 0;
    FUN_14005B104((WCHAR **)(obj + 0x5f));
    FUN_1400E9138(obj);
}

uint64_t PECMD_WndProcDispatch(HDC param_1, uint32_t param_2, HDC param_3, int64_t *param_4)
{
    /* @0x140003184 size=350 WndProc 消息分发 */
    uint64_t uVar1;
    uint8_t *p = (uint8_t *)(uintptr_t)param_1;

    if ((g_flagA24F < 0) && ((char)p[0xfa] != 'Z')) {
        p[0xfa] = 0x5a;
        PostMessageW(*(HWND *)(p + 8), 0x10, 0, 0);
    }
    if (param_2 != 0x10) {
        if (param_2 == 0x111) {
            if ((short)param_3 == 9) {
                param_3 = (HDC)(uint64_t)(uint32_t)(int)(int8_t)p[0xc2];
            }
            if (7 < (short)param_3) {
                return 0;
            }
            if (p[200] == 0) {
                p[200] = (uint8_t)(int)(short)param_3;
            }
            KillTimer(*(HWND *)(p + 8), 1);
            p[0xc9] = 0;
            FUN_1400e6860((WPARAM)param_1, 2);
            return 1;
        }
        if (param_2 == 0x400) {
            return 0x25e;
        }
        if (param_2 != 0x43c) {
            if (param_2 == 0x462) {
                return 0;
            }
            uVar1 = FUN_1400e89fc(param_1, (uint64_t)param_2, param_3, param_4);
            return uVar1;
        }
    }
    if (p[200] == 0) {
        p[200] = 2;
    }
    KillTimer(*(HWND *)(p + 8), 1);
    p[0xc9] = 0;
    if (*(int64_t *)(p + 0x54) != 0) {
        *(int64_t *)(p + 0x54) = 0;
        SendMessageW((HWND)0x0, 0x466, 2, *(LPARAM *)(p + 8));
    }
    FUN_1400e6860((WPARAM)param_1, 2);
    return 0;
}

/* 本批(B1 4 函数)还原 helper extern */
extern uint64_t PECMD_CheckFileAttr(LPCWSTR path);                /* @0x140101d74 定义于 core_b9_remaining.c */
extern void FUN_14007724c(LPWSTR param_1);                  /* @0x14007724c 自删除/延迟执行 */
extern char g_minintFlag;                                  /* MININT 检测标志 */
extern uint64_t g_u64d188;                              /* COM CLSID 槽 */
extern uint64_t g_u64d198;                              /* COM IID 槽 */
extern uint64_t PECMD_GetPackedSystemVersion(void);                        /* @0x14005ea5c 系统版本探测 */
extern uint16_t *FUN_14005b154(uint16_t **ps);              /* @0x14005b154 跳空白 */

void PECMD_ObjectTimerProc(int64_t param_1, uintptr_t param_2)
{
    /* @0x1400032e4 size=421 TimerProc */
    DWORD DVar1;
    int iVar3;
    LPCWSTR pwVar2;

    if (param_2 == 10) {
        DVar1 = GetTickCount();
        iVar3 = *(int *)(param_1 + 0x328) - (int)DVar1;
        if (((*(char *)(param_1 + 0x3c8) < 1) && (0 < iVar3)) && (0 < g_flagA24F)) {
            if (*(int *)(param_1 + 0x160) - iVar3 < 100) {
                return;
            }
            *(int *)(param_1 + 0x160) = iVar3;
            if (*(char *)(param_1 + 0x3c8) != 0) {
                return;
            }
            if (*(int *)(param_1 + 0x318) == 1000) {
                pwVar2 = WSTR("(%ld)");
            } else {
                pwVar2 = WSTR("(%ld)ms");
            }
            wsprintfW((LPWSTR)(param_1 + 0x164 + (int64_t)*(int *)(param_1 + 0x2f4) * 2),
                      pwVar2);
            SetWindowTextW(*(HWND *)(param_1 + 0x20), (LPCWSTR)(param_1 + 0x164));
            return;
        }
    } else {
        if (param_2 != 0x14) {
            if (param_2 == 0x16) {
                return;
            }
            if (param_2 != 0x1e) {
                return;
            }
            KillTimer(*(HWND *)(param_1 + 0x20), 0x1e);
            SetWindowPos(*(HWND *)(param_1 + 0x20), (HWND)0xfffffffffffffffeULL, 0, 0, 0, 0, 3);
            return;
        }
        if (0 < g_flagA24F) {
            return;
        }
    }
    KillTimer(*(HWND *)(param_1 + 0x20), param_2);
    *(int *)(param_1 + 0x324) = 0;
    *(int *)(param_1 + 800) = (int)*(char *)(param_1 + 0x308);
    SetWindowPos(*(HWND *)(param_1 + 0x20), (HWND)0x0, -10, -10, 1, 1, 0x96);
    if (*(char *)(param_1 + 0x120) == 1) {
        DestroyWindow(*(HWND *)(param_1 + 0x20));
    } else {
        PostMessageW(*(HWND *)(param_1 + 0x20), 0x10, 0, 0);
    }
}

void PECMD_SendPressMsg(int64_t *obj, uint32_t wParam, uint64_t packed)
{
    /* @0x1400034a4 size=47 向对象虚表发送 0x201 按下消息 */
    (**(void (**)(uint64_t, uint32_t, uint32_t, int64_t))(*obj + 8))(
        obj[4], 0x201, wParam,
        (int64_t)(int)(((uint32_t)(packed >> 32) << 0x10) | (uint32_t)packed));
}

void PECMD_SendRButtonUp(int64_t *obj, uint32_t wParam, uint64_t packed)
{
    /* @0x1400034d4 size=47 向对象虚表发送 0x205 消息 */
    (**(void (**)(uint64_t, uint32_t, uint32_t, int64_t))(*obj + 8))(
        obj[4], 0x205, wParam,
        (int64_t)(int)(((uint32_t)(packed >> 32) << 0x10) | (uint32_t)packed));
}

uint64_t *PECMD_ScriptDelete(uint64_t *obj, uint32_t flags)
{
    /* @0x1400035a4 size=47 析构对象并按标志释放 */
    PECMD_ScriptDtor(obj);
    if ((flags & 1) != 0) {
        free(obj);
    }
    return obj;
}

void PECMD_LayoutControlsScrollbar(int64_t *param_1, int param_2, uint32_t param_3, int param_4)
{
    /* @0x1400035d4 size=655 窗口控件布局/滚动条调整 */
    short sVar1;
    int bVar2;
    int bVar3;
    HWND hWnd;
    HWND hWnd_00;
    int iVar4;
    int cy;
    int cx;
    int iVar5;
    BOOL bShow;
    int iVar6;
    int iVar7;
    int iVar8;
    int iVar9;
    int64_t *plVar10;
    RECT local_38;

    if ((param_2 != 2) && (param_2 != 0)) {
        return;
    }
    if ((int)param_1[0x68] == 0) {
        return;
    }
    (**(void (**)(int64_t, int, int, int64_t))(*param_1 + 8))(
        param_1[4], 5, param_2, (int64_t)(int)(param_4 << 0x10 | param_3));
    local_38.left = 0;
    local_38.top = 0;
    local_38.right = 0;
    local_38.bottom = 0;
    GetWindowRect((HWND)param_1[4], &local_38);
    iVar8 = local_38.right - local_38.left;
    iVar7 = local_38.bottom - local_38.top;
    cy = iVar7 - *(int *)((int64_t)param_1 + 0x394);
    cx = iVar8 - (int)param_1[0x72];
    hWnd = GetDlgItem((HWND)param_1[4],
                      (-(uint32_t)((*(uint32_t *)((int64_t)param_1 + 0x31c) & 0x200000) != 0) & 4) + 0x7572);
    SetWindowPos(hWnd, (HWND)0x0, 0, 0, cx, cy, 6);
    sVar1 = *(short *)((int64_t)param_1 + 0x37c);
    iVar6 = 0;
    if (sVar1 != 0) {
        iVar6 = iVar8 / (sVar1 * 2) - (int)param_1[0x6a] / 2;
    }
    iVar4 = *(int *)((int64_t)param_1 + 0x34c);
    if (0 < sVar1) {
        iVar5 = 0;
        plVar10 = param_1 + 0x6b;
        iVar9 = iVar5;
        do {
            hWnd_00 = GetDlgItem((HWND)param_1[4], (int)*plVar10);
            SetWindowPos(hWnd_00, (HWND)0x0,
                         iVar6 + iVar9 / (int)*(short *)((int64_t)param_1 + 0x37c),
                         iVar7 - iVar4, 0, 0, 5);
            iVar5 = iVar5 + 1;
            iVar9 = iVar9 + iVar8;
            plVar10 = (int64_t *)((int64_t)plVar10 + 4);
        } while (iVar5 < *(short *)((int64_t)param_1 + 0x37c));
    }
    iVar4 = 1;
    bShow = 1;
    iVar6 = (int)param_1[0x73];
    iVar7 = *(int *)((int64_t)param_1 + 0x39c);
    bVar2 = cx < iVar6;
    iVar8 = iVar7;
    if (bVar2) {
        iVar8 = iVar7 + 0x20;
    }
    bVar3 = cy < iVar8;
    if (bVar3) {
        bVar2 = cx < iVar6 + 0x20;
    }
    if (bVar2) {
        bVar3 = cy < iVar7 + 0x20;
    }
    if (bVar3) {
        if (cx < iVar6 + 0x20) {
            iVar4 = 3;
            *(uint32_t *)(param_1 + 0x74) = 3;
            goto LAB_14000383f;
        }
        *(uint32_t *)(param_1 + 0x74) = 1;
        ShowScrollBar(hWnd, 1, 1);
        iVar4 = 0;
    } else {
        *(uint32_t *)(param_1 + 0x74) = 0;
        if (bVar2) {
            ShowScrollBar(hWnd, 0, 1);
        } else {
            iVar4 = 3;
        }
    }
    bShow = 0;
LAB_14000383f:
    ShowScrollBar(hWnd, iVar4, bShow);
}

int64_t PECMD_DelayLoadOleaut32(void)
{
    /* @0x1400038fc size=127 延迟加载 Oleaut32 位图/图片函数 */
    if (g_pSysFreeString == 0) {
        FUN_14005C828("SysAllocString", "Oleaut32", &g_pSysAllocString, &g_hOleaut32);
        FUN_14005C828("OleLoadPicture", "Oleaut32", (void **)&Ordinal_418_exref,
                      &g_hOleaut32);
        FUN_14005C828("SysFreeString", "Oleaut32", &g_pSysFreeString, &g_hOleaut32);
    }
    return (int64_t)(uintptr_t)g_pSysFreeString;
}

void FUN_14000397c(void)
{
    /* @0x14000397c size=161 (签名修正自反编译, 主体仍为 NO-OP stub) */
}

uint32_t FUN_140003aac(LPCWSTR param_1, LPCWSTR param_2, uint32_t param_3, int param_4)
{
    /* @0x140003aac size=277 */
    int iVar1;
    int iVar2;
    uint32_t uVar3;
    uint32_t local_28[4];

    local_28[0] = 0;
    if (DAT_14013c990 == 0) {
        FUN_14005C828("WTSSendMessageW", "Wtsapi32.DLL", (void **)&DAT_14013c990,
                      (HMODULE *)0);
    }
    FUN_14005C828("WTSGetActiveConsoleSessionId", "Kernel32", (void **)&DAT_14013c998,
                  &g_hKernel32);
    if ((DAT_14013c990 != 0) && (DAT_14013c998 != 0)) {
        iVar1 = lstrlenW(param_1);
        iVar2 = lstrlenW(param_2);
        uVar3 = (uint32_t)(*DAT_14013c998)();
        if ((*DAT_14013c990)((HANDLE)0, uVar3, param_2, (DWORD)(iVar2 * 2), param_1,
                             (DWORD)(iVar1 * 2), param_3,
                             (DWORD)((param_4 + 999U) / 1000), local_28, 1) != 0) {
            return local_28[0];
        }
    }
    return 0xc0000000;
}

uint64_t FUN_140003bc4(DWORD param_1, int64_t param_2)
{
    /* @0x140003bc4 size=226 */
    int64_t lVar1;
    int64_t lVar4;
    int iVar5;

    iVar5 = 1;
    lVar1 = param_2 / 10 + -1;
    do {
        lVar4 = lVar1;
        if (-1 < lVar1) {
            while (true) {
                if (FUN_1400e3d60(g_szEmpty, 0x10, 0x01, (HWND)0, param_1, 0, 0, 0) != (HWND)0) {
                    break;
                }
                FUN_1400195f0((uint64_t)(uintptr_t)g_Script, 10, 0, (uint64_t *)0);
                if (FUN_14002d708(g_szEmpty, 1, (int64_t *)0, 0, param_1) == 0 ||
                    (lVar4 = lVar4 + -1, lVar4 < 0)) {
                    break;
                }
            }
        }
        FUN_1400195f0((uint64_t)(uintptr_t)g_Script, 10, 99999, (uint64_t *)0);
        iVar5 = iVar5 + -1;
        if (iVar5 < 0) {
            return 0;
        }
    } while (true);
}

bool PECMD_LaunchServiceProcess(LPCWSTR param_1, LPCWSTR param_2)
{
    /* @0x140003ca8 size=2931 安装/拉起 PECMD 后台服务进程(共享内存/互斥/事件) */
    bool bVar2;
    uint32_t uVar3;
    DWORD DVar4;
    int iVar5;
    int iVar6;
    WCHAR *pWVar7;
    HANDLE pvVar8;
    uint64_t *puVar9;
    long long lVar10;
    uint64_t uVar11;
    DWORD DVar12;
    LPWSTR pWVar13;
    WCHAR *pWVar14;
    LPCWSTR pwVar15;
    LPCWSTR pwVar16;
    uint64_t uVar19;
    long long lVar20;
    uint32_t uVar21;
    long long lVar22;
    WCHAR *local_138;
    char local_130;
    bool local_12c[4];
    LPWSTR local_128;
    uint32_t local_120;
    WCHAR *local_118;
    int local_110;
    uint32_t local_10c;
    WCHAR *local_108;
    uint32_t local_100;
    uint32_t local_fc;
    DWORD local_f8;
    int local_f4;
    HANDLE local_f0;
    HANDLE local_e8;
    WCHAR *local_e0;
    WCHAR *local_d8;
    LPCWSTR local_d0;
    uint32_t local_c8;
    WCHAR *local_c0;
    DWORD local_b8;
    HANDLE local_b0;
    DWORD local_a8;
    uint32_t local_a4;
    int local_a0;
    long long local_98;
    long long local_90;
    FILETIME local_88;
    SECURITY_ATTRIBUTES local_80;
    uint8_t local_68;
    uint64_t local_67[5];
    uint64_t tmp;

    FUN_14001c2cc(WSTR("SeDebugPrivilege"), 2, 4);
    lVar20 = 0x80;
    /* 原 in_stack_00000058 标志位在 C ABI 中无实参, 全部按 0 处理 */
    local_10c = 0;
    local_a4 = 0;
    local_c8 = 0;
    pWVar14 = (WCHAR *)0;
    local_fc = 0;
    local_100 = 0;
    uVar21 = 0;
    bVar2 = false;
    local_d0 = g_szEmpty;
    /* bVar2==false: 不取 "--hide " */
    FUN_140063620(&local_108);
    if (param_1 != (LPCWSTR)0x0) {
        FUN_1400703E4(&local_108, WSTR(" --exe:\""));
        FUN_14006375C(&local_108, param_1);
        FUN_14006375C(&local_108, WSTR("\""));
    }
    pWVar7 = pWVar14;
    /* 标志位为 0: 不取环境块, 环境长度 uVar19=0, local_c0=NULL */
    uVar19 = 0;
    local_c0 = (WCHAR *)0;
    FUN_140063620(&local_138);
    FUN_1400637dc(&local_138, WSTR("-nfb -incmd =PECMD EXEC /InstallService "), (int64_t)-1,
                  (int64_t)-1);
    local_f4 = lstrlenW(local_138);
    PECMD_AllocString(&local_138, 0xa000);
    local_b8 = GetCurrentProcessId();
    FUN_140063620(&local_118);
    local_130 = '\0';
    memset(&local_88, 0, sizeof(local_88));
    local_a8 = GetTickCount();
    local_e8 = (HANDLE)0;
    local_f0 = (HANDLE)0;
    local_12c[0] = (*param_2 == L'\0');
    local_68 = 0;
    FUN_140102a90(local_67, 0, 0x27);
    local_80.lpSecurityDescriptor = &local_68;
    local_80.nLength = 0x18;
    local_80.bInheritHandle = 0;
    FUN_14005e7dc((uint64_t *)&local_80.lpSecurityDescriptor);
    local_128 = (LPWSTR)0;
    local_120 = 0;
    do {
        local_120 = local_120 + 1;
        if (((local_120 & 1) == 0) && (local_12c[0] == false)) {
            local_90 = uVar19 + 0x210;
            local_110 = 0;
            lVar22 = (long long)local_f4 * 2;
            pWVar13 = (LPWSTR)((long long)local_138 + lVar22);
            local_98 = lVar22;
            FUN_14005e04c();
            wsprintfW(pWVar13, WSTR("__PECMD.%s.%u.%I64u"), WSTR("EXEC.SYS"), local_b8);
            iVar6 = (int)uVar19 + 0x210;
        } else {
            local_110 = (local_12c[0] != false) + 1;
            lVar22 = (long long)local_f4 * 2;
            local_98 = lVar22;
            wsprintfW((LPWSTR)((long long)local_138 + lVar22), WSTR("PECMD2012.EXESVC"));
            iVar6 = 0x100000;
            local_90 = 0x100000;
        }
        bVar2 = false;
        if ((local_128 != (LPWSTR)0x0) || (local_110 != 0)) {
            bVar2 = true;
        }
        FUN_1400637dc(&local_118, WSTR("Global\\pecmd2012.shm."), (int64_t)-1, (int64_t)-1);
        local_a0 = lstrlenW(local_118);
        FUN_14006375C(&local_118, (LPCWSTR)((long long)local_138 + lVar22));
        DVar12 = iVar6 + 0x88;
        local_f8 = DVar12;
        do {
            pvVar8 = OpenFileMappingW(6, 0, local_118);
            local_b0 = pvVar8;
            GetLastError();
            if (pvVar8 != (HANDLE)0x0) {
                if (bVar2)
                    goto LAB_1400040bd;
LAB_140004287:
                CloseHandle(pvVar8);
                break;
            }
            if ((local_128 != (LPWSTR)0x0) || (local_110 != 1)) {
LAB_1400040bd:
                SetLastError(0);
                if (bVar2) {
                    if (local_e8 == (HANDLE)0x0) {
                        FUN_1400702F0((int64_t *)&local_d8, "Global\\pecmd2012.lock.",
                                      (uint64_t)-1);
                        FUN_14006375C(&local_d8, (LPCWSTR)(local_98 + (long long)local_138));
                        local_e8 = CreateMutexW(&local_80, 1, local_d8);
                        FUN_14005B104(&local_d8);
                        if (local_e8 == (HANDLE)0x0)
                            goto LAB_140004076;
                    }
                    if (local_f0 == (HANDLE)0x0) {
                        FUN_1400702F0((int64_t *)&local_e0, "Global\\pecmd2012.event.",
                                      (uint64_t)-1);
                        FUN_14006375C(&local_e0, (LPCWSTR)(local_98 + (long long)local_138));
                        local_f0 = CreateEventW(&local_80, 0, 0, local_e0);
                        if (local_f0 == (HANDLE)0x0) {
                            FUN_14005B104(&local_e0);
                            goto LAB_140004076;
                        }
                        FUN_14005B104(&local_e0);
                    }
                }
                if (pvVar8 == (HANDLE)0x0) {
                    pvVar8 = CreateFileMappingW((HANDLE)(uintptr_t)-1, &local_80, 4, 0, DVar12,
                                                local_118);
                    local_b0 = pvVar8;
                    DVar4 = GetLastError();
                    if (pvVar8 == (HANDLE)0x0) {
                        if (local_128 != (LPWSTR)0x0) {
                            if (DVar4 != 5)
                                goto LAB_140004076;
                            goto LAB_14000478b;
                        }
                        FUN_1400195f0((uint64_t)(uintptr_t)g_Script, 5, 0, NULL);
                        break;
                    }
                    if ((local_128 == (LPWSTR)0x0) && (DVar4 == 0xb7))
                        goto LAB_140004287;
                }
                puVar9 = MapViewOfFile(pvVar8, 6, 0, 0, (size_t)(long long)(int)DVar12);
                pvVar8 = local_b0;
                if (puVar9 == (uint64_t *)0x0)
                    goto LAB_140004287;
                FUN_140102a90(puVar9, 0, 0x80);
                if (bVar2) {
                    iVar5 = lstrlenW(local_d0);
                    iVar5 = iVar5 * 2;
                    iVar6 = lstrlenW(param_2);
                    iVar6 = iVar6 * 2 + 2;
                    lVar20 = (long long)(iVar6 + iVar5);
                    if (local_90 < lVar20) {
                        lVar20 = local_90;
                    }
                    if ((local_10c != 0) && (local_100 == 0)) {
                        puVar9[5] = puVar9[5] | 1;
                    }
                    FUN_14001d78c((uint8_t *)(puVar9 + 0x10), (uint8_t *)local_d0, iVar5 + 2);
                    FUN_14001d78c((uint8_t *)((long long)iVar5 + 0x80 + (long long)puVar9),
                                  (uint8_t *)param_2, iVar6);
                    *(int *)(puVar9 + 3) = (int)lVar20;
                    *(uint16_t *)(lVar20 + 0x7e + (long long)puVar9) = 0;
                    lVar20 = lVar20 + 0x80;
                    pWVar7 = local_c0;
                    DVar12 = local_f8;
                }
                /* (in_stack & 0x200)==0: 插入当前目录块被跳过 */
                DVar12 = local_f8;
                uVar11 = (uint64_t)(int)(local_f8 - (int)lVar20);
                if ((long long)uVar11 < (long long)uVar19) {
                    uVar19 = uVar11;
                }
                if ((pWVar7 != (WCHAR *)0x0) && (4 < uVar19)) {
                    FUN_14001d78c((uint8_t *)(lVar20 + (long long)puVar9), (uint8_t *)pWVar7,
                                  (int)uVar19);
                    lVar20 = lVar20 + uVar19;
                    *(uint32_t *)(lVar20 + -4 + (long long)puVar9) = 0;
                    *(int *)(puVar9 + 4) = (int)uVar19;
                }
                if (param_1 != (LPCWSTR)0x0) {
                    iVar5 = DVar12 - (int)lVar20;
                    iVar6 = lstrlenW(param_1);
                    iVar6 = iVar6 * 2 + 2;
                    if (iVar5 < iVar6) {
                        iVar6 = iVar5;
                    }
                    if (0 < iVar6) {
                        FUN_14001d78c((uint8_t *)(lVar20 + (long long)puVar9), (uint8_t *)param_1,
                                      iVar6);
                        lVar20 = lVar20 + iVar6;
                        *(int *)((long long)puVar9 + 0x24) = iVar6;
                        *(uint16_t *)(lVar20 + -2 + (long long)puVar9) = 0;
                    }
                }
                *(uint64_t *)(lVar20 + 8 + (long long)puVar9) = 0;
                *(uint32_t *)(lVar20 + (long long)puVar9) = 0x10;
                *(uint32_t *)(lVar20 + 4 + (long long)puVar9) = 0x10;
                *(uint32_t *)(puVar9 + 6) = 0x10;
                puVar9[0] = 0xffffffffffffffffULL;
                if ((bVar2) && (local_f0 != (HANDLE)0x0)) {
                    SetEvent(local_f0);
                }
                iVar5 = lstrlenW(local_138);
                uVar3 = local_100;
                iVar6 = local_110;
                lVar22 = (long long)local_a0;
                pwVar15 = g_szEmpty;
                pwVar16 = g_szEmpty;
                if (local_100 != 0) {
                    pwVar16 = WSTR("--usr");
                }
                if (local_110 != 0) {
                    pwVar15 = WSTR("--svr");
                }
                wsprintfW((LPWSTR)((long long)local_138 + (long long)iVar5 * 2),
                          WSTR(" --desk %s %s --ack:%s %s%s %s"), pwVar15, pwVar16);
                local_12c[0] = false;
                local_12c[1] = false;
                tmp = FUN_14000e26c((uint64_t)(uintptr_t)g_Script,
                                    (uint64_t)(uintptr_t)local_138,
                                    (uint64_t)(uintptr_t)g_Script, 0, 0, NULL, 0, NULL);
                memcpy(&local_88, &tmp, 8);
                local_130 = '\x01';
                if ((iVar6 == 0) && (uVar3 == 0)) {
                    bVar2 = false;
                } else {
                    bVar2 = true;
                    wsprintfW((LPWSTR)local_138, WSTR("-wait %s"));
                    puVar9 = PECMD_ServiceControl(&g_Script, (LPCWSTR)local_138);
                }
                DVar12 = GetTickCount();
                while ((uVar19 = *puVar9, uVar19 == 0xffffffffffffffffULL &&
                       (DVar4 = GetTickCount(), DVar4 - DVar12 < 30000))) {
                    FUN_1400195f0((uint64_t)(uintptr_t)g_Script, 1, 0, NULL);
                }
                if (local_c0 != (WCHAR *)0x0) {
                    FreeEnvironmentStringsW(local_c0);
                }
                pWVar14 = (WCHAR *)puVar9[2];
                uVar11 = puVar9[1];
                DVar12 = (DWORD)pWVar14;
                if (0 < (long long)pWVar14) {
                    pvVar8 = OpenProcess(0x100400, 0, DVar12);
                    GetLastError();
                    if (pvVar8 == (HANDLE)0x0) {
                        pvVar8 = OpenProcess(0x100000, 0, DVar12);
                    }
                    GetLastError();
                    /* in_stack_00000050==NULL: 不回填 (local_10c==0) */
                    if (((((local_fc != 0) || (uVar21 == 0)) || (local_a4 != 0)) &&
                         (WaitForInputIdle(pvVar8, 5000), local_fc != 0)) || (uVar21 == 0)) {
                        FUN_140003bc4(DVar12,
                                      (uint64_t)(((-(uint32_t)(local_fc != 0 || local_c8 != 0))
                                                  & 0x1220) + 0x168));
                    }
                }
                uVar21 = local_10c;
                if ((local_10c == 0) && (0 < (long long)pWVar14)) {
                    wsprintfW((LPWSTR)local_138, WSTR("*0x%X"),
                              (uint64_t)pWVar14 & 0xffffffff);
                    FUN_14003ed4c(&g_Script, (LPCWSTR)local_138);
                }
                if ((!bVar2) && (0 < (int)uVar19)) {
                    wsprintfW((LPWSTR)local_138, WSTR("*0x%X"), uVar11 & 0xffffffff);
                    FUN_14003cd0c(&g_Script, (void *)local_138);
                }
                if ((uVar21 == 0) && (0 < (long long)pWVar14)) {
                    wsprintfW((LPWSTR)local_138, WSTR("*0x%X"),
                              (uint64_t)pWVar14 & 0xffffffff);
                    FUN_14003cd0c(&g_Script, (void *)local_138);
                }
                if (!bVar2) {
                    wsprintfW((LPWSTR)local_138, WSTR("-wait !%s"), local_118 + lVar22);
                    PECMD_ServiceControl(&g_Script, (LPCWSTR)local_138);
                    wsprintfW((LPWSTR)local_138, WSTR("-delete %s"), local_118 + lVar22);
                    PECMD_ServiceControl(&g_Script, (LPCWSTR)local_138);
                }
LAB_140004778:
                if (local_b0 != (HANDLE)0x0) {
                    CloseHandle(local_b0);
                }
LAB_14000478b:
                if (local_f0 != (HANDLE)0x0) {
                    CloseHandle(local_f0);
                }
                pvVar8 = local_e8;
                if (local_e8 != (HANDLE)0x0) {
                    ReleaseMutex(local_e8);
                    CloseHandle(pvVar8);
                }
                if (((long long)pWVar14 < 1) && (local_130 != '\0')) {
                    SetLastError(local_88.dwLowDateTime);
                }
                FUN_14005B104(&local_118);
                FUN_14005B104(&local_138);
                FUN_14005B104(&local_108);
                return 0 < (long long)pWVar14;
            }
LAB_140004076:
            pWVar13 = local_128;
            DVar4 = GetTickCount();
            if (29999 < DVar4 - local_a8)
                goto LAB_140004778;
            FUN_1400195f0((uint64_t)(uintptr_t)g_Script, 5, 0, NULL);
        } while (pWVar13 != (LPWSTR)0x0);
    } while (true);
}

void FUN_14000481c(LARGE_INTEGER param_1, int param_2)
{
    /* @0x14000481c size=166 自删除/等待命令构造 */
    int iVar1;
    uint64_t uVar2;
    WCHAR *local_res18[2];

    if (param_2 != 0) {
        FUN_14003C06C((int64_t *)&g_Script, param_1, 0);
    }
    uVar2 = PECMD_CheckFileAttr((LPCWSTR)param_1.QuadPart);
    if (0 < (int64_t)uVar2) {
        if (param_2 == 0) {
            GetCurrentProcessId();
        }
        iVar1 = lstrlenW((LPCWSTR)param_1.QuadPart);
        FUN_140063694(local_res18, (int64_t)iVar1 + 200);
        /* TODO(verify): Ghidra 未显示 wsprintfW 的变参 */
        wsprintfW(local_res18[0], WSTR("PECMD**pecmd-cmd* WAIT *%lu -del \"%s\""));
        FUN_14007724c(local_res18[0]);
        FUN_14005b104((WCHAR **)local_res18);
    }
}

uint64_t FUN_1400048c4(int64_t *param_1)
{
    /* @0x1400048c4 size=322 (签名修正自反编译, 主体仍为 NO-OP stub) */
    (void)param_1;
    return 0;
}

void FUN_140004a08(void)
{
    /* @0x140004a08 size=188 */
    int iVar1;
    int64_t *local_res8[4];

    EnterCriticalSection((LPCRITICAL_SECTION)&g_csCom);
    FUN_140061C44();
    iVar1 = -3;
    if ((g_pOleInit != NULL) &&
        (iVar1 = ((int (*)(int))g_pOleInit)(0), iVar1 != 0) && (iVar1 == 1)) {
        ((void (*)(void))g_pOleUninit)();
    }
    local_res8[0] = NULL;
    ((int (*)(void *, int, int, void *, int64_t **))g_pCoCreateInstance)(
        (void *)&g_guid2d168, 0, 1, (void *)&g_guid2d178, local_res8);
    if (local_res8[0] == NULL) {
        if ((g_pOleUninit != NULL) && (iVar1 == 0)) {
            ((void (*)(void))g_pOleUninit)();
        }
    } else {
        ((uint32_t (*)(int64_t *, int))
         ((uint64_t *)(uintptr_t)*local_res8[0])[0x18 / 8])(local_res8[0], 4);
        ((void (*)(void))((uint64_t *)(uintptr_t)*local_res8[0])[0x10 / 8])();
        if ((g_pOleUninit != NULL) && (iVar1 == 0)) {
            ((void (*)(void))g_pOleUninit)();
        }
    }
    LeaveCriticalSection((LPCRITICAL_SECTION)&g_csCom);
}

uint64_t PECMD_SetDesktopWallpaper(void *param_1, uint64_t param_2)
{
    /* @0x140004ac4 size=878 COM 桌面壁纸设置例程 */
    int iVar1;
    BOOL BVar2;
    int iVar3;
    uint32_t uVar4;
    DWORD DVar5;
    uint64_t uVar6;
    uint64_t uVar7;
    int64_t *local_res18;
    uint32_t local_opt[3];   /* SetDesktopItemOptions 选项 {0xc,1,1} */
    uint32_t local_opt2[2];  /* SetWallpaperOptions 选项 {8,2} */

    EnterCriticalSection((LPCRITICAL_SECTION)&g_csCom);
    FUN_140061C44();
    iVar3 = -3;
    uVar6 = 0;
    iVar1 = -3;
    if ((g_pOleInit != NULL) &&
        (iVar1 = ((int (*)(int))g_pOleInit)(0), iVar1 != 0) && (iVar1 == 1)) {
        ((void (*)(void))g_pOleUninit)();
    }
    local_res18 = NULL;
    uVar7 = (uint64_t)g_wallpaperMode;
    if ((param_2 & 1) != 0) {
        uVar7 = uVar6;
    }
    g_wallpaperMode = (uint8_t)uVar7;
    uVar7 = 1;
    if (g_wallpaperMode != '\0') {
        BVar2 = SystemParametersInfoW(0x14, 0, param_1, 3);
        uVar7 = (uint64_t)(BVar2 == 0);
        if (uVar7 == 0) {
            if ((g_pOleUninit != NULL) && (iVar1 == 0)) {
                ((void (*)(void))g_pOleUninit)();
            }
            goto LAB_140004e0e;
        }
    }
    EnterCriticalSection((LPCRITICAL_SECTION)&g_csCom);
    FUN_140061C44();
    if ((g_pOleInit != NULL) &&
        (iVar3 = ((int (*)(int))g_pOleInit)(0), iVar3 != 0) && (iVar3 == 1)) {
        ((void (*)(void))g_pOleUninit)();
    }
    uVar4 = ((int (*)(void *, int, int, void *, int64_t **))g_pCoCreateInstance)(
        (void *)&g_guid2d168, 0, 1, (void *)&g_guid2d178, &local_res18);
    if ((int)uVar4 < 0) {
        FUN_140025f10((int64_t)(uintptr_t)g_Script, WSTR("<DEBUG>WALL CoCreateInstance()"),
                      uVar4, (void *)0, (void *)0, NULL);
    }
    g_wallpaperMode = (uint8_t)-1;
    if ((int)uVar4 < 0) {
LAB_140004d61:
        if ((g_pOleUninit != NULL) && (iVar3 == 0)) {
            ((void (*)(void))g_pOleUninit)();
        }
    } else {
        local_opt[0] = 0xc;
        local_opt[1] = 1;
        local_opt[2] = 1;
        uVar4 = ((uint32_t (*)(int64_t *, uint32_t *, int))
                 ((uint64_t *)(uintptr_t)*local_res18)[0x58 / 8])(
            local_res18, local_opt, 0);
        if ((int)uVar4 < 0) {
            FUN_140025f10((int64_t)(uintptr_t)g_Script,
                          WSTR("<DEBUG>WALL SetDesktopItemOptions()"),
                          uVar4, (void *)0, (void *)0, NULL);
        }
        uVar4 = ((uint32_t (*)(int64_t *, void *, int))
                 ((uint64_t *)(uintptr_t)*local_res18)[0x28 / 8])(
            local_res18, param_1, 0);
        if ((int)uVar4 < 0) {
            FUN_140025f10((int64_t)(uintptr_t)g_Script, WSTR("<DEBUG>WALL SetWallpaper()"),
                          uVar4, (void *)0, (void *)0, NULL);
        }
        local_opt2[0] = 8;
        local_opt2[1] = 2;
        uVar4 = ((uint32_t (*)(int64_t *, uint32_t *, int))
                 ((uint64_t *)(uintptr_t)*local_res18)[0x38 / 8])(
            local_res18, local_opt2, 0);
        if ((int)uVar4 < 0) {
            FUN_140025f10((int64_t)(uintptr_t)g_Script,
                          WSTR("<DEBUG>WALL SetWallpaperOptions()"),
                          uVar4, (void *)0, (void *)0, NULL);
        }
        g_wallpaperMode = (uint8_t)0x01;
        uVar4 = ((uint32_t (*)(int64_t *, int))
                 ((uint64_t *)(uintptr_t)*local_res18)[0x18 / 8])(local_res18, 7);
        uVar7 = (uint64_t)(int)uVar4;
        if (-1 < (int)uVar4) {
            g_wallpaperMode = (uint8_t)0x02;
            uVar7 = uVar6;
            goto LAB_140004d61;
        }
        FUN_140025f10((int64_t)(uintptr_t)g_Script, WSTR("<DEBUG>WALL ApplyChanges()"),
                      uVar4, (void *)0, (void *)0, NULL);
        if ((g_pOleUninit != NULL) && (iVar3 == 0)) {
            ((void (*)(void))g_pOleUninit)();
        }
    }
    LeaveCriticalSection((LPCRITICAL_SECTION)&g_csCom);
    if (local_res18 != NULL) {
        ((void (*)(void))((uint64_t *)(uintptr_t)*local_res18)[0x10 / 8])();
    }
    if (uVar7 != 0) {
        BVar2 = SystemParametersInfoW(0x14, 0, param_1, 3);
        DVar5 = GetLastError();
        if (BVar2 == 0) {
            if (local_res18 == NULL) {
                uVar6 = (uint64_t)DVar5;
                if (DVar5 == 0) {
                    uVar6 = 1;
                }
                goto LAB_140004dd0;
            }
        } else {
LAB_140004dd0:
            uVar7 = uVar6;
            if (BVar2 != 0) goto LAB_140004df7;
        }
        FUN_140025f10((int64_t)(uintptr_t)g_Script,
                      WSTR("<DEBUG>WALL SystemParametersInfo()"),
                      DVar5, (void *)0, (void *)0, NULL);
    }
LAB_140004df7:
    uVar6 = uVar7 & 0xffffffff;
    if ((g_pOleUninit != NULL) && (iVar1 == 0)) {
        ((void (*)(void))g_pOleUninit)();
    }
LAB_140004e0e:
    LeaveCriticalSection((LPCRITICAL_SECTION)&g_csCom);
    return uVar6;
}

uint64_t FUN_140004ebc(uint8_t *param_1)
{
    /* @0x140004ebc size=123 消息泵/线程循环 */
    DWORD DVar2;
    DWORD DVar3;
    uint8_t bVar1;
    uint64_t uVar4;

    DVar2 = GetTickCount();
    uVar4 = FUN_140004e34(5, (int64_t)param_1);
    if ((int)uVar4 == 0) {
        *param_1 = *param_1 & 0xef;
        FUN_140004e34(0x138d, (int64_t)param_1);
    }
    *param_1 = 0;
    bVar1 = param_1[1];
    while ((bVar1 == 0 && (DVar3 = GetTickCount(), DVar3 - DVar2 < 0x138d))) {
        Sleep(1);
        bVar1 = param_1[1];
    }
    g_flag16a = 0;
    return 0;
}

void PECMD_CreateHelperThread(uint8_t *param_1)
{
    /* @0x140004f38 size=67 */
    HANDLE pvVar1;

    *param_1 = 0x11;
    pvVar1 = FUN_14005b228((int64_t (*)(void *))FUN_140004ebc, (LPVOID)param_1, 0x10000,
                           0x10000, (DWORD *)&g_dwC96C, (LPSECURITY_ATTRIBUTES)0);
    if (pvVar1 == (HANDLE)0x0) {
        *param_1 = 0;
    }
}

void PECMD_LoadPsapiFunctions(void)
{
    /* @0x140004f7c size=87 延迟加载 PSAPI.DLL 函数 */
    uint64_t err[4] = {0};

    if (g_u64C9F0 == 0) {
        FUN_14005C828("GetModuleFileNameExW", "PSAPI.DLL",
                      (void **)&g_u64C9E8, (HMODULE *)err);
        FUN_14005C828("EnumProcessModules", "PSAPI.DLL",
                      (void **)&g_u64C9F0, (HMODULE *)err);
    }
}

void PECMD_SwitchFiberWorkingSet(uint64_t *param_1)
{
    /* @0x1400050a0 size=40 设置工作集并切换到目标 fiber */
    uint64_t uVar1;

    uVar1 = FUN_140004fd4((LPCWSTR)*param_1);
    param_1[1] = uVar1;
    SwitchToFiber(g_pFiber);
}

void FUN_140005148(void)
{
    /* @0x140005148 size=107 */
    HMODULE hModule;
    void *pFVar1;

    hModule = LoadLibraryA("user32.dll");
    if (hModule != (HMODULE)0x0) {
        pFVar1 = GetProcAddress(hModule, "ChangeWindowMessageFilter");
        if (pFVar1 != (void *)0x0) {
            ((void (*)(UINT, UINT))pFVar1)(0x233, 1);
            ((void (*)(UINT))pFVar1)(0x4a);
            ((void (*)(UINT))pFVar1)(0x49);
        }
        FreeLibrary(hModule);
    }
}

void FUN_1400051b4(void)
{
    /* @0x1400051b4 size=152 */
    UINT UVar1;

    UVar1 = SetErrorMode(0);
    SetErrorMode(UVar1 | 0x8001);
    g_hStdIn = GetStdHandle(0xfffffff6);
    g_hStdOut = GetStdHandle(0xfffffff4);
    g_hStdErr = GetStdHandle(0xfffffff5);
    if (g_hHeap == (HANDLE)0) {
        g_hHeap = GetProcessHeap();
    }
    g_exitCodeCache = 0;
    g_pExitCode = &g_exitCodeCache;
    g_dwC934 = TlsAlloc();
    FUN_140018534((uint64_t *)0);
    FUN_140005148();
}

bool FUN_14000524c(uint32_t param_1)
{
    /* @0x14000524c size=205 */
    DWORD DVar1;
    DWORD local_res10[2];   /* {cbSize, dwTime} (tagLASTINPUTINFO) */

    local_res10[0] = 8;
    local_res10[1] = 0;
    GetLastInputInfo(local_res10);
    if (local_res10[1] != g_LastTick && -1 < (int)(local_res10[1] - g_LastTick)) {
        g_LastTick = local_res10[1];
    }
    g_LastTick = GetTickCount();
    while (0 < g_flagA24F) {
        local_res10[1] = 0;
        GetLastInputInfo(local_res10);
        DVar1 = GetTickCount();
        if ((param_1 <= DVar1 - local_res10[1]) && (param_1 <= DVar1 - g_LastTick)) break;
        Sleep(10);
    }
    if (local_res10[1] != g_LastTick && -1 < (int)(local_res10[1] - g_LastTick)) {
        g_LastTick = local_res10[1];
    }
    return 0 < g_flagA24F;
}

uint64_t *FUN_140005390(uint64_t *p, uint64_t value, uint32_t count)
{
    /* @0x140005390 size=35 按字节数填充 */
    if ((int)(count - 1) >= 0) {
        memset(p, (int)value, (size_t)count);
    }
    return p;
}

LPCWSTR FUN_1400053b4(LPCWSTR param_1, LPCWSTR param_2)
{
    /* @0x1400053b4 size=182 不区分大小写子串搜索 */
    LPCWSTR pWVar1;
    int iVar2;
    int iVar3;
    LPCWSTR pWVar4;
    int64_t lVar5;
    WCHAR WVar6;
    uint16_t uVar7;
    short sVar8;

    sVar8 = 0;
    iVar2 = lstrlenW(param_1);
    iVar3 = lstrlenW(param_2);
    pWVar1 = param_1 + (int64_t)(iVar2 - iVar3);
    if (param_1 <= pWVar1) {
        lVar5 = (int64_t)((uintptr_t)param_1 - (uintptr_t)param_2);
        do {
            WVar6 = *param_2;
            pWVar4 = param_2;
            while (WVar6 != L'\0') {
                uVar7 = *(uint16_t *)((uintptr_t)lVar5 + (uintptr_t)pWVar4);
                WVar6 = *pWVar4;
                pWVar4 = pWVar4 + 1;
                if ((uVar7 < 0x5b) && (uVar7 = (uint16_t)(uVar7 | 0x20), uVar7 < 0x5b)) {
                    WVar6 = (WCHAR)(WVar6 | 0x20);
                }
                sVar8 = (short)(uVar7 - WVar6);
                if (sVar8 != 0) goto LAB_14000543b;
                WVar6 = *pWVar4;
            }
            if (sVar8 == 0) {
                return param_1;
            }
LAB_14000543b:
            param_1 = param_1 + 1;
            lVar5 = lVar5 + 2;
        } while (param_1 <= pWVar1);
    }
    return (LPCWSTR)0x0;
}

uint16_t *PECMD_SkipLeadingWhitespace(uint16_t *s)
{
    /* @0x14000531c size=38 跳过前导空白/控制字符 */
    while ((8 < *s && *s < 0xe) || *s == L' ') {
        s++;
    }
    return s;
}

uint16_t *FUN_14000546c(uint16_t *s)
{
    /* @0x14000546c size=85 跳过引号串或空白分隔的 token */
    uint16_t ch = *s;

    if (ch == L'"') {
        uint16_t *start = s;
        do {
            s = start + 1;
            if (*s == 0) {
                break;
            }
        } while (*s != L'"');
        if (*s == L'"') {
            s = start + 2;
        }
    } else {
        while (ch != 0 && ((ch < 9 || 0xd < ch) && ch != L' ')) {
            s++;
            ch = *s;
        }
    }
    return s;
}

void PECMD_SkipTokenAndSpaces(uint16_t *s)
{
    /* @0x1400054c4 size=29 跳过 token 及其前后空白 */
    PECMD_SkipLeadingWhitespace(FUN_14000546c(PECMD_SkipLeadingWhitespace(s)));
}

uint8_t PECMD_ParseDecimalDigits(uint16_t **pp, int *out)
{
    /* @0x1400054e4 size=55 解析连续十进制数字 */
    uint8_t r = 0;

    while (**pp > 0x2f && **pp < 0x3a) {
        *out = (**pp - 0x30) + *out * 10;
        (*pp)++;
        r = 1;
    }
    return r;
}

void FUN_14000551c(LPCSTR param_1)
{
    /* @0x14000551c size=208 按窗口标题关闭对话框/窗口 */
    HWND hWnd;

    if ((param_1 != (LPCSTR)0x0) && (*param_1 != '\0')) {
        if (g_pSharedMap == (void *)0x0) {
            hWnd = (HWND)0x0;
        } else {
            hWnd = *(HWND *)g_pSharedMap;
        }
        if (hWnd == (HWND)0x0) {
            FindWindowA("#32770", param_1);
            Sleep(0x32);
            hWnd = FindWindowA("#32770", param_1);
            if ((hWnd == (HWND)0x0) &&
                (hWnd = FindWindowA((LPCSTR)0x0, param_1), hWnd == (HWND)0x0)) {
                return;
            }
        }
        SendMessageW(hWnd, 0x10, 0, 0);
        if (hWnd != (HWND)0x0) {
            SendMessageW(hWnd, 0x111, 2, 0);
            SendMessageW(hWnd, 0x111, 7, 0);
            DestroyWindow(hWnd);
        }
    }
}

void FUN_1400055ec(LPCWSTR param_1, uint64_t param_2)
{
    /* @0x1400055ec size=159 */
    HANDLE hFileMappingObject;
    uint64_t *puVar1;

    hFileMappingObject =
        CreateFileMappingW((HANDLE)(uintptr_t)-1, (LPSECURITY_ATTRIBUTES)0x0, 0x8000004, 0,
                           100, param_1);
    if (hFileMappingObject != (HANDLE)0x0) {
        puVar1 = (uint64_t *)MapViewOfFile(hFileMappingObject, 6, 0, 0, 0);
        CloseHandle(hFileMappingObject);
        if (puVar1 != (uint64_t *)0x0) {
            FUN_140005390(puVar1, 0, 100);
            *(uint32_t *)puVar1 = 0xffffffff;
            puVar1[2] = param_2;
            PTR_DAT_14013a040 = (void *)(puVar1 + 1);
            g_u64CA20 = (uint64_t)(uintptr_t)puVar1;
        }
    }
}

void PECMD_UnmapFileView(void)
{
    /* @0x14000568c size=45 解除文件映射视图 */
    if ((LPCVOID)(uintptr_t)g_u64CA20 != NULL) {
        UnmapViewOfFile((LPCVOID)(uintptr_t)g_u64CA20);
    }
    g_u64CA20 = 0;
}

void FUN_1400056bc(void)
{
    /* @0x1400056bc size=121 */
}

uint64_t FUN_140005818(int64_t *param_1, LPCWSTR param_2)
{
    /* @0x140005818 size=115 */
    int64_t lVar1;
    void *puVar2;

    PECMD_GrowByteBuffer((void **)param_1, (int64_t)((int)param_1[1] * 8 + 8));
    lVar1 = *param_1;
    puVar2 = operator_new(8);
    if (puVar2 == NULL) {
        puVar2 = NULL;
    } else {
        FUN_1400702B0((WCHAR **)puVar2, param_2);
    }
    *(void **)(lVar1 + (int64_t)(int)param_1[1] * 8) = puVar2;
    *(int *)(param_1 + 1) = (int)param_1[1] + 1;
    return (uint64_t)(uintptr_t)param_2;
}

void PECMD_LockInit(int64_t unused, char enter)
{
    /* @0x14000588c size=34 按标志进入/离开全局临界区 */
    (void)unused;
    if (enter == 0) {
        LeaveCriticalSection(&g_csInit);
    } else {
        EnterCriticalSection(&g_csInit);
    }
}

LARGE_INTEGER PECMD_ExecuteStringCommand(int64_t obj, LPCWSTR s)
{
    /* @0x1400058b0 size=68 执行字符串命令并返回结果 */
    LARGE_INTEGER cmd;
    LARGE_INTEGER result;

    cmd.QuadPart = 0;
    FUN_1400702B0((WCHAR **)&cmd, s);
    result = FUN_14004c0bc(*(uint64_t *)(obj + 8), cmd,
                           *(void **)(obj + 0x10), NULL, NULL);
    FUN_14005B104((WCHAR **)&cmd);
    return result;
}

void PECMD_ExecuteCommandNoRet(int64_t obj, LARGE_INTEGER cmd)
{
    /* @0x1400058f4 size=31 执行命令（不取返回值） */
    FUN_14004c0bc(*(uint64_t *)(obj + 8), cmd,
                  *(void **)(obj + 0x10), NULL, NULL);
}

uint64_t FUN_140005b5c(short *param_1)
{
    /* @0x140005b5c size=187 版本串解析(每段 16 位) */
    uint64_t uVar1;
    short *local_res8;
    uint64_t local_res10;

    local_res10 = 0;
    local_res8 = param_1;
    FUN_140067b78((int64_t *)&local_res8, &local_res10);
    uVar1 = local_res10;
    if (*local_res8 == 0x2e) {
        local_res8 = local_res8 + 1;
    }
    FUN_140067b78((int64_t *)&local_res8, &local_res10);
    uVar1 = uVar1 << 0x10 | local_res10;
    if (*local_res8 == 0x2e) {
        local_res8 = local_res8 + 1;
    }
    FUN_140067b78((int64_t *)&local_res8, &local_res10);
    uVar1 = uVar1 << 0x10 | local_res10;
    if (*local_res8 == 0x2e) {
        local_res8 = local_res8 + 1;
    }
    FUN_140067b78((int64_t *)&local_res8, &local_res10);
    return uVar1 << 0x10 | local_res10;
}

void FUN_140005c18(LPWSTR param_1, uint64_t param_2, uint32_t param_3)
{
    /* @0x140005c18 size=49
     * [DECOMPILE FAILED] — 参考文件中该函数标记为 [DECOMPILE FAILED]。
     * 语义: 将多段版本号(uint64_t, 每 16 位一段)格式化成 "M.m" 宽串写入 param_1。
     * 因无法忠实恢复, 此处 SKIPPED-due-to-decompile-failure, 提供最小 no-op 体。 */
    (void)param_1;
    (void)param_2;
    (void)param_3;
}

uint64_t PECMD_GetFileVersionInfo(LPCWSTR param_1, void *param_2, void *param_3, uint64_t *param_4,
                       uint32_t param_5)
{
    /* @0x140005c4c size=1131 读取文件版本信息 (ProductVersion/FileVersion) 到指定串容器 */
    uint16_t uVar2;
    uint32_t uVar3;
    uint32_t uVar4;
    bool bVar5;
    LPWSTR lpSubBlock;
    DWORD dwLen;
    uint32_t uVar6;
    uint16_t *puVar1;
    uint16_t *puVar7;
    uint16_t *puVar8;
    uint64_t uVar9;
    LPCWSTR lpString1;
    LPWSTR lpData;
    DWORD local_res10[2];
    uint64_t local_res18[2];
    LPVOID local_68;
    LPVOID local_60;
    LPVOID local_58;
    LPWSTR local_50;
    LPWSTR local_48;
    long long local_40;

    FUN_140063694((WCHAR **)&local_40, 0x169);
    lpString1 = (LPCWSTR)(local_40 + 200);
    lstrcpynW((LPWSTR)lpString1, param_1, 0x104);
    uVar9 = 0;
    if ((param_2 == (void *)0x0) ||
        (bVar5 = true, param_2 == (void *)(uintptr_t)0xffffffffffffffffULL)) {
        bVar5 = false;
    }
    if (param_3 != (void *)0x0) {
        PECMD_AllocString((WCHAR **)param_3, 100);
        **(uint16_t **)param_3 = 0;
    }
    if (bVar5) {
        PECMD_AllocString((WCHAR **)param_2, 100);
        **(uint16_t **)param_2 = 0;
    }
    if (g_pGetFileVersionInfoExW == (void *)0x0) {
        local_res18[0] = 0;
        FUN_14005C828("GetFileVersionInfoSizeExW", "Version", (void **)&g_pGetFileVersionInfoSizeExW,
                      (HMODULE *)local_res18);
        FUN_14005C828("GetFileVersionInfoExW", "Version", (void **)&g_pGetFileVersionInfoExW,
                      (HMODULE *)local_res18);
        if ((g_pGetFileVersionInfoSizeExW == (void *)0x0) || (g_pGetFileVersionInfoExW == (void *)0x0)) {
            g_pGetFileVersionInfoSizeExW = (void *)(uintptr_t)-1;
            g_pGetFileVersionInfoExW = (void *)(uintptr_t)-1;
        }
    }
    local_res10[0] = 0;
    if (g_pGetFileVersionInfoExW == (void *)(uintptr_t)-1) {
        dwLen = GetFileVersionInfoSizeW(lpString1, local_res10);
    } else {
        dwLen = ((DWORD (*)(uint32_t, LPCWSTR, DWORD *))g_pGetFileVersionInfoSizeExW)(param_5, lpString1,
                                                                        local_res10);
        if (dwLen == 0) {
            param_5 = 2;
            dwLen = ((DWORD (*)(uint32_t, LPCWSTR, DWORD *))g_pGetFileVersionInfoSizeExW)(2, lpString1,
                                                                            local_res10);
        }
    }
    uVar6 = GetLastError();
    if (dwLen == 0) {
        if (param_4 != (uint64_t *)0x0) {
            if (uVar6 == 0) {
                uVar6 = 1;
            }
            *param_4 = (uint64_t)uVar6;
        }
    } else {
        local_res18[0] = local_res18[0] & 0xffffffff00000000ULL;
        local_60 = (LPVOID)0x0;
        FUN_1400633A8((void **)&local_48, (int64_t)(dwLen + 0x1a0));
        local_68 = (uint16_t *)0x0;
        local_50 = local_48;
        lpData = local_48 + 200;
        if (g_pGetFileVersionInfoExW == (void *)(uintptr_t)-1) {
            GetFileVersionInfoW(lpString1, local_res10[0], dwLen, lpData);
        } else {
            ((BOOL (*)(uint32_t, LPCWSTR, DWORD, DWORD, LPVOID))g_pGetFileVersionInfoExW)(
                param_5, lpString1, local_res10[0], dwLen, lpData);
        }
        VerQueryValueW(lpData, WSTR("\\"), &local_60, (UINT *)local_res18);
        if (param_2 == (void *)0x0) {
            uVar3 = *(uint32_t *)((long long)local_60 + 0x10);
            uVar4 = *(uint32_t *)((long long)local_60 + 0x14);
        } else {
            uVar3 = *(uint32_t *)((long long)local_60 + 8);
            uVar4 = *(uint32_t *)((long long)local_60 + 0xc);
        }
        local_58 = (LPVOID)0x0;
        uVar9 = ((uint64_t)uVar4 << 32) | uVar3;
        VerQueryValueW(lpData, WSTR("\\VarFileInfo\\Translation"), &local_58, (UINT *)local_res18);
        lpSubBlock = local_50;
        if ((param_3 != (void *)0x0) || (param_2 == (void *)0x0)) {
            wsprintfW(local_50, WSTR("\\StringFileInfo\\%08lx\\ProductVersion"));
            VerQueryValueW(lpData, lpSubBlock, &local_68, (UINT *)local_res18);
            puVar8 = (uint16_t *)local_68;
            if ((local_68 != (void *)0x0) && (*puVar8 != 0)) {
                uVar9 = FUN_140005b5c((short *)puVar8);
            }
            if (param_3 != (void *)0x0) {
                if ((local_68 == (void *)0x0) || (*puVar8 == 0)) {
                    FUN_140005c18((LPWSTR)*(void **)param_3,
                                  (uint64_t)*(uint32_t *)((long long)local_60 + 0x10),
                                  *(uint32_t *)((long long)local_60 + 0x14));
                } else {
                    puVar7 = (uint16_t *)*(void **)param_3;
                    puVar1 = puVar8 + 99;
                    while (puVar8 < puVar1) {
                        uVar2 = *puVar8;
                        puVar8 = puVar8 + 1;
                        *puVar7 = uVar2;
                        if ((uVar2 == 0) || (((8 < uVar2 && (uVar2 < 0xe)) || (uVar2 == 0x20))))
                            break;
                        puVar7 = puVar7 + 1;
                    }
                    *puVar7 = 0;
                }
            }
        }
        if ((bVar5) || (param_2 != (void *)0x0)) {
            wsprintfW(lpSubBlock, WSTR("\\StringFileInfo\\%08lx\\FileVersion"));
            VerQueryValueW(lpData, lpSubBlock, &local_68, (UINT *)local_res18);
            puVar8 = (uint16_t *)local_68;
            if ((local_68 != (void *)0x0) && (*puVar8 != 0)) {
                uVar9 = FUN_140005b5c((short *)puVar8);
            }
            if (bVar5) {
                if ((local_68 == (void *)0x0) || (*puVar8 == 0)) {
                    FUN_140005c18((LPWSTR)*(void **)param_2,
                                  (uint64_t)*(uint32_t *)((long long)local_60 + 8),
                                  *(uint32_t *)((long long)local_60 + 0xc));
                } else {
                    puVar7 = (uint16_t *)*(void **)param_2;
                    puVar1 = puVar8 + 99;
                    while (puVar8 < puVar1) {
                        uVar2 = *puVar8;
                        puVar8 = puVar8 + 1;
                        *puVar7 = uVar2;
                        if ((uVar2 == 0) || (((8 < uVar2 && (uVar2 < 0xe)) || (uVar2 == 0x20))))
                            break;
                        puVar7 = puVar7 + 1;
                    }
                    *puVar7 = 0;
                }
            }
        }
        FUN_14005B104((WCHAR **)&local_48);
    }
    FUN_14005B104((WCHAR **)&local_40);
    return uVar9;
}

HANDLE FUN_1400060b8(HANDLE param_1)
{
    /* @0x1400060b8 size=282 (签名修正自反编译, 主体仍为 NO-OP stub) */
    (void)param_1;
    return (HANDLE)0;
}

short *FUN_1400061d4(uint64_t *param_1, short *param_2, LPWSTR param_3, LPCWSTR param_4)
{
    /* @0x1400061d4 size=287 */
    DWORD DVar1;
    int64_t local_res10;
    LPCWSTR local_res20;
    LPCWSTR local_28;
    LPWSTR local_20;

    if (param_2 == NULL) {
        return NULL;
    }
    if (*param_2 != 0x2a) {
        return param_2;
    }
    local_res20 = param_4;
    FUN_140063694((WCHAR **)&local_20, 0x209);
    if (param_3 == NULL) {
        if (local_res20 == NULL) goto LAB_1400062c7;
        FUN_14005B154((WCHAR **)&local_res20);
        local_res10 = 0;
        FUN_140024c48((int64_t *)&local_res20, &local_res10, 0);
        FUN_1400702D4((WCHAR **)&local_28, local_res20, local_res10);
        if ((local_res10 < 1) ||
            (DVar1 = SearchPathW(NULL, local_28, NULL, 0x208, local_20, (LPWSTR *)NULL), DVar1 == 0)) {
            FUN_14005B104((WCHAR **)&local_28);
            goto LAB_1400062c7;
        }
        FUN_14005B104((WCHAR **)&local_28);
        param_3 = local_20;
    }
    local_res10 = 0;
    FUN_1400e3cd4(param_3, param_1, &local_res10);
    if (local_res10 != 0) {
        *(short *)(local_res10 + -2) = 0;
        param_2 = (short *)*param_1;
    }
LAB_1400062c7:
    FUN_14005B104((WCHAR **)&local_20);
    return param_2;
}

uint64_t PECMD_GetModuleFileNameIntoContainer(uint64_t *out)
{
    /* @0x1400062f4 size=45 获取当前模块文件名到串容器 */
    PECMD_AllocString((WCHAR **)out, 0x105);
    GetModuleFileNameW((HMODULE)0, (LPWSTR)*out, 0x104);
    return *out;
}

bool PECMD_IsFile(LPCWSTR path)
{
    /* @0x140006324 size=40 路径非空且不是目录 */
    if (path == NULL || *path == L'\0') {
        return false;
    }
    return (int)PECMD_IsDirectory(path) == 0;
}

bool FUN_14000634c(void)
{
    /* @0x14000634c size=153 */
    bool bVar1;
    int64_t local_res8[4];

    if ((uintptr_t)g_pMemQuery == 0) {
        bVar1 = false;
    } else if ((uintptr_t)g_pMemQuery == 1) {
        local_res8[0] = 0;
        FUN_14005C828("DeleteProcThreadAttributeList", "Kernel32.DLL",
                      (void **)&g_u64CA50, (HMODULE *)local_res8);
        FUN_14005C828("UpdateProcThreadAttribute", "Kernel32.DLL",
                      (void **)&g_pProcMemRead, (HMODULE *)local_res8);
        FUN_14005C828("InitializeProcThreadAttributeList", "Kernel32.DLL",
                      (void **)&g_pMemQuery, (HMODULE *)local_res8);
        bVar1 = (uintptr_t)g_pMemQuery != 0;
    } else {
        bVar1 = true;
    }
    return bVar1;
}

uint64_t *PECMD_ResetSlots(uint64_t *p)
{
    /* @0x1400063e8 size=17 清零三个槽 */
    p[0] = 0;
    p[1] = 0;
    p[2] = 0;
    return p;
}

uint64_t FUN_1400063fc(void *param_1, DWORD param_2, long long param_3)
{
    /* @0x1400063fc size=290 打开进程并(可选)建立可读内存视图 */
    uint64_t *pl;
    HANDLE pvVar2;
    uint64_t uVar3;
    long long local_res8;

    pl = (uint64_t *)param_1;
    if ((g_privFlags & 2) == 0) {
        g_privFlags = g_privFlags | 3;
        FUN_14001c2cc(WSTR("SeRestorePrivilege"), 2, 0);
        FUN_14001c2cc(WSTR("SeSecurityPrivilege"), 2, 0);
        FUN_14001c2cc(WSTR("SeTakeOwnershipPrivilege"), 2, 0);
    }
    pvVar2 = OpenProcess(0x1fffff, 0, param_2);
    pl[2] = pvVar2;
    if (pvVar2 != (HANDLE)0x0) {
        local_res8 = 0;
        if (FUN_14000634c() != 0) {
            ((void (*)(uint64_t, int, int, void *))g_pMemQuery)(0, 1, 0, &local_res8);
            uVar3 = (uint64_t)PECMD_GrowByteBuffer((void **)param_1, local_res8);
            pl[1] = uVar3;
            ((void (*)(uint64_t, int, int, void *))g_pMemQuery)(uVar3, 1, 0, &local_res8);
            if (pl[2] != 0) {
                ((void (*)(uint64_t, int, int, uint64_t *, int, int, int))g_pProcMemRead)(
                    pl[1], 0, 0x20000, &pl[2], 8, 0, 0);
            }
            if (param_3 != 0) {
                *(uint64_t *)(param_3 + 0x68) = pl[1];
            }
            return pl[2];
        }
    }
    return 0;
}

void PECMD_ReleaseObjectResources(int64_t *p)
{
    /* @0x140006520 size=52 调用释放回调、关闭句柄并释放串容器 */
    if (p[1] != 0) {
        ((void (*)(void))g_u64CA50)();
    }
    if ((HANDLE)(uintptr_t)p[2] != (HANDLE)0) {
        CloseHandle((HANDLE)(uintptr_t)p[2]);
    }
    FUN_14005B104((WCHAR **)p);
}

uint64_t FUN_140006660(HWND param_1)
{
    /* @0x140006660 size=86 (not in this batch; signature fixed from decompile) */
    (void)param_1;
    return 0;
}

void PECMD_ReleaseGdiObject(int64_t *obj, uint64_t unused, HGDIOBJ gdi)
{
    /* @0x1400066b8 size=44 调用对象虚表释放，并删除 GDI 对象 */
    (void)unused;
    if (obj != NULL) {
        (**(void (**)(int64_t *, int))(*obj + 0x18))(obj, 1);
        if (gdi != (HGDIOBJ)0) {
            DeleteObject(gdi);
        }
    }
}

int8_t FUN_1400066e4(void)
{
    /* @0x1400066e4 size=138 统计可运行处理器个数 */
    BOOL BVar1;
    HANDLE hProcess;
    uint32_t uVar2;
    int8_t cVar3;
    DWORD_PTR local_res8[4];

    cVar3 = (int8_t)g_u8CA49;
    if (g_u8CA49 == '\0') {
        EnterCriticalSection((void *)&g_csInit);
        local_res8[1] = 0;
        local_res8[0] = 0;
        hProcess = GetCurrentProcess();
        BVar1 = GetProcessAffinityMask(hProcess, &local_res8[1], &local_res8[0]);
        if (BVar1 == 0) {
            cVar3 = '\x01';
        } else {
            cVar3 = '\0';
            for (uVar2 = 0;
                 (g_u8CA49 = (uint8_t)cVar3, local_res8[0] != 0 && (uVar2 < 0x40));
                 uVar2 = uVar2 + 1) {
                if ((local_res8[0] & 1) != 0) {
                    cVar3 = (int8_t)(cVar3 + '\x01');
                }
                local_res8[0] = local_res8[0] >> 1;
            }
        }
        LeaveCriticalSection((void *)&g_csInit);
    }
    return cVar3;
}

uint16_t *FUN_140006770(DWORD param_1, uint16_t *param_2)
{
    /* @0x140006770 size=144 取指定进程的模块文件名 */
    int iVar1;
    HANDLE hObject;
    uint16_t *puVar2;
    uint32_t local_res10[2];
    uint64_t local_res18;

    puVar2 = (uint16_t *)0;
    *param_2 = 0;
    local_res18 = 0;
    local_res10[0] = 0;
    hObject = OpenProcess(0x410, 0, param_1);
    if (hObject != (HANDLE)0) {
        PECMD_LoadPsapiFunctions();
        iVar1 = ((int (*)(HANDLE, uint64_t *, uint32_t, uint32_t *))(uintptr_t)g_u64C9F0)
                    (hObject, &local_res18, 8, local_res10);
        if (iVar1 != 0) {
            iVar1 = ((DWORD (*)(HANDLE, uint64_t, uint16_t *, DWORD))(uintptr_t)g_u64C9E8)
                    (hObject, local_res18, param_2, 0x104);
            if (iVar1 != 0) {
                puVar2 = param_2;
            }
        }
        CloseHandle(hObject);
    }
    return puVar2;
}

uint64_t MemoryCopy(void)
{
    /* @0x140006800 size=28 */
    return 0;
}

int PECMD_MemCompareN(uint8_t *a, uint8_t *b, int64_t len)
{
    /* @0x140006830 size=31 限定长度字节比较 */
    uint8_t x;
    uint8_t y;

    do {
        len--;
        if (len < 0) {
            return 0;
        }
        x = *a++;
        y = *b++;
    } while ((uint32_t)x - (uint32_t)y == 0);
    return (int)(uint32_t)x - (int)(uint32_t)y;
}

uint64_t MemorySet(void)
{
    /* @0x140006840 size=28 */
    return 0;
}

void PECMD_DebugOutputWndProc(void)
{
    /* @0x14000685c size=97 OnWndProc1 调试输出 */
    WCHAR local_198[204];

    wsprintfW(local_198, WSTR("OnWndProc1 0x%I64X %I64u 0x%I64X 0x%I64X"));
    if (g_u64c9d0 != (uint64_t *)0x0) {
        PECMD_ExecuteCommand((int64_t *)g_u64c9d0, local_198, 0, (LPCWSTR)0x0, 0, 0, 1);
    }
}

void PECMD_OnWndProc2Debug(void)
{
    /* @0x1400068c0 size=97 OnWndProc2 调试输出 */
    WCHAR local_198[204];

    wsprintfW(local_198, WSTR("OnWndProc2 0x%I64X %I64u 0x%I64X 0x%I64X"));
    if (g_pOnWndProc2 != (void *)0x0) {
        PECMD_ExecuteCommand((int64_t *)g_pOnWndProc2, local_198, 0, (LPCWSTR)0x0, 0, 0, 1);
    }
}

void PECMD_OnWndProc3Debug(void)
{
    /* @0x140006924 size=97 OnWndProc3 调试输出 */
    WCHAR local_198[204];

    wsprintfW(local_198, WSTR("OnWndProc3 0x%I64X %I64u 0x%I64X 0x%I64X"));
    if (g_pOnWndProc3 != (void *)0x0) {
        PECMD_ExecuteCommand((int64_t *)g_pOnWndProc3, local_198, 0, (LPCWSTR)0x0, 0, 0, 1);
    }
}

uint64_t dllMain_Name(void)
{
    /* @0x140006a24 size=38 */
    return 0;
}

WCHAR *FUN_140006aa0(int64_t *param_1, int64_t param_2, uint64_t param_3, LPCWSTR param_4)
{
    /* @0x140006aa0 size=3024 卷信息枚举/格式化 */
    LPWSTR lpOutBuffer;
    uint32_t uVar1;
    int iVar2;
    BOOL BVar3;
    int iVar4;
    int iVar5;
    int iVar6;
    uint32_t uVar7;
    uint64_t uVar8;
    HANDLE pvVar9;
    int64_t lVar10;
    LPWSTR pWVar11;
    LPWSTR pWVar12;
    int64_t lVar13;
    WCHAR *pWVar14;
    LPWSTR pWVar15;
    char cVar16;
    char *pcVar17;
    LPWSTR pWVar18;
    int *piVar19;
    LPCWSTR pWVar20;
    WCHAR *pWVar21;
    WCHAR *pWVar22;
    uint8_t bVar23;
    WCHAR local_res18;
    int64_t local_150;
    LPCWSTR local_148;
    int64_t local_140;
    int local_138;
    int local_134;
    LPWSTR local_130;
    uint32_t local_128;
    DWORD local_124;
    LPWSTR local_120;
    LPCWSTR local_118;
    LPWSTR local_110;
    int local_108;
    int64_t local_100;
    LPCWSTR local_f8;
    LPWSTR local_f0;
    LPWSTR local_e8;
    LPWSTR local_e0;
    LPCWSTR local_d8;
    LPWSTR local_d0;
    WCHAR *local_c8;
    LPWSTR local_c0;
    int64_t local_b8;
    LPWSTR local_b0;
    LPCWSTR local_a8;
    LPWSTR local_a0;
    HANDLE local_98;
    int64_t local_90;
    LPWSTR local_88;
    int64_t local_80;
    uint32_t local_78[4];
    int64_t local_68;
    uint32_t local_60[8];

    FUN_1400633A8((void **)&local_c0, 0x527c);
    local_b0 = local_c0 + 0x2d0;
    local_e8 = local_c0 + 0x784;
    pWVar12 = local_c0 + 0x1cc;
    local_f0 = local_c0 + 0x30c;
    pWVar22 = NULL;
    local_e0 = local_c0 + 0x348;
    local_120 = local_c0;
    local_110 = local_c0 + 900;
    lpOutBuffer = local_c0 + 0x914;
    local_130 = pWVar12;
    PECMD_AllocSmallObject((uint64_t *)&local_140);
    pWVar18 = local_c0 + 0x97e;
    local_150 = -100;
    local_118 = NULL;
    local_a0 = pWVar18;
    memset(pWVar18, 0, 0x3b80);
    pWVar15 = pWVar12;
    if (param_4 != NULL) {
        pWVar15 = param_4;
    }
    local_c0[0x980] = (WCHAR)0xfffe;
    local_c0[0x981] = (WCHAR)0xffff;
    pWVar18[0] = (WCHAR)0xfffe;
    pWVar18[1] = (WCHAR)0xffff;
    *pWVar12 = L'\0';
    local_res18 = L'\0';
    uVar7 = (uint32_t)(param_3 >> 0x10);
    local_128 = uVar7 & 0x10;
    local_90 = (int64_t)(int16_t)param_3;
    uVar1 = (uint32_t)(((uint8_t)(param_3 >> 0x10) & 0x48) == 0x48);
    local_138 = 2;
    if (uVar1 != 0) {
        local_138 = 0;
    }
    local_c8 = NULL;
    local_98 = (HANDLE)0;
    local_d0 = pWVar15;
    if ((param_4 == NULL) &&
        (local_98 = ((FFVW_t)(void *)FindFirstVolumeW)(pWVar15, 0x103),
         local_98 == (HANDLE)-1)) {
LAB_140007647:
        FUN_14005B104((WCHAR **)&local_118);
        FUN_14005B104((WCHAR **)&local_140);
        FUN_14005B104((WCHAR **)&local_c0);
        pWVar14 = pWVar22;
    } else {
        *(uint32_t *)local_110 = (uint32_t)(-(uint32_t)((param_3 & 0x200000) != 0) & 0x20);
        if (param_4 != NULL) {
            iVar2 = StrCmpNW(WSTR("\\\\.\\"), param_4, 4);
            if (((iVar2 == 0) && (pWVar14 = param_4 + 4, *pWVar14 != L'\0')) &&
                (param_4[5] == L':')) {
                local_res18 = *pWVar14;
                local_c8 = pWVar14;
            } else {
                uVar8 = (uint64_t)(uint32_t)FUN_14005c788("\\\\?\\volume{", (const uint16_t *)param_4, 0xb);
                if ((((char)uVar8 != '\0') && (param_4[0xb] != L'\0')) &&
                    ((param_4[0xc] != L'\0' && (local_c8 = pWVar22, param_4[0xd] != L'\0')))) {
                    local_c8 = (WCHAR *)(param_4 + 0xb);
                }
            }
        }
        local_80 = (int64_t)(int)(uVar7 & 0x20);
        local_b8 = (int64_t)(int)uVar1;
        local_68 = (int64_t)(int)(uVar7 & 8);
        pWVar14 = pWVar22;
        do {
            iVar2 = lstrlenW(pWVar15);
            iVar2 = iVar2 + -1;
            if ((-1 < iVar2) && (pWVar15[iVar2] == L'\\')) {
                pWVar15[iVar2] = L'\0';
            }
            local_a8 = pWVar15;
            SetLastError(0);
            if ((param_4 == NULL) || (iVar2 = (int)FUN_140006a4c(pWVar15), iVar2 == 0)) {
                pvVar9 = CreateFileW(pWVar15, 0x80000000, 7, NULL, 3, 0x2000080, (HANDLE)0);
            } else {
                pvVar9 = (HANDLE)FUN_14001d810((uint64_t)pWVar15, 0x80000000, 7);
            }
            if ((pvVar9 != (HANDLE)-1) && (pvVar9 != (HANDLE)0)) {
                memset(lpOutBuffer, 0, 0xc);
                SetLastError(0);
                pWVar12 = local_e8;
                memcpy(local_c0 + 0x916, (const void *)WSTR("\xFFAA\xFFFF\xFFAA\xFFFF"), 8);
                BVar3 = DeviceIoControl(pvVar9, 0x2d1080, NULL, 0, lpOutBuffer, 0xc,
                                        (DWORD *)local_e8, NULL);
                if ((BVar3 == 0) || (*(int *)(local_c0 + 0x916) < 0)) {
                    memcpy(local_c0 + 0x916, (const void *)WSTR("\xFFAA\xFFFF\xFFAA\xFFFF"), 8);
                }
                pWVar18 = pWVar12 + 0x19a;
                memset(pWVar18, 0, 0x90);
                pWVar12[0x1a6] = (WCHAR)0xffaa;
                pWVar12[0x1a7] = (WCHAR)0xffff;
                BVar3 = DeviceIoControl(pvVar9, 0x70048, NULL, 0, pWVar18, 0x90,
                                        (DWORD *)pWVar12, NULL);
                iVar2 = 0;
                if (BVar3 == 0) {
                    pWVar12[0x1a2] = L'\0';
                    pWVar12[0x1a3] = L'\0';
                    pWVar12[0x1a4] = L'\0';
                    pWVar12[0x1a5] = L'\0';
                    pWVar12[0x19e] = L'\0';
                    pWVar12[0x19f] = L'\0';
                    pWVar12[0x1a0] = L'\0';
                    pWVar12[0x1a1] = L'\0';
                    pWVar12[0x1a6] = (WCHAR)0xffaa;
                    pWVar12[0x1a7] = (WCHAR)0xffff;
                } else {
                    iVar2 = *(int *)pWVar18;
                }
                CloseHandle(pvVar9);
                pWVar21 = local_c8;
                bVar23 = (uint8_t)(iVar2 == 1);
                if ((*(int *)(local_c0 + 0x916) < 0) || (*(int *)(local_c0 + 0x918) < 0)) {
                    FUN_140063620((WCHAR **)&local_d8);
                    FUN_140063620((WCHAR **)&local_f8);
                    if (local_res18 != L'\0') {
                        PECMD_BuildDosDevicePath(local_res18, (int64_t *)&local_d8);
                        pvVar9 = ((FFVW_t)(void *)FindFirstVolumeW)(local_130, 0x103);
                        pWVar21 = pWVar22;
                        if (pvVar9 != (HANDLE)-1) {
                            do {
                                iVar2 = lstrlenW(local_130);
                                if (local_130[iVar2 + -1] == L'\\') {
                                    local_130[iVar2 + -1] = L'\0';
                                }
                                local_130[1] = L'?';
                                PECMD_FindVolumeByDeviceId((uint32_t *)local_130, (int64_t *)&local_f8, local_130);
                                iVar2 = lstrcmpiW(local_d8, local_f8);
                                if (iVar2 == 0) {
                                    local_d0 = local_130;
                                    pWVar21 = local_130 + 0xb;
                                    pWVar15 = local_130;
                                    break;
                                }
                                *local_130 = L'\0';
                                BVar3 = ((FNFVW_t)(void *)FindNextVolumeW)(pvVar9, local_130, 0x103);
                            } while (BVar3 != 0);
                            FindVolumeClose(pvVar9);
                        }
                    }
                    if (local_150 < -99) {
                        local_150 = PECMD_EnumeratePartitions(param_2, &local_140, (uint32_t *)pWVar21);
                    }
                    if (0 < local_150) {
                        pcVar17 = (char *)(local_140 + 4);
                        if (pWVar21 == NULL) {
                            iVar2 = StrCmpNIW(WSTR("\\\\?\\volume{"), pWVar15, 0xb);
                            if (iVar2 == 0) {
                                local_148 = pWVar15 + 0xb;
                                lVar10 = PECMD_ParseVolumeGuid((int64_t *)&local_148, local_60, 1);
                                if (('\0' < (char)lVar10) && (iVar2 = 0, 0 < local_150)) {
                                    do {
                                        iVar4 = FUN_14005b184(pcVar17, (int64_t)local_60, 0x10);
                                        if (iVar4 == 0) goto LAB_1400070f7;
                                        iVar2 = iVar2 + 1;
                                        pcVar17 = pcVar17 + 0x34;
                                    } while (iVar2 < local_150);
                                }
                            }
                        } else {
LAB_1400070f7:
                            *(uint32_t *)(local_c0 + 0x916) = *(uint32_t *)(pcVar17 + 0x10);
                            bVar23 = (uint8_t)(bVar23 | 0x10);
                            *(uint32_t *)(local_c0 + 0x918) = *(uint32_t *)(pcVar17 + 0x14);
                            *(uint32_t *)lpOutBuffer = *(uint32_t *)(pcVar17 + 0x18);
                            *(uint64_t *)(local_e8 + 0x19e) = *(uint64_t *)(pcVar17 + 0x20);
                            *(uint64_t *)(local_e8 + 0x1a2) = *(uint64_t *)(pcVar17 + 0x28);
                        }
                    }
                    FUN_14005B104((WCHAR **)&local_f8);
                    FUN_14005B104((WCHAR **)&local_d8);
                }
                pWVar12 = local_e8;
                if ((((int16_t)local_90 < 0) || ((int)(int16_t)local_90 == *(int *)(local_c0 + 0x918))) &&
                    ((param_2 < 0 || (param_2 == *(int *)(local_c0 + 0x916))))) {
                    if (local_128 != 0) {
                        FUN_14007034c(param_1, pWVar15);
                        pWVar22 = (WCHAR *)*param_1;
                        goto LAB_140007647;
                    }
                    FUN_1400e6d68(local_b0, *(uint64_t *)(local_e8 + 0x19e));
                    FUN_1400e6d68(local_f0, *(uint64_t *)(pWVar12 + 0x1a2));
                    if (bVar23 == 1) {
                        FUN_14005fc90(local_e0, (uint32_t *)(pWVar12 + 0x1aa), 0);
                    } else {
                        wsprintfW(local_e0, WSTR("%d"));
                    }
                    if (local_80 != 0) {
                        if (param_4 != NULL) {
                            pWVar15 = param_4;
                        }
                        local_110[1] = L'\0';
                        PECMD_EnumDevices(pWVar15, local_110 + 1, 0x13, &DAT_14011e890);
                    }
                    pWVar12 = local_120;
                    iVar2 = *(int *)lpOutBuffer;
                    local_134 = -1;
                    if (-1 < *(int *)(local_c0 + 0x916)) {
                        local_134 = *(int *)(local_c0 + 0x916);
                    }
                    iVar4 = -1;
                    if (-1 < *(int *)(local_c0 + 0x918)) {
                        iVar4 = *(int *)(local_c0 + 0x918);
                    }
                    if (local_b8 == 0) {
                        wsprintfW(local_120, WSTR("\r\n%s%s %d %d %d %s %s %s%s"),
                                  local_a8, (LPCWSTR)g_szEmpty);
                    } else {
                        *local_120 = L'\0';
                    }
                    local_108 = lstrlenW((LPCWSTR)*param_1);
                    pWVar15 = local_d0;
                    iVar6 = *(int *)(local_c0 + 0x916);
                    lVar10 = PECMD_EnumNtSymbolicLink(local_d0, NULL, NULL, (int64_t *)&local_118);
                    pWVar18 = pWVar15;
                    if (((lVar10 != 0) && (local_118 != NULL)) && (*local_118 != L'\0')) {
                        pWVar18 = local_118;
                    }
                    pWVar20 = (LPCWSTR)0xffffffff;
                    if (local_68 != 0) {
                        pWVar11 = pWVar12;
                        if (local_b8 == 0) {
                            iVar5 = lstrlenW(pWVar12);
                            pWVar12[iVar5] = L' ';
                            pWVar11 = pWVar12 + iVar5 + 1;
                        }
                        *pWVar11 = L'\0';
                        local_148 = (LPCWSTR)((int64_t)(pWVar11 - pWVar12) >> 1);
                        if (local_res18 == L'\0') {
                            pWVar12 = FUN_140077190(pWVar11, (int64_t)iVar6,
                                                    (int64_t)*(int *)(local_c0 + 0x918),
                                                    (int *)local_a0, NULL, 0x20, pWVar18);
                        } else {
                            *pWVar11 = local_res18;
                            pWVar12 = pWVar11 + 2;
                            pWVar11[1] = L':';
                            *pWVar12 = L'\0';
                        }
                        pWVar20 = local_148;
                        if ((int)((int64_t)(pWVar12 - pWVar11) >> 1) == 0) {
                            pWVar11[-1] = L'\0';
                            if (local_150 < -99) {
                                local_150 = PECMD_EnumeratePartitions(param_2, &local_140, NULL);
                            }
                            lVar10 = local_140;
                            pWVar20 = local_148;
                            if (0 < local_150) {
                                pcVar17 = (char *)(local_140 + 4);
                                iVar6 = StrCmpNIW(WSTR("\\\\?\\volume{"), pWVar15, 0xb);
                                pWVar20 = local_148;
                                if (iVar6 == 0) {
                                    local_124 = 0;
                                    PECMD_AllocSmallObject((uint64_t *)&local_100);
                                    pWVar15[1] = L'?';
                                    FUN_1400690c0((HKEY)0xffffffff80000002,
                                                  WSTR("SYSTEM\\MountedDevices"), pWVar15,
                                                  &local_100, &local_124, NULL);
                                    lVar13 = local_100;
                                    if ((local_124 == 0x18) &&
                                        (iVar6 = FUN_14005b184("DMIO:ID:", local_100, 8), iVar6 == 0)) {
                                        FUN_14001d78c((uint8_t *)local_78,
                                                      (uint8_t *)(lVar13 + 8), 0x10);
                                        cVar16 = '\x02';
                                    } else {
                                        local_88 = pWVar15 + 0xb;
                                        lVar13 = PECMD_ParseVolumeGuid((int64_t *)&local_88, local_78, 1);
                                        cVar16 = (char)lVar13;
                                    }
                                    FUN_14005B104((WCHAR **)&local_100);
                                    pWVar20 = local_148;
                                    if (('\0' < cVar16) && (iVar6 = 0, 0 < local_150)) {
                                        piVar19 = (int *)(lVar10 + 0x18);
                                        do {
                                            iVar5 = FUN_14005b184(pcVar17, (int64_t)local_78, 0x10);
                                            pWVar12 = pWVar11;
                                            if (iVar5 == 0) {
                                                uVar7 = FUN_14008ba90(local_78, (int64_t)local_a0, 0, NULL);
                                                pWVar12 = local_120;
                                                if (local_134 < 0) {
                                                    local_134 = piVar19[-1];
LAB_1400074ad:
                                                    if (iVar4 < 0) {
LAB_1400074b2:
                                                        iVar4 = *piVar19;
                                                    }
                                                    if (iVar2 < 0) {
                                                        iVar2 = piVar19[1];
                                                    }
                                                    iVar5 = wsprintfW(local_120,
                                                        WSTR("\r\n%s%s %d %d %d %s %s %s"),
                                                        local_a8);
                                                    pWVar11 = pWVar12 + iVar5;
                                                } else {
                                                    if (iVar4 < 0) goto LAB_1400074b2;
                                                    if (iVar2 < 0) goto LAB_1400074ad;
                                                }
                                                pWVar12 = pWVar11;
                                                if (0 < (int)uVar7) {
                                                    *pWVar11 = (WCHAR)uVar7;
                                                    pWVar12 = pWVar11 + 2;
                                                    pWVar11[-1] = L' ';
                                                    pWVar11[1] = L':';
                                                    *pWVar12 = L'\0';
                                                }
                                            }
                                            iVar6 = iVar6 + 1;
                                            pcVar17 = pcVar17 + 0x34;
                                            piVar19 = piVar19 + 0xd;
                                            pWVar11 = pWVar12;
                                            pWVar20 = local_148;
                                        } while (iVar6 < local_150);
                                    }
                                }
                            }
                        }
                    }
                    iVar2 = local_138;
                    FUN_14006375C((WCHAR **)param_1, local_120 + local_138);
                    local_138 = 0;
                    pWVar14 = (WCHAR *)(*param_1 + (int64_t)((local_108 - iVar2) + (int)pWVar20) * 2);
                }
            }
            pvVar9 = local_98;
            pWVar15 = local_d0;
            if (local_98 == (HANDLE)0) goto LAB_1400075eb;
            *local_d0 = L'\0';
            BVar3 = ((FNFVW_t)(void *)FindNextVolumeW)(local_98, local_d0, 0x103);
        } while (BVar3 != 0);
        FindVolumeClose(pvVar9);
LAB_1400075eb:
        FUN_14005B104((WCHAR **)&local_118);
        FUN_14005B104((WCHAR **)&local_140);
        FUN_14005B104((WCHAR **)&local_c0);
        if (local_128 != 0) {
            pWVar14 = pWVar22;
        }
    }
    return pWVar14;
}

HANDLE PECMD_OpenLockVolume(int param_1, uint64_t param_2, int64_t param_3,
                     uint32_t param_4, WCHAR *param_5)
{
    /* @0x140007670 size=1286 卷批量开/锁/刷新与句柄缓存 */
    WCHAR *pWVar1;
    WCHAR *pWVar2;
    int iVar3;
    BOOL BVar4;
    BOOL BVar5;
    DWORD DVar6;
    DWORD DVar7;
    HANDLE pvVar8;
    HANDLE pvVar9;
    HANDLE pvVar11;
    int64_t *plVar10;
    LPWSTR lpString;
    uint64_t uVar12;
    uint64_t local_res10;
    char local_res18;
    int64_t local_res20;
    uint32_t local_e58;
    uint32_t local_e54;
    uint32_t local_e50;
    HANDLE local_e48;
    uint64_t local_e40;
    WCHAR *local_e30;
    uint64_t local_e28;
    uint64_t local_e20;
    int64_t local_e18;
    int local_e10;
    DWORD local_d98;
    DWORD local_d94[99];
    uint64_t buf;                       /* param_5 高低32位打包(作 LPDWORD 缓冲) */
    WCHAR local_a78[1308];

    pWVar2 = param_5;
    plVar10 = g_pVolHandles;
    uVar12 = param_2 + (uint64_t)param_3;
    local_res18 = (-1 < (int64_t)param_2) ? 1 : 0;
    local_a78[0] = L'\0';
    if (uVar12 < param_2) {
        uVar12 = 0x7fffffffffffffffULL;
    }
    pvVar8 = (HANDLE)(uint64_t)(param_5 != (WCHAR *)0);
    local_e48 = (HANDLE)0;
    local_e40 = uVar12;

    if (param_2 == 0xffffffffffffffffULL) {
        /* 清空并释放已缓存的卷句柄数组 */
        local_res10 = 0xffffffffffffffffULL;
        local_res18 = 0;
        if (plVar10 != (int64_t *)0) {
            while (0 < *plVar10) {
                *plVar10 = *plVar10 - 1;
                pvVar8 = (HANDLE)plVar10[*plVar10 + 1];
                plVar10[*plVar10 + 1] = 0;
                uVar12 = local_res10 >> 0x20;
                local_res10 = uVar12 << 0x20;
                if (pvVar8 != (HANDLE)0) {
                    local_res10 = uVar12 << 0x20;
                    DeviceIoControl(pvVar8, 0x9001c, (LPVOID)0, 0, (LPVOID)0, 0,
                                    (DWORD *)&local_res10, (void *)0);
                    GetLastError();
                    CloseHandle(pvVar8);
                }
            }
            FUN_14005B0D4(&g_pVolHandles);
        }
        return (HANDLE)0;
    }

    local_res10 = param_2;
    if (param_5 == (WCHAR *)0) {
        local_e48 = ((FFVW_t)(void *)FindFirstVolumeW)(local_a78, 0x514);
        if (local_e48 != (HANDLE)(intptr_t)-1) goto LAB_1400077a5;
        goto LAB_140007796;
    } else {
        goto LAB_1400077a5;
    }

LAB_140007796:
    pvVar8 = (HANDLE)1;
    return pvVar8;

LAB_1400077a5:
    if (plVar10 == (int64_t *)0) {
        plVar10 = (int64_t *)PECMD_GrowByteBuffer((void **)&g_pVolHandles, 0xfa8);
        if (g_pVolHandles == (int64_t *)0) goto LAB_140007796;
        *g_pVolHandles = 0;
    }
    local_res20 = (int64_t)(int)(param_4 & 1);
    lpString = local_a78;
    local_e30 = local_a78;
    if (pWVar2 != (WCHAR *)0) {
        lpString = pWVar2;
        local_e30 = pWVar2;
    }
    do {
        iVar3 = lstrlenW(lpString) - 1;
        if (0 < iVar3) {
            if (lpString[iVar3] == L'\\') {
                lpString[iVar3] = L'\0';
            }
            SetLastError(0);
            pvVar9 = CreateFileW(lpString, 0x80000000, 7, (LPSECURITY_ATTRIBUTES)0,
                                 3, 0x80, (HANDLE)0);
            if (pvVar9 != (HANDLE)(intptr_t)-1) {
                iVar3 = -100;
                SetLastError(0);
                pWVar1 = param_5;
                pvVar11 = pvVar9;
                if (pWVar2 == (WCHAR *)0) {
                    local_d98 = 0;
                    memset(local_d94, 0, 0x31c);
                    local_e54 = 0xffffffff;
                    local_e58 = 0;
                    local_e50 = 0;
                    BVar4 = DeviceIoControl(pvVar9, 0x2d1080, (LPVOID)0, 0,
                                            &local_e58, 0xc, &local_d98, (void *)0);
                    if (BVar4 != 0) {
                        iVar3 = (int)(int16_t)local_e54;
                    }
                    pWVar1 = param_5;
                    if (param_1 == iVar3) {
                        memset(&local_e28, 0, 0x90);
                        memset(&local_e28, 0, 0x90);
                        local_e10 = -1;
                        DeviceIoControl(pvVar9, 0x70048, (LPVOID)0, 0,
                                        &local_e28, 0x90, &local_d98, (void *)0);
                        DeviceIoControl(pvVar9, 0x70048, (LPVOID)0, 0,
                                        &local_e28, 0x90, &local_d98, (void *)0);
                        pWVar1 = param_5;
                        if (local_e10 == -1) {
                            iVar3 = -1;
                        } else if (local_res18 != 0 &&
                                   (uVar12 <= local_e20 ||
                                    (int64_t)(local_e20 + local_e18) < (int64_t)param_2)) {
                            iVar3 = -1;
                        }
                        goto LAB_14000795d;
                    }
                } else {
LAB_14000795d:
                    buf = (uint64_t)(uintptr_t)pWVar1 & 0xffffffff00000000ULL;
                    if ((param_1 == iVar3) || (pWVar2 != (WCHAR *)0)) {
                        iVar3 = 0;
                        buf &= 0xffffffff00000000ULL;
                        DVar6 = 0;
                        FlushFileBuffers(pvVar9);
                        FlushFileBuffers(pvVar9);
                        FlushFileBuffers(pvVar9);
                        FlushFileBuffers(pvVar9);
                        do {
                            buf &= 0xffffffff00000000ULL;
                            BVar4 = DeviceIoControl(pvVar9, 0x90018, (LPVOID)0, 0,
                                                     (LPVOID)0, 0, (DWORD *)&buf,
                                                     (void *)0);
                            if (BVar4 != 0) goto LAB_1400079f4;
                            Sleep(10);
                            iVar3 = iVar3 - 1;
                        } while (-1 < iVar3);
                        GetLastError();
LAB_1400079f4:
                        BVar5 = BVar4;
                        if (local_res20 != 0) {
                            iVar3 = 4;
                            do {
                                buf &= 0xffffffff00000000ULL;
                                BVar5 = DeviceIoControl(pvVar9, 0x90020, (LPVOID)0, 0,
                                                        (LPVOID)0, 0, (DWORD *)&buf,
                                                        (void *)0);
                                if (BVar5 != 0) goto LAB_140007a9e;
                                Sleep(10);
                                iVar3 = iVar3 - 1;
                            } while (-1 < iVar3);
                        }
                        DVar6 = 0;
                        if (BVar5 == 0) {
                            DVar6 = GetLastError();
                        }
LAB_140007a9e:
                        pvVar11 = (HANDLE)0;
                        if (pWVar2 == (WCHAR *)0) {
                            if (local_res20 != 0) goto L_shared;
                            if (BVar4 != 1) {
                                DVar7 = (DVar6 != 0) ? DVar6 : 1;
                                pvVar8 = (HANDLE)(int64_t)(int)DVar7;
                            }
                            goto L_shared;
                        } else {
                            pvVar8 = pvVar11;
                            if (local_res20 == 0) goto L_shared;
                            if (BVar5 != 1) goto L_shared;
                        }
L_shared:
                        lpString = local_e30;
                        param_2 = local_res10;
                        uVar12 = local_e40;
                        pWVar1 = (WCHAR *)(uintptr_t)buf;
                        if (*plVar10 < 500) {
                            plVar10[*plVar10 + 1] = (int64_t)pvVar9;
                            *plVar10 = *plVar10 + 1;
                        }
                    }
                }
                if (pvVar11 != (HANDLE)0) {
                    CloseHandle(pvVar11);
                }
            }
        }
        pvVar9 = local_e48;
        if (pWVar2 != (WCHAR *)0) break;
        *lpString = L'\0';
        BVar4 = ((FNFVW_t)(void *)FindNextVolumeW)(local_e48, lpString, 0x514);
    } while (BVar4 != 0);
    if (pvVar9 != (HANDLE)0) {
        FindVolumeClose(pvVar9);
    }
    return pvVar8;
}

int64_t *PECMD_SkipTokenWs(int64_t *pp)
{
    /* @0x140007b78 size=73 跳过 token 后再跳过空白 */
    uint16_t *p = (uint16_t *)(uintptr_t)*pp;

    while (*p != 0 && ((*p < 9 || 0xd < *p) && *p != L' ')) {
        p++;
        *pp = (int64_t)(uintptr_t)p;
    }
    FUN_14005B154((WCHAR **)pp);
    return pp;
}

int PECMD_ParseIntThenSkip(WCHAR **pp, int *out)
{
    /* @0x140007bcc size=36 数字解析后跳过紧邻字符; 返回解析成功标志 */
    int ok = PECMD_ParseUIntValue(pp, out);
    if (**(uint16_t **)pp != 0) {
        *pp = (WCHAR *)((uint16_t *)*pp + 1);
    }
    return ok;
}

void PECMD_CloseFindHandle(uint64_t *p)
{
    /* @0x140007bf0 size=39 关闭查找句柄 */
    HANDLE h = (HANDLE)(uintptr_t)*p;

    if (h != (HANDLE)0 && h != (HANDLE)-1) {
        FindClose(h);
    }
    *p = 0;
}

BOOL PECMD_CreateProcessAsUser(LPCWSTR param_1, LPWSTR param_2, LPSECURITY_ATTRIBUTES param_3,
                   LPSECURITY_ATTRIBUTES param_4, BOOL param_5, uint32_t param_6, LPVOID param_7,
                   LPCWSTR param_8, LPSTARTUPINFOW param_9, LUID param_10, uint32_t param_11,
                   LPCWSTR param_12, LPCWSTR param_13, LPCWSTR param_14)
{
    /* @0x140007c18 size=1270 以指定用户/令牌创建进程 */
    uint32_t dwCreationFlags;
    LPVOID lpEnvironment;
    LPPROCESS_INFORMATION lpProcessInformation;
    int iVar1;
    BOOL BVar2;
    HANDLE pvVar3;
    HANDLE pvVar4;
    LPVOID lpEnvironment_00;
    LPSTARTUPINFOW lpStartupInfo;
    DWORD dwCreationFlags_00;
    HANDLE local_b8;
    HANDLE local_b0;
    int local_a8[2];
    LPVOID local_a0;
    HANDLE local_98;
    SECURITY_ATTRIBUTES local_90;
    TOKEN_PRIVILEGES local_78;
    uint8_t local_68;
    uint64_t local_67[5];

    local_b0 = (HANDLE)0;
    local_68 = 0;
    memset(local_67, 0, 0x27);
    local_90.lpSecurityDescriptor = &local_68;
    local_90.nLength = 0x18;
    local_90.bInheritHandle = 0;
    FUN_14005e7dc((uint64_t *)&local_90.lpSecurityDescriptor);
    local_b8 = (HANDLE)0;
    FUN_14000397c();
    FUN_14005C828("WTSGetActiveConsoleSessionId", "Kernel32",
                  (void **)&DAT_14013c998, &g_hKernel32);
    /* param_10 实为调用方传入的 PROCESS_INFORMATION 地址(Ghidra 以 _LUID 承载) */
    lpProcessInformation = (LPPROCESS_INFORMATION)(uintptr_t)
        (((uint64_t)param_10.HighPart << 32) | (uint64_t)param_10.LowPart);
    lpEnvironment = param_7;
    dwCreationFlags = param_6;
    local_a8[0] = -1;
    lpStartupInfo = param_9;
    if ((DAT_14013c998 == NULL) ||
        (local_a8[0] = (*DAT_14013c998)(), lpStartupInfo = param_9, local_a8[0] == -1))
        goto LAB_140007fcb;
    local_98 = (HANDLE)0;
    if ((param_11 & 1) == 0) {
        pvVar3 = GetCurrentProcess();
        BVar2 = OpenProcessToken(pvVar3, 0x201eb, &local_b8);
        lpStartupInfo = param_9;
        if ((BVar2 == 0) ||
            (BVar2 = LookupPrivilegeValueW(NULL, WSTR("SeDebugPrivilege"), &param_10),
             lpStartupInfo = param_9, BVar2 == 0))
            goto LAB_140007fcb;
        local_78.Privileges[0].Luid.LowPart = param_10.LowPart;
        local_78.Privileges[0].Luid.HighPart = param_10.HighPart;
        local_78.PrivilegeCount = 1;
        local_78.Privileges[0].Attributes = 2;
        BVar2 = DuplicateTokenEx(local_b8, 0x2000000, NULL,
                                 /*SecurityIdentification*/1, /*TokenPrimary*/1, &local_b0);
        lpStartupInfo = param_9;
        if ((BVar2 == 0) ||
            (BVar2 = SetTokenInformation(local_b0, /*TokenSessionId*/12, local_a8, 4),
             lpStartupInfo = param_9, BVar2 == 0))
            goto LAB_140007fcb;
        iVar1 = AdjustTokenPrivileges(local_b0, 0, &local_78, 0x10, NULL, NULL);
LAB_140007e89:
        lpStartupInfo = param_9;
        if (iVar1 == 0) goto LAB_140007fcb;
    } else {
        if ((DAT_14013c988 == NULL) ||
            (iVar1 = (*DAT_14013c988)((DWORD)local_a8[0], &local_b8),
             lpStartupInfo = param_9, iVar1 == 0))
            goto LAB_140007fcb;
        param_6 = 0;
        BVar2 = GetTokenInformation(local_b8, /*TokenLinkedToken*/19, &local_98, 8, &param_6);
        if (BVar2 != 0) {
            local_b0 = local_98;
        }
        if (local_b0 == (HANDLE)0) {
            iVar1 = DuplicateTokenEx(local_b8, 0xf01ff, &local_90,
                                     /*SecurityIdentification*/1, /*TokenPrimary*/1, &local_b0);
            goto LAB_140007e89;
        }
    }
    pvVar3 = local_b0;
    lpStartupInfo = param_9;
    if (local_b0 != (HANDLE)0) {
        dwCreationFlags_00 = dwCreationFlags | 0x10;
        if ((param_11 & 2) == 0) {
            if (param_12 != NULL) goto LAB_140007fcb;
        } else {
            pvVar4 = FUN_1400060b8(local_b0);
            lpStartupInfo = param_9;
            if (pvVar4 == (HANDLE)0) goto LAB_140007fcb;
            CloseHandle(local_b0);
            local_b0 = pvVar4;
        }
        local_a0 = NULL;
        lpEnvironment_00 = lpEnvironment;
        if ((lpEnvironment == NULL) && (DAT_14013c9a0 != NULL)) {
            iVar1 = (*DAT_14013c9a0)(&local_a0, pvVar3, 0);
            if (local_a0 != NULL) {
                dwCreationFlags_00 = dwCreationFlags | 0x410;
            }
            lpEnvironment_00 = local_a0;
            lpStartupInfo = param_9;
            if (iVar1 == 0) goto LAB_140007fcb;
        }
        lpStartupInfo = param_9;
        BVar2 = CreateProcessAsUserW(local_b0, param_1, param_2, &local_90, &local_90, param_5,
                                     dwCreationFlags_00, lpEnvironment_00, param_8, param_9,
                                     lpProcessInformation);
        if (local_a0 != NULL) {
            (*DAT_14013c9a8)();
            local_a0 = NULL;
        }
        if (local_b8 != (HANDLE)0) {
            CloseHandle(local_b8);
        }
        local_b8 = (HANDLE)0;
        if (BVar2 != 0) {
            return BVar2;
        }
    }
LAB_140007fcb:
    if (local_b8 != (HANDLE)0) {
        CloseHandle(local_b8);
    }
    local_b8 = (HANDLE)0;
    if ((param_11 & 2) == 0) {
        if (param_12 == NULL) {
            BVar2 = CreateProcessW(param_1, param_2, param_3, param_4, param_5, dwCreationFlags,
                                   lpEnvironment, param_8, lpStartupInfo, lpProcessInformation);
        } else {
            BVar2 = CreateProcessWithLogonW(param_12, param_13, param_14, dwCreationFlags, param_1,
                                            param_2, dwCreationFlags, lpEnvironment, param_8,
                                            lpStartupInfo, lpProcessInformation);
        }
    } else {
        local_b8 = FUN_1400060b8((HANDLE)0);
        BVar2 = CreateProcessAsUserW(local_b8, param_1, param_2, param_3, param_4, param_5,
                                     dwCreationFlags, lpEnvironment, param_8, lpStartupInfo,
                                     lpProcessInformation);
    }
    if (local_b8 != (HANDLE)0) {
        CloseHandle(local_b8);
    }
    local_b8 = (HANDLE)0;
    if (local_b0 != (HANDLE)0) {
        CloseHandle(local_b0);
    }
    return BVar2;
}

HANDLE PECMD_CreateTempMutexDir(int64_t *param_1, int64_t *param_2, uint64_t *param_3, uint64_t param_4)
{
    /* @0x140008110 size=687 临时目录/互斥文件创建 */
    int64_t lVar1;
    bool bVar2;
    BOOL BVar3;
    int iVar4;
    uint64_t uVar5;
    int64_t *plVar6;
    HANDLE hObject;
    int iVar7;
    int64_t *plVar8;
    uint64_t local_res8;
    int local_res10;
    uint64_t *local_res18;
    uint64_t local_res20;
    int64_t local_60[4];

    hObject = (HANDLE)0;
    plVar8 = local_60 + 1;
    if (param_1 != NULL) {
        plVar8 = param_1;
    }
    local_60[1] = 0;
    plVar6 = local_60;
    if (param_2 != NULL) {
        plVar6 = param_2;
    }
    local_60[0] = 0;
    local_res18 = param_3;
    local_res20 = param_4;
    if (param_2 == NULL) {
        if (param_1 == NULL) {
            FUN_14005B104((WCHAR **)local_60);
            FUN_14005B104((WCHAR **)(local_60 + 1));
            return (HANDLE)0;
        }
        lstrcpyW((LPWSTR)*plVar6, (LPCWSTR)*plVar8);
    }
    PECMD_AllocString((WCHAR **)plVar8, 0x26c);
    uVar5 = FUN_1400048c4(plVar6);
    local_res8 = 0;
    local_res8 = (local_res8 & 0xffffffff00000000ULL) | (uint64_t)(uint32_t)uVar5;
    FUN_14001d78c((uint8_t *)*plVar8, (uint8_t *)*plVar6, ((int)uVar5 + 1) * 2);
    SetLastError(0);
    local_res10 = 199;
    do {
        iVar4 = (int)uVar5;
        if (param_1 == NULL) {
            bVar2 = false;
LAB_1400082ba:
            iVar7 = 0;
            goto LAB_1400082bf;
        }
        lVar1 = *plVar8;
        GetTickCount();
        FUN_14005e04c();
        GetCurrentProcessId();
        wsprintfW((LPWSTR)(lVar1 + (int64_t)iVar4 * 2), WSTR("~pecmd_%s.%lu.%lu~%s%"));
        BVar3 = CreateDirectoryW((LPCWSTR)*plVar8, NULL);
        if (BVar3 != 0) {
            bVar2 = true;
            if (param_2 != NULL) {
                iVar4 = lstrlenW((LPCWSTR)*plVar8);
                local_res8 = (local_res8 & 0xffffffff00000000ULL) |
                             (uint64_t)(uint32_t)(iVar4 + 1);
                FUN_14001d78c((uint8_t *)*plVar6, (uint8_t *)*plVar8, (iVar4 + 1) * 2);
                *(uint16_t *)(*plVar6 + (int64_t)iVar4 * 2) = 0x5c;
                *(uint16_t *)(*plVar6 + (int64_t)(int)local_res8 * 2) = 0;
                iVar4 = (int)local_res8;
                goto LAB_1400082ba;
            }
            goto LAB_140008397;
        }
        FUN_1400195f0((uint64_t)(uintptr_t)g_Script, 5, 0, NULL);
        local_res10 = local_res10 + -1;
        uVar5 = local_res8;
    } while (-1 < local_res10);
    goto LAB_14000836d;
    while (true) {
        FUN_1400195f0((uint64_t)(uintptr_t)g_Script, 5, 0, NULL);
        iVar7 = iVar7 + 1;
        if (199 < iVar7) break;
LAB_1400082bf:
        lVar1 = *plVar6;
        GetCurrentProcessId();
        wsprintfW((LPWSTR)(lVar1 + (int64_t)iVar4 * 2), WSTR("~pecmd_%s.%lu.%lu~%s"),
                  local_res20);
        local_res8 = 0;
        /* PECMD_OpenFileHandle 经 *out 写回句柄并返回 void (Ghidra 以 eax 残留承载返回值) */
        PECMD_OpenFileHandle((HANDLE *)&local_res8, (LPCWSTR)*plVar6,
                      0x40000000, 0, NULL, 1, 0x80, (HANDLE)0);
        hObject = (HANDLE)local_res8;
        if (hObject != (HANDLE)0) goto LAB_140008368;
    }
    *(uint16_t *)*plVar6 = 0;
LAB_140008368:
    if (bVar2) goto LAB_140008374;
LAB_14000836d:
    *(uint16_t *)*plVar8 = 0;
LAB_140008374:
    if (local_res18 == NULL) {
        if (hObject != (HANDLE)0) {
            CloseHandle(hObject);
        }
    } else {
        *local_res18 = (uint64_t)hObject;
    }
LAB_140008397:
    FUN_14005B104((WCHAR **)local_60);
    FUN_14005B104((WCHAR **)(local_60 + 1));
    return hObject;
}

void FUN_140008710(LPWSTR param_1, char param_2, uint8_t param_3)
{
    /* @0x140008710 size=290 在当前会话创建进程并(可选)等待 */
    uint32_t uVar1;
    BOOL BVar2;
    HANDLE local_98;
    HANDLE local_90;
    uint64_t local_88;
    STARTUPINFOW local_78;
    LUID luid10;

    local_90 = (HANDLE)0x0;
    local_98 = (HANDLE)0x0;
    local_78.cb = 0x68;
    local_78.lpReserved = (LPWSTR)0x0;
    local_88 = 0;
    FUN_140102a90(&local_78.lpDesktop, 0, 0x58);
    uVar1 = local_78.dwFlags | 0x80;
    if ((param_3 & 1) != 0) {
        local_78.wShowWindow = 0;
        uVar1 = local_78.dwFlags | 0x81;
    }
    local_78.dwFlags = uVar1;
    luid10.LowPart = (DWORD)(uintptr_t)&local_98;
    luid10.HighPart = (LONG)((uintptr_t)&local_98 >> 32);
    BVar2 = PECMD_CreateProcessAsUser((LPCWSTR)0x0, param_1, (LPSECURITY_ATTRIBUTES)0x0,
                          (LPSECURITY_ATTRIBUTES)0x0, 1, 0, (LPVOID)0x0, (LPCWSTR)0x0,
                          &local_78, luid10, (int)(char)param_3 & 2, (LPCWSTR)0x0,
                          WSTR("."), (LPCWSTR)0x0);
    if (BVar2 != 0) {
        CloseHandle(local_90);
        if (param_2 != '\0') {
            WaitForSingleObject(local_98, 0xffffffff);
        }
        CloseHandle(local_98);
    }
}

void FUN_140008c5c(int param_1)
{
    /* @0x140008c5c size=317 service 状态回调 */
    if (param_1 == 1) {
        g_flagA24F = 0;
        if ((uintptr_t)g_u64CA20 != 0) {
            *(int32_t *)(uintptr_t)g_u64CA20 = 0xffffffff;
        }
        if (*(int64_t *)PTR_DAT_14013a040 != 0) {
            PostMessageW(*(HWND *)PTR_DAT_14013a040, 0xbd1, 0xffffffffffffffffULL, -1);
        }
        FUN_1400056bc();
        g_msgRetCode = 1;
    } else if (param_1 == 2) {
        if ((uintptr_t)g_u64CA20 != 0) {
            *(int32_t *)(uintptr_t)g_u64CA20 = 0;
        }
        if (*(int64_t *)PTR_DAT_14013a040 != 0) {
            PostMessageW(*(HWND *)PTR_DAT_14013a040, 0xbd1, 0, 0);
        }
        g_msgRetCode = 7;
    } else {
        if (param_1 != 3) goto LAB_140008d7f;
        if ((uintptr_t)g_u64CA20 != 0) {
            *(int32_t *)(uintptr_t)g_u64CA20 = 1;
        }
        if (*(int64_t *)PTR_DAT_14013a040 != 0) {
            PostMessageW(*(HWND *)PTR_DAT_14013a040, 0xbd1, 1, 1);
        }
        g_msgRetCode = 4;
    }
    SetServiceStatus(g_pSvStatus, (void *)&g_svcStatus);
LAB_140008d7f:
    SetServiceStatus(g_pSvStatus, (void *)&g_svcStatus);
}

void FUN_140008d9c(uint16_t *param_1)
{
    /* @0x140008d9c size=152 */
    (void)param_1;
}

void FUN_140008e34(int64_t *param_1)
{
    /* @0x140008e34 size=107 释放串数组及元素 */
    int iVar1;
    int64_t lVar2;
    int64_t *_Memory;

    lVar2 = *param_1;
    iVar1 = (int)param_1[1];
    while (0 < iVar1) {
        *(int *)(param_1 + 1) = (int)param_1[1] - 1;
        if (*(int64_t *)(lVar2 + (int64_t)(int)param_1[1] * 8) != 0) {
            _Memory = *(int64_t **)(lVar2 + (int64_t)(int)param_1[1] * 8);
            if (_Memory != NULL) {
                FUN_14005B104((WCHAR **)_Memory);
                free(_Memory);
            }
            *(uint64_t *)(lVar2 + (int64_t)(int)param_1[1] * 8) = 0;
        }
        *(int *)(param_1 + 1) = (int)param_1[1] - 1;
        iVar1 = (int)param_1[1];
    }
    FUN_14005B104((WCHAR **)param_1);
}

void PECMD_ScanDirectory(uint64_t *param_1, LPCWSTR param_2, LPCWSTR param_3, int param_4, int param_5)
{
    /* @0x140008ea4 size=449 目录扫描 (FindFirstFileW/FindNextFileW) */
    char cVar1;
    int iVar2;
    BOOL BVar3;
    WCHAR *pWVar4;
    WCHAR WVar5;
    WCHAR *pWVar6;
    LPCWSTR local_288;
    HANDLE local_280;
    WIN32_FIND_DATAW local_278;

    local_278.dwFileAttributes = 0;
    memset(&local_278.ftCreationTime, 0, 0x24c);
    local_280 = (HANDLE)0;
    FUN_140101db8(&local_280, param_2, &local_278);
    if (local_280 != (HANDLE)0) {
        do {
            if ((local_278.dwFileAttributes & 0x10) == 0) {
                pWVar4 = local_278.cFileName + param_4;
                cVar1 = ((char (*)(uint64_t *))*param_1)(param_1);
                pWVar6 = pWVar4;
                if (cVar1 == '\x01') {
                    WVar5 = *pWVar4;
                    while ((WVar5 != L'\0' && (((WVar5 == L'=' || (WVar5 == L'!')) ||
                           (WVar5 == L'@'))))) {
                        pWVar6 = pWVar6 + 1;
                        WVar5 = *pWVar6;
                    }
                }
                FUN_140063620((WCHAR **)&local_288);
                iVar2 = (int)((int64_t)(pWVar6 - pWVar4) >> 1);
                if (0 < iVar2) {
                    PECMD_StrCopyW((WCHAR **)&local_288, pWVar4, (int64_t)iVar2);
                }
                FUN_14006375C((WCHAR **)&local_288, param_3);
                FUN_14006375C((WCHAR **)&local_288, local_278.cFileName);
                FUN_140005818((int64_t *)(param_1 + 2), local_288);
                iVar2 = lstrlenW(pWVar4);
                pWVar4[(int64_t)iVar2 - (int64_t)param_5] = L'\0';
                if (cVar1 == '\x02') {
                    WVar5 = *pWVar6;
                    pWVar4 = pWVar6;
                    while (WVar5 != L'\0') {
                        if (*pWVar4 == L'+') {
                            *pWVar4 = L'\0';
                            break;
                        }
                        pWVar4 = pWVar4 + 1;
                        WVar5 = *pWVar4;
                    }
                }
                FUN_140005818((int64_t *)(param_1 + 2), pWVar6);
                *(int *)(param_1 + 1) = *(int *)(param_1 + 1) + 1;
                FUN_14005B104((WCHAR **)&local_288);
            }
            BVar3 = FindNextFileW(local_280, &local_278);
        } while ((BVar3 != 0) && (local_280 != (HANDLE)0));
        if ((local_280 != (HANDLE)0) && (local_280 != (HANDLE)-1)) {
            FindClose(local_280);
        }
    }
}

uint64_t FUN_1400091e0(LPCWSTR param_1, int64_t *param_2, int64_t *param_3,
                       HANDLE *param_4, LPCWSTR param_5)
{
    /* @0x1400091e0 size=833 (签名修正自联调, 主体仍为 NO-OP stub) */
    (void)param_1; (void)param_2; (void)param_3; (void)param_4; (void)param_5;
    return 0;
}

int64_t PECMD_ParseVarArg(int64_t *param_1, int64_t *param_2, int64_t *param_3,
                      int *param_4, uint32_t *param_5)
{
    /* @0x140009524 size=433 */
    LPCWSTR pWVar1;
    LPCWSTR pWVar2;
    LPCWSTR pWVar3;
    int64_t lVar4;
    LPCWSTR local_res10;
    LPCWSTR local_38;
    LPCWSTR local_30;
    WCHAR *local_28;
    int64_t local_20;

    local_res10 = (LPCWSTR)*param_2;
    lVar4 = 0;
    if (*local_res10 == L':') {
        pWVar1 = local_res10 + 1;
        pWVar3 = pWVar1;
        if (*pWVar1 == L'%') {
            do {
                pWVar2 = pWVar3;
                local_res10 = pWVar2 + 1;
                if (*local_res10 == L'\0') break;
                pWVar3 = local_res10;
            } while (*local_res10 != L'%');
            if (*local_res10 == L'%') {
                local_res10 = pWVar2 + 2;
            }
            local_38 = pWVar1;
            FUN_1400702D4(&local_28, pWVar1,
                          ((int64_t)local_res10 - (int64_t)pWVar1) >> 1);
            local_30 = (LPCWSTR)0x0;
            FUN_14007bf44(param_1, local_28, &local_30, 0, 1);
            local_38 = local_30;
            if (*local_30 == L'*') {
                local_38 = local_30 + 1;
                *param_5 = 1;
            }
            PECMD_ParseUIntValue((WCHAR **)&local_38, param_4);
            FUN_14005B154((WCHAR **)&local_res10);
            FUN_14005B104((WCHAR **)&local_30);
            FUN_14005B104((WCHAR **)&local_28);
        } else {
            if (*pWVar1 == L'*') {
                *param_5 = 1;
                pWVar1 = local_res10 + 2;
            }
            local_res10 = pWVar1;
            PECMD_ParseUIntValue((WCHAR **)&local_res10, param_4);
        }
        if (*local_res10 == L':') {
            local_res10 = local_res10 + 1;
        }
        if (*param_4 < 1) {
            *param_5 = 0;
        }
    }
    *param_2 = (int64_t)local_res10;
    local_20 = 0;
    pWVar3 = (LPCWSTR)FUN_140024c48((int64_t *)&local_res10, &local_20, 1);
    *param_2 = (int64_t)pWVar3;
    if (local_res10 != pWVar3) {
        PECMD_StrCopyW((WCHAR **)param_3, local_res10, local_20);
        lVar4 = *param_3;
    }
    FUN_14005B154((WCHAR **)param_2);
    return lVar4;
}

uint64_t FUN_1400096d8(uint64_t param_1)
{
    /* @0x1400096d8 size=66 查找存活进程号(含当前进程) */
    DWORD DVar1;
    uint64_t uVar2;
    uint64_t uVar3;

    if ((long long)param_1 < 0) {
        DVar1 = GetCurrentProcessId();
        uVar2 = (uint64_t)DVar1;
        uVar3 = param_1;
        do {
            uVar3 = uVar3 + 1;
            param_1 = uVar2 & 0xffffffff;
            if (0 < (long long)uVar3) {
                return param_1;
            }
            uVar2 = PECMD_GetParentProcessId((DWORD)uVar2);
        } while ((int)uVar2 != 0);
    }
    return param_1;
}

short *PECMD_ConvertLfToCrlf(short *param_1, int64_t *param_2)
{
    /* @0x14000971c size=294 LF->CRLF 换行转换 */
    short sVar2;
    short *psVar3;
    int64_t lVar4;
    short *psVar5;
    short *psVar1;

    psVar1 = (short *)(param_2[1] + (int64_t)param_1);
    psVar5 = param_1;
    while (true) {
        if (psVar1 <= psVar5) return param_1;
        if (*psVar5 == 0) return param_1;
        if ((*psVar5 == 10) || (*psVar5 == 0xd)) break;
        psVar5 = psVar5 + 1;
    }
    lVar4 = ((int64_t)psVar5 - (int64_t)param_1) >> 1;
    PECMD_AllocString((WCHAR **)param_2, param_2[1] + 8);
    FUN_14001d78c((uint8_t *)*param_2, (uint8_t *)param_1, (int)lVar4 * 2);
    psVar3 = (short *)(*param_2 + lVar4 * 2);
joined_r0x0001400097be:
    do {
        while (true) {
            if (psVar1 <= psVar5) {
                psVar3[1] = 0;
                *psVar3 = 0;
                param_2[1] = (int64_t)psVar3 - *param_2;
                return (short *)*param_2;
            }
            if (*psVar5 != 0) break;
            *psVar3 = 0;
LAB_14000980c:
            psVar3 = psVar3 + 1;
            psVar5 = psVar5 + 1;
        }
        if (*psVar5 == 10) {
            sVar2 = psVar5[1];
            *psVar3 = 0xd;
            if (sVar2 != 0xd) {
LAB_140009802:
                psVar3 = psVar3 + 1;
LAB_140009806:
                *psVar3 = *psVar5;
                goto LAB_14000980c;
            }
            psVar3[1] = 10;
            psVar3 = psVar3 + 2;
            psVar5 = psVar5 + 2;
            goto joined_r0x0001400097be;
        }
        if (*psVar5 != 0xd) goto LAB_140009806;
        psVar5 = psVar5 + 1;
        sVar2 = *psVar5;
        *psVar3 = 0xd;
        if (sVar2 == 10) goto LAB_140009802;
        psVar3[1] = 10;
        psVar3 = psVar3 + 2;
    } while (true);
}

uint64_t FUN_140009844(uint64_t param_1)
{
    /* @0x140009844 size=150 */
    uint64_t uVar1;
    uint64_t *puVar2;
    uint64_t *puVar3;
    HICON lParam;

    uVar1 = FUN_140006660((HWND)(uintptr_t)param_1);
    puVar2 = (uint64_t *)operator_new(0x160);
    puVar3 = (uint64_t *)0;
    if (puVar2 != (uint64_t *)0) {
        puVar3 = FUN_1400E9048(puVar2, 0x12e, param_1);
    }
    lParam = LoadIconW(g_hInst, (LPCWSTR)(uintptr_t)0x1);
    SendMessageW((HWND)(uintptr_t)puVar3[4], 0x80, 1, (LPARAM)(uintptr_t)lParam);
    SendMessageW((HWND)(uintptr_t)puVar3[4], 0x80, 0, (LPARAM)(uintptr_t)lParam);
    return uVar1;
}

DWORD PECMD_ReadProcessCommandLine(DWORD param_1, uint64_t *param_2, int param_3)
{
    /* @0x1400098dc size=1007 取进程命令行 (跨进程 PEB 读取) */
    DWORD DVar1, DVar2, DVar7;
    int iVar3;
    BOOL BVar4;
    HANDLE hProcess;
    uint64_t uVar5;
    uint32_t uVar6;
    uint32_t local_res10[4];
    size_t local_res20;
    uint16_t *local_128;
    uint64_t local_120;
    LPCVOID local_118[5];
    uint64_t local_f0;
    uint64_t local_e8[3];
    LPCVOID local_d0;
    uint32_t local_b8;
    uint64_t local_b4[13];
    uint16_t local_48;
    LPCVOID local_40;

    FUN_140063694((WCHAR **)&local_128, 0x4002);
    *local_128 = 0;
    DVar7 = 0xffffffff;
    *param_2 = 0;
    hProcess = OpenProcess(0x410, 0, param_1);
    if (hProcess == (HANDLE)0) {
        DVar1 = GetLastError();
        goto LAB_140009ca4;
    }
    uVar6 = (uint32_t)(param_3 - 2);
    local_res10[0] = *(uint32_t *)(GetCommandLineW_exref + 1);
    local_res20 = 0;
    uVar5 = PECMD_GetPackedSystemVersion();
    DVar2 = GetVersion();
    DVar1 = DVar7;
    if ((uint32_t)(uVar5 >> 0x10) < 0x60000) {
        if (DVar2 < 0x80000000) {
            BVar4 = ReadProcessMemory(hProcess, (LPCVOID)(uintptr_t)local_res10[0],
                                      local_res10, 4, &local_res20);
            iVar3 = g_readBufSize;
            if (BVar4 != 0) {
                for (; 0x3ff < iVar3; iVar3 = iVar3 / 2) {
                    BVar4 = ReadProcessMemory(hProcess, (LPCVOID)(uintptr_t)local_res10[0],
                                              local_128, (size_t)(int64_t)iVar3, &local_res20);
                    if (BVar4 != 0) goto LAB_140009c5b;
                }
            }
        } else {
            BVar4 = ReadProcessMemory(hProcess, (LPCVOID)(uintptr_t)local_res10[0],
                                      local_res10, 4, &local_res20);
            if ((BVar4 != 0) &&
                (BVar4 = ReadProcessMemory(hProcess, (LPCVOID)(uintptr_t)local_res10[0],
                                           local_res10, 4, &local_res20),
                 iVar3 = g_readBufSize, BVar4 != 0)) {
                for (; 0x3ff < iVar3; iVar3 = iVar3 / 2) {
                    BVar4 = ReadProcessMemory(hProcess,
                                              (LPCVOID)(uintptr_t)(local_res10[0] + 0xc0),
                                              local_128, (size_t)(int64_t)iVar3, &local_res20);
                    if (BVar4 != 0) {
                        if (*local_128 != 0) goto LAB_140009c5b;
                        BVar4 = ReadProcessMemory(hProcess,
                                                  (LPCVOID)(uintptr_t)(local_res10[0] + 0x40),
                                                  local_res10, 4, &local_res20);
                        if ((BVar4 != 0) &&
                            (BVar4 = ReadProcessMemory(hProcess,
                                                       (LPCVOID)(uintptr_t)(local_res10[0] + 8),
                                                       local_res10, 4, &local_res20),
                             iVar3 = g_readBufSize, BVar4 != 0)) goto LAB_140009c25;
                        break;
                    }
                }
            }
        }
    } else {
        /* NT6+ : NtQueryInformationProcess 读 PEB/进程参数 */
        local_120 = 0;
        memset((void *)local_118, 0, 0x28);
        local_f0 = 0;
        memset(local_e8, 0, 0x30);
        local_b8 = 0;
        memset(local_b4, 0, 0x7c);
        FUN_14005C828("NtQueryInformationProcess", "NTDLL.DLL",
                      (void **)&g_pNtQueryInfo, (HMODULE *)0);
        DVar1 = 0xffffffff;
        /* TODO(verify): PBI/PEB 栈重叠语义(local_120/local_118 共享区) */
        if (((g_pNtQueryInfo != 0) &&
             (iVar3 = ((int (*)(HANDLE, int, void *, uint32_t, void *))(uintptr_t)g_pNtQueryInfo)
                          (hProcess, 0, &local_120, 0x30, 0),
              DVar1 = DVar7, iVar3 == 0)) &&
            (BVar4 = ReadProcessMemory(hProcess, local_118[0], &local_f0, 0x38, (size_t *)0),
             BVar4 != 0) &&
            (BVar4 = ReadProcessMemory(hProcess, local_d0, &local_b8, 0x80, (size_t *)0),
             BVar4 != 0)) {
            if ((uint32_t)(int)local_48 < uVar6) {
                uVar6 = (uint32_t)(int)local_48;
            }
            ReadProcessMemory(hProcess, local_40, param_2, (size_t)uVar6, (size_t *)0);
            *((uint16_t *)((uint8_t *)param_2 + uVar6)) = 0;
            DVar1 = 0;
        }
    }
    goto LAB_140009c98;
LAB_140009c25:
    if (iVar3 < 0x400) goto LAB_140009c98;
    BVar4 = ReadProcessMemory(hProcess, (LPCVOID)(uintptr_t)local_res10[0], local_128,
                              (size_t)(int64_t)iVar3, &local_res20);
    if (BVar4 != 0) goto LAB_140009c5b;
    iVar3 = iVar3 / 2;
    goto LAB_140009c25;
LAB_140009c5b:
    if (uVar6 <= local_res20) {
        local_res20 = (size_t)uVar6;
    }
    g_readBufSize = iVar3;
    FUN_14001d78c((uint8_t *)param_2, (const uint8_t *)local_128, (int)local_res20);
    *((uint16_t *)((uint8_t *)param_2 + local_res20)) = 0;
    DVar1 = 0;
LAB_140009c98:
    CloseHandle(hProcess);
LAB_140009ca4:
    FUN_14005B104((WCHAR **)&local_128);
    return DVar1;
}

uint64_t PECMD_MainEntryPoint(HINSTANCE param_1, uint64_t param_2, LPCWSTR param_3,
                       uint32_t param_4)
{
    /* @0x14000a14c size=885 */
    bool bVar1;
    LPCWSTR pWVar2;
    int iVar3;
    LPWSTR pWVar4;
    uint64_t uVar5;
    int64_t lVar6;
    uint64_t uVar7;
    LPCWSTR pWVar8;
    uint8_t bVar9;
    LPCWSTR local_res18[2];
    LPCWSTR local_58;
    WCHAR *local_50;
    LPCWSTR local_48;
    WCHAR *local_40;
    LPCWSTR local_38[2];
    (void)param_2;

    local_res18[0] = param_3;
    FUN_1400051b4();
    if ((g_hInstance == (HINSTANCE)0x0) && (param_1 != (HINSTANCE)0x0)) {
        g_hInstance = param_1;
    }
    bVar9 = 0;
    pWVar4 = GetCommandLineW();
    FUN_1400702B0((WCHAR **)local_38, pWVar4);
    local_res18[0] = local_38[0];
    FUN_14005B154((WCHAR **)local_res18);
    PECMD_InitEnvironmentVars(param_1, param_4);
    pWVar2 = local_res18[0];
    local_res18[0] = (LPCWSTR)FUN_140024c48((int64_t *)local_res18, (int64_t *)0x0, 5);
    do {
        if (local_res18[0] == (LPCWSTR)0x0) {
            uVar7 = 0xffffffff;
LAB_14000a447:
            FUN_14005B104((WCHAR **)local_38);
            return uVar7 & 0xffffffff;
        }
        local_58 = local_res18[0];
        FUN_14005B154((WCHAR **)&local_58);
        pWVar8 = local_58;
        if (*local_58 == L'\"') {
            pWVar8 = local_58 + 1;
        }
        uVar5 = (uint64_t)FUN_14005C788("EXEC", (const WCHAR *)pWVar8, 4);
        if (((char)uVar5 != '\0') &&
            ((((uint16_t)pWVar8[4] > 8) && ((uint16_t)pWVar8[4] < 0xe)) ||
             (pWVar8[4] == L' '))) {
            PECMD_AllocString(&g_szMsgBuf, 0x6592);
            g_szMsgBuf[0] = L'\0';
            LoadStringW(param_1, 0x2713, g_szMsgBuf, 26000);
            iVar3 = lstrlenW(g_szMsgBuf);
            PECMD_AllocString(&g_szMsgBuf, (int64_t)iVar3 + 2);
            FUN_1400702B0(&local_50, pWVar8);
            FUN_1400084d0((short *)local_50);
            FUN_14005B104(&local_50);
            g_runFlag = 0;
            FUN_14005B0D4(&g_szMsgBuf);
        }
        uVar5 = (uint64_t)FUN_14005C788("MAIN", (const WCHAR *)local_58, 4);
        if (((char)uVar5 != '\0') &&
            (((pWVar8 = local_58 + 4, ((uint16_t)*pWVar8 > 8) &&
              ((uint16_t)*pWVar8 < 0xe)) ||
              ((*pWVar8 == L' ') || (*pWVar8 == L'*'))))) {
            if ((((uint16_t)*pWVar8 > 8) && ((uint16_t)*pWVar8 < 0xe)) ||
                (bVar1 = false, *pWVar8 == L' ')) {
                bVar1 = true;
            }
            g_flag16a = 0;
            local_48 = pWVar8;
            FUN_14005B154((WCHAR **)&local_48);
            if ((bVar1) &&
                (lVar6 = FUN_14005c72c("**u", (const WCHAR *)local_48, 3),
                 (char)lVar6 != '\0'))
                goto LAB_14000a382;
            FUN_1400702B0(&local_40, local_58 + 4);
            FUN_140045c90((int64_t *)(uintptr_t)g_Script, (void *)local_40);
            FUN_14005B104(&local_40);
LAB_14000a49a:
            if (g_afterMain != '\0') {
                FUN_14002ca30();
            }
            uVar7 = FUN_14005b7dc();
            FUN_14005B21C((UINT)uVar7);
            goto LAB_14000a447;
        }
LAB_14000a382:
        if (bVar9 < 2) {
            PECMD_CreateHelperThread(g_b13caf8);
        }
        g_flag169 = g_state138;
        FUN_140004fd4(pWVar2);
        FUN_14004eaa8((int64_t)(uintptr_t)g_Script, 0);
        FUN_1400637dc((WCHAR **)g_Env, (LPCWSTR)g_DefEnv, (int64_t)-1, (int64_t)-1);
        g_state190 = 0;
        g_state198 = 0;
        g_runFlag = 0;
        g_runMode = 0x20;
        if (g_pNextCmd == (LPCWSTR)0x0) goto LAB_14000a49a;
        g_flagA24F = 1;
        local_res18[0] = g_pNextCmd;
        bVar9 = 2;
        PECMD_InitEnvironmentVars(param_1, param_4);
        pWVar2 = local_res18[0];
        local_res18[0] = (LPCWSTR)FUN_140024c48((int64_t *)local_res18, (int64_t *)0x0, 5);
    } while (1);
}

void FUN_14000a4c4(void *param_1, char param_2, void *param_3, char param_4)
{
    /* @0x14000a4c4 size=190 终止进程/作业并(可选)取消系统关机和关闭 */
    uint64_t *p1;
    uint64_t *p3;
    int iVar1;

    p1 = (uint64_t *)param_1;
    p3 = (uint64_t *)param_3;
    TerminateProcess((HANDLE)p1[0], 0);
    g_pShutdownFlag = 0;
    CloseHandle((HANDLE)p1[0]);
    p1[0] = 0;
    if ((param_3 != (void *)0x0) && ((HANDLE)p3[0] != (HANDLE)0x0)) {
        TerminateJobObject((HANDLE)p3[0], 0);
        CloseHandle((HANDLE)p3[0]);
        p3[0] = 0;
    }
    if (param_4 != '\0') {
        FUN_140008710(WSTR("shutdown -a"), '\0', 1);
    }
    FUN_14000551c((LPCSTR)g_pPendingObj);
    if (param_2 != '\0') {
        iVar1 = 2;
        do {
            AbortSystemShutdownW((LPWSTR)0x0);
            Sleep(1000);
            iVar1 = iVar1 + -1;
        } while (-1 < iVar1);
    }
}

DWORD FUN_14000a584(LPCWSTR param_1, int param_2)
{
    /* @0x14000a584 size=6520 */
    (void)param_1;
    (void)param_2;
    return 0;
}

uint64_t FUN_14000befc(uint16_t *param_1)
{
    /* @0x14000befc size=208 service 主循环 */
    uint16_t *puVar1;
    LPCWSTR pWVar2;
    int iVar3;
    char cVar4;

    g_runFlag = 0;
    g_LastTick = GetTickCount();
    /* PECMD_SkipTokenAndSpaces 为 void, 但调用方依赖其 eax 残留(跳过 token 后指针) */
    puVar1 = PECMD_SkipLeadingWhitespace(FUN_14000546c(PECMD_SkipLeadingWhitespace(param_1)));
    puVar1 = PECMD_SkipLeadingWhitespace(FUN_14000546c(PECMD_SkipLeadingWhitespace(puVar1)));
    pWVar2 = (LPCWSTR)PECMD_SkipLeadingWhitespace(FUN_14000546c(PECMD_SkipLeadingWhitespace(puVar1)));
    g_svcCmdStr = pWVar2;
    FUN_140008d9c((uint16_t *)pWVar2);
    g_flagA24F = 2;
    cVar4 = 1;
    do {
        if (g_flagA24F == 0) {
            Sleep(10);
        } else {
            FUN_14000a584(pWVar2, (int)cVar4);
            cVar4 = 2;
            iVar3 = 9;
            do {
                if (g_flagA24F < 1) break;
                Sleep(100);
                iVar3 = iVar3 - 1;
            } while (-1 < iVar3);
        }
        if (g_flagA24F < 0) {
            g_flagA24F = 0xfe;
            PECMD_UnmapFileView();
            return 0;
        }
    } while (1);
}

uint32_t PECMD_SearchComObject(uint64_t param_1, uint64_t param_2, LPCWSTR param_3)
{
    /* @0x14000bfcc size=1121 */
    int iVar1;
    int iVar2;
    int64_t lVar3;
    int64_t lVar4;
    LPWSTR pWVar5;
    LPCWSTR pWVar6;
    uint32_t uVar7;
    uint32_t uVar8;
    uint32_t uVar9;
    WCHAR local_res18[4];
    int local_res20[2];
    int64_t *local_108;
    int64_t *local_100;
    LPCWSTR local_f8;
    int64_t *local_f0;
    int64_t *local_e8;
    int64_t *local_e0;
    LPCWSTR local_d8;
    uint16_t local_d0;
    uint8_t uStack_ce[6];
    int64_t local_c8;
    uint64_t local_c0;
    LPCWSTR local_b8;
    uint64_t local_a8;
    int64_t local_a0;
    uint64_t local_98;
    int64_t local_88[10];

    EnterCriticalSection(&g_csCom);
    FUN_140061C44();
    uVar7 = 0;
    iVar1 = -3;
    if (((g_pOleInit != (void *)0x0) &&
         (iVar1 = ((int (*)(int))g_pOleInit)(0), iVar1 != 0)) && (iVar1 == 1)) {
        ((void (*)(void))g_pOleUninit)();
    }
    PECMD_DelayLoadOleaut32();
    local_res20[0] = 0;
    uVar8 = 0;
    if (*param_3 == L'|') {
        param_3 = param_3 + 1;
        uVar8 = 2;
    }
    lstrlenW(param_3);
    PECMD_NormalizeQuoteChars((int16_t *)param_3, 1);
    local_f8 = (LPCWSTR)0x0;
    local_b8 = param_3;
    if (uVar8 < 2) {
        FUN_1400703E4((WCHAR **)&local_f8, param_3);
        param_3 = local_f8;
        PECMD_NormalizeQuoteChars((int16_t *)local_f8, 0);
    }
    local_f0 = (int64_t *)0x0;
    ((int (*)(void *, void *, uint32_t, void *, void **))g_pCoCreateInstance)
        (&g_u64d188, 0, 0x15, &g_u64d198, (void **)&local_f0);
    local_e8 = (int64_t *)0x0;
    lVar3 = ((int64_t (*)(LPCWSTR))g_pSysAllocString)((LPCWSTR)param_1);
    local_d0 = 8;
    local_a8 = (uint64_t)(uint16_t)8;
    local_98 = local_c0;
    local_c8 = lVar3;
    local_a0 = lVar3;
    PECMD_VT_CALL(local_f0, 0x48, void (*)(int64_t *, int64_t *, int64_t **),
                  local_f0, &local_a8, &local_e8);
    local_e0 = (int64_t *)0x0;
    lVar4 = ((int64_t (*)(LPCWSTR))g_pSysAllocString)((LPCWSTR)param_2);
    PECMD_VT_CALL(local_e8, 0x60, void (*)(int64_t *, int64_t, int64_t **),
                  local_e8, lVar4, &local_e0);
    local_100 = (int64_t *)0x0;
    PECMD_VT_CALL(local_e0, 0xb0, void (*)(int64_t *, int64_t **),
                  local_e0, &local_100);
    PECMD_VT_CALL(local_100, 0x38, void (*)(int64_t *, int *),
                  local_100, local_res20);
    local_res18[0] = L'\0';
    iVar2 = local_res20[0];
    if (uVar8 < 3) {
        do {
            uVar9 = 0;
            if (0 < iVar2) {
                do {
                    local_d0 = 3;
                    local_c8 = (local_c8 & 0xffffffff00000000ULL) | (uint64_t)uVar9;
                    local_a8 = (uint64_t)(uint16_t)3;
                    local_108 = (int64_t *)0x0;
                    local_a0 = local_c8;
                    local_98 = local_c0;
                    PECMD_VT_CALL(local_100, 0x50, void (*)(int64_t *, int64_t *, int64_t **),
                                  local_100, &local_a8, &local_108);
                    local_d8 = (LPCWSTR)0x0;
                    PECMD_VT_CALL(local_108, 0x48, void (*)(int64_t *), local_108);
                    pWVar6 = local_d8;
                    PECMD_NormalizeQuoteChars((int16_t *)local_d8, 0);
                    local_88[0] = 1;
                    local_88[1] = 0;
                    local_88[2] = 0x3f;
                    local_88[3] = 0;
                    memset(local_88 + 4, 0, 0x30);
                    if ((((uVar8 == 0) &&
                          (pWVar5 = (LPWSTR)(uintptr_t)StrStrW(pWVar6, param_3),
                           pWVar5 != (LPWSTR)0x0)) ||
                         ((uVar8 == 1) &&
                          (pWVar5 = (LPWSTR)(uintptr_t)StrStrIW(pWVar6, param_3),
                           pWVar5 != (LPWSTR)0x0))) ||
                        (pWVar6 = FUN_14005182c(local_88, param_3, local_res18, pWVar6,
                                                (WCHAR *)0x0, (int64_t *)(uintptr_t)g_Script),
                         pWVar6 != (LPCWSTR)0x0)) {
                        PECMD_VT_CALL(local_108, 0x50, void (*)(int64_t *), local_108);
                        if (local_108 != (int64_t *)0x0) {
                            PECMD_VT_CALL(local_108, 0x10, void (*)(int64_t *), local_108);
                        }
                        if (local_100 != (int64_t *)0x0) {
                            PECMD_VT_CALL(local_100, 0x10, void (*)(int64_t *), local_100);
                        }
                        if (lVar4 != 0) {
                            ((void (*)(int64_t))g_pSysFreeString)(lVar4);
                        }
                        if (local_e0 != (int64_t *)0x0) {
                            PECMD_VT_CALL(local_e0, 0x10, void (*)(int64_t *), local_e0);
                        }
                        if (lVar3 != 0) {
                            ((void (*)(int64_t))g_pSysFreeString)(lVar3);
                        }
                        if (local_e8 != (int64_t *)0x0) {
                            PECMD_VT_CALL(local_e8, 0x10, void (*)(int64_t *), local_e8);
                        }
                        if (local_f0 != (int64_t *)0x0) {
                            PECMD_VT_CALL(local_f0, 0x10, void (*)(int64_t *), local_f0);
                        }
                        FUN_14005B104((WCHAR **)&local_f8);
                        if ((g_pOleUninit != (void *)0x0) && (iVar1 == 0)) {
                            ((void (*)(void))g_pOleUninit)();
                        }
                        goto LAB_14000c361;
                    }
                    if (local_108 != (int64_t *)0x0) {
                        PECMD_VT_CALL(local_108, 0x10, void (*)(int64_t *), local_108);
                    }
                    uVar9 = uVar9 + 1;
                    iVar2 = local_res20[0];
                } while ((int)uVar9 < local_res20[0]);
            }
            if (uVar8 == 1) {
                param_3 = local_b8;
            }
            uVar8 = uVar8 + 1;
        } while ((int)uVar8 < 3);
    }
    if (local_100 != (int64_t *)0x0) {
        PECMD_VT_CALL(local_100, 0x10, void (*)(int64_t *), local_100);
    }
    if (lVar4 != 0) {
        ((void (*)(int64_t))g_pSysFreeString)(lVar4);
    }
    if (local_e0 != (int64_t *)0x0) {
        PECMD_VT_CALL(local_e0, 0x10, void (*)(int64_t *), local_e0);
    }
    if (lVar3 != 0) {
        ((void (*)(int64_t))g_pSysFreeString)(lVar3);
    }
    if (local_e8 != (int64_t *)0x0) {
        PECMD_VT_CALL(local_e8, 0x10, void (*)(int64_t *), local_e8);
    }
    if (local_f0 != (int64_t *)0x0) {
        PECMD_VT_CALL(local_f0, 0x10, void (*)(int64_t *), local_f0);
    }
    FUN_14005B104((WCHAR **)&local_f8);
    if ((g_pOleUninit != (void *)0x0) && (iVar1 == 0)) {
        ((void (*)(void))g_pOleUninit)();
    }
    uVar7 = 1;
LAB_14000c361:
    LeaveCriticalSection(&g_csCom);
    return uVar7;
}

int64_t PECMD_EnumComMenuItems(uint64_t param_1, uint64_t param_2, int *param_3,
                      int64_t **param_4, HMENU param_5)
{
    /* @0x14000c430 size=814 COM 枚举子项 -> 菜单/字符串表 */
    LPCWSTR pWVar1;
    int iVar2;
    int64_t lVar3;
    int64_t lVar4;
    int iVar5;
    int iVar6;
    int local_98[2];
    int64_t *local_90;
    int64_t *local_88;
    WCHAR local_80[4];
    int64_t *local_78;
    int64_t *local_70;
    int64_t *local_68;
    LPCWSTR local_60;
    uint64_t args[5];   /* TODO(verify): COM 方法参数结构(Ghidra 栈布局 5×8B) */
    uint64_t local_48;
    uint64_t local_50;

    EnterCriticalSection((LPCRITICAL_SECTION)&g_csCom);
    FUN_140061C44();
    iVar5 = 0;
    iVar2 = -3;
    iVar6 = 0;
    if ((g_pOleInit != (void *)0) &&
        (iVar2 = ((int (*)(int))g_pOleInit)(0), iVar2 != 0) && (iVar2 == 1)) {
        ((void (*)(void))g_pOleUninit)();
    }
    PECMD_DelayLoadOleaut32();
    local_88 = (int64_t *)0;
    ((int (*)(void *, int, int, void *, int64_t **))g_pCoCreateInstance)(
        (void *)&g_u64d188, 0, 0x15, (void *)&g_u64d198, &local_88);
    local_90 = (int64_t *)0;
    lVar3 = ((int64_t (*)(uint64_t))g_pSysAllocString)(param_1);
    local_50 = (uint64_t)lVar3;
    args[0] = 8;
    args[1] = (uint64_t)lVar3;   /* local_50 */
    args[2] = (uint64_t)lVar3;   /* local_40 */
    args[3] = (uint64_t)lVar3;   /* local_30 */
    args[4] = local_48;          /* local_28 */
    ((void (*)(int64_t *, uint64_t *, int64_t **))
     ((uint64_t *)(uintptr_t)*local_88)[0x48 / 8])(local_88, args, &local_90);
    if (local_90 == (int64_t *)0) {
        if (lVar3 == 0) goto LAB_14000c705;
LAB_14000c6ec:
        ((void (*)(int64_t))g_pSysFreeString)(lVar3);
    } else {
        local_78 = (int64_t *)0;
        lVar4 = ((int64_t (*)(uint64_t))g_pSysAllocString)(param_2);
        ((void (*)(int64_t *, int64_t, int64_t **))
         ((uint64_t *)(uintptr_t)*local_90)[0x60 / 8])(local_90, lVar4, &local_78);
        if (local_78 == (int64_t *)0) {
            if (lVar4 != 0) {
LAB_14000c6ce:
                ((void (*)(int64_t))g_pSysFreeString)(lVar4);
                goto LAB_14000c6d7;
            }
        } else {
            local_70 = (int64_t *)0;
            ((void (*)(int64_t *, int64_t **))
             ((uint64_t *)(uintptr_t)*local_78)[0xb0 / 8])(local_78, &local_70);
            local_98[0] = 0;
            ((void (*)(int64_t *, int *))
             ((uint64_t *)(uintptr_t)*local_70)[0x38 / 8])(local_70, local_98);
            local_80[0] = L'\r';
            local_80[2] = 0;
            local_80[1] = 10;
            if (0 < local_98[0]) {
                do {
                    local_50 = (local_50 & 0xffffffff00000000ULL) | (uint64_t)(uint32_t)iVar5;
                    local_68 = (int64_t *)0;
                    args[0] = 3;
                    args[1] = local_50;
                    args[2] = (uint64_t)lVar3;   /* local_40 未变 */
                    args[3] = local_50;          /* local_30 */
                    args[4] = local_48;
                    ((void (*)(int64_t *, uint64_t *, int64_t **))
                     ((uint64_t *)(uintptr_t)*local_70)[0x50 / 8])(local_70, args, &local_68);
                    local_60 = (LPCWSTR)0;
                    ((void (*)(int64_t *, LPCWSTR *))
                     ((uint64_t *)(uintptr_t)*local_68)[0x48 / 8])(local_68, &local_60);
                    pWVar1 = local_60;
                    if (param_5 != (HMENU)0) {
                        AppendMenuW(param_5, (~(unsigned int)(*local_60 != L'\0') & 0x800),
                                    (uintptr_t)(iVar5 + 0x100), local_60);
                    }
                    if (param_4 != (int64_t **)0) {
                        if (*(int16_t *)(uintptr_t)*param_4 != 0) {
                            FUN_14006375c((WCHAR **)param_4, local_80);
                        }
                        FUN_14006375c((WCHAR **)param_4, pWVar1);
                    }
                    if (local_68 != (int64_t *)0) {
                        ((void (*)(int64_t *))((uint64_t *)(uintptr_t)*local_68)[0x10 / 8])(local_68);
                    }
                    iVar6 = iVar5 + 1;
                    iVar5 = iVar6;
                } while (iVar6 < local_98[0]);
            }
            *param_3 = iVar6;
            if (local_70 != (int64_t *)0) {
                ((void (*)(int64_t *))((uint64_t *)(uintptr_t)*local_70)[0x10 / 8])(local_70);
            }
            if (lVar4 != 0) goto LAB_14000c6ce;
LAB_14000c6d7:
            if (local_78 != (int64_t *)0) {
                ((void (*)(int64_t *))((uint64_t *)(uintptr_t)*local_78)[0x10 / 8])(local_78);
            }
        }
        if (lVar3 != 0) goto LAB_14000c6ec;
    }
    if (local_90 != (int64_t *)0) {
        ((void (*)(int64_t *))((uint64_t *)(uintptr_t)*local_90)[0x10 / 8])(local_90);
    }
LAB_14000c705:
    if (local_88 != (int64_t *)0) {
        ((void (*)(int64_t *))((uint64_t *)(uintptr_t)*local_88)[0x10 / 8])(local_88);
    }
    if ((g_pOleUninit != (void *)0) && (iVar2 == 0)) {
        ((void (*)(void))g_pOleUninit)();
    }
    LeaveCriticalSection((LPCRITICAL_SECTION)&g_csCom);
    return (int64_t)0xffffffff80004005;
}

uint8_t FUN_14000c764(LPWSTR param_1)
{
    /* @0x14000c764 size=1909 (stub; 真实签名 undefined1(LPWSTR)) */
    (void)param_1;
    return 0;
}

void FUN_14000cedc(WCHAR *param_1, int64_t *param_2)
{
    /* @0x14000cedc size=961 (stub; 真实签名 void(WCHAR*, int64_t*)) */
    (void)param_1;
    (void)param_2;
}

uint8_t PECMD_ParseEnvSwitches(const WCHAR *s, int64_t *param_2, short param_3)
{
    /* @0x14000d2a0 size=3609 环境变量 token-switch 解析器
     * (type-propagation 警告; _FILETIME 指针/标量复用已按语义拆分) */
    int iVar4, iVar5, iVar14;
    uint8_t uVar12;
    short sVar11;
    uint16_t uVar3;
    uint8_t bVar15;
    uint64_t v7, v10;
    uint64_t local_138;
    uint64_t num;
    uint64_t moff;
    uint64_t u6;
    uint8_t *optr;
    WCHAR *pbuf;
    WCHAR *p;
    WCHAR *local_128;
    WCHAR *w120;
    WCHAR *t13;
    uint64_t v120;
    uint64_t *fnp;
    WCHAR local_108[104];
    int local_res18[2];
    WCHAR **ppWVar8;

    v7 = 0;
    uVar12 = 0;
    if (param_3 == 0x26) {
        v7 = 3;
    }
    p = (WCHAR *)s;
    FUN_14005B154(&p);
    iVar14 = 1;
    sVar11 = param_3;
    do {
        v10 = v7;
        if (*p == 0x26) {
            sVar11 = 0x26;
        }
        while (*p == 0x26) {
            v10 = (uint32_t)v10 + 1;
            p++;
        }
        /* FORCELOCAL */
        if ((sVar11 == 0x26) && (FUN_14005C788("FORCELOCAL", p, 10) != 0) &&
            ((p[10] == 0x3d) || (8 < p[10] && p[10] < 0xe) ||
             (p[10] == 0x20) || (p[10] == 0))) {
            p += 10;
            FUN_14005B154(&p);
            if (*p == 0x3d) {
                p++;
                FUN_14005B154(&p);
                if ((*p & 0x30) == 0x30) {
                    if (2 < (uint32_t)v10) {
                        *(uint8_t *)((char *)param_2 + 0xd) = *p & 1;
                    }
                    goto D896;
                }
            }
            goto DCCA;
        }
        /* EnviMode(命中->else 分支) */
        if (FUN_14005C788("EnviMode", p, 8) == 0 ||
            !((p[8] == 0x3d) || (8 < p[8] && p[8] < 0xe) ||
              (p[8] == 0x20) || (p[8] == 0))) {
            /* ---- 大 token-switch: 非 EnviMode ---- */
            /* EXPORTLOCAL */
            if ((FUN_14005C788("EXPORTLOCAL", p, 0xb) != 0) &&
                ((p[11] == 0x3d) || (8 < p[11] && p[11] < 0xe) ||
                 (p[11] == 0x20) || (p[11] == 0))) {
                p += 11;
                FUN_14005B154(&p);
                if (*p == 0x3d) {
                    p++;
                    FUN_14005B154(&p);
                    if (*p == 0x31) g_flagCCC9 = 1;
                    if (*p == 0x30) g_flagCCC9 = 0;
                    if (*p == 0x26) {
                        if (p[1] == 0x31) *(uint8_t *)((char *)param_2 + 0xc) = 1;
                        if ((*p == 0x26) && (p[1] == 0x30))
                            *(uint8_t *)((char *)param_2 + 0xc) = 0;
                    }
                }
                goto D896;
            }
            /* __arg= */
            if (FUN_14005C788("__arg=", p, 6) != 0) {
                if (p[6] == 0x30) *(uint8_t *)((char *)param_2 + 0x4a) = 0;
                else if (p[6] == 0x31) *(uint8_t *)((char *)param_2 + 0x4a) = 1;
                goto D896;
            }
            /* logs_ln= */
            if (FUN_14005C788("logs_ln=", p, 8) != 0) {
                if (p[8] == 0x30) *(uint8_t *)(param_2 + 2) &= 0xbf;
                else if (p[8] == 0x31) *(uint8_t *)(param_2 + 2) |= 0x40;
                goto D896;
            }
            /* logs_np= */
            if (FUN_14005C788("logs_np=", p, 8) != 0) {
                if (p[8] == 0x30) *(uint8_t *)(param_2 + 2) &= 0xf7;
                else if (p[8] == 0x31) *(uint8_t *)(param_2 + 2) |= 8;
                goto D896;
            }
            /* HelpColor= */
            if (FUN_14005C788("HelpColor=", p, 10) != 0) {
                p += 10;
                FUN_14005B154(&p);
                FUN_140063620(&pbuf);
                FUN_14007a224(param_2, p, &pbuf, 0, 1);
                p = pbuf;
                if (*pbuf == L'*') {
                    p = pbuf + 1;
                    local_res18[0] = g_helpWord;
                    PECMD_ParseUIntValue(&p, local_res18);
                    g_helpWord = local_res18[0];
                }
                v120 = ((uint64_t)g_helpHi32 << 32) | (uint32_t)g_u32A22C;
                FUN_1400677b0(&p, (int64_t)(uintptr_t)&v120);
                g_u32A22C = (uint32_t)v120;
                g_helpHi32 = (uint32_t)(v120 >> 32);
                FUN_14005B104(&pbuf);
                return 1;
            }
            /* Arg= */
            if (FUN_14005C788("Arg=", p, 4) != 0) {
                p += 4;
                PECMD_TokenizeList(param_2, (short *)(p + (*p == 0x2a)),
                              (int)(*p == 0x2a));
                return 1;
            }
            /* Clipboard= */
            if (FUN_14005C788("Clipboard=", p, 10) != 0) {
                PECMD_SetClipboardUnicode((LPCWSTR)(p + 10));
                goto D896;
            }
            /* Clipboard?= */
            if (FUN_14005C788("Clipboard?=", p, 0xb) != 0) {
                p += 11;
                FUN_14005B154(&p);
                FUN_140063620(&pbuf);
                FUN_140078E90(&pbuf);
                FUN_1400629B8(param_2, (LPCWSTR)p, (LPCWSTR)pbuf);
                FUN_14005B104(&pbuf);
                return 1;
            }
            /* QueryCmd= */
            if (FUN_14005C788("QueryCmd=", p, 9) != 0) {
                if (g_popmenuFlag != 0) {
                    g_popmenuFlag = (p[9] == 0x31);
                }
                goto D896;
            }
            /* LoadPlugin= */
            if (FUN_14005C788("LoadPlugin=", p, 0xb) != 0) {
                if (g_flagCB08 != 0) {
                    return 1;
                }
                g_flagCB08 = FUN_14000C764((LPWSTR)(p + 11));
                return 1;
            }
            /* Alias */
            if (FUN_1400660ac("Alias", &p, 5) != 0) {
                FUN_14000cedc(p, param_2);
                return 1;
            }
            /* WndProc */
            if (FUN_14005C788("WndProc", p, 7) != 0) {
                p += 7;
                if ((*p == L':') || (*p == L'=')) {
                    p++;
                }
                local_res18[0] = 1;
                PECMD_ParseUIntValue(&p, local_res18);
                if ((0 < local_res18[0]) &&
                    (iVar14 = local_res18[0], 2 < local_res18[0])) {
                    iVar14 = 3;
                }
                local_res18[0] = iVar14 - 1;
                bVar15 = (WCHAR)(*p | 0x20) != 99;
                if (!bVar15) {
                    p++;
                }
                t13 = NULL;
                if ((*p == L',') || (*p == L';')) {
                    t13 = p + 1;
                    p = t13;
                    FUN_1400f429c(&p, 0x2c);
                    if (*p != L'\0') {
                        *p = L'\0';
                        p++;
                    }
                }
                iVar14 = lstrcmpW(WSTR("=0"), p);
                if (t13 != NULL) {
                    if (bVar15) {
                        fnp = (uint64_t *)PTR_WndProc1_14013a008[local_res18[0]];
                    } else {
                        fnp = (uint64_t *)PTR_FUN_14013a020[local_res18[0]];
                    }
                    FUN_1400668ec(param_2, (uint64_t)(uintptr_t)fnp,
                                  (LPCWSTR)t13, WSTR("0x%I64X"));
                }
                g_u64c9d0[local_res18[0]] =
                    (uint64_t)(iVar14 != 0 ? -(int64_t)1 : 0) &
                    (uint64_t)(uintptr_t)param_2;
                return 1;
            }
            /* TextDelay= / EnviBroad= / memvar= / LoadEnvi / zero= / DisX64= */
            if (FUN_14005C788("TextDelay=", p, 10) == 0) {
                if (FUN_14005C788("EnviBroad=", p, 10) == 0) {
                    if (FUN_14005C788("memvar=", p, 7) == 0) {
                        /* LoadEnvi */
                        if (FUN_1400660ac("LoadEnvi", &p, -1) != 0) {
                            pbuf = p;
                            uVar3 = *p;
                            if (uVar3 != 0) goto DD0F;
                            break;
                        }
                        /* zero= */
                        if (FUN_14005C788("zero=", p, 5) != 0) {
                            p += 5;
                            num &= 0xffffffff00000000ULL;
                            iVar4 = PECMD_ParseIntThenSkip(&p, (int *)&num);
                            if (0 < iVar4) {
                                *(uint8_t *)((char *)param_2 + 0x11) &= 0xfe;
                                *(uint8_t *)((char *)param_2 + 0x11) |=
                                    ((uint32_t)num != 0);
                            }
                            goto DCCA;
                        }
                        /* DisX64= */
                        if (FUN_14005C788("DisX64=", p, 7) == 0) {
                            return uVar12;
                        }
                        num &= 0xffffffff00000000ULL;
                        local_138 = 0;
                        FUN_140063620(&local_128);
                        p += 7;
                        local_108[0] = L'\0';
                        local_res18[0] = 0;
                        iVar5 = PECMD_ParseIntThenSkip(&p, local_res18);
                        iVar4 = 0;
                        if (0 < iVar5) {
                            if (local_res18[0] < 1) {
                                PECMD_ParseIntThenSkip(&p, (int *)&num);
                                FUN_1400a9a84(&p, &local_138);
                            } else {
                                FUN_1400547bc(param_2, &p, &local_128, 0x2c, 0);
                                if (*local_128 == L'\0') goto DCA1;
                                FUN_14005B154(&p);
                            }
                            EnterCriticalSection((LPCRITICAL_SECTION)&g_csInit);
                            if (-1 < local_res18[0]) {
                                u6 = FUN_140001188();
                                num = (uint32_t)(int32_t)(int8_t)(uint8_t)u6;
                                wsprintfW(local_108, WSTR("-1;%u;%p"),
                                          (uint32_t)num, (void *)0);
                            }
                            LeaveCriticalSection((LPCRITICAL_SECTION)&g_csInit);
                            iVar4 = iVar14;
                            uVar12 = 1;
                        }
DCA1:
                        FUN_1400629B8(param_2, (LPCWSTR)local_128,
                                      (LPCWSTR)local_108);
                        ppWVar8 = &local_128;
                        if (iVar4 == 0) {
                            FUN_14005B104(&local_128);
                            return uVar12;
                        }
                    } else {
                        /* memvar= */
                        moff = 0;
                        num = 0;
                        local_138 = 1;
                        FUN_140063620(&w120);
                        bVar15 = (p[7] != L'?');
                        p += 7;
                        if (!bVar15) {
                            p++;
                            FUN_1400547bc(param_2, &p, &w120, 0x2c, 0);
                        }
                        if (*p == L':') {
                            p++;
                            FUN_1400c1194(&p, &local_138);
                            if (*p != 0) p++;
                        }
                        FUN_1400745c8(&p, &moff);
                        if (*p != 0) p++;
                        optr = (uint8_t *)&DAT_140147001[0] + (uintptr_t)moff;
                        if (bVar15) {
                            /* 读模式: 写 num 到 *optr */
                            if (local_138 < 8) {
                                if (local_138 < 4) {
                                    if (local_138 < 2)
                                        *(uint8_t *)optr = (uint8_t)num;
                                    else
                                        *(uint16_t *)optr = (uint16_t)num;
                                } else {
                                    *(uint32_t *)optr = (uint32_t)num;
                                }
                            } else {
                                *(uint64_t *)optr = num;
                            }
                        } else {
                            /* 写模式: 读 *optr 到 num 并格式化 */
                            if (local_138 < 8) {
                                if (local_138 < 4) {
                                    if (local_138 < 2)
                                        num = (uint8_t)(*(uint8_t *)optr);
                                    else
                                        num = (uint16_t)(*(uint16_t *)optr);
                                } else {
                                    num = (uint32_t)(*(uint32_t *)optr);
                                }
                            } else {
                                num = *(uint64_t *)optr;
                            }
                            FUN_1400668ec(param_2, num, (LPCWSTR)w120,
                                          WSTR("0x%X"));
                        }
                        ppWVar8 = &w120;
                    }
                    FUN_14005B104(ppWVar8);
                } else {
                    /* EnviBroad= */
                    num &= 0xffffffff00000000ULL;
                    p += 10;
                    if (*p == 0x2d) {
                        g_flagCCB3 = *(uint8_t *)p;
                        goto D896;
                    }
                    FUN_1400c11c0(&p, (int *)&num);
                    g_flagCCB3 = ((uint32_t)num == 0);
                }
            } else {
                /* TextDelay= */
                p += 10;
                num &= 0xffffffff00000000ULL;
                PECMD_ParseUIntValue(&p, (int *)&num);
                g_u8A24A = (uint8_t)((int32_t)(uint32_t)num / 10);
                if (0xff < (int32_t)(uint32_t)num / 10) {
                    g_u8A24A = 0xff;
                }
            }
        } else {
            /* EnviMode 命中 */
            p += 8;
            FUN_14005B154(&p);
            if (*p == 0x3d) {
                p++;
                FUN_14005B154(&p);
                if ((*p & 0x30) == 0x30) {
                    *(uint8_t *)((char *)param_2 + 0xda) = *p & 1;
                }
            }
D896:
            uVar3 = *p;
            while ((uVar3 != 0 && (uVar3 < 9 || uVar3 > 0xd)) && uVar3 != 0x20) {
                p++;
                uVar3 = *p;
            }
            FUN_14005B154(&p);
        }
DCCA:
        uVar12 = 1;
        if (*p == 0) {
            return 1;
        }
    } while (true);

    /* LoadEnvi 结尾 (break 或 goto DD0F 到达) */
    DD38:
    if (*pbuf != 0) {
        *pbuf = 0;
        pbuf++;
        FUN_14005B154(&pbuf);
    }
    goto DD68;
    while (true) {
        pbuf++;
        uVar3 = *pbuf;
        if (uVar3 == 0) break;
DD0F:
        if (((8 < uVar3) && (uVar3 < 0xe)) || (uVar3 == 0x20)) {
            goto DD38;
        }
    }
DD68:
    if (*p == 0x2d) {
        p++;
    }
    LoadEnvi((LPCWSTR)p, (short *)pbuf);
    return 1;
}

uint32_t PECMD_DetectMinintBoot(void)
{
    /* @0x14000e0bc size=429 检测 MININT 引导选项 */
    WCHAR WVar1;
    uint32_t uVar2;
    uint32_t uVar3;
    int iVar4;
    uint64_t uVar5;
    DWORD local_res8[2];
    LPCWSTR local_res10;
    LPCWSTR local_res18;

    if (g_minintFlag == 0) {
        FUN_140063620((WCHAR **)&local_res18);
        local_res8[0] = 0xffffffff;
        uVar5 = PECMD_GetPackedSystemVersion();
        uVar3 = (uint32_t)(uVar5 >> 0x10);
        iVar4 = FUN_1400690c0((HKEY)0xffffffff80000002,
                              WSTR("SYSTEM\\CurrentControlSet\\Control"),
                              WSTR("SystemStartOptions"),
                              (int64_t *)&local_res18, local_res8, (void *)0);
        uVar2 = 0xffffffff;
        if ((-1 < iVar4) && (uVar2 = 0xffffffff, -1 < (int)local_res8[0])) {
            local_res10 = local_res18;
            if (*local_res18 != L'\0') {
                uVar2 = 2;
                do {
                    if (((uVar3 < 0x60001) &&
                         (iVar4 = StrCmpNW(local_res10, WSTR("/MININT"), 7), iVar4 == 0)) &&
                        ((WVar1 = local_res10[7], WVar1 == L'\0' ||
                          ((8 < (uint16_t)WVar1 && ((uint16_t)WVar1 < 0xe)) || (WVar1 == L' '))))) {
                        uVar2 = 1;
LAB_14000e261:
                        g_minintFlag = (char)uVar2;
                        goto LAB_14000e23f;
                    }
                    if (((0x5ffff < uVar3) &&
                         (iVar4 = StrCmpNW(local_res10, WSTR("MININT"), 6), iVar4 == 0)) &&
                        (((WVar1 = local_res10[6], WVar1 == L'\0' ||
                           ((8 < (uint16_t)WVar1 && ((uint16_t)WVar1 < 0xe)))) || (WVar1 == L' '))))
                        goto LAB_14000e261;
                    for (; (WVar1 = *local_res10, WVar1 != L'\0' &&
                           ((uint16_t)WVar1 < 9 || (0xd < (uint16_t)WVar1)) && (WVar1 != L' '));
                         local_res10 = local_res10 + 1) {
                    }
                    FUN_14005b154((WCHAR **)&local_res10);
                } while (*local_res10 != L'\0');
            }
            g_minintFlag = '\x10';
            uVar2 = 0;
        }
LAB_14000e23f:
        FUN_14005b104((WCHAR **)&local_res18);
    } else {
        uVar2 = (uint32_t)g_minintFlag & 1;
    }
    return uVar2;
}

int PECMD_DispatchBuiltin(int64_t *param_1, uintptr_t param_2, LPCWSTR param_3, LPCWSTR param_4,
                  FILETIME *param_5)
{
    /* @0x140016bc8 size=1094 命令表派发: 依 param_3 匹配多条内置命令表并执行 */
    LPCWSTR pWVar1;
    uint8_t uVar2;
    int iVar3;
    int iVar4;
    FILETIME _Var5;
    LARGE_INTEGER *pLVar6;
    LARGE_INTEGER LVar7;
    WCHAR **p_Var8;
    uint64_t uVar9;
    uint64_t uVar10;
    uint32_t uVar11;
    uint64_t uVar12;
    long long lVar13;
    LPCWSTR pWVar14;
    WCHAR *local_res18;
    WCHAR *local_48;
    void **local_40;
    int64_t *local_38;
    uint64_t local_30;
    uint64_t tmp;

    local_res18 = NULL;
    local_48 = NULL;
    uVar10 = 0;
    if (*param_3 == L'\0') {
        uVar2 = 0;
    } else {
        uVar2 = 1;
        uVar9 = uVar10;
        uVar12 = uVar10;
        if (0 < g_cmdTable1Count) {
            do {
                iVar3 = lstrcmpiW(param_3,
                                  *(LPCWSTR *)((uintptr_t)g_cmdTable1 + 0x10 + uVar9));
                if (iVar3 == 0) {
                    FUN_1400702B0(&local_res18, param_3);
                    FUN_14006375C(&local_res18, param_4);
                    local_40 = (void **)&PTR_FUN_14011cbe8;
                    local_38 = param_1;
                    local_30 = param_2;
                    iVar3 = ((int (*)(void *, WCHAR *))
                             *(void **)((uintptr_t)g_cmdTable1 +
                                        (long long)(int)uVar12 * 0x18))((void *)&local_40,
                                                                        local_res18);
                    tmp = (uint64_t)(int64_t)iVar3;
                    memcpy(param_5, &tmp, 8);
                    goto LAB_140016df1;
                }
                uVar11 = (int)uVar12 + 1;
                uVar9 = uVar9 + 0x18;
                uVar12 = (uint64_t)uVar11;
            } while ((int)uVar11 < g_cmdTable1Count);
        }
        uVar9 = uVar10;
        uVar12 = uVar10;
        if (0 < g_cmdTable2Count) {
            do {
                iVar3 = lstrcmpiW(param_3,
                                  (LPCWSTR)**((uintptr_t **)((uintptr_t)g_cmdTable2 + 8 +
                                                        uVar9)));
                if (iVar3 == 0) {
                    FUN_1400702B0(&local_res18,
                                  (LPCWSTR)**((uintptr_t **)((uintptr_t)g_cmdTable2 +
                                                        (long long)(int)uVar12 * 0x10)));
                    FUN_14006375C(&local_res18, WSTR(" "));
                    FUN_14006375C(&local_res18, param_4);
                    tmp = PECMD_RunCommand((void *)param_1, (void *)local_res18);
                    memcpy(&_Var5, &tmp, 8);
                    goto LAB_140016e53;
                }
                uVar11 = (int)uVar12 + 1;
                uVar9 = uVar9 + 0x10;
                uVar12 = (uint64_t)uVar11;
            } while ((int)uVar11 < g_cmdTable2Count);
        }
        uVar9 = uVar10;
        uVar12 = uVar10;
        if (0 < g_cmdTable3Count) {
LAB_140016c8d:
            iVar3 = lstrcmpiW(param_3,
                              (LPCWSTR)**((uintptr_t **)((uintptr_t)g_cmdTable3 + 8 + uVar9)));
            if (iVar3 != 0)
                goto code_r0x000140016cae;
            pWVar1 = (LPCWSTR)**((uintptr_t **)((uintptr_t)g_cmdTable3 +
                                           (long long)(int)uVar12 * 0x10));
            for (pWVar14 = pWVar1;
                 ((*pWVar14 == L'=' || (*pWVar14 == L'!')) || (*pWVar14 == L'@'));
                 pWVar14 = pWVar14 + 1) {
            }
            FUN_1400702B0(&local_res18, WSTR("--exe:"));
            FUN_14006375C(&local_res18, pWVar14);
            FUN_14006375C(&local_res18, WSTR(" "));
            FUN_14006375C(&local_res18, pWVar1);
            FUN_14006375C(&local_res18, WSTR(" "));
            FUN_14006375C(&local_res18, param_4);
            tmp = FUN_14000e26c((uint64_t)(intptr_t)(void *)param_1,
                                (uint64_t)(uintptr_t)local_res18,
                                (uint64_t)(intptr_t)(void *)param_1, 0, 0, NULL, 0, NULL);
            memcpy(&_Var5, &tmp, 8);
            goto LAB_140016e53;
        }
LAB_140016cbc:
        uVar9 = uVar10;
        uVar12 = uVar10;
        if (0 < g_cmdTable4Count) {
LAB_140016ccb:
            iVar3 = lstrcmpiW(param_3,
                              (LPCWSTR)**((uintptr_t **)((uintptr_t)g_cmdTable4 + 8 + uVar9)));
            if (iVar3 != 0)
                goto code_r0x000140016cec;
            FUN_1400702B0(&local_res18,
                          (LPCWSTR)**((uintptr_t **)((uintptr_t)g_cmdTable4 +
                                                (long long)(int)uVar12 * 0x10)));
            FUN_14006375C(&local_res18, WSTR(" "));
            FUN_14006375C(&local_res18, param_4);
            tmp = PECMD_ParseDateTimeSpec((void *)param_1, (void *)local_res18, 0, NULL);
            memcpy(&_Var5, &tmp, 8);
LAB_140016e53:
            memcpy(param_5, &_Var5, 8);
LAB_140016df1:
            p_Var8 = &local_res18;
            goto LAB_140016d69;
        }
LAB_140016cfa:
        local_48 = NULL;
        EnterCriticalSection(&g_csInit);
        iVar3 = g_cmdTable5Len / 2;
        if (0 < iVar3) {
            lVar13 = 0;
            uVar9 = uVar10;
            do {
                iVar4 = lstrcmpiW(param_3,
                                  (LPCWSTR)**((uintptr_t **)((long long)lVar13 + 8 +
                                                        (uintptr_t)g_cmdTable5)));
                if (iVar4 == 0) {
                    FUN_1400703E4(&local_48,
                                  (LPCWSTR)**((uintptr_t **)((uintptr_t)g_cmdTable5 +
                                                        (long long)(int)uVar9 * 0x10)));
                    LeaveCriticalSection(&g_csInit);
                    pLVar6 = (LARGE_INTEGER *)FUN_14007de70(&local_48, &local_res18, param_4);
                    LVar7 = FUN_14004c0bc((uint64_t)(intptr_t)(void *)param_1, *pLVar6,
                                          (long long *)0, (long long *)0, NULL);
                    memcpy(param_5, &LVar7, 8);
                    FUN_14005B104(&local_res18);
                    uVar2 = 1;
                    goto LAB_140016d64;
                }
                uVar10 = uVar10 + 1;
                uVar9 = (uint64_t)((int)uVar9 + 1);
                lVar13 = lVar13 + 0x10;
            } while ((long long)uVar10 < (long long)iVar3);
        }
        LeaveCriticalSection(&g_csInit);
        uVar2 = 0;
LAB_140016d64:
        p_Var8 = &local_48;
LAB_140016d69:
        FUN_14005B104(p_Var8);
    }
    return uVar2;

code_r0x000140016cae:
    uVar11 = (int)uVar12 + 1;
    uVar9 = uVar9 + 0x10;
    uVar12 = (uint64_t)uVar11;
    if (g_cmdTable3Count <= (int)uVar11)
        goto LAB_140016cbc;
    goto LAB_140016c8d;

code_r0x000140016cec:
    uVar11 = (int)uVar12 + 1;
    uVar9 = uVar9 + 0x10;
    uVar12 = (uint64_t)uVar11;
    if (g_cmdTable4Count <= (int)uVar11)
        goto LAB_140016cfa;
    goto LAB_140016ccb;
}

/* SKIP(CRT): FUN_14001708c 是 printf-family 包装
   (pthreadlocinfo/pthreadmbcinfo/va_list -> 转调 FUN_140103014)，
   按任务要求不实现，保留正确签名空桩。 */
void FUN_14001708c(WCHAR *param_1, size_t param_2, void *param_3, void *param_4)
{
    (void)param_1;
    (void)param_2;
    (void)param_3;
    (void)param_4;
}

uint64_t *PECMD_SkipLeadingControls(uint64_t *pp)
{
    /* @0x1400170b0 size=44 跳过前导空白/控制字符 */
    uint16_t *p;

    while ((p = (uint16_t *)(uintptr_t)*pp,
            (8 < *p && *p < 0xe) || *p == L' ')) {
        *pp = (uint64_t)(uintptr_t)(p + 1);
    }
    return pp;
}

uint64_t *PECMD_SkipWsByte(uint64_t *pp)
{
    /* @0x1400170dc size=51 跳过前导空白（字节版本） */
    uint8_t *p = (uint8_t *)(uintptr_t)*pp;

    while ((9 <= *p && *p <= 0xd) || *p == 0x20) {
        p++;
    }
    *pp = (uint64_t)(uintptr_t)p;
    return pp;
}

void PECMD_FreeNtdll(void)
{
    /* @0x140017180 size=35 释放 ntdll 模块句柄 */
    if (g_hNtdll != (HMODULE)0) {
        FreeLibrary(g_hNtdll);
    }
    g_hNtdll = (HMODULE)0;
}

uint64_t PECMD_MapPhysicalMemoryNT5(void)
{
    /* @0x1400172f4 size=298 物理内存映射(旧 NT5) */
    int iVar1;
    DWORD dwFileOffsetLow;
    uint8_t local_48[16];
    uint32_t local_38[2];
    uint64_t local_30;
    uint8_t *local_28;
    uint32_t local_20;
    uint64_t local_18;
    uint64_t local_10;

    DAT_14013cb90[0] = 0x114;   /* dwOSVersionInfoSize */
    GetVersionExW((OSVERSIONINFOW *)DAT_14013cb90);
    if (DAT_14013cb90[1] == 5) {                 /* dwMajorVersion == 5 */
        if (DAT_14013cb90[2] == 0) {             /* dwMinorVersion == 0 */
            dwFileOffsetLow = 0x30000;
        } else {
            if (DAT_14013cb90[2] != 1) {
                return 0;
            }
            dwFileOffsetLow = 0x39000;
        }
        ((void (*)(void *, LPCWSTR))(uintptr_t)g_pRtlInitUnicodeString)
            (local_48, WSTR("\\Device\\PhysicalMemory"));
        local_30 = 0;
        local_20 = 0;
        local_18 = 0;
        local_10 = 0;
        local_28 = local_48;
        local_38[0] = 0x30;
        iVar1 = ((int (*)(uint64_t *, DWORD, uint32_t *))(uintptr_t)g_pZwOpenSection)
                    (&g_u64cd08, 6, local_38);
        if (iVar1 == -0x3fffffde) {
            ((int (*)(uint64_t *, DWORD, uint32_t *))(uintptr_t)g_pZwOpenSection)
                (&g_u64cd08, 0x60000, local_38);
            FUN_1400171a4((HANDLE)(uintptr_t)g_u64cd08);
            CloseHandle((HANDLE)(uintptr_t)g_u64cd08);
            iVar1 = ((int (*)(uint64_t *, DWORD, uint32_t *))(uintptr_t)g_pZwOpenSection)
                        (&g_u64cd08, 6, local_38);
        }
        if (-1 < iVar1) {
            g_u64CD00 = (uint64_t)(uintptr_t)MapViewOfFile(
                                (HANDLE)(uintptr_t)g_u64cd08, 6, 0, dwFileOffsetLow, 0x1000);
            return -(uint64_t)((void *)(uintptr_t)g_u64CD00 != (void *)0)
                   & (uint64_t)g_u64cd08;
        }
    }
    return 0;
}

uint32_t FUN_140017420(uint64_t base, uint64_t key)
{
    /* @0x140017420 size=168 物理内存散列表查找 */
    uint32_t u1 = *(uint32_t *)(base + ((key & 0xffffffff) >> 0x16) * 4);

    if ((u1 & 1) != 0) {
        if (u1 & 0x80) {
            return ((u1 ^ (uint32_t)key) & 0x3fffff) ^ u1;
        }
        LPVOID pv = MapViewOfFile((HANDLE)(uintptr_t)g_u64cd08, 4, 0,
                                  u1 & 0xfffff000, 0x1000);
        uintptr_t lp = (uintptr_t)pv & 0xffffffff;
        uint64_t idx = ((key & 0xffffffff) >> 0xc) & 0x3ff;
        if (lp != 0 && (*(uint32_t *)(lp + idx * 4) & 1) != 0) {
            uint32_t a = *(uint32_t *)(lp + idx * 4);
            uint32_t b = *(uint32_t *)(lp + idx * 4);
            UnmapViewOfFile(pv);
            return ((a ^ (uint32_t)key) & 0xfff) ^ b;
        }
    }
    return 0;
}

uint32_t PECMD_ReadPhysMemSlot(uint64_t key)
{
    /* @0x1400174c8 size=99 读物理内存映射槽值 */
    uint32_t page = FUN_140017420(g_u64CD00, key);
    LPVOID pv = MapViewOfFile((HANDLE)(uintptr_t)g_u64cd08, 6, 0,
                              page & 0xfffff000, 0x1000);
    uint32_t r = 0;
    if (pv != NULL) {
        r = *(uint32_t *)((uintptr_t)pv + (uint64_t)(page >> 2 & 0x3ff) * 4);
        UnmapViewOfFile(pv);
    }
    return r;
}

bool PECMD_WritePhysicalMem(uint64_t key, uint32_t value)
{
    /* @0x14001752c size=113 写物理内存映射槽值 */
    uint32_t page = FUN_140017420(g_u64CD00, key);
    LPVOID pv = MapViewOfFile((HANDLE)(uintptr_t)g_u64cd08, 2, 0,
                              page & 0xfffff000, 0x1000);
    if (pv != NULL) {
        *(uint32_t *)((uintptr_t)pv + (uint64_t)(page >> 2 & 0x3ff) * 4) = value;
        UnmapViewOfFile(pv);
    }
    return pv != NULL;
}

void FUN_1400175a0(LPCWSTR name, LPCWSTR pattern)
{
    /* @0x1400175a0 size=147 模式匹配（'^' 与 '*' 互换后比对） */
    WCHAR *p;

    if (StrChrW((LPWSTR)pattern, L'^') != NULL) {
        for (p = (WCHAR *)name; *p != 0; p++) {
            if (*p == L'*') {
                *p = L'^';
            }
        }
    }
    if (PathMatchSpecW(name, pattern)) {
        for (p = (WCHAR *)name; *p != 0; p++) {
            if (*p == L'^') {
                *p = L'*';
            }
        }
    }
}

uint16_t *PECMD_FindLastPathSep(uint16_t *s)
{
    /* @0x140017634 size=52 返回最后一个路径分隔符后的位置 */
    uint16_t *last = s;

    while (*s != 0) {
        if (*s == L'\\' || *s == L'/') {
            last = s + 1;
        }
        s++;
    }
    return last;
}

HANDLE PECMD_CreateMutexA(LPCSTR name)
{
    /* @0x140017668 size=48 创建 ANSI 互斥体并等待 */
    HANDLE h = CreateMutexA(NULL, 0, name);

    if (h != (HANDLE)0) {
        WaitForSingleObject(h, 0xffffffff);
    }
    return h;
}

HANDLE FUN_140017698(LPCWSTR name)
{
    /* @0x140017698 size=139 创建宽字符互斥体（长名把 '\' 换 '/'）并等待 */
    LPCWSTR p;
    HANDLE h;

    if (name[0] != 0 && name[1] != 0 && name[2] != 0 && name[3] != 0 &&
        name[4] != 0 && name[5] != 0 && name[6] != 0 && name[7] != 0) {
        for (p = name + 8; *p != 0; p++) {
            if (*p == L'\\') {
                *(LPWSTR)p = L'/';
            }
        }
    }
    h = CreateMutexW(NULL, 0, name);
    if (h != (HANDLE)0) {
        WaitForSingleObject(h, 0xffffffff);
    }
    return h;
}

uint64_t *PECMD_CreateMutexSlot(uint64_t *p, LPCSTR name)
{
    /* @0x140017770 size=36 创建 ANSI 互斥体并存入槽 */
    HANDLE h = (HANDLE)0;
    if (name != NULL) {
        h = PECMD_CreateMutexA(name);
    }
    *p = (uint64_t)(uintptr_t)h;
    return p;
}

uint64_t *PECMD_CreateMutexToSlot(uint64_t *p, LPCWSTR name)
{
    /* @0x140017794 size=36 创建宽字符互斥体并存入槽 */
    HANDLE h = (HANDLE)0;
    if (name != NULL) {
        h = FUN_140017698(name);
    }
    *p = (uint64_t)(uintptr_t)h;
    return p;
}

void PECMD_ReleaseMutex(uint64_t *p)
{
    /* @0x1400177b8 size=23 释放互斥体 */
    if ((HANDLE)(uintptr_t)*p != (HANDLE)0) {
        ReleaseMutex((HANDLE)(uintptr_t)*p);
    }
}

int64_t FUN_1400177d0(int64_t *param_1)
{
    /* @0x1400177d0 size=168 */
    LPCWSTR lpString;
    int iVar1;
    int64_t lVar2;
    LPCWSTR local_res8;

    lpString = (LPCWSTR)*param_1;
    *param_1 = 0;
    local_res8 = lpString;
    iVar1 = lstrlenW(lpString);
    PECMD_GrowByteBuffer((void **)param_1, (int64_t)iVar1 * 5 + 0xb);
    iVar1 = WideCharToMultiByte(0xfde9, 0, lpString, iVar1, (LPSTR)*param_1,
                                (int)((int64_t)iVar1 * 5) + 10, (LPCSTR)0x0, (BOOL *)0x0);
    lVar2 = (int64_t)iVar1;
    if ((int64_t)iVar1 < 0) {
        lVar2 = 0;
    }
    *(uint8_t *)(lVar2 + *param_1) = 0;
    PECMD_GrowByteBuffer((void **)param_1, lVar2 + 3);
    FUN_14005B104((WCHAR **)&local_res8);
    return lVar2;
}

void PECMD_AppendDebugLog(LPCSTR text)
{
    /* @0x140017878 size=142 追加写入调试日志文件 */
    HANDLE h = CreateFileA("C:\\pe.debug.log", 0xc0000000, 7, NULL, 3, 0x80, (HANDLE)0);
    LARGE_INTEGER zero = {0};

    if (h != (HANDLE)-1) {
        PECMD_SetFilePointer(h, zero, 2);
        if (*text != '\0') {
            DWORD n = lstrlenA(text);
            DWORD written = 0;
            WriteFile(h, text, n, &written, NULL);
        }
        CloseHandle(h);
    }
}

LPVOID FUN_1400179f8(LPCWSTR param_1, LPVOID param_2, DWORD *param_3)
{
    /* @0x1400179f8 size=258 读文件到缓冲区(可选自分配) */
    HANDLE hFile;
    LARGE_INTEGER size[2] = {{0}};
    DWORD len[2] = {0, 0};
    LPVOID buf[2] = {0, 0};
    DWORD uVar1;

    PECMD_OpenFileHandle(&hFile, param_1, 0x80000000, 7,
                  (LPSECURITY_ATTRIBUTES)0, 3, 0, (HANDLE)0);
    if (hFile == (HANDLE)0) {
        return param_2;
    }
    size[0].QuadPart = 0;
    GetFileSizeEx(hFile, size);
    len[0] = size[0].LowPart;
    if (param_3 == (DWORD *)0) {
        param_3 = len;
    } else {
        *param_3 = len[0];
    }
    if (param_2 == (LPVOID)0) {
        FUN_1400633A8((void **)buf, (int64_t)(len[0] + 0x10));
        param_2 = buf[0];
        buf[0] = (LPVOID)0;
        FUN_14005B104((WCHAR **)buf);
        if (param_2 == (LPVOID)0) goto LAB_140017ae1;
    }
    size[0].QuadPart = size[0].QuadPart & 0xffffffff00000000ULL;
    ReadFile(hFile, param_2, *param_3, (DWORD *)&size[0].QuadPart, (void *)0);
    uVar1 = size[0].LowPart;
    if ((int)size[0].LowPart < 0) {
        uVar1 = 0;
    }
    *param_3 = uVar1;
    memset((uint8_t *)param_2 + uVar1, 0, 6);
LAB_140017ae1:
    CloseHandle(hFile);
    return param_2;
}

void FUN_140017b8c(void)
{
    /* @0x140017b8c size=336 延迟加载 SetupApi.DLL 设备管理函数 */
    HMODULE hmod;

    if (DAT_14013cef8 == 0) {
        hmod = (HMODULE)0;
        FUN_14005C828("SetupDiGetClassDevsW", "SetupApi.DLL", &DAT_14013cf00, &hmod);
        FUN_14005C828("SetupDiGetDeviceRegistryPropertyW", "SetupApi.DLL", &DAT_14013cf08, &hmod);
        FUN_14005C828("SetupDiDestroyDeviceInfoList", "SetupApi.DLL", &DAT_14013cf10, &hmod);
        FUN_14005C828("SetupDiSetClassInstallParamsW", "SetupApi.DLL", &g_pSetupDiSetClassInstallParamsW, &hmod);
        FUN_14005C828("SetupDiCallClassInstaller", "SetupApi.DLL", &DAT_14013cf20, &hmod);
        FUN_14005C828("SetupDiEnumDeviceInfo", "SetupApi.DLL", &DAT_14013cf28, &hmod);
        FUN_14005C828("SetupIterateCabinetW", "SetupApi.DLL", &DAT_14013cf30, &hmod);
        FUN_14005C828("InstallHinfSectionW", "SetupApi.DLL", &g_pInstallHinfSectionW, &hmod);
        FUN_14005C828("InstallHinfSection", "SetupApi.DLL", &g_pInstallHinfSection, &hmod);
        FUN_14005C828("CM_Get_DevNode_Status", "SetupApi.DLL", &DAT_14013cf48, &hmod);
        FUN_14005C828("SetupDiGetINFClassW", "SetupApi.DLL", (void **)&DAT_14013cef8, &hmod);
    }
}

uint32_t *PECMD_ClearStructFields(uint32_t *p)
{
    /* @0x140017f34 size=32 清零结构字段 */
    *p = 0;
    *(uint64_t *)(p + 2) = 0;
    *(uint64_t *)(p + 4) = 0;
    *(uint64_t *)(p + 6) = 0;
    *(uint64_t *)(p + 8) = 0;
    *(uint64_t *)(p + 10) = 0;
    *(uint64_t *)(p + 0xc) = 0;
    return p;
}

int64_t PECMD_FindResourceByNameOrId(int *param_1, LPCSTR param_2)
{
    /* @0x140017fbc size=249 按名字/序号查找资源项偏移 */
    uint32_t uVar1;
    uint32_t uVar2;
    int32_t iVar3;
    uint32_t uVar4;
    int64_t lVar5;
    int32_t iVar6;
    uint64_t uVar7;
    uint64_t uVar9;
    uint32_t uVar8;
    uint32_t uVar10;
    uint32_t *puVar11;

    lVar5 = *(int64_t *)(param_1 + 10);
    if (((*(int32_t *)(lVar5 + 0x88) != 0) && (*(int32_t *)(lVar5 + 0x8c) != 0)) &&
        (*param_1 != 0)) {
        uVar9 = (uint64_t)*(uint32_t *)(lVar5 + 0x88);
        uVar1 = *(uint32_t *)(lVar5 + 0x8c);
        lVar5 = *(int64_t *)(param_1 + 6);
        uVar2 = *(uint32_t *)(lVar5 + 0x1c + uVar9);
        uVar8 = *(uint32_t *)(lVar5 + 0x24 + uVar9);
        iVar3 = *(int32_t *)(lVar5 + 0x14 + uVar9);
        uVar4 = *(uint32_t *)(lVar5 + 0x18 + uVar9);
        puVar11 = (uint32_t *)((uint64_t)*(uint32_t *)(lVar5 + 0x20 + uVar9) + lVar5);
        if ((uintptr_t)param_2 < 0x10000U) {
            uVar8 = (uint32_t)(int32_t)(uintptr_t)param_2 -
                    *(int32_t *)(lVar5 + 0x10 + uVar9);
LAB_14001808f:
            if (((-1 < (int32_t)uVar8) && ((int32_t)uVar8 < iVar3)) &&
                ((uVar7 = (uint64_t)*(uint32_t *)((uint64_t)uVar2 + lVar5 +
                                                  (int64_t)(int32_t)uVar8 * 4),
                  uVar7 <= uVar9 || (uVar1 + uVar9 <= uVar7)))) {
                return *(int64_t *)(param_1 + 6) + (int64_t)uVar7;
            }
        } else {
            uVar10 = 0;
            if (uVar4 != 0) {
                do {
                    iVar6 = lstrcmpA((LPCSTR)((uint64_t)*puVar11 +
                                              *(int64_t *)(param_1 + 6)), param_2);
                    if (iVar6 == 0) {
                        if ((int32_t)uVar10 < 0) {
                            return 0;
                        }
                        uVar8 = (uint32_t)*(uint16_t *)((uint64_t)uVar8 + lVar5 +
                                                        (int64_t)(int32_t)uVar10 * 2);
                        goto LAB_14001808f;
                    }
                    uVar10 = uVar10 + 1;
                    puVar11 = puVar11 + 1;
                } while (uVar10 < uVar4);
            }
        }
    }
    return 0;
}

uint64_t FUN_1400180b8(int64_t param_1, int64_t param_2)
{
    /* @0x1400180b8 size=144 应用内存重定位 */
    uint16_t uVar1, uVar7;
    int iVar2;
    int64_t *plVar3;
    uint32_t *puVar4;
    int64_t lVar5, lVar6;

    lVar5 = param_2 - *(int64_t *)(*(int64_t *)(param_1 + 0x28) + 0x30);
    if (lVar5 != 0) {
        for (puVar4 = (uint32_t *)((uintptr_t)*(uint32_t *)(*(int64_t *)(param_1 + 0x28) + 0xb0)
                                   + (uintptr_t)param_2);
             puVar4[1] + *puVar4 != 0;
             puVar4 = (uint32_t *)((intptr_t)puVar4 + (uintptr_t)puVar4[1])) {
            lVar6 = 0;
            iVar2 = (int)(((uint64_t)puVar4[1] - 8) >> 1);
            if (0 < iVar2) {
                do {
                    uVar1 = *(uint16_t *)((uint8_t *)puVar4 + lVar6 * 2 + 8);
                    uVar7 = (uint16_t)(uVar1 & 0xf000);
                    if (uVar7 != 0) {
                        plVar3 = (int64_t *)((uintptr_t)*puVar4 + (uintptr_t)(uVar1 & 0xfff)
                                             + (uintptr_t)param_2);
                        if (uVar7 == 0x3000) {
                            *(int *)plVar3 = (int)*(int *)plVar3 + (int)lVar5;
                        } else {
                            *plVar3 = *plVar3 + lVar5;
                        }
                    }
                    lVar6 = lVar6 + 1;
                } while (lVar6 < iVar2);
            }
        }
    }
    return 1;
}

void PECMD_ResolveExportProc(HMODULE param_1, LPCSTR param_2)
{
    /* @0x140018148 size=46 解析模块导出函数(内存模块走内部解析) */
    if ((g_pMapBlk == (int *)0x0) || (*(HMODULE *)(g_pMapBlk + 6) != param_1)) {
        GetProcAddress(param_1, param_2);
    } else {
        PECMD_FindResourceByNameOrId(g_pMapBlk, param_2);
    }
}

void PECMD_GetModuleFileName(HMODULE hModule, LPWSTR buf, DWORD size)
{
    /* @0x140018178 size=63 获取模块文件名（内存 DLL 走特殊路径） */
    if (g_pMapBlk == NULL ||
        *(HMODULE *)((uint8_t *)g_pMapBlk + 0x18) != hModule) {
        GetModuleFileNameW(hModule, buf, size);
    } else {
        wsprintfW(buf, WSTR("\\??\\memdll?\\%p"));
    }
}

HMODULE PECMD_GetModuleHandle(LPCWSTR name)
{
    /* @0x1400181b8 size=102 获取模块句柄（识别内存 DLL 特殊路径） */
    if (g_pMapBlk != NULL &&
        *(HMODULE *)((uint8_t *)g_pMapBlk + 0x18) != (HMODULE)0) {
        WCHAR buf[72];
        wsprintfW(buf, WSTR("\\??\\memdll?\\%p"));
        if (lstrcmpiW(buf, name) == 0) {
            return *(HMODULE *)((uint8_t *)g_pMapBlk + 0x18);
        }
    }
    return GetModuleHandleW(name);
}

uint16_t FUN_1400183fc(int64_t param_1, int16_t *param_2, uint64_t param_3)
{
    /* @0x1400183fc size=156 验证 PE 头并建立节表指针 */
    uint16_t uVar1;
    int *piVar2;
    int iVar3;

    if (((0x3f < param_3) &&
         (*(int16_t **)(param_1 + 0x20) = param_2, *param_2 == 0x5a4d)) &&
        ((uint64_t)((int64_t)*(int *)(param_2 + 0x1e) + 0x108) <= (uint32_t)param_3)) {
        piVar2 = (int *)((int64_t)*(int *)(param_2 + 0x1e) + (int64_t)(uintptr_t)param_2);
        *(int **)(param_1 + 0x28) = piVar2;
        if (((*piVar2 == 0x4550) &&
             (uVar1 = *(uint16_t *)((uint8_t *)piVar2 + 0x16), (uVar1 & 0x2002) != 0)) &&
            (((*(uint8_t *)((uint8_t *)piVar2 + 0x16) & 2) != 0 &&
              ((int16_t)piVar2[5] == 0xf0)))) {
            iVar3 = 0;
            *(int **)(param_1 + 0x30) = piVar2 + 0x42;
            if (*(int16_t *)((uint8_t *)piVar2 + 6) != 0) {
                piVar2 = piVar2 + 0x46;
                do {
                    if (param_3 < (uint32_t)(piVar2[1] + *piVar2)) {
                        return 0;
                    }
                    iVar3 = iVar3 + 1;
                    piVar2 = piVar2 + 10;
                } while (iVar3 < (int)(uint32_t)*(uint16_t *)(*(int64_t *)(param_1 + 0x28) + 6));
            }
            return uVar1;
        }
    }
    return 0;
}

int64_t PECMD_AlignUp(int64_t unused, int64_t value, uint32_t align)
{
    /* @0x140018498 size=18 向上对齐到 align */
    uint64_t a = align;
    (void)unused;
    return (int64_t)((((a - 1) + (uint64_t)value) / a) * a);
}

uint64_t FUN_1400184ac(int64_t param_1)
{
    /* @0x1400184ac size=133 计算节区最大映射范围 */
    uint64_t uVar1, uVar2, uVar3, uVar4;
    int64_t lVar5;
    uint32_t *puVar6;
    uint32_t *puVar7;

    lVar5 = *(int64_t *)(param_1 + 0x28);
    if (lVar5 == 0) {
        uVar1 = 0;
    } else {
        uVar4 = (uint64_t)*(uint32_t *)(lVar5 + 0x38);
        uVar1 = PECMD_AlignUp(param_1, (uint64_t)*(uint32_t *)(lVar5 + 0x54),
                              *(uint32_t *)(lVar5 + 0x38));
        uVar3 = (uint64_t)*(uint16_t *)(lVar5 + 6);
        if (*(uint16_t *)(lVar5 + 6) != 0) {
            puVar7 = (uint32_t *)(*(int64_t *)(param_1 + 0x30) + 0xc);
            puVar6 = (uint32_t *)(*(int64_t *)(param_1 + 0x30) + 0x10);
            do {
                uVar2 = (uint64_t)puVar6[-2];
                if ((uint64_t)puVar6[-2] < (uint64_t)*puVar6) {
                    uVar2 = (uint64_t)*puVar6;
                }
                uVar2 = PECMD_AlignUp(param_1, uVar2 + *puVar7, (uint32_t)uVar4);
                if (uVar1 < uVar2) {
                    uVar1 = uVar2;
                }
                puVar6 = puVar6 + 10;
                puVar7 = puVar7 + 10;
                uVar3 = uVar3 - 1;
            } while (uVar3 != 0);
        }
    }
    return uVar1;
}

bool FUN_140018534(uint64_t *param_1)
{
    /* @0x140018534 size=146 TLS 槽缓冲初始化 */
    int iVar1;
    uint64_t *puVar2;
    uint64_t *lpTlsValue;
    bool bVar3;

    lpTlsValue = (uint64_t *)0;
    bVar3 = g_dwC934 != 0;
    if (bVar3) {
        do {
            puVar2 = (uint64_t *)HeapAlloc(g_hHeap, 8, 0x38);
            if (puVar2 != (uint64_t *)0) break;
            iVar1 = FUN_1400630D0(2);
        } while (iVar1 == 4);
        *(uint32_t *)((uint8_t *)puVar2 + 4) = 0xaa55;
        *puVar2 = 0x30;
        lpTlsValue = puVar2 + 1;
        puVar2[5] = 0xfffffffffff80000;
        TlsSetValue(g_dwC934, lpTlsValue);
    }
    if (param_1 != (uint64_t *)0) {
        *param_1 = (uint64_t)lpTlsValue;
    }
    return bVar3;
}

void FUN_1400185c8(char param_1, int64_t *param_2)
{
    /* @0x1400185c8 size=225 */
    (void)param_1;
    (void)param_2;
}

void PECMD_InitListHead(int64_t obj)
{
    /* @0x1400186ac size=13 初始化链表头 */
    *(uint64_t **)(obj + 0x50) = (uint64_t *)(obj + 0x58);
    *(uint64_t *)(obj + 0x58) = 0;
}

uint64_t *PECMD_SetObjVtable(uint64_t *p)
{
    /* @0x140018760 size=14 设置虚表并返回对象 */
    *p = (uint64_t)(uintptr_t)PTR_FUN_14011eb78;
    return p;
}

void PECMD_SetObjectVtable(uint64_t *p)
{
    /* @0x140018770 size=11 设置虚表 */
    *p = (uint64_t)(uintptr_t)PTR_FUN_14011eb78;
}

void PECMD_AllocSpecialString(WCHAR **ps, int count)
{
    /* @0x14001877c size=47 分配串并写入 "*?^" 特殊前缀 */
    WCHAR *p = PECMD_AllocString(ps, (int64_t)count);

    if (p != NULL) {
        *p = 0;
        *(uint16_t *)p = 0x2a;
        *(uint16_t *)((uint8_t *)p + 4) = 0x5e;
    }
}

void PECMD_CloseHandleSlot(int64_t obj)
{
    /* @0x1400187ac size=32 关闭并清空句柄槽 */
    HANDLE h = *(HANDLE *)(obj + 8);

    if (h != (HANDLE)0) {
        *(uint64_t *)(obj + 8) = 0;
        CloseHandle(h);
    }
}

void FUN_1400187cc(int64_t param_1, uint64_t param_2, int64_t param_3)
{
    /* @0x1400187cc size=65 DLL 卸载回调 */
    HMODULE hLibModule;

    (void)param_2;
    hLibModule = *(HMODULE *)(param_1 + 8);
    if (hLibModule != (HMODULE)0) {
        *(uint64_t *)(param_1 + 8) = 0;
        if (**(int16_t **)(param_3 + 8) == 0x3c) {
            FUN_140061C44();
            ((void (*)(HMODULE))g_pFreeLibrary)(hLibModule);
        } else {
            FreeLibrary(hLibModule);
        }
    }
}

void PECMD_CallSlotCleanup(int64_t unused1, int64_t unused2, int64_t ctx)
{
    /* @0x140018810 size=31 取槽内对象并调用其虚表 +0x10 清理函数 */
    void **slot = *(void ***)(ctx + 8);
    int64_t *obj;

    (void)unused1;
    (void)unused2;
    if (slot != NULL) {
        obj = (int64_t *)*slot;
        if (obj != NULL) {
            *slot = NULL;
            (**(void (**)(void))(*(int64_t *)*obj + 0x10))();
        }
    }
}

void FUN_140018830(int64_t param_1, uint64_t param_2, int64_t param_3)
{
    /* @0x140018830 size=130 字符串对象卸载回调 */
    uint64_t *puVar1;
    uint64_t local_res8;
    uint64_t local_res18[2];

    (void)param_2;
    puVar1 = *(uint64_t **)(param_3 + 8);
    local_res8 = 0;
    if (*(int64_t *)(param_1 + 8) == 0) {
        local_res8 = *puVar1;
    } else {
        if (*(int64_t *)(param_1 + 8) != 2) {
            if ((short)*puVar1 == 0) {
                return;
            }
            local_res18[0] = (uint64_t)(uintptr_t)puVar1;
            PECMD_ParseNumSkipWs((WCHAR **)local_res18, &local_res8);
            *puVar1 = 0;
            if (local_res8 == 0) {
                return;
            }
            goto LAB_140018897;
        }
        if ((short)*puVar1 != 8) {
            return;
        }
        local_res8 = puVar1[1];
    }
    if (local_res8 == 0) {
        return;
    }
    *puVar1 = 0;
LAB_140018897:
    PECMD_DelayLoadOleaut32();
    ((void (*)(uint64_t))g_pSysFreeString)(local_res8);
}

uint64_t *PECMD_InitStringObj(uint64_t *obj)
{
    /* @0x1400188b4 size=55 初始化串对象并设置虚表 */
    PECMD_SetObjVtable(obj);
    *obj = (uint64_t)(uintptr_t)PTR_FUN_14011eb88;
    obj[3] = 0;
    *(uint8_t *)((uint8_t *)obj + 0x21) = 0;
    *(uint8_t *)(obj + 4) = 0;
    obj[2] = 0;
    obj[5] = 0;
    obj[1] = 0xfffffffffffff800ULL;
    return obj;
}

void PECMD_FreeTripleString(int64_t *p)
{
    /* @0x140018944 size=51 释放三个串容器 */
    if ((*(uint8_t *)((uint8_t *)p + 0x1f) & 0xc0) != 0) {
        p[1] = 0;
    }
    FUN_14005B104((WCHAR **)(p + 2));
    FUN_14005B104((WCHAR **)(p + 1));
    FUN_14005B104((WCHAR **)p);
}

void FUN_140018b04(int64_t obj)
{
    /* @0x140018b04 size=82 重置对象若干状态字段 */
    *(uint8_t *)(obj + 0x11) &= 0xfc;
    *(uint64_t *)(obj + 0x78) = 0;
    *(uint64_t *)(obj + 0x80) = 0;
    *(uint32_t *)(obj + 0x8c) = 0;
    *(uint32_t *)(obj + 0x20) = 0;
    *(uint8_t *)(obj + 0xf) = 0;
    *(uint8_t *)(obj + 0xe) = 0;
    *(uint16_t *)(obj + 200) = 0;
    *(uint8_t *)(obj + 0x10) = (*(uint8_t *)(obj + 0x10) & 0xc1) | 0x41;
    *(uint8_t *)(obj + 0xca) = 0;
    *(uint64_t *)(obj + 0xd0) = 0;
    *(uint8_t *)(obj + 0xd8) = 0;
    *(uint8_t *)(obj + 0x12) = 0;
    *(uint64_t *)(obj + 0x18) = 0;
    *(uint8_t *)(obj + 0xd9) = 0;
}

uint16_t *PECMD_SkipSpacesTabs(uint16_t *s)
{
    /* @0x140018b58 size=24 跳过前导空格/制表符 */
    while (*s == L' ' || *s == L'\t') {
        s++;
    }
    return s;
}

LPCWSTR PECMD_StripTrailingSpaces(LPCWSTR s)
{
    /* @0x140018b70 size=60 去掉尾部空格/制表符 */
    LPWSTR p = (LPWSTR)s + lstrlenW(s) - 1;

    while ((LPCWSTR)p >= s && (*p == L' ' || *p == L'\t')) {
        *p = L'\0';
        p--;
    }
    return s;
}

void PECMD_TrimWhitespace(uint16_t *s)
{
    /* @0x140018bac size=21 去首尾空白 */
    PECMD_StripTrailingSpaces(PECMD_SkipSpacesTabs(s));
}

LPWSTR PECMD_GetFileName(LPWSTR path)
{
    /* @0x140018bc4 size=41 取路径最后反斜杠后的文件名 */
    LPWSTR p = StrRChrW(path, (LPCWSTR)0, L'\\');

    if (p != NULL) {
        path = p + 1;
    }
    return path;
}

ULARGE_INTEGER FUN_140018bf0(LPCWSTR path, int mode, int flag)
{
    /* @0x140018bf0 size=82 查询磁盘空闲/总空间 */
    ULARGE_INTEGER freeBytes;
    ULARGE_INTEGER total;
    ULARGE_INTEGER totalFree;
    BOOL ok;

    freeBytes.QuadPart = 0;
    total.QuadPart = 0;
    totalFree.QuadPart = 0;
    ok = GetDiskFreeSpaceExW(path, &freeBytes, &total, &totalFree);
    if (flag == 0) {
        if (mode != 0) {
            total = totalFree;
        }
    } else {
        total.QuadPart = (uint64_t)(int64_t)ok;
    }
    return total;
}

int64_t PECMD_GetDiskFreeMB(LPCWSTR path)
{
    /* @0x140018c44 size=25 取磁盘空闲空间（MB 量级） */
    return (int64_t)(FUN_140018bf0(path, 1, 0).QuadPart >> 0x14);
}

void PECMD_QueryDiskSpace(LPCWSTR path)
{
    /* @0x140018c60 size=11 仅触发磁盘空间查询 */
    FUN_140018bf0(path, 0, 1);
}

uint32_t FUN_140018e6c(void)
{
    /* @0x140018e6c size=275 读 Winlogon Background 三值 */
    DWORD DVar1;
    LPWSTR pWVar2;
    uint32_t uVar3, uVar4;
    uint32_t local_res8[2];
    uint32_t local_res10[2];
    DWORD local_res18[2];
    DWORD local_res20[2];
    WCHAR local_c8[88];

    local_res20[0] = 1;
    local_res18[0] = 0xa0;
    uVar4 = 0;
    DVar1 = FUN_14005C4E0((HKEY)(intptr_t)0xffffffff80000002,
                          WSTR("SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Winlogon"),
                          WSTR("Background"),
                          local_res20, (BYTE *)local_c8, local_res18);
    uVar3 = 0;
    if (DVar1 != 0) {
        local_c8[0] = 0;
    }
    local_res10[0] = 0;
    StrToIntExW(local_c8, 0, (int *)local_res10);
    pWVar2 = StrChrW(local_c8, L' ');
    if (pWVar2 != (LPWSTR)0) {
        *pWVar2 = 0;
        local_res8[0] = 0;
        StrToIntExW(pWVar2 + 1, 0, (int *)local_res8);
        uVar4 = local_res8[0] & 0xff;
        pWVar2 = StrChrW(pWVar2 + 1, L' ');
        if (pWVar2 != (LPWSTR)0) {
            *pWVar2 = 0;
            local_res8[0] = 0;
            StrToIntExW(pWVar2 + 1, 0, (int *)local_res8);
            uVar3 = local_res8[0] & 0xff;
        }
    }
    return (uVar4 << 8) | (uVar3 << 0x10) | (local_res10[0] & 0xff);
}

void PECMD_FillChar6(uint16_t *p, uint16_t ch)
{
    /* @0x140018f80 size=43 填充 6 个字符并清零后续 4 字节 */
    int i;

    for (i = 0; i < 6; i++) {
        p[i] = ch;
    }
    memset(p + 6, 0, 4);
}

uint64_t *PECMD_NormalizeNewlines(uint64_t *param_1)
{
    /* @0x140018fac size=178 将 LF 行改为 CRLF */
    WCHAR WVar1;
    int iVar2;
    WCHAR *pWVar3;
    WCHAR *pWVar4;
    LPCWSTR local_res8;
    int doCopy;

    local_res8 = (LPCWSTR)*param_1;
    *param_1 = 0;
    iVar2 = lstrlenW(local_res8);
    PECMD_AllocString((WCHAR **)param_1, (int64_t)iVar2 * 2);
    pWVar3 = (WCHAR *)*param_1;
    WVar1 = *local_res8;
    pWVar4 = (WCHAR *)local_res8;
    do {
        if (WVar1 == L'\0') {
            *pWVar3 = L'\0';
            FUN_14005B104((WCHAR **)&local_res8);
            return param_1;
        }
        doCopy = 0;
        if ((*pWVar4 == L'\r') && (pWVar4[1] == L'\n')) {
            *pWVar3 = L'\r';
            pWVar3 = pWVar3 + 1;
            pWVar4 = pWVar4 + 1;
            doCopy = 1;
        } else if (*pWVar4 == L'\n') {
            *pWVar3 = L'\r';
            pWVar3 = pWVar3 + 1;
            doCopy = 1;
        } else if (*pWVar4 != L'\r') {
            doCopy = 1;
        } else {
            *pWVar3 = L'\r';
            pWVar3[1] = L'\n';
            pWVar3 = pWVar3 + 2;
        }
        if (doCopy) {
            *pWVar3 = *pWVar4;
            pWVar3 = pWVar3 + 1;
        }
        pWVar4 = pWVar4 + 1;
        WVar1 = *pWVar4;
    } while (true);
}

HWND PECMD_ReplaceWithEditBox(HWND param_1, DWORD param_2, uint64_t param_3)
{
    /* @0x140019060 size=517 把窗口文本换成 EDIT 控件 */
    LPWSTR lpString;
    DWORD dwExStyle;
    int iVar1;
    HWND pHVar2;
    WPARAM wParam;
    uint32_t local_res8[2];
    uint32_t local_res18[2];
    POINT local_res20;
    LPWSTR local_58;
    RECT local_50;

    dwExStyle = (DWORD)GetWindowLongW(param_1, -0x14);
    pHVar2 = GetParent(param_1);
    local_50.left = 0;
    local_50.top = 0;
    local_50.right = 0;
    local_50.bottom = 0;
    GetWindowRect(param_1, &local_50);
    local_res20.x = 0;
    local_res20.y = 0;
    ScreenToClient(pHVar2, &local_res20);
    OffsetRect(&local_50, local_res20.x, local_res20.y);
    iVar1 = GetWindowTextLengthW(param_1);
    FUN_140063694((WCHAR **)&local_58, (int64_t)(iVar1 + 0x1b));
    lpString = local_58;
    *local_58 = 0;
    GetWindowTextW(param_1, local_58, iVar1 + 0xb);
    local_58[(int64_t)iVar1 + 10] = 0;
    wParam = (WPARAM)(intptr_t)SendMessageW(param_1, 0x31, 0, 0);
    local_res8[0] = 0xffffffff;
    local_res18[0] = 0xffffffff;
    SendMessageW(param_1, 0xb0, (WPARAM)(uintptr_t)local_res8, (LPARAM)(uintptr_t)local_res18);
    ShowWindow(param_1, 0);
    if ((int)param_3 < 0) {
        DestroyWindow(param_1);
    }
    pHVar2 = CreateWindowExW(dwExStyle, WSTR("EDIT"), (LPCWSTR)0, param_2,
                             local_50.left, local_50.top,
                             local_50.right - local_50.left, local_50.bottom - local_50.top,
                             pHVar2, (HMENU)(param_3 & 0xffff), g_hInst, (LPVOID)0);
    SendMessageW(pHVar2, 0x30, wParam, 1);
    SetWindowTextW(pHVar2, lpString);
    SendMessageW(pHVar2, 0xb1, (WPARAM)local_res8[0], (LPARAM)local_res18[0]);
    FUN_14005B104((WCHAR **)&local_58);
    return pHVar2;
}

void PECMD_DestroyWindowSlot(uint64_t *p)
{
    /* @0x140019268 size=28 销毁窗口并清空句柄槽 */
    DestroyWindow((HWND)(uintptr_t)*p);
    *p = 0;
}

void PECMD_RestoreWindowProc(int64_t obj)
{
    /* @0x140019284 size=74 恢复窗口过程并销毁子窗口 */
    HWND child = *(HWND *)(obj + 0xb0);

    if (child != (HWND)0) {
        SetWindowLongPtrW(child, -4, *(LONG_PTR *)(obj + 0xe8));
        PECMD_DestroyWindowSlot((uint64_t *)(obj + 0xb0));
        *(uint64_t *)(obj + 0xa8) = 0;
    }
}

uint64_t PECMD_DrawTooltipText(uint64_t param_1, HDC param_2, HWND param_3)
{
    /* @0x1400192d0 size=681 在 HDC/窗口绘制前台提示文本并计算/放置位置 */
    bool bVar1;
    char cVar2;
    HDC hdc;
    HGDIOBJ h;
    LONG LVar8;
    int iVar9;
    LONG LVar10;
    int iVar11;
    int iVar6;
    int iVar7;
    RECT local_58;
    RECT local_48;
    int local_38;
    int iStack_34;
    uint32_t dflags;
    (void)param_1;

    hdc = param_2;
    if (param_2 == (HDC)0) {
        hdc = GetDC(param_3);
    }
    h = g_hFontFast;
    if (g_hFontFast == (HFONT)0) {
        h = GetStockObject(0x11);
    }
    SelectObject(hdc, h);
    cVar2 = g_flagCF86;
    if ((g_flagCF86 == 0) || (param_3 == (HWND)0) || (param_2 != (HDC)0)) {
        bVar1 = false;
    } else {
        bVar1 = true;
    }
    if (g_pwszD258 != (WCHAR *)0) {
        local_48.left = g_intA238;
        local_48.top = g_intA23C;
        local_48.right = g_intA240;
        local_48.bottom = g_intA244;
        LVar10 = local_48.right;
        if (local_48.right < 1) {
            LVar10 = ((local_48.right * 2 >> 1) - local_48.left) + g_tipOffX;
            local_48.right = LVar10;
        }
        LVar8 = local_48.bottom;
        if (local_48.bottom < 1) {
            LVar8 = ((local_48.bottom * 2 >> 1) - local_48.top) + g_tipOffY;
            local_48.bottom = LVar8;
        }
        dflags = (uint32_t)(uint8_t)g_tipAlign;
        iVar11 = LVar10 - local_48.left;
        iVar9 = LVar8 - local_48.top;
        local_58 = local_48;
        memset(&local_38, 0, 0x10);
        if (cVar2 != 0) {
            local_58.top = iStack_34;
            local_58.left = local_38;
            local_58.bottom = iVar9;
            local_58.right = iVar11;
        }
        DrawTextW(hdc, g_pwszD258, -1, &local_58, dflags & 3 | 0x440);
        LVar8 = local_58.bottom;
        LVar10 = local_58.top;
        iVar11 = iVar11 - (local_58.right - local_58.left);
        iVar9 = iVar9 - (local_58.bottom - local_58.top);
        if (bVar1) {
            iVar6 = (g_tipAlign & 1) ? (iVar11 / 2) : 0;
            iVar7 = (g_tipAlign & 4) ? (iVar9 / 2) : 0;
            if (0 < iVar11) {
                local_38 = iVar6 + local_58.left + local_48.left;
            }
            if (0 < iVar9) {
                iStack_34 = local_58.top + iVar7 + local_48.top;
            }
            SetWindowPos(param_3, (HWND)0, local_38, iStack_34,
                         local_38 + 2 + (local_58.right - local_58.left),
                         (local_58.bottom - local_58.top) + iStack_34, 0x14);
        } else if (g_flagCF86 == 0) {
            if ((g_tipAlign & 1) != 0) {
                local_58.right = local_58.right + iVar11 / 2;
                local_58.left = local_58.left + iVar11 / 2;
            }
            if ((g_tipAlign & 4) != 0) {
                local_58.bottom = LVar8 + iVar9 / 2;
                local_58.top = LVar10 + iVar9 / 2;
            }
        }
        SetBkMode(hdc, 1);
        SetTextColor(hdc, g_dwTipsTextColor);
        DrawTextW(hdc, g_pwszD258, -1, &local_58, dflags & 3 | 0x40);
    }
    if (param_2 == (HDC)0) {
        ReleaseDC(param_3, hdc);
    }
    return 0;
}

HWND FUN_14001957c(void)
{
    /* @0x14001957c size=116 读 LogohWnd 注册表句柄并校验仍有效 */
    HWND h = 0;
    DWORD type = 0;
    DWORD size = 8;
    DWORD r;

    r = FUN_14005C4E0((HKEY)(intptr_t)0xffffffff80000002,
                      WSTR("SOFTWARE\\PELOGON"), WSTR("LogohWnd"),
                      &type, (BYTE *)&h, &size);
    if (h != (HWND)0 &&
        (r != 0 || !IsWindow(h))) {
        h = 0;
    }
    return h;
}

int FUN_140019814(LPWSTR root, int minSize, WCHAR *out)
{
    /* @0x140019814 size=228 扫描本地盘，找容量足够且类型匹配的盘 */
    WCHAR *drives = NULL;
    int ret = 0;

    FUN_140063694(&drives, 0x325);
    GetLogicalDriveStringsW(0x324, drives);
    CharUpperW(root);
    CharUpperW(drives);

    LPCWSTR p = drives;
    if (*p != L'\0') {
        do {
            int64_t freeMB = PECMD_GetDiskFreeMB(p);
            if (minSize + 0x10U <= (uint64_t)freeMB) {
                UINT t = GetDriveTypeW(p);
                if (t == 2) {
                    *out = *p;
                } else if (t == 3) {
                    ret = 1;
                    out[1] = *p;
                    if ((uint16_t)*root <= (uint16_t)*p) {
                        ret = 1;
                        break;
                    }
                }
            }
            int64_t len = PECMD_WideStrLen(p);
            p = p + (len + 1);
        } while (*p != L'\0');
    }
    FUN_14005B104((WCHAR **)&drives);
    return ret;
}

void FUN_1400198f8(uint64_t param, int mode)
{
    /* @0x1400198f8 size=100 查询服务状态并按模式校验 */
    uint8_t buf[0x24] = {0};
    DWORD ret = 0;
    int r;
    bool ok;

    r = ((int (*)(uint64_t, int, void *, DWORD, DWORD *))g_pQueryServiceStatusEx)(
        param, 0, buf, 0x24, &ret);
    if (mode == 1) {
        ok = (*(uint32_t *)(buf + 4) == 1);
    } else {
        ok = (r == 0);
    }
    if (!ok) {
        GetTickCount();
    }
}

void FUN_14001995c(void)
{
    /* @0x14001995c size=175 读取紧随启动的 fbwf.hook 批处理并转脚本执行 */
    DWORD DVar1;
    DWORD type[2];
    DWORD size[2];
    LARGE_INTEGER local_res18;

    FUN_140063694((WCHAR **)&local_res18, 0x1000);
    *(uint16_t *)&local_res18 = 0;
    type[0] = 0;
    size[0] = 0x2000;
    DVar1 = FUN_14005C4E0((HKEY)(intptr_t)0xffffffff80000002,
                          WSTR("SOFTWARE\\PELOGON\\RAMDATA"),
                          WSTR("fbwf.hook"),
                          type, (BYTE *)&local_res18, size);
    if ((DVar1 == 0) && (*(int16_t *)&local_res18 != 0)) {
        FUN_14005c61c((HKEY)(intptr_t)0xffffffff80000002,
                      WSTR("SOFTWARE\\PELOGON\\RAMDATA"), WSTR("fbwf.hook"));
        FUN_14004c0bc(0x14013d130, local_res18, 0, 0, 0);
    }
    FUN_14005B104((WCHAR **)&local_res18);
}

uint16_t *PECMD_AssignIfEmpty(WCHAR **pp, LPCWSTR src)
{
    /* @0x140019a0c size=32 串为空时先赋值再返回 */
    WCHAR *p = *pp;

    if (*p == 0) {
        FUN_1400703E4(pp, src);
        p = *pp;
    }
    return (uint16_t *)p;
}

void FUN_140019a2c(int64_t param_1)
{
    /* @0x140019a2c size=325 构造玩笑式 EXEC 命令串 */
    LPWSTR lpString;
    LPCWSTR lpString_00;
    int iVar1;
    int iVar2;
    uint16_t local_res8[4];
    LPWSTR buf;

    lpString = (LPWSTR)*(int64_t *)(param_1 + 0x220);
    lpString_00 = *(LPCWSTR *)(param_1 + 0x228);
    iVar1 = lstrlenW(lpString);
    if ((0 < iVar1) && (lpString[(int64_t)iVar1 - 1] == L'\\')) {
        iVar1 = iVar1 - 1;
        lpString[iVar1] = 0;
    }
    iVar2 = lstrlenW(lpString_00);
    if ((0 < iVar2) && (lpString_00[(int64_t)iVar2 - 1] == L'\\')) {
        iVar2 = iVar2 - 1;
        ((LPWSTR)lpString_00)[iVar2] = 0;
    }
    if (0 < iVar2 + iVar1) {
        FUN_140063694((WCHAR **)&buf, (int64_t)(iVar2 + iVar1 + 100));
        wsprintfW(buf, WSTR("-incmd /IDLE PECMD %s@FILE -force \"%s\""));
        if ((0 < iVar1) && (0 < iVar2)) {
            iVar1 = lstrlenW((LPCWSTR)buf);
            wsprintfW(buf + iVar1, WSTR("| @FILE -force \"%s\""));
        }
        *((LPWSTR)lpString_00) = 0;
        *lpString = 0;
        local_res8[0] = 0;
        FUN_14000e26c((uint64_t)(uintptr_t)g_Script,
                      (uint64_t)(uintptr_t)buf,
                      (uint64_t)(uintptr_t)g_Script,
                      *(uint64_t *)local_res8,
                      0, (void *)0, 0, (void *)0);
        FUN_14005B104((WCHAR **)&buf);
    }
}

void FUN_140019b74(int64_t *param_1)
{
    /* @0x140019b74 size=221 释放库并写回变量 */
    HMODULE hLibModule;
    int64_t *plVar1;
    int iVar2;

    if (*param_1 != 0) {
        hLibModule = *(HMODULE *)(param_1[3] + 0x138);
        plVar1 = *(int64_t **)(param_1[3] + 0x110);
        if (hLibModule != (HMODULE)0) {
            FreeLibrary(hLibModule);
            *(uint64_t *)(param_1[3] + 0x138) = 0;
            *(uint64_t *)(param_1[3] + 0x140) = 0;
        }
        if (((param_1[2] != 0) && (*(int64_t *)(uintptr_t)*param_1 != 0)) &&
            (plVar1 != (int64_t *)0)) {
            iVar2 = lstrlenW((LPCWSTR)(uintptr_t)*plVar1);
            *(uint16_t *)((uintptr_t)*plVar1 - 4 + (int64_t)iVar2 * 2) = 0;
            *(uint16_t *)((uintptr_t)*plVar1 - 2 + (int64_t)iVar2 * 2) = 0;
            FUN_1400629B8((void *)(uintptr_t)param_1[2],
                          *(LPCWSTR *)(uintptr_t)*param_1,
                          (LPCWSTR)((uintptr_t)*plVar1 + 6));
        }
        if ((((int64_t *)(uintptr_t)param_1[2] != (int64_t *)0) &&
             (*(int64_t *)(uintptr_t)param_1[1] != 0)) &&
            (*(uint64_t **)(param_1[3] + 0x160) != (uint64_t *)0)) {
            FUN_1400629B8((void *)(uintptr_t)param_1[2],
                          *(LPCWSTR *)(uintptr_t)param_1[1],
                          (LPCWSTR)(uintptr_t)**(uint64_t **)(param_1[3] + 0x160));
        }
        param_1[2] = 0;
        param_1[1] = 0;
        *param_1 = 0;
    }
}

void PECMD_InitEditBuffer(int64_t obj, uint16_t *p)
{
    /* @0x140019c54 size=26 初始化编辑缓冲指针/长度 */
    *p = 0;
    *(uint16_t **)(obj + 0x128) = p;
    *(uint16_t **)(obj + 0x130) = p;
    *(uint32_t *)(obj + 0x148) = 0;
}

void PECMD_TerminateJobObject(int64_t obj)
{
    /* @0x140019c70 size=70 终止并关闭作业对象 */
    HANDLE h;

    if (*(HANDLE *)(obj + 0x1b8) != (HANDLE)0) {
        TerminateJobObject(*(HANDLE *)(obj + 0x1b8), 0x4c7);
        h = *(HANDLE *)(obj + 0x1b8);
        if (h != (HANDLE)0 && h != (HANDLE)-1) {
            CloseHandle(h);
        }
        *(uint64_t *)(obj + 0x1b8) = 0;
    }
}

uint16_t *FUN_140019cb8(int64_t obj, LPCWSTR key, int len)
{
    /* @0x140019cb8 size=189 在换行分隔的表项中按前缀查找 */
    uint16_t *cur = *(uint16_t **)(*(uint64_t **)(obj + 0x110));
    uint16_t *p = cur;

    if (len < 0) {
        len = lstrlenW(key);
    }
    for (;;) {
        if (*p == 0) {
            return NULL;
        }
        if (*p == 0xd && p[1] == 10) {
            if (StrCmpNIW(p + 2, key, len) == 0 &&
                p[len + 2] == 0xd && p[len + 3] == 10) {
                return p + 2;
            }
            p++;
        }
        p++;
    }
}

uint16_t *PECMD_TableLookupEntry(int64_t obj, LPCWSTR key, int len)
{
    /* @0x140019d78 size=45 查表入口（空键/通配星跳过） */
    if (*key == L'\0' || *(uint16_t *)(*(uint64_t **)(obj + 0x110)) == 0x2a) {
        return NULL;
    }
    return FUN_140019cb8(obj, key, len);
}

void FUN_140019da8(int64_t obj, LPCWSTR key, int len)
{
    /* @0x140019da8 size=140 查不到则把键追加进表并补换行分隔 */
    uint16_t *found;
    int64_t tmp = 0;

    if (*key != L'\0') {
        found = FUN_140019cb8(obj, key, len);
        if (found == NULL) {
            FUN_1400702B0((WCHAR **)&tmp, WSTR("\r\n"));
            FUN_14006375C((WCHAR **)&tmp, key);
            FUN_14006375C((WCHAR **)&tmp, WSTR("\r\n"));
            FUN_14006375C((WCHAR **)(*(uint64_t **)(obj + 0x110)), key);
            FUN_14006375C((WCHAR **)(*(uint64_t **)(obj + 0x110)), WSTR("\r\n"));
            FUN_14005B104((WCHAR **)&tmp);
        }
    }
}

bool FUN_140019e34(int64_t param_1, WCHAR *param_2, uint64_t param_3)
{
    /* @0x140019e34 size=242 判断设备实例是否匹配指定 DevClass */
    int iVar1;
    WCHAR *_Str1;
    WCHAR *local_res8;
    uint32_t local_88[4];
    WCHAR local_78[56];

    local_88[0] = 0;
    local_88[1] = 0;
    local_88[2] = 0;
    local_88[3] = 0;
    FUN_140063694(&local_res8, 0xa2);
    (*DAT_14013cef8)((LPCWSTR)param_3, (GUID *)local_88, local_res8, 0xa0, 0);
    _Str1 = local_res8;
    if (*param_2 == L'{') {
        local_78[0] = L'\0';
        local_78[1] = L'\0';
        local_78[2] = L'\0';
        local_78[3] = L'\0';
        FUN_14005fc90(local_78, local_88, 1);
        _Str1 = local_78;
    }
    iVar1 = lstrcmpiW(_Str1, param_2);
    if ((*(uint8_t *)(param_1 + 0x157) & 1) != 0) {
        FUN_140018d8c((uint64_t)(uintptr_t)g_Script,
                      WSTR("\r\nDevClass[%s]:[%s] [%d] "),
                      (uint64_t)(uintptr_t)local_res8, (uint64_t)param_3);
    }
    FUN_14005B104((WCHAR **)&local_res8);
    return iVar1 == 0;
}

int FUN_140019f28(uint32_t param_1)
{
    /* @0x140019f28 size=119 设备重枚举 (延时 CM_Reenumerate) */
    int iVar1;
    uint32_t local_res10[6];

    local_res10[0] = 0;
    iVar1 = CM_Locate_DevNodeW(local_res10, NULL, 0);
    FUN_1400195f0((uint64_t)(uintptr_t)g_Script, 10, 0, (uint64_t *)0x0);
    if (iVar1 == 0) {
        iVar1 = CM_Reenumerate_DevNode(local_res10[0], param_1);
        FUN_1400195f0((uint64_t)(uintptr_t)g_Script, 5, 0, (uint64_t *)0x0);
        if (iVar1 == 0) {
            iVar1 = 0;
        }
    }
    return iVar1;
}

void FUN_140019fa0(LPCWSTR param_1, void *param_2, uint64_t *param_3, uint8_t *param_4)
{
    /* @0x140019fa0 size=253 设备类 GUID 解析 ('!'/ '~' 前缀 + '{...}' 或类名) */
    uint64_t uVar1;
    BOOL BVar2;
    int64_t lVar3;
    LPCWSTR local_res8;
    GUID *local_18[2];

    while (1) {
        for (; *param_1 == L'!'; param_1 = param_1 + 1) {
            *param_4 = 1;
        }
        if (*param_1 != L'~') {
            break;
        }
        param_1 = param_1 + 1;
        param_4[1] = 1;
    }
    if (*param_1 == L'{') {
        local_res8 = param_1;
        if (param_1[1] == L'}') {
            memset(param_3, 0, 0x10);
        } else {
            lVar3 = PECMD_ParseVolumeGuid((int64_t *)&local_res8, (uint32_t *)param_3, 1);
            if (lVar3 < 1) {
                return;
            }
        }
        *(void **)param_2 = param_3;
    } else {
        local_res8 = param_1;
        FUN_1400633A8((void **)local_18, 0x800);
        BVar2 = SetupDiClassGuidsFromNameW(param_1, local_18[0], 0x80, (DWORD *)&local_res8);
        if (((BVar2 != 0) && ((uint32_t)(uintptr_t)local_res8 != 0)) &&
            ((uint32_t)(uintptr_t)local_res8 < 0x81)) {
            uVar1 = *(uint64_t *)((uint8_t *)local_18[0] + 8);
            *param_3 = *(uint64_t *)local_18[0];
            param_3[1] = uVar1;
            *(void **)param_2 = param_3;
        }
        FUN_14005B104((WCHAR **)local_18);
    }
}

LARGE_INTEGER PECMD_ExecDashCommand(LPCWSTR s)
{
    /* @0x14001a0a0 size=81 执行 "-- " 前缀命令并返回结果 */
    LARGE_INTEGER cmd;
    LARGE_INTEGER result;

    cmd.QuadPart = 0;
    FUN_1400702B0((WCHAR **)&cmd, WSTR("-- "));
    FUN_14006375C((WCHAR **)&cmd, s);
    result = FUN_14003C06C((int64_t *)&g_Script, cmd, 0);
    FUN_14005B104((WCHAR **)&cmd);
    return result;
}

uint64_t FUN_14001a0f4(LPCWSTR path)
{
    /* @0x14001a0f4 size=251 判断是否为 PE 可执行文件 */
    HANDLE h;
    uint64_t r = 0;
    DWORD read;
    uint16_t dos[0x20] = {0};
    int pe[0x40] = {0};

    h = CreateFileW(path, 0x80000000, 1, NULL, 3, 0x80, (HANDLE)0);
    if (h == (HANDLE)-1) {
        return 0;
    }
    ReadFile(h, dos, 0x40, &read, NULL);
    if (read == 0x40 && dos[0] == 0x5a4d) {
        DWORD off = *(DWORD *)((uint8_t *)dos + 0x3c);
        if (SetFilePointer(h, (LONG)off, NULL, 0) != (DWORD)-1) {
            ReadFile(h, pe, 0xf8, &read, NULL);
            if (read == 0xf8 && pe[0] == 0x4550) {
                r = 1;
            }
        }
    }
    CloseHandle(h);
    return r;
}

uint16_t PECMD_NextNonDelimChar(uint16_t **pp)
{
    /* @0x14001a1f0 size=48 取当前非分隔字符并前进；空/空白返回 ',' */
    uint16_t *p = *pp;
    uint16_t ch = 0x2c;

    if (*p != 0 && *p != L' ' && *p != L'\t') {
        ch = *p;
        *pp = p + 1;
    }
    return ch;
}

bool PECMD_CmpStrN(LPCWSTR a, LPCWSTR b, int len, int caseSensitive)
{
    /* @0x14001a25c size=38 限定长度字符串比较 */
    int r = caseSensitive ? StrCmpNW(a, b, len) : StrCmpNIW(a, b, len);

    return r == 0;
}

LPWSTR PECMD_GetProcessUserName(DWORD pid, LPWSTR param_2)
{
    /* @0x14001a284 size=513 通过进程令牌查询并返回进程用户 SID 名 */
    void *Sid;
    BOOL BVar1;
    HANDLE ProcessHandle;
    LPWSTR TokenInformation;
    DWORD local_res10[2];
    DWORD local_res18[2];
    DWORD local_res20[2];
    int local_248[2];
    HANDLE local_240;
    WCHAR local_238;
    uint64_t local_236[65];
    LPWSTR pWVar2;

    pWVar2 = (LPWSTR)0;
    *param_2 = L'\0';
    local_240 = (HANDLE)0;
    local_res10[0] = 0;
    local_238 = L'\0';
    memset(local_236, 0, 0x206);
    ProcessHandle = OpenProcess(0x400, 0, pid);
    if (ProcessHandle == (HANDLE)0) {
        return (LPWSTR)0;
    }
    BVar1 = OpenProcessToken(ProcessHandle, 8, &local_240);
    if (BVar1 != 0) {
        if (local_240 == (HANDLE)0) goto LAB_14001a462;
        GetTokenInformation(local_240, 1, 0, 0, local_res10);
        TokenInformation = pWVar2;
        if ((local_res10[0] != 0) &&
            (TokenInformation = (LPWSTR)GlobalAlloc(0x40, (size_t)local_res10[0]),
             TokenInformation != (LPWSTR)0)) {
            BVar1 = GetTokenInformation(local_240, 1, TokenInformation, local_res10[0],
                                        local_res10);
        }
        if (BVar1 != 0) {
            if (TokenInformation == (LPWSTR)0) goto LAB_14001a452;
            Sid = *(void **)TokenInformation;
            local_248[0] = 0;   /* SidTypeUnknown */
            local_res20[0] = 0;
            local_res18[0] = 0;
            BVar1 = LookupAccountSidW(0, Sid, 0, local_res20, 0, local_res18, local_248);
            if ((local_res20[0] - 1 < 0x103) && (local_res18[0] - 1 < 0x104)) {
                BVar1 = LookupAccountSidW(0, Sid, param_2, local_res20, &local_238,
                                          local_res18, local_248);
            }
            if (BVar1 != 0) {
                pWVar2 = param_2;
            }
        }
        if (TokenInformation != (LPWSTR)0) {
            GlobalFree((HGLOBAL)TokenInformation);
        }
    }
LAB_14001a452:
    if (local_240 != (HANDLE)0) {
        CloseHandle(local_240);
    }
LAB_14001a462:
    CloseHandle(ProcessHandle);
    return pWVar2;
}

bool FUN_14001a488(LPCWSTR user, DWORD pid, int case_sens)
{
    /* @0x14001a488 size=136 判断进程用户是否等于指定用户 */
    int iVar1;
    LPWSTR pWVar2;
    bool bVar3;
    WCHAR local_218;
    uint64_t local_216[65];

    local_218 = L'\0';
    memset(local_216, 0, 0x206);
    pWVar2 = PECMD_GetProcessUserName(pid, &local_218);
    if (pWVar2 == (LPWSTR)0) {
        bVar3 = false;
    } else {
        if (case_sens == 0) {
            iVar1 = lstrcmpiW(user, &local_218);
        } else {
            iVar1 = lstrcmpW(user, &local_218);
        }
        bVar3 = iVar1 == 0;
    }
    return bVar3;
}

void PECMD_TerminateProcessById(DWORD pid, UINT exitCode, uint64_t *out)
{
    /* @0x14001a510 size=91 结束指定进程 */
    HANDLE h = OpenProcess(1, 0, pid);

    if (h != (HANDLE)0) {
        if (TerminateProcess(h, exitCode) != 0 && out != NULL) {
            *out = 0;
        }
        CloseHandle(h);
    }
}

uint32_t FUN_14001a5b0(LPCWSTR name)
{
    /* @0x14001a5b0 size=95 读 PELOGON 标志值 */
    int value = -1;
    DWORD type = 4;
    DWORD size = 4;
    uint32_t result = 0;
    DWORD r;

    r = FUN_14005C4E0((HKEY)(intptr_t)0xffffffff80000002,
                      WSTR("SOFTWARE\\PELOGON"), name,
                      &type, (BYTE *)&value, &size);
    if (r == 0 && value == 1) {
        result = 0x4000;
    }
    return result;
}

void PECMD_EnsureMciLoaded(void)
{
    /* @0x14001a610 size=48 延迟加载 mciSendStringW */
    if (DAT_14013d050 == 0) {
        FUN_14005C828("mciSendStringW", "winmm.DLL",
                      (void **)&DAT_14013d050, NULL);
    }
}

char PECMD_ChDirToPath(char *flag, LPCWSTR path)
{
    /* @0x14001a694 size=101 首次调用切到路径所在目录 */
    if (*flag == 1) {
        LPWSTR p = StrRChrW(path, (LPCWSTR)0, L'\\');
        if (p != NULL) {
            WCHAR *dir = NULL;
            FUN_1400702D4(&dir, path,
                          (((int64_t)(uintptr_t)p - (int64_t)(uintptr_t)path) >> 1) + 1);
            SetCurrentDirectoryW(dir);
            *flag = 2;
            FUN_14005B104(&dir);
        }
    }
    return *flag;
}

void FUN_14001a6fc(LARGE_INTEGER param_1)
{
    /* @0x14001a6fc size=256 枚举 CDROM 盘并写入 CDROM%u 环境变量 */
    UINT UVar1;
    int cd_count = 0;
    int idx;
    WCHAR Drv[8] = {L'?', L':', L'\\', 0, 0, 0, 0, 0};
    WCHAR buf[256];

    for (idx = 0; idx < 26; idx++) {
        Drv[0] = (WCHAR)('A' + idx);
        UVar1 = GetDriveTypeW(Drv);
        if (UVar1 == 5) {   /* DRIVE_CDROM */
            /* 注意: wsprintfW 的 CDROM%u=%s 参数在反编译中丢失; 按 REVIEW §2 补回 */
            wsprintfW(buf,
                      WSTR("HKLM\\SYSTEM\\CurrentControlSet\\Control\\Session Manager\\Environment\\CDROM%u=%s"),
                      (unsigned)cd_count, Drv);
            FUN_1400C13F8((HKEY)(uintptr_t)g_Script, (HKEY)(uintptr_t)buf, '\0');
            wsprintfW(buf, WSTR("CDROM%u"), (unsigned)cd_count);
            cd_count = cd_count + 1;   /* 计数器在两次 wsprintfW 之后自增 */
            FUN_1400629B8((void *)g_Script, buf, Drv);
        }
    }
    if (cd_count > 0) {
        LARGE_INTEGER cmd;
        cmd.QuadPart = 0x14011edb8;
        FUN_14004c0bc((uint64_t)param_1.QuadPart, cmd, (void *)0, (void *)0, (void *)0);
    }
}

void PECMD_StartDownMode(UINT cmdShow)
{
    /* @0x14001a7fc size=61 启动 "PECMD.EXE DOWN" */
    char cmd[40];

    memcpy(cmd, "PECMD.EXE DOWN", 0xf);
    memset(cmd + 0xf, 0, sizeof(cmd) - 0xf);
    WinExec(cmd, cmdShow);
}

void PECMD_ReadPelogonReg(uint64_t name, uint16_t *buf, int maxWChars)
{
    /* @0x14001a83c size=108 读 PELOGON 宽字符串注册表值 */
    DWORD type = 1;
    int size = maxWChars * 2;
    LONG r;

    *buf = 0;
    r = ((LONG(*)(HKEY, LPCWSTR, LPCWSTR, DWORD *, void *, int *))g_pSHGetValueW)(
        (HKEY)(intptr_t)0xffffffff80000002, WSTR("SOFTWARE\\PELOGON"),
        (LPCWSTR)name, &type, buf, &size);
    if (r != 0) {
        *buf = 0;
    }
    buf[maxWChars] = 0;
}

void PECMD_RegisterDeviceNotify(HANDLE hwnd)
{
    /* @0x14001a918 size=81 注册设备通知 */
    uint8_t filter[0x20] = {0};

    *(DWORD *)filter = 0x20;
    *(DWORD *)(filter + 4) = 5;
    RegisterDeviceNotificationW(hwnd, filter, 0);
}

bool PECMD_CreateCallbackWindow(HINSTANCE param_1)
{
    /* @0x14001a96c size=144 */
    g_hwndCF78 = CreateWindowExW(0, WSTR("PELogon CallBack Window"),
                                    WSTR("PELogon(Created By Lxl1638)"), 0xcf0000,
                                    (int)0x80000000, 0, (int)0x80000000, 0,
                                    (HWND)0x0, (HMENU)0x0, param_1, (LPVOID)0x0);
    if ((g_runFlag & 1) != 0) {
        PECMD_RegisterDeviceNotify(g_hwndCF78);
        SetTimer(g_hwndCF78, 0x14, 90000, (void *)0x0);
    }
    return g_hwndCF78 != (HWND)0x0;
}

void FUN_14001a9fc(void)
{
    /* @0x14001a9fc size=236 读取桌面壁纸注册表并应用 */
    WCHAR *pWVar1;
    int iVar2;
    bool bVar3;
    uint32_t local_res8[2];   /* size 缓冲 */
    uint32_t local_res10[2];  /* type 缓冲 */
    WCHAR *local_res18 = NULL;   /* 值缓冲 */

    FUN_140063694(&local_res18, 0x209);
    pWVar1 = local_res18;
    pWVar1[0] = L'\0';
    pWVar1[1] = L'\0';
    pWVar1[2] = L'\0';
    pWVar1[3] = L'\0';
    local_res10[0] = 0;
    local_res8[0] = 0x410;
    *local_res18 = L'\0';
    iVar2 = ((LONG (*)(HKEY, LPCWSTR, LPCWSTR, uint32_t *, void *, uint32_t *))g_pSHGetValueW)(
        (HKEY)(intptr_t)0xffffffff80000001, WSTR("Control Panel\\Desktop"),
        WSTR("Wallpaper.PECMD"), local_res10, (void *)local_res18, local_res8);
    bVar3 = (iVar2 == 0);
    if ((!bVar3) || (*local_res18 == L'\0')) {
        pWVar1[0] = L'\0';
        pWVar1[1] = L'\0';
        pWVar1[2] = L'\0';
        pWVar1[3] = L'\0';
        local_res8[0] = 0x410;
        *local_res18 = L'\0';
        iVar2 = ((LONG (*)(HKEY, LPCWSTR, LPCWSTR, uint32_t *, void *, uint32_t *))g_pSHGetValueW)(
            (HKEY)(intptr_t)0xffffffff80000001, WSTR("Control Panel\\Desktop"),
            WSTR("Wallpaper"), local_res10, (void *)local_res18, local_res8);
        bVar3 = (iVar2 == 0);
    }
    if ((bVar3) && (*local_res18 != L'\0')) {
        bVar3 = FUN_140101e70(local_res18);
        if (bVar3) {
            PECMD_SetDesktopWallpaper((void *)local_res18, 1);
        }
    }
    FUN_14005B104(&local_res18);
}

void PECMD_AddTrayIcon(int64_t param)
{
    /* @0x14001aae8 size=87 非运行模式添加托盘图标，否则通知回调窗口 */
    if (g_runFlag == 0) {
        if (param != 0) {
            FUN_14005B900((char *)&g_hwndTray, (uint64_t)param,
                          WSTR("PECMD2012"), (HICON)0, 0x457);
        }
    } else if (g_hwndCF78 != (HWND)0) {
        SendMessageW(g_hwndCF78, 1099, 0, 0);
    }
}

uint16_t *PECMD_IsExplorerPath(uint16_t *path)
{
    /* @0x14001ab40 size=66 检查路径文件名是否以 "Explorer." 开头 */
    uint16_t *p = PECMD_FindLastPathSep(path);
    int r = FUN_14005C788("Explorer", p, 8);

    if (r != 0 || (p[8] != L'.' && p[8] != 0)) {
        return NULL;
    }
    return p;
}

uint64_t PECMD_GetPELogonWindowValue(uint64_t name)
{
    /* @0x14001ac5c size=94 读 PELOGON 中的窗口句柄值 */
    DWORD type = 0xb;
    DWORD size = 8;
    uint64_t value = 0;
    LONG r;

    r = ((LONG(*)(HKEY, LPCWSTR, LPCWSTR, DWORD *, void *, DWORD *))g_pSHGetValueW)(
        (HKEY)(intptr_t)0xffffffff80000002, WSTR("SOFTWARE\\PELOGON"),
        (LPCWSTR)name, &type, &value, &size);
    return r == 0 ? value : 0;
}

uint64_t PECMD_ReadValidWindowHandle(uint64_t name)
{
    /* @0x14001acbc size=45 读窗口句柄并校验仍有效 */
    HWND h = (HWND)(uintptr_t)PECMD_GetPELogonWindowValue(name);

    if (h != (HWND)0 && IsWindow(h)) {
        return (uint64_t)(uintptr_t)h;
    }
    return 0;
}

void PECMD_RegisterCallbackWnd(int param)
{
    /* @0x14001acec size=68 处理回调窗口名注册/校验 */
    uint64_t h;

    if (param != 0 &&
        (h = PECMD_ReadValidWindowHandle((uint64_t)(uintptr_t)PTR_u_CallBackhWnd_14011e668_2_14013a288)) != 0) {
        return;
    }
    h = PECMD_ReadValidWindowHandle((uint64_t)(uintptr_t)L"UCallBackhWnd");
    if (h == 0 && param == 0) {
        PECMD_ReadValidWindowHandle((uint64_t)(uintptr_t)PTR_u_CallBackhWnd_14011e668_2_14013a288);
    }
}

uint64_t FUN_14001ad30(LPCWSTR arg)
{
    /* @0x14001ad30 size=120 根据参数切换 NumLock 状态 */
    int value = 0;
    BYTE keys[256] = {0};
    bool isZero;

    StrToIntExW(arg, 0, &value);
    isZero = (value == 0);
    GetKeyboardState(keys);
    if (isZero) {
        if ((keys[0x90] & 1) == 0) {
            return 0;
        }
    } else if ((keys[0x90] & 1) != 0) {
        return 0;
    }
    keybd_event(0x90, 'E', 1, 0);
    keybd_event(0x90, 'E', 3, 0);
    return 0;
}

uint64_t PECMD_SetRegistryOwnerRun(int64_t param_1, char param_2)
{
    /* @0x14001ada8 size=346 设置注册表 Owner/Run (名称,组织) */
    int iVar2;
    LPWSTR pWVar3;
    int64_t lVar4;
    LPCWSTR pWVar1;
    LPCWSTR pwVar5;
    LPCWSTR pwVar7;
    uint64_t uVar6;
    LPCWSTR local_res8;
    LPCWSTR local_res18;
    LPCWSTR local_res20;

    FUN_140063620((WCHAR **)&local_res20);
    FUN_140063620((WCHAR **)&local_res18);
    pWVar3 = StrRChrW((LPCWSTR)(param_1 + 2), (LPCWSTR)0x0, L',');
    uVar6 = 0;
    if (pWVar3 == (LPWSTR)0x0) {
        uVar6 = 1;
    } else {
        *pWVar3 = L'\0';
        FUN_14007bf44((int64_t *)(intptr_t)g_Script, (WCHAR *)(param_1 + 2), (void *)&local_res20,
                      0, 1);
        local_res8 = local_res20;
        FUN_14007bf44((int64_t *)(intptr_t)g_Script, pWVar3 + 1, (void *)&local_res18, 0, 1);
        if (param_2 == '\0') {
            lVar4 = PECMD_WideStrLen(local_res20);
            FUN_14005c5a0(HKEY_LOCAL_MACHINE,
                          WSTR("SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion"),
                          WSTR("RegisteredOwner"), 1, (BYTE *)local_res20, (int)lVar4 * 2);
            lVar4 = PECMD_WideStrLen(local_res18);
            pwVar7 = WSTR("RegisteredOrganization");
            pwVar5 = WSTR("SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion");
            iVar2 = (int)lVar4;
        } else {
            FUN_14005B154((WCHAR **)&local_res8);
            pWVar1 = local_res8;
            lVar4 = PECMD_WideStrLen(local_res8);
            pwVar5 = WSTR("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run");
            iVar2 = (int)lVar4;
            pwVar7 = local_res18;
            local_res18 = pWVar1;
        }
        FUN_14005c5a0(HKEY_LOCAL_MACHINE, pwVar5, pwVar7, 1, (BYTE *)local_res18, iVar2 * 2);
    }
    FUN_14005B104((WCHAR **)&local_res18);
    FUN_14005B104((WCHAR **)&local_res20);
    return uVar6;
}

uint64_t FUN_14001af14(void)
{
    /* @0x14001af14 size=103 */
    int iVar1;
    int iVar2;

    FUN_140017b8c();
    do {
        g_ramdrivFlag = ((int (*)(void *, void *, void *, void *))DAT_14013cf00)
                            (&g_u64e770, 0, 0, 0);
    } while (g_ramdrivFlag == -1);
    iVar2 = 0;
    g_u64CB60 = 0x20;
    while (1) {
        iVar1 = ((int (*)(int, int, void *))DAT_14013cf28)
                    (g_ramdrivFlag, iVar2, &g_u64CB60);
        if (iVar1 == 0) break;
        iVar2 = iVar2 + 1;
    }
    return 3;
}

bool FUN_14001af7c(uint32_t param_1)
{
    /* @0x14001af7c size=104 (not in this batch; signature fixed from decompile) */
    (void)param_1;
    return 0;
}

uint64_t PECMD_SetRamdrivDiskSize(int param_1, LPCWSTR param_2)
{
    /* @0x14001afe4 size=386 设置 Ramdriv DiskSize 并触发重枚举 */
    DWORD DVar1;
    int iVar2;
    uint64_t uVar3;
    int64_t lVar4;
    uint32_t uVar5;
    int local_res8[2];
    int local_res18[2];
    DWORD local_res20[2];
    DWORD local_28[4];

    uVar5 = 0;
    local_res18[0] = 0;
    local_res8[0] = param_1;
    FUN_140017b8c();
    uVar3 = FUN_14001af14();
    if (((int)uVar3 != 0) && ((int)uVar3 != 1)) {
        local_28[0] = 4;
        local_res20[0] = 4;
        DVar1 = FUN_14005C4E0((HKEY)(intptr_t)0xffffffff80000002,
                              WSTR("System\\CurrentControlSet\\Services\\Ramdriv\\Parameters"),
                              WSTR("DiskSize"),
                              local_28, (BYTE *)local_res18, local_res20);
        if (DVar1 == 0) {
            if (local_res8[0] == local_res18[0]) {
                return 1;
            }
            local_res20[0] = 4;
            DVar1 = FUN_14005c5a0((HKEY)(intptr_t)0xffffffff80000002,
                                  WSTR("System\\CurrentControlSet\\Services\\Ramdriv\\Parameters"),
                                  WSTR("DiskSize"),
                                  4, (BYTE *)local_res8, 4);
            if ((DVar1 == 0) && (lVar4 = PECMD_WideStrLen(param_2), 1 < (uint32_t)lVar4)) {
                do {
                    FUN_14001af7c(1);
                    FUN_14001af7c(2);
                    PECMD_QueryDiskSpace(param_2);
                    DVar1 = GetLastError();
                    if (DVar1 == 3) break;
                    uVar5 = uVar5 + 1;
                } while (uVar5 < 10);
                if (uVar5 != 10) {
                    uVar5 = 0;
                    do {
                        FUN_14001af7c(2);
                        FUN_14001af7c(1);
                        PECMD_QueryDiskSpace(param_2);
                        DVar1 = GetLastError();
                        if (DVar1 != 3) break;
                        uVar5 = uVar5 + 1;
                    } while (uVar5 < 10);
                    if (uVar5 != 10) {
                        do {
                            PECMD_QueryDiskSpace(param_2);
                            /* TODO(verify): 反编译残基 (PECMD_QueryDiskSpace 返回 void) */
                            iVar2 = (int)GetLastError();
                            if (iVar2 == 0) {
                                if (g_ramdrivFlag == -1) {
                                    return 1;
                                }
                                ((void (*)(void))(uintptr_t)DAT_14013cf10)();
                                return 1;
                            }
                            DVar1 = GetLastError();
                        } while (DVar1 == 3);
                    }
                }
            }
        }
    }
    if (g_ramdrivFlag != -1) {
        ((void (*)(void))(uintptr_t)DAT_14013cf10)();
    }
    return 0;
}

uint64_t FUN_14001B168(uint16_t *param_1, int param_2)
{
    /* @0x14001b168 size=211 (尚为桩，签名已按反编译修正) */
    (void)param_1;
    (void)param_2;
    return 0;
}

LPCWSTR FUN_14001b23c(int64_t param_1, uint64_t *param_2, uint16_t *param_3,
                      int64_t *param_4, char param_5)
{
    /* @0x14001b23c size=274 从表项中截取一段并用异或键拷贝到新串 */
    int16_t sVar1;
    LPCWSTR pWVar2;
    int iVar3;
    int16_t *psVar4;
    uint16_t *puVar5;

    if ((*param_4 == 0) && (*param_4 = (int64_t)param_3,
        *param_3 != *(uint16_t *)(param_1 + 0x88))) {
        for (;;) {
            sVar1 = *(int16_t *)*param_4;
            if (sVar1 == *(int16_t *)(param_1 + 0x90)) break;
            if ((sVar1 == *(int16_t *)(param_1 + 0x8a)) ||
                (psVar4 = (int16_t *)*param_4 + 1,
                 *param_4 = (int64_t)psVar4,
                 *psVar4 == *(int16_t *)(param_1 + 0x88))) break;
        }
    }
    for (; (param_3 <= (uint16_t *)*param_4 &&
            (*param_3 == *(uint16_t *)(param_1 + 0x92) ||
             *param_3 == *(uint16_t *)(param_1 + 0x94)));
         param_3 = param_3 + 1) {
    }
    iVar3 = (int)(((int64_t)*param_4 - (int64_t)(intptr_t)param_3) >> 1);
    PECMD_AllocString((WCHAR **)param_2, (int64_t)(iVar3 + 6));
    puVar5 = (uint16_t *)*param_2;
    for (; param_3 < (uint16_t *)*param_4; param_3 = param_3 + 1) {
        *puVar5 = *(uint16_t *)(param_1 + 0x48) ^ *param_3;
        puVar5 = puVar5 + 1;
    }
    *puVar5 = 0;
    puVar5[1] = 0;
    puVar5[2] = 0;
    pWVar2 = (LPCWSTR)*param_2;
    if ((g_charTableF > 0) && (*(char *)(param_1 + 0xe) == '\0')) {
        FUN_14001B168((uint16_t *)pWVar2, iVar3);
    }
    if (param_5 == '\0') {
        PECMD_StripTrailingSpaces(pWVar2);
    }
    return pWVar2;
}

int16_t FUN_14001b350(int64_t obj, uint16_t *s, int16_t *pattern, int count)
{
    /* @0x14001b350 size=78 带异或键的不区分大小写比较 */
    int i = count - 1;
    int16_t pc;
    uint16_t c;

    if (i >= 0) {
        do {
            pc = *pattern++;
            c = (*s++ ^ *(uint16_t *)(obj + 0x48));
            if ((uint16_t)(pc - 0x41) < 0x1a) {
                pc = (int16_t)(pc + 0x20);
            }
            if ((uint16_t)(c - 0x41) < 0x1a) {
                c = (uint16_t)(c + 0x20);
            }
            if ((uint16_t)(pc - c) != 0) {
                return pc - c;
            }
            i--;
        } while (i >= 0);
    }
    return 0;
}

void FUN_14001b494(int64_t obj)
{
    /* @0x14001b494 size=99 沿链表释放记录并释放中转缓冲 */
    int64_t local_res8[4];
    int i = 0;

    if (*(int64_t *)(obj + 0x40) != 0) {
        FUN_1400633A8((void **)&local_res8, 0x3ffff0);
        do {
            if ((*(uint8_t *)(obj + 0xd9) & 1) != 0) {
                goto lbl;
            }
            *(uint64_t *)(obj + 0x40) = 0;
            obj = *(int64_t *)(obj + 0x38);
            i++;
        } while (i < 0x7fffe);
        if ((*(uint8_t *)(obj + 0xd9) & 1) != 0) {
lbl:
            FUN_1400E56E4((uint64_t *)(obj + 0x40));
            *(uint64_t *)(obj + 0x40) = 0;
        }
        FUN_14005B104((WCHAR **)&local_res8);
    }
}

uint32_t PECMD_ReadRamdataDword(uint64_t name)
{
    /* @0x14001b608 size=88 读 PELOGON RAMDATA DWORD 值 */
    uint32_t value = 0;
    DWORD type = 0;
    DWORD size = 4;
    LONG r;

    r = ((LONG(*)(HKEY, LPCWSTR, LPCWSTR, DWORD *, void *, DWORD *))g_pSHGetValueW)(
        (HKEY)(intptr_t)0xffffffff80000002, WSTR("SOFTWARE\\PELOGON\\RAMDATA"),
        (LPCWSTR)name, &type, &value, &size);
    return r == 0 ? value : 0;
}

LRESULT FUN_14001b6b4(int param_1, WPARAM param_2, int *param_3)
{
    /* @0x14001b6b4 size=257 键盘钩子回调(TaskMgr 屏蔽) */
    uint16_t uVar1;
    LRESULT LVar2;
    uint64_t local_res20;   /* FILETIME 槽(8字节) */

    g_randState = g_randState + 0x13;
    if ((((param_1 == 0) && (param_2 == 0x100)) && (*param_3 == 0x2e)) &&
        ((uVar1 = GetAsyncKeyState(0x11), (uVar1 & 0x8000) != 0) &&
         (uVar1 = GetAsyncKeyState(0x12), (uVar1 & 0x8000) != 0))) {
        uVar1 = GetAsyncKeyState(0x10);
        if ((uVar1 & 0x8000) == 0) {
            FUN_1400702B0((WCHAR **)&local_res20, WSTR("TaskMgr.exe"));
            FUN_14000e26c((uint64_t)(uintptr_t)g_Script, local_res20,
                          (uint64_t)(uintptr_t)g_Script, 0, 0, NULL, 0, NULL);
            FUN_14005B104((WCHAR **)&local_res20);
        } else {
            PostQuitMessage(0x2e);
        }
        return 1;
    }
    LVar2 = CallNextHookEx((HHOOK)(uintptr_t)g_hMsgHook, param_1, param_2,
                           (LPARAM)param_3);
    return LVar2;
}

uint64_t PECMD_WritePELogonDword(uint64_t unused, LPCWSTR name, uint32_t value)
{
    /* @0x14001b7b8 size=58 写 PELOGON DWORD 注册表值 */
    uint32_t data = value;
    (void)unused;
    FUN_14005c5a0((HKEY)(intptr_t)0xffffffff80000002, WSTR("SOFTWARE\\PELOGON"),
                  name, 4, (BYTE *)&data, 4);
    return 0;
}

uint64_t PECMD_ReadPELogonDword(LPCWSTR name)
{
    /* @0x14001b7f4 size=90 读 PELOGON USRCFG DWORD 值 */
    uint32_t value = 0;
    DWORD type = 4;
    DWORD size = 4;
    DWORD r;

    r = FUN_14005C4E0((HKEY)(intptr_t)0xffffffff80000002,
                      WSTR("SOFTWARE\\PELOGON\\USRCFG"), name,
                      &type, (BYTE *)&value, &size);
    if (r != 0) {
        return 0x8000000000000000ULL;
    }
    return value;
}

uint64_t PECMD_WriteNumberToScriptVar(LPCWSTR key, uint64_t value, int64_t *script)
{
    /* @0x14001ba3c size=71 格式化数值并写入变量 */
    WCHAR buf[64] = {0};

    FUN_1400E6D74(buf, value);
    FUN_1400629B8(script, key, buf);
    return 0;
}

DWORD FUN_14001ba84(DWORD param_1, int param_2)
{
    /* @0x14001ba84 size=172 向上查找第 param_2 级父进程 PID */
    DWORD DVar1;
    int iVar2;
    HANDLE hObject;
    PROCESSENTRY32W pe;

    if (param_1 == 0) {
        param_1 = GetCurrentProcessId();
    }
    do {
        memset(&pe, 0, 0x230);
        pe.dwSize = (DWORD)sizeof(pe);
        hObject = (HANDLE)CreateToolhelp32Snapshot(2, param_1);
        iVar2 = Process32FirstW(hObject, &pe);
        while ((DVar1 = 0, iVar2 != 0 &&
                (DVar1 = pe.th32ParentProcessID, pe.th32ProcessID != param_1))) {
            iVar2 = Process32NextW(hObject, &pe);
        }
        param_1 = DVar1;
        CloseHandle(hObject);
        param_2 = param_2 - 1;
    } while (0 < param_2);
    return param_1;
}

void FUN_14001bbac(uint64_t unused, DWORD count, HANDLE *handles,
                   BOOL waitAll, DWORD timeout, DWORD mask)
{
    /* @0x14001bbac size=45 MsgWaitForMultipleObjects 包装 */
    (void)unused;
    MsgWaitForMultipleObjects(count, handles, waitAll, timeout, mask);
}

void FUN_14001bc7c(int64_t *param_1, int param_2, int param_3)
{
    /* @0x14001bc7c size=206 PELogon 窗口存活状态上报 (SendMessage 0x111) */
    HWND pHVar1;
    HWND hWnd;
    int64_t lVar2;

    EnterCriticalSection((void *)&g_csInit);
    pHVar1 = FUN_14001957c();
    lVar2 = 0;
    hWnd = g_hPelogonWnd;
    if (g_hPelogonWnd == (HWND)0x0) {
        if (pHVar1 == (HWND)0x0) {
            LeaveCriticalSection((void *)&g_csInit);
            return;
        }
        hWnd = pHVar1;
        if (pHVar1 != (HWND)0x0) goto LAB_14001bceb;
    }
    if ((param_2 == 0) && (param_3 == 0)) {
        lVar2 = 1;
    }
    *param_1 = lVar2;
LAB_14001bceb:
    FUN_14005c61c((HKEY)(intptr_t)0xffffffff80000002,
                  WSTR("SOFTWARE\\PELOGON"), WSTR("LogohWnd"));
    LeaveCriticalSection((void *)&g_csInit);
    SendMessageW(hWnd, 0x111,
                 (WPARAM)(0x67 - (uint64_t)(param_2 != 0)),
                 (LPARAM)(-(uint64_t)(*param_1 != 0) & (uint64_t)(uintptr_t)param_1));
}

void FUN_14001bd4c(int64_t param_1)
{
    /* @0x14001bd4c size=199 线程/对象卸载例程 */
    int64_t *p;

    EnterCriticalSection(&g_csInit);
    p = *(int64_t **)(param_1 + 0x28);
    if (*p != 0) {
        FUN_1400185c8(1, p);
        EnterCriticalSection(&g_csInit);
        g_i64CCB8 = g_i64CCB8 + (-1);
        LeaveCriticalSection(&g_csInit);
        CloseHandle((HANDLE)(uintptr_t)*p);
        *p = 0;
    }
    *(uint64_t *)((int64_t)p + 0x20) = 0xfffffffffff80000ULL;
    *(uint8_t *)((int64_t)p + 0x29) = *(uint8_t *)(param_1 + 0x21);
    *(uint8_t *)((int64_t)p + 0x28) = *(uint8_t *)(param_1 + 0x20);
    *(uint64_t *)((int64_t)p + 0x20) = *(uint64_t *)(param_1 + 8);
    *p = *(uint64_t *)(param_1 + 0x10);
    FUN_1400629B8(g_Script, WSTR("&__LOGS"), *(LPCWSTR *)(param_1 + 0x18));
    FUN_14005B0D4((void *)(uintptr_t)(param_1 + 0x18));
    LeaveCriticalSection(&g_csInit);
}

void FUN_14001be68(uint16_t **pp)
{
    /* @0x14001be68 size=64 跳过标识符字符 */
    uint16_t *p;
    uint16_t ch;

    for (;;) {
        p = *pp;
        ch = *p | 0x20;
        if ((ch < 0x100 && *p != L'_' && (ch < L'a' || ch > L'z')) &&
            (*p < L'0' || *p > L'9')) {
            break;
        }
        *pp = p + 1;
    }
}

int64_t FUN_14001c3ac(LPCWSTR param_1, DWORD param_2, HANDLE param_3)
{
    /* @0x14001c3ac size=158 */
    BOOL BVar1;
    int64_t lVar2;
    HANDLE local_res20;
    _TOKEN_PRIVILEGES local_18;

    local_res20 = (HANDLE)0x0;
    BVar1 = OpenProcessToken(param_3, 0x28, &local_res20);
    if (BVar1 == 0) {
        lVar2 = 0;
    } else {
        local_18.PrivilegeCount = 1;
        local_18.Privileges[0].Attributes = param_2;
        EnterCriticalSection(g_csInit);
        LookupPrivilegeValueW((LPCWSTR)0x0, param_1, &local_18.Privileges[0].Luid);
        BVar1 = AdjustTokenPrivileges(local_res20, 0, &local_18, 0x10,
                                      (TOKEN_PRIVILEGES *)0x0, (DWORD *)0x0);
        lVar2 = (int64_t)BVar1;
        LeaveCriticalSection(g_csInit);
    }
    return lVar2;
}

bool PECMD_IsImDiskMountPointAvailable(void)
{
    /* @0x14001c44c size=13 全局标志非零 */
    return g_pImDiskCreateMountPoint != 0;
}

void PECMD_LoadUnloadImdisk(uint64_t param_1, int param_2)
{
    /* @0x14001c45c size=975 Imdisk.cpl 动态装载/卸载 */
    (void)param_1;
    if ((g_pImDiskCreateMountPoint == 0) || (param_2 != 0)) {
        if (g_hImdiskCpl != (HMODULE)0) {
            FreeLibrary(g_hImdiskCpl);
        }
        g_hImdiskCpl = (HMODULE)0;
        if (param_2 == -1) {
            g_pImDiskGetVersion = 0;
            g_pImDiskFindFreeLetter = 0;
            g_pImDiskForceRemove = 0;
            g_pImDiskMask = 0;
            g_pImDiskCtrl = 0;
            g_pImDiskGetOffsetByExt = 0;
            g_pImDiskGetPartInfo = 0;
            g_pImDiskOpenDevice = 0;
            g_pDevOpen = 0;
            g_pDevOpen2 = 0;
            g_pImDiskRemoveMountPoint = 0;
            g_pImDiskStartService = 0;
            g_pImDiskCreateMountPoint = 0;
        } else {
            g_hImdiskCpl = LoadLibraryA("Imdisk.cpl");
            EnterCriticalSection(&g_csInit);
            g_pImDiskGetVersion = 0;
            FUN_14005C828("ImDiskGetVersion", "Imdisk.cpl", (void **)&g_pImDiskGetVersion, &g_hImdiskCpl);
            g_pImDiskFindFreeLetter = 0;
            FUN_14005C828("ImDiskFindFreeDriveLetter", "Imdisk.cpl", (void **)&g_pImDiskFindFreeLetter, &g_hImdiskCpl);
            g_pImDiskForceRemove = 0;
            FUN_14005C828("ImDiskForceRemoveDevice", "Imdisk.cpl", (void **)&g_pImDiskForceRemove, &g_hImdiskCpl);
            g_pImDiskMask = 0;
            FUN_14005C828("ImDiskGetDeviceList", "Imdisk.cpl", (void **)&g_pImDiskMask, &g_hImdiskCpl);
            g_pImDiskCtrl = 0;
            FUN_14005C828("ImDiskGetDeviceListEx", "Imdisk.cpl", (void **)&g_pImDiskCtrl, &g_hImdiskCpl);
            g_pImDiskGetOffsetByExt = 0;
            FUN_14005C828("ImDiskGetOffsetByFileExt", "Imdisk.cpl", (void **)&g_pImDiskGetOffsetByExt, &g_hImdiskCpl);
            g_pImDiskGetPartInfo = 0;
            FUN_14005C828("ImDiskGetPartitionInformation", "Imdisk.cpl", (void **)&g_pImDiskGetPartInfo, &g_hImdiskCpl);
            g_pImDiskOpenDevice = 0;
            FUN_14005C828("ImDiskOpenDeviceByName", "Imdisk.cpl", (void **)&g_pImDiskOpenDevice, &g_hImdiskCpl);
            g_pDevOpen = 0;
            FUN_14005C828("ImDiskOpenDeviceByNumber", "Imdisk.cpl", (void **)&g_pDevOpen, &g_hImdiskCpl);
            g_pDevOpen2 = 0;
            FUN_14005C828("ImDiskOpenDeviceByMountPoint", "Imdisk.cpl", (void **)&g_pDevOpen2, &g_hImdiskCpl);
            g_pImDiskRemoveMountPoint = 0;
            FUN_14005C828("ImDiskRemoveMountPoint", "Imdisk.cpl", (void **)&g_pImDiskRemoveMountPoint, &g_hImdiskCpl);
            g_pImDiskStartService = 0;
            FUN_14005C828("ImDiskStartService", "Imdisk.cpl", (void **)&g_pImDiskStartService, &g_hImdiskCpl);
            g_pImDiskCreateMountPoint = 0;
            FUN_14005C828("ImDiskCreateMountPoint", "Imdisk.cpl", (void **)&g_pImDiskCreateMountPoint, &g_hImdiskCpl);
            FUN_14005C828("_ImDiskGetVersion@8", "Imdisk.cpl", (void **)&g_pImDiskGetVersion, &g_hImdiskCpl);
            FUN_14005C828("_ImDiskFindFreeDriveLetter@0", "Imdisk.cpl", (void **)&g_pImDiskFindFreeLetter, &g_hImdiskCpl);
            FUN_14005C828("_ImDiskForceRemoveDevice@8", "Imdisk.cpl", (void **)&g_pImDiskForceRemove, &g_hImdiskCpl);
            FUN_14005C828("_ImDiskGetDeviceList@0", "Imdisk.cpl", (void **)&g_pImDiskMask, &g_hImdiskCpl);
            FUN_14005C828("_ImDiskGetDeviceListEx@8", "Imdisk.cpl", (void **)&g_pImDiskCtrl, &g_hImdiskCpl);
            FUN_14005C828("_ImDiskGetOffsetByFileExt@8", "Imdisk.cpl", (void **)&g_pImDiskGetOffsetByExt, &g_hImdiskCpl);
            FUN_14005C828("_ImDiskGetPartitionInformation@16", "Imdisk.cpl", (void **)&g_pImDiskGetPartInfo, &g_hImdiskCpl);
            FUN_14005C828("_ImDiskOpenDeviceByName@8", "Imdisk.cpl", (void **)&g_pImDiskOpenDevice, &g_hImdiskCpl);
            FUN_14005C828("_ImDiskOpenDeviceByNumber@8", "Imdisk.cpl", (void **)&g_pDevOpen, &g_hImdiskCpl);
            FUN_14005C828("_ImDiskOpenDeviceByMountPoint@8", "Imdisk.cpl", (void **)&g_pDevOpen2, &g_hImdiskCpl);
            FUN_14005C828("_ImDiskRemoveMountPoint@4", "Imdisk.cpl", (void **)&g_pImDiskRemoveMountPoint, &g_hImdiskCpl);
            FUN_14005C828("_ImDiskStartService@4", "Imdisk.cpl", (void **)&g_pImDiskStartService, &g_hImdiskCpl);
            FUN_14005C828("_ImDiskCreateMountPoint@8", "Imdisk.cpl", (void **)&g_pImDiskCreateMountPoint, &g_hImdiskCpl);
            LeaveCriticalSection(&g_csInit);
        }
    }
}

uint64_t FUN_14001c82c(void)
{
    /* @0x14001c82c size=16 固定返回 1 */
    return 1;
}

bool FUN_14001c83c(HANDLE hDevice)
{
    /* @0x14001c83c size=98 查询设备控制状态 */
    DWORD returned = 0;
    int out[4] = {0};
    BOOL ok;

    ok = DeviceIoControl(hDevice, 0x83722000, NULL, 0,
                         out, 4, &returned, NULL);
    if (!ok) {
        GetLastError();
        return false;
    }
    if (returned > 3) {
        return out[0] == 0x103;
    }
    return false;
}

uint32_t FUN_14001c8a0(uint64_t param_1)
{
    /* @0x14001c8a0 size=176 设备 I/O 控制查询 */
    uint32_t uVar1;
    BOOL BVar2;
    HANDLE hDevice;
    DWORD local_res10[2];
    uint64_t *local_res18;

    hDevice = (HANDLE)((HANDLE (*)(uint64_t, uint32_t))(uintptr_t)g_pDevOpen2)(param_1, 0x80);
    if (hDevice == (HANDLE)0xffffffffffffffff) {
        uVar1 = 0xfffffff7;
    } else {
        FUN_1400633A8((void **)&local_res18, 0x448);
        memset(local_res18, 0, 0x448);
        local_res10[0] = 0;
        BVar2 = DeviceIoControl(hDevice, 0x83722008, (LPVOID)0, 0, local_res18, 0x448,
                                local_res10, (void *)0);
        CloseHandle(hDevice);
        uVar1 = 0xfffffffe;
        if (BVar2 != 0) {
            uVar1 = (uint32_t)*local_res18;
        }
        FUN_14005B104((WCHAR **)&local_res18);
    }
    return uVar1;
}

int PECMD_EnumImDiskDrives(uint64_t param_1, int64_t *param_2, short param_3)
{
    /* @0x14001c950 size=549 枚举 ImDisk 虚拟盘并追加盘符列表到 param_2 */
    DWORD DVar1;
    HANDLE pvVar2;
    uint32_t *lpMem;
    int64_t lVar3;
    uint32_t uVar4;
    uint64_t uVar5;
    int iVar6 = 0;
    int iVar7 = 0;
    int iVar8 = 0;
    uint32_t uVar9;
    int64_t local_res10[2];
    int64_t local_res20;
    WCHAR local_108[104];
    (void)param_1;

    uVar5 = 300;
    uVar9 = (uint32_t)(*(short *)(*param_2 + 2) == 0);
    iVar8 = iVar6;
    if (g_pImDiskCtrl == (void *)0) {
        for (uVar5 = ((uint64_t (*)(void))g_pImDiskMask)();
             uVar5 != 0; uVar5 = uVar5 >> 1) {
            if ((uVar5 & 1) != 0) {
                /* 反编译中 wsprintfW 的 %s%u 实参丢失, 此处照低保留格式串 */
                wsprintfW(local_108, WSTR(" %s%u\r\n"));
                FUN_14006375C((WCHAR **)param_2, local_108 + (int)uVar9);
            }
        }
    } else {
        do {
            pvVar2 = GetProcessHeap();
            lpMem = HeapAlloc(pvVar2, 0, uVar5 << 2);
            if (lpMem == (uint32_t *)0) {
                return 0;
            }
            *lpMem = 0;
            lVar3 = ((int64_t (*)(uint64_t))g_pImDiskCtrl)(uVar5);
            if (lVar3 != 0) {
                if (*lpMem != 0) {
                    uVar4 = 1;
                    do {
                        wsprintfW(local_108, WSTR(" %s%u\r\n"));
                        FUN_14006375C((WCHAR **)param_2, local_108 + (int)uVar9);
                        uVar4 = uVar4 + 1;
                    } while (uVar4 <= *lpMem);
                }
                goto LAB_14001cb5a;
            }
            DVar1 = GetLastError();
            if (DVar1 == 2) {
                iVar7 = iVar6;
                if (param_3 != 0) {
                    FUN_1400702F0(&local_res20, "*?ImDiskNotLoad", 0xffffffffffffffff);
                    PECMD_ReplaceStringSlot(param_2, (uint64_t *)&local_res20);
                    FUN_14005B104((WCHAR **)&local_res20);
                }
                goto LAB_14001cb5a;
            }
            if (DVar1 != 0xea) {
                if (param_3 != 0) {
                    FUN_1400702F0(local_res10, "*?CannotControlImDiskDriver",
                                  0xffffffffffffffff);
                    PECMD_ReplaceStringSlot(param_2, (uint64_t *)local_res10);
                    FUN_14005B104((WCHAR **)local_res10);
                }
                iVar7 = -1;
LAB_14001cb5a:
                pvVar2 = GetProcessHeap();
                HeapFree(pvVar2, 0, lpMem);
                return iVar7;
            }
            uVar5 = (uint64_t)(*lpMem + 1);
            pvVar2 = GetProcessHeap();
            HeapFree(pvVar2, 0, lpMem);
            iVar8 = iVar8 + 1;
        } while (iVar8 < 2);
    }
    return 0;
}

uint32_t PECMD_QueryImDiskVolumeInfo(uint32_t param_1, LPCWSTR param_2, int64_t *param_3,
                       uint64_t param_4)
{
    /* @0x14001cb78 size=1146 查询 ImDisk 卷信息并格式化输出行 */
    LPWSTR pszDest;
    bool bVar2;
    DWORD DVar3;
    BOOL BVar4;
    int iVar5;
    int iVar8;
    int64_t lVar1;
    HANDLE hDevice;
    uint32_t uVar6;
    uint32_t uVar7;
    WCHAR drivePath[4];
    WCHAR local_68[32];
    DWORD local_res20[2];
    uint8_t *local_80;

    local_res20[0] = 0;
    FUN_1400633A8((void **)&local_80, 0x448);
    if (local_80 == NULL) {
        perror("Memory allocation error");
        uVar6 = 0xffffffff;
        goto LAB_14001cfc5;
    }
    if (param_2 == NULL) {
        hDevice = ((HANDLE (*)(uint32_t, DWORD))g_pDevOpen)(param_1, 0x80);
    LAB_14001cc98:
        if (hDevice != (HANDLE)0xffffffffffffffff) goto LAB_14001cd15;
        DVar3 = GetLastError();
        if (DVar3 == 2) {
            uVar6 = 1;
            goto LAB_14001cfc5;
        }
    } else {
        iVar5 = lstrlenW(param_2);
        if (iVar5 == 2) {
            bVar2 = (param_2[1] == L':');
        } else {
            iVar5 = lstrlenW(param_2);
            if (iVar5 == 3) {
                iVar5 = wcscmp(param_2 + 1, WSTR(":\\"));
                bVar2 = (iVar5 == 0);
            } else {
                bVar2 = false;
            }
        }
        if (bVar2) {
            /* 重建盘符路径缓冲: 视 template DAT_14011f970 为 "X:\" */
            drivePath[0] = *param_2;
            drivePath[1] = L':';
            drivePath[2] = L'\\';
            drivePath[3] = L'\0';
            hDevice = CreateFileW(drivePath, 0x80, 3, (LPSECURITY_ATTRIBUTES)0, 3,
                                  0x20000000, (HANDLE)0);
            goto LAB_14001cc98;
        }
        hDevice = ((HANDLE (*)(LPCWSTR))g_pDevOpen2)(param_2);
        if (hDevice == (HANDLE)0xffffffffffffffff) {
            DVar3 = GetLastError();
            if (((DVar3 != 1) && (DVar3 != 0x57)) &&
                ((DVar3 == 0x91) || ((DVar3 == 0x10b) || (DVar3 == 0x1126)))) {
                FUN_140103a20();
                FUN_14001c82c();
            }
            uVar6 = 9;
            goto LAB_14001cfc5;
        }
    LAB_14001cd15:
        bVar2 = FUN_14001c83c(hDevice);
        if (!bVar2) {
            CloseHandle(hDevice);
            uVar6 = 5;
            goto LAB_14001cfc5;
        }
        BVar4 = DeviceIoControl(hDevice, 0x83722008, (LPVOID)0, 0, local_80, 0x448,
                                local_res20, (void *)0);
        if ((BVar4 != 0) && (0x35 < local_res20[0])) {
            CloseHandle(hDevice);
            uVar7 = (uint32_t)(*(uint16_t *)((uint8_t *)local_80 + 0x2e) >> 1);
            iVar5 = lstrlenW((LPCWSTR)(uintptr_t)*param_3);
            iVar8 = iVar5 + 0x20b + (int)uVar7;
            PECMD_AllocString((WCHAR **)param_3, (int64_t)(iVar8 + 0x7c));
            lVar1 = *param_3;
            pszDest = (LPWSTR)(lVar1 + (int64_t)iVar5 * 2);
            StrCpyNW(pszDest + 0x209, (LPCWSTR)(local_80 + 0x30), uVar7 + 1);
            local_68[0] = L'\0';
            if ((param_4 & 2) != 0) {
                wsprintfW(local_68, WSTR("%d "), *(uint32_t *)local_80);
            }
            FUN_1400E6D74((WCHAR *)(lVar1 + (int64_t)iVar8 * 2),
                          *(uint64_t *)(local_80 + 8));
            FUN_1400E6D74((WCHAR *)(lVar1 + (int64_t)(iVar8 + 0x3e) * 2),
                          *(uint64_t *)(local_80 + 0x20));
            /* wnsprintfW 的其余可变实参在反编译中丢失; 此处仅保留 local_68 */
            wnsprintfW(pszDest, 0x208,
                       WSTR("%s%s = %s\r\nSize: %s bytes %s%s%s%s%s%s.\r\noffset: %s bytes"),
                       local_68);
            uVar6 = 0;
            pszDest[0x208] = L'\0';
            goto LAB_14001cfc5;
        }
        FUN_140103a20();
        FUN_14001c82c();
        CloseHandle(hDevice);
    }
    uVar6 = 2;
LAB_14001cfc5:
    FUN_14005B104((WCHAR **)&local_80);
    return uVar6;
}

char PECMD_DevLockUnlock(uint64_t param_1, LPCWSTR param_2, uint32_t param_3,
                   uint32_t param_4)
{
    /* @0x14001cff4 size=759 打开磁盘/卷并执行 LOCK/UNLOCK 等 IoCtl 检测 */
    int iVar1;
    DWORD DVar2;
    BOOL BVar3;
    HANDLE hFile;
    bool bVar4;
    DWORD local_res10[4];
    uint32_t local_48;
    uint32_t local_44;
    WCHAR local_40[8] = {0, 0, 0, 0, 0, 0, 0, 0};

    if ((param_2 == (LPCWSTR)0) || ((int)param_1 != -1)) {
        hFile = ((HANDLE (*)(uint64_t, DWORD))g_pDevOpen)(param_1, 0xc0000000);
        if (hFile == (HANDLE)0xffffffffffffffff) {
            hFile = ((HANDLE (*)(uint64_t, DWORD))g_pDevOpen)
                        (param_1 & 0xffffffff, 0x80000000);
LAB_14001d1a6:
            if (hFile == (HANDLE)0xffffffffffffffff) {
                DVar2 = GetLastError();
                return (char)(DVar2 == 2);
            }
        }
    } else {
        iVar1 = lstrlenW(param_2);
        if (iVar1 == 2) {
            bVar4 = (param_2[1] == L':');
        } else {
            iVar1 = lstrlenW(param_2);
            bVar4 = false;
            if (iVar1 == 3) {
                iVar1 = wcscmp(param_2 + 1, WSTR(":\\"));
                bVar4 = (iVar1 == 0);
            }
        }
        if (bVar4) {
            /* 重建盘符路径缓冲: 视 template DAT_14011f970 为 "X:\" */
            local_40[0] = *param_2;
            local_40[1] = L':';
            local_40[2] = L'\\';
            hFile = CreateFileW(local_40, 0xc0000000, 3,
                                (LPSECURITY_ATTRIBUTES)0, 3, 0x20000000, (HANDLE)0);
            if (hFile == (HANDLE)0xffffffffffffffff) {
                hFile = CreateFileW(local_40, 0x80000000, 3,
                                    (LPSECURITY_ATTRIBUTES)0, 3, 0x20000000, (HANDLE)0);
                goto LAB_14001d1a6;
            }
        } else {
            hFile = ((HANDLE (*)(LPCWSTR, DWORD))g_pDevOpen2)(param_2, 0xc0000000);
            if ((hFile == (HANDLE)0xffffffffffffffff) &&
                (hFile = ((HANDLE (*)(LPCWSTR, DWORD))g_pDevOpen2)
                             (param_2, 0x80000000),
                 hFile == (HANDLE)0xffffffffffffffff)) {
                DVar2 = GetLastError();
                if (((DVar2 != 1) && (DVar2 != 0x57)) &&
                    (((DVar2 == 0x91) || (DVar2 == 0x10b)) || (DVar2 == 0x1126))) {
                    FUN_140103a20();
                    FUN_14001c82c();
                }
                return '\x01';
            }
        }
    }
    bVar4 = FUN_14001c83c(hFile);
    if (!bVar4) {
        bVar4 = true;
    } else {
        if ((param_3 & 3) != 0) {
            FlushFileBuffers(hFile);
            BVar3 = DeviceIoControl(hFile, 0x90018, (LPVOID)0, 0, (LPVOID)0, 0,
                                    local_res10, (void *)0);
            bVar4 = false;
            if ((BVar3 == 0) ||
                (BVar3 = DeviceIoControl(hFile, 0x90020, (LPVOID)0, 0, (LPVOID)0, 0,
                                         local_res10, (void *)0),
                 BVar3 == 0)) {
                goto LAB_14001d2c2;
            }
        }
        local_48 = param_3;
        local_44 = param_4;
        DeviceIoControl(hFile, 0x83722014, &local_48, 8, &local_48, 8,
                        local_res10, (void *)0);
        bVar4 = false;
        if (local_48 != 0) {
            CloseHandle(hFile);
            FUN_140103a20();
            FUN_14001c82c();
            return '\x01';
        }
    }
LAB_14001d2c2:
    CloseHandle(hFile);
    return (char)bVar4;
}

char PECMD_DevAlignCheck(uint64_t param_1, LPCWSTR param_2, uint64_t param_3)
{
    /* @0x14001d2ec size=775 打开磁盘/卷并执行 IoCtl 就绪/对齐检测 */
    int iVar1;
    DWORD DVar2;
    BOOL BVar3;
    HANDLE hDevice;
    bool bVar4;
    DWORD local_res10[2];
    uint64_t local_res18;
    int64_t local_res20;
    int64_t local_68;
    WCHAR drivePath[4];
    uint32_t local_50[2];
    uint64_t local_48;
    uint8_t local_40[0x18];
    uint32_t local_2c;

    local_50[0] = 0;
    local_48 = 0;
    local_res18 = param_3;
    if (param_2 == (LPCWSTR)0) {
        hDevice = ((HANDLE (*)(uint64_t, DWORD))g_pDevOpen)(param_1, 0xc0000000);
        if (hDevice != (HANDLE)0xffffffffffffffff) goto LAB_14001d4a2;
        hDevice = ((HANDLE (*)(uint64_t, DWORD))g_pDevOpen)(param_1 & 0xffffffff,
                                                               0x80000000);
    } else {
        iVar1 = lstrlenW(param_2);
        if (iVar1 == 2) {
            bVar4 = (param_2[1] == L':');
        } else {
            iVar1 = lstrlenW(param_2);
            bVar4 = false;
            if (iVar1 == 3) {
                iVar1 = wcscmp(param_2 + 1, WSTR(":\\"));
                bVar4 = (iVar1 == 0);
            }
        }
        if (!bVar4) {
            hDevice = ((HANDLE (*)(LPCWSTR, DWORD))g_pDevOpen2)(param_2, 0xc0000000);
            if ((hDevice == (HANDLE)0xffffffffffffffff) &&
                (hDevice = ((HANDLE (*)(LPCWSTR, DWORD))g_pDevOpen2)
                               (param_2, 0x80000000),
                 hDevice == (HANDLE)0xffffffffffffffff)) {
                DVar2 = GetLastError();
                if (((DVar2 != 1) && (DVar2 != 0x57)) &&
                    (((DVar2 == 0x91) || (DVar2 == 0x10b)) || (DVar2 == 0x1126))) {
                    FUN_140103a20();
                    FUN_14001c82c();
                }
                return '\x01';
            }
            goto LAB_14001d4a2;
        }
        /* 重建盘符路径缓冲: 视 template DAT_14011f970 为 "X:\" */
        drivePath[0] = *param_2;
        drivePath[1] = L':';
        drivePath[2] = L'\\';
        drivePath[3] = L'\0';
        hDevice = CreateFileW(drivePath, 0xc0000000, 3, (LPSECURITY_ATTRIBUTES)0, 3,
                              0x20000000, (HANDLE)0);
        if (hDevice != (HANDLE)0xffffffffffffffff) goto LAB_14001d4a2;
        hDevice = CreateFileW(drivePath, 0x80000000, 3, (LPSECURITY_ATTRIBUTES)0, 3,
                              0x20000000, (HANDLE)0);
    }
    if (hDevice == (HANDLE)0xffffffffffffffff) {
        DVar2 = GetLastError();
        return (char)(DVar2 == 2);
    }
LAB_14001d4a2:
    local_48 = local_res18;
    local_50[0] = 1;
    BVar3 = DeviceIoControl(hDevice, 0x7c0d0, local_50, 0x10, (LPVOID)0, 0, local_res10,
                            (void *)0);
    bVar4 = true;
    if (BVar3 != 0) {
        BVar3 = DeviceIoControl(hDevice, 0x7405c, (LPVOID)0, 0, &local_res20, 8,
                                local_res10, (void *)0);
        if ((BVar3 == 0) ||
            (BVar3 = DeviceIoControl(hDevice, 0x70000, (LPVOID)0, 0, local_40, 0x18,
                                     local_res10, (void *)0),
             BVar3 == 0)) {
            FUN_140103a20();
            FUN_14001c82c();
            bVar4 = false;
        } else {
            local_68 = local_res20 / (int64_t)(uint64_t)local_2c;
            DeviceIoControl(hDevice, 0x900f0, &local_68, 8, (LPVOID)0, 0, local_res10,
                            (void *)0);
            bVar4 = false;
        }
    }
    CloseHandle(hDevice);
    return (char)bVar4;
}

uint8_t *FUN_14001d744(uint8_t *dst, uint8_t *src, int len)
{
    /* @0x14001d744 size=71 支持重叠的字节搬移 */
    ptrdiff_t diff = dst - src;
    int i;
    uint8_t *p;

    if (diff < 0) {
        i = len - 1;
        p = dst;
        while (i >= 0) {
            *p = p[diff];
            p++;
            i--;
        }
    } else if (diff > 0) {
        i = len - 1;
        p = dst + len;
        while (i >= 0) {
            p--;
            *p = p[diff];
            i--;
        }
    }
    return dst;
}

uint8_t *FUN_14001d78c(uint8_t *dst, const uint8_t *src, int len)
{
    /* @0x14001d78c size=36 字节拷贝（保留原反编译的逐字节循环） */
    uint8_t *p;
    int i;

    if (dst != src && len - 1 >= 0) {
        p = dst;
        i = len - 1;
        do {
            *p = p[(src - dst)];
            p++;
            i--;
        } while (i >= 0);
    }
    return dst;
}

uint64_t FUN_14001d7b0(uint64_t param_1)
{
    /* @0x14001d7b0 size=96 */
    uint64_t local_res10[3];
    uint8_t local_48[16];
    uint32_t local_38[2];
    uint64_t local_30;
    uint8_t *local_28;
    uint32_t local_20;
    uint64_t local_18;
    uint64_t local_10;

    local_res10[0] = 0xffffffffffffffffULL;
    ((void (*)(uint8_t *, uint64_t))g_pRtlInitUnicodeString)(local_48, param_1);
    local_28 = local_48;
    local_30 = 0;
    local_18 = 0;
    local_10 = 0;
    local_38[0] = 0x30;
    local_20 = 0x40;
    ((int (*)(uint64_t *, int, uint32_t *))g_pZwOpenSection)(local_res10, 4, local_38);
    return local_res10[0];
}

int64_t PECMD_EnumNtSymbolicLink(LPWSTR param_1, int64_t *param_2, int64_t *param_3,
                      int64_t *param_4)
{
    /* @0x14001d8c8 size=689 枚举 NT 符号链接对象并收集名字 */
    LPWSTR psz1;
    bool bVar1 = false;
    bool bVar4 = false;
    int iVar2;
    int iVar5;
    int64_t lVar3;
    uint64_t buf = 0;                    /* 串缓冲(原 local_res10/uStackX_14) */
    HANDLE local_98;
    int64_t local_90 = 0;
    int64_t local_88 = 0;
    uint16_t local_80[4];
    uint16_t *local_78;
    uint8_t local_70[16];
    uint32_t local_60[2];
    uint64_t local_58;
    uint8_t *local_50;
    uint32_t local_48;
    uint64_t local_40;
    uint64_t local_38;

    if ((param_2 == (int64_t *)0) && (param_3 == (int64_t *)0) &&
        (param_4 == (int64_t *)0)) {
        lVar3 = 0;
    } else {
        FUN_14005C828("NtOpenSymbolicLinkObject", "NTDLL.DLL",
                      (void **)&g_pNtOpenSymLink, (HMODULE *)&local_90);
        FUN_14005C828("NtQuerySymbolicLinkObject", "NTDLL.DLL",
                      (void **)&g_pNtQuerySymLink, (HMODULE *)&local_90);
        FUN_140063694((WCHAR **)&buf, 0x2002);
        local_78 = (uint16_t *)(uintptr_t)buf;
        iVar5 = -1;
        psz1 = local_78 + 0x1001;
        if ((param_3 != (int64_t *)0) || (param_4 != (int64_t *)0)) {
            bVar1 = true;
        }
        for (;;) {
            local_98 = (HANDLE)0;
            local_80[0] = 0;
            local_80[1] = 0x2000;
            iVar5 = iVar5 + 1;
            *local_78 = 0;
            ((void (*)(void *, void *))g_pRtlInitUnicodeString)(local_70, param_1);
            local_50 = local_70;
            local_60[0] = 0x30;
            local_58 = 0;
            local_48 = 0x40;
            local_40 = 0;
            local_38 = 0;
            iVar2 = ((int (*)(HANDLE *, int, void *))g_pNtOpenSymLink)(
                &local_98, 1, local_60);
            if ((-1 < iVar2) && (local_98 != (HANDLE)0)) {
                iVar2 = ((int (*)(HANDLE, void *, int))g_pNtQuerySymLink)(
                    local_98, local_80, 0);
            }
            CloseHandle(local_98);
            if (-1 < iVar2) {
                *(uint16_t *)((uintptr_t)buf + local_80[0]) = 0;
            }
            if (iVar5 == 0) {
                if (param_2 != (int64_t *)0) {
                    PECMD_ReplaceStringSlot(param_2, (uint64_t *)&buf);
                }
                if (*(int16_t *)(uintptr_t)buf == 0) {
                    if (param_3 != (int64_t *)0) {
                        PECMD_ReplaceStringSlot(param_3, (uint64_t *)&buf);
                    }
                    if (param_4 != (int64_t *)0) {
                        PECMD_ReplaceStringSlot(param_4, (uint64_t *)&buf);
                    }
                }
            }
            if ((!bVar1) || (*(int16_t *)(uintptr_t)buf == 0)) break;
            if (param_3 != (int64_t *)0) {
                if (0 < iVar5) {
                    FUN_14006375C((WCHAR **)param_3, WSTR("\t"));
                }
                FUN_14006375C((WCHAR **)param_3, (LPCWSTR)(uintptr_t)buf);
            }
            if (param_4 != (int64_t *)0) {
                PECMD_ReplaceStringSlot(param_4, (uint64_t *)&buf);
            }
            StrCpyNW(psz1, (LPCWSTR)(uintptr_t)buf, 0x1000);
            local_78 = (uint16_t *)(uintptr_t)buf;
            param_1 = psz1;
        }
        if ((param_2 == (int64_t *)0) &&
            (param_2 = param_3, param_3 == (int64_t *)0 &&
             (param_2 = param_4, param_4 == (int64_t *)0))) {
            bVar4 = true;
            FUN_1400702B0((WCHAR **)&local_88, (LPCWSTR)0);
            param_2 = &local_88;
        }
        lVar3 = *param_2;
        if (bVar4) {
            FUN_14005B104((WCHAR **)&local_88);
        }
        FUN_14005B104((WCHAR **)&buf);
    }
    return lVar3;
}

uint8_t *PECMD_ReadPhysicalMemory(uint8_t *param_1, uint64_t param_2, uint64_t *param_3)
{
    /* @0x14001db7c size=390 */
    uint64_t uVar1;
    int iVar2;
    int iVar3;
    int iVar4;
    HANDLE hObject;
    uint8_t *puVar5;
    uint32_t uVar6;
    uint32_t local_res10[2];
    int64_t local_res18;
    uint64_t local_res20;
    uint8_t local_68[48];

    uVar1 = *param_3;
    iVar2 = (int)uVar1;
    iVar3 = (int)(param_2 & 0xfffffffffffff000ULL);
    puVar5 = (uint8_t *)0x0;
    *param_3 = 0;
    uVar6 = (uint32_t)((iVar2 + 0xfff + (int)param_2) & (-iVar3 - 0x1000U));
    FUN_14005C828("ZwOpenSection", "NTDLL.DLL", (void **)&g_pZwOpenSection, (HMODULE *)0x0);
    FUN_14005C828("ZwMapViewOfSection", "NTDLL.DLL", (void **)&g_pZwMapViewOfSection, (HMODULE *)0x0);
    FUN_14005C828("ZwUnmapViewOfSection", "NTDLL.DLL", (void **)&g_pZwUnmapViewOfSection, (HMODULE *)0x0);
    memcpy(local_68, WSTR("\\Device\\PhysicalMemory"), 0x2e);
    hObject = (HANDLE)FUN_14001d7b0((uint64_t)(uintptr_t)local_68);
    if (hObject != (HANDLE)0x0) {
        GetLastError();
        local_res18 = 0;
        local_res10[0] = uVar6;
        local_res20 = param_2 & 0xfffffffffffff000ULL;
        iVar4 = ((int (*)(HANDLE, uint64_t, int64_t *, int, uint32_t, uint64_t *,
                          uint32_t *, int, int, int))g_pZwMapViewOfSection)
                    (hObject, 0xffffffffffffffffULL, &local_res18, 0, uVar6, &local_res20,
                     local_res10, 1, 0, 2);
        if (iVar4 == 0) {
            FUN_14001d78c(param_1,
                          (uint8_t *)(((int)param_2 - iVar3) + local_res18), iVar2);
            *param_3 = uVar1;
            ((void (*)(uint64_t, int64_t))g_pZwUnmapViewOfSection)(0xffffffffffffffffULL,
                                                         local_res18);
            puVar5 = param_1;
        }
        CloseHandle(hObject);
    }
    return puVar5;
}

LPCWSTR FUN_14001dd04(LPCWSTR param_1, char param_2)
{
    /* @0x14001dd04 size=170 反斜杠-换行展开 */
    int iVar1;
    WCHAR *pWVar2;
    WCHAR *pWVar3;
    WCHAR *pWVar4;
    uint64_t uVar5;

    iVar1 = lstrlenW(param_1);
    pWVar4 = (WCHAR *)param_1 + iVar1 + 1;
    if (*param_1 != L'\0') {
        uVar5 = 0x5c;
        pWVar2 = (WCHAR *)param_1;
        do {
            if (pWVar4 <= pWVar2) {
                return param_1;
            }
            pWVar3 = pWVar2;
            if ((WCHAR)uVar5 == *pWVar2) {
                pWVar3 = pWVar2 + 1;
                if (*pWVar3 == L'n') {
                    *pWVar2 = L'\r';
                    *pWVar3 = L'\n';
                    goto LAB_14001dd60;
                }
                if ((param_2 == '\0') && ((WCHAR)uVar5 == *pWVar3)) {
                    FUN_14001d744((uint8_t *)pWVar3, (uint8_t *)(pWVar2 + 2),
                                  (int)((((int64_t)pWVar4 - (int64_t)pWVar3) >> 1) * 2));
                    pWVar4 = pWVar4 - 1;
                }
            } else {
LAB_14001dd60:
                pWVar3 = pWVar3 + 1;
            }
            pWVar2 = pWVar3;
        } while (*pWVar3 != L'\0');
    }
    return param_1;
}

uint8_t FUN_14001ddb0(LPCWSTR param_1, int64_t *param_2, int param_3)
{
    /* @0x14001ddb0 size=156 前缀匹配则推进指针并跳过空白 */
    uint8_t uVar3 = 0;
    WCHAR WVar1;
    LPCWSTR lpStr2;
    int iVar2;

    if (param_3 < 0) {
        param_3 = lstrlenW(param_1);
    }
    lpStr2 = (LPCWSTR)*param_2;
    iVar2 = StrCmpNIW(param_1, lpStr2, param_3);
    if (iVar2 == 0) {
        WVar1 = lpStr2[param_3];
        if (((((uint16_t)WVar1 > 8) && ((uint16_t)WVar1 < 0xe)) ||
             (WVar1 == L' ')) || (WVar1 == L'\0')) {
            *param_2 = *param_2 + (int64_t)param_3 * 2;
            uVar3 = 1;
            PECMD_SkipLeadingControls((uint64_t *)param_2);
        }
    }
    return uVar3;
}

void PECMD_ParseNumberWs(int64_t *param_1, double *param_2)
{
    /* @0x14001de4c size=36 解析数字并跳过前导空白/符号 */
    FUN_140082448(param_1, param_2);
    if (*(int16_t *)(uintptr_t)*param_1 != 0) {
        *param_1 = (int64_t)((int16_t *)(uintptr_t)*param_1 + 1);
    }
}

void PECMD_ParseNumAdvance(WCHAR **pp, uint64_t *out)
{
    /* @0x14001de70 size=36 解析数字后前进一个字符 */
    PECMD_ParseNumSkipWs(pp, out);
    if (**(uint16_t **)pp != 0) {
        *pp = (WCHAR *)((uint16_t *)*pp + 1);
    }
}

int FUN_14001de94(LPCWSTR param_1)
{
    /* @0x14001de94 size=190 统计并移除 '^*' 转义序列个数 */
    int iVar1;
    int iVar2;
    LPWSTR pWVar3;
    LPWSTR pWVar4;

    pWVar3 = (LPWSTR)(uintptr_t)StrStrW(param_1, WSTR("^*"));
    if (pWVar3 == (LPWSTR)0x0) {
        iVar1 = 0;
    }
    else {
        iVar1 = lstrlenW(pWVar3 + 2);
        pWVar4 = pWVar3 + 1;
        FUN_14001d744((uint8_t *)pWVar4, (uint8_t *)(pWVar3 + 2), iVar1 * 2 + 2);
        iVar1 = 1;
        for (; *pWVar4 != L'\0'; pWVar4 = pWVar4 + 1) {
            if ((*pWVar4 == L'^') && (pWVar4[1] == L'*')) {
                iVar2 = lstrlenW(pWVar4 + 2);
                FUN_14001d744((uint8_t *)(pWVar4 + 1), (uint8_t *)(pWVar4 + 2),
                              iVar2 * 2 + 2);
                iVar1 = iVar1 + 1;
            }
        }
    }
    return iVar1;
}

uint16_t *PECMD_GetStringToken(int64_t *param_1, uint16_t param_2)
{
    /* @0x14001df54 size=304 字符串取引号/空白分隔 token */
    uint16_t uVar1;
    bool bVar2;
    uint16_t uVar3;
    uint16_t *puVar4;
    uint16_t *puVar5;

    puVar4 = (uint16_t *)(uintptr_t)*param_1;
    if (param_2 == 0x1a2) {
        uVar3 = *puVar4;
        if ((((uVar3 == 0x22) || (uVar3 == 0x27)) && (uVar1 = puVar4[1], uVar1 != 0)) &&
            ((uVar1 != 0x22 && (puVar5 = puVar4, uVar1 != 0x27)))) goto LAB_14001e002;
        param_2 = 0x22;
    }
    uVar3 = *puVar4;
    if ((uVar3 != 0x22) && ((param_2 == 0 || (uVar3 != 0x27)))) {
        while (true) {
            if (uVar3 == 0) {
                return puVar4;
            }
            puVar5 = (uint16_t *)(uintptr_t)*param_1;
            if ((8 < *puVar5) && (*puVar5 < 0xe)) break;
            if (*puVar5 == 0x20) {
                return puVar4;
            }
            *param_1 = (int64_t)(uintptr_t)(puVar5 + 1);
            uVar3 = puVar5[1];
        }
        return puVar4;
    }
    puVar5 = puVar4 + 1;
LAB_14001e002:
    puVar4 = puVar4 + 1;
    bVar2 = true;
    *param_1 = (int64_t)(uintptr_t)puVar4;
    uVar1 = *puVar4;
    while (true) {
        if (uVar1 == 0) {
            return puVar5;
        }
        if (((uVar3 == *puVar4) && (bVar2 = (bool)(bVar2 ^ 1), !bVar2)) &&
            (((uVar1 = puVar4[1], 8 < uVar1 && (uVar1 < 0xe)) ||
              ((uVar1 == 0x20 || (puVar4 = (uint16_t *)(uintptr_t)*param_1, uVar1 == 0)))))) break;
        puVar4 = puVar4 + 1;
        *param_1 = (int64_t)(uintptr_t)puVar4;
        uVar1 = *puVar4;
    }
    if (-1 < (char)param_2) {
        *(uint16_t *)(uintptr_t)*param_1 = 0;
    }
    *param_1 = *param_1 + 2;
    if ((param_2 & 0x100) == 0) {
        return puVar5;
    }
    *(uint16_t *)(uintptr_t)*param_1 = 0;
    return puVar5;
}

void FUN_14001e084(int64_t *param_1, int64_t *param_2, int64_t *param_3, LPCWSTR param_4)
{
    /* @0x14001e084 size=278 向串容器追加内容(可带分隔符/新行) */
    int iVar1;
    int iVar2;
    LPCWSTR pWVar3;
    long long lVar4;
    long long lVar5;
    bool bVar6;
    int64_t local_res10;

    lVar4 = 0;
    if (*(LPCWSTR)*param_2 != L'\0') {
        iVar1 = lstrlenW((LPCWSTR)*param_2);
        bVar6 = param_3 != (int64_t *)0x0;
        local_res10 = 0;
        lVar5 = (long long)iVar1;
        if (param_3 == (int64_t *)0x0) {
            param_3 = &local_res10;
            iVar2 = lstrlenW((LPCWSTR)*param_1);
            local_res10 = (long long)iVar2;
        }
        pWVar3 = g_szEmpty;
        if (*param_3 != 0) {
            if (param_4 == (LPCWSTR)0x0) {
                pWVar3 = WSTR("\r\n");
                lVar4 = 2;
            } else {
                iVar2 = lstrlenW(param_4);
                lVar4 = (long long)iVar2;
                pWVar3 = param_4;
            }
        }
        PECMD_AllocString((WCHAR **)param_1, *param_3 + lVar5 + 2 + lVar4);
        if (lVar4 != 0) {
            FUN_14001d78c((uint8_t *)(*param_1 + *param_3 * 2), (uint8_t *)pWVar3, (int)lVar4 * 2);
            *param_3 = *param_3 + lVar4;
        }
        FUN_14001d78c((uint8_t *)(*param_1 + *param_3 * 2), (uint8_t *)*param_2, (iVar1 + 1) * 2);
        *param_3 = *param_3 + lVar5;
        if (bVar6) {
            *param_2 = *param_2 + lVar5 * 2 + 2;
        }
    }
}

uint64_t FUN_14001e19c(LPCWSTR param_1)
{
    /* @0x14001e19c size=164 沿路径逐级创建缺失目录 */
    LPWSTR pWVar2;
    LPCWSTR pWVar3;

    pWVar3 = param_1;
    if ((((*param_1 == L'\\') && (param_1[1] == L'\\')) && (param_1[2] == L'?')) &&
        (param_1[3] == L'\\')) {
        pWVar3 = param_1 + 4;
    }
    pWVar2 = (LPWSTR)pWVar3;
    if ((*pWVar3 != L'\0') && (pWVar3[1] == L':') &&
        ((pWVar2 = pWVar3 + 2), *pWVar2 == L'\\')) {
        pWVar2 = pWVar3 + 3;
    }
    for (;;) {
        pWVar2 = StrChrW(pWVar2, L'\\');
        if (pWVar2 == (LPWSTR)0) break;
        *pWVar2 = L'\0';
        if ((int)PECMD_IsDirectory(param_1) == 0) {
            CreateDirectoryW(param_1, (LPSECURITY_ATTRIBUTES)0);
        }
        *pWVar2 = L'\\';
        pWVar2 = pWVar2 + 1;
    }
    return 1;
}

int64_t FUN_14001e240(LPCWSTR param_1, int64_t *param_2)
{
    /* @0x14001e240 size=137 读文件到缓冲并经编码识别装载 */
    uint64_t uVar3;
    int64_t lVar2;
    DWORD local_res10[6] = {0};
    void *local_28 = 0;
    uint64_t local_20 = 0;
    uint64_t local_18 = 0;
    void *pvVar1;

    local_28 = FUN_1400179f8(param_1, (LPVOID)0, local_res10);
    uVar3 = (uint64_t)local_res10[0];
    local_20 = uVar3;
    local_18 = uVar3;
    lVar2 = PECMD_EncodeBuffer((int64_t *)&local_28, param_2, 0);
    if (lVar2 == 1) {
        pvVar1 = (void *)*param_2;
        param_2[2] = (int64_t)uVar3;
        *param_2 = (int64_t)local_28;
        param_2[1] = (int64_t)uVar3;
        local_28 = pvVar1;
    }
    lVar2 = param_2[1];
    FUN_14005B104((WCHAR **)&local_28);
    return lVar2;
}

void PECMD_CollectPESections(int64_t param_1, uint8_t *param_2, int64_t param_3)
{
    /* @0x14001e3cc size=270 收集 PE 节区并修正 .rsrc 偏移 */
    int *piVar1;
    int64_t lVar2;
    int iVar3;
    int iVar4;
    uint32_t uVar5;
    uint64_t uVar7;
    uint64_t uVar6;

    FUN_14001d744(param_2, (uint8_t *)(uintptr_t)param_3,
                  *(int *)(*(int64_t *)(param_1 + 0x28) + 0x54) +
                  (uint32_t)*(uint16_t *)(*(int64_t *)(param_1 + 0x28) + 6) * 0x28);
    uVar6 = 0;
    uVar7 = uVar6;
    if (*(int16_t *)(*(int64_t *)(param_1 + 0x28) + 6) != 0) {
        do {
            lVar2 = *(int64_t *)(param_1 + 0x30);
            uVar5 = *(uint32_t *)(uVar7 + 0xc + (uint64_t)lVar2);
            if ((uVar5 != 0) && (iVar4 = *(int *)(uVar7 + 0x10 + (uint64_t)lVar2),
                                 iVar4 != 0)) {
                FUN_14001d744(param_2 + uVar5,
                              (uint8_t *)(uintptr_t)((uint64_t)*(uint32_t *)
                                  (uVar7 + 0x14 + (uint64_t)lVar2) + (uint64_t)param_3),
                              iVar4);
                iVar4 = lstrcmpA((LPCSTR)(*(int64_t *)(param_1 + 0x30) +
                                          (int64_t)(int)uVar6 * 0x28), ".rsrc");
                if (iVar4 == 0) {
                    iVar3 = (int)param_3;
                    iVar4 = (int)(intptr_t)(param_2 + uVar5) - iVar3;
                    piVar1 = (int *)(uVar7 + 0xc + *(int64_t *)(param_1 + 0x30));
                    *piVar1 = *piVar1 + iVar3 + iVar4;
                    piVar1 = (int *)(uVar7 + 0x14 + *(int64_t *)(param_1 + 0x30));
                    *piVar1 = *piVar1 + iVar3 + iVar4;
                }
            }
            uVar5 = (int)uVar6 + 1;
            uVar6 = (uint64_t)uVar5;
            uVar7 = uVar7 + 0x28;
        } while ((int)uVar5 <
                 (int)(uint32_t)*(uint16_t *)(*(int64_t *)(param_1 + 0x28) + 6));
    }
    *(uint8_t **)(param_1 + 0x20) = param_2;
    iVar4 = *(int *)(param_2 + 0x3c);
    *(uint8_t **)(param_1 + 0x28) = param_2 + iVar4;
    *(uint8_t **)(param_1 + 0x30) = param_2 + iVar4 + 0x108;
}

int64_t *FUN_14001e4dc(int64_t *param_1, int64_t param_2)
{
    /* @0x14001e4dc size=143 初始化对象并关联父对象(可选) */
    int64_t *plVar1;
    uint8_t uVar2;
    uint32_t uVar3;
    int64_t lVar4;

    lVar4 = 0;
    param_1[7] = param_2;
    param_1[8] = 0;
    *(uint32_t *)(param_1 + 0xc) = 0;
    param_1[0xd] = 0;
    param_1[0xe] = 0;
    FUN_140018b04((int64_t)param_1);
    plVar1 = param_1 + 0xb;
    *param_1 = lVar4;
    uVar3 = (uint32_t)lVar4;
    *(uint32_t *)(param_1 + 1) = uVar3;
    uVar2 = (uint8_t)lVar4;
    *(uint8_t *)((uint8_t *)param_1 + 0xc) = uVar2;
    *plVar1 = lVar4;
    *(uint8_t *)((uint8_t *)param_1 + 0xd) = uVar2;
    param_1[0x1c] = lVar4;
    *(uint32_t *)(param_1 + 0x1d) = uVar3;
    *(uint32_t *)((uint8_t *)param_1 + 0x24) = uVar3;
    param_1[5] = lVar4;
    param_1[6] = lVar4;
    *(uint8_t *)((uint8_t *)param_1 + 0xda) = uVar2;
    if (param_2 != lVar4) {
        plVar1 = *(int64_t **)(param_2 + 0x50);
    }
    param_1[10] = (int64_t)plVar1;
    param_1[0x18] = lVar4;
    if (param_2 != lVar4) {
        *(uint8_t *)((uint8_t *)param_1 + 0xd) = *(uint8_t *)(param_2 + 0xd);
        *(uint8_t *)((uint8_t *)param_1 + 0xda) = *(uint8_t *)(param_2 + 0xda);
    }
    return param_1;
}

uint64_t *PECMD_ReleaseStringContainer(uint64_t *obj, uint32_t flags)
{
    /* @0x14001e578 size=56 释放串容器并设置虚表，按标志释放对象 */
    FUN_14005B104((WCHAR **)(obj + 3));
    PECMD_SetObjectVtable(obj);
    if ((flags & 1) != 0) {
        free(obj);
    }
    return obj;
}

uint64_t FUN_14001e870(int mode)
{
    /* @0x14001e870 size=62 查询物理内存/可用物理内存 */
    struct {
        DWORD dwLength;
        DWORD dwMemoryLoad;
        uint64_t totalPhys;
        uint64_t availPhys;
        uint64_t totalPageFile;
        uint64_t availPageFile;
        uint64_t totalVirtual;
        uint64_t availVirtual;
        uint64_t availExtendedVirtual;
    } ms;

    memset(&ms, 0, sizeof(ms));
    ms.dwLength = 0x40;
    GlobalMemoryStatusEx(&ms);
    return mode != 0 ? ms.availPhys : ms.totalPhys;
}

int64_t PECMD_GetAvailPhysMemoryMB(void)
{
    /* @0x14001e8b0 size=29 可用物理内存（MB 量级） */
    return (int64_t)((FUN_14001e870(1) + 0xfffff) >> 0x14);
}

int64_t PECMD_GetPhysicalMemoryMb(void)
{
    /* @0x14001e8d0 size=26 物理内存总量（MB 量级） */
    return (int64_t)((FUN_14001e870(0) + 0xfffff) >> 0x14);
}

uint32_t *PECMD_InitScriptContext(uint32_t *param_1)
{
    /* @0x14001e8ec size=297 初始化脚本解析上下文结构 */
    FUN_140063620((WCHAR **)(param_1 + 0x1a));
    *(uint64_t *)(param_1 + 0x1c) = 0;
    *(uint64_t *)(param_1 + 0x1e) = 0;
    FUN_140063620((WCHAR **)(param_1 + 0x2e));
    PECMD_AllocSmallObject((uint64_t *)(param_1 + 0x30));
    param_1[0x12] = 0x80000000;
    param_1[0x34] = 0x80000000;
    param_1[0x33] = 0x80000000;
    *(uint64_t *)(param_1 + 0x3e) = 0;
    *(uint64_t *)(param_1 + 0x3c) = 0;
    *param_1 = 0;
    param_1[1] = 0;
    param_1[2] = 0;
    *(uint16_t *)((uint8_t *)param_1 + 0x112) = 0x131;
    param_1[3] = 0;
    param_1[4] = 0;
    param_1[5] = 0;
    param_1[6] = 0;
    param_1[7] = 0;
    param_1[8] = 0;
    param_1[9] = 0;
    *(uint16_t *)(param_1 + 0x32) = 0;
    param_1[0x11] = 0;
    param_1[0x10] = 0;
    param_1[0xf] = 0;
    param_1[0xe] = 0;
    param_1[0xd] = 0;
    param_1[0xc] = 0;
    param_1[0xb] = 0;
    param_1[10] = 0;
    *(uint64_t *)(param_1 + 0x28) = 0;
    *(uint64_t *)(param_1 + 0x2c) = 0;
    *(uint64_t *)(param_1 + 0x2a) = 0;
    *(uint64_t *)(param_1 + 0x3a) = 0;
    *(uint8_t *)((uint8_t *)param_1 + 0x35) = 0;
    param_1[0x13] = 0;
    *(uint8_t *)((uint8_t *)param_1 + 0x25) = 0;
    *(uint64_t *)(param_1 + 0x40) = 0;
    *(uint64_t *)(param_1 + 0x20) = 0;
    *(uint64_t *)(param_1 + 0x22) = 0;
    param_1[0x24] = 0;
    *(uint64_t *)(param_1 + 0x26) = 0;
    *(uint64_t *)(param_1 + 0x36) = 0;
    *(uint64_t *)(param_1 + 0x38) = 0;
    *(uint8_t *)((uint8_t *)param_1 + 0x44) = 0;
    *(uint64_t *)(param_1 + 0x42) = 0x12d;
    return param_1;
}

uint32_t PECMD_ReadFileStr(LPCWSTR param_1, int64_t *param_2)
{
    /* @0x14001ebdc size=383 读取脚本文件内容到容器, 转换编码 */
    HANDLE hFile;
    uint32_t uVar1;
    DWORD nNumberOfBytesToRead;
    DWORD local_res20[2];
    int64_t local_30;
    uint64_t uVar2;

    hFile = (HANDLE)0;
    PECMD_OpenFileHandle(&hFile, param_1, 0x80000000, 3, 0, 3, 0, 0);
    if (hFile == (HANDLE)0) {
        uVar1 = 0xfffffffe;
    } else {
        nNumberOfBytesToRead = FUN_1400e693c(hFile);
        PECMD_SetFilePointer(hFile, (LARGE_INTEGER){0}, 0);
        PECMD_GrowByteBuffer((void **)param_2, (int64_t)nNumberOfBytesToRead + 0xc);
        local_res20[0] = 0;
        ReadFile(hFile, (LPVOID)*param_2, nNumberOfBytesToRead, local_res20, 0);
        memset((uint8_t *)((uint64_t)local_res20[0] + (uint64_t)*param_2), 0, 6);
        param_2[2] = (int64_t)local_res20[0];
        param_2[1] = (int64_t)local_res20[0];
        uVar2 = FUN_14005f33c(*param_2, (int)local_res20[0]);
        if ((char)uVar2 == '\x01') {
            local_30 = *param_2;
            param_2[1] = 0;
            param_2[2] = 0;
            *param_2 = 0;
            PECMD_EncodeBuffer(&local_30, param_2, 0);
            PECMD_GrowByteBuffer((void **)param_2, param_2[1] + 0xc);
            FUN_14005B104((WCHAR **)&local_30);
        }
        FUN_1400e7d58(param_2, 1);
        if ((char)uVar2 < 0) {
            if (hFile != (HANDLE)0xffffffffffffffff) {
                CloseHandle(hFile);
            }
            uVar1 = 0xffffffff;
        } else {
            uVar1 = (uint32_t)param_2[1];
            if (hFile != (HANDLE)0xffffffffffffffff) {
                CloseHandle(hFile);
            }
        }
    }
    return uVar1;
}

uint8_t FUN_14001ed5c(int64_t param_1, LPCWSTR param_2)
{
    /* @0x14001ed5c size=365 (签名修正自联调, 主体仍为 NO-OP stub) */
    (void)param_1; (void)param_2;
    return 0;
}

uint64_t PECMD_ExecLoadCommand(LPCWSTR param_1, LPCWSTR param_2)
{
    /* @0x14001eecc size=441 由命令行/文件执行 PECMD LOAD */
    WCHAR WVar1;
    uint64_t uVar2;
    LPCWSTR pWVar3;
    uint16_t local_res8[8] = {0};
    WCHAR *local_res18 = NULL;   /* 命令串容器 */
    WCHAR *local_res20 = NULL;   /* 参数串容器 */
    uint64_t local_238[2];       /* 串容器 */
    WCHAR local_228[264];

    FUN_140063620((WCHAR **)local_238);
    uVar2 = 0;
    local_228[0] = L'\0';
    GetModuleFileNameW((HMODULE)0, local_228, 0x104);
    FUN_140063620(&local_res20);
    if (param_1 == (LPCWSTR)0) {
        if (param_2 != (LPCWSTR)0) {
            uVar2 = FUN_1400091e0(param_2, (int64_t *)0, (int64_t *)0,
                                  (HANDLE *)0, g_szEmpty);
            uVar2 = uVar2 & 0xffffffff;
            goto LAB_14001f038;
        }
    } else {
        WVar1 = *param_1;
        pWVar3 = param_1;
        if (WVar1 == L'\0') {
LAB_14001ef4a:
            if (*pWVar3 != L'\0') {
                FUN_14006375C(&local_res20, WSTR("\""));
            }
        } else {
            do {
                if (((8 < (uint16_t)WVar1) && ((uint16_t)WVar1 < 0xe)) ||
                    (WVar1 == L' ')) {
                    goto LAB_14001ef4a;
                }
                pWVar3 = pWVar3 + 1;
                WVar1 = *pWVar3;
            } while (WVar1 != L'\0');
        }
        FUN_14006375C(&local_res20, param_1);
        if (*pWVar3 != L'\0') {
            FUN_14006375C(&local_res20, WSTR("\""));
        }
    }
    FUN_1400702B0(&local_res18, WSTR("--exe:\""));
    FUN_14006375C(&local_res18, local_228);
    FUN_14006375C(&local_res18, WSTR("\" PECMD LOAD "));
    FUN_14006375C(&local_res18, g_szEmpty);
    FUN_14006375C(&local_res18, local_res20);
    local_res8[0] = 0;
    FUN_14000e26c((uint64_t)(uintptr_t)g_Script, (uint64_t)(uintptr_t)local_res18,
                  (uint64_t)(uintptr_t)g_Script, (uint64_t)(uintptr_t)local_res8,
                  0, (void *)0, 0, (void *)0);
    FUN_14005B104(&local_res18);
LAB_14001f038:
    FUN_14005B104(&local_res20);
    FUN_14005B104((WCHAR **)local_238);
    return uVar2;
}

void PECMD_HandleDropFile(int64_t param_1, void *param_2, char param_3)
{
    /* @0x14001f088 size=106 处理拖放: 取路径加载脚本并(可选)执行 */
    LPCWSTR local_res20 = NULL;

    FUN_140063620((WCHAR **)&local_res20);
    FUN_140006554(param_2, (int64_t *)&local_res20, (int64_t *)0);
    FUN_14001ed5c(param_1, (LPCWSTR)local_res20);
    if (param_3 != '\0') {
        PECMD_ExecLoadCommand((LPCWSTR)local_res20, (LPCWSTR)0);
    }
    FUN_14005B104((WCHAR **)&local_res20);
}

void FUN_14001f0f4(LPCWSTR param_1, int64_t *param_2)
{
    /* @0x14001f0f4 size=111 加载脚本文件并跳过前导 BOM/注释行 */
    WCHAR WVar1;
    int iVar2;
    LPCWSTR lpString;

    iVar2 = PECMD_ReadFileStr(param_1, param_2);
    if (0 < iVar2) {
        lpString = (LPCWSTR)*param_2;
        if ((char)lpString == -1 && (char)lpString[1] == -2) {
            lpString = lpString + 2;
        }
        for (WVar1 = *lpString; WVar1 == L'#'; ) {
            while ((WVar1 != L'\0' && (WVar1 != L'\r')) && (WVar1 != L'\n')) {
                lpString = lpString + 1;
                WVar1 = *lpString;
            }
            for (; (*lpString == L'\n' || (*lpString == L'\r')); lpString = lpString + 1) {
            }
            WVar1 = *lpString;
        }
        lstrlenW(lpString);
    }
}

uint64_t FUN_14001f164(short *param_1, LPCWSTR param_2)
{
    /* @0x14001f164 size=109 不区分大小写的限定长度串比较, 返回匹配字符数 */
    int iVar1 = lstrlenW(param_2);
    uint64_t uVar5 = 0;

    if (0 < (int64_t)iVar1) {
        long long lVar4 = (long long)param_2 - (long long)param_1;
        uint64_t uVar6 = 0;
        short sVar2;
        short sVar3;
        do {
            sVar3 = *param_1;
            sVar2 = *(short *)(lVar4 + (long long)param_1);
            if ((uint16_t)(sVar3 - 0x41U) < 0x1a) {
                sVar3 = (short)(sVar3 + 0x20);
            }
            if ((uint16_t)(sVar2 - 0x41U) < 0x1a) {
                sVar2 = (short)(sVar2 + 0x20);
            }
            if (sVar3 != sVar2) {
                return uVar5;
            }
            uVar6 = uVar6 + 1;
            uVar5 = (uint64_t)((int)uVar5 + 1);
            param_1 = param_1 + 1;
        } while ((long long)uVar6 < (long long)iVar1);
    }
    return uVar5;
}

HICON PECMD_LoadIcon(LPCWSTR param_1, uint64_t *param_2)
{
    /* @0x14001f1d4 size=2809 GDI+/图标加载 */
    WCHAR WVar1;
    int iVar2;
    int iVar3;
    BOOL BVar4;
    uint32_t uVar5;
    int64_t lVar6;
    LPWSTR pWVar7;
    LPWSTR pWVar8;
    LPWSTR lpString;
    LPCWSTR lpLibFileName;
    HMODULE pHVar9 = (HMODULE)0;
    HBITMAP ho;
    HICON pHVar10;
    uint64_t uVar11;
    HINSTANCE pHVar12 = (HINSTANCE)0;
    HMODULE pHVar13;
    uint32_t local_res18[2];
    int local_res20;
    HICON local_308;
    LPWSTR local_2f8 = (LPWSTR)0;
    LPWSTR local_300 = (LPWSTR)0;
    WCHAR *local_2f0;
    HANDLE local_2e8;
    void *local_2e0 = 0;
    uint64_t local_2d8;
    uint64_t local_2d0;
    uint64_t local_2c8;
    uint64_t local_2c0;
    WCHAR local_2b8[64];
    WIN32_FIND_DATAW local_288;

    if ((uint64_t)(uintptr_t)g_pGdiplusStartup == 0xffffffffffffffffULL) {
        return (HICON)0;
    }
    if ((param_2 == (uint64_t *)0) || ((*param_2 & 0x10) == 0)) {
        local_res20 = 0;
        local_2c8 = 0;
        local_2c0 = 0;
    } else {
        local_2c8 = param_2[1];
        local_res20 = 1;
        local_2c0 = param_2[2];
    }
    if (g_pGdiplusStartup == (void *)0) {
        if ((g_hGdiPlus == (HMODULE)0) &&
            (g_hGdiPlus = LoadLibraryW(WSTR("GdiPlus.dll")),
             g_hGdiPlus == (HMODULE)0)) {
            /* GdiPlus.dll 常规加载失败 -> 在 WinSxS 下搜索 */
            local_2e8 = (HANDLE)0;
            local_300 = (LPWSTR)0;
            memcpy(local_2b8, L"%SystemRoot%\\WinSxS\\*", 0x2c);
            FUN_14007BF44((int64_t *)&g_Script, local_2b8, (void *)&local_300, 0, 1);
            FUN_140101db8(&local_2e8, (LPCWSTR)local_300, &local_288);
            if (local_2e8 != (HANDLE)0) {
                pWVar7 = StrRChrW((LPCWSTR)local_300, (LPCWSTR)0, L'\\');
                if (pWVar7 != (LPWSTR)0) {
                    *pWVar7 = L'\0';
                }
                do {
                    if ((local_288.cFileName[0] != L'.') &&
                        ((local_288.dwFileAttributes & 0x10) != 0)) {
                        iVar2 = lstrlenW((LPCWSTR)local_300);
                        iVar3 = lstrlenW(local_288.cFileName);
                        FUN_140063694(&local_2f0, (int64_t)iVar3 + 0x1e + (int64_t)iVar2);
                        /* 注: FUN_14001708c 为 SKIP(CRT) 的格式包装,
                           此处路径串不会真正生成(已接受限制) */
                        FUN_14001708c(local_2f0, (size_t)0x140120040, (void *)local_300,
                                      (void *)local_288.cFileName);
                        g_hGdiPlus = LoadLibraryW(local_2f0);
                        FUN_14005B104((WCHAR **)&local_2f0);
                    }
                } while ((g_hGdiPlus == (HMODULE)0) &&
                         (BVar4 = FindNextFileW(local_2e8, &local_288), BVar4 != 0));
            }
            FUN_14005B104((WCHAR **)&local_300);
            if ((local_2e8 != (HANDLE)0) &&
                (local_2e8 != (HANDLE)(intptr_t)-1)) {
                FindClose(local_2e8);
            }
            if (g_hGdiPlus == (HMODULE)0) {
                g_pGdiplusStartup = (void *)(uintptr_t)0xffffffffffffffffULL;
                return (HICON)0;
            }
        }
        g_pGdiplusShutdown = GetProcAddress(g_hGdiPlus, "GdiplusShutdown");
        g_pGdipLoadImageFromFile = GetProcAddress(g_hGdiPlus, "GdipLoadImageFromFile");
        g_pGdipDisposeImage = (int (*)())GetProcAddress(g_hGdiPlus, "GdipDisposeImage");
        g_pGdipCreateFromHDC = (int (*)())GetProcAddress(g_hGdiPlus, "GdipCreateFromHDC");
        DAT_14013cd98 = GetProcAddress(g_hGdiPlus, "GdipDrawImageRectI");
        DAT_14013cde8 = GetProcAddress(g_hGdiPlus, "GdipCreateBitmapFromHBITMAP");
        DAT_14013cdf0 = GetProcAddress(g_hGdiPlus, "GdipCreateHBITMAPFromBitmap");
        g_pGdipSaveImageToFile = GetProcAddress(g_hGdiPlus, "GdipSaveImageToFile");
        DAT_14013cdb8 = GetProcAddress(g_hGdiPlus, "GdipGetImageEncodersSize");
        DAT_14013cdc0 = GetProcAddress(g_hGdiPlus, "GdipGetImageEncoders");
        g_pGdipDrawImageI = GetProcAddress(g_hGdiPlus, "GdipDrawImageI");
        g_pGdipGetImageWidth = (int (*)())GetProcAddress(g_hGdiPlus, "GdipGetImageWidth");
        g_pGdipGetImageHeight = (int (*)())GetProcAddress(g_hGdiPlus, "GdipGetImageHeight");
        g_pGdipDrawImageRectRectI = GetProcAddress(g_hGdiPlus, "GdipDrawImageRectRectI");
        g_pGdipDeleteGraphics = (int (*)())GetProcAddress(g_hGdiPlus, "GdipDeleteGraphics");
        DAT_14013ce38 = GetProcAddress(g_hGdiPlus, "GdipSetInterpolationMode");
        g_pGdipSetPixelOffsetMode = GetProcAddress(g_hGdiPlus, "GdipSetPixelOffsetMode");
        g_pGdipSetSmoothingMode = GetProcAddress(g_hGdiPlus, "GdipSetSmoothingMode");
        g_pGdipLoadImageFromStream = GetProcAddress(g_hGdiPlus, "GdipLoadImageFromStream");
        g_pGdipGetImageType = GetProcAddress(g_hGdiPlus, "GdipGetImageType");
        DAT_14013ce70 = GetProcAddress(g_hGdiPlus, "GdipCreateBitmapFromHICON");
        g_pGdipGetImageDimension = GetProcAddress(g_hGdiPlus, "GdipGetImageDimension");
        DAT_14013ce60 = GetProcAddress(g_hGdiPlus, "GdipCreateHICONFromBitmap");
        g_pGdipSetTextRenderingHint = GetProcAddress(g_hGdiPlus, "GdipSetTextRenderingHint");
        g_pGdipImageGetFrameDimensionsList = GetProcAddress(g_hGdiPlus, "GdipImageGetFrameDimensionsList");
        g_pGdipImageSelectActiveFrame = GetProcAddress(g_hGdiPlus, "GdipImageSelectActiveFrame");
        g_pGdipImageGetFrameDimensionsCount = GetProcAddress(g_hGdiPlus, "GdipImageGetFrameDimensionsCount");
        g_pGdipImageGetFrameCount = GetProcAddress(g_hGdiPlus, "GdipImageGetFrameCount");
        g_pGdipGetPropertyItemSize = GetProcAddress(g_hGdiPlus, "GdipGetPropertyItemSize");
        g_pGdipGetPropertyItem = GetProcAddress(g_hGdiPlus, "GdipGetPropertyItem");
        g_pGdipBitmapLockBits = GetProcAddress(g_hGdiPlus, "GdipBitmapLockBits");
        g_pGdipBitmapUnlockBits = GetProcAddress(g_hGdiPlus, "GdipBitmapUnlockBits");
        DAT_14013ce90 = GetProcAddress(g_hGdiPlus, "GdipCreateBitmapFromScan0");
        g_pGdipCreateBitmapFromResource = GetProcAddress(g_hGdiPlus, "GdipCreateBitmapFromResource");
        DAT_14013cea0 = GetProcAddress(g_hGdiPlus, "GdipCloneBitmapAreaI");
        DAT_14013cea8 = GetProcAddress(g_hGdiPlus, "GdipGetImageGraphicsContext");
        g_pGdipScaleWorldTransform = GetProcAddress(g_hGdiPlus, "GdipScaleWorldTransform");
        g_pGdipCreateTexture = GetProcAddress(g_hGdiPlus, "GdipCreateTexture");
        g_pGdipDeleteBrush = GetProcAddress(g_hGdiPlus, "GdipDeleteBrush");
        g_pGdipFillRectangle = GetProcAddress(g_hGdiPlus, "GdipFillRectangle");
        g_pGdipDrawString = GetProcAddress(g_hGdiPlus, "GdipDrawString");
        DAT_14013ced0 = GetProcAddress(g_hGdiPlus, "GdipNewPrivateFontCollection");
        DAT_14013ced8 = GetProcAddress(g_hGdiPlus, "GdipDeletePrivateFontCollection");
        DAT_14013cee0 = GetProcAddress(g_hGdiPlus, "GdipPrivateAddMemoryFont");
        DAT_14013cee8 = GetProcAddress(g_hGdiPlus, "GdipGetFontCollectionFamilyList");
        DAT_14013cef0 = GetProcAddress(g_hGdiPlus, "GdipGetFamilyName");
        g_pGdiplusStartup = GetProcAddress(g_hGdiPlus, "GdiplusStartup");
    }
    if ((uint64_t)(intptr_t)param_1 - 1U == 0xfffffffffffffffdULL) {
        return (HICON)g_pGdiplusStartup;
    }
    if ((g_gdiplusToken == (void *)0) && (g_pGdipLoadImageFromFile != (void *)0)) {
        memset(local_2b8, 0, sizeof(local_2b8));
        local_2b8[0] = L'\x01';
        ((int (*)(void *, void *, int))g_pGdiplusStartup)(
            (void *)&g_gdiplusToken, (void *)local_2b8, 0);
    }
    if (0xfffffffffffffffdULL < (uint64_t)(intptr_t)param_1 - 1U) {
        return (HICON)g_gdiplusToken;
    }

    local_308 = (HICON)0;
    local_res18[0] = 0;
    local_2d8 = 0;
    local_2d0 = 0;
    pWVar8 = StrChrW(param_1, L'|');
    pWVar7 = pWVar8;
    if (pWVar8 != (LPWSTR)0) {
        *pWVar8 = L'\0';
        pWVar7 = pWVar8 + 1;
    }
    lpString = StrRChrW(param_1, (LPCWSTR)0, L'#');
    pHVar13 = pHVar12;
    if ((*param_1 == L'#') || (pWVar7 != (LPWSTR)0) ||
        (g_pGdipLoadImageFromFile == (void *)0)) {
        if (*param_1 == L'#') {
            WVar1 = param_1[1];
            lpString = (LPWSTR)param_1;
            pHVar9 = g_hInst;
            if ((WVar1 != L'\0') &&
                (lpString = (LPWSTR)param_1,
                 9 < (uint16_t)(WVar1 + 0xfff0))) {
                lpString = (LPWSTR)param_1 + 1;
            }
        } else {
            if (lpString == (LPWSTR)0) {
                if (g_pGdipLoadImageFromFile == (void *)0) goto LAB_14001fc88;
                goto LAB_14001fb42;
            }
            WVar1 = *lpString;
            *lpString = L'\0';
            lpLibFileName = FUN_14001BE14((WCHAR *)param_1);
            pHVar9 = LoadLibraryExW(lpLibFileName, (HANDLE)0, 2);
            *lpString = WVar1;
            if (pWVar7 == (LPWSTR)0) {
                iVar2 = lstrlenW(lpString);
                lVar6 = (int64_t)iVar2;
            } else {
                lVar6 = (int64_t)(pWVar8 - lpString) >> 1;
            }
            PECMD_StrCopyW((WCHAR **)&local_2f8, lpString, lVar6);
            lpString = local_2f8;
            pHVar13 = pHVar9;
        }
        if (pWVar7 != (LPWSTR)0) {
            pWVar7 = (*pWVar7 != L'\0') ? pWVar7 : (LPWSTR)0;
        }
        if (pWVar7 != (LPWSTR)0) {
            uVar5 = PECMD_IsBitmapResource((uint16_t *)pWVar7);
            if ((char)uVar5 == '\0') {
                uVar5 = PECMD_IsIconResource((uint16_t *)pWVar7);
                if ((char)uVar5 == '\0') {
                    uVar5 = FUN_14005D7E8((uint16_t *)pWVar7);
                    if ((char)uVar5 == '\0') goto LAB_14001fbe7;
                    pWVar7 = (LPWSTR)0xe;
                    local_res18[0] = 2;
                } else {
                    pWVar7 = (LPWSTR)0x3;
                    local_res18[0] = 0x14;
                }
                pHVar10 = FUN_140073934(pHVar9, lpString, (int)local_2c8,
                                        (int)local_2c0, 0, local_res18);
                pHVar12 = pHVar9;
                if ((local_res18[0] >> 8 & 1) == 0) goto LAB_14001fba6;
                if (pHVar10 != (HICON)0) {
                    *param_2 = *param_2 | 4;
                    local_308 = pHVar10;
                    goto LAB_14001fbd5;
                }
                goto LAB_14001fbe2;
            }
            pWVar7 = (LPWSTR)0x2;
            ho = LoadBitmapW(pHVar9, lpString);
            if (ho != (HBITMAP)0) {
                ((void (*)(HBITMAP, int, HICON *))DAT_14013cde8)(ho, 0, &local_308);
                DeleteObject(ho);
            }
        }
    } else {
LAB_14001fb42:
        uVar11 = FUN_1400688E0(param_1);
        if ((char)uVar11 != '\0') {
            FUN_14001e240(param_1, (int64_t *)&local_2e0);
            local_308 = (HICON)FUN_140061E98(local_2e0, local_2d8, (void *)0);
            goto LAB_14001fc88;
        }
        pHVar10 = LoadImageW((HINSTANCE)0, param_1, 1, 0, 0, 0x10);
        if (pHVar10 == (HICON)0) {
            ((void (*)(LPCWSTR))g_pGdipLoadImageFromFile)(param_1);
            if ((local_308 != (HICON)0) && (param_2 != (uint64_t *)0) &&
                (pWVar7 = StrRChrW(param_1, (LPCWSTR)0, L'.'), pWVar7 != (LPWSTR)0) &&
                (iVar2 = lstrcmpiW(pWVar7 + 1, WSTR("ico")), iVar2 == 0)) {
                *param_2 = *param_2 | 4;
            }
            goto LAB_14001fc88;
        }
LAB_14001fba6:
        pHVar9 = pHVar12;
        if (local_res20 == 0) {
            ((void (*)(HICON, HICON *))DAT_14013ce70)(pHVar10, &local_308);
            DestroyIcon(pHVar10);
        } else {
            *param_2 = *param_2 | 2;
            local_308 = pHVar10;
        }
LAB_14001fbd5:
        if (local_308 == (HICON)0) {
LAB_14001fbe2:
            local_308 = (HICON)0;
        }
    }
LAB_14001fbe7:
    if ((pHVar9 != (HMODULE)0) && (local_308 == (HICON)0)) {
        if (pWVar7 == (LPWSTR)0) {
            pWVar7 = (LPWSTR)0x2;
        }
        FUN_14001EA18(pHVar9, lpString, (LPCWSTR)pWVar7, (int64_t *)&local_2e0,
                      (uint32_t *)0);
        local_308 = (HICON)FUN_140061E98(local_2e0, local_2d8, (void *)0);
        if (local_308 != (HICON)0) goto LAB_14001fc88;
    }
    if (pHVar13 != (HMODULE)0) {
        FreeLibrary(pHVar13);
    }
LAB_14001fc88:
    pHVar10 = local_308;
    if (pWVar8 != (LPWSTR)0) {
        *pWVar8 = L'|';
    }
    FUN_14005B104((WCHAR **)&local_2e0);
    FUN_14005B104((WCHAR **)&local_2f8);
    return pHVar10;
}

uint16_t *FUN_14001fcd0(uint16_t *param_1, uint16_t param_2)
{
    /* @0x14001fcd0 size=144 按无符号/等于匹配关键字 token */
    uint16_t uVar1;
    uint16_t uVar2;
    uint16_t *local_res8;

    local_res8 = param_1;
    while (1) {
        uVar1 = *local_res8;
        if (uVar1 == 0) {
            return (uint16_t *)0;
        }
        uVar2 = param_2;
        if (uVar1 == uVar2) break;
        if ((uVar2 == (uVar1 & 0xffdf)) && (0x40 < uVar2) && (uVar2 < 0x5b)) {
            return local_res8;
        }
        while ((uVar1 != 0 && ((uVar1 < 9) || (0xd < uVar1))) && (uVar1 != 0x20)) {
            local_res8 = local_res8 + 1;
            uVar1 = *local_res8;
        }
        PECMD_SkipLeadingControls((uint64_t *)&local_res8);
    }
    return local_res8;
}

