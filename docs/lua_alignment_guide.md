# Lua 对齐实施指南（基于 reverse-map 证据分级）

> 成文于 R27-d（2026-09-03）。前置：docs/lessons_validation_methodology.md（方法论）+
> HANDOVER_PROGRESS.md R26-a~R27-d（轮次史）+ pecmd_compat/docs/reverse-map.md（证据地图）。
> 本文回答：Lua 重写开工后，每一步怎么走、信什么、验什么。

---

## 0. 北极星与判定总则

- **北极星**：Lua 实现与原版 PECMD.EXE 行为等价（非 GUI 部分）。
- **判定总则**：任何命令的"对齐完成"= 该命令的全部语料（含就地补的）在原版 golden
  上值级 PASS。msvc C 只作语义歧义时的参考，不作基准。
- **证据分级（reverse-map）**：
  - `语料验证` = 可直接照语料实现（行为已锁定）；
  - `真体直移` = 实现可信、行为待语料（先补语料再实现或边实现边补）；
  - `真体+注` = 有已知偏差注记，读注记决定；
  - `桩/缩水` = 以 dc/实测为准（reverse-map F 段外的桩要查链接映射，见方法论 §4）。

---

## 1. 开工顺序（按语料覆盖从厚到薄）

| 阶段 | 命令族（建议） | 理由 |
|---|---|---|
| 1 | ENVI/FIND/IFEX/IFEX 文件族 + 字符串族（STRL/LSTR/SSTR/…） | 语料最厚、无设备副作用、纯变量/文本 |
| 2 | 文件 IO 族（FILE/READ/WRIT/SIZE/FEXT/DIR） | 语料厚；WRIT 基础形式安全面已确证（39 变体） |
| 3 | FORX（默认引擎深路径边补边做）+ LOOP/TEAM | 引擎语义已定案（R27-a） |
| 4 | GETF/PUTF/MOUN/PAGE 等设备系 | 语料已覆盖基本形态（GETF 065）；MOUN/PAGE 建议虚机 |
| 5 | PART 族 | **最后**：需 VHD 夹具 + hdN 形态（D-28 遗留） |

每个命令族开工前：读 reverse-map 该行动 + re-notes 对应文件 → 列出"待实测"项
→ 建/跑该族语料 → 实现 → 值级对拍。

---

## 2. 实现原则

1. **对着行为写，不对着 C 翻译**。msvc C 里为过 Ghidra/MSVC 的痕迹
   （双游标、SBORROW、指针整形互转、CONCAT 宏展开）是纯噪音。要的是 dc 的语义
   描述 + re-notes 的功能点清单 + 语料的输入输出对。
2. **值/串语义以原版实测为准**：有语料照语料；语料空白的形态，用探针
   （tools/_r27*.py 同款：隔离目录 + EXEC echo 回捞 + 双引擎对照）先钉方言再写。
3. **错误码与退出码是行为的一部分**：原版"怪"形态（FORX NL 零迭代 + exit=2、
   PART 无输出 + 特定 exit）照实对齐，不要"修正"。
4. **不实现的（登记即可）**：GUI 全族（AHK 路线）；WRIT 崩溃触发面（未定义行为区，
   防御处理——先验参数再写，见 D-26）；PART 写族在 hdN 形态修好前只实现 list。

---

## 3. 语料纪律

- **补证发生在有真实消费时**：Lua 实现跑到未覆盖形态 → 就地补语料（建案 → orig
  录 golden → msvc 双验归绿 → 你的实现再过同一案）。不为覆盖率预先铺案。
- 新语料模板照 067/068/069/070（manifest + main.pecmd + expected.md）：
  - 纯变量案：vars_val 值级锁定；
  - 设备案：`"vhd": true`（PART）/虚机（MOUN 类）；
  - 环境敏感案：flaky=true + 注记（如 069 全盘列表）。
- 语料只锁**已定案的形态**；探针（不进 corpus）试未定形态。

---

## 4. 工程接入

- harness 增加 Lua backend：run_case.py 现有 orig/msvc 双后端结构上再加一个
  `--exe lua`（label "lua"），产物路径指向你的 Lua 入口（如 lua_pecmd.exe 包装）。
  判定零改动——diff_case 已是"golden vs 任一后端"的通用对拍。
- 建议先做一个"空壳 Lua 后端"跑通 001_envi_smoke 的 PASS 再铺开
  （后端接入本身先验证，再谈命令实现）。

---

## 5. 常见坑速查（全部来自 R27 实证）

| 坑 | 症状 | 处理 |
|---|---|---|
| 信了 dc 行序 | "解析后被清零"这类假因果 | 回汇编/活体（方法论 §2.1/§3） |
| 信了文档旧状态 | chunks 说"FORX 是桩"（R26-e 前） | 信 reverse-map 最新行 + 实测 |
| 值级 vs 存在级 | vars 有值但值不同仍 PASS | 一律 vars_val 值级判 |
| 环境盘污染 golden | 列表多一个残留 VHD | 测试后 detach+删盘；录制前查 Get-Disk |
| attach 后立刻跑 | 列表少盘（就绪未完成） | 轮询等盘号 + 稳一拍 |
| 改共享桩 | 远端消费方时序崩（012） | 改链不改桩；查 stubs 惯例注释 |
| 宽格式串丢参 | wvsprintfW AV | 格式串算 %spec 数；查汇编参数 |
| C 转义丢反斜杠 | 设备路径打不开 | 宽字面量按字符数核对 dc |

---

## 6. 每轮收尾清单

1. 该族命令全部语料值级 PASS（含就地补的）；
2. 新方言/新坑登记进本指南或 re-notes；
3. reverse-map 对应行证据列升到"语料验证"；
4. compat 同步 + HANDOVER 轮次登记。

---

## 7. 参考文件索引

- 证据地图：pecmd_compat/docs/reverse-map.md（E/F 段 + 每族行）
- 逆向笔记：pecmd_compat/docs/re-notes/*.md（每族功能点 + 待实测 + 方言）
- 官方文档分块：pecmd_compat/chunks/001-005（功能点 + Lua 重构提示，核对用）
- 方法论：docs/lessons_validation_methodology.md
- 轮次史：HANDOVER_PROGRESS.md（R26-a 起每轮实质/遗留）
