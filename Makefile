NAME			=	minishell
SRCS			=	built_cd.c				\
					built_echo.c 			\
					built_env.c				\
					built_exit.c 			\
					built_export.c			\
					built_export2.c			\
					built_export3.c			\
					built_export4.c			\
					built_pwd.c 			\
					built_unset.c			\
					builtin.c 				\
					main.c					\
					path_find.c 			\
					tokenizer.c 			\
					tokenizer2.c			\
					tokenizer3.c			\
					tokenizer_utils.c   	\
					tokenizer_utils2.c		\
					tokenizer_utils3.c		\
					tokenizer_utils4.c		\
					parser.c    			\
					executor.c 				\
					signal.c				\
					handle_redirection.c	\
 					execute_command.c		\
					execute_pipe.c			\
					parser_utils.c			\
					parser_utils2.c			\
					tokenizer4.c			\
					heredoc.c				\
					utils.c					\
					executor_utils.c		\
					free.c					\
					heredoc_utils.c			\
					execute_utils_two.c		\

OBJS			=	$(SRCS:.c=.o)
RM				=	rm -f
CFLAGS			=	-Wall -Wextra -Werror
CC 				= 	cc
LIBFT_DIR		= 	./libft
LIBFT 			= 	$(LIBFT_DIR)/libft.a


all: $(LIBFT) $(NAME)

$(NAME): $(OBJS) $(LIBFT)
	$(CC) $(OBJS) $(LIBFT) -o $(NAME) -lreadline

$(LIBFT):
		$(MAKE) -C $(LIBFT_DIR)

clean:
	$(RM) $(OBJS)
	@make -C $(LIBFT_DIR) clean

fclean: clean
	$(RM) $(NAME)
	@make -C $(LIBFT_DIR) fclean

re: fclean all

.PHONY: all clean fclean re
