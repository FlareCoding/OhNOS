[extern ohnos_idt]
ohnos_idt_descriptor:
    dw 4095
    dq ohnos_idt

%macro PUSHALL 0
    push rax
    push rcx
    push rdx
    push r8
    push r9
    push r10
    push r11
%endmacro

%macro POPALL 0
    pop r11
    pop r10
    pop r9
    pop r8
    pop rdx
    pop rcx
    pop rax
%endmacro

[extern ohnos_isr_keyboard_handler]
ohnos_interrupt_service_routine_keyboard:
    PUSHALL
    call ohnos_isr_keyboard_handler
    POPALL
    
    iretq
    GLOBAL ohnos_interrupt_service_routine_keyboard

ohnos_load_idt:
    lidt [ohnos_idt_descriptor]
    sti
    ret
    GLOBAL ohnos_load_idt
