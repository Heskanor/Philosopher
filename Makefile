CC=gcc
SRC=src/*.c
INCLUDE=include/*.h
DEPS= $(SRC) $(INCLUDE)
CFLAGS=-Wall -Wextra -Werror -pthread
DFLAGS=-g -fsanitize=address
NAME=philo

all: $(NAME)

$(NAME): $(DEPS)
	$(CC) $(CFLAGS) $(DFLAGS) $(SRC) -o $(NAME)

clean:
	rm -rf $(NAME)

fclean: clean

re: clean all

test:
	echo "no test specified"

.PHONY: all clean fclean re test
