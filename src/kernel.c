#include "config.h"
#include "terminal.h"
#include "string.h"
#include "keyboard.h"
#include "type.h"
#include "pic.h"
#include "test.h"
#include "idt.h"

void kernel_main() {
    volatile uint16_t* vga = (volatile uint16_t*)0xB8000;
    vga[0] = (0x07 << 8) | 'H';
    vga[1] = (0x07 << 8) | 'i';

    config_load();
    terminal_init();

    asm volatile("cli");

    pic_init();
    init_idt();

    asm volatile("sti");

    run_test("VGA test", vga_test);

    const char* msg[3] = {
        "Copter kernel\n",
        "If you can read this, That means this kernel can boot!\n",
        "Test keyboard input here :"
    };

    for (int i = 0; i < 3; i++) {
        print(msg[i]);
    }

    while (1) {
        char c = keyboard_pop();

        if (c) {
            char str[2] = {c, 0};
            print(str);
        }
    }
}