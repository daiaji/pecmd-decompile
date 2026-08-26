# R19 分诊：54 fail 聚类（基线 997ff7a 后首诊）
总发现：全部失败用例 stdout/vars/fs 内容面完全一致——剩余失败面=退出码语义+挂起+崩溃。
- A 挂起(124)×5: 002_envi_assign,004_envi_global_scope,021_sub_call,037_sub_nested,038_sub_param_shadow
  初判: rb:4021 MsgWait 环对非 EXEC 路径缺有效 deadline(local_c28/c48/d08 来源待考)；DAT 统一后早退门恒假放大此缺陷。
- B 退出码183×7: 005,006,007,044,045(WRITE族)+061_logs,065_getf；原版=2。探针见 [WB] l180=183 入退出码链。
- C 垃圾退出码×13: 026,027,028,032,033(CALC) 029,040(LSTR) 048,049,050(RSTR/RPOS) 058(RECY) 062(STRL) 039(TEAM链)；多呈 HRESULT 形态(0x80070057=E_INVALIDARG)。疑错误码泄入 g_exitCode。
- D fastfail(0xC0000409)×4: 024,025(TEAM) 053(HASH) 056(TEMP)；栈cookie触发=真实缓冲越界。039 同属 TEAM 链但呈 C 形态。
- E 残余AV×5: 041(MDIR) 051(SIZE) 057(FORM) 059(LINK) 060(PATH)。
- F 退出语义×15: 010,012(FILE,得87) 011,013,014,016,017,043,046,047,052,054,063,064(得0,exp=2) 031(SET,得1) 055(SED,异常形态)。
修复优先级建议: A(挂起阻断后续一切观察) > D(内存越界) > B(C 线探针数据已在手) > C > E > F。
