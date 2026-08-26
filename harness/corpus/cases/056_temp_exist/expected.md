# 056_temp_exist — TEMP 目录回捞

## 断言要点
- `TEMP T`：取临时目录路径；`IFEX %T%` 断言该目录真实存在（目录形态的存在性检查为 ASSUMED，013 仅证文件形态）。
- 标志 R 与值 `%T%` 双通道。

## 方言置信度：ASSUMED（中等）
矩阵记「TEMP 目录变量回捞」（TempPathCommand）。参数序 `结果变量` 单参为假设。

## 稳定性注记
同机同用户 TEMP 路径（通常 `C:\Users\<u>\AppData\Local\Temp` 或 PE 环境 `\temp`）跨进程/跨日稳定，
值捕获可对拍；若录制与后续 msvc 对拍之间 TEMP 变更导致 diff，按 masks 待办登记（不阻塞录制）。

## 预期产物
- `vars.txt`: `CASE=056_temp_exist|T|<绝对临时目录路径>|R|temp_exist|BODY_DONE`
- `done.txt`: `OK`
- `exit.txt`: `exit=0`
- `stdout.txt`: 空

## fs 副作用
无新文件（TEMP 目录由系统管理，本案只读探测）。

## 人工核对步骤
1. 核对 T 为存在的绝对路径且以 `\` 结尾或纯目录形态。
2. R=temp_exist 必须成立（否则 TEMP 实现返回了无效路径——FAIL 复查）。
