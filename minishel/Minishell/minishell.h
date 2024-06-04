#include <fcntl.h>
#include <readline/history.h>
#include <readline/readline.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

typedef struct s_node
{
	void			*data;
	struct s_node	*next;
}					t_node;

typedef struct s_cmd
{
	int				**fd;
	char			*access_path;
	char			**env;
	char			**cmd;
	t_node			*infile;
	t_node			*outfile;
	t_node			*heredoc;
	t_node			*append;
	int				count;
	t_node			*head;
}					t_cmd;

void				*gc_malloc(t_node **gc, int size);
char				**ft_split_str(char *str, char *charset);
char				**ft_split(char *s, char c, t_node **gc);
void				ft_lstadd_back(t_node **lst, t_node *new);
void				ft_exc_cmd(t_node *cmd, t_node **gc, t_cmd *env,
						t_node **envp);
t_node				*ft_lstnew(void *ptr);
char				*ft_strchr_hlber(char *s, int c, int *n);
char				*ft_check_path(char *str, t_node **gc, t_cmd *env);
void				ft_access_to_path(t_node **gc, char **str, char *path_cmd);
void				ft_lstclear(t_node **lst);
char				*ft_check_space(char *av, t_node **gc);
char				*ft_strjoin(t_node **gc, char *s1, char *s2);
void				ft_first_child(int i, t_node **gc, t_cmd *token, int her);
void				ft_last_child(int i, t_node **gc, t_cmd *token, int her);
void				ft_midll_child(int i, t_node **gc, t_cmd *token, int her);
int					ft_strncmp(char *str, char *path, int line);
int					input_validation(char *line);
char				*ft_substr(char *s, int start, int len, t_node **gc);
void				ft_lstclear(t_node **lst);
int					ft_strlen(char *s1);
t_node				*ft_lstlast(t_node *lst);
int					ft_strcmp(char *s1, char *s2);
void				tokenisation(void *line, t_node **gc, t_cmd *tkoen);
void				ft_find_herdoc(t_cmd *env, int *i, int *id, t_node **gc);
int					ft_file(char *str);
int					count_herdoc(t_node *herdoc);
void				ft_fork_pipe(t_cmd *file_des, int *id, int i, t_node **gc);
char				*ft_strchr(char *s, int c);
void				ft_check_file(t_cmd *token, int file, t_node **gc, int her);
void				ft_check_infile(t_cmd *token, int file, int her,
						t_node **gc);
void				ft_one_child(int i, t_node **gc, t_cmd *token, int her);

// utils

int					ft_strlen1(char *s);
char				*ft_strcpy(char *s1, char *s2, int len);
static char			check(const char *set, char c);
char				*ft_strtrim(char *s1, char *set, t_node **gc);

// utils tokens

void				handle_append(char *line1, int *i, t_node **gc,
						t_cmd *token);
void				handle_outfile(char *line1, int *i, t_node **gc,
						t_cmd *token);
void				handle_heredoc(char *line1, int *i, t_node **gc,
						t_cmd *token);
void				handle_infile(char *line1, int *i, t_node **gc,
						t_cmd *token);

// utils builtins

void				ft_putchar_fd(char c, int fd);
void				ft_putstr_fd(char *s, int fd);
void				ft_putendl_fd(char *s, int fd);
void				ft_pwd(char *line, t_node **gc);
void				ft_echo(char *line);
void				ft_cd(char *line, t_cmd *token);
void				ft_exit(t_node **gc);
void				ft_export(t_node **gc, t_cmd *token, t_node **envp);

// handle quotes

int					in_single_quote(char *line, int index);
int					in_double_quote(char *line, int index);
char				*handle_quotes(char *line, t_node **gc);
