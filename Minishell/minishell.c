/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hfafouri <hfafouri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/29 01:51:21 by hfafouri          #+#    #+#             */
/*   Updated: 2024/08/29 18:49:33 by hfafouri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	has_non_space_chars(const char *str)
{
	while (*str)
	{
		if (!isspace((unsigned char)*str))
			return (1);
		str++;
	}
	return (0);
}

t_env	*init_environment(char ***envp, t_node **fd)
{
	t_env	*env_list;

	if (*envp == NULL || (*envp)[0] == NULL)
	{
		*envp = gc_malloc(fd, 4 * sizeof(char *));
		(*envp)[0] = ft_strdup(fd, "PWD=/nfs/homes/sbourziq");
		(*envp)[1] = ft_strdup(fd, "SHLVL=1");
		(*envp)[2] = ft_strdup(fd, "_=/usr/bin/env");
		(*envp)[3] = NULL;
	}
	env_list = init_env_list(*envp, fd);
	return (env_list);
}

void	setup_signals(void)
{
	signal(SIGINT, ft_signal_handler);
	signal(SIGQUIT, SIG_IGN);
}

void	process_input(char *line, t_cmd *ev)
{
	t_node	*command_gc;

	command_gc = NULL;
	ev->flag_signle = 0;
	if (input_validation(line, ev) != 1)
		split_pipe(line, ev, &command_gc);
	if (has_non_space_chars(line))
		add_history(line);
	ft_lstclear(&command_gc);
}

int	main(int argc, char **argv, char **envp)
{
	t_node	*gc;
	t_node	*fd;
	t_cmd	ev = {0};
	char	*line;

	gc = NULL;
	fd = NULL;
	ev.env = envp;
	ev.addres_env = init_environment(&envp, &fd);
	ev.addres_fd = fd;
	setup_signals();
	while (1)
	{
		ev.falg_to_exit = 0;
		ft_sort_env_list(&ev);
		line = readline(FG_YELLOW "$ " FG_GREEN);
		if (line != NULL)
		{
			process_input(line, &ev);
			free(line);
		}
		else
			break ;
	}
	printf("exit\n");
	ft_lstclear(&fd);
	rl_clear_history();
	return (ev.status);
}
