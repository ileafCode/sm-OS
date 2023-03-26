#pragma once

#include "../typedefs.h"
#include "../print/text_print.cpp"

bool left_shift_pressed = false;
bool caps_on = false;
uint_8 last_sc;

void standard_kbd_handler(uint_8 sc, uint_8 chr)
{
    if (chr != 0)
    {
        print_chr(chr);
    }
    else
    {
        switch (sc)
        {
        case 0x0E: // Backspace
            set_cursor_pos(cursor_pos - 1);
            print_chr(' ');
            set_cursor_pos(cursor_pos - 1);
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
            print_str("\n");
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
    switch (sc)
    {
    case 0x50: // Up
        set_cursor_pos(cursor_pos + VGA_W);
        break;
    case 0x48: // Down
        set_cursor_pos(cursor_pos - VGA_W);
        break;
    case 0x4D: // Right
        set_cursor_pos(cursor_pos + 1);
        break;
    case 0x4B: // Left
        set_cursor_pos(cursor_pos - 1);
        break;
    default:
        break;
    }
}

void kbd_handler(uint_8 sc, uint_8 chr)
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

    last_sc = sc;
}