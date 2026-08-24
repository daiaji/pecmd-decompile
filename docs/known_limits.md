# known_limits.md — KNOWN-LIMIT 登记簿（场景 B 交付物）

> 格式：id / 地址 / 现象 / 影响面 / triage（PRODUCTION_ROADMAP §3.B.5-5b）
> 本文件是场景 B 的"诚实标注"汇总；行为类警告逐条账目另见 tools/warning_census_b0.json。

## P10 — 隐式内建不匹配与残留警告三处（B0 开票，不修）

- KL-P10-1 / b3r_g1.c:140 / 隐式 memset 内建不匹配（64 位截断风险：size 实参经 int 中转）/
  影响面：该初始化路径的缓冲区大小上限 / triage: [RESOLVABLE-BLACKBOX]→C-P1 以 dc/asm 定案
- KL-P10-2 / core_b7c.c:2778 / 同上（memset 内建参数宽度不匹配）/
  影响面：同上 / triage: [RESOLVABLE-BLACKBOX]→C-P1
- KL-P10-3 / core_b3_remaining.c:21205 parentheses、core_b7c.c:4930 unused-label /
  影响面：无（可读性）/ triage: [RESOLVABLE-STATIC]→B1 机械清扫批

## P11 — link_stubs 疑似转写失真三处（只登记，禁顺手修复）

- KL-P11-1 / 原 ls:11986 / `(x & 0xffffffffff)==0x30003000300020` 恒假比较（掩码 40bit vs 常量 56bit）/
  影响面：该分支在原 EXE 行为中可能永假属真实语义、或转写失真，静态不可分辨 /
  triage: [RESOLVABLE-BLACKBOX] 强制 → C-P1 语义规格期以 dc/asm 对照定案
- KL-P11-2 / 原 ls:1666 / 对数组符号 DAT_14013cb18 判 `==0` 恒假（疑把数据读取误成数组地址比较）/
  影响面：日志开关判断路径 / triage: [RESOLVABLE-BLACKBOX] → C-P1
- KL-P11-3 / 原 ls:13410 / 同上模式 / triage: [RESOLVABLE-BLACKBOX] → C-P1

## 场景 B 边界声明

本库风格达生产规范，但**行为未经系统对拍验证**（PRODUCTION_ROADMAP 场景 B 边界）。
TODO(verify) 共 426 处为诚实标注，分诊地图见 docs/triage_map.md（B5a 产出）。

## 附录：TODO(verify) 分诊 BLACKBOX / UNK 全量清单（B5b 自动生成）

