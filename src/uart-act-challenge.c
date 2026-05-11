#include "../inc/uart-act-challenge.h"



int main (int argc, char *argv[])
{
    char *serial = DEFAULT_PORT;

    printf("=====================================\n");
    printf("========= UART-ACT-Connector ========\n");
    printf("=====================================\n");

    if (argc != 2)
    {
        printf("=> No serial specified, setting to default [%s]\n", DEFAULT_PORT);
    }
    else {
        printf("=> Serial set to [%s]\n", argv[1]);
        serial = argv[1];
    }
    printf("=> Trying to open port: [%s]...\n", serial);

    int fd = serial_open(serial, B9600);

    write_options_menu();
    char msg[] = "hey";
    write_to_device(fd, msg);



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

    printf("=> Closing [%s] before exiting...\n", serial);
    close(fd);
    printf("Goodbye!\n");
    return 0;
}