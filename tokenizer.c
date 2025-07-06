/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sakdil < sakdil@student.42istanbul.com.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/30 14:32:24 by segunes           #+#    #+#             */
/*   Updated: 2025/07/06 16:12:20 by sakdil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


void print_tokens(t_token *head)
{
	t_token *tmp = head;

	while (tmp)
	{
		printf("TOKEN TYPE: %u\tVALUE: %s\n", tmp->type, tmp->value);
		tmp = tmp->next;
	}
}// type yazdırmak için kontrol

void	add_token_to_list(t_token **head, t_token *new)
{
	t_token	*tmp;

	if (!*head)
		*head = new;
	else
	{
		tmp = *head;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = new;
	}
}

static int	is_word_char(char c)
{
	if (c == ' ' || c == '\t' || c == '\n'
		|| c == '|' || c == '<' || c == '>'
		|| c == '\'' || c == '"')
		return (0);
	return (1);
}

static char *collect_argument(char *input, int *i)
{
	char *arg = ft_strdup("");
	if (!arg)
		return (NULL);

	while (input[*i] &&
	       !(input[*i] == ' ' || input[*i] == '\t' ||
	         input[*i] == '|' || input[*i] == '<' || input[*i] == '>'))
	{
		if (input[*i] == '\'')
		{
			int start = ++(*i);
			while (input[*i] && input[*i] != '\'')
				(*i)++;
			if (!input[*i])
			{
				printf("syntax error: unclosed quote\n");
				free(arg);
				return (NULL);
			}
			char *piece = ft_substr(input, start, (*i) - start);
			if (!piece)
				return (free(arg), NULL);
			arg = ft_strjoin_free(arg, piece, 3);
			(*i)++;
		}
		else if (input[*i] == '"')
		{
			int start = ++(*i);
			while (input[*i] && input[*i] != '"')
				(*i)++;
			if (!input[*i])
			{
				printf("syntax error: unclosed quote\n");
				free(arg);
				return (NULL);
			}
			char *raw = ft_substr(input, start, (*i) - start);
			if (!raw)
				return (free(arg), NULL);
			char *exp = expand_variable(raw);
			if (!exp)
				return (free(raw), free(arg), NULL);
			arg = ft_strjoin_free(arg, exp, 3);
			free(raw);
			(*i)++;
		}
		else
		{
			int start = *i;
			while (input[*i] && is_word_char(input[*i]))
				(*i)++;
			char *raw = ft_substr(input, start, (*i) - start);
			if (!raw)
				return (free(arg), NULL);
			char *exp = expand_variable(raw);
			if (!exp)
				return (free(raw), free(arg), NULL);
			arg = ft_strjoin_free(arg, exp, 3);
			free(raw);
		}
	}
	return arg;
}


static int	handle_redir_operator(char *s, t_token **head)
{
	char	*op;
	int		len;

	if (s[0] == '>' && s[1] == '>')
	{
		op = ft_substr(s, 0, 2);
		len = 2;
	}
	else if (s[0] == '<' && s[1] == '<')
	{
		op = ft_substr(s, 0, 2);
		len = 2;
	}
	else
	{
		op = ft_substr(s, 0, 1);
		len = 1;
	}
	if (len == 2)
		add_token_to_list(head, create_token(op));
	else
		add_token_to_list(head, create_redir_token(op));
	return (len);
}

static char	*process_quoted(char *s, int *j, char q)
{
	int		start;
	char	*raw;
	char	*expanded;

	start = *j;
	while (s[*j] && s[*j] != q)
		(*j)++;
	raw = ft_substr(s, start, *j - start);
	if (q == '"')
		expanded = expand_variable(raw);
	else
		expanded = ft_strdup(raw);
	free(raw);
	if (s[*j] == q)
		(*j)++;
	return (expanded);
}

static char	*process_unquoted(char *s, int *j)
{
	int		start;
	char	*raw;
	char	*expanded;

	start = *j;
	while (s[*j] && is_word_char(s[*j]))
		(*j)++;
	raw = ft_substr(s, start, *j - start);
	expanded = expand_variable(raw);
	free(raw);
	return (expanded);
}

static int	handle_redir_file(char *s, int *i, t_token **head)
{
	int		j;
	char	*expanded;

	j = 0;
	while (s[j] == ' ' || s[j] == '\t')
		j++;
	if (!s[j])
	{
		*i += j;
		return (j);
	}
	if (s[j] == '\'' || s[j] == '"')
	{
		j++;
		expanded = process_quoted(s, &j, s[j - 1]);
	}
	else
		expanded = process_unquoted(s, &j);
	add_token_to_list(head, create_word_token(expanded));
	*i += j;
	return (j);
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
	int      i = 0;
	t_token *head = NULL;

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
	return head;
}
