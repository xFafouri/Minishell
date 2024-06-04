
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// typedef struct s_node
// {
// 	char			*data;
// 	struct s_node	*next;
// }					t_node;

// typedef struct s_cmd
// {
// 	char			**cmd;
// }					t_cmd;

// int	ft_strlen2(char *str, int *n)
// {
// 	int	i;

// 	i = 0;
// 	while (str[i] != '\0')
// 	{
// 		if (str[i] == '+' && str[i + 1] != '=')
// 		{
// 			*n = 1;
// 			break ;
// 		}
// 		i++;
// 	}
// 	return (i);
// }

// char	*ft_strjoin(t_node **gc, const char *s1, const char *s2)
// {
// 	char	*result;
// 	size_t	len1;
// 	size_t	len2;

// 	len1 = strlen(s1);
// 	len2 = strlen(s2);
// 	result = malloc(len1 + len2 + 1);
// 	if (!result)
// 		return (NULL);
// 	strcpy(result, s1);
// 	strcat(result, s2);
// 	// Assume gc_add is a function to add allocated memory to garbage collector list
// 	// gc_add(gc, result);
// 	return (result);
// }

// void	ft_find(char *str, t_node **envp, int *n, t_node **gc)
// {
// 	char	*ptr;
// 	char	*new_data;

// 	ptr = strchr(str, '=');
// 	if (ptr != NULL)
// 	{
// 		while ((*envp) != NULL)
// 		{
// 			if (strncmp((*envp)->data, str, ft_strlen2(str, n)) == 0)
// 			{
// 				printf("found\n");
// 				if (*n == 1)
// 				{
// 					new_data = ft_strjoin(gc, (*envp)->data, ptr + 1);
// 					// Skip the '=' character
// 					if (new_data)
// 						(*envp)->data = new_data;
// 				}
// 				break ;
// 			}
// 			(*envp) = (*envp)->next;
// 		}
// 	}
// }

// t_node	*ft_lstnew(char *data)
// {
// 	t_node	*new_node;

// 	new_node = malloc(sizeof(t_node));
// 	if (!new_node)
// 		return (NULL);
// 	new_node->data = strdup(data);
// 	new_node->next = NULL;
// 	return (new_node);
// }

// void	ft_lstadd_back(t_node **lst, t_node *new_node)
// {
// 	t_node	*temp;

// 	if (!lst || !new_node)
// 		return ;
// 	if (!*lst)
// 		*lst = new_node;
// 	else
// 	{
// 		temp = *lst;
// 		while (temp->next)
// 			temp = temp->next;
// 		temp->next = new_node;
// 	}
// }

// void	ft_export(t_node **gc, t_cmd *token, t_node **envp)
// {
// 	int		i;
// 	int		n;
// 	t_node	*envp_head;

// 	envp_head = *envp;
// 	i = 1;
// 	while (token->cmd[i])
// 	{
// 		n = 0;
// 		ft_find(token->cmd[i], envp, &n, gc);
// 		if (n != 1)
// 		{
// 			ft_lstadd_back(envp, ft_lstnew(token->cmd[i]));
// 		}
// 		i++;
// 		*envp = envp_head;
// 	}
// }

// int	main(int argc, char **argv, char **envp)
// {
// 	t_node	*gc;
// 	t_node	*all_env;
// 	t_cmd	ev;
// 	int		i;
// 	t_node	*temp;

// 	gc = NULL;
// 	all_env = NULL;
// 	ev.cmd = argv;
// 	i = 0;
// 	while (envp[i] != NULL)
// 	{
// 		ft_lstadd_back(&all_env, ft_lstnew(envp[i]));
// 		i++;
// 	}
// 	if (argc > 1 && strcmp(argv[1], "export") == 0)
// 	{
// 		ft_export(&gc, &ev, &all_env);
// 		temp = all_env;
// 		while (temp)
// 		{
// 			printf("%s\n", temp->data);
// 			temp = temp->next;
// 		}
// 	}
// 	return (0);
// }
