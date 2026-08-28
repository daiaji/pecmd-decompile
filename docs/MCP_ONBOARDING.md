# MCP 与工具链接入指引（参与者版）

> 目标：让新参与者在一台 Windows x64 机器上复现本仓库的调试/分析环境。
> 本项目的日常作业由两套 MCP 服务器支撑（Ghidra 反编译/二进制读取、WinDbg 活体取证），
> 终端交互统一走 Bash（winuxsh）。本文给出全部前置、安装步骤与验证清单。
> 作业契约与纪律见根目录 `AGENTS.md`；windbg MCP 已知故障面见
> `docs/WINDBG_MCP_FAULTS_REPRO.md`。

---

## 1. 总览

| 组件 | 用途 | 前置 |
|---|---|---|
| **ghidra-mcp**（bethington/ghidra-mcp） | Ghidra 反编译、.rdata 读取、跨二进制文档迁移（253 工具） | Java 21 LTS、Maven 3.9+、**Ghidra 12.1.2**、Python 3.10+（uv 或 pip+venv） |
| **windbg-mcp**（glslang/windbg-mcp） | 活体断点/栈取证、PageHeap 验证、崩溃 dump 分析 | Windows x64、`dbgeng.dll/dbghelp.dll`（System32）；`!analyze`/驱动工具/TTD 回放需 **WinDbg engine 拷贝**；`record_trace` 需 `ttd.exe`；内核/TTD 需管理员 |
| **winuxsh**（unixwin/winuxsh） | 原生 Windows Bash（rubash 引擎，GNU Bash 测试 86/86，无 WSL） | 单二进制，免安装入 PATH |
| **dsh-winuxsh**（caomengxuan666/dsh-winuxsh） | DeepSeek Harness（DSH）插件：winuxsh runtime/沙箱/Web 配置卡 | 先装 winuxsh 并入 PATH，再 `dsh plugin --profile web add @cmx666/dsh-winuxsh-bundle@0.1.0-rc.8` |

> 网络提示：GitHub 直连不可达时，用反代 `https://rp.erojk.eu.org/<完整目标URL>`（已实测可经它取
> `raw.githubusercontent.com` 与部分 `api.github.com` 路由）。示例：
> `https://rp.erojk.eu.org/https://raw.githubusercontent.com/glslang/windbg-mcp/master/docs/install.md`

---

## 2. ghidra-mcp 前置与安装

### 2.1 前置清单

