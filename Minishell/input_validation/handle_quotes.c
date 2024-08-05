/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_quotes.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hfafouri <hfafouri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/26 13:17:52 by hfafouri          #+#    #+#             */
/*   Updated: 2024/07/28 03:59:26 by hfafouri         ###   ########.fr       */
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
// "$HO"ME
// void	handle_quotes(t_cmd *cmd, t_node **gc)
// {
// 	int i;
// 	int j;
// 	int len;
// 	int in_dq;
// 	int in_sq;
// 	char c;
// 	char *buffer;
// 	char *line;
//     i = 0;

// 	while (cmd->cmd[i])
// 	{
// 		line = cmd->cmd[i];
// 		len = ft_strlen(line);
// 		in_dq = in_double_quote(line, len);
// 		in_sq = in_single_quote(line, len);
// 		buffer = (char *)gc_malloc(gc, sizeof(char) * (len + 1));
// 		if (!buffer)
// 			return ;
// 		j = 0;
//         int k = 0;
// 		while (k < len)
// 		{
// 			c = line[k];
// 			if (c == '\'' && !in_dq)
// 			{
// 				in_sq = !in_sq;
// 			}
// 			else if (c == '"' && !in_sq)
// 			{
// 				in_dq = !in_dq;
// 			}
// 			// else if (c == '\\' && in_dq)
// 			// {
// 			// 	if (line[k + 1] == '"' || line[k + 1] == '\\' || line[k
// 			// 		+ 1] == '$')
// 			// 	{
// 			// 		buffer[j++] = line[++k];
// 			// 	}
// 			// 	else
// 			// 	{
// 			// 		buffer[j++] = '\\';
// 			// 	}
// 			// }
// 			else
// 			{
// 				buffer[j++] = c;
// 			}
//             k++;
// 		}
// 		buffer[j] = '\0';
// 		cmd->cmd[i] = buffer;
//         i++;
// 	}
// }

#include <string.h>

// void	handle_quotes(t_cmd *cmd, t_node **gc)
// {
// 	int i;
// 	int j;
// 	int len;
// 	int in_dq;
// 	int in_sq;
// 	char c;
// 	char *buffer;
// 	char *line;
//     char var_name[256];
//     int in_var;

//     cmd->dollar = NULL;
//     i = 0;

// 	while (cmd->cmd[i])
// 	{
// 		line = cmd->cmd[i];
// 		len = ft_strlen(line);
// 		in_dq = in_double_quote(line, len);
// 		in_sq = in_single_quote(line, len);
// 		buffer = (char *)gc_malloc(gc, sizeof(char) * (len + 1));
// 		if (!buffer)
// 			return ;
// 		j = 0;
//         int k = 0;
//         in_var = 0;
// 		while (k < len)
// 		{
// 			c = line[k];
//             if (c == '$' && (in_dq || in_sq) && !in_var)
//             {
//                 in_var = 1;
//                 int l = 0;
//                 k++;
//                 while (k < len && line[k] != '\'' && line[k] != '"'
// && line[k] != ' ' && l < 255)
//                 {
//                     var_name[l++] = line[k++];
//                 }
//                 var_name[l] = '\0';
//                 cmd->dollar = (char *)gc_malloc(gc, sizeof(char)
// * (strlen(var_name) + 1));
//                 if (!cmd->dollar)
//                     return ;
//                 strcpy(cmd->dollar, var_name);
//                 continue ;
//             }
// 			if (c == '\'' && !in_dq)
// 			{
// 				in_sq = !in_sq;
// 			}
// 			else if (c == '"' && !in_sq)
// 			{
// 				in_dq = !in_dq;
// 			}
// 			else
// 			{
// 				buffer[j++] = c;
// 			}
//             k++;
// 		}
// 		buffer[j] = '\0';
// 		cmd->cmd[i] = buffer;
//         i++;
// 	}
// }

// void handle_dollar_and_quotes(t_cmd *cmd, t_node **gc)
// {
//     int i;
//     int len;
//     int in_dq, in_sq;
//     char c;
//     char *buffer;
//     char *line;
//     char var_name[256];
//     int in_var;
//     char *result;
//     int result_size;

//     cmd->aft_dol_dq = 0;
//     cmd->dollar = NULL;
//     i = 0;

//     while (cmd->cmd[i])
//     {
//         line = cmd->cmd[i];
//         len = ft_strlen(line);
//         in_dq = 0;
//         in_sq = 0;

