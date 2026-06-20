/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer3.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: segunes <segunes@student.42istanbul.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 10:55:20 by sakdil            #+#    #+#             */
/*   Updated: 2025/08/18 18:31:57 by segunes          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token	*create_token(char *value)
{
	t_token	*new_token;

	new_token = malloc(sizeof(t_token));
	if (!new_token)
		return (NULL);
	if (ft_strncmp(value, "<<", 2) == 0)
		new_token->type = HEREDOC;
	else
		new_token->type = APPEND;
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

char	*process_quoted(char *s, int *i, char quote_char)
{
	int	start;
	int	len;

	start = *i;
	len = 0;
	while (s[*i] && s[*i] != quote_char)
	{
		(*i)++;
		len++;
	}
	if (s[*i] == quote_char)
		(*i)++;
	return (ft_substr(s, start, len));
}

static int	is_unquoted_word_char(char c)
{
	return (c != '\0' && c != ' ' && c != '\t' && c != '|'
		&& c != '<' && c != '>' && c != '\'' && c != '"');
}

char	*process_unquoted(char *s, int *j, char **envp)
{
	int		start;
	char	*raw;
	char	*expanded;

	start = *j;
	while (s[*j] && is_unquoted_word_char(s[*j]))
		(*j)++;
	raw = ft_substr(s, start, *j - start);
	if (!raw)
		return (NULL);
	expanded = expand_variable(raw, envp);
	free(raw);
	if (!expanded)
		return (NULL);
	return (expanded);
}
