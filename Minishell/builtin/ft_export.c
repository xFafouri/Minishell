/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbourziq <sbourziq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/29 15:42:49 by hfafouri          #+#    #+#             */
/*   Updated: 2024/09/01 12:13:31 by sbourziq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	ft_handle_plus_sign(t_env *current, t_env *new_node, t_node *ft)
{
	if (current->value == NULL || current->value[0] == '\0')
		current->value = ft_strjoin(&ft, current->value, new_node->value);
	else
		current->value = ft_strjoin(&ft, current->value, new_node->value + 1);
}

void	ft_update_existing_env(t_env *current, t_env *new_node, char *name,
		t_cmd *token)
{
	t_node *ft;
	char *value;

	ft = token->addres_fd;
	value = token->temp;
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
