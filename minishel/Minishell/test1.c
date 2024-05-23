#include "minishell.h"

int main(int argc, char **argv, char **envp)
{
    int file = 0;
    int i = 2;
    int n = 0;
    t_node *infile = NULL;
    t_node *outfile = NULL;
    int fd[2][2];
    char *cmd[] = {"cat", "-e", NULL};

    if (pipe(fd[0]) < 0)
        (printf("error pipe filed\n"), exit(1));
    if (argc > 1)
        ft_lstadd_back(&infile, ft_lstnew(argv[1]));
    while (i < argc)
    {
        ft_lstadd_back(&outfile, ft_lstnew(argv[i]));
        i++;
    }
    int id[2];

    while (n < 2)
    {
        id[n] = fork();
        if (id[n] == 0 && n == 0)
        {
            close(fd[0][0]);
            if (infile != NULL && infile->data != NULL)
            {
                while (infile->next != NULL)
                    infile = infile->next;
                file = open(infile->data, O_RDONLY);
                if (file < 0)
                    (printf("error infile %s\n", infile->data), exit(1));
                if ((dup2(file, 0) < 0))
                    (printf("error first dub2 filed\n"), exit(1));
                close(file);
            }
            if (outfile != NULL && outfile->data != NULL)
            {
                while (outfile->next != NULL)
                {
                    file = open(outfile->data, O_CREAT | O_TRUNC, 0666);
                    if (file < 0)
                        (printf("error outfile\n"), exit(1));

                    close(file);
                    outfile = outfile->next;
                }
                file = open(outfile->data, O_WRONLY | O_CREAT | O_TRUNC, 0666);
                if (file < 0)
                {
                    printf("error outfile\n");
                    exit(1);
                }
                if ((dup2(file, 1) < 0))
                {
                    printf("error second dup2\n");
                    exit(1);
                }
                close(file);
            }
            if (dup2(fd[0][1], 1) < 0)
                (printf("error first dub2 filed\n"), exit(1));
            close(fd[0][1]);
            if (execve("/usr/bin/cat", cmd, envp) < 0)
                (printf("execve filde\n"), exit(126));
        }
        else if (id[n] == 0 && n == 1)
        {
            close(fd[0][1]);
            if (dup2(fd[0][0], 0) < 0)
                (printf("error first dub2 filed\n"), exit(1));
            char *cmd1[] = {"sort", "-s", NULL};
            if (outfile != NULL && outfile->data != NULL)
            {
                while (outfile->next != NULL)
                {
                    file = open(outfile->data, O_CREAT | O_TRUNC, 0666);
                    if (file < 0)
                        (printf("error outfile\n"), exit(1));

                    close(file);
                    outfile = outfile->next;
                }
                file = open(outfile->data, O_WRONLY | O_CREAT | O_TRUNC, 0666);
                if (file < 0)
                {
                    printf("error outfile\n");
                    exit(1);
                }
                if ((dup2(file, 1) < 0) || dup2(fd[0][0], 0))
                {
                    printf("error second dup2\n");
                    exit(1);
                }
                close(file);
            }
            close(fd[0][1]);
            if (execve("/usr/bin/sort", cmd1, envp) < 0)
                (printf("execve filde\n"), exit(126));
        }
        else
        {
             if (n == 0)
                close(fd[n][1]);
            else
            {
                close(fd[n][1]);
                close(fd[n - 1][0]);
            }
            waitpid(id[n], 0, 0);
        }
        n++;
    }
    return 0;
}
