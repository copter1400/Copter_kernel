#include "io.h"
#include "keyboard.h"

#include "terminal.h"

#define BUFFER_SIZE 128

static char buffer[BUFFER_SIZE];
static int head = 0;
static int tail = 0;

static int e0_mode = 0;

// PS/2 Set 1 scancode map
static const char scancode_map[128] = {
    0, 0,

    '1','2','3','4','5','6','7','8','9','0',
    '-', '=', '\b',

    '\t',
    'q','w','e','r','t','y','u','i','o','p',
    '[',']','\n',

    0,
    'a','s','d','f','g','h','j','k','l',
    ';','\'','`',

    0,
    '\\',
    'z','x','c','v','b','n','m',
    ',', '.', '/',

    0,
    '*',
    0,
    ' ',
};

// push to ring buffer
static void keyboard_push(int c) {
    int next = (head + 1) % BUFFER_SIZE;

    if (next == tail)
        return;

    buffer[head] = (char)c;
    head = next;
}

// pop from buffer
char keyboard_pop() {
    if (tail == head)
        return 0;

    char c = buffer[tail];
    tail = (tail + 1) % BUFFER_SIZE;
    return c;
}

// keyboard init
void keyboard_init() {

    // enable keyboard via controller command port
    outb(0x64, 0xAE);

    // clear buffer
    (void)inb(0x60);
}

// IRQ handler
void keyboard_handler() {
    unsigned char sc = inb(0x60);

    outb(0x20, 0x20);

    // extended key prefix
    if (sc == 0xE0) {
        e0_mode = 1;
        return;
    }

    // handle arrow key
    if (e0_mode) {
        e0_mode = 0;

        switch (sc) {
            case 0x48: keyboard_push(KEY_UP);    return;
            case 0x50: keyboard_push(KEY_DOWN);  return;
            case 0x4B: keyboard_push(KEY_LEFT);  return;
            case 0x4D: keyboard_push(KEY_RIGHT); return;
        }

        return;
    }

    // ignore key releases
    if (sc & 0x80)
        return;

    // for number
        switch(sc) {
        case 0x47: keyboard_push('7'); break;
        case 0x48: keyboard_push('8'); break;
        case 0x49: keyboard_push('9'); break;
        case 0x4B: keyboard_push('4'); break;
        case 0x4C: keyboard_push('5'); break;
        case 0x4D: keyboard_push('6'); break;
        case 0x4F: keyboard_push('1'); break;
        case 0x50: keyboard_push('2'); break;
        case 0x51: keyboard_push('3'); break;
        case 0x52: keyboard_push('0'); break;
    }

    // normal keys
    if (sc < 128) {
        char c = scancode_map[sc];

        if (c) {
            keyboard_push(c);
        }
    }
}

// TEST
// void keyboard_handler() {

//     unsigned char sc = inb(0x60);

//     outb(0x20, 0x20);

//     print("K");
// }