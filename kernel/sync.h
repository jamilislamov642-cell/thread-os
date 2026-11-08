#ifndef SYNC_H
#define SYNC_H

#include <stdint.h>

typedef struct {
    volatile uint32_t locked;
} spinlock_t;

typedef struct {
    volatile int locked;
    int owner_tid;
} mutex_t;

void spinlock_init(spinlock_t *s);
void spinlock_acquire(spinlock_t *s);
void spinlock_release(spinlock_t *s);

void mutex_init(mutex_t *m);
void mutex_lock(mutex_t *m);
void mutex_unlock(mutex_t *m);

uint32_t atomic_inc(volatile uint32_t *p);
uint32_t atomic_add(volatile uint32_t *p, uint32_t val);

#endif
