/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sakdil < sakdil@student.42istanbul.com.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 17:40:51 by sakdil            #+#    #+#             */
/*   Updated: 2025/07/06 15:51:33 by sakdil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	g_last_status = 0;

void execute_command(char *input)
{
    pid_t pid;
    char **args;
    const char *path;

    args = ft_split(input, ' ');
    if (!args || !args[0])
    {
        //free_args(args);
        return;
    }
    path = pathname(args[0]);
    if (!path)
    {
        printf("%s: command not found\n", args[0]);
       // free_args(args);
        return;
    }
    pid = fork();
    if (pid < 0)
    {
        perror("fork hatasi");
        //free_args(args);
        return;
    }
    else if (pid == 0)
    {
        execve(path, args, NULL);
        perror("execve hatasi");
        exit(1);
    }
    else
        wait(NULL);
    //free_args(args);
}

int	args_count(char **args)
{
	int	i;

	i = 0;
	while (args[i])
		i++;
	return (i);
}

int	main(int argc, char **argv, char **env)
{
	char	*input;
	int exit_status = 0;
	//int		arg_count;
	t_list *history;
	t_token *tokens;
	t_ast_tree *ast;
	//int history_seen = 0;
	
	(void)argv;
	(void)argc;
	history = NULL;
	while (1)
	{
		input = readline("minishell$ ");
		if (!input)
			break;
		if (is_only_spaces(input))
		{
			free(input);
			continue;
		}
		if (*input)
			add_history(input);
		char **args = ft_split(input, ' ');
		if (!args)
		{
			free(input);
			continue;
		}
		tokens = tokenize_input(input);
		if (!tokens)
		{
			free(input);
			continue;
		}
		if (ft_parser(tokens)) // Syntax kontrolü başarısızsa
		{
			free(input);
			continue;
		}
		ast = ft_build_ast(tokens);
		if (!ast)
		{
			free(input);
			continue;
		}
		//print_ast(ast, 0);// ast yazdırmak için
		// print_tokens(tokens); type yazdırmak için
		executor_structure(ast, env, 0, &exit_status);
		// printf("minishell exit_status = %d\n", exit_status); 
		//arg_count = args_count(args);
		// int builtin_result = builtin(arg_count, args, env, history);
		// if (builtin_result == 0 || builtin_result == 1 || builtin_result == 2)
		// {
		// 	if (ft_strcmp(args[0], "history") != 0)
		// 	{
		// 		if (history_seen == 1)
		// 			add_to_history(&history, input);
		// 		history_seen = 1;
		// 	}
		// 	else
		// 		add_to_history(&history, input);
		// 	//free_args
		// 	continue;
		// }
		return(exit_status);
		add_to_history(&history, input);
		//execute_command(input);
		free(input);
	}
	//free_history
}


// int	main(void)
// {
// 	char	*line;
// 	t_token *tokens;

// 	while (1)
// 	{
// 		line = readline("minishell$ ");
// 		if (!line)
// 			break;
// 		if (*line)
// 			add_history(line);
// 		tokens = tokenize_input(line);
// 		print_tokens(tokens);
// 		free_tokens(tokens);
// 		free(line);
// 	}
// 	return (0);
// }
