/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_export.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sakdil < sakdil@student.42istanbul.com.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 12:27:34 by sakdil            #+#    #+#             */
/*   Updated: 2025/07/06 16:15:12 by sakdil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_valid_identifier(const char *name)
{
	int i;

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
	size_t i;

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
	int i;
	int len;

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

static int	set_env_var(char *arg, char ***envp)
{
	char	*eq;
	char	*name;
	char	*value;
	char	*tmp;
	int		idx;

	eq = ft_strchr(arg, '=');
	name = ft_substr(arg, 0, eq - arg);
	value = ft_strdup(eq + 1);
	if (!name || !value)
		return (free(name), free(value), 1);
	idx = find_in_environ(name, *envp);
	if (idx >= 0)
	{
		free((*envp)[idx]);
		tmp = ft_strjoin(name, "=");
		(*envp)[idx] = ft_strjoin(tmp, value);
		free(tmp);
	}
	else
	{
		tmp = ft_strjoin(name, "=");
		tmp = ft_strjoin(tmp, value);
		if (extend_env(envp, tmp))
			return (free(name), free(value), 1);
	}
	free(name);
	free(value);
	return (0);
}

static void	print_sorted_env(char **env)
{
	int		n;
	char	**copy;
	int		i, j;
	char	*k1, *k2, *tmp;

	n = 0;
	while (env[n])
		n++;
	copy = malloc(sizeof(char *) * (n + 1));
	if (!copy)
		return;
	i = 0;
	while (i < n)
	{
		copy[i] = env[i];
		i++;
	}
	copy[n] = NULL;
	i = 0;
	while (i < n - 1)
	{
		j = 0;
		while (j < n - i - 1)
		{
			k1 = ft_substr(copy[j], 0, ft_strchr(copy[j], '=') - copy[j]);
			k2 = ft_substr(copy[j + 1], 0, ft_strchr(copy[j + 1], '=') - copy[j + 1]);
			if (ft_strcmp(k1, k2) > 0)
			{
				tmp = copy[j];
				copy[j] = copy[j + 1];
				copy[j + 1] = tmp;
			}
			free(k1);
			free(k2);
			j++;
		}
		i++;
	}
	i = 0;
	while (i < n)
	{
		tmp = ft_strchr(copy[i], '=');
		printf("declare -x %.*s=\"%s\"\n",
			(int)(tmp - copy[i]), copy[i], tmp + 1);
		i++;
	}
	free(copy);
}

int builtin_export(int argc, char **argv, char ***envp)
{
    int i;
    int status;
    char *msg;
    char *temp;

    i = 1;
    status = 0;
    if (argc == 1)
    {
        print_sorted_env(*envp);
        return (0);
    }
    while (i < argc)
    {
        if (ft_strchr(argv[i], '='))
        {
            if (!is_valid_identifier_len(argv[i], ft_strchr(argv[i], '=') - argv[i]))
            {
                temp = ft_substr(argv[i], 0, ft_strchr(argv[i], '=') - argv[i]);
                msg = ft_strjoin("export: '", temp);
                free(temp);
                temp = ft_strjoin(msg, "': not a valid identifier\n");
                free(msg);
                ft_putstr_fd(temp, 2);
                free(temp);
                status = 1;
            }
            else if (set_env_var(argv[i], envp))
                status = 1;
        }
        else
        {
            if (!is_valid_identifier(argv[i]))
            {
                msg = ft_strjoin("export: '", argv[i]);
                temp = ft_strjoin(msg, "': not a valid identifier\n");
                free(msg);
                ft_putstr_fd(temp, 2);
                free(temp);
                status = 1;
            }
            else if (find_in_environ(argv[i], *envp) < 0)
            {
                char *entry = ft_strjoin(argv[i], "=");
                if (!entry || extend_env(envp, entry))
                    status = 1;
                free(entry);
            }
        }
        i++;
    }
    return (status);
}