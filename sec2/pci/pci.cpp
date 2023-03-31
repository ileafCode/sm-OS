#pragma once

#include "../typedefs.h"
#include "../io/io.cpp"
#include "../stdio/text_print.cpp"
#include "../dynamic_mem/heap.cpp"
#include "../pit/pit.cpp"

struct __pci_driver;

typedef struct {
	uint_32 vendor;
	uint_32 device;
	uint_32 func;
	struct __pci_driver *driver;
} pci_device;

typedef struct {
	uint_32 vendor;
	uint_32 device;
	uint_32 func;
} pci_device_id;

typedef struct __pci_driver {
	pci_device_id *table;
	char *name;
	uint_8 (*init_one)(pci_device *);
	uint_8 (*init_driver)(void);
	uint_8 (*exit_driver)(void);
} pci_driver;

pci_device **pci_devices = 0;
uint_32 devs = 0;

pci_driver **pci_drivers = 0;
uint_32 drivs = 0;

void add_pci_device(pci_device *pdev)
{
	pci_devices[devs] = pdev;
	devs++;
	return;
}

uint_16 pci_read_word(uint_16 bus, uint_16 slot, uint_16 func, uint_16 offset)
{
	uint_64 address;
    uint_64 lbus = (uint_64)bus;
    uint_64 lslot = (uint_64)slot;
    uint_64 lfunc = (uint_64)func;
    uint_64 tmp = 0;
    address = (uint_64)((lbus << 16) | (lslot << 11) | (lfunc << 8) | (offset & 0xfc) | ((uint_32)0x80000000));
    outl(0xCF8, address);
    tmp = (uint_16)((inl(0xCFC) >> ((offset & 2) * 8)) & 0xFFFF);
    return (tmp);
}

uint_16 getVendorID(uint_16 bus, uint_16 device, uint_16 function)
{
    uint_32 r0 = pci_read_word(bus,device,function,0);
    return r0;
}

uint_16 getDeviceID(uint_16 bus, uint_16 device, uint_16 function)
{
    uint_32 r0 = pci_read_word(bus,device,function,2);
    return r0;
}

uint_16 getClassId(uint_16 bus, uint_16 device, uint_16 function)
{
    uint_32 r0 = pci_read_word(bus,device,function,0xA);
    return (r0 & ~0x00FF) >> 8;
}

uint_16 getSubClassId(uint_16 bus, uint_16 device, uint_16 function)
{
    uint_32 r0 = pci_read_word(bus,device,function,0xA);
    return (r0 & ~0xFF00);
}

void print_vendor_device(uint_16 vendor, uint_16 device)
{
    print_str("vendor: 0x");
    print_str(hex_str(vendor)); newl();

    print_str("device: 0x");
    print_str(hex_str(device)); newl();
}

void pci_probe()
{
	for(uint_32 bus = 0; bus < 256; bus++)
    {
        for(uint_32 slot = 0; slot < 32; slot++)
        {
            for(uint_32 function = 0; function < 8; function++)
            {
                uint_16 vendor = getVendorID(bus, slot, function);
                if(vendor == 0xffff) continue;
                uint_16 device = getDeviceID(bus, slot, function);
                if ((vendor != 0x0086 || vendor != 0x0034) && (device != 0x0000))
                {
                    print_vendor_device(vendor, device);
                    PIT::sleep_millis(600);
                }
                pci_device *pdev = (pci_device *)malloc(sizeof(pci_device));
                pdev->vendor = vendor;
                pdev->device = device;
                pdev->func = function;
                pdev->driver = 0;
                add_pci_device(pdev);
            }
        }
    }
}

uint_16 pciCheckVendor(uint_16 bus, uint_16 slot)
{
    uint_16 vendor, device;

    if ((vendor = pci_read_word(bus,slot,0,0)) != 0xFFFF)
    {
       device = pci_read_word(bus,slot,0,2);
    }
    return (vendor);
}

void init_pci()
{
	devs = drivs = 0;
	pci_devices = (pci_device **)malloc(32 * sizeof(pci_device));
	pci_drivers = (pci_driver **)malloc(32 * sizeof(pci_driver));
	pci_probe();
}

void pci_register_driver(pci_driver *driv)
{
	pci_drivers[drivs] = driv;
	drivs++;
	return;
}

void print_dump(uint_16 vendor, uint_16 device, uint_16 func, const char* name)
{
    print_str("vendor: 0x");
    print_str(hex_str(vendor)); newl();

    print_str("device: 0x");
    print_str(hex_str(device)); newl();

    print_str("func: 0x");
    print_str(hex_str(func)); newl();

    print_str("name: ");
    print_str(name); newl();
}

void pci_proc_dump(uint_8 *buffer)
{
	for(int i = 0; i < devs; i++)
	{
		pci_device *pci_dev = pci_devices[i];
		if(pci_dev->driver)
			print_dump(pci_dev->vendor, pci_dev->device, pci_dev->func, pci_dev->driver->name);
		else
			print_dump(pci_dev->vendor, pci_dev->device, pci_dev->func, "No name");
	}
}