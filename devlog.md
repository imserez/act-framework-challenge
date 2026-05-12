## 11-05-2026

Started working on the coding challenge for the RISC-V ACT Framework Enablement and M-Mode Firmware Validation mentorship by 10xEngineers.

Today I focused on understanding UART communication in Linux, using the `termios` API.
The MVP of today:

- Configure UART parameters
- Implement read/write (nonblocking), using select with timeout
- Implement a small menu, that later could be used to communicate and perform actions with the firmware.

I also started exploring how to emulate and validate the workflow using QEMU. The current idea is to:

- Create virtual UART communication setup
- Perform a ping-pong communication test
- Connect a minimal RISC-V firmware environment

The main objective for now is to build a clean and reproducible low-level communication toolchain before moving into firmware validation workflows, while developing a solid understanding of robust UART communication and debugging flows.

## 12-05-2026

Continued working on the coding challenge for the RISC-V ACT Framework Enablement and M-Mode Firmware Validation mentorship by 10xEngineers.

Today I focused on virtualizing the hardware environment using QEMU and establishing a functional UART communication workflow between the host and the emulated RISC-V machine.

After reviewing the QEMU virt machine implementation and the corresponding virt.c source code, I identified the UART configuration used by the platform:

- NS16550A UART
- MMIO-based communication
- UART base address mapped at 0x10000000
- Address range 0x10000000 - 0x100000FF

I also reviewed the 16550 UART architecture and explored how FIFO buffering and Line Status Register (LSR) polling work internally. The FIFO implementation and programmable trigger levels are particularly interesting for future improvements related to throughput and asynchronous communication handling.

Current progress:

Implemented basic UART polling using LSR registers
Successfully achieved bidirectional ping-pong communication using the emulated environment
Added new menu options for sending ping requests and file transfers directly from the host-side interface

One of the main challenges today was correctly mapping and understanding the UART register layout and status flags. The NS16550 documentation was especially helpful while debugging register interactions and validating the polling logic.

### Today's useful references

- https://github.com/qemu/qemu/blob/master/hw/riscv/virt.c
- https://en.wikipedia.org/wiki/16550_UART
- https://caro.su/msx/ocm_de1/16550.pdf
- https://labs.dese.iisc.ac.in/embeddedlab/binary-file-transfer-over-uart-using-fifo-extension-of-uart-shell/
