CC = gcc
AS = as
LD = ld

CFLAGS = -m32 -ffreestanding -fno-pic -fno-pie -Wall -Wextra -Iinclude -c
LDFLAGS = -m elf_i386 -T linker.ld

BUILD = build
SRC = src

# -------------------------
# AUTO SOURCES
# -------------------------
C_SOURCES = $(wildcard $(SRC)/*.c)
C_OBJECTS = $(patsubst $(SRC)/%.c,$(BUILD)/%.o,$(C_SOURCES))

ASM_SOURCES = $(wildcard $(SRC)/*.s)
ASM_OBJECTS = $(patsubst $(SRC)/%.s,$(BUILD)/%.o,$(ASM_SOURCES))

# -------------------------
# DEFAULT
# -------------------------
all: $(BUILD)/kernel.elf

# -------------------------
# C COMPILATION
# -------------------------
$(BUILD)/%.o: $(SRC)/%.c
	$(CC) $(CFLAGS) $< -o $@

# -------------------------
# ASM COMPILATION (GAS)
# -------------------------
$(BUILD)/%.o: $(SRC)/%.s
	$(AS) --32 $< -o $@

# -------------------------
# LINK KERNEL
# -------------------------
$(BUILD)/kernel.elf: $(C_OBJECTS) $(ASM_OBJECTS)
	$(LD) $(LDFLAGS) -o $@ $^

# -------------------------
# ISO BUILD
# -------------------------
iso: $(BUILD)/kernel.elf
	cp $(BUILD)/kernel.elf iso/boot/kernel.elf
	grub-mkrescue -o os.iso iso

# -------------------------
# RUN
# -------------------------
run: iso
	qemu-system-i386 -serial stdio -m 512M -cdrom os.iso

# -------------------------
# CLEAN
# -------------------------
clean:
	rm -rf $(BUILD)/*.o $(BUILD)/*.elf os.iso
