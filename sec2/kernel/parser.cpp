#pragma once

#include "../typedefs.h"
#include "../stdio/text_print.cpp"
#include "../etc/string.cpp"
#include "../stdio/input.cpp"
#include "../math_nums/math.cpp"
#include "../rtc_cmos/cmos.cpp"

void parse(char* input)
{

    if (string::str_cmp(input, "ping"))
    {
        stdio::print_str("Pong!\n");
    }

    else if (string::str_cmp(input, "pong"))
    {
        stdio::print_str("Ping!\n");
    }

    else if (string::str_cmp(input, "rand"))
    {
        //stdio::print_str(operand);
        math::srand(cmos::now());
        stdio::print_str(int_str(math::rand() % 100)); stdio::newl();
    }

    else if(string::str_cmp(input, "clr") || string::str_cmp(input, "clear"))
    {
        stdio::clear_screen();
        stdio::set_cursor_pos(pos_coords(0, 0));
    }

    else if (string::str_cmp(input, (const char*)0xA)); // If enter pressed without a command, do nothing.

    else
    {
        stdio::print_err("Unknown command: ", "[cmd]", false);
        stdio::print_str(input);
        stdio::newl();
    }
}