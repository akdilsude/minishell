NAME		=	minishell
SRCS		=	$(SRCS_DIR)/main.c																			\
				$(BUILT_DIR)/built_cd.c $(BUILT_DIR)/built_echo.c $(BUILT_DIR)/built_env.c					\
				$(BUILT_DIR)/built_exit.c $(BUILT_DIR)/built_export.c $(BUILT_DIR)/built_export2.c			\
				$(BUILT_DIR)/built_export3.c $(BUILT_DIR)/built_export4.c $(BUILT_DIR)/built_pwd.c			\
				$(BUILT_DIR)/built_unset.c $(BUILT_DIR)/builtin.c											\
				$(EXEC_DIR)/execute_command.c $(EXEC_DIR)/execute_pipe.c $(EXEC_DIR)/execute_utils_two.c	\
				$(EXEC_DIR)/executor.c $(EXEC_DIR)/executor_utils.c $(EXEC_DIR)/path_find.c					\
				$(PARSE_DIR)/parser.c $(PARSE_DIR)/parser_utils.c $(PARSE_DIR)/parser_utils2.c				\
				$(PARSE_DIR)/handle_redirection.c $(PARSE_DIR)/heredoc.c $(PARSE_DIR)/heredoc_utils.c		\
				$(TOK_DIR)/tokenizer.c $(TOK_DIR)/tokenizer2.c $(TOK_DIR)/tokenizer3.c						\
				$(TOK_DIR)/tokenizer4.c $(TOK_DIR)/tokenizer_utils.c $(TOK_DIR)/tokenizer_utils2.c			\
				$(TOK_DIR)/tokenizer_utils3.c $(TOK_DIR)/tokenizer_utils4.c									\
				$(UTIL_DIR)/free.c $(UTIL_DIR)/utils.c $(UTIL_DIR)/signal.c

OBJS		=	$(SRCS:.c=.o)
RM			=	rm -f
CFLAGS		=	-Wall -Wextra -Werror
CC			=	cc
INCLUDES	=	-I./inc -I./libft
SRCS_DIR	=	srcs
BUILT_DIR	=	$(SRCS_DIR)/builtins
EXEC_DIR	=	$(SRCS_DIR)/executor
PARSE_DIR	=	$(SRCS_DIR)/parser
TOK_DIR		=	$(SRCS_DIR)/tokenizer
UTIL_DIR	=	$(SRCS_DIR)/utils
LIBFT_DIR	=	./libft
LIBFT		=	$(LIBFT_DIR)/libft.a


all: $(LIBFT) $(NAME)

$(NAME): $(OBJS) $(LIBFT)
	$(CC) $(CFLAGS) $(OBJS) $(LIBFT) -o $(NAME) -lreadline

$(LIBFT):
		$(MAKE) -C $(LIBFT_DIR)

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	$(RM) $(OBJS)
	@make -C $(LIBFT_DIR) clean

fclean: clean
	$(RM) $(NAME)
	@make -C $(LIBFT_DIR) fclean

re: fclean all

.PHONY: all clean fclean re
