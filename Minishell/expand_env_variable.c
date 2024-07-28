/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_env_variable.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hfafouri <hfafouri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/22 17:07:14 by akaddour          #+#    #+#             */
/*   Updated: 2024/07/28 03:02:32 by hfafouri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*shearch_and_replace(char *line)
{
	char			*ret;
	t_quote_state	quote_state;

	ret = "";
	quote_state.in_single_quotes = 0;
	quote_state.in_double_quotes = 0;
	quote_state.nested_quotes = 0;
	while (*line)
	{
		if (*line == '\'' || *line == '\"')
		{
			toggle_quotes(*line, &quote_state);
			ret = concatenate_char(ret, *line);
		}
		else
			line = handle_variable_expansion(line, &ret, &quote_state);
		line++;
	}
	return (ret);
}

static void	help_function(t_bool *is_var, t_list **head, char *arg)
{
	char	*expanded;
	char	**tokens;
	t_list	*new_node;

	*is_var = true;
	g_shell_data.simple_cmd->should_split = true;
	expanded = shearch_and_replace(arg);
	if (ft_strcmp(arg, expanded) != 0)
	{
		tokens = ft_split_2(expanded);
		while (*tokens)
		{
			new_node = ft_lstnew_2(*tokens);
			ft_lstadd_back(head, new_node);
			tokens++;
		}
	}
	else
	{
		new_node = ft_lstnew_2(arg);
		ft_lstadd_back(head, new_node);
	}
}

char	*process_arg(char *arg, t_list **head, t_bool *is_var)
{
	char	*tmp;
	t_list	*new_node;

	tmp = arg;
	while (*tmp)
	{
		if (*tmp == '$')
		{
			help_function(is_var, head, arg);
			break ;
		}
		tmp++;
	}
	if (!*is_var)
	{
		new_node = ft_lstnew_2(arg);
		ft_lstadd_back(head, new_node);
	}
	return (arg);
}

static char	**help_function_2(t_list *head)
{
	char	**new_args;
	int		i;

	new_args = gc_malloc(sizeof(char *) * (ft_lstsize(head) + 1));
	if (!new_args)
		return (NULL);
	i = 0;
	while (head)
	{
		new_args[i++] = ft_strdup((char *)head->content);
		head = head->next;
	}
	new_args[i] = NULL;
	return (new_args);
}

char	**expand_env_variable(char **args)
{
	int		i;
	t_bool	is_var;
	t_list	*head;
	char	**new_args;

	if (!args)
		return (NULL);
	i = 0;
	is_var = false;
	head = NULL;
	while (args[i])
	{
		is_var = false;
		args[i] = process_arg(args[i], &head, &is_var);
		i++;
	}
	new_args = help_function_2(head);
	i = -1;
	while (new_args[++i] && is_var)
	{
		if (new_args[i][0] == '\0')
			g_shell_data.simple_cmd->is_var = true;
	}
	return (new_args);
}
