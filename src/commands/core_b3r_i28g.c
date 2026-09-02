/* Restored batch28 B3 business function (part i28g)
 * Function: PECMD_ExecCommandLine (PART 命令引擎)
 *
 * R26-i: dc FUN_14008cffc @0x14008cffc (reference/decompiled.c:89782-93623,
 * 3845 行 / 29895B, 项目最大单体) 全文直移。替换 core_b3_remaining.c:22664 的
 * decompile-failed 简化桩。
 *
 * 结构分段:
 *   S1  声明/初始化 + 选项循环 (-admin..-floppy 46 分支) + 哨兵归位 + 子命令编码
 *       (dc:90018-90713)
 *   S2  子命令词切分/盘号解析/MBR 保护性交换与引导模板写入 (dc:90714-91890)
 *   S3  list/part/drv/volume/dep/init/fix/update 分派 + GPT 写引擎 + epilogue
 *       (dc:91891-93623)
 *
 * 直移要点 (偏差登记见 HANDOVER R26-i):
 *   - pthreadmbcinfo 伪字段按字节偏移语义翻译 (capstone 实证):
 *       p->refcount      = p[0]      (byte+0)
 *       p->mbcodepage    = p[2]      (byte+4)
 *       p->ismbcodepage  = p[4]      (byte+8)
 *       p->mbulinfo      = p+6       (byte+12, ushort* 粒度)
 *     选项循环各分支匹配后的游标推进 = token 字符数 (asm: add r11,N*2)。
 *   - dc 哨兵指针 (0xfffffffffffffff7/-9, ...fffe/-2, ...ffff/-1) 落码为
 *     int64_t 数值槽, 游标使用处显式 (WCHAR *)(uintptr_t)cast。
 *   - Ghidra 寄存器残影 (local_310 = (uint)ppWVar43 两处) 无对应 store,
 *     不落码 (asm 0x14008d1c1-0x14008d2ec 区间核验)。
 *   - 引导扇区模板与 GPT 常量取自原版 .data: g_bootDos@14013a370(400B)/
 *     g_bootNt5@14013a500(384B)/g_bootNt6@14013a680(440B)/
 *     g_gptDiskGuidDef@140127a50(12B); GPT 未用条目模板 @140124100 = 16×0。
 *   - PART 无对拍语料 (磁盘副作用), 回归仅保 64/64 零回归。
 */
#include <stdbool.h>
#include <stddef.h>
#include <string.h>

#include "win32_stub.h"
#include "pecmd_defs.h"

/* FUN_140102a90 = CRT memset 别名 (同 core_b3r_h4.c/core_main.c 惯例) */
#define FUN_140102A90(dst, val, n) memset((dst), (val), (n))

/* ---- Ghidra primitive aliases (project convention) ---- */
typedef unsigned int uint;
typedef unsigned short ushort;
typedef uint8_t byte;
typedef uint64_t undefined8;
typedef void undefined;
typedef int64_t longlong;
typedef uint64_t ulonglong;

/* LARGE_INTEGER 标量构造 (dc LI64(expr) 直移) */
static LARGE_INTEGER LI64(int64_t v)
{
    LARGE_INTEGER r;
    r.QuadPart = v;
    return r;
}

/* Ghidra 符号借位 (溢出旗标) helper — dc 90974/93002 直移 */
static __inline int SBORROW4(int32_t op1, int32_t op2)
{
    int32_t r = (int32_t)((uint32_t)op1 - (uint32_t)op2);
    return ((op1 ^ op2) & (op1 ^ r)) < 0;
}

static __inline int SBORROW2(int16_t op1, int16_t op2)
{
    int16_t r = (int16_t)((uint16_t)op1 - (uint16_t)op2);
    return ((op1 ^ op2) & (op1 ^ r)) < 0;
}

/* ---- Globals ---- */
extern WCHAR g_szEmpty[];              /* DAT_14011c638 空串 */
extern uint8_t g_flagD6F5;             /* DAT_14013d6f5 卷锁旗标 */
extern CRITICAL_SECTION g_csDisk;      /* DAT_14013e0e8 磁盘临界区 */

/* SH* 注册表函数指针 (DAT_14013d3f0/14013d3f8, core_globals.c:93) */
extern int (*g_pSHSetValueW)(HKEY, LPCWSTR, LPCWSTR, DWORD, const void *, DWORD);
extern int (*g_pSHDeleteValueW)(HKEY, LPCWSTR, LPCWSTR);

/* ---- 依赖 helper externs (真体分布见各 @0x 注) ---- */
extern int64_t *PECMD_SkipLeadingControlChars(long long *pp);              /* @0x14005b154 */
extern void PECMD_AllocWStringBuffer(WCHAR **ps, int64_t count);           /* @0x140063694 */
extern WCHAR *PECMD_AllocStrSlot(WCHAR **out);                             /* @0x140063620 */
extern void FUN_140063888(void *ps, const WCHAR *src, int64_t len);        /* @0x140063888 定长拷贝 */
extern WCHAR *PECMD_AllocString(WCHAR **ps, int64_t count);                /* @0x140063720 保容量扩容 */
extern char PECMD_MatchTokenAdvance(const char *tok, void *pp, int n);     /* @0x1400660ac 匹配并推进 */
extern int FUN_14005C788(const char *s, const WCHAR *w, int n);            /* @0x14005c788 前缀比较不推进 */
extern int32_t PECMD_AsciiWideICmp(const char *a, const WCHAR *w);         /* @0x14005c7c4 整词 icmp */
extern bool PECMD_MatchAndAdvance(char *key, int64_t *pp, int len);        /* @0x1400661e4 */
extern void PECMD_SplitTokenTrimWs(WCHAR **src, WCHAR **dst, int16_t delim); /* @0x1400675b8 */
extern void FUN_1400702B0(WCHAR **ps, LPCWSTR src);                        /* @0x1400702b0 赋值不释放旧值 */
extern WCHAR *FUN_14006375C(WCHAR **ps, LPCWSTR src);                      /* @0x14006375c 追加 */
extern void FUN_140024C48(WCHAR **pp, size_t *plen, uint32_t flags);       /* @0x140024c48 token 分割 */
extern uint64_t PECMD_EvalExprSkipOneChar(int64_t *pp, uint64_t *out);     /* @0x1400a9a84 */
extern uint64_t PECMD_ParseSizeAndSkipWs(int64_t *pp, uint64_t *out);      /* @0x14006a7f4 */
extern bool PECMD_ParseUIntValue(WCHAR **pp, int *out);                    /* @0x140074838 */
extern uint64_t PECMD_ParseSizeValue(int64_t *pp, uint64_t *out);          /* @0x14006a8c4 */
extern bool PECMD_ParseNumSkipWs(WCHAR **pp, uint64_t *out);           /* @0x140067cf4 */
extern bool PECMD_ParseHexOrDecBool(long long *pp, int *out);              /* @0x1400c11c0 */
extern void PECMD_FreeStrBuf(void *ps);                                    /* @0x14005b104 */
extern void FUN_1400629B8(void *script, LPCWSTR key, LPCWSTR value);       /* @0x1400629b8 内置变量赋值 */
extern void PECMD_ReadTokenString(uint64_t ctx, int64_t *pp, uint32_t flags); /* @0x140069b68 */
extern long long FUN_140031454(long long *script, char *cmdline);      /* @0x140031454 RunCommand */
extern void PECMD_TlsLogWrite(uint64_t ctx, LPCWSTR fmt, ...);             /* @0x140018d8c 变参日志 */

/* 设备/磁盘 */
extern void PECMD_OpenFileHandle(HANDLE *out, LPCWSTR path, DWORD access, DWORD share,
                                 LPSECURITY_ATTRIBUTES sa, DWORD disp, DWORD flags,
                                 HANDLE tmpl);                             /* @0x140003864 */
extern int FUN_14005F96C(HANDLE h, int defSize);                           /* @0x14005f96c 扇区大小 */
extern int64_t FUN_14005FBD4(HANDLE h, int64_t *geo);                      /* @0x14005fbd4 设备大小 */
extern uint64_t *FUN_14005FEAC(HANDLE h, uint64_t *buf, uint32_t *out);    /* @0x14005feac 布局查询 */
extern uint64_t *PECMD_UpdatePartitionLayout(HANDLE h, void *buf, uint32_t *a3, byte *a4,
                                             uint32_t flags);              /* @0x14006abb8 */
extern void PECMD_CloseDeviceHandle(int64_t *h, int dev, byte flags, byte *res20, uint64_t *err,
                                    LPCWSTR name);                         /* @0x140078408 */
extern int PECMD_QueryDiskGeometry(HANDLE h, uint64_t *buf, int a, int b); /* @0x140069a20 */
extern uint64_t PECMD_ReadDiskSectorScan(uint64_t *p1, uint64_t *p2, uint32_t a, uint32_t b,
                                         uint64_t c, uint64_t *d, LARGE_INTEGER e); /* @0x140069868 */
extern void PECMD_WriteFloppyImage(HANDLE h, int64_t a2, int64_t a3, DWORD a4); /* @0x1400570ec */
extern int64_t PECMD_LockVolumeByDevice(int dev, uint mode);               /* @0x140078334 */
extern HANDLE PECMD_OpenLockVolume(int dev, uint64_t a2, int64_t a3, uint32_t flag, WCHAR *name); /* @0x140007670 = PECMD_OpenLockVolume (core_b1_remaining.c) */
extern int64_t PECMD_ParseVolumeGuid(int64_t *pp, uint32_t *out, int flag); /* @0x1400780fc */
extern DWORD PECMD_EnumDevices(LPCWSTR filter, LPWSTR out, uint32_t flags, void *guid); /* @0x140076554 */
extern int64_t PECMD_GetStorageDependency(uint64_t ctx, int64_t *out, WCHAR *path, int mode); /* @0x140077c1c */
extern int64_t PECMD_DescribePartitionInfo(int64_t *out, LPCWSTR dev, uint64_t a3, uint64_t a4,
                                           LARGE_INTEGER *p5, uint32_t *p6, char flag); /* @0x14008bcd4 */
extern uint32_t PECMD_EnumeratePhysicalDrives(longlong *out, uint flags);  /* @0x14008ac48 */
extern WCHAR *PECMD_EnumerateVolume(int64_t *out, int64_t dev, uint64_t flags, LPCWSTR name); /* @0x140006aa0 */
extern uint64_t PECMD_WriteMbrPartitionEntry(HANDLE h, int64_t sector, uint8_t *data, int mode); /* @0x140077fb8 */
extern void PECMD_FlushFileThrice(HANDLE h);                               /* @0x1400600f8 */
extern void PECMD_FlushDeviceIoctl(HANDLE h);                              /* @0x140060120 */
extern void PECMD_ClearFlagAndError(uint8_t *flags);                       /* @0x14006020c */
extern void PECMD_ErrorHandlerWrap(uint8_t *flags, int mode, uint64_t a, int64_t b, uint32_t c,
                                   WCHAR *d);                              /* @0x1400601b8 */
extern void PECMD_FixRandomSeed(int64_t obj);                              /* @0x140060180 */
extern void PECMD_PackRowCol(uint64_t pos, uint8_t *hi, char *row, uint8_t *col, uint32_t width,
                             uint32_t height, int clip);                   /* @0x140060088 CHS 打包 */
extern int64_t PECMD_AlignRemainder(int64_t value, int64_t align);         /* @0x14006006c */
extern void PECMD_MemMoveForward(uint8_t *a, uint8_t *b, int n);           /* @0x14001d78c 前向 memmove */
extern int PECMD_AnsiStrNCompare(char *buf, int64_t a, int64_t b);         /* @0x14005b184 字节区比较 */
extern LARGE_INTEGER PECMD_SetFilePointer(HANDLE h, LARGE_INTEGER off, DWORD method); /* @0x14005c674 */
extern uint32_t PECMD_Crc32HexOfBytes(const uint8_t *data, int64_t len, char *out); /* @0x1400e4cc0 */
extern void PECMD_FormatI64Dec(LPWSTR dst, int64_t value);                 /* @0x1400e6d68 */
extern WCHAR *PECMD_SprintfRetEnd(WCHAR *dst, uint64_t v, LPCWSTR fmt);    /* @0x1400e6d38 */
extern LPVOID PECMD_ReadFileToBuffer(LPCWSTR path, LPVOID buf, DWORD *size); /* @0x1400179f8 */
extern uint64_t FUN_14001EA18(HMODULE mod, LPCWSTR name, LPCWSTR type, uint64_t *sizeOut,
                              uint *p5);                                   /* @0x14001ea18 资源加载 */
extern DWORD PECMD_WaitHandlesOrMessages(uint64_t ctx, int64_t timeout, int maxmsg,
                                         uint64_t *param_4);               /* @0x1400195f0 */

/* ==================================================================
 * 引导扇区模板 (.data 直取, 见文件头注)
 * ================================================================== */
static const byte g_bootDos[400] = {      /* DAT_14013a370 "dos" 模板 */
    0x33,0xc0,0x8e,0xd0,0xbc,0x00,0x7c,0xfb,0x50,0x07,0x50,0x1f,0xfc,0xbe,0x1b,0x7c,
    0xbf,0x1b,0x06,0x50,0x57,0xb9,0xe5,0x01,0xf3,0xa4,0xcb,0xbe,0xbe,0x07,0xb1,0x04,
    0x38,0x2c,0x7c,0x09,0x75,0x15,0x83,0xc6,0x10,0xe2,0xf5,0xcd,0x18,0x8b,0x14,0x8b,
    0xee,0x83,0xc6,0x10,0x49,0x74,0x16,0x38,0x2c,0x74,0xf6,0xbe,0x10,0x07,0x4e,0xac,
    0x3c,0x00,0x74,0xfa,0xbb,0x07,0x00,0xb4,0x0e,0xcd,0x10,0xeb,0xf2,0x89,0x46,0x25,
    0x96,0x8a,0x46,0x04,0xb4,0x06,0x3c,0x0e,0x74,0x11,0xb4,0x0b,0x3c,0x0c,0x74,0x05,
    0x3a,0xc4,0x75,0x2b,0x40,0xc6,0x46,0x25,0x06,0x75,0x24,0xbb,0xaa,0x55,0x50,0xb4,
    0x41,0xcd,0x13,0x58,0x72,0x16,0x81,0xfb,0x55,0xaa,0x75,0x10,0xf6,0xc1,0x01,0x74,
    0x0b,0x8a,0xe0,0x88,0x56,0x24,0xc7,0x06,0xa1,0x06,0xeb,0x1e,0x88,0x66,0x04,0xbf,
    0x0a,0x00,0xb8,0x01,0x02,0x8b,0xdc,0x33,0xc9,0x83,0xff,0x05,0x7f,0x03,0x8b,0x4e,
    0x25,0x03,0x4e,0x02,0xcd,0x13,0x72,0x29,0xbe,0x46,0x07,0x81,0x3e,0xfe,0x7d,0x55,
    0xaa,0x74,0x5a,0x83,0xef,0x05,0x7f,0xda,0x85,0xf6,0x75,0x83,0xbe,0x27,0x07,0xeb,
    0x8a,0x98,0x91,0x52,0x99,0x03,0x46,0x08,0x13,0x56,0x0a,0xe8,0x12,0x00,0x5a,0xeb,
    0xd5,0x4f,0x74,0xe4,0x33,0xc0,0xcd,0x13,0xeb,0xb8,0x00,0x00,0x00,0x00,0x00,0x00,
    0x56,0x33,0xf6,0x56,0x56,0x52,0x50,0x06,0x53,0x51,0xbe,0x10,0x00,0x56,0x8b,0xf4,
    0x50,0x52,0xb8,0x00,0x42,0x8a,0x56,0x24,0xcd,0x13,0x5a,0x58,0x8d,0x64,0x10,0x72,
    0x0a,0x40,0x75,0x01,0x42,0x80,0xc7,0x02,0xe2,0xf7,0xf8,0x5e,0xc3,0xeb,0x74,0x49,
    0x6e,0x76,0x61,0x6c,0x69,0x64,0x20,0x70,0x61,0x72,0x74,0x69,0x74,0x69,0x6f,0x6e,
    0x20,0x74,0x61,0x62,0x6c,0x65,0x00,0x45,0x72,0x72,0x6f,0x72,0x20,0x6c,0x6f,0x61,
    0x64,0x69,0x6e,0x67,0x20,0x6f,0x70,0x65,0x72,0x61,0x74,0x69,0x6e,0x67,0x20,0x73,
    0x79,0x73,0x74,0x65,0x6d,0x00,0x4d,0x69,0x73,0x73,0x69,0x6e,0x67,0x20,0x6f,0x70,
    0x65,0x72,0x61,0x74,0x69,0x6e,0x67,0x20,0x73,0x79,0x73,0x74,0x65,0x6d,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x8b,0xfc,0x1e,0x57,0x8b,0xf5,0xcb,0x00,0x00,0x00,0x00,0x00,0x00,
};
static const byte g_bootNt5[384] = {      /* DAT_14013a500 "nt5" 模板 */
    0x33,0xc0,0x8e,0xd0,0xbc,0x00,0x7c,0xfb,0x50,0x07,0x50,0x1f,0xfc,0xbe,0x1b,0x7c,
    0xbf,0x1b,0x06,0x50,0x57,0xb9,0xe5,0x01,0xf3,0xa4,0xcb,0xbd,0xbe,0x07,0xb1,0x04,
    0x38,0x6e,0x00,0x7c,0x09,0x75,0x13,0x83,0xc5,0x10,0xe2,0xf4,0xcd,0x18,0x8b,0xf5,
    0x83,0xc6,0x10,0x49,0x74,0x19,0x38,0x2c,0x74,0xf6,0xa0,0xb5,0x07,0xb4,0x07,0x8b,
    0xf0,0xac,0x3c,0x00,0x74,0xfc,0xbb,0x07,0x00,0xb4,0x0e,0xcd,0x10,0xeb,0xf2,0x88,
    0x4e,0x10,0xe8,0x46,0x00,0x73,0x2a,0xfe,0x46,0x10,0x80,0x7e,0x04,0x0b,0x74,0x0b,
    0x80,0x7e,0x04,0x0c,0x74,0x05,0xa0,0xb6,0x07,0x75,0xd2,0x80,0x46,0x02,0x06,0x83,
    0x46,0x08,0x06,0x83,0x56,0x0a,0x00,0xe8,0x21,0x00,0x73,0x05,0xa0,0xb6,0x07,0xeb,
    0xbc,0x81,0x3e,0xfe,0x7d,0x55,0xaa,0x74,0x0b,0x80,0x7e,0x10,0x00,0x74,0xc8,0xa0,
    0xb7,0x07,0xeb,0xa9,0x8b,0xfc,0x1e,0x57,0x8b,0xf5,0xcb,0xbf,0x05,0x00,0x8a,0x56,
    0x00,0xb4,0x08,0xcd,0x13,0x72,0x23,0x8a,0xc1,0x24,0x3f,0x98,0x8a,0xde,0x8a,0xfc,
    0x43,0xf7,0xe3,0x8b,0xd1,0x86,0xd6,0xb1,0x06,0xd2,0xee,0x42,0xf7,0xe2,0x39,0x56,
    0x0a,0x77,0x23,0x72,0x05,0x39,0x46,0x08,0x73,0x1c,0xb8,0x01,0x02,0xbb,0x00,0x7c,
    0x8b,0x4e,0x02,0x8b,0x56,0x00,0xcd,0x13,0x73,0x51,0x4f,0x74,0x4e,0x32,0xe4,0x8a,
    0x56,0x00,0xcd,0x13,0xeb,0xe4,0x8a,0x56,0x00,0x60,0xbb,0xaa,0x55,0xb4,0x41,0xcd,
    0x13,0x72,0x36,0x81,0xfb,0x55,0xaa,0x75,0x30,0xf6,0xc1,0x01,0x74,0x2b,0x61,0x60,
    0x6a,0x00,0x6a,0x00,0xff,0x76,0x0a,0xff,0x76,0x08,0x6a,0x00,0x68,0x00,0x7c,0x6a,
    0x01,0x6a,0x10,0xb4,0x42,0x8b,0xf4,0xcd,0x13,0x61,0x61,0x73,0x0e,0x4f,0x74,0x0b,
    0x32,0xe4,0x8a,0x56,0x00,0xcd,0x13,0xeb,0xd6,0x61,0xf9,0xc3,0x49,0x6e,0x76,0x61,
    0x6c,0x69,0x64,0x20,0x70,0x61,0x72,0x74,0x69,0x74,0x69,0x6f,0x6e,0x20,0x74,0x61,
    0x62,0x6c,0x65,0x00,0x45,0x72,0x72,0x6f,0x72,0x20,0x6c,0x6f,0x61,0x64,0x69,0x6e,
    0x67,0x20,0x6f,0x70,0x65,0x72,0x61,0x74,0x69,0x6e,0x67,0x20,0x73,0x79,0x73,0x74,
    0x65,0x6d,0x00,0x4d,0x69,0x73,0x73,0x69,0x6e,0x67,0x20,0x6f,0x70,0x65,0x72,0x61,
    0x74,0x69,0x6e,0x67,0x20,0x73,0x79,0x73,0x74,0x65,0x6d,0x00,0x00,0x00,0x00,0x00,
};
static const byte g_bootNt6[440] = {      /* DAT_14013a680 "nt6"/"win" 模板 */
    0x33,0xc0,0x8e,0xd0,0xbc,0x00,0x7c,0x8e,0xc0,0x8e,0xd8,0xbe,0x00,0x7c,0xbf,0x00,
    0x06,0xb9,0x00,0x02,0xfc,0xf3,0xa4,0x50,0x68,0x1c,0x06,0xcb,0xfb,0xb9,0x04,0x00,
    0xbd,0xbe,0x07,0x80,0x7e,0x00,0x00,0x7c,0x0b,0x0f,0x85,0x0e,0x01,0x83,0xc5,0x10,
    0xe2,0xf1,0xcd,0x18,0x88,0x56,0x00,0x55,0xc6,0x46,0x11,0x05,0xc6,0x46,0x10,0x00,
    0xb4,0x41,0xbb,0xaa,0x55,0xcd,0x13,0x5d,0x72,0x0f,0x81,0xfb,0x55,0xaa,0x75,0x09,
    0xf7,0xc1,0x01,0x00,0x74,0x03,0xfe,0x46,0x10,0x66,0x60,0x80,0x7e,0x10,0x00,0x74,
    0x26,0x66,0x68,0x00,0x00,0x00,0x00,0x66,0xff,0x76,0x08,0x68,0x00,0x00,0x68,0x00,
    0x7c,0x68,0x01,0x00,0x68,0x10,0x00,0xb4,0x42,0x8a,0x56,0x00,0x8b,0xf4,0xcd,0x13,
    0x9f,0x83,0xc4,0x10,0x9e,0xeb,0x14,0xb8,0x01,0x02,0xbb,0x00,0x7c,0x8a,0x56,0x00,
    0x8a,0x76,0x01,0x8a,0x4e,0x02,0x8a,0x6e,0x03,0xcd,0x13,0x66,0x61,0x73,0x1c,0xfe,
    0x4e,0x11,0x75,0x0c,0x80,0x7e,0x00,0x80,0x0f,0x84,0x8a,0x00,0xb2,0x80,0xeb,0x84,
    0x55,0x32,0xe4,0x8a,0x56,0x00,0xcd,0x13,0x5d,0xeb,0x9e,0x81,0x3e,0xfe,0x7d,0x55,
    0xaa,0x75,0x6e,0xff,0x76,0x00,0xe8,0x8d,0x00,0x75,0x17,0xfa,0xb0,0xd1,0xe6,0x64,
    0xe8,0x83,0x00,0xb0,0xdf,0xe6,0x60,0xe8,0x7c,0x00,0xb0,0xff,0xe6,0x64,0xe8,0x75,
    0x00,0xfb,0xb8,0x00,0xbb,0xcd,0x1a,0x66,0x23,0xc0,0x75,0x3b,0x66,0x81,0xfb,0x54,
    0x43,0x50,0x41,0x75,0x32,0x81,0xf9,0x02,0x01,0x72,0x2c,0x66,0x68,0x07,0xbb,0x00,
    0x00,0x66,0x68,0x00,0x02,0x00,0x00,0x66,0x68,0x08,0x00,0x00,0x00,0x66,0x53,0x66,
    0x53,0x66,0x55,0x66,0x68,0x00,0x00,0x00,0x00,0x66,0x68,0x00,0x7c,0x00,0x00,0x66,
    0x61,0x68,0x00,0x00,0x07,0xcd,0x1a,0x5a,0x32,0xf6,0xea,0x00,0x7c,0x00,0x00,0xcd,
    0x18,0xa0,0xb7,0x07,0xeb,0x08,0xa0,0xb6,0x07,0xeb,0x03,0xa0,0xb5,0x07,0x32,0xe4,
    0x05,0x00,0x07,0x8b,0xf0,0xac,0x3c,0x00,0x74,0x09,0xbb,0x07,0x00,0xb4,0x0e,0xcd,
    0x10,0xeb,0xf2,0xf4,0xeb,0xfd,0x2b,0xc9,0xe4,0x64,0xeb,0x00,0x24,0x02,0xe0,0xf8,
    0x24,0x02,0xc3,0x49,0x6e,0x76,0x61,0x6c,0x69,0x64,0x20,0x70,0x61,0x72,0x74,0x69,
    0x74,0x69,0x6f,0x6e,0x20,0x74,0x61,0x62,0x6c,0x65,0x00,0x45,0x72,0x72,0x6f,0x72,
    0x20,0x6c,0x6f,0x61,0x64,0x69,0x6e,0x67,0x20,0x6f,0x70,0x65,0x72,0x61,0x74,0x69,
    0x6e,0x67,0x20,0x73,0x79,0x73,0x74,0x65,0x6d,0x00,0x4d,0x69,0x73,0x73,0x69,0x6e,
    0x67,0x20,0x6f,0x70,0x65,0x72,0x61,0x74,0x69,0x6e,0x67,0x20,0x73,0x79,0x73,0x74,
    0x65,0x6d,0x00,0x00,0x00,0x63,0x7b,0x9a,
};
static const byte g_gptDiskGuidDef[12] = { /* DAT_140127a50 GPT 磁盘 GUID 兜底 12B */
    0xf2,0xfc,0x34,0xe3,0x95,0xa3,0xeb,0x4c,0x93,0x95,0x9d,0x79,
};

/* 设备接口类 GUID (DAT_14011e870/880/8a0, 字节序按内存布局) */
static const byte g_guidDisk[16] = {      /* DAT_14011e870 53F56307-B6BF-11D0-94F2-00A0C91EFB8B */
    0x07,0x63,0xf5,0x53,0xbf,0xb6,0xd0,0x11,0x94,0xf2,0x00,0xa0,0xc9,0x1e,0xfb,0x8b,
};
static const byte g_guidCdrom[16] = {     /* DAT_14011e880 53F56308-B6BF-11D0-94F2-00A0C91EFB8B */
    0x08,0x63,0xf5,0x53,0xbf,0xb6,0xd0,0x11,0x94,0xf2,0x00,0xa0,0xc9,0x1e,0xfb,0x8b,
};
static const byte g_guidFloppy[16] = {    /* DAT_14011e8a0 53F56311-B6BF-11D0-94F2-00A0C91EFB8B */
    0x11,0x63,0xf5,0x53,0xbf,0xb6,0xd0,0x11,0x94,0xf2,0x00,0xa0,0xc9,0x1e,0xfb,0x8b,
};
static const byte g_gptUnusedEntry16[16] = { /* DAT_140124100 GPT 未用条目模板 (全 0) */
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
};

/* ==================================================================
 * @0x14008cffc  PART 命令引擎 (PECMD_ExecCommandLine)
 * signature: LARGE_INTEGER __fastcall FUN_14008cffc(longlong *param_1,
 *                                                    LPCWSTR param_2)
 * ================================================================== */
