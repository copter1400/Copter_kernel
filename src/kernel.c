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
#include "time.h"
#include "disk.h"

multiboot_info_t* g_mb;
void kernel_get_memory_info() {
    print_memory_map(g_mb);
    dump_memory_map(g_mb);
}

void kernel_get_memory_region(uint32_t base, uint32_t len) {
    dump_region(base, len);
}

// Test function
void A(uint32_t pid) {
    while (true) {
        print("time (H:M:S | D:M:Y (Timezone) : ");
        print_int(get_time(2));
        print(":");
        print_int(get_time(1));
        print(":");
        print_int(get_time(0));
        print(" | ");
        print_int(get_time(3));
        print(":");
        print_int(get_time(4));
        print(":");
        print_int(get_time(5));
        print(" (");
        print_int(get_time(6));
        print(")\n");
        sleep(1);
    }  
}

void B(uint32_t pid) {
    while (true) {
        print("B : this work WOW; current tick : ");
        print_int(get_tick());
        print("\n");
        sleep(2);
    }  
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
    pit_init(kernel_config.timer_hz);

    asm volatile("sti");

    uint16_t identify[256];
    int result;
    ata_identify(identify, result);

    if (result == 0) {
        for (size_t i=0;i<256;i++) {
            serial_print_hex16(identify[i]);
            serial_print(" ");
        }
    } else if (result == 1) {
        warn("No ATA device present", 0);
    } else if (result == 2) {
        warn("disk command", 0);
    }
    
    print("\n");
    

    const char* msg[2] = {
        "Ckernel\n",
        "type 'help' for available command\n"
    };

    for (int i = 0; i < 2; i++) {
        print_color(msg[i], 0x0F);
    }

    uint32_t cs;
    asm volatile("mov %%cs, %0" : "=r"(cs));
    serial_print("cs: ");
    serial_print_hex(cs);
    serial_print("\n");

    //create_task((uint32_t)A, "A task_A");
    //create_task((uint32_t)B, "B task_B");
    create_task((uint32_t)kshell_task, "kshell");

    while(1) { asm volatile("hlt"); }
}
