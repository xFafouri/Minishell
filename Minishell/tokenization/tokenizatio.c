/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizatio.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hfafouri <hfafouri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/19 15:40:39 by hfafouri          #+#    #+#             */
/*   Updated: 2024/07/10 19:18:23 by hfafouri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <string.h>

void	parse_commands(char *line1, int *i, t_node **gc, t_cmd *token)
{
	int	j;
	int l;

	j = 0;
	l = *i;
	while (line1[*i])
	{
		while (line1[*i] == ' ' || line1[*i] == '\t')
			(*i)++;
		if (line1[*i] == '>' || line1[*i] == '<')
			break ;
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
		token->cmd_count++;
	}
	if (token->cmd_count == 0)
	{
		printf("test\n");
		token->cmd = NULL;
		return ;
	}
	printf("token->cmd_count %d\n", token->cmd_count);
	token->cmd = gc_malloc(gc, (token->cmd_count + 1) * sizeof(char *));
	*i = l;
	while (line1[*i])
	{
		while (line1[*i] == ' ' || line1[*i] == '\t')
			(*i)++;
		if (line1[*i] == '>' || line1[*i] == '<')
			break ;
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
		token->cmd[token->k] = gc_malloc(gc, *i - j + 1);
		ft_strcpy(token->cmd[token->k], &line1[j], *i - j);
		token->k++;
	}
	printf("k = %d\n", token->k);
	token->cmd[token->k] = NULL;
	printf("cmd = %s\n",token->cmd[0]);
    printf("cmd = %s\n",token->cmd[1]);
}

void tokenisation(void *line, t_node **gc, t_cmd *token)
{
	t_node *node;
	token->infile = NULL;
	token->outfile = NULL;
	token->heredoc = NULL;
	token->append = NULL;
	token->cmd = NULL;
	int i = 0;
	int j = 0;
	int cmd_count = 0;
	int k = 0;
	char *word;
	char *line1 = ft_strtrim((const char *)line, " ", gc);
	
	while (line1[i])
	{
		while (line1[i] == ' ' || line1[i] == '\t')
			i++;
		if (line1[i] == '>' && line1[i + 1] == '>')
		{
			i += 2;
			while (line1[i] == ' ' || line1[i] == '\t')
				i++;
			if (line1[i] != '>' && line1[i] != '<' && line1[i])
			{
				j = i;
				while (line1[i] && line1[i] != ' ' && line1[i] != '\t' && line1[i] != '>' && line1[i] != '<')
					i++;
				word = gc_malloc(gc, i - j + 1);
				ft_strcpy(word, &line1[j], i - j);
				ft_lstadd_back(&token->append, ft_lstnew(word));
			}
		}
		else if (line1[i] == '>')
		{
			i++;
			while (line1[i] == ' ' || line1[i] == '\t')
				i++;
			if (line1[i] != '>' && line1[i] != '<' && line1[i])
			{
				j = i;
				while (line1[i] && line1[i] != ' ' && line1[i] != '\t' && line1[i] != '>' && line1[i] != '<')
					i++;
				word = gc_malloc(gc, i - j + 1);
				ft_strcpy(word, &line1[j], i - j);
				ft_lstadd_back(&token->outfile, ft_lstnew(word));
			}
		}
		else if (line1[i] == '<' && line1[i + 1] == '<')
		{
			i += 2;
			while (line1[i] == ' ' || line1[i] == '\t')
				i++;
			if (line1[i] != '>' && line1[i] != '<' && line1[i])
			{
				j = i;
				while (line1[i] && line1[i] != ' ' && line1[i] != '\t' && line1[i] != '>' && line1[i] != '<')
					i++;
				word = gc_malloc(gc, i - j + 1);
				ft_strcpy(word, &line1[j], i - j);
				ft_lstadd_back(&token->heredoc, ft_lstnew(word));
			}
				}
		else if (line1[i] == '<')
		{
			i++;
			while (line1[i] == ' ' || line1[i] == '\t')
				i++;
			if (line1[i] != '>' && line1[i] != '<' && line1[i])
			{
				j = i;
				while (line1[i] && line1[i] != ' ' && line1[i] != '\t' && line1[i] != '>' && line1[i] != '<')
					i++;
				word = gc_malloc(gc, i - j + 1);
				ft_strcpy(word, &line1[j], i - j);
				ft_lstadd_back(&token->infile, ft_lstnew(word));
			}
		}
		else
		{
			while (line1[i] == ' ' || line1[i] == '\t')
                i++;
			j = i;
			while (line1[i] && line1[i] != ' ' && line1[i] != '\t' && line1[i] != '>' && line1[i] != '<')
				i++;
			cmd_count++;
		}
	}
	token->cmd = gc_malloc(gc, (cmd_count + 1) * sizeof(char *));
	i = 0;
        while (line1[i])
    {
        while (line1[i] == ' ' || line1[i] == '\t')
            i++;
        if (line1[i] == '>' && line1[i + 1] == '>')
        {
            i += 2;
            while (line1[i] == ' ' || line1[i] == '\t')
                i++;
            if (line1[i] != '>' && line1[i] != '<' && line1[i])
            {
                while (line1[i] && line1[i] != ' ' && line1[i] != '\t' && line1[i] != '>' && line1[i] != '<')
                    i++;
            }
        }
        else if (line1[i] == '>')
        {
            i++;
            while (line1[i] == ' ' || line1[i] == '\t')
                i++;
            if (line1[i] != '>' && line1[i] != '<' && line1[i])
            {
                while (line1[i] && line1[i] != ' ' && line1[i] != '\t' && line1[i] != '>' && line1[i] != '<')
                    i++;
            }
        }
        else if (line1[i] == '<' && line1[i + 1] == '<')
        {
            i += 2;
            while (line1[i] == ' ' || line1[i] == '\t')
                i++;
            if (line1[i] != '>' && line1[i] != '<' && line1[i])
            {
                while (line1[i] && line1[i] != ' ' && line1[i] != '\t' && line1[i] != '>' && line1[i] != '<')
                    i++;
            }
        }
        else if (line1[i] == '<')
        {
            i++;
            while (line1[i] == ' ' || line1[i] == '\t')
                i++;
            if (line1[i] != '>' && line1[i] != '<' && line1[i])
            {
                while (line1[i] && line1[i] != ' ' && line1[i] != '\t' && line1[i] != '>' && line1[i] != '<')
                    i++;
            }
        }
        else
        {
			while (line1[i] == ' ' || line1[i] == '\t')
                i++;
			j = i;
            while (line1[i] && line1[i] != ' ' && line1[i] != '\t' && line1[i] != '>' && line1[i] != '<')
                i++;
            token->cmd[k] = gc_malloc(gc, i - j + 1);
            ft_strcpy(token->cmd[k], &line1[j], i - j);
            k++;
        }
    }
    token->cmd[k] = NULL;
}