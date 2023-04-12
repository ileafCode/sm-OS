#pragma once

#include "../math_nums/math.cpp"
#include "../gfx_13h/gfx.cpp"
#include "../keyboard/keyboard_handler.cpp"
#include "../math_nums/math.cpp"

namespace game2
{
int mbs = 6;
int player_x = 6;
int player_y = 6;
int player_a = 0;
int player_fov = 60;
int player_half_fov;
int speed = 2;

#define mapX 16
#define mapY 16

uint_8 map[mapX][mapY] = {
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1},
    {1, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
};

void draw_map()
{
    for (int y = 0; y < 16 * mbs; y += mbs)
    {
        for (int x = 0; x < 16 * mbs; x += mbs)
        {
            if (map[y / mbs][x / mbs] == 1) gfx::square(x, y, mbs, mbs, 0x2E);
            else gfx::square(x, y, mbs, mbs, 0x00);
        }
    }
}

void draw_GUI()
{
    gfx::line(100, 0, 100, 199, 0x03);
    gfx::line(0, 100, 99,  100, 0x0C);
    gfx::square(104, 3, 212, 194, 0x00);
}

void draw_player()
{
    gfx::square(player_x, player_y, mbs, mbs, 0x0F);
}

void draw_minimap()
{
    draw_map();
    draw_player();
}

void cast_rays()
{
    int rayAngle = player_a - player_half_fov;

    for (int rayCount = 0; rayCount < 213; rayCount++)
    {
        // Ray data
        math::point ray;
        ray.x = player_x;
        ray.y = player_y;

        int rayCos = math::cos(math::deg_rad(rayAngle)) / 64;
        int raySin = math::sin(math::deg_rad(rayAngle)) / 64;

        int ray_x_f = math::floor(ray.x);
        int ray_y_f = math::floor(ray.y);
        
        int wall = 0;
        while(wall == 0)
        {
            ray.x += rayCos;
            ray.y += raySin;
            wall = map[ray_y_f][ray_x_f];
        }

        int distance = math::sqrt(math::pow(player_x - ray.x, 2) + math::pow(player_y - ray.y, 2));
        distance = distance * math::cos(math::deg_rad(rayAngle - player_a));
        int wallHeight = math::floor((320 / 2) / distance);

        gfx::line(rayCount, 0, rayCount, (200 / 2) - wallHeight, 0x01);
        gfx::line(rayCount, (200 / 2) - wallHeight, rayCount, (200 / 2) + wallHeight, 0x04);
        gfx::line(rayCount, (200 / 2) + wallHeight, rayCount, 200, 0x02);

        rayAngle += (player_fov / 320);
    }
}

void draw()
{
    draw_minimap();
    draw_GUI();
    //cast_rays();
}

void logic()
{
    // --- Keyboard --- //
    if (last_ch == 'w' || last_ch == 'W')
    {
        int playerCos = math::cos(math::deg_rad(player_a)) * speed;
        int playerSin = math::sin(math::deg_rad(player_a)) * speed;
        int newX = player_x + playerCos;
        int newY = player_y + playerSin;

        int floor_newx = math::floor(newX);
        int floor_newy = math::floor(newY);

        if (map[floor_newy][floor_newx] == 0)
        {
            player_x = newX;
            player_y = newY;
        }
    }
    if (last_ch == 's' || last_ch == 'S')
    {
        int playerCos = math::cos(math::deg_rad(player_a)) * speed;
        int playerSin = math::sin(math::deg_rad(player_a)) * speed;
        int newX = player_x - playerCos;
        int newY = player_y - playerSin;

        int floor_newx = math::floor(newX);
        int floor_newy = math::floor(newY);

        if (map[floor_newy][floor_newx] == 0)
        {
            player_x = newX;
            player_y = newY;
        }
    }
    if (last_ch == 'a' || last_ch == 'A')
    {
        player_a += 5;
    }
    if (last_ch == 'd' || last_ch == 'D')
    {
        player_a += 5;
    }

    last_ch = 0;

    PIT::sleep_millis(2);

    // --- Player --- //
    if (player_x > 14 * mbs) player_x = 14 * mbs;
    if (player_y > 14 * mbs) player_y = 14 * mbs;
    if (player_x < 1  * mbs) player_x = 1  * mbs;
    if (player_y < 1  * mbs) player_y = 1  * mbs;
}

void loop()
{
    kbd_en = true;
    print_kbd = false;

    player_half_fov = player_fov / 2;

    while (true)
    {
        if (esc_pressed)
        {
            esc_pressed = false;
            kbd_en = false;
            print_kbd = true;
            return;
        }

        gfx::clear_screen();
        logic();
        draw();
        gfx::flip();
    }
}
}