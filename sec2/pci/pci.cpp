#pragma once

#include "../typedefs.h"
#include "../io/io.cpp"
#include "../gfx_13h/gfx.cpp"
#include "../dynamic_mem/heap.cpp"
#include "../pit/pit.cpp"

/**
 * Credits to: eduOS by szhou42 (github).
*/

#define PCI_CONFIG_ADDRESS       0xCF8
#define PCI_CONFIG_DATA          0xCFC

#define PCI_VENDOR_ID            0x00
#define PCI_DEVICE_ID            0x02
#define PCI_COMMAND              0x04
#define PCI_STATUS               0x06
#define PCI_REVISION_ID          0x08
#define PCI_PROG_IF              0x09
#define PCI_SUBCLASS             0x0a
#define PCI_CLASS                0x0b
#define PCI_CACHE_LINE_SIZE      0x0c
#define PCI_LATENCY_TIMER        0x0d
#define PCI_HEADER_TYPE          0x0e
#define PCI_BIST                 0x0f
#define PCI_BAR0                 0x10
#define PCI_BAR1                 0x14
#define PCI_BAR2                 0x18
#define PCI_BAR3                 0x1C
#define PCI_BAR4                 0x20
#define PCI_BAR5                 0x24
#define PCI_INTERRUPT_LINE       0x3C
#define PCI_SECONDARY_BUS        0x09

// Device type
#define PCI_HEADER_TYPE_DEVICE  0
#define PCI_HEADER_TYPE_BRIDGE  1
#define PCI_HEADER_TYPE_CARDBUS 2
#define PCI_TYPE_BRIDGE 0x0604
#define PCI_TYPE_SATA   0x0106
#define PCI_NONE 0xFFFF

#define DEVICE_PER_BUS           32
#define FUNCTION_PER_DEVICE      32

typedef union pci_dev {
    uint32_t bits;
    struct {
        uint32_t always_zero    : 2;
        uint32_t field_num      : 6;
        uint32_t function_num   : 3;
        uint32_t device_num     : 5;
        uint32_t bus_num        : 8;
        uint32_t reserved       : 7;
        uint32_t enable         : 1;
    };
} pci_dev_t;

uint32_t pci_size_map[100];
pci_dev_t dev_zero = { 0 };

uint32_t pci_read(pci_dev_t dev, uint32_t field)
{
	dev.field_num = (field & 0xFC) >> 2;
	dev.enable = 1;
	outl(PCI_CONFIG_ADDRESS, dev.bits);
	uint32_t size = pci_size_map[field];

	if(size == 1)
    {
		uint8_t t =inb(PCI_CONFIG_DATA + (field & 3));
		return t;
	}
	else if(size == 2)
    {
		uint16_t t = inw(PCI_CONFIG_DATA + (field & 2));
		return t;
	}
	else if(size == 4)
    {
		uint32_t t = inl(PCI_CONFIG_DATA);
		return t;
	}
	return 0xffff;
}


void pci_write(pci_dev_t dev, uint32_t field, uint32_t value)
{
	dev.field_num = (field & 0xFC) >> 2;
	dev.enable = 1;
	outl(PCI_CONFIG_ADDRESS, dev.bits);
	outl(PCI_CONFIG_DATA, value);
}

uint32_t get_device_type(pci_dev_t dev)
{
	uint32_t t = pci_read(dev, PCI_CLASS) << 8;
	return t | pci_read(dev, PCI_SUBCLASS);
}

uint32_t get_secondary_bus(pci_dev_t dev)
{
	return pci_read(dev, PCI_SECONDARY_BUS);
}

uint32_t pci_reach_end(pci_dev_t dev)
{
	uint32_t t = pci_read(dev, PCI_HEADER_TYPE);
	return !t;
}

pci_dev_t pci_scan_bus(uint16_t vendor_id, uint16_t device_id, uint32_t bus, int device_type);

