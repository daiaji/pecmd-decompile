# 055_sed_generate — SED 文本生成

## 断言要点
- `SED G,sed55seed`：3 字母分支最小探针；期望 G 收到生成文本。
- 不设 ok/bad 分支（生成规则完全未知），以 `%G%` 值捕获锁定行为。

## 方言置信度：ASSUMED（低，纯探针）
矩阵仅记「文本生成正例」。若参数序/语义不同，G 为空或字面量——均为有效锁定态。

## 预期产物
- `vars.txt`: `CASE=055_sed_generate|G|<生成串或空>|BODY_DONE`
- `done.txt`: `OK`
- `exit.txt`: `exit=0`
- `stdout.txt`: 空

## fs 副作用
无。

## 人工核对步骤
1. 按 G 实际值回填语义注记（模板展开？原样？空？）。
2. BODY_DONE=YES 在列即证明 SED 行未挂起。
