#include "minishell.h"

void	close_pipes(t_cmd *env, int i)
{
	if (i == 0)
		close((env->fd)[i][1]);
	else
	{
		close((env->fd)[i][1]);
		close((env->fd)[i - 1][0]);
	}
}


void	ft_fork_and_pipe(t_cmd *env, pid_t *id, int i, t_node **gc, int count,
		char *line_data)
{
	ft_fork_pipe(env, id, i, gc);
	if (id[i] == 0)
	{
		env->flag_file = 1;
		env->her = ft_file(line_data);
		if (count == 1)
			ft_one_child(i, gc, env);
		else if (i == 0)
			ft_first_child(i, gc, env, line_data);
		else if (i + 1 == count)
			ft_last_child(i, gc, env, line_data);
		else
			ft_midll_child(i, gc, env, line_data);
	}
	else
	{
		close_pipes(env, i);
	}
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