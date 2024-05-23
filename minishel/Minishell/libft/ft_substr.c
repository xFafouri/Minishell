/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hfafouri <hfafouri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/05 16:32:44 by hfafouri          #+#    #+#             */
/*   Updated: 2024/05/23 19:51:16 by hfafouri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char		*sub;
	size_t		i;

	if (s == NULL)
		return (NULL);
	i = 0;
	if (start >= ft_strlen(s))
		return (ft_strdup(""));
	if (len > ft_strlen (s) - start)
		len = ft_strlen(s) - start;
	sub = (char *)malloc(sizeof(char) * (len + 1));
	if (!sub)
		return (NULL);
	while (i < len)
	{
		sub[i] = s[start + i];
		i++;
	}
	sub[len] = '\0';
	return ((char *)sub);
}
/*#include <stdio.h>
int main ()
{
	const char *str = "hello, hamza!";
	char *r = ft_substr(str, 0,5);

	printf("%s", r);
}*/
