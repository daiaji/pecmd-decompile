/* ====================================================================
 * pecmd_offsets.h — PECMD 对象字段偏移宏（可读化阶段 2）
 *
 * 这些偏移来自反编译中高频出现的对象布局。不同对象族可能复用同一
 * 字节偏移表示不同字段；使用宏时请结合具体函数语义确认。
 *
 * 通用控件/窗口对象布局（0xe0+ 见各对象族）:
 *   +0x20  hwnd/子窗口句柄
 *   +0x38  画刷/旧 GDI 对象
 *   +0x40  颜色/前景色
 *   +0x68  自定义模式/字体句柄（按对象族区分）
 *   +0x70  暂存 wParam/消息参数
 *   +0xd0  关联对象/内部数据
 *   +0xe0  文本颜色
 *   +0xe8  样式/标志
 *   +0xf0  子窗口对象/关联窗口
 *   +0x200 double 字段（滚动比例等）
 *   +0x3b4 标志字段
 *
 * 数组/容器通用布局:
 *   [0] 数据指针  [1] 容量/元素数  [2] 当前计数/写指针
 * ==================================================================== */
#ifndef PECMD_OFFSETS_H
#define PECMD_OFFSETS_H

/* B3 提示: 本宏族的类型化访问器已迁至 include/domain/pecmd_domain.h
 * (WO_* 系列)。新代码请用 domain 头; 本头保留供存量引用渐进迁移。 */

#define OBJ_HWND       0x20  /* 子窗口/控件 HWND */
#define OBJ_BRUSH      0x38  /* 画刷/旧 GDI 对象 */
#define OBJ_COLOR      0x40  /* 颜色/前景色 */
#define OBJ_CUSTOMMODE 0x68  /* 自定义模式/字体句柄（按对象族区分） */
#define OBJ_WPARAM     0x70  /* 暂存 wParam/消息参数 */
#define OBJ_LINK       0xd0  /* 关联对象/内部数据 */
#define OBJ_TEXTCOLOR  0xe0  /* 文本颜色 */
#define OBJ_FLAGS      0xe8  /* 样式/标志 */
#define OBJ_BITMAP     0xe8  /* 控件背景位图/缓存（部分对象族） */
#define OBJ_SUBWND     0xf0  /* 子窗口对象/关联窗口 */
#define OBJ_DOUBLE_200 0x200 /* double 字段（滚动比例等） */
#define OBJ_FLAGS_3B4  0x3b4 /* 标志字段 */

#endif /* PECMD_OFFSETS_H */
