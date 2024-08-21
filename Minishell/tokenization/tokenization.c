/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenization.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hfafouri <hfafouri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/07 12:45:34 by hfafouri          #+#    #+#             */
/*   Updated: 2024/08/21 16:01:01 by hfafouri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	parse_commands(char *line1, t_node **gc, t_cmd *token)
{
	int		i;
	int		k;
	char	quote_char;
	int		j;
	int		len;
	char	*cmd;

	i = 0;
	k = 0;
	quote_char = 0;
	token->cmd_count = 0;
	while (line1[i])
	{
		while (line1[i] == ' ' || line1[i] == '\t')
			i++;
		if (line1[i] == '>' || line1[i] == '<')
		{
			if (line1[i + 1] == '>' || line1[i + 1] == '<')
				i++;
			i++;
			while (line1[i] == ' ' || line1[i] == '\t')
				i++;
			if (line1[i] == '"' || line1[i] == '\'')
			{
				quote_char = line1[i++];
				while (line1[i] && line1[i] != quote_char)
					i++;
				if (line1[i] == quote_char)
					i++;
			}
			else
			{
				while (line1[i] && line1[i] != ' ' && line1[i] != '\t'
					&& line1[i] != '>' && line1[i] != '<')
					i++;
			}
		}
		else if (line1[i])
		{
			token->cmd_count++;
			while (line1[i] && line1[i] != ' ' && line1[i] != '\t'
				&& line1[i] != '>' && line1[i] != '<')
			{
				if (line1[i] == '"' || line1[i] == '\'')
				{
					quote_char = line1[i++];
					while (line1[i] && line1[i] != quote_char)
					{
						if (line1[i] == '\\' && line1[i + 1])
							i++;
						i++;
					}
					if (line1[i] == quote_char)
						i++;
				}
				else
				{
					i++;
				}
			}
		}
	}
	token->cmd = gc_malloc(gc, (token->cmd_count + 1) * sizeof(char *));
	i = 0;
	k = 0;
	while (line1[i])
	{
		while (line1[i] == ' ' || line1[i] == '\t')
			i++;
		if (line1[i] == '>' || line1[i] == '<')
		{
			if (line1[i + 1] == '>' || line1[i + 1] == '<')
				i++;
			i++;
			while (line1[i] == ' ' || line1[i] == '\t')
				i++;
			if (line1[i] == '"' || line1[i] == '\'')
			{
				quote_char = line1[i++];
				while (line1[i] && line1[i] != quote_char)
					i++;
				if (line1[i] == quote_char)
					i++;
			}
			else
			{
				while (line1[i] && line1[i] != ' ' && line1[i] != '\t'
					&& line1[i] != '>' && line1[i] != '<')
					i++;
			}
		}
		else if (line1[i])
		{
			j = i;
			len = 0;
			cmd = gc_malloc(gc, strlen(line1) + 1);
			while (line1[i] && line1[i] != ' ' && line1[i] != '\t'
				&& line1[i] != '>' && line1[i] != '<')
			{
				if (line1[i] == '"' || line1[i] == '\'')
				{
					quote_char = line1[i++];
					while (line1[i] && line1[i] != quote_char)
					{
						if (line1[i] == '\\' && line1[i + 1])
						{
							i++;
						}
						cmd[len++] = line1[i++];
					}
					if (line1[i] == quote_char)
						i++;
				}
				else
				{
					cmd[len++] = line1[i++];
				}
			}
			cmd[len] = '\0';
			token->cmd[k++] = cmd;
		}
	}
	token->cmd[k] = NULL;
}

void	tokenisation(void *line, t_node **gc, t_cmd *token)
{
	char	*line1;
	int		i;
	int		j;

	token->infile = NULL;
	token->outfile = NULL;
	token->heredoc = NULL;
	token->append = NULL;
	token->cmd = NULL;
	token->cmd_count = 0;
	i = 0;
	j = 0;
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
		{
			while (line1[i] == ' ' || line1[i] == '\t')
				i++;
			j = i;
			if (line1[i] == '\"')
			{
				i++;
				while (line1[i] != '\"' && line1[i])
					i++;
				if (line1[i] == '\"')
					i++;
			}
			while (line1[i] && line1[i] != ' ' && line1[i] != '\t'
				&& line1[i] != '>' && line1[i] != '<')
				i++;
			token->cmd_count++;
		}
	}
	if (token->cmd_count == 0)
	{
		token->cmd = (char **)gc_malloc(gc, 2 * sizeof(char *));
		token->cmd[0] = strdup("");
		token->cmd[1] = NULL;
	}
}
