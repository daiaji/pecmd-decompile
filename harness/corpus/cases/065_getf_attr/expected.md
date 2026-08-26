# 065_getf_attr — GETF 文件属性

## 断言要点
- 自建文件后 `GETF G,%F%` 取属性；`IFEX $%G%>0` 派生标志。
- **刻意不捕获 G 原值**：属性串大概率含创建/修改时间戳，逐字节必漂移（masks.conf 为空且禁改）。
  标志化后：G 为数值形态 → 恒 `getf_num`；为时间串形态 → 数值比较走假支 → 同样恒 `getf_other`。两种实现口径各自确定。

## 方言置信度：ASSUMED
矩阵记「文件属性→变量」（GetfReadData core_b7c）。参数序 `结果,路径` 为假设。

## 预期产物
- `vars.txt`: `CASE=065_getf_attr|R|getf_num|BODY_DONE`
  - 替代合法落点: `R|getf_other`（属性为字符串/日期形态——同为锁定态）
- `done.txt`: `OK`
- `exit.txt`: `exit=0`
- `stdout.txt`: 空

## fs 副作用
- 创建 `C:\pectest\out\s65_attr.txt`（下轮清除）。

## 人工核对步骤
1. 录制当场手工打印 G（临时把 G 加进 vars 重跑一次亦可）记录形态，回填注记。
2. 后续若 masks.conf 增加 `[0-9]{...}` 时间戳掩码，登记「升级原值捕获」待办。
