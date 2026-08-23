/* ====================================================================
 * core_b8b.c — B8 小函数批 2 (1400e0000-1400fffff)
 *
 * 本批新实现函数全部使用人类可读 PECMD_ 名称，原始地址保留在 @0x 注释。
 *
 * 来源: PECMD原始.EXE (x64, ImageBase=0x140000000)
 *   销毁 GDI 对象(带释放)   FUN_1400F0F78 @0x1400f0f78
 *   销毁窗口对象 A(带释放)  FUN_1400F22AC @0x1400f22ac
 *   发送窗口消息 0x202       FUN_1400F22DC @0x1400f22dc
 *   销毁窗口对象 B(带释放)  FUN_1400F2A4C @0x1400f2a4c
 *   发送窗口消息 0x204       FUN_1400F51A8 @0x1400f51a8
 *   销毁对象(带释放)        FUN_1400F9104 @0x1400f9104
 *   销毁对象(带释放) B      FUN_1400FB558 @0x1400fb558
 *   销毁窗口对象 C(带释放)  FUN_1400FBDB0 @0x1400fbdb0
 *   销毁带串窗口对象(带释放) FUN_1400FC770 @0x1400fc770
 *   销毁对象(带释放) C      FUN_1400FC8BC @0x1400fc8bc
 *
 * 约定:
 *   - 新实现函数使用 PECMD_ 可读名；未实现依赖仍 extern FUN_ + TODO(verify)
 *   - 全局数据符号先用 extern 声明; 完整链接时统一在 core_globals.c 定义
 * ==================================================================== */
#include <stdlib.h>
#include <stdint.h>

#include "pecmd_defs.h"

/* ---- 已实现公共工具 (其他 core_*.c) ---- */
extern void PECMD_DtorCtlCoreObj(int64_t obj);            /* @0x1400f06fc */
extern void FUN_1400F1F18(uint64_t *obj);      /* @0x1400f1f18 */
extern void PECMD_DtorIpadSubObj(uint64_t *obj);      /* @0x1400f2924 */
extern void PECMD_DtorListViewObj(uint64_t *obj);      /* @0x1400fba3c */
extern void FUN_1400FC034(uint64_t *obj); /* @0x1400fc034 */

/* ---- 未实现依赖 (extern + TODO(verify)) ---- */
extern void FUN_1400F5D50(uint64_t *obj);
extern void PECMD_DestroyCtlResources(uint64_t *obj); /* @0x1400f9324 */
extern void FUN_1400FC33C(uint64_t *obj); /* @0x1400fc33c core_b8c.c */

/* ========== FUN_1400F0F78 @0x1400f0f78 ==========
 * 销毁 GDI 对象；若 flag 低位置 1 则同时释放对象内存。
 */
uint64_t *FUN_1400F0F78(uint64_t *obj, uint32_t flags)
{
    PECMD_DtorCtlCoreObj((int64_t)obj);
    if ((flags & 1) != 0) {
        free(obj);
    }
    return obj;
}

/* ========== FUN_1400F22AC @0x1400f22ac ==========
 * 销毁窗口对象 A；若 flag 低位置 1 则同时释放对象内存。
 */
uint64_t *FUN_1400F22AC(uint64_t *obj, uint32_t flags)
{
    FUN_1400F1F18(obj);
    if ((flags & 1) != 0) {
        free(obj);
    }
    return obj;
}

/* ========== FUN_1400F22DC @0x1400f22dc ==========
 * 通过对象 vtable 第 2 个槽发送消息 0x202。
 */
void FUN_1400F22DC(uint64_t *obj, uint32_t flags, uint64_t lParam)
{
    (**(void (**)(uint64_t, uint32_t, uint32_t, uint64_t))(*obj + 8))
        (obj[4], 0x202, flags,
         (int64_t)(int32_t)((int32_t)((uint64_t)lParam >> 0x20) << 0x10 | (uint32_t)lParam));
}

/* ========== FUN_1400F2A4C @0x1400f2a4c ==========
 * 销毁窗口对象 B；若 flag 低位置 1 则同时释放对象内存。
 */
uint64_t *FUN_1400F2A4C(uint64_t *obj, uint32_t flags)
{
    PECMD_DtorIpadSubObj(obj);
    if ((flags & 1) != 0) {
        free(obj);
    }
    return obj;
}

/* ========== FUN_1400F51A8 @0x1400f51a8 ==========
 * 通过对象 vtable 第 2 个槽发送消息 0x204。
 */
void FUN_1400F51A8(uint64_t *obj, uint32_t flags, uint64_t lParam)
{
    (**(void (**)(uint64_t, uint32_t, uint32_t, uint64_t))(*obj + 8))
        (obj[4], 0x204, flags,
         (int64_t)(int32_t)((int32_t)((uint64_t)lParam >> 0x20) << 0x10 | (uint32_t)lParam));
}

/* ========== FUN_1400F9104 @0x1400f9104 ==========
 * 销毁内部对象；若 flag 低位置 1 则同时释放对象内存。
 */
uint64_t *FUN_1400F9104(uint64_t *obj, uint32_t flags)
{
    FUN_1400F5D50(obj);
    if ((flags & 1) != 0) {
        free(obj);
    }
    return obj;
}

/* ========== FUN_1400FB558 @0x1400fb558 ==========
 * 销毁内部对象 B；若 flag 低位置 1 则同时释放对象内存。
 */
uint64_t *FUN_1400FB558(uint64_t *obj, uint32_t flags)
{
    PECMD_DestroyCtlResources(obj);
    if ((flags & 1) != 0) {
        free(obj);
    }
    return obj;
}

/* ========== FUN_1400FBDB0 @0x1400fbdb0 ==========
 * 销毁窗口对象 C；若 flag 低位置 1 则同时释放对象内存。
 */
uint64_t *FUN_1400FBDB0(uint64_t *obj, uint32_t flags)
{
    PECMD_DtorListViewObj(obj);
    if ((flags & 1) != 0) {
        free(obj);
    }
    return obj;
}

/* ========== FUN_1400FC770 @0x1400fc770 ==========
 * 销毁带串窗口对象；若 flag 低位置 1 则同时释放对象内存。
 */
uint64_t *FUN_1400FC770(uint64_t *obj, uint32_t flags)
{
    FUN_1400FC034(obj);
    if ((flags & 1) != 0) {
        free(obj);
    }
    return obj;
}

/* ========== FUN_1400FC8BC @0x1400fc8bc ==========
 * 销毁内部对象 C；若 flag 低位置 1 则同时释放对象内存。
 */
uint64_t *FUN_1400FC8BC(uint64_t *obj, uint32_t flags)
{
    FUN_1400FC33C(obj);
    if ((flags & 1) != 0) {
        free(obj);
    }
    return obj;
}
