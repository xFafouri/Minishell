#include "minishell.h"

void ft_check_infile(t_cmd *token, int file, int her, t_node **gc)
{
	if (token->infile != NULL && token->infile->data != NULL)
	{
		while (token->infile->next != NULL)
		{
			file = open(token->infile->data, O_RDONLY);
			if (file < 0)
				(perror(token->infile->data), ft_lstclear(gc), exit(1));
			close(file);
			token->infile = token->infile->next;
		}
		file = open(token->infile->data, O_RDONLY);
		if (file < 0)
			(perror(token->infile->data), ft_lstclear(gc), exit(1));
		if (her != 2)
		{
			if ((dup2(file, 0) < 0))
				(perror("dup2 filed\n"), ft_lstclear(gc), exit(1));
		}
		close(file);
	}
}

void ft_check_file(t_cmd *token, int file, t_node **gc, int her)
{
	if (token->outfile != NULL && token->outfile->data != NULL)
	{
		while (token->outfile->next != NULL)
		{
			file = open(token->outfile->data, O_CREAT | O_TRUNC, 0666);
			if (file < 0)
				(perror(token->outfile->data), ft_lstclear(gc), exit(1));
			close(file);
			token->outfile = token->outfile->next;
		}
		file = open(token->outfile->data, O_WRONLY | O_CREAT | O_TRUNC, 0666);
		if (file < 0)
			(perror(token->outfile->data), ft_lstclear(gc), exit(1));
		if ((dup2(file, 1) < 0))
			(perror("dup2 filed\n"), ft_lstclear(gc), exit(1));
		close(file);
	}
	ft_check_infile(token, file, her, gc);
}

void ft_one_child(int i, t_node **gc, t_cmd *token)
{
	int file;
	char *path;

	file = 0;
	path = NULL;
	ft_check_file(token, file, gc, token->her);
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
		(perror(path), ft_lstclear(gc), exit(126));
}

void ft_all_bildin(int i, t_node **gc, t_cmd *token, char *line)
{
    // Check if the command is a built-in command
    if (ft_check_buldin1(token, line, gc) == 0)
    {
        // Close the read end of the pipe
        close((token->fd)[i][0]);

        if (dup2((token->fd)[i][1], 1) < 0)
        {
            perror("dup2 failed\n");
            ft_lstclear(gc);
            exit(1);
        }

        // Close the write end of the pipe
        close((token->fd)[i][1]);

        // Check and handle input/output files
        ft_check_file(token, token->file, gc, token->her);

        // Execute the built-in command
        ft_check_buldin(token, line, gc);
        // Exit after executing the built-in command
        exit(1);
    }
}

void ft_first_child(int i, t_node **gc, t_cmd *token, char *line)
{
	char *path;

	token->file = 0;
	path = NULL;
	ft_all_bildin(i, gc, token, line); // Check if the command is a built-in command
	close((token->fd)[i][0]);
	if (dup2((token->fd)[i][1], 1) < 0)
		(perror("dup2 filed\n"), ft_lstclear(gc), exit(1));
	close((token->fd)[i][1]);
	ft_check_file(token, token->file, gc, token->her); //Check and handle input/output files
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
		(perror(path), ft_lstclear(gc), exit(126));
}

void ft_midll_child(int i, t_node **gc, t_cmd *token, char *line)
{
	char *path;

	token->file = 0;
	path = NULL;
	ft_all_bildin(i, gc, token, line);  // Check if the command is a built-in command
	close((token->fd)[i][0]);
	if (dup2((token->fd)[i - 1][0], 0) < 0)
		(perror("dup2 filed\n"), ft_lstclear(gc), exit(1));
	close((token->fd)[i - 1][0]);
	if (dup2((token->fd)[i][1], 1) < 0)
		(perror("dup2 filed\n"), ft_lstclear(gc), exit(1));
	close((token->fd)[i][1]);
	ft_check_file(token, token->file, gc, token->her);  //Check and handle input/output files
	path = ft_check_path((token->cmd)[0], gc, token);
	if (path == NULL)
	{
		if (ft_strchr((token->cmd)[0], '/') != NULL)
			(perror((token->cmd)[0]), ft_lstclear(gc), exit(127));
		write(2, (token->cmd)[0], ft_strlen((token->cmd)[0]));
		write(2, ": command not found\n", 21);
		(ft_lstclear(gc), exit(127));
	}
	if (execve(path, (token->cmd), token->env) < 0)
		(perror(path), ft_lstclear(gc), exit(126));
}

void ft_last_child(int i, t_node **gc, t_cmd *token, char *line)
{
	char *path;

	token->file = 0;
	path = NULL;
	if(ft_check_buldin1(token, line, gc) == 0)
	{
		ft_check_file(token, token->file, gc, token->her);
		(ft_check_buldin(token, line, gc), exit(1));
	}
	close((token->fd)[i][1]);
	if (dup2((token->fd)[i - 1][0], 0) < 0)
		(perror("dup2 filed\n"), ft_lstclear(gc), exit(1));
	close((token->fd)[i - 1][0]);
	ft_check_file(token, token->file, gc, token->her); //Check and handle input/output files
	path = ft_check_path((token->cmd)[0], gc, token);
	if (path == NULL)
	{
		if (ft_strchr((token->cmd)[0], '/') != NULL)
			(perror((token->cmd)[0]), ft_lstclear(gc), exit(127));
		write(2, (token->cmd)[0], ft_strlen((token->cmd)[0]));
		write(2, ": command not found\n", 21);
		(ft_lstclear(gc), exit(127));
	}
	if (execve(path, (token->cmd), token->env) < 0)
		(perror(path), ft_lstclear(gc), exit(126));
}
