/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sakdil <sakdil@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 16:59:10 by segunes           #+#    #+#             */
/*   Updated: 2026/06/18 16:48:22 by sakdil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**heredoc_signal(int flag, t_shell *shell, const char *target)
{
	static t_shell	*tmp;
	static char		*tmp_target;

	if (flag == 0)
	{
		tmp = malloc(sizeof(t_shell));
		tmp->envp = copy_env(shell->envp);
		tmp_target = ft_strdup(target);
	}
	if (flag == 1)
	{
		ft_free(tmp->envp);
		free(tmp);
		free(tmp_target);
	}
	if (flag == 2)
		return (tmp->envp);
	if (flag == 3)
		return (&tmp_target);
	return (0);
}

int	heredocs_prepare(t_ast_tree *node, t_shell *shell)
{
	t_redir	*r;
	int		fd;

	r = node->redir_list;
	while (r)
	{
		if (r->type == HEREDOC)
		{
			fd = handle_heredoc(r->target, r->quoted, shell);
			if (fd < 0)
				return (1);
			r->fd = fd;
		}
		r = r->next;
	}
	return (0);
}
