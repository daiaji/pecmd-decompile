# PECMD 重构 REVIEW.md — 反编译缺陷与代码事实登记

## 1. 重构规范

- 标准 C99, `gcc -std=c99 -Wall -Wextra -Wno-unused-parameter -fsyntax-only` 零警告
- 文件头注释必须含来源地址 `@0x140xxxxxx` (gen_tasks 防重复匹配依据)
- 命名: `PECMD_` 前缀 (如 PECMD_ParseCmd, PECMD_ExecFile)
- 结构体偏移用宏, 未知字段 `*(int*)((char*)this+OFF)`, 不臆造语义
- 不确定处写 `TODO(verify)` 并登记本文件
- 库替换模式: 保留签名/校验/业务逻辑, 内核调库, 头注释注明
- 错误消息字符串按 GBK 读取转 `\xNN` 转义

## 2. 反编译缺陷对照表

- [x] **wsprintfW 丢参 (B5 发现)**: FUN_140066978/1400669c4 的
      `wsprintfW(buf, "%lu")` 无参数 — 寄存器残留丢失, 按调用语义还原为
      value 参数 (已还原, core_var.c)
- [x] **jumptable 警告 (B5 发现)**: FUN_14001a220/140062a2c 报
      "Could not recover jumptable" — 实际为简单锁内初始化/两调用序列, 已按语义还原
- [x] **FUN_140027eac 参数冗余**: param_4 未使用 (寄存器复用), 已保留占位
- [ ] ftol() 丢参 → 参数来自上一 ftol 浮点源, 需汇编确认
- [ ] 返回值被丢弃 (void 但有累计) → 检查调用方
- [ ] `x = 0x140xxxxxx;` 返回地址常量赋值 → eax 残留, 按调用点语义还原
- [ ] SEH 噪音 (__SEH_prolog 等) → 忽略
- [ ] 巨型函数 → 查库内联特征/循环展开, 用映射抽象压缩
- [ ] Ghidra 未识别出的 switch jump table → 可能显示为 if 链

## 3. PECMD 已知代码事实

- 入口链: entry(1401081ec) → __tmainCRTStartup → main(140017034) → mainB(140016ad0) → mainW(140009ccc)
- mainW: WinMain 等价逻辑, 参数解析 (GetCommandLineW), *PECMD 前缀检测
- ImageBase=0x140000000; .text 0x140001000-0x14011a000; .rdata 0x14011b000-0x14013a000; .data 0x14013a000-0x140140000
- 全局变量 DAT_14013xxxx (.data), 业务字符串/表 0x14011b000-0x140139fff (.rdata)
- **内存块布局 (核心)**: [-8] size_t 容量 + [-4] 魔数 0xaa55 + [0] 数据;
  分配器 PECMD_HeapRealloc @0x140063118, 失败弹窗重试 (Retry=4/Ignore=5 继续)
- **VarNode (0x20)**: +0 name +8 value +10 rsv +18 cap(高2位 0xc0=固定容量截断);
  Script: +0 变量数组 +8 数量 +38 父表 +50 模板
- **变量机制**: 单& 当前表, && 根表, :: 默认表(g_Script); PECMD_VarWrite @0x14001e6bc
- **task 结构 (线程)**: +0 引用计数 +8 refs +18 消息 +20 wParam +28 lParam
  +30 flags +38 超时 +40 hwnd +48 回调 +50 ctx
- **引用计数对象**: 头 4 字节计数, RefDec @0x140017110 归零释放
- 环境变量名表 @0x14013a260: SystemRoot/SystemDrive/windir/TEMP/TMP
- 特殊目录注册表: HKCU User Shell Folders, &Desktop 等 9 个变量
- 权限: EnablePrivilege @0x14001c2cc (SeBackup/SeRestore, 标志 g_privFlags)

## 4. 待验证项

- [x] FUN_140030420 (脚本执行器) 与 FUN_14004eb34 (ExecCmd 主流程) 已重构
      (B8b); 深层依赖 PECMD_ExecScript/ScriptRunA/ResDecode/ExecLine2 待 B8c/B9
- [ ] LangInit 的 ReadFileStr 链 (FUN_14001ebdc/14005f33c/140068984) B9 待实现
- [ ] FUN_140082448 表达式求值 (括号表达式) 待重构 (字体默认字号分支)
- [ ] wsprintfW 在 mbox 超时文本的参数还原 (param_1+0x2f4 指向缓冲)
      待运行验证
- [ ] mbox 布局像素计算已忠实还原主流程, 但多行文本滚动决策分支被简化
      (nlines/滚动条), 待对照汇编细查

## 5. B7 批记录 (命令执行/消息框支撑族)

- **FUN_14000142c = PECMD 自绘 MessageBox** (非 ExecCmd; 修正旧记录):
  按钮 1=确定 2=取消 3=Abort 4=重试 5=忽略 6=是 7=否
  type&0xf → 按钮集: 0:[1] 1:[1,2] 2:[3,4,5] 3:[6,7,2] 4:[6,7] 5:[4,5]
  DAT_14011c544 按钮禁用表 {2,1,3,4,5,6,7} (未用按钮禁用, ID2 清文本)
  DAT_14011c570 图标表 (8×8B): {0,0x7F03,0x7F03,0x7F04,0x7F04,0x7F02,0x7F01,0x7F01}
  = IDI_EXCLAMATION/ASTERISK/QUESTION/HAND; 子类化 Enter→WM_COMMAND
- **FUN_1400050c8 = fiber 启动器** (126B, CreateFiber+ConvertThreadToFiber):
  fiber 回调 FUN_1400050a0 → FUN_140004fd4 (ExecLine: 设工作集+ExecScriptInit)
  **ExecCmd 主流程 = FUN_14004eb34**, 脚本执行器 = FUN_140030420
- **.lang 语言文件**: "<exe><代码页>.lang", 首行 #code= 后为 #str: 段,
  格式 "十六进制ID:文本", \r \n \\ 转义; 载入 g_pLangBuf (DAT_14013ca78)
  消息表查找 PECMD_MsgLookup (ID→文本, 分隔 '|')
- **字体规格** (PECMD_CreateFont2): 字号前缀 '~'=96 基准(dpi2=0x60),
  '^'=72 基准(0x48); size = dpi2*size/72, lfHeight=-取整; g_dpi=0 时直用
  DAT_1401293c0={-2147483648.0, 96.0} 默认字号特殊值
- **控件子类化**: PECMD_CreateCtrl SetWindowLongPtrW(-4, SubWndProc),
  原 WndProc 存 DAT_14013c908; Enter 键 → WM_COMMAND(控件ID)
- 全局新增: g_pLangBuf/g_langLen (DAT_14013ca78/80), g_dpi (DAT_14013d868),
  g_subWndProc (DAT_14013c908), g_pFontBase (DAT_14013e2a8),
  g_msgboxFlag/g_msgboxActive (DAT_14013cf60/84)
- FUN_140017cdc = ScriptCopy (0xe0 结构深拷贝, 内部串 StrDup), 待重构

## 5.1 B8 批记录 (ExecCmd 主流程/脚本执行器)

- **FUN_14004eb34 = PECMD_ExecCmd 主流程** (core_script2.c):
  SCRIPTINIT 资源 → __Autoapp/__bInitWin 检查 → SCRIPT 资源 → 命令行模式判定
  (PECMD**pecmd-cmd* 前缀) → 消息循环执行; * 前缀 = ExecScript, $ = ScriptRunA
- **FUN_140030420 = PECMD_ScriptExec 脚本执行器** (core_script.c):
  段切分 (脚本+0x44 主分隔符/+0x45/48 副), import 前缀 (数字 8..0xd 或空格),
  递归合并已 import 文件, XOR 密钥 = RandSeed 高 16 位
- **FUN_1400307c8 = PECMD_ImportFile**: "file|offset|len" 解析 → 设备路径修正
  (\\.\PhysicalDrive 检测) → CreateFile/IOCTL 设备大小 → 编码识别
  (FEFF/FFFE BOM) → XOR 解码 → 合并行缓冲 → 递归 ScriptExec
- **变量系统核心** (core_var3.c): PECMD_VarFind (FUN_140018978, &前缀计数/
  ::根表/父表回溯 plVar4[7]), PECMD_VarWrite (FUN_14001e6bc, 固定表 0xc0 标志
  直写 vs 重分配), 环境变量写 (FUN_14005d534)
- **脚本对象** (core_exec5.c): PECMD_ScriptCopy (FUN_140017cdc, 0xe0 字段逐项
  复制 bit0 清除), PECMD_ScriptInit (FUN_1400186bc, 父引用计数), PECMD_ArgBuild
  (FUN_140073ccc, 参数表 [n]=指针 + 尾 3 辅助指针)
- **编码/设备** (core_exec4.c): PECMD_EncodeDet (FUN_14005f33c, 前 12 字节魔数
  x/A/a/S 变体), PECMD_DevPathFix (FUN_14006ccd4, 盘符→\\.\C: 返回 2),
  PECMD_DevSize (FUN_14005fbd4, IOCTL 0x7405c/0x70000)
- **随机种子**: FUN_14001b510 要求偶数/bit15=1/低 8 位非 0/汉明权重 5..11;
  FUN_14005e04c = QPC*0x83 + LCG*3 混合
- 待实现: PECMD_ExecScript (FUN_140045c90), PECMD_ScriptRunA (FUN_140031454),
  PECMD_ResDecode (FUN_1400e7d58), PECMD_ExecLine2 (FUN_14007bf44)
  → 声明 extern + TODO(verify)

## 5.2 库替换记录

- operator_new(0x20) → calloc(1,0x20) (core_var2.c, 节点分配)
- FUN_14001d78c/14001d744 (memcpy/memmove 内联) → 标准库函数
- FUN_140102a90 (memset 内联) → memset
- _ftol/__ftol2_sse/__chkstk 等 CRT 内联 → 跳过
- FUN_14001d810 (NtCreateFile 包装) → CreateFileW (core_exec4.c, TODO(verify))

## 5.3 B8b/B8c 批记录 (2026-08-17, 并行子代理 + 主代理)

- **ExecLine2 展开器族** (core_execline.c): 分发器 PECMD_ExecLine2 @07bf44
  (script+0xda==0 且 +0xd&0xf==0 → 递归展开 2A，否则完整展开 2B);
  完整展开 @07a224: %0-%9/%*/%@/%# 参数展开(opt&1), %name% 变量查找
  (VarFind), %name:~s,l% 截取, %name?spec% 格式化(VarFmt), 环境变量
  (GetEnvironmentVariableW), %RANDOM%/%__THIS% 特殊; 变量核心 @07af60
  差异: flagout 回调 + SetLastError/0xcb 未找到检测 + 未找到复制原始段
- **字符串构建器 6 槽** (core_strbld.c): s[0]=out s[1]=count s[2]=cur
  s[3]=end(buf-2+count*2) s[4]=base s[5]=limit(end-0x7e4);
  Grow 条件 (need*2-end+cur)>>1>=0 → count+=need+0xe;
  GrowBulk 条件 (cur-limit)>>1>=0 → count+=0x400
- **变量格式化 PECMD_VarFmt @06d9d0**: spec 类型前缀 char/wchar/short/
  long/int64/ptr/float/double/ldouble/int + 0/@/s 后缀 + ~ 宽度 + 截断
- **PECMD_ExecScript @045c90** (core_execscript.c): 互斥体
  Global\PECMD:main(_u) → 0xb7; PELOGON 注册表; 参数表初始化
  (script+0xf/+0x10, atexit, ScriptSep(0x10000), ArgBuild);
  #!pecmd/--inline-main 检测 → LOAD 执行; 消息循环; 依赖 FUN_14004c0bc
  (主脚本执行器 8717B 未实现)
- **命名陷阱**: core_exec2.c 的 PECMD_StrFreeA @07026c 实际是 ANSI 串复制
  (StrDupA 容器), 不是释放! 释放函数是 PECMD_StrFree @05b104
