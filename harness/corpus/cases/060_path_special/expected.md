# 060_path_special — PATH 特殊路径指令（最小探针）

## 断言要点
- `PATH P`：矩阵仅记「特殊路径指令探针」（DispatchSpecialDirective），语义完全未证。
- 设计为**行为锁定探针**：不猜值、不分支，`%P%` 值捕获 + `BODY_DONE` 完成证明。

## 方言置信度：ASSUMED（最低档）
若 PATH 实为设置型动词（无回捞变量），P 恒空——golden 锁定空值仍有效。

## 预期产物
- `vars.txt`: `CASE=060_path_special|P|<路径或空>|BODY_DONE`
- `done.txt`: `OK`
- `exit.txt`: `exit=0`
- `stdout.txt`: 空

## fs 副作用
预期无；若录制时发现 PATH 有写盘行为，立即登记并评估是否降级该案。

## 人工核对步骤
1. 按 P 实际值回填语义注记（桌面？模块目录？空？）。
2. 若出现非预期 fs/系统副作用 → 在报告登记「本案降级/撤回」建议，勿直接删 golden。
