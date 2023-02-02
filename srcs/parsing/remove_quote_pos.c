/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   remove_quote_pos.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lloison < lloison@student.42mulhouse.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/01 11:35:52 by lloison           #+#    #+#             */
/*   Updated: 2023/02/01 11:36:09 by lloison          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	shift_left(char *s)
{
	int	i;

	i = 0;
	while (s[i])
	{
		s[i] = s[i + 1];
		i++;
	}
}

static t_bool	update_state(int *quote_number, int *quotes_pos,
	char *arg, int *quotes_i)
{
	(*quote_number)++;
	if (quotes_pos[*quotes_i] == *quote_number)
	{
		shift_left(arg);
		(*quotes_i)++;
		return (TRUE);
	}
	return (FALSE);
}

void	remove_quotes_pos(char **args, int	*quotes_pos)
{
	int	i;
	int	j;
	int	quotes_i;
	int	quote_number;

	i = 0;
	quotes_i = 0;
	quote_number = 0;
	while (args[i])
	{
		j = 0;
		while (args[i][j])
		{
			if (args[i][j] == '"' || args[i][j] == '\'')
				if (update_state(&quote_number, quotes_pos,
						&(args[i][j]), &quotes_i))
					continue ;
			j++;
		}
		i++;
	}
}
