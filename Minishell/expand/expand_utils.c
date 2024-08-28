/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hfafouri <hfafouri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/26 13:17:52 by hfafouri          #+#    #+#             */
/*   Updated: 2024/08/28 03:53:40 by hfafouri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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

char	*concatenate_char(char *str, char c, t_node **gc)
{
	size_t	len;
	char	*new_str;

	len = ft_strlen(str);
	new_str = gc_malloc(gc, len + 2);
	ft_strcpy1(new_str, str);
	new_str[len] = c;
	new_str[len + 1] = '\0';
	//free(str);
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
		//free(*ret);
		*ret = new_ret;
	}
}

void	handle_exit(char **ret, t_cmd *env)
{
	size_t	new_len;
	char	*new_ret;
	char	*exit_status_str;

	exit_status_str = ft_itoa(WIFEXITED(env->status));
	new_len = ft_strlen(*ret) + ft_strlen(exit_status_str) + 1;
	new_ret = gc_malloc(env->gc_comand, new_len);
	ft_strcpy1(new_ret, *ret);
	ft_strcat(new_ret, exit_status_str);
	//free(*ret);
	*ret = new_ret;
	//free(exit_status_str);
}

char	*handle_single_dollar(char *line, char **ret, t_node **gc)
{
	*ret = concatenate_char(*ret, '$', gc);
	return (line + 1);
}
