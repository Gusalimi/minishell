/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gsaile <gsaile@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/17 14:28:07 by gsaile            #+#    #+#             */
/*   Updated: 2023/01/24 16:22:16 by gsaile           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static t_bool	is_valid(char *s)
{
	int	i;

	i = 0;
	if (ft_isdigit(*s))
		return (FALSE);
	while (ft_isdigit(*s))
		s++;
	if (!(*s))
		return (FALSE);
	while (s[i])
	{
		if (!ft_isalnum(s[i]) && s[i] != '_')
			return (FALSE);
		i++;
	}
	return (TRUE);
}

static int	ft_remove(char *var, char ***envp, char **new_env)
{
	int		len;
	int		i;
	int		j;	

	len = 0;
	i = -1;
	j = 0;
	while ((*envp)[len])
		len++;
	new_env = malloc((len) * sizeof(char *));
	if (!new_env)
		return (free_all(*envp));
	new_env[len - 1] = 0;
	len = ft_strlen(var);
	while ((*envp)[++i])
	{
		if (ft_strncmp((*envp)[i], var, len) ||
		((*envp)[i][len] != '=' && (*envp)[i][len]))
			new_env[j++] = (*envp)[i];
		else
			free ((*envp)[i]);
	}
	free(*envp);
	*envp = new_env;
	return (0);
}

static int	check_valid(char **argv, int *return_value, int *i)
{
	if (!is_valid(argv[*i]))
	{
		ft_printf_error("minishell: unset: `%s\': not a valid identifier\n",
			argv[(*i)++]);
		*return_value = 1;
		return (1);
	}
	return (0);
}

int	bi_unset(char **argv, char ***envp)
{
	int		i;
	int		return_value;
	char	*name;

	i = 1;
	return_value = 0;
	while (argv[i])
	{
		if (check_valid(argv, &return_value, &i) == 1)
			continue ;
		name = ft_strjoin("$", argv[i]);
		if (!name)
			return (-1);
		if (!ft_getenv(name, *envp) && ft_strchr(name, '='))
		{
			free(name);
			i++;
			continue ;
		}
		free(name);
		ft_remove(argv[i], envp, NULL);
		i++;
	}
	return (return_value);
}
