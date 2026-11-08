[BITS 32]
section .text.entry

global _start
extern kernel_main

_start:
    ; Set up flat protected mode data segments (0x10 = kernel data in GDT)
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax

    ; Kernel stack at 0x90000 (below EBDA/video RAM)
    mov esp, 0x90000
    mov ebp, esp

    ; Clear flags
    push dword 0
    popfd

    ; Jump to C kernel entry point
    call kernel_main

    ; If main ever returns, halt
.halt:
    cli
    hlt
    jmp .halt
