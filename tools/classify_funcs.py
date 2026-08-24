#!/usr/bin/env python3
"""classify_funcs.py — PECMD 反编译函数分类 (业务 vs CRT内联 vs thunk)

用法: python3 tools/classify_funcs.py
输出: tools/pecmd_func_classes.json + 分类摘要  (B0 工具债固化, §8.3检查单7)

PECMD 布局 (PE32+ x64, ImageBase=0x140000000):
  .text   0x140001000 ~ 0x14011A000  代码
  .rdata  0x14011B000 ~ 0x14013A000  字符串/命令表/导入
  .data   0x14013A000 ~ 0x140140000  全局变量 (DAT_14013xxxx)

分类判据:
  THUNK   — 跳转/导入转发桩 (size<=7 且无函数块), 不重构
  CRT     — 大量 CRT 函数调用 (memcpy/strlen/_initterm/__CxxFrameHandler 等)
             且无 .data/.rdata 业务偏移引用 → 标准库内联, 跳过
  BIZ     — 引用 0x14013xxxx (全局数据) 或 0x14011Bxxx~0x140139xxx (业务表)
  SMALL   — 无特征小函数, 逐个快速重构
"""
import re
import json
import sys
import os

BASE = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))  # refactored/
SRC = os.path.join(os.path.dirname(BASE), 'decompiled.c')

BLOCK = re.compile(
    r'/\* =+ (\S+) @ ([0-9a-f]+)  size=(\d+) =+ \*/\n(.*?)(?=\n/\* =+ |\Z)',
    re.DOTALL,
)

# 标准 CRT / Win32 API 调用特征 (PECMD 静态链接 MSVC CRT)
CRT_CALLS = re.compile(r'\b(memcpy|memmove|memset|strlen|strcmp|strncmp|strcpy|strcat|'
                       r'strchr|strstr|malloc|free|realloc|calloc|abort|exit|_exit|'
                       r'_initterm|_initterm_e|__CxxFrameHandler|_CxxThrowException|'
                       r'_RTC_CheckStackVars|_RTC_CheckStackVars_|_RTC_InitBase|'
                       r'_RTC_Shutdown|__security_check_cookie|__security_init_cookie|'
                       r'_seh_filter|_XcptFilter|_invalid_parameter|_invoke_watson|'
                       r'atoi|atol|strtol|strtoul|strtod|sscanf|sprintf|fopen|fread|'
                       r'fwrite|fclose|printf|puts|putchar|tolower|toupper|isdigit|'
                       r'isalpha|isxdigit|isspace|wcscpy|wcscat|wcslen|wcsncmp|wcscmp|'
                       r'wcstok|wcstol|wcstoul|wcstod|swprintf|_snwprintf|_vsnwprintf|'
                       r'getcwd|_chdir|_mkdir|_rmdir|_stat|_findfirst|_findnext|'
                       r'GetProcAddress|LocalAlloc|LocalFree|HeapAlloc|HeapFree|'
                       r'lstrcpy|lstrcpyW|lstrlen|GetModuleHandleW|GetModuleFileNameW|'
                       r'LoadLibraryW|GetTickCount|GetSystemTime|GetLocalTime|'
                       r'wsprintfW|wvsprintfW|StrCmpNIW|StrCmpW|StrCmpIW|StrStrW|'
                       r'memcpy_s|strcpy_s|wcscpy_s|strcat_s|strncpy_s|memmove_s|'
                       r'wcsicmp|_wcsicmp|_wcsnicmp|_wcsicmp_l|_strnicmp|_strlwr|'
                       r'_strupr|_wcslwr|_wcsupr|_strdup|_wcsdup|_stricmp|_snprintf|'
                       r'_vsnprintf|_vscwprintf|_scwprintf|_wtoi|_wtoi64|_itow|'
                       r'_ui64tow|_i64tow|_ultow|_itow_s|_ultow_s|_wcstoi64|_wcstoui64|'
                       r'_strtoui64|_strtoi64|_ftol|__ftol2_sse|_ftol2|chkesp|'
                       r'_LocaleUpdate|_getptd|_getptd_noexit|_amsg_exit|_callnewh|'
                       r'_purecall|__alloca_probe|__chkstk|_crt_|__crt|_onexit|atexit|'
                       r'_cexit|_cinit|_mtinit|_mlock|_munlock|_lock|_unlock|'
                       r'_lock_fhandle|_unlock_fhandle|_get_osfhandle|_open|_close|'
                       r'_read|_write|_lseeki64|_commit|_get_errno|_set_errno|'
                       r'_errno|_doserrno|_get_daylight|_get_timezone|_get_tzname|'
                       r'tzset|_tzset|_localtime64|_gmtime64|_time64|_ftime64|'
                       r'mktime|_mktime64|_difftime|_fstat|fstat|_sopen|_sopen_s|'
                       r'_vsnwprintf_l|_snwprintf_l|_wcslwr_s|_wcsupr_s|_wcsicmp_l|'
                       r'strnicmp|wcsnicmp|_wcsnicmp_l|_strnicmp_l|memcmp|wmemcmp|'
                       r'wcschr|wcsrchr|_wcsrev|_strrev|_ultoa|_itoa|_ltoa|'
                       r'_ui64toa|_i64toa|_gcvt|_fcvt|_ecvt|sprintf_s|swprintf_s|'
                       r'strcpy_s|strcat_s|strncat|strncat_s|wcsncat|wcsncat_s)\b')

