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
        gfx::print_str("Pong!\n");
    }

    else if (string::str_cmp(command, "pong"))
    {
        gfx::print_str("Ping!\n");
    }

    else if (string::str_cmp(command, "rand"))
    {
        int rand_md = string::str_int(args);
        math::srand(cmos::now());
        gfx::print_str(int_str(math::rand() % rand_md)); gfx::newl();
    }

    else if(string::str_cmp(command, "clr") || string::str_cmp(command, "clear"))
    {
        gfx::clear_screen();
    }

    else if (string::str_cmp(command, "print"))
    {
        gfx::print_str(args); gfx::newl();
    }

    else if (string::str_cmp(command, (const char*)0xA)); // If enter pressed without a command, do nothing.

    else
    {
        gfx::print_err("Unknown command: ", "[cmd]", false);
        gfx::print_str(command);
        gfx::newl();
    }
}