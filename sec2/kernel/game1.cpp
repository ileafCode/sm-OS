#pragma once

#include "../typedefs.h"
#include "../stdio/input.cpp"
#include "../gfx_13h/gfx.cpp"
#include "../gfx_13h/icons.h"
#include "../stdio/text_print.cpp"
#include "../pit/pit.cpp"
#include "../math_nums/math.cpp"
#include "../rtc_cmos/cmos.cpp"

namespace game1 {
int  player_x = 0;
int  player_y = 0;

int  coin_x = 10;
int  coin_y = 5;
bool coin_collected = false;
int  score = 0;

int speed = 1;

uint_8 map[] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};

void get_keyboard()
{
    if (last_ch == 'w' || last_ch == 'W') player_y -= speed;
    if (last_ch == 'a' || last_ch == 'A') player_x -= speed;
    if (last_ch == 's' || last_ch == 'S') player_y += speed;
    if (last_ch == 'd' || last_ch == 'D') player_x += speed;
    last_ch = 0;
    PIT::sleep_millis(2);
}

void draw_map()
{
    for (int yy = 0; yy < 11 * TILE_SIZE; yy += TILE_SIZE)
    {
        for (int xx = 0; xx < 20 * TILE_SIZE; xx += TILE_SIZE)
        {
            if (map[(xx / TILE_SIZE) + (yy / TILE_SIZE)] == 0) gfx::tile(xx, yy, tile1);
        }
    }
}

void draw_coin(uint_16 x, uint_16 y)
{
    gfx::square(x * TILE_SIZE + 4, y * TILE_SIZE + 4, 8, 8, 0x0E);
}

void draw()
{
    draw_map();
    draw_coin(coin_x, coin_y);
    gfx::square(player_x * TILE_SIZE, player_y * TILE_SIZE, 16, 16, 0x0F);
    gfx::print_str_xy("Score: ", 10, 184, 0x0F); gfx::print_str_xy(int_str(score), 50, 184, 0x0F);
}

void logic()
{
    if (player_x == coin_x && player_y == coin_y) coin_collected = true;

    if (coin_collected)
    {
        math::srand(cmos::now());
        coin_x = math::rand() % 19;
        coin_y = math::rand() % 9;
        coin_collected = false;
        score++;
    }

    if (player_x >= 20) player_x = 19;
    if (player_y >= 10) player_y = 10;

    if (player_x < 0) player_x = 0;
    if (player_y < 0) player_y = 0;
}

void loop()
{
    kbd_en = true;
    print_kbd = false;

    while (true)
    {
        if (esc_pressed) // Reset everything.
        {
            player_x = 0;
            player_y = 0;

            coin_x = 10;
            coin_y = 5;
            coin_collected = false;
            score = 0;

            esc_pressed = false;
            kbd_en = false;
            print_kbd = true;
            return;
        }
        gfx::clear_screen();
        get_keyboard();
        logic();
        draw();
        gfx::flip();
    }
}
} // namespace game1