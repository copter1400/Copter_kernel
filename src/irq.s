.intel_syntax noprefix

.global isr_stub
isr_stub:
    pusha
    call panic_handler
    popa
    iretd

.global isr_stub_0
isr_stub_0:
    push 0
    jmp isr_common

.global isr_stub_13
isr_stub_13:
    push 13
    jmp isr_common_with_err

.global isr_stub_14
isr_stub_14:
    push 14
    jmp isr_common

isr_common:
    pusha
    mov eax, [esp+32]   # get the number we pushed before pusha
                        # pusha pushes 8 regs = 32 bytes, so number is at esp+32
    push eax            # push it as argument to panic_handler
    call panic_handler
    add esp, 4
    popa
    add esp, 4          # clean up original push
    iretd

isr_common_with_err:
    pusha
    mov eax, [esp+36]   # pusha=32 bytes + exception num=4 bytes = 36, then error code
    push eax            # error code
    mov eax, [esp+36]   # exception number
    push eax
    call gpf_handler
    add esp, 8
    popa
    add esp, 8          # exception num + error code
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

    mov eax, esp          # save current esp BEFORE touching stack
    push eax              # pass as argument
    call schedule_irq     # returns new esp in eax
    add esp, 4

    mov esp, eax          # switch to new stack

    popa
    iretd

.global divide_by_zero_irq
divide_by_zero_irq:
    pusha
    call divide_by_zero_handler
    popa
    iretd