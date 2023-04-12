#pragma once

#include "../typedefs.h"
#include "../stdio/text_print.cpp"
#include "../etc/string.cpp"
#include "../stdio/input.cpp"
#include "../math_nums/math.cpp"
#include "../rtc_cmos/cmos.cpp"
#include "../ata/ata.cpp"
#include "game1.cpp"
#include "game2.cpp"

void parse(char* input)
{
    char* inp_1 = 0;
    string::str_cpy(inp_1, input);
    char* command = string::str_tok(input, " ");
    command = string::str_lower(command);

    char* args = 0;
    for (int i = 0; i < string::str_len(inp_1); i++)
    {
        args[i] = inp_1[i + string::str_len(command) + 1];
    }

    if (string::str_cmp(command, "ping"))
    {
        gfx::print_str("Pong!"); gfx::newl();
    }

    else if (string::str_cmp(command, "pong"))
    {
        gfx::print_str("Ping!"); gfx::newl();
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

    else if (string::str_cmp(command, "game1"))
    {
        game1::loop();
        gfx::clear_screen();
    }

    else if (string::str_cmp(command, "game2"))
    {
        game2::loop();
        gfx::clear_screen();
    }
    
    else if (string::str_cmp(command, "txt"))
    {
        while (!esc_pressed)
        {
            stdio::getstr();
            stdio::clear_input_buffer();
        }
        esc_pressed = false;
    }

    else if (string::str_cmp(command, "wait")) 
    {
        int delay_ms = string::str_int(args);
        PIT::sleep_millis(delay_ms);
    }

    else if (string::str_cmp(command, (const char*)0)); // If enter pressed without a command, do nothing.

    else
    {
        gfx::print_err("Unknown command: ", "[cmd]", false);
        gfx::print_str(command);
        gfx::newl();
    }
}