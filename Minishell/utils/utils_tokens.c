/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_tokens.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hfafouri <hfafouri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/28 15:56:05 by hfafouri          #+#    #+#             */
/*   Updated: 2024/08/27 21:22:47 by hfafouri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	handle_infile(char *line1, int *i, t_node **gc, t_cmd *token)
{
	char	*word;
	char	*expanded;

	if (*i > 0 && line1[*i - 1] != '\0' && (line1[*i - 1] == '\'' || line1[*i
				- 1] == '"'))
	{
		(*i)++;
		return ;
	}
	(*i)++;
	help_skip_spaces(line1, i);
	if (line1[*i] != '>' && line1[*i] != '<' && line1[*i])
	{
		word = help_extract_word(line1, i, gc);
		expanded = expand_quotes(word);
		if ((expanded[0] == '\'' && expanded[ft_strlen(expanded) - 1] == '\'')
			|| (expanded[0] == '"' && expanded[ft_strlen(expanded) - 1] == '"'))
		{
			ft_memmove(expanded, expanded + 1, ft_strlen(expanded) - 2);
			expanded[ft_strlen(expanded) - 2] = '\0';
		}
		ft_lstadd_back(&token->infile, ft_lstnew(expanded, gc));
	}
	else
		(*i)--;
}

void	handle_append(char *line1, int *i, t_node **gc, t_cmd *token)
{
	char	*word;
	char	*expanded;

	if (*i > 0 && (line1[*i - 1] == '\'' || line1[*i - 1] == '"'))
	{
		(*i) += 2;
		return ;
	}
	*i += 2;
	help_skip_spaces(line1, i);
	if (line1[*i] != '>' && line1[*i] != '<' && line1[*i])
	{
		word = help_extract_word(line1, i, gc);
		expanded = expand_quotes(word);
		if ((expanded[0] == '\'' && expanded[ft_strlen(expanded) - 1] == '\'')
			|| (expanded[0] == '"' && expanded[ft_strlen(expanded) - 1] == '"'))
		{
			ft_memmove(expanded, expanded + 1, ft_strlen(expanded) - 2);
			expanded[ft_strlen(expanded) - 2] = '\0';
		}
		ft_lstadd_back(&token->append, ft_lstnew(expanded, gc));
	}
}

void	handle_outfile(char *line1, int *i, t_node **gc, t_cmd *token)
{
	char	*word;
	char	*expanded;

	if (*i > 0 && (line1[*i - 1] == '\'' || line1[*i - 1] == '"'))
	{
		(*i)++;
		return ;
	}
	(*i)++;
	help_skip_spaces(line1, i);
	if (line1[*i] != '>' && line1[*i] != '<' && line1[*i])
	{
		word = help_extract_word(line1, i, gc);
		expanded = expand_quotes(word);
		if ((expanded[0] == '\'' && expanded[ft_strlen(expanded) - 1] == '\'')
			|| (expanded[0] == '"' && expanded[ft_strlen(expanded) - 1] == '"'))
		{
			ft_memmove(expanded, expanded + 1, ft_strlen(expanded) - 2);
			expanded[ft_strlen(expanded) - 2] = '\0';
		}
		ft_lstadd_back(&token->outfile, ft_lstnew(expanded, gc));
	}
	else
		(*i)--;
}

void	handle_heredoc(char *line1, int *i, t_node **gc, t_cmd *token)
{
	char	*word;
	char	*expanded;

	token->flag_her = 0;
	if (*i > 0 && (line1[*i - 1] == '\'' || line1[*i - 1] == '"'))
	{
		(*i) += 2;
		return ;
	}
	*i += 2;
	help_skip_spaces(line1, i);
	if (line1[*i] != '>' && line1[*i] != '<' && line1[*i])
	{
		word = help_extract_word(line1, i, gc);
		expanded = expand_quotes(word);
		if (ft_strcmp(word, expanded) != 0)
			token->flag_her = 1;
		if ((expanded[0] == '\'' && expanded[ft_strlen(expanded) - 1] == '\'')
			|| (expanded[0] == '"' && expanded[ft_strlen(expanded) - 1] == '"'))
		{
			ft_memmove(expanded, expanded + 1, ft_strlen(expanded) - 2);
			expanded[ft_strlen(expanded) - 2] = '\0';
		}
		ft_lstadd_back(&token->heredoc, ft_lstnew(expanded, gc));
	}
}
