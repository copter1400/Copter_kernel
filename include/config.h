#ifndef CONFIG_H
#define CONFIG_H

#include "type.h"

typedef struct {
    uint16_t vga_width;
    uint16_t vga_height;
    uint8_t vga_color;
} kernel_config_t;

extern kernel_config_t kernel_config;

void config_load();

#endif