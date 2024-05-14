#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <readline/readline.h>

typedef struct s_node
{
    void *data;
    struct s_node *next;
} t_node;

typedef struct s_cmd
{
    char *access_path;
    char **env;
} t_cmd;

void *gc_malloc(t_node **gc, int size);
char **ft_split(char *s, char c, t_node **gc);
void ft_lstadd_back(t_node **lst, t_node *new);
void ft_exc_cmd(t_node *cmd, t_node **gc, t_cmd *env);
t_node *ft_lstnew(void *ptr);
char *ft_strchr_hlber(char *s, int c, int *n);
char *ft_check_path(char *str, t_node **gc, t_cmd *env);
void ft_access_to_path(t_node **gc, char **str, char *path_cmd);
void ft_first_child(char *cmd, int **fd, int i, t_node **gc, t_cmd *env);
void ft_lstclear(t_node **lst);
char *ft_check_space(char *av, t_node **gc);
char	*ft_strjoin(t_node **gc, char *s1, char *s2);
void ft_last_child(char *cmd, int **fd, int i, t_node **gc, t_cmd *env);
