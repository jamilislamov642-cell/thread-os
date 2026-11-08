CC      := i686-linux-gnu-gcc
LD      := i686-linux-gnu-ld
OBJCOPY := i686-linux-gnu-objcopy
NASM    := nasm

CFLAGS  := -m32 -ffreestanding -fno-builtin -fno-stack-protector -nostdlib \
           -nodefaultlibs -Wall -Wextra -O2 -Ikernel

DIST    := dist
BUILD   := build

C_SRCS  := $(wildcard kernel/*.c)
C_OBJS  := $(patsubst kernel/%.c,$(BUILD)/%.o,$(C_SRCS))

ASM_SRCS := kernel/entry.asm kernel/isr.asm kernel/context.asm
ASM_OBJS := $(BUILD)/entry.o $(BUILD)/isr.o $(BUILD)/context.o

OBJS := $(ASM_OBJS) $(C_OBJS)

BOOT_BIN   := $(DIST)/boot.bin
KERNEL_ELF := $(DIST)/kernel.elf
KERNEL_BIN := $(DIST)/kernel.bin
IMAGE      := $(DIST)/threados.img
ISO        := $(DIST)/threados.iso

# 96 sectors * 512 = 49152 bytes allocated for kernel
KERNEL_MAX_SIZE := 49152
# 1.44 MB standard floppy size in bytes
FLOPPY_SIZE     := 1474560

.PHONY: all clean run run-serial iso

all: $(IMAGE) $(ISO)

$(BUILD) $(DIST):
	mkdir -p $@

# Stage 1 bootloader (512 bytes)
$(BOOT_BIN): boot/boot.asm | $(DIST)
	$(NASM) -f bin $< -o $@

# Assembly kernel modules
$(BUILD)/entry.o: kernel/entry.asm | $(BUILD)
	$(NASM) -f elf32 $< -o $@

$(BUILD)/isr.o: kernel/isr.asm | $(BUILD)
	$(NASM) -f elf32 $< -o $@

$(BUILD)/context.o: kernel/context.asm | $(BUILD)
	$(NASM) -f elf32 $< -o $@

# C kernel modules
$(BUILD)/%.o: kernel/%.c | $(BUILD)
	$(CC) $(CFLAGS) -c $< -o $@

# Link kernel ELF
$(KERNEL_ELF): linker.ld $(OBJS) | $(DIST)
	$(LD) -m elf_i386 -T linker.ld -o $@ $(OBJS)

# Extract raw flat kernel binary
$(KERNEL_BIN): $(KERNEL_ELF)
	$(OBJCOPY) -O binary $< $@
	@sz=$$(stat -c %s $@); \
	if [ $$sz -gt $(KERNEL_MAX_SIZE) ]; then \
	    echo "ERROR: kernel binary ($$sz B) exceeds KERNEL_MAX_SIZE ($(KERNEL_MAX_SIZE) B)"; exit 1; \
	fi; \
	echo "Kernel size: $$sz bytes (limit: $(KERNEL_MAX_SIZE))"

# Assemble bootable floppy image: bootloader + kernel padded to 1.44 MB
$(IMAGE): $(BOOT_BIN) $(KERNEL_BIN)
	cat $(BOOT_BIN) $(KERNEL_BIN) > $@
	truncate -s $(FLOPPY_SIZE) $@
	@echo "Created bootable image: $@ ($$(stat -c %s $@) bytes)"

# Create bootable ISO with El Torito floppy emulation
$(ISO): $(IMAGE) | $(DIST)
	cp $(IMAGE) $(DIST)/floppy.img
	xorriso -as mkisofs -quiet \
	    -V "THREADOS" \
	    -b floppy.img \
	    -c boot.catalog \
	    -hide boot.catalog \
	    -o $@ $(DIST) 2>/dev/null || \
	( mkisofs -quiet -V "THREADOS" -b floppy.img -c boot.catalog -o $@ $(DIST) )
	rm -f $(DIST)/floppy.img
	@echo "Created bootable ISO: $@ ($$(stat -c %s $@) bytes)"

clean:
	rm -rf $(BUILD) $(DIST)

run: $(IMAGE)
	qemu-system-i386 -fda $(IMAGE) -m 64M -serial mon:stdio

run-serial: $(IMAGE)
	qemu-system-i386 -fda $(IMAGE) -m 64M -nographic -serial mon:stdio -no-reboot

run-iso: $(ISO)
	qemu-system-i386 -cdrom $(ISO) -m 64M -serial mon:stdio
