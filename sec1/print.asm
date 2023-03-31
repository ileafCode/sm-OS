
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
