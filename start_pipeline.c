/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   start_pipeline.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lloison < lloison@student.42mulhouse.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/24 16:25:22 by lloison           #+#    #+#             */
/*   Updated: 2023/01/25 14:40:14 by lloison          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	exec_child(t_cmd *cmd, char ***env)
{
	int	rtn_value;

	rtn_value = finish_parsing(cmd, *env);
	if (rtn_value != 0)
		exit(rtn_value - 1);
	if (handle_redirects(cmd->iostream) == -1)
		exit(255);
	if (cmd->is_builtin)
	{
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

static int	fork_and_exec(t_cmd *cmd, char ***env)
{
	pid_t	rtn_value;

	rtn_value = fork();
	if (rtn_value != 0)
		return (rtn_value);
	else
		exec_child(cmd, env);
	return (rtn_value);
}

static int	pipe_fork_and_exec(t_cmd **cmds, int *cmd_i, char ***env)
{
	int	rtn_value;

	if (setup_pipe(cmds, *cmd_i) == -1)
		return (-1);
	rtn_value = fork_and_exec(cmds[*cmd_i], env);
	if (rtn_value == -1)
		return (-1);
	clear_cmd_fds(&(cmds[*cmd_i]->iostream));
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
	pids_i = 0;
	while (cmds[*cmd_i]->next_breakpoint == br_pipe)
	{
		pids[pids_i] = pipe_fork_and_exec(cmds, cmd_i, env);
		if (pids[pids_i] == -1)
			return (free_and_return(pids, -1));
		*cmd_i = *cmd_i + 1;
		pids_i++;
	}
	pids[pids_i] = pipe_fork_and_exec(cmds, cmd_i, env);
	if (pids[pids_i] == -1 || waitpid(pids[pids_i], &exit_status, 0) == -1)
		return (free_and_return(pids, -1));
	while (--pids_i >= 0)
		if (waitpid(pids[pids_i], 0, 0) == -1)
			return (free_and_return(pids, -1));
	set_les(exit_status);
	return (free_and_return(pids, 0));
}
