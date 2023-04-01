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

extern const char logo[]; // Logo comes from "logo.txt" in the build folder.
                          // You can thank the "binaries.asm" file :)

namespace kernel
{
    void init_kernel()
    {
        cli();

        clear_screen();
        set_cursor_pos(pos_coords(0, 0));
        print_str(logo);
        print_str("\n\nsm/OS v1.2\n");

        mem_map_entry** usable_mem_maps = get_usable_mem_regions();
        init_heap(0x100000, 0x100000);
    
        PIT::set_div(65535);
        PIT::init_pit();

        init_idt();
        main_kbd_handler = kbd_handler;
        init_pci();

        sti();

        //print_str(int_str(1 / 0));
    }

    void main_kernel()
    {
        print_ok("Boot successful");
        //print_str(int_str(caps_on)); newl();
        //print_str(int_str(left_shift_pressed)); newl();
    
        while (true)
        {
            print_str("terminal > ");
            const char* str1 = getstr();
            str1 = string::str_lower((char*)str1);

            if (string::str_cmp(str1, "ping"))
            {
                print_str("Pong!\n");
            }
            else if (string::str_cmp(str1, "pong"))
            {
                print_str("Ping!\n");
            }
            else if (string::str_cmp(str1, "rand"))
            {
                math::srand(cmos::now());
                print_str(int_str(math::rand() % 100)); newl();
            }
            else if (string::str_cmp(str1, (const char*)0xA)) {} // If enter pressed without a command, do nothing.
            else
            {
                print_err("Unknown command: ", false); print_str(str1); newl();
            }

            clear_input_buffer();
        }
    }
}