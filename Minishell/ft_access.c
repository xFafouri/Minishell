/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_access.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hfafouri <hfafouri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/28 23:47:32 by hfafouri          #+#    #+#             */
/*   Updated: 2024/08/28 23:47:33 by hfafouri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_access_to_path(t_node **gc, char **str, char *path_cmd)
{
	if (access(path_cmd, F_OK) == 0)
	{
		if (access(path_cmd, X_OK) == 0)
			*str = path_cmd;
		else
			(perror(path_cmd), ft_lstclear(gc), exit(126));
	}
}

void	ft_all_bildin(int i, t_node **gc, t_cmd *token, char *line)
{
	if (ft_check_buldin1(token, line, gc) == 0)
	{
		ft_setup_builtin_io(i, token, gc);
		ft_check_file(token, gc, token->her);
		ft_check_buldin(token, line, gc);
		exit(0);
	}
}

void	ft_setup_builtin_io(int i, t_cmd *token, t_node **gc)
{
	close((token->fd)[i][0]);
	if (dup2((token->fd)[i][1], 1) < 0)
	{
		perror("dup2 failed\n");
		ft_lstclear(gc);
		exit(1);
	}
	close((token->fd)[i][1]);
}

// Function to handle the child process of the heredoc
void	ft_handle_heredoc_child(t_cmd *env, int *i, t_node **gc)
{
	char	*str;
	char	*temp;

	str = NULL;
	close((env->fd)[*i][0]);
	signal(SIGINT, ft_signal_handler_herdoc);
	ft_process_heredoc(env, i, &str, gc);
	(ft_lstclear(gc), exit(0));
}