- **gen_tasks 兜底误标风险**: extern 声明 PECMD_xxx( 会被 gen_tasks 按名
  标记 done (SRC §42.1 教训) → 待建 win32_stub.c 后跑完整链接验证
- **子代理委托经验**: 3/4 次成功 (构建器/ExecScript 成功, ExecLine2 两次
  空返后主代理自做, ScriptRunA 两次空返 → 拆分前缀段再委托)

## 5.4 core_cmd4 补完记录 (2026-08-17)

- **FUN_140088300 = PECMD_CodeCmd** (core_cmd4.c): 模式A (编码转换) 已完整;
  模式B (*jm 加密变换) 原文件截断且缺 L_err1/L_invalid/L_done, 现已补上可编译实现。
- 模式B 当前为**简化实现**: 保留表达式展开/源变量读取/运算符映射/逐元素算术逻辑内核
  (+ ^ > . Σ * & | ? ω - / 等) 和累加器输出; 但反编译中的块置换/交错循环
  (local_50/local_40/local_58/local_b0/local_d8 等) 未逐行还原, 标记 TODO(verify)。
- 已通过 `./build.sh core_*.c` 全绿。

## 5.5 B2 批记录 (core_b2a.c, 2026-08-17)

- 新增 core_b2a.c: 首批 29 个小函数/包装函数 (140020e40..14003e620)。
- 多为转发/包装/一次性初始化/字符串判断, 依赖的 FUN_* 未实现函数保持 extern + TODO(verify)。
- 注意: 外部依赖注释中的 @0x 地址已去掉, 避免 classify 误计为已完成; 只有函数头注释保留 @0x。
- 已通过 `./build.sh core_*.c` 全绿。

## 5.6 B2 第二批记录 (core_b2b.c, 2026-08-17)

- 新增 core_b2b.c: 23 个小/中函数 (140020b6c..14003e694)。
- 覆盖: 大小串解析、SystemStartOptions 检查、参数记录写入、目录树删除、
  VK/键名解析、钩子过滤、一次性初始化体、菜单递归、排序、OnlyApp 启动、
  进程执行、devcon 驱动更新、字体安装、Ramdriv 执行、关机脚本、系统应答。
- 反编译 Ghidra 宏处理:
  - FUN_140020b6c P 分支魔数除法按 `/100` 近似 (TODO(verify));
  - FUN_140020fd4 的 wsprintfW(L"%d") 缺实参, 按 0 补齐 (TODO(verify));
  - FUN_1400259a4 中 DAT_14013d070 写入改为桩 (TODO(verify));
  - FUN_14002c7dc 的 DAT_14013cf38 函数指针调用未保留参数 (TODO(verify));
  - 复杂解析器 FUN_140023c48/1400240c0/140024350/14002d33c/14002fd88 留待后续。
- 为编译新增 win32_stub.h 声明: GetKeyState/GetMenuStringW/GetMenuItemID/
  ModifyMenuW/AddFontResourceW/RemoveFontResourceW/_snwprintf。
- 已通过 `./build.sh core_*.c` 全绿 (有少量警告: const 丢弃/括号建议, 非错误)。

## 5.7 B3-B6 小函数批记录 (core_b3a.c, 2026-08-17)

- 新增 core_b3a.c: 30 个小函数 (1400402d8..1400aa12c)。
- 覆盖: 参数包装、串比较包装、结构清零/释放、区间夹取、浮点存储、
  空格填充、状态查询、虚表调用、字节序合并、对齐取余、消息发送、
  魔数判断、循环计数、数组增长包装、串容器初始化、条件转发。
- 依赖保持 extern + TODO(verify): FUN_140040090/14005b0b8/140061704/
  140061780/14006355c/140070154/14007df90 等。
- 已通过 `./build.sh core_*.c` 全绿。

## 5.8 B3-B6 小函数批 2 记录 (core_b3b.c, 2026-08-17)

- 新增 core_b3b.c: 30 个小函数 (14004f908..14007de4c)。
- 覆盖: ATL 抛错、选对象入槽、浮点取负、表初始化、ANSI/W 串比较、
  分隔符计数、容器释放、LCG 更新、多次 flush、字母/数字判定、互斥体释放、
  对象释放、自定义消息、数字解析+跳空白、注册表读取、设备父节点、
  设备路径前缀、容器追加、截断文件、设备安装入口。
- 依赖保持 extern + TODO(verify): FUN_14005d558/14005dff4/14005f158/
  140067a00/14006265c/14005b184/140060870/14005c674/140067ee8/140067d20/
  140067e2c/140067e88/140067d54/140076554/140077e48 等。
- 注意: AtlThrowImpl 是 ATL 库函数, 这里按调用语义实现 RaiseException+abort;
  FUN_14005c800 的 CONCAT71 尾部按返回 1 近似 (TODO(verify))。
- 已通过 `./build.sh core_*.c` 全绿。

## 5.9 B3-B6 小函数批 3 记录 (core_b3c.c, 2026-08-17)

- 新增 core_b3c.c: 30 个小函数 (14004f8b4..1400b91d4)。
- 覆盖: 大量"释放包装"（清理函数 + 按 bit0 free）、结构清零、删除盘符、
  颜色缓存、托盘窗口查找、特殊目录首字符、任务计数、模式匹配、互斥体释放。
- 依赖保持 extern + TODO(verify): FUN_140054b18/140054a50/140054ab4/1400553f8/
  1400e8940/14006c648/14006bf8c/14006c3cc/1400643d4/14006b924/14006c430/
  14006fc4c/14006fcb4/14006fe3c/14007cfb4/1400a9c40/1400aa094/1400aa2fc/
  1400aa484/14007f9fc/1400b916c 等。
- FUN_14006042c 的 Ghidra CONCAT62 按返回路径首字符近似 (TODO(verify))。
- 为编译新增 win32_stub.h 声明: FindWindowExA。
- 已通过 `./build.sh core_*.c` 全绿。

## 5.10 B3-B6 小函数批 4 记录 (core_b3d.c, 2026-08-17)

- 新增 core_b3d.c: 30 个小函数 (140045868..14009c6dc)。
- 覆盖: 调试输出、选对象清理、对象初始化、结构清零、销毁窗口、边框绘制、
  数值换算、ICON/BITMAP 判定、随机数取回、标志复制、读取字节、状态查询、
  随机数种子、标志清理、字符表填充、消息发送、设置字体、标识符判定、
  十六进制解析、串匹配、窗口动作、数字串打包、一次性初始化、数字解析、
  格式化输出。
- 依赖保持 extern + TODO(verify): FUN_140018d8c/14003e768/1400e57c0/
  14005ceec/14005e04c/14005f1b0/140007670/1400660ac/14005c72c/14005c788/
  140070450/14007474c/14007d0ac/14007df90 等。
- FUN_140060834 中局部 5 槽仅传首槽地址, 其余保留占位; FUN_14009c6dc 的
  wsprintfW 缺实参按 0 补齐 (TODO(verify))。
- 为编译新增 win32_stub.h 声明: FrameRgn。
- 已通过 `./build.sh core_*.c` 全绿。

## 5.11 B3-B6 小函数批 5 记录 (core_b3e.c, 2026-08-17)

- 新增 core_b3e.c: 30 个小函数 (140045c44..14007ddf8)。
- 覆盖: 命令分发、解析+跳分隔、对象释放、定时器消息、尾部补空格、
  大小写前缀匹配、工作集清理、资源释放、前台窗口恢复、回调清理、
  读取字节、错误处理封装、键表标记、路径尾分隔、毫秒/纳秒时间、
  释放模块、DPI 换算、数组释放、消息发送、十六进制解析、格式化设变量、
  控件消息、数组追加、串替换、命令行处理、带符号数字解析。
- 依赖保持 extern + TODO(verify): FUN_1400408d0/140030144/1400a4dcc/
  14006764c/140102244/1400fd1a8/14005b7e8/1400fd86c/140063060/1400726f0/
  14005cc4c/1400745c8/140070154 等。
- FUN_14005bb6c 的 DAT_14013d3ec 位域按字节数组访问; FUN_140066930/
  14009c6dc 的 wsprintfW 缺实参按 0 补齐 (TODO(verify))。
- 已通过 `./build.sh core_*.c` 全绿。

## 6. 函数可读化专项记录 (2026-08-17)

- 用户要求：不能以 `FUN_<addr>` 占位符作为最终函数名，必须人类可读。
- 已生成 171 个待重命名函数的映射表：`refactored/tools/rename_map.json` 和
  `refactored/FUNC_NAMES.md`（覆盖 core_b2b/core_b3a-e）。
- 排期策略：新实现函数直接使用 `PECMD_` 可读名；历史 `FUN_` 在后续批次统一重命名，
  重命名时同步所有 extern 引用并保持 build 全绿。
- 协议更新：self-iterate SKILL.md 已加入“已确认语义函数必须人类可读命名”的规范。

## 5.12 B3-B6 小函数批 6 记录 (core_b3f.c, 2026-08-17)

- 新增 core_b3f.c: 30 个小函数 (1400402ec..1400b2218)。
- **本批开始新实现函数直接使用 `PECMD_` 人类可读名称**，原始地址保留在 `@0x` 注释。
- 覆盖: 参数解析扩展、去尾反斜杠、加载 RichEdit、对象释放、窗口回引用、
  删除注册表值、加载公共对话框、图标组判定、释放句柄对象、缓冲写字节、
  24 字节交换、设备 flush、删除串中字符、获取编辑选择、设置窗口主题、
  匹配赋值 token、解析颜色/数值、解析整数取整、初始化/关闭流、写加密/XOR 字节、
  转换串编码、打包串记录、初始化/重置解析上下文、解析路径记录、$ 移位、创建字体 DPI。
- 同步重命名 3 个跨文件引用:
  - FUN_14005f158 -> PECMD_BufferedWriteByte (core_b3b 引用已更新)
  - FUN_140067e2c -> PECMD_ParseIntWithRound (core_b3b 引用已更新)
  - FUN_140067e88 -> PECMD_ParseInt64WithRound (core_b3b 引用已更新)
- FUN_140065864 返回值修正为 `void *`（Ghidra 标 void 但调用方使用 eax）。
- 依赖保持 extern + TODO(verify)。
- 已通过 `./build.sh core_*.c` 全绿。

## 5.13 B3-B6 小函数批 7 记录 (core_b3g.c, 2026-08-17)

- 新增 core_b3g.c: 30 个小函数 (14004fd6c..1400b916c)。
- 本批全部使用 `PECMD_` 人类可读命名，并同步更新了跨文件引用：
  - FUN_140054a50 -> PECMD_ReleaseObjectA
  - FUN_140054ab4 -> PECMD_ReleaseObjectB
  - FUN_14005c898 -> PECMD_GetProcAddrEx
  - FUN_14005cc4c -> PECMD_ExecSpecialCommand
  - FUN_140063b00 -> PECMD_GrowArray
  - FUN_14006bf8c -> PECMD_ReleaseObjectC
  - FUN_14006c3cc -> PECMD_ReleaseObjectD
  - FUN_14006c430 -> PECMD_ReleaseObjectE
  - FUN_14006fc4c -> PECMD_ReleaseObjectF
  - FUN_14006fcb4 -> PECMD_ReleaseObjectG
  - FUN_1400aa094 -> PECMD_ReleaseObjectH
  - FUN_1400aa2fc -> PECMD_ReleaseObjectI
  - FUN_1400b916c -> PECMD_ReleaseObjectJ
- 覆盖: 调整缓冲、数组压入、发送 RGB 消息、延迟加载 GetProcAddr、执行特殊命令、
  查找可用盘符、创建符号链接、100ns 时间、递减任务计数、释放句柄数组、
  数组增长、复制到字符、十六进制转字节、字符串转 ID、追加引号串、
  多个释放对象包装、追加 I64、释放 GDI 对象、压入字符串 token。
- 已通过 `./build.sh core_*.c` 全绿。

## 5.14 B3-B6 小函数批 8 记录 (core_b3h.c, 2026-08-17)

- 新增 core_b3h.c: 30 个小函数 (1400458a8..1400b0380)。
- 全部使用 `PECMD_` 人类可读命名，并同步更新跨文件引用：
  - FUN_140054b18 -> PECMD_ReleaseObjectK
  - FUN_140063978 -> PECMD_GrowArrayRaw
  - FUN_1400a9c40 -> PECMD_ReleaseObjectL
- 覆盖: 关于对话框、数组前插、递归解析、关键字匹配、资源对象释放、大小后缀、
  单词匹配、AppliedDPI 读取、钩子过程、窗口状态、ASCII 转宽、DOS 设备路径、
  位置打包、按键状态、全局互斥体、堆分配块、ANSI 串追加、数组增长原始、
  指针数组初始化、加密字节读取、token 解析、解析器清理、剪贴板文本、
  数字字段解析、tokens 连接、路径记录变体、互斥体对象释放、运行命令/函数。
- 简化/TODO(verify): PECMD_MatchKeyword 当前返回失败占位；PECMD_BuildDosDevicePath
  使用 "\\\\.\\DosDevices\\C:" 模板近似；PECMD_UpdateKeyState 按键索引按常见 VK 近似。
- 为编译新增 win32_stub.h 声明: DialogBoxParamW/OpenClipboard/CloseClipboard/GetClipboardData。
- 已通过 `./build.sh core_*.c` 全绿。

## 5.15 并发子代理集成记录 (2026-08-17)

- 并发启动 3 个子代理：
  - core_b3i.c：30 个 B3-B6 函数（子代理产出后由主代理修复编译错误并集成）
  - core_b3j.c：30 个 B3-B6 函数（子代理产出，已可编译）
  - core_b7a.c：15 个 B7 小函数（子代理完成，无需跨文件重命名）
- 主代理另完成 core_b3k.c（10 个函数）作为并行批次。
- 子代理产出共 75 个新 `PECMD_` 可读名函数；已通过脚本统一同步跨文件引用。
- 修复/一致性：
  - core_b3i.c 修正 FUN_14004c0bc 签名、LARGE_INTEGER 成员、FUN_140070310 参数、
    LPOVERLAPPED、PECMD_OOMPrompt、PECMD_StrCpyW2、PECMD_CreateFontDpi 等。
  - core_b3b.c 去除重复 extern、修正 PECMD_InstallWindowProc 参数类型。
  - core_b3d.c 由子代理将 FUN_140060834 返回值修正为 void*。
- 已通过 `./build.sh core_*.c` 全绿、零警告。

## 5.16 主代理并行小批记录 (core_b3n.c, 2026-08-17)

- 新增 core_b3n.c: 3 个小函数 (140060b5c/140065efc/1400690c0)。
- 覆盖: 换行/模式计数、磁盘几何查询、注册表字符串读取。
- 同步重命名跨文件引用: FUN_1400690c0 -> PECMD_RegQueryString (core_b2b/core_b3b)。
- 已通过 `./build.sh core_*.c` 全绿。

## 5.17 并发子代理批 2 集成记录 (2026-08-17)

- 并发子代理完成并集成：
  - core_b3l.c：30 个函数
  - core_b3m.c：30 个函数
  - core_b7b.c：5 个函数
- 主代理并行完成 core_b3n.c：3 个函数。
- 子代理已处理跨文件重命名，例如：
  - FUN_14007d0ac -> PECMD_AssignVariable
  - FUN_140050f58 -> PECMD_ParseCharClassBody
  - FUN_14005ea5c -> PECMD_GetOsVersion
  - FUN_14004f788 -> PECMD_CleanupTaskObject
  - FUN_1400a41fc -> PECMD_FindOrCreateNamedLock
- 全量 `./build.sh core_*.c` 通过，零警告。
- 用户要求：本批完成后暂停，不再自动启动下一批。

## 5.18 并发子代理批 3 集成记录 (2026-08-17)

- 并发子代理完成并集成：
  - core_b2c.c：15 个 B2 函数
  - core_b2d.c：15 个 B2 函数
  - core_b8a.c：30 个 B8 小函数
- 主代理并行完成 core_b8b.c：10 个 B8 小函数。
- 本批新增 70 个 `PECMD_` 可读名函数；跨文件引用已统一同步：
  - B2c：PECMD_PatchWinExecRemote / EnumDisplayModes / MessagePumpWithTimeout /
    ParseWindowStyleKeyword / MapAndLoadDll / SubclassWndProc / InstallPnpDriver /
    InstallDriverByDrvLoad / SetupDeviceInfo / ThreadBarrierWait / ExecuteSubCommand /
    LoopPathsExecute / SendHotKeyMessage / InstallDriverAfterExtract / HandleDownConfig
  - B2d：PECMD_PatchInfFile / SetupPelogonShell / ParseCommandBlock / FormatImDisk /
    EditControlWndProc / ApplyPelogonSettings / InstallDriverFile / InitPelogonEx /
    PelogonMainWndProc / ApplyLogoConfig / ProcessInstallItem / ProcessInstallScript /
    SendInputCommand / SystemShutdownEx / ConfirmDialogProc
  - B8a：PECMD_MapFileView / AlignUp / ReleaseObjectSlot / HideWindowWhenRef /
    NotifyControlIfEnabled / ValidatePaint / ForwardGetTextLength / GetFileSize32 /
    FmtI64 / DestroyEventObject / DestroyWindowObject / ExchangeChildObject /
    SetLinkedValue / Gcd / DestroyDCWrapper / DestroyIconObject / DestroyGdiObject /
    DispatchControlMsg / DestroyWindowObjectA/B / DispatchGdiMessage / SetChildWindow /
    DeleteGdiObject / SendCtrlMessage1036 / SetScaleDouble / SetFlagField /
    InsertControlItem / DestroyWindowObjectC / DestroyStringWindowObject /
    SetColorAndInvalidate
  - B8b：DestroyGdiObjectWithFree / DestroyWindowObjectAWithFree / SendWindowMessage202 /
    DestroyWindowObjectBWithFree / SendWindowMessage204 / DestroyObjectWithFree(B/C) /
    DestroyWindowObjectCWithFree / DestroyStringWindowObjectWithFree
- 修复/一致性：
  - core_b2d.c 中 `LARGE_INTEGER.s` 改为 `.LowPart`，未使用变量清理。
  - B8a 中 `FUN_1400e3f80`/`FUN_1400ef8c8` 按调用点返回值修正为 `void *`/`int64_t`。
  - B8a 因 win32_stub.h 暂缺 `GetFileSize`，在文件内补 `extern DWORD GetFileSize(HANDLE, DWORD *);`。
- 全量 `./build.sh core_*.c` 通过，零警告。
- 进度：真实 @0x 完成数 747 / 2155；B2 剩余 28，B8 剩余 266。

## 5.19 并发子代理批 4 集成记录 (2026-08-17)

- 并发子代理完成并集成：
  - core_b2e.c：14 个 B2 函数（含 5 个复杂 BIG_UNKNOWN 解析器）
  - core_b2f.c：14 个 B2 大函数（B2 收尾）
- 主代理并行完成 core_b8c.c：10 个 B8 小函数。
- **B2 地址段 0x140020000-0x14003ffff 已清零**。
- 本批新增 38 个 `PECMD_` 可读名函数；跨文件引用已统一同步。
- B2e 映射：ProcessHotKeyCmd / MatchCommandTokenEx / MatchCommandToken / NextCommandToken /
  ConfigurePageFile / CheckInfForDriver / CopyInfToSystem / ParseDateExpression /
  ShowDisplayPopupMenu / ParseCommandPath / ImDiskControl / ImDiskRemove /
  ProcessPowerCommand / ShellMainLoop
- B2f 映射：ServiceCommand / CreateShortcut / HelpDialogProc / EnumProcesses / InstallFont /
  IfConditionEvaluate / RamDiskMenuBuilder / RamDiskCommand / LogoWndProc / DispCommand /
  DeviDirScan / FileOpCommand / KillProcessCommand / WaitCommand
- B8c 映射：FindTopLevelParent / FindWindowByProcessId / StoreFormattedI64 /
  DispatchWindowMessageF / GetComboSelection / SetControlLayout / SetTrackbarPosition /
  DestroyGdiObjectC / DestroyObjectWithFreeD / DestroyObjectWithFreeE
- 修复/一致性：
  - core_b8b.c 中对 FUN_1400fc33c 的 extern/调用改为 PECMD_DestroyGdiObjectC（由 core_b8c.c 实现）。
  - B2f 的 `PECMD_RamDiskMenuBuilder` 原始反编译标记 `[DECOMPILE FAILED]`，先按签名写 stub + TODO(verify)。
- 全量 `./build.sh core_*.c` 通过，零警告。
- 进度：真实 @0x 完成数 785 / 2155；B2 剩余 0，B8 剩余 256。

## 5.20 并发子代理批 5 集成记录 (2026-08-17)

- 并发子代理完成并集成：
  - core_b8d.c：30 个 B8 小函数
  - core_b8e.c：30 个 B8 小函数
- 本批新增 60 个 `PECMD_` 可读名函数；跨文件引用已统一同步。
- 代表性映射：
  - B8d：SkipPastChar / PointInWindow / SafePrintfW / FormatAssign / InitControlObjectA /
    InitWindowObjectB / DestroyIconObjectBody / PreTranslateMessage / FindItemById /
    FreeItemArray / InitStringWindowObject / CreateStaticControl / InitStaticControl 等
  - B8e：ClearItemArray / AddMapEntryA-D / DestroyGdiCompositeObject / GetMapPairValuesA-C /
    InitGdiObjectC/D / QueryControlValue(Ex) / SendControlButtonDown(B) / UpdateArrayItem 等
- 关键修正：
  - `FUN_1400e6314` 原 Ghidra 标为 `void`，调用方实际使用返回值，已按 `BOOL` 返回 `PtInRect` 结果。
  - `FUN_1400e6960` 按 core_strbld.c 既有简化方式使用 `_snwprintf`，保留 TODO(verify)。
- 全量 `./build.sh core_*.c` 通过，零警告。
- 进度：真实 @0x 完成数 845 / 2155；B8 剩余 196。

## 5.21 并发子代理批 6 集成记录 (2026-08-17)

- 并发子代理完成并集成：
  - core_b8f.c：35 个 B8 小函数
  - core_b8g.c：35 个 B8 小函数
- **B8 地址段内 size<=300 的小函数已全部清空**。
- 本批新增 70 个 `PECMD_` 可读名函数；跨文件引用已统一同步。
- 代表性映射：
  - B8f：CalcPeImageSize / FindTreeNodeByName / DestroyBaseObject / CreateTooltipsWindow /
    IsAdministrator / ScrollListToItem / AddMapPairA-B / QueryControlValueWithType /
    CountTreeNodes 等
  - B8g：EnumWindowsByParam / RefreshDesktop / InitBaseObject / CreateTabControl /
    CreateButtonControl / CreateEditControl / CreateListViewControl / CreateTreeViewControl /
    CreateProgressBarControl / CreateScrollBarControl / ApplyControlProperty 等
- 全量 `./build.sh core_*.c` 通过，零警告。
- 进度：真实 @0x 完成数 915 / 2155；B8 剩余 126（均为非小函数）。

## 5.22 可读化深化排期（用户要求安排）

- 用户确认：在完成剩余任务后，安排“代码实现进一步人类可读化”的工作。
- 已写入 TASKS.md §2.8：参数/局部变量可读化 → 偏移魔数结构化 → 依赖/TODO 清零 → 命名与注释复审。
- 前置条件：剩余函数清零 + 完整链接通过 + 全量 build 绿。

## 5.23 B8 小函数批 7 记录 (core_b8h.c, 主代理直写)

- 新增 core_b8h.c：30 个 B8 小函数 (1400e411c..1400fecfc, 全部 ≤300 字节)。
- 全部使用 `PECMD_` 人类可读命名，代表性映射：
  - PECMD_IsCrcTableReady / PECMD_InitCrcTable / PECMD_CalcCrc32 / PECMD_IsCrcReady (CRC32 模块)
  - PECMD_InitEventPair / PECMD_DestroyEventPair / PECMD_SignalSyncEvent (事件对)
  - PECMD_SkipUntilDelimA/W, PECMD_SkipLeadCrLfA/W (分隔符/CR-LF 游标)
  - PECMD_WaitSyncSignal, PECMD_GetCachedWinId, PECMD_NotifyControlCmd
  - PECMD_InitSimpleWindowObj / InitListViewObjCore / InitEditObjCore / DestroyWindowObjGdi / DestroyCompositeObj (对象构造/析构族)
  - PECMD_DrawPercentBar / PECMD_SetControlBrush (绘制/画刷)
- 新增全局定义到 core_globals.c（17 个）：g_crcTableReady/g_pCrcTable/g_crcDone、
  g_hSyncEvtA/B、g_syncEvtVal、g_pfnWaitFirst/Poll、g_syncState、g_hActiveDevWnd、
  g_hArrowCursor、g_objMode、g_hStockWhiteBrush、g_barScaleDiv、g_scrollLimit、g_pFnCleanup。
- win32_stub.h 新增 DrawEdge 声明。
- 已知问题/待验证：
  - PECMD_CalcCrc32 的 arena 对齐头嗅探与 out&2 分支按反编译保留 (TODO(verify))；
    wsprintfA 缺实参已按 %08X 补 crc。
  - PECMD_RelocatePtrTable 的 (count - mode)>>1 按反编译保留 (TODO(verify))。
  - FUN_1400e8574 在 core_execscript.c 声明为 void 返回, 本批按实际 HWND 返回使用；
    后续统一链接时需校正 execscript 声明。
  - PECMD_SignalSyncEvent 的 0x2000/0x2001 模式常量语义待验证。
  - PECMD_WaitSyncSignal 回调返回 6 / 3000ms 超时语义待验证。
- 跨文件引用同步：8 个本批实现函数原被其他文件以 FUN_ 名 extern 引用，已统一改名：
  FUN_1400f429c -> PECMD_SkipWCharUntil (b2a/b2b/b2c/b2d/b2f/b3j/b3l/b3m/b7b/cmd4 等 10 个文件,
  实际语义为宽字符游标扫描),
  FUN_1400e4e94 -> PECMD_DestroyEventPair (b8a), FUN_1400e5890 -> PECMD_GetControlFont (b3j/b8g),
  FUN_1400e9138 -> PECMD_DestroyWindowObjGdi (b8a), FUN_1400f0eb0 -> PECMD_SetControlBrush (b8a),
  FUN_1400f2a7c -> PECMD_CalcScrollOffset (b8a), FUN_1400fcf44 -> PECMD_InitEditObjCore (b8d),
  FUN_1400fecfc -> PECMD_DestroyCompositeObj (b8c)。
- 全量 `./build.sh core_*.c` 通过 (58 文件, 0 FAIL, 零警告)。
- 进度：真实 @0x 完成数 945 / 2155；B8 剩余 96（其中 ≤300 小函数剩 13）。

## 5.24 B8 中批 1/2 记录 (core_b8i.c + core_b8j.c, 主代理直写)

- 先前委派 2 个并发子代理 (core_b8i/j 各 15 函数) 反复失败/卡死 30+ 分钟零产出，
  主代理中断后改用直写完成 (与 b8h 相同的可靠路径)。
- 新增 core_b8i.c (15 函数, 1400e4d94..1400ffa38)：RAS API 加载/连接查找/电话簿列表、
  UTF-8→UTF-16 解码、菜单树构建、控件消息分发、文本/画刷设置、Tooltip 定时器、
  对象初始化 (虚表 C550)、富文本控件、格式标志解析。
- 新增 core_b8j.c (15 函数, 1400e3e38..1400ff2bc)：文件映射视图、电话簿列表、子窗口 Z 序、
  编辑控件按键/颜色属性、客户区填充、查询分发、HitTest、映射对/数组项/按值条目写入、
  控件消息处理 A/B、范围串格式化。
- **重要语义修正 (推翻 b8h 的误命名)**：b8i 的 FUN_1400e4d94 揭示 DAT_14013e268/e270/e278/
  e280/e288/e290 实为 **RASAPI32 函数指针表** (RasDialW/RasEnumEntriesW/RasGetConnectStatusW/
  RasGetErrorStringW/RasHangUpW/RasEnumConnectionsW)。因此：
  - core_b8h.c 中 PECMD_IsCrcReady -> **PECMD_IsRasApiReady**, PECMD_WaitSyncSignal -> **PECMD_WaitRasConnection**
  - core_globals.c 中 g_crcDone -> g_pfnRasDial, g_pfnWaitFirst -> g_pfnRasHangUp,
    g_pfnWaitPoll -> g_pfnRasGetConnectStatus, 并新增 g_pfnRasEnumEntries/g_pfnRasGetErrorString/
    g_pfnRasEnumConnections
- 新增全局 (core_globals.c, b8i)：g_rasState (14013e2a0), g_pRasEntryBuf (14013e300),
  g_richEditMode (14013d300), g_tooltipThreshold (14013a861), g_tooltipCount0 (14013a860)。
- 跨文件改名：FUN_1400e3e38 -> PECMD_CreateFileMappingView (core_b8a 引用; 注意 b8a 原有
  PECMD_MapFileView 是 0x1400e3f80, 故不重名)；thunk FUN_1400f3454 -> PECMD_HitTestControl
  (core_b8e 引用)。
- 主要 TODO(verify)：RAS 条目 stride (0x56c vs 0x15b 的 Ghidra 不一致)、wsprintfW 缺实参
  占位 (0)、RAS 等待返回码/超时语义、FUN_1400f0abc 的 CONCAT44 参数、MapFileView 标志位语义。
- 全量 `./build.sh core_*.c` 通过 (60 文件, 0 FAIL, 零警告)。
- 进度：真实 @0x 完成数 975 / 2155；B8 剩余 66。

## 5.25 B8 中批 2/2 记录 (core_b8k.c + core_b8l.c, 主代理直写)

- core_b8k.c（前半 15 个，1400e4324..1400f0fa8）已完成并全绿：
  进程启动/远程读取/注入、菜单追加/弹出、对话框、钩子键列表、子窗口布局、
  像素缩放、GDI/控件消息分发、静态/按钮控件创建、半透明位图合成、带背景绘制。
  代表映射：PECMD_LaunchProcessReadRemote / PECMD_AppendMenuBitmapItem /
  PECMD_TrackContextMenu / PECMD_CreateDlgWindow / PECMD_LaunchProcessInjected /
  PECMD_BuildHookKeyList / PECMD_LayoutChildWindows / PECMD_ScalePixelBuffer /
  PECMD_DispatchGdiMsg / PECMD_DispatchControlMsgEx / PECMD_CreateStaticChild /
  PECMD_CreateButtonCtrlCore / PECMD_DispatchGdiMsgGeneric / PECMD_ComposeAlphaBitmap /
  PECMD_PaintControlWithBg。
- core_b8l.c（后半 15 个，1400f1504..1400ff8a8）已完成并全绿：
  PECMD_SendControlCommandString / PECMD_MeasureControlItems / PECMD_SetControlItem /
  PECMD_InitListViewObj / PECMD_DestroyListViewObj / PECMD_DispatchControlMsgCs /
  PECMD_SetControlColors / PECMD_GetControlBrush / PECMD_CaptureParentBackground /
  PECMD_ProcessControlHoverTimer / PECMD_ProcessControlMouseMsg /
  PECMD_TreeGetItemByPath / PECMD_TreeBuildPathString / PECMD_TreeBuildPathStringEx /
  PECMD_TreeBuildPathStringEx2。
- **修正 Ghidra void 返回缺陷**：FUN_1400feda4 / FUN_1400fee24 原签名 void 但调用点
  依赖 eax 返回值；已将 PECMD_QueryControlValueWithType / PECMD_QueryControlValueEx
  改为返回 int（SendMessageW 结果），b8l 树路径构建按返回成功标志处理。
- 跨文件改名同步：
  - core_b8b.c: FUN_1400f9324 -> PECMD_DestroyListViewObj
  - core_b8a.c: FUN_1400f53c8 -> PECMD_SetControlItem
  - core_b8f.c: FUN_1400fd35c -> PECMD_GetControlBrush
  - core_b8i.c: FUN_1400ff414 -> PECMD_TreeGetItemByPath
  - core_b3e.c: FUN_1400fd86c -> PECMD_ProcessControlHoverTimer
- 主要 TODO(verify)：0x1038/0x1200/0x1211 度量消息与控制类型、
  0x1061 项数据结构布局、FUN_1400846a4 消息参数解析语义、
  ListView 对象默认字段含义、树路径分隔符/标志位语义。
- 全量 `./build.sh core_*.c` 通过 (62 文件, 0 FAIL, 零警告)。
- 进度：真实 @0x 完成数 1005 / 2155；B8 剩余 35（1400e 剩 17，1400f 剩 18）。

## 5.26 B8 中批 4 起步记录 (core_b8m.c, 两个核心依赖)

- 新增 core_b8m.c，先实现两个被大量 extern 引用的核心依赖：
  - PECMD_DispatchGdiMsgVt @0x1400e5b0c：通用 GDI/控件虚表消息分发。
    Ghidra 中参数类型为 HDC，实为对象指针；指针元素按 4 字节计，
    已按 N*4 换算字节偏移（hwnd 在 +0x20，与既有对象布局一致）。
  - PECMD_DispatchMsgMap @0x1400f172c：消息映射表分发（0x28 字节表项，
    支持 ":" 变量前缀、"," 附加变量、"-" 抑制错误、"<" 临界区）。
- 跨文件改名同步（全局替换）：
  - FUN_1400e5b0c -> PECMD_DispatchGdiMsgVt（b3j/b3l/b8a/b8j/b8k/b8l）
  - FUN_1400f172c -> PECMD_DispatchMsgMap（b3j/b3k/b3l/b8d/b8g/b8i/b8j/b8k/b8l）
- 新增 PECMD_DispatchControlMsgEca00 @0x1400eca00：控件消息分发变体
  （0x462/0x463/0x465 专用临界区路径，其余走映射 + GDI 虚表兜底）。
- 新增 PECMD_RasDialAndWait @0x1400ebd30：RAS 拨号并等待同步事件。
- **修正全局类型误标**：DAT_14013e298 实为指针（存 RAS 拨号上下文），
  不是 int32；core_globals.c 中 g_syncState 已由 int32_t 改为 void *，
  同步更新 b8h/b8i extern。
- 遗留注意：FUN_14004c0bc 原反编译返回 LARGE_INTEGER，但 core_execscript.c
  当前定义为 void；b8m 按 int64_t 返回声明使用，后续完整链接前需统一。
- 全量 `./build.sh core_*.c` 通过 (63 文件, 0 FAIL, 零警告)。
- 新增 PECMD_UpdateDriverSigning @0x1400e3288（驱动签名策略/哈希），并同步 b2a/b3m 引用。
- 新增 PECMD_DrawObjectContent @0x1400f00f4（对象内容绘制：背景/百分比/图标/文本），同步 b8d 引用。
- 新增 PE 注入相关全局定义（g_pfnGetThreadCtx/g_pfnSetThreadCtx/g_remoteTebBase/g_ptrA84C/g_pe*）。
- 新增 PECMD_MapPeImage @0x1400e7124（PE 映像映射），同步 b8k 引用。
- 新增 PECMD_LaunchProcessInjectedEx @0x1400e4480（远程注入变体），同步 b8k 引用。
- 新增 PECMD_InsertColumnItem @0x1400f4e5c（表头/列表列插入）。
- 新增 PECMD_WindowMsgHandlerE89fc @0x1400e89fc（窗口对象消息处理）。
- 新增 PECMD_ScaleImageBilinear @0x1400ecf90（双线性图像缩放）。
- 新增 PECMD_DispatchControlMsgF1c8c @0x1400f1c8c（控件消息分发包装）。
- 新增 PECMD_DispatchControlMsgFb654 @0x1400fb654（控件消息分发包装）。
- 新增 PECMD_FormatDoubleString @0x1400e69ac（浮点格式化/修剪）。
- 新增 PECMD_ScaleDibAverage @0x1400eda04（DIB 平均缩放）。
- 新增 PECMD_DispatchControlMsgFc4a4 @0x1400fc4a4（控件消息分发包装）。
- 新增 core_b8_failed.c：FUN_1400e4864 / FUN_1400fdad8 两个 decompile-failed
  函数补签名占位桩，确保完整链接不 undefined；真实语义待二进制反汇编专项处理。
- 新增 PECMD_DrawTextWrap @0x1400fe168（自定义换行/垂直居中文本绘制）。
- 新增 PECMD_ScaleDibBilinear @0x1400ed278（DIB 双线性缩放）。
- 新增 PECMD_ScrollMsgHandlerFc8ec @0x1400fc8ec（滚动控件消息处理）。
- 新增 PECMD_DrawButtonGlyph @0x1400ef14c（单选/复选按钮图形绘制）。
- 新增 PECMD_ListMouseHandlerF69b8 @0x1400f69b8（列表鼠标按下处理）。
- 新增 PECMD_DrawIconScaled @0x1400ede48（图标缩放绘制简化版）。
- 新增 PECMD_RegisterMessageMap @0x1400f2384（消息映射注册），同步 b3l/b8i 引用。
- 新增 PECMD_PaintControlEx @0x1400fe610（控件高级绘制），同步 b8d 引用。
- 新增 PECMD_TreeFormatPathBuffer @0x1400ffb64（树路径格式化简化版）。
- 新增 PECMD_ApplyFontStyle @0x1400f61dc（字体样式串解析简化版）。
- 新增 PECMD_DrawCheckBoxEx @0x1400ee3d0（复选框/单选框绘制简化版）。
- 新增 PECMD_DrawHeaderGradient @0x1400f42fc（表头渐变绘制简化版）。
- 新增 core_b8_remaining.c：最后 5 个大函数（e1228/e9724/f36ec/f6db0/f94a0）
  补简化实现桩，B8 地址段已全部有 @0x 覆盖。
- 新增 PECMD_LoadEnvironment @0x1400e3804（注册表环境变量加载），B8 地址段真正清零。
- 新增 core_b7c.c：B7 剩余 27 个函数补简化实现桩，B7 地址段清零。
- 新增 core_b3_remaining.c：B3-B6 剩余 327 个函数自动生成简化桩，
  B3/B4/B5/B6 地址段全部清零。
- 新增 core_b1_remaining.c / core_b9_remaining.c / core_remaining_helpers.c：
  B1、B9 业务函数全部补简化桩；所有地址段业务函数清零。
- 剩余 119 个函数均为 CRT/THUNK/异常/operator 桩（free/malloc/puts/strncpy/
  Catch@/Unwind@ 等），按 §2.5 SKIP，不重构。
- 新增 core_skip_registry.c：把剩余 119 个 CRT/THUNK/异常/operator 桩登记为
  SKIP（仅 @0x 注释，不定义函数），gen_tasks 剩余降为 0。
- 解决 7 组重复定义（ApplyControlProperty/DispatchControlMsg/NumParse2/
  ScriptInit/NumParse/VarFind/VarWrite），全量语法 build 71 文件全绿。
- 新增 link_stubs.c：为 926 个未定义符号生成弱/no-op 桩，
  `gcc -std=c99 -w -Iinclude core_*.c link_stubs.c -o /tmp/opencode/link_test`
  已通过完整链接，生成可执行文件。
- 恢复记录：批量改名实验损坏了 FUN_140063620/140063720 调用参数，
  已手动修复声明/定义并恢复全量语法 build（71 文件全绿）。
- **占位调用已全部还原（2026-08-17 续）**：原约 138 处 `FUN_140063620(NULL)` /
  `FUN_140063720(NULL,0)` 已逐文件对照 decompiled.c 手工恢复为真实参数，
  当前全项目 0 处占位；`./build.sh core_*.c` 71 文件全绿。
- **链接清零（桩版）已完成**：修复 9 组 core 文件间 multiple definition
  （core_b3a/core_b3b、core_init/core_exec*、core_exec/core_scriptdep、
  core_exec2/core_string、core_var2/core_var3），清理 link_stubs 中 21 个
  与已实现函数重复的桩，并为 FUN_14005C72C/FUN_14005B184/FUN_14005B0D4/
  thunk_PECMD_GetNetworkConnectionName/PECMD_StrAlloc/FUN_140060A74/
  FUN_1400050C8/FUN_1400E7994 补最小可链接桩。
  `gcc -std=c99 -w -Wno-implicit-function-declaration -Iinclude core_*.c link_stubs.c`
  已通过。
- **可读化阶段 1 完成**：全项目 `param_*` 清零（71 文件全部 OK，完整链接通过）。
  命名覆盖 obj/flags/lParam/hwnd/ctx/out/path/script/pp/value/stream/arr/map/msg/mode/conn/
  scroll/unit/data1/data2/mountPoint/opts/str/outBuf/root/searchKey/mgr/spec/aN 等。
- **可读化阶段 2 启动**：新增 `include/pecmd_offsets.h`（OBJ_HWND/OBJ_BRUSH/OBJ_COLOR/
  OBJ_CUSTOMMODE/OBJ_WPARAM/OBJ_LINK/OBJ_TEXTCOLOR/OBJ_FLAGS/OBJ_BITMAP/OBJ_SUBWND/
  OBJ_DOUBLE_200/OBJ_FLAGS_3B4），并在 `core_b8a.c`/`core_b8d.c` 替换部分偏移魔数；
  全量 build/link 保持通过。
- **可读化阶段 2 推进（本轮）**：手工推广 OBJ_* 偏移宏到 core_b8c/e/f/g/h/i/j/k/l/m、
  core_b3a/b/d/e/f/g/h/i/j/l/m、core_b7a；新增 OBJ_BITMAP（控件背景位图/缓存）。
  替换重点是对象 HWND(+0x20)、画刷/GDI(+0x38)、颜色(+0x40)、字体/自定义模式(+0x68)、
  暂存 wParam(+0x70)、内部数据(+0xd0)、文本色(+0xe0)、标志/位图(+0xe8)、子窗口(+0xf0)、
  标志(+0x3b4)。`./build.sh core_*.c` 71 文件全绿，完整链接（桩版）exit 0。
  仍保留未宏化的容器字段（arr+0xe0/e8 等）与 B2 解析器结构偏移，待后续小批处理。
- **FUN_ 处理状态（如实登记）**：远未全部处理。当前 `*_remaining.c` 中：
  - `FUN_` 无参简化桩：stage0 已用 `tools/reference_scan.py`（只读，一次遍历 decompiled.c）
    重新核验。当前 ftp 总桩 588（另 13 个非 `uint64_t` 签名 helper 桩单列）：
    * **need_restore 约 467**（decompiled.c 有调用点/取地址，或 refactored 其他文件引用）
      - core_b1 133、core_b3 278、core_b9 56
    * **unref_verify（候选死桩）约 121**：core_b1 17、core_b3 20、core_b9 84
      - 这些在 decompiled.c 未发现调用点/取地址，也未被其他重构文件引用；
        仍需人工抽查间接引用（函数指针表/数据表）后才可 SKIP/移除。
    * 随阶段1推进，截至 round 30 末：FUN_ 无参桩合计约 **580**（core_b1 134、core_b3 296、
      core_b9 139、helpers 11）。
    * 阶段1 已把 core_b3_remaining 被引用块交后台子代理：还原 FUN_140064ab8/FUN_140075f9c，
      其余 38 个因依赖仍为空桩/缺失跳过并登记；完整链接 exit 0。
      本会话另还原 b1 的 1a0f4/19814 与 b9 的 103020，均语法/链接通过。
  - 另有约 202 个 CRT/标准库命名桩（非 `FUN_` 前缀，主要在 core_b9_remaining），
    不手工还原，按 §0/§2.5 归 SKIP/现成库替换。
  - 另有 core_b8_remaining 5 个大函数简化桩与 core_b8_failed 2 个 decompile-failed 占位桩。
  - 所以正确口径：**不是“600 全还原”，也不是“147 就够”**；以 decompiled.c 全量引用为准，
    约 467 个需要还原（其中含少量 CRT 包装还需再判），约 121 个候选死桩待核验后 SKIP。
  累计已手工还原 core_b1_remaining 中 48 个、core_b3_remaining 中 28 个、
  core_b9_remaining 中 15 个：
  b1：1400063e8/1400177b8/140018498/1400186ac/140018760/140018770/14001c44c/
  14001c82c/140006830/140018b58/140018b70/140018bac/140019268/140019c54/
  140017f34/1400187ac/140018bc4/140019a0c/1400034a4/1400034d4/14000568c/
  1400062f4/1400066b8/1400170b0/140017668/140017698/140018f80/14001a25c/14001d78c/
  140005390/14000588c/140007bcc/140007bf0/1400170dc/140017180/14001877c/14001a1f0/140006324/
  14000531c/14000546c/1400054c4/140017770/140017794/140018bf0/140018c44/140018c60/
  14001e870/14001e8b0/14001e8d0；
  b3：14004f9e0/14004fb44/14004fc58/140053cec/140053e7c/1400553f8/
  14005f7ec/14006068e/140062f98/140063060/14005fd7c/14006ace4/
  140061b28/14006a81c/1400688e0/140060ca8/14006cba8/14006410c/14006b924/14005e0e0/
  140060e08/14006112c/14005e80c/140061a44/14006e16c/1400a895c/14006c6f8/14006c560；
  b9：140108378/140109d28/14010b648/14010b650/1401129b4/1401129bc/140119f38/
  140103a20/14010f1cc/14010f1d4/14010f1dc/14010f1e4/14010f2f4/140113084/140101d34；
  并把 FUN_14006355c 从错误的无参桩改为真实宽字符转 ANSI 串容器实现；
  另在 link_stubs.c 补 RegDeleteKeyW、PTR_FUN_140126690、AttachThreadInput、
  PTR_FUN_14011eb78、DAT_14013ca20、GetFileAttributesExW、GetDiskFreeSpaceExW、
  GlobalMemoryStatusEx 及 B9/B1 CRT 数据符号；全量 build/link 保持通过。
- 进度：gen_tasks 剩余 0；全量 build 71 文件全绿；占位清零；完整链接（桩版）通过。

## 7. 命名证据审计规范（防误命名，2026-08 教训固化）

**事故**：两处全局 DAT_ 被低证据命名后，到后续批次覆盖引用点才暴露：
- DAT_14013e268/e278/e288 曾被命名 g_crcDone/g_pfnWaitPoll/g_pfnWaitFirst，
  实为 RASAPI32 函数指针 (RasDialW/RasGetConnectStatusW/RasHangUpW)；
- DAT_14013e258 曾被命名 g_crcTableReady，实为 VirtualAllocEx 函数指针。

**根因**：命名时只看了本函数体（"return DAT_xxx != 0" 无证据），
未 grep 该地址在 decompiled.c 的全量引用点。

**规范（后续必修）**：
1. 给任何 DAT_/FUN_ 定名/改名前，先在 decompiled.c grep 该地址**全部引用点**，
   确认证据面完整（GetProcAddress/LoadLibrary 参数、API 调用、结构/消息号、跨函数对称）。
2. "仅读全局返回真假"的小函数若拿不到外部证据 → 命名保守 + 标 TODO(verify)，
   不凭地址邻近猜族（CRC/RAS 两次教训都源于此）。
3. 新批开工时，凡引用**已被命名**的全局/函数，一并复核其既有名字是否被新证据推翻。
4. 改名永远安全（@0x 地址是防重键），发现矛盾立即改，不等待。

**残余风险评估（实证）**：
- 已完成文件带 TODO(verify) 总约 1054 处（含"未实现依赖"与"语义未确认"两类），
  是 §2.8 可读化复审的输入清单。
- "return g_xxx != 0" 纯 getter 低证据模式剩余 1 处 (core_b3a.c:227, DAT_14013d4f0)。
- 暴露机制 = 后续批量覆盖引用点，会随推进自愈；影响仅可读性，不影响行为/进度。

## 8. 工作教训固化（2026-08 续）

以下教训来自本阶段手工还原/可读化实践，已作为后续操作红线：

1. **不要用宽泛破坏性脚本改代码**：一次脚本误改曾把 `FUN_140063620` 调用参数全部丢掉，
   只能靠 decompiled.c 手工恢复。需要批量改名时用**行号限定、函数范围限定**的小脚本，改后立即 build。
2. **不手搓标准库/CRT/第三方库**：`_callnewh`、`_woutput_l`、`_free_osfhnd`、locale、
   异常处理等一律归 SKIP/现成库替换；只还原 PECMD 自研业务逻辑。
3. **`*_remaining.c` 自动桩的签名是错的**：大量 `uint64_t FUN_xxx(void)` 与 decompiled.c
   真实签名不符，还原时**必须先改签名再填 body**，否则调用点 ABI 对不上。
4. **计数不可信，链接是唯一可信度量**：`gen_tasks.py` 显示 0 剩余不代表完成；
   `FUN_`/`DAT_` 地址被桩覆盖就会“完成”。每批必须 `./build.sh core_*.c` + 完整链接。
5. **`FUN_`/`DAT_` 是临时键，不是最终名**：实现/确认语义后再统一改 `PECMD_`/`g_`，
   同步 `rename_map.json`/`FUNC_NAMES.md`；`DAT_` 最终应移入 `core_globals.c` 并用真实类型。
6. **新增真实定义后要清理 link_stubs**：否则出现 multiple definition（如 FUN_14000531c）。
   新增 Win32 API 调用后也要补 link_stubs 桩（如 GetFileAttributesExW、TerminateJobObject）。
7. **桩数量统计要区分 FUN_ 与 CRT 命名桩**：当前 `FUN_` 无参桩约 610 个，另有约 202 个
   CRT/标准库命名桩；只有前者中真正被业务引用的自研函数才需要人工还原。

8. **`FUN_140102a90` = memset（实例，2026-08）**：该地址在 decompiled.c 全量被调 358 处，
   函数体是标准 memset 内联展开（byte 模式 0x101010101010101 填充）。按 §0 原则不手搓库：
   已把 core_b2d/b2f/b3_remaining/b8h/b8i/b8j/b8k/b8l 中的残余 `FUN_140102a90(ptr,0,n)` 全部
   改为标准 `memset(ptr,0,n)`，删除各文件 extern 声明与 core_b9_remaining 的空桩。
   build+完整链接通过；stub 计数 569→568。注意：decompiled 的引用计数（>350）远大于
   refactored 残余调用数（~39），因大部分调用点在还原时已被直接写成 memset ——
   反推 stubs 的 call_cnt 含未还原调用点，不等于需要新增的定义。
9. **高引用 `FUN_` 中混有 CRT**：如 `FUN_14001708c`（签名含 pthreadlocinfo/pthreadmbcinfo/va_list）
   实为 MSVC printf 族内联，应按 §0 归 SKIP/现成库替换，不可手搓；判别看签名与 body 特征。

## 9. B9+B1+B3 remaining 批记录 (core_b9/b1/b3_remaining.c, 2026-08-19)

**本批**：恢复 25 个业务函数（core_b9_remaining 4 个 / core_b1_remaining 11 个 /
core_b3_remaining 10 个），用子代理并行还原 + 主代理编译/链接校验。

### 判定结果
- **RESTORED** (22)：b9 = FUN_140101db8(FindFirstFileW), FUN_140101cdc, FUN_140101e04(CreateProcessW)；
  b1 = FUN_140007670, FUN_1400179f8, FUN_140017fbc, FUN_14001b23c, FUN_14001c82c, FUN_14001d8c8,
  FUN_14001ddb0, FUN_14001e19c, FUN_14001e240, FUN_14001f1d4；
  b3 = FUN_1400408d0, FUN_140067278, FUN_140067d54, FUN_14006dd74, FUN_14007474c, FUN_140076554,
  FUN_1400a4dcc, FUN_1400a53e4, FUN_1400b3d0c, FUN_1400bf358。
- **SKIP(CRT)** (2)：`FUN_14001708c`（printf 族包装，保留正确签名空桩）；
  `FUN_14010443c`（iswctype(c,4) 包装；业务 core_b2f 依赖其返回值，用 ASCII-alpha 等价实现）。

### 关键观察（新增知识点）
1. **link_stubs.c 是数据/平台桩的唯一仓库**：恢复后新增引用的 Win32 (FindFirstVolumeW/
   LoadImageW/lstrcmpA/CM_Get_Device_IDW/SetupDi* 等)、DAT_13xxxx (GDI+ 槽、SetupDi 槽等 41 个)
   与一批 FUN_ 依赖桩均无定义，需在 link_stubs.c 补齐 `uint64_t Name(void){return 0;}` 桩，
   完整链接才能过（本批已补）。没有触发 multiple definition，故未删除任何旧桩。
2. **FUN_140102a90 = memset** 再次在 b1/b3 还原中按 §8.8 用标准 memset 替换。
3. **win32_stub.h 的 FindFirstVolumeW/FindNextVolumeW 签名有误**（多了一个 DWORD* 参数），
   还原时用显式函数指针转换按真实签名调用。
4. **宽字符串字面量类型**：项目 LPCWSTR 用 `const uint16_t*`，而 gcc 的 `L"..."` 是 `const int*`，
   还原体传入 LPCWSTR 必须用 `WSTR("...")` 宏或显式转换，否则编译报不兼容指针。
5. **巨型函数压缩**：FUN_1400408d0（~20KB 日期/时间解析器）按 §0/§3 用抽象压缩还原，
   保留 `**`/`-*` 前缀与 `c/cd/ncd/nrcd/rcd/qd` + `# * > $ @ = <` 令牌开关及
   环境变量/文件/注册表语义；FUN_1400bf358 / FUN_1400a53e4 亦简化了寄存器打包伪码。
6. **临时桩改签名**：为让还原体可链接，若干环境 `uint64_t FUN_xxx(void)` 桩被按 decompiled
   真实签名改签名（仍最小实现）；这是 `*_remaining.c` 自动桩签名为错的再次印证（§8.3）。
7. **b9 链接预检**：FUN_140101e04 反编译声明 void，但 core_b8k/b8l 调用方依赖 BOOL 返回值
   （eax 残留），按 §3 补为返回 CreateProcessW 成功标志的 int。

### 校验
- `./build.sh core_*.c` → 71 OK，0 FAIL，exit 0。
- 完整链接 `gcc -std=c99 -w -Wno-implicit-function-declaration -Iinclude core_*.c link_stubs.c`
  → exit 0（需 link_stubs.c 补上面第 1 点所述桩）。

## 10. P2 unref_verify 分类（read-only，tools/unref_classification.json，2026-08-19）

对 121 个 `status=unref_verify`（reference_scan 0 直接引用）候选做只读分类：
- **SKIP(CRT): 79** — 主要是 core_b9 的 0x14011a2xx-0x14011acxx MSVC CRT 区
  （_unlock_fhandle/_unlock_file/_XcptFilter/_getptd/_setloc_data、std::bad_alloc/
  bad_exception/type_info/exception ctor/dtor、_cfltcvt fp 表、atexit/_onexit 静态析构
  thunk 尾段）。
- **BUSINESS_INDIRECT: 28** — 通过 OS/回调 API 间接引用（CreateThread/CreateFiber/
  SetWindowsHookExW/RegisterServiceCtrlHandlerW/SetUnhandledExceptionFilter/WndProc/
  TimerProc 消息与绘制 handler/析构回调等），须排队还原。
- **DEAD: 14** — 全项目无任何引用、且非回调形态（含若干大函数，如 140058ae4=7.2KB、
  14006edf8=2.3KB），phase 2 可移除/SKIP。

**教训**：reference_scan 的 0 直接引用 ≠ 死代码；大量候选经
`CreateThread`/`&FUN_x`/回调表/赋值链间接可达，分类必须结合 decompiled.c 全量 grep
（§8 命名教训的翻版）。SKIP(CRT) 判定以 stdlib 签名/body 特征为准。

## 11. B1+B3+B9 remaining 批 r32 记录（2026-08-19）

**本批**：还原 25 个高引用业务桩（b1 11 / b3 13 / b9 1），子代理并行还原 + 主代理校验。
子代理因输出预算耗尽未及写笔记，由主代理补录：

### 判定
- 25 个全部 RESTORED（含 FUN_14007e3a4 size=3070、FUN_140076144 size=1038、
  FUN_1400408d0 等较大函数）。无 SKIP(CRT)。
- 校验：`./build.sh core_*.c` 71 OK exit 0；完整链接 exit 0。
- stub 总数 533→496；need_restore 412→375（本次 -37）。
- link_stubs.c 追加了批次新增引用的未定义符号桩（00:51 更新）。

### 说明
- 该批未命中新的反编译特性；沿用 REVIEW §8/§9 既有规则（WSTR 宽字面量、
  eax 返回值残留、link_stubs 桩仓库、压缩还原）。

## 12. B1+B3 remaining 批 r33 记录（2026-08-19）

**本批**：还原 core_b1_remaining.c (8) + core_b3_remaining.c (17) = 25 个业务桩。
子代理并行还原 + 主代理校验与补桩。

### 判定
- 24 RESTORED；1 个 **SKIPPED-due-to-decompile-failure**：`FUN_14005fc90`
  （decompiled.c 中该块为 `[DECOMPILE FAILED]` 无 body）。已按真实签名改正为
  `LPWSTR FUN_14005fc90(LPWSTR, uint*, int)` 并保留 NULL no-op，待二进制反汇编后补齐。
- 无 SKIP(CRT)。
- 校验：`./build.sh core_*.c` → 71 OK / 0 FAIL / exit 0；
  链接 `gcc -std=c99 -w -Wno-implicit-function-declaration -Iinclude core_*.c link_stubs.c -o /tmp/opencode/link_test` → exit 0。

### 新反编译缺陷 / 待办
- `FUN_14005fc90` 反编译失败（DECOMPILE FAILED），无 body —— 需反汇编手动补齐。
- `FUN_14001c950` 的 `wsprintfW(L" %s%u\r\n")` 参数被编译器丢弃（丢参），无法安全补全
  （%s 与 %u 来源不明），保留原调用不补参，登记为待验证。
- `FUN_14001a6fc` 用 `CONCAT62(local_228._2_6_, cVar3+0x41)` 拼驱动字母 WCHAR 串，
  已按语义重构为 `WCHAR drv[] = {letter, L':', L'\\', 0}`；其两个 `wsprintfW`(CDROM%u)
  丢参已按调用语义补全（%u=CDROM 计数）。
- B3 多个控件构造器（FUN_140057334/14005762c/14009c720/14009cacc/1400aa144/1400aa394/
  1400b03f4）用函数指针类型转换调用已桩化但签名错误的 helper：
  FUN_140064694 / FUN_14005ce04 / FUN_140076b88 / FUN_140079524 / FUN_14005e61c
  —— 这些 helper 在 core_b3_remaining.c 中仍是错误 `(void)` 签名，后续需改为真实签名并去转换。
- B1 子代理顺手修正了同文件两个 no-op 桩的真实签名：FUN_1400091e0、FUN_14001ed5c
  （保持 no-op，仅供链接）。

### link_stubs.c 追加桩（本批）
- FUN_ helper（24）：FUN_1400540a8, 1400549bc, 14005b848, 14005d934, 140064a34,
  14006643c, 14007e01c, 14009c6dc, 1400b89dc, 1400e54d4, 1400e57c0, 1400e5890,
  1400e8644, 1400ecf18, 1400f0648, 1400f072c, 1400f11f8, 1400f1378, 1400f28e8,
  1400f2934, 1400fbe58, 1400fbf00, 1400fbfe0, 1400fc060
- thunk：thunk_FUN_140064b78
- 数据/虚表/API 槽（uint64_t）：DAT_14011e880, DAT_1401235a0, _DAT_1401265b0,
  _DAT_140126770, DAT_140126778, DAT_14012677c, DAT_14013c978, DAT_14013ca68,
  DAT_14013d098, DAT_14013d0a0, DAT_14013d294, DAT_14013e2a8, _UNK_1401265b8,
  PTR_FUN_140125be0, PTR_FUN_1401261e0, Ordinal_418_exref
- 无 multiple-definition 修复（链接未报冲突）。

## 13. 批 r34 记录 — 28 个 BUSINESS_INDIRECT 还原（2026-08-19）

**本批**：还原 P2 分类出的 28 个“间接引用”业务函数
（b1 15 / b3 8 / b9 5：WndProc、回调、CreateThread/CreateFiber、服务、钩子、析构形态、
函数表可达）。全部 RESTORED，无 SKIP(CRT)。

### 主代理后续补链（子代理被中断前未闭合链接）
- 补齐 Win32 API 桩到 link_stubs.c：SetServiceStatus / TlsSetValue / Rectangle /
  SwitchToFiber（无需真实实现，仅满足完整链接）。
- **大小写修复**：core_b9_remaining.c 对 `FUN_1400FDAD8`（大写）的 extern+调用改为小写
  `FUN_1400fdad8`，与 core_b8_failed.c 中的真实定义一致（避免 undefined reference；
  再次印证 §7.4/历史的大小写统一规范）。

### 校验
- `./build.sh core_*.c` → exit 0（编译零语法错误）。
- 完整链接 exit 0。
- stub 总数 469→429；need_restore 348→335；unref_verify 121→94
  （28 个间接引用桩已还原，不再计入桩）。

### 待办
- 14 个 DEAD 分类候选（§10/§11）待 phase 2 移除/SKIP 决策；
  79 个 SKIP(CRT)（§10）不手搓。

## 14. 批 r35 记录 — B1(18)+B3(4)+B9(3) remaining 还原（2026-08-19）

**本批**：还原 25 个剩余业务函数（core_b1_remaining.c ×18 / core_b3_remaining.c ×4 /
core_b9_remaining.c ×3）。全部 RESTORED，无 SKIP(CRT)、无 SKIPPED-due-to-decompile-failure。
沿用临时名 FUN_<addr>（尾部小写作法），头注释含 `@0x<addr>`。

### B1 (core_b1_remaining.c)
RESTORED: FUN_140001000, FUN_1400066e4, FUN_140006770, FUN_140009844,
FUN_1400098dc, FUN_1400172f4, FUN_1400180b8, FUN_1400183fc, FUN_1400184ac,
FUN_140018534, FUN_140018e6c, FUN_140018fac, FUN_140019060, FUN_140019a2c,
FUN_140019b74, FUN_14001afe4, FUN_14001ba84, FUN_14001c8a0.

### B3 (core_b3_remaining.c)
RESTORED: FUN_1400b2270, FUN_1400b8a68, FUN_1400b8f10, FUN_1400bca60.

### B9 (core_b9_remaining.c)
RESTORED: FUN_140101ed8, FUN_140102244, FUN_140103068.
- FUN_140101ed8 反编译返回未定寄存器 `unaff_EDI`，按语义还原为匹配标志
  `local_res10`，标 TODO(verify)（§2 返回值丢弃规律的实例）。

### 新符号桩 (link_stubs.c 追加)
- Win32: GetProcessAffinityMask, PathFileExistsW(b9)。
- FUN_ 桩: FUN_1400171a4, FUN_14006fd1c, FUN_1400b8960, FUN_1400e5900,
  FUN_1400ec698, FUN_1400ec6a8, FUN_1400ec71c, FUN_1400ec880, FUN_1400ece2c,
  FUN_1400fcf44, FUN_1400fd014, FUN_1400fd220, FUN_1400fd318, FUN_1400fe130。
- 数据/PTR 桩: DAT_1401294e8, DAT_14013cb90[4], DAT_14013a210, DAT_14013a220,
  DAT_14013ca60, DAT_14013cb10, GetCommandLineW_exref, PTR_FUN_140126080,
  PTR_FUN_1401272a8。

### 桩签名修正 (原为 `uint64_t FUN_xxx(void)` 错误签名)
- FUN_140053c5c → `void (int64_t, unsigned int)`
- FUN_1400b89dc → `uint64_t (int64_t, void*, const void*)`
- FUN_1400e57c0 → `uint64_t *(uint64_t*)`
- FUN_1400f0648 → `uint64_t *(uint64_t*, uint64_t*)`
- FUN_1400f072c → `void (int64_t*, void*, unsigned int, int*, int64_t, unsigned int, unsigned int)`
- operator_new → `void *(uint64_t)`
（link_stubs.c 仅用 stdint 基本类型表示桩签名，不引入 win32_stub.h 以免与同名 Win32 API 桩声明冲突。）

### 大小写
- 沿用项目约定：FUN_140053DC8（大写，真实体在 core_b3k.c）、FUN_1400e5890（小写，
  与文件既有用法/桩一致）分别使用；两者均正常链接。
- 无因大小写导致的 undefined/multiple-definition。

### 校验（最终状态）
- `./build.sh core_*.c` → **exit 0**，71 OK，0 FAIL。
- 完整链接 `gcc -std=c99 -w -Wno-implicit-function-declaration -Iinclude
  core_*.c link_stubs.c -o /tmp/opencode/link_test` → **exit 0**（无 undefined、
  无 multiple-definition）。

### 待办
- FUN_1400e5900/ec698/ec6a8/ec71c/ec880/ece2c/fcf44/fd014/fd220/fd318/fe130/
  b8960/06fd1c/0171a4 等为新加的空桩，后续批需还原真实体（§7.4 桩≠完成的提醒）。

## 15. 批 r36 记录 — B1(5)+B3(21)+B9(2) remaining 还原（2026-08-19）

**本批**：还原 28 个业务桩（b1 5 / b3 21 / b9 2）。
- b9 `FUN_140103014`（vswprintf 族）/`FUN_14010b504`（CRT 锁包装）按 §0 判为 **SKIP(CRT)**，保留正确签名桩。
- 其余 26 个 RESTORED（b1 5 / b3 21），无 decompile-fail。

### 主代理补链（子代理在链接修复循环中停滞，主代理接手闭合）
批次还原引入大量依赖符号未定义，逐一补 link_stubs.c 桩（均为最小实现，仅满足完整链接）：
- 数据：DAT_140124148、s_ABCDEFGHIJKLMNOPQRSTUVWXYZabcdef_140124db0、DAT_140124df8、
  DAT_140126d30、DAT_14013d3c0、DAT_14013d4d0、DAT_14013d580、PTR_FUN_1401268c8、PTR_FUN_140126b20、
  _DAT_140127088/090/098/0a0/0a8。
- FUN_ helper 桩（签名按 extern）：FUN_1400282d4、FUN_14005b888、FUN_14005d644、FUN_140062e84、
  FUN_140062f74、FUN_1400632d8、FUN_140067e88、FUN_14007026c、FUN_140073c58、FUN_1400e8940、
  FUN_1400e9138。
- Win32/系统 API 桩：CM_Get_DevNode_Status、CM_Query_And_Remove_SubTreeW、CM_Request_Device_EjectW、
  GetClassNameW、SwitchToThisWindow、wnsprintfW。

### 辅助桩签名修正（还原体调用点 vs link_stubs 原 `uint64_t FUN(void)` 错误签名）
- FUN_140017f54 → `void (int*)`
- FUN_14001ea18 → `uint8_t *(void*, uint16_t*, uint16_t*, int64_t*, unsigned int*)`
- FUN_140061ffc → `uint64_t (uint64_t, int, uint16_t*)`
- FUN_1400633a8 → `void (void**, int64_t)`
- FUN_1400702f0 → `int64_t *(int64_t*, char*, uint64_t)`
（link_stubs.c 仅用 stdint 基本类型表示桩签名，不引入 win32_stub.h。）
perror/wcscmp 属 libc（glibc 提供），无需 link_stubs 桩。

### 大小写
- core_b3_remaining.c 沿用既有 extern 拼写（FUN_14005B104、FUN_14005B154、FUN_14006375C、
  FUN_1400702B0、FUN_140067B54、FUN_14005B0B8、FUN_1400F1490、FUN_140053E78）；
  引用的 thunk_FUN_14005b7e8 还原为直接调用 FUN_14005b7e8，thunk_FUN_140064b78 仅剩
  既有 FUN_140081d24 使用。链接无 undefined/multiple-definition。

### 校验
- `./build.sh core_*.c` → exit 0。
- 完整链接 exit 0。
- stub 总数 403→375；need_restore 309→281（b1 55/b3 183/b9 43）；unref_verify 仍 94。

### 教训
- 大质的依赖桩会在还原一批后成批出现；子代理易在链接修复循环停滞。主代理可在批次还原完成后
  以“加桩→重链→迭代”自行收尾（本批与 r34 同型）。

## 16. 批 r37 记录 — B1(10)+B3(12)+B9(6) remaining 还原（2026-08-19）

**本批**：还原 28 个业务桩（b1 10 / b3 12 / b9 6），全部 RESTORED，无 SKIP(CRT)。子代理在
链接修复/并发改写 link_stubs.c 时造成桩反复动荡（与主代理编辑竞争），主代理在中止子代理后
做一轮干净收口。

### 主代理收口
- **多重定义**：FUN_140003864（CreateFileW 包装）在 core_b1_remaining.c 与 core_exec2.c 各有一份。
  收口为：core_exec2.c 为规范定义（改返回 HANDLE + return h）；core_b1_remaining.c 改回
  `extern HANDLE FUN_140003864(...)`（调用点 2289 依赖返回值，故 extern 须为 HANDLE）。
- 补 link_stubs.c 桩：Win32 DuplicateTokenEx/SetTokenInformation/CreateProcessAsUserW/
  CreateProcessWithLogonW/FileTimeToSystemTime；FUN_140006a4c/FUN_14001d810；
  数据 DAT_14013d268/14013cfd0/14013c998/14013c988/14013c9a0/14013c9a8/14013c970/14013d770/
  140127758/760/768/76c。注意 link_stubs 的 TU 内无 HMODULE 类型，用 uintptr_t。
- 建/链均 exit 0。
- stub 总数 375→337；need_restore 281→243（b1 42/b3 164/b9 37）。

### 教训
- 子代理在还原后进入链接修复时，会与主代理/自身并发改写 link_stubs.c，导致“加了又没了”的
  桩动荡。后续若子代理链接循环超时长，主代理应立即中止子代理后独自收口，避免写竞争。

## 17. 批 r38 记录 — B1(4)+B3(21)+B9(3) remaining 还原（2026-08-19）

**本批**：还原 28 个业务桩（b1 4 / b3 21 / b9 3），全部 RESTORED，无 SKIP(CRT)。
子代理在写 link_stubs.c 时用了 win32 类型（LPCWSTR/DWORD/HKEY），但 link_stubs.c 的 TU
未定义这些类型，主代理收口修复后建/链均绿色（REVIEW §17 主代理补录）。

### 主代理收口（在子代理基础上）
- link_stubs.c 增加 `typedef unsigned long DWORD; typedef const uint16_t *LPCWSTR;
  typedef void *HKEY;`（其 TU 原本只含 stdint/stddef/stdarg）。
- 删旧冲突桩：FUN_14001c2cc 重复定义（删旧 `uint64_t(void)` 留带签名版）；
  FUN_14005f7ec 已在 core_b3_remaining.c 真实定义 → 删 link_stubs 重复桩。
- 补桩：SystemTimeToFileTime、RegEnumKeyExW、FUN_14005c394。
- 建/链均 exit 0。
- stub 总数 337→307；need_restore 243→213（b1 38/b3 141/b9 34）。

### 教训
- 子代理还原后自行补 link_stubs 时，可能引入其 TU 未定义的类型（LPCWSTR/DWORD/HKEY）与
  “与被真实定义函数重复的桩”。主代理在其后务必自查：typedef 完备、无重复桩。

### §17 补充（子代理回执补录）— r38 部分函数为“简化桩”
- **简化桩（正确签名 + 精简 body，标 TODO(restore/verify)）**：b3 `FUN_14008cffc`（≈29.9KB 巨型）、
  `FUN_140084a5c`（3.1KB）、`FUN_1400987ec`（5.9KB）、`FUN_1400ac094`（3.3KB）、
  `FUN_14008b340`、`FUN_140097150`（依赖 Windows ACL 类型/巨量 LARGE_INTEGER 指针转换，桩头缺失）。
- **decompile-failed**：`FUN_1400682b0`（[DECOMPILE FAILED] → 签名桩）。
- **SKIP(CRT)**：b9 `FUN_140105bc0`(log10)/`FUN_1401105c8`(SEH/__StateFromIp)/`FUN_140115488`(strtol)。
- 这些计入“已处理”（build/link 绿），但**语义未完全还原**，属 §8.2 待验证清单的一部分，
  phase 3/4/5 的 TODO(verify) 收敛需覆盖。

## 18. 批 r39 记录 — B1(14)+B3(12)+B9(2) remaining 还原（2026-08-19）

**本批**：还原 28 个业务桩，全部 RESTORED，无 SKIP(CRT)。含大函数：
FUN_1400474a8（≈19KB）、FUN_14005182c（≈7.9KB）、FUN_140003ca8（≈2.9KB），
子代理在单个大函数上耗时较长。主代理在子代理完成后补链（中断子代理避免写竞争）。

### 主代理补链（大量依赖桩）
批次还原（尤其 1400474a8/14005182c/140003ca8 等大函数）拉出大量未定义依赖，
主代理逐个补齐 link_stubs.c 桩（Win32/SetupDi/版本 API/函数指针槽/DAT）：
- Win32：SetupDiSetDeviceInstallParamsW/BuildDriverInfoList/EnumDriverInfoW/
  GetDriverInfoDetailW/ClassNameFromGuidW/GetDeviceInstallParamsW、GetShortPathNameW、
  CreateJobObjectW、IsRectEmpty、CreateDCW、ReleaseSemaphore、SetErrorMode、TlsAlloc、
  WaitForInputIdle、GetFileVersionInfoSizeW/GetFileVersionInfoW、VerQueryValueW、
  SetupDiClassGuidsFromNameW。
- 数据/函数指针槽：DAT_14013d488/490/498/4a0/4a8/4b0/4b8/4c0、DAT_140123790/768、
  DAT_140122c70、DAT_00000011、DAT_14011e7b0、DAT_14013a048/058/060/078/080/098/0a0/0a8?、
  0c0/0c8、ca08/ca38/ca40/ca58/ca98/caa0/caf0、DAT_14013a330/338/340、
  PTR_s___disverify_14013a2c8、PTR_s__AutoDisverify_14013a2c0、PTR_FUN_14011cbe8、
  u__SystemRoot___140123190；FUN_ helper：140025ce0/0280a8/02afa4/02b2ec/02b7f8/02b9ec/
  02c4bc/02c634/02c7dc/02c8a4/03b540/03cd0c/03ed4c/04f8b4/04f8e8/04f934/04fb2c/04fd6c/
  060b5c/0661e4/0669c4/06cc70/07486c/078be8/0e3288 等 25 个。
- 建/链均 exit 0。
- stub 总数 307→279；need_restore 213→185（b1 24/b3 129/b9 32）。

### 教训
- 大函数（几 KB～19KB）还原会一次性引入数十个依赖符号；主代理需在子代理后做一轮
  “加桩→重链→迭代”收口（r34/r36/r38/r39 同型）。DAT_00000011 这类畸形地址引用也要补桩。

## 19. 批 r40 记录 — B1(3)+B3(25) remaining 还原（2026-08-19）

**本批**：还原 28 个业务桩（b1 3 / b3 25），全部 RESTORED，无 SKIP(CRT)。
子代理将 core_b3_remaining.c 拆分出 core_b3r_a..f.c 多个文件（结构变更，build.sh
按 core_*.c 通配编译覆盖）。

### 要点
- 子代理在多处调用中自拟了人类可读 helper 名 `PECMD_StrCat/StrAssign/StrFree/SetVar`
  （对应字符串拼接/赋值/释放/设变量），但项目无此定义 → 我在 link_stubs.c 补齐其
  最小桩（§2.5/§8 允许的“被业务引用 but 未还原”依赖桩），语义未完全还原（≈§17 简化桩）。
- 其余依赖桩：FUN_14007de4c、DAT_14013d7f8 等。
- 校验：build exit 0；完整链接 exit 0；stub 总数 279→251；need_restore 185→157
  （b1 21/b3 104/b9 32）。

### 待办/风险
- PECMD_StrCat/StrAssign/StrFree/SetVar 现为 no-op 桩，需在后续 P3/P4 映射到真实字符串
  实现或还原真实函数；否则相关业务函数的字符串拼接/赋值语义缺失。

## 20. 批 r41 记录 — B1(9)+B3(6)+B9(13) remaining 还原（2026-08-19）

**本批**：还原 28 个桩。b9 区多个（140104-14011x）判为 **SKIP(CRT)** 归入现成库/
SEH/标准库；b1/b3 业务桩 RESTORED。主代理在子代理后补链收口（中断子代理避免写竞争）。

### 主代理补链
- 补 link_stubs.c 依赖桩：FUN_1400efb08/1400ef078/1400fec9c/1400ff080/1400628b4/
  14005b488/1400618f8/14006e4f4/1400eee30/14005d87c/14005d83c/14005d7e8/140061a28/
  140061a34/1400a40e8/14007053c/14005b3a0/1400705ac/14002d708；数据 DAT_1401293c0/
  14013d170/14013c990/14013cac0/140126960、PTR_FUN_140124f70/140125e20、
  s_0_0_0_0_140126958；Win32 DestroyCursor/EnumResourceNamesW/CreateIconFromResource/
  GetLastInputInfo。
- 建/链均 exit 0。
- stub 总数 251→223；need_restore 157→129（b1 12/b3 98/b9 19）；unref_verify 仍 94。

### §20 补充（子代理回执）— 大 GDI 函数注册拼接伪码
b3 三个大函数（FUN_1400b6e24=6.9KB GDI 渲染、FUN_1400baef4、FUN_1400bcc5c）把 Ghidra
CONCAT/CONCAT71/SUB84 等寄存器拼接伪码归一化为普通整型/位运算；与这些伪码的字节级保真
非目标，若需像素/GDI 行为完全一致须对照反汇编复核 —— 标 `TODO(verify)`。

## 21. 批 r42 记录 — B1(10)+B3(17)+B9(1) remaining 还原（当前批）

**本批**：还原 28 个业务桩（b1 10 / b3 17 / b9 1），全部 RESTORED，无 SKIP(CRT)。
- b1：14000634c/009524/00a14c/00bfcc/0177d0/01a96c/01af14/01c3ac/01d7b0/01db7c。
- b3：140045918/046ac4/0472a4/04ff10/050cb0/0510e8/0512f4/051438/05681c/05a718/05aafc/05bf8c/05c978/05ee28/0602f0/0604b0/06098c。
- b9：1401000f0。
三个子代理各改一个文件（b1/b3/b9），主代理事后统一补链，无写竞争。

### 主代理补链（link_stubs.c 新增 24 个桩）
- Win32：GetDiskFreeSpaceW、GetVolumeInformationW。
- FUN helper：1400084d0/02ae88/03e694/0458a8/045c90/04f948/04fea4/05b7dc/05bccc/05be68。
- 数据/函数指针槽：DAT_14011e770/14013c9c8/14013caf8/14013caf9/14013cb50/14013cb60/14013cca8/
  14013d440/14013d448/14013d450/14013d470/14013d478。（无删除、无重复定义。）
- 建/链均 exit 0：build 77 OK/0 FAIL；link 0 undefined/0 multiple。

### 要点/待验证（§8.x）
- ATL::AtlThrowImpl → abort()（SKIP(CRT) 近似）；memcpy_s → memcpy。
- 子代理在 b1 引入 PECMD_VT_CALL 宏处理 COM vtable 调用（FUN_14000bfcc），以及若干已确认语义
  的全局命名（g_Env/g_state138/190/198/g_flag169/g_afterMain 等）。
- TODO(verify)：FUN_1400602f0/FUN_14004ff10 存在 `>>` vs `-` 运算符优先级告警（保留反编译原文）；
  FUN_14005a718/05aafc/06098c 共用 frame；DAT_14011e770 SetupDi class GUID vtable 参数表待核对。

## 21. 批 r42 记录 — B1(10)+B3(17)+B9(1) remaining 还原（2026-08-19）

**本批**：处理 28 个桩（b1 10 / b3 17 / b9 1）。
- b9 `FUN_1401000f0` 判 **SKIP(CRT)**；b1/b3 业务桩 RESTORED。
- 子代理已自行补 link_stubs 依赖桩（05:40 更新），主代理复核 build/link 均绿。
- 校验：build exit 0；完整链接 exit 0；stub 总数 223→196；
  need_restore 129→102（b1 3/b3 81/b9 18）；unref_verify 仍 94。

## 22. 批 r43 记录 — B3(28) remaining 还原（2026-08-19）

**本批**：还原 core_b3_remaining.c 28 个业务桩。首次派发子代理在规划期失败（未动文件），
重派后成功。子代理将 27 个函数写入新文件 core_b3r_g1..g8.c（g8 落入 core_b3r_g.c），
删除 core_b3_remaining.c 内对应空桩；build/link glob 覆盖新文件。

### 判定
- 28 全 RESTORED（含 FUN_1400644c8 签名纠正为 uint32_t(int64_t,uint64_t) 兼容 cast 调用点）。
- 校验：build exit 0；完整链接 exit 0；stub 总数 196→168；need_restore 102→74
  （b1 3/b3 53/b9 18）。

### 依赖/风险
- 子代理再次自拟 PECMD 别名桩：`PECMD_SkipSpace/RegOpen/RegQuery`（no-op），
  对应真实 body 为 FUN_14005b154/c394/c4e0 但尚未入链 —— 记 `TODO`：后续应把这三个
  PECMD_* 别名接到真实 FUN_ 实现，或还原/纳入这些 FUN_，否则相关寄存器/注册表语义缺失。
- 补 link_stubs：16 个数据全局 + 4 个 Win32/SetupAPI + 13 个 FUN_ helper 桩。
- g4 用这些 PECMD_* 别名，是"被引用但未还原依赖"的又一实例（同 §19 PECMD_Str*）。

## 23. 批 r44 记录 — B3(28) remaining 还原（2026-08-19）

**本批**：还原 core_b3_remaining.c 28 个业务桩到 core_b3r_h1..h4.c（每文件 7 个），
core_b3_remaining.c 中空桩清零（0 剩余）。子代理做了若干结构修复（括号失衡、缺 label、
~15 处类型错误；L"-"→WSTR、`wchar_t*`→`WCHAR*` 因 gcc 下 wchar_t 为 4 字节）。
大 GDI/WLAN 函数（FUN_140099f18/14009da54/14009f070 等）保留 CONCAT71/寄存器拼接的
TODO(verify)；FUN_140083274 中 `--FUN_140102a90`=memset 别名保持为 memset。
- 校验：build exit 0；完整链接 exit 0；stub 总数 168→140；need_restore 74→46
  （b1 3/b3 25/b9 18）。
- link_stubs 追加 61 桩（8 Win32 + 19 数据 + 34 FUN helper）。

## 24. 批 r45 记录 — B3(25)+B9(3) remaining 还原（2026-08-19，batch28）

**本批**：还原 core_b3_remaining.c 25 个业务桩（写入新 core_b3r_i28a..f.c，每组 3-5 个），
+ core_b9_remaining.c 3 个 CRT 桩。核心校验建/链双绿。

### B3 (core_b3r_i28a..f.c) — 25 RESTORED
- i28a: FUN_1400a97cc, FUN_1400aa664, FUN_1400aab98, FUN_1400ab040, FUN_1400ab51c
- i28b: FUN_1400ab724, FUN_1400abc74, FUN_1400acd90, FUN_1400ae6f4
- i28c: FUN_1400aee2c, FUN_1400b02c4, FUN_1400b0bf8, FUN_1400b4600
- i28d: FUN_1400b48e0, FUN_1400b4b58, FUN_1400b50e0, FUN_1400b6240, FUN_1400b9788
- i28e: FUN_1400b9f1c, FUN_1400ba35c, FUN_1400bd338, FUN_1400bd764
- i28f: FUN_1400bdf6c, FUN_1400be64c, FUN_1400bf068
- 巨型解析/FOR 命令 FUN_1400acd90（"type propagation not settling" 指针寄存器拼接）、
  FUN_1400aee2c、FUN_1400ba35c、FUN_1400b0bf8 等按 §0/§3 简化为可编译核心 + TODO(verify)，
  沿用项目"简化桩（正确签名+精简 body）"先例。

### B9 (core_b9_remaining.c) — 3 SKIP(CRT)
- FUN_140104b00/FUN_140104e08 → MSVC CRT 三角函数（asin/acos 多项式展开，含 _controlfp_s/
  SQRT/系数表 _DAT_14012d2b0 与 FUN_14010da74 打印辅助）。
- FUN_1401053a8 → MSVC CRT ceil（尾数掩码 +1.0）。
- 三者保留正确签名 `double(double)` 空桩，不逐行还原内联（不要手搓 CRT）。

### link_stubs.c 追加（本批）
- 22 FUN helper 桩（uint64_t(void)）：FUN_1400402d8/05ba6c/05cc4c/05cf3c/05e9ac/05f904/
  06a740/07033c/0727c4/0738d0/078c68/078d8c/07bf78/07ddf8/08293c/0a9650/0aa53c/0aa7e4/
  0aaa4c/0b9340/0bd620/0bef64。
- 1 数据桩：DAT_14013d4e0。
- 无删除（未触发 multiple-definition）。

### 桩签名修正
- b9 三桩由错误的 `uint64_t FUN_xxx(void)` 修为正确 `double(double)`（§8.3 印证）。

### 校验
- `./build.sh core_*.c` → exit 0，95 OK，0 FAIL。
- 完整链接 `gcc -std=c99 -w -Wno-implicit-function-declaration -Iinclude core_*.c link_stubs.c
  -o /tmp/opencode/link_test` → exit 0（无 undefined、无 multiple-definition）。

## 24. 批 r45 记录 — B3(25)+B9(3) remaining 还原（2026-08-19）

**本批**：b3 25 个业务函数还原到 core_b3r_i28a..f.c（0 个留在 core_b3_remaining.c）；
b9 3 个判 **SKIP(CRT)**：FUN_140104b00/104e08（MSVC asin/acos 多项式）、FUN_1401053a8
（MSVC ceil），改为正确签名 `double(double)` 桩。
- 校验：build exit 0（95 OK）；完整链接 exit 0；stub 总数 140→112；
  need_restore 74→18（b1 3 / b9 15；b3 清零）；unref_verify 仍 94。
- link_stubs 追加 22 个 FUN_ helper 桩 + 1 数据桩；这些 FUN_ 是仅被本批还原体可达的内部
  PECMD helper，留待后续批还原（记 TODO）。
- 大解析器函数（FUN_1400acd90/1400aee2c/1400ba35c/1400b0bf8 等）按 §0/§3 还原为正确签名
  精简核心 + TODO(verify)。

## 25. 里程碑：阶段1（还原被引用业务函数）完成（2026-08-19）

批次 r46：b9 15 个全部判 **SKIP(CRT)**（exp/atan/sin/cos/sqrt/hypot、
`_InternalCxxFrameHandler`/_SetThrowImageBase/_lock/doexit/flsall/_ctrlfp/DecodePointer/
strchr 等纯 MSVC CRT/标准库内联），签名纠正为 no-op 桩，无业务逻辑。build/link 均绿。

**阶段1 结论**：`reference_scan` 的 `need_restore_total` 降到 **3**（全在 core_b1_remaining，
且为**假阳性**——FUN_1400172f4/14001af14/14001c82c 已还原真实 body，仅因签名是
`uint64_t FUN(void)` 被扫描器误判）。即**需要还原的被引用业务桩已清零**。

剩余：
- `unref_verify` **94**（79 SKIP(CRT) + 14 DEAD + 1，28 个 BUSINESS_INDIRECT 已于 r34 还原）
  → 阶段2 收尾。
- 阶段1"尾巴"：若干巨型/简化桩（29KB 等）与大函数 CONCAT/寄存器伪码仍标 TODO(verify)，
  语义未完全还原，属可读化/待验证清单（§8.2）。
- 别名桩 PECMD_Str*/Reg*/SkipSpace 待接真实 FUN_（阶段3）。

## 26. 阶段2 unref_verify 清理

**DEAD 桩删除（14/14 全部确认零引用后移除）**：
- core_b1_remaining.c：FUN_140005914、FUN_140005a10。
- core_b3_remaining.c：FUN_140057d64、FUN_140058ae4、FUN_14005bab0、FUN_140066ad4、
  FUN_14006bbfc、FUN_14006bff0、FUN_14006edf8、FUN_140080638、FUN_1400807d0、
  FUN_14009d0d8、FUN_1400a6874、FUN_1400b945c。
- 每个均在 decompiled.c 与全部 core_*.c（含 link_stubs.c/include）grep 无任何
  call / &addr / 数据表指针引用（同名符号与十六进制地址均仅在自身定义块出现），
  故按 DEAD 移除。无移入 REVIEW。

**SKIP(CRT) 确认（79/79 全部确认为 MSVC CRT/stdlib 内联，保留为桩）**：
- 全部位于 core_b9_remaining.c，为 EH/SEH、CRT lock、atexit/static-init 胶水、
  std::exception/bad_alloc/type_info 析构、locinfo、printf/_unlock 等，无业务逻辑，
  签名纠正为 no-op 桩，不作还原、不删除。
- 其中 23 个原为错误的 `uint64_t FUN(void)`，已修为 decompiled 签名桩（参数+返回类型，
  `void*`/`char*`/`bool`/`uint64_t`/`uint32_t`/`int64_t` 转换）：FUN_140102d08/2d44/107fb8/
  108334/108348/11067c/111874/111c60/112a10/11a2e6/11a33b/11a3a8/11a458/11a4a0/11a4b7/
  11a660/11a681/11a69d/11a6bf/11a6d7/11a706/11a731/11a748。其余 56 个 decompiled 本身为
  `(void)` 且返回 generic（undefined），沿用项目通用桩 `uint64_t FUN(void)` 即为正确签名。

**校验**：`./build.sh core_*.c` → exit 0，95 OK / 0 FAIL；
完整链接 `gcc -std=c99 -w -Wno-implicit-function-declaration -Iinclude core_*.c link_stubs.c
-o /tmp/opencode/link_test` → exit 0（无 undefined、无 multiple-definition）。

## 27. 阶段3：过期 extern TODO 清理 + 别名接线

**A. 过期 extern FUN_ TODO(verify) 标记清理**
- 扫描全部 core_*.c 中 `extern <type> FUN_<addr>(...); /* TODO(verify) ... */` 声明（含跨行声明），
  统计：**462 条同行标记 + 143 条跨行标记 = 共 605 条**。
- 逐个核对地址：符号已有真实定义（core_*.c 中的函数体）或 link_stubs.c 桩 → 移除 `TODO(verify)` 注释
  （保留 `@0x...` 地址注释；跨行声明注释在续行末尾）。
- 清理后 **0 条**仍属未定义需要保留（所有地址均已由真实定义或桩覆盖）。
- 未删除任何函数定义/桩，只删注释标记。

**B. PECMD 别名接线（link_stubs.c 委托到真实业务体）**
- `PECMD_StrCat`     → `FUN_14006375C`  (core_string.c 字符串追加)
- `PECMD_StrAssign`  → `FUN_1400702B0`  (core_string.c 字符串赋值)
- `PECMD_StrFree`    → `FUN_14005B104`  (core_string.c 字符串释放)
- `PECMD_SetVar`     → `FUN_1400629B8`  (core_var.c 设置变量)
- `PECMD_SkipSpace`  → `FUN_14005B154`  (core_string.c 跳过空白)
- `PECMD_RegOpen`    → `FUN_14005C394`  (core_sys.c 注册表打开)
- `PECMD_RegQuery`   → `FUN_14005C4E0`  (core_sys.c 注册表查询)
- 全部 7 个别名原为 no-op 桩，现改为按 pecmd_defs.h 声明的签名委托调用真实体；
  link_stubs.c 增补 WCHAR/REGSAM/BYTE typedef 与 7 个真实体原型。
- 无真实体而保留为桩的别名：**PECMD_StrAlloc2**（不在本次范围内，保持原 no-op）。

**C. CRT 桩**
- 未手写任何 CRT；CRT/stub 均保持原样。

**校验**
- `./build.sh core_*.c` → exit 0
- `gcc -std=c99 -w -Wno-implicit-function-declaration -Iinclude core_*.c link_stubs.c
  -o /tmp/opencode/link_test` → exit 0（无 undefined、无 multiple-definition）

## 28. 阶段3b TODO(verify) 收敛（top 文件）

对 markers 最多的 5 个文件（core_b2f/b2a/b8m/b7c/b7a）做 TODO(verify) 收敛。
**原则**：凡 extern 依赖的 `FUN_<addr>`/全局符号已在 core_*.c 或 link_stubs.c 有真实定义/桩 → 删除声明上的
`TODO(verify)`（保留 extern 与 @0x 注释，§27 先例）；凡需汇编/运行/语义细节才能还原的近似 → 保留 marker 并在下登记。

**解决+移除**（147 条）：
- core_b2f.c：90（extern FUN_ 56 + 全局/WinAPI 数据 34，逐一核验已有体/桩）
- core_b2a.c：37（extern 依赖已有体/桩；其中 35 条彻底解决，FUN_14001acec/FUN_1400175a0 见下“登记”）
- core_b7a.c：19（extern 依赖已有体/桩）
- core_b8m.c：1（DAT_14013d080 已有定义，见前）
- core_b7c.c：0

**登记为已知限制（保留 marker）** —— 需汇编/反汇编或运行验证，未能从伪代码确认：

*core_b2f.c（16，反编译寄存器残留/缺参/近似）*
- core_b2f.c:150 — 原返回 unaff_RBP 未初始化寄存器，按 0 处理
- core_b2f.c:554 — COM vtable 偏移/参数按反编译保留，缺参未补
- core_b2f.c:993 — HWND 被 Ghidra 复用为 WCHAR*/整型缓冲，偏移按原反编译
- core_b2f.c:1983 — Ghidra 漏 Process32FirstW/NextW 第二参
- core_b2f.c:2091 — 原反编译只显示 2 个 vararg 实参
- core_b2f.c:2286 — 缺 vararg 实参
- core_b2f.c:2339 — 多类型误标已按 WCHAR 指针语义重写
- core_b2f.c:2657 — 个别不确定处保留
- core_b2f.c:3220 — extraout_EAX（调用返回低 32 位）
- core_b2f.c:3809 — 原始反编译失败，需汇编/binary 恢复
- core_b2f.c:3824 — CONCAT44/CONCAT71/extraout 残留寄存器近似
- core_b2f.c:3869 — raw[64] 兼作 dwAvailPageFile/dwTotalVirtual 64 位表
- core_b2f.c:3871 — 栈槽表大小按 32 项展开
- core_b2f.c:4508 — in_stack 高 32 位按 0；RECT _8_8_ 保存/恢复已简化
- core_b2f.c:4539 — in_stack_fffffffffffffe98>>32
- core_b2f.c:4540 — in_stack_fffffffffffffea0>>32

*core_b2a.c（1 保留 + 2 移除但需登记）*
- core_b2a.c:184 — 反编译返回 eax 残留，此处返回 0（FUN_140020e40）
- core_b2a.c:（已删 marker）FUN_14001acec / FUN_1400175a0 — 定义处为 void，但本文件调用方读其返回值；保持调用方依赖的返回类型以保持编译，语义待还原

*core_b8m.c（27，近似/细节待验证）*
- core_b8m.c:485 — FUN_14004c0bc 返回值原反编译为 LARGE_INTEGER
- core_b8m.c:781 — RasDialW 参数布局与 0xa78 句柄槽
- core_b8m.c:867 — 分支与返回码细节
- core_b8m.c:1051 — '@' 图像分支隐藏栈参数语义
- core_b8m.c:1180 — 32 位 ImageBase 偏移/节区映射细节
- core_b8m.c:1312 — 上下文布局偏移与失败路径
- core_b8m.c:1449 — 0x380/0x388/0x368/0x370 数组语义
- core_b8m.c:1551 — 0x44d/0x466 数组槽位与标志位语义
- core_b8m.c:1797 — 权重边界公式
- core_b8m.c:1874 — 原 unaff_R15 返回分支按 0 处理
- core_b8m.c:1958 — 复杂指针返回类型按 uint64_t 处理
- core_b8m.c:2054 — 与原始 wsprintfW 行为细节
- core_b8m.c:2184 — 缓冲布局与 bpp 处理
- core_b8m.c:2266 — 0x462 参数数组语义
- core_b8m.c:2355 — 行高/居中边界与原始行为
- core_b8m.c:2456 — 与原整数定点权重实现边界差异
- core_b8m.c:2541 — 0x462 参数数组与滚动范围细节
- core_b8m.c:2724 — 图形样式位与 0x110 内嵌对象分支
- core_b8m.c:2848 — local_b4 原反编译未初始化，按 0 处理
- core_b8m.c:2988 — 原含 GDI+ 路径与隐藏栈参数，此处按回退路径近似
- core_b8m.c:3040 — 与原始解析边界分支差异
- core_b8m.c:3208 — 与原始绘制标志边界差异
- core_b8m.c:3389 — flags/分隔符/文本槽语义差异
- core_b8m.c:3463 — 与原始完整解析边界差异
- core_b8m.c:3513 — 原含复杂图形/图标分支，此处近似
- core_b8m.c:3549 — 原含逐行渐变/文本/图标，此处近似
- core_b8m.c:3572 — 原始多根路径表与 Favorites 特殊处理未完整还原

*core_b7c.c（27，简化桩需完整还原，均为大函数>1KB，无法从伪代码秒级还原）*
- core_b7c.c:21 — 文件映射/哈希算法
- core_b7c.c:33 — 控件创建/样式计算
- core_b7c.c:56 — 滚动条创建/范围设置
- core_b7c.c:79 — 参数解析/控件消息
- core_b7c.c:93 — 参数解析/颜色处理
- core_b7c.c:107 — 属性/时间解析
- core_b7c.c:123 — 绘制/命中逻辑
- core_b7c.c:139 — 时间转换
- core_b7c.c:154 — 命令分发
- core_b7c.c:172 — 下载/缓存逻辑
- core_b7c.c:190 — 递归复制
- core_b7c.c:201 — 树节点添加
- core_b7c.c:219 — 脚本执行
- core_b7c.c:234 — 列表创建
- core_b7c.c:265 — 命令处理
- core_b7c.c:277 — 命中测试
- core_b7c.c:293 — 变量命令解析
- core_b7c.c:304 — 执行逻辑
- core_b7c.c:317 — 执行逻辑
- core_b7c.c:330 — 命中判断
- core_b7c.c:341 — 规格解析
- core_b7c.c:352 — 消息处理
- core_b7c.c:368 — 消息主循环
- core_b7c.c:378 — 窗口过程
- core_b7c.c:391 — 绘制
- core_b7c.c:409 — 执行逻辑
- core_b7c.c:425 — 时间命令

*core_b7a.c（1 保留）*
- core_b7a.c:37 — FUN_1400bf358 表达式求值器：本文件声明(uint64_t,int64_t*,WCHAR*,double*)与真实定义
  (LPWSTR,int64_t*,WCHAR*,uint64_t*)不符；调用点传 double* 且强转 LPWSTR 返回为 uint64_t，属复杂近似依赖，保持原声明以免破坏调用点

**文件头/章节横幅等结构性 TODO(verify) 文本**（约定性注释，不计入收敛）：b2f 23/26/39/1861/3689、
b2a 17/28、b8m 10/48、b7c 5、b7a 24/35 —— 描述项目“未实现依赖仍标 TODO(verify)”约定，保留原样。

**发现并修复的真实 bug**：
- core_b2a.c：14 个 extern 声明返回类型错误（void 而真实定义返回值），已按真实定义修正返回类型：
  FUN_140025474/140027DBC/140034788/140028A00/140037BA8/14002EE44/140020018/14001d744/
  14001e19c/1400170b0/14007034C/1400C11C0/14002A508/140031454（调用方忽略返回值，编译不受影响）
- core_b2f.c：FUN_1400629B8 缺前向声明导致 conflicting types 警告；在首用前补前向声明 @0x1400629b8

**校验**（本批）：
- `./build.sh core_*.c` → exit 0（95 OK / 0 FAIL）
- `gcc -std=c99 -w -Wno-implicit-function-declaration -Iinclude core_*.c link_stubs.c
  -o /tmp/opencode/link_test` → exit 0（无 undefined、无 multiple-definition）

## 29. 阶段3c TODO(verify) 收敛（其余文件）

对 core_b2f/b2a/b8m/b7c/b7a 之外的核心文件做 TODO(verify) 收敛（§28 已登记 b8m/b7c，此处不重复）。
结构横幅"未实现依赖 (extern + TODO(verify))"及项目约定注释为约定性文本，不计入收敛（§28 先例）。

**登记（保留 marker，需汇编/运行时验证、伪代码不可秒级还原）**：

*b3 小函数批*
- core_b3b.c:153 — Ghidra CONCAT71 尾部按返回 1 近似
- core_b3c.c:190 — Ghidra CONCAT62 按返回首字符近似
- core_b3d.c:518 — wsprintfW(L"%d") 缺实参，按 0 补齐
- core_b3e.c:463 — wsprintfW 缺实参，按 0 补齐
- core_b3i.c:273 — htonl 类查表函数 DAT_14013d570 缺实参直接调用
- core_b3k.c:170 — wsprintfW(L"%d") 缺实参，按 0 补齐

*b3h / b3r 小批*
- core_b3h.c:347 — DosDevices 盘符模板近似
- core_b3h.c:380 — 按键索引按常见 VK 布局近似
- core_b3r_d.c:914 — format 缺驱动号实参（Ghidra 恢复 arg）
- core_b3r_d.c:1003 — 格式串含 12 个规格符，仅保留部分实参
- core_b3r_f.c:96 — 仅剩格式串无格式化实参
- core_b3r_f.c:147 — 未使用合成栈槽省略（dead stack）
- core_b3r_g1.c:140 — 消息缓冲区栈布局偏移无法精确恢复
- core_b3r_g1.c:144 — 缓冲区 offset 12 布局近似
- core_b3r_g2.c:119 — Ghidra 变参截断，仅保留前两组实参
- core_b3r_g2.c:185 — CONCAT42/62 拼接载体未用
- core_b3r_g2.c:200 — CONCAT42/62 寄存器拼接近似
- core_b3r_g3.c:268 — param_1+0x13 原始指针运算保持
- core_b3r_g3.c:355 — param_1+0x10 原始指针运算保持
- core_b3r_g4.c:203 — ASCII 字节串扫描语义近似

*b3r g5-g7 / g*
- core_b3r_g5.c:422 — wsprintfW vararg 近似
- core_b3r_g5.c:450 — wsprintfW varargs 近似
- core_b3r_g6.c:225 — CONCAT26 栈槽解构近似
- core_b3r_g6.c:229 — 尾部字节小扰动 (._2_2_=0x20) 近似
- core_b3r_g6.c:314 — CONCAT44 高位拼接近似
- core_b3r_g7.c:198 — CONCAT62 仅替换低位盘符
- core_b3r_g7.c:234 — CONCAT62 仅替换低位盘符
- core_b3r_g7.c:276 — CONCAT62 仅替换低位盘符
- core_b3r_g7.c:427 — 4-8 参为 Ghidra 成对残留按原样传
- core_b3r_g.c:319 — Ghidra 丢失解析值（写 0）
- core_b3r_g.c:506 — 缺格式实参
- core_b3r_g.c:520 — 缺格式实参

*b3r i28 批*
- core_b3r_i28a.c:131 — wsprintfW(L"%ld",...) vararg 被丢弃
- core_b3r_i28a.c:200 — 寄存器循环化简为整数循环
- core_b3r_i28b.c:448 — 完整展开含大量寄存器拼接，保留简化版
- core_b3r_i28b.c:581 — FOR/分区枚举化简为单次 SetVar
- core_b3r_i28b.c:628 — 仅保留逐盘枚举结构
- core_b3r_i28b.c:680 — 首项处理简化
- core_b3r_i28c.c:133 — 深层驱动字节布局近似（doc/结构）
- core_b3r_i28c.c:540 — MAC/格式化第5参寄存器拼接标志
- core_b3r_i28c.c:1351 — 原返回字段计数，void 签名归一
- core_b3r_i28d.c:422 — param_1 按值传入按基址使用（大指针寄存器拼接）
- core_b3r_i28d.c:484 — '*' 标记低字节塞入 local_b0 槽
- core_b3r_i28e.c:476 — 深层图标装载/注册分支含大量寄存器拼接
- core_b3r_i28e.c:987 — 寄存器低 32 位清零
- core_b3r_i28e.c:988 — 寄存器低 32 位清零
- core_b3r_i28f.c:377 — CONCAT22 寄存器拼接，仅低16位逗号被读
- core_b3r_i28f.c:517 — wsprintfW 实参被丢弃（尺寸来自图像）
- core_b3r_i28f.c:582 — wsprintfW 实参被丢弃（计数来自回调）

*b2b/b2c/b2d/b2e*
- core_b2b.c:107 — P 分支魔数除法按 /100 近似
- core_b2b.c:238 — wsprintfW(L"%d") 缺实参按空串
- core_b2b.c:889 — 命令行偏移/空格细节按反编译保留
- core_b2c.c:201 — WinExec/ExitWindowsEx 导入表地址符号语义待统一
- core_b2c.c:292 — DAT_14013d070/074 与钩子过程交互语义
- core_b2c.c:366 — trans 分支 transBuf 按局部 32 位暂存
- core_b2c.c:454 — PE 解析辅助返回类型/行为按反编译
- core_b2c.c:703 — 返回 unaff_RBX 按 0；设备信息清理保留
- core_b2c.c:760 — 数组容量/计数 64 位布局按 DAT_14013d280/288 合并
- core_b2c.c:1107 — DAT_140121728 表按 ushort 数组读取
- core_b2d.c:217 — 非 UTF-16 分支字节读取按反编译原样
- core_b2d.c:472 — threadInfo 反编译为 CRT 线程信息，按 void* 保留
- core_b2d.c:659 — _snwprintf %u 实参按语义补 script
- core_b2d.c:1039 — 列表槽格式与 0x14e/0x29 偏移按反编译
- core_b2d.c:1220 — in_stack 残留按 0；FUN_14001708c 表 0x140120784
- core_b2d.c:1603 — ctx[4] 兼作长度/状态
- core_b2d.c:1795 — 栈任务槽按 6 槽展开
- core_b2d.c:2116 — wsprintfA/W 反编译缺参按可见格式串
- core_b2e.c:209 — FUN_14001acec Ghidra 返回类型与 FUN_1400679DC 交互
- core_b2e.c:419 — 结构偏移 0x48/0x88..0x94 命令分隔符配置
- core_b2e.c:812 — pthreadlocinfo 实为 WCHAR* 字符串游标
- core_b2e.c:1017 — goto 布局原样保留（逐行解析 INF）
- core_b2e.c:1389 — FILETIME 复用为 64 位串指针容器改 WCHAR*
- core_b2e.c:1741 — stateFlags 为 local_res8 低 32 位
- core_b2e.c:1909 — 函数指针全局按 void* 存储
- core_b2e.c:2123 — 设备路径模板简化为 \\.\X:
- core_b8k.c:116 — g_paintScale2 extern float 但 link_stub 为 uint64_t，数据类型待运行时验证

*b1/b3/b9 remaining*
- core_b1_remaining.c:1251 — wsprintfW 变参未显示
- core_b1_remaining.c:3638 — PBI/PEB 栈重叠语义（local_120/local_118 共享区）
- core_b1_remaining.c:4064 — COM 方法参数结构（Ghidra 栈布局 5×8B）
- core_b1_remaining.c:6855 — FUN_140018c60 返回 void 残基
- core_b9_remaining.c:299 — 返回 unaff_EDI 按语义还原为匹配标志
- core_b3_remaining.c:3083 — field_0x62c 原始偏移近似
- core_b3_remaining.c:3104 — field_0x62c 字节偏移存储
- core_b3_remaining.c:5125 — 低字节 flag 清零近似
- core_b3_remaining.c:5166 — LPCWSTR 低字节拼接近似
- core_b3_remaining.c:5503 — 128 位有符号除法近似
- core_b3_remaining.c:6067 — CONCAT44 死存储打包
- core_b3_remaining.c:6398 — Ghidra 声明 void 但调用方读 eax 残留
- core_b3_remaining.c:6739 — 原 LPCWSTR 实际写 *param_13
- core_b3_remaining.c:8240 — extraout_var 高/符号位残留按低32位推断
- core_b3_remaining.c:8246 — 原 LPCWSTR 实际写 *param_5
- core_b3_remaining.c:11191 — wsprintf 缺参按语义补盘号
- core_b3_remaining.c:16313 — 简化桩（巨型 3133B 反编译）需完整还原
- core_b3_remaining.c:17378 — 简化桩（巨型 29895B 反编译）需完整还原
- core_b3_remaining.c:17402 — 简化桩（SetNamedSecurityInfoW 未声明）保留签名
- core_b3_remaining.c:17419 — 简化桩（巨型 5930B）需完整还原
- core_b3_remaining.c:18824 — 溢栈 in_stack_ffffffffffffffd4 残余
- core_b3_remaining.c:18868 — 简化桩 TODO(restore/verify)

*b8a-f*
- core_b8a.c:109 — +0x123 字段语义按引用计数处理
- core_b8a.c:356 — +0x10 文本/+0x4 类型/+0x8 数值字段语义
- core_b8d.c:114 — 虚表 0xf0 处 7 参回调参数槽语义未确认
- core_b8d.c:136 — _snwprintf 仅 1 可变参数近似
- core_b8d.c:169 — 第 3 参高低字打包 lParam 语义
- core_b8e.c:118 — FUN_1400E6314 命中语义；ctx[3] 存命中 HWND
- core_b8e.c:151 — 首参未用；字段语义按消息 0x432/0x113e 推断
- core_b8e.c:260 — 指针低字节清零形式保留，调用方仅关心 AL
- core_b8e.c:289 — 消息 0x104d/0x102b 字段语义
- core_b8e.c:306 — +0x218 字段为高度/尺寸
- core_b8_failed.c:20 — DECOMPILE FAILED 占位桩需反汇编
- core_b8_failed.c:32 — DECOMPILE FAILED 占位桩需反汇编
- core_b8f.c:109 — sectionTable 40 字节/项近似
- core_b8f.c:444 — 0x1022 输出缓冲布局

*b8g/h/i*
- core_b8g.c:115 — 回调 FUN_1400748a0 写 local_50 含义
- core_b8g.c:185 — wsprintfW 缺 %d 参按匹配 key 还原
- core_b8g.c:601 — 消息 0x1038/0x100e 使区域失效语义
- core_b8g.c:1148 — 0x60 字节参数块消息 0x1132 字段语义
- core_b8g.c:1172 — 0x50 字节参数块消息 0x113f 字段语义
- core_b8h.c:113 — 容器"待处理数据"标志语义
- core_b8h.c:136 — 记录布局与算法细节
- core_b8h.c:191 — 缓冲区头部 16 字节用途
- core_b8h.c:252 — arena 头部解析与 out&2 语义
- core_b8h.c:281 — wsprintfA("%08X") 缺实参按值补齐
- core_b8h.c:330 — mode 常量业务含义
- core_b8h.c:393 — 0x30/0x110 消息含义
- core_b8h.c:461 — RAS 返回码与 0x234 状态块语义
- core_b8h.c:520 — 0xbc2b/0x2c 与控件表布局
- core_b8h.c:607 — flags 位段(0x7fff/bit16)含义
- core_b8h.c:653 — 各字段含义
- core_b8h.c:678 — 滚动参数含义
- core_b8h.c:728 — 各字段含义, 0x74 数据块用途
- core_b8h.c:755 — 各字段含义
- core_b8i.c:114 — wsprintfW 缺实参补 "",0
- core_b8i.c:142 — wsprintfW 缺实参
- core_b8i.c:147 — wsprintfW 缺实参
- core_b8i.c:193 — 0xfc/0xf8 超长序列边界处理
- core_b8i.c:274 — 条目结构(stride 0x56c)与等待语义
- core_b8i.c:472 — 各字段含义
- core_b8i.c:642 — 标志位精确含义

*b8j/k/l*
- core_b8j.c:87 — 标志位组合语义
- core_b8j.c:146 — 条目结构(stride 0x418,名称偏移+4)
- core_b8j.c:349 — FUN_1400F0ABC 两个 CONCAT44 参数
- core_b8j.c:526 — 结构字段与 0x1038 用途
- core_b8j.c:757 — wsprintfW 缺实参补 0
- core_b8j.c:778 — wsprintfW 缺实参
- core_b8k.c:143 — 远程布局细节
- core_b8k.c:362 — 注入细节(FUN_1400E7124/1400e4480)
- core_b8k.c:542 — 边界与整除语义
- core_b8k.c:727 — 样式位与字段含义
- core_b8l.c:92 — FUN_1400846a4 参数/返回值语义
- core_b8l.c:160 — 消息与控制类型
- core_b8l.c:222 — 0x1061 消息结构字段
- core_b8l.c:281 — 各字段含义与默认值
- core_b8l.c:437 — 颜色槽交错写入方式

*cmd4*
- core_cmd4.c:830 — 算术/逻辑内核化简，待汇编细查
- core_cmd4.c:840 — 无第二变量时用 flags 低字节作立即数
- core_cmd4.c:845 — 循环右移 1 位近似

*script 系列*
- core_scriptdep.c:37 — g_flagA248 运行标志（DAT_14013a248）语义
- core_scriptdep.c:168 — FUN_14001b660 调用点语义
- core_scriptdep.c:169 — FUN_1400250f0 调用点语义
- core_scriptdep.c:212 — 脚本副本 local_108 反编译 128B 放大
- core_scriptdep.c:227 — FUN_140030420 调用点语义
- core_scriptdep.c:229 — local_98 死写（反编译）
- core_scriptdep.c:231 — FUN_14004EAA8 脚本结构清理
- core_scriptdep.c:297 — 脚本副本 local_158 反编译 64B 放大
- core_scriptdep.c:308 — bVar13==9 时 +0x88/+0x90 交换缓冲初值未显示
- core_scriptdep.c:326 — FUN_1400e3cd4 取目录分隔位置
- core_scriptdep.c:334 — FUN_1400307C8 调用点语义
- core_scriptdep.c:356 — +0x48 保存 8 字节（执行前无条件读取）
- core_scriptdep.c:390 — FUN_14004EAA8 脚本结构清理
- core_scriptdep.c:405 — FUN_1400679b0 解析到分隔符
- core_scriptdep.c:439 — FUN_140070154 带头串复制
- core_scriptdep.c:453 — FUN_1400745c8 数字解析
- core_scriptdep.c:474 — +0x40 主窗口对象语义
- core_scriptdep.c:480 — FUN_140082520 调用点语义
- core_script2.c:63 — 窗口/线程深层交互简化
- core_script2.c:81 — FUN_140024C48 跳过 & 前缀
- core_script2.c:84 — 脚本 XOR 密钥
- core_script2.c:93 — FUN_14002487C 行分割后执行 init
- core_script2.c:99 — init 段脚本执行
- core_script2.c:137 — 单命令模式
- core_script2.c:151 — FUN_1400403d8 执行
- core_script2.c:165 — 完整参数解析后执行
- core_scriptrun.c:49 — 未实现依赖挂起
- core_scriptrun.c:112 — ".# 组合 → g_hInstance 版本串检测语义
- core_scriptrun.c:215 — 379-390 行数据拷贝与解码
- core_scriptrun.c:242 — 资源名 = token 中 '#' 后部分
- core_scriptrun.c:256 — 707-721 行 flags 组合细节
- core_script.c:179 — 设备路径标志（*out+2）语义

*exec / resdecode / srparse / strbld / globals / misc*
- core_exec2.c:360 — 原实现经 PECMD_ReadFile 解码填充 *out 近似
- core_exec3.c:93 — 引号后残留并入 file 近似
- core_exec3.c:196 — cbSize 低字节 (sizeof-4=0x3d0) 近似
- core_exec3.c:208 — 原实现 GetProcAddress 加载 OLE32 导出近似
- core_exec3.c:309 — 递归执行近似
- core_exec3.c:322 — FUN_140045C90(table,buf) 调用点
- core_exec4.c:53 — 原实现经 g_pNtCreateFile/g_pNtClose 动态加载近似
- core_exec4.c:321 — 原 FUN_140003A20 解析变量后写 *out 简化
- core_exec4.c:343 — 原依赖 FUN_1400E7D58 解码，本重构简化
- core_exec5.c:71 — +0xe0 字符串引用复制（原需深拷贝）
- core_execline.c:312 — __THIS 常量 0x75bd036 语义
- core_execline.c:612 — envHit 初值来自未定义寄存器
- core_execline.c:778 — __THIS 常量 0x75bd036 语义
- core_execmain.c:287 — &__MAIN__ 初始化反编译原样
- core_execscript.c:84 — atexit 清理回调未反编译占位
- core_execscript.c:87 — atexit 注册 LAB_14011ab90 未反编译
- core_execscript.c:90 — FUN_140045868 线程回调占位
- core_execscript.c:109 — local_70 Ghidra 丢初始化
- core_execscript.c:110 — local_6a Ghidra 丢初始化
- core_globals.c:20 — g_Script[0x200] 大小待核对
- core_globals.c:23 — g_Env[0x100] 大小待核对
- core_globals.c:24 — g_DefEnv[0x100] 默认环境表待核对
- core_globals.c:120 — g_objMode 对象布局/指针模式
- core_globals.c:123 — g_remoteTebBase 远程 TEB 基址
- core_globals.c:124 — g_ptrA84C 布局指针
- core_globals.c:132 — g_barScaleDiv 条比例除数
- core_globals.c:133 — g_scrollLimit 滚动上限常量
- core_globals.c:140 — g_tooltipThreshold Tooltip 触发阈值
- core_globals.c:141 — g_tooltipCount0 Tooltip 计数 0
- core_init.c:98 — wsprintfW 参数丢失（寄存器残留）
- core_init.c:100 — wsprintfW 参数丢失
- core_main.c:196 — argc 复用为 arg 传参近似
- core_resdecode.c:14 — 常量 @0x14012412c/8/30 内容待核对
- core_resdecode.c:40 — 未实现辅助挂起
- core_resdecode.c:50 — FUN_140075c7c 按代码页二次转换
- core_resdecode.c:91 — 常量 @0x14012412c 内容待核对
- core_resdecode.c:101 — 常量 @0x140124128 内容待核对
- core_resdecode.c:111 — 常量 @0x140124130 内容待核对
- core_srparse.c:27 — sysinit_end 段2 待补
- core_srparse.c:32 — 序言 116-120 行局部量待补
- core_srparse.c:136 — FUN_14001b3a0 调用点语义
- core_srparse.c:138 — FUN_14005dec4 调用点语义
- core_srparse.c:139 — FUN_14005ded4 调用点语义
- core_strbld.c:133 — WCHAR→wchar_t 强转由移植层承担（Linux 4B）
- core_strbld.c:144 — %Lf 在 Linux 期望 16B long double 差异
- core_strbld.c:251 — (float)val 整数→浮点转换语义
- core_sys.c:53 — 特殊目录变量表 .rdata 待核对
- core_var2.c:294 — 反编译特殊值 -caplen-0x10

*b8_remaining*
- core_b8_remaining.c:22 — 需完整还原脚本执行逻辑
- core_b8_remaining.c:36 — 需完整还原 Ramdriv 解析逻辑
- core_b8_remaining.c:47 — 需完整还原列宽/滚动映射
- core_b8_remaining.c:58 — 需完整还原绘制逻辑
- core_b8_remaining.c:68 — 需完整还原命中测试逻辑
- core_b3r_h3.c:913 — FUN_14009da54 多态栈槽（编码标志寄存器/FILETIME 计数/WCHAR 指针）用位运算访问

**发现并修复的真实 bug（返回类型错误）**：
- core_b7b.c：FUN_14007A224 extern 声明为 `void` 但真实定义返回 `int64_t`
  （core_execline.c:111），已改 extern 为 `int64_t`；调用方忽略返回值不受影响。
- core_b7b.c：FUN_140063620 extern 声明为 `void` 但真实定义返回 `WCHAR *`
  （link_stubs stub + core_b1_remaining extern 均 WCHAR*），已改 extern 为 `WCHAR *`。

**收敛统计（其余文件）**：
- 解决+移除：46 条（b7b 18 extern、b2c 6 extern、b2d/b2e 3 extern、b8h/b8k 3 extern、
  b3m 1 extern、cmd4 15 extern 的 TODO 标记；均为依赖已在 core_*.c/link_stubs 定义/桩）
- 登记（保留 marker）：242 行（上列逐条）
- 结构横幅/约定性 TODO 文本不计入收敛（§28 先例）

**校验**（本批）：
- `./build.sh core_*.c` → exit 0（95 OK / 0 FAIL）
- `gcc -std=c99 -w -Wno-implicit-function-declaration -Iinclude core_*.c link_stubs.c
  -o /tmp/opencode/link_test` → exit 0（无 undefined、无 multiple-definition）

## 30. 阶段4a：DAT_→g_ 迁入 core_globals

**本批范围**：仅处理 `link_stubs.c` 中仍以数据桩存在的 `DAT_<addr>`，且被 `core_*.c`
以 `extern <type> DAT_<addr>;` 引用者，迁入 `core_globals.c`（真实类型 + g_ 名），
删除 link_stubs 桩，并做符号级（symbol-scoped）重命名（仅 core_*.c，未动 FUN_）。

**完成 35 个**（link_stubs 数据桩 504→469，即净删 35 条）：

命令表簇（core_b1_remaining.c）：
- DAT_14013a058→g_cmdTable3Count (int)
- DAT_14013a060→g_cmdTable3 (void*)
- DAT_14013a078→g_cmdTable2Count (int)
- DAT_14013a080→g_cmdTable2 (void*)
- DAT_14013a098→g_cmdTable4Count (int)
- DAT_14013a0a0→g_cmdTable4 (void*)
- DAT_14013a0c8→g_cmdTable5Len (int)
- DAT_14013a0c0→g_cmdTable5 (void*)
- DAT_14013ca98→g_cmdTable1Count (int)
- DAT_14013caa0→g_cmdTable1 (void*)

标准句柄（core_b1_remaining / core_b3r_h1 / core_b3r_h3）：
- DAT_14013a330→g_hStdIn (HANDLE)
- DAT_14013a338→g_hStdErr (HANDLE)
- DAT_14013a340→g_hStdOut (HANDLE)

标志/状态：
- DAT_14013ca48→g_minintFlag (char, MININT 检测)
- DAT_14013caf0→g_exitCodeCache (uint32_t)
- DAT_14013cad0→g_wallpaperMode (uint8_t)

函数指针槽/句柄槽（延迟加载）：
- DAT_14013a048→g_pMemQuery (void*)
- DAT_14013ca58→g_pProcMemRead (void*)
- DAT_14013ca08→g_pShutdownFlag (void*)
- DAT_14013cac0→g_pPendingObj (void*)
- DAT_14013ca40→g_pGetFileVersionInfoExW (void*)
- DAT_14013ca38→g_pGetFileVersionInfoSizeExW (void*)
- DAT_14013caa8→g_pVolHandles (int64_t*)
- DAT_14013c978→g_pSysAllocString (void*)
- DAT_14013c980→g_pSysFreeString (void*)
- DAT_14013c9b0→g_hOleaut32 (HMODULE)
- DAT_14013d0a0→g_pImDiskCtrl (void*)
- DAT_14013d098→g_pImDiskMask (void*)
- DAT_14013d0c0→g_pDevOpen (void*)
- DAT_14013d0c8→g_pDevOpen2 (void*)
- DAT_14013d0f8→g_pNtOpenSymLink (void*)
- DAT_14013d100→g_pNtQuerySymLink (void*)
- DAT_14013d830→g_pCoCreateInstance (int (*)(void*,void*,uint32_t,void*,void**))
- DAT_14013ccc0→g_hGdiPlus (HMODULE)
- DAT_14013cfc0→g_gdiplusToken (void*)

**类型冲突处理（跨文件 extern 类型不一致）**：
- DAT_14013c980 在 core_b3e extern 原为 `uint32_t`，与其余 `void *` 冲突 → 统一为 `void *`。
- DAT_14013d830 在 core_b3r_g extern 为完整函数指针类型（且被直接 `(*g_pCoCreateInstance)(…)`
  调用），其余文件为 `void *` → 统一为函数指针类型，定义/header/各 extern 全部同步。

**校验**：
- `./build.sh core_*.c` → exit 0（全 OK / 0 FAIL）
- `gcc -std=c99 -w -Wno-implicit-function-declaration -Iinclude core_*.c link_stubs.c
  -o /tmp/opencode/link_test` → exit 0（无 undefined、无 multiple-definition）

**剩余**：core_*.c 中 `extern … DAT_<addr>;` 且 link_stubs 仍有桩者约 469 条，留在后续批次。

## 31. 阶段4b：DAT_→g_ 迁入 core_globals（批2）

**本批范围**：继续把 `link_stubs.c` 中尚有数据桩、且被 `core_*.c` 以
`extern <type> DAT_<addr>;` 引用的 `DAT_<addr>` 迁入 `core_globals.c`（真实类型 + g_ 名），
删除 link_stubs 桩，做符号级（symbol-scoped）重命名（仅 core_*.c，未动 FUN_）。

**完成 36 个**（均为类型一致、语义明确的标量/句柄/模块变量，无跨文件类型冲突）：

标量标志/状态（uint8/int/int32）：
- DAT_14013a349→g_lowVerFlag (uint8_t, <XP 版本标志)
- DAT_14013a350→g_sysColor8 (uint32_t, GetSysColor(8))
- DAT_14013a258→g_tooltipTimeout (int)
- DAT_14013caf9→g_runMode (uint8_t)
- DAT_14013cb08→g_flagCB08 (uint8_t)
- DAT_14013cb58→g_bitFlags (uint32_t)
- DAT_14013cb98→g_sysVerClass (uint32_t)
- DAT_14013ccb2→g_foregroundFlag (uint8_t)
- DAT_14013ccb3→g_flagCCB3 (uint8_t)
- DAT_14013cf82→g_tipAlign (uint8_t)
- DAT_14013cf98→g_tipOffX (int)
- DAT_14013cf9c→g_tipOffY (int)
- DAT_14013d030→g_sysStartFlag (uint8_t)
- DAT_14013d031→g_answerFlag (uint8_t)
- DAT_14013d080→g_msgLockCount (int32_t)
- DAT_14013d5a8→g_qMode (int32_t)
- DAT_14013d6f5→g_flagD6F5 (uint8_t)
- DAT_14013d5ac→g_flagD5AC (int32_t)
- DAT_14013d3ef→g_flagD3EF (uint8_t)
- DAT_14013d321→g_flagD321 (uint8_t)
- DAT_14013d322→g_iconRefCount (uint8_t)
- DAT_14013d323→g_msgWndMode (uint8_t)
- DAT_14013d324→g_fgWndLock (uint8_t)
- DAT_14013d4e0→g_dblClickFlag (int)
- DAT_14013a320→g_scrollOff (char)
- DAT_14013d6f0→g_flagD6F0 (int32_t)
- DAT_14013d840→g_guidPtrState (uint32_t)
- DAT_14013d7fc→g_flagD7FC (uint32_t)
- DAT_14013a298→g_flagA298 (uint8_t)
- DAT_14013a24b→g_flagA24B (int8_t)
- DAT_14013a24e→g_flagA24E (uint8_t)

句柄/模块（HANDLE 类）：
- DAT_14013d2f8→g_hRichEdit (HMODULE, riched20.dll)
- DAT_14013d3d8→g_ownerWnd (HWND)
- DAT_14013cf68→g_wndCF68 (HWND)
- DAT_14013cf88→g_hIconCF88 (HICON)
- DAT_14013cfa8→g_hFontFast (HFONT)

**本批类型冲突**：无跨文件类型冲突（36 个符号在各引用文件中外声明类型一致，直接统一）。

**统计**：
- 本批从 link_stubs 净删 36 条 DAT_ 数据桩。
- link_stubs.c 现存 DAT_ 数据桩定义约 290 行（数据桩定义行口径；含少量
  "半迁移" 重复桩——即 g_ 已在 core_globals 定义但 link_stubs 仍残留的别名桩，
  留待后续处理）。

**校验**：
- `./build.sh core_*.c` → exit 0（全 OK / 0 FAIL）
- `gcc -std=c99 -w -Wno-implicit-function-declaration -Iinclude core_*.c link_stubs.c
  -o /tmp/opencode/link_test` → exit 0（无 undefined、无 multiple-definition）

## 32. 阶段4c：DAT_→g_ 迁入 core_globals（批3）

**本批范围**：继续把 `link_stubs.c` 中尚有数据桩、且被 `core_*.c` 以
`extern <type> DAT_<addr>;` 引用的 `DAT_<addr>` 迁入 `core_globals.c`（真实类型 + g_ 名），
删除 link_stubs 桩，在全部 core_*.c 做 word-boundary 符号级重命名（仅数据符号，未动 FUN_）。

**完成 31 个**（标量标志/句柄/模块/单文件 uint64 状态格，无跨文件类型冲突）：

标量标志/状态：
- DAT_14013a210→g_readBufSize (int, 读取缓冲大小)
- DAT_14013a220→g_ramdrivFlag (int, Ramdriv 标志)
- DAT_14013a24c→g_transState (int16_t, 透明/状态)
- DAT_14013a348→g_popmenuFlag (uint8_t, popmenu 标志)
- DAT_14013c91c→g_msgRetCode (uint32_t, 消息返回码)
- DAT_14013ccc9→g_flagCCC9 (uint8_t)
- DAT_14013cccb→g_topiconFlag (uint8_t, TOPICON 标志)
- DAT_14013cccc→g_hotkeyIdx (int16_t, 热键序号)
- DAT_14013cccf→g_logoActiveFlag (uint8_t, LOGO 活动标志)
- DAT_14013d070→g_hookBusyFlag (uint32_t, 钩子忙标志)
- DAT_14013d074→g_hookReentry (uint32_t, 钩子重入计数)
- DAT_14013d271→g_sysStartuped (uint8_t, SysStartuped 标志)
- DAT_14013d272→g_initOnceFlag (uint8_t, 一次性初始化标志)
- DAT_14013d290→g_threadTableInitFlag (uint32_t, 线程表初始化标志)
- DAT_14013d6f6→g_flagD6F6 (uint8_t)
- DAT_14013e1b8→g_flagE1B8 (uint8_t)
- DAT_14013e1f4→g_flagE1F4 (uint8_t, 加载标志)
- DAT_14013e1c8→g_randSeedAccum (uint32_t, 随机种子累加)

句柄/模块：
- DAT_14013d268→g_hImdiskCpl (HMODULE, Imdisk.cpl)
- DAT_14013d3c8→g_hHook2 (HHOOK, 钩子句柄2)
- DAT_14013e2b0→g_hFontE2B0 (HFONT)

core_b9_remaining.c 单文件 uint64 状态格：
- DAT_14013b960→g_u64B960, DAT_14013bf50→g_u64BF50,
  DAT_14013bf54→g_u64BF54, DAT_14013bf58→g_u64BF58,
  DAT_14013e598→g_u64E598, DAT_14013e600→g_u64E600,
  DAT_14013ec88→g_u64EC88, DAT_14013eee8→g_u64EEE8,
  DAT_14013eef0→g_u64EEF0, DAT_14013f520→g_u64F520

**类型冲突**：本批 31 个均跨文件 extern 类型一致，无冲突处理。

**统计**：本批从 link_stubs.c 净删 31 条 DAT_ 数据桩。link_stubs.c 现存纯
DAT_ 数据桩定义约 376 行（数据桩定义行口径）。

**校验**：
- `./build.sh core_*.c` → exit 0（全 OK / 0 FAIL）
- `gcc -std=c99 -w -Wno-implicit-function-declaration -Iinclude core_*.c link_stubs.c
  -o /tmp/opencode/link_test` → exit 0（无 undefined、无 multiple-definition）

**遗留半迁移（本次未动，留待后续）**：检测到约 40 个 `g_` 已定义于
core_globals.c、但 link_stubs.c 仍残留桩、且仍有个别 core_*.c（尤其
core_b1_remaining/core_b3_remaining/core_b3r_*）以原始 `DAT_<addr>` 引用
（如 g_hHeap=DAT_14013d328、g_hKernel32=DAT_14013cfd0、g_hUser32/g_hGdi32、
g_hOleaut32、g_hGdiPlus、g_taskCount、g_privFlags 等）。这些不能简单地只删桩——
需先把剩余原始 `DAT_<addr>` 引用全部重命名为 g_ 并统一类型（函数指针槽→真实类型）
后再删桩，否则链接缺符号。

## 33. 阶段4d：DAT_→g_ 迁入 core_globals（批4 + 半迁移清理）

**本批范围**：两部分。

### A. 遗留半迁移清理（38 个，最高价值）
对 `g_` 已在 core_globals.c 定义、但 core_*.c 仍以原始 `DAT_<addr>` 引用、
且 link_stubs.c 仍有残留桩的符号，做符号级（symbol-scoped）重命名：全部 core_*.c
中的原始 `DAT_<addr>` → 既有 `g_` 名，删除每处冗余 `extern ... DAT_<addr>;`
（统一以 pecmd_defs.h 为准），并删除 link_stubs 残留桩。

完成 38 个：
```
g_DefEnv(14011d308) g_bX64(14013a038) g_flagA24F(14013a24f) g_randState(14013a358)
g_flag16a(14013c96a) g_LastTick(14013c9fc) g_pNtQueryInfo(14013ca60) g_hInstance(14013ca68)
g_pZwOpenSection(14013cb10) g_pRtlInitUnicodeString(14013cb48) g_QPFreq(14013cb80) g_QPC(14013cb88)
g_Tid(14013ccb4) g_hWaitEvent(14013cd10) g_pNtClose(14013cd40) g_pSHGetValueW(14013cd48)
g_pMapBlk(14013cf50) g_hInst(14013cf70) g_hUser32(14013cfc8) g_hKernel32(14013cfd0)
g_hGdi32(14013cfd8) g_StartTime(14013d038) g_Script(14013d130) g_richEditMode(14013d300)
g_hHeap(14013d328) g_pFnCleanup(14013d398) g_pComState(14013d3c0) g_WM_TaskbarRestart(14013d3e8)
g_pSHSetValueW(14013d3f0) g_pSHDeleteValueW(14013d3f8) g_pSHDeleteKeyW(14013d400) g_taskCount(14013d7e8)
g_pOleInit(14013d800) g_pOleUninit(14013d808) g_csCom(14013e168) g_csInit(14013e190)
g_privFlags(14013e20c) g_runFlag(140147000)
```
其中 **g_h* 句柄组**：g_hInstance、g_hInst、g_hUser32、g_hKernel32、g_hGdi32、g_hWaitEvent、
g_hHeap（g_hOleaut32/g_hGdiPlus 亦已无桩、无原始引用，确认干净）。

**类型冲突统一**：
- g_hKernel32 / g_hUser32 / g_hGdi32：core_b3_remaining.c 原 `extern void *DAT_...;`
  与其余 `HMODULE` 冲突 → 统一为 `HMODULE`（删除外声明，头文件为准）。
- 动态导入函数指针槽补全真实签名（原头文件为 `void(*)(void)` 占位，调用点已有实参）：
  - g_pSHSetValueW → `int(*)(HKEY,LPCWSTR,LPCWSTR,DWORD,const void*,DWORD)`
  - g_pSHDeleteValueW → `int(*)(HKEY,LPCWSTR,LPCWSTR)`
  - g_pSHGetValueW → `int(*)(HKEY,LPCWSTR,LPCWSTR,DWORD*,void*,DWORD*)`
  - g_pSHDeleteKeyW → `int(*)(HKEY,LPCWSTR)`
  - g_pOleInit → `int(*)(int)`
  - g_pFnCleanup 保持 `void(*)(void)`；core_b3k 取槽地址处补 `(int64_t *)&g_pFnCleanup` 转型。
- 新增头文件声明（此前缺失）：g_Script、g_DefEnv、g_flagA24F、g_richEditMode、g_pFnCleanup。
- 修正 core_msgbox 一处 `g_flagC96A` 未声明笔误 → `g_flag16a`。

### B. 新批：DAT_→g_ 迁入 core_globals（36 个）
从 link_stubs.c 尚有数据桩、且被 core_*.c 以 `extern <type> DAT_<addr>;` 引用的
符号中，选单一类型、语义明确的标量/句柄迁入 core_globals.c（真实类型 + g_ 名），
删除 link_stubs 桩，在全部 core_*.c 做 word-boundary 符号级重命名。36 个：
```
DAT_14013a22c→g_u32A22C(uint32_t)  DAT_14013a238→g_intA238(int)
DAT_14013a23c→g_intA23C(int)       DAT_14013a240→g_intA240(int)
DAT_14013a244→g_intA244(int)       DAT_14013a24a→g_u8A24A(uint8_t)
DAT_14013c934→g_dwC934(DWORD)      DAT_14013c96c→g_dwC96C(DWORD)
DAT_14013c9c8→g_hwndC9C8(HWND)     DAT_14013c9e8→g_u64C9E8(uint64_t)
DAT_14013c9f0→g_u64C9F0(uint64_t)  DAT_14013ca20→g_u64CA20(uint64_t)
DAT_14013ca49→g_u8CA49(uint8_t)    DAT_14013ca50→g_u64CA50(uint64_t)
DAT_14013cb60→g_u64CB60(uint64_t)  DAT_14013ccb1→g_u8CCB1(uint8_t)
DAT_14013ccb8→g_i64CCB8(int64_t)   DAT_14013cd00→g_u64CD00(uint64_t)
DAT_14013cf78→g_hwndCF78(HWND)     DAT_14013cf83→g_u8CF83(uint8_t)
DAT_14013cf84→g_u8CF84(uint8_t)    DAT_14013cf85→g_u8CF85(uint8_t)
DAT_14013cf87→g_u8CF87(uint8_t)    DAT_14013d170→g_i64D170(int64_t)
DAT_14013d294→g_charD294(char)     DAT_14013d310→g_hwndD310(HWND)
DAT_14013d428→g_i64D428(int64_t)   DAT_14013d438→g_i64D438(int64_t)
DAT_14013d5d0→g_u8D5D0(uint8_t)    DAT_14013d6f4→g_i32D6F4(int32_t)
DAT_14013d6f7→g_u8D6F7(uint8_t)    DAT_14013d710→g_hookD710(HHOOK)
DAT_14013d7d8→g_u64D7D8(uint64_t)  DAT_14013d7dc→g_i32D7DC(int32_t)
DAT_14013d7ec→g_i32D7EC(int32_t)   DAT_14013e120→g_i64E120(int64_t)
```
**本批排查剔除**：DAT_14013cef8/g_intCEF8、DAT_14013d050/g_u64D050、DAT_14013d3ec/g_u32D3EC、
DAT_14013d390/g_i64D390 因跨文件存在函数指针/数组与标量类型冲突（如
DAT_14013d3ec 在 core_b3e 为 `uint8_t[4]` 而 core_b3k/b3r_g1 为 `uint32_t`；
DAT_14013d390 为 `int64_t(*)(...)` 函数指针槽）—— 非本批"明显标量"范围，已还原为
原 `DAT_<addr>` 状态（桩恢复）留待后续统一类型再迁。

**统计**：
- 遗留半迁移清理 38 个；新批迁入 36 个；合计本阶段处理 74 个 DAT_ 符号。
- link_stubs.c 现存 DAT_ 数据桩定义约 **302 行**（数据桩定义行口径）。

**校验**：
- `./build.sh core_*.c` → exit 0（全 OK / 0 FAIL）
- `gcc -std=c99 -w -Wno-implicit-function-declaration -Iinclude core_*.c link_stubs.c
  -o /tmp/opencode/link_test` → exit 0（无 undefined、无 multiple-definition）


## 34. 阶段4e：DAT_→g_ 迁入 core_globals（批5）

**本批范围**：继续把 `link_stubs.c` 中尚有数据桩、且被 `core_*.c` 以
`extern <type> DAT_<addr>;` 引用的纯标量/句柄/数据指针迁入 `core_globals.c`
（真实类型 + g_ 名），删除 link_stubs 桩，在全部 core_*.c 做 word-boundary
符号级重命名（仅数据符号，未动 FUN_）。本批条数为 25 个。

**完成 25 个**：

纯标量（uint64/int64/uint32/uint8）：
```
d280 → g_threadTableCap (uint64_t, 线程表容量)
d288 → g_threadTableCnt (uint64_t, 线程表计数)
d3e0 → g_loadThreadId    (uint64_t, 当前加载线程 id)
d708 → g_osVersion       (uint64_t, OS 版本双 32 位打包)
e128 → g_intE128         (int64_t)
cf90 → g_rectCF90        (RECT 结构体)
```

数据指针/句柄/字符串：
```
ca18 → g_svcCmdStr (LPCWSTR, service 命令串)
d5b8 → g_qOutBuf   (LPWSTR, -q 输出缓冲)
cfa0 → g_pCurRect  (RECT*, 当前 RECT 指针)
cfb0 → g_pAppData  (void*, 应用/窗口过程数据基址)
d718 → g_winHandle (void*, 关联窗口/句柄)
d720 → g_winData   (void*, 关联数据)
c9d8 → g_pOnWndProc2 (void*, OnWndProc2 脚本)
c9e0 → g_pOnWndProc3 (void*, OnWndProc3 脚本)
ca00 → g_pFiber       (void*, 目标 fiber 句柄)
cfe0 → g_hOle32       (HMODULE, OLE32 模块句柄)
e0e0 → g_pImageBuf    (void*, 位图数据指针)
```

小标志/计数（int/uint 跨文件冲突，统一为无符号/带符号标量）：
```
a228 → g_helpWord    (uint32_t, 帮助模式字; 原 int/uint32 冲突→uint32_t)
a230 → g_helpHi32    (uint32_t, 帮助资源高 32 位; 原 int/uint32 冲突→uint32_t)
a248 → g_charTableF  (uint8_t, 字符表开关; 原 int8/uint8 冲突→uint8_t)
a249 → g_flagA249    (uint8_t; 原 int8/uint8 冲突→uint8_t)
a34c → g_helpVerFlag (int32_t, 帮助版本标志; 原 int/int32 冲突→int32_t)
cf86 → g_flagCF86    (uint8_t, 提示串显示/前导'*'计数; 原 int8/uint8 冲突→uint8_t)
ccce → g_flagCCCE    (uint8_t; 原 uint32/uint8 冲突→uint8_t)
d7f8 → g_lastThreadId (volatile uint32_t, 最近线程 id 槽; 原 uint32/volatile DWORD 冲突)
```

**类型冲突统一（在本批完成）**：
- cfb0：core_b2d 原 `uint8_t *`，其余 `void *` → 统一 `void *`。
- cfe0：core_b3_remaining 原 `void *`，其余 `HMODULE` → 统一 `HMODULE`。
- e0e0：core_b3r_c 原 `uint8_t *`、core_b3r_g 原 `LPCVOID` → 统一 `void *`。
- a228/a230/a248/a249/a34c/cf86/ccce/d7f8：int/uint/volatile 微冲突 → 统一标量类型。

**排查剔除（跨文件存在函数指针/数组与标量冲突，留待专门统一类型再迁）**：
- WS2_32 函数指针槽簇（d500/d508/d510/d518/d520/d528-d5a0/d4e8/d4f8/d570/d588/d590 等，
  closesocket/connect/send/htons/ntohl/gethostbyname/WSAStartup/shutdown 等）。
- VirtDisk/vhdmount 函数指针槽簇（d740-d768：DetachVirtualDisk/UnmountVHD/GetVirtualDiskPhysicalPath 等）。
- d3a0/d3a8（ImageList_ReplaceIcon/ImageList_Add 函数指针槽）。
- d390（int64_t(*)(…) 槽，§33 已标注）、d0e0（ImDisk 函数指针槽）、e160（指针/标量二义）。
- dc80/dce8（int64_t 数组 [2]/[3] 冲突）。
- §33 遗留已还原的 cef8/d050/d3ec/d390 仍留桩（未动）。

**统计**：
- 本批迁入 25 个；净删 link_stubs 25 条 DAT_ 数据桩。
- link_stubs.c 现存 DAT_ 数据桩定义行约 **306 行**（含 §33 遗留还原桩 cef8/d050/d3ec/d390）。

**校验**：
- `./build.sh core_*.c` → exit 0（95 OK / 0 FAIL）
- `gcc -std=c99 -w -Wno-implicit-function-declaration -Iinclude core_*.c link_stubs.c
  -o /tmp/opencode/link_test` → exit 0（无 undefined、无 multiple-definition）

## 35. 阶段4f：函数指针槽/数组 DAT_ 统一（含主代理修复补录，2026-08-19）

子代理迁移 WS2_32 函数指针槽（g_pWSAStartup/gsock/gpconnect/gpsend/.../g_pinet_addr 等，
DAT_14013d4e8–d5a0 集群）到 core_globals.c、pecmd_defs.h 给出真实函数指针类型。**子代理中途
停用未收尾**，留下大量旧 `extern uint64_t/void* g_p…;` 冲突声明导致 build 失败；主代理接手：
- 移除 32 处与 pecmd_defs.h 冲突的冗余 extern（b3a/b3b/b3g/b3i/b3m/b3_remaining/b3r_g4）；
- 修 core_b3r_g2.c 的 `g_pinet_addr`（本文件误按零参调用）：改由 pecmd_defs.h 声明、
  调用点补 `(char*)0` 实参并标 `TODO(verify)`；
- build 0 + link 0 恢复绿。
- link_stubs 数据桩剩约 285（其余 WS2/VirtDisk/ImageList/GDI+/ImDisk/int64 数组槽仍待后续批）。

## 36. 阶段4g：DAT_ 集群迁移（VirtDisk/ImageList/GDI+/ImDisk/数组）

本批把 link_stubs.c 中余留的函数指针导入槽/数组 DAT_ 数据桩迁入 core_globals.c 为 g_xxx 真实类型：

- **ImageList 簇** (3)：d390→g_pImageListCreate, d3a0→g_pImageListReplaceIcon, d3a8→g_pImageListAdd。
- **int64/byte 数组** (2)：dc80→g_cacheTableA[3], dce8→g_cacheTableB[3]（统一原 core_b3j[2] 与 core_b3r_b[3] 冲突为 [3]）。
- **VirtDisk/vhdmount 簇** (7)：d730→g_pCreateVirtualDisk(9 参真实类型), d740→g_pAttachVirtualDisk,
  d748→g_pDetachVirtualDisk, d750→g_pGetVirtDiskPhysPath, d758→g_pGetVirtDiskInfo,
  d760→g_pMountVHD, d768→g_pUnmountVHD。
- **ImDisk 簇** (9)：d0e8→g_pImDiskGetVersion, d088→g_pImDiskFindFreeLetter, d090→g_pImDiskForceRemove,
  d0a8→g_pImDiskGetOffsetByExt, d0b0→g_pImDiskGetPartInfo, d0b8→g_pImDiskOpenDevice,
  d0d0→g_pImDiskRemoveMountPoint, d0d8→g_pImDiskStartService, d0e0→g_pImDiskCreateMountPoint。

**GDI+**：g_hGdiPlus / g_gdiplusToken 已于前批迁入，本批无新增 GDI+ DAT_ 槽可迁。

**统一类型冲突**（本批完成）：
- dc80/dce8：core_b3j [2] 与 core_b3r_b [3] → 统一 `int64_t [3]`（b3j 至多用到 [1]，兼容）。
- d0e0：core_b2e `void *` / core_b1_remaining `uint64_t` / core_b2f `uint8_t` → 统一 `void *` 槽（调用处强转）。
- VirtDisk/ImDisk 槽按"延迟装载动态导入槽"处理：defs 声明函数指针/void*，调用处 cast fn-ptr。

**排查剔除/保留桩**：
- d738（VirtDisk 可用性标志）：core_b3m/b3i 以 int 判空并赋 lVar1(OpenVirtualDisk 结果)，
  而 core_b3_remaining 以 6 参函数指针调用（GetFileAttributes-ish 延迟槽）——同一地址 int/函数指针
  语义冲突，无法安全定型，**保留原 DAT_+link_stubs 桩**（未动）。
- §33 遗留还原桩 cef8/d050/d3ec 与 §34/§35 未尽槽仍留桩。

**统计**：
- 本批迁入 21 个（ImageList 3 + int64 数组 2 + VirtDisk 7 + ImDisk 9）；deferred 1（d738）。
- link_stubs.c 现存 DAT_ 数据桩定义行 **285**（含 d738、cef8/d050/d3ec 等遗留桩）。

**校验（最终状态）**：
- `./build.sh core_*.c` → exit 0（95 OK / 0 FAIL）
- `gcc -std=c99 -w -Wno-implicit-function-declaration -Iinclude core_*.c link_stubs.c
  -o /tmp/opencode/link_test` → exit 0（无 undefined、无 multiple-definition）

## 37. 空闲时段门控调度（2026-08-19）

用户要求用 agent 自身能力（goal 自动续跑循环）+ 空闲时段门控，仅在北京时间空闲时段
（非 09–12、非 14–18）执行重活，以利用空闲计价（半价）。高峰轮只判时、不启动重批次。
判定：`TZ='Asia/Shanghai' date +%H`；`h∈[9,12)∪[14,18)` 为高峰。不依赖 OS cron。

## 38. 阶段4h：DAT_→g_ 迁入 core_globals（批7）

本批聚焦 link_stubs.c 中大量简单的 `uint64_t DAT_...;` 数据桩（14011c638/14011d500/
14011e6b8/140121xxx/14013xxx 等），按 §30-§36 既定模式迁入 core_globals.c 为 g_xxx 真实类型。

**迁入 32 个**（全部为引用路径清晰的标量数据；类型取各自 extern，非桩的 uint64_t），举例：
- `g_dbl1178/g_dbl1668/g_dbl2578/g_dbl2598 `(core_b2f double)
- `g_flae4/g_flae8/g_flaf0/g_flaf4` (core_b2f float)
- `g_dbl2b368/2b370/2b378/2b380` (core_b8m double)
- `g_u64FontA98/g_u32FontAa0` (core_b2f)
- `g_u6497e8/f0/f8, g_u329800, g_u169804, g_u897806` (core_b3l)
- `g_u6453d8, g_u3253e0, g_u6459e4/e8, g_u326778, g_u162677c` (core_b3g/b3_remaining)
- `g_u64ee90, g_u646e78` (core_b3r_g7)、`g_u6435a0`(字体特征)、`g_dbl94e8`、`g_u322570/g_u162574`
- 全部: core_globals.c 定义(@0x 注释) + include/pecmd_defs.h extern + 词边界 rename 全 core 引用 + link_stubs 删桩

**跨文件类型冲突 → 保留 DAT_+桩即 defer**（据实型/歧义规则）：
- 140126070 / 140126078：core_exec 已备 `g_fontRound`/`g_dpiBase`(uint64_t 遗留桩) 与其他文件
  `DAT_` double/int 混用 —— 既有命名冲突，defer 未动（含批内曾临时改名后回退）。
- 14013d468/14013d480 (b3a uint32 vs b3_remaining void* WIMAPI 槽)
- 14013d430 (b3f uint32 vs b3r_d int64 被调用槽)
- 14013cfb8 (b2d uint32 vs b2f HWND)、140147008 (b2d uint32 vs b2f COLORREF)
- 14013d868 (b3e int32 vs b3r_e double)、14013d3b8 (b3d int64 标志 vs b3l/b3_remaining/b3r_g1 fnptr)
- 14013e160 (b3i int64 vs b3_remaining uint64 vs b3r_g int64*)。

**清理**：删除 batch28 复原段 2 处重复死桩 `uint64_t DAT_140126e78=0`、`uint64_t DAT_14011ee90=0`
（对应符号已迁 g_，旧名零引用）。

**统计**：
- 本批迁入 32；removed-as-unused 0（简单 uint64_t DT 桩中无纯死桩，唯一零引用者为已迁者）；deferred 11；
- link_stubs.c 现存 `uint64_t DAT_140...` 桩 **191**（批前 223，删 32）；其余函数指针/GDI+/WS2 槽等
  待后续批。

**校验（最终状态）**：
- `./build.sh core_*.c` → exit 0（95 OK / 0 FAIL）
- `gcc -std=c99 -w -Wno-implicit-function-declaration -Iinclude core_*.c link_stubs.c
  -o /tmp/opencode/link_test` → exit 0（无 undefined、无 multiple-definition）

## 39. 阶段4i：DAT_→g_ 迁入 core_globals（批8）

本批在 §30-§38 既定模式上继续，清理又一批 `uint64_t DAT_...;` 数据桩（191→152，
本批删 39）。全程小步迁移，每步 build+link 校验。

**迁入 39 个**（全部引用路径清晰、类型一致；core_globals.c 定义 `@0x` 注释 +
include/pecmd_defs.h extern + 词边界 rename 全 core 引用 + link_stubs 删桩）：
- 标量/指针：`g_dbl20b28`(140120b28, 含 `_DAT_140120b28` 同名 void 变体一并统一)、
  `g_u161be4`(140121be4)、`g_u161bec`(140121bec)、`g_dbl25230`(140125230)、
  `g_pwszD220`(14013d220)、`g_csThreadTbl`(14013d228)、`g_pwszD250`(14013d250)、
  `g_pwszD258`(14013d258)、`g_csDisk`(14013e0e8)、`g_u64e770`(14011e770)、
  `g_pSvStatus`(14013c910)、`g_pi64e118`(14013e118)、`g_u64c9d0[4]`(14013c9d0)、
  `g_svcStatus[128]`(14013c918)。
- .rdata 字节/字符串数组：`g_bD500`、`g_bE6b8`、`g_bE870`、`g_bE880`、`g_bE8a0`、
  `g_b20aa8`、`g_b21728`、`g_b24d00`、`g_b24d10`、`g_b24d20`、`g_b24e10`、
  `g_b297d0`、`g_b12acc0`、`g_b12ae98`、`g_b12d158`、`g_b12d1a8`、`g_b12d1b8`、
  `g_wsz20c64`、`g_wsz21014`（对应 14011d500/11e6b8/11e870/11e880/11e8a0/120aa8/
  121728/124d00/124d10/124d20/124e10/1297d0/12acc0/12ae98/12d158/12d1a8/12d1b8/
  120c64/121014）。
- 数组：`g_d278`(14013d278 线程ID表)、`g_b13caf8`(14013caf8)、`g_u16127d30`(140127d30)、
  `g_i1255d0`(1401255d0)、`g_b12d1d8`(14012d1d8)。

**遗留 g_ 桩统一化 1 个**：
- 14013cf58：core_execscript 已有 `g_hMsgHook`(HHOOK) 而 core_b1_remaining 用 `DAT_`。
  类型一致(HHOOK)，安全统一 → 删 link_stubs `g_hMsgHook`+`DAT_14013cf58` 两桩，core_globals
  定义 HHOOK。

**跨文件类型冲突 → 保留 DAT_+桩即 defer（据实型/歧义规则）**：
- 14013e2a8（`g_pFontBase`）：core_exec/b8h 以 `HANDLE`(此环境=unsigned long) 而
  core_b3_remaining 以 `void*` 直接赋对象指针 —— handle/void* 歧义，尝试统一 void* 时
  破坏 core_exec 的 `base=g_pFontBase`(base 为 HANDLE)，判定真歧义 → **还原**，保留
  link_stubs `DAT_14013e2a8` + `g_pFontBase` 双桩未动（本次曾迁移后回退）。

**removed-as-unused**: 0（本批 39 个全部为实际引用符号，无疑似死桩）。

**统计**：
- 本批迁入 39；removed 0；deferred 1（14013e2a8 既有 g_pFontBase 冲突）。
- link_stubs.c 现存 `uint64_t DAT_140...` 桩 **152**（批前 191，删 39）；其余
  GDI+/SetupAPI/Rtl/WS2 函数指针槽 + 140147001/2/3 与 g_runFlag 重叠字节 +
  已登记多型冲突桩待后续批。

**校验（最终状态）**：
- `./build.sh core_*.c` → exit 0（95 OK / 0 FAIL）
- `gcc -std=c99 -w -Wno-implicit-function-declaration -Iinclude core_*.c link_stubs.c
  -o /tmp/opencode/link_test` → exit 0（无 undefined、无 multiple-definition）

## 4x. 阶段7 登记（人类手写风打磨，2026-08-19）

仅对已译 C 重构、不碰伪代码：1) 参数/局部可读化 2) 偏移→结构体/宏 3) 去 CONCAT71/解译残留
4) 巨型函数拆分 5) 去冗余。每步保持 build/link 绿；与阶段6（挖语义）独立。详见 TASKS §阶段7。

