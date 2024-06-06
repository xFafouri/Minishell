/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenization.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hfafouri <hfafouri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/19 15:40:39 by hfafouri          #+#    #+#             */
/*   Updated: 2024/05/23 19:51:39 by hfafouri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"


char *ft_strcpy(char *s1, char *s2, int len)
{
	int i;

	i = 0;
	while (s2[i] && len > 0)
	{
		s1[i] = s2[i];
		i++;
		len--;
	}
	s1[i] = '\0';
	return s1;
}
static char	check(const char *set, char c)
{
	while (*set && c != *set)
		set++;
	return (c == *set);
}
#include <string.h>
#include <stdlib.h>

int ft_strlen1(char *s)
{
	int i = 0;
	while(s[i])
		i++;
	return(i);
}
char	*ft_substr1(char const *s, unsigned int start, size_t len)
{
	char		*sub;
	size_t		i;

	if (s == NULL)
		return (NULL);
	i = 0;
	if (start >= ft_strlen1((char *)s))
		return ("");
	if (len > ft_strlen1((char *)s) - start)
		len = ft_strlen1((char *)s) - start;
	sub = (char *)malloc(sizeof(char) * (len + 1));
	if (!sub)
		return (NULL);
	while (i < len)
	{
		sub[i] = s[start + i];
		i++;
	}
	sub[len] = '\0';
	return ((char *)sub);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	size_t	first;
	size_t	last;

	if (!s1 || !set)
		return ("");
	first = 0;
	last = ft_strlen1((char *)s1);
	while (first < last && check(set, s1[first]))
		first++;
	if (first == last)
		return ("");
	while (last > 0 && check(set, s1[last - 1]))
		last--;
	return (ft_substr1(s1, first, last - first));
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
	char *line1 = ft_strtrim((const char *)line, " ");
	
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

