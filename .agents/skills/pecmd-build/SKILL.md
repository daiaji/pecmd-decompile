---
name: pecmd-build
description: PECMD 反编译还原工程的 MSVC 构建与冒烟规程 (双绿门/拷贝/运行)
whenToUse: 需要编译 pecmd_msvc.exe、跑 syntax 门、或部署到 C:\pectest 冒烟时
---

# PECMD 构建规程

## 前置事实

- 仓库: `D:\repo\PECMD反编译` (R21 起 git 根=项目根; 原 refactored 子层已上提; 语义真值在 reference\decompiled.c)
- VS 2022 Community; vcvarsall: `C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvarsall.bat`
- 本机 8GB 内存, 编译峰值 ~2GB — **不要并行多份构建**; 构建产物用完即删
- 门禁口径 (双绿门): `syntax` 全绿 + 完整构建 exit 0, 缺一不可

## 构建命令

```bat
cmd /c "call ""C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvarsall.bat"" x64 && tools\msvc_build.bat syntax"   :: 语法门 (cl /Zs)
cmd /c "call ""C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvarsall.bat"" x64 && tools\msvc_build.bat"          :: 完整构建 -> build\msvc\pecmd_msvc.exe (+ .map)
```

**构建前三查(R20/V6)**: ① 无在途 windbg 会话(含子代理取证会话, worker 持 PDB → LNK1201);
② 磁盘余量; ③ 注意 `cmd /c` 内的 cd 不影响外层 cwd, 收尾步骤用绝对路径。
绿门判定: exit 0 且输出含 `[msvc_build] OK`。

构建成功后必跑:
```bash
bash tools/make_symsnap.sh <repo根>    # 刷新 symsnap.txt(断点选址唯一来源)
bash tools/post_build.sh <repo根>      # 或一站完成: symsnap + 部署 C:\pectest + DEPLOYED_BUILD.txt 身份戳
```

构建日志是 GBK 编码: 用 `python -c "open(r'%TEMP%\log','rb').read().decode('gbk')"` 或
`Get-Content -Encoding UTF8` 读尾部; 统计错误用正则 `error (C\d+|LNK\d+)`。

## 部署 + 冒烟 (每次构建后)

```powershell
Stop-Process -Name pecmd_msvc -Force -ErrorAction SilentlyContinue   # 它常挂着消息泵锁住 exe!
Copy-Item build\msvc\pecmd_msvc.exe C:\pectest\ -Force
python harness\runners\check_corpus.py                                # 语料自检, 期望 "全部通过"
python harness\runners\run_case.py 001_envi_smoke                     # 双跑 (orig + msvc)
python harness\runners\diff_case.py 001_envi_smoke                    # verdict
```

- 原版真值 EXE 在 `C:\pectest\PECMD.EXE` (=PECMD原始.EXE, 版本 201201.88.05.94 mdyblog 修改版)
- 被测产物默认找 `C:\pectest\pecmd_msvc.exe`
- GUI 子系统程序无 stdout; 判定靠 epilogue 回捞产物 (vars.txt/done.txt)

## 已知坑

- `build.sh` 是 Linux gcc 参考门, 留档不作为 WIN 门禁
- `.map` 文件生成在 build\msvc\ 但被 gitignore — 崩溃符号定位前先确认还在 (被删则重构建)
