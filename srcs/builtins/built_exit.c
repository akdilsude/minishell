/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_exit.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: segunes <segunes@student.42istanbul.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/18 20:06:34 by sakdil            #+#    #+#             */
/*   Updated: 2025/08/20 11:57:12 by segunes          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_digit(int c)
{
	return (c >= '0' && c <= '9');
}

static int	is_numeric(const char *str)
{
	int	i;
	int	digits;

	i = 0;
	digits = 0;
	if (!str || !*str)
		return (0);
	if (str[0] == '-' || str[0] == '+')
		i++;
	while (str[i])
	{
		if (!is_digit(str[i]))
			return (0);
		digits++;
		i++;
	}
	return (digits > 0);
}

static int	is_too_large(char *str)
{
	int	len;
	int	neg;

	neg = 0;
	if (!str || !*str)
		return (1);
	if (*str == '+' || *str == '-')
	{
		neg = (*str == '-');
		str++;
	}
	while (*str == '0')
		str++;
	len = ft_strlen(str);
	if (len > 19)
		return (1);
	if (len < 19)
		return (0);
	if (neg)
		return (ft_strcmp(str, "9223372036854775808") > 0);
	else
		return (ft_strcmp(str, "9223372036854775807") > 0);
	return (0);
}

static int	check_exit_args(int argc, char **argv)
{
	if (argc >= 2 && (!argv[1] || !is_numeric(argv[1])
			|| is_too_large(argv[1])))
	{
		ft_putstr_fd("exit: ", 2);
		ft_putstr_fd(argv[1], 2);
		ft_putendl_fd(": numeric argument required", 2);
		return (2);
	}
	if (argc > 2)
	{
		ft_putendl_fd("exit: too many arguments", 2);
		return (1);
	}
	return (0);
}

int	builtin_exit(int argc, char **argv, t_shell *shell)
{
	int	check;
	int	exit_code;

	ft_putendl_fd("exit", 1);
	check = check_exit_args(argc, argv);
	if (check == 1)
	{
		ft_exit_code(1);
		return (1);
	}
	if (check == 2)
	{
		ft_exit_code(check);
		cleanup(shell, 0);
		exit (check);
	}
	if (argc == 1)
		exit_code = ft_exit_code(-1);
	else
		exit_code = (unsigned char)ft_atoi(argv[1]);
	ft_exit_code(exit_code);
	cleanup(shell, 0);
	shell = NULL;
	exit(exit_code);
}
