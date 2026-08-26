# 063_sstr_search — SSTR 子串搜索

## 断言要点
- `ENVI S=abcdef` + `SSTR P,%S%,cd`：期望返回 1-based 位置 3；`FIND $%P%=3` 派生标志。

## 方言置信度：ASSUMED（低）
SSTR 与 STRL/RSTR/LSTR 同属 §4.2 写回组字符串家族，§5 无独立行项。若实际语义是「截取子串」而非「位置」，
P 会是 `cdef` 类形态——锁定后回填。

## 预期产物
- `vars.txt`: `CASE=063_sstr_search|P|3|R|pos3|BODY_DONE`
  - 替代合法落点: P=cdef / 0 / 空 + `R|pos_other`
- `done.txt`: `OK`
- `exit.txt`: `exit=0`（或既有 errorlevel 形态 2）
- `stdout.txt`: 空

## fs 副作用
无。

## 人工核对步骤
1. 按 P 实际值判定语义（位置 vs 截取），与 049/062 结果合并成字符串家族方言结论。
2. R 与 P 自洽即可验收。