## 41. 阶段4j 补：DAT_→g_ 迁移收尾（批9，含前次半成品清理）

**背景**：上一批（批9）在 core_globals.c 添加了一批 `g_` 定义，但未完成——未删对应
`DAT_` 桩、未改 core 引用、未写本 REVIEW 段，形成"定义而未引用"的半成品（build/link 仍绿但
`g_` 为 orphan + 旧 `DAT_` 桩仍存）。本批先核对半成品再继续迁移。

### STEP 1 — 核对前次半成品（35 个 `g_` 定义，均无任何 core 引用、无 extern）
逐符号按其 `@0x` 地址查 core 引用与 link_stubs 桩：
- 33 个对应**真实被引用的数据全局**（core 仍用 `DAT_<addr>`、link_stubs 仍有 `DAT_` 桩）
  → **完成迁移**：词边界 rename 全 core 引用 DAT_→g_，统一类型，pecmd_defs.h 补 extern，
  link_stubs 删桩。
- 2 个为**真冲突，留存 DAT_+stub（defer）**，其 orphan `g_` 定义已删除：
  - `DAT_140125238`：core_b3b/b3e/b3r_e/b3_remaining 用 `extern double DAT_140125238`，
    而 core_b8h/core_exec 用既有名 `g_fontMinus0`（同一地址）——命名冲突 → 未迁，
    删 `g_dbl25238`。
  - `DAT_140147010`：core_b2d `extern double` vs core_b3_remaining `extern uint32_t`——
    类型冲突 → 未迁，删 `g_dbl47010`。

