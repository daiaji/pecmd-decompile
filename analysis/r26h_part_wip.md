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