| id | file:line | triage | rationale |
|---|---|---|---|
| KL-T001 | core_b2b.c:228 | RESOLVABLE-BLACKBOX | 被 Ghidra 丢弃的 %d 实参在 dc 内零线索(非 param 复用型)；需运行原版观察参数记录缓冲中该数值 |
| KL-T002 | core_b2d.c:1205 | RESOLVABLE-BLACKBOX | in_stack 入口残留值静态不可复原，需 PELOGON -dummy 流程黑盒验证；0x140120784 为 .rdata 地址可静态核 |
| KL-T003 | core_b2d.c:2101 | RESOLVABLE-BLACKBOX | wsprintfA/W vararg 被 Ghidra 截断，关机提示文案实参需运行原版观察 |
| KL-T004 | core_b2f.c:2276 | RESOLVABLE-BLACKBOX | Ghidra 截断全部 vararg 实参，精确实参列表需运行原版观察 LIST 进程输出格式 |
| KL-T005 | core_b2f.c:3210 | RESOLVABLE-BLACKBOX | extraout_EAX 为 Ghidra 寄存器残留，返回值高位真实性需运行观察盘符枚举结果 |
| KL-T006 | core_b2f.c:3806 | RESOLVABLE-BLACKBOX | CONCAT44/CONCAT71/extraout 残留值已被 Ghidra 丢弃，近似值需 RAM 盘命令黑盒比对 |
| KL-T007 | core_b2f.c:4490 | RESOLVABLE-BLACKBOX | in_stack_fffffffffffffe98>>32 为不可静态复原的入口栈残留，需 LOGO 对话框行为黑盒验证 |
| KL-T008 | core_b2f.c:4522 | RESOLVABLE-BLACKBOX | in_stack 高 32 位残留值静态无解，需运行观察绘制结果 |
| KL-T009 | core_b2f.c:4523 | RESOLVABLE-BLACKBOX | in_stack 高 32 位残留值静态无解，需运行观察绘制结果 |
| KL-T010 | core_b3d.c:514 | RESOLVABLE-BLACKBOX | %d 实参被 Ghidra 截断且上下文无候选值，需运行原版观察控件显示数字 |
| KL-T011 | core_b3i.c:265 | RESOLVABLE-BLACKBOX | htonl 缺失实参在 dc 内零线索；需运行原版抓取 0x30 字节 UDP 探测包观察第三字段实际值 |
| KL-T012 | core_b3k.c:139 | RESOLVABLE-BLACKBOX | 展开 %d 的实参来自未跟踪寄存器且函数内无候选变量, 按 0 补齐需运行 EXE 观察实际展开输出定案(C-P0 语料候选) |
| KL-T013 | core_b3r_d.c:1012 | RESOLVABLE-BLACKBOX | 其余 10 个 vararg 被 Ghidra 截断，PARTINFO 磁盘列表列序需运行原版观察 |
| KL-T014 | core_b3r_f.c:91 | RESOLVABLE-BLACKBOX | 定时器场景下写入命名变量的数值(interval? tick?)被 Ghidra 丢弃且 dc 无候选；需运行原版观察变量值 |
| KL-T015 | core_b3r_g.c:516 | RESOLVABLE-BLACKBOX | %d 实参寄存器被 Ghidra 连同同型两处一并丢弃, 候选(适配器记录数值字段)不唯一, 需运行 EXE 观察格式化输出定案(C-P0 语料候选) |
| KL-T016 | core_b3r_i28a.c:129 | RESOLVABLE-BLACKBOX | vararg 被 Ghidra 丢弃; 候选=刚解析的序号 iVar3 可推测但不唯一(可能为计数), 需运行 EXE 观察 &&xxx.Select 变量值定案(C-P0 语料候选) |
| KL-T017 | core_b3r_i28e.c:476 | RESOLVABLE-BLACKBOX | 简化是否无损取决于寄存器拼接的真实语义，静态难复原，需图标加载行为黑盒比对 |
| KL-T018 | core_b3r_i28e.c:988 | RESOLVABLE-BLACKBOX | 寄存器低 32 位真实值已被 Ghidra 丢弃，需运行观察控件初始化结果 |
| KL-T019 | core_b3r_i28e.c:989 | RESOLVABLE-BLACKBOX | 寄存器低 32 位真实值已被 Ghidra 丢弃，需运行观察控件初始化结果 |
| KL-T020 | core_b7c.c:1932 | RESOLVABLE-BLACKBOX | TODO 本身断言 decompiled 文本与二进制 else 配对存在错位嫌疑, dc 无法自证, 须对原 EXE 反汇编逐项复核 dash 开关族 |
| KL-T021 | core_b7c.c:3760 | RESOLVABLE-BLACKBOX | 虚表槽位内容属 .rdata 数据段, dc 未导出数据定义, 需读原 EXE 数据段核实其余槽是否为 0 |
| KL-T022 | core_b7c.c:5148 | RESOLVABLE-BLACKBOX | 虚表+0x108 槽真实目标函数地址存于 .rdata(dc 仅见 &PTR_FUN_14012c670 引用), 需查原 EXE 定位真实方法体 |
| KL-T023 | core_b7c.c:6357 | RESOLVABLE-BLACKBOX | 返回值为 unaff_RBX(Ghidra 未定), dc 无法给出语义, 需反汇编追踪或运行观察返回值 |
| KL-T024 | core_b7c.c:8287 | RESOLVABLE-BLACKBOX | movsxd rbx,[rsp+0xa0] 的栈槽语义 dc 以 unaff_RBX 呈现无法自证, 需反汇编确认 [rsp+0xa0] 写入点 |
| KL-T025 | core_b7c.c:8665 | RESOLVABLE-BLACKBOX | 第3/4栈参身份与色键像素偏移(&pHVar13->unused)是 Ghidra 类型推断缺口, dc 同样缺失, 需汇编核验 |
| KL-T026 | core_b8_remaining.c:580 | RESOLVABLE-BLACKBOX | 魔法除数按 roadmap 归黑盒语料；可先 python 解码 magic number 再以 MEM 命令样本验证 |
| KL-T027 | core_b8_remaining.c:841 | RESOLVABLE-BLACKBOX | 栈上 vararg 实参 Ghidra 不可见，需 IPC 连接串实际输出生成黑盒观察 |
| KL-T028 | core_b8_remaining.c:919 | RESOLVABLE-BLACKBOX | EXEC 命令行规格多于可见寄存器实参，栈参需运行原版抓取最终命令行 |
| KL-T029 | core_b8_remaining.c:926 | RESOLVABLE-BLACKBOX | proxy imdisk 规格/实参不齐，需运行观察生成的 imdisk 命令行 |
| KL-T030 | core_b8_remaining.c:973 | RESOLVABLE-BLACKBOX | -b/-s/-f 数值实参被截断，需运行观察 imdisk 最终参数 |
| KL-T031 | core_b8i.c:107 | RESOLVABLE-BLACKBOX | %s 实参被 Ghidra 丢弃且 dc 内无候选值线索；需运行原版注册菜单命令后观察生成的 '<X>.id' 变量名 |
| KL-T032 | core_b8i.c:135 | RESOLVABLE-BLACKBOX | 同 107 行疑点的代码位置：&%s.id 的 %s 实际值只能运行观察变量表定案 |
| KL-T033 | core_b8i.c:140 | RESOLVABLE-BLACKBOX | %d 是菜单 id 还是子项序号无法从 dc 判定(Ghidra 丢弃实参)；需运行观察 _COMMAND#N: 注册串 |
| KL-T034 | core_b8m.c:1049 | RESOLVABLE-BLACKBOX | Ghidra 未把 '@' 图像分支的隐藏栈参数建模进签名，dc 无该值线索；需运行原版构造图像对象观察绘制行为差异 |
| KL-T035 | core_b8m.c:1872 | RESOLVABLE-BLACKBOX | unaff_R15 为调用方寄存器残留值，dc 无法给出实际值；需运行观察返回值消费或反汇编调用点寄存器状态 |
| KL-T036 | core_b9_remaining.c:274 | RESOLVABLE-BLACKBOX | callee 返回 unaff_EDI 与 caller 读 extraout_EAX 双侧丢失真实值, 还原为 local_res10 目录标志属语义推断, 需黑盒观察路径判定行为定案(C-P0 语料候选) |
| KL-T037 | core_execline.c:309 | RESOLVABLE-BLACKBOX | script 指针+超大常量的语义合理性存疑(疑似 Ghidra 寄存器传播伪影)；%__THIS% 实际输出值需运行原版观察定案 |
| KL-T038 | core_execline.c:609 | RESOLVABLE-BLACKBOX | envHit 初值源自调用方 R13 寄存器残留(unaff)，dc 无法给值；需运行对比置位/清零两种行为路径 |
| KL-T039 | core_execline.c:775 | RESOLVABLE-BLACKBOX | 同 309 行疑点：__THIS 展开值的真实性需运行原版以脚本输出验证 |
| KL-T040 | core_execscript.c:81 | RESOLVABLE-BLACKBOX | atexit 回调 LAB_14011ab90 在 dc 仅见引用(dc:40691)无函数体, 需反汇编原 EXE 还原其行为 |
| KL-T041 | core_execscript.c:84 | RESOLVABLE-BLACKBOX | 同上: 清理函数体未反编译, dc 无解, 黑盒/反汇编定案 |
| KL-T042 | core_execscript.c:87 | RESOLVABLE-BLACKBOX | 线程回调原体仅为 LAB 标签(dc:40914 CreateThread 引用处), 函数体未反编译需反汇编还原 |
| KL-T043 | core_init.c:88 | RESOLVABLE-BLACKBOX | 两个 %06lu 实参与 QPC/启动时间的组合方式被截断，需运行原版读取 PECMDVER 值反推 |
| KL-T044 | core_init.c:90 | RESOLVABLE-BLACKBOX | 同上，参数丢失需黑盒观察 PECMDVER 输出定案 |
| KL-T045 | core_main.c:193 | RESOLVABLE-BLACKBOX | argc 经寄存器复用作 cmdLine 属 CRT 启动伪影，10 字节函数体的真实意图无法从 dc 判定；需运行原版带/不带参数对比行为 |
| KL-T046 | core_strbld.c:250 | RESOLVABLE-BLACKBOX | Ghidra 显示整数转浮点但真实汇编 movss(位解释)/cvtsi2ss(转换)伪代码不可辨, 需运行 EXE 构造 %f 用例观察输出定案(C-P0 语料候选) |
| KL-T047 | core_b2a.c:17 | UNK | 文件头约定描述, 非具体待验项 |
| KL-T048 | core_b2a.c:27 | UNK | extern 挂起段落标题, 惯例描述非待验项 |
| KL-T049 | core_b2a.c:185 | UNK | eax 残留无源(末调 FUN_14001995c 为 void), 调用方确在消费返回值; 黑盒需 PE 环境执行 FBWF 命令代价极高 -> KNOWN-LIMIT |
| KL-T050 | core_b2b.c:29 | UNK | 文件头风格约定说明，非具体待验证断言 |
| KL-T051 | core_b2b.c:40 | UNK | '未实现依赖 extern+TODO' 段落标题，泛指性说明 |
| KL-T052 | core_b2c.c:24 | UNK | 文件头约定描述, 非具体待验项 |
| KL-T053 | core_b2c.c:104 | UNK | extern 挂起段落标题, 惯例描述非待验项 |
| KL-T054 | core_b2c.c:694 | UNK | unaff_RBX 寄存器残留伪代码无源; 唯一调用点消费 RAX 而非 RBX; 黑盒需构造无冒号设备参数代价高 -> KNOWN-LIMIT |
| KL-T055 | core_b2d.c:24 | UNK | 文件头描述性样板注释，非具体验证点 |
| KL-T056 | core_b2d.c:27 | UNK | 文件头描述性样板注释，非具体验证点 |
| KL-T057 | core_b2d.c:57 | UNK | 区块头样板注释（extern 批量挂起），验证依赖后续波次还原 |
| KL-T058 | core_b2e.c:23 | UNK | 文件头描述性样板注释，非具体验证点 |
| KL-T059 | core_b2e.c:47 | UNK | 区块头样板注释（extern 批量挂起），验证依赖后续波次还原 |
| KL-T060 | core_b2f.c:23 | UNK | 文件头描述性样板注释（extern 挂起策略说明），非具体验证点，KNOWN-LIMIT |
| KL-T061 | core_b2f.c:26 | UNK | 文件头描述性样板注释（缺参/寄存器残留处理策略），非单点验证项 |
| KL-T062 | core_b2f.c:39 | UNK | 区块头样板注释（未实现依赖批量声明），验证依赖后续波次整批还原 |
| KL-T063 | core_b2f.c:1855 | UNK | 文件头描述性样板注释，非具体验证点 |
| KL-T064 | core_b2f.c:2329 | UNK | 文件头描述性样板注释（类型误标重写说明），非具体验证点 |
| KL-T065 | core_b2f.c:2647 | UNK | 文件头描述性样板注释，非具体验证点 |
| KL-T066 | core_b2f.c:3679 | UNK | 文件头描述性样板注释，非具体验证点 |
| KL-T067 | core_b3a.c:37 | UNK | 文件头描述性样板注释，非具体验证点 |
| KL-T068 | core_b3a.c:47 | UNK | 区块头样板注释（extern 批量挂起），验证依赖后续波次还原 |
| KL-T069 | core_b3b.c:37 | UNK | 文件头描述性样板注释，非具体验证点 |
| KL-T070 | core_b3b.c:48 | UNK | 区块头样板注释（extern 批量挂起），验证依赖后续波次还原 |
| KL-T071 | core_b3c.c:37 | UNK | 文件头约定说明, 非具体待验证断言 |
| KL-T072 | core_b3d.c:37 | UNK | 文件头描述性样板注释，非具体验证点 |
| KL-T073 | core_b3d.c:48 | UNK | 区块头样板注释（extern 批量挂起），验证依赖后续波次还原 |
| KL-T074 | core_b3e.c:37 | UNK | 文件头风格约定说明，非具体待验证断言 |
| KL-T075 | core_b3e.c:48 | UNK | '未实现依赖 extern+TODO' 段落标题，泛指性说明 |
| KL-T076 | core_b3f.c:39 | UNK | 文件头约定说明, 非具体待验证断言 |
| KL-T077 | core_b3g.c:39 | UNK | 文件头风格约定说明，非具体待验证断言 |
| KL-T078 | core_b3g.c:50 | UNK | '未实现依赖 extern+TODO' 段落标题，泛指性说明 |
| KL-T079 | core_b3h.c:39 | UNK | 文件头约定说明, 非具体待验证断言 |
| KL-T080 | core_b3i.c:39 | UNK | 文件头风格约定说明，非具体待验证断言 |
| KL-T081 | core_b3i.c:50 | UNK | '未实现依赖 extern+TODO' 段落标题，泛指性说明 |
| KL-T082 | core_b3j.c:39 | UNK | 文件头约定说明, 非具体待验证断言 |
| KL-T083 | core_b3k.c:19 | UNK | 文件头约定描述, 非具体待验项 |
| KL-T084 | core_b3k.c:29 | UNK | extern 挂起段落标题, 惯例描述非待验项 |
| KL-T085 | core_b3l.c:39 | UNK | 文件头约定描述, 非具体待验项 |
| KL-T086 | core_b3l.c:68 | UNK | extern 挂起段落标题, 惯例描述非待验项 |
| KL-T087 | core_b3m.c:39 | UNK | 文件头约定描述, 非具体待验项 |
| KL-T088 | core_b3m.c:52 | UNK | extern 挂起段落标题, 惯例描述非待验项 |
| KL-T089 | core_b3n.c:12 | UNK | 文件头约定说明, 非具体待验证断言 |
| KL-T090 | core_b3r_g.c:12 | UNK | 文件头描述性说明(标注约定总述), 非具体待验项 |
| KL-T091 | core_b3r_g1.c:12 | UNK | 文件头描述性总述(布局以字节缓冲重建的说明), 具体验证项在 L134/L138 |
| KL-T092 | core_b3r_g2.c:12 | UNK | 文件头描述性样板注释（归一化说明），非具体验证点 |
| KL-T093 | core_b3r_g2.c:15 | UNK | 文件头描述性样板注释，非具体验证点 |
| KL-T094 | core_b3r_g4.c:9 | UNK | 文件头 byte 指针归一化约定总述, 具体验证项在 L200 |
| KL-T095 | core_b3r_h2.c:20 | UNK | 文件头指引性描述(待核实项见函数内 TODO), 非独立待验项 |
| KL-T096 | core_b3r_i28c.c:131 | UNK | 文件头说明(预告深层驱动布局以下方 TODO 标注), 自身非具体断言 |
| KL-T097 | core_b7a.c:24 | UNK | 文件头约定说明, 非具体待验证断言 |
| KL-T098 | core_b7b.c:14 | UNK | 文件头约定说明, 非具体待验证断言 |
| KL-T099 | core_b7c.c:5 | UNK | 文件头说明文字(约定保留@0x与TODO标记), 非具体待验证断言 |
| KL-T100 | core_b8a.c:39 | UNK | 文件头描述性样板注释，非具体验证点 |
| KL-T101 | core_b8a.c:51 | UNK | 区块头样板注释（extern 批量挂起），验证依赖后续波次还原 |
| KL-T102 | core_b8b.c:19 | UNK | 文件头约定描述, 非具体待验项 |
| KL-T103 | core_b8b.c:34 | UNK | extern 挂起段落标题, 惯例描述非待验项 |
| KL-T104 | core_b8c.c:19 | UNK | 文件头约定描述, 非具体待验项 |
| KL-T105 | core_b8c.c:32 | UNK | extern 挂起段落标题, 惯例描述非待验项 |
| KL-T106 | core_b8d.c:39 | UNK | 文件头描述性样板注释，非具体验证点 |
| KL-T107 | core_b8d.c:53 | UNK | 区块头样板注释（extern 批量挂起），验证依赖后续波次还原 |
| KL-T108 | core_b8e.c:39 | UNK | 文件头约定说明, 非具体待验证断言 |
| KL-T109 | core_b8f.c:44 | UNK | 文件头风格约定说明，非具体待验证断言 |
| KL-T110 | core_b8f.c:84 | UNK | '未实现依赖 extern+TODO' 段落标题，泛指性说明 |
| KL-T111 | core_b8g.c:44 | UNK | 文件头约定描述, 非具体待验项 |
| KL-T112 | core_b8g.c:71 | UNK | extern 挂起段落标题, 惯例描述非待验项 |
| KL-T113 | core_b8h.c:39 | UNK | 文件头约定描述, 非具体待验项 |
| KL-T114 | core_b8h.c:59 | UNK | extern 挂起段落标题, 惯例描述非待验项 |
| KL-T115 | core_b8i.c:24 | UNK | 文件头风格约定说明，非具体待验证断言 |
| KL-T116 | core_b8i.c:48 | UNK | '未实现依赖 extern+TODO' 段落标题，泛指性说明无单一可验证命题 |
| KL-T117 | core_b8j.c:24 | UNK | 文件头约定说明(extern 挂起惯例), 非具体待验证断言 |
| KL-T118 | core_b8k.c:24 | UNK | 文件头风格约定说明，非具体待验证断言 |
| KL-T119 | core_b8k.c:76 | UNK | '未实现依赖 extern+TODO' 段落标题，泛指性说明 |
| KL-T120 | core_b8l.c:24 | UNK | 文件头风格约定说明，非具体待验证断言 |
| KL-T121 | core_b8l.c:61 | UNK | '未实现依赖 extern+TODO' 段落标题，泛指性说明 |
| KL-T122 | core_b8m.c:10 | UNK | 文件头风格约定说明(未实现依赖挂起策略)，非具体待验证断言，无可定案内容 |
| KL-T123 | core_b8m.c:48 | UNK | '未实现依赖 extern+TODO' 段落标题，泛指下方全部 extern 桩；具体定案需逐函数进行，标题本身无单一可验证命题 |
| KL-T124 | core_execmain.c:71 | UNK | '未实现(TODO(verify) 挂起)' 段落标题，泛指下方 extern 桩，无单一可验证命题 |
| KL-T125 | core_resdecode.c:36 | UNK | 未实现辅助段落标题, 惯例描述非待验项 |
| KL-T126 | core_script2.c:47 | UNK | 文件头描述性样板注释（简化范围声明），非具体验证点 |
| KL-T127 | core_scriptdep.c:23 | UNK | 文件头约定描述(extern挂起风格声明), 非具体待验项 |
| KL-T128 | core_scriptdep.c:48 | UNK | extern 挂起段落标题, 惯例描述非待验项 |
| KL-T129 | core_scriptdep.c:307 | UNK | local_c8/local_68/local_d0 初值 Ghidra 未显示伪代码无解; 黑盒需构造 flags&0x200 且 &9 调用代价高 -> KNOWN-LIMIT |
| KL-T130 | core_scriptrun.c:48 | UNK | 未实现挂起段落标题, 惯例描述非待验项 |
| KL-T131 | core_strbld.c:132 | UNK | 移植层 ABI 决策(Linux wchar_t=4B vs Win 2B)属已知限制, 非语义待验项 -> KNOWN-LIMIT |
| KL-T132 | core_strbld.c:143 | UNK | %Lf long double 大小 ABI 差异(Win 8B vs Linux 16B)属移植层已知限制 -> KNOWN-LIMIT |
| KL-T133 | restored_bodies.c:11573 | UNK | atexit 回调体未反编译, 静态无解; 黑盒需观察进程退出清理行为代价高 -> KNOWN-LIMIT |
| KL-T134 | restored_bodies.c:15237 | UNK | unaff_RDI 返回残留且无可见调用方, 伪代码无源; 黑盒需挂虚表消息链观察代价高 -> KNOWN-LIMIT |
| KL-T135 | unimplemented_stubs.c:380 | UNK | wave-INDIR 最小桩为计划内后续波次工作项（真体还原），非单点验证，KNOWN-LIMIT |
| KL-T136 | unimplemented_stubs.c:398 | UNK | wave-INDIR 最小桩为计划内后续波次工作项（真体还原），非单点验证，KNOWN-LIMIT |

