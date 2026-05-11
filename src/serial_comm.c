#include "../inc/uart-act-challenge.h"

int serial_open(char *port, int baud)
{
    int fd;
    struct termios tty;

    if ((fd = open(port, O_RDWR | O_NOCTTY | O_SYNC)) < 0)
    {
        perror("=> ERROR. Error trying to open port.\n");
        return -1;
    }
    printf("=> SUCCESS. Connection established with [%s]\n", port);

    printf("=> Trying to get associated parameters to port: [%s]...\n", port);

    if (tcgetattr(fd, &tty) < 0)
    {
        perror("=> ERROR. Error trying to obtain port parameters.\n");
        return -1;
    }
    printf("=> SUCCESS. Parameters obtained on port [%s]\n", port);

    printf("=> Trying to set the parameters to port: [%s]...\n", port);

    cfsetospeed(&tty, (speed_t) baud);
    cfsetispeed(&tty, (speed_t) baud);
    tty.c_cflag |= (CLOCAL | CREAD);
    tty.c_cflag &= ~CSIZE;
    tty.c_cflag |= CS8;
    tty.c_cflag &= ~PARENB;
    tty.c_cflag |= CSTOPB;
    tty.c_cflag &= ~CRTSCTS;
    tty.c_iflag &= ~(IGNBRK | BRKINT | PARMRK | ISTRIP | INLCR | IGNCR | ICRNL | IXON);
    tty.c_lflag &= ~(ECHO | ECHONL | ICANON | ISIG | IEXTEN);
    tty.c_oflag &= ~OPOST;
    tty.c_cc[VMIN] = 1;
    tty.c_cc[VTIME] = 1;

    if(tcsetattr(fd, TCSANOW, &tty))
    {
        printf("error3 \n");
        return -1;
    }
    printf("=> SUCCESS. Parameters configured on port [%s]\n", port);
    printf("=> The device [%s] is ready to initiate communication\n", port);
    printf("*****************************************************");

    return fd;
}

int write_to_device(int fd, char *msg)
{
    return write(fd, msg, sizeof(msg));
}