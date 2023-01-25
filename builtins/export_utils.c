/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gsaile <gsaile@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/24 13:43:37 by gsaile            #+#    #+#             */
/*   Updated: 2023/01/24 16:17:44 by gsaile           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	swap(char **a, char **b)
{
	char	*temp;

	temp = *a;
	*a = *b;
	*b = temp;
}

static void	sort_env(char ***env)
{
	int	i;
	int	j;
	int	n;

	n = 0;
	i = -1;
	while ((*env)[n])
		n++;
	while (++i < n - 1)
	{
		j = -1;
		while (++j < n - i - 1)
			if (ft_strcmp((*env)[j], (*env)[j + 1]) > 0)
				swap(&(*env)[j], &(*env)[j + 1]);
	}
}

static char	**copy_tab(char **tab)
{
	char	**ntab;
	int		size;
	int		i;

	size = 0;
	i = 0;
	while (tab[size])
		size++;
	ntab = malloc((size + 1) * sizeof(char *));
	if (!ntab)
		return (NULL);
	while (tab[i])
	{
		ntab[i] = ft_strdup(tab[i]);
		if (!ntab[i])
		{
			free_all(ntab);
			return (NULL);
		}
		i++;
	}
	ntab[i] = 0;
	return (ntab);
}

static int	print_env_export2(char **env, int i, int fd)
{
	int	j;

	j = 0;
	if (write(fd, "declare -x ", 11) == -1)
		return (-1);
	while (env[i][j] && env[i][j] != '=')
		if (write(fd, &env[i][j++], 1) == -1)
			return (-1);
	if (env[i][j])
	{
		if (write(fd, &env[i][j++], 1) == -1)
			return (-1);
		if (write(fd, "\"", 1) == -1)
			return (-1);
		while (env[i][j])
			if (write(fd, &env[i][j++], 1) == -1)
				return (-1);
		if (write(fd, "\"", 1) == -1)
			return (-1);
	}
	if (write(fd, "\n", 1) == -1)
		return (-1);
	return (0);
}

int	print_env_export(char **envp, int fd)
{
	char	**env;
	int		i;

	i = -1;
	env = copy_tab(envp);
	if (!env)
		return (-1);
	sort_env(&env);
	while (env[++i])
		if (ft_strncmp(env[i], "_=", 2) != 0)
			if (print_env_export2(env, i, fd) == -1)
				return (-1);
	free(env);
	return (0);
}
