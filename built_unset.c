/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_unset.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sakdil < sakdil@student.42istanbul.com.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/18 18:03:45 by sakdil            #+#    #+#             */
/*   Updated: 2025/07/11 10:08:52 by sakdil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static	int remove_from_environ(int idx, char ***env)
{
	char	**e;
	int		i;

	i = idx;
	e = *env;
	while (e[i + 1] != NULL)
	{
		e[i] = e[i + 1];
		i++;
	}
	e[i] = NULL;
	return (0);
}


static	int unset_one_var(char *var, int *status, char ***env)
{
	int	idx;

	if (!is_valid_identifier(var))
	{
		printf("unset: '%s': not a valid identifier\n", var);
		*status = 1;
		return (1);
	}
	idx = find_in_environ(var, *env);
	if (idx >= 0)
	{
		if (remove_from_environ(idx, env) != 0)
			*status = 1;
	}
	return (0);
}

int	builtin_unset(int argc, char **argv, char ***env)
{
	int	i;
	int	status;

	status = 0;
	i = 1;
	if (argc < 2)
		return (0);
	while (i < argc)
	{
		unset_one_var(argv[i], &status, env);
		i++;
	}
	return (status);
}
