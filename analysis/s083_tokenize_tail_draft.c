/* ============================================================================
 * ⚠️ 前提证伪存档 (SUPERSEDED / ARCHIVED) — 2026-08-26
 * ----------------------------------------------------------------------------
 * 本文件为 #083 原任务书（"TokenizeExpression 尾段缺失补齐"）下产生的转录起草。
 * 该任务前提已被复核与轻量交叉验证共同证伪：
 *   - FUN_1400a53e4 实际范围 dc:103346-103731，现有实现
 *     src\commands\core_b3_remaining.c:24757-25171 与其结构 1:1，完整无缺；
 *   - dc:103884-104028 属于 FUN_1400a600c = PECMD_CreateMenuItem，
 *     已完整转录于 src\commands\core_b3r_h4.c:724-950。
 * 因此本文件仅作存档保留，供后续 diff 校验参考，不得合入正式树、不参与构建。
 * 验证结论与证据见同目录 s083_tokenize_tail_notes.md（覆盖重写后的交叉验证报告）。
 * ============================================================================
 * s083_tokenize_tail_draft.c — #083 "TokenizeExpression 尾段缺失补齐" 转录起草
 * ----------------------------------------------------------------------------
 * 状态: 仅分析用草稿 (DRAFT), 不参与构建, 不改 src 树任何文件。
 *
 * 复核结论 (详见同目录 s083_tokenize_tail_notes.md):
 *   1) FUN_1400a53e4 (PECMD_TokenizeExpression) 全长仅 dc:103346-103731,
 *      现有 src\commands\core_b3_remaining.c:24757-25171 已 1:1 完整覆盖
 *      (标签集 {LAB_1400a5424, LAB_1400a598c} 与调用序列逐一对应),
 *      不存在缺失的 ~690 行尾段 —— 任务前提经核实为口径错误。
 *   2) 证据区 dc:103884-104028 实际落在 FUN_1400a600c
 *      (PECMD_CreateMenuItem, dc:103826-104037) 内, 该函数同样已在
 *      src\commands\core_b3r_h4.c:724-950 完整转录。
 *
 * 本草稿因此提供两份"对照性忠实转录", 作为独立二次转录供 diff 校验:
 *   PART A — TokenizeExpression 真正的尾段 (循环尾 + 结果合并/清理/错误路径),
 *            dc:103680-103731, 接缝对准 core_b3_remaining.c 既有变量名。
 *   PART B — 证据区 dc:103882-104037 (含上下文两行) 即 CreateMenuItem 中后段,
 *            接缝对准 core_b3r_h4.c 既有变量名与标签。
 *
 * 风格约定 (与现有实现一致):
 *   - MSVC x64, WCHAR 宽字符环境; 宽串字面量一律 WSTR("...");
 *   - 分配器契约: FUN_140063118 族返回 hdr+8 {size, magic 0xaa55},
 *     释放一律 HeapFree(ptr-8), 封装为 PECMD_FreeStrBuf;
 *   - operator_new 得到的块用 free() 释放 (与 dc 一致);
 *   - FourCC/DWORD case 类数值 (本尾段的 0x2d/0x7c/0x456/0x80 等) 保留数字原样;
 *   - 变量命名沿用 dc 局部名 (plVar/lVar/WVar/pWVar/local_xx/LVar/uVar...)。
 * ==========================================================================*/

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#include "pecmd_defs.h"

#ifndef WSTR
#define WSTR(s) ((const WCHAR *)(const void *)L##s)
#endif

/* ---- 本草稿引用的外部符号 (定义于项目其它翻译单元; 地址锚定) ---- */
typedef unsigned short ushort;
typedef uint8_t undefined1_;          /* dc undefined1 草稿别名 */
typedef void *undefined8_;
#define LI(v) ((LARGE_INTEGER)((intptr_t)(v))) /* dc LARGE_INTEGER 强转草稿别名 */

