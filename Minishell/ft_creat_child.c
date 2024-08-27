#include "minishell.h"

int	ft_handle_dollar_infile(t_cmd *token, t_node **gc)
{
	token->infile->data = handle_dollar_sign((char *)token->infile->data, token,
			gc);
	if (token->env_line == NULL)
	{
		write(2, "ambiguous redirect\n", 20);
		token->status = 1;
		token->falg_to_exit = 1;
		if (token->flag_file == 1)
		{
			ft_lstclear(gc);
			exit(1);
		}
		else
			return (1);
	}
	return (0);
}

int	ft_handle_dollar_outfile(t_cmd *token, t_node **gc)
{
	token->outfile->data = handle_dollar_sign((char *)token->outfile->data,
			token, gc);
	if (token->env_line == NULL)
	{
		write(2, "ambiguous redirect\n", 20);
		token->status = 1;
		token->falg_to_exit = 1;
		if (token->flag_file == 1)
		{
			ft_lstclear(gc);
			exit(1);
		}
		else
			return (1);
	}
	return (0);
}

int	ft_handle_dollar_append(t_cmd *token, t_node **gc)
{
	token->append->data = handle_dollar_sign((char *)token->append->data, token,
			gc);
	if (token->env_line == NULL)
	{
		write(2, "ambiguous redirect\n", 20);
		token->status = 1;
		token->falg_to_exit = 1;
		if (token->flag_file == 1)
		{
			ft_lstclear(gc);
			exit(1);
		}
		else
			return (1);
	}
	return (0);
}

int	ft_handle_infile_error(t_cmd *token, t_node **gc)
{
	if (token->flag_file == 1)
	{
		perror(token->infile->data);
		ft_lstclear(gc);
		exit(1);
	}
	else
	{
		perror(token->infile->data);
		token->falg_to_exit = 1;
		return (1);
	}
}

int	ft_check_last_infile(t_cmd *token, int file, int her, t_node **gc)
{
	int	result;

	if (check_dollars((char *)token->infile->data) == 1)
	{
		result = ft_handle_dollar_infile(token, gc);
		if (result != 0)
			return (result);
	}
	file = open(token->infile->data, O_RDONLY);
	if (file < 0)
		return (ft_handle_infile_error(token, gc));
	if (her != 2 && (dup2(file, 0) < 0))
	{
		perror("dup2 filed\n");
		ft_lstclear(gc);
		exit(1);
	}
	close(file);
	return (0);
}

int	ft_process_single_infile(t_cmd *token, int file, t_node **gc)
{
	int	result;

	if (check_dollars((char *)token->infile->data) == 1)
	{
		result = ft_handle_dollar_infile(token, gc);
		if (result != 0)
			return (result);
	}
	file = open(token->infile->data, O_RDONLY);
	if (file < 0)
		return (ft_handle_infile_error(token, gc));
	close(file);
	return (0);
}

int	ft_check_infile_loop(t_cmd *token, int file, t_node **gc)
{
	int	result;

	while (token->infile->next != NULL)
	{
		result = ft_process_single_infile(token, file, gc);
		if (result != 0)
			return (result);
		token->infile = token->infile->next;
	}
	return (0);
}

int	ft_check_infile(t_cmd *token, int file, int her, t_node **gc)
{
	int	result;

	if (token->infile != NULL && token->infile->data != NULL)
	{
		result = ft_check_infile_loop(token, file, gc);
		if (result != 0)
			return (result);
		if (ft_check_last_infile(token, file, her, gc) != 0)
			return (1);
	}
	return (0);
}

void	ft_init_file_descriptors(t_cmd *token, t_node **gc)
{
	token->file = -1;
	token->original_stdin = dup(STDIN_FILENO);
	token->original_stdout = dup(STDOUT_FILENO);
	if (token->original_stdin < 0 || token->original_stdout < 0)
	{
		perror("Failed to save original stdin/stdout");
		ft_lstclear(gc);
		exit(1);
	}
}
void	ft_handle_outfile_error(t_cmd *token, t_node **gc)
{
	if (token->flag_file == 1)
	{
		perror(token->outfile->data);
		ft_lstclear(gc);
		exit(1);
	}
	else
	{
		perror(token->outfile->data);
		token->falg_to_exit = 1;
	}
}
void	ft_process_single_outfile(t_cmd *token, t_node **gc)
{
	if (check_dollars((char *)token->outfile->data) == 1)
		ft_handle_dollar_outfile(token, gc);
	token->file = open(token->outfile->data, O_CREAT | O_TRUNC, 0666);
	if (token->file < 0)
	{
		ft_handle_outfile_error(token, gc);
	}
	close(token->file);
}

void	ft_process_last_outfile(t_cmd *token, t_node **gc)
{
	if (check_dollars((char *)token->outfile->data) == 1)
	{
		ft_handle_dollar_outfile(token, gc);
	}
	token->file = open(token->outfile->data, O_WRONLY | O_CREAT | O_TRUNC,
			0666);
	if (token->file < 0)
	{
		ft_handle_outfile_error(token, gc);
	}
	if (token->flag_appned == 1)
	{
		if ((dup2(token->file, 1) < 0))
		{
			perror("dup2 filed\n");
			ft_lstclear(gc);
			exit(1);
		}
	}
	close(token->file);
}

