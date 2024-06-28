/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hfafouri <hfafouri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/22 15:14:18 by hfafouri          #+#    #+#             */
/*   Updated: 2024/05/22 16:33:22 by hfafouri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int ft_strcmp(char *s1, char *s2)
{
    int i = 0;

    if(s1 == NULL || s2 == NULL)
        return (0);
    while (s1[i] != '\0' && s2[i] != '\0' && s1[i] == s2[i])
        i++;
    return (s1[i] - s2[i]);
}

void syntax_error(int fd, char *s)
{
    write(fd, "bash: syntax error near unexpected token ", 42);
    write(2, "`", 1);
    ft_putstr_fd(s, fd);
    write(2, "'\n", 3);
    exit(2);
}

void input_validation(char *line)
{
    int i = 0;

    if (ft_strcmp(line, "&&") == 0)
        syntax_error(2, "&&");
    if (ft_strcmp(line, "||") == 0)
        syntax_error(2, "||");

    if (ft_strcmp(line, "&") == 0)
        syntax_error(2, "&");

    while (line[i])
    {
        if (line[i] == '|')
        {
            if(line[i + 1] != '\0')
                i++;
            while (line[i] == ' ' || line[i] == '\t')
                i++;
            if (line[i] == '|' || line[i] == '\0')
            {
                syntax_error(2, "|");
            }
        }
        else if (line[i] == '>')
        {
            i++;
            if (line[i] == '>')
                i++;
            while (line[i] == ' ' || line[i] == '\t')
                i++;
            if (line[i] == '\0' || line[i] == '>')
            {
                syntax_error(2, "newline");
            }
            else if(line[i] == '|')
                syntax_error(2, "|");
        }
        else if (line[i] == '<')
        {
            i++;
            if (line[i] == '<')
                i++;
            while (line[i] == ' ' || line[i] == '\t')
                i++;
            if (line[i] == '\0' || line[i] == '>' || line[i] == '|')
            {
                syntax_error(2, "newline");
            }
        }
        else if (line[i] == '&')
        {
            i++;
            if (line[i] == '&')
            {
                syntax_error(2, "&&");
            }
            while (line[i] == ' ' || line[i] == '\t')
                i++;
            if (line[i] == '\0')
            {
                syntax_error(2, "&");
            }
        }
        else if (line[i] == '|')
        {
            i++;
            if (line[i] == '|')
            {
                syntax_error(2, "||");
            }
            while (line[i] == ' ' || line[i] == '\t')
                i++;
            if (line[i] == '\0')
            {
                syntax_error(2, "|");
            }
        }
        i++;
    }
}