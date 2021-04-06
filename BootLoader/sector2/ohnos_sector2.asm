jmp ohnos_enter_protected_mode

%include "ohnos_gdt.asm"
%include "../ohnos_boot_print_utils.asm"

ohnos_enter_protected_mode:
    ; enable a20
    call ohnos_enable_a20

    ; disable interrupts
    cli

    ; load GDT
    lgdt [ohnos_gdt_descriptor]

    mov eax, cr0
    or eax, 1
    mov cr0, eax

    jmp ohnos_codeseg:ohnos_start_protected_mode

ohnos_enable_a20:
    in al, 0x92
    or al, 2
    out 0x92, al
    ret

[bits 32]
%include "ohnos_cpuid.asm"
%include "ohnos_paging.asm"

ohnos_start_protected_mode:
    call ohnos_detect_cpuid
    call ohnos_detect_64bit_mode
    call ohnos_setup_paging
    call ohnos_edit_gdt

    jmp ohnos_codeseg:ohnos_start_64bit_mode

[bits 64]
[extern _kernel_main]

%include "../../Kernel/ohnos_idt.asm"

ohnos_start_64bit_mode:
    mov edi, 0xb8000
    mov rax, 0x0320032003200320
    mov ecx, 500
    rep stosq

    call _kernel_main
    jmp $

times 2048 - ($-$$) db 0