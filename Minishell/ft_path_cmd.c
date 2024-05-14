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

char *ft_strchr(char *s, int c)
{
    int i;

    i = 0;
    while (s[i] != '\0' && s[i] != ' ' && s[i] != 9)
    {
        if (s[i] == (char)c)
            return ((char *)(s + i));
        else
            i++;
    }
    if (s[i] == (char)c)
        return ((char *)(s + i));
    return (NULL);
}

void ft_empty(char *av)
{
    if (av[0] == '.' && av[1] == '\0')
    {
        write(2, ".: filename argument required\n", 31);
        write(2, ".: usage: . filename [arguments]\n", 34);
        exit(2);
    }
    if (av[0] == '\0')
        exit(0);
}

char	**ft_helper_find_path(t_node **gc, t_cmd *ptr)
{
    char **commond_path;

    commond_path = NULL;
	while (*(ptr->env))
	{
		if (ft_strncmp(*(ptr->env), "PATH=", 5) == 0)
		{
			commond_path = ft_split((*(ptr->env) + 5), ':', gc);
			break ;
		}
		(ptr->env)++;
	}
	return (commond_path);
}

int ft_find_path(char **path, t_cmd *ptr, char *cmd, t_node **gc)
{
    int i;
    char **commond_path;

    i = 0;
    commond_path = ft_helper_find_path(gc, ptr);
    if (commond_path== NULL)
        return (0);
    while ((commond_path)[i] != NULL)
    {
        (commond_path)[i] = ft_strjoin(gc, (commond_path)[i], cmd);
        if (access((commond_path)[i], F_OK) == 0)
        {
            if (access((commond_path)[i], X_OK) == 0)
                *path = (commond_path)[i];
            else
                (printf("no permisiont \n"), exit(127));
        }
        i++;
    }
    return (1);
}

char *ft_check_path(char *str, t_node **gc, t_cmd *env)
{
    int i;
    char *cmd;
    char *path;

    i = 0;
    ft_empty(str);
    cmd = ft_check_space(str, gc);
    printf("my string is cmd =  %s\n\n", cmd);
    printf("first string is str = %s\n\n", str);
    if (cmd == NULL)
        return (NULL);
    if (ft_strchr(str, '/') != NULL || str[0] == '.')
        ft_access_to_path(gc, &path, str);
    else
    {
        if (ft_find_path(&path, env, cmd, gc) == 0)
        {
            if (access(str, X_OK) == 0)
                path = str;
            else
                 (printf("no permisoint \n"), exit(127));
        }
    }
    return (path);
}