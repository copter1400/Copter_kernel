.intel_syntax noprefix

.global isr_stub
isr_stub:
    cli
    hlt         # just freeze so you can see where it died

.global keyboard_irq
keyboard_irq:
    pusha
    call keyboard_handler
    popa
    iretd

.global timer_irq
timer_irq:
    pusha
    call timer_handler
    popa
    iretd