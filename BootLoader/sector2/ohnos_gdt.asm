ohnos_gdt_nulldesc:
    dd 0
    dd 0

ohnos_gdt_codedesc:
    dw 0xFFFF       ; size
    dw 0x0000       ; base
    db 0x00         ; base offset
    db 10011010b    ; access privileges
    db 11001111b    ; granularity + limit
    db 0x00         ; base high

ohnos_gdt_datadesc:
    dw 0xFFFF       ; size
    dw 0x0000       ; base
    db 0x00         ; base offset
    db 10010010b    ; access privileges
    db 11001111b    ; granularity + limit
    db 0x00         ; base high

ohnos_gdt_end:

ohnos_gdt_descriptor:
    gdt_size:
        dw ohnos_gdt_end - ohnos_gdt_nulldesc - 1
        dq ohnos_gdt_nulldesc

ohnos_codeseg equ ohnos_gdt_codedesc - ohnos_gdt_nulldesc
ohnos_dataseg equ ohnos_gdt_datadesc - ohnos_gdt_nulldesc

[bits 32]
ohnos_edit_gdt:
    mov [ohnos_gdt_codedesc + 6], byte 10101111b
    mov [ohnos_gdt_datadesc + 6], byte 10101111b
    ret

[bits 16]