## 附录：FUN_ 残余豁免清单（B1 口径管理）

M1 实测 4,791 处 FUN_14 引用 = 唯一拼写 737 个。豁免构成（REVIEW §128 处置已落账）：
- biz 46 全部处置：纯桩 SKIP×5 / INDIR 已移植×3 / KEEP_LOW 待运行时钉死×3 / 零引用 vtable 候选 ~22 / 已还原待别名 25f10
- crt 147 永久豁免（MSVC CRT 内部符号，C 阶段随 crt_shims 处理）
- 其余为 thunk/别名拼写噪声（stubs_common.h 跨 TU 原型区 + 三个桩文件）

## 附录：B 场景剩余工作登记（WIN 交接后 Linux 端续作）

| 批次 | 剩余项 | 载体 |
|---|---|---|
| B1 | local_xx 26,565 / param_N 13,888 / DAT_ 2,632 语义化（精修区 A 区优先: b7c/b8m/b8h/b2f ≈5k） | docs/B1_pipeline_status.md |
| B1 | 机械清扫剩余: set-but-used 31 + parentheses 57 + cast 类 ~90 | warning_census_b0.json |
| B3 | OBJ_* 宏 235 处推广（domain 头已就绪, 试点 core_thread 已完成） | include/domain/pecmd_domain.h |
| B4 | 家族合并 4 族 (Destroy/SendWindow/InitCore/Release) + 文件头模板统一 | PRODUCTION_ROADMAP §3.B.4 |
| B5b | STATIC 287 条 TODO 销号（triage_map §2 已给 dc_hint） | docs/triage_map.md |
| 还原期 | S2-fix (b7c 存储侧 ushort→int16_t) 行号漂移, 随 C-P1 符号性复核 | REVIEW 记录 |
