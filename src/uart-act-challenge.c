#include "../inc/uart-act-challenge.h"

void send_binary_file(int fd, const char *filepath) {
    FILE *file = fopen(filepath, "rb");
    if (!file) {
        perror("=> [ERROR] Coult not open the .bin file");
        return;
    }

    // calculate file size in bytes
    fseek(file, 0, SEEK_END);
    uint32_t size = ftell(file);
    fseek(file, 0, SEEK_SET);

    printf("=> Sending (LOAD)...\n");
    // write_to_device(fd, "LOAD");
    write(fd, "LOAD", 4);
    usleep(200000); // safe-sleep here, to let firmware process LOAD

    printf("=> Sending the program size: [%u] bytes)...\n", size);
    write(fd, &size, sizeof(uint32_t));
    usleep(200000);

    printf("=> Sending binary...\n");
    char buffer[1];
    for (uint32_t i = 0; i < size; i++) {
        fread(buffer, 1, 1, file);
        write(fd, buffer, 1);
        //safe-delay
        usleep(1000);
    }

    fclose(file);
    printf("=> File successfuly sent.\n");
}

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
        printf("Preparing to send file...\n");
        send_binary_file(fd, "sample_elf_programs/sum.bin");

        printf("=> Waiting for test output from the board...\n\n");
        // wait for responses
        char read_buf[256];
        int bytes_read;

        // safe-delay for test to init
        usleep(500000);

        // read the response
        while (1) {
            memset(read_buf, 0, sizeof(read_buf));
            bytes_read = read(fd, read_buf, sizeof(read_buf) - 1);

            if (bytes_read > 0) {
                printf("%s", read_buf);

                // if firmware is ready, we can exit the reading loop
                if (strstr(read_buf, "READY") != NULL) {
                    break;
                }
            } else {
                perror("Error during read");
            }
        }
        printf("\n=> Test FINISHED.\n");
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