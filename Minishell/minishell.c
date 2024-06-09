#include "minishell.h"

int ft_isspace(char c)
{
	if (c == ' ' || c == '\t')
		return (1);
	return (0);
}

void rmv_wsp(void *data)
{
	char *str;
	char *read;
	char *write;

	str = (char *)data;
	if (!str)
		return;
	read = str;
	write = str;
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

void ft_lstiter(t_node *lst, void (*f)(void *))
{
	if (!lst || !f)
		return;
	while (lst != NULL)
	{
		f(lst->data);
		lst = lst->next;
	}
}

t_env *init_env_list(char **envp, t_node **gc)
{
	t_env *head = NULL;
	t_env *new_node;
	char *name;
	char *value;
	char *env_copy;
	int i = 0;

	while (envp[i] != NULL)
	{
		env_copy = strdup(envp[i]);
		if (!env_copy)
			return NULL;

		name = ft_substr(env_copy, 0, ft_strlen_untile_char(env_copy, '='), gc);
		value = ft_strchr(env_copy, '=');

		new_node = gc_malloc(gc, sizeof(t_env));
		if (!new_node)
		{
			free(env_copy);
			return NULL;
		}

		new_node->name = ft_strdup(gc, name);
		new_node->value = ft_strdup(gc, value);
		new_node->next = head;
		head = new_node;

		free(env_copy);
		i++;
	}
	return head;
}

void split_pipe(char *cmd, t_cmd *env, t_node **gc)
{
	t_node *link_cmd;
	char **all_cmd;

	link_cmd = NULL;
	all_cmd = ft_split(cmd, '|', gc);
	while (*all_cmd != NULL)
	{
		ft_lstadd_back(&link_cmd, ft_lstnew(*all_cmd));
		(all_cmd)++;
	}
	ft_exc_cmd(link_cmd, gc, env);
}

int main(int argc, char **argv, char **envp)
{
	t_node *gc;
	t_node *fd;
	t_cmd ev;
	t_cmd *dd;
	int i;

	ev.env = envp;
	gc = NULL;
	fd = NULL;
	i = 0;
	char *line;
	t_env *env_list;
	env_list = init_env_list(envp, &fd);
	ev.addres_env = env_list;
	ev.addres_fd = fd;
	while (1)
	{
		line = readline("$ ");
		if (line != NULL)
		{
			split_pipe(line, &ev, &gc);
			add_history(line);
			ft_lstclear(&gc);
		}
		else
			break;
	}
	rl_clear_history();
	ft_lstclear(&gc);
	return (0);
}