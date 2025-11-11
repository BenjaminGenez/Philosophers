
NAME = philo

CC = cc
CFLAGS = -Wall -Wextra -Werror -pthread -I includes
RM = rm -f

SRCS = main.c \
       init.c \
       utils.c \
       philosopher.c \
       routine.c \
       monitor.c

OBJS = $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

%.o: %.c includes/philo.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re
