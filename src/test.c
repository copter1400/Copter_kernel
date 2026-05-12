#include "test.h"
#include "terminal.h"
#include "type.h"

typedef int (*function)();

void run_test(const char* name, function fn) {
    int result = fn();

    if (result) {
        print("[ ");
        print_color("OK",0x02);
        print(" ]");
    } else {
        print("[");
        print_color("FAIL",0x04);
        print("]");
    }

    print(" : ");
    print(name);
    print("\n");
}