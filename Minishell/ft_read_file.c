/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_read_file.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbourziq <sbourziq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/28 23:49:53 by hfafouri          #+#    #+#             */
/*   Updated: 2024/09/01 12:29:50 by sbourziq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Function to process the heredoc input
void	ft_process_heredoc(t_cmd *env, int *i, char **str, t_node **gc)
{
	while (env->heredoc)
	{
		while (1)
		{
			*str = readline("heredoc> ");
			if (*str == NULL || (ft_strcmp(env->heredoc->data, *str) == 0))
				break ;
			ft_process_heredoc_line(env, str, gc);
			if (env->heredoc->next == NULL)
				write((env->fd)[*i][1], *str, ft_strlen(*str));
		}
		ft_close_heredoc_if_last(env, i);
		env->heredoc = env->heredoc->next;
	}
}

// Function to process a single line of heredoc input
void	ft_process_heredoc_line(t_cmd *env, char **str, t_node **gc)
{
	if (env->flag_her != 1)
		*str = handle_dollar_sign_heredoc(*str, env, gc);
	*str = ft_strjoin(gc, *str, "\n");
}

// Function to close the heredoc pipe if it's the last one
void	ft_close_heredoc_if_last(t_cmd *env, int *i)
{
	if (env->heredoc->next == NULL)
		close((env->fd)[*i][1]);
}

// Function to handle the parent process of the heredoc
void	ft_handle_heredoc_parent(t_cmd *env, int *i, int *id)
{
	close((env->fd)[*i][1]);
	waitpid(id[*i], &env->status, 0);
	if (WEXITSTATUS(env->status) == 130)
	{
		signal(SIGINT, ft_signal_handler);
		env->flag_signle = 1;
	}
}

// Main function to find and process heredocs
void	ft_find_herdoc(t_cmd *env, int *i, int *id, t_node **gc)
{
	ft_fork_pipe(env, id, *i, gc);
	if (id[*i] == 0)
		ft_handle_heredoc_child(env, i, gc);
	else
		ft_handle_heredoc_parent(env, i, id);
	(*i)++;
}
