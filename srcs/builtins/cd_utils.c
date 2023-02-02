/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gsaile <gsaile@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/30 12:34:13 by gsaile            #+#    #+#             */
/*   Updated: 2023/01/31 13:01:53 by gsaile           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_var(char *varname)
{
	char	*var;
	char	*path;

	path = getcwd(NULL, 0);
	if (!path)
		return (NULL);
	var = ft_strjoin(varname, path);
	free(path);
	return (var);
}

int	set_pwd(char ***envp)
{
	char	*pwd;

	pwd = get_var("PWD=");
	if (!pwd)
		return (-1);
	if (bi_export((char *[3]){"export", pwd, NULL}, envp, NULL) == -1)
	{
		free(pwd);
		return (-1);
	}
	free(pwd);
	return (0);
}

int	ft_free_return(void *ptr, int rtrn)
{
	free(ptr);
	return (rtrn);
}

static int	bi_cd_home2(t_cmd *cmd, char *var, char ***envp)
{
	char	*tmp;

	if (bi_export((char *[3]){"export", var, NULL}, envp, NULL) == -1)
		return (-1);
	if (set_pwd(envp) == -1)
		return (-1);
	if (!cmd->args[1])
		return (1);
	if (!cmd->args[1][1])
		return (1);
	tmp = ft_strdup(cmd->args[1] + 2);
	if (!tmp)
		return (-1);
	free(cmd->args[1]);
	cmd->args[1] = tmp;
	return (0);
}

int	bi_cd_home(t_cmd *cmd, char *var, char ***envp)
{
	char	*home;

	home = ft_getenv("$HOME", *envp);
	if (!home)
	{
		if (ft_printf_error("minishell: cd: HOME not set\n") == -1)
			return (-1);
		return (2);
	}
	if (chdir(home) == -1)
	{
		perror("cd");
		return (2);
	}
	return (bi_cd_home2(cmd, var, envp));
}
