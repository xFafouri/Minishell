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

void	ft_pwd(char *line, t_node **gc)
{
	char	*str;
	t_cmd	*token;

	str = NULL;
	token = shell();
	str = ft_strtrim(line, " ", gc);
	if (ft_strncmp(str, "pwd", 3) == 0)
	{
		ft_putstr_fd(getcwd(NULL, 0), 0);
		write(1, "\n", 1);
	}
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
	int	j;
	int	count;

	j = 0;
	i = 0;
	count = 0;
	env->env_line = NULL;
	while (env->env[i])
	{
		j = 0;
		count = 0;
		while (env->env[i][j] != '=' && env->env[i][j] != '\0')
		{
			count++;
			j++;
		}
		if (ft_strncmp(env->env[i], line, count) == 0)
		{
			env->env_line = ft_strstr(env->env[i], line);
			break ;
		}
		i++;
	}
	if (!env->env_line)
	{
		env->env_line = NULL;
	}
}

char	*expand_quotes(char *line)
{
	int		len;
	int		in_dq;
	int		in_sq;
	char	c;
	char	*buffer;

	int i, j, k;
	len = ft_strlen(line);
	in_dq = in_double_quote(line, len);
	in_sq = in_single_quote(line, len);
	buffer = (char *)malloc(sizeof(char) * (len + 1));
	if (!buffer)
		return (NULL);
	j = 0;
	k = 0;
	while (k < len)
	{
		c = line[k];
		if (c == '\'' && !in_dq)
		{
			in_sq = !in_sq;
		}
		else if (c == '"' && !in_sq)
		{
			in_dq = !in_dq;
		}
		else
		{
			buffer[j++] = c;
		}
		k++;
	}
	buffer[j] = '\0';
	return (buffer);
}

void	ft_echo(char *line, t_cmd *token)
{
	int	i;
	int	j;
	int	newline;
	int	is_flag;

	i = 1;
	newline = 1;
	is_flag = 1;
	if (ft_strcmp(token->cmd[0], "echo") == 0)
	{
		while (token->cmd[i] != NULL)
		{
			j = 0;
			if (is_flag && token->cmd[i][0] == '-' && token->cmd[i][1] == 'n')
			{
				j = 1;
				while (token->cmd[i][j] == 'n')
					j++;
				if (token->cmd[i][j] != '\0')
				{
					is_flag = 0;
					printf("%s", token->cmd[i]);
				}
				else
				{
					newline = 0;
				}
			}
			else
			{
				is_flag = 0;
				if (i > 1)
					printf(" ");
				printf("%s", token->cmd[i]);
			}
			i++;
		}
	}
	if (newline)
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
void	ft_cd(char *line, t_cmd *token)
{
	int		i;
	char	*cwd;
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
	else
	{
		if (i == 1 || ft_strcmp((token->cmd)[1], "~") == 0)
		{
			path = getenv("HOME");
			if (check_home_key_export(token, "HOME") == 0)
				path = NULL;
			if (!path)
			{
				ft_putstr_fd("cd: HOME not set\n", 2);
				token->status = 1;
				return ;
			}
		}
		else
			path = (token->cmd)[1];
		if (chdir(path) == 0)
		{
			cwd = getcwd(NULL, 0);
			if (cwd)
			{
				ft_setexport("PWD", cwd, token);
				// free(cwd);
			}
			else
				perror("getcwd");
		}
		else
		{
			perror(path);
			token->status = 1;
			return ;
		}
	}
	token->status = 0;
}

long ft_strtol(char *str, char **endptr, t_node **gc)
{
    long result = 0;
    int i = 0;
    int sign = 1;

    while ((str[i] >= 9 && str[i] <= 13) || (str[i] == 32))
        i++;
    if (str[i] == '-' || str[i] == '+')
    {
        if (str[i] == '-')
            sign = -1;
        i++;
    }
    while (str[i] != '\0')
    {
        if (str[i] >= '0' && str[i] <= '9')
        {
            if (result > (LONG_MAX - (str[i] - '0')) / 10)
            {
                *endptr = str + i;
				if(sign > 0)
					return (LONG_MAX);
				else
					return(LONG_MIN);
            }
            result = result * 10 + (str[i] - '0');
            i++;
        }
        else
            break;
    }
    *endptr = str + i;
    return (result * sign);
}

void ft_exit(t_node **gc, t_cmd *token)
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
        return;
    }
    ft_lstclear(gc);
    exit((int)(nb & 0xFF));
}