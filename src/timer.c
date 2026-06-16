#include "timer.h"
#include "io.h"
#include "task.h"
#include "serial.h"

volatile uint32_t timer_ticks = 0;

void pit_init(uint32_t hz) {

    uint32_t divisor = 1193182 / hz;

    outb(0x43, 0x36);

    outb(0x40, divisor & 0xFF);
    outb(0x40, (divisor >> 8) & 0xFF);
}

void timer_handler() {
    timer_ticks++;
    outb(0x20, 0x20);
}