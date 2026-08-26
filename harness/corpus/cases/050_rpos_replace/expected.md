# 050_rpos_replace — RPOS 变量内查找/替换

## 断言要点
- `ENVI S=abc123abc` + `RPOS T,%S%,123,X9`：把源串中 `123` 替换为 `X9`，期望 `abcX9abc`。
- `FIND $%T%=abcX9abc` 派生标志。

## 方言置信度：ASSUMED
矩阵仅记「变量串替换」；参数序 `结果,源,旧,新` 为假设。若 RPOS 实为「返回位置」语义，捕获值会是数字——同为有效锁定态，人工核对时按实际值回填语义注记。

## 预期产物
- `vars.txt`（主假设）: `CASE=050_rpos_replace|T|abcX9abc|F|rpos_hit|BODY_DONE`
  - 替代合法落点: T=位置数字（如 4）/ 原串 / 空 + `F|rpos_other`
- `done.txt`: `OK`
- `exit.txt`: `exit=0`（或既有 errorlevel 形态 2）
- `stdout.txt`: 空

## fs 副作用
无。

## 人工核对步骤
1. 按 T 捕获值判定真实语义（替换 / 位置 / 无操作），回填本文件注记。
2. F 与 T 自洽即可验收。
