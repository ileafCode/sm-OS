#pragma once
#include "../typedefs.h"

void memset(void* start, uint_64 val, uint_64 num)
{
    if (num <= 8)
    {
        uint_8* val_ptr = (uint_8*)&val;
        for (uint_8* ptr = (uint_8*)start; ptr < (uint_8*)((uint_64)start + num); ptr++)
        {
            *ptr = *val_ptr;
            val_ptr++;
        }

        return;
    }

    uint_64 pr_bytes = num % 8;
    uint_64 new_num = num - pr_bytes;

    for (uint_64* ptr = (uint_64*)start; ptr < (uint_64*)((uint_64)start + new_num); ptr++)
    {
        *ptr = val;
    }

    uint_8* val_ptr = (uint_8*)&val;
    for (uint_8* ptr = (uint_8*)((uint_64)start + new_num); ptr < (uint_8*)((uint_64)start + num); ptr++)
    {
        *ptr = *val_ptr;
        val_ptr++;
    }
}

void memcpy(void* dest, void* source, uint_64 num)
{
    if (num <= 8)
    {
        uint_8* val_ptr = (uint_8*)source;
        for (uint_8* ptr = (uint_8*)dest; ptr < (uint_8*)((uint_64)dest + num); ptr++)
        {
            *ptr = *val_ptr;
            val_ptr++;
        }

        return;
    }

    uint_64 pr_bytes = num % 8;
    uint_64 new_num  = num - pr_bytes;
    uint_64* src_ptr = (uint_64*)source;

    for (uint_64* destptr = (uint_64*)dest; destptr < (uint_64*)((uint_64)dest + new_num); destptr++)
    {
        *destptr = *src_ptr;
        src_ptr++;
    }

    uint_8* src_ptr8 = (uint_8*)((uint_64)source + new_num);
    for (uint_8* destptr8 = (uint_8*)((uint_64)dest + new_num); destptr8 < (uint_8*)((uint_64)dest + num); destptr8++)
    {
        *destptr8 = *src_ptr8;
        src_ptr8++;
    }
}
 
int memcmp(const void* aptr, const void* bptr, uint_64 size)
{
	const unsigned char* a = (const unsigned char*) aptr;
	const unsigned char* b = (const unsigned char*) bptr;

	for (uint_64 i = 0; i < size; i++)
    {
		if (a[i] < b[i]) return -1;
		else if (b[i] < a[i]) return 1;
	}
	return 0;
}