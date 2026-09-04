#ifndef LEGACYOS_PCI_H
#define LEGACYOS_PCI_H

#include <stdint.h>

#define PCI_CONFIG_ADDRESS_PORT 0xCF8
#define PCI_CONFIG_DATA_PORT 0xCFC

// IMPORTANT NOTE: "Bit 31 is an enable flag for determining when accesses to CONFIG_DATA should be translated to configuration cycles"
#define PCI_ENABLE_BIT 31
#define PCI_BUS_NUMBER_START_BIT 16
#define PCI_DEVICE_NUMBER_START_BIT 11
#define PCI_FUNCTION_NUMBER_START_BIT 8
#define PCI_REGISTER_OFFSET_START_BIT 0

uint32_t pciConfigReadRegister(uint8_t bus, uint8_t device, uint8_t function, uint8_t desiredRegister);
uint16_t pciGetVendorID(uint8_t bus, uint8_t device, uint8_t function);
uint16_t pciGetDeviceID(uint8_t bus, uint8_t device, uint8_t function);

#endif