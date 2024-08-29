#include "minishell.h"

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

int	ft_process_single_outfile(t_cmd *token, t_node **gc)
{
	if (check_dollars((char *)token->outfile->data) == 1)
		ft_handle_dollar_outfile(token, gc);
	token->file = open(token->outfile->data, O_CREAT | O_TRUNC, 0666);
	if (token->file < 0)
	{
		ft_handle_outfile_error(token, gc);
		return 1;
	}
	close(token->file);
	return (0);
}

int	ft_process_single_append(t_cmd *token, int file, t_node **gc)
{
	if (check_dollars((char *)token->append->data) == 1)
	{
		ft_handle_dollar_append(token, gc);
	}
	file = open(token->append->data, O_WRONLY | O_CREAT | O_APPEND, 0666);
	if (file < 0)
	{
		ft_handle_append_error(token, gc);
		return (1);
	}
	close(file);
	return(0);
}

void	ft_init_file_descriptors(t_cmd *token, t_node **gc)
{
	token->original_stdin = dup(STDIN_FILENO);
	token->original_stdout = dup(STDOUT_FILENO);
	token->file = -1;
	if (token->original_stdin < 0 || token->original_stdout < 0)
	{
		perror("Failed to save original stdin/stdout");
		ft_lstclear(gc);
		exit(1);
	}
}