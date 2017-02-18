[BITS 64]

SECTION .text

; Export assembly functions to use in C files.
global kInPortByte, kOutPortByte, kLoadGDTR, kLoadTR, kLoadIDTR

; Read single byte from the given port.
; PARAM: port number 
kInPortByte:
  push rdx

  mov rdx, rdi  ; port number
  mov rax, 0
  in al, dx

  pop rdx
  ret

; Write single byte to the given port.
; PARAM: port number, data
kOutPortByte:
  push rdx
  push rax

  mov rdx, rdi  ; port number
  mov rax, rsi  ; data
  out dx, al

  pop rax
  pop rdx
  ret

; Load GDTR
; PARAM: GDT struct address
kLoadGDTR:
  lgdt [rdi]
  ret

; Load TR
; PARAM: TSS Segment descriptor offset
kLoadTR:
  ltr di
  ret

; Load IDTR
; PARAM: IDT struct address
kLoadIDTR:
  lidt [rdi]
  ret
