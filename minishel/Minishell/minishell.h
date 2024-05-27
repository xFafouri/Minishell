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
}					t_cmd;

void				*gc_malloc(t_node **gc, int size);
char				**ft_split_str(char *str, char *charset);
char				**ft_split(char *s, char c, t_node **gc);
void				ft_lstadd_back(t_node **lst, t_node *new);
void				ft_exc_cmd(t_node *cmd, t_node **gc, t_cmd *env);
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
void				input_validation(char *line);
char				*ft_substr(char *s, int start, int len, t_node **gc);
void				ft_putstr_fd(char *s, int fd);
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
