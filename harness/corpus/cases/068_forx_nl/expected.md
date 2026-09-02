# 068_forx_nl — FORX /NL: /delims: 行迭代变体矩阵（首案，值级锁定）

## 断言要点（原版实测 2026-09-03 定案）

| 变体 | 命令 | 原版行为 | 语义 |
|---|---|---|---|
| A1 | `FORX /NL:# SRC,W,...` | 零迭代，exit=2 | `/NL:` 置 opts=0xE → **VarLookup 路径**; ENVI 全局变量不在 script VarLookup 表 → 失败退出 |
| A2 | `FORX /delims:# SRC,W,...` | 一轮, W=`SRC` 字面 | **`/delims:` 不置 bit2 (dc:107957-107958 不写 uVar33)** → 不查变量, 源=第一段字面; 无 `#` → 整串一轮 |
| A3 | `FORX /delims:, SRC2,W,...` | 一轮, W=`SRC2` 字面 | 同 A2 (逗号分隔源未触发切分 — 分隔符仅在变量值内容中有效) |
| A4 | `FORX /NL: SRC,W,...` | 零迭代，exit=2 | 同 A1 |
| A5 | `FORX /v SRC,W,...` | 零迭代，exit=2 | v 模式 opts=5 → VarLookup 路径同样失败 |

## 方言定案（含 R26-e 登记偏差修正）
- **`/delims:` 与 `/NL:` 语义不对称**（dc:107934 NL: 置 `uVar33=6`; dc:107957-107958
  delims: 直接 goto 共享尾不改 uVar33）——i28b 移植曾把两者都写成 `opts=6|10`，
  R27 语料 068 原版实证后修正 (i28b delims: 分支 → `opts = opts | 10U`)。
- **ENVI 全局变量不可作 NL 模式迭代源**（VarLookup 表隔离）；真源须为脚本内变量。
  "NL 遍历变量内容" 的可达路径待下轮以脚本内变量（_SUB/TEAM 上下文）扩案验证。

## 预期产物（字节级）
- `vars_val.txt`: `CASE=068_forx_nl|A1=start|A2=start_SRC|A3=start_SRC2|A4=start|A5=start|BODY_DONE=YES`
- `done.txt`: `OK`; `exit.txt`: `exit=2` (VarLookup 失败码传播, 既有正常形态)
- `stdout.txt`: 空

## fs 副作用
无（纯变量运算）。
