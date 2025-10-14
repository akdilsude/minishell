/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_redirection.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: segunes <segunes@student.42istanbul.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 20:53:42 by sakdil            #+#    #+#             */
/*   Updated: 2025/08/20 11:24:46 by segunes          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	reopen_path(int *dst_fd, const char *path, int flags, mode_t mode)
{
	if (*dst_fd != -1)
		close(*dst_fd);
	*dst_fd = open(path, flags, mode);
	if (*dst_fd < 0)
	{
		perror(path);
		return (1);
	}
	return (0);
}

static int	set_heredoc_fd(int src_fd, int *fd_in)
{
	if (src_fd < 0)
	{
		ft_putendl_fd("heredoc: invalid fd", 2);
		return (1);
	}
	if (*fd_in != -1)
		close(*fd_in);
	*fd_in = src_fd;
	return (0);
}

static int	apply_redirection_entry(t_redir *redir, int *fd_in, int *fd_out)
{
	if (!redir->target || redir->target[0] == '\0')
	{
		printf("ambiguous redirect\n");
		return (1);
	}
	if (redir->type == REDIR_IN)
		return (reopen_path(fd_in, redir->target, O_RDONLY, 0));
	if (redir->type == REDIR_OUT)
		return (reopen_path(fd_out, redir->target,
				O_WRONLY | O_CREAT | O_TRUNC, 0644));
	if (redir->type == APPEND)
		return (reopen_path(fd_out, redir->target,
				O_WRONLY | O_CREAT | O_APPEND, 0644));
	if (redir->type == HEREDOC)
	{
		if (set_heredoc_fd(redir->fd, fd_in))
			return (1);
		redir->fd = -1;
		return (0);
	}
	return (0);
}

static int	dup_stdio_and_close(int fd, int dst_stdio, const char *who)
{
	if (fd == -1)
		return (0);
	if (dup2(fd, dst_stdio) == -1)
	{
		perror(who);
		close(fd);
		return (1);
	}
	close(fd);
	return (0);
}

int	handle_redirections(t_ast_tree *node)
{
	t_redir	*redir;
	int		fd_in;
	int		fd_out;

	redir = node->redir_list;
	fd_in = -1;
	fd_out = -1;
	while (redir)
	{
		if (apply_redirection_entry(redir, &fd_in, &fd_out))
		{
			if (fd_in != -1)
				close(fd_in);
			if (fd_out != -1)
				close(fd_out);
			return (1);
		}
		redir = redir->next;
	}
	if (dup_stdio_and_close(fd_in, STDIN_FILENO, "dup"))
		return (1);
	if (dup_stdio_and_close(fd_out, STDOUT_FILENO, "dup"))
		return (1);
	return (0);
}
