#include "heap.h"
#include "type.h"

#define HEAP_START 0x01000000
#define HEAP_SIZE  0x00100000

static uint32_t heap_ptr = HEAP_START;
static uint32_t heap_end = HEAP_START + HEAP_SIZE;

void* kmalloc(uint32_t size) {

    // align to 4 bytes
    if (size % 4)
        size += 4 - (size % 4);

    if (heap_ptr + size >= heap_end) {
        return 0;
    }

    void* addr = (void*)heap_ptr;

    heap_ptr += size;

    return addr;
}