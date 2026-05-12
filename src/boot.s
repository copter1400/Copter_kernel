.intel_syntax noprefix
.global _start
.extern kernel_main

.set MAGIC, 0x1BADB002
.set FLAGS, 0x0
.set CHECKSUM, -(MAGIC + FLAGS)

.section .multiboot
.align 4

.long 0x1BADB002
.long 0x00000000
.long -(0x1BADB002 + 0x00000000)

.section .text
_start:
    mov esp, offset stack_top
    push ebx      # multiboot info pointer
    push eax      # magic
    call kernel_main
hang:
    cli
    hlt
    jmp hang