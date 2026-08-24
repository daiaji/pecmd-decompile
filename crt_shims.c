/* crt_shims.c - B0/P4: CRT isolation.
 * Collected from core_b9_remaining.c per PRODUCTION_ROADMAP §3.B.0 item 3:
 *   (a) MSVC CRT internal mechanism family (_xxx/__xxx)
 *   (b) libc same-name strong definitions (strtol/perror/raise/iswctype/wcstol/...)
 *   (c) math empty stubs (double FUN_xxx float-expansion placeholders)
 * FOR LINK VERIFICATION ONLY - must be removed before deployment linking
 * against a real system CRT. Bodies are byte-identical to their origin. */

#include "pecmd_defs.h"

uint64_t __onexitinit(void)
{
    /* @0x140102e04 size=66 */
    return 0;
}

uint64_t _onexit(void)
{
    /* @0x140102e48 size=261 */
    return 0;
}

uint64_t _vswprintf_l(void)
{
    /* @0x140102f68 size=170 */
    return 0;
}

uint64_t _LocaleUpdate(void)
{
    /* @0x140103084 size=169 */
    return 0;
}

uint64_t _wcsicmp(void)
{
    /* @0x140103244 size=148 */
    return 0;
}

uint64_t _wcsupr_s_l_stat(void)
{
    /* @0x1401032d8 size=534 */
    return 0;
}

uint64_t _wcsupr(void)
{
    /* @0x14010353c size=134 */
    return 0;
}

uint64_t __initstdio(void)
{
    /* @0x140103a28 size=226 */
    return 0;
}

uint64_t __endstdio(void)
{
    /* @0x140103b0c size=39 */
    return 0;
}

uint64_t _lock_file(void)
{
    /* @0x140103b34 size=95 */
    return 0;
}

uint64_t _lock_file2(void)
{
    /* @0x140103b94 size=45 */
    return 0;
}

uint64_t _unlock_file(void)
{
    /* @0x140103bc4 size=86 */
    return 0;
}

uint64_t _unlock_file2(void)
{
    /* @0x140103c1c size=39 */
    return 0;
}

uint64_t perror(void)
{
    /* @0x140103c78 size=173 */
    return 0;
}

uint64_t wcstol(void)
{
    /* @0x140104348 size=48 */
    return 0;
}

uint64_t wcstoul(void)
{
    /* @0x140104378 size=51 */
    return 0;
}

uint64_t _wcslwr_s_l_stat(void)
{
    /* @0x140104544 size=534 */
    return 0;
}

uint64_t _wcsnicmp(void)
{
    /* @0x14010490c size=165 */
    return 0;
}

double FUN_140104b00(double param_1)
{
    /* UNIMPLEMENTED @0xFUN_140104b00 — decompile-failed, body 未还原 */
/* @0x140104b00 size=773 */
    /* SKIP(CRT): MSVC CRT 三角函数 (asin/acos 多项式展开, 含 _controlfp_s/
       SQRT/系数表 _DAT_14012d2b0 与 FUN_14010da74 打印辅助). 使用 CRT 等价,
       不逐行还原内联展开. */
    (void)param_1;
    return 0.0;
}

double FUN_140104e08(double param_1)
{
    /* UNIMPLEMENTED @0xFUN_140104e08 — decompile-failed, body 未还原 */
/* @0x140104e08 size=787 */
    /* SKIP(CRT): MSVC CRT 三角函数 (asin/acos 多项式展开, 含 _controlfp_s/
       SQRT/系数表 _DAT_14012d2b0 与 FUN_14010da74 打印辅助). 使用 CRT 等价. */
    (void)param_1;
    return 0.0;
}

double FUN_14010511c(double param_1)
{
    /* UNIMPLEMENTED @0xFUN_14010511c — decompile-failed, body 未还原 */
/* @0x14010511c size=651 */
    /* SKIP(CRT): atan 数学函数，使用 CRT atan 句柄 */
    (void)param_1;
    return 0.0;
}

double FUN_1401053a8(double param_1)
{
    /* UNIMPLEMENTED @0xFUN_1401053a8 — decompile-failed, body 未还原 */
/* @0x1401053a8 size=276 */
    /* SKIP(CRT): MSVC CRT ceil 实现 (幂尾数掩码 + 有小数位时 +1.0, 含
       FUN_14010da74 打印辅助/非法参数路径). 使用 CRT ceil 等价, 不还原内联. */
    (void)param_1;
    return 0.0;
}

