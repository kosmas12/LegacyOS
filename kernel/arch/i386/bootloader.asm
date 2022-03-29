; Declare constants for the multiboot header.
MBALIGN  equ  1 << 0            ; align loaded modules on page boundaries
MEMINFO  equ  1 << 1            ; provide memory map
FLAGS    equ  MBALIGN | MEMINFO ; this is the Multiboot 'flag' field
MAGIC    equ  0x1BADB002        ; 'magic number' lets bootloader find the header
CHECKSUM equ -(MAGIC + FLAGS)   ; checksum of above, to prove we are multiboot

section .multiboot
align 4
	dd MAGIC
	dd FLAGS
	dd CHECKSUM

extern kernel_main

section .bss
align 16
stack_bottom:
resb 16384 ; 16 KiB
stack_top:

section .text

global GDTFlush        ; Allows the C code to link to this
extern pointerToGDT    ; Says that 'GDTPointer' is in another file
GDTFlush:
    lgdt [pointerToGDT]  ; Load the GDT from the address pointerToGDT points to
    mov ax, 0x10         ; Data Segment offset in the defined GDT
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    jmp 0x08:returnToC ; Code Segment offset to the defined GDT (far jump)
returnToC:
    ret

global IDTLoad
extern pointerToIDT
IDTLoad:
    lidt [pointerToIDT] ; Load IDT (from pointed address)
    sti
    ret

global _start:function (_start.end - _start)
_start:
    cli
	mov esp, stack_top
	call kernel_main

.hang:	hlt
	jmp .hang
.end:
