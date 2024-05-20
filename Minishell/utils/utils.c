/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hfafouri <hfafouri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/19 16:00:55 by hfafouri          #+#    #+#             */
/*   Updated: 2024/05/20 09:57:07 by hfafouri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_token	*new_token(int type,char *value)
{
	t_token	*new;

	new = malloc(sizeof(t_token));
	new->type = type;
	if (value)
        new->value = ft_strdup(value);
	else 
        new->value = NULL;
	new->next = NULL;
	return (new);
}
t_token	*new_token(int type, char *value)
{
	t_token	*new;

	new = malloc(sizeof(t_token));
	if (new == NULL)
		return (NULL);
	new->type = type;
	new->value = value;
	new->next = NULL;
	return (new);
}

t_token	*lstlast(t_token *lst)
{
	if (lst == NULL)
		return (NULL);
	while (lst->next != NULL)
		lst = lst->next;
	return (lst);
}

void	add_back(t_token **lst, t_token *new)
{
	t_token	*last;

	if (lst == NULL)
		return ;
	if (*lst == NULL)
	{
		*lst = new;
		return ;
	}
	last = lstlast(*lst);
	last->next = new;
}

// int	ft_lstsize(t_token *lst)
// {
// 	int	counter;

// 	counter = 0;
// 	while (lst != NULL)
// 	{
// 		lst = lst->next;
// 		++counter;
// 	}
// 	return (counter);
// }