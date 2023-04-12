#pragma once

#include "../typedefs.h"
#include "../stdio/text_print.cpp"
#include "../etc/string.cpp"
#include "../dynamic_mem/heap.cpp"
#include "../gfx_13h/gfx.cpp"

bool left_shift_pressed = false;
bool caps_on = false;
bool enter_pressed = false;
bool backspace_pressed = false;
bool esc_pressed = false;

bool up_pressed = false;
bool down_pressed = false;
bool left_pressed = false;
bool right_pressed = false;

bool kbd_en = false;
bool buf_en = false;
bool print_kbd = true;

uint_8 last_sc;
char last_ch;

static uint_8 buffer[512];
char buf_i;

void standard_kbd_handler(uint_8 sc, uint_8 chr)
{
    if (!kbd_en) return;

    if (chr != 0)
    {
        if (buf_en)
        {
            buffer[buf_i] = chr;
            if (buf_i < 140) buf_i++;
            else gfx::print_str("Too much characters in input buffer!!!");
        }
        if (print_kbd) gfx::print_chr(chr);
        last_ch = chr;
    }
    else
    {
        switch (sc)
        {
        case 0x0E: // Backspace
            if (buf_i <= 0) break;
            gfx::char_cols -= 6;
            gfx::print_chr('\0', gfx::bgc);
            gfx::char_cols -= 6;
            buf_i -= 1;
            buffer[buf_i] = 0;
            break;
        
        case 0x2A: // Left Shift
            left_shift_pressed = true;
            break;
        
        case 0xAA: // Left Shift Released
            left_shift_pressed = false;
            break;
        
        case 0x1C: // Enter
            enter_pressed = true;
            gfx::newl();
            buf_i = 0;
            break;
        
        case 0x3A: // Caps Lock
            caps_on = !caps_on;
            break;

        case 0x01: // Escape
            esc_pressed = true;
            break;
        }
    }

    last_sc = sc;
}

void kbd_arrow(uint_8 sc)
{
    if (!kbd_en) return;
    
    switch (sc)
    {
    case 0x50: // Up pressed
        up_pressed = true;
        break;
    case 0x48: // Down pressed
        down_pressed = true;
        break;
    case 0x4D: // Right pressed
        left_pressed = true;
        break;
    case 0x4B: // Left pressed
        right_pressed = true;
        break;
    
    case 0xD0: // Up released
        up_pressed = false;
        break;
    case 0xC8: // Down released
        down_pressed = false;
        break;
    case 0xCD: // Right released
        left_pressed = false;
        break;
    case 0xCB: // Left released
        right_pressed = false;
        break;
    
    default:
        break;
    }
}

void kbd_handler(uint_8 sc, uint_8 chr)
{
    if (kbd_en)
    {
        switch (last_sc)
        {
        case 0xE0:
            kbd_arrow(sc);
            break;
    
        default:
            standard_kbd_handler(sc, chr);
            break;
        }
    }
    
    last_sc = sc;
}