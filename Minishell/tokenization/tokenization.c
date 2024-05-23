/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenization.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hfafouri <hfafouri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/19 15:40:39 by hfafouri          #+#    #+#             */
/*   Updated: 2024/05/23 10:12:58 by hfafouri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/*
while (TRUE)
{
	1.skip(white_spaces(line);
		i++;
	2.pipe()
	--> new_token(value, PIPE);
	-->add_back(&head, new);
	3.word()
	--> while(!= | > < >> <<)
	-->new_token(value, WORD);
	-->add_back(&head, new);
	4.redirections()
	--> if ('> >> << <')
	--> new_token(value, REDIRIN OR OUT OU HERDOC OR APPEND);
	--> add_back(&head, new);
}
 */

// int	count_len(char *s)
// {
// 	int	len;
// 	int	i;

// 	len = 0;
// 	i = 0;
// 	while(s[i])
// 	{
// 		while (s[i] != '|' && s[i] != '<' && s[i] != '>')
// 		{
// 			len++;
// 		}
// 		i++;
// 	}
// 	return (len);
// }

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

void tokenisation(void *line, t_node **gc)
{
	t_cmd token;
	t_node *node;
	token.infile = NULL;
	token.outfile = NULL;
	token.heredoc = NULL;
	token.append = NULL;
	token.cmd = NULL;
	int i = 0;
	int j = 0;
	int cmd_count = 0;
	int k = 0;
	char *word;
	char *line1 = (char *)line;

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
				ft_lstadd_back(&token.append, ft_lstnew(word));
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
				ft_lstadd_back(&token.outfile, ft_lstnew(word));
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
				ft_lstadd_back(&token.heredoc, ft_lstnew(word));
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
				ft_lstadd_back(&token.infile, ft_lstnew(word));
			}
		}
		else
		{
			j = i;
			while (line1[i] && line1[i] != ' ' && line1[i] != '\t' && line1[i] != '>' && line1[i] != '<')
				i++;
			cmd_count++;
			// token.cmd[k] = gc_malloc(gc, i - j + 1);
			// ft_strcpy(word, &line1[j], i - j);
			// ft_lstadd_back(&token.cmd, ft_lstnew(word));
		}
	}
	token.cmd = gc_malloc(gc, (cmd_count + 1) * sizeof(char *));
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
            j = i;
            while (line1[i] && line1[i] != ' ' && line1[i] != '\t' && line1[i] != '>' && line1[i] != '<')
                i++;
            token.cmd[k] = gc_malloc(gc, i - j + 1);
            ft_strcpy(token.cmd[k], &line1[j], i - j);
            k++;
        }
    }
    token.cmd[k] = NULL;
	// k = 0;
	// while(token.cmd[k])
	// {
	// 	printf("cmd = %s\n", token.cmd[k]);
	// 	k++;
	// }
}
