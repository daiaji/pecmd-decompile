# R26-h PART 真体化 — 启动档案 (2026-09-02, 待续)

> PART = R26-b 队列项 3, 唯一未动大件。本文 = 启动分析 + 恢复待办, 供下轮续作。

## 1. 目标与现状

- **目标**: `PECMD_ExecCommandLine`(core_b3_remaining.c:22667 简化桩, 返回 0)
  全文直移 dc `FUN_14008cffc`(reference/decompiled.c:89782-93625, **3845 行 / 29895B**,
  项目最大单体命令)。
- **调用面(已核)**: rb:7107 命令分派 `0x54524150`("PART")→ ExecCommandLine(param_1, cmd+8);
  b7c:12478 (MOUN 的 "PART update %d " 委托, buf+8 传参, @1400e0f5f 实证); GETF 无调用。
- **规模评估**: FORX 712 行/轮、GETF 1106 行/轮 → PART ≈ 3 轮当量; 建议分 3 段落码
  (S1: 90000-91100 初始化+选项循环; S2: 91100-92300 list/磁盘枚举; S3: 92300-93625 写/删/update)。

## 2. 命令面盘点 (dc 实测提取)

- **子命令**: list / disk / part / drv / cdrom / floppy / parent / volume / dep / init /
  fix / update / xupdate / hupdate / hupdatef / del(-del)。
- **选项 (- 前缀)**: -admin -super -devid -devidx -devidn -devida -gui -usb -hextp -phy -drv
  -fill -dvol -up -xup -axup -hup -ahup -gpt -gptmini -gpth -mbr -cmp -force -fs0 -fs -x
  -img -lskip -locku -locku: -lock: -lock -lockx -lock- -alignend -align -clear -clean -clean-
  -raw -swap: -iv -cdrom -floppy -L; 无前缀子命令不带 -。
- 模式串: " GPT "(错误信息) / "#21:INDATA" / "EXEDATA" / "INDATA" / "nt6" (宽串残留)。

## 3. 依赖面预核 (全部疑似真体, 落码前逐名验证)

PECMD_ExecCommandLine 内部调用的 helper 大多已在 GETF/FORX 轮验证过真体:
SkipLeadingControlChars / AllocWStringBuffer / AllocStrSlot / FreeStrBuf / MatchTokenAdvance /
AsciiPrefixICmp / ExpandVarDispatch / SplitTokenTrimWs / ParseIntRound / NextToken /
UnquoteString / AssignString / StrDupAssign / SetVariable / VarLookup / MarkKeyTable /
TlsLogWrite(变参, R26-g 归位) / PickFreeDriveLetter / FindVolumeByDeviceId /
UpdatePartitionLayout(core_b3l.c:558 真体) / GetPartitionLayoutEntry(core_b3l.c:949) /
GetDeviceSize / NormalizeVolumeDevPath / QueryDeviceIoInfo / OpenFileExisting /
IsDevicePathPrefix / SetFilePointer / ZeroLenBuf / GrowByteBuffer / ExpandDriveList。

## 4. 恢复待办 (按序)

1. 读 dc 全文 89782-93625 (分 3 段), 建立 local_* 变量映射表 (函数有 ~120 个局部)。
2. S1 落码: dc:89782-91100 (初始化 ~40 槽 + 选项循环 -xxx 30+ 项 + 主 token 切分)。
3. S2 落码: list/disk/part 枚举族 (DeviceIoControl 布局查询)。
4. S3 落码: 写/删/update/init/fix 族 + epilogue (含 TlsLogWrite 变参调用点 5 处, R26-g 已备)。
5. 语法门 → clean 构建 → 64/64; 语料: PART 无对拍案 (磁盘副作用), 归队列项 9。
6. HANDOVER R26-h 入账 (偏差: 若有 Ghidra 丢参, 按 capstone 实证补, 同 R26-f 法)。

## 5. 会话背景 (本档案前已完成)

- R26-e FORX / R26-f GETF(+065 语料扩展) / R26-g SHOW TODO 收口 / run_case -hide 无窗口化;
  全部 64/64 零回归并提交 (pecmd-decompile db8c7e8/4ee67fa, compat 6d85fff)。
