#include "config.h"

kernel_config_t kernel_config;

void config_load() {
    kernel_config.vga_width         = 80;
    kernel_config.vga_height        = 25;
    kernel_config.vga_color         = 0x0F;
    kernel_config.vga_color_fail    = 0x04;
    kernel_config.vga_color_warning = 0x0E;
    kernel_config.vga_color_ok      = 0x02;
    kernel_config.kshell_buffer     = 256;
}