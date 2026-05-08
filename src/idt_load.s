.intel_syntax noprefix

.global idt_load

idt_load:
    mov eax, [esp + 4]   # argument (pointer to idt_ptr)
    lidt [eax]           # load IDT into CPU
    ret