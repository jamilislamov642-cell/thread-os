#include "mem.h"
#include "string.h"

#define BLOCK_MAGIC 0xDEADBEEF

typedef struct block_header {
    uint32_t magic;
    uint32_t size;          // usable bytes (excluding header)
    uint8_t  is_free;
    struct block_header *next;
} __attribute__((aligned(8))) block_header_t;

#define HEADER_SIZE sizeof(block_header_t)

static block_header_t *heap_first = (block_header_t *)0;
static uint32_t heap_base = 0;
static uint32_t heap_total_size = 0;

static size_t align8(size_t s) {
    return (s + 7) & ~((size_t)7);
}

void kmem_init(uint32_t start, uint32_t size) {
    heap_base = (start + 7) & ~7u;
    size = size - (heap_base - start);
    heap_total_size = size;

    heap_first = (block_header_t *)heap_base;
    heap_first->magic = BLOCK_MAGIC;
    heap_first->size = size - HEADER_SIZE;
    heap_first->is_free = 1;
    heap_first->next = (block_header_t *)0;
}

void *kmalloc(size_t size) {
    if (size == 0 || !heap_first) return (void *)0;

    size = align8(size);
    block_header_t *curr = heap_first;

    while (curr) {
        if (curr->magic != BLOCK_MAGIC) return (void *)0; // corrupted

        if (curr->is_free && curr->size >= size) {
            // Check if we can split
            if (curr->size >= size + HEADER_SIZE + 16) {
                block_header_t *next_block = (block_header_t *)((uint8_t *)curr + HEADER_SIZE + size);
                next_block->magic = BLOCK_MAGIC;
                next_block->size = curr->size - size - HEADER_SIZE;
                next_block->is_free = 1;
                next_block->next = curr->next;

                curr->size = size;
                curr->next = next_block;
            }
            curr->is_free = 0;
            return (void *)((uint8_t *)curr + HEADER_SIZE);
        }
        curr = curr->next;
    }
    return (void *)0; // out of memory
}

void kfree(void *ptr) {
    if (!ptr) return;

    block_header_t *header = (block_header_t *)((uint8_t *)ptr - HEADER_SIZE);
    if (header->magic != BLOCK_MAGIC) return;

    header->is_free = 1;

    // Coalesce adjacent free blocks
    block_header_t *curr = heap_first;
    while (curr) {
        if (curr->magic != BLOCK_MAGIC) break;

        while (curr->is_free && curr->next && curr->next->is_free) {
            curr->size += HEADER_SIZE + curr->next->size;
            curr->next = curr->next->next;
        }
        curr = curr->next;
    }
}

void *kalloc_stack(size_t size) {
    void *stack = kmalloc(size);
    if (!stack) return (void *)0;
    // Stack grows downwards, return top of stack (8-byte aligned)
    uint32_t top = (uint32_t)stack + size;
    return (void *)(top & ~7u);
}

uint32_t kmem_used(void) {
    uint32_t used = 0;
    block_header_t *curr = heap_first;
    while (curr && curr->magic == BLOCK_MAGIC) {
        if (!curr->is_free) used += curr->size + HEADER_SIZE;
        else used += HEADER_SIZE;
        curr = curr->next;
    }
    return used;
}

uint32_t kmem_available(void) {
    uint32_t free_bytes = 0;
    block_header_t *curr = heap_first;
    while (curr && curr->magic == BLOCK_MAGIC) {
        if (curr->is_free) free_bytes += curr->size;
        curr = curr->next;
    }
    return free_bytes;
}

uint32_t kmem_total(void) {
    return heap_total_size;
}
