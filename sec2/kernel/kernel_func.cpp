#pragma once

#include "../typedefs.h"
#include "../gfx_13h/gfx.cpp"
#include "../io/io.cpp"

namespace kernel
{
    void panic(const char* str)
    {
        gfx::clear_screen(0x29);
        gfx::print_str(str, 0x0F); gfx::newl();

        cli();
        halt();
    }
}
