ohnos_bl_print_string:
    push ax
    push bx

    mov ah, 0x0e ; tty
    
    .loop:
    cmp [bx], byte 0
    je .exit

    mov al, [bx]
    int 0x10
    inc bx
    jmp .loop
        
    .exit:

    pop ax
    pop bx
    ret

ohnos_bl_print_newline:
    push ax
    push bx
    
    mov ah, 0x0e ; tty
    mov al, 0x0a ; newline char
    int 0x10
    mov al, 0x0d ; carriage return
    int 0x10
    
    pop ax
    pop bx
    ret
