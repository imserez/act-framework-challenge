#include "uart-act-challenge.h"


void print_welcome(void)
{
    printf("=====================================\n");
    printf("UART-ACT-Connector\n");
    printf("=====================================\n");
}



void write_options_menu(char *msg)
{
    printf("WRITE TO THE DEVICE\n");
    printf("1. Write a message\n");
    printf("2. Send ELF\n");

    printf("Please, choose [1-2]: ");
    int c = getchar();
    while (c != '1' && c != 3)
    {
        if (c == '2') printf("Option [2] is not yet implemented!\n");
        c = getchar();
    }

    if (c == '1')
    {
        printf("Enter a string: ");
        scanf("%255s", msg);
    }
}