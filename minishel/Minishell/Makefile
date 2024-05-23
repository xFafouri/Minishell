SOR = minishell.c ft_link_node.c ft_split.c ft_access.c ft_helper.c ft_creat_child.c \
ft_write_path.c ft_path_cmd.c ft_exc_cmd.c  tokenization/tokenization.c \
input_validation/input.c
OBJC=$(SOR:.c=.o)
NAME = minishell
RM = rm -f
CC = cc -ggdb #-fsanitize=address
LDLIBS = -lreadline
#CFLAGS = -Wall -Wextra -Werror -fsanitize=address

all: libft\libft.a $(NAME)

libft\libft.a:
	make -C libft

$(NAME): $(OBJC) 
	$(CC) -o $(NAME) $^ $(LDLIBS) libft/libft.a

clean:
	$(RM) $(OBJC)
	make -C libft clean

fclean: clean
	$(RM) $(NAME)
	make -C libft fclean

re: fclean all
