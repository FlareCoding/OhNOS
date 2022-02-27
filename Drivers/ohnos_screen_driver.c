#include "ohnos_screen_driver.h"

static uint16_t ohnos_CursorPosition = 0;
static char ohnos_HexToStringOutput[128];

uint16_t ohnos_get_cursor_position()
{
    return ohnos_CursorPosition;
}

void ohnos_set_cursor_position(uint16_t pos)
{
    // if (pos > VGA_MAX_INDEX)
    //     pos = VGA_MAX_INDEX;

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

int ohnos_atoi(const char* str)
{
    // Initialize result
    int res = 0;
 
    // Initialize sign as positive
    int sign = 1;
 
    // Initialize index of first digit
    int i = 0;
 
    // If number is negative,
    // then update sign
    if (str[0] == '-') {
        sign = -1;
 
        // Also update index of first digit
        i++;
    }
 
    // Iterate through all digits
    // and update the result
    for (; str[i] != '\0'; i++)
        res = res * 10 + str[i] - '0';
 
    // Return result with sign
    return sign * res;
}

/* A utility function to reverse a string  */
void reverse(char str[], int length)
{
    int start = 0;
    int end = length -1;
    while (start < end)
    {
        char tmp = *(str+end);
		*(str+end) = *(str+start);
		*(str+start) = tmp;

        start++;
        end--;
    }
}

void ohnos_itoa(int num, char* str, int base)
{
    int i = 0;
    int isNegative = 0;
 
    /* Handle 0 explicitly, otherwise empty string is printed for 0 */
    if (num == 0)
    {
        str[i++] = '0';
        str[i] = '\0';
        return;
    }
 
    // In standard itoa(), negative numbers are handled only with
    // base 10. Otherwise numbers are considered unsigned.
    if (num < 0 && base == 10)
    {
        isNegative = 1;
        num = -num;
    }
 
    // Process individual digits
    while (num != 0)
    {
        int rem = num % base;
        str[i++] = (rem > 9)? (rem-10) + 'a' : rem + '0';
        num = num/base;
    }
 
    // If number is negative, append '-'
    if (isNegative)
        str[i++] = '-';
 
    str[i] = '\0'; // Append string terminator
 
    // Reverse the string
    reverse(str, i);
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

        if (cursor_idx > VGA_MAX_INDEX)
        {
            char* copy_start_addr = VGA_MEMORY_ADDR + (VGA_WIDTH * 2);
            uint64_t copy_size = (VGA_MAX_INDEX - VGA_WIDTH) * 2;

            memcpy(copy_start_addr, VGA_MEMORY_ADDR, copy_size);
            memset(VGA_MEMORY_ADDR + copy_size, 0, VGA_WIDTH * 2);
            cursor_idx -= VGA_WIDTH;

            *(VGA_MEMORY_ADDR + (cursor_idx - 1) * 2)       = *char_ptr;
            *(VGA_MEMORY_ADDR + (cursor_idx - 1) * 2 + 1)   = color;
            *(VGA_MEMORY_ADDR + (cursor_idx - 2) * 2)       = 0;
            *(VGA_MEMORY_ADDR + (cursor_idx - 2) * 2 + 1)   = color;
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

    if (ohnos_CursorPosition > VGA_MAX_INDEX)
    {
        char* copy_start_addr = VGA_MEMORY_ADDR + (VGA_WIDTH * 2);
        uint64_t copy_size = (VGA_MAX_INDEX - VGA_WIDTH) * 2;

        memcpy(copy_start_addr, VGA_MEMORY_ADDR, copy_size);
        memset(VGA_MEMORY_ADDR + copy_size, 0, VGA_WIDTH * 2);
        ohnos_CursorPosition -= VGA_WIDTH;

        *(VGA_MEMORY_ADDR + (ohnos_CursorPosition - 1) * 2)       = c;
        *(VGA_MEMORY_ADDR + (ohnos_CursorPosition - 1) * 2 + 1)   = color;
        *(VGA_MEMORY_ADDR + (ohnos_CursorPosition - 2) * 2)       = 0;
        *(VGA_MEMORY_ADDR + (ohnos_CursorPosition - 2) * 2 + 1)   = color;
    }

    uint16_t cp = ohnos_CursorPosition;
    ohnos_set_cursor_position(VGA_WIDTH - 4);
    ohnos_kprint_int((int)cp, VGA_BACKGROUND_BLACK | VGA_FOREGROUND_RED);
    ohnos_set_cursor_position(cp);
}

void ohnos_kprint_int(int i, uint8_t color)
{
    char result[128] = { 0 };
    ohnos_itoa(i, result, 10);
    uint16_t cursor_idx = ohnos_CursorPosition;

    char* result_ptr = result;
    while (*result_ptr)
    {
        *(VGA_MEMORY_ADDR + cursor_idx * 2) = *result_ptr;
        *(VGA_MEMORY_ADDR + cursor_idx * 2 + 1) = color;
        
        ++cursor_idx;
        ++result_ptr;
    }

    ohnos_set_cursor_position(cursor_idx);
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

void memcpy(char* src, char* dest, uint64_t size)
{
    for (uint64_t i = 0; i < size; ++i)
        *(dest + i) = *(src + i);
}

void memset(char* src, byte_t val, uint64_t size)
{
    for (uint64_t i = 0; i < size; ++i)
        src[i] = val;
}
