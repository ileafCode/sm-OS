#pragma once

#include "../typedefs.h"
#include "../stdio/text_print.cpp"
#include "../etc/string.cpp"
#include "../dynamic_mem/heap.cpp"

bool left_shift_pressed = false;
bool caps_on = false;
bool enter_pressed = false;
bool backspace_pressed = false;
bool kbd_en = false;
uint_8 last_sc;

static char buffer[512];
char buf_i;

void standard_kbd_handler(uint_8 sc, uint_8 chr)
{
    if (chr != 0)
    {   
        buffer[buf_i] = chr;
        buf_i++;
        print_chr(chr);
    }
    else
    {
        switch (sc)
        {
        case 0x0E: // Backspace
            if (buf_i <= 0) break;
            backspace_pressed = true;
            set_cursor_pos(cursor_pos - 1);
            print_chr(' ');
            set_cursor_pos(cursor_pos - 1);
            buf_i -= 1;
            buffer[buf_i] = 0;
            break;
        
        case 0x0F: // Tab
            print_str("    ");
            break;
        
        case 0x2A: // Left Shift
            left_shift_pressed = true;
            break;
        
        case 0xAA: // Left Shift Released
            left_shift_pressed = false;
            break;
        
        case 0x1C: // Enter
            enter_pressed = true;
            newl();
            buf_i = 0;
            break;
        
        case 0x3A: // Caps Lock
            caps_on = !caps_on;
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
    case 0x50: // Up
        //set_cursor_pos(cursor_pos + VGA_W);
        break;
    case 0x48: // Down
        //set_cursor_pos(cursor_pos - VGA_W);
        break;
    case 0x4D: // Right
        buf_i++;
        set_cursor_pos(cursor_pos + 1);
        break;
    case 0x4B: // Left
        if (buf_i <= 0) return;
        buf_i--;
        set_cursor_pos(cursor_pos - 1);
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