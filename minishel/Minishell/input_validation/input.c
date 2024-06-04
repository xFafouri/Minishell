/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbourziq <sbourziq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/22 15:14:18 by hfafouri          #+#    #+#             */
/*   Updated: 2024/05/29 10:11:35 by sbourziq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_strcmp(char *s1, char *s2)
{
	int	i;

	i = 0;
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

int	input_validation(char *line)
{
	int	i;
	int	ero;

	i = 0;
	ero = 0;
	if (ft_strcmp(line, "&&") == 0)
		syntax_error(2, "&&", &ero);
	if (ft_strcmp(line, "||") == 0)
		syntax_error(2, "||", &ero);
	if (ft_strcmp(line, "&") == 0)
		syntax_error(2, "&", &ero);
	while (line[i])
	{
		if (line[i] == '|')
		{
			if (line[i + 1] != '\0')
				i++;
			while (line[i] == ' ' || line[i] == '\t')
				i++;
			if (line[i] == '|' || line[i] == '\0')
				syntax_error(2, "|", &ero);
		}
		else if (line[i] == '>')
		{
			i++;
			if (line[i] == '>')
				i++;
			while (line[i] == ' ' || line[i] == '\t')
				i++;
			if (line[i] == '\0' || line[i] == '>')
				syntax_error(2, "newline", &ero);
			else if (line[i] == '|')
				syntax_error(2, "|", &ero);
		}
		else if (line[i] == '<')
		{
			i++;
			if (line[i] == '<')
				i++;
			while (line[i] == ' ' || line[i] == '\t')
				i++;
			if (line[i] == '\0' || line[i] == '>' || line[i] == '|')
				syntax_error(2, "newline", &ero);
		}
		else if (line[i] == '&')
		{
			i++;
			if (line[i] == '&')
				syntax_error(2, "&&", &ero);
			while (line[i] == ' ' || line[i] == '\t')
				i++;
			if (line[i] == '\0')
				syntax_error(2, "&", &ero);
		}
		else if (line[i] == '|')
		{
			i++;
			if (line[i] == '|')
				syntax_error(2, "||", &ero);
			while (line[i] == ' ' || line[i] == '\t')
				i++;
			if (line[i] == '\0')
				syntax_error(2, "|", &ero);
		}
		i++;
	}
	return (ero);
}
