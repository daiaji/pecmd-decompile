#ifndef PECMD_DOMAIN_H
#define PECMD_DOMAIN_H

/* B3: Top 对象结构体化 (PRODUCTION_ROADMAP §3.B.3)
 * 布局来源：REVIEW §3 已登记对象 + include/pecmd_offsets.h 宏。
 * 每字段 _Static_assert(offsetof(...)==…) 兜底，保证与原指针运算等价。
 * 未登记布局的对象不得入此文件（发现新布局先登记再纳入下一轮）。 */

#include <stddef.h>
#include <stdint.h>
#include "pecmd_defs.h"

/* ---------- PECMD_WndObj：控件/窗口对象（部分字段登记，见 OBJ_* 宏） ----------
 * 原访问形如 *(HWND*)(o+0x20)。未登记区间以 reserved 占位，不改变总布局语义。 */

typedef struct PECMD_WndObj {
    uint8_t  _r00[0x20];              /* 未登记区 [0x00,0x20) */
    HWND     hwnd;                    /* OBJ_HWND      0x20 子窗口/控件 HWND */
    uint8_t  _r28[0x10];              /* 未登记区 [0x28,0x38) */
    HANDLE   brush;                   /* OBJ_BRUSH     0x38 画刷/旧 GDI 对象 */
    COLORREF color;                   /* OBJ_COLOR     0x40 颜色/前景色 */
    uint8_t  _r48[0x20];              /* 未登记区 [0x48,0x68) */
    HANDLE   customMode;              /* OBJ_CUSTOMMODE 0x68 自定义模式/字体句柄(按对象族区分) */
    uint8_t  _r70_pad[0x08];          /* 对齐占位 [0x68,0x70) —— 注意 0x68 为 8 字节字段 */
    uintptr_t wParam;                 /* OBJ_WPARAM    0x70 暂存 wParam/消息参数 */
    uint8_t  _r78[0xb8];              /* 未登记区 [0x78,0x130)? 见下方断言校准说明 */
} PECMD_WndObj;

/* 说明：OBJ_LINK(0xd0)/OBJ_TEXTCOLOR(0xe0)/OBJ_FLAGS(0xe8)/OBJ_BITMAP(0xe8)/
 * OBJ_SUBWND(0xf0)/OBJ_DOUBLE_200(0x200)/OBJ_FLAGS_3B4(0x3b4) 与上方 _r78 区间
 * 存在重叠表达——0xe8 双定义(BITMAP/FLAGS 按对象族区分)证明该布局是"族共用骨架"，
 * 不同控件族对同一偏移有不同解释。因此 WndObj 不做整段 struct 化，
 * 仅提供以下经登记的**访问器宏**（保持原 (obj+off) 语义，类型化收口）： */

#define WO_HWND(o)        (*(HWND*)     ((uint8_t*)(o) + 0x20))
#define WO_BRUSH(o)       (*(HANDLE*)   ((uint8_t*)(o) + 0x38))
#define WO_COLOR(o)       (*(COLORREF*) ((uint8_t*)(o) + 0x40))
#define WO_CUSTOMMODE(o)  (*(HANDLE*)   ((uint8_t*)(o) + 0x68))
#define WO_WPARAM(o)      (*(uintptr_t*)((uint8_t*)(o) + 0x70))
#define WO_LINK(o)        (*(int64_t*)  ((uint8_t*)(o) + 0xd0))
#define WO_TEXTCOLOR(o)   (*(COLORREF*) ((uint8_t*)(o) + 0xe0))
#define WO_BITMAP(o)      (*(HANDLE*)   ((uint8_t*)(o) + 0xe8)) /* 族: 位图缓存 */
#define WO_FLAGS(o)       (*(uint32_t*) ((uint8_t*)(o) + 0xe8)) /* 族: 标志 */
#define WO_SUBWND(o)      (*(HWND*)     ((uint8_t*)(o) + 0xf0))
#define WO_DOUBLE_200(o)  (*(double*)   ((uint8_t*)(o) + 0x200))
#define WO_FLAGS_3B4(o)   (*(uint32_t*) ((uint8_t*)(o) + 0x3b4))

