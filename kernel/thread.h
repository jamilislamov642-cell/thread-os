#ifndef THREAD_H
#define THREAD_H

#include <stdint.h>
#include <stddef.h>

#define MAX_THREADS 32
#define THREAD_STACK_SIZE 8192

typedef enum {
    THREAD_FREE = 0,
    THREAD_READY,
    THREAD_RUNNING,
    THREAD_SLEEPING,
    THREAD_BLOCKED,
    THREAD_DEAD
} thread_state_t;

typedef struct thread {
    int tid;
    char name[20];
    thread_state_t state;
    uint32_t *esp;
    void *stack_base;
    uint32_t stack_size;
    uint32_t sleep_until;
    uint32_t runtime_ticks;
    uint32_t switches;
    int priority;      // 0 = highest, 3 = idle
    int quantum;
} thread_t;

typedef void (*thread_entry_fn)(void *arg);

void thread_init(void);
int thread_create(const char *name, thread_entry_fn fn, void *arg, int priority);
void thread_yield(void);
void thread_sleep(uint32_t ms);
void thread_exit(void);
int thread_kill(int tid);
void thread_schedule(void);
void thread_tick(void);
void thread_wake_sleeping(uint32_t ticks);
thread_t *thread_current(void);
int thread_get_all(thread_t **out_list);
int thread_count_active(void);
void thread_enable_preemption(void);
void thread_disable_preemption(void);

#endif
