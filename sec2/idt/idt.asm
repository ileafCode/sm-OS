[extern _idt]

idt_desc:
  dw 4095
  dq _idt

  %macro PUSHALL 0
  	push rax
  	push rcx
  	push rdx
  	push r8
  	push r9
  	push r10
  	push r11
  %endmacro

  %macro POPALL 0
  	pop r11
  	pop r10
  	pop r9
  	pop r8
  	pop rdx
  	pop rcx
  	pop rax

  %endmacro

[extern isr0_handler]
[extern isr1_handler]

isr0:
  PUSHALL
  call isr0_handler
  POPALL
  iretq
  GLOBAL isr0

isr1:
  PUSHALL
  call isr1_handler
  POPALL
  iretq
  GLOBAL isr1

load_idt:
  lidt [idt_desc]
  sti
  ret
  GLOBAL load_idt