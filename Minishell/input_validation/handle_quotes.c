/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_quotes.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hfafouri <hfafouri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/26 13:17:52 by hfafouri          #+#    #+#             */
/*   Updated: 2024/08/21 20:39:18 by hfafouri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	in_single_quote(char *line, int index)
{
	int	i;
	int	in_sq;

	i = 0;
	in_sq = 0;
	while (i < index)
	{
		if (line[i] == '\'' && !in_double_quote(line, i))
		{
			in_sq = !in_sq;
		}
		i++;
	}
	return (in_sq);
}

int	in_double_quote(char *line, int index)
{
	int	i;
	int	in_dq;

	i = 0;
	in_dq = 0;
	while (i < index)
	{
		if (line[i] == '"' && !in_single_quote(line, i))
		{
			in_dq = !in_dq;
		}
		i++;
	}
	return (in_dq);
}

void	handle_quotes(t_cmd *cmd, t_node **gc)
{
	int		i;
	int		j;
	int		len;
	int		in_dq;
	int		in_sq;
	char	c;
	char	*buffer;
	char	*line;
	int		k;

	cmd->aft_dol_dq = 0;
	cmd->dollar = NULL;
	i = 0;
	while (cmd->cmd[i])
	{
		line = cmd->cmd[i];
		len = ft_strlen(line);
		in_dq = in_double_quote(line, len);
		in_sq = in_single_quote(line, len);
		buffer = (char *)gc_malloc(gc, sizeof(char) * (len + 1));
		if (!buffer)
			return ;
		j = 0;
		k = 0;
		cmd->aft_dol_dq = 0;
		while (k < len)
		{
			c = line[k];
			if (c == '\'' && !in_dq && !cmd->aft_dol_dq)
				in_sq = !in_sq;
			else if (c == '"' && !in_sq && !cmd->aft_dol_dq)
				in_dq = !in_dq;
			else
				buffer[j++] = c;
			k++;
		}
		buffer[j] = '\0';
		cmd->cmd[i] = buffer;
		i++;
	}
}

static int	count(long long nb)
{
	int	i;

	i = 1;
	if (nb < 0)
	{
		i++;
		nb = -nb;
	}
	while (nb > 9)
	{
		nb = nb / 10;
		i++;
	}
	return (i);
}

char	*ft_itoa(int n)
{
	char		*str;
	long long	nbr;
	int			i;
	int			nb;

	nbr = n;
	nb = count(n);
	str = (char *)malloc(nb + 1);
	if (str == NULL)
		return (NULL);
	if (nbr < 0)
	{
		nbr = -nbr;
	}
	i = nb - 1;
	while (i >= 0)
	{
		str[i--] = nbr % 10 + '0';
		nbr = nbr / 10;
	}
	if (n < 0)
		str[0] = '-';
	str[nb] = '\0';
	return (str);
}

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

char	*concatenate_char(char *str, char c)
{
	size_t	len;
	char	*new_str;

	len = strlen(str);
	new_str = malloc(len + 2);
	if (!new_str)
		return (NULL);
	strcpy(new_str, str);
	new_str[len] = c;
	new_str[len + 1] = '\0';
	free(str);
	return (new_str);
}

char	*handle_variable_expansion(char *line, char **ret,
		t_quote_state *quote_state, t_cmd *env)
{
	char	var_name[256] = {0};
	char	*value;
	int		var_name_start;
	size_t	new_len;
	char	*new_ret;
	char	*exit_status_str;

	if (*line == '$' && (!quote_state->in_single_quotes
			|| (quote_state->in_double_quotes && !quote_state->nested_quotes)))
	{
		if (*(line + 1) == '\0')
		{
			*ret = concatenate_char(*ret, '$');
			return (line);
		}
		if (*(line + 1) == '?')
		{
			exit_status_str = ft_itoa(WIFEXITED(env->status));
			new_len = strlen(*ret) + strlen(exit_status_str) + 1;
			new_ret = malloc(new_len);
			if (!new_ret)
			{
				free(exit_status_str);
				return (NULL);
			}
			strcpy(new_ret, *ret);
			strcat(new_ret, exit_status_str);
			free(*ret);
			*ret = new_ret;
			free(exit_status_str);
			return (line + 1);
		}
		if (isdigit(*(line + 1)))
		{
			line+=2;
			while (isdigit(*line))
			{
				*ret = concatenate_char(*ret, *line);
				line++;
			}
			return (line - 1);
		}
		var_name_start = 0;
		line++;
		while (*line != '\0' && (isalnum(*line) || *line == '_'))
		{
			var_name[var_name_start++] = *line++;
		}
		var_name[var_name_start] = '\0';
		search_env(var_name, env);
		if (env->env_line)
		{
			new_len = strlen(*ret) + strlen(env->env_line) + 1;
			new_ret = malloc(new_len);
			if (!new_ret)
				return (NULL);
			strcpy(new_ret, *ret);
			strcat(new_ret, env->env_line);
			free(*ret);
			*ret = new_ret;
		}
		return (line - 1);
	}
	*ret = concatenate_char(*ret, *line);
	return (line);
}


char *handle_dollar_sign(char *line, t_cmd *env, t_node **gc)
{
    char            *result;
    t_quote_state   quote_state;
    int             only_dollar;

    quote_state.in_single_quotes = 0;
    quote_state.in_double_quotes = 0;
    quote_state.nested_quotes = 0;
    result = malloc(1);
    if (!result)
        return (NULL);
    result[0] = '\0';
    only_dollar = (strcmp(line, "$") == 0);
    while (*line)
    {
        if (*line == '\'' || *line == '\"')
        {
            toggle_quotes(*line, &quote_state);
            result = concatenate_char(result, *line);
        }
        else if (*line == '$')
        {
            if (only_dollar || *(line + 1) == '$' || *(line + 1) == '\0' || 
                quote_state.in_single_quotes)
                result = concatenate_char(result, '$');
            else if (!quote_state.in_double_quotes)
                line = handle_variable_expansion(line, &result, &quote_state, env);
            else if (!isalpha(*(line + 1)) && *(line + 1) != '_' && *(line + 1) != '?')
                result = concatenate_char(result, '$');
            else
                line = handle_variable_expansion(line, &result, &quote_state, env);
        }
        else
            result = concatenate_char(result, *line);
        line++;
    }
    return (result);
}

