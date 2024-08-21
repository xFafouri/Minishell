/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_tokens.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hfafouri <hfafouri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/28 15:56:05 by hfafouri          #+#    #+#             */
/*   Updated: 2024/08/20 15:53:19 by hfafouri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	check_dollars(char *line)
{
	int	i;

	i = 0;
	while (line[i])
	{
		if (line[i] == '$')
			return (1);
		i++;
	}
	return (0);
}

void	handle_append(char *line1, int *i, t_node **gc, t_cmd *token)
{
	char	*word;
	char	*expanded;
	int		j;
	int		in_quotes;
	char	quote_char;

	in_quotes = 0;
	quote_char = 0;
	if (*i > 0 && (line1[*i - 1] == '\'' || line1[*i - 1] == '"'))
	{
		(*i) += 2;
		return ;
	}
	*i += 2;
	while (line1[*i] == ' ' || line1[*i] == '\t')
		(*i)++;
	if (line1[*i] != '>' && line1[*i] != '<' && line1[*i])
	{
		j = *i;
		while (line1[*i])
		{
			if (!in_quotes && (line1[*i] == '\'' || line1[*i] == '"'))
			{
				in_quotes = 1;
				quote_char = line1[*i];
			}
			else if (in_quotes && line1[*i] == quote_char)
			{
				in_quotes = 0;
				quote_char = 0;
			}
			else if (!in_quotes && (line1[*i] == ' ' || line1[*i] == '\t'
					|| line1[*i] == '>' || line1[*i] == '<'))
				break ;
			(*i)++;
		}
		word = gc_malloc(gc, *i - j + 1);
		ft_strcpy(word, &line1[j], *i - j);
		word[*i - j] = '\0';
		expanded = expand_quotes(word);
		if ((expanded[0] == '\'' && expanded[strlen(expanded) - 1] == '\'')
			|| (expanded[0] == '"' && expanded[strlen(expanded) - 1] == '"'))
		{
			memmove(expanded, expanded + 1, strlen(expanded) - 2);
			expanded[strlen(expanded) - 2] = '\0';
		}
		ft_lstadd_back(&token->append, ft_lstnew(expanded));
	}
}

void	handle_outfile(char *line1, int *i, t_node **gc, t_cmd *token)
{
	char	*word;
	char	*expanded;
	int		j;
	int		in_quotes;
	char	quote_char;

	in_quotes = 0;
	quote_char = 0;
	if (*i > 0 && (line1[*i - 1] == '\'' || line1[*i - 1] == '"'))
	{
		(*i)++;
		return ;
	}
	(*i)++;
	while (line1[*i] == ' ' || line1[*i] == '\t')
		(*i)++;
	if (line1[*i] != '>' && line1[*i] != '<' && line1[*i])
	{
		j = *i;
		while (line1[*i])
		{
			if (!in_quotes && (line1[*i] == '\'' || line1[*i] == '"'))
			{
				in_quotes = 1;
				quote_char = line1[*i];
			}
			else if (in_quotes && line1[*i] == quote_char)
			{
				in_quotes = 0;
				quote_char = 0;
			}
			else if (!in_quotes && (line1[*i] == ' ' || line1[*i] == '\t'
					|| line1[*i] == '>' || line1[*i] == '<'))
				break ;
			(*i)++;
		}
		word = gc_malloc(gc, *i - j + 1);
		ft_strcpy(word, &line1[j], *i - j);
		word[*i - j] = '\0';
		expanded = expand_quotes(word);
		if ((expanded[0] == '\'' && expanded[strlen(expanded) - 1] == '\'')
			|| (expanded[0] == '"' && expanded[strlen(expanded) - 1] == '"'))
		{
			memmove(expanded, expanded + 1, strlen(expanded) - 2);
			expanded[strlen(expanded) - 2] = '\0';
		}
		ft_lstadd_back(&token->outfile, ft_lstnew(expanded));
	}
	else
		(*i)--;
}

void	handle_heredoc(char *line1, int *i, t_node **gc, t_cmd *token)
{
	char	*word;
	char	*expanded;
	int		j;
	int		in_quotes;
	char	quote_char;

	in_quotes = 0;
	quote_char = 0;
	token->flag_her = 0;
	if (*i > 0 && (line1[*i - 1] == '\'' || line1[*i - 1] == '"'))
	{
		(*i) += 2;
		return ;
	}
	*i += 2;
	while (line1[*i] == ' ' || line1[*i] == '\t')
		(*i)++;
	if (line1[*i] != '>' && line1[*i] != '<' && line1[*i])
	{
		j = *i;
		while (line1[*i])
		{
			if (!in_quotes && (line1[*i] == '\'' || line1[*i] == '"'))
			{
				in_quotes = 1;
				quote_char = line1[*i];
			}
			else if (in_quotes && line1[*i] == quote_char)
			{
				in_quotes = 0;
				quote_char = 0;
			}
			else if (!in_quotes && (line1[*i] == ' ' || line1[*i] == '\t'
					|| line1[*i] == '>' || line1[*i] == '<'))
				break ;
			(*i)++;
		}
		word = gc_malloc(gc, *i - j + 1);
		ft_strcpy(word, &line1[j], *i - j);
		word[*i - j] = '\0';
		expanded = expand_quotes(word);
		if (strcmp(word, expanded) != 0)
			token->flag_her = 1;
		if ((expanded[0] == '\'' && expanded[strlen(expanded) - 1] == '\'')
			|| (expanded[0] == '"' && expanded[strlen(expanded) - 1] == '"'))
		{
			memmove(expanded, expanded + 1, strlen(expanded) - 2);
			expanded[strlen(expanded) - 2] = '\0';
		}
		ft_lstadd_back(&token->heredoc, ft_lstnew(expanded));
	}
}

void	handle_infile(char *line1, int *i, t_node **gc, t_cmd *token)
{
	char	*word;
	char	*expanded;
	int		j;
	int		in_quotes;
	char	quote_char;

	in_quotes = 0;
	quote_char = 0;
	if (*i > 0 && (line1[*i - 1] == '\'' || line1[*i - 1] == '"'))
	{
		(*i)++;
		return ;
	}
	(*i)++;
	while (line1[*i] == ' ' || line1[*i] == '\t')
		(*i)++;
	if (line1[*i] != '>' && line1[*i] != '<' && line1[*i])
	{
		j = *i;
		while (line1[*i])
		{
			if (!in_quotes && (line1[*i] == '\'' || line1[*i] == '"'))
			{
				in_quotes = 1;
				quote_char = line1[*i];
			}
			else if (in_quotes && line1[*i] == quote_char)
			{
				in_quotes = 0;
				quote_char = 0;
			}
			else if (!in_quotes && (line1[*i] == ' ' || line1[*i] == '\t'
					|| line1[*i] == '>' || line1[*i] == '<'))
				break ;
			(*i)++;
		}
		word = gc_malloc(gc, *i - j + 1);
		ft_strcpy(word, &line1[j], *i - j);
		word[*i - j] = '\0';
		expanded = expand_quotes(word);
		if ((expanded[0] == '\'' && expanded[strlen(expanded) - 1] == '\'')
			|| (expanded[0] == '"' && expanded[strlen(expanded) - 1] == '"'))
		{
			memmove(expanded, expanded + 1, strlen(expanded) - 2);
			expanded[strlen(expanded) - 2] = '\0';
		}
		ft_lstadd_back(&token->infile, ft_lstnew(expanded));
	}
	else
		(*i)--;
}
