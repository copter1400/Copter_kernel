#include "disk.h"
#include "io.h"
#include "type.h"

// IO port
#define ATA_REG_DATA         0x1F0
#define ATA_REG_ERROR        0x1F1
#define ATA_REG_FEATURES     0x1F1
#define ATA_REG_SECCOUNT     0x1F2
#define ATA_REG_LBA0         0x1F3
#define ATA_REG_LBA1         0x1F4
#define ATA_REG_LBA2         0x1F5
#define ATA_REG_DEVICE       0x1F6
#define ATA_REG_STATUS       0x1F7
#define ATA_REG_COMMAND      0x1F7
#define ATA_REG_ALTSTATUS    0x3F6
#define ATA_REG_CONTROL      0x3F6

// status bits
#define ATA_SR_BSY 0x80
#define ATA_SR_DRQ 0x08
#define ATA_SR_ERR 0x01

void wait_BSY() {
    while (inb(ATA_REG_STATUS) & ATA_SR_BSY) { }
}

// result 0 : success / 1 : no ATA device / 2 : command fail
void ata_identify(uint16_t* identify, int* result) {
    // Wait until device is ready
    wait_BSY();

    // Select MASTER device
    outb(ATA_REG_DEVICE, 0xA0);

    // Clear all IDE register
    outb(ATA_REG_SECCOUNT, 0);
    outb(ATA_REG_LBA0, 0);
    outb(ATA_REG_LBA1, 0);
    outb(ATA_REG_LBA2, 0);

    // Send identify command
    outb(ATA_REG_COMMAND, 0xEC);

    // Read status
    uint8_t status = inb(ATA_REG_STATUS);

    if (status == 0) {
        // No ATA device present
        result = 1;
        return;
    }

    // Wait for BSY to clear and IDE to Data request
    do {
        status = inb(ATA_REG_STATUS);

        if (status & ATA_SR_ERR) {
            // Command failed
            result = 2;
            return;
        }
    } while ((status & ATA_SR_BSY) || !(status & ATA_SR_DRQ));

    // Read output of indentify device
    insw(ATA_REG_DATA, identify, 256);

    // success
    result = 0;
}