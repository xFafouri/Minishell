#include "../minishell.h"

void ft_remove_quotes(t_cmd *token)
{
	t_env *head;
	t_node *gc;

	head = token->addres_env;
	gc = token->addres_fd;
	while (head != NULL)
	{
		head->value = expand_quotes(head->value);
		head = head->next;
	}
}
void ft_add_qiotes(t_cmd *token)
{
	t_env *head;
	t_node *gc;
	char *s;

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

int ft_check_path_env(t_cmd *token)
{
	int i;
	int b;
	char *key;
	t_node *ft;

	i = 0;
	b = 1;
	ft = token->addres_fd;
	while (token->env[i] != NULL)
	{
		key = ft_substr(token->env[i], 0, ft_strlen_untile_char(token->env[i], '='), &ft);
		if (ft_strcmp(key, "PATH") == 0)
			b = 0;
		i++;
	}
	return (b);
}

void ft_env(t_cmd *token, t_node **gc)
{
    int i = 0;
    int has_args = 0;

    // Check if there are any arguments after "env"
    while (token->cmd[i] != NULL)
    {
        if (ft_strcmp(token->cmd[i], "env") != 0)
        {
            has_args = 1;
            break;
        }
        i++;
    }

    // If there are no arguments, print environment variables
    if (!has_args)
    {
        i = 0;
        while (token->env[i] != NULL)
        {
            printf("%s\n", token->env[i]);
            i++;
        }
        return;
    }

    int pid = fork();

    if (pid == -1)
    {
        perror("fork");
        token->status = 1;
        return;
    }
    else if (pid == 0)
    {
        char *path = NULL;

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

void ft_add_env(char *value, char *name, t_cmd *token)
{
	int i;
	char *join;
	char *key;
	char *value1;
	char *new_node;
	t_node *ft;

	i = 0;
	join = NULL;
	key = NULL;
	value1 = NULL;
	ft = token->addres_fd;
	new_node = ft_substr(name, 0, ft_strlen_untile_char(name, '+'), &ft);
	while (token->env[i] != NULL)
	{
		key = ft_substr(token->env[i], 0, ft_strlen_untile_char(token->env[i], '='), &ft);
		value1 = ft_strchr(token->env[i], '=');
		if (ft_strcmp(key, new_node) == 0)
		{
			if (name[ft_strlen(name) - 1] == '+')
				token->env[i] = ft_strjoin(&ft, token->env[i], value + 1);
			else
			{
				join = ft_strjoin(&ft, name, value);
				token->env[i] = ft_strdup(&ft, join);
			}
			return;
		}
		free(key);
		i++;
	}
	name = ft_substr(name, 0, ft_strlen_untile_char(name, '+'), &ft);
	join = ft_strjoin(&ft, name, value);
	token->env[i] = ft_strdup(&ft, join);
	token->env[i + 1] = NULL;
	free(join);
}
void ft_sort_env_list(t_cmd *token)
{
	t_env *head;
	t_node *ft;
	char *env_copy_name;
	char *env_copy_value;
	int swapped;
	t_env *current;

	if (token == NULL || token->addres_env == NULL)
		return;
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
				env_copy_name = ft_strdup(&ft, current->name);
				env_copy_value = ft_strdup(&ft, current->value);
				free(current->name);
				free(current->value);
				current->name = current->next->name;
				current->value = current->next->value;
				current->next->name = env_copy_name;
				current->next->value = env_copy_value;
				swapped = 1;
			}
			current = current->next;
		}
		if (!swapped)
			break;
	}
}

void ft_print_export(t_cmd *token)
{
	t_env *temp;

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

void ft_add_value_to_export(t_cmd *token, char *line)
{
	t_env *head;
	t_node *ft;
	char **str;
	char *name;
	char *value;
	char *env_copy;
	int i;
	t_env *new_node;
	t_env *current;
	t_env *prev;

	head = token->addres_env;
	ft = token->addres_fd;
	str = ft_split_qoute(line, ' ', &ft);
	ft_remove_quotes(token);
	name = NULL;
	value = NULL;
	env_copy = NULL;
	i = 1;
	while (str[i] != NULL)
	{
		env_copy = ft_strdup(&ft, str[i]);
		if (env_copy == NULL)
			return;
		name = ft_substr(env_copy, 0, ft_strlen_untile_char(env_copy, '='),
						 &ft);
		name = ft_substr(name, 0, ft_strlen_untile_char(env_copy, '>'),
						 &ft);
		name = ft_substr(name, 0, ft_strlen_untile_char(env_copy, '<'),
						 &ft);
		value = ft_strchr(env_copy, '=');
		name = expand_quotes(name);
		value = expand_quotes(value);
		// i have one eroore in this place export a+
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
			token->status = 1;
		}
		else
		{
			new_node = (t_env *)gc_malloc(&ft, sizeof(t_env));
			if (new_node == NULL)
				return;
			new_node->name = ft_substr(name, 0, ft_strlen_untile_char(name, '+'), &ft);
			new_node->value = ft_strdup(&ft, value);
			new_node->next = NULL;
			current = head;
			prev = NULL;
			while (current != NULL)
			{
				if (ft_strcmp(current->name, new_node->name) == 0)
				{
					if (new_node->value == NULL || new_node->value[0] == '\0')
					{
						new_node->value = ft_strdup(&ft, current->value);
					}
					else if (name[ft_strlen(name) - 1] == '+')
					{
						if (current->value == NULL || current->value[0] == '\0')
							current->value = ft_strjoin(&ft, current->value,
														new_node->value);
						else
							current->value = ft_strjoin(&ft, current->value,
														new_node->value + 1);
					}
					else
					{
						if (value != NULL)
							current->value = ft_strdup(&ft, value);
					}
					break;
				}
				prev = current;
				current = current->next;
			}
			if (value != NULL && value[0] != '\0')
				ft_add_env(value, name, token);
			if (current == NULL)
			{
				if (prev == NULL)
					token->addres_env = new_node;
				else
					prev->next = new_node;
			}
		}
		i++;
	}
	ft_add_qiotes(token);
}

void ft_export(t_cmd *token, char *line)
{
	if (token->cmd[1] == NULL)
	{
		ft_print_export(token);
		return;
	}
	ft_add_value_to_export(token, line);
}
