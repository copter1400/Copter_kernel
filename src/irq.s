.intel_syntax noprefix

.global isr_stub
isr_stub:
    pusha
    call panic_handler
    popa
    iretd

.global keyboard_irq
keyboard_irq:
    pusha
    call keyboard_handler

    mov al, 0x20
    out 0x20, al

    popa
    iretd

.global timer_irq
timer_irq:
    pusha
    call timer_handler

    mov al, 0x20
    out 0x20, al

    popa
    iretd

.global divide_by_zero_irq
divide_by_zero_irq:
    pusha
    call divide_by_zero_handler
    popa
    iretd