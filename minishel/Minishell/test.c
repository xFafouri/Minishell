#include "minishell.h"

int main(int argc, char **argv, char **envp)
{
    int file = 0;
    int i = 3;
    t_node *infile = NULL;
    t_node *outfile = NULL;
    char *cmd[] = {"cat", "-e", NULL};

    char *str = "ls <ifile -la > out1 > out2 > out3";

    if (argc > 1)
    {
        ft_lstadd_back(&infile, ft_lstnew(argv[1]));
        ft_lstadd_back(&infile, ft_lstnew(argv[2]));
    }
    while (i < argc)
    {
        ft_lstadd_back(&outfile, ft_lstnew(argv[i]));
        i++;
    }
    int id = fork();
    if (id == 0)
    {
        if (infile != NULL && infile->data != NULL)
        {
            while (infile->next != NULL)
                infile = infile->next;
            file = open(infile->data, O_RDONLY);
            if (file < 0)
                (printf("error infile %s\n", infile->data), exit(1));
            if (dup2(file, 0) < 0)
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
            if (dup2(file, 1) < 0)
            {
                printf("error second dup2\n");
                exit(1);
            }
            close(file);
        }
        if (execve("/usr/bin/cat", cmd, envp) < 0)
            (printf("execve filde\n"), exit(126));
    }
    else
        waitpid(id, 0, 0);
    return 0;
}