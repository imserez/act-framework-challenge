#include <termios.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

#include <sys/select.h>
#include <string.h>
#include <errno.h>

#ifndef UART_ACT_CHALLENGE
# define UART_ACT_CHALLENGE

int serial_open(char *port, int baud);

#endif