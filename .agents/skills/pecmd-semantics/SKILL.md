---
name: pecmd-semantics
description: PECMD 语义核对四重真源法 + 关键地址->符号地图 (修还原失真时防臆造)
whenToUse: 修复还原 bug、补桩真体、或需要确认原版某函数/命令的真实行为时
---

# 语义核对四重真源法

纪律: **任何"原版是这样做的"断言必须能落到四个真源之一**, 否则登记 divergences 而非臆造。

0. **Ghidra MCP（最快路径, 优先用）**: 桥接常驻(project=PECMD), 双程序同开
   `/pecmd_msvc.exe`(重建产物) 与 `/PECMD.exe`(原版, base 0x140000000)。
   - `switch_program` 切换; 多程序同开时工具调用**必须带 program 参数**。
   - `disassemble_function` / `decompile_function` 直接拿调用点现场——Ghidra 文本伪影
     (变参丢弃/unaff_ 寄存器残留)在反汇编里都能定案。
     实例: wsprintfW 伪影实参 @0x140008110、unaff_R13D 各 caller 进入 0x14004c0bc 前的 r13。
   - `get_function_callers` / `get_xrefs_to` 秒级画调用链, 免去 dc 全文 grep。
   - 注意: dc 文本(真源1)是它的导出物, 两者冲突时以反汇编为准并回写勘误。

1. **decompiled.c** (`reference\decompiled.c`, ~18 万行 Ghidra 原始输出)
   - 按地址 grep: `Select-String -Path decompiled.c -Pattern 'FUN_14001e6bc|14001e6bc'`
   - 每个函数头有 `/* ========== FUN_xxx @ addr  size=N ========== */` 锚点
   - 注意 Ghidra 噪声: "jumptable 警告"/"寄存器残留参数"(如 wsprintfW 参数丢失)/间接跳转当调用

2. **原版 EXE 二进制** (`C:\pectest\PECMD.EXE`)
   - utf-16 字符串扫描判定命令/API 存在性:
     `data.find('WRITE'.encode('utf-16-le')) >= 0`
   - 版本: FileVersion 201201.88.05.94, mdyblog 修改版 — 与网上常见 PECMD 手册可能有差异,
     以 EXE 实测为准 (案例: 此版本无 WRITE; TEXT 是屏幕显示)
   - 导出表/资源用 dumpbin /exports、/headers

3. **真机对照** (`run_case.py --exe orig`): 原版与产物的 vars.txt/stdout/exit 对拍。
   原版在 PE 环境设计, 注册表/服务/网络类行为与桌面 Windows 有差 → 差异记 docs/divergences.md。

## 关键地址速查 (已定案)

| 地址 | 符号 | 备注 |
|---|---|---|
| @0x1400050c8 | RunBootScriptInFiber | fiber 封装→最终调 @0x14004eb34; 已改为顺序转发 |
| @0x14004eb34 | PECMD_RunStartupScript | ExecCmd 主流程 (core_script2.c) |
| @0x140031454 | PECMD_RunCommand | 命令行调度 (core_scriptrun.c, 部分 TODO) |
| @0x14001e6bc | FUN_14001E6BC | 变量写入器 (core_var2.c) — T1 OOM 嫌疑区 |
| @0x1400629b8 | FUN_1400629B8 | 设变量分发 (&前缀→E6BC, 否则→D534 环境直写) |
| @0x14011a8b0 等 | CS 初始化器组 | 原版经 .CRT$XCU 表 main 前执行; 还原丢失已补 (core_globals.c) |
| @0x140017034 | main (CRT) | 入口链 mainW@0x140009ccc → MainB → Main1 |

## 变量写入链结构 (T1 相关)

```
FUN_1400629B8(script,key,value)
  ├─ key 无 '&' → FUN_14005D534(script,key,value) = SetEnvironmentVariableW
  └─ key 有 '&' → FUN_14001E6BC(script,key,value,caplen=-1)
        ├─ VarLookup 找到 node → AllocString(node+8, len+2) ← len 未初始化嫌疑 (caplen>=0 路径)
        └─ node==NULL → calloc(0x20) → NewVarNode → HeapRealloc(数组扩容)
```

`g_Script` 布局: +0 = 项数组指针, +8 = 项数 int; 节点: +8 数据指针, +0x18 长度, +0x1f 标志位。
