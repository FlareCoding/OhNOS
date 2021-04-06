#include "ohnos_screen_driver.h"

static uint16_t ohnos_CursorPosition = 0;
static char ohnos_HexToStringOutput[128];

uint16_t ohnos_get_cursor_position()
{
    return ohnos_CursorPosition;
}

void ohnos_set_cursor_position(uint16_t pos)
{
    if (pos > VGA_MAX_INDEX)
        pos = VGA_MAX_INDEX;

    if (pos < 0)
        pos = 0;

    ohnos_byte_out(0x3D4, 0x0F);
    ohnos_byte_out(0x3D5, (ubyte_t)(pos & 0xFF));

    ohnos_byte_out(0x3D4, 0x0E);
    ohnos_byte_out(0x3D5, (ubyte_t)((pos >> 8) & 0xFF));

    ohnos_CursorPosition = pos;
}

uint16_t ohnos_pos_from_coords(uint8_t x, uint8_t y)
{
    return y * VGA_WIDTH + x;
}

const char* ohnos_hex_to_string(uint64_t val)
{
    uint64_t* val_ptr = &val;
    uint8_t size = sizeof(uint64_t) * 2 - 1;

    for (uint8_t i = 0; i < size; i++)
    {
        uint8_t* ptr = ((uint8_t*)val_ptr + i);
        uint8_t tmp = ((*ptr & 0xF0) >> 4);
        ohnos_HexToStringOutput[size - (i * 2 + 1)] = tmp + (tmp > 9 ? 55 : 48);
        tmp = ((*ptr & 0x0F));
        ohnos_HexToStringOutput[size - (i * 2)] = tmp + (tmp > 9 ? 55 : 48);
    }

    ohnos_HexToStringOutput[size + 1] = '\0';
    return ohnos_HexToStringOutput;
}

void ohnos_kprint(const char* str, uint8_t color)
{
    char* char_ptr = (char*)str;
    uint16_t cursor_idx = ohnos_CursorPosition;

    while (*char_ptr != 0)
    {
        switch (*char_ptr)
        {
            case 10: // newline
                cursor_idx += VGA_WIDTH;
                break;
            case 13: // carriage return
                cursor_idx -= cursor_idx % VGA_WIDTH;
                break;
            default:
                *(VGA_MEMORY_ADDR + cursor_idx * 2) = *char_ptr;
                *(VGA_MEMORY_ADDR + cursor_idx * 2 + 1) = color;
                cursor_idx++;
                break;
        }

        char_ptr++;
    }

    ohnos_set_cursor_position(cursor_idx);
}

void ohnos_kprint_char(char c, uint8_t color)
{
    *(VGA_MEMORY_ADDR + ohnos_CursorPosition * 2) = c;
    *(VGA_MEMORY_ADDR + ohnos_CursorPosition * 2 + 1) = color;

    ohnos_set_cursor_position(ohnos_CursorPosition + 1);
}

void ohnos_kcls()
{
    int screen_size = VGA_WIDTH * VGA_HEIGHT;
    char* screen = (char*)VGA_MEMORY_ADDR;

    for (int i = 0; i < screen_size; i++)
    {
        *(VGA_MEMORY_ADDR + i * 2) = '\0';
        *(VGA_MEMORY_ADDR + i * 2 + 1) = 0x00;
    }

    ohnos_set_cursor_position(0);
}
