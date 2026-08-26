# 046_forx_enum — FORX 固定列表枚举（零覆盖动词首案）

## 断言要点
- `FORX W,a,b,c,ENVI LAST=%W%`：固定 4 元小列表枚举，循环体把当前元素赋给 LAST。
- `FIND $%LAST%=c`：派生确定性标志，末值应为最后一个元素 c。
- `ENVI BODY_DONE=YES`：体完成标记（若该行缺失 = 脚本中段夭折）。

## 方言置信度：ASSUMED
矩阵仅记「固定小列表枚举赋值」；逗号分隔参数沿用本库 `_SUB F,a,b` / `CALL F,X,Y` 实证风格。
若原版语法不同（如空格分隔或 `*` 通配前缀），录制会落入 `forx_other` 分支或中途夭折——golden 仍有效锁定行为，但须回填真实方言注记。

## 预期产物（字节级，LF/CRLF 由 diff_case 归一）
- `vars.txt`（主假设）: `CASE=046_forx_enum|LAST|c|R|forx_last_c|BODY_DONE`
  - 若 FORX 未迭代: `%LAST%` 展开为空 → `CASE=046_forx_enum|LAST||R|forx_other|BODY_DONE`（同为合法锁定态）
- `done.txt`: `OK`
- `exit.txt`: `exit=0`（若 FIND 传播 errorlevel 出现 `exit=2`，属既有正常形态，见 s12_golden_expansion.md §4.3）
- `stdout.txt`: 空

## fs 副作用
无（纯变量运算）。

## 人工核对步骤
1. 确认 vars.txt 中 LAST 捕获值 ∈ {c, 空} 且 R 分支与之自洽。
2. 确认 BODY_DONE=YES 在列（证明 FORX 行未挂起/未截断）。
3. 与 golden 三件套（done/exit/stdout）一致即验收。
