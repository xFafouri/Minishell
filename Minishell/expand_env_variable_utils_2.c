/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_env_variable_utils_2.c                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hfafouri <hfafouri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/23 01:01:50 by akaddour          #+#    #+#             */
/*   Updated: 2024/07/28 03:04:00 by hfafouri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*handle_variable_expansion(char *line, char **ret, t_quote_state *quote_state)
{
	char	*value;
	char	quote_char;

	if (*line == '$' && (!quote_state->in_single_quotes
			|| (quote_state->in_double_quotes && !quote_state->nested_quotes)))
	{
		if (*(line + 1) == '\"' || *(line + 1) == '\'')
		{
			quote_char = *(line + 1);
			line += 2;
			while (*line != quote_char && *line != '\0')
			{
				*ret = concatenate_char(*ret, *line);
				line++;
			}
			if (*line == '\0')
				*ret = concatenate_char(*ret, '$');
			return (line);
		}
		line = fetch_value(&value, line + 1, g_shell_data.environment_list);
		*ret = ft_strjoin(*ret, value);
		return (line);
	}
	*ret = concatenate_char(*ret, *line);
	return (line);
}
