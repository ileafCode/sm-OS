#pragma once

#include "../typedefs.h"
#include "../io/io.cpp"
#include "../dynamic_mem/heap.cpp"
#include "../stdio/text_print.cpp"
#include "../pit/pit.cpp"
#include "../kernel/kernel_func.cpp"

#define STATUS_BSY 0x80
#define STATUS_RDY 0x40
#define STATUS_DRQ 0x08
#define STATUS_DF  0x20
#define STATUS_ERR 0x01

static void ATA_wait_BSY();
static void ATA_wait_DRQ();
void read_sectors_ATA_PIO(uint_8* target_address, uint_32 LBA, uint_8 sector_count)
{
	ATA_wait_BSY();
	outb(0x1F6,0xE0 | ((LBA >> 24) & 0xF));
	outb(0x1F2,sector_count);
	outb(0x1F3, (uint_8) LBA);
	outb(0x1F4, (uint_8)(LBA >> 8));
	outb(0x1F5, (uint_8)(LBA >> 16)); 
	outb(0x1F7,0x20);

	for (int j = 0;j < sector_count; j++)
	{
		ATA_wait_BSY();
		ATA_wait_DRQ();
		for(int i=0;i<256;i++) target_address[i] = inw(0x1F0);
		target_address+=256;
	}
}

void write_sectors_ATA_PIO(uint_32 LBA, uint_8 sector_count, uint_32* bytes)
{
	ATA_wait_BSY();
	outb(0x1F6,0xE0 | ((LBA >>24) & 0xF));
	outb(0x1F2,sector_count);
	outb(0x1F3, (uint_8) LBA);
	outb(0x1F4, (uint_8)(LBA >> 8));
	outb(0x1F5, (uint_8)(LBA >> 16)); 
	outb(0x1F7,0x30);

	for (int j = 0; j < sector_count; j++)
	{
		ATA_wait_BSY();
		ATA_wait_DRQ();
		for(int i=0;i<256;i++)
		{
			outl(0x1F0, bytes[i]);
		}
	}
}

static void ATA_wait_BSY()
{
	while(inb(0x1F7)&STATUS_BSY);
}

static void ATA_wait_DRQ()
{
	while(!(inb(0x1F7)&STATUS_RDY));
}