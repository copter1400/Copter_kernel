#include "terminal.h"
#include "config.h"

void panic(const char* msg) {
    config_load();

    asm volatile("cli");  // disable interrupts

    if (!kernel_config.debug) {
        clear_screen();
        print_color("KERNEL PANIC: ", 0x4F);
        print_color(msg, 0x4F);
        print_color("\n", 0x4F);
    }

    while (1) {
        asm volatile("hlt");
    }
}

void panic_handler(uint32_t num) {
    config_load();

    asm volatile("cli");  // disable interrupts

    if (!kernel_config.debug) {
        char* msg = "ISR STUB";

    // clear_screen();
    // print_color("KERNEL PANIC: ", 0x4F);
    // print_color(msg, 0x4F);
    // print_color("\n", 0x4F);

        print("Exeption : ");
        print_int(num);
    }

    while (1) {
        asm volatile("hlt");
    }
}

void gpf_handler(uint32_t num, uint32_t err) {
    config_load();
    
    if (!kernel_config.debug) {
        asm volatile("cli");
        print("GPF exception: ");
        print_int(num);
        print(" error code: ");
        print_hex(err);
        print("\n");
    }

    while(1) asm volatile("hlt");
}