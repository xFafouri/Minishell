/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenization.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hfafouri <hfafouri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/07 12:45:34 by hfafouri          #+#    #+#             */
/*   Updated: 2024/08/29 02:10:15 by hfafouri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	parse_commands(char *line1, t_node **gc, t_cmd *token)
{
	int		i;

	i = 0;
	token->k = 0;
	token->quote_char = 0;
	count_commands(line1, token);
	token->cmd = gc_malloc(gc, (token->cmd_count + 1) * sizeof(char *));
	while (line1[i])
	{
		while (line1[i] == ' ' || line1[i] == '\t')
			i++;
		if (line1[i] == '>' || line1[i] == '<')
			help_one(line1, &i, token);
		else if (line1[i])
			help_two(line1, &i, token, gc);
	}
	token->cmd[token->k] = NULL;
	if (token->cmd_count == 0)
	{
		token->cmd = (char **)gc_malloc(gc, 2 * sizeof(char *));
		token->cmd[0] = ft_strdup(gc, "");
		token->cmd[1] = NULL;
	}
}

void	init_variable_tokens(t_cmd *token)
{
	token->infile = NULL;
	token->outfile = NULL;
	token->heredoc = NULL;
	token->append = NULL;
	token->cmd = NULL;
	token->cmd_count = 0;
}

void	check_commands(char *line1, int *i, t_cmd *token)
{
	while (line1[*i] == ' ' || line1[*i] == '\t')
		(*i)++;
	if (line1[*i] == '\"')
	{
		(*i)++;
		while (line1[*i] != '\"' && line1[*i])
			(*i)++;
		if (line1[*i] == '\"')
			(*i)++;
	}
	while (line1[*i] && line1[*i] != ' ' && line1[*i] != '\t'
		&& line1[*i] != '>' && line1[*i] != '<')
		(*i)++;
	token->cmd_count++;
}

void	tokenisation(void *line, t_node **gc, t_cmd *token)
{
	char	*line1;
	int		i;

	init_variable_tokens(token);
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
			check_commands(line1, &i, token);
	}
}
