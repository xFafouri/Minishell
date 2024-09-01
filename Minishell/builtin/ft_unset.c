/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbourziq <sbourziq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/27 21:26:00 by hfafouri          #+#    #+#             */
/*   Updated: 2024/09/01 11:56:19 by sbourziq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	ft_remove_key_env(t_cmd *token, char *str)
{
	int		i;
	char	*key;
	t_node	*ft;

	i = 0;
	ft = token->addres_fd;
	while (token->env[i] != NULL)
	{
		key = ft_substr(token->env[i], 0, ft_strlen_untile_char(token->env[i],
					'='), &ft);
		if (ft_strcmp(key, str) == 0)
		{
			while (token->env[i + 1] != NULL)
			{
				token->env[i] = token->env[i + 1];
				i++;
			}
			token->env[i] = NULL;
			return ;
		}
		i++;
	}
}

void	ft_remove_key_export(t_cmd *token, char *str)
{
	t_env	*head;
	t_env	*prev;

	head = token->addres_env;
	prev = NULL;
	while (head != NULL)
	{
		if (ft_strcmp(head->name, str) == 0)
		{
			if (prev == NULL)
				token->addres_env = head->next;
			else
				prev->next = head->next;
			return ;
		}
		prev = head;
		head = head->next;
	}
}

void	ft_unset(t_cmd *token)
{
	int	i;

	i = 0;
	while (token->cmd[i] != NULL)
	{
		ft_remove_key_export(token, token->cmd[i]);
		ft_remove_key_env(token, token->cmd[i]);
		i++;
	}
}
