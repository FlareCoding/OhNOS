ohnos_detect_cpuid:
    pushfd
    pop eax

    mov ecx, eax
    xor eax, 1 << 21
    
    push eax
    popfd

    pushfd
    pop eax

    push ecx
    popfd

    xor eax, ecx
    jz ohnos_halt

    ret

ohnos_detect_64bit_mode:
    mov eax, 0x80000001
    cpuid
    test edx, 1 << 29
    jz ohnos_halt
    ret

ohnos_halt:
    hlt