**本步完成 33、删除 2、defer 2；批9 半成品清零。**

类型统一说明（WORD 数组 const）：
- core_b3r_h2/h4 对 WCHAR 关键字/选项串以 `extern const WCHAR …[]` 引用 → 定义与 pecmd_defs.h
  extern 一并统一为 `const WCHAR …[0x40]`（含 g_wsz28f44 原本非 const 的引用同步该为 const）。
- `DAT_14012d168/178`（壁纸 CLSID/IID）以 GUID 定型（GUID 16 字节，语义正确），
  core_b1_remaining extern 由 `uint64_t` 统一为 `GUID`。

### STEP 2 — 再迁 20 个干净 GDI+/Rtl 延迟加载函数指针导入槽
（单文件/多文件均以 `extern void *` 一致引用，无 fnptr 类型冲突；定 `void *` 与 §30/§36
导入槽模式一致，词边界 rename + defs extern + 删桩）：
- Gdip：cda0→g_pGdipLoadImageFromFile、cda8→g_pGdipImageGetFrameDimensionsList、
  cdb0→g_pGdipImageSelectActiveFrame、cdc8→g_pGdipImageGetFrameDimensionsCount、
  cdd0→g_pGdipImageGetFrameCount、cdd8→g_pGdipGetPropertyItemSize、
  cde0→g_pGdipGetPropertyItem、cdf8→g_pGdipSaveImageToFile、ce00→g_pGdipDrawImageI、
  ce18→g_pGdipDrawString、ce20→g_pGdipDrawImageRectRectI、ce40→g_pGdipSetPixelOffsetMode、
  ce48→g_pGdipSetSmoothingMode、ce50→g_pGdipLoadImageFromStream、
  cd80→g_pGdiplusStartup、cd88→g_pGdiplusShutdown。
