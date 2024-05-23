SOR = minishell.c ft_link_node.c ft_split.c ft_access.c ft_helper.c ft_creat_child.c \
ft_write_path.c ft_path_cmd.c ft_exc_cmd.c tokenization/tokenization.c builtins/builtins.c utils/utils.c input_validation/input.c
OBJC=$(SOR:.c=.o)
NAME = minishell
RM = rm -f
CC = cc #-fsanitize=address
LDLIBS = -lreadline
#CFLAGS = -Wall -Wextra -Werror -fsanitize=address

all: $(NAME)

$(NAME): $(OBJC)
	$(CC) -o $(NAME) $^ $(LDLIBS)

clean:
	$(RM) $(OBJC)

fclean: clean
	$(RM) $(NAME)

re: fclean all
