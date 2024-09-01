/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_process_file.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbourziq <sbourziq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/30 00:39:29 by sbourziq          #+#    #+#             */
/*   Updated: 2024/08/31 18:12:19 by sbourziq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
		return ;
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
		if (ft_process_single_outfile(token, gc) != 0)
			return ;
		token->outfile = token->outfile->next;
	}
	ft_process_last_outfile(token, gc);
}

void	ft_handle_append_error(t_cmd *token, t_node **gc)
{
	if (token->flag_file == 1)
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
