#include "../../include/kernel/pci.h"
#include <stdint.h>

extern void writePort32(unsigned short port, unsigned int data);
extern unsigned int readPort32(unsigned short port);

uint32_t pciConfigReadRegister(uint8_t bus, uint8_t device, uint8_t function, uint8_t desiredRegister) {
    /* The layout of a configuration space address is as follows:
        Bits 0-7: Register*
        Bits 8-15: Function number
        Bits 16-23: Device number
        Bits 24-30: Reserved
        Bits 31: Enable flag

        *The register offset must always have the 2 least significant bits of the address as 0. This is because
        each configuration space access must be 32 bit-aligned (e.g. DWORD 0 would occupy addresses 00, 01, 10 and 11,
        DWORD 1 would occupy addresses 100, 101, 110 and 111 etc.).

    */

    uint32_t address = (uint32_t)((1 << PCI_ENABLE_BIT) | (bus << PCI_BUS_NUMBER_START_BIT) | (device << PCI_DEVICE_NUMBER_START_BIT) |
        (function << PCI_FUNCTION_NUMBER_START_BIT) | (desiredRegister * 0b100)); // Least significant 2 bits should always be 0

    // Send the address to the PCI controller
    writePort32(PCI_CONFIG_ADDRESS_PORT, address);

    // Read requested data
    uint32_t result = readPort32(PCI_CONFIG_DATA_PORT);

    return result;
}

uint16_t pciGetVendorID(uint8_t bus, uint8_t device, uint8_t function) {
    uint32_t fullDeviceID = pciConfigReadRegister(bus, device, function, 0);
    uint16_t vendorID = fullDeviceID & 0xFFFF;

    return vendorID;
}

uint16_t pciGetDeviceID(uint8_t bus, uint8_t device, uint8_t function) {
    uint32_t fullDeviceID = pciConfigReadRegister(bus, device, function, 0);
    uint16_t deviceID = (fullDeviceID >> 16) & 0xFFFF;

    return deviceID;
}
