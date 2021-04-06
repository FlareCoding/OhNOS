#include "ohnos_idt.h"
#include "ohnos_kb_scancodes.h"

// There are 256 possible interrupts that the OS can handle.
extern InterruptDescTable ohnos_idt[256];

// Service routines are located in the ohnos_idt.asm
extern uint64_t ohnos_interrupt_service_routine_keyboard;

// Assembly function for loading the IDT
extern void ohnos_load_idt();

static keyboard_handler_t ohnos_KernelKeyboardHandler = 0;

void ohnos_set_kernel_keyboard_handler(keyboard_handler_t fn)
{
    ohnos_KernelKeyboardHandler = fn;
}

void ohnos_init_idt()
{
    ohnos_idt[1].zero = 0;
    ohnos_idt[1].offset_low =   (uint16_t)(((uint64_t)&ohnos_interrupt_service_routine_keyboard & 0x000000000000ffff));
    ohnos_idt[1].offset_mid =   (uint16_t)(((uint64_t)&ohnos_interrupt_service_routine_keyboard & 0x00000000ffff0000) >> 16);
    ohnos_idt[1].offset_high =  (uint32_t)(((uint64_t)&ohnos_interrupt_service_routine_keyboard & 0xffffffff00000000) >> 32);
    ohnos_idt[1].ist = 0;
    ohnos_idt[1].selector = 0x08;
    ohnos_idt[1].types_attribs = 0x8e; // signifies that it's an interrupt

    ohnos_remap_PIC();

    ohnos_byte_out(0x21, 0xFD);
    ohnos_byte_out(0xA1, 0xFF);

    ohnos_load_idt();
}

void ohnos_isr_keyboard_handler()
{
    uint8_t scan_code = ohnos_byte_in(0x60);
    uint8_t chr = 0;

    if (scan_code < 0x3A)
        chr = ohnos_ScanCodes[scan_code];

    if (ohnos_KernelKeyboardHandler != 0)
        ohnos_KernelKeyboardHandler(scan_code, chr);

    // Specify that we finished handling the interrupt.
    ohnos_byte_out(0x20, 0x20);
    ohnos_byte_out(0xA0, 0x20);
}
