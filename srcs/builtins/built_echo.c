/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_echo.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sakdil < sakdil@student.42istanbul.com.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/10 14:20:45 by segunes           #+#    #+#             */
/*   Updated: 2025/08/20 00:32:39 by sakdil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	parse_n_flag(int argc, char **argv, int *idx)
{
	int	nflag;
	int	j;

	nflag = 0;
	while (*idx < argc && argv[*idx][0] == '-' && argv[*idx][1] == 'n')
	{
		j = 1;
		while (argv[*idx][j] == 'n')
			j++;
		if (argv[*idx][j] != '\0')
			break ;
		nflag = 1;
		(*idx)++;
	}
	return (nflag);
}

static int	echo_write_arg(const char *arg, int first)
{
	size_t	len;

	if (!first)
	{
		if (write(1, " ", 1) < 0)
		{
			if (errno == EPIPE)
				return (1);
			perror("echo");
			return (1);
		}
	}
	len = ft_strlen(arg);
	if (write(1, arg, len) < 0)
	{
		if (errno == EPIPE)
			return (1);
		perror("echo");
		return (1);
	}
	return (0);
}

int	builtin_echo(int argc, char **argv)
{
	int	i;
	int	first;
	int	nflag;

	i = 1;
	first = 1;
	nflag = parse_n_flag(argc, argv, &i);
	while (i < argc)
	{
		if (echo_write_arg(argv[i], first))
			return (1);
		first = 0;
		i++;
	}
	if (!nflag && write(1, "\n", 1) < 0)
	{
		if (errno == EPIPE)
			return (1);
		perror("echo");
		return (1);
	}
	return (0);
}
