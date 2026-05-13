# UART-ACT-Connector

This is my submission to the coding challenge "RISC-V ACT Framework Enablement and M-Mode Firmware Validation on Hardware Board (RISC-V Mentorship)" with 10xEngineers.

## What is this

This is a Proof-of-Concept framework designed to emulate a simple RISC-V ACT (Architecture Compliance Test) workflow sent over UART to a RISC-V machine.

This project initializes and configures a UART interface using the termios API and transmit and receives messages.

The correct behavior can be verified using a terminal with socat, and also, using QEMU simulating a RISC-V machine.

- Responds to PING command
- Accepts tests, simulating a simple ACT framework

The project virtualizes a RISC-V machine using QEMU, establishes UART communication between host and a pre-loaded firmware on that machine, uploads binaries sent by UART to specific memory location, jumps to that location to execute the program and then validates the execution results.

## File structure

## Features

- UART communication using Linux `termios`
- QEMU RISC-V hardware environment
- PING/PONG MMIO communication
- Binary transfers over UART
- Dynamic execution fro firmware
- PooC Pass/Fail execution
- Logging
- Scripts to automate compilation of the tests and setting up the QEMU environment

## How to use

The first step to run this program is to compile using the provided `Makefile` simply run `make` in the terminal

```bash
make
```

To execute the program, use the executable name: `uart-challenge`

```bash
./uart-challenge
```

### Simple communication

poner aqui img/socat-example-1.png

For this setup, we're going to use `socat`.

1. Set socat

```bash
socat -d -d pty,raw,echo=0 pty,raw,echo=0
```

In the `socat` output, we'll see the PTY that it opened to us.
For this particular example, socat opened `/dev/pts/8`

2. Update the PTY address in the program

This can be done either in code or passing the PTY as a program parameter

```c
# uart-act-challenge.h

# define DEFAULT_PORT "/dev/pts/8" // Specify the correct one

```

Or we can launch the program with the parameters:

```bash
make
./uart-challenge /dev/pts/8

```

3. Communicate with `socat`

Now, we can communicate sending messages and see that socat effectively received the data in the terminal. We can also communicate and send responses using socat to the program. To do that, we should use the other socat terminal open. We can use echo to send messages to it:

```bash
echo "Hello!" > /dev/pts/7
```

poner aqui img/socat-example-2.png

### RISC-V Machine using QEMU

-- breve explicacion de QEMU...
-- que necesitaran tener instalado...

1. Set the environment

You can prepare your environment using the provided script in `/qemu_firmware/run_qemu.sh`

```bash
chmod +x qemu_firmware/run_qemu.sh
./qemu_firmware/run_qemu.sh
```

What this script does is compile the linker, boot and main file (firmware) into an elf file that will be loaded as our RISC-V Machine Kernel

```bash
# COMPILE
riscv64-unknown-elf-gcc -nostdlib -fno-builtin -mcmodel=medany -march=rv64g -mabi=lp64 -T qemu_firmware/linker.ld qemu_firmware/boot.S qemu_firmware/main.c -o qemu_firmware/firmware.elf

# RUN QEMU
qemu-system-riscv64 -machine virt -nographic -bios none -kernel qemu_firmware/firmware.elf -serial pty
```

2. Set the test

In order to load a program to the firmware we've just compiled for `QEMU`, we'll use the script provided in `/sample_elf_programs/run_test.sh`

```bash
chmod +x sample_elf_programs/run_test.sh
./sample_elf_programs/run_test.sh
```

What this script does is to compile the test using the linker (this is important because we need to use specific memory regions) to an elf file. Then, using objcopy we conver it to `.bin` file to load it into the memory using UART.

There's also a debug print with objdump

```bash
# Compile
riscv64-unknown-elf-gcc -nostdlib -fno-builtin -mcmodel=medany -march=rv64g -mabi=lp64 -T sample_elf_programs/linker.ld sample_elf_programs/sum.c -o sample_elf_programs/sum.elf

# Console debug
riscv64-unknown-elf-objdump -d sample_elf_programs/sum.elf

# Convert .elf to .bin
riscv64-unknown-elf-objcopy -O binary sample_elf_programs/sum.elf sample_elf_programs/sum.bin
```

## Coding Challenge Requirements

## Technologies utilized

## Resources
