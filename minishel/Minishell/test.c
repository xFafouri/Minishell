#include "minishell.h"

int	ft_file(char *str)
{
	int	i;
	int	nb;

	i = 0;
	nb = 0;
	while (str[i])
	{
		if (str[i] == '<' && str[i + 1] == '<')
		{
			nb = 2;
			++i;
		}
		else if (str[i] == '<')
			nb = 1;
		i++;
	}
	return (nb);
}

void	ft_fork_pipe(t_cmd *file_des, int *id, int i, t_node **gc)
{
	if (pipe((file_des->fd)[i]) == -1)
	{
		perror("pipe failed");
		ft_lstclear(gc);
		exit(1);
	}
	id[i] = fork();
	if (id[i] < 0)
	{
		perror("fork failed");
		(ft_lstclear(gc), exit(1));
	}
}

int	count_cmd(t_node *cmd)
{
	int	i;

	i = 0;
	while (cmd != NULL)
	{
		i++;
		cmd = cmd->next;
	}
	return (i);
}

void	malloc_fd_id(int **id, t_cmd *file_des, int count, t_node **gc)
{
	int	i;

	i = 0;
	*id = gc_malloc(gc, count * sizeof(int));
	file_des->fd = gc_malloc(gc, count * sizeof(int *));
	while (i < count)
	{
		(file_des->fd)[i] = gc_malloc(gc, 2 * sizeof(int));
		i++;
	}
}

void	ft_exc_cmd(t_node *line, t_node **gc, t_cmd *env)
{
	int		*id;
	int		count;
	int		i;
	int		her;
	char	*str;

	i = 0;
	her = 0;
	count = count_cmd(line);
	str = NULL;
	malloc_fd_id(&id, env, count, gc);
	while (line != NULL)
	{
		ft_fork_pipe(env, id, i, gc);
		if (id[i] == 0)
		{
			her = ft_file((char *)line->data);
			tokenisation(line->data, gc, env);
			if (env->heredoc != NULL)
			{
				ft_fork_pipe(env, id, i, gc);
				if (id[i] == 0)
				{
					while (env->heredoc)
					{
						while (1)
						{
							str = readline("herdoc> ");
							if (str == NULL || (ft_strcmp(env->heredoc->data,
										str) == 0))
								break ;
							str = ft_strjoin(gc, str, "\n");
							if (env->heredoc->next == NULL)
								write((env->fd)[i][1], str, ft_strlen(str));
						}
						if (env->heredoc->next == NULL)
							close((env->fd)[i][1]);
						env->heredoc = env->heredoc->next;
					}
					exit(0);
				}
				else
				{
					close((env->fd)[i][1]);
					waitpid(id[i], 0, 0);
					i++;
				}
			}
			printf("my number is %d\n", i);
			printf("count number is %d\n", count);
			if (i == 0)
				ft_first_child(i, gc, env, her);
			else if (i + 1 == count)
				ft_last_child(i, gc, env, her);
			else
				ft_midll_child(i, gc, env, her);
		}
		else
		{
			if (i == 0)
				close((env->fd)[i][1]);
			else
			{
				close((env->fd)[i][1]);
				close((env->fd)[i - 1][0]);
			}
			waitpid(id[i], 0, 0);
		}
		line = line->next;
		i++;
	}
}