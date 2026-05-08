#include "io.h"
#include "type.h"

//
//I/O to hardware
//

// input byte
uint8_t inb(uint16_t port) {
    uint8_t ret;
    __asm__ volatile ("inb %1, %0" // read from [port] into [ret]
                    : "=a"(ret)    // CPU register AL get result
                    : "Nd"(port)   // Store result in ret
    );
    return ret;
}

// output byte
void outb(uint16_t port, uint8_t val) {
    __asm__ volatile ("outb %0, %1" // read from [port] into [ret]
                    :
                    : "a"(val), "Nd"(port)   // Store result in ret
    );
}