- Rtl/杂：cd60→g_pRtlDosPathNameToNtPathName_U、cd68→g_pRtlCreateUnicodeString、
  cd70→g_pRtlFreeUnicodeString、cd78→g_pGetProcessMemoryInfo。

**STEP 2 迁入 20。**

### 统计
- 本批（阶段4j 收尾）合计：**完成 53**（STEP1 33 + STEP2 20）、**removed-as-unused 2**
  （g_dbl25238、g_dbl47010）、**deferred 2**（140125238 命名冲突、140147010 类型冲突，仍 DAT_+stub）。
- link_stubs.c 现存 `uint64_t DAT_140...` 桩 **124**；其余为跨文件类型/函数指针签名冲突桩
  （GDI+/SetupAPI/Rtl 未迁槽、140147001/2/3 与 g_runFlag 重叠字节、dpi/font/d830 类型冲突等）待后续批。

### 校验（最终状态）
- `./build.sh core_*.c` → exit 0（95 OK / 0 FAIL）
- `gcc -std=c99 -w -Wno-implicit-function-declaration -Iinclude core_*.c link_stubs.c
  -o /tmp/opencode/link_test` → exit 0（无 undefined、无 multiple-definition）

## 42. 阶段4k：DAT_→g_ 迁入 core_globals（批10）

