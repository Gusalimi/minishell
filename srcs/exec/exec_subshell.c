/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_subshell.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lloison < lloison@student.42mulhouse.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/25 14:23:43 by lloison           #+#    #+#             */
/*   Updated: 2023/01/31 15:43:28 by lloison          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	exec_subshell(t_cmd *cmd, char ***env)
{
	pid_t	pid;
	int		exit_status;

	pid = fork();
	if (pid == -1)
		return (-1);
	else if (pid == 0)
	{
		set_ifs(TRUE);
		if (handle_redirects(cmd->iostream) != 0)
			exit(1);
		if (handle_cmd(cmd->args[1], env, FALSE) == -1)
			exit(255);
		exit(ft_atoi(get_les()));
	}
	clear_cmd_fds(&(cmd->iostream));
	if (waitpid(pid, &exit_status, 0) == -1)
		return (-1);
	set_les(WEXITSTATUS(exit_status));
	return (0);
}
