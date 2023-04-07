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

extern const char logo[]; // Logo comes from "logo.txt" in the build folder.
                          // You can thank the "binaries.asm" file :)

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
        main_kbd_handler = kbd_handler;
        init_pci();
    }

    void main_kernel()
    {
        gfx::init_db((uint_8*) 0x100000);

        gfx::clear_screen();
        gfx::print_str("sm/OS "); gfx::print_str(kernel_version); gfx::newl();

        gfx::print_ok("Boot successful");
        
        // uint_8 *t;
        // char bwrite[512];

        // stdio::clear_screen();
        // stdio::set_cursor_pos(pos_coords(0, 0));

        // for (int i = 0; i < 512; i++) bwrite[i] = 10;
        // write_sectors_ATA_PIO(0x0, 1, (uint_32*)bwrite);
        // read_sectors_ATA_PIO(t, 0x0, 1);

        // for (int i = 0; i < 256; i++)
        // {
        //     stdio::print_str(hex_str(t[i])); stdio::print_str(" ");
        //     if (i % 16 == 15) if (i != 0) stdio::newl();
        // }

        // stdio::newl();
        
        while (true)
        {
            gfx::print_str("shell > ");
            char* str1 = stdio::getstr();
            parse(str1);
            stdio::clear_input_buffer();
        }
    }
}