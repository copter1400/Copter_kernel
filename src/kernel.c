#include "config.h"
#include "terminal.h"
#include "string.h"
#include "keyboard.h"
#include "idt.h"
#include "type.h"
#include "pic.h"
#include "test.h"

void kernel_main() {
    config_load();
    terminal_init();
    
    test("VGA test", vga_test);

    char* msg[3] = {
        "Copter kernel\n",
        "If you can read this, That mean this kernel can boot!\n",
        "Test keyboard input here :"
    };

    for (int i=0;i<3;i++) {
        print(msg[i]);
    }

    pic_init();
    init_idt();

    asm volatile("sti");

    while (1) {
        char c = keyboard_pop();

        if (c) {
            char str[2] = {c, 0};
            print(str);
        }
    }
}