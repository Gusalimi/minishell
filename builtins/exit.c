/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gsaile <gsaile@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/18 12:04:19 by gsaile            #+#    #+#             */
/*   Updated: 2023/01/24 16:22:38 by gsaile           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	bi_exit(char **argv, char **envp)
{
	int			argc;
	long long	return_value;

	ft_printf_error("exit\n");
	argc = 1;
	return_value = 0;
	while (argv[argc])
		argc++;
	if (argc >= 2)
	{
		if (!is_number_valid(argv[1]))
		{
			ft_printf_error("minishell: exit: %s: numeric argument required\n",
				argv[1]);
			exit(255);
		}
		return_value = ft_atol(argv[1]);
	}
	if (argc > 2)
	{
		ft_printf_error("minishell: exit: too many arguments\n");
		return (1);
	}
	free_all(envp);
	exit(return_value);
}
