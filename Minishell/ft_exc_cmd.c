/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exc_cmd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hfafouri <hfafouri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/28 23:47:44 by hfafouri          #+#    #+#             */
/*   Updated: 2024/08/29 18:39:01 by hfafouri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_fork_pipe(t_cmd *file_des, int *id, int i, t_node **gc)
{
	if (pipe((file_des->fd)[i]) == -1)
	{
		perror("pipe failed");
		ft_lstclear(gc);
		exit(1);
	}
	id[i] = fork();
	if (id[i] < 0)
	{
		perror("fork failed");
		(ft_lstclear(gc), exit(1));
	}
}

int	count_cmd(t_node *cmd)
{
	int	i;

	i = 0;
	while (cmd != NULL)
	{
		i++;
		cmd = cmd->next;
	}
	return (i);
}

void	malloc_fd_id(int **id, t_cmd *file_des, int count, t_node **gc)
{
	int	i;

	i = 0;
	*id = gc_malloc(gc, count * sizeof(int));
	file_des->fd = gc_malloc(gc, count * sizeof(int *));
	while (i < count)
	{
		(file_des->fd)[i] = gc_malloc(gc, 2 * sizeof(int));
		i++;
	}
}

void	print_last_redirection(char *input, t_cmd *head)
{
	int	i;

	i = ft_strlen(input) - 1;
	while (i >= 0)
	{
		if (i >= 1 && input[i] == '>' && input[i - 1] == '>')
		{
			head->flag_appned = 2;
			return ;
		}
		else if (i >= 0 && input[i] == '>')
		{
			head->flag_appned = 1;
			return ;
		}
		i--;
	}
	head->flag_appned = 0;
}

void	restore_io(t_cmd *env)
{
	if (env->original_stdin != -1 && env->original_stdout != -1)
	{
		if (dup2(env->original_stdin, STDIN_FILENO) < 0)
			perror("Failed to restore original stdin");
		if (dup2(env->original_stdout, STDOUT_FILENO) < 0)
			perror("Failed to restore original stdout");
		close(env->original_stdin);
		close(env->original_stdout);
	}
}

void	close_pipes(t_cmd *env, int i)
{
	if (i == 0)
		close((env->fd)[i][1]);
	else
	{
		close((env->fd)[i][1]);
		close((env->fd)[i - 1][0]);
	}
}

void	handle_exit_status(int exit_status)
{
	if (exit_status == 131)
		write(STDERR_FILENO, "Quit (core dumped)\n", 20);
	else if (exit_status == 130 || exit_status == 2)
		write(STDERR_FILENO, "\n", 1);
}

void	handle_signal(int signal_num)
{
	if (signal_num == SIGQUIT)
		write(STDERR_FILENO, "Quit (core dumped)\n", 20);
	else if (signal_num == SIGINT)
		write(STDERR_FILENO, "\n", 1);
}

void	wait_and_handle_signals(int count, t_cmd *env)
{
	int	i;
	int	exit_status;
	int	signal_num;

	i = -1;
	while (++i < count)
		waitpid(env->id[i], &(env->status), 0);
	signal(SIGINT, ft_signal_handler);
	if (WIFEXITED(env->status))
	{
		exit_status = WEXITSTATUS(env->status);
		env->status = exit_status;
		handle_exit_status(exit_status);
	}
	else if (WIFSIGNALED(env->status))
	{
		signal_num = WTERMSIG(env->status);
		env->status = 128 + signal_num;
		handle_signal(signal_num);
	}
}

void	ft_fork_and_pipe(t_cmd *env, pid_t *id, int i, t_node **gc, int count,
		char *line_data)
{
	ft_fork_pipe(env, id, i, gc);
	if (id[i] == 0)
	{
		env->flag_file = 1;
		env->her = ft_file(line_data);
		if (count == 1)
			ft_one_child(i, gc, env);
		else if (i == 0)
			ft_first_child(i, gc, env, line_data);
		else if (i + 1 == count)
			ft_last_child(i, gc, env, line_data);
		else
			ft_midll_child(i, gc, env, line_data);
	}
	else
	{
		close_pipes(env, i);
	}
}

int	handle_single_command(t_cmd *env, char *line_data, t_node **gc)
{
	if (ft_check_buldin1(env, line_data, gc) == 0)
	{
		ft_check_file(env, gc, env->her);
		if (env->falg_to_exit == 1)
		{
			env->status = 1;
			env->status = env->status << 8;
			return (0);
		}
		ft_check_buldin(env, line_data, gc);
		restore_io(env);
		env->status = env->status << 8;
		return (0);
	}
	return (1);
}
void	process_line(t_node *line, t_cmd *env, t_node **gc, int *i, int count)
{
	print_last_redirection((char *)line->data, env);
	tokenisation(line->data, gc, env);
	line->data = handle_dollar_sign((char *)line->data, env, gc);
	parse_commands(line->data, gc, env);
	if (env->heredoc != NULL)
		ft_find_herdoc(env, i, env->id, gc);
	if (env->flag_signle == 1)
	{
		ft_lstclear(gc);
		return ;
	}
	if ((count_cmd(line) == 1) && (*i == 0))
	{
		if (handle_single_command(env, line->data, gc) == 0)
			return ;
	}
	ft_fork_and_pipe(env, env->id, *i, gc, count, line->data);
}

void	ft_exc_cmd(t_node *line, t_node **gc, t_cmd *env)
{
	int	count;
	int	i;

	i = 0;
	env->her = 0;
	count = count_cmd(line) + count_herdoc(line);
	malloc_fd_id(&(env->id), env, count + 1, gc);
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	while (line != NULL)
	{
		process_line(line, env, gc, &i, count);
		line = line->next;
		i++;
	}
	wait_and_handle_signals(count, env);
	ft_lstclear(gc);
}
