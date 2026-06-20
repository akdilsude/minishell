/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: segunes <segunes@student.42istanbul.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 20:54:37 by sakdil            #+#    #+#             */
/*   Updated: 2025/08/12 14:56:58 by segunes          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	find_pipe_split(t_token *tokens,
						t_token **left, t_token **mid, t_token **right)
{
	t_token	*prev;

	prev = NULL;
	*left = tokens;
	*mid = NULL;
	*right = NULL;
	while (tokens)
	{
		if (tokens->type == PIPE)
		{
			if (!tokens->next)
				return (1);
			*mid = tokens;
			*right = tokens->next;
			tokens->next = NULL;
			if (prev)
				prev->next = NULL;
			return (0);
		}
		prev = tokens;
		tokens = tokens->next;
	}
	return (0);
}

static int	count_words(t_token *cur)
{
	int	count;

	count = 0;
	while (cur)
	{
		if (cur->type == WORD)
			count++;
		cur = cur->next;
	}
	return (count);
}

static t_ast_tree	*build_cmd_node(t_token *left_token)
{
	t_ast_tree	*node;
	char		**args;
	int			count;

	count = count_words(left_token);
	node = create_cmd_node_with_args(count, &args);
	if (!node)
		return (NULL);
	if (fill_cmd_from_tokens(node, left_token, args) != 0)
	{
		free(args);
		free(node);
		free_token_list(left_token);
		return (NULL);
	}
	node->args = args;
	free_token_list(left_token);
	return (node);
}

static t_ast_tree	*make_pipe_node(t_token *left, t_token *right, t_token *mid,
				t_shell *shell)
{
	t_ast_tree	*node;

	node = malloc(sizeof(t_ast_tree));
	if (!node)
		return (NULL);
	node->type = NODE_PIPE;
	node->redir_list = NULL;
	node->args = NULL;
	node->left = ft_build_ast(left, shell);
	node->right = ft_build_ast(right, shell);
	if (mid)
	{
		if (mid->value)
			free(mid->value);
		free(mid);
	}
	if (!node->left || !node->right)
	{
		printf("syntax error near unexpected token\n");
		free(node);
		return (NULL);
	}
	return (node);
}

t_ast_tree	*ft_build_ast(t_token *tokens, t_shell *shell)
{
	t_token	*left;
	t_token	*right;
	t_token	*mid;

	if (!tokens)
		return (NULL);
	if (find_pipe_split(tokens, &left, &mid, &right))
	{
		printf("syntax error near unexpected token\n");
		free_token_list(tokens);
		return (NULL);
	}
	if (mid)
		return (make_pipe_node(left, right, mid, shell));
	return (build_cmd_node(tokens));
}
