/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   help_utils_token.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hfafouri <hfafouri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/27 21:23:10 by hfafouri          #+#    #+#             */
/*   Updated: 2024/08/27 21:23:21 by hfafouri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	help_skip_spaces(char *line, int *i)
{
	while (line[*i] == ' ' || line[*i] == '\t')
		(*i)++;
}

void	help_handle_quotes(char *line, int *i, int *in_quotes, char *quote_char)
{
	while (line[*i])
	{
		if (!*in_quotes && (line[*i] == '\'' || line[*i] == '"'))
		{
			*in_quotes = 1;
			*quote_char = line[*i];
		}
		else if (*in_quotes && line[*i] == *quote_char)
		{
			*in_quotes = 0;
			*quote_char = 0;
		}
		else if (!*in_quotes && (line[*i] == ' ' || line[*i] == '\t'
				|| line[*i] == '>' || line[*i] == '<'))
			break ;
		(*i)++;
	}
}

char	*help_extract_word(char *line, int *i, t_node **gc)
{
	int		j;
	int		in_quotes;
	char	quote_char;
	char	*word;

	j = *i;
	in_quotes = 0;
	quote_char = 0;
	help_handle_quotes(line, i, &in_quotes, &quote_char);
	word = gc_malloc(gc, *i - j + 1);
	ft_strcpy(word, &line[j], *i - j);
	word[*i - j] = '\0';
	return (word);
}