- 注意事项: build/msvc 需 clean 重建 (陈旧 obj 曾毒化); 部署需先 taskkill pecmd_msvc.exe;
  探针严禁触碰系统路径 (R26-f 事故教训)。

## 6. 结构图更新 (2026-09-02 深读确认)

- dc:90112-90593 = **选项循环** (-admin/-super/-devid* 四兄弟/-gui(→RunCommand #21:INDATA)/
  -usb/-hextp/-phy/-phy#/-drv/-fill/-dvol/-up/-xup/-axup/-hup/-ahup/-gpt/-gptmini/-gpth/
  -mbr/-cmp/-force/-fs0:/-fs0/-fs/-x/-img/-lskip/-locku/-locku:/-lock:/-lock/-lockx/-lock-/
  /mbr=/pbr/-alignend/-align/-clear/-clean/-clean-/del/-del/-raw/-swap:/-CHS=/-IMG=/-SKIP=/
  -iv=/-iv/-cdrom/-floppy), 出口 LAB_14008e216 (37 引用) → 空游标 goto LAB_14008e22c。
- dc:90596-90618 = 选项后处理 (ptVar68/local_228/local_248/-9 哨兵链 → -1/-8 归位)。
- dc:90620-90713 = **子命令编码**: list/disk/part/drv/cdrom/floppy/parent/volume/dep →
  local_3e8 低字节 1-8; init→local_448=1; fix→local_400=0x10; update→bVar52=0x10;
  xupdate→local_463=0x20; hupdate/hupdatef→bVar52=0x10/0x18。
- dc:90714+ = 主体分派 (local_3e8/local_448/local_400/local_463 编码 → 各执行段),
  至 dc:93616 LAB_14009445a/93614 LAB_14008f546 = 函数尾。
- 全部 96 LAB 可达, 无死代码。变量声明区自洽 (211 个, local_ 零遗漏)。
- **恢复建议**: S1=dc:90112-90713 (选项+编码, ~600 行, 无歧义); S2/S3 按档案原分段。

## 7. R26-i 直移进行中状态 (2026-09-03 存档, 会话可恢复)

### 7.1 测试安全边界 (定案, 不可越)

- PART = 磁盘写命令引擎, **严禁实体机执行任何 PART 语义用例** (-clear/-clean/del/
  init/fix/update/xupdate/-raw/-swap 等全部为写盘/锁卷路径)。
- 验证仅三件无副作用事: ① `cl /Zs` 语法门 ② clean 构建 (msvc, 先 taskkill
  pecmd_msvc.exe + 清 .obj) ③ 64/64 语料回归 (语料不含 PART 案, 仅保零回归)。
- **不新增 PART 对拍语料** (磁盘副作用, 维持队列项 9 决策)。

### 7.2 capstone 核验定案 (落码依据, 已实证)

- pthreadmbcinfo 伪字段字节偏移: refcount@0 / mbcodepage@4 / ismbcodepage@8 /
  mbulinfo@12(ushort* 粒度)。选项循环各分支匹配后游标推进 = token 字符数
  (asm `add r11,N*2`): -locku:+1 / -lock:+6 / -swap:+6 / -CHS=:+5 / -SKIP=:+6 /
  -iv=:+4 / -align:+6 / -alignend:+9 / /mbr /pbr 尾检:+4。
- dc:90995 wsprintfW 缺参 → r8d=[rsp+0x74]=local_434 (asm 0x14008ef0f)。
- dc:90998 `ptVar53->mbctype+10` → 实为 ptVar53+17 WCHAR (asm lea rax,[rsi+0x22])。
- dc:90999 ParseNumSkipWs(&local_2f0,&local_res20) 后 local_360=local_res20。
- dc:91241 `LStack_270._0_4_` → FUN_14001EA18 尺寸写在出参槽+8B:
  local_438=(DWORD)(local_278.QuadPart>>32) (asm 0x14009413e)。
- dc:90065/90072/90075 local_310 三处 = Ghidra 寄存器残影, asm 无 store, 不落码。
- 子命令编码: 0=list 1=disk 2=part 3=drv 4=cdrom(368=8) 5=floppy(368=0x10)
  6=parent 7=volume 8=dep; init→448=1,res20低32=1; fix→400=0x10;
  update→463=0x10(bVar52) xupdate→463=0x20 hupdate→0x10 hupdatef→0x18。

### 7.3 数据模板 (原版 .data 直取, 已生成 C 数组入码)

- g_bootDos[400]@14013a370 / g_bootNt5[384]@14013a500 / g_bootNt6[440]@14013a680 /
  g_gptDiskGuidDef[12]@140127a50; GPT 未用条目模板 @140124100 = 16×0。
- GPT 盘 GUID 固定值: local_2e8=0x4433b9e5ebd0a0a2, LStack_2e0=0xc79926b7b6689c87。
- GPT 默认分区名 local_1e8 = L"MS Basic Data" (@140127aa0)。
- DAT_14011d500 = L" " (list 分隔); 三 GUID @14011e870/880/8a0 =
  53F56307/53F56308/53F56311-B6BF-11D0-94F2-00A0C91EFB8B (disk/cdrom/floppy 接口类)。

### 7.4 落码载体

- 新文件 `src/commands/core_b3r_i28g.c` (i28b.c 同法), 替换
  core_b3_remaining.c:22664 桩 (收尾时移除桩)。
- 进度: dc:89782-91258 已落码 (头/externs/模板/声明/初始化/选项循环/哨兵归位/
  编码/切分/-img=/hd/# 后缀/路径构造/MBR 保护/锁路径/引导模板选择)。
- 恢复待办: ① dc:91259-91320 (打开+读+模板写首扇区) ② 91320-91890 (IMG/INDATA
  写引擎) ③ 91891-92460 (GPT 写引擎前半) ④ 92462-93140 (GPT 条目操作+list 输出)
  ⑤ 93141-93623 (尾+epilogue) ⑥ 移除旧桩 → 语法门/clean 构建/64/64 ⑦ HANDOVER
  R26-i 入账 + compat reverse-map PART 行同步。

## 8. R26-i 续作存档 #2 (2026-09-03, 全文已落码, 语法门修复中)

### 8.1 完成状态

- **落码完成**: src/commands/core_b3r_i28g.c = 4308 行全文直移
  dc:89782-93623 (PECMD_ExecCommandLine 全 3845 dc 行), 含:
  S1 声明/初始化/选项循环/哨兵归位/子命令编码;
  S2 切分/-img=/hd/# 后缀/路径构造/MBR 保护锁路径/引导模板选择;
  S3 DescribePartitionInfo/INDATA 资源写盘/parent-cdrom-floppy-volume-dep-drv
  分派/GPT 头构造与 CRC/备份镜像写回/-cmp 合并/条目槽/对齐与 CHS 打包/
  MBR 条目引擎/MountedDevices 注册表/mbr-part-list 输出/尾段扫描/epilogue。
- **旧桩已移除**: core_b3_remaining.c:22664 简化桩 → 指向 i28g.c 的注释。
- **sources.rsp**: 已加入 core_b3r_i28g.c (第 38 行)。
- **数据模板**: g_bootDos[400]/g_bootNt5[384]/g_bootNt6[440]/g_gptDiskGuidDef[12]/
  g_guidDisk/g_guidCdrom/g_guidFloppy/g_gptUnusedEntry16 = dc .data/.rdata 直取。
- **capstone 补全** (Ghidra 丢参/错参, 已定案):
  - 90995 wsprintfW "\.\PhysicalDrive%d" 补参 r8d=local_434
  - 90998 +17 WCHAR 数字起点; 90999 ParseNumSkipWs(&local_2f0,&local_res20)
  - 91241 FUN_14001EA18 尺寸 = 出参槽+8B → local_438=(DWORD)(size>>32)
  - 93099/93594 SHSetValueW 真 6 参 (HKLM,path,name,3,pcVar57+0x1b8,0xc)
  - 93601 SHDeleteValueW 3 参
  - 93170/93176 格式串实为 "%s%d"/"%s%d " (非 dc 的 %s%s%d)
- **Ghidra 残影剔除**: local_310 三处寄存器残影; ptVar44 哨兵链 -9/-2/-1 显式化。

### 8.2 修复中问题 (语法门剩余 ~20 处)

类型: (1) L'\' 悬空反斜杠字面量×2 (1384/2148/2177); (2) LPOVERLAPPED
未声明×多 → NULL 替换(已做 24 处); (3) LARGE_INTEGER 与整型互转缺 .QuadPart
(~10 处, 1668/2197/2298/2500/2825/3065-66 等); (4) undefined4 未声明
(2207) → uint32_t; (5) GetFileExInfoStandard 未声明 (2589) → 1; (6) void*
算术 local_390+6 已修。继续逐错修至 0 错误 → clean 构建 → 64/64 回归。

### 8.3 恢复命令

- 语法门: cmd //c "build\msvc\_r26i_syngate.bat"  (先修上述类型错误)
- 构建: 见 build/msvc 脚本 (先 taskkill pecmd_msvc.exe + 清 .obj)
- 回归: harness 64 案 (不含 PART 语料)

## 9. R26-i 续作存档 #3 (2026-09-03, 语法门修复中, 会话压缩点)

### 9.1 状态

- core_b3r_i28g.c 4308 行全文已落码 (dc:89782-93623), 旧桩已移除, sources.rsp 已加。
- 语法门 (cl /Zs) 未过。当前仅剩结构类错误, 集中在 do-while 嵌套区
  (dc:91137 do → dc:93500 `while(true)` 为**单一巨型循环**, 内部 goto 流):
  1. 1570 行 else 未闭合 (fatal C1075) — 91065 大 else 块贯穿到 dc:91340
  2. 1698 行 uVar18 语法错 (由 1570 未闭合连带)
  3. 1700 行字符字面量含真 NUL 字节 (0x27 0x00 0x27) — 需字节级修复为 `\0`
     (反复 replace 后文件仍余 1 个 NUL, 待查: 可能写入被缓存或 NUL 在注释区)
- 已修: LPOVERLAPPED→NULL(24处) / undefined4→uint32_t / GetFileExInfoStandard→1 /
  local_368 声明补 / L'\' 悬空字面量×3 / LI64 helper / CloseDeviceHandle void×3 /
  SetFilePointer 二参形式 / local_2e8[4] 下标 / LARGE_INTEGER cast / do-while 闭合骨架。

### 9.2 结构要点 (下轮恢复必读)

- dc 配对 (脚本实证): 91137 do→93500 while(true) 巨环; 内嵌 do: 92168→92213,
  92351→92410, 92470→92508, 92525→92532, 92541→92553, 92672→92683, 92791→92809,
  92800→92815, 93167→93196, 93277→93283, 93296→93303, 93315→93322, 93459 while。
- 91062 if → else(91065) 块尾在 dc:91340 (12 空格 `}`); 我的落码该处需自然闭合。
- 恢复命令: 语法门 cmd //c "build\msvc\_r26i_syngate.bat";
  通过后 clean 构建 (taskkill pecmd_msvc.exe + 清 build/msvc obj) → 64/64 回归 →
  HANDOVER R26-i 入账 + compat reverse-map PART 行同步。

## 10. R26-i 存档 #4 (2026-09-03 会话压缩点, 语法门修复中断)

### 10.1 精确进度

- 文件: src/commands/core_b3r_i28g.c (4319 行→压缩时 ~4300 行), 落码已完成,
  旧桩已移除, sources.rsp 已登记。
- 语法门仍 FAIL: 结构括号错误已收敛到 2 个症状:
  1. `(1698): error C2061 标识符 uVar18`
  2. `(246): fatal C1075 "{" 未找到匹配令牌` (= 函数体未闭合)
- 已确认修复项 (全部完成): NUL 字面量字节级清除 (曾因 `b"'\0'"` 八进制陷阱反复);
  LPOVERLAPPED→NULL; undefined4→uint32_t; GetFileExInfoStandard→1; local_368 声明;
  L'\' 悬空字面量×3; LI64() helper; CloseDeviceHandle void×3; SetFilePointer 二参形;
  local_2e8[4] 下标; LARGE_INTEGER cast 若干; 93501-93521 闭合链 (一度多闭, 压缩时
  正改为 2 个闭合: else(91065)@93508 + if(91062)@93519)。

### 10.2 关键结构定案 (dc 实证, 恢复必读)

- dc:91137 `do {` → dc:93500 `} while(true)` 单一巨环 (含 f48e 回边),
  其内嵌小 do 配对: 92168→92213, 92351→92410, 92470→92508, 92525→92532,
  92541→92553, 92672→92683, 92791→92809, 92800→92815, 93167→93196,
  93277→93283, 93296→93303, 93315→93322; 93459 是 while 非 do。
- 外层: dc:91051 if → dc:91062 if(大, 含上面巨环, 闭于 93519) → else 尾 93520-93521。
  dc:93514 `}` 闭 else(91065) 分支体; dc:93519 `}` 闭 if(91062)。
- 函数级尾部顺序: 93522 code_r0x → 93530 f5f0 → 93536 929d0 → 93543 f528 →
  93546-93559 cdrom 搜索(不可达) → 93561 367e → 93565 3820 → 93569 3838 →
  93581 return LVar35 (dc 尾 93623 前另一 return) → 93584 13e4 → 93588-93595 1419 →
  93596 1470 → 93603-93613 → 93614 f546 → 93616 9445a → 93622 return。
- **闭合链替换已实际落盘** (最后命令显示 cancelled 但已写入):
  文件现于 4199 行 `} /* dc:93508 闭 else(91065) */`、4200 行
  `} /* dc:93519 闭 if(91062) */`, code_r0x@4201 仅一处 (旧多闭链已删)。
  **恢复时不要再做此替换**, 直接跑语法门看剩余错误。

### 10.3 恢复步骤 (下轮)

1. 先完成 10.2 的闭合链替换 (删 4200-4212 旧链, 换 2 个 `}`)。
2. 跑语法门 cmd //c "build\msvc\_r26i_syngate.bat", 清零剩余 error C。
3. clean 构建 (taskkill pecmd_msvc.exe + 清 build/msvc obj) → 64/64 回归。
4. HANDOVER R26-i 入账 + compat reverse-map PART 行同步。

### 10.4 压缩点真实错误快照 (闭合链替换已落盘后复测)

- 语法门仅剩 2 错误:
  1. `(1698): error C2061 语法错误: 标识符"uVar18"`
  2. `(1563): fatal C1075 "{" 未找到匹配令牌` ← dc:91062 的 if 打开行
- 解读: 4199-4200 两个 `}` 仍与实际嵌套不匹配 —— dc:93514 `}`(闭 else-91065 分支体)
  与 dc:93519 `}`(闭 if-91062) 之间, 我的代码在 93514 处已提前闭过一层 (见 4199 前
  深度: 4198 `} while(true)` 后深度 5 → 4199 后 4 → 4200 后 3), 而期望是
  while(true) 闭 do(91138) 后还剩 else(91065)/if(91062)/if(91134?) 多层。
- 恢复建议: 以 dc 原文 93500-93521 逐行对照我的 4190-4200, 用 dc 缩进层级
  (12/10/8 空格) 直接定每层归属; 重点核对 93502-93507 (if-91134 的 else 尾清理)
  是否被误放函数层。dc 实证配对见 §10.2。

## 11. R26-i 收口终态 (2026-09-03, 全绿)

> 本节取代 §10 的待办语义。§7 安全边界仍然有效: PART 严禁实体机写盘测试、不新增 PART 语料。

### 11.1 语法门修复全记录 (syngate exit=0)

事件级对齐法: 以 dc 括号事件序列(kind+depth)对 C 逐段比对 (front 91064-91889 /
mid 91889-93213 / tail 93213-93519 / epilogue 93486-93625), 全部归零。修复清单:

| # | 位置 | 缺陷 | 修复 |
|---|---|---|---|
| 1 | 旧1697 | dc 91173-91185 只闭 3 层, C 有 4 个 `}` (多一个) | 删多余 `}` |
| 2 | 旧1703 | dc 91192 外层 if(local_1e0) 内还嵌 91193 内层 if(91193), C 合并成一层, LAB_140093fba 清理块失去内层保护 | 补内层 `if ((local_434<0) && (*ptVar30==L'\0'))` |
| 3 | 旧3297 | dc 92656 `}` (闭 else-if(92590)) 丢失 | 补 `}` |
| 4 | 旧3301 | dc 92660 单语句 if(goto) 无花括号, C 包成 `{}` 块 (+1 层) | 改单语句 |
| 5 | 旧3333 | dc 92690 单语句 if(goto) 无花括号, C 包块 | 改单语句 |
| 6 | 旧3634 | dc 92968 单语句 if(goto) 无花括号, C 包块 | 改单语句 |
| 7 | 旧3695+3701 | dc 93031 `}` (闭 else-93013) 在 C 中位置提前一行, 变两个 `}` (多一个) | 删 3695 的多余 `}` |
| 8 | 旧3710后 | dc 93041-93045 第二个 0x55/0xAA 签名 if 块整块丢失 | 补回 |
| 9 | 旧3748 | dc 93076 单语句 if(goto) 无花括号, C 包块 | 改单语句 |
| 10 | 旧3776 | dc 93101 单语句 if(goto LAB_140092b14) 无花括号, C 包块 | 改单语句 |
| 11 | 旧3827后 | dc 93146 `goto LAB_1400906d1;` + 93147 `}` 丢失 | 补回 |
| 12 | 旧3999 | dc 93310 if(-1<uVar20) 在 if(93291) 体内, C 提前闭了 if(93291) | 删提前 `}` (4142 原位闭) |
| 13 | 旧4142后 | dc 93452 `}` 闭 if(93291) + 93454 `}` 闭外层块 丢失 | 补两个 `}` (goto LAB_14008fdaf 前后) |
| 14 | 旧4184 | dc 93494 单语句 if(goto LAB_1400929d0) 无花括号, C 包块 | 改单语句 |
| 15 | 旧4198后 | dc 93501-93521 整段丢失: 闭 if(91135) / if(91117) 内清理(93502-93507)/闭 if(91117) / else 内清理(93509-93513)/闭 else(91071) / **LAB_14009444b 段(93515-93518)** / 闭 if(91062) / 函数级清理(93520-93521) | 全部重建 |
| 16 | 旧1697区 | dc 91186 `LAB_14008f58c:` 标签丢失 (3 处 goto 目标) | 补标签 |
| 17 | 旧3998区 | dc 93508-93514 清理段中的 **LAB_14008f528 goto 目标**(93507) / **LAB_14009444b**(93515) 两个标签缺失导致 goto 悬空 | 随 #15 一并归位 |

规律总结: ① dc 单语句 if(goto) 无花括号 → C 误包 `{}` (+1 层, 共 7 处);
② dc 巨体闭合链中有清理语句 + 标签夹层 → 移植时漏段 (93501-93521);
③ 上轮"闭合链已落盘勿重做"的判断基于错误前提, 本轮按 dc 实证配对全部重排。

### 11.2 链接修复 (4 符号)

- `FUN_140007670` → 已真体化于 core_b1_remaining.c 的 `PECMD_OpenLockVolume`
  (xproto.h:217), i28g extern 声明与 dc:91030 调用点同步改名。
- `FUN_140102A90` → memset 别名, 按 core_b3r_h4.c 惯例加
  `#define FUN_140102A90(dst,val,n) memset((dst),(val),(n))`。
- `SBORROW2/SBORROW4` → Ghidra 符号借位(溢出旗标) helper, dc 90974/93002 直移,
  i28g 文件内 static __inline 定义 (16/32 位两型, 仅本文件使用)。

### 11.3 构建与回归

- 语法门 `build\msvc\_r26i_syngate.bat` = **exit 0**。
- clean 构建 (`rm build/msvc/*.obj` → _msvc_cmd.bat) = **OK, exe 产出**。
- 部署 C:\pectest\pecmd_msvc.exe → `run_corpus.sh` 全量 →
  `diff_case.py --all` = **{"total": 64, "pass": 64, "fail": 0}**。
- PART 语料零新增 (安全边界 §7); PART 命令本身无对拍案, 归队列项 9。

### 11.4 下轮提示

- PART 真体化后仅经"编译+64/64 零回归"验证, **行为级对拍为空白** (无语料);
  如后续扩 PART 语料, 必须在无盘/虚机环境 (参考 §7 边界)。
- reverse-map.md (pecmd_compat) PART 两行已同步真体化状态 (本节同轮提交)。
