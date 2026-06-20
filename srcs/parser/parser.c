/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sakdil < sakdil@student.42istanbul.com.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 12:31:48 by segunes           #+#    #+#             */
/*   Updated: 2025/08/04 20:14:47 by sakdil           ###   ########.fr       */
/*       																	  */
/* ************************************************************************** */

#include "minishell.h"

void	free_token_list(t_token *cur)
{
	t_token	*n;

	while (cur)
	{
		n = cur->next;
		if (cur->value)
			free(cur->value);
		free(cur);
		cur = n;
	}
}

static int	ft_last(t_token *input)
{
	t_token	*last;

	last = input;
	while (last && last->next)
		last = last->next;
	if (last->type == APPEND || last->type == REDIR_IN
		|| last->type == REDIR_OUT
		|| last->type == HEREDOC || last->type == PIPE)
	{
		printf("syntax error near unexpected token\n");
		return (1);
	}
	return (0);
}

static int	is_invalid_redir_target(t_token *token)
{
	if (!token || token->type != WORD)
		return (1);
	if (!token->value || token->value[0] == '\0')
		return (1);
	if (token->type == REDIR_IN || token->type == REDIR_OUT
		|| token->type == APPEND || token->type == HEREDOC
		|| token->type == PIPE)
		return (1);
	return (0);
}

static int	token_pair_error(t_token *input)
{
	if (input->type == PIPE && input->next == NULL)
	{
		printf("syntax error near unexpected token\n");
		return (1);
	}
	if (input->type == PIPE && input->next->type == PIPE)
	{
		printf("syntax error near unexpected token\n");
		return (1);
	}
	if ((input->type == APPEND || input->type == REDIR_IN
			|| input->type == REDIR_OUT || input->type == HEREDOC)
		&& is_invalid_redir_target(input->next))
	{
		printf("syntax error near unexpected token\n");
		return (1);
	}
	return (0);
}

int	ft_parser(t_token *input)
{
	t_token	*start;

	start = input;
	if (!input)
		return (1);
	if (input->type == PIPE)
	{
		printf("syntax error near unexpected token\n");
		return (1);
	}
	while (input)
	{
		if (token_pair_error(input))
			return (1);
		input = input->next;
	}
	if (ft_last(start) == 1)
		return (1);
	return (0);
}
