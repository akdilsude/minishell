/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_cd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sakdil < sakdil@student.42istanbul.com.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/18 20:09:08 by sakdil            #+#    #+#             */
/*   Updated: 2025/08/20 00:04:24 by sakdil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	builtin_cd_home(t_shell *shell)
{
	const char	*home;

	home = ms_getenv("HOME", shell->envp);
	if (!home || !*home)
	{
		ft_putendl_fd("cd: HOME not set", 2);
		return (1);
	}
	if (chdir(home) != 0)
	{
		perror("cd");
		return (1);
	}
	return (0);
}

static int	builtin_cd_path(char *path)
{
	if (chdir(path) != 0)
	{
		perror("cd");
		return (1);
	}
	return (0);
}

int	builtin_cd(int argc, char **argv, t_shell *shell)
{
	if (argc > 2)
	{
		ft_putendl_fd("cd: too many arguments", 2);
		return (1);
	}
	if (argc == 1)
		return (builtin_cd_home(shell));
	else
		return (builtin_cd_path(argv[1]));
}
