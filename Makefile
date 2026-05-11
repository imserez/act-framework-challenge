CC      = gcc
CFLAGS  = -Wall -Wextra -Werror -Iinc
NAME    = uart-challenge

SRCS    = src/uart-act-challenge.c src/serial_comm.c src/interface.c
OBJS    = $(SRCS:.c=.o)
INC     = inc/uart-act-challenge.h

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

%.o: %.c $(INC)
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: all clean fclean re

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all