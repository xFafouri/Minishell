/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_env_variable_utils.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: melachyr <melachyr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/23 01:01:50 by akaddour          #+#    #+#             */
/*   Updated: 2024/06/12 22:45:33 by melachyr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	toggle_quotes(char current_char, t_quote_state *quote_state)
{
	if (current_char == '\'')
	{
		if (!quote_state->in_double_quotes)
			quote_state->in_single_quotes = !quote_state->in_single_quotes;
		else
			quote_state->nested_quotes = !quote_state->nested_quotes;
	}
	else if (current_char == '\"')
	{
		if (!quote_state->in_single_quotes)
			quote_state->in_double_quotes = !quote_state->in_double_quotes;
		else
			quote_state->nested_quotes = !quote_state->nested_quotes;
	}
}

char	*retrieve_env_var(char *name, t_env *env)
{
	t_env	*var;

	var = env;
	while (var)
	{
		if (!ft_strcmp(var->key, name) && var->value)
			return (var->value);
		var = var->next;
	}
	return ("");
}

char	*concatenate_char(char *str, char c)
{
	char	*tmp;
	int		len;

	if (!str)
	{
		tmp = gc_malloc(2);
		tmp[0] = c;
		tmp[1] = 0;
		return (tmp);
	}
	len = ft_strlen(str);
	tmp = gc_malloc(len + 2);
	ft_strlcpy(tmp, str, len + 1);
	tmp[len] = c;
	tmp[len + 1] = 0;
	return (tmp);
}

char	*concatenate_name(char *name, char **line)
{
	/// we have peoblem in isalnum
	while (**line == '_' || isalnum(**line))
	{
		name = concatenate_char(name, **line);
		(*line)++;
	}
	return (name);
}

char	*fetch_value(char **value, char *line, t_env *env)
{
	char	*name;

	name = NULL;
	if (*line == '?')
	{
		*value = ft_itoa(g_shell_data.status);
		return (line);
	}
	if (!ft_isalpha(*line) && *line != '_')
	{
		if (ft_isdigit(*line) || *line == '*')
		{
			line++;
			*value = "";
			return (line - 1);
		}
		*value = "$";
		return (line - 1);
	}
	name = concatenate_name(name, &line);
	if (!name)
		name = "";
	*value = retrieve_env_var(name, env);
	return (line - 1);
}
