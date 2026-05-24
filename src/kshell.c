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

void shell_task() {
    config_load();

    char* buffer = kmalloc(kernel_config.kshell_buffer);
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
            shell_exec(buffer);
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

void shell_exec(char* buffer) {
    int argc = 0;
    int i = 0;
    int in_word = 0;

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

    // argv part
    char **argv = kmalloc(sizeof(char*) * (argc + 1));
    int a = 0;
    for (int i = 0; i < kernel_config.kshell_buffer; i++) {

        if (buffer[i] != '\0' &&
            (i == 0 || buffer[i - 1] == '\0')) {

            argv[a++] = &buffer[i];
        }
    }
    argv[a] = 0;

    if (strcmp(argv[0], "") == 0 || argc == 0) {
        
    }

    // test command
    else if (strcmp(argv[0], "dvz") == 0) {
        asm volatile("int $0");
    }

    // real command
    else if   (strcmp(argv[0], "help") == 0) {
        print("Commands: help hello tick clear echo dump-ram dvz panic\n");
    } else if (strcmp(argv[0], "hello") == 0) {
        print("Hello, World!\n");
    } else if (strcmp(argv[0], "tick") == 0) {
        print_int(timer_ticks);
        print("\n");
    } else if (strcmp(argv[0], "clear") == 0) {
        clear_screen();
    } else if (strcmp(argv[0], "echo") == 0) {
        for (int i = 1; i < argc; i++) {
            print(argv[i]);
            print(" ");
        }
        print("\n");
    } else if (strcmp(argv[0], "dump-ram") == 0) {
        kernel_get_memory_info();
        kernel_get_memory_region(0x100000 + atoi(argv[1]), 10);
    } else if (strcmp(argv[0], "panic") == 0) {
        panic(argv[1]);
    }

    else {
        print("Unknown command [");
        print(argv[0]);
        print("]");
        print("\n");
    }

    // free argv
    kfree(argv);
}