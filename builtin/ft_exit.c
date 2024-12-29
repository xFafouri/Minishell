/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbourziq <sbourziq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/31 18:56:28 by sbourziq          #+#    #+#             */
/*   Updated: 2024/09/02 11:27:39 by sbourziq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static long	ft_process_digits(const char *str, int *i, int sign, char **endptr)
{
	long	result;

	result = 0;
	while (str[*i] != '\0')
	{
		if (str[*i] >= '0' && str[*i] <= '9')
		{
			if (result > (LONG_MAX - (str[*i] - '0')) / 10)
			{
				*endptr = (char *)str + *i;
				if (sign > 0)
					return (LONG_MAX);
				return (LONG_MIN);
			}
			result = result * 10 + (str[*i] - '0');
			(*i)++;
		}
		else
			break ;
	}
	return (result);
}

long	ft_strtol(char *str, char **endptr)
{
	long	result;
	int		i;
	int		sign;

	i = 0;
	sign = 1;
	while ((str[i] >= 9 && str[i] <= 13) || (str[i] == 32))
		i++;
	if (str[i] == '-' || str[i] == '+')
		if (str[i++] == '-')
			sign = -1;
	result = ft_process_digits(str, &i, sign, endptr);
	*endptr = str + i;
	return (result * sign);
}

void	ft_exit_numeric_error(t_cmd *token, t_node **gc)
{
	ft_putstr_fd("minishell: exit: ", 2);
	ft_putstr_fd(token->cmd[1], 2);
	ft_putendl_fd(": numeric argument required", 2);
	ft_lstclear(gc);
	ft_lstclear(&token->addres_fd);
	exit(2);
}

void	ft_exit_too_many_args(t_cmd *token)
{
	ft_putendl_fd("minishell: exit: too many arguments", 2);
	token->status = 1;
}

void	ft_exit(t_node **gc, t_cmd *token)
{
	long	nb;
	char	*endptr;

	nb = 0;
	endptr = NULL;
	if (token->flag_file != 1)
		ft_putendl_fd("exit", 1);
	if (token->cmd[1] == NULL)
	{
		ft_lstclear(gc);
		ft_lstclear(&token->addres_fd);
		exit(token->status);
	}
	nb = ft_strtol(token->cmd[1], &endptr);
	if (*endptr != '\0')
		ft_exit_numeric_error(token, gc);
	if (token->cmd[2] != NULL)
	{
		ft_exit_too_many_args(token);
		return ;
	}
	ft_lstclear(gc);
	ft_lstclear(&token->addres_fd);
	exit((int)(nb & 0xFF));
}
