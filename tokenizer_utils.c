/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sakdil < sakdil@student.42istanbul.com.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/31 17:51:21 by segunes           #+#    #+#             */
/*   Updated: 2025/07/11 11:01:47 by sakdil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_charjoin_free(char *res, char *val, int flag)
{
	char	*tmp;

	tmp = ft_strjoin(res, val);
	if (flag & 1) //res serbest
		free(res);
	if (flag & 2) //val serbest
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

void	free_tokens(t_token *head)
{
	t_token	*tmp;

	while (head)
	{
		tmp = head->next;
		free(head->value);
		free(head);
		head = tmp;
	}
}
