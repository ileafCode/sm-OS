#include "stdio/text_print.cpp"
#include "idt/idt.cpp"
#include "keyboard/keyboard_handler.cpp"
#include "mem_map/mem_map.cpp"
#include "dynamic_mem/heap.cpp"
#include "math_nums/math.cpp"
#include "pit/pit.cpp"
#include "sound/sound.cpp"
#include "stdio/input.cpp"
#include "gfx/gfx.cpp"
#include "gfx/font.h"

extern const char logo[];

void _start()
{
    clear_screen();
    set_cursor_pos(pos_coords(0, 0));
    print_str(logo);
    print_str("\n\nsm/OS v1.0\n");
    
    PIT::set_div(65535);
    PIT::init_pit();
    init_idt();

    main_kbd_handler = kbd_handler;

    mem_map_entry** usable_mem_maps = get_usable_mem_regions();

    init_heap(0x100000, 0x100000);
    
    //adlib::init();
    //startup_sound();

    uint_16 vga_g_width = 320;
    uint_16 vga_g_height = 200;

    //if (gfx::init_graph_vga(vga_g_width, vga_g_height, 1) == 0)
    //{
    //    print_str("Failed to init VGA");
    //    return;
    //}

    //gfx::clr_scr(0x00, vga_g_height, vga_g_height);

    //*(VGA_GMEM) = 0x3F;

    print_str("Hello\n");
    getch();

    return;
}