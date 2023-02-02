/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gsaile <gsaile@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/17 12:05:08 by gsaile            #+#    #+#             */
/*   Updated: 2023/02/02 16:04:15 by gsaile           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
	if (!(*s) || *s == '=' || *s == '+')
		return (FALSE);
	while (s[i] && s[i] != '=' && s[i] != '+')
	{
		if (!ft_isalnum(s[i]) && s[i] != '_')
			return (FALSE);
		i++;
	}
	if (s[i] == '+' && s[i + 1] != '=')
		return (FALSE);
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
	name++;
	len = ft_strlen(name);
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
	if (!tmp)
		return (-1);
	name = ft_strjoin("$", tmp);
	free(tmp);
	if (!name)
		return (-1);
	if (has_plus(argv[i]))
	{
		free(name);
		return (append(argv[i], envp));
	}
	else if (ft_getenv(name, *envp) || exists_no_equal(name, *envp))
	{
		free(name);
		return (modify(argv[i], envp));
	}
	free(name);
	return (add(argv[i], envp));
}
