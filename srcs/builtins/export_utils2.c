/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lloison < lloison@student.42mulhouse.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/24 16:14:43 by gsaile            #+#    #+#             */
/*   Updated: 2023/01/30 18:27:14 by lloison          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*find_plus(char *str)
{
	size_t	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == '+' && str[i + 1] == '=')
			return (str + i);
		i++;
	}
	if (str[i] == '+' && str[i + 1] == '=')
		return (str + i);
	return (0);
}

static int	add_append(char *var, int len, char ***envp)
{
	char	*tmp;
	char	*value;

	tmp = ft_strndup(var, len);
	if (!tmp)
		return (-1);
	value = ft_strjoin(tmp, find_plus(var) + 1);
	free(tmp);
	if (!value)
		return (-1);
	if (add(value, envp) == -1)
		return (-1);
	free(value);
	return (0);
}

int	append(char *var, char ***envp)
{
	int		len;
	int		i;
	char	*tmp;
	char	*value;

	if (!find_plus(var))
		return (0);
	len = find_plus(var) - var;
	i = 0;
	while ((*envp)[i])
	{
		if (ft_strncmp((*envp)[i], var, len) == 0 && (*envp)[i][len] == '=')
		{
			value = find_plus(var) + 2;
			tmp = ft_strjoin((*envp)[i], value);
			free((*envp)[i]);
			(*envp)[i] = tmp;
			return (-1 * (!(*envp)[i]));
		}
		i++;
	}
	return (add_append(var, len, envp));
}

int	modify(char *var, char ***envp)
{
	int	len;
	int	i;

	if (!ft_strchr(var, '='))
		return (0);
	len = ft_strchr(var, '=') - var + 1;
	i = 0;
	while ((*envp)[i])
	{
		if (ft_strncmp((*envp)[i], var, len) == 0 ||
		(ft_strncmp((*envp)[i], var, len - 1) == 0 && (*envp)[i][len - 1] == 0))
		{
			free((*envp)[i]);
			(*envp)[i] = ft_strdup(var);
			if (!(*envp)[i])
				return (-1);
			return (0);
		}
		i++;
	}
	return (0);
}

int	add(char *var, char ***envp)
{
	int		len;
	int		i;
	char	**new_env;

	len = 0;
	i = -1;
	while ((*envp)[len])
		len++;
	new_env = malloc((len + 2) * sizeof(char *));
	if (!new_env)
		return (free_all(*envp));
	new_env[len + 1] = 0;
	while ((*envp)[++i])
		new_env[i] = (*envp)[i];
	new_env[i++] = ft_strdup(var);
	if (!new_env[i - 1])
	{
		free(new_env);
		return (free_all(*envp));
	}
	free(*envp);
	*envp = new_env;
	return (0);
}
