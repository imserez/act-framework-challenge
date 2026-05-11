#include "../inc/uart-act-challenge.h"

#define TIMEOUT_SEC 2

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


    // addind non-blocking using select

    fd_set read_fds;
    struct timeval timeout;
    FD_ZERO(&read_fds);
    FD_SET(fd, &read_fds);

    timeout.tv_sec = TIMEOUT_SEC;
    timeout.tv_usec = 0;

    printf("Waiting %d seconds", TIMEOUT_SEC);

    int rv = select(fd + 1, &read_fds, NULL, NULL, &timeout);

    if (rv == -1)
    {
        perror("select error");
    } else if (rv == 0)
    {
        printf("timeout reached, no data received\n");
    }
    else
    {
        char res_buf[256];
        memset(&res_buf, '\0', sizeof(res_buf));

        int bread = read(fd, res_buf, sizeof(res_buf) - 1);
        if (bread < 0)
        {
            fprintf(stderr, "erorr %d reading: [%s]\n", errno, strerror(errno));
        }
        else if (bread > 0)
        {
            printf("\n--- Data Received ---\n");
            printf("Bytes read: %d\n", bread);
            printf("Payload: %s\n", res_buf);
            printf("---------------------\n");
        }
    }



    // char response[100];

    // read(fd, response, 100);

    // printf("the response was: [%s]\n", response);

    printf("exiting...");

    close(fd);
    return 0;
}