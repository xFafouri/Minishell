#include "minishell.h"

int ft_isspace(char c)
{
    if (c == ' ' || c == '\t')
        return 1;
    return 0;
}

void rmv_wsp(void *data)
{
    char *str;

    str = (char *)data;
    if (!str)
        return;
    char *read = str;
    char *write = str;
    while (*read)
    {
        if (!ft_isspace(*read))
        {
            *write = *read;
            write++;
        }
        read++;
    }
    *write = '\0';
}

void	ft_lstiter(t_node *lst, void (*f)(void *))
{
	if (!lst || !f)
		return ;
	while (lst != NULL)
	{
		f(lst->data);
		lst = lst->next;
	}
}

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
    // ft_lstiter(link_cmd, rmv_wsp);

    // printf("link->cmd = -%s-\n", (char *)link_cmd->data);
    // printf("link->cmd = -%s-\n", (char *)link_cmd->next->data);
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