extern void PECMD_AllocStrSlot(void *out);       /* @0x140063620 (dc FUN_140063620) */
extern void PECMD_FreeStrBuf(void *ps);          /* @0x14005b104 (dc FUN_14005b104) */
extern void PECMD_ClearTaskTable(int64_t s, int m); /* @0x14004eaa8 (dc FUN_14004eaa8) */
extern void PECMD_ForwardCall_6838(int64_t obj, int64_t key);   /* @0x140066838 */
extern void PECMD_ReleaseObjectSlots(int64_t obj, int64_t key); /* @0x140066f64 */
extern uint64_t PECMD_ParseCommandBlock(int64_t sc, int64_t *pp, int f, WCHAR *p); /* @0x140025474 */
extern void *operator_new(size_t size);
extern void *PECMD_GrowByteBuffer(void *ps, int64_t len);       /* @0x140063424 */
extern void PECMD_SetVariable(int64_t *sc, LPCWSTR k, LPCWSTR v);
extern void PECMD_SetVariableWithPrefix(int64_t *sc, LPCWSTR k, LPCWSTR v); /* dc FUN_14007d0ac 变体 */
extern void PECMD_CopyStrToSlot(void *dst, void *src);          /* @0x140070310 */
extern void PECMD_AppendWideStr(WCHAR **ps, LPCWSTR s);         /* @0x14006375c */
extern void *PECMD_ReplaceStringSlot(void *ps, void *src);      /* @0x140070398 */
extern int64_t *PECMD_AssignString(void *ps, const WCHAR *src); /* @0x14007034c */
extern void PECMD_ScriptCopy(uint64_t *obj, uint64_t *p);       /* @0x140017cdc */
extern void PECMD_ScriptInit(uint64_t *obj, int64_t p);         /* @0x1400186bc */
extern uint32_t PECMD_ArgTokenize(int64_t *sc, LPCWSTR s, int f); /* @0x140073ccc */
extern uint64_t PECMD_ParseAndExecuteLine(int64_t sc, LPCWSTR ln, uint64_t a3, uint64_t a4);
extern void PECMD_ScaleWindowPos(int64_t w, uint32_t id, int *p); /* @0x14005db98 */
extern void PECMD_UpdateTrayIcon(void *pc, uint64_t hwnd, LPCWSTR s, void *icon, uint32_t f);
extern void PECMD_BuildPopMenuRecursive(int64_t node, ushort *cnt, HMENU m,
                                        int64_t hwnd, int64_t a5, int64_t *a6); /* @0x1400e5248 */
extern void PECMD_MenuItemWithIcon(HMENU menu, UINT flags, uintptr_t item, void *data); /* @0x1400e4f14 */
extern void PECMD_MenuRegisterCommand(void *item, ushort *cnt, HMENU m,
                                      int64_t hwnd, int64_t a5, int64_t *a6);   /* @0x1400e5120 */
extern void PECMD_FreeInitObjectList(int64_t p);                /* thunk_FUN_140064b78 实体 */
extern void PECMD_ResetScriptChain(void *sc, void *p);          /* @0x14001b3a0 */
extern LPCWSTR PECMD_ParseCommandPath(int64_t *sc, WCHAR **name, uint32_t *idx, int64_t *parent);

extern CRITICAL_SECTION g_csInit;   /* dc DAT_14013e190 */
extern HANDLE g_hHeap;              /* dc DAT_14013d328 */
extern WCHAR g_szEmpty[];

/* ============================================================================
 * PART A — PECMD_TokenizeExpression (@0x1400a53e4) 真实尾段
 *          范围: dc:103680-103731 (= 大扫描 do-while 收口 → 函数返回)
 *          内容: 主循环尾 / 参数模式收尾分支 / 结果合并 / 子脚本对象清理 /
 *                错误路径共用出口 (LAB_1400a598c 已在前文)。
 *
 * // JOIN-POINT: 承接 src\commands\core_b3_remaining.c 中既有实现
 * //   (PECMD_TokenizeExpression, 起 24757 行)。下列变量全部沿用其作用域:
 * //   param_1/param_3/param_4/param_5, puVar16, uVar3, psVar12, sVar21,
 * //   plVar1, cVar14, bVar5, iVar15, ppWVar17, cVar18, pWVar19, pWVar20,
 * //   local_88/local_80/local_78/local_74/local_70/local_68/local_60/
 * //   local_58/local_50/local_48/local_44/local_40/local_38/local_30/
 * //   local_28/local_20, LVar9/LVar13, cVar2, lVar6/lVar8, WVar4/WVar7/WVar22。
 * // 接缝语句: 上文的 do { ... } while (*pWVar20 != *(WCHAR*)(param_1+0x88));
 * // (dc:103680) 之后 —— 本函数体从该循环闭合处开始。
 * ==========================================================================*/
