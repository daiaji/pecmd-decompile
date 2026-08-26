# 批次28 函数重构子代理指令

你在 /home/daiaji/repo/PECMD反编译/refactored 下工作。读取你的 group 文件 (gA.txt / gB.txt / gC.txt / gD.txt，取决于分配)，其中包含你要重构的函数的 Ghidra 反编译原始体。

任务：把其中每个函数重构为可编译的 C 代码，写入指定的输出文件。保持每个函数原有的 FUN_<addr> 名称（绝不发明新 PECMD_* helper 名）。

## 必须遵守的约定
1. 文件头注释: 每个函数在定义前写上 `/* @0x<addr> */`（从反编译的 `/* ========== FUN_xxx @ <addr> ========== */` 取地址）。
2. 签名: 使用反编译中的真实签名（参数类型/返回类型）。不要用 `uint64_t FUN_xxx(void)` 这种错误签名。
3. 宽字符串字面量: 反编译里是 `L"..."`，在 Linux gcc 下 L"..." 是 const int*，而项目 LPCWSTR 是 const uint16_t*。一律用宏 `WSTR("...")`（已在 include/win32_stub.h 定义）。例如 `L"foo"` → `WSTR("foo")`。
   - 但 `wchar_t` 局部/数组的_L"..." 保持原样也可（用 L"..." 分配给 WCHAR* 时改用 WSTR）。
   - 需要 LPCWSTR 传给 Win32 API 或 helper 的字符串字面量，统一用 WSTR("...")。
4. 类型: 项目 include/win32_stub.h 定义了 HANDLE,HWND,DWORD,UINT,LPCWSTR,WCHAR,RECT,FILETIME,LARGE_INTEGER 等。HWND/HANDLE/HICON 等是 uintptr_t。NULL 用 `(void*)0`；赋给整型/句柄时用 `0`。
5. 辅助函数调用: 反编译里的 FUN_140xxxxxx 调用点，保持原名。绝大多数已存在（可能带略有不同签名）。调用处用强制转换使其与已有声明兼容（参考已重构文件 core_b3r_*.c 的调用风格）。**不要改名，不要自己发明 PECMD_xxx**。
6. CRT/标准库: memset 用标准 `memset()`。反编译中的 FUN_140102a90 就是 memset 的别名，直接写 `memset`。其他如 lstrlenW/lstrcmpW/StrCmpNIW/wsprintfW 等已在 win32_stub.h 声明，直接用。
7. 反编译伪代码简化: CONCATxx 寄存器拼接指示是 Ghidra 噪音，普通整型/位运算正常写。`undefined4/undefined8/undefined1` 是类型占位，按上下文替换为合理类型（uint32_t/int64_t/uint8_t 等）。`(code *)` 函数指针调用 `(*DAT_xxx)(...)` 可还原为通过类型化函数指针调用；用一个 `typedef` 或将 DAT 声明为适当类型的函数指针。若 DAT 是函数指针全局，先假定项目已有声明。
8. 返回值: 如果反编译函数声明 void 但看起来有返回值依赖（eax 残留），按反编译给出的返回类型返回。拿不准写 `TODO(verify)`。
9. DAT_140xxxxxx 全局: 保持原名引用。这些是全局变量，项目可能已定义为适当类型；若引用处类型不匹配，加显式 cast。不要定义 DAT 变量（除非确认未定义，且是必需——在输出里注明）。
10. 输出格式: 每个函数输出为一个完整 C 函数定义，前面有 `/* @0x<addr> */` 头注释。函数之间空行分隔。不要输出其它文件内容。

## 输出
把全部重构好的函数体写到一个文本文件，路径由你的分配指定（如 outA.txt）。最后用 report 向父代理汇报：
- 每个函数的状态 (RESTORED / SKIP(CRT) / SKIPPED-decompile-fail)
- 你引用了哪些在本批次之外、你怀疑未定义的 FUN_/DAT_/API 符号（用于补 link_stubs）
- 是否有 TODO(verify)

注意: 只输出你负责的 group 文件里的函数，不要动 core_b3_remaining.c。
