#include <termios.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

#include <sys/select.h>
#include <string.h>
#include <errno.h>

#ifndef UART_ACT_CHALLENGE
# define UART_ACT_CHALLENGE

# define TIMEOUT_SEC 2
# define DEFAULT_PORT "/dev/pts/2"
# define LOG_LEVEL 2 // (min) 0,1,2 (max)

// serial_comm.c
int serial_open(char *port, int baud);
int write_to_device(int fd, char *msg);

// interface.c
void print_welcome(void);

#endif