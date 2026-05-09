#include "config.h"
#include "terminal.h"
#include "type.h"

terminal_t terminal;

// init terminal state
void terminal_init() {
    terminal.row = 0;
    terminal.col = 0;
}

// VGA test
int vga_test() {
    uint16_t* vga = (uint16_t*)0xB8000;

    int size = kernel_config.vga_width * kernel_config.vga_height;

    // write pattern
    for (int i = 0; i < size; i++) {
        uint8_t ch = (i & 0xFF);              // pattern char
        uint8_t color = (i * 3) & 0x0F;       // pattern color (0-15)

        vga[i] = (color << 8) | ch;
    }

    // read back and verify
    for (int i = 0; i < size; i++) {
        uint8_t expected_ch = (i & 0xFF);
        uint8_t expected_color = (i * 3) & 0x0F;

        uint16_t val = vga[i];

        uint8_t ch = val & 0xFF;
        uint8_t color = (val >> 8) & 0x0F;

        if (ch != expected_ch || color != expected_color) {
            return 0; // FAIL
        }
    }

    clear_screen();

    return 1; // OK
}

// clear entire screen
void clear_screen() {
    uint16_t* vga = (uint16_t*)0xB8000;

    int size = kernel_config.vga_width * kernel_config.vga_height;

    for (int i = 0; i < size; i++) {
        vga[i] = (0x07 << 8) | ' ';
    }

    // reset cursor here (IMPORTANT FIX)
    terminal.row = 0;
    terminal.col = 0;
}

// scroll screen up by 1 row
void scroll() {
    uint16_t* vga = (uint16_t*)0xB8000;

    int w = kernel_config.vga_width;
    int h = kernel_config.vga_height;

    // move rows up
    for (int row = 1; row < h; row++) {
        for (int col = 0; col < w; col++) {

            int src = row * w + col;
            int dst = (row - 1) * w + col;

            vga[dst] = vga[src];
        }
    }

    // clear last row
    for (int col = 0; col < w; col++) {
        int i = (h - 1) * w + col;
        vga[i] = (0x07 << 8) | ' ';
    }
}

// print text to screen
void print(const char* text) {

    uint16_t* vga = (uint16_t*)0xB8000;

    int i = 0;

    while (text[i]) {

        char c = text[i];

        if (c == '\n') {
            terminal.row++;
            terminal.col = 0;
        }
        else {
            int pos = terminal.row * kernel_config.vga_width + terminal.col;
            vga[pos] = (0x07 << 8) | c;

            terminal.col++;
        }

        if (terminal.col >= kernel_config.vga_width) {
            terminal.col = 0;
            terminal.row++;
        }

        if (terminal.row >= kernel_config.vga_height) {
            scroll();
            terminal.row = kernel_config.vga_height - 1;
        }

        i++;
    }
}