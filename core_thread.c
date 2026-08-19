/* ====================================================================
 * core_thread.c — PECMD 线程/等待/窗口枚举/引用计数族
 *
 * 来源: PECMD原始.EXE (x64)
 *   PECMD_GrowByteBuffer  @0x140063424   (字节缓冲扩容)
 *   FUN_1400633A8       @0x1400633a8   (分配 len+9 字节)
 *   FUN_14005B0B8      @0x14005b0b8   (按头长度清零)
 *   PECMD_ReleaseRefCount       @0x140017110   (引用计数减/释放)
 *   FUN_1400660AC  @0x1400660ac   (前缀词比较 + 推进)
 *   FUN_14001FD60 @0x14001fd60  (SendMessage 子线程)
 *   PECMD_EnumWindowsCallback  @0x14001fde4   (EnumWindows 回调: 每窗口建线程)
 *   PECMD_ThreadMainLoop   @0x14001ff24   (线程主函数: 等句柄数组)
 *   FUN_1400195F0     @0x1400195f0   (消息等待循环)
 *   FUN_140027EAC    @0x140027eac   (任务线程创建)
 * ==================================================================== */
#include <stdint.h>
#include <stddef.h>
#include <string.h>

#include "pecmd_defs.h"
extern void *FUN_140063118(void *ptr, size_t size); /* @0x140063118 */
extern WCHAR **FUN_14005B154(WCHAR **pp); /* @0x14005b154 */

/* 全局 */
extern uint32_t g_threadId;      /* g_dwC96C 线程 ID 输出 */

/* ========== 字节缓冲扩容 @0x140063424 ========== */
void *PECMD_GrowByteBuffer(void **ps, int64_t len)
{
    void *p = FUN_140063118(*ps, (size_t)len + 1);
    *ps = p;
    if (p != NULL) {
        ((uint8_t *)p)[len] = 0;
    }
    return *ps;
}

/* ========== 分配 (len+9 字节) @0x1400633a8 ========== */
/* 头: size=len+1, magic; 数据首字节写 0 */
void FUN_1400633A8(void **ps, int64_t len)
{
    uint8_t *hdr;

    *ps = NULL;
    if (len >= 0) {
        for (;;) {
            hdr = (uint8_t *)HeapAlloc(g_hHeap, 0, (size_t)len + 9);
            if (hdr) break;
            FUN_1400630D0(2);
        }
        *(size_t *)hdr = (size_t)len + 1;
        *(uint32_t *)(hdr + 4) = 0xaa55;
        *ps = hdr + 8;
        if (*ps != NULL) {
            *(uint8_t *)*ps = 0;
        }
    }
}

/* ========== 按头长度清零 @0x14005b0b8 ========== */
/* p[-1] = size (FUN_1400633A8 的头) */
void FUN_14005B0B8(void *p)
{
    if (p != NULL) {
        memset(p, 0, ((size_t *)p)[-1]);
    }
}

/* ========== 引用计数减 @0x140017110 ========== */
/* 计数到 0 释放 (FUN_14005B0D4 = FUN_14005B104), 返回 NULL */
void *PECMD_ReleaseRefCount(void **pp)
{
    void *p;

    EnterCriticalSection(&g_csInit);
    p = *pp;
    if (p == NULL) {
        LeaveCriticalSection(&g_csInit);
        return NULL;
    }
    if (*(int *)p > 0) {
        *(int *)p = *(int *)p - 1;
        if (*(int *)*pp < 1) {
            FUN_14005B104((WCHAR **)pp);
            *pp = NULL;
            LeaveCriticalSection(&g_csInit);
            return NULL;
        }
    }
    LeaveCriticalSection(&g_csInit);
    return p;
}

/* ========== 前缀词比较 @0x1400660ac ========== */
/* 匹配 word (ASCII) 前 n 字符且为完整词, 则 *pp 推进并跳空白 */
char FUN_1400660AC(const char *word, WCHAR **pp, int n)
{
    WCHAR *p;
    int r;

    if (n < 0) {
        n = (int)lstrlenA(word);
    }
    p = *pp;
    r = FUN_14005C788(word, p, n);   /* FUN_14005C72C 等价实现 */
    if (r != 0) {
        *pp = p + n;
        FUN_14005B154(pp);
    }
    return (char)r;
}

/* ========== SendMessage 子线程 @0x14001fd60 ========== */
/* 任务结构: +0x18 消息 +0x20 wParam +0x28 lParam +0x30 flags
 *           +0x38 timeout +0x40 hwnd +0x48 回调(引用计数) */
