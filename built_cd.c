/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_cd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sakdil < sakdil@student.42istanbul.com.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/18 20:09:08 by sakdil            #+#    #+#             */
/*   Updated: 2025/07/06 15:40:34 by sakdil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int builtin_cd(int argc, char **argv)
{
	if (argc > 2)
	{
		ft_putendl_fd("cd: too many arguments", 2);
		return (1);
	}
	if (argc == 1)
	{
		char *home = getenv("HOME");
		if (!home)
		{
			ft_putendl_fd("cd: HOME not set", 2);
			return (1);
		}
		if (chdir(home) != 0)
		{
			perror("cd");
			return (1);
		}
	}
	else
	{
		if (chdir(argv[1]) != 0)
		{
			ft_putstr_fd("cd: ", 2);
			ft_putstr_fd(argv[1], 2);
			ft_putendl_fd(": No such file or directory", 2);
			return (1);
		}
	}
	return (0);
}
