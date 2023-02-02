/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gsaile <gsaile@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/17 11:23:26 by gsaile            #+#    #+#             */
/*   Updated: 2023/02/02 16:04:06 by gsaile           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	bi_cd_last2(char *var, char ***envp, int fd, char *oldpwd)
{
	int	tmp;
	int	rv;

	tmp = chdir(oldpwd);
	rv = bi_export((char *[3]){"export", var, NULL}, envp, NULL);
	free(var);
	if (rv == -1)
		return (-1);
	if (set_pwd(envp) == -1)
		return (-1);
	if (tmp != 0)
	{
		free(oldpwd);
		return (1);
	}
	write(fd, oldpwd, ft_strlen(oldpwd));
	write(fd, "\n", 1);
	free(oldpwd);
	return (0);
}

static int	bi_cd_last(char *var, char ***envp, int fd)
{
	char	*oldpwd;

	oldpwd = ft_strdup(ft_getenv("$OLDPWD", *envp));
	if (oldpwd == NULL)
	{
		free(var);
		if (!ft_getenv("$OLDPWD", *envp))
		{
			if (ft_printf_error("minishell: cd: OLDPWD not set\n") == -1)
				return (-1);
			return (1);
		}
		return (-1);
	}
	return (bi_cd_last2(var, envp, fd, oldpwd));
}

static int	bi_cd2(char **args, char ***envp, char *opwd)
{
	int	rv;

	if (chdir(args[1]) == -1)
	{
		perror("cd");
		return (ft_free_return(opwd, 1));
	}
	rv = bi_export((char *[3]){"export", opwd, NULL}, envp, NULL);
	free(opwd);
	if (rv == -1)
		return (-1);
	if (set_pwd(envp) == -1)
		return (-1);
	return (0);
}

int	bi_cd(t_cmd *cmd, char ***envp)
{
	char	*opwd;
	int		tmp;
	int		ofd;
	int		ifd;

	get_io_fds(&cmd->iostream, &ifd, &ofd);
	opwd = get_var("OLDPWD=");
	if (!opwd)
		return (-1);
	if (!cmd->args[1] || cmd->args[1][0] == '~')
	{
		tmp = bi_cd_home(cmd, opwd, envp);
		if (tmp != 0)
			return (ft_free_return(opwd, tmp - 1));
	}
	else if (cmd->args[1][0] == '-' && cmd->args[1][1] == 0)
		return (bi_cd_last(opwd, envp, ofd));
	return (bi_cd2(cmd->args, envp, opwd));
}
