#ifndef TERMINAL_H
#define TERMINAL_H

#include "type.h"

typedef struct {
    int row;
    int col;
} terminal_t;

extern terminal_t terminal;

void move_cursor();
void terminal_init();
int vga_test();
void print(const char* text);
void print_color(const char* text, uint8_t color);
void scroll();
void clear_screen();
void terminal_backspace();

#endif