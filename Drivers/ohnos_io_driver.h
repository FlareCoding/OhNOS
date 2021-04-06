#pragma once
#include "../Kernel/ohnos_types.h"

// PIC Chip Control
#define PIC1_COMMAND 0x20
#define PIC1_DATA 0x21

#define PIC2_COMMAND 0xA0
#define PIC2_DATA 0xA1

// Initialization Control Word
#define ICW1_INIT 0x10
#define ICW1_ICW4 0x01
#define ICW4_8086 0x01

void ohnos_byte_out(uint16_t port, ubyte_t val);
ubyte_t ohnos_byte_in(uint16_t port);

// Remaps the PIC chip.
void ohnos_remap_PIC();