double FUN_1401055ec(double param_1)
{
    /* UNIMPLEMENTED @0xFUN_1401055ec — decompile-failed, body 未还原 */
/* @0x1401055ec size=734 */
    /* SKIP(CRT): MSVC CRT exp 数学函数内联实现 */
    (void)param_1;
    return 0.0;
}

double FUN_1401058cc(double param_1)
{
    /* UNIMPLEMENTED @0xFUN_1401058cc — decompile-failed, body 未还原 */
/* @0x1401058cc size=756 */
    /* SKIP(CRT): MSVC CRT atan 数学函数内联实现 */
    (void)param_1;
    return 0.0;
}

double FUN_140105bc0(uint64_t param_1)
{
    /* UNIMPLEMENTED @0xFUN_140105bc0 — decompile-failed, body 未还原 */
/* @0x140105bc0 size=846 */
    /* SKIP(CRT): 反编译为 MSVC CRT log10 数学函数内联实现 */
    (void)param_1;
    return 0.0;
}

double FUN_140105f10(double param_1, double param_2)
{
    /* UNIMPLEMENTED @0xFUN_140105f10 — decompile-failed, body 未还原 */
/* @0x140105f10 size=1494 */
    /* SKIP(CRT): pow 数学函数，使用 log2/f2xm1/fscale 内联实现 */
    (void)param_1;
    (void)param_2;
    return 0.0;
}

double FUN_140106500(double param_1, double param_2)
{
    /* UNIMPLEMENTED @0xFUN_140106500 — decompile-failed, body 未还原 */
/* @0x140106500 size=162 */
    /* SKIP(CRT): sin 三角函数，使用 CRT 内联实现 */
    (void)param_1;
    (void)param_2;
    return 0.0;
}

double FUN_1401065a4(double param_1, double param_2)
{
    /* UNIMPLEMENTED @0xFUN_1401065a4 — decompile-failed, body 未还原 */
/* @0x1401065a4 size=175 */
    /* SKIP(CRT): cos 三角函数，使用 CRT 内联实现 */
    (void)param_1;
    (void)param_2;
    return 0.0;
}

double FUN_140106654(double param_1)
{
    /* UNIMPLEMENTED @0xFUN_140106654 — decompile-failed, body 未还原 */
/* @0x140106654 size=807 */
    /* SKIP(CRT): MSVC CRT sin 数学函数内联实现 */
    (void)param_1;
    return 0.0;
}

double FUN_14010697c(double param_1)
{
    /* UNIMPLEMENTED @0xFUN_14010697c — decompile-failed, body 未还原 */
/* @0x14010697c size=266 */
    /* SKIP(CRT): MSVC CRT sqrt 数学函数内联实现 */
    (void)param_1;
    return 0.0;
}

double FUN_140106a88(double param_1, double param_2, int param_3)
{
    /* UNIMPLEMENTED @0xFUN_140106a88 — decompile-failed, body 未还原 */
/* @0x140106a88 size=468 */
    /* SKIP(CRT): atan/asin 数学函数，使用 CRT 内联实现 */
    (void)param_1;
    (void)param_2;
    (void)param_3;
    return 0.0;
}

double FUN_140106c5c(double param_1)
{
    /* UNIMPLEMENTED @0xFUN_140106c5c — decompile-failed, body 未还原 */
/* @0x140106c5c size=791 */
    /* SKIP(CRT): MSVC CRT cos 数学函数内联实现 */
    (void)param_1;
    return 0.0;
}

uint64_t _purecall(void)
{
    /* @0x140107350 size=54 */
    return 0;
}

double FUN_140107468(double param_1, double param_2)
{
    /* UNIMPLEMENTED @0xFUN_140107468 — decompile-failed, body 未还原 */
/* @0x140107468 size=644 */
    /* SKIP(CRT): MSVC CRT _hypot 数学函数 */
    (void)param_1;
    (void)param_2;
    return 0.0;
}

uint64_t _GetEstablisherFrame(void)
{
    /* @0x140107950 size=235 */
    return 0;
}

uint64_t _GetImageBase(void)
{
    /* @0x140107a3c size=21 */
    return 0;
}

uint64_t _GetThrowImageBase(void)
{
    /* @0x140107a54 size=21 */
    return 0;
}

uint64_t _SetImageBase(void)
{
    /* @0x140107a6c size=27 */
    return 0;
}

uint64_t __FrameUnwindToEmptyState(void)
{
    /* @0x140107aa4 size=172 */
    return 0;
}

