#ifndef CONFIG_H
#define CONFIG_H

#include "type.h"

typedef struct {
    uint16_t vga_width;
    uint16_t vga_height;
    uint8_t  vga_color;
    uint8_t  vga_color_fail;
    uint8_t  vga_color_warning;
    uint8_t  vga_color_ok;
    uint16_t kshell_buffer;
    uint16_t password;
    bool     debug;
    uint16_t timer_hz;

} kernel_config_t;

extern kernel_config_t kernel_config;

void config_load();

#endif