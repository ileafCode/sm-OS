#include "kernel/kernel.cpp"

void _start()
{
    kernel::init_kernel();
    kernel::main_kernel();

    for (;;); // Endless loop.
}