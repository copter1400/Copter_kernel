#include "multiboot.h"
#include "terminal.h"
#include "test.h"

void print_memory_map(multiboot_info_t* mb) {
    if (!(mb->flags & (1 << 6))) {
        warn("Memory map", 0);
        return;
    } else {
        warn("Memory map", 1);
    }

    print("ADDR=");
    print_int(mb->mmap_addr);
    print("\n");

    print("LEN=");
    print_int(mb->mmap_length);
    print("\n");

    uint32_t addr = mb->mmap_addr;
    uint32_t end  = mb->mmap_addr + mb->mmap_length;
    mmap_entry_t* entry = (mmap_entry_t*)addr;

    int usable = 0;
    int total_usable_bytes = 0;

    while ((uint32_t)entry < end) {

        if (entry->type == 1) {
            usable++;
            total_usable_bytes += entry->length;
        }

        entry = (mmap_entry_t*)((uint32_t)entry + entry->size + sizeof(entry->size));
    }

    print("Usable regions: ");
    print_int(usable);
    print("\n");
    print("Usable ram (BYTE): ");
    print_int(total_usable_bytes);
    print("\n");
}

void dump_memory_map(multiboot_info_t* mb) {

    uint32_t addr = mb->mmap_addr;
    uint32_t end  = mb->mmap_addr + mb->mmap_length;

    print("Memory Map:\n");

    while (addr < end) {

        mmap_entry_t* e = (mmap_entry_t*)addr;

        print_hex((uint32_t)e->base_addr);
        print(" - ");
        print_hex((uint32_t)(e->base_addr + e->length));
        print(" type=");
        print_int(e->type);
        print("\n");

        addr += e->size + sizeof(uint32_t);
    }
}

void dump_region(uint32_t base, uint32_t length) {

    uint8_t* ptr = (uint8_t*)base;

    print("dump ram\n");

    for (uint32_t i = 0; i < length; i++) {
        print_hex(base + i);
        print(": ");

        print_hex(ptr[i]);
        print("\n");
    }
}