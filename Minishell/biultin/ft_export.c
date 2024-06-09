#include "../minishell.h"

void ft_print_export(t_env *temp)
{
    while (temp != NULL)
    {
        printf("declare -x %s%s\n", temp->name, temp->value);
        temp = temp->next;
    }
}

void ft_add_value_to_export(t_cmd *token, char *line)
{
    t_env *head = token->addres_env;
    t_node *ft = token->addres_fd;
	t_env *new_node;
	char *name;
	char *value;
	char *env_copy;
	int i = 1;

	while (token->cmd[i] != NULL)
	{
		env_copy = strdup(token->cmd[i]);
		if (!env_copy)
			return;

		name = ft_substr(env_copy, 0, ft_strlen_untile_char(env_copy, '='), &ft);
		value = ft_strchr(env_copy, '=');

		new_node = gc_malloc(&ft, sizeof(t_env));
		if (!new_node)
		{
			free(env_copy);
			return;
		}

		new_node->name = ft_strdup(&ft, name);
		new_node->value = ft_strdup(&ft, value);
		new_node->next = head;
		head = new_node;

		free(env_copy);
		i++;
	}
}

void ft_export(t_cmd *token, char *line)
{
    t_env *temp = token->addres_env;
    if(token->cmd[1] == NULL)
        ft_print_export(temp);
    ft_add_value_to_export(token, line);
    temp = token->addres_env;
    while (temp != NULL)
    {
       printf("declare -x %s=%s\n", temp->name, temp->value);
        temp = temp->next;
    }
}
