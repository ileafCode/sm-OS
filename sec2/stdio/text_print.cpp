#pragma once

#include "../io/io.cpp"
#include "../typedefs.h"
#include "../dynamic_mem/mem.cpp"

#define VGA_MEM (unsigned char*) 0xb8000
#define VGA_W 80
#define VGA_H 25

#define FOREGROUND_BLACK 0x00
#define FOREGROUND_BLUE 0x01
#define FOREGROUND_GREEN 0x02
#define FOREGROUND_CYAN 0x03
#define FOREGROUND_RED 0x04
#define FOREGROUND_MAGENTA 0x05
#define FOREGROUND_BROWN 0x06
#define FOREGROUND_LIGHTGRAY 0x07
#define FOREGROUND_DARKGRAY 0x08
#define FOREGROUND_LIGHTBLUE 0x09
#define FOREGROUND_LIGHTGREEN 0x0A
#define FOREGROUND_LIGHTCYAN 0x0B
#define FOREGROUND_LIGHTRED 0x0C
#define FOREGROUND_LIGHTMAGENTA 0x0D
#define FOREGROUND_YELLOW 0x0E
#define FOREGROUND_WHITE 0x0F

#define BACKGROUND_BLACK 0x00
#define BACKGROUND_BLUE 0x10
#define BACKGROUND_GREEN 0x20
#define BACKGROUND_CYAN 0x30
#define BACKGROUND_RED 0x40
#define BACKGROUND_MAGENTA 0x50
#define BACKGROUND_BROWN 0x60
#define BACKGROUND_LIGHTGRAY 0x70
#define BACKGROUND_BLINKINGBLACK 0x80
#define BACKGROUND_BLINKINGBLUE 0x90
#define BACKGROUND_BLINKINGGREEN 0xA0
#define BACKGROUND_BLINKINGCYAN 0xB0
#define BACKGROUND_BLINKINGRED 0xC0
#define BACKGROUND_BLINKINGMAGENTA 0xD0
#define BACKGROUND_BLINKINGYELLOW 0xE0
#define BACKGROUND_BLINKINGWHITE 0xF0

uint_16 cursor_pos;

uint_64 default_color = BACKGROUND_BLACK | FOREGROUND_WHITE;

uint_16 pos_coords(uint_8 x, uint_8 y)
{
    return y * VGA_W + x;
}

namespace stdio
{
    void newl();

    void clear_screen(uint_64 clr_color = default_color)
    {
        uint_64 val = 0;
        val += clr_color << 8;
        val += clr_color << 24;
        val += clr_color << 40;
        val += clr_color << 56;

        for (uint_64* i = (uint_64*)VGA_MEM; i < (uint_64*)(VGA_MEM + 4000); i++)
        {
            *i = val;
        }
    }

    void set_cursor_pos(uint_16 position)
    {
        outb(0x3D4, 0x0F);
        outb(0x3D5, (uint_8)(position & 0xFF));
        outb(0x3D4, 0x0E);
        outb(0x3D5, (uint_8)((position >> 8) & 0xFF));

        cursor_pos = position;

        if (cursor_pos >= (VGA_W * VGA_H))
        {
            clear_screen();
            cursor_pos = 0;
        }
    }

    int get_cursor_pos()
    {
        outb(0x3D4, 14);
        int offset = inb(0x3D5) << 8;
        outb(0x3D4, 15);
        offset += inb(0x3D5);
        return offset * 2;
    }

    void print_str(const char* str, uint_8 c = default_color)
    {
        uint_8* char_ptr = (uint_8*)str;
        uint_16 index = cursor_pos;
        while (*char_ptr != 0)
        {
            switch (*char_ptr)
            {
            case 10: // \n + \r
                index += VGA_W;
                index -= index % VGA_W;
                break;

            case 13: // \r
                index -= index % VGA_W;
                break;
        
            default:
                *(VGA_MEM + index * 2) = *char_ptr;
                *(VGA_MEM + index * 2 + 1) = c;
                index++;
                break;
            }
            char_ptr++;
        }

        set_cursor_pos(index);
    }

