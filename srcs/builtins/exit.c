/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gsaile <gsaile@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/18 12:04:19 by gsaile            #+#    #+#             */
/*   Updated: 2023/02/02 16:04:28 by gsaile           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static long long	bi_exit2(char **argv)
{
	if (!is_number_valid(argv[1]))
	{
		ft_printf_error("minishell: exit: %s: numeric argument required\n",
			argv[1]);
		exit(255);
	}
	return (ft_atol(argv[1]));
}

int	bi_exit(char **argv, char **envp)
{
	int			argc;
	long long	return_value;

	if (!get_ifs())
		ft_printf_error("exit\n");
	argc = 1;
	return_value = ft_atoi(get_les());
	while (argv[argc])
		argc++;
	if (argc >= 2)
		return_value = bi_exit2(argv);
	if (argc > 2)
	{
		ft_printf_error("minishell: exit: too many arguments\n");
		return (1);
	}
	free_all(envp);
	exit(return_value);
}
