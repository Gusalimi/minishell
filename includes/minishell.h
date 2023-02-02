/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gsaile <gsaile@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/16 13:22:15 by lloison           #+#    #+#             */
/*   Updated: 2023/02/02 16:12:08 by gsaile           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "iostream.h"
# include "libft_e.h"
# include <curses.h>
# include <dirent.h>
# include <errno.h>
# include <fcntl.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/ioctl.h>
# include <sys/stat.h>
# include <sys/wait.h>
# include <termios.h>
# include <unistd.h>

typedef enum e_breakpoint_type
{
	br_none,
	br_pipe,
	br_and,
	br_or
}						t_breakpoint_type;

typedef struct s_pos
{
	int					arg_i;
	int					string_i;
}						t_pos;

typedef struct s_file
{
	char				*name;
	struct s_file		*next;
}						t_file;

typedef struct s_cmd
{
	char				**args;
	char				*name;
	char				*path;
	t_bool				is_builtin;
	t_bool				is_subshell;
	t_iostream			iostream;
	t_breakpoint_type	next_breakpoint;
}						t_cmd;

/* - - - - - parsing - - - - - */
t_cmd					**parse_line(char *line);
t_bool					is_line_valid(char *line);
char					**get_cmd_args(char *line, int *line_i, t_bool in_par);
char					*get_next_arg(char *line, int *line_i, t_bool *in_par);
char					*get_cmd_path(char *cmd, char **env);
int						init_cmd_iostream(t_cmd *cmd, char **env, int i);
int						finish_parsing(t_cmd *cmd, char **env);
int						replace_env_variable(t_cmd *cmd, char **env);
char					*replace_arg(char *arg, char **env);
void					remove_quotes_pos(char **args, int *quotes_pos);
int						*get_quotes_pos(char **args, char **env);
char					**get_updated_args(char **args, int arg_i, char **env);
int						get_updated_nb_args(char **args, int arg_i, char **env);
void					fill_buf(char *buf, char *arg, int *arg_i, char **env);
int						replace_var_in_quotes(char **args, int arg_i,
							char **env);
char					**get_files_wildcard(char *s, int *error,
							int *wd_pos, int nb_wd);
void					remove_empty_args(t_cmd *cmd);
int						*get_wildcards_pos(char **args, char **env);
int						expand_wildcard(t_cmd *cmd, int *wildcards_pos,
							int *wdn_wdpi_i[3], int j);
int						*get_arg_wd_pos(char *arg, int *wildcards_pos,
							int wd_pos_i, int first_wd_pos);

/* - - - - - parsing_utils - - - - - */
t_bool					skip_dollar(char *line, int *main_i);
t_bool					is_space(char c);
t_bool					is_breakpoint(char c1, char c2);
t_bool					skip_quotes(char *line, int *main_i);
void					skip_breakpoint(char *line, int *main_i);
t_bool					is_ending_quote(char *line, int i);
t_bool					is_redir(char c1, char c2);
void					skip_redir(char *line, int *main_i);
t_bool					is_symbol_char(char c);
t_bool					is_valid_symbols(char *line, int i);
void					skip_valid_symbols(char *line, int *i);
int						get_valid_symbol_length(char *line, int i);
t_bool					is_cmd_builtin(char *cmd);
char					*get_cmd_name(char **args, char *line);
t_bool					is_cmd_name_empty(char *s);
int						handle_here_doc(t_cmd *cmd, int i);
int						compare_text_no_case(char *s1, char *s2, size_t n);
t_bool					is_dir(char *filename);
t_bool					check_nb_braces(char *s, int *total_count);
t_bool					check_braces_positions(char *s, int nb_braces);
t_bool					check_for_no_empty_braces(char *s);
t_bool					is_number_valid(const char *str);
t_bool					has_alpha(const char *s);
void					skip_redir_and_filename(char *line, int *i);
void					skip_one_arg(char *line, int *i);
char					**ft_split_is_space(char const *s);
void					skip_parenthesis(char *line, int *i);
int						get_nb_cmds(char *line, int i);
int						get_par_length(char *s, int i);
t_bool					is_cmd_subshell(char *s);
void					copy_var_to_buf(char *buf, int *buf_i, char *arg,
							char **env);
