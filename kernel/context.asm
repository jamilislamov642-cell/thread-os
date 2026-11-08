[BITS 32]
section .text

global switch_to
; void switch_to(uint32_t **prev_esp, uint32_t *next_esp);
; Stack on entry: [esp+4] = prev_esp, [esp+8] = next_esp
; After 5 pushes: [esp+24] = prev_esp, [esp+28] = next_esp
switch_to:
    push ebx
    push esi
    push edi
    push ebp
    pushfd

    mov eax, [esp + 24]   ; eax = prev_esp
    mov edx, [esp + 28]   ; edx = next_esp
    mov [eax], esp        ; *prev_esp = current esp
    mov esp, edx          ; esp = next_esp

    popfd
    pop ebp
    pop edi
    pop esi
    pop ebx
    ret

global thread_entry_wrapper
extern thread_exit
; Wrapper executed when a new thread starts for the first time.
; ebx = fn, esi = arg
thread_entry_wrapper:
    sti
    push esi
    call ebx
    add esp, 4
    call thread_exit
.halt_loop:
    hlt
    jmp .halt_loop
