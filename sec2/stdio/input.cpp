#pragma once

#include "../typedefs.h"
#include "../io/io.cpp"
#include "../idt/idt.cpp"
#include "../keyboard/keyboard_handler.cpp"

uint_8 getch(bool p_chr = true, bool enter_p = true)
{
    kbd_en = true;

    while (true)
    {
        if (last_chr != 0)
        {
            uint_8 chr = last_chr;
            if (p_chr)
            {
                print_chr(chr);
            }

            if (enter_p)
                while (enter_pressed == false);

            return chr;
        }
    }

    return 0;
}