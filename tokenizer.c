/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sakdil < sakdil@student.42istanbul.com.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/30 14:32:24 by segunes           #+#    #+#             */
/*   Updated: 2025/07/12 01:06:23 by sakdil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// void print_tokens(t_token *head)
// {
// 	t_token *tmp = head;

// 	while (tmp)
// 	{
// 		printf("TOKEN TYPE: %u\tVALUE: %s\n", tmp->type, tmp->value);
// 		tmp = tmp->next;
// 	}
// }// type yazdırmak için kontrol


int	is_word_char(char c)
{
	if (c == ' ' || c == '\t' || c == '\n'
		|| c == '|' || c == '<' || c == '>'
		|| c == '\'' || c == '"')
		return (0);
	return (1);
}

static int	tokenize_redirection(char *input, t_token **head)
{
	int	op_len;

	op_len = handle_redir_operator(input, head);
	handle_redir_file(input + op_len, &op_len, head);
	return (op_len);
}

static void	handle_special_char(char *input, int *i, t_token **head)
{
	if (input[*i] == '>' || input[*i] == '<')
		*i += tokenize_redirection(input + *i, head);
	else if (input[*i] == '|')
	{
		add_token_to_list(head, create_pipe_token(ft_strdup("|")));
		(*i)++;
	}
	else
		(*i)++;
}

t_token *tokenize_input(char *input)
{
	int		i;
	t_token	*head;

	i = 0;
	head = NULL;
	while (input[i])
	{
		if (input[i] == ' ' || input[i] == '\t')
			i++;
		else if (input[i] == '>' || input[i] == '<' || input[i] == '|')
			handle_special_char(input, &i, &head);
		else
		{
			char *arg = collect_argument(input, &i);
			if (!arg)
				return (NULL);
			add_token_to_list(&head, create_word_token(arg));
		}
	}
	return (head);
}
