#include "../minishell.h"

int	ft_isalpha(char *str)
{
	int	i;

	i = 0;
	if (!str)
		return (0);
	if (str[i] == '+')
		return (0);
	while (str[i] != '\0')
	{
		if (str[i] >= 'a' && str[i] <= 'z')
			i++;
		else if ((str[i] >= 'A' && str[i] <= 'Z'))
			i++;
		else if (str[i] == '_')
			i++;
		else if (str[i] == '+' && str[i + 1] == '\0')
			i++;
		else if ((i != 0) && (str[i] >= '0' && str[i <= '9']))
			i++;
		else
			return (0);
	}
	return (i);
}

void	ft_putchar_fd(char c, int fd)
{
	if (fd >= 0)
		write(fd, &c, 1);
}

void	ft_check_buldin(t_cmd *env, char *line, t_node **gc)
{
	if (ft_strcmp((env->cmd)[0], "pwd") == 0)
		ft_pwd(line, gc);
	else if (ft_strcmp((env->cmd)[0], "echo") == 0)
		ft_echo(line, env);
	else if (ft_strcmp((env->cmd)[0], "export") == 0)
		ft_export(env, line);
	else if (ft_strcmp((env->cmd)[0], "cd") == 0)
		ft_cd(line, env);
	else if (ft_strcmp((env->cmd)[0], "exit") == 0)
		ft_exit(gc);
	else if (ft_strcmp((env->cmd)[0], "env") == 0)
		ft_env(env);
}
int	ft_check_buldin1(t_cmd *env, char *line, t_node **gc)
{
	int	n;

	n = 1;
	if (ft_strcmp((env->cmd)[0], "pwd") == 0)
		n = 0;
	else if (ft_strcmp((env->cmd)[0], "echo") == 0)
		n = 0;
	else if (ft_strcmp((env->cmd)[0], "cd") == 0)
		n = 0;
	else if (ft_strcmp((env->cmd)[0], "exit") == 0)
		n = 0;
	else if (ft_strcmp((env->cmd)[0], "export") == 0)
		n = 0;
	else if (ft_strcmp((env->cmd)[0], "env") == 0)
		n = 0;
	return (n);
}
void	ft_putstr_fd(char *s, int fd)
{
	if (!s || fd < 0)
		return ;
	while (*s)
	{
		ft_putchar_fd(*s, fd);
		s++;
	}
}
void	ft_putendl_fd(char *s, int fd)
{
	ft_putstr_fd(s, fd);
	ft_putchar_fd('\n', fd);
}
