#include "config.h"

kernel_config_t kernel_config;

void config_load() {
    kernel_config.vga_width = 80;
    kernel_config.vga_height = 25;
    kernel_config.vga_color = 0x0F;
}