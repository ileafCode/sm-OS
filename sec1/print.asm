
print_char:
    pusha
        mov ah, 0x0e
        int 0x10
    popa
    ret

print:
    pusha           ; preserve the registers 
                    ; on the stack.
    mov bx, ax
    print_string_loop:
        mov al, [bx]            ;move buffer index to al
        cmp al, 0               ;if [[ al == 0 ]]; then
        je print_string_end    ;   goto print_string_end
        inc bx                  ;else bx++
        call print_char
        jmp print_string_loop  ;   goto print_string_loop

    print_string_end:
    popa
    ret

print_hex:
    mov cx, 4	; offset in string, counter (4 hex characters)
    .hex_loop:
        mov ax, dx	              ; Hex word passed in DX
        and al, 0Fh               ; Use nibble in AL
        mov bx, hex_to_ascii
        xlatb                     ; AL = [DS:BX + AL]

        mov bx, cx                ; Need bx to index data
        mov [hexString+bx+1], al  ; Store hex char in string
        ror dx, 4                 ; Get next nibble
    loop .hex_loop 

    mov si, hexString             ; Print out hex string
    mov ah, 0Eh
    mov cx, 6                     ; Length of string
    .loop:
        lodsb
        int 10h
    loop .loop
    ret

hexString: db '0x0000'
hex_to_ascii: db '0123456789ABCDEF'