//         buffer = (char *)gc_malloc(gc, sizeof(char) * (len + 1));
//         if (!buffer)
//             return ;

//         // Initial result buffer size
//         result_size = len + 1;
//         result = (char *)gc_malloc(gc, sizeof(char) * result_size);
//         if (!result)
//             return ;

//         int j = 0, k = 0, l = 0;
//         in_var = 0;
//         cmd->aft_dol_dq = 0;

//         while (k < len)
//         {
//             c = line[k];
//             if (c == '"')
//             {
//                 in_dq = !in_dq;
//                 buffer[j++] = line[k++];
//                 continue ;
//             }

//             if (c == '$' && in_dq && !in_var)
//             {
//                 in_var = 1;
//                 k++;
//                 l = 0;
//                 while (k < len && line[k] != '"' && line[k] != ' '
// && l < 255)
//                 {
//                     var_name[l++] = line[k++];
//                 }
//                 var_name[l] = '\0';

//                 search_env(var_name, cmd);
//                 if (cmd->env_line)
//                 {
//                     int env_len = strlen(cmd->env_line);
//                     if (j + env_len >= result_size)
//                     {
//                         // Resize result buffer if needed
//                         result_size = j + env_len + 1;
//                         result = (char *)realloc(result, sizeof(char)
// * result_size);
//                         if (!result)
//                             return ;
//                     }
//                     strcpy(&result[j], cmd->env_line);
//                     j += env_len;
//                 }
//                 in_var = 0;
//                 continue ;
//             }

//             if (c == '\'' && !in_dq && !cmd->aft_dol_dq)
//             {
//                 in_sq = !in_sq;
//             }
//             else if (c == '"' && !in_sq && !cmd->aft_dol_dq)
//             {
//                 in_dq = !in_dq;
//             }
//             else
//             {
//                 if (j >= result_size - 1)
//                 {
//                     result_size *= 2;
//                     result = (char *)realloc(result, sizeof(char)
// * result_size);
//                     if (!result)
//                         return ;
//                 }
//                 result[j++] = c;
//             }
//             k++;
//         }
//         result[j] = '\0';
//         cmd->cmd[i] = result;
//         i++;
//     }
// }

// void handle_dollar_and_quotes(t_cmd *cmd, t_node **gc)
// {
//     int i;
//     int len;
//     int in_dq, in_sq;
//     char c;
//     char *buffer;
//     char *line;
//     char var_name[256];
//     int in_var;
//     char *result;

//     cmd->aft_dol_dq = 0;
//     cmd->dollar = NULL;
//     i = 0;

//     while (cmd->cmd[i])
//     {
//         line = cmd->cmd[i];
//         len = ft_strlen(line);
//         in_dq = 0;
//         in_sq = 0;
//         buffer = (char *)gc_malloc(gc, sizeof(char) * (len + 1));
//         if (!buffer)
//             return ;
//         result = (char *)gc_malloc(gc, sizeof(char) * (len + 1));
//         if (!result)
//             return ;

//         int j = 0, k = 0, l = 0;
//         in_var = 0;
//         cmd->aft_dol_dq = 0;

//         while (k < len)
//         {
//             c = line[k];
//             if (c == '"')
//             {
//                 in_dq = !in_dq;
//                 buffer[j++] = line[k++];
//                 continue ;
//             }

//             if (c == '$' && in_dq && !in_var)
//             {
//                 in_var = 1;
//                 k++;
//                 l = 0;
//                 while (k < len && line[k] != '"' && line[k] != ' '
// && l < 255)
//                 {
//                     var_name[l++] = line[k++];
//                 }
//                 var_name[l] = '\0';
// 				printf("var_name = %s\n",var_name);
//                 search_env(var_name, cmd);
//                 if (cmd->env_line)
//                 {
// 					printf("env-line = %s\n",cmd->env_line);
//                     strcpy(&result[j], cmd->env_line);
// 					printf("&result = %s\n",&result[j]);
//                     j += strlen(cmd->env_line);
//                 }
//                 in_var = 0;
//                 continue ;
//             }

//             if (c == '\'' && !in_dq && !cmd->aft_dol_dq)
//             {
//                 in_sq = !in_sq;
//             }
//             else if (c == '"' && !in_sq && !cmd->aft_dol_dq)
//             {
//                 in_dq = !in_dq;
//             }
//             else
//             {
//                 buffer[j++] = c;
//             }
//             k++;
//         }
//         buffer[j] = '\0';
// 		// printf("result %s\n", result);
//         strcpy(result, buffer);
//         cmd->cmd[i] = result;
//         i++;
//     }

