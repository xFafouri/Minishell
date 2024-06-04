#include "minishell.h"

void	split_pipe(char *cmd, t_cmd *env, t_node **gc, t_node **envp)
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
	// for (t_node *cmd = link_cmd; cmd; cmd = cmd->next)
	// {
	// 	printf("cmd-> %s\n", cmd->data);
	// }
	ft_exc_cmd(link_cmd, gc, env, envp);
}

int	main(int argc, char **argv, char **envp)
{
	t_node *gc;
	t_node *all_env;
	t_node *envp_head;
	t_cmd ev;
	char *line;

	ev.env = envp;
	all_env = NULL;
	line = NULL;
	gc = NULL;
	while (*envp != NULL)
	{
		ft_lstadd_back(&all_env, ft_lstnew("hello"));
		envp++;
	}

	// for (t_node *tmp = all_env; tmp; tmp = tmp->next)
	// {
	// 	printf("tmp-> %s\n", tmp->data);
	// }
	while (1)
	{
		line = readline("$ ");
		if (line != NULL)
		{
			if (input_validation(line) != 1)
				split_pipe(line, &ev, &gc, &all_env);
			add_history(line);
			ft_lstclear(&gc);
		}
		else
			break ;
	}
	rl_clear_history();
	return (0);
}