/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_command.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: segunes <segunes@student.42istanbul.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 20:51:02 by sakdil            #+#    #+#             */
/*   Updated: 2025/08/20 10:54:03 by segunes          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	cmd_skip_empty(char **args, int *cmd_idx, t_shell *shell)
{
	while (args[*cmd_idx] && args[*cmd_idx][0] == '\0')
		(*cmd_idx)++;
	if (!args[*cmd_idx])
	{
		if (*cmd_idx == 0)
		{
			cleanup(shell, 0);
			shell = NULL;
			exit(1);
		}
		cleanup(shell, 0);
		shell = NULL;
		exit(0);
	}
	return (0);
}

static void	run_builtin_or_exit(t_ast_tree *node, int cmd_idx,
					int in_pipeline, t_shell *shell)
{
	int	ret;

	ret = builtin(args_count(node->args + cmd_idx),
			node->args + cmd_idx, &shell->envp, shell);
	if (ret != -1)
	{
		if (in_pipeline)
			signal(SIGPIPE, SIG_DFL);
		else
			signal(SIGPIPE, SIG_IGN);
		cleanup(shell, 0);
		shell = NULL;
		exit(ret);
	}
	signal(SIGPIPE, SIG_DFL);
}

static void	exec_path(char *cmd, t_ast_tree *node, int cmd_idx, t_shell *shell)
{
	struct stat	sb;

	if (stat(cmd, &sb) == -1)
	{
		if (errno == ENOENT)
			exit_with_cmd_error(cmd, ": No such file or directory", 127, shell);
		perror(cmd);
		cleanup(shell, 0);
		shell = NULL;
		exit(127);
	}
	if (S_ISDIR(sb.st_mode))
		exit_with_cmd_error(cmd, ": Is a directory", 126, shell);
	if (access(cmd, X_OK) != 0)
		exit_with_cmd_error(cmd, ": Permission denied", 126, shell);
	execve(cmd, node->args + cmd_idx, shell->envp);
	perror("execve");
	cleanup(shell, 0);
	shell = NULL;
	exit(1);
}

static void	lookup_path(char *cmd, t_ast_tree *node,
				int cmd_idx, t_shell *shell)
{
	char	*path;

	path = find_path(cmd, shell->envp);
	if (path)
	{
		execve(path, node->args + cmd_idx, shell->envp);
		free(path);
		perror("execve");
		cleanup(shell, 0);
		shell = NULL;
		exit(1);
	}
	ft_putstr_fd(cmd, 2);
	ft_putendl_fd(": command not found", 2);
	cleanup(shell, 0);
	shell = NULL;
	ft_exit_code(127);
	exit(127);
}

void	execute_command(t_ast_tree *node, int in_pipeline, t_shell *shell)
{
	int		cmd_idx;
	char	*cmd;

	cmd_idx = 0;
	if (!node || !node->args)
	{
		cleanup(shell, 0);
		exit(1);
	}
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	cmd_skip_empty(node->args, &cmd_idx, shell);
	cmd = node->args[cmd_idx];
	if (handle_redirections(node) != 0)
	{
		cleanup(shell, 0);
		exit(1);
	}
	run_builtin_or_exit(node, cmd_idx, in_pipeline, shell);
	if (cmd[0] == '/' || cmd[0] == '.')
		exec_path(cmd, node, cmd_idx, shell);
	else
		lookup_path(cmd, node, cmd_idx, shell);
}
