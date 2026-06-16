#include "config.h"
#include "terminal.h"
#include "type.h"
#include "io.h"

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

// move cursor
void move_cursor() {

    uint16_t pos =
        terminal.row * kernel_config.vga_width
        + terminal.col;

    outb(0x3D4, 0x0F);
    outb(0x3D5, (uint8_t)(pos & 0xFF));

    outb(0x3D4, 0x0E);
    outb(0x3D5, (uint8_t)((pos >> 8) & 0xFF));
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
    move_cursor(0,0);
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
        } else if (c == '\r') {
            terminal.col = 0;
        } else {

            int pos =
                terminal.row * kernel_config.vga_width
                + terminal.col;

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

    move_cursor();
}

// print integer
void print_int(uint32_t value) {

    if (value == 0) {
        print("0");
        return;
    }

    char buffer[16];
    int i = 0;

    while (value > 0) {
        buffer[i++] = '0' + (value % 10);
        value /= 10;
    }

    while (i--) {
        char c[2] = { buffer[i], 0 };
        print(c);
    }
}

// print HEX
void print_hex(uint32_t value) {

    char hex[] = "0123456789ABCDEF";
    char buffer[11];

    buffer[0] = '0';
    buffer[1] = 'x';

    for (int i = 0; i < 8; i++) {

        uint32_t shift = (7 - i) * 4;
        buffer[i + 2] = hex[(value >> shift) & 0xF];
    }

    buffer[10] = 0;

    print(buffer);
}

// print text to screen and color
void print_color(const char* text, uint8_t color) {
    uint16_t* vga = (uint16_t*)0xB8000;

    int i = 0;

    while (text[i]) {

        char c = text[i];

        if (c == '\n') {
            terminal.row++;
            terminal.col = 0;
        }
        else {

            int pos =
                terminal.row * kernel_config.vga_width
                + terminal.col;

            vga[pos] = (color << 8) | c;

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

    move_cursor();
}

// backspace by 1
void terminal_backspace() {

    if (terminal.col > 0) {

        terminal.col--;

        int pos =
            terminal.row * kernel_config.vga_width
            + terminal.col;

        uint16_t* vga = (uint16_t*)0xB8000;

        vga[pos] = (0x07 << 8) | ' ';

        move_cursor();
    }
}