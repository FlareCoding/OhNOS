[org 0x7c00]

mov [BOOT_DISK], dl

mov bp, 0x7c00
mov sp, bp

mov bx, OHNOS_BOOT_MESSAGE
call ohnos_bl_print_string
call ohnos_bl_print_newline

call ohnos_read_disk

jmp PROGRAM_SPACE

%include "../ohnos_boot_print_utils.asm"
%include "ohnos_boot_disk_utils.asm"

OHNOS_BOOT_MESSAGE: db 'Booting up OhNOS...', 0

times 510 - ($-$$) db 0
dw 0xAA55
