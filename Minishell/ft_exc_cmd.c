/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exc_cmd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbourziq <sbourziq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/31 18:14:24 by sbourziq          #+#    #+#             */
/*   Updated: 2024/08/31 23:10:31 by sbourziq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

void	print_last_redirection(char *input, t_cmd *head)
{
	int	i;

	i = ft_strlen(input) - 1;
	while (i >= 0)
	{
		if (i >= 1 && input[i] == '>' && input[i - 1] == '>')
		{
			head->flag_appned = 2;
			return ;
		}
		else if (i >= 0 && input[i] == '>')
		{
			head->flag_appned = 1;
			return ;
		}
		i--;
	}
	head->flag_appned = 0;
}

void	process_line(t_node *line, t_cmd *env, int *i, int count)
{
	t_node	**gc;

	gc = env->gc_comand;
	print_last_redirection((char *)line->data, env);
	tokenisation(line->data, gc, env);
	line->data = handle_dollar_sign((char *)line->data, env, gc);
	parse_commands(line->data, gc, env);
	if (env->heredoc != NULL)
		ft_find_herdoc(env, i, env->id, gc);
	if (env->flag_signle == 1)
	{
		ft_lstclear(gc);
		return ;
	}
	if ((count_cmd(line) == 1) && (*i == 0))
	{
		if (handle_single_command(env, line->data, gc) == 0)
			return ;
	}
	ft_fork_and_pipe(env, *i, count, line->data);
}

void	ft_exc_cmd(t_node *line, t_node **gc, t_cmd *env)
{
	int	count;
	int	i;

	i = 0;
	env->her = 0;
	count = count_cmd(line) + count_herdoc(line);
	malloc_fd_id(&(env->id), env, count + 1, gc);
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	while (line != NULL)
	{
		process_line(line, env, &i, count);
		line = line->next;
		i++;
	}
	wait_and_handle_signals(count, env);
	ft_lstclear(gc);
}
