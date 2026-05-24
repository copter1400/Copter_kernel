#include "heap.h"
#include "type.h"

#define HEAP_START 0x01000000
#define HEAP_SIZE  0x00100000

typedef struct block {
    uint32_t size;
    uint8_t free;
    struct block* next;
} block_t;

static block_t* heap_start;

void heap_init() {
    heap_start = (block_t*)HEAP_START;

    heap_start->size = HEAP_SIZE - sizeof(block_t);
    heap_start->free = 1;
    heap_start->next = NULL;
}

void* kmalloc(uint32_t size) {

    if (size % 4)
        size += 4 - (size % 4);

    block_t* curr = heap_start;

    while (curr) {

        if (curr->free && curr->size >= size) {

            // split block
            if (curr->size > size + sizeof(block_t)) {

                block_t* new_block =
                    (block_t*)((char*)curr + sizeof(block_t) + size);

                new_block->size = curr->size - size - sizeof(block_t);
                new_block->free = 1;
                new_block->next = curr->next;

                curr->next = new_block;
                curr->size = size;
            }

            curr->free = 0;

            return (char*)curr + sizeof(block_t);
        }

        curr = curr->next;
    }

    return 0;
}

void kfree(void* ptr) {

    if (!ptr)
        return;

    block_t* block = (block_t*)((char*)ptr - sizeof(block_t));

    block->free = 1;

    // merge next block (basic coalescing)
    if (block->next && block->next->free) {
        block->size += sizeof(block_t) + block->next->size;
        block->next = block->next->next;
    }
}