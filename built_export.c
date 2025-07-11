/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_export.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sakdil < sakdil@student.42istanbul.com.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 12:27:34 by sakdil            #+#    #+#             */
/*   Updated: 2025/07/11 10:41:24 by sakdil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_valid_identifier(const char *name)
{
	int	i;

	if (!name || !name[0])
		return (0);
	if (name[0] >= '0' && name[0] <= '9')
		return (0);
	i = 0;
	while (name[i])
	{
		if (!(ft_isalnum(name[i]) || name[i] == '_'))
			return (0);
		i++;
	}
	return (1);
}

int	is_valid_identifier_len(const char *name, size_t len)
{
	size_t	i;

	if (len == 0 || (name[0] >= '0' && name[0] <= '9'))
		return (0);
	i = 0;
	while (i < len)
	{
		if (!(ft_isalnum(name[i]) || name[i] == '_'))
			return (0);
		i++;
	}
	return (1);
}

int	find_in_environ(const char *name, char **env)
{
	int	i;
	int	len;

	len = ft_strlen(name);
	i = 0;
	while (env[i])
	{
		if (ft_strncmp(env[i], name, len) == 0 && env[i][len] == '=')
			return (i);
		i++;
	}
	return (-1);
}

static int	extend_env(char ***envp, char *new_entry)
{
	char	**old;
	char	**new;
	int		cnt;
	int		j;

	old = *envp;
	cnt = 0;
	while (old[cnt])
		cnt++;
	new = malloc(sizeof(char *) * (cnt + 2));
	if (!new)
		return (1);
	j = 0;
	while (j < cnt)
	{
		new[j] = old[j];
		j++;
	}
	new[j++] = new_entry;
	new[j] = NULL;
	*envp = new;
	return (0);
}

static int	set_env_var_update(char *name, char *value, char **envp, int idx)
{
	char	*tmp;
	char	*new_var;

	free(envp[idx]);
	tmp = ft_strjoin(name, "=");
	if (!tmp)
		return (1);
	new_var = ft_strjoin(tmp, value);
	free(tmp);
	if (!new_var)
		return (1);
	envp[idx] = new_var;
	return (0);
}

static int	set_env_var_add(char *name, char *value, char ***envp)
{
	char	*tmp;
	char	*new_var;

	tmp = ft_strjoin(name, "=");
	if (!tmp)
		return (1);
	new_var = ft_strjoin(tmp, value);
	free(tmp);
	if (!new_var)
		return (1);
	if (extend_env(envp, new_var))
		return (free(new_var), 1);
	return (0);
}

static int	set_env_var(char *arg, char ***envp)
{
	char	*eq;
	char	*name;
	char	*value;
	int		idx;
	int		ret;

	eq = ft_strchr(arg, '=');
	if (!eq)
		return (1);
	name = ft_substr(arg, 0, eq - arg);
	value = ft_strdup(eq + 1);
	if (!name || !value)
	{
		free(name);
		free(value);
		return (1);
	}
	idx = find_in_environ(name, *envp);
	if (idx >= 0)
		ret = set_env_var_update(name, value, *envp, idx);
	else
		ret = set_env_var_add(name, value, envp);
	free(name);
	free(value);
	return (ret);
}

static char	**copy_env(char **env)
{
	int		n = 0;
	int		i;
	char	**copy;

	while (env[n])
		n++;
	copy = malloc(sizeof(char *) * (n + 1));
	if (!copy)
		return (NULL);
	i = 0;
	while (i < n)
	{
		copy[i] = env[i];
		i++;
	}
	copy[n] = NULL;
	return (copy);
}

static int	compare_keys(char *s1, char *s2)
{
	char	*k1;
	char	*k2;
	int		res;

	k1 = ft_substr(s1, 0, ft_strchr(s1, '=') - s1);
	k2 = ft_substr(s2, 0, ft_strchr(s2, '=') - s2);
	if (!k1 || !k2)
	{
		free(k1);
		free(k2);
		return (0);
	}
	res = ft_strcmp(k1, k2);
	free(k1);
	free(k2);
	return (res);
}

static void	sort_env(char **copy)
{
	int		n;
	int		i;
	int		j;
	char	*tmp;

	n = 0;
	while (copy[n])
		n++;
	i = 0;
	while (i < n - 1)
	{
		j = 0;
		while (j < n - i - 1)
		{
			if (compare_keys(copy[j], copy[j + 1]) > 0)
			{
				tmp = copy[j];
				copy[j] = copy[j + 1];
				copy[j + 1] = tmp;
			}
			j++;
		}
		i++;
	}
}

void	print_sorted_env(char **env)
{
	char	**copy;
	int		i;
	char	*tmp;

	copy = copy_env(env);
	if (!copy)
		return ;
	sort_env(copy);
	i = 0;
	while (copy[i])
	{
		tmp = ft_strchr(copy[i], '=');
		printf("declare -x %.*s=\"%s\"\n",
			(int)(tmp - copy[i]), copy[i], tmp + 1);
		i++;
	}
	free(copy);
}

static void	print_invalid_identifier(char *arg)
{
	char	*msg;
	char	*temp;

	msg = ft_strjoin("export: '", arg);
	if (!msg)
		return ;
	temp = ft_strjoin(msg, "': not a valid identifier\n");
	free(msg);
	if (!temp)
		return ;
	ft_putstr_fd(temp, 2);
	free(temp);
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

static int	process_export_name(char *arg, char ***envp)
{
	int		status;
	char	*entry;

	status = 0;
	if (!is_valid_identifier(arg))
	{
		print_invalid_identifier(arg);
		status = 1;
	}
	else if (find_in_environ(arg, *envp) < 0)
	{
		entry = ft_strjoin(arg, "=");
		if (!entry || extend_env(envp, entry))
			status = 1;
		free(entry);
	}
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
		if (ft_strchr(argv[i], '='))
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