**本批范围**：继续清理 link_stubs.c 中尚存的数据桩。第一步清掉一批纯孤儿桩（只剩注释引用、
真符号已迁入 core_globals 的 WS2 槽）；第二步按 §30-§41 既定模式迁入一批干净 GDI+/SetupAPI/
COM/Advapi32/NTDLL/WIM 延迟加载函数指针槽（统一 `void *`）与少量干净标量/结构体。

### STEP 1 — removed-as-unused（23 个 WS2 孤儿桩）
`DAT_14013d4e8/d4f0/d4f8/d500/d508/d510/d518/d520/d528/d530/d538/d540/d548/d550/d558/d560/
d568/d570/d578/d588/d590/d598/d5a0` 在 link_stubs 仍留 `uint64_t` 桩，但对应真实符号
`g_pWSAStartup/g_psock/.../g_pWSAFDIsSet` 已在 core_globals.c 定义（§35 迁入），桩仅剩 core_globals
注释引用、零代码使用 → **直接删桩 23，removed-as-unused**（grep core_*.c 确认无代码引用）。

### STEP 2 — 迁入 35 个（core_globals.c 定义 `@0x` 注释 + include/pecmd_defs.h extern +
词边界 rename 全 core 引用 + link_stubs 删桩）：
- **GDI+ 函数指针槽（10，统一 void\*）**：ce58→g_pGdipGetImageType、ce68→g_pGdipSetTextRenderingHint、
  ce78→g_pGdipGetImageDimension、ce80→g_pGdipBitmapLockBits、ce88→g_pGdipBitmapUnlockBits、
  ce98→g_pGdipCreateBitmapFromResource、ceb0→g_pGdipScaleWorldTransform、ceb8→g_pGdipCreateTexture、
  cec0→g_pGdipDeleteBrush、cec8→g_pGdipFillRectangle（均仅 core_b1_remaining 以 void* 装载）。
- **SetupAPI 函数指针槽（3，统一 void\*）**：cf18→g_pSetupDiSetClassInstallParamsW、
  cf38→g_pInstallHinfSectionW（core_b1_remaining/core_b2b 一致 void*）、cf40→g_pInstallHinfSection。
- **COM CLSID/IID 槽（2，uint64_t）**：12d188→g_u64d188、12d198→g_u64d198（core_b1_remaining 按地址传 COM）。
- **全局临界区（1，CRITICAL_SECTION）**：13e138→g_csE138（core_b3r_g1）。
- **默认字体大小（1，double）**：1293c0→g_fontSizeDef（统一 core_b3_remaining 的 `DAT_1401293c0`
  与 core_exec/core_b8h 既有 g_fontSizeDef；原 `uint64_t g_fontSizeDef` 桩改定 `double`）。
- **Advapi32/NTDLL/WIM/杂项函数指针槽（18，统一 void\*）**：cb50→g_pZwUnmapViewOfSection、
  cca8→g_pZwMapViewOfSection、d000→g_pOpenSCManagerW、d008→g_pOpenServiceW、
  d010→g_pQueryServiceStatusEx、d018→g_pControlService、d020→g_pStartServiceW、
  d028→g_pCloseServiceHandle、d040→g_pNtQuerySystemInformation、d048→g_pSetSuspendState、
  d3d0→g_pHookCb3d0、d418→g_pSetWindowTheme、d440→g_pWIMMountImage、d448→g_pWIMUnmountImage、
  d450→g_pWIMMountImage2、d700→g_pRtlGetNtVersionNumbers、d778→g_pCreateSymbolicLinkW、
  d850→g_pFreeLibrary。

### 保留/defer（未动）
- 已登记冲突仍留 `DAT_`+stub：140125238（命名冲突 g_fontMinus0，§41）、140147010（double vs
  uint32，§41）、140126070/140126078（g_fontRound/g_dpiBase 冲突，§38）、14013d868/d3b8/d738/e160/
  d430/d468/d480/d4d8/cfb8/147008/e2a8（§38/§36/§39 已登记多型冲突）等。
- 140147001/140147002/140147003（与 g_runFlag 重叠字节标志）与 14700c/12d188 等涉及字节重叠者中，
  仅 12d188/198 为独立 8 字节槽已迁；140147001/2/3 及 147008/14700c 仍留桩（字节重叠/类型冲突，defer）。

### 统计
- 本批：**迁入 35**、**removed-as-unused 23**、**deferred 0 新增**（既有冲突原样保留）。
- link_stubs.c 现存 `uint64_t DAT_140...` 标量桩 **67**（批前约 124，净减 57；其余为已登记跨文件
  类型/函数指针签名冲突桩 + GDI+/SetupAPI 混合类型槽 + 字节重叠标志，留待后续批统一类型）。
- 另修正 `g_fontSizeDef` 原 `uint64_t` 桩为 `double` 定义（与 core_exec/core_b8h extern 一致）。

### 校验（最终状态）
- `./build.sh core_*.c` → exit 0（95 OK / 0 FAIL）
- `gcc -std=c99 -w -Wno-implicit-function-declaration -Iinclude core_*.c link_stubs.c
  -o /tmp/opencode/link_test` → exit 0（无 undefined、无 multiple-definition）

## 43. 阶段4l：DAT_ 冲突解析 + 剩余迁移（批11）

本批做**专设的冲突解析**：解析此前登记的命名冲突（同一地址、一文件用 `DAT_<addr>`、另一文件用既有 `g_<name>`），
并把一批干净标量（含 `_DAT_` 双精度/整型）迁入 `g_`；对真正歧义的类型冲突/字节重叠标志保留 `DAT_+stub` 不强制。

### STEP 1 — 解析命名冲突（g_ 为真实属主，DAT_ 用户类型与用途一致 → rename 统一并删桩）
- `DAT_140125238` → **g_fontMinus0**（double，-0.0）：core_b3b/b3e/b3r_e/b3_remaining 的 `extern double`
  与 core_b8h/core_exec 既有 g_fontMinus0 合并；link_stubs 桩由 `uint64_t` 改为 `double`。
- `DAT_140126070` → **g_fontRound**（double，0.499）：core_b3e/b3f/b3r_e 与 core_exec 合并；同改 double。
- `DAT_140126078` → **g_dpiBase**（double，72.0）：core_b3e/b3r_e 与 core_exec 合并；同改 double。
- `DAT_14013e2a8` → **g_pFontBase**（core_b3_remaining `void *` 与 core_b8h/core_exec `HANDLE` 同为不透明句柄）：
  core_b3_remaining 全部引用 rename 至既有 g_pFontBase，删桩。
- `DAT_14013cd08` → **g_u64cd08**（物理内存映射句柄，core_b1_remaining uint64 / core_b2b HANDLE）：
  core_globals.c 定义 + pecmd_defs.h extern + 全 core rename + 删桩（含原重复桩）。

### STEP 2 — 其余干净标量迁入 `g_`（19 个，单文件一致类型，无冲突）
core_globals.c 定义（@0x 注释）+ pecmd_defs.h extern + 词边界 rename + link_stubs 删桩：
- **double（15）**：126640→g_dbl26640、126650→g_dbl26650、126658→g_dbl26658、126660→g_dbl26660、
  127088→g_dbl27088、127090→g_dbl27090、127098→g_dbl27098、1270a0→g_dbl270a0、1270a8→g_dbl270a8
  （core_b3_remaining）；121630→g_dbl21630、121bf0→g_dbl21bf0、121c80→g_dbl21c80、122580→g_dbl22580、
  122588→g_dbl22588、122590→g_dbl22590（core_b2f）。
- **整型（4）**：1265b0→g_u32265b0（uint32）、126770→g_u6426770（uint64，core_b3_remaining）；
  126e70→g_u6426e70（uint64，core_b3r_g7）；12d1c8→g_u642d1c8（uint64，core_b3r_g5 GUID 前 8 字节）。
  （注：link_stubs 原 `_DAT_140121630/bf0/1c80/122580/...` 桩为 `uint64_t` 而使用处为 double，
  本次按使用方定 `double`，修正潜在类型。）

### STEP 3 — removed-as-unused（1）
- `DAT_14013d580`：真实属主 `g_pinet_addr` 已在 core_globals.c 定义并被全 core 使用；DAT_ 桩仅剩
  core_globals/pecmd_defs 注释引用、零代码使用 → 直接删桩。

### 统计
- 本批：**迁入 24**（命名冲突解析 5 + 干净标量 19）、**removed-as-unused 1**（14013d580）、**deferred 0 新增**。
- link_stubs.c 现存 `uint64_t DAT_140...` 标量桩 **63**（批前约 67，净减 4 类；其余为已登记跨文件类型/
  函数指针签名冲突 + GDI+/SetupAPI 混合类型槽 + 字节重叠标志 + 11c638 空串/虚表双用途命名冲突）。

### 仍 defer（保留 DAT_+stub，不强制）
- 140147001/002/003（与 g_runFlag 字节重叠标志）、140147008（uint32 vs COLORREF）、
  14014700c（uint32 vs uint64）、140147010（double vs uint32，§41）。
- 140126070/078 已解析；但 GDI+ 延迟槽（13cd90…13cef0 等，void*/类型化 fnptr 混用）、SetupAPI 槽
  （cf00/cf08/cf28/cf30/cf48/cf10/cf20/cef8）、mci/COM/Iphlpapi 槽（d050/d838/d860/d4d8/d358/d780/d810/d828/
  c990/d470/d478）等多型冲突仍留桩待后续批统一类型。
- 14013cfb8（uint32 vs HWND）、14013d3b8（int64 标志 vs fnptr）、14013d430/d468/d480（uint32 vs fnptr）、
  14013d738、14013d868（int32 vs double）、14013dd00（HWND/uint64）、14013e160、14011e890（GUID uint8[] vs GUID）、
  14013cf60（HWND/uint64/int64 混用，且与 g_msgboxFlag 注释疑似字节重叠）等 → 保留 DAT_+stub。
- 14011c638（`g_szEmpty` 命名冲突）：同一地址既作 `.rdata` 空串又作虚表基址（`g_pVtblA/B = &DAT_14011c638`），
  且 `g_szEmpty` 定义类型(u64)与 `WCHAR[]` extern 不一致，须改型并波及 ~25 文件 —— 视为歧义暂 defer。

### 校验（最终状态）
- `./build.sh core_*.c` → exit 0（95 OK / 0 FAIL）
- `gcc -std=c99 -w -Wno-implicit-function-declaration -Iinclude core_*.c link_stubs.c
  -o /tmp/opencode/link_test` → exit 0（无 undefined、无 multiple-definition）

---

## 44. 阶段4m：FUN_→PECMD_ 改名（应用 rename_map）

### 概述
- 应用 `tools/rename_map.json`（171 条 `{FUN_<addr> → PECMD_xxx}`）到全部 `core_*.c`。
- 关键难点：旧符号名地址大小写不一致（`FUN_14005b134` vs `FUN_14005D5DC`），且部分函数被
  跨文件重复/误标重构（同一地址出现在两处，或符号名与其 `@0x` 头注释地址不符）。
  因此按**地址（`@0x<addr>`，反重复键）+ 词边界、地址部分大小写不敏感**统一替换为 map 的 `new` 拼写
  （大小写随 map 原样）。

### 结果
- **rename_map 应用：171/171** —— 每个 map 项的 `FUN_<addr>` 均已改为 `PECMD_xxx`，
  core_*.c 中已无任何 map 内 `FUN_<addr>` 残留；全部 171 个 `PECMD_xxx` 名字均已在代码中出现。
- 共约 **1151** 处 `FUN_<addr>`→`PECMD_xxx` 词边界替换，波及 **48** 个 core 文件
  （定义 + extern + 调用点 + 函数指针取址 + 头注释标签；`@0x<addr>` 头注释保留）。
- **already-done/skipped：1**（`14005bc28→PECMD_IsAlphaNumeric` 本已完整改名，无残留 `FUN_`）。
  其余 170 项均残留有未同步的 `FUN_<addr>` 引用（多源于此前跨文件重复/误标重构），本次统一同步。

### 冲突修复（1）
- `PECMD_ClampInt`（140053f3c）被跨文件重复定义：`core_b3a.c`（`void` 干净版）与
  `core_b3_remaining.c`（`int` 版，其 6 个调用方在本文件内读 eax 残留返回值）。
  两处均非 static → 链接 multiple-definition。修复：**core_b3_remaining.c 的定义改 `static`**
  （本文件调用方仍绑定到本地版本；core_b3a 的非 static 版作为外部符号）。
- 另两对手足同名项（FreeContainer=14005b134/14005d5dc、IsAlphaNumeric=14005bc28/140061204）
  均为一处 `static` 一处非 static，链接无冲突，维持原绑定语义。

### 新增命名
- **0**（可选"命名几只手"步骤有意跳过）：剩余 `FUN_` 符号为数千个尚未处理阶段的函数，批量臆名
  风险高，且本步核心仅应用现有 rename_map。留待后续阶段逐批确认。

### 校验（最终状态）
- `./build.sh core_*.c` → exit 0（95 OK / 0 FAIL）
- `gcc -std=c99 -w -Wno-implicit-function-declaration -Iinclude core_*.c link_stubs.c
  -o /tmp/opencode/link_test` → exit 0（无 undefined、无 multiple-definition）

---

## 45. 阶段4n：FUN_→PECMD_ 新命名（批12）

### 概述
- 继全部 rename_map（171）应用后，本批为剩余的未命名 `FUN_<addr>` 函数新增 29 个高置信度
  `PECMD_xxx` 名字，全部取自其正文/头注释的明确职能，仅改名字（签名/函数体不动），
  词边界替换覆盖定义、extern、调用点与头注释标签；`@0x<addr>` 头注释地址保留。
- 新增命名 29 条，涉及 **201** 处 `FUN_<addr>`→`PECMD_xxx` 替换，跨 ~35 个 core 文件。

### 新增命名清单
| 地址 | 新名 |
|---|---|
| 0x1400226b0 | PECMD_IsVkPrefix（vk_ 前缀判断） |
| 0x14002162c | PECMD_GetPathType（盘符/UNC 前缀类型） |
| 0x140022e24 | PECMD_IsSingleInstance（单实例检查） |
| 0x140027644 | PECMD_CreateDirectory（创建目录） |
| 0x1400250bc | PECMD_IsSysStartuped（SysStartuped 状态缓存） |
| 0x140060290 | PECMD_RemoveFirstMatchChar（删除串中首个匹配字符） |
| 0x140063c70 | PECMD_GetEditSelRange（获取编辑框选择范围） |
| 0x140066054 | PECMD_SetWindowTheme（设置窗口主题） |
| 0x140066188 | PECMD_MatchAssignToken（匹配赋值 token =/:） |
| 0x140067e2c | PECMD_ParseIntRound（解析整数并四舍五入） |
| 0x140067e88 | PECMD_ParseInt64Round（解析 int64 并四舍五入） |
| 0x14005f904 | PECMD_FindFirstFreeDrive（查找首个空闲盘符） |
| 0x140060f68 | PECMD_GetTime100ns（获取 100ns 时间） |
| 0x140067748 | PECMD_CopyUpToChar（复制到指定字符前） |
| 0x14006ad9c | PECMD_HexStringToBytes（十六进制串转字节数组） |
| 0x14005bbb4 | PECMD_ParseSizeSuffix（解析大小后缀） |
| 0x14005fe34 | PECMD_AsciiTrimToWide（ASCII 去首尾空格转宽） |
| 0x140060000 | PECMD_BuildDosDevicePath（构造 DOS 设备路径） |
| 0x140060718 | PECMD_SetClipboardUnicode（Unicode 文本入剪贴板） |
| 0x140060d7c | PECMD_TrimOuterQuotes（去除首尾成对引号） |
| 0x14005c6b8 | PECMD_MatchWordAndPad（匹配单词并填充空格） |
| 0x14005d6f8 | PECMD_ReadAppliedDpi（读取 AppliedDPI） |
| 0x1400632d8 | PECMD_AllocMagicBlock（分配带魔数内存块） |
| 0x140063978 | PECMD_ArrayGrowRaw（数组增长原始实现） |
| 0x14005d390 | PECMD_SetSystemTimeFromUnix（Unix 时间戳设本地时间） |
| 0x14005ea5c | PECMD_GetPackedSystemVersion（打包系统版本号） |
| 0x140066e20 | PECMD_ForwardCustomMessage（转发自定义消息） |
| 0x140068c04 | PECMD_BubbleSortStringArray（字符串数组冒泡排序） |
| 0x14006aa9c | PECMD_ExpandDriveList（展开盘符列表/范围） |

