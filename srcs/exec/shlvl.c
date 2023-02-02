/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shlvl.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gsaile <gsaile@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/30 17:54:18 by gsaile            #+#    #+#             */
/*   Updated: 2023/01/31 14:46:53 by gsaile           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	shlvl2(int oshlvl_i, char ***envp)
{
	char	*nshlvl_s;
	char	*var;

	nshlvl_s = ft_itoa(oshlvl_i);
	if (!nshlvl_s)
		return (-1);
	var = ft_strjoin("SHLVL=", nshlvl_s);
	if (!var)
	{
		free(nshlvl_s);
		return (-1);
	}
	if (bi_export((char *[3]){"export", var, NULL}, envp, NULL) == -1)
	{
		free(var);
		free(nshlvl_s);
		return (-1);
	}
	free(var);
	free(nshlvl_s);
	return (0);
}

int	shlvl(char ***envp)
{
	char	*oshlvl_s;
	int		oshlvl_i;

	oshlvl_s = ft_getenv("$SHLVL", *envp);
	if (!oshlvl_s || has_alpha(oshlvl_s))
		return (bi_export((char *[3]){"export", "SHLVL=1", NULL}, envp, NULL));
	oshlvl_i = ft_atoi(oshlvl_s);
	oshlvl_i++;
	if (oshlvl_i < 0)
	{
		return (bi_export((char *[3]){"export", "SHLVL=0", NULL}, envp, NULL));
		return (0);
	}
	if (oshlvl_i >= 2000000)
	{
		ft_printf_error("minishell: warning: shell level (%d) %s", oshlvl_i,
			"too high, resetting to 1\n");
		return (bi_export((char *[3]){"export", "SHLVL=1", NULL}, envp, NULL));
	}
	return (shlvl2(oshlvl_i, envp));
}