LARGE_INTEGER PECMD_ExecCommandLine(int64_t *param_1, LPCWSTR param_2)
{
    /* ---- dc 寄存器工作变量 (89810-89959 原序) ---- */
    byte *lpBuffer;
    DWORD *pDVar1;
    short sVar2;
    LARGE_INTEGER LVar3;
    size_t _Size;
    uint32_t uVar4;
    uint32_t uVar5;
    uint32_t uVar6;
    uint32_t uVar7;
    uint32_t uVar8;
    uint32_t uVar9;
    uint32_t uVar10;
    uint32_t uVar11;
    char cVar12;
    char cVar13;
    WCHAR WVar14;
    uint16_t uVar15;
    short sVar16;
    int iVar17;
    uint uVar18;
    BOOL BVar19;
    uint uVar20;
    uint uVar21;
    BOOL BVar22;
    DWORD DVar23;
    DWORD DVar24;
    uint uVar25;
    int iVar26;
    int iVar27;
    UINT UVar28;
    longlong *plVar29;
    WCHAR *ptVar30;                 /* 游标副本 (dc pthreadmbcinfo) */
    ulonglong uVar31;
    uint64_t uVar32;
    LARGE_INTEGER LVar33;
    longlong lVar34;
    LARGE_INTEGER LVar35;
    ulonglong *puVar36;
    WCHAR *pWVar37;
    uint *puVar38;
    LPCWSTR pWVar39;
    WCHAR WVar40;
    void **ppuVar41;
    ulonglong uVar42;
    LPCWSTR *ppWVar43;
    int64_t ptVar44;                /* 哨兵/游标两用 (dc pthreadmbcinfo) */
    wchar_t *pwVar45;
    uint8_t uVar46;
    HANDLE pvVar47;
    uint16_t *puVar48;
    ulonglong uVar49;
    byte *puVar50;
    LARGE_INTEGER LVar51;
    byte bVar52;
    WCHAR *ptVar53;
    const void *lpBuffer_00;
    LPWSTR pWVar54;
    WCHAR WVar55;
    longlong lVar56;
    char *pcVar57;
    ushort uVar58;
    ushort *puVar59;
    ulonglong *puVar60;
    GUID *pGVar61;
    ushort uVar62;
    LPCWSTR pWVar63;
    LPWSTR pWVar64;
    ulonglong uVar65;
    char *pcVar66;
    byte bVar67;
    int64_t ptVar68;                /* 哨兵/游标两用 (dc pthreadmbcinfo) */
    LARGE_INTEGER LVar69;
    longlong lVar70;
    ulonglong uVar71;
    ulonglong *_Src;
    bool bVar72;
    bool bVar73;

    /* ---- dc 局部槽 (89960-90016 原序) ---- */
    WCHAR *cur;                     /* dc local_res10 主游标 */
    uint64_t local_res20;           /* 多用途槽 (dc undefined8) */
    byte local_468;
    byte local_467;
    byte local_466;
    byte local_465;
    char local_464;
    byte local_463;
    char local_462;
    char local_461;
    byte local_460;
    byte local_45f;
    uint local_45c;
    LARGE_INTEGER local_458;
    char local_450;
    byte local_44f[3];
    uint local_44c;
    ulonglong local_448;
    DWORD local_440;
    byte local_43c;
    DWORD local_438;
    uint local_434;
    byte local_430;
    byte local_42f;
    uint local_42c;
    LARGE_INTEGER local_428;
    uint local_420[2];
    WCHAR *local_418;
    char local_410;
    char local_40f;
    char local_40e;
    byte local_40d;
    char local_40c;
    char local_40b;
    char local_40a;
    char local_409;
    uint local_408;
    int local_404;
    uint local_400;
    uint local_3fc;
    LARGE_INTEGER local_3f8;
    DWORD local_3f0;
    char local_3ec;
    byte local_3eb;
    char local_3ea;
    uint local_3e8;
    char local_3e4;
    ulonglong local_3e0;
    byte local_3d8;
    char local_3d7;
    byte local_3d6;
    uint local_3d4;
    void *local_3d0;
    uint local_3c8;
    void *local_3c0;
    LARGE_INTEGER local_3b8;
    DWORD local_3b0;
    uint local_3ac;
    HANDLE local_3a8;
    char local_3a0;
    byte local_39f;
    bool local_39e;
    char local_39d;
    char local_39c;
    char local_39b;
    uint local_398;
    void *local_390;
    LPWSTR local_388;
    uint local_380;
    LARGE_INTEGER local_378;
    int64_t local_370;              /* dc LPWSTR/-1 哨兵混用 */
    WCHAR *local_360;
    uint local_358[2];
    LARGE_INTEGER local_350;
    LARGE_INTEGER local_348;
    uint local_340;
    void *local_338;
    WCHAR *local_330;
    ulonglong local_328;
    LARGE_INTEGER local_320;
    void *local_318;
    uint local_310;
    int64_t local_308;              /* dc 哨兵/数值混用 */
    uint local_300;
    uint local_368;
    WCHAR *local_2f8;
    LARGE_INTEGER local_2f0;
    uint64_t local_2e8;             /* GPT 盘 GUID 前 8B (dc 0x4433b9e5ebd0a0a2) */
    uint64_t LStack_2e0;            /* GPT 盘 GUID 后 8B (dc -0x3866d94849973f79) */
    LARGE_INTEGER local_2d8;
    int local_2d0[2];
    char *local_2c8;
    uint local_2c0;
    LARGE_INTEGER local_2b8;
    LARGE_INTEGER local_2b0;
    ushort *local_2a8;
    LARGE_INTEGER local_2a0;
    WCHAR local_298[4];
    WCHAR *local_290;
    LARGE_INTEGER local_288;
    LARGE_INTEGER local_280;
    LARGE_INTEGER local_278;
    LARGE_INTEGER LStack_270;
    LARGE_INTEGER local_268;
    WCHAR *local_260;
    LARGE_INTEGER local_258;
    int local_250[2];
    int64_t local_248;              /* dc 哨兵/数值混用 */
    LPCWSTR local_240;
    LARGE_INTEGER local_238;
    int local_230;
    int64_t local_228;              /* dc 哨兵/数值混用 */
    int local_220;
    LARGE_INTEGER local_218;
    int local_210;
    LARGE_INTEGER local_208;
    int64_t local_200;              /* dc 串标签指针混用 */
    LARGE_INTEGER local_1f8;
    LARGE_INTEGER local_1f0;
    int64_t local_1e8;              /* dc LPCWSTR/-9 哨兵混用 */
    LPCWSTR local_1e0;
    DWORD local_1d8[2];
    int64_t local_1d0;
    undefined8 local_1c8;
    DWORD local_1c0;
    DWORD local_1bc;
    DWORD local_1b8[2];
    int64_t local_1b0;
    undefined8 local_1a8;
    longlong local_1a0;
    uint32_t local_198[4];          /* dc local_198/194/190/18c (WriteMbrPartitionEntry 出参区) */
    /* dc local_188/184/17c/178/174/16c/168: GetFileAttributesExW (WIN32_FILE_ATTRIBUTE_DATA)
     * 与 DeviceIoControl 0x70000 (DISK_GEOMETRY 前 20B) 共用栈区, union 承载 */
    union {
        byte geo[40];   /* +0 attr / +28 sizeHi / +32 sizeLo (FAD); DISK_GEOMETRY 前 24B */
    } local_188u;
    DWORD local_160;
    DWORD local_15c;
    DWORD local_158;
    DWORD local_154[3];
    byte local_148[280];            /* dc local_148[272] (\\.\X: 构造 / 条目交换暂存) */

    /* ================= dc:90018-90111 初始化 ================= */
    cur = (WCHAR *)param_2;
    PECMD_SkipLeadingControlChars((long long *)&cur);                        /* dc:90019 */
    PECMD_AllocWStringBuffer(&local_418, 5);                                 /* dc:90020 */
    PECMD_AllocWStringBuffer((WCHAR **)&local_1d0, 0x14);                    /* dc:90021 */
    PECMD_AllocStrSlot(&local_260);                                          /* dc:90022 */
    PECMD_AllocStrSlot(&local_2f8);                                          /* dc:90023 */
    PECMD_AllocStrSlot(&local_290);                                          /* dc:90024 */
    ppWVar43 = (LPCWSTR *)&local_2f8;
    local_3c8 = 0xffffffff;
    local_45c = 0xffffffff;
    local_428.QuadPart = -1;
    local_2f0.QuadPart = -1;
    local_3f8.QuadPart = -1;
    local_258.QuadPart = -1;
    local_1f8.QuadPart = -1;
    local_280.QuadPart = -1;
    local_218.QuadPart = -1;
    local_3b8.QuadPart = -1;
    local_448 = 0;
    local_308 = -1;                                                          /* dc ...fff7 哨兵 */
    local_1a8 = 0;
    local_2a8 = NULL;
    local_370 = 0;                    /* dc local_370 = NULL */
    local_378.QuadPart = -1;
    local_434 = 0xfffc0000;
    local_res20 = 0;                  /* dc CONCAT44(_4_4_,0)<<32 → 0 */
    local_3ac = 0;
    local_380 = 0xffff;               /* dc CONCAT22(_2_2_,0xffff) */
    local_2a0.QuadPart = 0;
    local_288.QuadPart = 0;
    local_3f0 = 0;
    local_208.QuadPart = -1;
    local_238.QuadPart = -1;
    local_1f0.QuadPart = -1;
    local_320.QuadPart = -1;
    local_3e4 = '\0';
    local_40a = '\0';
    local_39c = '\0';
    local_39d = '\0';
    local_461 = '\0';
    local_430 = 0;
    local_39f = 0;
    local_43c = 0;
    local_40c = '\0';
    local_368 = 0;                    /* dc local_368 复用 local_300 声明槽 */
    local_250[0] = 500;
    PECMD_SplitTokenTrimWs(&cur, &local_2f8, 0x2c);                          /* dc:90064 第一个逗号切分 */
    *local_2f8 = L'\0';                                                      /* dc:90066 */
    if (*cur == L',') {                                                      /* dc:90067 */
        *cur = L'\0';
        cur = cur + 1;
        ppWVar43 = (LPCWSTR *)&local_2f8;
        PECMD_SplitTokenTrimWs(&cur, &local_2f8, 0x2c);                      /* dc:90071 第二切分 */
        /* dc:90072 local_310 = (uint)ppWVar43 — Ghidra 寄存器残影, 无 store, 不落码 */
    }
    pWVar39 = local_2f8;
    /* dc:90075 local_310 &= 0xffffff00 — 同上残影 */
    local_45f = 0;
    local_3e8 = 0;                    /* dc local_3e8 &= 0xffffff00 */
    local_3a0 = -1;
    local_398 = 0xfffff000;
    local_358[0] = 0xfffff000;
    local_340 = 0;                    /* dc local_340 &= 0xffffff00 */
    local_3ec = '\0';
    local_39b = '\0';
    local_450 = '\0';
    local_462 = '\0';
    local_467 = 0;
    local_468 = 0;
    local_465 = 0;
    local_464 = '\0';
    local_3ea = '\0';
    local_2c0 = 0xff;                 /* dc CONCAT31(_1_3_,0xff) */
    local_40b = -1;
    local_300 = 0;
    local_1e0 = NULL;
    local_200 = 0;
    uVar18 = 0;
    local_408 = 0;
    local_410 = '\0';
    local_42f = 0;
    local_438 = 0xffffffff;
    local_420[0] = 0xffffffff;
    uVar49 = 1;
    local_248 = -9;                   /* dc ...fff7 */
    local_390 = (void *)-9;           /* dc ...fff7 */
    ptVar68 = -9;                     /* dc ...fff7 */
    local_228 = -9;                   /* dc ...fff7 */
    local_308 = -9;                   /* dc 覆盖: ...fff7 (与 asm 0x14008d2c1+ 一致) */
    local_240 = (LPCWSTR)0x1;
    local_348.QuadPart = 4;
    bVar52 = 1;
    if (*cur == L'\0') {                                                     /* dc:90111 */
        /* dc:90608-90609 LAB_14008e2bd */
        local_248 = (int64_t)0x1;
        goto LAB_14008e2bd;
    }

    /* ================= dc:90112-90595 选项循环 ================= */
LAB_14008d30e:
    cVar12 = PECMD_MatchTokenAdvance("-admin", &cur, 6);                     /* dc:90113 */
    if (cVar12 != '\0') {
        local_45f = local_45f | 1;
        goto LAB_14008e216;
    }
    cVar12 = PECMD_MatchTokenAdvance("-super", &cur, 6);                     /* dc:90118 */
    if (cVar12 != '\0') {
        local_45f = local_45f | 2;
        goto LAB_14008e216;
    }
    cVar12 = PECMD_MatchTokenAdvance("-devid", &cur, 6);                     /* dc:90123 */
    if (cVar12 != '\0') {
        local_3ac = local_3ac | 0x10000;
        goto LAB_14008e216;
    }
    cVar12 = PECMD_MatchTokenAdvance("-devidx", &cur, 7);                    /* dc:90128 */
    if (cVar12 != '\0') {
        local_3ac = local_3ac | 0x20000;
        goto LAB_14008e216;
    }
    cVar12 = PECMD_MatchTokenAdvance("-devidn", &cur, 7);                    /* dc:90133 */
    if (cVar12 != '\0') {
        local_3ac = local_3ac | 0x80000;
        goto LAB_14008e216;
    }
    cVar12 = PECMD_MatchTokenAdvance("-devida", &cur, 7);                    /* dc:90138 */
    if (cVar12 != '\0') {
        local_3ac = local_3ac | 0x40000;
        goto LAB_14008e216;
    }
    cVar12 = PECMD_MatchTokenAdvance("-gui", &cur, 4);                       /* dc:90143 */
    if (cVar12 != '\0') {
        FUN_1400702B0((WCHAR **)&local_res20, L"#21:INDATA ");
        FUN_14006375C((WCHAR **)&local_res20, (LPCWSTR)(uintptr_t)cur);
        LVar69.QuadPart = FUN_140031454((long long *)param_1, (char *)(uintptr_t)local_res20);
        PECMD_FreeStrBuf(&local_res20);
        goto LAB_14009445a;
    }
    cVar12 = PECMD_MatchTokenAdvance("-usb", &cur, 4);                       /* dc:90151 */
    if (cVar12 != '\0') goto LAB_14008e216;
    cVar12 = PECMD_MatchTokenAdvance("-hextp", &cur, 6);                     /* dc:90153 */
    if (cVar12 != '\0') {
        local_408 = uVar18 | 1;
        uVar18 = local_408;
        goto LAB_14008e216;
    }
    cVar12 = PECMD_MatchTokenAdvance("-phy", &cur, 4);                       /* dc:90159 */
    if (cVar12 != '\0') {
        local_408 = uVar18 | 2;
        uVar18 = local_408;
        goto LAB_14008e216;
    }
    cVar12 = PECMD_MatchTokenAdvance("-phy#", &cur, 5);                      /* dc:90165 */
    if (cVar12 != '\0') {
        local_408 = uVar18 | 0x10;
        uVar18 = local_408;
        goto LAB_14008e216;
    }
    cVar12 = PECMD_MatchTokenAdvance("-drv", &cur, 4);                       /* dc:90171 */
    if (cVar12 != '\0') {
        local_408 = uVar18 | 8;
        uVar18 = local_408;
        goto LAB_14008e216;
    }
    cVar12 = PECMD_MatchTokenAdvance("-fill", &cur, 5);                      /* dc:90177 */
    if (cVar12 != '\0') {
        local_408 = uVar18 | 4;
        uVar18 = local_408;
        goto LAB_14008e216;
    }
    cVar12 = PECMD_MatchTokenAdvance("-dvol", &cur, 5);                      /* dc:90183 */
    if (cVar12 != '\0') {
        local_408 = uVar18 | 0x100;
        uVar18 = local_408;
        goto LAB_14008e216;
    }
    cVar12 = PECMD_MatchTokenAdvance("-up", &cur, 3);                        /* dc:90189 */
    if (cVar12 != '\0') {
        local_467 = 1;
        goto LAB_14008e216;
    }
    cVar12 = PECMD_MatchTokenAdvance("-xup", &cur, 4);                       /* dc:90194 */
    if (cVar12 != '\0') {
        local_468 = 1;
        goto LAB_14008e216;
    }
    cVar12 = PECMD_MatchTokenAdvance("-axup", &cur, 5);                      /* dc:90199 */
    if (cVar12 != '\0') {
        local_468 = 0x21;
        goto LAB_14008e216;
    }
    cVar12 = PECMD_MatchTokenAdvance("-hup", &cur, 4);                       /* dc:90204 */
    if (cVar12 != '\0') {
        local_465 = 1;
        goto LAB_14008e216;
    }
    cVar12 = PECMD_MatchTokenAdvance("-ahup", &cur, 5);                      /* dc:90209 */
    if (cVar12 != '\0') {
        local_465 = 0x21;
        goto LAB_14008e216;
    }
    cVar12 = PECMD_MatchTokenAdvance("-gpt", &cur, 4);                       /* dc:90214 */
    if (cVar12 != '\0') {
        local_464 = '\x01';
        goto LAB_14008e216;
    }
    cVar12 = PECMD_MatchTokenAdvance("-gptmini", &cur, 8);                   /* dc:90219 (asm r8d=ebx=8) */
    if (cVar12 != '\0') {
        local_464 = '\x02';
        goto LAB_14008e216;
    }
    cVar12 = PECMD_MatchTokenAdvance("-gpth", &cur, 5);                      /* dc:90224 (asm r8d=bl=5) */
    if (cVar12 != '\0') {
        local_464 = '\x04';
        goto LAB_14008e216;
    }
    cVar12 = PECMD_MatchTokenAdvance("-mbr", &cur, 4);                       /* dc:90229 */
    if (cVar12 != '\0') {
        local_39d = '\x01';
        goto LAB_14008e216;
    }
    cVar12 = PECMD_MatchTokenAdvance("-cmp", &cur, 4);                       /* dc:90234 */
    if (cVar12 != '\0') {
        local_3ea = '\x01';
        goto LAB_14008e216;
    }
    cVar12 = PECMD_MatchTokenAdvance("-force", &cur, 6);                     /* dc:90239 */
    if (cVar12 != '\0') {
        local_39c = '\x01';
        goto LAB_14008e216;
    }
    bVar72 = PECMD_MatchAndAdvance("-fs0:", (int64_t *)(uintptr_t)&cur, 5);  /* dc:90244 */
    if (bVar72) {
        local_43c = 0x10;
        PECMD_ParseHexOrDecBool((long long *)&cur, local_250);               /* dc:90247 */
        goto LAB_14008e216;
    }
    cVar12 = PECMD_MatchTokenAdvance("-fs0", &cur, 4);                       /* dc:90250 */
    if (cVar12 != '\0') {
        local_43c = 0x10;
        goto LAB_14008e216;
    }
    cVar12 = PECMD_MatchTokenAdvance("-fs", &cur, 3);                        /* dc:90255 */
    if (cVar12 != '\0') {
        local_39f = 1;
        goto LAB_14008e216;
    }
    cVar12 = PECMD_MatchTokenAdvance("-x", &cur, 2);                         /* dc:90260 */
    if (cVar12 != '\0') {
        local_300 = local_300 + 0x100;
        goto LAB_14008e216;
    }
    cVar12 = PECMD_MatchTokenAdvance("-img", &cur, 4);                       /* dc:90265 */
    if (cVar12 != '\0') {
        local_40c = '\x01';
        goto LAB_14008e216;
    }
    cVar12 = PECMD_MatchTokenAdvance("-lskip", &cur, 6);                     /* dc:90270 */
    if (cVar12 != '\0') {
        local_430 = local_430 | 0x40;
        goto LAB_14008e216;
    }
    cVar12 = PECMD_MatchTokenAdvance("-locku", &cur, 6);                     /* dc:90275 */
    if (cVar12 != '\0') {
LAB_14008d970:
        local_461 = '\x03';
        goto LAB_14008e216;
    }
    /* dc:90281-90301 -locku: / -lock: / -lock / -lockx / -lock- */
    ptVar30 = cur;
    uVar32 = (uint64_t)(int)FUN_14005C788("-locku:", cur, 7);                /* dc:90282 不推进 */
    if ((char)uVar32 == '\0') {
        uVar32 = (uint64_t)(int)FUN_14005C788("-lock:", cur, 6);             /* dc:90284 */
        if ((char)uVar32 != '\0') {
            local_461 = '\x01';
            /* dc:90287 -lock: 值 = cur+6 WCHAR (asm add r11,0xc) */
            cur = cur + 6;
            WVar40 = *cur;
            goto LAB_14008d878_val;
        }
        cVar12 = PECMD_MatchTokenAdvance("-lock", &cur, 5);                  /* dc:90289 */
        if (cVar12 != '\0') {
            local_461 = '\x01';
            goto LAB_14008e216;
        }
        cVar12 = PECMD_MatchTokenAdvance("-lockx", &cur, 6);                 /* dc:90294 */
        if (cVar12 != '\0') goto LAB_14008d970;
        cVar12 = PECMD_MatchTokenAdvance("-lock-", &cur, 6);                 /* dc:90296 */
        if (cVar12 != '\0') {
            local_461 = -0x10;
            local_370 = -1;
            goto LAB_14008e216;
        }
        /* dc:90302-90326 /mbr (不推进比较, 匹配后看 +4 WCHAR 处) */
        ptVar44 = (int64_t)(uintptr_t)cur;
        uVar32 = (uint64_t)(int)FUN_14005C788("/mbr", cur, 4);
        if ((char)uVar32 != '\0') {
            /* dc:90305 &ptVar44->ismbcodepage = p+4 WCHAR (asm lea rax,[r11+8]) */
            ptVar30 = cur + 4;
            WVar40 = L'\t';
            if ((((*ptVar30 != L'\0') && (*ptVar30 != L'=')) &&
                 (((uint16_t)*ptVar30 < 9 || (0xd < (uint16_t)*ptVar30)))) &&
                (*ptVar30 != L' ')) {
                goto LAB_14008da0f;
            }
            WVar55 = L' ';
            local_200 = 0;
            if (*ptVar30 == L'=') {
                /* dc:90313-90320 /mbr=值 */
                cur = ptVar30 + 1;
                plVar29 = PECMD_SkipLeadingControlChars((long long *)&cur);
                local_1e0 = (LPCWSTR)*plVar29;
                for (; WVar14 = *cur, ptVar30 = cur, WVar14 != L'\0';
                    cur = cur + 1) {
                    if ((((uint16_t)WVar40 <= (uint16_t)WVar14) && ((uint16_t)WVar14 < 0xe)) ||
                        (WVar55 == WVar14)) {
                        goto LAB_14008dc86;
                    }
                }
            }
            else {
                local_1e0 = L"nt6";                                          /* dc:90323 */
            }
            goto LAB_14008e1ff;
        }
LAB_14008da0f:
        /* dc:90328-90333 /pbr (不推进, +4 WCHAR 处为 token 终止判定) */
        uVar32 = (uint64_t)(int)FUN_14005C788("/pbr", (WCHAR *)(uintptr_t)ptVar44, 4);
        if (((char)uVar32 == '\0') ||
            (((ptVar30 = (WCHAR *)(uintptr_t)ptVar44 + 4, *ptVar30 != L'\0' &&
              (*ptVar30 != L'=')) &&
             ((((uint16_t)*ptVar30 < 9 || (0xd < (uint16_t)*ptVar30))) &&
              (*ptVar30 != L' '))))) {
            /* dc:90334-90369 -alignend=N */
            uVar32 = (uint64_t)(int)FUN_14005C788("-alignend", (WCHAR *)(uintptr_t)ptVar44, 9);
            if (((char)uVar32 != '\0') &&
                ((((ptVar30 = (WCHAR *)(uintptr_t)ptVar44 + 9, *ptVar30 == L'\0' ||
                    (*ptVar30 == L'=')) ||
                   ((8 < (uint16_t)*ptVar30 && ((uint16_t)*ptVar30 < 0xe)))) ||
                  (*ptVar30 == L' ')))) {
                WVar40 = *ptVar30;
                cur = ptVar30;
                if (WVar40 == L'=') {
                    WVar55 = ptVar30[1];                                     /* dc:90343 (mbulinfo+4 = p+10) */
                    if ((uint16_t)(WVar55 + 0x20) < 9 || WVar55 == L'9') {
                        WCHAR *pval = ptVar30 + 1;
                        PECMD_ParseSizeAndSkipWs((int64_t *)&pval, (uint64_t *)&local_308);
                        ptVar44 = -2;
                        local_228 = local_308;
                        ptVar68 = local_308;
                        if ((longlong)local_308 < 1) {
                            local_228 = ptVar44;
                            local_308 = ptVar44;
                            ptVar68 = ptVar44;
                        }
                    }
                }
                else {
                    ptVar68 = -2;
                    local_228 = -2;
                    local_308 = -2;
                }
                while ((WVar40 != L'\0' &&
                       ((((uint16_t)WVar40 < 9 || (0xd < (uint16_t)WVar40))) && (WVar40 != L' ')))) {
                    ptVar30 = ptVar30 + 1;
                    cur = ptVar30;
                    WVar40 = *ptVar30;
                }
                PECMD_SkipLeadingControlChars((long long *)&cur);
                goto LAB_14008e216;
            }
            /* dc:90371-90400 -align=N */
            uVar32 = (uint64_t)(int)FUN_14005C788("-align", (WCHAR *)(uintptr_t)ptVar44, 6);
            if ((char)uVar32 != '\0') {
                puVar59 = (ushort *)(uintptr_t)ptVar44 + 6;                  /* dc:90373 mbulinfo = p+6 */
                uVar58 = 0x3d;
                uVar62 = 9;
                ptVar30 = (WCHAR *)puVar59;
                if ((((*puVar59 == 0) || (*puVar59 == 0x3d)) ||
                     ((8 < *puVar59 && (*puVar59 < 0xe)))) || (*puVar59 == 0x20)) {
                    for (; ((WVar40 = *ptVar30, WVar40 != L'\0' &&
                            (((uint16_t)WVar40 < 9 || (0xd < (uint16_t)WVar40)))) &&
                           (WVar40 != L' '));
                        ptVar30 = ptVar30 + 1) {
                    }
                    cur = ptVar30;
                    PECMD_SkipLeadingControlChars((long long *)&cur);
                    if (uVar58 == *puVar59) {
                        if ((uint16_t)(puVar59[1] - 0x30) <= uVar62) {
                            WCHAR *pval = (WCHAR *)puVar59 + 1;
                            PECMD_ParseSizeAndSkipWs((int64_t *)&pval, (uint64_t *)&local_390);
                            local_248 = local_390;
                            if ((longlong)local_390 < 1) {
LAB_14008de06:
                                local_248 = -1;
                                local_390 = (void *)-1;
                            }
                        }
                    }
                    else {
                        goto LAB_14008de06;
                    }
                    goto LAB_14008e216;
                }
            }
            /* dc:90401-90528 -clear/-clean/-clean-/del/-del/-raw/-swap:/-CHS=/
             * -IMG=/-SKIP=/-iv=/-iv/-cdrom/-floppy/未知选项 */
            cVar12 = PECMD_MatchTokenAdvance("-clear", &cur, 6);
            if (cVar12 == '\0') {
                cVar12 = PECMD_MatchTokenAdvance("-clean", &cur, 6);
                if (cVar12 == '\0') {
                    cVar12 = PECMD_MatchTokenAdvance("-clean-", &cur, 7);
                    if (cVar12 == '\0') {
                        cVar12 = PECMD_MatchTokenAdvance("del", &cur, 3);
                        if ((cVar12 == '\0') &&
                            (cVar12 = PECMD_MatchTokenAdvance("-del", &cur, 4), cVar12 == '\0')) {
                            cVar12 = PECMD_MatchTokenAdvance("-raw", &cur, 4);
                            if (cVar12 == '\0') {
                                ptVar30 = cur;
                                uVar32 = (uint64_t)(int)FUN_14005C788("-swap:", cur, 6);
                                if ((char)uVar32 == '\0') {
                                    uVar32 = (uint64_t)(int)FUN_14005C788("-CHS=", ptVar30, 5);
                                    if ((char)uVar32 == '\0') {
                                        uVar32 = (uint64_t)(int)FUN_14005C788("-IMG=", ptVar30, 5);
                                        if ((char)uVar32 != '\0') goto LAB_14008e22c;
                                        uVar32 = (uint64_t)(int)FUN_14005C788("-SKIP=", ptVar30, 6);
                                        if ((char)uVar32 == '\0') {
                                            uVar32 = (uint64_t)(int)FUN_14005C788("-iv=", ptVar30, 4);
                                            if ((char)uVar32 == '\0') {
                                                cVar12 = PECMD_MatchTokenAdvance("-iv", &cur, 3);
                                                if (cVar12 == '\0') {
                                                    cVar12 = PECMD_MatchTokenAdvance("-cdrom", &cur, 6);
                                                    if (cVar12 != '\0') {
                                                        local_368 = 8;
                                                        goto LAB_14008e216;
                                                    }
                                                    cVar12 = PECMD_MatchTokenAdvance("-floppy", &cur, 7);
                                                    if (cVar12 == '\0') {
                                                        if (*cur == L'-') {
                                                            /* dc:90432-90443 未知 '-' 选项跳过 */
                                                            WVar40 = L'-';
                                                            do {
                                                                ptVar30 = cur;
                                                                if (((8 < (uint16_t)WVar40) &&
                                                                     ((uint16_t)WVar40 < 0xe)) ||
                                                                    (WVar40 == L' ')) {
                                                                    break;
                                                                }
                                                                cur = cur + 1;
                                                                WVar40 = *cur;
                                                                ptVar30 = cur;
                                                            } while (WVar40 != L'\0');
                                                            goto LAB_14008e1ff;
                                                        }
                                                        goto LAB_14008e22c;
                                                    }
                                                    local_368 = 0x10;
                                                }
                                                else {
                                                    /* dc:90449-90453 -iv */
                                                    local_408 = uVar18 | 0x800;
                                                    local_3ac = local_3ac | 0x100000;
                                                    uVar18 = uVar18 | 0x800;
                                                }
                                            }
                                            else {
                                                /* dc:90455-90460 -iv=N (值 = p+4 WCHAR) */
                                                cur = ptVar30 + 4;
                                                PECMD_ParseUIntValue(&cur, (int *)local_420);
                                                local_408 = uVar18 | 0x800;
                                                uVar18 = local_408;
                                            }
                                        }
                                        else {
                                            /* dc:90462-90468 -SKIP=N (值 = p+6, 0x42 初值) */
                                            cur = ptVar30 + 6;
                                            PECMD_SkipLeadingControlChars((long long *)&cur);
                                            local_2d0[0] = 0x42;
                                            PECMD_ParseUIntValue(&cur, &local_2d0[0]);
                                            local_410 = (char)local_2d0[0];
                                        }
                                    }
                                    else {
                                        /* dc:90470-90498 -CHS=c:h:s (值 = p+5) */
                                        cur = ptVar30 + 5;
                                        PECMD_SkipLeadingControlChars((long long *)&cur);
                                        local_330 = cur;
                                        uVar58 = 9;
                                        WVar40 = *cur;
                                        ptVar30 = cur;
                                        while ((WVar40 != L'\0' &&
                                               ((((uint16_t)WVar40 < 9 || (0xd < (uint16_t)WVar40))) &&
                                                (WVar40 != L' ')))) {
                                            ptVar30 = ptVar30 + 1;
                                            WVar40 = *ptVar30;
                                        }
                                        ptVar44 = (int64_t)(uintptr_t)cur;
                                        cur = ptVar30;
                                        PECMD_SkipLeadingControlChars((long long *)&cur);
                                        if ((uint16_t)(*(WCHAR *)(uintptr_t)ptVar44 + 0x20) <= uVar58) {
                                            WCHAR *pval = local_330;
                                            PECMD_ParseSizeAndSkipWs((int64_t *)&pval,
                                                                     (uint64_t *)&local_3b8.QuadPart);
                                            ptVar44 = (int64_t)(uintptr_t)pval;
                                        }
                                        if (*(WCHAR *)(uintptr_t)ptVar44 == L':') {
                                            WCHAR *pval = (WCHAR *)(uintptr_t)ptVar44 + 1;
                                            PECMD_ParseSizeAndSkipWs((int64_t *)&pval,
                                                                     (uint64_t *)&local_238.QuadPart);
                                            ptVar44 = (int64_t)(uintptr_t)pval;
                                            if (*pval == L':') {
                                                pval = pval + 1;
                                                PECMD_ParseSizeAndSkipWs((int64_t *)&pval,
                                                                         (uint64_t *)&local_320.QuadPart);
                                                ptVar44 = (int64_t)(uintptr_t)pval;
                                            }
                                        }
                                    }
                                }
                                else {
                                    /* dc:90500-90505 -swap:N */
                                    cur = ptVar30 + 6;
                                    bVar72 = PECMD_ParseUIntValue(&cur, (int *)local_358) != 0;
                                    if ((int)(bVar72 ? 1 : 0) < 0) goto LAB_14008efdc;
                                    local_42f = 1;
                                }
                            }
                            else {
                                /* dc:90507-90511 -raw */
                                local_3ec = '\x01';
                                local_408 = uVar18 | 0x20;
                                uVar18 = local_408;
                            }
                        }
                        else {
                            /* dc:90513-90515 del/-del */
                            local_340 = 1;
                        }
                    }
                    else {
                        /* dc:90517-90519 -clean- */
                        local_40b = '\0';
                    }
                }
                else {
                    /* dc:90521-90523 -clean */
                    local_40b = '\x01';
                }
            }
            else {
                /* dc:90525-90527 -clear */
                local_3a0 = '\x01';
            }
            goto LAB_14008e216;
        }
    }
    else {
        /* dc:90283 asm 0x14008d838: -locku: 匹配 → local_461=3, cur+=1 WCHAR */
        local_461 = '\x03';
        cur = cur + 1;
        goto LAB_14008d878_val;
    }

    /* ================= dc:90553-90592 -lock:/-locku: 值解析 ================= */
LAB_14008d878_val:
    {
        WCHAR *pval;
        local_res20 = (uint64_t)(uintptr_t)cur;                              /* dc local_res10 */
        pval = (WCHAR *)(uintptr_t)local_res20 + 6;                          /* dc ptVar30->mbulinfo */
        WVar40 = *pval;
        cur = pval;
        if (((WVar40 == L'\0') ||
             (((8 < (uint16_t)WVar40 && ((uint16_t)WVar40 < 0xe)) || (WVar40 == L' ')))) ||
            ((ptVar30 = cur, (uint16_t)(WVar40 + 0x20) < 10))) {
            /* dc:90562-90564 数值形式: 两个表达式值 */
            PECMD_EvalExprSkipOneChar((int64_t *)&cur, (uint64_t *)&local_370);
            PECMD_EvalExprSkipOneChar((int64_t *)&cur, (uint64_t *)&local_378.QuadPart);
            ptVar30 = cur;
        }
        else {
            /* dc:90566-90579 名字形式: 截断存 local_2a8 */
            do {
                local_2a8 = (ushort *)cur;
                if (((8 < (uint16_t)WVar40) && ((uint16_t)WVar40 < 0xe)) || (WVar40 == L' ')) {
                    if (*ptVar30 != L'\0') {
                        *ptVar30 = L'\0';
                        goto LAB_14008dc92;
                    }
                    break;
                }
                ptVar30 = ptVar30 + 1;
                WVar40 = *ptVar30;
            } while (WVar40 != L'\0');
        }
    }
    goto LAB_14008e1ff;
LAB_14008dc86:
    ptVar30 = cur;
    if (*cur != L'\0') {
        *cur = L'\0';
LAB_14008dc92:
        cur = ptVar30;
        ptVar30 = cur + 1;
    }
LAB_14008e1ff:
    cur = ptVar30;
    PECMD_SkipLeadingControlChars((long long *)&cur);
LAB_14008e216:
    if (*cur == L'\0') goto LAB_14008e22c;                                   /* dc:90594 */
    goto LAB_14008d30e;

    /* ================= dc:90596-90618 哨兵归位 ================= */
LAB_14008e22c:
    if ((ptVar68 == -2) &&
        (ptVar68 = local_248, local_228 = local_248, local_248 == -9)) {      /* dc:90597 */
        ptVar68 = -1;
        local_228 = ptVar68;
    }
    local_208 = local_238;
    local_1f0 = local_320;
    if (local_248 == -9) {                                                    /* dc:90605 */
        local_218 = local_3b8;
        ptVar68 = local_228;
LAB_14008e2bd:
        local_248 = 1;
    }
    else {
        local_218 = local_3b8;
    }
    local_228 = ptVar68;
    if (ptVar68 == -9) {                                                      /* dc:90615 */
        local_228 = local_248;
    }

    /* ================= dc:90620-90713 子命令编码 ================= */
    local_438 = local_420[0];                                                 /* dc:90618 */
    local_398 = local_358[0];                                                 /* dc:90619 */
    cVar12 = PECMD_MatchTokenAdvance("list", &cur, 4);                        /* dc:90620 */
    if (cVar12 != '\0') {
        cVar12 = PECMD_MatchTokenAdvance("disk", &cur, 4);                    /* dc:90622 */
        if (cVar12 == '\0') {
            cVar12 = PECMD_MatchTokenAdvance("part", &cur, 4);                /* dc:90624 */
            if (cVar12 == '\0') {
                cVar12 = PECMD_MatchTokenAdvance("drv", &cur, 3);             /* dc:90626 */
                if (cVar12 == '\0') {
                    cVar12 = PECMD_MatchTokenAdvance("cdrom", &cur, 5);       /* dc:90628 */
                    if (cVar12 == '\0') {
                        cVar12 = PECMD_MatchTokenAdvance("floppy", &cur, 6);  /* dc:90630 */
                        if (cVar12 == '\0') {
                            cVar12 = PECMD_MatchTokenAdvance("parent", &cur, 6); /* dc:90632 */
                            if (cVar12 == '\0') {
                                cVar12 = PECMD_MatchTokenAdvance("volume", &cur, 6); /* dc:90634 */
                                if (cVar12 == '\0') {
                                    cVar12 = PECMD_MatchTokenAdvance("dep", &cur, 3); /* dc:90636 */
                                    local_3e8 = local_3e8 & 0xff;
                                    if (cVar12 != '\0') {
                                        local_3e8 = 8;
                                    }
                                }
                                else {
                                    local_3e8 = 7;                            /* volume */
                                }
                            }
                            else {
                                local_3e8 = 6;                                /* parent */
                            }
                        }
                        else {
                            local_368 = 0x10;
                            local_3e8 = 5;                                    /* floppy */
                        }
                    }
                    else {
                        local_3e8 = 4;                                        /* cdrom */
                        local_368 = 8;
                    }
                }
                else {
                    local_3e8 = 3;                                            /* drv */
                }
            }
            else {
                local_3e8 = 2;                                                /* part */
            }
        }
        else {
            local_3e8 = 1;                                                    /* disk */
        }
    }
    bVar67 = 0;                                                               /* dc:90672 */
    local_463 = 0;
    cVar12 = PECMD_MatchTokenAdvance("init", &cur, 4);                        /* dc:90674 */
    if (cVar12 != '\0') {
        local_448 = 1;
        local_res20 = (local_res20 & 0xffffffff00000000ULL) | 1;              /* dc:90677 */
    }
    cVar13 = PECMD_MatchTokenAdvance("fix", &cur, 3);                         /* dc:90679 */
    local_400 = (uint)(cVar12 != '\0');
    if (cVar13 != '\0') {
        local_400 = 0x10;
    }
    cVar12 = PECMD_MatchTokenAdvance("update", &cur, 6);                      /* dc:90684 */
    if (cVar12 == '\0') {
        cVar12 = PECMD_MatchTokenAdvance("xupdate", &cur, 7);                 /* dc:90686 */
        if (cVar12 == '\0') {
            cVar12 = PECMD_MatchTokenAdvance("hupdate", &cur, 7);             /* dc:90688 */
            if (cVar12 != '\0') {
                bVar52 = 0x10;
                goto LAB_14008e572;
            }
            cVar12 = PECMD_MatchTokenAdvance("hupdatef", &cur, 8);            /* dc:90693 */
            ptVar68 = (int64_t)(uintptr_t)cur;
            if (cVar12 == '\0') goto LAB_14008e62d;
            bVar52 = 0x18;
            goto LAB_14008e612;
        }
        local_463 = 0x20;                                                     /* xupdate */
        local_448 = 1;
        local_res20 = (local_res20 & 0xffffffff00000000ULL) | 1;
        ptVar68 = (int64_t)(uintptr_t)cur;
        bVar67 = 0x20;
    }
    else {
LAB_14008e572:
LAB_14008e612:
        local_448 = 1;
        local_res20 = (local_res20 & 0xffffffff00000000ULL) | 1;
        local_463 = bVar52;
        ptVar68 = (int64_t)(uintptr_t)cur;
        bVar67 = bVar52;
    }
LAB_14008e62d:
    for (; (WVar40 = *cur, WVar40 != L'\0' &&
           ((((uint16_t)WVar40 < 9 || (0xd < (uint16_t)WVar40))) && (WVar40 != L' ')));
        cur = cur + 1) {                                                      /* dc:90715 */
    }
    FUN_140063888(&local_260, (LPCWSTR)(uintptr_t)ptVar68,                    /* dc:90719 子命令词 */
                  ((int64_t)(uintptr_t)cur - (int64_t)(uintptr_t)ptVar68) >> 1);
    PECMD_SkipLeadingControlChars((long long *)&cur);                         /* dc:90721 */
    local_358[0] = 0;                                                         /* dc:90722 */
    if (*cur == L'*') {                                                       /* dc:90723 */
        cur = cur + 1;
        local_358[0] = 0x40000000;
    }
    WVar40 = *cur;                                                            /* dc:90727 */
    if ((WCHAR)(WVar40 | 0x20U) == 0x61) {                                    /* 'a'/'A' */
        local_380 = 0x80;
        while ((WVar40 != L'\0' &&
               ((((uint16_t)WVar40 < 9 || (0xd < (uint16_t)WVar40))) && (WVar40 != L' ')))) {
            cur = cur + 1;
            WVar40 = *cur;
        }
    }
    else if ((WVar40 == L'-') && ((WCHAR)(cur[1] | 0x20U) == 0x61)) {         /* dc:90736 "-a..." */
        local_380 = local_380 & 0xffff0000;
        WVar40 = L'-';
        do {
            if (((8 < (uint16_t)WVar40) && ((uint16_t)WVar40 < 0xe)) || (WVar40 == L' ')) break;
            cur = cur + 1;
            WVar40 = *cur;
        } while (WVar40 != L'\0');
    }
    else {
        local_358[0] = 0;
    }
    local_238.QuadPart = 0;                                                   /* dc:90749 */
    local_3b8.QuadPart = 0;
    local_278.QuadPart = 0;
    LStack_270.QuadPart = 0;
    local_230 = 0;
    local_2d0[0] = 0;
    local_1c8 = (uint64_t)local_1c8 & 0xffffffff00000000ULL;                  /* dc:90755 */
    local_318 = (void *)((uint64_t)(uintptr_t)local_318 & 0xffffffff00000000ULL); /* dc:90756 */
    local_210 = 0;
    local_220 = 0;
    local_1e8 = (int64_t)(uintptr_t)L"MS Basic Data";                         /* dc:90759 @140127aa0 */
    local_2e8 = 0x4433b9e5ebd0a0a2ULL;                                        /* dc:90760 GPT 盘 GUID */
    LStack_2e0 = 0xc79926b7b6689c87ULL;                                       /* dc:90761 */
    EnterCriticalSection((LPCRITICAL_SECTION)&g_csDisk);                      /* dc:90762 */
    cVar12 = local_464;                                                       /* dc:90763 */
    LVar69.QuadPart = 0;
    if (local_464 == '\0') {
        PECMD_ParseUIntValue(&cur, (int *)&local_45c);                        /* dc:90766 盘号 */
        if (*cur == L'-') {
            ptVar68 = (int64_t)(uintptr_t)(cur + 1);
            WVar40 = *(WCHAR *)(uintptr_t)ptVar68;
            local_3c8 = local_45c;
            if (8 < (uint16_t)(WVar40 + 0x20) && WVar40 != L'9') {
                local_39b = '\x01';
                cur = (WCHAR *)(uintptr_t)ptVar68;
            }
        }
        else {
            local_3c8 = local_45c;
        }
    }
    else {
        local_1c8 = (uint64_t)PECMD_ParseVolumeGuid((int64_t *)&cur,          /* dc:90781 */
                                                    (uint32_t *)&local_2e8, 0);
        cVar12 = local_464;
        if ((int)local_1c8 == -1) {
            LeaveCriticalSection((LPCRITICAL_SECTION)&g_csDisk);
            goto LAB_14009445a;
        }
    }
    WVar55 = L'\t';                                                           /* dc:90788 */
    WVar40 = L'-';
    if (*cur == L'+') {                                                       /* dc:90790 */
        local_3e4 = (char)*cur;
        cur = cur + 1;
    }
    if (*cur == L'-') {                                                       /* dc:90794 */
        WVar14 = L'-';
        do {
            if (((8 < (uint16_t)WVar14) && ((uint16_t)WVar14 < 0xe)) || (WVar14 == L' ')) break;
            cur = cur + 1;
            WVar14 = *cur;
        } while (WVar14 != L'\0');
        PECMD_SkipLeadingControlChars((long long *)&cur);                     /* dc:90801 */
    }
    else {
        uVar32 = PECMD_ParseSizeValue((int64_t *)&cur, (uint64_t *)&local_2f0.QuadPart); /* dc:90804 */
        WVar40 = L'-';
        local_210 = (int)uVar32;
        WVar55 = L'\t';
        local_428 = local_2f0;
    }
    bVar72 = *cur == L'@';                                                    /* dc:90810 */
    if (bVar72) {
        cur = cur + 1;
    }
    local_39e = !bVar72;
    WVar14 = *cur;                                                            /* dc:90815 */
    if (WVar40 == WVar14) {
        do {
            if ((((uint16_t)WVar55 <= (uint16_t)WVar14) && ((uint16_t)WVar14 < 0xe)) ||
                (WVar14 == L' ')) break;
            cur = cur + 1;
            WVar14 = *cur;
        } while (WVar14 != L'\0');
        PECMD_SkipLeadingControlChars((long long *)&cur);                     /* dc:90822 */
    }
    else {
        uVar32 = PECMD_ParseSizeValue((int64_t *)&cur, (uint64_t *)&local_258.QuadPart); /* dc:90825 */
        local_220 = (int)uVar32;
        local_3f8 = local_258;
    }
    if (cVar12 != '\0') {                                                     /* dc:90829 GPT 模式 */
        local_318 = (void *)(uint64_t)PECMD_ParseVolumeGuid((int64_t *)&cur,  /* dc:90830 */
                                                            (uint32_t *)&local_278, 0);
        uVar31 = PECMD_ParseSizeAndSkipWs((int64_t *)&cur, (uint64_t *)&local_3b8.QuadPart); /* dc:90831 */
        local_230 = (int)uVar31;
        if (local_43c == 0) {
            local_238.QuadPart = (local_238.QuadPart & 0xffffffffffff0000ULL) |
                                 (local_3b8.QuadPart & 0xffff);               /* dc:90834 .s 低 16 位 */
        }
        else {
            local_238.QuadPart = (local_238.QuadPart & 0xffffffffffff0000ULL) |
                                 (local_3b8.QuadPart & 0xffff);
            cVar12 = local_464;
            if ((local_3b8.QuadPart & 0x8000000000000000ULL) == 0) {
                local_238.QuadPart = local_3b8.QuadPart | 0x8000000000000000ULL;
                local_43c = local_43c | 0x20;
            }
        }
    }
    local_1a0 = 0;                                                            /* dc:90845 */
    if ((((WCHAR)*cur != L'-') && (cVar12 != '\0')) && (*cur != L'\0')) {
        local_1b0 = (int64_t)(uintptr_t)cur;
        FUN_140024C48((WCHAR **)&local_1b0, (size_t *)&local_1a0, 0);         /* dc:90849 */
        local_1e8 = local_1b0;
        ((WCHAR *)(uintptr_t)local_1b0)[local_1a0] = 0;                       /* dc:90851 mbulinfo[k-6]=p[k] */
        local_2d0[0] = 1;
    }

    /* ================= dc:90854-90958 -img=/hd 前缀处理 ================= */
    ptVar68 = (int64_t)(uintptr_t)local_260;                                  /* dc:90854 子命令词 */
    cVar12 = (char)local_3e8;
    local_40f = '\0';
    local_409 = '\0';
    local_3eb = 0;
    if ((cVar12 != '\x01') && (cVar12 < '\x04')) {
        local_434 = 0;
    }
    cur = local_260;                                                          /* dc:90862 */
    ptVar30 = local_260;
    uVar32 = (uint64_t)(int)FUN_14005C788("-img=", local_260, 5);             /* dc:90864 */
    if ((char)uVar32 == '\0') {
        uVar32 = (uint64_t)(int)FUN_14005C788("hd", (WCHAR *)(uintptr_t)ptVar68, 2); /* dc:90866 */
        if ((char)uVar32 != '\0') {
            ptVar30 = (WCHAR *)(uintptr_t)ptVar68 + 2;                        /* dc:90868 mbcodepage=p+2 */
            cur = ptVar30;
        }
        if (((0x2f < (uint16_t)*ptVar30) && ((uint16_t)*ptVar30 < 0x3a)) ||
            (*ptVar30 == L'(')) {                                             /* dc:90871 数字/( */
            PECMD_ParseHexOrDecBool((long long *)&cur, (int *)&local_434);    /* dc:90873 */
            ptVar30 = cur;
        }
        pWVar63 = (LPCWSTR)0x0;
        local_40f = (WCHAR)*(WCHAR *)(uintptr_t)ptVar68 != L'\0';             /* dc:90877 */
    }
    else {
        ptVar30 = (WCHAR *)(uintptr_t)ptVar68 + 5;                            /* dc:90880 ismbcodepage+2 = p+5 */
        if (*ptVar30 == L'*') {
            local_390 = (void *)((byte *)(uintptr_t)ptVar68 + 6);             /* dc:90882 mbulinfo = p+6 */
            cur = ptVar30;
            PECMD_ParseSizeValue((int64_t *)&local_390, (uint64_t *)&local_288.QuadPart); /* dc:90884 */
            local_3eb = 1;
            ptVar30 = (WCHAR *)(uintptr_t)local_390;
            if (*(WCHAR *)(uintptr_t)local_390 == L'*') {
                local_390 = (void *)((WCHAR *)(uintptr_t)local_390 + 1);
                PECMD_ParseSizeValue((int64_t *)&local_390, (uint64_t *)&local_280.QuadPart); /* dc:90889 */
                local_1f8 = local_280;
                local_3eb = 0x11;
                ptVar30 = (WCHAR *)(uintptr_t)local_390;
                if (*(WCHAR *)(uintptr_t)local_390 == L'*') {
                    ptVar30 = (WCHAR *)(uintptr_t)local_390 + 1;
                }
            }
            local_2a0 = local_288;
        }
        WVar40 = *ptVar30;                                                    /* dc:90899 */
        cur = ptVar30;
        if ((WVar40 != L'\\') && (local_2c0 = local_2c0 & 0xff, WVar40 != L'/')) {
            local_2c0 = 1;
        }
        while ((WVar40 != L'\0' &&
               (((((uint16_t)WVar40 < 9 || (0xd < (uint16_t)WVar40)) && (WVar40 != L' ')) &&
                (WVar40 != L'#'))))) {                                        /* dc:90904 */
            cur = cur + 1;
            WVar40 = *cur;
        }
        lVar56 = ((int64_t)(uintptr_t)cur - (int64_t)(uintptr_t)ptVar30) >> 1;
        if (0 < lVar56) {
            FUN_140063888(&local_290, (LPCWSTR)(uintptr_t)ptVar30, lVar56);   /* dc:90912 */
            local_434 = 0xfffc0000;
        }
        pWVar63 = (LPCWSTR)0x0;
        PECMD_SkipLeadingControlChars((long long *)&cur);                     /* dc:90916 */
        ptVar30 = cur;
    }
    local_350.QuadPart = local_2a0.QuadPart << 9;                             /* dc:90919 */
    if (*ptVar30 == L'#') {                                                   /* dc:90920 */
LAB_14008ed84:
        local_408 = uVar18 | 0x400;
        uVar18 = uVar18 | 0x400;
LAB_14008ed91:
        ptVar44 = (int64_t)(uintptr_t)(ptVar30 + 1);
        cur = (WCHAR *)(uintptr_t)ptVar44;
        uVar32 = (uint64_t)(int)FUN_14005C788("+L", (WCHAR *)(uintptr_t)ptVar44, 2); /* dc:90927 */
        cVar13 = (char)uVar32;
        ptVar30 = (WCHAR *)(uintptr_t)ptVar44;
        local_462 = cVar13;
        uVar32 = (uint64_t)(int)FUN_14005C788("-L", (WCHAR *)(uintptr_t)ptVar44, 2); /* dc:90931 */
        local_450 = (char)uVar32;
        local_409 = (char)(cVar13 == '\0' && local_450 == '\0');
        pWVar63 = (LPCWSTR)0x0;
        if ((cVar13 == '\0') && (*ptVar30 == L'+')) {
            ptVar30 = ptVar30 + 1;
            local_310 = 1;                                                    /* dc:90937 CONCAT71 残影→1 */
            cur = ptVar30;
        }
        if ((local_450 == '\0') &&
            (((*ptVar30 == L'@' || (*ptVar30 == L'-')) || (*ptVar30 == L'=')))) { /* dc:90940 */
            local_408 = uVar18 | 2;
            ptVar30 = ptVar30 + 1;
            cur = ptVar30;
        }
        if ((((uint16_t)*ptVar30 < 0x30) || (0x39 < (uint16_t)*ptVar30)) &&
            (*ptVar30 != L'(')) goto LAB_14008ee9b;                           /* dc:90947 */
        bVar72 = PECMD_ParseUIntValue(&cur, (int *)&local_res20);             /* dc:90949 */
        pWVar63 = (LPCWSTR)0x0;
        local_448 = (ulonglong)local_res20 & 0xffffffff;
        iVar17 = (int)local_res20;
        if ((int)(bVar72 ? 1 : 0) < 1) goto LAB_14008eea0;                    /* dc:90953 */
        local_40a = '\x01';
        bVar73 = false;
        bVar72 = (int)local_res20 == 0;
        if (!bVar72) goto LAB_14008eea3;
        local_462 = '\x01';
LAB_14008eea5:
        if (((cVar12 <= (char)pWVar63) && (local_1e0 == pWVar63)) &&
            ((LPCWSTR)(uintptr_t)local_200 == pWVar63)) {                     /* dc:90960 */
            uVar49 = (ulonglong)(uintptr_t)pWVar63 & 0xffffffff;
        }
        goto LAB_14008eeb3_after;
    }
    else {
        if (*ptVar30 == L':') {                                               /* dc:90966 */
            if (*ptVar30 == L'#') goto LAB_14008ed84;
            goto LAB_14008ed91;
        }
LAB_14008ee9b:
        iVar17 = (int)local_448;
LAB_14008eea0:
        iVar27 = (int)(int64_t)(uintptr_t)pWVar63;                            /* dc:90973 */
        bVar73 = SBORROW4(iVar17, iVar27);
        local_res20 = (uint64_t)(uint32_t)(iVar17 - iVar27);
        bVar72 = iVar17 == iVar27;
LAB_14008eea3:
        if (bVar72 || bVar73 != (int)local_res20 < 0) goto LAB_14008eea5;
    }
LAB_14008eeb3_after:

    /* ================= dc:90980-91157 路径构造/锁定/MBR 保护 ================= */
    ptVar30 = local_290;                                                      /* dc:90980 */
    local_360 = (WCHAR *)(int64_t)(int)local_434;                             /* dc:90981 盘号值载体 */
    ((uint16_t *)local_148)[0] = (uint16_t)(uint64_t)(uintptr_t)pWVar63;      /* dc:90982 SUB82 低16 */
    if (((local_461 != (char)pWVar63) && ((int)uVar49 == (int)pWVar63)) &&
        (*(WCHAR *)(uintptr_t)ptVar68 == ((uint16_t *)local_148)[0])) {       /* dc:90983 */
        local_434 = 0xfffc0000;
        local_360 = (WCHAR *)0xfffffffffffc0000ULL;
    }
    ptVar44 = (int64_t)(uintptr_t)local_360;                                  /* dc:90988 */
    ptVar53 = (WCHAR *)local_148;                                             /* dc:90989 */
    if (*local_290 != ((uint16_t *)local_148)[0]) {                           /* dc:90990 */
        ptVar53 = local_290;
    }
    local_res20 = (uint64_t)(uintptr_t)local_360;                             /* dc:90993 */
    local_330 = ptVar53;                                                      /* dc:90994 */
    wsprintfW((LPWSTR)local_148, L"\\.\PhysicalDrive%d", local_434);       /* dc:90995 (r8d=[rsp+0x74] asm 0x14008ef0f) */
    if (((local_40c < '\x01') && ((ulonglong)local_2a0.QuadPart == 0)) &&
        (iVar17 = StrCmpNIW(L"\\.\PhysicalDrive", (LPCWSTR)(uintptr_t)ptVar53, 0x11),
         iVar17 == 0)) {                                                      /* dc:90996 */
        local_2f0.QuadPart = (int64_t)(uintptr_t)(ptVar53 + 17);              /* dc:90998 asm lea rax,[rsi+0x22] */
        PECMD_ParseNumSkipWs((WCHAR **)&local_2f0, &local_res20);             /* dc:90999 */
        local_360 = (WCHAR *)(uintptr_t)local_res20;                          /* dc:91000 */
        ptVar44 = (int64_t)(uintptr_t)local_res20;
    }
    if ((((bVar67 != 0) || (local_468 != 0)) || (local_465 != 0)) &&
        (((int)local_434 < 0 && ((int64_t)(uintptr_t)ptVar44 < 0)))) {        /* dc:91003 */
LAB_14008efd3:
        LeaveCriticalSection((LPCRITICAL_SECTION)&g_csDisk);
LAB_14008efdc:
        LVar69.QuadPart = -0x7ff8ffa9;
        goto LAB_14009445a;
    }
    LVar69.QuadPart = -0x7ff8ffa9;                                            /* dc:91011 */
    uVar46 = 0;
    local_280.QuadPart = -0x7ff8ffa9;
    local_3a8 = (HANDLE)0x0;
    local_404 = -1;
    local_2b0.QuadPart = 0;
    local_res20 = local_res20 & 0xffffffffffffff00ULL;                        /* dc:91017 */
    if ((local_461 != '\0') &&
        (((-1 < (longlong)(int64_t)(uintptr_t)local_360) ||
          ((longlong)local_370 < 0)) || (local_2a8 != (ushort *)0x0))) {      /* dc:91018 */
        puVar59 = local_2a8;
        if ((local_2a8 != (ushort *)0x0) && (*local_2a8 == L'*')) {           /* dc:91021 */
            local_2b0.QuadPart = (int64_t)PECMD_LockVolumeByDevice(           /* dc:91022 */
                (int)(int64_t)(uintptr_t)local_360, ((int)local_461 & 2U) >> 1);
            puVar59 = (ushort *)0x0;
            g_flagD6F5 = g_flagD6F5 | 0x20;                                   /* dc:91024 */
            uVar46 = 0x20;
            local_res20 = (local_res20 & 0xffffffffffffff00ULL) | 0x20;
            local_280.QuadPart = local_2b0.QuadPart;
        }
        if (((-1 < (longlong)(int64_t)(uintptr_t)local_360) ||
             ((longlong)local_370 < 0)) || (puVar59 != (ushort *)0x0)) {      /* dc:91029 */
            local_2b0.QuadPart = (int64_t)PECMD_OpenLockVolume(               /* dc:91030 */
                (int)(int64_t)(uintptr_t)local_360, (ulonglong)local_370,
                local_378.QuadPart, ((int)local_461 & 2U) >> 1, (WCHAR *)puVar59);
            local_280 = local_2b0;
            if (local_370 == -1) {                                            /* dc:91034 */
                g_flagD6F5 = 0;
            }
            else {
                g_flagD6F5 = g_flagD6F5 | 0x10;
                local_res20 = ((local_res20 & 0xffffffffffffff00ULL) | uVar46) | 0x10;
            }
        }
    }
    uVar4 = (uint32_t)local_2e8;                                              /* dc:91043 GUID 半槽→寄存器 */
    uVar5 = (uint32_t)(local_2e8 >> 32);
    uVar6 = (uint32_t)LStack_2e0;
    uVar7 = (uint32_t)(LStack_2e0 >> 32);
    uVar8 = (uint32_t)local_278.QuadPart;
    uVar9 = (uint32_t)(local_278.QuadPart >> 32);
    uVar10 = (uint32_t)LStack_270.QuadPart;
    uVar11 = (uint32_t)(LStack_270.QuadPart >> 32);
    if ((local_42f == 0) || (local_464 != '\0')) {                            /* dc:91051 */
        local_42c = 0;
        pvVar47 = (HANDLE)0x0;
        ptVar44 = (int64_t)(uintptr_t)ptVar53;
        iVar17 = local_404;
        if (local_463 != 0x10) goto LAB_14008f58c;                            /* dc:91056 */
        uVar18 = 0;
        bVar52 = local_465;
        if ((local_465 & 0x20) != 0) goto LAB_14008f48e;
        goto LAB_14008f58c;
    }
    if (((-1 < (int)local_434) || (*ptVar30 != L'\0')) &&
        ((((int)local_398 < 1 || (local_398 != (uint)local_448)) &&
         (((int)(uint)local_448 < 5 && ('\x01' < (char)local_45f)))))) {      /* dc:91062 */
        PECMD_OpenFileHandle(&local_3a8, (LPCWSTR)(uintptr_t)ptVar53, 0xc0000000, 3,
                             (LPSECURITY_ATTRIBUTES)0x0, 3, 0x20000000, (HANDLE)0x0); /* dc:91065 */
        pvVar47 = local_3a8;
        if (local_3a8 == (HANDLE)0x0) {
            PECMD_ClearFlagAndError((byte *)&local_res20);                    /* dc:91069 */
        }
        else {
            local_404 = PECMD_QueryDiskGeometry(local_3a8, (uint64_t *)0x0, 1, 0x800); /* dc:91072 */
            uVar18 = FUN_14005F96C(pvVar47, 0x200);                           /* dc:91073 */
            LVar33.QuadPart = 0x200;
            if (0x1ff < (int)uVar18) {
                LVar33.HighPart = 0;
                LVar33.LowPart = uVar18;
            }
            DVar23 = LVar33.LowPart;
            local_3e0 = (local_3e0 & 0xffffffff00000000ULL) | DVar23;         /* dc:91080 */
            FUN_1400633A8(&local_390, (int64_t)(DVar23 * 3));                 /* dc:91081 缓冲分配 */
            LVar69.QuadPart =                                                /* dc:91082 */
                 (ulonglong)~(DVar23 - 1) &
                 (ulonglong)((int64_t)(uintptr_t)local_390 + (int64_t)(LVar33.QuadPart + -0xc));
            lpBuffer = (byte *)(LVar69.QuadPart + LVar33.QuadPart);
            local_3b8.QuadPart = LVar69.QuadPart;
            local_2f0 = LVar33;
            BVar19 = ReadFile(pvVar47, lpBuffer, DVar23, &local_3f0, NULL); /* dc:91088 */
            if (BVar19 == 0) {
                LVar69.LowPart = GetLastError();
                LVar69.HighPart = 0;
                if (LVar69.LowPart == 0) {
                    LVar69.QuadPart = 1;
                }
                PECMD_FreeStrBuf(&local_390);
                PECMD_ClearFlagAndError((byte *)&local_res20);
                if (pvVar47 != (HANDLE)-1) {
                    CloseHandle(pvVar47);
                }
                goto LAB_14008f546;
            }
            PECMD_MemMoveForward((byte *)(uintptr_t)LVar69.QuadPart, lpBuffer, 0x200); /* dc:91102 */
            local_42f = local_42f | 2;
            local_398 = local_398 - 1;
            uVar18 = (int)local_448 - 1;
            local_448 = (ulonglong)uVar18;
            lpBuffer[0x1fe] = 0x55;                                           /* dc:91107 签名回填 */
            iVar17 = local_398 * 0x10;
            lpBuffer[0x1ff] = 0xaa;
            lVar56 = (longlong)(int)(uVar18 * 0x10);
            PECMD_MemMoveForward(lpBuffer + lVar56 + 0x1be,                   /* dc:91111 条目交换 */
                                 (byte *)((int64_t)(local_3b8.QuadPart + 0x1be) + (int64_t)iVar17),
                                 0x10);
            PECMD_MemMoveForward(lpBuffer + (int64_t)iVar17 + 0x1be,
                                 (byte *)((int64_t)(local_3b8.QuadPart + 0x1be) + lVar56), 0x10);
            LVar33 = PECMD_SetFilePointer(pvVar47, local_350, 0);             /* dc:91115 */
            LVar69.QuadPart = 0;
            if (LVar33.LowPart == 0) {
                local_2b0.QuadPart = 0;
                iVar17 = PECMD_AnsiStrNCompare((char *)(uintptr_t)local_3b8.QuadPart,
                                               (int64_t)(uintptr_t)lpBuffer,
                                               (int64_t)(int)local_2f0.QuadPart); /* dc:91119 */
                if (iVar17 != 0) {
                    BVar19 = WriteFile(pvVar47, lpBuffer, (DWORD)local_3e0, &local_3f0,
                                       NULL);                    /* dc:91121 */
                    if (BVar19 == 0) {
                        LVar69.LowPart = GetLastError();
                        LVar69.HighPart = 0;
                        local_2b0 = LVar69;
                        if (LVar69.LowPart == 0) {
                            LVar69.QuadPart = 1;
                            local_2b0.QuadPart = LVar69.QuadPart;
                        }
                    }
                    PECMD_FlushFileThrice(pvVar47);                           /* dc:91131 */
                }
                bVar52 = local_465;
                local_468 = local_468 | local_465 | local_467;                /* dc:91134 */
                if ((local_465 != 0 || local_468 != 0) || local_467 != 0) {
                    PECMD_FreeStrBuf(&local_390);
                    do {
                        uVar49 = 0;
                        uVar18 = local_42c;
                        iVar17 = local_404;
LAB_14008f48e:
                        ptVar44 = (int64_t)(uintptr_t)local_330;              /* dc:91142 */
                        bVar67 = 0;
                        if (((local_461 == '\0') || ((int)uVar49 == 0)) &&
                            (bVar67 = 0x40, local_463 == 0x10)) {             /* dc:91144 */
                            bVar67 = 0x60;
                        }
                        if (((longlong)(int64_t)(uintptr_t)local_360 < 0) ||
                            (((iVar17 = local_404, local_463 != 0x20 && (local_463 != 0x10)) ||
                              (local_404 != 0xb)) &&
                             ((((local_468 & 0x20) == 0 &&
                                ((local_468 == 0 || (local_404 != 0xb)))) &&
                               (((bVar52 & 0x20) == 0 &&
                                 ((bVar52 == 0 || (local_404 != 0xb))))))))) { /* dc:91147 */
LAB_14009279a:
                            if (((char)local_2c0 < '\x01') &&
                               (((((local_467 != 0 || (local_463 == 1)) || (local_463 == 0x20)) ||
                                  (local_468 != 0)) &&
                                 ((bVar67 = bVar67 | 2, local_463 == 1 || (local_463 == 0x20)))))) {
                                uVar18 = uVar18 | 4;                          /* dc:91156 */
                            }
                        }
                        else if ((char)local_2c0 < '\x01') {                  /* dc:91158 */
                            bVar67 = bVar67 | 0x11;
                            goto LAB_14009279a;
                        }
                        if ((((bVar52 & 0x20) != 0) ||
                             ((bVar52 != 0 && (iVar17 == 0xb)))) ||
                            (((local_463 & 0x10) != 0 && (iVar17 != 0xc)))) { /* dc:91164 */
                            bVar67 = bVar67 | 8;
                        }
                        local_42c = uVar18 | (int)(char)bVar67;               /* dc:91168 */
                        PECMD_CloseDeviceHandle(&local_3a8,               /* dc:91169 */
                                                 (int)(int64_t)(uintptr_t)local_360,
                                                 (byte)local_42c,
                                                 (byte *)&local_res20,
                                                 (uint64_t *)&local_2b0.QuadPart,
                                                 (LPCWSTR)(uintptr_t)local_330);
                        pvVar47 = local_3a8;
                        iVar27 = 0;
                        if (-1 < iVar27) {
                            PECMD_ClearFlagAndError((byte *)&local_res20);    /* dc:91172 */
                            pvVar47 = local_3a8;
joined_r0x000140092fff:
                            LVar69.QuadPart = (int64_t)iVar27;         /* dc:91176 */
                            if (pvVar47 != (HANDLE)0x0) {
joined_r0x000140093008:
                                if (pvVar47 != (HANDLE)-1) {
                                    CloseHandle(pvVar47);                     /* dc:91179 */
                                }
                            }
                            goto LAB_14008f546;
                        }
LAB_14008f58c:                                                              /* dc:91186 */
                        uVar18 = local_368;                             /* dc:91185 */
                        iVar27 = (int)uVar49;
                        if ((local_461 != '\0') && (iVar27 == 0))
                            goto code_r0x00014008f598;
                        LVar69.QuadPart = 0;
                        uVar20 = 0;
                        if (local_1e0 != (int64_t)0) {                  /* dc:91192 */
                            if (((int)local_434 < 0) && (*ptVar30 == L'\0')) { /* dc:91193 */
LAB_140093fba:
                                PECMD_ClearFlagAndError((byte *)&local_res20);
joined_r0x000140093a51:
                                if ((pvVar47 != (HANDLE)0x0) && (pvVar47 != (HANDLE)-1)) {
                                    CloseHandle(pvVar47);
                                }
                                goto LAB_14008efd3;
                            }                                         /* dc:91201 闭内层 if(91193) */
                        uVar18 = 0x1b8;                                                       /* dc:91199 */
        local_438 = 0x1b8;
        local_278.QuadPart = LVar69.QuadPart;
        LStack_270.QuadPart = LVar69.QuadPart;
        local_268.QuadPart = LVar69.QuadPart;
        if ((*(WCHAR *)(uintptr_t)local_1e0 == L'\0') ||
            ((pWVar39 = (LPCWSTR)(uintptr_t)local_1e0, uVar18 = 0x1b8,
              uVar32 = (uint64_t)PECMD_AsciiWideICmp("nt6", (LPCWSTR)(uintptr_t)local_1e0),
              (char)uVar32 != '\0') ||
             (uVar32 = (uint64_t)PECMD_AsciiWideICmp("win", pWVar39), (char)uVar32 != '\0'))) { /* dc:91204 */
            LVar69.QuadPart = (int64_t)(uintptr_t)g_bootNt6;                  /* dc:91210 DAT_14013a680 */
            local_438 = uVar18;
LAB_140094184:
            if ((local_438 == 0) || ((ulonglong)LVar69.QuadPart == 0)) {      /* dc:91213 */
                PECMD_FreeStrBuf(&local_278.QuadPart);
                PECMD_ClearFlagAndError((byte *)&local_res20);
                goto joined_r0x000140094436;
            }
        }
        else {
            uVar32 = (uint64_t)PECMD_AsciiWideICmp("dos", pWVar39);           /* dc:91221 */
            if ((char)uVar32 == '\0') {
                uVar32 = (uint64_t)PECMD_AsciiWideICmp("nt5", pWVar39);       /* dc:91223 */
                if ((char)uVar32 == '\0') {
                    local_318 = (void *)-1;                                   /* dc:91225 */
                    local_438 = uVar20;
                    if (*pWVar39 == L'*') {                                   /* dc:91227 */
                        local_240 = pWVar39 + 1;
                        PECMD_ParseSizeAndSkipWs((int64_t *)&local_240,
                                                 (uint64_t *)&local_318);
                        pWVar39 = local_240;
                        if (*local_240 == L'*') {
                            pWVar39 = local_240 + 1;
                        }
                    }
                    uVar49 = (ulonglong)(uintptr_t)local_318;                 /* dc:91235 */
                    if (*pWVar39 == L'#') {                                   /* dc:91236 资源形式 */
                        FUN_1400702B0((WCHAR **)&local_240, pWVar39);
                        LVar69.QuadPart = FUN_14001EA18((HMODULE)0x0, local_240,
                                                        L"EXEDATA",
                                                        (uint64_t *)&local_278.QuadPart,
                                                        (uint *)0x0);         /* dc:91238 */
                        PECMD_FreeStrBuf(&local_240);
                        local_438 = (DWORD)(local_278.QuadPart >> 32);        /* dc:91241 asm [rsp+0x238] */
                    }
                    else {
                        LVar69.QuadPart = (int64_t)(uintptr_t)                /* dc:91242 */
                            PECMD_ReadFileToBuffer(pWVar39, (LPVOID)0x0, &local_438);
                        local_278.QuadPart = LVar69.QuadPart;
                    }
                    if ((-1 < (longlong)(int64_t)(uintptr_t)local_318) &&
                        ((longlong)(int64_t)(uintptr_t)local_318 <
                         (longlong)(ulonglong)local_438)) {                   /* dc:91245 */
                        local_438 = (uint)(uintptr_t)local_318;
                    }
                    goto LAB_140094184;
                }
                LVar69.QuadPart = (int64_t)(uintptr_t)g_bootNt5;              /* dc:91254 DAT_14013a500 */
                local_438 = 0x180;
            }
            else {
                LVar69.QuadPart = (int64_t)(uintptr_t)g_bootDos;              /* dc:91258 DAT_14013a370 */
                local_438 = 400;
            }
        }
        /* dc:91259-91320 打开盘 + 扇区对齐 + 模板写首扇区 */
        PECMD_OpenFileHandle(&local_3a8, (LPCWSTR)(uintptr_t)ptVar44, 0xc0000000, 3,
                             (LPSECURITY_ATTRIBUTES)0x0, 3, 0x20000000, (HANDLE)0x0); /* dc:91260 */
        pvVar47 = local_3a8;
        if (local_3a8 == (HANDLE)0x0) {
            PECMD_FreeStrBuf(&local_278.QuadPart);
            PECMD_ClearFlagAndError((byte *)&local_res20);
            goto LAB_14009444b;                                               /* dc:91266 */
        }
        uVar18 = FUN_14005F96C(local_3a8, 0x200);                             /* dc:91269 扇区大小 */
        uVar49 = 0x200;
        if (0x1ff < (int)uVar18) {
            uVar49 = (ulonglong)uVar18;
        }
        iVar17 = (int)uVar49;
        uVar18 = iVar17 - 1;
        uVar20 = (uint)((uVar18 + local_438) & (~uVar18));                    /* dc:91275 对齐 */
        uVar31 = uVar49;
        if (iVar17 <= (int)uVar20) {
            uVar31 = (ulonglong)uVar20;
        }
        FUN_1400633A8(&local_318, (int64_t)(uint)(iVar17 + (int)uVar31 * 2)); /* dc:91279 */
        LVar33 = local_350;
        LVar35.QuadPart = 0;
        pcVar66 = (char *)((uVar49 + (ulonglong)(uintptr_t)local_318) &
                           (ulonglong)~(ulonglong)uVar18);                    /* dc:91283 对齐基址 */
        pcVar57 = pcVar66 + (int)uVar31;
        if ((ulonglong)local_2a0.QuadPart != 0) {
            LVar35 = PECMD_SetFilePointer(pvVar47, local_350, 0);             /* dc:91286 */
        }
        if ((LVar35.QuadPart == LVar33.QuadPart) &&
            (BVar19 = ReadFile(pvVar47, pcVar57, uVar20, &local_3f0, NULL),
             BVar19 != 0)) {                                                  /* dc:91289 */
            PECMD_MemMoveForward((byte *)pcVar66, (byte *)pcVar57, (int)uVar20); /* dc:91291 旧盘快照 */
            uVar18 = local_438;
            if (0x1b8 < (int)local_438) {
                uVar18 = 0x1b8;                                               /* dc:91293 模板截断 0x1b8 */
            }
            PECMD_MemMoveForward((byte *)pcVar57, (byte *)(uintptr_t)LVar69.QuadPart, /* dc:91295 */
                                 (int)uVar18);
            pcVar57[0x1fe] = 'U';                                             /* dc:91296 'U'(0x55) */
            pcVar57[0x1ff] = (char)0xaa;                                      /* dc:91297 0xAA */
            if (0x200 < local_438) {
                PECMD_MemMoveForward((byte *)(pcVar57 + 0x200),               /* dc:91299 超扇区尾部 */
                                     (byte *)(LVar69.QuadPart + 0x200),
                                     (int)(local_438 - 0x200));
            }
            iVar17 = PECMD_AnsiStrNCompare(pcVar66, (int64_t)(uintptr_t)pcVar57, /* dc:91302 */
                                           (int64_t)(int)uVar20);
            if (iVar17 == 0) {
LAB_140094376:                                                                /* dc:91307 */
                PECMD_FreeStrBuf(&local_318);
                PECMD_FreeStrBuf(&local_278.QuadPart);
LAB_140094390:                                                                /* dc:91310 */
                LVar69.QuadPart = 0;
                PECMD_ClearFlagAndError((byte *)&local_res20);
                if (pvVar47 != (HANDLE)-1) {
                    CloseHandle(pvVar47);
                }
LAB_1400943b2:                                                                /* dc:91316 */
                LeaveCriticalSection((LPCRITICAL_SECTION)&g_csDisk);
                goto LAB_14009445a;
            }
            LVar69 = PECMD_SetFilePointer(pvVar47, LVar33, 0);                /* dc:91320 */
            if (LVar69.QuadPart == LVar33.QuadPart) {
                BVar19 = WriteFile(pvVar47, pcVar57, uVar20, &local_3f0, NULL); /* dc:91322 */
                PECMD_FlushFileThrice(pvVar47);
                if (BVar19 != 0) goto LAB_140094376;
            }
        }
        LVar69.LowPart = GetLastError();                                      /* dc:91315 */
        LVar69.HighPart = 0;
        if (LVar69.LowPart == 0) {
            LVar69.QuadPart = 1;
        }
        PECMD_FreeStrBuf(&local_318);
        PECMD_FreeStrBuf(&local_278.QuadPart);
LAB_1400943f2:
        PECMD_ClearFlagAndError((byte *)&local_res20);                        /* dc:91335 */
        if (pvVar47 != (HANDLE)-1) {
            CloseHandle(pvVar47);
        }
        goto LAB_14008f546;
    }
    /* ================= dc:91329-91510 DescribePartitionInfo + INDATA ================= */
    iVar17 = (int)local_448;                                                  /* dc:91329 */
    if ((void *)(uintptr_t)local_200 != (void *)0x0) {                        /* dc:91330 标签串 */
        if ((iVar17 < 1) || (((int)local_434 < 0) && (*ptVar30 == L'\0')))   /* dc:91331 */
            goto LAB_140093fba;                                               /* dc:91332 */
        local_3e0 = (local_3e0 & 0xffffffff00000000ULL) | 0xffffffff;         /* dc:91334 */
        local_348.QuadPart = LVar69.QuadPart;                                 /* dc:91335 */
        if ((int)local_434 < 0) {                                             /* dc:91336 */
            PECMD_ClearFlagAndError((byte *)&local_res20);
            goto joined_r0x000140093a51;
        }
        PECMD_DescribePartitionInfo(&local_418,                               /* dc:91340 */
                                    (LPCWSTR)(int64_t)(int)local_434,
                                    (uint64_t)(longlong)iVar17,
                                    (ulonglong)local_408, &local_348,
                                    (uint *)&local_3e0, local_410);
        LVar33 = local_348;
        if (0 < (int)(DWORD)local_3e0) {                                      /* dc:91344 */
            local_440 = 0x200;                                                /* dc:91345 */
            local_2e8 = (uint64_t)LVar69.QuadPart;
            LStack_2e0 = (uint64_t)LVar69.QuadPart;
            local_2d8.QuadPart = LVar69.QuadPart;
            if ((*(short *)(uintptr_t)local_200 == 0) ||
                (LVar69.QuadPart = (int64_t)(uintptr_t)local_200,
                 uVar32 = (uint64_t)PECMD_AsciiWideICmp("nt6",
                            (LPCWSTR)(uintptr_t)local_200), (char)uVar32 != '\0') ||
                (uVar32 = (uint64_t)PECMD_AsciiWideICmp("win",
                            (LPCWSTR)(uintptr_t)LVar69.QuadPart), (char)uVar32 != '\0')) { /* dc:91349 */
                pWVar39 = (LPCWSTR)(uintptr_t)9;                              /* dc:91353 INDATA ID 9 */
LAB_140093cc3:
                LVar69.QuadPart = FUN_14001EA18((HMODULE)0x0, pWVar39, L"INDATA", /* dc:91355 */
                                                (uint64_t *)&local_2e8, (uint *)0x0);
                bVar72 = local_440 != 0;
                local_440 = 0;
                if (bVar72) {
                    local_440 = (DWORD)(LStack_2e0 & 0xffffffff);             /* dc:91360 尺寸@槽+8 */
                }
LAB_140093cf9:
                if ((local_440 != 0) && ((ulonglong)LVar69.QuadPart != 0)) {  /* dc:91363 */
                    PECMD_OpenFileHandle(&local_3a8, (LPCWSTR)(uintptr_t)ptVar44, 0xc0000000, 3,
                                         (LPSECURITY_ATTRIBUTES)0x0, 3, 0x20000000, /* dc:91364 */
                                         (HANDLE)0x0);
                    pvVar47 = local_3a8;
                    if (local_3a8 == (HANDLE)0x0) {
                        PECMD_FreeStrBuf(&local_2e8);
                        PECMD_ClearFlagAndError((byte *)&local_res20);
                    }
                    else {
                        LVar33.QuadPart = local_2a0.QuadPart * 0x200 + LVar33.QuadPart; /* dc:91372 */
                        local_2a0.QuadPart = LVar33.QuadPart;
                        LVar35 = PECMD_SetFilePointer(local_3a8, LVar33, 0);  /* dc:91374 */
                        if (LVar35.QuadPart == LVar33.QuadPart) {
                            uVar18 = FUN_14005F96C(pvVar47, 0x200);           /* dc:91376 */
                            uVar49 = 0x200;
                            if (0x1ff < (int)uVar18) {
                                uVar49 = (ulonglong)uVar18;
                            }
                            uVar18 = (int)uVar49 - 1;
                            uVar20 = (uint)((uVar18 + local_440) & (~uVar18)); /* dc:91382 */
                            FUN_1400633A8(&local_3b8.QuadPart,                /* dc:91383 */
                                          (int64_t)((int)uVar49 + (int)uVar20 * 2));
                            pcVar66 = (char *)(((ulonglong)uVar49 +
                                                (ulonglong)(uintptr_t)local_3b8.QuadPart) &
                                               (ulonglong)~(ulonglong)uVar18);
                            pcVar57 = pcVar66 + (int)uVar20;
                            BVar19 = ReadFile(pvVar47, pcVar57, uVar20, &local_3f0, /* dc:91387 */
                                              NULL);
                            if (BVar19 == 0) {
                                PECMD_FreeStrBuf(&local_3b8.QuadPart);
                                PECMD_FreeStrBuf(&local_2e8);
                                PECMD_ClearFlagAndError((byte *)&local_res20);
                                if (pvVar47 != (HANDLE)-1) {
                                    CloseHandle(pvVar47);                     /* dc:91392 */
                                }
                            }
                            else {
                                PECMD_MemMoveForward((byte *)pcVar66, (byte *)pcVar57, /* dc:91397 */
                                                     (int)uVar20);
                                LVar33 = local_2a0;
                                LVar35 = PECMD_SetFilePointer(pvVar47, local_2a0, 0); /* dc:91399 */
                                uVar18 = local_440;
                                if (LVar35.QuadPart == LVar33.QuadPart) {
                                    PECMD_MemMoveForward((byte *)pcVar57,     /* dc:91402 模板头 3B */
                                                         (byte *)(uintptr_t)LVar69.QuadPart, 3);
                                    if (0x1fe < (int)uVar18) {
                                        uVar18 = 0x1fe;
                                    }
                                    PECMD_MemMoveForward((byte *)(pcVar57 + 0x1e), /* dc:91406 模板 0x1e.. */
                                                         (byte *)(LVar69.QuadPart + 0x1e),
                                                         (int)(uVar18 - 0x1e));
                                    pcVar57[0x1fe] = 'U';
                                    pcVar57[0x1ff] = (char)0xaa;
                                    if (0x200 < local_440) {
                                        PECMD_MemMoveForward((byte *)(pcVar57 + 0x200), /* dc:91410 */
                                                             (byte *)(LVar69.QuadPart + 0x200),
                                                             (int)(local_440 - 0x200));
                                    }
                                    iVar17 = PECMD_AnsiStrNCompare(pcVar66,   /* dc:91414 */
                                                                   (int64_t)(uintptr_t)pcVar57,
                                                                   (int64_t)(int)uVar20);
                                    if (iVar17 != 0) {
                                        BVar19 = WriteFile(pvVar47, pcVar57, uVar20, &local_3f0, /* dc:91416 */
                                                           NULL);
                                        local_240 = (LPCWSTR)(uint64_t)(uint)BVar19; /* dc:91418 */
                                        PECMD_FlushFileThrice(pvVar47);
                                    }
                                    LVar69.HighPart = 0;                      /* dc:91421 失败位 */
                                    LVar69.LowPart = ((int)(uintptr_t)local_240 == 0);
                                    PECMD_FreeStrBuf(&local_3b8.QuadPart);
                                    PECMD_FreeStrBuf(&local_2e8);
                                    PECMD_ClearFlagAndError((byte *)&local_res20);
                                    goto joined_r0x000140093008;
                                }
                                PECMD_FreeStrBuf(&local_3b8.QuadPart);
                                PECMD_FreeStrBuf(&local_2e8);
LAB_140092a2b:
                                PECMD_ClearFlagAndError((byte *)&local_res20); /* dc:91431 */
                                if (pvVar47 != (HANDLE)-1) {
                                    CloseHandle(pvVar47);
                                }
                            }
                        }
                        else {
                            PECMD_FreeStrBuf(&local_2e8);                     /* dc:91438 */
                            PECMD_ClearFlagAndError((byte *)&local_res20);
                            if (pvVar47 != (HANDLE)-1) {
                                CloseHandle(pvVar47);
                            }
                        }
                    }
                    goto LAB_14009444b;                                       /* dc:91445 */
                }                                                            /* dc:91458 闭 if(91375) */
            }                                                                /* dc:91459 闭 if(91363) */
            else {
                uVar32 = (uint64_t)PECMD_AsciiWideICmp("nt5",                  /* dc:91449 */
                            (LPCWSTR)(uintptr_t)LVar69.QuadPart);
                if ((char)uVar32 != '\0') {
                    pWVar39 = (LPCWSTR)(uintptr_t)8;                           /* INDATA ID 8 */
                    goto LAB_140093cc3;
                }
                uVar32 = (uint64_t)PECMD_AsciiWideICmp("nt6fat",               /* dc:91454 */
                            (LPCWSTR)(uintptr_t)LVar69.QuadPart);
                if (((char)uVar32 != '\0') ||
                    (uVar32 = (uint64_t)PECMD_AsciiWideICmp("winfat",
                            (LPCWSTR)(uintptr_t)LVar69.QuadPart), (char)uVar32 != '\0')) {
                    pWVar39 = (LPCWSTR)(uintptr_t)7;                           /* INDATA ID 7 */
                    goto LAB_140093cc3;
                }
                uVar32 = (uint64_t)PECMD_AsciiWideICmp("dos",                  /* dc:91460 */
                            (LPCWSTR)(uintptr_t)LVar69.QuadPart);
                if ((char)uVar32 != '\0') {
                    pWVar39 = (LPCWSTR)(uintptr_t)6;                           /* INDATA ID 6 */
                    goto LAB_140093cc3;
                }
                uVar32 = (uint64_t)PECMD_AsciiWideICmp("nt5fat",               /* dc:91465 */
                            (LPCWSTR)(uintptr_t)LVar69.QuadPart);
                if ((char)uVar32 != '\0') {
                    pWVar39 = (LPCWSTR)(uintptr_t)5;                           /* INDATA ID 5 */
                    goto LAB_140093cc3;
                }
                local_318 = (void *)-1;                                        /* dc:91470 */
                local_440 = uVar20;
                if (*(short *)(uintptr_t)LVar69.QuadPart == 0x2a) {            /* dc:91472 '*' 长度前缀 */
                    local_348.QuadPart = LVar69.QuadPart + 2;
                    PECMD_ParseSizeAndSkipWs((int64_t *)&local_348.QuadPart,
                                             (uint64_t *)&local_318);
                    LVar69.QuadPart = (int64_t)(uintptr_t)local_348.QuadPart;
                    if (*(short *)(uintptr_t)local_348.QuadPart == 0x2a) {
                        LVar69.QuadPart = local_348.QuadPart + 2;              /* dc:91477 */
                    }
                }
                uVar49 = (ulonglong)(uintptr_t)local_318;                      /* dc:91480 */
                if (*(short *)(uintptr_t)LVar69.QuadPart == 0x23) {            /* dc:91481 '#' 资源 */
                    FUN_1400702B0((WCHAR **)&local_348.QuadPart,
                                  (LPCWSTR)(uintptr_t)LVar69.QuadPart);
                    LVar69.QuadPart = FUN_14001EA18((HMODULE)0x0,
                                                    (LPCWSTR)(uintptr_t)local_348.QuadPart,
                                                    L"EXEDATA",
                                                    (uint64_t *)&local_2e8, (uint *)0x0); /* dc:91484 */
                    PECMD_FreeStrBuf(&local_348.QuadPart);
                    local_440 = (DWORD)(LStack_2e0 & 0xffffffff);
                }
                else {
                    LVar69.QuadPart = (int64_t)(uintptr_t)                     /* dc:91490 */
                        PECMD_ReadFileToBuffer((LPCWSTR)(uintptr_t)LVar69.QuadPart,
                                               (LPVOID)0x0, &local_440);
                    local_2e8 = (uint64_t)LVar69.QuadPart;
                }
                if (-1 < (longlong)(int64_t)(uintptr_t)local_318) {            /* dc:91493 '*' 截断 */
                    uVar31 = (ulonglong)local_440;
                    if ((longlong)(int64_t)(uintptr_t)local_318 <
                        (longlong)(ulonglong)local_440) {
                        uVar31 = (ulonglong)(uintptr_t)local_318;
                    }
                    local_440 = (DWORD)uVar31;
                }
                if ((ulonglong)LVar69.QuadPart != 0) goto LAB_140093cf9;       /* dc:91500 */
            }
            PECMD_FreeStrBuf(&local_2e8);                                      /* dc:91502 */
        }
        PECMD_ClearFlagAndError((byte *)&local_res20);                         /* dc:91504 */
joined_r0x000140094436:
        if ((pvVar47 != (HANDLE)0x0) && (pvVar47 != (HANDLE)-1)) {
            CloseHandle(pvVar47);
        }
        goto LAB_14009444b;
    }
    /* ================= dc:91511-91565 parent 枚举 (list parent/cdrom/floppy) ===== */
    cVar12 = (char)local_3e8;                                                 /* dc:91511 */
    if (cVar12 == '\x06') {                                                   /* parent */
        PECMD_AllocString(&local_418, 0x103c);                                /* dc:91525 扩容 */
        pWVar54 = local_418;
        LVar69.QuadPart = 0;
        pWVar63 = local_418 + 0x1000;                                         /* dc:91528 枚举缓冲 */
        pGVar61 = (GUID *)&g_guidDisk;                                        /* dc:91529 DAT_14011e870 */
        if ((*ptVar30 == L'\0') || (ptVar30[1] != L':')) {                    /* dc:91530 */
            if (-1 < (int)local_434) {
                if (local_368 == 8) {                                         /* cdrom */
                    wsprintfW((LPWSTR)pWVar63, L"\\.\CDROM%d", local_434); /* dc:91534 */
                    goto LAB_14009396e;
                }
                if (local_368 == 0x10) {                                      /* floppy */
                    pGVar61 = (GUID *)&g_guidFloppy;                          /* dc:91538 DAT_14011e8a0 */
                    pWVar63 = (LPCWSTR)(uint64_t)(uint)(local_434 & 0xfff);   /* dc:91539 */
                }
                else {
                    wsprintfW((LPWSTR)pWVar63, L"\\.\PhysicalDrive%d", local_434); /* dc:91542 */
                }
            }
        }
        else {
            wsprintfW((LPWSTR)pWVar63, L"\\.\%c:",                          /* dc:91547 */
                      (ulonglong)(uint16_t)*ptVar30);
            UVar28 = GetDriveTypeW(local_418 + 0x1004);                       /* dc:91548 "X:" */
            if (UVar28 == 2) {                                                /* DRIVE_REMOVABLE */
                if (local_368 != 0x10) goto LAB_14009399e;
                pGVar61 = (GUID *)&g_guidFloppy;
            }
            if (UVar28 == 5) {                                                /* DRIVE_CDROM */
LAB_14009396e:
                pGVar61 = (GUID *)&g_guidCdrom;                               /* dc:91555 DAT_14011e880 */
            }
        }
LAB_14009399e:
        PECMD_EnumDevices(pWVar63, local_418, local_300 | 0x20, pGVar61);     /* dc:91559 */
        FUN_1400629B8(param_1, pWVar39, local_418);                           /* dc:91560 变量赋值 */
        PECMD_ClearFlagAndError((byte *)&local_res20);                        /* dc:91561 */
        if ((pvVar47 != (HANDLE)0x0) && (pvVar47 != (HANDLE)-1)) {
            CloseHandle(pvVar47);
        }
        goto LAB_14008f546;
    }
    /* ================= dc:91567-91618 disk/part/dep 分派 ================= */
    if (cVar12 == '\x01') {                                                   /* dc:91567 disk */
        if ((local_40f == '\0') || (-1 < (longlong)(int64_t)(uintptr_t)local_360)) {
            bVar72 = local_430 != 0;
            local_430 = -local_430;
            PECMD_EnumeratePhysicalDrives((longlong *)&local_418,             /* dc:91571 */
                                          (uint)(-(int)bVar72 & 0x200000) |
                                          (uint)(int64_t)(uintptr_t)local_360 | local_3ac);
            LVar35.HighPart = 0;
            LVar35.LowPart = (*local_418 == L'\0');
            uVar18 = local_438;
        }
        else {
LAB_1400937d3:
            LVar35.QuadPart = -0x7ff8ffa9;
            uVar18 = local_438;
        }
        goto LAB_140093820;
    }
    if ((cVar12 == '\x02') && (local_40c == '\0')) {                          /* dc:91584 part */
        if (((longlong)(int64_t)(uintptr_t)local_360 < 0) ||
            ((local_409 != '\0' && (iVar17 < 0)))) {
            LVar35.QuadPart = -0x7ff8ffa9;
        }
        else {
            lVar56 = PECMD_DescribePartitionInfo(&local_418,                  /* dc:91589 */
                                                 (LPCWSTR)(int64_t)(uintptr_t)local_360,
                                                 (uint64_t)(longlong)iVar17,
                                                 (ulonglong)((int)(char)local_430 | local_408),
                                                 (LARGE_INTEGER *)0x0, (uint *)0x0, local_410);
            LVar35.HighPart = 0;
            LVar35.LowPart = (lVar56 < 1);
        }
        if ((int)local_438 < 1) goto LAB_140093838;                           /* dc:91595 */
        uVar18 = local_438 & 0x9fffffff | 0x10000000;
        goto LAB_140093820;
    }
    if ((cVar12 == '\b') && (local_40c == '\0')) {                            /* dc:91599 dep */
        if (((int)local_434 < 0) &&
            (((*ptVar30 != L'\0' &&
              (local_360 = ptVar30, ptVar30[1] == L':')) &&
             (*(short *)(ptVar30 + 2) == 0)))) {                              /* dc:91600 "X:" 形式 */
            ((uint16_t *)local_148)[0] = L'\\';                               /* dc:91604 构造 \.\X: */
            ((uint16_t *)local_148)[1] = 0x5c;
            ((uint16_t *)local_148)[3] = 0x5c;
            ((uint16_t *)local_148)[2] = 0x2e;
            ((uint16_t *)local_148)[4] = (uint16_t)*ptVar30;
            ((uint16_t *)local_148)[5] = 0x3a;
            local_360 = (WCHAR *)local_148;
            ((uint16_t *)local_148)[6] = 0;
        }
        if (((longlong)(int64_t)(uintptr_t)local_360 < 0) ||
            ((local_409 != '\0' && (iVar17 < 0))))                            /* dc:91613 */
            goto LAB_1400937d3;
        iVar17 = (int)PECMD_GetStorageDependency((uint64_t)(uintptr_t)param_1, /* dc:91615 */
                                                 &local_418,
                                                 (WCHAR *)(uintptr_t)local_360,
                                                 local_368);
        LVar35.QuadPart = (int64_t)iVar17;
        uVar18 = local_438;
        goto LAB_140093820;
    }
    /* ================= dc:91620-91660 drv 分派 ================= */
    local_458.QuadPart = LVar69.QuadPart;                                     /* dc:91620 */
    if (cVar12 == '\x03') {                                                   /* drv */
        uVar20 = (uint)(uint16_t)*ptVar30;                                    /* dc:91622 盘符字符 */
        uVar25 = uVar20 & 0xffffffdf;
        if ((int)local_368 < 1) {
            *local_418 = (WCHAR)*ptVar30;
            ptVar30 = local_360;
            if (*ptVar30 == L'\\') {
                ptVar30 = (WCHAR *)(uintptr_t)ptVar68;                        /* dc:91628 */
            }
            if (*local_418 == L'\0') {
                LVar35.QuadPart = -0x7ff8ffa9;
            }
            else {
                lVar56 = PECMD_DescribePartitionInfo(&local_418,              /* dc:91634 */
                                                     (LPCWSTR)(uintptr_t)ptVar30,
                                                     (uint64_t)(longlong)iVar17,
                                                     (ulonglong)(local_408 & 0xfffffffd | 0xc),
                                                     (LARGE_INTEGER *)0x0, (uint *)0x0,
                                                     local_410);
                LVar35.HighPart = 0;
                LVar35.LowPart = (lVar56 < 1);
            }
        }
        else {
            FUN_1400633A8(&local_348.QuadPart, 0x3b60);                       /* dc:91642 盘符表缓冲 */
            LVar69.QuadPart = local_348.QuadPart;
            PECMD_EnumDrivesToTable(local_348.QuadPart + 0x200, (LPWSTR)(uintptr_t)local_348.QuadPart, /* dc:91644 */
                                    uVar25 << 0x10 | uVar18);
            LVar35.QuadPart = 0;
            lVar56 = (longlong)(int)((uVar20 & 0xffffffdf) - 0x41) * 0x220;   /* dc:91646 条目偏移 */
            if (*(int *)(LVar69.QuadPart + 0x208 + lVar56) < 1) {
                LVar35.QuadPart = -0x7ff8ffa9;
            }
            else {
                PECMD_AllocString(&local_418, 100);                           /* dc:91651 */
                wsprintfW(local_418, L"%d %c:",                               /* dc:91652 */
                          *(uint32_t *)(LVar69.QuadPart + 0x204 + lVar56),
                          (ulonglong)uVar25);
            }
            PECMD_FreeStrBuf(&local_348.QuadPart);
        }
        PECMD_FreeStrBuf(&local_458.QuadPart);                                /* dc:91657 */
        uVar18 = local_438;
        goto LAB_140093820;
    }
    /* ================= dc:91661-91741 cdrom/floppy/volume 分派 ================= */
    if ((cVar12 == '\x04') || (cVar12 == '\x05')) {                           /* dc:91661 cdrom/floppy */
        PECMD_AllocString(&local_418, 0x1000);                                /* dc:91662 */
        if ((int)local_434 < 0) {
            if (local_40f == '\0') {
                pGVar61 = (GUID *)&g_guidFloppy;                              /* dc:91665 */
                if ((char)local_3e8 == '\x04') {
                    pGVar61 = (GUID *)&g_guidCdrom;
                }
                PECMD_EnumDevices((LPCWSTR)(int64_t)(int)local_434, local_418, 0, /* dc:91669 */
                                  pGVar61);
            }
            goto LAB_14009367e;
        }
        FUN_1400633A8(&local_348.QuadPart, 0x3b60);                           /* dc:91673 */
        LVar69.QuadPart = local_348.QuadPart;
        puVar36 = (ulonglong *)(local_348.QuadPart + 0x200);
        FUN_140102A90(puVar36, 0, 0x3960);   /* memset 别名 (同 core_b3r_h4.c 惯例) */
        PECMD_EnumDrivesToTable((int64_t)(uintptr_t)puVar36, (LPWSTR)(uintptr_t)LVar69.QuadPart, /* dc:91677 */
                                local_368);
        lVar56 = 0;
        iVar17 = 0;
        puVar38 = (uint *)(LVar69.QuadPart + 0x204);
        goto LAB_1400935e3;
    }
    if (cVar12 == '\a') {                                                     /* dc:91683 volume */
        uVar49 = (ulonglong)local_448;
        if ((int)local_434 < 0) {
            if (*ptVar30 != L'\0') {
                if (ptVar30[1] == L':') {                                     /* dc:91687 */
                    if ((*(short *)(ptVar30 + 2) == 0x5c) &&                  /* dc:91688 "X:\" 检查 */
                        (*(short *)(ptVar30 + 3) == 0)) {
                        *(short *)(ptVar30 + 2) = 0;                          /* dc:91690 */
                    }
                    if (*(short *)(ptVar30 + 2) == 0) {
                        PECMD_AllocString(&local_458.QuadPart, 9);            /* dc:91693 */
                        *(short *)(uintptr_t)local_458.QuadPart = 0x5c;
                        *(short *)(local_458.QuadPart + 2) = 0x5c;
                        *(short *)(local_458.QuadPart + 4) = 0x2e;
                        *(short *)(local_458.QuadPart + 6) = 0x5c;
                        *(WCHAR *)(local_458.QuadPart + 8) = (WCHAR)*ptVar30;
                        *(short *)(local_458.QuadPart + 10) = 0x3a;
                        *(short *)(local_458.QuadPart + 0xc) = 0;
                    }
                    if ((ulonglong)local_458.QuadPart != 0) goto LAB_140093485; /* dc:91702 */
                }
                PECMD_StrBldCopyWide((void *)&local_458.QuadPart,              /* dc:91704 FUN_1400703e4 */
                                     (LPCWSTR)(uintptr_t)ptVar30);
            }
LAB_140093485:
            uVar49 = (ulonglong)local_448 & 0xffffffff;
            if ((int)local_434 < 0) {
                uVar49 = 0xffffffff;
            }
        }
        iVar17 = (int)uVar49;                                                 /* dc:91712 */
        if (iVar17 < 1) {
            iVar17 = -1;
        }
        uVar18 = local_408;                                                   /* dc:91716 */
        if ((local_3ac >> 0x10 & 1) != 0) {
            uVar18 = local_408 | 0x20;
        }
        if ((((uVar18 & 0x402) != 0) && (-1 < (int)local_434)) && (0 < iVar17)) {
            *local_418 = L'\0';
            lVar56 = PECMD_DescribePartitionInfo(&local_418,                  /* dc:91722 */
                                                 (LPCWSTR)(int64_t)(int)local_434,
                                                 (uint64_t)(longlong)iVar17, 0x202,
                                                 (LARGE_INTEGER *)0x0, (uint *)0x0, '\0');
            iVar17 = (int)lVar56;
        }
        pWVar37 = PECMD_EnumerateVolume((int64_t *)&local_418,                /* dc:91727 */
                                        (int64_t)(int)local_434,
                                        ((int64_t)iVar17 & 0xfffffffff000ffffULL) |
                                        ((ulonglong)uVar18 << 0x10),
                                        (LPCWSTR)(uintptr_t)local_458.QuadPart);
        if ((pWVar37 == (WCHAR *)0x0) || (*local_418 == L'\0')) {
            iVar17 = -0x7ff8ffa9;
        }
        else {
            iVar17 = 0;
        }
        LVar35.QuadPart = (int64_t)iVar17;                              /* dc:91736 */
        local_278 = LVar35;
        PECMD_FreeStrBuf(&local_458.QuadPart);
        uVar18 = local_438;
        goto LAB_140093820;
    }
    /* ================= dc:91742-91791 -L/-cmp 预处理 + 打开设备 ================= */
    if (local_450 != '\0') {                                                  /* dc:91742 -L */
        local_380 = local_380 & 0xffff0000;
        local_448 = 5;
        local_428.QuadPart = LVar69.QuadPart;
        local_3f8.QuadPart = LVar69.QuadPart;
        local_3c8 = uVar20;
    }
    local_3b8.QuadPart = 0;                                                   /* dc:91749 */
    local_2b0.QuadPart = 0;
    local_448 = local_448 & 0xffffffff;
    if (local_462 != '\0') {                                                  /* dc:91752 +L */
        local_448 = 5;
    }
    if ((char)local_340 != '\0') {                                            /* dc:91755 del */
        local_380 = local_380 & 0xffff0000;
        local_3f8.QuadPart = 0;
        local_3c8 = 0;
        local_428.QuadPart = 0;
    }
    LVar33 = local_3f8;
    LVar69 = local_428;
    if (((local_40f != '\0') && ((longlong)(int64_t)(uintptr_t)local_360 < 0)) ||
        ((local_409 != '\0' && ((int)local_448 < 0)))) {                      /* dc:91763 */
        PECMD_FreeStrBuf(&local_458.QuadPart);
        PECMD_ClearFlagAndError((byte *)&local_res20);
        goto joined_r0x000140093a51;
    }
    if ((local_42f & 2) == 0) {                                               /* dc:91769 */
        local_448 = (ulonglong)((int)local_448 - 1);
    }
    local_460 = 0;                                                            /* dc:91772 */
    local_40d = 0;
    local_3b0 = local_3b0 & 0xffff0000;
    local_3d8 = 0;
    local_3d7 = '\0';
    local_370 = -1;                                                           /* dc:91777 */
    local_3e0 = 0xffffffffffffffffULL;
    local_298[0] = 0;
    if ((((local_464 == '\0') && (local_3c8 == 0)) &&
         ((ulonglong)local_428.QuadPart == 0)) &&
        (local_340 = local_340 & 0xff, (ulonglong)local_3f8.QuadPart == 0)) { /* dc:91780 */
        local_340 = 1;
    }
    SetLastError(0);                                                          /* dc:91786 */
    if ((((local_39f & 1) == 0) || (LVar69.QuadPart < 0)) ||
        (uVar18 = 1, LVar33.QuadPart < 1)) {                                  /* dc:91787 fs 类型可写判定 */
        uVar18 = 0;
    }
    local_440 = uVar18;
    PECMD_OpenFileHandle(&local_3a8, (LPCWSTR)(uintptr_t)local_330, 0xc0000000, 3, /* dc:91792 */
                         (LPSECURITY_ATTRIBUTES)0x0, 3, 0x20000000, (HANDLE)0x0);
    pvVar47 = local_3a8;
    if (local_3a8 == (HANDLE)0x0) {
        DVar23 = GetLastError();                                              /* dc:91796 */
        LVar69.HighPart = 0;
        LVar69.LowPart = DVar23;
        PECMD_FreeStrBuf(&local_458.QuadPart);
        PECMD_ClearFlagAndError((byte *)&local_res20);
        goto LAB_14008f546;
    }
    uVar20 = FUN_14005F96C(local_3a8, 0x200);                                 /* dc:91803 */
    local_44c = 0x200;
    if (0x1ff < (int)uVar20) {
        local_44c = uVar20;
    }
    if (uVar18 != 0) {                                                        /* dc:91808 */
        if (-1 < local_428.QuadPart) {
            PECMD_ErrorHandlerWrap((byte *)&local_res20,                      /* dc:91810 锁卷 */
                                   (int)(int64_t)(uintptr_t)local_360,
                                   (uint64_t)(local_428.QuadPart << 9),
                                   (int64_t)(local_44c * 0x28), 0, (WCHAR *)0x0);
        }
        if (local_462 == '\0') {                                              /* dc:91813 软盘镜像写 */
            PECMD_WriteFloppyImage(pvVar47,                                   /* dc:91814 */
                                   (int64_t)(local_428.QuadPart + local_2a0.QuadPart),
                                   local_3f8.QuadPart, local_44c);
            PECMD_ClearFlagAndError((byte *)&local_res20);
            local_280.QuadPart = 0;
        }
    }
    local_404 = PECMD_QueryDiskGeometry(local_3a8, (uint64_t *)0x0, 1, 0x800); /* dc:91820 */
    /* ================= dc:91821-91874 旗标收敛 + 锁定路径进入 ================= */
    if ((-1 < (longlong)(int64_t)(uintptr_t)local_360) &&
        ((((local_463 == 0x20 || (local_463 == 0x10)) && (local_404 == 0xb)) || /* dc:91822 */
         ((((local_468 & 0x20) != 0 ||
           (((local_468 != 0 && (local_404 == 0xb)) || ((local_465 & 0x20) != 0)))) ||
          ((local_465 != 0 && (local_404 == 0xb)))))))) {
        local_42c = local_42c | 1;
    }
    if (((char)local_2c0 < '\x01') &&
        (((local_467 != 0 || (local_463 == 1)) ||
          ((local_463 == 0x20 || (local_468 != 0)))))) {                        /* dc:91828 */
        local_42c = local_42c | 2;
    }
    if ((local_463 == 1) || (local_463 == 0x20)) {                             /* dc:91833 */
        local_42c = local_42c | 4;
    }
    if ((((local_465 & 0x20) != 0) || ((local_465 != 0 && (local_404 == 0xb)))) ||
        (((local_463 & 0x10) != 0 && (local_404 != 0xc)))) {                   /* dc:91836 */
        local_42c = local_42c | 8;
    }
    if ((char)local_2c0 < '\x01') {                                            /* dc:91840 */
        local_42c = local_42c | 0x10;
    }
    if (local_463 == 0x10) {                                                   /* dc:91843 */
        local_42c = local_42c | 0x20;
    }
    if ((local_461 == '\0') || (iVar27 == 0)) {                                /* dc:91846 */
        local_42c = local_42c | 0x40;
    }
    if (local_463 == 0x18) {                                                   /* dc:91849 */
        local_42c = local_42c | 0x80;
    }
    if ((local_404 == 0xc) && ((local_468 != 0 || (local_467 != 0)))) {        /* dc:91852 */
        local_42c = local_42c | 0x100;
    }
    LVar33.QuadPart = 0;                                                       /* dc:91855 */
    if ((((local_461 != '\0') && (iVar27 == 0)) ||
         ((local_463 == 0x10 && ((local_465 & 0x20) != 0)))) || (local_463 != 0)) {
        uVar49 = 0;
        PECMD_FreeStrBuf(&local_458.QuadPart);
        uVar18 = local_42c;
        bVar52 = local_465;
        iVar17 = local_404;
        goto LAB_14008f48e;                                                    /* dc:91863 锁定路径 */
    }
    local_2f0.QuadPart = 0;                                                    /* dc:91865 */
    if (0xff < (int)local_3c8) {                                               /* dc:91866 */
LAB_140093369:
        PECMD_FreeStrBuf(&local_458.QuadPart);
        PECMD_ClearFlagAndError((byte *)&local_res20);
        if (pvVar47 != (HANDLE)-1) {
            CloseHandle(pvVar47);
        }
        goto LAB_14008f5f0;
    }
    uVar20 = (uint)local_448;                                                  /* dc:91875 */
    if ((local_462 == '\0') && ((int)uVar20 < 0)) {
        if ((-1 < local_428.QuadPart) ||
            ((-1 < local_3f8.QuadPart || (-1 < (int)local_3c8))))            /* dc:91876 */
            goto LAB_140093369;                                               /* dc:91877 */
        local_3c8 = 0xffffffff;
        local_3f8.QuadPart = -1;
        local_428.QuadPart = -1;
    }
    local_320.QuadPart = 0;                                                    /* dc:91883 */
    local_288.QuadPart = 0;
    local_258.QuadPart = -1;
    local_2a8 = (ushort *)0xffffffffffffffff;
    local_3fc = uVar20;
    if ((((local_462 == '\0') && (local_450 == '\0')) && ((int)uVar20 < 4)) || /* dc:91889 */
        ((local_464 != '\0' || ((char)local_400 != '\0')))) {
LAB_140090245:
        uVar20 = local_44c;                                                   /* dc:91892 */
        LVar69.QuadPart = 1;
        local_3d6 = (3 < (int)local_448);
        puVar50 = (byte *)((longlong)(int64_t)LVar33.QuadPart + local_2a0.QuadPart);
        LVar33.QuadPart = (int64_t)(uintptr_t)puVar50 * 0x200;
        uVar49 = (ulonglong)local_44c;
        local_278.QuadPart = LVar33.QuadPart;
        if ((uVar18 != 0) && (local_462 != '\0')) {                           /* dc:91899 */
            PECMD_WriteFloppyImage(pvVar47,                                   /* dc:91900 */
                                   (int64_t)(local_2a0.QuadPart + local_428.QuadPart),
                                   local_3f8.QuadPart, local_44c);
            PECMD_ClearFlagAndError((byte *)&local_res20);
            local_280.QuadPart = 0;
        }
        LVar35 = PECMD_SetFilePointer(pvVar47, LVar33, 0);                    /* dc:91905 */
        if (LVar35.QuadPart + (int64_t)(uintptr_t)puVar50 * -0x200 != 0) {    /* dc:91906 定位失败 */
LAB_140093336:
            PECMD_FreeStrBuf(&local_458.QuadPart);
            PECMD_ClearFlagAndError((byte *)&local_res20);
            if (pvVar47 != (HANDLE)-1) {
                CloseHandle(pvVar47);
            }
            goto LAB_14008f528;
        }
        FUN_1400633A8(&local_3d0, (int64_t)(uVar20 * 3));                     /* dc:91914 双扇区缓冲 */
        local_2c8 = (char *)(((ulonglong)~(uVar20 - 1) & uVar49) +
                             (ulonglong)(uintptr_t)local_3d0);                /* dc:91915 对齐基址 */
        pWVar54 = (LPWSTR)(local_2c8 + uVar49);
        local_388 = pWVar54;
        local_328 = uVar49;
        memset(pWVar54, 0, uVar49);                                           /* dc:91919 */
        BVar19 = ReadFile(pvVar47, pWVar54, local_44c, &local_3f0, NULL); /* dc:91920 */
        if (BVar19 == 0) {
            PECMD_FreeStrBuf(&local_3d0);
            goto LAB_140093336;
        }
        PECMD_MemMoveForward((byte *)local_2c8, (byte *)pWVar54, (int)local_328); /* dc:91925 快照 */
        if (((char)local_400 != '\0') && (local_464 == '\0')) {               /* dc:91926 fix 路径 */
            PECMD_FixRandomSeed((int64_t)(uintptr_t)pWVar54);                 /* dc:91927 */
            if ((local_40b < '\x01') &&
                ((-1 < local_40b ||
                  (local_350.QuadPart <= (longlong)(int64_t)(local_44c * 2 + 0x4000))))) { /* dc:91928 */
                bVar72 = false;
            }
            else {
                bVar72 = true;
            }
            if ((char)local_400 == '\x01') {
                if ((bVar72) && (0x200 < local_44c)) {
                    memset(pWVar54 + 0x100, 0, (size_t)(local_44c - 0x200));  /* dc:91938 */
                }
                memset(pWVar54 + 0xdf, 0, 0x40);                              /* dc:91940 清表项 */
            }
            uVar49 = 0;
            ((uint16_t *)pWVar54)[0xff] = 0xaa55;                             /* dc:91943 签名 */
            iVar17 = PECMD_AnsiStrNCompare(local_2c8, (int64_t)(uintptr_t)pWVar54, /* dc:91944 */
                                           (int64_t)local_328);
            LVar69 = local_3b8;
            if (iVar17 != (int)uVar49) {
                PECMD_SetFilePointer(pvVar47, LVar33, 0);                     /* dc:91947 */
                local_3f0 = 0;
                BVar19 = WriteFile(pvVar47, pWVar54, local_44c, &local_3f0, NULL); /* dc:91950 */
                uVar49 = 0;
                local_2b0.QuadPart = (uint64_t)(byte)(BVar19 == 0);           /* dc:91952 */
                LVar69 = local_2b0;
            }
            bVar52 = local_465;                                               /* dc:91956 */
            if (((local_465 == 0 && local_468 == 0) && local_467 == 0) ||
                ((int)local_434 < (int)uVar49)) {
                PECMD_FreeStrBuf(&local_3d0);                                 /* dc:91959 */
                PECMD_FreeStrBuf(&local_458.QuadPart);
                PECMD_ClearFlagAndError((byte *)&local_res20);
                if (pvVar47 != (HANDLE)-1) {
                    CloseHandle(pvVar47);
                }
                goto LAB_1400943b2;
            }
            uVar49 = uVar49 & 0xffffffff;                                     /* dc:91967 */
            PECMD_FreeStrBuf(&local_3d0);
            PECMD_FreeStrBuf(&local_458.QuadPart);
            uVar18 = local_42c;                                               /* dc:91970 */
            iVar17 = local_404;
            goto LAB_14008f48e;
        }
        /* dc:91974-92030 布局刷新 + 容量计算 */
        if (((char)local_3e8 != '\x02') && (local_45f == 0)) {                /* dc:91974 */
            if (local_40a == '\0') goto LAB_1400904ec;
            if (1 < (int)local_448) goto LAB_1400904de;
LAB_1400904cc:
            ppuVar41 = &local_3d0;
            goto LAB_14008fc35;                                               /* dc:91979 清理出栈 */
        }
LAB_1400904de:                                                                /* dc:91981 */
        if ((local_40a != '\0') && ((int)local_448 < 0)) goto LAB_1400904cc;
LAB_1400904ec:                                                                /* dc:91983 */
        FUN_1400633A8(&local_3c0, 0x200010);                                  /* dc:91984 布局缓冲 */
        local_420[0] = 0;
        local_466 = 0xff;
        local_3f0 = 0;
        uVar18 = 0;
        if ((-1 < (short)local_380) &&
            (uVar18 = (int)local_448 + 1, (short)local_380 == 0)) {           /* dc:91989 */
            uVar18 = (uint)(uint16_t)(-(short)uVar18);
        }
        PECMD_UpdatePartitionLayout(pvVar47, &local_3c0, local_420, &local_466, /* dc:91992 */
                                    uVar18 | local_358[0]);
        puVar36 = (ulonglong *)(uintptr_t)local_3c0;
        if ((uVar18 != 0) && (-1 < (char)local_466)) {                        /* dc:91994 */
            LVar69.LowPart = ((int)(char)local_466 & 0x10U) >> 4;             /* dc:91995 */
            LVar69.HighPart = 0;
            PECMD_FreeStrBuf(&local_3c0);
            PECMD_FreeStrBuf(&local_3d0);
            PECMD_FreeStrBuf(&local_458.QuadPart);
            goto LAB_1400943f2;
        }
        bVar52 = local_466;                                                   /* dc:92002 */
        if ('\x0f' < (char)local_466) {
            bVar52 = local_466 & 0xf;
        }
        if ((-1 < (char)bVar52) &&
            ((local_462 != '\0' || local_450 != '\0') || (char)local_340 != '\0')) { /* dc:92006 */
            PECMD_FreeStrBuf(&local_3c0);
            goto LAB_1400904cc;
        }
        if ((longlong)(int64_t)(uintptr_t)local_360 < 0) {                    /* dc:92011 文件形式 */
            LVar69.QuadPart = 0;
            if (-1 < local_1f8.QuadPart) {
                LVar69 = local_1f8;
            }
            BVar19 = GetFileAttributesExW((LPCWSTR)(uintptr_t)local_330,
                                          1, (void *)local_188u.geo); /* dc:92015 */
            if ((BVar19 != 0) && ((*(uint32_t *)&local_188u.geo[0] & 0x10) == 0)) {
                uVar49 = ((ulonglong)*(uint32_t *)&local_188u.geo[28] & 0xfffffe00) << 0x17 | /* dc:92017 */
                         (ulonglong)(*(uint32_t *)&local_188u.geo[32] >> 9);
                goto LAB_140090624;
            }
        }
        else {
            lVar56 = FUN_14005FBD4(pvVar47, (int64_t *)(uintptr_t)local_3c0); /* dc:92023 设备容量 */
            uVar49 = (ulonglong)(lVar56 / (longlong)local_328);
LAB_140090624:                                                                /* dc:92025 */
            LVar69.LowPart = (DWORD)(uVar49 - (ulonglong)local_2a0.LowPart);  /* dc:92026 .s 截断 */
            if ((0 < local_1f8.QuadPart) &&
                ((ulonglong)LVar69.QuadPart < (ulonglong)local_1f8.QuadPart)) { /* dc:92027 */
                LVar69 = local_1f8;
            }
        }
        /* dc:92031-92045 布局查询 + MBR/GPT 判定 */
        local_3b8.QuadPart = (int64_t)(uintptr_t)                             /* dc:92031 */
            FUN_14005FEAC(pvVar47, (uint64_t *)(uintptr_t)puVar36, (uint32_t *)local_420);
        if (*ptVar30 == L'\0') {                                              /* dc:92032 */
            if ((ulonglong)local_3b8.QuadPart != 0) goto LAB_14009067f;       /* dc:92033 */
            if ((local_400 & 0x10) != 0) goto LAB_14009067a;                  /* dc:92034 */
            goto LAB_1400906d1;                                               /* dc:92035 */
        }
LAB_14009067a:                                                                /* dc:92037 */
        if ((ulonglong)local_3b8.QuadPart == 0) {
            iVar17 = 0;
        }
        else {
LAB_14009067f:                                                                /* dc:92042 */
            iVar17 = *(int *)(uintptr_t)local_3b8.QuadPart;
        }
        local_466 = (iVar17 == 1);                                            /* dc:92045 GPT? */
        if ((!(bool)local_466) || (local_44f[0] = 1, local_39d != '\0')) {    /* dc:92046 */
            local_44f[0] = 0;
        }
        if ((local_39c == '\0') && ((char)local_400 == '\0')) {               /* dc:92049 */
            if (local_44f[0] == 0) {
                if (local_464 == '\0') goto LAB_1400906f2;
            }
            else if (local_464 != '\0') goto LAB_1400906f2;
            goto LAB_1400906d1;
        }
LAB_1400906f2:                                                                /* dc:92056 */
        local_440 = (DWORD)local_218.LowPart;                                 /* dc:92057 柱面 */
        local_45c = (uint)(int)local_1f0.LowPart;                             /* dc:92058 扇区数 */
        local_3d4 = (DWORD)local_208.LowPart;                                 /* dc:92059 磁头 */
        if (*ptVar30 == L'\0') {                                              /* dc:92060 几何查询 */
            memset(&local_188u.geo[0], 0, 24);
            DeviceIoControl(pvVar47, 0x70000, (LPVOID)0x0, 0, &local_188u.geo[0], /* dc:92063 */
                            0x18, &local_3f0, NULL);
            if (local_218.QuadPart < 1) {                                     /* dc:92065 */
                local_218.QuadPart = (int64_t)(int)*(int32_t *)&local_188u.geo[0xc]; /* local_17c */
                local_440 = *(uint32_t *)&local_188u.geo[0xc];
            }
            if (local_208.QuadPart < 1) {                                     /* dc:92069 */
                local_208.QuadPart = (int64_t)(int)*(int32_t *)&local_188u.geo[0x10]; /* local_178 */
                local_3d4 = *(uint32_t *)&local_188u.geo[0x10];
            }
            if (local_1f0.QuadPart < 1) {                                     /* dc:92073 */
                local_1f0.QuadPart = (int64_t)(int)*(int32_t *)&local_188u.geo[0x14]; /* local_174 */
                local_45c = *(uint32_t *)&local_188u.geo[0x14];
            }
        }
        if (local_218.QuadPart < 1) {                                         /* dc:92078 */
            local_440 = 0xff;
            local_218.QuadPart = 0xff;
        }
        if (local_208.QuadPart < 1) {                                         /* dc:92082 */
            local_3d4 = 0x3f;
            local_208.QuadPart = 0x3f;
        }
        if (local_1f0.QuadPart < 1) {                                         /* dc:92086 */
            local_45c = 0x200;
            if (local_44c != 0) {
                local_45c = local_44c;
            }
            local_1f0.QuadPart = (int64_t)(int)local_45c;
        }
        /* dc:92093-92152 GPT 表区布局计算 */
        local_378.QuadPart = local_328 + (longlong)(uintptr_t)puVar36;        /* dc:92093 表区尾 */
        local_2b8.QuadPart = local_378.QuadPart + local_328;                  /* dc:92094 条目区 */
        local_420[0] = (int)(local_45c + ((int)local_45c >> 0x1f & 0x1ffU)) >> 9; /* dc:92095 扇区数 */
        LVar33.QuadPart = local_2b8.QuadPart + 0x10000;                       /* dc:92096 */
        if ((local_44f[0] != 0) || (local_464 != '\0')) {                     /* dc:92097 GPT 路径 */
            local_310 = local_310 & 0xff;                                     /* dc:92098 */
            if ((char)local_340 != '\0') {
                local_310 = 0;
            }
            cVar12 = (char)local_400;
            if ((local_466 == 0) || (cVar12 != '\0')) {
                uVar18 = 0x80;                                                /* dc:92104 条目数 */
            }
            else {
                uVar18 = *(uint32_t *)(local_3b8.QuadPart + 0x28);            /* dc:92107 现有条目数 */
            }
            lVar56 = (longlong)(int)local_420[0];
            local_428.QuadPart = local_428.QuadPart / lVar56;                 /* dc:92110 */
            local_1f8.QuadPart = local_1f8.QuadPart / lVar56;
            local_3f8.QuadPart = local_3f8.QuadPart / lVar56;
            local_370 = local_3f8.QuadPart + -1;                              /* dc:92113 */
            if (local_39e != false) {
                local_370 = (local_3f8.QuadPart + -1) + local_428.QuadPart;   /* dc:92115 */
            }
            local_388 = (LPWSTR)0x0;
            if (cVar12 == '\x01') {                                           /* dc:92118 fix */
                if ((local_3eb & 0x10) != 0) {
                    local_370 = local_1f8.QuadPart + -1;                      /* dc:92120 */
                    LVar69.QuadPart = local_1f8.QuadPart;
                }
                if (local_428.QuadPart < 1) {
                    local_428.QuadPart = 1;                                   /* dc:92123 */
                }
                local_388 = (LPWSTR)(uintptr_t)local_370;
                if ((longlong)(uintptr_t)local_370 < 1) {
                    local_388 = (LPWSTR)(LVar69.QuadPart + -1);               /* dc:92128 */
                }
            }
            local_3fc = uVar18;                                               /* dc:92131 */
            local_3b8.QuadPart = LVar33.QuadPart;                             /* dc:92132 */
            local_2f0.QuadPart = (int64_t)                                    /* dc:92133 */
                PECMD_ReadDiskSectorScan(&local_3a8, (uint64_t *)(uintptr_t)puVar36,
                                         uVar18, local_44c,
                                         (ulonglong)(-(uint64_t)(uint64_t)(cVar12 != '\0')) &
                                             (uint64_t)local_428.QuadPart,
                                         (uint64_t *)&local_388, local_350);
            LVar69.QuadPart = 1;
            if ((local_466 == 0) && ((ulonglong)local_2f0.QuadPart == 0)) {   /* dc:92138 */
                local_2f0.QuadPart = LVar69.QuadPart;
            }
            DVar23 = local_2f0.LowPart;
            if ((longlong)(uintptr_t)local_388 < 0x20) {                      /* dc:92142 */
LAB_1400932de:
                PECMD_FreeStrBuf(&local_3c0);
                PECMD_FreeStrBuf(&local_3d0);
                PECMD_FreeStrBuf(&local_458.QuadPart);
                PECMD_ClearFlagAndError((byte *)&local_res20);
                if (pvVar47 != (HANDLE)-1) {
                    CloseHandle(pvVar47);
                }
                goto LAB_14008efd3;
            }
            /* dc:92153-92215 list part 条目输出 */
            if ((char)local_3e8 == '\x02') {
                PECMD_AllocString(&local_418, (int64_t)(uVar18 * 10));        /* dc:92154 */
                LVar69 = local_2b8;
                puVar48 = (uint16_t *)g_szEmpty;                              /* dc:92156 DAT_14011c638 */
                uVar25 = (uint)local_448;
                uVar20 = 0;
                if ((-1 < (int)uVar25) &&
                    (uVar20 = uVar25, (int)(uVar25 + 1) < (int)uVar18)) {     /* dc:92159 */
                    uVar18 = uVar25 + 1;
                }
                lVar56 = (longlong)(int)uVar20;
                if (lVar56 < (int)uVar18) {
                    puVar36 = (ulonglong *)(lVar56 * 0x80 + local_2b8.QuadPart); /* dc:92164 */
                    lVar56 = (int)uVar18 - lVar56;
                    uVar49 = (ulonglong)local_448;
                    pWVar54 = local_418;
                    do {
                        uVar20 = uVar20 + 1;
                        uVar31 = (ulonglong)uVar20;
                        if ((*puVar36 != 0) || (puVar36[1] != 0)) {           /* dc:92171 条目非空 */
                            if ((int)uVar49 < 0) {
                                iVar17 = wsprintfW(pWVar54, L"%s%d", puVar48, uVar31); /* dc:92173 */
                                puVar48 = (uint16_t *)L" ";                   /* dc:92174 DAT_14011d500 */
                                pWVar54 = pWVar54 + iVar17;
                            }
                            else {
                                iVar17 = wsprintfW(pWVar54, L"%s%d ", puVar48, uVar31); /* dc:92178 */
                                pWVar54 = pWVar54 + iVar17;
                                puVar60 = (ulonglong *)((longlong)(int)uVar49 * 0x80 + /* dc:92180 */
                                                        LVar69.QuadPart);
                                PECMD_GuidToString(pWVar54, (uint32_t *)puVar60, 0); /* dc:92181 类型 GUID */
                                iVar17 = lstrlenW(pWVar54);
                                pWVar54 = pWVar54 + iVar17;
                                lstrcpyW(pWVar54, L" GPT ");                  /* dc:92184 */
                                PECMD_FormatI64Dec(pWVar54 + 5,               /* dc:92185 FirstLBA×512 */
                                                   (int64_t)(puVar60[4] << 9));
                                iVar17 = lstrlenW(pWVar54 + 5);
                                pWVar54 = pWVar54 + (longlong)iVar17 + 5;
                                *pWVar54 = L' ';
                                PECMD_FormatI64Dec(pWVar54 + 1,               /* dc:92189 尺寸 */
                                                   (int64_t)(((puVar60[5] - puVar60[4]) + 1) * 0x200));
                                iVar17 = lstrlenW(pWVar54 + 1);
                                pWVar54 = pWVar54 + (longlong)iVar17 + 1;
                                *pWVar54 = L' ';
                                PECMD_SprintfRetEnd(pWVar54 + 1, puVar60[6],  /* dc:92193 属性 */
                                                    L"%016I64X");
                                iVar17 = lstrlenW(pWVar54 + 1);
                                pWVar54[(longlong)iVar17 + 1] = L' ';
                                pWVar54 = pWVar54 + (longlong)iVar17 + 1 + 1;
                                PECMD_GuidToString(pWVar54, (uint32_t *)(puVar60 + 2), 0); /* dc:92197 唯一 GUID */
                                uVar49 = (ulonglong)local_448;
                                if ((local_408 & 0x10) != 0) {                /* dc:92199 -phy# 附加序号 */
                                    iVar17 = lstrlenW(pWVar54);
                                    pWVar54 = pWVar54 + iVar17;
                                    *pWVar54 = L' ';
                                    iVar17 = wsprintfW(pWVar54 + 1, L"%d", uVar31);
                                    pWVar54 = pWVar54 + (longlong)iVar17 + 1;
                                    uVar49 = (ulonglong)local_448;
                                }
                            }
                        }
                        puVar36 = puVar36 + 0x10;
                        lVar56 = lVar56 + -1;
                        pvVar47 = local_3a8;
                        pWVar39 = local_2f8;
                    } while (lVar56 != 0);
                }
                goto LAB_140093253;
            }
            /* dc:92216-92229 备份表区计算 */
            local_3e0 = (local_3e0 & 0xffffffff00000000ULL) | 1;              /* dc:92216 */
            uVar20 = (uint)local_448;
            uVar49 = (ulonglong)local_448;
            if (((uVar18 <= uVar20) || ((int)uVar18 <= (int)local_398)) ||
                ((-1 < (int)uVar20 &&
                  (iVar17 = PECMD_AnsiStrNCompare(                            /* dc:92222 */
                                    (char *)(local_2b8.QuadPart + (longlong)(int)uVar20 * 0x80),
                                    (int64_t)(uintptr_t)g_gptUnusedEntry16, 0x10),
                   iVar17 == 0)))) {
                local_3e0 = local_3e0 & 0xffffffff00000000ULL;
            }
            LVar35 = local_2b8;
            LVar33.QuadPart = (ulonglong)(local_44c + 0x3fff) / (ulonglong)local_44c; /* dc:92227 */
            local_3b0 = LVar33.LowPart;
            bVar52 = (byte)LVar69.LowPart;
            if ((byte)local_400 == bVar52) {                                  /* dc:92230 */
                if (uVar18 <= (uint)uVar49) goto LAB_1400932de;
                local_45c = local_3fc * 0x80;
                uVar18 = 0x4800 - (DVar23 + 1) * local_44c;
                local_420[0] = uVar18;
                if ((local_40b < '\x01') &&
                    ((-1 < local_40b ||
                      (local_350.QuadPart <= (longlong)(int64_t)(local_44c * 2 + 0x4000))))) {
                    local_460 = 0;                                            /* dc:92237 逐扇区清零 */
                    iVar17 = 0;
                    LVar33 = local_2b8;
                    LVar69 = local_378;
                    if (0 < (int)local_45c) {
                        do {
                            if (iVar17 < (int)uVar18) {
                                memset((void *)(uintptr_t)LVar33.QuadPart, 0, 0x1b0); /* dc:92244 */
                                memset((void *)(LVar33.QuadPart + 0x1be), 0, 0x40);
                                iVar17 = iVar17 + 0x200;
                                LVar33.QuadPart = LVar33.QuadPart + 0x200;
                            }
                            else {
                                memset((void *)(uintptr_t)LVar33.QuadPart, 0, 0x10); /* dc:92251 */
                                iVar17 = iVar17 + 0x80;
                                LVar33.QuadPart = LVar33.QuadPart + 0x80;
                            }
                            ptVar68 = (int64_t)(uintptr_t)local_260;
                            ptVar30 = local_290;
                            pWVar39 = local_2f8;
                            LVar69 = local_378;
                        } while (iVar17 < (int)local_45c);
                    }
                }
                else {                                                        /* dc:92261 块清零 */
                    local_460 = bVar52;
                    if (0x200 < local_44c) {
                        memset((byte *)(puVar36 + 0x40), 0, (size_t)(local_44c - 0x200)); /* dc:92263 */
                    }
                    LVar69 = local_378;
                    memset((void *)(local_378.QuadPart + 0x5c), 0,
                           (size_t)(local_44c - 0x5c));
                    memset((void *)(uintptr_t)LVar35.QuadPart, 0, 0x4000);
                }
                memset((void *)(uintptr_t)LVar69.QuadPart, 0, 0x38);
                memset((void *)(LVar69.QuadPart + 0x48), 0, 0x14);
                memset((byte *)((int64_t)(uintptr_t)puVar36 + 0x1be), 0, 0x40);
                *(uint16_t *)((int64_t)(uintptr_t)puVar36 + 0x1fe) = 0xaa55;  /* dc:92273 */
                if ((local_460 == 0) && (0x200 < local_44c)) {
                    *(uint16_t *)(LVar69.QuadPart + -2) = 0xaa55;
                }
                memcpy((void *)(uintptr_t)LVar69.QuadPart, "EFI PART", 8);    /* dc:92277 */
                *(uint32_t *)(LVar69.QuadPart + 8) = 0x10000;
                *(uint32_t *)(LVar69.QuadPart + 0xc) = 0x5c;
                *(int64_t *)(LVar69.QuadPart + 0x18) = (int64_t)local_428.QuadPart; /* dc:92280 */
                *(LPWSTR *)(LVar69.QuadPart + 0x20) = local_388;
                *(uint64_t *)(LVar69.QuadPart + 0x28) =                       /* dc:92282 */
                    (uint64_t)((longlong)(int)local_3b0 + 1 + local_428.QuadPart);
                *(uint64_t *)(LVar69.QuadPart + 0x30) =                       /* dc:92284 */
                    (uint64_t)((longlong)(uintptr_t)local_388 + (-1 - (longlong)(int)local_3b0));
                PECMD_FixRandomSeed((int64_t)(uintptr_t)puVar36);
                plVar29 = (longlong *)(LVar69.QuadPart + 0x38);
                if (((*plVar29 == 0x202020323354ULL) || (*plVar29 == 0x202020363154ULL)) ||
                    ((*plVar29 == 0) ||
                     ((*plVar29 == 0xffffffffffffffffULL) ||
                      (*(longlong *)(LVar69.QuadPart + 0x40) == 0) ||
                      (*(longlong *)(LVar69.QuadPart + 0x40) == 0xffffffffffffffffULL)))) { /* dc:92287 */
                    memcpy((void *)(uintptr_t)plVar29, g_gptDiskGuidDef, 0xc);
                    *(int *)(LVar69.QuadPart + 0x44) = (int)puVar36[0x37];
                }
                PECMD_FixRandomSeed(LVar69.QuadPart + -0x174);
                *(uint32_t *)(LVar69.QuadPart + 0x50) = 0x80;
                *(uint32_t *)(LVar69.QuadPart + 0x54) = 0x80;
                *(uint64_t *)(LVar69.QuadPart + 0x48) = (uint64_t)(local_428.QuadPart + 1); /* dc:92297 */
                pWVar54 = (LPWSTR)(uintptr_t)local_370;
                if ((longlong)(uintptr_t)local_370 < 1) {
                    pWVar54 = (LPWSTR)-1;
                }
                lVar56 = (longlong)(int)local_448;
                iVar17 = (int)local_448 * 0x10;
                *(uint32_t *)((int64_t)(uintptr_t)puVar36 + lVar56 * 0x10 + 0x1c2) = 0xee; /* dc:92303 */
                pcVar57 = (char *)((int64_t)(iVar17 + 0x1bf) + (int64_t)(uintptr_t)puVar36);
                local_370 = (int64_t)(uintptr_t)pWVar54;
                PECMD_PackRowCol((uint64_t)local_428.QuadPart,                /* dc:92307 */
                                 (byte *)(pcVar57 + 2), pcVar57, (byte *)(pcVar57 + 1),
                                 local_3d4, local_440, 1);
                LVar69.QuadPart = 1;
                pcVar57 = (char *)((int64_t)(iVar17 + 0x1c3) + (int64_t)(uintptr_t)puVar36); /* dc:92309 */
                *pcVar57 = (char)-2;
                pcVar57[1] = (char)0xff;
                pcVar57[2] = (char)-2;
                if (local_464 == '\x02') {                                    /* dc:92313 gptmini */
                    PECMD_PackRowCol((uint64_t)(uintptr_t)pWVar54,            /* dc:92314 */
                                     (byte *)(pcVar57 + 2), pcVar57, (byte *)(pcVar57 + 1),
                                     local_3d4, local_440, 1);
                }
                *(DWORD *)((int64_t)(uintptr_t)puVar36 + lVar56 * 0x10 + 0x1c6) = /* dc:92317 */
                    local_428.LowPart;
                *(uint32_t *)((int64_t)(uintptr_t)puVar36 + lVar56 * 0x10 + 0x1ca) = 0xffffffff; /* dc:92318 */
                if (local_464 == '\x02') {
                    *(int *)((int64_t)(uintptr_t)puVar36 + lVar56 * 0x10 + 0x1ca) = /* dc:92320 */
                        (int)(uintptr_t)pWVar54;
                    LVar33 = local_428;
                    uVar49 = (ulonglong)local_448;
                    uVar18 = local_3fc;
                    goto LAB_140091a8c;
                }
            /* dc:92331-92400 -cmp 条目比对合并循环 */
LAB_140091eb6:
            LVar69 = local_2b8;                                               /* dc:92332 */
            uVar49 = 0;
            uVar20 = 0;
            local_40d = local_43c & 0x20;
            local_420[0] = 0;
            local_3d4 = (uint)local_448;
            uVar18 = 0xfffffff8;
            if (local_40d != 0) {
                uVar18 = local_3d4;
            }
            local_45c = uVar18;
            if ((local_3ea == '\x01') && ((char)local_400 != '\x01')) {       /* dc:92342 -cmp */
                local_440 = 0;
                iVar17 = local_3fc - 1;
                LVar33 = local_2b8;
                uVar31 = uVar49;
                uVar42 = uVar49;
                uVar71 = uVar49;
                if (-1 < iVar17) {
                    do {
                        uVar65 = 0x10;
                        uVar20 = (uint)PECMD_AnsiStrNCompare((char *)(uintptr_t)LVar33.QuadPart, /* dc:92350 */
                                                             (int64_t)(uintptr_t)g_gptUnusedEntry16, 0x10);
                        local_45c = (uint)uVar31;
                        local_460 = (byte)(uVar20 == local_45c);
                        uVar25 = (uint)uVar49;
                        uVar20 = (uint)uVar42;
                        if (local_460 == (char)uVar31) {
                            local_3e0 = (local_3e0 & 0xffffffff00000000ULL) | local_45c; /* dc:92356 */
                            if (uVar25 != uVar20) goto LAB_140091f69;
                        }
                        else {
LAB_140091f69:
                            local_3e0 = (local_3e0 & 0xffffffff00000000ULL) | 1; /* dc:92361 */
                        }
                        if (uVar25 == uVar18) {
                            local_45c = 2;
                            local_3d4 = uVar20;
                        }
                        if (local_460 == (char)uVar31) {
                            if (uVar25 != uVar20) {
                                puVar36 = (ulonglong *)((longlong)(int)uVar20 * 0x80 + /* dc:92369 */
                                                        LVar69.QuadPart);
                                iVar27 = memcmp(puVar36, (void *)(uintptr_t)LVar33.QuadPart, 0x38);
                                if (iVar27 != 0) {
                                    local_45c = 1;
                                    memcpy(puVar36, (void *)(uintptr_t)LVar33.QuadPart, 0x38); /* dc:92372 */
                                }
                                iVar27 = StrCmpNW((LPCWSTR)(puVar36 + 7),      /* dc:92373 名字比对 */
                                                  (LPCWSTR)(LVar33.QuadPart + 0x38), 0x24);
                                if (iVar27 != 0) {
                                    local_45c = 1;
                                    StrCpyNW((LPWSTR)(puVar36 + 7),            /* dc:92374 */
                                             (LPCWSTR)(LVar33.QuadPart + 0x38), 0x24);
                                }
                                uVar31 = 0;
                                uVar71 = (ulonglong)local_420[0];
                                uVar65 = 0x10;
                                uVar25 = local_440;
                            }
                            uVar42 = (ulonglong)(uVar20 + 1);                  /* dc:92381 */
                        }
                        if (((DWORD)local_3e0 != (int)uVar31) &&
                            (local_460 == (byte)uVar31)) {                     /* dc:92383 */
                            local_45c = 1;
                            memset((void *)(uintptr_t)LVar33.QuadPart, 0, uVar65); /* dc:92385 */
                            uVar31 = 0;
                        }
                        if (local_45c != (uint)uVar31) {                       /* dc:92388 */
                            local_420[0] = uVar25 + 1;
                            uVar71 = (ulonglong)local_420[0];
                        }
                        uVar20 = (uint)uVar71;
                        local_440 = uVar25 + 1;
                        uVar49 = (ulonglong)local_440;
                        pvVar47 = local_3a8;
                        puVar36 = (ulonglong *)(uintptr_t)local_3c0;
                        LVar33.QuadPart = LVar33.QuadPart + 0x80;
                        ptVar68 = (int64_t)(uintptr_t)local_260;
                        ptVar30 = local_290;
                        pWVar39 = local_2f8;
                    } while ((int)local_440 <= iVar17);
                }
            }
            /* dc:92402-92446 GPT 头 CRC32 + 备份头构造 */
            LVar69 = local_378;                                               /* dc:92402 */
            PECMD_MemMoveForward((byte *)(uintptr_t)local_378.QuadPart,       /* dc:92403 */
                                 (byte *)"EFI PART", 8);
            PECMD_FixRandomSeed((int64_t)(uintptr_t)puVar36);                 /* dc:92404 */
            if ((*(ulonglong *)(LVar69.QuadPart + 0x38) == 0) ||              /* dc:92405 */
                (*(ulonglong *)(LVar69.QuadPart + 0x38) == 0xffffffffffffffffULL)) {
                PECMD_FixRandomSeed(LVar69.QuadPart + -0x174);                /* dc:92407 */
            }
            if ((int)uVar20 <= (int)local_448) {                              /* dc:92409 */
                uVar20 = (int)local_448 + 1;
            }
            if ((char)local_400 == '\x01') {                                  /* dc:92412 fix */
                uVar20 = local_3fc;
            }
            uVar18 = PECMD_Crc32HexOfBytes((byte *)(uintptr_t)local_2b8.QuadPart, /* dc:92415 */
                                           (ulonglong)(local_3fc << 7), (LPSTR)0x0);
            *(uint32_t *)(LVar69.QuadPart + 0x58) = uVar18;                   /* dc:92416 条目区 CRC */
            uVar18 = *(uint32_t *)(LVar69.QuadPart + 0xc);
            local_420[0] = uVar18;
            if (0x20000 < uVar18) goto LAB_1400906d1;                         /* dc:92419 头尺寸上限 */
            *(uint32_t *)(LVar69.QuadPart + 0x10) = 0;                        /* dc:92420 */
            uVar25 = PECMD_Crc32HexOfBytes((byte *)(uintptr_t)local_378.QuadPart, /* dc:92421 */
                                           (ulonglong)uVar18, (LPSTR)0x0);
            *(uint32_t *)(local_378.QuadPart + 0x10) = uVar25;
            PECMD_MemMoveForward((byte *)(uintptr_t)local_3b8.QuadPart,       /* dc:92423 备份头 */
                                 (byte *)(uintptr_t)local_378.QuadPart, (int)uVar18);
            *(ulonglong *)(local_3b8.QuadPart + 0x18) =                       /* dc:92424 */
                *(ulonglong *)(local_378.QuadPart + 0x20);
            *(ulonglong *)(local_3b8.QuadPart + 0x20) =                       /* dc:92425 */
                *(ulonglong *)(local_378.QuadPart + 0x18);
            *(uint32_t *)(local_3b8.QuadPart + 0x10) = 0;                     /* dc:92426 */
            uVar18 = PECMD_Crc32HexOfBytes((byte *)(uintptr_t)local_3b8.QuadPart, /* dc:92427 */
                                           (ulonglong)uVar18, (LPSTR)0x0);
            *(uint32_t *)(local_3b8.QuadPart + 0x10) = uVar18;
            FUN_1400633A8(&local_390,                                         /* dc:92429 */
                          (int64_t)((int)(local_3b0 + 1) * (longlong)(int)local_44c) &
                          0xffffffff);
            ptVar44 = (int64_t)(uintptr_t)local_390;
            uVar18 = (uVar20 * 0x80 + -1 + local_44c) / local_44c;            /* dc:92432 */
            lVar56 = (longlong)(int)local_44c;
            PECMD_SetFilePointer(pvVar47, local_350, 0);
            ReadFile(pvVar47, (WCHAR *)(uintptr_t)ptVar44, local_44c, &local_15c, /* dc:92435 */
                     NULL);
            iVar17 = memcmp((void *)(uintptr_t)ptVar44,                       /* dc:92436 */
                            (void *)(uintptr_t)puVar36, (size_t)local_328);
            if (iVar17 != 0) {
                PECMD_SetFilePointer(pvVar47, local_350, 0);
                WriteFile(pvVar47, (WCHAR *)(uintptr_t)puVar36, local_44c, &local_158, /* dc:92439 */
                          NULL);
            }
            LVar69 = local_378;                                               /* dc:92441 */
            iVar17 = (int)(((int)uVar18 * lVar56 & 0xffffffffU) / (ulonglong)local_44c) + 1; /* dc:92442 */
            local_3e0 = (local_3e0 & 0xffffffff00000000ULL) | (uint32_t)iVar17;
            local_278.LowPart = local_2f0.LowPart;                            /* dc:92444 */
            PECMD_SetFilePointer(pvVar47,                                     /* dc:92445 */
                                 LI64(local_328 * local_2f0.QuadPart +
                                                 local_350.QuadPart), 0);
            ReadFile(pvVar47, (WCHAR *)(uintptr_t)ptVar44, iVar17 * local_44c, /* dc:92446 */
                     local_154, NULL);
            /* dc:92447-92513 备份表镜像写回双循环 */
            iVar27 = (DWORD)local_3e0;                                        /* dc:92447 */
            iVar17 = 0;
            ptVar53 = (WCHAR *)(uintptr_t)ptVar44;                            /* dc:92449 */
            if (0 < (int)(DWORD)local_3e0) {
                LVar33 = local_278;
                _Size = local_328;
                if ((int)(DWORD)local_3e0 < 1) goto LAB_140092334;
LAB_140092302:
                do {
                    iVar26 = memcmp((void *)(uintptr_t)ptVar44,               /* dc:92456 */
                                    (void *)(uintptr_t)LVar69.QuadPart, _Size);
                    pvVar47 = local_3a8;
                    if (iVar26 == 0) {
                        iVar17 = iVar17 + 1;
                        LVar33.QuadPart = LVar33.QuadPart + 1;
                        LVar69.QuadPart = LVar69.QuadPart + _Size;
                        ptVar44 = (int64_t)((byte *)(uintptr_t)ptVar44 + _Size); /* dc:92462 mbulinfo+k-0xc */
                        if (iVar17 < iVar27) goto LAB_140092302;
                    }
LAB_140092334:
                    uVar49 = local_328;                                       /* dc:92466 */
                    ptVar53 = local_390;
                    ptVar68 = (int64_t)(uintptr_t)local_260;
                    ptVar30 = local_290;
                    pWVar39 = local_2f8;
                    LVar35 = LVar33;
                    local_320 = LVar69;
                    if ((int)local_3b0 <= iVar17) break;
                    while ((local_278 = LVar35, iVar17 < iVar27 &&            /* dc:92473 */
                            (iVar26 = memcmp((void *)(uintptr_t)ptVar44,
                                             (void *)(uintptr_t)LVar69.QuadPart, (size_t)uVar49),
                             pvVar47 = local_3a8, iVar26 != 0))) {
                        iVar17 = iVar17 + 1;
                        LVar33.QuadPart = LVar33.QuadPart + 1;
                        LVar69.QuadPart = LVar69.QuadPart + uVar49;
                        ptVar44 = (int64_t)((byte *)(uintptr_t)ptVar44 + uVar49);
                        LVar35 = local_278;
                    }
                    PECMD_SetFilePointer(pvVar47,                             /* dc:92481 */
                                         LI64(local_278.QuadPart * local_328 +
                                                         local_350.QuadPart), 0);
                    WriteFile(pvVar47, (LPCWSTR)(uintptr_t)local_320.QuadPart, /* dc:92483 */
                              LVar69.LowPart - local_320.LowPart, &local_160,
                              NULL);
                    ptVar53 = local_390;
                    ptVar68 = (int64_t)(uintptr_t)local_260;
                    ptVar30 = local_290;
                    pWVar39 = local_2f8;
                    _Size = local_328;
                } while (iVar17 < iVar27);
            }
            /* dc:92510-92561 主表镜像写回双循环 */
            LVar69 = local_2b8;                                               /* dc:92510 */
            DVar23 = local_3b0;
            local_320.QuadPart = (longlong)(uintptr_t)local_388 - (longlong)(int)local_3b0; /* dc:92512 */
            local_258.QuadPart = local_320.QuadPart;
            PECMD_SetFilePointer(pvVar47,                                     /* dc:92514 */
                                 LI64(local_320.QuadPart * local_328 +
                                                 local_350.QuadPart), 0);
            ReadFile(pvVar47, (WCHAR *)(uintptr_t)ptVar53, DVar23 * local_44c, /* dc:92516 */
                     local_1d8, NULL);
            uVar49 = local_328;                                               /* dc:92517 */
            iVar17 = 0;
            ptVar44 = (int64_t)(uintptr_t)ptVar53;
            LVar33 = local_320;
            DVar23 = local_3b0;
            if (0 < (int)local_3b0) {
                while (DVar24 = local_3b0,                                    /* dc:92523 */
                      ptVar44 = (int64_t)(uintptr_t)local_390,
                      ptVar68 = (int64_t)(uintptr_t)local_260,
                      ptVar30 = local_290, pWVar39 = local_2f8,
                      iVar17 < (int)DVar23) {
                    do {
                        iVar27 = memcmp((void *)(uintptr_t)ptVar53,           /* dc:92526 */
                                        (void *)(uintptr_t)LVar69.QuadPart, (size_t)uVar49);
                        if (iVar27 != 0) break;
                        iVar17 = iVar17 + 1;
                        LVar33.QuadPart = LVar33.QuadPart + 1;
                        LVar69.QuadPart = LVar69.QuadPart + uVar49;
                        ptVar53 = (WCHAR *)((byte *)(uintptr_t)ptVar53 + uVar49); /* dc:92531 */
                    } while (iVar17 < (int)DVar24);
                    pvVar47 = local_3a8;                                      /* dc:92533 */
                    ptVar44 = (int64_t)(uintptr_t)local_390;
                    ptVar68 = (int64_t)(uintptr_t)local_260;
                    ptVar30 = local_290;
                    pWVar39 = local_2f8;
                    LVar35 = LVar69;
                    LVar51 = LVar33;
                    if ((int)DVar24 <= iVar17) break;
                    do {
                        local_278 = LVar51;                                   /* dc:92542 */
                        local_320 = LVar35;
                        iVar27 = memcmp((void *)(uintptr_t)ptVar53,
                                        (void *)(uintptr_t)LVar69.QuadPart, (size_t)uVar49);
                        pvVar47 = local_3a8;
                        if (iVar27 == 0) break;
                        iVar17 = iVar17 + 1;
                        LVar33.QuadPart = LVar33.QuadPart + 1;
                        LVar69.QuadPart = LVar69.QuadPart + uVar49;
                        ptVar53 = (WCHAR *)((byte *)(uintptr_t)ptVar53 + uVar49); /* dc:92550 */
                        LVar35 = local_320;
                        LVar51 = local_278;
                    } while (iVar17 < (int)DVar24);
                    PECMD_SetFilePointer(local_3a8,                           /* dc:92554 */
                                         LI64(local_278.QuadPart * uVar49 +
                                                         local_350.QuadPart), 0);
                    WriteFile(pvVar47, (LPCWSTR)(uintptr_t)local_320.QuadPart, /* dc:92557 */
                              LVar69.LowPart - local_320.LowPart, &local_1c0,
                              NULL);
                    DVar23 = DVar24;
                }
            }
            /* dc:92562-92588 备份头写回 + 旗标出口 */
            uVar49 = local_328;                                               /* dc:92562 */
            LVar33.QuadPart = local_328 * (longlong)(uintptr_t)local_388 +    /* dc:92563 */
                              local_350.QuadPart;
            PECMD_SetFilePointer(pvVar47, LVar33, 0);
            ReadFile(pvVar47, (WCHAR *)(uintptr_t)ptVar44, local_44c, local_1b8, /* dc:92565 */
                     NULL);
            LVar69 = local_3b8;                                               /* dc:92566 */
            iVar17 = memcmp((void *)(uintptr_t)ptVar44,                       /* dc:92567 */
                            (void *)(uintptr_t)local_3b8.QuadPart, (size_t)uVar49);
            uVar18 = local_44c;
            if (iVar17 != 0) {
                PECMD_SetFilePointer(pvVar47, LVar33, 0);
                uVar18 = local_44c;
                WriteFile(pvVar47, (LPCWSTR)(uintptr_t)LVar69.QuadPart, local_44c, /* dc:92572 */
                          &local_1bc, NULL);
            }
            ptVar44 = (int64_t)(uintptr_t)local_330;                          /* dc:92574 */
            uVar31 = (ulonglong)uVar18;
            uVar49 = 0;
            if (local_40d == 0) {                                             /* dc:92577 */
LAB_1400926ff:
                if ((local_465 != 0 || local_468 != 0) || local_467 != 0) {   /* dc:92579 */
                    PECMD_FreeStrBuf(&local_390);
                    PECMD_FreeStrBuf(&local_3c0);
                    PECMD_FreeStrBuf(&local_3d0);
                    PECMD_FreeStrBuf(&local_458.QuadPart);
                    uVar18 = local_42c;
                    bVar52 = local_465;
                    iVar17 = local_404;
                    goto LAB_14008f48e;
                }
            }
            else if ((local_465 != 0 || local_468 != 0) || local_467 != 0) {  /* dc:92590 */
                if (-1 < (longlong)(int64_t)(uintptr_t)local_360) {
                    lVar34 = (longlong)(int)local_3d4;                        /* dc:92592 */
                    PECMD_CloseDeviceHandle(&local_3a8,                   /* dc:92593 */
                                             (int)(int64_t)(uintptr_t)local_360,
                                             (byte)local_42c,
                                             (byte *)&local_res20,
                                             (uint64_t *)&local_2b0.QuadPart,
                                             (LPCWSTR)(uintptr_t)local_330);
                    lVar56 = 1;
                    PECMD_OpenFileHandle(&local_3a8, (LPCWSTR)(uintptr_t)ptVar44, 0xc0000000, 3, /* dc:92596 */
                                         (LPSECURITY_ATTRIBUTES)0x0, 3, 0x20000000, (HANDLE)0x0);
                    LVar51 = local_2b8;
                    LVar33 = local_378;
                    ((ulonglong *)(local_2b8.QuadPart + 0x30))[lVar34 * 0x10] = /* dc:92600 清属性最高位 */
                        ((ulonglong *)(local_2b8.QuadPart + 0x30))[lVar34 * 0x10] &
                        0x7fffffffffffffffULL;
                    *(uint32_t *)(local_378.QuadPart + 0x58) = 0;             /* dc:92603 */
                    uVar18 = PECMD_Crc32HexOfBytes((byte *)(uintptr_t)local_2b8.QuadPart, /* dc:92604 */
                                                   (ulonglong)(local_3fc << 7), (LPSTR)0x0);
                    *(uint32_t *)(LVar33.QuadPart + 0x58) = uVar18;
                    *(uint32_t *)(LVar33.QuadPart + 0x10) = 0;
                    uVar49 = (ulonglong)local_420[0];
                    uVar18 = PECMD_Crc32HexOfBytes((byte *)(uintptr_t)LVar33.QuadPart, /* dc:92609 */
                                                   uVar49, (LPSTR)0x0);
                    *(uint32_t *)(LVar33.QuadPart + 0x10) = uVar18;
                    *(uint32_t *)(LVar69.QuadPart + 0x10) = 0;
                    uVar18 = PECMD_Crc32HexOfBytes((byte *)(uintptr_t)LVar69.QuadPart, /* dc:92612 */
                                                   uVar49, (LPSTR)0x0);
                    LVar35 = local_2f0;
                    pvVar47 = local_3a8;
                    *(uint32_t *)(LVar69.QuadPart + 0x10) = uVar18;
                    PECMD_SetFilePointer(local_3a8,                           /* dc:92616 */
                                         LI64(uVar31 * local_2f0.QuadPart +
                                                         local_350.QuadPart), 0);
                    WriteFile(pvVar47, (LPCWSTR)(uintptr_t)LVar33.QuadPart, local_44c, /* dc:92619 */
                              &local_1bc, NULL);
                    iVar17 = (int)((ulonglong)(lVar34 * 0x80) / uVar31);      /* dc:92620 */
                    PECMD_SetFilePointer(pvVar47,                             /* dc:92621 */
                                         LI64((longlong)((byte *)(uintptr_t)LVar35.QuadPart + 1) *
                                                         uVar31 + local_350.QuadPart), 0);
                    uVar18 = local_44c;
                    lpBuffer_00 = (const void *)(((longlong)iVar17 * (longlong)(int)local_44c &
                                                 0xffffffffU) + LVar51.QuadPart); /* dc:92625 */
                    WriteFile(pvVar47, lpBuffer_00, local_44c, local_1b8, NULL); /* dc:92627 */
                    LVar69 = local_350;                                       /* dc:92628 */
                    PECMD_SetFilePointer(pvVar47,                             /* dc:92629 */
                                         LI64((longlong)(local_258.QuadPart + iVar17) *
                                                         uVar31 + local_350.QuadPart), 0);
                    WriteFile(pvVar47, lpBuffer_00, uVar18, &local_1c0, NULL); /* dc:92632 */
                    PECMD_SetFilePointer(pvVar47,                             /* dc:92633 */
                                         LI64(LVar69.QuadPart +
                                                         uVar31 * (longlong)(uintptr_t)local_388), 0);
                    WriteFile(pvVar47, (WCHAR *)(LVar51.QuadPart + 0x10000), uVar18, /* dc:92635 */
                              local_1d8, NULL);
                    PECMD_FlushDeviceIoctl(pvVar47);                          /* dc:92637 */
                    iVar27 = 0;                                               /* dc:92638 */
                    if (-1 < (int)lVar56) {
                        iVar27 = (int)lVar56;
                    }
                    PECMD_FreeStrBuf(&local_390);                             /* dc:92642 */
                    PECMD_FreeStrBuf(&local_3c0);
                    PECMD_FreeStrBuf(&local_3d0);
                    PECMD_FreeStrBuf(&local_458.QuadPart);
                    PECMD_ClearFlagAndError((byte *)&local_res20);
                    goto joined_r0x000140092fff;
                }
                goto LAB_1400926ff;
            }
            PECMD_FreeStrBuf(&local_390);                                     /* dc:92651 */
            PECMD_FreeStrBuf(&local_3c0);
            PECMD_FreeStrBuf(&local_3d0);
            PECMD_FreeStrBuf(&local_458.QuadPart);
            goto LAB_140094390;
        }   /* dc:92656 闭 else-if(92590) */
        /* dc:92657-92684 条目槽定位 (+L 前缀回收空闲槽) */
LAB_140091a8c:
        LVar35 = local_2b8;                                                   /* dc:92658 */
        uVar31 = 0;
        if (('\0' < (char)local_400) || (('\0' < local_3ea && ((int)uVar49 < 0)))) /* dc:92660 */
            goto LAB_140091eb6;
        if (local_462 == '\0') {                                              /* dc:92662 */
LAB_140091b0b:
            uVar49 = (ulonglong)local_310;
        }
        else {
            uVar32 = 0;
            uVar49 = LVar33.QuadPart & 0xffffffffffffff00ULL;                 /* dc:92668 */
            local_310 = (uint)uVar49;
            local_448 = (local_448 & 0xffffffff00000000ULL) | 0xffffffff;     /* dc:92670 */
            if (uVar18 != 0) {
                do {
                    iVar17 = PECMD_AnsiStrNCompare(                           /* dc:92673 */
                                (char *)(LVar35.QuadPart + (int64_t)uVar31 * 0x80),
                                (int64_t)(uintptr_t)g_gptUnusedEntry16, 0x10);
                    if (iVar17 == (int)uVar32) {
                        local_448 = (local_448 & 0xffffffff00000000ULL) | (uint32_t)uVar31; /* dc:92675 */
                        memset((void *)(LVar35.QuadPart + (uVar31 & 0xffffffff) * 0x80), /* dc:92676 */
                               0, 0x80);
                        break;
                    }
                    uVar20 = (int)uVar31 + LVar69.LowPart;                    /* dc:92679 */
                    uVar31 = (ulonglong)uVar20;
                } while (uVar20 < uVar18);
                goto LAB_140091b0b;
            }
        }
        uVar31 = 0;                                                           /* dc:92685 _UNK_140124108 = 0 */
        uVar42 = 0;
        local_398 = local_398 - 1;                                            /* dc:92687 */
        if ((uVar18 <= (uint)local_448) || ((int)uVar18 <= (int)local_398))   /* dc:92690 */
            goto LAB_1400906d1;
        if ((char)uVar49 == '\0') {                                           /* dc:92692 */
            iVar17 = (int)local_3e0;
LAB_140091d3a:
            ptVar44 = local_1e8;                                              /* dc:92693 名字串 */
            iVar27 = 1;
            if ((char)local_400 != '\x01') {                                  /* dc:92695 非 fix */
                uVar18 = (uint)local_448;
                if ((int)local_398 < 0) {                                     /* dc:92697 */
                    if (iVar17 == 0) {
                        local_220 = 1;                                        /* dc:92699 */
                        local_210 = 1;
                        local_318 = 1;
                        local_1c8 = 1;
                        local_230 = 1;
                        local_2d0[0] = 1;
                    }
                    else {
                        iVar27 = (int)(int64_t)(uintptr_t)local_1c8;
                    }
                    if (0 < iVar27) {                                         /* dc:92710 类型 GUID */
                        puVar60 = (ulonglong *)(LVar35.QuadPart +
                                                (longlong)(int)uVar18 * 0x80);
                        *(uint32_t *)(uintptr_t)puVar60 = uVar4;              /* dc:92712 */
                        *(uint32_t *)((int64_t)(uintptr_t)puVar60 + 4) = uVar5;
                        *(uint32_t *)((int64_t)(uintptr_t)puVar60 + 8) = uVar6;
                        *(uint32_t *)((int64_t)(uintptr_t)puVar60 + 0xc) = uVar7;
                    }
                    if (0 < (int)(int64_t)(uintptr_t)local_318) {             /* dc:92717 唯一 GUID */
                        pDVar1 = (DWORD *)(LVar35.QuadPart +
                                           (longlong)(int)uVar18 * 0x80 + 0x10);
                        pDVar1[0] = uVar8;
                        pDVar1[1] = uVar9;
                        pDVar1[2] = uVar10;
                        pDVar1[3] = uVar11;
                    }
                    if (0 < local_210) {                                      /* dc:92724 FirstLBA */
                        *(LARGE_INTEGER *)(LVar35.QuadPart +
                                           (longlong)(int)uVar18 * 0x80 + 0x20) = local_428;
                    }
                    if (0 < local_220) {                                      /* dc:92727 LastLBA */
                        *(LPWSTR *)(LVar35.QuadPart +
                                    (longlong)(int)uVar18 * 0x80 + 0x28) =    /* dc:92728 */
                            (LPWSTR)(uintptr_t)local_370;
                    }
                    if (0 < local_230) {                                      /* dc:92730 属性 */
                        *(LARGE_INTEGER *)(LVar35.QuadPart +
                                           (longlong)(int)uVar18 * 0x80 + 0x30) = local_238;
                    }
                    if ((0 < local_2d0[0]) &&
                        (*(short *)(uintptr_t)local_1e8 != 0)) {              /* dc:92733 名字串 */
                        pWVar54 = (LPWSTR)(LVar35.QuadPart +
                                           (longlong)(int)uVar18 * 0x80 + 0x38); /* dc:92734 */
                        memset(pWVar54, 0, 0x48);
                        StrCpyNW(pWVar54, (LPCWSTR)(uintptr_t)ptVar44, 0x24);
                    }
                }
                else if (local_398 != uVar18) {                               /* dc:92739 条目交换 */
                    _Src = (ulonglong *)(LVar35.QuadPart +
                                         (longlong)(int)uVar18 * 0x80);
                    memcpy(local_148 + 0x70, _Src, 0x80);                     /* dc:92741 */
                    puVar60 = (ulonglong *)(LVar35.QuadPart +
                                            (longlong)(int)local_398 * 0x80);
                    memcpy(_Src, puVar60, 0x80);
                    memcpy(puVar60, local_148 + 0x70, 0x80);
                    if ((int)local_448 < (int)local_398) {
                        local_448 = (ulonglong)local_398;                     /* dc:92745 */
                    }
                    local_448 = local_448 & 0xffffffff;
                }
            }
            if ((char)local_340 != '\0') {                                    /* dc:92751 del */
                memset((void *)(local_2b8.QuadPart +
                                (longlong)(int)local_448 * 0x80), 0, 0x80);
            }
            goto LAB_140091eb6;
        }
        /* dc:92756-92815 FirstLBA 冲突清理 + 空槽位移 */
        if (local_466 != 0) {                                                 /* dc:92756 GPT 模式 */
            puVar60 = (ulonglong *)(LVar35.QuadPart + 0x20);                  /* dc:92757 */
            while (uVar20 = (uint)uVar42, uVar20 < uVar18) {
                if ((ulonglong)*puVar60 == (ulonglong)local_428.QuadPart) {   /* dc:92759 */
                    puVar60 = (ulonglong *)(LVar35.QuadPart + (int64_t)uVar42 * 0x80); /* dc:92760 */
                    *puVar60 = 0;                                             /* dc:92761 GUID 前 8B 清 */
                    puVar60[1] = uVar31;                                      /* dc:92762 */
                    if (uVar20 != (uint)local_448) {
                        memset(puVar60, 0, 0x80);                             /* dc:92764 */
                    }
                    break;
                }
                puVar60 = puVar60 + 0x10;
                uVar42 = (ulonglong)(uVar20 + 1);
            }
        }
        uVar32 = 1;                                                           /* dc:92772 */
        iVar27 = (int)local_448;
        LVar69.QuadPart = (longlong)iVar27 * 0x80 + LVar35.QuadPart;          /* dc:92774 */
        local_278.QuadPart = LVar69.QuadPart;
        iVar17 = PECMD_AnsiStrNCompare((char *)(uintptr_t)LVar69.QuadPart,    /* dc:92776 */
                                       (int64_t)(uintptr_t)g_gptUnusedEntry16, 0x10);
        if (iVar17 == 0) {
LAB_140091d04:
            memset((void *)(uintptr_t)LVar69.QuadPart, 0, 0x80);              /* dc:92779 */
            iVar17 = 0;
            goto LAB_140091d3a;
        }
        uVar31 = (ulonglong)(iVar27 + 1U);                                    /* dc:92783 */
        uVar49 = (ulonglong)local_3fc;
        if (iVar27 + 1U < local_3fc) {                                        /* dc:92785 */
            do {
                iVar27 = (int)uVar31;
                iVar17 = PECMD_AnsiStrNCompare(                               /* dc:92788 */
                            (char *)(LVar35.QuadPart + (int64_t)uVar31 * 0x80),
                            (int64_t)(uintptr_t)g_gptUnusedEntry16, 0x10);
                uVar18 = (uint)uVar49;
                if (iVar17 == 0) {
                    if ((int)local_448 < iVar27) {                            /* dc:92791 */
                        uVar49 = (ulonglong)(uint)(iVar27 - (int)local_448);
                        puVar60 = (ulonglong *)(LVar35.QuadPart +
                                                (longlong)iVar27 * 0x80);     /* dc:92793 */
                        do {
                            memcpy(puVar60, puVar60 + -0x10, 0x80);           /* dc:92795 条目后移 */
                            uVar49 = uVar49 - 1;
                            puVar60 = puVar60 + -0x10;
                            LVar35 = local_2b8;
                            LVar69 = local_278;
                            ptVar30 = local_290;
                            pWVar39 = local_2f8;
                            uVar18 = local_3fc;
                        } while (uVar49 != 0);
                    }
                    break;
                }
                uVar20 = iVar27 + (int)uVar32;                                /* dc:92807 */
                uVar31 = (ulonglong)uVar20;
            } while (uVar20 < uVar18);
            if ((uint)uVar31 < uVar18) goto LAB_140091d04;                    /* dc:92810 */
        }
LAB_1400906d1:                                                                /* dc:92812 */
        PECMD_FreeStrBuf(&local_3c0);
        ppuVar41 = &local_3d0;
        goto LAB_14008fdb7;
    }
    /* ================= dc:92817-92953 对齐计算 + MBR 条目打包 ================= */
    local_466 = 0;                                                            /* dc:92817 */
    local_3b8.QuadPart = LVar33.QuadPart;
    if (((char)local_340 == '\0') &&
        (((-1 < local_428.QuadPart || (-1 < local_3f8.QuadPart)) &&
          (-1 < (int)local_448)))) {                                          /* dc:92819 */
        FUN_1400633A8(&local_308, 0x100000);                                  /* dc:92822 */
        iVar17 = (int)(local_45c * local_3d4 +
                       ((int)(local_45c * local_3d4) >> 0x1f & 0x1ffU)) >> 9; /* dc:92823 */
        lVar56 = (longlong)(int)(iVar17 * local_440);
        if ((int)local_45c < 1) {
LAB_140092b02:
            PECMD_FreeStrBuf(&local_308);
            goto LAB_1400906d1;
        }
        if (*ptVar30 != L'\0') {                                              /* dc:92831 */
LAB_14009099e:
            uVar18 = local_420[0];
            lVar34 = 0;
            uVar20 = local_45c;
            uVar25 = local_3d4;
            uVar21 = local_440;
            if (0 < local_428.QuadPart) {
                if (local_248 == -1) {                                        /* dc:92839 */
                    if ((local_428.QuadPart < lVar56) ||
                        (lVar34 = lVar56, local_3d6 != 0)) {                  /* dc:92840 */
                        lVar34 = (longlong)iVar17;
                    }
                    lVar34 = PECMD_AlignRemainder(local_428.QuadPart, lVar34); /* dc:92843 */
                    local_428.QuadPart = local_428.QuadPart + lVar34;
                    uVar18 = local_420[0];
                }
                else if (local_3ec == '\0') {                                 /* dc:92847 */
                    /* dc:92849 mbulinfo 偏移抵消: local_248 + local_45c - 1 */
                    local_248 = ((local_248 + (longlong)(int)local_45c + -1) /
                                 (longlong)(int)local_45c) * (longlong)(int)local_420[0];
                    lVar34 = PECMD_AlignRemainder(local_428.QuadPart, local_248); /* dc:92851 */
                    local_428.QuadPart = local_428.QuadPart + lVar34;
                }
            }
            LVar69 = local_288;                                               /* dc:92855 */
            if (((0 < local_3f8.QuadPart) && (local_39b == '\0')) &&
                (local_3f8.QuadPart = local_3f8.QuadPart - lVar34,
                 local_3f8.QuadPart < 0)) {                                   /* dc:92856 */
                local_3f8.QuadPart = 0;
            }
            LVar33.QuadPart = 0;                                              /* dc:92860 */
            if (0 < local_428.QuadPart) {
                LVar33 = local_428;
            }
            if (0 < local_3f8.QuadPart) {                                     /* dc:92864 */
                if (local_228 == -1) {
                    lVar56 = (LVar33.QuadPart + local_3f8.QuadPart) % lVar56; /* dc:92866 */
                }
                else {
                    if (local_3ec != '\0') goto LAB_140090aff;
                    lVar56 = (longlong)(int)uVar20;
                    /* dc:92871 mbulinfo 偏移抵消: 向上取整到 local_45c 倍数 */
                    lVar56 = ((local_228 + lVar56 + -1) / lVar56) * lVar56;
                    local_228 = (int64_t)((lVar56 +                                    /* dc:92873 */
                                           (ulonglong)((uint)(lVar56 >> 0x3f) & 0x1ffU)) >> 9);
                    lVar56 = (LVar33.QuadPart + local_3f8.QuadPart) % local_228; /* dc:92875 */
                }
                local_3f8.QuadPart = local_3f8.QuadPart - lVar56;             /* dc:92878 */
            }
LAB_140090aff:                                                                /* dc:92880 */
            local_466 = 0;
            local_40e = '\0';
            local_44f[0] = 0;
            if (0 < (longlong)(uintptr_t)local_2a8) {                         /* dc:92884 扩展分区条目 */
                local_2a8 = (ushort *)                                        /* dc:92885 */
                    ((local_3f8.QuadPart - (longlong)(int64_t)(uintptr_t)local_288.QuadPart +
                      local_428.QuadPart) / (longlong)(int)uVar18);
                local_2e8 = 0;                                                /* dc:92889 */
                LStack_2e0 = 0;
                PECMD_PackRowCol((uint64_t)local_258.QuadPart, (uint8_t *)&local_466, /* dc:92891 */
                                 (char *)&local_40e, (uint8_t *)local_44f,
                                 uVar25, uVar21, 1);
                local_2e8 = ((local_2e8 & 0xffffffff00000000ULL) |            /* dc:92893 CONCAT 链 */
                             (((uint64_t)((uint32_t)local_466 << 16 |
                                          ((uint32_t)local_44f[0] << 8) |
                                          (uint32_t)(uint8_t)local_40e)) << 8));
                PECMD_PackRowCol((uint64_t)(local_258.QuadPart * 2 - 1),      /* dc:92897 */
                                 (uint8_t *)&local_466, (char *)&local_40e,
                                 (uint8_t *)local_44f, local_3d4, local_440, 1);
                ((byte *)&local_2e8)[4] = 5;                                 /* dc:92899 extended */
                ((byte *)&local_2e8)[5] = (uint8_t)local_40e;                  /* dc:92900 */
                ((byte *)&local_2e8)[6] = local_44f[0];                        /* dc:92901 */
                ((byte *)&local_2e8)[7] = (uint8_t)local_466;                  /* dc:92902 */
                LStack_2e0 = (LStack_2e0 & 0xffffffffULL) |                   /* dc:92903 尺寸 */
                              ((uint64_t)(uint32_t)(int)(uintptr_t)local_2a8 << 32);
                LStack_2e0 = (LStack_2e0 & 0xffffffff00000000ULL) |           /* dc:92904 LBA 起 */
                              (uint64_t)local_258.LowPart;
                PECMD_WriteMbrPartitionEntry(pvVar47, local_320.QuadPart,     /* dc:92905 */
                                             (uint8_t *)&local_2e8, 1);
            }
            uVar20 = local_3d4;                                               /* dc:92907 */
            DVar23 = local_440;
            lVar34 = (longlong)(int)uVar18;
            lVar56 = LVar69.QuadPart / lVar34;
            if (local_428.QuadPart < 0) {                                     /* dc:92911 */
                local_370 = 0;
            }
            else {
                local_370 = local_428.QuadPart / lVar34;                      /* dc:92915 */
            }
            if (local_3f8.QuadPart < 0) {                                     /* dc:92917 */
                local_3e0 = 0;
            }
            else {
                local_3e0 = local_3f8.QuadPart / lVar34;                      /* dc:92921 */
            }
            PECMD_PackRowCol((uint64_t)(uint64_t)(uintptr_t)local_370,        /* dc:92923 */
                             (uint8_t *)&local_3b0, (char *)&local_40d,
                             (uint8_t *)&local_460, local_3d4, local_440,
                             (uint)(3 < (int)local_448));
            if (0 < local_428.QuadPart) {                                     /* dc:92925 */
                local_370 = local_370 - lVar56;
            }
            local_466 = local_3d6;                                            /* dc:92928 */
            PECMD_PackRowCol((uint64_t)(uint32_t)((int)local_3e0 + -1 + (int)local_370), /* dc:92929 */
                             (uint8_t *)local_298, (char *)&local_3d7,
                             (uint8_t *)&local_3d8, uVar20, DVar23,
                             (int)(char)local_3d6);
            PECMD_FreeStrBuf(&local_308);                                     /* dc:92932 */
            goto LAB_140090d82;
        }
        local_3e0 = local_3e0 & 0xffffffff00000000ULL;                        /* dc:92935 */
        puVar36 = FUN_14005FEAC(pvVar47, (uint64_t *)(uintptr_t)local_308,    /* dc:92936 */
                                (uint32_t *)&local_3e0);
        if (puVar36 != (ulonglong *)0x0) {
            if (((3 < (int)local_448) && (local_462 == '\0')) &&
                ((int)(DWORD)local_3e0 <= (int)local_448)) goto LAB_140092b02; /* dc:92938 */
            goto LAB_14009099e;
        }
        PECMD_FreeStrBuf(&local_308);                                         /* dc:92942 */
LAB_140092ae8:                                                                /* dc:92943 */
        PECMD_FreeStrBuf(&local_3c0);
        ppuVar41 = &local_3d0;
        goto LAB_140092a1c;
    }
    /* ================= dc:92954-93045 MBR 条目写入引擎 ================= */
LAB_140090d82:
    iVar17 = local_3fc * 0x10;                                                /* dc:92955 */
    uVar49 = (ulonglong)iVar17;
    if ((char)local_3e8 != '\x02') {                                          /* dc:92957 非 part */
        PECMD_MemMoveForward((byte *)local_2c8, (byte *)pWVar54, (int)local_328); /* dc:92958 */
        if (-1 < (longlong)(uintptr_t)local_2a8) {                            /* dc:92960 */
            if (0 < (int)local_3fc) {
                memset((byte *)(pWVar54 + 0xdf), 0, (size_t)uVar49);          /* dc:92962 */
            }
            memset((byte *)((int64_t)(iVar17 + 0x10) + 0x1be + (int64_t)(uintptr_t)pWVar54), /* dc:92965 */
                   0, (size_t)(int)((3 - local_3fc) * 0x10));
        }
        LVar69 = local_428;                                                   /* dc:92969 */
        cVar12 = *(char *)((int64_t)(uintptr_t)pWVar54 + uVar49 + 0x1c2);     /* dc:92970 类型字节 */
        if (((local_462 == '\0') && ((char)local_45f < '\x02')) &&
            ((cVar12 == '\x05' || (cVar12 == '\x0f'))))                /* dc:92968 扩展分区 */
            goto LAB_1400906d1;
        uVar18 = *(uint32_t *)((int64_t)(uintptr_t)pWVar54 + uVar49 + 0x1c6); /* dc:92973 */
        uVar20 = *(uint32_t *)((int64_t)(uintptr_t)pWVar54 + uVar49 + 0x1ca); /* dc:92974 */
        if (((((local_462 != '\0') || (cVar12 == '\0')) || (local_3c8 == 0)) ||
             (('\x01' < (char)local_45f || (local_3d6 != 0)))) ||
             (((local_428.QuadPart < 0 ||
                (local_370 == (int64_t)uVar18)) &&
               (((local_3f8.QuadPart < 0 || (local_3e0 == uVar20)) || (uVar20 == 0)))))) { /* dc:92975 */
            pWVar64 = (LPWSTR)(uintptr_t)local_370;                           /* dc:92979 */
            if ((char)local_340 != '\0') {                                    /* dc:92980 del */
                local_3e0 = 0;
                pWVar64 = (LPWSTR)0x0;
                uVar18 = 0;
                uVar20 = 0;
            }
            iVar17 = (int)local_3e0;                                          /* dc:92986 */
            iVar27 = (int)(int64_t)(uintptr_t)pWVar64;
            if (-1 < (int)local_3c8) {                                        /* dc:92988 */
                *(char *)((int64_t)(uintptr_t)pWVar54 + uVar49 + 0x1c2) = (char)local_3c8;
            }
            sVar16 = (short)local_380;                                        /* dc:92991 */
            sVar2 = sVar16 - (short)local_1a8;
            if (sVar2 != 0 && (short)local_1a8 <= sVar16) {
                if (local_358[0] != 0) {                                      /* dc:92994 '*' 通配 */
                    *(byte *)((byte *)(uintptr_t)pWVar54 + 0x1ee) = 0;        /* dc:92995 */
                    *(byte *)((byte *)(uintptr_t)pWVar54 + 0x1de) = 0;        /* dc:92996 */
                    *(byte *)((byte *)(uintptr_t)pWVar54 + 0x1ce) = 0;        /* dc:92997 */
                    *(byte *)((byte *)(uintptr_t)pWVar54 + 0x1be) = 0;        /* dc:92998 */
                }
                sVar2 = sVar16 - (short)local_1a8;
            }
            if (SBORROW2(sVar16, (short)local_1a8) == (sVar2 < 0)) {          /* dc:93002 */
                *(char *)((int64_t)(uintptr_t)pWVar54 + uVar49 + 0x1be) = (char)local_380;
            }
            if ((longlong)local_428.QuadPart < 0) {                           /* dc:93005 */
                if (-1 < local_3f8.QuadPart) {
                    uVar18 = iVar17 + -1 + uVar18;                            /* dc:93007 */
LAB_140090f5a:
                    PECMD_PackRowCol((uint64_t)uVar18, (uint8_t *)local_298,  /* dc:93009 */
                                     (char *)&local_3d7, (uint8_t *)&local_3d8,
                                     local_3d4, local_440, (int)(char)local_466);
                    goto LAB_140090fab;
                }
LAB_140090fea:                                                                /* dc:93013 */
                if (-1 < LVar69.QuadPart) goto LAB_140090fef;
            }
            else {
                if (local_3f8.QuadPart < 0) {
                    uVar18 = iVar27 + -1 + uVar20;                            /* dc:93018 */
                    goto LAB_140090f5a;
                }
LAB_140090fab:                                                                /* dc:93021 */
                iVar17 = (int)local_3e0;
                if (-1 < LVar69.QuadPart) {
                    *(int32_t *)((int64_t)(uintptr_t)pWVar54 + uVar49 + 0x1c6) = iVar27; /* dc:93024 */
                    *(byte *)((int64_t)(uintptr_t)pWVar54 + uVar49 + 0x1bf) = local_40d; /* dc:93025 */
                    *(byte *)((int64_t)(uintptr_t)pWVar54 + uVar49 + 0x1c0) = local_460; /* dc:93026 */
                    *(char *)((int64_t)(uintptr_t)pWVar54 + uVar49 + 0x1c1) = (char)local_3b0; /* dc:93027 */
                }
                if (local_3f8.QuadPart < 0) goto LAB_140090fea;
LAB_140090fef:                                                                /* dc:93031 */
                *(char *)((int64_t)(uintptr_t)pWVar54 + uVar49 + 0x1c3) = local_3d7; /* dc:93032 */
                *(byte *)((int64_t)(uintptr_t)pWVar54 + uVar49 + 0x1c4) = local_3d8; /* dc:93033 */
                *(byte *)((int64_t)(uintptr_t)pWVar54 + uVar49 + 0x1c5) =         /* dc:93034 */
                    (byte)local_298[0];
        }
        LVar33 = local_278;                                                   /* dc:93036 */
        if (-1 < local_3f8.QuadPart) {                                        /* dc:93037 */
            *(int32_t *)((int64_t)(uintptr_t)pWVar54 + uVar49 + 0x1ca) = iVar17; /* dc:93038 */
        }
        if (((-1 < (int)local_3c8) || (-1 < (short)local_380)) ||
            ((-1 < LVar69.QuadPart || (-1 < local_3f8.QuadPart)))) {          /* dc:93036 */
            *(byte *)((byte *)(uintptr_t)pWVar54 + 0x1fe) = 0x55;             /* dc:93038 */
            *(byte *)((byte *)(uintptr_t)pWVar54 + 0x1ff) = 0xaa;
        }
        if ((((short)local_380 < 0) && ((int)local_3c8 < 0)) &&
            ((LVar69.QuadPart < 0 && (local_3f8.QuadPart < 0)))) {           /* dc:93041 */
            *(byte *)((byte *)(uintptr_t)pWVar54 + 0x1fe) = 0x55;             /* dc:93043 */
            *(byte *)((byte *)(uintptr_t)pWVar54 + 0x1ff) = 0xaa;
        }
        /* dc:93046-93093 写回 + -clear 二级引导交换 */
        LVar69 = PECMD_SetFilePointer(pvVar47, local_278, 0);                 /* dc:93046 */
        if (LVar69.QuadPart != LVar33.QuadPart) goto LAB_140092ae8;
        BVar19 = 1;
        if ((ulonglong)local_2f0.QuadPart == 0) {                             /* dc:93049 */
            PECMD_FixRandomSeed((int64_t)(uintptr_t)pWVar54);                 /* dc:93050 */
        }
        uVar49 = local_328;                                                   /* dc:93052 */
        iVar17 = PECMD_AnsiStrNCompare(local_2c8, (int64_t)(uintptr_t)pWVar54, /* dc:93053 */
                                       (int64_t)local_328);
        if (iVar17 != 0) {
            BVar19 = WriteFile(pvVar47, pWVar54, local_44c, &local_3f0, NULL); /* dc:93055 */
            PECMD_FlushFileThrice(pvVar47);
            uVar49 = local_328;
        }
        local_45c = *(uint32_t *)((byte *)(uintptr_t)pWVar54 + 0xdc);         /* dc:93059 签名 */
        pcVar57 = local_2c8;                                                  /* dc:93060 */
        if (((0x1ff < local_428.QuadPart) && (0x200 < local_3f8.QuadPart)) &&
            ('\0' < local_3a0)) {                                             /* dc:93061 -clear */
            memset(pWVar54, 0, (size_t)uVar49);                               /* dc:93063 */
            LVar69 = local_428;
            LVar35.QuadPart = local_350.QuadPart + local_428.QuadPart * 0x200;
            LVar33 = PECMD_SetFilePointer(pvVar47, LVar35, 0);                /* dc:93066 */
            if (LVar33.QuadPart + LVar69.QuadPart * -0x200 == 0) {
                BVar22 = ReadFile(pvVar47, local_2c8, local_44c, &local_3f0, NULL); /* dc:93068 */
                pcVar57 = local_2c8;
                pWVar54 = local_388;
                BVar19 = 0;
                if (((BVar22 != 0) && (local_3f0 == local_44c)) &&
                    (iVar17 = PECMD_AnsiStrNCompare(local_2c8,                 /* dc:93073 */
                                                    (int64_t)(uintptr_t)local_388,
                                                    (int64_t)local_328), BVar19 = BVar22, iVar17 != 0)) {
                    LVar33 = PECMD_SetFilePointer(pvVar47, LVar35, 0);        /* dc:93075 */
                    if (LVar33.QuadPart + LVar69.QuadPart * -0x200 != 0)      /* dc:93076 */
                        goto LAB_140091177;
                    BVar19 = WriteFile(pvVar47, local_388, local_44c, &local_3f0, NULL); /* dc:93077 */
                    pcVar57 = local_2c8;
                    pWVar54 = local_388;
                }
            }
            else {
LAB_140091177:                                                              /* dc:93084 */
                pcVar57 = local_2c8;
                pWVar54 = local_388;
                BVar19 = 0;
            }
        }
        LVar51.QuadPart = (uint64_t)(byte)(BVar19 == 0);                      /* dc:93090 */
        pWVar37 = (WCHAR *)0x0;                                               /* dc:93092 */
        local_2b0 = LVar51;
        if (local_43c != 0) {                                                 /* dc:93093 -fs0 */
            local_278.QuadPart = local_428.QuadPart << 9;                     /* dc:93095 */
            *(uint32_t *)((int64_t)(uintptr_t)pcVar57 + 0x1b8) = local_45c;   /* dc:93096 */
            *(int64_t *)((int64_t)(uintptr_t)pcVar57 + 0x1bc) =               /* dc:93097 */
                (int64_t)local_278.QuadPart;
            wsprintfW(pWVar54, L"#%04X%04X%04X", (ulonglong)local_45c,        /* dc:93098 名称 */
                      (local_428.QuadPart & 0x7fffffffffffffU) >> 0x17);
            /* dc:93099 SHSetValueW 丢参补全 (asm 0x1400912ad): (HKLM,path,name,
             * REG_BINARY, pcVar57+0x1b8, 0xc) */
            (*g_pSHSetValueW)(0x80000002, L"SYSTEM\MountedDevices", local_388,
                              3, (const void *)((byte *)(uintptr_t)pcVar57 + 0x1b8), 0xc);
            if (((local_468 == 0) && (local_465 == 0)) && (local_467 == 0))   /* dc:93101 */
                goto LAB_140092b14;
            if (-1 < (longlong)(int64_t)(uintptr_t)local_360) {               /* dc:93103 */
                PECMD_CloseDeviceHandle(&local_3a8,                       /* dc:93105 */
                                         (int)(int64_t)(uintptr_t)local_360,
                                         (byte)local_42c,
                                         (byte *)&local_res20,
                                         (uint64_t *)&local_2b0.QuadPart,
                                         (LPCWSTR)(uintptr_t)local_330);
                lVar56 = 1;
                if ((int)local_448 < 0) goto LAB_140091470;                   /* dc:93107 */
                *local_418 = L'\0';
                lVar34 = PECMD_DescribePartitionInfo(&local_418,              /* dc:93109 */
                                                     (LPCWSTR)(int64_t)(uintptr_t)local_360,
                                                     (uint64_t)(longlong)((int)local_448 + 1),
                                                     0x202, (LARGE_INTEGER *)0x0,
                                                     (uint *)0x0, '\0');
                if ((int)lVar34 < 1) goto LAB_140091470;
                DVar23 = GetTickCount();                                      /* dc:93113 */
                if (0 < local_250[0]) {
                    local_250[0] = 0;
                }
                uVar49 = (ulonglong)(int64_t)(int)lVar34 | 0x100000;          /* dc:93117 */
                pWVar37 = PECMD_EnumerateVolume((int64_t *)&local_418,        /* dc:93118 */
                                                (int64_t)(int)local_434, uVar49,
                                                (LPCWSTR)0x0);
                if (pWVar37 != (WCHAR *)0x0) goto LAB_140091419;
                goto LAB_1400913e4;
            }
        }
        bVar52 = local_465;                                                   /* dc:93124 */
        if (((local_468 == 0) && (local_465 == 0)) && (local_467 == 0)) {
LAB_140092b14:                                                              /* dc:93126 */
            PECMD_FreeStrBuf(&local_3c0);
            PECMD_FreeStrBuf(&local_3d0);
            PECMD_FreeStrBuf(&local_458.QuadPart);
            PECMD_ClearFlagAndError((byte *)&local_res20);
            if (pvVar47 != (HANDLE)-1) {
                CloseHandle(pvVar47);
            }
            LeaveCriticalSection((LPCRITICAL_SECTION)&g_csDisk);              /* dc:93134 */
            PECMD_FreeStrBuf(&local_290);
            PECMD_FreeStrBuf(&local_2f8);
            PECMD_FreeStrBuf(&local_260);
            PECMD_FreeStrBuf(&local_1d0);
            PECMD_FreeStrBuf(&local_418);
            return LVar51;                                                    /* dc:93140 */
        }
        PECMD_FreeStrBuf(&local_3c0);                                         /* dc:93142 */
        ppuVar41 = &local_3d0;
        goto LAB_14008fe86;                                                   /* dc:93144 */
    }                                                                          /* dc:93145 闭 if(92975) */
        goto LAB_1400906d1;                                                   /* dc:93146 */
    }                                                                          /* dc:93147 闭 if(92957) */
        /* dc:93148-93213 list part (MBR) 表格输出 */
        PECMD_AllocString(&local_418, 0x28);                                  /* dc:93148 */
        iVar27 = (int)local_448;                                              /* dc:93149 */
        iVar17 = 0;
        if (-1 < iVar27) {
            LVar3.HighPart = 0;
            LVar3.LowPart = (DWORD)(iVar27 + 1U);
            local_348.QuadPart = 4;
            iVar17 = iVar27;
            if ((int)(iVar27 + 1U) < 4) {
                local_348 = LVar3;
            }
        }
        lVar56 = (longlong)iVar17;
        if (lVar56 < (int)local_348.LowPart) {
            puVar48 = (uint16_t *)g_szEmpty;                                  /* dc:93162 分隔串 */
            lVar34 = (int)local_348.LowPart - lVar56;
            pWVar54 = local_388 + lVar56 * 8 + 0xe3;                          /* dc:93164 条目游标 */
            pWVar63 = local_418;
            uVar49 = (ulonglong)local_448;
            do {
                WVar40 = pWVar54[-2];                                         /* dc:93168 type 字节 */
                if ((int)uVar49 < 0) {
                    /* dc:93170 格式实为 L"%s%d" (asm 0x1400930da, Ghidra 格式串错位) */
                    iVar17 = wsprintfW(pWVar63, L"%s%d", puVar48, uVar31);
                    puVar48 = (uint16_t *)L" ";                               /* dc:93171 */
                    pWVar63 = pWVar63 + iVar17;
                    uVar49 = (ulonglong)local_448;
                }
                else if ((byte)WVar40 != 0) {                                 /* dc:93175 */
                    iVar17 = wsprintfW(pWVar63, L"%s%d ", puVar48, uVar31);   /* dc:93176 */
                    pwVar45 = L" %d";                                         /* dc:93177 */
                    pWVar64 = pWVar63 + iVar17;
                    if ((local_408 & 1) != 0) {
                        pwVar45 = L" 0x%X";                                   /* dc:93180 -hextp */
                    }
                    iVar17 = wsprintfW(pWVar64, pwVar45, (ulonglong)(byte)WVar40);
                    iVar27 = wsprintfW(pWVar64 + iVar17, L" %d",              /* dc:93183 */
                                       (ulonglong)(byte)((byte)pWVar54[-4] >> 7));
                    pWVar63 = pWVar64 + iVar17 + iVar27;
                    PECMD_SprintfRetEnd(pWVar63,                              /* dc:93185 LBA */
                                        (ulonglong)(*(uint32_t *)(uintptr_t)pWVar54 << 9),
                                        L" %I64u");
                    iVar17 = lstrlenW(pWVar63);
                    pWVar63 = pWVar63 + iVar17;
                    PECMD_SprintfRetEnd(pWVar63,                              /* dc:93188 尺寸 */
                                        (ulonglong)(*(uint32_t *)(uintptr_t)(pWVar54 + 2) << 9),
                                        L" %I64u");
                    uVar49 = (ulonglong)local_448;
                }
                pWVar54 = pWVar54 + 8;                                        /* dc:93191 +16B/条目 */
                lVar34 = lVar34 + -1;
                pvVar47 = local_3a8;
                pWVar39 = local_2f8;
            } while (lVar34 != 0);
        }
LAB_140093253:                                                              /* dc:93197 */
        if (0 < (int)local_438) {                                             /* dc:93199 */
            PECMD_ReadTokenString((uint64_t)(uintptr_t)param_1,               /* dc:93200 */
                                  (int64_t *)&local_418, local_438);
        }
        FUN_1400629B8(param_1, pWVar39, local_418);                           /* dc:93201 */
        PECMD_FreeStrBuf(&local_3c0);                                         /* dc:93202 */
        PECMD_FreeStrBuf(&local_3d0);
        PECMD_FreeStrBuf(&local_458.QuadPart);
        PECMD_ClearFlagAndError((byte *)&local_res20);
        if (pvVar47 != (HANDLE)-1) {
            CloseHandle(pvVar47);
        }
        LeaveCriticalSection((LPCRITICAL_SECTION)&g_csDisk);                  /* dc:93209 */
        LVar69.QuadPart = 0;
        goto LAB_14009445a;
    }
    /* ================= dc:93214-93308 update: 扩展分区扫描 ================= */
    uVar49 = (ulonglong)local_448 & 0xffffffff;                               /* dc:93214 */
    if (local_462 != '\0') {
        uVar49 = 4;
    }
    local_448 = uVar49;
    if ((local_3c8 == 5) || (local_3c8 == 0xf)) goto LAB_14008fdbc;           /* dc:93218 */
    local_420[0] = 0;
    FUN_1400633A8(&local_338, 0x100000);                                      /* dc:93220 */
    local_378.QuadPart = (int64_t)(uintptr_t)                                 /* dc:93221 */
        FUN_14005FEAC(pvVar47, (uint64_t *)(uintptr_t)local_338, (uint32_t *)local_420);
    local_45c = local_420[0];
    iVar17 = (int)uVar49;
    if (local_462 == '\0') {                                                  /* dc:93224 */
        if ((iVar17 <= (int)local_420[0]) || (iVar17 < 4)) {                  /* dc:93225 */
            if (local_450 != '\0') goto LAB_14008fc6b;
            bVar72 = false;
            local_466 = 0;
            if (iVar17 != 3 && 3 < iVar17 + 1) {                              /* dc:93229 */
                uVar49 = (ulonglong)((iVar17 + 1) * 4 - 0x10);
            }
            if (((int)uVar49 < 1) ||
                (((int)local_420[0] <= (int)uVar49 && (4 < iVar17))))         /* dc:93233 */
                goto LAB_14008fc2d;
            goto LAB_14008fcad;
        }
LAB_14008fc2d:                                                              /* dc:93237 */
    ppuVar41 = &local_338;
LAB_14008fc35:                                                              /* dc:93239 */
    PECMD_FreeStrBuf((void *)ppuVar41);
    PECMD_FreeStrBuf(&local_458.QuadPart);
    PECMD_ClearFlagAndError((byte *)&local_res20);
    if (pvVar47 != (HANDLE)-1) {                                              /* dc:93243 */
        CloseHandle(pvVar47);
    }
    goto LAB_14008efd3;
    }   /* dc:93247 闭 if(93225) */
LAB_14008fc6b:                                                              /* dc:93248 */
    bVar72 = true;
    local_466 = 1;
    if (local_450 == '\0') {                                                  /* dc:93250 */
        if (local_462 != '\0') {                                              /* dc:93251 */
            if ((int)local_3c8 < 0) goto LAB_14008fc2d;
            uVar15 = 0;
            if (0 < (short)local_380) {
                uVar15 = 0x80;
            }
            local_380 = (local_380 & 0xffff0000U) | uVar15;                   /* dc:93257 */
        }
    }
    else if ((int)local_420[0] < 5) goto LAB_14008fc2d;
LAB_14008fcad:                                                              /* dc:93262 */
    if ((int)local_420[0] < 5) {                                             /* dc:93263 */
        uVar18 = 0;
    }
    else {
        uVar18 = local_420[0] - 4 & 0xfffffffc;
    }
    uVar20 = 0;                                                               /* dc:93268 */
    uVar49 = 0xffffffff;
    local_3fc = 0xffffffff;
    if ((!bVar72) && (uVar18 = iVar17 * 4 - 0x10, iVar17 < 4)) {              /* dc:93271 */
        uVar18 = 0;
    }
    uVar25 = 0xffffffff;
    puVar36 = (ulonglong *)(local_378.QuadPart + 0x50);                       /* dc:93275 */
    do {
        if (((char)*puVar36 == '\x05') || ((char)*puVar36 == '\x0f')) {       /* dc:93277 */
            uVar25 = uVar20;
        }
        uVar20 = uVar20 + 1;
        puVar36 = puVar36 + 0x12;
    } while ((int)uVar20 < 4);
    uVar31 = 0;                                                               /* dc:93283 */
    if ((bVar72) && ((int)uVar25 < 0)) goto LAB_14008fdaf;                    /* dc:93284 */
    lVar70 = (longlong)(int)uVar25 * 0x90;                                    /* dc:93285 */
    lVar56 = *(longlong *)(lVar70 + 0x38 + local_378.QuadPart);               /* dc:93286 */
    lVar34 = *(longlong *)(lVar70 + 0x40 + local_378.QuadPart);               /* dc:93287 */
    LVar69.QuadPart = (longlong)((lVar56 >> 0x3f & 0x1ffU) + lVar56) >> 9;    /* dc:93288 */
    puVar59 = (ushort *)(longlong)(((lVar34 >> 0x3f & 0x1ffU) + lVar34) >> 9); /* dc:93289 */
    lVar56 = (longlong)(int)uVar18 * 0x90;                                    /* dc:93290 */
    if (local_450 == '\0') {                                                  /* dc:93291 */
        uVar20 = uVar25;
        if (4 < (int)local_420[0]) {
            puVar36 = (ulonglong *)(local_378.QuadPart + 0x50) + (longlong)(int)uVar18 * 0x12; /* dc:93294 */
            do {
                if (((char)*puVar36 == '\x05') || ((char)*puVar36 == '\x0f')) { /* dc:93296 */
                    uVar49 = uVar31;
                }
                uVar20 = (int)uVar31 + 1;
                uVar31 = (ulonglong)uVar20;
                puVar36 = puVar36 + 0x12;
            } while ((int)uVar20 < 4);
            uVar20 = (uint)uVar49;
            if (local_466 == 0) {                                             /* dc:93304 */
                uVar20 = (uint)uVar49 + uVar18;
            }
        }
        /* dc:93309-93356 条目区间换算 */
    uVar21 = 0;                                                               /* dc:93309 */
    if ((-1 < (int)uVar20) || (local_462 != '\0')) {
        local_3fc = 0;
        if (local_466 == 0) {
            local_3fc = 0xffffffff;                                           /* dc:93313 */
            pcVar57 = (char *)(lVar56 + 0x290 + local_378.QuadPart);
            do {
                if ((((*pcVar57 != '\0') || (uVar21 == 0)) && (*pcVar57 != '\x05')) &&
                    (*pcVar57 != '\x0f')) {
                    local_3fc = uVar21;                                       /* dc:93318 */
                }
                uVar21 = uVar21 + 1;
                pcVar57 = pcVar57 + 0x90;
            } while ((int)uVar21 < 4);
        }
        if ((local_466 != 0) || (-1 < (int)local_3fc)) {                      /* dc:93324 */
            if (local_462 == '\0') {
                local_2f0.QuadPart = (longlong)                               /* dc:93326 */
                    (((ulonglong *)(local_378.QuadPart + 0x38))[(longlong)(int)uVar20 * 0x12] +
                     ((longlong)
                      ((ulonglong *)(local_378.QuadPart + 0x38))[(longlong)(int)uVar20 * 0x12]
                      >> 0x3f & 0x1ffU)) >> 9;
                LVar69.QuadPart = (longlong)                                  /* dc:93332 */
                    (((ulonglong *)(local_378.QuadPart + 0x40))[(longlong)(int)uVar20 * 0x12] +
                     ((longlong)
                      ((ulonglong *)(local_378.QuadPart + 0x40))[(longlong)(int)uVar20 * 0x12]
                      >> 0x3f & 0x1ffU)) >> 9;
                if (local_3e4 != '\0') {
                    local_428.QuadPart = local_428.QuadPart + local_2f0.QuadPart; /* dc:93339 */
                }
                if ((0 < local_428.QuadPart) &&
                    (local_428.QuadPart <= local_2f0.QuadPart)) {             /* dc:93341 */
                    local_428.QuadPart = local_2f0.QuadPart;
                }
                local_288.QuadPart = local_2f0.QuadPart;
                if (0 < local_3f8.QuadPart) {                                 /* dc:93345 */
                    if (LVar69.QuadPart < local_3f8.QuadPart) {
                        local_3f8.QuadPart = LVar69.QuadPart;
                    }
                    if (local_3f8.QuadPart < 0) goto LAB_14008fdaf;
                }
LAB_140090221:                                                              /* dc:93351 */
                LVar33 = local_288;
                local_2f0 = local_288;
                PECMD_FreeStrBuf(&local_338);
                uVar18 = local_440;
                goto LAB_140090245;
            }
            uVar21 = uVar25;
            if (0 < (int)uVar18) {
                uVar21 = uVar18;
            }
            lVar34 = (longlong)(int)uVar21;
            local_320.QuadPart = (longlong)                                   /* dc:93363 */
                (((ulonglong *)(local_378.QuadPart + 0x38))[lVar34 * 0x12] +
                 (ulonglong)
                 ((uint)((longlong)((ulonglong *)(local_378.QuadPart + 0x38))[lVar34 * 0x12]
                        >> 0x3f) & 0x1ff)) >> 9;
            if ((4 < (int)local_420[0]) && ((int)uVar20 < 0)) {               /* dc:93369 */
                if (*(char *)(lVar56 + 0x50 + local_378.QuadPart) != '\0') {  /* dc:93370 */
                    if (8 < (int)local_420[0]) {
                        uVar25 = (local_420[0] - 8 & 0xfffffffc) + 1;         /* dc:93372 */
                    }
                    local_320.QuadPart = (longlong)                           /* dc:93374 */
                        (((ulonglong *)(local_378.QuadPart + 0x38))[(longlong)(int)uVar25 * 0x12]
                         + ((longlong)
                            ((ulonglong *)(local_378.QuadPart + 0x38))
                            [(longlong)(int)uVar25 * 0x12] >> 0x3f & 0x1ffU)) >> 9;
                    lVar56 = *(longlong *)(lVar56 + 0x40 + local_378.QuadPart) +
                             *(longlong *)(lVar56 + 0x38 + local_378.QuadPart); /* dc:93380 */
                    local_288.QuadPart = (longlong)(lVar56 + (lVar56 >> 0x3f & 0x1ffU)) >> 9; /* dc:93382 */
                    local_258.LowPart = (DWORD)(local_288.QuadPart - LVar69.QuadPart); /* dc:93383 */
                    puVar59 = (ushort *)(longlong)(longlong)(int64_t)(uintptr_t)puVar59 + /* dc:93384 */
                              (LVar69.QuadPart - local_288.QuadPart);
                    local_2a8 = puVar59;
                    uVar25 = local_420[0];
                    goto LAB_1400900d3;
                }
                if (((uVar18 == 4) && (-1 < (int)uVar25)) &&
                    (*(char *)(lVar70 + 0x50 + local_378.QuadPart) != '\0')) { /* dc:93390 */
                    uVar18 = 0xffffffff;
                    local_288.QuadPart = LVar69.QuadPart;
                    local_320.QuadPart = LVar69.QuadPart;
                    goto LAB_1400900cb;
                }
                ppuVar41 = &local_338;
LAB_140092a1c:                                                              /* dc:93398 */
                PECMD_FreeStrBuf((void *)ppuVar41);
                PECMD_FreeStrBuf(&local_458.QuadPart);
                goto LAB_140092a2b;
            }
            local_288.QuadPart = local_320.QuadPart;                          /* dc:93403 */
            if (0 < (int)uVar18) {
                lVar56 = ((ulonglong *)(local_378.QuadPart + 0x40))[lVar34 * 0x12] + /* dc:93405 */
                         ((ulonglong *)(local_378.QuadPart + 0x38))[lVar34 * 0x12];
                local_288.QuadPart = (longlong)(lVar56 +
                                                (ulonglong)((uint)(lVar56 >> 0x3f) & 0x1ff)) >> 9;
            }
            puVar59 = (ushort *)(longlong)(longlong)(int64_t)(uintptr_t)puVar59 + /* dc:93410 */
                      (LVar69.QuadPart - local_288.QuadPart);
            uVar25 = local_420[0];
LAB_1400900cb:                                                              /* dc:93412 */
            local_2a8 = (ushort *)0x0;
LAB_1400900d3:                                                              /* dc:93414 */
            if (0 < (longlong)(uintptr_t)puVar59) {
                if (local_3e4 != '\0') {
                    local_428.QuadPart = local_428.QuadPart + local_288.QuadPart; /* dc:93417 */
                }
                if (local_428.QuadPart <= local_288.QuadPart + 1) {           /* dc:93419 */
                    local_428.QuadPart = local_288.QuadPart + 1;
                }
                LVar69.QuadPart = (longlong)(uintptr_t)puVar59 +              /* dc:93422 */
                                  (local_288.QuadPart - local_428.QuadPart);
                if (LVar69.QuadPart < local_3f8.QuadPart) {
                    local_3f8.QuadPart = LVar69.QuadPart;
                }
                if (-1 < local_3f8.QuadPart) {
                    if ((int)uVar18 < 1) {
                        if ((int)(uVar25 & 0xfffffffc) < 4) {
                            local_448 = (ulonglong)((uVar25 & 0xfffffffc) + 1); /* dc:93430 */
                        }
                        else {
                            local_448 = (ulonglong)                           /* dc:93433 */
                                (((int)((uVar25 & 0xfffffffc) +
                                        ((int)uVar25 >> 0x1f & 3U)) >> 2) + 4);
                        }
                    }
                    else {
                        uVar18 = uVar25 - 1 & 0xfffffffc;
                        if ((int)uVar18 < 4) {
                            iVar17 = uVar18 + 1;
                        }
                        else {
                            iVar17 = ((int)(uVar18 + ((int)(uVar25 - 1) >> 0x1f & 3U)) >> 2) + 4; /* dc:93444 */
                        }
                        local_448 = (ulonglong)(iVar17 + 1);
                    }
                    goto LAB_140090221;
                }
            }
        }
    }   /* dc:93452 闭 if(93291) */
    goto LAB_14008fdaf;
    }   /* dc:93454 闭外层块 */
    /* dc:93455-93499 尾段: 剩余扩展条目扫描 + MBR 签名回写 */
    iVar17 = 0;                                                               /* dc:93455 */
    if (local_420[0] != 8) {
        lVar56 = (longlong)(int)(local_420[0] - 8 & 0xfffffffc);              /* dc:93457 */
        puVar36 = (ulonglong *)(local_378.QuadPart + 0x50) + lVar56 * 0x12;   /* dc:93458 */
        while ((*(char *)(uintptr_t)puVar36 != '\x05') &&
               (*(char *)(uintptr_t)puVar36 != '\x0f')) {                     /* dc:93459 */
            uVar31 = uVar31 + 1;
            iVar17 = iVar17 + 1;
            puVar36 = puVar36 + 0x12;
            if (3 < (longlong)uVar31) goto LAB_14008fdaf;                     /* dc:93463 */
        }
        LVar69.QuadPart = (longlong)                                          /* dc:93465 */
            (((ulonglong)((uint)((longlong)((ulonglong *)(local_378.QuadPart + 0x38))[lVar56 * 0x12]
                          >> 0x3f) & 0x1ff) +
              ((ulonglong *)(local_378.QuadPart + 0x38))[lVar56 * 0x12]) >> 9) -
            (ulonglong)*(uint32_t *)(local_378.QuadPart + 0x54 + lVar56 * 0x90);
        if (iVar17 < 0) {                                                     /* dc:93472 */
LAB_14008fdaf:                                                              /* dc:93473 */
            ppuVar41 = &local_338;
LAB_14008fdb7:                                                              /* dc:93475 */
            PECMD_FreeStrBuf((void *)ppuVar41);
LAB_14008fdbc:                                                              /* dc:93477 */
            PECMD_FreeStrBuf(&local_458.QuadPart);
            PECMD_ClearFlagAndError((byte *)&local_res20);
            if (pvVar47 != (HANDLE)-1) {
                CloseHandle(pvVar47);
            }
            goto LAB_14008efd3;
        }
    }
    local_198[0] = 0;                                                         /* dc:93486 签名区清零 */
    local_198[1] = 0;
    local_198[2] = 0;
    local_198[3] = 0;
    PECMD_WriteMbrPartitionEntry(pvVar47,                                     /* dc:93490 */
                                 (int64_t)(LVar69.QuadPart + local_2a0.QuadPart),
                                 (uint8_t *)local_198, iVar17);
    LVar69.QuadPart = 0;                                                      /* dc:93492 */
    local_2b0.QuadPart = 0;
    if (((local_465 == 0) && (local_468 == 0)) && (local_467 == 0))           /* dc:93494 */
        goto LAB_1400929d0;
    ppuVar41 = &local_338;
    bVar52 = local_465;
LAB_14008fe86:                                                              /* dc:93497 */
    PECMD_FreeStrBuf((void *)ppuVar41);
    PECMD_FreeStrBuf(&local_458.QuadPart);
    bVar52 = local_465;                                                     /* dc:93496 */
    uVar49 = 0;                                                             /* dc:91138 回边准备 */
    uVar18 = local_42c;
    iVar17 = local_404;
    goto LAB_14008f48e;                                                     /* dc:91139 do 体回边 */
        } while (true);                                                     /* dc:93500 */
    }   /* dc:93501 闭 if(91135) */
    PECMD_FreeStrBuf(&local_390);                                             /* dc:93502 */
    PECMD_ClearFlagAndError((byte *)&local_res20);                            /* dc:93503 */
    if (pvVar47 != (HANDLE)-1) {                                            /* dc:93504 */
        CloseHandle(pvVar47);
    }
    goto LAB_14008f528;                                                     /* dc:93507 */
    }   /* dc:93508 闭 if(91117) */
    PECMD_FreeStrBuf(&local_390);                                             /* dc:93509 */
    PECMD_ClearFlagAndError((byte *)&local_res20);                            /* dc:93510 */
    if (pvVar47 != (HANDLE)-1) {                                            /* dc:93511 */
        CloseHandle(pvVar47);
    }
    }   /* dc:93514 闭 else(91071) */
LAB_14009444b:                                                              /* dc:93515 */
    LeaveCriticalSection((LPCRITICAL_SECTION)&g_csDisk);                      /* dc:93516 */
    LVar69.QuadPart = 1;                                                      /* dc:93517 */
    goto LAB_14009445a;                                                       /* dc:93518 */
}   /* dc:93519 闭 if(91062) 整体 */
    PECMD_ClearFlagAndError((byte *)&local_res20);                            /* dc:93520 */
    goto LAB_14008f546;                                                     /* dc:93521 */
code_r0x00014008f598:                                                       /* dc:93522 */
    uVar18 = local_42c;
    bVar52 = local_465;
    if ((local_467 == 0) && ((local_468 == 0 && (local_465 == 0)))) {
        PECMD_ClearFlagAndError((byte *)&local_res20);                      /* dc:93526 */
        if ((pvVar47 != (HANDLE)0x0) && (pvVar47 != (HANDLE)-1)) {
            CloseHandle(pvVar47);
        }
LAB_14008f5f0:                                                              /* dc:93530 */
        LeaveCriticalSection((LPCRITICAL_SECTION)&g_csDisk);
        LVar69 = local_280;                                                 /* dc:93532 */
        goto LAB_14009445a;
    }
    goto LAB_14008f48e;                                                     /* dc:93535 */
LAB_1400929d0:                                                              /* dc:93536 */
    PECMD_FreeStrBuf(&local_338);
    PECMD_FreeStrBuf(&local_458.QuadPart);
    PECMD_ClearFlagAndError((byte *)&local_res20);
    if (pvVar47 != (HANDLE)-1) {
        CloseHandle(pvVar47);
    }
LAB_14008f528:                                                              /* dc:93543 */
    LeaveCriticalSection((LPCRITICAL_SECTION)&g_csDisk);
    goto LAB_14009445a;
    /* dc:93546-93557 cdrom/floppy 26 条目表搜索循环 (goto 目标 91681) */
LAB_1400935e3:                                                              /* dc:93551 */
    for (;;) {
        if (*puVar38 == local_434) {
            PECMD_AllocString(&local_418, 100);                               /* dc:93553 */
            wsprintfW(local_418, L"%d \"%c\"",                                /* dc:93554 */
                      *(uint32_t *)((int64_t)(uintptr_t)puVar36 +
                                      (longlong)iVar17 * 0x220 + 4),
                      (ulonglong)(byte)((byte *)(uintptr_t)puVar36)[(longlong)iVar17 * 0x44 + 2]);
            break;
        }
        lVar56 = lVar56 + 1;
        iVar17 = iVar17 + 1;
        puVar38 = puVar38 + 0x88;
        if (0x1a < lVar56) break;                                             /* dc:93550 */
    }
    PECMD_FreeStrBuf(&local_348.QuadPart);                                    /* dc:93560 */
LAB_14009367e:                                                              /* dc:93561 */
    LVar35.QuadPart = 0;
    PECMD_FreeStrBuf(&local_458.QuadPart);
    uVar18 = local_438;                                                       /* dc:93564 */
LAB_140093820:                                                              /* dc:93565 */
    if (0 < (int)uVar18) {                                                    /* dc:93566 */
        PECMD_ReadTokenString((uint64_t)(uintptr_t)param_1,                   /* dc:93567 */
                              (int64_t *)&local_418, uVar18);
    }
LAB_140093838:                                                              /* dc:93569 */
    FUN_1400629B8(param_1, pWVar39, local_418);                               /* dc:93570 */
    PECMD_ClearFlagAndError((byte *)&local_res20);
    if ((pvVar47 != (HANDLE)0x0) && (pvVar47 != (HANDLE)-1)) {
        CloseHandle(pvVar47);
    }
    LeaveCriticalSection((LPCRITICAL_SECTION)&g_csDisk);                      /* dc:93575 */
    PECMD_FreeStrBuf(&local_290);                                             /* dc:93576 */
    PECMD_FreeStrBuf(&local_2f8);
    PECMD_FreeStrBuf(&local_260);
    PECMD_FreeStrBuf(&local_1d0);
    PECMD_FreeStrBuf(&local_418);
    return LVar35;                                                            /* dc:93581 */
    /* dc:93582-93613 -fs0 挂载等待路径 (goto 目标 93121) */
LAB_1400913e4:                                                              /* dc:93584 */
    for (;;) {
        DVar24 = GetTickCount();                                              /* dc:93585 */
        if (5000 < DVar24 - DVar23) break;
        pWVar37 = PECMD_EnumerateVolume((int64_t *)&local_418,                /* dc:93582 重试 */
                                        (int64_t)(int)local_434, uVar49,
                                        (LPCWSTR)0x0);
        if (pWVar37 != (WCHAR *)0x0) break;
    }
    if (pWVar37 != (WCHAR *)0x0) {                                            /* dc:93588 */
LAB_140091419:                                                              /* dc:93589 */
        pWVar54 = local_388;
        local_388[0x3c] = L'#';                                               /* dc:93591 */
        lstrcpyW(local_388 + 0x3d, pWVar37);                                  /* dc:93592 */
        pWVar54[0x3e] = L'?';                                                 /* dc:93593 */
        /* dc:93594 SHSetValueW 丢参补全 (asm 0x14009146a):
         * (HKLM, L"SYSTEM\MountedDevices", local_388, REG_BINARY,
         *  local_2c8+0x1b8, 0xc) */
        (*g_pSHSetValueW)(0x80000002, L"SYSTEM\MountedDevices", local_388,
                          3, (const void *)((byte *)(uintptr_t)local_2c8 + 0x1b8), 0xc);
    }
LAB_140091470:                                                              /* dc:93596 */
    if ((-1 < local_250[0]) || (pWVar37 != (WCHAR *)0x0)) {                   /* dc:93597 */
        if ((0 < local_250[0]) && (pWVar37 == (WCHAR *)0x0)) {
            PECMD_WaitHandlesOrMessages((uint64_t)(uintptr_t)param_1,         /* dc:93599 */
                                        (int64_t)local_250[0], 0, (uint64_t *)0x0);
        }
        (*g_pSHDeleteValueW)(0x80000002, L"SYSTEM\MountedDevices",           /* dc:93601 */
                             local_388);
    }
    LVar69 = local_2b0;                                                       /* dc:93603 */
    if (-1 < (int)lVar56) {
        LVar69 = LI64((int64_t)iVar27);                              /* dc:93605 */
    }
    PECMD_FreeStrBuf(&local_3c0);                                             /* dc:93607 */
    PECMD_FreeStrBuf(&local_3d0);
    PECMD_FreeStrBuf(&local_458.QuadPart);
    PECMD_ClearFlagAndError((byte *)&local_res20);
    if ((local_3a8 != (HANDLE)0x0) && (local_3a8 != (HANDLE)-1)) {            /* dc:93611 */
        CloseHandle(local_3a8);
    }
LAB_14008f546:                                                              /* dc:93614 */
    LeaveCriticalSection((LPCRITICAL_SECTION)&g_csDisk);
LAB_14009445a:                                                              /* dc:93616 */
    PECMD_FreeStrBuf(&local_290);                                             /* dc:93617 */
    PECMD_FreeStrBuf(&local_2f8);
    PECMD_FreeStrBuf(&local_260);
    PECMD_FreeStrBuf(&local_1d0);
    PECMD_FreeStrBuf(&local_418);
    LVar69.QuadPart = LVar69.QuadPart;
    return LVar69;                                     /* dc:93622 */
}
