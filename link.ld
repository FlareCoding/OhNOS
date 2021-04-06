OUTPUT_FORMAT(binary)

ENTRY(_kernel_main)

INPUT
(
    BootLoader/bin/ohnos_sector2.o

    Kernel/bin/ohnos_kernel.o
    Kernel/bin/ohnos_kb_scancodes.o
    Kernel/bin/ohnos_idt.o

    Drivers/bin/ohnos_io_driver.o
    Drivers/bin/ohnos_screen_driver.o
)

OUTPUT
(
    ohnos_kernel.bin
)

SECTIONS
{
    . = 0x8000;

    .text : ALIGN(0x1000)
    {
        *(.text)
    }

    .idt BLOCK(0x1000) : ALIGN(0x1000)
    {
        ohnos_idt = .;
        . = . + 0x1000;
    }

    .data : ALIGN(0x1000)
    {
        *(.data)
    }

    .rodata : ALIGN(0x1000)
    {
        *(.rodata)
    }

    .bss : ALIGN(0x1000)
    {
        *(COMMON)
        *(.bss)
    }
}
