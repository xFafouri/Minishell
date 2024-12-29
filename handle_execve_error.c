/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_execve_error.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbourziq <sbourziq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/30 00:39:54 by sbourziq          #+#    #+#             */
/*   Updated: 2024/09/01 21:36:50 by sbourziq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_handle_command_not_found(t_cmd *token, t_node **gc)
{
	if (ft_strchr((token->cmd)[0], '/') != NULL)
	{
		perror((token->cmd)[0]);
		ft_lstclear(gc);
		ft_lstclear(&token->addres_fd);
		exit(127);
	}
	write(2, (token->cmd)[0], ft_strlen((token->cmd)[0]));
	write(2, ": command not found\n", 21);
	ft_lstclear(&token->addres_fd);
	ft_lstclear(gc);
	exit(127);
}

void	ft_handle_execve_error(char *path, t_node **gc, t_cmd *token)
{
	if (access(path, F_OK) == 0)
	{
		if (access(path, X_OK) == 0)
		{
			write(2, path, ft_strlen(path));
			write(2, ": is a directory\n", 18);
			ft_lstclear(gc);
			ft_lstclear(&token->addres_fd);
			exit(126);
		}
	}
	perror(path);
	ft_lstclear(gc);
	ft_lstclear(&token->addres_fd);
	exit(126);
}

void	ft_setup_first_child_io(int i, t_cmd *token, t_node **gc)
{
	close((token->fd)[i][0]);
	if (dup2((token->fd)[i][1], 1) < 0)
	{
		perror("dup2 filed\n");
		ft_lstclear(gc);
		ft_lstclear(&token->addres_fd);
		exit(1);
	}
	close((token->fd)[i][1]);
}

void	ft_setup_middle_child_io(int i, t_cmd *token, t_node **gc)
{
	close((token->fd)[i][0]);
	if (dup2((token->fd)[i - 1][0], 0) < 0 || dup2((token->fd)[i][1], 1) < 0)
	{
		perror("dup2 filed\n");
		ft_lstclear(gc);
		ft_lstclear(&token->addres_fd);
		exit(1);
	}
	close((token->fd)[i - 1][0]);
	close((token->fd)[i][1]);
}

void	ft_setup_last_child_io(int i, t_cmd *token, t_node **gc)
{
	close((token->fd)[i][1]);
	if (dup2((token->fd)[i - 1][0], 0) < 0)
	{
		perror("dup2 filed\n");
		ft_lstclear(gc);
		ft_lstclear(&token->addres_fd);
		exit(1);
	}
	close((token->fd)[i - 1][0]);
}
