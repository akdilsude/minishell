/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: segunes <segunes@student.42istanbul.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/10 22:08:06 by sakdil            #+#    #+#             */
/*   Updated: 2025/08/12 14:51:34 by segunes          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_tokens(t_token *tokens)
{
	t_token	*current;
	t_token	*next;

	if (!tokens)
		return ;
	current = tokens;
	while (current)
	{
		next = current->next;
		if (current->value)
		{
			free(current->value);
			current->value = NULL;
		}
		free(current);
		current = next;
	}
}

void	ft_free(char **envp)
{
	int	i;

	i = 0;
	while (envp[i])
	{
		free(envp[i]);
		i++;
	}
	free(envp);
}

static void	free_redir_list(t_redir *redir)
{
	t_redir	*tmp;

	while (redir)
	{
		tmp = redir;
		redir = redir->next;
		if (tmp->type == HEREDOC && tmp->fd >= 0)
			close(tmp->fd);
		if (tmp->target)
			free(tmp->target);
		free(tmp);
	}
}

static void	free_ast_tree_continue(t_ast_tree *node)
{
	int	i;

	i = 0;
	if (node->args)
	{
		while (node->args[i])
		{
			free(node->args[i]);
			i++;
		}
		free(node->args);
	}
	if (node->redir_list)
	{
		free_redir_list(node->redir_list);
		node->redir_list = NULL;
	}
}

void	free_ast_tree(t_ast_tree *node)
{
	if (!node)
		return ;
	free_ast_tree_continue(node);
	if (node->left)
	{
		free_ast_tree(node->left);
		node->left = NULL;
	}
	if (node->right)
	{
		free_ast_tree(node->right);
		node->right = NULL;
	}
	free(node);
}
