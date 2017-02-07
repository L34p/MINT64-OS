[BITS 64]

SECTION .text

; Export assembly functions to use in C files.
global kInPortByte, kOutPortByte

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
