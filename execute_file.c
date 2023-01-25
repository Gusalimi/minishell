/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_file.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lloison < lloison@student.42mulhouse.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/19 19:17:55 by lloison           #+#    #+#             */
/*   Updated: 2023/01/25 14:34:01 by lloison          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	execute_file(t_cmd *cmd, char **env)
{
	if ((cmd->path[0] == '.' || cmd->path[0] == '/') && is_dir(cmd->path))
	{
		ft_printf_error("minishell: %s: is a directory\n", cmd->name);
		exit(126);
	}
	if (access(cmd->path, F_OK) == -1 || is_dir(cmd->path))
	{
		if (cmd->path[0] == '/' || cmd->path[1] == '/')
			ft_printf_error("minishell: %s: No such file or directory\n",
				cmd->name);
		else
			ft_printf_error("minishell: %s: command not found\n", cmd->name);
		exit(127);
	}
	if (access(cmd->path, X_OK) == -1)
	{
		ft_printf_error("minishell: ");
		perror(cmd->name);
		exit(126);
	}
	if (handle_redirects(cmd->iostream) != 0)
		exit(1);
	execve(cmd->path, cmd->args, env);
	exit(0);
}
