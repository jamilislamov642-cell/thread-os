#include "thread.h"
#include "mem.h"
#include "string.h"
#include "io.h"

extern void switch_to(uint32_t **prev_esp, uint32_t *next_esp);
extern void thread_entry_wrapper(void);
extern uint32_t timer_ticks(void);

static thread_t threads[MAX_THREADS];
static thread_t *current_thread = (thread_t *)0;
static int next_tid = 1;
static int preemption_enabled = 0;
static int rr_cursor = 0;

static const int quantum_by_prio[4] = { 8, 5, 3, 1 };

static void idle_fn(void *arg) {
    (void)arg;
    for (;;) {
        hlt();
    }
}

void thread_init(void) {
    memset(threads, 0, sizeof(threads));
    for (int i = 0; i < MAX_THREADS; i++) {
        threads[i].state = THREAD_FREE;
        threads[i].tid = -1;
    }

    // Thread 0: current execution context (kernel main / shell)
    threads[0].tid = 0;
    strcpy(threads[0].name, "kernel_main");
    threads[0].state = THREAD_RUNNING;
    threads[0].stack_base = (void *)0; // already using boot stack
    threads[0].stack_size = 0;
    threads[0].priority = 1;
    threads[0].quantum = quantum_by_prio[1];
    threads[0].runtime_ticks = 0;
    threads[0].switches = 1;
    current_thread = &threads[0];

    // Idle thread: priority 3 (lowest), runs hlt when nothing else is ready
    thread_create("idle", idle_fn, (void *)0, 3);
}

void thread_enable_preemption(void) {
    preemption_enabled = 1;
}

void thread_disable_preemption(void) {
    preemption_enabled = 0;
}

int thread_create(const char *name, thread_entry_fn fn, void *arg, int priority) {
    if (priority < 0) priority = 0;
    if (priority > 3) priority = 3;

    int slot = -1;
    for (int i = 0; i < MAX_THREADS; i++) {
        if (threads[i].state == THREAD_FREE) {
            slot = i;
            break;
        }
    }
    // If no free slot, reuse a dead thread
    if (slot == -1) {
        for (int i = 0; i < MAX_THREADS; i++) {
            if (threads[i].state == THREAD_DEAD) {
                if (threads[i].stack_base) {
                    kfree(threads[i].stack_base);
                    threads[i].stack_base = (void *)0;
                }
                slot = i;
                break;
            }
        }
    }
    if (slot == -1) return -1;

    void *stack = kmalloc(THREAD_STACK_SIZE);
    if (!stack) return -1;

    thread_t *t = &threads[slot];
    t->tid = next_tid++;
    strncpy(t->name, name ? name : "task", 19);
    t->name[19] = '\0';
    t->stack_base = stack;
    t->stack_size = THREAD_STACK_SIZE;
    t->priority = priority;
    t->quantum = quantum_by_prio[priority];
    t->runtime_ticks = 0;
    t->switches = 0;
    t->sleep_until = 0;

    // Build initial stack frame for switch_to -> thread_entry_wrapper.
    // Restoration pops: popfd, pop ebp, pop edi, pop esi, pop ebx, ret.
    // Stack grows downwards, so push in reverse restoration order:
    uint32_t *top = (uint32_t *)((uint32_t)stack + THREAD_STACK_SIZE);
    top = (uint32_t *)((uint32_t)top & ~7u);

    *(--top) = (uint32_t)thread_entry_wrapper; // RET address (highest address)
    *(--top) = (uint32_t)fn;                   // EBX (thread entry function)
    *(--top) = (uint32_t)arg;                  // ESI (argument pointer)
    *(--top) = 0;                              // EDI
    *(--top) = 0;                              // EBP
    *(--top) = 0x202;                          // EFLAGS: IF=1 (lowest address, top of stack)

    t->esp = top;
    t->state = THREAD_READY;
    return t->tid;
}

static thread_t *pick_next_thread(void) {
    for (int p = 0; p <= 3; p++) {
        for (int step = 0; step < MAX_THREADS; step++) {
            int i = (rr_cursor + step) % MAX_THREADS;
            if (threads[i].state == THREAD_READY && threads[i].priority == p) {
                rr_cursor = (i + 1) % MAX_THREADS;
                return &threads[i];
            }
        }
    }
    return (thread_t *)0;
}

void thread_schedule(void) {
    thread_t *prev = current_thread;
    thread_t *next = pick_next_thread();

    if (!next) {
        if (prev && prev->state == THREAD_RUNNING) {
            prev->quantum = quantum_by_prio[prev->priority];
            return;
        }
        return;
    }

    if (prev == next && prev->state == THREAD_RUNNING) {
        prev->quantum = quantum_by_prio[prev->priority];
        return;
    }

    if (prev && prev->state == THREAD_RUNNING) {
        prev->state = THREAD_READY;
    }

    next->state = THREAD_RUNNING;
    next->quantum = quantum_by_prio[next->priority];
    next->switches++;
    current_thread = next;

    switch_to(&prev->esp, next->esp);
}

void thread_yield(void) {
    if (current_thread) {
        current_thread->quantum = 0;
    }
    thread_schedule();
}

void thread_sleep(uint32_t ms) {
    if (!current_thread) return;
    current_thread->sleep_until = timer_ticks() + (ms + 9) / 10;
    current_thread->state = THREAD_SLEEPING;
    thread_schedule();
}

void thread_wake_sleeping(uint32_t ticks) {
    for (int i = 0; i < MAX_THREADS; i++) {
        if (threads[i].state == THREAD_SLEEPING && ticks >= threads[i].sleep_until) {
            threads[i].state = THREAD_READY;
        }
    }
}

void thread_exit(void) {
    if (current_thread) {
        current_thread->state = THREAD_DEAD;
    }
    thread_schedule();
    for (;;) hlt();
}

int thread_kill(int tid) {
    if (tid == 0) return -1; // never kill kernel main
    if (current_thread && current_thread->tid == tid) {
        thread_exit();
        return 0;
    }
    for (int i = 0; i < MAX_THREADS; i++) {
        if (threads[i].tid == tid && threads[i].state != THREAD_FREE && threads[i].state != THREAD_DEAD) {
            threads[i].state = THREAD_DEAD;
            return 0;
        }
    }
    return -1;
}

void thread_tick(void) {
    if (current_thread) {
        current_thread->runtime_ticks++;
    }
    thread_wake_sleeping(timer_ticks());
    if (!preemption_enabled) return;
    if (current_thread && current_thread->state == THREAD_RUNNING) {
        current_thread->quantum--;
        if (current_thread->quantum <= 0) {
            thread_schedule();
        }
    }
}

thread_t *thread_current(void) {
    return current_thread;
}

int thread_get_all(thread_t **out_list) {
    if (out_list) *out_list = threads;
    return MAX_THREADS;
}

int thread_count_active(void) {
    int n = 0;
    for (int i = 0; i < MAX_THREADS; i++) {
        if (threads[i].state != THREAD_FREE && threads[i].state != THREAD_DEAD) {
            n++;
        }
    }
    return n;
}
