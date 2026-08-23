/*
 * core_exec3.c — 命令执行环境（B8a）
 *
 *   PECMD_SetCurFileVariables   @0x14002452c   CurFile/CurDir/CurDrv 变量设置
 *   PECMD_SetEnvIfChanged    @0x140061508   环境变量设置（值不同才写）
 *   PECMD_SetVarEn    @0x1400629b8   (已声明 FUN_1400629B8) 变量写（锁/直写分派）
 *   PECMD_EncodeBuffer   @0x140068984   资源解码加载
 *   FUN_140061C44      @0x140061c44   OLE/COM 初始化
 *   FUN_14005B7E8  @0x14005b7e8   托盘图标清理
 *   FUN_14001BB30  @0x14001bb30   钩子/资源清理
 *   PECMD_WaitCountPumpMessages  @0x14001bbdc   等待条件 + 消息泵
 *   PECMD_ShutdownCom  @0x140064328   退出清理（COM/回调）
 *   FUN_14006E8F4   @0x14006e8f4   脚本释放（引用计数）
 *   FUN_14004E2CC     @0x14004e2cc   任务释放
 *   FUN_14004EAA8    @0x14004eaa8   任务表清空
 *   FUN_140017F54   @0x140017f54   内存映射释放
 */
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>

#include "pecmd_defs.h"
extern void FUN_1400629B8(void *script, LPCWSTR key, LPCWSTR value); /* @0x1400629b8 */

extern void FUN_14005B0B8(void *p, size_t len);         /* @0x14005b0b8 */
extern DWORD FUN_14006459C(LPCWSTR src, uint32_t buflen, LPWSTR buf, LPWSTR *last); /* @0x14006459c */
extern void FUN_14004E2CC(int64_t table, int64_t *task);  /* @0x14004e2cc */
extern void FUN_14006E8F4(int64_t table);               /* @0x14006e8f4 */

/* ========== PECMD_SetEnvIfChanged @0x140061508 ==========
 * 设置环境变量（仅当与当前值不同时写入）。
 */
void PECMD_SetEnvIfChanged(LPCWSTR name, LPCWSTR value)
{
    WCHAR cur[0x208];
    cur[0] = L'\0';
    GetEnvironmentVariableW(name, cur, 0x208);
    if (lstrcmpW(value, cur) != 0) {
        SetEnvironmentVariableW(name, value);
    }
}

/* ========== PECMD_SetCurFileVariables @0x14002452c ==========
 * 设置 &&CurFile/&&CurDir/&&CurDrv 等脚本变量（B8a 核心）。
 *   script: 脚本表；curfile: 当前文件路径；flag: 0x40 = 只设变量不改目录。
 */
