#include "../../kernel/include/kernel/ata.h"
#include <stdio.h>

extern void writePort(unsigned short port, unsigned char data);
extern unsigned int readPort(unsigned short port);

// Internal variable to keep track which drives are present on which bus.
// Bit 0: Secondary bus, slave drive
// Bit 1: Secondary bus, master drive
// Bit 2: Primary bus, slave drive
// Bit 3: Primary bus, master drive
uint8_t presentATADrives = 0;

uint8_t ATAGetPresentDrives() {
	return presentATADrives;
}

void ATAInit() {
	uint8_t primaryBusCurrentStatus = ATAReadCurrentDriveStatus(ATA_PRIMARY_BUS_BASE);
	if (primaryBusCurrentStatus == 0xFF) {
		printf("There are no drives in the primary ATA bus.\n");
	}
	else {
		printf("Detected drives in the primary ATA bus.\n");
	}

	uint8_t secondaryBusCurrentStatus = ATAReadCurrentDriveStatus(ATA_SECONDARY_BUS_BASE);
	if (secondaryBusCurrentStatus == 0xFF) {
		printf("There are no drives in the secondary ATA bus.\n");
	}
	else {
		printf("Detected drives in the secondary ATA bus.\n");
	}

	uint8_t driveRegister = readPort(ATA_REGISTER(ATA_PRIMARY_BUS_BASE, ATA_HEAD_REGISTER));
	// Clear bit 4 to select drive 0
	driveRegister = driveRegister & 0xEF;
	writePort(ATA_REGISTER(ATA_PRIMARY_BUS_BASE, ATA_HEAD_REGISTER), 0xA0);

	// Same for secondary bus
	driveRegister = readPort(ATA_REGISTER(ATA_SECONDARY_BUS_BASE, ATA_HEAD_REGISTER));
	driveRegister = driveRegister & 0xEF;
	writePort(ATA_REGISTER(ATA_SECONDARY_BUS_BASE, ATA_HEAD_REGISTER), 0xA0);

	uint8_t canSelectPM = 0, canSelectPS = 0, canSelectSM = 0, canSelectSS = 0;

	// Delay to make sure the correct values are pushed to the bus
	for (uint16_t i = 0; i < 15; ++i) {
		ATAReadCurrentDriveStatus(ATA_PRIMARY_BUS_BASE);
	}

	// If the bit is clear, drive 0 (master) is selected
	uint8_t headRegister = readPort(ATA_REGISTER(ATA_PRIMARY_DEVICE_CONTROL_REGISTER, ATA_DRIVE_ADDRESS_REGISTER));
	if (!(headRegister & 0b00000001)) {
		canSelectPM = 1;
		presentATADrives |= 1 << 3;
	}

	headRegister = readPort(ATA_REGISTER(ATA_SECONDARY_DEVICE_CONTROL_REGISTER, ATA_DRIVE_ADDRESS_REGISTER));
	if (!(headRegister & 0b00000001)) {
		canSelectSM = 1;
		presentATADrives |= 1 << 1;
	}

	driveRegister = readPort(ATA_REGISTER(ATA_PRIMARY_BUS_BASE, ATA_HEAD_REGISTER));
	// Set bit 4 to select drive 1
	driveRegister = driveRegister |  (1 << ATA_HEAD_DRIVE_NUMBER_BIT);
	writePort(ATA_REGISTER(ATA_PRIMARY_BUS_BASE, ATA_HEAD_REGISTER), 0xB0);

	// Same for secondary bus
	driveRegister = readPort(ATA_REGISTER(ATA_SECONDARY_BUS_BASE, ATA_HEAD_REGISTER));
	driveRegister = driveRegister | (1 << ATA_HEAD_DRIVE_NUMBER_BIT);
	writePort(ATA_REGISTER(ATA_SECONDARY_BUS_BASE, ATA_HEAD_REGISTER),0xB0);

	// Delay to make sure the correct values are pushed to the bus
	for (uint16_t i = 0; i < 15; ++i) {
		ATAReadCurrentDriveStatus(ATA_PRIMARY_BUS_BASE);
	}

	// If the bit is set, drive 1 (slave) is selected
	headRegister = readPort(ATA_REGISTER(ATA_PRIMARY_DEVICE_CONTROL_REGISTER, ATA_DRIVE_ADDRESS_REGISTER));
	if ((headRegister & 0b00000001)) {
		canSelectPS = 1;
		presentATADrives |= 1 << 2;
	}

	headRegister = readPort(ATA_REGISTER(ATA_SECONDARY_DEVICE_CONTROL_REGISTER, ATA_DRIVE_ADDRESS_REGISTER));
	if ((headRegister & 0b00000001)) {
		canSelectSS = 1;
		presentATADrives |= 1;
	}

	if (canSelectPM) {
		printf("Detected master drive in primary bus.\n");
	}
	if (canSelectSM) {
		printf("Detected master drive in secondary bus.\n");
	}
	if (canSelectPS) {
		printf("Detected slave drive in primary bus.\n");
	}
	if (canSelectSS) {
		printf("Detected slave drive in secondary bus.\n");
	}
}

uint8_t ATAReadCurrentDriveStatus(uint16_t bus) {
	return (uint8_t) readPort(ATA_REGISTER(bus, ATA_STATUS_REGISTER));
}