void	ft_process_outfiles(t_cmd *token, t_node **gc)
{
	if (token->outfile == NULL || token->outfile->data == NULL)
		return ;
	while (token->outfile->next != NULL)
	{
		ft_process_single_outfile(token, gc);
		token->outfile = token->outfile->next;
	}
	ft_process_last_outfile(token, gc);
}

void	ft_handle_append_error(t_cmd *token, t_node **gc)
{
	if (token->file == 1)
	{
		perror(token->append->data);
		ft_lstclear(gc);
		exit(1);
	}
	else
	{
		perror(token->append->data);
		token->falg_to_exit = 1;
	}
}

void	ft_process_single_append(t_cmd *token, int file, t_node **gc)
{
	if (check_dollars((char *)token->append->data) == 1)
	{
		ft_handle_dollar_append(token, gc);
	}
	file = open(token->append->data, O_WRONLY | O_CREAT | O_APPEND, 0666);
	if (file < 0)
	{
		ft_handle_append_error(token, gc);
	}
	close(file);
}

void	ft_process_last_append(t_cmd *token, int file, t_node **gc)
{
	if (check_dollars((char *)token->append->data) == 1)
	{
		ft_handle_dollar_append(token, gc);
	}
	file = open(token->append->data, O_WRONLY | O_CREAT | O_APPEND, 0666);
	if (file < 0)
	{
		ft_handle_append_error(token, gc);
	}
	if (token->flag_appned == 2)
	{
		if ((dup2(file, 1) < 0))
		{
			perror("dup2 failed\n");
			ft_lstclear(gc);
			exit(1);
		}
	}
	close(file);
}

void	ft_append_outfile(t_cmd *token, int file, t_node **gc)
{
	t_node	*current_append;

	current_append = token->append;
	while (current_append->next != NULL)
	{
		ft_process_single_append(token, file, gc);
		current_append = current_append->next;
	}
	token->append = current_append;
	ft_process_last_append(token, file, gc);
}

void	ft_check_file(t_cmd *token, t_node **gc, int her)
{
	ft_init_file_descriptors(token, gc);
	if (ft_check_infile(token, token->file, her, gc) != 0)
		return ;
	ft_process_outfiles(token, gc);
	if (token->append != NULL && token->append->data != NULL)
		ft_append_outfile(token, token->file, gc);
}

void	ft_one_child(int i, t_node **gc, t_cmd *token)
{
	char	*path;

	path = NULL;
	ft_check_file(token, gc, token->her);
	ft_setup_child_signals();
	path = ft_check_path((token->cmd)[0], gc, token);
	ft_execute_command(path, token, gc);
}

void	ft_setup_child_signals(void)
{
	signal(SIGINT, ft_signal_handler_cmd);
	signal(SIGQUIT, ft_signal_back_slash);
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

void	ft_handle_command_not_found(t_cmd *token, t_node **gc)
{
	if (ft_strchr((token->cmd)[0], '/') != NULL)
	{
		perror((token->cmd)[0]);
		ft_lstclear(gc);
		exit(127);
	}
	write(2, (token->cmd)[0], ft_strlen((token->cmd)[0]));
	write(2, ": command not found\n", 21);
	ft_lstclear(gc);
	exit(127);
}

void	ft_handle_execve_error(char *path, t_node **gc)
{
	if (access(path, F_OK) == 0)
	{
		if (access(path, X_OK) == 0)
		{
			write(2, path, ft_strlen(path));
			write(2, ": is a directory\n", 18);
			ft_lstclear(gc);
			exit(126);
		}
	}
	perror(path);
	ft_lstclear(gc);
	exit(126);
}

void	ft_all_bildin(int i, t_node **gc, t_cmd *token, char *line)
{
	if (ft_check_buldin1(token, line, gc) == 0)
	{
		ft_setup_builtin_io(i, token, gc);
		ft_check_file(token, gc, token->her);
		ft_check_buldin(token, line, gc);
		exit(1);
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

void	ft_setup_first_child_io(int i, t_cmd *token, t_node **gc)
{
	close((token->fd)[i][0]);
	if (dup2((token->fd)[i][1], 1) < 0)
	{
		perror("dup2 filed\n");
		ft_lstclear(gc);
		exit(1);
	}
	close((token->fd)[i][1]);
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

void	ft_setup_middle_child_io(int i, t_cmd *token, t_node **gc)
{
	close((token->fd)[i][0]);
	if (dup2((token->fd)[i - 1][0], 0) < 0 || dup2((token->fd)[i][1], 1) < 0)
	{
		perror("dup2 filed\n");
		ft_lstclear(gc);
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
		exit(1);
	}
	close((token->fd)[i - 1][0]);
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