int32_t PECMD_SetCurFileVariables(void *script, LPCWSTR curfile, uint32_t flag)
{
    bool bSet = false;
    WCHAR *cur = NULL;
    LPCWSTR p = curfile;
    WCHAR *file = NULL;
    LPCWSTR raw = curfile;

    if (((flag >> 0x10 & 1) != 0) || ((flag & 0x40) == 0)) bSet = true;

    PECMD_AllocWStringBuffer(&cur, 0x41c);
    *cur = L'\0';
    GetCurrentDirectoryW(0x208, cur);
    FUN_1400629B8(script, WSTR("&&__OldDir"), cur);
    FUN_1400629B8(script, WSTR("&&CurFile.real"), curfile);

    if (curfile != NULL) {
        FUN_1400629B8(script, WSTR("&&CurFile.Raw"), curfile);
    }

    /* 特殊内部名 **#101 / **map: */
    if (curfile != NULL && !(curfile[0] == L'#' && curfile[1] != L'\0' && curfile[1] < 0x90)) {
        if (lstrcmpW(WSTR("**#101"), curfile) == 0) {
            p = WSTR("#101");
        } else {
            if (curfile[0] == L'*' && curfile[1] == L'*' &&
                curfile[2] == L'm' && curfile[3] == L'a' && curfile[4] == L':' &&
                curfile[5] == L':') {
                /* **map: 前缀 — 跳过 */
                p = curfile + 2;
            } else {
                /* 普通路径：去引号/前缀 */
                WCHAR *s = (WCHAR *)curfile;
                if (*s == L'"') {
                    s++;
                    WCHAR *e = s;
                    while (*e != L'\0' && *e != L'"') e++;
                    if (*e != L'\0') {
                        *e = L'\0';
                        e++;
                    }
                    /* TODO(verify): 引号后残留并入 file */
                    p = s;
                } else {
                    if (*s == L'*') s++;
                    p = s;
                }
            }
        }
    }

    /* file 部分：去目录 */
    PECMD_AllocString(&file, 0x105);
    {
        LPWSTR last = NULL;
        FUN_14006459C(p, 0x104, file, &last);
        FUN_1400629B8(script, WSTR("&&CurFile.File"), file);
        raw = file;
    }

    /* 无盘符路径 → 用模块名补全 */
    if (raw == NULL || *raw == L'\0') {
        GetModuleFileNameW(g_hInstance, cur, 0x104);
        FUN_1400629B8(script, WSTR("&&CurFile.File"), cur);
        file = cur;
    }

    /* 大写首字母 */
    if (file[0] > 0x60 && file[0] < 0x7b) file[0] = file[0] & 0xffdf;

    FUN_1400629B8(script, WSTR("&&CurFile"), file);
    if (bSet) PECMD_SetEnvIfChanged(WSTR("CurFile"), file);

    /* 目录部分 */
    {
        WCHAR *slash = StrRChrW(file, NULL, L'\\');
        if (slash != NULL) slash[1] = L'\0';
        if ((flag & 0x40) == 0) {
            SetCurrentDirectoryW(file);
        }
        if (slash != NULL) *slash = L'\0';
        FUN_1400629B8(script, WSTR("&&CurDir"), file);
        if (bSet) PECMD_SetEnvIfChanged(WSTR("CurDir"), file);

        /* 盘符部分 */
        file[2] = L'\0';
        FUN_1400629B8(script, WSTR("&&CurDrv"), file);
        if (bSet) PECMD_SetEnvIfChanged(WSTR("CurDrv"), file);
    }

    FUN_14005B104(&file);
    FUN_14005B104(&cur);
    return 0;
}

/* ========== FUN_14001BB30 @0x14001bb30 ==========
 * 清理钩子/堆块（托盘图标 + 消息钩子）。
 */
void FUN_14001BB30(void)
{
    EnterCriticalSection(&g_csInit);
    EnterCriticalSection(&g_csHook);
    if (g_hHook != 0) {
        UnhookWindowsHookEx(g_hHook);
    }
    g_hHook = 0;
    if (g_pHookData != 0) {
        HeapFree(GetProcessHeap(), 0, g_pHookData);
    }
    LeaveCriticalSection(&g_csHook);
    LeaveCriticalSection(&g_csInit);
}

/* ========== PECMD_WaitCountPumpMessages @0x14001bbdc ==========
 * 等待 *count <= limit，期间泵消息。
 */
void PECMD_WaitCountPumpMessages(int *count, int limit)
{
    HANDLE ev = g_hWaitEvent;
    MSG msg;
    if (ev == 0) {
        ev = CreateEventW(NULL, 0, 1, NULL);
        g_hWaitEvent = ev;
    }
    while (limit < *count) {
        MsgWaitForMultipleObjects(1, &ev, 0, 1000, 0x4ff);
        while (PeekMessageW(&msg, (HWND)0, 0, 0, 1)) {
            TranslateMessage(&msg);
            DispatchMessageW(&msg);
        }
    }
}

/* ========== FUN_14005B7E8 @0x14005b7e8 ==========
 * 托盘图标清理（NOTIFYICONDATA 结构）。
 */
void FUN_14005B7E8(char *nid)
{
    EnterCriticalSection(&g_csInit);
    if (*nid != '\0') {
        Shell_NotifyIconW(2, (PNOTIFYICONDATAW)(nid + 0x10));
    }
    *nid = '\0';
    memset(nid + 0x10, 0, 0x3d0);
    nid[0x10] = (char)0xd0;   /* cbSize 低字节 (sizeof-4=0x3d0 实际 TODO(verify)) */
    nid[0x11] = 0x03;
    nid[0x12] = 0;
    nid[0x13] = 0;
    LeaveCriticalSection(&g_csInit);
}

