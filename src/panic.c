#include "terminal.h"

void panic(const char* msg) {
    asm volatile("cli");  // disable interrupts

    clear_screen();
    print_color("KERNEL PANIC: ", 0x4F);
    print_color(msg, 0x4F);
    print_color("\n", 0x4F);

    while (1) {
        asm volatile("hlt");
    }
}

void panic_handler() {
    asm volatile("cli");  // disable interrupts

    char* msg = "ISR STUB";

    clear_screen();
    print_color("KERNEL PANIC: ", 0x4F);
    print_color(msg, 0x4F);
    print_color("\n", 0x4F);

    while (1) {
        asm volatile("hlt");
    }
}