#pragma once
#include "../Drivers/ohnos_io_driver.h"

typedef struct InterruptDescTable
{
    uint16_t    offset_low;
    uint16_t    selector;
    uint8_t     ist;
    uint8_t     types_attribs;
    uint16_t    offset_mid;
    uint32_t    offset_high;
    uint32_t    zero;
} InterruptDescTable, *PInterruptDescTable;

typedef void(*keyboard_handler_t)(uint8_t scan_code, uint8_t chr);

// Initializes the interrupt descriptor table.
void ohnos_init_idt();

// Sets the kernel keyboard handler function.
void ohnos_set_kernel_keyboard_handler(keyboard_handler_t fn);
