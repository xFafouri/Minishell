#include "minishell.h"

void	ft_check_infile(t_cmd *token, int file, int her, t_node **gc)
{
	if (token->infile != NULL && token->infile->data != NULL)
	{
		while (token->infile->next != NULL)
		{
			file = open(token->infile->data, O_RDONLY);
			if (file < 0)
			{
				if (token->flag_file == 1)
					(perror(token->infile->data), ft_lstclear(gc), exit(1));
				else
				{
					perror(token->infile->data);
					token->status = 1;
					return ;
				}
			}
			close(file);
			token->infile = token->infile->next;
		}
		file = open(token->infile->data, O_RDONLY);
		if (file < 0)
		{
			if (token->flag_file == 1)
				(perror(token->infile->data), ft_lstclear(gc), exit(1));
			else
			{
				perror(token->infile->data);
				token->status = 1;
				return ;
			}
		}
		if (her != 2)
		{
			if ((dup2(file, 0) < 0))
				(perror("dup2 filed\n"), ft_lstclear(gc), exit(1));
		}
		close(file);
	}
}

void	ft_check_file(t_cmd *token, t_node **gc, int her)
{
	t_node	*current;

	token->file = -1;
	token->original_stdin = dup(STDIN_FILENO);
	token->original_stdout = dup(STDOUT_FILENO);
	if (token->original_stdin < 0 || token->original_stdout < 0)
	{
		perror("Failed to save original stdin/stdout");
		ft_lstclear(gc);
		exit(1);
	}
	if (token->outfile != NULL && token->outfile->data != NULL)
	{
		while (token->outfile->next != NULL)
		{
			token->file = open(token->outfile->data, O_CREAT | O_TRUNC, 0666);
			if (token->file < 0)
			{
				if (token->flag_file == 1)
					(perror(token->outfile->data), ft_lstclear(gc), exit(1));
				else
				{
					perror(token->outfile->data);
					token->status = 1;
					return ;
				}
			}
			close(token->file);
			token->outfile = token->outfile->next;
		}
		token->file = open(token->outfile->data, O_WRONLY | O_CREAT | O_TRUNC,
				0666);
		if (token->file < 0)
		{
			if (token->flag_file == 1)
				(perror(token->outfile->data), ft_lstclear(gc), exit(1));
			else
			{
				perror(token->outfile->data);
				token->status = 2;
				return ;
			}
		}
		if (token->flag_appned == 1)
		{
			if ((dup2(token->file, 1) < 0))
				(perror("dup2 filed\n"), ft_lstclear(gc), exit(1));
		}
		close(token->file);
	}
	if (token->append != NULL && token->append->data != NULL)
		ft_append_outfile(token, token->file, gc);
	ft_check_infile(token, token->file, her, gc);
}

void	ft_append_outfile(t_cmd *token, int file, t_node **gc)
{
	while (token->append->next != NULL)
	{
		file = open(token->append->data, O_CREAT | O_TRUNC, 0666);
		if (file < 0)
		{
			if (token->flag_file == 1)
				(perror(token->append->data), ft_lstclear(gc), exit(1));
			else
			{
				perror(token->append->data);
				token->status = 1;
				return ;
			}
		}
		close(file);
		token->append = token->append->next;
	}
	file = open(token->append->data, O_WRONLY | O_CREAT | O_APPEND, 0666);
	if (file < 0)
		(perror(token->append->data), ft_lstclear(gc), exit(1));
	if (token->flag_appned == 2)
	{
		if ((dup2(file, 1) < 0))
			(perror("dup2 filed\n"), ft_lstclear(gc), exit(1));
	}
	close(file);
}

void	ft_one_child(int i, t_node **gc, t_cmd *token)
{
	char	*path;

	path = NULL;
	ft_check_file(token, gc, token->her);
	signal(SIGINT, ft_signal_handler_cmd);
	signal(SIGQUIT, ft_signal_back_slash);
	path = ft_check_path((token->cmd)[0], gc, token);
	if (path == NULL)
	{
		if (ft_strchr((token->cmd)[0], '/') != NULL)
			(perror((token->cmd)[0]), ft_lstclear(gc), exit(127));
		write(2, (token->cmd)[0], ft_strlen((token->cmd)[0]));
		write(2, ": command not found\n", 21);
		(ft_lstclear(gc), exit(127));
	}
	if (execve(path, token->cmd, token->env) < 0)
	{
		if (access(path, F_OK) == 0)
		{
			if (access(path, X_OK) == 0)
			{
				write(2, path, ft_strlen(path));
				write(2, ": is a directory\n", 18);
				(ft_lstclear(gc), exit(126));
			}
		}
		(perror(path), ft_lstclear(gc), exit(126));
	}
}

