#include "minishell.h"

void ft_first_child(char *cmd, int **fd, int i, t_node **gc, t_cmd *env)
{
   int n;
   char *redirect_in;
   char *path;
   char **str;

   n = 0;
    redirect_in = ft_strchr_hlber(cmd, '<', &n);
    if(redirect_in != NULL)
    {
        close(fd[i][0]);
        str = ft_split(redirect_in, ' ', gc);
        path = ft_check_path(str[0], gc, env);
        if(path == NULL)
            (printf("commond not exit\n"), exit(127));
        if(dup2(fd[i][1], 1) == -1)
            (perror("dup2 failed\n"), ft_lstclear(gc), exit(1));
        close(fd[i][1]);
        if(execve(path, str, env->env))
            printf("execve filed\n\n");
        
    }
   
}

void ft_midll_child(t_node *cmd)
{
    // check if we have < || << | > || >>
    //check is the commond is build in yes or no
    // check is the path of the commnd is exit
    // check is we have double or sinlge couts '' ""
    // erroore os the commnd
}

void ft_last_child(char *cmd, int **fd, int i, t_node **gc, t_cmd *env)
{
    int n;
   char *redirect_in;
   char *path;
   char **str;

   n = 0;
    redirect_in = ft_strchr_hlber(cmd, '<', &n);
    if(redirect_in != NULL)
    {
        close(fd[i][1]);
        str = ft_split(redirect_in, ' ', gc);
        path = ft_check_path(str[0], gc, env);
        if(path == NULL)
            (printf("commond not exit\n"), exit(127));
        if(dup2(fd[i - 1][0], 0) == -1)
            (perror("dup2 failed\n"), ft_lstclear(gc), exit(1));
        close(fd[i][0]);
        if(execve(path, str, env->env))
            (printf("execve filed\n\n"),exit(126));
        
    }
}
