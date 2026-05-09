ENTRY(_start)
SECTIONS
{
    . = 1M;
    .text : {
        *(.multiboot)
        *(.text)
        *(.text.*)
    }
    .data : {
        *(.data)
    }
    .rodata : {
        *(.rodata)
        *(.rodata.*)
    }
    .bss : {
        _bss_start = .;
        *(COMMON)
        *(.bss)
        _bss_end = .;
    }
    . = 0x200000;
    .bootstrap_stack : {
        stack_bottom = .;
        . = . + 16384;
        stack_top = .;
    }
}