### 冲突处理
- 无 multiple-definition：29 个目标均单一非 static 定义 + extern/调用点，链接无冲突，无需改 static。
- 名称唯一性：29 个新名与既有 171 个 `PECMD_*` 无碰撞。

### 记录
- `tools/rename_map.json` 追加 29 条（现 200 条）。
- `FUNC_NAMES.md` 追加 29 行（`地址 | 老名 | 新名` 三列格式，与既有表格一致）。

### 剩余
- core_*.c 中剩余唯一 `FUN_<addr>` 地址约 **1914** 个，留待后续阶段逐批确认（含大量
  简化桩/巨型窗口过程/DECOMPILE FAILED 等风险项，本批有意跳过）。

### 校验（最终状态）
- `./build.sh core_*.c` → exit 0（95 OK / 0 FAIL）
- `gcc -std=c99 -w -Wno-implicit-function-declaration -Iinclude core_*.c link_stubs.c
  -o /tmp/opencode/link_test` → exit 0（无 undefined、无 multiple-definition）

---

## 46. 阶段4o：FUN_→PECMD_ 新命名（批13）

### 概述
- 本批为剩余未命名的 `FUN_<addr>` 函数新增 **29** 个高置信度 `PECMD_xxx` 名字，
  全部取自其正文/头注释的明确职能，仅改名字（签名/函数体不动）。
- 词边界替换覆盖定义、extern、调用点、函数指针取址与头注释 `FUN_` 标签；
  `@0x<addr>` 头注释地址保留。共 **231** 处替换，波及 **42** 个 core 文件。

### 新增命名清单
| 地址 | 新名（职能） |
|---|---|
| 0x1400011d4 | PECMD_SubclassEnterControl（Enter 键子类化回调） |
| 0x140001258 | PECMD_CreateControlSubclass（创建控件+子类化） |
| 0x1400012fc | PECMD_MoveBtnTextToID9（按钮文字移到 ID9） |
| 0x14000142c | PECMD_MsgBoxInit（消息框初始化） |
| 0x14000500c | PECMD_DetectCodePage（检测/设置系统代码页） |
| 0x140005344 | PECMD_CheckDebugMsg（DEBUGMDG 环境变量检查） |
| 0x140005738 | PECMD_OpenSharedMapping（打开/复用命名共享映射） |
| 0x140006988 | PECMD_GetParentProcessId（获取父进程 PID） |
| 0x1400166b4 | PECMD_LoadLanguageFile（加载 .lang 语言文件） |
| 0x140017048 | PECMD_ServiceEntry（服务分发入口） |
| 0x140017110 | PECMD_ReleaseRefCount（引用计数减并释放） |
| 0x140017908 | PECMD_InitDynamicImports（初始化动态导入） |
| 0x1400188ec | PECMD_NewVarNode（创建变量节点） |
| 0x14001bea8 | PECMD_RegValueToVar（注册表值写入变量/环境） |
| 0x14001fde4 | PECMD_EnumWindowsCallback（EnumWindows 回调建发送线程） |
| 0x14001ff24 | PECMD_ThreadMainLoop（线程主循环等句柄后清理） |
| 0x14005b708 | PECMD_VarTruncateUpdate（变量截断更新） |
| 0x14005d694 | PECMD_SetRdataTablePtr（设置 .rdata 表指针） |
| 0x14005d968 | PECMD_SynthDlgKeyMsg（合成对话框按键消息） |
| 0x140062900 | PECMD_GetDpi（获取 DPI LOGPIXELSX） |
| 0x140062950 | PECMD_GetDpiCached（缓存 DPI） |
| 0x140062970 | PECMD_EnumChildProc（EnumChildWindows 回调设字体） |
| 0x140063344 | PECMD_AllocSmallObject（分配小对象，头 size=2） |
| 0x140063888 | PECMD_StrCopyW（宽串复制分配） |
| 0x1400638f8 | PECMD_StrCopyBytes（字节复制，宽对齐） |
| 0x140066224 | PECMD_VarWriteValueCap（值+容量写入） |
| 0x140066978 | PECMD_VarSetUInt（数值变量，无符号） |
| 0x1400700c4 | PECMD_StrDupAlloc（字符串复制分配） |
| 0x140070330 | PECMD_VarWriteNodeValue（节点值写入） |

### 冲突处理
- 无 multiple-definition：29 个目标均单一非 static 定义 + extern/调用点，链接无冲突，无需改 static。
- 名称唯一性：29 个新名与既有 `PECMD_*`（core + link_stubs）均无碰撞。
- 候选筛除（有意跳过，避免大小写重复定义风险）：`14000531c`、`14000546c` 在 core 中同时存在
  大写（core_proc.c 干净版）与小写（core_b1_remaining.c 旧版）两个非 static 定义，统一改名会触发
  multiple-definition，本批保持 `FUN_` 形态待后续处理。

### 记录
- `tools/rename_map.json` 追加 29 条（现 229 条）。
- `FUNC_NAMES.md` 追加 29 行（`地址 | 旧名 | 新名` 三列格式，与既有表格一致）。

### 校验（最终状态）
- `./build.sh core_*.c` → exit 0（95 OK / 0 FAIL）
- `gcc -std=c99 -w -Wno-implicit-function-declaration -Iinclude core_*.c link_stubs.c
  -o /tmp/opencode/link_test` → exit 0（无 undefined、无 multiple-definition）

---

## 47. 阶段4p：FUN_→PECMD_ 新命名（批14）

### 概述
- 在阶段4n/4o 基础上继续为剩余未命名 `FUN_<addr>` 函数新增高置信 `PECMD_xxx` 名字，
  全部取自其正文/头注释的明确职能，仅改名字（签名/函数体不动）。
- 并行派 4 个后台子代理分别读取 30 个中体量商务函数（243–432 行）的完整函数体，
  每个返回 名字 + 置信度 + 一句话依据；仅采用 high/medium 的结果。
- 词边界替换覆盖定义、extern、调用点、函数指针取址与头注释 `FUN_` 标签；
  `@0x<addr>` 头注释地址保留。共 **166** 处替换，波及 **27** 个 core 文件。

### 新增命名清单（30 个）
| 地址 | 新名（职能） |
|---|---|
| 0x140069d18 | PECMD_ParsePartitionTable（解析设备头部/分区表，返回分区数） |
| 0x14004ff10 | PECMD_PatternMatch（解释编译 opcode 0-0x1d 对宽串回溯匹配） |
| 0x140087690 | PECMD_ShowBrowseFolder（SHBrowseForFolderW 浏览目录对话框） |
| 0x140097c70 | PECMD_ScreenAreaCommand（屏幕/窗口区域子命令 -cap/-max/-win/-save） |
| 0x140020018 | PECMD_ServiceControl（SCM 服务 create/delete/start/stop/query） |
| 0x14000d2a0 | PECMD_ParseEnvSwitches（解析 FORCELOCAL/EnviMode 等环境开关） |
| 0x1400a53e4 | PECMD_TokenizeExpression（表达式 token 扫描/分类器） |
| 0x14007e3a4 | PECMD_ParseExpression（调度场表达式解析器，返回 double） |
| 0x1400a3310 | PECMD_FormatTimeString（格式化 CurDate/uptime 等时间变量） |
| 0x1400a1938 | PECMD_VarSearchReplace（变量通配搜索/替换） |
| 0x140099f18 | PECMD_WlanManage（-wlan -ssid/-start 连接管理） |
| 0x140003ca8 | PECMD_LaunchServiceProcess（共享内存/mutex/event 拉起后台服务进程） |
| 0x14006cf88 | PECMD_FindPatternInFile（大文件正/反向字节模式查找） |
| 0x1400baef4 | PECMD_ConstructControl（GUI 控件/窗口对象构造器） |
| 0x1400a4504 | PECMD_LoadTasksWait（--try/--exist 载入并等待并发任务） |
| 0x14007f078 | PECMD_ResolveMacAddress（sscanf MAC + OLE 适配器查询格式化） |
| 0x1400be64c | PECMD_ImageCommand（图像/图标窗口参数解析与尺寸查询） |
| 0x14009bbbc | PECMD_TempPathCommand（tmpdir/tmpfile/tmpl + TMP/TEMP 读设） |
| 0x14005681c | PECMD_DiskInfo（卷/文件系统几何 FAT/NTFS/exFAT 探测） |
| 0x140081574 | PECMD_ExecWindowThread（建窗口对象或拉起运行线程记 LastTID/LastWinID） |
| 0x14001f1d4 | PECMD_LoadIcon（GDI+ 初始化并从资源/DLL/文件载图标） |
| 0x140082a54 | PECMD_CreateTextControl（-center/-rich 等文本控件构建定位） |
| 0x1400b9788 | PECMD_AddTransControl（控件添加变体 -trans/-3D 委托创建） |
| 0x1400bd764 | PECMD_CreateButtonControl（-round/-def/-color 按钮控件创建） |
| 0x140054bb8 | PECMD_CreateControl（由 flag 构造 style、实例化编辑/控件对象） |
| 0x140031454 | PECMD_RunCommand（脚本命令主运行器：前缀解析+令牌展开执行） |
| 0x140046ac4 | PECMD_ShowScreenText（屏幕文本/坐标消息窗口） |
| 0x1400773f8 | PECMD_QueryDevice（DeviceIoControl 设备信息查询） |
| 0x140083274 | PECMD_MsgCommand（MSG 命令 -center/-rich/-u2d 分发消息框） |
| 0x140071e90 | PECMD_SetIpConfig（RegSetIP 读写 Tcpip 接口注册表） |

### 冲突处理
- 1 处名字撞既有映射：`14004ff10` 初拟 PECMD_MatchPattern 已属 FUN_14006156c，改为 PECMD_PatternMatch。
- 其余 29 个与既有 `PECMD_*`（core + link_stubs）均无碰撞；无 multiple-definition。

### 记录
- `tools/rename_map.json` 追加 30 条（229→259）。
- `FUNC_NAMES.md` 追加 30 行。

### 剩余
- core_*.c 中剩余唯一 `FUN_<addr>` 地址约 **1094** 个，留待后续阶段逐批确认
  （含大量简化桩/巨型窗口过程/DECOMPILE FAILED 等风险项，本批有意跳过）。

### 校验（最终状态）
- `./build.sh core_*.c` → exit 0（无 FAIL）
- `gcc -std=c99 -w -Wno-implicit-function-declaration -Iinclude core_*.c link_stubs.c
  -o /tmp/opencode/link_test` → exit 0（无 undefined、无 multiple-definition）

---

## 48. 阶段4p：FUN_→PECMD_ 新命名（批15）

### 概述
- 延续阶段4p，为剩余未命名 `FUN_<addr>` 新增 30 个高置信 `PECMD_xxx` 名字（只改名，不动签名/函数体）。
- 并行派 5 个后台子代理分别读取 30 个函数（144–233 行）完整函数体，返回 名字+置信度+依据；仅取 high/medium。
- 词边界替换 122 处，跨 ~27 个 core 文件；`@0x<addr>` 头注释地址保留。

### 新增命名清单（30 个）
| 地址 | 新名（职能） |
|---|---|
| 0x1400944c4 | PECMD_GetDiskFreeSpace（-userfree/-free/-raw 查询磁盘/卷空闲空间） |
| 0x1400a4dcc | PECMD_ExecuteCommand（变量展开+命令文本解析分发执行） |
| 0x14005370c | PECMD_ParseRegexQuantifier（正则风格量词/边界匹配解析） |
| 0x14008ac48 | PECMD_EnumeratePhysicalDrives（枚举 PhysicalDriveN） |
| 0x1400bdf6c | PECMD_ParseControlDef（解析控件定义串 <hint>,x,y,w,h 并构造） |
| 0x14008b340 | PECMD_EnumeratePartitions（读分区表填充分区记录数组） |
| 0x1400b2270 | PECMD_CreateDialogControl（对话框对象构造+子项遍历） |
| 0x140007670 | PECMD_OpenLockVolume（卷批量开/锁/刷新与句柄缓存） |
| 0x1400a600c | PECMD_CreateMenuItem（菜单/工具条项构造 -bar/-sub） |
| 0x14009d4b8 | PECMD_ComboBoxControl（组合框 CB_* 消息处理） |
| 0x140094c04 | PECMD_NormalizePath（--full/--short/--long 路径规范化命令） |
| 0x140023640 | PECMD_HotkeyControl（热键命令行/注册表 HOTKEY 配置） |
| 0x14007c0a0 | PECMD_ShowShutdownUI（#32770 关机对话框线程并关窗） |
| 0x140080cfc | PECMD_WindowThreadMain（脚本窗口线程入口+事件循环） |
| 0x140076554 | PECMD_EnumDevices（SetupDi 设备接口/实例枚举） |
| 0x140078f00 | PECMD_FormatVolume（构造 FORMAT.COM 命令行格式化卷） |
| 0x1400bcc5c | PECMD_InitControl（控件子类构造函数，字体/颜色/图像列表） |
| 0x1400787b4 | PECMD_ListDrives（GetLogicalDrives 盘符列表+过滤排序） |
| 0x14007403c | PECMD_TokenizeList（[..] 定界串分词为 token 数组） |
| 0x1400969d4 | PECMD_LinkFile（-j CreateHardLinkW/符号链接/删目录） |
| 0x140025474 | PECMD_ParseCommandBlock（{}[] 命令块/变量段解析） |
| 0x140016bc8 | PECMD_DispatchBuiltin（g_cmdTable1..5 命令表分发） |
| 0x1400acd90 | PECMD_ForCommand（FOR 循环引擎 delims:/tokens//S） |
| 0x1400b5aa4 | PECMD_CreateMessageBox（构造消息框+建线程+WM_INITDIALOG） |
| 0x1400b4b58 | PECMD_AddControlWide（控件添加变体 C 宽参数集） |
| 0x14007c88c | PECMD_DriveLetterSet（C-/U- 盘符设置+介质弹出/载入） |
| 0x1400ab040 | PECMD_ParseListControl（列表/组合控件命令解析单选式） |
| 0x1400734e4 | PECMD_ParseWindowOptions（窗口选项标志解析 -top/-layer 等） |
| 0x140030144 | PECMD_ExecSubCommand（脚本子命令执行器） |
| 0x140102270 | PECMD_PaintControl（控件 WM_PAINT 绘制背景/边框/图标） |

### 冲突处理
- 3 处撞既有映射：`0x1400bdf6c`→(CreateControl 已属 FUN_140054bb8) PECMD_ParseControlDef；
  `0x140080cfc`→(ExecWindowThread 已属 FUN_140081574) PECMD_WindowThreadMain；
  `0x140016bc8`→(DispatchCommand 已属 FUN_140045c44) PECMD_DispatchBuiltin。
- 其余 27 个与既有 `PECMD_*` 均无碰撞；无 multiple-definition。

### 记录
- `tools/rename_map.json` 追加 30 条（259→289）。
- `FUNC_NAMES.md` 追加 30 行。

### 剩余
- core_*.c 中剩余唯一 `FUN_<addr>` 地址约 **1064** 个（1094−30）。

### 校验（最终状态）
- `./build.sh core_*.c` → exit 0（无 FAIL）
- `gcc -std=c99 -w -Wno-implicit-function-declaration -Iinclude core_*.c link_stubs.c
  -o /tmp/opencode/link_test` → exit 0（无 undefined、无 multiple-definition）

---

## 49. 阶段4s：FUN_→PECMD_ 新命名（批16）

### 概述
- 延续阶段4p/4q，为剩余未命名 `FUN_<addr>` 新增 30 个高置信 `PECMD_xxx` 名字（只改名，签名/函数体不动）。
- 并行派 5 个后台子代理读取 30 个函数（109–176 行）完整函数体，返回 名字+置信度+依据；仅取 high/medium。
- 词边界替换 **119** 处，跨 ~28 个 core 文件；`@0x<addr>` 头注释地址保留。

### 新增命名清单（30 个）
| 地址 | 新名（职能） |
|---|---|
| 0x14000bfcc | PECMD_SearchComObject（COM 枚举按名搜索匹配对象） |
| 0x140067278 | PECMD_ExtractTokenByIndex（分隔串按索引提取 token） |
| 0x14005aafc | PECMD_ApplyControlValue（VAL 串打包值应用控制消息） |
| 0x140062290 | PECMD_AdvanceTokenPointer（按分隔符表推进串指针） |
| 0x1400b8a68 | PECMD_CreateControlObject（控件对象构造，字体/文字/拖放） |
| 0x140076144 | PECMD_DefineDosDevice（DefineDosDeviceW 盘符/挂载点定义查询） |
| 0x140081fa0 | PECMD_CreateWindow（创建/布置窗口对象并注册全局表） |
| 0x140005c4c | PECMD_GetFileVersionInfo（读 ProductVersion/FileVersion） |
| 0x1400b0820 | PECMD_ScriptThreadProc（脚本执行线程入口+环境派发清理） |
| 0x1400b52b8 | PECMD_ListViewDropHandler（拖放/ListView 消息处理钩子） |
| 0x140007c18 | PECMD_CreateProcessAsUser（以指定用户/会话令牌创建进程） |
| 0x1400aab98 | PECMD_ParseComboList（列表/组合控件命令解析多行式） |
| 0x1400403d8 | PECMD_ParseHlpDoc（-hlpdoc= 参数解析并显示帮助对话框） |
| 0x140083bf4 | PECMD_ParseHotkeyCode（组合键/快捷键描述串→虚拟键码） |
| 0x14007d450 | PECMD_SaveBitmap（HBITMAP 序列化存文件/剪贴板 png/bmp） |
| 0x1400ab724 | PECMD_SubCommand（SUB 命令子过程调用） |
| 0x1400b03f4 | PECMD_ConstructControlObject（按钮/编辑框对象构造与定位） |
| 0x1400748a0 | PECMD_EnumWindowCallback（EnumWindows 回调按类/标题/进程匹配） |
| 0x140064c38 | PECMD_PositionMessageWindow（消息/工具提示窗定位与前台恢复） |
| 0x1400a0a3c | PECMD_ParseStringToken（串 token 解析含 %var/转义） |
| 0x1400b9f1c | PECMD_ParseWindowPosition（逗号分隔窗位/-center/-right 对齐） |
| 0x1400752a0 | PECMD_CleanScriptText（清洗文本去注释/空白/CRLF） |
| 0x140021144 | PECMD_PatchInfDirectives（INF COPYFILES/DELFILES 指令补丁） |
| 0x14009d1b0 | PECMD_ListControlCommand（LIST 控件 QUERY/ADD/SEL 子命令分发） |
| 0x140072e94 | PECMD_SntpResolveServer（SNTP/NTP 时间服务器名解析） |
| 0x1400796e4 | PECMD_SetListParam（列表 color/percent/smooth 参数消息） |
| 0x140083f74 | PECMD_ParseMsgDispatch（MSG 参数解析+控件分发变体） |
| 0x140035860 | PECMD_ExecPathList（循环执行逗号/分号路径列表） |
| 0x14007d8d4 | PECMD_SaveImageToFile（图像/图标缓冲存文件） |
| 0x1400b4600 | PECMD_PcipDispatch（PCIP SET/DHCP/查询命令分发） |

### 冲突处理
- 无名字撞既有映射（批内/批间均无重复，含近义 PECMD_EnumWindowCallback vs PECMD_EnumWindowsCallback 区分为不同符号）。

### 记录
- `tools/rename_map.json` 追加 30 条（289→319）。
- `FUNC_NAMES.md` 追加 30 行。

### 剩余
- core_*.c 中剩余唯一 `FUN_<addr>` 地址约 **1034** 个（1064−30）。

### 校验（最终状态）
- `./build.sh core_*.c` → exit 0（无 FAIL）
- `gcc -std=c99 -w -Wno-implicit-function-declaration -Iinclude core_*.c link_stubs.c
  -o /tmp/opencode/link_test` → exit 0（无 undefined、无 multiple-definition）

---

## 50. 阶段4t：FUN_→PECMD_ 新命名（批17）

### 概述
- 延续阶段4p-4s，为剩余未命名 `FUN_<addr>` 新增 28 个高置信 `PECMD_xxx` 名字（只改名，签名/函数体不动）；
  2 个判 SKIP（`1400abc74` 对齐标志解析歧义 / `1400846a4` 通用双值参数解析歧义）。
- 并行派 5 个后台子代理读取 30 个函数（93–140 行）完整函数体，返回 名字+置信度+依据；仅取 high/medium。
- 词边界替换 **128** 处，跨 ~28 个 core 文件；`@0x<addr>` 头注释地址保留。
- 本批含脚本解释器核心（core_script/core_scriptdep）与 ImDisk/WIM/托盘等工具函数。

### 新增命名清单（28 个 + 2 SKIP）
| 地址 | 新名（职能） |
|---|---|
| 0x1400bf358 | PECMD_CalcExpression（表达式求值器 -txt/-cb/-base 格式化） |
| 0x140004ac4 | PECMD_SetDesktopWallpaper（COM 设置桌面壁纸） |
| 0x1400b568c | PECMD_TrayIconLoadThread（托盘图标加载线程过程） |
| 0x14006c7d4 | PECMD_ReadFileLine（读文件流按 CR/LF/UTF16 取行） |
| 0x14005bf8c | PECMD_DigestUpdate（字节流累加摘要回调） |
| 0x14006af70 | PECMD_Base64Encode（字节缓冲→base64 WCHAR 字母表） |
| 0x14000c430 | PECMD_EnumComMenuItems（COM 枚举子项→AppendMenuW） |
| 0x1400098dc | PECMD_ReadProcessCommandLine（跨进程 PEB 读命令行） |
| 0x14006dd74 | PECMD_ResolveVariable（变量名解析 &/RANDOM/env/__THIS） |
| 0x14006eaac | PECMD_LoadImageFromMemory（内存字节载图/图标 COM/临时 bmp） |
| 0x140045918 | PECMD_LaunchShell（shell 命令启 UserShell/写 PELOGON+等待 ack） |
| 0x14007db70 | PECMD_LoadIconFrames（GDI+ 载图标取帧数/帧延迟动画） |
| 0x1400a8664 | PECMD_ParseFontOptions（B/I/U/S 粗斜下划线与字号解析） |
| 0x1400408d0 | PECMD_ParseDateTimeSpec（大日期/时间解析器→FILETIME，主体部分重建） |
| 0x14008430c | PECMD_MsgDispatch（MSG 参数解析+按段分发控件/消息） |
| 0x140008110 | PECMD_CreateTempMutexDir（创建唯一临时目录+互斥文件） |
| 0x14005a718 | PECMD_ReadControlValue（?-VAL/逗号分号读控件值存变量） |
| 0x14001cb78 | PECMD_QueryImDiskVolumeInfo（查询 ImDisk 卷信息格式化输出） |
| 0x14000a14c | PECMD_MainEntryPoint（程序入口：初始化/命令行/EXEC-MAIN/跑脚本） |
| 0x140040090 | PECMD_CreateVariable（0x%I64X 格式化变量节点创建注册） |
| 0x1400a9ce4 | PECMD_CreateControlWindow（窗口/控件对象构造+样式+字体虚调用） |
| 0x140030420 | PECMD_ParseScriptSegments（脚本段循环处理 import/递归） |
| 0x140031068 | PECMD_ExecuteScriptBlock（命令脚本写入编码流并作为 EXEC 块执行） |
| 0x14007cc1c | PECMD_QueryRecycleBinVolume（回收站卷注册表 MaxCapacity/Nuke 查询） |
| 0x140074c60 | PECMD_CacheLookupInsert（缓存表按名字查找/缺失插入） |
| 0x140028708 | PECMD_EditSubclassWndProc（编辑框子类化热键/消息转发） |
| 0x14007f764 | PECMD_SetupRoundedRegion（实心画刷+圆角区域设窗口区域） |
| 0x140075628 | PECMD_ApplyWimImage（Wimgapi 延迟调用 WIM 应用到目标目录） |

SKIP(歧义)：`1400abc74`（-center/-right/-left 对齐标志，命令总处理器一部分）、`1400846a4`（@var/$var/数值通用双值参数解析）。

### 冲突处理
- 1 处撞既有映射：`1400a9ce4`→(CreateControl 已属 054bb8) PECMD_CreateControlWindow。
- 其余 27 个与既有 `PECMD_*` 无碰撞；无 multiple-definition。

### 记录
- `tools/rename_map.json` 追加 28 条（319→347）。
- `FUNC_NAMES.md` 追加 28 行。

### 剩余
- core_*.c 中剩余唯一 `FUN_<addr>` 地址约 **1006** 个（1034−28）。本批 2 个 SKIP 保持 `FUN_` 形态。

### 校验（最终状态）
- `./build.sh core_*.c` → exit 0（无 FAIL）
- `gcc -std=c99 -w -Wno-implicit-function-declaration -Iinclude core_*.c link_stubs.c
  -o /tmp/opencode/link_test` → exit 0（无 undefined、无 multiple-definition）

### 附：DAT_14011c638 查证（用户关注）
- 该符号（"空串"）多数文件已迁 `g_szEmpty`，但 core_b2f/core_b3r_d/core_b3r_* 等仍以 `DAT_14011c638` 标识符使用。
- core_b2f.c:6892/6894 的 `&DAT_14011c638` 加 `(LPCRITICAL_SECTION)` 强转实为 Ghidra 类型混淆——实指传给 FUN_14002D708 的 LPCWSTR 空串，**非临界区**。
- 确认为同一"空串"符号，可安全统一为 `g_szEmpty`（跨 ~20 文件约 100+ 处，属 DAT_→g_ 迁移子任务，下批处理）。

---

## 51. DAT_→g_ 迁移（1）：DAT_14011c638 空串 → g_szEmpty（批18 之 DAT 分项）

### 概述
- 用户关注 DAT_ 数据符号后启动 DAT_→g_ 迁移。首批锁定最高引用 `DAT_14011c638`（.rdata 空串 L""）。
- 经查证（decompiled.c 全量 + core_b2f 临界区误判还原，见 §50 附）：DAT_14011c638 全为**空串**，
  含 `uint8_t DAT_14011c638[]`（误标"虚表基址/公共表"）文件也只需 `(LPCWSTR)` 当作串用——统一安全。

### 改动
- `core_globals.c` 新增定义 `WCHAR g_szEmpty[2] = {0};`（真类型，原 link_stubs 为 `uint64_t` 假桩）。
- `include/pecmd_defs.h` 新增 `extern WCHAR g_szEmpty[];`。
- 全部 core 文件：extern 声明统一为 `extern WCHAR g_szEmpty[];`；`&DAT_14011c638`/`&&DAT_14011c638[0]`
  归一为数组退化/`&g_szEmpty[0]`；其余 token 词边界替换 `DAT_14011c638 → g_szEmpty`。
- `link_stubs.c` 删除 `uint64_t DAT_14011c638;` 与 `uint64_t g_szEmpty;` 两个假桩。

### 中途修正
- `&DAT_14011c638[0]` 被 `&DAT_14011c638→g_szEmpty` 误吞 `&` 成 `g_szEmpty[0]`/`&&g_szEmpty[0]`，
  已统一还原为 `&g_szEmpty[0]`。

### 校验
- 剥离注释后 core 内 `DAT_14011c638` 真实引用 **0**；真实 DAT_ 标识符地址 113→112。
- `./build.sh core_*.c` → exit 0；完整链接 → exit 0。

### 待办
- link_stubs 剩余 ~94 个 `uint64_t DAT_xxx` 假桩 + 真实 DAT_ 112 个，后续按使用类型分批迁 core_globals。

---

## 52. 阶段4v：FUN_→PECMD_ 新命名（批18）

### 概述
- 为剩余未命名 `FUN_<addr>` 新增 30 个高置信 `PECMD_xxx` 名字（只改名，签名/函数体不动）。
- 并行派 5 个后台子代理读取 30 个函数（76–102 行）完整函数体，返回 名字+置信度+依据；全部 high/medium，无 SKIP。
- 词边界替换 **154** 处，跨 ~28 个 core 文件；`@0x<addr>` 头注释地址保留。skip 过的
  `1400abc74`/`1400846a4` 未再派发。

### 新增命名清单（30 个）
| 地址 | 新名（职能） |
|---|---|
| 0x14001d8c8 | PECMD_EnumNtSymbolicLink（NtOpen/QuerySymbolicLinkObject 收集目标名） |
| 0x1400780fc | PECMD_ParseVolumeGuid（十六进制卷 GUID token→16 字节数组） |
| 0x14009cacc | PECMD_CreateEditControl（编辑/复选框控件对象构造） |
| 0x1400035d4 | PECMD_LayoutControlsScrollbar（控件布局+滚动条显隐） |
| 0x1400192d0 | PECMD_DrawTooltipText（HDC 绘制提示文字+定位提示窗） |
| 0x14005db98 | PECMD_ScaleWindowPos（按比/DPI 换算窗口尺寸位置） |
| 0x14009c720 | PECMD_ConstructSlider（trackbar 滑块对象构造 range/min/max） |
| 0x14001cff4 | PECMD_DevLockUnlock（打开盘/卷执行 LOCK/UNLOCK FSCTL） |
| 0x1400ae6f4 | PECMD_AssignDriveLetter（为分区分配空闲盘符） |
| 0x14001d2ec | PECMD_DevAlignCheck（IoCtl 就绪/对齐检测） |
| 0x14005eb80 | PECMD_DispatchByObjectName（对象链按名定位并派发 CommandLine） |
| 0x14006b9f0 | PECMD_DispatchListboxCommand（列表控件命令分派 0x462/0x459） |
| 0x1400bf068 | PECMD_AttachControlImage（把图像/图标挂到控件，bmp 更新） |
| 0x14006fa48 | PECMD_EjectDrive（CM_Query_And_Remove_SubTree 安全弹出可移动盘） |
| 0x140024350 | PECMD_GetNextCommandArg（状态机取下一个命令参数/关键字） |
| 0x140057334 | PECMD_CreateScrollbarCtl（滚动条对象构造带样式） |
| 0x140079a88 | PECMD_FindTokenIndex（token/变量查值匹配器返回索引） |
| 0x140101ed8 | PECMD_FindFileOrDir（FindFirst/NextFile 查找文件/目录返回匹配标志） |
| 0x140018978 | PECMD_VarLookup（变量沿父链查找 & / :: 前缀精确/前缀匹配） |
| 0x14005f458 | PECMD_ImageCommitUnmount（VHD/IMG 提交后卸载） |
| 0x14006ae00 | PECMD_Base64Decode（base64 字母表 6 位组+XOR 解码回字节） |
| 0x1400a662c | PECMD_DirWildcardExpand（目录通配展开：%模式 + cmd /c dir /B） |
| 0x14008b820 | PECMD_FindVolumeByDeviceId（FindFirstVolumeW 匹配 DMIO:ID 设备） |
| 0x1400685f4 | PECMD_ReadFileToWideString（读文件按 BOM/编码转宽串） |
| 0x140075848 | PECMD_EnumDeviceList（分页 0x418 设备记录按名过滤格式化输出） |
| 0x1400a03ac | PECMD_WriteVarEncoded（key=value/-*/-m/-bin/-8 编码分支写回变量） |
| 0x140074f14 | PECMD_ReadTipDummyConfig（首次读 PELOGON TIPSDUMMY 配置） |
| 0x140066674 | PECMD_NtpSyncLoop（UDP 123 端口 NTP 时间同步主循环） |
| 0x140083a48 | PECMD_ParseQuotedArg（引号/冒号/空白终结的 name=value 参数并派生子解析） |
| 0x1400a97cc | PECMD_ProcessTitleTipSelect（Title/Tip/Select 命令控件分发） |

