#include "minishell.h"

int	ft_validate_export_name(char *name, char *value)
{
	if (value == NULL || value[0] == '\0')
	{
		if (name[ft_strlen(name) - 1] == '+')
		{
			ft_putstr_fd("export: ", 2);
			if (name != NULL)
				ft_putstr_fd(name, 2);
			if (value != NULL)
				ft_putstr_fd(value, 2);
			ft_putstr_fd(": not a valid identifier\n", 2);
			return (0);
		}
	}
	if ((name == NULL || name[0] == '\0' || (ft_isalpha1(name) == 0)))
	{
		ft_putstr_fd("export: ", 2);
		if (name != NULL)
			ft_putstr_fd(name, 2);
		if (value != NULL)
			ft_putstr_fd(value, 2);
		if (name[0] == '-')
			ft_putstr_fd(": invalid option\n", 2);
		else
			ft_putstr_fd(": not a valid identifier\n", 2);
		return (0);
	}
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