/* ========== FUN_140061C44 @0x140061c44 ==========
 * 惰性加载 OLE32 COM 函数指针（失败静默）。
 */
int32_t FUN_140061C44(void)
{
    /* TODO(verify): 原实现 GetProcAddress 加载 OLE32 导出；
     *   g_guidPtrState 为 StringFromGUID2 指针（非 0 即已初始化）。
     * 本重构直接返回 0（链接时 stub 提供）。
     */
    return 0;
}

/* ========== FUN_140017F54 @0x140017f54 ==========
 * 释放映射资源块（type==1 时）。
 */
void FUN_140017F54(int *blk)
{
    if (*blk == 1 && *(int64_t *)(blk + 6) != 0) {
        if (g_pMapBlk == blk) g_pMapBlk = NULL;
        if (*(int64_t *)(blk + 6) != 0) {
            (*(void (**)(int64_t, int, int))(blk + 2))(*(int64_t *)(blk + 6), 0, 0);
        }
        if (*(void **)(blk + 6) != NULL) {
            VirtualFree(*(void **)(blk + 6), 0, 0x8000);
        }
        *blk = 0;
        blk[6] = 0;
        blk[7] = 0;
    }
}

/* ========== PECMD_ShutdownCom @0x140064328 ==========
 * 退出清理：COM 回调 + 任务释放 + 脚本清理。
 */
void PECMD_ShutdownCom(void)
{
    int r;
    if (g_pComState != NULL) {
        EnterCriticalSection(&g_csCom);
        FUN_140061C44();
        r = -3;
        if (g_pOleInit != NULL) {
            r = ((int (*)(int))g_pOleInit)(0);
            if (r != 0 && r == 1) {
                ((void (*)(void))g_pOleUninit)();
            }
        }
        g_pComState = NULL;
        FUN_140017F54((int *)g_pComState);
        free(g_pComState);
        PECMD_WaitCountPumpMessages(&g_taskCount, 2);
        if (g_pOleUninit != NULL && r == 0) {
            ((void (*)(void))g_pOleUninit)();
        }
        LeaveCriticalSection(&g_csCom);
    }
}

/* ========== FUN_14004EAA8 @0x14004eaa8 ==========
 * 清空任务表：释放索引 >= min 的任务。
 */
void FUN_14004EAA8(int64_t table, int min)
{
    int n;
    if (min < 0) min = 0;
    if (*(int64_t *)table != 0) {
        n = *(int32_t *)(table + 8);
        while (min < n) {
            *(int32_t *)(table + 8) = *(int32_t *)(table + 8) - 1;
            int64_t *task = *(int64_t **)(*(int64_t *)table + (int64_t)*(int32_t *)(table + 8) * 8);
            if (task != NULL) {
                FUN_14004E2CC(table, task);
                *(int64_t *)(*(int64_t *)table + (int64_t)*(int32_t *)(table + 8) * 8) = 0;
            }
            n = *(int32_t *)(table + 8);
        }
        if (*(int32_t *)(table + 8) < 1) {
            if (*(int64_t *)table != 0) {
                HeapFree(g_hHeap, 0, (void *)(*(int64_t *)table - 8));
            }
            *(int64_t *)table = 0;
        }
    }
    FUN_14006E8F4(table);
}

/* ========== FUN_14004E2CC @0x14004e2cc ==========
 * 释放任务节点：拼接脚本行再执行或调回调，释放资源。
 */
