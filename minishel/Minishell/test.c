#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void	parse_export_command(const char *command, char **name, char **value)
{
	char	*equal_sign;

	equal_sign = strchr(command, '=');
	if (equal_sign != NULL)
	{
		*name = strndup(command, equal_sign - command);
		*value = strdup(equal_sign + 1);
	}
	else
	{
		*name = strdup(command);
		*value = NULL;
	}
}
int	export_variable(const char *name, const char *value)
{
	if (name == NULL || value == NULL)
	{
		fprintf(stderr, "Invalid export command.\n");
		return (-1);
	}
	return (setenv(name, value, 1));
}

int	ft_export(const char *command)
{
	char	*name;
	char	*value;

	name = NULL;
	value = NULL;
	// Parse the export command
	parse_export_command(command, &name, &value);
	// Validate the variable name
	if (name == NULL || strlen(name) == 0 || strpbrk(name, " \t\n"))
	{
		fprintf(stderr, "Invalid variable name.\n");
		free(name);
		free(value);
		return (-1);
	}
	// Set or update the environment variable
	if (export_variable(name, value) != 0)
	{
		fprintf(stderr, "Failed to export variable.\n");
		free(name);
		free(value);
		return (-1);
	}
	// Free allocated memory
	free(name);
	free(value);
	return (0);
}

void	process_command(const char *command)
{
	if (strncmp(command, "export", 6) == 0)
	{
		ft_export(command + 7); // Pass the part after "export "
	}
	else
	{
		// Handle other commands
	}
}
int	main(void)
{
	char	*command;

	command = "export PATH=/usr/local/bin";
	ft_export(command);
	// Check if the environment variable was set
	printf("PATH=%s\n", getenv("PATH"));
	return (0);
}
