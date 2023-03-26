#pragma once

#include "../typedefs.h"
#include "../io/io.cpp"
#include "../pit/pit.cpp"

#define ADLIB_V_0 0
#define ADLIB_V_1 1
#define ADLIB_V_2 2
#define ADLIB_V_3 3
#define ADLIB_V_4 4
#define ADLIB_V_5 5
#define ADLIB_V_6 6
#define ADLIB_V_7 7
#define ADLIB_V_8 8

void startup_sound();

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

namespace adlib
{
	void init_voices()
	{
		for (uint_8 i = 0; i < 8; i++)
		{
			outb(0x388, 0xA0 | i);
    		outb(0x389, 0xA0);
		}
	}

	void init(uint_8 A_D = 0xE4, uint_8 S_R = 0x9D)
	{
		outb(0x388, 0x20); // Setup
    	outb(0x389, 1);

    	outb(0x388, 0x23);
    	outb(0x389, 1);

    	outb(0x388, 0x40);
    	outb(0x389, 0x1f); // Volume

    	outb(0x388, 0x43);
    	outb(0x389, 0);    // Key Scale Level

    	outb(0x388, 0x60); // Attack and Decay rate - A=Attack, D=Decay. Mapped like this: AAAA DDDD (binary)
    	outb(0x389, A_D);

    	outb(0x388, 0x63);
    	outb(0x389, A_D);

    	outb(0x388, 0x80); // Sustain and Release rate - S=Sustain, R=Release. Mapped like this: SSSS RRRR (binary)
    	outb(0x389, S_R);

    	outb(0x388, 0x83);
    	outb(0x389, S_R);

		init_voices();
	}

	void play_note(uint_8 voice_num, uint_8 note)
	{
		if (voice_num > 8) voice_num = 8;
		if (voice_num < 0) voice_num = 0;

		outb(0x388, 0xB0 | voice_num); // Play note
    	outb(0x389, note);
	}

	void reset_voice(uint_8 voice_num)
	{
		outb(0x388, 0xB0 | voice_num); // Reset voice
    	outb(0x389, 0x0);
	}
}

void startup_sound()
{
	adlib::play_note(ADLIB_V_0, 0x2C);

    PIT::sleep_millis(100);

    adlib::play_note(ADLIB_V_0, 0x2A);

    PIT::sleep_millis(1000);

    adlib::reset_voice(ADLIB_V_0);
}