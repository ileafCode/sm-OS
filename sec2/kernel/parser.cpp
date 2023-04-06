#pragma once

#include "../typedefs.h"
#include "../stdio/text_print.cpp"
#include "../etc/string.cpp"
#include "../stdio/input.cpp"
#include "../math_nums/math.cpp"
#include "../rtc_cmos/cmos.cpp"

void parse(char* input)
{
    char* inp_1 = 0;
    string::str_cpy(inp_1, input);
    char* command = string::str_tok(input, " ");

    char* args = 0;
    for (int i = 0; i < string::str_len(inp_1); i++)
    {
        args[i] = inp_1[i + string::str_len(command) + 1];
    }

    if (string::str_cmp(command, "ping"))
    {
        stdio::print_str("Pong!\n");
    }

    else if (string::str_cmp(command, "pong"))
    {
        stdio::print_str("Ping!\n");
    }

    else if (string::str_cmp(command, "rand"))
    {
        int rand_md = string::str_int(args);
        math::srand(cmos::now());
        stdio::print_str(int_str(math::rand() % rand_md)); stdio::newl();
    }

    else if(string::str_cmp(command, "clr") || string::str_cmp(command, "clear"))
    {
        stdio::clear_screen();
        stdio::set_cursor_pos(pos_coords(0, 0));
    }

    else if (string::str_cmp(command, "print"))
    {
        stdio::print_str(args); stdio::newl();
    }

    else if (string::str_cmp(command, (const char*)0xA)); // If enter pressed without a command, do nothing.

    else
    {
        stdio::print_err("Unknown command: ", "[cmd]", false);
        stdio::print_str(command);
        stdio::newl();
    }
}