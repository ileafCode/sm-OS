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

extern const char logo[]; // Logo comes from "logo.txt" in the build folder.
                          // You can thank the "binaries.asm" file :)

namespace kernel
{
    const char* kernel_version = "v1.2";

    void init_kernel()
    {
        stdio::clear_screen();
        stdio::set_cursor_pos(pos_coords(0, 0));
        stdio::print_str(logo);
        stdio::print_str("\n\nsm/OS "); stdio::print_str(kernel_version); stdio::newl();

        mem_map_entry** usable_mem_maps = get_usable_mem_regions();
        init_heap(0x100000, 0x100000);
    
        PIT::set_div(65535);
        PIT::init_pit();

        init_idt();
        main_kbd_handler = kbd_handler;
        init_pci();

        ata_lba_read();
    }

    void main_kernel()
    {
        stdio::print_ok("Boot successful");
        
        while (true)
        {
            stdio::print_str("shell > ");
            char* str1 = stdio::getstr();
            parse(str1);
            stdio::clear_input_buffer();
        }
    }
}