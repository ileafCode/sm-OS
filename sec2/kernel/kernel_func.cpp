#pragma once

#include "../typedefs.h"
#include "../stdio/text_print.cpp"
#include "../io/io.cpp"

namespace kernel
{
    void panic(const char* str)
    {
        stdio::clear_screen(BACKGROUND_RED | FOREGROUND_WHITE);
        stdio::set_cursor_pos(pos_coords(0, 0));
        stdio::print_str(str, BACKGROUND_RED | FOREGROUND_WHITE);
        cli();
        halt();
    }
}
