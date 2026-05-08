#include "terminal.h"
#include "string.h"
#include "keyboard.h"
#include "idt.h"
#include "type.h"
#include "pic.h"

void kernel_main() {
    clear_screen();
    print("Test");
}