#include "../inc/uart-act-challenge.h"

/* Open the file descriptor associated with the serial port */
int serial_open(const char *port)
{
    int fd;

    printf("[INFO] Opening port: %s...\n", port);
    fd = open(port, O_RDWR | O_NOCTTY | O_SYNC);

    if (fd < 0)
        perror("=> ERROR. Error trying to open port");
    else
        printf("=> SUCCESS. Connection established with [%s]\n", port);

    return fd;
}

/* Configure the serial port using termios parameters */
int serial_configuration(const char *port, int fd)
{
    struct termios tty;

    printf("=> Trying to get associated parameters to port: [%s]...\n", port);

    /* Get current serial port configuration */
    if (tcgetattr(fd, &tty) < 0)
    {
        perror("=> ERROR. Error trying to obtain port parameters");
        return -1;
    }

    printf("=> SUCCESS. Parameters obtained on port [%s]\n", port);
    printf("=> Trying to set the parameters to port: [%s]...\n", port);

    /* UART configuration */
    cfsetospeed(&tty, (speed_t) BAUD);
    cfsetispeed(&tty, (speed_t) BAUD);

    tty.c_cflag |= (CLOCAL | CREAD);
    tty.c_cflag &= ~CSIZE;
    tty.c_cflag |= CS8;
    tty.c_cflag &= ~PARENB;
    tty.c_cflag &= ~CSTOPB;

    tty.c_iflag &= ~(IGNBRK | BRKINT | PARMRK | ISTRIP | INLCR | IGNCR | ICRNL | IXON);
    tty.c_lflag &= ~(ECHO | ECHONL | ICANON | ISIG | IEXTEN);
    tty.c_oflag &= ~OPOST;

    tty.c_cc[VMIN] = 1;
    tty.c_cc[VTIME] = 1;

    /* Apply the new configuration immediately */
    if (tcsetattr(fd, TCSANOW, &tty) < 0)
    {
        perror("=> ERROR. Error trying to configure port parameters");
        return -1;
    }

    printf("=> SUCCESS. Parameters configured on port [%s]\n", port);
    printf("=> The device [%s] is ready to initiate communication\n", port);
    printf("*********************************************\n");

    return fd;
}

/* Send a message to the device */
int write_to_device(int fd, const char *msg)
{
    printf("Sending [%s] to device\n", msg);
    return write(fd, msg, strlen(msg));
}

/* Wait for a response using select() and a timeout */
void wait_response(int fd)
{
    fd_set read_fds;
    struct timeval timeout;

    FD_ZERO(&read_fds);
    FD_SET(fd, &read_fds);

    timeout.tv_sec = TIMEOUT_SEC;
    timeout.tv_usec = 0;

    int rv = select(fd + 1, &read_fds, NULL, NULL, &timeout);

    if (rv == -1)
    {
        perror("=> ERROR. Error during select");
    }
    else if (rv == 0)
    {
        printf("=> Timeout reached. No response received.\n");
    }
    else
    {
        char res_buf[2048];
        memset(res_buf, '\0', sizeof(res_buf));

        int bread = read(fd, res_buf, sizeof(res_buf) - 1);
        if (bread < 0)
        {
            fprintf(stderr, "ERROR [%d] reading: [%s]\n", errno, strerror(errno));
        }
        else if (bread > 0)
        {
            printf("\n--- Data Received ---\n");
            printf("Bytes read: %d\n", bread);
            printf("Payload: %s\n", res_buf);
            printf("---------------------\n");
        }
    }
}