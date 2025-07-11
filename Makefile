NAME			=	minishell
SRCS			=	built_cd.c			\
					built_echo.c 		\
					built_env.c			\
					built_exit.c 		\
					built_export.c		\
					built_pwd.c 		\
					built_unset.c		\
					builtin.c 			\
					check.c				\
					history.c 			\
					main.c				\
					path_find.c 		\
					tokenizer.c 		\
					tokenizer2.c		\
					tokenizer3.c		\
					tokenizer_utils.c   \
					tokenizer_utils2.c	\
					tokenizer_utils3.c	\
					parser.c    		\
					executor.c 			\


OBJS			=	$(SRCS:.c=.o)
RM				=	rm -f
CFLAGS			=	-Wall -Wextra -Werror
CC 				= 	cc
LIBFT_DIR		= 	./libft
LIBFT 			= 	$(LIBFT_DIR)/libft.a
READLINE_FLAGS 	= 	$(shell pkg-config --cflags --libs readline)


all: $(LIBFT) $(NAME)

$(NAME): $(OBJS) $(LIBFT)
	$(CC) $(OBJS) $(LIBFT) $(READLINE_FLAGS) -o $(NAME)

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
