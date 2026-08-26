# 062_strl_len — STRL 字符串长度

## 断言要点
- `ENVI S=abcdef` + `STRL L,%S%`：期望 L=6；`FIND $%L%=6` 派生标志。
- STRL 在矩阵 §5 无行项（仅 §4.2 写回组宿主清单可证其存在）——本条按任务规则标注 ASSUMED 并以 harness/矩阵交叉印证为据。

## 方言置信度：ASSUMED（低）
参数序 `结果,源` 沿用同族 LSTR 实证布局；若语义不同（如返回字节数含终止符=7），落入 `len_other` 锁定真实值。

## 预期产物
- `vars.txt`: `CASE=062_strl_len|L|6|R|len6|BODY_DONE`
  - 替代合法落点: L=7（含 \0 计数）/空 + `R|len_other`
- `done.txt`: `OK`
- `exit.txt`: `exit=0`（或既有 errorlevel 形态 2）
- `stdout.txt`: 空

## fs 副作用
无。

## 人工核对步骤
1. 按 L 实际值判定计数口径（字符数 vs 字节数），回填注记。
2. R 与 L 自洽即可验收。
