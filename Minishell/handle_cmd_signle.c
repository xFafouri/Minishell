/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_cmd_signle.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbourziq <sbourziq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/30 00:39:50 by sbourziq          #+#    #+#             */
/*   Updated: 2024/08/31 23:17:33 by sbourziq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
		if (env->id[i] > 0)
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

int	handle_single_command(t_cmd *env, char *line_data, t_node **gc)
{
	if (ft_check_buldin1(env) == 0)
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
