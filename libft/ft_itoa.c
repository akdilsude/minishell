/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: segunes <segunes@student.42istanbul.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/09 12:54:17 by segunes           #+#    #+#             */
/*   Updated: 2024/11/09 12:55:39 by segunes          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_order(int n)
{
	int		i;
	float	j;

	j = (float)n;
	i = 0;
	if (n == 0)
		return (1);
	while (j >= 1 || j <= -1)
	{
		j /= 10;
		i++;
		if (j > -1 && j < 0)
			i++;
	}
	return (i);
}

char	*ft_itoa(int n)
{
	char	*array;
	int		j;

	j = ft_order(n);
	if (n == -2147483648)
		return (ft_strdup("-2147483648"));
	array = (char *)malloc(j + 1);
	if (array == NULL)
		return (NULL);
	array[j] = '\0';
	if (n < 0)
	{
		array[0] = '-';
		n *= -1;
	}
	if (n == 0)
		array[0] = '0';
	while (n > 0)
	{
		array[--j] = (n % 10) + '0';
		n /= 10;
	}
	return (array);
}