# 业务数据引用: .data 全局变量 DAT_14013xxxx / PTR_DAT_14013xxxx,
# .rdata 业务表/字符串 0x14011B000~0x140139FFF
DAT_REF = re.compile(r'DAT_140[0-9a-f]{6}|PTR_DAT_140[0-9a-f]{6}')
RDATA_REF = re.compile(r'0x140(?:11[bcdef]|12[0-9a-f]|13[0-8])[0-9a-f]{3}')

# 字符串常量引用 (.rdata 字符串区)
STR_REF = re.compile(r'L?"[^"]{2,}')

# 指令序列特征: E8 近调用 (无意义, Ghidra 已解析为函数调用, 无需)

def classify(f):
    b, n = f['body'], f['name']
    if n.startswith('thunk_') or f['size'] <= 7:
        return 'THUNK'
    biz = len(DAT_REF.findall(b)) + len(RDATA_REF.findall(b))
    if biz >= 1:
        return 'BIZ'
    crt = len(CRT_CALLS.findall(b))
    if crt >= 2:
        return 'CRT'
    # 大函数无业务偏移但无 CRT → 可能是内联展开的算法, 归 BIZ 待人工
    if f['size'] > 400:
        return 'BIG_UNKNOWN'
    return 'SMALL'


def main():
    src = open(SRC, encoding='utf-8', errors='replace').read()
    funcs = []
    for m in BLOCK.finditer(src):
        name, addr, size, body = m.group(1), m.group(2), int(m.group(3)), m.group(4)
        funcs.append({'name': name, 'addr': addr, 'size': size,
                      'lines': body.count('\n'), 'body': body})

    # 排除已重构 (按 refactored/ 下 .c 文件头注释的 @0x140xxxxxx 地址)
    done = set()
    for path in os.listdir(BASE):
        if not path.endswith('.c'):
            continue
        t = open(os.path.join(BASE, path), encoding='utf-8', errors='replace').read()
        for m in re.finditer(r'@0x(140[0-9a-fA-F]{6})', t):
            done.add(m.group(1).lower())
    for m in re.finditer(r'真实体\s*@0x(140[0-9a-fA-F]{6})', t) if done else []:
        done.add(m.group(1).lower())

    cat = {}
    for f in funcs:
        if f['addr'].lower() in done:
            cat.setdefault('DONE', []).append(f)
            continue
        cat.setdefault(classify(f), []).append(f)

    order = ['THUNK', 'CRT', 'BIZ', 'BIG_UNKNOWN', 'SMALL', 'DONE']
    for c in order:
        fs = cat.get(c, [])
        print(f"{c:12s} {len(fs):4d}  {sum(f['lines'] for f in fs):7d} 行")

    out = {c: [{'name': f['name'], 'addr': f['addr'], 'size': f['size'], 'lines': f['lines']}
               for f in fs] for c, fs in cat.items()}
    json.dump(out, open(os.path.join(os.path.dirname(os.path.abspath(__file__)), 'pecmd_func_classes.json'), 'w'), indent=1)
    print('已存 tools/pecmd_func_classes.json')


if __name__ == '__main__':
    main()
