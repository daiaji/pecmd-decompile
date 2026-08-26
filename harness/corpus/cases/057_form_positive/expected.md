# 057_form_positive — FORM 磁盘剩余空间

## 断言要点
- `FORM V,C:` 取 C: 剩余空间；`IFEX $%V%>0` 派生确定性标志。
- **刻意不把 V 写进 manifest.vars**：自由字节数随任何写操作漂移，原值捕获必破坏逐字节比对（masks.conf 为空、本代理禁改）。
  标志位在「任意正数」域内恒真，跨时间稳定。

## 方言置信度：ASSUMED
矩阵记「盘剩余空间→变量(掩码对拍)」并自注需掩码——本案用标志化绕开掩码依赖。参数序 `结果,盘符` 为假设。

## 预期产物
- `vars.txt`: `CASE=057_form_positive|R|form_pos|BODY_DONE`
  - 若实现返回非数值/失败: `R|form_other`（锁定态，登记复查）
- `done.txt`: `OK`
- `exit.txt`: `exit=0`
- `stdout.txt`: 空

## fs 副作用
无（只读查询）。

## 人工核对步骤
1. R=form_pos 即验收；form_other 时人工用 `Get-PSDrive C` 对照确认是否实现缺陷还是方言差异。
2. 后续若 masks.conf 增加「大数字掩码」规则，可升级为原值捕获案（登记为待办，不在本批）。
