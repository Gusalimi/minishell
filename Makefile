NAME	= minishell

SRCS	=	parsing/parse_line.c \
			parsing/is_line_valid.c \
			parsing/is_line_valid2.c \
			parsing/is_line_valid3.c \
			parsing/get_cmd_args.c \
			parsing/get_next_arg.c \
			parsing/parsing_utils.c \
			parsing/parsing_utils2.c \
			parsing/parsing_utils3.c \
			parsing/parsing_utils4.c \
			parsing/parsing_utils5.c \
			parsing/parsing_utils6.c \
			parsing/get_cmd_path.c \
			parsing/init_cmd_iostream.c \
			parsing/handle_here_doc.c \
			parsing/is_number_valid.c \
			parsing/ft_split_is_space.c \
			parsing/finish_parsing.c \
			parsing/replace_env_variable.c \
			\
			builtins/echo.c \
			builtins/env.c \
			builtins/cd.c \
			builtins/pwd.c \
			builtins/export.c \
			builtins/export_utils.c \
			builtins/export_utils2.c \
			builtins/unset.c \
			builtins/exit.c \
			\
			main.c \
			init_minishell.c \
			env_handler.c \
			handle_cmds.c \
			history.c \
			last_exit_status.c \
			in_cmd_status.c \
			handle_redirects.c \
			handle_redirects2.c \
			execute_builtin.c \
			exec_subshell.c \
			execute_file.c \
			start_executing_cmds.c \
			start_pipeline.c \
			start_pipeline_utils.c \

OBJS	= $(SRCS:.c=.o)
RL		= -lreadline -L /Users/$$USER/.brew/opt/readline/lib 
INC		= -I /Users/$$USER/.brew/opt/readline/include -I .
LIBFT	= libft/libft_enhanced.a
CC		= gcc
CFLAGS	= -Wall -Wextra -Werror -g3

.c.o:
	@$(CC) -c $(CFLAGS) $(INC) $< -o $(<:.c=.o)

all:	$(NAME)

$(NAME):	$(OBJS) $(LIBFT)
	@echo minishell compiled
	@$(CC) $(CFLAGS) $(OBJS) $(LIBFT) $(RL) -o $(NAME)

debug:	$(OBJS) $(LIBFT)
	$(CC) $(CFLAGS) $(OBJS) $(LIBFT) $(RL) -o $(NAME) -fsanitize=address

$(LIBFT):
	@echo libft compiled
	@$(MAKE) -C libft

clean:
	$(RM) $(OBJS)
	$(MAKE) clean -C libft

fclean:	clean
	$(RM) $(NAME)
	$(MAKE) fclean -C libft

re:	fclean all

.PHONY: all debug clean fclean re