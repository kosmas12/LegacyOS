#include "shellcommands.h"
#include "../../include/kernel/pci.h"
#include <stdint.h>
#include <stdio.h>

void pciList() {
	for (uint8_t i = 0; i <= 255; ++i) {
		for (uint8_t j = 0; j < 32; ++j) {
			for (uint8_t k = 0; k < 8; ++k) {
				if (pciGetVendorID(i, j, k) != 0xFFFF) {
					printf("PCI vendor 0x%X and device 0x%X\n", pciGetVendorID(i, j, k), pciGetDeviceID(i, j, k));
				}
			}
		}
		if (i == 255) {
			break;
		}
	}
}