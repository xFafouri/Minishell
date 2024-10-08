/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_nor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbourziq <sbourziq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/31 23:07:02 by sbourziq          #+#    #+#             */
/*   Updated: 2024/09/01 12:49:03 by sbourziq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	initialize_env(t_cmd *ev, char **envp, t_node **fd)
{
	ev->status = 0;
	ev->temp = NULL;
	ev->addres_env = init_environment(&envp, fd, ev);
	ev->env = envp;
	ev->addres_fd = *fd;
}

void	cleanup(t_node *fd)
{
	printf("exit\n");
	ft_lstclear(&fd);
	rl_clear_history();
}

void	main_loop(t_cmd *ev)
{
	char	*line;

	while (1)
	{
		ev->falg_to_exit = 0;
		ev->flag_file = 0;
		ft_sort_env_list(ev);
		line = readline(FG_YELLOW "$ " FG_GREEN);
		if(glb.status != 0)
		{
			ev->status = glb.status;
			glb.status = 0;
		}
		if (line != NULL)
		{
			process_input(line, ev);
			free(line);
		}
		if (line == NULL)
			break ;
	}
}
