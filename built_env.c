/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_env.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sakdil < sakdil@student.42istanbul.com.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/18 20:10:19 by sakdil            #+#    #+#             */
/*   Updated: 2025/07/06 16:02:51 by sakdil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


int control_env(int argc)
{
	if (argc > 1)
	{	
		write(1, "env: too many arguments\n", 23);
		return (0);
	}
	return (1);	
}

int builtin_env(char **env)
{
	int i;

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
char **dup_env(char **env)
{
    int     i;
    char    **new_env;

    i = 0;
    while (env[i])
        i++;
    new_env = malloc(sizeof(char *) * (i + 1));
    if (!new_env)
        return NULL;
    i = 0;
    while (env[i])
    {
        new_env[i] = ft_strdup(env[i]);
        if (!new_env[i])
        {
            while (i-- > 0)
                free(new_env[i]);
            free(new_env);
            return NULL;
        }
        i++;
    }
    new_env[i] = NULL;
    return new_env;
}

void free_env(char **env)
{
    int i;

    if (!env)
        return;
    i = 0;
    while (env[i])
    {
        free(env[i]);
        i++;
    }
    free(env);
}
