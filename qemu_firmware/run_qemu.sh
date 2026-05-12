# COMPILE

riscv64-unknown-elf-gcc -nostdlib -fno-builtin -mcmodel=medany -march=rv64g -mabi=lp64 -T qemu_firmware/linker.ld qemu_firmware/boot.S qemu_firmware/main.c -o qemu_firmware/firmware.elf

# RUN QEMU

qemu-system-riscv64 -machine virt -nographic -bios none -kernel qemu_firmware/firmware.elf -serial pty