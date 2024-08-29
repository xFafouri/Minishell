/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hfafouri <hfafouri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/29 18:30:45 by hfafouri          #+#    #+#             */
/*   Updated: 2024/08/29 18:31:22 by hfafouri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*expand_quotes(char *line, t_node **gc, t_cmd *token)
{
	int		len;
	char	c;
	char	*buffer;
	int		j;
	int		k;

	len = ft_strlen(line);
	token->in_dq = 0;
	token->in_sq = 0;
	buffer = gc_malloc(gc, sizeof(char) * (len + 1));
	j = 0;
	k = 0;
	while (k < len)
	{
		c = line[k];
		if (c == '\'' && !token->in_dq)
			token->in_sq = !token->in_sq;
		else if (c == '"' && !token->in_sq)
			token->in_dq = !token->in_dq;
		else
			buffer[j++] = c;
		k++;
	}
	buffer[j] = '\0';
	return (buffer);
}

void	help_echo(t_cmd *token, int *i, int *j)
{
	if (token->is_flag && token->cmd[*i][0] == '-' && token->cmd[*i][1] == 'n')
	{
		*j = 1;
		while (token->cmd[*i][*j] == 'n')
			(*j)++;
		if (token->cmd[*i][*j] != '\0')
		{
			token->is_flag = 0;
			printf("%s", token->cmd[*i]);
		}
		else
			token->newline = 0;
	}
	else
	{
		token->is_flag = 0;
		if (*i > 1)
			printf(" ");
		printf("%s", token->cmd[*i]);
	}
}

void	ft_echo(char *line, t_cmd *token)
{
	int	i;
	int	j;

	i = 1;
	token->newline = 1;
	token->is_flag = 1;
	if (ft_strcmp(token->cmd[0], "echo") == 0)
	{
		while (token->cmd[i] != NULL)
		{
			j = 0;
			help_echo(token, &i, &j);
			i++;
		}
	}
	if (token->newline)
		printf("\n");
	token->status = 0;
}
