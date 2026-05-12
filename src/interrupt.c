#include "interrupt.h"
#include "terminal.h"

void divide_by_zero_handler() {
    print_color("DIVIDE BY ZERO ERROR\n", 0x04);
}