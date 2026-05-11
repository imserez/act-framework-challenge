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
