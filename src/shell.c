#include "keyboard.h"
#include "type.h"
#include "terminal.h"
#include "string.h"

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
            print("\n");
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

    if (strcmp(buffer, "help") == 0) {
        print("Commands: help clear echo");
    } else if (strcmp(buffer, "clear") == 0) {
        clear_screen();
    } else if (strcmp(buffer, "echo") == 0) {
        print(arg);
    }

    else {
        print("Unknown command");
    }
}