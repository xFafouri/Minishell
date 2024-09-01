/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_helper.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbourziq <sbourziq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/28 23:47:50 by hfafouri          #+#    #+#             */
/*   Updated: 2024/08/30 00:38:22 by sbourziq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_strchr_hlber(char *s, int c, int *n)
{
	int	i;

	i = 0;
	*n = 0;
	while (s[i] != '\0')
	{
		if (s[i] == (char)c)
			(*n)++;
		else if ((s[i] != (char)c) && ((*n) != 0))
			return ((char *)(s + i));
		i++;
	}
	if (s[i] == (char)c)
		return ((char *)(s + i));
	return (s);
}

char	*ft_check_space(char *av, t_node **gc)
{
	int		i;
	int		j;
	char	*str;

	i = 0;
	j = 0;
	while (av[i] != '\0')
		i++;
	str = (char *)gc_malloc(gc, (i + 2) * sizeof(char));
	if (str == NULL)
		return (NULL);
	str[0] = '/';
	j = 0;
	i = 1;
	while (av[j] != '\0')
	{
		str[i] = av[j];
		i++;
		j++;
	}
	str[i] = '\0';
	return (str);
}

char	*ft_strchr(char *s, int c)
{
	int	i;

	i = 0;
	while (s[i] != '\0' && s[i] != ' ' && s[i] != 9)
	{
		if (s[i] == (char)c)
			return ((char *)(s + i));
		else
			i++;
	}
	if (s[i] == (char)c)
		return ((char *)(s + i));
	return (NULL);
}

int	count_herdoc(t_node *herdoc)
{
	int	number;
	int	i;

	number = 0;
	i = 0;
	while (herdoc != NULL)
	{
		while (((char *)(herdoc->data))[i] != '\0')
		{
			if ((((char *)(herdoc->data))[i] == '<')
				&& (((char *)(herdoc->data))[i + 1] == '<'))
			{
				number++;
				break ;
			}
			i++;
		}
		i = 0;
		herdoc = herdoc->next;
	}
	return (number);
}

int	ft_file(char *str)
{
	int	i;
	int	nb;

	i = 0;
	nb = 0;
	while (str[i])
	{
		if (str[i] == '<' && str[i + 1] == '<')
		{
			nb = 2;
			++i;
		}
		else if (str[i] == '<')
			nb = 1;
		i++;
	}
	return (nb);
}
