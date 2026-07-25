#ifndef TIME_H
#define TIME_H

#include "type.h"

void time_update();
void time_reset();
void time_set(uint8_t second_p, uint8_t minute_p, uint8_t hour_p, uint8_t day_p, uint8_t month_p, uint8_t year_p, uint8_t timezone_p);
uint8_t get_time(uint8_t id);
void sleep(uint32_t seconds);

#endif
