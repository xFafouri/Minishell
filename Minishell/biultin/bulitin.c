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

	str = NULL;
	str = ft_strtrim(line, " ");
	if (ft_strncmp(str, "pwd", 3) == 0)
	{
		ft_putstr_fd(getcwd(NULL, 0), 0);
		write(1, "\n", 1);
	}
}
void	ft_echo(char *line, t_cmd *token)
{
	int		i;
	int		n;
	int		j;
	char	*s;

	i = 1;
	n = 0;
	if (ft_strncmp(token->cmd[0], "echo", 3) == 0)
	{
		while (token->cmd[i] != NULL)
		{
			j = 0;
			if (token->cmd[i][0] == '-' && n == 0)
			{
				j++;
				while (token->cmd[i][j] != '\0')
				{
					if (token->cmd[i][j] == 'n')
						j++;
					else
					{
						printf("%s", token->cmd[i]);
						break ;
					}
				}
				i++;
			}
			else
			{
				n = 1;
				printf(" ");
				printf("%s", token->cmd[i]);
				i++;
			}
		}
	}
	printf("\n");
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
	}
	else
	{
		if (i == 1 || ft_strcmp((token->cmd)[1], "~") == 0)
		{
			path = getenv("HOME");
			if (!path)
			{
				ft_putstr_fd("cd: HOME not set\n", 2);
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
			perror(path);
	}
}

void	ft_exit(t_node **gc)
{
	ft_lstclear(gc);
	exit(1);
}