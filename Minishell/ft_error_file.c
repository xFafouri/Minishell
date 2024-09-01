/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_error_file.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbourziq <sbourziq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/30 00:38:08 by sbourziq          #+#    #+#             */
/*   Updated: 2024/08/31 18:13:41 by sbourziq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