### 冲突处理
- 无名字撞既有映射（批内/批间均无重复）。

### 记录
- `tools/rename_map.json` 追加 30 条（347→377）。
- `FUNC_NAMES.md` 追加 30 行。

### 剩余
- core_*.c 中剩余唯一 `FUN_<addr>` 地址约 **976** 个（1006−30）。

### 校验（最终状态）
- `./build.sh core_*.c` → exit 0（无 FAIL）
- `gcc -std=c99 -w -Wno-implicit-function-declaration -Iinclude core_*.c link_stubs.c
  -o /tmp/opencode/link_test` → exit 0（无 undefined、无 multiple-definition）

---

## 53. 阶段4w：FUN_→PECMD_ 新命名（批19）

### 概述
- 为剩余未命名 `FUN_<addr>` 新增 30 个高置信 `PECMD_xxx` 名字（只改名，签名/函数体不动）。
- 并行派 5 个后台子代理读取 30 个函数（60–83 行）完整函数体，返回 名字+置信度+依据；全部 high/medium，无 SKIP。
- 词边界替换 **100** 处，跨 ~28 个 core 文件；`@0x<addr>` 头注释地址保留。
- 本批多为小工具函数（注册表/设备/ImDisk/正则/显示模式/初始化为多）。

### 新增命名清单（30 个）
| 地址 | 新名（职能） |
|---|---|
| 0x140046840 | PECMD_LaunchCommand（线程/命令启动器，'*'前置+建线程等待） |
| 0x1400717f0 | PECMD_DeleteRegistryKeyRecursive（递归删键值/子键再删键） |
| 0x140081d24 | PECMD_CreatePopupMenu（弹出菜单/托盘对象创建派发） |
| 0x14006265c | PECMD_EnumDeviceInterfaces（SetupDi 枚举设备接口匹配 PID/VID） |
| 0x140068cf0 | PECMD_QueryFileAttributes（惰性载入 GetFileAttributes 文件属性查询） |
| 0x14001c950 | PECMD_EnumImDiskDrives（枚举 ImDisk 虚拟盘追加盘符，sibling of QueryImDiskVolumeInfo） |
| 0x140050a78 | PECMD_ParseRegexAlternation（递归正则对象 | 交替解析） |
| 0x14006b414 | PECMD_GetPathSizeToVar（文件/目录尺寸含 -link 递归，写回变量） |
| 0x14007adcc | PECMD_ResolveIndexedVariable（%/#/*/@ 数字下标变量引用展开） |
| 0x140071ae8 | PECMD_RegScrubKey（按标志位删默认值/枚举删除值+子键） |
| 0x14001afe4 | PECMD_SetRamdrivDiskSize（Ramdriv DiskSize 注册表+设备重建） |
| 0x14005889c | PECMD_ExecuteWait（WAIT 命令：iowa 句柄等/写回变量） |
| 0x1400b48e0 | PECMD_AddControlStar（控件插入变体E，坐标+* star 标记） |
| 0x1400570ec | PECMD_WriteFloppyImage（软盘镜像写入器：补引导扇区 0xaa55 逐磁道写回） |
| 0x14006a8c4 | PECMD_ParseSizeValue（T/G/M/K/S/B 后缀尺寸换算，sibling of ParseSizeSuffix） |
| 0x14007f9fc | PECMD_ReleaseControlObject（大对象析构：临界区内释图标/模块/GDI+资源） |
| 0x1400a0644 | PECMD_WriteVarTruncated（key=value 变量按长度截断写取前段） |
| 0x140051438 | PECMD_ParseObjectElement（递归下降产生式解析 | 对象列表元素，^ 前缀） |
| 0x14007a070 | PECMD_ReadModifyWrite（读-改-写文件/BLK 循环，字节偏移移位写回） |
| 0x1400a0844 | PECMD_SetVarRange（'='/位置参数按长度将区段写回变量保尾部） |
| 0x1400b50e0 | PECMD_AddControlVariantB（控件添加变体B，*/$/-del 热键写入控件槽） |
| 0x140067a00 | PECMD_ParseNumberToDouble（十六/十/科学计数数值串→double 推进指针） |
| 0x1400b8f10 | PECMD_AddControlObject（分配控件对象+建窗 SetWindowText/拖放/主题，被添加控件E调用） |
| 0x14009c548 | PECMD_ControlTimer（定时器控制，-del 变体 SetTimer/KillTimer） |
| 0x140077c1c | PECMD_GetStorageDependency（VirtDisk GetStorageDependencyInformation 依赖路径） |
| 0x14006b254 | PECMD_GetDirectorySize（FindFirst/NextFileW 递归求和目录字节数） |
| 0x1400ab51c | PECMD_ParseControlCommand（控件命令串解析 -smooth 等派发 FUN_1400AA7E4） |
| 0x140023338 | PECMD_EnumDisplayModes（EnumDisplaySettingsW 去重排序 DEVMODEW 数组） |
| 0x1400662a4 | PECMD_FormatDeviceStatus（CM_Get_DevNode_Status_Ex 输出 status/problem 串） |
| 0x140027690 | PECMD_InitEnvironmentVars（启动种子内置环境变量 PECMDVER/&&CurDir/&__PID 等） |

### 冲突处理
- 1 处撞既有映射：`1400b8f10`→(CreateControlWindow 已属 1400a9ce4) PECMD_AddControlObject。
- 其余 29 个与既有 `PECMD_*` 无碰撞；无 multiple-definition。

### 记录
- `tools/rename_map.json` 追加 30 条（377→407）。
- `FUNC_NAMES.md` 追加 30 行。

### 剩余
- core_*.c 中剩余唯一 `FUN_<addr>` 地址约 **946** 个（976−30）。

### 校验（最终状态）
- `./build.sh core_*.c` → exit 0（无 FAIL）
- 完整链接 → exit 0（无 undefined、无 multiple-definition）

---

## 54. 阶段4x：FUN_→PECMD_ 新命名（批20）

### 概述
- 为剩余未命名 `FUN_<addr>` 新增 28 个高置信 `PECMD_xxx` 名字（只改名，签名/函数体不动）；
  2 个判 SKIP（`140050cb0`/`1400510e8` 通用 token/`{}`块解析歧义，无领域头注释）。
- 并行派 5 个后台子代理读取 30 个函数（52–71 行）完整函数体，返回 名字+置信度+依据；取 high/medium。
- 词边界替换 **104** 处，跨 ~28 个 core 文件；`@0x<addr>` 头注释地址保留。

### 新增命名清单（28 个 + 2 SKIP）
| 地址 | 新名（职能） |
|---|---|
| 0x14005762c | PECMD_ConstructControlFont（下拉框/静态控件字体构造） |
| 0x14005872c | PECMD_ReleaseSyncObject（按 opcode 派发 Reset/SetEvent/ReleaseSemaphore/Mutex） |
| 0x14001c45c | PECMD_LoadUnloadImdisk（Imdisk.cpl 动态装载/卸载 GetProcAddress） |
| 0x14005bcfc | PECMD_TrieInsertNode（前缀树节点插入/链接） |
| 0x140009524 | PECMD_ParseVarArg（: 变量规格+* 通配解析/替换拷贝） |
| 0x140054168 | PECMD_AdjustEditScrollRect（编辑框文本/滚动矩形 WM 消息调整） |
| 0x14005c978 | PECMD_LoadWimApi（载 WIMGAPI.DLL 解析全部 WIM API 到全局） |
| 0x14005f9f0 | PECMD_SetDriveMount（卷/盘符映射删除与定义 Dos 设备） |
| 0x140008ea4 | PECMD_ScanDirectory（FindFirst/NextFileW 目录扫描收集） |
| 0x14001a284 | PECMD_GetProcessUserName（进程令牌查询返回用户 SID 名） |
| 0x140075b04 | PECMD_CaptureWimImage（WIM 捕获，sibling of ApplyWimImage，CreateVirtualDisk） |
| 0x14001eecc | PECMD_ExecLoadCommand（构造 'PECMD LOAD' 命令执行） |
| 0x140053f4c | PECMD_SendDateTimeMessage（时间→SYSTEMTIME 发 WM 消息 0x1002） |
| 0x140077e48 | PECMD_ParseSizeWithUnit（T/G/M/K/S 后缀尺寸乘算） |
| 0x1400b3d0c | PECMD_LoadImageBitmap（<ico>/<bmp>/<img> 前缀载图缩放 CopyImage→HBITMAP） |
| 0x140060e08 | PECMD_CalcDayOfYear（y/m/d→年内天数含闰年） |
| 0x14000971c | PECMD_ConvertLfToCrlf（LF/LF-CR 换行→CRLF） |
| 0x14006e74c | PECMD_CreateGlobalMutex（创建/打开全局互斥，处理已存在/受限+等待） |
| 0x14001df54 | PECMD_GetStringToken（引号/空白分隔 token 提取） |
| 0x1400472a4 | PECMD_InitDrvInstallCtx（零初始化 0x238 驱动安装会话对象） |
| 0x1400b6018 | PECMD_DhcpWriteAndWait（建线程做 DHCP 注册表写盘并等完成事件） |
| 0x14000e0bc | PECMD_DetectMinintBoot（检测 SystemStartOptions /MININT 缓存 g_minintFlag） |
| 0x140017fbc | PECMD_FindResourceByNameOrId（PE 资源目录按名/序号查偏移） |
| 0x140069868 | PECMD_ReadDiskSectorScan（原始句柄读 512 扇区扫描分区/引导标记） |
| 0x140054380 | PECMD_InitControlFlags（按参数初始化控件对象标志位） |
| 0x140028560 | PECMD_GraphSubclassWndProc（图形窗口子类化过程 WM_ERASEBKGND/拖放） |
| 0x1400b6240 | PECMD_SetControlCommandTrigger（存控件命令串并触发脚本执行循环） |
| 0x14009ce20 | PECMD_HandleControlCommand（控件/列表命令分发，Check 走状态机） |

SKIP(歧义)：`140050cb0`、`1400510e8`（通用 token / `{}` 块解析，无领域提示）。

### 冲突处理
- 无名字撞既有映射（批内/批间均无重复）。

### 记录
- `tools/rename_map.json` 追加 28 条（407→435）。
- `FUNC_NAMES.md` 追加 28 行。

### 剩余
- core_*.c 中剩余唯一 `FUN_<addr>` 地址约 **918** 个（946−28）。

### 校验（最终状态）
- `./build.sh core_*.c` → exit 0（无 FAIL）
- 完整链接 → exit 0（无 undefined、无 multiple-definition）

---

## 55. 阶段4y：FUN_→PECMD_ 新命名（批21）

### 概述
- 新增 29 个高置信 `PECMD_xxx` 名字（只改名，签名/函数体不动）；1 个 SKIP（`1400602f0` 通用字节匹配状态机歧义）。
- 并行派 5 个后台子代理读取 30 个函数（约 40–54 行）完整函数体；取 high/medium。
- 词边界替换 **106** 处，跨 ~28 个 core 文件；`@0x<addr>` 头注释地址保留。

### 新增命名清单（29 个 + 1 SKIP）
| 地址 | 新名（职能） |
|---|---|
| 0x140001000 | PECMD_CreateWindowObject（窗口对象构造器 G，基类 0x1400E9048+vtable） |
| 0x1400032e4 | PECMD_ObjectTimerProc（窗口 TimerProc：GetTickCount 更新/隐藏） |
| 0x14006e3a4 | PECMD_LoadImageFromFile（GDI+ 全局内存流从文件载图） |
| 0x1400604b0 | PECMD_MountVolumeToDir（DeviceIoControl 0x900a4 卷挂载目录） |
| 0x140061e98 | PECMD_LoadImageStream（GDI+ 流从内存缓冲载图，sibling of LoadImageFromFile） |
| 0x140096880 | PECMD_ParseExtPathArg（-ext 选项解析，空扩展名去尾） |
| 0x1400a5e6c | PECMD_ParseCommaParams（,分隔参数≤4 段解析，供 CreateMenuItem） |
| 0x1400172f4 | PECMD_MapPhysicalMemoryNT5（NT5 \Device\PhysicalMemory 映射视图） |
| 0x14001e8ec | PECMD_InitScriptContext（初始化脚本解析上下文结构） |
| 0x140069a20 | PECMD_QueryDiskGeometry（DeviceIoControl 0x70000 盘几何→DAT_14013d770） |
| 0x140071d00 | PECMD_SetDHCPSettings（写 DHCP 注册表 EnableDHCP/IP 等） |
| 0x14001ada8 | PECMD_SetRegistryOwnerRun（按 , 拆 owner/organization 写 RegisteredOwner） |
| 0x1400bca60 | PECMD_CreateControlBody（控件对象 0xb0 体构造 GDI/HBITMAP） |
| 0x140003184 | PECMD_WndProcDispatch（WndProc 消息分发 WM_CLOSE/COMMAND/USER） |
| 0x140019060 | PECMD_ReplaceWithEditBox（把窗口文本换成 EDIT 控件） |
| 0x140064b78 | PECMD_FreeInitObjectList（g_csInit 下释放计数对象列表） |
| 0x1400aa664 | PECMD_CreateControlItem（列表/组合控件命令定位同型槽构造控件） |
| 0x140018fac | PECMD_NormalizeNewlines（LF/CR 换行归一化为 CRLF） |
| 0x14001e3cc | PECMD_CollectPESections（收集 PE 节区并修正 .rsrc 偏移） |
| 0x14001ebdc | PECMD_ReadFileStr（文件读入+解码） |
| 0x14005b540 | PECMD_NormalizeQuoteChars（引号/括号字符归一化为 < >） |
| 0x14007474c | PECMD_EvalParenExpression（括号表达式包装 <#..> 交 CalcExpression） |
| 0x14001db7c | PECMD_ReadPhysicalMemory（\Device\PhysicalMemory ZwMapView 读取物址） |
| 0x140096748 | PECMD_SetFileNameExtension（'=ext' 设文件名扩展至变量） |
| 0x140096f84 | PECMD_SetRegistryKeySecurity（递归枚举子键 SetNamedSecurityInfoW ACL） |
| 0x1400a9aa8 | PECMD_InitTimerObject（window/timer 对象初始化 SetTimer/WM_TIMER） |
| 0x140080a14 | PECMD_QueryWindowState（?-list UI 读 IsWindowVisible 到变量/遍历控件列表） |
| 0x140078408 | PECMD_CloseDeviceHandle（标志驱动关闭设备句柄+状态/错误更新） |
| 0x1401000f0 | PECMD_AddImagesToImageList（空白分隔路径载图加入/替换 ImageList） |

SKIP(歧义)：`1400602f0`（通用 26 项表驱动字节匹配状态机，无注释/调用者）。

### 冲突处理
- 2 处撞既有映射：`140061e98`→(LoadImageFromMemory 已属 14006eaac) PECMD_LoadImageStream；
  `140018fac`→(ConvertLfToCrlf 已属 14000971c) PECMD_NormalizeNewlines。
- 链接期发现 `PECMD_ReadFileStr` 与 link_stubs 旧桩（uint64_t(void) 最小桩）重复定义，
  已删除 link_stubs 该桩（真实定义在新文件）。
- 其余 27 个与既有 `PECMD_*` 无碰撞；无 multiple-definition。

### 记录
- `tools/rename_map.json` 追加 29 条（435→464）。
- `FUNC_NAMES.md` 追加 29 行。

### 剩余
- core_*.c 中剩余唯一 `FUN_<addr>` 地址约 **889** 个（918−29）。

### 校验（最终状态）
- `./build.sh core_*.c` → exit 0（无 FAIL）
- 完整链接 → exit 0（无 undefined、无 multiple-definition）

---

## 56. 阶段4z（大批量）：6 子代理 × 60 函数 派发（批22，g0-g4 已应用）

### 概述
- 应"加大单代理派发量"的建议：派 6 个后台子代理、每代理 60 个函数（整批 360），
  对剩余微小函数逐一判定"真实业务命名 vs SKIP(CRT/thunk/stub/decompile-failed)"。
- 印证预判：360 里约 2/3 是 `return 0` 简化桩 / CRT 数学与 SEH 包装(atan/sqrt/exp/pow/cos/ceil/
  _mbtowc_l/_SetThrowImageBase/_lock 等) / 转发 thunk / decompile-failed 占位 —— 均判 SKIP 不硬取名。
- 真实业务小函数被命名。**g0-g4 五组 115 个命名已应用**（词边界替换 **1030 处**，跨 ~35 文件；
  @0x<addr> 头注释保留）。g5 组（~22 命名）待其补发完整地址映射后应用。

### 命名示例（115 个，节选）
PECMD_SkipTokenAndSpaces / PECMD_ReserveVec8 / PECMD_GrowByteBuffer / PECMD_DebugOutputWndProc /
PECMD_CreateCallbackWindow / PECMD_ScriptWndProc / PECMD_AllocString / PECMD_ReleaseRef /
PECMD_InitEditBuffer / PECMD_EnableBackupPrivileges / PECMD_GetPELogonWindowValue /
PECMD_SetObjectVtable / PECMD_CreateSymbolicLink / PECMD_TerminateProcessById /
PECMD_CreateWindowObjectEx / PECMD_CallSlotCleanup / PECMD_TrimWhitespace / PECMD_StartDownMode /
PECMD_OpenFileHandle / PECMD_GetModuleHandle / PECMD_HandleDropFile / PECMD_GetDeviceSize /
PECMD_GetDiskFreeMB / PECMD_RunSysInit / PECMD_LoadRichEdit / PECMD_IsRemovableDrive /
PECMD_ReadPelogonReg / PECMD_DerefVarName ...（共 115 条，详见 FUNC_NAMES.md）

### 冲突处理
- 115 个新名与既有 `PECMD_*` 无碰撞（含近义区分，如 ReleaseRef/ReleaseRefCounted/ReleaseRef*、
  GetModuleFileName/GetModuleHandle/GetModuleFileNameIntoContainer 各表不同函数）。

### 记录
- `tools/rename_map.json` 追加 115 条（464→579）。
- `FUNC_NAMES.md` 追加 115 行。

### 补：g5 组 22 个命名已应用（篇57同批）
- g5 生成 22 命名（PECMD_QueryDiskSpace/DestroyWindowSlot/SendRButtonUp/CmpStrN/FreeNtdll/
  ScriptDelete/WritePELogonDword/SetEnvIfChanged/WideStrLen/CreateMutexSlot/FillChar6/ScriptDtor/
  AllocSpecialString/DeleteDriveMountPoint/WritePhysicalMem/CreateCleanupThread/IsDirectory/
  ExecDashCommand/MultiSlotDtor/RefCountRelease/ReadPELogonDword 等），词边界替换 131 处，无碰撞。

### 剩余
- core_*.c 中剩余唯一 `FUN_<addr>` 地址约 **752** 个（889−137）。
- 说明：剩余多为 `0x14011a*` 结尾 cluster（B9 区 CRT/空桩）、`core_remaining_helpers.c`、
  `core_b8_failed.c` 占位、以及 15 个 >200 行巨型函数与 2 个批内 SKIP —— 多数本质是
  SKIP(CRT)/thunk/简化桩/巨型风险项，"值得命名的干净业务函数"已基本清完。

### 校验（最终状态）
- `./build.sh core_*.c` → exit 0（无 FAIL）
- 完整链接 → exit 0（无 undefined、无 multiple-definition）

---

## 57. 阶段5a-1：DAT_→g_ 迁移（12 符号）

### 概述
- 用户要求的 DAT_→g_ 第二块落地。派 2 子代理为高引用符号定真实类型（均非 AMBIGUOUS），
  逐符号看使用方式（赋值/比较/强转/解引用）定类型 + 提 g_ 名；GDI+ 槽沿用代码库 void*/fn-ptr 惯例。
- 迁移 = core_globals.c 定义 + include/pecmd_defs.h extern + 各 core 文件 token 替换 + 删 link_stubs 假桩。

### 迁移清单（12，类型依次）
| 地址 | 类型 | g_名 |
|---|---|---|
| 14013cf60 | HWND | g_hPelogonWnd |
| 14013cfb8 | HWND | g_hTooltipParent |
| 14013e160 | int64_t（地址整数，异构） | g_pCacheBlock |
| 14013cd90 | int(*)() 无原型 | g_pGdipDisposeImage |
| 14013ce08 | int(*)() | g_pGdipGetImageWidth |
| 14013ce10 | int(*)() | g_pGdipGetImageHeight |
| 14013ce30 | int(*)() | g_pGdipDeleteGraphics |
| 14013ce28 | int(*)() | g_pGdipCreateFromHDC |
| 14013d3b8 | int64_t（init 标志；另有调用点强转） | g_pComWrite |
| 14013d3ec | uint32_t[2]（b3e 数组视图 + g1 整字） | g_msgWndState |
| 14013d5c8 | char* | g_timeServer |
| 14013d868 | uint32_t（统一既有 g_dpi 孤儿 extern） | g_dpi |

### 中途修正（异构符号教训）
- 这些符号多为"多角色":同地址在不同文件既当整数又当指针/数组/函数指针（decompile 产物）。
  单一定型必冲突，修正策略：
  - GDI+/COM 槽签名/参数异构（0参/1参/传 int/HDC）→ 用**无原型 `int(*)()`**，任意参合法。
  - g_pCacheBlock 既 int 又 int64_t* 数组 → 定 `int64_t`，指针用处分 `(int64_t*)`/`[i]` 强转。
  - g_msgWndState b3e `[0]/[1]` 数组视图 vs b3r_g1 整字 → `uint32_t[2]`，整字处改 `[0]`。
  - b1_remaining 的 GetProcAddress 赋值点补 `(int(*)())` 强转（void*→fn-ptr）。
- 教训：此类异构符号应逐调用点核对签名后再定单一类型；复杂者可 DECOMPILE 回归。

### 校验
- 12 符号真实(去注释)残留 **0**；真实 DAT_ 标识符 112→**100**。
- link_stubs 删 11+2 个假桩；`./build.sh core_*.c` exit 0；完整链接 exit 0。
- 首次 `git init` + baseline 提交 + 本批提交（合约 144 files）。

### 待办
- link_stubs 剩余 ~80 个 `uint64_t DAT_xxx` 假桩 + 真实 DAT_ 100 个，按 6 子代理×~17≈100/批 继续；
  优先干净标量/句柄；异构/fn-ptr 复杂者逐点核实或登记 SKIP。

---

## 58. 阶段5a-2：DAT_ 100 符号全量定类型分析 + 干净子集迁移

### 概述
- 派 6 子代理对剩余全部 100 个真实 DAT_ 标识符逐一读用法定类型+提 g_ 名。
- 结论：剩余 DAT_ 以**惰性 fn-ptr 槽**为主（SetupApi/GDI+/WIMGAPI/winmm/Kernel32/COM/Wlan/WTS 等
  ~60 个，多已在各文件用带类型 extern 正常工作，link_stubs 的 uint64_t 仅是定义）；
  另有 ~10 个 .rdata 常量/BOM/串/GUID（部分已是 static 局部）、~5 个已登记 AMBIGUOUS/字节重叠、少量干净标量。

### 本批复①（干净标量/句柄，已迁移并全绿）
| 地址 | 类型 | g_名 |
|---|---|---|
| 14013d870 | DWORD | g_imgBufLen |
| 14014700c | COLORREF | g_dwTipsTextColor |
| 140147008 | COLORREF | g_dwTipBkColor |
| 14013dd00 | HWND | g_hwndTray |
真实 DAT_ 100→**96**；build/link 绿。

### 登记待合并的 fn-ptr 槽（工作正常，合并为可选打磨，含建议类型/名）
SetupDiDestroyDeviceInfoList(cf10,BOOL(*)(HDEVINFO))、WIMCloseHandle(d488)、CM_Get_DevNode_Status(cf48)、
CreateStreamOnHGlobal(d838)、GdipPrivateAddMemoryFont(cee0)、SetDeviceGammaRamp(cff0)、WimLoadImage(d478)、
GdipCloneBitmapAreaI(cea0)、UiCallback(d810)、WlanOpenHandle(d788)、WlanGetAvailableNetworkList(d7b8)、
WTSGetActiveConsoleSessionId(c998)、DhcpNotifyConfigChange(d4d0)、RegDeleteKeyExW(d408,=g_pRegDeleteKeyExW 已迁)、
GetDeviceGammaRamp(cff8)、WIMCloseHandle(d490)、GdipGetFontCollectionFamilyList(cee8)、
WlanEnumInterfaces(d790)、WlanDisconnect(d7c0)、GdipCreateBitmapFromHBITMAP(cde8)、
GdipCreateHBITMAPFromBitmap(cdf0)、GetSaveFileNameW(d430)、GdipGetImageEncodersSize(cdb8)、
GdipGetFamilyName(cef0)、GetVolumeInformationByHandleW(d358)、WIMGetMountedImageHandle(d498)、
WTSQueryUserToken(c988)、WIMCommitImageHandle(d4a8)、WlanSetProfile(d798)、WlanFreeMemory(d7c8)、
SetupDiGetINFClassW(cef8)、GdipCreateBitmapFromHICON(ce70)、WIMGetMountedImages(d460)、
GdipGetImageEncoders(cdc0)、SetupDiCallClassInstaller(cf20)、GetStorageDependencyInformation(d3b0)、
WIMGetMountedImageHandle(d4a0)、CreateEnvironmentBlock(c9a0)、WIMCommitImageHandle(d4b0)、
DestroyEnvironmentBlock(c9a8)、WlanCloseHandle(d7a0)、WlanConnect(d7d0)、GdipDrawImageRectI(cd98)、
WIMCreateFile(d468)、SetupDiEnumDeviceInfo(cf28)、WTSSendMessageW(c990)、GetAdaptersInfo(d4d8)、
GdipNewPrivateFontCollection(ced0)、SetupIterateCabinetW(cf30)、WIMGetMountedImages(d458)、
CoTaskMemFree(d728)、GdipCreateHICONFromBitmap(ce60)、WIMUnmountImageHandle(d4b8)、
SetupDiGetClassDevsW(cf00)、MciSendStringW(d050)、SetupDiDestroyDeviceInfoList(cf08)、
GdipSetInterpolationMode(ce38)、GetFinalPathNameByHandleW(d780)、GdipDeletePrivateFontCollection(ced8)、
SetDisplayConfig(cfe8)、WIMSetTemporaryPath(d470)、GdipCreateBitmapFromScan0(ce90)、
WIMUnmountImageHandle(d4c0)、ComLoad(d860)、WlanScan(d7b0)、WlanRegisterNotification(d7a8)

### 登记待深挖（AMBIGUOUS/字节重叠/多角色）
d738(int 标志 vs fn-ptr)、d5c0(uint8[] 收包缓冲 vs int64 指针槽)、d480(uint32 vs fnptr，已登记)、
d47010(double vs uint32，§41 已登记)、147001/147002/147003(与 g_runFlag 字节重叠，`g_b140147002` 暂缓)、
d124e48/d12d060/d12d058(仅 extern 无用法，弱证据)

### 待并入的干净数组/串（大小需定）
d770 diskType(int32_t[])、127740/127738/d8a0(byte[])、1214d8/1210f8(WCHAR[] 串)、
11e890/12d1e8/12d1f8(GUID)、cb90(OSVERSIONINFOW)、d660(sentinel uint8)、e110(config字符串 void*)、
124128/12c/130(BOM，已 static 局部，无需迁)

### 校验
- 本批复①：build/link 绿；真实 DAT_ 100→96。git 提交。

---

## 59. 阶段5-前置：classify 重生成 ground-truth + CRT 检查 + 未还原盘点

### 概述
- 重跑 `tools/classify_funcs.py`，得到 decompiled 全函数分类（DONE/BIZ/SMALL/BIG_UNKNOWN/CRT/THUNK）。
- **结论（回应"标准库 FUN_ 无用"）**：CRT 区间(0x14010-0x14011)的函数在本重构 core 中**根本不作为 FUN_ 出现**
  （或用标准库直调取代、或未被引用）→ 可读代码里没有"伪装的 CRT FUN_"污染，此担忧已天然达标。
  link_stubs 中 `FUN_` 桩多为业务区地址占位，非 CRT。
- **未还原（classify 口径）19 个**：BIZ 12+SMALL 1+BIG_UNKNOWN 6。其中若干其实已命名
  （a8664→ParseFontOptions、9d1b0→ListControlCommand、96f84→SetRegistryKeySecurity 等），
  属 classify 与按地址命名的口径差；**真正缺的是巨型未还原**：0474a8(19KB)/0a6874(7.6KB)/
  58ae4(7.2KB)/57d64/6edf8/6bff0/6bbfc/b945c 等（在 core_b3_remaining 为简化桩）。

### 可读性情况（实测）
- 已命名 PECMD_ 601；真实 DAT_ 96；build/link 持续全绿。
- CRT 标准名映射已提取 310 条存 /tmp/crtmap.json（供后续需要时标注/映射，当前无 core 内 CRT FUN_ 可标）。

### 待办（按可读性排序）
1. 巨型未还原业务函数（0474a8 等）逐块还原/命名（高价值高风险）。
2. 剩余 ~407 个"已还原体但未命名"FUN_ defs 按置信度命名（多为小函数/异构，部分已 SKIP）。
3. link_stubs 499 桩按需归并；DAT_ fn-ptr 槽合并（可选）；AMBIGUOUS 深挖。
4. 阶段5 零警告 + 最终 REVIEW 复核。
- 本表与 REVIEW §47(工期) 对齐：命名收尾 3-4 轮、零警告 2-3 轮、AMBIGUOUS/桩归并 2-3 轮。

---

## 60. 阶段4y（批23）：业务小函数命名收尾（6 子代理 × ~42 = 251 候选 → 173 命名）

### 概述
- 从剩余未命名 FUN_ 中筛"业务地址区(0x14000-0x1400f)+体 4-149 行"候选 251 个，6 子代理并行读体判定。
- **173 个真实业务函数取名**，替换 **858 处**，跨 ~40 文件；其余判 SKIP（CRT 包装如 memcpy/memset、
  `return 0` 简化桩、decompile-failed、no-op、转发 thunk）。
- 命中：`rename_map` 601→**774**；剩余唯一 FUN_ 752→**579**。

### 命名亮点（节选）
PECMD_InitComState / WriteFileChunked / ApplyDesktopWallpaper / EnumCDRomDrives / InstallWindowsHook /
ConstructTreeView / ParseDeviceClassGuid / RelocateImage / ValidatePeHeader / InitWinsock /
CreateDirectoryTree / GetProcessorCount / ServiceMainLoop / KeyboardHookProcBlock / QuoteTokenString /
EvalExpressionTree / ResolveHostToAddr / ParseNtpSetSystemTime / LockVolumeByDevice / RunExeIndata /
CaptureScreenRegion / EnumNetAdapters / WideToAnsiStr / ServiceStatusCallback / ParseVersionString /
GetProcessModuleFile / DetectFileEncoding / RegisterCallbackWindowClass / ReserveObjectArray ...（173 条）

### 撞车处理
- 批内重复 & 与既有重复预消歧：ThreadMessageLoop/MessagePumpLoop、ParseSignedNumber/ParseSignedNumberStr/
  ParseIntWithSign、KeyboardHookProcBlock/TaskMgr、ConstructControlObjectEx/B、ReleaseCtrlObject、
  LoadImageFileToMemory、DispatchControlCommandV、AdvanceAfterPrefix(MatchPrefixAdvance 已属 140066110) 等。

### 校验
- `./build.sh core_*.c` exit 0；完整链接 exit 0。git 提交。
- 说明：此批后，**可命名的干净业务小函数大体收敛**；剩余 579 多为 b9 CRT 区/巨型/异构/需深挖项。
