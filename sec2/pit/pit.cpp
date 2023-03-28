#pragma once

#include "../io/io.cpp"
#include "../typedefs.h"

#define PIT_CHANNEL_0_DATA_PORT 0x40
#define PIT_CHANNEL_1_DATA_PORT 0x41
#define PIT_CHANNEL_2_DATA_PORT 0x42
#define PIT_MODE_CMD_REG        0x43

#define PIT_BINARY_MODE         0
#define PIT_OP_MODE_2           1 << 2
#define PIT_ACCESS_MODE_LO_HI   3 << 4
#define PIT_ACCESS_MODE_LO      2 << 4

namespace PIT
{
    double time_since_boot = 0;
    const uint_64 base_freq = 1193182;
    uint_16 div = 65535;

    void set_div(uint_16 divisor)
    {
        if (divisor < 100) divisor = 100;
        div = divisor;
        outb(0x40, (uint_8)(divisor & 0x00FF));
        io_wait();
        outb(0x40, (uint_8)((divisor & 0xFF00) >> 8));
    }

    uint_64 get_freq()
    {
        return base_freq / div;
    }

    void set_freq(uint_64 freq)
    {
        set_div(base_freq / freq);
    }

    void tick()
    {
        time_since_boot += 1 / (double)get_freq();
    }

    void sleepd(double secs)
    {
        double start_time = time_since_boot;
        while (time_since_boot < start_time + secs) halt();
    }

    void sleep_millis(uint_64 millis)
    {
        sleepd((double)millis / 1000);
    }

    void init_pit()
    {
        outb(PIT_MODE_CMD_REG, (uint_8) (PIT_ACCESS_MODE_LO_HI | PIT_OP_MODE_2 | PIT_BINARY_MODE));

        outb(PIT_CHANNEL_0_DATA_PORT, (uint_8) (div & 0xFF));
        outb(PIT_CHANNEL_0_DATA_PORT, (uint_8) ((div & 0xFF00) >> 8));
        //print_str("Initialized PIT\n");
    }
}
// namespace PIT