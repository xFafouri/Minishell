#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <readline/readline.h>
#include <readline/history.h>

typedef struct s_node
{
    void *data;
    struct s_node *next;
} t_node;

typedef struct s_cmd
{
    int **fd;
	char *access_path;
	char **env;
	char **cmd;
	t_node *infile;
	t_node *outfile;
	t_node *heredoc;
	t_node *append;
} t_cmd;


void *gc_malloc(t_node **gc, int size);
char **ft_split(char *s, char c, t_node **gc);
void ft_lstadd_back(t_node **lst, t_node *new);
void ft_exc_cmd(t_node *cmd, t_node **gc, t_cmd *env);
t_node *ft_lstnew(void *ptr);
char *ft_strchr_hlber(char *s, int c, int *n);
char *ft_check_path(char *str, t_node **gc, t_cmd *env);
void ft_access_to_path(t_node **gc, char **str, char *path_cmd);
void ft_lstclear(t_node **lst);
char *ft_check_space(char *av, t_node **gc);
char	*ft_strjoin(t_node **gc, char *s1, char *s2);
void ft_first_child(int i, t_node **gc, t_cmd *token);
void ft_last_child(int i, t_node **gc, t_cmd *token);
void ft_midll_child(int i, t_node **gc, t_cmd *token);
int ft_strncmp(char *str, char *path, int line);
void input_validation(char *line);
void ft_putstr_fd(char *s, int fd);


t_node *ft_lstlast(t_node *lst);
void tokenisation(void *line, t_node **gc, t_cmd *tkoen);
