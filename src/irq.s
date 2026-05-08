.intel_syntax noprefix

.global keyboard_irq

keyboard_irq:
    pusha
    call keyboard_handler
    popa
    iretd