- **Java 21 LTS**（OpenJDK）——`java -version` 验证
- **Apache Maven 3.9+**——`mvn -v` 验证
- **Ghidra 12.1.2**（或兼容版）——解压路径记为 `<GHIDRA_PATH>`，如 `F:\ghidra_12.1.2_PUBLIC`
- **Python 3.10+**，推荐 [uv](https://docs.astral.sh/uv/)（或 pip + venv）

### 2.2 安装

```bash
git clone https://github.com/bethington/ghidra-mcp.git
cd ghidra-mcp
python -m tools.setup preflight --ghidra-path "<GHIDRA_PATH>"   # 环境预检
python -m tools.setup ensure-prereqs --ghidra-path "<GHIDRA_PATH>"  # 装 Python 依赖 + Ghidra JAR 入本地 Maven 仓库
python -m tools.setup build
python -m tools.setup deploy --ghidra-path "<GHIDRA_PATH>"     # 装扩展、启动 Ghidra、等 MCP 健康、跑 schema 冒烟
```

注意（官方文档明示）：

- `deploy` 会**保存并关闭正在运行的匹配 Ghidra 实例**，安装扩展后重启并等 MCP 就绪。
- `.py` 脚本（`ghidra_scripts/`）需要 Ghidra 的 **Jython 扩展**（File > Install Extensions）并重启。
- 进阶排查：`python -m tools.setup --help`；仅构建模式 `python -m tools.setup build`。

### 2.3 本项目对接点

- 分析对象 = 仓库 `reference/decompiled.c`（Ghidra 导出的唯一语义真值）+ `reference/PECMD.EXE`（原版二进制）。
- 用 MCP 读原版 `.rdata` 常量、反编译**当前构建** `build/msvc/pecmd_msvc.exe`（如
  `FUN_14003e710` FILE 处理器、`FUN_140101b40` FORM 处理器）做“编译产物 vs 源码”对照。

---

## 3. windbg-mcp 前置与安装

### 3.1 前置清单

| 需求 | 说明 |
|---|---|
| Windows x64 | 主机位数与目标一致 |
| `dbgeng.dll`/`dbghelp.dll` | Win11 System32 自带（验证于 10.0.26100），够 live 用户态/内核/dump |
| **WinDbg engine 拷贝** | `!analyze`（winext 扩展）、驱动工具（winxp\kdexts.dll）、TTD 回放（.run）需要——System32 引擎不提供。来源：微软商店 `Microsoft.WinDbg` 包，把其 `ttd\`、`winext\`、`winxp\` 拷到 windbg-mcp 二进制旁 |
| `ttd.exe` | 仅 `record_trace` 需要（随 WinDbg/TTD 商店包，入 PATH） |
| 管理员 | live 内核调试与 TTD 录制（回放不需要） |
| 符号服务器 | 符号名查询（如 `ttd_calls("ucrtbase!_stdio_common_vfprintf")`）需要可及符号源；离线时地址类查询与数据模型仍可用 |

### 3.2 安装

二选一：

```bash
# A) 预编译二进制（免 Rust 工具链）——从 GitHub Releases 下载
#    windbg-mcp-vX.Y.Z-windows-x64.zip（附 SHA256SUMS.txt 校验）

# B) Scoop（社区 manifest，自动跟踪发布）
scoop bucket add dungeon https://github.com/gitfool/scoop-dungeon
scoop install windbg-mcp
scoop update windbg-mcp
# 已装 Microsoft.WinDbg 商店包时，manifest 的 post_install 会自动拷贝引擎 ttd/winext/winxp
```

客户端配置（以 Claude 系为例，`.mcp.json` 或等效 mcpServers）：

```jsonc
{
  "mcpServers": {
    "windbg": { "command": "C:\\path\\to\\windbg-mcp.exe" }
  }
}
```

跨机/远程：`--listen <addr>` 以 HTTP 提供同一工具集（每客户端一 bearer token），
Mac 可驱动 Windows VM——见官方 `docs/mcp-clients.md` 与 `docs/remote-listener.md`。

### 3.3 本项目对接点与陷阱

- 活体取证标准流程：`sxe av` / `sxe c0000374` → `g` → `r; k` 抓首违例帧；PageHeap（IFEO
  GlobalFlag 0x02000000）验证越界写；硬件断点 `ba w8 @rcx` 监视槽写者；断点歧义符号用 `bm`。
- **已知故障面**：本会话记录过 T2/T3（`g` 超时=目标挂起如弹窗阻塞、`r;k` 偶发 0x80040205、
  bp 链式自毁）——对应处置全录于 `docs/WINDBG_MCP_FAULTS_REPRO.md`，按该文三律规避：
  `execute` 单命令放行、构建前三查无在途会话（LNK1201）、断点不带内嵌命令串（T1）。
  （**2026-08-27 更新**：T2/T3 与 `r;k` 偶发 0x80040205 已随上游 #243+dbgscope#120 合并修复；
  现行行为=目标跑完自动 **ending**、后续工具 `stale_session` 拒绝，三律降级为防御性习惯。）
- 构建前必须无在途 windbg 会话（PDB 锁 → LNK1201），有则 `end_session` 后再 `build_msvc.sh`。

---

## 4. winuxsh 与 dsh-winuxsh

### 4.1 winuxsh（原生 Windows Bash）

单二进制，Bash 语法 + Windows 原生路径（`C:\...` 与 `C:/...` 均可用，`/c/...` 输入可识别），
内置 `ls/cat/grep/find/test/printf` 等 Unix 命令，`git/node/python/cargo` 走本机 PATH。
引擎 = rubash，通过 GNU Bash 自身测试 86/86。要点：**无 WSL、无虚拟机、无 /mnt/c**。

安装：下载发布二进制，把 `winuxsh.exe` 加入 PATH，`echo $BASH_VERSION` 验证。

### 4.2 dsh-winuxsh（DSH 插件）

前置：先装 winuxsh 并入 PATH。然后：

```bash
dsh plugin --profile web add @cmx666/dsh-winuxsh-bundle@0.1.0-rc.8
dsh web
```

Bundle 启用 `winuxsh-sandbox` 与 `tool-bash`、停用 PowerShell 行、加 Winuxsh 配置卡
（Settings > Plugins），并把 Session 导出并入会话动作组。包：
`@cmx666/dsh-winuxsh-local`（本地执行器）、`@cmx666/dsh-winuxsh-sandbox`（沙箱执行器）、
`@cmx666/dsh-winuxsh-bundle`（一键 profile）、`@cmx666/dsh-client-ui-winuxsh`（Web 卡）。

---

## 5. 推荐安装顺序（Quick Start）

```text
1) winuxsh.exe 入 PATH                                  （shell 基础）
2) DeepSeek Harness(DSH) + dsh-winuxsh bundle           （Web 会话基础）
3) Java 21 + Maven 3.9+ + Ghidra 12.1.2 + Python/uv     （ghidra-mcp 前置）
4) ghidra-mcp: clone → setup preflight/ensure-prereqs/build/deploy
5) windbg-mcp: 预编译 zip 或 Scoop + WinDbg engine 拷贝（winext/kdexts/ttd）
6) （可选）符号服务器可达性配置
```

### 验证清单

```bash
java -version            # ≥ 21
mvn -v                   # ≥ 3.9
python --version         # ≥ 3.10
echo $BASH_VERSION       # winuxsh 生效（GNU bash 系）
dsh plugin list          # 出现 winuxsh 相关条目
# ghidra-mcp: deploy 末尾 "runs schema smoke checks" 通过
# windbg-mcp: 对任一 .dmp 执行 crash_triage/!analyze 成功
```

---

## 6. 反代使用备忘

GitHub 直连被阻断时的取件路由（本会话实测）：

```text
https://rp.erojk.eu.org/https://raw.githubusercontent.com/<org>/<repo>/<branch>/<path>
https://rp.erojk.eu.org/https://api.github.com/repos/<org>/<repo>
```

注意：反代是第三方镜像，不承载凭据；`api.github.com` 经其访问受上游限流（可加
`Authorization: Bearer <token>` 提额）。官方源优先，反代仅作备胎。