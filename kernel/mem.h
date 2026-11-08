#ifndef MEM_H
#define MEM_H

#include <stdint.h>
#include <stddef.h>

void kmem_init(uint32_t start, uint32_t size);
void *kmalloc(size_t size);
void kfree(void *ptr);
void *kalloc_stack(size_t size);
uint32_t kmem_used(void);
uint32_t kmem_available(void);
uint32_t kmem_total(void);

#endif
