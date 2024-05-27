#include "minishell.h"

void	ft_check_infile(t_cmd *token, int file, int her)
{
	if (token->infile != NULL && token->infile->data != NULL)
	{
		while (token->infile->next != NULL)
		{
			file = open(token->infile->data, O_RDONLY);
			if (file < 0)
				(perror(token->infile->data), exit(1));
			close(file);
			token->infile = token->infile->next;
		}
		file = open(token->infile->data, O_RDONLY);
		if (file < 0)
			(perror(token->infile->data), exit(1));
		if(her != 2)
		{
		if ((dup2(file, 0) < 0))
			(printf("error first dub2 filed\n"), exit(1));
		}
		close(file);
	}
}

void	ft_check_file(t_cmd *token, int file, int *check, int her)
{
	ft_check_infile(token, file, her);
	if (token->outfile != NULL && token->outfile->data != NULL)
	{
		while (token->outfile->next != NULL)
		{
			file = open(token->outfile->data, O_CREAT | O_TRUNC, 0666);
			if (file < 0)
				(printf("error outfile\n"), exit(1));
			close(file);
			token->outfile = token->outfile->next;
		}
		*check = 1;
		file = open(token->outfile->data, O_WRONLY | O_CREAT | O_TRUNC, 0666);
		if (file < 0)
			(printf("error outfile\n"), exit(1));
		if ((dup2(file, 1) < 0))
			(printf("error second dup2\n"), exit(1));
		close(file);
	}
}

void	ft_first_child(int i, t_node **gc, t_cmd *token, int her)
{
	int		file;
	char	*path;
	int		check;

	file = 0;
	check = 0;
	path = NULL;
	close((token->fd)[i][0]);
	if (dup2((token->fd)[i][1], 1) < 0)
		(printf("error first dub2 filed1\n"), exit(1));
	close((token->fd)[i][1]);
	ft_check_file(token, file, &check, her);
	path = ft_check_path((token->cmd)[0], gc, token);
	if (path == NULL)
		(printf("commond not exit\n"), exit(127));
	if (execve(path, token->cmd, token->env) < 0)
		(printf("execve filde\n"), exit(126));
}

void	ft_midll_child(int i, t_node **gc, t_cmd *token, int her)
{
	int		file;
	char	*path;
	int		check;

	file = 0;
	check = 0;
	path = NULL;
	close((token->fd)[i][0]);
	if (dup2((token->fd)[i - 1][0], 0) < 0)
		(printf("error first dub2 filed2\n"), exit(1));
	close((token->fd)[i - 1][0]);
	if (dup2((token->fd)[i][1], 1) < 0)
		(printf("error secont dub2 filed3\n"), exit(1));
	close((token->fd)[i][1]);
	ft_check_file(token, file, &check, her);
	path = ft_check_path((token->cmd)[0], gc, token);
	if (path == NULL)
		(printf("commond not exit\n"), exit(127));
	if (execve(path, (token->cmd), token->env) < 0)
		(printf("execve filde\n"), exit(126));
}

void	ft_last_child(int i, t_node **gc, t_cmd *token, int her)
{
	int		file;
	char	*path;
	int		check;

	file = 0;
	check = 0;
	path = NULL;
	close((token->fd)[i][1]);
	if (dup2((token->fd)[i - 1][0], 0) < 0)
		(printf("error first dub2 filed4\n"), exit(1));
	close((token->fd)[i - 1][0]);
	ft_check_file(token, file, &check, her);
	path = ft_check_path((token->cmd)[0], gc, token);
	if (path == NULL)
		(printf("commond not exit\n"), exit(127));
	if (execve(path, (token->cmd), token->env) < 0)
		(printf("execve filde\n"), exit(126));
}
