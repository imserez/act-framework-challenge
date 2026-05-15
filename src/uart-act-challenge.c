#include "../inc/uart-act-challenge.h"

/* Read target output after sending the binary until READY is received */
static void wait_test_output(int fd)
{
    char read_buf[256];
    int bytes_read;

    /* Small delay to give the firmware time to start the test */
    usleep(500000);

    while (1)
    {
        memset(read_buf, 0, sizeof(read_buf));
        bytes_read = read(fd, read_buf, sizeof(read_buf) - 1);

        if (bytes_read > 0)
        {
            printf("%s", read_buf);
            fflush(stdout);

            /* Check if firmware has finished */
            if (strstr(read_buf, "READY") != NULL)
                break;
        }
        else if (bytes_read == 0)
        {
            printf("\n[ERROR] Target disconnected unexpectedly.\n");
            break;
        }
    }
}

/* Send a binary file to the firmware using the simple LOAD protocol */
static void send_binary_file(int fd, const char *filepath)
{
    FILE *file = fopen(filepath, "rb");
    if (!file)
    {
        printf("[ERROR] Could not open the .bin file: %s\n", filepath);
        return;
    }

    /* Calculate the file size in bytes */
    fseek(file, 0, SEEK_END);
    uint32_t size = ftell(file);
    fseek(file, 0, SEEK_SET);

    printf("[INFO] Initiating LOAD protocol...\n");
    write(fd, "LOAD", 4);
    usleep(200000);

    printf("[INFO] Sending payload size: %u bytes...\n", size);
    write(fd, &size, sizeof(uint32_t));
    usleep(200000);

    printf("[INFO] Transmitting binary data...\n");

    char buffer[1];
    for (uint32_t i = 0; i < size; i++)
    {
        if (fread(buffer, 1, 1, file) != 1)
        {
            printf("[ERROR] Failed while reading binary file.\n");
            fclose(file);
            return;
        }

        write(fd, buffer, 1);
        usleep(1000);
    }

    fclose(file);
    printf("[SUCCESS] File successfully transmitted.\n");
}

int main(int argc, char *argv[])
{
    char *port = DEFAULT_PORT;
    char msg[256] = "";
    int option;
    int fd;

    print_welcome();

    /* If no port is provided, use the default one */
    if (argc != 2)
    {
        printf("[INFO] No serial port specified, using default: %s\n", DEFAULT_PORT);
    }
    else
    {
        printf("[INFO] Serial port set to: %s\n", argv[1]);
        port = argv[1];
    }

    /* Open the UART device */
    fd = serial_open(port);
    if (fd < 0)
        return 1;

    /* Configure the serial port before starting communication */
    if (serial_configuration(port, fd) < 0)
    {
        close(fd);
        return 1;
    }

    /* Show menu and get the user action */
    option = write_options_menu(msg);

    if (option == 3)
    {
        printf("\n[INFO] Preparing to send payload: sample_elf_programs/sum.bin\n");
        send_binary_file(fd, "sample_elf_programs/sum.bin");

        printf("\n[INFO] Waiting for test execution on the target board...\n");
        printf("-----------------------------------------\n");
        printf("              TARGET OUTPUT              \n");
        printf("-----------------------------------------\n");

        wait_test_output(fd);

        printf("-----------------------------------------\n");
        printf("[SUCCESS] Test execution and validation finished.\n");
    }
    else
    {
        /* Option 2 sends a fixed PING message */
        if (option == 2)
            strcpy(msg, "PING");

        printf("\n[INFO] Sending message to device: %s\n", msg);
        write_to_device(fd, msg);

        printf("[INFO] Waiting device response (Timeout: %d seconds)...\n", TIMEOUT_SEC);
        wait_response(fd);
    }

    printf("\n[INFO] Closing port %s...\n", port);
    close(fd);
    printf("=========================================\n");
    printf("Exiting. Goodbye!\n");

    return 0;
}