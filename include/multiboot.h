#ifndef MULTIBOOT_H
#define MULTIBOOT_H

#include "type.h"

// memory map entry from GRUB
typedef struct mmap_entry {

    uint32_t size;
    uint64_t base_addr;
    uint64_t length;
    uint32_t type;

} __attribute__((packed)) mmap_entry_t;

// multiboot info struct
typedef struct multiboot_info {
    uint32_t flags;
    uint32_t mem_lower;
    uint32_t mem_upper;
    uint32_t boot_device;
    uint32_t cmdline;
    uint32_t mods_count;
    uint32_t mods_addr;
    uint32_t syms[4];

    uint32_t mmap_length;
    uint32_t mmap_addr;
} multiboot_info_t;

void print_memory_map(multiboot_info_t* mb);
void dump_memory_map(multiboot_info_t* mb);
void dump_region(uint32_t base, uint32_t length);

#endif