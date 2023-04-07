#pragma once

#include "../typedefs.h"
#include "../pit/pit.cpp"
#include <cstddef>
#include "font1.h"
#include "../dynamic_mem/heap.cpp"
#include "../dynamic_mem/mem.cpp"

#define GFX_MEM (unsigned char*) 0xA0000

#define GFX_W 320
#define GFX_H 200
#define GFX_PX_SIZE (GFX_W * GFX_H)

uint_8* BFR = 0;

namespace gfx
{
    void clear_screen(uint_8 clear_color = 0x13)
    {
        for (int i = 0; i < (GFX_PX_SIZE); i++) BFR[i] = clear_color;
    }

    void init_db(uint_8* bfr)
    {
        BFR = bfr;
    }

    uint_64 pos_coords(uint_16 x, uint_16 y)
    {
        if (x > GFX_W - 1) x = GFX_W - 1;
        if (y > GFX_H - 1) y = GFX_H - 1;
        return y * GFX_W + x;
    }

    void putpix(uint_16 x, uint_16 y, uint_8 color)
    {
        *(BFR + pos_coords(x, y)) = color;
    }

    void square(uint_16 x, uint_16 y, uint_16 size_x, uint_16 size_y, uint_8 color)
    {
        for (int yy = y; yy < y + size_y; yy++)
        {
            for (int xx = x; xx < x + size_x; xx++)
            {
                putpix(yy, xx, color);
            }
        }
    }

    void circle_no_use(uint_16 xc, uint_16 yc, uint_16 x, uint_16 y, uint_8 color)
    // DO NOT USE THIS FUNCTION - it is a helper function. It doesn't do anything.
    {
        putpix(xc+x, yc+y, color);
        putpix(xc-x, yc+y, color);
        putpix(xc+x, yc-y, color);
        putpix(xc-x, yc-y, color);
        putpix(xc+y, yc+x, color);
        putpix(xc-y, yc+x, color);
        putpix(xc+y, yc-x, color);
        putpix(xc-y, yc-x, color);
    }

    void circle(uint_16 xc, uint_16 yc, uint_16 r, uint_8 color)
    {
        int x = 0, y  = r;
        int d = 3 - 2 * r;
        circle_no_use(xc, yc, x, y, color);
        while (y >= x)
        {
            x++;
            if (d > 0)
            {
                y--;
                d = d + 4 * (x - y) + 10;
            }
            else d = d + 4 * x + 6;
            circle_no_use(xc, yc, x, y, color);
        }
    }

    void flip()
    {
        memcpy(GFX_MEM, BFR, GFX_PX_SIZE);
    }

    uint_16 char_cols = 1;
    uint_16 char_rows = 1;

    void newl()
    {
        char_rows += 8;
        char_cols =  1;
    }

    void print_chr(char chr, uint_8 color = 0x0F)
    {
        uint_8* glyph = font[(size_t) chr];

        if (chr == 0)
        {
            for (size_t yy = 0; yy < 8; yy++)
                for (size_t xx = 0; xx < 8; xx++)
                    putpix(char_cols + xx, char_rows + yy, 0x13);
        }

        for (size_t yy = 0; yy < 8; yy++)
            for (size_t xx = 0; xx < 8; xx++)
                if (glyph[yy] & (1 << xx))
                    putpix(char_cols + xx, char_rows + yy, color);
        
        char_cols += 8;
        if (char_cols > 320)
        {
            newl();
        }
        if (char_rows > 200)
        {
            clear_screen();
            char_rows = 1;
            char_cols = 1;
        }
        flip();
    }

    void print_str(const char* str, uint_8 color = 15)
    {
        int i = 0;
        while (str[i] != 0)
        {
            switch (str[i])
            {
            case '\n':
                newl();

            default:
                print_chr(str[i], color);
                i++;
                break;
            }
        }
    }

    void print_ok(const char* str, const char* who = "[kernel]", bool newline = true)
    {
        print_str(who, 0x2F);
        print_str(": OK | ", 0x2F);
        print_str(str);

        if (newline)
        {
            newl();
        }
    }

    void print_warn(const char* str, const char* who = "[kernel]", bool newline = true)
    {
        print_str(who, 0x0E);
        print_str(": WARN | ", 0x0E);
        print_str(str);
    
        if (newline)
        {
            newl();
        }
    }

    void print_err(const char* str, const char* who = "[kernel]", bool newline = true)
    {
        print_str(who, 0x28);
        print_str(": ERROR | ", 0x28);
        print_str(str);

        if (newline)
        {
            newl();
        }
    }
}