int64_t s083_tokenize_tail_A_reference(LARGE_INTEGER param_1, int64_t param_2,
                                       int64_t *param_3, uint32_t param_4, WCHAR *param_5)
{
    /* 仅为语法自洽而复述的局部声明 (与 core_b3_remaining.c:24761-24799 相同) */
    int64_t *plVar1;
    char cVar2;
    ushort uVar3;
    WCHAR WVar4;
    WCHAR WVar7;
    bool bVar5;
    int64_t lVar6;
    int64_t lVar8;
    LARGE_INTEGER LVar9;
    uint64_t uVar10;
    uint64_t uVar11;
    int16_t *psVar12;
    LPCWSTR lpString;
    LARGE_INTEGER LVar13;
    char cVar14;
    int iVar15;
    uint16_t *puVar16;
    WCHAR **ppWVar17;
    char cVar18;
    WCHAR *pWVar19;
    WCHAR *pWVar20;
    short sVar21;
    WCHAR WVar22;
    WCHAR *local_88;
    LARGE_INTEGER local_80;
    int local_78;
    uint32_t local_74;
    int64_t local_70;
    WCHAR *local_68;
    LPCWSTR local_60;
    WCHAR *local_58;
    LARGE_INTEGER local_50;
    uint32_t local_48;
    uint32_t local_44;
    int64_t local_40;
    uint64_t local_38;
    uint64_t local_30;
    int64_t *local_28;
    int64_t local_20;

    /* ---- 前文占位: 头部跳过空白/定界符、子脚本克隆、参数展开、大扫描 do-while
     *      (dc:103393-103679) 见既有实现, 此处从 dc:103680 起转录 ---- */

    /* dc:103680 主扫描 do-while 循环尾: 回到「当前字符 == 表达式首定界符
     * (param_1+0x88, 通常 '(' )」即退出 */
    /* } while (*pWVar20 != *(WCHAR *)(param_1.QuadPart + 0x88)); */
    if (*pWVar20 != *(WCHAR *)(param_1.QuadPart + 0x88)) {
        goto s083_scan_continue; /* 语法示意: 实际结构为 do-while 回边 */
    }

s083_scan_continue:
s083_loop_exit: /* dc:103681 关闭 do 包裹层 */
    ; /* dc:103682-103689 循环尾: 再跳过连续的空白/定界符 (0x92/0x94/0x8a/0x90) */
    while (1) {
        sVar21 = *(int16_t *)*(uintptr_t *)param_3;
        if ((((*(int16_t *)(param_1.QuadPart + 0x92) != sVar21) &&
              (*(int16_t *)(param_1.QuadPart + 0x94) != sVar21)) &&
             (*(int16_t *)(param_1.QuadPart + 0x8a) != sVar21)) &&
            (*(int16_t *)(param_1.QuadPart + 0x90) != sVar21)) {
            break;
        }
        *param_3 = (int64_t)((int16_t *)*(uintptr_t *)param_3 + 1);
    }
    iVar15 = 0; /* dc:103690 token 模式正常收尾 → 返回码清零 */

    if (local_74 != 0) {
        /* dc:103692-103696 参数替换模式 (local_74!=0): 走命令块解析器 */
        uVar10 = PECMD_ParseCommandBlock(local_80.QuadPart, param_3, 1, pWVar20);
        iVar15 = (int)uVar10;
        LVar9 = local_80;
    }
    /* dc:103691 else 与 103682-103690 共同汇合于此 (Ghidra 平凡合并) */

    /* ---- dc:103697-103718 结果合并与子脚本对象清理 (错误路径共用) ---- */
    LVar13 = local_50;
    /* 把子脚本状态字 (偏移 200=0xC8) 按 cVar2 掩码合并回父脚本;
     * TODO(dc:103698): (short)cVar2 先符号扩展再参与 OR —— Ghidra 类型推断痕迹,
     * cVar2 实取自 *(char*)(param_1+200) (进入时快照), 最佳猜测为按位并入标志位,
     * 汇编层需确认是否应为 (uint16_t)(uint8_t)cVar2。掩码 0x1ef 保留数字原样。 */
    *(uint16_t *)(param_1.QuadPart + 200) =
        (*(uint16_t *)(LVar9.QuadPart + 200) | (uint16_t)(char)cVar2) & 0x1ef;
    if (*(char *)(LVar9.QuadPart + 0xca) != '\0') {
        /* 子脚本发生过异常/跳转定位 (0xca 标志) → 回传 0xd0 定位槽 */
        *(uint64_t *)(param_1.QuadPart + 0xd0) = *(uint64_t *)(LVar9.QuadPart + 0xd0);
    }
    if (local_74 != 0) {
        /* 参数替换模式: 还原进入时快照的脚本游标/计数 (dc:103703-103705) */
        *(uint32_t *)(param_1.QuadPart + 0x24) = local_44;
        *(uint64_t *)(param_1.QuadPart + 0x28) = local_38;
        if (bVar5) {
            /* 换回原参数表: 0x70=旧表指针暂存 local_70, 0x60/0x68=计数与槽 */
            lVar8 = *(int64_t *)(param_1.QuadPart + 0x70);
            *(int64_t *)(param_1.QuadPart + 0x70) = local_70;
            *(uint32_t *)(param_1.QuadPart + 0x60) = local_48;
            *(uint64_t *)(param_1.QuadPart + 0x68) = local_30;
            local_70 = lVar8;
        }
    }
    if (local_50.QuadPart != 0) {
        /* 克隆的子脚本对象 (operator_new(0xf0)) 生命周期终止 */
        PECMD_ClearTaskTable(local_50.QuadPart, 0);
        PECMD_FreeStrBuf((void **)(LVar13.QuadPart + 0x70));
        free((void *)(uintptr_t)LVar13.QuadPart);
    }

    /* ---- dc:103719-103728 公共出口: 返回码落定 + 槽串释放 + 对象链摘除 ---- */
    lVar8 = (int64_t)iVar15;
    PECMD_FreeStrBuf((void **)&local_70); /* 旧参数表槽 (分配器契约 hdr+8, HeapFree(p-8)) */
    PECMD_FreeStrBuf((void **)&local_60); /* 展开后的参数原文槽 */
    EnterCriticalSection(&g_csInit);
    lVar6 = local_20;
    if (local_20 != 0) {
        PECMD_ForwardCall_6838(local_20, LVar9.QuadPart);   /* 从对象链摘除 */
        PECMD_ReleaseObjectSlots(lVar6, LVar9.QuadPart);    /* 递减引用/回收槽位 */
    }
    LeaveCriticalSection(&g_csInit);

    (void)param_2;
    (void)param_4;
    (void)param_5;
    (void)puVar16;
    (void)uVar3;
    (void)uVar11;
    (void)psVar12;
    (void)lpString;
    (void)ppWVar17;
    (void)cVar18;
    (void)pWVar19;
    (void)WVar4;
    (void)WVar7;
    (void)WVar22;
    (void)local_88;
    (void)local_78;
    (void)local_68;
    (void)local_58;
    (void)local_40;
    (void)local_28;
    return lVar8; /* dc:103730 */
}

