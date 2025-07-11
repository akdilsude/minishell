/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sakdil < sakdil@student.42istanbul.com.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 10:51:36 by sakdil            #+#    #+#             */
/*   Updated: 2025/07/11 10:59:54 by sakdil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*collect_single_quote(char *input, int *i, char *arg)
{
	int		start;
	char	*piece;

	start = ++(*i);
	while (input[*i] && input[*i] != '\'')
		(*i)++;
	if (!input[*i])
		return (printf("syntax error: unclosed quote\n"), free(arg), NULL);
	piece = ft_substr(input, start, (*i) - start);
	if (!piece)
		return (free(arg), NULL);
	arg = ft_charjoin_free(arg, piece, 3);
	(*i)++;
	return (arg);
}

static char	*collect_double_quote(char *input, int *i, char *arg)
{
	int		start;
	char	*raw;
	char	*exp;

	start = ++(*i);
	while (input[*i] && input[*i] != '"')
		(*i)++;
	if (!input[*i])
		return (printf("syntax error: unclosed quote\n"), free(arg), NULL);
	raw = ft_substr(input, start, (*i) - start);
	if (!raw)
		return (free(arg), NULL);
	exp = expand_variable(raw);
	free(raw);
	if (!exp)
		return (free(arg), NULL);
	arg = ft_charjoin_free(arg, exp, 3);
	(*i)++;
	return (arg);
}

static char	*collect_argument_word(char *input, int *i, char *arg)
{
	int		start;
	char	*raw;
	char	*exp;

	start = *i;
	while (input[*i] && is_word_char(input[*i]))
		(*i)++;
	raw = ft_substr(input, start, (*i) - start);
	if (!raw)
		return (free(arg), NULL);
	exp = expand_variable(raw);
	free(raw);
	if (!exp)
		return (free(arg), NULL);
	arg = ft_charjoin_free(arg, exp, 3);
	return (arg);
}

static char	*collect_argument_quote(char *input, int *i, char *arg)
{
	if (input[*i] == '\'')
		return (collect_single_quote(input, i, arg));
	else if (input[*i] == '"')
		return (collect_double_quote(input, i, arg));
	return (arg);
}

char	*collect_argument(char *input, int *i)
{
	char	*arg;

	arg = ft_strdup("");
	if (!arg)
		return (NULL);
	while (input[*i] && !(input[*i] == ' ' || input[*i] == '\t'
			|| input[*i] == '|' || input[*i] == '<' || input[*i] == '>'))
	{
		if (input[*i] == '\'' || input[*i] == '"')
		{
			arg = collect_argument_quote(input, i, arg);
			if (!arg)
				return (NULL);
		}
		else
		{
			arg = collect_argument_word(input, i, arg);
			if (!arg)
				return (NULL);
		}
	}
	return (arg);
}
