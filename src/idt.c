#include <idt.h>

extern void keyboard_irq();

struct idt_entry idt[256];
struct idt_ptr idtp;

void set_idt_gate(int n, uint32_t handler) {
    idt[n].base_low = handler & 0xFFFF;
    idt[n].base_high = (handler >> 16) & 0xFFFF;

    idt[n].selector = 0x08;   // kernel code segment
    idt[n].zero = 0;
    idt[n].flags = 0x8E;      // interrupt gate
}

void init_idt() {
    idtp.limit = sizeof(idt) - 1;
    idtp.base = (uint32_t)&idt;

    set_idt_gate(9, (uint32_t)keyboard_irq);

    idt_load((uint32_t)&idtp);
}