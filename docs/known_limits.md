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
