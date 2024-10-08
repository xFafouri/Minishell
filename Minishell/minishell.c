/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbourziq <sbourziq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/29 01:51:21 by hfafouri          #+#    #+#             */
/*   Updated: 2024/09/01 12:36:55 by sbourziq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_cmd glb;

int has_non_space_chars(const char *str)
{
	while (*str)
	{
		if (!isspace((unsigned char)*str))
			return (1);
		str++;
	}
	return (0);
}

t_env *init_environment(char ***envp, t_node **fd, t_cmd *ev)
{
	t_env *env_list;

	ev->flag = 0;
	if (*envp == NULL || (*envp)[0] == NULL)
	{
		ev->flag = 1;
		*envp = gc_malloc(fd, 4 * sizeof(char *));
		(*envp)[0] = ft_strdup(fd, "PWD=/nfs/homes/sbourziq");
		(*envp)[1] = ft_strdup(fd, "SHLVL=1");
		(*envp)[2] = ft_strdup(fd, "_=/usr/bin/env");
		(*envp)[3] = NULL;
	}
	env_list = init_env_list(*envp, fd);
	return (env_list);
}

void setup_signals(void)
{
	signal(SIGINT, ft_signal_handler);
	signal(SIGQUIT, SIG_IGN);
}

void process_input(char *line, t_cmd *ev)
{
	t_node *command_gc;

	command_gc = NULL;
	ev->flag_signle = 0;
	if (has_non_space_chars(line))
	{
		if (input_validation(line, ev) != 1)
			split_pipe(line, ev, &command_gc);
		add_history(line);
	}
	ft_lstclear(&command_gc);
}

int main(int argc, char **argv, char **envp)
{
	t_node *fd;
	t_cmd ev;

	fd = NULL;
	if (argc > 1)
	{
		write(1, argv[1], ft_strlen(argv[1]));
		write(2, ": No such file or directory\n", 29);
		exit(127);
	}
	initialize_env(&ev, envp, &fd);
	setup_signals();
	main_loop(&ev);
	cleanup(fd);
	return (ev.status);
}
