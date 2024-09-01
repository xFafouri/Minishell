/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_file_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbourziq <sbourziq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/30 00:38:15 by sbourziq          #+#    #+#             */
/*   Updated: 2024/08/31 18:30:57 by sbourziq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_remove_quotes(t_cmd *token)
{
	t_env	*head;
	t_node	*gc;

	head = token->addres_env;
	gc = token->addres_fd;
	while (head != NULL)
	{
		head->value = expand_quotes(head->value, &gc, token);
		head = head->next;
	}
}

void	ft_add_qiotes(t_cmd *token)
{
	t_env	*head;
	t_node	*gc;
	char	*s;

	head = token->addres_env;
	gc = token->addres_fd;
	while (head != NULL)
	{
		if (head->value[0] == '=')
		{
			s = gc_malloc(&gc, ft_strlen(head->value) + 3);
			s = "=\"";
			s = ft_strjoin(&gc, s, (head->value + 1));
			s = ft_strjoin(&gc, s, "\"");
			head->value = ft_strdup(&gc, s);
		}
		head = head->next;
	}
}

void	ft_env_no_args(t_cmd *token)
{
	int	i;

	i = 0;
	while (token->env[i] != NULL)
	{
		printf("%s\n", token->env[i]);
		i++;
	}
}

void	ft_env_exec(t_cmd *token, t_node **gc, int i)
{
	char	*path;

	path = NULL;
	ft_check_file(token, gc, token->her);
	signal(SIGINT, ft_signal_handler_cmd);
	signal(SIGQUIT, ft_signal_back_slash);
	path = ft_check_path(token->cmd[i], gc, token);
	if (path == NULL)
	{
		ft_putstr_fd(token->cmd[i], 2);
		ft_putstr_fd(": No such file or directory\n", 2);
		ft_lstclear(gc);
		exit(127);
	}
	if (execve(path, &token->cmd[i], token->env) < 0)
	{
		perror(path);
		ft_lstclear(gc);
		exit(126);
	}
}

void	ft_swap_env_nodes(t_env *current, t_env *next, t_node *ft)
{
	char	*env_copy_name;
	char	*env_copy_value;

	env_copy_name = ft_strdup(&ft, current->name);
	env_copy_value = ft_strdup(&ft, current->value);
	current->name = next->name;
	current->value = next->value;
	next->name = env_copy_name;
	next->value = env_copy_value;
}
