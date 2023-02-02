/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_file.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lloison < lloison@student.42mulhouse.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/19 19:17:55 by lloison           #+#    #+#             */
/*   Updated: 2023/01/30 18:01:42 by lloison          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_bool	is_path_empty(char **env)
{
	char	*path_var;

	path_var = ft_getenv("$PATH", env);
	if (path_var && path_var[0] == 0)
		return (TRUE);
	return (FALSE);
}

static void	check_error(t_cmd *cmd, char **env)
{
	if ((ft_strchr(cmd->path, '/')) && is_dir(cmd->path))
	{
		ft_printf_error("minishell: %s: is a directory\n", cmd->name);
		exit(126);
	}
	if ((access(cmd->path, F_OK) == -1 && ft_strchr(cmd->path, '/'))
		|| is_path_empty(env))
	{
		ft_printf_error("minishell: %s: No such file or directory\n",
			cmd->name);
		exit(127);
	}
	if (!ft_strchr(cmd->path, '/'))
	{
		ft_printf_error("minishell: %s: command not found\n", cmd->name);
		exit(127);
	}
	if (access(cmd->path, X_OK) == -1)
	{
		ft_printf_error("minishell: ");
		perror(cmd->path);
		exit(126);
	}
}

void	execute_file(t_cmd *cmd, char **env)
{
	set_ifs(TRUE);
	check_error(cmd, env);
	if (handle_redirects(cmd->iostream) != 0)
		exit(1);
	execve(cmd->path, cmd->args, env);
	exit(0);
}
