/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_env.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sakdil < sakdil@student.42istanbul.com.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/18 20:10:19 by sakdil            #+#    #+#             */
/*   Updated: 2025/07/12 00:42:36 by sakdil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	control_env(int argc)
{
	if (argc > 1)
	{	
		write(1, "env: too many arguments\n", 23);
		return (0);
	}
	return (1);	
}

int	builtin_env(char **env)
{
	int	i;

	i = 0;
	if (!env)
	{
		printf("env: environment is NULL\n");
		return (1);
	}
	while (env[i])
	{
		if (ft_strchr(env[i], '='))
			printf("%s\n", env[i]);
		i++;
	}
	return (0);
}