    void print_ok(const char* str, const char* who = "[kernel]", bool newline = true)
    {
        print_str(who, BACKGROUND_BLACK | FOREGROUND_GREEN);
        print_str(": OK | ", BACKGROUND_BLACK | FOREGROUND_GREEN);
        print_str(str);

        if (newline)
        {
            newl();
        }
    }

    void print_warn(const char* str, const char* who = "[kernel]", bool newline = true)
    {
        print_str(who, BACKGROUND_BLACK | FOREGROUND_YELLOW);
        print_str(": WARN | ", BACKGROUND_BLACK | FOREGROUND_YELLOW);
        print_str(str);
    
        if (newline)
        {
            newl();
        }
    }

    void print_err(const char* str, const char* who = "[kernel]", bool newline = true)
    {
        print_str(who, BACKGROUND_BLACK | FOREGROUND_RED);
        print_str(": ERROR | ", BACKGROUND_BLACK | FOREGROUND_RED);
        print_str(str);

        if (newline)
        {
            newl();
        }
    }

    void print_chr(char chr, uint_8 c = default_color)
    {
        *(VGA_MEM + cursor_pos * 2) = chr;   // Character to print
        *(VGA_MEM + cursor_pos * 2 + 1) = c; // Color of the character
        set_cursor_pos(cursor_pos+1);        // Increment the cursor position
    }

    void newl()
    {
        print_str("\n");
    }
}

char hex_str_o[128];
template<typename T>
const char* hex_str(T val)
{
    T* valPtr = &val;
    uint_8* ptr;
    uint_8 temp;
    uint_8 size = (sizeof(T)) * 2 - 1;
    uint_8 i;
    for (i = 0; i < size; i++)
    {
        ptr = ((uint_8*)valPtr + i);

        // Left side of hex number
        temp = ((*ptr & 0xF0) >> 4);
        hex_str_o[size - (i * 2 + 1)] = temp + (temp > 9 ? 55 : 48);
        
        // Right side of hex number
        temp = ((*ptr & 0x0F));
        hex_str_o[size - (i * 2)] = temp + (temp > 9 ? 55 : 48);
    }
    hex_str_o[size + 1] = 0;
    return hex_str_o;
}

char int_str_o[128];
template<typename T>
const char* int_str(T val)
{
	uint_8 isNegative = 0;

	if (val < 0) {
		isNegative = 1;
		val *= -1;
		int_str_o[0] = '-';
	}

	uint_8 size = 0;
	uint_64 sizeTester = (uint_64)val;
	while (sizeTester / 10 > 0) {
		sizeTester /= 10;
		size++;
	}

	uint_8 index = 0;
	uint_64 newValue = (uint_64)val;
	while (newValue / 10 > 0) {
		uint_8 remainder = newValue % 10;
		newValue /= 10;
		int_str_o[isNegative + size - index] = remainder + 48; 
		index++;
	}
	uint_8 remainder = newValue % 10;
	int_str_o[isNegative + size - index] = remainder + 48;
	int_str_o[isNegative + size + 1] = 0;
	return int_str_o;
}

char float_str_o[128];
const char* float_str(float value, uint_8 decimalPlaces = 4)
{
	char* intPtr = (char*)int_str((int)value);
	char* floatPtr = float_str_o;

	if (value < 0) {
		value *= -1;
	}

	while (*intPtr != 0) {
		*floatPtr = *intPtr;
		intPtr++;
		floatPtr++;
	}
	*floatPtr = '.';
	floatPtr++;

	float newValue = value - (int)value;

	for (uint_8 i = 0; i < decimalPlaces; i++) {
		newValue *= 10;
		*floatPtr = (int)newValue + 48;
		newValue -= (int)newValue;
		floatPtr++;
	}

	*floatPtr = 0;

	return float_str_o;
}

char bin_str_o[64];
template<typename T>
const char* bin_str(T val)
{
    uint_16 size_val = sizeof(val);
    size_val *= 8;
    
    for (int i = size_val; i >= 0; i--)
    {
        uint_64 mask = 1 << i;

        if (val & mask) bin_str_o[size_val - i] = '1';  
        else bin_str_o[size_val - i] = '0';
    }

    return bin_str_o;
}