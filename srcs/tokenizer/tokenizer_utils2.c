/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_utils2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sakdil < sakdil@student.42istanbul.com.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 10:58:23 by sakdil            #+#    #+#             */
/*   Updated: 2025/08/12 10:27:18 by sakdil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	var_name_len(const char *s, int i)
{
	int	j;

	if (!s[i + 1])
		return (1);
	if (s[i + 1] == '?')
		return (2);
	if (is_digit(s[i + 1]))
		return (2);
	if (ft_isalpha(s[i + 1]) || s[i + 1] == '_')
	{
		j = 2;
		while (s[i + j] && (ft_isalnum(s[i + j]) || s[i + j] == '_'))
			j++;
		return (j);
	}
	return (1);
}

static char	*handle_var_utils(char *res, char *str, int *i, char **envp)
{
	int			j;
	char		*var;
	const char	*val;
	char		*rem;

	j = var_name_len(str, *i);
	var = ft_substr(str, *i + 1, j - 1);
	if (!var)
		return (NULL);
	val = ms_getenv(var, envp);
	if (val)
		res = ft_charjoin_free(res, ft_strdup(val), 3);
	else
	{
		rem = ft_substr(str, *i + j, ft_strlen(str) - (*i + j));
		res = ft_charjoin_free(res, rem, 3);
		*i = ft_strlen(str);
		free(var);
		return (res);
	}
	free(var);
	*i += j;
	return (res);
}

static char	*handle_var(char *str, int *i, char *res, char **envp)
{
	char	*code;
	int		j;

	if (str[*i + 1] == '?')
	{
		code = ft_itoa(ft_exit_code(-1));
		if (!code)
			return (NULL);
		res = ft_charjoin_free(res, code, 3);
		*i += 2;
		return (res);
	}
	j = var_name_len(str, *i);
	if (j == 1)
	{
		res = ft_charjoin(res, '$');
		(*i)++;
		return (res);
	}
	return (handle_var_utils(res, str, i, envp));
}

static char	*expand_variable_loop(char *str, char *res, char **envp)
{
	int	idx;

	idx = 0;
	while (str[idx])
	{
		if (str[idx] == '$')
			res = handle_var(str, &idx, res, envp);
		else
			res = ft_charjoin(res, str[idx++]);
		if (!res)
			return (NULL);
	}
	return (res);
}

char	*expand_variable(char *str, char **envp)
{
	char	*res;

	res = ft_strdup("");
	if (!res)
		return (NULL);
	return (expand_variable_loop(str, res, envp));
}
