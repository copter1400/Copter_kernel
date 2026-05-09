#ifndef TERMINAL_H
#define TERMINAL_H

typedef struct {
    int row;
    int col;
} terminal_t;

extern terminal_t terminal;

void terminal_init();
int vga_test();
void print(const char* text);
void scroll();
void clear_screen();

#endif