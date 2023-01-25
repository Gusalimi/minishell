/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gsaile <gsaile@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/17 12:05:08 by gsaile            #+#    #+#             */
/*   Updated: 2023/01/24 16:18:07 by gsaile           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/* Nom env var valide = Commence pas par chiffre et uniquement lettres,
	chiffres et underscore */
static int	bi_export2(char **argv, char ***envp);
static int	bi_export3(char **argv, char ***envp, int i);

static t_bool	is_valid(char *s)
{
	int	i;

	i = 0;
	if (ft_isdigit(*s))
		return (FALSE);
	while (ft_isdigit(*s))
		s++;
	if (!(*s) || *s == '=')
		return (FALSE);
	while (s[i] && s[i] != '=')
	{
		if (!ft_isalnum(s[i]) && s[i] != '_')
			return (FALSE);
		i++;
	}
	return (TRUE);
}

int	bi_export(char **argv, char ***envp, t_iostream *iostream)
{
	int		ifd;
	int		ofd;
	int		return_value;

	return_value = 0;
	get_io_fds(iostream, &ifd, &ofd);
	if (!argv[1])
		return (print_env_export(*envp, ofd));
	return (bi_export2(argv, envp));
}

static int	bi_export2(char **argv, char ***envp)
{
	int	return_value;
	int	i;

	return_value = 0;
	i = 1;
	while (argv[i])
	{
		if (!is_valid(argv[i]))
		{
			ft_printf_error("minishell: export: `%s\': not a valid identifier\n",
				argv[i++]);
			return_value = 1;
			continue ;
		}
		if (bi_export3(argv, envp, i++) == -1)
			return (-1);
	}
	return (return_value);
}

t_bool	exists_no_equal(char *name, char **envp)
{
	int	i;
	int	len;

	i = 0;
	len = ft_strlen(name);
	name++;
	while (envp[i])
	{
		if (ft_strncmp(envp[i], name, len) == 0 && envp[i][len] == 0)
			return (TRUE);
		i++;
	}
	return (FALSE);
}

static int	bi_export3(char **argv, char ***envp, int i)
{
	char	*tmp;
	char	*name;

	tmp = ft_strndup(argv[i], ft_strchr(argv[i], '=') - argv[i]);
	name = ft_strjoin("$", tmp);
	free(tmp);
	if (ft_getenv(name, *envp) || exists_no_equal(name, *envp))
	{
		free(name);
		if (modify(argv[i], envp) == -1)
			return (-1);
		return (0);
	}
	free(name);
	if (add(argv[i], envp) == -1)
		return (-1);
	return (0);
}
