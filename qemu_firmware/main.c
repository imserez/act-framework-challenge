#include <stdint.h>

/* QEMU virt machine UART (NS16550A) base address and relevant registers */
#define UART_BASE 0x10000000L
#define UART_THR  (UART_BASE + 0x00)
#define UART_RHR  (UART_BASE + 0x00)
#define UART_LSR  (UART_BASE + 0x05)

/* UART Line Status Register bits used for polling */
#define UART_LSR_RX_READY 0x01
#define UART_LSR_TX_IDLE  0x20

/* RAM address where the received test binary will be loaded */
#define LOAD_ADDRESS 0x80010000L

/* Send one character through UART */
void uart_putc(const char c)
{
    while ((*(volatile uint8_t *)UART_LSR & UART_LSR_TX_IDLE) == 0)
        ;
    *(volatile uint8_t *)UART_THR = c;
}

/* Send a null-terminated string through UART */
void uart_puts(const char *str)
{
    while (*str)
        uart_putc(*str++);
}

/* Receive one character from UART using polling */
char uart_getc(void)
{
    while ((*(volatile uint8_t *)UART_LSR & UART_LSR_RX_READY) == 0)
        ;
    return *(volatile uint8_t *)UART_RHR;
}

/*
 * Main firmware command loop.
 * It currently supports:
 *  - PING: replies with PONG
 *  - LOAD: receives a binary, stores it in RAM and executes it
 */
void read_next(void)
{
    char buf[4] = {0};

    while (1)
    {
        /* Sliding window used to detect 4-byte commands */
        buf[0] = buf[1];
        buf[1] = buf[2];
        buf[2] = buf[3];
        buf[3] = uart_getc();

        if (buf[0] == 'P' && buf[1] == 'I' && buf[2] == 'N' && buf[3] == 'G')
        {
            uart_puts("PONG\r\n");
        }
        else if (buf[0] == 'L' && buf[1] == 'O' && buf[2] == 'A' && buf[3] == 'D')
        {
            uart_puts("[FIRMWARE] Command LOAD received. Awaiting payload size...\r\n");

            /* Deserialize the incoming payload size (little-endian, 4 bytes) */
            uint32_t size = 0;
            size |= ((uint32_t)uart_getc()) << 0;
            size |= ((uint32_t)uart_getc()) << 8;
            size |= ((uint32_t)uart_getc()) << 16;
            size |= ((uint32_t)uart_getc()) << 24;

            uart_puts("[FIRMWARE] Downloading binary payload into RAM...\r\n");

            /* Store the received binary at the predefined execution address */
            uint8_t *dest = (uint8_t *)LOAD_ADDRESS;
            for (uint32_t i = 0; i < size; i++)
            {
                dest[i] = uart_getc();
            }

            uart_puts("[FIRMWARE] Transfer complete. Jumping to execution...\r\n");

            /*
             * Ensure instruction fetch sees the code that has just been written
             * to memory before jumping to it.
             */
            __asm__ volatile("fence.i");

            /* Treat the loaded address as a function pointer and execute it */
            int (*test_func)(void) = (int (*)(void))LOAD_ADDRESS;
            int result = test_func();

            /* Minimal PASS/FAIL reporting based on the returned value */
            if (result == 1)
                uart_puts("[FIRMWARE] RESULT: [PASS]\r\n");
            else
                uart_puts("[FIRMWARE] RESULT: [FAIL]\r\n");

            uart_puts("FIRMWARE READY\r\n");

            /* Clear command buffer before waiting for the next command */
            buf[0] = 0;
            buf[1] = 0;
            buf[2] = 0;
            buf[3] = 0;
        }
    }
}

int main(void)
{
    uart_puts("[FIRMWARE] System Booted. M-MODE FIRMWARE READY\r\n");
    read_next();
    return 0;
}