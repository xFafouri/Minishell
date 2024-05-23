/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hfafouri <hfafouri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 21:53:43 by hfafouri          #+#    #+#             */
/*   Updated: 2024/05/22 16:03:00 by hfafouri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void ft_putchar_fd(char c, int fd)
{
    if (fd >= 0)
        write(fd, &c, 1);
}

void ft_putstr_fd(char *s, int fd)
{
    if (!s || fd < 0)
        return;
    while (*s)
    {
        ft_putchar_fd(*s, fd);
        s++;
    }
}

void ft_pwd(char *line)
{

    if (ft_strncmp(line, "pwd", 3) == 0)
    {
        ft_putstr_fd(getcwd(NULL, 0), 0);
        write(1, "\n", 1);
    }
}

// void    ft_echo(char *line)
// {
    
// }

// void cd_cmd(char *line)
// {
//      if (chdir(line) != 0) 
//      {
//         perror("cd");
//         return ;
//     }
// }
// int main(int ac, char **av)
// {
//     cd_cmd(av[2]);
// }
