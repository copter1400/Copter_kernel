#ifndef IDT_H
#define IDT_H

#include "type.h"

struct idt_entry {
    uint16_t base_low;
    uint16_t selector;
    uint8_t  zero;
    uint8_t  flags;
    uint16_t base_high;
} __attribute__((packed));

struct idt_ptr {
    uint16_t limit;
    uint32_t base;
} __attribute__((packed));

void init_idt();
void set_idt_gate(int n, uint32_t handler);

extern void idt_load(struct idt_ptr* ptr);
extern void keyboard_irq();
extern void timer_irq();
extern void isr_stub();
#endif