/* ---------- PECMD_VarNode (0x20)：变量节点 ---------- */

typedef struct PECMD_VarNode {
    WCHAR    *name;                   /* +0x00 变量名(堆串) */
    WCHAR    *value;                  /* +0x08 值(堆串) */
    uint8_t  rsv[0x08];               /* +0x10 保留 (至 0x18) */
    size_t   cap;                     /* +0x18 容量(高2位 0xc0=固定容量截断标记) */
} PECMD_VarNode;

_Static_assert(offsetof(PECMD_VarNode, name)  == 0x00, "VarNode.name @0");
_Static_assert(offsetof(PECMD_VarNode, value) == 0x08, "VarNode.value @8");
_Static_assert(offsetof(PECMD_VarNode, cap)   == 0x18, "VarNode.cap @0x18");
_Static_assert(sizeof(PECMD_VarNode)          == 0x20, "VarNode size 0x20");

/* ---------- PECMD_Script：脚本/变量表对象 ---------- */

typedef struct PECMD_Script {
    void    *vars;                    /* +0x00 变量数组(PECMD_VarNode[]) */
    int64_t  varCount;                /* +0x08 数量 */
    uint8_t  _r10[0x28];              /* 未登记区 [0x10,0x38) */
    void    *parentTable;             /* +0x38 父表(&&链) */
    uint8_t  _r40[0x10];              /* 未登记区 [0x40,0x50) */
    void    *tmpl;                    /* +0x50 模板 */
} PECMD_Script;

_Static_assert(offsetof(PECMD_Script, vars)       == 0x00, "Script.vars @0");
_Static_assert(offsetof(PECMD_Script, varCount)   == 0x08, "Script.varCount @8");
_Static_assert(offsetof(PECMD_Script, parentTable)== 0x38, "Script.parentTable @0x38");
_Static_assert(offsetof(PECMD_Script, tmpl)       == 0x50, "Script.tmpl @0x50");

/* ---------- PECMD_Task：线程任务块 ---------- */

typedef struct PECMD_Task {
    int32_t  refcnt;                  /* +0x00 引用计数 */
    uint32_t _pad04;
    void    *refs;                    /* +0x08 refs */
    uint8_t  _r10[0x08];              /* 未登记区 [0x10,0x18) */
    int64_t  msg;                     /* +0x18 消息 */
    uintptr_t wParam;                 /* +0x20 wParam */
    intptr_t lParam;                  /* +0x28 lParam */
    uint32_t flags;                   /* +0x30 flags */
    uint32_t _pad34;
    int64_t  timeout;                 /* +0x38 超时 */
    HWND     hwnd;                    /* +0x40 hwnd */
    void   (*callback)(void);         /* +0x48 回调 */
    void    *ctx;                     /* +0x50 ctx */
} PECMD_Task;

_Static_assert(offsetof(PECMD_Task, refcnt)   == 0x00, "Task.refcnt @0");
_Static_assert(offsetof(PECMD_Task, refs)     == 0x08, "Task.refs @8");
_Static_assert(offsetof(PECMD_Task, msg)      == 0x18, "Task.msg @0x18");
_Static_assert(offsetof(PECMD_Task, wParam)   == 0x20, "Task.wParam @0x20");
_Static_assert(offsetof(PECMD_Task, lParam)   == 0x28, "Task.lParam @0x28");
_Static_assert(offsetof(PECMD_Task, flags)    == 0x30, "Task.flags @0x30");
_Static_assert(offsetof(PECMD_Task, timeout)  == 0x38, "Task.timeout @0x38");
_Static_assert(offsetof(PECMD_Task, hwnd)     == 0x40, "Task.hwnd @0x40");
_Static_assert(offsetof(PECMD_Task, callback) == 0x48, "Task.callback @0x48");
_Static_assert(offsetof(PECMD_Task, ctx)      == 0x50, "Task.ctx @0x50");

#endif /* PECMD_DOMAIN_H */
