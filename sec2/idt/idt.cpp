#pragma once

#include "../typedefs.h"
#include "../stdio/text_print.cpp"
#include "../keyboard/kbd_sc_set_1.h"
#include "../keyboard/keyboard_handler.cpp"
#include "../pit/pit.cpp"

void (*main_kbd_handler)(uint_8 sc, uint_8 chr);

#define IDT_int_gate  0b10001110
#define IDT_call_gate 0b10001100
#define IDT_trap_gate 0b10001111

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

IDT_64 _idt[256];
extern uint_64 isr0;
extern uint_64 isr1;

extern "C" void load_idt();

void init_idt()
{
    _idt[0].zero = 0;
	_idt[0].offset_low  = (uint_16)(((uint_64)&isr0 & 0x000000000000ffff));
	_idt[0].offset_mid  = (uint_16)(((uint_64)&isr0 & 0x00000000ffff0000) >> 16);
	_idt[0].offset_high = (uint_32)(((uint_64)&isr0 & 0xffffffff00000000) >> 32);
	_idt[0].ist = 0;
	_idt[0].selector = 0x08;
	_idt[0].types_attr = IDT_int_gate;

    _idt[1].zero = 0;
	_idt[1].offset_low  = (uint_16)(((uint_64)&isr1 & 0x000000000000ffff));
	_idt[1].offset_mid  = (uint_16)(((uint_64)&isr1 & 0x00000000ffff0000) >> 16);
	_idt[1].offset_high = (uint_32)(((uint_64)&isr1 & 0xffffffff00000000) >> 32);
	_idt[1].ist = 0;
	_idt[1].selector = 0x08;
	_idt[1].types_attr = IDT_int_gate;

    remap_pic();

    outb(PIC1_DATA, 0b11111100);
    outb(PIC2_DATA, 0b11111111);

    load_idt();
    
    //print_ok("Initialized IDT\n");
}

extern "C" void pit_handler()
{
    PIT::tick();
    pic_end_master();
}

extern "C" void kbd_handler()
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