//-------------------------------------------------------------------
// void handle_dollar(char *line, t_node **gc, t_cmd *token)
// {
//     int i = 0;
//     int len = ft_strlen(line);
//     int in_dq = 0;
//     char var_name[256];
//     int in_var = 0;
//     char *buffer;
//     char *result;

//     buffer = (char *)gc_malloc(gc, sizeof(char) * (len + 1));
//     if (!buffer)
//         return ;

//     result = (char *)gc_malloc(gc, sizeof(char) * (len + 1));
//     if (!result)
//         return ;

//     int j = 0, k = 0;
//     while (i < len)
//     {
//         if (line[i] == '"')
//         {
//             in_dq = !in_dq;
//             buffer[j++] = line[i++];
//             continue ;
//         }
//         if (line[i] == '$' && in_dq && !in_var)
//         {
//             in_var = 1;
//             i++;
//             int l = 0;
//             while (i < len && line[i] != '"' && line[i] != ' ' && l < 255)
//             {
//                 var_name[l++] = line[i++];
//             }
//             var_name[l] = '\0';
//             search_env(var_name, token);
//             if (token->env_line)
//             {
//                 strcpy(&result[k], token->env_line);
//                 k += strlen(token->env_line);
//             }
//             in_var = 0;
//             continue ;
//         }

//         buffer[j++] = line[i++];
//     }
//     buffer[j] = '\0';
//     strcpy(result, buffer);
//     token->cmd[d] = result;
// }

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
			{
				in_sq = !in_sq;
			}
			else if (c == '"' && !in_sq && !cmd->aft_dol_dq)
			{
				in_dq = !in_dq;
			}
			else
			{
				buffer[j++] = c;
			}
			k++;
		}
		buffer[j] = '\0';
		cmd->cmd[i] = buffer;
		i++;
	}
}

// void    search_env(char *line, t_cmd *env)
// {
//     int i;
//     int j = 0;
//     int count;

//     i = 0;
//     count = 0;
//     env->env_line = NULL;
//     while (env->env[i])
//     {
//         j = 0;
//         count = 0;
//         while (env->env[i][j] != '=' && env->env[i][j] != '\0')
//         {
//             count++;
//             j++;
//         }
//         if (ft_strncmp(env->env[i], line, count) == 0)
//         {
//             env->env_line = ft_strstr(env->env[i], line);
//             break ;
//         }
//         i++;
//     }
//     if (!env->env_line)
//     {
//         env->env_line = NULL;
//     }
// }

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

	len = ft_strlen(str);
	char *new_str = malloc(len + 2);
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

	if (*line == '$' && (!quote_state->in_single_quotes
			|| (quote_state->in_double_quotes && !quote_state->nested_quotes)))
	{
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
			new_ret = realloc(*ret, new_len);
			if (!new_ret)
				return (NULL);
			*ret = new_ret;
			strcat(*ret, env->env_line);
		}
		return (line - 1);
	}
	*ret = concatenate_char(*ret, *line);
	return (line);
}

char	*handle_dollar_sign(char *line, t_cmd *env)
{
	char			*result;
	t_quote_state	quote_state;

	quote_state.in_single_quotes = 0;
	quote_state.in_double_quotes = 0;
	quote_state.nested_quotes = 0;
	result = malloc(1);
	if (!result)
		return (NULL);
	result[0] = '\0';
	while (*line)
	{
		if (*line == '\'' || *line == '\"')
		{
			toggle_quotes(*line, &quote_state);
			result = concatenate_char(result, *line);
		}
		else
		{
			line = handle_variable_expansion(line, &result, &quote_state, env);
		}
		line++;
	}
	return (result);
}

// char	*handle_dollar_sign(char *line, t_cmd *env)
// {
// 	int		i = 0, j;
// 	char	var_name[256] = {0};
// 	char	*result;
// 	int		in_double_quotes;
// 	int		var_name_start;

