#include "minishell.h"

t_node	*ft_lstlast(t_node *lst)
{
	if (!lst)
		return (NULL);
	while (lst->next != NULL)
		lst = lst->next;
	return (lst);
}

t_node	*ft_lstnew1(void *ptr)
{
	t_node	*str;

	str = (t_node *)malloc(sizeof(t_node));
	if (str == NULL || ptr == NULL)
	{
		free(str);
		return (NULL);
	}
	str->data = ptr;
	str->next = NULL;
	return (str);
}

t_node	*ft_lstnew(void *ptr, t_node **gc)
{
	t_node	*str;

	str = (t_node *)gc_malloc(gc, sizeof(t_node));
	if (str == NULL || ptr == NULL)
	{
		free(str);
		return (NULL);
	}
	str->data = ptr;
	str->next = NULL;
	return (str);
}


void	ft_lstadd_back(t_node **lst, t_node *new)
{
	t_node	*last;

	if (!lst || !new)
		return ;
	if (*lst == NULL)
	{
		*lst = new;
		return ;
	}
	last = *lst;
	while (last->next != NULL)
		last = last->next;
	last->next = new;
}

void	ft_lstclear(t_node **lst)
{
	t_node	*head;
	t_node	*copy;

	if (lst && *lst)
	{
		head = *lst;
		while (head != NULL)
		{
			copy = head;
			free(head->data);
			head = head->next;
			free(copy);
		}
		*lst = NULL;
	}
}

void	*gc_malloc(t_node **gc, int size)
{
	void *ptr;

	ptr = malloc(size);
	if (ptr == NULL)
	{
		free(ptr);
		ft_lstclear(gc);
		exit(1);
	}
	ft_lstadd_back(gc, ft_lstnew1(ptr));
	return (ptr);
}
