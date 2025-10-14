/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: segunes <segunes@student.42istanbul.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 17:40:51 by sakdil            #+#    #+#             */
/*   Updated: 2025/08/20 10:43:57 by segunes          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_shell	*init_shell(char **env)
{
	t_shell	*shell;

	shell = malloc(sizeof(t_shell));
	if (!shell)
		return (NULL);
	shell->input = NULL;
	shell->tokens = NULL;
	shell->ast = NULL;
	shell->envp = copy_env(env);
	if (!shell->envp)
	{
		free(shell);
		return (NULL);
	}
	return (shell);
}

static int	read_and_prepare(t_shell *shell)
{
	signal(SIGINT, signal_catch);
	signal(SIGQUIT, SIG_IGN);
	shell->input = readline("minishell$ ");
	if (shell->input == NULL)
	{
		write(1, "exit\n", 5);
		cleanup(shell, 0);
		exit(0);
	}
	if (is_only_spaces(shell->input) && *shell->input != '\0')
	{
		add_history(shell->input);
		cleanup(shell, 1);
		return (1);
	}
	if (*shell->input)
		add_history(shell->input);
	return (0);
}

static int	tokenize_parse_build(t_shell *shell)
{
	shell->tokens = tokenize_input(shell->input, shell->envp);
	if (!shell->tokens)
	{
		ft_exit_code(0);
		cleanup(shell, 1);
		return (1);
	}
	if (ft_parser(shell->tokens))
	{
		cleanup(shell, 1);
		return (1);
	}
	shell->ast = ft_build_ast(shell->tokens, shell);
	shell->tokens = NULL;
	if (!shell->ast)
	{
		cleanup(shell, 1);
		return (1);
	}
	if (prepare_all_heredocs(shell->ast, shell) != 0)
	{
		cleanup(shell, 1);
		return (1);
	}
	return (0);
}

static void	execute_and_cleanup(t_shell *shell)
{
	executor_structure(shell->ast, 0, shell);
	if (shell->ast)
	{
		free_ast_tree(shell->ast);
		shell->ast = NULL;
	}
	if (shell->input)
	{
		free(shell->input);
		shell->input = NULL;
	}
}

int	main(int argc, char **argv, char **env)
{
	t_shell	*shell;
	int		std_in;
	int		std_out;

	(void)argv;
	(void)argc;
	std_in = dup(STDIN_FILENO);
	std_out = dup(STDOUT_FILENO);
	shell = init_shell(env);
	if (!shell)
		return (1);
	ft_exit_code(0);
	while (1)
	{
		if (read_and_prepare(shell))
			continue ;
		if (tokenize_parse_build(shell))
			continue ;
		execute_and_cleanup(shell);
		dup2(std_in, STDIN_FILENO);
		dup2(std_out, STDOUT_FILENO);
	}
	cleanup(shell, 0);
	return (ft_exit_code(-1));
}
