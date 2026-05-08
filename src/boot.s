.intel_syntax noprefix

.global _start
.extern kernel_main

.set MAGIC, 0x1BADB002
.set FLAGS, 0x0
.set CHECKSUM, -(MAGIC + FLAGS)

.section .multiboot
.align 4
.long MAGIC
.long FLAGS
.long CHECKSUM

.section .bss
.align 16
stack_bottom:
.space 16384
stack_top:

.section .text

_start:
    mov esp, offset stack_top

    call kernel_main

hang:
    cli
    hlt
    jmp hang