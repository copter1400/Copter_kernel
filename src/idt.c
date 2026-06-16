#include "idt.h"
#include "pic.h"
#include "timer.h"

struct idt_entry idt[256];

// set gate
void set_idt_gate(int n, uint32_t handler) {
    idt[n].base_low = handler & 0xFFFF;
    idt[n].base_high = (handler >> 16) & 0xFFFF;
    idt[n].selector = 0x10;   // ← change from 0x08 to 0x10
    idt[n].zero = 0;
    idt[n].flags = 0x8E;
}

// init IDT
void init_idt() {
    for (int i = 0; i < 32; i++)
        set_idt_gate(i, (uint32_t)isr_stub);
    set_idt_gate(32, (uint32_t)timer_irq);
    set_idt_gate(33, (uint32_t)keyboard_irq);
    set_idt_gate(0,  (uint32_t)isr_stub_0);
    set_idt_gate(13, (uint32_t)isr_stub_13);
    set_idt_gate(14, (uint32_t)isr_stub_14);
    set_idt_gate(0, (uint32_t)divide_by_zero_irq);

    struct idt_ptr p;
    p.limit = sizeof(idt) - 1;
    p.base  = (uint32_t)&idt;
    idt_load(&p);
}