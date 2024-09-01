/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_process_env_variable.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbourziq <sbourziq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/30 00:38:38 by sbourziq          #+#    #+#             */
/*   Updated: 2024/09/01 12:31:33 by sbourziq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_env	*ft_create_env_node(char *name, char *value, t_node *ft)
{
	t_env	*new_node;

	new_node = (t_env *)gc_malloc(&ft, sizeof(t_env));
	if (new_node == NULL)
		return (NULL);
	new_node->name = ft_substr(name, 0, ft_strlen_untile_char(name, '+'), &ft);
	new_node->value = ft_strdup(&ft, value);
	new_node->next = NULL;
	return (new_node);
}

// Function to update or add an environment variable
void	ft_update_or_add_env(t_cmd *token, t_env *new_node, char *name,
		char *value)
{
	t_env	*current;
	t_env	*prev;
	t_node	*ft;

	current = token->addres_env;
	ft = token->addres_fd;
	prev = NULL;
	token->temp = ft_strdup(&ft, value);
	while (current != NULL)
	{
		if (ft_strcmp(current->name, new_node->name) == 0)
		{
			ft_update_existing_env(current, new_node, name, token);
			break ;
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
void	ft_process_env_variable(t_cmd *token, char *env_copy, t_node *ft)
{
	t_env	*new_node;
	char	*name;
	char	*value;

	name = NULL;
	value = NULL;
	ft_process_env_name(&name, env_copy, ft);
	value = ft_strchr(env_copy, '=');
	name = expand_quotes(name, &ft, token);
	value = expand_quotes(value, &ft, token);
	if (!ft_validate_export_name(name, value))
	{
		token->status = 1;
		return ;
	}
	new_node = ft_create_env_node(name, value, ft);
	if (new_node == NULL)
		return ;
	ft_update_or_add_env(token, new_node, name, value);
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

// Function to process the name of the environment variable
void	ft_process_env_name(char **name, char *env_copy, t_node *ft)
{
	*name = ft_substr(env_copy, 0, ft_strlen_untile_char(env_copy, '='), &ft);
	if (ft_serch_rid(*name) == 0)
	{
		*name = ft_substr(*name, 0, ft_strlen_untile_char(env_copy, '>'), &ft);
		if ((*name == NULL || (*name)[0] == '\0'))
			return ;
		*name = ft_substr(*name, 0, ft_strlen_untile_char(env_copy, '<'), &ft);
		if ((*name == NULL || (*name)[0] == '\0'))
			return ;
	}
}
