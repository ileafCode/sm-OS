#pragma once

#include "../typedefs.h"
#include "../stdio/text_print.cpp"

struct mem_map_entry 
{
    uint_64 base_address;
    uint_64 region_len;
    uint_32 region_type;
    uint_32 ext_attr;
};

extern uint_8 mem_region_count;
mem_map_entry* usable_mem_regions[10];
bool mem_regions_got = false;
uint_8 usable_mem_region_count;

void print_mem_map(mem_map_entry* mem_map)
{
    print_str("Memory Base: "); print_str(int_str(mem_map -> base_address));
    print_str("\n");
    print_str("Region Length: "); print_str(int_str(mem_map -> region_len));
    print_str("\n");
    print_str("Region Type: "); print_str(int_str(mem_map -> region_type));
    print_str("\n");
    print_str("Extended Attributes: "); print_str(int_str(mem_map -> ext_attr));
    print_str("\n\n");
}

mem_map_entry** get_usable_mem_regions()
{
    if (mem_regions_got)
    {
        return usable_mem_regions;
    }

    uint_8 usable_region_index;
    for (uint_8 i = 0; i < mem_region_count; i++)
    {
        mem_map_entry* mem_map = (mem_map_entry*)0x5000;
        mem_map += i;
        if (mem_map->region_type == 1)
        {
            usable_mem_regions[usable_region_index] = mem_map;
            usable_region_index++;
        }
    }

    usable_mem_region_count = usable_region_index;

    mem_regions_got = true;
    return usable_mem_regions;
}