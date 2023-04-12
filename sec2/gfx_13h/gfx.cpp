#pragma once

#include "../typedefs.h"
#include "../pit/pit.cpp"
#include <cstddef>
#include "font1.h"
#include "../dynamic_mem/heap.cpp"
#include "../dynamic_mem/mem.cpp"
#include "../math_nums/math.cpp"

#define GFX_MEM (uint_16*) 0xA0000

#define GFX_W 320
#define GFX_H 200
#define GFX_PX_SIZE (GFX_W * GFX_H)

#define TILE_SIZE 16
#define ICON_SIZE  8

#define sign(x) ((x < 0) ? -1 :((x > 0) ? 1 : 0))

uint_8* BFR = 0;

namespace gfx
{
    uint_8 fgc = 0x0F;
    uint_8 bgc = 0x13;

    uint_16 char_cols = 0;
    uint_16 char_rows = 1;

    void clear_screen(uint_8 clear_color = bgc)
    {
        for (int i = 0; i < (GFX_PX_SIZE); i++) *(BFR + i) = clear_color;
        char_cols = 0;
        char_rows = 1;
    }

    void flip()
    {
        memcpy(GFX_MEM, BFR, GFX_PX_SIZE);
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

    uint_8 get_pix(uint_16 x, uint_16 y)
    {
        return *(BFR + pos_coords(x, y));
    }

    void icon(uint_16 x, uint_16 y, uint_8 icon[ICON_SIZE])
    {
        int i = 0;
        for (int yy = y; yy < y + ICON_SIZE; yy++)
        {
            for (int xx = x; xx < x + ICON_SIZE; xx++)
            {
                putpix(yy, xx, icon[i]);
                i++;
            }
        }
    }

    void tile(uint_16 x, uint_16 y, uint_8 tile[TILE_SIZE * TILE_SIZE])
    {
        int i = 0;
        for (int yy = y; yy < y + TILE_SIZE; yy++)
        {
            for (int xx = x; xx < x + TILE_SIZE; xx++)
            {
                putpix(xx, yy, tile[i]);
                i++;
            }
        }
    }

    void line(int x1, int y1, int x2, int y2, uint_8 color)
    {
        int dx = x2-x1;
        int dy = y2-y1;
        int dxabs = __builtin_abs(dx);
        int dyabs = __builtin_abs(dy);
        int sdx = sign(dx);
        int sdy = sign(dy);
        int x = 0;
        int y = 0;
        int px = x1;
        int py = y1;

        putpix(px, py, color);
        if (dxabs >= dyabs)
        {
            for(int i=0;i<dxabs;i++){
                y+=dyabs;
                if (y>=dxabs)
                {
                    y-=dxabs;
                    py+=sdy;
                }
                px+=sdx;
                putpix(px, py, color);
            }
        }
        else
        {
            for(int i=0;i<dyabs;i++)
            {
                x+=dxabs;
                if (x>=dyabs)
                {
                    x-=dyabs;
                    px+=sdx;
                }
                py+=sdy;
                putpix(px, py, color);
            }
        }
    }

    void square(uint_16 x, uint_16 y, uint_16 size_x, uint_16 size_y, uint_8 color)
    {
        for (int yy = y; yy < y + size_y; yy++)
        {
            for (int xx = x; xx < x + size_x; xx++)
            {
                putpix(xx, yy, color);
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

    void newl()
    {
        char_rows += 8;
        char_cols =  0;
    }

    void print_chr(char chr, uint_8 color = fgc, bool flip_s = true)
    {
        uint_8* glyph = font_6x8[(size_t) chr];

        if (chr == 0)
        {
            for (size_t yy = 0; yy < 8; yy++)
                for (size_t xx = 0; xx < 6; xx++)
                    putpix(char_cols + xx, char_rows + yy, bgc);
        }

        for (size_t yy = 0; yy < 6; yy++)
            for (size_t xx = 0; xx < 8; xx++)
                if (glyph[yy] & (1 << xx))
                    putpix(char_cols + yy, char_rows + xx, color);
        
        char_cols += 6;
        if (char_cols > GFX_W) newl();
        if (char_rows > GFX_H)
        {
            char_rows = 1;
            char_cols = 1;
            clear_screen();
        }
        if (flip_s) flip();
    }

    uint_16 last_cols;
    uint_16 last_rows;

    void print_chr_xy(char chr, uint_16 x = char_cols, uint_16 y = char_rows, uint_8 color = fgc, bool flip_s = true)
    {
        last_cols = char_cols;
        last_rows = char_rows;

        char_cols = x;
        char_rows = y;

        uint_8* glyph = font_6x8[(size_t) chr];

        if (chr == 0)
        {
            for (size_t yy = 0; yy < 8; yy++)
                for (size_t xx = 0; xx < 6; xx++)
                    putpix(char_cols + xx, char_rows + yy, bgc);
        }

        for (size_t yy = 0; yy < 6; yy++)
            for (size_t xx = 0; xx < 8; xx++)
                if (glyph[yy] & (1 << xx))
                    putpix(char_cols + yy, char_rows + xx, color);
        
        char_cols += 6;
        if (char_cols > GFX_W)
        {
            newl();
        }
        if (char_rows > GFX_H)
        {
            char_rows = 1;
            char_cols = 1;
            clear_screen();
        }
        if (flip_s) flip();

        char_cols = last_cols;
        char_rows = last_rows;
    }

    void print_str(const char* str, uint_8 color = fgc)
    {
        int i = 0;
        while (str[i] != 0)
        {
            print_chr(str[i], color, false);
            //switch (str[i])
            //{
            //case '\n':
                //newl();
                //i++;
                //break;

            //default:
                //print_chr(str[i], color, false);
                //i++;
                //break;
            //}

            i++;
        }

        flip();
    }

    void print_str_xy(const char* str, uint_16 x = char_cols, uint_16 y = char_rows, uint_8 color = fgc)
    {
        int i = 0;
        while (str[i] != 0)
        {
            switch (str[i])
            {
            case '\n':
                newl();

            default:
                print_chr_xy(str[i], x, y, color, false);
                x += 6;
                i++;
                break;
            }
        }

        //flip();
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