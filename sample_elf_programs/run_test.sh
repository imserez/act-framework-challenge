riscv64-unknown-elf-gcc -nostdlib -fno-builtin -mcmodel=medany -march=rv64g -mabi=lp64 -T sample_elf_programs/linker.ld sample_elf_programs/sum.c -o sample_elf_programs/sum.elf

# Console debug
riscv64-unknown-elf-objdump -d sample_elf_programs/sum.elf

riscv64-unknown-elf-objcopy -O binary sample_elf_programs/sum.elf sample_elf_programs/sum.bin