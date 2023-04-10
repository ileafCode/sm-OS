#pragma once

#include "../typedefs.h"
#include "../keyboard/keyboard_handler.cpp"

namespace stdio
{
    void clear_input_buffer() // Use this after a getstr() function. Otherwise, it will "combine" a string and the last string together
    {
        for (int i = 0; i < 255; i++)
        {
            buffer[i] = 0; // clear the input buffer
        }
    }

    char* getstr()
    {
        buf_en = true;
        kbd_en = true;
        while (enter_pressed == false);
        kbd_en = false;
        buf_en = false;
        
        enter_pressed = false;

        return (char*)buffer;
    }

    char getch()
    {
        char chr = 0;
        kbd_en = true;
        print_kbd = false;
        while (last_ch == 0);
        chr = last_ch;
        print_kbd = true;
        kbd_en = false;
        last_ch = 0;
        return chr;
    }
}