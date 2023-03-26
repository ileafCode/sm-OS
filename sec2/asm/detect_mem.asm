
mem_region_count:
    db 0
    GLOBAL mem_region_count

get_mem:
    mov ax, 0
    mov es, ax
    mov di, 0x5000
    mov edx, 0x534D4150
    xor ebx, ebx

    .rep:
        mov eax, 0xE820
        mov ecx, 24
        int 0x15
        
        cmp ebx, 0
        je .finished

        add di, 24
        inc byte [mem_region_count]
        jmp .rep
    
    .finished:
        ret