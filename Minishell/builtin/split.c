/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hfafouri <hfafouri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/31 18:54:15 by sbourziq          #+#    #+#             */
/*   Updated: 2024/09/01 16:12:48 by hfafouri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	handle_quote(const char **s, int *in_quote, char *quote_char)
{
	if (**s == '"' || **s == '\'')
	{
		if (*in_quote && **s == *quote_char)
			*in_quote = 0;
		else if (!*in_quote)
		{
			*in_quote = 1;
			*quote_char = **s;
		}
		return (1);
	}
	return (0);
}

static int	ft_count_tokens(const char *s, char c)
{
	int		count;
	int		in_quote;
	char	quote_char;

	count = 0;
	count = 0;
	in_quote = 0;
	quote_char = 0;
	if (!s)
		return (0);
	while (*s)
	{
		if (!handle_quote(&s, &in_quote, &quote_char) && *s == c && !in_quote)
		{
			count++;
			while (*s == c)
				s++;
			continue ;
		}
		s++;
	}
	return (count + (*s != c));
}

static char	*ft_get_token(char **s, char c, t_node **gc)
{
	char	*start;
	int		in_quote;
	char	quote_char;

	start = *s;
	in_quote = 0;
	quote_char = 0;
	while (**s)
	{
		if (!handle_quote((const char **)s, &in_quote, &quote_char) && **s == c
			&& !in_quote)
			break ;
		(*s)++;
	}
	return (ft_substr(start, 0, *s - start, gc));
}

char	**ft_split_qoute(char *s, char c, t_node **gc)
{
	int		i;
	char	**tokens;

	i = 0;
	if (!s)
		return (NULL);
	tokens = gc_malloc(gc, (ft_count_tokens(s, c) + 1) * sizeof(char *));
	if (!s || !tokens)
		return (NULL);
	while (*s)
	{
		while (*s == c)
			s++;
		if (!*s)
			break ;
		tokens[i++] = ft_get_token(&s, c, gc);
	}
	tokens[i] = NULL;
	return (tokens);
}