pci_dev_t pci_scan_function(uint16_t vendor_id, uint16_t device_id, uint32_t bus, uint32_t device, uint32_t function, int device_type)
{
	pci_dev_t dev = {0};
	dev.bus_num = bus;
	dev.device_num = device;
	dev.function_num = function;
	if(get_device_type(dev) == PCI_TYPE_BRIDGE) pci_scan_bus(vendor_id, device_id, get_secondary_bus(dev), device_type);
	if(device_type == -1 || device_type == get_device_type(dev))
    {
		uint32_t devid  = pci_read(dev, PCI_DEVICE_ID);
		uint32_t vendid = pci_read(dev, PCI_VENDOR_ID);
		if(devid == device_id && vendor_id == vendid)
			return dev;
	}
	return dev_zero;
}

pci_dev_t pci_scan_device(uint16_t vendor_id, uint16_t device_id, uint32_t bus, uint32_t device, int device_type)
{
	pci_dev_t dev = {0};
	dev.bus_num = bus;
	dev.device_num = device;
	if(pci_read(dev,PCI_VENDOR_ID) == PCI_NONE) return dev_zero;
	pci_dev_t t = pci_scan_function(vendor_id, device_id, bus, device, 0, device_type);
	if(t.bits) return t;
	if(pci_reach_end(dev)) return dev_zero;

	for(int function = 1; function < FUNCTION_PER_DEVICE; function++)
    {
		if(pci_read(dev,PCI_VENDOR_ID) != PCI_NONE)
        {
			t = pci_scan_function(vendor_id, device_id, bus, device, function, device_type);
			if(t.bits) return t;
		}
	}
	return dev_zero;
}

pci_dev_t pci_scan_bus(uint16_t vendor_id, uint16_t device_id, uint32_t bus, int device_type)
{
	for(int device = 0; device < DEVICE_PER_BUS; device++)
    {
		pci_dev_t t = pci_scan_device(vendor_id, device_id, bus, device, device_type);
		if(t.bits) return t;
	}
	return dev_zero;
}

pci_dev_t pci_get_device(uint16_t vendor_id, uint16_t device_id, int device_type)
{
	pci_dev_t t = pci_scan_bus(vendor_id, device_id, 0, device_type);
	if(t.bits) return t;
	if(pci_reach_end(dev_zero)) gfx::print_str("PCI Get device failed...");
	for(int function = 1; function < FUNCTION_PER_DEVICE; function++)
    {
		pci_dev_t dev = {0};
		dev.function_num = function;

		if(pci_read(dev, PCI_VENDOR_ID) == PCI_NONE) break;
		t = pci_scan_bus(vendor_id, device_id, function, device_type);
		if(t.bits) return t;
	}
	return dev_zero;
}

void init_pci()
{
	pci_size_map[PCI_VENDOR_ID] = 2;
	pci_size_map[PCI_DEVICE_ID] = 2;
	pci_size_map[PCI_COMMAND] =	2;
	pci_size_map[PCI_STATUS] = 2;
	pci_size_map[PCI_SUBCLASS] = 1;
	pci_size_map[PCI_CLASS]	= 1;
	pci_size_map[PCI_CACHE_LINE_SIZE] = 1;
	pci_size_map[PCI_LATENCY_TIMER]	= 1;
	pci_size_map[PCI_HEADER_TYPE] = 1;
	pci_size_map[PCI_BIST] = 1;
	pci_size_map[PCI_BAR0] = 4;
	pci_size_map[PCI_BAR1] = 4;
	pci_size_map[PCI_BAR2] = 4;
	pci_size_map[PCI_BAR3] = 4;
	pci_size_map[PCI_BAR4] = 4;
	pci_size_map[PCI_BAR5] = 4;
	pci_size_map[PCI_INTERRUPT_LINE] = 1;
	pci_size_map[PCI_SECONDARY_BUS]	= 1;
}