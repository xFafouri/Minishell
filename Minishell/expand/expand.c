/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hfafouri <hfafouri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/27 21:28:34 by hfafouri          #+#    #+#             */
/*   Updated: 2024/08/28 03:53:36 by hfafouri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*process_var_name(char *line, char **ret, t_cmd *env)
{
	char	*var_name;
	int		var_name_len;

	var_name_len = 0;
	while (line[var_name_len] && (ft_isalnum(line[var_name_len])
			|| line[var_name_len] == '_'))
		var_name_len++;
	var_name = (char *)gc_malloc(env->gc_comand, var_name_len + 1);
	if (!var_name)
		return (line - 1);
	ft_strncpy(var_name, line, var_name_len);
	var_name[var_name_len] = '\0';
	check_env(env, var_name, ret);
	//free(var_name);
	return (line + var_name_len - 1);
}

char	*handle_variable_help(char *line, char **ret, t_cmd *env)
{
	if (*(line + 1) == '\0')
	{
		*ret = concatenate_char(*ret, '$');
		return (line);
	}
	if (*(line + 1) == '?')
		return (handle_exit(ret, env), line + 1);
	if (ft_isdigit(*(line + 1)))
	{
		line += 2;
		while (ft_isdigit(*line))
		{
			*ret = concatenate_char(*ret, *line);
			line++;
		}
		return (line - 1);
	}
	return (process_var_name(line + 1, ret, env));
}

char	*handle_variable(char *line, char **ret, t_quote_state *quote_state,
		t_cmd *env)
{
	if (*line == '$' && (!quote_state->in_single_quotes
			|| (quote_state->in_double_quotes && !quote_state->nested_quotes)))
	{
		return (handle_variable_help(line, ret, env));
	}
	*ret = concatenate_char(*ret, *line);
	return (line);
}

void	handle_help(char *line, char **result, t_quote_state *quote_state,
		t_cmd *env)
{
	while (*line)
	{
		if (*line == '\'' || *line == '\"')
		{
			toggle_quotes(*line, quote_state);
			*result = concatenate_char(*result, *line);
		}
		else if (*line == '$')
		{
			if (env->only_dollar || *(line + 1) == '$' || *(line + 1) == '\0'
				|| quote_state->in_single_quotes)
				*result = concatenate_char(*result, '$');
			else if (!quote_state->in_double_quotes)
				line = handle_variable(line, result, quote_state, env);
			else if (!ft_isalpha(*(line + 1)) && *(line + 1) != '_' && *(line
					+ 1) != '?')
				*result = concatenate_char(*result, '$');
			else
				line = handle_variable(line, result, quote_state, env);
		}
		else
			*result = concatenate_char(*result, *line);
		line++;
	}
}

char	*handle_dollar_sign(char *line, t_cmd *env, t_node **gc)
{
	char			*result;
	t_quote_state	quote_state;

	quote_state.in_single_quotes = 0;
	quote_state.in_double_quotes = 0;
	quote_state.nested_quotes = 0;
	result = gc_malloc(gc, 1);
	if (!result)
		return (NULL);
	result[0] = '\0';
	env->only_dollar = (ft_strcmp(line, "$") == 0);
	handle_help(line, &result, &quote_state, env);
	return (result);
}
