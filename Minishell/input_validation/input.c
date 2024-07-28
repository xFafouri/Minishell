/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hfafouri <hfafouri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/22 15:14:18 by hfafouri          #+#    #+#             */
/*   Updated: 2024/07/11 04:14:30 by hfafouri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_strcmp(char *s1, char *s2)
{
	int	i;

	i = 0;
	if (!s1 || !s2)
		return (1);
	while (s1[i] != '\0' && s2[i] != '\0' && s1[i] == s2[i])
		i++;
	return (s1[i] - s2[i]);
}

void	syntax_error(int fd, char *s, int *ero)
{
	write(fd, "syntax error near unexpected token ", 36);
	write(2, "`", 1);
	ft_putstr_fd(s, fd);
	write(2, "'\n", 3);
	*ero = 1;
}
int	double_quote_open(const char *line, int index)
{
	int	count;

	count = 0;
	for (int i = 0; i < index; i++)
	{
		if (line[i] == '\"' && (i == 0 || line[i - 1] != '\\'))
			count++;
	}
	return (count % 2);
}

int	single_quote_open(const char *line, int index)
{
	int	count;

	count = 0;
	for (int i = 0; i < index; i++)
	{
		if (line[i] == '\'' && (i == 0 || line[i - 1] != '\\'))
			count++;
	}
	return (count % 2);
}

int	input_validation(char *line)
{
	int		i;
	int		ero;
	int		in_dq;
	int		in_sq;
	char	quote;

	i = 0;
	ero = 0;
	in_dq = 0;
	in_sq = 0;
	if (ft_strcmp(line, "&&") == 0)
		syntax_error(2, "&&", &ero);
	if (ft_strcmp(line, "||") == 0)
		syntax_error(2, "||", &ero);
	if (ft_strcmp(line, "&") == 0)
		syntax_error(2, "&", &ero);
	// Check for pipe at the beginning
	while (line[i] == ' ' || line[i] == '\t')
		i++;
	if (line[i] == '|')
	{
		syntax_error(2, "Pipe at the beginning", &ero);
	}
	while (line[i] && !ero)
	{
		if (line[i] == '"')
		{
			if (!in_sq)
			{
				in_dq = !in_dq;
			}
		}
		else if (line[i] == '\'')
		{
			if (!in_dq)
			{
				in_sq = !in_sq;
			}
		}
		else if (line[i] == '&' && !in_dq && !in_sq)
		{
			if (line[i + 1] == '&')
			{
				i += 2;
				while (line[i] == ' ' || line[i] == '\t')
					i++;
				if (line[i] == '\0' || line[i] == '&')
					syntax_error(2, "&&", &ero);
			}
			else
			{
				syntax_error(2, "&", &ero);
			}
		}
		else if (line[i] == '|' && !in_dq && !in_sq)
		{
			i++;
			while (line[i] == ' ' || line[i] == '\t')
				i++;
			if (line[i] == '|' || line[i] == '\0')
				syntax_error(2, "|", &ero);
		}
		else if (line[i] == '>' && !in_dq && !in_sq)
		{
			i++;
			if (line[i] == '>')
			{
				i++;
			}
			while (line[i] == ' ' || line[i] == '\t')
				i++;
			if (line[i] == '\0' || line[i] == '>' || line[i] == '<'
				|| line[i] == '|')
				syntax_error(2, ">", &ero);
		}
		else if (line[i] == '<' && !in_dq && !in_sq)
		{
			i++;
			if (line[i] == '<')
			{
				i++;
				while (line[i] == ' ' || line[i] == '\t')
					i++;
				if (line[i] == '"' || line[i] == '\'')
				{
					quote = line[i];
					i++;
					while (line[i] && line[i] != quote)
						i++;
					if (line[i] == quote)
					{
						i++; 
						continue ;
					}
				}
			}
			while (line[i] == ' ' || line[i] == '\t')
				i++;
			if (line[i] == '\0' || line[i] == '>' || line[i] == '<'
				|| line[i] == '|')
				syntax_error(2, "<", &ero);
		}
		i++;
	}
	if (in_dq)
		syntax_error(2, "Unclosed double quote", &ero);
	if (in_sq)
		syntax_error(2, "Unclosed single quote", &ero);
	return (ero);
}

// int	input_validation(char *line)
// {
// 	int	i;
// 	int	ero;
// 	int	start;
// 	int	start;

// 	i = 0;
// 	ero = 0;
// 	if (ft_strcmp(line, "&&") == 0)
// 		syntax_error(2, "&&", &ero);
// 	if (ft_strcmp(line, "||") == 0)
// 		syntax_error(2, "||", &ero);
// 	if (ft_strcmp(line, "&") == 0)
// 		syntax_error(2, "&", &ero);
// 	while (line[i])
// 	{
// 		if (line[i] == '"')
// 		{
// 			if (!single_quote_open(line, i))
// 			{
// 				start = i;
// 				i++;
// 				while (line[i] && (line[i] != '"' || (i > 0 && line[i
// 							- 1] == '\\')))
// 					i++;
// 				if (line[i] != '"')
// 				{
// 					syntax_error(2, "Unclosed double quote", &ero);
// 				}
// 			}
// 		}
// 		else if (line[i] == '\'')
// 		{
// 			if (!double_quote_open(line, i))
// 			{
// 				start = i;
// 				i++;
// 				while (line[i] && (line[i] != '\'' || (i > 0 && line[i
// 						- 1] == '\\')))
// 					i++;
// 				if (line[i] != '\'')
// 				{
// 					syntax_error(2, "Unclosed single quote", &ero);
// 				}
// 			}
// 		}
// 		else if (line[i] == '|')
// 		{
// 			i++;
// 			while (line[i] == ' ' || line[i] == '\t')
// 				i++;
// 			if (line[i] == '|' || line[i] == '\0')
// 				syntax_error(2, "|", &ero);
// 		}
// 		else if (line[i] == '>')
// 		{
// 			i++;
// 			if (line[i] == '>')
// 				i++;
// 			while (line[i] == ' ' || line[i] == '\t')
// 				i++;
// 			if (line[i] == '\0' || line[i] == '>')
// 				syntax_error(2, "newline", &ero);
// 			else if (line[i] == '|')
// 				syntax_error(2, "|", &ero);
// 		}
// 		else if (line[i] == '<')
// 		{
// 			i++;
// 			if (line[i] == '<')
// 				i++;
// 			while (line[i] == ' ' || line[i] == '\t')
// 				i++;
// 			if (line[i] == '\0' || line[i] == '>' || line[i] == '|')
// 				syntax_error(2, "newline", &ero);
// 		}
// 		else if (line[i] == '&')
// 		{
// 			i++;
// 			if (line[i] == '&')
// 			{
// 				syntax_error(2, "&&", &ero);
// 				break ;
// 			}
// 			while (line[i] == ' ' || line[i] == '\t')
// 				i++;
// 			if (line[i] == '\0')
// 				syntax_error(2, "&", &ero);
// 		}
// 		else if (line[i] == ';' || line[i] == '!' || line[i] == '('
// 			|| line[i] == ')')
// 		{
// 			syntax_error(2, "Unknown char", &ero);
// 			break ;
// 		}
// 		i++;
// 	}
// 	if (single_quote_open(line, i))
// 		syntax_error(2, "Unclosed single quote", &ero);
// 	if (double_quote_open(line, i))
// 		syntax_error(2, "Unclosed double quote", &ero);
// 	return (ero);
// }