uint64_t _CallSETranslator(void)
{
    /* @0x140107c7c size=81 */
    return 0;
}

uint64_t _GetRangeOfTrysToCheck(void)
{
    /* @0x140107cd0 size=293 */
    return 0;
}

uint64_t _IsExceptionObjectToBeDestroyed(void)
{
    /* @0x140107e3c size=50 */
    return 0;
}

uint64_t _UnwindNestedFrames(void)
{
    /* @0x140107ed0 size=230 */
    return 0;
}

uint64_t __tmainCRTStartup(void)
{
    /* @0x140108014 size=441 */
    return 0;
}

uint64_t entry(void)
{
    /* @0x1401081ec size=18 */
    return 0;
}

uint64_t exception(void)
{
    /* @0x140108274 size=25 */
    return 0;
}

uint64_t _callnewh(void)
{
    /* @0x140108380 size=50 */
    return 0;
}

uint64_t _CxxThrowException(void)
{
    /* @0x1401083b4 size=145 */
    return 0;
}

uint64_t _mtterm(void)
{
    /* @0x140108464 size=37 */
    return 0;
}

uint64_t _initptd(void)
{
    /* @0x14010848c size=178 */
    return 0;
}

uint64_t _getptd_noexit(void)
{
    /* @0x140108540 size=132 */
    return 0;
}

uint64_t _freefls(void)
{
    /* @0x1401085e8 size=309 */
    return 0;
}

uint64_t _mtinit(void)
{
    /* @0x140108720 size=129 */
    return 0;
}

uint64_t _malloc_crt(void)
{
    /* @0x1401087a4 size=108 */
    return 0;
}

uint64_t _calloc_crt(void)
{
    /* @0x140108810 size=131 */
    return 0;
}

uint64_t _realloc_crt(void)
{
    /* @0x140108894 size=133 */
    return 0;
}

uint64_t _recalloc_crt(void)
{
    /* @0x14010891c size=136 */
    return 0;
}

uint64_t _msize(void)
{
    /* @0x1401089a4 size=70 */
    return 0;
}

uint64_t __crtExitProcess(void)
{
    /* @0x140108a58 size=21 */
    return 0;
}

uint64_t _cinit(void)
{
    /* @0x140108af8 size=175 */
    return 0;
}

uint64_t _init_pointers(void)
{
    /* @0x140108d6c size=103 */
    return 0;
}

uint64_t __C_specific_handler(void)
{
    /* @0x140108dd4 size=417 */
    return 0;
}

uint64_t _flsbuf(void)
{
    /* @0x140108f78 size=397 */
    return 0;
}

uint64_t _woutput_l(void)
{
    /* @0x140109218 size=2829 */
    return 0;
}

uint64_t _invalid_parameter(void)
{
    /* @0x140109e58 size=133 */
    return 0;
}

uint64_t _get_errno_from_oserr(void)
{
    /* @0x140109ee0 size=71 */
    return 0;
}

uint64_t __doserrno(void)
{
    /* @0x140109f28 size=32 */
    return 0;
}

uint64_t __doserrno_140109f48(void)
{
    /* @0x140109f48 size=32 */
    return 0;
}

uint64_t _dosmaperr(void)
{
    /* @0x140109f68 size=70 */
    return 0;
}

uint64_t __updatetmbcinfo(void)
{
    /* @0x14010a228 size=188 */
    return 0;
}

uint64_t _setmbcp_nolock(void)
{
    /* @0x14010a374 size=631 */
    return 0;
}

uint64_t _setmbcp(void)
{
    /* @0x14010a5ec size=504 */
    return 0;
}

uint64_t __initmbctable(void)
{
    /* @0x14010a7e4 size=40 */
    return 0;
}

uint64_t __freetlocinfo(void)
{
    /* @0x14010a80c size=378 */
    return 0;
}

uint64_t __addlocaleref(void)
{
    /* @0x14010a988 size=149 */
    return 0;
}

uint64_t __removelocaleref(void)
{
    /* @0x14010aa20 size=165 */
    return 0;
}

uint64_t _updatetlocinfoEx_nolock(void)
{
    /* @0x14010aac8 size=87 */
    return 0;
}

uint64_t __updatetlocinfo(void)
{
    /* @0x14010ab20 size=117 */
    return 0;
}

uint64_t __GSHandlerCheckCommon(void)
{
    /* @0x14010ac94 size=99 */
    return 0;
}

