/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bulitin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hfafouri <hfafouri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/28 23:54:01 by hfafouri          #+#    #+#             */
/*   Updated: 2024/08/29 02:16:45 by hfafouri         ###   ########.fr       */
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

	str = NULL;
	str = ft_strtrim(line, " ", gc);
	if (ft_strncmp(str, "pwd", 3) == 0)
		printf("%s\n", getcwd(NULL, 0));
	token->status = 0;
}

static char	*ft_strstr(char *haystack, char *needle)
{
	int	i;
	int	j;

	i = 0;
	if (*needle == 0)
		return ((char *)haystack);
	while (haystack[i] != '\0')
	{
		j = 0;
		while (haystack[i + j] == needle[j] && needle[j] != '\0')
			j++;
		if (j == ft_strlen(needle))
		{
			while (haystack[i] != '=')
				i++;
			return ((char *)&haystack[i] + 1);
		}
		i++;
	}
	return (NULL);
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
		if (*end == '=' && strncmp(start, line, len) == 0 && (line[len] == '='
				|| line[len] == '\0'))
		{
			env->env_line = end + 1;
			return ;
		}
		i++;
	}
	env->env_line = NULL;
}
char	*expand_quotes(char *line, t_node **gc, t_cmd *token)
{
	int		len;
	char	c;
	char	*buffer;
	int j;
	int	k;

	len = ft_strlen(line);
	token->in_dq = 0;
	token->in_sq = 0;
	buffer = gc_malloc(gc, sizeof(char) * (len + 1));
	j = 0;
	k = 0;
	while (k < len)
	{
		c = line[k];
		if (c == '\'' && !token->in_dq)
			token->in_sq = !token->in_sq;
		else if (c == '"' && !token->in_sq)
			token->in_dq = !token->in_dq;
		else
			buffer[j++] = c;
		k++;
	}
	buffer[j] = '\0';
	return (buffer);
}

void	help_echo(t_cmd *token, int *i, int *j)
{
	if (token->is_flag && token->cmd[*i][0] == '-' && token->cmd[*i][1] == 'n')
	{
		*j = 1;
		while (token->cmd[*i][*j] == 'n')
			(*j)++;
		if (token->cmd[*i][*j] != '\0')
		{
			token->is_flag = 0;
			printf("%s", token->cmd[*i]);
		}
		else
			token->newline = 0;
	}
	else
	{
		token->is_flag = 0;
		if (*i > 1)
			printf(" ");
		printf("%s", token->cmd[*i]);
	}
}

void	ft_echo(char *line, t_cmd *token)
{
	int	i;
	int	j;

	i = 1;
	token->newline = 1;
	token->is_flag = 1;
	if (ft_strcmp(token->cmd[0], "echo") == 0)
	{
		while (token->cmd[i] != NULL)
		{
			j = 0;
			help_echo(token, &i, &j);
			i++;
		}
	}
	if (token->newline)
		printf("\n");
	token->status = 0;
}

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

long	ft_strtol(char *str, char **endptr, t_node **gc)
{
	long	result;
	int		i;
	int		sign;

	i = 0;
	result = 0;
	i = 0, sign = 1;
	while ((str[i] >= 9 && str[i] <= 13) || (str[i] == 32))
		i++;
	if (str[i] == '-' || str[i] == '+')
		sign = (str[i++] == '-') ? -1 : 1;
	while (str[i] != '\0')
	{
		if (str[i] >= '0' && str[i] <= '9')
		{
			if (result > (LONG_MAX - (str[i] - '0')) / 10)
			{
				*endptr = str + i;
				return ((sign > 0) ? LONG_MAX : LONG_MIN);
			}
			result = result * 10 + (str[i] - '0');
			i++;
		}
		else
			break ;
	}
	*endptr = str + i;
	return (result * sign);
}

void	ft_exit(t_node **gc, t_cmd *token)
{
	long nb = 0;
	char *endptr = NULL;

	ft_putendl_fd("exit", 2);

	if (token->cmd[1] == NULL)
	{
		ft_lstclear(gc);
		exit(token->status);
	}

	nb = ft_strtol(token->cmd[1], &endptr, gc);

	if (*endptr != '\0')
	{
		ft_putstr_fd("minishell: exit: ", 2);
		ft_putstr_fd(token->cmd[1], 2);
		ft_putendl_fd(": numeric argument required", 2);
		ft_lstclear(gc);
		exit(2);
	}
	if (token->cmd[2] != NULL)
	{
		ft_putendl_fd("minishell: exit: too many arguments", 2);
		token->status = 1;
		return ;
	}
	ft_lstclear(gc);
	exit((int)(nb & 0xFF));
}