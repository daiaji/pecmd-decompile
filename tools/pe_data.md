# PE 数据段真值提取（来自 PECMD原始.EXE，ImageBase=0x140000000）
- `DAT_14013a208` SID authority (8B): 00 00 00 00 00 05 00 00  == SECURITY_NT_AUTHORITY
- `DAT_14013a0d0` 特权 LUID 表 (26x8B, Luid+Attributes=0): LUID=2,3,4,..,0x12,0x14(见脚本/hexdump)
- `DAT_14011d288` 服务名前缀 (20B): "service:" (UTF-16LE)
- `PTR_PTR_14013a050`=0x14011d4b8 → 表: 0x14000c760 + ".*.wcz/.*.wce/.*.wcs/..." 扩展名模式
- `PTR_PTR_14013a090`=0x14011d2a0 → 表: 0x140008ea0 + "PECMDTBL" + 0x1400091dc + "**logs:\\" + "PECMD LOAD ..."
- `PTR_PTR_14013a070`=0x14011d2b8 → 表: 0x1400091dc + "**logs:\\" + "PECMD LOAD  *map:0x..."

用法: python3 tools/pe_data_extract.py ../PECMD原始.EXE <addr> <n>  (addr 如 0x14013a208)
