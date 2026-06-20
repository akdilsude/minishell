/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: segunes <segunes@student.42istanbul.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 21:14:58 by sakdil            #+#    #+#             */
/*   Updated: 2025/08/14 14:27:07 by segunes          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*strip_quotes(char *str)
{
	size_t	len;

	if (!str)
		return (NULL);
	len = ft_strlen(str);
	if ((str[0] == '"' && str[len - 1] == '"')
		|| (str[0] == '\'' && str[len - 1] == '\''))
		return (ft_substr(str, 1, len - 2));
	return (ft_strdup(str));
}

static void	redir_fill_from_next_token(t_redir *r, t_token **current)
{
	char	*raw;
	char	*name;

	if (*current && (*current)->value)
	{
		raw = strip_quotes((*current)->value);
		name = normalize_filename(raw);
		if (name != raw)
			free(raw);
		r->target = name;
		r->quoted = (*current)->quoted;
	}
	else
		r->target = NULL;
	r->next = NULL;
	if (*current)
		*current = (*current)->next;
}

static t_redir	*new_redir_from(t_token **current)
{
	t_redir	*r;

	r = malloc(sizeof(t_redir));
	if (!r)
		return (NULL);
	r->type = (*current)->type;
	r->fd = -1;
	r->quoted = 0;
	*current = (*current)->next;
	redir_fill_from_next_token(r, current);
	return (r);
}

t_ast_tree	*create_cmd_node_with_args(int count, char ***args_out)
{
	t_ast_tree	*node;
	char		**args;

	node = malloc(sizeof(t_ast_tree));
	if (!node)
		return (NULL);
	node->type = NODE_COMMAND;
	node->redir_list = NULL;
	node->left = NULL;
	node->right = NULL;
	args = malloc(sizeof(char *) * (count + 1));
	if (!args)
	{
		free(node);
		return (NULL);
	}
	*args_out = args;
	return (node);
}

int	fill_cmd_from_tokens(t_ast_tree *node, t_token *current, char **args)
{
	int		i;
	t_redir	*r;

	i = 0;
	while (current)
	{
		if (current->type == WORD)
			args[i++] = ft_strdup(current->value);
		else if (current->type == APPEND || current->type == HEREDOC
			|| current->type == REDIR_IN || current->type == REDIR_OUT)
		{
			r = new_redir_from(&current);
			if (!r)
				return (-1);
			redir_push_back(&node->redir_list, r);
			continue ;
		}
		current = current->next;
	}
	args[i] = NULL;
	return (0);
}
