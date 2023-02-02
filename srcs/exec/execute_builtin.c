/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_builtin.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lloison < lloison@student.42mulhouse.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/19 19:19:00 by lloison           #+#    #+#             */
/*   Updated: 2023/01/31 11:53:54 by lloison          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	execute_child(t_cmd *cmd, char **env)
{
	set_ifs(TRUE);
	if (handle_redirects(cmd->iostream) != 0)
		exit (-1);
	if (compare_text_no_case("echo", cmd->args[0], 5) == 0)
		exit (bi_echo(cmd->args, env));
	if (compare_text_no_case("pwd", cmd->args[0], 4) == 0)
		exit (bi_pwd(cmd->args, env));
	if (compare_text_no_case("env", cmd->args[0], 4) == 0)
		exit (bi_env(cmd->args, env));
}

static int	fork_and_execute(t_cmd *cmd, char **env)
{
	int		exit_status;
	pid_t	pid;

	pid = fork();
	if (pid == -1)
		return (-1);
	if (pid == 0)
		execute_child(cmd, env);
	else
	{
		clear_cmd_fds(&(cmd->iostream));
		if (cmd->next_breakpoint != br_pipe)
		{
			waitpid(pid, &exit_status, 0);
			set_les(WEXITSTATUS(exit_status));
		}
	}
	return (0);
}

int	execute_builtin(t_cmd *cmd, char ***env, t_bool should_fork)
{
	if (should_fork
		&& (compare_text_no_case("echo", cmd->args[0], 5) == 0
			|| compare_text_no_case("pwd", cmd->args[0], 4) == 0
			|| compare_text_no_case("env", cmd->args[0], 4) == 0))
		if (fork_and_execute(cmd, *env) == -1)
			return (-1);
	if (!should_fork && compare_text_no_case("echo", cmd->args[0], 5) == 0)
		set_les(bi_echo(cmd->args, *env));
	else if (!should_fork && compare_text_no_case("pwd", cmd->args[0], 4) == 0)
		set_les(bi_pwd(cmd->args, *env));
	else if (!should_fork && compare_text_no_case("env", cmd->args[0], 4) == 0)
		set_les(bi_env(cmd->args, *env));
	else if (compare_text_no_case("cd", cmd->args[0], 3) == 0)
		set_les(bi_cd(cmd, env));
	else if (compare_text_no_case("export", cmd->args[0], 7) == 0)
		set_les(bi_export(cmd->args, env, &cmd->iostream));
	else if (compare_text_no_case("unset", cmd->args[0], 6) == 0)
		set_les(bi_unset(cmd->args, env));
	else if (compare_text_no_case("exit", cmd->args[0], 5) == 0)
		set_les(bi_exit(cmd->args, *env));
	clear_cmd_fds(&(cmd->iostream));
	if (ft_atoi(get_les()) == -1)
		return (-1);
	return (0);
}
