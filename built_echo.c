/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_echo.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sakdil < sakdil@student.42istanbul.com.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/10 14:20:45 by segunes           #+#    #+#             */
/*   Updated: 2025/07/06 16:02:14 by sakdil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void print_env_var(char *var_name, char **env)
{
	int i;
	char *equal;

	i = 0;
	if (!env) 
		return; 
	while (env[i]) 
	{
		equal = ft_strchr(env[i], '=');
		if (equal && ft_strncmp(env[i], var_name, equal - env[i]) == 0
			&& ft_strlen(var_name) == (size_t)(equal - env[i]))
		{
			printf("%s", equal + 1); // '=' sonrası değer
			return;
		}
		i++;
	}
}

void print_arg_cleaned(char *arg, char **env)
{
    int j;
    int in_single_quote;
    int in_double_quote;
    int start;

    j = 0;
    in_single_quote = 0;
    in_double_quote = 0;
    while (arg[j])
    {
        if (arg[j] == '\'' && !in_double_quote)
        {
            in_single_quote = !in_single_quote;
            j++;
            continue;
        }
        if (arg[j] == '"' && !in_single_quote)
        {
            in_double_quote = !in_double_quote;
            j++;
            continue;
        }
        if (in_single_quote)
        {
            printf("%c", arg[j]); // Tek tırnak içindeyken karakteri olduğu gibi yazdır
            j++;
            continue;
        }
        if (arg[j] == '$' && !in_single_quote)
        {
            j++; // '$' sonrası karaktere geç
            start = j;
            while (arg[j] && (isalnum(arg[j]) || arg[j] == '_'))
                j++;
            if (j > start) 
            {
                char var_name[256] = {0};
                ft_strncpy(var_name, &arg[start], j - start);
                print_env_var(var_name, env);
                continue; // $ sonrası yazdırıldı, devam et
            }
            // $ yazılı ama geçerli değişken adı yoksa, direk '$' bas
            printf("$");
            continue;
        }
        printf("%c", arg[j]);
        j++;
    }
}


int builtin_echo(int argc, char **argv, char **env)
{
    int i = 1;
    int newline = 1;
    (void)env;

    if (i < argc && ft_strcmp(argv[i], "-n") == 0)
    {
        newline = 0;
        i++;
    }
    while (i < argc)
    {
        printf("%s", argv[i]);
        if (i < argc - 1)
            printf(" ");
        i++;
    }
    if (newline)
        printf("\n");
    return 0;
}
