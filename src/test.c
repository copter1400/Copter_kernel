#include "test.h"
#include "terminal.h"
#include "test.h"
#include "terminal.h"


typedef int (*function)();

void test(const char* name, function fn) {
    int result = fn();

    if (result) {
        print("[ OK ]");
    } else {
        print("[FAIL]");
    }

    print(" : ");
    print(name);
    print("\n");
}