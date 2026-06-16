#include "heap.h"
#include "type.h"
#include "panic.h"
#include "config.h"

#define HEAP_START 0x01000000
#define HEAP_SIZE  0x00100000
#define HEAP_END   (HEAP_START + HEAP_SIZE)

#define MAX_ALLOC (HEAP_SIZE / 4)

#define MAGIC 0xC0FEBABE
#define GUARD 0xDEADBEEF

typedef struct block {
    uint32_t magic;
    uint32_t size;
    uint8_t free;
    struct block* next;
} block_t;

static block_t* heap_start;

static uint32_t* block_guard(block_t* block)
{
    return (uint32_t*)((char*)block +
                       sizeof(block_t) +
                       block->size);
}

void heap_check() {
    block_t* curr = heap_start;

    while (curr) {

        if (curr->magic != MAGIC)
            panic("heap corrupt (magic)");

        if ((uint32_t)curr < HEAP_START ||
            (uint32_t)curr >= HEAP_END)
            panic("heap corrupt (bounds)");

        curr = curr->next;
    }
}

void heap_init(void) {
    heap_start = (block_t*)HEAP_START;

    heap_start->magic = MAGIC;
    heap_start->free  = 1;
    heap_start->next  = 0;

    heap_start->size =
        HEAP_SIZE -
        sizeof(block_t) -
        sizeof(uint32_t);

    *block_guard(heap_start) = GUARD;

    config_load();
}

void* kmalloc(uint32_t size) {
    if (size == 0 || size > HEAP_SIZE / 4) {
        panic("invalid kmalloc size");
    }

    if (size > MAX_ALLOC) {
        return 0;
    }

    size = (size + 7) & ~7;

    block_t* curr = heap_start;

    while (curr) {

        if (curr->magic != MAGIC)
            panic("heap corrupt");

        if (curr->free &&
            curr->size >= size)
        {
            uint32_t remaining =
                curr->size - size;

            if (remaining >
                sizeof(block_t) +
                sizeof(uint32_t) +
                8)
            {
                block_t* new_block = (block_t*)((char*)curr +
                                     sizeof(block_t) +
                                     size +
                                     sizeof(uint32_t));

                new_block->magic = MAGIC;
                new_block->free  = 1;
                new_block->next  = curr->next;

                new_block->size = remaining - sizeof(block_t) - sizeof(uint32_t);

                *block_guard(new_block) = GUARD;

                curr->next = new_block;
                curr->size = size;
            }

            curr->free = 0;

            *block_guard(curr) = GUARD;

            return (char*)curr + sizeof(block_t);
        }

        curr = curr->next;
    }

    return 0;
}

void kfree(void* ptr)
{
    if (!ptr)
        return;

    // pointer range check
    if ((uint32_t)ptr < HEAP_START || (uint32_t)ptr >= HEAP_END) {
        panic("invalid free");
    }

    // recover block header
    block_t* block = (block_t*)((char*)ptr - sizeof(block_t));

    // validate block integrity
    if (block->magic != MAGIC) {
        panic("heap corrupt (magic)");
    }

    // double free detection
    if (block->free) {
        panic("double free");
    }

    block->free = 1;
}