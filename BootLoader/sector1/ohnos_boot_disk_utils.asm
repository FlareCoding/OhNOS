PROGRAM_SPACE equ 0x8000

ohnos_read_disk:
    mov ah, 0x02 ; BIOS code for reading the disk
    mov bx, PROGRAM_SPACE
    mov al, 34 ; loading 34 sectors (2048 bytes for sector2 and the rest for the kernel)
    mov dl, [BOOT_DISK]

    mov ch, 0x00 ; cylinder 0
    mov dh, 0x00 ; head 0
    mov cl, 0x02 ; sector 2

    int 0x13

    jc ohnos_disk_read_failed

    mov bx, DISK_READ_SUCCESS_STRING
    call ohnos_bl_print_string
    call ohnos_bl_print_newline

    ret

ohnos_disk_read_failed:
    mov bx, DISK_READ_FAILED_ERROR_STRING
    call ohnos_bl_print_string
    call ohnos_bl_print_newline
    
    jmp $

BOOT_DISK:
    db 0

DISK_READ_SUCCESS_STRING: db '[+] Disk read successfully', 0
DISK_READ_FAILED_ERROR_STRING: db '[-] Disk read failed', 0
