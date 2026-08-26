# 061_logs_smoke — LOGS 开日志冒烟

## 断言要点
- `LOGS C:\pectest\out\s61.log`：以合法路径开启日志后正常走到尾声（对照 REVIEW §S7 的 AV 形态——
  那是收尾对 `LOGS\0\n\n` 类畸形输入的行为，本案例不构造该形态）。
- `IFEX` 断言日志文件落盘。

## 方言置信度：ASSUMED（中等）
单参数路径形式按常识假设；日志内容含时间戳但**不入 golden**（runner 只回捞 vars/done），故无掩码问题。

## 预期产物
- `vars.txt`: `CASE=061_logs_smoke|R|logs_open|BODY_DONE`
  - 替代合法落点: `R|logs_none`
- `done.txt`: `OK`
- `exit.txt`: `exit=0`
- `stdout.txt`: 空

## fs 副作用
- 创建 `C:\pectest\out\s61.log`（下轮清除）。

## 人工核对步骤
1. 录制当场看 s61.log 是否有内容（仅确认开写成功，不比对内容）。
2. **红线**：若录制中出现崩溃/挂起，立即停止重试并登记「LOGS 收尾 AV 复现」，该案转 SKIP。
