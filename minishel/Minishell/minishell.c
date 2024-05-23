#include "minishell.h"

void split_pipe(char *cmd, t_cmd *env, t_node **gc)
{
    t_node *link_cmd;
    char **all_cmd;

    link_cmd = NULL;
    all_cmd = ft_split(cmd, '|' , gc);
    while(*all_cmd != NULL)
    {
        ft_lstadd_back(&link_cmd, ft_lstnew(*all_cmd));
        (all_cmd)++;
    }
    ft_exc_cmd(link_cmd , gc, env);
}

int main(int argc, char **argv, char **envp)
{
    t_node *gc;
    t_cmd ev;

    ev.env = envp;
    gc = NULL;
    char *line;
    while(1) 
    {
        line = readline("$ ");
        if(line != NULL)
        {
            split_pipe(line, &ev, &gc);
        }
        else 
            break;
    }
    return 0;
}