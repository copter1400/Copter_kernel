#include "io.h"
#include "keyboard.h"

//temp
#include "terminal.h"

#define BUFFER_SIZE 128

static char buffer[BUFFER_SIZE];
static int head = 0;
static int tail = 0;

// turn scancode to ascii
static char scancode_to_ascii(unsigned char sc) {
    switch (sc) {
        case 0x1E: return 'a';
        case 0x30: return 'b';
        case 0x2E: return 'c';
        case 0x20: return 'd';
        case 0x12: return 'e';
        case 0x21: return 'f';
        case 0x22: return 'g';
        case 0x23: return 'h';
        case 0x17: return 'i';
        case 0x24: return 'j';
        case 0x25: return 'k';
        case 0x26: return 'l';
        case 0x32: return 'm';
        case 0x31: return 'n';
        case 0x18: return 'o';
        case 0x19: return 'p';
        case 0x10: return 'q';
        case 0x13: return 'r';
        case 0x1F: return 's';
        case 0x14: return 't';
        case 0x16: return 'u';
        case 0x2F: return 'v';
        case 0x11: return 'w';
        case 0x2D: return 'x';
        case 0x15: return 'y';
        case 0x2C: return 'z';

        case 0x39: return ' ';   // space
        case 0x1C: return '\n';  // enter
        case 0x0E: return '\b';  // backspace

        default: return 0;
    }
}

// push char to buffer
static void keyboard_push(char c) {
    int next = (head + 1) % BUFFER_SIZE;

    // if buffer is full
    if (next == tail) {
        return; // ignore input
    }

    buffer[head] = c;
    head = next;
}

// pop out tail input
char keyboard_pop() {
    if (tail == head)
        return 0; // empty

    char c = buffer[tail];
    tail = (tail + 1) % BUFFER_SIZE;
    return c;
}
// IRQ handle
void keyboard_handler() {
    unsigned char sc = inb(0x60);
    char c = scancode_to_ascii(sc);
    if (c)
        keyboard_push(c);
    outb(0x20, 0x20);
}

// TEST
// void keyboard_handler() {
//     unsigned char sc = inb(0x60);  // MUST read scancode even in debug
//     print("K");
//     outb(0x20, 0x20);
// }