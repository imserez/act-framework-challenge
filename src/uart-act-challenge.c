#include "../inc/uart-act-challenge.h"

void send_binary_file(int fd, const char *filepath) {
    FILE *file = fopen(filepath, "rb");
    if (!file) {
        printf("[ERROR] Could not open the .bin file: %s\n", filepath);
        return;
    }

    // calculate file size in bytes
    fseek(file, 0, SEEK_END);
    uint32_t size = ftell(file);
    fseek(file, 0, SEEK_SET);

    printf("[INFO] Initiating LOAD protocol...\n");
    write(fd, "LOAD", 4);
    usleep(200000); // safe-sleep to let firmware process the command

    printf("[INFO] Sending payload size: %u bytes...\n", size);
    write(fd, &size, sizeof(uint32_t));
    usleep(200000);

    printf("[INFO] Transmitting binary data...\n");
    char buffer[1];
    for (uint32_t i = 0; i < size; i++) {
        fread(buffer, 1, 1, file);
        write(fd, buffer, 1);
        usleep(1000); // safe-delay to prevent buffer overflow
    }

    fclose(file);
    printf("[SUCCESS] File successfully transmitted.\n");
}

int main (int argc, char *argv[])
{
    char *serial = DEFAULT_PORT;

    printf("=========================================\n");
    printf("      UART-ACT-Connector (PoC Host)      \n");
    printf("=========================================\n");

    if (argc != 2)
    {
        printf("[INFO] No serial specified, using default: %s\n", DEFAULT_PORT);
    }
    else {
        printf("[INFO] Serial set to: %s\n", argv[1]);
        serial = argv[1];
    }
    printf("[INFO] Opening port: %s...\n", serial);

    int fd = serial_open(serial, B9600);

    char msg[256] = "";
    int option = write_options_menu(msg);

    if (option == 3) // Send file!
    {
        printf("\n[INFO] Preparing to send payload: sample_elf_programs/sum.bin\n");
        send_binary_file(fd, "sample_elf_programs/sum.bin");

        printf("\n[INFO] Waiting for test execution on the target board...\n");
        printf("-----------------------------------------\n");
        printf("              TARGET OUTPUT              \n");
        printf("-----------------------------------------\n");

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
                fflush(stdout); // Force print to screen immediately

                // if firmware is ready, we can exit the reading loop
                if (strstr(read_buf, "READY") != NULL) {
                    break;
                }
            } else if (bytes_read == 0) {
                printf("\n[ERROR] Target disconnected unexpectedly.\n");
                break;
            }
        }

        printf("-----------------------------------------\n");
        printf("[SUCCESS] Test execution and validation finished.\n");
    }
    else
    {
        if (option == 2)
        {
            msg[0] = 'P';  msg[1] = 'I';  msg[2] = 'N';  msg[3] = 'G';
        }

        printf("\n[INFO] Sending message to device: %s\n", serial);
        write_to_device(fd, msg);

        printf("[INFO] Waiting device response (Timeout: %d seconds)...\n", TIMEOUT_SEC);

        wait_response(fd);
    }

    printf("\n[INFO] Closing port %s...\n", serial);
    close(fd);
    printf("=========================================\n");
    printf("Exiting. Goodbye!\n");
    return 0;
}