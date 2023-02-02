/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_cmd_path.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lloison < lloison@student.42mulhouse.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/17 13:11:11 by lloison           #+#    #+#             */
/*   Updated: 2023/01/30 18:36:41 by lloison          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	**get_all_paths(char **env)
{
	char	**output;
	char	*paths;

	paths = ft_getenv("$PATH", env);
	if (!paths)
		return (NULL);
	output = ft_split(paths, ':');
	if (output == 0)
		return (NULL);
	return (output);
}

static void	*free_all_paths(char **all_paths)
{
	int	i;

	if (!all_paths)
		return (NULL);
	i = 0;
	while (all_paths[i])
	{
		free(all_paths[i]);
		i++;
	}
	free(all_paths);
	return (NULL);
}

static char	*get_cmd_path2(char *cmd, char **all_paths, int i)
{
	char	*tmp;
	char	*path;

	while (all_paths[++i])
	{
		tmp = ft_strjoin(all_paths[i], "/");
		if (tmp == 0)
			return (free_all_paths(all_paths));
		path = ft_strjoin(tmp, cmd);
		free(tmp);
		if (path == 0)
			return (free_all_paths(all_paths));
		if (access(path, F_OK) == 0)
		{
			free_all_paths(all_paths);
			return (path);
		}
		free(path);
	}
	free_all_paths(all_paths);
	return (ft_strdup(cmd));
}

char	*get_cmd_path(char *cmd, char **env)
{
	char	**all_paths;

	if (is_cmd_name_empty(cmd) || cmd[0] == '.' || cmd[0] == '/')
		return (ft_strdup(cmd));
	all_paths = get_all_paths(env);
	if (!all_paths)
		return (ft_strdup(cmd));
	return (get_cmd_path2(cmd, all_paths, -1));
}
