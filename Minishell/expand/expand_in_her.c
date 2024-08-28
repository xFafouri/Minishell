/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_in_her.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hfafouri <hfafouri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/27 21:28:32 by hfafouri          #+#    #+#             */
/*   Updated: 2024/08/28 03:52:22 by hfafouri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*process_var_name_here(char *line, char **ret, t_cmd *env)
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
	return (line + var_name_len);
}

void	count_dollars(t_cmd *env, char *line, int j, char **ret, t_node **gc)
{
	int	i;

	i = 0;
	if (j == 1)
	{
		env->dollar_count = 0;
		while (line[env->dollar_count] == '$')
			env->dollar_count++;
	}
	if (j == 2)
	{
		i = 0;
		while (i < env->dollar_count - 1)
		{
			*ret = concatenate_char(*ret, '$', gc);
			i++;
		}
	}
}

char	*handle_digit_case(char *line, char **ret, t_node **gc)
{
	while (ft_isdigit(*line))
	{
		*ret = concatenate_char(*ret, *line, gc);
		line++;
	}
	return (line);
}

char	*handle_variable_heredoc(char *line, char **ret, t_cmd *env, t_node **gc)
{
	count_dollars(env, line, 1, ret, gc);
	if (env->dollar_count == 1 && *(line + 1) != '?' && !ft_isalnum(*(line + 1))
		&& *(line + 1) != '_')
		return (handle_single_dollar(line, ret, gc));
	if (env->dollar_count > 0)
	{
		line += env->dollar_count;
		if (*line != '\0')
		{
			count_dollars(env, line - env->dollar_count, 2, ret, gc);
			if (*line == '?')
				return (handle_exit(ret, env), line + 1);
			if (ft_isdigit(*line))
				return (handle_digit_case(line + 1, ret,gc));
			return (process_var_name_here(line, ret, env));
		}
		count_dollars(env, line - env->dollar_count, 2, ret, gc);
		return (line);
	}
	*ret = concatenate_char(*ret, *line, gc);
	return (line + 1);
}

char	*handle_dollar_sign_heredoc(char *line, t_cmd *env, t_node **gc)
{
	char	*result;
	char	*expanded_line;

	result = gc_malloc(gc, 1);
	if (!result)
		return (NULL);
	result[0] = '\0';
	env->only_dollar = (ft_strcmp(line, "$") == 0);
	while (*line)
	{
		if (*line == '$')
		{
			expanded_line = handle_variable_heredoc(line, &result, env, gc);
			if (!expanded_line)
				return (NULL);
			line = expanded_line;
		}
		else
		{
			result = concatenate_char(result, *line, gc);
			line++;
		}
	}
	return (result);
}
