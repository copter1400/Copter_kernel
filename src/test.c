#include "test.h"
#include "terminal.h"
#include "type.h"
#include "config.h"

typedef int (*function)();

void run_test(const char* name, function fn) {
    config_load();
    
    int result = fn();

    if (result == 1) {
        print("[ ");
        print_color("OK",0x02);
        print(" ]");
    } else if (result == 0) {
        print("[");
        print_color("FAIL",0x04);
        print("]");
    } else if (result == 2) {
        print("[");
        print_color("WARN",0x0E);
        print("]");
    }

    print(" : ");
    print(name);
    print("\n");
}

void warn(const char* name, int is_ok) {
    if (is_ok == 1) {
        print("[ ");
        print_color("OK",kernel_config.vga_color_ok);
        print(" ]");
    } else if (is_ok == 0) {
        print("[");
        print_color("FAIL",kernel_config.vga_color_fail);
        print("]");
    } else if (is_ok == 2) {
        print("[");
        print_color("WARN",kernel_config.vga_color_warning);
        print("]");
    }

    print(" : ");
    print(name);
    print("\n");
}