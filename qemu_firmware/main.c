#include <stdint.h>

#define UART_BASE 0x10000000L
#define UART_THR  (UART_BASE + 0x00) // THR Transmit Holding Register
#define UART_RHR  (UART_BASE + 0x00) // RHR Receive Holding Register
#define UART_LSR  (UART_BASE + 0x05) // LSR Line Status Register

#define UART_LSR_RX_READY 0x01 // LSR:THR Empty
#define UART_LSR_TX_IDLE  0x20 // LSR:THR Empty


// Send single character
void uart_putc(const char c)
{
    while ((*(volatile uint8_t *)UART_LSR & UART_LSR_TX_IDLE) == 0);
        *(volatile uint8_t *) UART_THR = c;
}

// Send stream of characters one at a time
void uart_puts(const char *str)
{
    while (*str)
        uart_putc(*str++);
}

// Read character
char uart_getc(void)
{
    while ((*(volatile uint8_t *)UART_LSR & UART_LSR_RX_READY) == 0);
        return *(volatile uint8_t *) UART_RHR;
}

int main(void)
{
    uart_puts("Firmware READY\r\n");

    char buf[4] = {0};
    while (1) {
        buf[0] = buf[1];
        buf[1] = buf[2];
        buf[2] = buf[3];
        buf[3] = uart_getc();

        if (buf[0] == 'P' && buf[1] == 'I' && buf[2] == 'N' && buf[3] == 'G')
        {
            uart_puts("PONG\r\n");
        }
    }

    return (0);
}