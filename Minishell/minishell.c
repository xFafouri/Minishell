#include "minishell.h"

void	split_pipe(char *cmd, t_cmd *env, t_node **gc)
{
	t_node	*link_cmd;
	char	**all_cmd;

	link_cmd = NULL;
	all_cmd = ft_split(cmd, '|', gc);
	while (*all_cmd != NULL)
	{
		ft_lstadd_back(&link_cmd, ft_lstnew(*all_cmd));
		(all_cmd)++;
	}
	ft_exc_cmd(link_cmd, gc, env);
}

int	main(int argc, char **argv, char **envp)
{
	t_node *gc;
	t_cmd ev;
	t_token *lexer;

	ev.env = envp;
	gc = NULL;
	char *line;
	t_token *temp = lexer;
	while (1)
	{
		line = readline("$");
		if (line != NULL)
		{
			// split_pipe(line, &ev, &gc);
			tokenisation(line, &lexer);
		}
		// char *tab[6];
		// tab[0] = "PIPE";
		// tab[1] = "WORD";
		// tab[2] = "REDIN";
		// tab[3] = "REDOUT";
		// tab[4] = "HERDOC";
		// tab[5] = "APPEND";
		// while (lexer)
		// {
		// 	printf("value = %s    | %s\n", lexer->value, tab[lexer->type]);
		// 	lexer = lexer->next;
		// }
	}
	while (temp)
	{
		printf("Type: %d, Value: %s\n", temp->type, temp->value);
		temp = temp->next;
	}

	return (0);
}