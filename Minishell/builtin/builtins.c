/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hfafouri <hfafouri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/28 23:54:01 by hfafouri          #+#    #+#             */
/*   Updated: 2024/08/29 18:36:07 by hfafouri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	find_char(char *str, char c)
{
	int	i;
	int	b;

	i = 0;
	b = 0;
	if (!str)
		return (0);
	while (str[i] != '\0')
	{
		if (str[i] == c)
			b++;
		i++;
	}
	return (b);
}

int	ft_strlen_untile_char(char *str, char c)
{
	int	i;

	i = 0;
	while (str[i] != '\0')
	{
		if (str[i] == c)
			break ;
		i++;
	}
	return (i);
}

void	ft_pwd(char *line, t_node **gc, t_cmd *token)
{
	char	*str;
	char *line1;

	str = NULL;
	str = ft_strtrim(line, " ", gc);
	line1  = getcwd(NULL, 0);
	if (ft_strncmp(str, "pwd", 3) == 0)
		printf("%s\n", line1);
	free(line1);
	token->status = 0;
}


void	search_env(char *line, t_cmd *env)
{
	int	i;
	int	len;

	char *start, *end;
	i = 0;
	env->env_line = NULL;
	while (env->env[i])
	{
		start = env->env[i];
		end = start;
		while (*end != '=' && *end != '\0')
			end++;
		len = end - start;
		if (*end == '=' && ft_strncmp(start, line, len) == 0 && (line[len] == '='
				|| line[len] == '\0'))
		{
			env->env_line = end + 1;
			return ;
		}
		i++;
	}
	env->env_line = NULL;
}
