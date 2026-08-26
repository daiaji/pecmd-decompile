# 064_exec_explicit — EXEC 显式断言（覆盖升级案）

## 断言要点
- `EXEC =cmd /c echo exec64^>out\s64_exec.txt`：`=` 前缀通道（HANDOVER_WIN T2 已证三变体可用）。
- `IFEX` 断言回声文件落盘——EXEC 从「每案尾声 incidental 使用」升级为「有独立判定」的已覆盖动词。

## 方言置信度：实证（高）
与 runner 尾声同通道同转义风格（^>），是本批唯一非 ASSUMED 的新动词案。

## 预期产物
- `vars.txt`: `CASE=064_exec_explicit|R|exec_ok|BODY_DONE`
- `done.txt`: `OK`
- `exit.txt`: `exit=0`
- `stdout.txt`: 空

## fs 副作用
- 创建 `C:\pectest\out\s64_exec.txt`，内容应为 `exec64`+行尾（下轮清除）。

## 人工核对步骤
1. R=exec_ok 必须成立；exec_fail 说明 EXEC 主链回归——立即标 FAIL 复查（此案是最敏感的冒烟）。
2. 可选：录制当场确认 s64_exec.txt 内容为 exec64。
