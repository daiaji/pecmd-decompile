# FUN_14000e26c 恢复 — 续作交接 (round 2 完成: SEG6-SEG10 全量落地, 任务 A 已完结)

> **状态(2024 round2): FUN_14000e26c 全函数 3948 行已全部移植完毕并双绿**
> (build.sh 95 OK/0 FAIL + gcc link 0 error; nm 确认 FUN_14000e26c 唯一强符号 T)。
> SEG6=2384-2520, SEG7=2521-2800(+5 合成闭合), SEG8=2801-3100(+1, do-while 以 while(0) 暂收口),
> SEG9=3101-3418(-6 回到深度1), SEG10=3419-3944(终段真实返回
> `return ((uint64_t)(uint32_t)_Var39.ft.dwLowDateTime)|((uint64_t)_Var39.ft.dwHighDateTime<<32);`)。
> 全部占位标签(LAB_14001668b/LAB_140014f96)与临时标签占位(LAB_140013555/1400141ec/140014239/
> 140014c93/1400132d5)均已随真身定义删除。TODO(verify) 登记见函数体内注释与本文件末尾。

以下为 round1 的原始交接内容(留档):

文件: /home/daiaji/repo/PECMD反编译/refactored/link_stubs.c (唯一可编辑文件; 不动 core_*/include/globals; 不 git commit; 不重命名)
源: /home/daiaji/repo/PECMD反编译/decompiled.c @7726 (函数体提至 /tmp/opencode/fun_body.txt, 3948 行)
函数签名(保持): uint64_t FUN_14000e26c(uint64_t a..h) — 原 decompiled 为 8 参 _FILETIME; 形参映射:
  param_1=(longlong *)(uintptr_t)a; param_2=b; param_3=(longlong*)(uintptr_t)c; param_4=d;
  param_5=(uint)e; param_6=(uint64_t*)(uintptr_t)f; param_7=g; param_8=(uint64_t*)(uintptr_t)h
局部规则: 所有 _FILETIME 局部 → 联合 FTU64 { uint64_t v; FILETIME ft; } (.v 标量, .ft.dwLow/High 字段);
  (_FILETIME)0x0→0; (_FILETIME)&X→(uint64_t)(uintptr_t)&X; (_FILETIME)((longlong)X+K)→X.v+K;
  *(WCHAR *)X→*(WCHAR *)(uintptr_t)X(.v); CONCATnm/._N_M_ 按 Ghidra 语义展开; extraout_var 拼接残留归一化为 bVar;
  函数调用按 tools/rename_map.json:FUN_*→PECMD_* (未映射保持原名); L"..."→(const uint16_t *)L"...";
  longlong(≡long) vs 'long long' 严格区分(传递处强转 (long long *) 或按定义签名的确切类型);
  '('下注: GCC 15.2 把 incompatible-pointer-types/int-conversion 当错误, 所有跨型传递必须显式 cast.
转换工具: python3 /tmp/opencode/convert_seg.py <start> <end> (fun_body.txt 行号, 语句区; 含 FUN 改名/FTU64 化/指针 deref 化)
  然后按 /tmp/opencode/segN_fixed.txt 手工修 CONCAT/字节片/_FILETIME)FUN/递归调用/类型 cast (参考已完成的 seg1-5 修正清单模式).
验证: bash build.sh core_*.c (>95 OK, 0 FAIL) + gcc -std=c99 -w -Wno-implicit-function-declaration -Iinclude core_*.c link_stubs.c -lm -o /tmp/opencode/link_test (0 undef/multi/error). 每段必绿.

已完成(每段绿): SEG1=283-552, SEG2=553-731, SEG3=732-1478, SEG4=1479-1905, SEG5=1906-2383.
当前函数体: 起始行 ~775 的函数定义, 尾部标记 "  /* SEG5 完结 ... */\n  return 0;\n}" 之前的占位标签:
  LAB_14001668b: (void)0;  LAB_140014f96: (void)0;   ← 这两标签尚未定义, 保留占位直到对应 raw 行并入
  (LAB_140014f96 定义在 raw 3800; LAB_14001668b 在 raw 3941)
新增桩(link_stubs.c 内, 均已 arity 修正): FUN_1400c10c0 (2参), CreatePipe (4), DuplicateHandle(7), SetFilePointer(4),
  TerminateJobObject(2), TerminateProcess(2), GetExitCodeProcess(2), TrackPopupMenu(7), DestroyMenu(1),
  PECMD_ResDecode(2), PECMD_FindVarValue(4, 返回 int64_t), PECMD_RunCommandLine(3, 返回 uint64_t), PECMD_AllocString 未声明(隐式, 沿文件先例)
