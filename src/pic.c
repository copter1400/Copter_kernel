#include "io.h"

void pic_init() {
    outb(0x21, 0xFD);
    outb(0xA1, 0xFF);
}