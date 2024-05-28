/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hfafouri <hfafouri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/24 16:55:58 by hfafouri          #+#    #+#             */
/*   Updated: 2024/05/28 16:39:56 by hfafouri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void ft_pwd(char *line)
{

    if (ft_strncmp(line, "pwd", 3) == 0)
    {
        ft_putstr_fd(getcwd(NULL, 0), 0);
        write(1, "\n", 1);
    }
}

void ft_echo(char *line)
{
    char *s;
    if (ft_strncmp(line, "echo -n", 6) == 0)
    {
        s = line + 8;
        ft_putstr_fd(s, 1);
    }
    else if (ft_strncmp(line, "echo", 3) == 0)
    {
        s = line + 5;
        ft_putendl_fd(s, 1);
    }
}
