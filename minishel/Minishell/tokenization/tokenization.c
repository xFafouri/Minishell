/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenization.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbourziq <sbourziq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/19 15:40:39 by hfafouri          #+#    #+#             */
/*   Updated: 2024/06/03 09:48:34 by sbourziq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	parse_commands(char *line1, int *i, t_node **gc, t_cmd *token)
{
	int	j;
	int	k;
	int	cmd_count;

	j = 0;
	k = 0;
	cmd_count = 0;
	while (line1[*i])
	{
		while (line1[*i] == ' ' || line1[*i] == '\t')
			(*i)++;
		if (line1[*i] == '>' || line1[*i] == '<')
			break ;
		j = *i;
		if (line1[*i] == '\"')
		{
			(*i)++;
			while (line1[*i] != '\"' && line1[*i])
				(*i)++;
			if (line1[*i] == '\"')
				(*i)++;
		}
		else
		{
			while (line1[*i] && line1[*i] != ' ' && line1[*i] != '\t'
				&& line1[*i] != '>' && line1[*i] != '<')
				(*i)++;
		}
		cmd_count++;
	}
	token->cmd = gc_malloc(gc, (cmd_count + 1) * sizeof(char *));
	*i = 0;
	while (line1[*i])
	{
		while (line1[*i] == ' ' || line1[*i] == '\t')
			(*i)++;
		if (line1[*i] == '>' || line1[*i] == '<')
			break ;
		if (line1[*i] == '\"')
		{
			(*i)++;
			j = *i;
			while (line1[*i] != '\"' && line1[*i])
				(*i)++;
			token->cmd[k] = gc_malloc(gc, *i - j + 1);
			ft_strcpy(token->cmd[k], &line1[j], *i - j);
			if (line1[*i] == '\"')
				(*i)++;
		}
		else
		{
			j = *i;
			while (line1[*i] && line1[*i] != ' ' && line1[*i] != '\t'
				&& line1[*i] != '>' && line1[*i] != '<')
				(*i)++;
			token->cmd[k] = gc_malloc(gc, *i - j + 1);
			ft_strcpy(token->cmd[k], &line1[j], *i - j);
		}
		k++;
	}
	token->cmd[k] = NULL;
}

void	tokenisation(void *line, t_node **gc, t_cmd *token)
{
	char	*line1;
	int		i;

	token->infile = NULL;
	token->outfile = NULL;
	token->heredoc = NULL;
	token->append = NULL;
	token->cmd = NULL;
	i = 0;
	line1 = ft_strtrim((char *)line, " ", gc);
	while (line1[i])
	{
		while (line1[i] == ' ' || line1[i] == '\t')
			i++;
		if (line1[i] == '>' && line1[i + 1] == '>')
			handle_append(line1, &i, gc, token);
		else if (line1[i] == '>')
			handle_outfile(line1, &i, gc, token);
		else if (line1[i] == '<' && line1[i + 1] == '<')
			handle_heredoc(line1, &i, gc, token);
		else if (line1[i] == '<')
			handle_infile(line1, &i, gc, token);
		else
			parse_commands(line1, &i, gc, token);
	}
}
