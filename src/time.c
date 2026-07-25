#include "time.h"
#include "timer.h"
#include "type.h"

const int month_days[12] = {
    31, // jan
    28, // feb [use 28 as temp]
    31, // mar
    30, // april
    31, // may
    30, // june
    31, // july
    31, // aug
    30, // sep
    31, // oct
    30, // nov
    31  // dec
};

uint8_t second   = 0;
uint8_t minute   = 0;
uint8_t hour     = 0;
uint8_t day      = 0;
uint8_t month    = 0;
uint8_t year     = 0;
uint8_t timezone = 0;

void time_update() {
    second++;

    if (second >= 60) {
        minute++;
        second = 0;
    }
    if (minute >= 60) {
        hour++;
        minute = 0;
    }
    if (hour >= 24) {
        day++;
        hour = 0;
    }
    if (day >= month_days[month]) {
        month++;
        day = 0;
    }
    if (month >= 12) {
        year++;
        month = 0;
    }
}

void time_reset() {
    second   = 0;
    minute   = 0;
    hour     = 0;
    day      = 0;
    month    = 0;
    year     = 0;
    timezone = 0;
}

void time_set(uint8_t second_p, uint8_t minute_p, uint8_t hour_p, uint8_t day_p, uint8_t month_p, uint8_t year_p, uint8_t timezone_p) {
    second   = second_p;
    minute   = minute_p;
    hour     = hour_p;
    day      = day_p;
    month    = month_p;
    year     = year_p;
    timezone = timezone_p;
}

uint8_t get_time(uint8_t id) {
    switch (id) {
        case 0:
            return second;
            break;
        case 1:
            return minute;
            break;
        case 2:
            return hour;
            break;
        case 3:
            return day;
            break;
        case 4:
            return month;
            break;
        case 5:
            return year;
            break;
        case 6:
            return timezone;
            break;    
    }
}

void sleep(uint32_t seconds) {
    uint32_t start  = get_tick();
    uint32_t target = start + (seconds * get_hz());
    
    // The magic -1 from the target idk why it work
    while (get_tick() <= target - 1) {}
}