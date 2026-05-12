#ifndef TIMER_H
#define TIMER_H

#include "type.h"

extern volatile uint32_t timer_ticks;

void pit_init(uint32_t hz);
void timer_handler();

#endif