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

    char msg[256] = "";
    int option = write_options_menu(msg);

    if (option == 3) // Send file!
    {
        printf("Preparing to send file...");
    }
    else
    {
        if (option == 2)
        {
            msg[0] = 'P';  msg[1] = 'I';  msg[2] = 'N';  msg[3] = 'G';
        }

        printf("=> Sending message [%s] to device: [%s]...\n", msg, serial);
        write_to_device(fd, msg);

        printf("=> Waiting device response. Timeout at [%d] seconds...\n", TIMEOUT_SEC);

        wait_response(fd);
    }

    printf("=> Closing [%s] before exiting...\n", serial);
    close(fd);
    printf("----------\n");
    printf("Goodbye!\n");
    return 0;
}