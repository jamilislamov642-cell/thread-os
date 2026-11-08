#include "sync.h"
#include "thread.h"
#include "io.h"

void spinlock_init(spinlock_t *s) {
    s->locked = 0;
}

void spinlock_acquire(spinlock_t *s) {
    while (__atomic_test_and_set(&s->locked, __ATOMIC_ACQUIRE)) {
        thread_yield();
    }
}

void spinlock_release(spinlock_t *s) {
    __atomic_clear(&s->locked, __ATOMIC_RELEASE);
}

void mutex_init(mutex_t *m) {
    m->locked = 0;
    m->owner_tid = -1;
}

void mutex_lock(mutex_t *m) {
    while (__atomic_test_and_set(&m->locked, __ATOMIC_ACQUIRE)) {
        thread_yield();
    }
    m->owner_tid = thread_current() ? thread_current()->tid : -1;
}

void mutex_unlock(mutex_t *m) {
    m->owner_tid = -1;
    __atomic_clear(&m->locked, __ATOMIC_RELEASE);
}

uint32_t atomic_inc(volatile uint32_t *p) {
    return __atomic_add_fetch(p, 1, __ATOMIC_SEQ_CST);
}

uint32_t atomic_add(volatile uint32_t *p, uint32_t val) {
    return __atomic_add_fetch(p, val, __ATOMIC_SEQ_CST);
}
