#include "serial.h"
#include "type.h"

void serial_init() {
    outb(0x3F8 + 1, 0x00);  // disable interrupts
    outb(0x3F8 + 3, 0x80);  // enable DLAB
    outb(0x3F8 + 0, 0x03);  // 38400 baud
    outb(0x3F8 + 1, 0x00);
    outb(0x3F8 + 3, 0x03);  // 8 bits, no parity
    outb(0x3F8 + 2, 0xC7);  // enable FIFO
}

void serial_putc(char c) {
    while (!(inb(0x3FD) & 0x20));  // wait for transmit ready
    outb(0x3F8, c);
}

void serial_print(const char* s) {
    while (*s) serial_putc(*s++);
}

void serial_print_int(uint32_t value) {
    if (value == 0) {
        serial_print("0");
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
        serial_print(c);
    }
}

void serial_print_hex(uint32_t value) {
    char hex[] = "0123456789ABCDEF";
    char buffer[11];
    buffer[0] = '0';
    buffer[1] = 'x';
    for (int i = 0; i < 8; i++) {
        uint32_t shift = (7 - i) * 4;
        buffer[i + 2] = hex[(value >> shift) & 0xF];
    }
    buffer[10] = 0;
    serial_print(buffer);
}

static void serial_print_hex_generic(uint32_t value, int digits) {
    char hex[] = "0123456789ABCDEF";
    char buffer[11];

    buffer[0] = '0';
    buffer[1] = 'x';

    for (int i = 0; i < digits; i++) {
        int shift = (digits - 1 - i) * 4;
        buffer[i + 2] = hex[(value >> shift) & 0xF];
    }

    buffer[digits + 2] = '\0';

    serial_print(buffer);
}

void serial_print_hex8(uint8_t value) {
    serial_print_hex_generic(value, 2);
}

void serial_print_hex16(uint16_t value) {
    serial_print_hex_generic(value, 4);
}

void serial_print_hex32(uint32_t value) {
    serial_print_hex_generic(value, 8);
}