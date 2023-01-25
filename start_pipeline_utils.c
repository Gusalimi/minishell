/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   start_pipeline_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lloison < lloison@student.42mulhouse.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/25 14:21:26 by lloison           #+#    #+#             */
/*   Updated: 2023/01/25 14:40:35 by lloison          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	setup_pipe(t_cmd **cmds, int i)
{
	if (pipe(cmds[i]->iostream.output_pipe) == -1)
		return (-1);
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
