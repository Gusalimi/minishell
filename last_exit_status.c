/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   last_exit_status.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lloison < lloison@student.42mulhouse.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/18 14:45:00 by lloison           #+#    #+#             */
/*   Updated: 2023/01/18 15:07:30 by lloison          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	get_length(int n)
{
	int	count;

	count = 1;
	if (n < 0)
		count++;
	while (n / 10 != 0)
	{
		count++;
		n /= 10;
	}
	return (count);
}

static void	fill_les(char *les, int length,
	long long n, t_bool is_n)
{
	int	i;

	i = 0;
	while (i < 13)
	{
		les[i] = 0;
		i++;
	}
	i = 0;
	while (n / 10 > 0)
	{
		les[length - 1] = n % 10 + '0';
		length--;
		n /= 10;
	}
	les[length - 1] = n % 10 + '0';
	if (is_n == 1)
		les[0] = '-';
}

//caller is either getter(0) or setter(1)
char	*handle_les(int caller, int exit_status)
{
	static char	les[12] = "0";
	int			i;
	int			length;
	long long	n;
	t_bool		is_n;

	if (caller == 0)
		return (les);
	else if (caller != 1)
		return (NULL);
	i = 0;
	n = exit_status;
	is_n = false;
	length = get_length(n);
	if (n < 0)
	{
		n *= -1;
		is_n = TRUE;
	}
	fill_les(les, length, n, is_n);
	return (NULL);
}

char	*get_les(void)
{
	return (handle_les(0, 0));
}

void	set_les(int exit_status)
{
	handle_les(1, exit_status);
}
