/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: segunes <segunes@student.42istanbul.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/17 14:55:04 by segunes           #+#    #+#             */
/*   Updated: 2025/08/20 11:56:55 by segunes          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	handle_command_status(int status)
{
	int	sig;

	if (WIFSIGNALED(status))
	{
		sig = WTERMSIG(status);
		if (sig == SIGINT)
			ft_exit_code(130);
		else if (sig == SIGQUIT)
		{
			write(2, "Quit: 3\n", 8);
			ft_exit_code(131);
		}
		else if (sig == SIGPIPE)
			ft_exit_code(141);
	}
	else
		ft_exit_code(WEXITSTATUS(status));
}

int	try_execute_builtin(t_ast_tree *node, int in_pipeline,
							t_std std, t_shell *shell)
{
	int	argc;
	int	status;

	if (!in_pipeline && is_builtin(node->args[0]))
	{
		if (handle_redirections(node) != 0)
		{
			ft_exit_code(1);
			restore_std(std);
			return (1);
		}
		argc = args_count(node->args);
		status = builtin(argc, node->args, &shell->envp, shell);
		restore_std(std);
		if (status != -1)
		{
			ft_exit_code(status);
			return (1);
		}
	}
	return (0);
}

void	execute_and_wait(t_ast_tree *node, int in_pipeline, t_shell *shell)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid < 0)
	{
		perror("fork");
		ft_exit_code(1);
		return ;
	}
	else if (pid == 0)
	{
		execute_command(node, in_pipeline, shell);
		cleanup(shell, 0);
		exit(1);
	}
	else
	{
		signal(SIGINT, signal_child);
		waitpid(pid, &status, 0);
		handle_command_status(status);
		signal(SIGINT, signal_catch);
	}
}

void	free_redirections(t_ast_tree *node)
{
	t_redir	*current;
	t_redir	*next;

	current = node->redir_list;
	while (current)
	{
		next = current->next;
		if (current->type == HEREDOC && current->fd >= 0)
			close(current->fd);
		if (current->target)
			free(current->target);
		free(current);
		current = next;
	}
	node->redir_list = NULL;
}

void	executor_structure(t_ast_tree *node, int in_pipeline, t_shell *shell)
{
	t_std	std;

	std.std_in = dup(STDIN_FILENO);
	std.std_out = dup(STDOUT_FILENO);
	if (std.std_in < 0 || std.std_out < 0)
	{
		if (std.std_in >= 0)
			close(std.std_in);
		if (std.std_out >= 0)
			close(std.std_out);
		ft_exit_code(1);
		return ;
	}
	if (node->args && !node->args[0] && (node->redir_list->type == REDIR_IN
			|| node->redir_list->type == REDIR_OUT
			|| node->redir_list->type == APPEND))
		exec_handle_redirs(node, std);
	if (node->type == NODE_COMMAND)
		exec_command(node, in_pipeline, std, shell);
	else if (node->type == NODE_PIPE)
		execute_pipe(node, shell);
	close(std.std_in);
	close(std.std_out);
}
