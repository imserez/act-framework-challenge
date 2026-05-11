#include "uart-act-challenge.h"


void print_welcome(void)
{
    printf("=====================================\n");
    printf("UART-ACT-Connector\n");
    printf("=====================================\n");
}

void write_options_menu(void)
{
    printf("");
    clearScreen();
}

void clearScreen()
{
  const char *CLEAR_SCREEN_ANSI = "\e[1;1H\e[2J";
  write(STDOUT_FILENO, CLEAR_SCREEN_ANSI, 12);
}

// static void interface_selector(char *port)
// {

// }