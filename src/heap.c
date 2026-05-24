#include "heap.h"
#include "type.h"
#include "panic.h"

#define HEAP_START 0x01000000
#define HEAP_SIZE  0x00100000
#define HEAP_END   (HEAP_START + HEAP_SIZE)
#define MAX_ALLOC  (HEAP_SIZE / 4)

#define MAGIC 0xC0FEBABE
#define GUARD 0xDEADBEEF

typedef struct block {
    uint32_t magic;
    uint32_t size;
    uint8_t  free;
    uint32_t guard;
    struct block* next;
} block_t;

static block_t* heap_start;
static block_t* heap_end;

void heap_init() {
    heap_start = (block_t*)HEAP_START;
    heap_end   = (block_t*)((char*)HEAP_START + HEAP_SIZE);

    heap_start->magic = MAGIC;
    heap_start->size  = HEAP_SIZE - sizeof(block_t);
    heap_start->free  = 1;
    heap_start->guard = 0xDEADBEEF;
    heap_start->next  = NULL;
}

void* kmalloc(uint32_t size) {
    if (size == 0 || size > MAX_ALLOC){
        return 0;
    }
    
    size = (size + 7) & ~7;

    block_t* curr = heap_start;

    while (curr) {

        if (curr->free && curr->size >= size + sizeof(uint32_t)) {

            // split
            if (curr->size > size + sizeof(block_t) + sizeof(uint32_t)) {

                block_t* new_block =
                    (block_t*)((char*)curr + sizeof(block_t) + size + sizeof(uint32_t));

                new_block->magic = MAGIC;
                new_block->size  = curr->size - size - sizeof(block_t) - sizeof(uint32_t);
                new_block->free  = 1;
                new_block->next  = curr->next;

                curr->next = new_block;
                curr->size = size;
            }

            curr->free = 0;

            uint32_t* user = (uint32_t*)((char*)curr + sizeof(block_t));

            *user = GUARD;   // place guard after user memory

            return (char*)user + sizeof(uint32_t);
        }

        curr = curr->next;
    }

    return 0;
}

void kfree(void* ptr) {

    if (!ptr)
        return;

    uint32_t* guard_ptr = (uint32_t*)((char*)ptr - sizeof(uint32_t));
    block_t* block = (block_t*)((char*)guard_ptr - sizeof(block_t));

    if (ptr < HEAP_START || ptr >= HEAP_END) {
        panic("invalid free");    
    }
    
    if (block->magic != MAGIC) {
        panic("Heap corrupt (magic)");
    }

    if (*guard_ptr != GUARD) {
        panic("BUFFER OVERFLOW DETECTED");
    }

    block->free = 1;
}