#pragma once

#include "../stdio/text_print.cpp"
#include "../stdio/input.cpp"
#include "../mem_map/mem_map.cpp"
#include "../dynamic_mem/heap.cpp"
#include "../idt/idt.cpp"
#include "../keyboard/keyboard_handler.cpp"
#include "../math_nums/math.cpp"
#include "../pit/pit.cpp"
#include "../sound/sound.cpp"
#include "../etc/string.cpp"
#include "../rtc_cmos/cmos.cpp"
#include "../pci/pci.cpp"
#include "parser.cpp"
#include "../fs/fs.cpp"
#include "../ata/ata.cpp"
#include "../gfx_13h/gfx.cpp"
#include "../gfx_13h/icons.h"
#include "../acpi/acpi.cpp"

namespace kernel
{
    const char* kernel_version = "v1.2";

    void init_kernel()
    {
        init_heap(0x100000, 0x100000);
        mem_map_entry** usable_mem_maps = get_usable_mem_regions();
    
        PIT::set_div(65535);
        PIT::init_pit();

        init_idt();
        outb(PIC1_DATA, 0b11111000); // Bit masking, just in case!
        outb(PIC2_DATA, 0b11101111);

        main_kbd_handler = kbd_handler;
        //init_pci();
    }

    void main_kernel()
    {
        gfx::init_db((uint_8*) malloc(64000));

        gfx::clear_screen();
        gfx::print_str("sm/OS "); gfx::print_str(kernel_version); gfx::newl();

        gfx::print_ok("Boot successful");
        gfx::newl();
        
        while (true)
        {
            gfx::print_str("shell > ");
            char* str1 = stdio::getstr();
            parse(str1);
            stdio::clear_input_buffer();
            //gfx::clear_screen();
        }
    }
}