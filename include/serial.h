#ifndef SERIAL_H
#define SErIAL_H

#include "type.h"

void serial_init();
void serial_putc(char c);
void serial_print(const char* s);
void serial_print_int(uint32_t value);
void serial_print_hex(uint32_t value);

#endif