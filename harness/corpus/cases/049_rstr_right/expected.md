# 049_rstr_right — RSTR 右截取

## 断言要点
- `ENVI S=abcdef` + `RSTR R=%S%,2,3`：与已实证 LSTR(029) 完全同形的参数布局（`结果=%源%,p1,p2`），方向为右。
- `FIND $%R%=def` 派生标志。主假设：取末尾 3 字符 = `def`（p1/p2 中生效者按右侧计数）。

## 方言置信度：ASSUMED
p1=2/p2=3 的确切含义（起点 vs 长度、从左数 vs 从右数）无在库证据；无论落点为何，值捕获 `%R%` 都把真实行为字节锁进 golden。

## 预期产物
- `vars.txt`（主假设）: `CASE=049_rstr_right|R|def|F|rstr_def|BODY_DONE`
  - 替代合法落点: R=cde / bcd / 空 等任意确定形态 + 对应 `F|rstr_other`
- `done.txt`: `OK`
- `exit.txt`: `exit=0`（或既有 errorlevel 形态 2）
- `stdout.txt`: 空

## fs 副作用
无。

## 人工核对步骤
1. 记录 R 捕获值并回填到本文件的「真实方言」注记（供后续 STRL/SSTR 案参考同族语义）。
2. F 与 R 自洽即可验收。
