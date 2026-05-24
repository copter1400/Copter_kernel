#ifndef HEAP_H
#define HEAP_H

#include "type.h"


void heap_init();
void* kmalloc(uint32_t size);
void kfree(void* ptr);

#endif