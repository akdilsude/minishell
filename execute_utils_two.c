/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_utils_two.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: segunes <segunes@student.42istanbul.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/18 18:13:41 by segunes           #+#    #+#             */
/*   Updated: 2025/08/20 11:46:37 by segunes          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	exec_handle_redirs(t_ast_tree *node, t_std std)
{
	t_redir	*r;

	r = node->redir_list;
	if (r && (r->type == REDIR_IN || r->type == REDIR_OUT || r->type == APPEND))
	{
		if (handle_redirections(node) != 0)
		{
			ft_exit_code(1);
			restore_std(std);
			return (1);
		}
		restore_std(std);
	}
	return (0);
}

void	exec_command(t_ast_tree *node, int in_pipeline,
				t_std std, t_shell *shell)
{
	if (try_execute_builtin(node, in_pipeline, std, shell))
		return ;
	execute_and_wait(node, in_pipeline, shell);
	if (node->redir_list)
		free_redirections(node);
}
