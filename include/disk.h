#ifndef DISK
#define DISK

#include "type.h"

void wait_BSY();
void ata_identify(uint16_t* identify, int* result);

#endif