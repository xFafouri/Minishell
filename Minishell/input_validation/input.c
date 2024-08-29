/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hfafouri <hfafouri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/22 15:14:18 by hfafouri          #+#    #+#             */
/*   Updated: 2024/08/23 18:07:21 by hfafouri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	syntax_error(int fd, int *ero, t_cmd *token)
{
	write(fd, "syntax error\n", 13);
	*ero = 1;
	token->status = 2;
}

int	handle_q(char c, t_input_v *input)
{
	if (c == '"' && !input->in_sq)
		input->in_dq = !input->in_dq;
	else if (c == '\'' && !input->in_dq)
		input->in_sq = !input->in_sq;
	return (input->in_dq || input->in_sq);
}

void	handle_redirection(char *line, int *i, t_input_v *input, t_cmd *token)
{
	char	redir;

	redir = line[*i];
	(*i)++;
	if (line[*i] == redir)
		(*i)++;
	while (line[*i] == ' ' || line[*i] == '\t')
		(*i)++;
	if (line[*i] == '\0' || line[*i] == '>' || line[*i] == '<'
		|| line[*i] == '|')
		syntax_error(2, &input->ero, token);
}

void	help_validation(char *line, int *i, t_input_v *input, t_cmd *token)
{
	while (line[*i] != '\0' && !input->ero)
	{
		if (!handle_q(line[*i], input))
		{
			if (line[*i] == '|' && line[*i + 1] != '\0' && line[*i + 1] != '|')
			{
				(*i)++;
				while (line[*i] == ' ' || line[*i] == '\t')
					(*i)++;
				if (line[*i] == '\0' || line[*i] == '|')
					syntax_error(2, &input->ero, token);
				continue ;
			}
			else if (line[*i] == '>' || line[*i] == '<')
			{
				handle_redirection(line, i, input, token);
				continue ;
			}
			else if (line[*i] == '&' || line[*i] == '|' || line[*i] == ';'
				|| line[*i] == '(' || line[*i] == ')')
				syntax_error(2, &input->ero, token);
		}
		(*i)++;
	}
}
int	input_validation(char *line, t_cmd *token)
{
	t_input_v	input;
	int			i;

	input.in_sq = 0;
	input.in_dq = 0;
	input.ero = 0;
	i = 0;
	while (line[i] == ' ' || line[i] == '\t')
		i++;
	if (line[i] == '|')
		syntax_error(2, &input.ero,token);
	help_validation(line, &i, &input, token);
	if (input.in_dq || input.in_sq)
		syntax_error(2, &input.ero, token);
	return (input.ero);
}
