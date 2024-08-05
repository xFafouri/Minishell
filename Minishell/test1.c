#include "minishell.h"

void ft_restore_fds(t_fds *fds)
{
    if (fds->stdin_fd != -1)
    {
        dup2(fds->original_stdin, STDIN_FILENO);
        close(fds->stdin_fd);
        close(fds->original_stdin);
    }
    if (fds->stdout_fd != -1)
    {
        dup2(fds->original_stdout, STDOUT_FILENO);
        close(fds->stdout_fd);
        close(fds->original_stdout);
    }
}

t_fds ft_check_file1(t_cmd *env, t_node **gc, int her)
{
    t_fds fds = {-1, -1, -1, -1};
    
    fds.original_stdin = dup(STDIN_FILENO);
    fds.original_stdout = dup(STDOUT_FILENO);

    // Handle output file
    if (env->outfile != NULL && env->outfile->data != NULL)
    {
        fds.stdout_fd = open(env->outfile->data, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (fds.stdout_fd < 0)
        {
            perror("open failed");
            ft_lstclear(gc);
            exit(1);
        }
        if (dup2(fds.stdout_fd, STDOUT_FILENO) < 0)
        {
            perror("dup2 failed");
            close(fds.stdout_fd);
            ft_lstclear(gc);
            exit(1);
        }
    }
    
    // Handle input file
    if (env->infile != NULL && env->infile->data != NULL)
    {
        fds.stdin_fd = open(env->infile->data, O_RDONLY);
        if (fds.stdin_fd < 0)
        {
            perror("open failed");
            ft_lstclear(gc);
            exit(1);
        }
        if (dup2(fds.stdin_fd, STDIN_FILENO) < 0)
        {
            perror("dup2 failed");
            close(fds.stdin_fd);
            ft_lstclear(gc);
            exit(1);
        }
    }

    return fds;
}
