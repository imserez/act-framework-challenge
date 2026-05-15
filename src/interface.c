#include "../inc/uart-act-challenge.h"

void print_welcome(void)
{
    printf("=====================================\n");
    printf("UART-ACT-Connector\n");
    printf("=====================================\n");
}

int write_options_menu(char *msg)
{
    int c;

    printf("WRITE TO THE DEVICE\n");
    printf("1. Write a message\n");
    printf("2. Send PING\n");
    printf("3. Send ELF\n");
    printf("Please, choose [1-3]: ");

    c = getchar();
    while (c < '1' || c > '3')
        c = getchar();

    while (getchar() != '\n')
        ;

    if (c == '1')
    {
        printf("Enter a string: ");
        scanf("%255s", msg);
    }

    return c - '0';
}