.intel_syntax noprefix

.global keyboard_irq
.extern keyboard_handler

keyboard_irq:
    pusha

    call keyboard_handler

    popa
    iret