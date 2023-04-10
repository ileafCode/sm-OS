# sm/OS

This is an OS that i'm working on for some education.

## Description

You can now type basic commands like "rand", "ping" and "pong".

## Getting Started

### Dependencies

* Install QEMU
```
sudo apt install qemu
```

* Install NASM
```
sudo apt install nasm
```

* Install GCC
```
sudo apt install gcc
```

### Build and run OS

* First of all, go into the build directory of the OS.

* Run this to build and start the OS.
```
./run.sh
```

## Version History

* 1.0
    * Initial Release
* 1.1
    * Support for real hardware
    * CMOS support
    * Random number generation
* 1.2
    * Upper and Lower string modification using string namespace
    * Scrolling screen
    * PCI (kind of)
    * Graphics (320x200, 256 colors)

## Pictures
<img src="v1.0.jpg" title="v1.0">
<img src="v1.2.jpg" title="v1.2">

## Sources

1. https://wiki.osdev.org - Helped me with basically everything.
2. https://www.youtube.com/watch?v=XKy1cE4EYMk - ADLIB programming.