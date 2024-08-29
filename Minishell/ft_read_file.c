/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_read_file.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hfafouri <hfafouri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/28 23:49:53 by hfafouri          #+#    #+#             */
/*   Updated: 2024/08/28 23:49:54 by hfafouri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	count_herdoc(t_node *herdoc)
{
	int	number;
	int	i;

	number = 0;
	i = 0;
	while (herdoc != NULL)
	{
		while (((char *)(herdoc->data))[i] != '\0')
		{
			if ((((char *)(herdoc->data))[i] == '<')
				&& (((char *)(herdoc->data))[i + 1] == '<'))
			{
				number++;
				break ;
			}
			i++;
		}
		i = 0;
		herdoc = herdoc->next;
	}
	return (number);
}

void	ft_find_herdoc(t_cmd *env, int *i, int *id, t_node **gc)
{
	char	*str;
	char	*temp;

	str = NULL;
	ft_fork_pipe(env, id, *i, gc);
	if (id[*i] == 0)
	{
		close((env->fd)[*i][0]);
		signal(SIGINT, ft_signal_handler_herdoc);
		while (env->heredoc)
		{
			while (1)
			{
				str = readline("heredoc> ");
				if (str == NULL || (ft_strcmp(env->heredoc->data, str) == 0))
					break ;
				if (env->flag_her != 1)
					str = handle_dollar_sign_heredoc(str, env, gc);
				temp = str;
				str = ft_strjoin(gc, str, "\n");
				// free(temp);
				if (env->heredoc->next == NULL)
					write((env->fd)[*i][1], str, ft_strlen(str));
			}
			if (env->heredoc->next == NULL)
				close((env->fd)[*i][1]);
			env->heredoc = env->heredoc->next;
		}
		(ft_lstclear(gc), exit(0));
	}
	else
	{
		close((env->fd)[*i][1]);
		waitpid(id[*i], &env->status, 0);
		if (WEXITSTATUS(env->status) == 130)
		{
			signal(SIGINT, ft_signal_handler);
			env->flag_signle = 1;
		}
	}
	(*i)++;
}

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
