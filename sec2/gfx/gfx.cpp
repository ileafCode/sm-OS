#pragma once
#include "../typedefs.h"
#include "../io/io.cpp"

#define SZ(x) (sizeof(x)/sizeof(x[0]))

#define VGA_GMEM (unsigned char*) 0xA0000

#define R_COM  0x63

#define R_W256 0x00
#define R_W320 0x00
#define R_W360 0x04
#define R_W376 0x04
#define R_W400 0x04

#define R_H200 0x00
#define R_H224 0x80
#define R_H240 0x80
#define R_H256 0x80
#define R_H270 0x80
#define R_H300 0x80
#define R_H360 0x00
#define R_H400 0x00
#define R_H480 0x80
#define R_H564 0x80
#define R_H600 0x80

namespace gfx
{
    static const uint_8 hor_regs [] = { 0x0,  0x1,  0x2,  0x3,  0x4, 0x5,  0x13 };

    static const uint_8 width_256[] = { 0x5f, 0x3f, 0x40, 0x82, 0x4a, 0x9a, 0x20 };
    static const uint_8 width_320[] = { 0x5f, 0x4f, 0x50, 0x82, 0x54, 0x80, 0x28 };
    static const uint_8 width_360[] = { 0x6b, 0x59, 0x5a, 0x8e, 0x5e, 0x8a, 0x2d };
    static const uint_8 width_376[] = { 0x6e, 0x5d, 0x5e, 0x91, 0x62, 0x8f, 0x2f };
    static const uint_8 width_400[] = { 0x70, 0x63, 0x64, 0x92, 0x65, 0x82, 0x32 };

    static const uint_8 ver_regs  [] = { 0x6,  0x7,  0x9,  0x10, 0x11, 0x12, 0x15, 0x16 };

    static const uint_8 height_200[] = { 0xbf, 0x1f, 0x41, 0x9c, 0x8e, 0x8f, 0x96, 0xb9 };
    static const uint_8 height_224[] = { 0x0b, 0x3e, 0x41, 0xda, 0x9c, 0xbf, 0xc7, 0x04 };
    static const uint_8 height_240[] = { 0x0d, 0x3e, 0x41, 0xea, 0xac, 0xdf, 0xe7, 0x06 };
    static const uint_8 height_256[] = { 0x23, 0xb2, 0x61, 0x0a, 0xac, 0xff, 0x07, 0x1a };
    static const uint_8 height_270[] = { 0x30, 0xf0, 0x61, 0x20, 0xa9, 0x1b, 0x1f, 0x2f };
    static const uint_8 height_300[] = { 0x70, 0xf0, 0x61, 0x5b, 0x8c, 0x57, 0x58, 0x70 };
    static const uint_8 height_360[] = { 0xbf, 0x1f, 0x40, 0x88, 0x85, 0x67, 0x6d, 0xba };
    static const uint_8 height_400[] = { 0xbf, 0x1f, 0x40, 0x9c, 0x8e, 0x8f, 0x96, 0xb9 };
    static const uint_8 height_480[] = { 0x0d, 0x3e, 0x40, 0xea, 0xac, 0xdf, 0xe7, 0x06 };
    static const uint_8 height_564[] = { 0x62, 0xf0, 0x60, 0x37, 0x89, 0x33, 0x3c, 0x5c };
    static const uint_8 height_600[] = { 0x70, 0xf0, 0x60, 0x5b, 0x8c, 0x57, 0x58, 0x70 };

    int init_graph_vga(int width, int height, int chain4)
    {
        const uint_8 *w, *h;
        uint_8 val;
        int a;

        switch(width)
        {
        case 256: w = width_256; val = R_COM+R_W256; break;
        case 320: w = width_320; val = R_COM+R_W320; break;
        case 360: w = width_360; val = R_COM+R_W360; break;
        case 376: w = width_376; val = R_COM+R_W376; break;
        case 400: w = width_400; val = R_COM+R_W400; break;
        default: return 0;
        }
        switch(height)
        {
        case 200: h = height_200; val |= R_H200; break;
        case 224: h = height_224; val |= R_H224; break;
        case 240: h = height_240; val |= R_H240; break;
        case 256: h = height_256; val |= R_H256; break;
        case 270: h = height_270; val |= R_H270; break;
        case 300: h = height_300; val |= R_H300; break;
        case 360: h = height_360; val |= R_H360; break;
        case 400: h = height_400; val |= R_H400; break;
        case 480: h = height_480; val |= R_H480; break;
        case 564: h = height_564; val |= R_H564; break;
        case 600: h = height_600; val |= R_H600; break;
        default: return 0;
        }

        if (chain4 && (long)width * (long) height > 65536L) return 0;

        outb(0x3c2, val);
        outw(0x3d4, 0x0e11);

        for (a = 0; a < SZ(hor_regs); ++a)
        {
            outw(0x3d4, (uint_16)((w[a] << 8) + hor_regs[a]));
        }
        
        for (a = 0; a < SZ(ver_regs); ++a)
        {
            outw(0x3d4, (uint_16)((h[a] << 8) + ver_regs[a]));
        }

        outw(0x3d4, 0x0008);

        if (chain4)
        {
            outw(0x3d4, 0x4014);
            outw(0x3d4, 0xa317);
            outw(0x3c4, 0x0e04);
        }
        else
        {
            outw(0x3d4,0x0014);
            outw(0x3d4,0xe317);
            outw(0x3c4,0x0604);
        }

        outw(0x3c4, 0x0101);
        outw(0x3c4, 0x0f02);
        outw(0x3ce, 0x4005);
        outw(0x3ce, 0x0506);

        outw(0x3d4,0x000C);
        outw(0x3d4,0x000D);

        inb(0x3da);
        outb(0x3c0, 0x30); outb(0x3c0, 0x41);
        outb(0x3c0, 0x33); outb(0x3c0, 0x00);

        for (a = 0; a < 16; a++)
        {
            outb(0x3c0,(uint_8)a);
            outb(0x3c0,(uint_8)a);
        }
   
        outb(0x3c0, 0x20);

        return 1;
    }

    void clr_scr(uint_8 color, uint_16 vga_g_width, uint_16 vga_g_height)
    {
        for (int i = 0; i < (vga_g_width * vga_g_height); i++)
        {
            *(VGA_GMEM + i) = color;
        }
    }
}