
get_cpuid:

	pushfd 
	pop eax

	mov ecx, eax

	xor eax, 1 << 21

	push eax
	popfd

	pushfd 
	pop eax

	push ecx 
	popfd

	xor eax,ecx
	jz no_cpuid
	ret

get_long_mode:
	mov eax, 0x80000001
	cpuid
	test edx, 1 << 29
	jz no_long_mode
	ret

no_long_mode:
  mov [0xB8000], byte 'L'
  mov [0xB8002], byte 'M'
  mov [0xB8004], byte 'N'
  mov [0xB8006], byte 'E'
  jmp $

no_cpuid:
  mov [0xB8000], byte 'C'
  mov [0xB8002], byte 'P'
  mov [0xB8004], byte 'U'
  mov [0xB8006], byte 'E'
  jmp $