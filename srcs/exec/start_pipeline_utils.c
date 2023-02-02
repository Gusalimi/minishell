/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   start_pipeline_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lloison < lloison@student.42mulhouse.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/25 14:21:26 by lloison           #+#    #+#             */
/*   Updated: 2023/01/30 18:20:29 by lloison          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	setup_pipe(t_cmd **cmds, int i)
{
	if (pipe(cmds[i]->iostream.output_pipe) == -1)
	{
		return (-1);
	}
	if (cmds[i + 1])
	{
		cmds[i + 1]->iostream.input_pipe[0] = cmds[i]->iostream.output_pipe[0];
		cmds[i + 1]->iostream.input_pipe[1] = cmds[i]->iostream.output_pipe[1];
	}
	return (0);
}

pid_t	*init_pids(t_cmd **cmds, int cmd_i)
{
	pid_t	*output;
	int		i;

	i = 0;
	while (cmds[cmd_i + i]->next_breakpoint == br_pipe)
		i++;
	output = ft_calloc(sizeof(pid_t), i + 1);
	return (output);
}

static int	free_and_return(pid_t *pids, int rtn_value)
{
	free(pids);
	return (rtn_value);
}

int	fill_pids(t_cmd **cmds, int *cmd_i, char ***env, pid_t *pids)
{
	int	pids_i;
	int	check;

	pids_i = 0;
	(*cmd_i)--;
	while (cmds[++(*cmd_i)]->next_breakpoint == br_pipe)
	{
		check = pipe_fork_and_exec(cmds, cmd_i, env);
		if (check == 2)
			continue ;
		pids[pids_i] = check;
		if (pids[pids_i] == -1)
			return (free_and_return(pids, -1));
		pids_i++;
	}
	check = fork_and_exec(cmds[*cmd_i], env);
	if (check == 2)
		pids_i--;
	else
		pids[pids_i] = check;
	if (pids[pids_i] == -1)
		return (-1);
	return (pids_i);
}
