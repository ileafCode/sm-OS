#include "stdio/text_print.cpp"
#include "idt/idt.cpp"
#include "keyboard/keyboard_handler.cpp"
#include "mem_map/mem_map.cpp"
#include "dynamic_mem/heap.cpp"
#include "math_nums/math.cpp"
#include "pit/pit.cpp"
#include "sound/sound.cpp"
#include "stdio/input.cpp"
#include "etc/string.cpp"
#include "rtc_cmos/cmos.cpp"

extern const char logo[];

void drawVGA();

void _start()
{
    clear_screen();
    set_cursor_pos(pos_coords(0, 0));
    print_str(logo);
    print_str("\n\nsm/OS v1.0\n");
    
    PIT::set_div(65535);
    PIT::init_pit();
    init_idt();

    struct timeval t;

    main_kbd_handler = kbd_handler;

    mem_map_entry** usable_mem_maps = get_usable_mem_regions();

    init_heap(0x100000, 0x100000);
    
    //adlib::init();
    //startup_sound();

    while (true)
    {
        const char* str1 = getstr();

        if (str_cmp(str1, "ping"))
        {
           print_str("Pong!\n");
        }
        else if (str_cmp(str1, "pong"))
        {
           print_str("Ping!\n");
        }
        else if (str_cmp(str1, "rand"))
        {
            math::srand(cmos::now());
            print_str(int_str(math::rand() % 100)); newl();
        }
        else
        {
            print_str("?\n");
        }

        clear_input_buffer();
    }

    for (;;); // Endless loop.
}