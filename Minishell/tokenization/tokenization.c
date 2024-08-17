/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenization.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hfafouri <hfafouri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/07 12:45:34 by hfafouri          #+#    #+#             */
/*   Updated: 2024/08/13 15:15:18 by hfafouri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"


void parse_commands(char *line1, t_node **gc, t_cmd *token)
{
    int i = 0;
    int k = 0;
    int j = 0;
    int in_quotes = 0;
    char quote_char = 0;
    
    token->cmd = gc_malloc(gc, (token->cmd_count + 1) * sizeof(char *));
    while (line1[i])
    {
        while (line1[i] == ' ' || line1[i] == '\t')
            i++;
        
        if (line1[i] == '>' || line1[i] == '<')
        {
            if (line1[i + 1] == '>' || line1[i + 1] == '<')
                i++;
            i++;
            while (line1[i] == ' ' || line1[i] == '\t')
                i++;
            if (line1[i] == '"' || line1[i] == '\'')
            {
                quote_char = line1[i++];
                while (line1[i] && line1[i] != quote_char)
                    i++;
                if (line1[i] == quote_char)
                    i++;
            }
            else
            {
                while (line1[i] && line1[i] != ' ' && line1[i] != '\t' && line1[i] != '>' && line1[i] != '<')
                    i++;
            }
        }
        else if (line1[i])
        {
            j = i;
            while (line1[i])
            {
                if (line1[i] == '"' || line1[i] == '\'')
                {
                    quote_char = line1[i];
                    in_quotes = !in_quotes;
                }
                else if (!in_quotes && (line1[i] == ' ' || line1[i] == '\t' || line1[i] == '>' || line1[i] == '<'))
                    break;
                i++;
            }
            if (k < token->cmd_count)
            {
                token->cmd[k] = gc_malloc(gc, i - j + 1);
                ft_strcpy(token->cmd[k], &line1[j], i - j);
                k++;
            }
        }// f_out ==1 && env->env_line = NULL
    }
    token->cmd[k] = NULL;
}


void	tokenisation(void *line, t_node **gc, t_cmd *token)
{
	char	*line1;
	int		i;
    
	token->infile = NULL;
	token->outfile = NULL;
	token->heredoc = NULL;
	token->append = NULL;
	token->cmd = NULL;
	token->cmd_count = 0;
    // token->f_out = 0;
    // token->flag_her = 0;
	i = 0;
	int j = 0;
	line1 = ft_strtrim((char *)line, " ", gc);
	while (line1[i])
	{
		while (line1[i] == ' ' || line1[i] == '\t')
			i++;
		if (line1[i] == '>' && line1[i + 1] == '>') // $ and NULL put ambigious // syntax error handle quotes for append and outfile 
			handle_append(line1, &i, gc, token);
		else if (line1[i] == '>')
			handle_outfile(line1, &i, gc, token);
		else if (line1[i] == '<' && line1[i + 1] == '<')
			handle_heredoc(line1, &i, gc, token);
		else if (line1[i] == '<')
			handle_infile(line1, &i, gc, token);
		else
		{
			while (line1[i] == ' ' || line1[i] == '\t')
                i++;
			j = i;
            if (line1[i] == '\"')
            {
                i++;
                while (line1[i] != '\"' && line1[i])
                    i++;
                if (line1[i] == '\"')
                    i++;
            }
            while (line1[i] && line1[i] != ' ' && line1[i] != '\t'
                && line1[i] != '>' && line1[i] != '<')
				i++;
			token->cmd_count++;
		}
	}
	// parse_commands(line1, gc, token);
	if (token->cmd_count == 0)
	{
		token->cmd = (char **)gc_malloc(gc, 2 * sizeof(char *));
		token->cmd[0] = strdup("");
		token->cmd[1] = NULL;
	}
}
