#include <fcntl.h>
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
	char			*access_path;
	char			**env;
}					t_cmd;

// #define PIPE 15
typedef enum e_token_types
{
	PIPE = 0,
	WORD = 1,
	REDIN = 2,
	REDOUT = 3,
	HERDOC = 4,
	APPEND = 5,
}					t_token_types;

typedef struct s_token
{
	int				type;
	char			*value;
	struct s_token	*next;
}					t_token;

void				*gc_malloc(t_node **gc, int size);
char				**ft_split(char *s, char c, t_node **gc);
void				ft_lstadd_back(t_node **lst, t_node *new);
void				ft_exc_cmd(t_node *cmd, t_node **gc, t_cmd *env);
t_node				*ft_lstnew(void *ptr);
char				*ft_strchr_hlber(char *s, int c, int *n);
char				*ft_check_path(char *str, t_node **gc, t_cmd *env);
void				ft_access_to_path(t_node **gc, char **str, char *path_cmd);
void				ft_first_child(char *cmd, int **fd, int i, t_node **gc,
						t_cmd *env);
void				ft_lstclear(t_node **lst);
char				*ft_check_space(char *av, t_node **gc);
char				*ft_strjoin(t_node **gc, char *s1, char *s2);
void				ft_last_child(char *cmd, int **fd, int i, t_node **gc,
						t_cmd *env);

//-----------------utils

t_token				*new_token(int type, char *value);
t_token				*lstlast(t_token *lst);
void				add_back(t_token **lst, t_token *new);
int					lstsize(t_token *lst);
void				tokenisation(char *line, t_token **lexer);