void	ft_all_bildin(int i, t_node **gc, t_cmd *token, char *line)
{
	if (ft_check_buldin1(token, line, gc) == 0)
	{
		close((token->fd)[i][0]);
		if (dup2((token->fd)[i][1], 1) < 0)
		{
			perror("dup2 failed\n");
			ft_lstclear(gc);
			exit(1);
		}
		close((token->fd)[i][1]);
		ft_check_file(token, gc, token->her);
		ft_check_buldin(token, line, gc);
		exit(1);
	}
}

void	ft_first_child(int i, t_node **gc, t_cmd *token, char *line)
{
	char	*path;

	path = NULL;
	signal(SIGINT, ft_signal_handler_cmd);
	signal(SIGQUIT, ft_signal_back_slash);
	// Check if the command is a built-in command
	ft_all_bildin(i, gc, token, line);
	close((token->fd)[i][0]);
	if (dup2((token->fd)[i][1], 1) < 0)
		(perror("dup2 filed\n"), ft_lstclear(gc), exit(1));
	close((token->fd)[i][1]);
	ft_check_file(token, gc, token->her);
	// Check and handle input/output files
	path = ft_check_path((token->cmd)[0], gc, token);
	if (path == NULL)
	{
		if (ft_strchr((token->cmd)[0], '/') != NULL)
			(perror((token->cmd)[0]), ft_lstclear(gc), exit(127));
		write(2, (token->cmd)[0], ft_strlen((token->cmd)[0]));
		write(2, ": command not found\n", 21);
		(ft_lstclear(gc), exit(127));
	}
	if (execve(path, token->cmd, token->env) < 0)
	{
		if (access(path, F_OK) == 0)
		{
			if (access(path, X_OK) == 0)
			{
				write(2, path, ft_strlen(path));
				write(2, ": is a directory\n", 18);
				(ft_lstclear(gc), exit(126));
			}
		}
		(perror(path), ft_lstclear(gc), exit(126));
	}
}

void	ft_midll_child(int i, t_node **gc, t_cmd *token, char *line)
{
	char	*path;

	path = NULL;
	signal(SIGINT, ft_signal_handler_cmd);
	signal(SIGQUIT, ft_signal_back_slash);
	// Check if the command is a built-in command
	ft_all_bildin(i, gc, token, line);
	close((token->fd)[i][0]);
	if (dup2((token->fd)[i - 1][0], 0) < 0)
		(perror("dup2 filed\n"), ft_lstclear(gc), exit(1));
	close((token->fd)[i - 1][0]);
	if (dup2((token->fd)[i][1], 1) < 0)
		(perror("dup2 filed\n"), ft_lstclear(gc), exit(1));
	close((token->fd)[i][1]);
	ft_check_file(token, gc, token->her);
	// Check and handle input/output files
	path = ft_check_path((token->cmd)[0], gc, token);
	if (path == NULL)
	{
		if (ft_strchr((token->cmd)[0], '/') != NULL)
			(perror((token->cmd)[0]), ft_lstclear(gc), exit(127));
		write(2, (token->cmd)[0], ft_strlen((token->cmd)[0]));
		write(2, ": command not found\n", 21);
		(ft_lstclear(gc), exit(127));
	}
	if (execve(path, token->cmd, token->env) < 0)
	{
		if (access(path, F_OK) == 0)
		{
			if (access(path, X_OK) == 0)
			{
				write(2, path, ft_strlen(path));
				write(2, ": is a directory\n", 18);
				(ft_lstclear(gc), exit(126));
			}
		}
		(perror(path), ft_lstclear(gc), exit(126));
	}
}

void	ft_last_child(int i, t_node **gc, t_cmd *token, char *line)
{
	char	*path;

	path = NULL;
	signal(SIGINT, ft_signal_handler_cmd);
	signal(SIGQUIT, ft_signal_back_slash);
	if (ft_check_buldin1(token, line, gc) == 0)
	{
		ft_check_file(token, gc, token->her);
		(ft_check_buldin(token, line, gc), exit(1));
	}
	close((token->fd)[i][1]);
	if (dup2((token->fd)[i - 1][0], 0) < 0)
		(perror("dup2 filed\n"), ft_lstclear(gc), exit(1));
	close((token->fd)[i - 1][0]);
	ft_check_file(token, gc, token->her);
	// Check and handle input/output files
	path = ft_check_path((token->cmd)[0], gc, token);
	if (path == NULL)
	{
		if (ft_strchr((token->cmd)[0], '/') != NULL)
			(perror((token->cmd)[0]), ft_lstclear(gc), exit(127));
		write(2, (token->cmd)[0], ft_strlen((token->cmd)[0]));
		write(2, ": command not found\n", 21);
		(ft_lstclear(gc), exit(127));
	}
	if (execve(path, token->cmd, token->env) < 0)
	{
		if (access(path, F_OK) == 0)
		{
			if (access(path, X_OK) == 0)
			{
				write(2, path, ft_strlen(path));
				write(2, ": is a directory\n", 18);
				(ft_lstclear(gc), exit(126));
			}
		}
		(perror(path), ft_lstclear(gc), exit(126));
	}
}
