#include "ohnos_idt.h"
#include "ohnos_kb_scancodes.h"
#include "../Drivers/ohnos_screen_driver.h"

int shift_pressed = 0;
int kernel_prompt_offset = 8;

const char* OPTIONS_STRING = "Available Commands:\r\n"
                            "    whoami\r\n"
                            "    ifconfig\r\n"
                            "    clear\r\n"
                            "    welcome\r\n";

int strcmp(const char* X, const char* Y)
{
    while(*X)
    {
        if (*X != *Y)
            break;
 
        X++;
        Y++;
    }
 
    return *(const char*)X - *(const char*)Y;
}

void parse_user_input(const char* input, char** output)
{
    if (strcmp(input, "whoami") == 0)
        *output = (char*)"user:root";

    else if (strcmp(input, "ifconfig") == 0)
        *output = (char*)"127.0.0.1 [LAN not yet supported]";

    else if (strcmp(input, "options") == 0 || strcmp(input, "help") == 0)
        *output = (char*)OPTIONS_STRING;
        
    else if (strcmp(input, "welcome") == 0)
        *output = (char*)"Welcome to OhNOS!\r\nPlease enjoy your stay here :)";

    else
        *output = (char*)"Command not found, use \"help\" to see available commands.";
}

void handle_user_input(const char* input)
{
    ohnos_kcls();
    ohnos_set_cursor_position(0);
    ohnos_kprint("Kernel", VGA_BACKGROUND_BLACK | VGA_FOREGROUND_WHITE);
    ohnos_kprint("> ", VGA_BACKGROUND_BLACK | VGA_FOREGROUND_RED);

    if (strcmp(input, "clear") == 0)
        return;

    ohnos_kprint("\r\n", OHNOS_DEFAULT_COLOR);

    char* output = (char*)input;
    parse_user_input(input, &output);

    ohnos_kprint(output, VGA_BACKGROUND_BLACK | VGA_FOREGROUND_LIGHTCYAN);

    
    ohnos_set_cursor_position(kernel_prompt_offset);
}

void _keyboard_handler(uint8_t scan_code, uint8_t chr)
{
    if (chr)
    {
        if (shift_pressed)
            chr -= 32;

        ohnos_kprint_char(chr, VGA_BACKGROUND_BLACK | VGA_FOREGROUND_WHITE);
    }
    else
    {
        switch (scan_code)
        {
        case OHNOS_KEYCODE_BACKSPACE:
        {
            if (ohnos_get_cursor_position() > kernel_prompt_offset)
            {
                ohnos_set_cursor_position(ohnos_get_cursor_position() - 1);
                ohnos_kprint_char(' ', VGA_BACKGROUND_BLACK | VGA_FOREGROUND_WHITE);
                ohnos_set_cursor_position(ohnos_get_cursor_position() - 1);
            }
            break;
        }
        case OHNOS_KEYCODE_LSHIFT_PRESSED:
        {
            shift_pressed = 1;
            break;
        }
        case OHNOS_KEYCODE_LSHIFT_RELEASED:
        {
            shift_pressed = 0;
            break;
        }
        case OHNOS_KEYCODE_RSHIFT_PRESSED:
        {
            shift_pressed = 1;
            break;
        }
        case OHNOS_KEYCODE_RSHIFT_RELEASED:
        {
            shift_pressed = 0;
            break;
        }
        case OHNOS_KEYCODE_RETURN:
        {
            char input[256];
            int idx = kernel_prompt_offset;
            char current_char = *(VGA_MEMORY_ADDR + idx * 2);

            while (current_char != '\0')
            {
                current_char = *(VGA_MEMORY_ADDR + idx * 2);
                input[idx - kernel_prompt_offset] = current_char;

                idx++;
            }

            input[idx + 1] = '\0';

            handle_user_input(input);
            break;
        }
        default:
            break;
        }
    }
}

void _kernel_main()
{
    ohnos_kcls();
    ohnos_set_cursor_position(0);
    
    // Initialize interrupt descriptor table.
    ohnos_init_idt();

    // Settings the keyboard handler.
    ohnos_set_kernel_keyboard_handler(_keyboard_handler);

    ohnos_kprint("Kernel", VGA_BACKGROUND_BLACK | VGA_FOREGROUND_WHITE);
    ohnos_kprint("> ", VGA_BACKGROUND_BLACK | VGA_FOREGROUND_RED);

    return;
}