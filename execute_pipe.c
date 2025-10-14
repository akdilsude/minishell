/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_pipe.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: segunes <segunes@student.42istanbul.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 20:53:32 by sakdil            #+#    #+#             */
/*   Updated: 2025/08/20 10:57:34 by segunes          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	handle_pipe_status(int status)
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
		{
			ft_exit_code(141);
		}
		else
			ft_exit_code(128 + sig);
	}
	else
	{
		ft_exit_code(WEXITSTATUS(status));
	}
}

static void	child_run_with_pipe(t_ast_tree *node, int pipefd[2],
				char *side, t_shell *shell)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	if (ft_strcmp(side, "left") == 0)
	{
		dup2(pipefd[1], STDOUT_FILENO);
		close(pipefd[0]);
		close(pipefd[1]);
		executor_structure(node->left, 1, shell);
	}
	else
	{
		dup2(pipefd[0], STDIN_FILENO);
		close(pipefd[1]);
		close(pipefd[0]);
		executor_structure(node->right, 1, shell);
	}
	cleanup(shell, 0);
	shell = NULL;
	exit(ft_exit_code(-1));
}

static pid_t	create_child_with_pipe(t_ast_tree *node, int pipefd[2],
				char *side, t_shell *shell)
{
	pid_t	pid;

	pid = fork();
	if (pid < 0)
	{
		close(pipefd[0]);
		close(pipefd[1]);
		ft_exit_code(1);
		return (-1);
	}
	if (pid == 0)
		child_run_with_pipe(node, pipefd, side, shell);
	return (pid);
}

void	execute_pipe(t_ast_tree *node, t_shell *shell)
{
	int		pipefd[2];
	pid_t	pid1;
	pid_t	pid2;
	int		status1;
	int		status2;

	if (pipe(pipefd) == -1)
	{
		perror("pipe");
		ft_exit_code(1);
		return ;
	}
	pid1 = create_child_with_pipe(node, pipefd, "left", shell);
	pid2 = create_child_with_pipe(node, pipefd, "right", shell);
	close(pipefd[0]);
	close(pipefd[1]);
	signal(SIGINT, SIG_IGN);
	waitpid(pid1, &status1, 0);
	waitpid(pid2, &status2, 0);
	signal(SIGINT, signal_catch);
	handle_pipe_status(status2);
}
