/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signle.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbourziq <sbourziq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/28 23:46:57 by hfafouri          #+#    #+#             */
/*   Updated: 2024/09/01 13:16:44 by sbourziq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_signal_back_slash(int signum)
{
	(void)signum;
	ft_putstr_fd("\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
	ft_lstclear(glb.gc_comand);
	exit(131);
}

void	ft_signal_handler(int signum)
{
	(void)signum;
	ft_putstr_fd("\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
	glb.status = 130;
}

void	ft_signal_handler_herdoc(int signum)
{
	if (signum == SIGINT)
	{
		write(1, "\n", 1);
		ft_lstclear(glb.gc_comand);
		exit(130);
	}
}

void	ft_signal_handler_cmd(int signum)
{
	(void)signum;
	ft_putstr_fd("\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
	ft_lstclear(glb.gc_comand);
	exit(130);
}

void	ft_setup_child_signals(void)
{
	signal(SIGINT, ft_signal_handler_cmd);
	signal(SIGQUIT, ft_signal_back_slash);
}
