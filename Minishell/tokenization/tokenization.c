/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenization.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hfafouri <hfafouri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/19 15:40:39 by hfafouri          #+#    #+#             */
/*   Updated: 2024/05/20 09:57:29 by hfafouri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/*
while (TRUE)
{
	1.skip(white_spaces(line);
		i++;
	2.pipe()
	--> new_token(value, PIPE);
	-->add_back(&head, new);
	3.word()
	--> while(!= | > < >> <<)
	-->new_token(value, WORD);
	-->add_back(&head, new);
	4.redirections()
	--> if ('> >> << <')
	--> new_token(value, REDIRIN OR OUT OU HERDOC OR APPEND);
	--> add_back(&head, new);
}
 */


int	count_len(char *s)
{
	int	len;
	int	i;

	len = 0;
	i = 0;
	while(s[i])
	{	
		while (s[i] != '|' && s[i] != '<' && s[i] != '>')
		{
			len++;
		}
		i++;
	}
	return (len);
}

void	tokenisation(char *line, t_token **lexer)
{
	t_token *new;
	int i = 0;
	int start;
	int len;
	char *word;

	*lexer = NULL;
	while (line[i])
	{
		while (line[i] == ' ' || line[i] == '\t')
			i++;

		if (line[i] == '|')
		{
			new = new_token(PIPE, NULL);
			add_back(lexer, new);
			i++;
		}
		else if (line[i] == '>')
		{
			if (line[i + 1] == '>')
			{
				new = new_token(APPEND, NULL);
				i += 2;
			}
			else
			{
				new = new_token(REDOUT, NULL);
				i++;
			}
			add_back(lexer, new);
		}
		else if (line[i] == '<')
		{
			if (line[i + 1] == '<')
			{
				new = new_token(HERDOC, NULL);
				i += 2;
			}
			else
			{
				new = new_token(REDIN, NULL);
				i++;
			}
			add_back(lexer, new);
		}
		else
		{
			while (line[i] && line[i] != '|' && line[i] != '>' && line[i] != '<')
			{
				len = count_len(line);
			}
			word = malloc(len + 1);
			new = new_token(WORD, word);
			add_back(lexer, new);
		}
	}
}