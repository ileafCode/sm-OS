#pragma once

#include "../typedefs.h"
#include "../stdio/text_print.cpp"
#include "../etc/string.cpp"
#include "../stdio/input.cpp"
#include "../math_nums/math.cpp"
#include "../rtc_cmos/cmos.cpp"

void parse(const char* input)
{
    input = string::str_lower((char*)input);
    if (string::str_cmp(input, "ping"))
    {
        print_str("Pong!\n");
    }
    else if (string::str_cmp(input, "pong"))
    {
        print_str("Ping!\n");
    }
    else if (string::str_cmp(input, "rand"))
    {
        math::srand(cmos::now());
        print_str(int_str(math::rand() % 100)); newl();
    }
    else if (string::str_cmp(input, (const char*)0xA)) {} // If enter pressed without a command, do nothing.
    else
    {
        print_err("Unknown command: ", false); print_str(input); newl();
    }

    clear_input_buffer();
}