#include "ohnos_io_driver.h"

void ohnos_byte_out(uint16_t port, ubyte_t val)
{
    asm volatile ("outb %0, %1" : : "a"(val), "Nd"(port));
}

ubyte_t ohnos_byte_in(uint16_t port)
{
    unsigned char ret;
    asm volatile ("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

void ohnos_remap_PIC()
{
    // For storing the mask of the PIC chip.
    uint8_t a1, a2;

    a1 = ohnos_byte_in(PIC1_DATA);
    a2 = ohnos_byte_in(PIC2_DATA);

    // Tells the master PIC chip to start the initialization sequence.
    ohnos_byte_out(PIC1_COMMAND, ICW1_INIT | ICW1_ICW4);

    // Tells the slave PIC chip to start the initialization sequence.
    ohnos_byte_out(PIC2_COMMAND, ICW1_INIT | ICW1_ICW4);

    // Specify offsets
    ohnos_byte_out(PIC1_DATA, 0);
    ohnos_byte_out(PIC2_DATA, 8);
    ohnos_byte_out(PIC1_DATA, 4);
    ohnos_byte_out(PIC2_DATA, 2);
    ohnos_byte_out(PIC1_DATA, ICW4_8086);
    ohnos_byte_out(PIC2_DATA, ICW4_8086);

    // Restored saved masks.
    ohnos_byte_out(PIC1_DATA, a1);
    ohnos_byte_out(PIC2_DATA, a2);
}
