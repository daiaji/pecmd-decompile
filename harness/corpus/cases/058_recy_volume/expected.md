# 058_recy_volume — RECY 回收站卷查询

## 断言要点
- `RECY V`：查询回收站所在卷（QueryRecycleBinVolume）；`IFEX %V%` 断言返回路径存在性。
- 空返回时 `IFEX` 走假支 → `recy_none`，同样确定性。

## 方言置信度：ASSUMED
矩阵记「回收站卷查询→变量」。单参 `结果变量` 为假设。

## 稳定性注记
回收站卷符属机器级半稳定值（清空回收站/多盘配置变化才变）。录制与 msvc 对拍同机进行时通常一致；
若 diff 落在 V 上且期间无回收站配置变更，按环境漂移登记而非判 FAIL。

## 预期产物
- `vars.txt`: `CASE=058_recy_volume|V|<盘符或路径>|R|recy_exist|BODY_DONE`
  - 替代合法落点: V 空 + `R|recy_none`
- `done.txt`: `OK`
- `exit.txt`: `exit=0`
- `stdout.txt`: 空

## fs 副作用
无（只读查询；不动回收站内容）。

## 人工核对步骤
1. 核对 V 形态（如 `C:\` 或 `\??\C:\` 类卷路径）。
2. R 与 V 自洽即可验收。
