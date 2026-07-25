#include "timer.h"
#include "io.h"
#include "task.h"
#include "serial.h"
#include "config.h"
#include "time.h"

volatile uint32_t timer_ticks = 0;
uint32_t hz;

void pit_init(uint32_t hz_p) {
    config_load();

    hz = hz_p;

    uint32_t divisor = 1193182 / hz_p;

    outb(0x43, 0x36);

    outb(0x40, divisor & 0xFF);
    outb(0x40, (divisor >> 8) & 0xFF);
}

uint32_t get_tick() {
    return timer_ticks;
}

uint32_t get_hz() {
    return hz;
}

void timer_handler() {
    timer_ticks++;

    if (timer_ticks % kernel_config.timer_hz == 0) {
        time_update();
    }

    // ACK
    outb(0x20, 0x20);
}