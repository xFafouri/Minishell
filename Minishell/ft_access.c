/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_access.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hfafouri <hfafouri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/28 23:47:32 by hfafouri          #+#    #+#             */
/*   Updated: 2024/08/28 23:47:33 by hfafouri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_access_to_path(t_node **gc, char **str, char *path_cmd)
{
	if (access(path_cmd, F_OK) == 0)
	{
		if (access(path_cmd, X_OK) == 0)
			*str = path_cmd;
		else
			(perror(path_cmd), ft_lstclear(gc), exit(126));
	}
}
