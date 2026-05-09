#include "io.h"

void pic_init() {
    outb(0x21, 0xFD); // enable IRQ1 only
    outb(0xA1, 0xFF); // disable slave PIC
}