/* ============================================================================
 * PART B — 证据区真实归属: PECMD_CreateMenuItem (@0x1400a600c) 中后段
 *          范围: dc:103882-104037 (任务口径 dc:103884-104028 + 各 2 行上下文,
 *          起点提前到 pcVar13 赋值以保证语句完整, 终点补齐函数闭括号)。
 *          内容: 菜单项节点装配 / -clear 清空路径 (错误路径) /
 *                -sub 弹出菜单构造 / 单项与工具条 SetMenu 收尾 / 公共清理出口。
 *
 * // JOIN-POINT: 承接 src\commands\core_b3r_h4.c 既有实现 PECMD_CreateMenuItem
 * //   (起 724 行)。沿用其局部: param_1, cVar1(-bar)/cVar2(-sub), bVar3/local_res18
 * //   (-clear), uVar4, hMenu/local_60, LVar5/LVar10/LVar11, puVar6, pWVar12,
 * //   pcVar13, lVar9, local_res10/local_res18/uStackX_1c/local_res20/local_78/
 * //   local_70/local_68/local_58/local_50。
 * // 接缝语句: dc:103881 EnterCriticalSection(&g_csInit); 之后 (h4:775)。
 * ==========================================================================*/
byte *s083_create_menu_tail_B_reference(longlong *param_1, int16_t *param_2)
{
    char cVar1;
    char cVar2;
    byte bVar3;
    uint uVar4;
    HMENU hMenu;
    LARGE_INTEGER LVar5;
    undefined1_ *puVar6;
    LARGE_INTEGER LVar7;
    HMENU pHVar8;
    longlong lVar9;
    LARGE_INTEGER LVar10;
    LARGE_INTEGER LVar11;
    LPCWSTR pWVar12;
    char *pcVar13;
    int16_t *local_res10;
    uint32_t local_res18;
    uint32_t uStackX_1c;
    LPCWSTR local_res20;
    LPCWSTR local_78;
    LPCWSTR local_70;
    longlong local_68;
    HMENU local_60;
    longlong local_58;
    LPCWSTR local_50;

    /* ---- 前文占位: 选项解析 (-bar/-sub/-clear)、逗号参数解析、窗口句柄获取、
     *      进入临界区 (dc:103857-103881) 见 core_b3r_h4.c:751-775 ---- */

    /* dc:103882-103885 命令栈选择: 已有栈 or 窗口内建栈 (+0x2b0) */
    pcVar13 = (char *)param_1[3];
    if (pcVar13 == (char *)0x0) {
        pcVar13 = (char *)(param_1[8] + 0x2b0);
    }

    /* dc:103886-103896 新建菜单项节点 (0x48 字节), 四个字符串槽按契约置空 */
    LVar5 = LI(operator_new(0x48));
    LVar10.QuadPart = LVar11.QuadPart;
    if ((void *)(intptr_t)LVar5.QuadPart != (void *)0x0) {
        PECMD_AllocStrSlot((void *)(uintptr_t)(LVar5.QuadPart + 0x08));
        PECMD_AllocStrSlot((void *)(uintptr_t)(LVar5.QuadPart + 0x10));
        PECMD_AllocStrSlot((void *)(uintptr_t)(LVar5.QuadPart + 0x18));
        PECMD_AllocStrSlot((void *)(uintptr_t)(LVar5.QuadPart + 0x30));
        *(uint64_t *)(uintptr_t)(LVar5.QuadPart + 0x38) = 0;
        *(uint64_t *)(uintptr_t)(LVar5.QuadPart + 0x40) = 0;
        LVar10 = LVar5;
    }
    /* dc:103897-103905 装配: 名称槽/序号/提示串/文本串/热键串 + 标志字节 */
    PECMD_ReplaceStringSlot((void *)(uintptr_t)(LVar10.QuadPart + 0x18), &local_68);
    *(uint32_t *)(uintptr_t)(LVar10.QuadPart + 0x28) = *(uint32_t *)(pcVar13 + 4);
    PECMD_AssignString((void *)(uintptr_t)(LVar10.QuadPart + 0x10), local_70);
    PECMD_AssignString((void *)(uintptr_t)(LVar10.QuadPart + 0x08), local_res20);
    PECMD_AssignString((void *)(uintptr_t)(LVar10.QuadPart + 0x30), local_78);
    if ((int)uVar4 < 0) {
        /* TODO(dc:103903): 错误码归一化 `-v|1` (保号去零), 语义待汇编核验 */
        uVar4 = -uVar4 | 1;
    }
    *(byte *)(uintptr_t)LVar10.QuadPart = (byte)uVar4;

    if ((cVar2 == '\0' && cVar1 == '\0') || (local_res18 == 0)) {
        /* ===== 正常添加路径 (非纯 -clear) dc:103906-104015 ===== */
        if (**(int16_t **)(uintptr_t)(LVar10.QuadPart + 0x18) == 0x2d) { /* 名称以 '-' 开头 */
            *(byte *)(uintptr_t)LVar10.QuadPart = (byte)uVar4 | 2;      /* 禁用项标志 */
            **(int16_t **)(uintptr_t)(LVar10.QuadPart + 0x18) = 0;
        }
        if (**(int16_t **)(uintptr_t)(LVar10.QuadPart + 0x18) == 0x7c) { /* '|' 分隔符 */
            *(byte *)(uintptr_t)LVar10.QuadPart |= 1;                    /* 分组线标志 */
            **(int16_t **)(uintptr_t)(LVar10.QuadPart + 0x18) = 0;
        }
        /* 命令栈扩容至 (count+1)*0x48 (dc:103915) */
        PECMD_GrowByteBuffer((void *)(pcVar13 + 8),
                             (longlong)(*(int *)(pcVar13 + 4) + 1) * 0x48);
        if (**(LPCWSTR *)(uintptr_t)(LVar10.QuadPart + 0x18) != L'\0') {
            PECMD_SetVariable(param_1, *(LPCWSTR *)(uintptr_t)(LVar10.QuadPart + 0x18),
                              local_res20);
            PECMD_CopyStrToSlot((void *)&local_res18,
                                (void *)(uintptr_t)(LVar10.QuadPart + 0x18));
            PECMD_AppendWideStr((WCHAR **)&local_res18, WSTR(".Enable"));
            if (param_1[3] == 0) {
                /* 仅主脚本写 <名>.Enable 变量 (子脚本不落盘) */
                pWVar12 = WSTR("1");
                if ((*(byte *)(uintptr_t)LVar10.QuadPart & 1) != 0) {
                    pWVar12 = WSTR("0");
                }
                /* dc:103925 CONCAT44(uStackX_1c,local_res18) 高半字残留栈垃圾,
                 * 既有实现按 SetVariableWithPrefix 规避, 此处保持一致 */
                PECMD_SetVariableWithPrefix(
                    param_1,
                    (LPCWSTR)(uintptr_t)(((uint64_t)uStackX_1c << 32) | (uint32_t)local_res18),
                    pWVar12);
            }
            PECMD_FreeStrBuf((void *)&local_res18);
        }
        /* 节点入栈并 count++ (dc:103929-103931) */
        ((LARGE_INTEGER *)(uintptr_t)(*(longlong *)(pcVar13 + 8) +
                                       (longlong)*(int *)(pcVar13 + 4) * 8))
            ->QuadPart = (int64_t)LVar10.QuadPart;
        *(int *)(pcVar13 + 4) = *(int *)(pcVar13 + 4) + 1;

        if ((cVar1 != '\0') || (cVar2 != '\0')) { /* -bar 或 -sub */
            if (hMenu == (HMENU)0x0) {
                hMenu = CreateMenu();
                local_60 = hMenu;
            }
            if (cVar2 != '\0') { /* ===== -sub 子菜单路径 dc:103937-103991 ===== */
                local_58 = param_1[5];
                local_res18 = *(uint32_t *)((longlong)param_1 + 0x24);
                local_50 = PECMD_ParseCommandPath(
                    (int64_t *)param_1, *(WCHAR **)(uintptr_t)(LVar10.QuadPart + 0x18),
                    &local_res18, &local_58);
                if (local_50 == (LPCWSTR)0x0) {
                    /* 错误路径: 解析失败 → 放锁并带 1 返回 (dc:103943-103946) */
                    LeaveCriticalSection(&g_csInit);
                    LVar11.QuadPart = 1;
                    goto LAB_s083_a65eb;
                }
                /* 子脚本控制块 0x10 字节 (dc:103947-103951) */
                puVar6 = (undefined1_ *)operator_new(0x10);
                *(uint32_t *)((uintptr_t)puVar6 + 4) = 0;
                *(uint64_t *)((uintptr_t)puVar6 + 8) = 0;
                *(uint8_t *)(uintptr_t)puVar6 = 0;
                *(uint8_t *)(uintptr_t)LVar10.QuadPart = 0x80; /* 子菜单宿主标志 */
                if (*(longlong *)(uintptr_t)(LVar10.QuadPart + 0x10) != 0) {
                    /* 分配器契约: 字符串槽指针指向 hdr+8, 释放须 HeapFree(p-8) */
                    HeapFree(g_hHeap, 0,
                             (LPVOID)(uintptr_t)(*(longlong *)(uintptr_t)(LVar10.QuadPart + 0x10) +
                                                 -8));
                    *(uint64_t *)(uintptr_t)(LVar10.QuadPart + 0x10) = 0;
                }
                *(undefined1_ **)(uintptr_t)(LVar10.QuadPart + 0x10) = puVar6;
                /* 克隆子脚本并执行子命令行 (dc:103957-103982) */
                LVar7 = LI(operator_new(0xf0));
                LVar5.QuadPart = LVar11.QuadPart;
                if ((void *)(intptr_t)LVar7.QuadPart != (void *)0x0) {
                    PECMD_ScriptCopy((uint64_t *)(uintptr_t)LVar7.QuadPart,
                                     (uint64_t *)param_1);
                    PECMD_ScriptInit((uint64_t *)(uintptr_t)LVar7.QuadPart,
                                     (longlong)param_1);
                    *(uint64_t *)(uintptr_t)(LVar7.QuadPart + 0x40) = 0;
                    LVar5 = LVar7;
                }
                *(undefined1_ **)(uintptr_t)(LVar5.QuadPart + 0x18) = puVar6;
                *(uint32_t *)(uintptr_t)(LVar5.QuadPart + 0x24) = local_res18 + 1;
                *(longlong *)(uintptr_t)(LVar5.QuadPart + 0x28) = param_1[5];
                PECMD_ArgTokenize((int64_t *)(uintptr_t)LVar5.QuadPart, g_szEmpty, 0);
                lVar9 = param_1[8];
                *(longlong *)(uintptr_t)(LVar5.QuadPart + 0x40) = lVar9;
                EnterCriticalSection(&g_csInit);
                LeaveCriticalSection(&g_csInit);
                PECMD_ParseAndExecuteLine(
                    LVar5.QuadPart, local_50,
                    *(uint64_t *)(uintptr_t)(LVar5.QuadPart + 0x40), 1);
                EnterCriticalSection(&g_csInit);
                if (lVar9 != 0) {
                    PECMD_ForwardCall_6838(lVar9, LVar5.QuadPart);
                    PECMD_ReleaseObjectSlots(lVar9, LVar5.QuadPart);
                }
                LeaveCriticalSection(&g_csInit);
                PECMD_ClearTaskTable(LVar5.QuadPart, 0);
                PECMD_FreeStrBuf((void *)(uintptr_t)(LVar5.QuadPart + 0x70));
                free((void *)(uintptr_t)LVar5.QuadPart);
                if (param_1[3] != 0) {
                    goto LAB_s083_a65de; /* 子脚本模式下不再动 UI */
                }
                /* 用子命令块递归建弹出菜单, 计数+1 后挂到父菜单 (dc:103984-103990) */
                pHVar8 = CreatePopupMenu();
                lVar9 = param_1[8];
                PECMD_BuildPopMenuRecursive(
                    (longlong)(intptr_t)puVar6, (ushort *)(lVar9 + 0x218), pHVar8,
                    *(longlong *)(lVar9 + 0x20), *(longlong *)(lVar9 + 0x200),
                    *(longlong **)(lVar9 + 0x290));
                hMenu = local_60;
                *(short *)(param_1[8] + 0x218) = *(short *)(param_1[8] + 0x218) + 1;
                PECMD_MenuItemWithIcon(local_60, 0x10, (uintptr_t)pHVar8,
                                       (void *)(uintptr_t)LVar10.QuadPart);
            }
        }
        if ((param_1[3] == 0) && (param_1[8] != 0)) { /* dc:103993-104015 UI 收尾 */
            if (*(int *)(pcVar13 + 4) == 1) {         /* 栈中恰一项 */
                if (cVar1 != '\0') {
                    goto LAB_s083_a657f;
                }
                if (cVar2 == '\0') {
                    /* 单普通项 → 托盘图标路径 */
                    PECMD_UpdateTrayIcon(pcVar13, *(uint64_t *)(uintptr_t)(param_1[8] + 0x20),
                                         (LPCWSTR)0x0, (HICON)0x0, 0x456);
                    goto LAB_s083_a6576;
                }
            }
            else {
            LAB_s083_a6576:
                if ((cVar1 == '\0') && (cVar2 == '\0')) {
                    goto LAB_s083_a65de;
                }
            LAB_s083_a657f:
                if (cVar2 == '\0') { /* -bar: 注册工具条按钮 */
                    lVar9 = param_1[8];
                    PECMD_MenuRegisterCommand(
                        (void *)(uintptr_t)LVar10.QuadPart, (ushort *)(lVar9 + 0x218), hMenu,
                        *(longlong *)(lVar9 + 0x20), *(longlong *)(lVar9 + 0x200),
                        *(longlong **)(lVar9 + 0x290));
                }
            }
            if (*(int *)(pcVar13 + 4) == 1) {
                SetMenu(*(HWND *)(param_1[8] + 0x20), hMenu);
                PECMD_ScaleWindowPos(param_1[8], 3, (int *)0x0);
            }
        }
    }
    else if (param_1[3] != 0) {
        /* ===== -clear 清空路径 (错误/重置路径) dc:104017-104028 ===== */
        if (param_1[8] != 0) {
            SetMenu(*(HWND *)(param_1[8] + 0x20), (HMENU)0x0);
            PECMD_ScaleWindowPos(param_1[8], 3, (int *)0x0);
        }
        pcVar13 = (char *)param_1[3];
        if (pcVar13 != (char *)0x0) {
            PECMD_FreeInitObjectList((longlong)(intptr_t)pcVar13); /* dc thunk_FUN_140064b78 */
            free(pcVar13);
        }
        param_1[3] = 0;
    }

LAB_s083_a65de: /* dc:104029 公共成功出口: 放锁 */
    LeaveCriticalSection(&g_csInit);

LAB_s083_a65eb: /* dc:104031 公共清理出口: 四槽逆序释放 (错误路径汇合点) */
    PECMD_FreeStrBuf((void *)&local_78);
    PECMD_FreeStrBuf((void *)&local_70);
    PECMD_FreeStrBuf((void *)&local_res20);
    PECMD_FreeStrBuf((void *)&local_68);
    /* dc:104036 只取 LVar11 低 32 位返回 (HMENU 截断/错误码复用)
     * TODO(dc:104036): 成功路径返回值语义待汇编核验 */
    return (byte *)(uintptr_t)(uint32_t)LVar11.LowPart;
}

/* ============================================================================
 * 附注 (非代码):
 *  - PART A/B 中的 s083_* 函数名、LAB_s083_* 标签均为本草稿私有命名, 仅为让片段
 *    语法自洽可读; 合入正式树时应去掉包装壳, 直接贴回既有函数体对应接缝处。
 *  - undefined1_/undefined8_/LI() 为草稿内占位别名, 以免与正式头文件冲突
 *    (本草稿不参与编译, 不进构建脚本)。
 * ==========================================================================*/
