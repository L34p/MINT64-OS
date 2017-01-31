[BITS 64]

SECTION .text

; Import IA-32e Mode C Language Kernel main function
extern Main

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; 64-Bit IA-32e Mode Entry Point
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
START:
  ; Initialize IA-32e Mode segments (DS, ES, FS, GS, SS)
  mov ax, 0x10
  mov ds, ax
  mov es, ax
  mov fs, ax
  mov gs, ax

  ; Initialize stack segment, 0x600000 ~ 0x6FFFFF (1MB)
  mov ss, ax
  mov rsp, 0x6FFFF8
  mov rbp, 0x6FFFF8

  ; Call IA-32e Mode C Language Kernel. (02.Kernel64/Source/Main.c)
  call Main

  jmp $
