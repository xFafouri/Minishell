/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_env.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbourziq <sbourziq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/30 00:37:49 by sbourziq          #+#    #+#             */
/*   Updated: 2024/08/31 23:19:33 by sbourziq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_env_fork(t_cmd *token, t_node **gc, int i)
{
	int	pid;
	int	status;

	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		token->status = 1;
		return ;
	}
	else if (pid == 0)
	{
		ft_env_exec(token, gc, i);
	}
	else
	{
		waitpid(pid, &status, 0);
		if (WIFEXITED(status))
			token->status = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
			token->status = 128 + WTERMSIG(status);
		else
			token->status = 1;
	}
}

void	ft_env(t_cmd *token, t_node **gc)
{
	int	i;
	int	has_args;

	i = 0;
	has_args = 0;
	while (token->cmd[i] != NULL)
	{
		if (ft_strcmp(token->cmd[i], "env") != 0)
		{
			has_args = 1;
			break ;
		}
		i++;
	}
	if (!has_args)
	{
		ft_env_no_args(token);
		return ;
	}
	ft_env_fork(token, gc, i);
}

void	ft_add_env_existing(char *value, char *name, t_cmd *token, int i)
{
	char	*join;
	t_node	*ft;

	ft = token->addres_fd;
	if (name[ft_strlen(name) - 1] == '+')
		token->env[i] = ft_strjoin(&ft, token->env[i], value + 1);
	else
	{
		join = ft_strjoin(&ft, name, value);
		token->env[i] = ft_strdup(&ft, join);
	}
}

void	ft_add_env(char *value, char *name, t_cmd *token)
{
	int		i;
	char	*join;
	char	*key;
	char	*new_node;
	t_node	*ft;

	i = 0;
	ft = token->addres_fd;
	new_node = ft_substr(name, 0, ft_strlen_untile_char(name, '+'), &ft);
	while (token->env[i] != NULL)
	{
		key = ft_substr(token->env[i], 0, ft_strlen_untile_char(token->env[i],
					'='), &ft);
		if (ft_strcmp(key, new_node) == 0)
		{
			ft_add_env_existing(value, name, token, i);
			return ;
		}
		i++;
	}
	name = ft_substr(name, 0, ft_strlen_untile_char(name, '+'), &ft);
	join = ft_strjoin(&ft, name, value);
	token->env[i] = ft_strdup(&ft, join);
	token->env[i + 1] = NULL;
}

void	ft_sort_env_list(t_cmd *token)
{
	t_env	*head;
	t_node	*ft;
	int		swapped;
	t_env	*current;

	if (token == NULL || token->addres_env == NULL)
		return ;
	head = token->addres_env;
	ft = token->addres_fd;
	while (1)
	{
		swapped = 0;
		current = head;
		while (current != NULL && current->next != NULL)
		{
			if (ft_strcmp(current->name, current->next->name) > 0)
			{
				ft_swap_env_nodes(current, current->next, ft);
				swapped = 1;
			}
			current = current->next;
		}
		if (!swapped)
			break ;
	}
}
