#include <stdint.h>

#define UART_BASE 0x10000000L
#define UART_THR  (UART_BASE + 0x00)
#define UART_RHR  (UART_BASE + 0x00)
#define UART_LSR  (UART_BASE + 0x05)

#define UART_LSR_RX_READY 0x01
#define UART_LSR_TX_IDLE  0x20

#define LOAD_ADDRESS 0x80010000L

void uart_putc(const char c) {
    while ((*(volatile uint8_t *)UART_LSR & UART_LSR_TX_IDLE) == 0);
    *(volatile uint8_t *) UART_THR = c;
}

void uart_puts(const char *str) {
    while (*str) uart_putc(*str++);
}

char uart_getc(void) {
    while ((*(volatile uint8_t *)UART_LSR & UART_LSR_RX_READY) == 0);
    return *(volatile uint8_t *) UART_RHR;
}

void read_next(void) {
    char buf[4] = {0};
    while (1) {
        buf[0] = buf[1];
        buf[1] = buf[2];
        buf[2] = buf[3];
        buf[3] = uart_getc();

        if (buf[0] == 'P' && buf[1] == 'I' && buf[2] == 'N' && buf[3] == 'G') {
            uart_puts("PONG\r\n"); // Keep standard PONG response
        }
        else if (buf[0] == 'L' && buf[1] == 'O' && buf[2] == 'A' && buf[3] == 'D') {
            uart_puts("[FIRMWARE] Command LOAD received. Awaiting payload size...\r\n");

            //de-serialize
            uint32_t size = 0;
            size |= ((uint32_t)uart_getc()) << 0;
            size |= ((uint32_t)uart_getc()) << 8;
            size |= ((uint32_t)uart_getc()) << 16;
            size |= ((uint32_t)uart_getc()) << 24;

            uart_puts("[FIRMWARE] Downloading binary payload into RAM...\r\n");

            uint8_t *dest = (uint8_t*)LOAD_ADDRESS;
            for (uint32_t i = 0; i < size; i++) {
                dest[i] = uart_getc();
            }

            uart_puts("[FIRMWARE] Transfer complete. Jumping to execution...\r\n");

            // clean before executing
            __asm__ volatile("fence.i");

            // point to the function loaded at LOAD_ADDRESS that returns the result
            int (*test_func)() = (int(*)())LOAD_ADDRESS;

            // execute the test, and capture the return value
            int result = test_func();

            // check if the result is correct or not
            if (result == 1) {
                uart_puts("[FIRMWARE] RESULT: [PASS]\r\n");
            } else {
                uart_puts("[FIRMWARE] RESULT: [FAIL]\r\n");
            }

            uart_puts("FIRMWARE READY\r\n");
            buf[0]=0; buf[1]=0; buf[2]=0; buf[3]=0;
        }
    }
}

int main(void) {
    uart_puts("[FIRMWARE] System Booted. M-MODE FIRMWARE READY\r\n");
    read_next();
    return (0);
}