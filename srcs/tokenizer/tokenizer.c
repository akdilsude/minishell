/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sakdil < sakdil@student.42istanbul.com.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/30 14:32:24 by segunes           #+#    #+#             */
/*   Updated: 2025/08/18 22:07:53 by sakdil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	tokenize_redirection(char *input, t_token **head, char **envp)
{
	int	op_len;
	int	ret;

	op_len = handle_redir_operator(input, head);
	ret = handle_redir_file(input + op_len, &op_len, head, envp);
	if (ret < 0)
		return (-1);
	return (op_len);
}

static int	handle_special_char(char *input, int *i,
				t_token **head, char **envp)
{
	t_token	*tok;
	int		consumed;

	if (input[*i] == '>' || input[*i] == '<')
	{
		consumed = tokenize_redirection(input + *i, head, envp);
		if (consumed < 0)
			return (-1);
		*i += consumed;
	}
	else if (input[*i] == '|')
	{
		tok = create_pipe_token("|");
		if (tok)
			add_token_to_list(head, tok);
		(*i)++;
	}
	else
		(*i)++;
	return (0);
}

static int	process_word(char *input, int *i, t_token **head, char **envp)
{
	char	*arg;
	char	*tmp;
	t_token	*tok;

	arg = collect_argument(input, i, envp);
	if (!arg)
		return (-1);
	tmp = ft_strdup(arg);
	if (!tmp)
		return (free(arg), -1);
	tok = create_word_token(tmp);
	if (!tok)
	{
		free(tmp);
		free(arg);
		return (-1);
	}
	add_token_to_list(head, tok);
	free(tmp);
	free(arg);
	return (0);
}

t_token	*tokenize_input(char *input, char **envp)
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
		{
			if (handle_special_char(input, &i, &head, envp) < 0)
			{
				free_token_list(head);
				return (NULL);
			}
		}
		else if (process_word(input, &i, &head, envp) < 0)
		{
			free_token_list(head);
			return (NULL);
		}
	}
	return (head);
}

int	handle_redir_operator(char *s, t_token **head)
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
	free(op);
	return (len);
}
