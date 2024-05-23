#include "minishell.h"

void ft_fork_pipe(int **fd, int *id, int i, t_node **gc)
{
    if (pipe(fd[i]) == -1)
    {
        perror("pipe failed");
        ft_lstclear(gc);
        exit(1);
    }
    id[i] = fork();
    if (id[i] < 0)
    {
        perror("fork failed");
        (ft_lstclear(gc), exit(1));
    }
}

int count_cmd(t_node *cmd)
{
    int i;

    i = 0;
    while (cmd != NULL)
    {
        i++;
        cmd = cmd->next;
    }
    return (i);
}

// void malloc_fd_id(int *id, int **fd, int count, t_node **gc)
// {
//     int i;

//     i = 0;
//     id = gc_malloc(gc, (count * (sizeof(int))));
//     fd = gc_malloc(gc, (count * (sizeof(int *))));
//     while (i < count)
//     {
//         fd[i] = gc_malloc(gc, (2 * (sizeof(int))));
//         i++;
//     }
// }

void ft_exc_cmd(t_node *line, t_node **gc, t_cmd *env)
{
    int **fd;
    int *id;
    int count;
    int i;

    i = 0;

    printf("line = %s\n", (char *)line->data);
    count = count_cmd(line);
    // malloc_fd_id(&id, fd, count, gc);
    int n;

    n = 0;
    id = gc_malloc(gc, (count * (sizeof(int))));
    fd = gc_malloc(gc, (count * (sizeof(int *))));
    while (n < count)
    {
        fd[n] = gc_malloc(gc, (2 * (sizeof(int))));
        n++;
    }
    while (line != NULL)
    {
        tokenisation(line->data, gc);
        ft_pwd(line->data);
        ft_fork_pipe(fd, id, i, gc);
        if (id[i] == 0)
        {
            if (i == 0)
                ft_first_child((char *)line->data, fd, i, gc, env);
            else
                ft_last_child((char *)line->data, fd, i, gc, env);
        }
        else
        {
            if (i == 0)
                close(fd[i][1]);
            else
            {
                close(fd[i][1]);
                close(fd[i - 1][0]);
            }
            waitpid(id[0], 0, 0);
            waitpid(id[1], 0, 0);
        }
        line = line->next;
        i++;
    }
}
