/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_heredoc.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hfafouri <hfafouri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/11 04:19:40 by hfafouri          #+#    #+#             */
/*   Updated: 2024/07/11 04:30:45 by hfafouri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	expand_heredoc(t_cmd *env, t_node **gc)
{
    while(env->heredoc != NULL)
    {
        env->heredoc->data = expand_quotes(env->heredoc->data);
        env->heredoc = env->heredoc->next;
    }
}