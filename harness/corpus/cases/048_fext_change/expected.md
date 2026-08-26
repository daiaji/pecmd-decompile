# 048_fext_change — FEXT 扩展名替换

## 断言要点
- `FEXT M,%N%,jpg`：把 `s48pic.old` 的扩展名换成 jpg，期望结果 `s48pic.jpg`。
- `FIND $%M%=s48pic.jpg` 派生标志；处理函数名 SetFileNameExtension 强烈暗示纯字符串操作（不动真实文件）。

## 方言置信度：ASSUMED（中等偏高）
参数序 `结果,源串,新扩展名` 为假设；若原版参数序不同（如新扩展名在前），落入 `fext_other`，仍锁定行为。

## 预期产物
- `vars.txt`（主假设）: `CASE=048_fext_change|M|s48pic.jpg|R|fext_jpg|BODY_DONE`
- `done.txt`: `OK`
- `exit.txt`: `exit=0`（或既有 errorlevel 形态 2）
- `stdout.txt`: 空

## fs 副作用
无。

## 人工核对步骤
1. 核对 M 捕获值：应为替换扩展名后的完整文件名（或空/原样——记录即真相）。
2. R 分支与 M 自洽即可验收。
