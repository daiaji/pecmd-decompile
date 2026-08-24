# harness — C-P0 行为对拍最小闭环（v4.1 纯 WIN 化）

## 结构
```
corpus/cases/<id>_<slug>/     manifest.json + main.pecmd (用例包)
runners/run_case.py           WIN 端: 部署→运行→回捞 (双跑: 原版+编译产物; Windows 原生 Python)
runners/diff_case.py          判读: golden vs results → verdict (纯 stdlib, 原生可跑)
runners/report.py             汇总报告
runners/check_corpus.py       语料结构自检
runners/masks.conf            掩码正则 (时间戳/路径/随机值)
golden/win_real/<case>/       金标准 (原版 EXE 录制晋升, 唯一裁决依据)
results/win_real/<case>/      被测输出 (编译产物 pecmd_msvc.exe)
results/win_real_orig/<case>/ 原版输出 (真值参考; golden 录制唯一来源)
```

## WIN 端用法（Windows 原生 Python ≥3.8，无 WSL/git-bash）
```bat
:: 1. 双跑: 原版 → win_real_orig / 编译产物 → win_real (P0 产出前 msvc 分支自动跳过)
python harness/runners/run_case.py 001_envi_smoke

:: 2. golden 录制: 原版结果晋升 (单一裁判纪律, 只认 win_real_orig 来源)
python harness/runners/run_case.py 001_envi_smoke --exe orig --record-golden

:: 3. 判读 + 汇总
python harness/runners/diff_case.py 001_envi_smoke   :: → verdict
python harness/runners/report.py                     :: → 汇总
```

## 裁决纪律 (§6.7)
- verdict 只认真机 golden（源自原版 EXE 的 win_real_orig 结果）；历史 wine/qemu 输出仅存档参考
- gui=true 用例必须用户在场监督
