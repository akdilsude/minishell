/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sakdil < sakdil@student.42istanbul.com.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/17 14:55:04 by segunes           #+#    #+#             */
/*   Updated: 2025/07/06 16:43:52 by sakdil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void executor_structure(t_ast_tree *node, char **envp, int in_pipeline, int *exit_status)
{
    int   status;
    int   second_status;
    pid_t pid;
    pid_t pid1;
    pid_t pid2;
    int   pipefd[2];

    if (node->type == NODE_COMMAND)
    {
        if (!in_pipeline)
        {
            int bres = builtin(args_count(node->args), node->args, envp, NULL);
            if (bres >= 0)
            {
                *exit_status = bres;
                return;
            }
        }

        pid = fork();
        if (pid < 0)
        {
            perror("fork");
            return;
        }
        else if (pid == 0)
        {
            if (in_pipeline)
            {
                int bres = builtin(args_count(node->args), node->args, envp, NULL);
                if (bres >= 0)
                    exit(bres);
            }
            char *cmd = find_path(node->args[0]);
            if (cmd)
            {
                execve(cmd, node->args, envp);
                perror("execve");
                exit(1);
            }
            else
            {
                printf("%s: command not found\n", node->args[0]);
                exit(127);
            }
        }
        else
        {
            waitpid(pid, &status, 0);
            *exit_status = WEXITSTATUS(status);
        }
    }
    else if (node->type == NODE_PIPE)
    {
        int child_exit_status = 0;

        if (pipe(pipefd) == -1)
        {
            perror("pipe");
            return;
        }

        pid1 = fork();
        if (pid1 == 0)
        {
            dup2(pipefd[1], STDOUT_FILENO);
            close(pipefd[0]);
            close(pipefd[1]);
            executor_structure(node->left, envp, 1, &child_exit_status);
            exit(child_exit_status);
        }

        pid2 = fork();
        if (pid2 == 0)
        {
            dup2(pipefd[0], STDIN_FILENO);
            close(pipefd[1]);
            close(pipefd[0]);
            executor_structure(node->right, envp, 1, &child_exit_status);
            exit(child_exit_status);
        }

        /* parent */
        close(pipefd[0]);
        close(pipefd[1]);
        waitpid(pid1, &status, 0);
        waitpid(pid2, &second_status, 0);
        *exit_status = WEXITSTATUS(second_status);
    }
    else if (node->type == NODE_REDIR)
    {
        /* redirection handling (stub — implement as needed) */
        if (node->redir_type == REDIR_IN || node->redir_type == REDIR_OUT)
        {
            /* input/output redirection */
        }
        else if (node->redir_type == APPEND || node->redir_type == HEREDOC)
        {
            /* append/heredoc */
        }
        /* after setting up redirection, execute the command subtree */
        executor_structure(node->left, envp, in_pipeline, exit_status);
    }
}



/*
| pid == 0 | Bu kod bloğu child process içindir |
| pid > 0 | Bu kod bloğu parent process içindir |
| pid < 0 | Hata: fork başarısız oldu |

********************************************************************

dup() sistem çağrısı bir dosya tanımlayıcısının kopyasını oluşturur.

!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
															 !!
			  [ PARENT PROCESS ]                             !!
					 |                                       !!
				pipe(pipefd)                                 !!
					 |                                       !!
			  ┌──────┴──────┐                                !!
			  |             |                                !!
		fork() →         fork()                              !!
		 |                 |                                 !!
	 [ CHILD 1 ]       [ CHILD 2 ]                           !!
	  echo serap        wc -c                                !!
	 stdout → pipe      stdin ← pipe                         !!
		 |                  ^                                !!
		 └──── pipefd[1]    └──── pipefd[0]                  !!
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

*/