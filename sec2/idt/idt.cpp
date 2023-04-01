#pragma once

#include "../typedefs.h"
#include "../stdio/text_print.cpp"
#include "../keyboard/kbd_sc_set_1.h"
#include "../keyboard/keyboard_handler.cpp"
#include "../pit/pit.cpp"

#define IDT_int_gate  0b10001110
#define IDT_call_gate 0b10001100
#define IDT_trap_gate 0b10001111

extern uint_64 isr0;
extern uint_64 isr1;

extern "C" void load_idt();

void (*main_kbd_handler)(uint_8 sc, uint_8 chr);

struct IDT_64
{
    uint_16 offset_low;
    uint_16 selector;
    uint_8 ist;
    uint_8 types_attr;
    uint_16 offset_mid;
    uint_32 offset_high;
    uint_32 zero;
};

void make_interrupt(uint_64 func, uint_8 type_attr, uint_8 num);

extern IDT_64 _idt[256];
void init_idt()
{
    // --- Interrupts --- //
    make_interrupt((uint_64)&isr0, IDT_int_gate, 0); // PIT interrupt
    make_interrupt((uint_64)&isr1, IDT_int_gate, 1); // Keyboard interrupt

    remap_pic();

    outb(PIC1_DATA, 0b11111100);
    outb(PIC2_DATA, 0b11111111);
    
    load_idt();
    
    //print_str("Initialized IDT\n");
}

void make_interrupt(uint_64 func, uint_8 type_attr, uint_8 num)
{
    _idt[num].types_attr = type_attr;
    _idt[num].zero = 0;
	_idt[num].offset_low  = (uint_16)(((uint_64)func & 0x000000000000ffff));
	_idt[num].offset_mid  = (uint_16)(((uint_64)func & 0x00000000ffff0000) >> 16);
	_idt[num].offset_high = (uint_32)(((uint_64)func & 0xffffffff00000000) >> 32);
	_idt[num].ist = 0;
	_idt[num].selector = 0x08;
}

extern "C" void isr0_handler()
{
    PIT::tick();
    pic_end_master();
}

extern "C" void isr1_handler()
{
    uint_8 sc = inb(0x60);
    uint_8 chr = 0;
    if (sc < 0x3A)
        if (left_shift_pressed || caps_on)
            chr = kb_s1::ScanCodeLookupTableUpper[sc];
        else
            chr = kb_s1::ScanCodeLookupTableLower[sc];

    if (main_kbd_handler != 0)
    {
        main_kbd_handler(sc, chr);
    }

    outb(0x20, 0x20);
    outb(0xa0, 0x20);
}