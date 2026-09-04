#ifndef LEGACYOS_ATA_H
#define LEGACYOS_ATA_H

#include <stdint.h>

#define ATA_PRIMARY_BUS_BASE 0x1F0
#define ATA_SECONDARY_BUS_BASE 0x170

// Register sizes are written with (LBA28size/LBA48size) notation

#define ATA_DATA_REGISTER 0 // 16-bit/16-bit
#define ATA_ERROR_REGISTER 1 // 8-bit/16-bit
#define ATA_FEATURES_REGISTER 1 // 8-bit/16-bit
#define ATA_SECTOR_COUNT_REGISTER 2 // 8-bit/16-bit
#define ATA_SECTOR_NUMBER_REGISTER 3 // 8-bit/16-bit
#define ATA_CYLINDER_LOW_REGISTER 4 // 8-bit/16-bit
#define ATA_CYLINDER_HIGH_REGISTER 5 // 8-bit/16-bit
#define ATA_HEAD_REGISTER 6 // 8-bit/8-bit
#define ATA_STATUS_REGISTER 7 // 8-bit/8-bit
#define ATA_COMMAND_REGISTER 7 // 8-bit/8-bit

#define ATA_PRIMARY_DEVICE_CONTROL_REGISTER 0x3F6
#define ATA_SECONDARY_DEVICE_CONTROL_REGISTER 0x376

#define ATA_ALTERNATE_STATUS_REGISTER 0
#define ATA_DEVICE_CONTROL_REGISTER 0
#define ATA_DRIVE_ADDRESS_REGISTER 1

#define ATA_ERROR_AMNF_BIT 0 // Address Mark Not Found
#define ATA_ERROR_TKZNF_BIT 1 // TracK Zero Not Found
#define ATA_ERROR_ABRT_BIT 2 // ABoRTed command
#define ATA_ERROR_MCR_BIT 3 // Media Change Request
#define ATA_ERROR_IDNF_BIT 4 // ID Not Found
#define ATA_ERROR_MC_BIT 5 // Media Changed
#define ATA_ERROR_UNC_BIT 6 // UNCorrectable data error
#define ATA_ERROR_BBK_BIT 7 // Bad BlocK detected

#define ATA_HEAD_ADDRESS_BIT_BASE 0 // Bits 0-3 of the head in CHS, bits 24-27 of the block in LBA
#define ATA_HEAD_DRIVE_NUMBER_BIT 4
#define ATA_HEAD_ALWAYS_ONE_BIT 5,7 // These bits are always 1
#define ATA_HEAD_USE_LBA_BIT 6 // Clear = CHS addressing, Set = LBA addressing

#define ATA_STATUS_ERR_BIT 0 // ERRor occurred
#define ATA_STATUS_IDX_BIT 1 // InDeX bit, always 0
#define ATA_STATUS_CORR_BIT 2 // CORRected data, always 0
#define ATA_STATUS_DRQ_BIT 3 // set when the drive is about to send PIO data or when it can accept PIO data
#define ATA_STATUS_SRV_BIT 4 // overlapped mode SeRVice request
#define ATA_STATUS_DF_BIT 5 // Drive Fault (ERR doesn't get set with this)
#define ATA_STATUS_RDY_BIT 6 // drive is ReaDY, clear if errored or spun down
#define ATA_STATUS_BSY_BIT 7 // drive is BuSY. If it never clears, do a reset

#define ATA_DEVICE_CONTROL_FIRST_BIT 0 // This bit is always zero
#define ATA_DEVICE_CONTROL_NIEN_BIT 1 // Set to stop interrupts
#define ATA_DEVICE_CONTROL_SRST_BIT 2 // Set and clear after 5 microseconds to reset the ATA drives of the bus
#define ATA_DEVICE_CONTROL_RESERVED_BIT 3,4,5,6 // These bits are reserved
#define ATA_DEVICE_CONTROL_HOB_BIT 7 // set to read back the High Order Byte of the last LBA48 value sent to an I/O port

#define ATA_DRIVE_ADDRESS_DS0_BIT 0 // Drive Select 0. Clear when drive 0 is selected
#define ATA_DRIVE_ADDRESS_DS1_BIT 1 // Drive Select 1. Clear when drive 1 is selected
#define ATA_DRIVE_ADDRESS_HS0_BIT 2 // Selected Head's one's complement representation (bits 2-5)
#define ATA_DRIVE_ADDRESS_WTG_BIT 6 // WriTe Gate. Clear while the drive is writing
#define ATA_DRIVE_ADDRESS_RESERVED_BIT 7 // Reserved for floppy drive compatibility

#define ATA_REGISTER(x,y) (x + y)

uint8_t ATAGetPresentDrives();
void ATAInit();
uint8_t ATAReadCurrentDriveStatus(uint16_t bus);

#endif //LEGACYOS_ATA_H