#include "minishell.h"

void	ft_process_last_append(t_cmd *token, int file, t_node **gc)
{
	if (check_dollars((char *)token->append->data) == 1)
	{
		ft_handle_dollar_append(token, gc);
		return;
	}
	file = open(token->append->data, O_WRONLY | O_CREAT | O_APPEND, 0666);
	if (file < 0)
	{
		ft_handle_append_error(token, gc);
		return;
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
		if(ft_process_single_append(token, file, gc) != 0)
			return;
		current_append = current_append->next;
	}
	token->append = current_append;
	ft_process_last_append(token, file, gc);
}

void	ft_check_file(t_cmd *token, t_node **gc, int her)
{
	int is_child;

	is_child = token->flag;
	ft_init_file_descriptors(token, gc);
	if (ft_check_infile(token, token->file, her, gc) != 0)
		return ;
	ft_process_outfiles(token, gc);
	if(is_child == 1)
		token->flag_file = 1;
	if (token->append != NULL && token->append->data != NULL)
		ft_append_outfile(token, token->file, gc);
}