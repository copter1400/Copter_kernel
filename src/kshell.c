#include "kshell.h"
#include "keyboard.h"
#include "type.h"
#include "terminal.h"
#include "string.h"
#include "timer.h"
#include "panic.h"
#include "multiboot.h"

void shell_task() {
    char buffer[256];
    uint8_t pos = 0;

    print(">");
    while (1) {

        char c = keyboard_pop();

        if (!c){
            continue;
        }

        if (c == '\n') {

            buffer[pos] = 0;

            print("\n");
            shell_exec(buffer);
            print(">");

            pos = 0;
        } else if (c == '\b' && pos > 0) {
            terminal_backspace();
            pos--;
        } else if (pos < 255) {
            buffer[pos] = c;
            char str[2] = {c,0};
            print(str);
            pos++;
        }
    }
}

void shell_exec(char* buffer) {

    char* arg;
    int i = 0;

    while (buffer[i]) {
        if (buffer[i] == ' ') {
            buffer[i] = 0;
            arg = &buffer[i + 1];
            
            break;
        }

        i++;
    }

    if (strcmp(buffer, "") == 0) {
        
    }

    // test command
    else if (strcmp(buffer, "dvz") == 0) {
        asm volatile("int $0");
    }

    // real command
    else if   (strcmp(buffer, "help") == 0) {
        print("Commands: help hello tick clear echo dump-ram dvz panic\n");
    } else if (strcmp(buffer, "hello") == 0) {
        print("Hello, World!\n");
    } else if (strcmp(buffer, "tick") == 0) {
        print_int(timer_ticks);
        print("\n");
    } else if (strcmp(buffer, "clear") == 0) {
        clear_screen();
    } else if (strcmp(buffer, "echo") == 0) {
        print(arg);
        print("\n");
    } else if (strcmp(buffer, "dump-ram") == 0) {
        kernel_get_memory_info();
        kernel_get_memory_region(0x100000 + atoi(arg), 10);
    } else if (strcmp(buffer, "panic") == 0) {
        panic(arg);
    }

    else {
        print("Unknown command\n");
    }
}