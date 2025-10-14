/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: segunes <segunes@student.42istanbul.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 15:29:22 by segunes           #+#    #+#             */
/*   Updated: 2025/08/25 11:57:01 by segunes          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int is_only_spaces(char *str)
{
	while (*str)
	{
		if (*str != ' ' && *str != '\t')
			return (0);
		str++;
	}
	return (1);
}

int args_count(char **args)
{
	int i;

	i = 0;
	while (args[i])
		i++;
	return (i);
}

int ft_exit_code(int temp)
{
	static int exit_code;

	if (temp == -1)
	{
		return (exit_code);
	}
	exit_code = temp;
	return (exit_code);
}

static void free_iter_state(t_shell *shell)
{
	if (shell->ast)
	{
		free_ast_tree(shell->ast);
		shell->ast = NULL;
	}
	if (shell->tokens)
	{
		free_tokens(shell->tokens);
		shell->tokens = NULL;
	}
	if (shell->input)
	{
		free(shell->input);
		shell->input = NULL;
	}
}

void cleanup(t_shell *shell, int mode)
{
	if (!shell)
		return;
	if (mode == 1)
	{
		free_iter_state(shell);
		return;
	}
	free_iter_state(shell);
	if (shell->envp)
	{
		ft_free(shell->envp);
		shell->envp = NULL;
	}
	free(shell);
}
