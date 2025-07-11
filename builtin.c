/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sakdil < sakdil@student.42istanbul.com.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 14:32:39 by segunes           #+#    #+#             */
/*   Updated: 2025/07/11 14:38:52 by sakdil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	builtin(int argc, char **argv, char **env, t_list *history)
{
	if (argc >= 1 && ft_strcmp(argv[0], "exit") == 0)
		return (builtin_exit(argc, argv));
	else if (ft_strcmp(argv[0], "env") == 0)
	{
		if (control_env(argc) == 1)
			builtin_env(env);
		return (0);
	}
	else if (ft_strcmp(argv[0], "history") == 0)
	{
		print_history(history);
		return (0);
	}
	else if (ft_strcmp(argv[0], "echo") == 0)
		return (builtin_echo(argc, argv));
	else if (ft_strcmp(argv[0], "cd") == 0)
		return (builtin_cd(argc, argv));
	else if (ft_strcmp(argv[0], "export") == 0)
		return (builtin_export(argc, argv, &env));
	else if (ft_strcmp(argv[0], "pwd") == 0)
		return (builtin_pwd());
	else if (ft_strcmp(argv[0], "unset") == 0)
		return (builtin_unset(argc, argv, &env));
	return (-1);
}
