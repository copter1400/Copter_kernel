.intel_syntax noprefix
.global idt_load
idt_load:
    mov eax, [esp + 4]     # get pointer to idt_ptr
    mov bx, [eax]          # load limit into bx (debug)
    lidt [eax]             # load IDT
    ret