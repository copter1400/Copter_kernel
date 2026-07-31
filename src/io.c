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

// input word
uint16_t inw(uint16_t port) {
    uint16_t ret;
    __asm__ volatile ("inw %1, %0"
                    : "=a"(ret)
                    : "Nd"(port)
    );
    return ret;
}

// output word
void outw(uint16_t port, uint16_t val) {
    __asm__ volatile ("outw %0, %1"
                    :
                    : "a"(val), "Nd"(port)
    );
}

void insw(uint16_t port, void *buffer, uint32_t count) {
    uint16_t *buf = (uint16_t*)buffer;

    for (uint32_t i=0;i<count;i++) {
        *buf = inw(port);
        buf++;
    }
}