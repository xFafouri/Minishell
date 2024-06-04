/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbourziq <sbourziq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/24 16:55:58 by hfafouri          #+#    #+#             */
/*   Updated: 2024/06/04 09:35:35 by sbourziq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	ft_pwd(char *line, t_node **gc)
{
	char	*str;

	str = NULL;
	str = ft_strtrim(line, " ", gc);
	if (ft_strncmp(str, "pwd", 3) == 0)
	{
		ft_putstr_fd(getcwd(NULL, 0), 0);
		write(1, "\n", 1);
	}
}

void	ft_echo(char *line)
{
	char	*s;

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

void	ft_cd(char *line, t_cmd *token)
{
	int		i;
	char	*cwd;

	i = 0;
	while ((token->cmd)[i])
		i++;
	if (i > 2)
	{
		ft_putstr_fd((token->cmd)[0], 2);
		ft_putstr_fd(": too many arguments\n", 2);
	}
	else
	{
		if (chdir((token->cmd)[1]) == 0)
		{
			cwd = getcwd(NULL, 0);
			if (cwd)
			{
				ft_putstr_fd(cwd, 1);
				free(cwd);
			}
			else
				perror("getcwd");
		}
		else
			perror((token->cmd)[1]);
	}
}
void	ft_exit(t_node **gc)
{
	ft_lstclear(gc);
	exit(1);
}
////////////////////////////////////////////////////////
int	ft_strlen2(char *str, int *n)
{
	int	i;

	i = 0;
	while (str[i] != '\0')
	{
		if (str[i] == '+' && str[i + 1] != '=')
		{
			*n = 1;
			break ;
		}
		i++;
	}
	return (i);
}

/**
 * @detials skfjslkfjslkfjsdlfkjsdlskfj
 * @param str: fgdkjhgkjhgf
 *
 */
void	ft_find(char *str, t_node **envp, int *n, t_node **gc)
{
	char	*ptr;
	char	*new_data;
	t_node	*envp_head;

	ptr = ft_strchr(str, '=');
	envp_head = *envp;
	if (ptr != NULL)
	{
		while (*envp != NULL)
		{
			if (strncmp((*envp)->data, str, ft_strlen2(str, n)) == 0)
			{
				if (*n == 1)
				{
					printf("found this so we append\n");
					new_data = ft_strjoin(gc, (*envp)->data, ptr + 1);
					if (new_data)
						(*envp)->data = new_data;
				}
			}
			*envp = (*envp)->next;
		}
	}
	*envp = envp_head;
}

void	ft_export(t_node **gc, t_cmd *token, t_node **envp)
{
	int i = 1;
	int n;
	t_node *envp_head = *envp;

	while (token->cmd[i])
	{
		n = 0;
		// ft_find(token->cmd[i], envp, &n, gc);
		if (n != 1)
		{
			printf("not found so we add %s\n", token->cmd[i]);
			ft_lstadd_back(envp, ft_lstnew(token->cmd[i]));
		}
		i++;
		*envp = envp_head;
	}
}