#include <termios.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

int serial_open(char *port, int baud)
{
    int fd;
    struct termios tty;
    if ((fd = open(port, O_RDWR | O_NOCTTY | O_SYNC)) < 0)
    {
        perror("error1 \n");
        return -1;
    }

    if (tcgetattr(fd, &tty) < 0)
    {
        printf("error2 \n");
        return -1;
    }

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

    printf("returning ok \n");


    return fd;
}

int main (int argc, char *argv[])
{
    char *serial = NULL;

    if (argc != 2)
    {
        printf("No serial specified, setting to default /dev/pts/2\n");
        serial = "/dev/pts/2";
    }
    else
        serial = argv[1];

    printf("Opening port : [%s].\n", serial);

    int fd = serial_open(serial, B9600);


    char msg[] = "hey";
    write(fd, msg, sizeof(msg));

    char response[100];

    read(fd, response, 100);

    printf("the response was: [%s]\n", response);

    printf("exiting...");
    return 0;
}