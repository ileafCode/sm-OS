#pragma once

#include "../typedefs.h"

#define PIC1_CMD 0x20
#define PIC1_DATA 0x21

#define PIC2_CMD 0xa0
#define PIC2_DATA 0xa1

#define ICW1_INIT 0x10
#define ICW1_ICW4 0x01

#define ICW4_8086 0x01

#define PIC_EOI 0x20

#define ICW1_ICW4_NEEDED 1
#define ICW1_CALLADDR_4 1 << 2

void outb(uint_16 port, uint_8 val)
{
    asm volatile ("outb %0, %1" : : "a"(val), "Nd"(port));
}

void outw(uint_16 port, uint_16 value)
{
    asm volatile ("outw %%ax,%%dx": :"dN"(port), "a"(value));
}

void outl(uint_16 port, uint_64 value)
{
    asm volatile ("outl %%eax,%%edx": :"dN"(port), "a"(value));
}

void outpsm(uint_16 port, uint_8* data, uint_64 size)
{
	asm volatile ("rep outsw" : "+S" (data), "+c" (size) : "d" (port));
}

uint_8 inb(uint_16 port)
{
    uint_8 ret_val;
    asm volatile ("inb %1, %0"
    : "=a"(ret_val)
    : "Nd"(port));
    return ret_val;
}

uint_16 inw(uint_16 port)
{
    uint_8 ret_val;
    asm volatile ("inw %%dx, %%ax"
    : "=a"(ret_val)
    : "Nd"(port));
    return ret_val;
}

uint_64 inl(uint_16 port)
{
    uint_8 ret_val;
    asm volatile ("inl %%edx, %%eax"
    : "=a"(ret_val)
    : "Nd"(port));
    return ret_val;
}

void inpsm(uint_16 port, uint_8* data, uint_64 size)
{
	asm volatile ("rep insw" : "+D" (data), "+c" (size) : "d" (port) : "memory");
}

void insl(unsigned reg, unsigned int *buffer, int quads)
{
    int index;
    for(index = 0; index < quads; index++)
    {
        buffer[index] = inl(reg);
    }
}

void io_wait()
{
    asm volatile ("outb %%al, $0x80" : : "a"(0));
}

void sti() { asm volatile("sti"); } // Enable interrupts
void cli() { asm volatile("cli"); } // Clear interrupts

void halt() { asm volatile("hlt"); } // Halt CPU

void remap_pic()
{
    uint_8 a1, a2;
    a1 = inb(PIC1_DATA); io_wait();
    a2 = inb(PIC2_DATA); io_wait();

    outb(PIC1_CMD, ICW1_INIT | ICW1_ICW4); io_wait();
    outb(PIC2_CMD, ICW1_INIT | ICW1_ICW4); io_wait();

    outb(PIC1_DATA, 0); io_wait();
    outb(PIC2_DATA, 8); io_wait();

    outb(PIC1_DATA, 4); io_wait();
    outb(PIC2_DATA, 2); io_wait();

    outb(PIC1_DATA, ICW4_8086); io_wait();
    outb(PIC2_DATA, ICW4_8086); io_wait();

    outb(PIC1_DATA, a1); io_wait();
    outb(PIC2_DATA, a2); io_wait();
}

void pic_end_master()
{
    outb(PIC1_CMD, PIC_EOI);
}

void pic_end_slave()
{
    outb(PIC2_CMD, PIC_EOI);
    outb(PIC1_CMD, PIC_EOI);
}