uint64_t __GSHandlerCheck(void)
{
    /* @0x14010acf8 size=29 */
    return 0;
}

uint64_t __security_check_cookie(void)
{
    /* @0x14010ae70 size=31 */
    return 0;
}

uint64_t _ioinit(void)
{
    /* @0x14010ae90 size=746 */
    return 0;
}

uint64_t _fcloseall(void)
{
    /* @0x14010b180 size=176 */
    return 0;
}

uint64_t _flush(void)
{
    /* @0x14010b230 size=121 */
    return 0;
}

uint64_t _fflush_nolock(void)
{
    /* @0x14010b2ac size=71 */
    return 0;
}

uint64_t _mtinitlocks(void)
{
    /* @0x14010b3f4 size=138 */
    return 0;
}

uint64_t _mtdeletelocks(void)
{
    /* @0x14010b480 size=131 */
    return 0;
}

uint64_t _mtinitlocknum(void)
{
    /* @0x14010b51c size=232 */
    return 0;
}

uint64_t _lock(void)
{
    /* @0x14010b604 size=68 */
    return 0;
}

uint64_t _free_osfhnd(void)
{
    /* @0x14010b658 size=172 */
    return 0;
}

uint64_t _get_osfhandle(void)
{
    /* @0x14010b704 size=132 */
    return 0;
}

uint64_t __lock_fhandle(void)
{
    /* @0x14010b788 size=166 */
    return 0;
}

uint64_t _unlock_fhandle(void)
{
    /* @0x14010b830 size=40 */
    return 0;
}

uint64_t _write_nolock(void)
{
    /* @0x14010b858 size=1886 */
    return 0;
}

uint64_t _write(void)
{
    /* @0x14010bfb8 size=305 */
    return 0;
}

uint64_t _fassign_l(void)
{
    /* @0x14010c2e0 size=66 */
    return 0;
}

uint64_t _cftoe2_l(void)
{
    /* @0x14010c33c size=599 */
    return 0;
}

uint64_t _cftoe_l(void)
{
    /* @0x14010c594 size=268 */
    return 0;
}

uint64_t _cftof_l(void)
{
    /* @0x14010cc90 size=230 */
    return 0;
}

uint64_t _cftog_l(void)
{
    /* @0x14010cd78 size=324 */
    return 0;
}

uint64_t _cfltcvt_l(void)
{
    /* @0x14010cebc size=149 */
    return 0;
}

uint64_t _cfltcvt(void)
{
    /* @0x14010cf54 size=36 */
    return 0;
}

uint64_t _initp_misc_cfltcvt_tab(void)
{
    /* @0x14010cf78 size=54 */
    return 0;
}

uint64_t _stbuf(void)
{
    /* @0x14010cfb0 size=209 */
    return 0;
}

uint64_t _ftbuf(void)
{
    /* @0x14010d084 size=53 */
    return 0;
}

uint64_t _fileno(void)
{
    /* @0x14010d258 size=54 */
    return 0;
}

uint64_t _wchartodigit(void)
{
    /* @0x14010d290 size=405 */
    return 0;
}

uint64_t _iswctype_l(void)
{
    /* @0x14010d428 size=186 */
    return 0;
}

uint64_t iswctype(void)
{
    /* @0x14010d4e4 size=144 */
    return 0;
}

uint64_t _heap_init(void)
{
    /* @0x14010d574 size=76 */
    return 0;
}

uint64_t _calloc_impl(void)
{
    /* @0x14010d5c0 size=175 */
    return 0;
}

uint64_t _NMSG_WRITE(void)
{
    /* @0x14010d670 size=549 */
    return 0;
}

uint64_t _FF_MSGBANNER(void)
{
    /* @0x14010d898 size=67 */
    return 0;
}

uint64_t _controlfp_s(void)
{
    /* @0x14010d8dc size=115 */
    return 0;
}

uint64_t _exception_enabled(void)
{
    /* @0x14010d9b8 size=185 */
    return 0;
}

uint64_t _tzset_nolock(void)
{
    /* @0x14010e3d8 size=1175 */
    return 0;
}

uint64_t _isindst_nolock(void)
{
    /* @0x14010eb30 size=673 */
    return 0;
}

uint64_t __tzset(void)
{
    /* @0x14010edd4 size=59 */
    return 0;
}

uint64_t _isindst(void)
{
    /* @0x14010ee10 size=48 */
    return 0;
}

uint64_t _gmtime64_s(void)
{
    /* @0x14010ee40 size=716 */
    return 0;
}

