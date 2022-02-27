#pragma once
#include "ohnos_io_driver.h"

#define VGA_MEMORY_ADDR (ubyte_t*)0xb8000
#define VGA_WIDTH  80
#define VGA_HEIGHT 25
#define VGA_MAX_INDEX 2000

#define VGA_FOREGROUND_BLACK            0x00
#define VGA_FOREGROUND_BLUE             0x01
#define VGA_FOREGROUND_GREEN            0x02
#define VGA_FOREGROUND_CYAN             0x03
#define VGA_FOREGROUND_RED              0x04
#define VGA_FOREGROUND_MAGENTA          0x05
#define VGA_FOREGROUND_BROWN            0x06
#define VGA_FOREGROUND_LIGHTGRAY        0x07
#define VGA_FOREGROUND_DARKGRAY         0x08
#define VGA_FOREGROUND_LIGHTBLUE        0x09
#define VGA_FOREGROUND_LIGHTGREEN       0x0A
#define VGA_FOREGROUND_LIGHTCYAN        0x0B
#define VGA_FOREGROUND_LIGHTRED         0x0C
#define VGA_FOREGROUND_LIGHTMAGENTA     0x0D
#define VGA_FOREGROUND_YELLOW           0x0E
#define VGA_FOREGROUND_WHITE            0x0F

#define VGA_BACKGROUND_BLACK            0x00
#define VGA_BACKGROUND_BLUE             0x10
#define VGA_BACKGROUND_GREEN            0x20
#define VGA_BACKGROUND_CYAN             0x30
#define VGA_BACKGROUND_RED              0x40
#define VGA_BACKGROUND_MAGENTA          0x50
#define VGA_BACKGROUND_BROWN            0x60
#define VGA_BACKGROUND_LIGHTGRAY        0x70
#define VGA_BACKGROUND_BLINKING_BLACK   0x80
#define VGA_BACKGROUND_BLINKING_BLUE    0x90
#define VGA_BACKGROUND_BLINKING_GREEN   0xA0
#define VGA_BACKGROUND_BLINKING_CYAN    0xB0
#define VGA_BACKGROUND_BLINKING_RED     0xC0
#define VGA_BACKGROUND_BLINKING_MAGENTA 0xD0
#define VGA_BACKGROUND_BLINKING_YELLOW  0xE0
#define VGA_BACKGROUND_BLINKING_WHITE   0xF0

#define OHNOS_DEFAULT_COLOR (VGA_BACKGROUND_BLACK | VGA_FOREGROUND_CYAN)

#define KERNEL_LOG(str) ohnos_kprint(str, OHNOS_DEFAULT_COLOR)

// Returns the current position of the cursor.
uint16_t ohnos_get_cursor_position();

// Sets the position of the cursor in the VGA card to a given position.
void ohnos_set_cursor_position(uint16_t pos);

// Converts a number to a hexadecimal string.
const char* ohnos_hex_to_string(uint64_t val);

// Converts width/height (x/y) coodinates to a VGA screen index.
uint16_t ohnos_pos_from_coords(uint8_t x, uint8_t y);

// Converts a string to an integer
int ohnos_atoi(const char* str);

// Converts an integer to a string
void ohnos_itoa(int num, char* str, int base);

// Prints a string to screen.
void ohnos_kprint(const char* str, uint8_t color);

// Prints a single character to screen.
void ohnos_kprint_char(char c, uint8_t color);

// Prints an integer to screen
void ohnos_kprint_int(int i, uint8_t color);

// Clears the VGA screen buffer.
void ohnos_kcls();

// Here temporarily
void memcpy(char* src, char* dest, uint64_t size);

// Here temporarily
void memset(char* src, byte_t val, uint64_t size);
