/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sakdil < sakdil@student.42istanbul.com.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 10:51:36 by sakdil            #+#    #+#             */
/*   Updated: 2025/08/12 10:47:42 by sakdil           ###   ########.fr       */
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

static char	*collect_double_quote(char *input, int *i, char *arg, char **envp)
{
	int		start;
	char	*piece;

	start = ++(*i);
	while (input[*i] && input[*i] != '"')
		(*i)++;
	if (!input[*i])
	{
		ft_putendl_fd("syntax error: unclosed quote", 2);
		free(arg);
		return (NULL);
	}
	piece = expand_double_quote_content(input, start, *i, envp);
	if (!piece)
	{
		free(arg);
		return (NULL);
	}
	arg = ft_charjoin_free(arg, piece, 3);
	(*i)++;
	return (arg);
}

static char	*collect_argument_word(char *input, int *i, char *arg, char **envp)
{
	int		start;
	char	*raw;
	char	*exp;

	start = *i;
	while (input[*i] && is_word_char(input[*i]))
		(*i)++;
	raw = ft_substr(input, start, (*i) - start);
	if (!raw)
	{
		free(arg);
		return (NULL);
	}
	exp = expand_variable(raw, envp);
	free(raw);
	if (!exp)
	{
		free(arg);
		return (NULL);
	}
	arg = ft_charjoin_free(arg, exp, 3);
	return (arg);
}

static char	*collect_argument_quote(char *input, int *i, char *arg, char **envp)
{
	if (input[*i] == '\'')
		return (collect_single_quote(input, i, arg));
	else if (input[*i] == '"')
		return (collect_double_quote(input, i, arg, envp));
	return (arg);
}

char	*collect_argument(char *input, int *i, char **envp)
{
	char	*arg;

	arg = ft_strdup("");
	if (!arg)
		return (NULL);
	while (input[*i] && !is_redir_separator(input[*i]))
	{
		if (input[*i] == '\'' || input[*i] == '"')
		{
			arg = collect_argument_quote(input, i, arg, envp);
			if (!arg)
				return (NULL);
		}
		else
		{
			arg = collect_argument_word(input, i, arg, envp);
			if (!arg)
				return (NULL);
		}
	}
	return (arg);
}
