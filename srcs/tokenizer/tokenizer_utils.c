/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: segunes <segunes@student.42istanbul.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/31 17:51:21 by segunes           #+#    #+#             */
/*   Updated: 2025/08/12 15:00:57 by segunes          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_charjoin_free(char *res, char *val, int flag)
{
	char	*tmp;

	tmp = ft_strjoin(res, val);
	if (flag & 1)
		free(res);
	if (flag & 2)
		free(val);
	return (tmp);
}

char	*ft_charjoin(char *res, char c)
{
	char	*tmp;
	int		len;

	len = ft_strlen(res);
	tmp = malloc(len + 2);
	if (!tmp)
	{
		free(res);
		return (NULL);
	}
	ft_strlcpy(tmp, res, len + 1);
	tmp[len] = c;
	tmp[len + 1] = '\0';
	free(res);
	return (tmp);
}

int	is_word_char(char c)
{
	if (c == ' ' || c == '\t' || c == '\n' || c == '|'
		|| c == '<' || c == '>' || c == '\'' || c == '"')
		return (0);
	return (1);
}

t_token	*create_word_token(char *value)
{
	t_token	*new_token;

	new_token = malloc(sizeof(t_token));
	if (!new_token)
		return (NULL);
	new_token->type = WORD;
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

t_token	*create_pipe_token(char *value)
{
	t_token	*new_token;
	char	*dup_value;

	new_token = malloc(sizeof(t_token));
	if (!new_token)
		return (NULL);
	dup_value = ft_strdup(value);
	if (!dup_value)
	{
		free(new_token);
		return (NULL);
	}
	new_token->type = PIPE;
	new_token->value = dup_value;
	new_token->quoted = 0;
	new_token->next = NULL;
	return (new_token);
}
