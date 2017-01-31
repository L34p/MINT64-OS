[ORG 0x00]
[BITS 16]

SECTION .text

jmp 0x07C0:START

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; MINT64 OS Environment Variables
; - Sector counts will automatically adjusted by ImageMaker utility.
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
TOTALSECTORCOUNT:     dw 0x02 ; # of sectors except BootLoader
                              ; Address: 0x7C05

KERNEL32SECTORCOUNT:  dw 0x02 ; # of protected mode kernel sectors
                              ; Address: 0x7C07

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; BootLoader Code Section
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
START:
  ; Initialize Real Mode segments
  mov ax, 0x07C0 ; BootLoader start address (0x7C00)
  mov ds, ax
  mov ax, 0xB800 ; Video memory base address (0xB8000)
  mov es, ax

  ; Initialize stack segment, 0x0000 ~ 0xFFFF (64KB)
  mov ax, 0x0000
  mov ss, ax
  mov sp, 0xFFFE
  mov bp, 0xFFFE

  mov si, 0

.SCREENCLEARLOOP:
  mov byte [ es: si ], 0         ; Character, NULL
  mov byte [ es: si + 1 ], 0x0A  ; Attribute, Light Green
  add si, 2
  cmp si, 80 * 25 * 2            ; 80 characters per line, 25 lines
  jl .SCREENCLEARLOOP

  push BOOTMESSAGE
  push 0
  push 0
  call PRINTMESSAGE
  add sp, 6

  push IMAGELOADINGMESSAGE
  push 1
  push 0
  call PRINTMESSAGE
  add sp, 6

RESETDISK:
  mov ax, 0 ; Reset
  mov dl, 0 ; Floppy disk
  int 0x13  ; BIOS disk I/O service         
  jc HANDLEDISKERROR 

  mov si, 0x1000 ; Read disk data to ES:BS, 0x1000:0000 (0x10000)
  mov es, si
  mov bx, 0x0000

  mov di, word [ TOTALSECTORCOUNT ]

READDATA:
  cmp di, 0
  je READEND
  sub di, 0x1

  mov ah, 0x02 ; Sector read
  mov al, 0x01 ; # of sectors to read
  mov ch, byte [ TRACKNUMBER ]
  mov cl, byte [ SECTORNUMBER ]
  mov dh, byte [ HEADNUMBER ]
  mov dl, 0x00 ; Floppy disk
  int 0x13     ; BIOS disk I/O service
  jc HANDLEDISKERROR

  add si, 0x0020 ; Size of a sector is 512B (0x200)
  mov es, si     ; Move destination memory address

  mov al, byte [ SECTORNUMBER ]
  add al, 0x01
  mov byte [ SECTORNUMBER ], al
  cmp al, 19
  jl READDATA

  xor byte [ HEADNUMBER ], 0x01
  mov byte [ SECTORNUMBER ], 0x01

  cmp byte [ HEADNUMBER ], 0x00
  jne READDATA

  add byte [ TRACKNUMBER ], 0x01
  jmp READDATA

READEND:
  push LOADINGCOMPLETEMESSAGE
  push 1
  push 20
  call PRINTMESSAGE
  add sp, 6

  jmp 0x1000:0x0000 ; Jump to loaded OS image
  

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; BootLoader Function Codes
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
HANDLEDISKERROR:
  push DISKERRORMESSAGE
  push 1
  push 20
  call PRINTMESSAGE

  jmp $ ; infinite loop

; Print message to screen.
; PARAM: X axis, Y axis, STRING
PRINTMESSAGE:
  push bp
  mov bp, sp
  
  push es
  push si
  push di
  push ax
  push cx
  push dx

  mov ax, 0xB800
  mov es, ax

  mov ax, word [ bp + 6 ]
  mov si, 160
  mul si
  mov di, ax

  mov ax, word [ bp + 4 ]
  mov si, 2
  mul si
  add di, ax

  mov si, word [ bp + 8 ]

.MESSAGELOOP:
  mov cl, byte [ si ]
  cmp cl, 0
  je .MESSAGEEND

  mov byte [ es: di ], cl
  add si, 1
  add di, 2
  jmp .MESSAGELOOP

.MESSAGEEND:
  pop dx
  pop cx
  pop ax
  pop di
  pop si
  pop es
  pop bp
  ret

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Data Section
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
BOOTMESSAGE:            db 'MINT64 OS BOOTLOADER', 0
IMAGELOADINGMESSAGE:    db 'OS Image Loading...', 0
DISKERRORMESSAGE:       db 'Disk Error.', 0
LOADINGCOMPLETEMESSAGE: db 'Complete.', 0

SECTORNUMBER:           db 0x02
HEADNUMBER:             db 0x00
TRACKNUMBER:            db 0x00

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; BootSector magic number 0x55AA
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
times 510 - ( $ - $$ )  db 0x00
db 0x55
db 0xAA
