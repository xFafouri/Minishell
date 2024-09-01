/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hfafouri <hfafouri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/27 21:28:34 by hfafouri          #+#    #+#             */
/*   Updated: 2024/09/01 16:11:08 by hfafouri         ###   ########.fr       */
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
	return (line + var_name_len - 1);
}

char	*handle_variable_help(char *line, char **ret, t_cmd *env, t_node **gc)
{
	if (*(line + 1) == '\0')
	{
		*ret = concatenate_char(*ret, '$', gc);
		return (line);
	}
	if (*(line + 1) == '?')
		return (handle_exit(ret, env, gc), line + 1);
	if (ft_isdigit(*(line + 1)))
	{
		line += 2;
		while (ft_isdigit(*line))
		{
			*ret = concatenate_char(*ret, *line, gc);
			line++;
		}
		return (line - 1);
	}
	return (process_var_name(line + 1, ret, env));
}

char	*handle_variable(char *line, char **ret, t_cmd *env, t_node **gc)
{
	if (*line == '$' && (!env->quote_state->in_single
			|| (env->quote_state->in_double
				&& !env->quote_state->nested)))
	{
		return (handle_variable_help(line, ret, env, gc));
	}
	*ret = concatenate_char(*ret, *line, gc);
	return (line);
}

void	handle_help(char *line, char **result, t_cmd *env, t_node **gc)
{
	while (*line)
	{
		if (*line == '\'' || *line == '\"')
		{
			toggle_quotes(*line, env);
			*result = concatenate_char(*result, *line, gc);
		}
		else if (*line == '$')
		{
			if (env->only_dollar || *(line + 1) == '$' || *(line + 1) == '\0'
				|| env->quote_state->in_single)
				*result = concatenate_char(*result, '$', gc);
			else if (!env->quote_state->in_double)
				line = handle_variable(line, result, env, gc);
			else if (!ft_isalpha(*(line + 1)) && *(line + 1) != '_' && *(line
					+ 1) != '?')
				*result = concatenate_char(*result, '$', gc);
			else
				line = handle_variable(line, result, env, gc);
		}
		else
			*result = concatenate_char(*result, *line, gc);
		line++;
	}
}

char	*handle_dollar_sign(char *line, t_cmd *env, t_node **gc)
{
	char			*result;
	t_quote_state	*quote_state;

	quote_state = gc_malloc(gc, sizeof(t_quote_state));
	quote_state->in_single = 0;
	quote_state->in_double = 0;
	quote_state->nested = 0;
	env->quote_state = quote_state;
	result = gc_malloc(gc, 1);
	if (!result)
		return (NULL);
	result[0] = '\0';
	env->only_dollar = (ft_strcmp(line, "$") == 0);
	handle_help(line, &result, env, gc);
	return (result);
}
