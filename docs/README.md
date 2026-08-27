# docs/README.md — 文档索引

> 代理作业契约入口在仓库根 `AGENTS.md`；本目录只放档案与规程细则。

## 规程类（活性文档，随裁决更新）

| 文件 | 内容 | 状态 |
|---|---|---|
| WINDBG_MCP_ISSUES.md | windbg MCP 病理 A–H 登记、SOP、R19 误诊裁定附录、**R20 版本锁定纪律（V1–V6）** | 权威 |
| WINDBG_MCP_REPRO_UPSTREAM.md | R18 断点误诊复现报告（**已撤回，勿发上游**；保留作鉴别诊断样本） | 归档 |
| known_limits.md | 已知边界限制登记（KL-编号制） | 活页 |
| divergences.md | 与原版行为的差异登记 | 活页 |
| provenance.map | 符号 ↔ dc 地址 ↔ 源文件 映射表 | 活页 |
| triage_map.md / bc_evaluation_inputs.md | 分诊与评估输入留档 | 参考 |
| MCP_ONBOARDING.md | **参与者接入文档**: ghidra-mcp/windbg-mcp 前置与安装、winuxsh+dsh-winuxsh bundle、反代取件、对接点与故障禁忌 | 活页 |

## 历史批次记录（冻结，只增不改）

mingw_smoke_b0.md、B1_pipeline_status.md、msvc_build.md（构建脚本设计注记）

## analysis/ 目录约定

`r<轮次><线别>_<主题>.md`。重要锚点：
- `e26c_exec_wait_rootcause.md` — EXEC=259 根因（DAT_14013a24f 对象分裂）
- `r19b_exit183_chain.md` — 探针污染 GetLastError 教科书案例
- `r19a_hang_rootcause.md`(+活栈附录) / `r19d_fastfail_overrun.md` — A/D 线取证
- `s14_fourcc_replacement_census.md` — FourCC 替换施工图
- `s15_fun140034788_restore.md` — IFEX/FIND 共享体汇编还原

## tools/ 目录注意

`*.json` 多为历史批处理工件，被 `apply_rename.py` 等以相对路径引用——**不要移动/重命名**。