void					split_copy_var_to_buf(char *buf, int *buf_i, char *arg,
							char **env);
int						is_ambiguous(t_cmd *cmd, int i, int len, char **env);
t_bool					arg_has_wildcards(char *arg);
int						nb_wildcard_in_var(char *arg, char **env);
char					**split_wd(char const *s, int *wd_pos, int nb_wd);
void					list_remove_if(t_list **lst,
							t_bool (*test)(), char **param);
t_bool					hidden(char *file, char **origin);
t_bool					begin(char *file, char **origin);
t_bool					end(char *file, char **origin);
t_bool					between(char *file, char **no_stars);

/* - - - - - builtins - - - - - */
int						bi_exit(char **argv, char **envp);
int						bi_pwd(char **argv, char **envp);
int						bi_unset(char **argv, char ***envp);
int						bi_env(char **argv, char **envp);
int						bi_echo(char **argv, char **envp);
int						bi_export(char **argv, char ***envp,
							t_iostream *iostream);
int						bi_cd(t_cmd *cmd, char ***envp);

/* - - - - - builtins_utils - - - - - */
int						add(char *var, char ***envp);
int						append(char *var, char ***envp);
int						modify(char *var, char ***envp);
int						print_env_export(char **envp, int fd);
t_bool					has_plus(char *s);
char					*get_var(char *varname);
int						set_pwd(char ***envp);
int						ft_free_return(void *ptr, int rtrn);
int						bi_cd_home(t_cmd *cmd, char *var, char ***envp);
t_bool					exists_no_equal(char *name, char **envp);
void					sort_env(char ***env);

/* - - - - - init_minishell.c - - - - - */
int						init_minishell(char **env, char ***envp, int *fd,
							char **line);
void					overwrite_ctrl_c(int s);

/* - - - - - env_handler.c - - - - - */
char					**create_env(char **envp);
int						free_all(char **tab);
char					*ft_getenv(char *var, char **envp);

/* - - - - - handle_cmds.c - - - - - */
int						handle_cmd(char *line, char ***env, t_bool check_valid);
void					*free_cmds(t_cmd **cmds);

/* - - - - - history.c - - - - - */
int						create_history(void);
void					add_to_history(int fd, char *line);

/* - - - - - last_exit_status.c - - - - - */
char					*get_les(void);
void					set_les(int exit_status);

/* - - - - - in_cmds_status.c - - - - - */
t_bool					get_ics(void);
void					set_ics(t_bool value);

/* - - - - - in_fork_status.c - - - - - */
t_bool					get_ifs(void);
void					set_ifs(t_bool value);

/* - - - - - handle_redirects.c - - - - - */
int						handle_redirects(t_iostream iostream);
int						clear_cmd_fds(t_iostream *iostream);
int						get_io_fds(t_iostream *iostream,
							int *input_fd,
							int *output_fd);

/* - - - - - execute_file.c - - - - - */
void					execute_file(t_cmd *cmd, char **env);

/* - - - - - execute_builtin.c - - - - - */
int						execute_builtin(t_cmd *cmd, char ***env,
							t_bool should_fork);

/* - - - - - start_executing_cmds.c - - - - - */
int						start_executing_cmds(t_cmd **cmds, char ***env);
int						set_underscore(t_cmd *cmd, char ***env);

/* - - - - - start_pipeline.c - - - - - */
int						start_pipeline(t_cmd **cmds, int *cmd_i, char ***env);
int						fork_and_exec(t_cmd *cmd, char ***env);
int						pipe_fork_and_exec(t_cmd **cmds, int *cmd_i,
							char ***env);
int						fill_pids(t_cmd **cmds, int *cmd_i, char ***env,
							pid_t *pids);
int						setup_pipe(t_cmd **cmds, int i);
pid_t					*init_pids(t_cmd **cmds, int cmd_i);

/* - - - - - exec_subshell.c - - - - - */
int						exec_subshell(t_cmd *cmd, char ***env);

/* - - - - - shlvl.c - - - - - */
int						shlvl(char ***envp);

#endif