/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gsaile <gsaile@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/17 10:28:26 by gsaile            #+#    #+#             */
/*   Updated: 2023/01/24 16:04:54 by gsaile           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	bi_env(char **argv, char **envp)
{
	int	i;

	i = -1;
	if (argv[1])
	{
		ft_printf_error("env: no arguments needed\n");
		return (1);
	}
	while (envp[++i])
		if (ft_strchr(envp[i], '='))
			if (printf("%s\n", envp[i]) < 0)
				return (-1);
	return (0);
}