// 	int required_size = 1; // Start with 1 for the null terminator
// 	i = 0, j = 0;
// 	in_double_quotes = 0;
// 	// Calculate required size
// 	// Adjusted section within the while loop for calculating required size
// while (line[i] != '\0')
// {
//     if (line[i] == '\"')
//     {
//         in_double_quotes = !in_double_quotes; // Toggle state
//         required_size++;
//     }
//     else if (line[i] == '$')
//     {
//         if ((line[i + 1] != '\"' && line[i + 1] != '\'') && (in_double_quotes
// || isalnum(line[i + 1]) || line[i + 1] == '_'))
//         {
//             var_name_start = ++i;
//             while (line[i] != '\0' && (isalnum(line[i]) || line[i] == '_'))
//             {
//                 i++;
//             }
//             strncpy(var_name, line + var_name_start, i - var_name_start);
//             var_name[i - var_name_start] = '\0';
//             search_env(var_name, env);
//             if (env->env_line)
//             {
//                 required_size += strlen(env->env_line);
//             }
//             i--;
//         }
//         else
//         {
//             required_size += 2;
//             i++;
//         }
//     }
//     else
//     {
//         required_size++;
//     }
//     i++;
// }
// 	result = malloc(required_size);
// 	if (!result)
// 		return (NULL); // Allocation failed
// 	i = 0;
// 	in_double_quotes = 0;
// 	while (line[i] != '\0')
// 	{
// 		if (line[i] == '\"')
// 		{
// 			in_double_quotes = !in_double_quotes; // Toggle state
// 			result[j++] = line[i++];
// 		}
// 		else if (line[i] == '$' && (in_double_quotes || (line[i + 1] != '\"'
// 					&& line[i + 1] != '\'')))
// 		{
// 			var_name_start = ++i;
// 			while (line[i] != '\0' && (isalnum(line[i]) || line[i] == '_'))
// 			{
// 				i++;
// 			}
// 			strncpy(var_name, line + var_name_start, i - var_name_start);
// 			var_name[i - var_name_start] = '\0';
// 			search_env(var_name, env);
// 				// Assuming this function sets env->env_line
// 			if (env->env_line)
// 			{
// 				strcpy(result + j, env->env_line);
// 				j += strlen(env->env_line);
// 			}
// 			i--; // Adjust because the loop will increment i
// 		}
// 		else
// 		{
// 			result[j++] = line[i++];
// 		}
// 	}
// 	result[j] = '\0';
// 	return (result);
// }

// void	handle_quotes(t_cmd *cmd, t_node **gc)
// {
// 	int i;
// 	int j;
// 	int len;
// 	int in_dq;
// 	int in_sq;
// 	char c;
// 	char *buffer;
// 	char *line;
//     char var_name[256];
//     int in_var;
//     cmd->aft_dol_dq = 0;

//     cmd->dollar = NULL;
//     i = 0;

// 	while (cmd->cmd[i])
// 	{
// 		line = cmd->cmd[i];
// 		len = ft_strlen(line);
// 		in_dq = in_double_quote(line, len);
// 		in_sq = in_single_quote(line, len);
// 		buffer = (char *)gc_malloc(gc, sizeof(char) * (len + 1));
// 		if (!buffer)
// 			return ;
// 		j = 0;
//         int k = 0;
//         in_var = 0;
//         cmd->aft_dol_dq = 0;
// 		while (k < len)
// 		{
// 			c = line[k];
//             if (c == '$' && (in_dq || in_sq) && !in_var)
//             {
//                 in_var = 1;
//                 int l = 0;
//                 k++;
// 				printf("line[k] = %s\n",&line[k]);
//                 if (line[k] == '"' || line[k] == '\'')
//                 {
//                     cmd->aft_dol_dq = 1;
//                     char quote = line[k++];
//                     while (k < len && line[k] != quote && l < 255)
//                     {
//                         var_name[l++] = line[k++];
//                     }
//                     k++;
//                 }
//                 else
//                 {
//                     while (k < len && line[k] != '\'' && line[k] != '"'
// && line[k] != ' ' && l < 255)
//                     {
//                         var_name[l++] = line[k++];
//                     }
//                 }
//                 var_name[l] = '\0';
//                 cmd->dollar = (char *)gc_malloc(gc, sizeof(char)
// * (strlen(var_name) + 1));
//                 if (!cmd->dollar)
//                     return ;
//                 strcpy(cmd->dollar, var_name);
//                 continue ;
//             }
// 			if (c == '\'' && !in_dq && !cmd->aft_dol_dq)
// 			{
// 				in_sq = !in_sq;
// 			}
// 			else if (c == '"' && !in_sq && !cmd->aft_dol_dq)
// 			{
// 				in_dq = !in_dq;
// 			}
// 			else
// 			{
// 				buffer[j++] = c;
// 			}
//             k++;
// 		}
// 		buffer[j] = '\0';
// 		cmd->cmd[i] = buffer;
//         i++;
// 	}
// }