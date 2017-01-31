[BITS 32]

; Export assembly functions to use in C files.
global kReadCPUID, kSwitchAndExecute64bitKernel

SECTION .text

; Read CPUID.
; PARAM: DWORD dwEAX, DWORD* pdwEAX, DWORD* pdwEBX, DWORD* pdwECX, DWORD* pdwEDX
kReadCPUID:
  push ebp
  mov ebp, esp
  push eax
  push ebx
  push ecx
  push edx
  push esi

  ; Call CPUID with EAX value (1st PARAM)
  mov eax, dword [ ebp + 8 ]
  cpuid

  ; Store return values of CPUID
  ; EAX -> *pdwEAX (2nd PARAM)
  mov esi, dword [ ebp + 12 ]
  mov dword [ esi ], eax

  ; EBX -> *pdwEBX (3rd PARAM)
  mov esi, dword [ ebp + 16 ]
  mov dword [ esi ], ebx

  ; ECX -> *pdwECX (4th PARAM)
  mov esi, dword [ ebp + 20 ]
  mov dword [ esi ], ecx

  ; EDX -> *pdwEDX (5th PARAM)
  mov esi, dword [ ebp + 24 ]
  mov dword [ esi ], edx

  pop esi
  pop edx
  pop ecx
  pop ebx
  pop eax
  pop ebp
  ret

; Switch to IA-32e Mode.
; No PARAM
kSwitchAndExecute64bitKernel:
  ; Enable CR4.PAE
  mov eax, cr4
  or eax, 0x20
  mov cr4, eax

  ; Initialize CR3 for paging
  mov eax, 0x100000
  mov cr3, eax
  
  ; Enable IA-32e Mode, IA32_EFER.LME
  mov ecx, 0xC0000080
  rdmsr
  or eax, 0x0100
  wrmsr

  ; Enable Paging and Caching, No Write-through
  ; PG=1, CD=0, NW=0
  mov eax, cr0
  or eax, 0xE0000000
  xor eax, 0x60000000
  mov cr0, eax

  ; Jump to IA-32e Mode Entry Point
  jmp 0x08:0x200000
