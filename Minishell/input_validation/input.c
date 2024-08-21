/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hfafouri <hfafouri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/22 15:14:18 by hfafouri          #+#    #+#             */
/*   Updated: 2024/08/22 00:48:04 by hfafouri         ###   ########.fr       */
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

void	syntax_error(int fd, int *ero)
{
	write(fd, "syntax error\n", 13);
	*ero = 1;
}

int	input_validation(char *line)
{
	int		i;
	int		ero;
	int		in_dq;
	int		in_sq;
	char	quote;
	char	redir;

	i = 0;
	ero = 0;
	in_dq = 0;
	in_sq = 0;
	while (line[i] == ' ' || line[i] == '\t')
		i++;
	if (line[i] == '|')
		syntax_error(2, &ero);
	while (line[i] && !ero)
	{
		if (!in_sq && line[i] == '"')
			in_dq = !in_dq;
		else if (!in_dq && line[i] == '\'')
			in_sq = !in_sq;
		else if (!in_dq && !in_sq)
		{
			if (line[i] == '&' || (line[i] == '|' && line[i + 1] != '\0'
					&& line[i + 1] == '|') || line[i] == ';' || line[i] == '('
				|| line[i] == ')')
			{
				syntax_error(2, &ero);
				break ;
			}
			else if (line[i] == '|')
			{
				i++;
				while (line[i] == ' ' || line[i] == '\t')
					i++;
				if (line[i] == '\0')
					syntax_error(2, &ero);
				continue ;
			}
			else if (line[i] == '>' || line[i] == '<')
			{
				redir = line[i];
				i++;
				if (line[i] == redir)
					i++;
				while (line[i] == ' ' || line[i] == '\t')
					i++;
				if (line[i] == '\0' || line[i] == '>' || line[i] == '<'
					|| line[i] == '|')
					syntax_error(2, &ero);
				continue ;
			}
		}
		i++;
	}
	if (in_dq)
		syntax_error(2, &ero);
	if (in_sq)
		syntax_error(2, &ero);
	return (ero);
}
