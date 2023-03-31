nasm ../sec1/bootloader.asm -f bin -o bootloader.bin
nasm ../sec2/asm/ext_prg.asm -f elf64 -o ext_prg.o
nasm ../sec2/asm/binaries.asm -f elf64 -o bins.o
gcc -Ttext 0x8000 -ffreestanding -mno-red-zone -m64 -c "../sec2/main.cpp" -o "kernel.o"
ld -T "link.ld"
cat bootloader.bin kernel.bin > system.bin
qemu-system-x86_64 -soundhw pcspk -drive file=system.bin,format=raw,index=0,media=disk