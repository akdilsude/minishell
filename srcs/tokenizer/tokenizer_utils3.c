/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_utils3.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: segunes <segunes@student.42istanbul.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 10:25:58 by sakdil            #+#    #+#             */
/*   Updated: 2025/08/18 18:30:05 by segunes          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_redir_separator(char c)
{
	return (c == ' ' || c == '\t' || c == '|' || c == '<' || c == '>');
}

t_token	*create_redir_token(char *value)
{
	t_token	*new_token;

	new_token = malloc(sizeof(t_token));
	if (!new_token)
		return (NULL);
	if (value[0] == '<')
		new_token->type = REDIR_IN;
	else
		new_token->type = REDIR_OUT;
	new_token->value = ft_strdup(value);
	if (!new_token->value)
	{
		free(new_token);
		return (NULL);
	}
	new_token->quoted = 0;
	new_token->next = NULL;
	return (new_token);
}

char	*expand_double_quote_content(char *input, int start,
					int end, char **envp)
{
	char	*raw;
	char	*exp;

	raw = ft_substr(input, start, end - start);
	if (!raw)
		return (NULL);
	exp = expand_variable(raw, envp);
	free(raw);
	return (exp);
}

t_token	*last_token(t_token *head)
{
	if (!head)
		return (NULL);
	while (head->next)
		head = head->next;
	return (head);
}

char	*process_unquoted_raw(char *s, int *j)
{
	int	start;

	start = *j;
	while (s[*j] && !is_redir_separator(s[*j])
		&& s[*j] != '\'' && s[*j] != '"')
		(*j)++;
	return (ft_substr(s, start, *j - start));
}
