#pragma once

#include "../typedefs.h"
#include "../io/io.cpp"
#include "../idt/idt.cpp"
#include "../keyboard/keyboard_handler.cpp"

uint_8 getch(bool p_chr = true)
{
    outb(PIC1_DATA, 0b11111100);
    outb(PIC2_DATA, 0b11111111);

    while (true)
    {
        if (last_chr != 0)
        {
            if (p_chr)
            {
                print_chr(last_chr);
            }
            return last_chr;
        }
    }

    return 0;
}