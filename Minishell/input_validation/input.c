/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hfafouri <hfafouri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/22 15:14:18 by hfafouri          #+#    #+#             */
/*   Updated: 2024/08/12 11:45:04 by hfafouri         ###   ########.fr       */
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

int input_validation(char *line)
{
    int     i;
    int     ero;
    int     in_dq;
    int     in_sq;
    char    quote;

    i = 0;
    ero = 0;
    in_dq = 0;
    in_sq = 0;
    if (ft_strcmp(line, "&&") == 0 || ft_strcmp(line, "||") == 0 || ft_strcmp(line, "&") == 0)
        syntax_error(2, line, &ero);
    while (line[i] == ' ' || line[i] == '\t')
        i++;
    if (line[i] == '|')
        syntax_error(2, "syntax error near unexpected token `|'", &ero);
    while (line[i] && !ero)
    {
        if (!in_sq && line[i] == '"')
            in_dq = !in_dq;
        else if (!in_dq && line[i] == '\'')
            in_sq = !in_sq;
        else if (!in_dq && !in_sq)
        {
            if (line[i] == '&')
            {
                if (line[i + 1] == '&')
                {
                    i += 2;
                    while (line[i] == ' ' || line[i] == '\t')
                        i++;
                    if (line[i] == '\0' || line[i] == '&')
                        syntax_error(2, "syntax error near unexpected token `&&'", &ero);
                    continue;
                }
                else
                    syntax_error(2, "syntax error near unexpected token `&'", &ero);
            }
            else if (line[i] == '|')
            {
                i++;
                while (line[i] == ' ' || line[i] == '\t')
                    i++;
                if (line[i] == '|' || line[i] == '\0')
                    syntax_error(2, "syntax error near unexpected token `|'", &ero);
                continue;
            }
            else if (line[i] == '>' || line[i] == '<')
            {
                char redir = line[i];
                i++;
                if (line[i] == redir)
                    i++;
                while (line[i] == ' ' || line[i] == '\t')
                    i++;
                if (line[i] == '\0' || line[i] == '>' || line[i] == '<' || line[i] == '|')
                    syntax_error(2, "syntax error near unexpected token `newline'", &ero);
                continue;
            }
        }
        i++;
    }
    if (in_dq)
        syntax_error(2, "syntax error: unclosed double quote", &ero);
    if (in_sq)
        syntax_error(2, "syntax error: unclosed single quote", &ero);
    return ero;
}

