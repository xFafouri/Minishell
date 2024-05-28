/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_quotes.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hfafouri <hfafouri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/26 13:17:52 by hfafouri          #+#    #+#             */
/*   Updated: 2024/05/28 16:09:25 by hfafouri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int in_single_quote(char *line, int index)
{
    int i = 0;
    int in_sq = 0;
    while (i < index)
    {
        if (line[i] == '\'' && !in_double_quote(line, i))
        {
            in_sq = !in_sq;
        }
        i++;
    }
    return in_sq;
}

int in_double_quote(char *line, int index)
{
    int i = 0;
    int in_dq = 0;
    while (i < index)
    {
        if (line[i] == '"' && !in_single_quote(line, i))
        {
            in_dq = !in_dq;
        }
        i++;
    }
    return in_dq;
}

char *handle_quotes(char *line, t_node **gc)
{
    int i;
    int len;
    char c;
    char *buffer;
    int j = 0;
    len = ft_strlen(line);
    int in_dq = in_double_quote(line, len);
    int in_sq = in_single_quote(line, len);
    buffer = gc_malloc(gc, sizeof(char) * (len + 1));
    i = 0;
    while (i < len)
    {
        c = line[i];
        if (c == '\'' && !in_dq)
        {
            in_sq = !in_sq;
        }
        else if (c == '"' && !in_sq)
        {
            in_dq = !in_dq;
        }
        // else if (c == '\\' && in_dq)
        // {
        //     if (line[i + 1] == '"' || line[i + 1] == '\\' || line[i + 1] == '$')
        //     {
        //         buffer[j++] = line[++i];
        //     }
        //     else
        //     {
        //         buffer[j++] = '\\';
        //     }
        // }
        else
        {
            buffer[j++] = c;
        }
        i++;
    }
    buffer[j] = '\0';
    return buffer;
}
