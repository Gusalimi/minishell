/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   start_executing_cmds.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lloison < lloison@student.42mulhouse.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/24 16:08:26 by lloison           #+#    #+#             */
/*   Updated: 2023/01/25 15:35:52 by lloison          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_bool	should_ignore_cmd(t_cmd **cmds, int i)
{
	if (i <= 0)
		return (FALSE);
	if (cmds[i - 1]->next_breakpoint == br_and)
	{
		if (ft_atoi(get_les()) != 0)
			return (TRUE);
	}
	else if (cmds[i - 1]->next_breakpoint == br_or)
	{
		if (ft_atoi(get_les()) == 0)
			return (TRUE);
	}
	return (FALSE);
}

static int	exec_solo_cmd(t_cmd *cmd, char ***env)
{
	pid_t	pid;
	int		exit_status;
	exit_status = finish_parsing(cmd, *env);
	if (exit_status != 0)
		return (exit_status);
	if (cmd->is_builtin)
		if (execute_builtin(cmd, env, TRUE) == -1)
			return (-1);
	if (cmd->is_subshell)
		if (exec_subshell(cmd, env) == -1)
			return (-1);
	if (cmd->is_builtin || cmd->is_subshell)
		return (0);
	pid = fork();
	if (pid == -1)
		return (-1);
	if (pid == 0)
		execute_file(cmd, *env);
	if (waitpid(pid, &exit_status, 0) == -1)
		return (-1);
	set_les(exit_status);
	return (0);
}

int	start_executing_cmds(t_cmd **cmds, char ***env)
{
	int		cmd_index;

	cmd_index = 0;
	while (cmds[cmd_index])
	{
		if (should_ignore_cmd(cmds, cmd_index))
			continue ;
		if (cmds[cmd_index]->next_breakpoint == br_pipe)
		{
			if (start_pipeline(cmds, &cmd_index, env) == -1)
				return (-1);
			//continue ;
		}
		else
			if (exec_solo_cmd(cmds[cmd_index], env) == -1)
				return (-1);
		cmd_index++;
	}
	return (0);
}
