/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gsaile <gsaile@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/24 16:14:43 by gsaile            #+#    #+#             */
/*   Updated: 2023/01/24 16:18:15 by gsaile           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
	return (1);
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
	while ((*envp)[++i])
		new_env[i] = (*envp)[i];
	new_env[i++] = ft_strdup(var);
	if (!new_env[i - 1])
	{
		free(new_env);
		return (free_all(*envp));
	}
	new_env[i] = 0;
	free(*envp);
	*envp = new_env;
	return (0);
}