uint64_t FUN_14001FD60(void *task)
{
    uint64_t refs[4];
    uint32_t *msg = (uint32_t *)((uint8_t *)task + 0x18);

    refs[1] = 0;
    refs[0] = *(uint64_t *)((uint8_t *)task + 0x48);   /* 回调 */
    refs[2] = (uint64_t)task;
    if (*(uint32_t *)((uint8_t *)task + 0x38) == 0xffffffff) {
        SendMessageW(*(HWND *)((uint8_t *)task + 0x40), msg[0],
                     *(WPARAM *)((uint8_t *)task + 0x20),
                     *(LPARAM *)((uint8_t *)task + 0x28));
    } else {
        SendMessageTimeoutW(*(HWND *)((uint8_t *)task + 0x40), msg[0],
                            *(WPARAM *)((uint8_t *)task + 0x20),
                            *(LPARAM *)((uint8_t *)task + 0x28),
                            *(uint32_t *)((uint8_t *)task + 0x30),
                            *(uint32_t *)((uint8_t *)task + 0x38),
                            (DWORD *)(refs + 1));
    }
    PECMD_ReleaseRefCount((void **)&refs[0]);
    refs[0] = 0;
    FUN_14005B104((WCHAR **)&refs[0]);
    FUN_14005B104((WCHAR **)&refs[2]);
    return 0;
}

/* ========== EnumWindows 回调 @0x14001fde4 ========== */
/* 对每个顶层窗口创建 SendMessage 线程; 句柄存入任务数组 */
int PECMD_EnumWindowsCallback(HWND hwnd, void *ctx)
{
    uint64_t *c = (uint64_t *)ctx;
    void *task;
    HANDLE hThread;
    int64_t idx = (int64_t)c[2];           /* +0x10 当前数 */
    int64_t cap = (int64_t)c[3];           /* +0x18 容量 */

    if (cap <= idx) {
        cap += 0x40;
        c[3] = (uint64_t)cap;
        PECMD_GrowByteBuffer((void **)&c[1], cap * 0x18);
    }
    {
        uintptr_t *arr = (uintptr_t *)c[1];  /* +0x08 句柄数组 */
        FUN_1400633A8(&task, 0x60);
        FUN_14005B0B8(task);
        {
            uint8_t *t = (uint8_t *)task;
            *(uint32_t *)(t + 0x18) = *(uint32_t *)((uint8_t *)c + 0x20);
            *(uint64_t *)(t + 0x20) = c[5];  /* +0x28 */
            *(uint64_t *)(t + 0x28) = c[6];  /* +0x30 */
            *(uint32_t *)(t + 0x30) = *(uint32_t *)((uint8_t *)c + 0x38);
            *(uint64_t *)(t + 0x40) = (uint64_t)hwnd;
            *(uint64_t *)(t + 0x38) = c[7];  /* +0x40 超时 */
            *(uint64_t *)(t + 0x48) = c[9];  /* +0x50 回调 */
            *(uint64_t *)(t + 0x50) = (uint64_t)ctx;
        }
        hThread = CreateThread(NULL, 0x10000, (void *)FUN_14001FD60, task,
                               0x10004, &g_threadId);
        arr[idx * 0x18 / 8] = (uintptr_t)hThread;  /* 每项 0x18 字节, 首 qword 存句柄 */
        if (hThread) {
            if (c[9] != 0) {
                EnterCriticalSection(&g_csInit);
                if (c[9] != 0) {
                    (*(int *)c[9])++;
                }
                LeaveCriticalSection(&g_csInit);
            }
            ResumeThread(hThread);
            c[2] = c[2] + 1;
        }
        FUN_14005B104((WCHAR **)&task);
    }
    return 1;   /* 继续枚举 */
}

/* ========== 线程主函数 @0x14001ff24 ========== */
/* 任务: +0x08 句柄数组 +0x10 数量 +0x38 超时; 等全部句柄后清理 */
uint64_t PECMD_ThreadMainLoop(void *task)
{
    void *refs;
    HANDLE *handles;
    DWORD timeout;
    int64_t n;
    int64_t i;
    uint64_t r;

    if (task == NULL) {
        refs = NULL;
    } else {
        refs = (void *)((uint8_t *)task + 8);
    }
    n = (int64_t)((uint64_t *)refs)[9];       /* 回调引用 */
    handles = *(HANDLE **)refs;               /* 句柄数组 */
    timeout = (DWORD)((uint64_t *)refs)[7];   /* 超时 */
    r = (uint64_t)task;

    EnumWindows((void *)PECMD_EnumWindowsCallback, (LONG_PTR)task);

    n = (int64_t)((uint64_t *)refs)[1];       /* 数量 */
    handles = *(HANDLE **)refs;
    if (n > 0) {
        if (timeout != 0xffffffff) {
            timeout++;
        }
        WaitForMultipleObjects((DWORD)n, handles, TRUE, timeout);
    }
    for (i = 0; i < n; i++) {
        CloseHandle(handles[i]);
    }
    PECMD_ReleaseRefCount((void **)&refs);
    FUN_14005B104((WCHAR **)&refs);
    FUN_14005B104((WCHAR **)&r);
    return 0;
}

