#include "kshell.h"
#include "keyboard.h"
#include "type.h"
#include "terminal.h"
#include "string.h"
#include "timer.h"
#include "panic.h"
#include "multiboot.h"
#include "heap.h"
#include "kernel.h"
#include "config.h"
#include "test.h"
#include "task.h"

void kshell_task(int pid) {
    config_load();

    // char* buffer = kmalloc(kernel_config.kshell_buffer);
    char* buffer = kmalloc(256);
    uint8_t pos = 0;

    print(">");

    while (1) {

        char c = keyboard_pop();

        if (!c)
            continue;

        // arrows
        if (c == KEY_LEFT) {
            print("[LEFT]");
            continue;
        }

        if (c == KEY_RIGHT) {
            print("[RIGHT]");
            continue;
        }

        if (c == '\n') {

            buffer[pos] = 0;

            print("\n");
            kshell_exec(buffer, pid);
            print(">");

            pos = 0;
            continue;
        }

        if (c == '\b' && pos > 0) {
            terminal_backspace();
            pos--;
            continue;
        }

        if (pos < kernel_config.kshell_buffer - 1) {
            buffer[pos] = c;

            char str[2] = {c, 0};
            print(str);

            pos++;
        }
    }
}

void kshell_exec(char* buffer, int pid) {
    int argc = 0;
    int i = 0;
    int in_word = 0;

    // check heap
    heap_check();

    // argc part
    while (buffer[i] != '\0') {
        if (buffer[i] != ' ' && !in_word) {
            in_word = 1;
            argc++;
        }

        if (buffer[i] == ' ') {
            buffer[i] = '\0';
            in_word = 0;
        }

        i++;
    }

    char **argv = kmalloc(sizeof(char*) * (argc + 1));
    int a = 0;
    for (int i = 0; i < kernel_config.kshell_buffer && a < argc; i++) {
        if (buffer[i] != '\0' &&
            (i == 0 || buffer[i - 1] == '\0')) {
            argv[a++] = &buffer[i];
        }
    }
    
    argv[a] = 0;

    if (argc == 0 || argv[0] == 0) {
        return;
    }

    // test command
    else if (strcmp(argv[0], "dvz") == 0) {
        asm volatile("int $0");
    }

    // real command
    else if   (strcmp(argv[0], "help") == 0) {
        print("Commands: help hello tick clear echo dump-ram dvz vga-test set panic tls tkill\n");
    } else if (strcmp(argv[0], "hello") == 0) {
        print("Hello, World!\n");
    } else if (strcmp(argv[0], "tick") == 0) {
        print_int(timer_ticks);
        print("\n");
    } else if (strcmp(argv[0], "clear") == 0) {
        clear_screen();
    } else if (strcmp(argv[0], "set") == 0) {
        // SET FUNCTION
        if (strcmp(argv[1], "debug") == 0) {
            kernel_config.debug = 1;
            print("\ndebug has been set to ");
            print_int(kernel_config.debug);
            print("\n");
            warn("Aware that debug flag can corrupt your memory!\n",2);
        }
        // SET FUNCTION
    } else if (strcmp(argv[0], "echo") == 0) {
        for (int i = 1; i < argc; i++) {
            print(argv[i]);
            print(" ");
        }
        print("\n");
    } else if (strcmp(argv[0], "dump-ram") == 0) {
        kernel_get_memory_info();
        kernel_get_memory_region(0x100000 + atoi(argv[1]), 10);
    } else if (strcmp(argv[0], "vga-test") == 0) {
        run_test("VGA test", vga_test);
    } else if (strcmp(argv[0], "panic") == 0) {
        panic(argv[1]);
    } else if (strcmp(argv[0], "tls") == 0) {
        tls();
    } else if (strcmp(argv[0], "tkill") == 0) {
        kill_task(atoi(argv[1]));
    }

    else {
        print("Unknown command [");
        print(argv[0]);
        print("]");
        print("\n");
    }

    // free argv
    kfree(argv);

    // check heap
    heap_check();
}