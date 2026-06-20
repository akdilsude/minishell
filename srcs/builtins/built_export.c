/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_export.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: segunes <segunes@student.42istanbul.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 12:27:34 by sakdil            #+#    #+#             */
/*   Updated: 2025/08/12 14:52:43 by segunes          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_valid_identifier_len(const char *name, size_t len)
{
	size_t	i;

	i = 0;
	if (len == 0 || (name[0] >= '0' && name[0] <= '9'))
		return (0);
	while (i < len)
	{
		if (!(ft_isalnum(name[i]) || name[i] == '_'))
			return (0);
		i++;
	}
	return (1);
}

static int	process_export_assignment(char *arg, char ***envp)
{
	char	*name;
	int		len;
	int		status;

	status = 0;
	len = ft_strchr(arg, '=') - arg;
	if (!is_valid_identifier_len(arg, len))
	{
		name = ft_substr(arg, 0, len);
		if (name)
		{
			print_invalid_identifier(name);
			free(name);
		}
		status = 1;
	}
	else if (set_env_var(arg, envp))
		status = 1;
	return (status);
}

static int	export_add_if_missing(char *arg, char ***envp)
{
	char	*entry;

	if (find_in_environ(arg, *envp) >= 0)
		return (0);
	entry = ft_strdup(arg);
	if (!entry || extend_env(envp, entry))
	{
		if (entry)
			free(entry);
		return (1);
	}
	return (0);
}

static int	process_export_name(char *arg, char ***envp)
{
	int	status;

	status = 0;
	if (!arg || arg[0] == '\0')
	{
		print_invalid_identifier(arg);
		return (1);
	}
	if (arg[0] == '=' || ft_strchr(arg, '=') == arg)
	{
		print_invalid_identifier(arg);
		return (1);
	}
	if (!is_valid_identifier(arg))
	{
		print_invalid_identifier(arg);
		status = 1;
	}
	else if (export_add_if_missing(arg, envp))
		status = 1;
	return (status);
}

int	builtin_export(int argc, char **argv, char ***envp)
{
	int	i;
	int	status;

	status = 0;
	i = 1;
	if (argc == 1)
	{
		print_sorted_env(*envp);
		return (0);
	}
	while (i < argc)
	{
		if (argv[i] && ft_strchr(argv[i], '='))
		{
			if (process_export_assignment(argv[i], envp))
				status = 1;
		}
		else
		{
			if (process_export_name(argv[i], envp))
				status = 1;
		}
		i++;
	}
	return (status);
}
