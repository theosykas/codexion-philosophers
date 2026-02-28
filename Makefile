NAME = codexion

CC = cc
CFLAGS = -Wall -Wextra -Werror -Icoders
RM = rm -f

##-fsanitize=address

SRCS = coders/codexion.c \
		coders/utils.c \
		coders/parser.c \
		coders/init.c \
		coders/threads.c \
		coders/routine.c

OBJS = $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(OBJS)
	@echo "Linking $(NAME)..."
	@$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

%.o: %.c
	@echo "Compiling $<..."
	@$(CC) $(CFLAGS) -c $< -o $@

clean:
	@$(RM) $(OBJS)

fclean: clean
	@$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re