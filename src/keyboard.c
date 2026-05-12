#include "io.h"
#include "keyboard.h"

//temp
#include "terminal.h"

#define BUFFER_SIZE 128

static char buffer[BUFFER_SIZE];
static int head = 0;
static int tail = 0;

static const char scancode_map[128] = {
    0,   0,   // 0x00
    '1','2','3','4','5','6','7','8','9','0',
    '-', '=', '\b',

    '\t',      // 0x0F tab
    'q','w','e','r','t','y','u','i','o','p',
    '[',']','\n', // enter

    0,         // ctrl
    'a','s','d','f','g','h','j','k','l',
    ';','\'', '`',

    0,         // left shift
    '\\',
    'z','x','c','v','b','n','m',
    ',', '.', '/',

    0,         // right shift
    '*',
    0,         // alt
    ' ',       // space
};

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

    // ignore key release
    if (sc & 0x80) {
        outb(0x20, 0x20);
        return;
    }

    char c = scancode_map[sc];

    if (c) {
        keyboard_push(c);
    }

    outb(0x20, 0x20);
}

// TEST
// void keyboard_handler() {
//     unsigned char sc = inb(0x60);  // MUST read scancode even in debug
//     print("K");
//     outb(0x20, 0x20);
// }