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
#include "task.h"
#include "panic.h"
#include "serial.h"
#include "timer.h"

multiboot_info_t* g_mb;
void kernel_get_memory_info() {
    print_memory_map(g_mb);
    dump_memory_map(g_mb);
}

void kernel_get_memory_region(uint32_t base, uint32_t len) {
    dump_region(base, len);
}

void A(uint32_t pid) {
    serial_print("A: my pid is ");
    serial_print_int(pid);
    serial_print("\n");
    while(1) {}
}

void kernel_main(uint32_t mb_magic, uint32_t mb_addr) {
    config_load();
    heap_init();

    terminal_init();
    run_test("VGA test", vga_test);

    serial_init();

    multiboot_info_t* mb = (multiboot_info_t*)mb_addr;
    print_memory_map(mb);
    g_mb = (multiboot_info_t*)mb_addr;

    asm volatile("cli");

    pic_init();
    init_idt();
    keyboard_init();
    pit_init(100);

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

    uint32_t cs;
    asm volatile("mov %%cs, %0" : "=r"(cs));
    serial_print("cs: ");
    serial_print_hex(cs);
    serial_print("\n");

    create_task((uint32_t)A, "A task_A");
    create_task((uint32_t)kshell_task, "kshell");

    while(1) { asm volatile("hlt"); }
}
