# 051_size_known — SIZE 已知文件尺寸

## 断言要点
- `WRITE %F%,data51` 自建输入（runner 每次清空 out 后本案自建，无跨案依赖）。
- `SIZE Z,%F%` 取尺寸；`IFEX $%Z%>0` 派生确定性标志（不依赖具体字节数）。
- Z 的值本身确定：`data51`=6 字节；若 WRITE 以 CRLF 收尾（006 案已证该形态存在）则为 8。录制锁定其一。

## 方言置信度：ASSUMED（中等）
矩阵记「文件尺寸→变量」（GetPathSizeToVar，函数名即暗示 结果变量←路径 参数序）。

## 预期产物
- `vars.txt`（主假设）: `CASE=051_size_known|Z|6|R|size_pos|BODY_DONE`
  - 同等合法落点: `Z|8`（CRLF 形态）；异常时 `R|size_neg`
- `done.txt`: `OK`
- `exit.txt`: `exit=0`
- `stdout.txt`: 空

## fs 副作用
- 创建 `C:\pectest\out\s51_data.txt`（下轮运行被 runner 清除，不入 golden，与 041 先例一致）。

## 人工核对步骤
1. 核对 Z ∈ {6,8}（或记录实际值回填 WRITE 尾字节形态结论）。
2. R=size_pos 必须成立（Z≤0 即实现缺陷信号，标 FAIL 复查）。