uint64_t _get_daylight(void)
{
    /* @0x14010f10c size=63 */
    return 0;
}

uint64_t _get_dstbias(void)
{
    /* @0x14010f14c size=63 */
    return 0;
}

uint64_t _get_timezone(void)
{
    /* @0x14010f18c size=63 */
    return 0;
}

uint64_t __getgmtimebuf(void)
{
    /* @0x14010f1ec size=70 */
    return 0;
}

uint64_t _set_abort_behavior(void)
{
    /* @0x14010f2d8 size=28 */
    return 0;
}

uint64_t __check_float_string(void)
{
    /* @0x14010f2fc size=150 */
    return 0;
}

uint64_t _inc(void)
{
    /* @0x14010f394 size=38 */
    return 0;
}

uint64_t _input_l(void)
{
    /* @0x14010f3bc size=4480 */
    return 0;
}

uint64_t __StateFromIp(void)
{
    /* @0x14011053c size=138 */
    return 0;
}

uint64_t __GetCurrentState(void)
{
    /* @0x1401105d0 size=39 */
    return 0;
}

uint64_t __SetState(void)
{
    /* @0x1401105f8 size=12 */
    return 0;
}

uint64_t __SetUnwindTryBlock(void)
{
    /* @0x140110604 size=59 */
    return 0;
}

uint64_t __GetUnwindTryBlock(void)
{
    /* @0x140110640 size=41 */
    return 0;
}

uint64_t __TypeMatch(void)
{
    /* @0x1401106b8 size=291 */
    return 0;
}

uint64_t __DestructExceptionObject(void)
{
    /* @0x1401109bc size=61 */
    return 0;
}

uint64_t __AdjustPointer(void)
{
    /* @0x140110a00 size=35 */
    return 0;
}

uint64_t __BuildCatchObject(void)
{
    /* @0x140110f60 size=170 */
    return 0;
}

uint64_t __InternalCxxFrameHandler(void)
{
    /* @0x140111898 size=549 */
    return 0;
}

uint64_t _initp_eh_hooks(void)
{
    /* @0x140111b08 size=28 */
    return 0;
}

uint64_t _Type_info_dtor(void)
{
    /* @0x140111b24 size=113 */
    return 0;
}

uint64_t _XcptFilter(void)
{
    /* @0x140111cbc size=462 */
    return 0;
}

uint64_t _wwincmdln(void)
{
    /* @0x140111e8c size=90 */
    return 0;
}

uint64_t _wsetenvp(void)
{
    /* @0x140111ee8 size=310 */
    return 0;
}

uint64_t _wsetargv(void)
{
    /* @0x1401121b8 size=240 */
    return 0;
}

uint64_t __crtGetEnvironmentStringsW(void)
{
    /* @0x1401122a8 size=137 */
    return 0;
}

uint64_t _RTC_Initialize(void)
{
    /* @0x14011233c size=56 */
    return 0;
}

uint64_t __security_init_cookie(void)
{
    /* @0x1401123ac size=179 */
    return 0;
}

uint64_t _ValidateImageBase(void)
{
    /* @0x140112650 size=47 */
    return 0;
}

uint64_t _FindPESection(void)
{
    /* @0x140112680 size=70 */
    return 0;
}

uint64_t _IsNonwritableInCurrentImage(void)
{
    /* @0x1401126d0 size=63 */
    return 0;
}

uint64_t _initp_misc_winsig(void)
{
    /* @0x140112714 size=29 */
    return 0;
}

uint64_t raise(void)
{
    /* @0x140112740 size=625 */
    return 0;
}

uint64_t __crtInitCritSecAndSpinCount(void)
{
    /* @0x1401129c4 size=27 */
    return 0;
}

uint64_t _lseeki64_nolock(void)
{
    /* @0x140112a74 size=149 */
    return 0;
}

uint64_t _lseeki64(void)
{
    /* @0x140112b0c size=311 */
    return 0;
}

uint64_t _getbuf(void)
{
    /* @0x140112c44 size=83 */
    return 0;
}

uint64_t _isatty(void)
{
    /* @0x140112c98 size=111 */
    return 0;
}

uint64_t _fputwc_nolock(void)
{
    /* @0x140112d08 size=487 */
    return 0;
}

uint64_t _get_printf_count_output(void)
{
    /* @0x140112ef0 size=24 */
    return 0;
}

uint64_t __crtLCMapStringA_stat(void)
{
    /* @0x14011308c size=1341 */
    return 0;
}