新增全局: DAT_14013c968(uint8_t), DAT_14011dd24="catch", DAT_14011dcb0[16], DAT_14011d108= {'.','t','m','p',0}; g_hStdIn/Out/Err extern.
DAT 真值: 0x14013e20c(SeDebug标志, 已定义 uint32), 0x14013a330/338/340 = g_hStdIn/Out/Err (core_globals),
  0x14013cf70 已定义, 0x14013c9c8=g_hwndC9C8, 0x14013ccc9=g_flagCCC9, 0x14013ca90(惰性 LPCWSTR 槽), 0x14013ca4c(short)待定义(未用真值, 见正文),
  0x14013c968=0, 0x14013cb09(uint8_t 标志) 待定义, 0x14013c960(void* 函数槽) 待定义, 0x14013d130 extern, 0x140147000 已定义, DAT_14013cb18 已定义[8].
  提取脚本: python3 tools/pe_data_extract.py ../PECMD原始.EXE <va> <n>
剩余分段 (raw 2384-3948; 每段结尾必须括号深度 0, 或用"合成闭合尾部"法 — 见下):
  SEG6 = 2384-2520   (深度 0 @2520)
  SEG7 = 2521-2800   (中段切, 尾部补深度 D=depth(2521,2800) 个 '}' 占位, 标记 /* ---- SEG占位闭合(下段替换) ---- */; 下段把该标记+合成}替换为续文)
  SEG8 = 2801-3100   (同上)
  SEG9 = 3101-3418   (深度 0 @3418)
  SEG10 = 3419-3944  (终段: 3944 行 'return _Var39;' 为真实返回; 替换尾部 return 0; 并用 _Var39.v 拼装返回值:
     return ((uint64_t)(uint32_t)_Var39.ft.dwLowDateTime) | ((uint64_t)_Var39.ft.dwHighDateTime << 32); 且删除全部占位标签与合成闭合)
  raw 3800 LAB_140014f96: 定义并入后删占位; raw 3941 LAB_14001668b: 定义并入后删占位.
已知标签注解: LAB_140014f8e @3797, LAB_1400162b9 @3842 也是后段标签. 函数尾部 raw 3842-3944 是清理+返回路径.
关键诀窍拼接: 每段 = convert_seg.py 输出 → 修 specials (grep segN_conv.txt 查 CONCAT|_FILETIME\)|\._N_|FUN_140|0x14[0-9a-f]{7}|+ -|undefined[0-9]|L") → 
  用 python 以锚文本替换前段尾部: 前段 tail 更新为 新段语句 + 新占位标签 + (深度非0时合成闭合) + return 0; }.
  新增依赖函数 → 在函数定义前 (~line 763 前置声明块) 增加 extern (签名必须与既有 def 完全一致, 注意 longlong vs long long, 返回类型 int64_t/uint64_t 别混);
  link_stubs 内既有 (void) 桩配 N 参调用 → 修该桩 arity(返回类型保持), 并检查同 TU 其它 decl 一致.

## round2 补充登记 (SEG6-10 落地时的取舍, 供 REVIEW 引用)
- TODO(verify) 清单位置: link_stubs.c 内 "FUN_14000e26c" 函数体中 grep 'TODO(verify)':
  1) FUN_14001bbac(MsgWait 包装) 还原体返回 void 不透传结果 → 两处调用点按包装体内联展开为
     MsgWaitForMultipleObjects(...) 以保留反编译视图的 local_9a0 数据流。
  2) DAT_14013a24f: link_stubs 既有定义 @~508 为 int64_t 初值 0; 本段提取文件真值字节=0x01,
     未改既有定义(避免影响其它还原体), 差异在此登记。
- CONCAT71(extraout_var_N,bVar16) 全部按 "(bVar16 != false)" 简化 (寄存器拼接残留, bool 即真值)。
- local_ca8._N_M_/SUB84 字节片全部展开为 *(uint32_t/uint64_t *)&local_ca8[off] / (u>>32)。
- param_5._0_1_ 字节写展开为 param_5 = (param_5 & 0xffffff00u) | b。
- (_LUID)&local_af8 按既定先例(core_b1_remaining.c ReportPelogonStatus)取地址值本身: (uint64_t)(uintptr_t)&local_af8。
- FUN_1400e7414/FUN_1400229f8 无任何还原体 → 最小桩(返回0/void), 失败保守路径。
- CreateNamedPipeA 返回 INVALID_HANDLE_VALUE(走真实失败分支); ShellExecuteExW 返回 1(成功)。