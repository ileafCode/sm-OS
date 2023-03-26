
jmp prot_mode

%include "../sec2/asm/gdt.asm"
%include "../sec2/asm/detect_mem.asm"

prot_mode:
  call get_mem
  call en_a20
  cli
  lgdt [gdt_desc]
  mov eax, cr0
  or eax, 1
  mov cr0, eax
  jmp codeseg:start_prot_mode

en_a20:
  in al, 0x92
  or al, 2
  out 0x92, al
  ret

[bits 32]

%include "../sec2/asm/cpuid.asm"
%include "../sec2/asm/paging.asm"

start_prot_mode:
  mov ax, dataseg
  mov ds, ax
  mov ss, ax
  mov es, ax
  mov fs, ax
  mov gs, ax

  mov [0xB8000], byte 'Y'
  mov [0xB8002], byte 'E'
  mov [0xB8004], byte 'S'
  mov [0xB8006], byte '!'

  call get_cpuid
  call get_long_mode
  call id_paging
  call edit_gdt

  jmp codeseg:start_64

[bits 64]
[extern _Z6_startv]

%include "../sec2/idt/idt.asm"

start_64:
  call activate_sse
  call _Z6_startv
  jmp $

activate_sse:
  mov rax, cr0
  and ax, 0b11111101
  or ax, 0b00000001
  mov cr0, rax

  mov rax, cr4
  or ax, 0b1100000000
  mov cr4, rax

  ret