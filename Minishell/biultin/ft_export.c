/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hfafouri <hfafouri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/29 15:42:49 by hfafouri          #+#    #+#             */
/*   Updated: 2024/08/29 15:43:50 by hfafouri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	ft_remove_quotes(t_cmd *token)
{
	t_env	*head;
	t_node	*gc;

	head = token->addres_env;
	gc = token->addres_fd;
	while (head != NULL)
	{
		head->value = expand_quotes(head->value, &gc, token);
		head = head->next;
	}
}
void	ft_add_qiotes(t_cmd *token)
{
	t_env	*head;
	t_node	*gc;
	char	*s;

	head = token->addres_env;
	gc = token->addres_fd;
	while (head != NULL)
	{
		if (head->value[0] == '=')
		{
			s = gc_malloc(&gc, ft_strlen(head->value) + 3);
			s = "=\"";
			s = ft_strjoin(&gc, s, (head->value + 1));
			s = ft_strjoin(&gc, s, "\"");
			head->value = ft_strdup(&gc, s);
		}
		head = head->next;
	}
}
void	ft_env_no_args(t_cmd *token)
{
	int	i;

	i = 0;
	while (token->env[i] != NULL)
	{
		printf("%s\n", token->env[i]);
		i++;
	}
}

void	ft_env_exec(t_cmd *token, t_node **gc, int i)
{
	char	*path;

	path = NULL;
	ft_check_file(token, gc, token->her);
	signal(SIGINT, ft_signal_handler_cmd);
	signal(SIGQUIT, ft_signal_back_slash);
	path = ft_check_path(token->cmd[i], gc, token);
	if (path == NULL)
	{
		ft_putstr_fd(token->cmd[i], 2);
		ft_putstr_fd(": No such file or directory\n", 2);
		ft_lstclear(gc);
		exit(127);
	}
	if (execve(path, &token->cmd[i], token->env) < 0)
	{
		perror(path);
		ft_lstclear(gc);
		exit(126);
	}
}

void ft_env_fork(t_cmd *token, t_node **gc, int i)
{
    int pid = fork();
    if (pid == -1)
    {
        perror("fork");
        token->status = 1;
        return;
    }
    else if (pid == 0)
    {
        ft_env_exec(token, gc, i);
    }
    else
    {
        int status;
        waitpid(pid, &status, 0);
        if (WIFEXITED(status))
            token->status = WEXITSTATUS(status);
        else if (WIFSIGNALED(status))
            token->status = 128 + WTERMSIG(status);
        else
            token->status = 1;
    }
}

void ft_env(t_cmd *token, t_node **gc)
{
    int i = 0;
    int has_args = 0;

    while (token->cmd[i] != NULL)
    {
        if (ft_strcmp(token->cmd[i], "env") != 0)
        {
            has_args = 1;
            break;
        }
        i++;
    }
    if (!has_args)
    {
        ft_env_no_args(token);
        return;
    }
    ft_env_fork(token, gc, i);
}

void	ft_add_env_existing(char *value, char *name, t_cmd *token, int i,
		t_node *ft)
{
	char	*join;

	if (name[ft_strlen(name) - 1] == '+')
		token->env[i] = ft_strjoin(&ft, token->env[i], value + 1);
	else
	{
		join = ft_strjoin(&ft, name, value);
		token->env[i] = ft_strdup(&ft, join);
	}
}

void	ft_add_env(char *value, char *name, t_cmd *token)
{
	int		i;
	char	*join;
	char	*key;
	char	*new_node;
	t_node	*ft;

	i = 0;
	ft = token->addres_fd;
	new_node = ft_substr(name, 0, ft_strlen_untile_char(name, '+'), &ft);
	while (token->env[i] != NULL)
	{
		key = ft_substr(token->env[i], 0, ft_strlen_untile_char(token->env[i],
					'='), &ft);
		if (ft_strcmp(key, new_node) == 0)
		{
			ft_add_env_existing(value, name, token, i, ft);
			return ;
		}
		// free(key);
		i++;
	}
	name = ft_substr(name, 0, ft_strlen_untile_char(name, '+'), &ft);
	join = ft_strjoin(&ft, name, value);
	token->env[i] = ft_strdup(&ft, join);
	token->env[i + 1] = NULL;
}

void	ft_swap_env_nodes(t_env *current, t_env *next, t_node *ft)
{
	char	*env_copy_name;
	char	*env_copy_value;

	env_copy_name = ft_strdup(&ft, current->name);
	env_copy_value = ft_strdup(&ft, current->value);
	current->name = next->name;
	current->value = next->value;
	next->name = env_copy_name;
	next->value = env_copy_value;
}

void	ft_sort_env_list(t_cmd *token)
{
	t_env	*head;
	t_node	*ft;
	int		swapped;
	t_env	*current;

	if (token == NULL || token->addres_env == NULL)
		return ;
	head = token->addres_env;
	ft = token->addres_fd;
	while (1)
	{
		swapped = 0;
		current = head;
		while (current != NULL && current->next != NULL)
		{
			if (strcmp(current->name, current->next->name) > 0)
			{
				ft_swap_env_nodes(current, current->next, ft);
				swapped = 1;
			}
			current = current->next;
		}
		if (!swapped)
			break ;
	}
}

int	ft_validate_export_name(char *name, char *value)
{
	if (value == NULL || value[0] == '\0')
	{
		if (name[ft_strlen(name) - 1] == '+')
		{
			ft_putstr_fd("export: ", 2);
			if (name != NULL)
				ft_putstr_fd(name, 2);
			if (value != NULL)
				ft_putstr_fd(value, 2);
			ft_putstr_fd(": not a valid identifier\n", 2);
			return (0);
		}
	}
	if ((name == NULL || name[0] == '\0' || (ft_isalpha1(name) == 0)))
	{
		ft_putstr_fd("export: ", 2);
		if (name != NULL)
			ft_putstr_fd(name, 2);
		if (value != NULL)
			ft_putstr_fd(value, 2);
		if (name[0] == '-')
			ft_putstr_fd(": invalid option\n", 2);
		else
			ft_putstr_fd(": not a valid identifier\n", 2);
		return (0);
	}
	return (1);
}

void	ft_handle_plus_sign(t_env *current, t_env *new_node, t_node *ft)
{
	if (current->value == NULL || current->value[0] == '\0')
		current->value = ft_strjoin(&ft, current->value, new_node->value);
	else
		current->value = ft_strjoin(&ft, current->value, new_node->value + 1);
}

void	ft_update_existing_env(t_env *current, t_env *new_node, char *name,
		char *value, t_node *ft)
{
	if (new_node->value == NULL || new_node->value[0] == '\0')
	{
		new_node->value = ft_strdup(&ft, current->value);
	}
	else if (name[ft_strlen(name) - 1] == '+')
	{
		ft_handle_plus_sign(current, new_node, ft);
	}
	else
	{
		if (value != NULL)
			current->value = ft_strdup(&ft, value);
	}
}

void	ft_add_new_env(t_env **head, t_env *new_node, t_env *prev)
{
	if (prev == NULL)
		*head = new_node;
	else
		prev->next = new_node;
}

int	ft_serch_rid(char *line)
{
	int	i;

	i = 0;
	while (line[i] != '\0')
	{
		if (line[i] == '<' || line[i] == '>')
			return (0);
		i++;
	}
	return (1);
}

// Function to process the name of the environment variable
void ft_process_env_name(char **name, char *env_copy, t_node *ft)
{
    *name = ft_substr(env_copy, 0, ft_strlen_untile_char(env_copy, '='), &ft);
    if (ft_serch_rid(*name) == 0)
    {
        *name = ft_substr(*name, 0, ft_strlen_untile_char(env_copy, '>'), &ft);
        if ((*name == NULL || (*name)[0] == '\0'))
            return;
        *name = ft_substr(*name, 0, ft_strlen_untile_char(env_copy, '<'), &ft);
        if ((*name == NULL || (*name)[0] == '\0'))
            return;
    }
}

// Function to create a new environment node
t_env *ft_create_env_node(char *name, char *value, t_node *ft)
{
    t_env *new_node = (t_env *)gc_malloc(&ft, sizeof(t_env));
    if (new_node == NULL)
        return NULL;
    new_node->name = ft_substr(name, 0, ft_strlen_untile_char(name, '+'), &ft);
    new_node->value = ft_strdup(&ft, value);
    new_node->next = NULL;
    return new_node;
}

// Function to update or add an environment variable
void ft_update_or_add_env(t_cmd *token, t_env *new_node, char *name, char *value, t_node *ft)
{
    t_env *current = token->addres_env;
    t_env *prev = NULL;

    while (current != NULL)
    {
        if (ft_strcmp(current->name, new_node->name) == 0)
        {
            ft_update_existing_env(current, new_node, name, value, ft);
            break;
        }
        prev = current;
        current = current->next;
    }

    if (value != NULL && value[0] != '\0')
        ft_add_env(value, name, token);
    if (current == NULL)
        ft_add_new_env(&token->addres_env, new_node, prev);
}

// Main function to process environment variable
void ft_process_env_variable(t_cmd *token, char *env_copy, t_node *ft)
{
    char *name, *value;

    ft_process_env_name(&name, env_copy, ft);
    value = ft_strchr(env_copy, '=');

    name = expand_quotes(name, &ft, token);
    value = expand_quotes(value, &ft, token);

    if (!ft_validate_export_name(name, value))
    {
        token->status = 1;
        return;
    }

    t_env *new_node = ft_create_env_node(name, value, ft);
    if (new_node == NULL)
        return;

    ft_update_or_add_env(token, new_node, name, value, ft);
}
void	ft_add_value_to_export(t_cmd *token, char *line)
{
	t_node	*ft;
	char	**str;
	int		i;

	ft = token->addres_fd;
	str = ft_split_qoute(line, ' ', &ft);
	ft_remove_quotes(token);
	i = 1;
	while (str[i] != NULL)
	{
		// Check for redirection operators
		if (ft_strcmp(str[i], ">") == 0 || ft_strcmp(str[i], "<") == 0
			|| ft_strcmp(str[i], ">>") == 0 || ft_strcmp(str[i], "<<") == 0)
		{
			i = i + 2;
			continue ;
		}
		ft_process_env_variable(token, str[i], ft);
		i++;
	}
	ft_add_qiotes(token);
}
void	ft_print_export(t_cmd *token)
{
	t_env	*temp;

	temp = token->addres_env;
	ft_remove_quotes(token);
	ft_add_qiotes(token);
	while (temp != NULL)
	{
		if (temp->value != NULL)
			printf("declare -x %s%s\n", temp->name, temp->value);
		else
			printf("declare -x %s\n", temp->name);
		temp = temp->next;
	}
}
void	ft_export(t_cmd *token, char *line)
{
	if (token->cmd[1] == NULL)
	{
		ft_print_export(token);
		return ;
	}
	ft_add_value_to_export(token, line);
}