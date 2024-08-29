/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hfafouri <hfafouri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/29 01:51:21 by hfafouri          #+#    #+#             */
/*   Updated: 2024/08/29 18:49:33 by hfafouri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	count_heredocs(const char *line)
{
	int		count;
	char	*ptr;

	count = 0;
	ptr = (char *)line;
	while ((ptr = ft_strstr(ptr, "<<")) != NULL)
	{
		if (ptr == line || !ft_isalnum(*(ptr - 1)))
		{
			count++;
		}
		ptr += 2;
	}
	return (count);
}

t_env *create_env_node(char *env_copy, t_node **gc)
{
    t_env *new_node;
    char *name;
    char *value;

    name = ft_substr(env_copy, 0, ft_strlen_untile_char(env_copy, '='), gc);
    value = ft_strchr(env_copy, '=');
    new_node = gc_malloc(gc, sizeof(t_env));
    if (!new_node)
        return NULL;
    new_node->name = ft_strdup(gc, name);
    new_node->value = ft_strdup(gc, value);
    return new_node;
}

// Function to add a new node to the environment list
void add_env_node(t_env **head, t_env *new_node)
{
    new_node->next = *head;
    *head = new_node;
}

// Function to initialize the environment list
t_env *init_env_list(char **envp, t_node **gc)
{
    t_env *head = NULL;
    t_env *new_node;
    char *env_copy;
    int i = 0;

    while (envp[i] != NULL)
    {
        env_copy = ft_strdup(gc, envp[i]);
        if (!env_copy)
            return NULL;
        new_node = create_env_node(env_copy, gc);
        if (!new_node)
            return NULL;
        add_env_node(&head, new_node);
        i++;
    }
    return head;
}

void	split_pipe(char *cmd, t_cmd *env, t_node **gc)
{
	t_node	*link_cmd;
	char	**all_cmd;

	link_cmd = NULL;
	if (count_heredocs(cmd) >= 16)
	{
		printf("minishell: maximum here-document count exceeded\n");
		exit(2);
	}
	all_cmd = ft_split_qoute(cmd, '|', gc);
	env->gc_comand = gc;
	while (*all_cmd != NULL)
	{
		ft_lstadd_back(&link_cmd, ft_lstnew(*all_cmd, gc));
		(all_cmd)++;
	}
	ft_exc_cmd(link_cmd, gc, env);
}

int	has_non_space_chars(const char *str)
{
	while (*str)
	{
		if (!isspace((unsigned char)*str))
			return (1);
		str++;
	}
	return (0);
}

t_env	*init_environment(char ***envp, t_node **fd)
{
	t_env	*env_list;

	if (*envp == NULL || (*envp)[0] == NULL)
	{
		*envp = gc_malloc(fd, 4 * sizeof(char *));
		(*envp)[0] = ft_strdup(fd, "PWD=/nfs/homes/sbourziq");
		(*envp)[1] = ft_strdup(fd, "SHLVL=1");
		(*envp)[2] = ft_strdup(fd, "_=/usr/bin/env");
		(*envp)[3] = NULL;
	}
	env_list = init_env_list(*envp, fd);
	return (env_list);
}

void	setup_signals(void)
{
	signal(SIGINT, ft_signal_handler);
	signal(SIGQUIT, SIG_IGN);
}

void	process_input(char *line, t_cmd *ev)
{
	t_node	*command_gc;

	command_gc = NULL;
	ev->flag_signle = 0;
	if (input_validation(line, ev) != 1)
		split_pipe(line, ev, &command_gc);
	if (has_non_space_chars(line))
		add_history(line);
	ft_lstclear(&command_gc);
}

int	main(int argc, char **argv, char **envp)
{
	t_node	*gc;
	t_node	*fd;
	t_cmd	ev = {0};
	char	*line;

	gc = NULL;
	fd = NULL;
	ev.env = envp;
	ev.addres_env = init_environment(&envp, &fd);
	ev.addres_fd = fd;
	setup_signals();
	while (1)
	{
		ev.falg_to_exit = 0;
		ft_sort_env_list(&ev);
		line = readline(FG_YELLOW "$ " FG_GREEN);
		if (line != NULL)
		{
			process_input(line, &ev);
			free(line);
		}
		else
			break ;
	}
	printf("exit\n");
	ft_lstclear(&fd);
	rl_clear_history();
	return (ev.status);
}
