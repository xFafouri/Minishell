/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_path_cmd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hfafouri <hfafouri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/28 23:49:47 by hfafouri          #+#    #+#             */
/*   Updated: 2024/08/28 23:49:50 by hfafouri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_strncmp(char *str, char *path, int line)
{
	int	i;

	i = 0;
	if (str == NULL)
		return (1);
	while (i < line)
	{
		if (str[i] == path[i])
			i++;
		else
			return (1);
	}
	return (0);
}


void	ft_empty(char *av, t_node **gc)
{
	if (av == NULL)
		(ft_lstclear(gc), exit(0));
	if (av[0] == '.' && av[1] == '\0')
	{
		write(2, ".: filename argument required\n", 31);
		write(2, ".: usage: . filename [arguments]\n", 34);
		(ft_lstclear(gc), exit(2));
	}
	if (av[0] == '\0')
	{
		// write(2, ":command not found\n", 19);
		(ft_lstclear(gc), exit(127));
	}
}

char	**ft_helper_find_path(t_node **gc, t_cmd *ptr)
{
	int		i;
	char	**commond_path;

	i = 0;
	commond_path = NULL;
	if (ptr->flag == 1)
	{
		commond_path = ft_split("/nfs/homes/sbourziq/bin:/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin:/usr/games:/usr/local/games:/snap/bin",
				':', gc);
		return (commond_path);
	}
	while (ptr->env[i])
	{
		if (ft_strncmp(ptr->env[i], "PATH=", 5) == 0)
		{
			commond_path = ft_split((ptr->env[i] + 5), ':', gc);
			break ;
		}
		i++;
	}
	return (commond_path);
}

int	ft_find_path(char **path, t_cmd *ptr, char *cmd, t_node **gc)
{
	int		i;
	char	**commond_path;

	i = 0;
	commond_path = ft_helper_find_path(gc, ptr);
	if (commond_path == NULL)
		return (0);
	while ((commond_path)[i] != NULL)
	{
		(commond_path)[i] = ft_strjoin(gc, (commond_path)[i], cmd);
		if (access((commond_path)[i], F_OK) == 0)
		{
			if (access((commond_path)[i], X_OK) == 0)
				*path = (commond_path)[i];
			else
				(ft_lstclear(gc), exit(127));
		}
		i++;
	}
	return (1);
}

char	*ft_check_path(char *str, t_node **gc, t_cmd *env)
{
	int i;
	char *cmd;
	char *path;

	i = 0;
	ft_empty(str, gc);
	path = NULL;
	cmd = ft_check_space(str, gc);
	if (cmd == NULL)
		return (NULL);
	if (ft_strchr(str, '/'))
		ft_access_to_path(gc, &path, str);
	else
	{
		if (ft_find_path(&path, env, cmd, gc) == 0)
		{
			if (access(str, X_OK) == 0)
				path = str;
			else
				ft_access_to_path(gc, &path, str);
		}
	}
	return (path);
}