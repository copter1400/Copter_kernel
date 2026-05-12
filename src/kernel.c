#include "config.h"
#include "terminal.h"
#include "string.h"
#include "keyboard.h"
#include "type.h"
#include "pic.h"
#include "test.h"
#include "idt.h"
#include "shell.h"

void kernel_main() {
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
        "You can now use shell under here\n",
        "type 'help' for available command!"
    };

    for (int i = 0; i < 3; i++) {
        print_color(msg[i], 0x0F);
    }

    shell_task();
}