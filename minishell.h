/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sakdil < sakdil@student.42istanbul.com.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 17:40:47 by sakdil            #+#    #+#             */
/*   Updated: 2025/07/12 01:12:43 by sakdil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

#include <stdio.h>
#include <unistd.h> 
#include <readline/readline.h>
#include <readline/history.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include "libft/libft.h"

extern int	g_last_status;

typedef struct s_list
{
	void			*content;
	struct s_list	*next;
}	t_list;

typedef struct s_export_data
{
    char *arg;
    char *eq;
    int status;
    int i;
} t_export_data;

typedef enum e_node_type 
{	
    NODE_COMMAND,
	NODE_PIPE,
	NODE_REDIR
}t_node_type;

typedef struct s_ast_tree
{
    t_node_type type;
    char **args;
    char *redir_target;
    int redir_type;
    struct s_ast_tree *left;//her nodun içinde alt nodelar var bu alt nodelar aynı yapıya yani structa eşit olduğu için böyle
    struct s_ast_tree *right;
    
}t_ast_tree;

typedef enum e_token_type
{
    WORD,
    PIPE, // |
    REDIR_OUT, // >
    REDIR_IN, // <
    APPEND, // >>
    HEREDOC, // <<
    INVALID
} t_token_type;

typedef struct s_token
{
	t_token_type type;
	char *value;
    struct s_token *next;
} t_token;

int	builtin_cd(int argc, char **argv);
int	builtin_echo(int argc, char **argv);
int	control_env(int argc);
int	builtin_env(char **env);
int	builtin_exit(int argc, char **argv);
int	is_valid_identifier(const char *name);
int	builtin_export(int argc, char **argv, char ***envp);
int	find_in_environ(const char *name, char **env);
int	extend_env(char ***envp, char *new_entry);
int	set_env_var(char *arg, char ***envp);
void	print_sorted_env(char **env);
void	print_invalid_identifier(char *arg);
int	builtin_pwd(void);
int	builtin_unset(int argc, char **argv, char ***env);
int	builtin(int argc, char **argv, char **env, t_list *history);
int	is_only_spaces(char *str);
void executor_structure(t_ast_tree *node, char **envp, int in_pipeline,int *exit_status);

//history//////////////////
void	add_to_history(t_list **history, char *input);
void print_history(t_list *history);
////////////////////////////

//parser///////////////////////
t_ast_tree *ft_build_ast(t_token *tokens);
int ft_last(t_token *input);
int	is_invalid_redir_target(t_token *token);
void print_ast(t_ast_tree *node, int depth);
int ft_parser(t_token *input);
////////////////////////////////

//path_find//////////////////////////
void free_string_array(char **array);
char *find_path(char *command);
const char *pathname(char *command);
//////////////////////////////////////

char	*ft_charjoin_free(char *res, char *val, int flag);
char	*ft_charjoin(char *res, char c);
void	free_tokens(t_token *head); //şu anki mainde yok
char	*expand_variable(char *str);
t_token	*create_word_token(char *value);
t_token	*create_pipe_token(char *value);
t_token	*create_redir_token(char *value);
t_token	*create_token(char *value);
int	is_word_char(char c);
t_token *tokenize_input(char *input);
char	*collect_argument(char *input, int *i);
void	add_token_to_list(t_token **head, t_token *new);
int	handle_redir_operator(char *s, t_token **head);
int	handle_redir_file(char *s, int *i, t_token **head);

#endif
