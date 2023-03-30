#pragma once
#include "../typedefs.h"
#include "../io/io.cpp"

#define from_bcd(val)  ((val / 16) * 10 + (val & 0xf))

struct timeval
{
	uint_32 tv_sec;
	uint_32 tv_usec;
};

namespace cmos
{
	void cmos_dump(uint_16* values)
	{
		uint_16 index;
		for (index = 0; index < 128; ++index)
		{
			outb(0x70, index);
			values[index] = inb(0x71);
		}
	}

	void get_date(uint_16 * month, uint_16 * day)
	{
		uint_16 values[128];
		cmos_dump(values);

		*month = from_bcd(values[8]);
		*day   = from_bcd(values[7]);
	}

	void get_time(uint_16 * hours, uint_16 * minutes, uint_16 * seconds)
	{
		uint_16 values[128];
		cmos_dump(values);

		*hours   = from_bcd(values[4]);
		*minutes = from_bcd(values[2]);
		*seconds = from_bcd(values[0]);
	}

	uint_32 secs_of_years(int years)
	{
		uint_32 days = 0;
		years += 2000;
		while (years > 1969)
    	{
			days += 365;
			if (years % 4 == 0)
        	{
				if (years % 100 == 0)
            	{
					if (years % 400 == 0)
                	{
						days++;
					}
				} 
            	else
            	{
					days++;
				}
			}
			years--;
		}
		return days * 86400;
	}

	uint_32 secs_of_month(int months, int year)
	{
		year += 2000;

		uint_32 days = 0;
		switch(months) {
		case 11:
			days += 30;
		case 10:
			days += 31;
		case 9:
			days += 30;
		case 8:
			days += 31;
		case 7:
			days += 31;
		case 6:
			days += 30;
		case 5:
			days += 31;
		case 4:
			days += 30;
		case 3:
			days += 31;
		case 2:
			days += 28;
			if ((year % 4 == 0) && ((year % 100 != 0) || (year % 400 == 0)))
            {
				days++;
			}
		case 1:
			days += 31;
		default:
			break;
		}
		return days * 86400;
	}

	int gettimeofday(struct timeval * t, void *z)
	{
		uint_16 values[128];
		cmos_dump(values);

		uint_32 time = secs_of_years(from_bcd(values[9]) - 1) +
					secs_of_month(from_bcd(values[8]) - 1, from_bcd(values[9])) + 
					(from_bcd(values[7]) - 1) * 86400 +
					(from_bcd(values[4])) * 3600 +
					(from_bcd(values[2])) * 60 +
					from_bcd(values[0]) +
					0;
		t->tv_sec = time;
		t->tv_usec = 0;
		return 0;
	}

	uint_32 now()
	{
		struct timeval t;
		gettimeofday(&t, 0);
		return t.tv_sec;
	}
}