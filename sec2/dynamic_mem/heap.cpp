#pragma once
#include "../typedefs.h"
#include "../print/text_print.cpp"
#include "mem.cpp"

struct mem_seg_header
{
    uint_64 mem_l;
    mem_seg_header* next_seg;
    mem_seg_header* prev_seg;
    mem_seg_header* next_f_seg;
    mem_seg_header* prev_f_seg;
    bool free;
};

struct aligned_mem_seg_header
{
    uint_64 mem_seg_header_addr : 63;
    bool is_aligned : 1;
};

mem_seg_header* first_f_mem_seg;

void init_heap(uint_64 heap_addr, uint_64 heap_len)
{
    first_f_mem_seg = (mem_seg_header*)heap_addr;
    first_f_mem_seg->mem_l = heap_len - sizeof(mem_seg_header);
    first_f_mem_seg->next_seg   = 0;
    first_f_mem_seg->prev_seg   = 0;
    first_f_mem_seg->next_f_seg = 0;
    first_f_mem_seg->prev_f_seg = 0;
    first_f_mem_seg->free = true;
    print_str("Initialized heap\n");
}

void* malloc(uint_64 size)
{
    uint_64 rem = size % 8;
    size -= rem;

    if (rem != 0) size += 8;

    mem_seg_header* current_mem_seg = first_f_mem_seg;
    
    while (true)
    {
        if (current_mem_seg->mem_l >= size)
        {
            if (current_mem_seg->mem_l > size + sizeof(mem_seg_header))
            {
                mem_seg_header* new_seg_header = (mem_seg_header*)((uint_64)current_mem_seg +
                                                  sizeof(mem_seg_header) + size);
                
                new_seg_header->free       = true;
                new_seg_header->mem_l      = ((uint_64)current_mem_seg->mem_l) - (sizeof(mem_seg_header) + size);
                new_seg_header->next_f_seg = current_mem_seg->next_f_seg;
                new_seg_header->next_seg   = current_mem_seg->next_seg;
                new_seg_header->prev_seg   = current_mem_seg;
                new_seg_header->prev_f_seg = current_mem_seg->prev_f_seg;

                current_mem_seg->next_f_seg = new_seg_header;
                current_mem_seg->next_seg   = new_seg_header;
                current_mem_seg->mem_l = size;
            }

            if (current_mem_seg == first_f_mem_seg)
            {
                first_f_mem_seg = current_mem_seg->next_f_seg;
            }

            current_mem_seg->free = false;

            if (current_mem_seg->prev_f_seg != 0)
                current_mem_seg->prev_f_seg->next_f_seg = current_mem_seg->next_f_seg;
            if (current_mem_seg->next_f_seg != 0)
                current_mem_seg->next_f_seg->prev_f_seg = current_mem_seg->prev_f_seg;
            
            if (current_mem_seg->prev_seg != 0)
                current_mem_seg->prev_seg->next_f_seg = current_mem_seg->next_f_seg;
            if (current_mem_seg->next_seg != 0)
                current_mem_seg->next_seg->prev_f_seg = current_mem_seg->prev_f_seg;

            return current_mem_seg + 1;
        }

        if (current_mem_seg->next_f_seg == 0)
        {
            return 0;
        }
        current_mem_seg = current_mem_seg->next_f_seg;
    }

    return 0;
}

void comb_free_segs(mem_seg_header* a, mem_seg_header* b)
{
    if (a == 0 || b == 0) return;

    if (a < b)
    {
        a->mem_l += b->mem_l + sizeof(mem_seg_header);
        a->next_seg = b->next_seg;
        a->next_f_seg = b->next_f_seg;
        b->next_seg->prev_seg = a;
        b->next_seg->prev_f_seg = a;
        b->next_f_seg->prev_f_seg = a;
    }
    else
    {
        b->mem_l += a->mem_l + sizeof(mem_seg_header);
        b->next_seg = a->next_seg;
        b->next_f_seg = a->next_f_seg;
        a->next_seg->prev_seg = b;
        a->next_seg->prev_f_seg = b;
        a->next_f_seg->prev_f_seg = b;
    }
}

void* calloc(uint_64 size)
{
    void* malloc_val = malloc(size);

    memset(malloc_val, 0, size);

    return malloc_val;
}

void* calloc(uint_64 num, uint_64 size)
{
    return calloc(num * size);
}

void free(void* addr)
{
    mem_seg_header* current_mem_seg;
    
    aligned_mem_seg_header* amsh = (aligned_mem_seg_header*)addr - 1;

    if (amsh->is_aligned) 
    { 
        current_mem_seg = (mem_seg_header*)(uint_64)amsh->mem_seg_header_addr; 
    }
    else 
    { 
        current_mem_seg = ((mem_seg_header*)addr) - 1; 
    }

    current_mem_seg->free = true;

    if (current_mem_seg < first_f_mem_seg)
        first_f_mem_seg = current_mem_seg;
    
    // Free
    if (current_mem_seg->next_f_seg != 0)
    {
        if (current_mem_seg->next_f_seg->prev_f_seg < current_mem_seg)
            current_mem_seg->next_f_seg->prev_f_seg = current_mem_seg;
    }

    if (current_mem_seg->prev_f_seg != 0)
    {
        if (current_mem_seg->prev_f_seg->next_f_seg < current_mem_seg)
            current_mem_seg->prev_f_seg->next_f_seg = current_mem_seg;
    }

    // Not free
    if (current_mem_seg->next_seg != 0)
    {
        current_mem_seg->next_seg->prev_seg = current_mem_seg;
        if (current_mem_seg->next_seg->free)
            comb_free_segs(current_mem_seg, current_mem_seg->next_seg);
    }

    if (current_mem_seg->prev_seg != 0)
    {
        current_mem_seg->prev_seg->next_seg = current_mem_seg;
        if (current_mem_seg->prev_seg->free)
            comb_free_segs(current_mem_seg, current_mem_seg->prev_seg);
    }
}

void* realloc(void* addr, uint_64 new_size)
{
    mem_seg_header* old_seg_header;
    
    aligned_mem_seg_header* amsh = (aligned_mem_seg_header*)addr - 1;

    if (amsh->is_aligned) 
    { 
        old_seg_header = (mem_seg_header*)(uint_64)amsh->mem_seg_header_addr; 
    }
    else 
    { 
        old_seg_header = ((mem_seg_header*)addr) - 1; 
    }
    
    uint_64 smaller_size = new_size;

    if (old_seg_header->mem_l < new_size) smaller_size = old_seg_header->mem_l;
    void* new_mem = malloc(new_size);
    memcpy(new_mem, addr, smaller_size);
    free(addr);
    return (new_mem);
}

void* aligned_alloc(uint_64 alignment, uint_64 size)
{
    uint_64 al_rem = alignment % 8;
    alignment -= al_rem;
    if (al_rem != 0) alignment += 8;

    uint_64 size_rem = size % 8;
    size -= size_rem;
    if (size_rem != 0) size += 8;

    uint_64 full_size = size + alignment;

    void* malloc_val = malloc(full_size);
    uint_64 addr = (uint_64)malloc_val;

    uint_64 rem = addr % alignment;
    addr -= rem;
    if (rem != 0)
    {
        addr += alignment;
        aligned_mem_seg_header* amsh = (aligned_mem_seg_header*)addr - 1;
        amsh->is_aligned = true;
        amsh->mem_seg_header_addr = (uint_64)malloc_val - sizeof(mem_seg_header);
    }

    return (void*)addr;
}