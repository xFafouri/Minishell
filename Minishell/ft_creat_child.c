/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_creat_child.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hfafouri <hfafouri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/28 23:47:38 by hfafouri          #+#    #+#             */
/*   Updated: 2024/08/29 02:15:50 by hfafouri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_one_child(int i, t_node **gc, t_cmd *token)
{
	char	*path;

	path = NULL;
	ft_check_file(token, gc, token->her);
	ft_setup_child_signals();
	path = ft_check_path((token->cmd)[0], gc, token);
	ft_execute_command(path, token, gc);
}

void	ft_execute_command(char *path, t_cmd *token, t_node **gc)
{
	if (path == NULL)
	{
		ft_handle_command_not_found(token, gc);
	}
	if (execve(path, token->cmd, token->env) < 0)
	{
		ft_handle_execve_error(path, gc);
	}
}

void	ft_first_child(int i, t_node **gc, t_cmd *token, char *line)
{
	char	*path;

	path = NULL;
	ft_setup_child_signals();
	ft_all_bildin(i, gc, token, line);
	ft_setup_first_child_io(i, token, gc);
	ft_check_file(token, gc, token->her);
	path = ft_check_path((token->cmd)[0], gc, token);
	ft_execute_command(path, token, gc);
}


void	ft_midll_child(int i, t_node **gc, t_cmd *token, char *line)
{
	char	*path;

	path = NULL;
	ft_setup_child_signals();
	ft_all_bildin(i, gc, token, line);
	ft_setup_middle_child_io(i, token, gc);
	ft_check_file(token, gc, token->her);
	path = ft_check_path((token->cmd)[0], gc, token);
	ft_execute_command(path, token, gc);
}

void	ft_last_child(int i, t_node **gc, t_cmd *token, char *line)
{
	char	*path;

	path = NULL;
	ft_setup_child_signals();
	if (ft_check_buldin1(token, line, gc) == 0)
	{
		ft_check_file(token, gc, token->her);
		ft_check_buldin(token, line, gc);
		exit(1);
	}
	ft_setup_last_child_io(i, token, gc);
	ft_check_file(token, gc, token->her);
	path = ft_check_path((token->cmd)[0], gc, token);
	ft_execute_command(path, token, gc);
}
