#ifndef KERNEL_H
#define KERNEL_H

#include "type.h"

void kernel_get_memory_info();
void kernel_get_memory_region(uint32_t base, uint32_t len);

#endif