#include "terminal.h"

void clear_screen() {
    char* vga = (char*)0xB8000;

    // 80 columns 25 rows
    // 1 char 1 color per char
    // aka [char 1][color 1] [char 2][color 2]

    // set all to ' ' with light-grey text black background
    for (int i=0;i< 80 * 25 * 2;i+=2) {
        vga[i] = ' ';
        vga[i+1] = 0x07;
    }
}

void scroll() {
    char* vga = (char*)0xB8000;

    for (int row = 1; row < 25; row++) {
        for (int col = 0; col < 80; col++) {

            int src = (row * 80 + col) * 2;
            int dst = ((row - 1) * 80 + col) * 2;

            vga[dst] = vga[src];
            vga[dst + 1] = vga[src + 1];
        }
    }

    // clear last row
    for (int col = 0; col < 80; col++) {
        int i = (24 * 80 + col) * 2;
        vga[i] = ' ';
        vga[i + 1] = 0x07;
    }
}

void print(const char* text) {
    static int row = 0;
    static int col = 0;

    char* vga = (char*)0xB8000;

    int s = 0;

    while (text[s] != 0) {

        char c = text[s];

        // Newline
        if (c == '\n') {
            row++;
            col = 0;
        }
        else {
            int v = (row * 80 + col) * 2;

            vga[v] = c;
            vga[v + 1] = 0x07;

            col++;
        }

        // if col hit max will reset column and go to next row
        if (col >= 80) {
            col = 0;
            row++;
        }

        // if row hit max
        if (row >= 25) {
            scroll();
            row = 24;
        }

        s++;
    }
}