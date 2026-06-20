/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_utils4.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sakdil < sakdil@student.42istanbul.com.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 10:41:16 by sakdil            #+#    #+#             */
/*   Updated: 2025/08/12 10:44:04 by sakdil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*normalize_filename(char *str)
{
	if (!str || str[0] == '\0')
		return (NULL);
	return (ft_strdup(str));
}

int	prev_is_heredoc(t_token *head)
{
	t_token	*last;

	last = last_token(head);
	return (last && last->type == HEREDOC);
}
