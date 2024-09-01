/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_error_export.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbourziq <sbourziq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/30 00:37:39 by sbourziq          #+#    #+#             */
/*   Updated: 2024/08/31 23:33:14 by sbourziq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_error_message(char *name, char *value, char *message)
{
	ft_putstr_fd("export: ", 2);
	if (name != NULL)
		ft_putstr_fd(name, 2);
	if (value != NULL)
		ft_putstr_fd(value, 2);
	ft_putstr_fd(message, 2);
}

int	validate_empty_value(char *name, char *value)
{
	if (value == NULL || value[0] == '\0')
	{
		if (name[ft_strlen(name) - 1] == '+')
		{
			print_error_message(name, value, ": not a valid identifier\n");
			return (0);
		}
	}
	return (1);
}

int	validate_name(char *name, char *value)
{
	if (name == NULL || name[0] == '\0' || (ft_isalpha1(name) == 0))
	{
		if (name[0] == '-')
			print_error_message(name, value, ": invalid option\n");
		else
			print_error_message(name, value, ": not a valid identifier\n");
		return (0);
	}
	return (1);
}

int	ft_validate_export_name(char *name, char *value)
{
	if (!validate_empty_value(name, value))
		return (0);
	if (!validate_name(name, value))
		return (0);
	return (1);
}

int	ft_serch_rid(char *line)
{
	int	i;

	i = 0;
	while (line[i] != '\0')
	{
		if (line[i] == '<' || line[i] == '>')
			return (0);
		i++;
	}
	return (1);
}
