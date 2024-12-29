/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hfafouri <hfafouri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/31 18:49:06 by sbourziq          #+#    #+#             */
/*   Updated: 2024/09/01 16:10:07 by hfafouri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	toggle_quotes(char current_char, t_cmd *env)
{
	if (current_char == '\'')
	{
		if (!env->quote_state->in_double)
			env->quote_state->in_single = !env->quote_state->in_single;
		else
			env->quote_state->nested = !env->quote_state->nested;
	}
	else if (current_char == '\"')
	{
		if (!env->quote_state->in_single)
			env->quote_state->in_double = !env->quote_state->in_double;
		else
			env->quote_state->nested = !env->quote_state->nested;
	}
}

char	*concatenate_char(char *str, char c, t_node **gc)
{
	size_t	len;
	char	*new_str;

	len = ft_strlen(str);
	new_str = gc_malloc(gc, len + 2);
	ft_strcpy1(new_str, str);
	new_str[len] = c;
	new_str[len + 1] = '\0';
	return (new_str);
}

void	check_env(t_cmd *env, char *var_name, char **ret)
{
	size_t	new_len;
	char	*new_ret;

	new_len = 0;
	search_env(var_name, env);
	if (env->env_line)
	{
		new_len = ft_strlen(*ret) + ft_strlen(env->env_line) + 1;
		new_ret = gc_malloc(env->gc_comand, new_len);
		if (!new_ret)
			return ;
		ft_strcpy1(new_ret, *ret);
		ft_strcat(new_ret, env->env_line);
		*ret = new_ret;
	}
}

void	handle_exit(char **ret, t_cmd *env, t_node **gc)
{
	size_t	new_len;
	char	*new_ret;
	char	*exit_status_str;

	exit_status_str = ft_itoa((env->status), gc);
	new_len = ft_strlen(*ret) + ft_strlen(exit_status_str) + 1;
	new_ret = gc_malloc(env->gc_comand, new_len);
	ft_strcpy1(new_ret, *ret);
	ft_strcat(new_ret, exit_status_str);
	*ret = new_ret;
}

char	*handle_single_dollar(char *line, char **ret, t_node **gc)
{
	*ret = concatenate_char(*ret, '$', gc);
	return (line + 1);
}