void FUN_14004E2CC(int64_t table, int64_t *task)
{
    LPCWSTR lpString = (LPCWSTR)task[2];   /* 前缀 */
    LPCWSTR lpStr1 = (LPCWSTR)task[1];     /* 命令 */
    LPCWSTR lpStr0 = (LPCWSTR)task[0];     /* 回调 */
    WCHAR *buf;
    WCHAR *old;

    if (lpString != NULL) {
        int i2 = lstrlenW(lpString);
        int i3 = lstrlenW(lpStr1);
        int i4 = lstrlenW(lpStr0);
        old = (WCHAR *)task[2];
        task[2] = 0;
        if (*lpString == L'*') {
            /* 内置命令 **..^^ */
            if (*(int64_t *)(lpString - 4) < 5 || lpString[1] != L'\0' || lpString[2] != L'^') {
                /* TODO(verify): 递归执行 */
            } else {
                (*(void (**)(void))(*(int64_t *)(lpString + 4) + 8))();
            }
        } else {
            /* 拼接 "prefix &cmd 回调" 并执行 */
            PECMD_AllocWStringBuffer(&buf, (int64_t)i2 + i3 + i4 + 4);
            lstrcpyW(buf, lpString);
            buf[i2] = L' ';
            buf[i2+1] = L'&';
            lstrcpyW(buf + i2 + 2, lpStr1);
            buf[i2 + 2 + i3] = L' ';
            lstrcpyW(buf + i2 + 3 + i3, lpStr0);
            /* TODO(verify): FUN_140045C90(table, buf) */
            FUN_14005B104(&buf);
        }
        if ((*(uint8_t *)(table + 0x11) & 1) != 0) {
            FUN_14005B0B8(old, 0);
        }
        FUN_14005B104((WCHAR **)&old);
    }
    if ((*(uint8_t *)(table + 0x11) & 1) != 0) {
        FUN_14005B0B8((void *)*task, 0);
    }
    free(task);
}

/* ========== FUN_14006E8F4 @0x14006e8f4 ==========
 * 脚本释放：释放 COM/OLE 锁、任务表、子任务引用。
 */
void FUN_14006E8F4(int64_t table)
{
    int64_t *tasks;
    int i;

    if (*(uint8_t *)(table + 0x12) != 0) {
        if ((*(uint8_t *)(table + 0x12) & 4) != 0 && g_pOleUninit != NULL) {
            g_pOleUninit();
        }
        if ((*(uint8_t *)(table + 0x12) & 2) != 0 && g_pOleUninit != NULL) {
            g_pOleUninit();
            LeaveCriticalSection(&g_csCom);
        }
        if ((*(uint8_t *)(table + 0x12) & 1) != 0) {
            LeaveCriticalSection(&g_csInit);
        }
        *(uint8_t *)(table + 0x12) = 0;
    }

    tasks = *(int64_t **)(table + 0xe0);
    if (tasks != NULL) {
        int maxRef = 0;
        if (*(int32_t *)(table + 0xe8) > 0) {
            maxRef = *(int32_t *)(*tasks + 0x14);
        }
        if (*(int64_t *)(table + 0x38) == 0 ||
            *(int64_t *)(table + 0x38) == (int64_t)&g_Script) {
            maxRef = 0;
        }
        /* 引用计数裁剪 */
        for (i = 1; i < *(int32_t *)(table + 0xe8); i++) {
            int ref = *(int32_t *)(tasks[i] + 0x14);
            if (ref < 0) *(uint32_t *)(tasks[i] + 0x14) = 0;
            else if (maxRef < ref) *(int32_t *)(tasks[i] + 0x14) = maxRef;
            else if (ref < maxRef) maxRef = ref;
        }
        /* 释放无父任务 */
        while (*(int32_t *)(table + 0xe8) > 0) {
            int last = *(int32_t *)(table + 0xe8) - 1;
            if (*(int32_t *)(tasks[last] + 0x14) > 0 && *(int64_t *)(table + 0x38) != 0) break;
            *(int32_t *)(table + 0xe8) = last;
            (*(void (**)(void))(*(int64_t *)tasks[last] + 8))();
            tasks[*(int32_t *)(table + 0xe8)] = 0;
        }
        /* 父任务引用递减 */
        if (*(int64_t *)(table + 0x38) != 0) {
            for (i = 0; i < *(int32_t *)(table + 0xe8); i++) {
                if (*(int32_t *)(*tasks + 0x14) > 0) {
                    *(int32_t *)(*tasks + 0x14) = *(int32_t *)(*tasks + 0x14) - 1;
                }
                tasks[*(int32_t *)(table + 0xe8)] = 0;
            }
        }
        if (*(int64_t *)(table + 0xe0) != 0) {
            HeapFree(g_hHeap, 0, (void *)(*(int64_t *)(table + 0xe0) - 8));
        }
        *(int64_t *)(table + 0xe0) = 0;
        *(int32_t *)(table + 0xe8) = 0;
    }
}
