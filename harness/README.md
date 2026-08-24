# harness — C-P0 行为对拍最小闭环（WIN 交接前置, §8.3-6）

## 结构
```
corpus/cases/<id>_<slug>/   manifest.json + main.pecmd (用例包, Linux 造)
runners/run_case.sh          WIN 端: 部署→运行→回捞 (WSL/git-bash)
runners/diff_case.py         Linux 端: golden vs results → verdict.json
runners/report.py            Linux 端: 汇总报告
runners/masks.conf           掩码正则 (时间戳/路径/随机值)
golden/win_real/<case>/      金标准 (原版 EXE 真机录制, 唯一裁决依据)
results/win_real/<case>/     被测输出 (回捞产物)
```

## WIN 端三步 (真机, 由 WIN 端 DSH 或用户执行)
```bash
# 1. 部署+运行+回捞 (gui=false 可无人值守)
bash harness/runners/run_case.sh 001_envi_smoke
# 产物在 results/win_real/001_envi_smoke/ (拷回 Linux)
```

## Linux 端消费 (唯一裁判)
```bash
# 2. 首次: 将真机回捞的 results/ 同步入仓; golden 由"原版 EXE 录制"产生
python3 harness/runners/diff_case.py 001_envi_smoke   # → verdict.json
python3 harness/runners/report.py                     # → 汇总
```

## 裁决纪律 (§6.7)
- verdict 只认真机 Windows golden; wine/qemu 输出仅开发参考
- gui=true 用例必须用户在场监督 (本次冒烟用例 gui=false)
