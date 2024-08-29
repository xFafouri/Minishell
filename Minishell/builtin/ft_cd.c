/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hfafouri <hfafouri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/29 18:28:50 by hfafouri          #+#    #+#             */
/*   Updated: 2024/08/29 18:29:31 by hfafouri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	check_home_key_export(t_cmd *token, char *str)
{
	t_env	*head;
	t_node	*ft;

	head = token->addres_env;
	while (head != NULL)
	{
		if (ft_strcmp(head->name, str) == 0)
			return (1);
		head = head->next;
	}
	return (0);
}

static char	*get_cd_path(t_cmd *token)
{
	int		i;
	char	*path;

	i = 0;
	while ((token->cmd)[i])
		i++;
	if (i == 1 || ft_strcmp((token->cmd)[1], "~") == 0)
	{
		path = getenv("HOME");
		if (check_home_key_export(token, "HOME") == 0)
			path = NULL;
		if (!path)
		{
			ft_putstr_fd("cd: HOME not set\n", 2);
			token->status = 1;
			return (NULL);
		}
		return (path);
	}
	return ((token->cmd)[1]);
}

static void	change_directory(char *path, t_cmd *token)
{
	char	*cwd;

	if (chdir(path) == 0)
	{
		cwd = getcwd(NULL, 0);
		if (cwd)
		{
			ft_setexport("PWD", cwd, token);
			free(cwd);
		}
		else
			perror("getcwd");
		token->status = 0;
	}
	else
	{
		perror(path);
		token->status = 1;
	}
}

void	ft_cd(char *line, t_cmd *token)
{
	int		i;
	char	*path;

	i = 0;
	while ((token->cmd)[i])
		i++;
	if (i > 2)
	{
		ft_putstr_fd((token->cmd)[0], 2);
		ft_putstr_fd(": too many arguments\n", 2);
		token->status = 1;
		return ;
	}
	path = get_cd_path(token);
	if (path)
		change_directory(path, token);
}