uint64_t __crtGetStringTypeA_stat(void)
{
    /* @0x140113664 size=618 */
    return 0;
}

uint64_t __free_lconv_num(void)
{
    /* @0x140113b48 size=72 */
    return 0;
}

uint64_t __free_lconv_mon(void)
{
    /* @0x140113b90 size=149 */
    return 0;
}

uint64_t ___lc_codepage_func(void)
{
    /* @0x140113c28 size=55 */
    return 0;
}

uint64_t __report_gsfailure(void)
{
    /* @0x140113e98 size=330 */
    return 0;
}

uint64_t _commit(void)
{
    /* @0x1401140e8 size=275 */
    return 0;
}

uint64_t _putwch_nolock(void)
{
    /* @0x1401141fc size=276 */
    return 0;
}

uint64_t __dtold(void)
{
    /* @0x140114938 size=214 */
    return 0;
}

uint64_t _fltout2(void)
{
    /* @0x140114a10 size=192 */
    return 0;
}

uint64_t __crtMessageBoxA(void)
{
    /* @0x140114b40 size=500 */
    return 0;
}

uint64_t _set_error_mode(void)
{
    /* @0x140114d34 size=80 */
    return 0;
}

uint64_t _control87(void)
{
    /* @0x140114d84 size=627 */
    return 0;
}

uint64_t _raise_exc_ex(void)
{
    /* @0x140115020 size=825 */
    return 0;
}

uint64_t _raise_exc(void)
{
    /* @0x14011535c size=39 */
    return 0;
}

uint64_t _set_errno_from_matherr(void)
{
    /* @0x140115384 size=45 */
    return 0;
}

uint64_t _statfp(void)
{
    /* @0x1401153b4 size=17 */
    return 0;
}

uint64_t _clrfp(void)
{
    /* @0x1401153c8 size=29 */
    return 0;
}

uint64_t _ctrlfp(void)
{
    /* @0x1401153e8 size=102 */
    return 0;
}

uint64_t _set_statfp(void)
{
    /* @0x140115468 size=31 */
    return 0;
}

uint64_t _getenv_helper_nolock(void)
{
    /* @0x140115494 size=161 */
    return 0;
}

uint64_t _filbuf(void)
{
    /* @0x140115538 size=346 */
    return 0;
}

uint64_t _ungetc_nolock(void)
{
    /* @0x140115694 size=283 */
    return 0;
}

uint64_t _flswbuf(void)
{
    /* @0x1401157c4 size=399 */
    return 0;
}

uint64_t strtol(void)
{
    /* @0x140115dfc size=48 */
    return 0;
}

uint64_t __ansicp(void)
{
    /* @0x140115e2c size=83 */
    return 0;
}

uint64_t __convertcp(void)
{
    /* @0x140115e80 size=645 */
    return 0;
}

uint64_t __ascii_strnicmp(void)
{
    /* @0x140116108 size=71 */
    return 0;
}

uint64_t _close_nolock(void)
{
    /* @0x140116264 size=188 */
    return 0;
}

uint64_t _close(void)
{
    /* @0x140116320 size=280 */
    return 0;
}

uint64_t _freebuf(void)
{
    /* @0x140116438 size=55 */
    return 0;
}

uint64_t __initconout(void)
{
    /* @0x140116470 size=59 */
    return 0;
}

uint64_t __termcon(void)
{
    /* @0x1401164ac size=59 */
    return 0;
}

uint64_t __strgtold12_l(void)
{
    /* @0x1401171fc size=2297 */
    return 0;
}

uint64_t _fclrf(void)
{
    /* @0x14011866a size=20 */
    return 0;
}

uint64_t __wtomb_environ(void)
{
    /* @0x140118798 size=242 */
    return 0;
}

uint64_t _read_nolock(void)
{
    /* @0x14011888c size=1919 */
    return 0;
}

uint64_t _read(void)
{
    /* @0x14011900c size=370 */
    return 0;
}

uint64_t __mtold12(void)
{
    /* @0x140119180 size=542 */
    return 0;
}

uint64_t __crtCompareStringA_stat(void)
{
    /* @0x1401193a0 size=1242 */
    return 0;
}

uint64_t __crtsetenv(void)
{
    /* @0x140119a20 size=944 */
    return 0;
}

uint64_t _CallSettingFrame(void)
{
    /* @0x14011a100 size=64 */
    return 0;
}

uint64_t _get_fpsr(void)
{
    /* @0x14011a150 size=16 */
    return 0;
}

