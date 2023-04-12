
jmp prot_mode

%include "../sec2/asm/gdt.asm"
%include "../sec2/asm/detect_mem.asm"

prot_mode: ; 16-bit Protected Mode
  call get_mem
  call en_a20
  cli
  lgdt [gdt_desc]
  mov eax, cr0
  or eax, 1
  mov cr0, eax
  jmp codeseg:start_32

en_a20:
  in al, 0x92
  or al, 2
  out 0x92, al
  ret

[bits 32]

%include "../sec2/asm/cpuid.asm"
%include "../sec2/asm/paging.asm"

start_32: ; 32-bit Protected Mode
  mov ax, dataseg
  mov ds, ax
  mov ss, ax
  mov es, ax
  mov fs, ax
  mov gs, ax

  ; Now moving to 64-bit land.
  call get_cpuid
  call get_long_mode
  call id_paging
  call edit_gdt

  jmp codeseg:start_64

[bits 64]
[extern _Z6_startv]

%include "../sec2/idt/idt.asm"

start_64: ; 64-bit Protected Mode
  call activate_sse
  jmp _Z6_startv

activate_sse:
  mov rax, cr0
  and ax, 0b11111101
  or ax, 0b00000001
  mov cr0, rax

  mov rax, cr4
  or ax, 0b1100000000
  mov cr4, rax

  ret