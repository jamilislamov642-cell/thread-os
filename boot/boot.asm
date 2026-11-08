; =============================================================================
; ThreadOS - Stage 1 MBR Bootloader (512 bytes)
; Loads the 32-bit kernel from sectors 2..N into memory at 0x10000, enables A20,
; switches to 32-bit protected mode, and jumps to the kernel entry point.
; =============================================================================
[BITS 16]
[ORG 0x7C00]

KERNEL_LOAD_SEG  equ 0x1000   ; Kernel loaded at 0x1000:0000 -> linear 0x10000
KERNEL_ENTRY     equ 0x10000
KERNEL_SECTORS   equ 96       ; Read 96 sectors = 48 KB (plenty for kernel)
SPT              equ 18       ; 18 sectors per track (standard 1.44M floppy)
HEADS            equ 2

start:
    cli
    xor ax, ax
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    mov sp, 0x7C00
    sti

    mov [boot_drive], dl      ; Save BIOS boot drive

    ; Set 80x25 16-color text video mode
    mov ax, 0x0003
    int 0x10

    ; Print boot message
    mov si, msg_loading
    call print_str

    ; -------------------------------------------------------------------------
    ; Load kernel sectors using BIOS INT 13h AH=02h (sector by sector)
    ; -------------------------------------------------------------------------
    mov ax, KERNEL_LOAD_SEG
    mov es, ax
    xor bx, bx                ; ES:BX = 0x1000:0000 (0x10000 linear)

    mov dl, [boot_drive]
    mov ch, 0                 ; cylinder/track 0
    mov dh, 0                 ; head 0
    mov cl, 2                 ; start sector = 2 (sector 1 is MBR)
    mov bp, KERNEL_SECTORS    ; sector count

.read_loop:
    mov di, 5                 ; retry counter

.retry:
    mov ah, 0x02              ; BIOS read sectors
    mov al, 1                 ; read 1 sector
    int 0x13
    jnc .read_ok

    ; Reset disk on error and retry
    xor ah, ah
    mov dl, [boot_drive]
    int 0x13
    dec di
    jnz .retry

    ; Disk error
    mov si, msg_disk_err
    call print_str
.halt:
    cli
    hlt
    jmp .halt

.read_ok:
    ; Print progress dot every 16 sectors
    test bp, 15
    jnz .no_dot
    mov al, '.'
    mov ah, 0x0E
    int 0x10
.no_dot:

    ; Advance destination pointer by 512 bytes
    add bx, 512
    jnc .no_segment_advance
    mov ax, es
    add ax, 0x1000
    mov es, ax
.no_segment_advance:

    ; Next sector CHS arithmetic
    inc cl
    cmp cl, SPT
    jbe .chs_ok
    mov cl, 1
    inc dh
    cmp dh, HEADS
    jb .chs_ok
    mov dh, 0
    inc ch
.chs_ok:
    dec bp
    jnz .read_loop

    mov si, msg_done
    call print_str

    ; -------------------------------------------------------------------------
    ; Enable A20 gate
    ; -------------------------------------------------------------------------
    ; Try BIOS enable A20
    mov ax, 0x2401
    int 0x15

    ; Fast A20 port 0x92
    in al, 0x92
    test al, 2
    jnz .a20_ready
    or al, 2
    and al, 0xFE
    out 0x92, al
.a20_ready:

    ; -------------------------------------------------------------------------
    ; Switch to 32-bit Protected Mode
    ; -------------------------------------------------------------------------
    cli
    lgdt [gdt_desc]
    mov eax, cr0
    or eax, 1
    mov cr0, eax

    ; Far jump to flush pipeline and load CS with kernel code selector (0x08)
    jmp dword 0x08:pm_entry

; Print null-terminated string at DS:SI
print_str:
    lodsb
    test al, al
    jz .done
    mov ah, 0x0E
    mov bh, 0
    mov bl, 7
    int 0x10
    jmp print_str
.done:
    ret

; -----------------------------------------------------------------------------
; GDT (Flat 4GB memory model)
; -----------------------------------------------------------------------------
align 8
gdt_start:
    dd 0, 0                           ; Null descriptor
    ; 0x08: Kernel Code - base 0, limit 4GB, exec/read, 32-bit, ring 0
    dw 0xFFFF, 0x0000, 0x9A00, 0x00CF
    ; 0x10: Kernel Data - base 0, limit 4GB, read/write, 32-bit, ring 0
    dw 0xFFFF, 0x0000, 0x9200, 0x00CF
gdt_end:

gdt_desc:
    dw gdt_end - gdt_start - 1
    dd gdt_start

boot_drive:    db 0
msg_loading:   db "Loading ThreadOS kernel", 0
msg_done:      db " OK", 13, 10, 0
msg_disk_err:  db " Disk read error!", 13, 10, 0

; -----------------------------------------------------------------------------
; 32-bit Protected Mode Entry Point
; -----------------------------------------------------------------------------
[BITS 32]
pm_entry:
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    mov esp, 0x90000

    ; Jump to kernel entry at 0x10000
    jmp dword KERNEL_ENTRY

; Pad to 510 bytes and add boot signature
times 510 - ($ - $$) db 0
dw 0xAA55
