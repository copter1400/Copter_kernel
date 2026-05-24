#ifndef KEYBOARD_H
#define KEYBOARD_H

void keyboard_init();
void keyboard_handler();
char keyboard_pop();

#define KEY_UP    1001
#define KEY_DOWN  1002
#define KEY_LEFT  1003
#define KEY_RIGHT 1004

#endif