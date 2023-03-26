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

uint_8 inb(uint_16 port)
{
    uint_8 ret_val;
    asm volatile ("inb %1, %0"
    : "=a"(ret_val)
    : "Nd"(port));
    return ret_val;
}

void io_wait()
{
    asm volatile (
        "outb %%al, $0x80" : : "a"(0)
    );
}

void sti() { asm volatile("sti"); } // Enable interrupts
void cli() { asm volatile("cli"); } // Clear interrupts

void halt() { asm volatile("hlt"); } // Clear interrupts

void remap_pic()
{
    uint_8 a1, a2;
    a1 = inb(PIC1_DATA);
    a2 = inb(PIC2_DATA);

    outb(PIC1_CMD, ICW1_INIT | ICW1_ICW4);
    outb(PIC2_CMD, ICW1_INIT | ICW1_ICW4);

    outb(PIC1_DATA, 0);
    outb(PIC2_DATA, 8);

    outb(PIC1_DATA, 4);
    outb(PIC2_DATA, 2);

    outb(PIC1_DATA, ICW4_8086);
    outb(PIC2_DATA, ICW4_8086);

    outb(PIC1_DATA, a1);
    outb(PIC2_DATA, a2);
}