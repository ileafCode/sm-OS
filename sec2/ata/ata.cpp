#pragma once

#include "../typedefs.h"

extern "C" void ata_chs_read();
extern "C" void ata_lba_read();
extern "C" void ata_lba_write();

void ata_init()
{

}