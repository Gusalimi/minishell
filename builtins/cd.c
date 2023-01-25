/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gsaile <gsaile@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/17 11:23:26 by gsaile            #+#    #+#             */
/*   Updated: 2023/01/23 14:34:03 by gsaile           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	bi_cd_home(t_cmd *cmd, char *var, char ***envp)
{
	char	*home;
	char	*tmp;

	home = ft_getenv("$HOME", *envp);
	if (!home)
		return (ft_printf_error("minishell: cd: HOME not set\n"));
	if (chdir(home) == -1)
	{
		perror("cd");
		return (2);
	}
	bi_export((char *[3]){"export", var, NULL}, envp, NULL);
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

static int	bi_cd_last(char *var, char ***envp, int fd)
{
	int		tmp;
	char	*oldpwd;

	oldpwd = ft_strdup(ft_getenv("$OLDPWD", *envp));
	if (oldpwd == NULL)
	{
		free(var);
		ft_printf_error("minishell: cd: OLDPWD not set\n");
		return (1);
	}
	tmp = chdir(oldpwd);
	bi_export((char *[3]){"export", var, NULL}, envp, NULL);
	free(var);
	if (tmp)
		return (1);
	write(fd, oldpwd, ft_strlen(oldpwd));
	write(fd, "\n", 1);
	return (0);
}

static char	*get_var(void)
{
	char	*var;
	char	*path;

	path = getcwd(NULL, 0);
	if (!path)
		return (NULL);
	var = ft_strjoin("OLDPWD=", path);
	free(path);
	return (var);
}

static int	ft_free_return(void *ptr, int rtrn)
{
	free(ptr);
	return (rtrn);
}

int	bi_cd(t_cmd *cmd, char ***envp)
{
	char	*var;
	int		tmp;
	int		ofd;
	int		ifd;

	get_io_fds(&cmd->iostream, &ifd, &ofd);
	var = get_var();
	if (!var)
		return (-1);
	if (!cmd->args[1] || cmd->args[1][0] == '~')
	{
		tmp = bi_cd_home(cmd, var, envp);
		if (tmp != 0)
			return (ft_free_return(var, tmp - 1));
	}
	else if (cmd->args[1][0] == '-' && cmd->args[1][1] == 0)
		return (bi_cd_last(var, envp, ofd));
	if (chdir(cmd->args[1]) == -1)
	{
		perror("cd");
		return (ft_free_return(var, 1));
	}
	bi_export((char *[3]){"export", var, NULL}, envp, NULL);
	free(var);
	return (0);
}
