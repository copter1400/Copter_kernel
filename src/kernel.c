#include "config.h"
#include "terminal.h"
#include "string.h"
#include "keyboard.h"
#include "type.h"
#include "pic.h"
#include "test.h"
#include "idt.h"
#include "kshell.h"
#include "multiboot.h"
#include "heap.h"

multiboot_info_t* g_mb;
void kernel_get_memory_info() {
    print_memory_map(g_mb);
    dump_memory_map(g_mb);
}

void kernel_get_memory_region(uint32_t base, uint32_t len) {
    dump_region(base, len);
}

void kernel_main(uint32_t mb_magic, uint32_t mb_addr) {
    config_load();

    terminal_init();
    run_test("VGA test", vga_test);

    multiboot_info_t* mb = (multiboot_info_t*)mb_addr;
    print_memory_map(mb);
    g_mb = (multiboot_info_t*)mb_addr;

    asm volatile("cli");

    pic_init();
    init_idt();

    asm volatile("sti");

    const char* msg[4] = {
        "Copter kernel\n",
        "If you can read this, That means this kernel can boot!\n",
        "You can now use shell under here\n",
        "type 'help' for available command!"
    };

    for (int i = 0; i < 3; i++) {
        print_color(msg[i], 0x0F);
    }

    char* test = kmalloc(64);

    test[0] = 'H';
    test[1] = 'e';
    test[2] = 'a';
    test[3] = 'p';
    test[4] = ' ';
    test[5] = 't';
    test[6] = 'e';
    test[7] = 's';
    test[8] = 't';
    test[9] = '\n';
    test[10] = 0;

    print(test);

    shell_task();
}