/* ========== 消息等待循环 @0x1400195f0 ========== */
/* 等待句柄/消息直到超时 (timeout ms) 或消息数 (maxmsg) 或 g_flagA24F=0 */
DWORD FUN_1400195F0(void *script, int64_t timeout, int maxmsg, void *param4)
{
    HANDLE *handles;
    HANDLE local[2];
    HWND hwnd;
    uint64_t opts;
    uint64_t nCount;
    DWORD start;
    int64_t deadline;
    DWORD r;
    DWORD t;
    MSG msg;
    BOOL got;
    (void)script;

    if (param4 == NULL) {
        hwnd = 0;
        opts = 0;
        handles = local;
    } else {
        hwnd = (HWND)((uint64_t *)param4)[0];
        opts = ((uint64_t *)param4)[1];
        handles = (HANDLE *)((uint64_t *)param4 + 2);
    }
    nCount = opts & 0xffff;
    start = GetTickCount();
    deadline = (uint64_t)start + (uint64_t)timeout;
    if ((opts & 0x20000) == 0) {
        if (timeout < 1 && maxmsg < 1) {
            Sleep(0);
            return 0;
        }
    } else {
        timeout = 10000000;
    }
    r = 0;
    if ((opts & 0x10000) != 0) {
        GetCurrentThreadId();
    }
    while ((opts & 0x20000) != 0 || maxmsg > 0) {
    retry:
        if (g_flagA24F < 1) {
            goto done;
        }
        if (timeout > 0) {
            t = (DWORD)timeout;
            if (t > 100) {
                t = 100;
            }
            if (t > 1000) {
                t = 1000;
            }
            r = MsgWaitForMultipleObjects((DWORD)nCount, handles, (uint32_t)(opts >> 0x10) & 1,
                                          t, 0x4ff);
            if (r == 0xffffffff) {
                if (nCount != 0) {
                    return GetLastError();
                }
                Sleep(1);
            }
            if (r < nCount) {
                return r;
            }
        }
        while (g_flagA24F > 0) {
            if (((opts & 0x20000) == 0) && maxmsg < 1) {
                timeout = deadline - (uint64_t)GetTickCount();
                if ((int)timeout < 1) {
                    break;
                }
            }
            got = PeekMessageW(&msg, hwnd, 0, 0, 3);
            if (!got) {
                maxmsg = 0;
                break;
            }
            TranslateMessage(&msg);
            DispatchMessageW(&msg);
            if (maxmsg > 0) {
                maxmsg--;
            }
        }
        if (r == 0x102) {       /* WM_TIMER */
            maxmsg = 0;
        }
    }
    timeout = deadline - (uint64_t)GetTickCount();
    if ((int)timeout > 0) {
        goto retry;
    }
done:
    if (nCount == 0) {
        return 0;
    }
    if (r == nCount) {
        return 0x102;
    }
    return r;
}

/* ========== 任务线程创建 @0x140027eac ========== */
/* 创建线程执行 PECMD_ThreadMainLoop; 任务结构记录参数/回调/句柄
 * a5 = 回调表, a6 超时, a7 计数, a8 等待 */
uint64_t FUN_140027EAC(void *script, void **pref, uint32_t a3, uint64_t a4,
                         uint64_t a5, uint32_t a6, uint64_t a7, int64_t a8, int a9)
{
    void *task;
    uint8_t *t;
    HANDLE hThread;

    GetCurrentProcess();
    FUN_1400633A8(&task, 0x70);
    FUN_14005B0B8(task);
    t = (uint8_t *)task;
    if (task != NULL) {
        *(uint32_t *)t = 1;              /* 引用计数 */
    }
    if (task != NULL) {
        t = t + 8;
    }
    *(uint32_t *)(t + 0x18) = a3;
    *(uint64_t *)(t + 0x28) = a5;
    *(uint64_t *)(t + 0x20) = a4;
    *(uint64_t *)(t + 0x38) = a7;
    *(uint64_t *)(t + 0x48) = (uint64_t)pref;
    *(uint32_t *)(t + 0x30) = a6;
    *(uint64_t *)(t + 0x58) = (uint64_t)task;
    if (pref != NULL) {
        EnterCriticalSection(&g_csInit);
        if (pref != NULL) {
            (*(int *)pref)++;
        }
        LeaveCriticalSection(&g_csInit);
    }
    hThread = CreateThread(NULL, 0x10000, (void *)PECMD_ThreadMainLoop, task, 0x10004, &g_threadId);
    if (hThread) {
        if (pref != NULL) {
            EnterCriticalSection(&g_csInit);
            (*(int *)pref)++;
            LeaveCriticalSection(&g_csInit);
        }
        EnterCriticalSection(&g_csInit);
        if (task != NULL) {
            (*(int *)task)++;
        }
        LeaveCriticalSection(&g_csInit);
        ResumeThread(hThread);
    }
    if ((int64_t)a7 != -1) {
        a7 = a7 + 5 + a7 / 10;
    }
    if (hThread) {
        if (a8 != 0) {
            uint64_t w[4];
            w[0] = 0;              /* hwnd */
            w[1] = 0;              /* opts */
            w[2] = 1;              /* handles[0] */
            w[3] = (uint64_t)hThread;   /* handles[1] */
            FUN_1400195F0(script, a7 & 0xffffffff, a9, w);
        }
        CloseHandle(hThread);
    }
    PECMD_ReleaseRefCount((void **)&pref);
    pref = NULL;
    FUN_14005B104((WCHAR **)&pref);
    PECMD_ReleaseRefCount((void **)&task);
    task = NULL;
    FUN_14005B104((WCHAR **)&task);
    return 0;
}
