/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   start_pipeline.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gsaile <gsaile@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/24 16:25:22 by lloison           #+#    #+#             */
/*   Updated: 2023/01/31 14:05:20 by gsaile           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	exec_child(t_cmd *cmd, char ***env)
{
	if (cmd->is_builtin)
	{
		if (handle_redirects(cmd->iostream) != 0)
			exit(1);
		if (execute_builtin(cmd, env, FALSE) == -1)
			exit(255);
		exit (ft_atoi(get_les()));
	}
	if (cmd->is_subshell)
	{
		if (exec_subshell(cmd, env) == -1)
			exit(255);
		exit (ft_atoi(get_les()));
	}
	execute_file(cmd, *env);
}

int	fork_and_exec(t_cmd *cmd, char ***env)
{
	pid_t	rtn_value;

	rtn_value = finish_parsing(cmd, *env);
	if (rtn_value != 0)
		return (rtn_value);
	rtn_value = fork();
	if (rtn_value != 0)
	{
		clear_cmd_fds(&(cmd->iostream));
		return (rtn_value);
	}
	else
	{
		if (set_underscore(cmd, env) == -1)
			return (-1);
		set_ifs(TRUE);
		exec_child(cmd, env);
	}
	return (rtn_value);
}

int	pipe_fork_and_exec(t_cmd **cmds, int *cmd_i, char ***env)
{
	int	rtn_value;

	if (setup_pipe(cmds, *cmd_i) == -1)
		return (-1);
	rtn_value = fork_and_exec(cmds[*cmd_i], env);
	if (rtn_value == -1)
		return (-1);
	return (rtn_value);
}

static int	free_and_return(pid_t *pids, int rtn_value)
{
	free(pids);
	return (rtn_value);
}

int	start_pipeline(t_cmd **cmds, int *cmd_i, char ***env)
{
	pid_t	*pids;
	int		exit_status;
	int		pids_i;

	pids = init_pids(cmds, *cmd_i);
	if (!pids)
		return (free_and_return(NULL, -1));
	pids_i = fill_pids(cmds, cmd_i, env, pids);
	if (pids_i == -1)
		return (free_and_return(pids, -1));
	if (waitpid(pids[pids_i], &exit_status, 0) == -1)
		return (free_and_return(pids, -1));
	if (bi_unset((char *[3]){"unset", "_", NULL}, env) == -1)
		return (-1);
	set_les(WEXITSTATUS(exit_status));
	while (--pids_i >= 0)
		if (waitpid(pids[pids_i], 0, 0) == -1)
			return (free_and_return(pids, -1));
	return (free_and_return(pids, 0));
}
