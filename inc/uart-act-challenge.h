#include <termios.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdint.h>

#include <sys/select.h>
#include <string.h>
#include <errno.h>

#ifndef UART_ACT_CHALLENGE
# define UART_ACT_CHALLENGE

# define TIMEOUT_SEC 2
# define DEFAULT_PORT "/dev/pts/8"
# define BAUD B9600


// serial_comm.c
int     serial_open(const char *port);
int     write_to_device(int fd, const char *msg);
void    wait_response(int fd);
int     serial_configuration(const char *port, int fd);

// interface.c
void    print_welcome(void);
int     write_options_menu(char *msg);

#endif