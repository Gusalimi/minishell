/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_handler.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gsaile <gsaile@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/18 18:46:10 by lloison           #+#    #+#             */
/*   Updated: 2023/01/31 14:05:43 by gsaile           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	free_all(char **tab)
{
	int	i;

	i = 0;
	while (tab[i])
		free(tab[i++]);
	free(tab);
	return (-1);
}

char	**create_env(char **envp)
{
	int		len;
	int		i;
	char	**nenv;

	len = 0;
	i = -1;
	if (!envp)
		return (NULL);
	while (envp[len])
		len++;
	nenv = malloc((len + 1) * sizeof(char *));
	if (!nenv)
		return (NULL);
	while (envp[++i])
	{
		nenv[i] = ft_strdup(envp[i]);
		if (!nenv[i])
		{
			free_all(nenv);
			return (NULL);
		}
	}
	nenv[i] = 0;
	return (nenv);
}

char	*ft_getenv(char *var, char **envp)
{
	int		i;
	int		len;

	if (!var || !envp)
		return (NULL);
	var++;
	len = 0;
	if (var[0] == '?')
		return (get_les());
	if (!ft_isalnum(var[0]) && var[0] != '_')
		return ("$");
	if (ft_isdigit(var[0]))
		return (NULL);
	while (var[len] && (ft_isalnum(var[len]) || var[len] == '_'))
		len++;
	i = 0;
	while (envp[i])
	{
		if (ft_strncmp(envp[i], var, len) == 0
			&& (int)(ft_strlen(envp[i])) > len && envp[i][len] == '=')
			return (envp[i] + len + 1);
		i++;
	}
	return (NULL);
}
