NAME	= minishell

SRCS	=	srcs/parsing/parse_line.c \
			srcs/parsing/is_line_valid.c \
			srcs/parsing/is_line_valid2.c \
			srcs/parsing/is_line_valid3.c \
			srcs/parsing/get_cmd_args.c \
			srcs/parsing/get_next_arg.c \
			srcs/parsing/parsing_utils.c \
			srcs/parsing/parsing_utils2.c \
			srcs/parsing/parsing_utils3.c \
			srcs/parsing/parsing_utils4.c \
			srcs/parsing/parsing_utils5.c \
			srcs/parsing/parsing_utils6.c \
			srcs/parsing/parsing_utils7.c \
			srcs/parsing/get_cmd_path.c \
			srcs/parsing/init_cmd_iostream.c \
			srcs/parsing/handle_here_doc.c \
			srcs/parsing/is_number_valid.c \
			srcs/parsing/ft_split_is_space.c \
			srcs/parsing/finish_parsing.c \
			srcs/parsing/replace_env_variable.c \
			srcs/parsing/replace_arg.c \
			srcs/parsing/get_quotes_pos.c \
			srcs/parsing/get_updated_args.c \
			srcs/parsing/get_updated_args_utils.c \
			srcs/parsing/get_updated_nb_args.c \
			srcs/parsing/replace_var_in_quotes.c \
			srcs/parsing/handle_ambiguous.c \
			srcs/parsing/wildcard.c \
			srcs/parsing/wildcard_utils.c \
			srcs/parsing/remove_empty_args.c \
			srcs/parsing/remove_quote_pos.c \
			srcs/parsing/get_wildcards_pos.c \
			srcs/parsing/expand_wildcards.c \
			srcs/parsing/split_wildcard.c \
			srcs/parsing/get_arg_wildcards_pos.c \
			\
			srcs/builtins/echo.c \
			srcs/builtins/env.c \
			srcs/builtins/cd.c \
			srcs/builtins/pwd.c \
			srcs/builtins/export.c \
			srcs/builtins/export_utils.c \
			srcs/builtins/export_utils2.c \
			srcs/builtins/export_utils3.c \
			srcs/builtins/unset.c \
			srcs/builtins/exit.c \
			srcs/builtins/cd_utils.c \
			\
			srcs/exec/main.c \
			srcs/exec/init_minishell.c \
			srcs/exec/env_handler.c \
			srcs/exec/handle_cmds.c \
			srcs/exec/history.c \
			srcs/exec/last_exit_status.c \
			srcs/exec/in_cmd_status.c \
			srcs/exec/in_fork_status.c \
			srcs/exec/handle_redirects.c \
			srcs/exec/handle_redirects2.c \
			srcs/exec/execute_builtin.c \
			srcs/exec/exec_subshell.c \
			srcs/exec/execute_file.c \
			srcs/exec/start_executing_cmds.c \
			srcs/exec/start_pipeline.c \
			srcs/exec/start_pipeline_utils.c \
			srcs/exec/shlvl.c \

OBJS	= $(SRCS:.c=.o)
RL		= -lreadline -L /Users/$$USER/.brew/opt/readline/lib 
INC		= -I /Users/$$USER/.brew/opt/readline/include -I includes
LIBFT	= srcs/libft/libft_enhanced.a
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
	@$(MAKE) -C srcs/libft

clean:
	$(RM) $(OBJS)
	$(MAKE) clean -C srcs/libft

fclean:	clean
	$(RM) $(NAME)
	$(MAKE) fclean -C srcs/libft

re:	fclean all

.PHONY: all debug clean fclean re