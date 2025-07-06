/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: segunes <segunes@student.42istanbul.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/09 12:29:24 by segunes           #+#    #+#             */
/*   Updated: 2024/11/09 16:47:04 by segunes          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static char	**get_free(char **array, size_t i)
{
	while (array[i] != NULL)
		free(array[i++]);
	free(array);
	return (NULL);
}

static int	word_count(const char *s, char c)
{
	int	i;

	i = 0;
	while (*s != '\0')
	{
		while (*s == c && *s != '\0')
			s++;
		if (*s != c && *s != '\0')
			i++;
		while (*s != c && *s != '\0')
			s++;
	}
	return (i);
}

static char	*get_mem(const char *s, char c)
{
	char	*arr;
	int		i;
	int		j;

	i = 0;
	j = 0;
	while (s[i] != c && s[i] != '\0')
		i++;
	arr = (char *)malloc(i + 1);
	if (arr == NULL)
		return (NULL);
	while (j < i)
	{
		arr[j] = s[j];
		j++;
	}
	arr[j] = '\0';
	return (arr);
}

char	**ft_split(const char *s, char c)
{
	char	**array;
	int		j;

	j = 0;
	array = (char **)malloc((word_count(s, c) + 1) * sizeof(char *));
	if (array == NULL)
		return (NULL);
	while (*s != '\0')
	{
		while (*s == c && *s != '\0')
			s++;
		if (*s != c && *s != '\0')
		{
			array[j] = get_mem(s, c);
			if (array[j] == NULL)
				return (get_free(array, 0));
			j++;
		}
		while (*s != c && *s != '\0')
			s++;
	}
	array[j] = NULL;
	return (array);
}
