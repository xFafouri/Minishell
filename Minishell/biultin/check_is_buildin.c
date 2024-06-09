#include "../minishell.h"
void ft_putchar_fd(char c, int fd)
{
    if (fd >= 0)
        write(fd, &c, 1);
}

int ft_check_buldin(t_cmd *env, char *line, t_node **gc)
{
    int n;

    n = 1;
    if (ft_strcmp((env->cmd)[0], "pwd") == 0)
    {
        ft_pwd(line, gc);
        n = 0;
    }
    else if (ft_strcmp((env->cmd)[0], "echo") == 0)
    {
         ft_echo(line, env);
        n = 0;
    }
    else if (ft_strcmp((env->cmd)[0], "export") == 0)
    {
        ft_export(env, line);
        n = 0;
    }
    else if (ft_strcmp((env->cmd)[0], "cd") == 0)
    {
        ft_cd(line, env);
        n = 0;
    }
    else if (ft_strcmp((env->cmd)[0], "exit") == 0)
    {
        n = 0;
        ft_exit(gc);
    }
    return (n);
}
int ft_check_buldin1(t_cmd *env, char *line, t_node **gc)
{
    int n;

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
    return (n);
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
void ft_putendl_fd(char *s, int fd)
{
    ft_putstr_fd(s, fd);
    ft_putchar_fd('\n', fd);
}
