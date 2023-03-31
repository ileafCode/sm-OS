#pragma once

#include "../typedefs.h"
#include "../io/io.cpp"
#include "../pit/pit.cpp"

namespace beeper
{
	static void play_sound(uint_32 nFrequence)
	{
 		uint_32 Div;
 		uint_8 tmp;

 		Div = 1193180 / nFrequence;
 		outb(0x43, 0xb6);
 		outb(0x42, (uint_8) (Div) );
 		outb(0x42, (uint_8) (Div >> 8));

 		tmp = inb(0x61);
  		if (tmp != (tmp | 3))
		{
 			outb(0x61, tmp | 3);
 		}
	}

	static void no_sound()
	{
 		uint_8 tmp = inb(0x61) & 0xFC;
 		outb(0x61, tmp);
	}

	static void beep(uint_32 freq, uint_64 delay)
	{
		play_sound(freq);
		PIT::sleep_millis(delay);
		no_sound();
	}
}