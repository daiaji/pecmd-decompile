# B1 执行配方与状态 — local_xx / param_N / DAT_ 语义化流水线

> v4 定位注记：B1 语义化从"场景 B 门禁"改为"参考库可读性"推进项（ROADMAP v4 阶段 P3）；配方不变。


> 本文档是 B1 批次的**可续作交接物**。B1 总量约 40k 处标识符语义化，是场景 B 的最大单项，
> 设计为多会话子代理流水线执行。每完成一个文件批即 commit，双绿门不变。

## 0. 当前状态（本会话结束时）

| 流水线 | 总量 | 已完成 | 状态 |
|---|---|---|---|
| FUN_→PECMD_ | — | 1,594 符号 | ✅ 还原期已完成 |
| 机械警告清扫 | 349→328 条 | unused 类 ~36 条 | 🟨 简单模式已清；parentheses(57)/cast类(~90) 留子代理批 |
| param_N → 语义形参 | 13,888 | 0 | ⬜ 未启动 |
| local_xx → 语义局部 | 26,598 | 0 | ⬜ 未启动 |
| DAT_/PTR_ → g_ | 2,417 (core:1,196) | 0 | ⬜ 未启动 |

## 1. 精修区定义（改名预算只花在这里）

见 docs/triage_map.md §2：
- **A 区**（优先）：src/ui/core_b7c.c, core_b8m.c, core_b8h.c, src/commands/core_b2f.c
- **B 区**：src/lang/core_script*.c, core_srparse.c, core_execline.c, src/runtime/core_scriptdep.c, src/commands/core_b2c/b2d/b2e.c
- **C 区**：其余 src/**/*.c
- **桩区豁免**：restored/win32_api/unimplemented/crt_shims 只做编译所需

## 2. 子代理单文件批提示词模板

```
你是 PECMD 还原项目 B1 语义化代理。工作目录 refactored/。
目标文件：src/<zone>/<file>（独占编辑权）。

任务：
1. 对文件内全部 Ghidra 拼写标识符做保守语义化：
   - local_<hex> / uVar<N> / iVar<N> / lVar<N> / puVar<N> → 按用途命名
   - param_N → 按形参角色命名
   - (void)死变量直接删除（编译器兜底）
2. 保守字典（禁止臆造强语义名）：
   p/pp=游标, buf=缓冲, cnt/count=计数, flags=标志,
   ctx/hObj=对象指针, hXxx=句柄, psz/pwsz=字符串指针,
   tmp=暂存, ret=返回值暂存
3. 无法判定用途的保持原名并加 /* keep-g */ 注释
4. 同批清理该文件的 -Wunused-variable/-Wunused-label 警告

验收（逐项执行并附输出）：
- gcc -std=c99 -Wall -Wextra -Wno-unused-parameter -Wno-implicit-function-declaration
  -Iinclude -fsyntax-only <file> 通过
- bash build.sh 全绿
- 完整链接 exit 0:
  gcc -std=c99 -w -Wno-implicit-function-declaration -Iinclude \
      $(find src -name '*.c') restored_bodies.c win32_api_stubs.c \
      unimplemented_stubs.c crt_shims.c -lm -o /tmp/opencode/link_test
- M2/M3 计数下降值写入报告

【自主处理纪律条款】（tools/subagent_autonomy_clause.md 原文附上）
```

## 3. DAT_/PTR_ → g_ 迁移剧本

1. 从 `grep -n 'DAT_14\w+' <file>` 列出符号；
2. 每个符号查 tools/pe_registry.json（1082 真值库）：有语义者起 `g_<语义名>` 并登记
   rename_map.json；无语义者保持 DAT_ 名并在 known_limits 附录登记 KNOWN-LIMIT；
3. 类型存疑者一律 `void *` + KNOWN-LIMIT（禁止猜类型）；
4. 定义统一收口到 src/kernel/core_globals.c（已有分区注释模板）。

## 4. 批次划分建议（后续会话直接照做）

| 会话 | 目标 | M2/M3 降幅预期 |
|---|---|---|
| S+1 | A 区 4 文件（~5,075 local） | -19% |
| S+2 | B 区 8 文件（~550）+ DAT_ core 部分 | -21% |
| S+3..N | C 区按密度排序（core_b3_remaining 6k 最大单项） | 递减 |

## 5. 红线提醒

- 一切改动语义保持；凡编译器管不住的改名（如局部变量地址被存入全局槽）禁做；
- 单编辑者纪律：同一文件同时只允许一个代理；
- 每批双绿门 + git add 明确清单（禁止 add -A 扫入半成品——本会话教训见 git log）。
