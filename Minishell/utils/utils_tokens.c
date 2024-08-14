/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_tokens.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hfafouri <hfafouri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/28 15:56:05 by hfafouri          #+#    #+#             */
/*   Updated: 2024/08/14 15:42:50 by hfafouri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	check_dollar(char *line)
{
	int i = 0;
	while(line[i])
	{
		if(line[i] == '$')
			return(1);
		i++;
	}
	return(0);
}

t_node	*ft_lstnew_with_flag(void *content, int flag)
{
	t_node	*new_node;

	new_node = (t_node *)malloc(sizeof(t_node));
	if (!new_node)
		return (NULL);
	new_node->data = content;
	new_node->flag = flag;
	new_node->next = NULL;
	return (new_node);
}

void	handle_append(char *line1, int *i, t_node **gc, t_cmd *token)
{
	char	*word;
	char *expanded;
	int		j;

	// token->f_append = 0;
	*i += 2;
	while (line1[*i] == ' ' || line1[*i] == '\t')
		(*i)++;
	if (line1[*i] != '>' && line1[*i] != '<' && line1[*i])
	{
		j = *i;
		if (line1[*i] == '\"')
		{
			(*i)++;
			while (line1[*i] != '\"' && line1[*i])
				(*i)++;
			if (line1[*i] == '\"')
				(*i)++;
		}
		while (line1[*i] && line1[*i] != ' ' && line1[*i] != '\t'
			&& line1[*i] != '>' && line1[*i] != '<')
			(*i)++;
		word = gc_malloc(gc, *i - j + 1);
		ft_strcpy(word, &line1[j], *i - j);
		word[*i - j] = '\0';
		if (check_dollar(&line1[j]) == 1)
		{
			word = handle_dollar_sign(word, token);
			// token->f_append = 1;
		}
		expanded = expand_quotes(word);
		ft_lstadd_back(&token->append, ft_lstnew(expanded));
	}
}

void	handle_outfile(char *line1, int *i, t_node **gc, t_cmd *token)
{
	char	*word;
	char	*expanded;

	expanded = NULL;
	word = NULL;
	int		j;
	// token->f_out = 0;
	(*i)++;
	while (line1[*i] == ' ' || line1[*i] == '\t')
		(*i)++;
	if (line1[*i] != '>' && line1[*i] != '<' && line1[*i])
	{
		j = *i;
		if (line1[*i] == '\"')
		{
			(*i)++;
			while (line1[*i] != '\"' && line1[*i])
				(*i)++;
			if (line1[*i] == '\"')
				(*i)++;
		}
		while (line1[*i] && line1[*i] != ' ' && line1[*i] != '\t'
			&& line1[*i] != '>' && line1[*i] != '<')
			(*i)++;
		word = gc_malloc(gc, *i - j + 1);
		ft_strcpy(word, &line1[j], *i - j);
		word[*i - j] = '\0';
		if (check_dollar(&line1[j]) == 1)
		{
			word = handle_dollar_sign(word, token);
			// token->f_out = 1;
		}
		expanded = expand_quotes(word);
		printf("expanded = %s\n", expanded);
		// expanded = handle_dollar_sign(expanded, token);
		ft_lstadd_back(&token->outfile, ft_lstnew(expanded));
	}
}

void	handle_heredoc(char *line1, int *i, t_node **gc, t_cmd *token)
{
	char	*word;
	char *expanded;
	int		j;

	token->flag_her = 0;
	*i += 2;
	while (line1[*i] == ' ' || line1[*i] == '\t')
		(*i)++;
	if (line1[*i] != '>' && line1[*i] != '<' && line1[*i])
	{
		j = *i;
		while (line1[*i] && line1[*i] != ' ' && line1[*i] != '\t'
			&& line1[*i] != '>' && line1[*i] != '<')
			(*i)++;
		word = gc_malloc(gc, *i - j + 1);
		ft_strcpy(word, &line1[j], *i - j);
		word[*i - j] = '\0';
		expanded = expand_quotes(word);
		if (strcmp(word, expanded) != 0)
			token->flag_her = 1;
		ft_lstadd_back(&token->heredoc, ft_lstnew(expanded));
	}
}

void	handle_infile(char *line1, int *i, t_node **gc, t_cmd *token)
{
	char	*word;
	char	*expanded;
	int		j;

	(*i)++;
	while (line1[*i] == ' ' || line1[*i] == '\t')
		(*i)++;
	if (line1[*i] != '>' && line1[*i] != '<' && line1[*i])
	{
		j = *i;
		while (line1[*i] && line1[*i] != ' ' && line1[*i] != '\t'
			&& line1[*i] != '>' && line1[*i] != '<')
			(*i)++;
		word = gc_malloc(gc, *i - j + 1);
		ft_strcpy(word, &line1[j], *i - j);
		word[*i - j] = '\0';
		expanded = expand_quotes(word);
		ft_lstadd_back(&token->infile, ft_lstnew(expanded));
	}
}
