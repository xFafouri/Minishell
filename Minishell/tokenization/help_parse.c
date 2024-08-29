/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   help_parse.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hfafouri <hfafouri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/27 21:19:29 by hfafouri          #+#    #+#             */
/*   Updated: 2024/08/29 18:45:11 by hfafouri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	help_three(char *line1, int *i, t_cmd *token)
{
	token->cmd_count++;
	while (line1[*i] && line1[*i] != ' ' && line1[*i] != '\t'
		&& line1[*i] != '\n' && line1[*i] != '>' && line1[*i] != '<')
	{
		if (line1[*i] == '"' || line1[*i] == '\'')
		{
			token->quote_char = line1[(*i)++];
			while (line1[*i] && line1[*i] != token->quote_char)
			{
				if (line1[*i] == '\\' && line1[*i + 1])
					(*i)++;
				(*i)++;
			}
			if (line1[*i] == token->quote_char)
				(*i)++;
		}
		else
			(*i)++;
	}
}

void	count_commands(char *line1, t_cmd *token)
{
	int	i;

	token->quote_char = 0;
	token->cmd_count = 0;
	i = 0;
	while (line1[i])
	{
		while (line1[i] == ' ' || line1[i] == '\t' || line1[i] == '\n')
			i++;
		if (line1[i] == '>' || line1[i] == '<')
			help_one(line1, &i, token);
		else if (line1[i])
			help_three(line1, &i, token);
	}
}

void	help_one(char *line1, int *i, t_cmd *token)
{
	if (line1[*i + 1] == '>' || line1[*i + 1] == '<')
		(*i)++;
	(*i)++;
	while (line1[*i] == ' ' || line1[*i] == '\t' || line1[*i] == '\n')
		(*i)++;
	if (line1[*i] == '"' || line1[*i] == '\'')
	{
		token->quote_char = line1[(*i)++];
		while (line1[*i] && line1[*i] != token->quote_char)
			(*i)++;
		if (line1[*i] == token->quote_char)
			(*i)++;
	}
	else
	{
		while (line1[*i] && line1[*i] != ' ' && line1[*i] != '\t'
			&& line1[*i] != '\n' && line1[*i] != '>' && line1[*i] != '<')
			(*i)++;
	}
}

void	help_two(char *line1, int *i, t_cmd *token, t_node **gc)
{
	int		len;
	char	*cmd;

	len = 0;
	cmd = gc_malloc(gc, ft_strlen(line1) + 1);
	while (line1[*i] && line1[*i] != ' ' && line1[*i] != '\t'
		&& line1[*i] != '\n' && line1[*i] != '>' && line1[*i] != '<')
	{
		if (line1[*i] == '"' || line1[*i] == '\'')
		{
			token->quote_char = line1[(*i)++];
			while (line1[*i] && line1[*i] != token->quote_char)
			{
				if (line1[*i] == '\\' && line1[*i + 1])
					(*i)++;
				cmd[len++] = line1[(*i)++];
			}
			if (line1[*i] == token->quote_char)
				(*i)++;
		}
		else
			cmd[len++] = line1[(*i)++];
	}
	cmd[len] = '\0';
	token->cmd[token->k++] = cmd;
}
